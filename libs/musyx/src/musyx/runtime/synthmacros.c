#include "musyx/assert.h"
#include "musyx/hardware.h"
#include "musyx/macros.h"
#include "musyx/seq.h"
#include "musyx/snd.h"
#include "musyx/synth.h"
#include "musyx/synth_dbtab.h"
#include "musyx/synthdata.h"
#include "musyx/sal.h"

#include <float.h>
#include <string.h>

static u8 DebugMacroSteps;

static SYNTH_VOICE* macActiveMacroRoot;
static SYNTH_VOICE* macTimeQueueRoot;
static u64 macRealTime;

static void TimeQueueAdd(SYNTH_VOICE* svoice);

void macMakeActive(SYNTH_VOICE* svoice);

void macSetExternalKeyoff(SYNTH_VOICE* svoice);

static void DoSetPitch(SYNTH_VOICE* svoice);

static int SendSingleKeyOff(u32 voiceid) {
  u32 i;

  if (voiceid != 0xFFFFFFFF) {

    i = voiceid & 0xFF;

    if (voiceid == synthVoice[i].id) {

      macSetExternalKeyoff(&synthVoice[i]);

      return 0;
    }
  }

  return -1;
}

static u32 ExecuteTrap(SYNTH_VOICE* svoice, u8 trapType) {
  if (svoice->trapEventAny != 0 && svoice->trapEventAddr[trapType] != NULL) {
    svoice->curAddr = svoice->trapEventCurAddr[trapType];
    svoice->addr = svoice->trapEventAddr[trapType];
    svoice->trapEventAddr[trapType] = NULL;
    macMakeActive(svoice);
    return 1;
  }

  return 0;
}

static u32 HasHWEventTrap(SYNTH_VOICE* svoice) {
  if (svoice->trapEventAny != '\0') {
    return svoice->trapEventAddr[1] != NULL;
  }
  return 0;
}

static void CheckHWEventTrap(SYNTH_VOICE* svoice) {
  if ((svoice->cFlags & SYNTH_CFLAG_START_SAMPLE) == 0 && !hwIsActive(svoice->id & 0xff)) {
    ExecuteTrap(svoice, 1);
  }
}

static u32 mcmdWait(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u32 w;
  u32 ms;

  if ((ms = (u16)(cstep->para[1] >> 0x10))) {
    if (((u8)(cstep->para[0] >> 8) & 1)) {
      if (svoice->cFlags & SYNTH_CFLAG_KEY_OFF) {
        if (!(svoice->cFlags & SYNTH_CFLAG_PEDAL_HELD)) {
          return 0;
        }
        svoice->cFlags |= SYNTH_CFLAG_KEY_OFF_HELD;
      }
      svoice->cFlags |= SYNTH_CFLAG_WAIT_KEY_OFF;
    } else {
      svoice->cFlags &= ~SYNTH_CFLAG_WAIT_KEY_OFF;
    }

    if (((u8)(cstep->para[0] >> 0x18) & 1)) {
      if (!(svoice->cFlags & SYNTH_CFLAG_START_SAMPLE) && !hwIsActive(svoice->id & 0xFF)) {
        return 0;
      }
      svoice->cFlags |= SYNTH_CFLAG_WAIT_SAMPLE_END;
    } else {
      svoice->cFlags &= ~SYNTH_CFLAG_WAIT_SAMPLE_END;
    }

    if (((u8)(cstep->para[0] >> 0x10)) & 1) {
      ms = sndRand() % ms;
    }

    if (ms != 0xFFFF) {
      if ((w = ((u8)(cstep->para[1] >> 0x8) & 1) != 0)) {
        sndConvertMs(&ms);
      } else {
        sndConvertTicks(&ms, svoice);
      }

      if (w != 0) {
        if ((u8)cstep->para[1] & 1) {
          svoice->wait = svoice->macStartTime + ms;
        } else {
          svoice->wait = macRealTime + ms;
        }
      } else {
        if ((u8)cstep->para[1] & 1) {
          svoice->wait = ms;
        } else {
          svoice->wait = svoice->waitTime + ms;
        }
      }

      if (!(svoice->wait > macRealTime)) {
        svoice->waitTime = svoice->wait;
        svoice->wait = 0;
      }
    } else {
      svoice->wait = -1;
    }

    if (svoice->wait != 0) {
      if (svoice->wait != -1) {
        TimeQueueAdd(svoice);
      }
      macMakeInactive(svoice, MAC_STATE_YIELDED);
      return 1;
    }
  }

  return 0;
}

static u32 mcmdWaitMs(SYNTH_VOICE* svoice, MSTEP* cstep) {
  *((u8*)cstep->para + 6) = 1;
  return mcmdWait(svoice, cstep);
}

static u32 mcmdEndOfMacro(SYNTH_VOICE* svoice) {
  vidRemoveVoiceReferences(svoice);
  voiceFree(svoice);
  return 1;
}

static u32 mcmdStop(SYNTH_VOICE* svoice) { return mcmdEndOfMacro(svoice); }

static u32 mcmdReturn(SYNTH_VOICE* svoice) {
  if (svoice->callStackEntryNum != 0) {
    svoice->addr = svoice->callStack[svoice->callStackIndex].addr;
    svoice->curAddr = svoice->callStack[svoice->callStackIndex].curAddr;
    svoice->callStackIndex = (svoice->callStackIndex - 1) & 3;
    --svoice->callStackEntryNum;
  }
  return 0;
}

static void mcmdIfKey(SYNTH_VOICE* svoice, MSTEP* cstep) {
  MSTEP* addr;
  if (svoice->curNote < ((u8)(cstep->para[0] >> 8))) {
    return;
  }

  if ((addr = (MSTEP*)dataGetMacro((cstep->para[0] >> 0x10))) != NULL) {
    svoice->addr = addr;
    svoice->curAddr = addr + (u16)cstep->para[1];
  }
}

static void mcmdIfVelocity(SYNTH_VOICE* svoice, MSTEP* cstep) {
  MSTEP* addr;
  if (((u8)(svoice->volume >> 0x10)) < (u8)(cstep->para[0] >> 8)) {
    return;
  }

  if ((addr = (MSTEP*)dataGetMacro(cstep->para[0] >> 0x10))) {
    svoice->addr = addr;
    svoice->curAddr = addr + (u16)cstep->para[1];
  }
}

static void mcmdIfModulation(SYNTH_VOICE* svoice, MSTEP* cstep) {
  MSTEP* addr;
  u8 mod;

  if (svoice->midi == 0xff) {
    return;
  }
  mod = inpGetModulation(svoice) >> 7;
  if (mod < (u8)(cstep->para[0] >> 8)) {
    return;
  }

  if ((addr = (MSTEP*)dataGetMacro(cstep->para[0] >> 0x10))) {
    svoice->addr = addr;
    svoice->curAddr = addr + (u16)(cstep->para[1]);
  }
}

static void mcmdIfRandom(SYNTH_VOICE* svoice, MSTEP* cstep) {
  MSTEP* addr;
  if ((u8)sndRand() < (u8)(cstep->para[0] >> 8)) {
    return;
  }

  if ((addr = (MSTEP*)dataGetMacro(cstep->para[0] >> 0x10))) {
    svoice->addr = addr;
    svoice->curAddr = addr + (u16)cstep->para[1];
  }
}

static u32 mcmdGoto(SYNTH_VOICE* svoice, MSTEP* cstep) {
  MSTEP* addr;
  if ((addr = (MSTEP*)dataGetMacro(cstep->para[0] >> 0x10)) != NULL) {
    svoice->addr = addr;
    svoice->curAddr = addr + (u16)cstep->para[1];
    return 0;
  }

  return mcmdEndOfMacro(svoice);
}

static u32 mcmdGosub(SYNTH_VOICE* svoice, MSTEP* cstep) {
  MSTEP* addr;
  if ((addr = (MSTEP*)dataGetMacro((u16)(cstep->para[0] >> 0x10))) != NULL) {
    svoice->callStackIndex = (svoice->callStackIndex + 1) & 3;
    svoice->callStack[svoice->callStackIndex].addr = svoice->addr;
    svoice->callStack[svoice->callStackIndex].curAddr = svoice->curAddr;
    if (++svoice->callStackEntryNum > 4) {
      svoice->callStackEntryNum = 4;
    }

    svoice->addr = addr;
    svoice->curAddr = addr + (u16)cstep->para[1];
    return 0;
  }

  return mcmdEndOfMacro(svoice);
}

static void mcmdTrapEvent(SYNTH_VOICE* svoice, MSTEP* cstep) {
  MSTEP* addr;
  u8 t;
  if ((addr = (MSTEP*)dataGetMacro(cstep->para[0] >> 0x10)) != NULL) {
    t = (u8)(cstep->para[0] >> 8);
    svoice->trapEventAddr[t] = addr;
    svoice->trapEventCurAddr[t] = addr + (u16)cstep->para[1];
    svoice->trapEventAny = 1;
    if (t == 0 && (svoice->cFlags & (SYNTH_CFLAG_KEY_OFF | SYNTH_CFLAG_PEDAL_HELD)) ==
                      (SYNTH_CFLAG_KEY_OFF | SYNTH_CFLAG_PEDAL_HELD)) {
      svoice->cFlags |= SYNTH_CFLAG_KEY_OFF_HELD;
    }
  }
}

static void mcmdUntrapEvent(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u8 i;
  svoice->trapEventAddr[(u8)(cstep->para[0] >> 8)] = 0;

  for (i = 0; i < 3; ++i) {
    if (svoice->trapEventAddr[i] != NULL) {
      return;
    }
  }

  svoice->trapEventAny = 0;
}

static void mcmdLoop(SYNTH_VOICE* svoice, MSTEP* cstep) {

  if (svoice->loop == 0) {
    if ((u8)(cstep->para[0] >> 16) & 1) {
      svoice->loop = sndRand() % (u16)(cstep->para[1] >> 16);
    } else {
      svoice->loop = (cstep->para[1] >> 16);
    }

    if (svoice->loop == 0xFFFF) {
      goto skip;
    }
    ++svoice->loop;
  } else if (svoice->loop == 0xFFFF) {
    goto skip;
  }

  if (--svoice->loop == 0) {
    return;
  }
skip:
  if (((u8)(cstep->para[0] >> 8) & 1) != 0 &&
      (svoice->cFlags & (SYNTH_CFLAG_KEY_OFF | SYNTH_CFLAG_PEDAL_HELD)) == SYNTH_CFLAG_KEY_OFF) {
    svoice->loop = 0;

  } else if (((u8)(cstep->para[0] >> 0x18) & 1) &&
             (svoice->cFlags & SYNTH_CFLAG_START_SAMPLE) == 0 &&
             !hwIsActive(svoice->id & 0xFF)) {
    svoice->loop = 0;
  } else {
    svoice->curAddr = svoice->addr + ((u16)cstep->para[1]);
  }
}

static void mcmdPlayMacro(SYNTH_VOICE* svoice, MSTEP* cstep) {
  s32 key;
  u32 new_child;

  key = ((u32)svoice->orgNote + (s8)(u8)(cstep->para[0] >> 8));
  key = (key < 0) ? 0 : key > 0x7f ? 0x7f : key;

  if (svoice->fxFlag != 0) {
    key |= 0x80;
  }

  svoice->block = 1;
  new_child = macStart((u16)(cstep->para[0] >> 0x10), (u8)(cstep->para[1] >> 0x10),
                       (u8)(cstep->para[1] >> 0x18), svoice->allocId, key,
                       (u8)(svoice->volume >> 0x10), (u8)(svoice->panning[0] >> 0x10), svoice->midi,
                       svoice->midiSet, svoice->section, (u16)cstep->para[1], (u16)svoice->track, 0,
                       svoice->vGroup, svoice->studio, svoice->itdMode == 0);
  svoice->block = 0;
  if (new_child != 0xFFFFFFFF) {
    svoice->lastVID = synthVoice[(u8)new_child].vidList->vid;
    synthVoice[(u8)new_child].parent = svoice->id;
    if (svoice->child != -1) {
      synthVoice[(u8)new_child].child = svoice->child;
      synthVoice[(u8)svoice->child].parent = new_child;
    }
    svoice->child = new_child;
    if (svoice->fxFlag != 0) {
      synthFXCloneMidiSetup(&synthVoice[(u8)new_child], svoice);
    }
  } else {
    svoice->lastVID = 0xFFFFFFFF;
  }
}

static void mcmdSendKeyOff(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u32 voiceid;
  u32 i;

  voiceid = (svoice->orgNote + (u8)(cstep->para[0] >> 8)) << 8;
  voiceid |= ((u16)(cstep->para[0] >> 16)) << 16;
  for (i = 0; i < synthInfo.voiceNum; ++i) {
    if (synthVoice[i].id == (voiceid | i)) {
      SendSingleKeyOff(voiceid | i);
    }
  }
}

static void mcmdAddAgeCounter(SYNTH_VOICE* svoice, MSTEP* cstep) {
  s16 step;
  s32 age;

  step = (u16)(cstep->para[0] >> 16);
  age = (svoice->age >> 15) + step;

  if (age < 0) {
    svoice->age = 0;
  } else if (age > 0xFFFF) {
    svoice->age = 0x7fff8000;
  } else {
    svoice->age = age * 0x8000;
  }

  hwSetPriority(svoice->id & 0xFF, ((u32)svoice->prio << 24) | ((u32)svoice->age >> 15));
}

static void mcmdSetAgeCounter(SYNTH_VOICE* svoice, MSTEP* cstep) {
  svoice->age = (u16)(cstep->para[0] >> 0x10) << 0xf;
  hwSetPriority(svoice->id & 0xff, (u32)svoice->prio << 0x18 | svoice->age >> 0xf);
}

static void mcmdSetAgeCounterSpeed(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u32 time = cstep->para[1];
  if (time != 0) {
    svoice->ageSpeed = (svoice->age >> 8) / time;
  } else {
    svoice->ageSpeed = 0;
  }
}
static void mcmdSetAgeCounterByVolume(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u32 age;

  age = (((u8)(svoice->volume >> 16) * (u16)cstep->para[1]) >> 7) + (u16)(cstep->para[0] >> 16);
  svoice->age = age > 60000 ? 0x75300000 : age * 0x8000;
  hwSetPriority(svoice->id & 0xff, (u32)svoice->prio << 0x18 | svoice->age >> 0xf);
}

static void mcmdAddPriority(SYNTH_VOICE* svoice, MSTEP* cstep) {
  s16 add;
  s16 prio;
  add = (u16)(cstep->para[0] >> 16);
  prio = svoice->prio + add;
  prio = (prio < 0) ? 0 : (prio > 0xFF) ? 0xFF : prio;

  voiceSetPriority(svoice, prio);
}

static void mcmdSetPriority(SYNTH_VOICE* svoice, MSTEP* cstep) {
  voiceSetPriority(svoice, cstep->para[0] >> 8);
}

static void mcmdSendFlag(MSTEP* cstep) {
  synthGlobalVariable[(u8)(cstep->para[0] >> 8)] = (u8)(cstep->para[0] >> 16);
}

static void mcmdSetPitchWheelRange(SYNTH_VOICE* svoice, MSTEP* cstep) {
  svoice->pbLowerKeyRange = (u8)(cstep->para[0] >> 0x10);
  svoice->pbUpperKeyRange = (u8)(cstep->para[0] >> 8);
}

static u32 mcmdSetKey(SYNTH_VOICE* svoice, MSTEP* cstep) {
  svoice->curNote = (u8)(cstep->para[0] >> 8) & 0x7f;
  svoice->curDetune = (s8)(cstep->para[0] >> 0x10);
  if (voiceIsLastStarted(svoice) != 0) {
    inpSetMidiLastNote(svoice->midi, svoice->midiSet, svoice->curNote & 0xff);
  }
  cstep->para[0] = 4;
  return mcmdWait(svoice, cstep);
}

static u32 mcmdAddKey(SYNTH_VOICE* svoice, MSTEP* cstep) {
  if ((u8)(cstep->para[0] >> 0x18) == 0) {
    svoice->curNote += (s8)(u8)(cstep->para[0] >> 8);
  } else {
    svoice->curNote = (u16)svoice->orgNote + (s16)(s8)(u8)(cstep->para[0] >> 8);
  }

  svoice->curNote = (s16)svoice->curNote < 0 ? 0 : svoice->curNote > 0x7f ? 0x7f : svoice->curNote;
  svoice->curDetune = (s8)(cstep->para[0] >> 0x10);

  if (voiceIsLastStarted(svoice) != 0) {
    inpSetMidiLastNote(svoice->midi, svoice->midiSet, svoice->curNote);
  }
  cstep->para[0] = 4;
  return mcmdWait(svoice, cstep);
}

static u32 mcmdLastKey(SYNTH_VOICE* svoice, MSTEP* cstep) {
  svoice->curNote = svoice->lastNote + (s8)(u8)(cstep->para[0] >> 8);
  svoice->curNote = (s16)svoice->curNote < 0 ? 0 : svoice->curNote > 0x7f ? 0x7f : svoice->curNote;
  svoice->curDetune = (s8)(cstep->para[0] >> 16);
  if (svoice->midi != 0xFF) {
    inpSetMidiLastNote(svoice->midi, svoice->midiSet, svoice->curNote);
  }
  cstep->para[0] = 4;

  return mcmdWait(svoice, cstep);
}

static void mcmdStartSample(SYNTH_VOICE* svoice, MSTEP* cstep) {
  static SAMPLE_INFO newsmp;
  u16 smp;
  smp = cstep->para[0] >> 8;

  if (dataGetSample(smp, &newsmp) != 0) {
    return;
  }
  switch ((u8)(cstep->para[0] >> 0x18)) {
  case 0:
    newsmp.offset = cstep->para[1];
    break;
  case 1:
    newsmp.offset = ((u8)(0x7f - (svoice->volume >> 0x10)) * (u32)cstep->para[1]) / 0x7f;

    break;
  case 2:
    newsmp.offset = ((u8)((svoice->volume >> 0x10)) * (u32)cstep->para[1]) / 0x7f;
    break;
  default:
    newsmp.offset = 0;
    break;
  }

  if (newsmp.offset >= newsmp.length) {
    newsmp.offset = newsmp.length - 1;
  }

  hwInitSamplePlayback(svoice->id & 0xFF, smp, &newsmp,
                       (svoice->cFlags & SYNTH_CFLAG_ADSR_SET) == 0,
                       ((u32)svoice->prio << 24) | ((u32)svoice->age >> 15), svoice->id,
                       (svoice->cFlags & SYNTH_CFLAG_SRC_SET) == 0, svoice->itdMode);

  svoice->sInfo = newsmp.info;
#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 1)
  svoice->sampleId = smp;
#endif

  if (svoice->playFrq != -1) {
    DoSetPitch(svoice);
  }
  svoice->cFlags |= SYNTH_CFLAG_START_SAMPLE;
  synthKeyStateUpdate(svoice);
}

static void mcmdStopSample(SYNTH_VOICE* svoice) { hwBreak(svoice->id & 0xFF); }
static void mcmdKeyOff(SYNTH_VOICE* svoice) {
  svoice->cFlags |= SYNTH_CFLAG_KEY_OFF_PENDING;
  synthKeyStateUpdate(svoice);
}

static void mcmdSetMod2Vibrato(SYNTH_VOICE* svoice, MSTEP* cstep) {
  svoice->vibModAddScale = (s8)(cstep->para[0] >> 8) << 8;
  if (svoice->vibModAddScale >= 0) {
    svoice->vibModAddScale += ((s16)(s8)(cstep->para[0] >> 0x10) << 8) / 100;

  } else {
    svoice->vibModAddScale -= ((s16)(s8)(cstep->para[0] >> 0x10) << 8) / 100;
  }
}

static void mcmdVibrato(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u32 time;
  s8 kr;
  s8 cr;

  if ((u8)(cstep->para[0] >> 0x18) & 3) {
    svoice->cFlags |= SYNTH_CFLAG_VIBRATO_MOD_WHEEL;
  } else {
    svoice->cFlags &= ~SYNTH_CFLAG_VIBRATO_MOD_WHEEL;
  }

  time = (u16)(cstep->para[1] >> 0x10);
  if ((u8)(cstep->para[1] >> 8) & 1) {
    sndConvertMs(&time);
  } else {
    sndConvertTicks(&time, svoice);
  }

  if (time) {
    svoice->cFlags |= SYNTH_CFLAG_VIBRATO;
    svoice->vibPeriod = time;

    kr = (s8)(cstep->para[0] >> 8);
    cr = (s8)(cstep->para[0] >> 16);

    if (kr < 0) {
      if (cr < 0) {
        svoice->vibCentRange = -cr;
      } else {
        svoice->vibCentRange = cr;
      }

      svoice->vibKeyRange = -kr;
      svoice->vibCurTime = svoice->vibPeriod / 2;
    } else {
      if (cr < 0) {
        if (kr == 0) {
          svoice->vibCentRange = -cr;
          svoice->vibCurTime = svoice->vibPeriod / 2;
        } else {
          --kr;
          svoice->vibCentRange = 100 - cr;
          svoice->vibCurTime = 0;
        }
      } else {
        svoice->vibCentRange = cr;
        svoice->vibCurTime = 0;
      }
      svoice->vibKeyRange = kr;
    }
  } else {
    svoice->cFlags &= ~SYNTH_CFLAG_VIBRATO;
  }
}

static void mcmdSetupLFO(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u32 time;
  u32 phase;
  u8 n;

  n = (u8)(cstep->para[0] >> 8);
  time = (u16)(cstep->para[0] >> 0x10);
  sndConvertMs(&time);
  if (svoice->lfo[n].period != 0) {
    phase = (u16)cstep->para[1];
    sndConvertMs(&phase);
    svoice->lfo[n].time = phase;
  }
  svoice->lfo[n].period = time;
}

static void DoSetPitch(SYNTH_VOICE* svoice) {
  u32 f;
  u32 of;
  u32 i;
  u32 frq;
  u32 ofrq;
  u32 no;
  s32 key;
  u8 oKey;
  static u16 kf[13] = {
      4096, 4339, 4597, 4871, 5160, 5467, 5792, 6137, 6502, 6888, 7298, 7732, 8192,
  };

  frq = svoice->playFrq & 0xFFFFFF;
  ofrq = svoice->sInfo & 0xFFFFFF;

  if (ofrq == frq) {
    svoice->curNote = (u8)(svoice->sInfo >> 24);
    svoice->curDetune = 0;
  } else if (ofrq < frq) {
    f = (frq << 12) / ofrq;
    of = f >> 12;

    for (no = 0; no < 11; no++) {
      if (of < (1 << (no + 1))) {
        break;
      }
    }

    f /= (1 << no);

    // BUG (here and below): when f is exactly 4096 (an exact octave ratio), f > kf[0] fails too and the
    // search runs below kf[0].
    for (i = 11;; i--) {
      if (f > kf[i]) {
        break;
      }
    }

    svoice->curNote = (svoice->sInfo >> 24) + (no * 12) + i;
    svoice->curDetune = ((f - kf[i]) * 100) / (kf[i + 1] - kf[i]);
  } else {
    f = (ofrq << 12) / frq;
    of = f >> 12;

    for (no = 0; no < 11; no++) {
      if (of < (1 << (no + 1))) {
        break;
      }
    }

    f /= (1 << no);

    for (i = 11;; i--) {
      if (f > kf[i]) {
        break;
      }
    }

    key = i + (no * 12);
    oKey = (svoice->sInfo >> 24);
    if (key > oKey) {
      svoice->curNote = svoice->curDetune = 0;
    } else {
      svoice->curNote = oKey - key;
      svoice->curDetune = ((kf[i] - f) * 100) / (kf[i + 1] - kf[i]);
    }
  }
}

static void mcmdSetPitch(SYNTH_VOICE* svoice, MSTEP* cstep) {
  svoice->playFrq = (u32)(cstep->para[0] >> 8);
  svoice->playFrq |= (u8)cstep->para[1];
  if (svoice->sInfo != -1) {
    DoSetPitch(svoice);
  }
}

static void mcmdSetADSR(SYNTH_VOICE* svoice, MSTEP* cstep) {
  ADSR_INFO adsr;
  ADSR_INFO* adsr_ptr;
  s32 ascale;
  s32 dscale;
  f32 sScale;

  if ((adsr_ptr = dataGetCurve(cstep->para[0] >> 8)) != NULL) {
    if (!(u8)(cstep->para[0] >> 24)) {
      adsr.data.linear.atime = adsr_ptr->data.linear.atime >> 8 | adsr_ptr->data.linear.atime << 8;
      adsr.data.linear.dtime = adsr_ptr->data.linear.dtime >> 8 | adsr_ptr->data.linear.dtime << 8;
      adsr.data.linear.slevel = adsr_ptr->data.linear.slevel >> 8 | adsr_ptr->data.linear.slevel
                                                                        << 8;
      adsr.data.linear.rtime = adsr_ptr->data.linear.rtime >> 8 | adsr_ptr->data.linear.rtime << 8;
      hwSetADSR(svoice->id & 0xFF, &adsr, FALSE);
    } else {
      sScale =
          dspDLSVolTab[(u16)(adsr_ptr->data.dls.slevel >> 8 | adsr_ptr->data.dls.slevel << 8) >> 5];
      adsr.data.dls.atime =
          ((u8*)&adsr_ptr->data.dls.atime)[0] << 0 | ((u8*)&adsr_ptr->data.dls.atime)[1] << 8 |
          ((u8*)&adsr_ptr->data.dls.atime)[2] << 16 | ((u8*)&adsr_ptr->data.dls.atime)[3] << 24;
      adsr.data.dls.dtime =
          ((u8*)&adsr_ptr->data.dls.dtime)[0] << 0 | ((u8*)&adsr_ptr->data.dls.dtime)[1] << 8 |
          ((u8*)&adsr_ptr->data.dls.dtime)[2] << 16 | ((u8*)&adsr_ptr->data.dls.dtime)[3] << 24;
      adsr.data.dls.slevel = 4096.f * sScale;
      adsr.data.dls.rtime = adsr_ptr->data.dls.rtime >> 8 | adsr_ptr->data.dls.rtime << 8;
      ascale =
          ((u8*)&adsr_ptr->data.dls.ascale)[0] << 0 | ((u8*)&adsr_ptr->data.dls.ascale)[1] << 8 |
          ((u8*)&adsr_ptr->data.dls.ascale)[2] << 16 | ((u8*)&adsr_ptr->data.dls.ascale)[3] << 24;

      dscale =
          ((u8*)&adsr_ptr->data.dls.dscale)[0] << 0 | ((u8*)&adsr_ptr->data.dls.dscale)[1] << 8 |
          ((u8*)&adsr_ptr->data.dls.dscale)[2] << 16 | ((u8*)&adsr_ptr->data.dls.dscale)[3] << 24;

      if (ascale != 0x80000000) {
        adsr.data.dls.atime += (s32)(FLT_EPSILON * svoice->orgVolume * ascale); // FLT_EPSILON = 2^-23, used as a scale factor
      }

      if (dscale != 0x80000000) {
        adsr.data.dls.dtime += (s32)(0.0078125f * svoice->orgNote * dscale);
      }

      hwSetADSR(svoice->id & 0xFF, &adsr, TRUE);
    }

    svoice->cFlags |= SYNTH_CFLAG_ADSR_SET;
  }
}

static s32 midi2TimeTab[128] = {
    0,      10,     20,     30,     40,     50,     60,     70,     80,     90,     100,    110,
    110,    120,    130,    140,    150,    160,    170,    190,    200,    220,    230,    250,
    270,    290,    310,    330,    350,    380,    410,    440,    470,    500,    540,    580,
    620,    660,    710,    760,    820,    880,    940,    1000,   1000,   1100,   1200,   1300,
    1400,   1500,   1600,   1700,   1800,   2000,   2100,   2300,   2400,   2600,   2800,   3000,
    3200,   3500,   3700,   4000,   4300,   4600,   4900,   5300,   5700,   6100,   6500,   7000,
    7500,   8100,   8600,   9300,   9900,   10000,  11000,  12000,  13000,  14000,  15000,  16000,
    17000,  18000,  19000,  21000,  22000,  24000,  26000,  28000,  30000,  32000,  34000,  37000,
    39000,  42000,  45000,  49000,  50000,  55000,  60000,  65000,  70000,  75000,  80000,  85000,
    90000,  95000,  100000, 105000, 110000, 115000, 120000, 125000, 130000, 135000, 140000, 145000,
    150000, 155000, 160000, 165000, 170000, 175000, 180000, 0,
};

static void mcmdSetADSRFromCtrl(SYNTH_VOICE* svoice, MSTEP* cstep) {

  f32 sScale;
  ADSR_INFO adsr;

  sScale = dspDLSVolTab[inpGetMidiCtrl(cstep->para[0] >> 24, svoice->midi, svoice->midiSet) >> 7];
  adsr.data.dls.atime =
      midi2TimeTab[inpGetMidiCtrl(cstep->para[0] >> 8, svoice->midi, svoice->midiSet) >> 7];
  adsr.data.dls.dtime =
      midi2TimeTab[inpGetMidiCtrl(cstep->para[0] >> 16, svoice->midi, svoice->midiSet) >> 7];
  adsr.data.dls.slevel = 193 - dspScale2IndexTab[(u32)(1023.f * sScale)];
  adsr.data.dls.rtime =
      midi2TimeTab[inpGetMidiCtrl(cstep->para[1], svoice->midi, svoice->midiSet) >> 7];
  adsr.data.dls.ascale = -0x80000000;
  adsr.data.dls.dscale = -0x80000000;
  hwSetADSR((u8)svoice->id, &adsr, 2);
  svoice->cFlags |= SYNTH_CFLAG_ADSR_SET;
}

static void mcmdSetPitchADSR(SYNTH_VOICE* svoice, MSTEP* cstep) {
  ADSR_INFO adsr;
  ADSR_INFO* adsr_ptr;
  u32 sl;
  s32 ascale;
  s32 dscale;

  if ((adsr_ptr = dataGetCurve((cstep->para[0] >> 8))) == NULL) {
    return;
  }

  svoice->pitchADSRRange = ((s8)cstep->para[1] << 8);

  if (svoice->pitchADSRRange >= 0) {
    svoice->pitchADSRRange += ((s16)(s8)(cstep->para[1] >> 8) << 8) / 100;
  } else {
    svoice->pitchADSRRange -= ((s16)(s8)(cstep->para[1] >> 8) << 8) / 100;
  }

  adsr.data.dls.atime =
      (((u8*)&adsr_ptr->data.dls.atime)[0] << 0) | (((u8*)&adsr_ptr->data.dls.atime)[1] << 8) |
      (((u8*)&adsr_ptr->data.dls.atime)[2] << 16) | (((u8*)&adsr_ptr->data.dls.atime)[3] << 24);
  adsr.data.dls.dtime =
      (((u8*)&adsr_ptr->data.dls.dtime)[0] << 0) | (((u8*)&adsr_ptr->data.dls.dtime)[1] << 8) |
      (((u8*)&adsr_ptr->data.dls.dtime)[2] << 16) | (((u8*)&adsr_ptr->data.dls.dtime)[3] << 24);

  adsr.data.dls.slevel = (adsr_ptr->data.dls.slevel >> 8) | (adsr_ptr->data.dls.slevel << 8);
  adsr.data.dls.rtime = (adsr_ptr->data.dls.rtime >> 8) | (adsr_ptr->data.dls.rtime << 8);
  ascale =
      (((u8*)&adsr_ptr->data.dls.ascale)[0] << 0) | (((u8*)&adsr_ptr->data.dls.ascale)[1] << 8) |
      (((u8*)&adsr_ptr->data.dls.ascale)[2] << 16) | (((u8*)&adsr_ptr->data.dls.ascale)[3] << 24);
  dscale =
      (((u8*)&adsr_ptr->data.dls.dscale)[0] << 0) | (((u8*)&adsr_ptr->data.dls.dscale)[1] << 8) |
      (((u8*)&adsr_ptr->data.dls.dscale)[2] << 16) | (((u8*)&adsr_ptr->data.dls.dscale)[3] << 24);

  if (ascale != 0x80000000) {
    adsr.data.dls.atime += (s32)((FLT_EPSILON * svoice->orgVolume) * (f32)ascale); // FLT_EPSILON = 2^-23, used as a scale factor
  }
  if (dscale != 0x80000000) {
    adsr.data.dls.dtime += (s32)((0.0078125f * svoice->orgNote) * (f32)dscale);
  }

  svoice->pitchADSR.mode = 1;
  svoice->pitchADSR.data.dls.aMode = 0;
  svoice->pitchADSR.data.dls.aTime = adsrConvertTimeCents(adsr.data.dls.atime);
  svoice->pitchADSR.data.dls.dTime = adsrConvertTimeCents(adsr.data.dls.dtime);
  sl = (adsr.data.dls.slevel >> 2);

  svoice->pitchADSR.data.dls.sLevel = 193 - dspScale2IndexTab[MIN(sl, 0x3ffull)]; // FAKE: 64-bit literal; 0x3ff changes the object
  svoice->pitchADSR.data.dls.rTime = adsr.data.dls.rtime;
  adsrSetup(&svoice->pitchADSR);
  svoice->cFlags |= SYNTH_CFLAG_PITCH_ADSR;
}

static u32 mcmdPitchSweep(SYNTH_VOICE* svoice, MSTEP* cstep, int num) {
  s32 delta;
  svoice->sweepOff[num] = 0;
  svoice->sweepNum[num] = (u8)(cstep->para[0] >> 8);
  svoice->sweepCnt[num] = (s32)svoice->sweepNum[num] << 0x10;
  delta = (int)(short)(cstep->para[0] >> 0x10);
  if (delta >= 0) {
    delta = hwFrq2Pitch(delta);
  } else {
    delta = -hwFrq2Pitch(-delta);
  }
  svoice->sweepAdd[num] = delta << 0x10;
  cstep->para[0] = 0;
  return mcmdWait(svoice, cstep);
}

static void DoPanningSetup(SYNTH_VOICE* svoice, MSTEP* cstep, u8 pi) {
  s32 width;
  u32 mstime;
  svoice->panTime[pi] = width = (u16)(cstep->para[0] >> 16);
  sndConvertMs(&svoice->panTime[pi]);
  mstime = (s8)(cstep->para[1]);
  svoice->panning[pi] = ((u8)(cstep->para[0] >> 8)) << 16;
  svoice->panTarget[pi] = svoice->panning[pi] + mstime * 0x10000;
  if (svoice->panTime[pi] != 0) {
    svoice->panDelta[pi] = (s32)(mstime << 16) / width;
  } else {
    svoice->panDelta[pi] = (s32)(mstime << 16);
  }

  svoice->cFlags |= SYNTH_CFLAG_PAN_CHANGED;
}

static void mcmdSetPanning(SYNTH_VOICE* svoice, MSTEP* cstep) { DoPanningSetup(svoice, cstep, 0); }

static void mcmdSetSurroundPanning(SYNTH_VOICE* svoice, MSTEP* cstep) {
  DoPanningSetup(svoice, cstep, 1);
}

static void mcmdSetPianoPanning(SYNTH_VOICE* svoice, MSTEP* cstep) {
  s32 delta;
  s32 scale;
  delta = (svoice->curNote << 16) - ((u8)(cstep->para[0] >> 16) << 16);
  scale = (s8)((u8)(cstep->para[0] >> 8));
  delta = ((delta * scale) >> 7);
  delta += (u8)(cstep->para[0] >> 0x18) << 16;
  delta = delta < 0 ? 0 : delta > 0x7f0000 ? 0x7f0000 : delta;
  svoice->panTarget[0] = delta;
  svoice->panning[0] = delta;
}

static u32 TranslateVolume(u32 volume, u16 curve) {
  u8* ptr;
  u32 vlow;
  u32 vhigh;
  s32 d;

  if (curve != 0xFFFF) {
    if ((ptr = (u8*)dataGetCurve(curve))) {
      vhigh = (volume >> 16) & 0xFFFF;
      vlow = volume & 0xFFFF;

      if (vhigh < 0x7f) {
        d = vlow * (ptr[vhigh + 1] - ptr[vhigh]);
#if MUSY_VERSION >= MUSY_VERSION_CHECK(1, 5, 4)
        volume = d + ((u16)ptr[vhigh] << 16);
#else
        volume = d + (ptr[vhigh] << 16);
#endif
      } else {
        volume = ptr[vhigh] << 16;
      }
    }
  }

  return volume;
}

static void mcmdScaleVolume(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u16 curve;
  u16 scale;
  scale = (u16)(u8)(cstep->para[0] >> 8);

  if ((u8)(cstep->para[1] >> 8) == 0) {
    svoice->volume = (svoice->volume * scale) / 0x7f;
  } else {
    svoice->volume = (svoice->orgVolume * scale) / 0x7f;
  }
  svoice->volume += (u8)(cstep->para[0] >> 16) << 16;
  if (svoice->volume > 0x7f0000) {
    svoice->volume = 0x7f0000;
  }

  curve = (u8)(cstep->para[0] >> 0x18);
  curve |= ((u16)((u8)cstep->para[1]) << 8);

  svoice->volume = TranslateVolume(svoice->volume, curve);
  svoice->cFlags |= SYNTH_CFLAG_VOLUME_CHANGED;
}

static void mcmdScaleVolumeDLS(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u16 scale;

  scale = (cstep->para[0] >> 8);
  if ((u8)(cstep->para[0] >> 0x18) == 0) {
    svoice->volume = ((svoice->volume >> 5) * scale) >> 7;
  } else {
    svoice->volume = ((svoice->orgVolume >> 5) * scale) >> 7;
  }
  if (svoice->volume > 0x7f0000) {
    svoice->volume = 0x7f0000;
  }

  svoice->cFlags |= SYNTH_CFLAG_VOLUME_CHANGED;
}

static void DoEnvelopeCalculation(SYNTH_VOICE* svoice, MSTEP* cstep, s32 start_vol) {
  u32 tvol;
  u32 time;
  s32 mstime;
  u16 curve;

  time = (u16)(cstep->para[1] >> 16);

  if ((u8)(cstep->para[1] >> 8) & 1) {
    sndConvertMs(&time);
  } else {
    sndConvertTicks(&time, svoice);
  }

  mstime = sndConvert2Ms(time);
  if (mstime == 0) {
    mstime = 1;
  }

  tvol = (svoice->volume * (u8)(cstep->para[0] >> 8) >> 7);
  tvol += (u8)(cstep->para[0] >> 16) << 16;

  if (tvol > 0x7f0000) {
    tvol = 0x7f0000;
  }

  curve = (u16)(u8)(cstep->para[0] >> 0x18);
  curve |= (((u16)(u8)cstep->para[1]) << 8);
  tvol = TranslateVolume(tvol, curve);
  svoice->envTarget = tvol;
  svoice->envCurrent = start_vol;
  svoice->envDelta = (s32)(tvol - start_vol) / mstime;
  svoice->volume = start_vol;
  svoice->cFlags |= SYNTH_CFLAG_VOL_ENVELOPE;
}

static void mcmdEnvelope(SYNTH_VOICE* svoice, MSTEP* cstep) {
  DoEnvelopeCalculation(svoice, cstep, svoice->volume);
}
static void mcmdFadeIn(SYNTH_VOICE* svoice, MSTEP* cstep) {
  DoEnvelopeCalculation(svoice, cstep, 0);
}

static void mcmdRandomKey(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u8 k1;
  u8 k2;
  u8 t;
  s32 i1;
  s32 i2;
  u8 detune;

  if (!(u8)(cstep->para[1] >> 8)) {
    k1 = (cstep->para[0] >> 8);
    k2 = (cstep->para[0] >> 24);
    if (k1 > k2) {
      t = k1;
      k1 = k2;
      k2 = t;
    }
  } else {
    i1 = svoice->curNote - (u8)(cstep->para[0] >> 8);
    i2 = svoice->curNote + (u8)(cstep->para[0] >> 24);

    k1 = i1 < 0 ? 0 : i1 > 127 ? 127 : i1;
    k2 = i2 < 0 ? 0 : i2 > 127 ? 127 : i2;
  }

  if ((u8)cstep->para[1]) {
    detune = (sndRand() % 201) - 100;
  } else {
    detune = (u8)(cstep->para[0] >> 16);
  }

  cstep->para[0] = ((u8)detune << 16) | 0x19 | ((k1 + (sndRand() % ((k2 - k1) + 1))) * 0x100);
  cstep->para[1] = 0;
  mcmdSetKey(svoice, cstep);
}

static void mcmdSetPitchbendAfterKeyOff(SYNTH_VOICE* svoice) {
  svoice->cFlags |= SYNTH_CFLAG_PITCH_BEND_AFTER_KEY_OFF;
}
static void mcmdScaleReverb(SYNTH_VOICE* svoice, MSTEP* cstep) {
  svoice->revVolScale = (u8)(cstep->para[0] >> 8);
  svoice->revVolOffset = (u8)(cstep->para[0] >> 0x10);
}
static void SelectSource(SYNTH_VOICE* svoice, CTRL_DEST* dest, MSTEP* cstep, u64 tstflag,
                         u32 dirtyFlag) {
  u8 comb;
  s32 scale;

  if (!(svoice->cFlags & tstflag)) {
    comb = 0;
    svoice->cFlags |= tstflag;
  } else {
    comb = (u8)cstep->para[1];
  }

  scale = ((s16)(cstep->para[0] >> 16) << 16) / 100;
  if (scale < 0) {
    scale -= ((s8)(cstep->para[1] >> 0x10) << 8) / 100;
  } else {
    scale += ((s8)(cstep->para[1] >> 0x10) << 8) / 100;
  }

  inpAddCtrl(dest, (u8)(cstep->para[0] >> 8), scale, comb, (u8)(cstep->para[1] >> 8) != 0);

  if ((dirtyFlag & 0x80000000) != 0) {
    inpSetGlobalMIDIDirtyFlag(svoice->midi, svoice->midiSet, dirtyFlag);
  } else {
    svoice->midiDirtyFlags |= dirtyFlag;
  }
}

static void mcmdVolumeSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpVolume, cstep, SYNTH_CFLAG_SELECT_VOLUME, SYNTH_DIRTY_VOLUME);
}

static void mcmdPanningSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpPanning, cstep, SYNTH_CFLAG_SELECT_PANNING, SYNTH_DIRTY_PANNING);
}

static void mcmdPitchWheelSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpPitchBend, cstep, SYNTH_CFLAG_SELECT_PITCH_BEND,
               SYNTH_DIRTY_PITCH_BEND);
}

static void mcmdModWheelSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpModulation, cstep, SYNTH_CFLAG_SELECT_MODULATION,
               SYNTH_DIRTY_MODULATION);
}

static void mcmdPedalSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpPedal, cstep, SYNTH_CFLAG_SELECT_PEDAL, SYNTH_DIRTY_PEDAL);
}

static void mcmdPortamentoSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpPortamento, cstep, SYNTH_CFLAG_SELECT_PORTAMENTO,
               SYNTH_DIRTY_PORTAMENTO);
}

static void mcmdReverbSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpReverb, cstep, SYNTH_CFLAG_SELECT_REVERB, SYNTH_DIRTY_REVERB);
}

static void mcmdPreAuxASelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpPreAuxA, cstep, SYNTH_CFLAG_SELECT_PRE_AUXA,
               SYNTH_DIRTY_PRE_AUXA);
}

static void mcmdPreAuxBSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpPreAuxB, cstep, SYNTH_CFLAG_SELECT_PRE_AUXB,
               SYNTH_DIRTY_PRE_AUXB);
}

static void mcmdPostAuxBSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpPostAuxB, cstep, SYNTH_CFLAG_SELECT_POST_AUXB,
               SYNTH_DIRTY_POST_AUXB);
}

static void mcmdSurroundPanningSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpSurroundPanning, cstep, SYNTH_CFLAG_SELECT_SURROUND_PANNING,
               SYNTH_DIRTY_SURROUND_PANNING);
}

static void mcmdDopplerSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpDoppler, cstep, SYNTH_CFLAG_SELECT_DOPPLER, SYNTH_DIRTY_DOPPLER);
}

static void mcmdTremoloSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpTremolo, cstep, SYNTH_CFLAG_SELECT_TREMOLO, SYNTH_DIRTY_TREMOLO);
}

#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 1)
static void mcmdFilterSwitchSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpFilterSwitch, cstep, SYNTH_CFLAG_SELECT_FILTER_SWITCH,
               SYNTH_DIRTY_FILTER_SWITCH);
}

static void mcmdFilterParameterSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  SelectSource(svoice, &svoice->inpFilterParameter, cstep, SYNTH_CFLAG_SELECT_FILTER_PARAM,
               SYNTH_DIRTY_FILTER_PARAM);
}
#endif

static void mcmdAuxAFXSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u32 i;
  static u64 mask[4] = {SYNTH_CFLAG_SELECT_AUXA_0, SYNTH_CFLAG_SELECT_AUXA_1,
                        SYNTH_CFLAG_SELECT_AUXA_2, SYNTH_CFLAG_SELECT_AUXA_3}; // size: 0x20
  static u32 dirty[4] = {0x80000001, 0x80000002, 0x80000004, 0x80000008};    // size: 0x10
  i = (u8)(cstep->para[1] >> 0x18);
  SelectSource(svoice, &inpAuxA[svoice->studio][i], cstep, mask[i], dirty[i]);
}

static void mcmdAuxBFXSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u32 i;
  static u64 mask[4] = {SYNTH_CFLAG_SELECT_AUXB_0, SYNTH_CFLAG_SELECT_AUXB_1,
                        SYNTH_CFLAG_SELECT_AUXB_2, SYNTH_CFLAG_SELECT_AUXB_3}; // size: 0x20
  static u32 dirty[4] = {0x80000010, 0x80000020, 0x80000040, 0x80000080};        // size: 0x10
  i = (u8)(cstep->para[1] >> 0x18);
  SelectSource(svoice, &inpAuxB[svoice->studio][i], cstep, mask[i], dirty[i]);
}

static void mcmdPortamento(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u32 time;
  svoice->portType = cstep->para[0] >> 16;
  time = (u16)(cstep->para[1] >> 16);
  if (((u8)(cstep->para[1] >> 8) & 1)) {
    sndConvertMs(&time);
  } else {
    sndConvertTicks(&time, svoice);
  }

  svoice->portDuration = time;

  switch ((u8)(cstep->para[0] >> 8)) {
  case 0:
    if (svoice->midi != 0xFF) {
      inpSetMidiCtrl(0x41, svoice->midi, svoice->midiSet, 0);
    }

    svoice->cFlags &= ~SYNTH_CFLAG_PORTAMENTO;
    return;
  case 1:
    if (svoice->midi != 0xFF) {
      inpSetMidiCtrl(0x41, svoice->midi, svoice->midiSet, 0x7f);
    }
  init_port:
    if (!(svoice->cFlags & SYNTH_CFLAG_PORTAMENTO)) {
      synthInitPortamento(svoice);
    }
    svoice->cFlags |= SYNTH_CFLAG_PORTAMENTO;
    break;
  case 2:
    if (svoice->midi != 0xFF && inpGetMidiCtrl(0x41, svoice->midi, svoice->midiSet) > 8064) {
      goto init_port;
    }
    break;
  }
}

s32 varGet32(SYNTH_VOICE* svoice, u32 ctrl, u8 index) {
  if (ctrl != 0) {
    return inpGetExCtrl(svoice, index);
  }

  index &= 0x1f;
  return index < 16 ? svoice->local_vars[index] : synthGlobalVariable[index - 16];
}

s16 varGet(SYNTH_VOICE* svoice, u32 ctrl, u8 index) { return varGet32(svoice, ctrl, index); }

void varSet32(SYNTH_VOICE* svoice, u32 ctrl, u8 index, s32 v) {
  if (ctrl != 0) {
    inpSetExCtrl(svoice, index, v);
    return;
  }
  index &= 0x1f;

  if (index < 16) {
    svoice->local_vars[index] = v;
    return;
  }

  synthGlobalVariable[index - 16] = v;
}
void varSet(SYNTH_VOICE* svoice, u32 ctrl, u8 index, s16 v) { varSet32(svoice, ctrl, index, v); }

static void mcmdVarCalculation(SYNTH_VOICE* svoice, MSTEP* cstep, u8 op) {
  s16 s1;
  s16 s2;
  s32 t;

  s1 = varGet(svoice, (u8)(cstep->para[0] >> 24), cstep->para[1]);
  if (op == 4) {
    s2 = cstep->para[1] >> 8;
  } else {
    s2 = varGet(svoice, (u8)(cstep->para[1] >> 8), cstep->para[1] >> 16);
  }
  switch (op) {
  case 4:
  case 0:
    t = (s1 + s2);
    break;
  case 1:
    t = (s1 - s2);
    break;
  case 2:
    t = (s1 * s2);
    break;
  case 3:
    t = s2 != 0 ? (s1 / s2) : 0;
    break;
  }

  varSet(svoice, (u8)(cstep->para[0] >> 8), (u8)(cstep->para[0] >> 0x10),
         (t < -0x8000  ? -0x8000
          : t > 0x7FFF ? 0x7FFF
                       : t));
}

static void mcmdSetVarImmediate(SYNTH_VOICE* svoice, MSTEP* cstep) {
  varSet(svoice, (u8)(cstep->para[0] >> 8), (u8)(cstep->para[0] >> 0x10), (s16)cstep->para[1]);
}

static void mcmdIfVarCompare(SYNTH_VOICE* svoice, MSTEP* cstep, u8 cmp) {
  s32 a;
  s32 b;
  u8 result;

  a = varGet32(svoice, (u8)(cstep->para[0] >> 8), (u8)(cstep->para[0] >> 0x10));
  b = varGet32(svoice, (u8)(cstep->para[0] >> 0x18), (u8)cstep->para[1]);

  switch (cmp) {
  case 0:
    result = (a == b);
    break;
  case 1:
    result = (a < b);
    break;
  }

  if ((u8)(cstep->para[1] >> 8) != 0) {
    result = !result;
  }
  if ((u8)result != 0) {
    svoice->curAddr = svoice->addr + (u16)(cstep->para[1] >> 0x10);
  }
}
bool macPostMessage(u32 vid, s32 mesg) {
  SYNTH_VOICE* sv;
  if ((vid = vidGetInternalId(vid)) != -1 && (sv = &synthVoice[vid & 0xFF])->mesgNum < 4) {
    ++sv->mesgNum;
    sv->mesgQueue[sv->mesgWrite] = mesg;
    sv->mesgWrite = (sv->mesgWrite + 1) & 3;
    ExecuteTrap(sv, 2);
    return 1;
  }

  return 0;
}
static void mcmdSendMessage(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u8 i;
  s32 mesg;
  u16 macro;

  mesg = varGet32(svoice, 0, (u8)(cstep->para[1] >> 8));

  if (!(u8)(cstep->para[0] >> 8)) {
    macro = (u16)(cstep->para[0] >> 16);
    if (macro != 0xFFFF) {
      for (i = 0; i < synthInfo.voiceNum; ++i) {
        if (synthVoice[i].addr != NULL && macro == synthVoice[i].macroId) {
          macPostMessage(synthVoice[i].vidList->vid, mesg);
        }
      }
    } else if (synthMessageCallback != NULL) {
      synthMessageCallback(svoice->vidList->vid, mesg);
    }
  } else {
    macPostMessage(varGet32(svoice, 0, (u8)cstep->para[1]), mesg);
  }
}

static void mcmdGetMessage(SYNTH_VOICE* svoice, MSTEP* cstep) {
  s32 mesg;
  mesg = 0;
  if (svoice->mesgNum != '\0') {
    mesg = svoice->mesgQueue[svoice->mesgRead];
    svoice->mesgRead = (svoice->mesgRead + 1) & 3;
    --svoice->mesgNum;
  }
  varSet32(svoice, 0, (u8)(cstep->para[0] >> 8), mesg);
}

static void mcmdGetVID(SYNTH_VOICE* svoice, MSTEP* cstep) {
  if ((u8)(cstep->para[0] >> 0x10) == 0) {
    varSet32(svoice, 0, (u8)(cstep->para[0] >> 8), svoice->vidList->vid);
  } else {
    varSet32(svoice, 0, (u8)(cstep->para[0] >> 8), svoice->lastVID);
  }
}
static void mcmdModeSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  svoice->volTable = (u8)(cstep->para[0] >> 8) ? TRUE : FALSE;
  svoice->itdMode = (u8)(cstep->para[0] >> 0x10) ? FALSE : TRUE;
}
static void mcmdSRCModeSelect(SYNTH_VOICE* svoice, MSTEP* cstep) {
  hwSetSRCType(svoice->id & 0xff, (u8)(cstep->para[0] >> 8));
  hwSetPolyPhaseFilter(svoice->id & 0xff, (u8)(cstep->para[0] >> 0x10));
  svoice->cFlags |= SYNTH_CFLAG_SRC_SET;
}
static void mcmdSetKeyGroup(SYNTH_VOICE* svoice, MSTEP* cstep) {
  u32 i;
  u8 kg;
  u32 kill;

  svoice->keyGroup = 0;
  kg = (u8)(cstep->para[0] >> 8);
  kill = (u8)(cstep->para[0] >> 0x10) != 0;

  if (kg) {
    for (i = 0; i < synthInfo.voiceNum; ++i) {
      if (synthVoice[i].addr != NULL && (synthVoice[i].cFlags & SYNTH_CFLAG_START_PENDING) == 0 &&
          kg == synthVoice[i].keyGroup) {
        if (!kill) {
          macSetExternalKeyoff(&synthVoice[i]);
        } else {
          voiceKill(i);
        }
      }
    }
    svoice->keyGroup = kg;
  }
}
static void mcmdSetupTremolo(SYNTH_VOICE* svoice, MSTEP* cstep) {
  svoice->treScale = (cstep->para[0] >> 8);
  svoice->treModAddScale = cstep->para[1];
  svoice->treCurScale = 1.f;
}

static void macHandleActive(SYNTH_VOICE* svoice) {
  u8 i;
  u8 lastNote;
  u32 ex;
  CHANNEL_DEFAULTS* channelDefaults;
  static MSTEP cstep;

  if (svoice->cFlags & (SYNTH_CFLAG_START_BREAK | SYNTH_CFLAG_START_PENDING)) {
    if (svoice->cFlags & SYNTH_CFLAG_START_BREAK) {
      svoice->cFlags &= ~SYNTH_CFLAG_START_BREAK;
      hwBreak(svoice->id & 0xFF);
    }

    svoice->panning[0] = svoice->panTarget[0] = (u32)(svoice->setup.pan) << 16;
    svoice->panning[1] = svoice->panTarget[1] = 0;
    svoice->volume = (u32)(svoice->setup.vol << 16);
    svoice->volTable = 0;
    svoice->orgVolume = svoice->volume;
    svoice->midi = svoice->setup.midi;
    svoice->midiSet = svoice->setup.midiSet;
    svoice->section = svoice->setup.section;
    svoice->track = svoice->setup.track;
    svoice->itdMode = svoice->setup.itdMode;
    svoice->keyGroup = 0;
    svoice->vibModAddScale = 0;
    svoice->treScale = 0;
    inpInit(svoice);
    if ((lastNote = inpGetMidiLastNote(svoice->midi, svoice->midiSet)) != 0xFF) {
      svoice->lastNote = lastNote;
    } else {
      svoice->lastNote = svoice->orgNote;
    }

    inpSetMidiLastNote(svoice->midi, svoice->midiSet, svoice->orgNote);
    voiceSetLastStarted(svoice);
    svoice->vGroup = svoice->setup.vGroup;
    svoice->studio = svoice->setup.studio;
    svoice->portTime = 0;
    svoice->portDuration = 25600;
    svoice->portType = 0;
    if (svoice->midi != 0xff) {
      svoice->portLastCtrlState = inpGetMidiCtrl(65, svoice->midi, svoice->midiSet);
    } else {
      svoice->portLastCtrlState = 0;
    }
    channelDefaults = inpGetChannelDefaults(svoice->midi, svoice->midiSet);
    svoice->pbLowerKeyRange = channelDefaults->pbRange;
    svoice->pbUpperKeyRange = channelDefaults->pbRange;
#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 1)
    svoice->lpfLowerFrqBoundary = channelDefaults->lpfLowerFrqBoundary;
    svoice->lpfUpperFrqBoundary = channelDefaults->lpfUpperFrqBoundary;
#endif
    svoice->revVolScale = 128;
    svoice->revVolOffset = 0;
    svoice->loop = 0;
    svoice->sweepNum[0] = 0;
    svoice->sweepNum[1] = 0;
    svoice->sweepOff[0] = 0;
    svoice->sweepOff[1] = 0;
    svoice->lfo[0].period = 0;
    svoice->lfo[0].value = 0;
    svoice->lfo[0].lastValue = 0x7fff;
    svoice->lfo[1].period = 0;
    svoice->lfo[1].value = 0;
    svoice->lfo[1].lastValue = 0x7fff;

    for (i = 0; i < 3; ++i) {
      svoice->trapEventAddr[i] = NULL;
    }

    svoice->trapEventAny = 0;
    svoice->sInfo = -1;
    svoice->playFrq = -1;
#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 1)
    svoice->sampleId = 0xFFFF;
#endif
    svoice->pbLast = 0x2000;
    svoice->curOutputVolume = 0;
    svoice->cFlags &= SYNTH_CFLAG_KEY_OFF;
    svoice->cFlags |= (SYNTH_CFLAG_VOLUME_CHANGED | SYNTH_CFLAG_PAN_CHANGED);
    memset(svoice->local_vars, 0, sizeof(svoice->local_vars));
    svoice->waitTime = macRealTime;
    svoice->macStartTime = macRealTime;
    synthStartSynthJobHandling(svoice);
  }

  DebugMacroSteps = 0;

  do {
    if (++DebugMacroSteps > 32) {
      break;
    }

    cstep.para[0] = svoice->curAddr->para[0];
    cstep.para[1] = svoice->curAddr->para[1];
    ++svoice->curAddr;
    ex = 0;
    switch (cstep.para[0] & 0x7f) {
    case MACRO_OP_END_OF_MACRO:
      ex = mcmdEndOfMacro(svoice);
      break;
    case MACRO_OP_STOP:
      ex = mcmdStop(svoice);
      break;
    case MACRO_OP_IF_KEY:
      mcmdIfKey(svoice, &cstep);
      break;
    case MACRO_OP_IF_VELOCITY:
      mcmdIfVelocity(svoice, &cstep);
      break;
    case MACRO_OP_WAIT:
      ex = mcmdWait(svoice, &cstep);
      break;
    case MACRO_OP_LOOP:
      mcmdLoop(svoice, &cstep);
      break;
    case MACRO_OP_GOTO:
      ex = mcmdGoto(svoice, &cstep);
      break;
    case MACRO_OP_WAIT_MS:
      ex = mcmdWaitMs(svoice, &cstep);
      break;
    case MACRO_OP_PLAY_MACRO:
      mcmdPlayMacro(svoice, &cstep);
      break;
    case MACRO_OP_SEND_KEY_OFF:
      mcmdSendKeyOff(svoice, &cstep);
      break;
    case MACRO_OP_IF_MODULATION:
      mcmdIfModulation(svoice, &cstep);
      break;
    case MACRO_OP_SET_PIANO_PANNING:
      mcmdSetPianoPanning(svoice, &cstep);
      break;
    case MACRO_OP_SET_ADSR:
      mcmdSetADSR(svoice, &cstep);
      break;
    case MACRO_OP_SCALE_VOLUME:
      mcmdScaleVolume(svoice, &cstep);
      break;
    case MACRO_OP_SET_PANNING:
      mcmdSetPanning(svoice, &cstep);
      break;
    case MACRO_OP_ENVELOPE:
      mcmdEnvelope(svoice, &cstep);
      break;
    case MACRO_OP_START_SAMPLE:
      mcmdStartSample(svoice, &cstep);
      break;
    case MACRO_OP_STOP_SAMPLE:
      mcmdStopSample(svoice);
      break;
    case MACRO_OP_KEY_OFF:
      mcmdKeyOff(svoice);
      break;
    case MACRO_OP_IF_RANDOM:
      mcmdIfRandom(svoice, &cstep);
      break;
    case MACRO_OP_FADE_IN:
      mcmdFadeIn(svoice, &cstep);
      break;
    case MACRO_OP_SET_SURROUND_PANNING:
      mcmdSetSurroundPanning(svoice, &cstep);
      break;
    case MACRO_OP_SET_ADSR_FROM_CTRL:
      mcmdSetADSRFromCtrl(svoice, &cstep);
      break;
    case MACRO_OP_RANDOM_KEY:
      mcmdRandomKey(svoice, &cstep);
      break;
    case MACRO_OP_ADD_KEY:
      ex = mcmdAddKey(svoice, &cstep);
      break;
    case MACRO_OP_SET_KEY:
      ex = mcmdSetKey(svoice, &cstep);
      break;
    case MACRO_OP_LAST_KEY:
      ex = mcmdLastKey(svoice, &cstep);
      break;
    case MACRO_OP_PORTAMENTO:
      mcmdPortamento(svoice, &cstep);
      break;
    case MACRO_OP_VIBRATO:
      mcmdVibrato(svoice, &cstep);
      break;
    case MACRO_OP_PITCH_SWEEP_1:
      ex = mcmdPitchSweep(svoice, &cstep, 0);
      break;
    case MACRO_OP_PITCH_SWEEP_2:
      ex = mcmdPitchSweep(svoice, &cstep, 1);
      break;
    case MACRO_OP_SET_PITCH:
      mcmdSetPitch(svoice, &cstep);
      break;
    case MACRO_OP_SET_PITCH_ADSR:
      mcmdSetPitchADSR(svoice, &cstep);
      break;
    case MACRO_OP_SCALE_VOLUME_DLS:
      mcmdScaleVolumeDLS(svoice, &cstep);
      break;
    case MACRO_OP_SET_MOD2_VIBRATO:
      mcmdSetMod2Vibrato(svoice, &cstep);
      break;
    case MACRO_OP_SETUP_TREMOLO:
      mcmdSetupTremolo(svoice, &cstep);
      break;
    case MACRO_OP_RETURN:
      mcmdReturn(svoice);
      break;
    case MACRO_OP_GOSUB:
      ex = mcmdGosub(svoice, &cstep);
      break;
    case MACRO_OP_TRAP_EVENT:
      mcmdTrapEvent(svoice, &cstep);
      break;
    case MACRO_OP_UNTRAP_EVENT:
      mcmdUntrapEvent(svoice, &cstep);
      break;
    case MACRO_OP_SEND_MESSAGE:
      mcmdSendMessage(svoice, &cstep);
      break;
    case MACRO_OP_GET_MESSAGE:
      mcmdGetMessage(svoice, &cstep);
      break;
    case MACRO_OP_GET_VID:
      mcmdGetVID(svoice, &cstep);
      break;
    case MACRO_OP_ADD_AGE_COUNTER:
      mcmdAddAgeCounter(svoice, &cstep);
      break;
    case MACRO_OP_SET_AGE_COUNTER:
      mcmdSetAgeCounter(svoice, &cstep);
      break;
    case MACRO_OP_SEND_FLAG:
      mcmdSendFlag(&cstep);
      break;
    case MACRO_OP_SET_PITCH_WHEEL_RANGE:
      mcmdSetPitchWheelRange(svoice, &cstep);
      break;
    case MACRO_OP_SCALE_REVERB:
      mcmdScaleReverb(svoice, &cstep);
      break;
    case MACRO_OP_SET_PITCHBEND_AFTER_KEY_OFF:
      mcmdSetPitchbendAfterKeyOff(svoice);
      break;
    case MACRO_OP_SET_PRIORITY:
      mcmdSetPriority(svoice, &cstep);
      break;
    case MACRO_OP_ADD_PRIORITY:
      mcmdAddPriority(svoice, &cstep);
      break;
    case MACRO_OP_SET_AGE_COUNTER_SPEED:
      mcmdSetAgeCounterSpeed(svoice, &cstep);
      break;
    case MACRO_OP_SET_AGE_COUNTER_BY_VOLUME:
      mcmdSetAgeCounterByVolume(svoice, &cstep);
      break;
    case MACRO_OP_VOLUME_SELECT:
      mcmdVolumeSelect(svoice, &cstep);
      break;
    case MACRO_OP_PANNING_SELECT:
      mcmdPanningSelect(svoice, &cstep);
      break;
    case MACRO_OP_PITCH_WHEEL_SELECT:
      mcmdPitchWheelSelect(svoice, &cstep);
      break;
    case MACRO_OP_MOD_WHEEL_SELECT:
      mcmdModWheelSelect(svoice, &cstep);
      break;
    case MACRO_OP_PEDAL_SELECT:
      mcmdPedalSelect(svoice, &cstep);
      break;
    case MACRO_OP_PORTAMENTO_SELECT:
      mcmdPortamentoSelect(svoice, &cstep);
      break;
    case MACRO_OP_REVERB_SELECT:
      mcmdReverbSelect(svoice, &cstep);
      break;
    case MACRO_OP_SURROUND_PANNING_SELECT:
      mcmdSurroundPanningSelect(svoice, &cstep);
      break;
    case MACRO_OP_DOPPLER_SELECT:
      mcmdDopplerSelect(svoice, &cstep);
      break;
    case MACRO_OP_TREMOLO_SELECT:
      mcmdTremoloSelect(svoice, &cstep);
      break;
    case MACRO_OP_PRE_AUX_A_SELECT:
      mcmdPreAuxASelect(svoice, &cstep);
      break;
    case MACRO_OP_PRE_AUX_B_SELECT:
      mcmdPreAuxBSelect(svoice, &cstep);
      break;
    case MACRO_OP_POST_AUX_B_SELECT:
      mcmdPostAuxBSelect(svoice, &cstep);
      break;
    case MACRO_OP_AUX_AFX_SELECT:
      mcmdAuxAFXSelect(svoice, &cstep);
      break;
    case MACRO_OP_AUX_BFX_SELECT:
      mcmdAuxBFXSelect(svoice, &cstep);
      break;
    case MACRO_OP_SETUP_LFO:
      mcmdSetupLFO(svoice, &cstep);
      break;
    case MACRO_OP_MODE_SELECT:
      mcmdModeSelect(svoice, &cstep);
      break;
    case MACRO_OP_SET_KEY_GROUP:
      mcmdSetKeyGroup(svoice, &cstep);
      break;
    case MACRO_OP_SRC_MODE_SELECT:
      mcmdSRCModeSelect(svoice, &cstep);
      break;
#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 1)
    case MACRO_OP_FILTER_SWITCH_SELECT:
      mcmdFilterSwitchSelect(svoice, &cstep);
      break;
    case MACRO_OP_FILTER_PARAMETER_SELECT:
      mcmdFilterParameterSelect(svoice, &cstep);
      break;
#endif
    case MACRO_OP_VAR_ADD:
      mcmdVarCalculation(svoice, &cstep, 0);
      break;
    case MACRO_OP_VAR_SUB:
      mcmdVarCalculation(svoice, &cstep, 1);
      break;
    case MACRO_OP_VAR_MUL:
      mcmdVarCalculation(svoice, &cstep, 2);
      break;
    case MACRO_OP_VAR_DIV:
      mcmdVarCalculation(svoice, &cstep, 3);
      break;
    case MACRO_OP_VAR_ADD_ALT:
      mcmdVarCalculation(svoice, &cstep, 4);
      break;
    case MACRO_OP_SET_VAR_IMMEDIATE:
      mcmdSetVarImmediate(svoice, &cstep);
      break;
    case MACRO_OP_IF_VAR_EQUAL:
      mcmdIfVarCompare(svoice, &cstep, 0);
      break;
    case MACRO_OP_IF_VAR_LESS:
      mcmdIfVarCompare(svoice, &cstep, 1);
      break;
    }
  } while (!ex);
}

void macHandle(u32 deltaTime) {
  SYNTH_VOICE* sv;
  SYNTH_VOICE* nextSv;
  u64 w;

  for (sv = macTimeQueueRoot; sv != NULL && sv->wait <= macRealTime;) {
    nextSv = sv->nextTimeQueueMacro;
    w = sv->wait;
    macMakeActive(sv);
    sv->waitTime = w;
    sv = nextSv;
  }

  for (sv = macActiveMacroRoot; sv != NULL;
       sv = ((MUSY_VERSION <= MUSY_VERSION_CHECK(2, 0, 1)) ? sv->nextMacActive : nextSv)) {
    nextSv = sv->nextMacActive;
    if (HasHWEventTrap(sv) != 0) {
      CheckHWEventTrap(sv);
    }
    macHandleActive(sv);
  }
  macRealTime += deltaTime;
}

void macSampleEndNotify(SYNTH_VOICE* sv) {
  if (sv->macState != MAC_STATE_YIELDED) {
    return;
  }
  /* clang-format off */
  MUSY_ASSERT(sv->addr!=NULL);
  /* clang-format on */

  if (!ExecuteTrap(sv, 1) && (sv->cFlags & SYNTH_CFLAG_WAIT_SAMPLE_END)) {
    macMakeActive(sv);
  }
}
void macSetExternalKeyoff(SYNTH_VOICE* sv) {
  sv->cFlags |= SYNTH_CFLAG_KEY_OFF;
  if (!sv->addr) {
    return;
  }

  if (!(sv->cFlags & SYNTH_CFLAG_PEDAL_HELD)) {
    if (!ExecuteTrap(sv, 0) && (sv->cFlags & SYNTH_CFLAG_WAIT_KEY_OFF)) {
      macMakeActive(sv);
    }
  } else {
    sv->cFlags |= SYNTH_CFLAG_KEY_OFF_HELD;
  }
}

void macSetPedalState(SYNTH_VOICE* svoice, u32 state) {
  if (state != 0) {
    svoice->cFlags |= SYNTH_CFLAG_PEDAL_HELD;
  } else {
    if (svoice->addr && (svoice->cFlags & SYNTH_CFLAG_KEY_OFF_HELD)) {
      if (!ExecuteTrap(svoice, 0) && (svoice->cFlags & SYNTH_CFLAG_WAIT_KEY_OFF)) {
        macMakeActive(svoice);
      }
    }

    svoice->cFlags &= ~(SYNTH_CFLAG_PEDAL_HELD | SYNTH_CFLAG_KEY_OFF_HELD);
  }
}

static void TimeQueueAdd(SYNTH_VOICE* svoice) {
  SYNTH_VOICE* sv;
  SYNTH_VOICE* lastSv;

  lastSv = NULL;
  for (sv = macTimeQueueRoot; sv != NULL && sv->wait < svoice->wait;) {
    lastSv = sv;
    sv = sv->nextTimeQueueMacro;
  }

  if (sv == NULL) {
    if (lastSv == NULL) {
      macTimeQueueRoot = svoice;
      svoice->nextTimeQueueMacro = NULL;
      svoice->prevTimeQueueMacro = NULL;
    } else {
      lastSv->nextTimeQueueMacro = svoice;
      svoice->prevTimeQueueMacro = lastSv;
      svoice->nextTimeQueueMacro = NULL;
    }
  } else {
    svoice->nextTimeQueueMacro = sv;
    if (svoice->prevTimeQueueMacro = sv->prevTimeQueueMacro) {
      sv->prevTimeQueueMacro->nextTimeQueueMacro = svoice;
    } else {
      macTimeQueueRoot = svoice;
    }
    sv->prevTimeQueueMacro = svoice;
  }
}
static void UnYieldMacro(SYNTH_VOICE* svoice, bool disableUpdate) {
  if (svoice->wait != 0) {
    if (svoice->wait != -1) {
      if (svoice->prevTimeQueueMacro == NULL) {
        macTimeQueueRoot = svoice->nextTimeQueueMacro;
      } else {
        svoice->prevTimeQueueMacro->nextTimeQueueMacro = svoice->nextTimeQueueMacro;
      }

      if (svoice->nextTimeQueueMacro) {
        svoice->nextTimeQueueMacro->prevTimeQueueMacro = svoice->prevTimeQueueMacro;
      }
    }

    if (!disableUpdate) {
      synthForceLowPrecisionUpdate(svoice);
    }

    svoice->wait = 0;
    svoice->waitTime = macRealTime;
    svoice->cFlags &= ~(SYNTH_CFLAG_WAIT_KEY_OFF | SYNTH_CFLAG_WAIT_SAMPLE_END);
  }
}
void macMakeActive(SYNTH_VOICE* sv) {
  if (sv->macState == MAC_STATE_RUNNABLE) {
    return;
  }
  /* clang-format off */
  MUSY_ASSERT(sv->addr!=NULL);
  /* clang-format on */
  UnYieldMacro(sv, 0);
  if (sv->nextMacActive = macActiveMacroRoot) {
    macActiveMacroRoot->prevMacActive = sv;
  }
  sv->prevMacActive = NULL;
  macActiveMacroRoot = sv;
  sv->macState = MAC_STATE_RUNNABLE;
}

void macMakeInactive(SYNTH_VOICE* svoice, MAC_STATE newState) {
  if (svoice->macState == newState) {
    return;
  }

  /* clang-format off */
  MUSY_ASSERT(svoice->addr!=NULL);
  /* clang-format on */
  if (svoice->macState == MAC_STATE_RUNNABLE) {
    if (svoice->prevMacActive == NULL) {
      macActiveMacroRoot = svoice->nextMacActive;
    } else {
      svoice->prevMacActive->nextMacActive = svoice->nextMacActive;
    }

    if (svoice->nextMacActive != NULL) {
      svoice->nextMacActive->prevMacActive = svoice->prevMacActive;
    }
  }

  if (newState == MAC_STATE_STOPPED) {
    UnYieldMacro(svoice, 1);
  }
  svoice->macState = newState;
}

u32 macStart(u16 macid, u8 priority, u8 maxVoices,
#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 1)
             u32 allocId,
#else
             u16 allocId,
#endif
             u8 key, u8 vol, u8 panning, u8 midi, u8 midiSet, u8 section, u16 step, u16 trackid,
             u8 new_vid, u8 vGroup, u8 studio, u32 itd) {
  u32 voice;
  u32 vid;
  MSTEP* addr;
  SYNTH_VOICE* svoice;
  u16 seqPrio;

  if ((addr = dataGetMacro(macid))) {
    if (!(key & 0x80) && (seqPrio = seqGetMIDIPriority(midiSet, midi)) != 0xFFFF) {
      priority = seqPrio;
    }

    if ((voice = voiceAllocate(priority, maxVoices, allocId, (key & 0x80) ? 1 : 0)) != -1) {
      svoice = &synthVoice[voice];
      vidRemoveVoiceReferences(svoice);
      macMakeInactive(svoice, MAC_STATE_STOPPED);
      svoice->cFlags = (svoice->cFlags & SYNTH_CFLAG_KEY_ON) | SYNTH_CFLAG_START_PENDING;

      if (hwIsActive(voice)) {
        svoice->cFlags |= SYNTH_CFLAG_START_BREAK;
      }

      svoice->wait = 0;

      if ((key & 0x80) != 0) {
        svoice->fxFlag = 01;
        key &= 0x7f;
        inpResetMidiCtrl(voice, 0xff, 1);
        inpResetChannelDefaults(voice, 0xff);
        svoice->setup.midi = voice;
        svoice->setup.midiSet = 0xff;
        svoice->setup.section = 0;
      } else {
        svoice->fxFlag = 0;
        svoice->setup.midi = midi;
        svoice->setup.midiSet = midiSet;
        svoice->setup.section = section;
      }

      svoice->macroId = macid;
      svoice->allocId = allocId;
      svoice->age = 0x75300000;
      svoice->ageSpeed = 0x400;
      svoice->addr = addr;
      svoice->curAddr = addr + step;
      svoice->orgNote = key;
      svoice->curNote = key;
      svoice->curDetune = 0;
      svoice->setup.vol = vol;
      svoice->setup.pan = panning;
      svoice->setup.track = trackid;
      svoice->callStackEntryNum = 0;
      svoice->callStackIndex = 0;
      svoice->child = -1;
      svoice->parent = -1;
      svoice->lastVID = -1;
      svoice->setup.vGroup = vGroup;
      svoice->setup.studio = studio;
      svoice->setup.itdMode = itd != 0 ? 0 : 1;
      svoice->mesgNum = svoice->mesgRead = svoice->mesgWrite = 0;
      svoice->id = voice | ((macid << 16) | (key << 8));
      voiceSetPriority(svoice, priority);

      if ((vid = vidMakeNew(svoice, new_vid)) != -1) {
        macMakeActive(svoice);
        return vid;
      }

      if (hwIsActive(voice)) {
        hwBreak(voice);
      }

      voiceFree(svoice);
    }
  }

  return -1;
}

void macInit() {
  u32 i;

  macActiveMacroRoot = 0;
  macTimeQueueRoot = 0;
  macRealTime = 0;
  for (i = 0; i < synthInfo.voiceNum; ++i) {
    synthVoice[i].addr = NULL;
    synthVoice[i].macState = MAC_STATE_STOPPED;
    synthVoice[i].loop = 0;
  }
}

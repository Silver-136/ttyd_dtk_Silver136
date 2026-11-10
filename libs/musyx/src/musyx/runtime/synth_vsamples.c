#include "musyx/musyx.h"

#include "musyx/assert.h"
#include "musyx/hardware.h"
#include "musyx/macros.h"
#include "musyx/snd.h"
#include "musyx/synthdata.h"
#include "musyx/voice.h"

#if MUSY_TARGET == MUSY_TARGET_DOLPHIN
#include <dolphin/os.h>
#endif

u32 vidGetPublicId(u32 voice);
u32 seqGetInstanceForVoice(u32 vid);
u32 hwGetSampleExtraData(u32 voice);

static VS vs;

void vsInit() {
  u32 i;
  vs.numBuffers = 0;
  for (i = 0; i < 64; i++) {
    vs.voices[i] = 0xFF;
  }

  vs.nextInstID = 0;
  vs.callback = NULL;
}

static u16 vsNewInstanceID() {
  u8 i;
  u16 instID;
  do {
    instID = vs.nextInstID++;
    for (i = 0; i < vs.numBuffers; ++i) {
      if (vs.streamBuffer[i].state != VS_STATE_FREE && vs.streamBuffer[i].info.instID == instID) {
        break;
      }
    }
  } while (i != vs.numBuffers);

  return instID;
}

static u8 vsAllocateBuffer() {
  u8 i;

  for (i = 0; i < vs.numBuffers; ++i) {
    if (vs.streamBuffer[i].state != VS_STATE_FREE) {
      continue;
    }
    vs.streamBuffer[i].state = VS_STATE_STREAMING;
    vs.streamBuffer[i].last = 0;
    return i;
  }

  return 0xFF;
}

static void vsFreeBuffer(u8 bufferIndex) {
  vs.streamBuffer[bufferIndex].state = VS_STATE_FREE;
  vs.voices[vs.streamBuffer[bufferIndex].voice] = 0xFF;
}

u32 vsSampleStartNotify(unsigned long vid) {
  u8 voice;
  u8 sb;
  u8 i;
  size_t addr;

  voice = vid & 0xFF;
  for (i = 0; i < vs.numBuffers; ++i) {
    if (vs.streamBuffer[i].state != VS_STATE_FREE && vs.streamBuffer[i].voice == voice) {
      vsFreeBuffer(i);
    }
  }

  sb = vs.voices[voice] = vsAllocateBuffer();
  if (sb != 0xFF) {
    addr = aramGetStreamBufferAddress(vs.voices[voice], 0);
    hwSetVirtualSampleLoopBuffer(voice, (void*)addr, vs.bufferLength);
    vs.streamBuffer[sb].info.smpID = hwGetSampleID(voice);
    vs.streamBuffer[sb].info.instID = vsNewInstanceID();
    if ((vs.streamBuffer[sb].info.pubID = vidGetPublicId(vid)) != 0xFFFFFFFF) {
      vs.streamBuffer[sb].info.seqInstID = seqGetInstanceForVoice(vs.streamBuffer[sb].info.pubID);
    } else {
      vs.streamBuffer[sb].info.seqInstID = 0xFFFFFFFF;
    }
    vs.streamBuffer[sb].info.data.extraData = hwGetSampleExtraData(voice);
    vs.streamBuffer[sb].smpType = hwGetSampleType(voice);
    vs.streamBuffer[sb].voice = voice;
    if (vs.callback != NULL && vs.callback(SND_VIRTUALSAMPLE_REASON_INIT, &vs.streamBuffer[sb].info) == 0) {
      return (vs.streamBuffer[sb].info.instID << 8) | voice;
    }
    hwSetVirtualSampleLoopBuffer(voice, 0, 0);
    vs.streamBuffer[sb].state = VS_STATE_FREE;
    vs.voices[vs.streamBuffer[sb].voice] = 0xFF;
  } else {
    hwSetVirtualSampleLoopBuffer(voice, 0, 0);
  }

  return 0xFFFFFFFF;
}

void vsSampleEndNotify(u32 pubID) {
  u8 sb;


  if (pubID != 0xFFFFFFFF) {
    u8 id = (u8)pubID;
    sb = vs.voices[id];
    if (sb != 0xFF) {
      if (vs.streamBuffer[sb].info.instID == ((pubID >> 8) & 0xFFFF)) {
        if (vs.callback != NULL) {
          vs.callback(SND_VIRTUALSAMPLE_REASON_STOP, &vs.streamBuffer[sb].info);
        }
        vsFreeBuffer(sb);
      }
    }
  }
}

static void vsUpdateBuffer(struct VS_BUFFER* sb, unsigned long cpos) {
  u32 len;
  if (sb->last == cpos) {
    return;
  }
  if (sb->last < cpos) {
    switch (sb->smpType) {
    case SAMPLE_COMP_ADPCM_VIRTUAL: {
      sb->info.data.update.off1 = (sb->last / 14) * 8;
      sb->info.data.update.len1 = cpos - sb->last;
      sb->info.data.update.off2 = 0;
      sb->info.data.update.len2 = 0;
      if ((len = vs.callback(SND_VIRTUALSAMPLE_REASON_UPDATE, &sb->info)) != 0) {
        sb->last = (sb->last + len) % vs.bufferLength;
      }
    } break;
    case SAMPLE_COMP_PCM16_VIRTUAL: {
      sb->info.data.update.off1 = sb->last * 2;
      sb->info.data.update.len1 = cpos - sb->last;
      sb->info.data.update.off2 = 0;
      sb->info.data.update.len2 = 0;
      if ((len = vs.callback(SND_VIRTUALSAMPLE_REASON_UPDATE, &sb->info)) != 0) {
        sb->last = (sb->last + len) % vs.bufferLength;
      }
    } break;
    default:
      break;
    }
  } else if (cpos == 0) {
    switch (sb->smpType) {
    case SAMPLE_COMP_ADPCM_VIRTUAL: {
      sb->info.data.update.off1 = (sb->last / 14) * 8;
      sb->info.data.update.len1 = vs.bufferLength - sb->last;
      sb->info.data.update.off2 = 0;
      sb->info.data.update.len2 = 0;
      if ((len = vs.callback(SND_VIRTUALSAMPLE_REASON_UPDATE, &sb->info)) != 0) {
        sb->last = (sb->last + len) % vs.bufferLength;
      }
    } break;
    case SAMPLE_COMP_PCM16_VIRTUAL: {
      sb->info.data.update.off1 = sb->last * 2;
      sb->info.data.update.len1 = vs.bufferLength - sb->last;
      sb->info.data.update.off2 = 0;
      sb->info.data.update.len2 = 0;
      if ((len = vs.callback(SND_VIRTUALSAMPLE_REASON_UPDATE, &sb->info)) != 0) {
        sb->last = (sb->last + len) % vs.bufferLength;
      }
    } break;
    default:
      break;
    }
  } else {
    switch (sb->smpType) {
    case SAMPLE_COMP_ADPCM_VIRTUAL: {
      sb->info.data.update.off1 = (sb->last / 14) * 8;
      sb->info.data.update.len1 = vs.bufferLength - sb->last;
      sb->info.data.update.off2 = 0;
      sb->info.data.update.len2 = cpos;
      if ((len = vs.callback(SND_VIRTUALSAMPLE_REASON_UPDATE, &sb->info)) != 0) {
        sb->last = (sb->last + len) % vs.bufferLength;
      }
    } break;
    case SAMPLE_COMP_PCM16_VIRTUAL: {
      sb->info.data.update.off1 = sb->last * 2;
      sb->info.data.update.len1 = vs.bufferLength - sb->last;
      sb->info.data.update.off2 = 0;
      sb->info.data.update.len2 = cpos;
      if ((len = vs.callback(SND_VIRTUALSAMPLE_REASON_UPDATE, &sb->info)) != 0) {
        sb->last = (sb->last + len) % vs.bufferLength;
      }
    } break;
    default:
      break;
    }
  }
}

void vsSampleUpdates() {
  u32 i;
  u32 cpos;
  u32 realCPos;
  VS_BUFFER* sb;
  u32 nextSamples;

  if (vs.callback == NULL) {
    return;
  }

  for (i = 0; i < 64; ++i) {
    if (vs.voices[i] != 0xFF && hwGetVirtualSampleState(i) != 0) {
      sb = &vs.streamBuffer[vs.voices[i]];
      realCPos = hwGetPos(i);
      if (sb->smpType == SAMPLE_COMP_ADPCM_VIRTUAL) {
        cpos = (realCPos / 14) * 14;
      } else {
        cpos = realCPos;
      }

      switch (sb->state) {
      case VS_STATE_STREAMING:
        vsUpdateBuffer(sb, cpos);
        break;
      case VS_STATE_DRAINING:
#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 0)
      case VS_STATE_DRAINING_ABORT:
#endif
        if (((sb->info.instID << 8) | sb->voice) == hwGetVirtualSampleID(sb->voice)) {
          vsUpdateBuffer(sb, cpos);

          if (realCPos >= sb->finalLast) {
            sb->finalGoodSamples -= (realCPos - sb->finalLast);
          } else {
            sb->finalGoodSamples -= (vs.bufferLength - (sb->finalLast - realCPos));
          }

          sb->finalLast = realCPos;
          nextSamples = (synthVoice[sb->voice].curPitch * 160 + 0xFFF) / 4096;
          if ((s32)nextSamples > (s32)sb->finalGoodSamples) {
            if (!hwVoiceInStartup(sb->voice)) {
#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 0)
              if (sb->state == VS_STATE_DRAINING) {
                hwBreak(sb->voice);
                macSampleEndNotify(&synthVoice[sb->voice]);
              } else {
                voiceKill(sb->voice);
              }
#else
              hwBreak(sb->voice);
#endif
            }

            sb->state = VS_STATE_FREE;
            vs.voices[sb->voice] = 0xff;
          }
        } else {
          sb->state = VS_STATE_FREE;
          vs.voices[sb->voice] = 0xff;
        }
        break;
      }
    }
  }
}

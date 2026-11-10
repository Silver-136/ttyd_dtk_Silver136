#define SND3D_LISTENER_NO_ROOM 1
#define SND3D_EMITTER_NO_ROOM 1
#include "musyx/musyx.h"

#include "musyx/hardware.h"
#include "musyx/sal.h"
#include "musyx/synth.h"
#include "musyx/s3d_key.h"

static u8 s3dCallCnt;
u8 gap_08_80418DD9_sbss[7];
static SND_EMITTER* s3dEmitterRoot;
static SND_LISTENER* s3dListenerRoot;

// The room list (s3dRoomRoot) of the original source is not reconstructed: only the room functions this
// game does not link use it.
static u32 snd_used_studios;
static u8 snd_base_studio;
static u8 snd_max_studios;
static u8 s3dUseMaxVoices;
static u8 s3dUseLegacyLogic;
static void (*s3dOcclusionCallback)(struct SND_EMITTER*, SND_FVECTOR*, SND_FVECTOR*, void*, SND_FVECTOR*, SND_FVECTOR*, f32*, f32*);

// Not reconstructed (this game does not link them): UpdateRoomDistances, CheckRoomStatus, sndAddRoom, sndRemoveRoom,
// sndUpdateRoom, AddListener2Room, RemoveListenerFromRoom, CalcDoorParameters, CheckDoorStatus, sndAddDoor,
// sndRemoveDoor.
static void CalcEmitter(struct SND_EMITTER* em, f32* vol, f32* doppler, f32* xPan, f32* yPan,
                        f32* zPan, f32* occl) {
  SND_LISTENER* li;
  f32 v1, filt, occ, minFilt, minOcc;
  SND_FVECTOR d;
  SND_FVECTOR v;
  SND_FVECTOR p;
  f32 relspeed;
  f32 distance;
  f32 new_distance;
  f32 ft;
  f32 vd;
  SND_FVECTOR pan;
  u32 n;
  ft = 1.f / 60.f;
  *vol = 0.f;
  *doppler = 1.f;

  pan.z = 0.f;
  minOcc = 1.f;
  minFilt = 1.f;
  pan.x = pan.y = 0.f;

  for (n = 0, li = s3dListenerRoot; li != NULL; li = li->next, ++n) {
    d.x = em->pos.x - (li->pos.x + li->heading.x * li->volPosOff);
    d.y = em->pos.y - (li->pos.y + li->heading.y * li->volPosOff);
    d.z = em->pos.z - (li->pos.z + li->heading.z * li->volPosOff);

    distance = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);

    if (em->maxDis >= distance) {
      vd = distance / em->maxDis;

      if (em->volPush >= 0.f) {
        v1 =
            li->vol * (em->minVol + (em->maxVol - em->minVol) *
                                        (1.f - ((1.f - em->volPush) * vd + em->volPush * vd * vd)));
      } else {
        v1 =
            li->vol * (em->minVol + (em->maxVol - em->minVol) *
                                        (1.f - ((em->volPush + 1.f) * vd -
                                                em->volPush * (1.f - (1.f - vd) * (1.f - vd)))));
      }

      if ((em->flags & 0x180) && s3dOcclusionCallback != NULL) {
        s3dOcclusionCallback(em, &li->pos, &li->heading, (u8*)li + 0x40, &em->pos, &em->dir, &occ,
                             &filt);
        v1 *= 1.f - occ;
      } else {
        filt = 0.f;
      }

      if (s3dUseLegacyLogic) {

        *vol += v1;
      } else if (*vol < v1) {
        *vol = v1;
      }

      if (!(em->flags & 0x80000)) {
        if ((em->flags & 0x8) || (li->flags & 1)) {
          v.x = li->dir.x - em->dir.x;
          v.y = li->dir.y - em->dir.y;
          v.z = li->dir.z - em->dir.z;
          relspeed = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

          if (relspeed > 0.f) {

            d.x = (em->pos.x + em->dir.x * ft) - (li->pos.x + li->dir.x * ft);
            d.y = (em->pos.y + em->dir.y * ft) - (li->pos.y + li->dir.y * ft);
            d.z = (em->pos.z + em->dir.z * ft) - (li->pos.z + li->dir.z * ft);

            new_distance = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);

            if (new_distance < distance) {
              if (li->soundSpeed - relspeed >= 1e-05f) {
                *doppler = li->soundSpeed / (li->soundSpeed - relspeed);
              } else {
                *doppler = 1e05f * li->soundSpeed;
              }
            } else {
              *doppler = li->soundSpeed / (li->soundSpeed + relspeed);
            }
          }
        }

        if (distance != 0.f) {
          salApplyMatrix(&li->mat, &em->pos, &p);

          if (p.z <= 0.f) {
            pan.z += -li->surroundDisFront < p.z ? -p.z / li->surroundDisFront : 1.f;
          } else {
            pan.z += li->surroundDisBack > p.z ? -p.z / li->surroundDisBack : -1.f;
          }

          if (p.x != 0.f || p.y != 0.f || p.z != 0.f) {
            salNormalizeVector(&p);
          }

          pan.x += p.x;
          pan.y -= p.y;
          if (em->flags & 0x80) {
            if (minOcc > 0.f) {
              minOcc = 0.f;
            }
          }
          if (minFilt > filt) {
            minFilt = filt;
          }
        } else {
          *occl = 0.f;
        }
      }
    }
  }

  if (n != 0) {
    *xPan = pan.x / n;
    *yPan = pan.y / n;
    *zPan = pan.z / n;
  }

  if (em->flags & 0x80) {
    if (em->flags & 0x100) {
      *occl = minOcc < minFilt ? minFilt : minOcc;
    } else {
      *occl = minOcc;
    }
  } else if (em->flags & 0x100) {
    *occl = minFilt;
  } else {
    *occl = 0.f;
  }
}

typedef struct START_LIST {
  // total size: 0x20
  struct START_LIST* next; // offset 0x0, size 0x4
  f32 vol;                 // offset 0x4, size 0x4
  f32 xPan;                // offset 0x8, size 0x4
  f32 yPan;                // offset 0xC, size 0x4
  f32 zPan;                // offset 0x10, size 0x4
  f32 pitch;               // offset 0x14, size 0x4
  f32 occl;                // offset 0x18, size 0x4
  SND_EMITTER* em;         // offset 0x1C, size 0x4
} START_LIST;

typedef struct RUN_LIST {
  // total size: 0xC
  struct RUN_LIST* next; // offset 0x0, size 0x4
  f32 vol;               // offset 0x4, size 0x4
  SND_EMITTER* em;       // offset 0x8, size 0x4
} RUN_LIST;

typedef struct START_GROUP {
  // total size: 0x10
  unsigned long id;          // offset 0x0, size 0x4
  struct START_LIST* list;   // offset 0x4, size 0x4
  struct RUN_LIST* running;  // offset 0x8, size 0x4
  unsigned short numRunning; // offset 0xC, size 0x2
} START_GROUP;

static u8 clip127(u8 v) {
  if (v > 0x7f) {
    return 0x7f;
  }
  return v;
}

static u16 clip3FFF(f32 v) {
  if (v > 16383.f) {
    return 0x3fff;
  }
  return (u16)v;
}

static void SetFXParameters(SND_EMITTER* const em, f32 vol, f32 xPan, f32 yPan, f32 zPan,
                            f32 doppler, f32 occl) {
  SND_VOICEID vid;
  u8 i;
  SND_PARAMETER* pPtr;

  vid = em->vid;
  if ((em->flags & 0x100000) != 0) {
    synthFXSetCtrl(vid, SND_MIDICTRL_VOLUME, clip127((em->fade * vol) * 127.f));
  } else {
    synthFXSetCtrl(vid, SND_MIDICTRL_VOLUME, clip127(vol * 127.f));
  }

  synthFXSetCtrl(vid, SND_MIDICTRL_PANNING, clip127((1.f + xPan) * 64.f));
  synthFXSetCtrl(vid, SND_MIDICTRL_SPANNING, clip127((1.f - zPan) * 64.f));
  synthFXSetCtrl14(vid, SND_MIDICTRL_DOPPLER, clip3FFF(doppler * 8192.f));

  if ((em->flags & 0x180) != 0) {
    if (occl != 0.f) {
      synthFXSetCtrl14(vid, 0x1f, clip3FFF(occl * 16383.f));
      synthFXSetCtrl(vid, 0x4f, 0x7f);
    } else {
      synthFXSetCtrl(vid, 0x4f, 0);
    }
  }

  if (em->paraInfo != NULL) {
    pPtr = em->paraInfo->paraArray;
    for (i = 0; i < em->paraInfo->numPara; ++pPtr, ++i) {
      if ((pPtr->ctrl & 0xFF00) == 0) {
        if (pPtr->ctrl < 0x40 || pPtr->ctrl == 0x80 || pPtr->ctrl == 0x84) {
          synthFXSetCtrl14(vid, pPtr->ctrl, (pPtr->paraData).value14);
        } else {
          synthFXSetCtrl(vid, pPtr->ctrl, (pPtr->paraData).value7);
        }
      }
    }
  }
}

static inline void EmitterShutdown(SND_EMITTER* em) {
  if (em->next != NULL) {
    em->next->prev = em->prev;
  }

  if (em->prev != NULL) {
    em->prev->next = em->next;
  } else {
    s3dEmitterRoot = em->next;
  }

  em->flags &= 0xFFFF;
  if (em->vid != -1) {
    synthSendKeyOff(em->vid);
  }
}


static START_GROUP startGroup[64];
static RUN_LIST runList[128];



bool sndUpdateEmitter(SND_EMITTER* em, SND_FVECTOR* pos, SND_FVECTOR* dir, u8 maxVol,
                      SND_ROOM* room) {
  if (sndActive) {
    hwDisableIrq();

    *(SND_FVECTOR*)((u8*)em + 0x10) = *pos;
    *(SND_FVECTOR*)((u8*)em + 0x1c) = *dir;
    *(f32*)((u8*)em + 0x2c) = maxVol / 127.f;
    if (*(f32*)((u8*)em + 0x30) > *(f32*)((u8*)em + 0x2c)) {
      *(f32*)((u8*)em + 0x30) = *(f32*)((u8*)em + 0x2c);
    }
    hwEnableIrq();
    return TRUE;
  }

  return FALSE;
}

bool sndCheckEmitter(SND_EMITTER* em) {
  if (sndActive) {
    return (*(u32*)((u8*)em + 0xc) & 0x10000) != 0;
  }
  return FALSE;
}

// Not reconstructed (this game does not link them): AddEmitter, sndAddEmitter, sndAddEmitterEx, sndAddEmitterPara,
// sndAddEmitterParaEx, sndAddEmitter2Studio, sndAddEmitter2StudioEx, sndAddEmitter2StudioPara,
// sndAddEmitter2StudioParaEx.
bool sndRemoveEmitter(SND_EMITTER* em) {
  if (sndActive) {
    hwDisableIrq();
    if (em->flags & 0x10000) {
      EmitterShutdown(em);
    }

    hwEnableIrq();
    return TRUE;
  }

  return FALSE;
}

SND_VOICEID sndEmitterVoiceID(SND_EMITTER* em) {
  SND_VOICEID ret;

  ret = 0xffffffff;
  if (sndActive != FALSE) {
    hwDisableIrq();
    if ((em->flags & 0x10000) != 0) {
      ret = em->vid;
    }
    hwEnableIrq();
  }
  return ret;
}

void s3dKillAllEmitter() {
  struct SND_EMITTER* em;
  struct SND_EMITTER* nem;

  em = s3dEmitterRoot;
  while (em != NULL) {
    nem = em->next;
    sndRemoveEmitter(em);
    em = nem;
  }
}

void s3dKillEmitterByFXID(FX_TAB* fxTab, unsigned long num) {
  struct SND_EMITTER* em;
  struct SND_EMITTER* nem;
  unsigned long i;

  for (em = s3dEmitterRoot; em != NULL; em = nem) {
    nem = em->next;
    for (i = 0; i < num; ++i) {
      if (em->fxid == fxTab[i].id) {
        sndRemoveEmitter(em);
        break;
      }
    }
  }
}

static inline void MakeListenerMatrix(SND_LISTENER* li) {
  struct SND_FMATRIX mat;
  salCrossProduct(&li->right, &li->heading, &li->up);
  mat.m[0][0] = li->right.x;
  mat.m[1][0] = li->right.y;
  mat.m[2][0] = li->right.z;
  mat.m[0][1] = li->up.x;
  mat.m[1][1] = li->up.y;
  mat.m[2][1] = li->up.z;
  mat.m[0][2] = -li->heading.x;
  mat.m[1][2] = -li->heading.y;
  mat.m[2][2] = -li->heading.z;
  mat.t[0] = li->pos.x;
  mat.t[1] = li->pos.y;
  mat.t[2] = li->pos.z;
  salInvertMatrix(&li->mat, &mat);
}

bool sndUpdateListener(SND_LISTENER* li, SND_FVECTOR* pos, SND_FVECTOR* dir, SND_FVECTOR* heading,
                       SND_FVECTOR* up, u8 vol, SND_ROOM* room) {
  if (sndActive) {
    hwDisableIrq();
    li->pos = *pos;
    li->dir = *dir;
    li->heading = *heading;
    li->up = *up;

    MakeListenerMatrix(li);
    li->vol = vol / 127.f;
    hwEnableIrq();
    return TRUE;
  }

  return FALSE;
}

// Not reconstructed (this game does not link them): sndAddListenerEx, sndAddListener, sndRemoveListener.



static u8 startGroupNum;            // size: 0x1
extern START_LIST startListNum[64]; // size: 0x700
static u8 startListNumnum;          // size: 0x1

static u8 runListNum;               // size: 0x1
static void (*s3dOcclusionCallback)(struct SND_EMITTER*, SND_FVECTOR*, SND_FVECTOR*, void*, SND_FVECTOR*, SND_FVECTOR*, f32*, f32*) = NULL;

static inline void ClearStartList() {
  startGroupNum = 0;
  startListNumnum = 0;
  runListNum = 0;
}

// BUG: no bound checks here (AddStartingEmitter has them): a 65th group overruns startGroup[64] and a
// 129th running emitter overruns runList[128].
static inline void AddRunningEmitter(SND_EMITTER* em, f32 vol) {
  long i;
  RUN_LIST* rl;
  RUN_LIST* lrl;

  for (i = 0; i < startGroupNum; ++i) {
    if (em->group == startGroup[i].id) {
      break;
    }
  }

  if (i == startGroupNum) {
    startGroup[i].list = NULL;
    startGroup[i].running = NULL;
    startGroup[i].numRunning = 0;
    startGroup[i].id = em->group;
    ++startGroupNum;
  }

  ++startGroup[i].numRunning;

  lrl = NULL;
  for (rl = startGroup[i].running; rl != NULL; rl = rl->next) {
    if (rl->vol > vol) {
      break;
    }
    lrl = rl;
  }

  if (lrl == NULL) {
    startGroup[i].running = &runList[runListNum];
  } else {
    lrl->next = &runList[runListNum];
  }

  runList[runListNum].next = rl;
  runList[runListNum].em = em;
  runList[runListNum++].vol = vol;
}

static bool AddStartingEmitter(SND_EMITTER* em, f32 vol, f32 xPan, f32 yPan, f32 zPan, f32 pitch, f32 occl) {
  long i;
  START_LIST* sl;

  for (i = 0; i < startGroupNum; ++i) {
    if (em->group == startGroup[i].id) {
      break;
    }
  }

  if (i == startGroupNum) {
    if (startGroupNum == 64) {
      return FALSE;
    }

    startGroup[i].list = NULL;
    startGroup[i].running = NULL;
    startGroup[i].numRunning = 0;
    startGroup[i].id = em->group;
    ++startGroupNum;
  }

  if (startListNumnum == 64) {
    return FALSE;
  }

  sl = startGroup[i].list;

  if (sl != NULL) {
    for (; sl->next != NULL; sl = sl->next) {
      if (sl->vol < vol) {
        break;
      }
    }
    startListNum[startListNumnum].next = sl->next;
    sl->next = &startListNum[startListNumnum];
  } else {
    startListNum[startListNumnum].next = startGroup[i].list;
    startGroup[i].list = &startListNum[startListNumnum];
  }

  startListNum[startListNumnum].em = em;
  startListNum[startListNumnum].pitch = pitch;
  startListNum[startListNumnum].occl = occl;
  startListNum[startListNumnum].xPan = xPan;
  startListNum[startListNumnum].yPan = yPan;
  startListNum[startListNumnum].zPan = zPan;
  startListNum[startListNumnum++].vol = vol;

  return TRUE;
}

int synthCheckFXRealloc(unsigned short fid);
static void StartContinousEmitters() {
  f32 dv;

  if (s3dUseLegacyLogic) {
    long i;
    START_LIST* sl;
    SND_EMITTER* em;
  for (i = 0; i < startGroupNum; ++i) {

    for (sl = startGroup[i].list; sl != NULL; sl = sl->next) {
      if ((startGroup[i].running != NULL) &&
          !(((s3dUseMaxVoices != '\0' && ((startGroup[i].id & 0x80000000) != 0)) &&
             (startGroup[i].numRunning < startGroup[i].list->em->maxVoices)))) {

        dv = sl->vol - (startGroup[i].running)->vol;
        if (dv <= 0.08f) {
          continue;
        } else if (dv <= 0.15f) {
          if (++sl->em->VolLevelCnt < 20) {
            continue;
          }
        } else {
          sl->em->VolLevelCnt = 0;
        }
      }
      em = sl->em;
      {
        u8 key = s3dGetEmitterKey(em);
        if (
          (em->vid = synthFXStart(em->fxid, key, 127, 64, em->studio, (em->flags & 0x10) != 0))
          == -1) {
        if (!(em->flags & 0x2)) {
          em->flags |= 0x40000;
          em->flags &= ~0x20000;
        }
      } else {
        if (!(em->flags & 0x20)) {
          em->flags |= 0x100000;
          em->fade = 0.f;
        } else {
          em->fade = 1.f;
        }
        SetFXParameters(em, sl->vol, sl->xPan, sl->yPan, sl->zPan, sl->pitch, sl->occl);
        em->flags &= ~0x20000;
        ++startGroup[i].numRunning;
        if (startGroup[i].running != NULL) {
          startGroup[i].running = startGroup[i].running->next;
        }
      }
      }
    }
  }
  } else {
    long i;
    START_LIST* sl;
    SND_EMITTER* em;
  for (i = 0; i < startGroupNum; ++i) {
    for (sl = startGroup[i].list; sl != NULL; sl = sl->next) {
      if (startGroup[i].running != NULL) {
        if (!(s3dUseMaxVoices != 0 && synthCheckFXRealloc(startGroup[i].list->em->fxid) == 0)) {
          dv = sl->vol - (startGroup[i].running)->vol;
          if (dv <= 0.08f) {
            continue;
          } else if (dv <= 0.15f) {
            if (++sl->em->VolLevelCnt < 20) {
              continue;
            }
          } else {
            sl->em->VolLevelCnt = 0;
          }
          voiceKillSound(startGroup[i].running->em->vid);
          startGroup[i].running = startGroup[i].running->next;
          --startGroup[i].numRunning;
        }
      }
      em = sl->em;
      {
        u8 key = s3dGetEmitterKey(em);
      if ((em->vid = synthFXStart(em->fxid, key, 127, 64, em->studio, (em->flags & 0x10) != 0))
          == -1) {
        if (!(em->flags & 0x2)) {
          em->flags |= 0x40000;
          em->flags &= ~0x20000;
        }
      } else {
        if (!(em->flags & 0x20)) {
          em->flags |= 0x100000;
          em->fade = 0.f;
        } else {
          em->fade = 1.f;
        }
        SetFXParameters(em, sl->vol, sl->xPan, sl->yPan, sl->zPan, sl->pitch, sl->occl);
        em->flags &= ~0x20000;
        {
          RUN_LIST* rl;
          for (rl = startGroup[i].running; rl != NULL; rl = rl->next) {
            if (rl->vol >= sl->vol) {
              RUN_LIST* nr = &runList[runListNum];
              f32 sv;
              nr->em = em;
              sv = sl->vol;
              startGroup[i].running = nr;
              nr->vol = sv;
              nr->next = rl;
              ++startGroup[i].numRunning;
              ++runListNum;
              goto next_sl;
            }
          }
        }
      }
      }
    next_sl:;
    }
  }
  }
}

static START_LIST startListNum[64];

void s3dHandle() {

  SND_EMITTER* em;
  SND_EMITTER* nem;
  f32 vol;
  f32 xPan;
  f32 yPan;
  f32 zPan;
  f32 pitch;
  f32 occl;

  if (s3dCallCnt != 0) {
    --s3dCallCnt;
    return;
  }
  s3dCallCnt = 3; // the rest of s3dHandle runs on every fourth call
  ClearStartList();
  em = s3dEmitterRoot;
  for (; em != NULL; em = nem) {
    nem = em->next;
    if ((em->flags & 0x40000) != 0) {
      EmitterShutdown(em);
      continue;
    }

    // BUG: only this call sets vol, xPan, yPan, zPan, pitch and occl; an emitter with neither bit
    // (not continuous, no start pending) that still has a voice reads them unset further down.
    if ((em->flags & 0x20001) != 0) {
      CalcEmitter(em, &vol, &pitch, &xPan, &yPan, &zPan, &occl);
    }

    if (!(em->flags & 0x80000)) {
      if (em->flags & 0x20000) {
        if (vol == 0.f && em->flags & SND_EMITTER_PAUSABLE) {
          em->flags |= 0x80000;
          em->flags &= ~0x20000;
          goto found_emitter;
        } else if (vol == 0.f && em->flags & SND_EMITTER_NOSILENTSTART) {
          EmitterShutdown(em);
          continue;
        }

        if (em->flags & SND_EMITTER_CONTINUOUS) {
          if (AddStartingEmitter(em, vol, xPan, yPan, zPan, pitch, occl)) {
            continue;
          }
        } else {
          u8 key = s3dGetEmitterKey(em);
          if ((em->vid = synthFXStart(em->fxid, key, 127, 64, em->studio, (em->flags & SND_EMITTER_ITD) != 0)) == -1) {
            if (!(em->flags & SND_EMITTER_RESTARTABLE)) {
              em->flags |= 0x40000;
              em->flags &= ~0x20000;
            } else {
              continue;
            }
          }
        }
      } else if ((em->vid = sndFXCheck(em->vid)) == -1) {
        if ((em->flags & SND_EMITTER_RESTARTABLE)) {
          em->flags |= 0x20000;
        } else {
          em->flags |= 0x40000;
        }
      }

    found_emitter:
      if (em->vid != -1) {
        if ((em->flags & SND_EMITTER_CONTINUOUS) != 0) {
          AddRunningEmitter(em, vol);
        }
        if ((vol == 0.f) && ((em->flags & SND_EMITTER_PAUSABLE) != 0)) {
          synthSendKeyOff(em->vid);
          em->vid = 0xffffffff;
          if ((em->flags & SND_EMITTER_RESTARTABLE)) {
            em->flags |= 0x80000;
          } else {
            em->flags |= 0x40000;
          }
        } else {
          SetFXParameters(em, vol, xPan, yPan, zPan, pitch, occl);
        }
      }
      if ((em->flags & 0x100000) != 0) {
        em->fade += .3f;
        if (em->fade >= 1.f) {
          em->flags &= ~0x100000;
        }
      }
    } else if (vol != 0.f) {
      em->flags &= ~0x80000;
      em->flags |= 0x20000;
    }
  }
  StartContinousEmitters();
}

// Not reconstructed (this game does not link them): sndSetup3DStudios, sndGet3DParameters.
void s3dInit(u32 flags) {
  
  s3dEmitterRoot = 0;
  s3dListenerRoot = 0;
  snd_used_studios = 0;
  snd_base_studio = 1;
  snd_max_studios = 3;
  s3dCallCnt = 0;
  s3dUseMaxVoices = ((flags & 2) != 0);
  s3dUseLegacyLogic = ((flags & 4) != 0);
}

static inline void s3dExit() {}

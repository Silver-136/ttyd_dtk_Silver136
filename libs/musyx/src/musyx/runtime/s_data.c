#define _MATH_H_

#include "musyx/assert.h"
#include "musyx/hardware.h"
#include "musyx/s3d.h"
#include "musyx/seq.h"
#include "musyx/synth.h"
#include "musyx/synthdata.h"

typedef struct GSROOT {
  struct GSROOT* next;
  s32 id;
  s16 sp;
  GSTACK stack[128];
  ARAMInfo aramInfo;
} GSROOT;

static GSROOT gsDefault;
static s32 gsUnused;
static GSROOT* gsRoot;
static GSROOT* gsCurrent;
static s32 gsNextID;

void dataInitStack(unsigned long aramBase, unsigned long aramSize) {
  gsRoot = NULL;
  gsDefault.id = -2;
  gsDefault.sp = 0;
  gsDefault.aramInfo.aramBase = aramBase;
  gsDefault.aramInfo.aramWrite = aramBase;
  gsDefault.aramInfo.aramTop = aramBase + aramSize;
  gsDefault.next = NULL;
  gsRoot = &gsDefault;
  gsNextID = 0;
  gsCurrent = &gsDefault;
}

ARAMInfo* dataARAMDefaultGetInfo() { return &gsDefault.aramInfo; }

static inline MEM_DATA* GetPoolAddr(u16 id, MEM_DATA* m) {
  while (m->nextOff != 0xFFFFFFFF) {
    if (m->id == id) {
      return m;
    }

    m = (MEM_DATA*)((u8*)m + m->nextOff);
  }
  return NULL;
}

static inline MEM_DATA* GetMacroAddr(u16 id, POOL_DATA* pool) {
  return pool == NULL ? NULL : GetPoolAddr(id, (MEM_DATA*)((u8*)pool + pool->macroOff));
}

static inline MEM_DATA* GetCurveAddr(u16 id, POOL_DATA* pool) {
  return pool == NULL ? NULL : GetPoolAddr(id, (MEM_DATA*)((u8*)pool + pool->curveOff));
}
static inline MEM_DATA* GetKeymapAddr(u16 id, POOL_DATA* pool) {
  return pool == NULL ? NULL : GetPoolAddr(id, (MEM_DATA*)((u8*)pool + pool->keymapOff));
}
static inline MEM_DATA* GetLayerAddr(u16 id, POOL_DATA* pool) {
  return pool == NULL ? NULL : GetPoolAddr(id, (MEM_DATA*)((u8*)pool + pool->layerOff));
}

void InsertData(u16 id, void* data, u8 dataType, u32 remove) {
  MEM_DATA* m; // r30

  switch (dataType) {
  case 0:
    if (!remove) {
      if ((m = GetMacroAddr(id, data)) != NULL) {
        dataInsertMacro(id, &m->data.cmd);

      } else {
        dataInsertMacro(id, NULL);
      }
    } else {
      dataRemoveMacro(id);
    }
    break;
  case 2: {
    id |= 0x4000;
    if (!remove) {
      if ((m = GetKeymapAddr(id, data)) != NULL) {
        dataInsertKeymap(id, &m->data.map);
      } else {
        dataInsertKeymap(id, NULL);
      }
    } else {
      dataRemoveKeymap(id);
    }
  } break;
  case 3: {
    id |= 0x8000;
    if (!remove) {
      if ((m = GetLayerAddr(id, data)) != NULL) {
        dataInsertLayer(id, &m->data.layer.entry, m->data.layer.num);
      } else {
        dataInsertLayer(id, NULL, 0);
      }
    } else {
      dataRemoveLayer(id);
    }
  } break;
  case 4:
    if (!remove) {
      if ((m = GetCurveAddr(id, data)) != NULL) {
        dataInsertCurve(id, &m->data.tab);
      } else {
        dataInsertCurve(id, NULL);
      }
    } else {
      dataRemoveCurve(id);
    }
    break;
  case 1:
    if (!remove) {
      dataAddSampleReference(id, &gsCurrent->aramInfo);
    } else {
      dataRemoveSampleReference(id, &gsCurrent->aramInfo);
    }
    break;
  }
}

static inline void ScanIDList(u16* ref, void* data, u8 dataType, u32 remove) {
  u16 id; // r30

  while (*ref != 0xFFFF) {
    if ((*ref & 0x8000)) {
      id = *ref & 0x3fff;
      while (id <= ref[1]) {
        InsertData(id, data, dataType, remove);
        ++id;
      }
      ref += 2;

    } else {
      InsertData(*ref++, data, dataType, remove);
    }
  }
}

static inline void ScanIDListReverse(u16* refBase, void* data, u8 dataType, u32 remove) {
  s16 id;
  u16* ref;

  if (*refBase != 0xffff) {
    ref = refBase;
    while (*ref != 0xffff) {
      ref++;
    }
    ref--;

    while (ref >= refBase) {
      if (ref != refBase) {
        if ((ref[-1] & 0x8000) != 0) {
          id = *ref;
          while (id >= (s16)(ref[-1] & 0x3fff)) {
            InsertData(id, data, dataType, remove);
            id--;
          }
          ref -= 2;
        } else {
          InsertData(*ref, data, dataType, remove);
          ref--;
        }
      } else {
        InsertData(*ref, data, dataType, remove);
        ref--;
      }
    }
  }
}

static inline void InsertMacros(unsigned short* ref, void* pool) { ScanIDList(ref, pool, 0, 0); }

static inline void InsertCurves(unsigned short* ref, void* pool) { ScanIDList(ref, pool, 4, 0); }

static inline void InsertKeymaps(unsigned short* ref, void* pool) { ScanIDList(ref, pool, 2, 0); }

static inline void InsertLayers(unsigned short* ref, void* pool) { ScanIDList(ref, pool, 3, 0); }

static inline void RemoveMacros(unsigned short* ref) { ScanIDList(ref, NULL, 0, 1); }

static inline void RemoveCurves(unsigned short* ref) { ScanIDList(ref, NULL, 4, 1); }

static inline void RemoveKeymaps(unsigned short* ref) { ScanIDList(ref, NULL, 2, 1); }

static inline void RemoveLayers(unsigned short* ref) { ScanIDList(ref, NULL, 3, 1); }

static inline void InsertSamples(u16* ref, void* samples, void* sdir) {
  samples = hwTransAddr(samples);
  if (dataInsertSDir((SDIR_DATA*)sdir, samples)) {
    ScanIDList(ref, sdir, 1, 0);
  }
}

static inline void RemoveSamples(unsigned short* ref, void* sdir) {
  ScanIDListReverse(ref, NULL, 1, 1);
  dataRemoveSDir(sdir);
}

static inline void InsertFXTab(unsigned short gid, FX_DATA* fd) { dataInsertFX(gid, fd->fx, fd->num); }

static inline void RemoveFXTab(unsigned short gid) { dataRemoveFX(gid); }

void sndSetSampleDataUploadCallback(void* (*callback)(u32, u32), u32 chunckSize) {
  hwSetSaveSampleCallback(callback, chunckSize);
}

bool sndPushGroup(void* prj_data, u16 gid, void* samples, void* sdir, void* pool) {
  GROUP_DATA* g; // r31
  MUSY_ASSERT_MSG(prj_data != NULL, "Project data pointer is NULL");
  MUSY_ASSERT_MSG(sdir != NULL, "Sample directory pointer is NULL");

  if (sndActive && gsCurrent->sp < 128) {
    g = prj_data;

    while (g->nextOff != 0xFFFFFFFF) {
      if (g->id == gid) {
        gsCurrent->stack[gsCurrent->sp].gAddr = g;
        gsCurrent->stack[gsCurrent->sp].prjAddr = prj_data;
        gsCurrent->stack[gsCurrent->sp].sdirAddr = sdir;
        InsertSamples((u16*)((u8*)prj_data + g->sampleOff), samples, sdir);
        InsertMacros((u16*)((u8*)prj_data + g->macroOff), pool);
        InsertCurves((u16*)((u8*)prj_data + g->curveOff), pool);
        InsertKeymaps((u16*)((u8*)prj_data + g->keymapOff), pool);
        InsertLayers((u16*)((u8*)prj_data + g->layerOff), pool);
        if (g->type == 1) {
          InsertFXTab(gid, (FX_DATA*)((u8*)prj_data + g->data.song.normpageOff));
        }
        hwSyncSampleMem();
        ++gsCurrent->sp;
        return TRUE;
      }

      g = (GROUP_DATA*)((u8*)prj_data + g->nextOff);
    }
  }

  MUSY_DEBUG("Group ID=%d could not be pushed.\n", gid);
  return FALSE;
}

/*










*/
bool sndPopGroup() {
  GROUP_DATA* g;
  SDIR_DATA* sdir;
  void* prj;
  FX_DATA* fd;

  MUSY_ASSERT_MSG(sndActive != FALSE, "Sound system is not initialized.");
  MUSY_ASSERT_MSG(gsCurrent->sp != 0, "Soundstack is empty.");
  g = gsCurrent->stack[--gsCurrent->sp].gAddr;
  prj = gsCurrent->stack[gsCurrent->sp].prjAddr;
  sdir = gsCurrent->stack[gsCurrent->sp].sdirAddr;
  hwDisableIrq();

  if (g->type == 1) {
    fd = (FX_DATA*)((u8*)prj + g->data.song.normpageOff);
    s3dKillEmitterByFXID(fd->fx, fd->num);
  } else {
    seqKillInstancesByGroupID(g->id);
  }

  synthKillVoicesByMacroReferences((u16*)((u8*)prj + g->macroOff));
  synthKillVoicesBySampleReferences((u16*)((u8*)prj + g->sampleOff));
  hwEnableIrq();
  RemoveSamples((u16*)((u8*)prj + g->sampleOff), sdir);
  RemoveMacros((u16*)((u8*)prj + g->macroOff));
  RemoveCurves((u16*)((u8*)prj + g->curveOff));
  RemoveKeymaps((u16*)((u8*)prj + g->keymapOff));
  RemoveLayers((u16*)((u8*)prj + g->layerOff));
  if (g->type == 1) {
    RemoveFXTab(g->id);
  }
  return 1;
}

/*












*/

u32 seqPlaySong(u16 sgid, u16 sid, void* arrfile, SND_PLAYPARA* para, u8 irq_call, u8 studio) {
  int i;
  GROUP_DATA* g;
  PAGE* norm;
  PAGE* drum;
  MIDISETUP* midiSetup;
  GSROOT* root;
  u32 seqId;
  void* prj;
  MUSY_ASSERT_MSG(sndActive != FALSE, "Sound system is not initialized.");

  for (root = gsRoot; root != NULL; root = root->next) {
    for (i = 0; i < root->sp; ++i) {
    if (root->stack[i].gAddr->id != sgid) {
      continue;
    }

    if (root->stack[i].gAddr->type == 0) {
      g = root->stack[i].gAddr;
      prj = root->stack[i].prjAddr;
      norm = (PAGE*)((size_t)prj + g->data.song.normpageOff);
      drum = (PAGE*)((size_t)prj + g->data.song.drumpageOff);
      midiSetup = (MIDISETUP*)((size_t)prj + g->data.song.midiSetupOff);
      while (midiSetup->songId != 0xFFFF) {
        if (midiSetup->songId == sid) {
          if (irq_call != 0) {
            seqId = seqStartPlay(norm, drum, midiSetup, arrfile, para, studio, sgid);
          } else {
            hwDisableIrq();
            seqId = seqStartPlay(norm, drum, midiSetup, arrfile, para, studio, sgid);
            hwEnableIrq();
          }
          return seqId;
        }

        ++midiSetup;
      }

      MUSY_DEBUG("Song ID=%d is not in group ID=%d.", sid, sgid);
      return 0xffffffff;
    } else {
      MUSY_DEBUG("Group ID=%d is no songgroup.", sgid);
      return 0xffffffff;
    }
  }
  }

  MUSY_DEBUG("Group ID=%d is not on soundstack.", sgid);
  seqId = 0xffffffff;
done:
  return seqId;
}

u32 sndSeqPlayEx(u16 sgid, u16 sid, void* arrfile, SND_PLAYPARA* para, u8 studio) {
  int i;
  u32 seqId;
  GROUP_DATA* g;
  MIDISETUP* midiSetup;
  PAGE* drum;
  PAGE* norm;
  GSROOT* root;
  void* prj;
  for (root = gsRoot; root != NULL; root = root->next) {
    /* fake: r steps one GSTACK per i; reproduces the register use of seqPlaySong(.., 0, ..) inlined */
    GSROOT* r;
    for (i = 0, r = root; i < root->sp; r = (GSROOT*)((u8*)r + sizeof(GSTACK)), ++i) {
      if (r->stack[0].gAddr->id != sgid) {
        continue;
      }
      if (root->stack[i].gAddr->type == 0) {
        g = root->stack[i].gAddr;
        prj = root->stack[i].prjAddr;
        norm = (PAGE*)((size_t)prj + g->data.song.normpageOff);
        drum = (PAGE*)((size_t)prj + g->data.song.drumpageOff);
        midiSetup = (MIDISETUP*)((size_t)prj + g->data.song.midiSetupOff);
        while (midiSetup->songId != 0xFFFF) {
          if (midiSetup->songId == sid) {
            hwDisableIrq();
            seqId = seqStartPlay(norm, drum, midiSetup, arrfile, para, studio, sgid);
            hwEnableIrq();
            goto done;
          }
          ++midiSetup;
        }
        seqId = 0xffffffff; goto done;
      } else {
        seqId = 0xffffffff; goto done;
      }
    }
  }
  seqId = 0xffffffff;
done:
  return seqId;
}

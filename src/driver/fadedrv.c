#include "driver/fadedrv.h"
#include "driver/animdrv.h"
#include "driver/arcdrv.h"
#include "driver/camdrv.h"
#include "driver/imgdrv.h"
#include "driver/dispdrv.h"
#include "driver/offscreendrv.h"
#include "manager/dvdmgr.h"
#include "manager/evtmgr.h"
#include "pmario_sound.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/dvd.h>
#include <string.h>
#include "version.h"
#include "libc_decls.h"

#ifdef VERSION_G8ME01
// US-only named constant (dispdrv reads it by name), followed by a zero word
#pragma force_active on
const f32 float_1_8041f770 = 1.0f;
const f32 float_0_8041f774 = 0.0f;
#pragma force_active reset
#endif

s32 evt_img_alloc_capture(EventEntry* event, BOOL isFirstCall);

static FadeWork* wp;

//local prototypes
void _callback(s32 error, DVDFileInfo* info);
void disp_texture(CameraId camId, void* param);
static f32 z(void);
static void zFill(void);
void disp_maku(CameraId camId, void* param);
void disp_tec(CameraId camId, void* param);
void disp_dummy_black(CameraId camId, void* param);

void _callback(s32 error, DVDFileInfo* info) {
	DVDEntry* entry = info->cb.userData;
	UnpackTexPalette(wp->texture);
	DVDMgrClose(entry);
	wp->loaded = TRUE;
}

void fadeInit(void) {
	FadeEntry* entry;
	int i;

	wp = __memAlloc(HEAP_DEFAULT, sizeof(FadeWork));
	memset(wp, 0, sizeof(FadeWork));

	for (i = 0; i < 5; i++) {
		entry = &wp->entries[i];
		entry->poseId = -1;
		entry->paperPoseId = -1;
		entry->fadeSpotX = 0.0f;
		entry->fadeSpotY = 0.0f;
		entry->animOffsetX = 0.0f;
		entry->animOffsetY = 0.0f;
		entry->scale = 1.0f;
		entry->delayFrames = 0;
	}

	wp->lastEntryId = -1;
	wp->texture = NULL;
	wp->loaded = FALSE;
}

void fadeTexSetup(void) {
	DVDEntry* dvd;
	u32 size;
	char path[128];

	wp->texture = NULL;
	wp->texture = arcOpen("fade.tpl", NULL, NULL);
	if (!wp->texture) {
		sprintf(path, "%s/fade.tpl", getMarioStDvdRoot());
		dvd = DVDMgrOpen(path, 2, 0);
		size = OSRoundUp32B(DVDMgrGetLength(dvd));
		wp->texture = __memAlloc(HEAP_DEFAULT, size);
		dvd->info.cb.userData = dvd;
		DVDMgrReadAsync(dvd, wp->texture, size, 0, _callback);
	}
	else {
		UnpackTexPalette(wp->texture);
		wp->loaded = TRUE;
	}
}

void fadeEntry(s32 type, s32 duration, GXColor color) {
	FadeEntry* entry;
	s32 id;

	if (!type) {
		return;
	}
	if (type == 16) { //type 16 is type 9 but white
		type = 9;
		color = (GXColor){0xF0, 0xF0, 0xF0, 0xFF};
	}
	if (type == 17) {
		type = 10;
		color = (GXColor){0xF0, 0xF0, 0xF0, 0xFF};
	}
	if (type == 18) {
		type = 11;
		color = (GXColor){0xF0, 0xF0, 0xF0, 0xFF};
	}
	if (type == 19) {
		type = 12;
		color = (GXColor){0xF0, 0xF0, 0xF0, 0xFF};
	}
	if (type == 20) {
		type = 14;
		color = (GXColor){0xF0, 0xF0, 0xF0, 0xFF};
	}
	if (type == 21) {
		type = 15;
		color = (GXColor){0xF0, 0xF0, 0xF0, 0xFF};
	}
	if ((type == 1 || type == 2) && duration < 1000) {
		duration = 1000;
	}
	if (type == 7) {
		type = 3;
	}
	else if (type == 8) {
		type = 4;
	}
	else if (((u32)(type - 3) <= 2 || type == 6) && duration < 1000) {
		duration = 1000;
	}
	switch (type) {
		case 0x1: case 0x2: case 0x3: case 0x4: case 0x5: case 0x6: case 0x9: case 0xA: case 0xB: case 0xC: case 0xD: case 0xE: case 0xF:
			id = 0;
			entry = &wp->entries[0];
			fadeReset(2);
			entry->delayFrames = 0;
			switch (type) {
				case 1: case 3: case 5: case 9: case 11: case 14:
					if (duration) {
						entry->delayFrames = 2;
					}
					break;
			}
			break;
		case 0x16: case 0x17: case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E: case 0x1F: case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27: case 0x28: case 0x33:
			id = 1;
			entry = &wp->entries[1];
			break;
		case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2E: case 0x2F: case 0x30:
			id = 2;
			entry = &wp->entries[2];
			fadeReset(0);
			break;
		case 0x31: case 0x32: case 0x34: case 0x35: case 0x36: case 0x37: case 0x38: case 0x39: case 0x3A: case 0x3B: case 0x3C: case 0x3D: case 0x3E:
			id = 3;
			entry = &wp->entries[3];
			switch (type) {
				case 0x32: case 0x35: case 0x36: case 0x38: case 0x3A:
					fadeReset(0);
					break;
			}
			break;
		case 0x3F: case 0x40:
			id = 4;
			entry = &wp->entries[4];
			break;
	}
	entry->flags = 0;
	entry->flags |= 1;
	entry->type = type;
	*(s64*)&entry->durationHi = duration;
	entry->color = color;
	entry->progress = 0.0f;
	entry->rotation = 0.0f;
	entry->elapsed = 0.0f;
	entry->state = 0;
	entry->animOffsetX = 0.0f;
	entry->animOffsetY = 0.0f;
	entry->scale = 1.0f;
	if (type != 0x32 && type != 0x35) {
		entry->callback = NULL;
	}
	wp->lastEntryId = id;
	if (type == 0x22 || type == 0x28) {
		entry->scale = 3.0f;
	}
	switch (type) {
		case 0x2B:
			entry->animFile = "OFF_d_dokan_down";
			entry->animName = "A_1";
			entry->imageType = 2;
			break;
		case 0x2C:
			entry->animFile = "OFF_d_dokan_down";
			entry->animName = "A_2";
			entry->imageType = 2;
			break;
		case 0x29:
			entry->animFile = "OFF_d_dokan_up";
			entry->animName = "A_1";
			entry->imageType = 2;
			break;
		case 0x2A:
			entry->animFile = "OFF_d_dokan_up";
			entry->animName = "A_2";
			entry->imageType = 2;
			break;
		case 0x2D:
			entry->animFile = "OFF_d_dokan_left";
			entry->animName = "A_1";
			entry->imageType = 2;
			break;
		case 0x2E:
			entry->animFile = "OFF_d_dokan_left";
			entry->animName = "A_2";
			entry->imageType = 2;
			break;
		case 0x2F:
			entry->animFile = "OFF_d_dokan_right";
			entry->animName = "A_1";
			entry->imageType = 2;
			break;
		case 0x30:
			entry->animFile = "OFF_d_dokan_right";
			entry->animName = "A_2";
			entry->imageType = 2;
			break;
		case 0x16:
			entry->animFile = "OFF_d_maku_up_down";
			entry->animName = "A_1";
			entry->imageType = 2;
			break;
		case 0x17:
			entry->animFile = "OFF_d_maku_up_down";
			entry->animName = "A_2";
			entry->imageType = 2;
			break;
		case 0x18:
		case 0x1B:
		case 0x3F:
			entry->animFile = "OFF_d_maku_left_right";
			entry->animName = "A_1";
			entry->imageType = 2;
			break;
		case 0x1A:
			entry->animFile = "OFF_d_maku_left_right";
			entry->animName = "Z_1";
			entry->imageType = 2;
			break;
		case 0x19:
		case 0x1C:
		case 0x40:
			entry->animFile = "OFF_d_maku_left_right";
			entry->animName = "A_2";
			entry->imageType = 2;
			break;
		case 0x1D:
		case 0x23:
			entry->animFile = "OFF_d_maku_center";
			entry->animName = "A_1";
			entry->imageType = 2;
			break;
		case 0x1E:
		case 0x24:
			entry->animFile = "OFF_d_maku_center";
			entry->animName = "A_2";
			entry->imageType = 2;
			break;
		case 0x1F:
		case 0x22:
		case 0x25:
		case 0x28:
			entry->animFile = "OFF_d_maku_center";
			entry->animName = "Z_2";
			entry->imageType = 2;
			break;
		case 0x20:
		case 0x26:
			entry->animFile = "OFF_d_maku_center";
			entry->animName = "Z_1";
			entry->imageType = 2;
			break;
		case 0x31:
		case 0x34:
			entry->animFile = "OFF_d_meku";
			entry->animName = "Z_1";
			entry->imageType = 2;
			break;
		case 0x32:
		case 0x35:
			entry->animFile = "OFF_d_meku";
			entry->animName = "A_1";
			entry->imageType = 2;
			break;
		case 0x33:
			entry->animFile = "OFF_d_four_meku_5sec";
			entry->animName = "A_1";
			entry->imageType = 2;
			break;
		case 0x36:
			entry->animFile = "OFF_f_in_out_mario";
			entry->animName = "A_3";
			entry->imageType = 2;
			break;
		case 0x37:
			entry->animFile = "OFF_f_in_out_mario";
			entry->animName = "A_1";
			entry->imageType = 2;
			break;
		case 0x38:
			entry->animFile = "OFF_f_in_out_peach";
			entry->animName = "A_3";
			entry->imageType = 2;
			break;
		case 0x39:
			entry->animFile = "OFF_f_in_out_peach";
			entry->animName = "A_1";
			entry->imageType = 2;
			break;
		case 0x3A:
			entry->animFile = "OFF_f_in_out_kuppa";
			entry->animName = "A_3";
			entry->imageType = 2;
			break;
		case 0x3B:
			entry->animFile = "OFF_f_in_out_kuppa";
			entry->animName = "A_1";
			entry->imageType = 2;
			break;
		case 0x3C:
			entry->animFile = "OFF_f_in_out_mario";
			entry->animName = "A_2";
			entry->imageType = 2;
			break;
		case 0x3D:
			entry->animFile = "OFF_f_in_out_peach";
			entry->animName = "A_2";
			entry->imageType = 2;
			break;
		case 0x3E:
			entry->animFile = "OFF_f_in_out_kuppa";
			entry->animName = "A_2";
			entry->imageType = 2;
			break;
	}
}

void fadeTecOn(void) {
	wp->flags |= 0x1000;
}

void fadeTecOff(void) {
	wp->flags &= ~0x1000;
}

void fadeTecSoftOn(void) {
	wp->flags |= 0x1010;
	wp->softness = 0.0f;
}

void fadeTecSoftOff(void) {
	wp->flags |= 0x1020;
	wp->softness = 1.0f;
}

void fadeSoftFocusOn(void) {
	wp->flags |= 0x2000;
}

void fadeSoftFocusOff(void) {
	wp->flags &= ~0x2000;
}

void fadeWhiteOutOn(void) {
	wp->flags |= 0x4000;
}

void fadeWhiteOutOff(void) {
	wp->flags &= ~0x4000;
}

void fadeSetSpotPos(f32 x, f32 y) {
	FadeEntry* entry = &wp->entries[0];
	entry->fadeSpotX = x;
	entry->fadeSpotY = y;
}

void fadeSetAnimOfsPos(f32 x, f32 y) {
	FadeEntry* entry = &wp->entries[1];
	entry->animOffsetX = x;
	entry->animOffsetY = y;
}

void fadeSetAnimVirtualPos(f32 x, f32 y, f32 z) {
	FadeEntry* entry = &wp->entries[1];
	entry->VirtualPos = (Vec){x, y, z};
	entry->flags |= 4;
}

void fadeSetOffscreenCallback(OffscreenCallback callback, void* param) {
	FadeEntry* entry = &wp->entries[3];
	entry->callback = callback;
	entry->offparam = param;
}

static inline void fadeCapture(const char* name, const char* off, s32 mode) {
	EventEntry evt;
	EvtWord args[10];

	evt.args = args;
	args[0] = (EvtWord)name;
	args[1] = 0;
	args[2] = (EvtWord)off;
	args[3] = mode;
	args[4] = 0;
	args[5] = 0;
	args[6] = 608;
	args[7] = 480;
	evt_img_alloc_capture(&evt, TRUE);
}

#define FADE_DURATION(e) (*(s64*)&(e)->durationHi)
#define SFX_ON(id) psndSFXOn(SFX_NAME_WORD(id))

void fadeMain(void) {
	FadeEntry* entry;
	s32 i;

	entry = &wp->entries[0];
	for (i = 0; i < 5; i++, entry++) {
		if (!(entry->flags & 1)) {
			continue;
		}
		switch (entry->type) {
			case 1: case 3: case 5: case 9: case 11: case 14:
				switch (entry->state) {
					case 0:
						if (FADE_DURATION(entry) != 0) {
							if ((s32)entry->delayFrames != 0) {
								entry->delayFrames--;
							}
							else {
								entry->elapsed += VERSION_PICK(16.666666f, 1000.0f / gp->framerate);
							}
							entry->progress = entry->elapsed / FADE_DURATION(entry);
							entry->progress = 1.0f - entry->progress;
							entry->rotation += 10.0f;
							if (entry->progress < 0.0f) {
								entry->progress = 0.0f;
								entry->flags |= 2;
								entry->state++;
							}
						}
						else {
							entry->progress = 0.0f;
							entry->flags |= 2;
							entry->state++;
						}
						break;
				}
				break;
			case 2: case 4: case 6: case 10: case 12: case 13: case 15:
				switch (entry->state) {
					case 0:
						if (FADE_DURATION(entry) != 0) {
							entry->elapsed += VERSION_PICK(16.666666f, 1000.0f / gp->framerate);
							entry->progress = entry->elapsed / FADE_DURATION(entry);
							entry->rotation -= 10.0f;
							if (entry->progress > 1.0f) {
								entry->progress = 1.0f;
								entry->flags |= 2;
								entry->state++;
							}
						}
						else {
							entry->progress = 1.0f;
							entry->flags |= 2;
							entry->state++;
						}
						break;
				}
				break;
			case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2E: case 0x2F: case 0x30:
				switch (entry->state) {
					case 0:
						if ((s32)animGroupBaseAsync(entry->animFile, entry->imageType, NULL) == 0) {
							if (entry->paperPoseId < 0 && (entry->type == 0x2A || entry->type == 0x2C || entry->type == 0x2E || entry->type == 0x30)) {
								wp->flags |= 0x8000;
							}
							break;
						}
						wp->flags &= ~0x8000;
						if (entry->paperPoseId >= 0) {
							imgRelease(entry->image, entry->imageType);
							animPaperPoseRelease(entry->paperPoseId);
							entry->paperPoseId = -1;
							entry->image = NULL;
						}
						entry->paperPoseId = animPaperPoseEntry(entry->animFile, entry->imageType);
						imgEntry("fImg_dokan", entry->imageType);
						entry->image = imgNameToPtr("fImg_dokan", entry->imageType);
						entry->image->poseId = animPaperPoseGetId(entry->animFile, entry->imageType);
						entry->image->animTime = animTimeGetTime(entry->imageType);
						entry->image->animName = entry->animName;
						imgClearVirtualPoint(entry->image);
						fadeCapture("fImg_dokan", NULL, 0);
						switch (entry->type) {
							case 0x2D:
								entry->sfxId = SFX_ON(0x403AB);
								break;
							case 0x2F:
								entry->sfxId = SFX_ON(0x403AC);
								break;
							case 0x29:
								entry->sfxId = SFX_ON(0x403AD);
								break;
							case 0x2B:
								entry->sfxId = SFX_ON(0x403AE);
								break;
							case 0x2E:
								entry->sfxId = SFX_ON(0x403AF);
								break;
							case 0x30:
								entry->sfxId = SFX_ON(0x403B0);
								break;
							case 0x2A:
								entry->sfxId = SFX_ON(0x403B1);
								break;
							case 0x2C:
								entry->sfxId = SFX_ON(0x403B2);
								break;
						}
						entry->state++;
						break;
					case 1:
						if (entry->image->loopTimes >= 1.0f) {
							entry->flags |= 2;
							switch (entry->type) {
								case 0x2A: case 0x2C: case 0x2E: case 0x30:
									imgRelease(entry->image, entry->imageType);
									animPaperPoseRelease(entry->paperPoseId);
									entry->paperPoseId = -1;
									entry->image = NULL;
									break;
							}
							entry->state++;
						}
						break;
				}
				break;
			case 0x31: case 0x34: case 0x37: case 0x39: case 0x3B: case 0x3C: case 0x3D: case 0x3E:
				switch (entry->state) {
					case 0:
						if ((s32)animGroupBaseAsync(entry->animFile, entry->imageType, NULL) == 0) {
							break;
						}
						if (entry->paperPoseId >= 0) {
							imgRelease(entry->image, entry->imageType);
							animPaperPoseRelease(entry->paperPoseId);
							entry->paperPoseId = -1;
							entry->image = NULL;
						}
						entry->paperPoseId = animPaperPoseEntry(entry->animFile, entry->imageType);
						animPoseSetMaterialFlagOn(entry->paperPoseId, 0x1800);
						imgEntry("fImg_peron", entry->imageType);
						entry->image = imgNameToPtr("fImg_peron", entry->imageType);
						entry->image->poseId = animPaperPoseGetId(entry->animFile, entry->imageType);
						entry->image->animTime = animTimeGetTime(entry->imageType);
						entry->image->animName = entry->animName;
						imgClearVirtualPoint(entry->image);
						switch (entry->type) {
							case 0x34:
								offscreenEntry("fOff_peron");
								fadeCapture("fImg_peron", "fOff_peron", 0);
								break;
							case 0x37: case 0x39: case 0x3B:
								fadeCapture("fImg_peron", NULL, 2);
								entry->image->camId = 10;
								entry->image->dispCamId = 10;
								SFX_ON(0x403B4);
								break;
							case 0x3C: case 0x3D: case 0x3E:
								fadeCapture("fImg_peron", NULL, 2);
								entry->image->camId = 9;
								entry->image->dispCamId = 9;
								break;
							default:
								fadeCapture("fImg_peron", NULL, 2);
								break;
						}
						entry->state++;
						break;
					case 1:
						if (entry->image->loopTimes >= 1.0f) {
							switch (entry->type) {
								case 0x37: case 0x39: case 0x3B: case 0x3C: case 0x3D: case 0x3E:
									entry->image->animTime = animTimeGetTime(entry->imageType);
									entry->image->animName = "A_2";
									break;
								default:
									entry->flags |= 2;
									break;
							}
							entry->state++;
						}
						break;
					case 2:
						if (entry->image->loopTimes >= 1.0f) {
							entry->flags |= 2;
							entry->state++;
						}
						break;
				}
				break;
			case 0x32: case 0x35: case 0x36: case 0x38: case 0x3A:
				switch (entry->state) {
					case 0:
						entry->image = imgNameToPtr("fImg_peron", entry->imageType);
						entry->image->poseId = animPaperPoseGetId(entry->animFile, entry->imageType);
						entry->image->animTime = animTimeGetTime(entry->imageType);
						entry->image->animName = entry->animName;
						imgClearVirtualPoint(entry->image);
						switch (entry->type) {
							case 0x36: case 0x38: case 0x3A:
								SFX_ON(0x403B5);
								break;
							case 0x32: case 0x35:
								SFX_ON(0x40365);
								break;
						}
						entry->state++;
						break;
					case 1:
						if (entry->image->loopTimes >= 1.0f) {
							entry->flags |= 2;
							imgRelease(entry->image, entry->imageType);
							animPaperPoseRelease(entry->paperPoseId);
							entry->paperPoseId = -1;
							entry->image = NULL;
							if (entry->type == 0x35) {
								offscreenDelete("fOff_peron");
							}
							entry->state++;
						}
						break;
				}
				break;
			case 0x16:
				switch (entry->state) {
					case 0:
						if ((s32)animGroupBaseAsync(entry->animFile, entry->imageType, NULL) == 0) {
							break;
						}
						if (entry->poseId >= 0) {
							animPoseRelease(entry->poseId);
						}
						entry->poseId = animPoseEntry(entry->animFile, entry->imageType);
						animPoseSetMaterialFlagOn(entry->poseId, 0x1800);
						animPoseSetAnim(entry->poseId, entry->animName, TRUE);
						entry->animColor = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
						animPoseSetMaterialFlagOff(entry->poseId, 0x40);
						entry->state++;
					case 1:
						animPoseMain(entry->poseId);
						if (animPoseGetLoopTimes(entry->poseId) >= 1.0f) {
							entry->flags |= 2;
							entry->state++;
						}
						break;
					case 2:
						animPoseMain(entry->poseId);
						break;
				}
				break;
			case 0x17:
				switch (entry->state) {
					case 0:
						if ((s32)animGroupBaseAsync(entry->animFile, entry->imageType, NULL) == 0) {
							break;
						}
						if (entry->poseId >= 0) {
							animPoseRelease(entry->poseId);
						}
						entry->poseId = animPoseEntry(entry->animFile, entry->imageType);
						animPoseSetMaterialFlagOn(entry->poseId, 0x1800);
						animPoseSetAnim(entry->poseId, entry->animName, TRUE);
						entry->animColor = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
						animPoseSetMaterialFlagOff(entry->poseId, 0x40);
						entry->state++;
					case 1:
						if (animPoseGetLoopTimes(entry->poseId) >= 0.5f) {
							entry->animColor.r *= 0.98f;
							entry->animColor.g *= 0.98f;
							entry->animColor.b *= 0.98f;
							animPoseSetMaterialEvtColor(entry->poseId, entry->animColor);
							animPoseSetMaterialFlagOn(entry->poseId, 0x40);
						}
						animPoseMain(entry->poseId);
						if (animPoseGetLoopTimes(entry->poseId) >= 1.0f) {
							entry->flags |= 2;
							entry->state++;
							animPoseRelease(entry->poseId);
							entry->poseId = -1;
						}
						break;
				}
				break;
			case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E: case 0x1F: case 0x20:
			case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x28: case 0x33: case 0x3F: case 0x40:
				switch (entry->state) {
					case 0:
						if ((s32)animGroupBaseAsync(entry->animFile, entry->imageType, NULL) == 0) {
							break;
						}
						switch (entry->type) {
							case 0x18: case 0x1B: case 0x3F:
								entry->sfxId = SFX_ON(0x40325);
								break;
							case 0x19: case 0x1C: case 0x40:
								entry->sfxId = SFX_ON(0x40326);
								break;
							case 0x1D: case 0x23:
								entry->sfxId = SFX_ON(0x40321);
								break;
							case 0x1E: case 0x24:
								entry->sfxId = SFX_ON(0x40322);
								break;
						}
						if (entry->poseId >= 0) {
							animPoseRelease(entry->poseId);
						}
						entry->poseId = animPoseEntry(entry->animFile, entry->imageType);
						animPoseSetMaterialFlagOn(entry->poseId, 0x1800);
						animPoseSetAnim(entry->poseId, entry->animName, TRUE);
						entry->state++;
						if (entry->type == 0x22 || entry->type == 0x28) {
							entry->scale = 3.0f;
							entry->scaleTimer = 0;
						}
					case 1:
						animPoseMain(entry->poseId);
						if (entry->type == 0x22 || entry->type == 0x28) {
							if (!(entry->flags & 8)) {
								if (entry->scaleTimer < FPS_FRAMES(180)) {
									entry->scale = intplGetValue(12, ++entry->scaleTimer, FPS_FRAMES(180), 3.0f, 1.0f);
								}
							}
							else {
								if (entry->scaleTimer < FPS_FRAMES(90)) {
									entry->scale = intplGetValue(12, ++entry->scaleTimer, FPS_FRAMES(90), 1.5f, 1.0f);
								}
							}
						}
						if (animPoseGetLoopTimes(entry->poseId) >= 1.0f) {
							if (entry->type == 0x22 || entry->type == 0x28) {
								if (!(entry->flags & 8)) {
									if (entry->scaleTimer == FPS_FRAMES(180)) {
										entry->flags |= 2;
										entry->state++;
									}
								}
								else {
									if (entry->scaleTimer == FPS_FRAMES(90)) {
										entry->flags |= 2;
										entry->state++;
									}
								}
							}
							else {
								entry->flags |= 2;
								entry->state++;
								psndSFXOff(entry->sfxId);
								if (entry->type == 0x19 || entry->type == 0x1C || entry->type == 0x40) {
									animPoseRelease(entry->poseId);
									entry->poseId = -1;
								}
							}
						}
						break;
					case 2:
						if (entry->poseId >= 0) {
							animPoseMain(entry->poseId);
						}
						break;
				}
				break;
			case 0x21: case 0x27:
				switch (entry->state) {
					case 0:
						entry->scale = 1.0f;
						entry->scaleTimer = 0;
						entry->state++;
					case 1:
						animPoseMain(entry->poseId);
						if (entry->scaleTimer < FPS_FRAMES(120)) {
							entry->scale = intplGetValue(12, ++entry->scaleTimer, FPS_FRAMES(120), 1.0f, 3.0f);
						}
						else {
							entry->flags |= 2;
							entry->state++;
							animPoseRelease(entry->poseId);
							entry->poseId = -1;
						}
						break;
				}
				break;
		}
		switch (entry->type) {
			case 0x16: case 0x17: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27: case 0x28:
				dispEntry(CAMERA_FADE, 0.0f, 0, disp_maku, entry);
				break;
			case 0x1B: case 0x1C: case 0x3F: case 0x40:
				dispEntry(CAMERA_FADE2, 0.0f, 0, disp_maku, entry);
				break;
			case 0x18: case 0x19: case 0x1A: case 0x1D: case 0x1E: case 0x1F: case 0x20: case 0x21: case 0x22: case 0x33:
				dispEntry(CAMERA_3D_IMAGE, 0.0f, 0, disp_maku, entry);
				break;
			case 0x31: case 0x32: case 0x34: case 0x35: case 0x36: case 0x37: case 0x38: case 0x39: case 0x3A:
			case 0x3B: case 0x3C: case 0x3D: case 0x3E:
				if (entry->paperPoseId >= 0 && entry->callback) {
					dispEntry(CAMERA_OFFSCREEN2, 0.0f, 0, entry->callback, entry->offparam);
				}
				break;
			case 0xB: case 0xC: case 0xD:
				dispEntry(CAMERA_2D, 1000.0f, 0, disp_texture, wp);
				break;
			case 5: case 6: case 0xE: case 0xF:
				dispEntry(CAMERA_3D, 0.0f, 7, disp_texture, wp);
				break;
			case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2E: case 0x2F: case 0x30:
				break;
			default:
				dispEntry(CAMERA_FADE, 0.0f, 0, disp_texture, wp);
				break;
		}
	}
	if (wp->flags & 0x3000) {
		dispEntry(CAMERA_2D, 999.0f, 0, disp_tec, wp);
	}
	if (wp->flags & 0x8000) {
		dispEntry(CAMERA_FADE, -100.0f, 0, disp_dummy_black, wp);
	}
	if (wp->flags & 0x4000) {
		dispEntry(CAMERA_3D, 1000.0f, 7, disp_dummy_black, wp);
	}
}


void disp_texture(CameraId camId, void* param) {
	static u8 i4tex[1] = {0xFF};
	FadeWork* work;
	CameraEntry* camera;
	f32 scale;
	f32 p;
	f32 size;
	Mtx mtx1;
	Mtx mtx2;
	Mtx mtx3;
	Mtx mtx4;
	GXTexObj obj;
	f32 proj[7];

	camera = camGetPtr(CAMERA_2D);
	work = wp;
	switch (work->entries[0].type) {
		case 1:
		case 3:
		case 5:
		case 9:
		case 11:
		case 14:
			if (work->entries[0].flags & 2) {
				return;
			}
	}
	GXGetProjectionv(proj);
	GXSetProjection(camera->projection, camera->projType);
	GXSetZCompLoc(GX_TRUE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, (GXLogicOp)7);
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXSetNumChans(0);
	GXSetNumTevStages(1);
	GXSetNumTexGens(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
	GXSetCullMode(GX_CULL_BACK);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	switch (work->entries[0].type) {
		case 1:
		case 2:
			GXLoadPosMtxImm(camera->view, 0);
			GXSetCurrentMtx(0);
			TEXGetGXTexObjFromPalette(wp->texture, &obj, 0);
			GXLoadTexObj(&obj, GX_TEXMAP0);
			if (!(work->entries[0].flags & 2)) {
				p = work->entries[0].progress;
				scale = p * (p * (p * (100.0f * p * p)));
				PSMTXTrans(mtx1, 0.5f, 0.5f, 0.0f);
				PSMTXScale(mtx3, scale, scale, 1.0f);
				PSMTXTrans(mtx2, -0.5f, -0.5f, 0.0f);
				PSMTXConcat(mtx1, mtx3, mtx3);
				PSMTXConcat(mtx3, mtx2, work->entries[0].texMtx);
			}
			else if (work->entries[0].type == 2) {
				PSMTXTrans(work->entries[0].texMtx, 1.0f, 1.0f, 0.0f);
			}
			break;
		case 3:
		case 4:
		case 5:
		case 6:
			GXLoadPosMtxImm(camera->view, 0);
			GXSetCurrentMtx(0);
			TEXGetGXTexObjFromPalette(wp->texture, &obj, 1);
			GXLoadTexObj(&obj, GX_TEXMAP0);
			if (!(work->entries[0].flags & 2)) {
				p = work->entries[0].progress;
				scale = p * (p * (p * (100.0f * p * p)));
				PSMTXTrans(mtx1, 0.5f, 0.5f, 0.0f);
				PSMTXScale(mtx3, scale, scale, 1.0f);
				PSMTXRotRad(mtx4, 'z', 0.017453292f * work->entries[0].rotation);
				PSMTXTrans(mtx2, -0.5f - work->entries[0].fadeSpotX / gp->fbWidth,
					-0.5f + work->entries[0].fadeSpotY / gp->efbHeight, 0.0f);
				PSMTXConcat(mtx1, mtx3, mtx3);
				PSMTXConcat(mtx3, mtx4, mtx3);
				PSMTXConcat(mtx3, mtx2, work->entries[0].texMtx);
			}
			else if (work->entries[0].type == 4 || work->entries[0].type == 6) {
				PSMTXTrans(work->entries[0].texMtx, 1.0f, 1.0f, 0.0f);
			}
			break;
		case 9:
		case 10:
		case 11:
		case 12:
		case 14:
		case 15:
			GXLoadPosMtxImm(camera->view, 0);
			GXSetCurrentMtx(0);
			GXInitTexObj(&obj, i4tex, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
			GXLoadTexObj(&obj, GX_TEXMAP0);
			PSMTXIdentity(work->entries[0].texMtx);
			work->entries[0].color.a = 255.0f * work->entries[0].progress;
			break;
		case 13:
			GXLoadPosMtxImm(camera->view, 0);
			GXSetCurrentMtx(0);
			GXInitTexObj(&obj, i4tex, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
			GXLoadTexObj(&obj, GX_TEXMAP0);
			PSMTXIdentity(work->entries[0].texMtx);
			break;
	}
	GXLoadTexMtxImm(work->entries[0].texMtx, GX_TEXMTX0, GX_MTX2x4);
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
	GXSetTevColor(GX_TEVREG0, work->entries[0].color);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	size = 304.0f;
	GXPosition3f32(-size, size, 0.0f);
	GXTexCoord2f32(0.0f, 0.0f);
	GXPosition3f32(size, size, 0.0f);
	GXTexCoord2f32(1.0f, 0.0f);
	GXPosition3f32(size, -size, 0.0f);
	GXTexCoord2f32(1.0f, 1.0f);
	GXPosition3f32(-size, -size, 0.0f);
	GXTexCoord2f32(0.0f, 1.0f);
	GXSetProjectionv(proj);
}

#pragma dont_inline on
static f32 z(void) {
	FadeWork* work = wp;
	CameraEntry* camera;
	Vec pos;
	f32 base;
	f32 one = 1.0f;
	f32 pz;

	camera = camGetPtr(CAMERA_3D);
	if (camera->projType == GX_PERSPECTIVE) {
		return 1.0f;
	}
	camera = camGetPtr(CAMERA_FADE);
	PSMTXMultVec(camera->view, &work->entries[1].VirtualPos, &pos);
	pz = pos.z;
	base = camera->viewport[5] + (camera->viewport[5] - camera->viewport[4]) *
		((1.0f / -pz) * (camera->projection[2][2] * pz + camera->projection[2][3]));
	camera = camGetPtr(CAMERA_3D);
	PSMTXMultVec(camera->view, &work->entries[1].VirtualPos, &pos);
	return (camera->viewport[5] + (camera->viewport[5] - camera->viewport[4]) *
		(one * (camera->projection[2][2] * pos.z + camera->projection[2][3]))) / base;
}

#pragma dont_inline reset

static void zFill(void) {
	static s32 depth_image[32] __attribute__((aligned(32))) = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	CameraEntry* camera;
	f32 y0, y1, x0, x1;
	f32 zpos;
	GXTexObj obj;
	Mtx mtx;

	camera = camGetCurPtr();
	y0 = (camera->near + camera->far) / 2.0f;
	y0 = y0 * (f32)tan(0.5f * ((6.2831855f * camera->fovY) / 360.0f));
	x1 = y0 * camera->aspect;
	y1 = -y0;
	x0 = -x1;
	GXInitTexObj(&obj, depth_image, 4, 4, GX_TF_Z8, GX_REPEAT, GX_REPEAT, GX_FALSE);
	GXLoadTexObj(&obj, GX_TEXMAP0);
	GXSetNumTexGens(1);
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetZTexture(GX_ZT_REPLACE, GX_TF_Z8, 0);
	GXSetNumChans(0);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetZCompLoc(GX_TRUE);
	GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
	GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
	GXSetColorUpdate(GX_FALSE);
	GXSetAlphaUpdate(GX_FALSE);
	GXSetCullMode(GX_CULL_NONE);
	GXClearVtxDesc();
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 0);
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	PSMTXIdentity(mtx);
	GXLoadPosMtxImm(camera->view, 0);
	GXSetCurrentMtx(0);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	zpos = -10000.0f;
	GXPosition3f32(x0, y0, zpos);
	GXTexCoord2u8(0, 0);
	GXPosition3f32(x1, y0, zpos);
	GXTexCoord2u8(1, 0);
	GXPosition3f32(x1, y1, zpos);
	GXTexCoord2u8(1, 1);
	GXPosition3f32(x0, y1, zpos);
	GXTexCoord2u8(0, 1);
	GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);
	GXSetColorUpdate(GX_TRUE);
}

static inline f32 fadeMadd(f32 a, f32 b, f32 c) { return a * b + c; }
void disp_maku(CameraId camId, void* param) {
	FadeEntry* entry = param;
	CameraEntry* camera;
	f32 scale, depth, sx, mag = 3.141592f;
	Mtx mtx;
	Mtx mtx2;
	Vec dist;
	Vec dir;

	camera = camGetPtr(camId);
	switch (entry->type) {
		case 0x17:
		case 0x19:
		case 0x1C:
		case 0x40:
			if (entry->flags & 2) {
				return;
			}
	}
	switch (entry->type) {
		case 0x16: case 0x17: case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C:
		case 0x1D: case 0x1E: case 0x1F: case 0x20: case 0x21: case 0x22: case 0x23:
		case 0x24: case 0x25: case 0x26: case 0x27: case 0x28: case 0x33: case 0x3F: case 0x40:
			break;
		default:
			return;
	}
	if (entry->poseId < 0) {
		return;
	}
	if (entry->flags & 4) {
		camera = camGetPtr(CAMERA_3D);
		mag = 3.141592f; depth = camera->fovY; depth = (depth *= 0.5f) / 180.0f; depth = tan(mag * depth);
		depth = 24.0f / depth;
		PSVECSubtract(&camGetPtr(CAMERA_3D)->cameraPos, &entry->VirtualPos, &dist);
		camera = camGetPtr(CAMERA_3D);
		dir = *(Vec*)camera->view[2];
		PSVECNormalize(&dir, &dir);
		dist.x *= dir.x;
		dist.y *= dir.y;
		dist.z *= dir.z;
		mag = PSVECMag(&dist);
		scale = mag / depth;
		depth = depth - mag;
		animPoseSetGXFunc(entry->poseId, NULL, FALSE);
		GXSetZScaleOffset(z(), 0.0f);
	}
	else {
		scale = entry->scale;
		depth = 0.0f;
		zFill();
	}
	PSMTXTrans(mtx, 64.0f * (fadeMadd(entry->animOffsetX, scale, 0.0f) / gp->fbWidth),
		48.0f * (fadeMadd(entry->animOffsetY, scale, 0.0f) / gp->efbHeight), depth);
	PSMTXTrans(mtx2, -32.0f, 24.0f, 0.0f);
	PSMTXConcat(mtx, mtx2, mtx);
	sx = 608.0f / gp->fbWidth;
	PSMTXScale(mtx2, sx, 480.0f / gp->efbHeight, sx);
	PSMTXConcat(mtx, mtx2, mtx);
	PSMTXTrans(mtx2, 32.0f, -24.0f, 0.0f);
	PSMTXConcat(mtx, mtx2, mtx);
	animPoseDrawMtx(entry->poseId, mtx, 1, 0.0f, scale);
	animPoseDrawMtx(entry->poseId, mtx, 2, 0.0f, scale);
	animPoseDrawMtx(entry->poseId, mtx, 3, 0.0f, scale);
	sysWaitDrawSync();
}

void fadeStrippedPool(f32* f) {
	f[0] = 3.141592f;
}

static inline GXColor fadeTecAlpha(f32 a) {
	GXColor c = (GXColor){0, 0, 0, 0};
	c.a = 255.0f * a;
	return c;
}

void disp_tec(CameraId camId, void* param) {
	CameraEntry* camera;
	GXTexObj obj;

	camera = camGetPtr(camId);
	GXSetZCompLoc(GX_TRUE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, (GXLogicOp)7);
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXSetNumChans(0);
	GXSetNumTexGens(1);
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	if (wp->flags & 0x1000) {
		TEXGetGXTexObjFromPalette(wp->texture, &obj, 2);
		GXLoadTexObj(&obj, GX_TEXMAP0);
	}
	else {
		TEXGetGXTexObjFromPalette(wp->texture, &obj, 3);
		GXLoadTexObj(&obj, GX_TEXMAP0);
	}
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_ZERO);
	if (wp->flags & 0x1000) {
		GXSetTevColor(GX_TEVREG0, (GXColor){0x19, 0x23, 0x2A, 0xFF});
	}
	else {
		GXSetTevColor(GX_TEVREG0, (GXColor){0xE6, 0xE6, 0xE6, 0xFF});
	}
	if (wp->flags & 0x30) {
		if (wp->flags & 0x10) {
			wp->softness += 0.04f;
			if (wp->softness > 1.0f) {
				wp->softness = 1.0f;
				wp->flags &= ~0x10;
			}
		}
		else {
			wp->softness -= 0.04f;
			if (wp->softness < 0.0f) {
				wp->softness = 0.0f;
				wp->flags &= ~0x20;
				wp->flags &= ~0x3000;
			}
		}
		GXSetTevColor(GX_TEVREG1, fadeTecAlpha(wp->softness));
		GXSetNumTevStages(2);
		GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
		GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
		GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
		GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_A1, GX_CA_ZERO);
	}
	GXSetCullMode(GX_CULL_BACK);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXLoadPosMtxImm(camera->view, 0);
	GXSetCurrentMtx(0);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3s16(-308, 244, 0);
	GXTexCoord2f32(0.0f, 0.0f);
	GXPosition3s16(308, 244, 0);
	GXTexCoord2f32(2.0f, 0.0f);
	GXPosition3s16(308, -244, 0);
	GXTexCoord2f32(2.0f, 2.0f);
	GXPosition3s16(-308, -244, 0);
	GXTexCoord2f32(0.0f, 2.0f);
}

void disp_dummy_black(CameraId camId, void* param) {
	CameraEntry* camera;
	f32 proj[7];

	camera = camGetPtr(CAMERA_FADE);
	GXGetProjectionv(proj);
	GXSetProjection(camera->projection, camera->projType);
	GXSetZCompLoc(GX_FALSE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, (GXLogicOp)7);
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	if (wp->flags & 0x8000) {
		GXSetChanMatColor(GX_COLOR0A0, (GXColor){0x00, 0x00, 0x00, 0xFF});
	}
	else {
		GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
	}
	GXSetNumTevStages(1);
	GXSetNumTexGens(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXLoadPosMtxImm(camera->view, 0);
	GXSetCurrentMtx(0);
	GXSetCullMode(GX_CULL_BACK);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3s16(-32, 24, 0);
	GXPosition3s16(32, 24, 0);
	GXPosition3s16(32, -24, 0);
	GXPosition3s16(-32, -24, 0);
	GXSetProjectionv(proj);
}

BOOL fadeIsFinish(void) {
	if (wp->lastEntryId == -1) {
		return TRUE;
	}
	return (*(u16*)((u8*)wp + wp->lastEntryId * sizeof(FadeEntry) + offsetof(FadeWork, entries[0].flags)) >> 1) & 1; // FAKE: byte-offset load so the entries offset folds into the index (mulli/addi/lhzx), as in the target
}

void fadeReset(s32 id) { //TODO: better param name?
	FadeEntry* entry;

	entry = &wp->entries[id];
	if (entry->poseId >= 0) {
		animPoseRelease(entry->poseId);
	}
	if (entry->paperPoseId >= 0) {
		animPaperPoseRelease(entry->paperPoseId);
	}
	if (entry->image) {
		imgRelease(entry->image, entry->imageType);
	}
	memset(entry, 0, sizeof(FadeEntry));
	entry->poseId = -1;
	entry->paperPoseId = -1;
	entry->fadeSpotX = 0.0f;
	entry->fadeSpotY = 0.0f;
	entry->animOffsetX = 0.0f;
	entry->animOffsetY = 0.0f;
	entry->scale = 1.0f;
}

TEXPalette* fadeGetTpl(void) {
	return wp->texture;
}

void fadeSetNarrowFast(void) {
	wp->entries[1].flags |= 8;
}



#pragma once

#include "evt/evt_cmd.h"
#include "system.h" // EasingType for the movement curve arguments
#include <dolphin/types.h>

USER_FUNC(btlevtcmd_WaitEventEnd);
USER_FUNC(btlevtcmd_check_battleflag);
USER_FUNC(btlevtcmd_onoff_battleflag);
USER_FUNC(btlevtcmd_get_turn);
USER_FUNC(btlevtcmd_reset_turn);
USER_FUNC(btlevtcmd_reset_move_color_lv_all);
USER_FUNC(btlevtcmd_CheckPhase);
USER_FUNC(btlevtcmd_GetPos);
USER_FUNC(btlevtcmd_GetPosFloat);
USER_FUNC(btlevtcmd_GetRandomValue);
USER_FUNC(btlevtcmd_DrawLots);
USER_FUNC(btlevtcmd_SpawnUnit);
USER_FUNC(btlevtcmd_KillUnit);
USER_FUNC(btlevtcmd_ReplaceParts);
USER_FUNC(btlevtcmd_ChangeParty);
USER_FUNC(btlevtcmd_SetPos);
USER_FUNC(btlevtcmd_AddPos);
USER_FUNC(btlevtcmd_GetPartsPos);
USER_FUNC(btlevtcmd_SetPartsPos);
USER_FUNC(btlevtcmd_AddPartsPos);
USER_FUNC(btlevtcmd_SetDispOffset);
USER_FUNC(btlevtcmd_SetPartsDispOffset);
USER_FUNC(btlevtcmd_AddPartsDispOffset);
USER_FUNC(btlevtcmd_SetTogeOffset);
USER_FUNC(btlevtcmd_GetHomePos);
USER_FUNC(btlevtcmd_SetHomePos);
USER_FUNC(btlevtcmd_AddHomePos);
USER_FUNC(btlevtcmd_SetPartsHomePos);
USER_FUNC(btlevtcmd_GetHitPos);
USER_FUNC(btlevtcmd_SetHitOffset);
USER_FUNC(btlevtcmd_SetHitCursorOffset);
USER_FUNC(btlevtcmd_GetWidth);
USER_FUNC(btlevtcmd_GetHeight);
USER_FUNC(btlevtcmd_SetHeight);
USER_FUNC(btlevtcmd_GetStatusMg);
USER_FUNC(btlevtcmd_SetStatusIconOffset);
USER_FUNC(btlevtcmd_SetHpGaugeOffset);
USER_FUNC(btlevtcmd_GetHpGaugeOffset);
USER_FUNC(btlevtcmd_SetAlpha);
USER_FUNC(btlevtcmd_GetRGB);
USER_FUNC(btlevtcmd_SetRGB);
USER_FUNC(btlevtcmd_GetRotate);
USER_FUNC(btlevtcmd_SetRotate);
USER_FUNC(btlevtcmd_AddRotate);
USER_FUNC(btlevtcmd_GetPartsRotate);
USER_FUNC(btlevtcmd_SetPartsRotate);
USER_FUNC(btlevtcmd_AddPartsRotate);
USER_FUNC(btlevtcmd_SetBaseRotate);
USER_FUNC(btlevtcmd_SetPartsBaseRotate);
USER_FUNC(btlevtcmd_SetRotateOffset);
USER_FUNC(btlevtcmd_SetPartsRotateOffset);
USER_FUNC(btlevtcmd_AddPartsRotateOffset);
USER_FUNC(btlevtcmd_SetRotateOffsetFromCenterOffset);
USER_FUNC(btlevtcmd_SetCutBaseOffset);
USER_FUNC(btlevtcmd_SetBintaHitOffset); //Love Slap
USER_FUNC(btlevtcmd_SetKissHitOffset); //Lip Lock
USER_FUNC(btlevtcmd_SetCutWidth);
USER_FUNC(btlevtcmd_SetCutHeight);
USER_FUNC(btlevtcmd_SetPossessionItemOffset);
USER_FUNC(btlevtcmd_SetBaseScale);
USER_FUNC(btlevtcmd_SetPartsBaseScale);
USER_FUNC(btlevtcmd_GetScale);
USER_FUNC(btlevtcmd_SetScale);
USER_FUNC(btlevtcmd_AddScale);
USER_FUNC(btlevtcmd_SetPartsScale);
USER_FUNC(btlevtcmd_AddPartsScale);
USER_FUNC(btlevtcmd_GetHp);
USER_FUNC(btlevtcmd_GetFp);
USER_FUNC(btlevtcmd_GetMaxHp);
USER_FUNC(btlevtcmd_GetMaxFp);
USER_FUNC(btlevtcmd_SetHp);
USER_FUNC(btlevtcmd_SetFp);
USER_FUNC(btlevtcmd_SetMaxFp);
USER_FUNC(btlevtcmd_SetSwallowParam);
USER_FUNC(btlevtcmd_SetSwallowAttribute);
USER_FUNC(btlevtcmd_SetMaxMoveCount);
USER_FUNC(btlevtcmd_GetDamagePartsId);
USER_FUNC(btlevtcmd_GetHpDamage);
USER_FUNC(btlevtcmd_GetFpDamage);
USER_FUNC(btlevtcmd_GetHpDamageCount);
USER_FUNC(btlevtcmd_RecoverHp);
USER_FUNC(btlevtcmd_RecoverFp);
USER_FUNC(btlevtcmd_GetOverTurnCount);
USER_FUNC(btlevtcmd_SetOverTurnCount);
USER_FUNC(btlevtcmd_GetUnitWork);
USER_FUNC(btlevtcmd_SetUnitWork);
USER_FUNC(btlevtcmd_AddUnitWork);
USER_FUNC(btlevtcmd_GetUnitWorkFloat);
USER_FUNC(btlevtcmd_SetUnitWorkFloat);
USER_FUNC(btlevtcmd_GetPartsWork);
USER_FUNC(btlevtcmd_SetPartsWork);
USER_FUNC(btlevtcmd_GetDamage);
USER_FUNC(btlevtcmd_GetTotalDamage);
USER_FUNC(btlevtcmd_GetDamageCode);
USER_FUNC(btlevtcmd_CheckDamageCode);
USER_FUNC(btlevtcmd_StartAvoid);
USER_FUNC(btlevtcmd_RunHitEventDirect);
USER_FUNC(btlevtcmd_CheckDamagePattern);
USER_FUNC(btlevtcmd_CheckSpace);
USER_FUNC(btlevtcmd_GetEnemyBelong);
USER_FUNC(btlevtcmd_GetFriendBelong);
USER_FUNC(btlevtcmd_GetProtectId);
USER_FUNC(btlevtcmd_GetUnitKind);
USER_FUNC(btlevtcmd_GetPartnerId);
USER_FUNC(btlevtcmd_GetMarioId);
USER_FUNC(btlevtcmd_GetPartyId);
USER_FUNC(btlevtcmd_GetBodyId);
USER_FUNC(btlevtcmd_GetUnitId);
USER_FUNC(btlevtcmd_GetPartyTechLv);
USER_FUNC(btlevtcmd_CalculateFaceDirection);
USER_FUNC(btlevtcmd_GetFaceDirection);
USER_FUNC(btlevtcmd_ChangeFaceDirection);
USER_FUNC(btlevtcmd_ChangePartsFaceDirection);
USER_FUNC(btlevtcmd_ResetFaceDirection);
USER_FUNC(btlevtcmd_CheckAttribute);
USER_FUNC(btlevtcmd_OnAttribute);
USER_FUNC(btlevtcmd_OffAttribute);
USER_FUNC(btlevtcmd_CheckPartsAttribute);
USER_FUNC(btlevtcmd_OnPartsAttribute);
USER_FUNC(btlevtcmd_OffPartsAttribute);
USER_FUNC(btlevtcmd_CheckPartsCounterAttribute);
USER_FUNC(btlevtcmd_OnPartsCounterAttribute);
USER_FUNC(btlevtcmd_OffPartsCounterAttribute);
USER_FUNC(btlevtcmd_SetPartsBlur);
USER_FUNC(btlevtcmd_OnToken);
USER_FUNC(btlevtcmd_OffToken);
USER_FUNC(btlevtcmd_CheckToken);
USER_FUNC(btlevtcmd_SetRegistStatus);
USER_FUNC(btlevtcmd_CheckCommandUnit);





















USER_FUNC(btlevtcmd_ACRStart);
USER_FUNC(btlevtcmd_ACRGetResult);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_StartWaitEvent);

USER_FUNC(btlevtcmd_ACHelpSet);
USER_FUNC(btlevtcmd_ACSuccessEffect);
USER_FUNC(btlevtcmd_AcGetDifficulty);
USER_FUNC(btlevtcmd_AcGetOutputParam);
USER_FUNC(btlevtcmd_AcGetParam);
USER_FUNC(btlevtcmd_AcSetDifficulty);
USER_FUNC(btlevtcmd_AcSetFlag);
USER_FUNC(btlevtcmd_AcSetGaugeParam);
USER_FUNC(btlevtcmd_AcSetOutputParam);
USER_FUNC(btlevtcmd_AcSetParam);
USER_FUNC(btlevtcmd_AcSetParamAll);
USER_FUNC(btlevtcmd_AnimeChangeGroup);
USER_FUNC(btlevtcmd_AnimeChangePaperAnime);
USER_FUNC(btlevtcmd_AnimeChangePaperGroup);
USER_FUNC(btlevtcmd_AnimeChangePose);
USER_FUNC(btlevtcmd_AnimeChangePoseDirect);
USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_AnimeChangePoseType);
USER_FUNC(btlevtcmd_AnimeFlagOnOff);
USER_FUNC(btlevtcmd_AnimeSetMotionSpeed);
USER_FUNC(btlevtcmd_AnimeSetPoseSoundTable);
USER_FUNC(btlevtcmd_AnimeSetPoseTable);
USER_FUNC(btlevtcmd_AnimeWaitPlayComplete);
USER_FUNC(btlevtcmd_AttackDeclare);
USER_FUNC(btlevtcmd_AttackDeclareAll);
USER_FUNC(btlevtcmd_AudienceAppeal);
USER_FUNC(btlevtcmd_AudienceDeclareACResult);
USER_FUNC(btlevtcmd_AudienceDeclareAcrobatResult);
USER_FUNC(btlevtcmd_ChangeDataTable);
USER_FUNC(btlevtcmd_ChangeKind);
USER_FUNC(btlevtcmd_CheckActStatus);
USER_FUNC(btlevtcmd_CheckDamage);
USER_FUNC(btlevtcmd_CheckDataOfDataTable);
USER_FUNC(btlevtcmd_CheckStatus);
USER_FUNC(btlevtcmd_CheckWeaponPluralTarget);
USER_FUNC(btlevtcmd_ChoiceSamplingEnemy);
USER_FUNC(btlevtcmd_ClearAllStatus);
USER_FUNC(btlevtcmd_CommandCheckDamage);
USER_FUNC(btlevtcmd_CommandGetWeaponActionLv);
USER_FUNC(btlevtcmd_CommandGetWeaponAddress);
USER_FUNC(btlevtcmd_CommandPayWeaponCost);
USER_FUNC(btlevtcmd_CommandPreCheckCounter);
USER_FUNC(btlevtcmd_CommandPreCheckDamage);
USER_FUNC(btlevtcmd_ConsumeItemReserve);
USER_FUNC(btlevtcmd_DamageDirect);
USER_FUNC(btlevtcmd_DispItemIcon);
USER_FUNC(btlevtcmd_DivePartsPosition);
USER_FUNC(btlevtcmd_DivePosition);
USER_FUNC(btlevtcmd_EnemyItemUseCheck);
USER_FUNC(btlevtcmd_FaceDirectionAdd);
USER_FUNC(btlevtcmd_FaceDirectionSub);
USER_FUNC(btlevtcmd_FallPartsPosition);
USER_FUNC(btlevtcmd_FallPosition);
USER_FUNC(btlevtcmd_GetBackItem);
USER_FUNC(btlevtcmd_GetCoin);
USER_FUNC(btlevtcmd_GetDataFromDataTable);
USER_FUNC(btlevtcmd_GetExp);
USER_FUNC(btlevtcmd_GetFirstAttackTarget);
USER_FUNC(btlevtcmd_GetGuardKouraId);
USER_FUNC(btlevtcmd_GetMoveFrame);
USER_FUNC(btlevtcmd_GetResultAC);
USER_FUNC(btlevtcmd_GetResultACDefence);
USER_FUNC(btlevtcmd_GetResultCountAC);
USER_FUNC(btlevtcmd_GetResultPrizeLv);
USER_FUNC(btlevtcmd_GetSelectEnemy);
USER_FUNC(btlevtcmd_GetSelectNextEnemy);
USER_FUNC(btlevtcmd_GetStageAudienceAreaSize);
USER_FUNC(btlevtcmd_GetStageSize);
USER_FUNC(btlevtcmd_GetTakeoffPosition);
USER_FUNC(btlevtcmd_GetWeaponActionLv);
USER_FUNC(btlevtcmd_InviteApInfoReport);
USER_FUNC(btlevtcmd_JumpContinue);
USER_FUNC(btlevtcmd_JumpPartsContinue);
USER_FUNC(btlevtcmd_JumpPartsPosition);
USER_FUNC(btlevtcmd_JumpPartsSetting);
USER_FUNC(btlevtcmd_JumpPosition);
USER_FUNC(btlevtcmd_JumpSetting);
USER_FUNC(btlevtcmd_MarioJumpParam);
USER_FUNC(btlevtcmd_MarioJumpPosition);
USER_FUNC(btlevtcmd_MoveDirectionAdd);
USER_FUNC(btlevtcmd_MovePosition);
USER_FUNC(btlevtcmd_OffStatusFlag);
USER_FUNC(btlevtcmd_OffUnitFlag);
USER_FUNC(btlevtcmd_OnOffStatus);
USER_FUNC(btlevtcmd_OnStatusFlag);
USER_FUNC(btlevtcmd_OnUnitFlag);
USER_FUNC(btlevtcmd_PadCheckNow);
USER_FUNC(btlevtcmd_PadCheckTrigger);
USER_FUNC(btlevtcmd_PartsFaceDirectionSub);
USER_FUNC(btlevtcmd_PayWeaponCost);
USER_FUNC(btlevtcmd_PhaseEventStartDeclare);
USER_FUNC(btlevtcmd_PreCheckCounter);
USER_FUNC(btlevtcmd_PreCheckDamage);
USER_FUNC(btlevtcmd_ResultAC);
USER_FUNC(btlevtcmd_ResultACDefence);
USER_FUNC(btlevtcmd_RunDataEventChild);
USER_FUNC(btlevtcmd_SacBgsetMove);
USER_FUNC(btlevtcmd_SamplingEnemy);
USER_FUNC(btlevtcmd_SetDiveSound);
USER_FUNC(btlevtcmd_SetEventCeilFall);
USER_FUNC(btlevtcmd_SetFallAccel);
USER_FUNC(btlevtcmd_SetJumpSpeed);
USER_FUNC(btlevtcmd_SetMoveSoundDataWork);
USER_FUNC(btlevtcmd_SetMoveSpeed);
USER_FUNC(btlevtcmd_SetPartsDefenceAttrTable);
USER_FUNC(btlevtcmd_SetPartsDefenceTable);
USER_FUNC(btlevtcmd_SetPartsDiveSound);
USER_FUNC(btlevtcmd_SetPartsFallAccel);
USER_FUNC(btlevtcmd_SetPartsJumpSound);
USER_FUNC(btlevtcmd_SetPartsMoveSpeed);
USER_FUNC(btlevtcmd_SetPartsRunSound);
USER_FUNC(btlevtcmd_SetPartsWalkSound);
USER_FUNC(btlevtcmd_SetRunSound);
USER_FUNC(btlevtcmd_SetStayPose);
USER_FUNC(btlevtcmd_SetStayPoseType);
USER_FUNC(btlevtcmd_SetTalkPose);
USER_FUNC(btlevtcmd_SetTalkPoseType);
USER_FUNC(btlevtcmd_SetWalkSound);
USER_FUNC(btlevtcmd_SetupAC);
USER_FUNC(btlevtcmd_StageDispellFog);
USER_FUNC(btlevtcmd_StartAC);
USER_FUNC(btlevtcmd_StatusWindowOnOff);
USER_FUNC(btlevtcmd_StopAC);
USER_FUNC(btlevtcmd_StopWaitEvent);
USER_FUNC(btlevtcmd_StoreCoin);
USER_FUNC(btlevtcmd_StoreExp);
USER_FUNC(btlevtcmd_TransStageFloorPosition);
USER_FUNC(btlevtcmd_WaitAttackEnd);
USER_FUNC(btlevtcmd_WaitGuardMove);
USER_FUNC(btlevtcmd_WeaponAftereffect);
USER_FUNC(btlevtcmd_ac_timing_flag_onoff);
USER_FUNC(btlevtcmd_ac_timing_get_success_frame);
USER_FUNC(btlevtcmd_ftof);
USER_FUNC(btlevtcmd_ftomsec);
USER_FUNC(btlevtcmd_snd_se);
USER_FUNC(btlevtcmd_snd_se_offset);
USER_FUNC(btlevtcmd_snd_voice);

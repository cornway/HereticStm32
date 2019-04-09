//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 1993-2008 Raven Software
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//
// External definitions for action pointer functions.
//

#ifndef HERETIC_P_ACTION_H
#define HERETIC_P_ACTION_H

void A_FreeTargMobj (void);
void A_RestoreSpecialThing1 (void);
void A_RestoreSpecialThing2 (void);
void A_HideThing (void);
void A_UnHideThing (void);
void A_RestoreArtifact (void);
void A_Scream (void);
void A_Explode (void);
void A_PodPain (void);
void A_RemovePod (void);
void A_MakePod (void);
void A_InitKeyGizmo (void);
void A_VolcanoSet (void);
void A_VolcanoBlast (void);
void A_BeastPuff (void);
void A_VolcBallImpact (void);
void A_SpawnTeleGlitter (void);
void A_SpawnTeleGlitter2 (void);
void A_AccTeleGlitter (void);
void A_Light0 (void);
void A_WeaponReady (void);
void A_Lower (void);
void A_Raise (void);
void A_StaffAttackPL1 (void);
void A_ReFire (void);
void A_StaffAttackPL2 (void);
void A_BeakReady (void);
void A_BeakRaise (void);
void A_BeakAttackPL1 (void);
void A_BeakAttackPL2 (void);
void A_GauntletAttack (void);
void A_FireBlasterPL1 (void);
void A_FireBlasterPL2 (void);
void A_SpawnRippers (void);
void A_FireMacePL1 (void);
void A_FireMacePL2 (void);
void A_MacePL1Check (void);
void A_MaceBallImpact (void);
void A_MaceBallImpact2 (void);
void A_DeathBallImpact (void);
void A_FireSkullRodPL1 (void);
void A_FireSkullRodPL2 (void);
void A_SkullRodPL2Seek (void);
void A_AddPlayerRain (void);
void A_HideInCeiling (void);
void A_SkullRodStorm (void);
void A_RainImpact (void);
void A_FireGoldWandPL1 (void);
void A_FireGoldWandPL2 (void);
void A_FirePhoenixPL1 (void);
void A_InitPhoenixPL2 (void);
void A_FirePhoenixPL2 (void);
void A_ShutdownPhoenixPL2 (void);
void A_PhoenixPuff (void);
void A_RemovedPhoenixFunc (void);
void A_FlameEnd (void);
void A_FloatPuff (void);
void A_FireCrossbowPL1 (void);
void A_FireCrossbowPL2 (void);
void A_BoltSpark (void);
void A_Pain (void);
void A_NoBlocking (void);
void A_AddPlayerCorpse (void);
void A_SkullPop (void);
void A_FlameSnd (void);
void A_CheckBurnGone (void);
void A_CheckSkullFloor (void);
void A_CheckSkullDone (void);
void A_Feathers (void);
void A_ChicLook (void);
void A_ChicChase (void);
void A_ChicPain (void);
void A_FaceTarget (void);
void A_ChicAttack (void);
void A_Look (void);
void A_Chase (void);
void A_MummyAttack (void);
void A_MummyAttack2 (void);
void A_MummySoul (void);
void A_ContMobjSound (void);
void A_MummyFX1Seek (void);
void A_BeastAttack (void);
void A_SnakeAttack (void);
void A_SnakeAttack2 (void);
void A_HeadAttack (void);
void A_BossDeath (void);
void A_HeadIceImpact (void);
void A_HeadFireGrow (void);
void A_WhirlwindSeek (void);
void A_ClinkAttack (void);
void A_WizAtk1 (void);
void A_WizAtk2 (void);
void A_WizAtk3 (void);
void A_GhostOff (void);
void A_ImpMeAttack (void);
void A_ImpMsAttack (void);
void A_ImpMsAttack2 (void);
void A_ImpDeath (void);
void A_ImpXDeath1 (void);
void A_ImpXDeath2 (void);
void A_ImpExplode (void);
void A_KnightAttack (void);
void A_DripBlood (void);
void A_Sor1Chase (void);
void A_Sor1Pain (void);
void A_Srcr1Attack (void);
void A_SorZap (void);
void A_SorcererRise (void);
void A_SorRise (void);
void A_SorSightSnd (void);
void A_Srcr2Decide (void);
void A_Srcr2Attack (void);
void A_Sor2DthInit (void);
void A_SorDSph (void);
void A_Sor2DthLoop (void);
void A_SorDExp (void);
void A_SorDBon (void);
void A_BlueSpark (void);
void A_GenWizard (void);
void A_MinotaurAtk1 (void);
void A_MinotaurDecide (void);
void A_MinotaurAtk2 (void);
void A_MinotaurAtk3 (void);
void A_MinotaurCharge (void);
void A_MntrFloorFire (void);
void A_ESound (void);

#endif /* #ifndef HERETIC_P_ACTION_H */


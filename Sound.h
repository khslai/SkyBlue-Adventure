//=============================================================================
//
// �T�E���h�w�b�_�[ [Sound.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �T�E���h�ʂ��i���o�[�ASound_No
enum
{	
	BGM_Battle,
	BGM_Boss_Loop,
	BGM_Boss_Start,
	BGM_BossPratice,
	BGM_GameOver,
	BGM_Result,
	BGM_Title,
	BGM_Tutorial,
	SE_Be_Shot,
	SE_BombExplosion,
	SE_BombBulletExplosion,
	SE_BombExtend,
	SE_BossDead,
	SE_BossHPZero,
	SE_BossShot_01,
	SE_BossShot_02,
	SE_BossShot_03,
	SE_BulletFreeze,
	SE_BulletShot_01,
	SE_BulletShot_02,
	SE_BulletShot_03,
	SE_BulletShot_04,
	SE_CalculateScore,
	SE_Determine_No,
	SE_Determine_Yes,
	SE_EnemyDead,
	SE_GameClear_01,
	SE_GameClear_02,
	SE_GameStart_01,
	SE_GameStart_02,
	SE_GetItem,
	SE_HPAlarm,
	SE_LockOn,
	SE_Menu,
	SE_PlayerBeShot,
	SE_PlayerDead,
	SE_PlayerShot,
	SE_SelectMove,
	SE_ShotBomb,
	SE_ShotBombBullet,
	SE_ShowScore,
	SE_StarExtend,
	SE_TutorialEnter,
	Sound_Max,
};

// �Đ��p�t���O
enum
{	
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT	InitSound(HWND hWnd);
void ReInitSound(void);
void UninitSound(void);		
void PlaySound(int Sound_No, int Flag, bool Restart);
void StopSound(int Sound_No);
bool IsPlaying(int Sound_No);
void SoundFadeOut(int Sound_No);

#endif
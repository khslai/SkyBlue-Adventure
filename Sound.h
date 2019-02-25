//=============================================================================
//
// サウンドヘッダー [Sound.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// サウンド番号
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

// 再生用フラグ
enum
{	
	E_DS8_FLAG_NONE,	// 一回しかプレイしない
	E_DS8_FLAG_LOOP,	// ループ再生
	E_DS8_FLAG_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT	InitSound(HWND hWnd);
// 再初期化処理
void ReInitSound(void);
// 終了処理
void UninitSound(void);
// サウンドを設置する
void PlaySound(int Sound_No, int Flag, bool Restart);
// サウンドを停止する
void StopSound(int Sound_No);
// 再生中かどうか調べる
bool IsPlaying(int Sound_No);
// サウンドのフェイドアウト
void SoundFadeOut(int Sound_No);

#endif
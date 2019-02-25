//=============================================================================
//
// プレイヤー弾幕ヘッダー [PlayerBullet.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _PLAYERBULLET_H_
#define _PLAYERBULLET_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_PlayerBullet_A		_T("data/Texture/Player/PlayerBullet_A.png")
#define Texture_PlayerBullet_B		_T("data/Texture/Player/PlayerBullet_B.png")
#define Texture_OptionBullet_A		_T("data/Texture/Player/OptionBullet_A.png")
#define Texture_OptionBullet_B		_T("data/Texture/Player/OptionBullet_B.png")


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitPlayerBullet(int InitState);
// 終了処理
void UninitPlayerBullet(void);
// 更新処理
void UpdatePlayerBullet(void);
// 描画処理
void DrawPlayerBullet(void);
// プレイヤーの弾発射
void PlayerShot(void);

#endif

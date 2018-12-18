//=============================================================================
//
// ボムヘッダー [Bomb.h]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#ifndef _BOMB_H_
#define _BOMB_H_

#include "Barrage.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_ShotBomb				_T("data/Texture/Player/ShotBomb.png")
#define Texture_ShotBomb_Width_Total	(480)
#define Texture_ShotBomb_Height_Total	(96)
#define Texture_ShotBomb_Divide_X		(5)		// テクスチャ内分割数（横)
#define Texture_ShotBomb_Divide_Y		(1)		// テクスチャ内分割数（縦)
#define Texture_ShotBomb_Width			(Texture_ShotBomb_Width_Total / Texture_ShotBomb_Divide_X)
#define Texture_ShotBomb_Height			(Texture_ShotBomb_Height_Total / Texture_ShotBomb_Divide_Y)
#define Texture_BombExtend				_T("data/Texture/Player/BombExtend.png")
#define Texture_BombExtend_Width		(64)
#define Texture_BombExtend_Height		(64)
#define Texture_WhiteScreen				_T("data/Texture/Player/WhiteScreen.png")
#define Texture_WhiteScreen_Width		(1024)
#define Texture_WhiteScreen_Height		(768)
#define Texture_LockOn					_T("data/Texture/Player/LockOn.png")
#define Texture_LockOn_Width			(64)
#define Texture_LockOn_Height			(64)
#define Texture_BombBullet				_T("data/Texture/Player/BombBullet.png")
#define Texture_BombBullet_Width		(32)
#define Texture_BombBullet_Height		(32)

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;							// 座標
	float			Radius;							// 画像中心から頂点までの距離
	float			BaseAngle;						// 画像中心から頂点までの角度
	int				Target_No;						// ロックオンされたエネミーの番号
	bool			Use;							// 使用フラグ
}LOCKON;

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;							// 座標
	D3DXVECTOR3		ShakePos;						// 画面揺れる効果用座標
	D3DXVECTOR3		Rot;							// テクスチャの回転角度
	LOCKON			LockOn[Enemy_Max];				// 照準
	BULLET			Bullet[Enemy_Max];				// ボムの弾
	float			Radius;							// 画像中心から頂点までの距離
	float			HitRadius;						// 当たり判定の円の半径
	float			BaseAngle;						// 画像中心から頂点までの角度
	float			ExplosionRadius[BombBullet_Max];// 爆発半径
	int				BulletTarget[BombBullet_Max];	// 弾のターゲット
	int				LockOnNum;						// ロックオンされたエネミーの数
	int				AnimPattern;					// プレイヤーのアニメパターン
	int				Count;							// カウント
	int				Alpha;							// 透明合成用アルファ値
	int				Phase;							// ボムを広がる状態
	bool			Use;							// 使用フラグ
}BOMB;

// ボムの弾の状態
enum BombBulletState
{
	Homing,
	Explosion,
	Over,
	BulletDisspappear,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBomb(int InitState);
void UninitBomb(void);
void UpdateBomb(void);
void DrawBomb(void);
void SetBomb_A(void);
void SetBomb_B(void);
BOMB *GetBomb(void);

#endif

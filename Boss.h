//=============================================================================
//
// ボスヘッダー [Boss.h]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "Barrage.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Boss				_T("data/Texture/Boss/Boss.png")
#define Texture_Boss_Width_Total	(320)
#define Texture_Boss_Height_Total	(480)
#define Texture_Boss_Divide_X		(4)		// テクスチャ内分割数（横)
#define Texture_Boss_Divide_Y		(6)		// テクスチャ内分割数（縦)
#define Texture_Boss_Width			(Texture_Boss_Width_Total / Texture_Boss_Divide_X)
#define Texture_Boss_Height			(Texture_Boss_Height_Total / Texture_Boss_Divide_Y)
#define Texture_BossLife			_T("data/Texture/Boss/BossLife.png")
#define Texture_BossLife_Width		(32)
#define Texture_BossLife_Height		(32)


typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;					// 座標
	D3DXVECTOR3		BeforeMovePos;			// 移動始まる前の座標
	D3DXVECTOR3		Velocity;				// 初速度
	D3DXVECTOR3		Acceleration;			// 移動の加速度
	D3DXVECTOR3		Destination;			// 変位の距離
	D3DXVECTOR3		Rot;					// テクスチャの回転角度
	BULLET			Bullet[BossBullet_Max];	// ボスの弾
	float			Radius;					// 画像中心から頂点までの距離
	float			HitRadius;				// 当たり判定の円の半径
	float			BaseAngle;				// 画像中心から頂点までの角度
	float			MoveAngle;				// 移動方向を変更する角度
	float			ShotAngle;				// 弾発射のベース角度
	float			MoveTime;				// 目的座標までにかかる移動時間
	float			HP;						// 体力
	float			HP_Max;					// 最大体力
	int				AnimPattern;			// アニメパターン
	int				Count;					// 存在時間を計算するカウント
	int				ShotCount;				// 発射時間を計算するカウント
	int				MoveCount;				// 移動時間を計算するカウント
	int				MovePattern;			// 移動パターン
	int				Color;					// 色
	int				Life;					// 残機数
	int				BulletType;				// 弾の種類
	int				State;					// 状態
	int				Phase;					// 今は何段階
	bool			Exist;					// 存在かどうかのフラグ
	bool			MiddleBossOver;			// 中ボス終了かのフラグ
	bool			InMove;					// 移動してるかのフラグ
	bool			DebutMove;				// 初登場の移動かのフラグ
	bool			BombDamage;				// ボムダメージ受けたフラグ
}BOSS;

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	bool			Use;					// 使用かどうかのフラグ
}BOSSLIFE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBoss(int InitState);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
BOSS *GetBoss(void);
void SetBoss(void);
void SetBossColor(int Color);
void BossMove(D3DXVECTOR3 DestinationPos, float MoveTime);

#endif

//=============================================================================
//
// エネミーヘッダー [Enemy.h]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "File.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Enemy00					_T("data/Texture/Enemy/Enemy00.png")
#define Texture_Enemy00_Width_Total		(240)
#define Texture_Enemy00_Height_Total	(256)
#define Texture_Enemy00_Divide_X		(3)
#define Texture_Enemy00_Divide_Y		(4)
#define Texture_Enemy00_Width			(Texture_Enemy00_Width_Total / Texture_Enemy00_Divide_X)
#define Texture_Enemy00_Height			(Texture_Enemy00_Height_Total / Texture_Enemy00_Divide_Y)
#define Texture_Enemy01					_T("data/Texture/Enemy/Enemy01.png")
#define Texture_Enemy01_Width_Total		(240)
#define Texture_Enemy01_Height_Total	(256)
#define Texture_Enemy01_Divide_X		(3)
#define Texture_Enemy01_Divide_Y		(4)
#define Texture_Enemy01_Width			(Texture_Enemy01_Width_Total / Texture_Enemy01_Divide_X)
#define Texture_Enemy01_Height			(Texture_Enemy01_Height_Total / Texture_Enemy01_Divide_Y)
#define Texture_Enemy02					_T("data/Texture/Enemy/Enemy02.png")
#define Texture_Enemy02_Width_Total		(150)
#define Texture_Enemy02_Height_Total	(168)
#define Texture_Enemy02_Divide_X		(3)
#define Texture_Enemy02_Divide_Y		(4)
#define Texture_Enemy02_Width			(Texture_Enemy02_Width_Total / Texture_Enemy02_Divide_X)
#define Texture_Enemy02_Height			(Texture_Enemy02_Height_Total / Texture_Enemy02_Divide_Y)
#define Texture_Enemy03					_T("data/Texture/Enemy/Enemy03.png")
#define Texture_Enemy03_Width_Total		(135)
#define Texture_Enemy03_Height_Total	(200)
#define Texture_Enemy03_Divide_X		(3)
#define Texture_Enemy03_Divide_Y		(4)
#define Texture_Enemy03_Width			(Texture_Enemy03_Width_Total / Texture_Enemy03_Divide_X)
#define Texture_Enemy03_Height			(Texture_Enemy03_Height_Total / Texture_Enemy03_Divide_Y)
#define Texture_Enemy04					_T("data/Texture/Enemy/Enemy04.png")
#define Texture_Enemy04_Width_Total		(150)
#define Texture_Enemy04_Height_Total	(256)
#define Texture_Enemy04_Divide_X		(3)
#define Texture_Enemy04_Divide_Y		(4)
#define Texture_Enemy04_Width			(Texture_Enemy04_Width_Total / Texture_Enemy04_Divide_X)
#define Texture_Enemy04_Height			(Texture_Enemy04_Height_Total / Texture_Enemy04_Divide_Y)

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;					// エネミーの座標
	D3DXVECTOR3		Rot;					// テクスチャの回転角度
	float			Speed;					// 移動スピード
	float			BulletSpeed;			// 弾のスピード
	float			Radius;					// 画像中心から頂点までの距離
	float			HitRadius;				// 当たり判定の円の半径
	float			BaseAngle;				// 画像中心から頂点までの角度
	float			MoveAngle;				// 移動方向を変更する角度
	int				Direction;				// エネミーの向き
	int				Width;					// エネミーの横幅
	int				Height;					// エネミーの縦幅
	int				AnimPattern;			// エネミーのアニメパターン
	int				Count;					// 存在時間を計算するカウント
	int				MovePattern;			// 移動パターン
	int				EnemyType;				// エネミーの種類
	int				ShotTime;				// 弾幕開始時間
	int				BarrageType;			// 弾幕の種類
	float			HP;						// 体力
	int				BulletType;				// 弾の種類
	int				State;					// 状態
	int				WaitCount;				// 停滞時間
	int				ExitCount;				// 退場時間
	int				Item[EnemyItem_Max];	// アイテム
	bool			Exist;					// 存在かどうかのフラグ
}ENEMY;

// エネミーの方向
enum EnemyDirection
{
	Down,
	Left,
	Right,
	Up,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(int InitState);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(int Enemy_No);
void SetEnemy(int Status_No);
void SetEnemyColor(int Enemy_No, int Color);

#endif

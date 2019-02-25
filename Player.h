//=============================================================================
//
// プレイヤーヘッダー [Player.h]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Barrage.h"
#include "Bomb.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_PlayerDebut_A			_T("data/Texture/Player/PlayerDebut_A.png")
#define Texture_PlayerDebut_B			_T("data/Texture/Player/PlayerDebut_B.png")
#define Texture_PlayerDebut_Width_Total	(480)
#define Texture_PlayerDebut_Divide_X	(5)
#define Texture_PlayerDebut_Width		(Texture_PlayerDebut_Width_Total / Texture_PlayerDebut_Divide_X)
#define Texture_PlayerDebut_Height		(96)
#define Texture_Player_A				_T("data/Texture/Player/Player_A.png")
#define Texture_Player_B				_T("data/Texture/Player/Player_B.png")
#define Texture_Player_Width_Total		(288)
#define Texture_Player_Height_Total		(288)
#define Texture_Player_Divide_X			(3)		// プレイヤーのテクスチャ内分割数（横)
#define Texture_Player_Divide_Y			(3)		// プレイヤーのテクスチャ内分割数（縦)
#define Texture_Player_Width			(Texture_Player_Width_Total / Texture_Player_Divide_X)
#define Texture_Player_Height			(Texture_Player_Height_Total / Texture_Player_Divide_Y)
#define Texture_PlayerOption			_T("data/Texture/Player/PlayerOption.png")
#define Texture_PlayerOption_Width		(32)
#define Texture_PlayerOption_Height		(32)
#define Texture_HitPoint				_T("data/Texture/Player/HitPoint.png")
#define Texture_HitPoint_Width			(19)
#define Texture_HitPoint_Height			(19)


// オプション構造体
typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];		// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;						// プレイヤーの座標
	D3DXVECTOR3		Rot;						// テクスチャの回転角度
	float			Radius;						// 画像中心から頂点までの距離
	float			BaseAngle;					// 画像中心から頂点までの角度
	int				BulletType;					// 弾のタイプ
	bool			Use;						// 使用フラグ
}OPTION;

// 当たりポイント構造体
typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];		// 頂点情報格納ワーク
	D3DXVECTOR3		Rot;						// テクスチャの回転角度
	float			Radius;						// 画像中心から頂点までの距離
	float			BaseAngle;					// 画像中心から頂点までの角度
}HITPOINT;

// プレイヤー構造体
typedef	struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];		// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;						// プレイヤーの座標
	D3DXVECTOR3		Rot;						// テクスチャの回転角度
	BULLET			Bullet[PlayerBullet_Max];	// プレイヤーのバレット
	OPTION			Option[PlayerOption_Max];	// プレイヤーのオプション
	HITPOINT		HitPoint;					// 当たりポイント
	float			Radius;						// 画像中心から頂点までの距離
	float			HitRadius;					// 当たり判定の円の半径
	float			BaseAngle;					// 画像中心から頂点までの角度
	float			Power;						// プレイヤーのパワー
	float			HP;							// 体力
	int				Type;						// 自機のタイプ
	int				BombNum;					// 持つボムの数
	int				AnimCount;					// プレイヤーのアニメカウント
	int				AnimPattern;				// プレイヤーのアニメパターン
	int				Direction;					// プレイヤーの向き
	int				ShotCount;					// プレイヤーのショットカウント
	int				BulletType;					// 弾のタイプ
	int				InvincibleCount;			// 撃たれた後の無敵カウント
	int				Score;						// スコア
	bool			Exist;						// 存在フラグ
	bool			LowSpeedMode;				// 低速モードのフラグ
	bool			InBomb;						// ボム発射中
	bool			ClearStandBy;				// ゲームクリア時の準備フラグ
}PLAYER;

// プレイヤーのタイプ
enum
{
	Player_A,
	Player_B,
};

// オプションのタイプ
enum
{
	Option_A,
	Option_B,
};

// プレイヤーの方向
enum
{
	Forward,
	Flying,
	Backward,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(int InitState);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(void);
PLAYER *GetPlayer(int Player_No);

#endif

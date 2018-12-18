//=============================================================================
//
// エネミー弾幕ヘッダー [Barrage.h]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#ifndef _BARRAGE_H_
#define _BARRAGE_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Bullet_00		_T("data/Texture/Bullet/Bullet_00.png")
#define Texture_Bullet_01		_T("data/Texture/Bullet/Bullet_01.png")
#define Texture_Bullet_02		_T("data/Texture/Bullet/Bullet_02.png")
#define Texture_Bullet_03		_T("data/Texture/Bullet/Bullet_03.png")
#define Texture_Bullet_04		_T("data/Texture/Bullet/Bullet_04.png")
#define Texture_Bullet_05		_T("data/Texture/Bullet/Bullet_05.png")

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;					// 弾の座標
	D3DXVECTOR3		PreviousPos;			// 1フレイム前の弾の座標
	D3DXVECTOR3		Rot;					// テクスチャの回転角度
	float			Radius;					// 画像中心から頂点までの距離
	float			HitRadius;				// 当たり判定の円の半径
	float			BaseAngle;				// 画像中心から頂点までの角度
	float			MoveAngle;				// 移動方向を変更する角度
	float			Speed;					// 弾のスピード
	float			Damage;					// ダメージ
	int				Type;					// 弾のタイプ
	int				Count;					// カウント
	int				Color;					// 色
	int				State;					// 弾の状態
	bool			Use;					// 発射されたかのフラグ
	bool			DelayDisappear;			// 消失が遅れるかのフラグ
}BULLET;

typedef struct
{
	BULLET			Bullet[EnemyBullet_Max];// 弾幕のバレット
	float			BaseAngle;				// 弾幕の基本角度
	int				Phase;					// 弾幕の段階
	int				Type;					// 弾幕の種類
	int				Count;					// カウント
	int				Attacker_No;			// どのエネミーからの弾幕
}BARRAGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBarrage(int InitState);
void UninitBarrage(void);
void UpdateBarrage(void);
void DrawBarrage(void);
void SetBarrage(int Enemy_No);
BARRAGE *GetBarrage(int Barrage_No);
void PlayerShot(void);

#endif

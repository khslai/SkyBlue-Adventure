//=============================================================================
//
// エフェクトヘッダー [Effect.h]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_DeadEffect				_T("data/Texture/Enemy/DeadEffect.png")
#define Texture_DeadEffect_Width_Total	(700)
#define Texture_DeadEffect_Height_Total	(140)
#define Texture_DeadEffect_Divide_X		(5)
#define Texture_DeadEffect_Divide_Y		(1)
#define Texture_DeadEffect_Width		(Texture_DeadEffect_Width_Total / Texture_DeadEffect_Divide_X)
#define Texture_DeadEffect_Height		(Texture_DeadEffect_Height_Total / Texture_DeadEffect_Divide_Y)
#define Texture_BulletDead				_T("data/Texture/Bullet/BulletDead.png")
#define Texture_BulletDead_Width_Total	(600)
#define Texture_BulletDead_Height_Total	(60)
#define Texture_BulletDead_Divide_X		(10)
#define Texture_BulletDead_Divide_Y		(1)	
#define Texture_BulletDead_Width		(Texture_BulletDead_Width_Total / Texture_BulletDead_Divide_X)
#define Texture_BulletDead_Height		(Texture_BulletDead_Height_Total / Texture_BulletDead_Divide_Y)
#define Texture_BossBG_Circle			_T("data/Texture/Boss/BossBG_Circle.png")
#define Texture_BossBG_Circle_Width		(300)
#define Texture_BossBG_Circle_Height	(300)
#define Texture_BossBG_Star				_T("data/Texture/Boss/BossBG_Star.png")
#define Texture_BossBG_Star_Width		(200)
#define Texture_BossBG_Star_Height		(200)

typedef struct 
{
	VERTEX_2D		VertexWk[NUM_VERTEX];		// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;						// 座標
	D3DXVECTOR3		Rot;						// テクスチャの回転角度
	float			Radius;						// 画像中心から頂点までの距離
	float			BaseAngle;					// 画像中心から頂点までの角度
	int				Count;						// カウント
	int				AnimPattern;				// 色
	int				Alpha;						// 加算合成、透明合成用アルファ値
	int				TextureType;				// エフェクトテクスチャの種類
	bool			Use;						// 使用フラグ
}EFFECT;

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];		// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;						// 座標
	int				Count;						// カウント
	int				AnimPattern;				// テクスチャの何番分割
	int				Type;						// エフェクトの種類
	bool			Use;						// 使用フラグ
}EFFECTCOUNTER;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(int InitState);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEnemyDeadEffect(int Enemy_No);
void SetBossDeadEffect(void);
void SetBossEffect(void);
void SetBulletDeadEffect(D3DXVECTOR3 Pos);

#endif

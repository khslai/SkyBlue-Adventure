//=============================================================================
//
// エフェクトヘッダー [Effect.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
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

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// エフェクト構造体
typedef struct 
{
	VERTEX_2D		VertexWk[Num_Vertex];		// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;						// 座標
	D3DXVECTOR3		Rot;						// テクスチャの回転角度
	float			Radius;						// 画像中心から頂点までの距離
	float			BaseAngle;					// 画像中心から頂点までの角度
	int				Count;						// カウント
	int				AnimPattern;				// テクスチャの何番分割
	int				Alpha;						// 加算合成、透明合成用アルファ値
	int				TextureType;				// エフェクトテクスチャの種類
	bool			Use;						// 使用フラグ
}EFFECT;

// エフェクトコントローラー構造体
typedef struct
{
	VERTEX_2D		VertexWk[Num_Vertex];		// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;						// 座標
	int				Count;						// カウント
	int				AnimPattern;				// テクスチャの何番分割
	int				Type;						// エフェクトの種類
	bool			Use;						// 使用フラグ
}EFFECTCOUNTER;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitEffect(int InitState);
// 終了処理
void UninitEffect(void);
// 更新処理
void UpdateEffect(void);
// 描画処理
void DrawEffect(void);
// エネミー死亡エフェクトの設置
void SetEnemyDeadEffect(int Enemy_No);
// ボス死亡エフェクトの設置
void SetBossDeadEffect(void);
// ボス背景エフェクトの設置
void SetBossEffect(void);
// ボムによって、エネミーの弾が消えるエフェクトの設置
void SetBulletDeadEffect(D3DXVECTOR3 Pos);

#endif

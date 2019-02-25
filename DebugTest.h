//=============================================================================
//
// デバッグテストヘッダー [DebugTest.h]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#ifndef _DEBUGTEST_H_
#define _DEBUGTEST_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_HitCircle			_T("data/Texture/Other/HitCircle.png")
#define Texture_HitCircle_Width		(50)
#define Texture_HitCircle_Height	(50)

typedef struct 
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;					// 座標
	D3DXVECTOR3		Rot;					// テクスチャの回転角度
	float			Radius;					// 画像中心から頂点までの距離
	float			BaseAngle;				// 画像中心から頂点までの角度
	int				Owner;					// このHitCircleの所有者
	int				EnemyNo;				// このHitCircle対応するエネミー番号
	int				BulletNo;				// このHitCircle対応するバレット番号
	int				BarrageNo;				// このHitCircle対応する弾幕番号
	bool			Use;					// 使用フラグ
}HITCIRCLE;

// HitCircleの種類
enum HitCircleType
{
	PlayerHC,
	PlayerBulletHC,
	BombHC,
	EnemyHC,
	EnemyBulletHC,
	BossHC,
	BossBulletHC,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDebugTest(int InitState);
void UninitDebugTest(void);
void UpdateDebugTest(void);
void DrawDebugTest(void);
void SetPlayerHC(D3DVECTOR Pos, float HitRadius, int Owner);
void SetPlayerBulletHC(D3DVECTOR Pos, float HitRadius, int Bullet_No, int Owner);
void SetBombHC(D3DVECTOR Pos, float HitRadius, int Owner);
void SetEnemyHC(D3DVECTOR Pos, float HitRadius, int Enemy_No, int Owner);
void SetEnemyBulletHC(D3DVECTOR Pos, float HitRadius, int Barrage_No, int Bullet_No, int Owner);
void SetBossHC(D3DVECTOR Pos, float HitRadius, int Owner);
void SetBossBulletHC(D3DVECTOR Pos, float HitRadius, int Bullet_No, int Owner);


#endif

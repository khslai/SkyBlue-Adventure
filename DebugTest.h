//=============================================================================
//
// デバッグテストヘッダー [DebugTest.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
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
// 構造体定義
//*****************************************************************************
// 当たり範囲構造体
typedef struct 
{
	VERTEX_2D		VertexWk[Num_Vertex];	// 頂点情報格納ワーク
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


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitDebugTest(int InitState);
// 終了処理
void UninitDebugTest(void);
// 更新処理
void UpdateDebugTest(void);
// 描画処理
void DrawDebugTest(void);
// プレイヤーの当たり範囲の設置
void SetPlayerHC(D3DVECTOR Pos, float HitRadius, int Owner);
// プレイヤー弾の当たり範囲の設置
void SetPlayerBulletHC(D3DVECTOR Pos, float HitRadius, int Bullet_No, int Owner);
// ボムの当たり範囲の設置
void SetBombHC(D3DVECTOR Pos, float HitRadius, int Owner);
// エネミーの当たり範囲の設置
void SetEnemyHC(D3DVECTOR Pos, float HitRadius, int Enemy_No, int Owner);
// エネミー弾の当たり範囲の設置
void SetEnemyBulletHC(D3DVECTOR Pos, float HitRadius, int Barrage_No, int Bullet_No, int Owner);
// ボスの当たり範囲の設置
void SetBossHC(D3DVECTOR Pos, float HitRadius, int Owner);
// ボス弾の当たり範囲の設置
void SetBossBulletHC(D3DVECTOR Pos, float HitRadius, int Bullet_No, int Owner);

#endif

//=============================================================================
//
// エフェクト処理 [Effect.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Effect.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "BackGround.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// エフェクトテクスチャの最大数
#define EffectTexture_Max				(5)	

// エフェクトの種類
enum
{
	EnemyDead,			// エネミー死亡
	BossDead,			// ボス死亡
	BulletDisappear,	// ボムによって、弾が消える
	BossBackGround,		// ボスの背景エフェクト
};

// エフェクトテクスチャの種類
enum
{
	DeadEffect,	
	BossDeadEffect,
	BulletDead,
	BossBG_Circle,
	BossBG_Star,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeEffectVertex(int Effect_No);
// 頂点座標の設定
void SetEffectVertex(int Effect_No);
// テクスチャ座標の設定
void SetEffectTexture(int Effect_No);
// 色、透明度の設定
void SetEffectDiffuse(int Effect_No, int Alpha);
// エフェクトを設置する
void SetEffect(int EC_No, int EffectType);
// エフェクトの計算
void CalculateEffect(int Effect_No, int EffectType);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 EffectTexture[EffectTexture_Max] = { NULL };
// エフェクトコントローラー構造体
EFFECTCOUNTER EffectCounter[EffectCounter_Max];
// エフェクト構造体
EFFECT Effect[Effect_Max];
// テクスチャ半径
static float BaseAngle[EffectTexture_Max] = { 0.0f };
// 中心点と四頂点の成す角
static float Radius[EffectTexture_Max] = { 0.0f };

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(int InitState)
{
	D3DXVECTOR2 temp = D3DXVECTOR2(0.0f, 0.0f);
	int Effect_No = 0;
	// EC = EffectCounter
	int EC_No = 0;

	for (Effect_No = 0; Effect_No < Effect_Max; Effect_No++)
	{
		Effect[Effect_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Effect[Effect_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Effect[Effect_No].Radius = 0.0f;
		Effect[Effect_No].BaseAngle = 0.0f;
		Effect[Effect_No].Count = 0;
		Effect[Effect_No].AnimPattern = 0;
		Effect[Effect_No].Alpha = 0;
		Effect[Effect_No].TextureType = 0;
		Effect[Effect_No].Use = false;

		// 頂点情報の作成
		MakeEffectVertex(Effect_No);
	}

	for (EC_No = 0; EC_No < EffectCounter_Max; EC_No++)
	{
		EffectCounter[EC_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		EffectCounter[EC_No].Count = 0;
		EffectCounter[EC_No].AnimPattern = 0;
		EffectCounter[EC_No].Type = 0;
		EffectCounter[EC_No].Use = false;
	}

	temp = D3DXVECTOR2(Texture_DeadEffect_Width / 2, Texture_DeadEffect_Height / 2);
	Radius[DeadEffect] = D3DXVec2Length(&temp);
	BaseAngle[DeadEffect] = atan2f(Texture_DeadEffect_Height, Texture_DeadEffect_Width);

	temp = D3DXVECTOR2(Texture_BulletDead_Width / 2, Texture_BulletDead_Height / 2);
	Radius[BulletDead] = D3DXVec2Length(&temp);
	BaseAngle[BulletDead] = atan2f(Texture_BulletDead_Height, Texture_BulletDead_Width);

	temp = D3DXVECTOR2(Texture_BossBG_Circle_Width / 2, Texture_BossBG_Circle_Height / 2);
	Radius[BossBG_Circle] = D3DXVec2Length(&temp);
	BaseAngle[BossBG_Circle] = atan2f(Texture_BossBG_Circle_Height, Texture_BossBG_Circle_Width);

	temp = D3DXVECTOR2(Texture_BossBG_Star_Width / 2, Texture_BossBG_Star_Height / 2);
	Radius[BossBG_Star] = D3DXVec2Length(&temp);
	BaseAngle[BossBG_Star] = atan2f(Texture_BossBG_Star_Height, Texture_BossBG_Star_Width);

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_DeadEffect, &EffectTexture[DeadEffect], "Enemy") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BulletDead, &EffectTexture[BulletDead], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BossBG_Circle, &EffectTexture[BossBG_Circle], "Boss") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BossBG_Star, &EffectTexture[BossBG_Star], "Boss") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	for (int i = 0; i < EffectTexture_Max; i++)
	{
		SafeRelease(EffectTexture[i]);
	}

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	int EC_No = 0;
	int Effect_No = 0;

	for (EC_No = 0; EC_No < EffectCounter_Max; EC_No++)
	{
		if (EffectCounter[EC_No].Use == true)
		{
			SetEffect(EC_No, EffectCounter[EC_No].Type);
			EffectCounter[EC_No].Count++;
		}
	}

	for (Effect_No = 0; Effect_No < Effect_Max; Effect_No++)
	{
		if (Effect[Effect_No].Use == true)
		{
			CalculateEffect(Effect_No, Effect[Effect_No].TextureType);

			Effect[Effect_No].Count++;

			SetEffectTexture(Effect_No);
			SetEffectDiffuse(Effect_No, Effect[Effect_No].Alpha);
			SetEffectVertex(Effect_No);
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Effect_No = 0;

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (Effect_No = 0; Effect_No < Effect_Max; Effect_No++)
	{
		if (Effect[Effect_No].Use == true)
		{
			// テクスチャの設定
			switch (Effect[Effect_No].TextureType)
			{
			case DeadEffect:
				Device->SetTexture(0, EffectTexture[DeadEffect]);
				// 加算合成モードに設置する
				Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			case BossDeadEffect:
				Device->SetTexture(0, EffectTexture[DeadEffect]);
				break;
			case BulletDead:
				Device->SetTexture(0, EffectTexture[BulletDead]);
				break;
			case BossBG_Circle:
				Device->SetTexture(0, EffectTexture[BossBG_Circle]);
				break;
			case BossBG_Star:
				Device->SetTexture(0, EffectTexture[BossBG_Star]);
			default:
				break;
			}
			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Effect[Effect_No].VertexWk, sizeof(VERTEX_2D));
			// 本来の設置を戻す
			Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeEffectVertex(int Effect_No)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点座標の設定	
	SetEffectVertex(Effect_No);

	// rhwの設定
	Effect[Effect_No].VertexWk[0].rhw = 1.0f;
	Effect[Effect_No].VertexWk[1].rhw = 1.0f;
	Effect[Effect_No].VertexWk[2].rhw = 1.0f;
	Effect[Effect_No].VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	SetEffectDiffuse(Effect_No, Effect[Effect_No].Alpha);

	// テクスチャ座標の設定
	SetEffectTexture(Effect_No);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetEffectTexture(int Effect_No)
{
	int AnimPattern = Effect[Effect_No].AnimPattern;
	float sizeX = 0.0f;

	switch (Effect[Effect_No].TextureType)
	{
	case DeadEffect:
	case BossDeadEffect:
		sizeX = 1.0f / Texture_DeadEffect_Divide_X;
		Effect[Effect_No].VertexWk[0].tex = D3DXVECTOR2((float)(AnimPattern)* sizeX, 0.0f);
		Effect[Effect_No].VertexWk[1].tex = D3DXVECTOR2((float)(AnimPattern)* sizeX + sizeX, 0.0f);
		Effect[Effect_No].VertexWk[2].tex = D3DXVECTOR2((float)(AnimPattern)* sizeX, 1.0f);
		Effect[Effect_No].VertexWk[3].tex = D3DXVECTOR2((float)(AnimPattern)* sizeX + sizeX, 1.0f);
		break;
	case BulletDead:
		sizeX = 1.0f / Texture_BulletDead_Divide_X;
		Effect[Effect_No].VertexWk[0].tex = D3DXVECTOR2((float)(AnimPattern)* sizeX, 0.0f);
		Effect[Effect_No].VertexWk[1].tex = D3DXVECTOR2((float)(AnimPattern)* sizeX + sizeX, 0.0f);
		Effect[Effect_No].VertexWk[2].tex = D3DXVECTOR2((float)(AnimPattern)* sizeX, 1.0f);
		Effect[Effect_No].VertexWk[3].tex = D3DXVECTOR2((float)(AnimPattern)* sizeX + sizeX, 1.0f);
		break;
	case BossBG_Circle:
	case BossBG_Star:
		Effect[Effect_No].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Effect[Effect_No].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Effect[Effect_No].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Effect[Effect_No].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// 反射光の設定
//=============================================================================
void SetEffectDiffuse(int Effect_No, int Alpha)
{
	// 反射光の設定
	Effect[Effect_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	Effect[Effect_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	Effect[Effect_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	Effect[Effect_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, Alpha);

	return;
}



//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetEffectVertex(int Effect_No)
{
	Effect[Effect_No].VertexWk[0].vtx.x =
		Effect[Effect_No].Pos.x - cosf(Effect[Effect_No].BaseAngle + Effect[Effect_No].Rot.z) * Effect[Effect_No].Radius;
	Effect[Effect_No].VertexWk[0].vtx.y =
		Effect[Effect_No].Pos.y - sinf(Effect[Effect_No].BaseAngle + Effect[Effect_No].Rot.z) * Effect[Effect_No].Radius;
	Effect[Effect_No].VertexWk[1].vtx.x =
		Effect[Effect_No].Pos.x + cosf(Effect[Effect_No].BaseAngle - Effect[Effect_No].Rot.z) * Effect[Effect_No].Radius;
	Effect[Effect_No].VertexWk[1].vtx.y =
		Effect[Effect_No].Pos.y - sinf(Effect[Effect_No].BaseAngle - Effect[Effect_No].Rot.z) * Effect[Effect_No].Radius;
	Effect[Effect_No].VertexWk[2].vtx.x =
		Effect[Effect_No].Pos.x - cosf(Effect[Effect_No].BaseAngle - Effect[Effect_No].Rot.z) * Effect[Effect_No].Radius;
	Effect[Effect_No].VertexWk[2].vtx.y =
		Effect[Effect_No].Pos.y + sinf(Effect[Effect_No].BaseAngle - Effect[Effect_No].Rot.z) * Effect[Effect_No].Radius;
	Effect[Effect_No].VertexWk[3].vtx.x =
		Effect[Effect_No].Pos.x + cosf(Effect[Effect_No].BaseAngle + Effect[Effect_No].Rot.z) * Effect[Effect_No].Radius;
	Effect[Effect_No].VertexWk[3].vtx.y =
		Effect[Effect_No].Pos.y + sinf(Effect[Effect_No].BaseAngle + Effect[Effect_No].Rot.z) * Effect[Effect_No].Radius;

	return;
}

//=============================================================================
// エネミー死亡エフェクトの設置
//=============================================================================
void SetEnemyDeadEffect(int Enemy_No)
{
	int EC_No = 0;
	ENEMY *Enemy = GetEnemy(Enemy_No);

	for (EC_No = 0; EC_No < EffectCounter_Max; EC_No++)
	{
		if (EffectCounter[EC_No].Use == false)
		{
			// 座標をセット
			EffectCounter[EC_No].Pos.x = Enemy->Pos.x;
			EffectCounter[EC_No].Pos.y = Enemy->Pos.y;
			// 色をセット
			EffectCounter[EC_No].AnimPattern = rand() % (4 + 1);
			EffectCounter[EC_No].Count = 0;
			EffectCounter[EC_No].Type = EnemyDead;
			EffectCounter[EC_No].Use = true;
			break;
		}
	}

	return;
}

//=============================================================================
// ボス死亡エフェクトの設置
//=============================================================================
void SetBossDeadEffect(void)
{
	int EC_No = 0;
	BOSS *Boss = GetBoss();

	for (EC_No = 0; EC_No < EffectCounter_Max; EC_No++)
	{
		if (EffectCounter[EC_No].Use == false)
		{
			// 座標をセット
			EffectCounter[EC_No].Pos.x = Boss->Pos.x;
			EffectCounter[EC_No].Pos.y = Boss->Pos.y;
			// 色をセット
			EffectCounter[EC_No].AnimPattern = 0;
			EffectCounter[EC_No].Count = 0;
			EffectCounter[EC_No].Type = BossDead;
			EffectCounter[EC_No].Use = true;

			SetBGShake(true);

			break;
		}
	}

	return;
}

//=============================================================================
// ボムによって、エネミーの弾が消えるエフェクトの設置
//=============================================================================
void SetBulletDeadEffect(D3DXVECTOR3 Pos)
{
	int EC_No = 0;

	for (EC_No = 0; EC_No < EffectCounter_Max; EC_No++)
	{
		if (EffectCounter[EC_No].Use == false)
		{
			// 座標をセット
			EffectCounter[EC_No].Pos.x = Pos.x;
			EffectCounter[EC_No].Pos.y = Pos.y;
			EffectCounter[EC_No].AnimPattern = 0;
			EffectCounter[EC_No].Count = 0;
			EffectCounter[EC_No].Type = BulletDisappear;
			EffectCounter[EC_No].Use = true;
			break;
		}
	}

	return;
}

//=============================================================================
// ボス背景エフェクトの設置
//=============================================================================
void SetBossEffect(void)
{
	int EC_No = 0;
	BOSS *Boss = GetBoss();

	for (EC_No = 0; EC_No < EffectCounter_Max; EC_No++)
	{
		if (EffectCounter[EC_No].Use == false)
		{
			// 座標をセット
			EffectCounter[EC_No].Pos.x = Boss->Pos.x;
			EffectCounter[EC_No].Pos.y = Boss->Pos.y;
			// 色をセット
			EffectCounter[EC_No].AnimPattern = 0;
			EffectCounter[EC_No].Count = 0;
			EffectCounter[EC_No].Type = BossBackGround;
			EffectCounter[EC_No].Use = true;
			break;
		}
	}

	return;
}


//=============================================================================
// エフェクトの設置
//=============================================================================
void SetEffect(int EC_No, int EffectType)
{
	int Effect_No = 0;
	BOSS *Boss = GetBoss();

	for (Effect_No = 0; Effect_No < Effect_Max; Effect_No++)
	{
		if (Effect[Effect_No].Use == false)
		{
			// EffectCounterのEffectTypeによって、エフェクトを設置する
			switch (EffectType)
			{
			case EnemyDead:
				if (EffectCounter[EC_No].Count % 2 == 0)
				{
					Effect[Effect_No].Use = true;
					// 座標をセット
					Effect[Effect_No].Pos = EffectCounter[EC_No].Pos;
					// 輝度を最大値に
					Effect[Effect_No].Alpha = 255;
					// 角度をランダムに
					Effect[Effect_No].Rot.z = RandRadian(DegreeToRadian(180));
					Effect[Effect_No].BaseAngle = BaseAngle[DeadEffect];
					Effect[Effect_No].Radius = Radius[DeadEffect];
					// 色をセット
					Effect[Effect_No].Count = 0;
					Effect[Effect_No].AnimPattern = EffectCounter[EC_No].AnimPattern;
					Effect[Effect_No].TextureType = DeadEffect;
				}
				if (EffectCounter[EC_No].Count > 8)
				{
					// カウントが8より大きくなったら終了
					EffectCounter[EC_No].Use = false;
				}
				break;
			case BossDead:
				if (EffectCounter[EC_No].Count % 2 == 0)
				{
					Effect[Effect_No].Use = true;
					// 座標をセット
					Effect[Effect_No].Pos = EffectCounter[EC_No].Pos;
					// 輝度を最大値に
					Effect[Effect_No].Alpha = 255;
					// 角度をランダムに
					Effect[Effect_No].Rot.z = DegreeToRadian(24) * EffectCounter[EC_No].Count / 2;
					Effect[Effect_No].BaseAngle = BaseAngle[DeadEffect];
					Effect[Effect_No].Radius = Radius[DeadEffect] * 2;
					// 色をセット
					Effect[Effect_No].Count = 0;
					Effect[Effect_No].AnimPattern = rand() % (4 + 1);
					Effect[Effect_No].TextureType = BossDeadEffect;
				}
				if (EffectCounter[EC_No].Count > 30)
				{
					// カウントが30より大きくなったら終了
					SetBGShake(false);
					EffectCounter[EC_No].Use = false;
				}
				break;
			case BulletDisappear:
				EffectCounter[EC_No].Use = false;
				Effect[Effect_No].Use = true;
				Effect[Effect_No].Pos = EffectCounter[EC_No].Pos;
				Effect[Effect_No].Alpha = 255;
				Effect[Effect_No].Rot.z = 0.0f;
				Effect[Effect_No].BaseAngle = BaseAngle[BulletDead];
				Effect[Effect_No].Radius = Radius[BulletDead];
				Effect[Effect_No].Count = 0;
				Effect[Effect_No].AnimPattern = 0;
				Effect[Effect_No].TextureType = BulletDead;
				break;
			case BossBackGround:
				if (EffectCounter[EC_No].Count == 0)
				{
					Effect[Effect_No].Use = true;
					Effect[Effect_No].Pos = EffectCounter[EC_No].Pos;
					Effect[Effect_No].Alpha = 255;
					Effect[Effect_No].Rot.z = 0.0f;
					Effect[Effect_No].BaseAngle = BaseAngle[BossBG_Circle];
					Effect[Effect_No].Radius = Radius[BossBG_Circle];
					Effect[Effect_No].Count = 0;
					Effect[Effect_No].AnimPattern = 0;
					Effect[Effect_No].TextureType = BossBG_Circle;
				}
				else if (EffectCounter[EC_No].Count == 1)
				{
					Effect[Effect_No].Use = true;
					Effect[Effect_No].Pos = EffectCounter[EC_No].Pos;
					Effect[Effect_No].Alpha = 255;
					Effect[Effect_No].Rot.z = 0.0f;
					Effect[Effect_No].BaseAngle = BaseAngle[BossBG_Star];
					Effect[Effect_No].Radius = Radius[BossBG_Star];
					Effect[Effect_No].Count = 0;
					Effect[Effect_No].AnimPattern = 0;
					Effect[Effect_No].TextureType = BossBG_Star;
				}
				if (EffectCounter[EC_No].Count >= 2)
				{
					// カウントが2より大きくなったら終了
					EffectCounter[EC_No].Use = false;
				}
				break;
			default:
				break;
			}
			break;
		}
	}

	return;
}

//=============================================================================
// エフェクトの計算
//=============================================================================
void CalculateEffect(int Effect_No, int TextureType)
{
	int Count = Effect[Effect_No].Count;
	BOSS *Boss = GetBoss();

	switch (TextureType)
	{
	case DeadEffect:
		if (Count < 20)
		{
			Effect[Effect_No].Radius += 0.1f;
			if (Count >= 10)
			{
				Effect[Effect_No].Alpha -= 25;
			}
		}
		else
		{
			Effect[Effect_No].Use = false;
		}
		break;
	case BossDeadEffect:
		if (Count < 60)
		{
			Effect[Effect_No].Radius += 0.1f;
		}
		else
		{
			Effect[Effect_No].Use = false;
		}
		break;
	case BulletDead:
		if (Count % 2 == 0)
		{
			Effect[Effect_No].AnimPattern++;
		}
		if (Count > 20)
		{
			Effect[Effect_No].Use = false;
		}
		break;
	case BossBG_Circle:
		if (Boss->Exist == false)
		{
			Effect[Effect_No].Use = false;
		}
		Effect[Effect_No].Pos = Boss->Pos;
		if (Count % 300 < 150)
		{
			Effect[Effect_No].Radius += 0.5f;
		}
		else
		{
			Effect[Effect_No].Radius -= 0.5f;
		}
		Effect[Effect_No].Rot.z += 0.05f;
		break;
	case BossBG_Star:
		if (Boss->Exist == false)
		{
			Effect[Effect_No].Use = false;
		}
		Effect[Effect_No].Pos = Boss->Pos;
		Effect[Effect_No].Rot.z += 0.01f;
		break;
	default:
		break;
	}

	return;
}
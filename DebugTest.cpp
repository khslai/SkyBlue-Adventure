//=============================================================================
//
// デバッグテスト処理 [DebugTest.cpp]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#include "main.h"
#include "DebugTest.h"
#include "Player.h"
#include "Enemy.h"
#include "Barrage.h"
#include "Bomb.h"
#include "Boss.h"
#include "Input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define HitCircle_Max (999)
#define HitCircle_Alpha (200)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// HC = HitCircle
HRESULT MakeHCVertex(int HitCircle_No);
void SetHCTexture(int HitCircle_No);
void SetHCVertex(int HitCircle_No);
void UpdateHitCircle(void);
void DrawHitCircle(void);
void DrawFPS(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 HCTexture = NULL;
HITCIRCLE HitCircle[HitCircle_Max];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDebugTest(int InitState)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int HitCircle_No = 0;
	float BaseAngle = 0.0f;

	BaseAngle = atan2f(Texture_HitCircle_Height, Texture_HitCircle_Width);

	for (HitCircle_No = 0; HitCircle_No < HitCircle_Max; HitCircle_No++)
	{
		HitCircle[HitCircle_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		HitCircle[HitCircle_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		HitCircle[HitCircle_No].BaseAngle = BaseAngle;
		HitCircle[HitCircle_No].Radius = 0.0f;
		HitCircle[HitCircle_No].Owner = 0;
		HitCircle[HitCircle_No].EnemyNo = 0;
		HitCircle[HitCircle_No].BulletNo = 0;
		HitCircle[HitCircle_No].BarrageNo = 0;
		HitCircle[HitCircle_No].Use = false;

		// 頂点情報の作成
		MakeHCVertex(HitCircle_No);
	}

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_HitCircle, &HCTexture, "HitCircle") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDebugTest(void)
{
	SafeRelease(HCTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDebugTest(void)
{
	UpdateHitCircle();

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDebugTest(void)
{
	// 当たり範囲表示
	//DrawHitCircle();

	// FPS表示
	//DrawFPS();

	return;
}

//=============================================================================
// HitCircle更新処理
//=============================================================================
void UpdateHitCircle(void)
{
	int HitCircle_No = 0;
	int Barrage_No = 0;
	int Bullet_No = 0;
	int Enemy_No = 0;
	PLAYER *Player = GetPlayer(0);
	ENEMY *Enemy = GetEnemy(0);
	BARRAGE *Barrage = GetBarrage(Barrage_No);
	BOMB *Bomb = GetBomb();
	BOSS *Boss = GetBoss();

	for (HitCircle_No = 0; HitCircle_No < HitCircle_Max; HitCircle_No++)
	{
		if (HitCircle[HitCircle_No].Use == true)
		{
			if (HitCircle[HitCircle_No].Owner == PlayerHC)
			{
				HitCircle[HitCircle_No].Pos = Player->Pos;
			}
			else if (HitCircle[HitCircle_No].Owner == PlayerBulletHC)
			{
				for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
				{
					if (HitCircle[HitCircle_No].BulletNo == Bullet_No)
					{
						if (Player->Bullet[Bullet_No].Use == true)
						{
							HitCircle[HitCircle_No].Pos = Player->Bullet[Bullet_No].Pos;
						}
						else
						{
							HitCircle[HitCircle_No].Use = false;
						}
						break;
					}
				}
			}
			else if (HitCircle[HitCircle_No].Owner == BombHC)
			{
				if (Bomb->Use == true)
				{
					HitCircle[HitCircle_No].Pos = Bomb->Pos;
					HitCircle[HitCircle_No].Radius = Bomb->HitRadius;
				}
				else
				{
					HitCircle[HitCircle_No].Use = false;
				}
			}
			else if (HitCircle[HitCircle_No].Owner == EnemyHC)
			{
				Enemy = GetEnemy(0);
				for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++, Enemy++)
				{
					if (HitCircle[HitCircle_No].EnemyNo == Enemy_No)
					{
						if (Enemy->Exist == true)
						{
							HitCircle[HitCircle_No].Pos = Enemy->Pos;
						}
						else
						{
							HitCircle[HitCircle_No].Use = false;
						}
						break;
					}
				}
			}
			else if (HitCircle[HitCircle_No].Owner == EnemyBulletHC)
			{
				Barrage = GetBarrage(0);
				for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++, Barrage++)
				{
					if (HitCircle[HitCircle_No].BarrageNo == Barrage_No)
					{
						if (Barrage->Phase != BarrageOver)
						{
							for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
							{
								if (HitCircle[HitCircle_No].BulletNo == Bullet_No)
								{
									if (Barrage->Bullet[Bullet_No].Use == true)
									{
										HitCircle[HitCircle_No].Pos = Barrage->Bullet[Bullet_No].Pos;
									}
									else
									{
										HitCircle[HitCircle_No].Use = false;
									}
									break;
								}
							}
						}
						else
						{
							HitCircle[HitCircle_No].Use = false;
						}
						break;
					}
				}
			}
			else if (HitCircle[HitCircle_No].Owner == BossHC)
			{
				if (Boss->Exist == true)
				{
					HitCircle[HitCircle_No].Pos = Boss->Pos;
				}
				else
				{
					HitCircle[HitCircle_No].Use = false;
				}
			}
			else if (HitCircle[HitCircle_No].Owner == BossBulletHC)
			{
				for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
				{
					if (HitCircle[HitCircle_No].BulletNo == Bullet_No)
					{
						if (Boss->Bullet[Bullet_No].Use == true)
						{
							HitCircle[HitCircle_No].Pos = Boss->Bullet[Bullet_No].Pos;
						}
						else
						{
							HitCircle[HitCircle_No].Use = false;
						}
					}
				}
			}

			SetHCTexture(HitCircle_No);
			SetHCVertex(HitCircle_No);
		}
	}

	return;
}

//=============================================================================
// HitCircle描画処理
//=============================================================================
void DrawHitCircle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int HitCircle_No = 0;

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (HitCircle_No = 0; HitCircle_No < HitCircle_Max; HitCircle_No++)
	{
		if (HitCircle[HitCircle_No].Use == true)
		{
			// テクスチャの設定
			Device->SetTexture(0, HCTexture);

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, HitCircle[HitCircle_No].VertexWk, sizeof(VERTEX_2D));
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeHCVertex(int HitCircle_No)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点座標の設定	
	SetHCVertex(HitCircle_No);

	// rhwの設定
	HitCircle[HitCircle_No].VertexWk[0].rhw = 1.0f;
	HitCircle[HitCircle_No].VertexWk[1].rhw = 1.0f;
	HitCircle[HitCircle_No].VertexWk[2].rhw = 1.0f;
	HitCircle[HitCircle_No].VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	HitCircle[HitCircle_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, HitCircle_Alpha);
	HitCircle[HitCircle_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, HitCircle_Alpha);
	HitCircle[HitCircle_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, HitCircle_Alpha);
	HitCircle[HitCircle_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, HitCircle_Alpha);

	// テクスチャ座標の設定
	SetHCTexture(HitCircle_No);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetHCTexture(int HitCircle_No)
{
	HitCircle[HitCircle_No].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	HitCircle[HitCircle_No].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	HitCircle[HitCircle_No].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	HitCircle[HitCircle_No].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetHCVertex(int HitCircle_No)
{
	HitCircle[HitCircle_No].VertexWk[0].vtx.x =
		HitCircle[HitCircle_No].Pos.x - HitCircle[HitCircle_No].Radius;
	HitCircle[HitCircle_No].VertexWk[0].vtx.y =
		HitCircle[HitCircle_No].Pos.y - HitCircle[HitCircle_No].Radius;
	HitCircle[HitCircle_No].VertexWk[1].vtx.x =
		HitCircle[HitCircle_No].Pos.x + HitCircle[HitCircle_No].Radius;
	HitCircle[HitCircle_No].VertexWk[1].vtx.y =
		HitCircle[HitCircle_No].Pos.y - HitCircle[HitCircle_No].Radius;
	HitCircle[HitCircle_No].VertexWk[2].vtx.x =
		HitCircle[HitCircle_No].Pos.x - HitCircle[HitCircle_No].Radius;
	HitCircle[HitCircle_No].VertexWk[2].vtx.y =
		HitCircle[HitCircle_No].Pos.y + HitCircle[HitCircle_No].Radius;
	HitCircle[HitCircle_No].VertexWk[3].vtx.x =
		HitCircle[HitCircle_No].Pos.x + HitCircle[HitCircle_No].Radius;
	HitCircle[HitCircle_No].VertexWk[3].vtx.y =
		HitCircle[HitCircle_No].Pos.y + HitCircle[HitCircle_No].Radius;

	/*
	HitCircle[HitCircle_No].VertexWk[0].vtx.x = HitCircle[HitCircle_No].Pos.x - cosf(HitCircle[HitCircle_No].BaseAngle + HitCircle[HitCircle_No].Rot.z) * HitCircle[HitCircle_No].Radius * Root2;
	HitCircle[HitCircle_No].VertexWk[0].vtx.y = HitCircle[HitCircle_No].Pos.y - sinf(HitCircle[HitCircle_No].BaseAngle + HitCircle[HitCircle_No].Rot.z) * HitCircle[HitCircle_No].Radius * Root2;
	HitCircle[HitCircle_No].VertexWk[1].vtx.x = HitCircle[HitCircle_No].Pos.x + cosf(HitCircle[HitCircle_No].BaseAngle - HitCircle[HitCircle_No].Rot.z) * HitCircle[HitCircle_No].Radius * Root2;
	HitCircle[HitCircle_No].VertexWk[1].vtx.y = HitCircle[HitCircle_No].Pos.y - sinf(HitCircle[HitCircle_No].BaseAngle - HitCircle[HitCircle_No].Rot.z) * HitCircle[HitCircle_No].Radius * Root2;
	HitCircle[HitCircle_No].VertexWk[2].vtx.x = HitCircle[HitCircle_No].Pos.x - cosf(HitCircle[HitCircle_No].BaseAngle - HitCircle[HitCircle_No].Rot.z) * HitCircle[HitCircle_No].Radius * Root2;
	HitCircle[HitCircle_No].VertexWk[2].vtx.y = HitCircle[HitCircle_No].Pos.y + sinf(HitCircle[HitCircle_No].BaseAngle - HitCircle[HitCircle_No].Rot.z) * HitCircle[HitCircle_No].Radius * Root2;
	HitCircle[HitCircle_No].VertexWk[3].vtx.x = HitCircle[HitCircle_No].Pos.x + cosf(HitCircle[HitCircle_No].BaseAngle + HitCircle[HitCircle_No].Rot.z) * HitCircle[HitCircle_No].Radius * Root2;
	HitCircle[HitCircle_No].VertexWk[3].vtx.y = HitCircle[HitCircle_No].Pos.y + sinf(HitCircle[HitCircle_No].BaseAngle + HitCircle[HitCircle_No].Rot.z) * HitCircle[HitCircle_No].Radius * Root2;
	*/
}

//=============================================================================
// 当たり範囲の設置
//=============================================================================
void SetPlayerHC(D3DVECTOR Pos, float HitRadius, int Owner)
{
	int HitCircle_No = 0;

	for (HitCircle_No = 0; HitCircle_No < HitCircle_Max; HitCircle_No++)
	{
		if (HitCircle[HitCircle_No].Use == false)
		{
			HitCircle[HitCircle_No].Pos.x = Pos.x;
			HitCircle[HitCircle_No].Pos.y = Pos.y;
			HitCircle[HitCircle_No].Radius = HitRadius;
			HitCircle[HitCircle_No].Owner = Owner;
			HitCircle[HitCircle_No].Use = true;
			break;
		}
	}

	return;
}

//=============================================================================
// 当たり範囲の設置
//=============================================================================
void SetPlayerBulletHC(D3DVECTOR Pos, float HitRadius, int Bullet_No, int Owner)
{
	int HitCircle_No = 0;

	for (HitCircle_No = 0; HitCircle_No < HitCircle_Max; HitCircle_No++)
	{
		if (HitCircle[HitCircle_No].Use == false)
		{
			HitCircle[HitCircle_No].Pos.x = Pos.x;
			HitCircle[HitCircle_No].Pos.y = Pos.y;
			HitCircle[HitCircle_No].Radius = HitRadius;
			HitCircle[HitCircle_No].Owner = Owner;
			HitCircle[HitCircle_No].BulletNo = Bullet_No;
			HitCircle[HitCircle_No].Use = true;
			break;
		}
	}

	return;
}

//=============================================================================
// 当たり範囲の設置
//=============================================================================
void SetBombHC(D3DVECTOR Pos, float HitRadius, int Owner)
{
	int HitCircle_No = 0;

	for (HitCircle_No = 0; HitCircle_No < HitCircle_Max; HitCircle_No++)
	{
		if (HitCircle[HitCircle_No].Use == false)
		{
			HitCircle[HitCircle_No].Pos.x = Pos.x;
			HitCircle[HitCircle_No].Pos.y = Pos.y;
			HitCircle[HitCircle_No].Radius = HitRadius;
			HitCircle[HitCircle_No].Owner = Owner;
			HitCircle[HitCircle_No].Use = true;
			break;
		}
	}

	return;
}

//=============================================================================
// 当たり範囲の設置
//=============================================================================
void SetEnemyHC(D3DVECTOR Pos, float HitRadius, int Enemy_No, int Owner)
{
	int HitCircle_No = 0;

	for (HitCircle_No = 0; HitCircle_No < HitCircle_Max; HitCircle_No++)
	{
		if (HitCircle[HitCircle_No].Use == false)
		{
			HitCircle[HitCircle_No].Pos.x = Pos.x;
			HitCircle[HitCircle_No].Pos.y = Pos.y;
			HitCircle[HitCircle_No].Radius = HitRadius;
			HitCircle[HitCircle_No].Owner = Owner;
			HitCircle[HitCircle_No].EnemyNo = Enemy_No;
			HitCircle[HitCircle_No].Use = true;
			break;
		}
	}

	return;
}

//=============================================================================
// 当たり範囲の設置
//=============================================================================
void SetEnemyBulletHC(D3DVECTOR Pos, float HitRadius, int Barrage_No, int Bullet_No, int Owner)
{
	int HitCircle_No = 0;

	for (HitCircle_No = 0; HitCircle_No < HitCircle_Max; HitCircle_No++)
	{
		if (HitCircle[HitCircle_No].Use == false)
		{
			HitCircle[HitCircle_No].Pos.x = Pos.x;
			HitCircle[HitCircle_No].Pos.y = Pos.y;
			HitCircle[HitCircle_No].Radius = HitRadius;
			HitCircle[HitCircle_No].Owner = Owner;
			HitCircle[HitCircle_No].BarrageNo = Barrage_No;
			HitCircle[HitCircle_No].BulletNo = Bullet_No;
			HitCircle[HitCircle_No].Use = true;
			break;
		}
	}

	return;
}

//=============================================================================
// 当たり範囲の設置
//=============================================================================
void SetBossHC(D3DVECTOR Pos, float HitRadius, int Owner)
{
	int HitCircle_No = 0;

	for (HitCircle_No = 0; HitCircle_No < HitCircle_Max; HitCircle_No++)
	{
		if (HitCircle[HitCircle_No].Use == false)
		{
			HitCircle[HitCircle_No].Pos.x = Pos.x;
			HitCircle[HitCircle_No].Pos.y = Pos.y;
			HitCircle[HitCircle_No].Radius = HitRadius;
			HitCircle[HitCircle_No].Owner = Owner;
			HitCircle[HitCircle_No].Use = true;
			break;
		}
	}

	return;
}

//=============================================================================
// 当たり範囲の設置
//=============================================================================
void SetBossBulletHC(D3DVECTOR Pos, float HitRadius, int Bullet_No, int Owner)
{
	int HitCircle_No = 0;

	for (HitCircle_No = 0; HitCircle_No < HitCircle_Max; HitCircle_No++)
	{
		if (HitCircle[HitCircle_No].Use == false)
		{
			HitCircle[HitCircle_No].Pos.x = Pos.x;
			HitCircle[HitCircle_No].Pos.y = Pos.y;
			HitCircle[HitCircle_No].Radius = HitRadius;
			HitCircle[HitCircle_No].Owner = Owner;
			HitCircle[HitCircle_No].BulletNo = Bullet_No;
			HitCircle[HitCircle_No].Use = true;
			break;
		}
	}

	return;
}

//=============================================================================
// FPS表示処理
//=============================================================================
void DrawFPS(void)
{
	int FPSCount = GetFPSCount();
	int GameCount = GetGameCount();
	LPD3DXFONT DebugFont = GetFont(FontSize_30);
	RECT rect = { 0, 0, Screen_Width, Screen_Height };
	TCHAR str[256];

	wsprintf(str, _T("FPS:%d\n"), FPSCount);
	// テキスト描画
	DebugFont->DrawText(NULL, str, -1, &rect, DT_LEFT | DT_BOTTOM, BLACK(255));

	rect.bottom -= 30;
	wsprintf(str, _T("GameCount:%d\n"), GameCount);
	DebugFont->DrawText(NULL, str, -1, &rect, DT_LEFT | DT_BOTTOM, BLACK(255));

	return;
}

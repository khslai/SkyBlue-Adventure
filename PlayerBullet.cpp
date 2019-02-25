//=============================================================================
//
// プレイヤー弾幕処理 [PlayerBullet.cpp]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#include "main.h"
#include "PlayerBullet.h"
#include "Player.h"
#include "Boss.h"
#include "Enemy.h"
#include "File.h"
#include "Sound.h"
#include "DebugTest.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PlayerBulletDamage (4)
#define PlayerBulletSpeed (20.0f)

enum BulletType
{
	PlayerBullet,
	OptionBullet,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
static HRESULT MakeBulletVertex(int Bullet_No);
static void SetBulletTexture(int Bullet_No);
static void SetBulletVertex(int Bullet_No);
void PlayerShot(void);
void CalculateHoming(int Bullet_No);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9 Bullet_ATexture = NULL;
static LPDIRECT3DTEXTURE9 Bullet_BTexture = NULL;
static LPDIRECT3DTEXTURE9 OptionBullet_ATexture = NULL;
static LPDIRECT3DTEXTURE9 OptionBullet_BTexture = NULL;
// 弾のダメージ倍率
const float ShotDamageRate[4] = { 1.0f, 1.2f, 1.3f, 1.5f };
const float HomingDamageRate = 0.15f;
// 弾発射位置(Pos.x + 10, Pos.y - 20) , (Pos.x + 10, Pos.y + 20)
const int PlayerShot_x[2] = { 10, 10 };
const int PlayerShot_y[2] = { -20, 20 };
// オプション弾発射角度(Pos.x + 10, Pos.y - 20) , (Pos.x + 10, Pos.y + 20)
const int OptionAngle[4] = { -30, 30, -60, 60 };
const int OptionAngle_LowSpeed[4] = { 0, 0, -15, 15 };


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayerBullet(int InitState)
{
	int Bullet_No = 0;
	PLAYER *Player = GetPlayer(0);

	for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
	{
		Player->Bullet[Bullet_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Player->Bullet[Bullet_No].PreviousPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Player->Bullet[Bullet_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Player->Bullet[Bullet_No].Radius = 0.0f;
		Player->Bullet[Bullet_No].HitRadius = 0.0f;
		Player->Bullet[Bullet_No].BaseAngle = 0.0f;
		Player->Bullet[Bullet_No].MoveAngle = 0.0f;
		Player->Bullet[Bullet_No].Speed = 0.0f;
		Player->Bullet[Bullet_No].Damage = 0;
		Player->Bullet[Bullet_No].Type = 0;
		Player->Bullet[Bullet_No].Count = 0;
		Player->Bullet[Bullet_No].Color = 0;
		Player->Bullet[Bullet_No].State = 0;
		Player->Bullet[Bullet_No].Use = false;
		Player->Bullet[Bullet_No].DelayDisappear = false;

		// 頂点情報の作成
		MakeBulletVertex(Bullet_No);
	}

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_PlayerBullet_A, &Bullet_ATexture, "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_PlayerBullet_B, &Bullet_BTexture, "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_OptionBullet_A, &OptionBullet_ATexture, "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_OptionBullet_B, &OptionBullet_BTexture, "Player") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayerBullet(void)
{
	SafeRelease(Bullet_ATexture);
	SafeRelease(Bullet_BTexture);
	SafeRelease(OptionBullet_ATexture);
	SafeRelease(OptionBullet_BTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayerBullet(void)
{
	int Bullet_No = 0;
	int GameStage = GetGameStage();
	PLAYER *Player = GetPlayer(0);
	BULLETTYPE *BulletType = GetBulletType(Player->BulletType);

	for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
	{
		if (Player->Bullet[Bullet_No].Use == true)
		{
			if (Player->Bullet[Bullet_No].DelayDisappear == true)
			{
				Player->Bullet[Bullet_No].Use = false;
			}
			else
			{
				if (Player->Type == Player_B && Player->Bullet[Bullet_No].Type == OptionBullet)
				{
					CalculateHoming(Bullet_No);
				}
				Player->Bullet[Bullet_No].PreviousPos = Player->Bullet[Bullet_No].Pos;
				Player->Bullet[Bullet_No].Pos.x +=
					cosf(Player->Bullet[Bullet_No].MoveAngle) * Player->Bullet[Bullet_No].Speed;
				Player->Bullet[Bullet_No].Pos.y +=
					sinf(Player->Bullet[Bullet_No].MoveAngle) * Player->Bullet[Bullet_No].Speed;
				if (Player->Type == Player_B && Player->Bullet[Bullet_No].Type == OptionBullet)
				{
					Player->Bullet[Bullet_No].Rot.z += 0.1f;
				}
				Player->Bullet[Bullet_No].Count++;
				if (GameStage != Stage_SelectChar)
				{
					if (CheckRange(OutOfRange, Player->Bullet[Bullet_No].Pos,
						BulletType->Width, BulletType->Height) == true)
					{
						Player->Bullet[Bullet_No].Use = false;
					}
				}
				else
				{
					if (CheckRange(OutOfRange_SelectChar, Player->Bullet[Bullet_No].Pos,
						BulletType->Width, BulletType->Height) == true)
					{
						Player->Bullet[Bullet_No].Use = false;
					}
				}


				SetBulletTexture(Bullet_No);
				SetBulletVertex(Bullet_No);
			}
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayerBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	PLAYER *Player = GetPlayer(0);
	int Bullet_No = 0;

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
	{
		if (Player->Bullet[Bullet_No].Use == true)
		{
			// テクスチャの設定
			if (Player->Bullet[Bullet_No].Type == PlayerBullet)
			{
				if (Player->Type == Player_A)
				{
					Device->SetTexture(0, Bullet_ATexture);
				}
				else if (Player->Type == Player_B)
				{
					Device->SetTexture(0, Bullet_BTexture);
				}
			}
			else if (Player->Bullet[Bullet_No].Type == OptionBullet)
			{
				if (Player->Type == Player_A)
				{
					Device->SetTexture(0, OptionBullet_ATexture);
				}
				else if (Player->Type == Player_B)
				{
					Device->SetTexture(0, OptionBullet_BTexture);
				}
			}

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Player->Bullet[Bullet_No].VertexWk, sizeof(VERTEX_2D));
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeBulletVertex(int Bullet_No)
{
	PLAYER *Player = GetPlayer(0);

	// 頂点座標の設定	
	SetBulletVertex(Bullet_No);

	// rhwの設定
	Player->Bullet[Bullet_No].VertexWk[0].rhw = 1.0f;
	Player->Bullet[Bullet_No].VertexWk[1].rhw = 1.0f;
	Player->Bullet[Bullet_No].VertexWk[2].rhw = 1.0f;
	Player->Bullet[Bullet_No].VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	Player->Bullet[Bullet_No].VertexWk[0].diffuse = WHITE(255);
	Player->Bullet[Bullet_No].VertexWk[1].diffuse = WHITE(255);
	Player->Bullet[Bullet_No].VertexWk[2].diffuse = WHITE(255);
	Player->Bullet[Bullet_No].VertexWk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetBulletTexture(Bullet_No);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBulletTexture(int Bullet_No)
{
	PLAYER *Player = GetPlayer(0);

	Player->Bullet[Bullet_No].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Player->Bullet[Bullet_No].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Player->Bullet[Bullet_No].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Player->Bullet[Bullet_No].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBulletVertex(int Bullet_No)
{
	PLAYER *Player = GetPlayer(0);

	Player->Bullet[Bullet_No].VertexWk[0].vtx.x =
		Player->Bullet[Bullet_No].Pos.x - cosf(Player->Bullet[Bullet_No].BaseAngle + Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[0].vtx.y =
		Player->Bullet[Bullet_No].Pos.y - sinf(Player->Bullet[Bullet_No].BaseAngle + Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[1].vtx.x =
		Player->Bullet[Bullet_No].Pos.x + cosf(Player->Bullet[Bullet_No].BaseAngle - Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[1].vtx.y =
		Player->Bullet[Bullet_No].Pos.y - sinf(Player->Bullet[Bullet_No].BaseAngle - Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[2].vtx.x =
		Player->Bullet[Bullet_No].Pos.x - cosf(Player->Bullet[Bullet_No].BaseAngle - Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[2].vtx.y =
		Player->Bullet[Bullet_No].Pos.y + sinf(Player->Bullet[Bullet_No].BaseAngle - Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[3].vtx.x =
		Player->Bullet[Bullet_No].Pos.x + cosf(Player->Bullet[Bullet_No].BaseAngle + Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[3].vtx.y =
		Player->Bullet[Bullet_No].Pos.y + sinf(Player->Bullet[Bullet_No].BaseAngle + Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;

	return;
}

//=============================================================================
// プレイヤー弾の設置
//=============================================================================
void PlayerShot(void)
{
	int i = 0;
	int j = 0;
	int Bullet_No = 0;
	int GameStage = GetGameStage();
	PLAYER *Player = GetPlayer(0);
	BULLETTYPE *BulletType = GetBulletType(Player->BulletType);

	// 弾発射音
	if (GameStage != Stage_SelectChar)
	{
		PlaySound(SE_PlayerShot, E_DS8_FLAG_NONE, true);
	}

	// 自機ショット、一回2発
	for (i = 0; i < 2; i++)
	{
		for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
		{
			if (Player->Bullet[Bullet_No].Use == false)
			{
				Player->Bullet[Bullet_No].Use = true;
				Player->Bullet[Bullet_No].DelayDisappear = false;
				// 通常モード
				if (Player->LowSpeedMode == false)
				{
					Player->Bullet[Bullet_No].Pos.x = Player->Pos.x + PlayerShot_x[i];
					Player->Bullet[Bullet_No].Pos.y = Player->Pos.y + PlayerShot_y[i];
				}
				// 低速モード
				else
				{
					Player->Bullet[Bullet_No].Pos.x = Player->Pos.x + PlayerShot_x[i] / 3;
					Player->Bullet[Bullet_No].Pos.y = Player->Pos.y + PlayerShot_y[i] / 2;
				}
				Player->Bullet[Bullet_No].Count = 0;
				Player->Bullet[Bullet_No].Type = PlayerBullet;
				Player->Bullet[Bullet_No].MoveAngle = 0.0f;
				Player->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
				Player->Bullet[Bullet_No].Rot.z = Player->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
				Player->Bullet[Bullet_No].Radius = BulletType->Radius;
				Player->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
				Player->Bullet[Bullet_No].Speed = PlayerBulletSpeed;
				if (GameStage != Stage_SelectChar)
				{
					if (Player->Power < 1.0f)
					{
						Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[0];
					}
					else if (Player->Power >= 1.0f && Player->Power < 2.0f)
					{
						Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[1];
					}
					else if (Player->Power >= 2.0f && Player->Power < 3.0f)
					{
						Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[2];
					}
					else if (Player->Power == 3.0f)
					{
						Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[3];
					}
				}
				else
				{
					Player->Bullet[Bullet_No].Damage = 0.0f;
				}

#ifdef _DEBUG
				SetPlayerBulletHC(Player->Bullet[Bullet_No].Pos, Player->Bullet[Bullet_No].HitRadius, Bullet_No, PlayerBulletHC);
#endif

				break;
			}
		}
	}

	// オプションショット
	for (i = 0; i < PlayerOption_Max; i++)
	{
		if (Player->Option[i].Use == true)
		{
			BulletType = GetBulletType(Player->Option[i].BulletType);
			for (j = 0; j < (Player->Power == 3.0f ? 2 : 1); j++)
			{
				for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
				{
					if (Player->Bullet[Bullet_No].Use == false)
					{
						Player->Bullet[Bullet_No].Use = true;
						Player->Bullet[Bullet_No].DelayDisappear = false;
						Player->Bullet[Bullet_No].Pos.x = Player->Option[i].Pos.x;
						Player->Bullet[Bullet_No].Pos.y = Player->Option[i].Pos.y;
						Player->Bullet[Bullet_No].Count = 0;
						Player->Bullet[Bullet_No].Type = OptionBullet;
						if (Player->LowSpeedMode == false)
						{
							Player->Bullet[Bullet_No].MoveAngle = DegreeToRadian(OptionAngle[i + j * 2]);
						}
						else
						{
							Player->Bullet[Bullet_No].MoveAngle = DegreeToRadian(OptionAngle_LowSpeed[i + j * 2]);
						}
						Player->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
						Player->Bullet[Bullet_No].Rot.z = Player->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
						Player->Bullet[Bullet_No].Radius = BulletType->Radius;
						Player->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
						Player->Bullet[Bullet_No].Speed = PlayerBulletSpeed;
						if (GameStage != Stage_SelectChar)
						{
							if (Player->Power < 1.0f)
							{
								Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[0];
							}
							else if (Player->Power >= 1.0f && Player->Power < 2.0f)
							{
								Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[1];
							}
							else if (Player->Power >= 2.0f && Player->Power < 3.0f)
							{
								Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[2];
							}
							else if (Player->Power == 3.0f)
							{
								Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[3];
							}
						}
						else
						{
							Player->Bullet[Bullet_No].Damage = 0.0f;
						}
						if (Player->Type == Player_B)
						{
							Player->Bullet[Bullet_No].Damage = Player->Bullet[Bullet_No].Damage * HomingDamageRate;
						}


#ifdef _DEBUG
						SetPlayerBulletHC(Player->Bullet[Bullet_No].Pos, Player->Bullet[Bullet_No].HitRadius, Bullet_No, PlayerBulletHC);
#endif

						break;
					}
				}
			}
		}
	}

	return;
}

//=============================================================================
// 弾のホーミングを計算
//=============================================================================
void CalculateHoming(int Bullet_No)
{
	int Enemy_No = 0;
	int Target = -1;
	float Min_Distance = 0.0f;
	float Distance = 0.0f;
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();
	ENEMY *Enemy = GetEnemy(Enemy_No);

	// ボスが居ない時
	if (Boss->Exist == false)
	{
		Enemy_No = 0;
		Enemy = GetEnemy(Enemy_No);
		for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++, Enemy++)
		{
			// 敵の総数分
			if (Enemy->Exist == true)
			{
				temp = Enemy->Pos - Player->Bullet[Bullet_No].Pos;
				// ショットと敵の距離
				Distance = D3DXVec3LengthSq(&temp);
				if (Distance < Min_Distance || Min_Distance == 0.0f)
				{
					// 計算結果が最小値かまだ格納していないなら
					// エネミー番号記録
					Target = Enemy_No;
					// エネミーとの距離記録
					Min_Distance = Distance;
				}
			}
		}
	}
	// 近い敵が見つかったら、あるいはボスがいて、HPがあるときは角度をセット
	if (Target != -1 || (Boss->Exist == true && Boss->HP > 0))
	{
		if (Boss->Exist == false)
		{
			Enemy = GetEnemy(Target);
			Player->Bullet[Bullet_No].MoveAngle = AngleCalculate(Player->Bullet[Bullet_No].Pos, Enemy->Pos);
		}
		else
		{
			Player->Bullet[Bullet_No].MoveAngle = AngleCalculate(Player->Bullet[Bullet_No].Pos, Boss->Pos);
		}
	}

	return;
}
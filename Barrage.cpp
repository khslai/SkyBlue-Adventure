//=============================================================================
//
// エネミー弾幕処理 [Barrage.cpp]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#include "main.h"
#include "Barrage.h"
#include "Player.h"
#include "Enemy.h"
#include "File.h"
#include "Sound.h"
#include "DebugTest.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
static HRESULT MakeBulletVertex(int Barrage_No, int Bullet_No);
static void SetBulletTexture(int Barrage_No, int Bullet_No);
static void SetBulletVertex(int Barrage_No, int Bullet_No);
// バレットの弾幕パターンを設置
void BarragePattern(int Barrage_No, int Papptern_No);
// 弾幕移動計算
void BulletCalculate(int Barrage_No);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// バレットテクスチャのポインタ
static LPDIRECT3DTEXTURE9 BulletTexture[BulletType_Max] = { NULL };
// 弾幕構造体
BARRAGE Barrage[Barrage_Max];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBarrage(int InitState)
{
	int Barrage_No = 0;
	int Bullet_No = 0;

	for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++)
	{
		for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
		{
			Barrage[Barrage_No].Bullet[Bullet_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Barrage[Barrage_No].Bullet[Bullet_No].PreviousPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Barrage[Barrage_No].Bullet[Bullet_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Barrage[Barrage_No].Bullet[Bullet_No].Radius = 0.0f;
			Barrage[Barrage_No].Bullet[Bullet_No].HitRadius = 0.0f;
			Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle = 0.0f;
			Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle = 0.0f;
			Barrage[Barrage_No].Bullet[Bullet_No].Speed = 0.0f;
			Barrage[Barrage_No].Bullet[Bullet_No].Type = 0;
			Barrage[Barrage_No].Bullet[Bullet_No].Count = 0;
			Barrage[Barrage_No].Bullet[Bullet_No].Color = 0;
			Barrage[Barrage_No].Bullet[Bullet_No].Use = false;
			Barrage[Barrage_No].Bullet[Bullet_No].DelayDisappear = false;

			// 頂点情報の作成
			MakeBulletVertex(Barrage_No, Bullet_No);
		}
		Barrage[Barrage_No].BaseAngle = 0.0f;
		Barrage[Barrage_No].Phase = BarrageOver;
		Barrage[Barrage_No].Type = 0;
		Barrage[Barrage_No].Count = 0;
		Barrage[Barrage_No].Attacker_No = 0;
	}

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_Bullet_00, &BulletTexture[0], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Bullet_01, &BulletTexture[1], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Bullet_02, &BulletTexture[2], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Bullet_03, &BulletTexture[3], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Bullet_04, &BulletTexture[4], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Bullet_05, &BulletTexture[5], "Bullet") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBarrage(void)
{
	for (int i = 0; i < BulletType_Max; i++)
	{
		SafeRelease(BulletTexture[i]);
	}

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBarrage(void)
{
	int Barrage_No = 0;
	int Bullet_No = 0;

	for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++)
	{
		if (Barrage[Barrage_No].Phase != BarrageOver)
		{
			// 弾幕パターン関数を呼ぶ
			BarragePattern(Barrage_No, Barrage[Barrage_No].Type);
			// Barrage_No番目の弾幕の移動を計算
			BulletCalculate(Barrage_No);
			Barrage[Barrage_No].Count++;
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBarrage(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Barrage_No = 0;
	int Bullet_No = 0;
	int Laser_No = 0;
	PLAYER *Player = GetPlayer(0);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++)
	{
		if (Barrage[Barrage_No].Phase != BarrageOver)
		{
			for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
			{
				if (Barrage[Barrage_No].Bullet[Bullet_No].Use == true)
				{
					// テクスチャの設定
					switch (Barrage[Barrage_No].Bullet[Bullet_No].Type)
					{
					case 0: Device->SetTexture(0, BulletTexture[0]); break;
					case 1: Device->SetTexture(0, BulletTexture[1]); break;
					case 2: Device->SetTexture(0, BulletTexture[2]); break;
					case 3: Device->SetTexture(0, BulletTexture[3]); break;
					case 4: Device->SetTexture(0, BulletTexture[4]); break;
					case 5: Device->SetTexture(0, BulletTexture[5]); break;
					default:
						break;
					}

					// ポリゴンの描画
					Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Barrage[Barrage_No].Bullet[Bullet_No].VertexWk, sizeof(VERTEX_2D));
				}
			}
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeBulletVertex(int Barrage_No, int Bullet_No)
{
	// 頂点座標の設定	
	SetBulletVertex(Barrage_No, Bullet_No);

	// rhwの設定
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[0].rhw = 1.0f;
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[1].rhw = 1.0f;
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[2].rhw = 1.0f;
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetBulletTexture(Barrage_No, Bullet_No);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBulletTexture(int Barrage_No, int Bullet_No)
{
	BULLETTYPE *BulletType = GetBulletType(Barrage[Barrage_No].Bullet[Bullet_No].Type);
	// 弾の色
	int x = Barrage[Barrage_No].Bullet[Bullet_No].Color;
	float sizeX = 1.0f / BulletType->Devide_X;

	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[0].tex = D3DXVECTOR2((float)x * sizeX, 0.0f);
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[1].tex = D3DXVECTOR2((float)x * sizeX + sizeX, 0.0f);
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[2].tex = D3DXVECTOR2((float)x * sizeX, 1.0f);
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[3].tex = D3DXVECTOR2((float)x * sizeX + sizeX, 1.0f);

	return;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBulletVertex(int Barrage_No, int Bullet_No)
{
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[0].vtx.x =
		Barrage[Barrage_No].Bullet[Bullet_No].Pos.x - cosf(Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle + Barrage[Barrage_No].Bullet[Bullet_No].Rot.z) * Barrage[Barrage_No].Bullet[Bullet_No].Radius;
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[0].vtx.y =
		Barrage[Barrage_No].Bullet[Bullet_No].Pos.y - sinf(Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle + Barrage[Barrage_No].Bullet[Bullet_No].Rot.z) * Barrage[Barrage_No].Bullet[Bullet_No].Radius;
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[1].vtx.x =
		Barrage[Barrage_No].Bullet[Bullet_No].Pos.x + cosf(Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle - Barrage[Barrage_No].Bullet[Bullet_No].Rot.z) * Barrage[Barrage_No].Bullet[Bullet_No].Radius;
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[1].vtx.y =
		Barrage[Barrage_No].Bullet[Bullet_No].Pos.y - sinf(Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle - Barrage[Barrage_No].Bullet[Bullet_No].Rot.z) * Barrage[Barrage_No].Bullet[Bullet_No].Radius;
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[2].vtx.x =
		Barrage[Barrage_No].Bullet[Bullet_No].Pos.x - cosf(Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle - Barrage[Barrage_No].Bullet[Bullet_No].Rot.z) * Barrage[Barrage_No].Bullet[Bullet_No].Radius;
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[2].vtx.y =
		Barrage[Barrage_No].Bullet[Bullet_No].Pos.y + sinf(Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle - Barrage[Barrage_No].Bullet[Bullet_No].Rot.z) * Barrage[Barrage_No].Bullet[Bullet_No].Radius;
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[3].vtx.x =
		Barrage[Barrage_No].Bullet[Bullet_No].Pos.x + cosf(Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle + Barrage[Barrage_No].Bullet[Bullet_No].Rot.z) * Barrage[Barrage_No].Bullet[Bullet_No].Radius;
	Barrage[Barrage_No].Bullet[Bullet_No].VertexWk[3].vtx.y =
		Barrage[Barrage_No].Bullet[Bullet_No].Pos.y + sinf(Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle + Barrage[Barrage_No].Bullet[Bullet_No].Rot.z) * Barrage[Barrage_No].Bullet[Bullet_No].Radius;

	return;
}

//=============================================================================
// エネミー弾幕の設置
//=============================================================================
void SetBarrage(int Enemy_No)
{
	int Barrage_No = 0;
	int Bullet_No = 0;
	ENEMY *Enemy = GetEnemy(Enemy_No);

	for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++)
	{
		// 未使用の弾幕データがあれば
		if (Barrage[Barrage_No].Phase == BarrageOver)
		{
			// 弾幕発射開始
			Barrage[Barrage_No].Phase = BarrageStart;
			// 角度の初期化
			Barrage[Barrage_No].BaseAngle = 0.0f;
			// 弾幕の種類
			Barrage[Barrage_No].Type = Enemy->BarrageType;
			// どの敵から発射されたものか。
			Barrage[Barrage_No].Attacker_No = Enemy_No;
			// 弾幕のカウント
			Barrage[Barrage_No].Count = 0;
			// 弾の初期化
			for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
			{
				Barrage[Barrage_No].Bullet[Bullet_No].Use = false;
			}
			break;
		}
	}

	return;
}

//=============================================================================
// エネミー弾幕の情報を取得する
//=============================================================================
BARRAGE *GetBarrage(int Barrage_No)
{
	return &Barrage[Barrage_No];
}

//=============================================================================
// 弾幕の移動パターン
//=============================================================================
void BarragePattern(int Barrage_No, int Papptern_No)
{
	int i = 0;
	int Bullet_No = 0;
	int Shot_Max = 60;
	int Count = Barrage[Barrage_No].Count;
	PLAYER *Player = GetPlayer(0);
	// どのエネミーからの攻撃を記録する
	ENEMY *Enemy = GetEnemy(Barrage[Barrage_No].Attacker_No);
	// 発射する弾の種類
	BULLETTYPE *BulletType = GetBulletType(Enemy->BulletType);

	switch (Papptern_No)
	{
		// 弾幕パターン0
		// 自機に向かって直線発射(常に自機狙い)
	case 0:
		if (Count % 150 <= 120 && Count % 30 == 0)
		{
			if (Barrage[Barrage_No].Phase != EnemyDisappear)
			{
				// どのエネミーからの攻撃を記録する
				Enemy = GetEnemy(Barrage[Barrage_No].Attacker_No);
				// 発射する弾の種類
				BulletType = GetBulletType(Enemy->BulletType);
				// 発射音
				PlaySound(SE_BulletShot_01, E_DS8_FLAG_NONE, true);
				for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
				{
					// 使用されていない弾があれば
					if (Barrage[Barrage_No].Bullet[Bullet_No].Use == false)
					{
						// 弾の再初期化処理
						Barrage[Barrage_No].Bullet[Bullet_No].Use = true;
						Barrage[Barrage_No].Bullet[Bullet_No].Type = Enemy->BulletType;
						Barrage[Barrage_No].Bullet[Bullet_No].Pos = Enemy->Pos;
						Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
						Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle = AngleCalculate(Enemy->Pos, Player->Pos);
						Barrage[Barrage_No].Bullet[Bullet_No].Rot.z =
							Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
						Barrage[Barrage_No].Bullet[Bullet_No].Radius = BulletType->Radius;
						Barrage[Barrage_No].Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
						Barrage[Barrage_No].Bullet[Bullet_No].Color = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Count = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Speed = Enemy->BulletSpeed;
						break;
					}
				}
			}
		}
		break;
		// 弾幕パターン1
		// 自機に向かって直線発射(一発目の角度記憶)
	case 1:
		if (Count % 150 <= 120 && Count % 20 == 0)
		{
			if (Barrage[Barrage_No].Phase != EnemyDisappear)
			{
				// 最初のカウントにエネミーとプレイヤーの角度を記録
				if (Count == 0)
				{
					Barrage[Barrage_No].BaseAngle = AngleCalculate(Enemy->Pos, Player->Pos);
				}
				// どのエネミーからの攻撃を記録する
				Enemy = GetEnemy(Barrage[Barrage_No].Attacker_No);
				// 発射する弾の種類
				BulletType = GetBulletType(Enemy->BulletType);
				// 発射音
				PlaySound(SE_BulletShot_01, E_DS8_FLAG_NONE, true);
				for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
				{
					// 使用されていない弾があれば
					if (Barrage[Barrage_No].Bullet[Bullet_No].Use == false)
					{
						// 弾の再初期化処理
						Barrage[Barrage_No].Bullet[Bullet_No].Use = true;
						Barrage[Barrage_No].Bullet[Bullet_No].Type = Enemy->BulletType;
						Barrage[Barrage_No].Bullet[Bullet_No].Pos = Enemy->Pos;
						Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
						Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle = Barrage[Barrage_No].BaseAngle;
						Barrage[Barrage_No].Bullet[Bullet_No].Rot.z =
							Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
						Barrage[Barrage_No].Bullet[Bullet_No].Radius = BulletType->Radius;
						Barrage[Barrage_No].Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
						Barrage[Barrage_No].Bullet[Bullet_No].Color = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Count = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Speed = Enemy->BulletSpeed;
						break;
					}
				}
			}
		}
		break;
		// 弾幕パターン2
		// 100カウント中に10発、自機に向かってスピード変化直線発射
	case 2:
		if (Count <= 100 && Count % 10 == 0)
		{
			if (Barrage[Barrage_No].Phase != EnemyDisappear)
			{
				// どのエネミーからの攻撃を記録する
				Enemy = GetEnemy(Barrage[Barrage_No].Attacker_No);
				// 発射する弾の種類
				BulletType = GetBulletType(Enemy->BulletType);
				// 発射音
				if (IsPlaying(SE_BulletShot_03) == false)
				{
					PlaySound(SE_BulletShot_03, E_DS8_FLAG_NONE, true);
				}
				for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
				{
					// 使用されていない弾があれば
					if (Barrage[Barrage_No].Bullet[Bullet_No].Use == false)
					{
						// 弾の再初期化処理
						Barrage[Barrage_No].Bullet[Bullet_No].Use = true;
						Barrage[Barrage_No].Bullet[Bullet_No].Type = Enemy->BulletType;
						Barrage[Barrage_No].Bullet[Bullet_No].Pos = Enemy->Pos;
						Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
						Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle = AngleCalculate(Enemy->Pos, Player->Pos);
						Barrage[Barrage_No].Bullet[Bullet_No].Rot.z =
							Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
						Barrage[Barrage_No].Bullet[Bullet_No].Radius = BulletType->Radius;
						Barrage[Barrage_No].Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
						Barrage[Barrage_No].Bullet[Bullet_No].Color = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Count = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Speed = Enemy->BulletSpeed + 5.0f / 100.0f * Count;
						break;
					}
				}
			}
		}
		break;
		// 弾幕パターン3
		// 20フレイムに1回ずつ円形発射、一つの円はShot_Max発の弾がある
	case 3:
		if (Count % 20 == 0)
		{
			if (Barrage[Barrage_No].Phase != EnemyDisappear)
			{
				// 一つの円は20発ある
				Shot_Max = 20;
				// エネミーとプレイヤーの角度を記録する
				Barrage[Barrage_No].BaseAngle = AngleCalculate(Enemy->Pos, Player->Pos);
				// どのエネミーからの攻撃を記録する
				Enemy = GetEnemy(Barrage[Barrage_No].Attacker_No);
				// 発射する弾の種類
				BulletType = GetBulletType(Enemy->BulletType);
				// 発射音
				PlaySound(SE_BulletShot_01, E_DS8_FLAG_NONE, true);
				for (i = 0; i < Shot_Max; i++)
				{
					for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
					{
						// 使用されていない弾があれば
						if (Barrage[Barrage_No].Bullet[Bullet_No].Use == false)
						{
							// 弾の再初期化処理
							Barrage[Barrage_No].Bullet[Bullet_No].Use = true;
							Barrage[Barrage_No].Bullet[Bullet_No].Type = Enemy->BulletType;
							Barrage[Barrage_No].Bullet[Bullet_No].Pos = Enemy->Pos;
							Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							// 角度 + 360度(2PI) / 20発 * 何発目
							Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle =
								Barrage[Barrage_No].BaseAngle + 2 * PI / Shot_Max * i;
							Barrage[Barrage_No].Bullet[Bullet_No].Rot.z =
								Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Barrage[Barrage_No].Bullet[Bullet_No].Radius = BulletType->Radius;
							Barrage[Barrage_No].Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Barrage[Barrage_No].Bullet[Bullet_No].Color = 0;
							Barrage[Barrage_No].Bullet[Bullet_No].Count = 0;
							Barrage[Barrage_No].Bullet[Bullet_No].Speed = Enemy->BulletSpeed;
							break;
						}
					}
				}
			}
		}
		break;
		// 弾幕パターン4
		// 2フレイムに1回ずつばらまきショット
	case 4:
		if (Count <= 180 && Count % 2 == 0)
		{
			if (Barrage[Barrage_No].Phase != EnemyDisappear)
			{
				// どのエネミーからの攻撃を記録する
				Enemy = GetEnemy(Barrage[Barrage_No].Attacker_No);
				// 発射する弾の種類
				BulletType = GetBulletType(Enemy->BulletType);
				// 発射音
				PlaySound(SE_BulletShot_02, E_DS8_FLAG_NONE, true);
				for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
				{
					// 使用されていない弾があれば
					if (Barrage[Barrage_No].Bullet[Bullet_No].Use == false)
					{
						// 弾の再初期化処理
						Barrage[Barrage_No].Bullet[Bullet_No].Use = true;
						Barrage[Barrage_No].Bullet[Bullet_No].Type = Enemy->BulletType;
						Barrage[Barrage_No].Bullet[Bullet_No].Pos = Enemy->Pos;
						Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
						Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle =
							// 角度 + (-45度 ~ 45度)
							AngleCalculate(Enemy->Pos, Player->Pos) + RandRadian(PI / 4);
						Barrage[Barrage_No].Bullet[Bullet_No].Rot.z =
							Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
						Barrage[Barrage_No].Bullet[Bullet_No].Radius = BulletType->Radius;
						Barrage[Barrage_No].Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
						Barrage[Barrage_No].Bullet[Bullet_No].Color = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Count = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Speed = Enemy->BulletSpeed + ((float)rand() / RAND_MAX);
						break;
					}
				}
			}
		}
		break;
		// 弾幕パターン5
		// 2フレイムに1回ずつばらまきショット(減速あり)
	case 5:
		if (Count <= 180 && Count % 2 == 0)
		{
			if (Barrage[Barrage_No].Phase != EnemyDisappear)
			{
				// どのエネミーからの攻撃を記録する
				Enemy = GetEnemy(Barrage[Barrage_No].Attacker_No);
				// 発射する弾の種類
				BulletType = GetBulletType(Enemy->BulletType);
				// 発射音
				PlaySound(SE_BulletShot_02, E_DS8_FLAG_NONE, true);
				for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
				{
					// 使用されていない弾があれば
					if (Barrage[Barrage_No].Bullet[Bullet_No].Use == false)
					{
						// 弾の再初期化処理
						Barrage[Barrage_No].Bullet[Bullet_No].Use = true;
						Barrage[Barrage_No].Bullet[Bullet_No].Type = Enemy->BulletType;
						Barrage[Barrage_No].Bullet[Bullet_No].Pos = Enemy->Pos;
						Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
						Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle =
							AngleCalculate(Enemy->Pos, Player->Pos) + RandRadian(PI / 4);
						Barrage[Barrage_No].Bullet[Bullet_No].Rot.z =
							Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
						Barrage[Barrage_No].Bullet[Bullet_No].Radius = BulletType->Radius;
						Barrage[Barrage_No].Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
						Barrage[Barrage_No].Bullet[Bullet_No].Color = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Count = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Speed = Enemy->BulletSpeed + ((float)rand() / RAND_MAX);
						break;
					}
				}
			}
		}
		for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
		{
			if (Barrage[Barrage_No].Bullet[Bullet_No].Use == true)
			{
				// 速度が1.5より大きい弾があれば
				if (Barrage[Barrage_No].Bullet[Bullet_No].Speed > 1.5f)
				{
					// 減速
					Barrage[Barrage_No].Bullet[Bullet_No].Speed -= 0.06f;
				}
			}
		}
		break;
		// 弾幕パターン6
		// 自機に向かって直線発射(一発)
	case 6:
		if (Count % 60 == 0)
		{
			if (Barrage[Barrage_No].Phase != EnemyDisappear)
			{
				// どのエネミーからの攻撃を記録する
				Enemy = GetEnemy(Barrage[Barrage_No].Attacker_No);
				// 発射する弾の種類
				BulletType = GetBulletType(Enemy->BulletType);
				// 発射音
				PlaySound(SE_BulletShot_01, E_DS8_FLAG_NONE, true);
				for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
				{
					// 使用されていない弾があれば
					if (Barrage[Barrage_No].Bullet[Bullet_No].Use == false)
					{
						// 弾の再初期化処理
						Barrage[Barrage_No].Bullet[Bullet_No].Use = true;
						Barrage[Barrage_No].Bullet[Bullet_No].Type = Enemy->BulletType;
						Barrage[Barrage_No].Bullet[Bullet_No].Pos = Enemy->Pos;
						Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
						Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle = AngleCalculate(Enemy->Pos, Player->Pos);
						Barrage[Barrage_No].Bullet[Bullet_No].Rot.z =
							Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
						Barrage[Barrage_No].Bullet[Bullet_No].Radius = BulletType->Radius;
						Barrage[Barrage_No].Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
						Barrage[Barrage_No].Bullet[Bullet_No].Color = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Count = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Speed = Enemy->BulletSpeed;
						break;
					}
				}
			}
		}
		break;
		// 弾幕パターン7
		// 回転発射(10度逸らす)
	case 7:
		if (Count % 10 == 0)
		{
			if (Barrage[Barrage_No].Phase != EnemyDisappear)
			{
				// どのエネミーからの攻撃を記録する
				Enemy = GetEnemy(Barrage[Barrage_No].Attacker_No);
				// 発射する弾の種類
				BulletType = GetBulletType(Enemy->BulletType);
				// 発射音
				PlaySound(SE_BulletShot_01, E_DS8_FLAG_NONE, true);
				for (i = 0; i < 4; i++)
				{
					for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
					{
						// 使用されていない弾があれば
						if (Barrage[Barrage_No].Bullet[Bullet_No].Use == false)
						{
							// 弾の再初期化処理
							Barrage[Barrage_No].Bullet[Bullet_No].Use = true;
							Barrage[Barrage_No].Bullet[Bullet_No].Type = Enemy->BulletType;
							Barrage[Barrage_No].Bullet[Bullet_No].Pos = Enemy->Pos;
							Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle =
								Barrage[Barrage_No].BaseAngle + DegreeToRadian(90) * i;
							Barrage[Barrage_No].Bullet[Bullet_No].Rot.z =
								Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Barrage[Barrage_No].Bullet[Bullet_No].Radius = BulletType->Radius;
							Barrage[Barrage_No].Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Barrage[Barrage_No].Bullet[Bullet_No].Color = 0;
							Barrage[Barrage_No].Bullet[Bullet_No].Count = 0;
							Barrage[Barrage_No].Bullet[Bullet_No].Speed = Enemy->BulletSpeed;
							break;
						}
					}
				}
				// 10度の角度を加算
				Barrage[Barrage_No].BaseAngle += DegreeToRadian(10);
			}
		}
		break;
		// 弾幕パターン8
		// 四つ方向発射、角度変化あり(0度基準 <-> 45度基準)
	case 8:
		if (Count % 30 == 0)
		{
			if (Barrage[Barrage_No].Phase != EnemyDisappear)
			{
				// どのエネミーからの攻撃を記録する
				Enemy = GetEnemy(Barrage[Barrage_No].Attacker_No);
				// 発射する弾の種類
				BulletType = GetBulletType(Enemy->BulletType);
				// 発射音
				PlaySound(SE_BulletShot_01, E_DS8_FLAG_NONE, true);
				for (i = 0; i < 4; i++)
				{
					for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
					{
						// 使用されていない弾があれば
						if (Barrage[Barrage_No].Bullet[Bullet_No].Use == false)
						{
							// 弾の再初期化処理
							Barrage[Barrage_No].Bullet[Bullet_No].Use = true;
							Barrage[Barrage_No].Bullet[Bullet_No].Type = Enemy->BulletType;
							Barrage[Barrage_No].Bullet[Bullet_No].Pos = Enemy->Pos;
							Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle =
								Barrage[Barrage_No].BaseAngle + DegreeToRadian(90) * i;
							Barrage[Barrage_No].Bullet[Bullet_No].Rot.z =
								Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Barrage[Barrage_No].Bullet[Bullet_No].Radius = BulletType->Radius;
							Barrage[Barrage_No].Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Barrage[Barrage_No].Bullet[Bullet_No].Color = 0;
							Barrage[Barrage_No].Bullet[Bullet_No].Count = 0;
							Barrage[Barrage_No].Bullet[Bullet_No].Speed = Enemy->BulletSpeed;
							break;
						}
					}
				}
				// 0度 <-> 45度変換
				Barrage[Barrage_No].BaseAngle += DegreeToRadian(45);
			}
		}
		break;
		// 弾幕パターン9
		// 四つ方向発射
	case 9:
		if (Count % 30 == 0)
		{
			if (Barrage[Barrage_No].Phase != EnemyDisappear)
			{
				// どのエネミーからの攻撃を記録する
				Enemy = GetEnemy(Barrage[Barrage_No].Attacker_No);
				// 発射する弾の種類
				BulletType = GetBulletType(Enemy->BulletType);
				// 発射音
				PlaySound(SE_BulletShot_04, E_DS8_FLAG_NONE, true);
				for (i = 0; i < 4; i++)
				{
					for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
					{
						// 使用されていない弾があれば
						if (Barrage[Barrage_No].Bullet[Bullet_No].Use == false)
						{
							// 弾の再初期化処理
							Barrage[Barrage_No].Bullet[Bullet_No].Use = true;
							Barrage[Barrage_No].Bullet[Bullet_No].Type = Enemy->BulletType;
							Barrage[Barrage_No].Bullet[Bullet_No].Pos = Enemy->Pos;
							Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle =
								Barrage[Barrage_No].BaseAngle + DegreeToRadian(90) * i;
							Barrage[Barrage_No].Bullet[Bullet_No].Rot.z =
								Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Barrage[Barrage_No].Bullet[Bullet_No].Radius = BulletType->Radius;
							Barrage[Barrage_No].Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Barrage[Barrage_No].Bullet[Bullet_No].Color = 0;
							Barrage[Barrage_No].Bullet[Bullet_No].Count = 0;
							Barrage[Barrage_No].Bullet[Bullet_No].Speed = Enemy->BulletSpeed;
							break;
						}
					}
				}
			}
		}
		break;
		// 弾幕パターン10
		// 螺旋発射
	case 10:
		if (Barrage[Barrage_No].Phase != EnemyDisappear)
		{
			// どのエネミーからの攻撃を記録する
			Enemy = GetEnemy(Barrage[Barrage_No].Attacker_No);
			// 発射する弾の種類
			BulletType = GetBulletType(Enemy->BulletType);
			// 発射音
			PlaySound(SE_BulletShot_02, E_DS8_FLAG_NONE, true);
			for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
			{
				// 使用されていない弾があれば
				if (Barrage[Barrage_No].Bullet[Bullet_No].Use == false)
				{
					// 弾の再初期化処理
					Barrage[Barrage_No].Bullet[Bullet_No].Use = true;
					Barrage[Barrage_No].Bullet[Bullet_No].Type = Enemy->BulletType;
					Barrage[Barrage_No].Bullet[Bullet_No].Pos = Enemy->Pos;
					Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
					Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle =
						Barrage[Barrage_No].BaseAngle + DegreeToRadian(180);
					Barrage[Barrage_No].Bullet[Bullet_No].Rot.z =
						Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
					Barrage[Barrage_No].Bullet[Bullet_No].Radius = BulletType->Radius;
					Barrage[Barrage_No].Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
					Barrage[Barrage_No].Bullet[Bullet_No].Color = 0;
					Barrage[Barrage_No].Bullet[Bullet_No].Count = 0;
					Barrage[Barrage_No].Bullet[Bullet_No].Speed = Enemy->BulletSpeed;
					break;
				}
			}
			// 12度の角度を加算
			Barrage[Barrage_No].BaseAngle += DegreeToRadian(12);
		}
		// 30カウントを経ったら、発射角度は15度を加算
		if (Barrage[Barrage_No].Count % 30 == 0)
		{
			Barrage[Barrage_No].BaseAngle += DegreeToRadian(15);
		}
		break;
		// テスト用
	case 99:
		if (Count % 60 == 0)
		{
			if (Barrage[Barrage_No].Phase != EnemyDisappear)
			{
				// どのエネミーからの攻撃を記録する
				Enemy = GetEnemy(Barrage[Barrage_No].Attacker_No);
				// 発射する弾の種類
				BulletType = GetBulletType(Enemy->BulletType);
				for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
				{
					// 使用されていない弾があれば
					if (Barrage[Barrage_No].Bullet[Bullet_No].Use == false)
					{
						// 弾の再初期化処理
						Barrage[Barrage_No].Bullet[Bullet_No].Use = true;
						Barrage[Barrage_No].Bullet[Bullet_No].Type = Enemy->BulletType;
						Barrage[Barrage_No].Bullet[Bullet_No].Pos = Enemy->Pos;
						Barrage[Barrage_No].Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
						Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle = AngleCalculate(Enemy->Pos, Player->Pos);
						Barrage[Barrage_No].Bullet[Bullet_No].Rot.z =
							Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
						Barrage[Barrage_No].Bullet[Bullet_No].Radius = BulletType->Radius;
						Barrage[Barrage_No].Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
						Barrage[Barrage_No].Bullet[Bullet_No].Color = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Count = 0;
						Barrage[Barrage_No].Bullet[Bullet_No].Speed = Enemy->BulletSpeed;

#ifdef _DEBUG
						SetEnemyBulletHC(Barrage[Barrage_No].Bullet[Bullet_No].Pos,
							Barrage[Barrage_No].Bullet[Bullet_No].HitRadius,
							Barrage_No, Bullet_No, EnemyBulletHC);
#endif

						break;
					}
				}
			}
		}
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// 弾の移動計算
//=============================================================================
void BulletCalculate(int Barrage_No)
{
	int Bullet_No = 0;
	int Enemy_No = 0;
	ENEMY *Enemy = GetEnemy(Enemy_No);
	BULLETTYPE *BulletType = GetBulletType(Enemy->BulletType);

	Enemy_No = Barrage[Barrage_No].Attacker_No;
	Enemy = GetEnemy(Enemy_No);
	// 敵が倒されたら
	if (Enemy->Exist == false)
	{
		// それ以上ショットを登録しないフラグに変える
		// エネミー消えた、でも残った弾そのまま移動
		Barrage[Barrage_No].Phase = EnemyDisappear;
	}

	for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
	{
		// Barrage_Noの弾幕データの弾を計算
		// その弾が登録されていたら
		if (Barrage[Barrage_No].Bullet[Bullet_No].Use == true)
		{
			// 1フレイム前の座標を記録する、当たり判定用
			Barrage[Barrage_No].Bullet[Bullet_No].PreviousPos = Barrage[Barrage_No].Bullet[Bullet_No].Pos;
			// 弾の移動計算
			Barrage[Barrage_No].Bullet[Bullet_No].Pos.x +=
				cosf(Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle) * Barrage[Barrage_No].Bullet[Bullet_No].Speed;
			Barrage[Barrage_No].Bullet[Bullet_No].Pos.y +=
				sinf(Barrage[Barrage_No].Bullet[Bullet_No].MoveAngle) * Barrage[Barrage_No].Bullet[Bullet_No].Speed;
			//　カウント++
			Barrage[Barrage_No].Bullet[Bullet_No].Count++;
			// 頂点座標の設定
			SetBulletTexture(Barrage_No, Bullet_No);
			// テクスチャ座標の設定
			SetBulletVertex(Barrage_No, Bullet_No);
			// 画面から外れたら
			if (CheckRange(OutOfRange, Barrage[Barrage_No].Bullet[Bullet_No].Pos,
				BulletType->Width, BulletType->Height) == true)
			{
				// 弾を消す
				Barrage[Barrage_No].Bullet[Bullet_No].Use = false;
			}
		}
	}

	// 現在表示中の弾が一つでもあるかどうか調べる
	for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
	{
		if (Barrage[Barrage_No].Bullet[Bullet_No].Use == true)
		{
			// 表示中の弾があったら戻る
			return;
		}
	}

	// 表示中の弾がなければ、弾幕の使用終了
	if (Enemy->Exist == false)
	{
		Barrage[Barrage_No].Phase = BarrageOver;
	}

	return;
}
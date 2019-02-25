//=============================================================================
//
// ボム処理 [Bomb.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Bomb.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Barrage.h"
#include "Sound.h"
#include "DebugTest.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ボムAタイプの拡大速度
#define ExtendSpeed_Phase1	(10.0f)
#define ExtendSpeed_Phase2	(200.0f)
// ボムAタイプの消失カウント
#define BombDisappearCount	(100)
// ロックオンマークの拡大係数
#define LockOnZoomRate		(2.0f)
// ボムBタイプの弾の移動速度
#define BombBulletSpeed		(8.0f)

// ボムAタイプ状態
enum Bomb_A_Phase
{
	ShotBomb,		// ボム発射
	BombExtending,	// 拡大中
	ExtendOver,		// 拡大終了
};

// ボムBタイプの状態
enum Bomb_B_Phase
{
	TargetLockOn,	// ターゲットをロックオン
	Shot,			// 弾発射
	ShotOver,		// ボム終了
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeBombVertex(void);
HRESULT MakeLockOnVertex(int LockOn_No);
static HRESULT MakeBulletVertex(int Bullet_No);
// 頂点座標の設定
void SetBombVertex(void);
void SetLockOnVertex(int LockOn_No);
static void SetBulletVertex(int Bullet_No);
// テクスチャ座標の設定
void SetBombTexture(void);
void SetLockOnTexture(int LockOn_No);
static void SetBulletTexture(int Bullet_No);
// ボムAタイプの透明度を設置する
void SetBombDiffuse(int Alpha);
// ボムBタイプの弾を発射する
void ShotBombBullet(void);
// ボムBタイプの弾が命中する時の衝撃波を描画する
void DrawImpactWave(D3DXVECTOR3 Center, float Radius, D3DCOLOR Color);
// ボムAタイプの拡大範囲を確認する
void CheckVertex(VERTEX_2D *Vtx);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 ShotBombTexture = NULL;
LPDIRECT3DTEXTURE9 BombExtendTexture = NULL;
LPDIRECT3DTEXTURE9 WhiteScreenTexture = NULL;
LPDIRECT3DTEXTURE9 LockOnTexture = NULL;
LPDIRECT3DTEXTURE9 BombBulletTexture = NULL;
// ボムAタイプの拡大範囲を表示する頂点
VERTEX_2D BombExtendPixel[TextureDevideNum][TextureDevideNum][Num_Vertex];
// テクスチャ半径
float LockOnRadius = 0.0f;
float ShotBombRadius = 0.0f;
float BombExtendRadius = 0.0f;
// 中心点と四頂点の成す角
float LockOnBaseAngle = 0.0f;
float ShotBombBaseAngle = 0.0f;
float BombExtendBaseAngle = 0.0f;
// ボム構造体
BOMB Bomb;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBomb(int InitState)
{
	int Bullet_No = 0;
	D3DXVECTOR2 temp = D3DXVECTOR2(0.0f, 0.0f);

	Bomb.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Bomb.ShakePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Bomb.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Bomb.Radius = 0.0f;
	Bomb.HitRadius = 0.0f;
	Bomb.BaseAngle = 0.0f;
	Bomb.LockOnNum = 0;
	Bomb.AnimPattern = 0;
	Bomb.Count = 0;
	Bomb.Alpha = 255;
	Bomb.Phase = ShotBomb;
	Bomb.Use = false;

	// 頂点情報の作成
	MakeBombVertex();

	for (int LockOn_No = 0; LockOn_No < BombBullet_Max; LockOn_No++)
	{
		Bomb.LockOn[LockOn_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.LockOn[LockOn_No].Radius = 0.0f;
		Bomb.LockOn[LockOn_No].BaseAngle = 0.0f;
		Bomb.LockOn[LockOn_No].Target_No = 0;
		Bomb.LockOn[LockOn_No].Use = false;

		// 頂点情報の作成
		MakeLockOnVertex(LockOn_No);
	}

	for (Bullet_No = 0; Bullet_No < BombBullet_Max; Bullet_No++)
	{
		Bomb.Bullet[Bullet_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.Bullet[Bullet_No].PreviousPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.Bullet[Bullet_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.Bullet[Bullet_No].Radius = 0.0f;
		Bomb.Bullet[Bullet_No].HitRadius = 0.0f;
		Bomb.Bullet[Bullet_No].BaseAngle = 0.0f;
		Bomb.Bullet[Bullet_No].MoveAngle = 0.0f;
		Bomb.Bullet[Bullet_No].Speed = 0.0f;
		Bomb.Bullet[Bullet_No].Damage = 0;
		Bomb.Bullet[Bullet_No].Type = 0;
		Bomb.Bullet[Bullet_No].Count = 0;
		Bomb.Bullet[Bullet_No].Color = 0;
		Bomb.Bullet[Bullet_No].State = 0;
		Bomb.Bullet[Bullet_No].Use = false;
		Bomb.Bullet[Bullet_No].DelayDisappear = false;
		Bomb.BulletTarget[Bullet_No] = 0;
		Bomb.ExplosionRadius[Bullet_No] = 0.0f;

		// 頂点情報の作成
		MakeBulletVertex(Bullet_No);
	}

	temp = D3DXVECTOR2(Texture_LockOn_Width / 2, Texture_LockOn_Height / 2);
	LockOnRadius = D3DXVec2Length(&temp);
	LockOnBaseAngle = atan2f(Texture_LockOn_Height, Texture_LockOn_Width);

	temp = D3DXVECTOR2(Texture_ShotBomb_Width / 2, Texture_ShotBomb_Height / 2);
	ShotBombRadius = D3DXVec2Length(&temp);
	ShotBombBaseAngle = atan2f(Texture_ShotBomb_Height, Texture_ShotBomb_Width);

	temp = D3DXVECTOR2(Texture_BombExtend_Width / 2, Texture_BombExtend_Height / 2);
	BombExtendRadius = D3DXVec2Length(&temp);
	BombExtendBaseAngle = atan2f(Texture_BombExtend_Height, Texture_BombExtend_Width);

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_ShotBomb, &ShotBombTexture, "Bomb") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BombExtend, &BombExtendTexture, "Bomb") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_WhiteScreen, &WhiteScreenTexture, "Bomb") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_LockOn, &LockOnTexture, "Bomb") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BombBullet, &BombBulletTexture, "Bomb") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBomb(void)
{
	SafeRelease(ShotBombTexture);
	SafeRelease(BombExtendTexture);
	SafeRelease(WhiteScreenTexture);
	SafeRelease(LockOnTexture);
	SafeRelease(BombBulletTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBomb(void)
{
	int i = 0, j = 0;
	int LockOn_No = 0;
	int Bullet_No = 0;
	// 今終了した弾の数
	int BulletOverNum = 0;
	PLAYER *Player = GetPlayer(0);
	ENEMY *Enemy = GetEnemy(0);
	BOSS *Boss = GetBoss();
	BULLETTYPE *BulletType = GetBulletType(BombBulletType);

	if (Bomb.Use == true)
	{
		Bomb.Count++;
		// Aタイプ
		if (Player->Type == Player_A)
		{
			// 発射中、拡大がまだ始まらない
			if (Bomb.Phase == ShotBomb)
			{
				if (IsPlaying(SE_ShotBomb) == false)
				{
					PlaySound(SE_ShotBomb, E_DS8_FLAG_NONE, true);
				}
				if (Bomb.Count % (30 / Texture_ShotBomb_Divide_X) == 0)
				{
					Bomb.AnimPattern = (Bomb.AnimPattern + 1) % Texture_ShotBomb_Divide_X;
				}
				// 発射終わる、ボム拡大開始
				if (Bomb.Count > 30)
				{
					PlaySound(SE_BombExtend, E_DS8_FLAG_NONE, true);
					Bomb.Phase = BombExtending;
					Bomb.Pos.x += 16.0f;
					Bomb.Radius = BombExtendRadius;
					Bomb.HitRadius = BombExtendRadius / Root2;
					Bomb.BaseAngle = BombExtendBaseAngle;
					Bomb.Alpha = 128;
					Bomb.Count = 0;
#ifdef _DEBUG
					SetBombHC(Bomb.Pos, Bomb.HitRadius, BombHC);
#endif
				}
			}
			// ボム拡大中
			else if (Bomb.Phase == BombExtending)
			{
				if (Bomb.Count % 4 == 0)
				{
					if (Bomb.Count <= 60)
					{
						// 45度は 1 : 1 : Root2、だから斜辺たす"1"の時、底辺はたす"1 / Root2"
						Bomb.Radius += ExtendSpeed_Phase1;
						Bomb.HitRadius += ExtendSpeed_Phase1 / Root2;
						Bomb.Alpha += 5;
					}
					else
					{
						Bomb.Radius += ExtendSpeed_Phase2;
						Bomb.HitRadius += ExtendSpeed_Phase2 / Root2;
						Bomb.Alpha += 10;
						if (Bomb.Alpha >= 200)
						{
							Bomb.Alpha = 200;
						}
					}
				}
			}
			// ボム拡大終了、画面揺れる
			else if (Bomb.Phase == ExtendOver)
			{
				if (IsPlaying(SE_BombExplosion) == false)
				{
					PlaySound(SE_BombExplosion, E_DS8_FLAG_NONE, true);
				}
				Bomb.ShakePos.x = (float)(rand() % (30 + 1) - 15);
				Bomb.ShakePos.y = (float)(rand() % (30 + 1) - 15);
				if (Bomb.Count > BombDisappearCount)
				{
					Bomb.Use = false;
					Player->InBomb = false;
					if (Boss->Exist == true)
					{
						Boss->BombDamage = false;
					}
				}
			}
			// テクスチャ更新処理
			SetBombTexture();
			SetBombDiffuse(Bomb.Alpha);
			SetBombVertex();
		}
		// Bタイプ
		else if (Player->Type == Player_B)
		{
			// ロックオン設置
			for (LockOn_No = 0; LockOn_No < BombBullet_Max; LockOn_No++)
			{
				if (Bomb.LockOn[LockOn_No].Use == true)
				{
					Enemy = GetEnemy(Bomb.LockOn[LockOn_No].Target_No);

					// エネミー(ボス)の座標と一緒に移動
					if (Enemy->Exist == true)
					{
						Bomb.LockOn[LockOn_No].Pos = Enemy->Pos;
					}
					else if (Boss->Exist == true)
					{
						Bomb.LockOn[LockOn_No].Pos = Boss->Pos;
					}
					else
					{
						Bomb.LockOn[LockOn_No].Use = false;
						continue;
					}

					// どんどん小さくなる
					if (Bomb.LockOn[LockOn_No].Radius > LockOnRadius)
					{
						Bomb.LockOn[LockOn_No].Radius -= 3.0f;
					}
					else
					{
						Bomb.LockOn[LockOn_No].Radius = LockOnRadius;
					}

					// テクスチャ更新
					SetLockOnTexture(LockOn_No);
					SetLockOnVertex(LockOn_No);
				}
			}

			// ある程度なったら、弾発射
			if (Bomb.Phase == TargetLockOn && Bomb.Count >= 30.0f)
			{
				Bomb.Phase = Shot;
				StopSound(SE_LockOn);
			}
			// 弾発射
			else if (Bomb.Phase == Shot)
			{
				ShotBombBullet();
				PlaySound(SE_ShotBombBullet, E_DS8_FLAG_NONE, true);
				Bomb.Phase = ShotOver;
			}
			// 発射終了、移動計算
			else if (Bomb.Phase == ShotOver)
			{
				for (Bullet_No = 0; Bullet_No < BombBullet_Max; Bullet_No++)
				{
					if (Bomb.Bullet[Bullet_No].Use == true)
					{
						Enemy = GetEnemy(Bomb.BulletTarget[Bullet_No]);

						// 目標存在、弾移動計算
						if (Enemy->Exist == true || Boss->Exist == true && Boss->HP > 0.0f)
						{
							Bomb.Bullet[Bullet_No].PreviousPos = Bomb.Bullet[Bullet_No].Pos;
							// エネミー
							if (Boss->Exist == false)
							{
								Bomb.Bullet[Bullet_No].MoveAngle =
									AngleCalculate(Bomb.Bullet[Bullet_No].Pos, Enemy->Pos);
							}
							// ボス
							else if (Boss->Exist == true)
							{
								Bomb.Bullet[Bullet_No].MoveAngle =
									AngleCalculate(Bomb.Bullet[Bullet_No].Pos, Boss->Pos);
							}
							Bomb.Bullet[Bullet_No].Rot.z = Bomb.Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Bomb.Bullet[Bullet_No].Pos.x +=
								cosf(Bomb.Bullet[Bullet_No].MoveAngle) * Bomb.Bullet[Bullet_No].Speed;
							Bomb.Bullet[Bullet_No].Pos.y +=
								sinf(Bomb.Bullet[Bullet_No].MoveAngle) * Bomb.Bullet[Bullet_No].Speed;
							Bomb.Bullet[Bullet_No].Count++;

							// 移動位置チェック
							if (CheckRange(OutOfRange, Bomb.Bullet[Bullet_No].Pos,
								BulletType->Width, BulletType->Height) == true)
							{
								Bomb.Bullet[Bullet_No].Use = false;
								Bomb.LockOn[Bullet_No].Use = false;
								Bomb.Bullet[Bullet_No].State = BulletDisspappear;
							}

							// テクスチャ更新
							SetBulletTexture(Bullet_No);
							SetBulletVertex(Bullet_No);
						}
						// 目標消える、弾とロックオン消滅
						else
						{
							Bomb.Bullet[Bullet_No].Use = false;
							Bomb.LockOn[Bullet_No].Use = false;
							Bomb.Bullet[Bullet_No].State = BulletDisspappear;
						}
					}
					// 弾命中、衝撃波描く
					else if (Bomb.Bullet[Bullet_No].State == Explosion)
					{
						Bomb.ExplosionRadius[Bullet_No] += 30.0f;
						// ある程度広がったら、終了
						if (Bomb.ExplosionRadius[Bullet_No] >= Screen_Width)
						{
							Bomb.Bullet[Bullet_No].State = Over;
							BulletOverNum++;
						}
					}
					else if (Bomb.Bullet[Bullet_No].Use == false)
					{
						BulletOverNum++;
					}
					// 全部の弾終了 -> ボム終了
					if (BulletOverNum == BombBullet_Max)
					{
						Bomb.Use = false;
						Player->InBomb = false;
						if (Boss->Exist == true)
						{
							Boss->BombDamage = false;
						}
					}
				}
			}
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBomb(void)
{
	int i = 0, j = 0;
	int LockOn_No = 0;
	int Bullet_No = 0;
	LPDIRECT3DDEVICE9 Device = GetDevice();
	PLAYER *Player = GetPlayer(0);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	if (Bomb.Use == true)
	{
		// Aタイプ
		if (Player->Type == Player_A)
		{
			// テクスチャの設定
			if (Bomb.Phase == ShotBomb)
			{
				Device->SetTexture(0, ShotBombTexture);
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Bomb.VertexWk, sizeof(VERTEX_2D));
			}
			else if (Bomb.Phase == BombExtending)
			{
				Device->SetTexture(0, BombExtendTexture);
				for (i = 0; i < TextureDevideNum; i++)
				{
					for (j = 0; j < TextureDevideNum; j++)
					{
						Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BombExtendPixel[i][j], sizeof(VERTEX_2D));
					}
				}
			}
			else if (Bomb.Phase == ExtendOver)
			{
				Device->SetTexture(0, WhiteScreenTexture);
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Bomb.VertexWk, sizeof(VERTEX_2D));
			}
		}
		// Bタイプ
		else if (Player->Type == Player_B)
		{
			for (LockOn_No = 0; LockOn_No < BombBullet_Max; LockOn_No++)
			{
				if (Bomb.LockOn[LockOn_No].Use == true)
				{
					Device->SetTexture(0, LockOnTexture);
					Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Bomb.LockOn[LockOn_No].VertexWk, sizeof(VERTEX_2D));
				}
			}
			if (Bomb.Phase != TargetLockOn)
			{
				for (Bullet_No = 0; Bullet_No < BombBullet_Max; Bullet_No++)
				{
					if (Bomb.Bullet[Bullet_No].Use == true && Bomb.Bullet[Bullet_No].Count != 0)
					{
						Device->SetTexture(0, BombBulletTexture);
						Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Bomb.Bullet[Bullet_No].VertexWk, sizeof(VERTEX_2D));
					}
					else if (Bomb.Bullet[Bullet_No].State == Explosion)
					{
						DrawImpactWave(Bomb.Bullet[Bullet_No].Pos, Bomb.ExplosionRadius[Bullet_No], WHITE(255));
					}
				}
			}
		}
	}


	return;
}

//=============================================================================
// ボムAタイプの設置
//=============================================================================
void SetBomb_A(void)
{
	PLAYER *Player = GetPlayer(0);

	Bomb.Pos.x = Player->Pos.x + 80.0f;
	Bomb.Pos.y = Player->Pos.y;
	Bomb.ShakePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Bomb.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Bomb.Radius = ShotBombRadius;
	Bomb.HitRadius = 0.0f;
	Bomb.BaseAngle = ShotBombBaseAngle;
	Bomb.AnimPattern = 0;
	Bomb.Count = 0;
	Bomb.Alpha = 255;
	Bomb.Phase = ShotBomb;
	Bomb.Use = true;

	StopSound(SE_BombExplosion);

	return;
}

//=============================================================================
// ボムBタイプの設置
//=============================================================================
void SetBomb_B(void)
{
	int LockOn_No = 0;
	int Bullet_No = 0;
	int Enemy_No = 0;
	int ExistEnemyNum = 0;
	PLAYER *Player = GetPlayer(0);
	ENEMY *Enemy = GetEnemy(Enemy_No);
	BOSS *Boss = GetBoss();

	// 弾の初期化
	for (Bullet_No = 0; Bullet_No < BombBullet_Max; Bullet_No++)
	{
		Bomb.Bullet[Bullet_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.Bullet[Bullet_No].PreviousPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.Bullet[Bullet_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.Bullet[Bullet_No].Radius = 0.0f;
		Bomb.Bullet[Bullet_No].HitRadius = 0.0f;
		Bomb.Bullet[Bullet_No].BaseAngle = 0.0f;
		Bomb.Bullet[Bullet_No].MoveAngle = 0.0f;
		Bomb.Bullet[Bullet_No].Speed = 0.0f;
		Bomb.Bullet[Bullet_No].Damage = 0;
		Bomb.Bullet[Bullet_No].Type = 0;
		Bomb.Bullet[Bullet_No].Count = 0;
		Bomb.Bullet[Bullet_No].Color = 0;
		Bomb.Bullet[Bullet_No].State = 0;
		Bomb.Bullet[Bullet_No].Use = false;
		Bomb.Bullet[Bullet_No].DelayDisappear = false;
		Bomb.BulletTarget[Bullet_No] = 0;
		Bomb.ExplosionRadius[Bullet_No] = 0.0f;
	}

	// 照準の初期化
	for (LockOn_No = 0; LockOn_No < BombBullet_Max; LockOn_No++)
	{
		Bomb.LockOn[LockOn_No].Use = false;
		Bomb.LockOn[LockOn_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.LockOn[LockOn_No].Radius = 0.0f;
		Bomb.LockOn[LockOn_No].BaseAngle = 0.0f;
		Bomb.LockOn[LockOn_No].Target_No = 0;
	}

	// エネミー
	if (Boss->Exist == false)
	{
		for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++, Enemy++)
		{
			if (Enemy->Exist == true)
			{
				ExistEnemyNum++;
				for (LockOn_No = 0; LockOn_No < BombBullet_Max; LockOn_No++)
				{
					if (Bomb.LockOn[LockOn_No].Use == false)
					{
						Bomb.LockOn[LockOn_No].Use = true;
						Bomb.LockOn[LockOn_No].Pos = Enemy->Pos;
						Bomb.LockOn[LockOn_No].Radius = LockOnRadius * LockOnZoomRate;
						Bomb.LockOn[LockOn_No].BaseAngle = LockOnBaseAngle;
						Bomb.LockOn[LockOn_No].Target_No = Enemy_No;
						Bomb.BulletTarget[LockOn_No] = Enemy_No;
						Bomb.LockOnNum++;
						break;
					}
				}
			}
		}
	}
	// ボス
	else if (Boss->Exist == true && Boss->HP > 0.0f)
	{
		for (LockOn_No = 0; LockOn_No < BombBullet_Max / 2; LockOn_No++)
		{
			if (Bomb.LockOn[LockOn_No].Use == false)
			{
				Bomb.LockOn[LockOn_No].Use = true;
				Bomb.LockOn[LockOn_No].Pos = Boss->Pos;
				Bomb.LockOn[LockOn_No].Radius = LockOnRadius * LockOnZoomRate;
				Bomb.LockOn[LockOn_No].BaseAngle = LockOnBaseAngle;
				Bomb.LockOn[LockOn_No].Target_No = 0;
				Bomb.LockOnNum++;
			}
		}
	}

	// ロックオンされた数は0より大きい、もしくはボス存在
	if (ExistEnemyNum > 0 || (Boss->Exist == true && Boss->HP > 0))
	{
		Bomb.Phase = TargetLockOn;
		Bomb.Count = 0;
		Bomb.Use = true;
		PlaySound(SE_LockOn, E_DS8_FLAG_NONE, true);
	}
	else
	{
		Bomb.Use = false;
		Bomb.LockOnNum = 0;
		Player->InBomb = false;
		Player->BombNum++;
	}

	return;
}

//=============================================================================
// ボムBタイプの弾を発射する
//=============================================================================
void ShotBombBullet(void)
{
	int Bullet_No = 0;
	int LockOn_No = 0;
	PLAYER *Player = GetPlayer(0);
	BULLETTYPE *BulletType = GetBulletType(BombBulletType);

	for (LockOn_No = 0; LockOn_No < BombBullet_Max; LockOn_No++)
	{
		if (Bomb.LockOn[LockOn_No].Use == true)
		{
			for (Bullet_No = 0; Bullet_No < BombBullet_Max; Bullet_No++)
			{
				if (Bomb.Bullet[Bullet_No].Use == false)
				{
					Bomb.Bullet[Bullet_No].Use = true;
					Bomb.Bullet[Bullet_No].DelayDisappear = false;
					Bomb.Bullet[Bullet_No].Pos.x = (float)(Player->Pos.x + 50.0f * cos(DegreeToRadian(36 * Bullet_No)));
					Bomb.Bullet[Bullet_No].Pos.y = (float)(Player->Pos.y - 50.0f * sin(DegreeToRadian(36 * Bullet_No)));
					Bomb.Bullet[Bullet_No].Count = 0;
					Bomb.Bullet[Bullet_No].Type = 0;
					Bomb.Bullet[Bullet_No].State = Homing;
					Bomb.Bullet[Bullet_No].MoveAngle = 0.0f;
					Bomb.Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
					Bomb.Bullet[Bullet_No].Rot.z = Bomb.Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
					Bomb.Bullet[Bullet_No].Radius = BulletType->Radius;
					Bomb.Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
					Bomb.Bullet[Bullet_No].Speed = BombBulletSpeed;
					//Bomb.BulletTarget[Bullet_No] = Bomb.LockOn[LockOn_No].Target_No;
					Bomb.ExplosionRadius[Bullet_No] = 0.0f;
#ifdef _DEBUG
					SetPlayerBulletHC(Bomb.Bullet[Bullet_No].Pos, Bomb.Bullet[Bullet_No].HitRadius, Bullet_No, PlayerBulletHC);
#endif

					break;
				}
			}
		}
	}

	return;
}

//=============================================================================
// ボムBタイプの弾が命中する時の衝撃波を描画する
//=============================================================================
void DrawImpactWave(D3DXVECTOR3 Center, float Radius, D3DCOLOR Color)
{
	// 円の点の数
	const int CirclePointNum = 40;
	int i = 0;
	float Theta = 0.0f;
	float WedgeAngle = 0.0f;
	LPD3DXLINE Line = GetLine();
	D3DXVECTOR2 LinePoint[CirclePointNum + 1] = { D3DXVECTOR2(0.0f, 0.0f) };

	// 360度 / 点の数 = 一つの点移動の角度
	WedgeAngle = (float)((2 * PI) / (CirclePointNum));

	for (i = 0; i <= CirclePointNum; i++)
	{
		// 今の角度
		Theta = i * WedgeAngle;

		// 円の各点の座標を計算
		LinePoint[i].x = (float)(Center.x - Radius * cos(Theta));
		LinePoint[i].y = (float)(Center.y - Radius * sin(Theta));
	}

	// 円を描く
	Line->SetWidth(5);
	Line->Begin();
	Line->Draw(LinePoint, CirclePointNum + 1, Color);
	Line->End();

	return;
}

//=============================================================================
// ボムAタイプの拡大範囲を確認する
//=============================================================================
void CheckVertex(VERTEX_2D *Vtx)
{
	int i = 0;

	if (Vtx->vtx.x <= 0 && Vtx->vtx.y <= MoveRange_Y)
	{
		i++;
	}
	if ((Vtx + 1)->vtx.x >= Screen_Width && (Vtx + 1)->vtx.y <= MoveRange_Y)
	{
		i++;
	}
	if ((Vtx + 2)->vtx.x <= 0 && (Vtx + 2)->vtx.y >= Screen_Height)
	{
		i++;
	}
	if ((Vtx + 3)->vtx.x >= Screen_Width && (Vtx + 3)->vtx.y >= Screen_Height)
	{
		i++;
	}

	if (i == 4)
	{
		Bomb.Phase = ExtendOver;
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeBombVertex(void)
{
	int i = 0, j = 0;

	// 頂点座標の設定	
	SetBombVertex();

	// rhwの設定
	Bomb.VertexWk[0].rhw = 1.0f;
	Bomb.VertexWk[1].rhw = 1.0f;
	Bomb.VertexWk[2].rhw = 1.0f;
	Bomb.VertexWk[3].rhw = 1.0f;

	for (i = 0; i < TextureDevideNum; i++)
	{
		for (j = 0; j < TextureDevideNum; j++)
		{
			BombExtendPixel[i][j][0].rhw = 1.0f;
			BombExtendPixel[i][j][1].rhw = 1.0f;
			BombExtendPixel[i][j][2].rhw = 1.0f;
			BombExtendPixel[i][j][3].rhw = 1.0f;
		}
	}

	// 反射光の設定
	SetBombDiffuse(Bomb.Alpha);

	// テクスチャ座標の設定
	SetBombTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBombTexture(void)
{
	int i = 0, j = 0;
	int x = Bomb.AnimPattern;
	float Size_X = 0.0f;
	float Size_Y = 0.0f;

	if (Bomb.Phase == ShotBomb)
	{
		Size_X = 1.0f / Texture_ShotBomb_Divide_X;
		Bomb.VertexWk[0].tex = D3DXVECTOR2((float)x * Size_X, 0.0f);
		Bomb.VertexWk[1].tex = D3DXVECTOR2((float)x * Size_X + Size_X, 0.0f);
		Bomb.VertexWk[2].tex = D3DXVECTOR2((float)x * Size_X, 1.0f);
		Bomb.VertexWk[3].tex = D3DXVECTOR2((float)x * Size_X + Size_X, 1.0f);
	}
	else if (Bomb.Phase == BombExtending)
	{
		Size_X = 1.0f / (float)TextureDevideNum;
		Size_Y = 1.0f / (float)TextureDevideNum;
		for (i = 0; i < TextureDevideNum; i++)
		{
			for (j = 0; j < TextureDevideNum; j++)
			{
				BombExtendPixel[i][j][0].tex = D3DXVECTOR2(Size_X * j, Size_Y * i);
				BombExtendPixel[i][j][1].tex = D3DXVECTOR2(Size_X * (j + 1), Size_Y * i);
				BombExtendPixel[i][j][2].tex = D3DXVECTOR2(Size_X * j, Size_Y * (i + 1));
				BombExtendPixel[i][j][3].tex = D3DXVECTOR2(Size_X * (j + 1), Size_Y * (i + 1));
			}
		}
	}
	else if (Bomb.Phase == ExtendOver)
	{
		Bomb.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Bomb.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Bomb.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Bomb.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return;
}

//=============================================================================
// ボムAタイプの透明度を設置する
//=============================================================================
void SetBombDiffuse(int Alpha)
{
	int i = 0, j = 0;

	Bomb.VertexWk[0].diffuse = WHITE(Alpha);
	Bomb.VertexWk[1].diffuse = WHITE(Alpha);
	Bomb.VertexWk[2].diffuse = WHITE(Alpha);
	Bomb.VertexWk[3].diffuse = WHITE(Alpha);

	if (Bomb.Phase == BombExtending)
	{
		for (i = 0; i < TextureDevideNum; i++)
		{
			for (j = 0; j < TextureDevideNum; j++)
			{
				BombExtendPixel[i][j][0].diffuse = WHITE(Alpha);
				BombExtendPixel[i][j][1].diffuse = WHITE(Alpha);
				BombExtendPixel[i][j][2].diffuse = WHITE(Alpha);
				BombExtendPixel[i][j][3].diffuse = WHITE(Alpha);
			}
		}
	}

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBombVertex(void)
{
	int i = 0, j = 0;
	float TextureWidth = 0.0f;
	float TextureHeight = 0.0f;
	float Size_X = 0.0f;
	float Size_Y = 0.0f;

	if (Bomb.Phase == ShotBomb || Bomb.Phase == BombExtending)
	{
		Bomb.VertexWk[0].vtx.x = Bomb.Pos.x - cosf(Bomb.BaseAngle + Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[0].vtx.y = Bomb.Pos.y - sinf(Bomb.BaseAngle + Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[1].vtx.x = Bomb.Pos.x + cosf(Bomb.BaseAngle - Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[1].vtx.y = Bomb.Pos.y - sinf(Bomb.BaseAngle - Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[2].vtx.x = Bomb.Pos.x - cosf(Bomb.BaseAngle - Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[2].vtx.y = Bomb.Pos.y + sinf(Bomb.BaseAngle - Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[3].vtx.x = Bomb.Pos.x + cosf(Bomb.BaseAngle + Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[3].vtx.y = Bomb.Pos.y + sinf(Bomb.BaseAngle + Bomb.Rot.z) * Bomb.Radius;
	}
	if (Bomb.Phase == BombExtending)
	{
		TextureWidth = Bomb.VertexWk[1].vtx.x - Bomb.VertexWk[0].vtx.x;
		TextureHeight = Bomb.VertexWk[2].vtx.y - Bomb.VertexWk[0].vtx.y;
		Size_X = TextureWidth / (float)TextureDevideNum;
		Size_Y = TextureHeight / (float)TextureDevideNum;

		for (i = 0; i < TextureDevideNum; i++)
		{
			for (j = 0; j < TextureDevideNum; j++)
			{
				BombExtendPixel[i][j][0].vtx.x = Bomb.VertexWk[0].vtx.x + Size_X * j;
				BombExtendPixel[i][j][0].vtx.y = Bomb.VertexWk[0].vtx.y + Size_Y * i;
				BombExtendPixel[i][j][1].vtx.x = Bomb.VertexWk[0].vtx.x + Size_X * (j + 1);
				BombExtendPixel[i][j][1].vtx.y = Bomb.VertexWk[0].vtx.y + Size_Y * i;
				BombExtendPixel[i][j][2].vtx.x = Bomb.VertexWk[0].vtx.x + Size_X * j;
				BombExtendPixel[i][j][2].vtx.y = Bomb.VertexWk[0].vtx.y + Size_Y * (i + 1);
				BombExtendPixel[i][j][3].vtx.x = Bomb.VertexWk[0].vtx.x + Size_X * (j + 1);
				BombExtendPixel[i][j][3].vtx.y = Bomb.VertexWk[0].vtx.y + Size_Y * (i + 1);
				if (BombExtendPixel[i][j][0].vtx.y <= MoveRange_Y && BombExtendPixel[i][j][1].vtx.y <= MoveRange_Y)
				{
					BombExtendPixel[i][j][0].diffuse = WHITE(0);
					BombExtendPixel[i][j][1].diffuse = WHITE(0);
					BombExtendPixel[i][j][2].diffuse = WHITE(0);
					BombExtendPixel[i][j][3].diffuse = WHITE(0);
				}
			}
		}

		CheckVertex(&Bomb.VertexWk[0]);
	}
	if (Bomb.Phase == ExtendOver)
	{
		Bomb.VertexWk[0].vtx = D3DXVECTOR3(0 + Bomb.ShakePos.x, MoveRange_Y + Bomb.ShakePos.y, 0);
		Bomb.VertexWk[1].vtx = D3DXVECTOR3(Screen_Width + Bomb.ShakePos.x, MoveRange_Y + Bomb.ShakePos.y, 0);
		Bomb.VertexWk[2].vtx = D3DXVECTOR3(0 + Bomb.ShakePos.x, Screen_Height + Bomb.ShakePos.y, 0);
		Bomb.VertexWk[3].vtx = D3DXVECTOR3(Screen_Width + Bomb.ShakePos.x, Screen_Height + Bomb.ShakePos.y, 0);
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeLockOnVertex(int LockOn_No)
{
	// 頂点座標の設定	
	SetLockOnVertex(LockOn_No);

	// rhwの設定
	Bomb.LockOn[LockOn_No].VertexWk[0].rhw = 1.0f;
	Bomb.LockOn[LockOn_No].VertexWk[1].rhw = 1.0f;
	Bomb.LockOn[LockOn_No].VertexWk[2].rhw = 1.0f;
	Bomb.LockOn[LockOn_No].VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	Bomb.LockOn[LockOn_No].VertexWk[0].diffuse = WHITE(255);
	Bomb.LockOn[LockOn_No].VertexWk[1].diffuse = WHITE(255);
	Bomb.LockOn[LockOn_No].VertexWk[2].diffuse = WHITE(255);
	Bomb.LockOn[LockOn_No].VertexWk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetLockOnTexture(LockOn_No);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetLockOnTexture(int LockOn_No)
{
	Bomb.LockOn[LockOn_No].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Bomb.LockOn[LockOn_No].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Bomb.LockOn[LockOn_No].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Bomb.LockOn[LockOn_No].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetLockOnVertex(int LockOn_No)
{
	Bomb.LockOn[LockOn_No].VertexWk[0].vtx.x = Bomb.LockOn[LockOn_No].Pos.x - cosf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[0].vtx.y = Bomb.LockOn[LockOn_No].Pos.y - sinf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[1].vtx.x = Bomb.LockOn[LockOn_No].Pos.x + cosf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[1].vtx.y = Bomb.LockOn[LockOn_No].Pos.y - sinf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[2].vtx.x = Bomb.LockOn[LockOn_No].Pos.x - cosf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[2].vtx.y = Bomb.LockOn[LockOn_No].Pos.y + sinf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[3].vtx.x = Bomb.LockOn[LockOn_No].Pos.x + cosf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[3].vtx.y = Bomb.LockOn[LockOn_No].Pos.y + sinf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeBulletVertex(int Bullet_No)
{
	// 頂点座標の設定	
	SetBulletVertex(Bullet_No);

	// rhwの設定
	Bomb.Bullet[Bullet_No].VertexWk[0].rhw = 1.0f;
	Bomb.Bullet[Bullet_No].VertexWk[1].rhw = 1.0f;
	Bomb.Bullet[Bullet_No].VertexWk[2].rhw = 1.0f;
	Bomb.Bullet[Bullet_No].VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	Bomb.Bullet[Bullet_No].VertexWk[0].diffuse = WHITE(255);
	Bomb.Bullet[Bullet_No].VertexWk[1].diffuse = WHITE(255);
	Bomb.Bullet[Bullet_No].VertexWk[2].diffuse = WHITE(255);
	Bomb.Bullet[Bullet_No].VertexWk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetBulletTexture(Bullet_No);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBulletTexture(int Bullet_No)
{
	Bomb.Bullet[Bullet_No].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Bomb.Bullet[Bullet_No].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Bomb.Bullet[Bullet_No].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Bomb.Bullet[Bullet_No].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBulletVertex(int Bullet_No)
{
	Bomb.Bullet[Bullet_No].VertexWk[0].vtx.x =
		Bomb.Bullet[Bullet_No].Pos.x - cosf(Bomb.Bullet[Bullet_No].BaseAngle + Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[0].vtx.y =
		Bomb.Bullet[Bullet_No].Pos.y - sinf(Bomb.Bullet[Bullet_No].BaseAngle + Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[1].vtx.x =
		Bomb.Bullet[Bullet_No].Pos.x + cosf(Bomb.Bullet[Bullet_No].BaseAngle - Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[1].vtx.y =
		Bomb.Bullet[Bullet_No].Pos.y - sinf(Bomb.Bullet[Bullet_No].BaseAngle - Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[2].vtx.x =
		Bomb.Bullet[Bullet_No].Pos.x - cosf(Bomb.Bullet[Bullet_No].BaseAngle - Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[2].vtx.y =
		Bomb.Bullet[Bullet_No].Pos.y + sinf(Bomb.Bullet[Bullet_No].BaseAngle - Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[3].vtx.x =
		Bomb.Bullet[Bullet_No].Pos.x + cosf(Bomb.Bullet[Bullet_No].BaseAngle + Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[3].vtx.y =
		Bomb.Bullet[Bullet_No].Pos.y + sinf(Bomb.Bullet[Bullet_No].BaseAngle + Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;

	return;
}

//=============================================================================
// ボムの情報を取得する
//=============================================================================
BOMB *GetBomb(void)
{
	return &Bomb;
}

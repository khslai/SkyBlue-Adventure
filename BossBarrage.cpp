//=============================================================================
//
// ボス弾幕処理 [BossBarrage.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Boss.h"
#include "Player.h"
#include "Enemy.h"
#include "Barrage.h"
#include "Laser.h"
#include "Effect.h"
#include "Item.h"
#include "Sound.h"
#include "DebugTest.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define StarDrawRange (150.0f)	// 星型弾幕の描画範囲
#define StarDrawSpeed (12)		// 星型弾幕の描画速度

// 弾の状態
enum
{
	StopRotate,			// 回転停止
	InRotate,			// 回転中
	AccelStart,			// 加速開始
	AccelStop,			// 加速停止
	ClockWise,			// 回転方向は右回り
	Counterclockwise,	// 回転方向は左回り
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
static HRESULT MakeBulletVertex(int Bullet_No);
// 頂点座標の設定
static void SetBulletVertex(int Bullet_No);
// テクスチャ座標の設定
static void SetBulletTexture(int Bullet_No);
// ボス弾幕設置
void BossShot(int ShotPattern);
// 星型弾幕計算
void StarBarrage(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
static LPDIRECT3DTEXTURE9 BulletTexture[BulletType_Max] = { NULL };

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBossBarrage(int InitState)
{
	int Bullet_No = 0;
	BOSS *Boss = GetBoss();

	for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
	{
		Boss->Bullet[Bullet_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Boss->Bullet[Bullet_No].PreviousPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Boss->Bullet[Bullet_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Boss->Bullet[Bullet_No].Radius = 0.0f;
		Boss->Bullet[Bullet_No].HitRadius = 0.0f;
		Boss->Bullet[Bullet_No].BaseAngle = 0.0f;
		Boss->Bullet[Bullet_No].MoveAngle = 0.0f;
		Boss->Bullet[Bullet_No].Speed = 0.0f;
		Boss->Bullet[Bullet_No].Type = 0;
		Boss->Bullet[Bullet_No].Count = 0;
		Boss->Bullet[Bullet_No].Color = 0;
		Boss->Bullet[Bullet_No].State = 0;
		Boss->Bullet[Bullet_No].Use = false;

		// 頂点情報の作成
		MakeBulletVertex(Bullet_No);
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
void UninitBossBarrage(void)
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
void UpdateBossBarrage(void)
{
	int Bullet_No = 0;
	int GameStage = GetGameStage();
	BOSS *Boss = GetBoss();
	BULLETTYPE *BulletType = GetBulletType(Boss->BulletType);

	if (Boss->Exist == true)
	{
		if (Boss->State == ShotBullet)
		{
			switch (Boss->Phase)
			{
			case MiddleBoss_Phase1:
				BossShot(MiddleBoss_Phase1);
				break;
			case MiddleBoss_LastPhase:
				BossShot(MiddleBoss_LastPhase);
				break;
			case LastBoss_Phase1:
				BossShot(LastBoss_Phase1);
				break;
			case LastBoss_Phase2:
				BossShot(LastBoss_Phase2);
				break;
			case LastBoss_Phase3:
				BossShot(LastBoss_Phase3);
				break;
			case LastBoss_LastPhase:
				// レーザー設置
				if (Boss->ShotCount % 300 == 0 && Boss->BombDamage == false)
				{
					SetLaser();
				}
				BossShot(LastBoss_LastPhase);
				break;
			default:
				break;
			}
			// カウント加算
			Boss->ShotCount++;
		}

		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				// 回復中、消失中、ラスボス終了の状態なら
				if (Boss->State == HPRecover || Boss->State == BossDisappear || Boss->State == LastBossOver)
				{
					// すべての弾を消す
					Boss->Bullet[Bullet_No].Use = false;
					// 弾のアイテムを設置
					SetBossBulletItem(Boss->Bullet[Bullet_No].Pos);
					continue;
				}
				// 1フレイム前の弾座標を記録
				Boss->Bullet[Bullet_No].PreviousPos = Boss->Bullet[Bullet_No].Pos;
				// 座標計算
				Boss->Bullet[Bullet_No].Pos.x += cosf(Boss->Bullet[Bullet_No].MoveAngle) * Boss->Bullet[Bullet_No].Speed;
				Boss->Bullet[Bullet_No].Pos.y += sinf(Boss->Bullet[Bullet_No].MoveAngle) * Boss->Bullet[Bullet_No].Speed;
				// 特定タイプの弾だったら、回転する
				if (Boss->Bullet[Bullet_No].State == InRotate && Boss->Bullet[Bullet_No].Type == 5)
				{
					Boss->Bullet[Bullet_No].Rot.z += 0.1f;
				}
				// カウント加算
				Boss->Bullet[Bullet_No].Count++;
				// テクスチャ更新
				SetBulletTexture(Bullet_No);
				SetBulletVertex(Bullet_No);
				// 弾の移動先を検査
				if (GameStage != Stage_PraticeSelect)
				{
					if (CheckRange(OutOfRange, Boss->Bullet[Bullet_No].Pos,
						BulletType->Width, BulletType->Height) == true)
					{
						Boss->Bullet[Bullet_No].Use = false;
					}
				}
				else
				{
					if (CheckRange(OutOfRange_SelectChar, Boss->Bullet[Bullet_No].Pos,
						BulletType->Width, BulletType->Height) == true)
					{
						Boss->Bullet[Bullet_No].Use = false;
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
void DrawBossBarrage(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	BOSS *Boss = GetBoss();
	int Bullet_No = 0;

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	if (Boss->Exist == true)
	{
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				// テクスチャの設定
				switch (Boss->Bullet[Bullet_No].Type)
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
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Boss->Bullet[Bullet_No].VertexWk, sizeof(VERTEX_2D));
			}
		}
	}

	return;
}


//=============================================================================
// ボスが弾を発射
//=============================================================================
void BossShot(int ShotPattern)
{
	int i = 0;
	int Bullet_No = 0;
	int Shot_Max = 0;
	int GameStage = GetGameStage();
	int GameDifficulty = GetGameDiffuculty();
	static int MoveDirection = 0;
	BOSS *Boss = GetBoss();
	int Count = Boss->ShotCount;
	PLAYER *Player = GetPlayer(0);
	BULLETTYPE *BulletType = GetBulletType(Boss->BulletType);

	// ボムのダメージを受ける最中
	if (Boss->BombDamage == true)
	{
		// 準備状態に戻る
		Boss->State = StandBy;
	}
	else
	{
		switch (ShotPattern)
		{
		case MiddleBoss_Phase1:
			// 1ターン60カウント、6回円形30発の弾をうつ
			// 毎回円形の弾は15度の差がある
			if (GameDifficulty == Easy)
			{
				Shot_Max = 20;
			}
			else
			{
				Shot_Max = 30;
			}
			// 最初の設置
			if (Count == 0)
			{
				// ボスとプレイヤーの角度を記録
				Boss->ShotAngle = AngleCalculate(Boss->Pos, Player->Pos);
				// 弾のタイプを設置
				Boss->BulletType = 0;
				BulletType = GetBulletType(Boss->BulletType);
			}
			// 90カウントずつ移動
			if (Count % 90 == 0)
			{
				if (GameStage != Stage_PraticeSelect)
				{
					// X = 650 ~ 800、Y = 250 ~ 450の区間内移動
					BossMove(
						D3DXVECTOR3((float)(rand() % (150 + 1) + 650), (float)(rand() % (200 + 1) + 250), 0.0f),
						60.0f);
				}
			}
			// 10カウントずつ弾発射
			if (Count % 10 == 0)
			{
				// 弾発射音
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BulletShot_01, E_DS8_FLAG_NONE, true);
				}
				for (i = 0; i < Shot_Max; i++)
				{
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						if (Boss->Bullet[Bullet_No].Use == false)
						{
							// 弾の再初期化処理
							Boss->Bullet[Bullet_No].Use = true;
							Boss->Bullet[Bullet_No].Pos = Boss->Pos;

							Boss->Bullet[Bullet_No].MoveAngle =
								Boss->ShotAngle + Count / 10 * DegreeToRadian(-15) + (2 * PI / Shot_Max * i);
							Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
							Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Boss->Bullet[Bullet_No].Type = Boss->BulletType;
							Boss->Bullet[Bullet_No].Count = 0;
							Boss->Bullet[Bullet_No].Color = 0;
							Boss->Bullet[Bullet_No].State = StopRotate;
							Boss->Bullet[Bullet_No].Speed = 5.0f;
							break;
						}
					}
				}
			}
			// 60カウント後、発射停止
			else if (Count > 60)
			{
				Boss->State = StandBy;
			}
			break;
		case MiddleBoss_LastPhase:
			// サイレントセレナ
			// 1ターン60カウント、10回円形20発の弾をうつ
			// 1ターンの最初は自機狙い、半分からは自機狙いからずらす
			Shot_Max = 20;
			Count = Boss->ShotCount % 60;
			// ターンの最初
			if (Count == 0)
			{
				// 自機狙い
				Boss->ShotAngle = AngleCalculate(Boss->Pos, Player->Pos);
				// 弾のタイプ設置
				if (Boss->ShotCount == 0)
				{
					Boss->BulletType = 2;
					BulletType = GetBulletType(Boss->BulletType);
					MoveDirection = 1;
				}
			}
			// 半分からは自機狙いからずらす
			else if (Count == 30)
			{
				// 角度を加算
				Boss->ShotAngle += PI / Shot_Max;
			}
			// 4ターン = 240カウントずつ移動
			if (Boss->ShotCount % (60 * 4) == 0)
			{
				if (GameStage != Stage_PraticeSelect)
				{
					// 移動パターンを変わる
					if (Boss->Pos.y >= ScreenCenter_Y + 70.0f)
					{
						MoveDirection = -1;
					}
					else if (Boss->Pos.y <= ScreenCenter_Y - 70.0f)
					{
						MoveDirection = 1;
					}
					// ボス移動
					BossMove(D3DXVECTOR3(Boss->Pos.x, Boss->Pos.y + 70.0f * MoveDirection, Boss->Pos.z), 60.0f);
				}
			}
			// 8カウントずつ一回発射
			if (Count % 8 == 0)
			{
				// 弾発射音
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BossShot_02, E_DS8_FLAG_NONE, true);
				}
				for (i = 0; i < Shot_Max; i++)
				{
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						if (Boss->Bullet[Bullet_No].Use == false)
						{
							Boss->Bullet[Bullet_No].Use = true;
							Boss->Bullet[Bullet_No].Pos = Boss->Pos;
							// ベース角度から20個回転して発射
							Boss->Bullet[Bullet_No].MoveAngle = Boss->ShotAngle + PI * 2 / 20 * i;
							Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
							Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Boss->Bullet[Bullet_No].Type = Boss->BulletType;
							Boss->Bullet[Bullet_No].Count = 0;
							Boss->Bullet[Bullet_No].Color = 0;
							Boss->Bullet[Bullet_No].State = StopRotate;
							Boss->Bullet[Bullet_No].Speed = 4.5f;
							break;
						}
					}
				}
			}
			// 5カウントに1回、左に発射する弾を登録
			if (Count % 5 == 0 && GameDifficulty != Easy)
			{
				for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
				{
					if (Boss->Bullet[Bullet_No].Use == false)
					{
						Boss->Bullet[Bullet_No].Use = true;
						Boss->Bullet[Bullet_No].Pos.x = (float)(rand() % (Screen_Width / 3 + 1) + Screen_Width * 2 / 3);
						Boss->Bullet[Bullet_No].Pos.y = (float)(rand() % (Screen_Height + 1));
						Boss->Bullet[Bullet_No].MoveAngle = DegreeToRadian(180);
						Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
						Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
						Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
						Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
						Boss->Bullet[Bullet_No].Type = Boss->BulletType;
						Boss->Bullet[Bullet_No].Count = 0;
						Boss->Bullet[Bullet_No].Color = 0;
						Boss->Bullet[Bullet_No].State = StopRotate;
						Boss->Bullet[Bullet_No].Speed = 1.0f + ((float)rand() / RAND_MAX);
						break;
					}
				}
			}
			break;
		case LastBoss_Phase1:
			// ミシャグジさま
			if (GameDifficulty == Easy)
			{
				Shot_Max = 40;
			}
			else if (GameDifficulty == Normal)
			{
				Shot_Max = 50;
			}
			else if (GameDifficulty == Hard)
			{
				Shot_Max = 60;
			}
			// 弾のタイプ設置
			if (Boss->ShotCount == 0)
			{
				Boss->BulletType = 4;
				BulletType = GetBulletType(Boss->BulletType);
			}
			// 90カウントずつ弾発射
			if (Count % 90 == 0)
			{
				// 弾発射音
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BossShot_01, E_DS8_FLAG_NONE, true);
				}
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < Shot_Max; j++)
					{
						for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
						{
							// 使用されていない弾があれば
							if (Boss->Bullet[Bullet_No].Use == false)
							{
								// 弾の再初期化処理
								Boss->Bullet[Bullet_No].Use = true;
								Boss->Bullet[Bullet_No].Type = Boss->BulletType;
								Boss->Bullet[Bullet_No].Pos = Boss->Pos;
								Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
								Boss->Bullet[Bullet_No].MoveAngle = Boss->BaseAngle + PI * 2 / Shot_Max * j;
								Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
								Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
								Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
								Boss->Bullet[Bullet_No].Color = 0;
								Boss->Bullet[Bullet_No].Count = 0;
								Boss->Bullet[Bullet_No].Speed = 3.0f;
								// ステータス。0か1かで回転方向がかわる
								Boss->Bullet[Bullet_No].State = i == 0 ? ClockWise : Counterclockwise;
								break;
							}
						}
					}
				}
			}

			// 弾の回転計算
			for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
			{
				if (Boss->Bullet[Bullet_No].Use == true)
				{
					// 30～120カウントなら
					if (30 < Boss->Bullet[Bullet_No].Count && Boss->Bullet[Bullet_No].Count < 120)
					{
						// 90カウントかけて1.2減らす
						Boss->Bullet[Bullet_No].Speed -= 1.2f / 90.0f;
						// 90カウントかけて90°傾ける
						Boss->Bullet[Bullet_No].MoveAngle += (PI / 2) / 90.0f * (Boss->Bullet[Bullet_No].State == ClockWise ? 1 : -1);
						Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
					}
				}
			}
			break;
		case LastBoss_Phase2:
			// パーフェクトフリーズ
			// 1ターン600カウント
			Count = Boss->ShotCount % 600;
			// 0、270カウントの時に移動
			if (Count == 0 || Count == 270)
			{
				if (GameStage != Stage_PraticeSelect)
				{
					// X = 650 ~ 800、Y = 250 ~ 450の区間内移動
					BossMove(
						D3DXVECTOR3((float)(rand() % (150 + 1) + 650), (float)(rand() % (200 + 1) + 250), 0.0f),
						60.0f);
				}
			}
			//最初のランダム発射
			if (Count < 240)
			{
				// 弾発射音
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BulletShot_03, E_DS8_FLAG_NONE, true);
				}
				// 弾のタイプは5番
				BulletType = GetBulletType(5);
				// 1カウントに2回発射
				for (i = 0; i < 2; i++)
				{
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						if (Boss->Bullet[Bullet_No].Use == false)
						{
							Boss->Bullet[Bullet_No].Use = true;
							Boss->Bullet[Bullet_No].Pos = Boss->Pos;
							Boss->Bullet[Bullet_No].MoveAngle = RandRadian(PI * 2 / 20) + PI * 2 / 10 * Count;
							Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
							Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Boss->Bullet[Bullet_No].Type = 5;
							Boss->Bullet[Bullet_No].Color = rand() % (6 + 1);
							Boss->Bullet[Bullet_No].Count = 0;
							Boss->Bullet[Bullet_No].State = InRotate;
							Boss->Bullet[Bullet_No].Speed = 5.0f + (3.0f * (float)rand() / RAND_MAX);
							break;
						}
					}
				}
			}
			// 240カウントの時に全てストップさせ、白くし、カウントリセット
			else if (Count == 240)
			{
				// 弾停止音
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BulletFreeze, E_DS8_FLAG_NONE, true);
				}
				for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
				{
					if (Boss->Bullet[Bullet_No].Use == true)
					{
						if (Boss->Bullet[Bullet_No].Type == 5)
						{
							Boss->Bullet[Bullet_No].Color = 7;
							Boss->Bullet[Bullet_No].Count = 0;
							Boss->Bullet[Bullet_No].State = StopRotate;
							Boss->Bullet[Bullet_No].Speed = 0.0f;
						}
					}
				}
			}
			// 自機依存による8方向発射
			else if (Count >= 270 && Count < 330 && Count % 3 == 0)
			{
				// 弾発射音
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BulletShot_03, E_DS8_FLAG_NONE, true);
				}
				// 自機狙い
				Boss->ShotAngle = AngleCalculate(Boss->Pos, Player->Pos);
				// 0番タイプの弾
				BulletType = GetBulletType(0);
				for (i = 0; i < 7; i++)
				{
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						if (Boss->Bullet[Bullet_No].Use == false)
						{
							Boss->Bullet[Bullet_No].Use = true;
							Boss->Bullet[Bullet_No].Pos = Boss->Pos;
							// 自機とボスとの成す角を基準に7方向に発射する
							// 0度 -> -15度 -> 30度 -> -45度 -> 15度 -> -30度 -> 45度
							Boss->Bullet[Bullet_No].MoveAngle =
								Boss->ShotAngle + DegreeToRadian(15) * (i >= 4 ? i % 4 + 1 : i % 4) * (i % 2 == 0 ? 1 : -1) + RandRadian(PI / 180);
							Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
							Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Boss->Bullet[Bullet_No].Type = 0;
							Boss->Bullet[Bullet_No].Color = 0;
							Boss->Bullet[Bullet_No].Count = 0;
							Boss->Bullet[Bullet_No].State = StopRotate;
							Boss->Bullet[Bullet_No].Speed = 5.0f;
							break;
						}
					}
				}
			}
			// フリーズ終了、移動開始
			else if (Count >= 330)
			{
				for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
				{
					if (Boss->Bullet[Bullet_No].Use == true)
					{
						// ランダムな方向に移動し始める
						if (Boss->Bullet[Bullet_No].Type == 5)
						{
							if (Boss->Bullet[Bullet_No].Count == 150)
							{
								// 全方向ランダム
								Boss->Bullet[Bullet_No].MoveAngle = RandRadian(PI);
								Boss->Bullet[Bullet_No].State = InRotate;
							}
							if (Boss->Bullet[Bullet_No].Count > 150)
							{
								// 段々加速
								Boss->Bullet[Bullet_No].Speed += 0.02f;
							}
						}
					}
				}
			}
			break;
		case LastBoss_Phase3:
			// ☆形弾幕
			StarBarrage();
			break;
		case LastBoss_LastPhase:
			// 円形発射
			// 弾のタイプ設置
			if (Count == 0)
			{
				// 1番の弾
				Boss->BulletType = 1;
				BulletType = GetBulletType(Boss->BulletType);
			}
			// 一回は30発
			Shot_Max = 30;
			// 20カウントずつ弾発射
			if (Count % 20 == 0)
			{
				// 弾発射音
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BulletShot_01, E_DS8_FLAG_NONE, true);
				}

				// 自機狙い
				Boss->ShotAngle = AngleCalculate(Boss->Pos, Player->Pos);
				for (int i = 0; i < Shot_Max; i++)
				{
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						// 使用されていない弾があれば
						if (Boss->Bullet[Bullet_No].Use == false)
						{
							// 弾の再初期化処理
							Boss->Bullet[Bullet_No].Use = true;
							Boss->Bullet[Bullet_No].Type = Boss->BulletType;
							Boss->Bullet[Bullet_No].Pos = Boss->Pos;
							// 角度 + 360度(2PI) / 20発 * 何発目
							Boss->Bullet[Bullet_No].MoveAngle = Boss->ShotAngle + 2 * PI / Shot_Max * i;
							Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
							Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Boss->Bullet[Bullet_No].Type = Boss->BulletType;
							Boss->Bullet[Bullet_No].Count = 0;
							Boss->Bullet[Bullet_No].Color = 0;
							Boss->Bullet[Bullet_No].State = StopRotate;
							Boss->Bullet[Bullet_No].Speed = 4.0f;
							break;
						}
					}
				}
			}
			break;
		default:
			break;
		}
	}

	return;
}

//=============================================================================
// ☆形弾幕
//=============================================================================
void StarBarrage(void)
{
	int i = 0;
	int Bullet_No = 0;
	int GameStage = GetGameStage();
	BOSS *Boss = GetBoss();
	int Count = Boss->ShotCount;
	BULLETTYPE *BulletType = GetBulletType(Boss->BulletType);
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Distance = 0.0f;
	// ☆形描画終了のフラグ
	bool DrawOver = false;
	// 今描画している線の番号
	static int DrawStarLine = 0;
	// ☆形描画終了後の時間を記録
	static int DrawOverCount = 0;
	// 弾の移動角度計算用
	static int Num = 0;
	static float AngleTemp[BossBullet_Max] = { 0.0f };
	// 描画の角度
	static float DrawAngle = 0.0f;
	// 今描画している座標
	static float DrawPos_X = 0.0f;
	static float DrawPos_Y = 0.0f;
	// ☆形の中心座標
	static float CenterPos_X = 0.0f;
	static float CenterPos_Y = 0.0f;
	// ☆形の五つの頂点座標
	static float VertexPos_X[5] = { 0.0f };
	static float VertexPos_Y[5] = { 0.0f };

	// 750カウント1ループ
	Count = Boss->ShotCount % 750;
	if (Boss->ShotCount == 0)
	{
		Boss->BulletType = 0;
		BulletType = GetBulletType(Boss->BulletType);
	}
	// 1周期の最初なら
	if (Count == 0)
	{
		DrawOverCount = 0;
		DrawStarLine = 0;
		// ☆形の五つの頂点座標を記録
		for (i = 0; i < 5; i++)
		{
			VertexPos_X[i] = Boss->Pos.x + cosf(DegreeToRadian(90) + DegreeToRadian(72) * i) * StarDrawRange;
			VertexPos_Y[i] = Boss->Pos.y - sinf(DegreeToRadian(90) + DegreeToRadian(72) * i) * StarDrawRange;
		}
		// 描画座標は頂点[0]
		DrawPos_X = VertexPos_X[0];
		DrawPos_Y = VertexPos_Y[0];
		// 中心座標はボスの位置
		CenterPos_X = Boss->Pos.x;
		CenterPos_Y = Boss->Pos.y;
		// 最初の角度は108度 -> -36 -> -180 -> 36 -> -108
		DrawAngle = DegreeToRadian(108);
		Num = 0;
	}
	// ☆を描く
	if (DrawStarLine < 5)
	{
		// 描画座標を計算
		DrawPos_X += cosf(DrawAngle) * StarDrawSpeed;
		DrawPos_Y += sinf(DrawAngle) * StarDrawSpeed;
		// 中心からの距離を計算
		temp = D3DXVECTOR3((DrawPos_X - CenterPos_X), (DrawPos_Y - CenterPos_Y), 0.0f);
		Distance = D3DXVec3LengthSq(&temp);
		// 中心からの距離は半径より大きいなら -> 円と交わる
		if (Distance > StarDrawRange * StarDrawRange)
		{
			// 方向転換、今の角度-144度
			DrawAngle -= DegreeToRadian(144);
			// 一つの線を描く完了
			DrawStarLine++;
			// 描画座標は頂点座標になる
			DrawPos_X = VertexPos_X[(DrawStarLine * 2) % 5];
			DrawPos_Y = VertexPos_Y[(DrawStarLine * 2) % 5];
		}
		// 弾発射音
		if (IsPlaying(SE_BossShot_03) == false && GameStage != Stage_PraticeSelect)
		{
			PlaySound(SE_BossShot_03, E_DS8_FLAG_NONE, true);
		}
		for (i = 0; i < 5; i++)
		{
			// 星を五つ描く
			for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
			{
				if (Boss->Bullet[Bullet_No].Use == false)
				{
					Boss->Bullet[Bullet_No].Use = true;
					Boss->Bullet[Bullet_No].Type = Boss->BulletType;
					Boss->Bullet[Bullet_No].Pos.x = DrawPos_X;
					Boss->Bullet[Bullet_No].Pos.y = DrawPos_Y;
					Boss->Bullet[Bullet_No].MoveAngle = DegreeToRadian(-90) + DegreeToRadian(72) * i;
					Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
					Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
					Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
					Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
					Boss->Bullet[Bullet_No].Type = Boss->BulletType;
					Boss->Bullet[Bullet_No].Count = 0;
					Boss->Bullet[Bullet_No].Color = i;
					Boss->Bullet[Bullet_No].State = 0;
					Boss->Bullet[Bullet_No].Speed = 0.0f;
					AngleTemp[Bullet_No] = DrawAngle - PI + PI / 10 * Num;
					break;
				}
			}
		}
		Num++;
	}
	// 描く終了
	else
	{
		StopSound(SE_BossShot_03);
		DrawOver = true;
		DrawOverCount++;
	}

	if (DrawOver == true && DrawOverCount >= 60 && DrawOverCount <= 300)
	{
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				if (Boss->Bullet[Bullet_No].State != AccelStart)
				{
					// 弾展開効果音
					if (IsPlaying(SE_StarExtend) == false && GameStage != Stage_PraticeSelect)
					{
						PlaySound(SE_StarExtend, E_DS8_FLAG_NONE, false);
					}
					// 描画完了一秒後なら星を５方向に発射
					// スピード
					Boss->Bullet[Bullet_No].Speed = 4.0f;
					// カウンタリセット
					Boss->Bullet[Bullet_No].Count = 0;
					Boss->Bullet[Bullet_No].State = AccelStart;
				}
				else if (Boss->Bullet[Bullet_No].State == AccelStart)
				{
					if (Boss->Bullet[Bullet_No].Count <= 80)
					{
						// 減速
						Boss->Bullet[Bullet_No].Speed -= 0.05f;
					}
					else if (Boss->Bullet[Bullet_No].Count == 120)
					{
						// セットした基準角度から綺麗な曲線を描く
						Boss->Bullet[Bullet_No].MoveAngle = AngleTemp[Bullet_No];
					}
					else if (Boss->Bullet[Bullet_No].Count > 120)
					{
						// 加速
						Boss->Bullet[Bullet_No].Speed += 0.02f;
					}
				}
			}
		}
	}

	return;
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeBulletVertex(int Bullet_No)
{
	BOSS *Boss = GetBoss();

	// 頂点座標の設定	
	SetBulletVertex(Bullet_No);

	// rhwの設定
	Boss->Bullet[Bullet_No].VertexWk[0].rhw = 1.0f;
	Boss->Bullet[Bullet_No].VertexWk[1].rhw = 1.0f;
	Boss->Bullet[Bullet_No].VertexWk[2].rhw = 1.0f;
	Boss->Bullet[Bullet_No].VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	Boss->Bullet[Bullet_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Boss->Bullet[Bullet_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Boss->Bullet[Bullet_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Boss->Bullet[Bullet_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetBulletTexture(Bullet_No);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBulletTexture(int Bullet_No)
{
	BOSS *Boss = GetBoss();
	BULLETTYPE *BulletType = GetBulletType(Boss->Bullet[Bullet_No].Type);
	int x = Boss->Bullet[Bullet_No].Color;
	float sizeX = 1.0f / BulletType->Devide_X;

	Boss->Bullet[Bullet_No].VertexWk[0].tex = D3DXVECTOR2((float)x * sizeX, 0.0f);
	Boss->Bullet[Bullet_No].VertexWk[1].tex = D3DXVECTOR2((float)x * sizeX + sizeX, 0.0f);
	Boss->Bullet[Bullet_No].VertexWk[2].tex = D3DXVECTOR2((float)x * sizeX, 1.0f);
	Boss->Bullet[Bullet_No].VertexWk[3].tex = D3DXVECTOR2((float)x * sizeX + sizeX, 1.0f);

	return;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBulletVertex(int Bullet_No)
{
	BOSS *Boss = GetBoss();

	Boss->Bullet[Bullet_No].VertexWk[0].vtx.x =
		Boss->Bullet[Bullet_No].Pos.x - cosf(Boss->Bullet[Bullet_No].BaseAngle + Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[0].vtx.y =
		Boss->Bullet[Bullet_No].Pos.y - sinf(Boss->Bullet[Bullet_No].BaseAngle + Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[1].vtx.x =
		Boss->Bullet[Bullet_No].Pos.x + cosf(Boss->Bullet[Bullet_No].BaseAngle - Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[1].vtx.y =
		Boss->Bullet[Bullet_No].Pos.y - sinf(Boss->Bullet[Bullet_No].BaseAngle - Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[2].vtx.x =
		Boss->Bullet[Bullet_No].Pos.x - cosf(Boss->Bullet[Bullet_No].BaseAngle - Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[2].vtx.y =
		Boss->Bullet[Bullet_No].Pos.y + sinf(Boss->Bullet[Bullet_No].BaseAngle - Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[3].vtx.x =
		Boss->Bullet[Bullet_No].Pos.x + cosf(Boss->Bullet[Bullet_No].BaseAngle + Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[3].vtx.y =
		Boss->Bullet[Bullet_No].Pos.y + sinf(Boss->Bullet[Bullet_No].BaseAngle + Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;

	return;
}
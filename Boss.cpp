//=============================================================================
//
// ボス処理 [Boss.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Boss.h"
#include "Title.h"
#include "Player.h"
#include "Enemy.h"
#include "Barrage.h"
#include "Laser.h"
#include "Effect.h"
#include "Gauge.h"
#include "Item.h"
#include "PraticeSelect.h"
#include "Sound.h"
#include "DebugTest.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define AnimChangeCount		(15)		// 15カウントボスアニメ変換
#define MiddleBossCount		(5400)		// 5400カウントの時中ボス出現
#define LastBossCount		(9000)		// 9000カウントの時ラスボス出現
#define BossStartHP			(3000)		// ボスのHP
#define BossLife_Max		(5)			// ボス残機の最大数	
#define BossLifePos_X		(BossHPGauge_Pos_X - Texture_BossLife_Width)	// ボス残機の表示座標
#define BossLifePos_Y		(BossHPGauge_Pos_Y - Texture_Gauge_Boss_Height)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeBossVertex(void);
HRESULT MakeBossLifeVertex(void);
// 頂点座標の設定
void SetBossVertex(void);
void SetBossLifeVertex(void);
// テクスチャ座標の設定
void SetBossTexture(void);
void SetBossLifeTexture(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 BossTexture = NULL;
LPDIRECT3DTEXTURE9 BossLifeTexture = NULL;
// ボス構造体
BOSS Boss;
// ボス生命値構造体
BOSSLIFE BossLife[BossLife_Max];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBoss(int InitState)
{
	Boss.Exist = false;
	Boss.MiddleBossOver = false;
	Boss.InMove = false;
	Boss.DebutMove = false;
	Boss.BombDamage = false;
	Boss.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.BeforeMovePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.Acceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.Destination = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.Radius = 0.0f;
	Boss.HitRadius = 0.0f;
	Boss.BaseAngle = 0.0f;
	Boss.MoveAngle = 0.0f;
	Boss.ShotAngle = 0.0f;
	Boss.MoveTime = 0.0f;
	Boss.AnimPattern = 0;
	Boss.Count = 0;
	Boss.ShotCount = 0;
	Boss.MoveCount = 0;
	Boss.MovePattern = 0;
	Boss.Color = 0;
	Boss.HP_Max = 0.0f;
	Boss.HP = Boss.HP_Max;
	Boss.Life = 0;
	Boss.BulletType = 0;
	Boss.State = 0;
	Boss.Phase = 0;

	// 頂点情報の作成
	MakeBossVertex();

	for (int i = 0; i < BossLife_Max; i++)
	{
		BossLife[i].Use = false;
		MakeBossLifeVertex();
	}

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_Boss, &BossTexture, "Boss") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BossLife, &BossLifeTexture, "Boss") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoss(void)
{
	SafeRelease(BossTexture);
	SafeRelease(BossLifeTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoss(void)
{
	D3DXVECTOR3 PreviousPos = Boss.Pos;
	int GameCount = GetGameCount();
	int GameStage = GetGameStage();

	// ボス設置
	if ((GameCount == MiddleBossCount || GameCount == LastBossCount) && Boss.Exist == false)
	{
		SetBoss();
	}

	if (Boss.Exist == true)
	{
		// ボスの段階変化
		// ボスのHPは0になったら
		if (Boss.State == HPZero)
		{
			// 生命値 - 1
			Boss.Life--;
			// 指定位置に移動
			BossMove(D3DXVECTOR3(Screen_Width * 0.75, ScreenCenter_Y, 0.0f), 60.0f);
			// ボスアイテム設置
			SetBossItem(Boss.Pos);
			// HP回復段階を入る
			Boss.State = HPRecover;
		}
		// HP回復終了、次の段階を入る
		else if (Boss.State == NextPhase)
		{
			Boss.HP = Boss.HP_Max;
			Boss.ShotCount = 0;
			Boss.Color++;
			Boss.Phase++;
			Boss.State = StandBy;
		}
		// 中ボス終了
		else if (Boss.State == MiddleBossOver)
		{
			BossMove(D3DXVECTOR3(Screen_Width + 100, ScreenCenter_Y, 0.0f), 60.0f);
			SetBossItem(Boss.Pos);
			Boss.State = BossDisappear;
		}
		// ステージ中、ボスが消える
		else if (Boss.State == BossDisappear)
		{
			if (CheckRange(OutOfRange, Boss.Pos, Texture_Boss_Width, Texture_Boss_Height) == true)
			{
				Boss.Exist = false;
			}
		}
		// ラスボス終了
		else if (Boss.State == LastBossOver)
		{
			PlaySound(SE_BossDead, E_DS8_FLAG_NONE, true);
			Boss.Exist = false;
			SetBossItem(Boss.Pos);
			SetBossDeadEffect();
			return;
		}

		// ボスのアニメ計算
		if ((Boss.Count % AnimChangeCount) == 0)
		{
			// テクスチャアニメパターン計算
			Boss.AnimPattern = (Boss.AnimPattern + 1) % Texture_Boss_Divide_X;
		}

		// 一般状態の上下移動
		if (Boss.InMove == false && Boss.Phase != MiddleBoss_LastPhase)
		{
			if (Boss.Count % 120 < 30 || Boss.Count % 120 >= 90)
			{
				Boss.Pos.y++;
			}
			else if (Boss.Count % 120 >= 30 && Boss.Count % 120 < 90)
			{
				Boss.Pos.y--;
			}
			// 移動不可の範囲に入ったら、前の座標に戻る
			if (CheckRange(InRange, Boss.Pos, Texture_Boss_Width, Texture_Boss_Height) == false && Boss.DebutMove == false)
			{
				Boss.Pos.y = PreviousPos.y;
			}
		}

		// ボスの移動計算
		if (Boss.MoveCount < Boss.MoveTime)
		{
			Boss.Pos.x =
				Boss.BeforeMovePos.x + (Boss.Velocity.x * Boss.MoveCount + 0.5f * Boss.Acceleration.x * Boss.MoveCount * Boss.MoveCount);
			if (CheckRange(InRange, Boss.Pos, Texture_Boss_Width, Texture_Boss_Height) == false && Boss.DebutMove == false && Boss.State != BossDisappear)
			{
				Boss.Pos.x = PreviousPos.x;
			}
			Boss.Pos.y =
				Boss.BeforeMovePos.y + (Boss.Velocity.y * Boss.MoveCount + 0.5f * Boss.Acceleration.y * Boss.MoveCount * Boss.MoveCount);
			if (CheckRange(InRange, Boss.Pos, Texture_Boss_Width, Texture_Boss_Height) == false && Boss.DebutMove == false && Boss.State != BossDisappear)
			{
				Boss.Pos.y = PreviousPos.y;
			}
			Boss.MoveCount++;
		}
		// 移動完了、弾発射開始
		else if (Boss.MoveCount >= Boss.MoveTime && Boss.InMove == true && Boss.State != HPRecover)
		{
			Boss.InMove = false;
			Boss.DebutMove = false;
			if (Boss.State != StandBy)
			{
				Boss.State = ShotBullet;
			}
		}

		// 待機時間計算
		if (Boss.InMove == false && Boss.State == StandBy)
		{
			// 待機完了、発射開始
			if (Boss.Count % 180 == 0)
			{
				Boss.State = ShotBullet;
				Boss.ShotCount = 0;
			}
		}

		// ボスカウント加算
		Boss.Count++;

		// ボステクスチャ更新
		SetBossTexture();
		SetBossColor(White);
		SetBossVertex();

		// ボス生命値のテクスチャ更新
		SetBossLifeTexture();
		SetBossLifeVertex();
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	if (Boss.Exist == true)
	{
		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		Device->SetTexture(0, BossTexture);

		// ポリゴンの描画
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Boss.VertexWk, sizeof(VERTEX_2D));

		for (int i = 0; i < Boss.Life; i++)
		{
			if (BossLife[i].Use == true)
			{
				Device->SetTexture(0, BossLifeTexture);

				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BossLife[i].VertexWk, sizeof(VERTEX_2D));
			}
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeBossVertex(void)
{
	// 頂点座標の設定	
	SetBossVertex();

	// rhwの設定
	Boss.VertexWk[0].rhw = 1.0f;
	Boss.VertexWk[1].rhw = 1.0f;
	Boss.VertexWk[2].rhw = 1.0f;
	Boss.VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	SetBossColor(White);

	// テクスチャ座標の設定
	SetBossTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBossTexture(void)
{
	int x = Boss.AnimPattern;
	int y = Boss.Color;
	float sizeX = 1.0f / Texture_Boss_Divide_X;
	float sizeY = 1.0f / Texture_Boss_Divide_Y;

	Boss.VertexWk[0].tex = D3DXVECTOR2((float)x * sizeX, (float)y * sizeY);
	Boss.VertexWk[1].tex = D3DXVECTOR2((float)x * sizeX + sizeX, (float)y * sizeY);
	Boss.VertexWk[2].tex = D3DXVECTOR2((float)x * sizeX, (float)y * sizeY + sizeY);
	Boss.VertexWk[3].tex = D3DXVECTOR2((float)x * sizeX + sizeX, (float)y * sizeY + sizeY);

	return;
}

//=============================================================================
// ボスの色の設定
//=============================================================================
void SetBossColor(int Color)
{
	switch (Color)
	{
	case White:
		Boss.VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Boss.VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Boss.VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Boss.VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		break;
		// 撃たれたら、色が変わる
	case Red:
		Boss.VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		Boss.VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		Boss.VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		Boss.VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBossVertex(void)
{
	Boss.VertexWk[0].vtx.x = Boss.Pos.x - cosf(Boss.BaseAngle + Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[0].vtx.y = Boss.Pos.y - sinf(Boss.BaseAngle + Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[1].vtx.x = Boss.Pos.x + cosf(Boss.BaseAngle - Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[1].vtx.y = Boss.Pos.y - sinf(Boss.BaseAngle - Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[2].vtx.x = Boss.Pos.x - cosf(Boss.BaseAngle - Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[2].vtx.y = Boss.Pos.y + sinf(Boss.BaseAngle - Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[3].vtx.x = Boss.Pos.x + cosf(Boss.BaseAngle + Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[3].vtx.y = Boss.Pos.y + sinf(Boss.BaseAngle + Boss.Rot.z) * Boss.Radius;

	return;
}

//=============================================================================
// ボスの情報を取得する
//=============================================================================
BOSS *GetBoss(void)
{
	return &Boss;
}

//=============================================================================
// ボスの設置
//=============================================================================
void SetBoss(void)
{
	int Enemy_No = 0;
	int Barrage_No = 0;
	int Bullet_No = 0;
	int GameStage = GetGameStage();
	int GameDifficulty = GetGameDiffuculty();
	int PraticeSelect = GetPraticeSelect();
	D3DXVECTOR2 temp = D3DXVECTOR2(0.0f, 0.0f);
	ENEMY *Enemy = GetEnemy(Enemy_No);
	BARRAGE *Barrage = GetBarrage(Barrage_No);

	// エネミー消す
	Enemy = GetEnemy(0);
	for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++, Enemy++)
	{
		if (Enemy->Exist == true)
		{
			Enemy->Exist = false;
			SetEnemyDeadEffect(Enemy_No);
		}
	}

	// エネミーの弾消す
	Barrage = GetBarrage(0);
	for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++, Barrage++)
	{
		if (Barrage->Phase != BarrageOver)
		{
			for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
			{
				if (Barrage->Bullet[Bullet_No].Use == true)
				{
					Barrage->Bullet[Bullet_No].Use = false;
					SetBulletDeadEffect(Barrage->Bullet[Bullet_No].Pos);
				}
			}
			Barrage->Phase = BarrageOver;
		}
	}

	temp = D3DXVECTOR2(Texture_Boss_Width / 2, Texture_Boss_Height / 2);

	Boss.Exist = true;
	Boss.DebutMove = true;
	Boss.BombDamage = false;
	if (Boss.MiddleBossOver != true)
	{
		Boss.MiddleBossOver = false;
	}
	Boss.Pos = D3DXVECTOR3(Screen_Width, ScreenCenter_Y, 0.0f);
	Boss.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.BaseAngle = atan2f(Texture_Boss_Height, Texture_Boss_Width);
	Boss.MoveAngle = 0.0f;
	Boss.ShotAngle = 0.0f;
	Boss.Radius = D3DXVec2Length(&temp);
	Boss.HitRadius = Boss.Radius / 2.0f;
	Boss.AnimPattern = 0;
	Boss.ShotCount = 0;
	Boss.MovePattern = 0;
	Boss.Count = 0;
	Boss.Color = 0;
	Boss.HP_Max = BossStartHP;
	Boss.HP = Boss.HP_Max;
	// ゲーム本編
	if (GameStage == Stage_Game)
	{
		// 中ボス設置
		if (Boss.MiddleBossOver == false)
		{
			switch (GameDifficulty)
			{
			case Easy:
				Boss.Phase = MiddleBoss_Phase1;
				Boss.Life = 0;
				break;
			case Normal:
				Boss.Phase = MiddleBoss_Phase1;
				Boss.Life = 1;
				break;
			case Hard:
				Boss.Phase = MiddleBoss_Phase1;
				Boss.Life = 1;
				break;
			default:
				break;
			}
		}
		// ラスボス設置
		else if (Boss.MiddleBossOver == true)
		{
			switch (GameDifficulty)
			{
			case Easy:
				Boss.Phase = LastBoss_Phase1;
				Boss.Life = 1;
				break;
			case Normal:
				Boss.Phase = LastBoss_Phase1;
				Boss.Life = 2;
				break;
			case Hard:
				Boss.Phase = LastBoss_Phase1;
				Boss.Life = 3;
				break;
			default:
				break;
			}
		}
		//if (Boss.State == BossDisappear)
		//{
		//	Boss.Phase = LastBoss_Phase1;
		//	Boss.Life = 3;
		//}
		//else
		//{
		//	Boss.Phase = MiddleBoss_Phase1;
		//	Boss.Life = 1;
		//}
	}
	// 練習弾幕選択
	else if (GameStage == Stage_PraticeSelect)
	{
		Boss.Life = 0;
	}
	// ボス練習モード
	else if (GameStage == Stage_BossPratice)
	{
		if (PraticeSelect != BossRush)
		{
			Boss.Life = 0;
		}
		else
		{
			Boss.Life = 5;
		}
	}
	Boss.BulletType = 0;
	Boss.State = StandBy;
	for (int i = 0; i < Boss.Life; i++)
	{
		BossLife[i].Use = true;
	}

	if (GameStage != Stage_PraticeSelect)
	{
		BossMove(D3DXVECTOR3(Screen_Width * 0.75, ScreenCenter_Y, 0.0f), 60.0f);

		SetBossEffect();
	}

#ifdef _DEBUG
	SetBossHC(Boss.Pos, Boss.HitRadius, BossHC);
#endif

	return;
}

//=============================================================================
// ボスの移動位置設置
//=============================================================================
void BossMove(D3DXVECTOR3 DestinationPos, float MoveTime)
{
	D3DXVECTOR2 Distance = D3DXVECTOR2(0.0f, 0.0f);

	Boss.InMove = true;
	Boss.BeforeMovePos = Boss.Pos;
	Boss.MoveCount = 0;
	Boss.MoveTime = MoveTime;
	Boss.Destination = D3DXVECTOR3(DestinationPos.x, DestinationPos.y, DestinationPos.z);
	Distance.x = Boss.Destination.x - Boss.Pos.x;
	Distance.y = Boss.Destination.y - Boss.Pos.y;
	Boss.Velocity.x = 2 * Distance.x / Boss.MoveTime;
	Boss.Velocity.y = 2 * Distance.y / Boss.MoveTime;
	Boss.Acceleration.x = -2 * Distance.x / (Boss.MoveTime * Boss.MoveTime);
	Boss.Acceleration.y = -2 * Distance.y / (Boss.MoveTime * Boss.MoveTime);

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeBossLifeVertex(void)
{
	int i = 0;

	// 頂点座標の設定	
	SetBossLifeVertex();

	// rhwの設定
	for (i = 0; i < BossLife_Max; i++)
	{
		BossLife[i].VertexWk[0].rhw = 1.0f;
		BossLife[i].VertexWk[1].rhw = 1.0f;
		BossLife[i].VertexWk[2].rhw = 1.0f;
		BossLife[i].VertexWk[3].rhw = 1.0f;
	}

	// 反射光の設定
	for (i = 0; i < BossLife_Max; i++)
	{
		BossLife[i].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		BossLife[i].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		BossLife[i].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		BossLife[i].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// テクスチャ座標の設定
	SetBossLifeTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBossLifeTexture(void)
{
	for (int i = 0; i < Boss.Life; i++)
	{
		BossLife[i].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		BossLife[i].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		BossLife[i].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		BossLife[i].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBossLifeVertex(void)
{
	for (int i = 0; i < Boss.Life; i++)
	{
		BossLife[i].VertexWk[0].vtx =
			D3DXVECTOR3(BossLifePos_X, BossLifePos_Y + (float)Texture_BossLife_Height * i, 0);
		BossLife[i].VertexWk[1].vtx =
			D3DXVECTOR3(BossLifePos_X + Texture_BossLife_Width, BossLifePos_Y + (float)Texture_BossLife_Height * i, 0);
		BossLife[i].VertexWk[2].vtx =
			D3DXVECTOR3(BossLifePos_X, BossLifePos_Y + Texture_BossLife_Height + (float)Texture_BossLife_Height * i, 0);
		BossLife[i].VertexWk[3].vtx =
			D3DXVECTOR3(BossLifePos_X + Texture_BossLife_Width, BossLifePos_Y + Texture_BossLife_Height + (float)Texture_BossLife_Height * i, 0);
	}

	return;
}
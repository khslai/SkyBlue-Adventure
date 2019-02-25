//=============================================================================
//
// ゲージ処理 [Gauge.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Gauge.h"
#include "Score.h"
#include "Player.h"
#include "Boss.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ゲージテクスチャの最大数
#define GaugeTexture_Max	(3)

// ゲージの種類
enum
{
	PlayerHP,			// プレイヤーのHP
	PlayerPower,		// プレイヤーのPower
	BossHP,				// ボスのHP
	PlayerHPBox,		// プレイヤーHPゲージの枠
	PlayerPowerBox,		// プレイヤーPowerゲージの枠
	BossHPBox,			// ボスHPゲージの枠
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeGaugeVertex(GAUGE *GaugePtr);
HRESULT MakeSymbolVertex(void);
// 頂点座標の設定
void SetGaugeVertex(GAUGE *GaugePtr);
void SetSymbolVertex(void);
// テクスチャ座標の設定
void SetGaugeTexture(GAUGE *GaugePtr);
void SetSymbolTexture(void);
// 色、透明度の設定
void SetGaugeDiffuse(GAUGE *GaugePtr, int Color);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 GaugeBoxTexture[GaugeTexture_Max] = { NULL };
LPDIRECT3DTEXTURE9 GaugeTexture[GaugeTexture_Max] = { NULL };
LPDIRECT3DTEXTURE9 BombSymbolTexture = NULL;
// ゲージ構造体
GAUGE PlayerHPGauge;
GAUGE PlayerHPGaugeBox;
GAUGE PlayerPowerGauge;
GAUGE PlayerPowerGaugeBox;
GAUGE PlayerBombSymbol[PlayerBomb_Max];
GAUGE BossHPGauge;
GAUGE BossHPGaugeBox;
// ゲージのパーセント
float GaugePercent_PlayerHP = 1.0f;
float GaugePercent_PlayerPower = PlayerStartPower / PlayerPower_Max;
float GaugePercent_BossHP = 1.0f;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGauge(int InitState)
{
	PlayerHPGauge.Type = PlayerHP;
	PlayerHPGaugeBox.Type = PlayerHPBox;
	PlayerPowerGauge.Type = PlayerPower;
	PlayerPowerGaugeBox.Type = PlayerPowerBox;
	BossHPGauge.Type = BossHP;
	BossHPGaugeBox.Type = BossHPBox;

	GaugePercent_PlayerHP = 1.0f;
	GaugePercent_PlayerPower = PlayerStartPower / PlayerPower_Max;
	GaugePercent_BossHP = 1.0f;

	// 頂点情報の作成
	MakeGaugeVertex(&PlayerHPGauge);
	MakeGaugeVertex(&PlayerHPGaugeBox);
	MakeGaugeVertex(&PlayerPowerGauge);
	MakeGaugeVertex(&PlayerPowerGaugeBox);
	MakeGaugeVertex(&BossHPGauge);
	MakeGaugeVertex(&BossHPGaugeBox);
	MakeSymbolVertex();

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_Gauge_Player, &GaugeTexture[PlayerHP], "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GaugeBox_PlayerHP, &GaugeBoxTexture[PlayerHP], "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Gauge_Player, &GaugeTexture[PlayerPower], "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GaugeBox_PlayerPower, &GaugeBoxTexture[PlayerPower], "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BombSymbol, &BombSymbolTexture, "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Gauge_Boss, &GaugeTexture[BossHP], "Boss") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GaugeBox_Boss, &GaugeBoxTexture[BossHP], "Boss") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGauge(void)
{
	for (int i = 0; i < GaugeTexture_Max; i++)
	{
		SafeRelease(GaugeTexture[i]);
		SafeRelease(GaugeBoxTexture[i]);
	}
	SafeRelease(BombSymbolTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGauge(void)
{
	int GameCount = GetGameCount();
	static int TimeCount = 0;
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();

	// プレイヤーHPゲージ
	// 緑色
	if (GaugePercent_PlayerHP >= 0.6f)
	{
		SetGaugeDiffuse(&PlayerHPGauge, Green);
		StopSound(SE_HPAlarm);
	}
	// 黄色
	else if (GaugePercent_PlayerHP > 0.2f && GaugePercent_PlayerHP < 0.6f)
	{
		SetGaugeDiffuse(&PlayerHPGauge, Yellow);
		StopSound(SE_HPAlarm);
	}
	// 赤色
	else
	{
		if (GameCount >= GameClearCount)
		{
			StopSound(SE_HPAlarm);
		}
		else
		{
			PlaySound(SE_HPAlarm, E_DS8_FLAG_LOOP, false);
		}
		TimeCount++;
		if (TimeCount < 10)
		{
			SetGaugeDiffuse(&PlayerHPGauge, Red);
		}
		else if (TimeCount >= 10 && TimeCount < 20)
		{
			SetGaugeDiffuse(&PlayerHPGauge, Red_AlphaHalf);
		}
		else if (TimeCount >= 20)
		{
			TimeCount = 0;
		}
	}

	SetGaugeTexture(&PlayerHPGauge);
	SetGaugeVertex(&PlayerHPGauge);

	SetGaugeTexture(&PlayerHPGaugeBox);
	SetGaugeVertex(&PlayerHPGaugeBox);

	// プレイヤーパワーゲージ
	if (GaugePercent_PlayerPower < 1.0f)
	{
		SetGaugeDiffuse(&PlayerPowerGauge, SkyBlue);
	}
	else
	{
		SetGaugeDiffuse(&PlayerPowerGauge, PowerGaugeGradation);
	}

	SetGaugeTexture(&PlayerPowerGauge);
	SetGaugeVertex(&PlayerPowerGauge);

	SetGaugeTexture(&PlayerPowerGaugeBox);
	SetGaugeVertex(&PlayerPowerGaugeBox);

	// プレイヤーボム
	SetSymbolTexture();

	// ボスHPゲージ
	if (Boss->Exist == true)
	{
		if (GaugePercent_BossHP >= 0.6f)
		{
			SetGaugeDiffuse(&BossHPGauge, Green);
		}
		else if (GaugePercent_BossHP > 0.2f && GaugePercent_BossHP < 0.6f)
		{
			SetGaugeDiffuse(&BossHPGauge, Yellow);
		}
		else
		{
			SetGaugeDiffuse(&BossHPGauge, Red);
		}

		SetGaugeTexture(&BossHPGauge);
		SetGaugeVertex(&BossHPGauge);

		SetGaugeTexture(&BossHPGaugeBox);
		SetGaugeVertex(&BossHPGaugeBox);

	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();

	Device->SetFVF(FVF_VERTEX_2D);

	Device->SetTexture(0, GaugeTexture[PlayerHP]);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PlayerHPGauge.VertexWk, sizeof(VERTEX_2D));

	Device->SetTexture(0, GaugeBoxTexture[PlayerHP]);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PlayerHPGaugeBox.VertexWk, sizeof(VERTEX_2D));

	Device->SetTexture(0, GaugeTexture[PlayerPower]);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PlayerPowerGauge.VertexWk, sizeof(VERTEX_2D));

	Device->SetTexture(0, GaugeBoxTexture[PlayerPower]);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PlayerPowerGaugeBox.VertexWk, sizeof(VERTEX_2D));

	for (int i = 0; i < PlayerBomb_Max; i++)
	{
		Device->SetTexture(0, BombSymbolTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PlayerBombSymbol[i].VertexWk, sizeof(VERTEX_2D));
	}

	if (Boss->Exist == true && Boss->State != BossDisappear)
	{
		Device->SetTexture(0, GaugeBoxTexture[BossHP]);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BossHPGaugeBox.VertexWk, sizeof(VERTEX_2D));

		Device->SetTexture(0, GaugeTexture[BossHP]);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BossHPGauge.VertexWk, sizeof(VERTEX_2D));
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeGaugeVertex(GAUGE *GaugePtr)
{
	// 頂点座標の設定	
	SetGaugeVertex(GaugePtr);

	// rhwの設定
	GaugePtr->VertexWk[0].rhw = 1.0f;
	GaugePtr->VertexWk[1].rhw = 1.0f;
	GaugePtr->VertexWk[2].rhw = 1.0f;
	GaugePtr->VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	GaugePtr->VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	GaugePtr->VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	GaugePtr->VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	GaugePtr->VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetGaugeTexture(GaugePtr);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetGaugeTexture(GAUGE *GaugePtr)
{
	GaugePtr->VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	GaugePtr->VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	GaugePtr->VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	GaugePtr->VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}



//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetGaugeVertex(GAUGE *GaugePtr)
{
	int GameStage = GetGameStage();
	float PlayerHPPercent = 0.0f;
	float PlayerPowerPercent = 0.0f;
	float BossHPPercent = 0.0f;
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();

	switch (GaugePtr->Type)
	{
	case PlayerHP:
		// 現在プレイヤーHPの割合
		PlayerHPPercent = (float)Player->HP / (float)PlayerHP_Max;

		// 1フレイム前のHP %は現在より多ければ、少しずつ減る
		if (GaugePercent_PlayerHP > PlayerHPPercent)
		{
			GaugePercent_PlayerHP -= 0.01f;
			// ゲージ固定
			if (GaugePercent_PlayerHP <= PlayerHPPercent)
			{
				GaugePercent_PlayerHP = PlayerHPPercent;
			}
			// ゲージが0になったら、ゲームオーバー
			if (GaugePercent_PlayerHP <= 0.0f)
			{
				GaugePercent_PlayerHP = 0.0f;
				SetGameStage(Stage_GameOver);
			}
		}
		// 1フレイム前のHP %は現在より少なければ、少しずつ増やす
		else if (GaugePercent_PlayerHP < PlayerHPPercent)
		{
			GaugePercent_PlayerHP += 0.01f;
			// ゲージ固定
			if (GaugePercent_PlayerHP >= PlayerHPPercent)
			{
				GaugePercent_PlayerHP = PlayerHPPercent;
			}
			//　100％超えたら、100％になる
			if (GaugePercent_PlayerHP >= 1.0f)
			{
				GaugePercent_PlayerHP = 1.0f;
			}
		}
		GaugePtr->VertexWk[0].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X, PlayerHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[1].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X + Texture_Gauge_Player_Width * GaugePercent_PlayerHP, PlayerHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[2].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X, PlayerHPGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		GaugePtr->VertexWk[3].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X + Texture_Gauge_Player_Width * GaugePercent_PlayerHP, PlayerHPGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		break;
	case PlayerHPBox:
		GaugePtr->VertexWk[0].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X, PlayerHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[1].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X + Texture_Gauge_Player_Width, PlayerHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[2].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X, PlayerHPGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		GaugePtr->VertexWk[3].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X + Texture_Gauge_Player_Width, PlayerHPGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		break;
	case PlayerPower:
		// 現在プレイヤーPowerの割合
		PlayerPowerPercent = (float)Player->Power / (float)PlayerPower_Max;

		// 1フレイム前のPower %は現在より多ければ、少しずつ減る
		if (GaugePercent_PlayerPower > PlayerPowerPercent)
		{
			GaugePercent_PlayerPower -= 0.01f;
			// ゲージ固定
			if (GaugePercent_PlayerPower <= PlayerPowerPercent)
			{
				GaugePercent_PlayerPower = PlayerPowerPercent;
			}
			//　0%より小さい、0になる
			if (GaugePercent_PlayerPower <= 0.0f)
			{
				GaugePercent_PlayerPower = 0.0f;
			}
		}
		// 1フレイム前のPower %は現在より少なければ、少しずつ増やす
		else if (GaugePercent_PlayerPower < PlayerPowerPercent)
		{
			GaugePercent_PlayerPower += 0.01f;
			// ゲージ固定
			if (GaugePercent_PlayerPower >= PlayerPowerPercent)
			{
				GaugePercent_PlayerPower = PlayerPowerPercent;
			}
			//　100％超えたら、100％になる
			if (GaugePercent_PlayerPower >= 1.0f)
			{
				GaugePercent_PlayerPower = 1.0f;
			}
		}
		GaugePtr->VertexWk[0].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X, PlayerPowerGauge_Pos_Y, 0);
		GaugePtr->VertexWk[1].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X + Texture_Gauge_Player_Width * GaugePercent_PlayerPower, PlayerPowerGauge_Pos_Y, 0);
		GaugePtr->VertexWk[2].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X, PlayerPowerGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		GaugePtr->VertexWk[3].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X + Texture_Gauge_Player_Width * GaugePercent_PlayerPower, PlayerPowerGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		break;
	case PlayerPowerBox:
		GaugePtr->VertexWk[0].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X, PlayerPowerGauge_Pos_Y, 0);
		GaugePtr->VertexWk[1].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X + Texture_Gauge_Player_Width, PlayerPowerGauge_Pos_Y, 0);
		GaugePtr->VertexWk[2].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X, PlayerPowerGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		GaugePtr->VertexWk[3].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X + Texture_Gauge_Player_Width, PlayerPowerGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		break;
	case BossHP:
		// 現在ボスHPの割合
		BossHPPercent = (float)Boss->HP / (float)Boss->HP_Max;

		// 1フレイム前のHP %は現在より多ければ、少しずつ減る
		if (GaugePercent_BossHP > BossHPPercent && Boss->State != HPRecover && Boss->State != BossDisappear)
		{
			GaugePercent_BossHP -= 0.01f;
			// ゲージ固定
			if (GaugePercent_BossHP <= BossHPPercent)
			{
				GaugePercent_BossHP = BossHPPercent;
			}
			// ゲージが0になったら、ボスは次の段階を入る
			if (GaugePercent_BossHP <= 0.0f)
			{
				PlaySound(SE_BossHPZero, E_DS8_FLAG_NONE, true);
				GaugePercent_BossHP = 0.0f;
				Boss->State = HPZero;
				// 残機があれば
				if (Boss->Life == 0)
				{
					if (GameStage == Stage_Game)
					{
						// 今のは中ボス
						if (Boss->MiddleBossOver == false)
						{
							// 中ボス終了
							Boss->MiddleBossOver = true;
							Boss->State = MiddleBossOver;
							GaugePercent_BossHP = 1.0f;
						}
						// 今のはラスボス
						else if (Boss->MiddleBossOver == true)
						{
							// ラスボス終了
							Boss->State = LastBossOver;
							GaugePercent_BossHP = 1.0f;
						}
						// // 今のはラスボス
						//if (Boss->Phase == LastBoss_LastPhase)
						//{
						//	 ラスボス終了
						//	Boss->State = LastBossOver;
						//	GaugePercent_BossHP = 1.0f;
						//}
						//else if (Boss->Phase == MiddleBoss_LastPhase)
						//{
						//	// 中ボス終了
						//	Boss->State = MiddleBossOver;
						//	GaugePercent_BossHP = 1.0f;
						//}
					}
					else if (GameStage == Stage_BossPratice)
					{
						// ボス練習モード終了
						Boss->State = LastBossOver;
					}
				}
			}
		}
		// HP回復
		else if (Boss->State == HPRecover)
		{
			GaugePercent_BossHP += 0.01f;
			// 100%になったら、次の段階を入る
			if (GaugePercent_BossHP >= 1.0f)
			{
				GaugePercent_BossHP = 1.0f;
				Boss->State = NextPhase;
			}
		}
		GaugePtr->VertexWk[0].vtx = D3DXVECTOR3(BossHPGauge_Pos_X, BossHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[1].vtx = D3DXVECTOR3(BossHPGauge_Pos_X + Texture_Gauge_Boss_Width, BossHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[2].vtx = D3DXVECTOR3(BossHPGauge_Pos_X, BossHPGauge_Pos_Y - Texture_Gauge_Boss_Height * GaugePercent_BossHP, 0);
		GaugePtr->VertexWk[3].vtx = D3DXVECTOR3(BossHPGauge_Pos_X + Texture_Gauge_Boss_Width, BossHPGauge_Pos_Y - Texture_Gauge_Boss_Height * GaugePercent_BossHP, 0);
		break;
	case BossHPBox:
		GaugePtr->VertexWk[0].vtx = D3DXVECTOR3(BossHPGauge_Pos_X, BossHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[1].vtx = D3DXVECTOR3(BossHPGauge_Pos_X + Texture_Gauge_Boss_Width, BossHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[2].vtx = D3DXVECTOR3(BossHPGauge_Pos_X, BossHPGauge_Pos_Y - Texture_Gauge_Boss_Height, 0);
		GaugePtr->VertexWk[3].vtx = D3DXVECTOR3(BossHPGauge_Pos_X + Texture_Gauge_Boss_Width, BossHPGauge_Pos_Y - Texture_Gauge_Boss_Height, 0);
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// 色、透明度の設定
//=============================================================================
void SetGaugeDiffuse(GAUGE *GaugePtr, int Color)
{
	switch (Color)
	{
	case Green:
		GaugePtr->VertexWk[0].diffuse = GREEN(255);
		GaugePtr->VertexWk[1].diffuse = GREEN(255);
		GaugePtr->VertexWk[2].diffuse = GREEN(255);
		GaugePtr->VertexWk[3].diffuse = GREEN(255);
		break;
	case Yellow:
		GaugePtr->VertexWk[0].diffuse = YELLOW(255);
		GaugePtr->VertexWk[1].diffuse = YELLOW(255);
		GaugePtr->VertexWk[2].diffuse = YELLOW(255);
		GaugePtr->VertexWk[3].diffuse = YELLOW(255);
		break;
	case Red:
		GaugePtr->VertexWk[0].diffuse = RED(255);
		GaugePtr->VertexWk[1].diffuse = RED(255);
		GaugePtr->VertexWk[2].diffuse = RED(255);
		GaugePtr->VertexWk[3].diffuse = RED(255);
		break;
	case Red_AlphaHalf:
		GaugePtr->VertexWk[0].diffuse = RED(128);
		GaugePtr->VertexWk[1].diffuse = RED(128);
		GaugePtr->VertexWk[2].diffuse = RED(128);
		GaugePtr->VertexWk[3].diffuse = RED(128);
		break;
	case SkyBlue:
		GaugePtr->VertexWk[0].diffuse = SKYBLUE(255);
		GaugePtr->VertexWk[1].diffuse = SKYBLUE(255);
		GaugePtr->VertexWk[2].diffuse = SKYBLUE(255);
		GaugePtr->VertexWk[3].diffuse = SKYBLUE(255);
		break;
	case PowerGaugeGradation:
		GaugePtr->VertexWk[0].diffuse = SKYBLUE(255);
		GaugePtr->VertexWk[1].diffuse = BLUE(255);
		GaugePtr->VertexWk[2].diffuse = SKYBLUE(255);
		GaugePtr->VertexWk[3].diffuse = BLUE(255);
	default:
		break;
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeSymbolVertex(void)
{
	int i = 0;

	// 頂点座標の設定	
	SetSymbolTexture();

	// rhwの設定
	for (i = 0; i < PlayerBomb_Max; i++)
	{
		PlayerBombSymbol[i].VertexWk[0].rhw = 1.0f;
		PlayerBombSymbol[i].VertexWk[1].rhw = 1.0f;
		PlayerBombSymbol[i].VertexWk[2].rhw = 1.0f;
		PlayerBombSymbol[i].VertexWk[3].rhw = 1.0f;
	}

	// 反射光の設定
	for (i = 0; i < PlayerBomb_Max; i++)
	{
		PlayerBombSymbol[i].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		PlayerBombSymbol[i].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		PlayerBombSymbol[i].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		PlayerBombSymbol[i].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// テクスチャ座標の設定
	SetSymbolVertex();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetSymbolTexture(void)
{
	PLAYER *Player = GetPlayer(0);

	for (int i = 0; i < PlayerBomb_Max; i++)
	{
		if (i < Player->BombNum)
		{
			// ボムある
			PlayerBombSymbol[i].VertexWk[0].tex = D3DXVECTOR2(0.5f, 0.0f);
			PlayerBombSymbol[i].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			PlayerBombSymbol[i].VertexWk[2].tex = D3DXVECTOR2(0.5f, 1.0f);
			PlayerBombSymbol[i].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		else
		{
			// ボムなし
			PlayerBombSymbol[i].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			PlayerBombSymbol[i].VertexWk[1].tex = D3DXVECTOR2(0.5f, 0.0f);
			PlayerBombSymbol[i].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			PlayerBombSymbol[i].VertexWk[3].tex = D3DXVECTOR2(0.5f, 1.0f);
		}
	}

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetSymbolVertex(void)
{
	for (int i = 0; i < PlayerBomb_Max; i++)
	{
		PlayerBombSymbol[i].VertexWk[0].vtx =
			D3DXVECTOR3((float)PlayerBombSymbol_Pos_X + i * Texture_BombSymbol_Width, PlayerBombSymbol_Pos_Y, 0);
		PlayerBombSymbol[i].VertexWk[1].vtx =
			D3DXVECTOR3((float)PlayerBombSymbol_Pos_X + (i + 1) * Texture_BombSymbol_Width, PlayerBombSymbol_Pos_Y, 0);
		PlayerBombSymbol[i].VertexWk[2].vtx =
			D3DXVECTOR3((float)PlayerBombSymbol_Pos_X + i * Texture_BombSymbol_Width, PlayerBombSymbol_Pos_Y + Texture_BombSymbol_Height, 0);
		PlayerBombSymbol[i].VertexWk[3].vtx =
			D3DXVECTOR3((float)PlayerBombSymbol_Pos_X + (i + 1) * Texture_BombSymbol_Width, PlayerBombSymbol_Pos_Y + Texture_BombSymbol_Height, 0);
	}

	return;
}

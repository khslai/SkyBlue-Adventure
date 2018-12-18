//=============================================================================
//
// 練習弾幕選択処理 [PraticeSelect.cpp]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#include "main.h"
#include "PraticeSelect.h"
#include "Title.h"
#include "Boss.h"
#include "BossBarrage.h"
#include "Laser.h"
#include "SelectChar.h"
#include "Transition.h"
#include "Sound.h"
#include "Input.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define UpDownArrowPos_Y (200)
#define RectHeight (100)
#define TitlePos_Y (50)
#define OptionPos_Y	(150)
#define ReferencePos_Y (170)




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakePraticeSelectVertex(void);
void SetPraticeSelectTexture(void);
void SetPraticeSelectVertex(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
VERTEX_2D PraticeSelectBGVertexWk[NUM_VERTEX];
VERTEX_2D PraticeSelectDrawBoxVertexWk[NUM_VERTEX];
VERTEX_2D UpDownArrowVertexWk[NUM_VERTEX];
LPDIRECT3DTEXTURE9 PraticeSelectSelectBGTexture = NULL;
LPDIRECT3DTEXTURE9 PraticeSelectDrawBoxTexture = NULL;
LPDIRECT3DTEXTURE9 UpDownArrowTexture = NULL;
int PraticeSelectPhase = 0;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPraticeSelect(int InitState)
{
	int GameStage = GetGameStage();

	MakePraticeSelectVertex();

	if (GameStage != Stage_GameOver && GameStage != Stage_BossPratice)
	{
		PraticeSelectPhase = SetBossStart;
	}

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_SelectCharBG, &PraticeSelectSelectBGTexture, "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_DrawBox, &PraticeSelectDrawBoxTexture, "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_UpDownArrow, &UpDownArrowTexture, "BackGround") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPraticeSelect(void)
{
	SafeRelease(PraticeSelectSelectBGTexture);
	SafeRelease(PraticeSelectDrawBoxTexture);
	SafeRelease(UpDownArrowTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePraticeSelect(void)
{
	int Bullet_No = 0;
	int Laser_No = 0;
	BOSS *Boss = GetBoss();
	LASER *Laser = GetLaser(Laser_No);
	TRANSITION *Transition = GetTransition();

	// ボス設置
	if (Transition->FadeInOver != true)
	{
		if (PraticeSelectPhase == SetBossStart)
		{
			InitBoss(ReInit);
			SetBoss();
			Boss->Pos = D3DXVECTOR3(ScreenCenter_X, 500.0f, 0.0f);
			Boss->Phase = MiddleBoss_Phase1;
			PraticeSelectPhase = BossBarrage_M1;
		}
	}

	// 画面遷移、次のステージに移す
	if (Transition->FadeInOver == true && PraticeSelectPhase != SetBossStart)
	{
		InitBoss(ReInit);
		InitBossBarrage(ReInit);
		InitLaser(ReInit);
		InitSelectChar(ReInit);
		if (PraticeSelectPhase != BackToSelectChar)
		{
			SetGameStage(Stage_BossPratice);
		}
		else if (PraticeSelectPhase == BackToSelectChar)
		{
			SetGameStage(Stage_SelectChar);
			PraticeSelectPhase = SetBossStart;
		}
	}

	// Enterキー
	if (Transition->FadeInOver != true)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || GetKeyboardTrigger(DIK_Z) ||
			IsButtonTriggered(0, BUTTON_B))
		{
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			SetTransition();
		}
		if (GetKeyboardTrigger(DIK_X) || IsButtonTriggered(0, BUTTON_A))
		{
			PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
			SetTransition();
			PraticeSelectPhase = BackToSelectChar;
		}
	}

	// 選択肢更新
	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
	{
		PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
		// 弾を消す
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			Boss->Bullet[Bullet_No].Use = false;
		}
		// レーザーを消す
		Laser_No = 0;
		Laser = GetLaser(Laser_No);
		for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++, Laser++)
		{
			Laser->Use = false;
		}
		// ボスの状態を初期化
		Boss->Pos = D3DXVECTOR3(ScreenCenter_X, 500.0f, 0.0f);
		Boss->State = StandBy;
		Boss->ShotCount = 0;

		switch (PraticeSelectPhase)
		{
		case BossBarrage_M1:
			PraticeSelectPhase = BossBarrage_M2;
			Boss->Phase = MiddleBoss_LastPhase;
			break;
		case BossBarrage_M2:
			PraticeSelectPhase = BossBarrage_L1;
			Boss->Phase = LastBoss_Phase1;
			break;
		case BossBarrage_L1:
			PraticeSelectPhase = BossBarrage_L2;
			Boss->Phase = LastBoss_Phase2;
			break;
		case BossBarrage_L2:
			PraticeSelectPhase = BossBarrage_L3;
			Boss->Phase = LastBoss_Phase3;
			break;
		case BossBarrage_L3:
			PraticeSelectPhase = BossBarrage_L4;
			Boss->Phase = LastBoss_LastPhase;
			break;
		case BossBarrage_L4:
			PraticeSelectPhase = BossRush;
			Boss->Phase = 99;
			break;
		case BossRush:
			PraticeSelectPhase = BossBarrage_M1;
			Boss->Phase = MiddleBoss_Phase1;
			break;
		default:
			break;
		}
	}
	else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
	{
		PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
		// 弾を消す
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			Boss->Bullet[Bullet_No].Use = false;
		}
		// レーザーを消す
		Laser_No = 0;
		Laser = GetLaser(Laser_No);
		for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++, Laser++)
		{
			Laser->Use = false;
		}
		// ボスの状態を初期化
		Boss->Pos = D3DXVECTOR3(ScreenCenter_X, 500.0f, 0.0f);
		Boss->State = StandBy;
		Boss->ShotCount = 0;

		switch (PraticeSelectPhase)
		{
		case BossBarrage_M1:
			PraticeSelectPhase = BossRush;
			Boss->Phase = 99;
			break;
		case BossBarrage_M2:
			PraticeSelectPhase = BossBarrage_M1;
			Boss->Phase = MiddleBoss_Phase1;
			break;
		case BossBarrage_L1:
			PraticeSelectPhase = BossBarrage_M2;
			Boss->Phase = MiddleBoss_LastPhase;
			break;
		case BossBarrage_L2:
			PraticeSelectPhase = BossBarrage_L1;
			Boss->Phase = LastBoss_Phase1;
			break;
		case BossBarrage_L3:
			PraticeSelectPhase = BossBarrage_L2;
			Boss->Phase = LastBoss_Phase2;
			break;
		case BossBarrage_L4:
			PraticeSelectPhase = BossBarrage_L3;
			Boss->Phase = LastBoss_Phase3;
			break;
		case BossRush:
			PraticeSelectPhase = BossBarrage_L4;
			Boss->Phase = LastBoss_LastPhase;
			break;
		default:
			break;
		}
	}

	// テクスチャ更新
	SetPraticeSelectTexture();
	SetPraticeSelectVertex();

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPraticeSelect(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPD3DXFONT Title = GetFont(FontSize_54);
	LPD3DXFONT Option = GetFont(FontSize_40);
	LPD3DXFONT Information = GetFont(FontSize_30);
	RECT TextRect = { 0, 0, Screen_Width, RectHeight };

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, PraticeSelectSelectBGTexture);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PraticeSelectBGVertexWk, sizeof(VERTEX_2D));

	Device->SetTexture(0, PraticeSelectDrawBoxTexture);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PraticeSelectDrawBoxVertexWk, sizeof(VERTEX_2D));

	Device->SetTexture(0, UpDownArrowTexture);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, UpDownArrowVertexWk, sizeof(VERTEX_2D));

	TextRect.top = TitlePos_Y;
	TextRect.bottom = TitlePos_Y + RectHeight;
	Title->DrawText(NULL, "練習したい弾幕を選んでください", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));

	TextRect.top = OptionPos_Y;
	TextRect.bottom = OptionPos_Y + RectHeight;
	switch (PraticeSelectPhase)
	{
	case BossBarrage_M1:
		Option->DrawText(NULL, "中ボス第一段階", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	case BossBarrage_M2:
		Option->DrawText(NULL, "中ボス第二段階", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	case BossBarrage_L1:
		Option->DrawText(NULL, "ラスボス第一段階", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	case BossBarrage_L2:
		Option->DrawText(NULL, "ラスボス第二段階", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	case BossBarrage_L3:
		Option->DrawText(NULL, "ラスボス第三段階", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	case BossBarrage_L4:
		Option->DrawText(NULL, "ラスボス第四段階", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	case BossRush:
		Option->DrawText(NULL, "ボスラッシュ", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	default:
		break;
	}

	TextRect.top = ReferencePos_Y;
	TextRect.bottom = ReferencePos_Y + RectHeight;
	switch (PraticeSelectPhase)
	{
	case BossBarrage_M1:
		Information->DrawText(NULL, "オリジナル", -1, &TextRect, DT_RIGHT | DT_VCENTER, WHITE(255));
		break;
	case BossBarrage_M2:
		Information->DrawText(NULL, "参考：サイレントセレナ", -1, &TextRect, DT_RIGHT | DT_VCENTER, WHITE(255));
		break;
	case BossBarrage_L1:
		Information->DrawText(NULL, "参考：ミシャグジさま", -1, &TextRect, DT_RIGHT | DT_VCENTER, WHITE(255));
		break;
	case BossBarrage_L2:
		Information->DrawText(NULL, "参考：パーフェクトフリーズ", -1, &TextRect, DT_RIGHT | DT_VCENTER, WHITE(255));
		break;
	case BossBarrage_L3:
		Information->DrawText(NULL, "参考：四聖龍神録Plus", -1, &TextRect, DT_RIGHT | DT_VCENTER, WHITE(255));
		break;
	case BossBarrage_L4:
		Information->DrawText(NULL, "参考：反魂蝶-一分咲-", -1, &TextRect, DT_RIGHT | DT_VCENTER, WHITE(255));
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakePraticeSelectVertex(void)
{
	// 頂点座標の設定	
	SetPraticeSelectVertex();

	// rhwの設定
	PraticeSelectBGVertexWk[0].rhw = 1.0f;
	PraticeSelectBGVertexWk[1].rhw = 1.0f;
	PraticeSelectBGVertexWk[2].rhw = 1.0f;
	PraticeSelectBGVertexWk[3].rhw = 1.0f;

	PraticeSelectDrawBoxVertexWk[0].rhw = 1.0f;
	PraticeSelectDrawBoxVertexWk[1].rhw = 1.0f;
	PraticeSelectDrawBoxVertexWk[2].rhw = 1.0f;
	PraticeSelectDrawBoxVertexWk[3].rhw = 1.0f;

	UpDownArrowVertexWk[0].rhw = 1.0f;
	UpDownArrowVertexWk[1].rhw = 1.0f;
	UpDownArrowVertexWk[2].rhw = 1.0f;
	UpDownArrowVertexWk[3].rhw = 1.0f;

	// 反射光の設定
	PraticeSelectBGVertexWk[0].diffuse = WHITE(255);
	PraticeSelectBGVertexWk[1].diffuse = WHITE(255);
	PraticeSelectBGVertexWk[2].diffuse = WHITE(255);
	PraticeSelectBGVertexWk[3].diffuse = WHITE(255);

	PraticeSelectDrawBoxVertexWk[0].diffuse = SKYBLUE(255);
	PraticeSelectDrawBoxVertexWk[1].diffuse = SKYBLUE(255);
	PraticeSelectDrawBoxVertexWk[2].diffuse = SKYBLUE(255);
	PraticeSelectDrawBoxVertexWk[3].diffuse = SKYBLUE(255);

	UpDownArrowVertexWk[0].diffuse = WHITE(255);
	UpDownArrowVertexWk[1].diffuse = WHITE(255);
	UpDownArrowVertexWk[2].diffuse = WHITE(255);
	UpDownArrowVertexWk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetPraticeSelectTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetPraticeSelectTexture(void)
{
	PraticeSelectBGVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	PraticeSelectBGVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	PraticeSelectBGVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	PraticeSelectBGVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	PraticeSelectDrawBoxVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	PraticeSelectDrawBoxVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	PraticeSelectDrawBoxVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	PraticeSelectDrawBoxVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	UpDownArrowVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	UpDownArrowVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	UpDownArrowVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	UpDownArrowVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetPraticeSelectVertex(void)
{
	PraticeSelectBGVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PraticeSelectBGVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	PraticeSelectBGVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	PraticeSelectBGVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	PraticeSelectDrawBoxVertexWk[0].vtx = D3DXVECTOR3(DrawBox_X, DrawBox_Y, 0.0f);
	PraticeSelectDrawBoxVertexWk[1].vtx = D3DXVECTOR3(DrawBox_X + Texture_DrawBox_Width, DrawBox_Y, 0.0f);
	PraticeSelectDrawBoxVertexWk[2].vtx = D3DXVECTOR3(DrawBox_X, DrawBox_Y + Texture_DrawBox_Height, 0.0f);
	PraticeSelectDrawBoxVertexWk[3].vtx = D3DXVECTOR3(DrawBox_X + Texture_DrawBox_Width, DrawBox_Y + Texture_DrawBox_Height, 0.0f);

	UpDownArrowVertexWk[0].vtx = D3DXVECTOR3(ScreenCenter_X - Texture_UpDownArrow_Width / 2, UpDownArrowPos_Y - Texture_UpDownArrow_Height, 0.0f);
	UpDownArrowVertexWk[1].vtx = D3DXVECTOR3(ScreenCenter_X + Texture_UpDownArrow_Width / 2, UpDownArrowPos_Y - Texture_UpDownArrow_Height, 0.0f);
	UpDownArrowVertexWk[2].vtx = D3DXVECTOR3(ScreenCenter_X - Texture_UpDownArrow_Width / 2, UpDownArrowPos_Y + Texture_UpDownArrow_Height, 0.0f);
	UpDownArrowVertexWk[3].vtx = D3DXVECTOR3(ScreenCenter_X + Texture_UpDownArrow_Width / 2, UpDownArrowPos_Y + Texture_UpDownArrow_Height, 0.0f);

	return;
}

//=============================================================================
// 選んだ弾幕モードを取得する
//=============================================================================
int GetPraticeSelect(void)
{
	return PraticeSelectPhase;
}
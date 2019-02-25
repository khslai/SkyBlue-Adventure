//=============================================================================
//
// ボス練習モード処理 [BossPratice.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "BossPratice.h"
#include "PraticeSelect.h"
#include "BackGround.h"
#include "Transition.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Boss.h"
#include "BossBarrage.h"
#include "Laser.h"
#include "Sound.h"
#include "Input.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 練習モード背景スクロールの速度
#define BGSpeed				(0.001f)
// テキスト矩形の高さ
#define RectHeight			(100)
// テキスト表示の位置
#define PausePos_Y			(128)
#define ResumePos_Y			(350)
#define RestartPos_Y		(450)
#define ToTitlePos_Y		(550)
#define AskSentencePos_Y	(200)
#define YesNoPos_Y			(450)
#define YesPos_X			(256)
#define NoPos_X				(768)


// ボス練習モードの状態
enum BossPraticePhase
{
	SetBossPraticeStart,
	SetBossPraticeOver,
	Resume,
	Restart,
	RestartCheck,
	ToTitle,
	ToTitleCheck,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeBossPraticeVertex(void);
// 頂点座標の設定
void SetBossPraticeVertex(void);
// テクスチャ座標の設定
void SetBossPraticeTexture(void);
// プレイヤーとボスの設置
void SetBossPratice(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// メニュー画面頂点
static VERTEX_2D BlackScreenVertexWk[Num_Vertex];
// 背景テクスチャ
LPDIRECT3DTEXTURE9 BossPraticeBGTexture = NULL;
// 背景構造体
static BACKGROUND BossPraticeBG;
// 選択肢
static SELECT BossPraticeSelect;
// 画面揺れるの移動量
static D3DXVECTOR2 ShakePos = D3DXVECTOR2(0.0f, 0.0f);

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBossPratice(int InitState)
{
	BossPraticeBG.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ShakePos = D3DXVECTOR2(0.0f, 0.0f);

	BossPraticeSelect.Pos = D3DXVECTOR3(ScreenCenter_X, ResumePos_Y, 0.0f);
	BossPraticeSelect.Phase = SetBossPraticeStart;
	BossPraticeSelect.InYes = false;

	MakeBossPraticeVertex();

	if (InitState == FirstInit)
	{
		if (SafeLoad(Texture_BG_Day, &BossPraticeBGTexture, "BackGround") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBossPratice(void)
{
	SafeRelease(BossPraticeBGTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBossPratice(void)
{
	static int Count = 0;
	static int PressCount = 0;
	TRANSITION *Transition = GetTransition();
	BOSS *Boss = GetBoss();

	// プレイヤー、ボス設置
	if (BossPraticeSelect.Phase == SetBossPraticeStart)
	{
		ReInitSound();
		PlaySound(BGM_BossPratice, E_DS8_FLAG_LOOP, false);
		SetBossPratice();
		Count = 0;
		BossPraticeSelect.Phase = SetBossPraticeOver;
	}

	// 画面遷移
	if (Transition->FadeInOver == true && BossPraticeSelect.InYes == true)
	{
		// 最初から
		if (BossPraticeSelect.Phase == RestartCheck)
		{
			ReInitGame(true);
			SetGameStage(Stage_BossPratice);
		}
		// タイトルに戻る
		else if (BossPraticeSelect.Phase == ToTitleCheck)
		{
			ReInitGame(true);
			SetGameStage(Stage_Title);
		}

		return;
	}

	// 背景スクロール
	if (BossPraticeSelect.Phase == SetBossPraticeOver)
	{
		BossPraticeBG.Pos.x += BGSpeed;

		// ボスが倒された
		if (Boss->Exist == false)
		{
			// 練習モード終了
			if (Transition->FadeInOver == true)
			{
				ReInitGame(true);
				SetGameStage(Stage_Title);
			}

			// 画面揺れる
			if (Count <= 30)
			{
				BossPraticeBG.InShake = true;
			}
			else
			{
				BossPraticeBG.InShake = false;
			}

			// 揺れる座標計算
			if (BossPraticeBG.InShake == true)
			{
				ShakePos.x = (float)(rand() % (30 + 1) - 15);
				ShakePos.y = (float)(rand() % (30 + 1) - 15);
			}
			else
			{
				ShakePos.x = 0.0f;
				ShakePos.y = 0.0f;
			}

			// 遷移画面設置
			if (Count == 180)
			{
				SetTransition();
			}

			Count++;
		}
	}

	// 一時停止
	if ((GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_START)) && BossPraticeSelect.Phase == SetBossPraticeOver)
	{
		PlaySound(SE_Menu, E_DS8_FLAG_NONE, true);
		BossPraticeSelect.Phase = Resume;
		BossPraticeSelect.Pos.x = ScreenCenter_X;
		BossPraticeSelect.Pos.y = ResumePos_Y;

	}

	// Enterキー
	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || GetKeyboardTrigger(DIK_Z) ||
		IsButtonTriggered(0, BUTTON_B))
	{
		switch (BossPraticeSelect.Phase)
		{
		case Resume:
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			BossPraticeSelect.Phase = SetBossPraticeOver;
			BossPraticeSelect.Pos.y = ResumePos_Y;
			return;
			break;
		case Restart:
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			BossPraticeSelect.Phase = RestartCheck;
			BossPraticeSelect.Pos.x = NoPos_X;
			BossPraticeSelect.Pos.y = YesNoPos_Y;
			break;
		case RestartCheck:
			if (BossPraticeSelect.InYes == true)
			{
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				SetTransition();
			}
			else
			{
				PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
				BossPraticeSelect.Phase = Restart;
				BossPraticeSelect.Pos.x = ScreenCenter_X;
				BossPraticeSelect.Pos.y = RestartPos_Y;
			}
			break;
		case ToTitle:
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			BossPraticeSelect.Phase = ToTitleCheck;
			BossPraticeSelect.Pos.x = NoPos_X;
			BossPraticeSelect.Pos.y = YesNoPos_Y;
			break;
		case ToTitleCheck:
			if (BossPraticeSelect.InYes == true)
			{
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				SetTransition();
			}
			else
			{
				PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
				BossPraticeSelect.Phase = ToTitle;
				BossPraticeSelect.Pos.x = ScreenCenter_X;
				BossPraticeSelect.Pos.y = ToTitlePos_Y;
			}
			break;
		default:
			break;
		}
	}

	// 選択肢移動
	if (BossPraticeSelect.Phase == Resume || BossPraticeSelect.Phase == Restart || BossPraticeSelect.Phase == ToTitle)
	{
		if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			switch (BossPraticeSelect.Phase)
			{
			case Resume:
				BossPraticeSelect.Phase = Restart;
				BossPraticeSelect.Pos.y = RestartPos_Y;
				break;
			case Restart:
				BossPraticeSelect.Phase = ToTitle;
				BossPraticeSelect.Pos.y = ToTitlePos_Y;
				break;
			case ToTitle:
				BossPraticeSelect.Phase = Resume;
				BossPraticeSelect.Pos.y = ResumePos_Y;
				break;
			default:
				break;
			}
		}
		else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			switch (BossPraticeSelect.Phase)
			{
			case Resume:
				BossPraticeSelect.Phase = ToTitle;
				BossPraticeSelect.Pos.y = ToTitlePos_Y;
				break;
			case Restart:
				BossPraticeSelect.Phase = Resume;
				BossPraticeSelect.Pos.y = ResumePos_Y;
				break;
			case ToTitle:
				BossPraticeSelect.Phase = Restart;
				BossPraticeSelect.Pos.y = RestartPos_Y;
				break;
			default:
				break;
			}
		}

		// 選択肢ループ
		if (GetKeyboardRepeat(DIK_DOWN) || IsButtonRepeat(0, BUTTON_DOWN))
		{
			PressCount++;
			if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				if (BossPraticeSelect.Phase == Resume)
				{
					BossPraticeSelect.Phase = Restart;
					BossPraticeSelect.Pos.y = RestartPos_Y;
				}
				else if (BossPraticeSelect.Phase == Restart)
				{
					BossPraticeSelect.Phase = ToTitle;
					BossPraticeSelect.Pos.y = ToTitlePos_Y;
				}
				else if (BossPraticeSelect.Phase == ToTitle)
				{
					BossPraticeSelect.Phase = Resume;
					BossPraticeSelect.Pos.y = ResumePos_Y;
				}
			}
		}
		else if (GetKeyboardRepeat(DIK_UP) || IsButtonRepeat(0, BUTTON_UP))
		{
			PressCount++;
			if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				if (BossPraticeSelect.Phase == Resume)
				{
					BossPraticeSelect.Phase = ToTitle;
					BossPraticeSelect.Pos.y = ToTitlePos_Y;
				}
				else if (BossPraticeSelect.Phase == Restart)
				{
					BossPraticeSelect.Phase = Resume;
					BossPraticeSelect.Pos.y = ResumePos_Y;
				}
				else if (BossPraticeSelect.Phase == ToTitle)
				{
					BossPraticeSelect.Phase = Restart;
					BossPraticeSelect.Pos.y = RestartPos_Y;
				}
			}
		}


		if (GetKeyboardRelease(DIK_UP) || GetKeyboardRelease(DIK_DOWN) ||
			IsButtonReleased(0, BUTTON_UP) || IsButtonReleased(0, BUTTON_DOWN))
		{
			PressCount = 0;
		}
	}

	// はい、いいえ更新
	if (BossPraticeSelect.Phase == RestartCheck || BossPraticeSelect.Phase == ToTitleCheck)
	{
		if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			BossPraticeSelect.Pos.x = YesPos_X;
			BossPraticeSelect.InYes = true;
		}
		else if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			BossPraticeSelect.Pos.x = NoPos_X;
			BossPraticeSelect.InYes = false;
		}
	}

	// テクスチャ更新
	SetBossPraticeTexture();
	SetBossPraticeVertex();

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBossPratice(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, BossPraticeBGTexture);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BossPraticeBG.VertexWk, sizeof(VERTEX_2D));

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBossPraticeMenu(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPDIRECT3DTEXTURE9 BlackScreenTexture = GetUITexture(BlackScreen);
	LPDIRECT3DTEXTURE9 BossPraticeSelectTexture = GetUITexture(SelectBox);
	LPD3DXFONT Pause = GetFont(FontSize_108);
	LPD3DXFONT Option = GetFont(FontSize_54);
	RECT TextRect = { 0, 0, Screen_Width, RectHeight };

	if (BossPraticeSelect.Phase != SetBossPraticeStart && BossPraticeSelect.Phase != SetBossPraticeOver)
	{
		// 黒い画面描画
		Device->SetTexture(0, BlackScreenTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BlackScreenVertexWk, sizeof(VERTEX_2D));

		// 選択枠描画
		Device->SetTexture(0, BossPraticeSelectTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BossPraticeSelect.VertexWk, sizeof(VERTEX_2D));

		if (BossPraticeSelect.Phase != RestartCheck && BossPraticeSelect.Phase != ToTitleCheck)
		{
			TextRect.top = PausePos_Y;
			TextRect.bottom = PausePos_Y + RectHeight;
			Pause->DrawText(NULL, "P A U S E", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));

			TextRect.top = ResumePos_Y;
			TextRect.bottom = ResumePos_Y + RectHeight;
			if (BossPraticeSelect.Phase == Resume)
			{
				Option->DrawText(NULL, "続く", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Option->DrawText(NULL, "続く", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}

			TextRect.top = RestartPos_Y;
			TextRect.bottom = RestartPos_Y + RectHeight;
			if (BossPraticeSelect.Phase == Restart)
			{
				Option->DrawText(NULL, "最初から", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Option->DrawText(NULL, "最初から", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}

			TextRect.top = ToTitlePos_Y;
			TextRect.bottom = ToTitlePos_Y + RectHeight;
			if (BossPraticeSelect.Phase == ToTitle)
			{
				Option->DrawText(NULL, "タイトルへ", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Option->DrawText(NULL, "タイトルへ", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
		}
		else if (BossPraticeSelect.Phase == RestartCheck || BossPraticeSelect.Phase == ToTitleCheck)
		{
			if (BossPraticeSelect.Phase == RestartCheck)
			{
				TextRect.top = AskSentencePos_Y;
				TextRect.bottom = AskSentencePos_Y + RectHeight;
				Option->DrawText(NULL, "最初からよろしですか？", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			else if (BossPraticeSelect.Phase == ToTitleCheck)
			{
				TextRect.top = AskSentencePos_Y;
				TextRect.bottom = AskSentencePos_Y + RectHeight;
				Option->DrawText(NULL, "タイトルに戻りますか？", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}

			TextRect.top = YesNoPos_Y;
			TextRect.bottom = YesNoPos_Y + RectHeight;
			TextRect.left = 0;
			TextRect.right = ScreenCenter_X;
			if (BossPraticeSelect.InYes == true)
			{
				Option->DrawText(NULL, "はい", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Option->DrawText(NULL, "はい", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			TextRect.left = ScreenCenter_X;
			TextRect.right = Screen_Width;
			if (BossPraticeSelect.InYes == true)
			{
				Option->DrawText(NULL, "いいえ", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			else
			{
				Option->DrawText(NULL, "いいえ", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
		}
	}

	return;
}

//=============================================================================
// プレイヤーとボスの設置
//=============================================================================
void SetBossPratice(void)
{
	int PraticeSelect = GetPraticeSelect();
	BOSS *Boss = GetBoss();

	SetPlayer();

	SetBoss();
	switch (PraticeSelect)
	{
	case BossBarrage_M1:
		Boss->Phase = MiddleBoss_Phase1;
		break;
	case BossBarrage_M2:
		Boss->Phase = MiddleBoss_LastPhase;
		break;
	case BossBarrage_L1:
		Boss->Phase = LastBoss_Phase1;
		break;
	case BossBarrage_L2:
		Boss->Phase = LastBoss_Phase2;
		break;
	case BossBarrage_L3:
		Boss->Phase = LastBoss_Phase3;
		break;
	case BossBarrage_L4:
		Boss->Phase = LastBoss_LastPhase;
		break;
	case BossRush:
		Boss->Phase = MiddleBoss_Phase1;
		break;
	default:
		break;
	}


	return;
}

//=============================================================================
// チュートリアルの状態を取得する
//=============================================================================
bool GetExitPratice(void)
{
	if (BossPraticeSelect.Phase != SetBossPraticeStart && BossPraticeSelect.Phase != SetBossPraticeOver)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeBossPraticeVertex(void)
{
	// 頂点座標の設定	
	SetBossPraticeVertex();

	// rhwの設定
	BossPraticeBG.VertexWk[0].rhw = 1.0f;
	BossPraticeBG.VertexWk[1].rhw = 1.0f;
	BossPraticeBG.VertexWk[2].rhw = 1.0f;
	BossPraticeBG.VertexWk[3].rhw = 1.0f;

	BlackScreenVertexWk[0].rhw = 1.0f;
	BlackScreenVertexWk[1].rhw = 1.0f;
	BlackScreenVertexWk[2].rhw = 1.0f;
	BlackScreenVertexWk[3].rhw = 1.0f;

	BossPraticeSelect.VertexWk[0].rhw = 1.0f;
	BossPraticeSelect.VertexWk[1].rhw = 1.0f;
	BossPraticeSelect.VertexWk[2].rhw = 1.0f;
	BossPraticeSelect.VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	BossPraticeBG.VertexWk[0].diffuse = WHITE(255);
	BossPraticeBG.VertexWk[1].diffuse = WHITE(255);
	BossPraticeBG.VertexWk[2].diffuse = WHITE(255);
	BossPraticeBG.VertexWk[3].diffuse = WHITE(255);

	BlackScreenVertexWk[0].diffuse = WHITE(200);
	BlackScreenVertexWk[1].diffuse = WHITE(200);
	BlackScreenVertexWk[2].diffuse = WHITE(200);
	BlackScreenVertexWk[3].diffuse = WHITE(200);

	BossPraticeSelect.VertexWk[0].diffuse = WHITE(255);
	BossPraticeSelect.VertexWk[1].diffuse = WHITE(255);
	BossPraticeSelect.VertexWk[2].diffuse = WHITE(255);
	BossPraticeSelect.VertexWk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetBossPraticeTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBossPraticeTexture(void)
{
	float BGRate_X = (float)Screen_Width / (float)Texture_BG_Width;

	BossPraticeBG.VertexWk[0].tex = D3DXVECTOR2(BossPraticeBG.Pos.x, 0.0f);
	BossPraticeBG.VertexWk[1].tex = D3DXVECTOR2(BossPraticeBG.Pos.x + BGRate_X, 0.0f);
	BossPraticeBG.VertexWk[2].tex = D3DXVECTOR2(BossPraticeBG.Pos.x, 1.0f);
	BossPraticeBG.VertexWk[3].tex = D3DXVECTOR2(BossPraticeBG.Pos.x + BGRate_X, 1.0f);

	BlackScreenVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	BlackScreenVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	BlackScreenVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	BlackScreenVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	BossPraticeSelect.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	BossPraticeSelect.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	BossPraticeSelect.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	BossPraticeSelect.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBossPraticeVertex(void)
{
	BossPraticeBG.VertexWk[0].vtx = D3DXVECTOR3(0.0f + ShakePos.x, 0.0f + ShakePos.y, 0.0f);
	BossPraticeBG.VertexWk[1].vtx = D3DXVECTOR3(Screen_Width + ShakePos.x, 0.0f + ShakePos.y, 0.0f);
	BossPraticeBG.VertexWk[2].vtx = D3DXVECTOR3(0.0f + ShakePos.x, Screen_Height + ShakePos.y, 0.0f);
	BossPraticeBG.VertexWk[3].vtx = D3DXVECTOR3(Screen_Width + ShakePos.x, Screen_Height + ShakePos.y, 0.0f);

	BlackScreenVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BlackScreenVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	BlackScreenVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	BlackScreenVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	BossPraticeSelect.VertexWk[0].vtx = D3DXVECTOR3(BossPraticeSelect.Pos.x - Texture_SelectBox_Width / 2, (float)BossPraticeSelect.Pos.y, 0.0f);
	BossPraticeSelect.VertexWk[1].vtx = D3DXVECTOR3(BossPraticeSelect.Pos.x + Texture_SelectBox_Width / 2, (float)BossPraticeSelect.Pos.y, 0.0f);
	BossPraticeSelect.VertexWk[2].vtx = D3DXVECTOR3(BossPraticeSelect.Pos.x - Texture_SelectBox_Width / 2, (float)BossPraticeSelect.Pos.y + Texture_SelectBox_Height, 0.0f);
	BossPraticeSelect.VertexWk[3].vtx = D3DXVECTOR3(BossPraticeSelect.Pos.x + Texture_SelectBox_Width / 2, (float)BossPraticeSelect.Pos.y + Texture_SelectBox_Height, 0.0f);

	return;
}
//=============================================================================
//
// リザルト処理 [Result.cpp]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#include "main.h"
#include "Result.h"
#include "Player.h"
#include "Pause.h"
#include "Transition.h"
#include "Sound.h"
#include "Input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RectHeight (100)
// 描画始点
#define RectPos_X (112)
#define RectPos_Y (84)
#define LinePos_X (112)
#define LinePos_Y (484)
// メニュー座標
#define PressAnyKeyPos_Y (610)
#define RestartPos_Y (264)
#define ToTitlePos_Y (444)
#define AskSentencePos_Y (200)
#define YesNoPos_Y (450)
#define YesPos_X (256)
#define NoPos_X (768)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeResultVertex(void);
void SetResultTexture(void);
void SetResultVertex(void);

enum
{
	Calculating,
	ScoreCalOver,
	Restart,
	ToTitle,
	RestartCheck,
	ToTitleCheck,
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// 頂点情報格納ワーク
VERTEX_2D ResultBGVertexWk[NUM_VERTEX];
static VERTEX_2D BlackScreenVertexWk[NUM_VERTEX];
// テクスチャへのポインタ
LPDIRECT3DTEXTURE9 ResultBGTexture = NULL;
LPD3DXLINE ResultLine = NULL;
// 時間計算用カウント
static int Count = 0;
// スコア計算用
static int TotalScore = 0;
static SELECT ResultSelect;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(int InitState)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	ResultSelect.Pos = D3DXVECTOR3(ScreenCenter_X, RestartPos_Y, 0.0f);
	ResultSelect.Phase = Calculating;
	ResultSelect.InYes = false;

	Count = 0;
	TotalScore = 0;

	// 頂点情報の作成
	MakeResultVertex();

	if (InitState == FirstInit)
	{
		// 情報表示用ラインを設定
		D3DXCreateLine(Device, &ResultLine);

		// テクスチャの読み込み
		if (SafeLoad(Texture_ResultBG, &ResultBGTexture, "Player") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	SafeRelease(ResultBGTexture);
	SafeRelease(ResultLine);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	static int PressCount = 0;
	TRANSITION *Transition = GetTransition();

	if (IsPlaying(BGM_Result) == false)
	{
		PlaySound(BGM_Result, E_DS8_FLAG_LOOP, true);
	}

	// 画面遷移
	if (Transition->FadeInOver == true && ResultSelect.InYes == true)
	{
		if (ResultSelect.Phase == RestartCheck)
		{
			ReInitGame(true);
			SetGameStage(Stage_Game);
		}
		else if (ResultSelect.Phase == ToTitleCheck)
		{
			ReInitGame(true);
			SetGameStage(Stage_Title);
		}

		return;
	}

	// スコア計算中じゃなければ
	if (ResultSelect.Phase != Calculating)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || GetKeyboardTrigger(DIK_Z) ||
			IsButtonTriggered(0, BUTTON_B))
		{
			switch (ResultSelect.Phase)
			{
			case ScoreCalOver:
				PlaySound(SE_Menu, E_DS8_FLAG_NONE, true);
				ResultSelect.Pos.y = RestartPos_Y;
				ResultSelect.Phase = Restart;
				break;
			case Restart:
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				ResultSelect.Phase = RestartCheck;
				ResultSelect.Pos.x = NoPos_X;
				ResultSelect.Pos.y = YesNoPos_Y;
				break;
			case RestartCheck:
				if (ResultSelect.InYes == true)
				{
					PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
					SetTransition();
				}
				else
				{
					PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
					ResultSelect.Phase = Restart;
					ResultSelect.Pos.x = ScreenCenter_X;
					ResultSelect.Pos.y = RestartPos_Y;
				}
				break;
			case ToTitle:
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				ResultSelect.Phase = ToTitleCheck;
				ResultSelect.Pos.x = NoPos_X;
				ResultSelect.Pos.y = YesNoPos_Y;
				break;
			case ToTitleCheck:
				if (ResultSelect.InYes == true)
				{
					PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
					SetTransition();
				}
				else
				{
					PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
					ResultSelect.Phase = ToTitle;
					ResultSelect.Pos.x = ScreenCenter_X;
					ResultSelect.Pos.y = ToTitlePos_Y;
				}
				break;
			default:
				break;
			}
		}

		if (ResultSelect.Phase != ScoreCalOver)
		{
			if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_DOWN) ||
				IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, BUTTON_DOWN))
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				switch (ResultSelect.Phase)
				{
				case Restart:
					ResultSelect.Phase = ToTitle;
					ResultSelect.Pos.y = ToTitlePos_Y;
					break;
				case ToTitle:
					ResultSelect.Phase = Restart;
					ResultSelect.Pos.y = RestartPos_Y;
					break;
				default:
					break;
				}
			}

			// 選択肢ループ
			if (GetKeyboardRepeat(DIK_DOWN) || IsButtonRepeat(0, BUTTON_DOWN) ||
				GetKeyboardRepeat(DIK_UP) || IsButtonRepeat(0, BUTTON_UP))
			{
				PressCount++;
				if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
				{
					PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
					if (ResultSelect.Phase == Restart)
					{
						ResultSelect.Phase = ToTitle;
						ResultSelect.Pos.y = ToTitlePos_Y;
					}
					else if (ResultSelect.Phase == ToTitle)
					{
						ResultSelect.Phase = Restart;
						ResultSelect.Pos.y = RestartPos_Y;
					}
				}
			}

			// プレスカウント初期化
			if (GetKeyboardRelease(DIK_UP) || GetKeyboardRelease(DIK_DOWN) ||
				IsButtonReleased(0, BUTTON_UP) || IsButtonReleased(0, BUTTON_DOWN))
			{
				PressCount = 0;
			}
		}

		if (ResultSelect.Phase == RestartCheck || ResultSelect.Phase == ToTitleCheck)
		{
			if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				ResultSelect.Pos.x = YesPos_X;
				ResultSelect.InYes = true;
			}
			else if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				ResultSelect.Pos.x = NoPos_X;
				ResultSelect.InYes = false;
			}
		}

		// テクスチャ更新
		SetResultVertex();
	}

	// カウント加算
	Count++;

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPDIRECT3DTEXTURE9 BlackScreenTexture = GetUITexture(BlackScreen);
	LPDIRECT3DTEXTURE9 ResultSelectTexture = GetUITexture(SelectBox);
	LPD3DXFONT Result = GetFont(FontSize_54);
	int HPScore = 0;
	int PowerScore = 0;
	int BombScore = 0;
	// 四捨五入計算用
	TCHAR RateRound[256];
	TCHAR Str[256];
	RECT TextRect = { RectPos_X, RectPos_Y, RectPos_X + 800, RectPos_Y + RectHeight };
	D3DXVECTOR2 LinePoint[] = { D3DXVECTOR2(LinePos_X, LinePos_Y), D3DXVECTOR2(LinePos_X + 800, LinePos_Y) };
	PLAYER *Player = GetPlayer(0);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, ResultBGTexture);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, ResultBGVertexWk, sizeof(VERTEX_2D));

	// 基本スコア
	if (Count >= 60)
	{
		if (Count == 60)
		{
			PlaySound(SE_ShowScore, E_DS8_FLAG_NONE, true);
		}
		// テキスト描く
		Result->DrawText(NULL, "基本スコア", -1, &TextRect, DT_VCENTER | DT_LEFT, WHITE(255));

		wsprintf(Str, _T("%d\n"), Player->Score);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_RIGHT, BLACK(255));
	}

	// HP Rate
	if (Count >= 120)
	{
		if (Count == 120)
		{
			PlaySound(SE_ShowScore, E_DS8_FLAG_NONE, true);
		}
		// 矩形位置調整
		TextRect.top += RectHeight;
		TextRect.bottom += RectHeight;
		// 小数点一位まで四捨五入
		sprintf(RateRound, "%.1f", Player->HP);
		_stprintf(Str, _T("HP Rate：%s%%"), RateRound);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_LEFT, WHITE(255));

		// スコア計算
		HPScore = (int)(atof(RateRound) * 50000);
		wsprintf(Str, _T("%d\n"), HPScore);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_RIGHT, BLACK(255));
	}

	// Power Rate
	if (Count >= 180)
	{
		if (Count == 180)
		{
			PlaySound(SE_ShowScore, E_DS8_FLAG_NONE, true);
		}
		// 矩形位置調整
		TextRect.top += RectHeight;
		TextRect.bottom += RectHeight;
		// 小数点二位まで四捨五入
		sprintf(RateRound, "%.2f", Player->Power);
		_stprintf(Str, _T("Power Rate：%s / 3.00"), RateRound);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_LEFT, WHITE(255));

		// スコア計算
		PowerScore = (int)(atof(RateRound) * 1500000);
		wsprintf(Str, _T("%d\n"), PowerScore);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_RIGHT, BLACK(255));
	}

	// ボムスコア
	if (Count >= 240)
	{
		if (Count == 240)
		{
			PlaySound(SE_ShowScore, E_DS8_FLAG_NONE, true);
		}
		// 矩形位置調整
		TextRect.top += RectHeight;
		TextRect.bottom += RectHeight;
		// 小数点一位まで四捨五入
		wsprintf(Str, _T("残りボム：%d / 5\n"), Player->BombNum);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_LEFT, WHITE(255));

		// スコア計算
		BombScore = (Player->BombNum * 1000000);
		wsprintf(Str, _T("%d\n"), BombScore);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_RIGHT, BLACK(255));
	}

	// Total Score
	if (Count >= 300)
	{
		if (Count == 300)
		{
			PlaySound(SE_CalculateScore, E_DS8_FLAG_LOOP, false);
		}
		// 直線を描く
		ResultLine->SetAntialias(TRUE);
		ResultLine->SetWidth(5);
		ResultLine->Begin();
		ResultLine->Draw(LinePoint, 2, BLACK(255));
		ResultLine->End();

		// 矩形位置調整
		TextRect.top += RectHeight;
		TextRect.bottom += RectHeight;
		Result->DrawText(NULL, "Total Score", -1, &TextRect, DT_VCENTER | DT_LEFT, WHITE(255));

		// スコア計算
		if (TotalScore < (Player->Score + HPScore + PowerScore + BombScore))
		{
			if (Count < 300)
			{
				TotalScore += 11;
			}
			else if (Count >= 300 && Count < 330)
			{
				TotalScore += 111;
			}
			else if (Count >= 330 && Count < 360)
			{
				TotalScore += 1111;
			}
			else if (Count >= 360 && Count < 390)
			{
				TotalScore += 11111;
			}
			else
			{
				TotalScore += 111111;
			}
		}
		// 計算終了
		if (TotalScore >= (Player->Score + HPScore + PowerScore + BombScore))
		{
			TotalScore = Player->Score + HPScore + PowerScore + BombScore;
			if (ResultSelect.Phase == Calculating)
			{
				StopSound(SE_CalculateScore);
				ResultSelect.Phase = ScoreCalOver;
			}
		}
		wsprintf(Str, _T("%d\n"), TotalScore);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_RIGHT, BLACK(255));
	}

	if (ResultSelect.Phase != Calculating)
	{
		// 矩形位置調整
		TextRect.top = PressAnyKeyPos_Y;
		TextRect.bottom = PressAnyKeyPos_Y + RectHeight;
		Result->DrawText(NULL, "Press Enter Key", -1, &TextRect, DT_VCENTER | DT_CENTER, BLACK(255));
	}

	// メニュー画面描画
	if (ResultSelect.Phase != Calculating && ResultSelect.Phase != ScoreCalOver)
	{
		Device->SetTexture(0, BlackScreenTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BlackScreenVertexWk, sizeof(VERTEX_2D));

		Device->SetTexture(0, ResultSelectTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, ResultSelect.VertexWk, sizeof(VERTEX_2D));

		if (ResultSelect.Phase == Restart || ResultSelect.Phase == ToTitle)
		{
			TextRect.top = RestartPos_Y;
			TextRect.bottom = RestartPos_Y + Texture_SelectBox_Height;
			if (ResultSelect.Phase == Restart)
			{
				Result->DrawText(NULL, "最初から", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Result->DrawText(NULL, "最初から", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			TextRect.top = ToTitlePos_Y;
			TextRect.bottom = ToTitlePos_Y + Texture_SelectBox_Height;
			if (ResultSelect.Phase == ToTitle)
			{
				Result->DrawText(NULL, "タイトルへ", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Result->DrawText(NULL, "タイトルへ", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
		}
		else if (ResultSelect.Phase == RestartCheck || ResultSelect.Phase == ToTitleCheck)
		{
			TextRect.top = AskSentencePos_Y;
			TextRect.bottom = AskSentencePos_Y + 100;
			if (ResultSelect.Phase == RestartCheck)
			{
				Result->DrawText(NULL, "最初からよろしですか？", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			else if (ResultSelect.Phase == ToTitleCheck)
			{
				Result->DrawText(NULL, "タイトルに戻りますか？", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			TextRect.top = YesNoPos_Y;
			TextRect.bottom = YesNoPos_Y + 100;
			TextRect.left = 0;
			TextRect.right = ScreenCenter_X;
			if (ResultSelect.InYes == true)
			{
				Result->DrawText(NULL, "はい", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Result->DrawText(NULL, "はい", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			TextRect.left = ScreenCenter_X;
			TextRect.right = Screen_Width;
			if (ResultSelect.InYes == true)
			{
				Result->DrawText(NULL, "いいえ", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			else
			{
				Result->DrawText(NULL, "いいえ", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
		}
	}

	return;
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeResultVertex(void)
{
	// 頂点座標の設定	
	SetResultTexture();

	// rhwの設定
	ResultBGVertexWk[0].rhw = 1.0f;
	ResultBGVertexWk[1].rhw = 1.0f;
	ResultBGVertexWk[2].rhw = 1.0f;
	ResultBGVertexWk[3].rhw = 1.0f;

	BlackScreenVertexWk[0].rhw = 1.0f;
	BlackScreenVertexWk[1].rhw = 1.0f;
	BlackScreenVertexWk[2].rhw = 1.0f;
	BlackScreenVertexWk[3].rhw = 1.0f;

	ResultSelect.VertexWk[0].rhw = 1.0f;
	ResultSelect.VertexWk[1].rhw = 1.0f;
	ResultSelect.VertexWk[2].rhw = 1.0f;
	ResultSelect.VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	ResultBGVertexWk[0].diffuse = WHITE(255);
	ResultBGVertexWk[1].diffuse = WHITE(255);
	ResultBGVertexWk[2].diffuse = WHITE(255);
	ResultBGVertexWk[3].diffuse = WHITE(255);

	BlackScreenVertexWk[0].diffuse = WHITE(200);
	BlackScreenVertexWk[1].diffuse = WHITE(200);
	BlackScreenVertexWk[2].diffuse = WHITE(200);
	BlackScreenVertexWk[3].diffuse = WHITE(200);

	ResultSelect.VertexWk[0].diffuse = WHITE(255);
	ResultSelect.VertexWk[1].diffuse = WHITE(255);
	ResultSelect.VertexWk[2].diffuse = WHITE(255);
	ResultSelect.VertexWk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetResultVertex();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetResultTexture(void)
{
	ResultBGVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	ResultBGVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	ResultBGVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	ResultBGVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	BlackScreenVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	BlackScreenVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	BlackScreenVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	BlackScreenVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	ResultSelect.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	ResultSelect.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	ResultSelect.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	ResultSelect.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetResultVertex(void)
{
	ResultBGVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ResultBGVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	ResultBGVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	ResultBGVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	BlackScreenVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BlackScreenVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	BlackScreenVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	BlackScreenVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	ResultSelect.VertexWk[0].vtx = D3DXVECTOR3(ResultSelect.Pos.x - Texture_SelectBox_Width / 2, (float)ResultSelect.Pos.y, 0.0f);
	ResultSelect.VertexWk[1].vtx = D3DXVECTOR3(ResultSelect.Pos.x + Texture_SelectBox_Width / 2, (float)ResultSelect.Pos.y, 0.0f);
	ResultSelect.VertexWk[2].vtx = D3DXVECTOR3(ResultSelect.Pos.x - Texture_SelectBox_Width / 2, (float)ResultSelect.Pos.y + Texture_SelectBox_Height, 0.0f);
	ResultSelect.VertexWk[3].vtx = D3DXVECTOR3(ResultSelect.Pos.x + Texture_SelectBox_Width / 2, (float)ResultSelect.Pos.y + Texture_SelectBox_Height, 0.0f);

	return;
}
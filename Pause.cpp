//=============================================================================
//
// 一時停止処理 [Pause.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Pause.h"
#include "Player.h"
#include "Transition.h"
#include "Sound.h"
#include "Input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テキスト矩形の高さ
#define RectHeight (100)
// テキスト表示の座標
#define PausePos_Y (128)
#define ResumePos_Y (350)
#define RestartPos_Y (450)
#define ToTitlePos_Y (550)
#define AskSentencePos_Y (200)
#define YesNoPos_Y (450)
#define YesPos_X (256)
#define NoPos_X (768)

// 一時停止の状態
enum
{
	Resume,			// 続く
	Restart,		// 最初から
	ToTitle,		// タイトルへ
	RestartCheck,	// 最初からの確認
	ToTitleCheck,	// タイトルへの確認
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakePauseVertex(void);
// 頂点座標の設定
void SetPauseVertex(void);
// テクスチャ座標の設定
void SetPauseTexture(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// 頂点情報格納ワーク
VERTEX_2D PauseVertexWk[Num_Vertex];
// 一時停止選択肢
static SELECT PauseSelect;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPause(int InitState)
{
	PauseSelect.Pos = D3DXVECTOR3(ScreenCenter_X, ResumePos_Y, 0.0f);
	PauseSelect.Phase = Resume;
	PauseSelect.InYes = false;

	// 頂点情報の作成
	MakePauseVertex();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{
	static int PressCount = 0;
	PLAYER *Player = GetPlayer(0);
	TRANSITION *Transition = GetTransition();

	// ステージ遷移
	if (Transition->FadeInOver == true && PauseSelect.InYes == true)
	{
		if (PauseSelect.Phase == RestartCheck)
		{
			ReInitGame(true);
			SetGameStage(Stage_Game);
		}
		else if (PauseSelect.Phase == ToTitleCheck)
		{
			ReInitGame(true);
			SetGameStage(Stage_Title);
		}

		return;
	}

	// Enterキー
	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || GetKeyboardTrigger(DIK_Z) ||
		IsButtonTriggered(0, BUTTON_B))
	{
		switch (PauseSelect.Phase)
		{
		case Resume:
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			SetGameStage(Stage_Game);
			PauseSelect.Phase = Resume;
			PauseSelect.Pos.y = ResumePos_Y;
			return;
			break;
		case Restart:
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			PauseSelect.Phase = RestartCheck;
			PauseSelect.Pos.x = NoPos_X;
			PauseSelect.Pos.y = YesNoPos_Y;
			break;
		case RestartCheck:
			if (PauseSelect.InYes == true)
			{
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				SetTransition();
			}
			else
			{
				PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
				PauseSelect.Phase = Restart;
				PauseSelect.Pos.x = ScreenCenter_X;
				PauseSelect.Pos.y = RestartPos_Y;
			}
			break;
		case ToTitle:
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			PauseSelect.Phase = ToTitleCheck;
			PauseSelect.Pos.x = NoPos_X;
			PauseSelect.Pos.y = YesNoPos_Y;
			break;
		case ToTitleCheck:
			if (PauseSelect.InYes == true)
			{
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				SetTransition();
			}
			else
			{
				PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
				PauseSelect.Phase = ToTitle;
				PauseSelect.Pos.x = ScreenCenter_X;
				PauseSelect.Pos.y = ToTitlePos_Y;
			}
			break;
		default:
			break;
		}
	}

	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
	{
		PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
		switch (PauseSelect.Phase)
		{
		case Resume:
			PauseSelect.Phase = Restart;
			PauseSelect.Pos.y = RestartPos_Y;
			break;
		case Restart:
			PauseSelect.Phase = ToTitle;
			PauseSelect.Pos.y = ToTitlePos_Y;
			break;
		case ToTitle:
			PauseSelect.Phase = Resume;
			PauseSelect.Pos.y = ResumePos_Y;
			break;
		default:
			break;
		}
	}
	else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
	{
		PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
		switch (PauseSelect.Phase)
		{
		case Resume:
			PauseSelect.Phase = ToTitle;
			PauseSelect.Pos.y = ToTitlePos_Y;
			break;
		case Restart:
			PauseSelect.Phase = Resume;
			PauseSelect.Pos.y = ResumePos_Y;
			break;
		case ToTitle:
			PauseSelect.Phase = Restart;
			PauseSelect.Pos.y = RestartPos_Y;
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
			if (PauseSelect.Phase == Resume)
			{
				PauseSelect.Phase = Restart;
				PauseSelect.Pos.y = RestartPos_Y;
			}
			else if (PauseSelect.Phase == Restart)
			{
				PauseSelect.Phase = ToTitle;
				PauseSelect.Pos.y = ToTitlePos_Y;
			}
			else if (PauseSelect.Phase == ToTitle)
			{
				PauseSelect.Phase = Resume;
				PauseSelect.Pos.y = ResumePos_Y;
			}
		}
	}
	else if (GetKeyboardRepeat(DIK_UP) || IsButtonRepeat(0, BUTTON_UP))
	{
		PressCount++;
		if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			if (PauseSelect.Phase == Resume)
			{
				PauseSelect.Phase = ToTitle;
				PauseSelect.Pos.y = ToTitlePos_Y;
			}
			else if (PauseSelect.Phase == Restart)
			{
				PauseSelect.Phase = Resume;
				PauseSelect.Pos.y = ResumePos_Y;
			}
			else if (PauseSelect.Phase == ToTitle)
			{
				PauseSelect.Phase = Restart;
				PauseSelect.Pos.y = RestartPos_Y;
			}
		}
	}


	if (GetKeyboardRelease(DIK_UP) || GetKeyboardRelease(DIK_DOWN) ||
		IsButtonReleased(0, BUTTON_UP) || IsButtonReleased(0, BUTTON_DOWN))
	{
		PressCount = 0;
	}

	if (PauseSelect.Phase == RestartCheck || PauseSelect.Phase == ToTitleCheck)
	{
		if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			PauseSelect.Pos.x = YesPos_X;
			PauseSelect.InYes = true;
		}
		else if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			PauseSelect.Pos.x = NoPos_X;
			PauseSelect.InYes = false;
		}
	}

	// テクスチャ更新
	SetPauseVertex();

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPDIRECT3DTEXTURE9 PauseTexture = GetUITexture(BlackScreen);
	LPDIRECT3DTEXTURE9 PauseSelectTexture = GetUITexture(SelectBox);
	LPD3DXFONT Pause = GetFont(FontSize_108);
	LPD3DXFONT Option = GetFont(FontSize_54);
	RECT rect = { 0, 0, Screen_Width, RectHeight };

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, PauseTexture);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PauseVertexWk, sizeof(VERTEX_2D));

	// テクスチャの設定
	Device->SetTexture(0, PauseSelectTexture);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PauseSelect.VertexWk, sizeof(VERTEX_2D));

	if (PauseSelect.Phase != RestartCheck && PauseSelect.Phase != ToTitleCheck)
	{
		rect.top = PausePos_Y;
		rect.bottom = PausePos_Y + RectHeight;
		Pause->DrawText(NULL, "P A U S E", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		rect.top = ResumePos_Y;
		rect.bottom = ResumePos_Y + RectHeight;
		if (PauseSelect.Phase == Resume)
		{
			Option->DrawText(NULL, "続く", -1, &rect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "続く", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}

		rect.top = RestartPos_Y;
		rect.bottom = RestartPos_Y + RectHeight;
		if (PauseSelect.Phase == Restart)
		{
			Option->DrawText(NULL, "最初から", -1, &rect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "最初から", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}

		rect.top = ToTitlePos_Y;
		rect.bottom = ToTitlePos_Y + RectHeight;
		if (PauseSelect.Phase == ToTitle)
		{
			Option->DrawText(NULL, "タイトルへ", -1, &rect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "タイトルへ", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
	}
	else
	{
		rect.top = AskSentencePos_Y;
		rect.bottom = AskSentencePos_Y + RectHeight;
		if (PauseSelect.Phase == RestartCheck)
		{
			Option->DrawText(NULL, "最初からよろしですか？", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		else if (PauseSelect.Phase == ToTitleCheck)
		{
			Option->DrawText(NULL, "タイトルに戻りますか？", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		rect.top = YesNoPos_Y;
		rect.bottom = YesNoPos_Y + RectHeight;
		rect.left = 0;
		rect.right = ScreenCenter_X;
		if (PauseSelect.InYes == true)
		{
			Option->DrawText(NULL, "はい", -1, &rect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "はい", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		rect.left = ScreenCenter_X;
		rect.right = Screen_Width;
		if (PauseSelect.InYes == true)
		{
			Option->DrawText(NULL, "いいえ", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		else
		{
			Option->DrawText(NULL, "いいえ", -1, &rect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
	}

	return;
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakePauseVertex(void)
{
	// 頂点座標の設定	
	SetPauseTexture();

	// rhwの設定
	PauseVertexWk[0].rhw = 1.0f;
	PauseVertexWk[1].rhw = 1.0f;
	PauseVertexWk[2].rhw = 1.0f;
	PauseVertexWk[3].rhw = 1.0f;

	PauseSelect.VertexWk[0].rhw = 1.0f;
	PauseSelect.VertexWk[1].rhw = 1.0f;
	PauseSelect.VertexWk[2].rhw = 1.0f;
	PauseSelect.VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	PauseVertexWk[0].diffuse = WHITE(200);
	PauseVertexWk[1].diffuse = WHITE(200);
	PauseVertexWk[2].diffuse = WHITE(200);
	PauseVertexWk[3].diffuse = WHITE(200);

	PauseSelect.VertexWk[0].diffuse = WHITE(255);
	PauseSelect.VertexWk[1].diffuse = WHITE(255);
	PauseSelect.VertexWk[2].diffuse = WHITE(255);
	PauseSelect.VertexWk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetPauseVertex();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetPauseTexture(void)
{
	PauseVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	PauseVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	PauseVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	PauseVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	PauseSelect.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	PauseSelect.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	PauseSelect.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	PauseSelect.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetPauseVertex(void)
{
	PauseVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PauseVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	PauseVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	PauseVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	PauseSelect.VertexWk[0].vtx = D3DXVECTOR3(PauseSelect.Pos.x - Texture_SelectBox_Width / 2, (float)PauseSelect.Pos.y, 0.0f);
	PauseSelect.VertexWk[1].vtx = D3DXVECTOR3(PauseSelect.Pos.x + Texture_SelectBox_Width / 2, (float)PauseSelect.Pos.y, 0.0f);
	PauseSelect.VertexWk[2].vtx = D3DXVECTOR3(PauseSelect.Pos.x - Texture_SelectBox_Width / 2, (float)PauseSelect.Pos.y + Texture_SelectBox_Height, 0.0f);
	PauseSelect.VertexWk[3].vtx = D3DXVECTOR3(PauseSelect.Pos.x + Texture_SelectBox_Width / 2, (float)PauseSelect.Pos.y + Texture_SelectBox_Height, 0.0f);

	return;
}
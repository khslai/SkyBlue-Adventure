//=============================================================================
//
// タイトル画面処理 [Title.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Title.h"
#include "BackGround.h"
#include "FrontBG.h"
#include "Transition.h"
#include "Sound.h"
#include "Input.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 背景スクロール速度
#define TitleBGSpeed (0.0003f)
// 雲スクロール速度
#define CloudSpeed (0.0002f)
// テキスト矩形の高さ
#define RectHeight (100)
// テキスト表示の座標
#define GameTitlePos_X (87)
#define GameTitlePos_Y (200)
#define GameStartPos_Y (384)
#define BossPraticePos_Y (448)
#define ExitGamePos_Y (512)
#define EasyModePos_Y (384)
#define NormalModePos_Y (448)
#define HardModePos_Y (512)
#define AskSentencePos_Y (200)
#define YesNoPos_Y (450)
#define YesPos_X (256)
#define NoPos_X (768)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeTitleVertex(void);
// 頂点座標の設定
void SetTitleVertex(void);
// テクスチャ座標の設定
void SetTitleTexture(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		TitleBGTexture = NULL;
LPDIRECT3DTEXTURE9		GameTitleTexture = NULL;
LPDIRECT3DTEXTURE9		LightTexture = NULL;
LPDIRECT3DTEXTURE9		CloudTexture = NULL;
// 頂点情報格納ワーク
VERTEX_2D				BlackScreenVertexWk[Num_Vertex];
// タイトル背景構造体
TITLE					TitleBG;
// タイトル構造体
TITLE					GameTitle;
// 光構造体
TITLE					Light;
// 雲構造体
TITLE					Cloud;
// タイトル選択肢
static SELECT			TitleSelect;
// ゲーム難易度を選択しているフラグ
bool					InSelectDifficulty;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(int InitState)
{
	TitleBG.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	GameTitle.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Light.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Cloud.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	TitleSelect.Pos = D3DXVECTOR3(NoPos_X, YesNoPos_Y, 0.0f);
	TitleSelect.Phase = GameStart;
	TitleSelect.InYes = false;
	InSelectDifficulty = false;

	// 頂点情報の作成
	MakeTitleVertex();

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_TitleBG, &TitleBGTexture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GameTitle, &GameTitleTexture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Light, &LightTexture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_FrontBG, &CloudTexture, "Title") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	SafeRelease(TitleBGTexture);
	SafeRelease(GameTitleTexture);
	SafeRelease(LightTexture);
	SafeRelease(CloudTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	int i = 0, j = 0;
	static int PressCount = 0;
	TRANSITION *Transition = GetTransition();

	// タイトルBGM
	PlaySound(BGM_Title, E_DS8_FLAG_LOOP, false);

	// 背景スクロール
	TitleBG.Pos.x += TitleBGSpeed;
	Cloud.Pos.x += CloudSpeed;

	// 画面遷移
	if (Transition->FadeInOver == true && (TitleSelect.Phase == StartYes || TitleSelect.Phase == PraticeYes))
	{
		ReInitGame(false);
		InSelectDifficulty = false;
		SetGameStage(Stage_SelectChar);
	}

	// Enter押したら、ステージを切り替える
	if (Transition->FadeInOver != true)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || GetKeyboardTrigger(DIK_Z) ||
			IsButtonTriggered(0, BUTTON_B))
		{
			if (TitleSelect.Phase != ExitCheck)
			{
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				if (TitleSelect.Phase == GameStart)
				{
					InSelectDifficulty = true;
					TitleSelect.Phase = NormalMode;
				}
				else if (TitleSelect.Phase == BossPratice)
				{
					TitleSelect.Phase = PraticeYes;
					SetTransition();
				}
				else if (TitleSelect.Phase == ExitGame)
				{
					TitleSelect.Phase = ExitCheck;
				}
				else if (TitleSelect.Phase == EasyMode)
				{
					SetGameDiffuculty(Easy);
					TitleSelect.Phase = StartYes;
					SetTransition();
				}
				else if (TitleSelect.Phase == NormalMode)
				{
					SetGameDiffuculty(Normal);
					TitleSelect.Phase = StartYes;
					SetTransition();
				}
				else if (TitleSelect.Phase == HardMode)
				{
					SetGameDiffuculty(Hard);
					TitleSelect.Phase = StartYes;
					SetTransition();
				}
			}
			else if (TitleSelect.Phase == ExitCheck)
			{
				PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
				if (TitleSelect.InYes == true)
				{
					TitleSelect.Phase = ExitYes;
				}
				else
				{
					TitleSelect.Phase = ExitGame;
				}
			}
		}
	}

	if (TitleSelect.Phase != ExitCheck)
	{
		// 下に選択肢を移動
		if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
		{
			// 効果音
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			if (TitleSelect.Phase == GameStart)
			{
				TitleSelect.Phase = BossPratice;
			}
			else if (TitleSelect.Phase == BossPratice)
			{
				TitleSelect.Phase = ExitGame;
			}
			else if (TitleSelect.Phase == ExitGame)
			{
				TitleSelect.Phase = GameStart;
			}
			else if (TitleSelect.Phase == EasyMode)
			{
				TitleSelect.Phase = NormalMode;
			}
			else if (TitleSelect.Phase == NormalMode)
			{
				TitleSelect.Phase = HardMode;
			}
			else if (TitleSelect.Phase == HardMode)
			{
				TitleSelect.Phase = EasyMode;
			}
		}
		// 上に選択肢を移動
		else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
		{
			// 効果音
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			if (TitleSelect.Phase == GameStart)
			{
				TitleSelect.Phase = ExitGame;
			}
			else if (TitleSelect.Phase == BossPratice)
			{
				TitleSelect.Phase = GameStart;
			}
			else if (TitleSelect.Phase == ExitGame)
			{
				TitleSelect.Phase = BossPratice;
			}
			else if (TitleSelect.Phase == EasyMode)
			{
				TitleSelect.Phase = HardMode;
			}
			else if (TitleSelect.Phase == NormalMode)
			{
				TitleSelect.Phase = EasyMode;
			}
			else if (TitleSelect.Phase == HardMode)
			{
				TitleSelect.Phase = NormalMode;
			}
		}

		// プレスカウント初期化
		if (GetKeyboardRelease(DIK_UP) || GetKeyboardRelease(DIK_DOWN) ||
			IsButtonReleased(0, BUTTON_UP) || IsButtonReleased(0, BUTTON_DOWN))
		{
			PressCount = 0;
		}

		// 選択肢ループ
		if (GetKeyboardRepeat(DIK_DOWN) || IsButtonRepeat(0, BUTTON_DOWN))
		{
			PressCount++;
			if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				if (InSelectDifficulty == false)
				{
					TitleSelect.Phase += 2;
					TitleSelect.Phase = TitleSelect.Phase % 6;
				}
				else
				{
					TitleSelect.Phase++;
					if (TitleSelect.Phase > HardMode)
					{
						TitleSelect.Phase = EasyMode;
					}
				}
			}
		}
		else if (GetKeyboardRepeat(DIK_UP) || IsButtonRepeat(0, BUTTON_UP))
		{
			PressCount++;
			if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				if (InSelectDifficulty == false)
				{
					TitleSelect.Phase -= 2;
					if (TitleSelect.Phase < GameStart)
					{
						TitleSelect.Phase = ExitGame;
					}
				}
				else
				{
					TitleSelect.Phase--;
					if (TitleSelect.Phase < EasyMode)
					{
						TitleSelect.Phase = HardMode;
					}
				}
			}
		}

		// 前の画面に戻る
		if ((GetKeyboardTrigger(DIK_X) || IsButtonTriggered(0, BUTTON_A)) && InSelectDifficulty == true)
		{
			PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
			InSelectDifficulty = false;
			TitleSelect.Phase = GameStart;
		}
	}
	else if (TitleSelect.Phase == ExitCheck)
	{
		// 選択肢移動
		if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			TitleSelect.InYes = false;
			TitleSelect.Pos.x = NoPos_X;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			TitleSelect.InYes = true;
			TitleSelect.Pos.x = YesPos_X;
		}
	}

	// テクスチャ更新
	SetTitleTexture();
	SetTitleVertex();

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPD3DXFONT Option = GetFont(FontSize_54);
	RECT TextRect = { 0, 0, Screen_Width, RectHeight };

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, TitleBGTexture);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TitleBG.VertexWk, sizeof(VERTEX_2D));

	// タイトル
	Device->SetTexture(0, GameTitleTexture);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, GameTitle.VertexWk, sizeof(VERTEX_2D));

	// 光
	Device->SetTexture(0, LightTexture);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Light.VertexWk, sizeof(VERTEX_2D));
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 雲
	Device->SetTexture(0, CloudTexture);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Cloud.VertexWk, sizeof(VERTEX_2D));
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (InSelectDifficulty == false)
	{

		// Game Start
		TextRect.top = GameStartPos_Y;
		TextRect.bottom = GameStartPos_Y + RectHeight;
		if (TitleSelect.Phase == GameStart)
		{
			Option->DrawText(NULL, "Game Start", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "Game Start", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(128));
		}

		// Boss Mode
		TextRect.top = BossPraticePos_Y;
		TextRect.bottom = BossPraticePos_Y + RectHeight;
		if (TitleSelect.Phase == BossPratice)
		{
			Option->DrawText(NULL, "Boss Mode", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "Boss Mode", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(128));
		}

		// Exit Game
		TextRect.top = ExitGamePos_Y;
		TextRect.bottom = ExitGamePos_Y + RectHeight;
		if (TitleSelect.Phase == ExitGame)
		{
			Option->DrawText(NULL, "Exit Game", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "Exit Game", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(128));
		}
	}
	// GameDifficulty
	else
	{
		// Easy
		TextRect.top = EasyModePos_Y;
		TextRect.bottom = EasyModePos_Y + RectHeight;
		if (TitleSelect.Phase == EasyMode)
		{
			Option->DrawText(NULL, "Easy", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "Easy", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(128));
		}

		// Normal
		TextRect.top = NormalModePos_Y;
		TextRect.bottom = NormalModePos_Y + RectHeight;
		if (TitleSelect.Phase == NormalMode)
		{
			Option->DrawText(NULL, "Normal", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "Normal", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(128));
		}

		// Hard
		TextRect.top = HardModePos_Y;
		TextRect.bottom = HardModePos_Y + RectHeight;
		if (TitleSelect.Phase == HardMode)
		{
			Option->DrawText(NULL, "Hard", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "Hard", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(128));
		}
	}

	return;
}

//=============================================================================
// メニュー画面描画処理
//=============================================================================
void DrawTitleMenu(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPDIRECT3DTEXTURE9 BlackScreenTexture = GetUITexture(BlackScreen);
	LPDIRECT3DTEXTURE9 TitleSelectTexture = GetUITexture(SelectBox);
	LPD3DXFONT Title = GetFont(FontSize_54);
	LPD3DXFONT Option = GetFont(FontSize_40);
	RECT TextRect = { 0, 0, Screen_Width, RectHeight };

	if (TitleSelect.Phase == ExitCheck)
	{
		// 黒い画面描画
		Device->SetTexture(0, BlackScreenTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BlackScreenVertexWk, sizeof(VERTEX_2D));

		// 選択枠描画
		Device->SetTexture(0, TitleSelectTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TitleSelect.VertexWk, sizeof(VERTEX_2D));

		TextRect.top = AskSentencePos_Y;
		TextRect.bottom = AskSentencePos_Y + RectHeight;
		Title->DrawText(NULL, "本当にゲームを終了しますか？", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));

		TextRect.top = YesNoPos_Y;
		TextRect.bottom = YesNoPos_Y + RectHeight;
		TextRect.left = 0;
		TextRect.right = ScreenCenter_X;
		if (TitleSelect.InYes == true)
		{
			Title->DrawText(NULL, "はい", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Title->DrawText(NULL, "はい", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		TextRect.left = ScreenCenter_X;
		TextRect.right = Screen_Width;
		if (TitleSelect.InYes == true)
		{
			Title->DrawText(NULL, "いいえ", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		else
		{
			Title->DrawText(NULL, "いいえ", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeTitleVertex(void)
{
	// テクスチャ座標の設定
	SetTitleTexture();

	// rhwの設定
	TitleBG.VertexWk[0].rhw = 1.0f;
	TitleBG.VertexWk[1].rhw = 1.0f;
	TitleBG.VertexWk[2].rhw = 1.0f;
	TitleBG.VertexWk[3].rhw = 1.0f;

	GameTitle.VertexWk[0].rhw = 1.0f;
	GameTitle.VertexWk[1].rhw = 1.0f;
	GameTitle.VertexWk[2].rhw = 1.0f;
	GameTitle.VertexWk[3].rhw = 1.0f;

	Light.VertexWk[0].rhw = 1.0f;
	Light.VertexWk[1].rhw = 1.0f;
	Light.VertexWk[2].rhw = 1.0f;
	Light.VertexWk[3].rhw = 1.0f;

	Cloud.VertexWk[0].rhw = 1.0f;
	Cloud.VertexWk[1].rhw = 1.0f;
	Cloud.VertexWk[2].rhw = 1.0f;
	Cloud.VertexWk[3].rhw = 1.0f;

	BlackScreenVertexWk[0].rhw = 1.0f;
	BlackScreenVertexWk[1].rhw = 1.0f;
	BlackScreenVertexWk[2].rhw = 1.0f;
	BlackScreenVertexWk[3].rhw = 1.0f;

	TitleSelect.VertexWk[0].rhw = 1.0f;
	TitleSelect.VertexWk[1].rhw = 1.0f;
	TitleSelect.VertexWk[2].rhw = 1.0f;
	TitleSelect.VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	TitleBG.VertexWk[0].diffuse = WHITE(255);
	TitleBG.VertexWk[1].diffuse = WHITE(255);
	TitleBG.VertexWk[2].diffuse = WHITE(255);
	TitleBG.VertexWk[3].diffuse = WHITE(255);

	GameTitle.VertexWk[0].diffuse = WHITE(255);
	GameTitle.VertexWk[1].diffuse = WHITE(255);
	GameTitle.VertexWk[2].diffuse = WHITE(255);
	GameTitle.VertexWk[3].diffuse = WHITE(255);

	Light.VertexWk[0].diffuse = WHITE(128);
	Light.VertexWk[1].diffuse = WHITE(128);
	Light.VertexWk[2].diffuse = WHITE(128);
	Light.VertexWk[3].diffuse = WHITE(128);

	Cloud.VertexWk[0].diffuse = WHITE(128);
	Cloud.VertexWk[1].diffuse = WHITE(128);
	Cloud.VertexWk[2].diffuse = WHITE(128);
	Cloud.VertexWk[3].diffuse = WHITE(128);

	BlackScreenVertexWk[0].diffuse = WHITE(200);
	BlackScreenVertexWk[1].diffuse = WHITE(200);
	BlackScreenVertexWk[2].diffuse = WHITE(200);
	BlackScreenVertexWk[3].diffuse = WHITE(200);

	TitleSelect.VertexWk[0].diffuse = WHITE(255);
	TitleSelect.VertexWk[1].diffuse = WHITE(255);
	TitleSelect.VertexWk[2].diffuse = WHITE(255);
	TitleSelect.VertexWk[3].diffuse = WHITE(255);

	// 頂点座標の設定
	SetTitleVertex();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTitleTexture(void)
{
	float BGRate_X = (float)Screen_Width / (float)Texture_TitleBG_Width;
	float CloudRate_X = (float)Screen_Width / (float)Texture_FrontBG_Width;

	TitleBG.VertexWk[0].tex = D3DXVECTOR2(TitleBG.Pos.x, 0.0f);
	TitleBG.VertexWk[1].tex = D3DXVECTOR2(TitleBG.Pos.x + BGRate_X, 0.0f);
	TitleBG.VertexWk[2].tex = D3DXVECTOR2(TitleBG.Pos.x, 1.0f);
	TitleBG.VertexWk[3].tex = D3DXVECTOR2(TitleBG.Pos.x + BGRate_X, 1.0f);

	GameTitle.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	GameTitle.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	GameTitle.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	GameTitle.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	Light.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Light.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Light.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Light.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	Cloud.VertexWk[0].tex = D3DXVECTOR2(TitleBG.Pos.x, 0.0f);
	Cloud.VertexWk[1].tex = D3DXVECTOR2(TitleBG.Pos.x + CloudRate_X, 0.0f);
	Cloud.VertexWk[2].tex = D3DXVECTOR2(TitleBG.Pos.x, 1.0f);
	Cloud.VertexWk[3].tex = D3DXVECTOR2(TitleBG.Pos.x + CloudRate_X, 1.0f);

	BlackScreenVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	BlackScreenVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	BlackScreenVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	BlackScreenVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	TitleSelect.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	TitleSelect.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	TitleSelect.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	TitleSelect.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetTitleVertex(void)
{
	static bool ZoomPositive_X = false;
	static bool ZoomPositive_Y = false;
	static float ZoomRate_X = 1.0f;
	static float ZoomRate_Y = 1.0f;

	// 揺れる光の計算
	if (ZoomRate_X >= 1.5f && ZoomPositive_X == true)
	{
		ZoomPositive_X = false;
		ZoomPositive_Y = false;
	}

	if (ZoomRate_Y >= 1.5f && ZoomPositive_Y == true)
	{
		ZoomPositive_X = true;
		ZoomPositive_Y = false;
	}
	else if (ZoomRate_Y <= 1.0f && ZoomPositive_Y == false)
	{
		ZoomPositive_Y = true;
	}

	if (ZoomPositive_Y == false)
	{
		if (ZoomPositive_X == true)
		{
			ZoomRate_X += 0.001f;
		}
		else
		{
			ZoomRate_X -= 0.001f;
		}
	}

	if (ZoomPositive_X == false)
	{
		if (ZoomPositive_Y == true)
		{
			ZoomRate_Y += 0.001f;
		}
		else
		{
			ZoomRate_Y -= 0.001f;
		}
	}

	TitleBG.VertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	TitleBG.VertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	TitleBG.VertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	TitleBG.VertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	GameTitle.VertexWk[0].vtx = D3DXVECTOR3(GameTitlePos_X, GameTitlePos_Y, 0.0f);
	GameTitle.VertexWk[1].vtx = D3DXVECTOR3(GameTitlePos_X + Texture_GameTitle_Width, GameTitlePos_Y, 0.0f);
	GameTitle.VertexWk[2].vtx = D3DXVECTOR3(GameTitlePos_X, GameTitlePos_Y + Texture_GameTitle_Height, 0.0f);
	GameTitle.VertexWk[3].vtx = D3DXVECTOR3(GameTitlePos_X + Texture_GameTitle_Width, GameTitlePos_Y + Texture_GameTitle_Height, 0.0f);

	Light.VertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Light.VertexWk[1].vtx = D3DXVECTOR3(Screen_Width * ZoomRate_X, 0.0f, 0.0f);
	Light.VertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height * ZoomRate_Y, 0.0f);
	Light.VertexWk[3].vtx = D3DXVECTOR3(Screen_Width * ZoomRate_X, Screen_Height * ZoomRate_Y, 0.0f);

	Cloud.VertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Cloud.VertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	Cloud.VertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	Cloud.VertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	BlackScreenVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BlackScreenVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	BlackScreenVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	BlackScreenVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	TitleSelect.VertexWk[0].vtx = D3DXVECTOR3(TitleSelect.Pos.x - Texture_SelectBox_Width / 2, (float)TitleSelect.Pos.y, 0.0f);
	TitleSelect.VertexWk[1].vtx = D3DXVECTOR3(TitleSelect.Pos.x + Texture_SelectBox_Width / 2, (float)TitleSelect.Pos.y, 0.0f);
	TitleSelect.VertexWk[2].vtx = D3DXVECTOR3(TitleSelect.Pos.x - Texture_SelectBox_Width / 2, (float)TitleSelect.Pos.y + Texture_SelectBox_Height, 0.0f);
	TitleSelect.VertexWk[3].vtx = D3DXVECTOR3(TitleSelect.Pos.x + Texture_SelectBox_Width / 2, (float)TitleSelect.Pos.y + Texture_SelectBox_Height, 0.0f);

	return;
}

//=============================================================================
// タイトル選択肢の情報を取得する
//=============================================================================
SELECT *GetTitleSelect(void)
{
	return &TitleSelect;
}
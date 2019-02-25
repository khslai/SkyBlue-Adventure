//=============================================================================
//
// 自機選択処理 [SelectChar.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "SelectChar.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "Sound.h"
#include "Input.h"
#include "Transition.h"
#include "Title.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// アニメーションを切り替えるカウント
#define AnimChangeCount (15)
// テキスト矩形の高さ
#define RectHeight (100)
// テキスト表示の座標
#define TitlePos_Y (50)
#define OptionPos_Y	(150)
#define AskSentencePos_Y (200)
#define YesNoPos_Y (450)
#define YesPos_X (256)
#define NoPos_X (768)



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeSelectCharVertex(void);
// 頂点座標の設定
void SetSelectCharVertex(void);
// テクスチャ座標の設定
void SetSelectCharTexture(void);
// 色、透明度の設定
void SetEnemyColor(void);
// プレイヤーとエネミーの設置
void SetObject(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 SelectBGTexture = NULL;
LPDIRECT3DTEXTURE9 DrawBoxTexture = NULL;
// 頂点情報格納ワーク
VERTEX_2D SelectBGVertexWk[Num_Vertex];
VERTEX_2D DrawBoxVertexWk[Num_Vertex];
static VERTEX_2D BlackScreenVertexWk[Num_Vertex];
// 自機タイプ選択肢
static SELECT SelectChar;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSelectChar(int InitState)
{
	SelectChar.Pos = D3DXVECTOR3(NoPos_X, YesNoPos_Y, 0.0f);
	SelectChar.Phase = SetObjectStart;
	SelectChar.InYes = false;

	// 頂点情報の作成
	MakeSelectCharVertex();

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_SelectCharBG, &SelectBGTexture, "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_DrawBox, &DrawBoxTexture, "BackGround") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSelectChar(void)
{
	SafeRelease(SelectBGTexture);
	SafeRelease(DrawBoxTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSelectChar(void)
{
	int Bullet_No = 0;
	SELECT *TitleSelect = GetTitleSelect();
	static bool MoveUp = false;
	PLAYER *Player = GetPlayer(0);
	ENEMY *Enemy = GetEnemy(0);
	TRANSITION *Transition = GetTransition();

	// プレイヤー、エネミー設置
	if (SelectChar.Phase == SetObjectStart)
	{
		SetObject();
		SelectChar.Phase = ChooseTypeA;
		MoveUp = false;
	}

	// 画面遷移、次のステージに移す
	if (Transition->FadeInOver == true &&
		(SelectChar.Phase == ChooseOver || SelectChar.Phase == ChooseCheck || SelectChar.Phase == BackToTitle))
	{
		InitPlayer(ReInit);
		InitPlayerBullet(ReInit);
		InitEnemy(ReInit);

		// タイトルで選択のは"Game Start"
		if (TitleSelect->Phase == StartYes)
		{
			ReInitSound();
			if (SelectChar.InYes == true)
			{
				SetGameStage(Stage_Game);
			}
			else
			{
				SetGameStage(Stage_Tutorial);
			}
		}
		// タイトルで選択のは"Pratice Mode"
		else if (TitleSelect->Phase == PraticeYes)
		{
			SetGameStage(Stage_PraticeSelect);
		}
		// タイトルに戻す
		else if (SelectChar.Phase == BackToTitle)
		{
			SetGameStage(Stage_Title);
		}

		return;
	}

	// Enterキー
	if (Transition->FadeInOver != true)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || GetKeyboardTrigger(DIK_Z) ||
			IsButtonTriggered(0, BUTTON_B))
		{
			if (SelectChar.Phase == ChooseTypeA)
			{
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				Player->Type = Player_A;
				SelectChar.Phase = ChooseCheck;
			}
			else if (SelectChar.Phase == ChooseTypeB)
			{
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				Player->Type = Player_B;
				SelectChar.Phase = ChooseCheck;
			}
			else if (SelectChar.Phase == ChooseCheck)
			{
				// はい
				if (SelectChar.InYes == true)
				{
					PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
					if (TitleSelect->Phase == StartYes)
					{
						SelectChar.Phase = ChooseOver;
						SelectChar.Pos.x = NoPos_X;
						SelectChar.InYes = false;
					}
					else if (TitleSelect->Phase == PraticeYes)
					{
						SetTransition();
					}
				}
				// いいえ
				else
				{
					PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
					if (Player->Type == Player_A)
					{
						SelectChar.Phase = ChooseTypeA;
					}
					else if (Player->Type == Player_B)
					{
						SelectChar.Phase = ChooseTypeB;
					}
				}
			}
			else if (SelectChar.Phase == ChooseOver)
			{
				// はい
				if (SelectChar.InYes == true)
				{
					PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				}
				// いいえ
				else
				{
					PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
				}
				SetTransition();
			}
		}

		// 前の画面に戻る
		if ((GetKeyboardTrigger(DIK_X) || IsButtonTriggered(0,BUTTON_A)) && 
			(SelectChar.Phase == ChooseTypeA || SelectChar.Phase == ChooseTypeB))
		{
			PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
			SelectChar.Phase = BackToTitle;
			SetTransition();
			if (TitleSelect->Phase == StartYes)
			{
				TitleSelect->Phase = GameStart;
			}
			else if (TitleSelect->Phase == PraticeYes)
			{
				TitleSelect->Phase = BossPratice;
			}
		}
	}

	// 選択肢移動
	if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0,BUTTON_RIGHT))
	{
		PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
		if (SelectChar.Phase == ChooseTypeA)
		{
			for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
			{
				if (Player->Bullet[Bullet_No].Use == true)
				{
					Player->Bullet[Bullet_No].Use = false;
				}
			}
			Player->Type = Player_B;
			Player->BulletType = PlayerBulletType_B;
			Player->Option[Option_A].BulletType = OptionBulletType_B;
			Player->Option[Option_B].BulletType = OptionBulletType_B;
			SelectChar.Phase = ChooseTypeB;
		}
		else if (SelectChar.Phase == ChooseCheck || SelectChar.Phase == ChooseOver)
		{
			SelectChar.InYes = false;
			SelectChar.Pos.x = NoPos_X;
		}
	}
	else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
	{
		PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
		if (SelectChar.Phase == ChooseTypeB)
		{
			for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
			{
				if (Player->Bullet[Bullet_No].Use == true)
				{
					Player->Bullet[Bullet_No].Use = false;
				}
			}
			Player->Type = Player_A;
			Player->BulletType = PlayerBulletType_A;
			Player->Option[Option_A].BulletType = OptionBulletType_A;
			Player->Option[Option_B].BulletType = OptionBulletType_A;
			SelectChar.Phase = ChooseTypeA;
		}
		else if (SelectChar.Phase == ChooseCheck || SelectChar.Phase == ChooseOver)
		{
			SelectChar.InYes = true;
			SelectChar.Pos.x = YesPos_X;
		}
	}

	// プレイヤー、エネミー更新
	if (SelectChar.Phase != ChooseCheck && SelectChar.Phase != ChooseOver)
	{
		Player->AnimCount++;
		if ((Player->AnimCount % AnimChangeCount) == 0)
		{
			Player->AnimPattern = (Player->AnimPattern + 1) % Texture_Player_Divide_X;
		}

		Player->ShotCount++;
		if (Player->ShotCount % 4 == 0)
		{
			PlayerShot();
		}

		if (MoveUp == false)
		{
			Player->Pos.y += 3.0f;
			if (Player->Pos.y >= 640.0f)
			{
				MoveUp = true;
			}
		}
		else if (MoveUp == true)
		{
			Player->Pos.y -= 3.0f;
			if (Player->Pos.y <= 384.0f)
			{
				MoveUp = false;
			}
		}
		Player->Option[Option_A].Pos.x = Player->Pos.x + OptionPos_x[Option_A];
		Player->Option[Option_A].Pos.y = Player->Pos.y + OptionPos_y[Option_A];
		Player->Option[Option_B].Pos.x = Player->Pos.x + OptionPos_x[Option_B];
		Player->Option[Option_B].Pos.y = Player->Pos.y + OptionPos_y[Option_B];

		Enemy->Count++;
		if ((Enemy->Count % AnimChangeCount) == 0)
		{
			Enemy->AnimPattern = (Enemy->AnimPattern + 1) % Texture_Enemy03_Divide_X;
		}
		SetEnemyColor();
	}

	// テクスチャ更新
	SetSelectCharTexture();
	SetSelectCharVertex();

	return;
}

//=============================================================================
// 自機選択背景の描画処理
//=============================================================================
void DrawSelectCharBG(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, SelectBGTexture);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, SelectBGVertexWk, sizeof(VERTEX_2D));

	Device->SetTexture(0, DrawBoxTexture);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, DrawBoxVertexWk, sizeof(VERTEX_2D));

	return;
}

//=============================================================================
// 自機選択メニューの描画処理
//=============================================================================
void DrawSelectCharMenu(void)
{
	int i = 0;
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPDIRECT3DTEXTURE9 BlackScreenTexture = GetUITexture(BlackScreen);
	LPDIRECT3DTEXTURE9 SelectTexture = GetUITexture(SelectBox);
	LPD3DXFONT Title = GetFont(FontSize_54);
	LPD3DXFONT Option = GetFont(FontSize_40);
	PLAYER *Player = GetPlayer(0);
	RECT TextRect = { 0, 0, Screen_Width, RectHeight };

	TextRect.top = TitlePos_Y;
	TextRect.bottom = TitlePos_Y + RectHeight;
	Title->DrawText(NULL, "自機のタイプを選んでください", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));

	TextRect.top = OptionPos_Y;
	TextRect.bottom = OptionPos_Y + RectHeight;
	TextRect.left += 100;
	TextRect.right -= 100;
	//if (SelectChar.Phase == ChooseTypeA || (SelectChar.Phase == ChooseOver && Player->Type == Player_A))
	if (Player->Type == Player_A)
	{
		Option->DrawText(NULL, "タイプA 攻撃力重視型", -1, &TextRect, DT_LEFT | DT_VCENTER, BLUE(255));
		Option->DrawText(NULL, "タイプB 誘導重視型", -1, &TextRect, DT_RIGHT | DT_VCENTER, WHITE(64));
	}
	//else if (SelectChar.Phase == ChooseTypeB || (SelectChar.Phase == ChooseOver && Player->Type == Player_B))
	else if (Player->Type == Player_B)
	{
		Option->DrawText(NULL, "タイプA 攻撃力重視型", -1, &TextRect, DT_LEFT | DT_VCENTER, WHITE(64));
		Option->DrawText(NULL, "タイプB 誘導重視型", -1, &TextRect, DT_RIGHT | DT_VCENTER, BLUE(255));
	}

	if (SelectChar.Phase == ChooseCheck || SelectChar.Phase == ChooseOver)
	{
		Device->SetTexture(0, BlackScreenTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BlackScreenVertexWk, sizeof(VERTEX_2D));

		Device->SetTexture(0, SelectTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, SelectChar.VertexWk, sizeof(VERTEX_2D));

		TextRect.top = AskSentencePos_Y;
		TextRect.bottom = AskSentencePos_Y + RectHeight;
		if (SelectChar.Phase == ChooseCheck)
		{
			Title->DrawText(NULL, "本当によろしいですか？", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		else if (SelectChar.Phase == ChooseOver)
		{
			Title->DrawText(NULL, "チュートリアルをスキップしますか？", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
		}

		TextRect.top = YesNoPos_Y;
		TextRect.bottom = YesNoPos_Y + RectHeight;
		TextRect.left = 0;
		TextRect.right = ScreenCenter_X;
		if (SelectChar.InYes == true)
		{
			Title->DrawText(NULL, "はい", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Title->DrawText(NULL, "はい", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
		}

		TextRect.left = ScreenCenter_X;
		TextRect.right = Screen_Width;
		if (SelectChar.InYes == true)
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
// プレイヤーとエネミーの設置
//=============================================================================
void SetObject(void)
{
	PLAYER *Player = GetPlayer(0);
	ENEMY *Enemy = GetEnemy(0);
	D3DXVECTOR2 temp = D3DXVECTOR2(0.0f, 0.0f);

	temp = D3DXVECTOR2(Texture_Player_Width / 2, Texture_Player_Height / 2);
	Player->Exist = true;
	Player->Pos = D3DXVECTOR3(200.0f, 512.0f, 0.0f);
	Player->BaseAngle = atan2f(Texture_Player_Height, Texture_Player_Width);
	Player->Radius = D3DXVec2Length(&temp);
	Player->Direction = Flying;
	Player->Power = PlayerStartPower;
	Player->Type = Player_A;
	Player->BulletType = PlayerBulletType_A;

	temp = D3DXVECTOR2(Texture_PlayerOption_Width / 2, Texture_PlayerOption_Height / 2);
	Player->Option[Option_A].Use = true;
	Player->Option[Option_A].Pos.x = Player->Pos.x + OptionPos_x[Option_A];
	Player->Option[Option_A].Pos.y = Player->Pos.y + OptionPos_y[Option_A];
	Player->Option[Option_A].Radius = D3DXVec2Length(&temp);
	Player->Option[Option_A].BaseAngle = atan2f(Texture_PlayerOption_Height, Texture_PlayerOption_Width);
	Player->Option[Option_A].BulletType = OptionBulletType_A;
	Player->Option[Option_B].Use = true;
	Player->Option[Option_B].Pos.x = Player->Pos.x + OptionPos_x[Option_B];
	Player->Option[Option_B].Pos.y = Player->Pos.y + OptionPos_y[Option_B];
	Player->Option[Option_B].Radius = Player->Option[Option_A].Radius;
	Player->Option[Option_B].BaseAngle = Player->Option[Option_A].BaseAngle;
	Player->Option[Option_B].BulletType = OptionBulletType_A;

	temp = D3DXVECTOR2(Texture_Enemy03_Width / 2, Texture_Enemy03_Height / 2);
	Enemy->Exist = true;
	Enemy->EnemyType = Enemy03;
	Enemy->HP = 1.0f;
	Enemy->Pos = D3DXVECTOR3(800.0f, 512.0f, 0.0f);
	Enemy->Radius = D3DXVec2Length(&temp) * 1.6f;
	Enemy->HitRadius = Enemy->Radius * 0.5f;
	Enemy->BaseAngle = atan2f(Texture_Enemy03_Height, Texture_Enemy03_Width);
	Enemy->Direction = Left;

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeSelectCharVertex(void)
{
	// 頂点座標の設定	
	SetSelectCharVertex();

	// rhwの設定
	SelectBGVertexWk[0].rhw = 1.0f;
	SelectBGVertexWk[1].rhw = 1.0f;
	SelectBGVertexWk[2].rhw = 1.0f;
	SelectBGVertexWk[3].rhw = 1.0f;

	DrawBoxVertexWk[0].rhw = 1.0f;
	DrawBoxVertexWk[1].rhw = 1.0f;
	DrawBoxVertexWk[2].rhw = 1.0f;
	DrawBoxVertexWk[3].rhw = 1.0f;

	BlackScreenVertexWk[0].rhw = 1.0f;
	BlackScreenVertexWk[1].rhw = 1.0f;
	BlackScreenVertexWk[2].rhw = 1.0f;
	BlackScreenVertexWk[3].rhw = 1.0f;

	SelectChar.VertexWk[0].rhw = 1.0f;
	SelectChar.VertexWk[1].rhw = 1.0f;
	SelectChar.VertexWk[2].rhw = 1.0f;
	SelectChar.VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	SelectBGVertexWk[0].diffuse = WHITE(255);
	SelectBGVertexWk[1].diffuse = WHITE(255);
	SelectBGVertexWk[2].diffuse = WHITE(255);
	SelectBGVertexWk[3].diffuse = WHITE(255);

	DrawBoxVertexWk[0].diffuse = SKYBLUE(255);
	DrawBoxVertexWk[1].diffuse = SKYBLUE(255);
	DrawBoxVertexWk[2].diffuse = SKYBLUE(255);
	DrawBoxVertexWk[3].diffuse = SKYBLUE(255);

	BlackScreenVertexWk[0].diffuse = WHITE(200);
	BlackScreenVertexWk[1].diffuse = WHITE(200);
	BlackScreenVertexWk[2].diffuse = WHITE(200);
	BlackScreenVertexWk[3].diffuse = WHITE(200);

	SelectChar.VertexWk[0].diffuse = WHITE(255);
	SelectChar.VertexWk[1].diffuse = WHITE(255);
	SelectChar.VertexWk[2].diffuse = WHITE(255);
	SelectChar.VertexWk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetSelectCharTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetSelectCharTexture(void)
{
	int i = 0;
	PLAYER *Player = GetPlayer(0);
	ENEMY *Enemy = GetEnemy(0);
	int x = Player->AnimPattern;
	int y = Player->Direction;
	float sizeX = 1.0f / Texture_Player_Divide_X;
	float sizeY = 1.0f / Texture_Player_Divide_Y;

	SelectBGVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	SelectBGVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	SelectBGVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	SelectBGVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	DrawBoxVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	DrawBoxVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	DrawBoxVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	DrawBoxVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	BlackScreenVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	BlackScreenVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	BlackScreenVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	BlackScreenVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	SelectChar.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	SelectChar.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	SelectChar.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	SelectChar.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	Player->VertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	Player->VertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	Player->VertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	Player->VertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

	for (i = 0; i < PlayerOption_Max; i++)
	{
		Player->Option[i].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Player->Option[i].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Player->Option[i].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Player->Option[i].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	x = Enemy->AnimPattern;
	y = Enemy->Direction;
	sizeX = 1.0f / Texture_Enemy03_Divide_X;
	sizeY = 1.0f / Texture_Enemy03_Divide_Y;

	Enemy->VertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	Enemy->VertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	Enemy->VertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	Enemy->VertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetSelectCharVertex(void)
{
	int i = 0;
	PLAYER *Player = GetPlayer(0);
	ENEMY *Enemy = GetEnemy(0);

	SelectBGVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SelectBGVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	SelectBGVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	SelectBGVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	DrawBoxVertexWk[0].vtx = D3DXVECTOR3(DrawBox_X, DrawBox_Y, 0.0f);
	DrawBoxVertexWk[1].vtx = D3DXVECTOR3(DrawBox_X + Texture_DrawBox_Width, DrawBox_Y, 0.0f);
	DrawBoxVertexWk[2].vtx = D3DXVECTOR3(DrawBox_X, DrawBox_Y + Texture_DrawBox_Height, 0.0f);
	DrawBoxVertexWk[3].vtx = D3DXVECTOR3(DrawBox_X + Texture_DrawBox_Width, DrawBox_Y + Texture_DrawBox_Height, 0.0f);

	BlackScreenVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BlackScreenVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	BlackScreenVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	BlackScreenVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	SelectChar.VertexWk[0].vtx = D3DXVECTOR3(SelectChar.Pos.x - Texture_SelectBox_Width / 2, (float)SelectChar.Pos.y, 0.0f);
	SelectChar.VertexWk[1].vtx = D3DXVECTOR3(SelectChar.Pos.x + Texture_SelectBox_Width / 2, (float)SelectChar.Pos.y, 0.0f);
	SelectChar.VertexWk[2].vtx = D3DXVECTOR3(SelectChar.Pos.x - Texture_SelectBox_Width / 2, (float)SelectChar.Pos.y + Texture_SelectBox_Height, 0.0f);
	SelectChar.VertexWk[3].vtx = D3DXVECTOR3(SelectChar.Pos.x + Texture_SelectBox_Width / 2, (float)SelectChar.Pos.y + Texture_SelectBox_Height, 0.0f);

	// プレイヤー
	Player->VertexWk[0].vtx.x = Player->Pos.x - cosf(Player->BaseAngle + Player->Rot.z) * Player->Radius;
	Player->VertexWk[0].vtx.y = Player->Pos.y - sinf(Player->BaseAngle + Player->Rot.z) * Player->Radius;
	Player->VertexWk[1].vtx.x = Player->Pos.x + cosf(Player->BaseAngle - Player->Rot.z) * Player->Radius;
	Player->VertexWk[1].vtx.y = Player->Pos.y - sinf(Player->BaseAngle - Player->Rot.z) * Player->Radius;
	Player->VertexWk[2].vtx.x = Player->Pos.x - cosf(Player->BaseAngle - Player->Rot.z) * Player->Radius;
	Player->VertexWk[2].vtx.y = Player->Pos.y + sinf(Player->BaseAngle - Player->Rot.z) * Player->Radius;
	Player->VertexWk[3].vtx.x = Player->Pos.x + cosf(Player->BaseAngle + Player->Rot.z) * Player->Radius;
	Player->VertexWk[3].vtx.y = Player->Pos.y + sinf(Player->BaseAngle + Player->Rot.z) * Player->Radius;

	// オプション
	for (i = 0; i < PlayerOption_Max; i++)
	{
		Player->Option[i].VertexWk[0].vtx.x =
			Player->Option[i].Pos.x - cosf(Player->Option[i].BaseAngle + Player->Option[i].Rot.z) * Player->Option[i].Radius;
		Player->Option[i].VertexWk[0].vtx.y =
			Player->Option[i].Pos.y - sinf(Player->Option[i].BaseAngle + Player->Option[i].Rot.z) * Player->Option[i].Radius;
		Player->Option[i].VertexWk[1].vtx.x =
			Player->Option[i].Pos.x + cosf(Player->Option[i].BaseAngle - Player->Option[i].Rot.z) * Player->Option[i].Radius;
		Player->Option[i].VertexWk[1].vtx.y =
			Player->Option[i].Pos.y - sinf(Player->Option[i].BaseAngle - Player->Option[i].Rot.z) * Player->Option[i].Radius;
		Player->Option[i].VertexWk[2].vtx.x =
			Player->Option[i].Pos.x - cosf(Player->Option[i].BaseAngle - Player->Option[i].Rot.z) * Player->Option[i].Radius;
		Player->Option[i].VertexWk[2].vtx.y =
			Player->Option[i].Pos.y + sinf(Player->Option[i].BaseAngle - Player->Option[i].Rot.z) * Player->Option[i].Radius;
		Player->Option[i].VertexWk[3].vtx.x =
			Player->Option[i].Pos.x + cosf(Player->Option[i].BaseAngle + Player->Option[i].Rot.z) * Player->Option[i].Radius;
		Player->Option[i].VertexWk[3].vtx.y =
			Player->Option[i].Pos.y + sinf(Player->Option[i].BaseAngle + Player->Option[i].Rot.z) * Player->Option[i].Radius;
	}

	// エネミー
	Enemy->VertexWk[0].vtx.x = Enemy->Pos.x - cosf(Enemy->BaseAngle + Enemy->Rot.z) * Enemy->Radius;
	Enemy->VertexWk[0].vtx.y = Enemy->Pos.y - sinf(Enemy->BaseAngle + Enemy->Rot.z) * Enemy->Radius;
	Enemy->VertexWk[1].vtx.x = Enemy->Pos.x + cosf(Enemy->BaseAngle - Enemy->Rot.z) * Enemy->Radius;
	Enemy->VertexWk[1].vtx.y = Enemy->Pos.y - sinf(Enemy->BaseAngle - Enemy->Rot.z) * Enemy->Radius;
	Enemy->VertexWk[2].vtx.x = Enemy->Pos.x - cosf(Enemy->BaseAngle - Enemy->Rot.z) * Enemy->Radius;
	Enemy->VertexWk[2].vtx.y = Enemy->Pos.y + sinf(Enemy->BaseAngle - Enemy->Rot.z) * Enemy->Radius;
	Enemy->VertexWk[3].vtx.x = Enemy->Pos.x + cosf(Enemy->BaseAngle + Enemy->Rot.z) * Enemy->Radius;
	Enemy->VertexWk[3].vtx.y = Enemy->Pos.y + sinf(Enemy->BaseAngle + Enemy->Rot.z) * Enemy->Radius;

	return;
}

//=============================================================================
// エネミーの色を設置
//=============================================================================
void SetEnemyColor(void)
{
	ENEMY *Enemy = GetEnemy(0);

	Enemy->VertexWk[0].diffuse = WHITE(255);
	Enemy->VertexWk[1].diffuse = WHITE(255);
	Enemy->VertexWk[2].diffuse = WHITE(255);
	Enemy->VertexWk[3].diffuse = WHITE(255);

	return;
}

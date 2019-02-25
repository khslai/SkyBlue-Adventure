//=============================================================================
//
// プレイヤー処理 [Player.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Player.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Bomb.h"
#include "DebugTest.h"
#include "BackGround.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// アニメーションを切り替えるカウント
#define AnimChangeCount		(15)
// プレイヤーの移動速度
#define PlayerSpeed			(7.0f)
// 低速移動係数、移動速度は半分になる
#define LowSpeed			(0.5f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakePlayerVertex(void);
// 頂点座標の設定
void SetPlayerVertex(void);
// テクスチャ座標の設定
void SetPlayerTexture(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 PlayerDebut_ATexture = NULL;
LPDIRECT3DTEXTURE9 PlayerDebut_BTexture = NULL;
LPDIRECT3DTEXTURE9 Player_ATexture = NULL;
LPDIRECT3DTEXTURE9 Player_BTexture = NULL;
LPDIRECT3DTEXTURE9 PlayerOptionTexture = NULL;
LPDIRECT3DTEXTURE9 HitPointTexture = NULL;
// プレイヤー構造体
PLAYER Player[Player_Max];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(int InitState)
{
	int i = 0;
	int GameStage = GetGameStage();
	PLAYER *Player = GetPlayer(0);

	Player->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Player->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Player->Radius = 0.0f;
	Player->HitRadius = 0.0f;
	Player->BaseAngle = 0.0f;
	Player->HP = PlayerHP_Max;
	Player->Power = PlayerStartPower;
	Player->BombNum = PlayerStartBomb;
	if (GameStage == Stage_Title)
	{
		Player->Type = 0;
	}
	Player->AnimCount = 0;
	Player->AnimPattern = 0;
	Player->Direction = 0;
	Player->ShotCount = 0;
	Player->InvincibleCount = 0;
	Player->Score = 0;
	Player->BulletType = 0;
	Player->Exist = false;
	Player->LowSpeedMode = false;
	Player->InBomb = false;
	Player->ClearStandBy = false;

	for (i = 0; i < PlayerOption_Max; i++)
	{
		Player->Option[i].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Player->Option[i].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Player->Option[i].Radius = 0.0f;
		Player->Option[i].BaseAngle = 0.0f;
		Player->Option[i].BulletType = 0;
		Player->Option[i].Use = false;
	}

	Player->HitPoint.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Player->HitPoint.Radius = 0.0f;
	Player->HitPoint.BaseAngle = 0.0f;

	// 頂点情報の作成
	MakePlayerVertex();

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_PlayerDebut_A, &PlayerDebut_ATexture, "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_PlayerDebut_B, &PlayerDebut_BTexture, "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Player_A, &Player_ATexture, "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Player_B, &Player_BTexture, "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_PlayerOption, &PlayerOptionTexture, "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_HitPoint, &HitPointTexture, "Player") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	SafeRelease(PlayerDebut_ATexture);
	SafeRelease(PlayerDebut_BTexture);
	SafeRelease(Player_ATexture);
	SafeRelease(Player_BTexture);
	SafeRelease(PlayerOptionTexture);
	SafeRelease(HitPointTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	int GameCount = GetGameCount();
	int GameStage = GetGameStage();
	float MoveSpeed = PlayerSpeed;
	static bool Flag_UpDown = false;
	static bool Flag_LeftRight = false;
	PLAYER *Player = GetPlayer(0);
	GAMECLEAR *GameClear = GetGameClear();
	D3DXVECTOR3 PreviousPos = Player->Pos;

	if (Player->Exist == true)
	{
		// テクスチャアニメパターン計算
		Player->AnimCount++;
		if ((Player->AnimCount % AnimChangeCount) == 0)
		{
			Player->AnimPattern = (Player->AnimPattern + 1) % Texture_Player_Divide_X;
		}

		// 一時停止
		if ((GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_START)) && GameStage == Stage_Game)
		{
			PlaySound(SE_Menu, E_DS8_FLAG_NONE, true);
			SetGameStage(Stage_Pause);
			return;
		}

		// 撃たれた、無敵状態になる
		if (Player->InvincibleCount > 0)
		{
			Player->InvincibleCount++;
			// 2秒後
			if (Player->InvincibleCount >= PlayerInvCount)
			{
				Player->InvincibleCount = 0;
			}
		}

		// 同時に左右と上下を押したら、移動速度はMoveSpeed / root2になる
		if (Flag_UpDown == true && Flag_LeftRight == true)
		{
			MoveSpeed /= Root2;
		}

		// 低速モードだたっら、減速
		if (Player->LowSpeedMode == true)
		{
			MoveSpeed *= LowSpeed;
		}

		// パワーは1.0fより大きいなら、オプションA設置
		if (Player->Power >= 1.0f)
		{
			Player->Option[Option_A].Use = true;
			// 通常モード
			if (Player->LowSpeedMode == false)
			{
				Player->Option[Option_A].Pos.x = Player->Pos.x + OptionPos_x[Option_A];
				Player->Option[Option_A].Pos.y = Player->Pos.y + OptionPos_y[Option_A];
				Player->Option[Option_B].Use = false;
			}
			// 低速モード
			else
			{
				Player->Option[Option_A].Pos.x = Player->Pos.x + OptionPos_x_LowSpeed[Option_A];
				Player->Option[Option_A].Pos.y = Player->Pos.y + OptionPos_y_LowSpeed[Option_A];
				Player->Option[Option_B].Use = false;
			}

			// パワーは2.0fより大きいなら、オプションB設置
			if (Player->Power >= 2.0f)
			{
				Player->Option[Option_B].Use = true;
				// 通常モード
				if (Player->LowSpeedMode == false)
				{
					Player->Option[Option_B].Pos.x = Player->Pos.x + OptionPos_x[Option_B];
					Player->Option[Option_B].Pos.y = Player->Pos.y + OptionPos_y[Option_B];
				}
				// 低速モード
				else
				{
					Player->Option[Option_B].Pos.x = Player->Pos.x + OptionPos_x_LowSpeed[Option_B];
					Player->Option[Option_B].Pos.y = Player->Pos.y + OptionPos_y_LowSpeed[Option_B];
				}
			}
		}
		else
		{
			Player->Option[Option_A].Use = false;
			Player->Option[Option_B].Use = false;
		}

		if (GameCount < GameClearCount)
		{
			// 弾発射
			if (GetKeyboardPress(DIK_Z) || IsButtonPressed(0, BUTTON_B))
			{
				Player->ShotCount++;
				if (Player->ShotCount % 4 == 0)
				{
					PlayerShot();
				}
			}
			else
			{
				Player->ShotCount = 0;
			}

			// ボム発射
			if ((GetKeyboardTrigger(DIK_X) || IsButtonTriggered(0, BUTTON_Y)) &&
				Player->BombNum > 0 && Player->InBomb == false)
			{
				if (GameStage == Stage_Game || GameStage == Stage_BossPratice)
				{
					//Player->BombNum--;
				}
				Player->InBomb = true;
				if (Player->Type == Player_A)
				{
					SetBomb_A();
				}
				else if (Player->Type == Player_B)
				{
					SetBomb_B();
				}
			}

			// 低速モード
			if (GetKeyboardPress(DIK_LSHIFT) || GetKeyboardPress(DIK_RSHIFT) ||
				IsButtonPressed(0, BUTTON_LB) || IsButtonPressed(0, BUTTON_RB))
			{
				Player->LowSpeedMode = true;
			}
			else if (GetKeyboardRelease(DIK_LSHIFT) || GetKeyboardRelease(DIK_RSHIFT) ||
				IsButtonReleased(0, BUTTON_LB) || IsButtonReleased(0, BUTTON_RB))
			{
				Player->LowSpeedMode = false;;
			}

			// プレイヤー上下移動
			if (GetKeyboardPress(DIK_UP) || IsButtonPressed(0, BUTTON_UP))
			{
				Player->Pos.y -= MoveSpeed;
				Flag_UpDown = true;
				if (CheckRange(InRange, Player->Pos, Texture_Player_Width, Texture_Player_Height) == false)
				{
					Player->Pos.y = PreviousPos.y;
				}
			}
			else if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(0, BUTTON_DOWN))
			{
				Player->Pos.y += MoveSpeed;
				Flag_UpDown = true;
				if (CheckRange(InRange, Player->Pos, Texture_Player_Width, Texture_Player_Height) == false)
				{
					Player->Pos.y = PreviousPos.y;
				}
			}

			// プレイヤー左右移動
			if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT))
			{
				Player->Pos.x -= MoveSpeed;
				Player->Direction = Backward;
				Flag_LeftRight = true;
				if (CheckRange(InRange, Player->Pos, Texture_Player_Width, Texture_Player_Height) == false)
				{
					Player->Pos.x = PreviousPos.x;
				}
			}
			else if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT))
			{
				Player->Pos.x += MoveSpeed;
				Player->Direction = Forward;
				Flag_LeftRight = true;
				if (CheckRange(InRange, Player->Pos, Texture_Player_Width, Texture_Player_Height) == false)
				{
					Player->Pos.x = PreviousPos.x;
				}
			}

			// 移動キーが離れたら
			if (GetKeyboardRelease(DIK_RIGHT) || GetKeyboardRelease(DIK_LEFT) ||
				IsButtonReleased(0, BUTTON_RIGHT) || IsButtonReleased(0, BUTTON_LEFT))
			{
				Player->Direction = Flying;
				Flag_LeftRight = false;
			}
			if (GetKeyboardRelease(DIK_UP) || GetKeyboardRelease(DIK_DOWN) ||
				IsButtonReleased(0, BUTTON_UP) || IsButtonReleased(0, BUTTON_DOWN))
			{
				Flag_UpDown = false;
			}
		}
		// ゲームクリア時間になったら
		else
		{
			// 指定位置(100,384)に移動
			if (Player->Pos.x != 200.0f)
			{
				if (Player->Pos.x < 100.0f - 3.0f)
				{
					Player->Pos.x++;
					Player->Direction = Forward;
				}
				else if (Player->Pos.x > 100.0f + 3.0f)
				{
					Player->Pos.x -= 3.0f;
					Player->Direction = Backward;
				}
				else
				{
					Player->Pos.x = 100.0f;
					Player->Direction = Flying;
				}
			}
			if (Player->Pos.y != ScreenCenter_Y)
			{
				if (Player->Pos.y < ScreenCenter_Y - 3.0f)
				{
					Player->Pos.y++;
				}
				else if (Player->Pos.y > ScreenCenter_Y + 3.0f)
				{
					Player->Pos.y--;
				}
				else
				{
					Player->Pos.y = ScreenCenter_Y;
				}
			}
			// 移動終わったら
			if (Player->Pos.x == 100.0f && Player->Pos.y == ScreenCenter_Y)
			{
				// 準備完了
				Player->ClearStandBy = true;
				// 効果音
				if (IsPlaying(SE_GameClear_01) == false)
				{
					ReInitSound();
					PlaySound(SE_GameClear_01, E_DS8_FLAG_NONE, true);
				}
			}
			if (Player->ClearStandBy == true && GameClear->FadeInOver == true)
			{
				// 右にダッシュ
				Player->Pos.x += 15.0f;
				Player->Direction = Forward;
			}
		}
	}
	else if (Player->Exist == false && GameCount >= PlayerDebutCount)
	{
		// テクスチャアニメパターン計算
		Player->AnimCount++;
		if ((Player->AnimCount % 5) == 0)
		{
			Player->AnimPattern++;
			if (Player->AnimPattern == Texture_PlayerDebut_Divide_X)
			{
				SetPlayer();
			}
		}
	}

	SetPlayerTexture();
	SetPlayerVertex();

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int i = 0;
	int GameCount = GetGameCount();
	PLAYER *Player = GetPlayer(0);
	static bool PlayerFlash = false;

	if (Player->Exist == true)
	{
		// 撃たれた、無敵カウント始まる
		if (Player->InvincibleCount > 0)
		{
			// 5カウントずつ、描画する←→描画しないの状態を切り替える
			if (Player->InvincibleCount % 5 == 0)
			{
				PlayerFlash = PlayerFlash == false ? true : false;
			}
		}
		// 普通状態
		else
		{
			PlayerFlash = false;
		}

		if (PlayerFlash == false)
		{
			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_2D);

			// プレイヤー
			if (Player->Exist == true)
			{
				// テクスチャの設定
				if (Player->Type == Player_A)
				{
					Device->SetTexture(0, Player_ATexture);
				}
				else if (Player->Type == Player_B)
				{
					Device->SetTexture(0, Player_BTexture);
				}

				// ポリゴンの描画
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Player->VertexWk, sizeof(VERTEX_2D));
			}

			// 当たりポイント
			if (Player->LowSpeedMode == true)
			{
				// テクスチャの設定
				Device->SetTexture(0, HitPointTexture);

				// ポリゴンの描画
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Player->HitPoint.VertexWk, sizeof(VERTEX_2D));
			}

			// オプション
			for (i = 0; i < PlayerOption_Max; i++)
			{
				if (Player->Option[i].Use == true)
				{
					// テクスチャの設定
					Device->SetTexture(0, PlayerOptionTexture);

					// ポリゴンの描画
					Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Player->Option[i].VertexWk, sizeof(VERTEX_2D));
				}
			}
		}
	}
	else if (Player->Exist == false && GameCount >= PlayerDebutCount)
	{
		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		if (Player->Type == Player_A)
		{
			Device->SetTexture(0, PlayerDebut_ATexture);
		}
		else if (Player->Type == Player_B)
		{
			Device->SetTexture(0, PlayerDebut_BTexture);
		}

		// ポリゴンの描画
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Player->VertexWk, sizeof(VERTEX_2D));
	}

	return;
}

//=============================================================================
// プレイヤー登場
//=============================================================================
void SetPlayer(void)
{
	int i = 0;
	PLAYER *Player = GetPlayer(0);
	D3DXVECTOR2 temp = D3DXVECTOR2(0.0f, 0.0f);

	temp = D3DXVECTOR2(Texture_Player_Width / 2, Texture_Player_Height / 2);

	Player->Pos = D3DXVECTOR3(PlayerDebutPos_X + 10, PlayerDebutPos_Y, 0.0f);
	Player->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Player->Radius = D3DXVec2Length(&temp);
	Player->HitRadius = Player->Radius / 10.0f;
	Player->BaseAngle = atan2f(Texture_Player_Height, Texture_Player_Width);
	Player->Power = PlayerStartPower;
	Player->BombNum = PlayerStartBomb;
	Player->AnimCount = 0;
	Player->AnimPattern = 0;
	Player->Direction = Flying;
	Player->ShotCount = 0;
	Player->InvincibleCount = 0;
	Player->Score = 0;
	Player->HP = PlayerHP_Max;
	if (Player->Type == Player_A)
	{
		Player->BulletType = PlayerBulletType_A;
	}
	else if (Player->Type == Player_B)
	{
		Player->BulletType = PlayerBulletType_B;
	}
	Player->Exist = true;
	Player->LowSpeedMode = false;
	Player->InBomb = false;
	Player->ClearStandBy = false;

	temp = D3DXVECTOR2(Texture_PlayerOption_Width / 2, Texture_PlayerOption_Height / 2);
	for (i = 0; i < PlayerOption_Max; i++)
	{
		Player->Option[i].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Player->Option[i].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Player->Option[i].Radius = D3DXVec2Length(&temp);
		Player->Option[i].BaseAngle = atan2f(Texture_PlayerOption_Height, Texture_PlayerOption_Width);
		if (Player->Type == Player_A)
		{
			Player->Option[i].BulletType = OptionBulletType_A;
		}
		else if (Player->Type == Player_B)
		{
			Player->Option[i].BulletType = OptionBulletType_B;
		}
		Player->Option[i].Use = false;
	}

	Player->HitPoint.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Player->HitPoint.Radius = Player->HitRadius * Root2;
	Player->HitPoint.BaseAngle = atan2f(Texture_HitPoint_Height, Texture_HitPoint_Width);

#ifdef _DEBUG
	SetPlayerHC(Player->Pos, Player->HitRadius, PlayerHC);
#endif

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakePlayerVertex(void)
{
	int i = 0;
	PLAYER *Player = GetPlayer(0);

	// 頂点座標の設定	
	SetPlayerTexture();

	// rhwの設定
	Player->VertexWk[0].rhw = 1.0f;
	Player->VertexWk[1].rhw = 1.0f;
	Player->VertexWk[2].rhw = 1.0f;
	Player->VertexWk[3].rhw = 1.0f;

	Player->HitPoint.VertexWk[0].rhw = 1.0f;
	Player->HitPoint.VertexWk[1].rhw = 1.0f;
	Player->HitPoint.VertexWk[2].rhw = 1.0f;
	Player->HitPoint.VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	Player->VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Player->VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Player->VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Player->VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	Player->HitPoint.VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Player->HitPoint.VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Player->HitPoint.VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Player->HitPoint.VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	for (i = 0; i < PlayerOption_Max; i++)
	{
		Player->Option[i].VertexWk[0].rhw = 1.0f;
		Player->Option[i].VertexWk[1].rhw = 1.0f;
		Player->Option[i].VertexWk[2].rhw = 1.0f;
		Player->Option[i].VertexWk[3].rhw = 1.0f;

		Player->Option[i].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Player->Option[i].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Player->Option[i].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Player->Option[i].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// テクスチャ座標の設定
	SetPlayerVertex();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetPlayerTexture(void)
{
	int i = 0;
	PLAYER *Player = GetPlayer(0);
	int x = Player->AnimPattern;
	int y = Player->Direction;
	float sizeX = 1.0f / Texture_Player_Divide_X;
	float sizeY = 1.0f / Texture_Player_Divide_Y;

	if (Player->Exist == true)
	{
		// プレイヤー
		Player->VertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		Player->VertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		Player->VertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		Player->VertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 当たりポイント
		if (Player->LowSpeedMode == true)
		{
			Player->HitPoint.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Player->HitPoint.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			Player->HitPoint.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Player->HitPoint.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// オプション
		for (i = 0; i < PlayerOption_Max; i++)
		{
			if (Player->Option[i].Use == true)
			{
				Player->Option[i].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				Player->Option[i].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				Player->Option[i].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				Player->Option[i].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
	}
	else
	{
		sizeX = 1.0f / Texture_PlayerDebut_Divide_X;

		Player->VertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
		Player->VertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
		Player->VertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
		Player->VertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);
	}

	return;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetPlayerVertex(void)
{
	int i = 0;
	PLAYER *Player = GetPlayer(0);

	if (Player->Exist == true)
	{
		// プレイヤー
		Player->VertexWk[0].vtx.x =
			Player->Pos.x - cosf(Player->BaseAngle + Player->Rot.z) * Player->Radius;
		Player->VertexWk[0].vtx.y =
			Player->Pos.y - sinf(Player->BaseAngle + Player->Rot.z) * Player->Radius;
		Player->VertexWk[1].vtx.x =
			Player->Pos.x + cosf(Player->BaseAngle - Player->Rot.z) * Player->Radius;
		Player->VertexWk[1].vtx.y =
			Player->Pos.y - sinf(Player->BaseAngle - Player->Rot.z) * Player->Radius;
		Player->VertexWk[2].vtx.x =
			Player->Pos.x - cosf(Player->BaseAngle - Player->Rot.z) * Player->Radius;
		Player->VertexWk[2].vtx.y =
			Player->Pos.y + sinf(Player->BaseAngle - Player->Rot.z) * Player->Radius;
		Player->VertexWk[3].vtx.x =
			Player->Pos.x + cosf(Player->BaseAngle + Player->Rot.z) * Player->Radius;
		Player->VertexWk[3].vtx.y =
			Player->Pos.y + sinf(Player->BaseAngle + Player->Rot.z) * Player->Radius;

		// 当たりポイント
		if (Player->LowSpeedMode == true)
		{
			Player->HitPoint.VertexWk[0].vtx.x = Player->Pos.x - cosf(Player->HitPoint.BaseAngle + Player->HitPoint.Rot.z) * Player->HitPoint.Radius;
			Player->HitPoint.VertexWk[0].vtx.y = Player->Pos.y - sinf(Player->HitPoint.BaseAngle + Player->HitPoint.Rot.z) * Player->HitPoint.Radius;
			Player->HitPoint.VertexWk[1].vtx.x = Player->Pos.x + cosf(Player->HitPoint.BaseAngle - Player->HitPoint.Rot.z) * Player->HitPoint.Radius;
			Player->HitPoint.VertexWk[1].vtx.y = Player->Pos.y - sinf(Player->HitPoint.BaseAngle - Player->HitPoint.Rot.z) * Player->HitPoint.Radius;
			Player->HitPoint.VertexWk[2].vtx.x = Player->Pos.x - cosf(Player->HitPoint.BaseAngle - Player->HitPoint.Rot.z) * Player->HitPoint.Radius;
			Player->HitPoint.VertexWk[2].vtx.y = Player->Pos.y + sinf(Player->HitPoint.BaseAngle - Player->HitPoint.Rot.z) * Player->HitPoint.Radius;
			Player->HitPoint.VertexWk[3].vtx.x = Player->Pos.x + cosf(Player->HitPoint.BaseAngle + Player->HitPoint.Rot.z) * Player->HitPoint.Radius;
			Player->HitPoint.VertexWk[3].vtx.y = Player->Pos.y + sinf(Player->HitPoint.BaseAngle + Player->HitPoint.Rot.z) * Player->HitPoint.Radius;
		}

		// オプション
		for (i = 0; i < PlayerOption_Max; i++)
		{
			if (Player->Option[i].Use == true)
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
		}
	}
	else
	{
		Player->VertexWk[0].vtx =
			D3DXVECTOR3(PlayerDebutPos_X - Texture_PlayerDebut_Width / 2, PlayerDebutPos_Y - Texture_PlayerDebut_Height / 2, 0.0f);
		Player->VertexWk[1].vtx =
			D3DXVECTOR3(PlayerDebutPos_X + Texture_PlayerDebut_Width / 2, PlayerDebutPos_Y - Texture_PlayerDebut_Height / 2, 0.0f);
		Player->VertexWk[2].vtx =
			D3DXVECTOR3(PlayerDebutPos_X - Texture_PlayerDebut_Width / 2, PlayerDebutPos_Y + Texture_PlayerDebut_Height / 2, 0.0f);
		Player->VertexWk[3].vtx =
			D3DXVECTOR3(PlayerDebutPos_X + Texture_PlayerDebut_Width / 2, PlayerDebutPos_Y + Texture_PlayerDebut_Height / 2, 0.0f);
	}

	return;
}

//=============================================================================
// プレイヤーの情報を取得する
//=============================================================================
PLAYER *GetPlayer(int Player_No)
{
	return &Player[Player_No];
}


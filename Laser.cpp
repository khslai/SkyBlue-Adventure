//=============================================================================
//
// レーザー処理 [Laser.cpp]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#include "main.h"
#include "Laser.h"
#include "Player.h"
#include "Boss.h"
#include "Enemy.h"
#include "Sound.h"
#include "DebugTest.h"
#include "CheckHit.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 消失開始のカウント
#define DisappearCount (270)
// レーザーの発射座標とエネミー座標の距離
#define DistanceFromEnemy (30.0f)
// レーザー最大発射数 = Laser_Num_Max * 2
#define Laser_Num_Max (8)

enum
{
	Clockwise = 1,
	Counterclockwise = -1,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeLaserVertex(int	Laser_No);
void SetLaserTexture(int Laser_No);
void SetLaserVertex(int Laser_No);
void CalculateLaserRotate(int Laser_No);
void CalculateLaserRect(D3DXVECTOR3 Pos, float Length);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 LaserTexture = NULL;
LASER Laser[Laser_Max];
int Laser_Num = 0;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitLaser(int InitState)
{
	int Laser_No = 0;
	Laser_Num = 2;

	for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++)
	{
		Laser[Laser_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Laser[Laser_No].RotateCenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Laser[Laser_No].BeforeRotPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Laser[Laser_No].RotateTime = 0.0f;
		Laser[Laser_No].MoveAngle = 0.0f;
		Laser[Laser_No].BeforeRotAngle = 0.0f;
		Laser[Laser_No].RotateAngle = 0.0f;
		Laser[Laser_No].Width = 0.0f;
		Laser[Laser_No].Length = 0.0f;
		Laser[Laser_No].State = 0;
		Laser[Laser_No].Count = 0;
		Laser[Laser_No].Use = false;
		Laser[Laser_No].InRotate = false;

		MakeLaserVertex(Laser_No);
	}

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_Laser, &LaserTexture, "Laser") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLaser(void)
{
	SafeRelease(LaserTexture);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLaser(void)
{
	int Laser_No = 0;
	BOSS *Boss = GetBoss();

	for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++)
	{
		if (Laser[Laser_No].Use == true)
		{
			if (Boss->State == HPRecover || Boss->State == BossDisappear || Boss->State == LastBossOver)
			{
				Laser[Laser_No].Use = false;
				continue;
			}
			CalculateLaserRotate(Laser_No);
			Laser[Laser_No].Count++;

			SetLaserVertex(Laser_No);
			SetLaserTexture(Laser_No);
		}

	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLaser(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Laser_No = 0;

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++)
	{
		if (Laser[Laser_No].Use == true)
		{
			Device->SetTexture(0, LaserTexture);
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Laser[Laser_No].VertexWk, sizeof(VERTEX_2D));
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeLaserVertex(int Laser_No)
{
	// 頂点座標の設定	
	SetLaserVertex(Laser_No);

	// rhwの設定
	Laser[Laser_No].VertexWk[0].rhw = 1.0f;
	Laser[Laser_No].VertexWk[1].rhw = 1.0f;
	Laser[Laser_No].VertexWk[2].rhw = 1.0f;
	Laser[Laser_No].VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	Laser[Laser_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Laser[Laser_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Laser[Laser_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Laser[Laser_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetLaserTexture(Laser_No);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetLaserTexture(int Laser_No)
{
	Laser[Laser_No].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Laser[Laser_No].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Laser[Laser_No].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Laser[Laser_No].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetLaserVertex(int Laser_No)
{
	Laser[Laser_No].VertexWk[0].vtx.x =
		Laser[Laser_No].Pos.x + cosf(Laser[Laser_No].MoveAngle + PI / 2) * Laser[Laser_No].Width / 2;
	Laser[Laser_No].VertexWk[0].vtx.y =
		Laser[Laser_No].Pos.y + sinf(Laser[Laser_No].MoveAngle + PI / 2) * Laser[Laser_No].Width / 2;
	Laser[Laser_No].VertexWk[1].vtx.x =
		Laser[Laser_No].Pos.x + cosf(Laser[Laser_No].MoveAngle + PI / 2) * Laser[Laser_No].Width / 2 + cosf(Laser[Laser_No].MoveAngle) * Laser[Laser_No].Length;
	Laser[Laser_No].VertexWk[1].vtx.y =
		Laser[Laser_No].Pos.y + sinf(Laser[Laser_No].MoveAngle + PI / 2) * Laser[Laser_No].Width / 2 + sinf(Laser[Laser_No].MoveAngle) * Laser[Laser_No].Length;
	Laser[Laser_No].VertexWk[2].vtx.x =
		Laser[Laser_No].Pos.x + cosf(Laser[Laser_No].MoveAngle - PI / 2) * Laser[Laser_No].Width / 2;
	Laser[Laser_No].VertexWk[2].vtx.y =
		Laser[Laser_No].Pos.y + sinf(Laser[Laser_No].MoveAngle - PI / 2) * Laser[Laser_No].Width / 2;
	Laser[Laser_No].VertexWk[3].vtx.x =
		Laser[Laser_No].Pos.x + cosf(Laser[Laser_No].MoveAngle - PI / 2) * Laser[Laser_No].Width / 2 + cosf(Laser[Laser_No].MoveAngle) * Laser[Laser_No].Length;
	Laser[Laser_No].VertexWk[3].vtx.y =
		Laser[Laser_No].Pos.y + sinf(Laser[Laser_No].MoveAngle - PI / 2) * Laser[Laser_No].Width / 2 + sinf(Laser[Laser_No].MoveAngle) * Laser[Laser_No].Length;

	return;
}

//=============================================================================
// レーザーの設置
//=============================================================================
void SetLaser()
{
	int Laser_No = 0;
	int Angle = 0;
	int AngleHalf = 0;
	int MoveAngle = 0;
	int GameStage = GetGameStage();
	BOSS *Boss = GetBoss();

	// レーザー発射音
	if (GameStage != Stage_PraticeSelect)
	{
		PlaySound(SE_BossShot_01, E_DS8_FLAG_NONE, true);
	}

	// 初期化
	if (GameStage == Stage_PraticeSelect && Boss->ShotCount == 0)
	{
		Laser_Num = 2;
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < Laser_Num; j++)
		{
			for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++)
			{
				if (Laser[Laser_No].Use == false)
				{
					// 360度 ÷ レーザーの数 = 成す角
					Angle = 360 / (2 * Laser_Num);
					// 成す角の半分
					AngleHalf = Angle / 2;
					// 角度の計算
					MoveAngle = j * 2 * Angle + AngleHalf * (Laser_Num % 2) + i * Angle;
					// i = 0 -> 右回り、i = 1 -> 左回り
					Laser[Laser_No].State = i == 0 ? Clockwise : Counterclockwise;
					Laser[Laser_No].Use = true;
					Laser[Laser_No].InRotate = true;
					Laser[Laser_No].MoveAngle = DegreeToRadian(MoveAngle);
					Laser[Laser_No].BeforeRotAngle = Laser[Laser_No].MoveAngle;
					// エネミーの座標を中心で回転する
					Laser[Laser_No].RotateCenterPos.x = Boss->Pos.x;
					Laser[Laser_No].RotateCenterPos.y = Boss->Pos.y;
					// レーザーの発射座標
					Laser[Laser_No].Pos.x = Boss->Pos.x + cosf(Laser[Laser_No].MoveAngle) * DistanceFromEnemy;
					Laser[Laser_No].Pos.y = Boss->Pos.y + sinf(Laser[Laser_No].MoveAngle) * DistanceFromEnemy;
					Laser[Laser_No].BeforeRotPos = Laser[Laser_No].Pos;
					// 回転時間
					Laser[Laser_No].RotateTime = 60.0f;
					//Laser[Laser_No].RotateAngle = DegreeToRadian(0);
					// 回転角度は成す角、i = 0 -> 右回り、i = 1 -> 左回り
					Laser[Laser_No].RotateAngle = DegreeToRadian(Angle) * Laser[Laser_No].State;
					// レーザーの幅
					Laser[Laser_No].Width = Texture_Laser_Width / 10;
					// レーザーの長さ
					if (GameStage != Stage_PraticeSelect)
					{
						Laser[Laser_No].Length = Screen_Width;
					}
					else
					{
						Laser[Laser_No].Length = 250.0f;
					}
					Laser[Laser_No].Count = 0;
					break;
				}
			}
		}
	}

	CalculateLaserRect(Boss->Pos, Screen_Width);

	if (Laser_Num < Laser_Num_Max)
	{
		Laser_Num++;
	}

	return;
}

//=============================================================================
// レーザーの情報を取得する
//=============================================================================
LASER *GetLaser(int Laser_No)
{
	return &Laser[Laser_No];
}

//=============================================================================
// レーザーの回転、移動を計算する
//=============================================================================
void CalculateLaserRotate(int Laser_No)
{
	float Theta = 0.0f;
	float x0 = 0.0f;
	float y0 = 0.0f;
	// 回転したい角度
	float RotAngle = Laser[Laser_No].RotateAngle;
	// 回転必要の時間
	float RotTime = Laser[Laser_No].RotateTime;
	// 今は何カウント
	int Count = Laser[Laser_No].Count;

	// 回転角度計算
	Theta = (2 * RotAngle * Count / RotTime) - (RotAngle * Count * Count / (RotTime * RotTime));
	// 回転移動時間内なら
	if (RotTime > 0 && Laser[Laser_No].InRotate == true)
	{
		// 回転する
		Laser[Laser_No].MoveAngle = Laser[Laser_No].BeforeRotAngle + Theta;
		// 座標変換する
		// (Center_x,Center_y)を中心で回転する
		// 中心座標を引かないと、回転の基準点は(0,0)になる
		// x0 = 中心からレーザー発射座標のx成分
		x0 = Laser[Laser_No].BeforeRotPos.x - Laser[Laser_No].RotateCenterPos.x;
		// y0 = 中心からレーザー発射座標のy成分
		y0 = Laser[Laser_No].BeforeRotPos.y - Laser[Laser_No].RotateCenterPos.y;
		// 公式。(x0,y0)θ角を回転後(x1,y1)
		// x1 = x0cosθ - y0sinθ
		// y1 = x0sinθ + y0cosθ
		// 回転後x座標 = 中心座標 + x1
		Laser[Laser_No].Pos.x = Laser[Laser_No].RotateCenterPos.x + (x0 * cosf(Theta) - y0 * sinf(Theta));
		// 回転後y座標 = 中心座標 + y1
		Laser[Laser_No].Pos.y = Laser[Laser_No].RotateCenterPos.y + (x0 * sinf(Theta) + y0 * cosf(Theta));
	}

	if (Count > RotTime)
	{
		// 回転時間を過ぎるとやめる
		Laser[Laser_No].RotateTime = 0;
		Laser[Laser_No].InRotate = false;
	}

	// 消滅時間を超えたら、幅は少しずつ減る
	if (Count >= DisappearCount)
	{
		// 0.5秒以内消滅
		Laser[Laser_No].Width -= Texture_Laser_Width / 30.0f;
		// 0以下なら、レーザー消滅
		if (Laser[Laser_No].Width <= 0)
		{
			Laser[Laser_No].Use = false;
		}
	}
	else if (Laser[Laser_No].InRotate == false)
	{
		Laser[Laser_No].Width = Texture_Laser_Width;
	}

	return;
}


//=============================================================================
// レーザーの当たり判定用矩形を計算する
//=============================================================================
void CalculateLaserRect(D3DXVECTOR3 Pos, float Length)
{
	// 0度のレーザー矩形を計算する
	float Angle = 0;
	// レーザーの左側の真ん中
	float LaserPos_x = Pos.x + cosf(DegreeToRadian(Angle)) * DistanceFromEnemy;
	float LaserPos_y = Pos.y + sinf(DegreeToRadian(Angle)) * DistanceFromEnemy;
	// 矩形左上の頂点座標
	D3DXVECTOR3 RectLeftUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 矩形右下の頂点座標
	D3DXVECTOR3 RectRightDown = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 矩形左上の頂点座標
	RectLeftUp.x = LaserPos_x;
	RectLeftUp.y = LaserPos_y - Texture_Laser_Width / 2;
	// 矩形右下の頂点座標
	RectRightDown.x = LaserPos_x + Length;
	RectRightDown.y = LaserPos_y + Texture_Laser_Width / 2;
	// レーザー矩形の判定範囲を設置する
	SetLaserCheckRect(RectLeftUp, RectRightDown);

	return;
}


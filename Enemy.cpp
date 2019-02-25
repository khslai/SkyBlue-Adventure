//=============================================================================
//
// エネミー処理 [Enemy.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Enemy.h"
#include "File.h"
#include "Barrage.h"
#include "Laser.h"
#include "DebugTest.h"
#include "Boss.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// アニメーションを切り替えるカウント
#define AnimChangeCount (15)

// エネミー移動パターン9、10番の状態
enum
{
	Debut,			// エネミー登場
	FirstRotate,	// 第一回回転
	FirstRotEnd,	// 回転終了
	SecondRotate,	// 第二回回転
	EnemyExit,		// 移動終了
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeEnemyVertex(int Enemy_No);
// 頂点座標の設定
void SetEnemyVertex(int Enemy_No);
// テクスチャ座標の設定
void SetEnemyTexture(int Enemy_No);
// エネミーの移動パターン
void EnemyMove(int Enemy_No, int Pattern_No);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャへのポインタ
LPDIRECT3DTEXTURE9 EnemyTexture[EnemyType_Max] = { NULL };
// テクスチャ半径
static float Radius[EnemyType_Max] = { 0.0f };
// 当たり判定の半径
static float HitRadius[EnemyType_Max] = { 0.0f };
// 中心点と四頂点の成す角
static float BaseAngle[EnemyType_Max] = { 0.0f };
// テクスチャ半径拡大の倍率
static const float RadiusZoomRate[EnemyType_Max] = { 1.2f, 1.2f, 1.5f, 1.6f, 1.6f };
// 当たり判定の半径拡大の倍率、HR = HitRadius
static const float HRZoomRate[EnemyType_Max] = { 0.6f, 0.6f, 0.5f, 0.5f, 0.5f };
// エネミー構造体
ENEMY Enemy[Enemy_Max];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(int InitState)
{
	int Enemy_No = 0;
	D3DXVECTOR2 temp = D3DXVECTOR2(0.0f, 0.0f);

	for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++)
	{
		Enemy[Enemy_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Enemy[Enemy_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Enemy[Enemy_No].Exist = false;
		Enemy[Enemy_No].Speed = 0.0f;
		Enemy[Enemy_No].BulletSpeed = 0.0f;
		Enemy[Enemy_No].BaseAngle = 0.0f;
		Enemy[Enemy_No].MoveAngle = 0.0f;
		Enemy[Enemy_No].Radius = 0.0f;
		Enemy[Enemy_No].HitRadius = 0.0f;
		Enemy[Enemy_No].Direction = 0;
		Enemy[Enemy_No].Width = 0;
		Enemy[Enemy_No].Height = 0;
		Enemy[Enemy_No].AnimPattern = 0;
		Enemy[Enemy_No].Count = 0;
		Enemy[Enemy_No].MovePattern = 0;
		Enemy[Enemy_No].EnemyType = 0;
		Enemy[Enemy_No].ShotTime = 0;
		Enemy[Enemy_No].BarrageType = 0;
		Enemy[Enemy_No].HP = 0.0f;
		Enemy[Enemy_No].BulletType = 0;
		Enemy[Enemy_No].State = 0;
		Enemy[Enemy_No].WaitCount = 0;
		Enemy[Enemy_No].ExitCount = 0;
		for (int i = 0; i < EnemyItem_Max; i++)
		{
			Enemy[Enemy_No].Item[i] = Nothing;
		}

		// 頂点情報の作成
		MakeEnemyVertex(Enemy_No);
	}

	// 半径、当たり範囲、角度の計算
	temp = D3DXVECTOR2(Texture_Enemy00_Width / 2, Texture_Enemy00_Height / 2);
	Radius[Enemy00] = D3DXVec2Length(&temp) * RadiusZoomRate[Enemy00];
	HitRadius[Enemy00] = Radius[Enemy00] * HRZoomRate[Enemy00];
	BaseAngle[Enemy00] = atan2f(Texture_Enemy00_Height, Texture_Enemy00_Width);

	temp = D3DXVECTOR2(Texture_Enemy01_Width / 2, Texture_Enemy01_Height / 2);
	Radius[Enemy01] = D3DXVec2Length(&temp) * RadiusZoomRate[Enemy01];
	HitRadius[Enemy01] = Radius[Enemy01] * HRZoomRate[Enemy01];
	BaseAngle[Enemy01] = atan2f(Texture_Enemy01_Height, Texture_Enemy01_Width);

	temp = D3DXVECTOR2(Texture_Enemy02_Width / 2, Texture_Enemy02_Height / 2);
	Radius[Enemy02] = D3DXVec2Length(&temp) * RadiusZoomRate[Enemy02];
	HitRadius[Enemy02] = Radius[Enemy02] * HRZoomRate[Enemy02];
	BaseAngle[Enemy02] = atan2f(Texture_Enemy02_Height, Texture_Enemy02_Width);

	temp = D3DXVECTOR2(Texture_Enemy03_Width / 2, Texture_Enemy03_Height / 2);
	Radius[Enemy03] = D3DXVec2Length(&temp) * RadiusZoomRate[Enemy03];
	HitRadius[Enemy03] = Radius[Enemy03] * HRZoomRate[Enemy03];
	BaseAngle[Enemy03] = atan2f(Texture_Enemy03_Height, Texture_Enemy03_Width);

	temp = D3DXVECTOR2(Texture_Enemy04_Width / 2, Texture_Enemy04_Height / 2);
	Radius[Enemy04] = D3DXVec2Length(&temp) * RadiusZoomRate[Enemy04];
	HitRadius[Enemy04] = Radius[Enemy04] * HRZoomRate[Enemy04];
	BaseAngle[Enemy04] = atan2f(Texture_Enemy04_Height, Texture_Enemy04_Width);

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_Enemy00, &EnemyTexture[Enemy00], "Enemy") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Enemy01, &EnemyTexture[Enemy01], "Enemy") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Enemy02, &EnemyTexture[Enemy02], "Enemy") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Enemy03, &EnemyTexture[Enemy03], "Enemy") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Enemy04, &EnemyTexture[Enemy04], "Enemy") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int i = 0; i < EnemyType_Max; i++)
	{
		SafeRelease(EnemyTexture[i]);
	}

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	int Enemy_No = 0;
	int GameCount = GetGameCount();
	int GameDifficulty = GetGameDiffuculty();
	int Status_No = 0;
	ENEMYSTATUS *EnemyStatus = GetEnemyStatus(Status_No);
	BOSS *Boss = GetBoss();

	if (Boss->Exist == false)
	{
		// エネミー設置
		for (Status_No = 0; Status_No < StatusType_Max; Status_No++, EnemyStatus++)
		{
			if (GameCount == EnemyStatus->AppearCount)
			{
				switch (GameDifficulty)
				{
				case Easy:
					if (EnemyStatus->EnemyDifficulty == Easy)
					{
						SetEnemy(Status_No);
					}
					break;
				case Normal:
					if (EnemyStatus->EnemyDifficulty != Hard)
					{
						SetEnemy(Status_No);
					}
					break;
				case Hard:
					SetEnemy(Status_No);
					break;
				default:
					break;
				}
			}
		}

		// 存在していたエネミーの計算
		for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++)
		{
			if (Enemy[Enemy_No].Exist == true)
			{
				// テクスチャアニメパターン計算
				if ((Enemy[Enemy_No].Count % AnimChangeCount) == 0)
				{
					switch (Enemy[Enemy_No].EnemyType)
					{
					case Enemy00:
						Enemy[Enemy_No].AnimPattern = (Enemy[Enemy_No].AnimPattern + 1) % Texture_Enemy00_Divide_X;
						break;
					case Enemy01:
						Enemy[Enemy_No].AnimPattern = (Enemy[Enemy_No].AnimPattern + 1) % Texture_Enemy01_Divide_X;
						break;
					case Enemy02:
						Enemy[Enemy_No].AnimPattern = (Enemy[Enemy_No].AnimPattern + 1) % Texture_Enemy02_Divide_X;
						break;
					case Enemy03:
						Enemy[Enemy_No].AnimPattern = (Enemy[Enemy_No].AnimPattern + 1) % Texture_Enemy03_Divide_X;
						break;
					case Enemy04:
						Enemy[Enemy_No].AnimPattern = (Enemy[Enemy_No].AnimPattern + 1) % Texture_Enemy04_Divide_X;
						break;
					default:
						break;
					}
				}

				// エネミー移動計算
				EnemyMove(Enemy_No, Enemy[Enemy_No].MovePattern);

				Enemy[Enemy_No].Pos.x += cosf(Enemy[Enemy_No].MoveAngle) * Enemy[Enemy_No].Speed;
				Enemy[Enemy_No].Pos.y += sinf(Enemy[Enemy_No].MoveAngle) * Enemy[Enemy_No].Speed;

				//敵が画面から外れたら消す
				if (CheckRange(OutOfRange, Enemy[Enemy_No].Pos, Enemy[Enemy_No].Width, Enemy[Enemy_No].Height) == true)
				{
					Enemy[Enemy_No].Exist = false;
					continue;
				}

				// 弾幕設置
				if (Enemy[Enemy_No].Count == Enemy[Enemy_No].ShotTime)
				{
					SetBarrage(Enemy_No);
				}

				Enemy[Enemy_No].Count++;

				// テクスチャ更新
				SetEnemyTexture(Enemy_No);
				SetEnemyColor(Enemy_No, White);
				SetEnemyVertex(Enemy_No);
			}
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	int Enemy_No = 0;
	LPDIRECT3DDEVICE9 Device = GetDevice();
	BOSS *Boss = GetBoss();

	if (Boss->Exist == false)
	{
		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++)
		{
			if (Enemy[Enemy_No].Exist == true)
			{
				// テクスチャの設定
				switch (Enemy[Enemy_No].EnemyType)
				{
				case Enemy00:
					Device->SetTexture(0, EnemyTexture[Enemy00]);
					break;
				case Enemy01:
					Device->SetTexture(0, EnemyTexture[Enemy01]);
					break;
				case Enemy02:
					Device->SetTexture(0, EnemyTexture[Enemy02]);
					break;
				case Enemy03:
					Device->SetTexture(0, EnemyTexture[Enemy03]);
					break;
				case Enemy04:
					Device->SetTexture(0, EnemyTexture[Enemy04]);
					break;
				default:
					break;
				}

				// ポリゴンの描画
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Enemy[Enemy_No].VertexWk, sizeof(VERTEX_2D));
			}
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeEnemyVertex(int Enemy_No)
{
	// 頂点座標の設定	
	SetEnemyVertex(Enemy_No);

	// rhwの設定
	Enemy[Enemy_No].VertexWk[0].rhw = 1.0f;
	Enemy[Enemy_No].VertexWk[1].rhw = 1.0f;
	Enemy[Enemy_No].VertexWk[2].rhw = 1.0f;
	Enemy[Enemy_No].VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	SetEnemyColor(Enemy_No, White);

	// テクスチャ座標の設定
	SetEnemyTexture(Enemy_No);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetEnemyTexture(int Enemy_No)
{
	int x = Enemy[Enemy_No].AnimPattern;
	int y = Enemy[Enemy_No].Direction;
	float sizeX = 0.0f;
	float sizeY = 0.0f;

	switch (Enemy[Enemy_No].EnemyType)
	{
	case Enemy00:
		sizeX = 1.0f / Texture_Enemy00_Divide_X;
		sizeY = 1.0f / Texture_Enemy00_Divide_Y;
		break;
	case Enemy01:
		sizeX = 1.0f / Texture_Enemy01_Divide_X;
		sizeY = 1.0f / Texture_Enemy01_Divide_Y;
		break;
	case Enemy02:
		sizeX = 1.0f / Texture_Enemy02_Divide_X;
		sizeY = 1.0f / Texture_Enemy02_Divide_Y;
		break;
	case Enemy03:
		sizeX = 1.0f / Texture_Enemy03_Divide_X;
		sizeY = 1.0f / Texture_Enemy03_Divide_Y;
		break;
	case Enemy04:
		sizeX = 1.0f / Texture_Enemy04_Divide_X;
		sizeY = 1.0f / Texture_Enemy04_Divide_Y;
		break;
	default:
		break;
	}

	Enemy[Enemy_No].VertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	Enemy[Enemy_No].VertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	Enemy[Enemy_No].VertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	Enemy[Enemy_No].VertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);


	return;
}

//=============================================================================
// 反射光の設定
//=============================================================================
void SetEnemyColor(int Enemy_No, int Color)
{
	switch (Color)
	{
	case White:
		Enemy[Enemy_No].VertexWk[0].diffuse = WHITE(255);
		Enemy[Enemy_No].VertexWk[1].diffuse = WHITE(255);
		Enemy[Enemy_No].VertexWk[2].diffuse = WHITE(255);
		Enemy[Enemy_No].VertexWk[3].diffuse = WHITE(255);
		break;
	case Red:
		Enemy[Enemy_No].VertexWk[0].diffuse = RED(255);
		Enemy[Enemy_No].VertexWk[1].diffuse = RED(255);
		Enemy[Enemy_No].VertexWk[2].diffuse = RED(255);
		Enemy[Enemy_No].VertexWk[3].diffuse = RED(255);
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetEnemyVertex(int Enemy_No)
{
	Enemy[Enemy_No].VertexWk[0].vtx.x =
		Enemy[Enemy_No].Pos.x - cosf(Enemy[Enemy_No].BaseAngle + Enemy[Enemy_No].Rot.z) * Enemy[Enemy_No].Radius;
	Enemy[Enemy_No].VertexWk[0].vtx.y =
		Enemy[Enemy_No].Pos.y - sinf(Enemy[Enemy_No].BaseAngle + Enemy[Enemy_No].Rot.z) * Enemy[Enemy_No].Radius;
	Enemy[Enemy_No].VertexWk[1].vtx.x =
		Enemy[Enemy_No].Pos.x + cosf(Enemy[Enemy_No].BaseAngle - Enemy[Enemy_No].Rot.z) * Enemy[Enemy_No].Radius;
	Enemy[Enemy_No].VertexWk[1].vtx.y =
		Enemy[Enemy_No].Pos.y - sinf(Enemy[Enemy_No].BaseAngle - Enemy[Enemy_No].Rot.z) * Enemy[Enemy_No].Radius;
	Enemy[Enemy_No].VertexWk[2].vtx.x =
		Enemy[Enemy_No].Pos.x - cosf(Enemy[Enemy_No].BaseAngle - Enemy[Enemy_No].Rot.z) * Enemy[Enemy_No].Radius;
	Enemy[Enemy_No].VertexWk[2].vtx.y =
		Enemy[Enemy_No].Pos.y + sinf(Enemy[Enemy_No].BaseAngle - Enemy[Enemy_No].Rot.z) * Enemy[Enemy_No].Radius;
	Enemy[Enemy_No].VertexWk[3].vtx.x =
		Enemy[Enemy_No].Pos.x + cosf(Enemy[Enemy_No].BaseAngle + Enemy[Enemy_No].Rot.z) * Enemy[Enemy_No].Radius;
	Enemy[Enemy_No].VertexWk[3].vtx.y =
		Enemy[Enemy_No].Pos.y + sinf(Enemy[Enemy_No].BaseAngle + Enemy[Enemy_No].Rot.z) * Enemy[Enemy_No].Radius;
}

//=============================================================================
// エネミーの情報を取得する
//=============================================================================
ENEMY *GetEnemy(int Enemy_No)
{
	return &Enemy[Enemy_No];
}

//=============================================================================
// エネミーの設置
//=============================================================================
void SetEnemy(int Status_No)
{
	int Enemy_No = 0;
	ENEMYSTATUS *EnemyStatus = GetEnemyStatus(Status_No);

	for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++)
	{
		if (Enemy[Enemy_No].Exist == false)
		{
			Enemy[Enemy_No].Exist = true;
			Enemy[Enemy_No].Pos.x = EnemyStatus->Pos.x;
			Enemy[Enemy_No].Pos.y = EnemyStatus->Pos.y;
			Enemy[Enemy_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Enemy[Enemy_No].MoveAngle = 0.0f;
			Enemy[Enemy_No].Speed = EnemyStatus->Speed;
			Enemy[Enemy_No].BulletSpeed = EnemyStatus->BulletSpeed;
			Enemy[Enemy_No].Count = 0;
			Enemy[Enemy_No].State = 0;
			Enemy[Enemy_No].MovePattern = EnemyStatus->MovePattern;
			Enemy[Enemy_No].Direction = Left;
			Enemy[Enemy_No].EnemyType = EnemyStatus->EnemyType;
			Enemy[Enemy_No].ShotTime = EnemyStatus->ShotTime;
			Enemy[Enemy_No].BarrageType = EnemyStatus->BarrageType;
			Enemy[Enemy_No].BulletType = EnemyStatus->BulletType;
			Enemy[Enemy_No].WaitCount = EnemyStatus->WaitCount;
			Enemy[Enemy_No].ExitCount = EnemyStatus->ExitCount;
			//Enemy[Enemy_No].HP_Max = EnemyStatus->HP_Max;
			Enemy[Enemy_No].HP = EnemyStatus->HP_Max;
			// 半径、当たり範囲、角度の設置
			switch (Enemy[Enemy_No].EnemyType)
			{
			case Enemy00:
				Enemy[Enemy_No].Radius = Radius[Enemy00];
				Enemy[Enemy_No].HitRadius = HitRadius[Enemy00];
				Enemy[Enemy_No].BaseAngle = BaseAngle[Enemy00];
				Enemy[Enemy_No].Width = Texture_Enemy00_Width;
				Enemy[Enemy_No].Height = Texture_Enemy00_Height;
				break;
			case Enemy01:
				Enemy[Enemy_No].Radius = Radius[Enemy01];
				Enemy[Enemy_No].HitRadius = HitRadius[Enemy01];
				Enemy[Enemy_No].BaseAngle = BaseAngle[Enemy01];
				Enemy[Enemy_No].Width = Texture_Enemy01_Width;
				Enemy[Enemy_No].Height = Texture_Enemy01_Height;
				break;
			case Enemy02:
				Enemy[Enemy_No].Radius = Radius[Enemy02];
				Enemy[Enemy_No].HitRadius = HitRadius[Enemy02];
				Enemy[Enemy_No].BaseAngle = BaseAngle[Enemy02];
				Enemy[Enemy_No].Width = Texture_Enemy02_Width;
				Enemy[Enemy_No].Height = Texture_Enemy02_Height;
				break;
			case Enemy03:
				Enemy[Enemy_No].Radius = Radius[Enemy03];
				Enemy[Enemy_No].HitRadius = HitRadius[Enemy03];
				Enemy[Enemy_No].BaseAngle = BaseAngle[Enemy03];
				Enemy[Enemy_No].Width = Texture_Enemy03_Width;
				Enemy[Enemy_No].Height = Texture_Enemy03_Height;
				break;
			case Enemy04:
				Enemy[Enemy_No].Radius = Radius[Enemy04];
				Enemy[Enemy_No].HitRadius = HitRadius[Enemy04];
				Enemy[Enemy_No].BaseAngle = BaseAngle[Enemy04];
				Enemy[Enemy_No].Width = Texture_Enemy04_Width;
				Enemy[Enemy_No].Height = Texture_Enemy04_Height;
				break;
			default:
				break;
			}
			// 落とすアイテムの設置
			for (int i = 0; i < EnemyItem_Max; i++)
			{
				Enemy[Enemy_No].Item[i] = EnemyStatus->Item[i];
			}

#ifdef _DEBUG
			SetEnemyHC(Enemy[Enemy_No].Pos, Enemy[Enemy_No].HitRadius, Enemy_No, EnemyHC);
#endif
			break;
		}
	}


	return;
}

//=============================================================================
// エネミーの移動パターン
//=============================================================================
void EnemyMove(int Enemy_No, int Pattern_No)
{
	int Count = Enemy[Enemy_No].Count;
	// 目的角度までにかかる時間
	int SpendTime = 0;
	// 角度変化が始まるからの時間
	int ChangeTime = 0;
	// 今エネミーの移動角度
	float MoveAngle = 0.0f;


	//       MoveAngle移動角度
	//           270(-90)
	//			    ↑
	// 180(-180) <--+--> 0(360)
	//			    ↓
	//			 90(-270)
	switch (Pattern_No)
	{
	case 0:
		// 開始
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(180);
			Enemy[Enemy_No].Direction = Left;
			//Enemy[Enemy_No].Speed = 3.0f;
		}
		// 登録された時間だけ停滞して
		else if (Count == Enemy[Enemy_No].ExitCount - Enemy[Enemy_No].WaitCount)
		{
			// 止まる
			Enemy[Enemy_No].Speed = 0.0f;
		}
		// 退場時間、退場が始まる
		else if (Count == Enemy[Enemy_No].ExitCount)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(0);
			Enemy[Enemy_No].Direction = Right;
			Enemy[Enemy_No].Speed = 3.0f;
		}
		break;
	case 1:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(180);
			Enemy[Enemy_No].Direction = Left;
			//Enemy[Enemy_No].Speed = 3.0f;
		}
		else if (Count == Enemy[Enemy_No].ExitCount - Enemy[Enemy_No].WaitCount)
		{
			Enemy[Enemy_No].Speed = 0.0f;
		}
		else if (Count == Enemy[Enemy_No].ExitCount)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(-45);
			Enemy[Enemy_No].Rot.z = DegreeToRadian(-45);
			Enemy[Enemy_No].Speed = 3.0f;
			Enemy[Enemy_No].Direction = Right;
		}
		break;
	case 2:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(180);
			Enemy[Enemy_No].Direction = Left;
			//Enemy[Enemy_No].Speed = 3.0f;
		}
		else if (Count == Enemy[Enemy_No].ExitCount - Enemy[Enemy_No].WaitCount)
		{
			Enemy[Enemy_No].Speed = 0.0f;
		}
		else if (Count == Enemy[Enemy_No].ExitCount)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(45);
			Enemy[Enemy_No].Rot.z = DegreeToRadian(45);
			Enemy[Enemy_No].Speed = 3.0f;
			Enemy[Enemy_No].Direction = Right;
		}
		break;
	case 3:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(180);
			Enemy[Enemy_No].Direction = Left;
			//Enemy[Enemy_No].Speed = 3.0f;
		}
		else if (Count >= Enemy[Enemy_No].ExitCount)
		{
			// 180度 -> 270度
			SpendTime = 90;
			ChangeTime = Count - Enemy[Enemy_No].ExitCount;
			MoveAngle = ChangeTime < SpendTime ? 180.0f + ChangeTime * 90.0f / SpendTime : 270.0f;
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(MoveAngle);
			Enemy[Enemy_No].Direction = Up;
		}
		break;
	case 4:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(180);
			Enemy[Enemy_No].Direction = Left;
			//Enemy[Enemy_No].Speed = 3.0f;
		}
		else if (Count >= Enemy[Enemy_No].ExitCount)
		{
			// 180度 -> 90度
			SpendTime = 90;
			ChangeTime = Count - Enemy[Enemy_No].ExitCount;
			MoveAngle = ChangeTime < SpendTime ? 180.0f - ChangeTime * 90.0f / SpendTime : 90.0f;
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(MoveAngle);
			Enemy[Enemy_No].Direction = Down;
		}
		break;
	case 5:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(180);
			Enemy[Enemy_No].Direction = Left;
			//Enemy[Enemy_No].Speed = 3.0f;
		}
		else if (Count >= 120 && Count < 180)
		{
			// 180 -> 0
			SpendTime = 60;
			ChangeTime = Count - 120;
			MoveAngle = ChangeTime < SpendTime ? 180.0f - ChangeTime * 180.0f / SpendTime : 0.0f;
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(MoveAngle);
			if (Count >= 150)
			{
				Enemy[Enemy_No].Direction = Right;
			}
		}
		else if (Count >= 180 && Count < 240)
		{
			// 0 -> -90
			SpendTime = 60;
			ChangeTime = Count - 180;
			MoveAngle = ChangeTime < SpendTime ? 0.0f - ChangeTime * 90.0f / SpendTime : -90.0f;
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(MoveAngle);
		}
		else if (Count == 240)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(-90);
			Enemy[Enemy_No].Direction = Up;
		}
		break;
	case 6:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(180);
			Enemy[Enemy_No].Direction = Left;
			//Enemy[Enemy_No].Speed = 3.0f;
		}
		else if (Count >= 120 && Count < 180)
		{
			// 180 -> 360
			SpendTime = 60;
			ChangeTime = Count - 120;
			MoveAngle = ChangeTime < SpendTime ? 180.0f + ChangeTime * 180.0f / SpendTime : 0.0f;
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(MoveAngle);
			if (Count >= 150)
			{
				Enemy[Enemy_No].Direction = Right;
			}
		}
		else if (Count >= 180 && Count < 240)
		{
			// 0 -> 90
			SpendTime = 60;
			ChangeTime = Count - 180;
			MoveAngle = ChangeTime < SpendTime ? 0.0f + ChangeTime * 90.0f / SpendTime : 90.0f;
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(MoveAngle);
		}
		else if (Count == 240)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(90);
			Enemy[Enemy_No].Direction = Down;
		}
		break;
	case 7:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(180);
			Enemy[Enemy_No].Direction = Left;
			//Enemy[Enemy_No].Speed = 3.0f;
		}
		else if (Count >= Enemy[Enemy_No].ExitCount)
		{
			// 180 -> 0
			SpendTime = 60;
			ChangeTime = Count - Enemy[Enemy_No].ExitCount;
			MoveAngle = ChangeTime < SpendTime ? 180.0f - ChangeTime * 180.0f / SpendTime : 0.0f;
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(MoveAngle);
			if (Count >= Enemy[Enemy_No].ExitCount + SpendTime / 2)
			{
				Enemy[Enemy_No].Direction = Right;
			}
		}
		break;
	case 8:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(180);
			Enemy[Enemy_No].Direction = Left;
			//Enemy[Enemy_No].Speed = 3.0f;
		}
		else if (Count >= Enemy[Enemy_No].ExitCount)
		{
			// 180 -> 360
			SpendTime = 60;
			ChangeTime = Count - Enemy[Enemy_No].ExitCount;
			MoveAngle = ChangeTime < SpendTime ? 180.0f + ChangeTime * 180.0f / SpendTime : 0.0f;
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(MoveAngle);
			if (Count >= Enemy[Enemy_No].ExitCount + SpendTime / 2)
			{
				Enemy[Enemy_No].Direction = Right;
			}
		}
		break;
	case 9:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(90);
			Enemy[Enemy_No].Direction = Down;
			//Enemy[Enemy_No].Speed = 3.0f;
			Enemy[Enemy_No].State = Debut;
		}
		if (Enemy[Enemy_No].State == Debut && Enemy[Enemy_No].Pos.y >= Screen_Height * 0.6f)
		{
			Enemy[Enemy_No].State = FirstRotate;
		}
		else if (Enemy[Enemy_No].State == FirstRotate)
		{
			// 90 -> 270
			SpendTime = 60;
			Enemy[Enemy_No].MoveAngle += DegreeToRadian(180) / SpendTime;
			Enemy[Enemy_No].Direction = Left;
			if (Enemy[Enemy_No].MoveAngle >= DegreeToRadian(270))
			{
				Enemy[Enemy_No].MoveAngle = DegreeToRadian(270);
				Enemy[Enemy_No].Direction = Up;
				Enemy[Enemy_No].State = FirstRotEnd;
			}
		}
		else if (Enemy[Enemy_No].State == FirstRotEnd && Enemy[Enemy_No].Pos.y <= Screen_Height * 0.4f)
		{
			Enemy[Enemy_No].State = SecondRotate;
		}
		else if (Enemy[Enemy_No].State == SecondRotate)
		{
			// 270 -> 450(90)
			SpendTime = 60;
			Enemy[Enemy_No].MoveAngle += DegreeToRadian(180) / SpendTime;
			Enemy[Enemy_No].Direction = Right;
			if (Enemy[Enemy_No].MoveAngle >= DegreeToRadian(450))
			{
				Enemy[Enemy_No].MoveAngle = DegreeToRadian(450);
				Enemy[Enemy_No].Direction = Down;
				Enemy[Enemy_No].State = EnemyExit;
			}
		}
		break;
	case 10:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(270);
			Enemy[Enemy_No].Direction = Up;
			//Enemy[Enemy_No].Speed = 3.0f;
			Enemy[Enemy_No].State = Debut;
		}
		if (Enemy[Enemy_No].State == Debut && Enemy[Enemy_No].Pos.y <= Screen_Height * 0.4f)
		{
			Enemy[Enemy_No].State = FirstRotate;
		}
		else if (Enemy[Enemy_No].State == FirstRotate)
		{
			// 270 -> 90
			SpendTime = 60;
			Enemy[Enemy_No].MoveAngle -= DegreeToRadian(180) / SpendTime;
			Enemy[Enemy_No].Direction = Left;
			if (Enemy[Enemy_No].MoveAngle <= DegreeToRadian(90))
			{
				Enemy[Enemy_No].MoveAngle = DegreeToRadian(90);
				Enemy[Enemy_No].Direction = Down;
				Enemy[Enemy_No].State = FirstRotEnd;
			}
		}
		else if (Enemy[Enemy_No].State == FirstRotEnd && Enemy[Enemy_No].Pos.y >= Screen_Height * 0.6f)
		{
			Enemy[Enemy_No].State = SecondRotate;
		}
		else if (Enemy[Enemy_No].State == SecondRotate)
		{
			// 90 -> -90
			SpendTime = 60;
			Enemy[Enemy_No].MoveAngle -= DegreeToRadian(180) / SpendTime;
			Enemy[Enemy_No].Direction = Right;
			if (Enemy[Enemy_No].MoveAngle <= DegreeToRadian(-90))
			{
				Enemy[Enemy_No].MoveAngle = DegreeToRadian(-90);
				Enemy[Enemy_No].Direction = Up;
				Enemy[Enemy_No].State = EnemyExit;
			}
		}
		break;
	case 11:
		if (Count == 0)
		{
			if (Enemy[Enemy_No].Pos.x <= 0)
			{
				// 左 -> 右
				Enemy[Enemy_No].MoveAngle = DegreeToRadian(0);
				Enemy[Enemy_No].Direction = Right;
			}
			else if (Enemy[Enemy_No].Pos.x >= Screen_Width)
			{
				// 右 -> 左
				Enemy[Enemy_No].MoveAngle = DegreeToRadian(180);
				Enemy[Enemy_No].Direction = Left;
			}
			else
			{
				Enemy[Enemy_No].MoveAngle = DegreeToRadian(0);
				Enemy[Enemy_No].Direction = Right;
			}
		}
		break;
	case 12:
		if (Count == 0)
		{
			if (Enemy[Enemy_No].Pos.y <= 0)
			{
				// 上 -> 下
				Enemy[Enemy_No].MoveAngle = DegreeToRadian(90);
				Enemy[Enemy_No].Direction = Down;
			}
			else if (Enemy[Enemy_No].Pos.y >= Screen_Height)
			{
				// 下 -> 上
				Enemy[Enemy_No].MoveAngle = DegreeToRadian(270);
				Enemy[Enemy_No].Direction = Up;
			}
			else
			{
				Enemy[Enemy_No].MoveAngle = DegreeToRadian(90);
				Enemy[Enemy_No].Direction = Down;
			}
		}
		break;
	case 13:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(0);
			Enemy[Enemy_No].Direction = Right;
		}
		else if (Count > Enemy[Enemy_No].ExitCount)
		{
			// 0度 -> -90度
			SpendTime = 90;
			ChangeTime = Count - Enemy[Enemy_No].ExitCount;
			MoveAngle = ChangeTime < SpendTime ? 0.0f - ChangeTime * 90.0f / SpendTime : -90.0f;
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(MoveAngle);
			Enemy[Enemy_No].Direction = Up;
		}
		break;
	case 14:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(0);
			Enemy[Enemy_No].Direction = Right;
		}
		else if (Count > Enemy[Enemy_No].ExitCount)
		{
			// 0度 -> 90度
			SpendTime = 90;
			ChangeTime = Count - Enemy[Enemy_No].ExitCount;
			MoveAngle = ChangeTime < SpendTime ? 0.0f + ChangeTime * 90.0f / SpendTime : 90.0f;
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(MoveAngle);
			Enemy[Enemy_No].Direction = Down;
		}
		break;
		// チュートリアル用バターン
	case 98:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(180);
			Enemy[Enemy_No].Direction = Left;
		}
		if (Enemy[Enemy_No].Pos.x <= 800.0f)
		{
			Enemy[Enemy_No].Speed = 0.0f;
			Enemy[Enemy_No].Pos.x = 800.0f;
		}
		break;
		// テスト用バターン
	case 99:
		if (Count == 0)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(90);
			Enemy[Enemy_No].Direction = Down;
		}
		if (Enemy[Enemy_No].Pos.y <= 150)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(90);
			Enemy[Enemy_No].Direction = Down;
		}
		else if (Enemy[Enemy_No].Pos.y >= 700)
		{
			Enemy[Enemy_No].MoveAngle = DegreeToRadian(-90);
			Enemy[Enemy_No].Direction = Up;
		}
		Enemy[Enemy_No].Speed = 3.0f;
	default:
		break;
	}

	return;
}
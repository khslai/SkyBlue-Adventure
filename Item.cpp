//=============================================================================
//
// アイテム処理 [Item.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Item.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// アイテムが自動的に吸収されるボーダーラインの座標
#define AttractBorderline (Screen_Width * 0.65)
// プレイヤーがアイテムを吸収する半径
#define PlayerAttractRadius (80)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeItemVertex(int Item_No);
// 頂点座標の設定
void SetItemVertex(int Item_No);
// テクスチャ座標の設定
void SetItemTexture(int Item_No);
// 色、透明度の設定
void SetItemDiffuse(int Item_No, int Color);
// アイテムの回転、移動、吸収の計算
void CalculateItem(int Item_No);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
static LPDIRECT3DTEXTURE9 HPTexture = NULL;
static LPDIRECT3DTEXTURE9 ScoreTexture = NULL;
static LPDIRECT3DTEXTURE9 PowerTexture = NULL;
static LPDIRECT3DTEXTURE9 BackGroundTexture = NULL;
// テクスチャ半径
static float ItemRadius = 0.0f;
// 中心点と四頂点の成す角
static float ItemBaseAngle = 0.0f;
// アイテム構造体
ITEM Item[ItemNum_Max];


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitItem(int InitState)
{
	int Item_No = 0;
	D3DXVECTOR2 temp = D3DXVECTOR2(0.0f, 0.0f);

	for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
	{
		Item[Item_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Item[Item_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Item[Item_No].Radius = 0.0f;
		Item[Item_No].BaseAngle = 0.0f;
		Item[Item_No].BaseAngle = 0.0f;
		Item[Item_No].Color = 0;
		Item[Item_No].Type = 0;
		Item[Item_No].Count = 0;
		Item[Item_No].Use = false;
		Item[Item_No].InAttract = false;

		// 頂点情報の作成
		MakeItemVertex(Item_No);
	}

	temp = D3DXVECTOR2(Texture_HPItem_Width / 2, Texture_HPItem_Height / 2);
	ItemRadius = D3DXVec2Length(&temp) * 1.5f;
	ItemBaseAngle = atan2f(Texture_HPItem_Height, Texture_HPItem_Width);

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_HPItem, &HPTexture, "Item") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_ScoreItem, &ScoreTexture, "Item") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_PowerItem, &PowerTexture, "Item") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_ItemBackGround, &BackGroundTexture, "Item") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	SafeRelease(HPTexture);
	SafeRelease(ScoreTexture);
	SafeRelease(PowerTexture);
	SafeRelease(BackGroundTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{
	int Item_No = 0;

	for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
	{
		if (Item[Item_No].Use == true)
		{
			CalculateItem(Item_No);
			if (Item[Item_No].Type == ItemBackGround)
			{
				SetItemDiffuse(Item_No, Item[Item_No].Color);
			}
			SetItemTexture(Item_No);
			SetItemVertex(Item_No);
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	int Item_No = 0;
	LPDIRECT3DDEVICE9 Device = GetDevice();

	Device->SetFVF(FVF_VERTEX_2D);

	for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
	{
		if (Item[Item_No].Use == true && Item[Item_No].Count != 0)
		{
			switch (Item[Item_No].Type)
			{
			case HPItem:
				Device->SetTexture(0, HPTexture);
				break;
			case ScoreItem:
				Device->SetTexture(0, ScoreTexture);
				break;
			case PowerItem:
				Device->SetTexture(0, PowerTexture);
				break;
			case ItemBackGround:
				Device->SetTexture(0, BackGroundTexture);
				break;
			default:
				break;
			}
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Item[Item_No].VertexWk, sizeof(VERTEX_2D));
		}
	}

	return;
}

//=============================================================================
// アイテムの回転、移動、吸収の計算
//=============================================================================
void CalculateItem(int Item_No)
{
	PLAYER *Player = GetPlayer(0);
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Distance = 0.0f;

	// プレイヤーとの距離計算
	temp = Item[Item_No].Pos - Player->Pos;
	Distance = D3DXVec3LengthSq(&temp);

	// 加速
	if (Item[Item_No].Speed < 2.5f)
	{
		Item[Item_No].Speed += 0.1f;
	}
	// 速度が速すぎる、減速
	else if (Item[Item_No].Speed > 2.5f && Item[Item_No].InAttract == false)
	{
		Item[Item_No].Speed = 2.5f;
	}
	// アイテム背景なら、回転させる
	if (Item[Item_No].Type == ItemBackGround)
	{
		Item[Item_No].Rot.z += 0.05f * ((Item_No % 3) + 1);
	}
	// 1. プレイヤーは吸収ラインを超えたっら
	// 2. 吸収範囲内
	// 3. アイテムは移動不可能範囲内
	// 吸収状態になる
	if (Player->Pos.x >= AttractBorderline ||
		Distance < (PlayerAttractRadius * PlayerAttractRadius) ||
		Item[Item_No].Pos.y <= MoveRange_Y)
	{
		Item[Item_No].InAttract = true;
	}
	// 吸収状態なら
	if (Item[Item_No].InAttract == true)
	{
		// プレイヤーに向かって移動する
		Item[Item_No].MoveAngle = AngleCalculate(Item[Item_No].Pos, Player->Pos);
		Item[Item_No].Speed += 1.0f;
	}

	// 移動計算
	Item[Item_No].Pos.x += cosf(Item[Item_No].MoveAngle) * Item[Item_No].Speed;
	Item[Item_No].Pos.y += sinf(Item[Item_No].MoveAngle) * Item[Item_No].Speed;

	// 範囲チェック
	if ((Item[Item_No].Pos.x + Texture_HPItem_Width / 2) <= 0)
	{
		Item[Item_No].Use = false;
	}

	Item[Item_No].Count++;

	return;
}

//=============================================================================
// アイテムの情報を取得する
//=============================================================================
ITEM *GetItem(int Item_No)
{
	return &Item[Item_No];
}

//=============================================================================
// プレイヤーが撃たれた時、ドロップアイテムの設置
//=============================================================================
void SetPlayerItem(D3DXVECTOR3 Pos)
{
	int i = 0, j = 0;
	int Item_No = 0;
	D3DXVECTOR3 PosTemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (i = 0; i < PlayerItem_Max; i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
			{
				if (Item[Item_No].Use == false)
				{
					if (j == 0)
					{
						Item[Item_No].Pos.x = Pos.x + rand() % (50 + 1) + PlayerAttractRadius;
						Item[Item_No].Pos.y = Pos.y + rand() % (100 + 1) - 50;
						PosTemp = Item[Item_No].Pos;
						// 指定したアイテムを出現させる
						Item[Item_No].Type = PowerItem;
					}
					else
					{
						Item[Item_No].Pos = PosTemp;
						// アイテム背景を出現させる
						Item[Item_No].Type = ItemBackGround;
						Item[Item_No].Color = PowerItem;
					}
					Item[Item_No].Use = true;
					Item[Item_No].Radius = ItemRadius;
					Item[Item_No].BaseAngle = ItemBaseAngle;
					Item[Item_No].MoveAngle = DegreeToRadian(180);
					Item[Item_No].Rot.z = 0.0f;
					Item[Item_No].Speed = -4.0f;
					Item[Item_No].Count = 0;
					Item[Item_No].InAttract = false;
					break;
				}
			}
		}
	}

	return;
}

//=============================================================================
// エネミー死亡する時、ドロップアイテムの設置
//=============================================================================
void SetEnemyItem(D3DXVECTOR3 Pos, int ItemType[], int ItemNum)
{
	int i = 0, j = 0;
	int Item_No = 0;
	D3DXVECTOR3 PosTemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (i = 0; i < ItemNum; i++)
	{
		if (ItemType[i] != Nothing)
		{
			for (j = 0; j < 2; j++)
			{
				for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
				{
					if (Item[Item_No].Use == false)
					{
						if (j == 0)
						{
							Item[Item_No].Pos = Pos;
							// 複数なら適当にちらばらせる
							if (i > 0)
							{
								Item[Item_No].Pos.x += rand() % (100 + 1) - 50;
								Item[Item_No].Pos.y += rand() % (100 + 1) - 50;
							}
							PosTemp = Item[Item_No].Pos;
							// 指定したアイテムを出現させる
							Item[Item_No].Type = ItemType[i];
						}
						else
						{
							Item[Item_No].Pos = PosTemp;
							// アイテム背景を出現させる
							Item[Item_No].Type = ItemBackGround;
							Item[Item_No].Color = ItemType[i];
						}
						Item[Item_No].Use = true;
						Item[Item_No].Radius = ItemRadius;
						Item[Item_No].BaseAngle = ItemBaseAngle;
						Item[Item_No].MoveAngle = DegreeToRadian(180);
						Item[Item_No].Rot.z = 0.0f;
						Item[Item_No].Speed = -4.0f;
						Item[Item_No].Count = 0;
						Item[Item_No].InAttract = false;
						break;
					}
				}
			}
		}
	}

	return;
}

//=============================================================================
// ボス撃破する時、ドロップアイテムの設置
//=============================================================================
void SetBossItem(D3DXVECTOR3 Pos)
{
	int i = 0, j = 0;
	int Item_No = 0;
	int TypeTemp = 0;
	D3DXVECTOR3 PosTemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (i = 0; i < BossItem_Max; i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
			{
				if (Item[Item_No].Use == false)
				{
					if (j == 0)
					{
						Item[Item_No].Pos.x = Pos.x + rand() % (100 + 1) - 50;
						Item[Item_No].Pos.y = Pos.y + rand() % (100 + 1) - 50;
						PosTemp = Item[Item_No].Pos;
						// アイテムはランダムに出現させる
						Item[Item_No].Type = rand() % (2 + 1);
						TypeTemp = Item[Item_No].Type;
					}
					else
					{
						// アイテム背景を出現させる
						Item[Item_No].Pos = PosTemp;
						Item[Item_No].Type = ItemBackGround;
						Item[Item_No].Color = TypeTemp;
					}
					Item[Item_No].Use = true;
					Item[Item_No].Radius = ItemRadius;
					Item[Item_No].BaseAngle = ItemBaseAngle;
					Item[Item_No].MoveAngle = DegreeToRadian(180);
					Item[Item_No].Rot.z = 0.0f;
					Item[Item_No].Speed = -4.0f;
					Item[Item_No].Count = 0;
					Item[Item_No].InAttract = false;
					break;
				}
			}
		}
	}

	return;
}

//=============================================================================
// ボス撃破する時、バレットがスコアアイテムになる
//=============================================================================
void SetBossBulletItem(D3DXVECTOR3 Pos)
{
	int i = 0;
	int Item_No = 0;

	for (i = 0; i < 2; i++)
	{
		for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
		{
			if (Item[Item_No].Use == false)
			{
				if (i == 0)
				{
					// 指定したアイテムを出現させる
					Item[Item_No].Type = ScoreItem;
				}
				else
				{
					// アイテム背景を出現させる
					Item[Item_No].Type = ItemBackGround;
					Item[Item_No].Color = ScoreItem;
				}
				Item[Item_No].Pos = Pos;
				Item[Item_No].Use = true;
				Item[Item_No].Radius = ItemRadius;
				Item[Item_No].BaseAngle = ItemBaseAngle;
				Item[Item_No].MoveAngle = DegreeToRadian(180);
				Item[Item_No].Rot.z = 0.0f;
				Item[Item_No].Speed = 0.0f;
				Item[Item_No].Count = 0;
				Item[Item_No].InAttract = true;
				break;
			}
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeItemVertex(int Item_No)
{
	// 頂点座標の設定	
	SetItemVertex(Item_No);

	// rhwの設定
	Item[Item_No].VertexWk[0].rhw = 1.0f;
	Item[Item_No].VertexWk[1].rhw = 1.0f;
	Item[Item_No].VertexWk[2].rhw = 1.0f;
	Item[Item_No].VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	Item[Item_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Item[Item_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Item[Item_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Item[Item_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetItemTexture(Item_No);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetItemTexture(int Item_No)
{
	Item[Item_No].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Item[Item_No].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Item[Item_No].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Item[Item_No].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}



//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetItemVertex(int Item_No)
{
	Item[Item_No].VertexWk[0].vtx.x = Item[Item_No].Pos.x - cosf(Item[Item_No].BaseAngle + Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[0].vtx.y = Item[Item_No].Pos.y - sinf(Item[Item_No].BaseAngle + Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[1].vtx.x = Item[Item_No].Pos.x + cosf(Item[Item_No].BaseAngle - Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[1].vtx.y = Item[Item_No].Pos.y - sinf(Item[Item_No].BaseAngle - Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[2].vtx.x = Item[Item_No].Pos.x - cosf(Item[Item_No].BaseAngle - Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[2].vtx.y = Item[Item_No].Pos.y + sinf(Item[Item_No].BaseAngle - Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[3].vtx.x = Item[Item_No].Pos.x + cosf(Item[Item_No].BaseAngle + Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[3].vtx.y = Item[Item_No].Pos.y + sinf(Item[Item_No].BaseAngle + Item[Item_No].Rot.z) * Item[Item_No].Radius;

	return;
}

//=============================================================================
// 色、透明度の設定
//=============================================================================
void SetItemDiffuse(int Item_No, int Color)
{
	switch (Color)
	{
	case HPItem:
		Item[Item_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		Item[Item_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		Item[Item_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		Item[Item_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		break;
	case ScoreItem:
		Item[Item_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(0, 255, 33, 255);
		Item[Item_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(0, 255, 33, 255);
		Item[Item_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(0, 255, 33, 255);
		Item[Item_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(0, 255, 33, 255);
		break;
	case PowerItem:
		Item[Item_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		Item[Item_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		Item[Item_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		Item[Item_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
//
// アイテムヘッダー [Item.h]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_HPItem					_T("data/Texture/Item/HPItem.png")
#define Texture_HPItem_Width			(35)
#define Texture_HPItem_Height			(35)
#define Texture_ScoreItem				_T("data/Texture/Item/ScoreItem.png")
#define Texture_ScoreItem_Width			(35)
#define Texture_ScoreItem_Height		(35)
#define Texture_PowerItem				_T("data/Texture/Item/PowerItem.png")
#define Texture_PowerItem_Width			(35)
#define Texture_PowerItem_Height		(35)
#define Texture_ItemBackGround			_T("data/Texture/Item/ItemBackGround.png")
#define Texture_ItemBackGround_Width	(35)
#define Texture_ItemBackGround_Height	(35)

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;					// 座標
	D3DXVECTOR3		Rot;					// テクスチャの回転角度
	float			Radius;					// 画像中心から頂点までの距離
	float			BaseAngle;				// 画像中心から頂点までの角度
	float			MoveAngle;				// 移動方向を変更する角度
	float			Speed;					// 移動スピード
	int				Color;					// 色
	int				Type;					// アイテムの種類
	int				Count;					// カウント
	bool			Use;					// 使用フラグ
	bool			InAttract;				// アイテム吸収状態のフラグ
}ITEM;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitItem(int InitState);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
ITEM *GetItem(int Item_No);
void SetPlayerItem(D3DXVECTOR3 Pos);
void SetEnemyItem(D3DXVECTOR3 Pos, int ItemType[], int ItemNum);
void SetBossItem(D3DXVECTOR3 Pos);
void SetBossBulletItem(D3DXVECTOR3 Pos);

#endif

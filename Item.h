//=============================================================================
//
// アイテムヘッダー [Item.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
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

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// アイテム構造体
typedef struct
{
	VERTEX_2D		VertexWk[Num_Vertex];	// 頂点情報格納ワーク
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
// 初期化処理
HRESULT InitItem(int InitState);
// 終了処理
void UninitItem(void);
// 更新処理
void UpdateItem(void);
// 描画処理
void DrawItem(void);
// アイテムの情報を取得する
ITEM *GetItem(int Item_No);
// プレイヤーが撃たれた時、ドロップアイテムの設置
void SetPlayerItem(D3DXVECTOR3 Pos);
// エネミー死亡する時、ドロップアイテムの設置
void SetEnemyItem(D3DXVECTOR3 Pos, int ItemType[], int ItemNum);
// ボス撃破する時、ドロップアイテムの設置
void SetBossItem(D3DXVECTOR3 Pos);
// ボス撃破する時、バレットがスコアアイテムになる
void SetBossBulletItem(D3DXVECTOR3 Pos);

#endif

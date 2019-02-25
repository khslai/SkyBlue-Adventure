//=============================================================================
//
// レーザーヘッダー [Laser.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _LASER_H_
#define _LASER_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Laser			_T("data/Texture/Bullet/Laser.png")
#define Texture_Laser_Width		(30)
#define Texture_Laser_Height	(460)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// レーザー構造体
typedef struct
{
	VERTEX_2D		VertexWk[Num_Vertex];	// 頂点情報格納ワーク
	D3DXVECTOR3		RotateCenterPos;		// 回転中心のPosを記録する
	D3DXVECTOR3		Pos;					// 中心点の座標じゃない、左側の真ん中の座標
											//  ┌──────────┐
											// →|          |
											//  └──────────┘
	D3DXVECTOR3		BeforeRotPos;			// 回転始まる前のPosを記録する
	float			RotateTime;				// 回転必要の時間
	float			BeforeRotAngle;			// 回転始まる前のMoveAngleを記録する
	float			MoveAngle;				// 移動角度
	float			RotateAngle;			// 回転したい角度
	float			Width;					// レーザーの幅
	float			Length;					// レーザーの長さ
	int				State;					// 回転の方向を記録する、
	int				Count;					// カウント
	bool			Use;					// 使用中かどうかのフラグ
	bool			InRotate;				// 今は回転かどうかのフラグ
}LASER;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitLaser(int InitState);
// 終了処理
void UninitLaser(void);
// 更新処理
void UpdateLaser(void);
// 描画処理
void DrawLaser(void);
// レーザーの設置
void SetLaser();
// レーザーの情報を取得する
LASER *GetLaser(int Laser_No);

#endif

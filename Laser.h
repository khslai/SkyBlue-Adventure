//=============================================================================
//
// レーザーヘッダー [Laser.h]
// Author：TH_GP11_GP11B341_35_頼凱興
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

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
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
HRESULT InitLaser(int InitState);
void UninitLaser(void);
void UpdateLaser(void);
void DrawLaser(void);
void SetLaser();
LASER *GetLaser(int Laser_No);

#endif

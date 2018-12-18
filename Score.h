//=============================================================================
//
// スコアヘッダー [Score.h]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "Gauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Score				_T("data/Texture/Other/ScoreNumber.png")
#define Texture_Score_Width_Total	(240)
#define Texture_Score_Divide_X		(10)
#define Texture_Score_Width			(Texture_Score_Width_Total / Texture_Score_Divide_X)
#define Texture_Score_Height		(30)
#define ScorePos_X					(PlayerHPGauge_Pos_X + 800)
#define ScorePos_Y					(5)

typedef struct
{
	VERTEX_2D				VertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				Pos;					// 座標
	int						Number;					// 表示する数字
}SCORE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(int InitState);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int Score);

#endif

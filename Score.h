//=============================================================================
//
// スコアヘッダー [Score.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
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

// スコア表示座標
#define ScorePos_X					(PlayerHPGauge_Pos_X + 800)
#define ScorePos_Y					(5)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// スコア構造体
typedef struct
{
	VERTEX_2D				VertexWk[Num_Vertex];	// 頂点情報格納ワーク
	D3DXVECTOR3				Pos;					// 座標
	int						Number;					// 表示する数字
}SCORE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitScore(int InitState);
// 終了処理
void UninitScore(void);
// 更新処理
void UpdateScore(void);
// 描画処理
void DrawScore(void);
// スコア加算
void AddScore(int Score);

#endif

//=============================================================================
//
// ゲームオーバーヘッダー [GameOver.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitGameOver(int InitState);
// 終了処理
void UninitGameOver(void);
// 更新処理
void UpdateGameOver(void);
// 描画処理
void DrawGameOver(void);

#endif

//=============================================================================
//
// ボス練習モードヘッダー [BossPratice.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _BOSSPRATICE_H_
#define _BOSSPRATICE_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitBossPratice(int InitState);
// 終了処理
void UninitBossPratice(void);
// 更新処理
void UpdateBossPratice(void);
// 描画処理
void DrawBossPratice(void);
void DrawBossPraticeMenu(void);
// ボスモードの状態を取得する
bool GetExitPratice(void);

#endif

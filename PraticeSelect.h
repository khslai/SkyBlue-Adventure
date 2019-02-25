//=============================================================================
//
// 練習弾幕選択ヘッダー [PraticeSelect.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _PRATICESELECT_H_
#define _PRATICESELECT_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_UpDownArrow			_T("data/Texture/BackGround/UpDownArrow.png")
#define Texture_UpDownArrow_Width	(40)
#define Texture_UpDownArrow_Height	(60)



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitPraticeSelect(int InitState);
// 終了処理
void UninitPraticeSelect(void);
// 更新処理
void UpdatePraticeSelect(void);
// 描画処理
void DrawPraticeSelect(void);
// 選択肢の情報を取得する
int GetPraticeSelect(void);

#endif

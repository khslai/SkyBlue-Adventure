//=============================================================================
//
// 自機選択ヘッダー [SelectChar.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _SELECTCHAR_H_
#define _SELECTCHAR_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_SelectCharBG			_T("data/Texture/BackGround/SelectCharBG.png")
#define Texture_SelectCharBG_Width		(1024)
#define Texture_SelectCharBG_Height		(768)
#define Texture_DrawBox					_T("data/Texture/BackGround/DrawBox.png")
#define Texture_DrawBox_Width			(800)
#define Texture_DrawBox_Height			(500)

// 半透明枠の描く座標
#define DrawBox_X						(112)
#define DrawBox_Y						(250)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitSelectChar(int InitState);
// 終了処理
void UninitSelectChar(void);
// 更新処理
void UpdateSelectChar(void);
// 背景描画処理
void DrawSelectCharBG(void);
// メニュー画面描画処理
void DrawSelectCharMenu(void);

#endif

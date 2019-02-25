//=============================================================================
//
// 自機選択ヘッダー [SelectChar.h]
// Author：TH_GP11_GP11B341_35_頼凱興
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
HRESULT InitSelectChar(int InitState);
void UninitSelectChar(void);
void UpdateSelectChar(void);
void DrawSelectCharBG(void);
void DrawSelectCharMenu(void);

#endif

//=============================================================================
//
// 前景ヘッダー [FrontBG.h]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#ifndef _FRONTBG_H_
#define _FRONTBG_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_FrontBG				_T("data/Texture/BackGround/FrontBG.png")
#define Texture_FrontBG_Width		(2400)
#define Texture_FrontBG_Height		(768)



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFrontBG(int InitState);
void UninitFrontBG(void);
void UpdateFrontBG(void);
void DrawFrontBG(void);

#endif

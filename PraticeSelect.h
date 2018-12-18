//=============================================================================
//
// 練習弾幕選択ヘッダー [PraticeSelect.h]
// Author：TH_GP11_GP11B341_35_頼凱興
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
HRESULT InitPraticeSelect(int InitState);
void UninitPraticeSelect(void);
void UpdatePraticeSelect(void);
void DrawPraticeSelect(void);
int GetPraticeSelect(void);

#endif

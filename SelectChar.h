//=============================================================================
//
// ©@Iπwb_[ [SelectChar.h]
// AuthorFTH_GP11_GP11B341_35_M»
//
//=============================================================================
#ifndef _SELECTCHAR_H_
#define _SELECTCHAR_H_


//*****************************************************************************
// }Nθ`
//*****************************************************************************
#define Texture_SelectCharBG			_T("data/Texture/BackGround/SelectCharBG.png")
#define Texture_SelectCharBG_Width		(1024)
#define Texture_SelectCharBG_Height		(768)
#define Texture_DrawBox					_T("data/Texture/BackGround/DrawBox.png")
#define Texture_DrawBox_Width			(800)
#define Texture_DrawBox_Height			(500)

// Ό§ΎgΜ`­ΐW
#define DrawBox_X						(112)
#define DrawBox_Y						(250)




//*****************************************************************************
// vg^CvιΎ
//*****************************************************************************
HRESULT InitSelectChar(int InitState);
void UninitSelectChar(void);
void UpdateSelectChar(void);
void DrawSelectCharBG(void);
void DrawSelectCharMenu(void);

#endif

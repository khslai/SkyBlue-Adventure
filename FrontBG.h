//=============================================================================
//
// �O�i�w�b�_�[ [FrontBG.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _FRONTBG_H_
#define _FRONTBG_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_FrontBG				_T("data/Texture/BackGround/FrontBG.png")
#define Texture_FrontBG_Width		(2400)
#define Texture_FrontBG_Height		(768)



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFrontBG(int InitState);
void UninitFrontBG(void);
void UpdateFrontBG(void);
void DrawFrontBG(void);

#endif

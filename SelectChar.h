//=============================================================================
//
// ���@�I���w�b�_�[ [SelectChar.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _SELECTCHAR_H_
#define _SELECTCHAR_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_SelectCharBG			_T("data/Texture/BackGround/SelectCharBG.png")
#define Texture_SelectCharBG_Width		(1024)
#define Texture_SelectCharBG_Height		(768)
#define Texture_DrawBox					_T("data/Texture/BackGround/DrawBox.png")
#define Texture_DrawBox_Width			(800)
#define Texture_DrawBox_Height			(500)

// �������g�̕`�����W
#define DrawBox_X						(112)
#define DrawBox_Y						(250)




//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSelectChar(int InitState);
void UninitSelectChar(void);
void UpdateSelectChar(void);
void DrawSelectCharBG(void);
void DrawSelectCharMenu(void);

#endif

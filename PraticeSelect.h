//=============================================================================
//
// ���K�e���I���w�b�_�[ [PraticeSelect.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _PRATICESELECT_H_
#define _PRATICESELECT_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_UpDownArrow			_T("data/Texture/BackGround/UpDownArrow.png")
#define Texture_UpDownArrow_Width	(40)
#define Texture_UpDownArrow_Height	(60)



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPraticeSelect(int InitState);
void UninitPraticeSelect(void);
void UpdatePraticeSelect(void);
void DrawPraticeSelect(void);
int GetPraticeSelect(void);

#endif

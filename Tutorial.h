//=============================================================================
//
// �`���[�g���A���w�b�_�[ [Tutorial.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Tutorial1_KeyBoard	_T("data/Texture/BackGround/Tutorial1_KeyBoard.png")
#define Texture_Tutorial1_GamePad	_T("data/Texture/BackGround/Tutorial1_GamePad.png")
#define Texture_Tutorial2			_T("data/Texture/BackGround/Tutorial2.png")
#define Texture_Tutorial_Width		(1024)
#define Texture_Tutorial_Height		(768)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTutorial(int InitState);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
void DrawTutorialMenu(void);
int GetTutorialPhase(void);

#endif

//=============================================================================
//
// �`���[�g���A���w�b�_�[ [Tutorial.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
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
// ����������
HRESULT InitTutorial(int InitState);
// �I������
void UninitTutorial(void);
// �X�V����
void UpdateTutorial(void);
// �`�揈��
void DrawTutorial(void);
// ���j���[��ʕ`�揈��
void DrawTutorialMenu(void);
// �`���[�g���A���̏�Ԃ��擾����
int GetTutorialPhase(void);

#endif

//=============================================================================
//
// ���@�I���w�b�_�[ [SelectChar.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
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
// ����������
HRESULT InitSelectChar(int InitState);
// �I������
void UninitSelectChar(void);
// �X�V����
void UpdateSelectChar(void);
// �w�i�`�揈��
void DrawSelectCharBG(void);
// ���j���[��ʕ`�揈��
void DrawSelectCharMenu(void);

#endif

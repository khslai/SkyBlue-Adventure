//=============================================================================
//
// ���K�e���I���w�b�_�[ [PraticeSelect.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
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
// ����������
HRESULT InitPraticeSelect(int InitState);
// �I������
void UninitPraticeSelect(void);
// �X�V����
void UpdatePraticeSelect(void);
// �`�揈��
void DrawPraticeSelect(void);
// �I�����̏����擾����
int GetPraticeSelect(void);

#endif

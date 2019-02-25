//=============================================================================
//
// ��ʑJ�ڃw�b�_�[ [Transition.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _TRANSITION_H_
#define _TRANSITION_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Transition			_T("data/Texture/Other/Transition.png")
#define Texture_Transition_Width	(1024)
#define Texture_Transition_Height	(768)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �X�e�[�W�J�ڍ\����
typedef struct
{
	VERTEX_2D	Pixel[TextureDevideNum][TextureDevideNum][Num_Vertex];	// ���_���i�[���[�N
	int			Count;													// �v�Z�J�E���g
	int			Alpha;													// �A���t�@�l
	float		DissappearRadius;										// ���S��������̔��a
	bool		FadeInOver;												// �t�F�C�h�C���I�����̃t���O
	bool		Use;													// �g�p�t���O
}TRANSITION;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitTransition(int InitState);
// �I������
void UninitTransition(void);
// �X�V����
void UpdateTransition(void);
// �`�揈��
void DrawTransition(void);
// �X�e�[�W�J�ڂ�ݒu����
void SetTransition(void);
// �X�e�[�W�J�ڂ̏����擾����
TRANSITION *GetTransition(void);

#endif

//=============================================================================
//
// ��ʑJ�ڃw�b�_�[ [Transition.h]
// Author�FTH_GP11_GP11B341_35_���M��
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

typedef struct
{
	VERTEX_2D	Pixel[TextureDevideNum][TextureDevideNum][NUM_VERTEX];	// ���_���i�[���[�N
	int			Count;													// �v�Z�J�E���g
	int			Alpha;													// �A���t�@�l
	float		DissappearRadius;										// ���S��������̔��a
	bool		FadeInOver;												// �t�F�C�h�C���I�����̃t���O
	bool		Use;													// �g�p�t���O
}TRANSITION;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTransition(int InitState);
void UninitTransition(void);
void UpdateTransition(void);
void DrawTransition(void);
void SetTransition(void);
TRANSITION *GetTransition(void);


#endif

//=============================================================================
//
// �w�i�w�b�_�[ [BackGround.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_BG_Day				_T("data/Texture/BackGround/BackGround_Day.png")
#define Texture_BG_Afternoon		_T("data/Texture/BackGround/BackGround_Afternoon.png")
#define Texture_BG_Night			_T("data/Texture/BackGround/BackGround_Night.png")
#define Texture_BG_Width			(1988)
#define Texture_BG_Height			(768)
#define Texture_GameStart			_T("data/Texture/BackGround/GameStart.png")
#define Texture_GameStart_Width		(600)
#define Texture_GameStart_Height	(100)
#define Texture_GameClear			_T("data/Texture/BackGround/GameClear.png")
#define Texture_GameClear_Width		(600)
#define Texture_GameClear_Height	(120)

#define GameClearVertexDevide		(6)		// �e�N�X�`���͘Z�̃|���S���𕪊�����


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �w�i�\����
typedef	struct
{
	VERTEX_2D		VertexWk[Num_Vertex];	// ���_���i�[���[�N
	D3DXVECTOR3		Pos;					// ���W
	bool			InShake;				// ���͗h��邩
}BACKGROUND;

// �Q�[���N���A�\����
typedef struct
{
	VERTEX_2D		Pixel[GameClearVertexDevide][Num_Vertex];		// ���_���i�[���[�N
	int				Alpha;											// �A���t�@�l
	bool			FadeInOver;										// �t�F�C�h�C���I����
	bool			AnimationOver;									// ���o�I���
}GAMECLEAR;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitBG(int InitState);
// �I������
void UninitBG(void);
// �X�V����
void UpdateBG(void);
// �`�揈��
void DrawBG(void);
// �v���C���[UI�̕`�揈��
void DrawInformation(void);
// �w�i���h��鉉�o�̐ݒu
void SetBGShake(bool State);
// �w�i�̏�Ԃ��擾����
int GetBGState(void);
// �w�i�̃t�F�C�h�A�E�g��Ԃ��擾����
bool GetBGFadeout(void);
// �Q�[���N���A�̏����擾����
GAMECLEAR *GetGameClear(void);

#endif

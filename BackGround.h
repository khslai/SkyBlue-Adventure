//=============================================================================
//
// �w�i�w�b�_�[ [BackGround.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define Texture_BG_Day				_T("data/Texture/BackGround/BackGround_Day.png")
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



typedef	struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3		Pos;					// ���W
	bool			InShake;				// ���͗h��邩
}BACKGROUND;

typedef struct
{
	VERTEX_2D		Pixel[GameClearVertexDevide][NUM_VERTEX];		// ���_���i�[���[�N
	int				Alpha;											// �A���t�@�l
	bool			FadeInOver;										// �t�F�C�h�C���I����
	bool			AnimationOver;									// ���o�I���
}GAMECLEAR;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBG(int InitState);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
void DrawInformation(void);
void SetBGShake(bool State);
int GetBGState(void);
bool GetBGFadeout(void);
GAMECLEAR *GetGameClear(void);


#endif

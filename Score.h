//=============================================================================
//
// �X�R�A�w�b�_�[ [Score.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "Gauge.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Score				_T("data/Texture/Other/ScoreNumber.png")
#define Texture_Score_Width_Total	(240)
#define Texture_Score_Divide_X		(10)
#define Texture_Score_Width			(Texture_Score_Width_Total / Texture_Score_Divide_X)
#define Texture_Score_Height		(30)
#define ScorePos_X					(PlayerHPGauge_Pos_X + 800)
#define ScorePos_Y					(5)

typedef struct
{
	VERTEX_2D				VertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				Pos;					// ���W
	int						Number;					// �\�����鐔��
}SCORE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScore(int InitState);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int Score);

#endif

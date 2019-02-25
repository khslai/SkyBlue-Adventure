//=============================================================================
//
// �X�R�A�w�b�_�[ [Score.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
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

// �X�R�A�\�����W
#define ScorePos_X					(PlayerHPGauge_Pos_X + 800)
#define ScorePos_Y					(5)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �X�R�A�\����
typedef struct
{
	VERTEX_2D				VertexWk[Num_Vertex];	// ���_���i�[���[�N
	D3DXVECTOR3				Pos;					// ���W
	int						Number;					// �\�����鐔��
}SCORE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitScore(int InitState);
// �I������
void UninitScore(void);
// �X�V����
void UpdateScore(void);
// �`�揈��
void DrawScore(void);
// �X�R�A���Z
void AddScore(int Score);

#endif

//=============================================================================
//
// �X�R�A���� [Score.cpp]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#include "main.h"
#include "Score.h"
#include "Player.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DigitMax (10)
#define ScoreMax (9999999999)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeScoreVertex(int ScoreDigit, int ScoreNum);
void SetScoreTexture(int ScoreDigit, int ScoreNum);
void SetScoreVertex(int ScoreDigit);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9 ScoreTexture = NULL;
SCORE Score[DigitMax];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitScore(int InitState)
{
	int ScoreDigit = 0;

	for (ScoreDigit = 0; ScoreDigit < DigitMax; ScoreDigit++)
	{
		Score[ScoreDigit].Pos = D3DXVECTOR3((float)ScorePos_X - ScoreDigit * Texture_Score_Width, ScorePos_Y, 0.0f);
		Score[ScoreDigit].Number = 0;
		// ���_���̍쐬
		MakeScoreVertex(ScoreDigit, Score[ScoreDigit].Number);
	}

	if (InitState == FirstInit)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Score, &ScoreTexture, "Score") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
	// �e�N�X�`���̊J��
	SafeRelease(ScoreTexture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(void)
{
	PLAYER *Player = GetPlayer(0);
	int ScoreDigit = 0;
	int Scoretemp = Player->Score;

	for (ScoreDigit = 0; ScoreDigit < DigitMax; ScoreDigit++)
	{
		Score[ScoreDigit].Number = Scoretemp % 10;
		Scoretemp /= 10;
		//Score[ScoreDigit].Number = Scoretemp / (int)pow(10, DigitMax - ScoreDigit - 1);
		//Scoretemp = Scoretemp % (int)pow(10, DigitMax - ScoreDigit - 1);
		// �e�N�X�`�����W��ݒ�
		SetScoreTexture(ScoreDigit, Score[ScoreDigit].Number);
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
	int ScoreDigit = 0;
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	for (ScoreDigit = 0; ScoreDigit < DigitMax; ScoreDigit++)
	{
		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, ScoreTexture);

		// �|���S���̕`��
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_SCORE, Score[ScoreDigit].VertexWk, sizeof(VERTEX_2D));
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeScoreVertex(int ScoreDigit, int ScoreNum)
{
	// ���_���W�̐ݒ�	
	SetScoreVertex(ScoreDigit);

	// rhw�̐ݒ�
	Score[ScoreDigit].VertexWk[0].rhw = 1.0f;
	Score[ScoreDigit].VertexWk[1].rhw = 1.0f;
	Score[ScoreDigit].VertexWk[2].rhw = 1.0f;
	Score[ScoreDigit].VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Score[ScoreDigit].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Score[ScoreDigit].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Score[ScoreDigit].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Score[ScoreDigit].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetScoreTexture(ScoreDigit, ScoreNum);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetScoreTexture(int ScoreDigit, int ScoreNum)
{
	Score[ScoreDigit].VertexWk[0].tex = D3DXVECTOR2(ScoreNum * 0.1f, 0.0f);
	Score[ScoreDigit].VertexWk[1].tex = D3DXVECTOR2((ScoreNum + 1) * 0.1f, 0.0f);
	Score[ScoreDigit].VertexWk[2].tex = D3DXVECTOR2(ScoreNum * 0.1f, 1.0f);
	Score[ScoreDigit].VertexWk[3].tex = D3DXVECTOR2((ScoreNum + 1) * 0.1f, 1.0f);

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetScoreVertex(int ScoreDigit)
{
	Score[ScoreDigit].VertexWk[0].vtx =
		D3DXVECTOR3(Score[ScoreDigit].Pos.x, Score[ScoreDigit].Pos.y, 0.0f);
	Score[ScoreDigit].VertexWk[1].vtx =
		D3DXVECTOR3(Score[ScoreDigit].Pos.x + Texture_Score_Width, Score[ScoreDigit].Pos.y, 0.0f);
	Score[ScoreDigit].VertexWk[2].vtx =
		D3DXVECTOR3(Score[ScoreDigit].Pos.x, Score[ScoreDigit].Pos.y + Texture_Score_Height, 0.0f);
	Score[ScoreDigit].VertexWk[3].vtx =
		D3DXVECTOR3(Score[ScoreDigit].Pos.x + Texture_Score_Width, Score[ScoreDigit].Pos.y + Texture_Score_Height, 0.0f);

	return;
}

//=============================================================================
// �X�R�A�̌v�Z
//=============================================================================
void AddScore(int Score)
{
	PLAYER *Player = GetPlayer(0);

	Player->Score += Score;
	Player->Score = Player->Score > ScoreMax ? (int)ScoreMax : Player->Score;

	return;
}

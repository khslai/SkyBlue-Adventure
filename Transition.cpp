//=============================================================================
//
// ��ʑJ�ڏ��� [Transition.cpp]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#include "main.h"
#include "Transition.h"
#include "Input.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TextureDevideNum (50)



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeTransitionVertex(void);
void SetTransitionTexture(void);
void SetTransitionVertex(void);



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 TransitionTexture = NULL;
TRANSITION Transition;



//=============================================================================
// ����������
//=============================================================================
HRESULT InitTransition(int InitState)
{
	Transition.Alpha = 0;
	Transition.Count = 0;
	Transition.DissappearRadius = 0.0f;
	Transition.FadeInOver = false;
	Transition.Use = false;

	MakeTransitionVertex();

	if (InitState == FirstInit)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Transition, &TransitionTexture, "Transition") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTransition(void)
{
	SafeRelease(TransitionTexture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTransition(void)
{
	int i = 0, j = 0;
	float Distance = 0.0f;
	D3DXVECTOR2 temp = D3DXVECTOR2(0.0f, 0.0f);

	if (Transition.Use == true && Transition.FadeInOver == false)
	{
		Transition.Alpha += 10;
		if (Transition.Alpha >= 255)
		{
			Transition.Alpha = 255;
			Transition.FadeInOver = true;
		}
		for (i = 0; i < TextureDevideNum; i++)
		{
			for (j = 0; j < TextureDevideNum; j++)
			{
				Transition.Pixel[i][j][0].diffuse = WHITE(Transition.Alpha);
				Transition.Pixel[i][j][1].diffuse = WHITE(Transition.Alpha);
				Transition.Pixel[i][j][2].diffuse = WHITE(Transition.Alpha);
				Transition.Pixel[i][j][3].diffuse = WHITE(Transition.Alpha);
			}
		}
	}

	if (Transition.Use == true && Transition.FadeInOver == true)
	{
		if (Transition.DissappearRadius > Screen_Width / 2 * Root2)
		{
			Transition.DissappearRadius = Screen_Width / 2 * Root2;
			Transition.Use = false;
			Transition.FadeInOver = false;
		}
		else
		{
			Transition.DissappearRadius += 10.0f;
		}
		Transition.Alpha -= 4;
		if (Transition.Alpha <= 0)
		{
			Transition.Alpha = 0;
		}
		for (i = 0; i < TextureDevideNum; i++)
		{
			for (j = 0; j < TextureDevideNum; j++)
			{
				temp = D3DXVECTOR2(Transition.Pixel[i][j][0].vtx.x, Transition.Pixel[i][j][0].vtx.y) - D3DXVECTOR2(ScreenCenter_X, ScreenCenter_Y);
				Distance = D3DXVec2Length(&temp);
				if (Distance <= Transition.DissappearRadius)
				{
					Transition.Pixel[i][j][0].diffuse = WHITE(Transition.Alpha);
					Transition.Pixel[i][j][1].diffuse = WHITE(Transition.Alpha);
					Transition.Pixel[i][j][2].diffuse = WHITE(Transition.Alpha);
					Transition.Pixel[i][j][3].diffuse = WHITE(Transition.Alpha);
				}
				else
				{
					Transition.Pixel[i][j][0].diffuse = WHITE(255);
					Transition.Pixel[i][j][1].diffuse = WHITE(255);
					Transition.Pixel[i][j][2].diffuse = WHITE(255);
					Transition.Pixel[i][j][3].diffuse = WHITE(255);
				}
			}
		}
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTransition(void)
{
	int i = 0, j = 0;
	LPDIRECT3DDEVICE9 Device = GetDevice();

	if (Transition.Use == true)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, TransitionTexture);

		// �|���S���̕`��
		for (i = 0; i < TextureDevideNum; i++)
		{
			for (j = 0; j < TextureDevideNum; j++)
			{
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Transition.Pixel[i][j], sizeof(VERTEX_2D));
			}
		}
	}

	return;
}

//=============================================================================
// �X�e�[�W�J�ډ�ʐݒu
//=============================================================================
void SetTransition(void)
{
	if (Transition.Use == false)
	{
		Transition.Count = 0;
		Transition.DissappearRadius = 0.0f;
		Transition.FadeInOver = false;
		Transition.Use = true;
	}

	return;
}

//=============================================================================
// �{���̏����擾����
//=============================================================================
TRANSITION *GetTransition(void)
{
	return &Transition;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeTransitionVertex(void)
{
	int i = 0, j = 0;

	// ���_���W�̐ݒ�	
	SetTransitionVertex();

	// rhw�̐ݒ�
	for (i = 0; i < TextureDevideNum; i++)
	{
		for (j = 0; j < TextureDevideNum; j++)
		{
			Transition.Pixel[i][j][0].rhw = 1.0f;
			Transition.Pixel[i][j][1].rhw = 1.0f;
			Transition.Pixel[i][j][2].rhw = 1.0f;
			Transition.Pixel[i][j][3].rhw = 1.0f;
		}
	}

	// ���ˌ��̐ݒ�
	for (i = 0; i < TextureDevideNum; i++)
	{
		for (j = 0; j < TextureDevideNum; j++)
		{
			Transition.Pixel[i][j][0].diffuse = WHITE(255);
			Transition.Pixel[i][j][1].diffuse = WHITE(255);
			Transition.Pixel[i][j][2].diffuse = WHITE(255);
			Transition.Pixel[i][j][3].diffuse = WHITE(255);
		}
	}

	// �e�N�X�`�����W�̐ݒ�
	SetTransitionTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTransitionTexture(void)
{
	int i = 0, j = 0;
	float Size_X = 1.0f / (float)TextureDevideNum;
	float Size_Y = 1.0f / (float)TextureDevideNum;

	for (i = 0; i < TextureDevideNum; i++)
	{
		for (j = 0; j < TextureDevideNum; j++)
		{
			Transition.Pixel[i][j][0].tex = D3DXVECTOR2(Size_X * j, Size_Y * i);
			Transition.Pixel[i][j][1].tex = D3DXVECTOR2(Size_X * (j + 1), Size_Y * i);
			Transition.Pixel[i][j][2].tex = D3DXVECTOR2(Size_X * j, Size_Y * (i + 1));
			Transition.Pixel[i][j][3].tex = D3DXVECTOR2(Size_X * (j + 1), Size_Y * (i + 1));
		}
	}

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetTransitionVertex(void)
{
	int i = 0, j = 0;
	float Size_X = Texture_Transition_Width / (float)TextureDevideNum;
	float Size_Y = Texture_Transition_Height / (float)TextureDevideNum;

	for (i = 0; i < TextureDevideNum; i++)
	{
		for (j = 0; j < TextureDevideNum; j++)
		{
			Transition.Pixel[i][j][0].vtx.x = 0.0f + Size_X * j;
			Transition.Pixel[i][j][0].vtx.y = 0.0f + Size_Y * i;
			Transition.Pixel[i][j][1].vtx.x = 0.0f + Size_X * (j + 1);
			Transition.Pixel[i][j][1].vtx.y = 0.0f + Size_Y * i;
			Transition.Pixel[i][j][2].vtx.x = 0.0f + Size_X * j;
			Transition.Pixel[i][j][2].vtx.y = 0.0f + Size_Y * (i + 1);
			Transition.Pixel[i][j][3].vtx.x = 0.0f + Size_X * (j + 1);
			Transition.Pixel[i][j][3].vtx.y = 0.0f + Size_Y * (i + 1);
		}
	}

	return;
}



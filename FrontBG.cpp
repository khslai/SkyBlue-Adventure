//=============================================================================
//
// �O�i���� [FrontBG.cpp]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#include "main.h"
#include "FrontBG.h"
#include "BackGround.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FrontBGSpeed (0.005f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeFrontBGVertex(void);
void SetFrontBGTexture(void);
void SetFrontBGDiffuse(int Alpha);
void SetFrontBGVertex(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 FrontBGTexture = NULL;
BACKGROUND FrontBG;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFrontBG(int InitState)
{
	FrontBG.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	MakeFrontBGVertex();

	if (InitState == FirstInit)
	{
		if (SafeLoad(Texture_FrontBG, &FrontBGTexture, "FrontBG") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFrontBG(void)
{
	SafeRelease(FrontBGTexture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFrontBG(void)
{
	bool BGFadeout = GetBGFadeout();
	int State = GetBGState();
	static int Alpha = 64;

	FrontBG.Pos.x += FrontBGSpeed;

	if (State == Afternoon && BGFadeout == true)
	{
		Alpha = Alpha <= 0 ? 0 : Alpha - 1;
	}
	else if (State == Night && BGFadeout == true)
	{
		Alpha = Alpha >= 64 ? 64 : Alpha + 1;
	}

	SetFrontBGTexture();
	SetFrontBGDiffuse(Alpha);
	SetFrontBGVertex();

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFrontBG(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// ���Z�������[�h�ɐݒu����
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, FrontBGTexture);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_BG, FrontBG.VertexWk, sizeof(VERTEX_2D));

	// �{���̐ݒu��߂�
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeFrontBGVertex(void)
{
	// ���_���W�̐ݒ�	
	SetFrontBGVertex();

	// rhw�̐ݒ�
	FrontBG.VertexWk[0].rhw = 1.0f;
	FrontBG.VertexWk[1].rhw = 1.0f;
	FrontBG.VertexWk[2].rhw = 1.0f;
	FrontBG.VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	FrontBG.VertexWk[0].diffuse = WHITE(255);
	FrontBG.VertexWk[1].diffuse = WHITE(255);
	FrontBG.VertexWk[2].diffuse = WHITE(255);
	FrontBG.VertexWk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetFrontBGTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetFrontBGTexture(void)
{
	float BGDistance_X = (float)Screen_Width / (float)Texture_FrontBG_Width;

	FrontBG.VertexWk[0].tex = D3DXVECTOR2(FrontBG.Pos.x, 0.0f);
	FrontBG.VertexWk[1].tex = D3DXVECTOR2(FrontBG.Pos.x + BGDistance_X, 0.0f);
	FrontBG.VertexWk[2].tex = D3DXVECTOR2(FrontBG.Pos.x, 1.0f);
	FrontBG.VertexWk[3].tex = D3DXVECTOR2(FrontBG.Pos.x + BGDistance_X, 1.0f);

	return;
}

//=============================================================================
// �F�A�����x�̐ݒ�
//=============================================================================
void SetFrontBGDiffuse(int Alpha)
{
	FrontBG.VertexWk[0].diffuse = WHITE(Alpha);
	FrontBG.VertexWk[1].diffuse = WHITE(Alpha);
	FrontBG.VertexWk[2].diffuse = WHITE(Alpha);
	FrontBG.VertexWk[3].diffuse = WHITE(Alpha);

	return;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetFrontBGVertex(void)
{
	FrontBG.VertexWk[0].vtx = D3DXVECTOR3(0, 0, 0);
	FrontBG.VertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0, 0);
	FrontBG.VertexWk[2].vtx = D3DXVECTOR3(0, Screen_Height, 0);
	FrontBG.VertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0);

	return;
}

//=============================================================================
//
// ���K�e���I������ [PraticeSelect.cpp]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#include "main.h"
#include "PraticeSelect.h"
#include "Title.h"
#include "Boss.h"
#include "BossBarrage.h"
#include "Laser.h"
#include "SelectChar.h"
#include "Transition.h"
#include "Sound.h"
#include "Input.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define UpDownArrowPos_Y (200)
#define RectHeight (100)
#define TitlePos_Y (50)
#define OptionPos_Y	(150)
#define ReferencePos_Y (170)




//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakePraticeSelectVertex(void);
void SetPraticeSelectTexture(void);
void SetPraticeSelectVertex(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
VERTEX_2D PraticeSelectBGVertexWk[NUM_VERTEX];
VERTEX_2D PraticeSelectDrawBoxVertexWk[NUM_VERTEX];
VERTEX_2D UpDownArrowVertexWk[NUM_VERTEX];
LPDIRECT3DTEXTURE9 PraticeSelectSelectBGTexture = NULL;
LPDIRECT3DTEXTURE9 PraticeSelectDrawBoxTexture = NULL;
LPDIRECT3DTEXTURE9 UpDownArrowTexture = NULL;
int PraticeSelectPhase = 0;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPraticeSelect(int InitState)
{
	int GameStage = GetGameStage();

	MakePraticeSelectVertex();

	if (GameStage != Stage_GameOver && GameStage != Stage_BossPratice)
	{
		PraticeSelectPhase = SetBossStart;
	}

	if (InitState == FirstInit)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_SelectCharBG, &PraticeSelectSelectBGTexture, "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_DrawBox, &PraticeSelectDrawBoxTexture, "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_UpDownArrow, &UpDownArrowTexture, "BackGround") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPraticeSelect(void)
{
	SafeRelease(PraticeSelectSelectBGTexture);
	SafeRelease(PraticeSelectDrawBoxTexture);
	SafeRelease(UpDownArrowTexture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePraticeSelect(void)
{
	int Bullet_No = 0;
	int Laser_No = 0;
	BOSS *Boss = GetBoss();
	LASER *Laser = GetLaser(Laser_No);
	TRANSITION *Transition = GetTransition();

	// �{�X�ݒu
	if (Transition->FadeInOver != true)
	{
		if (PraticeSelectPhase == SetBossStart)
		{
			InitBoss(ReInit);
			SetBoss();
			Boss->Pos = D3DXVECTOR3(ScreenCenter_X, 500.0f, 0.0f);
			Boss->Phase = MiddleBoss_Phase1;
			PraticeSelectPhase = BossBarrage_M1;
		}
	}

	// ��ʑJ�ځA���̃X�e�[�W�Ɉڂ�
	if (Transition->FadeInOver == true && PraticeSelectPhase != SetBossStart)
	{
		InitBoss(ReInit);
		InitBossBarrage(ReInit);
		InitLaser(ReInit);
		InitSelectChar(ReInit);
		if (PraticeSelectPhase != BackToSelectChar)
		{
			SetGameStage(Stage_BossPratice);
		}
		else if (PraticeSelectPhase == BackToSelectChar)
		{
			SetGameStage(Stage_SelectChar);
			PraticeSelectPhase = SetBossStart;
		}
	}

	// Enter�L�[
	if (Transition->FadeInOver != true)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || GetKeyboardTrigger(DIK_Z) ||
			IsButtonTriggered(0, BUTTON_B))
		{
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			SetTransition();
		}
		if (GetKeyboardTrigger(DIK_X) || IsButtonTriggered(0, BUTTON_A))
		{
			PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
			SetTransition();
			PraticeSelectPhase = BackToSelectChar;
		}
	}

	// �I�����X�V
	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
	{
		PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
		// �e������
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			Boss->Bullet[Bullet_No].Use = false;
		}
		// ���[�U�[������
		Laser_No = 0;
		Laser = GetLaser(Laser_No);
		for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++, Laser++)
		{
			Laser->Use = false;
		}
		// �{�X�̏�Ԃ�������
		Boss->Pos = D3DXVECTOR3(ScreenCenter_X, 500.0f, 0.0f);
		Boss->State = StandBy;
		Boss->ShotCount = 0;

		switch (PraticeSelectPhase)
		{
		case BossBarrage_M1:
			PraticeSelectPhase = BossBarrage_M2;
			Boss->Phase = MiddleBoss_LastPhase;
			break;
		case BossBarrage_M2:
			PraticeSelectPhase = BossBarrage_L1;
			Boss->Phase = LastBoss_Phase1;
			break;
		case BossBarrage_L1:
			PraticeSelectPhase = BossBarrage_L2;
			Boss->Phase = LastBoss_Phase2;
			break;
		case BossBarrage_L2:
			PraticeSelectPhase = BossBarrage_L3;
			Boss->Phase = LastBoss_Phase3;
			break;
		case BossBarrage_L3:
			PraticeSelectPhase = BossBarrage_L4;
			Boss->Phase = LastBoss_LastPhase;
			break;
		case BossBarrage_L4:
			PraticeSelectPhase = BossRush;
			Boss->Phase = 99;
			break;
		case BossRush:
			PraticeSelectPhase = BossBarrage_M1;
			Boss->Phase = MiddleBoss_Phase1;
			break;
		default:
			break;
		}
	}
	else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
	{
		PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
		// �e������
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			Boss->Bullet[Bullet_No].Use = false;
		}
		// ���[�U�[������
		Laser_No = 0;
		Laser = GetLaser(Laser_No);
		for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++, Laser++)
		{
			Laser->Use = false;
		}
		// �{�X�̏�Ԃ�������
		Boss->Pos = D3DXVECTOR3(ScreenCenter_X, 500.0f, 0.0f);
		Boss->State = StandBy;
		Boss->ShotCount = 0;

		switch (PraticeSelectPhase)
		{
		case BossBarrage_M1:
			PraticeSelectPhase = BossRush;
			Boss->Phase = 99;
			break;
		case BossBarrage_M2:
			PraticeSelectPhase = BossBarrage_M1;
			Boss->Phase = MiddleBoss_Phase1;
			break;
		case BossBarrage_L1:
			PraticeSelectPhase = BossBarrage_M2;
			Boss->Phase = MiddleBoss_LastPhase;
			break;
		case BossBarrage_L2:
			PraticeSelectPhase = BossBarrage_L1;
			Boss->Phase = LastBoss_Phase1;
			break;
		case BossBarrage_L3:
			PraticeSelectPhase = BossBarrage_L2;
			Boss->Phase = LastBoss_Phase2;
			break;
		case BossBarrage_L4:
			PraticeSelectPhase = BossBarrage_L3;
			Boss->Phase = LastBoss_Phase3;
			break;
		case BossRush:
			PraticeSelectPhase = BossBarrage_L4;
			Boss->Phase = LastBoss_LastPhase;
			break;
		default:
			break;
		}
	}

	// �e�N�X�`���X�V
	SetPraticeSelectTexture();
	SetPraticeSelectVertex();

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPraticeSelect(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPD3DXFONT Title = GetFont(FontSize_54);
	LPD3DXFONT Option = GetFont(FontSize_40);
	LPD3DXFONT Information = GetFont(FontSize_30);
	RECT TextRect = { 0, 0, Screen_Width, RectHeight };

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, PraticeSelectSelectBGTexture);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PraticeSelectBGVertexWk, sizeof(VERTEX_2D));

	Device->SetTexture(0, PraticeSelectDrawBoxTexture);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PraticeSelectDrawBoxVertexWk, sizeof(VERTEX_2D));

	Device->SetTexture(0, UpDownArrowTexture);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, UpDownArrowVertexWk, sizeof(VERTEX_2D));

	TextRect.top = TitlePos_Y;
	TextRect.bottom = TitlePos_Y + RectHeight;
	Title->DrawText(NULL, "���K�������e����I��ł�������", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));

	TextRect.top = OptionPos_Y;
	TextRect.bottom = OptionPos_Y + RectHeight;
	switch (PraticeSelectPhase)
	{
	case BossBarrage_M1:
		Option->DrawText(NULL, "���{�X���i�K", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	case BossBarrage_M2:
		Option->DrawText(NULL, "���{�X���i�K", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	case BossBarrage_L1:
		Option->DrawText(NULL, "���X�{�X���i�K", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	case BossBarrage_L2:
		Option->DrawText(NULL, "���X�{�X���i�K", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	case BossBarrage_L3:
		Option->DrawText(NULL, "���X�{�X��O�i�K", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	case BossBarrage_L4:
		Option->DrawText(NULL, "���X�{�X��l�i�K", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	case BossRush:
		Option->DrawText(NULL, "�{�X���b�V��", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		break;
	default:
		break;
	}

	TextRect.top = ReferencePos_Y;
	TextRect.bottom = ReferencePos_Y + RectHeight;
	switch (PraticeSelectPhase)
	{
	case BossBarrage_M1:
		Information->DrawText(NULL, "�I���W�i��", -1, &TextRect, DT_RIGHT | DT_VCENTER, WHITE(255));
		break;
	case BossBarrage_M2:
		Information->DrawText(NULL, "�Q�l�F�T�C�����g�Z���i", -1, &TextRect, DT_RIGHT | DT_VCENTER, WHITE(255));
		break;
	case BossBarrage_L1:
		Information->DrawText(NULL, "�Q�l�F�~�V���O�W����", -1, &TextRect, DT_RIGHT | DT_VCENTER, WHITE(255));
		break;
	case BossBarrage_L2:
		Information->DrawText(NULL, "�Q�l�F�p�[�t�F�N�g�t���[�Y", -1, &TextRect, DT_RIGHT | DT_VCENTER, WHITE(255));
		break;
	case BossBarrage_L3:
		Information->DrawText(NULL, "�Q�l�F�l�����_�^Plus", -1, &TextRect, DT_RIGHT | DT_VCENTER, WHITE(255));
		break;
	case BossBarrage_L4:
		Information->DrawText(NULL, "�Q�l�F������-�ꕪ��-", -1, &TextRect, DT_RIGHT | DT_VCENTER, WHITE(255));
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakePraticeSelectVertex(void)
{
	// ���_���W�̐ݒ�	
	SetPraticeSelectVertex();

	// rhw�̐ݒ�
	PraticeSelectBGVertexWk[0].rhw = 1.0f;
	PraticeSelectBGVertexWk[1].rhw = 1.0f;
	PraticeSelectBGVertexWk[2].rhw = 1.0f;
	PraticeSelectBGVertexWk[3].rhw = 1.0f;

	PraticeSelectDrawBoxVertexWk[0].rhw = 1.0f;
	PraticeSelectDrawBoxVertexWk[1].rhw = 1.0f;
	PraticeSelectDrawBoxVertexWk[2].rhw = 1.0f;
	PraticeSelectDrawBoxVertexWk[3].rhw = 1.0f;

	UpDownArrowVertexWk[0].rhw = 1.0f;
	UpDownArrowVertexWk[1].rhw = 1.0f;
	UpDownArrowVertexWk[2].rhw = 1.0f;
	UpDownArrowVertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	PraticeSelectBGVertexWk[0].diffuse = WHITE(255);
	PraticeSelectBGVertexWk[1].diffuse = WHITE(255);
	PraticeSelectBGVertexWk[2].diffuse = WHITE(255);
	PraticeSelectBGVertexWk[3].diffuse = WHITE(255);

	PraticeSelectDrawBoxVertexWk[0].diffuse = SKYBLUE(255);
	PraticeSelectDrawBoxVertexWk[1].diffuse = SKYBLUE(255);
	PraticeSelectDrawBoxVertexWk[2].diffuse = SKYBLUE(255);
	PraticeSelectDrawBoxVertexWk[3].diffuse = SKYBLUE(255);

	UpDownArrowVertexWk[0].diffuse = WHITE(255);
	UpDownArrowVertexWk[1].diffuse = WHITE(255);
	UpDownArrowVertexWk[2].diffuse = WHITE(255);
	UpDownArrowVertexWk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetPraticeSelectTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetPraticeSelectTexture(void)
{
	PraticeSelectBGVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	PraticeSelectBGVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	PraticeSelectBGVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	PraticeSelectBGVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	PraticeSelectDrawBoxVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	PraticeSelectDrawBoxVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	PraticeSelectDrawBoxVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	PraticeSelectDrawBoxVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	UpDownArrowVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	UpDownArrowVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	UpDownArrowVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	UpDownArrowVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetPraticeSelectVertex(void)
{
	PraticeSelectBGVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PraticeSelectBGVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	PraticeSelectBGVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	PraticeSelectBGVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	PraticeSelectDrawBoxVertexWk[0].vtx = D3DXVECTOR3(DrawBox_X, DrawBox_Y, 0.0f);
	PraticeSelectDrawBoxVertexWk[1].vtx = D3DXVECTOR3(DrawBox_X + Texture_DrawBox_Width, DrawBox_Y, 0.0f);
	PraticeSelectDrawBoxVertexWk[2].vtx = D3DXVECTOR3(DrawBox_X, DrawBox_Y + Texture_DrawBox_Height, 0.0f);
	PraticeSelectDrawBoxVertexWk[3].vtx = D3DXVECTOR3(DrawBox_X + Texture_DrawBox_Width, DrawBox_Y + Texture_DrawBox_Height, 0.0f);

	UpDownArrowVertexWk[0].vtx = D3DXVECTOR3(ScreenCenter_X - Texture_UpDownArrow_Width / 2, UpDownArrowPos_Y - Texture_UpDownArrow_Height, 0.0f);
	UpDownArrowVertexWk[1].vtx = D3DXVECTOR3(ScreenCenter_X + Texture_UpDownArrow_Width / 2, UpDownArrowPos_Y - Texture_UpDownArrow_Height, 0.0f);
	UpDownArrowVertexWk[2].vtx = D3DXVECTOR3(ScreenCenter_X - Texture_UpDownArrow_Width / 2, UpDownArrowPos_Y + Texture_UpDownArrow_Height, 0.0f);
	UpDownArrowVertexWk[3].vtx = D3DXVECTOR3(ScreenCenter_X + Texture_UpDownArrow_Width / 2, UpDownArrowPos_Y + Texture_UpDownArrow_Height, 0.0f);

	return;
}

//=============================================================================
// �I�񂾒e�����[�h���擾����
//=============================================================================
int GetPraticeSelect(void)
{
	return PraticeSelectPhase;
}
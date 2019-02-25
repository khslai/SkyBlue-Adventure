//=============================================================================
//
// �{�X���K���[�h���� [BossPratice.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "BossPratice.h"
#include "PraticeSelect.h"
#include "BackGround.h"
#include "Transition.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Boss.h"
#include "BossBarrage.h"
#include "Laser.h"
#include "Sound.h"
#include "Input.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// ���K���[�h�w�i�X�N���[���̑��x
#define BGSpeed				(0.001f)
// �e�L�X�g��`�̍���
#define RectHeight			(100)
// �e�L�X�g�\���̈ʒu
#define PausePos_Y			(128)
#define ResumePos_Y			(350)
#define RestartPos_Y		(450)
#define ToTitlePos_Y		(550)
#define AskSentencePos_Y	(200)
#define YesNoPos_Y			(450)
#define YesPos_X			(256)
#define NoPos_X				(768)


// �{�X���K���[�h�̏��
enum BossPraticePhase
{
	SetBossPraticeStart,
	SetBossPraticeOver,
	Resume,
	Restart,
	RestartCheck,
	ToTitle,
	ToTitleCheck,
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeBossPraticeVertex(void);
// ���_���W�̐ݒ�
void SetBossPraticeVertex(void);
// �e�N�X�`�����W�̐ݒ�
void SetBossPraticeTexture(void);
// �v���C���[�ƃ{�X�̐ݒu
void SetBossPratice(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// ���j���[��ʒ��_
static VERTEX_2D BlackScreenVertexWk[Num_Vertex];
// �w�i�e�N�X�`��
LPDIRECT3DTEXTURE9 BossPraticeBGTexture = NULL;
// �w�i�\����
static BACKGROUND BossPraticeBG;
// �I����
static SELECT BossPraticeSelect;
// ��ʗh���̈ړ���
static D3DXVECTOR2 ShakePos = D3DXVECTOR2(0.0f, 0.0f);

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBossPratice(int InitState)
{
	BossPraticeBG.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ShakePos = D3DXVECTOR2(0.0f, 0.0f);

	BossPraticeSelect.Pos = D3DXVECTOR3(ScreenCenter_X, ResumePos_Y, 0.0f);
	BossPraticeSelect.Phase = SetBossPraticeStart;
	BossPraticeSelect.InYes = false;

	MakeBossPraticeVertex();

	if (InitState == FirstInit)
	{
		if (SafeLoad(Texture_BG_Day, &BossPraticeBGTexture, "BackGround") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBossPratice(void)
{
	SafeRelease(BossPraticeBGTexture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBossPratice(void)
{
	static int Count = 0;
	static int PressCount = 0;
	TRANSITION *Transition = GetTransition();
	BOSS *Boss = GetBoss();

	// �v���C���[�A�{�X�ݒu
	if (BossPraticeSelect.Phase == SetBossPraticeStart)
	{
		ReInitSound();
		PlaySound(BGM_BossPratice, E_DS8_FLAG_LOOP, false);
		SetBossPratice();
		Count = 0;
		BossPraticeSelect.Phase = SetBossPraticeOver;
	}

	// ��ʑJ��
	if (Transition->FadeInOver == true && BossPraticeSelect.InYes == true)
	{
		// �ŏ�����
		if (BossPraticeSelect.Phase == RestartCheck)
		{
			ReInitGame(true);
			SetGameStage(Stage_BossPratice);
		}
		// �^�C�g���ɖ߂�
		else if (BossPraticeSelect.Phase == ToTitleCheck)
		{
			ReInitGame(true);
			SetGameStage(Stage_Title);
		}

		return;
	}

	// �w�i�X�N���[��
	if (BossPraticeSelect.Phase == SetBossPraticeOver)
	{
		BossPraticeBG.Pos.x += BGSpeed;

		// �{�X���|���ꂽ
		if (Boss->Exist == false)
		{
			// ���K���[�h�I��
			if (Transition->FadeInOver == true)
			{
				ReInitGame(true);
				SetGameStage(Stage_Title);
			}

			// ��ʗh���
			if (Count <= 30)
			{
				BossPraticeBG.InShake = true;
			}
			else
			{
				BossPraticeBG.InShake = false;
			}

			// �h�����W�v�Z
			if (BossPraticeBG.InShake == true)
			{
				ShakePos.x = (float)(rand() % (30 + 1) - 15);
				ShakePos.y = (float)(rand() % (30 + 1) - 15);
			}
			else
			{
				ShakePos.x = 0.0f;
				ShakePos.y = 0.0f;
			}

			// �J�ډ�ʐݒu
			if (Count == 180)
			{
				SetTransition();
			}

			Count++;
		}
	}

	// �ꎞ��~
	if ((GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_START)) && BossPraticeSelect.Phase == SetBossPraticeOver)
	{
		PlaySound(SE_Menu, E_DS8_FLAG_NONE, true);
		BossPraticeSelect.Phase = Resume;
		BossPraticeSelect.Pos.x = ScreenCenter_X;
		BossPraticeSelect.Pos.y = ResumePos_Y;

	}

	// Enter�L�[
	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || GetKeyboardTrigger(DIK_Z) ||
		IsButtonTriggered(0, BUTTON_B))
	{
		switch (BossPraticeSelect.Phase)
		{
		case Resume:
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			BossPraticeSelect.Phase = SetBossPraticeOver;
			BossPraticeSelect.Pos.y = ResumePos_Y;
			return;
			break;
		case Restart:
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			BossPraticeSelect.Phase = RestartCheck;
			BossPraticeSelect.Pos.x = NoPos_X;
			BossPraticeSelect.Pos.y = YesNoPos_Y;
			break;
		case RestartCheck:
			if (BossPraticeSelect.InYes == true)
			{
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				SetTransition();
			}
			else
			{
				PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
				BossPraticeSelect.Phase = Restart;
				BossPraticeSelect.Pos.x = ScreenCenter_X;
				BossPraticeSelect.Pos.y = RestartPos_Y;
			}
			break;
		case ToTitle:
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			BossPraticeSelect.Phase = ToTitleCheck;
			BossPraticeSelect.Pos.x = NoPos_X;
			BossPraticeSelect.Pos.y = YesNoPos_Y;
			break;
		case ToTitleCheck:
			if (BossPraticeSelect.InYes == true)
			{
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				SetTransition();
			}
			else
			{
				PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
				BossPraticeSelect.Phase = ToTitle;
				BossPraticeSelect.Pos.x = ScreenCenter_X;
				BossPraticeSelect.Pos.y = ToTitlePos_Y;
			}
			break;
		default:
			break;
		}
	}

	// �I�����ړ�
	if (BossPraticeSelect.Phase == Resume || BossPraticeSelect.Phase == Restart || BossPraticeSelect.Phase == ToTitle)
	{
		if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			switch (BossPraticeSelect.Phase)
			{
			case Resume:
				BossPraticeSelect.Phase = Restart;
				BossPraticeSelect.Pos.y = RestartPos_Y;
				break;
			case Restart:
				BossPraticeSelect.Phase = ToTitle;
				BossPraticeSelect.Pos.y = ToTitlePos_Y;
				break;
			case ToTitle:
				BossPraticeSelect.Phase = Resume;
				BossPraticeSelect.Pos.y = ResumePos_Y;
				break;
			default:
				break;
			}
		}
		else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			switch (BossPraticeSelect.Phase)
			{
			case Resume:
				BossPraticeSelect.Phase = ToTitle;
				BossPraticeSelect.Pos.y = ToTitlePos_Y;
				break;
			case Restart:
				BossPraticeSelect.Phase = Resume;
				BossPraticeSelect.Pos.y = ResumePos_Y;
				break;
			case ToTitle:
				BossPraticeSelect.Phase = Restart;
				BossPraticeSelect.Pos.y = RestartPos_Y;
				break;
			default:
				break;
			}
		}

		// �I�������[�v
		if (GetKeyboardRepeat(DIK_DOWN) || IsButtonRepeat(0, BUTTON_DOWN))
		{
			PressCount++;
			if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				if (BossPraticeSelect.Phase == Resume)
				{
					BossPraticeSelect.Phase = Restart;
					BossPraticeSelect.Pos.y = RestartPos_Y;
				}
				else if (BossPraticeSelect.Phase == Restart)
				{
					BossPraticeSelect.Phase = ToTitle;
					BossPraticeSelect.Pos.y = ToTitlePos_Y;
				}
				else if (BossPraticeSelect.Phase == ToTitle)
				{
					BossPraticeSelect.Phase = Resume;
					BossPraticeSelect.Pos.y = ResumePos_Y;
				}
			}
		}
		else if (GetKeyboardRepeat(DIK_UP) || IsButtonRepeat(0, BUTTON_UP))
		{
			PressCount++;
			if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				if (BossPraticeSelect.Phase == Resume)
				{
					BossPraticeSelect.Phase = ToTitle;
					BossPraticeSelect.Pos.y = ToTitlePos_Y;
				}
				else if (BossPraticeSelect.Phase == Restart)
				{
					BossPraticeSelect.Phase = Resume;
					BossPraticeSelect.Pos.y = ResumePos_Y;
				}
				else if (BossPraticeSelect.Phase == ToTitle)
				{
					BossPraticeSelect.Phase = Restart;
					BossPraticeSelect.Pos.y = RestartPos_Y;
				}
			}
		}


		if (GetKeyboardRelease(DIK_UP) || GetKeyboardRelease(DIK_DOWN) ||
			IsButtonReleased(0, BUTTON_UP) || IsButtonReleased(0, BUTTON_DOWN))
		{
			PressCount = 0;
		}
	}

	// �͂��A�������X�V
	if (BossPraticeSelect.Phase == RestartCheck || BossPraticeSelect.Phase == ToTitleCheck)
	{
		if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			BossPraticeSelect.Pos.x = YesPos_X;
			BossPraticeSelect.InYes = true;
		}
		else if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			BossPraticeSelect.Pos.x = NoPos_X;
			BossPraticeSelect.InYes = false;
		}
	}

	// �e�N�X�`���X�V
	SetBossPraticeTexture();
	SetBossPraticeVertex();

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBossPratice(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, BossPraticeBGTexture);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BossPraticeBG.VertexWk, sizeof(VERTEX_2D));

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBossPraticeMenu(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPDIRECT3DTEXTURE9 BlackScreenTexture = GetUITexture(BlackScreen);
	LPDIRECT3DTEXTURE9 BossPraticeSelectTexture = GetUITexture(SelectBox);
	LPD3DXFONT Pause = GetFont(FontSize_108);
	LPD3DXFONT Option = GetFont(FontSize_54);
	RECT TextRect = { 0, 0, Screen_Width, RectHeight };

	if (BossPraticeSelect.Phase != SetBossPraticeStart && BossPraticeSelect.Phase != SetBossPraticeOver)
	{
		// ������ʕ`��
		Device->SetTexture(0, BlackScreenTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BlackScreenVertexWk, sizeof(VERTEX_2D));

		// �I��g�`��
		Device->SetTexture(0, BossPraticeSelectTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BossPraticeSelect.VertexWk, sizeof(VERTEX_2D));

		if (BossPraticeSelect.Phase != RestartCheck && BossPraticeSelect.Phase != ToTitleCheck)
		{
			TextRect.top = PausePos_Y;
			TextRect.bottom = PausePos_Y + RectHeight;
			Pause->DrawText(NULL, "P A U S E", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));

			TextRect.top = ResumePos_Y;
			TextRect.bottom = ResumePos_Y + RectHeight;
			if (BossPraticeSelect.Phase == Resume)
			{
				Option->DrawText(NULL, "����", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Option->DrawText(NULL, "����", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}

			TextRect.top = RestartPos_Y;
			TextRect.bottom = RestartPos_Y + RectHeight;
			if (BossPraticeSelect.Phase == Restart)
			{
				Option->DrawText(NULL, "�ŏ�����", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Option->DrawText(NULL, "�ŏ�����", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}

			TextRect.top = ToTitlePos_Y;
			TextRect.bottom = ToTitlePos_Y + RectHeight;
			if (BossPraticeSelect.Phase == ToTitle)
			{
				Option->DrawText(NULL, "�^�C�g����", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Option->DrawText(NULL, "�^�C�g����", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
		}
		else if (BossPraticeSelect.Phase == RestartCheck || BossPraticeSelect.Phase == ToTitleCheck)
		{
			if (BossPraticeSelect.Phase == RestartCheck)
			{
				TextRect.top = AskSentencePos_Y;
				TextRect.bottom = AskSentencePos_Y + RectHeight;
				Option->DrawText(NULL, "�ŏ������낵�ł����H", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			else if (BossPraticeSelect.Phase == ToTitleCheck)
			{
				TextRect.top = AskSentencePos_Y;
				TextRect.bottom = AskSentencePos_Y + RectHeight;
				Option->DrawText(NULL, "�^�C�g���ɖ߂�܂����H", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}

			TextRect.top = YesNoPos_Y;
			TextRect.bottom = YesNoPos_Y + RectHeight;
			TextRect.left = 0;
			TextRect.right = ScreenCenter_X;
			if (BossPraticeSelect.InYes == true)
			{
				Option->DrawText(NULL, "�͂�", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Option->DrawText(NULL, "�͂�", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			TextRect.left = ScreenCenter_X;
			TextRect.right = Screen_Width;
			if (BossPraticeSelect.InYes == true)
			{
				Option->DrawText(NULL, "������", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			else
			{
				Option->DrawText(NULL, "������", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
		}
	}

	return;
}

//=============================================================================
// �v���C���[�ƃ{�X�̐ݒu
//=============================================================================
void SetBossPratice(void)
{
	int PraticeSelect = GetPraticeSelect();
	BOSS *Boss = GetBoss();

	SetPlayer();

	SetBoss();
	switch (PraticeSelect)
	{
	case BossBarrage_M1:
		Boss->Phase = MiddleBoss_Phase1;
		break;
	case BossBarrage_M2:
		Boss->Phase = MiddleBoss_LastPhase;
		break;
	case BossBarrage_L1:
		Boss->Phase = LastBoss_Phase1;
		break;
	case BossBarrage_L2:
		Boss->Phase = LastBoss_Phase2;
		break;
	case BossBarrage_L3:
		Boss->Phase = LastBoss_Phase3;
		break;
	case BossBarrage_L4:
		Boss->Phase = LastBoss_LastPhase;
		break;
	case BossRush:
		Boss->Phase = MiddleBoss_Phase1;
		break;
	default:
		break;
	}


	return;
}

//=============================================================================
// �`���[�g���A���̏�Ԃ��擾����
//=============================================================================
bool GetExitPratice(void)
{
	if (BossPraticeSelect.Phase != SetBossPraticeStart && BossPraticeSelect.Phase != SetBossPraticeOver)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeBossPraticeVertex(void)
{
	// ���_���W�̐ݒ�	
	SetBossPraticeVertex();

	// rhw�̐ݒ�
	BossPraticeBG.VertexWk[0].rhw = 1.0f;
	BossPraticeBG.VertexWk[1].rhw = 1.0f;
	BossPraticeBG.VertexWk[2].rhw = 1.0f;
	BossPraticeBG.VertexWk[3].rhw = 1.0f;

	BlackScreenVertexWk[0].rhw = 1.0f;
	BlackScreenVertexWk[1].rhw = 1.0f;
	BlackScreenVertexWk[2].rhw = 1.0f;
	BlackScreenVertexWk[3].rhw = 1.0f;

	BossPraticeSelect.VertexWk[0].rhw = 1.0f;
	BossPraticeSelect.VertexWk[1].rhw = 1.0f;
	BossPraticeSelect.VertexWk[2].rhw = 1.0f;
	BossPraticeSelect.VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	BossPraticeBG.VertexWk[0].diffuse = WHITE(255);
	BossPraticeBG.VertexWk[1].diffuse = WHITE(255);
	BossPraticeBG.VertexWk[2].diffuse = WHITE(255);
	BossPraticeBG.VertexWk[3].diffuse = WHITE(255);

	BlackScreenVertexWk[0].diffuse = WHITE(200);
	BlackScreenVertexWk[1].diffuse = WHITE(200);
	BlackScreenVertexWk[2].diffuse = WHITE(200);
	BlackScreenVertexWk[3].diffuse = WHITE(200);

	BossPraticeSelect.VertexWk[0].diffuse = WHITE(255);
	BossPraticeSelect.VertexWk[1].diffuse = WHITE(255);
	BossPraticeSelect.VertexWk[2].diffuse = WHITE(255);
	BossPraticeSelect.VertexWk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetBossPraticeTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetBossPraticeTexture(void)
{
	float BGRate_X = (float)Screen_Width / (float)Texture_BG_Width;

	BossPraticeBG.VertexWk[0].tex = D3DXVECTOR2(BossPraticeBG.Pos.x, 0.0f);
	BossPraticeBG.VertexWk[1].tex = D3DXVECTOR2(BossPraticeBG.Pos.x + BGRate_X, 0.0f);
	BossPraticeBG.VertexWk[2].tex = D3DXVECTOR2(BossPraticeBG.Pos.x, 1.0f);
	BossPraticeBG.VertexWk[3].tex = D3DXVECTOR2(BossPraticeBG.Pos.x + BGRate_X, 1.0f);

	BlackScreenVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	BlackScreenVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	BlackScreenVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	BlackScreenVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	BossPraticeSelect.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	BossPraticeSelect.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	BossPraticeSelect.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	BossPraticeSelect.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetBossPraticeVertex(void)
{
	BossPraticeBG.VertexWk[0].vtx = D3DXVECTOR3(0.0f + ShakePos.x, 0.0f + ShakePos.y, 0.0f);
	BossPraticeBG.VertexWk[1].vtx = D3DXVECTOR3(Screen_Width + ShakePos.x, 0.0f + ShakePos.y, 0.0f);
	BossPraticeBG.VertexWk[2].vtx = D3DXVECTOR3(0.0f + ShakePos.x, Screen_Height + ShakePos.y, 0.0f);
	BossPraticeBG.VertexWk[3].vtx = D3DXVECTOR3(Screen_Width + ShakePos.x, Screen_Height + ShakePos.y, 0.0f);

	BlackScreenVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BlackScreenVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	BlackScreenVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	BlackScreenVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	BossPraticeSelect.VertexWk[0].vtx = D3DXVECTOR3(BossPraticeSelect.Pos.x - Texture_SelectBox_Width / 2, (float)BossPraticeSelect.Pos.y, 0.0f);
	BossPraticeSelect.VertexWk[1].vtx = D3DXVECTOR3(BossPraticeSelect.Pos.x + Texture_SelectBox_Width / 2, (float)BossPraticeSelect.Pos.y, 0.0f);
	BossPraticeSelect.VertexWk[2].vtx = D3DXVECTOR3(BossPraticeSelect.Pos.x - Texture_SelectBox_Width / 2, (float)BossPraticeSelect.Pos.y + Texture_SelectBox_Height, 0.0f);
	BossPraticeSelect.VertexWk[3].vtx = D3DXVECTOR3(BossPraticeSelect.Pos.x + Texture_SelectBox_Width / 2, (float)BossPraticeSelect.Pos.y + Texture_SelectBox_Height, 0.0f);

	return;
}
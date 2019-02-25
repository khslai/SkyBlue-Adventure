//=============================================================================
//
// �ꎞ��~���� [Pause.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "Pause.h"
#include "Player.h"
#include "Transition.h"
#include "Sound.h"
#include "Input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�L�X�g��`�̍���
#define RectHeight (100)
// �e�L�X�g�\���̍��W
#define PausePos_Y (128)
#define ResumePos_Y (350)
#define RestartPos_Y (450)
#define ToTitlePos_Y (550)
#define AskSentencePos_Y (200)
#define YesNoPos_Y (450)
#define YesPos_X (256)
#define NoPos_X (768)

// �ꎞ��~�̏��
enum
{
	Resume,			// ����
	Restart,		// �ŏ�����
	ToTitle,		// �^�C�g����
	RestartCheck,	// �ŏ�����̊m�F
	ToTitleCheck,	// �^�C�g���ւ̊m�F
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakePauseVertex(void);
// ���_���W�̐ݒ�
void SetPauseVertex(void);
// �e�N�X�`�����W�̐ݒ�
void SetPauseTexture(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// ���_���i�[���[�N
VERTEX_2D PauseVertexWk[Num_Vertex];
// �ꎞ��~�I����
static SELECT PauseSelect;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPause(int InitState)
{
	PauseSelect.Pos = D3DXVECTOR3(ScreenCenter_X, ResumePos_Y, 0.0f);
	PauseSelect.Phase = Resume;
	PauseSelect.InYes = false;

	// ���_���̍쐬
	MakePauseVertex();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPause(void)
{

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePause(void)
{
	static int PressCount = 0;
	PLAYER *Player = GetPlayer(0);
	TRANSITION *Transition = GetTransition();

	// �X�e�[�W�J��
	if (Transition->FadeInOver == true && PauseSelect.InYes == true)
	{
		if (PauseSelect.Phase == RestartCheck)
		{
			ReInitGame(true);
			SetGameStage(Stage_Game);
		}
		else if (PauseSelect.Phase == ToTitleCheck)
		{
			ReInitGame(true);
			SetGameStage(Stage_Title);
		}

		return;
	}

	// Enter�L�[
	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || GetKeyboardTrigger(DIK_Z) ||
		IsButtonTriggered(0, BUTTON_B))
	{
		switch (PauseSelect.Phase)
		{
		case Resume:
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			SetGameStage(Stage_Game);
			PauseSelect.Phase = Resume;
			PauseSelect.Pos.y = ResumePos_Y;
			return;
			break;
		case Restart:
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			PauseSelect.Phase = RestartCheck;
			PauseSelect.Pos.x = NoPos_X;
			PauseSelect.Pos.y = YesNoPos_Y;
			break;
		case RestartCheck:
			if (PauseSelect.InYes == true)
			{
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				SetTransition();
			}
			else
			{
				PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
				PauseSelect.Phase = Restart;
				PauseSelect.Pos.x = ScreenCenter_X;
				PauseSelect.Pos.y = RestartPos_Y;
			}
			break;
		case ToTitle:
			PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
			PauseSelect.Phase = ToTitleCheck;
			PauseSelect.Pos.x = NoPos_X;
			PauseSelect.Pos.y = YesNoPos_Y;
			break;
		case ToTitleCheck:
			if (PauseSelect.InYes == true)
			{
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				SetTransition();
			}
			else
			{
				PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
				PauseSelect.Phase = ToTitle;
				PauseSelect.Pos.x = ScreenCenter_X;
				PauseSelect.Pos.y = ToTitlePos_Y;
			}
			break;
		default:
			break;
		}
	}

	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
	{
		PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
		switch (PauseSelect.Phase)
		{
		case Resume:
			PauseSelect.Phase = Restart;
			PauseSelect.Pos.y = RestartPos_Y;
			break;
		case Restart:
			PauseSelect.Phase = ToTitle;
			PauseSelect.Pos.y = ToTitlePos_Y;
			break;
		case ToTitle:
			PauseSelect.Phase = Resume;
			PauseSelect.Pos.y = ResumePos_Y;
			break;
		default:
			break;
		}
	}
	else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
	{
		PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
		switch (PauseSelect.Phase)
		{
		case Resume:
			PauseSelect.Phase = ToTitle;
			PauseSelect.Pos.y = ToTitlePos_Y;
			break;
		case Restart:
			PauseSelect.Phase = Resume;
			PauseSelect.Pos.y = ResumePos_Y;
			break;
		case ToTitle:
			PauseSelect.Phase = Restart;
			PauseSelect.Pos.y = RestartPos_Y;
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
			if (PauseSelect.Phase == Resume)
			{
				PauseSelect.Phase = Restart;
				PauseSelect.Pos.y = RestartPos_Y;
			}
			else if (PauseSelect.Phase == Restart)
			{
				PauseSelect.Phase = ToTitle;
				PauseSelect.Pos.y = ToTitlePos_Y;
			}
			else if (PauseSelect.Phase == ToTitle)
			{
				PauseSelect.Phase = Resume;
				PauseSelect.Pos.y = ResumePos_Y;
			}
		}
	}
	else if (GetKeyboardRepeat(DIK_UP) || IsButtonRepeat(0, BUTTON_UP))
	{
		PressCount++;
		if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			if (PauseSelect.Phase == Resume)
			{
				PauseSelect.Phase = ToTitle;
				PauseSelect.Pos.y = ToTitlePos_Y;
			}
			else if (PauseSelect.Phase == Restart)
			{
				PauseSelect.Phase = Resume;
				PauseSelect.Pos.y = ResumePos_Y;
			}
			else if (PauseSelect.Phase == ToTitle)
			{
				PauseSelect.Phase = Restart;
				PauseSelect.Pos.y = RestartPos_Y;
			}
		}
	}


	if (GetKeyboardRelease(DIK_UP) || GetKeyboardRelease(DIK_DOWN) ||
		IsButtonReleased(0, BUTTON_UP) || IsButtonReleased(0, BUTTON_DOWN))
	{
		PressCount = 0;
	}

	if (PauseSelect.Phase == RestartCheck || PauseSelect.Phase == ToTitleCheck)
	{
		if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			PauseSelect.Pos.x = YesPos_X;
			PauseSelect.InYes = true;
		}
		else if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			PauseSelect.Pos.x = NoPos_X;
			PauseSelect.InYes = false;
		}
	}

	// �e�N�X�`���X�V
	SetPauseVertex();

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPDIRECT3DTEXTURE9 PauseTexture = GetUITexture(BlackScreen);
	LPDIRECT3DTEXTURE9 PauseSelectTexture = GetUITexture(SelectBox);
	LPD3DXFONT Pause = GetFont(FontSize_108);
	LPD3DXFONT Option = GetFont(FontSize_54);
	RECT rect = { 0, 0, Screen_Width, RectHeight };

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, PauseTexture);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PauseVertexWk, sizeof(VERTEX_2D));

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, PauseSelectTexture);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PauseSelect.VertexWk, sizeof(VERTEX_2D));

	if (PauseSelect.Phase != RestartCheck && PauseSelect.Phase != ToTitleCheck)
	{
		rect.top = PausePos_Y;
		rect.bottom = PausePos_Y + RectHeight;
		Pause->DrawText(NULL, "P A U S E", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		rect.top = ResumePos_Y;
		rect.bottom = ResumePos_Y + RectHeight;
		if (PauseSelect.Phase == Resume)
		{
			Option->DrawText(NULL, "����", -1, &rect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "����", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}

		rect.top = RestartPos_Y;
		rect.bottom = RestartPos_Y + RectHeight;
		if (PauseSelect.Phase == Restart)
		{
			Option->DrawText(NULL, "�ŏ�����", -1, &rect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "�ŏ�����", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}

		rect.top = ToTitlePos_Y;
		rect.bottom = ToTitlePos_Y + RectHeight;
		if (PauseSelect.Phase == ToTitle)
		{
			Option->DrawText(NULL, "�^�C�g����", -1, &rect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "�^�C�g����", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
	}
	else
	{
		rect.top = AskSentencePos_Y;
		rect.bottom = AskSentencePos_Y + RectHeight;
		if (PauseSelect.Phase == RestartCheck)
		{
			Option->DrawText(NULL, "�ŏ������낵�ł����H", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		else if (PauseSelect.Phase == ToTitleCheck)
		{
			Option->DrawText(NULL, "�^�C�g���ɖ߂�܂����H", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		rect.top = YesNoPos_Y;
		rect.bottom = YesNoPos_Y + RectHeight;
		rect.left = 0;
		rect.right = ScreenCenter_X;
		if (PauseSelect.InYes == true)
		{
			Option->DrawText(NULL, "�͂�", -1, &rect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "�͂�", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		rect.left = ScreenCenter_X;
		rect.right = Screen_Width;
		if (PauseSelect.InYes == true)
		{
			Option->DrawText(NULL, "������", -1, &rect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		else
		{
			Option->DrawText(NULL, "������", -1, &rect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
	}

	return;
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakePauseVertex(void)
{
	// ���_���W�̐ݒ�	
	SetPauseTexture();

	// rhw�̐ݒ�
	PauseVertexWk[0].rhw = 1.0f;
	PauseVertexWk[1].rhw = 1.0f;
	PauseVertexWk[2].rhw = 1.0f;
	PauseVertexWk[3].rhw = 1.0f;

	PauseSelect.VertexWk[0].rhw = 1.0f;
	PauseSelect.VertexWk[1].rhw = 1.0f;
	PauseSelect.VertexWk[2].rhw = 1.0f;
	PauseSelect.VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	PauseVertexWk[0].diffuse = WHITE(200);
	PauseVertexWk[1].diffuse = WHITE(200);
	PauseVertexWk[2].diffuse = WHITE(200);
	PauseVertexWk[3].diffuse = WHITE(200);

	PauseSelect.VertexWk[0].diffuse = WHITE(255);
	PauseSelect.VertexWk[1].diffuse = WHITE(255);
	PauseSelect.VertexWk[2].diffuse = WHITE(255);
	PauseSelect.VertexWk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetPauseVertex();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetPauseTexture(void)
{
	PauseVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	PauseVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	PauseVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	PauseVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	PauseSelect.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	PauseSelect.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	PauseSelect.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	PauseSelect.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetPauseVertex(void)
{
	PauseVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PauseVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	PauseVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	PauseVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	PauseSelect.VertexWk[0].vtx = D3DXVECTOR3(PauseSelect.Pos.x - Texture_SelectBox_Width / 2, (float)PauseSelect.Pos.y, 0.0f);
	PauseSelect.VertexWk[1].vtx = D3DXVECTOR3(PauseSelect.Pos.x + Texture_SelectBox_Width / 2, (float)PauseSelect.Pos.y, 0.0f);
	PauseSelect.VertexWk[2].vtx = D3DXVECTOR3(PauseSelect.Pos.x - Texture_SelectBox_Width / 2, (float)PauseSelect.Pos.y + Texture_SelectBox_Height, 0.0f);
	PauseSelect.VertexWk[3].vtx = D3DXVECTOR3(PauseSelect.Pos.x + Texture_SelectBox_Width / 2, (float)PauseSelect.Pos.y + Texture_SelectBox_Height, 0.0f);

	return;
}
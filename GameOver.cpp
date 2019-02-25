//=============================================================================
//
// �Q�[���I�[�o�[���� [GameOver.cpp]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#include "main.h"
#include "GameOver.h"
#include "Player.h"
#include "Boss.h"
#include "Pause.h"
#include "Transition.h"
#include "Sound.h"
#include "PraticeSelect.h"
#include "Input.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RectHeight (100)
#define GameOverPos_Y (128)
#define RestartPos_Y (384)
#define ToTitlePos_Y (512)
#define AskSentencePos_Y (300)
#define YesNoPos_Y (500)
#define YesPos_X (256)
#define NoPos_X (768)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeGameOverVertex(void);
void SetGameOverTexture(void);
void SetGameOverVertex(void);

enum GameOverSelectPhase
{
	BGMStart,
	Display,
	Restart,
	ToTitle,
	RestartCheck,
	ToTitleCheck,
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// ���_���i�[���[�N
static VERTEX_2D BlackScreenVertexWk[NUM_VERTEX];
// ���Ԍv�Z�p�J�E���g
static int Count = 0;
// "GAME OVER"�̃A���t�@�l
static int Alpha = 0;
static SELECT GameOverSelect;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGameOver(int InitState)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	GameOverSelect.Pos = D3DXVECTOR3(ScreenCenter_X, RestartPos_Y, 0.0f);
	GameOverSelect.Phase = BGMStart;
	GameOverSelect.InYes = false;

	Count = 0;
	Alpha = 0;

	// ���_���̍쐬
	MakeGameOverVertex();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGameOver(void)
{

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGameOver(void)
{
	int PraticeSelect = GetPraticeSelect();
	static int PressCount = 0;
	TRANSITION *Transition = GetTransition();

	// ��ʑJ��
	if (Transition->FadeInOver == true && GameOverSelect.InYes == true)
	{
		if (GameOverSelect.Phase == RestartCheck)
		{
			if (PraticeSelect == SetBossStart)
			{
				ReInitGame(true);
				SetGameStage(Stage_Game);
			}
			else
			{
				ReInitGame(true);
				SetGameStage(Stage_BossPratice);
			}
		}
		else if (GameOverSelect.Phase == ToTitleCheck)
		{
			ReInitGame(true);
			SetGameStage(Stage_Title);
		}

		return;
	}

	// Game Over�̕������\��������Ȃ����
	if (GameOverSelect.Phase != Display && GameOverSelect.Phase != BGMStart)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || GetKeyboardTrigger(DIK_Z) ||
			IsButtonTriggered(0, BUTTON_B))
		{
			switch (GameOverSelect.Phase)
			{
			case Restart:
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				GameOverSelect.Phase = RestartCheck;
				GameOverSelect.Pos.x = NoPos_X;
				GameOverSelect.Pos.y = YesNoPos_Y;
				break;
			case RestartCheck:
				if (GameOverSelect.InYes == true)
				{
					PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
					SetTransition();
				}
				else
				{
					PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
					GameOverSelect.Phase = Restart;
					GameOverSelect.Pos.x = ScreenCenter_X;
					GameOverSelect.Pos.y = RestartPos_Y;
				}
				break;
			case ToTitle:
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				GameOverSelect.Phase = ToTitleCheck;
				GameOverSelect.Pos.x = NoPos_X;
				GameOverSelect.Pos.y = YesNoPos_Y;
				break;
			case ToTitleCheck:
				if (GameOverSelect.InYes == true)
				{
					PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
					SetTransition();
				}
				else
				{
					PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
					GameOverSelect.Phase = ToTitle;
					GameOverSelect.Pos.x = ScreenCenter_X;
					GameOverSelect.Pos.y = ToTitlePos_Y;
				}
				break;
			default:
				break;
			}
		}

		if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_DOWN) ||
			IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, BUTTON_DOWN))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			switch (GameOverSelect.Phase)
			{
			case Restart:
				GameOverSelect.Phase = ToTitle;
				GameOverSelect.Pos.y = ToTitlePos_Y;
				break;
			case ToTitle:
				GameOverSelect.Phase = Restart;
				GameOverSelect.Pos.y = RestartPos_Y;
				break;
			default:
				break;
			}
		}

		// �I�������[�v
		if (GetKeyboardRepeat(DIK_DOWN) || IsButtonRepeat(0, BUTTON_DOWN) ||
			GetKeyboardRepeat(DIK_UP) || IsButtonRepeat(0, BUTTON_UP))
		{
			PressCount++;
			if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				if (GameOverSelect.Phase == Restart)
				{
					GameOverSelect.Phase = ToTitle;
					GameOverSelect.Pos.y = ToTitlePos_Y;
				}
				else if (GameOverSelect.Phase == ToTitle)
				{
					GameOverSelect.Phase = Restart;
					GameOverSelect.Pos.y = RestartPos_Y;
				}
			}
		}

		// �v���X�J�E���g������
		if (GetKeyboardRelease(DIK_UP) || GetKeyboardRelease(DIK_DOWN) ||
			IsButtonReleased(0, BUTTON_UP) || IsButtonReleased(0, BUTTON_DOWN))
		{
			PressCount = 0;
		}

		if (GameOverSelect.Phase == RestartCheck || GameOverSelect.Phase == ToTitleCheck)
		{
			if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				GameOverSelect.Pos.x = YesPos_X;
				GameOverSelect.InYes = true;
			}
			else if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				GameOverSelect.Pos.x = NoPos_X;
				GameOverSelect.InYes = false;
			}
		}

		SetGameOverVertex();
	}
	else if (GameOverSelect.Phase == BGMStart)
	{
		ReInitSound();
		PlaySound(SE_PlayerDead, E_DS8_FLAG_NONE, true);
		PlaySound(BGM_GameOver, E_DS8_FLAG_LOOP, false);
		GameOverSelect.Phase = Display;
	}

	// �J�E���g���Z
	Count++;

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGameOver(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPDIRECT3DTEXTURE9 BlackScreenTexture = GetUITexture(BlackScreen);
	LPDIRECT3DTEXTURE9 GameOverSelectTexture = GetUITexture(SelectBox);
	LPD3DXFONT GameOver = GetFont(FontSize_108);
	LPD3DXFONT Option = GetFont(FontSize_54);
	RECT TextRect = { 0, 0, Screen_Width, RectHeight };

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, BlackScreenTexture);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BlackScreenVertexWk, sizeof(VERTEX_2D));

	TextRect.top = GameOverPos_Y;
	TextRect.bottom = GameOverPos_Y + Texture_SelectBox_Height;
	if (GameOverSelect.Phase == Display)
	{
		Alpha += 2;
		if (Alpha >= 255)
		{
			Alpha = 255;
			GameOverSelect.Phase = Restart;
		}
	}
	GameOver->DrawText(NULL, "GAME OVER", -1, &TextRect, DT_CENTER | DT_VCENTER, RED(Alpha));

	if (GameOverSelect.Phase != Display && GameOverSelect.Phase != BGMStart)
	{
		Device->SetTexture(0, GameOverSelectTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, GameOverSelect.VertexWk, sizeof(VERTEX_2D));

		if (GameOverSelect.Phase == Restart || GameOverSelect.Phase == ToTitle)
		{
			TextRect.top = RestartPos_Y;
			TextRect.bottom = RestartPos_Y + Texture_SelectBox_Height;
			if (GameOverSelect.Phase == Restart)
			{
				Option->DrawText(NULL, "�ŏ�����", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Option->DrawText(NULL, "�ŏ�����", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			TextRect.top = ToTitlePos_Y;
			TextRect.bottom = ToTitlePos_Y + Texture_SelectBox_Height;
			if (GameOverSelect.Phase == ToTitle)
			{
				Option->DrawText(NULL, "�^�C�g����", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Option->DrawText(NULL, "�^�C�g����", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
		}
		else
		{
			TextRect.top = AskSentencePos_Y;
			TextRect.bottom = AskSentencePos_Y + 100;
			if (GameOverSelect.Phase == RestartCheck)
			{
				Option->DrawText(NULL, "�ŏ������낵�ł����H", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			else if (GameOverSelect.Phase == ToTitleCheck)
			{
				Option->DrawText(NULL, "�^�C�g���ɖ߂�܂����H", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			TextRect.top = YesNoPos_Y;
			TextRect.bottom = YesNoPos_Y + 100;
			TextRect.left = 0;
			TextRect.right = ScreenCenter_X;
			if (GameOverSelect.InYes == true)
			{
				Option->DrawText(NULL, "�͂�", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Option->DrawText(NULL, "�͂�", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			TextRect.left = ScreenCenter_X;
			TextRect.right = Screen_Width;
			if (GameOverSelect.InYes == true)
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
// ���_�̍쐬
//=============================================================================
HRESULT MakeGameOverVertex(void)
{
	// ���_���W�̐ݒ�	
	SetGameOverTexture();

	// rhw�̐ݒ�
	BlackScreenVertexWk[0].rhw = 1.0f;
	BlackScreenVertexWk[1].rhw = 1.0f;
	BlackScreenVertexWk[2].rhw = 1.0f;
	BlackScreenVertexWk[3].rhw = 1.0f;

	GameOverSelect.VertexWk[0].rhw = 1.0f;
	GameOverSelect.VertexWk[1].rhw = 1.0f;
	GameOverSelect.VertexWk[2].rhw = 1.0f;
	GameOverSelect.VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	BlackScreenVertexWk[0].diffuse = WHITE(200);
	BlackScreenVertexWk[1].diffuse = WHITE(200);
	BlackScreenVertexWk[2].diffuse = WHITE(200);
	BlackScreenVertexWk[3].diffuse = WHITE(200);

	GameOverSelect.VertexWk[0].diffuse = WHITE(255);
	GameOverSelect.VertexWk[1].diffuse = WHITE(255);
	GameOverSelect.VertexWk[2].diffuse = WHITE(255);
	GameOverSelect.VertexWk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetGameOverVertex();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetGameOverTexture(void)
{
	BlackScreenVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	BlackScreenVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	BlackScreenVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	BlackScreenVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	GameOverSelect.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	GameOverSelect.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	GameOverSelect.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	GameOverSelect.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetGameOverVertex(void)
{
	BlackScreenVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BlackScreenVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	BlackScreenVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	BlackScreenVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	GameOverSelect.VertexWk[0].vtx = D3DXVECTOR3(GameOverSelect.Pos.x - Texture_SelectBox_Width / 2, (float)GameOverSelect.Pos.y, 0.0f);
	GameOverSelect.VertexWk[1].vtx = D3DXVECTOR3(GameOverSelect.Pos.x + Texture_SelectBox_Width / 2, (float)GameOverSelect.Pos.y, 0.0f);
	GameOverSelect.VertexWk[2].vtx = D3DXVECTOR3(GameOverSelect.Pos.x - Texture_SelectBox_Width / 2, (float)GameOverSelect.Pos.y + Texture_SelectBox_Height, 0.0f);
	GameOverSelect.VertexWk[3].vtx = D3DXVECTOR3(GameOverSelect.Pos.x + Texture_SelectBox_Width / 2, (float)GameOverSelect.Pos.y + Texture_SelectBox_Height, 0.0f);

	return;
}
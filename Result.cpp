//=============================================================================
//
// ���U���g���� [Result.cpp]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#include "main.h"
#include "Result.h"
#include "Player.h"
#include "Pause.h"
#include "Transition.h"
#include "Sound.h"
#include "Input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RectHeight (100)
// �`��n�_
#define RectPos_X (112)
#define RectPos_Y (84)
#define LinePos_X (112)
#define LinePos_Y (484)
// ���j���[���W
#define PressAnyKeyPos_Y (610)
#define RestartPos_Y (264)
#define ToTitlePos_Y (444)
#define AskSentencePos_Y (200)
#define YesNoPos_Y (450)
#define YesPos_X (256)
#define NoPos_X (768)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeResultVertex(void);
void SetResultTexture(void);
void SetResultVertex(void);

enum
{
	Calculating,
	ScoreCalOver,
	Restart,
	ToTitle,
	RestartCheck,
	ToTitleCheck,
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// ���_���i�[���[�N
VERTEX_2D ResultBGVertexWk[NUM_VERTEX];
static VERTEX_2D BlackScreenVertexWk[NUM_VERTEX];
// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 ResultBGTexture = NULL;
LPD3DXLINE ResultLine = NULL;
// ���Ԍv�Z�p�J�E���g
static int Count = 0;
// �X�R�A�v�Z�p
static int TotalScore = 0;
static SELECT ResultSelect;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(int InitState)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	ResultSelect.Pos = D3DXVECTOR3(ScreenCenter_X, RestartPos_Y, 0.0f);
	ResultSelect.Phase = Calculating;
	ResultSelect.InYes = false;

	Count = 0;
	TotalScore = 0;

	// ���_���̍쐬
	MakeResultVertex();

	if (InitState == FirstInit)
	{
		// ���\���p���C����ݒ�
		D3DXCreateLine(Device, &ResultLine);

		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_ResultBG, &ResultBGTexture, "Player") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	SafeRelease(ResultBGTexture);
	SafeRelease(ResultLine);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	static int PressCount = 0;
	TRANSITION *Transition = GetTransition();

	if (IsPlaying(BGM_Result) == false)
	{
		PlaySound(BGM_Result, E_DS8_FLAG_LOOP, true);
	}

	// ��ʑJ��
	if (Transition->FadeInOver == true && ResultSelect.InYes == true)
	{
		if (ResultSelect.Phase == RestartCheck)
		{
			ReInitGame(true);
			SetGameStage(Stage_Game);
		}
		else if (ResultSelect.Phase == ToTitleCheck)
		{
			ReInitGame(true);
			SetGameStage(Stage_Title);
		}

		return;
	}

	// �X�R�A�v�Z������Ȃ����
	if (ResultSelect.Phase != Calculating)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || GetKeyboardTrigger(DIK_Z) ||
			IsButtonTriggered(0, BUTTON_B))
		{
			switch (ResultSelect.Phase)
			{
			case ScoreCalOver:
				PlaySound(SE_Menu, E_DS8_FLAG_NONE, true);
				ResultSelect.Pos.y = RestartPos_Y;
				ResultSelect.Phase = Restart;
				break;
			case Restart:
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				ResultSelect.Phase = RestartCheck;
				ResultSelect.Pos.x = NoPos_X;
				ResultSelect.Pos.y = YesNoPos_Y;
				break;
			case RestartCheck:
				if (ResultSelect.InYes == true)
				{
					PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
					SetTransition();
				}
				else
				{
					PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
					ResultSelect.Phase = Restart;
					ResultSelect.Pos.x = ScreenCenter_X;
					ResultSelect.Pos.y = RestartPos_Y;
				}
				break;
			case ToTitle:
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				ResultSelect.Phase = ToTitleCheck;
				ResultSelect.Pos.x = NoPos_X;
				ResultSelect.Pos.y = YesNoPos_Y;
				break;
			case ToTitleCheck:
				if (ResultSelect.InYes == true)
				{
					PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
					SetTransition();
				}
				else
				{
					PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
					ResultSelect.Phase = ToTitle;
					ResultSelect.Pos.x = ScreenCenter_X;
					ResultSelect.Pos.y = ToTitlePos_Y;
				}
				break;
			default:
				break;
			}
		}

		if (ResultSelect.Phase != ScoreCalOver)
		{
			if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_DOWN) ||
				IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, BUTTON_DOWN))
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				switch (ResultSelect.Phase)
				{
				case Restart:
					ResultSelect.Phase = ToTitle;
					ResultSelect.Pos.y = ToTitlePos_Y;
					break;
				case ToTitle:
					ResultSelect.Phase = Restart;
					ResultSelect.Pos.y = RestartPos_Y;
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
					if (ResultSelect.Phase == Restart)
					{
						ResultSelect.Phase = ToTitle;
						ResultSelect.Pos.y = ToTitlePos_Y;
					}
					else if (ResultSelect.Phase == ToTitle)
					{
						ResultSelect.Phase = Restart;
						ResultSelect.Pos.y = RestartPos_Y;
					}
				}
			}

			// �v���X�J�E���g������
			if (GetKeyboardRelease(DIK_UP) || GetKeyboardRelease(DIK_DOWN) ||
				IsButtonReleased(0, BUTTON_UP) || IsButtonReleased(0, BUTTON_DOWN))
			{
				PressCount = 0;
			}
		}

		if (ResultSelect.Phase == RestartCheck || ResultSelect.Phase == ToTitleCheck)
		{
			if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				ResultSelect.Pos.x = YesPos_X;
				ResultSelect.InYes = true;
			}
			else if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				ResultSelect.Pos.x = NoPos_X;
				ResultSelect.InYes = false;
			}
		}

		// �e�N�X�`���X�V
		SetResultVertex();
	}

	// �J�E���g���Z
	Count++;

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPDIRECT3DTEXTURE9 BlackScreenTexture = GetUITexture(BlackScreen);
	LPDIRECT3DTEXTURE9 ResultSelectTexture = GetUITexture(SelectBox);
	LPD3DXFONT Result = GetFont(FontSize_54);
	int HPScore = 0;
	int PowerScore = 0;
	int BombScore = 0;
	// �l�̌ܓ��v�Z�p
	TCHAR RateRound[256];
	TCHAR Str[256];
	RECT TextRect = { RectPos_X, RectPos_Y, RectPos_X + 800, RectPos_Y + RectHeight };
	D3DXVECTOR2 LinePoint[] = { D3DXVECTOR2(LinePos_X, LinePos_Y), D3DXVECTOR2(LinePos_X + 800, LinePos_Y) };
	PLAYER *Player = GetPlayer(0);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, ResultBGTexture);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, ResultBGVertexWk, sizeof(VERTEX_2D));

	// ��{�X�R�A
	if (Count >= 60)
	{
		if (Count == 60)
		{
			PlaySound(SE_ShowScore, E_DS8_FLAG_NONE, true);
		}
		// �e�L�X�g�`��
		Result->DrawText(NULL, "��{�X�R�A", -1, &TextRect, DT_VCENTER | DT_LEFT, WHITE(255));

		wsprintf(Str, _T("%d\n"), Player->Score);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_RIGHT, BLACK(255));
	}

	// HP Rate
	if (Count >= 120)
	{
		if (Count == 120)
		{
			PlaySound(SE_ShowScore, E_DS8_FLAG_NONE, true);
		}
		// ��`�ʒu����
		TextRect.top += RectHeight;
		TextRect.bottom += RectHeight;
		// �����_��ʂ܂Ŏl�̌ܓ�
		sprintf(RateRound, "%.1f", Player->HP);
		_stprintf(Str, _T("HP Rate�F%s%%"), RateRound);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_LEFT, WHITE(255));

		// �X�R�A�v�Z
		HPScore = (int)(atof(RateRound) * 50000);
		wsprintf(Str, _T("%d\n"), HPScore);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_RIGHT, BLACK(255));
	}

	// Power Rate
	if (Count >= 180)
	{
		if (Count == 180)
		{
			PlaySound(SE_ShowScore, E_DS8_FLAG_NONE, true);
		}
		// ��`�ʒu����
		TextRect.top += RectHeight;
		TextRect.bottom += RectHeight;
		// �����_��ʂ܂Ŏl�̌ܓ�
		sprintf(RateRound, "%.2f", Player->Power);
		_stprintf(Str, _T("Power Rate�F%s / 3.00"), RateRound);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_LEFT, WHITE(255));

		// �X�R�A�v�Z
		PowerScore = (int)(atof(RateRound) * 1500000);
		wsprintf(Str, _T("%d\n"), PowerScore);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_RIGHT, BLACK(255));
	}

	// �{���X�R�A
	if (Count >= 240)
	{
		if (Count == 240)
		{
			PlaySound(SE_ShowScore, E_DS8_FLAG_NONE, true);
		}
		// ��`�ʒu����
		TextRect.top += RectHeight;
		TextRect.bottom += RectHeight;
		// �����_��ʂ܂Ŏl�̌ܓ�
		wsprintf(Str, _T("�c��{���F%d / 5\n"), Player->BombNum);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_LEFT, WHITE(255));

		// �X�R�A�v�Z
		BombScore = (Player->BombNum * 1000000);
		wsprintf(Str, _T("%d\n"), BombScore);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_RIGHT, BLACK(255));
	}

	// Total Score
	if (Count >= 300)
	{
		if (Count == 300)
		{
			PlaySound(SE_CalculateScore, E_DS8_FLAG_LOOP, false);
		}
		// ������`��
		ResultLine->SetAntialias(TRUE);
		ResultLine->SetWidth(5);
		ResultLine->Begin();
		ResultLine->Draw(LinePoint, 2, BLACK(255));
		ResultLine->End();

		// ��`�ʒu����
		TextRect.top += RectHeight;
		TextRect.bottom += RectHeight;
		Result->DrawText(NULL, "Total Score", -1, &TextRect, DT_VCENTER | DT_LEFT, WHITE(255));

		// �X�R�A�v�Z
		if (TotalScore < (Player->Score + HPScore + PowerScore + BombScore))
		{
			if (Count < 300)
			{
				TotalScore += 11;
			}
			else if (Count >= 300 && Count < 330)
			{
				TotalScore += 111;
			}
			else if (Count >= 330 && Count < 360)
			{
				TotalScore += 1111;
			}
			else if (Count >= 360 && Count < 390)
			{
				TotalScore += 11111;
			}
			else
			{
				TotalScore += 111111;
			}
		}
		// �v�Z�I��
		if (TotalScore >= (Player->Score + HPScore + PowerScore + BombScore))
		{
			TotalScore = Player->Score + HPScore + PowerScore + BombScore;
			if (ResultSelect.Phase == Calculating)
			{
				StopSound(SE_CalculateScore);
				ResultSelect.Phase = ScoreCalOver;
			}
		}
		wsprintf(Str, _T("%d\n"), TotalScore);
		Result->DrawText(NULL, Str, -1, &TextRect, DT_VCENTER | DT_RIGHT, BLACK(255));
	}

	if (ResultSelect.Phase != Calculating)
	{
		// ��`�ʒu����
		TextRect.top = PressAnyKeyPos_Y;
		TextRect.bottom = PressAnyKeyPos_Y + RectHeight;
		Result->DrawText(NULL, "Press Enter Key", -1, &TextRect, DT_VCENTER | DT_CENTER, BLACK(255));
	}

	// ���j���[��ʕ`��
	if (ResultSelect.Phase != Calculating && ResultSelect.Phase != ScoreCalOver)
	{
		Device->SetTexture(0, BlackScreenTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BlackScreenVertexWk, sizeof(VERTEX_2D));

		Device->SetTexture(0, ResultSelectTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, ResultSelect.VertexWk, sizeof(VERTEX_2D));

		if (ResultSelect.Phase == Restart || ResultSelect.Phase == ToTitle)
		{
			TextRect.top = RestartPos_Y;
			TextRect.bottom = RestartPos_Y + Texture_SelectBox_Height;
			if (ResultSelect.Phase == Restart)
			{
				Result->DrawText(NULL, "�ŏ�����", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Result->DrawText(NULL, "�ŏ�����", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			TextRect.top = ToTitlePos_Y;
			TextRect.bottom = ToTitlePos_Y + Texture_SelectBox_Height;
			if (ResultSelect.Phase == ToTitle)
			{
				Result->DrawText(NULL, "�^�C�g����", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Result->DrawText(NULL, "�^�C�g����", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
		}
		else if (ResultSelect.Phase == RestartCheck || ResultSelect.Phase == ToTitleCheck)
		{
			TextRect.top = AskSentencePos_Y;
			TextRect.bottom = AskSentencePos_Y + 100;
			if (ResultSelect.Phase == RestartCheck)
			{
				Result->DrawText(NULL, "�ŏ������낵�ł����H", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			else if (ResultSelect.Phase == ToTitleCheck)
			{
				Result->DrawText(NULL, "�^�C�g���ɖ߂�܂����H", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			TextRect.top = YesNoPos_Y;
			TextRect.bottom = YesNoPos_Y + 100;
			TextRect.left = 0;
			TextRect.right = ScreenCenter_X;
			if (ResultSelect.InYes == true)
			{
				Result->DrawText(NULL, "�͂�", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
			else
			{
				Result->DrawText(NULL, "�͂�", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			TextRect.left = ScreenCenter_X;
			TextRect.right = Screen_Width;
			if (ResultSelect.InYes == true)
			{
				Result->DrawText(NULL, "������", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
			}
			else
			{
				Result->DrawText(NULL, "������", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
			}
		}
	}

	return;
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeResultVertex(void)
{
	// ���_���W�̐ݒ�	
	SetResultTexture();

	// rhw�̐ݒ�
	ResultBGVertexWk[0].rhw = 1.0f;
	ResultBGVertexWk[1].rhw = 1.0f;
	ResultBGVertexWk[2].rhw = 1.0f;
	ResultBGVertexWk[3].rhw = 1.0f;

	BlackScreenVertexWk[0].rhw = 1.0f;
	BlackScreenVertexWk[1].rhw = 1.0f;
	BlackScreenVertexWk[2].rhw = 1.0f;
	BlackScreenVertexWk[3].rhw = 1.0f;

	ResultSelect.VertexWk[0].rhw = 1.0f;
	ResultSelect.VertexWk[1].rhw = 1.0f;
	ResultSelect.VertexWk[2].rhw = 1.0f;
	ResultSelect.VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	ResultBGVertexWk[0].diffuse = WHITE(255);
	ResultBGVertexWk[1].diffuse = WHITE(255);
	ResultBGVertexWk[2].diffuse = WHITE(255);
	ResultBGVertexWk[3].diffuse = WHITE(255);

	BlackScreenVertexWk[0].diffuse = WHITE(200);
	BlackScreenVertexWk[1].diffuse = WHITE(200);
	BlackScreenVertexWk[2].diffuse = WHITE(200);
	BlackScreenVertexWk[3].diffuse = WHITE(200);

	ResultSelect.VertexWk[0].diffuse = WHITE(255);
	ResultSelect.VertexWk[1].diffuse = WHITE(255);
	ResultSelect.VertexWk[2].diffuse = WHITE(255);
	ResultSelect.VertexWk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetResultVertex();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetResultTexture(void)
{
	ResultBGVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	ResultBGVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	ResultBGVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	ResultBGVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	BlackScreenVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	BlackScreenVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	BlackScreenVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	BlackScreenVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	ResultSelect.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	ResultSelect.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	ResultSelect.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	ResultSelect.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetResultVertex(void)
{
	ResultBGVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ResultBGVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	ResultBGVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	ResultBGVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	BlackScreenVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BlackScreenVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	BlackScreenVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	BlackScreenVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	ResultSelect.VertexWk[0].vtx = D3DXVECTOR3(ResultSelect.Pos.x - Texture_SelectBox_Width / 2, (float)ResultSelect.Pos.y, 0.0f);
	ResultSelect.VertexWk[1].vtx = D3DXVECTOR3(ResultSelect.Pos.x + Texture_SelectBox_Width / 2, (float)ResultSelect.Pos.y, 0.0f);
	ResultSelect.VertexWk[2].vtx = D3DXVECTOR3(ResultSelect.Pos.x - Texture_SelectBox_Width / 2, (float)ResultSelect.Pos.y + Texture_SelectBox_Height, 0.0f);
	ResultSelect.VertexWk[3].vtx = D3DXVECTOR3(ResultSelect.Pos.x + Texture_SelectBox_Width / 2, (float)ResultSelect.Pos.y + Texture_SelectBox_Height, 0.0f);

	return;
}
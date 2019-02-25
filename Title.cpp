//=============================================================================
//
// �^�C�g����ʏ��� [Title.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "Title.h"
#include "BackGround.h"
#include "FrontBG.h"
#include "Transition.h"
#include "Sound.h"
#include "Input.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �w�i�X�N���[�����x
#define TitleBGSpeed (0.0003f)
// �_�X�N���[�����x
#define CloudSpeed (0.0002f)
// �e�L�X�g��`�̍���
#define RectHeight (100)
// �e�L�X�g�\���̍��W
#define GameTitlePos_X (87)
#define GameTitlePos_Y (200)
#define GameStartPos_Y (384)
#define BossPraticePos_Y (448)
#define ExitGamePos_Y (512)
#define EasyModePos_Y (384)
#define NormalModePos_Y (448)
#define HardModePos_Y (512)
#define AskSentencePos_Y (200)
#define YesNoPos_Y (450)
#define YesPos_X (256)
#define NoPos_X (768)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeTitleVertex(void);
// ���_���W�̐ݒ�
void SetTitleVertex(void);
// �e�N�X�`�����W�̐ݒ�
void SetTitleTexture(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		TitleBGTexture = NULL;
LPDIRECT3DTEXTURE9		GameTitleTexture = NULL;
LPDIRECT3DTEXTURE9		LightTexture = NULL;
LPDIRECT3DTEXTURE9		CloudTexture = NULL;
// ���_���i�[���[�N
VERTEX_2D				BlackScreenVertexWk[Num_Vertex];
// �^�C�g���w�i�\����
TITLE					TitleBG;
// �^�C�g���\����
TITLE					GameTitle;
// ���\����
TITLE					Light;
// �_�\����
TITLE					Cloud;
// �^�C�g���I����
static SELECT			TitleSelect;
// �Q�[����Փx��I�����Ă���t���O
bool					InSelectDifficulty;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(int InitState)
{
	TitleBG.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	GameTitle.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Light.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Cloud.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	TitleSelect.Pos = D3DXVECTOR3(NoPos_X, YesNoPos_Y, 0.0f);
	TitleSelect.Phase = GameStart;
	TitleSelect.InYes = false;
	InSelectDifficulty = false;

	// ���_���̍쐬
	MakeTitleVertex();

	if (InitState == FirstInit)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_TitleBG, &TitleBGTexture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GameTitle, &GameTitleTexture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Light, &LightTexture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_FrontBG, &CloudTexture, "Title") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	SafeRelease(TitleBGTexture);
	SafeRelease(GameTitleTexture);
	SafeRelease(LightTexture);
	SafeRelease(CloudTexture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	int i = 0, j = 0;
	static int PressCount = 0;
	TRANSITION *Transition = GetTransition();

	// �^�C�g��BGM
	PlaySound(BGM_Title, E_DS8_FLAG_LOOP, false);

	// �w�i�X�N���[��
	TitleBG.Pos.x += TitleBGSpeed;
	Cloud.Pos.x += CloudSpeed;

	// ��ʑJ��
	if (Transition->FadeInOver == true && (TitleSelect.Phase == StartYes || TitleSelect.Phase == PraticeYes))
	{
		ReInitGame(false);
		InSelectDifficulty = false;
		SetGameStage(Stage_SelectChar);
	}

	// Enter��������A�X�e�[�W��؂�ւ���
	if (Transition->FadeInOver != true)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || GetKeyboardTrigger(DIK_Z) ||
			IsButtonTriggered(0, BUTTON_B))
		{
			if (TitleSelect.Phase != ExitCheck)
			{
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				if (TitleSelect.Phase == GameStart)
				{
					InSelectDifficulty = true;
					TitleSelect.Phase = NormalMode;
				}
				else if (TitleSelect.Phase == BossPratice)
				{
					TitleSelect.Phase = PraticeYes;
					SetTransition();
				}
				else if (TitleSelect.Phase == ExitGame)
				{
					TitleSelect.Phase = ExitCheck;
				}
				else if (TitleSelect.Phase == EasyMode)
				{
					SetGameDiffuculty(Easy);
					TitleSelect.Phase = StartYes;
					SetTransition();
				}
				else if (TitleSelect.Phase == NormalMode)
				{
					SetGameDiffuculty(Normal);
					TitleSelect.Phase = StartYes;
					SetTransition();
				}
				else if (TitleSelect.Phase == HardMode)
				{
					SetGameDiffuculty(Hard);
					TitleSelect.Phase = StartYes;
					SetTransition();
				}
			}
			else if (TitleSelect.Phase == ExitCheck)
			{
				PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
				if (TitleSelect.InYes == true)
				{
					TitleSelect.Phase = ExitYes;
				}
				else
				{
					TitleSelect.Phase = ExitGame;
				}
			}
		}
	}

	if (TitleSelect.Phase != ExitCheck)
	{
		// ���ɑI�������ړ�
		if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
		{
			// ���ʉ�
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			if (TitleSelect.Phase == GameStart)
			{
				TitleSelect.Phase = BossPratice;
			}
			else if (TitleSelect.Phase == BossPratice)
			{
				TitleSelect.Phase = ExitGame;
			}
			else if (TitleSelect.Phase == ExitGame)
			{
				TitleSelect.Phase = GameStart;
			}
			else if (TitleSelect.Phase == EasyMode)
			{
				TitleSelect.Phase = NormalMode;
			}
			else if (TitleSelect.Phase == NormalMode)
			{
				TitleSelect.Phase = HardMode;
			}
			else if (TitleSelect.Phase == HardMode)
			{
				TitleSelect.Phase = EasyMode;
			}
		}
		// ��ɑI�������ړ�
		else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
		{
			// ���ʉ�
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			if (TitleSelect.Phase == GameStart)
			{
				TitleSelect.Phase = ExitGame;
			}
			else if (TitleSelect.Phase == BossPratice)
			{
				TitleSelect.Phase = GameStart;
			}
			else if (TitleSelect.Phase == ExitGame)
			{
				TitleSelect.Phase = BossPratice;
			}
			else if (TitleSelect.Phase == EasyMode)
			{
				TitleSelect.Phase = HardMode;
			}
			else if (TitleSelect.Phase == NormalMode)
			{
				TitleSelect.Phase = EasyMode;
			}
			else if (TitleSelect.Phase == HardMode)
			{
				TitleSelect.Phase = NormalMode;
			}
		}

		// �v���X�J�E���g������
		if (GetKeyboardRelease(DIK_UP) || GetKeyboardRelease(DIK_DOWN) ||
			IsButtonReleased(0, BUTTON_UP) || IsButtonReleased(0, BUTTON_DOWN))
		{
			PressCount = 0;
		}

		// �I�������[�v
		if (GetKeyboardRepeat(DIK_DOWN) || IsButtonRepeat(0, BUTTON_DOWN))
		{
			PressCount++;
			if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				if (InSelectDifficulty == false)
				{
					TitleSelect.Phase += 2;
					TitleSelect.Phase = TitleSelect.Phase % 6;
				}
				else
				{
					TitleSelect.Phase++;
					if (TitleSelect.Phase > HardMode)
					{
						TitleSelect.Phase = EasyMode;
					}
				}
			}
		}
		else if (GetKeyboardRepeat(DIK_UP) || IsButtonRepeat(0, BUTTON_UP))
		{
			PressCount++;
			if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
			{
				PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
				if (InSelectDifficulty == false)
				{
					TitleSelect.Phase -= 2;
					if (TitleSelect.Phase < GameStart)
					{
						TitleSelect.Phase = ExitGame;
					}
				}
				else
				{
					TitleSelect.Phase--;
					if (TitleSelect.Phase < EasyMode)
					{
						TitleSelect.Phase = HardMode;
					}
				}
			}
		}

		// �O�̉�ʂɖ߂�
		if ((GetKeyboardTrigger(DIK_X) || IsButtonTriggered(0, BUTTON_A)) && InSelectDifficulty == true)
		{
			PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
			InSelectDifficulty = false;
			TitleSelect.Phase = GameStart;
		}
	}
	else if (TitleSelect.Phase == ExitCheck)
	{
		// �I�����ړ�
		if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			TitleSelect.InYes = false;
			TitleSelect.Pos.x = NoPos_X;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			TitleSelect.InYes = true;
			TitleSelect.Pos.x = YesPos_X;
		}
	}

	// �e�N�X�`���X�V
	SetTitleTexture();
	SetTitleVertex();

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPD3DXFONT Option = GetFont(FontSize_54);
	RECT TextRect = { 0, 0, Screen_Width, RectHeight };

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, TitleBGTexture);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TitleBG.VertexWk, sizeof(VERTEX_2D));

	// �^�C�g��
	Device->SetTexture(0, GameTitleTexture);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, GameTitle.VertexWk, sizeof(VERTEX_2D));

	// ��
	Device->SetTexture(0, LightTexture);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Light.VertexWk, sizeof(VERTEX_2D));
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �_
	Device->SetTexture(0, CloudTexture);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Cloud.VertexWk, sizeof(VERTEX_2D));
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (InSelectDifficulty == false)
	{

		// Game Start
		TextRect.top = GameStartPos_Y;
		TextRect.bottom = GameStartPos_Y + RectHeight;
		if (TitleSelect.Phase == GameStart)
		{
			Option->DrawText(NULL, "Game Start", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "Game Start", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(128));
		}

		// Boss Mode
		TextRect.top = BossPraticePos_Y;
		TextRect.bottom = BossPraticePos_Y + RectHeight;
		if (TitleSelect.Phase == BossPratice)
		{
			Option->DrawText(NULL, "Boss Mode", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "Boss Mode", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(128));
		}

		// Exit Game
		TextRect.top = ExitGamePos_Y;
		TextRect.bottom = ExitGamePos_Y + RectHeight;
		if (TitleSelect.Phase == ExitGame)
		{
			Option->DrawText(NULL, "Exit Game", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "Exit Game", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(128));
		}
	}
	// GameDifficulty
	else
	{
		// Easy
		TextRect.top = EasyModePos_Y;
		TextRect.bottom = EasyModePos_Y + RectHeight;
		if (TitleSelect.Phase == EasyMode)
		{
			Option->DrawText(NULL, "Easy", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "Easy", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(128));
		}

		// Normal
		TextRect.top = NormalModePos_Y;
		TextRect.bottom = NormalModePos_Y + RectHeight;
		if (TitleSelect.Phase == NormalMode)
		{
			Option->DrawText(NULL, "Normal", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "Normal", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(128));
		}

		// Hard
		TextRect.top = HardModePos_Y;
		TextRect.bottom = HardModePos_Y + RectHeight;
		if (TitleSelect.Phase == HardMode)
		{
			Option->DrawText(NULL, "Hard", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Option->DrawText(NULL, "Hard", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(128));
		}
	}

	return;
}

//=============================================================================
// ���j���[��ʕ`�揈��
//=============================================================================
void DrawTitleMenu(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPDIRECT3DTEXTURE9 BlackScreenTexture = GetUITexture(BlackScreen);
	LPDIRECT3DTEXTURE9 TitleSelectTexture = GetUITexture(SelectBox);
	LPD3DXFONT Title = GetFont(FontSize_54);
	LPD3DXFONT Option = GetFont(FontSize_40);
	RECT TextRect = { 0, 0, Screen_Width, RectHeight };

	if (TitleSelect.Phase == ExitCheck)
	{
		// ������ʕ`��
		Device->SetTexture(0, BlackScreenTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BlackScreenVertexWk, sizeof(VERTEX_2D));

		// �I��g�`��
		Device->SetTexture(0, TitleSelectTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TitleSelect.VertexWk, sizeof(VERTEX_2D));

		TextRect.top = AskSentencePos_Y;
		TextRect.bottom = AskSentencePos_Y + RectHeight;
		Title->DrawText(NULL, "�{���ɃQ�[�����I�����܂����H", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));

		TextRect.top = YesNoPos_Y;
		TextRect.bottom = YesNoPos_Y + RectHeight;
		TextRect.left = 0;
		TextRect.right = ScreenCenter_X;
		if (TitleSelect.InYes == true)
		{
			Title->DrawText(NULL, "�͂�", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Title->DrawText(NULL, "�͂�", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		TextRect.left = ScreenCenter_X;
		TextRect.right = Screen_Width;
		if (TitleSelect.InYes == true)
		{
			Title->DrawText(NULL, "������", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		else
		{
			Title->DrawText(NULL, "������", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeTitleVertex(void)
{
	// �e�N�X�`�����W�̐ݒ�
	SetTitleTexture();

	// rhw�̐ݒ�
	TitleBG.VertexWk[0].rhw = 1.0f;
	TitleBG.VertexWk[1].rhw = 1.0f;
	TitleBG.VertexWk[2].rhw = 1.0f;
	TitleBG.VertexWk[3].rhw = 1.0f;

	GameTitle.VertexWk[0].rhw = 1.0f;
	GameTitle.VertexWk[1].rhw = 1.0f;
	GameTitle.VertexWk[2].rhw = 1.0f;
	GameTitle.VertexWk[3].rhw = 1.0f;

	Light.VertexWk[0].rhw = 1.0f;
	Light.VertexWk[1].rhw = 1.0f;
	Light.VertexWk[2].rhw = 1.0f;
	Light.VertexWk[3].rhw = 1.0f;

	Cloud.VertexWk[0].rhw = 1.0f;
	Cloud.VertexWk[1].rhw = 1.0f;
	Cloud.VertexWk[2].rhw = 1.0f;
	Cloud.VertexWk[3].rhw = 1.0f;

	BlackScreenVertexWk[0].rhw = 1.0f;
	BlackScreenVertexWk[1].rhw = 1.0f;
	BlackScreenVertexWk[2].rhw = 1.0f;
	BlackScreenVertexWk[3].rhw = 1.0f;

	TitleSelect.VertexWk[0].rhw = 1.0f;
	TitleSelect.VertexWk[1].rhw = 1.0f;
	TitleSelect.VertexWk[2].rhw = 1.0f;
	TitleSelect.VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	TitleBG.VertexWk[0].diffuse = WHITE(255);
	TitleBG.VertexWk[1].diffuse = WHITE(255);
	TitleBG.VertexWk[2].diffuse = WHITE(255);
	TitleBG.VertexWk[3].diffuse = WHITE(255);

	GameTitle.VertexWk[0].diffuse = WHITE(255);
	GameTitle.VertexWk[1].diffuse = WHITE(255);
	GameTitle.VertexWk[2].diffuse = WHITE(255);
	GameTitle.VertexWk[3].diffuse = WHITE(255);

	Light.VertexWk[0].diffuse = WHITE(128);
	Light.VertexWk[1].diffuse = WHITE(128);
	Light.VertexWk[2].diffuse = WHITE(128);
	Light.VertexWk[3].diffuse = WHITE(128);

	Cloud.VertexWk[0].diffuse = WHITE(128);
	Cloud.VertexWk[1].diffuse = WHITE(128);
	Cloud.VertexWk[2].diffuse = WHITE(128);
	Cloud.VertexWk[3].diffuse = WHITE(128);

	BlackScreenVertexWk[0].diffuse = WHITE(200);
	BlackScreenVertexWk[1].diffuse = WHITE(200);
	BlackScreenVertexWk[2].diffuse = WHITE(200);
	BlackScreenVertexWk[3].diffuse = WHITE(200);

	TitleSelect.VertexWk[0].diffuse = WHITE(255);
	TitleSelect.VertexWk[1].diffuse = WHITE(255);
	TitleSelect.VertexWk[2].diffuse = WHITE(255);
	TitleSelect.VertexWk[3].diffuse = WHITE(255);

	// ���_���W�̐ݒ�
	SetTitleVertex();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTitleTexture(void)
{
	float BGRate_X = (float)Screen_Width / (float)Texture_TitleBG_Width;
	float CloudRate_X = (float)Screen_Width / (float)Texture_FrontBG_Width;

	TitleBG.VertexWk[0].tex = D3DXVECTOR2(TitleBG.Pos.x, 0.0f);
	TitleBG.VertexWk[1].tex = D3DXVECTOR2(TitleBG.Pos.x + BGRate_X, 0.0f);
	TitleBG.VertexWk[2].tex = D3DXVECTOR2(TitleBG.Pos.x, 1.0f);
	TitleBG.VertexWk[3].tex = D3DXVECTOR2(TitleBG.Pos.x + BGRate_X, 1.0f);

	GameTitle.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	GameTitle.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	GameTitle.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	GameTitle.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	Light.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Light.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Light.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Light.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	Cloud.VertexWk[0].tex = D3DXVECTOR2(TitleBG.Pos.x, 0.0f);
	Cloud.VertexWk[1].tex = D3DXVECTOR2(TitleBG.Pos.x + CloudRate_X, 0.0f);
	Cloud.VertexWk[2].tex = D3DXVECTOR2(TitleBG.Pos.x, 1.0f);
	Cloud.VertexWk[3].tex = D3DXVECTOR2(TitleBG.Pos.x + CloudRate_X, 1.0f);

	BlackScreenVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	BlackScreenVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	BlackScreenVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	BlackScreenVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	TitleSelect.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	TitleSelect.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	TitleSelect.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	TitleSelect.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetTitleVertex(void)
{
	static bool ZoomPositive_X = false;
	static bool ZoomPositive_Y = false;
	static float ZoomRate_X = 1.0f;
	static float ZoomRate_Y = 1.0f;

	// �h�����̌v�Z
	if (ZoomRate_X >= 1.5f && ZoomPositive_X == true)
	{
		ZoomPositive_X = false;
		ZoomPositive_Y = false;
	}

	if (ZoomRate_Y >= 1.5f && ZoomPositive_Y == true)
	{
		ZoomPositive_X = true;
		ZoomPositive_Y = false;
	}
	else if (ZoomRate_Y <= 1.0f && ZoomPositive_Y == false)
	{
		ZoomPositive_Y = true;
	}

	if (ZoomPositive_Y == false)
	{
		if (ZoomPositive_X == true)
		{
			ZoomRate_X += 0.001f;
		}
		else
		{
			ZoomRate_X -= 0.001f;
		}
	}

	if (ZoomPositive_X == false)
	{
		if (ZoomPositive_Y == true)
		{
			ZoomRate_Y += 0.001f;
		}
		else
		{
			ZoomRate_Y -= 0.001f;
		}
	}

	TitleBG.VertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	TitleBG.VertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	TitleBG.VertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	TitleBG.VertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	GameTitle.VertexWk[0].vtx = D3DXVECTOR3(GameTitlePos_X, GameTitlePos_Y, 0.0f);
	GameTitle.VertexWk[1].vtx = D3DXVECTOR3(GameTitlePos_X + Texture_GameTitle_Width, GameTitlePos_Y, 0.0f);
	GameTitle.VertexWk[2].vtx = D3DXVECTOR3(GameTitlePos_X, GameTitlePos_Y + Texture_GameTitle_Height, 0.0f);
	GameTitle.VertexWk[3].vtx = D3DXVECTOR3(GameTitlePos_X + Texture_GameTitle_Width, GameTitlePos_Y + Texture_GameTitle_Height, 0.0f);

	Light.VertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Light.VertexWk[1].vtx = D3DXVECTOR3(Screen_Width * ZoomRate_X, 0.0f, 0.0f);
	Light.VertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height * ZoomRate_Y, 0.0f);
	Light.VertexWk[3].vtx = D3DXVECTOR3(Screen_Width * ZoomRate_X, Screen_Height * ZoomRate_Y, 0.0f);

	Cloud.VertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Cloud.VertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	Cloud.VertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	Cloud.VertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	BlackScreenVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BlackScreenVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	BlackScreenVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	BlackScreenVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	TitleSelect.VertexWk[0].vtx = D3DXVECTOR3(TitleSelect.Pos.x - Texture_SelectBox_Width / 2, (float)TitleSelect.Pos.y, 0.0f);
	TitleSelect.VertexWk[1].vtx = D3DXVECTOR3(TitleSelect.Pos.x + Texture_SelectBox_Width / 2, (float)TitleSelect.Pos.y, 0.0f);
	TitleSelect.VertexWk[2].vtx = D3DXVECTOR3(TitleSelect.Pos.x - Texture_SelectBox_Width / 2, (float)TitleSelect.Pos.y + Texture_SelectBox_Height, 0.0f);
	TitleSelect.VertexWk[3].vtx = D3DXVECTOR3(TitleSelect.Pos.x + Texture_SelectBox_Width / 2, (float)TitleSelect.Pos.y + Texture_SelectBox_Height, 0.0f);

	return;
}

//=============================================================================
// �^�C�g���I�����̏����擾����
//=============================================================================
SELECT *GetTitleSelect(void)
{
	return &TitleSelect;
}
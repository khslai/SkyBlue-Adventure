//=============================================================================
//
// �`���[�g���A������ [Tutorial.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "Tutorial.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "Sound.h"
#include "Input.h"
#include "BackGround.h"
#include "Transition.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �w�i�X�N���[�����x
#define BGSpeed (0.001f)
// �e�L�X�g��`�̍���
#define RectHeight (100)
// �e�L�X�g�\���̍��W
#define HelpPos_Y (550)
#define ExitTutorialPos_Y (650)
#define AskSentencePos_Y (200)
#define YesNoPos_Y (450)
#define YesPos_X (256)
#define NoPos_X (768)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeTutorialVertex(void);
// ���_���W�̐ݒ�
void SetTutorialVertex(void);
// �e�N�X�`�����W�̐ݒ�
void SetTutorialTexture(void);
// �v���C���[�ƃG�l�~�[�̐ݒu
void SetTutorial(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// ���_���i�[���[�N
static VERTEX_2D BlackScreenVertexWk[Num_Vertex];
VERTEX_2D TutorialVertexWk[Num_Vertex];
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 TutorialBGTexture = NULL;
LPDIRECT3DTEXTURE9 Tutorial1_Keyboard_Texture = NULL;
LPDIRECT3DTEXTURE9 Tutorial1_GamePad_Texture = NULL;
LPDIRECT3DTEXTURE9 Tutorial2Texture = NULL;
// �w�i�\����
static BACKGROUND TutorialBG;
// �`���[�g���A���I�����\����
static SELECT TutorialSelect;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTutorial(int InitState)
{
	TutorialBG.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	TutorialSelect.Pos = D3DXVECTOR3(NoPos_X, YesNoPos_Y, 0.0f);
	TutorialSelect.Phase = ShowTutorial1;
	TutorialSelect.InYes = false;

	MakeTutorialVertex();

	if (InitState == FirstInit)
	{
		if (SafeLoad(Texture_BG_Day, &TutorialBGTexture, "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Tutorial1_KeyBoard, &Tutorial1_Keyboard_Texture, "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Tutorial1_GamePad, &Tutorial1_GamePad_Texture, "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Tutorial2, &Tutorial2Texture, "BackGround") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTutorial(void)
{
	SafeRelease(TutorialBGTexture);
	SafeRelease(Tutorial1_Keyboard_Texture);
	SafeRelease(Tutorial1_GamePad_Texture);
	SafeRelease(Tutorial2Texture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTutorial(void)
{
	static int RespawnCount = 0;
	static bool TutorialFirst = true;
	ENEMY *Enemy = GetEnemy(0);
	BARRAGE *Barrage = GetBarrage(0);
	TRANSITION *Transition = GetTransition();

	if (TutorialSelect.Phase == ShowTutorial1)
	{
		PlaySound(BGM_Tutorial, E_DS8_FLAG_LOOP, false);
	}
	else if (TutorialSelect.Phase == SetTutorialStart)
	{
		// �`���[�g���A���ݒu
		SetTutorial();
		TutorialSelect.Phase = SetTutorialOver;
	}
	else if (TutorialSelect.Phase == SetTutorialOver)
	{
		// �G�l�~�[�����A���߂Đݒu
		if (Enemy->Exist == false)
		{
			RespawnCount++;
			if (RespawnCount == 60)
			{
				RespawnCount = 0;
				SetEnemy(0);
				Enemy->Pos = D3DXVECTOR3(Screen_Width, ScreenCenter_Y, 0.0f);
				Enemy->HP = 100.0f;
				Enemy->MovePattern = 98;
				Enemy->BarrageType = 0;
				Enemy->ShotTime = 60;
				Barrage->Phase = BarrageOver;
			}
		}
	}

	// �w�i�X�N���[��
	if (TutorialSelect.Phase != ExitTutorial)
	{
		TutorialBG.Pos.x += BGSpeed;
	}

	// �`���[�g���A���I����
	if ((GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_START)) && TutorialSelect.Phase == SetTutorialOver)
	{
		PlaySound(SE_Menu, E_DS8_FLAG_NONE, true);
		TutorialSelect.Phase = ExitTutorial;
	}

	// ������x�`���[�g���A������
	if ((GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_X)) && TutorialSelect.Phase == SetTutorialOver)
	{
		PlaySound(SE_Menu, E_DS8_FLAG_NONE, true);
		TutorialSelect.Phase = ShowTutorial1;
	}

	// �I�����ړ�
	if (TutorialSelect.Phase == ExitTutorial)
	{
		if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			TutorialSelect.InYes = false;
			TutorialSelect.Pos.x = NoPos_X;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
		{
			PlaySound(SE_SelectMove, E_DS8_FLAG_NONE, true);
			TutorialSelect.InYes = true;
			TutorialSelect.Pos.x = YesPos_X;
		}
	}

	// �I���m�F
	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || GetKeyboardTrigger(DIK_Z) ||
		IsButtonTriggered(0, BUTTON_B))
	{
		if (TutorialSelect.Phase == ShowTutorial1)
		{
			PlaySound(SE_TutorialEnter, E_DS8_FLAG_NONE, true);
			TutorialSelect.Phase = ShowTutorial2;
		}
		else if (TutorialSelect.Phase == ShowTutorial2)
		{
			PlaySound(SE_TutorialEnter, E_DS8_FLAG_NONE, true);
			if (TutorialFirst == true)
			{
				TutorialSelect.Phase = SetTutorialStart;
				TutorialFirst = false;
			}
			else
			{
				TutorialSelect.Phase = SetTutorialOver;
			}
		}
		else if (TutorialSelect.Phase == ExitTutorial)
		{
			if (TutorialSelect.InYes == true)
			{
				PlaySound(SE_Determine_Yes, E_DS8_FLAG_NONE, true);
				SetTransition();
			}
			else
			{
				// ����
				PlaySound(SE_Determine_No, E_DS8_FLAG_NONE, true);
				TutorialSelect.Phase = SetTutorialOver;
			}
		}
	}

	// �`���[�g���A���I��
	if (Transition->FadeInOver == true && TutorialSelect.Phase == ExitTutorial && TutorialSelect.InYes == true)
	{
		TutorialFirst = true;
		// �ď�����
		ReInitGame(true);
		SetGameStage(Stage_Game);
	}

	// �e�N�X�`���X�V
	SetTutorialTexture();
	SetTutorialVertex();

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, TutorialBGTexture);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TutorialBG.VertexWk, sizeof(VERTEX_2D));

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTutorialMenu(void)
{
	bool GamePadUsed = GetGamePadUsed();
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPDIRECT3DTEXTURE9 BlackScreenTexture = GetUITexture(BlackScreen);
	LPDIRECT3DTEXTURE9 TutorialSelectTexture = GetUITexture(SelectBox);
	LPD3DXFONT Title = GetFont(FontSize_54);
	LPD3DXFONT Option = GetFont(FontSize_40);
	PLAYER *Player = GetPlayer(0);
	RECT TextRect = { 0, 0, Screen_Width, RectHeight };

	if (TutorialSelect.Phase == ShowTutorial1)
	{
		if (GamePadUsed == false)
		{
			Device->SetTexture(0, Tutorial1_Keyboard_Texture);
		}
		else if (GamePadUsed == true)
		{
			Device->SetTexture(0, Tutorial1_GamePad_Texture);
		}
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TutorialVertexWk, sizeof(VERTEX_2D));
	}
	else if (TutorialSelect.Phase == ShowTutorial2)
	{
		Device->SetTexture(0, Tutorial2Texture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TutorialVertexWk, sizeof(VERTEX_2D));
	}
	else if (TutorialSelect.Phase == SetTutorialOver)
	{
		if (GamePadUsed == false)
		{
			TextRect.top = HelpPos_Y;
			TextRect.bottom = HelpPos_Y + RectHeight;
			Title->DrawText(NULL, "�w���v�F(H)������", -1, &TextRect, DT_CENTER | DT_VCENTER, BLACK(255));

			TextRect.top = ExitTutorialPos_Y;
			TextRect.bottom = ExitTutorialPos_Y + RectHeight;
			Title->DrawText(NULL, "�`���[�g���A���I���F(P)������", -1, &TextRect, DT_CENTER | DT_VCENTER, BLACK(255));
		}
		else if (GamePadUsed == true)
		{
			TextRect.top = HelpPos_Y;
			TextRect.bottom = HelpPos_Y + RectHeight;
			Title->DrawText(NULL, "�w���v�F(X)������", -1, &TextRect, DT_CENTER | DT_VCENTER, BLACK(255));

			TextRect.top = ExitTutorialPos_Y;
			TextRect.bottom = ExitTutorialPos_Y + RectHeight;
			Title->DrawText(NULL, "�`���[�g���A���I���F(START)������", -1, &TextRect, DT_CENTER | DT_VCENTER, BLACK(255));
		}
	}
	else if (TutorialSelect.Phase == ExitTutorial)
	{
		// ������ʕ`��
		Device->SetTexture(0, BlackScreenTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BlackScreenVertexWk, sizeof(VERTEX_2D));

		// �I��g�`��
		Device->SetTexture(0, TutorialSelectTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TutorialSelect.VertexWk, sizeof(VERTEX_2D));

		TextRect.top = AskSentencePos_Y;
		TextRect.bottom = AskSentencePos_Y + RectHeight;
		Title->DrawText(NULL, "�`���[�g���A�����I�����܂����H", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));

		TextRect.top = YesNoPos_Y;
		TextRect.bottom = YesNoPos_Y + RectHeight;
		TextRect.left = 0;
		TextRect.right = ScreenCenter_X;
		if (TutorialSelect.InYes == true)
		{
			Title->DrawText(NULL, "�͂�", -1, &TextRect, DT_CENTER | DT_VCENTER, BLUE(255));
		}
		else
		{
			Title->DrawText(NULL, "�͂�", -1, &TextRect, DT_CENTER | DT_VCENTER, WHITE(255));
		}
		TextRect.left = ScreenCenter_X;
		TextRect.right = Screen_Width;
		if (TutorialSelect.InYes == true)
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
// �v���C���[�ƃG�l�~�[�̐ݒu
//=============================================================================
void SetTutorial(void)
{
	ENEMY *Enemy = GetEnemy(0);

	SetPlayer();

	SetEnemy(0);
	Enemy->Pos = D3DXVECTOR3(Screen_Width, ScreenCenter_Y, 0.0f);
	Enemy->HP = 100.0f;
	Enemy->MovePattern = 98;
	Enemy->BarrageType = 0;
	Enemy->ShotTime = 60;

	return;
}

//=============================================================================
// �`���[�g���A���̏�Ԃ��擾����
//=============================================================================
int GetTutorialPhase(void)
{
	return TutorialSelect.Phase;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeTutorialVertex(void)
{
	// ���_���W�̐ݒ�	
	SetTutorialVertex();

	// rhw�̐ݒ�
	TutorialBG.VertexWk[0].rhw = 1.0f;
	TutorialBG.VertexWk[1].rhw = 1.0f;
	TutorialBG.VertexWk[2].rhw = 1.0f;
	TutorialBG.VertexWk[3].rhw = 1.0f;

	TutorialVertexWk[0].rhw = 1.0f;
	TutorialVertexWk[1].rhw = 1.0f;
	TutorialVertexWk[2].rhw = 1.0f;
	TutorialVertexWk[3].rhw = 1.0f;

	BlackScreenVertexWk[0].rhw = 1.0f;
	BlackScreenVertexWk[1].rhw = 1.0f;
	BlackScreenVertexWk[2].rhw = 1.0f;
	BlackScreenVertexWk[3].rhw = 1.0f;

	TutorialSelect.VertexWk[0].rhw = 1.0f;
	TutorialSelect.VertexWk[1].rhw = 1.0f;
	TutorialSelect.VertexWk[2].rhw = 1.0f;
	TutorialSelect.VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	TutorialBG.VertexWk[0].diffuse = WHITE(255);
	TutorialBG.VertexWk[1].diffuse = WHITE(255);
	TutorialBG.VertexWk[2].diffuse = WHITE(255);
	TutorialBG.VertexWk[3].diffuse = WHITE(255);

	TutorialVertexWk[0].diffuse = WHITE(255);
	TutorialVertexWk[1].diffuse = WHITE(255);
	TutorialVertexWk[2].diffuse = WHITE(255);
	TutorialVertexWk[3].diffuse = WHITE(255);

	BlackScreenVertexWk[0].diffuse = WHITE(200);
	BlackScreenVertexWk[1].diffuse = WHITE(200);
	BlackScreenVertexWk[2].diffuse = WHITE(200);
	BlackScreenVertexWk[3].diffuse = WHITE(200);

	TutorialSelect.VertexWk[0].diffuse = WHITE(255);
	TutorialSelect.VertexWk[1].diffuse = WHITE(255);
	TutorialSelect.VertexWk[2].diffuse = WHITE(255);
	TutorialSelect.VertexWk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetTutorialTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTutorialTexture(void)
{
	float BGRate_X = (float)Screen_Width / (float)Texture_BG_Width;

	TutorialBG.VertexWk[0].tex = D3DXVECTOR2(TutorialBG.Pos.x, 0.0f);
	TutorialBG.VertexWk[1].tex = D3DXVECTOR2(TutorialBG.Pos.x + BGRate_X, 0.0f);
	TutorialBG.VertexWk[2].tex = D3DXVECTOR2(TutorialBG.Pos.x, 1.0f);
	TutorialBG.VertexWk[3].tex = D3DXVECTOR2(TutorialBG.Pos.x + BGRate_X, 1.0f);

	TutorialVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	TutorialVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	TutorialVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	TutorialVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	BlackScreenVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	BlackScreenVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	BlackScreenVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	BlackScreenVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	TutorialSelect.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	TutorialSelect.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	TutorialSelect.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	TutorialSelect.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetTutorialVertex(void)
{
	TutorialBG.VertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	TutorialBG.VertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	TutorialBG.VertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	TutorialBG.VertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	TutorialVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	TutorialVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	TutorialVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	TutorialVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	BlackScreenVertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BlackScreenVertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	BlackScreenVertexWk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	BlackScreenVertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	TutorialSelect.VertexWk[0].vtx = D3DXVECTOR3(TutorialSelect.Pos.x - Texture_SelectBox_Width / 2, (float)TutorialSelect.Pos.y, 0.0f);
	TutorialSelect.VertexWk[1].vtx = D3DXVECTOR3(TutorialSelect.Pos.x + Texture_SelectBox_Width / 2, (float)TutorialSelect.Pos.y, 0.0f);
	TutorialSelect.VertexWk[2].vtx = D3DXVECTOR3(TutorialSelect.Pos.x - Texture_SelectBox_Width / 2, (float)TutorialSelect.Pos.y + Texture_SelectBox_Height, 0.0f);
	TutorialSelect.VertexWk[3].vtx = D3DXVECTOR3(TutorialSelect.Pos.x + Texture_SelectBox_Width / 2, (float)TutorialSelect.Pos.y + Texture_SelectBox_Height, 0.0f);

	return;
}
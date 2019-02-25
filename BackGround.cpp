//=============================================================================
//
// �w�i���� [BackGround.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "BackGround.h"
#include "Gauge.h"
#include "Score.h"
#include "Player.h"
#include "Boss.h"
#include "Transition.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �w�i�X�N���[�����x
#define BGSpeed				(0.001f)					
// �[���ɕς�鎞��
#define ToAfternoonCount	(5300)						
// ��ɕς�鎞��
#define ToNightCount		(8800)						
// ��̉~�̓_�̐��i�~�̉𑜓x�j
#define CircleResolution	(100)						
// �Q�[���N���A���o�̉~�̐�
#define GameClearCircleNum	(GameClearVertexDevide + 1)	


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// �w�i�\��
HRESULT MakeBGVertex(int State);
void SetBGTexture(int State);
void SetBGVertex(int State);
void SetBGDiffuse(int State, int Alpha);
// �Q�[���X�^�[�g�\��
HRESULT MakeGameStartVertex(void);
void SetGameStartTexture(void);
void SetGameStartVertex(void);
void DrawGameStart(void);
// �Q�[���N���A�\��
HRESULT MakeGameClearVertex(void);
void SetGameClearTexture(void);
void SetGameClearVertex(void);
void DrawGameClear(void);
// �~��`��
void DrawCircle(D3DXVECTOR2 Center, float Radius, D3DCOLOR Color, bool HalfCircle, bool DrawFromRight, bool DrawSlowly);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 BGTexture[3] = { NULL };
LPDIRECT3DTEXTURE9 GameStartTexture = NULL;
LPDIRECT3DTEXTURE9 GameClearTexture = NULL;
// �w�i�\����
BACKGROUND BackGround[3];
// �Q�[���X�^�[�g�\����
BACKGROUND GameStart_SkyBlue;
BACKGROUND GameStart_White;
// �Q�[���N���A�\����
GAMECLEAR GameClear;
// �w�i�̃A���t�@�l
static int Alpha = 255;
// �~�̃A���t�@�l
static int CircleAlpha = 255;
// �w�i�̏��
static int State = Day;
// �w�i�ϊ����鎞�A�t�F�C�h�A�E�g�̃t���O
static bool BGFadeout = false;
// �~�̕`��I���t���O
static bool DrawCircleOver = false;
// �~�̊g��I���t���O
static bool ExtendOver = false;
// �~�̊g�唼�a
static float ExtendRadius = 0.0f;
// �w�i�h��鉉�o�̈ړ����W
static D3DXVECTOR2 ShakePos = D3DXVECTOR2(0.0f, 0.0f);

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBG(int InitState)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	BackGround[Day].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BackGround[Afternoon].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BackGround[Night].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	GameStart_SkyBlue.Pos = D3DXVECTOR3(Screen_Width, 334.0f, 0.0f);
	GameStart_White.Pos = D3DXVECTOR3(0.0f, 434.0f, 0.0f);

	GameClear.Alpha = 0;
	GameClear.FadeInOver = false;
	GameClear.AnimationOver = false;

	Alpha = 255;
	CircleAlpha = 255;
	State = Day;
	BGFadeout = false;
	DrawCircleOver = false;
	ExtendOver = false;
	ExtendRadius = 150.0f;
	ShakePos = D3DXVECTOR2(0.0f, 0.0f);

	// ���_���̍쐬
	MakeBGVertex(Day);
	MakeBGVertex(Afternoon);
	MakeBGVertex(Night);
	MakeGameStartVertex();
	MakeGameClearVertex();

	if (InitState == FirstInit)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_BG_Day, &BGTexture[Day], "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BG_Afternoon, &BGTexture[Afternoon], "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BG_Night, &BGTexture[Night], "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GameStart, &GameStartTexture, "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GameClear, &GameClearTexture, "BackGround") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBG(void)
{
	SafeRelease(BGTexture[Day]);
	SafeRelease(BGTexture[Afternoon]);
	SafeRelease(BGTexture[Night]);
	SafeRelease(GameStartTexture);
	SafeRelease(GameClearTexture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBG(void)
{
	int i = 0, j = 0;
	int GameCount = GetGameCount();
	static bool BossBGMLoop = false;
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();
	TRANSITION *Transition = GetTransition();

	// �w�i�ړ�
	BackGround[Day].Pos.x += BGSpeed;
	BackGround[Afternoon].Pos.x += BGSpeed;
	BackGround[Night].Pos.x += BGSpeed;

	// BGM�𗬂�
	if (Boss->Exist == true && Boss->Phase != MiddleBoss_Phase1 && Boss->Phase != MiddleBoss_LastPhase)
	{
		if (BossBGMLoop == false)
		{
			StopSound(BGM_Battle);
			PlaySound(BGM_Boss_Start, E_DS8_FLAG_NONE, false);
			BossBGMLoop = true;
		}
		else if (BossBGMLoop == true && IsPlaying(BGM_Boss_Start) == false)
		{
			PlaySound(BGM_Boss_Loop, E_DS8_FLAG_LOOP, false);
		}
	}
	else if (Boss->State != LastBossOver)
	{
		BossBGMLoop = false;
		PlaySound(BGM_Battle, E_DS8_FLAG_LOOP, false);
	}

	ShakePos = D3DXVECTOR2(0.0f, 0.0f);

	// �e�N�X�`�����W��ݒ�
	SetBGTexture(Day);
	SetBGTexture(Afternoon);
	SetBGTexture(Night);

	// �w�i�ς��
	if (GameCount == ToAfternoonCount || GameCount == ToNightCount)
	{
		BGFadeout = true;
	}

	// �w�i�h���
	if (BackGround[Day].InShake == true || BackGround[Afternoon].InShake == true || BackGround[Night].InShake == true)
	{
		ShakePos.x = (float)(rand() % (30 + 1) - 15);
		ShakePos.y = (float)(rand() % (30 + 1) - 15);
	}

	// �t�F�C�h�A�E�g�J�n
	if (BGFadeout == true)
	{
		// �T�E���h�t�F�C�h�A�E�g
		if (GameCount >= ToNightCount)
		{
			SoundFadeOut(BGM_Battle);
		}

		// �����l�ς��
		if (Alpha > 0)
		{
			Alpha--;
		}
		else
		{
			State++;
			if (State > Night)
			{
				State = Day;
			}
			Alpha = 255;
			BGFadeout = false;
		}
	}

	// ���ˌ��̐ݒ�
	switch (State)
	{
	case Day:
		SetBGDiffuse(Day, Alpha);
		SetBGDiffuse(Afternoon, 255 - Alpha);
		SetBGDiffuse(Night, 0);
		break;
	case Afternoon:
		SetBGDiffuse(Day, 0);
		SetBGDiffuse(Afternoon, Alpha);
		SetBGDiffuse(Night, 255 - Alpha);
		break;
	case Night:
		SetBGDiffuse(Day, 255 - Alpha);
		SetBGDiffuse(Afternoon, 0);
		SetBGDiffuse(Night, Alpha);
		break;
	default:
		break;
	}

	// ���_�̌v�Z����
	SetBGVertex(Day);
	SetBGVertex(Afternoon);
	SetBGVertex(Night);

	// Game Start�v�Z
	if (ExtendOver == true && GameCount < 420)
	{
		GameStart_SkyBlue.Pos.x -= 30.0f;
		if (GameStart_SkyBlue.Pos.x <= ScreenCenter_X)
		{
			GameStart_SkyBlue.Pos.x = ScreenCenter_X;
		}
		GameStart_White.Pos.x += 30.0f;
		if (GameStart_White.Pos.x >= ScreenCenter_X)
		{
			GameStart_White.Pos.x = ScreenCenter_X;
		}
		SetGameStartVertex();
	}
	// Game Clear�v�Z
	else if (GameCount >= GameClearCount && Player->ClearStandBy == true)
	{
		// �����t�F�C�h�C��
		GameClear.Alpha += 5;
		if (GameClear.Alpha >= 255)
		{
			GameClear.Alpha = 255;
			GameClear.FadeInOver = true;
		}
		for (i = 0; i < GameClearVertexDevide; i++)
		{
			GameClear.Pixel[i][0].diffuse = WHITE(GameClear.Alpha);
			GameClear.Pixel[i][1].diffuse = WHITE(GameClear.Alpha);
			GameClear.Pixel[i][2].diffuse = WHITE(GameClear.Alpha);
			GameClear.Pixel[i][3].diffuse = WHITE(GameClear.Alpha);

			// �X�e�[�W�J��
			if (GameClear.AnimationOver == true)
			{
				SetTransition();
				if (Transition->FadeInOver == true)
				{
					ReInitSound();
					SetGameStage(Stage_Result);
				}
			}

			// �����̐F��ς��
			if (((GameClear.Pixel[i][0].vtx.x + GameClear.Pixel[i][1].vtx.x) / 2) <= Player->Pos.x)
			{
				PlaySound(SE_GameClear_02, E_DS8_FLAG_NONE, false);
				if (GameClear.Pixel[i][0].vtx.x >= 212.0f  && GameClear.Pixel[i][0].vtx.x < 312.0f)
				{
					GameClear.Pixel[i][0].diffuse = RED(GameClear.Alpha);
					GameClear.Pixel[i][1].diffuse = ORANGE(GameClear.Alpha);
					GameClear.Pixel[i][2].diffuse = RED(GameClear.Alpha);
					GameClear.Pixel[i][3].diffuse = ORANGE(GameClear.Alpha);
				}
				else if (GameClear.Pixel[i][0].vtx.x >= 312.0f  && GameClear.Pixel[i][0].vtx.x < 412.0f)
				{
					GameClear.Pixel[i][0].diffuse = ORANGE(GameClear.Alpha);
					GameClear.Pixel[i][1].diffuse = YELLOW(GameClear.Alpha);
					GameClear.Pixel[i][2].diffuse = ORANGE(GameClear.Alpha);
					GameClear.Pixel[i][3].diffuse = YELLOW(GameClear.Alpha);
				}
				else if (GameClear.Pixel[i][0].vtx.x >= 412.0f  && GameClear.Pixel[i][0].vtx.x < 512.0f)
				{
					GameClear.Pixel[i][0].diffuse = YELLOW(GameClear.Alpha);
					GameClear.Pixel[i][1].diffuse = GREEN(GameClear.Alpha);
					GameClear.Pixel[i][2].diffuse = YELLOW(GameClear.Alpha);
					GameClear.Pixel[i][3].diffuse = GREEN(GameClear.Alpha);
				}
				else if (GameClear.Pixel[i][0].vtx.x >= 512.0f  && GameClear.Pixel[i][0].vtx.x < 612.0f)
				{
					GameClear.Pixel[i][0].diffuse = GREEN(GameClear.Alpha);
					GameClear.Pixel[i][1].diffuse = BLUE(GameClear.Alpha);
					GameClear.Pixel[i][2].diffuse = GREEN(GameClear.Alpha);
					GameClear.Pixel[i][3].diffuse = BLUE(GameClear.Alpha);
				}
				else if (GameClear.Pixel[i][0].vtx.x >= 612.0f  && GameClear.Pixel[i][0].vtx.x < 712.0f)
				{
					GameClear.Pixel[i][0].diffuse = BLUE(GameClear.Alpha);
					GameClear.Pixel[i][1].diffuse = PURPLE(GameClear.Alpha);
					GameClear.Pixel[i][2].diffuse = BLUE(GameClear.Alpha);
					GameClear.Pixel[i][3].diffuse = PURPLE(GameClear.Alpha);
				}
				else if (GameClear.Pixel[i][0].vtx.x >= 712.0f  && GameClear.Pixel[i][0].vtx.x < 812.0f)
				{
					GameClear.Pixel[i][0].diffuse = PURPLE(GameClear.Alpha);
					GameClear.Pixel[i][1].diffuse = SKYBLUE(GameClear.Alpha);
					GameClear.Pixel[i][2].diffuse = PURPLE(GameClear.Alpha);
					GameClear.Pixel[i][3].diffuse = SKYBLUE(GameClear.Alpha);
				}
			}
		}
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	PLAYER *Player = GetPlayer(0);

	int GameCount = GetGameCount();

	// ������
	if (GameCount == 0)
	{
		Alpha = 255;
		ExtendRadius = 150.0f;
	}

	// �w�i�`��
	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);
	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, BGTexture[Day]);
	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BackGround[Day].VertexWk, sizeof(VERTEX_2D));

	Device->SetTexture(0, BGTexture[Afternoon]);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BackGround[Afternoon].VertexWk, sizeof(VERTEX_2D));

	Device->SetTexture(0, BGTexture[Night]);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BackGround[Night].VertexWk, sizeof(VERTEX_2D));

	// �v���C���[��ԕ`��
	DrawInformation();

	// �Q�[���X�^�[�g�A�j���`��
	if (GameCount >= GameStartCount && GameCount < 420)
	{
		DrawGameStart();
	}

	if (GameCount >= GameClearCount && Player->ClearStandBy == true)
	{
		DrawGameClear();
	}

	return;
}

//=============================================================================
// �v���C���[��Ԃ�`��
//=============================================================================
void DrawInformation(void)
{
	LPD3DXFONT Information = GetFont(FontSize_30);
	LPD3DXLINE InformationLine = GetLine();
	RECT rect = { 0, 0, 100, 40 };
	D3DXVECTOR2 LinePoint[] = { D3DXVECTOR2(0, MoveRange_Y), D3DXVECTOR2(Screen_Width, MoveRange_Y) };

	// �e�L�X�g��`��
	Information->DrawText(NULL, "HP", -1, &rect, DT_CENTER | DT_VCENTER, D3DCOLOR_RGBA(0, 255, 0, 255));
	rect.top += 40;
	rect.bottom += 40;
	Information->DrawText(NULL, "Power", -1, &rect, DT_CENTER | DT_VCENTER, D3DCOLOR_RGBA(135, 206, 235, 255));
	rect.top = ScorePos_Y;
	rect.bottom = ScorePos_Y + Texture_Score_Height;
	rect.left = ScorePos_X - Texture_Score_Width_Total - 100;
	rect.right = ScorePos_X - Texture_Score_Width_Total;
	Information->DrawText(NULL, "Score", -1, &rect, DT_CENTER | DT_VCENTER, D3DCOLOR_RGBA(0, 0, 255, 255));
	rect.top = PlayerBombSymbol_Pos_Y;
	rect.bottom = PlayerBombSymbol_Pos_Y + Texture_BombSymbol_Height;
	Information->DrawText(NULL, "Bomb", -1, &rect, DT_CENTER | DT_VCENTER, D3DCOLOR_RGBA(179, 179, 0, 255));

	// ������`��
	InformationLine->SetWidth(5);
	InformationLine->Begin();
	InformationLine->Draw(LinePoint, 2, D3DCOLOR_RGBA(255, 255, 255, 255));
	InformationLine->End();

	return;
}

//=============================================================================
// �Q�[���X�^�[�g�A�j����`��
//=============================================================================
void DrawGameStart(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int GameCount = GetGameCount();
	int GameStage = GetGameStage();
	static bool SoundPlayed = false;

	// ������
	if (GameCount == GameStartCount)
	{
		SoundPlayed = false;
	}

	// ��Ɖ��̔��~�`��`��
	DrawCircle(D3DXVECTOR2(ScreenCenter_X, ScreenCenter_Y), 150.0f, WHITE(255), true, false, true);
	DrawCircle(D3DXVECTOR2(ScreenCenter_X, ScreenCenter_Y), 150.0f, SKYBLUE(255), true, true, true);

	// �`���I���A�L����~�`�`��
	if (DrawCircleOver == true && GameCount < GameStartCount + 90)
	{
		if (SoundPlayed == false)
		{
			PlaySound(SE_GameStart_01, E_DS8_FLAG_NONE, false);
			SoundPlayed = true;
		}
		DrawCircle(D3DXVECTOR2(ScreenCenter_X, ScreenCenter_Y), ExtendRadius, WHITE(CircleAlpha), false, false, false);
		// PAUSE��Ԃŕ`���Ȃ�
		if (GameStage == Stage_Game)
		{
			CircleAlpha -= 3;
			if (CircleAlpha <= 128)
			{
				CircleAlpha = 128;
			}
			ExtendRadius += 5.0f;
		}
	}
	// 90�J�E���g��A�g��I��
	else if (GameCount >= GameStartCount + 90 && ExtendOver == false)
	{
		ExtendOver = true;
		ExtendRadius = 0.0f;
		SoundPlayed = false;
	}
	// �g��I���A���̉摜�`��
	if (ExtendOver == true)
	{
		if (SoundPlayed == false)
		{
			PlaySound(SE_GameStart_02, E_DS8_FLAG_NONE, false);
			SoundPlayed = true;
		}
		// ���~�`�`���A�������g��
		DrawCircle(D3DXVECTOR2(ScreenCenter_X - 200, ScreenCenter_Y), ExtendRadius, SKYBLUE(255), true, false, false);
		DrawCircle(D3DXVECTOR2(ScreenCenter_X + 200, ScreenCenter_Y), ExtendRadius, WHITE(255), true, true, false);
		// PAUSE��Ԃŕ`���Ȃ�
		if (GameStage == Stage_Game)
		{
			ExtendRadius += 5.0f;
		}

		// GameStart�e�N�X�`��
		Device->SetTexture(0, GameStartTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, GameStart_SkyBlue.VertexWk, sizeof(VERTEX_2D));

		Device->SetTexture(0, GameStartTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, GameStart_White.VertexWk, sizeof(VERTEX_2D));
	}

	return;
}

//=============================================================================
// �Q�[���N���A�A�j����`��
//=============================================================================
void DrawGameClear(void)
{
	int i = 0;
	static int Alpha[GameClearCircleNum] = { 255, 255, 255, 255, 255, 255, 255 };
	static float Radius[GameClearCircleNum] = { 0.0f };
	LPDIRECT3DDEVICE9 Device = GetDevice();
	PLAYER *Player = GetPlayer(0);

	// �e�N�X�`���`��
	Device->SetTexture(0, GameClearTexture);
	for (i = 0; i < GameClearVertexDevide; i++)
	{
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, GameClear.Pixel[i], sizeof(VERTEX_2D));
	}

	if (Player->Pos.x < 212.0f)
	{
		for (i = 0; i < GameClearCircleNum; i++)
		{
			Alpha[i] = 255;
			Radius[i] = 0.0f;
		}
	}

	// �~��`��
	if (Player->Pos.x >= 212.0f)
	{
		Radius[0] += 3.0f;
		Alpha[0] -= 2;
		if (Alpha[0] <= 0)
		{
			Alpha[0] = 0;
		}
		DrawCircle(D3DXVECTOR2(212.0f, Player->Pos.y), Radius[0], RED(Alpha[0]), false, false, false);
	}
	if (Player->Pos.x >= 312.0f)
	{
		Radius[1] += 3.0f;
		Alpha[1] -= 2;
		if (Alpha[1] <= 0)
		{
			Alpha[1] = 0;
		}
		DrawCircle(D3DXVECTOR2(312.0f, Player->Pos.y), Radius[1], ORANGE(Alpha[1]), false, false, false);
	}
	if (Player->Pos.x >= 412.0f)
	{
		Radius[2] += 3.0f;
		Alpha[2] -= 2;
		if (Alpha[2] <= 0)
		{
			Alpha[2] = 0;
		}
		DrawCircle(D3DXVECTOR2(412.0f, Player->Pos.y), Radius[2], YELLOW(Alpha[2]), false, false, false);
	}
	if (Player->Pos.x >= 512.0f)
	{
		Radius[3] += 3.0f;
		Alpha[3] -= 2;
		if (Alpha[3] <= 0)
		{
			Alpha[3] = 0;
		}
		DrawCircle(D3DXVECTOR2(512.0f, Player->Pos.y), Radius[3], GREEN(Alpha[3]), false, false, false);
	}
	if (Player->Pos.x >= 612.0f)
	{
		Radius[4] += 3.0f;
		Alpha[4] -= 2;
		if (Alpha[4] <= 0)
		{
			Alpha[4] = 0;
		}
		DrawCircle(D3DXVECTOR2(612.0f, Player->Pos.y), Radius[4], BLUE(Alpha[4]), false, false, false);
	}
	if (Player->Pos.x >= 712.0f)
	{
		Radius[5] += 3.0f;
		Alpha[5] -= 2;
		if (Alpha[5] <= 0)
		{
			Alpha[5] = 0;
		}
		DrawCircle(D3DXVECTOR2(712.0f, Player->Pos.y), Radius[5], PURPLE(Alpha[5]), false, false, false);
	}
	if (Player->Pos.x >= 812.0f)
	{
		Radius[6] += 3.0f;
		Alpha[6] -= 2;
		if (Alpha[6] <= 0)
		{
			Alpha[6] = 0;
			GameClear.AnimationOver = true;
		}
		DrawCircle(D3DXVECTOR2(812.0f, Player->Pos.y), Radius[6], SKYBLUE(Alpha[6]), false, false, false);
	}

	return;
}


//=============================================================================
// �~��`��
//=============================================================================
void DrawCircle(D3DXVECTOR2 Center, float Radius, D3DCOLOR Color, bool HalfCircle, bool DrawFromRight, bool DrawSlowly)
{
	// �~�̓_�̐�
	const int CirclePointNum = 40;
	int i = 0;
	static int DrawPointNum = 0;
	float Theta = 0.0f;
	float WedgeAngle = 0.0f;
	LPD3DXLINE Line = GetLine();
	D3DXVECTOR2 LinePoint[CirclePointNum + 1] = { D3DXVECTOR2(0.0f, 0.0f) };

	// 360�x / �_�̐� = ��̓_�ړ��̊p�x
	if (HalfCircle == false)
	{
		// �~�`
		WedgeAngle = (float)((2 * PI) / (CirclePointNum));
	}
	else
	{
		// ���~�`
		WedgeAngle = (float)((2 * PI / 2) / (CirclePointNum));
	}

	for (i = 0; i <= CirclePointNum; i++)
	{
		// ���̊p�x
		Theta = i * WedgeAngle;

		// �~�̊e�_�̍��W���v�Z
		if (DrawFromRight == false)
		{
			// ���S�̍�����`��
			LinePoint[i].x = (float)(Center.x - Radius * cos(Theta));
			LinePoint[i].y = (float)(Center.y - Radius * sin(Theta));
		}
		else
		{
			// ���S�̉E����`��
			LinePoint[i].x = (float)(Center.x + Radius * cos(Theta));
			LinePoint[i].y = (float)(Center.y + Radius * sin(Theta));
		}
	}

	// �~��`��
	Line->SetWidth(8);
	Line->Begin();
	if (DrawSlowly == false || DrawCircleOver == true)
	{
		Line->Draw(LinePoint, CirclePointNum + 1, Color);
	}
	else
	{
		Line->Draw(LinePoint, DrawPointNum, Color);
	}
	Line->End();

	// �������`��̌v�Z
	if (DrawPointNum < CirclePointNum + 1 && DrawSlowly == true)
	{
		DrawPointNum++;
	}
	else
	{
		DrawCircleOver = true;
		DrawPointNum = 0;
	}

	return;
}

//=============================================================================
// �w�i���h��鉉�o�̐ݒu
//=============================================================================
void SetBGShake(bool InShake)
{
	BackGround[Day].InShake = InShake;
	BackGround[Afternoon].InShake = InShake;
	BackGround[Night].InShake = InShake;
	return;
}

//=============================================================================
// �w�i�̏�Ԃ��擾����
//=============================================================================
int GetBGState(void)
{
	return State;
}

//=============================================================================
// �w�i�̃t�F�C�h�A�E�g��Ԃ��擾����
//=============================================================================
bool GetBGFadeout(void)
{
	return BGFadeout;
}

//=============================================================================
// �Q�[���N���A�̏�Ԃ��擾����
//=============================================================================
GAMECLEAR *GetGameClear(void)
{
	return &GameClear;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeBGVertex(int State)
{
	// ���_���W�̐ݒ�	
	SetBGVertex(State);

	// rhw�̐ݒ�
	BackGround[State].VertexWk[0].rhw = 1.0f;
	BackGround[State].VertexWk[1].rhw = 1.0f;
	BackGround[State].VertexWk[2].rhw = 1.0f;
	BackGround[State].VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	SetBGDiffuse(State, Alpha);

	// �e�N�X�`�����W�̐ݒ�
	SetBGTexture(State);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetBGTexture(int State)
{
	float BGRate_X = (float)Screen_Width / (float)Texture_BG_Width;

	BackGround[State].VertexWk[0].tex = D3DXVECTOR2(BackGround[State].Pos.x, 0.0f);
	BackGround[State].VertexWk[1].tex = D3DXVECTOR2(BackGround[State].Pos.x + BGRate_X, 0.0f);
	BackGround[State].VertexWk[2].tex = D3DXVECTOR2(BackGround[State].Pos.x, 1.0f);
	BackGround[State].VertexWk[3].tex = D3DXVECTOR2(BackGround[State].Pos.x + BGRate_X, 1.0f);

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetBGVertex(int State)
{
	BackGround[State].VertexWk[0].vtx = D3DXVECTOR3(0 + ShakePos.x, 0 + ShakePos.y, 0);
	BackGround[State].VertexWk[1].vtx = D3DXVECTOR3(Screen_Width + ShakePos.x, 0 + ShakePos.y, 0);
	BackGround[State].VertexWk[2].vtx = D3DXVECTOR3(0 + ShakePos.x, Screen_Height + ShakePos.y, 0);
	BackGround[State].VertexWk[3].vtx = D3DXVECTOR3(Screen_Width + ShakePos.x, Screen_Height + ShakePos.y, 0);

	return;
}

//=============================================================================
// ���ˌ��̐ݒ�
//=============================================================================
void SetBGDiffuse(int State, int Alpha)
{
	BackGround[State].VertexWk[0].diffuse = WHITE(Alpha);
	BackGround[State].VertexWk[1].diffuse = WHITE(Alpha);
	BackGround[State].VertexWk[2].diffuse = WHITE(Alpha);
	BackGround[State].VertexWk[3].diffuse = WHITE(Alpha);

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeGameStartVertex(void)
{
	// ���_���W�̐ݒ�	
	SetGameStartVertex();

	// rhw�̐ݒ�
	GameStart_SkyBlue.VertexWk[0].rhw = 1.0f;
	GameStart_SkyBlue.VertexWk[1].rhw = 1.0f;
	GameStart_SkyBlue.VertexWk[2].rhw = 1.0f;
	GameStart_SkyBlue.VertexWk[3].rhw = 1.0f;

	GameStart_White.VertexWk[0].rhw = 1.0f;
	GameStart_White.VertexWk[1].rhw = 1.0f;
	GameStart_White.VertexWk[2].rhw = 1.0f;
	GameStart_White.VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	GameStart_SkyBlue.VertexWk[0].diffuse = SKYBLUE(255);
	GameStart_SkyBlue.VertexWk[1].diffuse = SKYBLUE(255);
	GameStart_SkyBlue.VertexWk[2].diffuse = SKYBLUE(255);
	GameStart_SkyBlue.VertexWk[3].diffuse = SKYBLUE(255);

	GameStart_White.VertexWk[0].diffuse = WHITE(255);
	GameStart_White.VertexWk[1].diffuse = WHITE(255);
	GameStart_White.VertexWk[2].diffuse = WHITE(255);
	GameStart_White.VertexWk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetGameStartTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetGameStartTexture(void)
{
	// ����
	GameStart_SkyBlue.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	GameStart_SkyBlue.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	GameStart_SkyBlue.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	GameStart_SkyBlue.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ����
	GameStart_White.VertexWk[0].tex = D3DXVECTOR2(1.0f, 1.0f);
	GameStart_White.VertexWk[1].tex = D3DXVECTOR2(0.0f, 1.0f);
	GameStart_White.VertexWk[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	GameStart_White.VertexWk[3].tex = D3DXVECTOR2(0.0f, 0.0f);

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetGameStartVertex(void)
{
	GameStart_SkyBlue.VertexWk[0].vtx = D3DXVECTOR3(GameStart_SkyBlue.Pos.x - Texture_GameStart_Width / 2, GameStart_SkyBlue.Pos.y - Texture_GameStart_Height / 2, 0);
	GameStart_SkyBlue.VertexWk[1].vtx = D3DXVECTOR3(GameStart_SkyBlue.Pos.x + Texture_GameStart_Width / 2, GameStart_SkyBlue.Pos.y - Texture_GameStart_Height / 2, 0);
	GameStart_SkyBlue.VertexWk[2].vtx = D3DXVECTOR3(GameStart_SkyBlue.Pos.x - Texture_GameStart_Width / 2, GameStart_SkyBlue.Pos.y + Texture_GameStart_Height / 2, 0);
	GameStart_SkyBlue.VertexWk[3].vtx = D3DXVECTOR3(GameStart_SkyBlue.Pos.x + Texture_GameStart_Width / 2, GameStart_SkyBlue.Pos.y + Texture_GameStart_Height / 2, 0);

	GameStart_White.VertexWk[0].vtx = D3DXVECTOR3(GameStart_White.Pos.x - Texture_GameStart_Width / 2, GameStart_White.Pos.y - Texture_GameStart_Height / 2, 0);
	GameStart_White.VertexWk[1].vtx = D3DXVECTOR3(GameStart_White.Pos.x + Texture_GameStart_Width / 2, GameStart_White.Pos.y - Texture_GameStart_Height / 2, 0);
	GameStart_White.VertexWk[2].vtx = D3DXVECTOR3(GameStart_White.Pos.x - Texture_GameStart_Width / 2, GameStart_White.Pos.y + Texture_GameStart_Height / 2, 0);
	GameStart_White.VertexWk[3].vtx = D3DXVECTOR3(GameStart_White.Pos.x + Texture_GameStart_Width / 2, GameStart_White.Pos.y + Texture_GameStart_Height / 2, 0);

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeGameClearVertex(void)
{
	int i = 0;

	// ���_���W�̐ݒ�	
	SetGameClearTexture();

	// rhw�̐ݒ�
	for (i = 0; i < GameClearVertexDevide; i++)
	{
		GameClear.Pixel[i][0].rhw = 1.0f;
		GameClear.Pixel[i][1].rhw = 1.0f;
		GameClear.Pixel[i][2].rhw = 1.0f;
		GameClear.Pixel[i][3].rhw = 1.0f;
	}

	// ���ˌ��̐ݒ�
	for (i = 0; i < GameClearVertexDevide; i++)
	{
		GameClear.Pixel[i][0].diffuse = WHITE(GameClear.Alpha);
		GameClear.Pixel[i][1].diffuse = WHITE(GameClear.Alpha);
		GameClear.Pixel[i][2].diffuse = WHITE(GameClear.Alpha);
		GameClear.Pixel[i][3].diffuse = WHITE(GameClear.Alpha);
	}

	// �e�N�X�`�����W�̐ݒ�
	SetGameClearVertex();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetGameClearTexture(void)
{
	int i = 0;
	float Size_X = 1.0f / GameClearVertexDevide;

	for (i = 0; i < GameClearVertexDevide; i++)
	{
		GameClear.Pixel[i][0].tex = D3DXVECTOR2(Size_X * i, 0.0f);
		GameClear.Pixel[i][1].tex = D3DXVECTOR2(Size_X * (i + 1), 0.0f);
		GameClear.Pixel[i][2].tex = D3DXVECTOR2(Size_X * i, 1.0f);
		GameClear.Pixel[i][3].tex = D3DXVECTOR2(Size_X * (i + 1), 1.0f);
	}

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetGameClearVertex(void)
{
	int i = 0;
	float Size_X = Texture_GameClear_Width / (float)GameClearVertexDevide;
	float Start_X = ScreenCenter_X - Texture_GameClear_Width / 2;
	float Start_Y = ScreenCenter_Y - Texture_GameClear_Height / 2;

	for (i = 0; i < GameClearVertexDevide; i++)
	{
		GameClear.Pixel[i][0].vtx.x = Start_X + Size_X * i;
		GameClear.Pixel[i][0].vtx.y = Start_Y;
		GameClear.Pixel[i][1].vtx.x = Start_X + Size_X * (i + 1);
		GameClear.Pixel[i][1].vtx.y = Start_Y;
		GameClear.Pixel[i][2].vtx.x = Start_X + Size_X * i;
		GameClear.Pixel[i][2].vtx.y = Start_Y + Texture_GameClear_Height;
		GameClear.Pixel[i][3].vtx.x = Start_X + Size_X * (i + 1);
		GameClear.Pixel[i][3].vtx.y = Start_Y + Texture_GameClear_Height;
	}

	return;
}

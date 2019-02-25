//=============================================================================
//
// Main���� [main.cpp]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#include "main.h"
#include "resource.h"
#include "File.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "Input.h"
#include "BackGround.h"
#include "FrontBG.h"
#include "Barrage.h"
#include "Score.h"
#include "Title.h"
#include "Gauge.h"
#include "DebugTest.h"
#include "Laser.h"
#include "CheckHit.h"
#include "Effect.h"
#include "Bomb.h"
#include "Boss.h"
#include "BossBarrage.h"
#include "Item.h"
#include "Sound.h"
#include "Pause.h"
#include "Result.h"
#include "GameOver.h"
#include "Tutorial.h"
#include "SelectChar.h"
#include "Transition.h"
#include "PraticeSelect.h"
#include "BossPratice.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME			_T("AppClass")			// �E�C���h�E�̃N���X��
#define WINDOW_NAME			_T("SkyBlue Adventure")	// �E�C���h�E�̃L���v�V������
#define FPS					(60)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9				g_pD3D = NULL;							// Direct3D�I�u�W�F�N�g
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;					// Device�I�u�W�F�N�g(�`��ɕK�v)
LPDIRECT3DTEXTURE9		UITexture[UITexture_Max] = { NULL };	// UI�e�N�X�`���ւ̃|�C���^
LPD3DXFONT				Font[Font_Max] = { NULL };				// �t�H���g�ւ̃|�C���^
LPD3DXLINE				Line = NULL;							// ���C���ւ̃|�C���^
int						GameStage = Stage_Title;				// �Q�[���X�e�[�W
int						GameCount = 0;							// �Q�[���J�E���g
int						Difficulty = Normal;					// ��Փx
int						FPSCount = 0;							// FPS�J�E���g

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// �Q�[���A�C�R��
	HICON MyIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	// ���Ԍv���p
	DWORD dwExecLastTime;	// �O���ʏ����̎���
	DWORD dwFPSLastTime;	// �O��FPG�v�Z�̎���
	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwFrameCount;		// ��b�ԉ��񏈗����� = FPS
	SELECT *TitleSelect = GetTitleSelect();
	BOSS *Boss = GetBoss();

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		MyIcon,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		MyIcon
	};
	HWND		hWnd;
	MSG			msg;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																			// �E�B���h�E�̍����W
		CW_USEDEFAULT,																			// �E�B���h�E�̏���W
		Screen_Width + GetSystemMetrics(SM_CXDLGFRAME) * 2,										// �E�B���h�E����
		Screen_Height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
		NULL,
		NULL,
		hInstance,
		NULL);

	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	if (FAILED(Init(hWnd, true)))
	{
		return -1;
	}

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;

	// �E�C���h�E�̕\��(Init()�̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (TitleSelect->Phase == ExitYes)
		{
			break;
		}

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// �V�X�e���������擾
			dwCurrentTime = timeGetTime();

			// 1�b���ƂɎ��s
			if ((dwCurrentTime - dwFPSLastTime) >= 1000)
			{

#ifdef _DEBUG
				// FPS�v�Z
				if (GameStage == Stage_Game)
				{
					FPSCount = dwFrameCount;
				}
#endif
				// FPS�𑪒肵��������ۑ�
				dwFPSLastTime = dwCurrentTime;
				// �J�E���g���N���A
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / FPS))	// 1/60�b���ƂɎ��s
			{
				// ��������������ۑ�
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				// �Q�[���J�E���g�����Z
				if (GameStage == Stage_Game && Boss->Exist == false)
				{
					GameCount++;
				}

				// �����񐔂̃J�E���g�����Z
				dwFrameCount++;
			}
		}
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
			// [ESC]�L�[�������ꂽ
		case VK_ESCAPE:
			//if (MessageBox(hWnd, "Do you want to Exit Game�H", "Game Exit", MB_YESNO) == IDYES)
			//{
			DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
		//}
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));				// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = Screen_Width;			// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = Screen_Height;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;		// �o�b�N�o�b�t�@�̃t�H�[�}�b�g�͌��ݐݒ肳��Ă�����̂��g��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = bWindow;						// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil = TRUE;			// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.BackBufferFormat = d3ddm.Format;			// �J���[���[�h�̎w��

	if (bWindow)
	{
		// �E�B���h�E���[�h
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;						// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz = 0;							// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		// �C���^�[�o��
	}
	else
	{
		// �t���X�N���[�����[�h
		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;							// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		// �f�B�X�v���C�A�_�v�^
		D3DDEVTYPE_HAL,										// �f�B�X�v���C�^�C�v
		hWnd,												// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,				// �f�o�C�X�쐬����̑g�ݍ��킹
		&d3dpp,												// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
		&g_pD3DDevice)))									// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// �J�����O���s��Ȃ�
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�

	// �e�N�X�`���̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// �e�N�X�`���̓ǂݍ���
	if (SafeLoad(Texture_BlackScreen, &UITexture[BlackScreen], "Other") == false)
	{
		return E_FAIL;
	}
	if (SafeLoad(Texture_SelectBox, &UITexture[SelectBox], "Other") == false)
	{
		return E_FAIL;
	}

	// ���\���p�t�H���g�̐ݒ�
	D3DXCreateFont(g_pD3DDevice, 108, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Consolas"), &Font[FontSize_108]);

	D3DXCreateFont(g_pD3DDevice, 54, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Consolas"), &Font[FontSize_54]);

	D3DXCreateFont(g_pD3DDevice, 40, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Consolas"), &Font[FontSize_40]);

	D3DXCreateFont(g_pD3DDevice, 30, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Consolas"), &Font[FontSize_30]);

	// ���\���p���C����ݒ�
	D3DXCreateLine(g_pD3DDevice, &Line);

	// �����_���̏�����
	srand((unsigned)time(NULL));

#ifdef _DEBUG
	// �f�o�b�O�e�X�g�̏���������
	if (FAILED(InitDebugTest(FirstInit)))
	{
		return E_FAIL;
	}
#endif

	// �G�l�~�[�X�e�[�^�X�t�@�C����ǂݍ���
	if (FAILED(LoadEnemyStatus()))
	{
		return E_FAIL;
	}

	// �o���b�g�^�C�v�t�@�C����ǂݍ���
	if (FAILED(LoadBulletType()))
	{
		return E_FAIL;
	}

	// �T�E���h�̏���������
	if (FAILED(InitSound(hWnd)))
	{
		return E_FAIL;
	}

	// �X�e�[�W�J�ډ�ʂ̏���������
	if (FAILED(InitTransition(FirstInit)))
	{
		return E_FAIL;
	}

	// �^�C�g���̏���������
	if (FAILED(InitTitle(FirstInit)))
	{
		return E_FAIL;
	}

	// ���@�I���̏���������
	if (FAILED(InitSelectChar(FirstInit)))
	{
		return E_FAIL;
	}

	// �`���[�g���A���̏���������
	if (FAILED(InitTutorial(FirstInit)))
	{
		return E_FAIL;
	}

	// �w�i�̏���������
	if (FAILED(InitBG(FirstInit)))
	{
		return E_FAIL;
	}

	// �O�i�̏���������
	if (FAILED(InitFrontBG(FirstInit)))
	{
		return E_FAIL;
	}

	// �v���C���[�̏���������
	if (FAILED(InitPlayer(FirstInit)))
	{
		return E_FAIL;
	}

	// �v���C���[�̒e�̏���������
	if (FAILED(InitPlayerBullet(FirstInit)))
	{
		return E_FAIL;
	}

	// �{���̏���������
	if (FAILED(InitBomb(FirstInit)))
	{
		return E_FAIL;
	}

	// �G�l�~�[�̏���������
	if (FAILED(InitEnemy(FirstInit)))
	{
		return E_FAIL;
	}

	// �e���̏���������
	if (FAILED(InitBarrage(FirstInit)))
	{
		return E_FAIL;
	}

	// �{�X�̏���������
	if (FAILED(InitBoss(FirstInit)))
	{
		return E_FAIL;
	}

	// �{�X�̒e���̏���������
	if (FAILED(InitBossBarrage(FirstInit)))
	{
		return E_FAIL;
	}

	// ���[�U�[�̏���������
	if (FAILED(InitLaser(FirstInit)))
	{
		return E_FAIL;
	}

	// �G�t�F�N�g�̏���������
	if (FAILED(InitEffect(FirstInit)))
	{
		return E_FAIL;
	}

	// �A�C�e���̏���������
	if (FAILED(InitItem(FirstInit)))
	{
		return E_FAIL;
	}

	// ��~��ʂ̏���������
	if (FAILED(InitPause(FirstInit)))
	{
		return E_FAIL;
	}

	// �Q�[�W�o�[�̏���������
	if (FAILED(InitGauge(FirstInit)))
	{
		return E_FAIL;
	}

	// �X�R�A�̏���������
	if (FAILED(InitScore(FirstInit)))
	{
		return E_FAIL;
	}

	// ���K���[�h�I���̏���������
	if (FAILED(InitPraticeSelect(FirstInit)))
	{
		return E_FAIL;
	}

	// �{�X���K���[�h�̏���������
	if (FAILED(InitBossPratice(FirstInit)))
	{
		return E_FAIL;
	}

	// �Q�[���I�[�o�[�̏���������
	if (FAILED(InitGameOver(FirstInit)))
	{
		return E_FAIL;
	}

	// �Q�[�����ʂ̏���������
	if (FAILED(InitResult(FirstInit)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	int i = 0;

	// �f�o�C�X�̊J��
	SafeRelease(g_pD3DDevice);

	// Direct3D�I�u�W�F�N�g�̊J��
	SafeRelease(g_pD3D);

	// �e�N�X�`���̊J��
	for (i = 0; i < UITexture_Max; i++)
	{
		SafeRelease(UITexture[i]);
	}

	// �t�H���g�̊J��
	for (i = 0; i < Font_Max; i++)
	{
		SafeRelease(Font[i]);
	}

	// ���C���̊J��
	SafeRelease(Line);

#ifdef _DEBUG
	// �f�o�b�O�e�X�g�̏I������
	UninitDebugTest();
#endif

	// �T�E���h�̏I������
	UninitSound();

	// �X�e�[�W�J�ډ�ʂ̏I������
	UninitTransition();

	// �^�C�g���̏I������
	UninitTitle();

	// ���@�I���̏I������
	UninitSelectChar();

	// �`���[�g���A���̏I������
	UninitTutorial();

	// �w�i�̏I������
	UninitBG();

	// �O�i�̏I������
	UninitFrontBG();

	// �v���C���[�̏I������
	UninitPlayer();

	// �v���C���[�̒e�̏I������
	UninitPlayerBullet();

	// �{���̏I������
	UninitBomb();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �e���̏I������
	UninitBarrage();

	// �{�X�̏I������
	UninitBoss();

	// �{�X�̒e���̏I������
	UninitBossBarrage();

	// ���[�U�[�̏I������
	UninitLaser();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �A�C�e���̏I������
	UninitItem();

	// ��~��ʂ̏I������
	UninitPause();

	// �Q�[�W�o�[�̏I������
	UninitGauge();

	// �X�R�A�̏I������
	UninitScore();

	// ���K���[�h�I���̏I������
	UninitPraticeSelect();

	// �{�X���K���[�h�̏I������
	UninitBossPratice();

	// �Q�[���I�[�o�[�̏I������
	UninitGameOver();

	// �Q�[�����ʂ̏I������
	UninitResult();

	// ���͏����̏I������
	UninitInput();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	int TutorialPhase = 0;
	bool InExitPratice = false;

	// ���͂̍X�V����
	UpdateInput();

	switch (GameStage)
	{
	case Stage_Title:

		// �^�C�g���̍X�V����
		UpdateTitle();

		// �X�e�[�W�J�ډ�ʂ̍X�V����
		UpdateTransition();

		break;
	case Stage_SelectChar:

		// ���@�I���̍X�V����
		UpdateSelectChar();

		// �v���C���[�̒e�̍X�V����
		UpdatePlayerBullet();

		// �����蔻��
		CheckHit();

		// �X�e�[�W�J�ډ�ʂ̍X�V����
		UpdateTransition();

		break;
	case Stage_Tutorial:

		// �`���[�g���A���̍X�V����
		UpdateTutorial();
		TutorialPhase = GetTutorialPhase();

		if (TutorialPhase != ExitTutorial)
		{
			// �O�i�̍X�V����
			UpdateFrontBG();
		}

		if (TutorialPhase == SetTutorialOver)
		{
			// �v���C���[�̍X�V����
			UpdatePlayer();

			// �v���C���[�̒e�̍X�V����
			UpdatePlayerBullet();

			// �{���̍X�V����
			UpdateBomb();

			// �G�l�~�[�̍X�V����
			UpdateEnemy();

			// �e���̍X�V����
			UpdateBarrage();

			// �G�t�F�N�g�̍X�V����
			UpdateEffect();

			// �A�C�e���̍X�V����
			UpdateItem();

			// �X�R�A�̍X�V����
			UpdateScore();

			// �����蔻��
			CheckHit();
		}

		// �Q�[�W�o�[�̍X�V����
		UpdateGauge();

		// �X�e�[�W�J�ډ�ʂ̍X�V����
		UpdateTransition();

		break;
	case Stage_Pause:

		// ��~��ʂ̍X�V����
		UpdatePause();

		// �X�e�[�W�J�ډ�ʂ̍X�V����
		UpdateTransition();

		break;
	case Stage_Game:

#ifdef _DEBUG
		// �f�o�b�O�e�X�g�̍X�V����
		UpdateDebugTest();
#endif
		// �w�i�̍X�V����
		UpdateBG();

		// �O�i�̍X�V����
		UpdateFrontBG();

		// �v���C���[�̍X�V����
		UpdatePlayer();

		// �v���C���[�̒e�̍X�V����
		UpdatePlayerBullet();

		// �{���̍X�V����
		UpdateBomb();

		// �G�l�~�[�̍X�V����
		UpdateEnemy();

		// �e���̍X�V����
		UpdateBarrage();

		// �{�X�̍X�V����
		UpdateBoss();

		// �{�X�̒e���̍X�V����
		UpdateBossBarrage();

		// ���[�U�[�̍X�V����
		UpdateLaser();

		// �G�t�F�N�g�̍X�V����
		UpdateEffect();

		// �A�C�e���̍X�V����
		UpdateItem();

		// �Q�[�W�o�[�̍X�V����
		UpdateGauge();

		// �X�R�A�̍X�V����
		UpdateScore();

		// �X�e�[�W�J�ډ�ʂ̍X�V����
		UpdateTransition();

		// �����蔻��
		CheckHit();

		break;
	case Stage_PraticeSelect:

		// ���K���[�h�I���̍X�V����
		UpdatePraticeSelect();

		// �{�X�̍X�V����
		UpdateBoss();

		// �{�X�̒e���̍X�V����
		UpdateBossBarrage();

		// ���[�U�[�̍X�V����
		UpdateLaser();

		// �X�e�[�W�J�ډ�ʂ̍X�V����
		UpdateTransition();

		break;
	case Stage_BossPratice:

		// �{�X���K���[�h�̍X�V����
		UpdateBossPratice();
		InExitPratice = GetExitPratice();

		if (InExitPratice == false)
		{
			// �O�i�̍X�V����
			UpdateFrontBG();

			// �v���C���[�̍X�V����
			UpdatePlayer();

			// �v���C���[�̒e�̍X�V����
			UpdatePlayerBullet();

			// �{���̍X�V����
			UpdateBomb();

			// �{�X�̍X�V����
			UpdateBoss();

			// �{�X�̒e���̍X�V����
			UpdateBossBarrage();

			// ���[�U�[�̍X�V����
			UpdateLaser();

			// �G�t�F�N�g�̍X�V����
			UpdateEffect();

			// �A�C�e���̍X�V����
			UpdateItem();

			// �Q�[�W�o�[�̍X�V����
			UpdateGauge();

			// �X�R�A�̍X�V����
			UpdateScore();

			// �����蔻��
			CheckHit();
		}

		// �X�e�[�W�J�ډ�ʂ̍X�V����
		UpdateTransition();

		break;
	case Stage_GameOver:

		// �Q�[���I�[�o�[�̍X�V����
		UpdateGameOver();

		// �X�e�[�W�J�ډ�ʂ̍X�V����
		UpdateTransition();

		break;
	case Stage_Result:

		// �Q�[�����ʂ̍X�V����
		UpdateResult();

		// �X�e�[�W�J�ډ�ʂ̍X�V����
		UpdateTransition();

		break;
	default:
		break;
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(128, 128, 255, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (GameStage)
		{
		case Stage_Title:

			// �^�C�g���̕`�揈��
			DrawTitle();

			// �^�C�g�����j���[�̕`�揈��
			DrawTitleMenu();

			// �X�e�[�W�J�ډ�ʂ̕`�揈��
			DrawTransition();

			break;
		case Stage_SelectChar:

			// ���@�I��w�i�̕`�揈��
			DrawSelectCharBG();

			// �G�l�~�[�̕`�揈��	
			DrawEnemy();

			// �v���C���[�̕`�揈��
			DrawPlayer();

			// �v���C���[�̒e�̕`�揈��
			DrawPlayerBullet();

			// ���@�I�����j���[�̕`�揈��
			DrawSelectCharMenu();

			// �X�e�[�W�J�ډ�ʂ̕`�揈��
			DrawTransition();

			break;
		case Stage_Tutorial:

			// �`���[�g���A���̕`�揈��
			DrawTutorial();

			// �v���C���[��Ԃ̕`�揈��
			DrawInformation();

			// �G�t�F�N�g�̕`�揈��
			DrawEffect();

			// �G�l�~�[�̕`�揈��	
			DrawEnemy();

			// �v���C���[�̕`�揈��
			DrawPlayer();

			// �A�C�e���̕`�揈��
			DrawItem();

			// �v���C���[�̒e�̕`�揈��
			DrawPlayerBullet();

			// �e���̕`�揈��
			DrawBarrage();

			// �Q�[�W�o�[�̕`�揈��
			DrawGauge();

			// �X�R�A�̕`�揈��
			DrawScore();

			// �O�i�̕`�揈��
			DrawFrontBG();

			// �{���̕`�揈��
			DrawBomb();

			// �`���[�g���A�����j���[�̕`�揈��
			DrawTutorialMenu();

			// �X�e�[�W�J�ډ�ʂ̕`�揈��
			DrawTransition();

			break;
		case Stage_Pause:

			// �w�i�̕`�揈��
			DrawBG();

			// �G�t�F�N�g�̕`�揈��
			DrawEffect();

			// �G�l�~�[�̕`�揈��	
			DrawEnemy();

			// �{�X�̕`�揈��	
			DrawBoss();

			// �v���C���[�̕`�揈��
			DrawPlayer();

			// �A�C�e���̕`�揈��
			DrawItem();

			// �v���C���[�̒e�̕`�揈��
			DrawPlayerBullet();

			// �e���̕`�揈��
			DrawBarrage();

			// ���[�U�[�̕`�揈��
			DrawLaser();

			// �{�X�̒e���̕`�揈��
			DrawBossBarrage();

			// �Q�[�W�o�[�̕`�揈��
			DrawGauge();

			// �X�R�A�̕`�揈��
			DrawScore();

			// �O�i�̕`�揈��
			DrawFrontBG();

			// �{���̕`�揈��
			DrawBomb();

			// ��~��ʂ̕`�揈��
			DrawPause();

			// �X�e�[�W�J�ډ�ʂ̕`�揈��
			DrawTransition();

			break;
		case Stage_Game:

			// �w�i�̕`�揈��
			DrawBG();

			// �G�t�F�N�g�̕`�揈��
			DrawEffect();

			// �G�l�~�[�̕`�揈��	
			DrawEnemy();

			// �{�X�̕`�揈��	
			DrawBoss();

			// �v���C���[�̕`�揈��
			DrawPlayer();

			// �A�C�e���̕`�揈��
			DrawItem();

			// �v���C���[�̒e�̕`�揈��
			DrawPlayerBullet();

			// �e���̕`�揈��
			DrawBarrage();

			// ���[�U�[�̕`�揈��
			DrawLaser();

			// �{�X�̒e���̕`�揈��
			DrawBossBarrage();

			// �Q�[�W�o�[�̕`�揈��
			DrawGauge();

			// �X�R�A�̕`�揈��
			DrawScore();

			// �O�i�̕`�揈��
			DrawFrontBG();

			// �{���̕`�揈��
			DrawBomb();

			// �X�e�[�W�J�ډ�ʂ̕`�揈��
			DrawTransition();

			break;
		case Stage_PraticeSelect:

			// ���K���[�h�I���̕`�揈��
			DrawPraticeSelect();

			// �X�e�[�W�J�ډ�ʂ̕`�揈��
			DrawTransition();

			// �{�X�̕`�揈��	
			DrawBoss();

			// ���[�U�[�̕`�揈��
			DrawLaser();

			// �{�X�̒e���̕`�揈��
			DrawBossBarrage();

			// �X�e�[�W�J�ډ�ʂ̕`�揈��
			DrawTransition();

			break;
		case Stage_BossPratice:

			// �{�X���K���[�h�̕`�揈��
			DrawBossPratice();

			// �v���C���[��Ԃ̕`�揈��
			DrawInformation();

			// �G�t�F�N�g�̕`�揈��
			DrawEffect();

			// �{�X�̕`�揈��	
			DrawBoss();

			// �v���C���[�̕`�揈��
			DrawPlayer();

			// �A�C�e���̕`�揈��
			DrawItem();

			// �v���C���[�̒e�̕`�揈��
			DrawPlayerBullet();

			// ���[�U�[�̕`�揈��
			DrawLaser();

			// �{�X�̒e���̕`�揈��
			DrawBossBarrage();

			// �Q�[�W�o�[�̕`�揈��
			DrawGauge();

			// �X�R�A�̕`�揈��
			DrawScore();

			// �O�i�̕`�揈��
			DrawFrontBG();

			// �{���̕`�揈��
			DrawBomb();

			// �{�X���K���[�h���j���[�̕`�揈��
			DrawBossPraticeMenu();

			// �X�e�[�W�J�ډ�ʂ̕`�揈��
			DrawTransition();

			break;
		case Stage_GameOver:

			// �w�i�̕`�揈��
			DrawBG();

			// �G�t�F�N�g�̕`�揈��
			DrawEffect();

			// �G�l�~�[�̕`�揈��	
			DrawEnemy();

			// �{�X�̕`�揈��	
			DrawBoss();

			// �v���C���[�̕`�揈��
			DrawPlayer();

			// �A�C�e���̕`�揈��
			DrawItem();

			// �v���C���[�̒e�̕`�揈��
			DrawPlayerBullet();

			// �e���̕`�揈��
			DrawBarrage();

			// ���[�U�[�̕`�揈��
			DrawLaser();

			// �{�X�̒e���̕`�揈��
			DrawBossBarrage();

			// �Q�[�W�o�[�̕`�揈��
			DrawGauge();

			// �X�R�A�̕`�揈��
			DrawScore();

			// �O�i�̕`�揈��
			DrawFrontBG();

			// �{���̕`�揈��
			DrawBomb();

			// �Q�[���I�[�o�[�̕`�揈��
			DrawGameOver();

			// �X�e�[�W�J�ډ�ʂ̕`�揈��
			DrawTransition();

			break;
		case Stage_Result:

			// �Q�[�����ʂ̕`�揈��
			DrawResult();

			// �X�e�[�W�J�ډ�ʂ̕`�揈��
			DrawTransition();

			break;
		default:
			break;
		}
		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

#ifdef _DEBUG
	// �f�o�b�O�e�X�g�̕`�揈��
	DrawDebugTest();
#endif

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�C�X�擾�֐�
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return(g_pD3DDevice);
}

//=============================================================================
// �e�N�X�`���ǂݍ��ފ֐�
//=============================================================================
bool SafeLoad(LPCSTR SrcFile, LPDIRECT3DTEXTURE9* TexturePtr, const char* ErrorSrc)
{
	char Message[64];

	D3DXCreateTextureFromFile(g_pD3DDevice, SrcFile, TexturePtr);
	if (*TexturePtr == NULL)
	{
		sprintf(Message, "Load %s Texture Failed�I", ErrorSrc);
		MessageBox(0, Message, "Error", 0);
		return false;
	}

	return true;
}

//=============================================================================
// FPS�擾�֐�
//=============================================================================
int GetFPSCount(void)
{
	return FPSCount;
}

//=============================================================================
// GameCount�擾�֐�
//=============================================================================
int GetGameCount(void)
{
	return GameCount;
}

//=============================================================================
// �Q�[����Փx�擾�֐�
//=============================================================================
int GetGameDiffuculty(void)
{
	return Difficulty;
}

//=============================================================================
// �Q�[����Փx�ݒ�֐�
//=============================================================================
void SetGameDiffuculty(int DifficultyMode)
{
	Difficulty = DifficultyMode;

	return;
}

//=============================================================================
// UITexture�擾�֐�
//=============================================================================
LPDIRECT3DTEXTURE9 GetUITexture(int UITextureType)
{
	return UITexture[UITextureType];
}

//=============================================================================
// Font�擾�֐�
//=============================================================================
LPD3DXFONT GetFont(int FontType)
{
	return Font[FontType];
}

//=============================================================================
// Line�擾�֐�
//=============================================================================
LPD3DXLINE GetLine(void)
{
	return Line;
}

//=============================================================================
// �Q�[���X�e�[�W�擾�֐�
//=============================================================================
int GetGameStage(void)
{
	return GameStage;
}

//=============================================================================
// �Q�[���X�e�[�W�ݒ�֐��i��ʑJ�ځj
//=============================================================================
void SetGameStage(int Stage)
{
	GameStage = Stage;

	return;
}

//=============================================================================
// �Q�[���X�e�[�W�ݒ�֐��i��ʑJ�ځj
//=============================================================================
bool CheckRange(int CheckMode, D3DXVECTOR3 Pos, int Width, int Height)
{
	bool Result = false;

	switch (CheckMode)
	{
	case OutOfRange:
		if (Pos.x - Width / 2 >= Screen_Width ||
			Pos.x + Width / 2 <= 0 ||
			Pos.y - Height / 2 >= Screen_Height ||
			Pos.y + Height / 2 <= 0)
		{
			Result = true;
		}
		break;
	case OutOfRange_SelectChar:
		if (Pos.x + Width / 2 >= DrawBox_X + Texture_DrawBox_Width ||
			Pos.x - Width / 2 <= DrawBox_X ||
			Pos.y + Height / 2 >= DrawBox_Y + Texture_DrawBox_Height ||
			Pos.y - Height / 2 <= DrawBox_Y)
		{
			Result = true;
		}
		break;
	case InRange:
		if (Pos.x + Width / 2 <= Screen_Width &&
			Pos.x - Width / 2 >= 0 &&
			Pos.y + Height / 2 <= Screen_Height &&
			Pos.y - Height / 2 >= MoveRange_Y)
		{
			Result = true;
		}
		break;
	default:
		break;
	}

	return Result;
}

//=============================================================================
// ��̍��W�̈ړ��p�x���v�Z����
//=============================================================================
float AngleCalculate(D3DXVECTOR3 ReferencePos, D3DXVECTOR3 ObjectPos)
{
	return atan2f(ObjectPos.y - ReferencePos.y, ObjectPos.x - ReferencePos.x);
}

//=============================================================================
// +Radian ~ -Radian�����_���ȃ��W�A����Ԃ�
//=============================================================================
float RandRadian(float Radian)
{
	return (-Radian + Radian * 2 * (rand() % (1000 + 1)) / 1000);
}

//=============================================================================
// �Q�[���̍ď�����
//=============================================================================
void ReInitGame(bool InitSound)
{
	// �J�E���g�̍ď�����
	GameCount = 0;
	FPSCount = 0;

#ifdef _DEBUG
	// �f�o�b�O�e�X�g�̍ď���������
	InitDebugTest(ReInit);
#endif

	// �T�E���h�̍ď���������
	if (InitSound == true)
	{
		ReInitSound();
	}

	if (GameStage != Stage_Title)
	{
		// �^�C�g���̍ď���������
		InitTitle(ReInit);
	}

	// ���@�I���̍ď���������
	InitSelectChar(ReInit);

	// �`���[�g���A���̍ď���������
	InitTutorial(ReInit);

	// �w�i�̍ď���������
	InitBG(ReInit);

	// �O�i�̍ď���������
	InitFrontBG(ReInit);

	// �v���C���[�̍ď���������
	InitPlayer(ReInit);

	// �v���C���[�̒e�̍ď���������
	InitPlayerBullet(ReInit);

	// �{���̍ď���������
	InitBomb(ReInit);

	// �G�l�~�[�̍ď���������
	InitEnemy(ReInit);

	// �e���̍ď���������
	InitBarrage(ReInit);

	// �{�X�̍ď���������
	InitBoss(ReInit);

	// �{�X�̒e���̍ď���������
	InitBossBarrage(ReInit);

	// ���[�U�[�̍ď���������
	InitLaser(ReInit);

	// �G�t�F�N�g�̍ď���������
	InitEffect(ReInit);

	// �A�C�e���̍ď���������
	InitItem(ReInit);

	// ��~��ʂ̍ď���������
	InitPause(ReInit);

	// �Q�[�W�o�[�̍ď���������
	InitGauge(ReInit);

	// �X�R�A�̍ď���������
	InitScore(ReInit);

	if (GameStage != Stage_PraticeSelect)
	{
		// ���K���[�h�I���̍ď���������
		InitPraticeSelect(ReInit);
	}

	// �{�X���K���[�h�̍ď���������
	InitBossPratice(ReInit);

	// �Q�[���I�[�o�[�̍ď���������
	InitGameOver(ReInit);

	// �Q�[�����ʂ̍ď���������
	InitResult(ReInit);

	return;
}


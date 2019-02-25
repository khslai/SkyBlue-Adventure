//=============================================================================
//
// Main処理 [main.cpp]
// Author：TH_GP11_GP11B341_35_頼凱興
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
// マクロ定義
//*****************************************************************************
#define CLASS_NAME			_T("AppClass")			// ウインドウのクラス名
#define WINDOW_NAME			_T("SkyBlue Adventure")	// ウインドウのキャプション名
#define FPS					(60)


//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9				g_pD3D = NULL;							// Direct3Dオブジェクト
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;					// Deviceオブジェクト(描画に必要)
LPDIRECT3DTEXTURE9		UITexture[UITexture_Max] = { NULL };	// UIテクスチャへのポインタ
LPD3DXFONT				Font[Font_Max] = { NULL };				// フォントへのポインタ
LPD3DXLINE				Line = NULL;							// ラインへのポインタ
int						GameStage = Stage_Title;				// ゲームステージ
int						GameCount = 0;							// ゲームカウント
int						Difficulty = Normal;					// 難易度
int						FPSCount = 0;							// FPSカウント

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// ゲームアイコン
	HICON MyIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	// 時間計測用
	DWORD dwExecLastTime;	// 前回画面処理の時間
	DWORD dwFPSLastTime;	// 前回FPG計算の時間
	DWORD dwCurrentTime;	// 現在時間
	DWORD dwFrameCount;		// 一秒間何回処理する = FPS
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

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																			// ウィンドウの左座標
		CW_USEDEFAULT,																			// ウィンドウの上座標
		Screen_Width + GetSystemMetrics(SM_CXDLGFRAME) * 2,										// ウィンドウ横幅
		Screen_Height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL);

	// DirectXの初期化(ウィンドウを作成してから行う)
	if (FAILED(Init(hWnd, true)))
	{
		return -1;
	}

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	// ウインドウの表示(Init()の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
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
				// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// システム時刻を取得
			dwCurrentTime = timeGetTime();

			// 1秒ごとに実行
			if ((dwCurrentTime - dwFPSLastTime) >= 1000)
			{

#ifdef _DEBUG
				// FPS計算
				if (GameStage == Stage_Game)
				{
					FPSCount = dwFrameCount;
				}
#endif
				// FPSを測定した時刻を保存
				dwFPSLastTime = dwCurrentTime;
				// カウントをクリア
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / FPS))	// 1/60秒ごとに実行
			{
				// 処理した時刻を保存
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				// ゲームカウントを加算
				if (GameStage == Stage_Game && Boss->Exist == false)
				{
					GameCount++;
				}

				// 処理回数のカウントを加算
				dwFrameCount++;
			}
		}
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
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
			// [ESC]キーが押された
		case VK_ESCAPE:
			//if (MessageBox(hWnd, "Do you want to Exit Game？", "Game Exit", MB_YESNO) == IDYES)
			//{
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
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
// 初期化処理
//=============================================================================
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));				// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;						// バックバッファの数
	d3dpp.BackBufferWidth = Screen_Width;			// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = Screen_Height;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;		// バックバッファのフォーマットは現在設定されているものを使う
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;			// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// デプスバッファとして16bitを使う
	d3dpp.BackBufferFormat = d3ddm.Format;			// カラーモードの指定

	if (bWindow)
	{
		// ウィンドウモード
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;						// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = 0;							// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		// インターバル
	}
	else
	{
		// フルスクリーンモード
		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;							// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		// ディスプレイアダプタ
		D3DDEVTYPE_HAL,										// ディスプレイタイプ
		hWnd,												// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,				// デバイス作成制御の組み合わせ
		&d3dpp,												// デバイスのプレゼンテーションパラメータ
		&g_pD3DDevice)))									// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダリングステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングを行わない
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定

	// テクスチャの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// テクスチャの読み込み
	if (SafeLoad(Texture_BlackScreen, &UITexture[BlackScreen], "Other") == false)
	{
		return E_FAIL;
	}
	if (SafeLoad(Texture_SelectBox, &UITexture[SelectBox], "Other") == false)
	{
		return E_FAIL;
	}

	// 情報表示用フォントの設定
	D3DXCreateFont(g_pD3DDevice, 108, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Consolas"), &Font[FontSize_108]);

	D3DXCreateFont(g_pD3DDevice, 54, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Consolas"), &Font[FontSize_54]);

	D3DXCreateFont(g_pD3DDevice, 40, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Consolas"), &Font[FontSize_40]);

	D3DXCreateFont(g_pD3DDevice, 30, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Consolas"), &Font[FontSize_30]);

	// 情報表示用ラインを設定
	D3DXCreateLine(g_pD3DDevice, &Line);

	// ランダムの初期化
	srand((unsigned)time(NULL));

#ifdef _DEBUG
	// デバッグテストの初期化処理
	if (FAILED(InitDebugTest(FirstInit)))
	{
		return E_FAIL;
	}
#endif

	// エネミーステータスファイルを読み込む
	if (FAILED(LoadEnemyStatus()))
	{
		return E_FAIL;
	}

	// バレットタイプファイルを読み込む
	if (FAILED(LoadBulletType()))
	{
		return E_FAIL;
	}

	// サウンドの初期化処理
	if (FAILED(InitSound(hWnd)))
	{
		return E_FAIL;
	}

	// ステージ遷移画面の初期化処理
	if (FAILED(InitTransition(FirstInit)))
	{
		return E_FAIL;
	}

	// タイトルの初期化処理
	if (FAILED(InitTitle(FirstInit)))
	{
		return E_FAIL;
	}

	// 自機選択の初期化処理
	if (FAILED(InitSelectChar(FirstInit)))
	{
		return E_FAIL;
	}

	// チュートリアルの初期化処理
	if (FAILED(InitTutorial(FirstInit)))
	{
		return E_FAIL;
	}

	// 背景の初期化処理
	if (FAILED(InitBG(FirstInit)))
	{
		return E_FAIL;
	}

	// 前景の初期化処理
	if (FAILED(InitFrontBG(FirstInit)))
	{
		return E_FAIL;
	}

	// プレイヤーの初期化処理
	if (FAILED(InitPlayer(FirstInit)))
	{
		return E_FAIL;
	}

	// プレイヤーの弾の初期化処理
	if (FAILED(InitPlayerBullet(FirstInit)))
	{
		return E_FAIL;
	}

	// ボムの初期化処理
	if (FAILED(InitBomb(FirstInit)))
	{
		return E_FAIL;
	}

	// エネミーの初期化処理
	if (FAILED(InitEnemy(FirstInit)))
	{
		return E_FAIL;
	}

	// 弾幕の初期化処理
	if (FAILED(InitBarrage(FirstInit)))
	{
		return E_FAIL;
	}

	// ボスの初期化処理
	if (FAILED(InitBoss(FirstInit)))
	{
		return E_FAIL;
	}

	// ボスの弾幕の初期化処理
	if (FAILED(InitBossBarrage(FirstInit)))
	{
		return E_FAIL;
	}

	// レーザーの初期化処理
	if (FAILED(InitLaser(FirstInit)))
	{
		return E_FAIL;
	}

	// エフェクトの初期化処理
	if (FAILED(InitEffect(FirstInit)))
	{
		return E_FAIL;
	}

	// アイテムの初期化処理
	if (FAILED(InitItem(FirstInit)))
	{
		return E_FAIL;
	}

	// 停止画面の初期化処理
	if (FAILED(InitPause(FirstInit)))
	{
		return E_FAIL;
	}

	// ゲージバーの初期化処理
	if (FAILED(InitGauge(FirstInit)))
	{
		return E_FAIL;
	}

	// スコアの初期化処理
	if (FAILED(InitScore(FirstInit)))
	{
		return E_FAIL;
	}

	// 練習モード選択の初期化処理
	if (FAILED(InitPraticeSelect(FirstInit)))
	{
		return E_FAIL;
	}

	// ボス練習モードの初期化処理
	if (FAILED(InitBossPratice(FirstInit)))
	{
		return E_FAIL;
	}

	// ゲームオーバーの初期化処理
	if (FAILED(InitGameOver(FirstInit)))
	{
		return E_FAIL;
	}

	// ゲーム結果の初期化処理
	if (FAILED(InitResult(FirstInit)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	int i = 0;

	// デバイスの開放
	SafeRelease(g_pD3DDevice);

	// Direct3Dオブジェクトの開放
	SafeRelease(g_pD3D);

	// テクスチャの開放
	for (i = 0; i < UITexture_Max; i++)
	{
		SafeRelease(UITexture[i]);
	}

	// フォントの開放
	for (i = 0; i < Font_Max; i++)
	{
		SafeRelease(Font[i]);
	}

	// ラインの開放
	SafeRelease(Line);

#ifdef _DEBUG
	// デバッグテストの終了処理
	UninitDebugTest();
#endif

	// サウンドの終了処理
	UninitSound();

	// ステージ遷移画面の終了処理
	UninitTransition();

	// タイトルの終了処理
	UninitTitle();

	// 自機選択の終了処理
	UninitSelectChar();

	// チュートリアルの終了処理
	UninitTutorial();

	// 背景の終了処理
	UninitBG();

	// 前景の終了処理
	UninitFrontBG();

	// プレイヤーの終了処理
	UninitPlayer();

	// プレイヤーの弾の終了処理
	UninitPlayerBullet();

	// ボムの終了処理
	UninitBomb();

	// エネミーの終了処理
	UninitEnemy();

	// 弾幕の終了処理
	UninitBarrage();

	// ボスの終了処理
	UninitBoss();

	// ボスの弾幕の終了処理
	UninitBossBarrage();

	// レーザーの終了処理
	UninitLaser();

	// エフェクトの終了処理
	UninitEffect();

	// アイテムの終了処理
	UninitItem();

	// 停止画面の終了処理
	UninitPause();

	// ゲージバーの終了処理
	UninitGauge();

	// スコアの終了処理
	UninitScore();

	// 練習モード選択の終了処理
	UninitPraticeSelect();

	// ボス練習モードの終了処理
	UninitBossPratice();

	// ゲームオーバーの終了処理
	UninitGameOver();

	// ゲーム結果の終了処理
	UninitResult();

	// 入力処理の終了処理
	UninitInput();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	int TutorialPhase = 0;
	bool InExitPratice = false;

	// 入力の更新処理
	UpdateInput();

	switch (GameStage)
	{
	case Stage_Title:

		// タイトルの更新処理
		UpdateTitle();

		// ステージ遷移画面の更新処理
		UpdateTransition();

		break;
	case Stage_SelectChar:

		// 自機選択の更新処理
		UpdateSelectChar();

		// プレイヤーの弾の更新処理
		UpdatePlayerBullet();

		// 当たり判定
		CheckHit();

		// ステージ遷移画面の更新処理
		UpdateTransition();

		break;
	case Stage_Tutorial:

		// チュートリアルの更新処理
		UpdateTutorial();
		TutorialPhase = GetTutorialPhase();

		if (TutorialPhase != ExitTutorial)
		{
			// 前景の更新処理
			UpdateFrontBG();
		}

		if (TutorialPhase == SetTutorialOver)
		{
			// プレイヤーの更新処理
			UpdatePlayer();

			// プレイヤーの弾の更新処理
			UpdatePlayerBullet();

			// ボムの更新処理
			UpdateBomb();

			// エネミーの更新処理
			UpdateEnemy();

			// 弾幕の更新処理
			UpdateBarrage();

			// エフェクトの更新処理
			UpdateEffect();

			// アイテムの更新処理
			UpdateItem();

			// スコアの更新処理
			UpdateScore();

			// 当たり判定
			CheckHit();
		}

		// ゲージバーの更新処理
		UpdateGauge();

		// ステージ遷移画面の更新処理
		UpdateTransition();

		break;
	case Stage_Pause:

		// 停止画面の更新処理
		UpdatePause();

		// ステージ遷移画面の更新処理
		UpdateTransition();

		break;
	case Stage_Game:

#ifdef _DEBUG
		// デバッグテストの更新処理
		UpdateDebugTest();
#endif
		// 背景の更新処理
		UpdateBG();

		// 前景の更新処理
		UpdateFrontBG();

		// プレイヤーの更新処理
		UpdatePlayer();

		// プレイヤーの弾の更新処理
		UpdatePlayerBullet();

		// ボムの更新処理
		UpdateBomb();

		// エネミーの更新処理
		UpdateEnemy();

		// 弾幕の更新処理
		UpdateBarrage();

		// ボスの更新処理
		UpdateBoss();

		// ボスの弾幕の更新処理
		UpdateBossBarrage();

		// レーザーの更新処理
		UpdateLaser();

		// エフェクトの更新処理
		UpdateEffect();

		// アイテムの更新処理
		UpdateItem();

		// ゲージバーの更新処理
		UpdateGauge();

		// スコアの更新処理
		UpdateScore();

		// ステージ遷移画面の更新処理
		UpdateTransition();

		// 当たり判定
		CheckHit();

		break;
	case Stage_PraticeSelect:

		// 練習モード選択の更新処理
		UpdatePraticeSelect();

		// ボスの更新処理
		UpdateBoss();

		// ボスの弾幕の更新処理
		UpdateBossBarrage();

		// レーザーの更新処理
		UpdateLaser();

		// ステージ遷移画面の更新処理
		UpdateTransition();

		break;
	case Stage_BossPratice:

		// ボス練習モードの更新処理
		UpdateBossPratice();
		InExitPratice = GetExitPratice();

		if (InExitPratice == false)
		{
			// 前景の更新処理
			UpdateFrontBG();

			// プレイヤーの更新処理
			UpdatePlayer();

			// プレイヤーの弾の更新処理
			UpdatePlayerBullet();

			// ボムの更新処理
			UpdateBomb();

			// ボスの更新処理
			UpdateBoss();

			// ボスの弾幕の更新処理
			UpdateBossBarrage();

			// レーザーの更新処理
			UpdateLaser();

			// エフェクトの更新処理
			UpdateEffect();

			// アイテムの更新処理
			UpdateItem();

			// ゲージバーの更新処理
			UpdateGauge();

			// スコアの更新処理
			UpdateScore();

			// 当たり判定
			CheckHit();
		}

		// ステージ遷移画面の更新処理
		UpdateTransition();

		break;
	case Stage_GameOver:

		// ゲームオーバーの更新処理
		UpdateGameOver();

		// ステージ遷移画面の更新処理
		UpdateTransition();

		break;
	case Stage_Result:

		// ゲーム結果の更新処理
		UpdateResult();

		// ステージ遷移画面の更新処理
		UpdateTransition();

		break;
	default:
		break;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(128, 128, 255, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (GameStage)
		{
		case Stage_Title:

			// タイトルの描画処理
			DrawTitle();

			// タイトルメニューの描画処理
			DrawTitleMenu();

			// ステージ遷移画面の描画処理
			DrawTransition();

			break;
		case Stage_SelectChar:

			// 自機選択背景の描画処理
			DrawSelectCharBG();

			// エネミーの描画処理	
			DrawEnemy();

			// プレイヤーの描画処理
			DrawPlayer();

			// プレイヤーの弾の描画処理
			DrawPlayerBullet();

			// 自機選択メニューの描画処理
			DrawSelectCharMenu();

			// ステージ遷移画面の描画処理
			DrawTransition();

			break;
		case Stage_Tutorial:

			// チュートリアルの描画処理
			DrawTutorial();

			// プレイヤー状態の描画処理
			DrawInformation();

			// エフェクトの描画処理
			DrawEffect();

			// エネミーの描画処理	
			DrawEnemy();

			// プレイヤーの描画処理
			DrawPlayer();

			// アイテムの描画処理
			DrawItem();

			// プレイヤーの弾の描画処理
			DrawPlayerBullet();

			// 弾幕の描画処理
			DrawBarrage();

			// ゲージバーの描画処理
			DrawGauge();

			// スコアの描画処理
			DrawScore();

			// 前景の描画処理
			DrawFrontBG();

			// ボムの描画処理
			DrawBomb();

			// チュートリアルメニューの描画処理
			DrawTutorialMenu();

			// ステージ遷移画面の描画処理
			DrawTransition();

			break;
		case Stage_Pause:

			// 背景の描画処理
			DrawBG();

			// エフェクトの描画処理
			DrawEffect();

			// エネミーの描画処理	
			DrawEnemy();

			// ボスの描画処理	
			DrawBoss();

			// プレイヤーの描画処理
			DrawPlayer();

			// アイテムの描画処理
			DrawItem();

			// プレイヤーの弾の描画処理
			DrawPlayerBullet();

			// 弾幕の描画処理
			DrawBarrage();

			// レーザーの描画処理
			DrawLaser();

			// ボスの弾幕の描画処理
			DrawBossBarrage();

			// ゲージバーの描画処理
			DrawGauge();

			// スコアの描画処理
			DrawScore();

			// 前景の描画処理
			DrawFrontBG();

			// ボムの描画処理
			DrawBomb();

			// 停止画面の描画処理
			DrawPause();

			// ステージ遷移画面の描画処理
			DrawTransition();

			break;
		case Stage_Game:

			// 背景の描画処理
			DrawBG();

			// エフェクトの描画処理
			DrawEffect();

			// エネミーの描画処理	
			DrawEnemy();

			// ボスの描画処理	
			DrawBoss();

			// プレイヤーの描画処理
			DrawPlayer();

			// アイテムの描画処理
			DrawItem();

			// プレイヤーの弾の描画処理
			DrawPlayerBullet();

			// 弾幕の描画処理
			DrawBarrage();

			// レーザーの描画処理
			DrawLaser();

			// ボスの弾幕の描画処理
			DrawBossBarrage();

			// ゲージバーの描画処理
			DrawGauge();

			// スコアの描画処理
			DrawScore();

			// 前景の描画処理
			DrawFrontBG();

			// ボムの描画処理
			DrawBomb();

			// ステージ遷移画面の描画処理
			DrawTransition();

			break;
		case Stage_PraticeSelect:

			// 練習モード選択の描画処理
			DrawPraticeSelect();

			// ステージ遷移画面の描画処理
			DrawTransition();

			// ボスの描画処理	
			DrawBoss();

			// レーザーの描画処理
			DrawLaser();

			// ボスの弾幕の描画処理
			DrawBossBarrage();

			// ステージ遷移画面の描画処理
			DrawTransition();

			break;
		case Stage_BossPratice:

			// ボス練習モードの描画処理
			DrawBossPratice();

			// プレイヤー状態の描画処理
			DrawInformation();

			// エフェクトの描画処理
			DrawEffect();

			// ボスの描画処理	
			DrawBoss();

			// プレイヤーの描画処理
			DrawPlayer();

			// アイテムの描画処理
			DrawItem();

			// プレイヤーの弾の描画処理
			DrawPlayerBullet();

			// レーザーの描画処理
			DrawLaser();

			// ボスの弾幕の描画処理
			DrawBossBarrage();

			// ゲージバーの描画処理
			DrawGauge();

			// スコアの描画処理
			DrawScore();

			// 前景の描画処理
			DrawFrontBG();

			// ボムの描画処理
			DrawBomb();

			// ボス練習モードメニューの描画処理
			DrawBossPraticeMenu();

			// ステージ遷移画面の描画処理
			DrawTransition();

			break;
		case Stage_GameOver:

			// 背景の描画処理
			DrawBG();

			// エフェクトの描画処理
			DrawEffect();

			// エネミーの描画処理	
			DrawEnemy();

			// ボスの描画処理	
			DrawBoss();

			// プレイヤーの描画処理
			DrawPlayer();

			// アイテムの描画処理
			DrawItem();

			// プレイヤーの弾の描画処理
			DrawPlayerBullet();

			// 弾幕の描画処理
			DrawBarrage();

			// レーザーの描画処理
			DrawLaser();

			// ボスの弾幕の描画処理
			DrawBossBarrage();

			// ゲージバーの描画処理
			DrawGauge();

			// スコアの描画処理
			DrawScore();

			// 前景の描画処理
			DrawFrontBG();

			// ボムの描画処理
			DrawBomb();

			// ゲームオーバーの描画処理
			DrawGameOver();

			// ステージ遷移画面の描画処理
			DrawTransition();

			break;
		case Stage_Result:

			// ゲーム結果の描画処理
			DrawResult();

			// ステージ遷移画面の描画処理
			DrawTransition();

			break;
		default:
			break;
		}
		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

#ifdef _DEBUG
	// デバッグテストの描画処理
	DrawDebugTest();
#endif

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイス取得関数
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return(g_pD3DDevice);
}

//=============================================================================
// テクスチャ読み込む関数
//=============================================================================
bool SafeLoad(LPCSTR SrcFile, LPDIRECT3DTEXTURE9* TexturePtr, const char* ErrorSrc)
{
	char Message[64];

	D3DXCreateTextureFromFile(g_pD3DDevice, SrcFile, TexturePtr);
	if (*TexturePtr == NULL)
	{
		sprintf(Message, "Load %s Texture Failed！", ErrorSrc);
		MessageBox(0, Message, "Error", 0);
		return false;
	}

	return true;
}

//=============================================================================
// FPS取得関数
//=============================================================================
int GetFPSCount(void)
{
	return FPSCount;
}

//=============================================================================
// GameCount取得関数
//=============================================================================
int GetGameCount(void)
{
	return GameCount;
}

//=============================================================================
// ゲーム難易度取得関数
//=============================================================================
int GetGameDiffuculty(void)
{
	return Difficulty;
}

//=============================================================================
// ゲーム難易度設定関数
//=============================================================================
void SetGameDiffuculty(int DifficultyMode)
{
	Difficulty = DifficultyMode;

	return;
}

//=============================================================================
// UITexture取得関数
//=============================================================================
LPDIRECT3DTEXTURE9 GetUITexture(int UITextureType)
{
	return UITexture[UITextureType];
}

//=============================================================================
// Font取得関数
//=============================================================================
LPD3DXFONT GetFont(int FontType)
{
	return Font[FontType];
}

//=============================================================================
// Line取得関数
//=============================================================================
LPD3DXLINE GetLine(void)
{
	return Line;
}

//=============================================================================
// ゲームステージ取得関数
//=============================================================================
int GetGameStage(void)
{
	return GameStage;
}

//=============================================================================
// ゲームステージ設定関数（画面遷移）
//=============================================================================
void SetGameStage(int Stage)
{
	GameStage = Stage;

	return;
}

//=============================================================================
// ゲームステージ設定関数（画面遷移）
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
// 二つの座標の移動角度を計算する
//=============================================================================
float AngleCalculate(D3DXVECTOR3 ReferencePos, D3DXVECTOR3 ObjectPos)
{
	return atan2f(ObjectPos.y - ReferencePos.y, ObjectPos.x - ReferencePos.x);
}

//=============================================================================
// +Radian ~ -Radianランダムなラジアンを返す
//=============================================================================
float RandRadian(float Radian)
{
	return (-Radian + Radian * 2 * (rand() % (1000 + 1)) / 1000);
}

//=============================================================================
// ゲームの再初期化
//=============================================================================
void ReInitGame(bool InitSound)
{
	// カウントの再初期化
	GameCount = 0;
	FPSCount = 0;

#ifdef _DEBUG
	// デバッグテストの再初期化処理
	InitDebugTest(ReInit);
#endif

	// サウンドの再初期化処理
	if (InitSound == true)
	{
		ReInitSound();
	}

	if (GameStage != Stage_Title)
	{
		// タイトルの再初期化処理
		InitTitle(ReInit);
	}

	// 自機選択の再初期化処理
	InitSelectChar(ReInit);

	// チュートリアルの再初期化処理
	InitTutorial(ReInit);

	// 背景の再初期化処理
	InitBG(ReInit);

	// 前景の再初期化処理
	InitFrontBG(ReInit);

	// プレイヤーの再初期化処理
	InitPlayer(ReInit);

	// プレイヤーの弾の再初期化処理
	InitPlayerBullet(ReInit);

	// ボムの再初期化処理
	InitBomb(ReInit);

	// エネミーの再初期化処理
	InitEnemy(ReInit);

	// 弾幕の再初期化処理
	InitBarrage(ReInit);

	// ボスの再初期化処理
	InitBoss(ReInit);

	// ボスの弾幕の再初期化処理
	InitBossBarrage(ReInit);

	// レーザーの再初期化処理
	InitLaser(ReInit);

	// エフェクトの再初期化処理
	InitEffect(ReInit);

	// アイテムの再初期化処理
	InitItem(ReInit);

	// 停止画面の再初期化処理
	InitPause(ReInit);

	// ゲージバーの再初期化処理
	InitGauge(ReInit);

	// スコアの再初期化処理
	InitScore(ReInit);

	if (GameStage != Stage_PraticeSelect)
	{
		// 練習モード選択の再初期化処理
		InitPraticeSelect(ReInit);
	}

	// ボス練習モードの再初期化処理
	InitBossPratice(ReInit);

	// ゲームオーバーの再初期化処理
	InitGameOver(ReInit);

	// ゲーム結果の再初期化処理
	InitResult(ReInit);

	return;
}


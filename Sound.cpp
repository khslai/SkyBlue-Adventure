//=============================================================================
//
// サウンド処理 [Sound.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// サウンドファイルのパス（hの通しナンバーと合わせること）
const TCHAR* SoundFilename[] =
{
	_T("data/Music/BGM/Battle.wav"),
	_T("data/Music/BGM/Boss_Loop.wav"),
	_T("data/Music/BGM/Boss_Start.wav"),
	_T("data/Music/BGM/BossPratice.wav"),
	_T("data/Music/BGM/GameOver.wav"),
	_T("data/Music/BGM/Result.wav"),
	_T("data/Music/BGM/Title.wav"),
	_T("data/Music/BGM/Tutorial.wav"),
	_T("data/Music/SE/Be_Shot.wav"),
	_T("data/Music/SE/BombExplosion.wav"),
	_T("data/Music/SE/BombBulletExplosion.wav"),
	_T("data/Music/SE/BombExtend.wav"),
	_T("data/Music/SE/BossDead.wav"),
	_T("data/Music/SE/BossHPZero.wav"),
	_T("data/Music/SE/BossShot_01.wav"),
	_T("data/Music/SE/BossShot_02.wav"),
	_T("data/Music/SE/BossShot_03.wav"),
	_T("data/Music/SE/BulletFreeze.wav"),
	_T("data/Music/SE/BulletShot_01.wav"),
	_T("data/Music/SE/BulletShot_02.wav"),
	_T("data/Music/SE/BulletShot_03.wav"),
	_T("data/Music/SE/BulletShot_04.wav"),
	_T("data/Music/SE/CalculateScore.wav"),
	_T("data/Music/SE/Determine_No.wav"),
	_T("data/Music/SE/Determine_Yes.wav"),
	_T("data/Music/SE/EnemyDead.wav"),
	_T("data/Music/SE/GameClear_01.wav"),
	_T("data/Music/SE/GameClear_02.wav"),
	_T("data/Music/SE/GameStart_01.wav"),
	_T("data/Music/SE/GameStart_02.wav"),
	_T("data/Music/SE/GetItem.wav"),
	_T("data/Music/SE/HPAlarm.wav"),
	_T("data/Music/SE/LockOn.wav"),
	_T("data/Music/SE/Menu.wav"),
	_T("data/Music/SE/PlayerBeShot.wav"),
	_T("data/Music/SE/PlayerDead.wav"),
	_T("data/Music/SE/PlayerShot.wav"),
	_T("data/Music/SE/SelectMove.wav"),
	_T("data/Music/SE/ShotBomb.wav"),
	_T("data/Music/SE/ShotBombBullet.wav"),
	_T("data/Music/SE/ShowScore.wav"),
	_T("data/Music/SE/StarExtend.wav"),
	_T("data/Music/SE/TutorialEnter.wav"),
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// サウンドのロード
LPDIRECTSOUNDBUFFER8 LoadSound(int Sound_No);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// サウンドインターフェース
IDirectSound8 *DirectSound = NULL;							
// サウンド用バッファ
LPDIRECTSOUNDBUFFER8 SoundBuffer[Sound_Max] = { NULL };		
// サウンドのボリューム
int SoundVolume = 0;

//=============================================================================
// 初期化処理
//=============================================================================
// hWnd:ウィンドウハンドル
HRESULT	InitSound(HWND hWnd)
{
	int Sound_No = 0;
	char Message[64];

	// DirectSoundオブジェクトの作成
	if (FAILED(DirectSoundCreate8(NULL, &DirectSound, NULL)))
	{
		return E_FAIL;
	}

	// 協調レベル設定 
	if (FAILED(DirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		return E_FAIL;
	}

	// サウンドのロード
	for (int Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		SoundBuffer[Sound_No] = LoadSound(Sound_No);
		if (SoundBuffer[Sound_No] == NULL)
		{
			sprintf(Message, "Load Sound No.%02d Failed！", Sound_No);
			MessageBox(0, Message, "Error", 0);
			return E_FAIL;
		}
	}

	// 開始位置に戻る
	for (Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		SoundBuffer[Sound_No]->SetCurrentPosition(0);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSound()
{
	for (int Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		SafeRelease(SoundBuffer[Sound_No]);
	}
	SafeRelease(DirectSound);

	return;
}

//=============================================================================
// サウンドのロード
//=============================================================================
LPDIRECTSOUNDBUFFER8 LoadSound(int Sound_No)
{
	// Sound_No:サウンドナンバー（ヘッダに定義された列挙型定数）
	// MMIO = マルチメディア入出力、の略。
	LPDIRECTSOUNDBUFFER  pBaseBuffer = NULL;	// 曲データの総合バッファ
	LPDIRECTSOUNDBUFFER8 pBuffer = NULL;		// 曲データのバッファ
	DSBUFFERDESC buff;							// バッファ設定構造体

	HMMIO hMmio = NULL;							// MMIOハンドル
	MMIOINFO mmioInfo;							// マルチメディアデータ構造体

	MMRESULT mmRes;								// MM処理結果格納変数
	MMCKINFO riffChunk, formatChunk, dataChunk;	// チャンクデータ構造体
	DWORD size;									// データサイズ格納用
	WAVEFORMATEX pcm;							// 曲データフォーマット構造体

	LPVOID pBlock;								// セカンダリバッファのデータ書き込み先アドレス
	DWORD  dwSize;								// セカンダリバッファのサイズ

	// 1.ハンドルをもとにファイルを開く
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen((LPSTR)SoundFilename[Sound_No], &mmioInfo, MMIO_READ);
	if (!hMmio)
	{
		return NULL;
	}

	// 2.ファイル解析① RIFFチャンク検索
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');				// 検索キーワード
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);	// キーワードをもとに検索
	// 見つからなかったら異常終了
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 3.ファイル解析② フォーマットチャンク検索
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// 検索キーワード
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// キーワードをもとに検索
	// 見つからなかったら異常終了
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);	// 検索情報をもとに読み込み
	// 正しく読み込めなかったら異常終了
	if (size != formatChunk.cksize)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// 4.ファイル解析③ データチャンク検索
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');					// 検索キーワード
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);	// キーワードをもとに検索
	// 見つからなかったら異常終了
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 5.データ読み込み
	char *pData = new char[dataChunk.cksize];					// 必要な大きさの領域を確保して
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);		// データを読み込む
	// 正しく読み込めなかったら異常終了
	if (size != dataChunk.cksize)
	{
		delete[] pData;
		return NULL;
	}

	// 6.曲を読み込む「セカンダリバッファ」を用意
	ZeroMemory(&buff, sizeof(DSBUFFERDESC));		// まず初期化
	buff.dwSize = sizeof(DSBUFFERDESC);				// そこから各種設定
	buff.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME;
	buff.dwBufferBytes = size;
	buff.lpwfxFormat = &pcm;

	// 総合バッファを作る
	if (FAILED(DirectSound->CreateSoundBuffer(&buff, &pBaseBuffer, NULL)))
	{
		return NULL;
	}

	// サウンドバッファを取り出す
	if (FAILED(pBaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&pBuffer)))
	{
		return NULL;
	}

	// 取り出したら総合バッファを捨てる
	pBaseBuffer->Release();

	// 7.作られたセカンダリバッファにデータを転送
	// まずは書き込みできるようバッファをロック
	if (FAILED(pBuffer->Lock(0, 0, &pBlock, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
	{
		return NULL;
	}

	memcpy(pBlock, pData, dwSize);	// データ転送
	delete[] pData;					// 元の読み込み領域を消す

	// バッファロック解除
	pBuffer->Unlock(pBlock, dwSize, 0, 0);

	// セカンダリバッファを返してようやく完了...
	return pBuffer;
}

//=============================================================================
// 音を鳴らす
//=============================================================================
void PlaySound(int Sound_No, int Flag, bool Restart)
{
	if (Restart == true)
	{
		// 続きから鳴らすので、最初から鳴らしたい場合はSetCurrentPosition(0)をすること
		SoundBuffer[Sound_No]->SetCurrentPosition(0);
	}
	// Flag   :1(E_DS8_FLAG_LOOP)ならループ再生
	SoundBuffer[Sound_No]->Play(0, 0, Flag);

	return;
}

//=============================================================================
// サウンドを停止する
//=============================================================================
void StopSound(int Sound_No)
{
	DWORD status;

	SoundBuffer[Sound_No]->GetStatus(&status);
	// 鳴っていたら
	if (status & DSBSTATUS_PLAYING)
	{
		// 意味的にはPauseになる。
		SoundBuffer[Sound_No]->Stop();
	}

	return;
}

//=============================================================================
// 再生中かどうか調べる
//=============================================================================
bool IsPlaying(int Sound_No)
{
	DWORD status;

	SoundBuffer[Sound_No]->GetStatus(&status);
	if (status & DSBSTATUS_PLAYING)
	{
		return true;
	}

	return false;
}

//=============================================================================
// 再初期化処理
//=============================================================================
void ReInitSound(void)
{
	// ボリューム初期化
	SoundVolume = 0;

	for (int Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		// 流してるサウンドを停止
		StopSound(Sound_No);

		// 本来のボリュームに戻す
		SoundBuffer[Sound_No]->SetVolume(SoundVolume);

		// プレイ位置が先頭に戻る
		SoundBuffer[Sound_No]->SetCurrentPosition(0);
	}

	return;
}

//=============================================================================
// サウンドのフェイドアウト
//=============================================================================
void SoundFadeOut(int Sound_No)
{
	// ボリューム少しずつ減る
	SoundVolume -= 10;

	// ボリューム設定
	SoundBuffer[Sound_No]->SetVolume(SoundVolume);

	return;
}
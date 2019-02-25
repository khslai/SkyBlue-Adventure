//=============================================================================
//
// �T�E���h���� [Sound.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �T�E���h�t�@�C���̃p�X�ih�̒ʂ��i���o�[�ƍ��킹�邱�Ɓj
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
// �v���g�^�C�v�錾
//*****************************************************************************
// �T�E���h�̃��[�h
LPDIRECTSOUNDBUFFER8 LoadSound(int Sound_No);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �T�E���h�C���^�[�t�F�[�X
IDirectSound8 *DirectSound = NULL;							
// �T�E���h�p�o�b�t�@
LPDIRECTSOUNDBUFFER8 SoundBuffer[Sound_Max] = { NULL };		
// �T�E���h�̃{�����[��
int SoundVolume = 0;

//=============================================================================
// ����������
//=============================================================================
// hWnd:�E�B���h�E�n���h��
HRESULT	InitSound(HWND hWnd)
{
	int Sound_No = 0;
	char Message[64];

	// DirectSound�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectSoundCreate8(NULL, &DirectSound, NULL)))
	{
		return E_FAIL;
	}

	// �������x���ݒ� 
	if (FAILED(DirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		return E_FAIL;
	}

	// �T�E���h�̃��[�h
	for (int Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		SoundBuffer[Sound_No] = LoadSound(Sound_No);
		if (SoundBuffer[Sound_No] == NULL)
		{
			sprintf(Message, "Load Sound No.%02d Failed�I", Sound_No);
			MessageBox(0, Message, "Error", 0);
			return E_FAIL;
		}
	}

	// �J�n�ʒu�ɖ߂�
	for (Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		SoundBuffer[Sound_No]->SetCurrentPosition(0);
	}

	return S_OK;
}

//=============================================================================
// �I������
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
// �T�E���h�̃��[�h
//=============================================================================
LPDIRECTSOUNDBUFFER8 LoadSound(int Sound_No)
{
	// Sound_No:�T�E���h�i���o�[�i�w�b�_�ɒ�`���ꂽ�񋓌^�萔�j
	// MMIO = �}���`���f�B�A���o�́A�̗��B
	LPDIRECTSOUNDBUFFER  pBaseBuffer = NULL;	// �ȃf�[�^�̑����o�b�t�@
	LPDIRECTSOUNDBUFFER8 pBuffer = NULL;		// �ȃf�[�^�̃o�b�t�@
	DSBUFFERDESC buff;							// �o�b�t�@�ݒ�\����

	HMMIO hMmio = NULL;							// MMIO�n���h��
	MMIOINFO mmioInfo;							// �}���`���f�B�A�f�[�^�\����

	MMRESULT mmRes;								// MM�������ʊi�[�ϐ�
	MMCKINFO riffChunk, formatChunk, dataChunk;	// �`�����N�f�[�^�\����
	DWORD size;									// �f�[�^�T�C�Y�i�[�p
	WAVEFORMATEX pcm;							// �ȃf�[�^�t�H�[�}�b�g�\����

	LPVOID pBlock;								// �Z�J���_���o�b�t�@�̃f�[�^�������ݐ�A�h���X
	DWORD  dwSize;								// �Z�J���_���o�b�t�@�̃T�C�Y

	// 1.�n���h�������ƂɃt�@�C�����J��
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen((LPSTR)SoundFilename[Sound_No], &mmioInfo, MMIO_READ);
	if (!hMmio)
	{
		return NULL;
	}

	// 2.�t�@�C����͇@ RIFF�`�����N����
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');				// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);	// �L�[���[�h�����ƂɌ���
	// ������Ȃ�������ُ�I��
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 3.�t�@�C����͇A �t�H�[�}�b�g�`�����N����
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// �L�[���[�h�����ƂɌ���
	// ������Ȃ�������ُ�I��
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);	// �����������Ƃɓǂݍ���
	// �������ǂݍ��߂Ȃ�������ُ�I��
	if (size != formatChunk.cksize)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// 4.�t�@�C����͇B �f�[�^�`�����N����
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');					// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);	// �L�[���[�h�����ƂɌ���
	// ������Ȃ�������ُ�I��
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 5.�f�[�^�ǂݍ���
	char *pData = new char[dataChunk.cksize];					// �K�v�ȑ傫���̗̈���m�ۂ���
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);		// �f�[�^��ǂݍ���
	// �������ǂݍ��߂Ȃ�������ُ�I��
	if (size != dataChunk.cksize)
	{
		delete[] pData;
		return NULL;
	}

	// 6.�Ȃ�ǂݍ��ށu�Z�J���_���o�b�t�@�v��p��
	ZeroMemory(&buff, sizeof(DSBUFFERDESC));		// �܂�������
	buff.dwSize = sizeof(DSBUFFERDESC);				// ��������e��ݒ�
	buff.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME;
	buff.dwBufferBytes = size;
	buff.lpwfxFormat = &pcm;

	// �����o�b�t�@�����
	if (FAILED(DirectSound->CreateSoundBuffer(&buff, &pBaseBuffer, NULL)))
	{
		return NULL;
	}

	// �T�E���h�o�b�t�@�����o��
	if (FAILED(pBaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&pBuffer)))
	{
		return NULL;
	}

	// ���o�����瑍���o�b�t�@���̂Ă�
	pBaseBuffer->Release();

	// 7.���ꂽ�Z�J���_���o�b�t�@�Ƀf�[�^��]��
	// �܂��͏������݂ł���悤�o�b�t�@�����b�N
	if (FAILED(pBuffer->Lock(0, 0, &pBlock, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
	{
		return NULL;
	}

	memcpy(pBlock, pData, dwSize);	// �f�[�^�]��
	delete[] pData;					// ���̓ǂݍ��ݗ̈������

	// �o�b�t�@���b�N����
	pBuffer->Unlock(pBlock, dwSize, 0, 0);

	// �Z�J���_���o�b�t�@��Ԃ��Ă悤�₭����...
	return pBuffer;
}

//=============================================================================
// ����炷
//=============================================================================
void PlaySound(int Sound_No, int Flag, bool Restart)
{
	if (Restart == true)
	{
		// ��������炷�̂ŁA�ŏ�����炵�����ꍇ��SetCurrentPosition(0)�����邱��
		SoundBuffer[Sound_No]->SetCurrentPosition(0);
	}
	// Flag   :1(E_DS8_FLAG_LOOP)�Ȃ烋�[�v�Đ�
	SoundBuffer[Sound_No]->Play(0, 0, Flag);

	return;
}

//=============================================================================
// �T�E���h���~����
//=============================================================================
void StopSound(int Sound_No)
{
	DWORD status;

	SoundBuffer[Sound_No]->GetStatus(&status);
	// ���Ă�����
	if (status & DSBSTATUS_PLAYING)
	{
		// �Ӗ��I�ɂ�Pause�ɂȂ�B
		SoundBuffer[Sound_No]->Stop();
	}

	return;
}

//=============================================================================
// �Đ������ǂ������ׂ�
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
// �ď���������
//=============================================================================
void ReInitSound(void)
{
	// �{�����[��������
	SoundVolume = 0;

	for (int Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		// �����Ă�T�E���h���~
		StopSound(Sound_No);

		// �{���̃{�����[���ɖ߂�
		SoundBuffer[Sound_No]->SetVolume(SoundVolume);

		// �v���C�ʒu���擪�ɖ߂�
		SoundBuffer[Sound_No]->SetCurrentPosition(0);
	}

	return;
}

//=============================================================================
// �T�E���h�̃t�F�C�h�A�E�g
//=============================================================================
void SoundFadeOut(int Sound_No)
{
	// �{�����[������������
	SoundVolume -= 10;

	// �{�����[���ݒ�
	SoundBuffer[Sound_No]->SetVolume(SoundVolume);

	return;
}
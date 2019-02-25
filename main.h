//=============================================================================
//
// Main�w�b�_�[ [main.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_


//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~

#include <time.h>
#include <Windows.h>
#include <stdio.h>
#include <d3dx9.h>
#include <tchar.h>


#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include <dsound.h>
#include <dinput.h>
#include <mmsystem.h>

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#if 1	// [������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J" -> "����" -> "�ǉ��̈ˑ��t�@�C��"�ɑΏۃ��C�u������ݒ肷��]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dsound.lib")
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// UI
#define Screen_Width		(1024)					// �E�C���h�E�̕�
#define Screen_Height		(768)					// �E�C���h�E�̍���
#define ScreenCenter_X		(Screen_Width / 2)		// �E�C���h�E�̒��S���W
#define ScreenCenter_Y		(Screen_Height / 2)		
#define MoveRange_Y			(90)					// �ړ��\�͈̔͂�Y���W
// �Q�[������
#define GameStartCount		(150)					// �Q�[���X�^�[�g�̎���
#define GameClearCount		(9300)					// �Q�[���N���A�̎���
// ���[�h�t�@�C��
#define StatusType_Max		(145)					// �G�l�~�[�X�e�[�^�X�����̍ő吔
#define BulletType_Max		(11)					// �o���b�g�X�e�[�^�X�����̍ő吔
#define EnemyBulletType_Max	(6)						// �e��ނ̍ő吔
#define PlayerBulletType_A	(EnemyBulletType_Max)   // �v���C���[�̒e�̃^�C�v
#define PlayerBulletType_B	(PlayerBulletType_A + 1)
#define OptionBulletType_A	(PlayerBulletType_B + 1)
#define OptionBulletType_B	(OptionBulletType_A + 1)
#define BombBulletType		(EnemyBulletType_Max + 4)
// �v���C���[
#define Player_Max			(1)						// �v���C���[�̍ő�l��
#define PlayerOption_Max	(2)						// �v���C���[�I�v�V�����̍ő吔
#define PlayerInvCount		(120)					// �v���C���[���G����
#define PlayerDebutCount	(120)					// �v���C���[�o��̃J�E���g
#define PlayerDebutPos_X	(80)					// �v���C���[�o��̍��W
#define PlayerDebutPos_Y	(ScreenCenter_Y)
#define PlayerStartPower	(2.0f)					// �v���C���[�p���[�̏������
#define PlayerStartBomb		(3)						// �v���C���[�{���̏������
#define PlayerHP_Max		(100.0f)				// �v���C���[HP�̍ő吔
#define PlayerPower_Max		(3.0f)					// �v���C���[�p���[�̍ő吔
#define PlayerBomb_Max		(5)						// �v���C���[�{���̍ő吔
#define PlayerItem_Max		(10)					// �v���C���[���A�C�e���̍ő吔
#define PlayerBullet_Max	(200)					// �v���C���[���ő�e��
#define BombBullet_Max		(Enemy_Max)				// �{�����˂���e�̍ő吔
// �G�l�~�[
#define Enemy_Max			(20)					// ��ʒ��\������G�l�~�[�̍ő吔
#define EnemyType_Max		(5)						// �G�l�~�[��ނ̍ő吔
#define EnemyBullet_Max		(1000)					// ��̃G�l�~�[���ő�e��
#define EnemyItem_Max		(6)						// ��̃G�l�~�[���A�C�e���̍ő吔
#define Barrage_Max			(30)					// ��ʒ��\������e���̍ő吔
// �{�X
#define BossBullet_Max		(1000)					// �{�X���ő�e��
#define BossItem_Max		(30)					// �{�X���A�C�e���̍ő吔
#define Laser_Max			(20)					// ��ʒ��\�����郌�[�U�[�̍ő吔
// �G�t�F�N�g
#define EffectCounter_Max	(200)					// ���\���ł���G�t�F�N�g�̍ő���
#define Effect_Max			(200)					// ���\���ł���G�t�F�N�g�̍ő吔
// �A�C�e��
#define ItemNum_Max			(500)					// ���\���ł���A�C�e���̍ő吔
#define HPItemValue			(0.5f)					// �A�C�e���㏸�̐��l
#define ScoreItemValue		(5000)
#define PowerItemValue		(0.02f)
// �I�������[�v
#define RepeatCount (60)
#define RepeatSpeed (5)
// �v�Z�p
#define PI					(D3DX_PI)
#define Root2				(1.414214f)
#define TextureDevideNum	(50)					// �|���S�������̐�

// ���_��
#define	Num_Vertex			(4)					
// �|���S����
#define Num_Polygon			(2)

// �F
#define WHITE(Alpha)		D3DCOLOR_RGBA(255, 255, 255, Alpha)
#define BLACK(Alpha)		D3DCOLOR_RGBA(0, 0, 0, Alpha)
#define GREEN(Alpha)		D3DCOLOR_RGBA(0, 255, 0, Alpha)
#define BLUE(Alpha)			D3DCOLOR_RGBA(0, 0, 255, Alpha)
#define YELLOW(Alpha)		D3DCOLOR_RGBA(255, 255, 0, Alpha)
#define RED(Alpha)			D3DCOLOR_RGBA(255, 0, 0, Alpha)
#define SKYBLUE(Alpha)		D3DCOLOR_RGBA(135, 206, 235, Alpha)
#define ORANGE(Alpha)		D3DCOLOR_RGBA(255, 165, 0, Alpha)
#define PURPLE(Alpha)		D3DCOLOR_RGBA(128, 0, 128, Alpha)

// ���j���[�e�N�X�`��
#define Texture_BlackScreen			_T("data/Texture/Other/BlackScreen.png")
#define Texture_BlackScreen_Width	(1024)
#define Texture_BlackScreen_Height	(768)
#define Texture_SelectBox			_T("data/Texture/Other/SelectBox.png")
#define Texture_SelectBox_Width		(350)
#define Texture_SelectBox_Height	(100)

// �v���C���[�I�v�V�����̍��W�Ɣ��ˊp�x
const int OptionPos_x[2] = { 0,0 };
const int OptionPos_y[2] = { -50, 50 };
const int OptionPos_x_LowSpeed[2] = { 30, 30 };
const int OptionPos_y_LowSpeed[2] = { -40, 40 };

// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// �e�N�X�`���̃������������[�X
#define SafeRelease(ReleaseObject) { if(ReleaseObject != NULL) { ReleaseObject->Release(); ReleaseObject = NULL; } }
// �p�x�����W�A���ɕϊ�����
#define DegreeToRadian(Degree)	(Degree) * (0.017456f)

// �w�i�̏��
enum BGState
{
	Day,
	Afternoon,
	Night,
};

// SetStage�̈���
enum StagePhase
{
	Stage_Title,
	Stage_SelectChar,
	Stage_Tutorial,
	Stage_Game,
	Stage_PraticeSelect,
	Stage_BossPratice,
	Stage_Pause,
	Stage_Result,
	Stage_GameOver,
};

// �^�C�g���̏��
enum TitlePhase
{
	GameStart,
	StartYes,
	BossPratice,
	PraticeYes,
	ExitGame,
	ExitCheck,
	ExitYes,
	EasyMode,
	NormalMode,
	HardMode,
};

// �Q�[���̓�Փx
enum GameDifficulty
{
	Easy,
	Normal,
	Hard,
};

// ���@�I�ԉ�ʂ̏��
enum SelectCharPhase
{
	SetObjectStart,
	ChooseTypeA,
	ChooseTypeB,
	ChooseCheck,
	ChooseOver,
	BackToTitle,
};

// �`���[�g���A���̏��
enum TutorialPhase
{
	ShowTutorial1,
	ShowTutorial2,
	SetTutorialStart,
	SetTutorialOver,
	ExitTutorial,
};

// ���K���[�h�I���̏��
enum PraticeSelectState
{
	SetBossStart,
	BossBarrage_M1,
	BossBarrage_M2,
	BossBarrage_L1,
	BossBarrage_L2,
	BossBarrage_L3,
	BossBarrage_L4,
	BossRush,
	BackToSelectChar,
};

// �������̏��
enum InitState
{
	FirstInit,
	ReInit,
};

// CheckRange�̈���
enum CheckType
{
	OutOfRange,
	OutOfRange_SelectChar,
	InRange,
};

// �G�l�~�[�̎��
enum
{
	Enemy00,
	Enemy01,
	Enemy02,
	Enemy03,
	Enemy04,
};

// �A�C�e���̎��
enum ItemType
{
	HPItem,
	ScoreItem,
	PowerItem,
	ItemBackGround,
	Nothing = -1,
};

// �e���̏��
enum BarragePhase
{
	BarrageStart,			// ���ˊJ�n
	EnemyDisappear,			// �G�l�~�[�͎��񂾁A�ł��e�͎c���Ă�
	BarrageOver,			// �S���̒e��������A�e���I���
};

// �e�N�X�`���F�̈���
enum Texturecolor
{
	White,
	Red,
	Red_AlphaHalf,
	Green,
	SkyBlue,
	Yellow,
	PowerGaugeGradation,
};

// �t�H���g�̎��
enum FontType
{
	FontSize_108,
	FontSize_54,
	FontSize_40,
	FontSize_30,
	Font_Max,
};

// UI�e�N�X�`���̎��
enum UITextureType
{
	BlackScreen,
	SelectBox,
	UITexture_Max,
};

// �{�X���̏��
enum BossState
{
	StandBy,
	ShotBullet,
	HPZero,
	HPRecover,
	NextPhase,
	MiddleBossOver,
	BossDisappear,
	LastBossOver,
};

// �{�X���̒e���i�K
enum BossPhase
{
	MiddleBoss_Phase1,
	MiddleBoss_LastPhase,
	LastBoss_Phase1,
	LastBoss_Phase2,
	LastBoss_Phase3,
	LastBoss_LastPhase,
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ��L���_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3		vtx;					// ���_���W
	float			rhw;					// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR		diffuse;				// ���ˌ�
	D3DXVECTOR2		tex;					// �e�N�X�`�����W
} VERTEX_2D;

// �I�����\����
typedef struct
{
	VERTEX_2D		VertexWk[Num_Vertex];	// ���_���i�[���[�N
	D3DXVECTOR3		Pos;					// ���W
	int				Phase;					// ���w���I����
	bool			InYes;					// ������"YES"��"No"��
}SELECT;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// �f�o�C�X�擾�֐�
LPDIRECT3DDEVICE9 GetDevice(void);
// UI�e�N�X�`���擾�֐�
LPDIRECT3DTEXTURE9 GetUITexture(int UITextureType);
// �e�L�X�g�t�H���g�擾�֐�
LPD3DXFONT GetFont(int FontType);
// �`�惉�C���擾�֐�
LPD3DXLINE GetLine(void);
// GameCount�擾�֐�
int GetGameCount(void);
// FPS�擾�֐�
int GetFPSCount(void);
// �Q�[���X�e�[�W�擾�֐�
int GetGameStage(void);
// �Q�[����Փx�擾�֐�
int GetGameDiffuculty(void);
// �Q�[����Փx�ݒ�֐�
void SetGameDiffuculty(int DifficultyMode);
// �Q�[���X�e�[�W�ݒ�֐��i��ʑJ�ځj
void SetGameStage(int Stage);
// �Q�[���̍ď�����
void ReInitGame(bool InitSound);
// �e�̓X�N���[���͈͓̔����ǂ������`�F�b�N
bool CheckRange(int CheckMode, D3DXVECTOR3 Pos, int Width, int Height);
// �e�N�X�`���ǂݍ��ފ֐�
bool SafeLoad(LPCSTR SrcFile, LPDIRECT3DTEXTURE9* TexturePtr, const char* ErrorSrc);
// ��̍��W�̈ړ��p�x���v�Z����
float AngleCalculate(D3DXVECTOR3 ReferencePos, D3DXVECTOR3 ObjectPos);
// +Radian ~ -Radian�����_���ȃ��W�A����Ԃ�
float RandRadian(float Radian);

#endif
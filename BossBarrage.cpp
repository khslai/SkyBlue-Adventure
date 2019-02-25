//=============================================================================
//
// �{�X�e������ [BossBarrage.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "Boss.h"
#include "Player.h"
#include "Enemy.h"
#include "Barrage.h"
#include "Laser.h"
#include "Effect.h"
#include "Item.h"
#include "Sound.h"
#include "DebugTest.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define StarDrawRange (150.0f)	// ���^�e���̕`��͈�
#define StarDrawSpeed (12)		// ���^�e���̕`�摬�x

// �e�̏��
enum
{
	StopRotate,			// ��]��~
	InRotate,			// ��]��
	AccelStart,			// �����J�n
	AccelStop,			// ������~
	ClockWise,			// ��]�����͉E���
	Counterclockwise,	// ��]�����͍����
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
static HRESULT MakeBulletVertex(int Bullet_No);
// ���_���W�̐ݒ�
static void SetBulletVertex(int Bullet_No);
// �e�N�X�`�����W�̐ݒ�
static void SetBulletTexture(int Bullet_No);
// �{�X�e���ݒu
void BossShot(int ShotPattern);
// ���^�e���v�Z
void StarBarrage(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���̃|�C���^
static LPDIRECT3DTEXTURE9 BulletTexture[BulletType_Max] = { NULL };

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBossBarrage(int InitState)
{
	int Bullet_No = 0;
	BOSS *Boss = GetBoss();

	for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
	{
		Boss->Bullet[Bullet_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Boss->Bullet[Bullet_No].PreviousPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Boss->Bullet[Bullet_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Boss->Bullet[Bullet_No].Radius = 0.0f;
		Boss->Bullet[Bullet_No].HitRadius = 0.0f;
		Boss->Bullet[Bullet_No].BaseAngle = 0.0f;
		Boss->Bullet[Bullet_No].MoveAngle = 0.0f;
		Boss->Bullet[Bullet_No].Speed = 0.0f;
		Boss->Bullet[Bullet_No].Type = 0;
		Boss->Bullet[Bullet_No].Count = 0;
		Boss->Bullet[Bullet_No].Color = 0;
		Boss->Bullet[Bullet_No].State = 0;
		Boss->Bullet[Bullet_No].Use = false;

		// ���_���̍쐬
		MakeBulletVertex(Bullet_No);
	}

	if (InitState == FirstInit)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Bullet_00, &BulletTexture[0], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Bullet_01, &BulletTexture[1], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Bullet_02, &BulletTexture[2], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Bullet_03, &BulletTexture[3], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Bullet_04, &BulletTexture[4], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Bullet_05, &BulletTexture[5], "Bullet") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBossBarrage(void)
{
	for (int i = 0; i < BulletType_Max; i++)
	{
		SafeRelease(BulletTexture[i]);
	}

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBossBarrage(void)
{
	int Bullet_No = 0;
	int GameStage = GetGameStage();
	BOSS *Boss = GetBoss();
	BULLETTYPE *BulletType = GetBulletType(Boss->BulletType);

	if (Boss->Exist == true)
	{
		if (Boss->State == ShotBullet)
		{
			switch (Boss->Phase)
			{
			case MiddleBoss_Phase1:
				BossShot(MiddleBoss_Phase1);
				break;
			case MiddleBoss_LastPhase:
				BossShot(MiddleBoss_LastPhase);
				break;
			case LastBoss_Phase1:
				BossShot(LastBoss_Phase1);
				break;
			case LastBoss_Phase2:
				BossShot(LastBoss_Phase2);
				break;
			case LastBoss_Phase3:
				BossShot(LastBoss_Phase3);
				break;
			case LastBoss_LastPhase:
				// ���[�U�[�ݒu
				if (Boss->ShotCount % 300 == 0 && Boss->BombDamage == false)
				{
					SetLaser();
				}
				BossShot(LastBoss_LastPhase);
				break;
			default:
				break;
			}
			// �J�E���g���Z
			Boss->ShotCount++;
		}

		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				// �񕜒��A�������A���X�{�X�I���̏�ԂȂ�
				if (Boss->State == HPRecover || Boss->State == BossDisappear || Boss->State == LastBossOver)
				{
					// ���ׂĂ̒e������
					Boss->Bullet[Bullet_No].Use = false;
					// �e�̃A�C�e����ݒu
					SetBossBulletItem(Boss->Bullet[Bullet_No].Pos);
					continue;
				}
				// 1�t���C���O�̒e���W���L�^
				Boss->Bullet[Bullet_No].PreviousPos = Boss->Bullet[Bullet_No].Pos;
				// ���W�v�Z
				Boss->Bullet[Bullet_No].Pos.x += cosf(Boss->Bullet[Bullet_No].MoveAngle) * Boss->Bullet[Bullet_No].Speed;
				Boss->Bullet[Bullet_No].Pos.y += sinf(Boss->Bullet[Bullet_No].MoveAngle) * Boss->Bullet[Bullet_No].Speed;
				// ����^�C�v�̒e��������A��]����
				if (Boss->Bullet[Bullet_No].State == InRotate && Boss->Bullet[Bullet_No].Type == 5)
				{
					Boss->Bullet[Bullet_No].Rot.z += 0.1f;
				}
				// �J�E���g���Z
				Boss->Bullet[Bullet_No].Count++;
				// �e�N�X�`���X�V
				SetBulletTexture(Bullet_No);
				SetBulletVertex(Bullet_No);
				// �e�̈ړ��������
				if (GameStage != Stage_PraticeSelect)
				{
					if (CheckRange(OutOfRange, Boss->Bullet[Bullet_No].Pos,
						BulletType->Width, BulletType->Height) == true)
					{
						Boss->Bullet[Bullet_No].Use = false;
					}
				}
				else
				{
					if (CheckRange(OutOfRange_SelectChar, Boss->Bullet[Bullet_No].Pos,
						BulletType->Width, BulletType->Height) == true)
					{
						Boss->Bullet[Bullet_No].Use = false;
					}
				}
			}
		}
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBossBarrage(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	BOSS *Boss = GetBoss();
	int Bullet_No = 0;

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	if (Boss->Exist == true)
	{
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				// �e�N�X�`���̐ݒ�
				switch (Boss->Bullet[Bullet_No].Type)
				{
				case 0: Device->SetTexture(0, BulletTexture[0]); break;
				case 1: Device->SetTexture(0, BulletTexture[1]); break;
				case 2: Device->SetTexture(0, BulletTexture[2]); break;
				case 3: Device->SetTexture(0, BulletTexture[3]); break;
				case 4: Device->SetTexture(0, BulletTexture[4]); break;
				case 5: Device->SetTexture(0, BulletTexture[5]); break;
				default:
					break;
				}

				// �|���S���̕`��
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Boss->Bullet[Bullet_No].VertexWk, sizeof(VERTEX_2D));
			}
		}
	}

	return;
}


//=============================================================================
// �{�X���e�𔭎�
//=============================================================================
void BossShot(int ShotPattern)
{
	int i = 0;
	int Bullet_No = 0;
	int Shot_Max = 0;
	int GameStage = GetGameStage();
	int GameDifficulty = GetGameDiffuculty();
	static int MoveDirection = 0;
	BOSS *Boss = GetBoss();
	int Count = Boss->ShotCount;
	PLAYER *Player = GetPlayer(0);
	BULLETTYPE *BulletType = GetBulletType(Boss->BulletType);

	// �{���̃_���[�W���󂯂�Œ�
	if (Boss->BombDamage == true)
	{
		// ������Ԃɖ߂�
		Boss->State = StandBy;
	}
	else
	{
		switch (ShotPattern)
		{
		case MiddleBoss_Phase1:
			// 1�^�[��60�J�E���g�A6��~�`30���̒e������
			// ����~�`�̒e��15�x�̍�������
			if (GameDifficulty == Easy)
			{
				Shot_Max = 20;
			}
			else
			{
				Shot_Max = 30;
			}
			// �ŏ��̐ݒu
			if (Count == 0)
			{
				// �{�X�ƃv���C���[�̊p�x���L�^
				Boss->ShotAngle = AngleCalculate(Boss->Pos, Player->Pos);
				// �e�̃^�C�v��ݒu
				Boss->BulletType = 0;
				BulletType = GetBulletType(Boss->BulletType);
			}
			// 90�J�E���g���ړ�
			if (Count % 90 == 0)
			{
				if (GameStage != Stage_PraticeSelect)
				{
					// X = 650 ~ 800�AY = 250 ~ 450�̋�ԓ��ړ�
					BossMove(
						D3DXVECTOR3((float)(rand() % (150 + 1) + 650), (float)(rand() % (200 + 1) + 250), 0.0f),
						60.0f);
				}
			}
			// 10�J�E���g���e����
			if (Count % 10 == 0)
			{
				// �e���ˉ�
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BulletShot_01, E_DS8_FLAG_NONE, true);
				}
				for (i = 0; i < Shot_Max; i++)
				{
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						if (Boss->Bullet[Bullet_No].Use == false)
						{
							// �e�̍ď���������
							Boss->Bullet[Bullet_No].Use = true;
							Boss->Bullet[Bullet_No].Pos = Boss->Pos;

							Boss->Bullet[Bullet_No].MoveAngle =
								Boss->ShotAngle + Count / 10 * DegreeToRadian(-15) + (2 * PI / Shot_Max * i);
							Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
							Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Boss->Bullet[Bullet_No].Type = Boss->BulletType;
							Boss->Bullet[Bullet_No].Count = 0;
							Boss->Bullet[Bullet_No].Color = 0;
							Boss->Bullet[Bullet_No].State = StopRotate;
							Boss->Bullet[Bullet_No].Speed = 5.0f;
							break;
						}
					}
				}
			}
			// 60�J�E���g��A���˒�~
			else if (Count > 60)
			{
				Boss->State = StandBy;
			}
			break;
		case MiddleBoss_LastPhase:
			// �T�C�����g�Z���i
			// 1�^�[��60�J�E���g�A10��~�`20���̒e������
			// 1�^�[���̍ŏ��͎��@�_���A��������͎��@�_�����炸�炷
			Shot_Max = 20;
			Count = Boss->ShotCount % 60;
			// �^�[���̍ŏ�
			if (Count == 0)
			{
				// ���@�_��
				Boss->ShotAngle = AngleCalculate(Boss->Pos, Player->Pos);
				// �e�̃^�C�v�ݒu
				if (Boss->ShotCount == 0)
				{
					Boss->BulletType = 2;
					BulletType = GetBulletType(Boss->BulletType);
					MoveDirection = 1;
				}
			}
			// ��������͎��@�_�����炸�炷
			else if (Count == 30)
			{
				// �p�x�����Z
				Boss->ShotAngle += PI / Shot_Max;
			}
			// 4�^�[�� = 240�J�E���g���ړ�
			if (Boss->ShotCount % (60 * 4) == 0)
			{
				if (GameStage != Stage_PraticeSelect)
				{
					// �ړ��p�^�[����ς��
					if (Boss->Pos.y >= ScreenCenter_Y + 70.0f)
					{
						MoveDirection = -1;
					}
					else if (Boss->Pos.y <= ScreenCenter_Y - 70.0f)
					{
						MoveDirection = 1;
					}
					// �{�X�ړ�
					BossMove(D3DXVECTOR3(Boss->Pos.x, Boss->Pos.y + 70.0f * MoveDirection, Boss->Pos.z), 60.0f);
				}
			}
			// 8�J�E���g����񔭎�
			if (Count % 8 == 0)
			{
				// �e���ˉ�
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BossShot_02, E_DS8_FLAG_NONE, true);
				}
				for (i = 0; i < Shot_Max; i++)
				{
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						if (Boss->Bullet[Bullet_No].Use == false)
						{
							Boss->Bullet[Bullet_No].Use = true;
							Boss->Bullet[Bullet_No].Pos = Boss->Pos;
							// �x�[�X�p�x����20��]���Ĕ���
							Boss->Bullet[Bullet_No].MoveAngle = Boss->ShotAngle + PI * 2 / 20 * i;
							Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
							Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Boss->Bullet[Bullet_No].Type = Boss->BulletType;
							Boss->Bullet[Bullet_No].Count = 0;
							Boss->Bullet[Bullet_No].Color = 0;
							Boss->Bullet[Bullet_No].State = StopRotate;
							Boss->Bullet[Bullet_No].Speed = 4.5f;
							break;
						}
					}
				}
			}
			// 5�J�E���g��1��A���ɔ��˂���e��o�^
			if (Count % 5 == 0 && GameDifficulty != Easy)
			{
				for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
				{
					if (Boss->Bullet[Bullet_No].Use == false)
					{
						Boss->Bullet[Bullet_No].Use = true;
						Boss->Bullet[Bullet_No].Pos.x = (float)(rand() % (Screen_Width / 3 + 1) + Screen_Width * 2 / 3);
						Boss->Bullet[Bullet_No].Pos.y = (float)(rand() % (Screen_Height + 1));
						Boss->Bullet[Bullet_No].MoveAngle = DegreeToRadian(180);
						Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
						Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
						Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
						Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
						Boss->Bullet[Bullet_No].Type = Boss->BulletType;
						Boss->Bullet[Bullet_No].Count = 0;
						Boss->Bullet[Bullet_No].Color = 0;
						Boss->Bullet[Bullet_No].State = StopRotate;
						Boss->Bullet[Bullet_No].Speed = 1.0f + ((float)rand() / RAND_MAX);
						break;
					}
				}
			}
			break;
		case LastBoss_Phase1:
			// �~�V���O�W����
			if (GameDifficulty == Easy)
			{
				Shot_Max = 40;
			}
			else if (GameDifficulty == Normal)
			{
				Shot_Max = 50;
			}
			else if (GameDifficulty == Hard)
			{
				Shot_Max = 60;
			}
			// �e�̃^�C�v�ݒu
			if (Boss->ShotCount == 0)
			{
				Boss->BulletType = 4;
				BulletType = GetBulletType(Boss->BulletType);
			}
			// 90�J�E���g���e����
			if (Count % 90 == 0)
			{
				// �e���ˉ�
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BossShot_01, E_DS8_FLAG_NONE, true);
				}
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < Shot_Max; j++)
					{
						for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
						{
							// �g�p����Ă��Ȃ��e�������
							if (Boss->Bullet[Bullet_No].Use == false)
							{
								// �e�̍ď���������
								Boss->Bullet[Bullet_No].Use = true;
								Boss->Bullet[Bullet_No].Type = Boss->BulletType;
								Boss->Bullet[Bullet_No].Pos = Boss->Pos;
								Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
								Boss->Bullet[Bullet_No].MoveAngle = Boss->BaseAngle + PI * 2 / Shot_Max * j;
								Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
								Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
								Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
								Boss->Bullet[Bullet_No].Color = 0;
								Boss->Bullet[Bullet_No].Count = 0;
								Boss->Bullet[Bullet_No].Speed = 3.0f;
								// �X�e�[�^�X�B0��1���ŉ�]�����������
								Boss->Bullet[Bullet_No].State = i == 0 ? ClockWise : Counterclockwise;
								break;
							}
						}
					}
				}
			}

			// �e�̉�]�v�Z
			for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
			{
				if (Boss->Bullet[Bullet_No].Use == true)
				{
					// 30�`120�J�E���g�Ȃ�
					if (30 < Boss->Bullet[Bullet_No].Count && Boss->Bullet[Bullet_No].Count < 120)
					{
						// 90�J�E���g������1.2���炷
						Boss->Bullet[Bullet_No].Speed -= 1.2f / 90.0f;
						// 90�J�E���g������90���X����
						Boss->Bullet[Bullet_No].MoveAngle += (PI / 2) / 90.0f * (Boss->Bullet[Bullet_No].State == ClockWise ? 1 : -1);
						Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
					}
				}
			}
			break;
		case LastBoss_Phase2:
			// �p�[�t�F�N�g�t���[�Y
			// 1�^�[��600�J�E���g
			Count = Boss->ShotCount % 600;
			// 0�A270�J�E���g�̎��Ɉړ�
			if (Count == 0 || Count == 270)
			{
				if (GameStage != Stage_PraticeSelect)
				{
					// X = 650 ~ 800�AY = 250 ~ 450�̋�ԓ��ړ�
					BossMove(
						D3DXVECTOR3((float)(rand() % (150 + 1) + 650), (float)(rand() % (200 + 1) + 250), 0.0f),
						60.0f);
				}
			}
			//�ŏ��̃����_������
			if (Count < 240)
			{
				// �e���ˉ�
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BulletShot_03, E_DS8_FLAG_NONE, true);
				}
				// �e�̃^�C�v��5��
				BulletType = GetBulletType(5);
				// 1�J�E���g��2�񔭎�
				for (i = 0; i < 2; i++)
				{
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						if (Boss->Bullet[Bullet_No].Use == false)
						{
							Boss->Bullet[Bullet_No].Use = true;
							Boss->Bullet[Bullet_No].Pos = Boss->Pos;
							Boss->Bullet[Bullet_No].MoveAngle = RandRadian(PI * 2 / 20) + PI * 2 / 10 * Count;
							Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
							Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Boss->Bullet[Bullet_No].Type = 5;
							Boss->Bullet[Bullet_No].Color = rand() % (6 + 1);
							Boss->Bullet[Bullet_No].Count = 0;
							Boss->Bullet[Bullet_No].State = InRotate;
							Boss->Bullet[Bullet_No].Speed = 5.0f + (3.0f * (float)rand() / RAND_MAX);
							break;
						}
					}
				}
			}
			// 240�J�E���g�̎��ɑS�ăX�g�b�v�����A�������A�J�E���g���Z�b�g
			else if (Count == 240)
			{
				// �e��~��
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BulletFreeze, E_DS8_FLAG_NONE, true);
				}
				for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
				{
					if (Boss->Bullet[Bullet_No].Use == true)
					{
						if (Boss->Bullet[Bullet_No].Type == 5)
						{
							Boss->Bullet[Bullet_No].Color = 7;
							Boss->Bullet[Bullet_No].Count = 0;
							Boss->Bullet[Bullet_No].State = StopRotate;
							Boss->Bullet[Bullet_No].Speed = 0.0f;
						}
					}
				}
			}
			// ���@�ˑ��ɂ��8��������
			else if (Count >= 270 && Count < 330 && Count % 3 == 0)
			{
				// �e���ˉ�
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BulletShot_03, E_DS8_FLAG_NONE, true);
				}
				// ���@�_��
				Boss->ShotAngle = AngleCalculate(Boss->Pos, Player->Pos);
				// 0�ԃ^�C�v�̒e
				BulletType = GetBulletType(0);
				for (i = 0; i < 7; i++)
				{
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						if (Boss->Bullet[Bullet_No].Use == false)
						{
							Boss->Bullet[Bullet_No].Use = true;
							Boss->Bullet[Bullet_No].Pos = Boss->Pos;
							// ���@�ƃ{�X�Ƃ̐����p�����7�����ɔ��˂���
							// 0�x -> -15�x -> 30�x -> -45�x -> 15�x -> -30�x -> 45�x
							Boss->Bullet[Bullet_No].MoveAngle =
								Boss->ShotAngle + DegreeToRadian(15) * (i >= 4 ? i % 4 + 1 : i % 4) * (i % 2 == 0 ? 1 : -1) + RandRadian(PI / 180);
							Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
							Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Boss->Bullet[Bullet_No].Type = 0;
							Boss->Bullet[Bullet_No].Color = 0;
							Boss->Bullet[Bullet_No].Count = 0;
							Boss->Bullet[Bullet_No].State = StopRotate;
							Boss->Bullet[Bullet_No].Speed = 5.0f;
							break;
						}
					}
				}
			}
			// �t���[�Y�I���A�ړ��J�n
			else if (Count >= 330)
			{
				for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
				{
					if (Boss->Bullet[Bullet_No].Use == true)
					{
						// �����_���ȕ����Ɉړ����n�߂�
						if (Boss->Bullet[Bullet_No].Type == 5)
						{
							if (Boss->Bullet[Bullet_No].Count == 150)
							{
								// �S���������_��
								Boss->Bullet[Bullet_No].MoveAngle = RandRadian(PI);
								Boss->Bullet[Bullet_No].State = InRotate;
							}
							if (Boss->Bullet[Bullet_No].Count > 150)
							{
								// �i�X����
								Boss->Bullet[Bullet_No].Speed += 0.02f;
							}
						}
					}
				}
			}
			break;
		case LastBoss_Phase3:
			// ���`�e��
			StarBarrage();
			break;
		case LastBoss_LastPhase:
			// �~�`����
			// �e�̃^�C�v�ݒu
			if (Count == 0)
			{
				// 1�Ԃ̒e
				Boss->BulletType = 1;
				BulletType = GetBulletType(Boss->BulletType);
			}
			// ����30��
			Shot_Max = 30;
			// 20�J�E���g���e����
			if (Count % 20 == 0)
			{
				// �e���ˉ�
				if (GameStage != Stage_PraticeSelect)
				{
					PlaySound(SE_BulletShot_01, E_DS8_FLAG_NONE, true);
				}

				// ���@�_��
				Boss->ShotAngle = AngleCalculate(Boss->Pos, Player->Pos);
				for (int i = 0; i < Shot_Max; i++)
				{
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						// �g�p����Ă��Ȃ��e�������
						if (Boss->Bullet[Bullet_No].Use == false)
						{
							// �e�̍ď���������
							Boss->Bullet[Bullet_No].Use = true;
							Boss->Bullet[Bullet_No].Type = Boss->BulletType;
							Boss->Bullet[Bullet_No].Pos = Boss->Pos;
							// �p�x + 360�x(2PI) / 20�� * ������
							Boss->Bullet[Bullet_No].MoveAngle = Boss->ShotAngle + 2 * PI / Shot_Max * i;
							Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
							Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
							Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
							Boss->Bullet[Bullet_No].Type = Boss->BulletType;
							Boss->Bullet[Bullet_No].Count = 0;
							Boss->Bullet[Bullet_No].Color = 0;
							Boss->Bullet[Bullet_No].State = StopRotate;
							Boss->Bullet[Bullet_No].Speed = 4.0f;
							break;
						}
					}
				}
			}
			break;
		default:
			break;
		}
	}

	return;
}

//=============================================================================
// ���`�e��
//=============================================================================
void StarBarrage(void)
{
	int i = 0;
	int Bullet_No = 0;
	int GameStage = GetGameStage();
	BOSS *Boss = GetBoss();
	int Count = Boss->ShotCount;
	BULLETTYPE *BulletType = GetBulletType(Boss->BulletType);
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Distance = 0.0f;
	// ���`�`��I���̃t���O
	bool DrawOver = false;
	// ���`�悵�Ă�����̔ԍ�
	static int DrawStarLine = 0;
	// ���`�`��I����̎��Ԃ��L�^
	static int DrawOverCount = 0;
	// �e�̈ړ��p�x�v�Z�p
	static int Num = 0;
	static float AngleTemp[BossBullet_Max] = { 0.0f };
	// �`��̊p�x
	static float DrawAngle = 0.0f;
	// ���`�悵�Ă�����W
	static float DrawPos_X = 0.0f;
	static float DrawPos_Y = 0.0f;
	// ���`�̒��S���W
	static float CenterPos_X = 0.0f;
	static float CenterPos_Y = 0.0f;
	// ���`�̌܂̒��_���W
	static float VertexPos_X[5] = { 0.0f };
	static float VertexPos_Y[5] = { 0.0f };

	// 750�J�E���g1���[�v
	Count = Boss->ShotCount % 750;
	if (Boss->ShotCount == 0)
	{
		Boss->BulletType = 0;
		BulletType = GetBulletType(Boss->BulletType);
	}
	// 1�����̍ŏ��Ȃ�
	if (Count == 0)
	{
		DrawOverCount = 0;
		DrawStarLine = 0;
		// ���`�̌܂̒��_���W���L�^
		for (i = 0; i < 5; i++)
		{
			VertexPos_X[i] = Boss->Pos.x + cosf(DegreeToRadian(90) + DegreeToRadian(72) * i) * StarDrawRange;
			VertexPos_Y[i] = Boss->Pos.y - sinf(DegreeToRadian(90) + DegreeToRadian(72) * i) * StarDrawRange;
		}
		// �`����W�͒��_[0]
		DrawPos_X = VertexPos_X[0];
		DrawPos_Y = VertexPos_Y[0];
		// ���S���W�̓{�X�̈ʒu
		CenterPos_X = Boss->Pos.x;
		CenterPos_Y = Boss->Pos.y;
		// �ŏ��̊p�x��108�x -> -36 -> -180 -> 36 -> -108
		DrawAngle = DegreeToRadian(108);
		Num = 0;
	}
	// ����`��
	if (DrawStarLine < 5)
	{
		// �`����W���v�Z
		DrawPos_X += cosf(DrawAngle) * StarDrawSpeed;
		DrawPos_Y += sinf(DrawAngle) * StarDrawSpeed;
		// ���S����̋������v�Z
		temp = D3DXVECTOR3((DrawPos_X - CenterPos_X), (DrawPos_Y - CenterPos_Y), 0.0f);
		Distance = D3DXVec3LengthSq(&temp);
		// ���S����̋����͔��a���傫���Ȃ� -> �~�ƌ����
		if (Distance > StarDrawRange * StarDrawRange)
		{
			// �����]���A���̊p�x-144�x
			DrawAngle -= DegreeToRadian(144);
			// ��̐���`������
			DrawStarLine++;
			// �`����W�͒��_���W�ɂȂ�
			DrawPos_X = VertexPos_X[(DrawStarLine * 2) % 5];
			DrawPos_Y = VertexPos_Y[(DrawStarLine * 2) % 5];
		}
		// �e���ˉ�
		if (IsPlaying(SE_BossShot_03) == false && GameStage != Stage_PraticeSelect)
		{
			PlaySound(SE_BossShot_03, E_DS8_FLAG_NONE, true);
		}
		for (i = 0; i < 5; i++)
		{
			// �����܂`��
			for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
			{
				if (Boss->Bullet[Bullet_No].Use == false)
				{
					Boss->Bullet[Bullet_No].Use = true;
					Boss->Bullet[Bullet_No].Type = Boss->BulletType;
					Boss->Bullet[Bullet_No].Pos.x = DrawPos_X;
					Boss->Bullet[Bullet_No].Pos.y = DrawPos_Y;
					Boss->Bullet[Bullet_No].MoveAngle = DegreeToRadian(-90) + DegreeToRadian(72) * i;
					Boss->Bullet[Bullet_No].Rot.z = Boss->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
					Boss->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
					Boss->Bullet[Bullet_No].Radius = BulletType->Radius;
					Boss->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
					Boss->Bullet[Bullet_No].Type = Boss->BulletType;
					Boss->Bullet[Bullet_No].Count = 0;
					Boss->Bullet[Bullet_No].Color = i;
					Boss->Bullet[Bullet_No].State = 0;
					Boss->Bullet[Bullet_No].Speed = 0.0f;
					AngleTemp[Bullet_No] = DrawAngle - PI + PI / 10 * Num;
					break;
				}
			}
		}
		Num++;
	}
	// �`���I��
	else
	{
		StopSound(SE_BossShot_03);
		DrawOver = true;
		DrawOverCount++;
	}

	if (DrawOver == true && DrawOverCount >= 60 && DrawOverCount <= 300)
	{
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				if (Boss->Bullet[Bullet_No].State != AccelStart)
				{
					// �e�W�J���ʉ�
					if (IsPlaying(SE_StarExtend) == false && GameStage != Stage_PraticeSelect)
					{
						PlaySound(SE_StarExtend, E_DS8_FLAG_NONE, false);
					}
					// �`�抮����b��Ȃ琯���T�����ɔ���
					// �X�s�[�h
					Boss->Bullet[Bullet_No].Speed = 4.0f;
					// �J�E���^���Z�b�g
					Boss->Bullet[Bullet_No].Count = 0;
					Boss->Bullet[Bullet_No].State = AccelStart;
				}
				else if (Boss->Bullet[Bullet_No].State == AccelStart)
				{
					if (Boss->Bullet[Bullet_No].Count <= 80)
					{
						// ����
						Boss->Bullet[Bullet_No].Speed -= 0.05f;
					}
					else if (Boss->Bullet[Bullet_No].Count == 120)
					{
						// �Z�b�g������p�x�����Y��ȋȐ���`��
						Boss->Bullet[Bullet_No].MoveAngle = AngleTemp[Bullet_No];
					}
					else if (Boss->Bullet[Bullet_No].Count > 120)
					{
						// ����
						Boss->Bullet[Bullet_No].Speed += 0.02f;
					}
				}
			}
		}
	}

	return;
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeBulletVertex(int Bullet_No)
{
	BOSS *Boss = GetBoss();

	// ���_���W�̐ݒ�	
	SetBulletVertex(Bullet_No);

	// rhw�̐ݒ�
	Boss->Bullet[Bullet_No].VertexWk[0].rhw = 1.0f;
	Boss->Bullet[Bullet_No].VertexWk[1].rhw = 1.0f;
	Boss->Bullet[Bullet_No].VertexWk[2].rhw = 1.0f;
	Boss->Bullet[Bullet_No].VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Boss->Bullet[Bullet_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Boss->Bullet[Bullet_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Boss->Bullet[Bullet_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Boss->Bullet[Bullet_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetBulletTexture(Bullet_No);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetBulletTexture(int Bullet_No)
{
	BOSS *Boss = GetBoss();
	BULLETTYPE *BulletType = GetBulletType(Boss->Bullet[Bullet_No].Type);
	int x = Boss->Bullet[Bullet_No].Color;
	float sizeX = 1.0f / BulletType->Devide_X;

	Boss->Bullet[Bullet_No].VertexWk[0].tex = D3DXVECTOR2((float)x * sizeX, 0.0f);
	Boss->Bullet[Bullet_No].VertexWk[1].tex = D3DXVECTOR2((float)x * sizeX + sizeX, 0.0f);
	Boss->Bullet[Bullet_No].VertexWk[2].tex = D3DXVECTOR2((float)x * sizeX, 1.0f);
	Boss->Bullet[Bullet_No].VertexWk[3].tex = D3DXVECTOR2((float)x * sizeX + sizeX, 1.0f);

	return;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetBulletVertex(int Bullet_No)
{
	BOSS *Boss = GetBoss();

	Boss->Bullet[Bullet_No].VertexWk[0].vtx.x =
		Boss->Bullet[Bullet_No].Pos.x - cosf(Boss->Bullet[Bullet_No].BaseAngle + Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[0].vtx.y =
		Boss->Bullet[Bullet_No].Pos.y - sinf(Boss->Bullet[Bullet_No].BaseAngle + Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[1].vtx.x =
		Boss->Bullet[Bullet_No].Pos.x + cosf(Boss->Bullet[Bullet_No].BaseAngle - Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[1].vtx.y =
		Boss->Bullet[Bullet_No].Pos.y - sinf(Boss->Bullet[Bullet_No].BaseAngle - Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[2].vtx.x =
		Boss->Bullet[Bullet_No].Pos.x - cosf(Boss->Bullet[Bullet_No].BaseAngle - Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[2].vtx.y =
		Boss->Bullet[Bullet_No].Pos.y + sinf(Boss->Bullet[Bullet_No].BaseAngle - Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[3].vtx.x =
		Boss->Bullet[Bullet_No].Pos.x + cosf(Boss->Bullet[Bullet_No].BaseAngle + Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;
	Boss->Bullet[Bullet_No].VertexWk[3].vtx.y =
		Boss->Bullet[Bullet_No].Pos.y + sinf(Boss->Bullet[Bullet_No].BaseAngle + Boss->Bullet[Bullet_No].Rot.z) * Boss->Bullet[Bullet_No].Radius;

	return;
}
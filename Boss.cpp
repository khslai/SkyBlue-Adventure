//=============================================================================
//
// �{�X���� [Boss.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "Boss.h"
#include "Title.h"
#include "Player.h"
#include "Enemy.h"
#include "Barrage.h"
#include "Laser.h"
#include "Effect.h"
#include "Gauge.h"
#include "Item.h"
#include "PraticeSelect.h"
#include "Sound.h"
#include "DebugTest.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define AnimChangeCount		(15)		// 15�J�E���g�{�X�A�j���ϊ�
#define MiddleBossCount		(5400)		// 5400�J�E���g�̎����{�X�o��
#define LastBossCount		(9000)		// 9000�J�E���g�̎����X�{�X�o��
#define BossStartHP			(3000)		// �{�X��HP
#define BossLife_Max		(5)			// �{�X�c�@�̍ő吔	
#define BossLifePos_X		(BossHPGauge_Pos_X - Texture_BossLife_Width)	// �{�X�c�@�̕\�����W
#define BossLifePos_Y		(BossHPGauge_Pos_Y - Texture_Gauge_Boss_Height)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeBossVertex(void);
HRESULT MakeBossLifeVertex(void);
// ���_���W�̐ݒ�
void SetBossVertex(void);
void SetBossLifeVertex(void);
// �e�N�X�`�����W�̐ݒ�
void SetBossTexture(void);
void SetBossLifeTexture(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 BossTexture = NULL;
LPDIRECT3DTEXTURE9 BossLifeTexture = NULL;
// �{�X�\����
BOSS Boss;
// �{�X�����l�\����
BOSSLIFE BossLife[BossLife_Max];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBoss(int InitState)
{
	Boss.Exist = false;
	Boss.MiddleBossOver = false;
	Boss.InMove = false;
	Boss.DebutMove = false;
	Boss.BombDamage = false;
	Boss.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.BeforeMovePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.Acceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.Destination = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.Radius = 0.0f;
	Boss.HitRadius = 0.0f;
	Boss.BaseAngle = 0.0f;
	Boss.MoveAngle = 0.0f;
	Boss.ShotAngle = 0.0f;
	Boss.MoveTime = 0.0f;
	Boss.AnimPattern = 0;
	Boss.Count = 0;
	Boss.ShotCount = 0;
	Boss.MoveCount = 0;
	Boss.MovePattern = 0;
	Boss.Color = 0;
	Boss.HP_Max = 0.0f;
	Boss.HP = Boss.HP_Max;
	Boss.Life = 0;
	Boss.BulletType = 0;
	Boss.State = 0;
	Boss.Phase = 0;

	// ���_���̍쐬
	MakeBossVertex();

	for (int i = 0; i < BossLife_Max; i++)
	{
		BossLife[i].Use = false;
		MakeBossLifeVertex();
	}

	if (InitState == FirstInit)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Boss, &BossTexture, "Boss") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BossLife, &BossLifeTexture, "Boss") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBoss(void)
{
	SafeRelease(BossTexture);
	SafeRelease(BossLifeTexture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBoss(void)
{
	D3DXVECTOR3 PreviousPos = Boss.Pos;
	int GameCount = GetGameCount();
	int GameStage = GetGameStage();

	// �{�X�ݒu
	if ((GameCount == MiddleBossCount || GameCount == LastBossCount) && Boss.Exist == false)
	{
		SetBoss();
	}

	if (Boss.Exist == true)
	{
		// �{�X�̒i�K�ω�
		// �{�X��HP��0�ɂȂ�����
		if (Boss.State == HPZero)
		{
			// �����l - 1
			Boss.Life--;
			// �w��ʒu�Ɉړ�
			BossMove(D3DXVECTOR3(Screen_Width * 0.75, ScreenCenter_Y, 0.0f), 60.0f);
			// �{�X�A�C�e���ݒu
			SetBossItem(Boss.Pos);
			// HP�񕜒i�K�����
			Boss.State = HPRecover;
		}
		// HP�񕜏I���A���̒i�K�����
		else if (Boss.State == NextPhase)
		{
			Boss.HP = Boss.HP_Max;
			Boss.ShotCount = 0;
			Boss.Color++;
			Boss.Phase++;
			Boss.State = StandBy;
		}
		// ���{�X�I��
		else if (Boss.State == MiddleBossOver)
		{
			BossMove(D3DXVECTOR3(Screen_Width + 100, ScreenCenter_Y, 0.0f), 60.0f);
			SetBossItem(Boss.Pos);
			Boss.State = BossDisappear;
		}
		// �X�e�[�W���A�{�X��������
		else if (Boss.State == BossDisappear)
		{
			if (CheckRange(OutOfRange, Boss.Pos, Texture_Boss_Width, Texture_Boss_Height) == true)
			{
				Boss.Exist = false;
			}
		}
		// ���X�{�X�I��
		else if (Boss.State == LastBossOver)
		{
			PlaySound(SE_BossDead, E_DS8_FLAG_NONE, true);
			Boss.Exist = false;
			SetBossItem(Boss.Pos);
			SetBossDeadEffect();
			return;
		}

		// �{�X�̃A�j���v�Z
		if ((Boss.Count % AnimChangeCount) == 0)
		{
			// �e�N�X�`���A�j���p�^�[���v�Z
			Boss.AnimPattern = (Boss.AnimPattern + 1) % Texture_Boss_Divide_X;
		}

		// ��ʏ�Ԃ̏㉺�ړ�
		if (Boss.InMove == false && Boss.Phase != MiddleBoss_LastPhase)
		{
			if (Boss.Count % 120 < 30 || Boss.Count % 120 >= 90)
			{
				Boss.Pos.y++;
			}
			else if (Boss.Count % 120 >= 30 && Boss.Count % 120 < 90)
			{
				Boss.Pos.y--;
			}
			// �ړ��s�͈̔͂ɓ�������A�O�̍��W�ɖ߂�
			if (CheckRange(InRange, Boss.Pos, Texture_Boss_Width, Texture_Boss_Height) == false && Boss.DebutMove == false)
			{
				Boss.Pos.y = PreviousPos.y;
			}
		}

		// �{�X�̈ړ��v�Z
		if (Boss.MoveCount < Boss.MoveTime)
		{
			Boss.Pos.x =
				Boss.BeforeMovePos.x + (Boss.Velocity.x * Boss.MoveCount + 0.5f * Boss.Acceleration.x * Boss.MoveCount * Boss.MoveCount);
			if (CheckRange(InRange, Boss.Pos, Texture_Boss_Width, Texture_Boss_Height) == false && Boss.DebutMove == false && Boss.State != BossDisappear)
			{
				Boss.Pos.x = PreviousPos.x;
			}
			Boss.Pos.y =
				Boss.BeforeMovePos.y + (Boss.Velocity.y * Boss.MoveCount + 0.5f * Boss.Acceleration.y * Boss.MoveCount * Boss.MoveCount);
			if (CheckRange(InRange, Boss.Pos, Texture_Boss_Width, Texture_Boss_Height) == false && Boss.DebutMove == false && Boss.State != BossDisappear)
			{
				Boss.Pos.y = PreviousPos.y;
			}
			Boss.MoveCount++;
		}
		// �ړ������A�e���ˊJ�n
		else if (Boss.MoveCount >= Boss.MoveTime && Boss.InMove == true && Boss.State != HPRecover)
		{
			Boss.InMove = false;
			Boss.DebutMove = false;
			if (Boss.State != StandBy)
			{
				Boss.State = ShotBullet;
			}
		}

		// �ҋ@���Ԍv�Z
		if (Boss.InMove == false && Boss.State == StandBy)
		{
			// �ҋ@�����A���ˊJ�n
			if (Boss.Count % 180 == 0)
			{
				Boss.State = ShotBullet;
				Boss.ShotCount = 0;
			}
		}

		// �{�X�J�E���g���Z
		Boss.Count++;

		// �{�X�e�N�X�`���X�V
		SetBossTexture();
		SetBossColor(White);
		SetBossVertex();

		// �{�X�����l�̃e�N�X�`���X�V
		SetBossLifeTexture();
		SetBossLifeVertex();
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	if (Boss.Exist == true)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, BossTexture);

		// �|���S���̕`��
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Boss.VertexWk, sizeof(VERTEX_2D));

		for (int i = 0; i < Boss.Life; i++)
		{
			if (BossLife[i].Use == true)
			{
				Device->SetTexture(0, BossLifeTexture);

				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BossLife[i].VertexWk, sizeof(VERTEX_2D));
			}
		}
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeBossVertex(void)
{
	// ���_���W�̐ݒ�	
	SetBossVertex();

	// rhw�̐ݒ�
	Boss.VertexWk[0].rhw = 1.0f;
	Boss.VertexWk[1].rhw = 1.0f;
	Boss.VertexWk[2].rhw = 1.0f;
	Boss.VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	SetBossColor(White);

	// �e�N�X�`�����W�̐ݒ�
	SetBossTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetBossTexture(void)
{
	int x = Boss.AnimPattern;
	int y = Boss.Color;
	float sizeX = 1.0f / Texture_Boss_Divide_X;
	float sizeY = 1.0f / Texture_Boss_Divide_Y;

	Boss.VertexWk[0].tex = D3DXVECTOR2((float)x * sizeX, (float)y * sizeY);
	Boss.VertexWk[1].tex = D3DXVECTOR2((float)x * sizeX + sizeX, (float)y * sizeY);
	Boss.VertexWk[2].tex = D3DXVECTOR2((float)x * sizeX, (float)y * sizeY + sizeY);
	Boss.VertexWk[3].tex = D3DXVECTOR2((float)x * sizeX + sizeX, (float)y * sizeY + sizeY);

	return;
}

//=============================================================================
// �{�X�̐F�̐ݒ�
//=============================================================================
void SetBossColor(int Color)
{
	switch (Color)
	{
	case White:
		Boss.VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Boss.VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Boss.VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Boss.VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		break;
		// �����ꂽ��A�F���ς��
	case Red:
		Boss.VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		Boss.VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		Boss.VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		Boss.VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetBossVertex(void)
{
	Boss.VertexWk[0].vtx.x = Boss.Pos.x - cosf(Boss.BaseAngle + Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[0].vtx.y = Boss.Pos.y - sinf(Boss.BaseAngle + Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[1].vtx.x = Boss.Pos.x + cosf(Boss.BaseAngle - Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[1].vtx.y = Boss.Pos.y - sinf(Boss.BaseAngle - Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[2].vtx.x = Boss.Pos.x - cosf(Boss.BaseAngle - Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[2].vtx.y = Boss.Pos.y + sinf(Boss.BaseAngle - Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[3].vtx.x = Boss.Pos.x + cosf(Boss.BaseAngle + Boss.Rot.z) * Boss.Radius;
	Boss.VertexWk[3].vtx.y = Boss.Pos.y + sinf(Boss.BaseAngle + Boss.Rot.z) * Boss.Radius;

	return;
}

//=============================================================================
// �{�X�̏����擾����
//=============================================================================
BOSS *GetBoss(void)
{
	return &Boss;
}

//=============================================================================
// �{�X�̐ݒu
//=============================================================================
void SetBoss(void)
{
	int Enemy_No = 0;
	int Barrage_No = 0;
	int Bullet_No = 0;
	int GameStage = GetGameStage();
	int GameDifficulty = GetGameDiffuculty();
	int PraticeSelect = GetPraticeSelect();
	D3DXVECTOR2 temp = D3DXVECTOR2(0.0f, 0.0f);
	ENEMY *Enemy = GetEnemy(Enemy_No);
	BARRAGE *Barrage = GetBarrage(Barrage_No);

	// �G�l�~�[����
	Enemy = GetEnemy(0);
	for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++, Enemy++)
	{
		if (Enemy->Exist == true)
		{
			Enemy->Exist = false;
			SetEnemyDeadEffect(Enemy_No);
		}
	}

	// �G�l�~�[�̒e����
	Barrage = GetBarrage(0);
	for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++, Barrage++)
	{
		if (Barrage->Phase != BarrageOver)
		{
			for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
			{
				if (Barrage->Bullet[Bullet_No].Use == true)
				{
					Barrage->Bullet[Bullet_No].Use = false;
					SetBulletDeadEffect(Barrage->Bullet[Bullet_No].Pos);
				}
			}
			Barrage->Phase = BarrageOver;
		}
	}

	temp = D3DXVECTOR2(Texture_Boss_Width / 2, Texture_Boss_Height / 2);

	Boss.Exist = true;
	Boss.DebutMove = true;
	Boss.BombDamage = false;
	if (Boss.MiddleBossOver != true)
	{
		Boss.MiddleBossOver = false;
	}
	Boss.Pos = D3DXVECTOR3(Screen_Width, ScreenCenter_Y, 0.0f);
	Boss.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Boss.BaseAngle = atan2f(Texture_Boss_Height, Texture_Boss_Width);
	Boss.MoveAngle = 0.0f;
	Boss.ShotAngle = 0.0f;
	Boss.Radius = D3DXVec2Length(&temp);
	Boss.HitRadius = Boss.Radius / 2.0f;
	Boss.AnimPattern = 0;
	Boss.ShotCount = 0;
	Boss.MovePattern = 0;
	Boss.Count = 0;
	Boss.Color = 0;
	Boss.HP_Max = BossStartHP;
	Boss.HP = Boss.HP_Max;
	// �Q�[���{��
	if (GameStage == Stage_Game)
	{
		// ���{�X�ݒu
		if (Boss.MiddleBossOver == false)
		{
			switch (GameDifficulty)
			{
			case Easy:
				Boss.Phase = MiddleBoss_Phase1;
				Boss.Life = 0;
				break;
			case Normal:
				Boss.Phase = MiddleBoss_Phase1;
				Boss.Life = 1;
				break;
			case Hard:
				Boss.Phase = MiddleBoss_Phase1;
				Boss.Life = 1;
				break;
			default:
				break;
			}
		}
		// ���X�{�X�ݒu
		else if (Boss.MiddleBossOver == true)
		{
			switch (GameDifficulty)
			{
			case Easy:
				Boss.Phase = LastBoss_Phase1;
				Boss.Life = 1;
				break;
			case Normal:
				Boss.Phase = LastBoss_Phase1;
				Boss.Life = 2;
				break;
			case Hard:
				Boss.Phase = LastBoss_Phase1;
				Boss.Life = 3;
				break;
			default:
				break;
			}
		}
		//if (Boss.State == BossDisappear)
		//{
		//	Boss.Phase = LastBoss_Phase1;
		//	Boss.Life = 3;
		//}
		//else
		//{
		//	Boss.Phase = MiddleBoss_Phase1;
		//	Boss.Life = 1;
		//}
	}
	// ���K�e���I��
	else if (GameStage == Stage_PraticeSelect)
	{
		Boss.Life = 0;
	}
	// �{�X���K���[�h
	else if (GameStage == Stage_BossPratice)
	{
		if (PraticeSelect != BossRush)
		{
			Boss.Life = 0;
		}
		else
		{
			Boss.Life = 5;
		}
	}
	Boss.BulletType = 0;
	Boss.State = StandBy;
	for (int i = 0; i < Boss.Life; i++)
	{
		BossLife[i].Use = true;
	}

	if (GameStage != Stage_PraticeSelect)
	{
		BossMove(D3DXVECTOR3(Screen_Width * 0.75, ScreenCenter_Y, 0.0f), 60.0f);

		SetBossEffect();
	}

#ifdef _DEBUG
	SetBossHC(Boss.Pos, Boss.HitRadius, BossHC);
#endif

	return;
}

//=============================================================================
// �{�X�̈ړ��ʒu�ݒu
//=============================================================================
void BossMove(D3DXVECTOR3 DestinationPos, float MoveTime)
{
	D3DXVECTOR2 Distance = D3DXVECTOR2(0.0f, 0.0f);

	Boss.InMove = true;
	Boss.BeforeMovePos = Boss.Pos;
	Boss.MoveCount = 0;
	Boss.MoveTime = MoveTime;
	Boss.Destination = D3DXVECTOR3(DestinationPos.x, DestinationPos.y, DestinationPos.z);
	Distance.x = Boss.Destination.x - Boss.Pos.x;
	Distance.y = Boss.Destination.y - Boss.Pos.y;
	Boss.Velocity.x = 2 * Distance.x / Boss.MoveTime;
	Boss.Velocity.y = 2 * Distance.y / Boss.MoveTime;
	Boss.Acceleration.x = -2 * Distance.x / (Boss.MoveTime * Boss.MoveTime);
	Boss.Acceleration.y = -2 * Distance.y / (Boss.MoveTime * Boss.MoveTime);

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeBossLifeVertex(void)
{
	int i = 0;

	// ���_���W�̐ݒ�	
	SetBossLifeVertex();

	// rhw�̐ݒ�
	for (i = 0; i < BossLife_Max; i++)
	{
		BossLife[i].VertexWk[0].rhw = 1.0f;
		BossLife[i].VertexWk[1].rhw = 1.0f;
		BossLife[i].VertexWk[2].rhw = 1.0f;
		BossLife[i].VertexWk[3].rhw = 1.0f;
	}

	// ���ˌ��̐ݒ�
	for (i = 0; i < BossLife_Max; i++)
	{
		BossLife[i].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		BossLife[i].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		BossLife[i].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		BossLife[i].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// �e�N�X�`�����W�̐ݒ�
	SetBossLifeTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetBossLifeTexture(void)
{
	for (int i = 0; i < Boss.Life; i++)
	{
		BossLife[i].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		BossLife[i].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		BossLife[i].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		BossLife[i].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetBossLifeVertex(void)
{
	for (int i = 0; i < Boss.Life; i++)
	{
		BossLife[i].VertexWk[0].vtx =
			D3DXVECTOR3(BossLifePos_X, BossLifePos_Y + (float)Texture_BossLife_Height * i, 0);
		BossLife[i].VertexWk[1].vtx =
			D3DXVECTOR3(BossLifePos_X + Texture_BossLife_Width, BossLifePos_Y + (float)Texture_BossLife_Height * i, 0);
		BossLife[i].VertexWk[2].vtx =
			D3DXVECTOR3(BossLifePos_X, BossLifePos_Y + Texture_BossLife_Height + (float)Texture_BossLife_Height * i, 0);
		BossLife[i].VertexWk[3].vtx =
			D3DXVECTOR3(BossLifePos_X + Texture_BossLife_Width, BossLifePos_Y + Texture_BossLife_Height + (float)Texture_BossLife_Height * i, 0);
	}

	return;
}
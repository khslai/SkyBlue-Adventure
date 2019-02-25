//=============================================================================
//
// �����蔻�菈�� [CheckHit.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "CheckHit.h"
#include "Player.h"
#include "Enemy.h"
#include "Barrage.h"
#include "Laser.h"
#include "Score.h"
#include "Effect.h"
#include "Bomb.h"
#include "Boss.h"
#include "Item.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �Q�[����Փx�W��
#define DifficultyRate (0.6f)
// �U�����鎞�̃X�R�A
#define AttackScore (490)
// �{�����e���폜���鎞�̃X�R�A
#define ClearBulletScore (90)
// �G�l�~�[��|�����̃X�R�A
#define KillEnemyScore (9990)
// �{�X��|�����̃X�R�A
#define KillBossScore (1000000)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// CP = Cross Product(�O��)
bool CheckHitCP(D3DXVECTOR3 Object, float ObjectRadius, D3DXVECTOR3 Bullet, D3DXVECTOR3 BulletPrePos);
// BC = �o�E���f�B���O�T�[�N��
bool CheckHitBC(float CheckRange, D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, D3DXVECTOR3 BulletPrePos, float BulletSpeed);
// CR = �T�[�N�� �� ��`(��]����)
bool CheckHitCR(D3DXVECTOR3 RotateCenter, float MoveAngle, D3DXVECTOR3 Object, float ObjectRadius);
// �e�������`�F�b�N����
bool CheckBulletDisappear(D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, float BulletRadius);
// �v���C���[�������ꂽ����
void PlayerEvent(void);
// �G�l�~�[���S����
void EnemyDeadEvent(int Enemy_No);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// ���[�U�[�̔���͈́i��`�j
RECT Rect;

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	int Barrage_No = 0;
	int Laser_No = 0;
	int Bullet_No = 0;
	int BombBullet_No = 0;
	int Enemy_No = 0;
	int Item_No = 0;
	int GameDifficulty = GetGameDiffuculty();
	static bool AddBossScore = false;
	PLAYER *Player = GetPlayer(0);
	ENEMY *Enemy = GetEnemy(Enemy_No);
	BARRAGE *Barrage = GetBarrage(Barrage_No);
	LASER *Laser = GetLaser(Laser_No);
	ITEM *Item = GetItem(Item_No);
	BOMB *Bomb = GetBomb();
	BOSS *Boss = GetBoss();

	// �v���C���[�̒e�ƃG�l�~�[�A�{�X
	for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
	{
		if (Player->Bullet[Bullet_No].Use == true)
		{
			// �G�l�~�[
			if (Boss->Exist == false)
			{
				// �G�l�~�[�̃|�C���^�[��������
				Enemy_No = 0;
				Enemy = GetEnemy(Enemy_No);
				for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++, Enemy++)
				{
					if (Enemy->Exist == true)
					{
						if (CheckHitBC((Enemy->HitRadius + Player->Bullet[Bullet_No].HitRadius),
							Enemy->Pos,
							Player->Bullet[Bullet_No].Pos,
							Player->Bullet[Bullet_No].PreviousPos,
							Player->Bullet[Bullet_No].Speed) == true)
						{
							if (CheckBulletDisappear(Enemy->Pos,
								Player->Bullet[Bullet_No].Pos,
								Player->Bullet[Bullet_No].HitRadius) == true)
							{
								// 1�t���C����e��������
								Player->Bullet[Bullet_No].DelayDisappear = true;
							}
							else
							{
								// ���̃t���C���ɒe��������
								Player->Bullet[Bullet_No].Use = false;
							}
							// �X�R�A�㏸
							AddScore(AttackScore);
							// �G�l�~�[HP����
							if (GameDifficulty == Easy)
							{
								Enemy->HP -= Player->Bullet[Bullet_No].Damage * DifficultyRate;
							}
							else
							{
								Enemy->HP -= Player->Bullet[Bullet_No].Damage;
							}
							// ��e�G�t�F�N�g
							SetEnemyColor(Enemy_No, Red);
							// �G�l�~�[���S
							if (Enemy->HP <= 0)
							{
								EnemyDeadEvent(Enemy_No);
							}
							break;
						}
					}
				}
			}
			// �{�X
			else if (Boss->Exist == true)
			{
				if (CheckHitBC((Boss->HitRadius + Player->Bullet[Bullet_No].HitRadius),
					Boss->Pos,
					Player->Bullet[Bullet_No].Pos,
					Player->Bullet[Bullet_No].PreviousPos,
					Player->Bullet[Bullet_No].Speed) == true)
				{
					if (CheckBulletDisappear(Enemy->Pos,
						Player->Bullet[Bullet_No].Pos,
						Player->Bullet[Bullet_No].HitRadius) == true)
					{
						// 1�t���C����e��������
						Player->Bullet[Bullet_No].DelayDisappear = true;
					}
					else
					{
						// ���̃t���C���ɒe��������
						Player->Bullet[Bullet_No].Use = false;
					}
					if (Boss->State != HPRecover)
					{
						AddScore(AttackScore);
						Boss->HP -= Player->Bullet[Bullet_No].Damage;
						if (Boss->HP <= 0)
						{
							Boss->HP = 0;
							if (AddBossScore == false)
							{
								AddScore(KillBossScore);
								AddBossScore = true;
							}
						}
						else
						{
							AddBossScore = false;
						}
					}
					SetBossColor(Red);
				}
			}
		}
	}

	// �v���C���[�̃{���ƃG�l�~�[�A�e
	if (Bomb->Use == true && Player->Type == Player_A)
	{
		if (Boss->Exist == false)
		{
			// �G�l�~�[
			// �G�l�~�[�̃|�C���^�[��������
			Enemy_No = 0;
			Enemy = GetEnemy(Enemy_No);
			for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++, Enemy++)
			{
				if (Enemy->Exist == true)
				{
					if (CheckHitBC((Enemy->HitRadius + Bomb->HitRadius),
						Enemy->Pos,
						Bomb->Pos,
						Bomb->Pos,
						0.0f) == true)
					{
						Enemy->HP = 0;
						EnemyDeadEvent(Enemy_No);
					}
				}
			}
			// �G�l�~�[�̒e
			// �e���̃|�C���^�[��������
			Barrage_No = 0;
			Barrage = GetBarrage(Barrage_No);
			for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++, Barrage++)
			{
				if (Barrage->Phase != BarrageOver)
				{
					for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
					{
						if (Barrage->Bullet[Bullet_No].Use == true)
						{
							if (CheckHitBC((Barrage->Bullet[Bullet_No].HitRadius + Bomb->HitRadius),
								Barrage->Bullet[Bullet_No].Pos,
								Bomb->Pos,
								Bomb->Pos,
								0.0f) == true)
							{
								AddScore(ClearBulletScore);
								Barrage->Bullet[Bullet_No].Use = false;
								SetBulletDeadEffect(Barrage->Bullet[Bullet_No].Pos);
							}
						}
					}
				}
			}
		}
		else if (Boss->Exist == true)
		{
			// �{�X
			if (Boss->BombDamage == false &&
				CheckHitBC((Boss->HitRadius + Bomb->HitRadius),
					Boss->Pos, Bomb->Pos, Bomb->Pos, 0) == true)
			{
				if (Player->Power <= 1.0f)
				{
					Boss->HP -= 500.0f;
				}
				else
				{
					Boss->HP -= Player->Power * 500.0f;
				}
				Boss->BombDamage = true;
				if (Boss->HP <= 0)
				{
					Boss->HP = 0;
					if (AddBossScore == false)
					{
						AddScore(KillBossScore);
						AddBossScore = true;
					}
				}
				else
				{
					AddBossScore = false;
				}
			}
			// �{�X�̒e
			for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
			{
				if (Boss->Bullet[Bullet_No].Use == true)
				{
					if (CheckHitBC((Boss->Bullet[Bullet_No].HitRadius + Bomb->HitRadius),
						Boss->Bullet[Bullet_No].Pos,
						Bomb->Pos,
						Bomb->Pos,
						0.0f) == true)
					{
						AddScore(ClearBulletScore);
						Boss->Bullet[Bullet_No].Use = false;
						SetBulletDeadEffect(Boss->Bullet[Bullet_No].Pos);
					}
				}
			}
			// �{�X�̃��[�U�[
			Laser_No = 0;
			Laser = GetLaser(Laser_No);
			for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++, Laser++)
			{
				if (Laser->Use == true)
				{
					if (CheckHitBC((1.0f + Bomb->HitRadius),
						Laser->Pos,
						Bomb->Pos,
						Bomb->Pos,
						0) == true)
					{
						AddScore(ClearBulletScore);
						Laser->Use = false;
						SetBulletDeadEffect(Laser->Pos);
					}
				}
			}
		}
	}

	// �{���̒e�ƃG�l�~�[
	if (Bomb->Use == true && Player->Type == Player_B)
	{
		for (BombBullet_No = 0; BombBullet_No < BombBullet_Max; BombBullet_No++)
		{
			if (Bomb->Bullet[BombBullet_No].Use == true)
			{
				// �e�ƃG�l�~�[
				if (Boss->Exist == false)
				{
					Enemy = GetEnemy(Bomb->BulletTarget[BombBullet_No]);
					if (CheckHitBC((Bomb->Bullet[BombBullet_No].HitRadius + Enemy->HitRadius),
						Enemy->Pos,
						Bomb->Bullet[BombBullet_No].Pos,
						Bomb->Bullet[BombBullet_No].PreviousPos,
						0.0f) == true)
					{
						// ���e���ʉ�
						StopSound(SE_ShotBombBullet);
						PlaySound(SE_BombBulletExplosion, E_DS8_FLAG_NONE, true);
						Enemy->HP = 0;
						EnemyDeadEvent(Bomb->BulletTarget[BombBullet_No]);
						Bomb->Bullet[BombBullet_No].Use = false;
						Bomb->LockOn[BombBullet_No].Use = false;
						Bomb->Bullet[BombBullet_No].State = Explosion;
					}
				}
				// �e�ƃ{�X
				else
				{
					if (CheckHitBC((Bomb->Bullet[BombBullet_No].HitRadius + Boss->HitRadius),
						Boss->Pos,
						Bomb->Bullet[BombBullet_No].Pos,
						Bomb->Bullet[BombBullet_No].PreviousPos,
						0.0f) == true)
					{
						// ���e���ʉ�
						StopSound(SE_ShotBombBullet);
						PlaySound(SE_BombBulletExplosion, E_DS8_FLAG_NONE, true);
						Boss->BombDamage = true;
						if (Player->Power <= 1.0f)
						{
							Boss->HP -= 50.0f;
						}
						else
						{
							Boss->HP -= 50.0f * Player->Power;
						}
						if (Boss->HP <= 0)
						{
							Boss->HP = 0;
							if (AddBossScore == false)
							{
								AddScore(KillBossScore);
								AddBossScore = true;
							}
						}
						else
						{
							AddBossScore = false;
						}
						Bomb->Bullet[BombBullet_No].Use = false;
						Bomb->Bullet[BombBullet_No].State = Explosion;
						Bomb->LockOn[BombBullet_No].Use = false;
					}
				}
			}
			// �Ռ��g�ƒe
			else if (Bomb->Bullet[BombBullet_No].State == Explosion)
			{
				if (Boss->Exist == false)
				{
					// �G�l�~�[�̒e
					// �e���̃|�C���^�[��������
					Barrage_No = 0;
					Barrage = GetBarrage(Barrage_No);
					for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++, Barrage++)
					{
						if (Barrage->Phase != BarrageOver)
						{
							for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
							{
								if (Barrage->Bullet[Bullet_No].Use == true)
								{
									if (CheckHitBC(
										(Barrage->Bullet[Bullet_No].HitRadius + Bomb->ExplosionRadius[BombBullet_No]),
										Barrage->Bullet[Bullet_No].Pos,
										Bomb->Bullet[BombBullet_No].Pos,
										Bomb->Bullet[BombBullet_No].Pos,
										0.0f) == true)
									{
										AddScore(ClearBulletScore);
										Barrage->Bullet[Bullet_No].Use = false;
										SetBulletDeadEffect(Barrage->Bullet[Bullet_No].Pos);
									}
								}
							}
						}
					}
				}
				else
				{
					// �{�X�̒e
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						if (Boss->Bullet[Bullet_No].Use == true)
						{
							if (CheckHitBC((Boss->Bullet[Bullet_No].HitRadius + Bomb->ExplosionRadius[BombBullet_No]),
								Boss->Bullet[Bullet_No].Pos,
								Bomb->Bullet[BombBullet_No].Pos,
								Bomb->Bullet[BombBullet_No].Pos,
								0.0f) == true)
							{
								AddScore(ClearBulletScore);
								Boss->Bullet[Bullet_No].Use = false;
								SetBulletDeadEffect(Boss->Bullet[Bullet_No].Pos);
							}
						}
					}
					// �{�X�̃��[�U�[
					Laser_No = 0;
					Laser = GetLaser(Laser_No);
					for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++, Laser++)
					{
						if (Laser->Use == true)
						{
							if (CheckHitBC((1.0f + Bomb->ExplosionRadius[BombBullet_No]),
								Laser->Pos,
								Bomb->Bullet[BombBullet_No].Pos,
								Bomb->Bullet[BombBullet_No].Pos,
								0.0f) == true)
							{
								AddScore(ClearBulletScore);
								Laser->Use = false;
								SetBulletDeadEffect(Laser->Pos);
							}
						}
					}
				}
			}
		}
	}

	// �G�l�~�[�̒e�ƃv���C���[
	if (Boss->Exist == false)
	{
		// �e���|�C���^��������
		Barrage_No = 0;
		Barrage = GetBarrage(Barrage_No);
		for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++, Barrage++)
		{
			if (Barrage->Phase != BarrageOver)
			{
				for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
				{
					if (Barrage->Bullet[Bullet_No].Use == true)
					{
						if (CheckHitBC((Player->HitRadius + Barrage->Bullet[Bullet_No].HitRadius),
							Player->Pos,
							Barrage->Bullet[Bullet_No].Pos,
							Barrage->Bullet[Bullet_No].PreviousPos,
							Barrage->Bullet[Bullet_No].Speed) == true)
						{
							Barrage->Bullet[Bullet_No].Use = false;
							if (Player->InBomb == false)
							{
								if (Player->InvincibleCount == 0)
								{
									PlayerEvent();
								}
							}
						}
					}
				}
			}
		}
	}
	// �{�X�̒e�ƃv���C���[
	else if (Boss->Exist == true)
	{
		for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				if (CheckHitBC((Player->HitRadius + Boss->Bullet[Bullet_No].HitRadius),
					Player->Pos,
					Boss->Bullet[Bullet_No].Pos,
					Boss->Bullet[Bullet_No].PreviousPos,
					Boss->Bullet[Bullet_No].Speed) == true)
				{
					Boss->Bullet[Bullet_No].Use = false;
					if (Player->InBomb == false)
					{
						if (Player->InvincibleCount == 0)
						{
							PlayerEvent();
						}
					}
				}
			}
		}
	}

	// ���[�U�[�ƃv���C���[
	// ���[�U�[�|�C���^��������
	Laser_No = 0;
	Laser = GetLaser(Laser_No);
	for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++, Laser++)
	{
		if (Laser->Use == true && Laser->InRotate == false)
		{
			if (CheckHitCR(Laser->RotateCenterPos, Laser->MoveAngle, Player->Pos, Player->HitRadius) == true)
			{
				if (Player->InBomb == false)
				{
					if (Player->InvincibleCount == 0)
					{
						PlayerEvent();
					}
				}
			}
		}
	}

	// �A�C�e���ƃv���C���[
	// �A�C�e���|�C���^��������
	Item_No = 0;
	Item = GetItem(Item_No);
	for (Item_No = 0; Item_No < ItemNum_Max; Item_No++, Item++)
	{
		if (Item->Use == true)
		{
			if (CheckHitBC((Player->HitRadius + Item->Radius), Player->Pos, Item->Pos, Item->Pos, Item->Speed) == true)
			{
				Item->Use = false;
				// ���ʉ�
				PlaySound(SE_GetItem, E_DS8_FLAG_NONE, true);
				switch (Item->Type)
				{
				case HPItem:
					Player->HP += HPItemValue;
					if (Player->HP >= PlayerHP_Max)
					{
						Player->HP = PlayerHP_Max;
					}
					break;
				case ScoreItem:
					AddScore(ScoreItemValue);
					break;
				case PowerItem:
					Player->Power += PowerItemValue;
					if (Player->Power >= PlayerPower_Max)
					{
						Player->Power = PlayerPower_Max;
					}
					break;
				default:
					break;
				}
			}
		}
	}

	return;
}

//=============================================================================
// �����蔻��i�����Ɖ~�j
//=============================================================================
bool CheckHitCP(D3DXVECTOR3 Object, float ObjectRadius, D3DXVECTOR3 Bullet, D3DXVECTOR3 BulletPrePos)
{
	// �����蔻��̌���
	bool Result = false;
	// Vector_A = �e�Ɖ~�̒��S�̃x�N�g��
	VECTOR Vector_A;
	// Vector_B = �e��1�t���C���O�̒e�̃x�N�g��
	VECTOR Vector_B;
	// Vector_B�̒������v�Z
	D3DXVECTOR2 temp = D3DXVECTOR2(0.0f, 0.0f);
	// Object�̒��S�ƒe�̋O�Ղ̋���
	float Distance = 0.0f;
	// Object�̒��S�͒e�̈ړ��͈͓����ǂ����H
	bool InRange = false;


	if (Object.x >= BulletPrePos.x && Object.x < Bullet.x		// �e�͍�����E
		|| Object.x <= BulletPrePos.x && Object.x > Bullet.x)	// �e�͉E���獶
	{
		Vector_A.x = Bullet.x - Object.x;
		Vector_A.y = Bullet.y - Object.y;
		temp = D3DXVECTOR2(Bullet.x - BulletPrePos.x, Bullet.y - BulletPrePos.y);
		// Vector_B = �P�ʃx�N�g��
		Vector_B.Length = D3DXVec2Length(&temp);
		Vector_B.x = temp.x / Vector_B.Length;
		Vector_B.y = temp.y / Vector_B.Length;
		// Vector_A��Vector_B�̊O�όv�Z
		Distance = fabsf(Vector_A.x * Vector_B.y - Vector_B.x * Vector_A.y);
		Result = Distance <= ObjectRadius ? true : false;
	}

	return Result;
}

//=============================================================================
// �����蔻��i�~�Ɖ~�j
//=============================================================================
bool CheckHitBC(float CheckRange, D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, D3DXVECTOR3 BulletPrePos, float BulletSpeed)
{
	int i = 0;
	// �����蔻��̌���
	bool Result = false;
	// ���S�_�Ԃ̒������v�Z
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 BulletPostemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// Object�̒��S�ƒe�̒��S�̋���
	float Distance = 0.0f;

	// �e�̃X�s�[�h�͓�����͈͂��傫���Ȃ�
	if (BulletSpeed > CheckRange)
	{
		// ���̒e��1�t���C���O�̒e�̋������L�^����
		BulletPostemp = Bullet - BulletPrePos;
		// �e�̔�s���� / ������͈� = ���[�v��
		for (i = 0; i < BulletSpeed / CheckRange; i++)
		{
			// �ڕW�ƒe�̋������v�Z����
			temp = Object - BulletPrePos;
			Distance = D3DXVec3LengthSq(&temp);
			if (Distance <= pow(CheckRange, 2))
			{
				Result = true;
				break;
			}
			else
			{
				// �e��i��ł�A�i��ł鋗�� = �S���̔�s���� / ���[�v��
				BulletPrePos.x += BulletPostemp.x / (BulletSpeed / CheckRange);
				BulletPrePos.y += BulletPostemp.y / (BulletSpeed / CheckRange);
				Result = false;
			}
		}
	}
	else
	{
		temp = Object - Bullet;
		Distance = D3DXVec3LengthSq(&temp);
		Result = Distance <= pow(CheckRange, 2) ? true : false;
	}

	return Result;
}

//=============================================================================
// ���[�U�[�̔���͈́i��`�j��ݒu����
//=============================================================================
void SetLaserCheckRect(D3DXVECTOR3 Point_LeftUp, D3DXVECTOR3 Point_RightDown)
{
	Rect.left = (long)Point_LeftUp.x;
	Rect.top = (long)Point_LeftUp.y;
	Rect.right = (long)Point_RightDown.x;
	Rect.bottom = (long)Point_RightDown.y;
	return;
}

//=============================================================================
// �����蔻��i�~�Ƌ�`�j
//=============================================================================
bool CheckHitCR(D3DXVECTOR3 RotateCenter, float MoveAngle, D3DXVECTOR3 Object, float ObjectRadius)
{
	// �����蔻��̌���
	bool Result = false;
	// �~�Ƌ�`�̍ŒZ�����̍��W
	D3DXVECTOR3 ClosestPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// RotateCenter�𒆐S�ŋt��]��̍��W
	D3DXVECTOR3 ObjectPos_NoRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// ��̍��W�Ԃ̒������v�Z
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// Object�̒��S�ƒe�̒��S�̋���
	float Distance = 0.0f;

	// ���[�U�[�̊p�x�ɂ���āA�v���C���[�̍��W���t��]������
	ObjectPos_NoRot.x = (Object.x - RotateCenter.x) * cosf(-MoveAngle) - (Object.y - RotateCenter.y) * sinf(-MoveAngle) + RotateCenter.x;
	ObjectPos_NoRot.y = (Object.x - RotateCenter.x) * sinf(-MoveAngle) + (Object.y - RotateCenter.y) * cosf(-MoveAngle) + RotateCenter.y;

	// �v���C���[��x���W�͋�`�̍�����菬�����Ȃ�A�ŒZ������x���W = ��`�̍���
	if (ObjectPos_NoRot.x < Rect.left)
	{
		ClosestPoint.x = (float)Rect.left;
	}
	// �v���C���[��x���W�͋�`�̉E�����傫���Ȃ�A�ŒZ������x���W = ��`�̉E��
	else if (ObjectPos_NoRot.x > Rect.right)
	{
		ClosestPoint.x = (float)Rect.right;
	}
	// �ȏザ��Ȃ�������A�ŒZ������x���W = �v���C���[��x���W
	else
	{
		ClosestPoint.x = ObjectPos_NoRot.x;
	}

	// �v���C���[��y���W�͋�`�̏㑤��菬�����Ȃ�A�ŒZ������y���W = ��`�̏㑤
	if (ObjectPos_NoRot.y < Rect.top)
	{
		ClosestPoint.y = (float)Rect.top;
	}
	// �v���C���[��y���W�͋�`�̉������傫���Ȃ�A�ŒZ������y���W = ��`�̉���
	else if (ObjectPos_NoRot.y > Rect.bottom)
	{
		ClosestPoint.y = (float)Rect.bottom;
	}
	// �ȏザ��Ȃ�������A�ŒZ������y���W = �v���C���[��y���W
	else
	{
		ClosestPoint.y = ObjectPos_NoRot.y;
	}

	// �����v�Z
	temp = ObjectPos_NoRot - ClosestPoint;
	Distance = D3DXVec3LengthSq(&temp);
	// �����蔻��
	Result = Distance <= pow(ObjectRadius, 2) ? true : false;

	return Result;
}

//=============================================================================
// �e�������`�F�b�N����
//=============================================================================
bool CheckBulletDisappear(D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, float BulletRadius)
{
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool Result = false;
	float Distance = 0.0f;

	temp = Object - Bullet;
	Distance = D3DXVec3LengthSq(&temp);
	// �����蔻��
	// ���a * 2 = ���a
	Result = Distance >= pow((BulletRadius * 2), 2) ? true : false;

	return Result;
}

//=============================================================================
// �v���C���[�������ꂽ����
//=============================================================================
void PlayerEvent(void)
{
	int Bullet_No = 0;
	int Barrage_No = 0;
	int Laser_No = 0;
	int GameStage = GetGameStage();
	PLAYER *Player = GetPlayer(0);
	BARRAGE *Barrage = GetBarrage(Barrage_No);
	BOSS *Boss = GetBoss();
	LASER *Laser = GetLaser(Laser_No);

	if (Boss->Exist == false)
	{
		// �G�l�~�[�̒e
		Barrage_No = 0;
		Barrage = GetBarrage(Barrage_No);
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
			}
		}
	}
	else if (Boss->Exist == true)
	{
		// �{�X�̒e
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				Boss->Bullet[Bullet_No].Use = false;
				SetBulletDeadEffect(Boss->Bullet[Bullet_No].Pos);
			}
		}

		// �{�X�̃��[�U�[
		Laser_No = 0;
		Laser = GetLaser(Laser_No);
		for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++, Laser++)
		{
			if (Laser->Use == true)
			{
				Laser->Use = false;
			}
		}
	}

	// ���ʉ�
	PlaySound(SE_PlayerBeShot, E_DS8_FLAG_NONE, true);
	// ���G�J�E���g���Z
	Player->InvincibleCount++;
	if (GameStage == Stage_Game || GameStage == Stage_BossPratice)
	{
		// HP��2������
		Player->HP -= PlayerHP_Max * 0.2f;
		if (Player->HP <= 0.0f)
		{
			Player->HP = 0.0f;
		}

		// Power��0.5����
		Player->Power -= 0.5f;
		if (Player->Power <= 0.0f)
		{
			Player->Power = 0.0f;
		}

		// �{������1
		Player->BombNum++;
		if (Player->BombNum >= PlayerBomb_Max)
		{
			Player->BombNum = PlayerBomb_Max;
		}

		// �v���C���[�A�C�e���ݒu
		SetPlayerItem(Player->Pos);
	}

	return;
}


//=============================================================================
// �G�l�~�[���S����
//=============================================================================
void EnemyDeadEvent(int Enemy_No)
{
	int Barrage_No = 0;
	ENEMY *Enemy = GetEnemy(Enemy_No);
	BARRAGE *Barrage = GetBarrage(Barrage_No);

	// ���ʉ�
	PlaySound(SE_EnemyDead, E_DS8_FLAG_NONE, true);
	// �X�R�A���Z
	AddScore(KillEnemyScore);
	// �t���O����
	Enemy->Exist = false;
	// ���Ƃ��A�C�e���ݒu
	SetEnemyItem(Enemy->Pos, Enemy->Item, EnemyItem_Max);
	// ���S�G�t�F�N�g�ݒu
	SetEnemyDeadEffect(Enemy_No);
	// �e����Ԃ�ς��
	for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++, Barrage++)
	{
		if (Barrage->Phase != BarrageOver)
		{
			if (Barrage->Attacker_No == Enemy_No)
			{
				Barrage->Phase = EnemyDisappear;
				break;
			}
		}
	}

	return;
}
//=============================================================================
//
// �{������ [Bomb.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "Bomb.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Barrage.h"
#include "Sound.h"
#include "DebugTest.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �{��A�^�C�v�̊g�呬�x
#define ExtendSpeed_Phase1	(10.0f)
#define ExtendSpeed_Phase2	(200.0f)
// �{��A�^�C�v�̏����J�E���g
#define BombDisappearCount	(100)
// ���b�N�I���}�[�N�̊g��W��
#define LockOnZoomRate		(2.0f)
// �{��B�^�C�v�̒e�̈ړ����x
#define BombBulletSpeed		(8.0f)

// �{��A�^�C�v���
enum Bomb_A_Phase
{
	ShotBomb,		// �{������
	BombExtending,	// �g�咆
	ExtendOver,		// �g��I��
};

// �{��B�^�C�v�̏��
enum Bomb_B_Phase
{
	TargetLockOn,	// �^�[�Q�b�g�����b�N�I��
	Shot,			// �e����
	ShotOver,		// �{���I��
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeBombVertex(void);
HRESULT MakeLockOnVertex(int LockOn_No);
static HRESULT MakeBulletVertex(int Bullet_No);
// ���_���W�̐ݒ�
void SetBombVertex(void);
void SetLockOnVertex(int LockOn_No);
static void SetBulletVertex(int Bullet_No);
// �e�N�X�`�����W�̐ݒ�
void SetBombTexture(void);
void SetLockOnTexture(int LockOn_No);
static void SetBulletTexture(int Bullet_No);
// �{��A�^�C�v�̓����x��ݒu����
void SetBombDiffuse(int Alpha);
// �{��B�^�C�v�̒e�𔭎˂���
void ShotBombBullet(void);
// �{��B�^�C�v�̒e���������鎞�̏Ռ��g��`�悷��
void DrawImpactWave(D3DXVECTOR3 Center, float Radius, D3DCOLOR Color);
// �{��A�^�C�v�̊g��͈͂��m�F����
void CheckVertex(VERTEX_2D *Vtx);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 ShotBombTexture = NULL;
LPDIRECT3DTEXTURE9 BombExtendTexture = NULL;
LPDIRECT3DTEXTURE9 WhiteScreenTexture = NULL;
LPDIRECT3DTEXTURE9 LockOnTexture = NULL;
LPDIRECT3DTEXTURE9 BombBulletTexture = NULL;
// �{��A�^�C�v�̊g��͈͂�\�����钸�_
VERTEX_2D BombExtendPixel[TextureDevideNum][TextureDevideNum][Num_Vertex];
// �e�N�X�`�����a
float LockOnRadius = 0.0f;
float ShotBombRadius = 0.0f;
float BombExtendRadius = 0.0f;
// ���S�_�Ǝl���_�̐����p
float LockOnBaseAngle = 0.0f;
float ShotBombBaseAngle = 0.0f;
float BombExtendBaseAngle = 0.0f;
// �{���\����
BOMB Bomb;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBomb(int InitState)
{
	int Bullet_No = 0;
	D3DXVECTOR2 temp = D3DXVECTOR2(0.0f, 0.0f);

	Bomb.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Bomb.ShakePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Bomb.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Bomb.Radius = 0.0f;
	Bomb.HitRadius = 0.0f;
	Bomb.BaseAngle = 0.0f;
	Bomb.LockOnNum = 0;
	Bomb.AnimPattern = 0;
	Bomb.Count = 0;
	Bomb.Alpha = 255;
	Bomb.Phase = ShotBomb;
	Bomb.Use = false;

	// ���_���̍쐬
	MakeBombVertex();

	for (int LockOn_No = 0; LockOn_No < BombBullet_Max; LockOn_No++)
	{
		Bomb.LockOn[LockOn_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.LockOn[LockOn_No].Radius = 0.0f;
		Bomb.LockOn[LockOn_No].BaseAngle = 0.0f;
		Bomb.LockOn[LockOn_No].Target_No = 0;
		Bomb.LockOn[LockOn_No].Use = false;

		// ���_���̍쐬
		MakeLockOnVertex(LockOn_No);
	}

	for (Bullet_No = 0; Bullet_No < BombBullet_Max; Bullet_No++)
	{
		Bomb.Bullet[Bullet_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.Bullet[Bullet_No].PreviousPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.Bullet[Bullet_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.Bullet[Bullet_No].Radius = 0.0f;
		Bomb.Bullet[Bullet_No].HitRadius = 0.0f;
		Bomb.Bullet[Bullet_No].BaseAngle = 0.0f;
		Bomb.Bullet[Bullet_No].MoveAngle = 0.0f;
		Bomb.Bullet[Bullet_No].Speed = 0.0f;
		Bomb.Bullet[Bullet_No].Damage = 0;
		Bomb.Bullet[Bullet_No].Type = 0;
		Bomb.Bullet[Bullet_No].Count = 0;
		Bomb.Bullet[Bullet_No].Color = 0;
		Bomb.Bullet[Bullet_No].State = 0;
		Bomb.Bullet[Bullet_No].Use = false;
		Bomb.Bullet[Bullet_No].DelayDisappear = false;
		Bomb.BulletTarget[Bullet_No] = 0;
		Bomb.ExplosionRadius[Bullet_No] = 0.0f;

		// ���_���̍쐬
		MakeBulletVertex(Bullet_No);
	}

	temp = D3DXVECTOR2(Texture_LockOn_Width / 2, Texture_LockOn_Height / 2);
	LockOnRadius = D3DXVec2Length(&temp);
	LockOnBaseAngle = atan2f(Texture_LockOn_Height, Texture_LockOn_Width);

	temp = D3DXVECTOR2(Texture_ShotBomb_Width / 2, Texture_ShotBomb_Height / 2);
	ShotBombRadius = D3DXVec2Length(&temp);
	ShotBombBaseAngle = atan2f(Texture_ShotBomb_Height, Texture_ShotBomb_Width);

	temp = D3DXVECTOR2(Texture_BombExtend_Width / 2, Texture_BombExtend_Height / 2);
	BombExtendRadius = D3DXVec2Length(&temp);
	BombExtendBaseAngle = atan2f(Texture_BombExtend_Height, Texture_BombExtend_Width);

	if (InitState == FirstInit)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_ShotBomb, &ShotBombTexture, "Bomb") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BombExtend, &BombExtendTexture, "Bomb") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_WhiteScreen, &WhiteScreenTexture, "Bomb") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_LockOn, &LockOnTexture, "Bomb") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BombBullet, &BombBulletTexture, "Bomb") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBomb(void)
{
	SafeRelease(ShotBombTexture);
	SafeRelease(BombExtendTexture);
	SafeRelease(WhiteScreenTexture);
	SafeRelease(LockOnTexture);
	SafeRelease(BombBulletTexture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBomb(void)
{
	int i = 0, j = 0;
	int LockOn_No = 0;
	int Bullet_No = 0;
	// ���I�������e�̐�
	int BulletOverNum = 0;
	PLAYER *Player = GetPlayer(0);
	ENEMY *Enemy = GetEnemy(0);
	BOSS *Boss = GetBoss();
	BULLETTYPE *BulletType = GetBulletType(BombBulletType);

	if (Bomb.Use == true)
	{
		Bomb.Count++;
		// A�^�C�v
		if (Player->Type == Player_A)
		{
			// ���˒��A�g�傪�܂��n�܂�Ȃ�
			if (Bomb.Phase == ShotBomb)
			{
				if (IsPlaying(SE_ShotBomb) == false)
				{
					PlaySound(SE_ShotBomb, E_DS8_FLAG_NONE, true);
				}
				if (Bomb.Count % (30 / Texture_ShotBomb_Divide_X) == 0)
				{
					Bomb.AnimPattern = (Bomb.AnimPattern + 1) % Texture_ShotBomb_Divide_X;
				}
				// ���ˏI���A�{���g��J�n
				if (Bomb.Count > 30)
				{
					PlaySound(SE_BombExtend, E_DS8_FLAG_NONE, true);
					Bomb.Phase = BombExtending;
					Bomb.Pos.x += 16.0f;
					Bomb.Radius = BombExtendRadius;
					Bomb.HitRadius = BombExtendRadius / Root2;
					Bomb.BaseAngle = BombExtendBaseAngle;
					Bomb.Alpha = 128;
					Bomb.Count = 0;
#ifdef _DEBUG
					SetBombHC(Bomb.Pos, Bomb.HitRadius, BombHC);
#endif
				}
			}
			// �{���g�咆
			else if (Bomb.Phase == BombExtending)
			{
				if (Bomb.Count % 4 == 0)
				{
					if (Bomb.Count <= 60)
					{
						// 45�x�� 1 : 1 : Root2�A������Εӂ���"1"�̎��A��ӂ͂���"1 / Root2"
						Bomb.Radius += ExtendSpeed_Phase1;
						Bomb.HitRadius += ExtendSpeed_Phase1 / Root2;
						Bomb.Alpha += 5;
					}
					else
					{
						Bomb.Radius += ExtendSpeed_Phase2;
						Bomb.HitRadius += ExtendSpeed_Phase2 / Root2;
						Bomb.Alpha += 10;
						if (Bomb.Alpha >= 200)
						{
							Bomb.Alpha = 200;
						}
					}
				}
			}
			// �{���g��I���A��ʗh���
			else if (Bomb.Phase == ExtendOver)
			{
				if (IsPlaying(SE_BombExplosion) == false)
				{
					PlaySound(SE_BombExplosion, E_DS8_FLAG_NONE, true);
				}
				Bomb.ShakePos.x = (float)(rand() % (30 + 1) - 15);
				Bomb.ShakePos.y = (float)(rand() % (30 + 1) - 15);
				if (Bomb.Count > BombDisappearCount)
				{
					Bomb.Use = false;
					Player->InBomb = false;
					if (Boss->Exist == true)
					{
						Boss->BombDamage = false;
					}
				}
			}
			// �e�N�X�`���X�V����
			SetBombTexture();
			SetBombDiffuse(Bomb.Alpha);
			SetBombVertex();
		}
		// B�^�C�v
		else if (Player->Type == Player_B)
		{
			// ���b�N�I���ݒu
			for (LockOn_No = 0; LockOn_No < BombBullet_Max; LockOn_No++)
			{
				if (Bomb.LockOn[LockOn_No].Use == true)
				{
					Enemy = GetEnemy(Bomb.LockOn[LockOn_No].Target_No);

					// �G�l�~�[(�{�X)�̍��W�ƈꏏ�Ɉړ�
					if (Enemy->Exist == true)
					{
						Bomb.LockOn[LockOn_No].Pos = Enemy->Pos;
					}
					else if (Boss->Exist == true)
					{
						Bomb.LockOn[LockOn_No].Pos = Boss->Pos;
					}
					else
					{
						Bomb.LockOn[LockOn_No].Use = false;
						continue;
					}

					// �ǂ�ǂ񏬂����Ȃ�
					if (Bomb.LockOn[LockOn_No].Radius > LockOnRadius)
					{
						Bomb.LockOn[LockOn_No].Radius -= 3.0f;
					}
					else
					{
						Bomb.LockOn[LockOn_No].Radius = LockOnRadius;
					}

					// �e�N�X�`���X�V
					SetLockOnTexture(LockOn_No);
					SetLockOnVertex(LockOn_No);
				}
			}

			// ������x�Ȃ�����A�e����
			if (Bomb.Phase == TargetLockOn && Bomb.Count >= 30.0f)
			{
				Bomb.Phase = Shot;
				StopSound(SE_LockOn);
			}
			// �e����
			else if (Bomb.Phase == Shot)
			{
				ShotBombBullet();
				PlaySound(SE_ShotBombBullet, E_DS8_FLAG_NONE, true);
				Bomb.Phase = ShotOver;
			}
			// ���ˏI���A�ړ��v�Z
			else if (Bomb.Phase == ShotOver)
			{
				for (Bullet_No = 0; Bullet_No < BombBullet_Max; Bullet_No++)
				{
					if (Bomb.Bullet[Bullet_No].Use == true)
					{
						Enemy = GetEnemy(Bomb.BulletTarget[Bullet_No]);

						// �ڕW���݁A�e�ړ��v�Z
						if (Enemy->Exist == true || Boss->Exist == true && Boss->HP > 0.0f)
						{
							Bomb.Bullet[Bullet_No].PreviousPos = Bomb.Bullet[Bullet_No].Pos;
							// �G�l�~�[
							if (Boss->Exist == false)
							{
								Bomb.Bullet[Bullet_No].MoveAngle =
									AngleCalculate(Bomb.Bullet[Bullet_No].Pos, Enemy->Pos);
							}
							// �{�X
							else if (Boss->Exist == true)
							{
								Bomb.Bullet[Bullet_No].MoveAngle =
									AngleCalculate(Bomb.Bullet[Bullet_No].Pos, Boss->Pos);
							}
							Bomb.Bullet[Bullet_No].Rot.z = Bomb.Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
							Bomb.Bullet[Bullet_No].Pos.x +=
								cosf(Bomb.Bullet[Bullet_No].MoveAngle) * Bomb.Bullet[Bullet_No].Speed;
							Bomb.Bullet[Bullet_No].Pos.y +=
								sinf(Bomb.Bullet[Bullet_No].MoveAngle) * Bomb.Bullet[Bullet_No].Speed;
							Bomb.Bullet[Bullet_No].Count++;

							// �ړ��ʒu�`�F�b�N
							if (CheckRange(OutOfRange, Bomb.Bullet[Bullet_No].Pos,
								BulletType->Width, BulletType->Height) == true)
							{
								Bomb.Bullet[Bullet_No].Use = false;
								Bomb.LockOn[Bullet_No].Use = false;
								Bomb.Bullet[Bullet_No].State = BulletDisspappear;
							}

							// �e�N�X�`���X�V
							SetBulletTexture(Bullet_No);
							SetBulletVertex(Bullet_No);
						}
						// �ڕW������A�e�ƃ��b�N�I������
						else
						{
							Bomb.Bullet[Bullet_No].Use = false;
							Bomb.LockOn[Bullet_No].Use = false;
							Bomb.Bullet[Bullet_No].State = BulletDisspappear;
						}
					}
					// �e�����A�Ռ��g�`��
					else if (Bomb.Bullet[Bullet_No].State == Explosion)
					{
						Bomb.ExplosionRadius[Bullet_No] += 30.0f;
						// ������x�L��������A�I��
						if (Bomb.ExplosionRadius[Bullet_No] >= Screen_Width)
						{
							Bomb.Bullet[Bullet_No].State = Over;
							BulletOverNum++;
						}
					}
					else if (Bomb.Bullet[Bullet_No].Use == false)
					{
						BulletOverNum++;
					}
					// �S���̒e�I�� -> �{���I��
					if (BulletOverNum == BombBullet_Max)
					{
						Bomb.Use = false;
						Player->InBomb = false;
						if (Boss->Exist == true)
						{
							Boss->BombDamage = false;
						}
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
void DrawBomb(void)
{
	int i = 0, j = 0;
	int LockOn_No = 0;
	int Bullet_No = 0;
	LPDIRECT3DDEVICE9 Device = GetDevice();
	PLAYER *Player = GetPlayer(0);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	if (Bomb.Use == true)
	{
		// A�^�C�v
		if (Player->Type == Player_A)
		{
			// �e�N�X�`���̐ݒ�
			if (Bomb.Phase == ShotBomb)
			{
				Device->SetTexture(0, ShotBombTexture);
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Bomb.VertexWk, sizeof(VERTEX_2D));
			}
			else if (Bomb.Phase == BombExtending)
			{
				Device->SetTexture(0, BombExtendTexture);
				for (i = 0; i < TextureDevideNum; i++)
				{
					for (j = 0; j < TextureDevideNum; j++)
					{
						Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BombExtendPixel[i][j], sizeof(VERTEX_2D));
					}
				}
			}
			else if (Bomb.Phase == ExtendOver)
			{
				Device->SetTexture(0, WhiteScreenTexture);
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Bomb.VertexWk, sizeof(VERTEX_2D));
			}
		}
		// B�^�C�v
		else if (Player->Type == Player_B)
		{
			for (LockOn_No = 0; LockOn_No < BombBullet_Max; LockOn_No++)
			{
				if (Bomb.LockOn[LockOn_No].Use == true)
				{
					Device->SetTexture(0, LockOnTexture);
					Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Bomb.LockOn[LockOn_No].VertexWk, sizeof(VERTEX_2D));
				}
			}
			if (Bomb.Phase != TargetLockOn)
			{
				for (Bullet_No = 0; Bullet_No < BombBullet_Max; Bullet_No++)
				{
					if (Bomb.Bullet[Bullet_No].Use == true && Bomb.Bullet[Bullet_No].Count != 0)
					{
						Device->SetTexture(0, BombBulletTexture);
						Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Bomb.Bullet[Bullet_No].VertexWk, sizeof(VERTEX_2D));
					}
					else if (Bomb.Bullet[Bullet_No].State == Explosion)
					{
						DrawImpactWave(Bomb.Bullet[Bullet_No].Pos, Bomb.ExplosionRadius[Bullet_No], WHITE(255));
					}
				}
			}
		}
	}


	return;
}

//=============================================================================
// �{��A�^�C�v�̐ݒu
//=============================================================================
void SetBomb_A(void)
{
	PLAYER *Player = GetPlayer(0);

	Bomb.Pos.x = Player->Pos.x + 80.0f;
	Bomb.Pos.y = Player->Pos.y;
	Bomb.ShakePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Bomb.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Bomb.Radius = ShotBombRadius;
	Bomb.HitRadius = 0.0f;
	Bomb.BaseAngle = ShotBombBaseAngle;
	Bomb.AnimPattern = 0;
	Bomb.Count = 0;
	Bomb.Alpha = 255;
	Bomb.Phase = ShotBomb;
	Bomb.Use = true;

	StopSound(SE_BombExplosion);

	return;
}

//=============================================================================
// �{��B�^�C�v�̐ݒu
//=============================================================================
void SetBomb_B(void)
{
	int LockOn_No = 0;
	int Bullet_No = 0;
	int Enemy_No = 0;
	int ExistEnemyNum = 0;
	PLAYER *Player = GetPlayer(0);
	ENEMY *Enemy = GetEnemy(Enemy_No);
	BOSS *Boss = GetBoss();

	// �e�̏�����
	for (Bullet_No = 0; Bullet_No < BombBullet_Max; Bullet_No++)
	{
		Bomb.Bullet[Bullet_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.Bullet[Bullet_No].PreviousPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.Bullet[Bullet_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.Bullet[Bullet_No].Radius = 0.0f;
		Bomb.Bullet[Bullet_No].HitRadius = 0.0f;
		Bomb.Bullet[Bullet_No].BaseAngle = 0.0f;
		Bomb.Bullet[Bullet_No].MoveAngle = 0.0f;
		Bomb.Bullet[Bullet_No].Speed = 0.0f;
		Bomb.Bullet[Bullet_No].Damage = 0;
		Bomb.Bullet[Bullet_No].Type = 0;
		Bomb.Bullet[Bullet_No].Count = 0;
		Bomb.Bullet[Bullet_No].Color = 0;
		Bomb.Bullet[Bullet_No].State = 0;
		Bomb.Bullet[Bullet_No].Use = false;
		Bomb.Bullet[Bullet_No].DelayDisappear = false;
		Bomb.BulletTarget[Bullet_No] = 0;
		Bomb.ExplosionRadius[Bullet_No] = 0.0f;
	}

	// �Ə��̏�����
	for (LockOn_No = 0; LockOn_No < BombBullet_Max; LockOn_No++)
	{
		Bomb.LockOn[LockOn_No].Use = false;
		Bomb.LockOn[LockOn_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bomb.LockOn[LockOn_No].Radius = 0.0f;
		Bomb.LockOn[LockOn_No].BaseAngle = 0.0f;
		Bomb.LockOn[LockOn_No].Target_No = 0;
	}

	// �G�l�~�[
	if (Boss->Exist == false)
	{
		for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++, Enemy++)
		{
			if (Enemy->Exist == true)
			{
				ExistEnemyNum++;
				for (LockOn_No = 0; LockOn_No < BombBullet_Max; LockOn_No++)
				{
					if (Bomb.LockOn[LockOn_No].Use == false)
					{
						Bomb.LockOn[LockOn_No].Use = true;
						Bomb.LockOn[LockOn_No].Pos = Enemy->Pos;
						Bomb.LockOn[LockOn_No].Radius = LockOnRadius * LockOnZoomRate;
						Bomb.LockOn[LockOn_No].BaseAngle = LockOnBaseAngle;
						Bomb.LockOn[LockOn_No].Target_No = Enemy_No;
						Bomb.BulletTarget[LockOn_No] = Enemy_No;
						Bomb.LockOnNum++;
						break;
					}
				}
			}
		}
	}
	// �{�X
	else if (Boss->Exist == true && Boss->HP > 0.0f)
	{
		for (LockOn_No = 0; LockOn_No < BombBullet_Max / 2; LockOn_No++)
		{
			if (Bomb.LockOn[LockOn_No].Use == false)
			{
				Bomb.LockOn[LockOn_No].Use = true;
				Bomb.LockOn[LockOn_No].Pos = Boss->Pos;
				Bomb.LockOn[LockOn_No].Radius = LockOnRadius * LockOnZoomRate;
				Bomb.LockOn[LockOn_No].BaseAngle = LockOnBaseAngle;
				Bomb.LockOn[LockOn_No].Target_No = 0;
				Bomb.LockOnNum++;
			}
		}
	}

	// ���b�N�I�����ꂽ����0���傫���A�������̓{�X����
	if (ExistEnemyNum > 0 || (Boss->Exist == true && Boss->HP > 0))
	{
		Bomb.Phase = TargetLockOn;
		Bomb.Count = 0;
		Bomb.Use = true;
		PlaySound(SE_LockOn, E_DS8_FLAG_NONE, true);
	}
	else
	{
		Bomb.Use = false;
		Bomb.LockOnNum = 0;
		Player->InBomb = false;
		Player->BombNum++;
	}

	return;
}

//=============================================================================
// �{��B�^�C�v�̒e�𔭎˂���
//=============================================================================
void ShotBombBullet(void)
{
	int Bullet_No = 0;
	int LockOn_No = 0;
	PLAYER *Player = GetPlayer(0);
	BULLETTYPE *BulletType = GetBulletType(BombBulletType);

	for (LockOn_No = 0; LockOn_No < BombBullet_Max; LockOn_No++)
	{
		if (Bomb.LockOn[LockOn_No].Use == true)
		{
			for (Bullet_No = 0; Bullet_No < BombBullet_Max; Bullet_No++)
			{
				if (Bomb.Bullet[Bullet_No].Use == false)
				{
					Bomb.Bullet[Bullet_No].Use = true;
					Bomb.Bullet[Bullet_No].DelayDisappear = false;
					Bomb.Bullet[Bullet_No].Pos.x = (float)(Player->Pos.x + 50.0f * cos(DegreeToRadian(36 * Bullet_No)));
					Bomb.Bullet[Bullet_No].Pos.y = (float)(Player->Pos.y - 50.0f * sin(DegreeToRadian(36 * Bullet_No)));
					Bomb.Bullet[Bullet_No].Count = 0;
					Bomb.Bullet[Bullet_No].Type = 0;
					Bomb.Bullet[Bullet_No].State = Homing;
					Bomb.Bullet[Bullet_No].MoveAngle = 0.0f;
					Bomb.Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
					Bomb.Bullet[Bullet_No].Rot.z = Bomb.Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
					Bomb.Bullet[Bullet_No].Radius = BulletType->Radius;
					Bomb.Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
					Bomb.Bullet[Bullet_No].Speed = BombBulletSpeed;
					//Bomb.BulletTarget[Bullet_No] = Bomb.LockOn[LockOn_No].Target_No;
					Bomb.ExplosionRadius[Bullet_No] = 0.0f;
#ifdef _DEBUG
					SetPlayerBulletHC(Bomb.Bullet[Bullet_No].Pos, Bomb.Bullet[Bullet_No].HitRadius, Bullet_No, PlayerBulletHC);
#endif

					break;
				}
			}
		}
	}

	return;
}

//=============================================================================
// �{��B�^�C�v�̒e���������鎞�̏Ռ��g��`�悷��
//=============================================================================
void DrawImpactWave(D3DXVECTOR3 Center, float Radius, D3DCOLOR Color)
{
	// �~�̓_�̐�
	const int CirclePointNum = 40;
	int i = 0;
	float Theta = 0.0f;
	float WedgeAngle = 0.0f;
	LPD3DXLINE Line = GetLine();
	D3DXVECTOR2 LinePoint[CirclePointNum + 1] = { D3DXVECTOR2(0.0f, 0.0f) };

	// 360�x / �_�̐� = ��̓_�ړ��̊p�x
	WedgeAngle = (float)((2 * PI) / (CirclePointNum));

	for (i = 0; i <= CirclePointNum; i++)
	{
		// ���̊p�x
		Theta = i * WedgeAngle;

		// �~�̊e�_�̍��W���v�Z
		LinePoint[i].x = (float)(Center.x - Radius * cos(Theta));
		LinePoint[i].y = (float)(Center.y - Radius * sin(Theta));
	}

	// �~��`��
	Line->SetWidth(5);
	Line->Begin();
	Line->Draw(LinePoint, CirclePointNum + 1, Color);
	Line->End();

	return;
}

//=============================================================================
// �{��A�^�C�v�̊g��͈͂��m�F����
//=============================================================================
void CheckVertex(VERTEX_2D *Vtx)
{
	int i = 0;

	if (Vtx->vtx.x <= 0 && Vtx->vtx.y <= MoveRange_Y)
	{
		i++;
	}
	if ((Vtx + 1)->vtx.x >= Screen_Width && (Vtx + 1)->vtx.y <= MoveRange_Y)
	{
		i++;
	}
	if ((Vtx + 2)->vtx.x <= 0 && (Vtx + 2)->vtx.y >= Screen_Height)
	{
		i++;
	}
	if ((Vtx + 3)->vtx.x >= Screen_Width && (Vtx + 3)->vtx.y >= Screen_Height)
	{
		i++;
	}

	if (i == 4)
	{
		Bomb.Phase = ExtendOver;
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeBombVertex(void)
{
	int i = 0, j = 0;

	// ���_���W�̐ݒ�	
	SetBombVertex();

	// rhw�̐ݒ�
	Bomb.VertexWk[0].rhw = 1.0f;
	Bomb.VertexWk[1].rhw = 1.0f;
	Bomb.VertexWk[2].rhw = 1.0f;
	Bomb.VertexWk[3].rhw = 1.0f;

	for (i = 0; i < TextureDevideNum; i++)
	{
		for (j = 0; j < TextureDevideNum; j++)
		{
			BombExtendPixel[i][j][0].rhw = 1.0f;
			BombExtendPixel[i][j][1].rhw = 1.0f;
			BombExtendPixel[i][j][2].rhw = 1.0f;
			BombExtendPixel[i][j][3].rhw = 1.0f;
		}
	}

	// ���ˌ��̐ݒ�
	SetBombDiffuse(Bomb.Alpha);

	// �e�N�X�`�����W�̐ݒ�
	SetBombTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetBombTexture(void)
{
	int i = 0, j = 0;
	int x = Bomb.AnimPattern;
	float Size_X = 0.0f;
	float Size_Y = 0.0f;

	if (Bomb.Phase == ShotBomb)
	{
		Size_X = 1.0f / Texture_ShotBomb_Divide_X;
		Bomb.VertexWk[0].tex = D3DXVECTOR2((float)x * Size_X, 0.0f);
		Bomb.VertexWk[1].tex = D3DXVECTOR2((float)x * Size_X + Size_X, 0.0f);
		Bomb.VertexWk[2].tex = D3DXVECTOR2((float)x * Size_X, 1.0f);
		Bomb.VertexWk[3].tex = D3DXVECTOR2((float)x * Size_X + Size_X, 1.0f);
	}
	else if (Bomb.Phase == BombExtending)
	{
		Size_X = 1.0f / (float)TextureDevideNum;
		Size_Y = 1.0f / (float)TextureDevideNum;
		for (i = 0; i < TextureDevideNum; i++)
		{
			for (j = 0; j < TextureDevideNum; j++)
			{
				BombExtendPixel[i][j][0].tex = D3DXVECTOR2(Size_X * j, Size_Y * i);
				BombExtendPixel[i][j][1].tex = D3DXVECTOR2(Size_X * (j + 1), Size_Y * i);
				BombExtendPixel[i][j][2].tex = D3DXVECTOR2(Size_X * j, Size_Y * (i + 1));
				BombExtendPixel[i][j][3].tex = D3DXVECTOR2(Size_X * (j + 1), Size_Y * (i + 1));
			}
		}
	}
	else if (Bomb.Phase == ExtendOver)
	{
		Bomb.VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Bomb.VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Bomb.VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Bomb.VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return;
}

//=============================================================================
// �{��A�^�C�v�̓����x��ݒu����
//=============================================================================
void SetBombDiffuse(int Alpha)
{
	int i = 0, j = 0;

	Bomb.VertexWk[0].diffuse = WHITE(Alpha);
	Bomb.VertexWk[1].diffuse = WHITE(Alpha);
	Bomb.VertexWk[2].diffuse = WHITE(Alpha);
	Bomb.VertexWk[3].diffuse = WHITE(Alpha);

	if (Bomb.Phase == BombExtending)
	{
		for (i = 0; i < TextureDevideNum; i++)
		{
			for (j = 0; j < TextureDevideNum; j++)
			{
				BombExtendPixel[i][j][0].diffuse = WHITE(Alpha);
				BombExtendPixel[i][j][1].diffuse = WHITE(Alpha);
				BombExtendPixel[i][j][2].diffuse = WHITE(Alpha);
				BombExtendPixel[i][j][3].diffuse = WHITE(Alpha);
			}
		}
	}

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetBombVertex(void)
{
	int i = 0, j = 0;
	float TextureWidth = 0.0f;
	float TextureHeight = 0.0f;
	float Size_X = 0.0f;
	float Size_Y = 0.0f;

	if (Bomb.Phase == ShotBomb || Bomb.Phase == BombExtending)
	{
		Bomb.VertexWk[0].vtx.x = Bomb.Pos.x - cosf(Bomb.BaseAngle + Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[0].vtx.y = Bomb.Pos.y - sinf(Bomb.BaseAngle + Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[1].vtx.x = Bomb.Pos.x + cosf(Bomb.BaseAngle - Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[1].vtx.y = Bomb.Pos.y - sinf(Bomb.BaseAngle - Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[2].vtx.x = Bomb.Pos.x - cosf(Bomb.BaseAngle - Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[2].vtx.y = Bomb.Pos.y + sinf(Bomb.BaseAngle - Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[3].vtx.x = Bomb.Pos.x + cosf(Bomb.BaseAngle + Bomb.Rot.z) * Bomb.Radius;
		Bomb.VertexWk[3].vtx.y = Bomb.Pos.y + sinf(Bomb.BaseAngle + Bomb.Rot.z) * Bomb.Radius;
	}
	if (Bomb.Phase == BombExtending)
	{
		TextureWidth = Bomb.VertexWk[1].vtx.x - Bomb.VertexWk[0].vtx.x;
		TextureHeight = Bomb.VertexWk[2].vtx.y - Bomb.VertexWk[0].vtx.y;
		Size_X = TextureWidth / (float)TextureDevideNum;
		Size_Y = TextureHeight / (float)TextureDevideNum;

		for (i = 0; i < TextureDevideNum; i++)
		{
			for (j = 0; j < TextureDevideNum; j++)
			{
				BombExtendPixel[i][j][0].vtx.x = Bomb.VertexWk[0].vtx.x + Size_X * j;
				BombExtendPixel[i][j][0].vtx.y = Bomb.VertexWk[0].vtx.y + Size_Y * i;
				BombExtendPixel[i][j][1].vtx.x = Bomb.VertexWk[0].vtx.x + Size_X * (j + 1);
				BombExtendPixel[i][j][1].vtx.y = Bomb.VertexWk[0].vtx.y + Size_Y * i;
				BombExtendPixel[i][j][2].vtx.x = Bomb.VertexWk[0].vtx.x + Size_X * j;
				BombExtendPixel[i][j][2].vtx.y = Bomb.VertexWk[0].vtx.y + Size_Y * (i + 1);
				BombExtendPixel[i][j][3].vtx.x = Bomb.VertexWk[0].vtx.x + Size_X * (j + 1);
				BombExtendPixel[i][j][3].vtx.y = Bomb.VertexWk[0].vtx.y + Size_Y * (i + 1);
				if (BombExtendPixel[i][j][0].vtx.y <= MoveRange_Y && BombExtendPixel[i][j][1].vtx.y <= MoveRange_Y)
				{
					BombExtendPixel[i][j][0].diffuse = WHITE(0);
					BombExtendPixel[i][j][1].diffuse = WHITE(0);
					BombExtendPixel[i][j][2].diffuse = WHITE(0);
					BombExtendPixel[i][j][3].diffuse = WHITE(0);
				}
			}
		}

		CheckVertex(&Bomb.VertexWk[0]);
	}
	if (Bomb.Phase == ExtendOver)
	{
		Bomb.VertexWk[0].vtx = D3DXVECTOR3(0 + Bomb.ShakePos.x, MoveRange_Y + Bomb.ShakePos.y, 0);
		Bomb.VertexWk[1].vtx = D3DXVECTOR3(Screen_Width + Bomb.ShakePos.x, MoveRange_Y + Bomb.ShakePos.y, 0);
		Bomb.VertexWk[2].vtx = D3DXVECTOR3(0 + Bomb.ShakePos.x, Screen_Height + Bomb.ShakePos.y, 0);
		Bomb.VertexWk[3].vtx = D3DXVECTOR3(Screen_Width + Bomb.ShakePos.x, Screen_Height + Bomb.ShakePos.y, 0);
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeLockOnVertex(int LockOn_No)
{
	// ���_���W�̐ݒ�	
	SetLockOnVertex(LockOn_No);

	// rhw�̐ݒ�
	Bomb.LockOn[LockOn_No].VertexWk[0].rhw = 1.0f;
	Bomb.LockOn[LockOn_No].VertexWk[1].rhw = 1.0f;
	Bomb.LockOn[LockOn_No].VertexWk[2].rhw = 1.0f;
	Bomb.LockOn[LockOn_No].VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Bomb.LockOn[LockOn_No].VertexWk[0].diffuse = WHITE(255);
	Bomb.LockOn[LockOn_No].VertexWk[1].diffuse = WHITE(255);
	Bomb.LockOn[LockOn_No].VertexWk[2].diffuse = WHITE(255);
	Bomb.LockOn[LockOn_No].VertexWk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetLockOnTexture(LockOn_No);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetLockOnTexture(int LockOn_No)
{
	Bomb.LockOn[LockOn_No].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Bomb.LockOn[LockOn_No].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Bomb.LockOn[LockOn_No].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Bomb.LockOn[LockOn_No].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetLockOnVertex(int LockOn_No)
{
	Bomb.LockOn[LockOn_No].VertexWk[0].vtx.x = Bomb.LockOn[LockOn_No].Pos.x - cosf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[0].vtx.y = Bomb.LockOn[LockOn_No].Pos.y - sinf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[1].vtx.x = Bomb.LockOn[LockOn_No].Pos.x + cosf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[1].vtx.y = Bomb.LockOn[LockOn_No].Pos.y - sinf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[2].vtx.x = Bomb.LockOn[LockOn_No].Pos.x - cosf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[2].vtx.y = Bomb.LockOn[LockOn_No].Pos.y + sinf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[3].vtx.x = Bomb.LockOn[LockOn_No].Pos.x + cosf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;
	Bomb.LockOn[LockOn_No].VertexWk[3].vtx.y = Bomb.LockOn[LockOn_No].Pos.y + sinf(Bomb.LockOn[LockOn_No].BaseAngle) * Bomb.LockOn[LockOn_No].Radius;

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeBulletVertex(int Bullet_No)
{
	// ���_���W�̐ݒ�	
	SetBulletVertex(Bullet_No);

	// rhw�̐ݒ�
	Bomb.Bullet[Bullet_No].VertexWk[0].rhw = 1.0f;
	Bomb.Bullet[Bullet_No].VertexWk[1].rhw = 1.0f;
	Bomb.Bullet[Bullet_No].VertexWk[2].rhw = 1.0f;
	Bomb.Bullet[Bullet_No].VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Bomb.Bullet[Bullet_No].VertexWk[0].diffuse = WHITE(255);
	Bomb.Bullet[Bullet_No].VertexWk[1].diffuse = WHITE(255);
	Bomb.Bullet[Bullet_No].VertexWk[2].diffuse = WHITE(255);
	Bomb.Bullet[Bullet_No].VertexWk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetBulletTexture(Bullet_No);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetBulletTexture(int Bullet_No)
{
	Bomb.Bullet[Bullet_No].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Bomb.Bullet[Bullet_No].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Bomb.Bullet[Bullet_No].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Bomb.Bullet[Bullet_No].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetBulletVertex(int Bullet_No)
{
	Bomb.Bullet[Bullet_No].VertexWk[0].vtx.x =
		Bomb.Bullet[Bullet_No].Pos.x - cosf(Bomb.Bullet[Bullet_No].BaseAngle + Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[0].vtx.y =
		Bomb.Bullet[Bullet_No].Pos.y - sinf(Bomb.Bullet[Bullet_No].BaseAngle + Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[1].vtx.x =
		Bomb.Bullet[Bullet_No].Pos.x + cosf(Bomb.Bullet[Bullet_No].BaseAngle - Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[1].vtx.y =
		Bomb.Bullet[Bullet_No].Pos.y - sinf(Bomb.Bullet[Bullet_No].BaseAngle - Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[2].vtx.x =
		Bomb.Bullet[Bullet_No].Pos.x - cosf(Bomb.Bullet[Bullet_No].BaseAngle - Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[2].vtx.y =
		Bomb.Bullet[Bullet_No].Pos.y + sinf(Bomb.Bullet[Bullet_No].BaseAngle - Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[3].vtx.x =
		Bomb.Bullet[Bullet_No].Pos.x + cosf(Bomb.Bullet[Bullet_No].BaseAngle + Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;
	Bomb.Bullet[Bullet_No].VertexWk[3].vtx.y =
		Bomb.Bullet[Bullet_No].Pos.y + sinf(Bomb.Bullet[Bullet_No].BaseAngle + Bomb.Bullet[Bullet_No].Rot.z) * Bomb.Bullet[Bullet_No].Radius;

	return;
}

//=============================================================================
// �{���̏����擾����
//=============================================================================
BOMB *GetBomb(void)
{
	return &Bomb;
}

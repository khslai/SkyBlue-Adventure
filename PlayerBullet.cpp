//=============================================================================
//
// �v���C���[�e������ [PlayerBullet.cpp]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#include "main.h"
#include "PlayerBullet.h"
#include "Player.h"
#include "Boss.h"
#include "Enemy.h"
#include "File.h"
#include "Sound.h"
#include "DebugTest.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PlayerBulletDamage (4)
#define PlayerBulletSpeed (20.0f)

enum BulletType
{
	PlayerBullet,
	OptionBullet,
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
static HRESULT MakeBulletVertex(int Bullet_No);
static void SetBulletTexture(int Bullet_No);
static void SetBulletVertex(int Bullet_No);
void PlayerShot(void);
void CalculateHoming(int Bullet_No);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9 Bullet_ATexture = NULL;
static LPDIRECT3DTEXTURE9 Bullet_BTexture = NULL;
static LPDIRECT3DTEXTURE9 OptionBullet_ATexture = NULL;
static LPDIRECT3DTEXTURE9 OptionBullet_BTexture = NULL;
// �e�̃_���[�W�{��
const float ShotDamageRate[4] = { 1.0f, 1.2f, 1.3f, 1.5f };
const float HomingDamageRate = 0.15f;
// �e���ˈʒu(Pos.x + 10, Pos.y - 20) , (Pos.x + 10, Pos.y + 20)
const int PlayerShot_x[2] = { 10, 10 };
const int PlayerShot_y[2] = { -20, 20 };
// �I�v�V�����e���ˊp�x(Pos.x + 10, Pos.y - 20) , (Pos.x + 10, Pos.y + 20)
const int OptionAngle[4] = { -30, 30, -60, 60 };
const int OptionAngle_LowSpeed[4] = { 0, 0, -15, 15 };


//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayerBullet(int InitState)
{
	int Bullet_No = 0;
	PLAYER *Player = GetPlayer(0);

	for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
	{
		Player->Bullet[Bullet_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Player->Bullet[Bullet_No].PreviousPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Player->Bullet[Bullet_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Player->Bullet[Bullet_No].Radius = 0.0f;
		Player->Bullet[Bullet_No].HitRadius = 0.0f;
		Player->Bullet[Bullet_No].BaseAngle = 0.0f;
		Player->Bullet[Bullet_No].MoveAngle = 0.0f;
		Player->Bullet[Bullet_No].Speed = 0.0f;
		Player->Bullet[Bullet_No].Damage = 0;
		Player->Bullet[Bullet_No].Type = 0;
		Player->Bullet[Bullet_No].Count = 0;
		Player->Bullet[Bullet_No].Color = 0;
		Player->Bullet[Bullet_No].State = 0;
		Player->Bullet[Bullet_No].Use = false;
		Player->Bullet[Bullet_No].DelayDisappear = false;

		// ���_���̍쐬
		MakeBulletVertex(Bullet_No);
	}

	if (InitState == FirstInit)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_PlayerBullet_A, &Bullet_ATexture, "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_PlayerBullet_B, &Bullet_BTexture, "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_OptionBullet_A, &OptionBullet_ATexture, "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_OptionBullet_B, &OptionBullet_BTexture, "Player") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayerBullet(void)
{
	SafeRelease(Bullet_ATexture);
	SafeRelease(Bullet_BTexture);
	SafeRelease(OptionBullet_ATexture);
	SafeRelease(OptionBullet_BTexture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayerBullet(void)
{
	int Bullet_No = 0;
	int GameStage = GetGameStage();
	PLAYER *Player = GetPlayer(0);
	BULLETTYPE *BulletType = GetBulletType(Player->BulletType);

	for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
	{
		if (Player->Bullet[Bullet_No].Use == true)
		{
			if (Player->Bullet[Bullet_No].DelayDisappear == true)
			{
				Player->Bullet[Bullet_No].Use = false;
			}
			else
			{
				if (Player->Type == Player_B && Player->Bullet[Bullet_No].Type == OptionBullet)
				{
					CalculateHoming(Bullet_No);
				}
				Player->Bullet[Bullet_No].PreviousPos = Player->Bullet[Bullet_No].Pos;
				Player->Bullet[Bullet_No].Pos.x +=
					cosf(Player->Bullet[Bullet_No].MoveAngle) * Player->Bullet[Bullet_No].Speed;
				Player->Bullet[Bullet_No].Pos.y +=
					sinf(Player->Bullet[Bullet_No].MoveAngle) * Player->Bullet[Bullet_No].Speed;
				if (Player->Type == Player_B && Player->Bullet[Bullet_No].Type == OptionBullet)
				{
					Player->Bullet[Bullet_No].Rot.z += 0.1f;
				}
				Player->Bullet[Bullet_No].Count++;
				if (GameStage != Stage_SelectChar)
				{
					if (CheckRange(OutOfRange, Player->Bullet[Bullet_No].Pos,
						BulletType->Width, BulletType->Height) == true)
					{
						Player->Bullet[Bullet_No].Use = false;
					}
				}
				else
				{
					if (CheckRange(OutOfRange_SelectChar, Player->Bullet[Bullet_No].Pos,
						BulletType->Width, BulletType->Height) == true)
					{
						Player->Bullet[Bullet_No].Use = false;
					}
				}


				SetBulletTexture(Bullet_No);
				SetBulletVertex(Bullet_No);
			}
		}
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayerBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	PLAYER *Player = GetPlayer(0);
	int Bullet_No = 0;

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
	{
		if (Player->Bullet[Bullet_No].Use == true)
		{
			// �e�N�X�`���̐ݒ�
			if (Player->Bullet[Bullet_No].Type == PlayerBullet)
			{
				if (Player->Type == Player_A)
				{
					Device->SetTexture(0, Bullet_ATexture);
				}
				else if (Player->Type == Player_B)
				{
					Device->SetTexture(0, Bullet_BTexture);
				}
			}
			else if (Player->Bullet[Bullet_No].Type == OptionBullet)
			{
				if (Player->Type == Player_A)
				{
					Device->SetTexture(0, OptionBullet_ATexture);
				}
				else if (Player->Type == Player_B)
				{
					Device->SetTexture(0, OptionBullet_BTexture);
				}
			}

			// �|���S���̕`��
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Player->Bullet[Bullet_No].VertexWk, sizeof(VERTEX_2D));
		}
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeBulletVertex(int Bullet_No)
{
	PLAYER *Player = GetPlayer(0);

	// ���_���W�̐ݒ�	
	SetBulletVertex(Bullet_No);

	// rhw�̐ݒ�
	Player->Bullet[Bullet_No].VertexWk[0].rhw = 1.0f;
	Player->Bullet[Bullet_No].VertexWk[1].rhw = 1.0f;
	Player->Bullet[Bullet_No].VertexWk[2].rhw = 1.0f;
	Player->Bullet[Bullet_No].VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Player->Bullet[Bullet_No].VertexWk[0].diffuse = WHITE(255);
	Player->Bullet[Bullet_No].VertexWk[1].diffuse = WHITE(255);
	Player->Bullet[Bullet_No].VertexWk[2].diffuse = WHITE(255);
	Player->Bullet[Bullet_No].VertexWk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetBulletTexture(Bullet_No);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetBulletTexture(int Bullet_No)
{
	PLAYER *Player = GetPlayer(0);

	Player->Bullet[Bullet_No].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Player->Bullet[Bullet_No].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Player->Bullet[Bullet_No].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Player->Bullet[Bullet_No].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetBulletVertex(int Bullet_No)
{
	PLAYER *Player = GetPlayer(0);

	Player->Bullet[Bullet_No].VertexWk[0].vtx.x =
		Player->Bullet[Bullet_No].Pos.x - cosf(Player->Bullet[Bullet_No].BaseAngle + Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[0].vtx.y =
		Player->Bullet[Bullet_No].Pos.y - sinf(Player->Bullet[Bullet_No].BaseAngle + Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[1].vtx.x =
		Player->Bullet[Bullet_No].Pos.x + cosf(Player->Bullet[Bullet_No].BaseAngle - Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[1].vtx.y =
		Player->Bullet[Bullet_No].Pos.y - sinf(Player->Bullet[Bullet_No].BaseAngle - Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[2].vtx.x =
		Player->Bullet[Bullet_No].Pos.x - cosf(Player->Bullet[Bullet_No].BaseAngle - Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[2].vtx.y =
		Player->Bullet[Bullet_No].Pos.y + sinf(Player->Bullet[Bullet_No].BaseAngle - Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[3].vtx.x =
		Player->Bullet[Bullet_No].Pos.x + cosf(Player->Bullet[Bullet_No].BaseAngle + Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;
	Player->Bullet[Bullet_No].VertexWk[3].vtx.y =
		Player->Bullet[Bullet_No].Pos.y + sinf(Player->Bullet[Bullet_No].BaseAngle + Player->Bullet[Bullet_No].Rot.z) * Player->Bullet[Bullet_No].Radius;

	return;
}

//=============================================================================
// �v���C���[�e�̐ݒu
//=============================================================================
void PlayerShot(void)
{
	int i = 0;
	int j = 0;
	int Bullet_No = 0;
	int GameStage = GetGameStage();
	PLAYER *Player = GetPlayer(0);
	BULLETTYPE *BulletType = GetBulletType(Player->BulletType);

	// �e���ˉ�
	if (GameStage != Stage_SelectChar)
	{
		PlaySound(SE_PlayerShot, E_DS8_FLAG_NONE, true);
	}

	// ���@�V���b�g�A���2��
	for (i = 0; i < 2; i++)
	{
		for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
		{
			if (Player->Bullet[Bullet_No].Use == false)
			{
				Player->Bullet[Bullet_No].Use = true;
				Player->Bullet[Bullet_No].DelayDisappear = false;
				// �ʏ탂�[�h
				if (Player->LowSpeedMode == false)
				{
					Player->Bullet[Bullet_No].Pos.x = Player->Pos.x + PlayerShot_x[i];
					Player->Bullet[Bullet_No].Pos.y = Player->Pos.y + PlayerShot_y[i];
				}
				// �ᑬ���[�h
				else
				{
					Player->Bullet[Bullet_No].Pos.x = Player->Pos.x + PlayerShot_x[i] / 3;
					Player->Bullet[Bullet_No].Pos.y = Player->Pos.y + PlayerShot_y[i] / 2;
				}
				Player->Bullet[Bullet_No].Count = 0;
				Player->Bullet[Bullet_No].Type = PlayerBullet;
				Player->Bullet[Bullet_No].MoveAngle = 0.0f;
				Player->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
				Player->Bullet[Bullet_No].Rot.z = Player->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
				Player->Bullet[Bullet_No].Radius = BulletType->Radius;
				Player->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
				Player->Bullet[Bullet_No].Speed = PlayerBulletSpeed;
				if (GameStage != Stage_SelectChar)
				{
					if (Player->Power < 1.0f)
					{
						Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[0];
					}
					else if (Player->Power >= 1.0f && Player->Power < 2.0f)
					{
						Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[1];
					}
					else if (Player->Power >= 2.0f && Player->Power < 3.0f)
					{
						Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[2];
					}
					else if (Player->Power == 3.0f)
					{
						Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[3];
					}
				}
				else
				{
					Player->Bullet[Bullet_No].Damage = 0.0f;
				}

#ifdef _DEBUG
				SetPlayerBulletHC(Player->Bullet[Bullet_No].Pos, Player->Bullet[Bullet_No].HitRadius, Bullet_No, PlayerBulletHC);
#endif

				break;
			}
		}
	}

	// �I�v�V�����V���b�g
	for (i = 0; i < PlayerOption_Max; i++)
	{
		if (Player->Option[i].Use == true)
		{
			BulletType = GetBulletType(Player->Option[i].BulletType);
			for (j = 0; j < (Player->Power == 3.0f ? 2 : 1); j++)
			{
				for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
				{
					if (Player->Bullet[Bullet_No].Use == false)
					{
						Player->Bullet[Bullet_No].Use = true;
						Player->Bullet[Bullet_No].DelayDisappear = false;
						Player->Bullet[Bullet_No].Pos.x = Player->Option[i].Pos.x;
						Player->Bullet[Bullet_No].Pos.y = Player->Option[i].Pos.y;
						Player->Bullet[Bullet_No].Count = 0;
						Player->Bullet[Bullet_No].Type = OptionBullet;
						if (Player->LowSpeedMode == false)
						{
							Player->Bullet[Bullet_No].MoveAngle = DegreeToRadian(OptionAngle[i + j * 2]);
						}
						else
						{
							Player->Bullet[Bullet_No].MoveAngle = DegreeToRadian(OptionAngle_LowSpeed[i + j * 2]);
						}
						Player->Bullet[Bullet_No].BaseAngle = BulletType->BaseAngle;
						Player->Bullet[Bullet_No].Rot.z = Player->Bullet[Bullet_No].MoveAngle + BulletType->Rot_z;
						Player->Bullet[Bullet_No].Radius = BulletType->Radius;
						Player->Bullet[Bullet_No].HitRadius = BulletType->HitRadius;
						Player->Bullet[Bullet_No].Speed = PlayerBulletSpeed;
						if (GameStage != Stage_SelectChar)
						{
							if (Player->Power < 1.0f)
							{
								Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[0];
							}
							else if (Player->Power >= 1.0f && Player->Power < 2.0f)
							{
								Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[1];
							}
							else if (Player->Power >= 2.0f && Player->Power < 3.0f)
							{
								Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[2];
							}
							else if (Player->Power == 3.0f)
							{
								Player->Bullet[Bullet_No].Damage = PlayerBulletDamage * ShotDamageRate[3];
							}
						}
						else
						{
							Player->Bullet[Bullet_No].Damage = 0.0f;
						}
						if (Player->Type == Player_B)
						{
							Player->Bullet[Bullet_No].Damage = Player->Bullet[Bullet_No].Damage * HomingDamageRate;
						}


#ifdef _DEBUG
						SetPlayerBulletHC(Player->Bullet[Bullet_No].Pos, Player->Bullet[Bullet_No].HitRadius, Bullet_No, PlayerBulletHC);
#endif

						break;
					}
				}
			}
		}
	}

	return;
}

//=============================================================================
// �e�̃z�[�~���O���v�Z
//=============================================================================
void CalculateHoming(int Bullet_No)
{
	int Enemy_No = 0;
	int Target = -1;
	float Min_Distance = 0.0f;
	float Distance = 0.0f;
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();
	ENEMY *Enemy = GetEnemy(Enemy_No);

	// �{�X�����Ȃ���
	if (Boss->Exist == false)
	{
		Enemy_No = 0;
		Enemy = GetEnemy(Enemy_No);
		for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++, Enemy++)
		{
			// �G�̑�����
			if (Enemy->Exist == true)
			{
				temp = Enemy->Pos - Player->Bullet[Bullet_No].Pos;
				// �V���b�g�ƓG�̋���
				Distance = D3DXVec3LengthSq(&temp);
				if (Distance < Min_Distance || Min_Distance == 0.0f)
				{
					// �v�Z���ʂ��ŏ��l���܂��i�[���Ă��Ȃ��Ȃ�
					// �G�l�~�[�ԍ��L�^
					Target = Enemy_No;
					// �G�l�~�[�Ƃ̋����L�^
					Min_Distance = Distance;
				}
			}
		}
	}
	// �߂��G������������A���邢�̓{�X�����āAHP������Ƃ��͊p�x���Z�b�g
	if (Target != -1 || (Boss->Exist == true && Boss->HP > 0))
	{
		if (Boss->Exist == false)
		{
			Enemy = GetEnemy(Target);
			Player->Bullet[Bullet_No].MoveAngle = AngleCalculate(Player->Bullet[Bullet_No].Pos, Enemy->Pos);
		}
		else
		{
			Player->Bullet[Bullet_No].MoveAngle = AngleCalculate(Player->Bullet[Bullet_No].Pos, Boss->Pos);
		}
	}

	return;
}
//=============================================================================
//
// ���[�U�[���� [Laser.cpp]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#include "main.h"
#include "Laser.h"
#include "Player.h"
#include "Boss.h"
#include "Enemy.h"
#include "Sound.h"
#include "DebugTest.h"
#include "CheckHit.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �����J�n�̃J�E���g
#define DisappearCount (270)
// ���[�U�[�̔��ˍ��W�ƃG�l�~�[���W�̋���
#define DistanceFromEnemy (30.0f)
// ���[�U�[�ő唭�ː� = Laser_Num_Max * 2
#define Laser_Num_Max (8)

enum
{
	Clockwise = 1,
	Counterclockwise = -1,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeLaserVertex(int	Laser_No);
void SetLaserTexture(int Laser_No);
void SetLaserVertex(int Laser_No);
void CalculateLaserRotate(int Laser_No);
void CalculateLaserRect(D3DXVECTOR3 Pos, float Length);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 LaserTexture = NULL;
LASER Laser[Laser_Max];
int Laser_Num = 0;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitLaser(int InitState)
{
	int Laser_No = 0;
	Laser_Num = 2;

	for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++)
	{
		Laser[Laser_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Laser[Laser_No].RotateCenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Laser[Laser_No].BeforeRotPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Laser[Laser_No].RotateTime = 0.0f;
		Laser[Laser_No].MoveAngle = 0.0f;
		Laser[Laser_No].BeforeRotAngle = 0.0f;
		Laser[Laser_No].RotateAngle = 0.0f;
		Laser[Laser_No].Width = 0.0f;
		Laser[Laser_No].Length = 0.0f;
		Laser[Laser_No].State = 0;
		Laser[Laser_No].Count = 0;
		Laser[Laser_No].Use = false;
		Laser[Laser_No].InRotate = false;

		MakeLaserVertex(Laser_No);
	}

	if (InitState == FirstInit)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Laser, &LaserTexture, "Laser") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitLaser(void)
{
	SafeRelease(LaserTexture);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLaser(void)
{
	int Laser_No = 0;
	BOSS *Boss = GetBoss();

	for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++)
	{
		if (Laser[Laser_No].Use == true)
		{
			if (Boss->State == HPRecover || Boss->State == BossDisappear || Boss->State == LastBossOver)
			{
				Laser[Laser_No].Use = false;
				continue;
			}
			CalculateLaserRotate(Laser_No);
			Laser[Laser_No].Count++;

			SetLaserVertex(Laser_No);
			SetLaserTexture(Laser_No);
		}

	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLaser(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Laser_No = 0;

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++)
	{
		if (Laser[Laser_No].Use == true)
		{
			Device->SetTexture(0, LaserTexture);
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Laser[Laser_No].VertexWk, sizeof(VERTEX_2D));
		}
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeLaserVertex(int Laser_No)
{
	// ���_���W�̐ݒ�	
	SetLaserVertex(Laser_No);

	// rhw�̐ݒ�
	Laser[Laser_No].VertexWk[0].rhw = 1.0f;
	Laser[Laser_No].VertexWk[1].rhw = 1.0f;
	Laser[Laser_No].VertexWk[2].rhw = 1.0f;
	Laser[Laser_No].VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Laser[Laser_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Laser[Laser_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Laser[Laser_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Laser[Laser_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetLaserTexture(Laser_No);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetLaserTexture(int Laser_No)
{
	Laser[Laser_No].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Laser[Laser_No].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Laser[Laser_No].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Laser[Laser_No].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetLaserVertex(int Laser_No)
{
	Laser[Laser_No].VertexWk[0].vtx.x =
		Laser[Laser_No].Pos.x + cosf(Laser[Laser_No].MoveAngle + PI / 2) * Laser[Laser_No].Width / 2;
	Laser[Laser_No].VertexWk[0].vtx.y =
		Laser[Laser_No].Pos.y + sinf(Laser[Laser_No].MoveAngle + PI / 2) * Laser[Laser_No].Width / 2;
	Laser[Laser_No].VertexWk[1].vtx.x =
		Laser[Laser_No].Pos.x + cosf(Laser[Laser_No].MoveAngle + PI / 2) * Laser[Laser_No].Width / 2 + cosf(Laser[Laser_No].MoveAngle) * Laser[Laser_No].Length;
	Laser[Laser_No].VertexWk[1].vtx.y =
		Laser[Laser_No].Pos.y + sinf(Laser[Laser_No].MoveAngle + PI / 2) * Laser[Laser_No].Width / 2 + sinf(Laser[Laser_No].MoveAngle) * Laser[Laser_No].Length;
	Laser[Laser_No].VertexWk[2].vtx.x =
		Laser[Laser_No].Pos.x + cosf(Laser[Laser_No].MoveAngle - PI / 2) * Laser[Laser_No].Width / 2;
	Laser[Laser_No].VertexWk[2].vtx.y =
		Laser[Laser_No].Pos.y + sinf(Laser[Laser_No].MoveAngle - PI / 2) * Laser[Laser_No].Width / 2;
	Laser[Laser_No].VertexWk[3].vtx.x =
		Laser[Laser_No].Pos.x + cosf(Laser[Laser_No].MoveAngle - PI / 2) * Laser[Laser_No].Width / 2 + cosf(Laser[Laser_No].MoveAngle) * Laser[Laser_No].Length;
	Laser[Laser_No].VertexWk[3].vtx.y =
		Laser[Laser_No].Pos.y + sinf(Laser[Laser_No].MoveAngle - PI / 2) * Laser[Laser_No].Width / 2 + sinf(Laser[Laser_No].MoveAngle) * Laser[Laser_No].Length;

	return;
}

//=============================================================================
// ���[�U�[�̐ݒu
//=============================================================================
void SetLaser()
{
	int Laser_No = 0;
	int Angle = 0;
	int AngleHalf = 0;
	int MoveAngle = 0;
	int GameStage = GetGameStage();
	BOSS *Boss = GetBoss();

	// ���[�U�[���ˉ�
	if (GameStage != Stage_PraticeSelect)
	{
		PlaySound(SE_BossShot_01, E_DS8_FLAG_NONE, true);
	}

	// ������
	if (GameStage == Stage_PraticeSelect && Boss->ShotCount == 0)
	{
		Laser_Num = 2;
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < Laser_Num; j++)
		{
			for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++)
			{
				if (Laser[Laser_No].Use == false)
				{
					// 360�x �� ���[�U�[�̐� = �����p
					Angle = 360 / (2 * Laser_Num);
					// �����p�̔���
					AngleHalf = Angle / 2;
					// �p�x�̌v�Z
					MoveAngle = j * 2 * Angle + AngleHalf * (Laser_Num % 2) + i * Angle;
					// i = 0 -> �E���Ai = 1 -> �����
					Laser[Laser_No].State = i == 0 ? Clockwise : Counterclockwise;
					Laser[Laser_No].Use = true;
					Laser[Laser_No].InRotate = true;
					Laser[Laser_No].MoveAngle = DegreeToRadian(MoveAngle);
					Laser[Laser_No].BeforeRotAngle = Laser[Laser_No].MoveAngle;
					// �G�l�~�[�̍��W�𒆐S�ŉ�]����
					Laser[Laser_No].RotateCenterPos.x = Boss->Pos.x;
					Laser[Laser_No].RotateCenterPos.y = Boss->Pos.y;
					// ���[�U�[�̔��ˍ��W
					Laser[Laser_No].Pos.x = Boss->Pos.x + cosf(Laser[Laser_No].MoveAngle) * DistanceFromEnemy;
					Laser[Laser_No].Pos.y = Boss->Pos.y + sinf(Laser[Laser_No].MoveAngle) * DistanceFromEnemy;
					Laser[Laser_No].BeforeRotPos = Laser[Laser_No].Pos;
					// ��]����
					Laser[Laser_No].RotateTime = 60.0f;
					//Laser[Laser_No].RotateAngle = DegreeToRadian(0);
					// ��]�p�x�͐����p�Ai = 0 -> �E���Ai = 1 -> �����
					Laser[Laser_No].RotateAngle = DegreeToRadian(Angle) * Laser[Laser_No].State;
					// ���[�U�[�̕�
					Laser[Laser_No].Width = Texture_Laser_Width / 10;
					// ���[�U�[�̒���
					if (GameStage != Stage_PraticeSelect)
					{
						Laser[Laser_No].Length = Screen_Width;
					}
					else
					{
						Laser[Laser_No].Length = 250.0f;
					}
					Laser[Laser_No].Count = 0;
					break;
				}
			}
		}
	}

	CalculateLaserRect(Boss->Pos, Screen_Width);

	if (Laser_Num < Laser_Num_Max)
	{
		Laser_Num++;
	}

	return;
}

//=============================================================================
// ���[�U�[�̏����擾����
//=============================================================================
LASER *GetLaser(int Laser_No)
{
	return &Laser[Laser_No];
}

//=============================================================================
// ���[�U�[�̉�]�A�ړ����v�Z����
//=============================================================================
void CalculateLaserRotate(int Laser_No)
{
	float Theta = 0.0f;
	float x0 = 0.0f;
	float y0 = 0.0f;
	// ��]�������p�x
	float RotAngle = Laser[Laser_No].RotateAngle;
	// ��]�K�v�̎���
	float RotTime = Laser[Laser_No].RotateTime;
	// ���͉��J�E���g
	int Count = Laser[Laser_No].Count;

	// ��]�p�x�v�Z
	Theta = (2 * RotAngle * Count / RotTime) - (RotAngle * Count * Count / (RotTime * RotTime));
	// ��]�ړ����ԓ��Ȃ�
	if (RotTime > 0 && Laser[Laser_No].InRotate == true)
	{
		// ��]����
		Laser[Laser_No].MoveAngle = Laser[Laser_No].BeforeRotAngle + Theta;
		// ���W�ϊ�����
		// (Center_x,Center_y)�𒆐S�ŉ�]����
		// ���S���W�������Ȃ��ƁA��]�̊�_��(0,0)�ɂȂ�
		// x0 = ���S���烌�[�U�[���ˍ��W��x����
		x0 = Laser[Laser_No].BeforeRotPos.x - Laser[Laser_No].RotateCenterPos.x;
		// y0 = ���S���烌�[�U�[���ˍ��W��y����
		y0 = Laser[Laser_No].BeforeRotPos.y - Laser[Laser_No].RotateCenterPos.y;
		// �����B(x0,y0)�Ɗp����]��(x1,y1)
		// x1 = x0cos�� - y0sin��
		// y1 = x0sin�� + y0cos��
		// ��]��x���W = ���S���W + x1
		Laser[Laser_No].Pos.x = Laser[Laser_No].RotateCenterPos.x + (x0 * cosf(Theta) - y0 * sinf(Theta));
		// ��]��y���W = ���S���W + y1
		Laser[Laser_No].Pos.y = Laser[Laser_No].RotateCenterPos.y + (x0 * sinf(Theta) + y0 * cosf(Theta));
	}

	if (Count > RotTime)
	{
		// ��]���Ԃ��߂���Ƃ�߂�
		Laser[Laser_No].RotateTime = 0;
		Laser[Laser_No].InRotate = false;
	}

	// ���Ŏ��Ԃ𒴂�����A���͏���������
	if (Count >= DisappearCount)
	{
		// 0.5�b�ȓ�����
		Laser[Laser_No].Width -= Texture_Laser_Width / 30.0f;
		// 0�ȉ��Ȃ�A���[�U�[����
		if (Laser[Laser_No].Width <= 0)
		{
			Laser[Laser_No].Use = false;
		}
	}
	else if (Laser[Laser_No].InRotate == false)
	{
		Laser[Laser_No].Width = Texture_Laser_Width;
	}

	return;
}


//=============================================================================
// ���[�U�[�̓����蔻��p��`���v�Z����
//=============================================================================
void CalculateLaserRect(D3DXVECTOR3 Pos, float Length)
{
	// 0�x�̃��[�U�[��`���v�Z����
	float Angle = 0;
	// ���[�U�[�̍����̐^��
	float LaserPos_x = Pos.x + cosf(DegreeToRadian(Angle)) * DistanceFromEnemy;
	float LaserPos_y = Pos.y + sinf(DegreeToRadian(Angle)) * DistanceFromEnemy;
	// ��`����̒��_���W
	D3DXVECTOR3 RectLeftUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// ��`�E���̒��_���W
	D3DXVECTOR3 RectRightDown = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ��`����̒��_���W
	RectLeftUp.x = LaserPos_x;
	RectLeftUp.y = LaserPos_y - Texture_Laser_Width / 2;
	// ��`�E���̒��_���W
	RectRightDown.x = LaserPos_x + Length;
	RectRightDown.y = LaserPos_y + Texture_Laser_Width / 2;
	// ���[�U�[��`�̔���͈͂�ݒu����
	SetLaserCheckRect(RectLeftUp, RectRightDown);

	return;
}


//=============================================================================
//
// �G�l�~�[�w�b�_�[ [Enemy.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "File.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Enemy00					_T("data/Texture/Enemy/Enemy00.png")
#define Texture_Enemy00_Width_Total		(240)
#define Texture_Enemy00_Height_Total	(256)
#define Texture_Enemy00_Divide_X		(3)
#define Texture_Enemy00_Divide_Y		(4)
#define Texture_Enemy00_Width			(Texture_Enemy00_Width_Total / Texture_Enemy00_Divide_X)
#define Texture_Enemy00_Height			(Texture_Enemy00_Height_Total / Texture_Enemy00_Divide_Y)
#define Texture_Enemy01					_T("data/Texture/Enemy/Enemy01.png")
#define Texture_Enemy01_Width_Total		(240)
#define Texture_Enemy01_Height_Total	(256)
#define Texture_Enemy01_Divide_X		(3)
#define Texture_Enemy01_Divide_Y		(4)
#define Texture_Enemy01_Width			(Texture_Enemy01_Width_Total / Texture_Enemy01_Divide_X)
#define Texture_Enemy01_Height			(Texture_Enemy01_Height_Total / Texture_Enemy01_Divide_Y)
#define Texture_Enemy02					_T("data/Texture/Enemy/Enemy02.png")
#define Texture_Enemy02_Width_Total		(150)
#define Texture_Enemy02_Height_Total	(168)
#define Texture_Enemy02_Divide_X		(3)
#define Texture_Enemy02_Divide_Y		(4)
#define Texture_Enemy02_Width			(Texture_Enemy02_Width_Total / Texture_Enemy02_Divide_X)
#define Texture_Enemy02_Height			(Texture_Enemy02_Height_Total / Texture_Enemy02_Divide_Y)
#define Texture_Enemy03					_T("data/Texture/Enemy/Enemy03.png")
#define Texture_Enemy03_Width_Total		(135)
#define Texture_Enemy03_Height_Total	(200)
#define Texture_Enemy03_Divide_X		(3)
#define Texture_Enemy03_Divide_Y		(4)
#define Texture_Enemy03_Width			(Texture_Enemy03_Width_Total / Texture_Enemy03_Divide_X)
#define Texture_Enemy03_Height			(Texture_Enemy03_Height_Total / Texture_Enemy03_Divide_Y)
#define Texture_Enemy04					_T("data/Texture/Enemy/Enemy04.png")
#define Texture_Enemy04_Width_Total		(150)
#define Texture_Enemy04_Height_Total	(256)
#define Texture_Enemy04_Divide_X		(3)
#define Texture_Enemy04_Divide_Y		(4)
#define Texture_Enemy04_Width			(Texture_Enemy04_Width_Total / Texture_Enemy04_Divide_X)
#define Texture_Enemy04_Height			(Texture_Enemy04_Height_Total / Texture_Enemy04_Divide_Y)

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3		Pos;					// �G�l�~�[�̍��W
	D3DXVECTOR3		Rot;					// �e�N�X�`���̉�]�p�x
	float			Speed;					// �ړ��X�s�[�h
	float			BulletSpeed;			// �e�̃X�s�[�h
	float			Radius;					// �摜���S���璸�_�܂ł̋���
	float			HitRadius;				// �����蔻��̉~�̔��a
	float			BaseAngle;				// �摜���S���璸�_�܂ł̊p�x
	float			MoveAngle;				// �ړ�������ύX����p�x
	int				Direction;				// �G�l�~�[�̌���
	int				Width;					// �G�l�~�[�̉���
	int				Height;					// �G�l�~�[�̏c��
	int				AnimPattern;			// �G�l�~�[�̃A�j���p�^�[��
	int				Count;					// ���ݎ��Ԃ��v�Z����J�E���g
	int				MovePattern;			// �ړ��p�^�[��
	int				EnemyType;				// �G�l�~�[�̎��
	int				ShotTime;				// �e���J�n����
	int				BarrageType;			// �e���̎��
	float			HP;						// �̗�
	int				BulletType;				// �e�̎��
	int				State;					// ���
	int				WaitCount;				// ��؎���
	int				ExitCount;				// �ޏꎞ��
	int				Item[EnemyItem_Max];	// �A�C�e��
	bool			Exist;					// ���݂��ǂ����̃t���O
}ENEMY;

// �G�l�~�[�̕���
enum EnemyDirection
{
	Down,
	Left,
	Right,
	Up,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(int InitState);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(int Enemy_No);
void SetEnemy(int Status_No);
void SetEnemyColor(int Enemy_No, int Color);

#endif

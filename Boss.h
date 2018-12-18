//=============================================================================
//
// �{�X�w�b�_�[ [Boss.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "Barrage.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Boss				_T("data/Texture/Boss/Boss.png")
#define Texture_Boss_Width_Total	(320)
#define Texture_Boss_Height_Total	(480)
#define Texture_Boss_Divide_X		(4)		// �e�N�X�`�����������i��)
#define Texture_Boss_Divide_Y		(6)		// �e�N�X�`�����������i�c)
#define Texture_Boss_Width			(Texture_Boss_Width_Total / Texture_Boss_Divide_X)
#define Texture_Boss_Height			(Texture_Boss_Height_Total / Texture_Boss_Divide_Y)
#define Texture_BossLife			_T("data/Texture/Boss/BossLife.png")
#define Texture_BossLife_Width		(32)
#define Texture_BossLife_Height		(32)


typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3		Pos;					// ���W
	D3DXVECTOR3		BeforeMovePos;			// �ړ��n�܂�O�̍��W
	D3DXVECTOR3		Velocity;				// �����x
	D3DXVECTOR3		Acceleration;			// �ړ��̉����x
	D3DXVECTOR3		Destination;			// �ψʂ̋���
	D3DXVECTOR3		Rot;					// �e�N�X�`���̉�]�p�x
	BULLET			Bullet[BossBullet_Max];	// �{�X�̒e
	float			Radius;					// �摜���S���璸�_�܂ł̋���
	float			HitRadius;				// �����蔻��̉~�̔��a
	float			BaseAngle;				// �摜���S���璸�_�܂ł̊p�x
	float			MoveAngle;				// �ړ�������ύX����p�x
	float			ShotAngle;				// �e���˂̃x�[�X�p�x
	float			MoveTime;				// �ړI���W�܂łɂ�����ړ�����
	float			HP;						// �̗�
	float			HP_Max;					// �ő�̗�
	int				AnimPattern;			// �A�j���p�^�[��
	int				Count;					// ���ݎ��Ԃ��v�Z����J�E���g
	int				ShotCount;				// ���ˎ��Ԃ��v�Z����J�E���g
	int				MoveCount;				// �ړ����Ԃ��v�Z����J�E���g
	int				MovePattern;			// �ړ��p�^�[��
	int				Color;					// �F
	int				Life;					// �c�@��
	int				BulletType;				// �e�̎��
	int				State;					// ���
	int				Phase;					// ���͉��i�K
	bool			Exist;					// ���݂��ǂ����̃t���O
	bool			MiddleBossOver;			// ���{�X�I�����̃t���O
	bool			InMove;					// �ړ����Ă邩�̃t���O
	bool			DebutMove;				// ���o��̈ړ����̃t���O
	bool			BombDamage;				// �{���_���[�W�󂯂��t���O
}BOSS;

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	bool			Use;					// �g�p���ǂ����̃t���O
}BOSSLIFE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBoss(int InitState);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
BOSS *GetBoss(void);
void SetBoss(void);
void SetBossColor(int Color);
void BossMove(D3DXVECTOR3 DestinationPos, float MoveTime);

#endif

//=============================================================================
//
// �G�l�~�[�e���w�b�_�[ [Barrage.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _BARRAGE_H_
#define _BARRAGE_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Bullet_00		_T("data/Texture/Bullet/Bullet_00.png")
#define Texture_Bullet_01		_T("data/Texture/Bullet/Bullet_01.png")
#define Texture_Bullet_02		_T("data/Texture/Bullet/Bullet_02.png")
#define Texture_Bullet_03		_T("data/Texture/Bullet/Bullet_03.png")
#define Texture_Bullet_04		_T("data/Texture/Bullet/Bullet_04.png")
#define Texture_Bullet_05		_T("data/Texture/Bullet/Bullet_05.png")

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3		Pos;					// �e�̍��W
	D3DXVECTOR3		PreviousPos;			// 1�t���C���O�̒e�̍��W
	D3DXVECTOR3		Rot;					// �e�N�X�`���̉�]�p�x
	float			Radius;					// �摜���S���璸�_�܂ł̋���
	float			HitRadius;				// �����蔻��̉~�̔��a
	float			BaseAngle;				// �摜���S���璸�_�܂ł̊p�x
	float			MoveAngle;				// �ړ�������ύX����p�x
	float			Speed;					// �e�̃X�s�[�h
	float			Damage;					// �_���[�W
	int				Type;					// �e�̃^�C�v
	int				Count;					// �J�E���g
	int				Color;					// �F
	int				State;					// �e�̏��
	bool			Use;					// ���˂��ꂽ���̃t���O
	bool			DelayDisappear;			// �������x��邩�̃t���O
}BULLET;

typedef struct
{
	BULLET			Bullet[EnemyBullet_Max];// �e���̃o���b�g
	float			BaseAngle;				// �e���̊�{�p�x
	int				Phase;					// �e���̒i�K
	int				Type;					// �e���̎��
	int				Count;					// �J�E���g
	int				Attacker_No;			// �ǂ̃G�l�~�[����̒e��
}BARRAGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBarrage(int InitState);
void UninitBarrage(void);
void UpdateBarrage(void);
void DrawBarrage(void);
void SetBarrage(int Enemy_No);
BARRAGE *GetBarrage(int Barrage_No);
void PlayerShot(void);

#endif

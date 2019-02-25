//=============================================================================
//
// ���[�h�t�@�C���w�b�_�[ [File.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef FILE_H
#define FILE_H


/*******************************************************************************
* �}�N����`
*******************************************************************************/



/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct
{
	D3DXVECTOR3		Pos;					// �������W
	float			Speed;					// �ړ��X�s�[�h
	float			BulletSpeed;			// �e�̃X�s�[�h
	float			HP_Max;					// �̗�
	int				AppearCount;			// �J�E���^
	int				MovePattern;			// �ړ��p�^�[��
	int				EnemyType;				// �G�̎��
	int				EnemyDifficulty;		// �ǂ�ȓ�Փx�ŏo������
	int				ShotTime;				// �e���J�n����
	int				BarrageType;			// �e���̎��
	int				BulletType;				// �e�̎��
	int				WaitCount;				// ��؎���
	int				ExitCount;				// �ޏꎞ��
	int				Item[EnemyItem_Max];	// �A�C�e��
}ENEMYSTATUS;

typedef struct
{
	int				Width;					// ����
	int				Height;					// �c��
	int				Devide_X;				// �e�N�X�`�����������i��)
	float			Radius;					// �摜���S���璸�_�܂ł̋���
	float			HitRadius;				// �����蔻��̉~�̔��a
	float			BaseAngle;				// �摜���S���璸�_�܂ł̊p�x
	float			Rot_z;					// �摜�̉�]�p�x
}BULLETTYPE;

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT LoadEnemyStatus(void);
ENEMYSTATUS *GetEnemyStatus(int StatusType_No);
HRESULT LoadBulletType(void);
BULLETTYPE *GetBulletType(int Type_No);

#endif

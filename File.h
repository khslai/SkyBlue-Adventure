//=============================================================================
//
// ���[�h�t�@�C���w�b�_�[ [File.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef FILE_H
#define FILE_H


//******************************************************************************
// �}�N����`
//*****************************************************************************/


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �G�l�~�[�X�e�[�^�X�\����
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

// �e�^�C�v�\����
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

//******************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************/
// �G�l�~�[�X�e�[�^�X��ǂݍ���
HRESULT LoadEnemyStatus(void);
// �G�l�~�[�X�e�[�^�X�̏����擾����
ENEMYSTATUS *GetEnemyStatus(int StatusType_No);
// �e�^�C�v��ǂݍ���
HRESULT LoadBulletType(void);
// �e�^�C�v�̏����擾����
BULLETTYPE *GetBulletType(int Type_No);

#endif

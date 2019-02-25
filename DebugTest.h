//=============================================================================
//
// �f�o�b�O�e�X�g�w�b�_�[ [DebugTest.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _DEBUGTEST_H_
#define _DEBUGTEST_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_HitCircle			_T("data/Texture/Other/HitCircle.png")
#define Texture_HitCircle_Width		(50)
#define Texture_HitCircle_Height	(50)

// HitCircle�̎��
enum HitCircleType
{
	PlayerHC,
	PlayerBulletHC,
	BombHC,
	EnemyHC,
	EnemyBulletHC,
	BossHC,
	BossBulletHC,
};


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ������͈͍\����
typedef struct 
{
	VERTEX_2D		VertexWk[Num_Vertex];	// ���_���i�[���[�N
	D3DXVECTOR3		Pos;					// ���W
	D3DXVECTOR3		Rot;					// �e�N�X�`���̉�]�p�x
	float			Radius;					// �摜���S���璸�_�܂ł̋���
	float			BaseAngle;				// �摜���S���璸�_�܂ł̊p�x
	int				Owner;					// ����HitCircle�̏��L��
	int				EnemyNo;				// ����HitCircle�Ή�����G�l�~�[�ԍ�
	int				BulletNo;				// ����HitCircle�Ή�����o���b�g�ԍ�
	int				BarrageNo;				// ����HitCircle�Ή�����e���ԍ�
	bool			Use;					// �g�p�t���O
}HITCIRCLE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitDebugTest(int InitState);
// �I������
void UninitDebugTest(void);
// �X�V����
void UpdateDebugTest(void);
// �`�揈��
void DrawDebugTest(void);
// �v���C���[�̓�����͈͂̐ݒu
void SetPlayerHC(D3DVECTOR Pos, float HitRadius, int Owner);
// �v���C���[�e�̓�����͈͂̐ݒu
void SetPlayerBulletHC(D3DVECTOR Pos, float HitRadius, int Bullet_No, int Owner);
// �{���̓�����͈͂̐ݒu
void SetBombHC(D3DVECTOR Pos, float HitRadius, int Owner);
// �G�l�~�[�̓�����͈͂̐ݒu
void SetEnemyHC(D3DVECTOR Pos, float HitRadius, int Enemy_No, int Owner);
// �G�l�~�[�e�̓�����͈͂̐ݒu
void SetEnemyBulletHC(D3DVECTOR Pos, float HitRadius, int Barrage_No, int Bullet_No, int Owner);
// �{�X�̓�����͈͂̐ݒu
void SetBossHC(D3DVECTOR Pos, float HitRadius, int Owner);
// �{�X�e�̓�����͈͂̐ݒu
void SetBossBulletHC(D3DVECTOR Pos, float HitRadius, int Bullet_No, int Owner);

#endif

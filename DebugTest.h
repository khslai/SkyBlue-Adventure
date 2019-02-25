//=============================================================================
//
// �f�o�b�O�e�X�g�w�b�_�[ [DebugTest.h]
// Author�FTH_GP11_GP11B341_35_���M��
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

typedef struct 
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// ���_���i�[���[�N
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
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDebugTest(int InitState);
void UninitDebugTest(void);
void UpdateDebugTest(void);
void DrawDebugTest(void);
void SetPlayerHC(D3DVECTOR Pos, float HitRadius, int Owner);
void SetPlayerBulletHC(D3DVECTOR Pos, float HitRadius, int Bullet_No, int Owner);
void SetBombHC(D3DVECTOR Pos, float HitRadius, int Owner);
void SetEnemyHC(D3DVECTOR Pos, float HitRadius, int Enemy_No, int Owner);
void SetEnemyBulletHC(D3DVECTOR Pos, float HitRadius, int Barrage_No, int Bullet_No, int Owner);
void SetBossHC(D3DVECTOR Pos, float HitRadius, int Owner);
void SetBossBulletHC(D3DVECTOR Pos, float HitRadius, int Bullet_No, int Owner);


#endif

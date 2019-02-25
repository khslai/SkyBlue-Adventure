//=============================================================================
//
// �G�t�F�N�g�w�b�_�[ [Effect.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_DeadEffect				_T("data/Texture/Enemy/DeadEffect.png")
#define Texture_DeadEffect_Width_Total	(700)
#define Texture_DeadEffect_Height_Total	(140)
#define Texture_DeadEffect_Divide_X		(5)
#define Texture_DeadEffect_Divide_Y		(1)
#define Texture_DeadEffect_Width		(Texture_DeadEffect_Width_Total / Texture_DeadEffect_Divide_X)
#define Texture_DeadEffect_Height		(Texture_DeadEffect_Height_Total / Texture_DeadEffect_Divide_Y)
#define Texture_BulletDead				_T("data/Texture/Bullet/BulletDead.png")
#define Texture_BulletDead_Width_Total	(600)
#define Texture_BulletDead_Height_Total	(60)
#define Texture_BulletDead_Divide_X		(10)
#define Texture_BulletDead_Divide_Y		(1)	
#define Texture_BulletDead_Width		(Texture_BulletDead_Width_Total / Texture_BulletDead_Divide_X)
#define Texture_BulletDead_Height		(Texture_BulletDead_Height_Total / Texture_BulletDead_Divide_Y)
#define Texture_BossBG_Circle			_T("data/Texture/Boss/BossBG_Circle.png")
#define Texture_BossBG_Circle_Width		(300)
#define Texture_BossBG_Circle_Height	(300)
#define Texture_BossBG_Star				_T("data/Texture/Boss/BossBG_Star.png")
#define Texture_BossBG_Star_Width		(200)
#define Texture_BossBG_Star_Height		(200)

typedef struct 
{
	VERTEX_2D		VertexWk[NUM_VERTEX];		// ���_���i�[���[�N
	D3DXVECTOR3		Pos;						// ���W
	D3DXVECTOR3		Rot;						// �e�N�X�`���̉�]�p�x
	float			Radius;						// �摜���S���璸�_�܂ł̋���
	float			BaseAngle;					// �摜���S���璸�_�܂ł̊p�x
	int				Count;						// �J�E���g
	int				AnimPattern;				// �F
	int				Alpha;						// ���Z�����A���������p�A���t�@�l
	int				TextureType;				// �G�t�F�N�g�e�N�X�`���̎��
	bool			Use;						// �g�p�t���O
}EFFECT;

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];		// ���_���i�[���[�N
	D3DXVECTOR3		Pos;						// ���W
	int				Count;						// �J�E���g
	int				AnimPattern;				// �e�N�X�`���̉��ԕ���
	int				Type;						// �G�t�F�N�g�̎��
	bool			Use;						// �g�p�t���O
}EFFECTCOUNTER;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffect(int InitState);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEnemyDeadEffect(int Enemy_No);
void SetBossDeadEffect(void);
void SetBossEffect(void);
void SetBulletDeadEffect(D3DXVECTOR3 Pos);

#endif

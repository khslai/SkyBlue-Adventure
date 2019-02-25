//=============================================================================
//
// �{���w�b�_�[ [Bomb.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _BOMB_H_
#define _BOMB_H_

#include "Barrage.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_ShotBomb				_T("data/Texture/Player/ShotBomb.png")
#define Texture_ShotBomb_Width_Total	(480)
#define Texture_ShotBomb_Height_Total	(96)
#define Texture_ShotBomb_Divide_X		(5)		// �e�N�X�`�����������i��)
#define Texture_ShotBomb_Divide_Y		(1)		// �e�N�X�`�����������i�c)
#define Texture_ShotBomb_Width			(Texture_ShotBomb_Width_Total / Texture_ShotBomb_Divide_X)
#define Texture_ShotBomb_Height			(Texture_ShotBomb_Height_Total / Texture_ShotBomb_Divide_Y)
#define Texture_BombExtend				_T("data/Texture/Player/BombExtend.png")
#define Texture_BombExtend_Width		(64)
#define Texture_BombExtend_Height		(64)
#define Texture_WhiteScreen				_T("data/Texture/Player/WhiteScreen.png")
#define Texture_WhiteScreen_Width		(1024)
#define Texture_WhiteScreen_Height		(768)
#define Texture_LockOn					_T("data/Texture/Player/LockOn.png")
#define Texture_LockOn_Width			(64)
#define Texture_LockOn_Height			(64)
#define Texture_BombBullet				_T("data/Texture/Player/BombBullet.png")
#define Texture_BombBullet_Width		(32)
#define Texture_BombBullet_Height		(32)


// �{���̒e�̏��
enum BombBulletState
{
	Homing,				// �z�[�~���O
	Explosion,			// �������̔���
	Over,				// �I��
	BulletDisspappear,	// �ڕW�������A�e����
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ���b�N�I���\����
typedef struct
{
	VERTEX_2D		VertexWk[Num_Vertex];			// ���_���i�[���[�N
	D3DXVECTOR3		Pos;							// ���W
	float			Radius;							// �摜���S���璸�_�܂ł̋���
	float			BaseAngle;						// �摜���S���璸�_�܂ł̊p�x
	int				Target_No;						// ���b�N�I�����ꂽ�G�l�~�[�̔ԍ�
	bool			Use;							// �g�p�t���O
}LOCKON;

// �{���\����
typedef struct
{
	VERTEX_2D		VertexWk[Num_Vertex];			// ���_���i�[���[�N
	D3DXVECTOR3		Pos;							// ���W
	D3DXVECTOR3		ShakePos;						// ��ʗh�����ʗp���W
	D3DXVECTOR3		Rot;							// �e�N�X�`���̉�]�p�x
	LOCKON			LockOn[Enemy_Max];				// �Ə�
	BULLET			Bullet[Enemy_Max];				// �{���̒e
	float			Radius;							// �摜���S���璸�_�܂ł̋���
	float			HitRadius;						// �����蔻��̉~�̔��a
	float			BaseAngle;						// �摜���S���璸�_�܂ł̊p�x
	float			ExplosionRadius[BombBullet_Max];// �������a
	int				BulletTarget[BombBullet_Max];	// �e�̃^�[�Q�b�g
	int				LockOnNum;						// ���b�N�I�����ꂽ�G�l�~�[�̐�
	int				AnimPattern;					// �v���C���[�̃A�j���p�^�[��
	int				Count;							// �J�E���g
	int				Alpha;							// ���������p�A���t�@�l
	int				Phase;							// �{�����L������
	bool			Use;							// �g�p�t���O
}BOMB;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitBomb(int InitState);
// �I������
void UninitBomb(void);
// �X�V����
void UpdateBomb(void);
// �`�揈��
void DrawBomb(void);
// �{�X�i�^�C�vA�j�̐ݒu
void SetBomb_A(void);
// �{�X�i�^�C�vB�j�̐ݒu
void SetBomb_B(void);
// �{���̏����擾����
BOMB *GetBomb(void);

#endif

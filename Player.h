//=============================================================================
//
// �v���C���[�w�b�_�[ [Player.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Barrage.h"
#include "Bomb.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_PlayerDebut_A			_T("data/Texture/Player/PlayerDebut_A.png")
#define Texture_PlayerDebut_B			_T("data/Texture/Player/PlayerDebut_B.png")
#define Texture_PlayerDebut_Width_Total	(480)
#define Texture_PlayerDebut_Divide_X	(5)
#define Texture_PlayerDebut_Width		(Texture_PlayerDebut_Width_Total / Texture_PlayerDebut_Divide_X)
#define Texture_PlayerDebut_Height		(96)
#define Texture_Player_A				_T("data/Texture/Player/Player_A.png")
#define Texture_Player_B				_T("data/Texture/Player/Player_B.png")
#define Texture_Player_Width_Total		(288)
#define Texture_Player_Height_Total		(288)
#define Texture_Player_Divide_X			(3)		// �v���C���[�̃e�N�X�`�����������i��)
#define Texture_Player_Divide_Y			(3)		// �v���C���[�̃e�N�X�`�����������i�c)
#define Texture_Player_Width			(Texture_Player_Width_Total / Texture_Player_Divide_X)
#define Texture_Player_Height			(Texture_Player_Height_Total / Texture_Player_Divide_Y)
#define Texture_PlayerOption			_T("data/Texture/Player/PlayerOption.png")
#define Texture_PlayerOption_Width		(32)
#define Texture_PlayerOption_Height		(32)
#define Texture_HitPoint				_T("data/Texture/Player/HitPoint.png")
#define Texture_HitPoint_Width			(19)
#define Texture_HitPoint_Height			(19)


// �I�v�V�����\����
typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];		// ���_���i�[���[�N
	D3DXVECTOR3		Pos;						// �v���C���[�̍��W
	D3DXVECTOR3		Rot;						// �e�N�X�`���̉�]�p�x
	float			Radius;						// �摜���S���璸�_�܂ł̋���
	float			BaseAngle;					// �摜���S���璸�_�܂ł̊p�x
	int				BulletType;					// �e�̃^�C�v
	bool			Use;						// �g�p�t���O
}OPTION;

// ������|�C���g�\����
typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];		// ���_���i�[���[�N
	D3DXVECTOR3		Rot;						// �e�N�X�`���̉�]�p�x
	float			Radius;						// �摜���S���璸�_�܂ł̋���
	float			BaseAngle;					// �摜���S���璸�_�܂ł̊p�x
}HITPOINT;

// �v���C���[�\����
typedef	struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];		// ���_���i�[���[�N
	D3DXVECTOR3		Pos;						// �v���C���[�̍��W
	D3DXVECTOR3		Rot;						// �e�N�X�`���̉�]�p�x
	BULLET			Bullet[PlayerBullet_Max];	// �v���C���[�̃o���b�g
	OPTION			Option[PlayerOption_Max];	// �v���C���[�̃I�v�V����
	HITPOINT		HitPoint;					// ������|�C���g
	float			Radius;						// �摜���S���璸�_�܂ł̋���
	float			HitRadius;					// �����蔻��̉~�̔��a
	float			BaseAngle;					// �摜���S���璸�_�܂ł̊p�x
	float			Power;						// �v���C���[�̃p���[
	float			HP;							// �̗�
	int				Type;						// ���@�̃^�C�v
	int				BombNum;					// ���{���̐�
	int				AnimCount;					// �v���C���[�̃A�j���J�E���g
	int				AnimPattern;				// �v���C���[�̃A�j���p�^�[��
	int				Direction;					// �v���C���[�̌���
	int				ShotCount;					// �v���C���[�̃V���b�g�J�E���g
	int				BulletType;					// �e�̃^�C�v
	int				InvincibleCount;			// �����ꂽ��̖��G�J�E���g
	int				Score;						// �X�R�A
	bool			Exist;						// ���݃t���O
	bool			LowSpeedMode;				// �ᑬ���[�h�̃t���O
	bool			InBomb;						// �{�����˒�
	bool			ClearStandBy;				// �Q�[���N���A���̏����t���O
}PLAYER;

// �v���C���[�̃^�C�v
enum
{
	Player_A,
	Player_B,
};

// �I�v�V�����̃^�C�v
enum
{
	Option_A,
	Option_B,
};

// �v���C���[�̕���
enum
{
	Forward,
	Flying,
	Backward,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(int InitState);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(void);
PLAYER *GetPlayer(int Player_No);

#endif

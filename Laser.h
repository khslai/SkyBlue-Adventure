//=============================================================================
//
// ���[�U�[�w�b�_�[ [Laser.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _LASER_H_
#define _LASER_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Laser			_T("data/Texture/Bullet/Laser.png")
#define Texture_Laser_Width		(30)
#define Texture_Laser_Height	(460)

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3		RotateCenterPos;		// ��]���S��Pos���L�^����
	D3DXVECTOR3		Pos;					// ���S�_�̍��W����Ȃ��A�����̐^�񒆂̍��W
											//  ������������������������
											// ��|          |
											//  ������������������������
	D3DXVECTOR3		BeforeRotPos;			// ��]�n�܂�O��Pos���L�^����
	float			RotateTime;				// ��]�K�v�̎���
	float			BeforeRotAngle;			// ��]�n�܂�O��MoveAngle���L�^����
	float			MoveAngle;				// �ړ��p�x
	float			RotateAngle;			// ��]�������p�x
	float			Width;					// ���[�U�[�̕�
	float			Length;					// ���[�U�[�̒���
	int				State;					// ��]�̕������L�^����A
	int				Count;					// �J�E���g
	bool			Use;					// �g�p�����ǂ����̃t���O
	bool			InRotate;				// ���͉�]���ǂ����̃t���O
}LASER;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitLaser(int InitState);
void UninitLaser(void);
void UpdateLaser(void);
void DrawLaser(void);
void SetLaser();
LASER *GetLaser(int Laser_No);

#endif

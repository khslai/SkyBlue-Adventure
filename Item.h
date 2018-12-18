//=============================================================================
//
// �A�C�e���w�b�_�[ [Item.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_HPItem					_T("data/Texture/Item/HPItem.png")
#define Texture_HPItem_Width			(35)
#define Texture_HPItem_Height			(35)
#define Texture_ScoreItem				_T("data/Texture/Item/ScoreItem.png")
#define Texture_ScoreItem_Width			(35)
#define Texture_ScoreItem_Height		(35)
#define Texture_PowerItem				_T("data/Texture/Item/PowerItem.png")
#define Texture_PowerItem_Width			(35)
#define Texture_PowerItem_Height		(35)
#define Texture_ItemBackGround			_T("data/Texture/Item/ItemBackGround.png")
#define Texture_ItemBackGround_Width	(35)
#define Texture_ItemBackGround_Height	(35)

typedef struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3		Pos;					// ���W
	D3DXVECTOR3		Rot;					// �e�N�X�`���̉�]�p�x
	float			Radius;					// �摜���S���璸�_�܂ł̋���
	float			BaseAngle;				// �摜���S���璸�_�܂ł̊p�x
	float			MoveAngle;				// �ړ�������ύX����p�x
	float			Speed;					// �ړ��X�s�[�h
	int				Color;					// �F
	int				Type;					// �A�C�e���̎��
	int				Count;					// �J�E���g
	bool			Use;					// �g�p�t���O
	bool			InAttract;				// �A�C�e���z����Ԃ̃t���O
}ITEM;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitItem(int InitState);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
ITEM *GetItem(int Item_No);
void SetPlayerItem(D3DXVECTOR3 Pos);
void SetEnemyItem(D3DXVECTOR3 Pos, int ItemType[], int ItemNum);
void SetBossItem(D3DXVECTOR3 Pos);
void SetBossBulletItem(D3DXVECTOR3 Pos);

#endif

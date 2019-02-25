//=============================================================================
//
// �A�C�e���w�b�_�[ [Item.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
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

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �A�C�e���\����
typedef struct
{
	VERTEX_2D		VertexWk[Num_Vertex];	// ���_���i�[���[�N
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
// ����������
HRESULT InitItem(int InitState);
// �I������
void UninitItem(void);
// �X�V����
void UpdateItem(void);
// �`�揈��
void DrawItem(void);
// �A�C�e���̏����擾����
ITEM *GetItem(int Item_No);
// �v���C���[�������ꂽ���A�h���b�v�A�C�e���̐ݒu
void SetPlayerItem(D3DXVECTOR3 Pos);
// �G�l�~�[���S���鎞�A�h���b�v�A�C�e���̐ݒu
void SetEnemyItem(D3DXVECTOR3 Pos, int ItemType[], int ItemNum);
// �{�X���j���鎞�A�h���b�v�A�C�e���̐ݒu
void SetBossItem(D3DXVECTOR3 Pos);
// �{�X���j���鎞�A�o���b�g���X�R�A�A�C�e���ɂȂ�
void SetBossBulletItem(D3DXVECTOR3 Pos);

#endif

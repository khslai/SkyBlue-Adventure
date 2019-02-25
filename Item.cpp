//=============================================================================
//
// �A�C�e������ [Item.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "Item.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �A�C�e���������I�ɋz�������{�[�_�[���C���̍��W
#define AttractBorderline (Screen_Width * 0.65)
// �v���C���[���A�C�e�����z�����锼�a
#define PlayerAttractRadius (80)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeItemVertex(int Item_No);
// ���_���W�̐ݒ�
void SetItemVertex(int Item_No);
// �e�N�X�`�����W�̐ݒ�
void SetItemTexture(int Item_No);
// �F�A�����x�̐ݒ�
void SetItemDiffuse(int Item_No, int Color);
// �A�C�e���̉�]�A�ړ��A�z���̌v�Z
void CalculateItem(int Item_No);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���̃|�C���^
static LPDIRECT3DTEXTURE9 HPTexture = NULL;
static LPDIRECT3DTEXTURE9 ScoreTexture = NULL;
static LPDIRECT3DTEXTURE9 PowerTexture = NULL;
static LPDIRECT3DTEXTURE9 BackGroundTexture = NULL;
// �e�N�X�`�����a
static float ItemRadius = 0.0f;
// ���S�_�Ǝl���_�̐����p
static float ItemBaseAngle = 0.0f;
// �A�C�e���\����
ITEM Item[ItemNum_Max];


//=============================================================================
// ����������
//=============================================================================
HRESULT InitItem(int InitState)
{
	int Item_No = 0;
	D3DXVECTOR2 temp = D3DXVECTOR2(0.0f, 0.0f);

	for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
	{
		Item[Item_No].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Item[Item_No].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Item[Item_No].Radius = 0.0f;
		Item[Item_No].BaseAngle = 0.0f;
		Item[Item_No].BaseAngle = 0.0f;
		Item[Item_No].Color = 0;
		Item[Item_No].Type = 0;
		Item[Item_No].Count = 0;
		Item[Item_No].Use = false;
		Item[Item_No].InAttract = false;

		// ���_���̍쐬
		MakeItemVertex(Item_No);
	}

	temp = D3DXVECTOR2(Texture_HPItem_Width / 2, Texture_HPItem_Height / 2);
	ItemRadius = D3DXVec2Length(&temp) * 1.5f;
	ItemBaseAngle = atan2f(Texture_HPItem_Height, Texture_HPItem_Width);

	if (InitState == FirstInit)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_HPItem, &HPTexture, "Item") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_ScoreItem, &ScoreTexture, "Item") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_PowerItem, &PowerTexture, "Item") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_ItemBackGround, &BackGroundTexture, "Item") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitItem(void)
{
	SafeRelease(HPTexture);
	SafeRelease(ScoreTexture);
	SafeRelease(PowerTexture);
	SafeRelease(BackGroundTexture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateItem(void)
{
	int Item_No = 0;

	for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
	{
		if (Item[Item_No].Use == true)
		{
			CalculateItem(Item_No);
			if (Item[Item_No].Type == ItemBackGround)
			{
				SetItemDiffuse(Item_No, Item[Item_No].Color);
			}
			SetItemTexture(Item_No);
			SetItemVertex(Item_No);
		}
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawItem(void)
{
	int Item_No = 0;
	LPDIRECT3DDEVICE9 Device = GetDevice();

	Device->SetFVF(FVF_VERTEX_2D);

	for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
	{
		if (Item[Item_No].Use == true && Item[Item_No].Count != 0)
		{
			switch (Item[Item_No].Type)
			{
			case HPItem:
				Device->SetTexture(0, HPTexture);
				break;
			case ScoreItem:
				Device->SetTexture(0, ScoreTexture);
				break;
			case PowerItem:
				Device->SetTexture(0, PowerTexture);
				break;
			case ItemBackGround:
				Device->SetTexture(0, BackGroundTexture);
				break;
			default:
				break;
			}
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Item[Item_No].VertexWk, sizeof(VERTEX_2D));
		}
	}

	return;
}

//=============================================================================
// �A�C�e���̉�]�A�ړ��A�z���̌v�Z
//=============================================================================
void CalculateItem(int Item_No)
{
	PLAYER *Player = GetPlayer(0);
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Distance = 0.0f;

	// �v���C���[�Ƃ̋����v�Z
	temp = Item[Item_No].Pos - Player->Pos;
	Distance = D3DXVec3LengthSq(&temp);

	// ����
	if (Item[Item_No].Speed < 2.5f)
	{
		Item[Item_No].Speed += 0.1f;
	}
	// ���x����������A����
	else if (Item[Item_No].Speed > 2.5f && Item[Item_No].InAttract == false)
	{
		Item[Item_No].Speed = 2.5f;
	}
	// �A�C�e���w�i�Ȃ�A��]������
	if (Item[Item_No].Type == ItemBackGround)
	{
		Item[Item_No].Rot.z += 0.05f * ((Item_No % 3) + 1);
	}
	// 1. �v���C���[�͋z�����C���𒴂�������
	// 2. �z���͈͓�
	// 3. �A�C�e���͈ړ��s�\�͈͓�
	// �z����ԂɂȂ�
	if (Player->Pos.x >= AttractBorderline ||
		Distance < (PlayerAttractRadius * PlayerAttractRadius) ||
		Item[Item_No].Pos.y <= MoveRange_Y)
	{
		Item[Item_No].InAttract = true;
	}
	// �z����ԂȂ�
	if (Item[Item_No].InAttract == true)
	{
		// �v���C���[�Ɍ������Ĉړ�����
		Item[Item_No].MoveAngle = AngleCalculate(Item[Item_No].Pos, Player->Pos);
		Item[Item_No].Speed += 1.0f;
	}

	// �ړ��v�Z
	Item[Item_No].Pos.x += cosf(Item[Item_No].MoveAngle) * Item[Item_No].Speed;
	Item[Item_No].Pos.y += sinf(Item[Item_No].MoveAngle) * Item[Item_No].Speed;

	// �͈̓`�F�b�N
	if ((Item[Item_No].Pos.x + Texture_HPItem_Width / 2) <= 0)
	{
		Item[Item_No].Use = false;
	}

	Item[Item_No].Count++;

	return;
}

//=============================================================================
// �A�C�e���̏����擾����
//=============================================================================
ITEM *GetItem(int Item_No)
{
	return &Item[Item_No];
}

//=============================================================================
// �v���C���[�������ꂽ���A�h���b�v�A�C�e���̐ݒu
//=============================================================================
void SetPlayerItem(D3DXVECTOR3 Pos)
{
	int i = 0, j = 0;
	int Item_No = 0;
	D3DXVECTOR3 PosTemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (i = 0; i < PlayerItem_Max; i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
			{
				if (Item[Item_No].Use == false)
				{
					if (j == 0)
					{
						Item[Item_No].Pos.x = Pos.x + rand() % (50 + 1) + PlayerAttractRadius;
						Item[Item_No].Pos.y = Pos.y + rand() % (100 + 1) - 50;
						PosTemp = Item[Item_No].Pos;
						// �w�肵���A�C�e�����o��������
						Item[Item_No].Type = PowerItem;
					}
					else
					{
						Item[Item_No].Pos = PosTemp;
						// �A�C�e���w�i���o��������
						Item[Item_No].Type = ItemBackGround;
						Item[Item_No].Color = PowerItem;
					}
					Item[Item_No].Use = true;
					Item[Item_No].Radius = ItemRadius;
					Item[Item_No].BaseAngle = ItemBaseAngle;
					Item[Item_No].MoveAngle = DegreeToRadian(180);
					Item[Item_No].Rot.z = 0.0f;
					Item[Item_No].Speed = -4.0f;
					Item[Item_No].Count = 0;
					Item[Item_No].InAttract = false;
					break;
				}
			}
		}
	}

	return;
}

//=============================================================================
// �G�l�~�[���S���鎞�A�h���b�v�A�C�e���̐ݒu
//=============================================================================
void SetEnemyItem(D3DXVECTOR3 Pos, int ItemType[], int ItemNum)
{
	int i = 0, j = 0;
	int Item_No = 0;
	D3DXVECTOR3 PosTemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (i = 0; i < ItemNum; i++)
	{
		if (ItemType[i] != Nothing)
		{
			for (j = 0; j < 2; j++)
			{
				for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
				{
					if (Item[Item_No].Use == false)
					{
						if (j == 0)
						{
							Item[Item_No].Pos = Pos;
							// �����Ȃ�K���ɂ���΂点��
							if (i > 0)
							{
								Item[Item_No].Pos.x += rand() % (100 + 1) - 50;
								Item[Item_No].Pos.y += rand() % (100 + 1) - 50;
							}
							PosTemp = Item[Item_No].Pos;
							// �w�肵���A�C�e�����o��������
							Item[Item_No].Type = ItemType[i];
						}
						else
						{
							Item[Item_No].Pos = PosTemp;
							// �A�C�e���w�i���o��������
							Item[Item_No].Type = ItemBackGround;
							Item[Item_No].Color = ItemType[i];
						}
						Item[Item_No].Use = true;
						Item[Item_No].Radius = ItemRadius;
						Item[Item_No].BaseAngle = ItemBaseAngle;
						Item[Item_No].MoveAngle = DegreeToRadian(180);
						Item[Item_No].Rot.z = 0.0f;
						Item[Item_No].Speed = -4.0f;
						Item[Item_No].Count = 0;
						Item[Item_No].InAttract = false;
						break;
					}
				}
			}
		}
	}

	return;
}

//=============================================================================
// �{�X���j���鎞�A�h���b�v�A�C�e���̐ݒu
//=============================================================================
void SetBossItem(D3DXVECTOR3 Pos)
{
	int i = 0, j = 0;
	int Item_No = 0;
	int TypeTemp = 0;
	D3DXVECTOR3 PosTemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (i = 0; i < BossItem_Max; i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
			{
				if (Item[Item_No].Use == false)
				{
					if (j == 0)
					{
						Item[Item_No].Pos.x = Pos.x + rand() % (100 + 1) - 50;
						Item[Item_No].Pos.y = Pos.y + rand() % (100 + 1) - 50;
						PosTemp = Item[Item_No].Pos;
						// �A�C�e���̓����_���ɏo��������
						Item[Item_No].Type = rand() % (2 + 1);
						TypeTemp = Item[Item_No].Type;
					}
					else
					{
						// �A�C�e���w�i���o��������
						Item[Item_No].Pos = PosTemp;
						Item[Item_No].Type = ItemBackGround;
						Item[Item_No].Color = TypeTemp;
					}
					Item[Item_No].Use = true;
					Item[Item_No].Radius = ItemRadius;
					Item[Item_No].BaseAngle = ItemBaseAngle;
					Item[Item_No].MoveAngle = DegreeToRadian(180);
					Item[Item_No].Rot.z = 0.0f;
					Item[Item_No].Speed = -4.0f;
					Item[Item_No].Count = 0;
					Item[Item_No].InAttract = false;
					break;
				}
			}
		}
	}

	return;
}

//=============================================================================
// �{�X���j���鎞�A�o���b�g���X�R�A�A�C�e���ɂȂ�
//=============================================================================
void SetBossBulletItem(D3DXVECTOR3 Pos)
{
	int i = 0;
	int Item_No = 0;

	for (i = 0; i < 2; i++)
	{
		for (Item_No = 0; Item_No < ItemNum_Max; Item_No++)
		{
			if (Item[Item_No].Use == false)
			{
				if (i == 0)
				{
					// �w�肵���A�C�e�����o��������
					Item[Item_No].Type = ScoreItem;
				}
				else
				{
					// �A�C�e���w�i���o��������
					Item[Item_No].Type = ItemBackGround;
					Item[Item_No].Color = ScoreItem;
				}
				Item[Item_No].Pos = Pos;
				Item[Item_No].Use = true;
				Item[Item_No].Radius = ItemRadius;
				Item[Item_No].BaseAngle = ItemBaseAngle;
				Item[Item_No].MoveAngle = DegreeToRadian(180);
				Item[Item_No].Rot.z = 0.0f;
				Item[Item_No].Speed = 0.0f;
				Item[Item_No].Count = 0;
				Item[Item_No].InAttract = true;
				break;
			}
		}
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeItemVertex(int Item_No)
{
	// ���_���W�̐ݒ�	
	SetItemVertex(Item_No);

	// rhw�̐ݒ�
	Item[Item_No].VertexWk[0].rhw = 1.0f;
	Item[Item_No].VertexWk[1].rhw = 1.0f;
	Item[Item_No].VertexWk[2].rhw = 1.0f;
	Item[Item_No].VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Item[Item_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Item[Item_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Item[Item_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Item[Item_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetItemTexture(Item_No);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetItemTexture(int Item_No)
{
	Item[Item_No].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Item[Item_No].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Item[Item_No].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Item[Item_No].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}



//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetItemVertex(int Item_No)
{
	Item[Item_No].VertexWk[0].vtx.x = Item[Item_No].Pos.x - cosf(Item[Item_No].BaseAngle + Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[0].vtx.y = Item[Item_No].Pos.y - sinf(Item[Item_No].BaseAngle + Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[1].vtx.x = Item[Item_No].Pos.x + cosf(Item[Item_No].BaseAngle - Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[1].vtx.y = Item[Item_No].Pos.y - sinf(Item[Item_No].BaseAngle - Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[2].vtx.x = Item[Item_No].Pos.x - cosf(Item[Item_No].BaseAngle - Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[2].vtx.y = Item[Item_No].Pos.y + sinf(Item[Item_No].BaseAngle - Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[3].vtx.x = Item[Item_No].Pos.x + cosf(Item[Item_No].BaseAngle + Item[Item_No].Rot.z) * Item[Item_No].Radius;
	Item[Item_No].VertexWk[3].vtx.y = Item[Item_No].Pos.y + sinf(Item[Item_No].BaseAngle + Item[Item_No].Rot.z) * Item[Item_No].Radius;

	return;
}

//=============================================================================
// �F�A�����x�̐ݒ�
//=============================================================================
void SetItemDiffuse(int Item_No, int Color)
{
	switch (Color)
	{
	case HPItem:
		Item[Item_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		Item[Item_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		Item[Item_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		Item[Item_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		break;
	case ScoreItem:
		Item[Item_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(0, 255, 33, 255);
		Item[Item_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(0, 255, 33, 255);
		Item[Item_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(0, 255, 33, 255);
		Item[Item_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(0, 255, 33, 255);
		break;
	case PowerItem:
		Item[Item_No].VertexWk[0].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		Item[Item_No].VertexWk[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		Item[Item_No].VertexWk[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		Item[Item_No].VertexWk[3].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		break;
	default:
		break;
	}

	return;
}

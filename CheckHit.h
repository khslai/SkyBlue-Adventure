//=============================================================================
//
// �����蔻��w�b�_�[ [CheckHit.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _CHECKHIT_H_
#define _CHECKHIT_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
typedef struct
{
	float x;
	float y;
	float Length;
}VECTOR;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void CheckHit(void);
void SetLaserCheckRect(D3DXVECTOR3 Point_LeftUp, D3DXVECTOR3 Point_RightDown);

#endif
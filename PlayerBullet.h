//=============================================================================
//
// �v���C���[�e���w�b�_�[ [PlayerBullet.h]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#ifndef _PLAYERBULLET_H_
#define _PLAYERBULLET_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_PlayerBullet_A		_T("data/Texture/Player/PlayerBullet_A.png")
#define Texture_PlayerBullet_B		_T("data/Texture/Player/PlayerBullet_B.png")
#define Texture_OptionBullet_A		_T("data/Texture/Player/OptionBullet_A.png")
#define Texture_OptionBullet_B		_T("data/Texture/Player/OptionBullet_B.png")


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayerBullet(int InitState);
void UninitPlayerBullet(void);
void UpdatePlayerBullet(void);
void DrawPlayerBullet(void);

#endif

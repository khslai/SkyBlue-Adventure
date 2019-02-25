//=============================================================================
//
// �v���C���[�e���w�b�_�[ [PlayerBullet.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
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
// ����������
HRESULT InitPlayerBullet(int InitState);
// �I������
void UninitPlayerBullet(void);
// �X�V����
void UpdatePlayerBullet(void);
// �`�揈��
void DrawPlayerBullet(void);
// �v���C���[�̒e����
void PlayerShot(void);

#endif

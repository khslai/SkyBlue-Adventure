//=============================================================================
//
// �L�[���̓w�b�_�[ [Input.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �v���O������������Ƃ��Ɏg��
#define	USE_KEYBOARD				// �錾����ƃL�[�{�[�h�ő���\�ɂȂ�
#define	USE_MOUSE					// �錾����ƃ}�E�X�ő���\�ɂȂ�
#define	USE_PAD						// �錾����ƃp�b�h�ő���\�ɂȂ�

/* Game Pad��� */
#define BUTTON_UP		0x00000001l	// ���X�e�B�b�N�A�����L�[��(.lY < 0)
#define BUTTON_DOWN		0x00000002l	// ���X�e�B�b�N�A�����L�[��(.lY > 0)
#define BUTTON_LEFT		0x00000004l	// ���X�e�B�b�N�A�����L�[��(.lX < 0)
#define BUTTON_RIGHT	0x00000008l	// ���X�e�B�b�N�A�����L�[�E(.lX > 0)
#define BUTTON_RSUP		0x00010000l	// �E�X�e�B�b�N��	(.lRy < 0)
#define BUTTON_RSDOWN	0x00020000l	// �E�X�e�B�b�N��	(.lRy > 0)
#define BUTTON_RSLEFT	0x00040000l	// �E�X�e�B�b�N��	(.lRx < 0)
#define BUTTON_RSRIGHT	0x00080000l	// �E�X�e�B�b�N�E	(.lRy > 0)
#define BUTTON_A		0x00000010l	// �`�{�^��			(.rgbButtons[0] & 0x80)
#define BUTTON_B		0x00000020l	// �a�{�^��			(.rgbButtons[1] & 0x80)
#define BUTTON_X		0x00000040l	// �w�{�^��			(.rgbButtons[2] & 0x80)
#define BUTTON_Y		0x00000080l	// �x�{�^��			(.rgbButtons[3] & 0x80)
#define BUTTON_LB		0x00000100l	// �k�a�{�^��		(.rgbButtons[4] & 0x80)
#define BUTTON_RB		0x00000200l	// �q�a�{�^��		(.rgbButtons[5] & 0x80)
#define BUTTON_SELECT	0x00000400l	// �r�d�k�d�b�s�{�^��	(.rgbButtons[6] & 0x80)
#define BUTTON_START	0x00000800l	// �r�s�`�q�s�{�^��	(.rgbButtons[7] & 0x80)
#define BUTTON_L3		0x00001000l	// �k�R�{�^��		(.rgbButtons[8] & 0x80)
#define BUTTON_R3		0x00002000l	// �q�R�{�^��		(.rgbButtons[9] & 0x80)
#define GAMEPADMAX		1			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
// �I������
void UninitInput(void);
// �X�V����
void UpdateInput(void);

//---------------------------- keyboard
bool GetKeyboardPress(int nKey);	// �L�[�����������
bool GetKeyboardTrigger(int nKey);	// �L�[���������u��
bool GetKeyboardRepeat(int nKey);	// �L�[���������܂܂̏��
bool GetKeyboardRelease(int nKey);	// �L�[�������[�X�������

//---------------------------- mouse
BOOL IsMouseLeftPressed(void);      // ���N���b�N�������
BOOL IsMouseLeftTriggered(void);    // ���N���b�N�����u��
BOOL IsMouseRightPressed(void);     // �E�N���b�N�������
BOOL IsMouseRightTriggered(void);   // �E�N���b�N�����u��
BOOL IsMouseCenterPressed(void);    // ���N���b�N�������
BOOL IsMouseCenterTriggered(void);  // ���N���b�N�����u��
long GetMouseX(void);               // �}�E�X��X�����ɓ��������Βl
long GetMouseY(void);               // �}�E�X��Y�����ɓ��������Βl
long GetMouseZ(void);               // �}�E�X�z�C�[�������������Βl

//---------------------------- game pad
BOOL IsButtonPressed(int Pad_No, DWORD Button);	   // �{�^�������������
BOOL IsButtonTriggered(int Pad_No, DWORD Button);  // �{�^�����������u��
BOOL IsButtonReleased(int Pad_No, DWORD Button);   // �{�^���������[�X�������
BOOL IsButtonRepeat(int Pad_No, DWORD Button);	   // �{�^�����������܂܂̏��
BOOL GetGamePadUsed(void);						   // �Q�[���p�b�h�̎g�p�t���O���擾����

#endif
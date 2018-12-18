//=============================================================================
//
// �L�[���̓w�b�_�[ [Input.h]
// Author�FTH_GP11_GP11B341_35_���M��
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
#define BUTTON_UP		0x00000001l	// �����L�[��(.IY<0)
#define BUTTON_DOWN		0x00000002l	// �����L�[��(.IY>0)
#define BUTTON_LEFT		0x00000004l	// �����L�[��(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// �����L�[�E(.IX>0)
#define BUTTON_A		0x00000010l	// �`�{�^��(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// �a�{�^��(.rgbButtons[1]&0x80)
#define BUTTON_X		0x00000040l	// �w�{�^��(.rgbButtons[2]&0x80)
#define BUTTON_Y		0x00000080l	// �x�{�^��(.rgbButtons[3]&0x80)
#define BUTTON_L		0x00000100l	// �k�{�^��(.rgbButtons[4]&0x80)
#define BUTTON_R		0x00000200l	// �q�{�^��(.rgbButtons[5]&0x80)
#define BUTTON_START	0x00000400l	// �r�s�`�q�s�{�^��(.rgbButtons[7]&0x80)
#define GAMEPADMAX		1			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//---------------------------- keyboard
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

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
BOOL IsButtonPressed(int Pad_No, DWORD Button);
BOOL IsButtonTriggered(int Pad_No, DWORD Button);
BOOL IsButtonReleased(int Pad_No, DWORD Button);
BOOL IsButtonRepeat(int Pad_No, DWORD Button);
BOOL GetGamePadUsed(void);


#endif
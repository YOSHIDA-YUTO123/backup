//==============================================================================================================
//
// ���͏���[input.h]
// Author:TEAM_C
//
//==============================================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//**************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//**************************************************************************************************************
#include "main.h"

//***************************
// �W���C�p�b�h�̃L�[�̗񋓌^
//***************************
typedef enum
{
	JOYKEY_UP = 0,				//�\���L�[(��)
	JOYKEY_DOWN,				//�\���L�[(��)
	JOYKEY_LEFT,				//�\���L�[(��)
	JOYKEY_RIGHT,				//�\���L�[(�E)

	JOYKEY_START,				//�X�^�[�g�L�[�������ꂽ
	JOYKEY_BACK,				//�o�b�N�L�[�������ꂽ

	JOYKEY_LS,					//L3(L�X�e�B�b�N)�������ꂽ
	JOYKEY_RS,					//R3(R�X�e�B�b�N)�������ꂽ

	JOYKEY_LEFT_B,				//LB�L�[�������ꂽ
	JOYKEY_RIGHT_B,				//RB�L�[�������ꂽ

	JOYKEY_LEFT_TRIGGER,		//LT�L�[�������ꂽ
	JOYKEY_RIGHT_TRIGGER,		//RT�L�[�������ꂽ

	JOYKEY_A,					//A�{�^���������ꂽ
	JOYKEY_B,					//B�{�^���������ꂽ
	JOYKEY_X,					//X�{�^���������ꂽ
	JOYKEY_Y,					//Y�{�^���������ꂽ

	JOYKEY_MAX					//�p�b�h�̍ő吔
}JOYKEY;

//***********************
// �}�E�X�̎�ނ̗񋓌^
//***********************
typedef enum
{
	MOUSE_LEFTBUTTON = 0,
	MOUSE_RIGHTBUTTON,
	MOUSE_WHEEL,
	MOUSE_MAX
}MOUSEBUTTON;

//********************
// �v���g�^�C�v�錾
//********************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);	// �L�[�̏�����
void UninitKeyboard(void);							// �L�[�̏I��
void UpdateKeyboard(void);							// �L�[�̍X�V
bool GetKeyboardPress(int nkey);					// �L�[�̃v���X
bool KeyboardTrigger(int nKey);						// �L�[�̃g���K�[
bool KeyboardRelease(int nKey);						// �L�[�̃����[�X
bool KeyboardRepeat(int nKey);						// �L�[�̃��s�[�g

HRESULT InitJoypad(void);							// �W���C�p�b�h�̏�����
void UninitJoypad(void);							// �W���C�p�b�h�̏I��
void UpdateJoypad(void);							// �W���C�p�b�h�̍X�V
bool JoypadPress(JOYKEY key);						// �W���C�p�b�h�̃v���X�����擾
bool JoypadTrigger(JOYKEY key);						// �W���C�p�b�h�̃g���K�[�����擾
bool JoypadRelease(JOYKEY key);						// �L�[�̃����[�X
bool JoypadRepeat(JOYKEY key);						// �L�[�̃��s�[�g

bool GetJoyStick(void);								//�W���C�p�b�h�̃X�e�B�b�N���(L)
bool GetJoyStickRrepeat(void);						//�W���C�p�b�h�̃X�e�B�b�N���(R)
void UpdateStick(void);								//R�X�e�B�b�N

XINPUT_STATE* GetJoyStickAngle(void);				//�X�e�B�b�N�̊p�x���

bool PressAnyKey(void);								// �����L�[�������Ɣ�������
bool PressAnyButton(void);							// �����{�^���������Ɣ�������

bool JoypadTriggerTrigger(JOYKEY key);				// �W���C�p�b�h�̃g���K�[�̎擾
bool JoypadTriggerPress(JOYKEY key);				// �W���C�p�b�h�̃g���K�[�̃v���X
#endif
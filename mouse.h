//==============================================================================================================
//
// �}�E�X [mouse.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _MOUSE_H_
#define _MOUSE_H_

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "main.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define LEFT_MOUSE (0)  // �}�E�X�̍��N���b�N
#define RIGHT_MOUSE (1) // �}�E�X�̉E�N���b�N
#define MOUSE_WHEEL (2) // �}�E�X�z�C�[���N���b�N

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
HRESULT InitMouse(HINSTANCE hInstance,HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool OnMouseTriggerDown(int button_type);
bool OnMouseTriggerUp(int button_type);
D3DXVECTOR2 GetMouseVelocity();
D3DXVECTOR2 GetMouseOldVelocity();
bool OnMousePress(int button_type);
BOOL IsMouseWheelPresent(void);
void SetCursorVisibility(bool visible);
LPDIRECTINPUTDEVICE8 GetMouseDevice(void);
bool GetMouseState(DIMOUSESTATE* mouseState);
int GetMouseWheel(void); //	�}�E�X�z�C�[���̎擾
#endif
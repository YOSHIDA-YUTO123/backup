//==============================================================================================================
//
// �}�E�X [mouse.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//************************************************************************************************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "mouse.h"

//**************************************************************************************************************
// �O���[�o���ϐ��錾
//**************************************************************************************************************
#define MAX_MOUSE (32)

//**************************************************************************************************************
// �O���[�o���ϐ��錾
//**************************************************************************************************************
LPDIRECTINPUT8 g_pInputInterface = NULL;
LPDIRECTINPUTDEVICE8 g_pMouseDevice = NULL;
DIMOUSESTATE g_MouseState;
DIMOUSESTATE g_MouseOldState;
DIMOUSESTATE g_PrevMouseState;
DIMOUSESTATE g_CurrentMouseState;

//==============================================================================================================
// �}�E�X�̏���������
//==============================================================================================================
HRESULT InitMouse(HINSTANCE hInstance,HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInputInterface,
		NULL)))
	{
		return E_FAIL;
	}
	
	if (FAILED(g_pInputInterface->CreateDevice(
		GUID_SysMouse,
		&g_pMouseDevice,
		NULL)))
	{
		return E_FAIL;
	}
	
	if (FAILED(g_pMouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// �������[�h�̐ݒ�
	if (FAILED(g_pMouseDevice->SetCooperativeLevel(
		hWnd,
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�̃A�N�Z�X�����l��
	g_pMouseDevice->Acquire();

	return S_OK;
}
//==============================================================================================================
// �}�E�X�̏I������
//==============================================================================================================
void UninitMouse(void)
{
	//���̓f�o�C�X(�}�E�X)�̔j��
	if (g_pMouseDevice != NULL)
	{
		g_pMouseDevice->Unacquire();//�L�[�{�[�h�ւ̃A�N�Z�X�������
		g_pMouseDevice->Release();
		g_pMouseDevice = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̍U��
	if (g_pInputInterface != NULL)
	{
		g_pInputInterface->Release();
		g_pInputInterface = NULL;
	}
}
//==============================================================================================================
// �}�E�X�̍X�V����
//==============================================================================================================
void UpdateMouse(void)
{
	g_PrevMouseState.lX = g_CurrentMouseState.lX;
	g_PrevMouseState.lY = g_CurrentMouseState.lY;

	// �X�V�O�ɍŐV�}�E�X����ۑ�����
	g_PrevMouseState = g_CurrentMouseState;

	// �ŐV�̃}�E�X�̏�Ԃ��X�V
	HRESULT	hr = g_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState);

	if (FAILED(hr))
	{
		g_pMouseDevice->Acquire();
	}

	// �}�E�X���W���擾����
	POINT p;
	GetCursorPos(&p);

	// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
	ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &p);

	g_CurrentMouseState.lX = p.x;
	g_CurrentMouseState.lY = p.y;


}
//==============================================================================================================
//�N���b�N���ꂽ
//==============================================================================================================
bool OnMouseTriggerDown(int button_type)
{
	if (!(g_PrevMouseState.rgbButtons[button_type] & (0x80)) &&
		g_CurrentMouseState.rgbButtons[button_type] & (0x80))
	{
		return true;
	}
	return false;
}
//==============================================================================================================
// �N���b�N����߂�
//==============================================================================================================
bool OnMouseTriggerUp(int button_type)
{
	if (g_PrevMouseState.rgbButtons[button_type] & (0x80) &&
		!(g_CurrentMouseState.rgbButtons[button_type] & (0x80)))
	{
		return true;
	}

	return false;
}
//==============================================================================================================
// �ړ��ʂ̎擾
//==============================================================================================================
D3DXVECTOR2 GetMouseVelocity()
{
	return D3DXVECTOR2((float)g_CurrentMouseState.lX, (float)g_CurrentMouseState.lY);
}
//==============================================================================================================
// �ߋ��̈ړ��ʂ̎擾
//==============================================================================================================
D3DXVECTOR2 GetMouseOldVelocity()
{
	return D3DXVECTOR2((float)g_PrevMouseState.lX, (float)g_PrevMouseState.lY);
}
//==============================================================================================================
// �ߋ��̈ړ��ʂ̎擾
//==============================================================================================================
bool OnMousePress(int button_type)
{
	return (g_CurrentMouseState.rgbButtons[button_type] & 0x80) ? true : false;
}
//==============================================================================================================
// �}�E�X�z�C�[���̎擾
//==============================================================================================================
BOOL IsMouseWheelPresent(void)
{
	return (GetSystemMetrics(SM_MOUSEWHEELPRESENT) != 0);
}
//=======================================================================================================================================
//�}�E�X�̏�Ԏ擾
//=======================================================================================================================================
LPDIRECTINPUTDEVICE8 GetMouseDevice(void)
{
	return g_pMouseDevice;
}
//=======================================================================================================================================
//�}�E�X�̏��
//=======================================================================================================================================
bool GetMouseState(DIMOUSESTATE* mouseState)
{
	// �}�E�X�f�o�C�X���擾
	LPDIRECTINPUTDEVICE8 pMouse = GetMouseDevice();
	if (pMouse == NULL)
	{
		return false;
	}

	// �}�E�X�̏�Ԃ��擾(��������������)
	HRESULT hr = pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)mouseState);

	if (FAILED(hr))
	{
		// ���̓f�o�C�X�����Z�b�g����Ă���ꍇ�A�Ď擾�����݂�
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			pMouse->Acquire();

			// �Ď擾�����݂�
			hr = pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)mouseState);
			if (FAILED(hr))
			{
				return false;
			}
		}
		else
		{
			//�G���[�̏ꍇ
			return false;
		}
	}

	return true; // ����Ɏ擾�ł����ꍇ
}
//==============================================================================================================
//	�}�E�X�z�C�[���̎擾
//==============================================================================================================
int GetMouseWheel(void)
{
	return g_CurrentMouseState.lZ;
}

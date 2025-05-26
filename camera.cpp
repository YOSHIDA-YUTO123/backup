//============================================================================================================
//
// �J�������� [camera.cpp]
// Author: TEAM_C
// 
//============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//**************************************************************************************************************
#include "camera.h"
#include "input.h"
#include "mouse.h"
#include "player.h"
#include"math.h"
#include "quaternion.h"

//***************************************************************************************************************
// �}�N����`
//***************************************************************************************************************
#define MAX_VIEWUP (2.90f) // �J�����̐���
#define MAX_VIEWDOWN (0.1f) // �J�����̐���

//***************************************************************************************************************
// �v���g�^�C�v�錾
//***************************************************************************************************************
void MouseView(void);       // �Q�[���̎��̃}�E�X�̎��_�ړ�
void MouseEditMode(void); // �ҏW���[�h�̎��̃}�E�X�ړ�

//***************************************************************************************************************
// �O���[�o���ϐ��錾
//***************************************************************************************************************
Camera g_camera;		// �J�������

//===========================================================================================================
// �J�����̏���������
//===========================================================================================================
void InitCamera(void)
{
	Player* pPlayer = GetPlayer();

	// �ϐ��̏�����
	g_camera.posV = D3DXVECTOR3(0.0f, 550.0f, -600.0f);			// �J�����̈ʒu

	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̌��Ă���ʒu
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// ������x�N�g��
	g_camera.rot = D3DXVECTOR3(D3DX_PI * 0.65f, 0.0f, 0.0f);	    // �p�x
	g_camera.g_CameraMode = CAMERAMODE_PLAYER;						// �������
	g_camera.bTracking = true;						// �������

	float fRotx = g_camera.posV.x - g_camera.posR.x;
	float fRoty = g_camera.posV.y - g_camera.posR.y;
	float fRotz = g_camera.posV.z - g_camera.posR.z;

	g_camera.fDistance = sqrtf((fRotx * fRotx) + (fRoty * fRoty) + (fRotz * fRotz));	// ���_���璍���_�܂ł̋���
	
	//g_camera.Direction = 1.0f; // �ړ���

	//g_camera.viewport.X = 0; // 2DX���W
	//g_camera.viewport.Y = 0; // 2DY���W
	//g_camera.viewport.Width = 1280; // ��
	//g_camera.viewport.Height = 720; // ����
	//g_camera.viewport.MinZ = 0.0f;
	//g_camera.viewport.MaxZ = 1.0f;

	//g_camera.viewport.X = 1000; // 2DX���W
	//g_camera.viewport.Y = 100; // 2DY���W
	//g_camera.viewport.Width = 200; // ��
	//g_camera.viewport.Height = 200; // ����
	//g_camera.viewport.MinZ = 0.0f;
	//g_camera.viewport.MaxZ = 1.0f;
	
}
//===========================================================================================================
// �J�����̏I������
//===========================================================================================================
void UninitCamera(void)
{
	// ����
}
//===========================================================================================================
// �J�����̍X�V����
//===========================================================================================================
void UpdateCamera(void)
{
	// �v���C���[���擾
	Player* pPlayer = GetPlayer();
	MODE mode = GetMode();

	MouseEditMode();	//�ҏW���[�h���̃J�����ړ�
	
	MouseWheel();
	// �Q�[���̎��̃J�����̍X�V
		
	//// �J�����̉E�X�e�B�b�N
	//StickCamera();

	////�}�E�X�̎��_�ړ�
	//MouseView();


	//D3DXVECTOR3 ViewPos(pPlayer->pos.x, pPlayer->pos.y + DiffPos.y, pPlayer->pos.z);

	// �J�����Ǐ]�ł���
	if (g_camera.bTracking == true)
	{
		//g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rotDestPlayer.y) * 1.0f;
		//g_camera.posRDest.y = pPlayer->pos.y + sinf(pPlayer->rotDestPlayer.y) * 1.0f;
		//g_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rotDestPlayer.y) * 1.0f;

		//g_camera.posVDest.x = pPlayer->pos.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		//g_camera.posVDest.y = pPlayer->pos.y - cosf(g_camera.rot.y) * g_camera.fDistance;
		//g_camera.posVDest.z = pPlayer->pos.z - cosf(g_camera.rot.y) * g_camera.fDistance;

		//g_camera.posR.x += ((g_camera.posRDest.x - g_camera.posR.x) * 0.3f);
		//g_camera.posR.y += ((g_camera.posRDest.y - g_camera.posR.y) * 0.3f);
		//g_camera.posR.z += ((g_camera.posRDest.z - g_camera.posR.z) * 0.3f);

		//g_camera.posV.x += ((g_camera.posVDest.x - g_camera.posV.x) * 0.3f);
		//g_camera.posV.z += ((g_camera.posVDest.z - g_camera.posV.z) * 0.3f);
	}
#if 0
	//******************
	// ���_�̐���
	//******************
	if (GetKeyboardPress(DIK_LEFT) == true || JoypadPress(JOYKEY_LEFT_B))
	{// LEFT�L�[�������ꂽ

		// �J�����̉�]
		g_camera.rot.y -= 0.03f;

		// �p�x�̐��K��
		if (g_camera.rot.y < -D3DX_PI)
		{// D3DX_PI��菬�����Ȃ�����
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
	}

	if (GetKeyboardPress(DIK_RIGHT) == true || JoypadPress(JOYKEY_RIGHT_B))
	{// RIGHT�L�[�������ꂽ
		// �J�����̉�]
		g_camera.rot.y += 0.03f;

		// �p�x�̐��K��
		if (g_camera.rot.y > D3DX_PI)
		{// D3DX_PI���傫���Ȃ�����
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}

	}

#ifdef _DEBUG

	if (GetKeyboardPress(DIK_UP))
	{// UP�L�[�������ꂽ
		// �J�����̉�]
		g_camera.rot.x += 0.03f;

		// �p�x�̐��K��
		if (g_camera.rot.x > D3DX_PI)
		{// D3DX_PI��菬�����Ȃ�����
			g_camera.rot.x -= D3DX_PI * 2.0f;
		}

	}
	if (GetKeyboardPress(DIK_DOWN))
	{// DOWN�L�[�������ꂽ
		// �J�����̉�]
		g_camera.rot.x -= 0.03f;

		// �p�x�̐��K��
		if (g_camera.rot.x < -D3DX_PI)
		{// D3DX_PI��菬�����Ȃ�����
			g_camera.rot.x += D3DX_PI * 2.0f;
		}
	}

#endif // DEBUG

	
	if (g_camera.rot.x <= D3DX_PI * 0.55f)
	{// �J�����̉���

		g_camera.rot.x = D3DX_PI * 0.55f;
	}
	else if (g_camera.rot.x >= D3DX_PI * 0.9f)
	{// �J�����̏��

		g_camera.rot.x = D3DX_PI * 0.9f;
	}
#endif
#if 0

		// �J�����̎��_�̏��
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;



		switch (g_camera.g_CameraMode)
		{// ���[�h�ؑ�
		case CAMERAMODE_NONE:
			break;

		case CAMERAMODE_PLAYER:
			// �v���C���[�ɒǏ]������
			g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rotDestPlayer.y) * 1.0f;
			g_camera.posRDest.y = pPlayer->pos.y + cosf(pPlayer->rotDestPlayer.y) * 1.0f;
			g_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rotDestPlayer.y) * 1.0f;

			g_camera.posVDest.x = pPlayer->pos.x - sinf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posVDest.y = pPlayer->pos.y - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posVDest.z = pPlayer->pos.z - cosf(g_camera.rot.y) * g_camera.fDistance;

			g_camera.posR.x += ((g_camera.posRDest.x - g_camera.posR.x) * 0.3f);
			g_camera.posR.z += ((g_camera.posRDest.z - g_camera.posR.z) * 0.3f);
			g_camera.posR.y += ((g_camera.posRDest.y - g_camera.posR.y) * 0.3f);

			g_camera.posV.x += ((g_camera.posVDest.x - g_camera.posV.x) * 0.3f);
			g_camera.posV.z += ((g_camera.posVDest.z - g_camera.posV.z) * 0.3f);
			
			break;

		default:
			break;
		}
	

	if (KeyboardTrigger(DIK_F2))
	{// ���[�h�ؑ�
		if (g_camera.g_CameraMode == CAMERAMODE_NONE)
		{// �Ǐ]���[�h
			g_camera.g_CameraMode = CAMERAMODE_PLAYER;
		}
		else if (g_camera.g_CameraMode == CAMERAMODE_PLAYER)
		{// �ʏ���
			g_camera.g_CameraMode = CAMERAMODE_NONE;
		}
	}

#endif // !_DEBUG
}
//===========================================================================================================
// �J�����̐ݒ菈��
//===========================================================================================================
void SetCamera()
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		100000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
	
}
//===========================================================================================================
// �J�����̎擾����
//===========================================================================================================
Camera* GetCamera()
{
	return &g_camera;
}
//====================================================================================================================
// �J�����̂��߂̉E�X�e�B�b�N����
//====================================================================================================================
void StickCamera(void)
{
	// �X�e�B�b�N
	XINPUT_STATE* pStick = GetJoyStickAngle();

#if 1
	if (GetJoyStick())
	{
		float RStickAngleY = pStick->Gamepad.sThumbRY;
		float RStickAngleX = pStick->Gamepad.sThumbRX;

		float DeadZone = 10920.0f;
		float fMag = sqrtf((RStickAngleX * RStickAngleX) + (RStickAngleY * RStickAngleY));

		if (fMag > DeadZone)
		{
			if (pStick->Gamepad.sThumbRX < -10920.0f)
			{// ���E�ړ�
				// �p�x
				g_camera.rot.y -= 0.03f;

			}
			else if (pStick->Gamepad.sThumbRX > 10920.0f)
			{// ���E�ړ�
				// �p�x
				g_camera.rot.y += 0.03f;
			}

			if (pStick->Gamepad.sThumbRY < -10920.0f)
			{// ���E�ړ�
				// �p�x
				g_camera.rot.x -= 0.03f;
			}
			else if (pStick->Gamepad.sThumbRY > 10920.0f)
			{// ���E�ړ�
				// �p�x
				g_camera.rot.x += 0.03f;
			}

			// �J�����̎��_�̏��
			g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
			g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;


			//if (pStick->Gamepad.sThumbRY < -10920.0f)
			//{// �㉺�ړ�
			//	// �p�x
			//	g_camera.rot.x -= 0.03f;

			//	// �J�����̎��_�̏��
			//	g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
			//	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

			//}
			//else if (pStick->Gamepad.sThumbRY > 10920.0f)
			//{
			//	// �p�x
			//	g_camera.rot.x += 0.03f;

			//	// �J�����̎��_�̏��
			//	g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
			//	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

			//}

			// �p�x�̐��K��
			if (g_camera.rot.y > D3DX_PI)
			{// D3DX_PI���傫���Ȃ�����
				g_camera.rot.y -= D3DX_PI * 2.0f;
			}

			// �p�x�̐��K��
			if (g_camera.rot.y < -D3DX_PI)
			{// D3DX_PI��菬�����Ȃ�����
				g_camera.rot.y += D3DX_PI * 2.0f;
			}

			if (g_camera.rot.x <= D3DX_PI * 0.55f)
			{// �J�����̉���

				g_camera.rot.x = D3DX_PI * 0.55f;
			}

			if (g_camera.rot.x >= D3DX_PI * 0.9f)
			{// �J�����̏��

				g_camera.rot.x = D3DX_PI * 0.9f;
			}

		}
	}
#endif
}
//===========================================================================================================
// �}�E�X�̎��_�ړ�����
//===========================================================================================================
void MouseView(void)
{
	MODE mode = GetMode();

	DIMOUSESTATE mouseState;

	if (GetMouseState(&mouseState))
	{
		static POINT prevCursorPos = { (long)SCREEN_WIDTH / (long)1.5f,(long)SCREEN_HEIGHT / (long)1.5f };

		POINT cursorPos;
		GetCursorPos(&cursorPos);

		float X = (float)cursorPos.x - prevCursorPos.x;
		float Y = (float)cursorPos.y - prevCursorPos.y;

		const float mouseSensitivity = 0.00025f;

		X *= mouseSensitivity;
		Y *= mouseSensitivity;

		g_camera.rot.y += X;
		g_camera.rot.x += Y;

		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		else if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y += -D3DX_PI * 2.0f;
		}

		if (g_camera.rot.x < -D3DX_PI)
		{
			g_camera.rot.x += D3DX_PI * 2.0f;
		}
		else if (g_camera.rot.x > D3DX_PI)
		{
			g_camera.rot.x += -D3DX_PI * 2.0f;
		}

		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= Y;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= Y;
		}

		SetCursorPos((long)SCREEN_WIDTH / (long)1.5f, (long)SCREEN_HEIGHT / (long)1.5f);

		prevCursorPos.x = (long)SCREEN_WIDTH / (long)1.5f;
		prevCursorPos.y = (long)SCREEN_HEIGHT / (long)1.5f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.y = g_camera.posV.y + cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
		
	}
}
//==============================================================================================================
// �ҏW���[�h�̎��̃}�E�X�ړ�
//==============================================================================================================
void MouseEditMode(void)
{
	if (!GetKeyboardPress(DIK_LALT)) return;

	D3DXVECTOR2 Move = GetMouseVelocity();
	D3DXVECTOR2 MoveOld = GetMouseOldVelocity();

	D3DXVECTOR2 fAngle = Move - MoveOld;

	if (OnMousePress(RIGHT_MOUSE))
	{

		//��]�ʂ��X�V
		g_camera.rot.y += fAngle.x * 0.01f;
		g_camera.rot.x += fAngle.y * 0.01f;

		//��]�ʂ𐧌�
		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}

		//�J�������W���X�V
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.y = g_camera.posV.y + cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

	}
	else if (OnMousePress(LEFT_MOUSE))
	{
		//��]�ʂ��X�V
		g_camera.rot.y += fAngle.x * 0.01f;
		g_camera.rot.x += fAngle.y * 0.01f;

		//��]�ʂ𐧌�
		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}

		// �J�����̎��_�̏��
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	//���K��
	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y += -D3DX_PI * 2.0f;
	}
	if (g_camera.rot.x < -D3DX_PI)
	{
		g_camera.rot.x += D3DX_PI * 2.0f;
	}
	else if (g_camera.rot.x > D3DX_PI)
	{
		g_camera.rot.x += -D3DX_PI * 2.0f;
	}


}
//===========================================================================================================
// �}�E�X�̎��_�ړ�����
//===========================================================================================================
void MouseWheel(void)
{
	// ���݂̃��[�h���擾
	MODE nMode = GetMode();

	// �}�E�X�z�C�[���̉�]�ʂ��擾
	int nWheel = GetMouseWheel();

	if (nWheel == 0)
	{
		return;
	}

	if (nWheel < 0)
	{
		g_camera.fDistance += 50.0f;
	}
	else if (nWheel > 0)
	{
		g_camera.fDistance -= 50.0f;
	}
	// �J�����̎��_�̏��
	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
	g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;	
}

//============================================================================================================
//
// main [main.cpp]
// Author: YOSHIDA YUTO
//
//============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//**************************************************************************************************************
#include "main.h"
#include <time.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include "camera.h"
#include "light.h"
#include "input.h"
#include "mouse.h"
#include "player.h"
#include <Shlwapi.h>
//#include"imgui.h"
#include "imguimaneger.h"
#include "imgui_impl_win32.h" 
#include "meshfield.h"
#include "billboard.h"
#include "enemy.h"
#include"sphereGage.h"
#include"ball.h"
//***************************************************************************************************************
// �}�N����`
//***************************************************************************************************************
#define DRAWMODEL_NUMINFO (10) // 10�p�[�c���̃��f���̏��̕\��

//***************************************************************************************************************
// �v���g�^�C�v�錾
//***************************************************************************************************************
void ResetDevice();					// �f�o�C�X�̃��Z�b�g
bool CreateDeviceD3D(HWND hWnd);	// �f�o�C�X�̍쐬
void ToggleFullScreen(HWND hWnd);	// �t���X�N���[���̐ݒ�

void SetDebugFont(int PosX, int PosY, D3DXCOLOR col, const char* Word, ...);	// �v���C���[�̏��

//***************************************************************************************************************
// �O���[�o���ϐ��錾
//***************************************************************************************************************
RECT g_windowRect;						// �E�B���h�E��؂�ւ��邽�߂̕ϐ�
static LPDIRECT3D9 g_pD3D = NULL;				// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
static LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3D�f�o�C�X�ւ̃|�C���^
static bool g_DeviceLost = false;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS  g_d3dpp = {};
bool g_DeviceInitialized = true;
MODE g_mode = MODE_MOTIONEDIT;				// ���݂̃��[�h
LPD3DXFONT g_pFont = NULL;				// �t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;					// FPS�J�E���^
int nGetNumber = 0;						// �ԍ�
bool g_bWireFrame = false;

LPDIRECT3DTEXTURE9 g_RenderTexture = NULL; // ��ʂ̃e�N�X�`��
LPDIRECT3DSURFACE9 g_Surface = NULL;	   // �e�N�X�`������鎞�ɂЂ悤�Ȃ��
LPD3DXEFFECT g_Effect = NULL;			   // �V�F�[�_�[�ւ̃|�C���^

//==================================================================================================================
// ���C���֐�
//==================================================================================================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// ���������[�N���m�p�̃t���O

	DWORD dwCurrentTime;			// ���ݎ���
	DWORD dwExecLastTime;			// �I������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				 // WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,						 // �E�C���h�E�̃X�^�C��
		WindowProc,						 // �E�C���h�E�v���V�[�W��
		0,								 // 0�ɂ���
		0,								 // 0�ɂ���
		hInstance,						 // �C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),  // �^�X�N�o�[�A�C�R��
		LoadCursor(NULL,IDC_ARROW),      // �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),      // �N���C�A���g�̈�̔w�i�F
		NULL,                            // ���j���[�o�[
		CLASS_NAME,                      // �E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),  // �t�@�C���A�C�R��
	};

	HWND hWnd = {};  // �E�C���h�E�n���h��(���ʎq)
	MSG msg;		 // ���b�Z�[�W���i�[����ϐ�

	// ��ʃT�C�Y�̍\��
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// �t���X�N���[�����[�h
	BOOL bWindow = FALSE;

	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈�
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,		// �g���E�C���h�E�X�^�C��
		CLASS_NAME,					// �E�C���h�E�N���X�̖��O
		WINDOW_NAME,				// �E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,		// �E�C���h�E�X�^�C��
		CW_USEDEFAULT,				// �E�C���h�E�̍����X���W
		CW_USEDEFAULT,				// �E�C���h�E�̍����Y���W
		(rect.right - rect.left),   // �E�C���h�E��
		(rect.bottom - rect.top),   // �E�C���h�E�̍���
		NULL,
		NULL,
		hInstance,					// �C���X�^���X�n���h��
		NULL);						// �E�C���h�E�쐬�f�[�^

		// �f�o�C�X�̍쐬
	if (!CreateDeviceD3D(hWnd))
	{
		Uninit();

		//�E�C���h�E�N���X�̓o�^������
		UnregisterClass(CLASS_NAME, wcex.hInstance);

		return 1;
	}
	// Main loop
	bool done = false;

	// ����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;					// ������
	dwExecLastTime = timeGetTime();		// ���ݎ�����ۑ�

	//g_Render = D3DXCreateTexture(g_pD3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, NULL, NULL, &g_Render);
	// ����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		// ���������������s�����Ƃ�
		return -1;
	}

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, SW_SHOWMAXIMIZED); // �E�C���h�E�̕\����Ԃ̐ݒ�
	UpdateWindow(hWnd);				    // �N���C�A���g�̈�̍X�V

	// Imgui�̃E�B���h�E�̏���������
	InitImgui(hWnd, g_pD3DDevice);

	// ������
	DWORD dwFrameCount;					// �t���[���J�E���g
	DWORD dwFPSLastTime;				// �Ō��FPS���v����������

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else 
		{
			// DirectX�̏���
			dwCurrentTime = timeGetTime();		// ���ݎ����̎擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				// 0.5�b�o��
				// FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// �ۑ�

				dwFrameCount = 0;				// 0�ɖ߂�
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				// 60/1�o��
				dwExecLastTime = dwCurrentTime;	// �����J�n�̎���

				dwFrameCount++;					// �t���[���J�E���g�����Z

								// Handle lost D3D9 device
				if (g_DeviceLost)
				{
					HRESULT hr = g_pD3DDevice->TestCooperativeLevel();
					if (hr == D3DERR_DEVICELOST)
					{
						::Sleep(50);
						continue;
					}
					if (hr == D3DERR_DEVICENOTRESET)
						ResetDevice();
					g_DeviceLost = false;
				}

				// Handle window resize (we don't resize directly in the WM_SIZE handler)
				if (g_ResizeWidth > 0 && g_ResizeHeight > 0)
				{
					g_d3dpp.BackBufferWidth = g_ResizeWidth;
					g_d3dpp.BackBufferHeight = g_ResizeHeight;
					g_ResizeWidth = g_ResizeHeight = 0;
					ResetDevice();
				}
				NewFlameImgui();

				// �X�V����
				Update();

				ImGui::EndFrame();

				// �`�揈��
				Draw();

			}
		}
	}

	// IMGUI�̏I������
	UninitImgui();

	// �I������
	Uninit();

	// ����\��߂�
	timeEndPeriod(1);

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//=========================================================================================================================
// IMGUI�̃n���h���[�錾
//=========================================================================================================================
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=========================================================================================================================
// �E�C���h�E�v���V�[�W��
//=========================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Player* pPlayer = GetPlayer();

	// IMGUI�̃N���b�N����
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }; // �E�C���h�E�̗̈�

	int nID;

	switch (uMsg)
	{
	case WM_SIZE:
	
		if (wParam == SIZE_MINIMIZED)
			return 0;

		// ���݂̃E�B���h�E�̑傫������
		g_ResizeWidth = (UINT)LOWORD(lParam);
		g_ResizeHeight = (UINT)HIWORD(lParam);
	
		return 0;
	case WM_ACTIVATE:	// �A�N�e�B�u���F1�@��A�N�e�B�u���F0
		return 0L;
	case WM_CREATE:
		DragAcceptFiles(hWnd, TRUE);
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:// �E�C���h�E�j�����b�Z�[�W
		// WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:				// �L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]�L�[�������ꂽ
			nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);// �E�C���h�E�j�����b�Z�[�W

			}
			else
			{
				return 0;			// �Ԃ�
			}
			break;

		case VK_F11:
			ToggleFullScreen(hWnd);	// F11�Ńt���X�N���[��
			break;
		}

		// �E�B���h�E�v���V�[�W����
	//case WM_SETCURSOR:	// �J�[�\���̐ݒ�
	//	SetCursor(NULL);
	//	return TRUE;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); // ����̏������J��Ԃ�
}
//===========================================================================================================
// ����������
//===========================================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h

	// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����̃p�����[�^�[��ݒ�
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp)); // �p�����[�^�[��0�N���A

	g_d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	g_d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	g_d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	g_d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	g_d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g�p
	g_d3dpp.Windowed = bWindow;									// �E�C���h�E���[�h
	g_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V���V�[�g
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&g_d3dpp,
		&g_pD3DDevice)))
	{
		// Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A,���_������CPU�ōs��)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&g_d3dpp,
			&g_pD3DDevice)))
		{
			// Direct3D�f�o�C�X�̐���(�`�揈��,���_������CPU�ōs��)
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&g_d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�`���[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//===================================================================================================
	// �e�평��������
	//===================================================================================================

	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	if (FAILED(InitMouse(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//// �f�o�b�O�\���p�̃t�H���g��ݒ�
	//D3DXCreateFont(g_pD3DDevice, 56, 0, FW_NORMAL, 0,
	//	FALSE,
	//	DEFAULT_CHARSET,
	//	OUT_DEFAULT_PRECIS,
	//	DEFAULT_QUALITY,
	//	DEFAULT_PITCH | FF_DONTCARE,
	//	"���C���I", &g_pFont);

	// �e�N�X�`���̓ǂݍ���
	g_pD3DDevice->CreateTexture(g_d3dpp.BackBufferWidth, g_d3dpp.BackBufferHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&g_RenderTexture,
		nullptr);

	LPD3DXBUFFER error;
	HRESULT hr;
	if (FAILED(hr = D3DXCreateEffectFromFile(g_pD3DDevice, "postprocess.hlsl", nullptr, nullptr, 0, nullptr, &g_Effect, &error)))
	{
		MessageBox(NULL, (LPCTSTR)error->GetBufferPointer(), "�G���[", MB_OK);
	}

	g_RenderTexture->GetSurfaceLevel(0, &g_Surface);

	//// �T�E���h�̏�����
	//InitSound(hWnd);

	// ���b�V���t�B�[���h�̏���������
	InitMeshField();

	// �v���C���[�̏���������
	InitPlayer();

	// �J�����̏���������
	InitCamera();	

	// ���C�g�̏���������
	InitLight();

	// �r���{�[�h�̏���������
	InitBillboard();

	// �G�̏���������
	InitEnemy();

	// �~�`�̃Q�[�W�̏���������
	InitSphere();

	// �{�[���̏���������
	InitBalll();

	SetEnemy(D3DXVECTOR3(0.0f,0.0f,500.0f));

	SetBillboard(D3DXVECTOR3(0.0f, 500.0f, 200.0f));

	// ���[�h�̐ݒ�
	SetMode(g_mode);

	return S_OK;
}
//====================================================================================================
// �I������
//====================================================================================================
void Uninit(void)
{
	//====================================================================================================
	// �e��I������
	//====================================================================================================

	// �L�[�{�[�h�̏I��
	UninitKeyboard();

	// �W���C�p�b�h�̏I��
	UninitJoypad();

	//�}�E�X�̏I��
	UninitMouse();

	// ���b�V���t�B�[���h�̏I������
	UninitMeshField();

	// �v���C���[�̏I������
	UninitPlayer();

	// �J�����̏I������
	UninitCamera();

	// ���C�g�̏I������
	UninitLight();

	// �r���{�[�h�̏I������
	UninitBillboard();

	// �G�̏I������
	UninitEnemy();

	// �~�`�̃Q�[�W�̏I������
	UninitSphere();

	// �{�[���̏I������
	UninitBalll();

	if (g_Effect != NULL)
	{
		g_Effect->Release();
		g_Effect = NULL;
	}

	if (g_Surface != NULL)
	{
		g_Surface->Release();
		g_Surface = NULL;
	}

	if (g_RenderTexture != NULL)
	{
		g_RenderTexture->Release();
		g_RenderTexture = NULL;
	}

	// �f�o�b�O�\���p�̃t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}
//=====================================================================================================
// �X�V����
//=====================================================================================================
void Update(void)
{
	//============================================================================================================
	// �e��I�u�W�F�N�g�X�V����
	//============================================================================================================

	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �W���C�p�b�h�̍X�V
	UpdateJoypad();

	//�}�E�X�̍X�V
	UpdateMouse();

	// �f�o�b�O�pIMGUI
	DebugImgui();

	static bool bPause = false;

	// �J�����̍X�V����
	UpdateCamera();

	// ���C�g�̍X�V����
	UpdateLight();

	// �~�`�̃Q�[�W�̍X�V����
	UpdateSphere();

	ImGui::Checkbox(u8"�|�[�Y", &bPause);

	if (bPause == false)
	{
		// �G�̍X�V����
		UpdateEnemy();

		// ���b�V���t�B�[���h�̍X�V����
		UpdateMeshField();

		// �v���C���[�̍X�V����
		UpdatePlayer();

		// �r���{�[�h�̍X�V����
		UpdateBillboard();

		// �{�[���̍X�V����
		UpdateBalll();
	}
	switch (g_mode)
	{
	case MODE_MOTIONEDIT: // �^�C�g�����
		break;
	}

}
//=====================================================================================================
// �`�揈��
//=====================================================================================================
void Draw(void)
{
	// �J�������擾
	Camera* pCamera = GetCamera();

	LPDIRECT3DSURFACE9 oldSurface;

	g_pD3DDevice->GetRenderTarget(0, &oldSurface);

	g_pD3DDevice->SetRenderTarget(0, g_Surface);

	//��ʃN���A(�o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		SetCamera(); // �J�������Z�b�g

	// ���b�V���t�B�[���h�̕`�揈��
		DrawMeshField();

		// �G�̕`�揈��
		DrawEnemy();

		// �v���C���[�̕`�揈��
		DrawPlayer();

		// �{�[���̕`�揈��
		DrawBalll();

		// ���C�g�̕`�揈��
		DrawLight();

		// �r���{�[�h�̕`�揈��
		DrawBillboard();

		//// �~�`�̃Q�[�W�̕`�揈��
		//DrawSphere();

		//// Fps�̕\��
		//DrawFPS();

		// IMGUI�̃E�B���h�E�̃����_���[�̐ݒ�
		EndImguiRender();

		// �`��I��
		g_pD3DDevice->EndScene();

	}

	g_pD3DDevice->SetRenderTarget(0, oldSurface);

	VERTEX_2D quad[4] =
	{
		{ D3DXVECTOR3(0.0f,   0.0f,   0.0f), 1.0f, 0xFFFFFFFF, D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(g_d3dpp.BackBufferWidth, 0.0f,   0.0f), 1.0f, 0xFFFFFFFF, D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f,   g_d3dpp.BackBufferHeight, 0.0f), 1.0f, 0xFFFFFFFF, D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3(g_d3dpp.BackBufferWidth, g_d3dpp.BackBufferHeight, 0.0f), 1.0f, 0xFFFFFFFF, D3DXVECTOR2(1.0f, 1.0f) },
	};

	if (g_Effect != NULL)
	{
		g_Effect->SetTexture("tex0", g_RenderTexture);

		g_Effect->Begin(0, 0);

		g_Effect->BeginPass(0);

		g_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &quad, sizeof(VERTEX_2D));

		g_Effect->EndPass();

		g_Effect->End();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	if (oldSurface != NULL)
	{
		oldSurface->Release();
		oldSurface = NULL;
	}
}
//========================================================================================================
// FPS�̕\��
//========================================================================================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	//������ɑ��
	wsprintf(&aString[0], "FPS:%d\n", g_nCountFPS);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
//==============================================================================================================
// �G�̃f�o�b�O�\��
//==============================================================================================================
void DrawEnemySet(int g_nNumEnemy)
{
	// ���[�J���ϐ�
	RECT rect = { 0,180,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	// ������ɑ��
	wsprintf(&aString[0], "�G�̑���:%d\n", g_nNumEnemy);

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
//================================================================================================================
// �v���C���[�̍��W�\��
//================================================================================================================
void DrawDebugPlayerPos(void)
{
#if 0
	// ���[�J���ϐ�
	RECT rect = { 0,80,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,100,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect2 = { 0,120,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect3 = { 0,140,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aString[256];
	char aString1[256];
	char aString2[256];
	char aString3[256];

	// �擾
	PLAYER* pPlayer = GetPlayer();

	// ������ɑ��
	sprintf(&aString[0], "�v���C���[��pos.x:%.2f\n", pPlayer->pos.x);
	sprintf(&aString1[0], "�v���C���[��pos.y:%.2f\n", pPlayer->pos.y);
	sprintf(&aString2[0], "�v���C���[��pos.z:%.2f\n", pPlayer->pos.z);
	wsprintf(&aString3[0], "���C���[�t���[�� ON[F3]:OFF[F4]\n");

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString2[0], -1, &rect2, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString3[0], -1, &rect3, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
#endif
}
//==========================================================================================================
// �J����
//==========================================================================================================
void DrawCameraPos(void)
{
	// ���[�J���ϐ�
	RECT rect = { 0,40,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,60,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aString[256];
	char aString1[256];

	// �擾
	Camera* pCamera = GetCamera();

	// ������ɑ��
	sprintf(&aString[0], "�J������rot.y:%.2f\n", pCamera->rot.y);
	sprintf(&aString1[0], "�J������rot.x:%.2f\n", pCamera->rot.x);

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

}
//==========================================================================================================
// �J�����̎��
//==========================================================================================================
void DrawCameraType(void)
{
	RECT rect = { 0,20,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];
	char aString1[256];

	// �擾
	Camera* pCamera = GetCamera();

	if (pCamera->g_CameraMode == CAMERAMODE_NONE)
	{
		strcpy(&aString1[0], "NORMAL");
	}
	else if (pCamera->g_CameraMode == CAMERAMODE_PLAYER)
	{
		strcpy(&aString1[0], "PLAYER");
	}

	//������ɑ��
	wsprintf(&aString[0], "���݂̃��[�h[F2] : %s\n", &aString1[0]);

	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

}
//========================================================================================================
// ���[�h�̐ݒ�
//========================================================================================================
void SetMode(MODE mode)
{
	// ���݂̉�ʂ̏I������
	switch (g_mode)
	{
	case MODE_MOTIONEDIT: // �^�C�g�����
		break;
	}

	g_mode = mode;		// ��ʐ؂�ւ�

	// �V������ʂ̏���������
	switch (mode)
	{
	case MODE_MOTIONEDIT: // �^�C�g�����
		break;
	}
}
//========================================================================================================
// �f�o�C�X�̎擾
//========================================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//======================================================================================================
// ���[�h�̎擾
//======================================================================================================
MODE GetMode(void)
{
	return g_mode; // �ϐ���Ԃ�
}
//=======================================================================================================
// ���C���[�t���[��ON
//=======================================================================================================
void onWireFrame()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}
//=======================================================================================================
// ���C���[�t���[��OFF
//=======================================================================================================
void offWireFrame()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
//============================================================================================================
// �}�E�X��3D���W�ɕϊ�����
//============================================================================================================
void GetRayFromMouse(int mouseX, int mouseY, D3DXVECTOR3* outOrigin, D3DXVECTOR3* outDir)
{
	// �v���W�F�N�V�����}�g���b�N�X�ƃr���[�}�g���b�N�X
	D3DXMATRIX matView, matproj;

	// �v���W�F�N�V�����}�g���b�N�X�ƃr���[�}�g���b�N�X���擾
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matproj);

	RECT rect;
	GetClientRect(FindWindowA(CLASS_NAME, nullptr), &rect);

	int screenWidth = rect.right - rect.left;
	int screenHeight = rect.bottom - rect.top;

	// NDC (-1�`1)�ɕϊ�
	float pX = ((2.0f * mouseX) / screenWidth) - 1.0f;
	float pY = ((2.0f * mouseY) / screenHeight) - 1.0f;
	pY *= -1.0f;

	// �r���[��Ԃ�Near/Far
	D3DXVECTOR3 vNear = D3DXVECTOR3(pX, pY, 0.0f);
	D3DXVECTOR3 vFar = D3DXVECTOR3(pX, pY, 1.0f);

	// �����s��̋t�s��
	D3DXMATRIX matInvVP;
	D3DXMATRIX matVP = matView * matproj;
	D3DXMatrixInverse(&matInvVP, NULL, &matVP);

	// ���[���h���W�ɕϊ�
	D3DXVec3TransformCoord(&vNear, &vNear, &matInvVP);
	D3DXVec3TransformCoord(&vFar, &vFar, &matInvVP);

	// ���C�̌��_
	*outOrigin = vNear;
	D3DXVECTOR3 Ray = vFar - vNear;
	D3DXVec3Normalize(outDir, &Ray);
}
//============================================================================================================
// �v���C���[�̏��
//============================================================================================================
void SetDebugFont(int PosX, int PosY, D3DXCOLOR col, const char* Word, ...)
{
	RECT rect = { PosX,PosY,SCREEN_WIDTH,SCREEN_HEIGHT };

	// ������ۊǗp�ϐ�
	char aString[256] = {};

	// �ψ���������
	va_list args;

	// �ψ����̏������n�߂�
	va_start(args, Word);

	// �t�H�[�}�b�g�ς݂̕������astring�ɕۊ�
	vsnprintf(aString, sizeof(aString), Word, args);

	// �ψ����̏������I��
	va_end(args);

	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, col);
}
//====================================================================
// �t���X�N���[���̐ݒ�
//====================================================================
void ToggleFullScreen(HWND hWnd)
{
	static bool isFullscreen = true;

	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (isFullscreen)
	{
		// �E�B���h�E���[�h�ɐ؂�ւ�
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
			g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// �t���X�N���[�����[�h�ɐ؂�ւ�
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	isFullscreen = !isFullscreen;
}
//====================================================================
// �f�o�C�X�̃��Z�b�g
//====================================================================
void ResetDevice()
{
	if (g_Effect)
		g_Effect->OnLostDevice();

	if (g_Surface) {
		g_Surface->Release();
		g_Surface = NULL;
	}
	if (g_RenderTexture) {
		g_RenderTexture->Release();
		g_RenderTexture = NULL;
	}

	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT hr = g_pD3DDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();

	// �e�N�X�`���̓ǂݍ���
	g_pD3DDevice->CreateTexture(g_d3dpp.BackBufferWidth, g_d3dpp.BackBufferHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&g_RenderTexture,
		nullptr);
	LPD3DXBUFFER error;
	hr;
	//if (FAILED(hr = D3DXCreateEffectFromFile(g_pD3DDevice, "postprocess.hlsl", nullptr, nullptr, 0, nullptr, &g_Effect, &error)))
	//{
	//	MessageBox(NULL, (LPCTSTR)error->GetBufferPointer(), "�G���[", MB_OK);
	//}

	g_RenderTexture->GetSurfaceLevel(0, &g_Surface);

	g_Effect->OnResetDevice();


	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�`���[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//// �f�o�b�O�\���p�̃t�H���g��ݒ�
	//D3DXCreateFont(g_pD3DDevice, 86, 0, FW_BOLD, 0,
	//	FALSE,
	//	DEFAULT_CHARSET,
	//	OUT_DEFAULT_PRECIS,
	//	DEFAULT_QUALITY,
	//	DEFAULT_PITCH | FF_DONTCARE,
	//	"Jump Streets", &g_pFont);

	// ���C�g�̏����ݒ�
	InitLight();

	// ���C�g��ݒu
	SetLight(D3DXVECTOR3(0.0f, 1200.0f, 0.0f), D3DLIGHT_DIRECTIONAL, D3DXVECTOR3(0.0f, -1.0f, 0.0f));

	// ���C�g��ݒu
	SetLight(D3DXVECTOR3(4200.0f, 500.0f, 0.0f), D3DLIGHT_DIRECTIONAL, D3DXVECTOR3(1.0f, -1.0f, 0.0f));

	// ���C�g��ݒu
	SetLight(D3DXVECTOR3(-4200.0f, 500.0f, 0.0f), D3DLIGHT_DIRECTIONAL, D3DXVECTOR3(-1.0f, -1.0f, 0.0f));

}
//====================================================================
// �f�o�C�X�̍쐬
//====================================================================
bool CreateDeviceD3D(HWND hWnd)
{
	D3DDISPLAYMODE d3ddm = {}; //�f�B�X�v���C���[�h

	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
		return false;

	// Create the D3DDevice
	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));//�p�����[�^�̃[���N���A

	g_d3dpp.BackBufferWidth = g_ResizeWidth;//�Q�[����ʃT�C�Y(��)
	g_d3dpp.BackBufferHeight = g_ResizeHeight;//�Q�[����ʃT�C�Y(����)
	g_d3dpp.BackBufferFormat = d3ddm.Format;//�o�b�N�o�b�t�@�̌`��
	g_d3dpp.BackBufferCount = 1;//�o�b�N�o�b�t�@�̐�
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//�o�b�N�o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	g_d3dpp.Windowed = TRUE;//�E�C���h�E���[�h
	g_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//���t���b�V�����[�g
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//�C���^�[�o��

	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pD3DDevice) < 0)
		return false;

	return true;
}
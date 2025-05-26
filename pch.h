
#ifndef _PCH_H_  // ���̃}�N����`������ĂȂ�������
#define _PCH_H_  // 2�d�C���N���[�h�h�~�̃}�N����`

//***************************
// �C���N���[�h�t�@�C���錾
//***************************
#include <windows.h>
#define DIRECTINPUT_VERSION (0x0800)// �r���h���x���Ώ��p�̃}�N��
#include "d3dx9.h"					// �`�揈���ɕK�v
#include "dinput.h"					// ���͏����ɕK�v
#include "Xinput.h"					// �W���C�p�b�h�����ɕK�v
#include "xaudio2.h"
//**************************
// ���C�u�����̃����N�錾
//**************************
#pragma comment(lib,"d3d9.lib")		// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	// DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"xinput.lib")	// �W���C�p�b�h�����ɕK�v
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib, "Shlwapi.lib")

//**************************
// �}�N����`
//**************************
#define CLASS_NAME "WindowClass"      // �E�C���h�E�N���X�̖��O
#define WINDOW_NAME "Motion Viewer"		  // �E�C���h�E�̖��O(�L���v�V�����ɕ\��)
#define SCREEN_WIDTH (1280)           // �E�C���h�E�̕�
#define SCREEN_HEIGHT (720)           // �E�C���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) // ���_�t�H�[�}�b�g[2D]
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL |  D3DFVF_DIFFUSE | D3DFVF_TEX1) // ���W,�@��,�J���[,�e�N�X�`��
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//**********************
// ���_���[2D]�̍\����
//**********************
typedef struct
{
	D3DXVECTOR3 pos;			// ���_���W
	float rhw;					// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;				// ���_�J���[
	D3DXVECTOR2 tex;			// �e�N�X�`�����W
}VERTEX_2D;

//**********************
// ���_���[3D]�̍\����
//**********************
typedef struct
{
	D3DXVECTOR3 pos;			// ���_���W
	D3DXVECTOR3 nor;			// �@���x�N�g��
	D3DCOLOR col;				// ���_�J���[
	D3DXVECTOR2 tex;			// �e�N�X�`�����W
}VERTEX_3D;

//**********************
// ���̍\����
//**********************
typedef struct
{
	D3DXVECTOR3 pos;
	DWORD color;
}LINEVERTEX;

#endif
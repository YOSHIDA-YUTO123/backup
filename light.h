//==========================
//
// ���C�g���� [light.h]
// Author: TEAM_C
//
//==========================
#ifndef _LIGHT_H_
#define _LIGHT_H_

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "main.h"

//****************************
// �}�N����`
//****************************
#define MAX_LIGHT (50)			// ���C�g�̍ő吔
#define MAX_TEXTURE (128)		// �e�N�X�`���̍ő吔

//****************************
// ���C�g�̎��
//****************************
typedef enum
{
	LIGHTTYPE_NORMAL = 0,
	LIGHTTYPE_FIVER,
	LIGHTTYPE_MAX
}LIGHTTYPE;


//****************************
// ���C�g�I���W�i���̃��f���̍\����
//****************************
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEXTURE];	//�v���C���[�̃e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh;							//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;						//�}�e���A���ւ̃|�C���^
	DWORD dwNumMat;								//�}�e���A���̐�
}LightModelOrigin;

//****************************
// ���C�g�̃��f���̍\����
//****************************
typedef struct
{
	D3DLIGHT9 alight;
	bool bUse;
	D3DXVECTOR3 rot;							// ����
	D3DXMATRIX mtxWorld;						// ���[���h�}�g���N�X
	LightModelOrigin lightModel;				// ���C�g�̃��f��
}LightModel;

//**********************
// �v���g�^�C�v�錾
//**********************
void InitLight(void);		// ���C�g�̏�����
void UninitLight(void);		// ���C�g�̏I��
void UpdateLight(void);		// ���C�g�̍X�V
void DrawLight(void);		// ���C�g�̕`�揈��
void SetLight(D3DXVECTOR3 pos, D3DLIGHTTYPE type, D3DXVECTOR3 dir);		// ���C�g�̐ݒ菈��
#endif
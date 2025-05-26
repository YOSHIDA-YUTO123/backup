//==============================================================================================================
//
// ���f��[model.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _MODEL_H_
#define _MODEL_H_

//***************************
//�C���N���[�h�t�@�C��
//***************************
#include"main.h"

//***************************
//�}�N����`
//***************************
#define MAX_TEXTURE (128)	// �e�N�X�`���̍ő吔
#define MAX_NAME (256)		// ���O�̒���

//***************************
//���f���̍\����
//***************************
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEXTURE];	//�v���C���[�̃e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh;							//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;						//�}�e���A���ւ̃|�C���^
	DWORD dwNumMat;								//�}�e���A���̐�
	int nIdxModelParent;						//�e���f���̃C���f�b�N�X
	D3DXVECTOR3 pos;							//�ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;							//����
	D3DXVECTOR3 offpos;							//�ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 offrot;							//����
	D3DXMATRIX mtxWorld;						//���[���h�}�g���b�N�X
	D3DXVECTOR3 vtxMin, vtxMax;
	D3DXVECTOR3 Size;
	bool bUpperBody;							// �㔼�g�������g��
	char aName[MAX_NAME];						// ���O�̒���
}MODEL;

//***************************
//�v���g�^�C�v�錾
//***************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
#endif

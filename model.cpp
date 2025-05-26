//==================================================
//
// ���f��[model.cpp](�e���v���[�g�p)
// Author:YOSHIDA YUUTO
//
//==================================================

//**************************************************
//�C���N���[�h�t�@�C��
//**************************************************
#include "model.h"

//**************************************************
//�O���[�o���ϐ��錾
//**************************************************
LPDIRECT3DTEXTURE9 g_apTextureModel[128] = {};
LPD3DXMESH g_pMeshModel = NULL;//���b�V��(���_���W)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatModel = NULL;//�}�e���A���ւ̃|�C���^
DWORD g_dwNumMatModel = 0;//�}�e���A���̐�
D3DXVECTOR3 g_posModel;//�ʒu
D3DXVECTOR3 g_rotModel;//����
D3DXVECTOR3 Scal;
D3DXMATRIX g_mtxWorldModel;//���[���h�}�g���b�N�X

//==================================================
//���f���̏���������
//==================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	g_posModel = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);
	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Scal = D3DXVECTOR3(3.0f, 3.0f, 3.0f);
	
	////X�t�@�C���̓ǂݍ���
	//D3DXLoadMeshFromX("data\\MODEL\\3dtest_zako.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatModel,
	//	NULL,
	//	&g_dwNumMatModel,
	//	&g_pMeshModel);

	//D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

	////�}�e���A���̃f�[�^�ւ̃|�C���^���擾
	//pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	//{
	//	if (pMat[nCntMat].pTextureFilename != NULL)
	//	{
	//		//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
	//			//�e�N�X�`���̓ǂݍ���
	//		D3DXCreateTextureFromFile(pDevice,
	//			pMat[nCntMat].pTextureFilename,
	//			&g_apTextureModel[nCntMat]);
	//	}
	//}
}
//================================================
//���f���̏I������
//================================================
void UninitModel(void)
{
	//���b�V���̔j��
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}
//=================================================
//���f���̍X�V����
//=================================================
void UpdateModel(void)
{
	//���͂Ȃ�
}
//=================================================
//���f���̕`�揈��
//=================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans,mtxScal;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldModel);


	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotModel.y, g_rotModel.x, g_rotModel.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posModel.x, g_posModel.y, g_posModel.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureModel[nCntMat]);

		//���f��(�p�[�c)�̕`��
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);
}

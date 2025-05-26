//============================================================================================================
//
// ���C�g���� [light.cpp]
// Author:TEAM_C
//
//============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//**************************************************************************************************************
#include "light.h"
#include "player.h"

//***************************************************************************************************************
// �O���[�o���ϐ��錾
//***************************************************************************************************************
LightModel g_Light[MAX_LIGHT];	// ���C�g�̃��f���̍\����
LightModelOrigin g_LightOrigin;	// ���C�g�̃��f���̃I���W�i��

//===========================================================================================================
//�@�J�����̏���������
//===========================================================================================================
void InitLight(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���C�g�̎�ނ�ݒ�
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// ���C�g�̃N���A����(�[���������[�֐�)
		ZeroMemory(&g_Light[nCnt].alight, sizeof(g_Light[nCnt].alight));

		// ���C�g�̎g�p���
		g_Light[nCnt].bUse = false;
	}

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\OBJ\\light.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_LightOrigin.pBuffMat,
		NULL,
		&g_LightOrigin.dwNumMat,
		&g_LightOrigin.pMesh);

	D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

	//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_LightOrigin.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_LightOrigin.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
				//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_LightOrigin.pTexture[nCntMat]);
		}
	}
}
//========================================================
//�@���C�g�̏I������
//========================================================
void UninitLight(void)
{
	// �}�e���A������
	for (int nCnt = 0; nCnt < (int)g_LightOrigin.dwNumMat; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (g_LightOrigin.pTexture[nCnt] != NULL)
		{
			g_LightOrigin.pTexture[nCnt]->Release();
			g_LightOrigin.pTexture[nCnt] = NULL;
		}
	}

	// �o�b�t�@�̔j��
	if (g_LightOrigin.pBuffMat != NULL)
	{
		g_LightOrigin.pBuffMat->Release();
		g_LightOrigin.pBuffMat = NULL;
	}

	// ���b�V���̔j��
	if (g_LightOrigin.pMesh != NULL)
	{
		g_LightOrigin.pMesh->Release();
		g_LightOrigin.pMesh = NULL;
	}

	// ���C�g�̍ő吔����
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// �}�e���A������
		for (int nCnt = 0; nCnt < (int)g_Light[nCntLight].lightModel.dwNumMat; nCnt++)
		{
			// �e�N�X�`���̔j��
			if (g_Light[nCntLight].lightModel.pTexture[nCnt] != NULL)
			{
				g_Light[nCntLight].lightModel.pTexture[nCnt] = NULL;
			}
		}

		// �o�b�t�@�̔j��
		if (g_Light[nCntLight].lightModel.pBuffMat != NULL)
		{
			g_Light[nCntLight].lightModel.pBuffMat = NULL;
		}

		// ���b�V���̔j��
		if (g_Light[nCntLight].lightModel.pMesh != NULL)
		{
			g_Light[nCntLight].lightModel.pMesh = NULL;
		}
	}
}
//========================================================
//�@���C�g�̍X�V����
//========================================================
void UpdateLight(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X���擾
	pDevice = GetDevice();

	Player* pPlayer = GetPlayer();

	// ���C�g�̎�ނ�ݒ�
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		//g_light[nCnt].Type = D3DLIGHT_POINT;	// ���s����

		//D3DXVECTOR3 Pos(pPlayer->SwordMtx._41, pPlayer->SwordMtx._42, pPlayer->SwordMtx._43);

		//// ���C�g�̊g�U��
		//g_light[nCnt].Direction = pPlayer->pos - pPlayer->posOld;
		//g_light[nCnt].Position = D3DXVECTOR3(Pos);

		//g_light[nCnt].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//g_light[nCnt].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//g_light[nCnt].Range = 500.0f;

		//// �X�|�b�g���C�g�̊p�x�ݒ�i�����̊p�x�j
		//g_light[nCnt].Theta = D3DXToRadian(15.0f);  // 15�x

		//// �X�|�b�g���C�g�̊O���̊p�x�ݒ�
		//g_light[nCnt].Phi = D3DXToRadian(15.0f);    // 30�x


		//g_light[nCnt].Attenuation0 = 1.0f;
		////g_light[nCnt].Attenuation1 = 0.1f;
		//g_light[nCnt].Attenuation2 = 0.0f;

		//g_light[nCnt].Falloff = 1.0f;  // �W���I�ȍL����

		//// ���C�g�̐ݒ�
		//pDevice->SetLight(nCnt, &g_light[nCnt]);

		//// ���C�g��L����
		//pDevice->LightEnable(nCnt, TRUE);
	}

}
//========================================================
// ���C�g�̕`�揈��
//========================================================
void DrawLight(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	// ���C�g�̎�ނ�ݒ�
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		if (g_Light[nCnt].bUse == false) continue;

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Light[nCnt].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Light[nCnt].rot.y, g_Light[nCnt].rot.x, g_Light[nCnt].rot.z);
		D3DXMatrixMultiply(&g_Light[nCnt].mtxWorld, &g_Light[nCnt].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Light[nCnt].alight.Position.x, g_Light[nCnt].alight.Position.y, g_Light[nCnt].alight.Position.z);
		D3DXMatrixMultiply(&g_Light[nCnt].mtxWorld, &g_Light[nCnt].mtxWorld, &mtxTrans);


		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Light[nCnt].mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_Light[nCnt].lightModel.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Light[nCnt].lightModel.dwNumMat; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_Light[nCnt].lightModel.pTexture[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_Light[nCnt].lightModel.pMesh->DrawSubset(nCntMat);
		}

		D3DXVECTOR3 start = g_Light[nCnt].alight.Position;
		D3DXVECTOR3 dir = g_Light[nCnt].alight.Direction;

		// ���₷����������i���Ƃ���100.0f�{�j
		D3DXVECTOR3 end = start + dir * 100000.0f;

	}
	//�ۑ����Ă����}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);
}
//========================================================
// ���C�g�̐ݒ菈��
//========================================================
void SetLight(D3DXVECTOR3 pos, D3DLIGHTTYPE type,D3DXVECTOR3 dir)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���C�g�̍ő吔����
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		if (g_Light[nCnt].bUse == false)
		{
			g_Light[nCnt].lightModel = g_LightOrigin;
			g_Light[nCnt].alight.Position = pos;
			g_Light[nCnt].alight.Type = type;
			g_Light[nCnt].bUse = true;

			// ���C�g�̊g�U��
			g_Light[nCnt].alight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���K������
			D3DXVec3Normalize(&dir, &dir); // �x�N�g���̑傫����1�ɂ���
			g_Light[nCnt].alight.Direction = dir;

			// ���C�g�̐ݒ�
			pDevice->SetLight(nCnt, &g_Light[nCnt].alight);

			// ���C�g��L����
			pDevice->LightEnable(nCnt, TRUE);

			break;
		}
	}

}


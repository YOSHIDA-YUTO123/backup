//==================================================
//
// �{�[�� [ball.cpp]
// Author:YOSHIDA YUUTO
//
//==================================================

//**************************************************
//�C���N���[�h�t�@�C��
//**************************************************
#include "ball.h"
#include "math.h"
#include "input.h"
#include"imgui.h"
#include "meshfield.h"
#include "line.h"
#include "player.h"
#include"quaternion.h"

//**************************************************
// �v���g�^�C�v�ϐ��錾
//**************************************************
void UpdateBoundball(const float resutitution);

//**************************************************
// �O���[�o���ϐ��錾
//**************************************************
Ball g_aBoll;

//==================================================
// ����������
//==================================================
void InitBalll(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_aBoll.pos = D3DXVECTOR3(-500.0f,0.0f,0.0f);
	g_aBoll.rot = INIT_VECTOR3;
	g_aBoll.move = INIT_VECTOR3;
	g_aBoll.bJump = true;

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\OBJ\\ball2.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aBoll.pBuffMat,
		NULL,
		&g_aBoll.dwNumMat,
		&g_aBoll.pMesh);

	D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

	//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_aBoll.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aBoll.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
				//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_aBoll.pTexture[nCntMat]);
		}
	}

	//���_���W�̎擾
	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	nNumVtx = g_aBoll.pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y�擾
	sizeFVF = D3DXGetFVFVertexSize(g_aBoll.pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	g_aBoll.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
		if (vtx.x < g_aBoll.vtxMin.x)
		{
			g_aBoll.vtxMin.x = vtx.x;
		}
		else if (vtx.y < g_aBoll.vtxMin.y)
		{
			g_aBoll.vtxMin.y = vtx.y;
		}
		else if (vtx.z < g_aBoll.vtxMin.z)
		{
			g_aBoll.vtxMin.z = vtx.z;
		}

		else if (vtx.x > g_aBoll.vtxMax.x)
		{
			g_aBoll.vtxMax.x = vtx.x;
		}

		else if (vtx.y > g_aBoll.vtxMax.y)
		{
			g_aBoll.vtxMax.y = vtx.y;
		}

		else if (vtx.z > g_aBoll.vtxMax.z)
		{
			g_aBoll.vtxMax.z = vtx.z;
		}

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	//�e���f�����Ƃ̃T�C�Y����
	g_aBoll.Size.x = g_aBoll.vtxMax.x - g_aBoll.vtxMin.x;
	g_aBoll.Size.y = g_aBoll.vtxMax.y - g_aBoll.vtxMin.y;
	g_aBoll.Size.z = g_aBoll.vtxMax.z - g_aBoll.vtxMin.z;

	//���_�o�b�t�@�̃A�����b�N
	g_aBoll.pMesh->UnlockVertexBuffer();
}
//==================================================
// �I������
//==================================================
void UninitBalll(void)
{
	// �}�e���A������
	for (int nCnt = 0; nCnt < (int)g_aBoll.dwNumMat; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (g_aBoll.pTexture[nCnt] != NULL)
		{
			g_aBoll.pTexture[nCnt]->Release();
			g_aBoll.pTexture[nCnt] = NULL;
		}
	}

	// �o�b�t�@�̔j��
	if (g_aBoll.pBuffMat != NULL)
	{
		g_aBoll.pBuffMat->Release();
		g_aBoll.pBuffMat = NULL;
	}

	// ���b�V���̔j��
	if (g_aBoll.pMesh != NULL)
	{
		g_aBoll.pMesh->Release();
		g_aBoll.pMesh = NULL;
	}
}
//==================================================
// �X�V����
//==================================================
void UpdateBalll(void)
{
	g_aBoll.posOld = g_aBoll.pos;

	g_aBoll.pos += g_aBoll.move;

	if (CollisionField(&g_aBoll.pos))
	{
		g_aBoll.bJump = true;

		UpdateBoundball(0.7f);
	}
	else
	{
		g_aBoll.bJump = false;
	}
	Player* pPlayer = GetPlayer();

	if (KeyboardTrigger(DIK_H))
	{
		g_aBoll.move = ComputeThrowVelocity(pPlayer->pos, g_aBoll.pos, 1.0f, 100.0f);
	}


	if (GetKeyboardPress(DIK_LSHIFT))
	{
		CollisionRayToSphere(g_aBoll.pos, &g_aBoll.move, &g_aBoll.pos, 50.0f);
	}


	g_aBoll.move.y -= 1.0f;

	if (ImGui::Button(u8"�{�[���W�����v"))
	{
		g_aBoll.move.y = 15.0f;
	}
}
//==================================================
// �`�揈��
//==================================================
void DrawBalll(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aBoll.mtxWorld);

	// ��]����p�x
	static float fAngle;

	// �ړ����������߂�
	D3DXVECTOR3 MoveDir = g_aBoll.pos - g_aBoll.posOld;

	// �P�ʃx�N�g���ɂ���
	D3DXVec3Normalize(&MoveDir, &MoveDir);

	// �ړ��ʂ����߂�
	D3DXVECTOR3 velocity = g_aBoll.pos - g_aBoll.posOld;

	// y�����̈ړ��ʂ͖���
	velocity.y = 0.0f;

	// �ړ��������������Ƃ߂�
	float distance = D3DXVec3Length(&velocity);

	// ��]�ʂ�ݒ�
	fAngle += distance / (g_aBoll.Size.y * 0.5f);

	// ��]��,������x�N�g��
	D3DXVECTOR3 axis, normal(0.0f, -1.0f, 0.0f);

	// ��]�������߂�
	D3DXVec3Cross(&axis, &normal, &MoveDir);
	D3DXVec3Normalize(&axis, &axis);

	// �N�H�[�^�j�I��
	D3DXQUATERNION q;

	// �N�H�[�^�j�I���𐶐�
	CreateQuaternion(&q, axis,fAngle);

	// ��]�s���ݒ�
	SetMatrixRotationQuaternion(&mtxRot, q);

	// ��]�s����|�����킹��
	D3DXMatrixMultiply(&g_aBoll.mtxWorld, &g_aBoll.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aBoll.pos.x, g_aBoll.pos.y + g_aBoll.Size.y * 0.5f, g_aBoll.pos.z);
	D3DXMatrixMultiply(&g_aBoll.mtxWorld, &g_aBoll.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aBoll.mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_aBoll.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aBoll.dwNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aBoll.pTexture[nCntMat]);

		//���f��(�p�[�c)�̕`��
		g_aBoll.pMesh->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);
}
//==================================================
// �{�[���̎擾����
//==================================================
Ball* GetBall(void)
{
	return &g_aBoll;
}
//==================================================
// �{�[���̃o�E���h
//==================================================
void UpdateBoundball(const float resutitution)
{
	D3DXVECTOR3 FieldNormal = GetCollisionNormal();

	float dot = D3DXVec3Dot(&g_aBoll.move, &FieldNormal);

	D3DXVECTOR3 NewMove = g_aBoll.move - (dot * 2.0f) * FieldNormal;
	
	g_aBoll.move = NewMove * resutitution;
}

//===============================================================================================================
//
// �G [enemy.cpp]
// Author:YOSHIDA YUUTO
//
//===============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "input.h"
#include "camera.h"
#include <stdio.h>
#include <string.h>
#include "motion.h"
#include "mouse.h"
#include "meshfield.h"
#include "math.h"
#include <iostream>
#include "imgui.h"
#include "imguimaneger.h"
#include "imgui_internal.h"
#include "line.h"
#include "quaternion.h"
#include"enemy.h"
#include "player.h"

using namespace ImGui;

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define MAX_WORD (256)			// �ő啶����
#define PLAYERLIFE (1000)		// �v���C���[�̗̑�
#define MAX_TEXPLAYER (128)		// �e�N�X�`���̍ő吔
#define MAX_JUMP (15.0f)		// �W�����v��
#define MAX_MOVE (1.0f)			// �v���C���[�̈ړ���
#define NUM_MTX (8)				// ���̓����蔻��̃}�g���N�X�̐�
#define LANDINGEXPLOSION (6)	// ���n�����Ƃ��ɏo�鉌
#define MAX_JUMP (15.0f)        // �W�����v��

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void LoadFullBodyEnemy(const char* Filepath);										 // �v���C���[�̃��[�h����
int LoadFilenameEnemy(FILE* pFile, int nNumModel, char* aString);						 // �v���C���[�̃��f���̃��[�h����
void LoadCharacterSetEnemy(FILE* pFile, char* aString, int nNumparts);					 // �v���C���[�̃p�[�c�̐ݒ菈��
void LoadMotionSetEnemy(FILE* pFile, char* aString, int nNumModel);						 // �v���C���[�̃��[�V�����̃��[�h����
void LoadKeySetEnemy(FILE* pFile, char* aString, int nCntMotion);						 // �v���C���[�̃��[�V�����̃L�[�̓ǂݍ��ݏ���

void ChasePlayer(Player* pPlayer);
void TransEnemyState(void);
bool FinishActionMotion(void);
void EndActionTransState(void);


//**************************************************************************************************************
//�O���[�o���ϐ��錾
//**************************************************************************************************************
Enemy g_Enemy;//�v���C���[�\����

int nCntMotionEnemy, nKeyEnemy;

//===============================================================================================================
//�v���C���[�̏���������
//===============================================================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	MODE mode = GetMode();//���݂̃��[�h���擾

	//�v���C���[�̏�����
	g_Enemy.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);		   // ���W
	g_Enemy.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   // �p�x
	g_Enemy.Size = D3DXVECTOR3(20.0f, 200.0f, 20.0f);	   // �v���C���[�̑傫��
	g_Enemy.rotDestEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �ړI�̊p�x
	g_Enemy.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   // �ړ���
	g_Enemy.bJump = true;								   // �W�����v�����ۂ�
	g_Enemy.nLife = PLAYERLIFE;						   // �̗�
	g_Enemy.bUse = false;
	//g_Enemy.state = PLAYERSTATE_NORMAL;				   // ���
	nCntMotionEnemy = 0;
	nKeyEnemy = 0;

	LoadFullBodyEnemy("data/motion.txt");
}
//===============================================================================================================
//�v���C���[�̏I������
//===============================================================================================================
void UninitEnemy(void)
{
	// �e�N�X�`���̔j��
	for (int nCntModel = 0; nCntModel < g_Enemy.Motion.nNumModel; nCntModel++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_Enemy.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (g_Enemy.Motion.aModel[nCntModel].pTexture[nCntMat] != NULL)
			{
				g_Enemy.Motion.aModel[nCntModel].pTexture[nCntMat]->Release();
				g_Enemy.Motion.aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_Enemy.Motion.aModel[nCntModel].pMesh != NULL)
		{
			g_Enemy.Motion.aModel[nCntModel].pMesh->Release();
			g_Enemy.Motion.aModel[nCntModel].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_Enemy.Motion.aModel[nCntModel].pBuffMat != NULL)
		{
			g_Enemy.Motion.aModel[nCntModel].pBuffMat->Release();
			g_Enemy.Motion.aModel[nCntModel].pBuffMat = NULL;
		}
	}
}
//===============================================================================================================
//�v���C���[�̍X�V����
//===============================================================================================================
void UpdateEnemy(void)
{
	Player* pPlayer = GetPlayer();

	g_Enemy.move.x += (0.0f - g_Enemy.move.x) * 0.5f;
	g_Enemy.move.z += (0.0f - g_Enemy.move.z) * 0.51f;

	g_Enemy.posOld = g_Enemy.pos;

	g_Enemy.pos += g_Enemy.move;

	// ���b�V���t�B�[���h�̓����蔻��
	if (CollisionField(&g_Enemy.pos))
	{
		g_Enemy.bJump = true;
	}
	else
	{
		g_Enemy.bJump = false;
	}

	// �J�v�Z���̓����蔻��
	if (CollisionCapsule(pPlayer->pos, SetMtxConversion(pPlayer->Motion.aModel[2].mtxWorld),
		g_Enemy.pos, SetMtxConversion(g_Enemy.Motion.aModel[2].mtxWorld), 50.0f, 50.0f))
	{
		ImGui::Text("true");
	}

	// �v���C���[��ǂ�������
	ChasePlayer(pPlayer);

	// �G�̏�ԑJ�ڏ���
	TransEnemyState();
	
	// ���[�V�������I�������
	if (FinishActionMotion())
	{
		EndActionTransState();
	}
	// �d��
	g_Enemy.move.y -= 1.0f;

	//�v���C���[�̊p�x�̐��K��
	if (g_Enemy.rotDestEnemy.y - g_Enemy.rot.y >= D3DX_PI)
	{
		g_Enemy.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_Enemy.rotDestEnemy.y - g_Enemy.rot.y <= -D3DX_PI)
	{
		g_Enemy.rot.y -= D3DX_PI * 2.0f;
	}

	// ���[�V�����̍X�V����
	UpdateMotion(&g_Enemy.Motion, true);

	//�v���C���[�̌�����ړI�̌����ɋ߂Â���
	g_Enemy.rot.y += (g_Enemy.rotDestEnemy.y - g_Enemy.rot.y) * 0.1f;
}
//===============================================================================================================
// �v���C���[�̕`�揈��
//===============================================================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	Player* pPlayer = GetPlayer();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxSize, mtxShadow;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	if (g_Enemy.bUse == true)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Enemy.mtxWorldEnemy);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy.rot.y, g_Enemy.rot.x, g_Enemy.rot.z);
		D3DXMatrixMultiply(&g_Enemy.mtxWorldEnemy, &g_Enemy.mtxWorldEnemy, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Enemy.pos.x, g_Enemy.pos.y, g_Enemy.pos.z);
		D3DXMatrixMultiply(&g_Enemy.mtxWorldEnemy, &g_Enemy.mtxWorldEnemy, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Enemy.mtxWorldEnemy);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

		//�S���f���p�[�c�̕`��
		for (int nCntModel = 0; nCntModel < g_Enemy.Motion.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransform;//�v�Z�p
			D3DXMATRIX mtxParent;//�e�̃}�g���b�N�X

			//�p�[�c�̃}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Enemy.Motion.aModel[nCntModel].mtxWorld);

			//�p�[�c�̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Enemy.Motion.aModel[nCntModel].rot.y, g_Enemy.Motion.aModel[nCntModel].rot.x, g_Enemy.Motion.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Enemy.Motion.aModel[nCntModel].mtxWorld, &g_Enemy.Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

			//�p�[�c�̈ʒu(�I�t�Z�b�g)�𔽉f
			D3DXMatrixTranslation(&mtxTransform, g_Enemy.Motion.aModel[nCntModel].pos.x, g_Enemy.Motion.aModel[nCntModel].pos.y, g_Enemy.Motion.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Enemy.Motion.aModel[nCntModel].mtxWorld, &g_Enemy.Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

			//�p�[�c��[�e�̃}�g���b�N�X]��ݒ�
			if (g_Enemy.Motion.aModel[nCntModel].nIdxModelParent != -1)
			{
				//�e���f��������ꍇ
				mtxParent = g_Enemy.Motion.aModel[g_Enemy.Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{//�e���f�����Ȃ��ꍇ
				mtxParent = g_Enemy.mtxWorldEnemy;
			}

			//�Z�o����[�p�[�c�̃��[���h�}�g���b�N�X]��[�e�̃}�g���b�N�X]���������킹��
			D3DXMatrixMultiply(&g_Enemy.Motion.aModel[nCntModel].mtxWorld,
				&g_Enemy.Motion.aModel[nCntModel].mtxWorld,
				&mtxParent);//���������e

			//�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD,
				&g_Enemy.Motion.aModel[nCntModel].mtxWorld);

			for (int nCntMat = 0; nCntMat < (int)g_Enemy.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_Enemy.Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

				// ���f���̏�񂪂Ȃ�������
				if (g_Enemy.Motion.aModel[nCntModel].pBuffMat == NULL)
				{
					continue;
				}

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_Enemy.Motion.aModel[nCntModel].pTexture[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_Enemy.Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}
		}
	}
	//�}�e���A���̐ݒ�
	pDevice->SetMaterial(&matDef);
}
//============================================
// �G�̐ݒ菈��
//============================================
void SetEnemy(D3DXVECTOR3 pos)
{
	if (g_Enemy.bUse == false)
	{
		g_Enemy.pos = pos;
		g_Enemy.bUse = true;
	}
}
//============================================
// �v���C���[�̃��[�h����
//============================================
void LoadFullBodyEnemy(const char* Filepath)
{
	FILE* pFile;
	char aStr[MAX_WORD] = {};
	char Skip[3] = {};

	int nNumModel = 0;
	int nLoadCnt = 0;
	int nNumParts = 0;

	// �t�@�C�����J��
	pFile = fopen(Filepath, "r");

	if (pFile != NULL)
	{
		while (1)
		{
			// �������ǂݍ���
			int nData = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], "#") == 0)
			{//�R�����g��������
				//������ǂݔ�΂�
				continue;
			}

			// NUM_MODEL��ǂݎ������
			if (strcmp(&aStr[0], "NUM_MODEL") == 0)
			{
				nData = fscanf(pFile, "%s", &Skip[0]);						// [=]��ǂݔ�΂�
				nData = fscanf(pFile, "%d", &nNumModel);					// ���f���̍ő吔����
				g_Enemy.Motion.nNumModel = nNumModel;	// ���f���̍ő吔����
			}

			// ���f���̓ǂݍ��݂��܂��I����Ă��Ȃ�������
			if (nLoadCnt < nNumModel)
			{
				// ���f���̓ǂݍ��񂾐���Ԃ�
				nLoadCnt = LoadFilenameEnemy(pFile, nNumModel, &aStr[0]);
			}

			// CHARACTERSET��ǂݎ������
			if (strcmp(&aStr[0], "CHARACTERSET") == 0)
			{
				// �p�[�c�̐ݒ菈��
				LoadCharacterSetEnemy(pFile, &aStr[0], nNumParts);
			}

			// MOTIONSET��ǂݎ������
			if (strcmp(&aStr[0], "MOTIONSET") == 0)
			{
				// ���[�V�����̐ݒ菈��
				LoadMotionSetEnemy(pFile, &aStr[0], nNumModel);
			}

			// EOF��ǂݎ������
			if (nData == EOF)
			{
				// ���[�V�����̑�������
				g_Enemy.Motion.nNumMotion = nCntMotionEnemy;
				nCntMotionEnemy = 0; // ���[�V�����̃J�E���g�����Z�b�g����
				break;          // While���𔲂���
			}
		}
	}
	else
	{
		// �t�@�C�����J���Ȃ�������
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(LoadEnemy)", MB_OK);
		return;
	}

	// �t�@�C�������
	fclose(pFile);
}
//=================================================
// �v���C���[�̃��f�����[�h
//=================================================
int LoadFilenameEnemy(FILE* pFile, int nNumModel, char* aString)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

	//���_���W�̎擾
	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^

	char Skip[3] = {}; // [=]�ǂݔ�΂��悤�ϐ�

	int nCntLoadModel = 0; // ���f���̃��[�h�̃J�E���^�[

	// �J�E���g�����f������艺��������
	while (nCntLoadModel < nNumModel)
	{
		// ������ǂݎ��
		int nData = fscanf(pFile, "%s", aString);

		// MODEL_FILENAME��ǂݎ������
		if (strcmp(aString, "MODEL_FILENAME") == 0)
		{
			nData = fscanf(pFile, "%s", &Skip[0]); // [=]��ǂݔ�΂�
			nData = fscanf(pFile, "%s", aString);  // �t�@�C������ǂݎ��

			const char* FILE_NAME = {};    // �t�@�C��������p�ϐ�

			FILE_NAME = aString;           // �t�@�C��������

			//X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(FILE_NAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_Enemy.Motion.aModel[nCntLoadModel].pBuffMat,
				NULL,
				&g_Enemy.Motion.aModel[nCntLoadModel].dwNumMat,
				&g_Enemy.Motion.aModel[nCntLoadModel].pMesh);

			if (g_Enemy.Motion.aModel[nCntLoadModel].pBuffMat == NULL)
			{
				// ���b�Z�[�W�{�b�N�X�̕\��
				MessageBox(NULL, FILE_NAME, "���f�����ǂݍ��߂܂���", MB_OK | MB_ICONWARNING);

				nCntLoadModel++; // ���f���̃J�E���g�𑝂₷
				continue;
			}

			//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_Enemy.Motion.aModel[nCntLoadModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Enemy.Motion.aModel[nCntLoadModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
						//�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_Enemy.Motion.aModel[nCntLoadModel].pTexture[nCntMat]);

					// �e�N�X�`�����ǂݍ��߂Ȃ�������
					if (g_Enemy.Motion.aModel[nCntLoadModel].pTexture[nCntMat] == NULL)
					{
						// �t�@�C���l�[����ۑ�
						const char* name = pMat[nCntMat].pTextureFilename;

						// ���b�Z�[�W�{�b�N�X�̕\��
						MessageBox(NULL, name, "�e�N�X�`�����ǂݍ��߂܂���", MB_OK | MB_ICONWARNING);
					}
				}
			}

			//���_���̎擾
			nNumVtx = g_Enemy.Motion.aModel[nCntLoadModel].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y�擾
			sizeFVF = D3DXGetFVFVertexSize(g_Enemy.Motion.aModel[nCntLoadModel].pMesh->GetFVF());

			//���_�o�b�t�@�̃��b�N
			g_Enemy.Motion.aModel[nCntLoadModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
				if (vtx.x < g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.x)
				{
					g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.y)
				{
					g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.z)
				{
					g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.x)
				{
					g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.y)
				{
					g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.z)
				{
					g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.z = vtx.z;
				}

				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				pVtxBuff += sizeFVF;
			}

			////�T�C�Y����
			//g_Enemy.Size.x = g_Enemy.vtxMaxEnemy.x - g_Enemy.vtxMinEnemy.x;
			//g_Enemy.Size.y = g_Enemy.vtxMaxEnemy.y - g_Enemy.vtxMinEnemy.y;
			//g_Enemy.Size.z = g_Enemy.vtxMaxEnemy.z - g_Enemy.vtxMinEnemy.z;

			//�e���f�����Ƃ̃T�C�Y����
			g_Enemy.Motion.aModel[nCntLoadModel].Size.x = g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.x - g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.x;
			g_Enemy.Motion.aModel[nCntLoadModel].Size.y = g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.y - g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.y;
			g_Enemy.Motion.aModel[nCntLoadModel].Size.z = g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.z - g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.z;

			//���_�o�b�t�@�̃A�����b�N
			g_Enemy.Motion.aModel[nCntLoadModel].pMesh->UnlockVertexBuffer();

			nCntLoadModel++; // ���f���̃J�E���g�𑝂₷
		}
	}
	return nCntLoadModel; // ���f���̃J�E���g��Ԃ�

}
//=============================================
// �v���C���[�̃L�����N�^�[�Z�b�g
//=============================================
void LoadCharacterSetEnemy(FILE* pFile, char* aString, int nNumparts)
{
	int nIdx = 0;		// �C���f�b�N�X�i�[�ϐ�
	char Skip[3] = {};	// [=]�ǂݔ�΂��ϐ�
	int bUpper = 0;		// �㔼�g�������g���̔��� 

	// END_CHARACTERSET��ǂݎ���ĂȂ�������
	while (strcmp(aString, "END_CHARACTERSET") != 0)
	{
		// ������ǂݎ��
		int nData = fscanf(pFile, "%s", aString);

		if (strcmp(aString, "MOVE") == 0)
		{
			nData = fscanf(pFile, "%s", &Skip[0]); // [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%f", &g_Enemy.speed);    // �X�s�[�h����
		}

		//if (strcmp(aString, "JUMP") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]�ǂݔ�΂�
		//	nData = fscanf(pFile, "%f", &g_Enemy.JumpHeight);    // �W�����v�̍�������
		//}

		//if (strcmp(aString, "SIZE") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]�ǂݔ�΂�
		//	nData = fscanf(pFile, "%f", &g_Enemy.Size.x);    // �傫������
		//	nData = fscanf(pFile, "%f", &g_Enemy.Size.y);    // �傫������
		//	nData = fscanf(pFile, "%f", &g_Enemy.Size.z);    // �傫������
		//}

		//if (strcmp(aString, "BLOWOFF") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]�ǂݔ�΂�
		//	nData = fscanf(pFile, "%f", &g_Enemy.BlowMove.x);    // �傫������
		//	nData = fscanf(pFile, "%f", &g_Enemy.BlowMove.y);    // �傫������
		//}

		//if (strcmp(aString, "AVOIDMOVE") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]�ǂݔ�΂�
		//	nData = fscanf(pFile, "%f", &g_Enemy.AvoidMove.x);    // �傫������
		//	nData = fscanf(pFile, "%f", &g_Enemy.AvoidMove.y);    // �傫������
		//}

		else if (strcmp(aString, "PART") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �e�̃C���f�b�N�X��ۑ�
			nData = fscanf(pFile, "%d", &bUpper);

			g_Enemy.Motion.aModel[nIdx].bUpperBody = bUpper == 1 ? true : false;
		}

		// PARENT��ǂݎ������
		else if (strcmp(aString, "PARENT") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �e�̃C���f�b�N�X��ۑ�
			nData = fscanf(pFile, "%d", &g_Enemy.Motion.aModel[nIdx].nIdxModelParent);
		}
		// POS��ǂݎ������
		else if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aModel[nIdx].offpos.x);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aModel[nIdx].offpos.y);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aModel[nIdx].offpos.z);
		}
		// ROT��ǂݎ������
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aModel[nIdx].offrot.x);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aModel[nIdx].offrot.y);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aModel[nIdx].offrot.z);
			nIdx++;
		}
	}
}
//=================================================
// �v���C���[�̃��[�V�����Z�b�g
//=================================================
void LoadMotionSetEnemy(FILE* pFile, char* aString, int nNumModel)
{
	char Skip[3] = {}; // [=]�ǂݔ�΂��ϐ�
	int Loop = 0;	   // ���[�v���邩

	while (1)
	{
		// ������ǂݎ��
		int nData = fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0 || strcmp(aString, "<<") == 0)
		{// �R�����g��������
			// �R�����g��ǂݔ�΂�
			continue;
		}

		// LOOP��ǂݒʂ�����
		if (strcmp(aString, "LOOP") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// ���[�v���邩���Ȃ���
			nData = fscanf(pFile, "%d", &Loop);

			// ���[�v���邩�ǂ���
			g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].bLoop = (Loop == 1) ? true : false;
		}
		// NUM_KEY��ǂݒʂ�����
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �L�[�̍ő吔����
			nData = fscanf(pFile, "%d", &g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].nNumkey);
		}
		else if (strcmp(aString, "NUM_FRAME") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �L�[�̍ő吔����
			nData = fscanf(pFile, "%d", &g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].nNumFrame);
		}
		else if (strcmp(aString, "START_FRAME") == 0)
		{
			int nCntFrame = 0;

			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �Ō�܂œ���I�������
			while (nCntFrame < g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].nNumFrame)
			{
				// �L�[�̍ő吔����
				nData = fscanf(pFile, "%d", &g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].nStartFrame[nCntFrame]);

				nCntFrame++;
			}
		}
		else if (strcmp(aString, "END_FRAME") == 0)
		{
			int nCntFrame = 0;

			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �Ō�܂œ���I�������
			while (nCntFrame < g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].nNumFrame)
			{
				// �L�[�̍ő吔����
				nData = fscanf(pFile, "%d", &g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].nEndFrame[nCntFrame]);

				nCntFrame++;
			}
		}

		// KEYSET��ǂݒʂ�����
		if (strcmp(aString, "KEYSET") == 0)
		{
			// �L�[�̐ݒ菈��
			LoadKeySetEnemy(pFile, aString, nCntMotionEnemy);
		}
		// END_MOTIONSET��ǂݒʂ�����
		if (strcmp(aString, "END_MOTIONSET") == 0)
		{
			nCntMotionEnemy++; // ���[�V�����̃J�E���g�����Z�b�g
			nKeyEnemy = 0;     // �L�[�����Z�b�g
			break;
		}
	}
}
//============================================
// �v���C���[�̃��[�V�����̃L�[�Z�b�g
//============================================
void LoadKeySetEnemy(FILE* pFile, char* aString, int nCntMotion)
{
	char Skip[3] = {}; // [=]�ǂݔ�΂��ϐ�
	int nCntPos = 0;   // �ʒu�̃J�E���g
	int nCntRot = 0;   // �p�x�̃J�E���g


	while (1)
	{
		// ������ǂݎ��
		int nData = fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0)
		{// �R�����g��������
			// �R�����g��ǂݔ�΂�
			continue;
		}

		// FRAME��ǂݒʂ�����
		if (strcmp(aString, "FRAME") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �t���[����ǂݍ���
			nData = fscanf(pFile, "%d", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].nFrame);
		}

		// POS��ǂݒʂ�����
		if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �ʒu��ǂݍ���

			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntPos].fPosX);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntPos].fPosY);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROT��ǂݒʂ�����
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �p�x��ǂݍ���
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntRot].fRotX);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntRot].fRotY);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntRot].fRotZ);
			nCntRot++;
		}
		// END_KEYSET��ǂݒʂ�����
		else if (strcmp(aString, "END_KEYSET") == 0)
		{
			nKeyEnemy++;		 // �L�[�𑝂₷
			nCntPos = 0; // �ʒu�̃J�E���g�����Z�b�g
			nCntRot = 0; // �p�x�̃J�E���g�����Z�b�g
			break;
		}
	}
}
//============================================
// �v���C���[��Ǐ]���鏈��
//============================================
void ChasePlayer(Player* pPlayer)
{
	float fDistance = GetDistance(pPlayer->pos, g_Enemy.pos);

	ImGui::Text(u8"���� = %.1f", fDistance);

	// �������߂�������
	if (fDistance <= 100.0f)
	{
		g_Enemy.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		 
		if(g_Enemy.Motion.MotionData[0].motiontypeBlend != MOTIONTYPE_ACTION) g_Enemy.nActionType = ACTIONSTATE_ATTACK;

		return;
	}

	g_Enemy.nActionType = ACTIONSTATE_MOVE;

	// �ʒu�̍���
	D3DXVECTOR3 DiffPos = pPlayer->pos - g_Enemy.pos;

	// ���݂̈ړ�����
	float fRotMove = atan2f(g_Enemy.move.x,g_Enemy.move.z);

	// �G�̌�����ۑ�
	float fAngle = fRotMove;

	// �p�x
	float fDestRot = atan2f(DiffPos.x, DiffPos.z);

	// ���������߂�
	float fDIffRot = fDestRot - fRotMove;

	// �p�x�̕␳
	if (fDIffRot < -D3DX_PI)
	{
		fDIffRot += D3DX_PI * 2.0f;
	}

	// �p�x�̕␳
	if (fDIffRot > D3DX_PI)
	{
		fDIffRot += -D3DX_PI * 2.0f;
	}

	// �ړ����������񂾂�߂Â���
	fRotMove += fDIffRot * 0.1f;

	// �ړI�̊p�x������
	g_Enemy.rotDestEnemy.y = fAngle + D3DX_PI;

	// �ړ��ʂ��X�V
	g_Enemy.move.x = sinf(fRotMove) * 5.0f;
	g_Enemy.move.z = cosf(fRotMove) * 5.0f;

}
//============================================
// �G�̏�Ԃ̑J��
//============================================
void TransEnemyState(void)
{
	// �G�̏�Ԃ̑J��
	switch (g_Enemy.nActionType)
	{
	case ACTONSTATE_NEUTRAL:
		// �ړ���Ԃ�������
		if (g_Enemy.Motion.MotionData[0].motiontypeBlend == MOTIONTYPE_MOVE) SetMotion(&g_Enemy.Motion, MOTIONTYPE_NEUTRAL, true, 10, 0);
		break;
	case ACTIONSTATE_MOVE:
		// �ړ����[�V�������J�n
		SetMotion(&g_Enemy.Motion, MOTIONTYPE_MOVE, true, 10, 0);
		break;
	case ACTIONSTATE_ATTACK:
		SetMotion(&g_Enemy.Motion, MOTIONTYPE_ACTION, true, 10, 0);
		break;
	case ACTIONSTATE_BACKSTEP:
		SetMotion(&g_Enemy.Motion, MOTIONTYPE_JUMP, true, 10, 0);
		break;
	default:
		break;
	}

}
//============================================
// �G�̍U����Ԃ̎擾
//============================================
bool FinishActionMotion(void)
{
	Motion_Data* pMotionData = &g_Enemy.Motion.MotionData[0];

	// �U����Ԃ���Ȃ�������
	if (pMotionData->motiontypeBlend != MOTIONTYPE_ACTION && pMotionData->motionType != MOTIONTYPE_ACTION)
	{
		return false;
	}

	int nLastKey = g_Enemy.Motion.aMotionInfo[MOTIONTYPE_ACTION].nNumkey - 1;

	if (pMotionData->nKey >= nLastKey)
	{
		return true;
	}

	return false;
}
//============================================
// �A�N�V�������[�V�����I����̏�ԑJ��
//============================================
void EndActionTransState(void)
{
	int nRandom_Value = rand() % 3;

	switch (nRandom_Value)
	{
	case 0:
		g_Enemy.nActionType = ACTIONSTATE_ATTACK;
		break;
	case 1:
		g_Enemy.nActionType = ACTONSTATE_NEUTRAL;
		break;
	case 2:
		g_Enemy.nActionType = ACTIONSTATE_BACKSTEP;
		break;
	default:
		break;
	}
}

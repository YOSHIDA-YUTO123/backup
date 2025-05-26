//===============================================================================================================
//
// �v���C���[[player.cpp]
// Author:YOSHIDA YUUTO
//
//===============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "player.h"
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

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define MAX_WORD (256)			// �ő啶����
#define PLAYERLIFE (100)		// �v���C���[�̗̑�
#define MAX_TEXPLAYER (128)		// �e�N�X�`���̍ő吔
#define MAX_JUMP (15.0f)		// �W�����v��
#define MAX_MOVE (1.0f)			// �v���C���[�̈ړ���
#define NUM_MTX (8)				// ���̓����蔻��̃}�g���N�X�̐�
#define LANDINGEXPLOSION (6)	// ���n�����Ƃ��ɏo�鉌
#define MAX_JUMP (15.0f)        // �W�����v��

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void UpdatePlayerMove(void);

void LoadFullBodyPlayer(const char* Filepath);										 // �v���C���[�̃��[�h����
int LoadFilename(FILE* pFile, int nNumModel, char* aString);						 // �v���C���[�̃��f���̃��[�h����
void LoadCharacterSet(FILE* pFile, char* aString, int nNumparts);					 // �v���C���[�̃p�[�c�̐ݒ菈��
void LoadMotionSet(FILE* pFile, char* aString, int nNumModel);						 // �v���C���[�̃��[�V�����̃��[�h����
void LoadKeySet(FILE* pFile, char* aString, int nCntMotion);						 // �v���C���[�̃��[�V�����̃L�[�̓ǂݍ��ݏ���
void SetFullBodyModel(LPDIRECT3DDEVICE9 pDevice);									 // �e��������Ȃ���
void StartNeutralMotion(MOTION* pMotion);											 // �j���[�g�������[�V�����̊J�n
void StartMoveMotion(MOTION* pMotion);												 // 

//**************************************************************************************************************
//�O���[�o���ϐ��錾
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureImgui = NULL;//�v���C���[�̃e�N�X�`���ւ̃|�C���^
Player g_player;//�v���C���[�\����
ImVec2 blockPos = ImVec2(300, 300);

int g_nCounterState,g_AttackState; // ��ԃJ�E���^�[
int nCntMotion,nKey;

//===============================================================================================================
//�v���C���[�̏���������
//===============================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	MODE mode = GetMode();//���݂̃��[�h���擾

	//�v���C���[�̏�����
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);		   // ���W
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   // �p�x
	g_player.Size = D3DXVECTOR3(20.0f, 200.0f, 20.0f);	   // �v���C���[�̑傫��
	g_player.rotDestPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �ړI�̊p�x
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   // �ړ���
	g_player.bJump = true;								   // �W�����v�����ۂ�
	g_player.bDisp = true;								   // �g�p���
	g_player.nLife = PLAYERLIFE;						   // �̗�
	g_player.nMaxLife = PLAYERLIFE;
	g_player.state = PLAYERSTATE_NORMAL;				   // ���
	g_nCounterState = 0;                                   // ��ԃJ�E���^�[
	g_AttackState = 0;									   // �U����Ԃ̃J�E���^�[
	nCntMotion = 0;
	nKey = 0;

	LoadFullBodyPlayer("data/motion.txt");
}
//===============================================================================================================
//�v���C���[�̏I������
//===============================================================================================================
void UninitPlayer(void)
{
	// �e�N�X�`���̔j��
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel; nCntModel++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_player.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (g_player.Motion.aModel[nCntModel].pTexture[nCntMat] != NULL)
			{
				g_player.Motion.aModel[nCntModel].pTexture[nCntMat]->Release();
				g_player.Motion.aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_player.Motion.aModel[nCntModel].pMesh != NULL)
		{
			g_player.Motion.aModel[nCntModel].pMesh->Release();
			g_player.Motion.aModel[nCntModel].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_player.Motion.aModel[nCntModel].pBuffMat != NULL)
		{
			g_player.Motion.aModel[nCntModel].pBuffMat->Release();
			g_player.Motion.aModel[nCntModel].pBuffMat = NULL;
		}
	}

	if (g_apTextureImgui != NULL)
	{
		g_apTextureImgui->Release();
		g_apTextureImgui = NULL;
	}
}
//===============================================================================================================
//�v���C���[�̍X�V����
//===============================================================================================================
void UpdatePlayer(void)
{
	// �v���C���[�̈ړ�����
	UpdatePlayerMove();

	// ���b�V���t�B�[���h�̓����蔻��
	if (CollisionField(&g_player.pos))
	{
		g_player.bJump = true;
	}
	else
	{
		g_player.bJump = false;
	}

	if (KeyboardTrigger(DIK_SPACE))
	{
		if (g_player.bJump == true)
		{
			g_player.move.y = 15.0f;
			g_player.bJump = false;
		}
	}

	ImGui::DragInt(u8"Hp", &g_player.nLife, 1.0f, 0, g_player.nMaxLife);
	// �d��
	g_player.move.y -= MAX_GLABITY;
	

	//�v���C���[�̊p�x�̐��K��
	if (g_player.rotDestPlayer.y - g_player.rot.y >= D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDestPlayer.y - g_player.rot.y <= -D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	// ���[�V�����̍X�V����
	UpdateMotion(&g_player.Motion, g_player.bFullBody);

	if (GetKeyboardPress(DIK_LSHIFT))
	{
		CollisionRayToSphere(g_player.pos,&g_player.move, &g_player.pos, 50.0f);
	}

	//�v���C���[�̌�����ړI�̌����ɋ߂Â���
	g_player.rot.y += (g_player.rotDestPlayer.y - g_player.rot.y) * 0.1f;
}
//===============================================================================================================
// �v���C���[�̕`�揈��
//===============================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxSize, mtxShadow;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	int Parent = 0;

	// �e�N�X�`���̔j��
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel; nCntModel++)
	{
		if (g_player.Motion.aModel[nCntModel].nIdxModelParent == -1)
		{
			Parent++;
		}
	}

	g_player.bFullBody = (Parent == 2) ? false : true;

	// �S�g���S�g����Ȃ���
	const bool isFullBody = g_player.bFullBody == true;

	if (g_player.bDisp == true)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_player.mtxWorldPlayer);

		Draw3DLine(g_player.pos,SetMtxConversion(g_player.Motion.aModel[2].mtxWorld), COLOR_RED);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
		D3DXMatrixMultiply(&g_player.mtxWorldPlayer, &g_player.mtxWorldPlayer, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
		D3DXMatrixMultiply(&g_player.mtxWorldPlayer, &g_player.mtxWorldPlayer, &mtxTrans);

		//D3DXMatrixShadow(&mtxShadow, &Light, &plane);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorldPlayer);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �e��������Ȃ���
		SetFullBodyModel(pDevice);
	}
	//�}�e���A���̐ݒ�
	pDevice->SetMaterial(&matDef);
}
//===============================================================================================================
//�v���C���[�̎擾����
//===============================================================================================================
Player* GetPlayer(void)
{
	return &g_player;
}
//===============================================================================================================
// �v���C���[�̈ړ�����
//===============================================================================================================
void UpdatePlayerMove(void)
{
	Camera* pCamera = GetCamera();

	// �v���C���[�̈ړ����x
	float fSpeed = 5.0f;

	if (GetKeyboardPress(DIK_A))
	{
		//�v���C���[�̈ړ�(��)
		if (GetKeyboardPress(DIK_W) == true)
		{
			// �ړ����[�V�����̊J�n
			StartMoveMotion(&g_player.Motion);

			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.25f) * fSpeed;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.25f) * fSpeed;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.75f;
		}
		//�v���C���[�̈ړ�(��)
		else if (GetKeyboardPress(DIK_S))
		{
			// �ړ����[�V�����̊J�n
			StartMoveMotion(&g_player.Motion);

			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.75f) * fSpeed;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.75f) * fSpeed;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.25f;
		}
		//�v���C���[�̈ړ�(��)
		else
		{
			// �ړ����[�V�����̊J�n
			StartMoveMotion(&g_player.Motion);

			g_player.move.z += sinf(pCamera->rot.y) * fSpeed;
			g_player.move.x -= cosf(pCamera->rot.y) * fSpeed;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.5f;
		}
	}
	//�v���C���[�̈ړ�(�E)
	else if (GetKeyboardPress(DIK_D))
	{
		// �ړ����[�V�����̊J�n
		StartMoveMotion(&g_player.Motion);

		//�v���C���[�̈ړ�(��)
		if (GetKeyboardPress(DIK_W))
		{
			// �ړ����[�V�����̊J�n
			StartMoveMotion(&g_player.Motion);

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * fSpeed;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * fSpeed;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.75f;
		}
		//�v���C���[�̈ړ�(��)
		else if (GetKeyboardPress(DIK_S))
		{
			// �ړ����[�V�����̊J�n
			StartMoveMotion(&g_player.Motion);

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * fSpeed;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * fSpeed;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.25f;
		}
		//�v���C���[�̈ړ�(�E)
		else
		{
			// �ړ����[�V�����̊J�n
			StartMoveMotion(&g_player.Motion);

			g_player.move.z -= sinf(pCamera->rot.y) * fSpeed;
			g_player.move.x += cosf(pCamera->rot.y) * fSpeed;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.5f;
		}

	}
	//�v���C���[�̈ړ�(��)
	else if (GetKeyboardPress(DIK_W) == true)
	{
		// �ړ����[�V�����̊J�n
		StartMoveMotion(&g_player.Motion);

		g_player.move.x += sinf(pCamera->rot.y) * fSpeed;
		g_player.move.z += cosf(pCamera->rot.y) * fSpeed;

		g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI;
	}
	//�v���C���[�̈ړ�(��)
	else if (GetKeyboardPress(DIK_S) == true)
	{
		// �ړ����[�V�����̊J�n
		StartMoveMotion(&g_player.Motion);

		g_player.move.x -= sinf(pCamera->rot.y) * fSpeed;
		g_player.move.z -= cosf(pCamera->rot.y) * fSpeed;

		g_player.rotDestPlayer.y = pCamera->rot.y;
	}
	else
	{
		StartNeutralMotion(&g_player.Motion);
	}
	// �v���C���[�̈ړ��ʂ̌���
	g_player.move.x += (0.0f - g_player.move.x) * 0.2f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.2f;

	// �O��̈ʒu�̕ۑ�
	g_player.posOld = g_player.pos;

	// �ʒu�̍X�V
	g_player.pos += g_player.move;
}
//===========================================
// �f�o�b�O�pImgui
//===========================================
void DebugImgui(void)
{
	StartImgui("Debug", IMGUITYPE_DEFOULT);

	SetPosImgui(ImVec2(0.0f, 0.0f));
	SetSizeImgui(ImVec2(400.0f, 500.0f));

	ImGui::DragFloat3(u8"��]", g_player.rot, 0.1f, -D3DX_PI, D3DX_PI);

	ImGui::Text(u8"�s�� 11 = %.4f 21 = %.4f 31 = %.4f", g_player.mtxWorldPlayer._11, g_player.mtxWorldPlayer._21, g_player.mtxWorldPlayer._31);
	ImGui::Text(u8"�s�� 12 = %.4f 22 = %.4f 32 = %.4f", g_player.mtxWorldPlayer._12, g_player.mtxWorldPlayer._22, g_player.mtxWorldPlayer._32);
	ImGui::Text(u8"�s�� 13 = %.4f 23 = %.4f 33 = %.4f", g_player.mtxWorldPlayer._13, g_player.mtxWorldPlayer._23, g_player.mtxWorldPlayer._33);
	ImGui::Text(u8"�s�� 41 = %.4f 42 = %.4f 43 = %.4f", g_player.mtxWorldPlayer._41, g_player.mtxWorldPlayer._42, g_player.mtxWorldPlayer._43);

	ImGui::Text(u8"�v���C���[�̈ʒu X = %.1f,Y = %.1f,Z = %.1f",g_player.pos.x, g_player.pos.y, g_player.pos.z);

	ImTextureID texture = (ImTextureID)(intptr_t)g_apTextureImgui;

	// �e�N�X�`��ID��n���A�{�^���̃T�C�Y���w��
	if (ImGui::ImageButton("����������", texture, ImVec2(128, 128)))
	{
		ImVec2 mousePos = ImGui::GetMousePos();
		blockPos = mousePos; // �摜���N���b�N���ꂽ�ʒu�Ƀu���b�N��z�u
	}

	// �h���b�O���̏���
	if (ImGui::IsMouseDragging(0))
	{
		ImVec2 mousePos = ImGui::GetMousePos();
		blockPos = mousePos; // �h���b�O���Ƀu���b�N�̈ʒu���X�V
	}

	static D3DXCOLOR col(1.0f,1.0,1.0f,1.0f);

	// �F�I���p�l����\���iRGBA�j
	ImGui::ColorEdit4("Pick a color", col);  // �F��I���ł���p�l����\��

		// ���C���[�t���[���ɂ��邩���Ȃ���
	static bool bWireFrame = false;

	// �J�����Ǐ]�̐ݒ�
	if (ImGui::Checkbox(u8"���C���[�t���[��", &bWireFrame))
	{
		if (bWireFrame)
		{
			onWireFrame(); // ���C���[�t���[��ON
		}
		else
		{
			offWireFrame(); // ���C���[�t���[��OFF
		}
	}


	ImGui::End();

}
//============================================
// �v���C���[�̃��[�h����
//============================================
void LoadFullBodyPlayer(const char* Filepath)
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
				g_player.Motion.nNumModel = nNumModel;	// ���f���̍ő吔����
			}

			// ���f���̓ǂݍ��݂��܂��I����Ă��Ȃ�������
			if (nLoadCnt < nNumModel)
			{
				// ���f���̓ǂݍ��񂾐���Ԃ�
				nLoadCnt = LoadFilename(pFile, nNumModel, &aStr[0]);
			}

			// CHARACTERSET��ǂݎ������
			if (strcmp(&aStr[0], "CHARACTERSET") == 0)
			{
				// �p�[�c�̐ݒ菈��
				LoadCharacterSet(pFile, &aStr[0], nNumParts);
			}

			// MOTIONSET��ǂݎ������
			if (strcmp(&aStr[0], "MOTIONSET") == 0)
			{
				// ���[�V�����̐ݒ菈��
				LoadMotionSet(pFile, &aStr[0], nNumModel);
			}

			// EOF��ǂݎ������
			if (nData == EOF)
			{
				// ���[�V�����̑�������
				g_player.Motion.nNumMotion = nCntMotion;
				nCntMotion = 0; // ���[�V�����̃J�E���g�����Z�b�g����
				break;          // While���𔲂���
			}
		}
	}
	else
	{
		// �t�@�C�����J���Ȃ�������
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(LoadPlayer)", MB_OK);
		return;
	}

	// �t�@�C�������
	fclose(pFile);
}
//=================================================
// �v���C���[�̃��f�����[�h
//=================================================
int LoadFilename(FILE* pFile, int nNumModel, char* aString)
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
				&g_player.Motion.aModel[nCntLoadModel].pBuffMat,
				NULL,
				&g_player.Motion.aModel[nCntLoadModel].dwNumMat,
				&g_player.Motion.aModel[nCntLoadModel].pMesh);

			if (g_player.Motion.aModel[nCntLoadModel].pBuffMat == NULL)
			{
				// ���b�Z�[�W�{�b�N�X�̕\��
				MessageBox(NULL, FILE_NAME, "���f�����ǂݍ��߂܂���", MB_OK | MB_ICONWARNING);

				nCntLoadModel++; // ���f���̃J�E���g�𑝂₷
				continue;
			}

			//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_player.Motion.aModel[nCntLoadModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_player.Motion.aModel[nCntLoadModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
						//�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_player.Motion.aModel[nCntLoadModel].pTexture[nCntMat]);

					// �e�N�X�`�����ǂݍ��߂Ȃ�������
					if (g_player.Motion.aModel[nCntLoadModel].pTexture[nCntMat] == NULL)
					{
						// �t�@�C���l�[����ۑ�
						const char* name = pMat[nCntMat].pTextureFilename;

						// ���b�Z�[�W�{�b�N�X�̕\��
						MessageBox(NULL, name, "�e�N�X�`�����ǂݍ��߂܂���", MB_OK | MB_ICONWARNING);
					}
				}
			}

			//���_���̎擾
			nNumVtx = g_player.Motion.aModel[nCntLoadModel].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y�擾
			sizeFVF = D3DXGetFVFVertexSize(g_player.Motion.aModel[nCntLoadModel].pMesh->GetFVF());

			//���_�o�b�t�@�̃��b�N
			g_player.Motion.aModel[nCntLoadModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
				if (vtx.x < g_player.Motion.aModel[nCntLoadModel].vtxMin.x)
				{
					g_player.Motion.aModel[nCntLoadModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_player.Motion.aModel[nCntLoadModel].vtxMin.y)
				{
					g_player.Motion.aModel[nCntLoadModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_player.Motion.aModel[nCntLoadModel].vtxMin.z)
				{
					g_player.Motion.aModel[nCntLoadModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_player.Motion.aModel[nCntLoadModel].vtxMax.x)
				{
					g_player.Motion.aModel[nCntLoadModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_player.Motion.aModel[nCntLoadModel].vtxMax.y)
				{
					g_player.Motion.aModel[nCntLoadModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_player.Motion.aModel[nCntLoadModel].vtxMax.z)
				{
					g_player.Motion.aModel[nCntLoadModel].vtxMax.z = vtx.z;
				}

				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				pVtxBuff += sizeFVF;
			}

			////�T�C�Y����
			//g_player.Size.x = g_player.vtxMaxPlayer.x - g_player.vtxMinPlayer.x;
			//g_player.Size.y = g_player.vtxMaxPlayer.y - g_player.vtxMinPlayer.y;
			//g_player.Size.z = g_player.vtxMaxPlayer.z - g_player.vtxMinPlayer.z;

			//�e���f�����Ƃ̃T�C�Y����
			g_player.Motion.aModel[nCntLoadModel].Size.x = g_player.Motion.aModel[nCntLoadModel].vtxMax.x - g_player.Motion.aModel[nCntLoadModel].vtxMin.x;
			g_player.Motion.aModel[nCntLoadModel].Size.y = g_player.Motion.aModel[nCntLoadModel].vtxMax.y - g_player.Motion.aModel[nCntLoadModel].vtxMin.y;
			g_player.Motion.aModel[nCntLoadModel].Size.z = g_player.Motion.aModel[nCntLoadModel].vtxMax.z - g_player.Motion.aModel[nCntLoadModel].vtxMin.z;

			//���_�o�b�t�@�̃A�����b�N
			g_player.Motion.aModel[nCntLoadModel].pMesh->UnlockVertexBuffer();

			nCntLoadModel++; // ���f���̃J�E���g�𑝂₷
		}
	}
	return nCntLoadModel; // ���f���̃J�E���g��Ԃ�

}
//=============================================
// �v���C���[�̃L�����N�^�[�Z�b�g
//=============================================
void LoadCharacterSet(FILE* pFile, char* aString, int nNumparts)
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
			nData = fscanf(pFile, "%f", &g_player.speed);    // �X�s�[�h����
		}

		//if (strcmp(aString, "JUMP") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]�ǂݔ�΂�
		//	nData = fscanf(pFile, "%f", &g_player.JumpHeight);    // �W�����v�̍�������
		//}

		//if (strcmp(aString, "SIZE") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]�ǂݔ�΂�
		//	nData = fscanf(pFile, "%f", &g_player.Size.x);    // �傫������
		//	nData = fscanf(pFile, "%f", &g_player.Size.y);    // �傫������
		//	nData = fscanf(pFile, "%f", &g_player.Size.z);    // �傫������
		//}

		//if (strcmp(aString, "BLOWOFF") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]�ǂݔ�΂�
		//	nData = fscanf(pFile, "%f", &g_player.BlowMove.x);    // �傫������
		//	nData = fscanf(pFile, "%f", &g_player.BlowMove.y);    // �傫������
		//}

		//if (strcmp(aString, "AVOIDMOVE") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]�ǂݔ�΂�
		//	nData = fscanf(pFile, "%f", &g_player.AvoidMove.x);    // �傫������
		//	nData = fscanf(pFile, "%f", &g_player.AvoidMove.y);    // �傫������
		//}

		else if (strcmp(aString, "PART") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �e�̃C���f�b�N�X��ۑ�
			nData = fscanf(pFile, "%d", &bUpper);

			g_player.Motion.aModel[nIdx].bUpperBody = bUpper == 1 ? true : false;
		}

		// PARENT��ǂݎ������
		else if (strcmp(aString, "PARENT") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �e�̃C���f�b�N�X��ۑ�
			nData = fscanf(pFile, "%d", &g_player.Motion.aModel[nIdx].nIdxModelParent);
		}
		// POS��ǂݎ������
		else if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			nData = fscanf(pFile, "%f", &g_player.Motion.aModel[nIdx].offpos.x);
			nData = fscanf(pFile, "%f", &g_player.Motion.aModel[nIdx].offpos.y);
			nData = fscanf(pFile, "%f", &g_player.Motion.aModel[nIdx].offpos.z);
		}
		// ROT��ǂݎ������
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			nData = fscanf(pFile, "%f", &g_player.Motion.aModel[nIdx].offrot.x);
			nData = fscanf(pFile, "%f", &g_player.Motion.aModel[nIdx].offrot.y);
			nData = fscanf(pFile, "%f", &g_player.Motion.aModel[nIdx].offrot.z);
			nIdx++;
		}
	}
}
//=================================================
// �v���C���[�̃��[�V�����Z�b�g
//=================================================
void LoadMotionSet(FILE* pFile, char* aString, int nNumModel)
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
			g_player.Motion.aMotionInfo[nCntMotion].bLoop = (Loop == 1) ? true : false;
		}
		// NUM_KEY��ǂݒʂ�����
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �L�[�̍ő吔����
			nData = fscanf(pFile, "%d", &g_player.Motion.aMotionInfo[nCntMotion].nNumkey);
		}
		else if (strcmp(aString, "NUM_FRAME") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �L�[�̍ő吔����
			nData = fscanf(pFile, "%d", &g_player.Motion.aMotionInfo[nCntMotion].nNumFrame);
		}
		else if (strcmp(aString, "START_FRAME") == 0)
		{
			int nCntFrame = 0;

			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �Ō�܂œ���I�������
			while (nCntFrame < g_player.Motion.aMotionInfo[nCntMotion].nNumFrame)
			{
				// �L�[�̍ő吔����
				nData = fscanf(pFile, "%d", &g_player.Motion.aMotionInfo[nCntMotion].nStartFrame[nCntFrame]);

				nCntFrame++;
			}
		}
		else if (strcmp(aString, "END_FRAME") == 0)
		{
			int nCntFrame = 0;

			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �Ō�܂œ���I�������
			while (nCntFrame < g_player.Motion.aMotionInfo[nCntMotion].nNumFrame)
			{
				// �L�[�̍ő吔����
				nData = fscanf(pFile, "%d", &g_player.Motion.aMotionInfo[nCntMotion].nEndFrame[nCntFrame]);

				nCntFrame++;
			}
		}

		// KEYSET��ǂݒʂ�����
		if (strcmp(aString, "KEYSET") == 0)
		{
			// �L�[�̐ݒ菈��
			LoadKeySet(pFile, aString, nCntMotion);
		}
		// END_MOTIONSET��ǂݒʂ�����
		if (strcmp(aString, "END_MOTIONSET") == 0)
		{
			nCntMotion++; // ���[�V�����̃J�E���g�����Z�b�g
			nKey = 0;     // �L�[�����Z�b�g
			break;
		}
	}
}
//============================================
// �v���C���[�̃��[�V�����̃L�[�Z�b�g
//============================================
void LoadKeySet(FILE* pFile, char* aString, int nCntMotion)
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
			nData = fscanf(pFile, "%d", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].nFrame);
		}

		// POS��ǂݒʂ�����
		if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �ʒu��ǂݍ���

			nData = fscanf(pFile, "%f", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosX);
			nData = fscanf(pFile, "%f", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosY);
			nData = fscanf(pFile, "%f", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROT��ǂݒʂ�����
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			nData = fscanf(pFile, "%s", &Skip[0]);

			// �p�x��ǂݍ���
			nData = fscanf(pFile, "%f", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotX);
			nData = fscanf(pFile, "%f", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotY);
			nData = fscanf(pFile, "%f", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotZ);
			nCntRot++;
		}
		// END_KEYSET��ǂݒʂ�����
		else if (strcmp(aString, "END_KEYSET") == 0)
		{
			nKey++;		 // �L�[�𑝂₷
			nCntPos = 0; // �ʒu�̃J�E���g�����Z�b�g
			nCntRot = 0; // �p�x�̃J�E���g�����Z�b�g
			break;
		}
	}
}
//=================================================
// �e��������Ȃ���
//=================================================
void SetFullBodyModel(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	//�S���f���p�[�c�̕`��
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransform;//�v�Z�p
		D3DXMATRIX mtxParent;//�e�̃}�g���b�N�X

		//�p�[�c�̃}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_player.Motion.aModel[nCntModel].mtxWorld);

		//�p�[�c�̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.Motion.aModel[nCntModel].rot.y, g_player.Motion.aModel[nCntModel].rot.x, g_player.Motion.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_player.Motion.aModel[nCntModel].mtxWorld, &g_player.Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

		//�p�[�c�̈ʒu(�I�t�Z�b�g)�𔽉f
		D3DXMatrixTranslation(&mtxTransform, g_player.Motion.aModel[nCntModel].pos.x, g_player.Motion.aModel[nCntModel].pos.y, g_player.Motion.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_player.Motion.aModel[nCntModel].mtxWorld, &g_player.Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

		//�p�[�c��[�e�̃}�g���b�N�X]��ݒ�
		if (g_player.Motion.aModel[nCntModel].nIdxModelParent != -1)
		{
			//�e���f��������ꍇ
			mtxParent = g_player.Motion.aModel[g_player.Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{//�e���f�����Ȃ��ꍇ
			mtxParent = g_player.mtxWorldPlayer;
		}

		//�Z�o����[�p�[�c�̃��[���h�}�g���b�N�X]��[�e�̃}�g���b�N�X]���������킹��
		D3DXMatrixMultiply(&g_player.Motion.aModel[nCntModel].mtxWorld,
			&g_player.Motion.aModel[nCntModel].mtxWorld,
			&mtxParent);//���������e

		//�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD,
			&g_player.Motion.aModel[nCntModel].mtxWorld);


		for (int nCntMat = 0; nCntMat < (int)g_player.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_player.Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

			// ���f���̏�񂪂Ȃ�������
			if (g_player.Motion.aModel[nCntModel].pBuffMat == NULL)
			{
				continue;
			}

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_player.Motion.aModel[nCntModel].pTexture[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_player.Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
		}

		//// ���ɂȂ�����
		//if (nCntModel == g_player.nOrbitModel)
		//{
		//	SetMtx();
		//}
	}
}
//=================================================
// �j���[�g�������[�V�����̊J�n
//=================================================
void StartNeutralMotion(MOTION* pMotion)
{
	// ���[�V�����̃p�[�g����
	for (int nCnt = 0; nCnt < MOTION_PART_MAX; nCnt++)
	{
		if (g_player.bFullBody == true && nCnt == 1) break;

		if (pMotion->MotionData[nCnt].motiontypeBlend == MOTIONTYPE_MOVE)
		{
			SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, true, 5, nCnt);
		}
	}
}
//=================================================
// �ړ����[�V�����̊J�n
//=================================================
void StartMoveMotion(MOTION* pMotion)
{
	// ���[�V�����̃p�[�g����
	for (int nCnt = 0; nCnt < MOTION_PART_MAX; nCnt++)
	{
		if (g_player.bFullBody == true && nCnt == 1) break;

		// ���[�V�����̐ݒ�
		SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 10, nCnt);	
	}
}

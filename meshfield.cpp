//=====================================================================================================================
//
// ���b�V���t�B�[���h[meshfield.cpp]
// Author: TEAM_C
//
//=====================================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "meshfield.h"
#include "input.h"
#include "player.h"
#include "math.h"
#include"imgui.h"
#include "line.h"
#include "ball.h"

//**************************************************************************************************************
//�O���[�o���ϐ��錾
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;	    // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL; // ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshFiled = NULL;  // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MESHFILED g_MeshField;

//=================================================================================================================
//���b�V���t�B�[���h�̏���������
//=================================================================================================================
void InitMeshField(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/field.png",
		&g_pTextureMeshField);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(DWORD) * INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshFiled,
		NULL);

	g_MeshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshField.bEditMode = false;

	VERTEX_3D* pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	float fTexX = 1.0f / XVTX;
	float fTexY = 1.0f / ZVTX;
	int nCnt = 0;

	//�c
	for (int nCntZ = 0; nCntZ <= ZVTX; nCntZ++)
	{
		//��
		for (int nCntX = 0; nCntX <= XVTX; nCntX++)
		{
			pVtx[nCnt].pos = D3DXVECTOR3(
				(0.0f + (MAX_WIDTH / XVTX) * nCntX) - (MAX_WIDTH * 0.5f),
				0.0f,
				MAX_HEIGHT - ((MAX_HEIGHT / ZVTX) * nCntZ) - (MAX_HEIGHT * 0.5f));

			//�@���x�N�g���̐ݒ�
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntX,nCntZ * fTexY);

			nCnt++;//���Z
		}
	}

	g_pVtxBuffMeshField->Unlock();

	DWORD* pIdx;

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	g_pIdxBuffMeshFiled->Lock(0, 0, (void**)&pIdx, 0);

	int IndxNum = XVTX + 1;//X

	int IdxCnt = 0;//�z��

	int Num = 0;//

	//�C���f�b�N�X�̐ݒ�
	for (int IndxCount1 = 0; IndxCount1 < ZVTX; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= XVTX; IndxCount2++, IndxNum++, Num++)
		{
			pIdx[IdxCnt] = IndxNum;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}

		// NOTE:�Ō�̍s����Ȃ�������
		if (IndxCount1 < ZVTX - 1) 
		{
			pIdx[IdxCnt] = Num - 1;
			pIdx[IdxCnt + 1] = IndxNum;
			IdxCnt += 2;
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffMeshFiled->Unlock();
}

//=================================================================================================================
//���b�V���t�B�[���h�̏I������
//=================================================================================================================ccc
void UninitMeshField(void)
{
	//�e�N�X�`���̉��
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//���_�o�b�t�@�̉��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̉��
	if (g_pIdxBuffMeshFiled != NULL)
	{
		g_pIdxBuffMeshFiled->Release();
		g_pIdxBuffMeshFiled = NULL;
	}
}

//=================================================================================================================
//���b�V���t�B�[���h�̍X�V����
//=================================================================================================================
void UpdateMeshField(void)
{
	VERTEX_3D* pVtx;

	MODE mode = GetMode();

	Player* pPlayer = GetPlayer();

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	static float speed = -10.5f;
	float fHeight = 100.0f;
	int nCnt = 0;

	for (int nCntVertex = 0; nCntVertex < VERTEX; nCntVertex++)
	{
		if (GetKeyboardPress(DIK_UP))
		{
			if (sphererange(&pVtx[nCntVertex].pos, &pPlayer->pos, 100.0f, 50.0f))
			{
				pVtx[nCntVertex].pos.y += 10.0f;
			}
		}
	}

	//for (int nCntZ = 0; nCntZ <= ZVTX; nCntZ++)
	//{
	//	// ��
	//	for (int nCntX = 0; nCntX <= XVTX; nCntX++)
	//	{
	//		float offset = sinf(g_MeshField.fTime * speed + nCntZ);  // �s���Ƃ̔g�̈ʑ��𒲐�

	//		// �g�̌v�Z�i�e���_�̍����j
	//		pVtx[nCnt].pos.y = offset * fHeight;
	//		//
	//		nCnt++;
	//	}
	//}

	g_MeshField.fTime += 1.0f / 60.0f;



	//for (int nCntZ = 0; nCntZ <= ZVTX; nCntZ++)
	//{
	//	// ��
	//	for (int nCntX = 0; nCntX <= XVTX; nCntX++)
	//	{

	//		float fDistance = GetDistance(pVtx[nCnt].pos, pos);

	//		float offSet = sinf(g_MeshField.fTime + (fDistance * 0.01f));

	//		if (offSet <= 0.0f)
	//		{
	//			offSet *= -1.0f;
	//		}

	//		if (fDistance >= speed - 100.0f && fDistance <= speed + 100.0f)
	//		{
	//			// �g�̌v�Z�i�e���_�̍����j
	//			pVtx[nCnt].pos.y = fHeight * offSet;
	//			pVtx[nCnt].col = COLOR_RED;
	//		}
	//		else
	//		{
	//			pVtx[nCnt].pos.y = 0.0f;
	//			pVtx[nCnt].col = COLOR_WHITE;
	//		}
	//		nCnt++;
	//	}
	//}


	//if (GetKeyboardPress(DIK_Q))
	//{
	//	speed -= 4.0f;
	//}
	//else
	//{
	//	speed += 4.0f;
	//}
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffMeshField->Unlock();
}
//=================================================================================================================
//���b�V���t�B�[���h�̕`�揈��
//=================================================================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_MeshField.mtxWorldMeshField);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField.rot.y, g_MeshField.rot.x, g_MeshField.rot.z);
	D3DXMatrixMultiply(&g_MeshField.mtxWorldMeshField, &g_MeshField.mtxWorldMeshField, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_MeshField.pos.x, g_MeshField.pos.y, g_MeshField.pos.z);
	D3DXMatrixMultiply(&g_MeshField.mtxWorldMeshField, &g_MeshField.mtxWorldMeshField, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshField.mtxWorldMeshField);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshFiled);

	//�e�N�X�`���t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshField);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,VERTEX,0,POLYGON);
}
//==================================================================================================================
//���b�V���t�B�[���h�̓����蔻��
//==================================================================================================================
bool CollisionField(D3DXVECTOR3* pPos)
{
	Player* pPlayer = GetPlayer();

	// ���n����
	bool bLanding = false;

	DWORD* pIdx;

	VERTEX_3D* pVtx;

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	g_pIdxBuffMeshFiled->Lock(0, 0, (void**)&pIdx, 0);

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	// 1�}�X�̃T�C�Y
	float GridSizeX = MAX_WIDTH / (float)XVTX;
	float GridSizeZ = MAX_HEIGHT / (float)ZVTX;

	float X = pPos->x + (MAX_WIDTH * 0.5f);
	float Z = (MAX_HEIGHT * 0.5f) - pPos->z;

	// ���Ԗڂ̃|���S����
	int polyX = (int)(X / GridSizeX);
	int polyZ = (int)(Z / GridSizeZ);

	// ���݂̃|���S���̃C���f�b�N�X�ԍ�
	int polyIndex = ((polyZ * (XVTX - 1) + polyX) * 2) + (polyZ * 6);

	ImGui::Text("polyIndex = %d", polyIndex);

	for (int nCnt = 0; nCnt < NUM_POLYGON; nCnt++)
	{	
		// ���_�̃C���f�b�N�X
		int Vertex = (polyIndex + nCnt);

		// �}�C�i�X��������
		if (Vertex < 0 || Vertex > INDEX) break;

		// �C���f�b�N�X���擾
		int nIdx0 = pIdx[Vertex];
		int nIdx1 = pIdx[Vertex + 1];
		int nIdx2 = pIdx[Vertex + 2];

		// ���_���擾
		D3DXVECTOR3 vtx0 = pVtx[nIdx0].pos;
		D3DXVECTOR3 vtx1 = pVtx[nIdx1].pos;
		D3DXVECTOR3 vtx2 = pVtx[nIdx2].pos;

		D3DXVECTOR3 edge0 = vtx1 - vtx0; // �Ӄx�N�g��0
		D3DXVECTOR3 edge1 = vtx2 - vtx1; // �Ӄx�N�g��1
		D3DXVECTOR3 edge2 = vtx0 - vtx2; // �Ӄx�N�g��2

		D3DXVECTOR3 Normal = {};

		// �����Ԗڂ�������
		if (nCnt % 2 == 0)
		{
			// �����Ԗڂ̎O�p�`
			D3DXVec3Cross(&Normal, &edge0, &edge1);
		}
		else
		{
			// ��Ԗڂ̎O�p�`�i�������t�ɂȂ��Ă���j
			D3DXVec3Cross(&Normal, &edge1, &edge0);
		}

		D3DXVec3Normalize(&Normal, &Normal);

		g_MeshField.normal = Normal;

		D3DXVECTOR3 PlayerVec0 = *pPos - vtx0;
		D3DXVECTOR3 PlayerVec1 = *pPos - vtx1;
		D3DXVECTOR3 PlayerVec2 = *pPos - vtx2;

		D3DXVECTOR3 Cross0 = {};
		D3DXVECTOR3 Cross1 = {};
		D3DXVECTOR3 Cross2 = {};

		if (nCnt % 2 == 0)
		{
			// �O�p�`�̒��_����O��
			D3DXVec3Cross(&Cross0, &edge0, &PlayerVec0);
			D3DXVec3Normalize(&Cross0, &Cross0);

			D3DXVec3Cross(&Cross1, &edge1, &PlayerVec1);
			D3DXVec3Normalize(&Cross1, &Cross1);

			D3DXVec3Cross(&Cross2, &edge2, &PlayerVec2);
			D3DXVec3Normalize(&Cross2, &Cross2);
		}
		else
		{
			// �O�p�`�̒��_����O��
			D3DXVec3Cross(&Cross0, &PlayerVec0, &edge0);
			D3DXVec3Normalize(&Cross0, &Cross0);

			D3DXVec3Cross(&Cross1, &PlayerVec1, &edge1);
			D3DXVec3Normalize(&Cross1, &Cross1);

			D3DXVec3Cross(&Cross2, &PlayerVec2, &edge2);
			D3DXVec3Normalize(&Cross2, &Cross2);
		}

		if (Cross0.y >= 0.0f && Cross1.y >= 0.0f && Cross2.y >= 0.0f)
		{
			// ���ʂ̕�������D���v�Z
			float D = -(Normal.x * vtx0.x + Normal.y * vtx0.y + Normal.z * vtx0.z);

			// �v���C���[�̈ʒu�Ɋ�Â��āA�v���C���[��Y�ʒu���v�Z
			float PosY = (Normal.x * pPos->x + Normal.z * pPos->z + D) / -Normal.y;

			D3DXVECTOR3 vec = vtx0 - *pPos;
			D3DXVec3Normalize(&vec, &vec);

			// �v���C���[���|���S���̗������ǂ����̔���
			float dot = D3DXVec3Dot(&Normal, &vec); // �@���ƃv���C���[�����Ƃ̓���

			// �v���C���[���|���S���̗����ɏo��
			if (dot > 0)
			{
				GetBall()->normal = Normal;
				// Y���W��ݒ�
				pPos->y = PosY;

				// ���n���Ă���
				bLanding = true;
				break;
			}
		}
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffMeshField->Unlock();

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffMeshFiled->Unlock();

	return bLanding;//�����Ԃ�
}
//===================================================
// ����Ă���|���S���̖@���̎擾
//===================================================
D3DXVECTOR3 GetCollisionNormal(void)
{
	return g_MeshField.normal;
}


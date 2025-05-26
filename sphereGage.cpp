//=============================================================
//
// �~�`�̃Q�[�W [sphereGage.cpp]
// Author:YUTO YOSHIDA
//
//=============================================================

//*************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************
#include "sphereGage.h"
#include "player.h"

//*************************************************************
// �}�N����`
//*************************************************************

#define XVTX (36)
#define YVTX (1)

#define VERTEX ((XVTX + 1) * (YVTX + 1)) //���_��
#define INDEX (POLYGON + 2)  //�C���f�b�N�X
#define POLYGON (((XVTX * YVTX) * 2)) + (4 * (YVTX - 1)) //�|���S����

//*************************************************************
// �O���[�o���ϐ��錾
//*************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSphere = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSphere = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBufferSphere = NULL;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

//=============================================================
// ����������
//=============================================================
void InitSphere(void)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\gameclear.png",
		&g_pTextureSphere);

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSphere,
		NULL);

	Player* pPlayer = GetPlayer();

	// HP�̊��������߂�
	float fRateLife = (float)pPlayer->nLife / (float)pPlayer->nMaxLife;

	float fCurrentVtx = fRateLife * VERTEX;

	//���_���b�N
	g_pVtxBuffSphere->Lock(0, 0, (void**)&pVtx, 0);

	float fTexX = 1.0f / XVTX;
	float fTexY = 1.0f / YVTX;

	float fRadius = 50.0f;

	int nCnt = 0;

	for (int nCntY = 0; nCntY <= YVTX; nCntY++)
	{
		for (int nCntX = 0; nCntX <= XVTX; nCntX++)
		{
			float fAngle = (D3DX_PI * 2.0f) / XVTX * nCntX;

			//���_���W�̐ݒ�
			pVtx[nCnt].pos = D3DXVECTOR3(
				640.0f + sinf(fAngle) * fRadius,
				340.0f + cosf(fAngle) * fRadius,
				0.0f);

			//rhw�̐ݒ�
			pVtx[nCnt].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[nCnt].col = D3DCOLOR_RGBA(200, 255, 100, 255);

			pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntY, fTexY * nCntX);

			nCnt++;
		}
		fRadius += fRadius;
	}
	//���_���b�N����
	g_pVtxBuffSphere->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBufferSphere,
		NULL);

	WORD* pIdx;

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	g_pIdxBufferSphere->Lock(0, 0, (void**)&pIdx, 0);

	int IndxNum = XVTX + 1;//X

	int IdxCnt = 0;//�z��

	int Num = 0;//

	//�C���f�b�N�X�̐ݒ�
	for (int IndxCount1 = 0; IndxCount1 < YVTX; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= XVTX; IndxCount2++, IndxNum++, Num++)
		{
			pIdx[IdxCnt] = IndxNum;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}

		// NOTE:�Ō�̍s����Ȃ�������
		if (IndxCount1 < YVTX - 1)
		{
			pIdx[IdxCnt] = Num - 1;
			pIdx[IdxCnt + 1] = IndxNum;
			IdxCnt += 2;
		}

	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBufferSphere->Unlock();

}
//=============================================================
// �I������
//=============================================================
void UninitSphere(void)
{
	if (g_pTextureSphere != NULL)
	{
		g_pTextureSphere->Release();
		g_pTextureSphere = NULL;
	}
	if (g_pVtxBuffSphere != NULL)
	{
		g_pVtxBuffSphere->Release();
		g_pVtxBuffSphere = NULL;
	}
	if (g_pIdxBufferSphere != NULL)
	{
		g_pIdxBufferSphere->Release();
		g_pIdxBufferSphere = NULL;
	}

}
//=============================================================
// �X�V����
//============================================================
void UpdateSphere(void)
{

}
//=============================================================
// �`�揈��
//============================================================
void DrawSphere(void)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSphere, 0, sizeof(VERTEX_2D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBufferSphere);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	Player* pPlayer = GetPlayer();

	// HP�̊��������߂�
	float fRateLife = (float)pPlayer->nLife / (float)pPlayer->nMaxLife;

	int fCurrentVtxX = fRateLife * XVTX;

	int vertex = (fCurrentVtxX + 1) * (YVTX + 1);
	int fCurrentPolygon = ((fCurrentVtxX * YVTX) * 2) + (4 * (YVTX - 1));

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, vertex, 0, fCurrentPolygon);
}

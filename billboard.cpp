//============================
//
// �r���{�[�h[billboard.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "billboard.h"
#include "input.h"
#include "player.h"
#include "camera.h"
#include "quaternion.h"

//****************************
//�}�N����`
//****************************
#define BILL_WIDTH (3.0f) //����
#define BILL_HEIGHT (100.0f) //����
#define MAX_BILLBOARD (128) //�r���{�[�h�̍ő吔
#define MAX_MOVE (5.0f) //�ړ���

//****************************
//�O���[�o���ϐ��錾
//****************************
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL; //���_�o�b�t�@�ւ̃|�C���^
BILLBOARD g_Billboard[MAX_BILLBOARD]; 

//============================
//�r���{�[�h�̏���������
//============================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",
		&g_pTextureBillboard);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D* pVtx = NULL;

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		g_Billboard[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Billboard[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Billboard[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Billboard[nCnt].nLife = 0;
		g_Billboard[nCnt].bUse = false;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Billboard[nCnt].pos.x - BILL_WIDTH, g_Billboard[nCnt].pos.y + BILL_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Billboard[nCnt].pos.x + BILL_WIDTH, g_Billboard[nCnt].pos.y + BILL_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Billboard[nCnt].pos.x - BILL_WIDTH, g_Billboard[nCnt].pos.y - BILL_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Billboard[nCnt].pos.x + BILL_WIDTH, g_Billboard[nCnt].pos.y - BILL_HEIGHT, 0.0f);

		//�e���_�̖@���̐ݒ�(�x�N�g���̑傫���͂P�ɂ���K�v������)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);//�J�����ɑ΂��Đ�����
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.3f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffBillboard->Unlock();
}
//============================
//�r���{�[�h�̏I������
//============================
void UninitBillboard(void)
{
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//���_�o�b�t�@�̉��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}
//============================
//�r���{�[�h�̍X�V����
//============================
void UpdateBillboard(void)
{
	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		//�g�p����Ă���
		if (g_Billboard[nCnt].bUse == true)
		{
			g_Billboard[nCnt].rot.x += 0.01f;
		}
	}
}
//============================
//�r���{�[�h�̕`�揈��
//============================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	Player* pPlayer = GetPlayer();
	Camera* pCamera = GetCamera();

	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans,mtxView;

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (g_Billboard[nCnt].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Billboard[nCnt].mtxWorld);

			pDevice->GetTransform(D3DTS_VIEW, &mtxView);//�r���[�}�g���b�N�X�̎擾

			//�J�����̋t�s��̐ݒ�
			g_Billboard[nCnt].mtxWorld._11 = mtxView._11;
			g_Billboard[nCnt].mtxWorld._12 = mtxView._21;
			g_Billboard[nCnt].mtxWorld._13 = mtxView._31;

			g_Billboard[nCnt].mtxWorld._21 = mtxView._12;
			g_Billboard[nCnt].mtxWorld._22 = mtxView._22;
			g_Billboard[nCnt].mtxWorld._23 = mtxView._32;

			g_Billboard[nCnt].mtxWorld._31 = mtxView._13;
			g_Billboard[nCnt].mtxWorld._32 = mtxView._23;
			g_Billboard[nCnt].mtxWorld._33 = mtxView._33;
			
			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Billboard[nCnt].pos.x, g_Billboard[nCnt].pos.y, g_Billboard[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Billboard[nCnt].mtxWorld, &g_Billboard[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Billboard[nCnt].mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//�e�N�X�`���t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBillboard);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//============================
//�r���{�[�h�̐ݒ菈��
//============================
void SetBillboard(D3DXVECTOR3 pos)
{
	VERTEX_3D* pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (g_Billboard[nCnt].bUse == false)
		{
			g_Billboard[nCnt].pos = pos;
			g_Billboard[nCnt].bUse = true;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - BILL_WIDTH, pos.y + BILL_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + BILL_WIDTH, pos.y + BILL_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - BILL_WIDTH, pos.y - BILL_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + BILL_WIDTH, pos.y - BILL_HEIGHT, 0.0f);

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffBillboard->Unlock();
}

//============================================================================================================
//
// ���̕`��  [line.cpp]
// Author: YOSHIDA YUTO
//
//============================================================================================================

//***********************************************************************************************************
// �C���N���[�h�t�@�C��
//***********************************************************************************************************
#include "line.h"

//***********************************************************************************************************
// �}�N����`
//***********************************************************************************************************
#define BOX_VERTEX (8)		// ���̒��_�̐�
#define BOXLINE_VERTEX (12) // ���̐��̒��_�̐�

//============================================================================================================
// �v���g�^�C�v�錾
//============================================================================================================

//============================================================================================================
// �O���[�o���ϐ��錾
//============================================================================================================


//============================================================================================================
// ���̕`�揈��
//============================================================================================================
void DrawBoxLine(D3DXVECTOR3 pos,D3DXVECTOR3 Size,D3DXCOLOR col,bool bSenterPos)
{
	D3DXVECTOR3 BoundingBoxPos[BOX_VERTEX] = {};

	// ���S��������
	if (bSenterPos == true)
	{
		// �{�b�N�X�̒��_��ł�
		BoundingBoxPos[0] = pos + D3DXVECTOR3(-Size.x, -Size.y, -Size.z);
		BoundingBoxPos[1] = pos + D3DXVECTOR3(Size.x, -Size.y, -Size.z);
		BoundingBoxPos[2] = pos + D3DXVECTOR3(Size.x, Size.y, -Size.z);
		BoundingBoxPos[3] = pos + D3DXVECTOR3(-Size.x, Size.y, -Size.z);
		BoundingBoxPos[4] = pos + D3DXVECTOR3(-Size.x, -Size.y, Size.z);
		BoundingBoxPos[5] = pos + D3DXVECTOR3(Size.x, -Size.y, Size.z);
		BoundingBoxPos[6] = pos + D3DXVECTOR3(Size.x, Size.y, Size.z);
		BoundingBoxPos[7] = pos + D3DXVECTOR3(-Size.x, Size.y, Size.z);
	}
	else // ���S����Ȃ�������
	{
		// �{�b�N�X�̒��_��ł�
		BoundingBoxPos[0] = pos + D3DXVECTOR3(-Size.x, 1.0f, -Size.z);
		BoundingBoxPos[1] = pos + D3DXVECTOR3(Size.x, 1.0f, -Size.z);
		BoundingBoxPos[2] = pos + D3DXVECTOR3(Size.x, Size.y, -Size.z);
		BoundingBoxPos[3] = pos + D3DXVECTOR3(-Size.x, Size.y, -Size.z);
		BoundingBoxPos[4] = pos + D3DXVECTOR3(-Size.x, 1.0f, Size.z);
		BoundingBoxPos[5] = pos + D3DXVECTOR3(Size.x, 1.0f, Size.z);
		BoundingBoxPos[6] = pos + D3DXVECTOR3(Size.x, Size.y, Size.z);
		BoundingBoxPos[7] = pos + D3DXVECTOR3(-Size.x, Size.y, Size.z);
	}

	// �o�E���f�B���O�{�b�N�X���\�����郉�C���̃y�A�i���_�̃C���f�b�N�X�j
	int lines[BOXLINE_VERTEX][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},  // �O��
		{4, 5}, {5, 6}, {6, 7}, {7, 4},  // �w��
		{0, 4}, {1, 5}, {2, 6}, {3, 7}   // �㉺
	};

	// ����`��
	for (int nCntVetx = 0; nCntVetx < BOXLINE_VERTEX; nCntVetx++)
	{
		// ���_�̃C���f�b�N�X
		int startidx = lines[nCntVetx][0];

		// �I�_�̃C���f�b�N�X
		int endidx = lines[nCntVetx][1];

		D3DXVECTOR3 start = BoundingBoxPos[startidx];
		D3DXVECTOR3 end = BoundingBoxPos[endidx];

		// ���̕`��
		Draw3DLine(start, end, col);
	}
}
//============================================================================================================
// 
//============================================================================================================
void Draw3DLine(D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR color)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX prevWorld;
	pDevice->GetTransform(D3DTS_WORLD, &prevWorld);  // ���̃}�g���N�X�ۑ�

	D3DXMATRIX Identity;
	D3DXMatrixIdentity(&Identity);
	pDevice->SetTransform(D3DTS_WORLD, &Identity);  // �ꎞ�I�ɖ��e���ɂ���


	LINEVERTEX line[2];  // �����\������2�̒��_

	line[0].pos = start;
	line[0].color = color;

	line[1].pos = end;
	line[1].color = color;

	// ���_�t�H�[�}�b�g��ݒ�
	pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	// �e�N�X�`�����O��
	pDevice->SetTexture(0, NULL);

	// ���_�f�[�^���g���ă��C����`��
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, line, sizeof(LINEVERTEX));

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetTransform(D3DTS_WORLD, &prevWorld);  // ���ɖ߂�
}

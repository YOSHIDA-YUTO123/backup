//============================
//
// �r���{�[�h[billboard.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"main.h"

//****************************
//�r���{�[�h�̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 rot;//����
	D3DXMATRIX mtxWorld;//���[���h�}�g���b�N�X
	int nLife;//����
	bool bUse;//�g�p���Ă��邩�ǂ���
}BILLBOARD;

//****************************
//�v���g�^�C�v�錾
//****************************
void InitBillboard(void);//�r���{�[�h�̏���������
void UninitBillboard(void);//�r���{�[�h�̏I������
void UpdateBillboard(void);//�r���{�[�h�̍X�V����
void DrawBillboard(void);//�r���{�[�h�̕`�揈��
void SetBillboard(D3DXVECTOR3 pos);
#endif

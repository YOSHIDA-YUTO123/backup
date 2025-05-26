//==============================================================================================================
//
// �{�[�� [ball.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _BALL_H_
#define _BALL_H_

//***************************
//�C���N���[�h�t�@�C��
//***************************
#include"main.h"

//***************************
//�}�N����`
//***************************
#define MAX_TEXTURE (128)	// �e�N�X�`���̍ő吔
#define MAX_NAME (256)		// ���O�̒���

//***************************
//���f���̍\����
//***************************
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEXTURE];	//�v���C���[�̃e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh;							//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;						//�}�e���A���ւ̃|�C���^
	DWORD dwNumMat;								//�}�e���A���̐�
	D3DXVECTOR3 pos,posOld;							//�ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;							//����
	D3DXMATRIX mtxWorld;						//���[���h�}�g���b�N�X
	D3DXVECTOR3 vtxMin, vtxMax;
	D3DXVECTOR3 Size;
	D3DXVECTOR3 normal;
	bool bJump;
}Ball;

//***************************
//�v���g�^�C�v�錾
//***************************
void InitBalll(void);
void UninitBalll(void);
void UpdateBalll(void);
void DrawBalll(void);
Ball* GetBall(void);
#endif

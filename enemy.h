//==================================================
//
// �G
// Author:YOSHIDA YUUTO
//
//==================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_

//**************************
//�C���N���[�h�t�@�C��
//**************************
#include"main.h"
#include"motion.h"

//**************************
//�}�N����`
//**************************
#define MAX_TEXTURE (128)

//**************************
// �G�̍s���̎��
//**************************
typedef enum
{
	ACTONSTATE_NEUTRAL = 0,
	ACTIONSTATE_MOVE,
	ACTIONSTATE_ATTACK,
	ACTIONSTATE_BACKSTEP,
	ACTIONSTATE_MAX
}ACTIONSTATE;

//**************************
// �v���C���[�\����
//**************************
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 posOld;			// �O��̈ʒu
	D3DXVECTOR3 Size;			// �傫��
	D3DXVECTOR3 rot;			// ����
	D3DXVECTOR3 rotDestEnemy;	// �ړI�̊p�x
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3	vtxMinEnemy;
	D3DXVECTOR3	vtxMaxEnemy;
	D3DXMATRIX mtxWorldEnemy;	// ���[���h�}�g���b�N�X
	int nIdxShadow;
	bool bJump;
	bool bUse;
	int nCounterState;
	int nLife;
	int nMaxLife;
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEXTURE];	// �v���C���[�̃e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh;							// ���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;						// �}�e���A���ւ̃|�C���^
	DWORD dwNumMat;								// �}�e���A���̐�
	D3DXVECTOR3 axis;	// �v���C���[�̉�]��
	MOTION Motion;		// ���[�V�����̏��
	float speed;		// ����
	bool bFullBody;		// �S�g�ɂ��邩���Ȃ���
	int nActionType;
}Enemy;

//**************************
//�v���g�^�C�v�錾
//**************************
void InitEnemy(void);//�v���C���[�̏���������
void UninitEnemy(void);//�v���C���[�̏I������
void UpdateEnemy(void);//�v���C���[�̍X�V����
void DrawEnemy(void);//�v���C���[�̕`�揈��
void SetEnemy(D3DXVECTOR3 pos);
Enemy* GetEnemy(void);//�v���C���[�̎擾����
void DebugImgui(void);
#endif

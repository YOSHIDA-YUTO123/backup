//==============================================================================================================
//
//�v���C���[
//Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

//**************************
//�C���N���[�h�t�@�C��
//**************************
#include"main.h"
#include"motion.h"

//**************************
//�}�N����`
//**************************
#define PLAYER_MOVE (1.0f)
#define WIDTH (100.0f) //�ǂ̉���
#define HEIGHT (100.0f) //�ǂ̍���
#define MAX_MODEL (16)
#define MAX_ITEM (256)
#define MAX_GLABITY (1.0f)      // �d��
#define MAX_TEXTURE (128)

//**************************
//�v���C���[�̏��
//**************************
typedef enum
{
	PLAYERSTATE_NORMAL = 0,
	PLAYERSTATE_MOVE,
	PLAYERSTATE_ATTACK,
	PLAYERSTATE_JUMP,
	PLAYERSTATE_LANDING,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_FALL,
	PLAYERSTATE_INVISIBLE,
	PLAYERSTATE_NO_WEPON,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//**************************
//�v���C���[�\����
//**************************
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 posOld;			// �O��̈ʒu
	D3DXVECTOR3 Size;			// �傫��
	D3DXVECTOR3 rot;			// ����
	D3DXVECTOR3 rotDestPlayer;	// �ړI�̊p�x
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3	vtxMinPlayer;
	D3DXVECTOR3	vtxMaxPlayer;
	D3DXMATRIX mtxWorldPlayer;	// ���[���h�}�g���b�N�X
	int nIdxShadow;
	bool bJump;
	bool bDisp;

	PLAYERSTATE state;
	int nCounterState;
	int nLife;
	int nMaxLife;
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEXTURE];	//�v���C���[�̃e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh;							//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;						//�}�e���A���ւ̃|�C���^
	DWORD dwNumMat;								//�}�e���A���̐�

	D3DXVECTOR3 axis;	// �v���C���[�̉�]��
	MOTION Motion;		// ���[�V�����̏��
	float speed;		// ����
	bool bFullBody;		// �S�g�ɂ��邩���Ȃ���
	D3DXVECTOR3 normal;
}Player;

//**************************
//�v���g�^�C�v�錾
//**************************
void InitPlayer(void);//�v���C���[�̏���������
void UninitPlayer(void);//�v���C���[�̏I������
void UpdatePlayer(void);//�v���C���[�̍X�V����
void DrawPlayer(void);//�v���C���[�̕`�揈��
Player* GetPlayer(void);//�v���C���[�̎擾����
void DebugImgui(void);
#endif

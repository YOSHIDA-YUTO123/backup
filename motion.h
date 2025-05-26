//==============================================================================================================
//
// ���[�V����[motion.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _MOTION_H_
#define _MOTION_H_

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "main.h"
#include<stdio.h>
#include"model.h"

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define FILENAME001 ("data\\ModelPlayer.txt")
#define MAX_PARTS (30) // �p�[�c�̍ő吔
#define MAX_MOTION (25) // ���[�V�����̍ő吔
#define MAX_KEY (56) // �L�[�̍ő吔
#define NUM_FRAME (10)	// �����蔻��̔����t���[��

//**************************
// �L�����̎��
//**************************
typedef enum
{
	MOTION_PART_FULLBODY = 0,
	MOTION_PART_UPPERBODY,
	MOTION_PART_LOWERBODY,
	MOTION_PART_MAX
}MOTION_PART;

//**************************
// ���f���̃��[�V����
//**************************
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,
	MOTIONTYPE_MOVE,
	MOTIONTYPE_ACTION,
	MOTIONTYPE_JUMP,
	MOTIONTYPE_LANDING,
	MOTIONTYPE_ACTION2,
	MOTIONTYPE_ACTION3,
	MOTIONTYPE_ACTION4,
	MOTIONTYPE_DAMAGE,
	MOTIONTYPE_BLOWOFF,
	MOTIONTYPE_AVOID,
	MOTIONTYPE_MAX
}MOTIONTYPE;

//**************************
// ���[�V�����̖��O
//**************************
static const char* MOTIONNAME[MAX_MOTION] =
{
	"NEUTRAL",
	"MOVE",
	"ACTION",
	"JUMP",
	"LANDING",
	"ACTION2",
	"ACTION3",
	"ACTION4",
	"DAMAGE",
	"BLOWOFF",
	"AVOID",
	"free0",
	"free1",
	"free2",
	"free3",
	"free4",
	"free5",
	"free6",
	"free7",
	"free8",
	"free9",
	"free10",
	"free11",
	"free12",
	"free13",
};

//**************************
// �L�[�\����
//**************************
typedef struct
{
	float fPosX;//�ʒuX
	float fPosY;//�ʒuY
	float fPosZ;//�ʒuZ

	float fRotX;//����X
	float fRotY;//����Y
	float fRotZ;//����Z

}KEY;

//**************************
// �L�[���\����
//**************************
typedef struct
{
	int nFrame;			// �Đ��t���[��
	KEY aKey[MAX_PARTS];// �e�L�[�p�[�c�̃L�[�v�f
}KEY_INFO;


//**************************
// ���[�V�������\����
//**************************
typedef struct
{
	bool bLoop;					// ���[�v���邩�ǂ���
	int nNumkey;				// �L�[�̑���
	KEY_INFO aKeyInfo[MAX_KEY];	// �L�[���
	int nStartFrame[NUM_FRAME], nEndFrame[NUM_FRAME];	// �����t���[��
	int nNumFrame;	// �����t���[���̐�
}MOTION_INFO;

//**************************
// ���[�V�����\����
//**************************
typedef struct
{
	MOTIONTYPE motionType;						// ���[�V�����̎��
	bool bLoopMotion;							// ���[�v���邩�ǂ���
	int nNumKey;								// �L�[�̑���
	int nKey;									// ���݂̃L�[No.
	int nCountMotion;							// ���[�V�����̃J�E���^�[
	int nextKey;                                // ���̃L�[
	int nAllFrame;								// �t���[���̍��v
	int nAllCounterMotion;						// ���v�t���[���̃J�E���g

	MOTIONTYPE motiontypeBlend;					// �u�����h�̎��
	bool bBlendMotion;							// �u�����h�����邩�ǂ���
	bool bLoopMotionBlend;						// ���[�v���邩�ǂ���
	bool bFinishMotion;							// ���[�V�������I��������ǂ���
	bool bFirstMotion;                          // ���[�V�������n�܂����t���O
	int nNumKeyBlend;							// �u�����h���[�V�����̍ő�̃L�[
	int nKeyBlend;								// �u�����h���[�V�����̌��݂̃L�[
	int nNextKeyBlend;							// �u�����h���[�V�����̎��̃L�[
	int nCounterMotionBlend;					// �u�����h�̃J�E���^�[

	int nFrameBlend;							// �u�����h�̃t���[����
	int nCounterBlend;							// �u�����h�J�E���^�[
}Motion_Data;

//**************************
// ���[�V�����\����
//**************************
typedef struct
{
	MODEL aModel[MAX_PARTS];					// ���f���̏��
	int nNumModel;								// ���f��(�p�[�c)�̑���(���ۂɎg�����f���̐�)
	MOTION_INFO aMotionInfo[MAX_MOTION];		// ���[�V�������
	int nNumMotion;								// ���[�V�����̑���
	Motion_Data MotionData[MOTION_PART_MAX];	// �S�g�A�㔼�g�A�����g
}MOTION;

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void UpdateMotion(MOTION* pMotion, bool bFullBody);			// �S�g�̃��[�V�����̍X�V����

void SetMotion(MOTION* pMotion, MOTIONTYPE motiontypeBlend, bool Blend, int nFrameBlend,int part);
void NormalizeRotation(float *pRotX, float* pRotY, float* pRotZ);
bool CheckActionMotion(MOTION* pMotion);																	 // ���[�V�������A�N�V����������Ȃ���
bool CheckMotionBounds(MOTION* pMotion, int nStartFrame, int nEndFrame, int nPart);

#endif

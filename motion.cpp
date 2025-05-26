//==============================================================================================================
//
// ���[�V���� [motion.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "motion.h"
#include"meshfield.h"
#include <stdio.h>
#include "player.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define MAX_WORD (128) // �ő�̕�����

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void UpdateCurrentMotion(MOTION* pMotion,int nCntModel,Motion_Data *pMotionData);					// �S�g�̌��݂̃��[�V�����̍X�V����
void UpdateBlendMotion(MOTION* pMotion, int nCntModel, int nextKey, Motion_Data* pMotionData);		// �S�g�̃u�����h�̃��[�V�����̍X�V����
bool EndMotion(MOTION* pMotion, Motion_Data* pMotionData,int nFrameBlend);							// ���[�V�������I��������ǂ���		

//=========================================================================================================
// ���[�V�����̏�����
//=========================================================================================================

//=========================================================================================================
// ���[�V�����̍X�V����
//=========================================================================================================
void UpdateMotion(MOTION *pMotion,bool bFullBody)
{
	for (int nCnt = 0; nCnt < MOTION_PART_MAX; nCnt++)
	{
		// �S�g�̃��[�V�����f�[�^�̃A�h���X����
		Motion_Data* pMotionData = &pMotion->MotionData[nCnt];

		// ���f���̐�����
		for (int nCntModel = 0; nCntModel < pMotion->nNumModel; nCntModel++)
		{
			if (bFullBody == true && nCnt != MOTION_PART_FULLBODY) return;

			// �㔼�g�̎��ɉ����g�̃��f����������
			if (nCnt == MOTION_PART_UPPERBODY && pMotion->aModel[nCntModel].bUpperBody == false) continue;

			// �����g�̎��ɏ㔼�g�̃��f����������
			if (nCnt == MOTION_PART_LOWERBODY && pMotion->aModel[nCntModel].bUpperBody == true) continue;

			if (pMotion->aMotionInfo[pMotionData->motionType].nNumkey != NULL)
			{
				// ���̃L�[
				pMotionData->nextKey = (pMotionData->nKey + 1) % pMotion->aMotionInfo[pMotionData->motionType].nNumkey;
			}
			else if (pMotion->aMotionInfo[pMotionData->motiontypeBlend].nNumkey != NULL)
			{
				// �u�����h���[�V�����̎��̃L�[
				pMotionData->nNextKeyBlend = (pMotionData->nKeyBlend + 1) % pMotion->aMotionInfo[pMotionData->motiontypeBlend].nNumkey;
			}

			// ���[�V�����u�����h���I��������
			if (pMotionData->bFinishMotion == false)
			{
				// ���݂̃��[�V�����̍X�V����
				UpdateCurrentMotion(pMotion, nCntModel, pMotionData);
			}

			if ((pMotionData->bFinishMotion == true || pMotionData->bFirstMotion == true) && pMotionData->bBlendMotion == true)
			{
				// �u�����h���[�V�����̍X�V����
				UpdateBlendMotion(pMotion, nCntModel, pMotionData->nextKey, pMotionData);
			}

			// �I�t�Z�b�g���l�����������ݒ�
			pMotion->aModel[nCntModel].rot += pMotion->aModel[nCntModel].offrot;

			// �I�t�Z�b�g���l�������ʒu�ݒ�
			pMotion->aModel[nCntModel].pos += pMotion->aModel[nCntModel].offpos;
		}


		int LastKey = pMotion->aMotionInfo[pMotionData->motionType].nNumkey - 1;

		int nBlendFrame = pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[LastKey].nFrame;

		// ���[�V�������I�������
		if (pMotion->aMotionInfo[pMotionData->motionType].bLoop == false && pMotionData->nKey >= pMotion->aMotionInfo[pMotionData->motionType].nNumkey - 1 &&
			pMotionData->bBlendMotion == true && pMotionData->bFinishMotion == false && pMotionData->bFirstMotion == false)
		{
			pMotionData->nCounterBlend = 0;
			pMotionData->nFrameBlend = nBlendFrame;
			pMotionData->bFinishMotion = true;
			pMotionData->motiontypeBlend = MOTIONTYPE_NEUTRAL; // ���[�V�����^�C�v���j���[�g�����ɂ���
		}

		// ���[�V�����̏o�����̃u�����h���I������
		if (pMotionData->bFirstMotion == true && pMotionData->nCounterBlend >= pMotionData->nFrameBlend)
		{
			// ���Ƃɖ߂�
			pMotionData->bFirstMotion = false;

			// ���[�V�������u�����h�������[�V�����ɂ���
			pMotionData->motionType = pMotionData->motiontypeBlend;

			// 0�Ԗڂ���n�߂�
			pMotionData->nKey = 0;

			// �u�����h�����t���[������J�n
			pMotionData->nCountMotion = 0;
		}

		// ���[�V�������I��邩�L�[���ő傩�u�����h�̃J�E���g���ő�ɂȂ���
		if (pMotionData->nCounterBlend >= nBlendFrame && pMotionData->bBlendMotion == true && pMotionData->bFinishMotion == true && pMotionData->bFirstMotion == false)
		{
			pMotionData->bFinishMotion = false;           // ���Ƃɖ߂�
			pMotionData->bBlendMotion = false;			  // ���Ƃɖ߂�
			pMotionData->motionType = MOTIONTYPE_NEUTRAL; // ���[�V�����^�C�v���j���[�g�����ɂ���
		}

		// �u�����h���Ȃ�
		if (!pMotionData->bBlendMotion == true && pMotionData->bFinishMotion == true)
		{
			pMotionData->motionType = MOTIONTYPE_NEUTRAL;
		}

		// ���[�V�����J�E���g�̐ݒ�
		if (pMotionData->nCountMotion >= pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[pMotionData->nKey].nFrame)
		{
			// ��񂪂Ȃ�������
			if (pMotion->aMotionInfo[pMotionData->motionType].nNumkey != NULL)
			{
				//���[�V�����J�E���g���ő�ɂȂ�����0�ɖ߂�
				pMotionData->nKey = (pMotionData->nKey + 1) % pMotion->aMotionInfo[pMotionData->motionType].nNumkey;
			}
			else
			{

			}
			pMotionData->nCountMotion = 0;

		}

		int Frame = 0;

		// �L�[�̑�������
		for (int nCntKey = 0; nCntKey < pMotion->aMotionInfo[pMotionData->motiontypeBlend].nNumkey; nCntKey++)
		{
			Frame += pMotion->aMotionInfo[pMotionData->motiontypeBlend].aKeyInfo[nCntKey].nFrame;

			// �t���[���̍��v�𒲂ׂ�
			pMotionData->nAllFrame = Frame;
		}


		// ���v�t���[���𒴂�����
		if (pMotionData->nAllCounterMotion >= pMotionData->nAllFrame)
		{
			// �J�E���g�����Z�b�g
			pMotionData->nAllCounterMotion = 0;
		}

		// ���[�V�����J�E���^�[�̍X�V
		if (pMotionData->bFirstMotion == false)
		{
			// ���v�t���[���̃J�E���g
			pMotionData->nAllCounterMotion++;

			//���[�V�����J�E���g�����Z
			pMotionData->nCountMotion++;
		}

		// �u�����h���n�܂�����
		if (pMotionData->bFinishMotion == true || pMotionData->bFirstMotion == true)
		{
			pMotionData->nCounterBlend++; // �u�����h���[�V�����J�E���g���C���N�������g
		}
	}
	//// ���[�V���������S��������
	//if (pMotion->motiontypeBlend == MOTIONTYPE_DEATH && pMotion->nKey >= pMotion->aMotionInfo[pMotion->motionType].nNumkey - 1)
	//{
	//	pMotion->nCountMotion--;
	//	pMotion->nCounterBlend--;
	//}

}
//================================================================================================================
// ���[�V�����̐ݒ菈��
//================================================================================================================
void SetMotion(MOTION* pMotion, MOTIONTYPE motiontypeBlend, bool Blend, int nFrameBlend,int part)
{
	Motion_Data* pMotionData = &pMotion->MotionData[part];

	// ���[�V�����̏�񂪂Ȃ�������
	if (pMotion->aMotionInfo[pMotionData->motiontypeBlend].nNumkey == 0) return;

	if (pMotion->MotionData[part].motiontypeBlend == motiontypeBlend) return;

	// �ŏ��̃u�����h
	// ���[�V�����u�����h�����邩�ǂ���
	if (Blend == true)
	{
		// �ŏ��̃��[�V�����u�����h���I����Ă���
		if (pMotionData->bFirstMotion == false)
		{
			pMotionData->bFirstMotion = true;
			pMotionData->nKeyBlend = 0;						// 0����n�߂�
			pMotionData->nCounterBlend = 0;					// 0����n�߂�
		}
		pMotionData->bBlendMotion = Blend;					// �u�����h�����邩�ǂ���
		pMotionData->nFrameBlend = nFrameBlend;				// �u�����h�̃t���[������
		pMotionData->motiontypeBlend = motiontypeBlend;		// �u�����h���郂�[�V�����̃^�C�v����
		pMotionData->bFinishMotion = false;
	}
	// ���[�V�����u�����h���Ȃ�
	else
	{
		pMotionData->bBlendMotion = Blend;					// �u�����h�����邩�ǂ���
		pMotionData->motionType = motiontypeBlend;				// �u�����h���郂�[�V�����̃^�C�v����
		pMotionData->motiontypeBlend = motiontypeBlend;			// �u�����h���郂�[�V�����̃^�C�v����
		pMotionData->bFinishMotion = false;
	}

	pMotionData->nAllCounterMotion = 0;
}
//================================================================================================================
// �p�x�̐��K��
//================================================================================================================
void NormalizeRotation(float* pRotX, float* pRotY, float* pRotZ)
{
	// �p�x�̐��K��
	if (*pRotX > D3DX_PI)
	{
		*pRotX += -D3DX_PI * 2.0f;
	}
	else if (*pRotX < -D3DX_PI)
	{
		*pRotX += D3DX_PI * 2.0f;
	}

	if (*pRotY > D3DX_PI)
	{
		*pRotY += -D3DX_PI * 2.0f;
	}
	else if (*pRotY < -D3DX_PI)
	{
		*pRotY += D3DX_PI * 2.0f;
	}

	if (*pRotZ > D3DX_PI)
	{
		*pRotZ += -D3DX_PI * 2.0f;
	}
	else if (*pRotZ < -D3DX_PI)
	{
		*pRotZ += D3DX_PI * 2.0f;
	}
}
//================================================================================================================
// �A�N�V�������[�V��������Ȃ���
//================================================================================================================
bool CheckActionMotion(MOTION *pMotion)
{
	//bool bFrag = false;
	//// ���[�V�������U������Ȃ�������
	//if (pMotion->motiontypeBlend != MOTIONTYPE_ACTION &&
	//	pMotion->motiontypeBlend != MOTIONTYPE_ACTION2 &&
	//	pMotion->motiontypeBlend != MOTIONTYPE_ACTION3 &&
	//	pMotion->motiontypeBlend != MOTIONTYPE_ACTION4)
	//{
	//	bFrag = true;
	//}
	return false;
}
//==============================================================================================================
// �S�g�̃��[�V�����̍X�V����
//==============================================================================================================
void UpdateFullBodyMotion()
{
}
//================================================================================================================
// ���݂̃��[�V�����̍X�V����
//================================================================================================================
void UpdateCurrentMotion(MOTION* pMotion, int nCntModel, Motion_Data* pMotionData)
{
	// ���݂̃L�[�̃A�h���X���擾
	KEY* pCurrentKey = &pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[pMotionData->nKey].aKey[nCntModel];

	// ���̃L�[
	int nextKey = pMotionData->nextKey;

	//���̃L�[�̃A�h���X���擾
	KEY* pNextKey = &pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[nextKey].aKey[nCntModel];

	// �ʒu�̍����A�����̍���
	float DiffPosX = pNextKey->fPosX - pCurrentKey->fPosX;
	float DiffPosY = pNextKey->fPosY - pCurrentKey->fPosY;
	float DiffPosZ = pNextKey->fPosZ - pCurrentKey->fPosZ;
	float DiffRotX = pNextKey->fRotX - pCurrentKey->fRotX;
	float DiffRotY = pNextKey->fRotY - pCurrentKey->fRotY;
	float DiffRotZ = pNextKey->fRotZ - pCurrentKey->fRotZ;

	// �p�x�̐��K��
	if (DiffRotX > D3DX_PI)
	{
		DiffRotX += -D3DX_PI * 2.0f;
	}
	else if (DiffRotX < -D3DX_PI)
	{
		DiffRotX += D3DX_PI * 2.0f;
	}

	if (DiffRotY > D3DX_PI)
	{
		DiffRotY += -D3DX_PI * 2.0f;
	}
	else if (DiffRotY < -D3DX_PI)
	{
		DiffRotY += D3DX_PI * 2.0f;
	}

	if (DiffRotZ > D3DX_PI)
	{
		DiffRotZ += -D3DX_PI * 2.0f;
	}
	else if (DiffRotZ < -D3DX_PI)
	{
		DiffRotZ += D3DX_PI * 2.0f;
	}

	int nFrame = pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[pMotionData->nKey].nFrame;

	// �t���[���̑��Βl
	float fRateMotion = (float)pMotionData->nCountMotion / (float)nFrame;

	// �ʒu�����߂�
	float fPosX = pCurrentKey->fPosX + DiffPosX * fRateMotion;
	float fPosY = pCurrentKey->fPosY + DiffPosY * fRateMotion;
	float fPosZ = pCurrentKey->fPosZ + DiffPosZ * fRateMotion;

	// ���������߂�
	float fRotX = pCurrentKey->fRotX + DiffRotX * fRateMotion;
	float fRotY = pCurrentKey->fRotY + DiffRotY * fRateMotion;
	float fRotZ = pCurrentKey->fRotZ + DiffRotZ * fRateMotion;

	// ���݂̈ʒu��ݒ�
	pMotion->aModel[nCntModel].pos = D3DXVECTOR3(fPosX, fPosY, fPosZ);

	// ������ݒ�
	pMotion->aModel[nCntModel].rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);
}
//================================================================================================================
// �u�����h���[�V�����̍X�V����
//================================================================================================================
void UpdateBlendMotion(MOTION* pMotion, int nCntModel, int nextKey, Motion_Data* pMotionData)
{
	// �Đ��t���[��
	int nFrame = pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[pMotionData->nKey].nFrame;

	float fRateMotion = (float)pMotionData->nCountMotion / (float)nFrame; // ���Βl
	float fRateBlend = (float)pMotionData->nCounterBlend / (float)pMotionData->nFrameBlend;
	
	// ���݂̃L�[�̃A�h���X���擾
	KEY* pCurrentKey = &pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[pMotionData->nKey].aKey[nCntModel];

	//���̃L�[�̃A�h���X���擾
	KEY* pNextKey = &pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[nextKey].aKey[nCntModel];


	//���݂̃��[�V�����̉�]X
	float fDiffMotionRX = pNextKey->fRotX - pCurrentKey->fRotX;

	//���݂̃��[�V�����̉�]Y
	float fDiffMotionRY = pNextKey->fRotY - pCurrentKey->fRotY;

	//���݂̃��[�V�����̉�]Z
	float fDiffMotionRZ = pNextKey->fRotZ - pCurrentKey->fRotZ;

	// ����X
	float fRotXCurrent = pCurrentKey->fRotX + (fDiffMotionRX * fRateMotion);

	// ����Y
	float fRotYCurrent = pCurrentKey->fRotY + (fDiffMotionRY * fRateMotion);

	// ����Z
	float fRotZCurrent = pCurrentKey->fRotZ + (fDiffMotionRZ * fRateMotion);

	// �p�x�̐��K��
	if (fDiffMotionRX > D3DX_PI)
	{
		fDiffMotionRX += -D3DX_PI * 2.0f;
	}
	else if (fDiffMotionRX < -D3DX_PI)
	{
		fDiffMotionRX += D3DX_PI * 2.0f;
	}

	if (fDiffMotionRY > D3DX_PI)
	{
		fDiffMotionRY += -D3DX_PI * 2.0f;
	}
	else if (fDiffMotionRY < -D3DX_PI)
	{
		fDiffMotionRY += D3DX_PI * 2.0f;
	}

	if (fDiffMotionRZ > D3DX_PI)
	{
		fDiffMotionRZ += -D3DX_PI * 2.0f;
	}
	else if (fDiffMotionRZ < -D3DX_PI)
	{
		fDiffMotionRZ += D3DX_PI * 2.0f;
	}

	// �u�����h�̃L�[�̃A�h���X���擾
	KEY* pCurrentBlendKey = &pMotion->aMotionInfo[pMotionData->motiontypeBlend].aKeyInfo[pMotionData->nKeyBlend].aKey[nCntModel];

	// �u�����h�̎��̃L�[�̃A�h���X���擾
	KEY* pNextBlendKey = &pMotion->aMotionInfo[pMotionData->motiontypeBlend].aKeyInfo[pMotionData->nNextKeyBlend].aKey[nCntModel];

	//�u�����h���[�V�����̉�]X
	float fDiffMotionBlendRX = pNextBlendKey->fRotX - pCurrentBlendKey->fRotX;

	//�u�����h���[�V�����̉�]Y
	float fDiffMotionBlendRY = pNextBlendKey->fRotY - pCurrentBlendKey->fRotY;

	//�u�����h���[�V�����̉�]Z
	float fDiffMotionBlendRZ = pNextBlendKey->fRotZ - pCurrentBlendKey->fRotZ;

	// �p�x�̐��K��
	if (fDiffMotionBlendRX > D3DX_PI)
	{
		fDiffMotionBlendRX += -D3DX_PI * 2.0f;
	}
	else if (fDiffMotionBlendRX < -D3DX_PI)
	{
		fDiffMotionBlendRX += D3DX_PI * 2.0f;
	}

	if (fDiffMotionBlendRY > D3DX_PI)
	{
		fDiffMotionBlendRY += -D3DX_PI * 2.0f;
	}
	else if (fDiffMotionBlendRY < -D3DX_PI)
	{
		fDiffMotionBlendRY += D3DX_PI * 2.0f;
	}

	if (fDiffMotionBlendRZ > D3DX_PI)
	{
		fDiffMotionBlendRZ += -D3DX_PI * 2.0f;
	}
	else if (fDiffMotionBlendRZ < -D3DX_PI)
	{
		fDiffMotionBlendRZ += D3DX_PI * 2.0f;
	}

	// �u�����h���[�V�����̉�]X
	float fRotXBlend = pCurrentBlendKey->fRotX + (fDiffMotionBlendRX * fRateBlend);

	// �u�����h���[�V�����̉�]Y
	float fRotYBlend = pCurrentBlendKey->fRotY + (fDiffMotionBlendRY * fRateBlend);

	// �u�����h���[�V�����̉�]Z
	float fRotZBlend = pCurrentBlendKey->fRotZ + (fDiffMotionBlendRZ * fRateBlend);


	//���݂̃��[�V�����̈ʒuX
	float fDiffMotionPX = pNextKey->fPosX - pCurrentKey->fPosX;

	// ���݂̈ʒu�̐ݒ�
	float fPosXCurrent = pCurrentKey->fPosX + (fDiffMotionPX * fRateMotion);

	//���݂̃��[�V�����̈ʒuY
	float fDiffMotionPY = pNextKey->fPosY - pCurrentKey->fPosY;

	// ���݂̈ʒu�̐ݒ�
	float fPosYCurrent = pCurrentKey->fPosY + (fDiffMotionPY * fRateMotion);

	//���݂̃��[�V�����̈ʒuZ
	float fDiffMotionPZ = pNextKey->fPosZ - pCurrentKey->fPosZ;

	// ���݂̈ʒu�̐ݒ�
	float fPosZCurrent = pCurrentKey->fPosZ + (fDiffMotionPZ * fRateMotion);

	//�u�����h���[�V�����̈ʒuX
	float fDiffMotionBlendPX = pNextBlendKey->fPosX - pCurrentBlendKey->fPosX;

	float fPosXBlend = pCurrentBlendKey->fPosX + (fDiffMotionBlendPX * fRateBlend);

	//�u�����h���[�V�����̈ʒuY
	float fDiffMotionBlendPY = pNextBlendKey->fPosY - pCurrentBlendKey->fPosY;

	float fPosYBlend = pCurrentBlendKey->fPosY + (fDiffMotionBlendPY * fRateBlend);

	//�u�����h���[�V�����̈ʒuZ
	float fDiffMotionBlendPZ = pNextBlendKey->fPosZ - pCurrentBlendKey->fPosZ;

	float fPosZBlend = pCurrentBlendKey->fPosZ + (fDiffMotionBlendPZ * fRateBlend);

	float fDiffBlendRX = fRotXBlend - fRotXCurrent; // ����X
	float fDiffBlendRY = fRotYBlend - fRotYCurrent; // ����Y
	float fDiffBlendRZ = fRotZBlend - fRotZCurrent; // ����Z

	float fRotX = fRotXCurrent + (fDiffBlendRX * fRateBlend); // ���߂�l
	float fRotY = fRotYCurrent + (fDiffBlendRY * fRateBlend); // ���߂�l
	float fRotZ = fRotZCurrent + (fDiffBlendRZ * fRateBlend); // ���߂�l

	float fDiffBlendPX = fPosXBlend - fPosXCurrent; // ����X
	float fDiffBlendPY = fPosYBlend - fPosYCurrent; // ����Y
	float fDiffBlendPZ = fPosZBlend - fPosZCurrent; // ����Z

	float fPosX = fPosXCurrent + (fDiffBlendPX * fRateBlend); // ���߂�l
	float fPosY = fPosYCurrent + (fDiffBlendPY * fRateBlend); // ���߂�l
	float fPosZ = fPosZCurrent + (fDiffBlendPZ * fRateBlend); // ���߂�l

	// �ʒu��ݒ�
	pMotion->aModel[nCntModel].pos = D3DXVECTOR3(fPosX, fPosY, fPosZ);

	// ������ݒ�
	pMotion->aModel[nCntModel].rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);
}
//===============================================================================================================
// ���[�V�������I��������ǂ���		
//===============================================================================================================
bool EndMotion(MOTION* pMotion, Motion_Data* pMotionData, int nFrameBlend)
{
	return false;
}
//===============================================================================================================
// ���[�V�����̔���ݒ菈��
//===============================================================================================================
bool CheckMotionBounds(MOTION* pMotion, int nStartFrame, int nEndFrame, int nPart)
{
	// ����p�ϐ�
	bool bFlag = false;

	// ���[�V�����̃f�[�^�̍\���̂̃A�h���X���擾
	Motion_Data* pMotionData = &pMotion->MotionData[nPart];

	if (pMotionData->bFirstMotion == true) return false;
	if (pMotionData->bFinishMotion == true) return false;

	// ����J�n�̃t���[��
	if (pMotionData->nAllCounterMotion >= nStartFrame && pMotionData->nAllCounterMotion <= nEndFrame)
	{
		// ����J�n
		bFlag = true;
	}
	// �����Ԃ�
	return bFlag;
}

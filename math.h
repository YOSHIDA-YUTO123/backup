//==============================================================================================================
//
// math [math.h]
// Author: YOSHIDA YUTO
//
//==============================================================================================================
#ifndef _MATH_H_
#define _MATH_H_

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include"main.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define INIT_VECTOR3 (D3DXVECTOR3(0.0f,0.0f,0.0f)) // �������p�ϐ�

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
float SetSmoothAprroach(float fDestValue, float Value, float coefficient);
bool sphererange(D3DXVECTOR3* pPos1, D3DXVECTOR3* pPos2, float radius1, float radius2); // �~�̓����蔻��
void SetTextureAnimation(int pPosX, int pPosY, int pAnimSpeed, int* pCounterAnim, int* pPatternAnim, VERTEX_2D* pVtx, LPDIRECT3DVERTEXBUFFER9 pVtxBuff, int nCnt);
D3DXVECTOR3 SetMtxConversion(D3DXMATRIX mtxworld); // ���[���h�}�g���b�N�X��D3DXVECTOR3�ɕϊ�����
D3DXVECTOR3 SetSubtractVectors(D3DXVECTOR3 targetpos, D3DXVECTOR3 pos); // �ڕW�܂ł̃x�N�g�����v�Z����
float GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2); // �������v�Z����֐�
int FindNearEnemy(D3DXVECTOR3 *enemies, int nMaxEnemies, bool* bUse);
bool CheckLongPress(int time, int nKey);	// ���������Ĕ�������܂ł̎���
bool ShowSaveFileDialog(char* filePath, size_t maxLength);	// �t�@�C���ۑ����_�C�A���O�\������
bool ShowOpenFileDialog(char* filePath, size_t max_length);	// �t�@�C�����J��
bool CollisionCapsule(D3DXVECTOR3 Start1, D3DXVECTOR3 End1, D3DXVECTOR3 Start2, D3DXVECTOR3 End2, float radius1, float radius2);
float LineDistance(D3DXVECTOR3 Start, D3DXVECTOR3 End, D3DXVECTOR3 Point);
bool CollisionRayToSphere(D3DXVECTOR3 SenterPos, D3DXVECTOR3* pMove, D3DXVECTOR3* DragPos, float fRadius);
D3DXVECTOR3 ComputeThrowVelocity(const D3DXVECTOR3 dest, const D3DXVECTOR3 start, const float gravity, const float time);
#endif
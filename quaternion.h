//==========================
//
// �N�E�H�[�^��I�� [quaternion.h]
// Author: TEAM_C
//
//==========================
#ifndef _QUATERNION_H_
#define _QUATERNION_H_

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "main.h"

//****************************
// �v���g�^�C�v�錾
//****************************
D3DXQUATERNION CreateQuaternion(D3DXQUATERNION *pOut,D3DXVECTOR3 axis, float fAngle);

D3DXMATRIX SetMatrixRotationQuaternion(D3DXMATRIX *pOut, D3DXQUATERNION quaternion);
#endif

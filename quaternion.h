//==========================
//
// クウォータ二オン [quaternion.h]
// Author: TEAM_C
//
//==========================
#ifndef _QUATERNION_H_
#define _QUATERNION_H_

//****************************
// インクルードファイル宣言
//****************************
#include "main.h"

//****************************
// プロトタイプ宣言
//****************************
D3DXQUATERNION CreateQuaternion(D3DXQUATERNION *pOut,D3DXVECTOR3 axis, float fAngle);

D3DXMATRIX SetMatrixRotationQuaternion(D3DXMATRIX *pOut, D3DXQUATERNION quaternion);
#endif

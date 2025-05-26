//==========================
//
// クウォータ二オン [aQuaternion.cpp]
// Author: YUTO YOSHIDA
//
//==========================

//****************************
// インクルードファイル宣言
//****************************
#include "quaternion.h"

//==========================
// クォータニオンの作成処理
//==========================
D3DXQUATERNION CreateQuaternion(D3DXQUATERNION* pOut, D3DXVECTOR3 axis, float fAngle)
{
	// クウォータ二オン
	D3DXQUATERNION aQuaternion;

	// 角度を半分にする(クウォータ二オンは半分の角度をもとに計算するため)
	float angle = fAngle / 2.0f;

	D3DXVECTOR3 NormalizeAxis;

	// 軸を正規化する
	D3DXVec3Normalize(&NormalizeAxis, &axis);

	// クウォータ二オンの計算
	aQuaternion.w = cosf(angle);
	aQuaternion.x = sinf(angle) * NormalizeAxis.x;
	aQuaternion.y = sinf(angle) * NormalizeAxis.y;
	aQuaternion.z = sinf(angle) * NormalizeAxis.z;

	// 計算結果を入れる
	*pOut = aQuaternion;

	return aQuaternion;
}

//==========================
// クウォータ二オンの設定
//==========================
D3DXMATRIX SetMatrixRotationQuaternion(D3DXMATRIX* pOut, D3DXQUATERNION quaternion)
{
	// 計算用ワールドマトリックス
	D3DXMATRIX outMatrix;

	// 行列を初期化さ
	D3DXMatrixIdentity(&outMatrix);

	// 角度の2乗
	float sqX = quaternion.x * quaternion.x;
	float sqY = quaternion.y * quaternion.y;
	float sqZ = quaternion.z * quaternion.z;
	float sqW = quaternion.w * quaternion.w;

	// 行列_11 _21 _31の計算
	outMatrix._11 = 1 - (2 * (sqY + sqZ));
	outMatrix._21 = (2 * ((quaternion.x * quaternion.y) + (quaternion.w * quaternion.z)));
	outMatrix._31 = (2 * ((quaternion.x * quaternion.z) + (quaternion.w * quaternion.y)));

	// 行列_12 _22 _32の計算
	outMatrix._12 = 2 * ((quaternion.x * quaternion.y) - (quaternion.w * quaternion.z));
	outMatrix._22 = 1 - (2 * (sqX + sqZ));
	outMatrix._32 = (2 * ((quaternion.y * quaternion.z) + (quaternion.w * quaternion.x)));

	// 行列_13 _23 _33の計算
	outMatrix._13 = 2 * ((quaternion.x * quaternion.z) - (quaternion.w * quaternion.y));
	outMatrix._23 = 2 * ((quaternion.y * quaternion.z) - (quaternion.w * quaternion.x));
	outMatrix._33 = 1 - (2 * (sqX + sqY));

	outMatrix._14 = 0.0f;
	outMatrix._24 = 0.0f;
	outMatrix._34 = 0.0f;

	outMatrix._41 = 0.0f;
	outMatrix._42 = 0.0f;
	outMatrix._43 = 0.0f;
	outMatrix._44 = 1.0f;

	// 計算結果を代入
	*pOut = outMatrix;

	// 計算したマトリックスを返す
	return outMatrix;
}

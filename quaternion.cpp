//==========================
//
// �N�E�H�[�^��I�� [aQuaternion.cpp]
// Author: YUTO YOSHIDA
//
//==========================

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "quaternion.h"

//==========================
// �N�H�[�^�j�I���̍쐬����
//==========================
D3DXQUATERNION CreateQuaternion(D3DXQUATERNION* pOut, D3DXVECTOR3 axis, float fAngle)
{
	// �N�E�H�[�^��I��
	D3DXQUATERNION aQuaternion;

	// �p�x�𔼕��ɂ���(�N�E�H�[�^��I���͔����̊p�x�����ƂɌv�Z���邽��)
	float angle = fAngle / 2.0f;

	D3DXVECTOR3 NormalizeAxis;

	// ���𐳋K������
	D3DXVec3Normalize(&NormalizeAxis, &axis);

	// �N�E�H�[�^��I���̌v�Z
	aQuaternion.w = cosf(angle);
	aQuaternion.x = sinf(angle) * NormalizeAxis.x;
	aQuaternion.y = sinf(angle) * NormalizeAxis.y;
	aQuaternion.z = sinf(angle) * NormalizeAxis.z;

	// �v�Z���ʂ�����
	*pOut = aQuaternion;

	return aQuaternion;
}

//==========================
// �N�E�H�[�^��I���̐ݒ�
//==========================
D3DXMATRIX SetMatrixRotationQuaternion(D3DXMATRIX* pOut, D3DXQUATERNION quaternion)
{
	// �v�Z�p���[���h�}�g���b�N�X
	D3DXMATRIX outMatrix;

	// �s�����������
	D3DXMatrixIdentity(&outMatrix);

	// �p�x��2��
	float sqX = quaternion.x * quaternion.x;
	float sqY = quaternion.y * quaternion.y;
	float sqZ = quaternion.z * quaternion.z;
	float sqW = quaternion.w * quaternion.w;

	// �s��_11 _21 _31�̌v�Z
	outMatrix._11 = 1 - (2 * (sqY + sqZ));
	outMatrix._21 = (2 * ((quaternion.x * quaternion.y) + (quaternion.w * quaternion.z)));
	outMatrix._31 = (2 * ((quaternion.x * quaternion.z) + (quaternion.w * quaternion.y)));

	// �s��_12 _22 _32�̌v�Z
	outMatrix._12 = 2 * ((quaternion.x * quaternion.y) - (quaternion.w * quaternion.z));
	outMatrix._22 = 1 - (2 * (sqX + sqZ));
	outMatrix._32 = (2 * ((quaternion.y * quaternion.z) + (quaternion.w * quaternion.x)));

	// �s��_13 _23 _33�̌v�Z
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

	// �v�Z���ʂ���
	*pOut = outMatrix;

	// �v�Z�����}�g���b�N�X��Ԃ�
	return outMatrix;
}

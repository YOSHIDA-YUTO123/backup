//==============================================================================================================
//
// math [math.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

//*******************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//*******************************************************************************************************************
#include "math.h"
#include "player.h"
#include"input.h"

//==============================================================================================================
// ���񂾂�ڕW�̒l�ɋ߂Â���֐�
//==============================================================================================================
float SetSmoothAprroach(float fDestValue, float Value, float coefficient)
{
	// ���񂾂�ڕW�̒l�ɋ߂Â���
	float OutValue = (fDestValue - Value) * coefficient;
	return OutValue;
}
//===============================================================================================================
// �~�̔���
//===============================================================================================================
bool sphererange(D3DXVECTOR3* pPos1, D3DXVECTOR3* pPos2, float radius1, float radius2) // �~�̓����蔻��
{
	bool bRange = false;

	D3DXVECTOR3 DiffPos; // �v�Z�p

	// ����XYZ�����߂�
	DiffPos.x = pPos1->x - pPos2->x;
	DiffPos.y = pPos1->y - pPos2->y;
	DiffPos.z = pPos1->z - pPos2->z;

	// ���������߂�
	float fDistance = (DiffPos.x * DiffPos.x) + (DiffPos.y * DiffPos.y) + (DiffPos.z * DiffPos.z);

	// ���a�����߂�
	float fRadius = radius1 + radius2;

	// ���a�����߂�
	fRadius = fRadius * fRadius;

	if (fDistance <= fRadius)
	{
		bRange = true;
	}
	return bRange;
}
//=====================================================================================================
// ���[���h�}�g���b�N�X��D3DXVECTOR3�ɕϊ�����
//=====================================================================================================
D3DXVECTOR3 SetMtxConversion(D3DXMATRIX mtxworld)
{
	// XYZ�ɕϊ�
	D3DXVECTOR3 OutPut(mtxworld._41, mtxworld._42, mtxworld._43);

	// �ϊ������l��Ԃ�
	return OutPut;
}
//=====================================================================================================
// �ڕW�܂ł̃x�N�g�����v�Z����
//=====================================================================================================
D3DXVECTOR3 SetSubtractVectors(D3DXVECTOR3 targetpos, D3DXVECTOR3 pos)
{
	D3DXVECTOR3 OutPut = targetpos - pos;

	return OutPut;
}
//==============================================================================================================
// �������v�Z����֐�
//==============================================================================================================
float GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	// ���������߂�
	D3DXVECTOR3 DiffPos = pos1 - pos2;

	// �������v�Z
	float OutDistance = sqrtf((DiffPos.x * DiffPos.x) + (DiffPos.y * DiffPos.y) + (DiffPos.z * DiffPos.z));

	// ���߂�������Ԃ�
	return OutDistance;
}
//==============================================================================================================
// �����΂�߂��G�����߂�
//==============================================================================================================
int FindNearEnemy(D3DXVECTOR3* enemies, int nMaxEnemies,bool *bUse)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �߂��G�̃C���f�b�N�X
	int nIdx = -1;

	// ����
	float fDistance = 0.0f;

	// �����ۑ��p�ϐ�
	float fDistanceStock = FLT_MAX;

	// �G�̍ő吔����
	for (int nCnt = 0; nCnt < nMaxEnemies; nCnt++, enemies++, bUse++)
	{
		// �����Ă���Ȃ�
		if (*bUse == false) continue;

		// ���������߂�
		fDistance = GetDistance(pPlayer->pos, *enemies);

		// �������ۑ����Ă��������Z��������
		if (fDistance < fDistanceStock)
		{
			// ������ۑ�
			fDistanceStock = fDistance;

			// �C���f�b�N�X��ۑ�
			nIdx = nCnt;
		}
	}

	// �C���f�b�N�X��Ԃ�
	return nIdx;
}
//=====================================================================================================
// ���������Ĕ�������܂ł̎���
//=====================================================================================================
bool CheckLongPress(int time, int nKey)
{
	// �����Ă鎞��(�L�[�̐������p��)
	static int nPressTime[256] = { 0 };

	// �����Ԃ��ϐ�
	bool bFlag = false;

	// �L�[�{�[�h�𒷉������Ă�����
	if (GetKeyboardPress(nKey) == true)
	{
		nPressTime[nKey]++;

		if (time <= nPressTime[nKey] || nPressTime[nKey] == 1)
		{
			bFlag = true;
		}
	}
	else
	{
		// �������烊�Z�b�g
		nPressTime[nKey] = 0;
	}
	return bFlag;
}
//=====================================================================================================
// �t�@�C���ۑ����_�C�A���O�\������
//=====================================================================================================
bool ShowSaveFileDialog(char* filePath, size_t maxLength)
{
	OPENFILENAME ofn;	// �_�C�A���O�{�b�N�X�\����

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);

	ofn.hwndOwner = GetActiveWindow();

	ofn.lpstrFile = filePath;

	ofn.nMaxFile = (DWORD)maxLength;
	ofn.lpstrFilter = "�e�L�X�g�t�@�C��\0*.txt\0���ׂẴt�@�C��\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrDefExt = "txt";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	// �_�C�A���O�{�b�N�X��\�����A�ۑ���t�@�C���p�X���擾
	return GetSaveFileName(&ofn) == TRUE;
}
//=====================================================================================================
// �t�@�C���ǂݍ��݂��_�C�A���O�\������
//=====================================================================================================
bool ShowOpenFileDialog(char* filePath, size_t max_length)
{
	OPENFILENAME ofn;    // �_�C�A���O�{�b�N�X�\����
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = (DWORD)max_length;
	ofn.lpstrFilter = "�e�L�X�g�t�@�C��\0*.txt\0���ׂẴt�@�C��\0*.*\0";
	ofn.lpstrFile[0] = '\0';   // ������
	ofn.lpstrDefExt = "txt";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// �_�C�A���O�{�b�N�X��\�����A�I�����ꂽ�t�@�C���p�X���擾
	return GetOpenFileName(&ofn) == TRUE;
}
//=====================================================================================================
// �J�v�Z���ƃJ�v�Z���̓����蔻��
//=====================================================================================================
bool CollisionCapsule(D3DXVECTOR3 Start1,D3DXVECTOR3 End1,D3DXVECTOR3 Start2, D3DXVECTOR3 End2,float radius1,float radius2)
{
	//D3DXVECTOR3 player_head = SetMtxConversion(pPlayer->Motion.aModel[2].mtxWorld);
	//D3DXVECTOR3 Enemy_head = SetMtxConversion(g_Enemy.Motion.aModel[2].mtxWorld);

	D3DXVECTOR3 Player_dir = Start1 - End1;
	D3DXVECTOR3 Enemy_dir = Start2 - End2;

	// �������̎n�_�ԃx�N�g��
	D3DXVECTOR3 head_to_head = End1 - End2;

	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &Player_dir, &Enemy_dir);
	float fcross_squard = D3DXVec3Dot(&Cross, &Cross);

	D3DXVECTOR3 closeset_on_player, closeset_on_enemy;

	float ft_player = 0.0f;
	float ft_enemy = 0.0f;
	float fDistance = 0.0f;

	if (fcross_squard < 0.00001f)
	{
		closeset_on_player = Start1;
		fDistance = LineDistance(Start1,End1, Start2);
	}
	else
	{
		float dot_p1e1 = D3DXVec3Dot(&Player_dir, &Enemy_dir);
		float dot_p1p1 = D3DXVec3Dot(&Player_dir, &Player_dir);
		float dot_e1e1 = D3DXVec3Dot(&Enemy_dir, &Enemy_dir);

		float dot_p1d = D3DXVec3Dot(&Player_dir, &head_to_head);
		float dot_e1d = D3DXVec3Dot(&Enemy_dir, &head_to_head);

		float denom = dot_p1p1 * dot_e1e1 - dot_p1e1 * dot_p1e1;

		ft_player = (dot_p1e1 * dot_e1d - dot_e1e1 * dot_p1d) / denom;
		ft_enemy = (dot_p1p1 * dot_e1d - dot_p1e1 * dot_p1d) / denom;

		ft_player = max(0.0f, min(1.0f, ft_player));
		ft_enemy = max(0.0f, min(1.0f, ft_enemy));

		closeset_on_player = End1 + Player_dir * ft_player;
		closeset_on_enemy = End2 + Enemy_dir * ft_enemy;

		fDistance = GetDistance(closeset_on_player, closeset_on_enemy);
	}

	float fRadius = radius1 + radius2;

	if (fDistance <= fRadius)
	{
		return true;
	}

	return false;
}
//============================================
// �����Ƃ̍ŒZ����
//============================================
float LineDistance(D3DXVECTOR3 Start,D3DXVECTOR3 End,D3DXVECTOR3 Point)
{
	// �x�N�g���̒�����1�Ƃ������̃X�^�[�g����I�_�܂ł̒���
	float fDot = 0.0f;

	D3DXVECTOR3 PlayerLine = Start - End;

	float Linedot = D3DXVec3Dot(&PlayerLine, &PlayerLine);

	if (Linedot > 0.0f)
	{
		D3DXVECTOR3 sp = Point - End;

		fDot = D3DXVec3Dot(&PlayerLine, &sp) / Linedot;
	}

	D3DXVECTOR3 mp = End + PlayerLine * fDot;

	float fDistance = GetDistance(Point, mp);

	// �����̊O��������(�n�_���)
	if (fDot < 0.0f)
	{
		mp = Start;
		fDistance = GetDistance(Point, mp);

		return fDistance;
	}
	else if (fDot > 0.0f)
	{
		mp = End;
		fDistance = GetDistance(Point, mp);

		return fDistance;
	}

	return fDistance;
}
//=====================================================================================================
// �G�t�F�N�g�̃e�N�X�`���A�j���[�V����
//=====================================================================================================
void SetTextureAnimation(int pPosX,int pPosY,int pAnimSpeed,int *pCounterAnim,int *pPatternAnim, VERTEX_2D *pVtx, LPDIRECT3DVERTEXBUFFER9 pVtxBuff,int nCnt)
{
	//���_�o�b�t�@�����b�N
	if (FAILED(pVtxBuff->Lock(0, 0, (void**)&pVtx, 0)))
	{
		// �G���[���N������֐��𔲂���
		return;
	}

	// �A�j���[�V�����J�E���^�[�����Z
	(*pCounterAnim)++;

	// �e�N�X�`�����W���v�Z
	float Uv = 1.0f / pPosX;
	float Hv = 1.0f / pPosY;


	// �A�j���[�V�����̃X�s�[�h
	if (*pCounterAnim >= pAnimSpeed)
	{
		*pCounterAnim = 0;//�J�E���^�[�������l�ɖ߂�

		(*pPatternAnim)++;//�p�^�[���i���o�[���X�V

		pVtx += 4 * nCnt;

		// �e�N�X�`�����W�̍X�V
		pVtx[0].tex = D3DXVECTOR2(*pPatternAnim * Uv, (*pPatternAnim / pPosX) * Hv);
		pVtx[1].tex = D3DXVECTOR2(Uv + *pPatternAnim * Uv, (*pPatternAnim / pPosX) * Hv);
		pVtx[2].tex = D3DXVECTOR2(*pPatternAnim * Uv, (*pPatternAnim / pPosX) * Hv + Hv);
		pVtx[3].tex = D3DXVECTOR2(Uv + *pPatternAnim * Uv, (*pPatternAnim / pPosX) * Hv + Hv);

		// �p�^�[�����ő�ɂȂ�����
		if (*pPatternAnim >= (pPosX * pPosY))
		{
			*pPatternAnim = 0;//�p�^�[���i���o�[�������l�ɖ߂�
		}
	}

	// ���_�o�b�t�@�̃A�����b�N
	pVtxBuff->Unlock();
}
//============================================
// ���C�Ɖ~�̔���
//============================================
bool CollisionRayToSphere(D3DXVECTOR3 SenterPos, D3DXVECTOR3* pMove, D3DXVECTOR3* DragPos, float fRadius)
{
	// ���C�̎n�_�ƏI�_
	D3DXVECTOR3 rayOrigin, rayDir;

	// �}�E�X�̍��W
	POINT pt;

	// �}�E�X�̍��W�̎擾
	GetCursorPos(&pt);

	// �}�E�X�̍��W���N���C�A���g���W�ɕϊ�
	ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &pt);

	// �}�E�X��3D���W�ɕϊ����ă��C���΂�
	GetRayFromMouse(pt.x, pt.y, &rayOrigin, &rayDir);

	// ���C�̌��_�`�~�̒��S�܂ł̃x�N�g�������߂�
	D3DXVECTOR3 SenterVec = SenterPos - rayOrigin;

	// �ǂꂾ������Ă��邩����ςŋ��߂�
	float dot = SenterVec.x * rayDir.x + SenterVec.y * rayDir.y + SenterVec.z * rayDir.z;

	// �ŒZ�̓_�����߂�
	D3DXVECTOR3 NearPos = rayOrigin + rayDir * dot;

	// �ŒZ�̋��������߂�
	D3DXVECTOR3 Diff = NearPos - SenterPos;

	// �����ɕϊ�
	float distance = D3DXVec3Length(&Diff);

	// ���a
	float radius = fRadius + fRadius;

	// ���C�̋��������a�����傫��������
	if (distance >= radius)
	{
		// �������Ă��Ȃ�
		return false;
	}

	// ���������_�����߂�
	D3DXVECTOR3 HitPos = rayOrigin + rayDir * dot;

	// ���������Ώۂ̈ʒu���}�E�X�̍��W�ɂ���
	DragPos->x = HitPos.x;
	DragPos->z = HitPos.z;

	// �ړ��ʂ����Z�b�g����
	*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �������Ă���
	return true;
}
//============================================
// �����O����`���ė����n�_�ɓ��B���鏈��
//============================================
D3DXVECTOR3 ComputeThrowVelocity(const D3DXVECTOR3 dest,const D3DXVECTOR3 start,const float gravity,const float time)
{
	// ����
	D3DXVECTOR3 velocity;

	D3DXVECTOR3 DiffPos = dest - start;

	// ��t���[���ǂ̂��炢�����������v�Z
	velocity.x = DiffPos.x / time;
	velocity.z = DiffPos.z / time;

	// �d�͂ɂ�闎�������̉e��(0.5f * -g * time * time)
	velocity.y = (DiffPos.y - 0.5f * -gravity * time * time) / time;

	// �l��Ԃ�
	return velocity;
}

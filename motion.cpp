//==============================================================================================================
//
// モーション [motion.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include "motion.h"
#include"meshfield.h"
#include <stdio.h>
#include "player.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MAX_WORD (128) // 最大の文字数

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void UpdateCurrentMotion(MOTION* pMotion,int nCntModel,Motion_Data *pMotionData);					// 全身の現在のモーションの更新処理
void UpdateBlendMotion(MOTION* pMotion, int nCntModel, int nextKey, Motion_Data* pMotionData);		// 全身のブレンドのモーションの更新処理
bool EndMotion(MOTION* pMotion, Motion_Data* pMotionData,int nFrameBlend);							// モーションが終わったかどうか		

//=========================================================================================================
// モーションの初期化
//=========================================================================================================

//=========================================================================================================
// モーションの更新処理
//=========================================================================================================
void UpdateMotion(MOTION *pMotion,bool bFullBody)
{
	for (int nCnt = 0; nCnt < MOTION_PART_MAX; nCnt++)
	{
		// 全身のモーションデータのアドレスを代入
		Motion_Data* pMotionData = &pMotion->MotionData[nCnt];

		// モデルの数分回す
		for (int nCntModel = 0; nCntModel < pMotion->nNumModel; nCntModel++)
		{
			if (bFullBody == true && nCnt != MOTION_PART_FULLBODY) return;

			// 上半身の時に下半身のモデルが来たら
			if (nCnt == MOTION_PART_UPPERBODY && pMotion->aModel[nCntModel].bUpperBody == false) continue;

			// 下半身の時に上半身のモデルが来たら
			if (nCnt == MOTION_PART_LOWERBODY && pMotion->aModel[nCntModel].bUpperBody == true) continue;

			if (pMotion->aMotionInfo[pMotionData->motionType].nNumkey != NULL)
			{
				// 次のキー
				pMotionData->nextKey = (pMotionData->nKey + 1) % pMotion->aMotionInfo[pMotionData->motionType].nNumkey;
			}
			else if (pMotion->aMotionInfo[pMotionData->motiontypeBlend].nNumkey != NULL)
			{
				// ブレンドモーションの次のキー
				pMotionData->nNextKeyBlend = (pMotionData->nKeyBlend + 1) % pMotion->aMotionInfo[pMotionData->motiontypeBlend].nNumkey;
			}

			// モーションブレンドが終了したら
			if (pMotionData->bFinishMotion == false)
			{
				// 現在のモーションの更新処理
				UpdateCurrentMotion(pMotion, nCntModel, pMotionData);
			}

			if ((pMotionData->bFinishMotion == true || pMotionData->bFirstMotion == true) && pMotionData->bBlendMotion == true)
			{
				// ブレンドモーションの更新処理
				UpdateBlendMotion(pMotion, nCntModel, pMotionData->nextKey, pMotionData);
			}

			// オフセットを考慮した向き設定
			pMotion->aModel[nCntModel].rot += pMotion->aModel[nCntModel].offrot;

			// オフセットを考慮した位置設定
			pMotion->aModel[nCntModel].pos += pMotion->aModel[nCntModel].offpos;
		}


		int LastKey = pMotion->aMotionInfo[pMotionData->motionType].nNumkey - 1;

		int nBlendFrame = pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[LastKey].nFrame;

		// モーションが終わったら
		if (pMotion->aMotionInfo[pMotionData->motionType].bLoop == false && pMotionData->nKey >= pMotion->aMotionInfo[pMotionData->motionType].nNumkey - 1 &&
			pMotionData->bBlendMotion == true && pMotionData->bFinishMotion == false && pMotionData->bFirstMotion == false)
		{
			pMotionData->nCounterBlend = 0;
			pMotionData->nFrameBlend = nBlendFrame;
			pMotionData->bFinishMotion = true;
			pMotionData->motiontypeBlend = MOTIONTYPE_NEUTRAL; // モーションタイプをニュートラルにする
		}

		// モーションの出だしのブレンドが終了した
		if (pMotionData->bFirstMotion == true && pMotionData->nCounterBlend >= pMotionData->nFrameBlend)
		{
			// もとに戻す
			pMotionData->bFirstMotion = false;

			// モーションをブレンドしたモーションにする
			pMotionData->motionType = pMotionData->motiontypeBlend;

			// 0番目から始める
			pMotionData->nKey = 0;

			// ブレンドしたフレームから開始
			pMotionData->nCountMotion = 0;
		}

		// モーションが終わるかつキーが最大かつブレンドのカウントが最大になった
		if (pMotionData->nCounterBlend >= nBlendFrame && pMotionData->bBlendMotion == true && pMotionData->bFinishMotion == true && pMotionData->bFirstMotion == false)
		{
			pMotionData->bFinishMotion = false;           // もとに戻す
			pMotionData->bBlendMotion = false;			  // もとに戻す
			pMotionData->motionType = MOTIONTYPE_NEUTRAL; // モーションタイプをニュートラルにする
		}

		// ブレンドがない
		if (!pMotionData->bBlendMotion == true && pMotionData->bFinishMotion == true)
		{
			pMotionData->motionType = MOTIONTYPE_NEUTRAL;
		}

		// モーションカウントの設定
		if (pMotionData->nCountMotion >= pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[pMotionData->nKey].nFrame)
		{
			// 情報がなかったら
			if (pMotion->aMotionInfo[pMotionData->motionType].nNumkey != NULL)
			{
				//モーションカウントが最大になったら0に戻す
				pMotionData->nKey = (pMotionData->nKey + 1) % pMotion->aMotionInfo[pMotionData->motionType].nNumkey;
			}
			else
			{

			}
			pMotionData->nCountMotion = 0;

		}

		int Frame = 0;

		// キーの総数分回す
		for (int nCntKey = 0; nCntKey < pMotion->aMotionInfo[pMotionData->motiontypeBlend].nNumkey; nCntKey++)
		{
			Frame += pMotion->aMotionInfo[pMotionData->motiontypeBlend].aKeyInfo[nCntKey].nFrame;

			// フレームの合計を調べる
			pMotionData->nAllFrame = Frame;
		}


		// 合計フレームを超えたら
		if (pMotionData->nAllCounterMotion >= pMotionData->nAllFrame)
		{
			// カウントをリセット
			pMotionData->nAllCounterMotion = 0;
		}

		// モーションカウンターの更新
		if (pMotionData->bFirstMotion == false)
		{
			// 合計フレームのカウント
			pMotionData->nAllCounterMotion++;

			//モーションカウントを加算
			pMotionData->nCountMotion++;
		}

		// ブレンドが始まったら
		if (pMotionData->bFinishMotion == true || pMotionData->bFirstMotion == true)
		{
			pMotionData->nCounterBlend++; // ブレンドモーションカウントをインクリメント
		}
	}
	//// モーションが死亡だったら
	//if (pMotion->motiontypeBlend == MOTIONTYPE_DEATH && pMotion->nKey >= pMotion->aMotionInfo[pMotion->motionType].nNumkey - 1)
	//{
	//	pMotion->nCountMotion--;
	//	pMotion->nCounterBlend--;
	//}

}
//================================================================================================================
// モーションの設定処理
//================================================================================================================
void SetMotion(MOTION* pMotion, MOTIONTYPE motiontypeBlend, bool Blend, int nFrameBlend,int part)
{
	Motion_Data* pMotionData = &pMotion->MotionData[part];

	// モーションの情報がなかったら
	if (pMotion->aMotionInfo[pMotionData->motiontypeBlend].nNumkey == 0) return;

	if (pMotion->MotionData[part].motiontypeBlend == motiontypeBlend) return;

	// 最初のブレンド
	// モーションブレンドがあるかどうか
	if (Blend == true)
	{
		// 最初のモーションブレンドが終わってたら
		if (pMotionData->bFirstMotion == false)
		{
			pMotionData->bFirstMotion = true;
			pMotionData->nKeyBlend = 0;						// 0から始める
			pMotionData->nCounterBlend = 0;					// 0から始める
		}
		pMotionData->bBlendMotion = Blend;					// ブレンドがあるかどうか
		pMotionData->nFrameBlend = nFrameBlend;				// ブレンドのフレームを代入
		pMotionData->motiontypeBlend = motiontypeBlend;		// ブレンドするモーションのタイプを代入
		pMotionData->bFinishMotion = false;
	}
	// モーションブレンドがない
	else
	{
		pMotionData->bBlendMotion = Blend;					// ブレンドがあるかどうか
		pMotionData->motionType = motiontypeBlend;				// ブレンドするモーションのタイプを代入
		pMotionData->motiontypeBlend = motiontypeBlend;			// ブレンドするモーションのタイプを代入
		pMotionData->bFinishMotion = false;
	}

	pMotionData->nAllCounterMotion = 0;
}
//================================================================================================================
// 角度の正規化
//================================================================================================================
void NormalizeRotation(float* pRotX, float* pRotY, float* pRotZ)
{
	// 角度の正規化
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
// アクションモーションじゃないか
//================================================================================================================
bool CheckActionMotion(MOTION *pMotion)
{
	//bool bFrag = false;
	//// モーションが攻撃じゃなかったら
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
// 全身のモーションの更新処理
//==============================================================================================================
void UpdateFullBodyMotion()
{
}
//================================================================================================================
// 現在のモーションの更新処理
//================================================================================================================
void UpdateCurrentMotion(MOTION* pMotion, int nCntModel, Motion_Data* pMotionData)
{
	// 現在のキーのアドレスを取得
	KEY* pCurrentKey = &pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[pMotionData->nKey].aKey[nCntModel];

	// 次のキー
	int nextKey = pMotionData->nextKey;

	//次のキーのアドレスを取得
	KEY* pNextKey = &pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[nextKey].aKey[nCntModel];

	// 位置の差分、向きの差分
	float DiffPosX = pNextKey->fPosX - pCurrentKey->fPosX;
	float DiffPosY = pNextKey->fPosY - pCurrentKey->fPosY;
	float DiffPosZ = pNextKey->fPosZ - pCurrentKey->fPosZ;
	float DiffRotX = pNextKey->fRotX - pCurrentKey->fRotX;
	float DiffRotY = pNextKey->fRotY - pCurrentKey->fRotY;
	float DiffRotZ = pNextKey->fRotZ - pCurrentKey->fRotZ;

	// 角度の正規化
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

	// フレームの相対値
	float fRateMotion = (float)pMotionData->nCountMotion / (float)nFrame;

	// 位置を求める
	float fPosX = pCurrentKey->fPosX + DiffPosX * fRateMotion;
	float fPosY = pCurrentKey->fPosY + DiffPosY * fRateMotion;
	float fPosZ = pCurrentKey->fPosZ + DiffPosZ * fRateMotion;

	// 向きを求める
	float fRotX = pCurrentKey->fRotX + DiffRotX * fRateMotion;
	float fRotY = pCurrentKey->fRotY + DiffRotY * fRateMotion;
	float fRotZ = pCurrentKey->fRotZ + DiffRotZ * fRateMotion;

	// 現在の位置を設定
	pMotion->aModel[nCntModel].pos = D3DXVECTOR3(fPosX, fPosY, fPosZ);

	// 向きを設定
	pMotion->aModel[nCntModel].rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);
}
//================================================================================================================
// ブレンドモーションの更新処理
//================================================================================================================
void UpdateBlendMotion(MOTION* pMotion, int nCntModel, int nextKey, Motion_Data* pMotionData)
{
	// 再生フレーム
	int nFrame = pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[pMotionData->nKey].nFrame;

	float fRateMotion = (float)pMotionData->nCountMotion / (float)nFrame; // 相対値
	float fRateBlend = (float)pMotionData->nCounterBlend / (float)pMotionData->nFrameBlend;
	
	// 現在のキーのアドレスを取得
	KEY* pCurrentKey = &pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[pMotionData->nKey].aKey[nCntModel];

	//次のキーのアドレスを取得
	KEY* pNextKey = &pMotion->aMotionInfo[pMotionData->motionType].aKeyInfo[nextKey].aKey[nCntModel];


	//現在のモーションの回転X
	float fDiffMotionRX = pNextKey->fRotX - pCurrentKey->fRotX;

	//現在のモーションの回転Y
	float fDiffMotionRY = pNextKey->fRotY - pCurrentKey->fRotY;

	//現在のモーションの回転Z
	float fDiffMotionRZ = pNextKey->fRotZ - pCurrentKey->fRotZ;

	// 向きX
	float fRotXCurrent = pCurrentKey->fRotX + (fDiffMotionRX * fRateMotion);

	// 向きY
	float fRotYCurrent = pCurrentKey->fRotY + (fDiffMotionRY * fRateMotion);

	// 向きZ
	float fRotZCurrent = pCurrentKey->fRotZ + (fDiffMotionRZ * fRateMotion);

	// 角度の正規化
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

	// ブレンドのキーのアドレスを取得
	KEY* pCurrentBlendKey = &pMotion->aMotionInfo[pMotionData->motiontypeBlend].aKeyInfo[pMotionData->nKeyBlend].aKey[nCntModel];

	// ブレンドの次のキーのアドレスを取得
	KEY* pNextBlendKey = &pMotion->aMotionInfo[pMotionData->motiontypeBlend].aKeyInfo[pMotionData->nNextKeyBlend].aKey[nCntModel];

	//ブレンドモーションの回転X
	float fDiffMotionBlendRX = pNextBlendKey->fRotX - pCurrentBlendKey->fRotX;

	//ブレンドモーションの回転Y
	float fDiffMotionBlendRY = pNextBlendKey->fRotY - pCurrentBlendKey->fRotY;

	//ブレンドモーションの回転Z
	float fDiffMotionBlendRZ = pNextBlendKey->fRotZ - pCurrentBlendKey->fRotZ;

	// 角度の正規化
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

	// ブレンドモーションの回転X
	float fRotXBlend = pCurrentBlendKey->fRotX + (fDiffMotionBlendRX * fRateBlend);

	// ブレンドモーションの回転Y
	float fRotYBlend = pCurrentBlendKey->fRotY + (fDiffMotionBlendRY * fRateBlend);

	// ブレンドモーションの回転Z
	float fRotZBlend = pCurrentBlendKey->fRotZ + (fDiffMotionBlendRZ * fRateBlend);


	//現在のモーションの位置X
	float fDiffMotionPX = pNextKey->fPosX - pCurrentKey->fPosX;

	// 現在の位置の設定
	float fPosXCurrent = pCurrentKey->fPosX + (fDiffMotionPX * fRateMotion);

	//現在のモーションの位置Y
	float fDiffMotionPY = pNextKey->fPosY - pCurrentKey->fPosY;

	// 現在の位置の設定
	float fPosYCurrent = pCurrentKey->fPosY + (fDiffMotionPY * fRateMotion);

	//現在のモーションの位置Z
	float fDiffMotionPZ = pNextKey->fPosZ - pCurrentKey->fPosZ;

	// 現在の位置の設定
	float fPosZCurrent = pCurrentKey->fPosZ + (fDiffMotionPZ * fRateMotion);

	//ブレンドモーションの位置X
	float fDiffMotionBlendPX = pNextBlendKey->fPosX - pCurrentBlendKey->fPosX;

	float fPosXBlend = pCurrentBlendKey->fPosX + (fDiffMotionBlendPX * fRateBlend);

	//ブレンドモーションの位置Y
	float fDiffMotionBlendPY = pNextBlendKey->fPosY - pCurrentBlendKey->fPosY;

	float fPosYBlend = pCurrentBlendKey->fPosY + (fDiffMotionBlendPY * fRateBlend);

	//ブレンドモーションの位置Z
	float fDiffMotionBlendPZ = pNextBlendKey->fPosZ - pCurrentBlendKey->fPosZ;

	float fPosZBlend = pCurrentBlendKey->fPosZ + (fDiffMotionBlendPZ * fRateBlend);

	float fDiffBlendRX = fRotXBlend - fRotXCurrent; // 差分X
	float fDiffBlendRY = fRotYBlend - fRotYCurrent; // 差分Y
	float fDiffBlendRZ = fRotZBlend - fRotZCurrent; // 差分Z

	float fRotX = fRotXCurrent + (fDiffBlendRX * fRateBlend); // 求める値
	float fRotY = fRotYCurrent + (fDiffBlendRY * fRateBlend); // 求める値
	float fRotZ = fRotZCurrent + (fDiffBlendRZ * fRateBlend); // 求める値

	float fDiffBlendPX = fPosXBlend - fPosXCurrent; // 差分X
	float fDiffBlendPY = fPosYBlend - fPosYCurrent; // 差分Y
	float fDiffBlendPZ = fPosZBlend - fPosZCurrent; // 差分Z

	float fPosX = fPosXCurrent + (fDiffBlendPX * fRateBlend); // 求める値
	float fPosY = fPosYCurrent + (fDiffBlendPY * fRateBlend); // 求める値
	float fPosZ = fPosZCurrent + (fDiffBlendPZ * fRateBlend); // 求める値

	// 位置を設定
	pMotion->aModel[nCntModel].pos = D3DXVECTOR3(fPosX, fPosY, fPosZ);

	// 向きを設定
	pMotion->aModel[nCntModel].rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);
}
//===============================================================================================================
// モーションが終わったかどうか		
//===============================================================================================================
bool EndMotion(MOTION* pMotion, Motion_Data* pMotionData, int nFrameBlend)
{
	return false;
}
//===============================================================================================================
// モーションの判定設定処理
//===============================================================================================================
bool CheckMotionBounds(MOTION* pMotion, int nStartFrame, int nEndFrame, int nPart)
{
	// 判定用変数
	bool bFlag = false;

	// モーションのデータの構造体のアドレスを取得
	Motion_Data* pMotionData = &pMotion->MotionData[nPart];

	if (pMotionData->bFirstMotion == true) return false;
	if (pMotionData->bFinishMotion == true) return false;

	// 判定開始のフレーム
	if (pMotionData->nAllCounterMotion >= nStartFrame && pMotionData->nAllCounterMotion <= nEndFrame)
	{
		// 判定開始
		bFlag = true;
	}
	// 判定を返す
	return bFlag;
}

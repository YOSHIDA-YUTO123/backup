//==============================================================================================================
//
// モーション[motion.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _MOTION_H_
#define _MOTION_H_

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include "main.h"
#include<stdio.h>
#include"model.h"

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define FILENAME001 ("data\\ModelPlayer.txt")
#define MAX_PARTS (30) // パーツの最大数
#define MAX_MOTION (25) // モーションの最大数
#define MAX_KEY (56) // キーの最大数
#define NUM_FRAME (10)	// 当たり判定の発生フレーム

//**************************
// キャラの種類
//**************************
typedef enum
{
	MOTION_PART_FULLBODY = 0,
	MOTION_PART_UPPERBODY,
	MOTION_PART_LOWERBODY,
	MOTION_PART_MAX
}MOTION_PART;

//**************************
// モデルのモーション
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
// モーションの名前
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
// キー構造体
//**************************
typedef struct
{
	float fPosX;//位置X
	float fPosY;//位置Y
	float fPosZ;//位置Z

	float fRotX;//向きX
	float fRotY;//向きY
	float fRotZ;//向きZ

}KEY;

//**************************
// キー情報構造体
//**************************
typedef struct
{
	int nFrame;			// 再生フレーム
	KEY aKey[MAX_PARTS];// 各キーパーツのキー要素
}KEY_INFO;


//**************************
// モーション情報構造体
//**************************
typedef struct
{
	bool bLoop;					// ループするかどうか
	int nNumkey;				// キーの総数
	KEY_INFO aKeyInfo[MAX_KEY];	// キー情報
	int nStartFrame[NUM_FRAME], nEndFrame[NUM_FRAME];	// 発生フレーム
	int nNumFrame;	// 発生フレームの数
}MOTION_INFO;

//**************************
// モーション構造体
//**************************
typedef struct
{
	MOTIONTYPE motionType;						// モーションの種類
	bool bLoopMotion;							// ループするかどうか
	int nNumKey;								// キーの総数
	int nKey;									// 現在のキーNo.
	int nCountMotion;							// モーションのカウンター
	int nextKey;                                // 次のキー
	int nAllFrame;								// フレームの合計
	int nAllCounterMotion;						// 合計フレームのカウント

	MOTIONTYPE motiontypeBlend;					// ブレンドの種類
	bool bBlendMotion;							// ブレンドがあるかどうか
	bool bLoopMotionBlend;						// ループするかどうか
	bool bFinishMotion;							// モーションが終わったかどうか
	bool bFirstMotion;                          // モーションが始まったフラグ
	int nNumKeyBlend;							// ブレンドモーションの最大のキー
	int nKeyBlend;								// ブレンドモーションの現在のキー
	int nNextKeyBlend;							// ブレンドモーションの次のキー
	int nCounterMotionBlend;					// ブレンドのカウンター

	int nFrameBlend;							// ブレンドのフレーム数
	int nCounterBlend;							// ブレンドカウンター
}Motion_Data;

//**************************
// モーション構造体
//**************************
typedef struct
{
	MODEL aModel[MAX_PARTS];					// モデルの情報
	int nNumModel;								// モデル(パーツ)の総数(実際に使うモデルの数)
	MOTION_INFO aMotionInfo[MAX_MOTION];		// モーション情報
	int nNumMotion;								// モーションの総数
	Motion_Data MotionData[MOTION_PART_MAX];	// 全身、上半身、下半身
}MOTION;

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void UpdateMotion(MOTION* pMotion, bool bFullBody);			// 全身のモーションの更新処理

void SetMotion(MOTION* pMotion, MOTIONTYPE motiontypeBlend, bool Blend, int nFrameBlend,int part);
void NormalizeRotation(float *pRotX, float* pRotY, float* pRotZ);
bool CheckActionMotion(MOTION* pMotion);																	 // モーションがアクションかじゃないか
bool CheckMotionBounds(MOTION* pMotion, int nStartFrame, int nEndFrame, int nPart);

#endif

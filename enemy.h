//==================================================
//
// 敵
// Author:YOSHIDA YUUTO
//
//==================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_

//**************************
//インクルードファイル
//**************************
#include"main.h"
#include"motion.h"

//**************************
//マクロ定義
//**************************
#define MAX_TEXTURE (128)

//**************************
// 敵の行動の種類
//**************************
typedef enum
{
	ACTONSTATE_NEUTRAL = 0,
	ACTIONSTATE_MOVE,
	ACTIONSTATE_ATTACK,
	ACTIONSTATE_BACKSTEP,
	ACTIONSTATE_MAX
}ACTIONSTATE;

//**************************
// プレイヤー構造体
//**************************
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 posOld;			// 前回の位置
	D3DXVECTOR3 Size;			// 大きさ
	D3DXVECTOR3 rot;			// 向き
	D3DXVECTOR3 rotDestEnemy;	// 目的の角度
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3	vtxMinEnemy;
	D3DXVECTOR3	vtxMaxEnemy;
	D3DXMATRIX mtxWorldEnemy;	// ワールドマトリックス
	int nIdxShadow;
	bool bJump;
	bool bUse;
	int nCounterState;
	int nLife;
	int nMaxLife;
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEXTURE];	// プレイヤーのテクスチャへのポインタ
	LPD3DXMESH pMesh;							// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;						// マテリアルへのポインタ
	DWORD dwNumMat;								// マテリアルの数
	D3DXVECTOR3 axis;	// プレイヤーの回転軸
	MOTION Motion;		// モーションの情報
	float speed;		// 速さ
	bool bFullBody;		// 全身にするかしないか
	int nActionType;
}Enemy;

//**************************
//プロトタイプ宣言
//**************************
void InitEnemy(void);//プレイヤーの初期化処理
void UninitEnemy(void);//プレイヤーの終了処理
void UpdateEnemy(void);//プレイヤーの更新処理
void DrawEnemy(void);//プレイヤーの描画処理
void SetEnemy(D3DXVECTOR3 pos);
Enemy* GetEnemy(void);//プレイヤーの取得処理
void DebugImgui(void);
#endif

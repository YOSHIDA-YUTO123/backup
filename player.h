//==============================================================================================================
//
//プレイヤー
//Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

//**************************
//インクルードファイル
//**************************
#include"main.h"
#include"motion.h"

//**************************
//マクロ定義
//**************************
#define PLAYER_MOVE (1.0f)
#define WIDTH (100.0f) //壁の横幅
#define HEIGHT (100.0f) //壁の高さ
#define MAX_MODEL (16)
#define MAX_ITEM (256)
#define MAX_GLABITY (1.0f)      // 重力
#define MAX_TEXTURE (128)

//**************************
//プレイヤーの状態
//**************************
typedef enum
{
	PLAYERSTATE_NORMAL = 0,
	PLAYERSTATE_MOVE,
	PLAYERSTATE_ATTACK,
	PLAYERSTATE_JUMP,
	PLAYERSTATE_LANDING,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_FALL,
	PLAYERSTATE_INVISIBLE,
	PLAYERSTATE_NO_WEPON,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//**************************
//プレイヤー構造体
//**************************
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 posOld;			// 前回の位置
	D3DXVECTOR3 Size;			// 大きさ
	D3DXVECTOR3 rot;			// 向き
	D3DXVECTOR3 rotDestPlayer;	// 目的の角度
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3	vtxMinPlayer;
	D3DXVECTOR3	vtxMaxPlayer;
	D3DXMATRIX mtxWorldPlayer;	// ワールドマトリックス
	int nIdxShadow;
	bool bJump;
	bool bDisp;

	PLAYERSTATE state;
	int nCounterState;
	int nLife;
	int nMaxLife;
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEXTURE];	//プレイヤーのテクスチャへのポインタ
	LPD3DXMESH pMesh;							//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;						//マテリアルへのポインタ
	DWORD dwNumMat;								//マテリアルの数

	D3DXVECTOR3 axis;	// プレイヤーの回転軸
	MOTION Motion;		// モーションの情報
	float speed;		// 速さ
	bool bFullBody;		// 全身にするかしないか
	D3DXVECTOR3 normal;
}Player;

//**************************
//プロトタイプ宣言
//**************************
void InitPlayer(void);//プレイヤーの初期化処理
void UninitPlayer(void);//プレイヤーの終了処理
void UpdatePlayer(void);//プレイヤーの更新処理
void DrawPlayer(void);//プレイヤーの描画処理
Player* GetPlayer(void);//プレイヤーの取得処理
void DebugImgui(void);
#endif

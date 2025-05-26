//==============================================================================================================
//
// ボール [ball.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _BALL_H_
#define _BALL_H_

//***************************
//インクルードファイル
//***************************
#include"main.h"

//***************************
//マクロ定義
//***************************
#define MAX_TEXTURE (128)	// テクスチャの最大数
#define MAX_NAME (256)		// 名前の長さ

//***************************
//モデルの構造体
//***************************
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEXTURE];	//プレイヤーのテクスチャへのポインタ
	LPD3DXMESH pMesh;							//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;						//マテリアルへのポインタ
	DWORD dwNumMat;								//マテリアルの数
	D3DXVECTOR3 pos,posOld;							//位置(オフセット)
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;							//向き
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
	D3DXVECTOR3 vtxMin, vtxMax;
	D3DXVECTOR3 Size;
	D3DXVECTOR3 normal;
	bool bJump;
}Ball;

//***************************
//プロトタイプ宣言
//***************************
void InitBalll(void);
void UninitBalll(void);
void UpdateBalll(void);
void DrawBalll(void);
Ball* GetBall(void);
#endif

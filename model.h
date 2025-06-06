//==============================================================================================================
//
// モデル[model.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _MODEL_H_
#define _MODEL_H_

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
	int nIdxModelParent;						//親モデルのインデックス
	D3DXVECTOR3 pos;							//位置(オフセット)
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;							//向き
	D3DXVECTOR3 offpos;							//位置(オフセット)
	D3DXVECTOR3 offrot;							//向き
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
	D3DXVECTOR3 vtxMin, vtxMax;
	D3DXVECTOR3 Size;
	bool bUpperBody;							// 上半身か下半身か
	char aName[MAX_NAME];						// 名前の長さ
}MODEL;

//***************************
//プロトタイプ宣言
//***************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
#endif

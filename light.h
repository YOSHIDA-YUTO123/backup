//==========================
//
// ライト処理 [light.h]
// Author: TEAM_C
//
//==========================
#ifndef _LIGHT_H_
#define _LIGHT_H_

//****************************
// インクルードファイル宣言
//****************************
#include "main.h"

//****************************
// マクロ定義
//****************************
#define MAX_LIGHT (50)			// ライトの最大数
#define MAX_TEXTURE (128)		// テクスチャの最大数

//****************************
// ライトの種類
//****************************
typedef enum
{
	LIGHTTYPE_NORMAL = 0,
	LIGHTTYPE_FIVER,
	LIGHTTYPE_MAX
}LIGHTTYPE;


//****************************
// ライトオリジナルのモデルの構造体
//****************************
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEXTURE];	//プレイヤーのテクスチャへのポインタ
	LPD3DXMESH pMesh;							//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;						//マテリアルへのポインタ
	DWORD dwNumMat;								//マテリアルの数
}LightModelOrigin;

//****************************
// ライトのモデルの構造体
//****************************
typedef struct
{
	D3DLIGHT9 alight;
	bool bUse;
	D3DXVECTOR3 rot;							// 向き
	D3DXMATRIX mtxWorld;						// ワールドマトリクス
	LightModelOrigin lightModel;				// ライトのモデル
}LightModel;

//**********************
// プロトタイプ宣言
//**********************
void InitLight(void);		// ライトの初期化
void UninitLight(void);		// ライトの終了
void UpdateLight(void);		// ライトの更新
void DrawLight(void);		// ライトの描画処理
void SetLight(D3DXVECTOR3 pos, D3DLIGHTTYPE type, D3DXVECTOR3 dir);		// ライトの設定処理
#endif
//============================
//
// ビルボード[billboard.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//****************************
//インクルードファイル
//****************************
#include"main.h"

//****************************
//ビルボードの構造体
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXMATRIX mtxWorld;//ワールドマトリックス
	int nLife;//寿命
	bool bUse;//使用しているかどうか
}BILLBOARD;

//****************************
//プロトタイプ宣言
//****************************
void InitBillboard(void);//ビルボードの初期化処理
void UninitBillboard(void);//ビルボードの終了処理
void UpdateBillboard(void);//ビルボードの更新処理
void DrawBillboard(void);//ビルボードの描画処理
void SetBillboard(D3DXVECTOR3 pos);
#endif

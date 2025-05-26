//============================================================================================================
//
// 線の描画  [line.cpp]
// Author: YOSHIDA YUTO
//
//============================================================================================================

//***********************************************************************************************************
// インクルードファイル
//***********************************************************************************************************
#include "line.h"

//***********************************************************************************************************
// マクロ定義
//***********************************************************************************************************
#define BOX_VERTEX (8)		// 箱の頂点の数
#define BOXLINE_VERTEX (12) // 箱の線の頂点の数

//============================================================================================================
// プロトタイプ宣言
//============================================================================================================

//============================================================================================================
// グローバル変数宣言
//============================================================================================================


//============================================================================================================
// 線の描画処理
//============================================================================================================
void DrawBoxLine(D3DXVECTOR3 pos,D3DXVECTOR3 Size,D3DXCOLOR col,bool bSenterPos)
{
	D3DXVECTOR3 BoundingBoxPos[BOX_VERTEX] = {};

	// 中心だったら
	if (bSenterPos == true)
	{
		// ボックスの頂点を打つ
		BoundingBoxPos[0] = pos + D3DXVECTOR3(-Size.x, -Size.y, -Size.z);
		BoundingBoxPos[1] = pos + D3DXVECTOR3(Size.x, -Size.y, -Size.z);
		BoundingBoxPos[2] = pos + D3DXVECTOR3(Size.x, Size.y, -Size.z);
		BoundingBoxPos[3] = pos + D3DXVECTOR3(-Size.x, Size.y, -Size.z);
		BoundingBoxPos[4] = pos + D3DXVECTOR3(-Size.x, -Size.y, Size.z);
		BoundingBoxPos[5] = pos + D3DXVECTOR3(Size.x, -Size.y, Size.z);
		BoundingBoxPos[6] = pos + D3DXVECTOR3(Size.x, Size.y, Size.z);
		BoundingBoxPos[7] = pos + D3DXVECTOR3(-Size.x, Size.y, Size.z);
	}
	else // 中心じゃなかったら
	{
		// ボックスの頂点を打つ
		BoundingBoxPos[0] = pos + D3DXVECTOR3(-Size.x, 1.0f, -Size.z);
		BoundingBoxPos[1] = pos + D3DXVECTOR3(Size.x, 1.0f, -Size.z);
		BoundingBoxPos[2] = pos + D3DXVECTOR3(Size.x, Size.y, -Size.z);
		BoundingBoxPos[3] = pos + D3DXVECTOR3(-Size.x, Size.y, -Size.z);
		BoundingBoxPos[4] = pos + D3DXVECTOR3(-Size.x, 1.0f, Size.z);
		BoundingBoxPos[5] = pos + D3DXVECTOR3(Size.x, 1.0f, Size.z);
		BoundingBoxPos[6] = pos + D3DXVECTOR3(Size.x, Size.y, Size.z);
		BoundingBoxPos[7] = pos + D3DXVECTOR3(-Size.x, Size.y, Size.z);
	}

	// バウンディングボックスを構成するラインのペア（頂点のインデックス）
	int lines[BOXLINE_VERTEX][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},  // 前面
		{4, 5}, {5, 6}, {6, 7}, {7, 4},  // 背面
		{0, 4}, {1, 5}, {2, 6}, {3, 7}   // 上下
	};

	// 線を描画
	for (int nCntVetx = 0; nCntVetx < BOXLINE_VERTEX; nCntVetx++)
	{
		// 視点のインデックス
		int startidx = lines[nCntVetx][0];

		// 終点のインデックス
		int endidx = lines[nCntVetx][1];

		D3DXVECTOR3 start = BoundingBoxPos[startidx];
		D3DXVECTOR3 end = BoundingBoxPos[endidx];

		// 線の描画
		Draw3DLine(start, end, col);
	}
}
//============================================================================================================
// 
//============================================================================================================
void Draw3DLine(D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR color)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX prevWorld;
	pDevice->GetTransform(D3DTS_WORLD, &prevWorld);  // 今のマトリクス保存

	D3DXMATRIX Identity;
	D3DXMatrixIdentity(&Identity);
	pDevice->SetTransform(D3DTS_WORLD, &Identity);  // 一時的に無影響にする


	LINEVERTEX line[2];  // 線を構成する2つの頂点

	line[0].pos = start;
	line[0].color = color;

	line[1].pos = end;
	line[1].color = color;

	// 頂点フォーマットを設定
	pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	// テクスチャを外す
	pDevice->SetTexture(0, NULL);

	// 頂点データを使ってラインを描画
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, line, sizeof(LINEVERTEX));

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetTransform(D3DTS_WORLD, &prevWorld);  // 元に戻す
}

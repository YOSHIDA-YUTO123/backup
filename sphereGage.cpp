//=============================================================
//
// 円形のゲージ [sphereGage.cpp]
// Author:YUTO YOSHIDA
//
//=============================================================

//*************************************************************
// インクルードファイル
//*************************************************************
#include "sphereGage.h"
#include "player.h"

//*************************************************************
// マクロ定義
//*************************************************************

#define XVTX (36)
#define YVTX (1)

#define VERTEX ((XVTX + 1) * (YVTX + 1)) //頂点数
#define INDEX (POLYGON + 2)  //インデックス
#define POLYGON (((XVTX * YVTX) * 2)) + (4 * (YVTX - 1)) //ポリゴン数

//*************************************************************
// グローバル変数宣言
//*************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSphere = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSphere = NULL;//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBufferSphere = NULL;	// インデックスバッファへのポインタ

//=============================================================
// 初期化処理
//=============================================================
void InitSphere(void)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\gameclear.png",
		&g_pTextureSphere);

	//頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSphere,
		NULL);

	Player* pPlayer = GetPlayer();

	// HPの割合を求める
	float fRateLife = (float)pPlayer->nLife / (float)pPlayer->nMaxLife;

	float fCurrentVtx = fRateLife * VERTEX;

	//頂点ロック
	g_pVtxBuffSphere->Lock(0, 0, (void**)&pVtx, 0);

	float fTexX = 1.0f / XVTX;
	float fTexY = 1.0f / YVTX;

	float fRadius = 50.0f;

	int nCnt = 0;

	for (int nCntY = 0; nCntY <= YVTX; nCntY++)
	{
		for (int nCntX = 0; nCntX <= XVTX; nCntX++)
		{
			float fAngle = (D3DX_PI * 2.0f) / XVTX * nCntX;

			//頂点座標の設定
			pVtx[nCnt].pos = D3DXVECTOR3(
				640.0f + sinf(fAngle) * fRadius,
				340.0f + cosf(fAngle) * fRadius,
				0.0f);

			//rhwの設定
			pVtx[nCnt].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCnt].col = D3DCOLOR_RGBA(200, 255, 100, 255);

			pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntY, fTexY * nCntX);

			nCnt++;
		}
		fRadius += fRadius;
	}
	//頂点ロック解除
	g_pVtxBuffSphere->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBufferSphere,
		NULL);

	WORD* pIdx;

	//インデックスバッファのロック
	g_pIdxBufferSphere->Lock(0, 0, (void**)&pIdx, 0);

	int IndxNum = XVTX + 1;//X

	int IdxCnt = 0;//配列

	int Num = 0;//

	//インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < YVTX; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= XVTX; IndxCount2++, IndxNum++, Num++)
		{
			pIdx[IdxCnt] = IndxNum;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}

		// NOTE:最後の行じゃなかったら
		if (IndxCount1 < YVTX - 1)
		{
			pIdx[IdxCnt] = Num - 1;
			pIdx[IdxCnt + 1] = IndxNum;
			IdxCnt += 2;
		}

	}

	//インデックスバッファのアンロック
	g_pIdxBufferSphere->Unlock();

}
//=============================================================
// 終了処理
//=============================================================
void UninitSphere(void)
{
	if (g_pTextureSphere != NULL)
	{
		g_pTextureSphere->Release();
		g_pTextureSphere = NULL;
	}
	if (g_pVtxBuffSphere != NULL)
	{
		g_pVtxBuffSphere->Release();
		g_pVtxBuffSphere = NULL;
	}
	if (g_pIdxBufferSphere != NULL)
	{
		g_pIdxBufferSphere->Release();
		g_pIdxBufferSphere = NULL;
	}

}
//=============================================================
// 更新処理
//============================================================
void UpdateSphere(void)
{

}
//=============================================================
// 描画処理
//============================================================
void DrawSphere(void)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSphere, 0, sizeof(VERTEX_2D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBufferSphere);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	Player* pPlayer = GetPlayer();

	// HPの割合を求める
	float fRateLife = (float)pPlayer->nLife / (float)pPlayer->nMaxLife;

	int fCurrentVtxX = fRateLife * XVTX;

	int vertex = (fCurrentVtxX + 1) * (YVTX + 1);
	int fCurrentPolygon = ((fCurrentVtxX * YVTX) * 2) + (4 * (YVTX - 1));

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, vertex, 0, fCurrentPolygon);
}

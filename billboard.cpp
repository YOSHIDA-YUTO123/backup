//============================
//
// ビルボード[billboard.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include "billboard.h"
#include "input.h"
#include "player.h"
#include "camera.h"
#include "quaternion.h"

//****************************
//マクロ定義
//****************************
#define BILL_WIDTH (3.0f) //横幅
#define BILL_HEIGHT (100.0f) //高さ
#define MAX_BILLBOARD (128) //ビルボードの最大数
#define MAX_MOVE (5.0f) //移動量

//****************************
//グローバル変数宣言
//****************************
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL; //頂点バッファへのポインタ
BILLBOARD g_Billboard[MAX_BILLBOARD]; 

//============================
//ビルボードの初期化処理
//============================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",
		&g_pTextureBillboard);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D* pVtx = NULL;

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		g_Billboard[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Billboard[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Billboard[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Billboard[nCnt].nLife = 0;
		g_Billboard[nCnt].bUse = false;
	}

	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Billboard[nCnt].pos.x - BILL_WIDTH, g_Billboard[nCnt].pos.y + BILL_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Billboard[nCnt].pos.x + BILL_WIDTH, g_Billboard[nCnt].pos.y + BILL_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Billboard[nCnt].pos.x - BILL_WIDTH, g_Billboard[nCnt].pos.y - BILL_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Billboard[nCnt].pos.x + BILL_WIDTH, g_Billboard[nCnt].pos.y - BILL_HEIGHT, 0.0f);

		//各頂点の法線の設定(ベクトルの大きさは１にする必要がある)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);//カメラに対して垂直に
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.3f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffBillboard->Unlock();
}
//============================
//ビルボードの終了処理
//============================
void UninitBillboard(void)
{
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//頂点バッファの解放
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}
//============================
//ビルボードの更新処理
//============================
void UpdateBillboard(void)
{
	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		//使用されている
		if (g_Billboard[nCnt].bUse == true)
		{
			g_Billboard[nCnt].rot.x += 0.01f;
		}
	}
}
//============================
//ビルボードの描画処理
//============================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	Player* pPlayer = GetPlayer();
	Camera* pCamera = GetCamera();

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans,mtxView;

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (g_Billboard[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Billboard[nCnt].mtxWorld);

			pDevice->GetTransform(D3DTS_VIEW, &mtxView);//ビューマトリックスの取得

			//カメラの逆行列の設定
			g_Billboard[nCnt].mtxWorld._11 = mtxView._11;
			g_Billboard[nCnt].mtxWorld._12 = mtxView._21;
			g_Billboard[nCnt].mtxWorld._13 = mtxView._31;

			g_Billboard[nCnt].mtxWorld._21 = mtxView._12;
			g_Billboard[nCnt].mtxWorld._22 = mtxView._22;
			g_Billboard[nCnt].mtxWorld._23 = mtxView._32;

			g_Billboard[nCnt].mtxWorld._31 = mtxView._13;
			g_Billboard[nCnt].mtxWorld._32 = mtxView._23;
			g_Billboard[nCnt].mtxWorld._33 = mtxView._33;
			
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Billboard[nCnt].pos.x, g_Billboard[nCnt].pos.y, g_Billboard[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Billboard[nCnt].mtxWorld, &g_Billboard[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Billboard[nCnt].mtxWorld);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//テクスチャフォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBillboard);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//============================
//ビルボードの設定処理
//============================
void SetBillboard(D3DXVECTOR3 pos)
{
	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (g_Billboard[nCnt].bUse == false)
		{
			g_Billboard[nCnt].pos = pos;
			g_Billboard[nCnt].bUse = true;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - BILL_WIDTH, pos.y + BILL_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + BILL_WIDTH, pos.y + BILL_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - BILL_WIDTH, pos.y - BILL_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + BILL_WIDTH, pos.y - BILL_HEIGHT, 0.0f);

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffBillboard->Unlock();
}

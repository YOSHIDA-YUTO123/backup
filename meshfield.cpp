//=====================================================================================================================
//
// メッシュフィールド[meshfield.cpp]
// Author: TEAM_C
//
//=====================================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include "meshfield.h"
#include "input.h"
#include "player.h"
#include "math.h"
#include"imgui.h"
#include "line.h"
#include "ball.h"

//**************************************************************************************************************
//グローバル変数宣言
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;	    // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL; // 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshFiled = NULL;  // インデックスバッファへのポインタ
MESHFILED g_MeshField;

//=================================================================================================================
//メッシュフィールドの初期化処理
//=================================================================================================================
void InitMeshField(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/field.png",
		&g_pTextureMeshField);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(DWORD) * INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshFiled,
		NULL);

	g_MeshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshField.bEditMode = false;

	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	float fTexX = 1.0f / XVTX;
	float fTexY = 1.0f / ZVTX;
	int nCnt = 0;

	//縦
	for (int nCntZ = 0; nCntZ <= ZVTX; nCntZ++)
	{
		//横
		for (int nCntX = 0; nCntX <= XVTX; nCntX++)
		{
			pVtx[nCnt].pos = D3DXVECTOR3(
				(0.0f + (MAX_WIDTH / XVTX) * nCntX) - (MAX_WIDTH * 0.5f),
				0.0f,
				MAX_HEIGHT - ((MAX_HEIGHT / ZVTX) * nCntZ) - (MAX_HEIGHT * 0.5f));

			//法線ベクトルの設定
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntX,nCntZ * fTexY);

			nCnt++;//加算
		}
	}

	g_pVtxBuffMeshField->Unlock();

	DWORD* pIdx;

	//インデックスバッファのロック
	g_pIdxBuffMeshFiled->Lock(0, 0, (void**)&pIdx, 0);

	int IndxNum = XVTX + 1;//X

	int IdxCnt = 0;//配列

	int Num = 0;//

	//インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < ZVTX; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= XVTX; IndxCount2++, IndxNum++, Num++)
		{
			pIdx[IdxCnt] = IndxNum;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}

		// NOTE:最後の行じゃなかったら
		if (IndxCount1 < ZVTX - 1) 
		{
			pIdx[IdxCnt] = Num - 1;
			pIdx[IdxCnt + 1] = IndxNum;
			IdxCnt += 2;
		}
	}

	//インデックスバッファのアンロック
	g_pIdxBuffMeshFiled->Unlock();
}

//=================================================================================================================
//メッシュフィールドの終了処理
//=================================================================================================================ccc
void UninitMeshField(void)
{
	//テクスチャの解放
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//頂点バッファの解放
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffMeshFiled != NULL)
	{
		g_pIdxBuffMeshFiled->Release();
		g_pIdxBuffMeshFiled = NULL;
	}
}

//=================================================================================================================
//メッシュフィールドの更新処理
//=================================================================================================================
void UpdateMeshField(void)
{
	VERTEX_3D* pVtx;

	MODE mode = GetMode();

	Player* pPlayer = GetPlayer();

	//頂点バッファをロック
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	static float speed = -10.5f;
	float fHeight = 100.0f;
	int nCnt = 0;

	for (int nCntVertex = 0; nCntVertex < VERTEX; nCntVertex++)
	{
		if (GetKeyboardPress(DIK_UP))
		{
			if (sphererange(&pVtx[nCntVertex].pos, &pPlayer->pos, 100.0f, 50.0f))
			{
				pVtx[nCntVertex].pos.y += 10.0f;
			}
		}
	}

	//for (int nCntZ = 0; nCntZ <= ZVTX; nCntZ++)
	//{
	//	// 横
	//	for (int nCntX = 0; nCntX <= XVTX; nCntX++)
	//	{
	//		float offset = sinf(g_MeshField.fTime * speed + nCntZ);  // 行ごとの波の位相を調整

	//		// 波の計算（各頂点の高さ）
	//		pVtx[nCnt].pos.y = offset * fHeight;
	//		//
	//		nCnt++;
	//	}
	//}

	g_MeshField.fTime += 1.0f / 60.0f;



	//for (int nCntZ = 0; nCntZ <= ZVTX; nCntZ++)
	//{
	//	// 横
	//	for (int nCntX = 0; nCntX <= XVTX; nCntX++)
	//	{

	//		float fDistance = GetDistance(pVtx[nCnt].pos, pos);

	//		float offSet = sinf(g_MeshField.fTime + (fDistance * 0.01f));

	//		if (offSet <= 0.0f)
	//		{
	//			offSet *= -1.0f;
	//		}

	//		if (fDistance >= speed - 100.0f && fDistance <= speed + 100.0f)
	//		{
	//			// 波の計算（各頂点の高さ）
	//			pVtx[nCnt].pos.y = fHeight * offSet;
	//			pVtx[nCnt].col = COLOR_RED;
	//		}
	//		else
	//		{
	//			pVtx[nCnt].pos.y = 0.0f;
	//			pVtx[nCnt].col = COLOR_WHITE;
	//		}
	//		nCnt++;
	//	}
	//}


	//if (GetKeyboardPress(DIK_Q))
	//{
	//	speed -= 4.0f;
	//}
	//else
	//{
	//	speed += 4.0f;
	//}
	// 頂点バッファのアンロック
	g_pVtxBuffMeshField->Unlock();
}
//=================================================================================================================
//メッシュフィールドの描画処理
//=================================================================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_MeshField.mtxWorldMeshField);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField.rot.y, g_MeshField.rot.x, g_MeshField.rot.z);
	D3DXMatrixMultiply(&g_MeshField.mtxWorldMeshField, &g_MeshField.mtxWorldMeshField, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshField.pos.x, g_MeshField.pos.y, g_MeshField.pos.z);
	D3DXMatrixMultiply(&g_MeshField.mtxWorldMeshField, &g_MeshField.mtxWorldMeshField, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshField.mtxWorldMeshField);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshFiled);

	//テクスチャフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshField);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,VERTEX,0,POLYGON);
}
//==================================================================================================================
//メッシュフィールドの当たり判定
//==================================================================================================================
bool CollisionField(D3DXVECTOR3* pPos)
{
	Player* pPlayer = GetPlayer();

	// 着地判定
	bool bLanding = false;

	DWORD* pIdx;

	VERTEX_3D* pVtx;

	//インデックスバッファのロック
	g_pIdxBuffMeshFiled->Lock(0, 0, (void**)&pIdx, 0);

	//頂点バッファをロック
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	// 1マスのサイズ
	float GridSizeX = MAX_WIDTH / (float)XVTX;
	float GridSizeZ = MAX_HEIGHT / (float)ZVTX;

	float X = pPos->x + (MAX_WIDTH * 0.5f);
	float Z = (MAX_HEIGHT * 0.5f) - pPos->z;

	// 何番目のポリゴンか
	int polyX = (int)(X / GridSizeX);
	int polyZ = (int)(Z / GridSizeZ);

	// 現在のポリゴンのインデックス番号
	int polyIndex = ((polyZ * (XVTX - 1) + polyX) * 2) + (polyZ * 6);

	ImGui::Text("polyIndex = %d", polyIndex);

	for (int nCnt = 0; nCnt < NUM_POLYGON; nCnt++)
	{	
		// 頂点のインデックス
		int Vertex = (polyIndex + nCnt);

		// マイナスだったら
		if (Vertex < 0 || Vertex > INDEX) break;

		// インデックスを取得
		int nIdx0 = pIdx[Vertex];
		int nIdx1 = pIdx[Vertex + 1];
		int nIdx2 = pIdx[Vertex + 2];

		// 頂点を取得
		D3DXVECTOR3 vtx0 = pVtx[nIdx0].pos;
		D3DXVECTOR3 vtx1 = pVtx[nIdx1].pos;
		D3DXVECTOR3 vtx2 = pVtx[nIdx2].pos;

		D3DXVECTOR3 edge0 = vtx1 - vtx0; // 辺ベクトル0
		D3DXVECTOR3 edge1 = vtx2 - vtx1; // 辺ベクトル1
		D3DXVECTOR3 edge2 = vtx0 - vtx2; // 辺ベクトル2

		D3DXVECTOR3 Normal = {};

		// 偶数番目だったら
		if (nCnt % 2 == 0)
		{
			// 偶数番目の三角形
			D3DXVec3Cross(&Normal, &edge0, &edge1);
		}
		else
		{
			// 奇数番目の三角形（順序が逆になっている）
			D3DXVec3Cross(&Normal, &edge1, &edge0);
		}

		D3DXVec3Normalize(&Normal, &Normal);

		g_MeshField.normal = Normal;

		D3DXVECTOR3 PlayerVec0 = *pPos - vtx0;
		D3DXVECTOR3 PlayerVec1 = *pPos - vtx1;
		D3DXVECTOR3 PlayerVec2 = *pPos - vtx2;

		D3DXVECTOR3 Cross0 = {};
		D3DXVECTOR3 Cross1 = {};
		D3DXVECTOR3 Cross2 = {};

		if (nCnt % 2 == 0)
		{
			// 三角形の頂点から外積
			D3DXVec3Cross(&Cross0, &edge0, &PlayerVec0);
			D3DXVec3Normalize(&Cross0, &Cross0);

			D3DXVec3Cross(&Cross1, &edge1, &PlayerVec1);
			D3DXVec3Normalize(&Cross1, &Cross1);

			D3DXVec3Cross(&Cross2, &edge2, &PlayerVec2);
			D3DXVec3Normalize(&Cross2, &Cross2);
		}
		else
		{
			// 三角形の頂点から外積
			D3DXVec3Cross(&Cross0, &PlayerVec0, &edge0);
			D3DXVec3Normalize(&Cross0, &Cross0);

			D3DXVec3Cross(&Cross1, &PlayerVec1, &edge1);
			D3DXVec3Normalize(&Cross1, &Cross1);

			D3DXVec3Cross(&Cross2, &PlayerVec2, &edge2);
			D3DXVec3Normalize(&Cross2, &Cross2);
		}

		if (Cross0.y >= 0.0f && Cross1.y >= 0.0f && Cross2.y >= 0.0f)
		{
			// 平面の方程式のDを計算
			float D = -(Normal.x * vtx0.x + Normal.y * vtx0.y + Normal.z * vtx0.z);

			// プレイヤーの位置に基づいて、プレイヤーのY位置を計算
			float PosY = (Normal.x * pPos->x + Normal.z * pPos->z + D) / -Normal.y;

			D3DXVECTOR3 vec = vtx0 - *pPos;
			D3DXVec3Normalize(&vec, &vec);

			// プレイヤーがポリゴンの裏側かどうかの判定
			float dot = D3DXVec3Dot(&Normal, &vec); // 法線とプレイヤー方向との内積

			// プレイヤーがポリゴンの裏側に出た
			if (dot > 0)
			{
				GetBall()->normal = Normal;
				// Y座標を設定
				pPos->y = PosY;

				// 着地している
				bLanding = true;
				break;
			}
		}
	}

	// 頂点バッファのアンロック
	g_pVtxBuffMeshField->Unlock();

	//インデックスバッファのアンロック
	g_pIdxBuffMeshFiled->Unlock();

	return bLanding;//判定を返す
}
//===================================================
// 乗っているポリゴンの法線の取得
//===================================================
D3DXVECTOR3 GetCollisionNormal(void)
{
	return g_MeshField.normal;
}


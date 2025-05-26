//============================================================================================================
//
// ライト処理 [light.cpp]
// Author:TEAM_C
//
//============================================================================================================

//**************************************************************************************************************
// インクルードファイル宣言
//**************************************************************************************************************
#include "light.h"
#include "player.h"

//***************************************************************************************************************
// グローバル変数宣言
//***************************************************************************************************************
LightModel g_Light[MAX_LIGHT];	// ライトのモデルの構造体
LightModelOrigin g_LightOrigin;	// ライトのモデルのオリジナル

//===========================================================================================================
//　カメラの初期化処理
//===========================================================================================================
void InitLight(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ライトの種類を設定
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// ライトのクリア処理(ゼロメモリー関数)
		ZeroMemory(&g_Light[nCnt].alight, sizeof(g_Light[nCnt].alight));

		// ライトの使用状態
		g_Light[nCnt].bUse = false;
	}

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\OBJ\\light.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_LightOrigin.pBuffMat,
		NULL,
		&g_LightOrigin.dwNumMat,
		&g_LightOrigin.pMesh);

	D3DXMATERIAL* pMat;//マテリアルへのポインタ

	//マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_LightOrigin.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_LightOrigin.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//このファイル名を使用してテクスチャを読み込む
				//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_LightOrigin.pTexture[nCntMat]);
		}
	}
}
//========================================================
//　ライトの終了処理
//========================================================
void UninitLight(void)
{
	// マテリアル分回す
	for (int nCnt = 0; nCnt < (int)g_LightOrigin.dwNumMat; nCnt++)
	{
		// テクスチャの破棄
		if (g_LightOrigin.pTexture[nCnt] != NULL)
		{
			g_LightOrigin.pTexture[nCnt]->Release();
			g_LightOrigin.pTexture[nCnt] = NULL;
		}
	}

	// バッファの破棄
	if (g_LightOrigin.pBuffMat != NULL)
	{
		g_LightOrigin.pBuffMat->Release();
		g_LightOrigin.pBuffMat = NULL;
	}

	// メッシュの破棄
	if (g_LightOrigin.pMesh != NULL)
	{
		g_LightOrigin.pMesh->Release();
		g_LightOrigin.pMesh = NULL;
	}

	// ライトの最大数分回す
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// マテリアル分回す
		for (int nCnt = 0; nCnt < (int)g_Light[nCntLight].lightModel.dwNumMat; nCnt++)
		{
			// テクスチャの破棄
			if (g_Light[nCntLight].lightModel.pTexture[nCnt] != NULL)
			{
				g_Light[nCntLight].lightModel.pTexture[nCnt] = NULL;
			}
		}

		// バッファの破棄
		if (g_Light[nCntLight].lightModel.pBuffMat != NULL)
		{
			g_Light[nCntLight].lightModel.pBuffMat = NULL;
		}

		// メッシュの破棄
		if (g_Light[nCntLight].lightModel.pMesh != NULL)
		{
			g_Light[nCntLight].lightModel.pMesh = NULL;
		}
	}
}
//========================================================
//　ライトの更新処理
//========================================================
void UpdateLight(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得
	pDevice = GetDevice();

	Player* pPlayer = GetPlayer();

	// ライトの種類を設定
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		//g_light[nCnt].Type = D3DLIGHT_POINT;	// 平行光源

		//D3DXVECTOR3 Pos(pPlayer->SwordMtx._41, pPlayer->SwordMtx._42, pPlayer->SwordMtx._43);

		//// ライトの拡散光
		//g_light[nCnt].Direction = pPlayer->pos - pPlayer->posOld;
		//g_light[nCnt].Position = D3DXVECTOR3(Pos);

		//g_light[nCnt].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//g_light[nCnt].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//g_light[nCnt].Range = 500.0f;

		//// スポットライトの角度設定（内側の角度）
		//g_light[nCnt].Theta = D3DXToRadian(15.0f);  // 15度

		//// スポットライトの外側の角度設定
		//g_light[nCnt].Phi = D3DXToRadian(15.0f);    // 30度


		//g_light[nCnt].Attenuation0 = 1.0f;
		////g_light[nCnt].Attenuation1 = 0.1f;
		//g_light[nCnt].Attenuation2 = 0.0f;

		//g_light[nCnt].Falloff = 1.0f;  // 標準的な広がり

		//// ライトの設定
		//pDevice->SetLight(nCnt, &g_light[nCnt]);

		//// ライトを有効化
		//pDevice->LightEnable(nCnt, TRUE);
	}

}
//========================================================
// ライトの描画処理
//========================================================
void DrawLight(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	// ライトの種類を設定
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		if (g_Light[nCnt].bUse == false) continue;

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Light[nCnt].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Light[nCnt].rot.y, g_Light[nCnt].rot.x, g_Light[nCnt].rot.z);
		D3DXMatrixMultiply(&g_Light[nCnt].mtxWorld, &g_Light[nCnt].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Light[nCnt].alight.Position.x, g_Light[nCnt].alight.Position.y, g_Light[nCnt].alight.Position.z);
		D3DXMatrixMultiply(&g_Light[nCnt].mtxWorld, &g_Light[nCnt].mtxWorld, &mtxTrans);


		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Light[nCnt].mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルのデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Light[nCnt].lightModel.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Light[nCnt].lightModel.dwNumMat; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_Light[nCnt].lightModel.pTexture[nCntMat]);

			//モデル(パーツ)の描画
			g_Light[nCnt].lightModel.pMesh->DrawSubset(nCntMat);
		}

		D3DXVECTOR3 start = g_Light[nCnt].alight.Position;
		D3DXVECTOR3 dir = g_Light[nCnt].alight.Direction;

		// 見やすく長くする（たとえば100.0f倍）
		D3DXVECTOR3 end = start + dir * 100000.0f;

	}
	//保存していたマテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}
//========================================================
// ライトの設定処理
//========================================================
void SetLight(D3DXVECTOR3 pos, D3DLIGHTTYPE type,D3DXVECTOR3 dir)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ライトの最大数分回す
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		if (g_Light[nCnt].bUse == false)
		{
			g_Light[nCnt].lightModel = g_LightOrigin;
			g_Light[nCnt].alight.Position = pos;
			g_Light[nCnt].alight.Type = type;
			g_Light[nCnt].bUse = true;

			// ライトの拡散光
			g_Light[nCnt].alight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 正規化する
			D3DXVec3Normalize(&dir, &dir); // ベクトルの大きさを1にする
			g_Light[nCnt].alight.Direction = dir;

			// ライトの設定
			pDevice->SetLight(nCnt, &g_Light[nCnt].alight);

			// ライトを有効化
			pDevice->LightEnable(nCnt, TRUE);

			break;
		}
	}

}


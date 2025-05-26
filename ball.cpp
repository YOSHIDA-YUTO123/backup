//==================================================
//
// ボール [ball.cpp]
// Author:YOSHIDA YUUTO
//
//==================================================

//**************************************************
//インクルードファイル
//**************************************************
#include "ball.h"
#include "math.h"
#include "input.h"
#include"imgui.h"
#include "meshfield.h"
#include "line.h"
#include "player.h"
#include"quaternion.h"

//**************************************************
// プロトタイプ変数宣言
//**************************************************
void UpdateBoundball(const float resutitution);

//**************************************************
// グローバル変数宣言
//**************************************************
Ball g_aBoll;

//==================================================
// 初期化処理
//==================================================
void InitBalll(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_aBoll.pos = D3DXVECTOR3(-500.0f,0.0f,0.0f);
	g_aBoll.rot = INIT_VECTOR3;
	g_aBoll.move = INIT_VECTOR3;
	g_aBoll.bJump = true;

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\OBJ\\ball2.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aBoll.pBuffMat,
		NULL,
		&g_aBoll.dwNumMat,
		&g_aBoll.pMesh);

	D3DXMATERIAL* pMat;//マテリアルへのポインタ

	//マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_aBoll.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aBoll.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//このファイル名を使用してテクスチャを読み込む
				//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_aBoll.pTexture[nCntMat]);
		}
	}

	//頂点座標の取得
	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマットのサイズ
	BYTE* pVtxBuff;	//頂点バッファへのポインタ

	//頂点数の取得
	nNumVtx = g_aBoll.pMesh->GetNumVertices();

	//頂点フォーマットのサイズ取得
	sizeFVF = D3DXGetFVFVertexSize(g_aBoll.pMesh->GetFVF());

	//頂点バッファのロック
	g_aBoll.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//頂点座標を比較してブロックの最小値,最大値を取得
		if (vtx.x < g_aBoll.vtxMin.x)
		{
			g_aBoll.vtxMin.x = vtx.x;
		}
		else if (vtx.y < g_aBoll.vtxMin.y)
		{
			g_aBoll.vtxMin.y = vtx.y;
		}
		else if (vtx.z < g_aBoll.vtxMin.z)
		{
			g_aBoll.vtxMin.z = vtx.z;
		}

		else if (vtx.x > g_aBoll.vtxMax.x)
		{
			g_aBoll.vtxMax.x = vtx.x;
		}

		else if (vtx.y > g_aBoll.vtxMax.y)
		{
			g_aBoll.vtxMax.y = vtx.y;
		}

		else if (vtx.z > g_aBoll.vtxMax.z)
		{
			g_aBoll.vtxMax.z = vtx.z;
		}

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	//各モデルごとのサイズを代入
	g_aBoll.Size.x = g_aBoll.vtxMax.x - g_aBoll.vtxMin.x;
	g_aBoll.Size.y = g_aBoll.vtxMax.y - g_aBoll.vtxMin.y;
	g_aBoll.Size.z = g_aBoll.vtxMax.z - g_aBoll.vtxMin.z;

	//頂点バッファのアンロック
	g_aBoll.pMesh->UnlockVertexBuffer();
}
//==================================================
// 終了処理
//==================================================
void UninitBalll(void)
{
	// マテリアル分回す
	for (int nCnt = 0; nCnt < (int)g_aBoll.dwNumMat; nCnt++)
	{
		// テクスチャの破棄
		if (g_aBoll.pTexture[nCnt] != NULL)
		{
			g_aBoll.pTexture[nCnt]->Release();
			g_aBoll.pTexture[nCnt] = NULL;
		}
	}

	// バッファの破棄
	if (g_aBoll.pBuffMat != NULL)
	{
		g_aBoll.pBuffMat->Release();
		g_aBoll.pBuffMat = NULL;
	}

	// メッシュの破棄
	if (g_aBoll.pMesh != NULL)
	{
		g_aBoll.pMesh->Release();
		g_aBoll.pMesh = NULL;
	}
}
//==================================================
// 更新処理
//==================================================
void UpdateBalll(void)
{
	g_aBoll.posOld = g_aBoll.pos;

	g_aBoll.pos += g_aBoll.move;

	if (CollisionField(&g_aBoll.pos))
	{
		g_aBoll.bJump = true;

		UpdateBoundball(0.7f);
	}
	else
	{
		g_aBoll.bJump = false;
	}
	Player* pPlayer = GetPlayer();

	if (KeyboardTrigger(DIK_H))
	{
		g_aBoll.move = ComputeThrowVelocity(pPlayer->pos, g_aBoll.pos, 1.0f, 100.0f);
	}


	if (GetKeyboardPress(DIK_LSHIFT))
	{
		CollisionRayToSphere(g_aBoll.pos, &g_aBoll.move, &g_aBoll.pos, 50.0f);
	}


	g_aBoll.move.y -= 1.0f;

	if (ImGui::Button(u8"ボールジャンプ"))
	{
		g_aBoll.move.y = 15.0f;
	}
}
//==================================================
// 描画処理
//==================================================
void DrawBalll(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aBoll.mtxWorld);

	// 回転する角度
	static float fAngle;

	// 移動方向を求める
	D3DXVECTOR3 MoveDir = g_aBoll.pos - g_aBoll.posOld;

	// 単位ベクトルにする
	D3DXVec3Normalize(&MoveDir, &MoveDir);

	// 移動量を求める
	D3DXVECTOR3 velocity = g_aBoll.pos - g_aBoll.posOld;

	// y方向の移動量は無視
	velocity.y = 0.0f;

	// 移動した距離をもとめる
	float distance = D3DXVec3Length(&velocity);

	// 回転量を設定
	fAngle += distance / (g_aBoll.Size.y * 0.5f);

	// 回転軸,上方向ベクトル
	D3DXVECTOR3 axis, normal(0.0f, -1.0f, 0.0f);

	// 回転軸を求める
	D3DXVec3Cross(&axis, &normal, &MoveDir);
	D3DXVec3Normalize(&axis, &axis);

	// クォータニオン
	D3DXQUATERNION q;

	// クォータニオンを生成
	CreateQuaternion(&q, axis,fAngle);

	// 回転行列を設定
	SetMatrixRotationQuaternion(&mtxRot, q);

	// 回転行列を掛け合わせる
	D3DXMatrixMultiply(&g_aBoll.mtxWorld, &g_aBoll.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aBoll.pos.x, g_aBoll.pos.y + g_aBoll.Size.y * 0.5f, g_aBoll.pos.z);
	D3DXMatrixMultiply(&g_aBoll.mtxWorld, &g_aBoll.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aBoll.mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_aBoll.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aBoll.dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_aBoll.pTexture[nCntMat]);

		//モデル(パーツ)の描画
		g_aBoll.pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}
//==================================================
// ボールの取得処理
//==================================================
Ball* GetBall(void)
{
	return &g_aBoll;
}
//==================================================
// ボールのバウンド
//==================================================
void UpdateBoundball(const float resutitution)
{
	D3DXVECTOR3 FieldNormal = GetCollisionNormal();

	float dot = D3DXVec3Dot(&g_aBoll.move, &FieldNormal);

	D3DXVECTOR3 NewMove = g_aBoll.move - (dot * 2.0f) * FieldNormal;
	
	g_aBoll.move = NewMove * resutitution;
}

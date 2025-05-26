//===============================================================================================================
//
// 敵 [enemy.cpp]
// Author:YOSHIDA YUUTO
//
//===============================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include "input.h"
#include "camera.h"
#include <stdio.h>
#include <string.h>
#include "motion.h"
#include "mouse.h"
#include "meshfield.h"
#include "math.h"
#include <iostream>
#include "imgui.h"
#include "imguimaneger.h"
#include "imgui_internal.h"
#include "line.h"
#include "quaternion.h"
#include"enemy.h"
#include "player.h"

using namespace ImGui;

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define MAX_WORD (256)			// 最大文字数
#define PLAYERLIFE (1000)		// プレイヤーの体力
#define MAX_TEXPLAYER (128)		// テクスチャの最大数
#define MAX_JUMP (15.0f)		// ジャンプ量
#define MAX_MOVE (1.0f)			// プレイヤーの移動量
#define NUM_MTX (8)				// 剣の当たり判定のマトリクスの数
#define LANDINGEXPLOSION (6)	// 着地したときに出る煙
#define MAX_JUMP (15.0f)        // ジャンプ量

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void LoadFullBodyEnemy(const char* Filepath);										 // プレイヤーのロード処理
int LoadFilenameEnemy(FILE* pFile, int nNumModel, char* aString);						 // プレイヤーのモデルのロード処理
void LoadCharacterSetEnemy(FILE* pFile, char* aString, int nNumparts);					 // プレイヤーのパーツの設定処理
void LoadMotionSetEnemy(FILE* pFile, char* aString, int nNumModel);						 // プレイヤーのモーションのロード処理
void LoadKeySetEnemy(FILE* pFile, char* aString, int nCntMotion);						 // プレイヤーのモーションのキーの読み込み処理

void ChasePlayer(Player* pPlayer);
void TransEnemyState(void);
bool FinishActionMotion(void);
void EndActionTransState(void);


//**************************************************************************************************************
//グローバル変数宣言
//**************************************************************************************************************
Enemy g_Enemy;//プレイヤー構造体

int nCntMotionEnemy, nKeyEnemy;

//===============================================================================================================
//プレイヤーの初期化処理
//===============================================================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	MODE mode = GetMode();//現在のモードを取得

	//プレイヤーの初期化
	g_Enemy.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);		   // 座標
	g_Enemy.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   // 角度
	g_Enemy.Size = D3DXVECTOR3(20.0f, 200.0f, 20.0f);	   // プレイヤーの大きさ
	g_Enemy.rotDestEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 目的の角度
	g_Enemy.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   // 移動量
	g_Enemy.bJump = true;								   // ジャンプ中か否か
	g_Enemy.nLife = PLAYERLIFE;						   // 体力
	g_Enemy.bUse = false;
	//g_Enemy.state = PLAYERSTATE_NORMAL;				   // 状態
	nCntMotionEnemy = 0;
	nKeyEnemy = 0;

	LoadFullBodyEnemy("data/motion.txt");
}
//===============================================================================================================
//プレイヤーの終了処理
//===============================================================================================================
void UninitEnemy(void)
{
	// テクスチャの破棄
	for (int nCntModel = 0; nCntModel < g_Enemy.Motion.nNumModel; nCntModel++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_Enemy.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (g_Enemy.Motion.aModel[nCntModel].pTexture[nCntMat] != NULL)
			{
				g_Enemy.Motion.aModel[nCntModel].pTexture[nCntMat]->Release();
				g_Enemy.Motion.aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

		//メッシュの破棄
		if (g_Enemy.Motion.aModel[nCntModel].pMesh != NULL)
		{
			g_Enemy.Motion.aModel[nCntModel].pMesh->Release();
			g_Enemy.Motion.aModel[nCntModel].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_Enemy.Motion.aModel[nCntModel].pBuffMat != NULL)
		{
			g_Enemy.Motion.aModel[nCntModel].pBuffMat->Release();
			g_Enemy.Motion.aModel[nCntModel].pBuffMat = NULL;
		}
	}
}
//===============================================================================================================
//プレイヤーの更新処理
//===============================================================================================================
void UpdateEnemy(void)
{
	Player* pPlayer = GetPlayer();

	g_Enemy.move.x += (0.0f - g_Enemy.move.x) * 0.5f;
	g_Enemy.move.z += (0.0f - g_Enemy.move.z) * 0.51f;

	g_Enemy.posOld = g_Enemy.pos;

	g_Enemy.pos += g_Enemy.move;

	// メッシュフィールドの当たり判定
	if (CollisionField(&g_Enemy.pos))
	{
		g_Enemy.bJump = true;
	}
	else
	{
		g_Enemy.bJump = false;
	}

	// カプセルの当たり判定
	if (CollisionCapsule(pPlayer->pos, SetMtxConversion(pPlayer->Motion.aModel[2].mtxWorld),
		g_Enemy.pos, SetMtxConversion(g_Enemy.Motion.aModel[2].mtxWorld), 50.0f, 50.0f))
	{
		ImGui::Text("true");
	}

	// プレイヤーを追いかける
	ChasePlayer(pPlayer);

	// 敵の状態遷移処理
	TransEnemyState();
	
	// モーションが終わったら
	if (FinishActionMotion())
	{
		EndActionTransState();
	}
	// 重力
	g_Enemy.move.y -= 1.0f;

	//プレイヤーの角度の正規化
	if (g_Enemy.rotDestEnemy.y - g_Enemy.rot.y >= D3DX_PI)
	{
		g_Enemy.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_Enemy.rotDestEnemy.y - g_Enemy.rot.y <= -D3DX_PI)
	{
		g_Enemy.rot.y -= D3DX_PI * 2.0f;
	}

	// モーションの更新処理
	UpdateMotion(&g_Enemy.Motion, true);

	//プレイヤーの向きを目的の向きに近づける
	g_Enemy.rot.y += (g_Enemy.rotDestEnemy.y - g_Enemy.rot.y) * 0.1f;
}
//===============================================================================================================
// プレイヤーの描画処理
//===============================================================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	Player* pPlayer = GetPlayer();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize, mtxShadow;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	if (g_Enemy.bUse == true)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Enemy.mtxWorldEnemy);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy.rot.y, g_Enemy.rot.x, g_Enemy.rot.z);
		D3DXMatrixMultiply(&g_Enemy.mtxWorldEnemy, &g_Enemy.mtxWorldEnemy, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Enemy.pos.x, g_Enemy.pos.y, g_Enemy.pos.z);
		D3DXMatrixMultiply(&g_Enemy.mtxWorldEnemy, &g_Enemy.mtxWorldEnemy, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Enemy.mtxWorldEnemy);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

		//全モデルパーツの描画
		for (int nCntModel = 0; nCntModel < g_Enemy.Motion.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransform;//計算用
			D3DXMATRIX mtxParent;//親のマトリックス

			//パーツのマトリックスの初期化
			D3DXMatrixIdentity(&g_Enemy.Motion.aModel[nCntModel].mtxWorld);

			//パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Enemy.Motion.aModel[nCntModel].rot.y, g_Enemy.Motion.aModel[nCntModel].rot.x, g_Enemy.Motion.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Enemy.Motion.aModel[nCntModel].mtxWorld, &g_Enemy.Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

			//パーツの位置(オフセット)を反映
			D3DXMatrixTranslation(&mtxTransform, g_Enemy.Motion.aModel[nCntModel].pos.x, g_Enemy.Motion.aModel[nCntModel].pos.y, g_Enemy.Motion.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Enemy.Motion.aModel[nCntModel].mtxWorld, &g_Enemy.Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

			//パーツの[親のマトリックス]を設定
			if (g_Enemy.Motion.aModel[nCntModel].nIdxModelParent != -1)
			{
				//親モデルがある場合
				mtxParent = g_Enemy.Motion.aModel[g_Enemy.Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{//親モデルがない場合
				mtxParent = g_Enemy.mtxWorldEnemy;
			}

			//算出した[パーツのワールドマトリックス]と[親のマトリックス]をかけあわせる
			D3DXMatrixMultiply(&g_Enemy.Motion.aModel[nCntModel].mtxWorld,
				&g_Enemy.Motion.aModel[nCntModel].mtxWorld,
				&mtxParent);//自分自分親

			//パーツのワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD,
				&g_Enemy.Motion.aModel[nCntModel].mtxWorld);

			for (int nCntMat = 0; nCntMat < (int)g_Enemy.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				//マテリアルのデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Enemy.Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

				// モデルの情報がなかったら
				if (g_Enemy.Motion.aModel[nCntModel].pBuffMat == NULL)
				{
					continue;
				}

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_Enemy.Motion.aModel[nCntModel].pTexture[nCntMat]);

				//モデル(パーツ)の描画
				g_Enemy.Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}
		}
	}
	//マテリアルの設定
	pDevice->SetMaterial(&matDef);
}
//============================================
// 敵の設定処理
//============================================
void SetEnemy(D3DXVECTOR3 pos)
{
	if (g_Enemy.bUse == false)
	{
		g_Enemy.pos = pos;
		g_Enemy.bUse = true;
	}
}
//============================================
// プレイヤーのロード処理
//============================================
void LoadFullBodyEnemy(const char* Filepath)
{
	FILE* pFile;
	char aStr[MAX_WORD] = {};
	char Skip[3] = {};

	int nNumModel = 0;
	int nLoadCnt = 0;
	int nNumParts = 0;

	// ファイルを開く
	pFile = fopen(Filepath, "r");

	if (pFile != NULL)
	{
		while (1)
		{
			// 文字列を読み込む
			int nData = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], "#") == 0)
			{//コメントが来たら
				//処理を読み飛ばす
				continue;
			}

			// NUM_MODELを読み取ったら
			if (strcmp(&aStr[0], "NUM_MODEL") == 0)
			{
				nData = fscanf(pFile, "%s", &Skip[0]);						// [=]を読み飛ばす
				nData = fscanf(pFile, "%d", &nNumModel);					// モデルの最大数を代入
				g_Enemy.Motion.nNumModel = nNumModel;	// モデルの最大数を代入
			}

			// モデルの読み込みがまだ終わっていなかったら
			if (nLoadCnt < nNumModel)
			{
				// モデルの読み込んだ数を返す
				nLoadCnt = LoadFilenameEnemy(pFile, nNumModel, &aStr[0]);
			}

			// CHARACTERSETを読み取ったら
			if (strcmp(&aStr[0], "CHARACTERSET") == 0)
			{
				// パーツの設定処理
				LoadCharacterSetEnemy(pFile, &aStr[0], nNumParts);
			}

			// MOTIONSETを読み取ったら
			if (strcmp(&aStr[0], "MOTIONSET") == 0)
			{
				// モーションの設定処理
				LoadMotionSetEnemy(pFile, &aStr[0], nNumModel);
			}

			// EOFを読み取ったら
			if (nData == EOF)
			{
				// モーションの総数を代入
				g_Enemy.Motion.nNumMotion = nCntMotionEnemy;
				nCntMotionEnemy = 0; // モーションのカウントをリセットする
				break;          // While文を抜ける
			}
		}
	}
	else
	{
		// ファイルが開けなかったら
		MessageBox(NULL, "ファイルが開けません。", "エラー(LoadEnemy)", MB_OK);
		return;
	}

	// ファイルを閉じる
	fclose(pFile);
}
//=================================================
// プレイヤーのモデルロード
//=================================================
int LoadFilenameEnemy(FILE* pFile, int nNumModel, char* aString)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL* pMat;//マテリアルへのポインタ

	//頂点座標の取得
	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマットのサイズ
	BYTE* pVtxBuff;	//頂点バッファへのポインタ

	char Skip[3] = {}; // [=]読み飛ばしよう変数

	int nCntLoadModel = 0; // モデルのロードのカウンター

	// カウントがモデル数より下だったら
	while (nCntLoadModel < nNumModel)
	{
		// 文字を読み取る
		int nData = fscanf(pFile, "%s", aString);

		// MODEL_FILENAMEを読み取ったら
		if (strcmp(aString, "MODEL_FILENAME") == 0)
		{
			nData = fscanf(pFile, "%s", &Skip[0]); // [=]を読み飛ばす
			nData = fscanf(pFile, "%s", aString);  // ファイル名を読み取る

			const char* FILE_NAME = {};    // ファイル名代入用変数

			FILE_NAME = aString;           // ファイル名を代入

			//Xファイルの読み込み
			D3DXLoadMeshFromX(FILE_NAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_Enemy.Motion.aModel[nCntLoadModel].pBuffMat,
				NULL,
				&g_Enemy.Motion.aModel[nCntLoadModel].dwNumMat,
				&g_Enemy.Motion.aModel[nCntLoadModel].pMesh);

			if (g_Enemy.Motion.aModel[nCntLoadModel].pBuffMat == NULL)
			{
				// メッセージボックスの表示
				MessageBox(NULL, FILE_NAME, "モデルが読み込めません", MB_OK | MB_ICONWARNING);

				nCntLoadModel++; // モデルのカウントを増やす
				continue;
			}

			//マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Enemy.Motion.aModel[nCntLoadModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Enemy.Motion.aModel[nCntLoadModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//このファイル名を使用してテクスチャを読み込む
						//テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_Enemy.Motion.aModel[nCntLoadModel].pTexture[nCntMat]);

					// テクスチャが読み込めなかったら
					if (g_Enemy.Motion.aModel[nCntLoadModel].pTexture[nCntMat] == NULL)
					{
						// ファイルネームを保存
						const char* name = pMat[nCntMat].pTextureFilename;

						// メッセージボックスの表示
						MessageBox(NULL, name, "テクスチャが読み込めません", MB_OK | MB_ICONWARNING);
					}
				}
			}

			//頂点数の取得
			nNumVtx = g_Enemy.Motion.aModel[nCntLoadModel].pMesh->GetNumVertices();

			//頂点フォーマットのサイズ取得
			sizeFVF = D3DXGetFVFVertexSize(g_Enemy.Motion.aModel[nCntLoadModel].pMesh->GetFVF());

			//頂点バッファのロック
			g_Enemy.Motion.aModel[nCntLoadModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//頂点座標を比較してブロックの最小値,最大値を取得
				if (vtx.x < g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.x)
				{
					g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.y)
				{
					g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.z)
				{
					g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.x)
				{
					g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.y)
				{
					g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.z)
				{
					g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.z = vtx.z;
				}

				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;
			}

			////サイズを代入
			//g_Enemy.Size.x = g_Enemy.vtxMaxEnemy.x - g_Enemy.vtxMinEnemy.x;
			//g_Enemy.Size.y = g_Enemy.vtxMaxEnemy.y - g_Enemy.vtxMinEnemy.y;
			//g_Enemy.Size.z = g_Enemy.vtxMaxEnemy.z - g_Enemy.vtxMinEnemy.z;

			//各モデルごとのサイズを代入
			g_Enemy.Motion.aModel[nCntLoadModel].Size.x = g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.x - g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.x;
			g_Enemy.Motion.aModel[nCntLoadModel].Size.y = g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.y - g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.y;
			g_Enemy.Motion.aModel[nCntLoadModel].Size.z = g_Enemy.Motion.aModel[nCntLoadModel].vtxMax.z - g_Enemy.Motion.aModel[nCntLoadModel].vtxMin.z;

			//頂点バッファのアンロック
			g_Enemy.Motion.aModel[nCntLoadModel].pMesh->UnlockVertexBuffer();

			nCntLoadModel++; // モデルのカウントを増やす
		}
	}
	return nCntLoadModel; // モデルのカウントを返す

}
//=============================================
// プレイヤーのキャラクターセット
//=============================================
void LoadCharacterSetEnemy(FILE* pFile, char* aString, int nNumparts)
{
	int nIdx = 0;		// インデックス格納変数
	char Skip[3] = {};	// [=]読み飛ばし変数
	int bUpper = 0;		// 上半身か下半身かの判定 

	// END_CHARACTERSETを読み取ってなかったら
	while (strcmp(aString, "END_CHARACTERSET") != 0)
	{
		// 文字を読み取る
		int nData = fscanf(pFile, "%s", aString);

		if (strcmp(aString, "MOVE") == 0)
		{
			nData = fscanf(pFile, "%s", &Skip[0]); // [=]読み飛ばす
			nData = fscanf(pFile, "%f", &g_Enemy.speed);    // スピードを代入
		}

		//if (strcmp(aString, "JUMP") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]読み飛ばす
		//	nData = fscanf(pFile, "%f", &g_Enemy.JumpHeight);    // ジャンプの高さを代入
		//}

		//if (strcmp(aString, "SIZE") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]読み飛ばす
		//	nData = fscanf(pFile, "%f", &g_Enemy.Size.x);    // 大きさを代入
		//	nData = fscanf(pFile, "%f", &g_Enemy.Size.y);    // 大きさを代入
		//	nData = fscanf(pFile, "%f", &g_Enemy.Size.z);    // 大きさを代入
		//}

		//if (strcmp(aString, "BLOWOFF") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]読み飛ばす
		//	nData = fscanf(pFile, "%f", &g_Enemy.BlowMove.x);    // 大きさを代入
		//	nData = fscanf(pFile, "%f", &g_Enemy.BlowMove.y);    // 大きさを代入
		//}

		//if (strcmp(aString, "AVOIDMOVE") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]読み飛ばす
		//	nData = fscanf(pFile, "%f", &g_Enemy.AvoidMove.x);    // 大きさを代入
		//	nData = fscanf(pFile, "%f", &g_Enemy.AvoidMove.y);    // 大きさを代入
		//}

		else if (strcmp(aString, "PART") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// 親のインデックスを保存
			nData = fscanf(pFile, "%d", &bUpper);

			g_Enemy.Motion.aModel[nIdx].bUpperBody = bUpper == 1 ? true : false;
		}

		// PARENTを読み取ったら
		else if (strcmp(aString, "PARENT") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// 親のインデックスを保存
			nData = fscanf(pFile, "%d", &g_Enemy.Motion.aModel[nIdx].nIdxModelParent);
		}
		// POSを読み取ったら
		else if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aModel[nIdx].offpos.x);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aModel[nIdx].offpos.y);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aModel[nIdx].offpos.z);
		}
		// ROTを読み取ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aModel[nIdx].offrot.x);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aModel[nIdx].offrot.y);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aModel[nIdx].offrot.z);
			nIdx++;
		}
	}
}
//=================================================
// プレイヤーのモーションセット
//=================================================
void LoadMotionSetEnemy(FILE* pFile, char* aString, int nNumModel)
{
	char Skip[3] = {}; // [=]読み飛ばし変数
	int Loop = 0;	   // ループするか

	while (1)
	{
		// 文字を読み取る
		int nData = fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0 || strcmp(aString, "<<") == 0)
		{// コメントが来たら
			// コメントを読み飛ばす
			continue;
		}

		// LOOPを読み通ったら
		if (strcmp(aString, "LOOP") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// ループするかしないか
			nData = fscanf(pFile, "%d", &Loop);

			// ループするかどうか
			g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].bLoop = (Loop == 1) ? true : false;
		}
		// NUM_KEYを読み通ったら
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// キーの最大数を代入
			nData = fscanf(pFile, "%d", &g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].nNumkey);
		}
		else if (strcmp(aString, "NUM_FRAME") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// キーの最大数を代入
			nData = fscanf(pFile, "%d", &g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].nNumFrame);
		}
		else if (strcmp(aString, "START_FRAME") == 0)
		{
			int nCntFrame = 0;

			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// 最後まで入れ終わったら
			while (nCntFrame < g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].nNumFrame)
			{
				// キーの最大数を代入
				nData = fscanf(pFile, "%d", &g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].nStartFrame[nCntFrame]);

				nCntFrame++;
			}
		}
		else if (strcmp(aString, "END_FRAME") == 0)
		{
			int nCntFrame = 0;

			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// 最後まで入れ終わったら
			while (nCntFrame < g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].nNumFrame)
			{
				// キーの最大数を代入
				nData = fscanf(pFile, "%d", &g_Enemy.Motion.aMotionInfo[nCntMotionEnemy].nEndFrame[nCntFrame]);

				nCntFrame++;
			}
		}

		// KEYSETを読み通ったら
		if (strcmp(aString, "KEYSET") == 0)
		{
			// キーの設定処理
			LoadKeySetEnemy(pFile, aString, nCntMotionEnemy);
		}
		// END_MOTIONSETを読み通ったら
		if (strcmp(aString, "END_MOTIONSET") == 0)
		{
			nCntMotionEnemy++; // モーションのカウントをリセット
			nKeyEnemy = 0;     // キーをリセット
			break;
		}
	}
}
//============================================
// プレイヤーのモーションのキーセット
//============================================
void LoadKeySetEnemy(FILE* pFile, char* aString, int nCntMotion)
{
	char Skip[3] = {}; // [=]読み飛ばし変数
	int nCntPos = 0;   // 位置のカウント
	int nCntRot = 0;   // 角度のカウント


	while (1)
	{
		// 文字を読み取る
		int nData = fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0)
		{// コメントが来たら
			// コメントを読み飛ばす
			continue;
		}

		// FRAMEを読み通ったら
		if (strcmp(aString, "FRAME") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// フレームを読み込む
			nData = fscanf(pFile, "%d", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].nFrame);
		}

		// POSを読み通ったら
		if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// 位置を読み込む

			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntPos].fPosX);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntPos].fPosY);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROTを読み通ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// 角度を読み込む
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntRot].fRotX);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntRot].fRotY);
			nData = fscanf(pFile, "%f", &g_Enemy.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntRot].fRotZ);
			nCntRot++;
		}
		// END_KEYSETを読み通ったら
		else if (strcmp(aString, "END_KEYSET") == 0)
		{
			nKeyEnemy++;		 // キーを増やす
			nCntPos = 0; // 位置のカウントをリセット
			nCntRot = 0; // 角度のカウントをリセット
			break;
		}
	}
}
//============================================
// プレイヤーを追従する処理
//============================================
void ChasePlayer(Player* pPlayer)
{
	float fDistance = GetDistance(pPlayer->pos, g_Enemy.pos);

	ImGui::Text(u8"距離 = %.1f", fDistance);

	// 距離が近かったら
	if (fDistance <= 100.0f)
	{
		g_Enemy.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		 
		if(g_Enemy.Motion.MotionData[0].motiontypeBlend != MOTIONTYPE_ACTION) g_Enemy.nActionType = ACTIONSTATE_ATTACK;

		return;
	}

	g_Enemy.nActionType = ACTIONSTATE_MOVE;

	// 位置の差分
	D3DXVECTOR3 DiffPos = pPlayer->pos - g_Enemy.pos;

	// 現在の移動方向
	float fRotMove = atan2f(g_Enemy.move.x,g_Enemy.move.z);

	// 敵の向きを保存
	float fAngle = fRotMove;

	// 角度
	float fDestRot = atan2f(DiffPos.x, DiffPos.z);

	// 差分を求める
	float fDIffRot = fDestRot - fRotMove;

	// 角度の補正
	if (fDIffRot < -D3DX_PI)
	{
		fDIffRot += D3DX_PI * 2.0f;
	}

	// 角度の補正
	if (fDIffRot > D3DX_PI)
	{
		fDIffRot += -D3DX_PI * 2.0f;
	}

	// 移動方向をだんだん近づける
	fRotMove += fDIffRot * 0.1f;

	// 目的の角度を入れる
	g_Enemy.rotDestEnemy.y = fAngle + D3DX_PI;

	// 移動量を更新
	g_Enemy.move.x = sinf(fRotMove) * 5.0f;
	g_Enemy.move.z = cosf(fRotMove) * 5.0f;

}
//============================================
// 敵の状態の遷移
//============================================
void TransEnemyState(void)
{
	// 敵の状態の遷移
	switch (g_Enemy.nActionType)
	{
	case ACTONSTATE_NEUTRAL:
		// 移動状態だったら
		if (g_Enemy.Motion.MotionData[0].motiontypeBlend == MOTIONTYPE_MOVE) SetMotion(&g_Enemy.Motion, MOTIONTYPE_NEUTRAL, true, 10, 0);
		break;
	case ACTIONSTATE_MOVE:
		// 移動モーションを開始
		SetMotion(&g_Enemy.Motion, MOTIONTYPE_MOVE, true, 10, 0);
		break;
	case ACTIONSTATE_ATTACK:
		SetMotion(&g_Enemy.Motion, MOTIONTYPE_ACTION, true, 10, 0);
		break;
	case ACTIONSTATE_BACKSTEP:
		SetMotion(&g_Enemy.Motion, MOTIONTYPE_JUMP, true, 10, 0);
		break;
	default:
		break;
	}

}
//============================================
// 敵の攻撃状態の取得
//============================================
bool FinishActionMotion(void)
{
	Motion_Data* pMotionData = &g_Enemy.Motion.MotionData[0];

	// 攻撃状態じゃなかったら
	if (pMotionData->motiontypeBlend != MOTIONTYPE_ACTION && pMotionData->motionType != MOTIONTYPE_ACTION)
	{
		return false;
	}

	int nLastKey = g_Enemy.Motion.aMotionInfo[MOTIONTYPE_ACTION].nNumkey - 1;

	if (pMotionData->nKey >= nLastKey)
	{
		return true;
	}

	return false;
}
//============================================
// アクションモーション終了後の状態遷移
//============================================
void EndActionTransState(void)
{
	int nRandom_Value = rand() % 3;

	switch (nRandom_Value)
	{
	case 0:
		g_Enemy.nActionType = ACTIONSTATE_ATTACK;
		break;
	case 1:
		g_Enemy.nActionType = ACTONSTATE_NEUTRAL;
		break;
	case 2:
		g_Enemy.nActionType = ACTIONSTATE_BACKSTEP;
		break;
	default:
		break;
	}
}

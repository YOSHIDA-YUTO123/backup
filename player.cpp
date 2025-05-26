//===============================================================================================================
//
// プレイヤー[player.cpp]
// Author:YOSHIDA YUUTO
//
//===============================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include "player.h"
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

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define MAX_WORD (256)			// 最大文字数
#define PLAYERLIFE (100)		// プレイヤーの体力
#define MAX_TEXPLAYER (128)		// テクスチャの最大数
#define MAX_JUMP (15.0f)		// ジャンプ量
#define MAX_MOVE (1.0f)			// プレイヤーの移動量
#define NUM_MTX (8)				// 剣の当たり判定のマトリクスの数
#define LANDINGEXPLOSION (6)	// 着地したときに出る煙
#define MAX_JUMP (15.0f)        // ジャンプ量

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void UpdatePlayerMove(void);

void LoadFullBodyPlayer(const char* Filepath);										 // プレイヤーのロード処理
int LoadFilename(FILE* pFile, int nNumModel, char* aString);						 // プレイヤーのモデルのロード処理
void LoadCharacterSet(FILE* pFile, char* aString, int nNumparts);					 // プレイヤーのパーツの設定処理
void LoadMotionSet(FILE* pFile, char* aString, int nNumModel);						 // プレイヤーのモーションのロード処理
void LoadKeySet(FILE* pFile, char* aString, int nCntMotion);						 // プレイヤーのモーションのキーの読み込み処理
void SetFullBodyModel(LPDIRECT3DDEVICE9 pDevice);									 // 親が一つしかない時
void StartNeutralMotion(MOTION* pMotion);											 // ニュートラルモーションの開始
void StartMoveMotion(MOTION* pMotion);												 // 

//**************************************************************************************************************
//グローバル変数宣言
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureImgui = NULL;//プレイヤーのテクスチャへのポインタ
Player g_player;//プレイヤー構造体
ImVec2 blockPos = ImVec2(300, 300);

int g_nCounterState,g_AttackState; // 状態カウンター
int nCntMotion,nKey;

//===============================================================================================================
//プレイヤーの初期化処理
//===============================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	MODE mode = GetMode();//現在のモードを取得

	//プレイヤーの初期化
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);		   // 座標
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   // 角度
	g_player.Size = D3DXVECTOR3(20.0f, 200.0f, 20.0f);	   // プレイヤーの大きさ
	g_player.rotDestPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 目的の角度
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   // 移動量
	g_player.bJump = true;								   // ジャンプ中か否か
	g_player.bDisp = true;								   // 使用状態
	g_player.nLife = PLAYERLIFE;						   // 体力
	g_player.nMaxLife = PLAYERLIFE;
	g_player.state = PLAYERSTATE_NORMAL;				   // 状態
	g_nCounterState = 0;                                   // 状態カウンター
	g_AttackState = 0;									   // 攻撃状態のカウンター
	nCntMotion = 0;
	nKey = 0;

	LoadFullBodyPlayer("data/motion.txt");
}
//===============================================================================================================
//プレイヤーの終了処理
//===============================================================================================================
void UninitPlayer(void)
{
	// テクスチャの破棄
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel; nCntModel++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_player.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (g_player.Motion.aModel[nCntModel].pTexture[nCntMat] != NULL)
			{
				g_player.Motion.aModel[nCntModel].pTexture[nCntMat]->Release();
				g_player.Motion.aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

		//メッシュの破棄
		if (g_player.Motion.aModel[nCntModel].pMesh != NULL)
		{
			g_player.Motion.aModel[nCntModel].pMesh->Release();
			g_player.Motion.aModel[nCntModel].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_player.Motion.aModel[nCntModel].pBuffMat != NULL)
		{
			g_player.Motion.aModel[nCntModel].pBuffMat->Release();
			g_player.Motion.aModel[nCntModel].pBuffMat = NULL;
		}
	}

	if (g_apTextureImgui != NULL)
	{
		g_apTextureImgui->Release();
		g_apTextureImgui = NULL;
	}
}
//===============================================================================================================
//プレイヤーの更新処理
//===============================================================================================================
void UpdatePlayer(void)
{
	// プレイヤーの移動処理
	UpdatePlayerMove();

	// メッシュフィールドの当たり判定
	if (CollisionField(&g_player.pos))
	{
		g_player.bJump = true;
	}
	else
	{
		g_player.bJump = false;
	}

	if (KeyboardTrigger(DIK_SPACE))
	{
		if (g_player.bJump == true)
		{
			g_player.move.y = 15.0f;
			g_player.bJump = false;
		}
	}

	ImGui::DragInt(u8"Hp", &g_player.nLife, 1.0f, 0, g_player.nMaxLife);
	// 重力
	g_player.move.y -= MAX_GLABITY;
	

	//プレイヤーの角度の正規化
	if (g_player.rotDestPlayer.y - g_player.rot.y >= D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDestPlayer.y - g_player.rot.y <= -D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	// モーションの更新処理
	UpdateMotion(&g_player.Motion, g_player.bFullBody);

	if (GetKeyboardPress(DIK_LSHIFT))
	{
		CollisionRayToSphere(g_player.pos,&g_player.move, &g_player.pos, 50.0f);
	}

	//プレイヤーの向きを目的の向きに近づける
	g_player.rot.y += (g_player.rotDestPlayer.y - g_player.rot.y) * 0.1f;
}
//===============================================================================================================
// プレイヤーの描画処理
//===============================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize, mtxShadow;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	int Parent = 0;

	// テクスチャの破棄
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel; nCntModel++)
	{
		if (g_player.Motion.aModel[nCntModel].nIdxModelParent == -1)
		{
			Parent++;
		}
	}

	g_player.bFullBody = (Parent == 2) ? false : true;

	// 全身か全身じゃないか
	const bool isFullBody = g_player.bFullBody == true;

	if (g_player.bDisp == true)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.mtxWorldPlayer);

		Draw3DLine(g_player.pos,SetMtxConversion(g_player.Motion.aModel[2].mtxWorld), COLOR_RED);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
		D3DXMatrixMultiply(&g_player.mtxWorldPlayer, &g_player.mtxWorldPlayer, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
		D3DXMatrixMultiply(&g_player.mtxWorldPlayer, &g_player.mtxWorldPlayer, &mtxTrans);

		//D3DXMatrixShadow(&mtxShadow, &Light, &plane);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorldPlayer);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// 親が一つしかない時
		SetFullBodyModel(pDevice);
	}
	//マテリアルの設定
	pDevice->SetMaterial(&matDef);
}
//===============================================================================================================
//プレイヤーの取得処理
//===============================================================================================================
Player* GetPlayer(void)
{
	return &g_player;
}
//===============================================================================================================
// プレイヤーの移動処理
//===============================================================================================================
void UpdatePlayerMove(void)
{
	Camera* pCamera = GetCamera();

	// プレイヤーの移動速度
	float fSpeed = 5.0f;

	if (GetKeyboardPress(DIK_A))
	{
		//プレイヤーの移動(上)
		if (GetKeyboardPress(DIK_W) == true)
		{
			// 移動モーションの開始
			StartMoveMotion(&g_player.Motion);

			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.25f) * fSpeed;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.25f) * fSpeed;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.75f;
		}
		//プレイヤーの移動(下)
		else if (GetKeyboardPress(DIK_S))
		{
			// 移動モーションの開始
			StartMoveMotion(&g_player.Motion);

			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.75f) * fSpeed;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.75f) * fSpeed;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.25f;
		}
		//プレイヤーの移動(左)
		else
		{
			// 移動モーションの開始
			StartMoveMotion(&g_player.Motion);

			g_player.move.z += sinf(pCamera->rot.y) * fSpeed;
			g_player.move.x -= cosf(pCamera->rot.y) * fSpeed;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.5f;
		}
	}
	//プレイヤーの移動(右)
	else if (GetKeyboardPress(DIK_D))
	{
		// 移動モーションの開始
		StartMoveMotion(&g_player.Motion);

		//プレイヤーの移動(上)
		if (GetKeyboardPress(DIK_W))
		{
			// 移動モーションの開始
			StartMoveMotion(&g_player.Motion);

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * fSpeed;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * fSpeed;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.75f;
		}
		//プレイヤーの移動(下)
		else if (GetKeyboardPress(DIK_S))
		{
			// 移動モーションの開始
			StartMoveMotion(&g_player.Motion);

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * fSpeed;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * fSpeed;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.25f;
		}
		//プレイヤーの移動(右)
		else
		{
			// 移動モーションの開始
			StartMoveMotion(&g_player.Motion);

			g_player.move.z -= sinf(pCamera->rot.y) * fSpeed;
			g_player.move.x += cosf(pCamera->rot.y) * fSpeed;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.5f;
		}

	}
	//プレイヤーの移動(上)
	else if (GetKeyboardPress(DIK_W) == true)
	{
		// 移動モーションの開始
		StartMoveMotion(&g_player.Motion);

		g_player.move.x += sinf(pCamera->rot.y) * fSpeed;
		g_player.move.z += cosf(pCamera->rot.y) * fSpeed;

		g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI;
	}
	//プレイヤーの移動(下)
	else if (GetKeyboardPress(DIK_S) == true)
	{
		// 移動モーションの開始
		StartMoveMotion(&g_player.Motion);

		g_player.move.x -= sinf(pCamera->rot.y) * fSpeed;
		g_player.move.z -= cosf(pCamera->rot.y) * fSpeed;

		g_player.rotDestPlayer.y = pCamera->rot.y;
	}
	else
	{
		StartNeutralMotion(&g_player.Motion);
	}
	// プレイヤーの移動量の減衰
	g_player.move.x += (0.0f - g_player.move.x) * 0.2f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.2f;

	// 前回の位置の保存
	g_player.posOld = g_player.pos;

	// 位置の更新
	g_player.pos += g_player.move;
}
//===========================================
// デバッグ用Imgui
//===========================================
void DebugImgui(void)
{
	StartImgui("Debug", IMGUITYPE_DEFOULT);

	SetPosImgui(ImVec2(0.0f, 0.0f));
	SetSizeImgui(ImVec2(400.0f, 500.0f));

	ImGui::DragFloat3(u8"回転", g_player.rot, 0.1f, -D3DX_PI, D3DX_PI);

	ImGui::Text(u8"行列 11 = %.4f 21 = %.4f 31 = %.4f", g_player.mtxWorldPlayer._11, g_player.mtxWorldPlayer._21, g_player.mtxWorldPlayer._31);
	ImGui::Text(u8"行列 12 = %.4f 22 = %.4f 32 = %.4f", g_player.mtxWorldPlayer._12, g_player.mtxWorldPlayer._22, g_player.mtxWorldPlayer._32);
	ImGui::Text(u8"行列 13 = %.4f 23 = %.4f 33 = %.4f", g_player.mtxWorldPlayer._13, g_player.mtxWorldPlayer._23, g_player.mtxWorldPlayer._33);
	ImGui::Text(u8"行列 41 = %.4f 42 = %.4f 43 = %.4f", g_player.mtxWorldPlayer._41, g_player.mtxWorldPlayer._42, g_player.mtxWorldPlayer._43);

	ImGui::Text(u8"プレイヤーの位置 X = %.1f,Y = %.1f,Z = %.1f",g_player.pos.x, g_player.pos.y, g_player.pos.z);

	ImTextureID texture = (ImTextureID)(intptr_t)g_apTextureImgui;

	// テクスチャIDを渡し、ボタンのサイズを指定
	if (ImGui::ImageButton("あいうえお", texture, ImVec2(128, 128)))
	{
		ImVec2 mousePos = ImGui::GetMousePos();
		blockPos = mousePos; // 画像がクリックされた位置にブロックを配置
	}

	// ドラッグ中の処理
	if (ImGui::IsMouseDragging(0))
	{
		ImVec2 mousePos = ImGui::GetMousePos();
		blockPos = mousePos; // ドラッグ中にブロックの位置を更新
	}

	static D3DXCOLOR col(1.0f,1.0,1.0f,1.0f);

	// 色選択パネルを表示（RGBA）
	ImGui::ColorEdit4("Pick a color", col);  // 色を選択できるパネルを表示

		// ワイヤーフレームにするかしないか
	static bool bWireFrame = false;

	// カメラ追従の設定
	if (ImGui::Checkbox(u8"ワイヤーフレーム", &bWireFrame))
	{
		if (bWireFrame)
		{
			onWireFrame(); // ワイヤーフレームON
		}
		else
		{
			offWireFrame(); // ワイヤーフレームOFF
		}
	}


	ImGui::End();

}
//============================================
// プレイヤーのロード処理
//============================================
void LoadFullBodyPlayer(const char* Filepath)
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
				g_player.Motion.nNumModel = nNumModel;	// モデルの最大数を代入
			}

			// モデルの読み込みがまだ終わっていなかったら
			if (nLoadCnt < nNumModel)
			{
				// モデルの読み込んだ数を返す
				nLoadCnt = LoadFilename(pFile, nNumModel, &aStr[0]);
			}

			// CHARACTERSETを読み取ったら
			if (strcmp(&aStr[0], "CHARACTERSET") == 0)
			{
				// パーツの設定処理
				LoadCharacterSet(pFile, &aStr[0], nNumParts);
			}

			// MOTIONSETを読み取ったら
			if (strcmp(&aStr[0], "MOTIONSET") == 0)
			{
				// モーションの設定処理
				LoadMotionSet(pFile, &aStr[0], nNumModel);
			}

			// EOFを読み取ったら
			if (nData == EOF)
			{
				// モーションの総数を代入
				g_player.Motion.nNumMotion = nCntMotion;
				nCntMotion = 0; // モーションのカウントをリセットする
				break;          // While文を抜ける
			}
		}
	}
	else
	{
		// ファイルが開けなかったら
		MessageBox(NULL, "ファイルが開けません。", "エラー(LoadPlayer)", MB_OK);
		return;
	}

	// ファイルを閉じる
	fclose(pFile);
}
//=================================================
// プレイヤーのモデルロード
//=================================================
int LoadFilename(FILE* pFile, int nNumModel, char* aString)
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
				&g_player.Motion.aModel[nCntLoadModel].pBuffMat,
				NULL,
				&g_player.Motion.aModel[nCntLoadModel].dwNumMat,
				&g_player.Motion.aModel[nCntLoadModel].pMesh);

			if (g_player.Motion.aModel[nCntLoadModel].pBuffMat == NULL)
			{
				// メッセージボックスの表示
				MessageBox(NULL, FILE_NAME, "モデルが読み込めません", MB_OK | MB_ICONWARNING);

				nCntLoadModel++; // モデルのカウントを増やす
				continue;
			}

			//マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_player.Motion.aModel[nCntLoadModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_player.Motion.aModel[nCntLoadModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//このファイル名を使用してテクスチャを読み込む
						//テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_player.Motion.aModel[nCntLoadModel].pTexture[nCntMat]);

					// テクスチャが読み込めなかったら
					if (g_player.Motion.aModel[nCntLoadModel].pTexture[nCntMat] == NULL)
					{
						// ファイルネームを保存
						const char* name = pMat[nCntMat].pTextureFilename;

						// メッセージボックスの表示
						MessageBox(NULL, name, "テクスチャが読み込めません", MB_OK | MB_ICONWARNING);
					}
				}
			}

			//頂点数の取得
			nNumVtx = g_player.Motion.aModel[nCntLoadModel].pMesh->GetNumVertices();

			//頂点フォーマットのサイズ取得
			sizeFVF = D3DXGetFVFVertexSize(g_player.Motion.aModel[nCntLoadModel].pMesh->GetFVF());

			//頂点バッファのロック
			g_player.Motion.aModel[nCntLoadModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//頂点座標を比較してブロックの最小値,最大値を取得
				if (vtx.x < g_player.Motion.aModel[nCntLoadModel].vtxMin.x)
				{
					g_player.Motion.aModel[nCntLoadModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_player.Motion.aModel[nCntLoadModel].vtxMin.y)
				{
					g_player.Motion.aModel[nCntLoadModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_player.Motion.aModel[nCntLoadModel].vtxMin.z)
				{
					g_player.Motion.aModel[nCntLoadModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_player.Motion.aModel[nCntLoadModel].vtxMax.x)
				{
					g_player.Motion.aModel[nCntLoadModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_player.Motion.aModel[nCntLoadModel].vtxMax.y)
				{
					g_player.Motion.aModel[nCntLoadModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_player.Motion.aModel[nCntLoadModel].vtxMax.z)
				{
					g_player.Motion.aModel[nCntLoadModel].vtxMax.z = vtx.z;
				}

				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;
			}

			////サイズを代入
			//g_player.Size.x = g_player.vtxMaxPlayer.x - g_player.vtxMinPlayer.x;
			//g_player.Size.y = g_player.vtxMaxPlayer.y - g_player.vtxMinPlayer.y;
			//g_player.Size.z = g_player.vtxMaxPlayer.z - g_player.vtxMinPlayer.z;

			//各モデルごとのサイズを代入
			g_player.Motion.aModel[nCntLoadModel].Size.x = g_player.Motion.aModel[nCntLoadModel].vtxMax.x - g_player.Motion.aModel[nCntLoadModel].vtxMin.x;
			g_player.Motion.aModel[nCntLoadModel].Size.y = g_player.Motion.aModel[nCntLoadModel].vtxMax.y - g_player.Motion.aModel[nCntLoadModel].vtxMin.y;
			g_player.Motion.aModel[nCntLoadModel].Size.z = g_player.Motion.aModel[nCntLoadModel].vtxMax.z - g_player.Motion.aModel[nCntLoadModel].vtxMin.z;

			//頂点バッファのアンロック
			g_player.Motion.aModel[nCntLoadModel].pMesh->UnlockVertexBuffer();

			nCntLoadModel++; // モデルのカウントを増やす
		}
	}
	return nCntLoadModel; // モデルのカウントを返す

}
//=============================================
// プレイヤーのキャラクターセット
//=============================================
void LoadCharacterSet(FILE* pFile, char* aString, int nNumparts)
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
			nData = fscanf(pFile, "%f", &g_player.speed);    // スピードを代入
		}

		//if (strcmp(aString, "JUMP") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]読み飛ばす
		//	nData = fscanf(pFile, "%f", &g_player.JumpHeight);    // ジャンプの高さを代入
		//}

		//if (strcmp(aString, "SIZE") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]読み飛ばす
		//	nData = fscanf(pFile, "%f", &g_player.Size.x);    // 大きさを代入
		//	nData = fscanf(pFile, "%f", &g_player.Size.y);    // 大きさを代入
		//	nData = fscanf(pFile, "%f", &g_player.Size.z);    // 大きさを代入
		//}

		//if (strcmp(aString, "BLOWOFF") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]読み飛ばす
		//	nData = fscanf(pFile, "%f", &g_player.BlowMove.x);    // 大きさを代入
		//	nData = fscanf(pFile, "%f", &g_player.BlowMove.y);    // 大きさを代入
		//}

		//if (strcmp(aString, "AVOIDMOVE") == 0)
		//{
		//	nData = fscanf(pFile, "%s", &Skip[0]); // [=]読み飛ばす
		//	nData = fscanf(pFile, "%f", &g_player.AvoidMove.x);    // 大きさを代入
		//	nData = fscanf(pFile, "%f", &g_player.AvoidMove.y);    // 大きさを代入
		//}

		else if (strcmp(aString, "PART") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// 親のインデックスを保存
			nData = fscanf(pFile, "%d", &bUpper);

			g_player.Motion.aModel[nIdx].bUpperBody = bUpper == 1 ? true : false;
		}

		// PARENTを読み取ったら
		else if (strcmp(aString, "PARENT") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// 親のインデックスを保存
			nData = fscanf(pFile, "%d", &g_player.Motion.aModel[nIdx].nIdxModelParent);
		}
		// POSを読み取ったら
		else if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			nData = fscanf(pFile, "%f", &g_player.Motion.aModel[nIdx].offpos.x);
			nData = fscanf(pFile, "%f", &g_player.Motion.aModel[nIdx].offpos.y);
			nData = fscanf(pFile, "%f", &g_player.Motion.aModel[nIdx].offpos.z);
		}
		// ROTを読み取ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			nData = fscanf(pFile, "%f", &g_player.Motion.aModel[nIdx].offrot.x);
			nData = fscanf(pFile, "%f", &g_player.Motion.aModel[nIdx].offrot.y);
			nData = fscanf(pFile, "%f", &g_player.Motion.aModel[nIdx].offrot.z);
			nIdx++;
		}
	}
}
//=================================================
// プレイヤーのモーションセット
//=================================================
void LoadMotionSet(FILE* pFile, char* aString, int nNumModel)
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
			g_player.Motion.aMotionInfo[nCntMotion].bLoop = (Loop == 1) ? true : false;
		}
		// NUM_KEYを読み通ったら
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// キーの最大数を代入
			nData = fscanf(pFile, "%d", &g_player.Motion.aMotionInfo[nCntMotion].nNumkey);
		}
		else if (strcmp(aString, "NUM_FRAME") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// キーの最大数を代入
			nData = fscanf(pFile, "%d", &g_player.Motion.aMotionInfo[nCntMotion].nNumFrame);
		}
		else if (strcmp(aString, "START_FRAME") == 0)
		{
			int nCntFrame = 0;

			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// 最後まで入れ終わったら
			while (nCntFrame < g_player.Motion.aMotionInfo[nCntMotion].nNumFrame)
			{
				// キーの最大数を代入
				nData = fscanf(pFile, "%d", &g_player.Motion.aMotionInfo[nCntMotion].nStartFrame[nCntFrame]);

				nCntFrame++;
			}
		}
		else if (strcmp(aString, "END_FRAME") == 0)
		{
			int nCntFrame = 0;

			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// 最後まで入れ終わったら
			while (nCntFrame < g_player.Motion.aMotionInfo[nCntMotion].nNumFrame)
			{
				// キーの最大数を代入
				nData = fscanf(pFile, "%d", &g_player.Motion.aMotionInfo[nCntMotion].nEndFrame[nCntFrame]);

				nCntFrame++;
			}
		}

		// KEYSETを読み通ったら
		if (strcmp(aString, "KEYSET") == 0)
		{
			// キーの設定処理
			LoadKeySet(pFile, aString, nCntMotion);
		}
		// END_MOTIONSETを読み通ったら
		if (strcmp(aString, "END_MOTIONSET") == 0)
		{
			nCntMotion++; // モーションのカウントをリセット
			nKey = 0;     // キーをリセット
			break;
		}
	}
}
//============================================
// プレイヤーのモーションのキーセット
//============================================
void LoadKeySet(FILE* pFile, char* aString, int nCntMotion)
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
			nData = fscanf(pFile, "%d", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].nFrame);
		}

		// POSを読み通ったら
		if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// 位置を読み込む

			nData = fscanf(pFile, "%f", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosX);
			nData = fscanf(pFile, "%f", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosY);
			nData = fscanf(pFile, "%f", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROTを読み通ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			nData = fscanf(pFile, "%s", &Skip[0]);

			// 角度を読み込む
			nData = fscanf(pFile, "%f", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotX);
			nData = fscanf(pFile, "%f", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotY);
			nData = fscanf(pFile, "%f", &g_player.Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotZ);
			nCntRot++;
		}
		// END_KEYSETを読み通ったら
		else if (strcmp(aString, "END_KEYSET") == 0)
		{
			nKey++;		 // キーを増やす
			nCntPos = 0; // 位置のカウントをリセット
			nCntRot = 0; // 角度のカウントをリセット
			break;
		}
	}
}
//=================================================
// 親が一つしかない時
//=================================================
void SetFullBodyModel(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//全モデルパーツの描画
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransform;//計算用
		D3DXMATRIX mtxParent;//親のマトリックス

		//パーツのマトリックスの初期化
		D3DXMatrixIdentity(&g_player.Motion.aModel[nCntModel].mtxWorld);

		//パーツの向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.Motion.aModel[nCntModel].rot.y, g_player.Motion.aModel[nCntModel].rot.x, g_player.Motion.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_player.Motion.aModel[nCntModel].mtxWorld, &g_player.Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

		//パーツの位置(オフセット)を反映
		D3DXMatrixTranslation(&mtxTransform, g_player.Motion.aModel[nCntModel].pos.x, g_player.Motion.aModel[nCntModel].pos.y, g_player.Motion.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_player.Motion.aModel[nCntModel].mtxWorld, &g_player.Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

		//パーツの[親のマトリックス]を設定
		if (g_player.Motion.aModel[nCntModel].nIdxModelParent != -1)
		{
			//親モデルがある場合
			mtxParent = g_player.Motion.aModel[g_player.Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{//親モデルがない場合
			mtxParent = g_player.mtxWorldPlayer;
		}

		//算出した[パーツのワールドマトリックス]と[親のマトリックス]をかけあわせる
		D3DXMatrixMultiply(&g_player.Motion.aModel[nCntModel].mtxWorld,
			&g_player.Motion.aModel[nCntModel].mtxWorld,
			&mtxParent);//自分自分親

		//パーツのワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD,
			&g_player.Motion.aModel[nCntModel].mtxWorld);


		for (int nCntMat = 0; nCntMat < (int)g_player.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			//マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_player.Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

			// モデルの情報がなかったら
			if (g_player.Motion.aModel[nCntModel].pBuffMat == NULL)
			{
				continue;
			}

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_player.Motion.aModel[nCntModel].pTexture[nCntMat]);

			//モデル(パーツ)の描画
			g_player.Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
		}

		//// 剣になったら
		//if (nCntModel == g_player.nOrbitModel)
		//{
		//	SetMtx();
		//}
	}
}
//=================================================
// ニュートラルモーションの開始
//=================================================
void StartNeutralMotion(MOTION* pMotion)
{
	// モーションのパート分回す
	for (int nCnt = 0; nCnt < MOTION_PART_MAX; nCnt++)
	{
		if (g_player.bFullBody == true && nCnt == 1) break;

		if (pMotion->MotionData[nCnt].motiontypeBlend == MOTIONTYPE_MOVE)
		{
			SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, true, 5, nCnt);
		}
	}
}
//=================================================
// 移動モーションの開始
//=================================================
void StartMoveMotion(MOTION* pMotion)
{
	// モーションのパート分回す
	for (int nCnt = 0; nCnt < MOTION_PART_MAX; nCnt++)
	{
		if (g_player.bFullBody == true && nCnt == 1) break;

		// モーションの設定
		SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 10, nCnt);	
	}
}

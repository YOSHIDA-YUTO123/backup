//==============================================================================================================
//
// math [math.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

//*******************************************************************************************************************
// インクルードファイル宣言
//*******************************************************************************************************************
#include "math.h"
#include "player.h"
#include"input.h"

//==============================================================================================================
// だんだん目標の値に近づける関数
//==============================================================================================================
float SetSmoothAprroach(float fDestValue, float Value, float coefficient)
{
	// だんだん目標の値に近づける
	float OutValue = (fDestValue - Value) * coefficient;
	return OutValue;
}
//===============================================================================================================
// 円の判定
//===============================================================================================================
bool sphererange(D3DXVECTOR3* pPos1, D3DXVECTOR3* pPos2, float radius1, float radius2) // 円の当たり判定
{
	bool bRange = false;

	D3DXVECTOR3 DiffPos; // 計算用

	// 距離XYZを求める
	DiffPos.x = pPos1->x - pPos2->x;
	DiffPos.y = pPos1->y - pPos2->y;
	DiffPos.z = pPos1->z - pPos2->z;

	// 距離を求める
	float fDistance = (DiffPos.x * DiffPos.x) + (DiffPos.y * DiffPos.y) + (DiffPos.z * DiffPos.z);

	// 半径を求める
	float fRadius = radius1 + radius2;

	// 半径を求める
	fRadius = fRadius * fRadius;

	if (fDistance <= fRadius)
	{
		bRange = true;
	}
	return bRange;
}
//=====================================================================================================
// ワールドマトリックスをD3DXVECTOR3に変換する
//=====================================================================================================
D3DXVECTOR3 SetMtxConversion(D3DXMATRIX mtxworld)
{
	// XYZに変換
	D3DXVECTOR3 OutPut(mtxworld._41, mtxworld._42, mtxworld._43);

	// 変換した値を返す
	return OutPut;
}
//=====================================================================================================
// 目標までのベクトルを計算する
//=====================================================================================================
D3DXVECTOR3 SetSubtractVectors(D3DXVECTOR3 targetpos, D3DXVECTOR3 pos)
{
	D3DXVECTOR3 OutPut = targetpos - pos;

	return OutPut;
}
//==============================================================================================================
// 距離を計算する関数
//==============================================================================================================
float GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	// 差分を求める
	D3DXVECTOR3 DiffPos = pos1 - pos2;

	// 距離を計算
	float OutDistance = sqrtf((DiffPos.x * DiffPos.x) + (DiffPos.y * DiffPos.y) + (DiffPos.z * DiffPos.z));

	// 求めた距離を返す
	return OutDistance;
}
//==============================================================================================================
// いちばん近い敵を求める
//==============================================================================================================
int FindNearEnemy(D3DXVECTOR3* enemies, int nMaxEnemies,bool *bUse)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// 近い敵のインデックス
	int nIdx = -1;

	// 距離
	float fDistance = 0.0f;

	// 距離保存用変数
	float fDistanceStock = FLT_MAX;

	// 敵の最大数分回す
	for (int nCnt = 0; nCnt < nMaxEnemies; nCnt++, enemies++, bUse++)
	{
		// つかわれているなら
		if (*bUse == false) continue;

		// 距離を求める
		fDistance = GetDistance(pPlayer->pos, *enemies);

		// 距離が保存してた距離より短かったら
		if (fDistance < fDistanceStock)
		{
			// 距離を保存
			fDistanceStock = fDistance;

			// インデックスを保存
			nIdx = nCnt;
		}
	}

	// インデックスを返す
	return nIdx;
}
//=====================================================================================================
// 長押しして反応するまでの時間
//=====================================================================================================
bool CheckLongPress(int time, int nKey)
{
	// 押してる時間(キーの数だけ用意)
	static int nPressTime[256] = { 0 };

	// 判定を返す変数
	bool bFlag = false;

	// キーボードを長押ししていたら
	if (GetKeyboardPress(nKey) == true)
	{
		nPressTime[nKey]++;

		if (time <= nPressTime[nKey] || nPressTime[nKey] == 1)
		{
			bFlag = true;
		}
	}
	else
	{
		// 離したらリセット
		nPressTime[nKey] = 0;
	}
	return bFlag;
}
//=====================================================================================================
// ファイル保存をダイアログ表示する
//=====================================================================================================
bool ShowSaveFileDialog(char* filePath, size_t maxLength)
{
	OPENFILENAME ofn;	// ダイアログボックス構造体

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);

	ofn.hwndOwner = GetActiveWindow();

	ofn.lpstrFile = filePath;

	ofn.nMaxFile = (DWORD)maxLength;
	ofn.lpstrFilter = "テキストファイル\0*.txt\0すべてのファイル\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrDefExt = "txt";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	// ダイアログボックスを表示し、保存先ファイルパスを取得
	return GetSaveFileName(&ofn) == TRUE;
}
//=====================================================================================================
// ファイル読み込みをダイアログ表示する
//=====================================================================================================
bool ShowOpenFileDialog(char* filePath, size_t max_length)
{
	OPENFILENAME ofn;    // ダイアログボックス構造体
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = (DWORD)max_length;
	ofn.lpstrFilter = "テキストファイル\0*.txt\0すべてのファイル\0*.*\0";
	ofn.lpstrFile[0] = '\0';   // 初期化
	ofn.lpstrDefExt = "txt";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// ダイアログボックスを表示し、選択されたファイルパスを取得
	return GetOpenFileName(&ofn) == TRUE;
}
//=====================================================================================================
// カプセルとカプセルの当たり判定
//=====================================================================================================
bool CollisionCapsule(D3DXVECTOR3 Start1,D3DXVECTOR3 End1,D3DXVECTOR3 Start2, D3DXVECTOR3 End2,float radius1,float radius2)
{
	//D3DXVECTOR3 player_head = SetMtxConversion(pPlayer->Motion.aModel[2].mtxWorld);
	//D3DXVECTOR3 Enemy_head = SetMtxConversion(g_Enemy.Motion.aModel[2].mtxWorld);

	D3DXVECTOR3 Player_dir = Start1 - End1;
	D3DXVECTOR3 Enemy_dir = Start2 - End2;

	// 両直線の始点間ベクトル
	D3DXVECTOR3 head_to_head = End1 - End2;

	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &Player_dir, &Enemy_dir);
	float fcross_squard = D3DXVec3Dot(&Cross, &Cross);

	D3DXVECTOR3 closeset_on_player, closeset_on_enemy;

	float ft_player = 0.0f;
	float ft_enemy = 0.0f;
	float fDistance = 0.0f;

	if (fcross_squard < 0.00001f)
	{
		closeset_on_player = Start1;
		fDistance = LineDistance(Start1,End1, Start2);
	}
	else
	{
		float dot_p1e1 = D3DXVec3Dot(&Player_dir, &Enemy_dir);
		float dot_p1p1 = D3DXVec3Dot(&Player_dir, &Player_dir);
		float dot_e1e1 = D3DXVec3Dot(&Enemy_dir, &Enemy_dir);

		float dot_p1d = D3DXVec3Dot(&Player_dir, &head_to_head);
		float dot_e1d = D3DXVec3Dot(&Enemy_dir, &head_to_head);

		float denom = dot_p1p1 * dot_e1e1 - dot_p1e1 * dot_p1e1;

		ft_player = (dot_p1e1 * dot_e1d - dot_e1e1 * dot_p1d) / denom;
		ft_enemy = (dot_p1p1 * dot_e1d - dot_p1e1 * dot_p1d) / denom;

		ft_player = max(0.0f, min(1.0f, ft_player));
		ft_enemy = max(0.0f, min(1.0f, ft_enemy));

		closeset_on_player = End1 + Player_dir * ft_player;
		closeset_on_enemy = End2 + Enemy_dir * ft_enemy;

		fDistance = GetDistance(closeset_on_player, closeset_on_enemy);
	}

	float fRadius = radius1 + radius2;

	if (fDistance <= fRadius)
	{
		return true;
	}

	return false;
}
//============================================
// 線分との最短距離
//============================================
float LineDistance(D3DXVECTOR3 Start,D3DXVECTOR3 End,D3DXVECTOR3 Point)
{
	// ベクトルの長さを1とした時のスタートから終点までの長さ
	float fDot = 0.0f;

	D3DXVECTOR3 PlayerLine = Start - End;

	float Linedot = D3DXVec3Dot(&PlayerLine, &PlayerLine);

	if (Linedot > 0.0f)
	{
		D3DXVECTOR3 sp = Point - End;

		fDot = D3DXVec3Dot(&PlayerLine, &sp) / Linedot;
	}

	D3DXVECTOR3 mp = End + PlayerLine * fDot;

	float fDistance = GetDistance(Point, mp);

	// 直線の外だったら(始点より)
	if (fDot < 0.0f)
	{
		mp = Start;
		fDistance = GetDistance(Point, mp);

		return fDistance;
	}
	else if (fDot > 0.0f)
	{
		mp = End;
		fDistance = GetDistance(Point, mp);

		return fDistance;
	}

	return fDistance;
}
//=====================================================================================================
// エフェクトのテクスチャアニメーション
//=====================================================================================================
void SetTextureAnimation(int pPosX,int pPosY,int pAnimSpeed,int *pCounterAnim,int *pPatternAnim, VERTEX_2D *pVtx, LPDIRECT3DVERTEXBUFFER9 pVtxBuff,int nCnt)
{
	//頂点バッファをロック
	if (FAILED(pVtxBuff->Lock(0, 0, (void**)&pVtx, 0)))
	{
		// エラーが起きたら関数を抜ける
		return;
	}

	// アニメーションカウンターを加算
	(*pCounterAnim)++;

	// テクスチャ座標を計算
	float Uv = 1.0f / pPosX;
	float Hv = 1.0f / pPosY;


	// アニメーションのスピード
	if (*pCounterAnim >= pAnimSpeed)
	{
		*pCounterAnim = 0;//カウンターを初期値に戻す

		(*pPatternAnim)++;//パターンナンバーを更新

		pVtx += 4 * nCnt;

		// テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(*pPatternAnim * Uv, (*pPatternAnim / pPosX) * Hv);
		pVtx[1].tex = D3DXVECTOR2(Uv + *pPatternAnim * Uv, (*pPatternAnim / pPosX) * Hv);
		pVtx[2].tex = D3DXVECTOR2(*pPatternAnim * Uv, (*pPatternAnim / pPosX) * Hv + Hv);
		pVtx[3].tex = D3DXVECTOR2(Uv + *pPatternAnim * Uv, (*pPatternAnim / pPosX) * Hv + Hv);

		// パターンが最大になったら
		if (*pPatternAnim >= (pPosX * pPosY))
		{
			*pPatternAnim = 0;//パターンナンバーを初期値に戻す
		}
	}

	// 頂点バッファのアンロック
	pVtxBuff->Unlock();
}
//============================================
// レイと円の判定
//============================================
bool CollisionRayToSphere(D3DXVECTOR3 SenterPos, D3DXVECTOR3* pMove, D3DXVECTOR3* DragPos, float fRadius)
{
	// レイの始点と終点
	D3DXVECTOR3 rayOrigin, rayDir;

	// マウスの座標
	POINT pt;

	// マウスの座標の取得
	GetCursorPos(&pt);

	// マウスの座標をクライアント座標に変換
	ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &pt);

	// マウスを3D座標に変換してレイを飛ばす
	GetRayFromMouse(pt.x, pt.y, &rayOrigin, &rayDir);

	// レイの原点～円の中心までのベクトルを求める
	D3DXVECTOR3 SenterVec = SenterPos - rayOrigin;

	// どれだけ離れているかを内積で求める
	float dot = SenterVec.x * rayDir.x + SenterVec.y * rayDir.y + SenterVec.z * rayDir.z;

	// 最短の点を求める
	D3DXVECTOR3 NearPos = rayOrigin + rayDir * dot;

	// 最短の距離を求める
	D3DXVECTOR3 Diff = NearPos - SenterPos;

	// 長さに変換
	float distance = D3DXVec3Length(&Diff);

	// 半径
	float radius = fRadius + fRadius;

	// レイの距離が半径よりも大きかったら
	if (distance >= radius)
	{
		// 当たっていない
		return false;
	}

	// 当たった点を求める
	D3DXVECTOR3 HitPos = rayOrigin + rayDir * dot;

	// 当たった対象の位置をマウスの座標にする
	DragPos->x = HitPos.x;
	DragPos->z = HitPos.z;

	// 移動量をリセットする
	*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 当たっている
	return true;
}
//============================================
// 放物軌道を描いて落下地点に到達する処理
//============================================
D3DXVECTOR3 ComputeThrowVelocity(const D3DXVECTOR3 dest,const D3DXVECTOR3 start,const float gravity,const float time)
{
	// 初速
	D3DXVECTOR3 velocity;

	D3DXVECTOR3 DiffPos = dest - start;

	// 一フレームどのくらいうごくかを計算
	velocity.x = DiffPos.x / time;
	velocity.z = DiffPos.z / time;

	// 重力による落下距離の影響(0.5f * -g * time * time)
	velocity.y = (DiffPos.y - 0.5f * -gravity * time * time) / time;

	// 値を返す
	return velocity;
}

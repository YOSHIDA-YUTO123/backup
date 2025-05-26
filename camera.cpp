//============================================================================================================
//
// カメラ処理 [camera.cpp]
// Author: TEAM_C
// 
//============================================================================================================

//**************************************************************************************************************
// インクルードファイル宣言
//**************************************************************************************************************
#include "camera.h"
#include "input.h"
#include "mouse.h"
#include "player.h"
#include"math.h"
#include "quaternion.h"

//***************************************************************************************************************
// マクロ定義
//***************************************************************************************************************
#define MAX_VIEWUP (2.90f) // カメラの制限
#define MAX_VIEWDOWN (0.1f) // カメラの制限

//***************************************************************************************************************
// プロトタイプ宣言
//***************************************************************************************************************
void MouseView(void);       // ゲームの時のマウスの視点移動
void MouseEditMode(void); // 編集モードの時のマウス移動

//***************************************************************************************************************
// グローバル変数宣言
//***************************************************************************************************************
Camera g_camera;		// カメラ情報

//===========================================================================================================
// カメラの初期化処理
//===========================================================================================================
void InitCamera(void)
{
	Player* pPlayer = GetPlayer();

	// 変数の初期化
	g_camera.posV = D3DXVECTOR3(0.0f, 550.0f, -600.0f);			// カメラの位置

	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの見ている位置
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// 上方向ベクトル
	g_camera.rot = D3DXVECTOR3(D3DX_PI * 0.65f, 0.0f, 0.0f);	    // 角度
	g_camera.g_CameraMode = CAMERAMODE_PLAYER;						// 初期状態
	g_camera.bTracking = true;						// 初期状態

	float fRotx = g_camera.posV.x - g_camera.posR.x;
	float fRoty = g_camera.posV.y - g_camera.posR.y;
	float fRotz = g_camera.posV.z - g_camera.posR.z;

	g_camera.fDistance = sqrtf((fRotx * fRotx) + (fRoty * fRoty) + (fRotz * fRotz));	// 視点から注視点までの距離
	
	//g_camera.Direction = 1.0f; // 移動量

	//g_camera.viewport.X = 0; // 2DX座標
	//g_camera.viewport.Y = 0; // 2DY座標
	//g_camera.viewport.Width = 1280; // 幅
	//g_camera.viewport.Height = 720; // 高さ
	//g_camera.viewport.MinZ = 0.0f;
	//g_camera.viewport.MaxZ = 1.0f;

	//g_camera.viewport.X = 1000; // 2DX座標
	//g_camera.viewport.Y = 100; // 2DY座標
	//g_camera.viewport.Width = 200; // 幅
	//g_camera.viewport.Height = 200; // 高さ
	//g_camera.viewport.MinZ = 0.0f;
	//g_camera.viewport.MaxZ = 1.0f;
	
}
//===========================================================================================================
// カメラの終了処理
//===========================================================================================================
void UninitCamera(void)
{
	// 無し
}
//===========================================================================================================
// カメラの更新処理
//===========================================================================================================
void UpdateCamera(void)
{
	// プレイヤーを取得
	Player* pPlayer = GetPlayer();
	MODE mode = GetMode();

	MouseEditMode();	//編集モード中のカメラ移動
	
	MouseWheel();
	// ゲームの時のカメラの更新
		
	//// カメラの右スティック
	//StickCamera();

	////マウスの視点移動
	//MouseView();


	//D3DXVECTOR3 ViewPos(pPlayer->pos.x, pPlayer->pos.y + DiffPos.y, pPlayer->pos.z);

	// カメラ追従できる
	if (g_camera.bTracking == true)
	{
		//g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rotDestPlayer.y) * 1.0f;
		//g_camera.posRDest.y = pPlayer->pos.y + sinf(pPlayer->rotDestPlayer.y) * 1.0f;
		//g_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rotDestPlayer.y) * 1.0f;

		//g_camera.posVDest.x = pPlayer->pos.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		//g_camera.posVDest.y = pPlayer->pos.y - cosf(g_camera.rot.y) * g_camera.fDistance;
		//g_camera.posVDest.z = pPlayer->pos.z - cosf(g_camera.rot.y) * g_camera.fDistance;

		//g_camera.posR.x += ((g_camera.posRDest.x - g_camera.posR.x) * 0.3f);
		//g_camera.posR.y += ((g_camera.posRDest.y - g_camera.posR.y) * 0.3f);
		//g_camera.posR.z += ((g_camera.posRDest.z - g_camera.posR.z) * 0.3f);

		//g_camera.posV.x += ((g_camera.posVDest.x - g_camera.posV.x) * 0.3f);
		//g_camera.posV.z += ((g_camera.posVDest.z - g_camera.posV.z) * 0.3f);
	}
#if 0
	//******************
	// 視点の旋回
	//******************
	if (GetKeyboardPress(DIK_LEFT) == true || JoypadPress(JOYKEY_LEFT_B))
	{// LEFTキーが押された

		// カメラの回転
		g_camera.rot.y -= 0.03f;

		// 角度の正規化
		if (g_camera.rot.y < -D3DX_PI)
		{// D3DX_PIより小さくなったら
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
	}

	if (GetKeyboardPress(DIK_RIGHT) == true || JoypadPress(JOYKEY_RIGHT_B))
	{// RIGHTキーが押された
		// カメラの回転
		g_camera.rot.y += 0.03f;

		// 角度の正規化
		if (g_camera.rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}

	}

#ifdef _DEBUG

	if (GetKeyboardPress(DIK_UP))
	{// UPキーが押された
		// カメラの回転
		g_camera.rot.x += 0.03f;

		// 角度の正規化
		if (g_camera.rot.x > D3DX_PI)
		{// D3DX_PIより小さくなったら
			g_camera.rot.x -= D3DX_PI * 2.0f;
		}

	}
	if (GetKeyboardPress(DIK_DOWN))
	{// DOWNキーが押された
		// カメラの回転
		g_camera.rot.x -= 0.03f;

		// 角度の正規化
		if (g_camera.rot.x < -D3DX_PI)
		{// D3DX_PIより小さくなったら
			g_camera.rot.x += D3DX_PI * 2.0f;
		}
	}

#endif // DEBUG

	
	if (g_camera.rot.x <= D3DX_PI * 0.55f)
	{// カメラの下限

		g_camera.rot.x = D3DX_PI * 0.55f;
	}
	else if (g_camera.rot.x >= D3DX_PI * 0.9f)
	{// カメラの上限

		g_camera.rot.x = D3DX_PI * 0.9f;
	}
#endif
#if 0

		// カメラの視点の情報
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;



		switch (g_camera.g_CameraMode)
		{// モード切替
		case CAMERAMODE_NONE:
			break;

		case CAMERAMODE_PLAYER:
			// プレイヤーに追従させる
			g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rotDestPlayer.y) * 1.0f;
			g_camera.posRDest.y = pPlayer->pos.y + cosf(pPlayer->rotDestPlayer.y) * 1.0f;
			g_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rotDestPlayer.y) * 1.0f;

			g_camera.posVDest.x = pPlayer->pos.x - sinf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posVDest.y = pPlayer->pos.y - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posVDest.z = pPlayer->pos.z - cosf(g_camera.rot.y) * g_camera.fDistance;

			g_camera.posR.x += ((g_camera.posRDest.x - g_camera.posR.x) * 0.3f);
			g_camera.posR.z += ((g_camera.posRDest.z - g_camera.posR.z) * 0.3f);
			g_camera.posR.y += ((g_camera.posRDest.y - g_camera.posR.y) * 0.3f);

			g_camera.posV.x += ((g_camera.posVDest.x - g_camera.posV.x) * 0.3f);
			g_camera.posV.z += ((g_camera.posVDest.z - g_camera.posV.z) * 0.3f);
			
			break;

		default:
			break;
		}
	

	if (KeyboardTrigger(DIK_F2))
	{// モード切替
		if (g_camera.g_CameraMode == CAMERAMODE_NONE)
		{// 追従モード
			g_camera.g_CameraMode = CAMERAMODE_PLAYER;
		}
		else if (g_camera.g_CameraMode == CAMERAMODE_PLAYER)
		{// 通常状態
			g_camera.g_CameraMode = CAMERAMODE_NONE;
		}
	}

#endif // !_DEBUG
}
//===========================================================================================================
// カメラの設定処理
//===========================================================================================================
void SetCamera()
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		100000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
	
}
//===========================================================================================================
// カメラの取得処理
//===========================================================================================================
Camera* GetCamera()
{
	return &g_camera;
}
//====================================================================================================================
// カメラのための右スティック処理
//====================================================================================================================
void StickCamera(void)
{
	// スティック
	XINPUT_STATE* pStick = GetJoyStickAngle();

#if 1
	if (GetJoyStick())
	{
		float RStickAngleY = pStick->Gamepad.sThumbRY;
		float RStickAngleX = pStick->Gamepad.sThumbRX;

		float DeadZone = 10920.0f;
		float fMag = sqrtf((RStickAngleX * RStickAngleX) + (RStickAngleY * RStickAngleY));

		if (fMag > DeadZone)
		{
			if (pStick->Gamepad.sThumbRX < -10920.0f)
			{// 左右移動
				// 角度
				g_camera.rot.y -= 0.03f;

			}
			else if (pStick->Gamepad.sThumbRX > 10920.0f)
			{// 左右移動
				// 角度
				g_camera.rot.y += 0.03f;
			}

			if (pStick->Gamepad.sThumbRY < -10920.0f)
			{// 左右移動
				// 角度
				g_camera.rot.x -= 0.03f;
			}
			else if (pStick->Gamepad.sThumbRY > 10920.0f)
			{// 左右移動
				// 角度
				g_camera.rot.x += 0.03f;
			}

			// カメラの視点の情報
			g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
			g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;


			//if (pStick->Gamepad.sThumbRY < -10920.0f)
			//{// 上下移動
			//	// 角度
			//	g_camera.rot.x -= 0.03f;

			//	// カメラの視点の情報
			//	g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
			//	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

			//}
			//else if (pStick->Gamepad.sThumbRY > 10920.0f)
			//{
			//	// 角度
			//	g_camera.rot.x += 0.03f;

			//	// カメラの視点の情報
			//	g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
			//	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

			//}

			// 角度の正規化
			if (g_camera.rot.y > D3DX_PI)
			{// D3DX_PIより大きくなったら
				g_camera.rot.y -= D3DX_PI * 2.0f;
			}

			// 角度の正規化
			if (g_camera.rot.y < -D3DX_PI)
			{// D3DX_PIより小さくなったら
				g_camera.rot.y += D3DX_PI * 2.0f;
			}

			if (g_camera.rot.x <= D3DX_PI * 0.55f)
			{// カメラの下限

				g_camera.rot.x = D3DX_PI * 0.55f;
			}

			if (g_camera.rot.x >= D3DX_PI * 0.9f)
			{// カメラの上限

				g_camera.rot.x = D3DX_PI * 0.9f;
			}

		}
	}
#endif
}
//===========================================================================================================
// マウスの視点移動処理
//===========================================================================================================
void MouseView(void)
{
	MODE mode = GetMode();

	DIMOUSESTATE mouseState;

	if (GetMouseState(&mouseState))
	{
		static POINT prevCursorPos = { (long)SCREEN_WIDTH / (long)1.5f,(long)SCREEN_HEIGHT / (long)1.5f };

		POINT cursorPos;
		GetCursorPos(&cursorPos);

		float X = (float)cursorPos.x - prevCursorPos.x;
		float Y = (float)cursorPos.y - prevCursorPos.y;

		const float mouseSensitivity = 0.00025f;

		X *= mouseSensitivity;
		Y *= mouseSensitivity;

		g_camera.rot.y += X;
		g_camera.rot.x += Y;

		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		else if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y += -D3DX_PI * 2.0f;
		}

		if (g_camera.rot.x < -D3DX_PI)
		{
			g_camera.rot.x += D3DX_PI * 2.0f;
		}
		else if (g_camera.rot.x > D3DX_PI)
		{
			g_camera.rot.x += -D3DX_PI * 2.0f;
		}

		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= Y;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= Y;
		}

		SetCursorPos((long)SCREEN_WIDTH / (long)1.5f, (long)SCREEN_HEIGHT / (long)1.5f);

		prevCursorPos.x = (long)SCREEN_WIDTH / (long)1.5f;
		prevCursorPos.y = (long)SCREEN_HEIGHT / (long)1.5f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.y = g_camera.posV.y + cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
		
	}
}
//==============================================================================================================
// 編集モードの時のマウス移動
//==============================================================================================================
void MouseEditMode(void)
{
	if (!GetKeyboardPress(DIK_LALT)) return;

	D3DXVECTOR2 Move = GetMouseVelocity();
	D3DXVECTOR2 MoveOld = GetMouseOldVelocity();

	D3DXVECTOR2 fAngle = Move - MoveOld;

	if (OnMousePress(RIGHT_MOUSE))
	{

		//回転量を更新
		g_camera.rot.y += fAngle.x * 0.01f;
		g_camera.rot.x += fAngle.y * 0.01f;

		//回転量を制限
		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}

		//カメラ座標を更新
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.y = g_camera.posV.y + cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

	}
	else if (OnMousePress(LEFT_MOUSE))
	{
		//回転量を更新
		g_camera.rot.y += fAngle.x * 0.01f;
		g_camera.rot.x += fAngle.y * 0.01f;

		//回転量を制限
		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}

		// カメラの視点の情報
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	//正規化
	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y += -D3DX_PI * 2.0f;
	}
	if (g_camera.rot.x < -D3DX_PI)
	{
		g_camera.rot.x += D3DX_PI * 2.0f;
	}
	else if (g_camera.rot.x > D3DX_PI)
	{
		g_camera.rot.x += -D3DX_PI * 2.0f;
	}


}
//===========================================================================================================
// マウスの視点移動処理
//===========================================================================================================
void MouseWheel(void)
{
	// 現在のモードを取得
	MODE nMode = GetMode();

	// マウスホイールの回転量を取得
	int nWheel = GetMouseWheel();

	if (nWheel == 0)
	{
		return;
	}

	if (nWheel < 0)
	{
		g_camera.fDistance += 50.0f;
	}
	else if (nWheel > 0)
	{
		g_camera.fDistance -= 50.0f;
	}
	// カメラの視点の情報
	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
	g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;	
}

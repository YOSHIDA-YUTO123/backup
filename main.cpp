//============================================================================================================
//
// main [main.cpp]
// Author: YOSHIDA YUTO
//
//============================================================================================================

//**************************************************************************************************************
// インクルードファイル宣言
//**************************************************************************************************************
#include "main.h"
#include <time.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include "camera.h"
#include "light.h"
#include "input.h"
#include "mouse.h"
#include "player.h"
#include <Shlwapi.h>
//#include"imgui.h"
#include "imguimaneger.h"
#include "imgui_impl_win32.h" 
#include "meshfield.h"
#include "billboard.h"
#include "enemy.h"
#include"sphereGage.h"
#include"ball.h"
//***************************************************************************************************************
// マクロ定義
//***************************************************************************************************************
#define DRAWMODEL_NUMINFO (10) // 10パーツ分のモデルの情報の表示

//***************************************************************************************************************
// プロトタイプ宣言
//***************************************************************************************************************
void ResetDevice();					// デバイスのリセット
bool CreateDeviceD3D(HWND hWnd);	// デバイスの作成
void ToggleFullScreen(HWND hWnd);	// フルスクリーンの設定

void SetDebugFont(int PosX, int PosY, D3DXCOLOR col, const char* Word, ...);	// プレイヤーの情報

//***************************************************************************************************************
// グローバル変数宣言
//***************************************************************************************************************
RECT g_windowRect;						// ウィンドウを切り替えるための変数
static LPDIRECT3D9 g_pD3D = NULL;				// Direct3Dオブジェクトへのポインタ
static LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3Dデバイスへのポインタ
static bool g_DeviceLost = false;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS  g_d3dpp = {};
bool g_DeviceInitialized = true;
MODE g_mode = MODE_MOTIONEDIT;				// 現在のモード
LPD3DXFONT g_pFont = NULL;				// フォントへのポインタ
int g_nCountFPS = 0;					// FPSカウンタ
int nGetNumber = 0;						// 番号
bool g_bWireFrame = false;

LPDIRECT3DTEXTURE9 g_RenderTexture = NULL; // 画面のテクスチャ
LPDIRECT3DSURFACE9 g_Surface = NULL;	   // テクスチャを作る時にひつようなやつ
LPD3DXEFFECT g_Effect = NULL;			   // シェーダーへのポインタ

//==================================================================================================================
// メイン関数
//==================================================================================================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// メモリリーク検知用のフラグ

	DWORD dwCurrentTime;			// 現在時刻
	DWORD dwExecLastTime;			// 終了時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				 // WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						 // ウインドウのスタイル
		WindowProc,						 // ウインドウプロシージャ
		0,								 // 0にする
		0,								 // 0にする
		hInstance,						 // インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),  // タスクバーアイコン
		LoadCursor(NULL,IDC_ARROW),      // マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),      // クライアント領域の背景色
		NULL,                            // メニューバー
		CLASS_NAME,                      // ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),  // ファイルアイコン
	};

	HWND hWnd = {};  // ウインドウハンドル(識別子)
	MSG msg;		 // メッセージを格納する変数

	// 画面サイズの構造
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// フルスクリーンモード
	BOOL bWindow = FALSE;

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを生成
	hWnd = CreateWindowEx(0,		// 拡張ウインドウスタイル
		CLASS_NAME,					// ウインドウクラスの名前
		WINDOW_NAME,				// ウインドウの名前
		WS_OVERLAPPEDWINDOW,		// ウインドウスタイル
		CW_USEDEFAULT,				// ウインドウの左上のX座標
		CW_USEDEFAULT,				// ウインドウの左上のY座標
		(rect.right - rect.left),   // ウインドウ幅
		(rect.bottom - rect.top),   // ウインドウの高さ
		NULL,
		NULL,
		hInstance,					// インスタンスハンドル
		NULL);						// ウインドウ作成データ

		// デバイスの作成
	if (!CreateDeviceD3D(hWnd))
	{
		Uninit();

		//ウインドウクラスの登録を解除
		UnregisterClass(CLASS_NAME, wcex.hInstance);

		return 1;
	}
	// Main loop
	bool done = false;

	// 分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;					// 初期化
	dwExecLastTime = timeGetTime();		// 現在時刻を保存

	//g_Render = D3DXCreateTexture(g_pD3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, NULL, NULL, &g_Render);
	// 初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		// 初期化処理が失敗したとき
		return -1;
	}

	// ウインドウの表示
	ShowWindow(hWnd, SW_SHOWMAXIMIZED); // ウインドウの表示状態の設定
	UpdateWindow(hWnd);				    // クライアント領域の更新

	// Imguiのウィンドウの初期化処理
	InitImgui(hWnd, g_pD3DDevice);

	// 初期化
	DWORD dwFrameCount;					// フレームカウント
	DWORD dwFPSLastTime;				// 最後にFPSを計測した時刻

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else 
		{
			// DirectXの処理
			dwCurrentTime = timeGetTime();		// 現在時刻の取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				// 0.5秒経過
				// FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// 保存

				dwFrameCount = 0;				// 0に戻す
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				// 60/1経過
				dwExecLastTime = dwCurrentTime;	// 処理開始の時刻

				dwFrameCount++;					// フレームカウントを加算

								// Handle lost D3D9 device
				if (g_DeviceLost)
				{
					HRESULT hr = g_pD3DDevice->TestCooperativeLevel();
					if (hr == D3DERR_DEVICELOST)
					{
						::Sleep(50);
						continue;
					}
					if (hr == D3DERR_DEVICENOTRESET)
						ResetDevice();
					g_DeviceLost = false;
				}

				// Handle window resize (we don't resize directly in the WM_SIZE handler)
				if (g_ResizeWidth > 0 && g_ResizeHeight > 0)
				{
					g_d3dpp.BackBufferWidth = g_ResizeWidth;
					g_d3dpp.BackBufferHeight = g_ResizeHeight;
					g_ResizeWidth = g_ResizeHeight = 0;
					ResetDevice();
				}
				NewFlameImgui();

				// 更新処理
				Update();

				ImGui::EndFrame();

				// 描画処理
				Draw();

			}
		}
	}

	// IMGUIの終了処理
	UninitImgui();

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//=========================================================================================================================
// IMGUIのハンドラー宣言
//=========================================================================================================================
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=========================================================================================================================
// ウインドウプロシージャ
//=========================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Player* pPlayer = GetPlayer();

	// IMGUIのクリック判定
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }; // ウインドウの領域

	int nID;

	switch (uMsg)
	{
	case WM_SIZE:
	
		if (wParam == SIZE_MINIMIZED)
			return 0;

		// 現在のウィンドウの大きさを代入
		g_ResizeWidth = (UINT)LOWORD(lParam);
		g_ResizeHeight = (UINT)HIWORD(lParam);
	
		return 0;
	case WM_ACTIVATE:	// アクティブ時：1　非アクティブ時：0
		return 0L;
	case WM_CREATE:
		DragAcceptFiles(hWnd, TRUE);
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:// ウインドウ破棄メッセージ
		// WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:				// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);// ウインドウ破棄メッセージ

			}
			else
			{
				return 0;			// 返す
			}
			break;

		case VK_F11:
			ToggleFullScreen(hWnd);	// F11でフルスクリーン
			break;
		}

		// ウィンドウプロシージャ内
	//case WM_SETCURSOR:	// カーソルの設定
	//	SetCursor(NULL);
	//	return TRUE;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); // 既定の処理を繰り返す
}
//===========================================================================================================
// 初期化処理
//===========================================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションのパラメーターを設定
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp)); // パラメーターの0クリア

	g_d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	g_d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	g_d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	g_d3dpp.BackBufferCount = 1;									// バックバッファの数
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	g_d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使用
	g_d3dpp.Windowed = bWindow;									// ウインドウモード
	g_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュシート
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&g_d3dpp,
		&g_pD3DDevice)))
	{
		// Direct3Dデバイスの生成(描画処理はハードウェア,頂点処理をCPUで行う)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&g_d3dpp,
			&g_pD3DDevice)))
		{
			// Direct3Dデバイスの生成(描画処理,頂点処理をCPUで行う)
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&g_d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージチャートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//===================================================================================================
	// 各種初期化処理
	//===================================================================================================

	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	if (FAILED(InitMouse(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//// デバッグ表示用のフォントを設定
	//D3DXCreateFont(g_pD3DDevice, 56, 0, FW_NORMAL, 0,
	//	FALSE,
	//	DEFAULT_CHARSET,
	//	OUT_DEFAULT_PRECIS,
	//	DEFAULT_QUALITY,
	//	DEFAULT_PITCH | FF_DONTCARE,
	//	"メイリオ", &g_pFont);

	// テクスチャの読み込み
	g_pD3DDevice->CreateTexture(g_d3dpp.BackBufferWidth, g_d3dpp.BackBufferHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&g_RenderTexture,
		nullptr);

	LPD3DXBUFFER error;
	HRESULT hr;
	if (FAILED(hr = D3DXCreateEffectFromFile(g_pD3DDevice, "postprocess.hlsl", nullptr, nullptr, 0, nullptr, &g_Effect, &error)))
	{
		MessageBox(NULL, (LPCTSTR)error->GetBufferPointer(), "エラー", MB_OK);
	}

	g_RenderTexture->GetSurfaceLevel(0, &g_Surface);

	//// サウンドの初期化
	//InitSound(hWnd);

	// メッシュフィールドの初期化処理
	InitMeshField();

	// プレイヤーの初期化処理
	InitPlayer();

	// カメラの初期化処理
	InitCamera();	

	// ライトの初期化処理
	InitLight();

	// ビルボードの初期化処理
	InitBillboard();

	// 敵の初期化処理
	InitEnemy();

	// 円形のゲージの初期化処理
	InitSphere();

	// ボールの初期化処理
	InitBalll();

	SetEnemy(D3DXVECTOR3(0.0f,0.0f,500.0f));

	SetBillboard(D3DXVECTOR3(0.0f, 500.0f, 200.0f));

	// モードの設定
	SetMode(g_mode);

	return S_OK;
}
//====================================================================================================
// 終了処理
//====================================================================================================
void Uninit(void)
{
	//====================================================================================================
	// 各種終了処理
	//====================================================================================================

	// キーボードの終了
	UninitKeyboard();

	// ジョイパッドの終了
	UninitJoypad();

	//マウスの終了
	UninitMouse();

	// メッシュフィールドの終了処理
	UninitMeshField();

	// プレイヤーの終了処理
	UninitPlayer();

	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// ビルボードの終了処理
	UninitBillboard();

	// 敵の終了処理
	UninitEnemy();

	// 円形のゲージの終了処理
	UninitSphere();

	// ボールの終了処理
	UninitBalll();

	if (g_Effect != NULL)
	{
		g_Effect->Release();
		g_Effect = NULL;
	}

	if (g_Surface != NULL)
	{
		g_Surface->Release();
		g_Surface = NULL;
	}

	if (g_RenderTexture != NULL)
	{
		g_RenderTexture->Release();
		g_RenderTexture = NULL;
	}

	// デバッグ表示用のフォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}
//=====================================================================================================
// 更新処理
//=====================================================================================================
void Update(void)
{
	//============================================================================================================
	// 各種オブジェクト更新処理
	//============================================================================================================

	// キーボードの更新
	UpdateKeyboard();

	// ジョイパッドの更新
	UpdateJoypad();

	//マウスの更新
	UpdateMouse();

	// デバッグ用IMGUI
	DebugImgui();

	static bool bPause = false;

	// カメラの更新処理
	UpdateCamera();

	// ライトの更新処理
	UpdateLight();

	// 円形のゲージの更新処理
	UpdateSphere();

	ImGui::Checkbox(u8"ポーズ", &bPause);

	if (bPause == false)
	{
		// 敵の更新処理
		UpdateEnemy();

		// メッシュフィールドの更新処理
		UpdateMeshField();

		// プレイヤーの更新処理
		UpdatePlayer();

		// ビルボードの更新処理
		UpdateBillboard();

		// ボールの更新処理
		UpdateBalll();
	}
	switch (g_mode)
	{
	case MODE_MOTIONEDIT: // タイトル画面
		break;
	}

}
//=====================================================================================================
// 描画処理
//=====================================================================================================
void Draw(void)
{
	// カメラを取得
	Camera* pCamera = GetCamera();

	LPDIRECT3DSURFACE9 oldSurface;

	g_pD3DDevice->GetRenderTarget(0, &oldSurface);

	g_pD3DDevice->SetRenderTarget(0, g_Surface);

	//画面クリア(バックバッファ&Zバッファのクリア)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		SetCamera(); // カメラをセット

	// メッシュフィールドの描画処理
		DrawMeshField();

		// 敵の描画処理
		DrawEnemy();

		// プレイヤーの描画処理
		DrawPlayer();

		// ボールの描画処理
		DrawBalll();

		// ライトの描画処理
		DrawLight();

		// ビルボードの描画処理
		DrawBillboard();

		//// 円形のゲージの描画処理
		//DrawSphere();

		//// Fpsの表示
		//DrawFPS();

		// IMGUIのウィンドウのレンダラーの設定
		EndImguiRender();

		// 描画終了
		g_pD3DDevice->EndScene();

	}

	g_pD3DDevice->SetRenderTarget(0, oldSurface);

	VERTEX_2D quad[4] =
	{
		{ D3DXVECTOR3(0.0f,   0.0f,   0.0f), 1.0f, 0xFFFFFFFF, D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(g_d3dpp.BackBufferWidth, 0.0f,   0.0f), 1.0f, 0xFFFFFFFF, D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f,   g_d3dpp.BackBufferHeight, 0.0f), 1.0f, 0xFFFFFFFF, D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3(g_d3dpp.BackBufferWidth, g_d3dpp.BackBufferHeight, 0.0f), 1.0f, 0xFFFFFFFF, D3DXVECTOR2(1.0f, 1.0f) },
	};

	if (g_Effect != NULL)
	{
		g_Effect->SetTexture("tex0", g_RenderTexture);

		g_Effect->Begin(0, 0);

		g_Effect->BeginPass(0);

		g_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &quad, sizeof(VERTEX_2D));

		g_Effect->EndPass();

		g_Effect->End();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	if (oldSurface != NULL)
	{
		oldSurface->Release();
		oldSurface = NULL;
	}
}
//========================================================================================================
// FPSの表示
//========================================================================================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	//文字列に代入
	wsprintf(&aString[0], "FPS:%d\n", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
//==============================================================================================================
// 敵のデバッグ表示
//==============================================================================================================
void DrawEnemySet(int g_nNumEnemy)
{
	// ローカル変数
	RECT rect = { 0,180,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	// 文字列に代入
	wsprintf(&aString[0], "敵の総数:%d\n", g_nNumEnemy);

	// テキスト描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
//================================================================================================================
// プレイヤーの座標表示
//================================================================================================================
void DrawDebugPlayerPos(void)
{
#if 0
	// ローカル変数
	RECT rect = { 0,80,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,100,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect2 = { 0,120,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect3 = { 0,140,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aString[256];
	char aString1[256];
	char aString2[256];
	char aString3[256];

	// 取得
	PLAYER* pPlayer = GetPlayer();

	// 文字列に代入
	sprintf(&aString[0], "プレイヤーのpos.x:%.2f\n", pPlayer->pos.x);
	sprintf(&aString1[0], "プレイヤーのpos.y:%.2f\n", pPlayer->pos.y);
	sprintf(&aString2[0], "プレイヤーのpos.z:%.2f\n", pPlayer->pos.z);
	wsprintf(&aString3[0], "ワイヤーフレーム ON[F3]:OFF[F4]\n");

	// テキスト描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString2[0], -1, &rect2, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString3[0], -1, &rect3, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
#endif
}
//==========================================================================================================
// カメラ
//==========================================================================================================
void DrawCameraPos(void)
{
	// ローカル変数
	RECT rect = { 0,40,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,60,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aString[256];
	char aString1[256];

	// 取得
	Camera* pCamera = GetCamera();

	// 文字列に代入
	sprintf(&aString[0], "カメラのrot.y:%.2f\n", pCamera->rot.y);
	sprintf(&aString1[0], "カメラのrot.x:%.2f\n", pCamera->rot.x);

	// テキスト描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

}
//==========================================================================================================
// カメラの種類
//==========================================================================================================
void DrawCameraType(void)
{
	RECT rect = { 0,20,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];
	char aString1[256];

	// 取得
	Camera* pCamera = GetCamera();

	if (pCamera->g_CameraMode == CAMERAMODE_NONE)
	{
		strcpy(&aString1[0], "NORMAL");
	}
	else if (pCamera->g_CameraMode == CAMERAMODE_PLAYER)
	{
		strcpy(&aString1[0], "PLAYER");
	}

	//文字列に代入
	wsprintf(&aString[0], "現在のモード[F2] : %s\n", &aString1[0]);

	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

}
//========================================================================================================
// モードの設定
//========================================================================================================
void SetMode(MODE mode)
{
	// 現在の画面の終了処理
	switch (g_mode)
	{
	case MODE_MOTIONEDIT: // タイトル画面
		break;
	}

	g_mode = mode;		// 画面切り替え

	// 新しい画面の初期化処理
	switch (mode)
	{
	case MODE_MOTIONEDIT: // タイトル画面
		break;
	}
}
//========================================================================================================
// デバイスの取得
//========================================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//======================================================================================================
// モードの取得
//======================================================================================================
MODE GetMode(void)
{
	return g_mode; // 変数を返す
}
//=======================================================================================================
// ワイヤーフレームON
//=======================================================================================================
void onWireFrame()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}
//=======================================================================================================
// ワイヤーフレームOFF
//=======================================================================================================
void offWireFrame()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
//============================================================================================================
// マウスを3D座標に変換する
//============================================================================================================
void GetRayFromMouse(int mouseX, int mouseY, D3DXVECTOR3* outOrigin, D3DXVECTOR3* outDir)
{
	// プロジェクションマトリックスとビューマトリックス
	D3DXMATRIX matView, matproj;

	// プロジェクションマトリックスとビューマトリックスを取得
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matproj);

	RECT rect;
	GetClientRect(FindWindowA(CLASS_NAME, nullptr), &rect);

	int screenWidth = rect.right - rect.left;
	int screenHeight = rect.bottom - rect.top;

	// NDC (-1～1)に変換
	float pX = ((2.0f * mouseX) / screenWidth) - 1.0f;
	float pY = ((2.0f * mouseY) / screenHeight) - 1.0f;
	pY *= -1.0f;

	// ビュー空間のNear/Far
	D3DXVECTOR3 vNear = D3DXVECTOR3(pX, pY, 0.0f);
	D3DXVECTOR3 vFar = D3DXVECTOR3(pX, pY, 1.0f);

	// 合成行列の逆行列
	D3DXMATRIX matInvVP;
	D3DXMATRIX matVP = matView * matproj;
	D3DXMatrixInverse(&matInvVP, NULL, &matVP);

	// ワールド座標に変換
	D3DXVec3TransformCoord(&vNear, &vNear, &matInvVP);
	D3DXVec3TransformCoord(&vFar, &vFar, &matInvVP);

	// レイの原点
	*outOrigin = vNear;
	D3DXVECTOR3 Ray = vFar - vNear;
	D3DXVec3Normalize(outDir, &Ray);
}
//============================================================================================================
// プレイヤーの情報
//============================================================================================================
void SetDebugFont(int PosX, int PosY, D3DXCOLOR col, const char* Word, ...)
{
	RECT rect = { PosX,PosY,SCREEN_WIDTH,SCREEN_HEIGHT };

	// 文字列保管用変数
	char aString[256] = {};

	// 可変引数を処理
	va_list args;

	// 可変引数の処理を始める
	va_start(args, Word);

	// フォーマット済みの文字列をastringに保管
	vsnprintf(aString, sizeof(aString), Word, args);

	// 可変引数の処理を終了
	va_end(args);

	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, col);
}
//====================================================================
// フルスクリーンの設定
//====================================================================
void ToggleFullScreen(HWND hWnd)
{
	static bool isFullscreen = true;

	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (isFullscreen)
	{
		// ウィンドウモードに切り替え
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
			g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	isFullscreen = !isFullscreen;
}
//====================================================================
// デバイスのリセット
//====================================================================
void ResetDevice()
{
	if (g_Effect)
		g_Effect->OnLostDevice();

	if (g_Surface) {
		g_Surface->Release();
		g_Surface = NULL;
	}
	if (g_RenderTexture) {
		g_RenderTexture->Release();
		g_RenderTexture = NULL;
	}

	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT hr = g_pD3DDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();

	// テクスチャの読み込み
	g_pD3DDevice->CreateTexture(g_d3dpp.BackBufferWidth, g_d3dpp.BackBufferHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&g_RenderTexture,
		nullptr);
	LPD3DXBUFFER error;
	hr;
	//if (FAILED(hr = D3DXCreateEffectFromFile(g_pD3DDevice, "postprocess.hlsl", nullptr, nullptr, 0, nullptr, &g_Effect, &error)))
	//{
	//	MessageBox(NULL, (LPCTSTR)error->GetBufferPointer(), "エラー", MB_OK);
	//}

	g_RenderTexture->GetSurfaceLevel(0, &g_Surface);

	g_Effect->OnResetDevice();


	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージチャートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//// デバッグ表示用のフォントを設定
	//D3DXCreateFont(g_pD3DDevice, 86, 0, FW_BOLD, 0,
	//	FALSE,
	//	DEFAULT_CHARSET,
	//	OUT_DEFAULT_PRECIS,
	//	DEFAULT_QUALITY,
	//	DEFAULT_PITCH | FF_DONTCARE,
	//	"Jump Streets", &g_pFont);

	// ライトの初期設定
	InitLight();

	// ライトを設置
	SetLight(D3DXVECTOR3(0.0f, 1200.0f, 0.0f), D3DLIGHT_DIRECTIONAL, D3DXVECTOR3(0.0f, -1.0f, 0.0f));

	// ライトを設置
	SetLight(D3DXVECTOR3(4200.0f, 500.0f, 0.0f), D3DLIGHT_DIRECTIONAL, D3DXVECTOR3(1.0f, -1.0f, 0.0f));

	// ライトを設置
	SetLight(D3DXVECTOR3(-4200.0f, 500.0f, 0.0f), D3DLIGHT_DIRECTIONAL, D3DXVECTOR3(-1.0f, -1.0f, 0.0f));

}
//====================================================================
// デバイスの作成
//====================================================================
bool CreateDeviceD3D(HWND hWnd)
{
	D3DDISPLAYMODE d3ddm = {}; //ディスプレイモード

	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
		return false;

	// Create the D3DDevice
	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));//パラメータのゼロクリア

	g_d3dpp.BackBufferWidth = g_ResizeWidth;//ゲーム画面サイズ(幅)
	g_d3dpp.BackBufferHeight = g_ResizeHeight;//ゲーム画面サイズ(高さ)
	g_d3dpp.BackBufferFormat = d3ddm.Format;//バックバッファの形式
	g_d3dpp.BackBufferCount = 1;//バックバッファの数
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//バックバッファの切り替え(映像信号に同期)
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//デプスバッファとして16bitを使う
	g_d3dpp.Windowed = TRUE;//ウインドウモード
	g_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//リフレッシュレート
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//インターバル

	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pD3DDevice) < 0)
		return false;

	return true;
}

#ifndef _PCH_H_  // このマクロ定義がされてなかったら
#define _PCH_H_  // 2重インクルード防止のマクロ定義

//***************************
// インクルードファイル宣言
//***************************
#include <windows.h>
#define DIRECTINPUT_VERSION (0x0800)// ビルド時警告対処用のマクロ
#include "d3dx9.h"					// 描画処理に必要
#include "dinput.h"					// 入力処理に必要
#include "Xinput.h"					// ジョイパッド処理に必要
#include "xaudio2.h"
//**************************
// ライブラリのリンク宣言
//**************************
#pragma comment(lib,"d3d9.lib")		// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	// DirectXコンポーネント(部品)使用に必要
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"xinput.lib")	// ジョイパッド処理に必要
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib, "Shlwapi.lib")

//**************************
// マクロ定義
//**************************
#define CLASS_NAME "WindowClass"      // ウインドウクラスの名前
#define WINDOW_NAME "Motion Viewer"		  // ウインドウの名前(キャプションに表示)
#define SCREEN_WIDTH (1280)           // ウインドウの幅
#define SCREEN_HEIGHT (720)           // ウインドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) // 頂点フォーマット[2D]
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL |  D3DFVF_DIFFUSE | D3DFVF_TEX1) // 座標,法線,カラー,テクスチャ
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//**********************
// 頂点情報[2D]の構造体
//**********************
typedef struct
{
	D3DXVECTOR3 pos;			// 頂点座標
	float rhw;					// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;				// 頂点カラー
	D3DXVECTOR2 tex;			// テクスチャ座標
}VERTEX_2D;

//**********************
// 頂点情報[3D]の構造体
//**********************
typedef struct
{
	D3DXVECTOR3 pos;			// 頂点座標
	D3DXVECTOR3 nor;			// 法線ベクトル
	D3DCOLOR col;				// 頂点カラー
	D3DXVECTOR2 tex;			// テクスチャ座標
}VERTEX_3D;

//**********************
// 線の構造体
//**********************
typedef struct
{
	D3DXVECTOR3 pos;
	DWORD color;
}LINEVERTEX;

#endif
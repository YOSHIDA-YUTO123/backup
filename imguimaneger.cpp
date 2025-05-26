//=============================================================
//
//　Imguimaneger[Imguimaneger.cpp]
//　Author:YUTO YOSHIDA
//
//=============================================================

//*************************************************************
//インクルードファイル
//*************************************************************
#include "imguimaneger.h"
#include "imgui_internal.h"

//=============================================================
// ImGuiの初期化処理
//=============================================================
void InitImgui(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();
	 //ImGui::StyleColorsLight(); // 他のスタイルも選べます

	// フォント設定（デフォルトのフォントとして Consolas を使用）
	ImFont* myFont = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/meiryo.ttc", 20.0f,NULL, io.Fonts->GetGlyphRangesJapanese());

	io.FontDefault = myFont;
	io.FontGlobalScale = 1.0f;
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 10.0f;      // 角丸
	style.Colors[ImGuiCol_Button] = ImVec4(0.3f, 0.6f, 0.1f, 1.0f); // 好きな色

	//style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);     // 文字色
	//style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1, 0.1, 0.1, 1); // 背景色
	//style.Colors[ImGuiCol_Button] = ImVec4(0.4, 0.7, 0.2, 1); // ボタン
	//style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5, 0.8, 0.3, 1); // ホバー
	//style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.3, 0.6, 0.2, 1); // 押されたとき
	//style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2, 0.2, 0.2, 1); // 入力欄の背景
	//style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0, 1.0, 1.0, 1); // スライダーつまみ
	//style.Colors[ImGuiCol_Header] = ImVec4(0.4, 0.4, 0.9, 1); // TreeNodeヘッダー

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);
}

//=============================================================
// ImGuiの終了処理
//=============================================================
void UninitImgui()
{
	// Cleanup
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//=============================================================
// ImGuiのウィンドウの位置を設定
//=============================================================
void SetPosImgui(ImVec2 Pos)
{
	ImGui::SetNextWindowPos(Pos, ImGuiCond_Once);
}

//=============================================================
// ImGuiのウィンドウの大きさを設定
//=============================================================
void SetSizeImgui(ImVec2 Size)
{
	ImGui::SetNextWindowSize(Size, ImGuiCond_Once);
}

//=============================================================
// ImGuiのウィンドウの描画開始
//=============================================================
void StartImgui(const char* ImguiName, IMGUITYPE nType)
{
	if (ImguiName != NULL)
	{
		switch (nType)
		{
		case IMGUITYPE_DEFOULT:
			ImGui::Begin(ImguiName);
			break;
		case IMGUITYPE_NOMOVE:
			ImGui::Begin(ImguiName, NULL, ImGuiWindowFlags_NoMove);
			break;
		case IMGUITYPE_NOMOVESIZE:
			ImGui::Begin(ImguiName, NULL, ImGuiWindowFlags_NoResize);
			break;
		case IMGUITYPE_NOMOVEANDSIZE:
			ImGui::Begin(ImguiName, NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			break;
		default:
			break;
		}
	}
}

//=============================================================
// ImGuiのウィンドウの描画を終了
//=============================================================
void EndImgui()
{
	ImGui::End();
}

//=============================================================
// ImGuiのウィンドウのフレームを設定
//=============================================================
void NewFlameImgui()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

//=============================================================
// ImGuiのウィンドウのレンダーを設定
//=============================================================
void EndImguiRender()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());	
}



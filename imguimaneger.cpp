//=============================================================
//
//�@Imguimaneger[Imguimaneger.cpp]
//�@Author:YUTO YOSHIDA
//
//=============================================================

//*************************************************************
//�C���N���[�h�t�@�C��
//*************************************************************
#include "imguimaneger.h"
#include "imgui_internal.h"

//=============================================================
// ImGui�̏���������
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
	 //ImGui::StyleColorsLight(); // ���̃X�^�C�����I�ׂ܂�

	// �t�H���g�ݒ�i�f�t�H���g�̃t�H���g�Ƃ��� Consolas ���g�p�j
	ImFont* myFont = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/meiryo.ttc", 20.0f,NULL, io.Fonts->GetGlyphRangesJapanese());

	io.FontDefault = myFont;
	io.FontGlobalScale = 1.0f;
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 10.0f;      // �p��
	style.Colors[ImGuiCol_Button] = ImVec4(0.3f, 0.6f, 0.1f, 1.0f); // �D���ȐF

	//style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);     // �����F
	//style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1, 0.1, 0.1, 1); // �w�i�F
	//style.Colors[ImGuiCol_Button] = ImVec4(0.4, 0.7, 0.2, 1); // �{�^��
	//style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5, 0.8, 0.3, 1); // �z�o�[
	//style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.3, 0.6, 0.2, 1); // �����ꂽ�Ƃ�
	//style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2, 0.2, 0.2, 1); // ���͗��̔w�i
	//style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0, 1.0, 1.0, 1); // �X���C�_�[�܂�
	//style.Colors[ImGuiCol_Header] = ImVec4(0.4, 0.4, 0.9, 1); // TreeNode�w�b�_�[

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);
}

//=============================================================
// ImGui�̏I������
//=============================================================
void UninitImgui()
{
	// Cleanup
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//=============================================================
// ImGui�̃E�B���h�E�̈ʒu��ݒ�
//=============================================================
void SetPosImgui(ImVec2 Pos)
{
	ImGui::SetNextWindowPos(Pos, ImGuiCond_Once);
}

//=============================================================
// ImGui�̃E�B���h�E�̑傫����ݒ�
//=============================================================
void SetSizeImgui(ImVec2 Size)
{
	ImGui::SetNextWindowSize(Size, ImGuiCond_Once);
}

//=============================================================
// ImGui�̃E�B���h�E�̕`��J�n
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
// ImGui�̃E�B���h�E�̕`����I��
//=============================================================
void EndImgui()
{
	ImGui::End();
}

//=============================================================
// ImGui�̃E�B���h�E�̃t���[����ݒ�
//=============================================================
void NewFlameImgui()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

//=============================================================
// ImGui�̃E�B���h�E�̃����_�[��ݒ�
//=============================================================
void EndImguiRender()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());	
}



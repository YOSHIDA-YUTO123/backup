//*************************************
//
// ImGuiÇêßå‰Ç∑ÇÈCpp[ImguiManager.h]
// Author YUTO YOSHIDA
//
//*************************************

#ifndef _IMGUIMANEGER_H_
#define _IMGUIMANEGER_H_

#include "main.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

typedef enum
{
	IMGUITYPE_DEFOULT = 0,
	IMGUITYPE_NOMOVE,
	IMGUITYPE_NOMOVESIZE,
	IMGUITYPE_NOMOVEANDSIZE,
	IMGUITYPE_MAX
}IMGUITYPE;

void InitImgui(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
void UninitImgui();
void SetPosImgui(ImVec2 Pos);
void SetSizeImgui(ImVec2 Size);
void StartImgui(const char* ImguiName, IMGUITYPE nType);
void EndImgui();
void EndImguiRender();
void NewFlameImgui();
void SetRotation();

#endif // !_IMGUIMANEGER_H_
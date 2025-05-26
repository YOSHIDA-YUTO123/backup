//============================================================================================================
//
// main [main.h]
// Author: YOSHIDA YUTO
//
//============================================================================================================

#ifndef _MAIN_H_  // ���̃}�N����`������ĂȂ�������
#define _MAIN_H_  // 2�d�C���N���[�h�h�~�̃}�N����`


//**************************
// �}�N����`(�F�ݒ�)
//**************************
#define COLOR_INDIANRED D3DCOLOR_RGBA(205,92,92,255)				 // �C���f�B�A�����b�h
#define COLOR_LIGHTCORAL D3DCOLOR_RGBA(240,128,128,255)				 // ���C�g�R�[����
#define COLOR_SALMON D3DCOLOR_RGBA(250,128,114,255)					 // �T�[�����J���[
#define COLOR_DARKSALMON D3DCOLOR_RGBA(233,150,122,255)				 // �����T�[�����J���[
#define COLOR_LIGHTSALMON D3DCOLOR_RGBA(255,160,122,255)			 // ���邢�T�[�����J���[
#define COLOR_CRIMSON D3DCOLOR_RGBA(220,20,60,255)					 // �N�����]��
#define COLOR_RED D3DCOLOR_RGBA(255,0,0,255)						 // ��
#define COLOR_FIREBRICK D3DCOLOR_RGBA(178,34,34,255)				 // �t�@�C�A�u���b�N
#define COLOR_DARKRED D3DCOLOR_RGBA(139,0,0,255)					 // �Â߂̐�
#define COLOR_PINK D3DCOLOR_RGBA(255,192,203,255)					 // �s���N
#define COLOR_LIGHTPINK D3DCOLOR_RGBA(255,182,193,255)				 // ���邢�s���N
#define COLOR_HOTPINK D3DCOLOR_RGBA(255,105,180,255)				 // �Z���s���N
#define COLOR_DEEPPINK D3DCOLOR_RGBA(255,20,147,255)				 // ����ɔZ���s���N
#define COLOR_MEDIUMVIOLETRED D3DCOLOR_RGBA(199,21,133,255)			 // �����ƔZ���s���N
#define COLOR_PALEVIOLETRED D3DCOLOR_RGBA(219,112,147,255)			 // �Â߂̃z�b�g�s���N
#define COLOR_CORAL D3DCOLOR_RGBA(255,127,80,255)					 // �R�[����
#define COLOR_TOMATO D3DCOLOR_RGBA(255,99,71,255)					 // �g�}�g�F
#define COLOR_ORANGERED D3DCOLOR_RGBA(255,69,0,255)					 // �Ԃ߂̃I�����W
#define COLOR_DARKORANGE D3DCOLOR_RGBA(255,140,0,255)				 // �Â߂̃I�����W
#define COLOR_ORANGE D3DCOLOR_RGBA(255,165,0,255)					 // �I�����W
#define COLOR_GOLD D3DCOLOR_RGBA(255,215,0,255)						 // ���F
#define COLOR_YELLOW D3DCOLOR_RGBA(255,255,0,255)					 // ���F
#define COLOR_LIGHTYELLOW D3DCOLOR_RGBA(255,255,224,255)			 // �N���[���F
#define COLOR_LEMONCHIFFON D3DCOLOR_RGBA(255,250,205,255)			 // �������V�t�H��
#define COLOR_LIGHTGOLDENRODYELLOW D3DCOLOR_RGBA(250,250,210,255)	 // �S�[���h�C�G���[
#define COLOR_PAPAYAWHIP D3DCOLOR_RGBA(255,239,213,255)				 // �p�p�C���|�b�v
#define COLOR_MOCCASIN D3DCOLOR_RGBA(255,228,181,255)				 // ���J�V��
#define COLOR_PEACHPUFF D3DCOLOR_RGBA(255,218,185,255)				 // �s�[�`�p�t
#define COLOR_PALEGOLDENROD D3DCOLOR_RGBA(238,232,170,255)			 // �y�[���S�[���f�����b�h
#define COLOR_KHAKI D3DCOLOR_RGBA(240,230,140,255)					 // �J�[�L
#define COLOR_DARKHAKI D3DCOLOR_RGBA(189,183,107,255)				 // �_�[�N�J�[�L
#define COLOR_LAVENDER D3DCOLOR_RGBA(230,230,250,255)				 // ���x���_�[
#define COLOR_THISTLE D3DCOLOR_RGBA(216,191,216,255)				 // �e�B�X��
#define COLOR_PLUM D3DCOLOR_RGBA(221,160,221,255)					 // �v����
#define COLOR_VIOLET D3DCOLOR_RGBA(238,130,238,255)					 // ���@�C�I���b�g
#define COLOR_ORCHID D3DCOLOR_RGBA(218,112,214,255)					 // �I�[�L�b�h
#define COLOR_FUCHSIA D3DCOLOR_RGBA(255,0,255,255)					 // �t�N�V�A
#define COLOR_MAGENTA D3DCOLOR_RGBA(255,0,255,255)					 // �}�[���^
#define COLOR_MIDIUMORCHID D3DCOLOR_RGBA(186,85,211,255)			 // �~�f�B�A���I�[�L�b�h
#define COLOR_MIDIUMPURPLE D3DCOLOR_RGBA(147,112,219,255)			 // �~�f�B�A���p�[�v��
#define COLOR_AMETHYST D3DCOLOR_RGBA(153,102,204,255)				 // �A���W�X�g
#define COLOR_BLUEVIOLET D3DCOLOR_RGBA(138,43,226,255)				 // �u���[���@�C�I���b�g
#define COLOR_DARKVIOLET D3DCOLOR_RGBA(148,0,211,255)				 // �_�[�N���@�C�I���b�g
#define COLOR_DARKORCHID D3DCOLOR_RGBA(153,50,204,255)				 // �_�[�N�I�[�L�b�h
#define COLOR_DARKMAGENTA D3DCOLOR_RGBA(139,0,139,255)				 // �_�[�N�}�[���^
#define COLOR_PURPLE D3DCOLOR_RGBA(128,0,128,255)					 // �p�[�v��
#define COLOR_INDIGO D3DCOLOR_RGBA(75,0,130,255)					 // ���F
#define COLOR_SLATEBLUE D3DCOLOR_RGBA(106,90,205,255)				 // �X���[�g�u���[
#define COLOR_DARKSLATEBLUE D3DCOLOR_RGBA(72,61,139,255)			 // �_�[�N�X���[�g�u���[
#define COLOR_GREENYELLOW D3DCOLOR_RGBA(173,255,47,255)				 // �O���[���C�G���[
#define COLOR_CHARTREUSE D3DCOLOR_RGBA(127,255,0,255)				 // �V�����g���[�Y
#define COLOR_LAWNGREEN D3DCOLOR_RGBA(124,252,0,255)				 // ���[���O���[��
#define COLOR_LIME D3DCOLOR_RGBA(0,255,0,255)					     // ���C��
#define COLOR_LIMEGREEN D3DCOLOR_RGBA(50,205,50,255)				 // ���C���O���[��
#define COLOR_PALEGREEN D3DCOLOR_RGBA(152,251,152,255)				 // �y�[���O���[��
#define COLOR_LIGHTGREEN D3DCOLOR_RGBA(144,238,144,255)				 // ���C�g�O���[��
#define COLOR_MEDIUMSPRINGGREEN D3DCOLOR_RGBA(0,250,154,255)		 // �~�f�B�A���X�v�����O�O���[��
#define COLOR_SPRINGGREEN D3DCOLOR_RGBA(0,255,127,255)				 // �X�v�����O�O���[��
#define COLOR_MEDIUMSEAGREEN D3DCOLOR_RGBA(60,179,113,255)			 // �~�f�B�A���V�[�O���[��
#define COLOR_SEAGREEN D3DCOLOR_RGBA(46,139,87,255)					 // �V�[�O���[��
#define COLOR_FORESTGREEN D3DCOLOR_RGBA(34,139,34,255)				 // �t�H���X�g�O���[��
#define COLOR_GREEN D3DCOLOR_RGBA(0,128,0,255)						 // ��
#define COLOR_DARKGREEN D3DCOLOR_RGBA(0,100,0,255)					 // �_�[�N�O���[��
#define COLOR_YELLOWGREEN D3DCOLOR_RGBA(154,205,50,255)				 // �C�G���[�O���[��
#define COLOR_OLIVEDRAB D3DCOLOR_RGBA(107,142,35,255)				 // �I���[�u�h���u
#define COLOR_OLIVE D3DCOLOR_RGBA(128,128,0,255)					 // �I���[�u
#define COLOR_DARKOLIVEGREEN D3DCOLOR_RGBA(85,107,47,255)			 // �_�[�N�I���[�u�O���[��
#define COLOR_MEDIUMAQUAMARINE D3DCOLOR_RGBA(102,205,170,255)		 // �~�f�B�A���A�N�A�}����
#define COLOR_DARKSEAGREEN D3DCOLOR_RGBA(143,188,143,255)			 // �_�[�N�V�[�O���[��
#define COLOR_LIGHTSEAGREEN D3DCOLOR_RGBA(32,178,170,255)			 // ���C�g�V�[�O���[��
#define COLOR_DARKCYAN D3DCOLOR_RGBA(0,139,139,255)					 // �_�[�N�V�A��
#define COLOR_TEAL D3DCOLOR_RGBA(0,128,128,255)						 // �e�[��
#define COLOR_AQUA D3DCOLOR_RGBA(0,255,255,255)						 // �A�N�A
#define COLOR_CYAN D3DCOLOR_RGBA(0,255,255,255)						 // �V�A��
#define COLOR_LIGHTCYAN D3DCOLOR_RGBA(224,255,255,255)				 // ���C�g�V�A��
#define COLOR_PALETURQUOISE D3DCOLOR_RGBA(175,238,238,255)			 // �y�[���^�[�R�C�Y
#define COLOR_AQUAMARINE D3DCOLOR_RGBA(127,255,212,255)				 // �A�N�A�}����
#define COLOR_TURQUOISE D3DCOLOR_RGBA(64,224,208,255)				 // �^�[�R�C�Y
#define COLOR_MEDIUMTURQUOISE D3DCOLOR_RGBA(72,209,204,255)			 // �~�f�B�A���^�[�R�C�Y
#define COLOR_DARKTURQUOISE D3DCOLOR_RGBA(0,206,209,255)			 // �_�[�N�^�[�R�C�Y
#define COLOR_CADEBLUE D3DCOLOR_RGBA(95,158,160,255)				 // �J�f�b�g�u���[
#define COLOR_STEELBLUE D3DCOLOR_RGBA(70,130,180,255)				 // �X�e�B�[���u���[
#define COLOR_LIGHTSTEELBLUE D3DCOLOR_RGBA(176,196,222,255)			 // ���C�g�X�e�B�[���u���[
#define COLOR_POWDERBLUE D3DCOLOR_RGBA(176,224,230,255)				 // �p�E�_�[�u���[
#define COLOR_LIGHTBLUE D3DCOLOR_RGBA(173,216,230,255)				 // ���C�g�u���[
#define COLOR_SKYBLUE D3DCOLOR_RGBA(135,206,235,255)				 // �X�J�C�u���[
#define COLOR_LIGHTSKYBLUE D3DCOLOR_RGBA(135,206,250,255)			 // ���C�g�X�J�C�u���[
#define COLOR_DEEPSKYBLUE D3DCOLOR_RGBA(0,191,255,255)				 // �f�B�[�v�X�J�C�u���[
#define COLOR_DODGERBLUE D3DCOLOR_RGBA(30,144,255,255)				 // �h�W���[�u���[
#define COLOR_CORNFLOWERBLUE D3DCOLOR_RGBA(100,149,237,255)			 // �R�[���t�����[�u���[
#define COLOR_MEDIUMSLATEBLUE D3DCOLOR_RGBA(123,104,238,255)		 // �~�f�B�A���X���[�g�u���[
#define COLOR_ROYALBLUE D3DCOLOR_RGBA(65,105,225,255)				 // ���C�����u���[
#define COLOR_BLUE D3DCOLOR_RGBA(0,0,255,255)						 // ��
#define COLOR_MEDIUMBLUE D3DCOLOR_RGBA(0,0,205,255)					 // �~�f�B�A���u���[
#define COLOR_DARKBLUE D3DCOLOR_RGBA(0,0,139,255)					 // �_�[�N�u���[
#define COLOR_NAVY D3DCOLOR_RGBA(0,0,128,255)						 // �l�[�r�[
#define COLOR_MIDINIGHTBLUE D3DCOLOR_RGBA(25,25,112,255)			 // �~�b�h�i�C�g�u���[
#define COLOR_CORNSILK D3DCOLOR_RGBA(255,248,220,255)				 // �R�[���V���N
#define COLOR_BLANCHEDALMOND D3DCOLOR_RGBA(255,235,205,255)			 // �u�����`�h�A�[�����h
#define COLOR_BISQUE D3DCOLOR_RGBA(255,228,196,255)					 // �r�X�N
#define COLOR_NAVAJOWHITE D3DCOLOR_RGBA(255,222,173,255)			 // �i�o�z�z���C�g
#define COLOR_WHEAT D3DCOLOR_RGBA(245,222,179,255)				     // �E�B�[�g
#define COLOR_BURLYWOOD D3DCOLOR_RGBA(222,184,135,255)			     // �o�[���[�E�b�h
#define COLOR_TAN D3DCOLOR_RGBA(210,180,140,255)					 // �e��
#define COLOR_ROSYBROWN D3DCOLOR_RGBA(188,143,143,255)				 // ���[�W�u���E��
#define COLOR_SANDYBROWN D3DCOLOR_RGBA(244,164,96,255)				 // �T���f�B�[�u���E��
#define COLOR_GOLDENROD D3DCOLOR_RGBA(218,165,32,255)				 // �S�[���f�����b�h
#define COLOR_DARKGOLDENROD D3DCOLOR_RGBA(184,134,11,255)			 // �_�[�N�S�[���f�����b�h
#define COLOR_PERU D3DCOLOR_RGBA(205,133,63,255)					 // �y��
#define COLOR_CHOCOLATE D3DCOLOR_RGBA(210,105,30,255)				 // �`���R���[�g
#define COLOR_SADDLEBEOWN D3DCOLOR_RGBA(139,69,19,255)				 // �T�h���u���E��
#define COLOR_SIENNA D3DCOLOR_RGBA(160,82,45,255)					 // �V�G�i
#define COLOR_BWOWN D3DCOLOR_RGBA(165,42,42,255)					 // �u���E��
#define COLOR_MAROON D3DCOLOR_RGBA(128,0,0,255)						 // �}���[��
#define COLOR_WHITE D3DCOLOR_RGBA(255,255,255,255)					 // �z���C�g
#define COLOR_SNOW D3DCOLOR_RGBA(255,250,250,255)					 // ��
#define COLOR_HONEYDEW D3DCOLOR_RGBA(240,255,240,255)				 // �}���[��
#define COLOR_MINTCREAM D3DCOLOR_RGBA(245,255,250,255)				 // �~���g�O���[��
#define COLOR_AZURE D3DCOLOR_RGBA(240,255,255,255)					 // �A�W���[��
#define COLOR_ALICEBLUE D3DCOLOR_RGBA(240,248,255,255)				 // �A���X�u���[
#define COLOR_GHOSTWHITE D3DCOLOR_RGBA(248,248,255,255)				 // �S�[�X�g�z���C�g
#define COLOR_WHITESMOKE D3DCOLOR_RGBA(245,245,245,255)				 // �z���C�g�X���[�N
#define COLOR_SEASHELL D3DCOLOR_RGBA(255,245,238,255)				 // �Z�[�V�F��
#define COLOR_BEIGE D3DCOLOR_RGBA(245,245,220,255)					 // �x�C�W
#define COLOR_OLDLACE D3DCOLOR_RGBA(253,245,230,255)				 // �I�[���h���[�X
#define COLOR_FLORALWHITE D3DCOLOR_RGBA(255,250,240,255)			 // �t���[�����z���C�g
#define COLOR_IVORY D3DCOLOR_RGBA(255,255,240,255)					 // �A�C�{���[
#define COLOR_ANTIQUEWHITE D3DCOLOR_RGBA(250,235,215,255)			 // �A���e�B�[�N�z���C�g
#define COLOR_LINEN D3DCOLOR_RGBA(250,240,230,255)					 // ���l��
#define COLOR_LAVENDERBLUSH D3DCOLOR_RGBA(255,240,245,255)			 // ���x���_�[�u���b�V��
#define COLOR_MISTYROSE D3DCOLOR_RGBA(255,228,225,255)				 // �~�X�e�B�[���[�Y
#define COLOR_GAINSBORO D3DCOLOR_RGBA(220,220,220,255)				 // �_�[�N�J�[�L
#define COLOR_LIGHTGREY D3DCOLOR_RGBA(211,211,211,255)				 // �Q�C���Y�{��
#define COLOR_SILVER D3DCOLOR_RGBA(192,192,192,255)					 // ��
#define COLOR_DARKGRAY D3DCOLOR_RGBA(169,169,169,255)				 // �_�[�N�O���[
#define COLOR_GRAY D3DCOLOR_RGBA(128,128,128,255)					 // �O���[
#define COLOR_DIMGRAY D3DCOLOR_RGBA(105,105,105,255)				 // �f�B���O���[
#define COLOR_LIGHTSLATEGRAY D3DCOLOR_RGBA(119,136,153,255)			 // ���C�g�X���[�g�O���[
#define COLOR_SLATEGRAY D3DCOLOR_RGBA(112,128,144,255)				 // �X���[�g�O���[
#define COLOR_DARKSLATEGRAY D3DCOLOR_RGBA(47,79,79,255)				 // �_�[�N�X���[�g�O���[
#define COLOR_BLACK D3DCOLOR_RGBA(0,0,0,255)						 // �u���b�N


//**********************
// ��ʂ̎��
//**********************
typedef enum
{
	MODE_MOTIONEDIT = 0,		// �^�C�g��
	MODE_MAX,
}MODE;

//**********************
// �v���g�^�C�v�錾
//**********************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//�@�E�B���h�E�v���V�[�W��
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);							//�@����������
void Uninit(void);																	//�@�I������
void Update(void);																	//�@�X�V����
void Draw(void);																	//�@�`�揈��
LPDIRECT3DDEVICE9 GetDevice(void);													//�@�f�o�C�X�̎擾
void SetMode(MODE mode);															//�@���[�h�̐ݒ�
MODE GetMode(void);																	//�@���[�h�̎擾
void DrawFPS(void);																	//�@�f�o�b�O�\��
void DrawEnemySet(int g_nNumEnemy);													//�@�G�̑����̕\��

void DrawDebugPlayerPos(void);														//�@�v���C���[�̍��W�̕\��
void DrawCameraPos(void);															//�@�J�����̍��W�\��
void DrawCameraType(void);															//�@�J�����̎�ނ̕\��

void onWireFrame();																	// ���C���[�t���[���N��
void offWireFrame();																// ���C���[�t���[���I��
void SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density,bool enable);

void GetRayFromMouse(int mouseX, int mouseY, D3DXVECTOR3* outOrigin, D3DXVECTOR3* outDir);
#endif
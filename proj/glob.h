#pragma once

#include "stdafx.h"
#include "mesh.h"
#include "manipul.h"
#include "configur.h"
#include "light.h"
#include "camera.h"
#include "xmlreader.h"

#define KEYBOARD_BUFFER_SIZE	1
#define GREEN					1
#define RED						0
// направления движения
#define LEFT					0
#define RIGHT					1
#define UP						2
#define ANYWHERE				3

#define COUNTOFRULS				12

extern LPDIRECT3D9			pDirect3D;
extern LPDIRECT3DDEVICE9	pDirect3DDevice;
extern LPD3DXFONT			pFont;
extern LPDIRECTINPUT		pDInput;
extern LPDIRECTINPUTDEVICE	pDIKeyboard;


extern RECT rec;
extern HFONT hFont;
extern Mesh car1;
extern Mesh car2;
extern Mesh road;

extern Mesh carMain;
extern Mesh carL;
extern Mesh carR;
extern Mesh carT;

//extern Mesh trafficR;
//extern Mesh trafficG;
extern Mesh trafficDown;
extern Mesh trafficUp;
extern Mesh trafficLeft;
extern Mesh trafficRight;

extern CCamera Camera;
extern CLight Light;
extern CParser CPrsr;
extern Configuration config;

extern int stage;
//extern unsigned int counter;

extern int g_car;
extern int g_rotCam;

extern char s_road;
extern bool s_scourse;

extern bool s_left;
extern bool s_right;
extern bool s_up;
extern bool s_colorG;
extern bool s_colorR;
extern bool s_colorCh; // сигнал смены цвета
extern bool s_action;
extern bool s_play;
extern bool s_start;

extern char keyboard[256];
extern bool keyEvent[256];


//TEST
//===============================================================================
extern int counttest;  
extern BSTR bstrItemText;
extern bool first;
//===============================================================================

//extern bool keyevent[256];	// scan code of a pressed key
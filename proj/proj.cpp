// proj.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "proj.h"
#include "delete.h"
#include "initdirect.h"
//#include "light.h"
#include "mesh.h"
#include "renderbegin.h"
#include "renderroad.h"
#include "configur.h"
#include "InitInput.h"
#include "setcourse.h"
#include "light.h"
#include "actionscene.h"
#include "camera.h"

#define PATHCAR1 L"car1_1.x"
#define PATHCAR2 L"car2_1.x"
#define PATHROAD L"road.x"
#define PATHTRAFFICR L"trafficR.x"
#define PATHTRAFFICG L"trafficG.x"
#define PATHTRAFFIC L"traffic.x"

// ����������
LPDIRECT3D9			pDirect3D			= NULL;
LPDIRECT3DDEVICE9	pDirect3DDevice		= NULL;
LPDIRECTINPUT		pDInput				= NULL;
LPDIRECTINPUTDEVICE	pDIKeyboard			= NULL;

LPD3DXFONT pFont						= NULL;
RECT rec;
HFONT hFont;
Mesh car1;
Mesh car2;
Mesh carMain;

Mesh carL; // ������ �����
Mesh carR; // ������ ������
Mesh carT; // ������ ������

Mesh road;

Mesh trafficDown;
Mesh trafficUp;
Mesh trafficLeft;
Mesh trafficRight;

CCamera Camera;

CLight Light;

CParser CPrsr;

//TEST
//===============================================================================
int counttest; 
BSTR bstrItemText;
bool first = true;
//===============================================================================

int g_car; // ����� ������� �������� ( 0, 1 )
int stage = 0;
int g_rotCam = 15; // ������� ������ � road

Configuration config; // ������ ���������� �� �������� �� ������

//unsigned int counter = 0;

// ������� 
char s_road = 0; //1 ������ road
bool s_start = false; // �������� �������������� ����� 
bool s_scourse = false; //true - ������ ������������ ��������
bool s_left = false;
bool s_right = false;
bool s_up = false;
bool s_colorR = false;
bool s_colorG = false;
bool s_colorCh = false;
bool s_action = false;
bool s_play = false;
char keyboard[256];	//��� inputa
//bool keyevent[256];	// scan code of a pressed key
bool keyEvent[256]; // ��� � ���������
//���������
LRESULT CALLBACK MainWinProc(HWND hwnd,	UINT msg, WPARAM wparam, LPARAM lparam);

//���:		WinMain
//�� ����:	hinstance - ���������� ����������
//			hprevinstance
//			LpCmdLine
//			nCmdShow
int WINAPI WinMain (HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR LpCmdLine, int nCmdShow)
{

	int stage;
	
	// c������ ����� ���� � ��������� ��� ���� ���������
	WNDCLASSEX windowclass;													//������� ������ ����

	windowclass.cbSize = sizeof(WNDCLASSEX);								//������ ����������� ��������� 
	windowclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_DBLCLKS;	//����� ����
	/*windowclass.style = CS_VREDRAW | CS_HREDRAW;*/
	windowclass.lpfnWndProc = MainWinProc;									//��������� �� ������� ��������� ������
	windowclass.cbClsExtra = 0;												//����� ������������
	windowclass.cbWndExtra = 0;												//����� ������������
	windowclass.hInstance = hinstance;										//��������, ���������� �� ��������� ������������ ����������
	windowclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);					//������
	windowclass.hCursor = LoadCursor(NULL, IDC_ARROW);						//������
	windowclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);		//���� ���� ����
	windowclass.lpszMenuName = NULL;										//����������� ������������ ���� � ����
	windowclass.lpszClassName = L"WINDOWSCLASS";							//�������� ������ ����
	windowclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);					//��������� ����� �����������

	//����������� ����� ����
	if(!RegisterClassEx(& windowclass))
		return 0;

	//������� ����
	HWND hwnd;	// ���������� ����
	MSG msg;	// �������������� ���������
	
	if(!(hwnd = CreateWindowEx(
								NULL,								//���� ������ ����. ������������ �����
								L"WINDOWSCLASS",					//��� ������ ������������ ����
								L"������� DirectX",					//��������� ����
								WS_OVERLAPPEDWINDOW | WS_VISIBLE,	//�������� ����� � ��������� ������������ ����
								0, 0,								//����������
								800, 500,							//������ � ������
								NULL,								//���������� ������������� ����
								NULL,								//���������� ����
								hinstance,							//��������� ������ ���������
								NULL)))
	{
		return 0;
	}
	
	//���� ������������� ���������� ������ �������, �� �������� ����
	if SUCCEEDED(IntialDirect3D(hwnd))
	{
		if FAILED( InitInput( hwnd, hinstance)) // ������������� DirectInput
			return E_FAIL;
		if FAILED( car1.InitMesh(PATHCAR1))
			return E_FAIL;
		if FAILED( car2.InitMesh(PATHCAR2))
			return E_FAIL;
		if FAILED( road.InitMesh(PATHROAD))
			return E_FAIL;
		if( FAILED( trafficDown.InitMesh( PATHTRAFFIC ) ) )
			return E_FAIL;
		if( FAILED( trafficUp.InitMesh( PATHTRAFFIC ) ) )
			return E_FAIL;
		if( FAILED( trafficLeft.InitMesh( PATHTRAFFIC ) ) )
			return E_FAIL;
		if( FAILED( trafficRight.InitMesh( PATHTRAFFIC ) ) )
			return E_FAIL;
		for( int i = 0; i < 256; ++i )
		{
			keyEvent[i] = false;
		}
		//Light.LightGlob(5,5,5);
		ShowWindow(hwnd, SW_SHOWDEFAULT);	//���������� ����
		UpdateWindow(hwnd);					//��������� ����
		ZeroMemory(&msg, sizeof(msg));
		stage = 1; 
		g_car = 0;
		
		while(msg.message != WM_QUIT)
		{
				 
			if(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
			{
				TranslateMessage( &msg );	//�������������� ������������� �����
				DispatchMessage( &msg );		//��������� � ��������� � �-�� WinProc
			}
			else
			{
				switch( stage )
				{
				case 1: case 0:
					RenderBegin( stage );
					break;
				case 3: case 4:
					RenderRoad( stage );
					break;
				case 5: case 6: case 7: case 8: case 9:
					if( FAILED( SetCourse( stage ) ) )
						return E_FAIL;
					break;
				case 10: case 11: case 12:
					if( FAILED( ActionScene( stage ) ) )
						return E_FAIL;
					//PostQuitMessage( 0 );
					break;
				}
			}
		}
	}
	
	return msg.wParam;
}


//���:		MainWinProc
//�� ����:	hwnd - ���������� ����
//			msg - ����������� �������
//			wparam
//			lparam
LRESULT CALLBACK MainWinProc(HWND hwnd,	UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_DESTROY:				//��������� ���������� ����� ���������� ������� ����
			DeleteDirect3D();			//����������� �������
			PostQuitMessage(0);			//������ � ������� ��������� WM_QUIT, ������� ������� ���� ����������
			return 0;
	}
	return (DefWindowProc(hwnd, msg, wParam, lParam));	// �-�� DefWindowProc ������������ �� ��������� �� ��������� ������� �� �� ����������
}

//case WM_PAINT:					//��������� ���������� ��� ������������� ����������� ����
		//	//RenderingDirect3D();		//�������������� ����� (�� ������� ����� ��� �����, �� ���� ���� �����)
		//	ValidateRect(hwnd, NULL);	//���������� ��� ����������� ���� ���������� ������� ����
		//	return 0;

		/*case WM_KEYDOWN:
        
			switch( wparam )
			{
					case VK_ESCAPE:
						PostQuitMessage(0);
						break;
			}*/
			//if( s_start == 1 )
			//{
			//	switch( wparam )
			//	{
			//		case VK_ESCAPE:
			//			PostQuitMessage(0);
			//			break;
			//	
			//		case VK_UP:
			//			g_car = 0;
			//			break;

			//		case VK_DOWN:
			//			g_car = 1;
			//			break;

			//		case VK_RETURN:
			//			s_start = 0;
			//			s_road = 1;
			//			break;	
			//	}
			//}

			//else if( s_road == 1 )
			//{
			//	switch( wparam )
			//	{

			//	case VK_ESCAPE:
			//			PostQuitMessage(0);
			//			break;

			//	case VK_UP:
			//		//g_rotCam = g_rotCam + 10;
			//		break; 

			//	case VK_DOWN:
			//		//g_rotCam += 10;
			//		break;

			//	case VK_F1:
			//		config.leftCar = !config.leftCar;
			//		//g_rotCam += 10;
			//		break;
			//	}
			//}
        //}
		//break;

		/*if (FAILED ( trafficR.InitMesh(PATHTRAFFICR)))
			return E_FAIL;
		if (FAILED ( trafficG.InitMesh(PATHTRAFFICG)))
			return E_FAIL;*/
		/*if ( FAILED( traffic.InitMesh( PATHTRAFFIC ) ) )
			return E_FAIL;*/
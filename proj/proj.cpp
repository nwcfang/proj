// proj.cpp: определяет точку входа для приложения.
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

// Глобальные
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

Mesh carL; // машина слева
Mesh carR; // машина справа
Mesh carT; // машина сверху

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

int g_car; // какаю машинку выбераем ( 0, 1 )
int stage = 0;
int g_rotCam = 15; // вращает камеру в road

Configuration config; // объект отвечающий за ситуацию на дороге

//unsigned int counter = 0;

// Сигналы 
char s_road = 0; //1 начать road
bool s_start = false; // начинает воспроизводить сцену 
bool s_scourse = false; //true - начать утанавливать маршруты
bool s_left = false;
bool s_right = false;
bool s_up = false;
bool s_colorR = false;
bool s_colorG = false;
bool s_colorCh = false;
bool s_action = false;
bool s_play = false;
char keyboard[256];	//для inputa
//bool keyevent[256];	// scan code of a pressed key
bool keyEvent[256]; // это я использую
//Прототипы
LRESULT CALLBACK MainWinProc(HWND hwnd,	UINT msg, WPARAM wparam, LPARAM lparam);

//Имя:		WinMain
//На вход:	hinstance - дескриптор приложения
//			hprevinstance
//			LpCmdLine
//			nCmdShow
int WINAPI WinMain (HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR LpCmdLine, int nCmdShow)
{

	int stage;
	
	// cоздаем класс окна и заполняем все поля структуры
	WNDCLASSEX windowclass;													//создаем объект окна

	windowclass.cbSize = sizeof(WNDCLASSEX);								//размер создаваемой структуры 
	windowclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_DBLCLKS;	//стили окна
	/*windowclass.style = CS_VREDRAW | CS_HREDRAW;*/
	windowclass.lpfnWndProc = MainWinProc;									//указатель на функцию обратного вызова
	windowclass.cbClsExtra = 0;												//редко используется
	windowclass.cbWndExtra = 0;												//редко используется
	windowclass.hInstance = hinstance;										//параметр, отвечающий за экземпляр создаваемого приложения
	windowclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);					//иконка
	windowclass.hCursor = LoadCursor(NULL, IDC_ARROW);						//курсор
	windowclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);		//цвет фона окна
	windowclass.lpszMenuName = NULL;										//подключение стандартного меню к окну
	windowclass.lpszClassName = L"WINDOWSCLASS";							//название класса окна
	windowclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);					//дисриптор малой пиктограммы

	//Регистируем класс окна
	if(!RegisterClassEx(& windowclass))
		return 0;

	//Создаем окно
	HWND hwnd;	// дескриптор окна
	MSG msg;	// индентификатор сообщения
	
	if(!(hwnd = CreateWindowEx(
								NULL,								//флаг стилей окна. Используется редко
								L"WINDOWSCLASS",					//имя класса создаваемого окна
								L"Изучаем DirectX",					//заголовок окна
								WS_OVERLAPPEDWINDOW | WS_VISIBLE,	//описание стиля и поведения создаваемого окна
								0, 0,								//координаты
								800, 500,							//ширина и высота
								NULL,								//дескриптор родительского окна
								NULL,								//дескриптор меню
								hinstance,							//экземпляр нашего приложния
								NULL)))
	{
		return 0;
	}
	
	//если инициализация устройства прошла успешно, то показать окно
	if SUCCEEDED(IntialDirect3D(hwnd))
	{
		if FAILED( InitInput( hwnd, hinstance)) // инициализация DirectInput
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
		ShowWindow(hwnd, SW_SHOWDEFAULT);	//показываем окно
		UpdateWindow(hwnd);					//обновляем окно
		ZeroMemory(&msg, sizeof(msg));
		stage = 1; 
		g_car = 0;
		
		while(msg.message != WM_QUIT)
		{
				 
			if(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
			{
				TranslateMessage( &msg );	//преобразование клавиатурного ввода
				DispatchMessage( &msg );		//обработка и пересылка в ф-ию WinProc
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


//Имя:		MainWinProc
//На вход:	hwnd - дескриптор окна
//			msg - индификатор события
//			wparam
//			lparam
LRESULT CALLBACK MainWinProc(HWND hwnd,	UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_DESTROY:				//сообщение посылается когда необходимо закрыть окно
			DeleteDirect3D();			//освобождаем ресурсы
			PostQuitMessage(0);			//ставит в очередь сообщение WM_QUIT, которое закроет наше приложение
			return 0;
	}
	return (DefWindowProc(hwnd, msg, wParam, lParam));	// ф-ия DefWindowProc обрабатывает по умолчанию то сообщения которые мы не используем
}

//case WM_PAINT:					//сообщение посылается при необходимости перерисовки окна
		//	//RenderingDirect3D();		//перерисовываем сцену (не понятно зачем тут нужно, но путь пока будет)
		//	ValidateRect(hwnd, NULL);	//вызывается для перерисовки всей клиентской области окна
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
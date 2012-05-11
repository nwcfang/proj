// proj.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "proj.h"


//#include <windows.h>
//#include <d3d9.h>
//#include <d3dx9.h>
//#include <MMSystem.h>
//#include <d3dx9mesh.h>
//#include <d3dx9core.h>
//#include <dinput.h>

#define MAX_LOADSTRING 100

//Глобальная область
LPDIRECT3D9 pDirect3D						= NULL;		// объявляем указатель на интерфейс IDirect3D9 для работы с трехмерной графикой
LPDIRECT3DDEVICE9 pDirect3DDevice			= NULL;		// объявляем указатель на интерфейс устройства для работы с трехмерной графикой

LPD3DXMESH pCar1							= NULL;		// указатель на интерфейс меш
D3DMATERIAL9* pMeshMaterialsCar1			= NULL;		// указатель для работы с материалом
LPDIRECT3DTEXTURE9* pMeshTextureCar1		= NULL;		// указфтель для работы с текстурой
LPD3DXBUFFER pMeshBuffer					= NULL;		// содержит информацию о материале и текстуре
DWORD dwNumberCar1;


// Прототипы
LRESULT CALLBACK MainWinProc(HWND hwnd,	UINT msg, WPARAM wparam, LPARAM lparam);
HRESULT IntialDirect3D(HWND hwnd, LPDIRECT3D9 pDirect3D, LPDIRECT3DDEVICE9 pDirect3DDevice);
VOID RenderingDirect3D();
VOID DeleteDirect3D();
VOID DrawCar1();
HRESULT InitialMesh();


//Имя:		WinMain
//На вход:	hinstance - дескриптор приложения
//			hprevinstance
//			LpCmdLine
//			nCmdShow
int WINAPI WinMain (HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR LpCmdLine, int nCmdShow)
{
	
	// cоздаем класс окна и заполняем все поля структуры
	WNDCLASSEX windowclass;													//создаем объект окна

	windowclass.cbSize = sizeof(WNDCLASSEX);								//размер создаваемой структуры 
	windowclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_DBLCLKS;	//стили окна
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
								500, 500,							//ширина и высота
								NULL,								//дескриптор родительского окна
								NULL,								//дескриптор меню
								hinstance,							//экземпляр нашего приложния
								NULL)))
	{
		return 0;
	}
	
	//если инициализация устройства прошла успешно, то показать окно
	if SUCCEEDED(IntialDirect3D(hwnd, pDirect3D, pDirect3DDevice))
	{
		if SUCCEEDED(InitialMesh())
		{
			ShowWindow(hwnd, SW_SHOWDEFAULT);	//показываем окно
			UpdateWindow(hwnd);					//Обновляем окно
			ZeroMemory(&msg, sizeof(msg));
	
			while(msg.message != WM_QUIT)
			{
				if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(& msg);	//преобразование клавиатурного ввода
					DispatchMessage(& msg);		//обработка и пересылка в ф-ию WinProc
				}
				else
				{
					RenderingDirect3D();
				}
			}
		}
	}
	
	return (msg.wParam);
}


//Имя:		MainWinProc
//На вход:	hwnd - дескриптор окна
//			msg - индификатор события
//			wparam
//			lparam
LRESULT CALLBACK MainWinProc(HWND hwnd,	UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
		case WM_PAINT:					//сообщение посылается при необходимости перерисовки окна
			RenderingDirect3D();	//перерисовываем сцену
			ValidateRect(hwnd, NULL);	//вызывается для перерисовки всей клиентской области окна
			return 0;

		case WM_DESTROY:				//сообщение посылается когда необходимо закрыть окно
			DeleteDirect3D();			//освобождаем ресурсы
			PostQuitMessage(0);			//ставит в очередь сообщение WM_QUIT, которое закроет наше приложение
			return 0;
	}
	return (DefWindowProc(hwnd, msg, wparam, lparam));	// ф-ия DefWindowProc обрабатывает по умолчанию то сообщения которые мы не используем
}


//Имя:		IntialDirect3D
//На вход:	hwnd - дескриптор окна
//HRESULT IntialDirect3D( HWND hwnd, LPDIRECT3D9 pDirect3D, LPDIRECT3DDEVICE9 pDirect3DDevice )
//{
//	//создаем основной указатель на интерфейс IDirect3D9. 
//	//D3D_SDK_VERSION - макрос указывающий на текущую версию SDK
//	if (NULL == (pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
//		return E_FAIL;
//	
//	D3DDISPLAYMODE Display;		//будет использоваться при создании заднего буфера для соответствия текущих установок дисплея
//	
//	if (FAILED(pDirect3D->GetAdapterDisplayMode(					//получаем текущих формат дисплея
//											D3DADAPTER_DEFAULT,		//порядковый номер адаптора дисплея
//											&Display				//структура, которая будет заполнения информацией по текущему адаптору
//											)))	
//		return E_FAIL;
//
//	D3DPRESENT_PARAMETERS Direct3DParametr;   //объект, заполняя поля которого, можно определить, как будет вести себя наше 3D-приложение
//
//	//Очищаем нашу структуру от всевозможного мусора
//	//Первый параметр - это ссылка на параметр представления, иначе говоря: то, что нужно очистить.
//	//Второй - это размер того, что будет очищаться
//	ZeroMemory(&Direct3DParametr, sizeof(Direct3DParametr));
//
//	Direct3DParametr.Windowed = TRUE; //видеорежим
//	Direct3DParametr.SwapEffect = D3DSWAPEFFECT_DISCARD;	//служит для определения обмена буферов
//	Direct3DParametr.BackBufferFormat = D3DFMT_UNKNOWN;		//формат поверхности заднего буфера
//	Direct3DParametr.EnableAutoDepthStencil = TRUE;
//	Direct3DParametr.AutoDepthStencilFormat = D3DFMT_D16;
//
//	//создаем объект для интерфейса устройства
//	if(FAILED(pDirect3D -> CreateDevice(D3DADAPTER_DEFAULT,						//наша видеокарта
//										D3DDEVTYPE_HAL,							//определяет тип устройста. D3DDEVTYPE_HAL - использует возможности аппаратного обеспечения.
//										hwnd,									//дескриптор главного окна
//										D3DCREATE_SOFTWARE_VERTEXPROCESSING,	//указывает как будут обрабатываться вершины. D3DCREATE_HARDWARE_VERTEXPROCESSING - исп. видеокарта и аппаратная обработка вершин
//										&Direct3DParametr,
//										&pDirect3DDevice
//										)))
//	{
//		return E_FAIL;
//	}
//	
//	pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			//отключить освещение Direct3D
//	pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);		//создаёт буффер глубины 
//	pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff );		
//	//pDirect3DDevice -> SetRenderState ( D3DRS_LIGHTING, TRUE );
//	//pDirect3DDevice -> SetRenderState ( D3DRS_AMBIENT, 0 );
//
//	return S_OK;
//}


//Имя:		InitialMesh
//На вход:
HRESULT InitialMesh()
{
	D3DXMATERIAL* D3DXMeshMaterials;

	// загружаем первую машинку
	if( FAILED( D3DXLoadMeshFromX( 
				L"car1.x",				// x-файл
				D3DXMESH_SYSTEMMEM,		// выделение памяти для ресурсов происходит из RAM
				pDirect3DDevice,		// указатель на Direct3D девайс
				NULL,					// адрес указателя на буффер, содержащий данные о смежности
				&pMeshBuffer,			// адрес указателя на буффер, содержащий данные о материале и текстуре
				NULL,					// адрес указателя на интерфейс ID3DXBuffer, содержащий специфические эффекты
				&dwNumberCar1,				// указатель на массив данных для структуры D3DXMATERIAL
				&pCar1 ) ) )			// рузультат всей операции
	{
		MessageBox( NULL, L"Could not find car1.x", L"ERROR", MB_OK );
		return E_FAIL;
	}
		
	D3DXMeshMaterials = ( D3DXMATERIAL* ) pMeshBuffer->GetBufferPointer();
	pMeshMaterialsCar1 = new D3DMATERIAL9 [dwNumberCar1];
	pMeshTextureCar1 = new LPDIRECT3DTEXTURE9 [dwNumberCar1];

	for( DWORD i = 0; i < dwNumberCar1; ++ i )
	{
		pMeshMaterialsCar1[i] = D3DXMeshMaterials[i].MatD3D;
		pMeshMaterialsCar1[i].Ambient = pMeshMaterialsCar1[i].Diffuse;

		if( FAILED( D3DXCreateTextureFromFileA( pDirect3DDevice, D3DXMeshMaterials[i].pTextureFilename, &pMeshTextureCar1[i]) ) )
			pMeshTextureCar1[i] = NULL;
	}

	// уничтожаем материальный буффер
	pMeshBuffer->Release();

	// загружаем вторую машинку
	//if( FAILED( D3DXLoadMeshFromX( L"car2.x", D3DXMESH_SYSTEMMEM, pDirect3DDevice, NULL, &pMeshBuffer, NULL, &dwNumberCar2, &pCar2 ) ) )
	//{
	//	MessageBox( NULL, L"Could not find car2.x", L"ERROR", MB_OK );
	//	return E_FAIL;
	//}

	//D3DXMeshMaterials = ( D3DXMATERIAL* ) pMeshBuffer->GetBufferPointer();
	//pMeshMaterialsCar2 = new D3DMATERIAL9 [dwNumberCar2];
	//pMeshTextureCar2 = new LPDIRECT3DTEXTURE9 [dwNumberCar2];
	//
	//for( DWORD i = 0; i < dwNumberCar2; ++ i )
	//{
	//	pMeshMaterialsCar2[i] = D3DXMeshMaterials[i].MatD3D;
	//	pMeshMaterialsCar2[i].Ambient = pMeshMaterialsCar1[i].Diffuse;

	//	if( FAILED( D3DXCreateTextureFromFileA( pDirect3DDevice, D3DXMeshMaterials[i].pTextureFilename, &pMeshTextureCar2[i]) ) )
	//		pMeshTextureCar2[i] = NULL;
	//}

	//// уничтожаем материальный буффер
	//pMeshBuffer->Release();

	return S_OK;
}


//Имя:		RenderingDirect3D
//На вход:
VOID RenderingDirect3D()
{
	if(pDirect3DDevice == NULL)
		return;

	//очищаем задний буфер
	pDirect3DDevice -> Clear(0,										// Номер массива прямоугольников, подвергающихся очистке. 0 - используется вся поверхность
							NULL,									//адрес массива прямоугольников
							D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,		//параметр указывающий какие из поверхностей будут очищены. D3DCLEAR_TARGET - очистить буфер поверхности до цвета
							D3DCOLOR_XRGB(255, 255, 255),			//макрос цвета.
							1.0f,									//значение для Z-буфера
							0);										//значение буфера трафарета

	pDirect3DDevice -> BeginScene();		//cигнализируем о том, что начался рендериг объектов сцены

	Matrix( pDirect3DDevice );								//вращения 
	DrawCar1();
		
	pDirect3DDevice ->	EndScene();			//Сигнализируем о том, что закончился рендериг объектов сцены

	//копируем задний буфер в передний
	pDirect3DDevice ->Present(NULL,		//указатель на структуру исходной поверхности. NULL - будет представлена полная исходная поверхность
							NULL,		//указатель на структуру поверхности адресата. NULL - копирует содержимое заднего буфера "один в один"
							NULL,		//указатель на окно адресата. NULL - используется поле hWndDeviceWindow в структуре D3DPRESENT_PARAMETERS
							NULL);		//устаревший параметр
}


//Имя:		DeleteDirect3D
//На вход:
VOID DeleteDirect3D()
{
	if(pMeshMaterialsCar1 != NULL)
		delete [] pMeshMaterialsCar1;
	if( pMeshTextureCar1 )
	{
		for( DWORD i = 0; i < dwNumberCar1; ++ i )
		{
			if( pMeshTextureCar1[i] )
				pMeshTextureCar1[i]->Release();
		}
		delete [] pMeshTextureCar1;
	}
	if(pCar1 != NULL)
		pCar1->Release();

	/*if(pMeshMaterialsCar2 != NULL)
		delete [] pMeshMaterialsCar2;
	if( pMeshTextureCar2 )
	{
		for( DWORD i = 0; i < dwNumberCar2; ++ i )
		{
			if( pMeshTextureCar2[i] )
				pMeshTextureCar2[i]->Release();
		}
		delete [] pMeshTextureCar2;
	}
	if(pCar2 != NULL)
		pCar2->Release();*/

	if(pDirect3DDevice != NULL)
		pDirect3DDevice -> Release();
	if(pDirect3D != NULL)
		pDirect3D -> Release();
}


//Имя:		DrawCar1
//На вход:
VOID DrawCar1()
{
	for( DWORD i = 0; i < dwNumberCar1; ++ i )
	{
		pDirect3DDevice->SetMaterial( &pMeshMaterialsCar1[i] );
		pDirect3DDevice->SetTexture( 0, pMeshTextureCar1[i] );
		pCar1->DrawSubset(i);
	}
}

VOID LightMaterial()
{
	D3DMATERIAL9 Material;
	D3DLIGHT9 Light;

	//установка материала
	ZeroMemory( &Material, sizeof( D3DMATERIAL9 ) );

	Material.Diffuse.r = Material.Ambient.r = 1.0f;
	Material.Diffuse.g = Material.Ambient.g = 1.0f;
	Material.Diffuse.b = Material.Ambient.b = 0.0f;
	Material.Diffuse.a = Material.Ambient.a = 1.0f;

	pDirect3DDevice -> SetMaterial( &Material );

	//установка света
	ZeroMemory( &Light, sizeof( D3DLIGHT9 ) );

	Light.Type = D3DLIGHT_DIRECTIONAL;

	Light.Diffuse.r = 1.0f;
	Light.Diffuse.g = 1.0f;
	Light.Diffuse.b = 1.0f;

	Light.Range = 1000.0f;

	D3DXVECTOR3 VectorDir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	D3DXVec3Normalize( (D3DXVECTOR3 *) &Light.Direction, &VectorDir ); 

	pDirect3DDevice -> SetLight( 0, &Light );
	pDirect3DDevice -> LightEnable ( 0, TRUE );

	pDirect3DDevice -> SetRenderState ( D3DRS_LIGHTING, TRUE );
	pDirect3DDevice -> SetRenderState ( D3DRS_AMBIENT, 0 );


}
#include "stdafx.h"
#include "glob.h"
#include "light.h"


HRESULT IntialDirect3D(HWND hwnd)
{
	//создаем основной указатель на интерфейс IDirect3D9. 
	//D3D_SDK_VERSION - макрос указывающий на текущую версию SDK
	if (NULL == (pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;
	
	D3DDISPLAYMODE Display;		//будет использоваться при создании заднего буфера для соответствия текущих установок дисплея
	
	if (FAILED(pDirect3D -> GetAdapterDisplayMode(					//получаем текущих формат дисплея
											D3DADAPTER_DEFAULT,		//порядковый номер адаптора дисплея
											&Display				//структура, которая будет заполнения информацией по текущему адаптору
											)))	
		return E_FAIL;

	D3DPRESENT_PARAMETERS Direct3DParametr;   //объект, заполняя поля которого, можно определить, как будет вести себя наше 3D-приложение

	//Очищаем нашу структуру от всевозможного мусора
	//Первый параметр - это ссылка на параметр представления, иначе говоря: то, что нужно очистить.
	//Второй - это размер того, что будет очищаться
	ZeroMemory(&Direct3DParametr, sizeof(Direct3DParametr));

	
	Direct3DParametr.SwapEffect = D3DSWAPEFFECT_DISCARD;	//служит для определения обмена буферов
	Direct3DParametr.BackBufferFormat = Display.Format;		//формат поверхности заднего буфера

	Direct3DParametr.Windowed = TRUE; //видеорежим
	// Для полноэкранного режима
	//Direct3DParametr.Windowed = FALSE; //видеорежим
	//Direct3DParametr.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);
	//Direct3DParametr.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);
	//Direct3DParametr.BackBufferCount = 3;
	//Direct3DParametr.FullScreen_RefreshRateInHz = Display.RefreshRate;

	Direct3DParametr.EnableAutoDepthStencil = TRUE;
	Direct3DParametr.AutoDepthStencilFormat = D3DFMT_D16;
	Direct3DParametr.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

	//создаем объект для интерфейса устройства
	if(FAILED(pDirect3D -> CreateDevice(D3DADAPTER_DEFAULT,						//наша видеокарта
										D3DDEVTYPE_HAL,							//определяет тип устройста. D3DDEVTYPE_HAL - использует возможности аппаратного обеспечения.
										hwnd,									//дескриптор главного окна
										D3DCREATE_HARDWARE_VERTEXPROCESSING,	//указывает как будут обрабатываться вершины. D3DCREATE_HARDWARE_VERTEXPROCESSING - исп. видеокарта и аппаратная обработка вершин
										&Direct3DParametr,
										&pDirect3DDevice
										)))
	{
		return E_FAIL;
	}
	
	pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);		//создаёт буффер глубины 
	pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff );		

	Light.LightGlob( 0.0f, 0.0f, 1.0f );
	Light.LightTrafficInit();
	
	D3DXMATRIX MatrixView;			// матрица вида
	D3DXMATRIX MatrixProjection;	// матрица проекции

	//Матрица вида
	D3DXMatrixLookAtLH(&MatrixView,
					&D3DXVECTOR3(0.0f, 0.0f, -1000.0f),			//указатель на структуру D3DXVECTOR3 который определяет точку, из которой происходит просмотр сцены
					&D3DXVECTOR3(0.0f, 0.0f, 0.0f),				//указатель на структуру D3DXVECTOR3 который определяет то на что мы смотрим
					&D3DXVECTOR3(0.0f, 1.0f, 0.0f));			//указатель на структуру D3DXVECTOR3 который определяет то что мы видим
	pDirect3DDevice -> SetTransform(D3DTS_VIEW, &MatrixView);	//применяем матрицу вида

	//Матрица проекций
	D3DXMatrixPerspectiveFovLH(&MatrixProjection,
							D3DX_PI/4,										//определяет поле зрения в направлении оси Y, измеряется в радианах. Типичное поле - D3DX_PI/4
							800.0f / 500.0f,								//коэффициент сжатия
							1.0f,											//передний план отсечения сцены
							3000.0f);										//задний план отсечения
	pDirect3DDevice -> SetTransform(D3DTS_PROJECTION, &MatrixProjection);	//применяем матрицу проекции
	return S_OK;
}
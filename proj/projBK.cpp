// proj.cpp: ���������� ����� ����� ��� ����������.
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

//���������� �������
LPDIRECT3D9 pDirect3D						= NULL;		// ��������� ��������� �� ��������� IDirect3D9 ��� ������ � ���������� ��������
LPDIRECT3DDEVICE9 pDirect3DDevice			= NULL;		// ��������� ��������� �� ��������� ���������� ��� ������ � ���������� ��������

LPD3DXMESH pCar1							= NULL;		// ��������� �� ��������� ���
D3DMATERIAL9* pMeshMaterialsCar1			= NULL;		// ��������� ��� ������ � ����������
LPDIRECT3DTEXTURE9* pMeshTextureCar1		= NULL;		// ��������� ��� ������ � ���������
LPD3DXBUFFER pMeshBuffer					= NULL;		// �������� ���������� � ��������� � ��������
DWORD dwNumberCar1;


// ���������
LRESULT CALLBACK MainWinProc(HWND hwnd,	UINT msg, WPARAM wparam, LPARAM lparam);
HRESULT IntialDirect3D(HWND hwnd, LPDIRECT3D9 pDirect3D, LPDIRECT3DDEVICE9 pDirect3DDevice);
VOID RenderingDirect3D();
VOID DeleteDirect3D();
VOID DrawCar1();
HRESULT InitialMesh();


//���:		WinMain
//�� ����:	hinstance - ���������� ����������
//			hprevinstance
//			LpCmdLine
//			nCmdShow
int WINAPI WinMain (HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR LpCmdLine, int nCmdShow)
{
	
	// c������ ����� ���� � ��������� ��� ���� ���������
	WNDCLASSEX windowclass;													//������� ������ ����

	windowclass.cbSize = sizeof(WNDCLASSEX);								//������ ����������� ��������� 
	windowclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_DBLCLKS;	//����� ����
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
								500, 500,							//������ � ������
								NULL,								//���������� ������������� ����
								NULL,								//���������� ����
								hinstance,							//��������� ������ ���������
								NULL)))
	{
		return 0;
	}
	
	//���� ������������� ���������� ������ �������, �� �������� ����
	if SUCCEEDED(IntialDirect3D(hwnd, pDirect3D, pDirect3DDevice))
	{
		if SUCCEEDED(InitialMesh())
		{
			ShowWindow(hwnd, SW_SHOWDEFAULT);	//���������� ����
			UpdateWindow(hwnd);					//��������� ����
			ZeroMemory(&msg, sizeof(msg));
	
			while(msg.message != WM_QUIT)
			{
				if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(& msg);	//�������������� ������������� �����
					DispatchMessage(& msg);		//��������� � ��������� � �-�� WinProc
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


//���:		MainWinProc
//�� ����:	hwnd - ���������� ����
//			msg - ����������� �������
//			wparam
//			lparam
LRESULT CALLBACK MainWinProc(HWND hwnd,	UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
		case WM_PAINT:					//��������� ���������� ��� ������������� ����������� ����
			RenderingDirect3D();	//�������������� �����
			ValidateRect(hwnd, NULL);	//���������� ��� ����������� ���� ���������� ������� ����
			return 0;

		case WM_DESTROY:				//��������� ���������� ����� ���������� ������� ����
			DeleteDirect3D();			//����������� �������
			PostQuitMessage(0);			//������ � ������� ��������� WM_QUIT, ������� ������� ���� ����������
			return 0;
	}
	return (DefWindowProc(hwnd, msg, wparam, lparam));	// �-�� DefWindowProc ������������ �� ��������� �� ��������� ������� �� �� ����������
}


//���:		IntialDirect3D
//�� ����:	hwnd - ���������� ����
//HRESULT IntialDirect3D( HWND hwnd, LPDIRECT3D9 pDirect3D, LPDIRECT3DDEVICE9 pDirect3DDevice )
//{
//	//������� �������� ��������� �� ��������� IDirect3D9. 
//	//D3D_SDK_VERSION - ������ ����������� �� ������� ������ SDK
//	if (NULL == (pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
//		return E_FAIL;
//	
//	D3DDISPLAYMODE Display;		//����� �������������� ��� �������� ������� ������ ��� ������������ ������� ��������� �������
//	
//	if (FAILED(pDirect3D->GetAdapterDisplayMode(					//�������� ������� ������ �������
//											D3DADAPTER_DEFAULT,		//���������� ����� �������� �������
//											&Display				//���������, ������� ����� ���������� ����������� �� �������� ��������
//											)))	
//		return E_FAIL;
//
//	D3DPRESENT_PARAMETERS Direct3DParametr;   //������, �������� ���� ��������, ����� ����������, ��� ����� ����� ���� ���� 3D-����������
//
//	//������� ���� ��������� �� ������������� ������
//	//������ �������� - ��� ������ �� �������� �������������, ����� ������: ��, ��� ����� ��������.
//	//������ - ��� ������ ����, ��� ����� ���������
//	ZeroMemory(&Direct3DParametr, sizeof(Direct3DParametr));
//
//	Direct3DParametr.Windowed = TRUE; //����������
//	Direct3DParametr.SwapEffect = D3DSWAPEFFECT_DISCARD;	//������ ��� ����������� ������ �������
//	Direct3DParametr.BackBufferFormat = D3DFMT_UNKNOWN;		//������ ����������� ������� ������
//	Direct3DParametr.EnableAutoDepthStencil = TRUE;
//	Direct3DParametr.AutoDepthStencilFormat = D3DFMT_D16;
//
//	//������� ������ ��� ���������� ����������
//	if(FAILED(pDirect3D -> CreateDevice(D3DADAPTER_DEFAULT,						//���� ����������
//										D3DDEVTYPE_HAL,							//���������� ��� ���������. D3DDEVTYPE_HAL - ���������� ����������� ����������� �����������.
//										hwnd,									//���������� �������� ����
//										D3DCREATE_SOFTWARE_VERTEXPROCESSING,	//��������� ��� ����� �������������� �������. D3DCREATE_HARDWARE_VERTEXPROCESSING - ���. ���������� � ���������� ��������� ������
//										&Direct3DParametr,
//										&pDirect3DDevice
//										)))
//	{
//		return E_FAIL;
//	}
//	
//	pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			//��������� ��������� Direct3D
//	pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);		//������ ������ ������� 
//	pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff );		
//	//pDirect3DDevice -> SetRenderState ( D3DRS_LIGHTING, TRUE );
//	//pDirect3DDevice -> SetRenderState ( D3DRS_AMBIENT, 0 );
//
//	return S_OK;
//}


//���:		InitialMesh
//�� ����:
HRESULT InitialMesh()
{
	D3DXMATERIAL* D3DXMeshMaterials;

	// ��������� ������ �������
	if( FAILED( D3DXLoadMeshFromX( 
				L"car1.x",				// x-����
				D3DXMESH_SYSTEMMEM,		// ��������� ������ ��� �������� ���������� �� RAM
				pDirect3DDevice,		// ��������� �� Direct3D ������
				NULL,					// ����� ��������� �� ������, ���������� ������ � ���������
				&pMeshBuffer,			// ����� ��������� �� ������, ���������� ������ � ��������� � ��������
				NULL,					// ����� ��������� �� ��������� ID3DXBuffer, ���������� ������������� �������
				&dwNumberCar1,				// ��������� �� ������ ������ ��� ��������� D3DXMATERIAL
				&pCar1 ) ) )			// ��������� ���� ��������
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

	// ���������� ������������ ������
	pMeshBuffer->Release();

	// ��������� ������ �������
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

	//// ���������� ������������ ������
	//pMeshBuffer->Release();

	return S_OK;
}


//���:		RenderingDirect3D
//�� ����:
VOID RenderingDirect3D()
{
	if(pDirect3DDevice == NULL)
		return;

	//������� ������ �����
	pDirect3DDevice -> Clear(0,										// ����� ������� ���������������, �������������� �������. 0 - ������������ ��� �����������
							NULL,									//����� ������� ���������������
							D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,		//�������� ����������� ����� �� ������������ ����� �������. D3DCLEAR_TARGET - �������� ����� ����������� �� �����
							D3DCOLOR_XRGB(255, 255, 255),			//������ �����.
							1.0f,									//�������� ��� Z-������
							0);										//�������� ������ ���������

	pDirect3DDevice -> BeginScene();		//c������������ � ���, ��� ������� �������� �������� �����

	Matrix( pDirect3DDevice );								//�������� 
	DrawCar1();
		
	pDirect3DDevice ->	EndScene();			//������������� � ���, ��� ���������� �������� �������� �����

	//�������� ������ ����� � ��������
	pDirect3DDevice ->Present(NULL,		//��������� �� ��������� �������� �����������. NULL - ����� ������������ ������ �������� �����������
							NULL,		//��������� �� ��������� ����������� ��������. NULL - �������� ���������� ������� ������ "���� � ����"
							NULL,		//��������� �� ���� ��������. NULL - ������������ ���� hWndDeviceWindow � ��������� D3DPRESENT_PARAMETERS
							NULL);		//���������� ��������
}


//���:		DeleteDirect3D
//�� ����:
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


//���:		DrawCar1
//�� ����:
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

	//��������� ���������
	ZeroMemory( &Material, sizeof( D3DMATERIAL9 ) );

	Material.Diffuse.r = Material.Ambient.r = 1.0f;
	Material.Diffuse.g = Material.Ambient.g = 1.0f;
	Material.Diffuse.b = Material.Ambient.b = 0.0f;
	Material.Diffuse.a = Material.Ambient.a = 1.0f;

	pDirect3DDevice -> SetMaterial( &Material );

	//��������� �����
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
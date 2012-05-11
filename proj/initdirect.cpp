#include "stdafx.h"
#include "glob.h"
#include "light.h"


HRESULT IntialDirect3D(HWND hwnd)
{
	//������� �������� ��������� �� ��������� IDirect3D9. 
	//D3D_SDK_VERSION - ������ ����������� �� ������� ������ SDK
	if (NULL == (pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;
	
	D3DDISPLAYMODE Display;		//����� �������������� ��� �������� ������� ������ ��� ������������ ������� ��������� �������
	
	if (FAILED(pDirect3D -> GetAdapterDisplayMode(					//�������� ������� ������ �������
											D3DADAPTER_DEFAULT,		//���������� ����� �������� �������
											&Display				//���������, ������� ����� ���������� ����������� �� �������� ��������
											)))	
		return E_FAIL;

	D3DPRESENT_PARAMETERS Direct3DParametr;   //������, �������� ���� ��������, ����� ����������, ��� ����� ����� ���� ���� 3D-����������

	//������� ���� ��������� �� ������������� ������
	//������ �������� - ��� ������ �� �������� �������������, ����� ������: ��, ��� ����� ��������.
	//������ - ��� ������ ����, ��� ����� ���������
	ZeroMemory(&Direct3DParametr, sizeof(Direct3DParametr));

	
	Direct3DParametr.SwapEffect = D3DSWAPEFFECT_DISCARD;	//������ ��� ����������� ������ �������
	Direct3DParametr.BackBufferFormat = Display.Format;		//������ ����������� ������� ������

	Direct3DParametr.Windowed = TRUE; //����������
	// ��� �������������� ������
	//Direct3DParametr.Windowed = FALSE; //����������
	//Direct3DParametr.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);
	//Direct3DParametr.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);
	//Direct3DParametr.BackBufferCount = 3;
	//Direct3DParametr.FullScreen_RefreshRateInHz = Display.RefreshRate;

	Direct3DParametr.EnableAutoDepthStencil = TRUE;
	Direct3DParametr.AutoDepthStencilFormat = D3DFMT_D16;
	Direct3DParametr.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

	//������� ������ ��� ���������� ����������
	if(FAILED(pDirect3D -> CreateDevice(D3DADAPTER_DEFAULT,						//���� ����������
										D3DDEVTYPE_HAL,							//���������� ��� ���������. D3DDEVTYPE_HAL - ���������� ����������� ����������� �����������.
										hwnd,									//���������� �������� ����
										D3DCREATE_HARDWARE_VERTEXPROCESSING,	//��������� ��� ����� �������������� �������. D3DCREATE_HARDWARE_VERTEXPROCESSING - ���. ���������� � ���������� ��������� ������
										&Direct3DParametr,
										&pDirect3DDevice
										)))
	{
		return E_FAIL;
	}
	
	pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);		//������ ������ ������� 
	pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff );		

	Light.LightGlob( 0.0f, 0.0f, 1.0f );
	Light.LightTrafficInit();
	
	D3DXMATRIX MatrixView;			// ������� ����
	D3DXMATRIX MatrixProjection;	// ������� ��������

	//������� ����
	D3DXMatrixLookAtLH(&MatrixView,
					&D3DXVECTOR3(0.0f, 0.0f, -1000.0f),			//��������� �� ��������� D3DXVECTOR3 ������� ���������� �����, �� ������� ���������� �������� �����
					&D3DXVECTOR3(0.0f, 0.0f, 0.0f),				//��������� �� ��������� D3DXVECTOR3 ������� ���������� �� �� ��� �� �������
					&D3DXVECTOR3(0.0f, 1.0f, 0.0f));			//��������� �� ��������� D3DXVECTOR3 ������� ���������� �� ��� �� �����
	pDirect3DDevice -> SetTransform(D3DTS_VIEW, &MatrixView);	//��������� ������� ����

	//������� ��������
	D3DXMatrixPerspectiveFovLH(&MatrixProjection,
							D3DX_PI/4,										//���������� ���� ������ � ����������� ��� Y, ���������� � ��������. �������� ���� - D3DX_PI/4
							800.0f / 500.0f,								//����������� ������
							1.0f,											//�������� ���� ��������� �����
							3000.0f);										//������ ���� ���������
	pDirect3DDevice -> SetTransform(D3DTS_PROJECTION, &MatrixProjection);	//��������� ������� ��������
	return S_OK;
}
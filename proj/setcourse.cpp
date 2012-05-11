#include "stdafx.h"
#include "glob.h"
#include "delete.h"
#include "initinput.h"
#include "tools.h"
#include "light.h"

HRESULT ChooseDirection ( Mesh &Obj, int &cdStage );
HRESULT ChooseColor( Mesh &trafficD,  Mesh &trafficU, Mesh &trafficL, Mesh &trafficR, CLight &ccLight );
HRESULT RenderScene();

// ��������� ������������ �������� ������ ������
HRESULT SetCourse( int &scStage )
{
	D3DXMATRIX scMatrixView;
	pDirect3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,		
							D3DCOLOR_XRGB( 191, 245, 251 ),	1.0f, 0);

	pDirect3DDevice->BeginScene();
	
	if( scStage == 5 )
	{
		D3DXMatrixLookAtLH(&scMatrixView,
					&D3DXVECTOR3( -20.0f, 15.0f, -20.f ),				// ���������� �����, �� ������� ���������� �������� �����
					&D3DXVECTOR3(0.0f, 0.0f, -10.0f),					// ���������� �� �� ��� �� �������
					&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		pDirect3DDevice->SetTransform( D3DTS_VIEW, &scMatrixView );
		RenderScene();

		rec.top = 5;
		rec.left = 5;
		rec.right = 5;
		rec.bottom = 5;
		
		pFont->DrawTextW( NULL,
							L"������� ����������� ��� ������� ������",
							38, &rec, DT_NOCLIP,  
							D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
		ChooseDirection( carMain, scStage );
		//scStage = 6;
	}
	
	if( scStage == 6 )
	{
		RenderScene();
		if( config.leftCar )
		{
			D3DXMatrixLookAtLH(&scMatrixView,
					&D3DXVECTOR3( -20.0f, 15.0f, -20.f ),				// ���������� �����, �� ������� ���������� �������� �����
					&D3DXVECTOR3(-10.0f, 0.0f, 0.0f),					// ���������� �� �� ��� �� �������
					&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			pDirect3DDevice->SetTransform( D3DTS_VIEW, &scMatrixView );

			rec.top = 5;
			rec.left = 5;
			rec.right = 5;
			rec.bottom = 5;
		
			pFont->DrawTextW( NULL,
							L"������� ����������� ��� ������ �����",
							36, &rec, DT_NOCLIP,  
							D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
			ChooseDirection( carL, scStage );
		}
		else
			++ scStage;
	}
	if( scStage == 7 )
	{
		RenderScene();

		if( config.rightCar )
		{
			D3DXMatrixLookAtLH( &scMatrixView,
								&D3DXVECTOR3( 0.0f, 15.0f, -20.f ),				// ���������� �����, �� ������� ���������� �������� �����
								&D3DXVECTOR3( 10.0f, -5.0f, 0.0f),					// ���������� �� �� ��� �� �������
								&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			pDirect3DDevice->SetTransform( D3DTS_VIEW, &scMatrixView );
			rec.top = 5;
			rec.left = 5;
			rec.right = 5;
			rec.bottom = 5;
		
			pFont->DrawTextW( NULL,
							L"������� ����������� ��� ������ ������",
							37, &rec, DT_NOCLIP,  
							D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
			ChooseDirection( carR, scStage );
		}
		else
			++ scStage;
	}
	if( scStage == 8 )
	{
		RenderScene();

		if( config.topCar )
		{
			D3DXMatrixLookAtLH( &scMatrixView,
								&D3DXVECTOR3( -12.0f, 20.0f, 0.f ),				// ���������� �����, �� ������� ���������� �������� �����
								&D3DXVECTOR3( 0.0f, 0.0f, 15.0f),					// ���������� �� �� ��� �� �������
								&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			pDirect3DDevice->SetTransform( D3DTS_VIEW, &scMatrixView );

			rec.top = 5;
			rec.left = 5;
			rec.right = 5;
			rec.bottom = 5;
		
			pFont->DrawTextW( NULL,
							L"������� ����������� ��� ������ ������",
							37, &rec, DT_NOCLIP,  
							D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
			ChooseDirection( carT, scStage );
		}
		else
			++ scStage;
	}
	if( scStage == 9 ) // ����������� ����� ������� ���������
	{
		RenderScene();

		if( config.trafficLight )
		{
			D3DXMatrixLookAtLH( &scMatrixView,
								&D3DXVECTOR3( 4.0f, 7.0f, -10.0f ),				// ���������� �����, �� ������� ���������� �������� �����
								&D3DXVECTOR3( 4.0f, 0.0f, 0.0f),					// ���������� �� �� ��� �� �������
								&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			pDirect3DDevice->SetTransform( D3DTS_VIEW, &scMatrixView );
			
			Light.LightTrafficEnable();
			
			ChooseColor( trafficDown, trafficUp, trafficLeft, trafficRight, Light );
			if( s_action )
				scStage = 10;
		}
		else
			scStage = 10;
	}
	pDirect3DDevice->EndScene();
	pDirect3DDevice->Present( NULL, NULL, NULL, NULL );
	return S_OK;
}


HRESULT ChooseDirection( Mesh &Obj, int &cdStage )
{	
	GetDInputCourse();
		if( s_left )
		{
			Obj.MoveTo = LEFT;
			s_left = false;
			++ cdStage;
		}
		else if( s_right )
		{
			Obj.MoveTo = RIGHT;
			s_right = false;
			++ cdStage;
		}
		else if( s_up )
		{
			Obj.MoveTo = UP;
			s_up = false;
			++ cdStage;
		}
	return S_OK;
}

HRESULT ChooseColor( Mesh &trafficD,  Mesh &trafficU, Mesh &trafficL, Mesh &trafficR, CLight &ccLight )
{	
	GetDInputCourse2();
		ccLight.LightTrafficChange( s_colorCh );
	return S_OK;
}
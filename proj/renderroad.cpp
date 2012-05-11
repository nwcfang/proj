#include "stdafx.h"
#include "glob.h"
#include "text.h"
#include "manipul.h"
#include "light.h"
#include "configur.h"
#include "InitInput.h"
#include "placer.h"
#include "tools.h"

#define PATHCAR1 L"car1_1.x"
#define PATHCAR2 L"car2_1.x"
int PlaceMainCar();
INT PlaceLeftCar();
INT PlaceRightCar();
INT PlaceTopCar();
//HRESULT PlaceTraffic( const bool mode );
HRESULT PlaceTraffic();

VOID RenderRoad( int &stage )

{
	
	D3DXMATRIX rMatrix;
	D3DXMATRIX sMatrix;
	D3DXMATRIX rrMatrixView;
	D3DXMATRIX mMatrix;
	Manipulation rr_Mnp;
	
	pDirect3DDevice->Clear( 0,										// Номер массива прямоугольников, подвергающихся очистке. 0 - используется вся поверхность
							NULL,									// адрес массива прямоугольников
							D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,		// параметр указывающий какие из поверхностей будут очищены. D3DCLEAR_TARGET - очистить буфер поверхности до цвета
							D3DCOLOR_XRGB( 191, 245, 251 ),			// макрос цвета.
							1.0f,									// значение для Z-буфера
							0);										// значение буфера трафарета

	pDirect3DDevice->BeginScene();		//cигнализируем о том, что начался рендериг объектов сцены
	GetDInput();
	if( stage == 3 )
	{
		road.roatX = 0.0f;
		road.size = 1.0f;
		
		Light.LightGlob( 0.0f, -100.0f, 0.0f );

		// В проект
		D3DXMatrixLookAtLH(&rrMatrixView,
					&D3DXVECTOR3( -20.0f, 25.0f, -20.f ),			// определяет точку, из которой происходит просмотр сцены
					&D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// определяет то на что мы смотрим
					&D3DXVECTOR3(0.0f, 1.0f, 0.0f));				// определяет то что мы видим

		// Дебаг
		//D3DXMatrixLookAtLH(&rrMatrixView,
		//			&D3DXVECTOR3( 0.0f, 50.0f, -0.001f ),			
		//			&D3DXVECTOR3(0.0f, 0.0f, 0.0f),					
		//			&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

		// Дебаг 2
		/*D3DXMatrixLookAtLH(&rrMatrixView,
					&D3DXVECTOR3( -20.0f, 0.0f, -10.0f ),			
					&D3DXVECTOR3(-20.0f, 0.0f, 0.0f),					
					&D3DXVECTOR3(0.0f, 1.0f, 0.0f));*/

		pDirect3DDevice -> SetTransform(D3DTS_VIEW, &rrMatrixView);	
		
		stage = 4;
		car1.roatX = 0.0f;

		if( g_car )
		{
			carR.InitMesh( PATHCAR1 );
			carL.InitMesh( PATHCAR1 );
			carT.InitMesh( PATHCAR1 );
			carMain.InitMesh( PATHCAR2 );
		}
		else
		{
			carR.InitMesh( PATHCAR2 );
			carL.InitMesh( PATHCAR2 );
			carT.InitMesh( PATHCAR2 );
			carMain.InitMesh( PATHCAR1 );
		}
		
	}

	if( stage == 4 )
	{

		SetMatrixAndDraw( road ); 
		pFont->DrawTextW( NULL, L"Этап конфигурации ситуации на дороге", 36, &rec, DT_NOCLIP,  D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
		
		PlaceMainCar();

		if( config.leftCar )
			PlaceLeftCar();
		if( config.rightCar )
			PlaceRightCar();
		if( config.topCar )
			PlaceTopCar();
		if( config.trafficLight )
			PlaceTraffic();

		/*if( config.trafficLight == GREEN )
			PlaceTraffic( GREEN );
		if( config.trafficLight == RED )
			PlaceTraffic( RED );*/
	}


	pDirect3DDevice->EndScene();

	// копируем задний буфер в передний
	pDirect3DDevice ->Present(NULL,	NULL, NULL,	NULL);
	if( s_scourse )
		stage = 5;
}

INT PlaceLeftCar()
{
	D3DXMATRIX sMatrix;
	D3DXMATRIX rMatrix;
	D3DXMATRIX mMatrix;

	/*if( g_car )
		memcpy( &carL , &car1, sizeof( Mesh ) );
	else
		memcpy( &carL , &car2, sizeof( Mesh ) );*/
	

	carL.ClearUp();
	carL.size = 0.015f;
	sMatrix = carL.Mnp.Scale( carL.size );
	carL.roatY = 90.0f;
	rMatrix = carL.Mnp.Rotation(carL.roatX, carL.roatY, carL.roatZ);
	carL.movX = -20.0f;
	carL.movY = 0.8f;
	carL.movZ = -1.125f;
	mMatrix = carL.Mnp.Moving( carL.movX, carL.movY, carL.movZ );
	sMatrix = sMatrix * rMatrix * mMatrix;
	pDirect3DDevice->SetTransform( D3DTS_WORLD, &sMatrix );
	carL.DrawMesh();

	return 0;
}

INT PlaceRightCar()
{
	D3DXMATRIX sMatrix;
	D3DXMATRIX rMatrix;
	D3DXMATRIX mMatrix;

	//if( g_car )
	//	carR.InitMesh( PATHCAR1 );
	//else
	//	//memcpy( &carR , &car2, sizeof( Mesh ) );
	//	carR.InitMesh(PATHCAR2 );
	carR.ClearUp();
	carR.size = 0.015f;
	sMatrix = carR.Mnp.Scale( carR.size );
	carR.roatY = -90.0f;
	rMatrix = carR.Mnp.Rotation(carR.roatX, carR.roatY, carR.roatZ);
	carR.movX = 20.0f;
	carR.movY = 0.8f;
	carR.movZ = 1.125f;
	mMatrix = carR.Mnp.Moving( carR.movX, carR.movY, carR.movZ );
	sMatrix = sMatrix * rMatrix * mMatrix;
	pDirect3DDevice->SetTransform( D3DTS_WORLD, &sMatrix );
	carR.DrawMesh();

	return 0;
}

INT PlaceTopCar()
{
	D3DXMATRIX sMatrix;
	D3DXMATRIX rMatrix;
	D3DXMATRIX mMatrix;

	/*if( g_car )
		memcpy( &carT , &car1, sizeof( Mesh ) );
	else
		memcpy( &carT , &car2, sizeof( Mesh ) );*/
	/*if( g_car )
		carT.InitMesh( PATHCAR1 );
	else
		carT.InitMesh( PATHCAR2 );*/

	carT.ClearUp();
	carT.size = 0.015f;
	sMatrix = carT.Mnp.Scale( carT.size );
	carT.roatY = 180.0f;
	rMatrix = carT.Mnp.Rotation(carT.roatX, carT.roatY, carT.roatZ);
	carT.movX = -1.125f;
	carT.movY = 0.8f;
	carT.movZ = 20.0f;
	mMatrix = carT.Mnp.Moving( carT.movX, carT.movY, carT.movZ );
	sMatrix = sMatrix * rMatrix * mMatrix;
	pDirect3DDevice->SetTransform( D3DTS_WORLD, &sMatrix );
	carT.DrawMesh();

	return 0;
}

HRESULT PlaceTraffic()
{
	D3DXMATRIX sMatrix;
	D3DXMATRIX rMatrix;
	D3DXMATRIX mMatrix;

	trafficDown.ClearUp();
	trafficDown.movX = 5.0f;
	trafficDown.movY = 0.0f;
	trafficDown.movZ = -5.0f;

	SetMatrixAndDraw( trafficDown );

	trafficUp.ClearUp();
	trafficUp.movX = -5.0f;
	trafficUp.movY = 0.0f;
	trafficUp.movZ = 5.0f;
	trafficUp.roatY = 180.0f;

	SetMatrixAndDraw( trafficUp );
	
	trafficLeft.ClearUp();
	trafficLeft.movX = -5.0f;
	trafficLeft.movY = 0.0f;
	trafficLeft.movZ = -5.0f;
	trafficLeft.roatY = 90.0f;

	SetMatrixAndDraw( trafficLeft );

	trafficRight.ClearUp();
	trafficRight.movX = 5.0f;
	trafficRight.movY = 0.0f;
	trafficRight.movZ = 5.0f;
	trafficRight.roatY = -90.0f;

	SetMatrixAndDraw( trafficRight );

	return S_OK;
}

int	PlaceMainCar()
	{
		D3DXMATRIX sMatrix;
		D3DXMATRIX rMatrix;
		D3DXMATRIX mMatrix;

		/*if( !g_car )
			memcpy( &carMain , &car1, sizeof( Mesh ) );
		else
			memcpy( &carMain , &car2, sizeof( Mesh ) );*/

		carMain.ClearUp();
		carMain.size = 0.015f;
		carMain.roatY = 0.0f;
		carMain.movX = 1.125f;
		carMain.movY = 0.8f;
		carMain.movZ = -20.0f;

		SetMatrixAndDraw( carMain );
		return 0;
	}
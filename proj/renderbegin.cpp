#include "stdafx.h"
#include "glob.h"
#include "text.h"
#include "initinput.h"

VOID RenderBegin( int &stage)
{
	D3DXMATRIX rMatrix;
	D3DXMATRIX mMatrix;
	D3DXMATRIX sMatrix;
	Manipulation Mnp;

	if(pDirect3DDevice == NULL)
		return;
	
	//очищаем задний буфер
	pDirect3DDevice->Clear( 0,										// Номер массива прямоугольников, подвергающихся очистке. 0 - используется вся поверхность
							NULL,									// адрес массива прямоугольников
							D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,		// параметр указывающий какие из поверхностей будут очищены. D3DCLEAR_TARGET - очистить буфер поверхности до цвета
							D3DCOLOR_XRGB( 255, 255, 255 ),			// макрос цвета.
							1.0f,									// значение для Z-буфера
							0);										// значение буфера трафарета

	pDirect3DDevice->BeginScene();		//cигнализируем о том, что начался рендериг объектов сцены
	
	GetDInputInStart();

	if( stage == 1 )
	{
		TextInit();
		car1.roatY = 90.0f;
		car1.movX = -450.0f;
		car1.movY = 220.0f;
	
		car2.roatY = 90.0f;
		car2.movX = -450.0f;
		car2.movY = 100.0f;
	}
	stage = 0;

	if( stage == 0 )
	{
		if( g_car == 0 )
		{
			rMatrix = Mnp.RotationPlus( 0.0f,  0.09f, 0.0f, car1.roatX, car1.roatY, car1.roatZ );
			mMatrix = Mnp.Moving ( car1.movX, car1.movY,car1.movZ );
			rMatrix *= mMatrix;
			pDirect3DDevice->SetTransform( D3DTS_WORLD, &rMatrix);
			car1.DrawMesh();

			car2.roatY = 90.0f;
			rMatrix = Mnp.Rotation( car2.roatX, car2.roatY,car2.roatZ );
			mMatrix = Mnp.Moving ( car2.movX, car2.movY,car2.movZ );
			rMatrix *= mMatrix;
			pDirect3DDevice->SetTransform( D3DTS_WORLD, &rMatrix);
			car2.DrawMesh();
		}

		if( g_car == 1 )
		{
			rMatrix = Mnp.RotationPlus( 0.0f,  0.09f, 0.0f, car2.roatX, car2.roatY, car2.roatZ );
			mMatrix = Mnp.Moving ( car2.movX, car2.movY,car2.movZ );
			rMatrix *= mMatrix;
			pDirect3DDevice->SetTransform( D3DTS_WORLD, &rMatrix);
			car2.DrawMesh();

			car1.roatY = 90.0f;
			rMatrix = Mnp.Rotation( car1.roatX, car1.roatY,car1.roatZ );
			mMatrix = Mnp.Moving ( car1.movX, car1.movY,car1.movZ );
			rMatrix *= mMatrix;
			pDirect3DDevice->SetTransform( D3DTS_WORLD, &rMatrix);
			car1.DrawMesh();
		}
	}

	DrawMyText( 10,10,300,10 );

	pDirect3DDevice->EndScene();

	// копируем задний буфер в передний
	pDirect3DDevice ->Present(NULL,	NULL, NULL,	NULL);	
	if( s_road )
	{
		stage = 3;
	}
}	

//if( stage == 1 )
	//{
	//	rMatrix = Mnp.RotationPlus( 0.0f,  90.0f, 0.0f, car1.roatX, car1.roatY, car1.roatZ );
	//	mMatrix = Mnp.MovingPlus( -450.0f,  100.0f, 0.0f, car1.movX , car1.movY, car1.movZ );
	//	sMatrix = Mnp.ScalePlus( 1.0f, car1.size );
	//}

	//else
	//{
	//	rMatrix = Mnp.Rotation( car1.roatX, car1.roatY,car1.roatZ );
	//	mMatrix = Mnp.Moving ( car1.movX, car1.movY,car1.movZ );
	//	sMatrix = Mnp.Scale( car1.size );
	//}

	//rMatrix = rMatrix * mMatrix * sMatrix;
	//pDirect3DDevice->SetTransform( D3DTS_WORLD, &rMatrix);
	//car1.DrawMesh();

	//if( stage == 1 )
	//{
	//	rMatrix = Mnp.RotationPlus( 0.0f,  90.0f, 0.0f, car2.roatX, car2.roatY, car2.roatZ );
	//	mMatrix = Mnp.MovingPlus( -510.0f,  100.0f, 0.0f, car2.movX , car2.movY, car2.movZ );
	//	sMatrix = Mnp.ScalePlus( 1.0f, car2.size );
	//	
	//}

	//else
	//{
	//	rMatrix = Mnp.Rotation( car2.roatX, car2.roatY,car2.roatZ );
	//	mMatrix = Mnp.Moving ( car2.movX, car2.movY,car2.movZ );
	//	sMatrix = Mnp.Scale( car2.size );
	//}

	//rMatrix = rMatrix * mMatrix * sMatrix;
	//pDirect3DDevice->SetTransform( D3DTS_WORLD, &rMatrix);
	//car2.DrawMesh();			
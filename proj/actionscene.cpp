#include "stdafx.h"
#include "glob.h"
#include "tools.h"
#include "camera.h"
#include "initinput.h"
#include "xmlreader.h"
#define SPEED 0.005f

HRESULT GoStraight( Mesh &Obj );
HRESULT GoLeft( Mesh &Obj, const float gsSpeed );
VOID Calculation();
bool Start();

HRESULT ActionScene( int &asStage )
{
	//CParser CPrsr;
	HRESULT hr;

	D3DXMATRIX acMatrixView;
	pDirect3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,		
							D3DCOLOR_XRGB( 191, 245, 251 ),	1.0f, 0);

	pDirect3DDevice->BeginScene();

	if( asStage == 10 )
	{
		GetDInputAction();
		Calculation();

		++ asStage;
	}

	if( asStage == 11 )
	{

		D3DXMatrixLookAtLH(&acMatrixView,
					&D3DXVECTOR3( 0.0f, 40.0f, 0.f ),				// определ€ет точку, из которой происходит просмотр сцены
					&D3DXVECTOR3(0.0f, 0.0f, 0.0001f),					// определ€ет то на что мы смотрим
					&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		pDirect3DDevice->SetTransform( D3DTS_VIEW, &acMatrixView );

		if(	Start() )
		{
		 	GetDInputAction();
			if( s_start )
			{
				++ asStage;
			}
		}

		RenderScene();
	}

	//ƒвижение согласно XML
	if( asStage == 12 )
	{
		GetDInputAction();

		if( first )
		{
			CPrsr.InitCarList();
			Mesh *a;
			Mesh *b;
			Mesh *c;
			Mesh *d;
			for( int i = 0; i < 4; ++ i)
			{
				a = *(CPrsr.iteratorList);
				CPrsr.Iteration();
				b = *(CPrsr.iteratorList);
				CPrsr.Iteration();
				c = *(CPrsr.iteratorList);
				CPrsr.Iteration();
				d = *(CPrsr.iteratorList);

				

				hr = CPrsr.YourTurn( a, b, c, d );
 				if( hr == S_OK )
					a->ICanGo = true;
				else if( hr == S_FALSE )
					MessageBox( NULL, L"Ќет машин, проезжающих перекресток", L"End", MB_OK );
				CPrsr.Iteration();
				CPrsr.Iteration();

			first = false;
			}
		}
		CPrsr.MoveCars();
		RenderScene2();
	}
	pDirect3DDevice->EndScene();
	pDirect3DDevice->Present( NULL, NULL, NULL, NULL );
	return S_OK;
}

VOID Calculation()
{
	carMain.PathCalc( SPEED );
	if( config.leftCar )
	{
		carL.PathCalc( SPEED );
		carL.mChange = true;
	}

	if( config.rightCar )
	{
		carR.PathCalc( SPEED );
		carR.mChange = true;
	}
	if( config.topCar )
	{
		carT.PathCalc( SPEED );
		carT.mChange = true;
	}
}

bool Start()
{
	bool a, b, c, d;
	a = b = c = d = false;
	d = carMain.OnReady();
	if( config.leftCar )
		a = carL.OnReady();
	if( config.rightCar )
		b = carR.OnReady();
	if( config.topCar )
		c = carT.OnReady();
	if( !(a || b || c || d) )
	{
		return true;	
	}
	return false;
}
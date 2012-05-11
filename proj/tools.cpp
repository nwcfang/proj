#include "stdafx.h"
#include "glob.h"

void SetMatrixAndDraw( Mesh &Obj )
{
	if( Obj.mChange == true )
	{
		D3DXMATRIX smadMatrixRes;
		D3DXMATRIX smadMatrix;

		Obj.mMatrixWorld = Obj.Mnp.Rotation( Obj.roatX, Obj.roatY, Obj.roatZ );

		smadMatrix = Obj.Mnp.Scale( Obj.size );
		D3DXMatrixMultiply( &Obj.mMatrixWorld, &Obj.mMatrixWorld, &smadMatrix );

		smadMatrix = Obj.Mnp.Moving( Obj.movX, Obj.movY, Obj.movZ );
		D3DXMatrixMultiply( &Obj.mMatrixWorld, &Obj.mMatrixWorld, &smadMatrix );
		
	}

	pDirect3DDevice->SetTransform( D3DTS_WORLD, &Obj.mMatrixWorld );

	Obj.DrawMesh();
	Obj.mChange = false;
}

HRESULT RenderScene()
{
	SetMatrixAndDraw( road );
	SetMatrixAndDraw( carMain );
	if( config.leftCar )
		SetMatrixAndDraw( carL );
	if( config.rightCar )
		SetMatrixAndDraw( carR );
	if( config.topCar )
		SetMatrixAndDraw( carT );
	if( config.trafficLight )
	{
		SetMatrixAndDraw( trafficDown );
		SetMatrixAndDraw( trafficUp );
		SetMatrixAndDraw( trafficLeft );
		SetMatrixAndDraw( trafficRight );
	}
	return S_OK;
}
 
HRESULT RenderScene2()
{
	SetMatrixAndDraw( road );
	if( carMain.mTime3 >= 0 )
		SetMatrixAndDraw( carMain );
	if( config.leftCar && carL.mTime3 >= 0 )
		SetMatrixAndDraw( carL );
	if( config.rightCar && carR.mTime3 >= 0 )
		SetMatrixAndDraw( carR );
	if( config.topCar && carT.mTime3 >= 0 )
		SetMatrixAndDraw( carT );
	if( config.trafficLight )
	{
		SetMatrixAndDraw( trafficDown );
		SetMatrixAndDraw( trafficUp );
		SetMatrixAndDraw( trafficLeft );
		SetMatrixAndDraw( trafficRight );
	}
	return S_OK;
}
#include "stdafx.h"
#include "glob.h"

VOID Roration( FLOAT x, FLOAT y, FLOAT z )
{
	D3DXMATRIX MatriX;
	D3DXMATRIX MatriY;
	D3DXMATRIX MatriZ;

	if( x )
	{
		FLOAT Angle = x * (2.0f * D3DX_PI) / 100;
		D3DXMatrixRotationX( &MatriX, Angle );
	}

	Angle = y * (2.0f * D3DX_PI) / 100;
	D3DXMatrixRotationX( &MatriY, Angle );

	Angle = z * (2.0f * D3DX_PI) / 100;
	D3DXMatrixRotationX( &MatriZ, Angle );

	MatriX = MatriX * MatriY * MatriZ;

	pDirect3DDevice->SetTransform( D3DTS_WORLD, &MatriX );
}

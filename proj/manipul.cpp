#include "stdafx.h"
#include "glob.h"

Manipulation::Manipulation()
{

}


D3DXMATRIX Manipulation::Rotation( FLOAT rx, FLOAT ry, FLOAT rz )
{
	D3DXMATRIX MatriX;
	D3DXMATRIX MatriY;
	D3DXMATRIX MatriZ;
	
	angleX = rx * (2.0f * D3DX_PI) / 360;
	D3DXMatrixRotationX( &MatriX, angleX );

	angleY = ry * (2.0f * D3DX_PI) / 360;
	D3DXMatrixRotationY( &MatriY, angleY );

	angleZ = rz * (2.0f * D3DX_PI) / 360;
	D3DXMatrixRotationZ( &MatriZ, angleZ );

	rMatrix = MatriX * MatriY * MatriZ;
	return rMatrix;
}

D3DXMATRIX Manipulation::RotationPlus( FLOAT rx, FLOAT ry, FLOAT rz, FLOAT &mesh_rx, FLOAT &mesh_ry, FLOAT &mesh_rz )
{
	D3DXMATRIX MatriX;
	D3DXMATRIX MatriY;
	D3DXMATRIX MatriZ;

	mesh_rx += rx;
	angleX = mesh_rx * (2.0f * D3DX_PI) / 360;
	D3DXMatrixRotationX( &MatriX, angleX );
	
	mesh_ry += ry;
	angleY = mesh_ry * (2.0f * D3DX_PI) / 360;
	D3DXMatrixRotationY( &MatriY, angleY );
	
	mesh_rz += rz;
	angleZ = mesh_rz * (2.0f * D3DX_PI) / 360;
	D3DXMatrixRotationZ( &MatriZ, angleZ );
	

	rMatrix = MatriX * MatriY * MatriZ;
	return rMatrix;
}

D3DXMATRIX Manipulation::Moving( FLOAT mx, FLOAT my, FLOAT mz )
{
	D3DXMatrixTranslation( &mMatrix, mx, my, mz );
	return mMatrix;
}

D3DXMATRIX Manipulation::MovingPlus( FLOAT mx, FLOAT my, FLOAT mz, FLOAT &mesh_mx,
							FLOAT &mesh_my, FLOAT &mesh_mz )
{
	mesh_mx += mx;
	mesh_my += my;
	mesh_mz += mz;

	D3DXMatrixTranslation( &mMatrix, mesh_mx, mesh_my, mesh_mz );
	return mMatrix;
}

D3DXMATRIX Manipulation::Scale( FLOAT sc )
{
	D3DXMatrixScaling( &sMatrix, sc, sc, sc  );
	return sMatrix;
}

D3DXMATRIX Manipulation::ScalePlus( FLOAT sc, FLOAT &mesh_s )
{
	mesh_s += sc;
	D3DXMatrixScaling( &sMatrix, mesh_s, mesh_s, mesh_s  );
	return sMatrix;
}

//VOID Manipulation::Clean()
//{
//	this->movingX = 0;
//	this->movingY = 0;
//	this->movingZ = 0;
//	this->angleX = 0;
//	this->angleY = 0;
//	this->angleZ = 0;
//	this->scale = 0;
//	this->scaleY = 0;
//	this->scaleZ = 0;
//}

//VOID Manipulation::SetItAll()
//{
//	rMatrix = sMatrix * rMatrix * mMatrix;
//	pDirect3DDevice->SetTransform( D3DTS_WORLD, &rMatrix );
//}
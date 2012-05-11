#pragma once
#include "stdafx.h"

//  ласс дл€ манипул€ции объектами (поворот вокруг осей, изменение размера,
// движение вдоль осей)

class Manipulation
{
public:
	Manipulation();
	
	D3DXMATRIX Rotation( FLOAT rx, FLOAT ry, FLOAT rz );
	D3DXMATRIX RotationPlus( FLOAT rx, FLOAT ry, FLOAT rz, FLOAT &mesh_rx, 
							FLOAT &mesh_ry, FLOAT &mesh_rz );
	D3DXMATRIX Moving( FLOAT mx, FLOAT my, FLOAT mz );
	D3DXMATRIX MovingPlus( FLOAT mx, FLOAT my, FLOAT mz, FLOAT &mesh_mx,
							FLOAT &mesh_my, FLOAT &mesh_mz );
	D3DXMATRIX Scale( FLOAT sc );
	D3DXMATRIX ScalePlus( FLOAT sc, FLOAT &mesh_s );

	//VOID SetItAll();
	//VOID Clean();

private:
	FLOAT angleX;
	FLOAT angleY;
	FLOAT angleZ;

	//FLOAT scale;

	D3DXMATRIX rMatrix;
	D3DXMATRIX mMatrix;
	D3DXMATRIX sMatrix;
};
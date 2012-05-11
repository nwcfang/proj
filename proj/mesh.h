#pragma once
#include "stdafx.h"
#include "manipul.h"

class Mesh
{
public:
	LPD3DXMESH	pMesh;
	D3DMATERIAL9* pMeshMaterials;
	LPDIRECT3DTEXTURE9* pMeshTexture;
	DWORD dwNumber;
	
	FLOAT roatX;
	FLOAT roatY;
	FLOAT roatZ;

	FLOAT movX;
	FLOAT movY;
	FLOAT movZ;

	FLOAT size;

	CHAR MoveTo;
	bool color;

	unsigned int mCounter;

	

	int mTime0;
	int mTime1;
	int mTime2;
	int mTime3;

	float mDistCommon;
	float mDist0;
	float mDist1;
	float mDist2;
	float mDist3;

	float mSpeed;

	float mDelta;

	bool mChange;
	D3DXMATRIX mMatrixWorld;

	Mesh();
	~Mesh();
	Manipulation Mnp;
	HRESULT InitMesh( LPCWSTR filename );
	HRESULT GoStraight();
	HRESULT GoLeft();
	HRESULT GoRight();
	HRESULT GoUp();
	HRESULT JustDoIt(); // отрабатывает маршрут
	
	bool OnReady();
	bool ICanGo;
	VOID PathCalc( const float pcSpeed );  
	VOID DrawMesh();
	VOID ClearUp();
};

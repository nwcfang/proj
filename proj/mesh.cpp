#include "stdafx.h"
#include "glob.h"

Mesh::Mesh()
{
	pMesh = NULL;
	pMeshMaterials = NULL;
	pMeshTexture = NULL;
	dwNumber = 0;

	D3DXMatrixIdentity( &mMatrixWorld );
	
	roatX = 0;
	roatY = 0;
	roatZ = 0;

	movX = 0;
	movY = 0;
	movZ = 0;

	size = 0;

	mCounter = 0;

	mTime0 = 0;
	mTime1 = 0;
	mTime2 = 0;
	mTime3 = 0;

	mDist0 = 7.0f;
	mDist1 = 8.0f;
	mDist2 = 9.0f;
	mDist3 = 19.0f;

	mSpeed = 0.0f;
	 
	mDelta = 0.0f;

	mChange = true;
	MoveTo = 0;
	color = false;
	ICanGo = false; // если true - машина отрабатывает маршрут
}

Mesh::~Mesh()
{
	pMesh = NULL;
	pMeshMaterials = NULL;
	pMeshTexture = NULL;
	
}

HRESULT Mesh::InitMesh( LPCWSTR filename)
{
	D3DXMATERIAL* D3DXMeshMaterials;
	LPD3DXBUFFER pMeshBuffer = NULL;		// содержит информацию о материале и текстуре

	// загружаем первую машинку
	if( FAILED( D3DXLoadMeshFromX( 
				filename,				// x-файл
				D3DXMESH_SYSTEMMEM,		// выделение памяти для ресурсов происходит из RAM
				pDirect3DDevice,		// указатель на Direct3D девайс
				NULL,					// адрес указателя на буффер, содержащий данные о смежности
				&pMeshBuffer,			// адрес указателя на буффер, содержащий данные о материале и текстуре
				NULL,					// адрес указателя на интерфейс ID3DXBuffer, содержащий специфические эффекты
				&(this->dwNumber),				// указатель на массив данных для структуры D3DXMATERIAL
				&(this->pMesh) ) ) )			// рузультат всей операции
	{
		MessageBox( NULL, L"Could not find car1.x", L"ERROR", MB_OK );
		return E_FAIL;
	}
		
	D3DXMeshMaterials = ( D3DXMATERIAL* ) pMeshBuffer->GetBufferPointer();
	this->pMeshMaterials = new D3DMATERIAL9 [this->dwNumber];
	this->pMeshTexture = new LPDIRECT3DTEXTURE9 [this->dwNumber];

	for( DWORD i = 0; i < this->dwNumber; ++ i )
	{
		this->pMeshMaterials[i] = D3DXMeshMaterials[i].MatD3D;
		this->pMeshMaterials[i].Ambient = this->pMeshMaterials[i].Diffuse;

		if( FAILED( D3DXCreateTextureFromFileA( pDirect3DDevice, D3DXMeshMaterials[i].pTextureFilename, &(this->pMeshTexture[i])) ) )
			this->pMeshTexture[i] = NULL;
	}

	// уничтожаем материальный буффер
	pMeshBuffer->Release();

	return S_OK;
}


VOID Mesh::ClearUp()
{
	roatX = 0.0f;
	roatY = 0.0f;
	roatZ = 0.0f;

	movX = 0.0f;
	movY = 0.0f;
	movZ = 0.0f;

	size = 1.0f;

	mChange = true;
}

VOID Mesh::DrawMesh()
{
	for( DWORD i = 0; i < this->dwNumber; ++ i )
	{
		pDirect3DDevice->SetMaterial( &(this->pMeshMaterials[i]) );
		pDirect3DDevice->SetTexture( 0, this->pMeshTexture[i] );
		this->pMesh->DrawSubset(i);
	}
}

VOID Mesh::PathCalc( const float pcSpeed )
{
	if( MoveTo == LEFT )
	{
		mDist1 = 8.0f;
		mDist2 = 9.0f;
		mDist3 = 24.0f;
	}
	else if( MoveTo == RIGHT )
	{
		mDist1 = 6.0f;
		mDist2 = 9.0f;
		mDist3 = 24.0f;		
	}
	else if( MoveTo == UP )
	{
		mDist1 = 6.0f;
		mDist2 = 9.0f;
		mDist3 = 24.0f;
	}
	mSpeed  = pcSpeed;
	mTime0 = mDist0 / mSpeed;
	mTime1 = mDist1 / mSpeed;
	mTime2 = mDist2 / mSpeed;
	mTime3 = mDist3 / mSpeed;

	mDelta = 90.0f /  mTime2;
}

HRESULT Mesh::GoStraight()
{
	D3DXMATRIX gsMat;
	D3DXVECTOR3 gsVec = D3DXVECTOR3 ( 0.0f, 0.0f, 1.0f );

	D3DXMatrixIdentity( &gsMat );

	gsVec *= mSpeed;

	float Angle = roatY * ( 2.0f * D3DX_PI ) / 360;
	D3DXMatrixRotationY( &gsMat, Angle );
	D3DXVec3TransformCoord( &gsVec, &gsVec, &gsMat);

	movX += gsVec.x;
	movY += gsVec.y;
	movZ += gsVec.z;

	return S_OK;
}

HRESULT Mesh::GoLeft()
{
	if( mTime1 >= 0 )
	{
		mChange = true;
		GoStraight();
		-- mTime1; 
	}
	else if( mTime2 >= 0 )
	{
		mChange = true;
		roatY -= mDelta;
		GoStraight();
		-- mTime2;
	}
	else if( mTime3 >= 0 )
	{
		mChange = true;
		GoStraight();
		-- mTime3;
	}
	//else
		//return E_FAIL;
	if( (mTime1 < 0) && (mTime2 < 0) && (mTime3 < 0) )
		return S_OK;

	return S_FALSE;
}

HRESULT Mesh::GoRight()
{
	if( mTime1 >= 0 )
	{
		mChange = true;
		GoStraight();
		-- mTime1; 
	}
	else if( mTime2 >= 0 )
	{
		mChange = true;
		roatY += mDelta;
		GoStraight();
		-- mTime2;
	}
	else if( mTime3 >= 0 )
	{
		mChange = true;
		GoStraight();
		-- mTime3;
	}
	//else
		//return false;
	if( (mTime1 < 0) && (mTime2 < 0) && (mTime3 < 0) )
		return S_OK;

	return S_FALSE;
}

HRESULT Mesh::GoUp()
{
	if( mTime1 >= 0 )
	{
		mChange = true;
		GoStraight();
		-- mTime1; 
	}
	else if( mTime2 >= 0 )
	{
		mChange = true;
	//	roatY += mDelta;
		GoStraight();
		-- mTime2;
	}
	else if( mTime3 >= 0 )
	{
		mChange = true;
		GoStraight();
		-- mTime3;
	}

	/*else
		return false;*/
	if( (mTime1 < 0) && (mTime2 < 0) && (mTime3 < 0) )
		return S_OK;

	return S_FALSE;
}

bool Mesh::OnReady()
{
	if( mTime0 > 0 )
	{
		mChange = true;
		GoStraight();
		-- mTime0;
	}
	else
		return false;

	return true;
}

HRESULT Mesh::JustDoIt()
{
	HRESULT hr = S_OK;
	if( MoveTo == LEFT )
	{
		hr = GoLeft();
		if( hr == S_OK )
		{
			MoveTo = ANYWHERE;
			ICanGo = false;
			return S_OK;
		}
	}
	else if( MoveTo == RIGHT )
	{
		hr = GoRight();
		if( hr == S_OK )
		{
			MoveTo = ANYWHERE;
			ICanGo = false;
			return S_OK;
		}
	}
	else if( MoveTo == UP )
	{
		hr = GoUp();
		if( hr == S_OK )
		{	
			MoveTo = ANYWHERE;
			ICanGo = false;
			return S_OK;
		}
	}
	return S_FALSE;
}
#include "stdafx.h"
#include "glob.h"


VOID DeleteMyMesh( Mesh &Obj );

// повторный вызов DeleteMyMesh() вызывает непонятную ошибку
VOID DeleteDirect3D()
{
	if( pFont != NULL )
		pFont->Release();

	if(car1.pMesh != NULL)
	DeleteMyMesh( car1 );

	if(car2.pMesh != NULL)
	DeleteMyMesh( car2 );

	if(carMain.pMesh != NULL)
		DeleteMyMesh( carMain );

	if(carL.pMesh != NULL)
	DeleteMyMesh( carL );

	if(carR.pMesh != NULL)
	DeleteMyMesh( carR );

	if(carT.pMesh != NULL)
	DeleteMyMesh( carT );

	if(road.pMesh != NULL)
	DeleteMyMesh( road );

	if(trafficDown.pMesh != NULL)
	DeleteMyMesh( trafficDown );

	if(trafficUp.pMesh != NULL)
	DeleteMyMesh( trafficUp );

	if(trafficLeft.pMesh != NULL)
	DeleteMyMesh( trafficLeft );

	if(trafficRight.pMesh != NULL)
	DeleteMyMesh( trafficRight );

	if(pDirect3DDevice != NULL)
		pDirect3DDevice -> Release();
	if(pDirect3D != NULL)
		pDirect3D -> Release();

	if (pDInput != NULL)
		pDInput->Release();

	if (pDIKeyboard != NULL)
	{
		pDIKeyboard->Unacquire();
		pDIKeyboard->Release();
	}
	
}

VOID DeleteMyMesh( Mesh &Obj )
{
	if(Obj.pMeshMaterials != NULL)
		delete [] Obj.pMeshMaterials;
	if( Obj.pMeshTexture )
	{
		for( DWORD i = 0; i < Obj.dwNumber; ++ i )
		{
			if( Obj.pMeshTexture[i] )
				Obj.pMeshTexture[i]->Release();
		}
		delete [] Obj.pMeshTexture;
	}
	if(Obj.pMesh != NULL)
		Obj.pMesh->Release();
}
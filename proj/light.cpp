#include "stdafx.h"
#include "glob.h"
#include "light.h"
#define RANGE 0.3f

CLight::CLight()
{
	ZeroMemory( &LightD, sizeof( D3DLIGHT9 ) );
	ZeroMemory( &LightU, sizeof( D3DLIGHT9 ) );
	ZeroMemory( &LightL, sizeof( D3DLIGHT9 ) );
	ZeroMemory( &LightR, sizeof( D3DLIGHT9 ) );
	ZeroMemory( &LightG, sizeof( D3DLIGHT9 ) );
	ZeroMemory( &tmpPosition, sizeof( tmpPosition ) );
}

VOID  CLight::LightGlob( FLOAT lx, FLOAT ly, FLOAT lz )
{
	D3DLIGHT9 Light;

	//установка света
	ZeroMemory( &Light, sizeof( D3DLIGHT9 ) );

	Light.Type = D3DLIGHT_DIRECTIONAL;

	Light.Diffuse.r = 1.0f;
	Light.Diffuse.g = 1.0f;
	Light.Diffuse.b = 1.0f;

	//Light.Range = 10.0f;

	D3DXVECTOR3 VectorDir = D3DXVECTOR3( lx, ly, lz );
	D3DXVec3Normalize( (D3DXVECTOR3 *) &Light.Direction, &VectorDir ); 

	pDirect3DDevice -> SetLight( 0, &Light );
	pDirect3DDevice -> LightEnable ( 0, TRUE );

	pDirect3DDevice -> SetRenderState ( D3DRS_LIGHTING, TRUE );
	pDirect3DDevice -> SetRenderState ( D3DRS_AMBIENT, 0x00202020 );
}

HRESULT CLight::LightTrafficInit()
{
	// Down
	LightD.Type = D3DLIGHT_POINT;
	LightD.Diffuse.r = 1.0f;
	LightD.Diffuse.g = 0.0f;
	LightD.Diffuse.b = 0.0f;
	LightD.Diffuse.a = 0.0f;
	LightD.Position.x = 5.0f;
	LightD.Position.y = 4.38f;
	LightD.Position.z = -5.3f;
	//LightD.Attenuation0 = 1.0f;
	//LightD.Attenuation1 = 0.0f;
	LightD.Range = RANGE;
	pDirect3DDevice->SetLight( 1, &LightD );

	// Up
	LightU.Type = D3DLIGHT_POINT;
	LightU.Diffuse.r = 1.0f;
	LightU.Diffuse.g = 0.0f;
	LightU.Diffuse.b = 0.0f;
	LightU.Diffuse.a = 0.0f;
	LightU.Position.x = -5.0f;
	LightU.Position.y = 4.38f;
	LightU.Position.z = 5.3f;
	//LightU.Attenuation0 = 1.0f;
	//LightU.Attenuation1 = 0.0f;
	LightU.Range = RANGE;
	pDirect3DDevice->SetLight( 2, &LightU );

	// Left
	LightL.Type = D3DLIGHT_POINT;
	LightL.Diffuse.r = 0.0f;
	LightL.Diffuse.g = 1.0f;
	LightL.Diffuse.b = 0.0f;
	LightL.Diffuse.a = 0.0f;
	LightL.Position.x = -5.3f;
	LightL.Position.y = 3.5f;
	LightL.Position.z = -5.0f;
	//LightL.Attenuation0 = 1.0f;
	//LightL.Attenuation1 = 0.0f;
	LightL.Range = RANGE;
	pDirect3DDevice->SetLight( 3, &LightL );

	// Right
	LightR.Type = D3DLIGHT_POINT;
	LightR.Diffuse.r = 0.0f;
	LightR.Diffuse.g = 1.0f;
	LightR.Diffuse.b = 0.0f;
	LightR.Diffuse.a = 0.0f;
	LightR.Position.x = 5.3f;
	LightR.Position.y = 3.5f;
	LightR.Position.z = 5.0f;
	//LightR.Attenuation0 = 1.0f;
	//LightR.Attenuation1 = 0.0f;
	LightR.Range = RANGE;
	pDirect3DDevice->SetLight( 4, &LightR );

	// Включить освещение для устройства
	pDirect3DDevice -> SetRenderState ( D3DRS_LIGHTING, TRUE );
	pDirect3DDevice -> SetRenderState ( D3DRS_AMBIENT, 0x00202020 );

	return S_OK;
}

HRESULT CLight::LightTrafficEnable()
{
	pDirect3DDevice->LightEnable( 1, TRUE );
	pDirect3DDevice->LightEnable( 2, TRUE );
	pDirect3DDevice->LightEnable( 3, TRUE );
	pDirect3DDevice->LightEnable( 4, TRUE );
	return S_OK;
}



HRESULT CLight::LightTrafficChange( bool &ltChange )
{
	if( ltChange )
	{

		tmpPosition.x = LightD.Position.x;
		tmpPosition.z = LightD.Position.z;

		LightD.Position.x = LightL.Position.x;
		LightD.Position.z = LightL.Position.z;

		LightL.Position.x = tmpPosition.x;
		LightL.Position.z = tmpPosition.z;

		tmpPosition.x = LightU.Position.x;
		tmpPosition.z = LightU.Position.z;

		LightU.Position.x = LightR.Position.x;
		LightU.Position.z = LightR.Position.z;

		LightR.Position.x = tmpPosition.x;
		LightR.Position.z = tmpPosition.z;

		pDirect3DDevice->SetLight( 1, &LightD );
		pDirect3DDevice->SetLight( 3, &LightL );
		pDirect3DDevice->SetLight( 2, &LightU );
		pDirect3DDevice->SetLight( 4, &LightR );

		ltChange = false;	
	}
	
	return S_OK;
}

//устаревшая функция
//HRESULT LightTraffic()
//{
//	D3DLIGHT9 Light2;
//
//	ZeroMemory( &Light2, sizeof( D3DLIGHT9 ) );
//
//	Light2.Type = D3DLIGHT_POINT;
//	Light2.Diffuse.r = 1.0f;
//	Light2.Diffuse.g = 0.0f;
//	Light2.Diffuse.b = 0.0f;
//	Light2.Diffuse.a = 0.0f;
//	Light2.Position.x = 0.0f;
//	Light2.Position.y = 3.0f;
//	Light2.Position.z = 0.0f;
//	//Light2.Attenuation0 = 1.0f;
//	//Light2.Attenuation1 = 0.0f;
//	Light2.Range = RANGE;
//
//	pDirect3DDevice->SetLight( 1, &Light2 );
//	pDirect3DDevice -> LightEnable ( 1, TRUE );
//	pDirect3DDevice -> SetRenderState ( D3DRS_LIGHTING, TRUE );
//	pDirect3DDevice -> SetRenderState ( D3DRS_AMBIENT, 0x00202020 );
//
//
//	return S_OK;
//}

//HRESULT LightTrafficInit()
//{
//	D3DLIGHT9 Light;
//
//
//	// 1
//	ZeroMemory( &Light, sizeof( D3DLIGHT9 ) );
//	Light.Type = D3DLIGHT_POINT;
//	Light.Diffuse.r = 1.0f;
//	Light.Diffuse.g = 0.0f;
//	Light.Diffuse.b = 0.0f;
//	Light.Diffuse.a = 0.0f;
//	Light.Position.x = 5.0f;
//	Light.Position.y = 4.38f;
//	Light.Position.z = -5.3f;
//	//Light.Attenuation0 = 1.0f;
//	//Light.Attenuation1 = 0.0f;
//	Light.Range = RANGE;
//	pDirect3DDevice->SetLight( 1, &Light );
//
//	//включение этого освещения
//	//pDirect3DDevice -> LightEnable( 1, TRUE );
//
//	// 2
//	ZeroMemory( &Light, sizeof( D3DLIGHT9 ) );
//	Light.Type = D3DLIGHT_POINT;
//	Light.Diffuse.r = 1.0f;
//	Light.Diffuse.g = 0.0f;
//	Light.Diffuse.b = 0.0f;
//	Light.Diffuse.a = 0.0f;
//	Light.Position.x = -5.0f;
//	Light.Position.y = 4.38f;
//	Light.Position.z = 5.3f;
//	//Light.Attenuation0 = 1.0f;
//	//Light.Attenuation1 = 0.0f;
//	Light.Range = RANGE;
//	pDirect3DDevice->SetLight( 2, &Light );
//
//	// 3
//	ZeroMemory( &Light, sizeof( D3DLIGHT9 ) );
//	Light.Type = D3DLIGHT_POINT;
//	Light.Diffuse.r = 0.0f;
//	Light.Diffuse.g = 1.0f;
//	Light.Diffuse.b = 0.0f;
//	Light.Diffuse.a = 0.0f;
//	Light.Position.x = -5.3f;
//	Light.Position.y = 3.5f;
//	Light.Position.z = -5.0f;
//	//Light.Attenuation0 = 1.0f;
//	//Light.Attenuation1 = 0.0f;
//	Light.Range = RANGE;
//	pDirect3DDevice->SetLight( 3, &Light );
//
//	// 4
//	ZeroMemory( &Light, sizeof( D3DLIGHT9 ) );
//	Light.Type = D3DLIGHT_POINT;
//	Light.Diffuse.r = 0.0f;
//	Light.Diffuse.g = 1.0f;
//	Light.Diffuse.b = 0.0f;
//	Light.Diffuse.a = 0.0f;
//	Light.Position.x = 5.3f;
//	Light.Position.y = 3.5f;
//	Light.Position.z = 5.0f;
//	//Light.Attenuation0 = 1.0f;
//	//Light.Attenuation1 = 0.0f;
//	Light.Range = RANGE;
//	pDirect3DDevice->SetLight( 4, &Light );
//
//	// Включить освещение для устройства
//	pDirect3DDevice -> SetRenderState ( D3DRS_LIGHTING, TRUE );
//	pDirect3DDevice -> SetRenderState ( D3DRS_AMBIENT, 0x00202020 );
//
//	return S_OK;
//}

//
//HRESULT LightTrafficEnable()
//{
//	pDirect3DDevice->LightEnable( 1, TRUE );
//	pDirect3DDevice->LightEnable( 2, TRUE );
//	pDirect3DDevice->LightEnable( 3, TRUE );
//	pDirect3DDevice->LightEnable( 4, TRUE );
//	return S_OK;
//}
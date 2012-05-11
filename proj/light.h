#pragma once
#include "stdafx.h"

//VOID LightMaterial( FLOAT lx, FLOAT ly, FLOAT lz );
//HRESULT LightTraffic();
//HRESULT LightTrafficInit();
//HRESULT LightTrafficEnable();
//HRESULT LightTrafficChange();

class CLight
{
public:
	CLight();
	VOID LightGlob( FLOAT lx, FLOAT ly, FLOAT lz ); // устанавливает направленное освещение
	HRESULT LightTrafficInit();						// Инициализирует свет на светофорах 
	HRESULT LightTrafficEnable();					// делает видимыми обьекты света на светафорах
	HRESULT LightTrafficChange( bool &ltChange );	// смена света светофора

private:
	D3DLIGHT9 LightD; // нижний светофор
	D3DLIGHT9 LightU; // верхний светофор
	D3DLIGHT9 LightL; // левый светофор
	D3DLIGHT9 LightR; // правый светофор

	D3DLIGHT9 LightG; // глобальной освещений

	D3DVECTOR tmpPosition; // для смены света
};
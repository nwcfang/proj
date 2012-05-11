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
	VOID LightGlob( FLOAT lx, FLOAT ly, FLOAT lz ); // ������������� ������������ ���������
	HRESULT LightTrafficInit();						// �������������� ���� �� ���������� 
	HRESULT LightTrafficEnable();					// ������ �������� ������� ����� �� ����������
	HRESULT LightTrafficChange( bool &ltChange );	// ����� ����� ���������

private:
	D3DLIGHT9 LightD; // ������ ��������
	D3DLIGHT9 LightU; // ������� ��������
	D3DLIGHT9 LightL; // ����� ��������
	D3DLIGHT9 LightR; // ������ ��������

	D3DLIGHT9 LightG; // ���������� ���������

	D3DVECTOR tmpPosition; // ��� ����� �����
};
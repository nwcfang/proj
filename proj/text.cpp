#include "stdafx.h"
#include "glob.h"

VOID DrawMyText(  
	int dtx, int dty, int dtx1, int dty1  )
{
	// ���������� ��������������
	rec.left = dtx;
	rec.top = dty;
	rec.right = dtx1;
	rec.bottom = dty1;

	// �������������� �����
	/*D3DXCreateFontW( pDirect3DDevice, 
		30, 10, 0, 0, FALSE, DEFAULT_CHARSET, 0, 0, DEFAULT_PITCH|FF_MODERN,   L"Arial",
		&pFont );*/
	pFont->DrawTextW( NULL, L"�������� ������", 15, &rec, DT_NOCLIP,  D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ));

}

VOID TextInit()
{
	D3DXCreateFontW( pDirect3DDevice, 
		21, 8, 1, 0, FALSE, DEFAULT_CHARSET, 0, 0, DEFAULT_PITCH|FF_MODERN,   L"Arial",
		&pFont );
	
}
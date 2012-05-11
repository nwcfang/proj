#include "stdafx.h"
#include "glob.h"
#include "delete.h"

INT InitInput( HWND hwnd, HINSTANCE hinstance )
{
	if( DirectInput8Create( hinstance, DIRECTINPUT_VERSION,  IID_IDirectInput8, (LPVOID *)&pDInput, NULL ) != DI_OK )
		return 1;
	if( pDInput->CreateDevice( GUID_SysKeyboard, &pDIKeyboard, NULL ) != DI_OK )
		return 1;
	if( pDIKeyboard->SetDataFormat( &c_dfDIKeyboard) != DI_OK )
		return 1;
	if( pDIKeyboard->SetCooperativeLevel( hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND ) != DI_OK )
		return 1;

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	
	
	dipdw.dwData = KEYBOARD_BUFFER_SIZE;

	if( pDIKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) != DI_OK )
		return 1;
	if( pDIKeyboard->Acquire() != DI_OK)
		return 1;

	return 0;
}

int GetDInput()
{
	if(pDIKeyboard->GetDeviceState(256, (LPVOID)keyboard) != DI_OK)
		return 1;
	
	pDIKeyboard->Acquire();
	
	//выставить или убрать машину слева
	if(keyboard[DIK_LEFT] & 0x80 )
		keyEvent[DIK_LEFT] = true;
	if(((keyboard[DIK_LEFT] & 0x80) == false) && (keyEvent[DIK_LEFT] == true) )
	{
		config.leftCar = !config.leftCar;
		keyEvent[DIK_LEFT] = false;
	}

	//выставить или убрать машину справа
	if(keyboard[DIK_RIGHT] & 0x80 )
		keyEvent[DIK_RIGHT] = true;
	if(((keyboard[DIK_RIGHT] & 0x80) == false) && (keyEvent[DIK_RIGHT] == true) )
	{
		config.rightCar = !config.rightCar;
		keyEvent[DIK_RIGHT] = false;
	}

	//выставить или убрать машину сверху
	if(keyboard[DIK_UP] & 0x80 )
		keyEvent[DIK_UP] = true;
	if(((keyboard[DIK_UP] & 0x80) == false) && (keyEvent[DIK_UP] == true) )
	{
		config.topCar = !config.topCar;
		keyEvent[DIK_UP] = false;
	}

	//выставить красный\зелёный\убрать светофор
	if(keyboard[DIK_DOWN] & 0x80 )
		keyEvent[DIK_DOWN] = true;
	if(((keyboard[DIK_DOWN] & 0x80) == false) && (keyEvent[DIK_DOWN] == true) )
	{
		config.trafficLight = !config.trafficLight;
		keyEvent[DIK_DOWN] = false;
	}

	if(keyboard[DIK_ESCAPE] & 0x80 ) 
	{
		PostQuitMessage(0);
	}

	// переход к стадии задания маршрутов
		if(keyboard[DIK_RETURN] & 0x80 )
		keyEvent[DIK_RETURN] = true;
	if(((keyboard[DIK_RETURN] & 0x80) == false) && (keyEvent[DIK_RETURN] == true) )
	{
		s_scourse = true;
		keyEvent[DIK_RETURN] = false;
	}


	return 0;
}

int GetDInputInStart()
{
	if(pDIKeyboard->GetDeviceState(256, (LPVOID)keyboard) != DI_OK)
		return 1;

	pDIKeyboard->Acquire();
	
	if(keyboard[DIK_DOWN] & 0x80)
		g_car = 1;

	if(keyboard[DIK_UP] & 0x80)
		g_car = 0;

	if(keyboard[DIK_RETURN] & 0x80 )
		keyEvent[DIK_RETURN] = true;
	if(((keyboard[DIK_RETURN] & 0x80) == false) && (keyEvent[DIK_RETURN] == true) )
	{
		s_road = 1;
		keyEvent[DIK_RETURN] = false;
	}

	if(keyboard[DIK_ESCAPE] & 0x80 ) 
	{
		//DeleteDirect3D();
		PostQuitMessage(0);
	}
		
	return 0;
}

HRESULT GetDInputCourse()
{

	if(pDIKeyboard->GetDeviceState(256, (LPVOID)keyboard) != DI_OK)
		return E_FAIL;
	pDIKeyboard->Acquire();

	if(keyboard[DIK_LEFT] & 0x80 )
		keyEvent[DIK_LEFT] = true;
	if(((keyboard[DIK_LEFT] & 0x80) == false) && (keyEvent[DIK_LEFT] == true) )
	{
		keyEvent[DIK_LEFT] = false;
		s_left = true;
	}
	if(keyboard[DIK_RIGHT] & 0x80 )
		keyEvent[DIK_RIGHT] = true;
	if(((keyboard[DIK_RIGHT] & 0x80) == false) && (keyEvent[DIK_RIGHT] == true) )
	{
		keyEvent[DIK_RIGHT] = false;
		s_right = true;
	}
	if(keyboard[DIK_UP] & 0x80 )
		keyEvent[DIK_UP] = true;
	if(((keyboard[DIK_UP] & 0x80) == false) && (keyEvent[DIK_UP] == true) )
	{
		keyEvent[DIK_UP] = false;
		s_up = true;
	}
	

	if(keyboard[DIK_ESCAPE] & 0x80 )
	{
		PostQuitMessage(0);
	}
	return S_OK;
}

HRESULT GetDInputCourse2()
{
	if(pDIKeyboard->GetDeviceState(256, (LPVOID)keyboard) != DI_OK)
		return E_FAIL;

	pDIKeyboard->Acquire();

	if(keyboard[DIK_UP] & 0x80 )
		keyEvent[DIK_UP] = true;
	if(((keyboard[DIK_UP] & 0x80) == false) && (keyEvent[DIK_UP] == true) )
	{
		s_colorCh = true;
		keyEvent[DIK_UP] = false;
	}

	if(keyboard[DIK_DOWN] & 0x80 )
		keyEvent[DIK_DOWN] = true;
	if(((keyboard[DIK_DOWN] & 0x80) == false) && (keyEvent[DIK_DOWN] == true) )
	{
		s_colorCh = true;
		keyEvent[DIK_DOWN] = false;
	}	

	if(keyboard[DIK_ESCAPE] & 0x80 )
		PostQuitMessage(0);

	if(keyboard[DIK_RETURN] & 0x80 )
		s_action = true;

	return S_OK;
}

HRESULT GetInputPlacer()
{
	if(pDIKeyboard->GetDeviceState(256, (LPVOID)keyboard) != DI_OK)
		return 1;
	pDIKeyboard->Acquire();

	return S_OK;
}

HRESULT GetDInputAction()
{
	if(pDIKeyboard->GetDeviceState(256, (LPVOID)keyboard) != DI_OK)
		return E_FAIL;

	pDIKeyboard->Acquire();
	
	if(keyboard[DIK_ESCAPE] & 0x80 )
		PostQuitMessage(0);

	//if(keyboard[DIK_UP] & 0x80 )
	//	keyEvent[DIK_UP] = true;
	//if(((keyboard[DIK_UP] & 0x80) == false) && (keyEvent[DIK_UP] == true) )
	//{
	//	s_colorCh = true;
	//	keyEvent[DIK_UP] = false;
	//}

	if(keyboard[DIK_RETURN] & 0x80 )
		keyEvent[DIK_RETURN] = true;
	if(((keyboard[DIK_RETURN] & 0x80) == false) && (keyEvent[DIK_RETURN] == true) )
	{
		s_start = true;
		keyEvent[DIK_RETURN] = false;
	}	
	/*if(keyboard[DIK_RETURN] & 0x80 )
		s_play = true;*/

	return S_OK;
}

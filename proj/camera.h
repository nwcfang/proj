#pragma once
#include "stdafx.h"

class CCamera
{
public:
	CCamera();
	void SetPostion( D3DXVECTOR3 *Pos );
	void MoveForward( float Dist );
	void MoveRight( float Dist );
	void MoveUp( float Dist );
	void MoveInDirection( float Dist, D3DXVECTOR3 *Dir ); 

	void RotateDown( float Angle );
	void RotateRight( float Angle );
	void Roll( float Angle );

	bool Update();

protected:
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_LookAt;
	D3DXVECTOR3 m_Right;
	D3DXVECTOR3 m_Up;

	bool m_bChanged;

	float m_RotateAroundUp;
	float m_RotateAroundRight;
	float m_RotateAroundLookAt;

	D3DXMATRIX m_MatView;

};
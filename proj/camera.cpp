#include "stdafx.h"
#include "camera.h"
#include "glob.h"

CCamera::CCamera()
{
	m_Position	= D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f ); 
	m_LookAt	= D3DXVECTOR3 ( 0.0f, 0.0f, 1.0f );
	m_Right		= D3DXVECTOR3 ( 1.0f, 0.0f, 0.0f );
	m_Up		=  D3DXVECTOR3 ( 0.0f, 1.0f, 0.0f );

	m_RotateAroundLookAt = m_RotateAroundRight = m_RotateAroundUp = 0;

	D3DXMatrixIdentity( &m_MatView );
}

bool CCamera::Update()
{
	if( m_bChanged )
	{
		D3DXMATRIX MatTotal;
		D3DXMATRIX MatRotateAroundRight;
		D3DXMATRIX MatRotateAroundUp;
		D3DXMATRIX MatRotateAroundLookAt;

		D3DXMatrixRotationAxis( &MatRotateAroundRight, &m_Right, m_RotateAroundRight );
		D3DXMatrixRotationAxis( &MatRotateAroundUp, &m_Up, m_RotateAroundUp );
		D3DXMatrixRotationAxis( &MatRotateAroundLookAt, &m_LookAt, m_RotateAroundLookAt );

		D3DXMatrixMultiply( &MatTotal, &MatRotateAroundUp, &MatRotateAroundRight );
		D3DXMatrixMultiply( &MatTotal, &MatRotateAroundLookAt, &MatTotal );

		D3DXVec3TransformCoord( &m_Right, &m_Right, &MatTotal );
		D3DXVec3TransformCoord( &m_Up, &m_Up, &MatTotal );
		D3DXVec3Cross( &m_LookAt, &m_Right, &m_Up );

		if( fabs( D3DXVec3Dot( &m_Up, &m_Right ) ) > 0.01 )
		{
			D3DXVec3Cross( &m_Up, &m_LookAt, &m_Right );
		}

		D3DXVec3Normalize( &m_Right, &m_Right );
		D3DXVec3Normalize( &m_Up, &m_Up );
		D3DXVec3Normalize( &m_LookAt, &m_LookAt );

		float fView41, fView42, fView43;
		fView41 = -D3DXVec3Dot( &m_Right, &m_Position );
		fView42 = -D3DXVec3Dot( &m_Up, &m_Position );
		fView43 = -D3DXVec3Dot( &m_LookAt, &m_Position );

		m_MatView = D3DXMATRIX( m_Right.x, m_Up.x, m_LookAt.x, 0.0f,
								m_Right.y, m_Up.y, m_LookAt.y, 0.0f,
								m_Right.z, m_Up.z, m_LookAt.z, 0.0f,
								fView41, fView42, fView43, 1.0f);
	}

	pDirect3DDevice->SetTransform( D3DTS_VIEW, &m_MatView );

	m_RotateAroundLookAt = m_RotateAroundRight = m_RotateAroundUp = 0.0f;
	m_bChanged = false;

	return true;
}

void CCamera::RotateDown( float Angle )
{
	m_RotateAroundRight += Angle;
	m_bChanged = true;
}

void CCamera::RotateRight( float Angle )
{
	m_RotateAroundUp += Angle;
	m_bChanged = true;
}

void CCamera::Roll( float Angle )
{
	m_RotateAroundLookAt += Angle;
	m_bChanged = true;
}

void CCamera::SetPostion( D3DXVECTOR3 *Pos )
{
	m_Position = *Pos;
	m_bChanged = true;
}

void CCamera::MoveForward( float Dist )
{
	m_Position += Dist*m_LookAt;
	m_bChanged = true;
}

void CCamera::MoveRight( float Dist )
{
	m_Position += Dist*m_Right;
	m_bChanged = true;
}

void CCamera::MoveUp( float Dist )
{
	m_Position += Dist*m_Up;
	m_bChanged = true;
}

void CCamera::MoveInDirection( float Dist, D3DXVECTOR3 *Dir )
{
	m_Position += Dist*(*Dir);
	m_bChanged = true;
}
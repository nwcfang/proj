// ����� ������� ������� �������������� ��� ������ � XML
#pragma once 
#include "stdafx.h"
#include "mesh.h"

struct KeyVal
{
	//BSTR key[12];
	LPSTR key[12];
	int val[12];

};

// �������� ���������
class CParser
{
private:
	int _CountCars;
	vector<Mesh *> *_VListCars;				// ������ �����
	
	Mesh *_carGhost;						// ��������� �� ������ �������
	KeyVal _kv;								// ��������� � �������

	HRESULT SetIterator();					// ������ ��������
	HRESULT InitGhostCar();					// ������� ������ �������
	HRESULT ReleseGhostCar();				// ���������� ������ �������

public:

	vector<Mesh *>::iterator iteratorList; // �������� ��� ������ �����
	
	CParser();
	~CParser();
	HRESULT CalcCountCars();	// ������� ����������� ����� �� ������
	HRESULT InitCarList();		// �������� ������ �����
	HRESULT YourTurn( Mesh* ObjM, Mesh* ObjL, Mesh* ObjT, Mesh* ObjR  );			// ��������� ����� �� ������ ���������
	HRESULT ReleseCarList();
	HRESULT MoveCars();			// �������� � �������� ������
	
	HRESULT Iteration();		// ������ ����������������� ��������� (������)

};

// �������������������� � ������������� �� ���� ��������� ������ ��� ������ � ������������ � � ������� �����������
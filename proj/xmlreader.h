// «десь описаны функции предназначеные дл€ работы с XML
#pragma once 
#include "stdafx.h"
#include "mesh.h"

struct KeyVal
{
	//BSTR key[12];
	LPSTR key[12];
	int val[12];

};

// ќсновной интерфейс
class CParser
{
private:
	int _CountCars;
	vector<Mesh *> *_VListCars;				// список машин
	
	Mesh *_carGhost;						// указатель на машину призрак
	KeyVal _kv;								// структура с данными

	HRESULT SetIterator();					// задать итератор
	HRESULT InitGhostCar();					// создать машину призрак
	HRESULT ReleseGhostCar();				// уничтожить машину призрак

public:

	vector<Mesh *>::iterator iteratorList; // итератор дл€ списка машин
	
	CParser();
	~CParser();
	HRESULT CalcCountCars();	// подсчет колличества машин на дороге
	HRESULT InitCarList();		// создание списка машин
	HRESULT YourTurn( Mesh* ObjM, Mesh* ObjL, Mesh* ObjT, Mesh* ObjR  );			// определет может ли машина двигатьс€
	HRESULT ReleseCarList();
	HRESULT MoveCars();			// приводит в движение машины
	
	HRESULT Iteration();		// способ инкрементировани€ итератора (кольцо)

};

// поэксперементировать с конструктором на счет выделени€ пам€ти под вектор и освобождени€ еЄ с помощью деструктора
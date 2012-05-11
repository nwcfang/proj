#include "stdafx.h"
#include "glob.h"
#include "mesh.h"
#include "xmlreader.h"




CParser::CParser()
{
	_CountCars = 1;
	_VListCars = NULL;
}

CParser::~CParser()
{
	/*_VListCars->clear();
	ReleseGhostCar();

	delete _VListCars;
	_VListCars = NULL;*/
}

HRESULT CParser::CalcCountCars()
{
	if( config.leftCar )
		++ _CountCars;
	if( config.rightCar )
		++ _CountCars;
	if( config.topCar )
		++ _CountCars;
	return S_OK;
}

HRESULT CParser::InitCarList()
{
	_VListCars = new vector<Mesh *>;
	if( _VListCars == NULL )
	{
		MessageBox( NULL, L"Could new vector", L"ERROR", MB_OK );
		return E_FAIL;
	}

	InitGhostCar();

	_VListCars->push_back( &carMain );

	if( config.leftCar )
		_VListCars->push_back( &carL );
	else
		_VListCars->push_back( _carGhost );

	if( config.topCar )
		_VListCars->push_back( &carT );
	else
		_VListCars->push_back( _carGhost );

	if( config.rightCar )
		_VListCars->push_back( &carR );
	else
		_VListCars->push_back( _carGhost );

	SetIterator();

	return S_OK;
}

HRESULT CParser::ReleseCarList()
{
	ReleseGhostCar();
	_VListCars->clear();
	//delete []_VListCars;
	return S_OK;
}

HRESULT CParser::InitGhostCar()
{
	_carGhost = new Mesh;
	_carGhost->MoveTo = ANYWHERE;
	return S_OK;
}

HRESULT CParser::ReleseGhostCar()
{
	delete _carGhost;
	return S_OK;
}

HRESULT CParser::Iteration()
{
	++ iteratorList;
	if( iteratorList >= _VListCars->end() )
		SetIterator();
	return S_OK;
}

HRESULT CParser::SetIterator()
{
	iteratorList = _VListCars->begin();
	return S_OK;
}

HRESULT CParser::YourTurn(Mesh* ObjM, Mesh* ObjL, Mesh* ObjT, Mesh* ObjR  )
{	
	if(  (ObjM->MoveTo == ANYWHERE) && (ObjL->MoveTo == ANYWHERE) && (ObjT->MoveTo == ANYWHERE) && (ObjR->MoveTo == ANYWHERE) )
		return S_FALSE;

	CoInitialize(NULL);
   
       // Create an instance of the parser
       CComPtr<IXMLDOMDocument> spXMLDOM;
	   CComPtr<IXMLDOMElement> spXMLEle;
       HRESULT hr = spXMLDOM.CoCreateInstance(__uuidof(DOMDocument));
        if ( FAILED(hr) ) MessageBox( NULL, L"Couldnt spXMLDOM.CoCreateInstance", L"ERROR", MB_OK );
        if ( spXMLDOM.p == NULL ) MessageBox( NULL, L"spXMLDOM.p == NULL", L"ERROR", MB_OK );
 

        // Load the XML document file...
        VARIANT_BOOL bSuccess = false;
        hr = spXMLDOM->load(CComVariant(L"rulesV01.xml"),&bSuccess);
        if ( FAILED(hr) ) MessageBox( NULL, L"Unable to load XML document into the parser", L"ERROR", MB_OK );
        if ( !bSuccess )  MessageBox( NULL, L"Unable to load XML document into the parser", L"ERROR", MB_OK );

        // Check for <xmldata>
        // <xmlnode>...
        //
        // Construct search string
        // "xmldata/xmlnode"
        CComBSTR bstrSS(L"xmldata/xmlnode");
        CComPtr<IXMLDOMNode> spXMLNode;
		CComPtr<IXMLDOMNodeList> spXMLNodeList;
		CComPtr<IXMLDOMNodeList> spXMLNodeList2;

		//================================================================================
		// Чтение concept
		//================================================================================
		CComBSTR bstrSS3(L"concept");
		hr = spXMLDOM->getElementsByTagName (bstrSS3, &spXMLNodeList2);
        if ( FAILED(hr) )	MessageBox( NULL, L"Unable to locate list 'concept' XML node", L"ERROR", MB_OK );
        if ( spXMLNodeList2.p == NULL ) MessageBox( NULL, L"Unable to locate list 'concept' XML node", L"ERROR", MB_OK );

		CComPtr<IXMLDOMNode> pIDOMNode;
		CComPtr<IXMLDOMNode> pIDOMNode2;
		BSTR bstrItemText;
		long value;

		

		CComPtr<IXMLDOMNamedNodeMap> pIDOMNamesNoteMap;
		CComPtr<IXMLDOMElement> Element;
		VARIANT val;
		BSTR str;
		VariantInit( &val );
		hr = spXMLNodeList2->get_length(&value);
		spXMLNodeList2->reset();
		long value3 = value - 3;
		for(int ii = 0; ii < value3; ii++)
		{
			
			spXMLNodeList2->get_item(ii, &pIDOMNode);
			Element = pIDOMNode;
			Element->getAttribute(L"name", &val );
			if ( val.vt == VT_BSTR ) 
			{
				USES_CONVERSION;
				_kv.key[ii] = W2A(val.bstrVal);
			}			
			//_kv.key[ii] = val.bstrVal;
			if(pIDOMNode )
			{
				pIDOMNode = NULL; 
				//pIDOMNode->Release();								//не работает почемуто
			}
		}
				
		for( int j = 0; j < 12; ++ j )
		{
			_kv.val[j] = 0;
		}

// Ппределение массива для сравнения с xml правилами 
//===============================================================================

		if( ObjM->MoveTo == LEFT )
			_kv.val[0] = 1;
		if( ObjM->MoveTo == RIGHT )
			_kv.val[1] = 1;
		if( ObjM->MoveTo == UP )
			_kv.val[2] = 1;

		if( ObjL->MoveTo != ANYWHERE )
		{
			if( ObjL->MoveTo == LEFT )
				_kv.val[4] = 1;
			if( ObjL->MoveTo == RIGHT )
				_kv.val[3] = 1;
			if( ObjL->MoveTo == UP )
				_kv.val[5] = 1;
		}
		if( ObjR->MoveTo != ANYWHERE )
		{
			if( ObjR->MoveTo == LEFT )
				_kv.val[7] = 1;
			if( ObjR->MoveTo == RIGHT )
				_kv.val[6] = 1;
			if( ObjR->MoveTo == UP )
				_kv.val[8] = 1;
		}
		if( ObjT->MoveTo != ANYWHERE )
		{
			if( ObjT->MoveTo == LEFT )
				_kv.val[10] = 1;
			if( ObjT->MoveTo == RIGHT )
				_kv.val[9] = 1;
			if( ObjT->MoveTo == UP )
				_kv.val[11] = 1;
		}
//===============================================================================

		if( ObjM->MoveTo == LEFT )
			spXMLNodeList2->get_item( 13 , &pIDOMNode2 );
		else if( ObjM->MoveTo == RIGHT )
			spXMLNodeList2->get_item( 14 , &pIDOMNode2 );		
		else if( ObjM->MoveTo == UP )
			spXMLNodeList2->get_item( 12 , &pIDOMNode2 );
		if( ObjM->MoveTo != ANYWHERE )
		{
			CComPtr<IXMLDOMNodeList> childList;
			CComPtr<IXMLDOMNode> firstChild;
			CComPtr<IXMLDOMNode> Child;
			//VARIANT_BOOL hasChi;
			//pIDOMNode2->hasChildNodes( &hasChi );
			firstChild = NULL;
			pIDOMNode2->get_firstChild( &firstChild ) ;

			firstChild->get_childNodes( &childList );
			//firstChild=NULL;
			childList->get_length( &value);
			LPSTR lpstrMsg;
			childList->reset();

			int rs = 0;
			int container = 0;
			for ( int f = 0; f < value; ++ f )
			{
				childList->get_item(f, &Child);
				Element = Child;
				Child = NULL;
				Element->getAttribute(L"name", &val );
				if ( val.vt == VT_BSTR ) 
				{
					USES_CONVERSION;
					lpstrMsg = W2A(val.bstrVal);
				}
				for( int i = 0; i < COUNTOFRULS; ++ i )
				{

					rs = strcmp( _kv.key[i], lpstrMsg );
					if( rs == 0 )
					{
						container = i;
						break;
					}
				}
				if ( rs != 0 )
				{
					MessageBox( NULL, L"Ошибка в поиске правила. Правило не найдено", L"ERROR", MB_OK );
					return E_FAIL;
				}

				hr = Element->getAttribute(L"ODZname", &val );
				if( hr != S_OK )
				{
					MessageBox( NULL, L"Аттрибут ODZname ненайден.", L"ERROR", MB_OK );
					return E_FAIL;
				}

				if ( val.vt == VT_BSTR ) 
				{
					USES_CONVERSION;
					lpstrMsg = W2A(val.bstrVal);
				}
				if ( strcmp( lpstrMsg, "1" ) != 0 )
				{
					if( _kv.val[container] == 1 )
						return E_ABORT;
				}
				firstChild = NULL;
			}
			childList=NULL;
			Child = NULL;
		}
		if( ObjM->MoveTo == ANYWHERE )
		{
			return E_ABORT;
		}
	CoUninitialize();
	return S_OK;
}



HRESULT CParser::MoveCars()
{
	vector<Mesh *> ::iterator iter = _VListCars->begin();
	for( int i = 0; i < _VListCars->size(); ++ i )
	{

		if( (*iter)->ICanGo == true)
		{
			if( S_OK == (*iter)->JustDoIt() )
			{
				ReleseCarList();
				first = true;
				return S_OK;
			}
		}

		++ iter;
	}
	return S_OK;
}
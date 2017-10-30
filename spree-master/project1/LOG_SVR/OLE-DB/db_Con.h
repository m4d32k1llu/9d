#pragma	   once

#include <objbase.h>
#include <atldbcli.h>
#include <oledb.h>

/*
	OLE DB Connection Module
	
	2004.06.03		ozzywow
*/


class _j_DB_Con
{

protected:
	HRESULT hr ;
	CDataSource _db ;

public:	
	
	bool init( LPCOLESTR ConStr );					// �ʱ�ȭ(����)
	void release();									// ����	(����)	
	HRESULT GetHresult() { return hr ; } 
	HRESULT SessionLink( CSession * Session ) ;		// ���� ����	
};

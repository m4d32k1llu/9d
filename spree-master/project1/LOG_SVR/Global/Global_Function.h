#pragma once


#ifndef _EXCLUDE_WINSOCK
#include <winsock2.h>
#endif
#include <stdio.h>
#include <Iphlpapi.h>

#include <atldbcli.h>


#define SAFE_DELETE(x) if(x) delete x ;

// gf_ : global function

typedef unsigned (__stdcall *PTHREAD_START) (void *) ;

#define BEGINTHREADEX(psa,cbStack,pfnStartAddr,pvParam,fdwCreate,pdwThreadID) \
	((HANDLE) _beginthreadex((void*)(psa),(unsigned)(cbStack), \
	(PTHREAD_START)(pfnStartAddr),(void*)(pvParam), \
(unsigned)(fdwCreate),(unsigned*)(pdwThreadID)))


//=================================================================================
//  �ٸ� ������ ��Ŷ�� ������. ���� �ڵ�� ���� ������ ��´�. 
int gf_SendtoServer( int nServer, unsigned int length, char * packet ) ;


//=================================================================================
//  Random �� ������ : Global Function 
int gf_Random( int num ) ; 


//=================================================================================
//  ������ Ÿ�� �ڵ� : Global Function
unsigned short gf_Get_CurrTime( unsigned int currTick ) ;


//=================================================================================
// float,int ���밪 ���ϱ�
inline float gf_ABS( float x ) { return (x>0.0f) ? x : -x ; } ;
inline int   gf_ABS( int x )   { return (x>0)    ? x : -x ; } ;
inline float gf_Dist( float x, float z ) { 
	x = (x>0.0f) ? x : -x ; 
	z = (z>0.0f) ? z : -z ;
	return x+z ;
}


inline short Get_Tile( float x, float z ) {
	int ix = (int)x + 512 ;
	int iz = (int)z + 512 ;

	if( ix > 1020 ) ix = 1000 ;
	else if( ix < 1 ) ix = 1 ;
	if( iz > 1020 ) iz = 1000 ;
	else if( iz < 1 ) iz = 1 ;

	// return ((ix>>6)<<4) + (iz>>6) ;		// for 64
	return (short) (((ix>>7)<<3) + (iz>>7)) ;		// for 128
}

inline short Get_RefTile( float x, float z ) {
	int ix = (int)x + 576 ;
	int iz = (int)z + 576 ;
	
	if( ix > 1124 ) ix = 1100 ;
	else if( ix < 0 ) ix = 1 ;
	if( iz > 1124 ) iz = 1100 ;
	else if( iz < 0 ) iz = 1 ;
	
	// return ((ix>>6)<<4) + (iz>>6) ;		// for 64
	return (short) (((ix>>7)*9) + (iz>>7)) ;		// for 128
}


// �ʷ� ������� ��.
#define _SECOND_YEAR_		32140800			// 1�� 
#define _SECOND_MONTH_		2678400				// 31 �� 
#define _SECOND_DAY_		86400				// 1��
#define _SECOND_HOUR_		3600				// 1�ð�
#define _SECOND_MINUTE_		60					// 1��

#define _GAME_TIME_MONTH_		864000
#define _GAME_TIME_DAY_			28800
//=================================================================================
// Global �ð� : 2002/11/04
// ��� �������� �� Ŭ������ �ν��Ͻ��� ���� �ð��� �����. 
class _h_Global_Time
{
protected:
	int		m_baseGlobal ;
	int		m_baseGameTime ;
	int		m_baseLocal ;

public:
	_h_Global_Time()	{}

public:
	int inittime(SYSTEMTIME * curTime);	
	int getGlobalTime() ;	// ���� �ð� ���ϱ�
 	int getGameTime() ;		// ���ӿ����� �ð�.
} ;

bool is_lowcase( char * acc ) ;

inline int gf_strlen( char * pName ) {// ĳ���� �̸� �˻翡�� ����. �ٸ� �ſ� ���� �ȵȴ�. 
	int len = 0 ;
	char * p = pName ;
	bool err = false ;
	
	while( *p != 0 ) {
		if( *p == ' ' ) {
			err = true ;
			break ;
		}
		len++ ;
		p++ ;
	}
	
	if( err )
		return 0 ;
	return len ;
}


//################ HSS Added... 2003.4.2
// ���������� �Ÿ� ���ϴ� �Լ� 
float Get_Distance( float x, float z, float gotoX, float gotoZ ) ;
//################ HSS Ended.. 2003.4.2

//#################################################################################
void PrintConsole( const char *format,... ) ;
//#################################################################################

void ip_char(DWORD ip, char *buf) ;


int	GetGlobalCount();

inline int GetCheckTime();

#define _COUNT_FILE_	"Count.db"
class _h_Global_Count
{
protected:
	FILE *		m_fpCount ;
	int			m_iCount ;
	
	CRITICAL_SECTION	m_cs ;
public:
	bool Init() ;
	
	inline void Lock()		{ EnterCriticalSection( &m_cs ) ; }
	inline void Unlock()	{ LeaveCriticalSection( &m_cs ) ; }
	
	int get_uid() ;	// ����ũ ���̵� ��� �Լ�
	void write() ;
	
	void Release() ;	// ���α׷� ����� �� �Լ� ȣ��
} ;



inline char * GetMyIP( char mode = 0 /*0:LAN, 1:WAN*/)
{
	// MyIP Address
	static char m_cMyOuterip[17] ;		// �ܺ�
	static char m_cMyInnerip[17] ;		// ����

	memset(m_cMyOuterip, 0, sizeof(m_cMyOuterip) ) ;
	memset(m_cMyInnerip, 0, sizeof(m_cMyInnerip) ) ;
	
	DWORD dwSize = 0 ;
	GetIpAddrTable( NULL, &dwSize, FALSE ) ;
	
	PMIB_IPADDRTABLE pIpAddrTable = (PMIB_IPADDRTABLE)new BYTE[dwSize] ;
	
	char myOutIP[16] ;
	
	if( pIpAddrTable ) {
		
		if( GetIpAddrTable( pIpAddrTable, &dwSize, FALSE ) == NO_ERROR ) {
			
			printf( "pIpAddrTable->dwNumEntries = %u\n", pIpAddrTable->dwNumEntries ) ;
			
			if( pIpAddrTable->dwNumEntries > 0 ) 
			{
				for( int i = 0 ; i < (int)pIpAddrTable->dwNumEntries ; i++ ) 
				{
					in_addr ina ;
					ina.S_un.S_addr = pIpAddrTable->table[i].dwAddr ;
					
					strcpy( myOutIP, inet_ntoa( ina ) ) ;
					
					if( /*myOutIP[0] == '0' 
						|| */( myOutIP[0] == '1' && myOutIP[1] == '2' && myOutIP[2] == '7' ) 
						|| (myOutIP[0] == '1' && myOutIP[1] == '9' && myOutIP[2] == '2'))
						;
					else
					{
						sprintf("%s\n",myOutIP) ;
						if( //(myOutIP[0] == '1' && myOutIP[1] == '9' && myOutIP[2] == '2') ||
							(myOutIP[0] == '1' && myOutIP[1] == '0' && myOutIP[2] == '.')  )
						{						
							if( m_cMyInnerip[0] == 0 || myOutIP[2] == '.' )	// 10.10... �� �����ϴ� �����Ǹ� �켱���� �Ѵ�.
							{
								strcpy( m_cMyInnerip, myOutIP ) ;
							}							
						}
						else
						{
							strcpy( m_cMyOuterip, myOutIP ) ;
						}
						
					}
				}
			}
		}
	}
	
	delete[] pIpAddrTable ;
	
	if( mode == 0 ) {
#ifdef _DEBUG
		memcpy( m_cMyInnerip, m_cMyOuterip, 17 ) ;
#else
		if( m_cMyInnerip[0] == 0  )
		{
			memcpy( m_cMyInnerip, m_cMyOuterip, 17 ) ;
		}
#endif
		return m_cMyInnerip ;
	}
	else {
		return m_cMyOuterip ;
	}
}


inline void DBErrorOutput(HRESULT hrErr)
{
#ifndef _OLE_DB_ERROR_TRACE_
	return ;
#endif
	
	CDBErrorInfo ErrorInfo;
	ULONG        cRecords;
	HRESULT      hr;
	ULONG        i;
	CComBSTR     bstrDesc, bstrHelpFile, bstrSource;
	GUID         guid;
	DWORD        dwHelpContext;
	WCHAR        wszGuid[40];
	USES_CONVERSION;
	
	// If the user passed in an HRESULT then trace it
	if (hrErr != S_OK)
		::PrintConsole("OLE DB Error Record dump for hr = 0x%x\n", hrErr);
	
	LCID lcLocale = GetSystemDefaultLCID();
	
	hr = ErrorInfo.GetErrorRecords(&cRecords);
	if (FAILED(hr) && ErrorInfo.m_spErrorInfo == NULL)
	{
		::PrintConsole("No OLE DB Error Information found: hr = 0x%x\n", hr);
	}
	else
	{
		for (i = 0; i < cRecords; i++)
		{
			hr = ErrorInfo.GetAllErrorInfo(i, lcLocale, &bstrDesc, &bstrSource, &guid,
				&dwHelpContext, &bstrHelpFile);
			if (FAILED(hr))
			{
				::PrintConsole("OLE DB Error Record dump retrieval failed: hr = 0x%x\n", hr);
				return;
			}
			StringFromGUID2(guid, wszGuid, sizeof(wszGuid) / sizeof(WCHAR));
			::PrintConsole("Row #: %4d Source: \"%s\" Description: \"%s\" Help File: \"%s\" Help Context: %4d GUID: %s\n",
				i, OLE2T(bstrSource), OLE2T(bstrDesc), OLE2T(bstrHelpFile), dwHelpContext, OLE2T(wszGuid));
			bstrSource.Empty();
			bstrDesc.Empty();
			bstrHelpFile.Empty();
		}
		::PrintConsole("OLE DB Error Record dump end\n");
	}
}



//////////////////////////////////////////////////////////////////////////

char * DBF_GetDBStringName( en_db_string_where where, int n, bool bIDC ) ;
// -1 - fail, 0 - idc, 1 - extra
int DBF_IsIDC( const char * input ) ;
// fail : -1, else return group number
int DBF_GetGroupIndex( const char * input ) ;

//////////////////////////////////////////////////////////////////////////

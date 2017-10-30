//#define _WIN32_WINNT	0x500
//#define WIN32_LEAN_AND_MEAN

#include "..\stdafx.h"


//#include <windows.h>
//#include <Mmsystem.h>
//#include <stdlib.h>

#include <stdarg.h>
#include <math.h>
#include <time.h>

extern HANDLE hOut2 ;
char strconsole[256] ;
DWORD	dwWritten ;


extern CRITICAL_SECTION		g_csDebufPrint ;		// �ܼ�.
extern _j_GlobalLog * g_pSystemLog ;



//#include "DS_Packets.h"
#include "Global_Define.h"
#include "Global_Function.h"


int gf_Random( int num )
{
	if( num > 0 )
		return ( rand() % num ) ;
	else
		return 0 ;
}

unsigned short gf_Get_CurrTime( unsigned int currTick )
{
	return (unsigned short) ( ( currTick / 1000U ) % 7200U ) ;
}



int _h_Global_Time::inittime(SYSTEMTIME * curTime)
{
	// ������ : 2002 �� 1�� 1�� 0�� 0�� 0��  : �� ��¥���� ���̰��� ���� �ð����� �Ѵ�.

	//SYSTEMTIME curTime ;
	//GetLocalTime( &curTime ) ;

	//m_baseGlobal

	int	iRturnTime = ( curTime->wYear - 2003 ) * _SECOND_YEAR_ +
		( curTime->wMonth - 1 ) * _SECOND_MONTH_ +
		( curTime->wDay - 1 ) * _SECOND_DAY_ +
		( curTime->wHour ) * _SECOND_HOUR_ +
		( curTime->wMinute ) * _SECOND_MINUTE_ +
		( curTime->wSecond ) ;
	//m_baseGameTime = m_baseGlobal % _GAME_TIME_MONTH_ ;

	//m_baseLocal = timeGetTime() / 1000 ;
	return iRturnTime;
}



int _h_Global_Time::getGlobalTime()
{
	int temp = timeGetTime() / 1000 ;	// �ʴ���.
	
	return m_baseGlobal + ( temp - m_baseLocal ) ;
}

int _h_Global_Time::getGameTime()
{
	int temp = timeGetTime() / 1000 ;	// �ʴ���.

	return ( ( m_baseGameTime + temp - m_baseLocal ) % _GAME_TIME_MONTH_ ) ;
}

//short _h_Global_Time::getGameTime()
//{
//	//return 1000U ;
//	return (short) ( ( timeGetTime() >> 10 ) % 28800 ) ;
//}
//
//int _h_Time_Code::inittime()
//{
//}
//
//int _h_Time_Code::get_timecode()
//{
//}


bool _h_Global_Count::Init()
{
	int nRead = 0 ;
	
	InitializeCriticalSection( &m_cs ) ;
	if( ( m_fpCount = fopen( _COUNT_FILE_, "rb+" ) ) == NULL ) {
		printf( "Count.db �� �������� �ʽ��ϴ�.\ntest ������ filetemp�� �����Ͻʽÿ�.\n" ) ;
		return false ;
	}
	
	m_iCount = 0 ;
	
	nRead = fread( &m_iCount, sizeof(int), 1, m_fpCount ) ;
	//m_iCount = 1 ;
	//write() ;
	if( nRead == 0 ) {
		printf( "���Ͽ� �̻��� �ֽ��ϴ�. \n" ) ;
		fclose( m_fpCount ) ;
		m_fpCount = NULL ;
		return false ;
	}
	
	/*printf( "���� ������ ����ũ ���̵�� %d �Դϴ�.  \
	\n�⺻���� %d �� ���� ����� ���Դϴ�.       \
	\n�� ���� ����Ͻðڽ��ϱ�? (y/n)", m_iCount, m_iCount+128 ) ;
	
	  yes = getchar() ;
	  fflush(stdin) ;
	  
		if( yes != 'y' && yes != 'Y' ) {
		fclose( m_fpCount ) ;
		m_fpCount = NULL ;
		return false ;
}*/
	
//	m_iCount+=128 ;
	
	return true ;
}

u_int _h_Global_Count::get_uid()
{	
	write() ;
	if( m_iCount > 4294960000 )
	{
		m_iCount = 0 ;
		::PrintConsole("[MAIL] �����մϴ�. ���� ���� ������ 4294960000(42�￩��) ���� �����߽��ϴ�. \n " ) ;
		::PrintConsole("[MAIL] ���� ����̱���..  ^^;  \n " ) ;
	}

	return ++m_iCount ;
}

void _h_Global_Count::write()
{
	// ������ �Ǿ����� ����
	// ���� ����, ������ ���� �Ѵ�. ^^
	fseek( m_fpCount, 0, SEEK_SET ) ;
	fwrite( &m_iCount, sizeof(int), 1, m_fpCount ) ;
	fflush( m_fpCount ) ;
}

void _h_Global_Count::Release()
{
	if( m_fpCount ) {
		write() ;
		fclose( m_fpCount ) ;
		m_fpCount = NULL ;
	}
}

bool is_lowcase( char * acc ) {
	char * p = acc ;
	
	while( *p != 0 ) {
		if( *p >= 'A' && *p <= 'Z' )
			return false ;
		p++ ;
	}
	return true ;
}

//################ HSS Added... 2003.4.2
// ���������� �Ÿ� ���ϴ� �Լ� 
float Get_Distance( float x, float z, float gotoX, float gotoZ )
{
	float dx = gotoX - x;
	float dz = gotoZ - z;
	
	float distance = (float)sqrt( dx * dx + dz * dz );
	return distance;
}
//################ HSS Ended.. 2003.4.2

//#################################################################################
// PrintConsole : �ܼ� â�� �޼��� ����
//#################################################################################
void PrintConsole( const char *format,... )
{
	EnterCriticalSection( &g_csDebufPrint ) ;
	try{
		va_list arglist ;
		va_start(arglist, format) ;
		vsprintf(strconsole, format, arglist) ;
		va_end(arglist) ;
		
		WriteConsole( hOut2, strconsole, strlen( strconsole ), &dwWritten, NULL ) ;
		
#ifdef _CONSOLE_OUPUT_CATCH_LOG_
		// �ܼ�â�� ����� �α׷� �����..
		g_pSystemLog->LogFileWrite( strconsole ) ;
#endif
	}
	catch (...) {
		PrintConsole("[exception] %s, %d \n", __FILE__, __LINE__ ) ;
	}
	

	LeaveCriticalSection( &g_csDebufPrint ) ;
}

void ip_char(DWORD ip, char *buf)
{
	int temp ;
	char * p = buf ;
	
	for( int i = 0 ; i < 4 ; i++ )
	{
		temp = ( ip >> (i*8) ) & 0x000000ff ;
		sprintf( p, "%3u.", temp ) ;
		p += 4 ;
	}
	
	buf[15] = 0 ;
}



int GetCheckTime(){
	time_t	tempTime;
    time(&tempTime);	
	return tempTime - _SINCE_;
}



/*
 *	CString GetSocketError(int errorcode)
{
    CString strError;

    switch(errorcode)
    {
    case 1004: 
        strError = "Interrupted system call.";
    break;
    case 1009:
        strError = "Bad file number.";
    break;
    case 10013:
        strError = "Permission denied.";
    break;
    case 10014:
        strError = "Bad address.";
    break;
    case 10022:
        strError = "Invalid argument.";
    break;
    case 10024:
        strError = "Too many open files.";
    break;
    case 10035:
        strError = "Operation would block.";
    break;
    case 10036:
        strError = "Operation now in progress.";
    break;
    case 10037:
        strError = "Operation already in progress.";
    break;
    case 10038:
        strError = "Socket operation on nonsocket.";
    break;
    case 10039:
        strError = "Destination address required.";
    break;
    case 10040:
        strError = "Message too long.";
    break;
    case 10041:
        strError = "Protocol wrong type for socket.";
    break;
    case 10042:
        strError = "Protocol not available.";
    break;
    case 10043:
        strError = "Protocol not supported.";
    break;
    case 10044:
        strError = "Socket type not supported.";
    break;
    case 10045:
        strError = "Operation not supported on socket.";
    break;
    case 10046:
        strError = "Protocol family not supported.";
    break;
    case 10047:
        strError = "Address family not supported by protocol family.";
    break;
    case 10048:
        strError = "Address already in use.";
    break;
    case 10049:
        strError = "Cannot assign requested address.";
    break;
    case 10050:
        strError = "Network is down.";
    break;
    case 10051:
        strError = "Network is unreachable.";
    break;
    case 10052:
        strError = "Network dropped connection on reset.";
    break;
    case 10053:
        strError = "Software caused connection abort.";
    break;
    case 10054:
        strError = "Connection reset by peer.";
    break;
    case 10055:
        strError = "No buffer space available.";
    break;
    case 10056:
        strError = "Socket is already connected.";
    break;
    case 10057:
        strError = "Socket is not connected.";
    break;
    case 10058:
        strError = "Cannot send after socket shutdown.";
    break;
    case 10059:
        strError = "Too many references: cannot splice.";
    break;
    case 10060:
        strError = "Connection timed out.";
    break;
    case 10061:
        strError = "Connection refused.";
    break;
    case 10062:
        strError = "Too many levels of symbolic links.";
    break;
    case 10063:
        strError = "File name too long.";
    break;
    case 10064:
        strError = "Host is down.";
    break;
    case 10065:
        strError = "No route to host.";
    break;
    case 10091:
        strError = "Returned by WSAStartup(), indicating that the network subsystem is 
unusable.";
    break;
    case 10092:
        strError = "Returned by WSAStartup(), indicating that the Windows Sockets DLL 
cannot support this application.";
    break;
    case 10093:
        strError = "Winsock not initialized. ";
    break;
    case 10101:
        strError = "Disconnect.";
    break; 
    case 11001:
        strError = "Host not found.";
    break;
    case 11002:
        strError = "Nonauthoritative host not found.";
    break;
    case 11003:
        strError = "Nonrecoverable error.";
    break;
    case 11004:
        strError = "Valid name, no data record of requested type.";
    break;

    }

    return "Socket Error: " + strError;
}
 */



char * DBF_GetDBStringName( en_db_string_where where, int n, bool bIDC )
{
	static char s_cDBStringName[64] ;

	if( n > 1000 || n < 0 )
	{
		s_cDBStringName[0] = 0 ;
		return &s_cDBStringName[0] ;
	}

	switch( where )
	{
	case game_db_string:
		sprintf( s_cDBStringName, "%s_%s_%d", bIDC ? "IDC" : "EXTRA", "GAME_DB_CONNECT_STRING", n ) ;
		break ;
	case account_db_string:
		sprintf( s_cDBStringName, "%s_%s", bIDC ? "IDC" : "EXTRA", "ACCOUNT_DB_CONNECT_STRING" ) ;
		break ;
	case log_db_string:
		sprintf( s_cDBStringName, "%s_%s_%d", bIDC ? "IDC" : "EXTRA", "LOG_DB_CONNECT_STRING", n ) ;
		break ;
	case event_db_string:
		sprintf( s_cDBStringName, "%s_%s", bIDC ? "IDC" : "EXTRA", "EVENT_DB_CONNECT_STRING" ) ;
		break ;
	}

	return &s_cDBStringName[0] ;
}



int DBF_IsIDC( const char * input )
{
	char temp[6] ;
	strncpy( temp, input, 5 ) ;

	for( int i = 0 ; i < 5 ; ++i )
	{
		temp[i] = toupper(temp[i]) ;
	}

	if( strncmp( temp, "IDC", 3 ) == 0 )
		return 0 ;
	else if( strncmp( temp, "EXTRA", 5 ) == 0 )
		return 1 ;
	else
		return -1 ;
}



int DBF_GetGroupIndex( const char * input )
{
	int i ;
	int len = strlen( input ) ;
	for( i = 0 ; i < len ; ++i )
	{
		if( isdigit(input[i]) )
			break ;
	}

	if( i == len )
		return -1 ;

	return atoi( &input[i] ) ;
}


char * GetMyIP( char mode /*0:LAN, 1:WAN*/)
{
	static char m_cMyOuterip[17] = {0};		// �ܺ�
	static char m_cMyInnerip[17] = {0};		// ����

	int first_part = 0;

	DWORD dwSize = 0 ;
	GetIpAddrTable( NULL, &dwSize, FALSE );

	PMIB_IPADDRTABLE pIpAddrTable = (PMIB_IPADDRTABLE)new BYTE[dwSize] ;

	char myOutIP[16] ;

	if( pIpAddrTable ) {

		if( GetIpAddrTable( pIpAddrTable, &dwSize, FALSE ) == NO_ERROR ) {

			printf( "pIpAddrTable->dwNumEntries = %u\n", pIpAddrTable->dwNumEntries ) ;

			if( pIpAddrTable->dwNumEntries > 0 ) {

				//				char szIP[16] ;

				for( int i = 0 ; i < (int)pIpAddrTable->dwNumEntries ; i++ ) {

					in_addr ina ;
					ina.S_un.S_addr = pIpAddrTable->table[i].dwAddr ;

					strcpy( myOutIP, inet_ntoa( ina ) ) ;

					first_part = atoi( myOutIP );

					if( first_part == 0 || first_part == 127 )
						;
					else
					{
						if( first_part == 192 || first_part == 10 )
						{
							strcpy( m_cMyInnerip, myOutIP ) ;
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
	
	if( m_cMyOuterip[0] == 0 )
	{
		strcpy( m_cMyOuterip, m_cMyInnerip );
	}

	if( mode == 0 ) {
		if( m_cMyInnerip[0] == 0  )
		{
			memcpy( m_cMyInnerip, m_cMyOuterip, 17 ) ;
		}

		printf( "[IP] Select %s\n", m_cMyInnerip ); 
		g_pSystemLog->LogFileWrite( "[IP] Select %s\n", m_cMyInnerip ); 

		return m_cMyInnerip ;
	}
	else 
	{
		printf( "[IP] Select %s\n", m_cMyOuterip ); 
		g_pSystemLog->LogFileWrite( "[IP] Select %s\n", m_cMyOuterip ); 
		return m_cMyOuterip ;
	}
}
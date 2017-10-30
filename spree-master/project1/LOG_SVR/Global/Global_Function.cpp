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
	
	m_iCount+=128 ;
	
	return true ;
}

int _h_Global_Count::get_uid()
{	
	write() ;
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
	va_list arglist ;
	va_start(arglist, format) ;
	vsprintf(strconsole, format, arglist) ;
	va_end(arglist) ;
	
	WriteConsole( hOut2, strconsole, strlen( strconsole ), &dwWritten, NULL ) ;

#ifdef _CONSOLE_OUPUT_CATCH_LOG_
	// �ܼ�â�� ����� �α׷� �����..
	g_pSystemLog->LogFileWrite( strconsole ) ;
#endif

	LeaveCriticalSection( &g_csDebufPrint ) ;
}


void ip_char(DWORD ip, char *buf)
{
	unsigned int temp ;
	int i, j = 0 ;
	char v1, v2 ;
	char * p = buf ;
	
	for( i = 0 ; i < 4 ; i++ )
	{
		temp = ( ip >> (i*8) ) & 0x000000ff ;
		
		v1 = (temp / 100) % 10 ;
		if( v1 )
		{
			p[j] = v1 + '0' ;
			++j ;
		}
		temp = temp % 100 ;
		
		v2 = temp / 10 ;
		if( v1 || v2 )
		{
			p[j] = v2 + '0' ;
			++j ;
		}
		
		p[j] = temp % 10 + '0' ;
		++j ;
		
		p[j] = '.' ;
		++j ;
	}
	
	i = j-1 ;
	while( i < 16 )
	{
		p[i] = 0 ;
		++i ;
	}
}


int GetCheckTime(){
	time_t	tempTime;
    time(&tempTime);	
	return tempTime - _SINCE_;
}



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

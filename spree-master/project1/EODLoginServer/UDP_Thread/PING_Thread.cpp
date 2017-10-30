#include "..\stdafx.h"
#include "..\Global\Global_Define.h"

void PrintConsole( const char *format,... ) ;
extern bool	g_bContinue ;

DWORD WINAPI th_UDPPingThread( LPVOID lp )
{
	
	// 1. �ʱ�ȭ	
	SOCKADDR_IN fromAddr ;		// recvfrom �� ���ڷ� 
	int			frimLen = sizeof(SOCKADDR_IN) ;			// recvfrom �� ���� ����	
	char		recvbuf[1024] ;			// �ޱ� ����
	int			bufSize = sizeof(char) * 1024 ;
	int			nReturn = 0 ;
	u_short		sPort = PING_PORT ;
	int			passKey = PASS_KEY ;
	SOCKET s ;
	
	// Initailize socket	
	s = RestoreUDPSocket(sPort, 1) ;
	if( s == INVALID_SOCKET ) 
	{
		printf("Create socket failed. \n");	
		closesocket( s ) ;
		return 0;
	}
	
	while (g_bContinue) {
		// 2. Recv
		memset( recvbuf, 0, bufSize ) ;
		
		
		nReturn = recvfrom( s, recvbuf, bufSize, 0, (sockaddr*)&fromAddr, &frimLen ) ;
		if (nReturn == 0 || nReturn == SOCKET_ERROR )
		{
			Sleep(30000) ;		// �׳� �����..	
			closesocket( s ) ;
			s = RestoreUDPSocket(sPort, 0) ;
			continue ;
		}

		try {
			
			if( nReturn > 0 ) {
				
				
				// �����ϱ�..
				if( *(int*)recvbuf == passKey )
					sendto(s, recvbuf, nReturn, 0, (sockaddr*)&fromAddr, frimLen ) ;
			}
			else {
				continue ;
			}
		}
		catch ( ... ) {
			continue ;
		}
		
	}
	
	PrintConsole( "[EXIT] th_UDPPingThread Return\n " ) ;
	
	return 0 ;
}


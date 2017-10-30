#include "..\stdafx.h"
#include "..\Global\Global_Define.h"

SOCKET RestoreSocket( const unsigned short usPortNum, char mode )  ;
void PrintConsole( const char *format,... ) ;

DWORD WINAPI th_UDPPingThread( LPVOID lp )
{
	
	// 1. �ʱ�ȭ	
	SOCKADDR_IN fromAddr ;		// recvfrom �� ���ڷ� 
	int			frimLen = sizeof(SOCKADDR_IN) ;			// recvfrom �� ���� ����	
	char		recvbuf[2048] ;			// �ޱ� ����
	int			bufSize = sizeof(recvbuf)  ;
	int			nReturn = 0 ;
	u_short		sPort = (unsigned short)lp ;
	int			passKey = PASS_KEY ;
	SOCKET s ;
	
	// Initailize socket	
	s = RestoreSocket(sPort, 0) ;
	if( s == INVALID_SOCKET ) 
	{
		printf("[ERROR] Ping - UDP Socket failed : %d\n", WSAGetLastError() ); 
		return 0;
	}
	
	while (1) {
		// 2. Recv
				
		try{
			nReturn = recvfrom( s, recvbuf, bufSize, 0, (sockaddr*)&fromAddr, &frimLen ) ;
			if (nReturn == 0)
			{
				PrintConsole("[SO_ERR] UDP Recv failed / ���� ���� ����\n");
//				g_pErrorMsg->ErrMsg("[SO_ERR] UDP Recv failed / ���� ���� ����\n");
				if( s == INVALID_SOCKET ) 
				{
					printf("������ �����߽��ϴ�. \n");
					closesocket( s ) ;
					s = RestoreSocket(sPort, 0) ;
					Sleep(1000) ;		// �׳� �����..	
				}						
				continue ;
			}
			else if (nReturn == SOCKET_ERROR)
			{			
				nReturn = WSAGetLastError() ;
				if( nReturn == WSAETIMEDOUT )
				{
					continue ;
				}
				PrintConsole("[SO_ERR] UDP Recv failed / Recv ����. Error No.%d\n", WSAGetLastError());
				//g_pErrorMsg->ErrMsg("[SO_ERR] UDP Recv failed / Recv ����. Error No.%d\n", WSAGetLastError());
				if( s == INVALID_SOCKET ) 
				{
					printf("������ �����߽��ϴ�. \n");
					closesocket( s ) ;
					s = RestoreSocket(sPort, 0) ;
				}			
				Sleep(1000) ;		// �׳� �����..			
				continue ;
			}
			else if( nReturn > 0 ) {

				// �����ϱ�..
				if( *(int*)recvbuf == passKey )
					sendto(s, recvbuf, nReturn, 0, (sockaddr*)&fromAddr, frimLen ) ;
			}
			else {
				continue ;
			}

		}
		catch (...) {
			::PrintConsole( "[EXCEPTION] thread exception \n" ) ;
			Sleep(50) ;	// �������� ���� ������ ���� �� �����ϱ�.. 
		}		
		
	}
		
	PrintConsole( "[EXIT] th_ServerCom Return\n " ) ;
	
	return 0 ;
}


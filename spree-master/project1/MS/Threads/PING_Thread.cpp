#include "..\stdafx.h"
#include "..\Global\Global_Define.h"
#include "..\Global\MSPacket_Msg.h"
#include "..\VMManager\VMManager.h"
#include "..\Queue\Packet_Queue.h"
#include "..\Map\MapManager.h"
#include "..\OLE-DB\db_command.h"



extern CDB_Connector	*	g_pDBCon ;
extern _j_BackupReady * g_pBackupReady ; 
extern _j_GlobalLog * g_pSystemLog ;
extern _j_MemeberQueue * g_pMemberQueue ;
extern _j_queue_UDP_Packet * g_pUDPPacket ;
extern bool g_pThreadRunning ;
extern bool g_pMainThreadRunning ;
extern CMYTrace *			g_pTrace ;

SOCKET RestoreUDPSocket( SOCKADDR_IN * addr )
{
	SOCKET s  = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ;
	if( s == INVALID_SOCKET ) {
		printf("[SO_ERR] INVALID_SOCKET. \n");
		return INVALID_SOCKET;
	}

	DWORD dwBytesReturned = 0;
	BOOL bNewBehavior = FALSE;
	DWORD status;

	// disable  new behavior using
	// IOCTL: SIO_UDP_CONNRESET
	status = WSAIoctl(s, SIO_UDP_CONNRESET,
		&bNewBehavior, sizeof(bNewBehavior),
		NULL, 0, &dwBytesReturned,
		NULL, NULL);
	
	if (SOCKET_ERROR == status)
	{
		DWORD dwErr = WSAGetLastError();
		if (WSAEWOULDBLOCK != dwErr)
		{
			printf("WSAIoctl(SIO_UDP_CONNRESET) Error: %d\n", dwErr);
			closesocket( s );
			s = INVALID_SOCKET;
			return s;
		}
	}
	
	if( bind( s, (SOCKADDR *)addr, sizeof( SOCKADDR_IN ) ) == SOCKET_ERROR ) {
		printf( "Error : ServerComThread socket bind %d, port = %u\n", WSAGetLastError() );
		closesocket( s );
		s = INVALID_SOCKET;
	}

	return s;
}


DWORD WINAPI th_UDPRecvThread( LPVOID lp )
{

		
	// 1. �ʱ�ȭ	
	SOCKADDR_IN fromAddr ;		// recvfrom �� ���ڷ� 
	int			frimLen = sizeof(SOCKADDR_IN) ;			// recvfrom �� ���� ����	
	char		recvbuf[1024] ;			// �ޱ� ����
	int			bufSize = sizeof(char) * 1024 ;
	int			nReturn = 0 ;
	u_short		sPort = (unsigned short)lp ;
	int			passKey = _SERVER_AUTHOR_ ;
	SOCKET s ;

	DWORD dwBytesReturned = 0;
	BOOL bNewBehavior = FALSE;
	
	// Initailize socket		
	fromAddr.sin_family = AF_INET ;
	fromAddr.sin_port = htons( sPort ) ;
	fromAddr.sin_addr.s_addr = htonl( INADDR_ANY ) ;

	while( 1 )
	{
		s = RestoreUDPSocket( &fromAddr );
		if( s == INVALID_SOCKET ) 
		{
			Sleep( 30000 );
			continue;
		}
		
		while (g_pThreadRunning) {
			// 2. Recv
			ZeroMemory( recvbuf, sizeof(recvbuf) );

			nReturn = recvfrom( s, recvbuf, bufSize, 0, (sockaddr*)&fromAddr, &frimLen ) ;

			if (nReturn == 0)
			{
				g_pSystemLog->LogFileWrite("[SO_ERR] UDP Recv Thread - recvfrom return zero.\n");

				closesocket( s );
				Sleep(30000);
				continue; 

			}
			else if (nReturn == SOCKET_ERROR)
			{
				g_pSystemLog->LogFileWrite("[SO_ERR] UDP Recv Error(%d)\n", WSAGetLastError());

				closesocket( s );
				Sleep(30000);
				continue ;
			}
			
			try {
				
				if( nReturn > 0 ) {
					
					// ��ȣ�� Ʋ���� ƨ��..
					if( *(int*)recvbuf != passKey ) {
						continue ;
					}
					
					if(  recvbuf[4] == 0 ){	// 0 ���� Ping �̴�.
						// �����ϱ�..
						sendto(s, recvbuf, nReturn, 0, (sockaddr*)&fromAddr, frimLen ) ;
					}
					else {
						// ť�� �ְ� ��ũ�����忡 �ñ���.					
						g_pUDPPacket->insert( &fromAddr, recvbuf, nReturn ) ;					
					}
				}
				else {
					continue ;
				}
			}
			catch ( ... ) {
				::PrintConsole("[EXCEPTION] Recv thread work \n") ;
				continue ;
			}
			
		}
	}
	
	PrintConsole( "[EXIT] th_UDPRecvThread Return\n " ) ;
	
	return 0 ;
}


DWORD WINAPI th_UDPWorkThread( LPVOID lp )
{

	////////////////////////////////////////////////////////////////
	
	_Mail				mail = {0} ;
	
	_FriendInfo * pMyInfo ;
	_FriendInfo * pFriendInfo ;
	list<_Member> * pFriendList ;
	list<_Member>::iterator itr ;
	list<_Member>::iterator frditr ;
	
	
	MMSG_CUT_FRIEND_REPORT msgCutFriendReport = {0} ;
	msgCutFriendReport.sLength = sizeof(MMSG_CUT_FRIEND_REPORT) ;
	msgCutFriendReport.sCrypto = false ;
	msgCutFriendReport.sCompressed = false ;
	msgCutFriendReport.cMessage = MMSG_NO_CUT_FRIEND_REPORT_S_C ;
	

	COLE_DB_Executer *		g_pDBCommand ;
	g_pDBCommand = new COLE_DB_Executer(g_pDBCon ) ;
	assert(g_pDBCommand) ;

	_packetUDP * packet = NULL ;

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;	
	LARGE_INTEGER li = {0, } ;
	SetWaitableTimer( hTimer, &li, 10, NULL, NULL, TRUE ) ;

	int pos ;
	

	while (g_pThreadRunning) {


		packet = g_pUDPPacket->get() ;


		if( packet == NULL )
		{
			WaitForSingleObject( hTimer, INFINITE ) ;
			continue ;
		}


		try{
			if( packet->buf[4] == UMSG_CHARAC_DELETE_DS_MS_NUM )
			{
				pos = 1 ;

				UMSG_CHARAC_DELETE_DS_MS * pMsgCharacDelete ;
				pMsgCharacDelete = (UMSG_CHARAC_DELETE_DS_MS*)packet->buf ;
				
#ifdef _TRACE_
				g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recv 'UMSG_CHARAC_DELETE_DS_MS_NUM' (chr:%s, uid:%d) \n "
					, pMsgCharacDelete->cCharacName, pMsgCharacDelete->iUid ) ;
#endif


				pMyInfo = NULL ;
				pFriendInfo = NULL ;
				pFriendList = NULL ;
				
				pMyInfo = g_pMemberQueue->FindNode( pMsgCharacDelete->cCharacName ) ;
				

				pos = 2 ;

				if( pMyInfo == NULL ) {
				
					pMyInfo = g_pMemberQueue->GetNode( pMsgCharacDelete->cCharacName ) ;
				

					if ( !pMyInfo ) 
					{
						continue ;
					}
					else{

						// ������ ĳ���ʹ� DB���� �̸��� �ٲ�����Ƿ� uid �� Ű������ �о�;� �Ѵ�..
						if( g_pDBCommand->GetMyDataByUID( pMyInfo, pMsgCharacDelete->iUid ) == false )
						{
							g_pMemberQueue->PutNode( pMyInfo ) ;							
							::PrintConsole( "[ERROR] g_pDBCommand->GetMyDataByUID (%s) \n", pMsgCharacDelete->cCharacName ) ;
							continue ;
						}
					}
				}			

				pos = 3 ;
				 
				pMyInfo->DeleteCharacter( g_pDBCommand ) ;
				g_pMemberQueue->PutNode( pMyInfo ) ;
				

				pos = 9 ;
						
#ifdef _TRACE_
				g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Erase Charac(%s) data \n", pMsgCharacDelete->cCharacName ) ;
#endif	
				
				
			}
			else if( UMSG_GM_CHARAC_SET_DS_MS_NUM == packet->buf[4] )
			{
				UMSG_GM_CHARAC_SET_DS_MS * pMsgGmCharacSet = (UMSG_GM_CHARAC_SET_DS_MS*) packet->buf ;
				pMyInfo = g_pMemberQueue->FindNode( pMsgGmCharacSet->cCharacName ) ;
				if( pMyInfo )
				{
					pMyInfo->SetGmClass( pMsgGmCharacSet->cClass ) ;
				}
			}

			else if( UMSG_CHARAC_OR_KICK_NUM == packet->buf[4] )
			{
				UMSG_CHARAC_OR_KICK * pMsgCharacOrKick = (UMSG_CHARAC_OR_KICK*) packet->buf ;

				bool bRes ;
				// �ʿ� ģ���� ����Ÿ�� ����.. DB�� ���� �ܾ�;��Ѵ�..					
				_FriendInfo * pFriendInfo = g_pMemberQueue->FindNode( pMsgCharacOrKick->cCharacName ) ;
				if( pFriendInfo == NULL ) {						
					pFriendInfo = g_pMemberQueue->GetNode( pMsgCharacOrKick->cCharacName ) ;				
					
					if( pFriendInfo ) {							
						bRes = pFriendInfo->ReadFromDB( g_pDBCommand ) ;
						
						if( bRes == false ) {								
							g_pMemberQueue->PutNode( pFriendInfo ) ;
							continue ;
						}
					}
					else {
						continue ;
					}


						
					if( pFriendInfo->GetMailCurCount() < MAX_MAIL_COUNT ) {
						// ���� �ε��� ��ȣ�� ���´�.							
						u_int uiMailIndex = 0 ;
						char cFromName[13] = {"[������]"} ;
						char cText[101] = {0} ;
						sprintf( cText, "%s", pMsgCharacOrKick->cORName ) ;
						
						
						// ���Ŀ� �α��� ������ �������� �˷���� �Ѵ�..
						// �� ���� ��������..

						// �Ʒ� �� 20080201 _PRIVATE ���� _SYSTEM_MSG_OR_KICK ���� ����
						//if( g_pDBCommand->SendMail( &uiMailIndex, cFromName, cText, strlen( cText ), _MailInfo::_PRIVATE ) )
						if( g_pDBCommand->SendMail( &uiMailIndex, cFromName, cText, strlen( cText ), _MailInfo::_SYSTEM_MSG_OR_KICK ) )
						{
							// �ش� ģ���� ���� ����Ʈ�� �ε����� �־���� �Ѵ�.

							_Mail mail = {0} ;
							mail.uiIndex = uiMailIndex ;
							mail.check = false ;							
							
							pFriendInfo->AddMail( mail ) ;							
							pFriendInfo->_BackupState.bMailListBackup = BACKUP_READY ;
							g_pBackupReady->Insert( pFriendInfo ) ;
							
						}						
						
					}
					
					
				}
				
			}

			else if( MSG_SYSTEM_OFF_REQ_NUM == packet->buf[4] )
			{
				MSG_SYSTEM_OFF_REQ * pMsgSystemOff = (MSG_SYSTEM_OFF_REQ*) packet->buf ;
				if( pMsgSystemOff->cServer_num == -1 )
				{
					// �ý��� �����϶�..
					::PrintConsole( "[AUTO SHUTDOWN] GMS ��û�� ���� �ڵ� ����˴ϴ�. \n") ;
					g_pMainThreadRunning  = false ;
				}
			}


		}
		catch(...){
			char logbuf[128] = {0} ;
			sprintf(logbuf,"[EXCEPTION] th_UDPWorkThread (pos.%d) \n", pos) ;
			::PrintConsole(logbuf) ;
		}
	} // while

	delete g_pDBCommand ;
	printf("[EXIT] th_UDPWorkThread Exit \n" ) ;
	return 0 ;

}



DWORD WINAPI th_PingThread( LPVOID lp )
{

		
	// 1. �ʱ�ȭ	
	SOCKADDR_IN fromAddr ;		// recvfrom �� ���ڷ� 
	int			frimLen = sizeof(SOCKADDR_IN) ;			// recvfrom �� ���� ����	
	char		recvbuf[1024] ;			// �ޱ� ����
	int			bufSize = sizeof(char) * 1024 ;
	int			nReturn = 0 ;
	u_short		sPort = (unsigned short)lp ;
	int			passKey = _SERVER_AUTHOR_ ;
	SOCKET s ;

	DWORD dwBytesReturned = 0;
	BOOL bNewBehavior = FALSE;
	
	// Initailize socket		
	fromAddr.sin_family = AF_INET ;
	fromAddr.sin_port = htons( sPort ) ;
	fromAddr.sin_addr.s_addr = htonl( INADDR_ANY ) ;

	while( 1 )
	{
		s = RestoreUDPSocket( &fromAddr );
		if( s == INVALID_SOCKET ) 
		{
			Sleep( 30000 );
			continue;
		}
		
		while (g_pThreadRunning) {
			// 2. Recv
			ZeroMemory( recvbuf, sizeof(recvbuf) );

			nReturn = recvfrom( s, recvbuf, bufSize, 0, (sockaddr*)&fromAddr, &frimLen ) ;

			if (nReturn == 0)
			{
				g_pSystemLog->LogFileWrite("[SO_ERR] UDP Ping Thread - recvfrom return zero.\n");

				closesocket( s );
				Sleep(30000);
				continue; 

			}
			else if (nReturn == SOCKET_ERROR)
			{
				g_pSystemLog->LogFileWrite("[SO_ERR] UDP Ping Error(%d)\n", WSAGetLastError());

				closesocket( s );
				Sleep(30000);
				continue ;
			}
			
			try {
				
				if( nReturn > 0 ) {
					
					// ��ȣ�� Ʋ���� ƨ��..
					if( *(int*)recvbuf != passKey ) {
						continue ;
					}
					
					if(  recvbuf[4] == 0 ){	// 0 ���� Ping �̴�.
						// �����ϱ�..
						sendto(s, recvbuf, nReturn, 0, (sockaddr*)&fromAddr, frimLen ) ;
					}
				}
				else {
					continue ;
				}
			}
			catch ( ... ) {
				::PrintConsole("[EXCEPTION] Ping thread work \n") ;
				continue ;
			}
		}
	}
	
	PrintConsole( "[EXIT] th_UDPPingThread Return\n " ) ;
	
	return 0 ;
}
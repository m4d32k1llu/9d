#include "..\stdafx.h"
#include "..\VMManager\VMManager.h"
#include "..\Packet_Queue\Packet_Queue.h"
#include "..\Global\Global_Define.h"
#include "..\Ole-DB\db_command.h"
//#include "..\DS_Com\DS_Com.h"
#include "..\Timer\TimeOut.h"



extern _j_queue_UDP_Packet *	g_pUDPSendPacketQ ;
extern _j_queue_UDP_Packet *	g_pUDPRecvPacketQ ;
extern _j_UserQueue *		g_pUserQueue  ;
extern _h_ServerInfo *		g_pServerInfo ; 
extern CDB_Connector	*	g_pDBCon ;

extern int nHeaderSize ;
extern bool	g_bContinue ;

const unsigned short usRecvPort = UDP_RECV_PORT ;					// send port number
const unsigned short usSendPort = UDP_SEND_PORT ;
SOCKET	SendSock = INVALID_SOCKET ;
SOCKET	RecvSock = INVALID_SOCKET ;

bool g_bDebugGroup[5] = {
	false,		// UDP �޼���
	false,		// LoginUDP
	false,
	false,
	false
} ;

// Send queue �� ��� �ִ� ��Ŷ�� ������ ������ send �Ѵ�..
DWORD WINAPI th_UDPSendThread( LPVOID lp )
{	
	// �ʱ�ȭ
	_packetUDP	* packet;	
	SOCKADDR_IN * toAddr ;										// Send addr pointer
	int			toLen = sizeof(SOCKADDR_IN) ;					// send addr len

	int			nReturn ;									// send result				
	char	*	sendBuf ;										// Send buffer pointer	
	int			sendSize ;									// send data size
	
	
	// Initailize socket	
	if( SendSock == INVALID_SOCKET ) 
	{		
		SendSock = RestoreUDPSocket( usSendPort, 0 ) ;
	}

	// loop
	while (g_bContinue) 
	{
		// 1. Check queue		
		if( g_pUDPSendPacketQ->empty() ) 
		{			
			Sleep(10) ;
			continue ;
		}
		else 
		{

			try{
				// 2. Get packet at the queue
				packet = g_pUDPSendPacketQ->get() ;								// ��Ŷ�� ���				
				if(packet == NULL) 
				{
					Sleep(10) ;
					continue;
				}

			}
			catch (...) 
			{
				::PrintConsole("[EXCEPTION] g_pPacketQ->get()\n ") ;
				g_pErrMsg->LinePrint( HTML("[EXCEPTION] g_pPacketQ->get()\n ", _ns_trace_::_FONT_COLOR_RED, _BLINK_) );
				continue;
			}		

		}		

		// 3. send packet 
		sendSize = packet->sBufSize ;
		if( sendSize > sizeof(_packetUDP ) )
		{
			continue ;
		}
		sendBuf  = packet->buf ;
		toAddr = &packet->addr ;


		try{
			// �������� ����Ÿ ��� ��û
			nReturn =sendto(SendSock, sendBuf, sendSize, 0, (sockaddr*)toAddr, toLen);
			Sleep( 5 ) ;
		}
		catch (...) {
			continue ;
		}


		if (nReturn == 0)
		{
			PrintConsole("[SO_ERR] UDP send failed / ���� ���� ����\n");
			//closesocket( SendSock ) ;
			//SendSock = RestoreUDPSocket( usSendPort, 0 ) ;
			continue ;
		}
		else if (nReturn == SOCKET_ERROR)
		{		
			PrintConsole("[SO_ERR] UDP send failed / send ����. Error No.%d / ip:%s, port:%d \n"
				, WSAGetLastError(), inet_ntoa( toAddr->sin_addr ), toAddr->sin_port ) ;
			//closesocket( SendSock ) ;
			//SendSock = RestoreUDPSocket( usSendPort, 0 ) ;
			continue ;
		}
		else if (nReturn != sendSize)
		{		
			PrintConsole("[SO_ERR] UDP send failed / ���������� ����. Error No.%d\n", WSAGetLastError());
			//closesocket( SendSock ) ;
			//SendSock = RestoreUDPSocket( usSendPort, 0 ) ;
			continue ;
		}
		continue ;

	} //while

	::PrintConsole( "[EXIT] th_UDPSendThread Return\n " ) ;
	return 0 ;
}

// recv ���� ����Ÿ�� Recv queue �� �ִ´�.. (����Ÿ ó�� �۾��� �۾�������(th_UDPRecvPacketWorkThread)�� �Ѵ�.)
DWORD WINAPI th_UDPRecvThread( LPVOID lp )
{	

	// 1. �ʱ�ȭ	
	SOCKADDR_IN fromAddr ;								// recvfrom �� ���ڷ� 
	int			frimLen = sizeof(SOCKADDR_IN) ;			// recvfrom �� ���� ����	
	char 		recvbuf[_MAX_UDP_PACKET_SIZE] ;								// �ޱ� ����
	const int	bufSize = sizeof(char) * _MAX_UDP_PACKET_SIZE ;
	int			nReturn ;




	static SOCKET		s ;								// recv socket

	// Initailize socket	
	if( RecvSock == INVALID_SOCKET ) 
	{	
		RecvSock = RestoreUDPSocket( usRecvPort, 0 ) ;
	}



	while (g_bContinue) {
		// 2. Recv	
		Sleep(5) ;
		nReturn = recvfrom( RecvSock, recvbuf, bufSize, 0, (SOCKADDR*)&fromAddr, &frimLen ) ;

		if (nReturn == 0)
		{
			PrintConsole("[SO_ERR] UDP Recv failed / ���� ���� ����\n");
			if( s == INVALID_SOCKET ) 
			{	
				closesocket( RecvSock ) ;
				RecvSock = RestoreUDPSocket(usRecvPort, 0) ;
			}
			
			continue ;
		}
		else if (nReturn == SOCKET_ERROR)
		{			
			PrintConsole("[SO_ERR] UDP Recv failed / Recv ����. Error No.%d\n", WSAGetLastError());			
			if( RecvSock == INVALID_SOCKET ) 
			{			
				closesocket( RecvSock ) ;
				RecvSock = RestoreUDPSocket(usRecvPort, 0) ;
			}			
			Sleep(1000) ;		// �׳� �����..			
			continue ;
		}

		try {
			if( nReturn > 0 ) {
				g_pUDPRecvPacketQ->insert( &fromAddr, recvbuf, nReturn ) ;
			}			
		}
		catch ( ... ) {
			::PrintConsole("[Exception] g_pUDPRecvPacketQ->insert \n") ;
			g_pErrMsg->LinePrint( HTML("[Exception] g_pUDPRecvPacketQ->insert \n", _ns_trace_::_FONT_COLOR_RED, _BLINK_) ) ;

		}

	}	// while

	PrintConsole( "[EXIT] th_UDPRecvThread Return\n " ) ;

	return 0 ;
}



DWORD WINAPI th_UDPRecvPacketWorkThread( LPVOID lp )
{
	// �ʱ�ȭ
	_packetUDP	*		packet;
	SOCKADDR_IN	*		fromAddr;
	const int			fromLen = sizeof(SOCKADDR_IN);

	char *				pSendBuf ;
	char *				buf ;

	char				gsip[17] ;
	char				msip[17] ;
	
	int ret ;
	_h_User * pUser ;

	COLE_DB_Executer * pDBExecutor = new COLE_DB_Executer(g_pDBCon) ;
	if( pDBExecutor == NULL  )
	{
		return 0 ;
	}


	MSG_LOGIN_SUCCESS msgLoginSuccess = {0} ;
	msgLoginSuccess.Header.sLength = sizeof(MSG_LOGIN_SUCCESS) ;
	msgLoginSuccess.cMessage = MSG_NO_LOGIN_SUCCESS ;
		

	MSG_LOGIN_FAIL	msgLoginFail = {0};
	msgLoginFail.Header.sLength = sizeof(MSG_LOGIN_FAIL);
	msgLoginFail.cMessage = MSG_NO_LOGIN_FAIL;	


	
	while (g_bContinue) 
	{

// 1. ť �˻�

		if( g_pUDPRecvPacketQ->empty() )
		{
			Sleep(20) ;
			continue ;
		}
		else
		{

			try{
// 2. ť���� ��������
				packet = g_pUDPRecvPacketQ->get() ;								// ��Ŷ�� ���				
				if(packet == NULL) 
				{
					Sleep(20) ;
					continue;
				}

			}
			catch (...) {
				PrintConsole("[EXCEPTION] g_pUDPRecvPacketQ->get()\n ");
				g_pErrMsg->LinePrint( HTML("[EXCEPTION] g_pUDPRecvPacketQ->get()\n ", _ns_trace_::_FONT_COLOR_RED, _BLINK_ ) );
				continue;
			}	

// 3. ��Ŷ ó��			
			buf = packet->buf;
			fromAddr = &packet->addr ;
			pUser = NULL ;

			if( buf[4] == UMSG_LOGIN_DS_LS_RESULT_NUM )
			{				
				
				UMSG_LOGIN_DS_LS_RESULT * pMsgLoginResult = (UMSG_LOGIN_DS_LS_RESULT*)packet->buf ;
#ifdef _TRACE_
				g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "[Recv] Recv UDP UMSG_LOGIN_DS_LS_RESULT_NUM (acc:%s)\n", pMsgLoginResult->cAccount ) ;
#endif
								
				if( pMsgLoginResult->usUid == 0 ) {		// uid �� 0�� ���� �����ִ�.
					::PrintConsole( "[ERROR] temp uid = %d \n", pMsgLoginResult->usUid ) ;
					continue ;
				}
				pUser = g_pUserQueue->FindUser( pMsgLoginResult->iTempID ) ;
				if( pUser == NULL )
				{
					continue ;
				}

				
				if( pMsgLoginResult->cServerNo > 0 ) // ���� �㰡
				{					
					// �α��� ����.					
					msgLoginSuccess.ulServerIP = g_pServerInfo->GetGSAcceptIP( pMsgLoginResult->cForestNo, pMsgLoginResult->cServerNo ) ;
					msgLoginSuccess.usServerPort = g_pServerInfo->GetGSAcdeptPort( pMsgLoginResult->cForestNo, pMsgLoginResult->cServerNo ) ;
					
					msgLoginSuccess.ulMessengerIP = g_pServerInfo->GetMessengerIP(pMsgLoginResult->cForestNo) ;
					msgLoginSuccess.usMessengerPort = g_pServerInfo->GetMessengerPort(pMsgLoginResult->cForestNo) ;
					
					msgLoginSuccess.usUniqueID = pMsgLoginResult->usUid ;	


					ip_char(msgLoginSuccess.ulServerIP, gsip) ;
					ip_char(msgLoginSuccess.ulMessengerIP, msip ) ;

					strncpy( msgLoginSuccess.cNDAccount, pMsgLoginResult->cAccount, en_max_lil+1 ) ;

					g_pTrace->OutputString( _TRACE_CLASS_LOGIN_SUCCESS, "[PASS] <%s> (GS ip = %s, port = %u, uid = %d \n        MS ip = %s, port = %u  )\n",																	
						pMsgLoginResult->cAccount, gsip , msgLoginSuccess.usServerPort 
						, pMsgLoginResult->usUid, msip, msgLoginSuccess.usMessengerPort ) ;
					
					Logout( "[PASS] <%s> (GS ip = %s, port = %u, uid = %d \n        MS ip = %s, port = %u  )\n",																	
						pMsgLoginResult->cAccount, gsip , msgLoginSuccess.usServerPort 
						, pMsgLoginResult->usUid, msip, msgLoginSuccess.usMessengerPort ) ;
					
					
					// ��ȣȭ					
					pSendBuf = (char *)&msgLoginSuccess ;
					/*
					if( !g_Crypto.Encryption( &pSendBuf[nHeaderSize], sizeof(MSG_LOGIN_SUCCESS) - nHeaderSize ) ){
						PrintConsole("[ERROR] MSG_LOGIN_SUCCESS msg Encryption failed \n ") ;
						
						
						msgLoginFail.ucErrorCode = _LOGIN_ERROR_CODE_ETC ;				
						ret = pUser->m_pNetIO->SendRequest((char*)&msgLoginFail, sizeof(MSG_LOGIN_FAIL)) ;				
						if( ret != 0 ) {						
							::PrintConsole("[ERROR] Send Request failed (%d)\n", WSAGetLastError()) ;
						}
						
						pUser->DisConnect() ;
						continue ;
						
					}
					*/
					
					
					
					if( pUser->m_pNetIO )
					{
						ret = pUser->m_pNetIO->SendRequest((char*)&msgLoginSuccess, sizeof(MSG_LOGIN_SUCCESS)) ;
						{
							if( ret != 0 ) 
							{				
								::PrintConsole("[ERROR] Send Request failed (%d)\n", WSAGetLastError()) ;						
							}			
						}
					}				
					// �α��� ����..
					
				}
				else
				{						

					switch( pMsgLoginResult->cServerNo ) {
					case 0:
						g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[LOGIN FALED] ID = <%s> : Message : pMsgLoginResult->cServerNo == 0 \n", pUser->cID ) ;
						msgLoginFail.ucErrorCode = _LOGIN_ERROR_CODE_SERVERISFULL ;
						break;
					case _ERRNO_IS_FULL_:
						// ���� �ο� �ʰ�..					
						g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[LOGIN FALED] ID = <%s> : Message : _ERRNO_IS_FULL_ \n", pUser->cID ) ;
						msgLoginFail.ucErrorCode = _LOGIN_ERROR_CODE_SERVERISFULL ;
						break;
					case _ERRNO_USING_ACCOUNT_:						
						g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[LOGIN FALED] ID = <%s> : Message : _ERRNO_USING_ACCOUNT_\n", pUser->cID ) ;						
						msgLoginFail.ucErrorCode = _LOGIN_ERROR_CODE_ALREADY ;						
						break;
					case _ERRNO_DB_ERROR_:
						g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[LOGIN FALED] ID = <%s> : Message : _ERRNO_DB_ERROR_ \n", pUser->cID ) ;						
						msgLoginFail.ucErrorCode = _LOGIN_ERROR_CODE_ETC ;						
						break;
					case _ERRNO_STATE_ERROR_:
						g_pTrace->OutputString( _TRACE_CLASS_SYSTEM, "[LOGIN FALED] ID = <%s> : Message : _ERRNO_STATE_ERROR_\n", pUser->cID ) ;						
						msgLoginFail.ucErrorCode = _LOGIN_ERROR_CODE_ETC ;	
						// 20070126 : ������ �õ��� ����, ���� ���¸� �ǵ��� ���´�.
						pDBExecutor->DSLogout( pUser->cID );
						break;
					}

					if( pUser->m_pNetIO )
					{
						ret = pUser->m_pNetIO->SendRequest((char*)&msgLoginFail, sizeof(MSG_LOGIN_FAIL)) ;				

						if( ret != 0 ) {						
							::PrintConsole("[ERROR] Send Request failed (%d)\n", WSAGetLastError()) ;						
						}				
					}
				}	
				
				pUser->DisConnect() ;

			}			
			// ���� ���� ��û�� ���� ����
			else if ( buf[4] == UMSG_LOGIN_INFO_RES_DS_LS_NUM ) {
				UMSG_LOGIN_INFO_RES_DS_LS * pInfoMsg = (UMSG_LOGIN_INFO_RES_DS_LS *)buf ;

				IF_DSUDPDEBUG
					::PrintConsole( "[DS UDP RECV] Forest %d :  State %d\n", pInfoMsg->cForestNum, pInfoMsg->cForestState ) ;

				g_pServerInfo->SetForestState(pInfoMsg->cForestState, pInfoMsg->cForestNum ) ;
				// üũ �÷��׸� 0���� �ǵ��� ���´�..
				g_pServerInfo->m_ForestCheckFlag[pInfoMsg->cForestNum] = 0 ;			

				// DB�� ������Ʈ �Ѵ�.
				pDBExecutor->ServerStatusUpdate( pInfoMsg->cForestNum, pInfoMsg->cForestState ) ;
				
			}
		} 	
	} // while

	pDBExecutor->ServerStatusUpdate( -1, 0 ) ;


	return 0 ;
	
} // thread end


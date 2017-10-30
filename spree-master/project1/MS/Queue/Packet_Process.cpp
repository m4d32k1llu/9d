#include "..\stdafx.h"

#include "..\Global\Global_Define.h"
#include "..\Network\Network.h"
#include "..\VMManager\VMManager.h"
#include "..\Queue\Packet_Queue.h"
#include "..\Map\MapManager.h"
#include "..\Global\Global_LogFile.h"
#include "..\OLE-DB\db_command.h"



// Packet ó�� Functions
bool GSConnect(char cServerNum, _h_DS_Com * dsCom) ;
bool CreateParty(char cServerNum, short uid1, short uid2) ;




extern _queue_Packet * g_pPacketQueue ;
extern _queue_Packet * g_pChatPacketQueue ;
extern _queue_Packet * g_pLoginPacketQueue ;

extern _j_GlobalLog * g_pSystemLog ;
#ifdef _LOGIN_FILE_LOG_WRITE_
extern _j_GlobalLog *		g_pLoginLog ;
#endif
extern _j_MemeberQueue * g_pMemberQueue ;
extern _j_BackupReady * g_pBackupReady ;
extern _j_ComQueue * g_pComQueue ;
extern bool g_pThreadRunning ;
extern CDB_Connector	*	g_pDBCon ;
extern CMYTrace *			g_pTrace ;

extern _j_Templet_Timer * g_pTimeOutTimer ;



DWORD	WINAPI	th_WorkThread( LPVOID lp)
{

	// ole-db	
	COLE_DB_Executer *	g_pDBCommand = new COLE_DB_Executer(g_pDBCon) ;
	if(g_pDBCommand == NULL )
	{	
		g_pSystemLog->LogFileWrite( "th_WorkThread _j_DB_Command Initalize failed\n" ) ;
		return 0 ;
	}	

#ifdef _DEBUG
	//_FriendInfo * f = g_pMemberQueue->GetNode("ûȭ��") ;
	//_FriendInfo  f ;
	//g_pDBCommand->GetMyData( f ) ;
	//g_pDBCommand->UpdateMyData( f ) ;
	//g_pDBCommand->GetMyDataByUID(&f, 29) ;
#endif	

	
	int			Mainmsg = 0 ;	
	_packet *	pack = NULL ;	
	_h_DS_Com * pDSComm = NULL ;
	_Member		member = {0} ;	
	_FriendInfo * pMyInfo = NULL ;	
	_FriendInfo * pFriendInfo = NULL ;	
	_FriendInfo * pTemp = NULL ;
	list<_Member> * pMyFriendList = NULL ;	
	list<_Member>::iterator memberListIter ;

	list<_Mail> * pMyMailList = NULL ;
	list<_Mail>::iterator mailListIter ;

	list<_Mail> * g_pMailList = new list<_Mail>;

	int			iRes ;
	bool		bRes  ;
	_Mail		mail = {0} ;
	

	// loop ��..
	int i = 0, j = 0 ;	

	// Result MSG Initalize
	MMSG_MAKE_FRIEND_RES	msgMakeFriendRes = {0} ;
	msgMakeFriendRes.sLength = sizeof(MMSG_MAKE_FRIEND_RES) ;
	msgMakeFriendRes.sCrypto = false;
	msgMakeFriendRes.sCompressed = false ;
	msgMakeFriendRes.cMessage = MMSG_NO_MAKE_FRIEND_RES_S_C ;

	MMSG_FRIEND_LIST_RES	msgFriendListRes = {0} ;
	//msgFriendListRes.sLength = sizeof(MMSG_FRIEND_LIST_RES) ; <-��������..
	msgFriendListRes.sCrypto = false ;
	msgFriendListRes.sCompressed = false ;
	msgFriendListRes.cMessage = MMSG_NO_FRIEND_LIST_RES_S_C ;	

	MMSG_FRIEND_STATE_REPORT msgFriendStateReport = {0} ;
	msgFriendStateReport.sLength = sizeof(MMSG_FRIEND_STATE_REPORT) ;
	msgFriendStateReport.sCrypto = false ;
	msgFriendStateReport.sCompressed = false ;
	msgFriendStateReport.cMessage = MMSG_NO_FRIEND_STATE_REPORT_S_C ;

	MMSG_MAKE_GROUP_RES msgMakeGroupRes = {0} ;
	msgMakeGroupRes.sLength = sizeof(MMSG_MAKE_GROUP_RES) ;
	msgMakeGroupRes.sCrypto = false ;
	msgMakeGroupRes.sCompressed = false ;	
	msgMakeGroupRes.cMessage = MMSG_NO_MAKE_GROUP_RES_S_C ;

	MMSG_CUT_GROUP_RES msgCutGroupRes = {0} ;
	msgCutGroupRes.sLength = sizeof(MMSG_CUT_GROUP_RES) ;
	msgCutGroupRes.sCrypto = false ;
	msgCutGroupRes.sCompressed = false ;	
	msgCutGroupRes.cMessage = MMSG_NO_CUT_GROUP_RES_S_C ;

	MMSG_JOIN_GROUP_RES msgJoinGroupRes = {0} ;
	msgJoinGroupRes.sLength = sizeof(MMSG_JOIN_GROUP_RES) ;
	msgJoinGroupRes.sCrypto = false ;
	msgJoinGroupRes.sCompressed = false ;	
	msgJoinGroupRes.cMessage = MMSG_NO_JOIN_GROUP_RES_S_C ;

	MMSG_CUT_FRIEND_REPORT msgCutFriendReport = {0} ;
	msgCutFriendReport.sLength = sizeof(MMSG_CUT_FRIEND_REPORT) ;
	msgCutFriendReport.sCrypto = false ;
	msgCutFriendReport.sCompressed = false ;
	msgCutFriendReport.cMessage = MMSG_NO_CUT_FRIEND_REPORT_S_C ;

	MMSG_RENAME_GROUP_RES msgRenameGroupRes = {0} ;
	msgRenameGroupRes.sLength = sizeof(MMSG_RENAME_GROUP_RES) ;
	msgRenameGroupRes.sCrypto = false ;
	msgRenameGroupRes.sCompressed = false ;
	msgRenameGroupRes.cMessage = MMSG_NO_RENAME_GROUP_RES_S_C ;


	// Mail =======================================================
	MMSG_MAIL_LIST_RES msgMailListRes = {0} ;
	msgMailListRes.header.sLength = sizeof(MMSG_MAIL_LIST_RES) ;
	msgMailListRes.header.sCrypto = false ;
	msgMailListRes.header.sCompressed = false ;
	msgMailListRes.cMessage = MMSG_NO_MAIL_LIST_RES_S_C ;
	
	MMSG_MAIL_TEXT_RES msgMailTextRes = {0} ;
	msgMailTextRes.header.sLength = sizeof(MMSG_MAIL_TEXT_RES) ;
	msgMailTextRes.header.sCrypto = false ;
	msgMailTextRes.header.sCompressed = false ;
	msgMailTextRes.cMessage = MMSG_NO_MAIL_TEXT_RES_S_C ;

	MMSG_MAIL_SEND_RES msgMailSendRes = {0} ;
	msgMailSendRes.header.sLength = sizeof(MMSG_MAIL_SEND_RES) ;
	msgMailSendRes.header.sCrypto = false ;
	msgMailSendRes.header.sCompressed = false ;
	msgMailSendRes.cMessage = MMSG_NO_MAIL_SEND_RES_S_C ;

	MMSG_MAIL_RECV msgMailRecv = {0} ;
	msgMailRecv.header.sLength = sizeof(MMSG_MAIL_RECV) ;
	msgMailRecv.header.sCrypto = false ;
	msgMailRecv.header.sCompressed = false ;
	msgMailRecv.cMessage = MMSG_NO_MAIL_RECV_S_C ;


	MMSG_MAIL_FAIL msgMailFail = {0} ;
	msgMailFail.header.sLength = sizeof(MMSG_MAIL_FAIL) ;
	msgMailFail.header.sCrypto = false ;
	msgMailFail.header.sCompressed = false ;
	msgMailFail.cMessage = MMSG_NO_MAIL_FAIL_S_C ;


	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;	
	LARGE_INTEGER li = {0, } ;
	SetWaitableTimer( hTimer, &li, 10, NULL, NULL, TRUE ) ;


	while (g_pThreadRunning) {
		if( g_pPacketQueue->empty() ) {			
			WaitForSingleObject( hTimer, INFINITE ) ;
			continue ;
		}
		else {
			pack = g_pPacketQueue->get() ;			
			
			if( pack == NULL ) {
				WaitForSingleObject( hTimer, INFINITE ) ;
				continue ;
			}
			else {

			
				Mainmsg = (u_char)pack->buf[2] ;

				
				// ������ �ʱ�ȭ..													
				//pMyCharacName = NULL;
				pMyInfo = NULL ;		
				pFriendInfo = NULL ;	
				pTemp = NULL ;
				pMyFriendList = NULL ;

				int pos = 0 ;
				try{
					// ������ �������� ���� �����͵��̴�..
					pDSComm = pack->pDSComm ;
					if(!pDSComm){
						::PrintConsole("[ERROR] 'pDSComm' is NULL \n") ;
						continue ;
					}

					pos = 1 ;
					
					pMyInfo = (_FriendInfo*)pDSComm->m_pFriendInfo ;		// �ڱ� ����
					if( !pMyInfo )
					{
						pDSComm->DisConnect() ;
						::PrintConsole("[ERROR] 'WORK:pDSComm->m_pFriendInfo' is NULL \n") ;
						continue ;
					}

					pos = 2 ;

					//pMyCharacName = pMyInfo->GetName() ;		// ĳ���̸�	
					pMyFriendList = pMyInfo->GetFriendListPtr() ;				// ģ���� ���� ����Ʈ
					pMyMailList = pMyInfo->GetMailListPtr() ;

					pos = 3 ;

					if( !pMyMailList  || !pMyFriendList ) {
						pDSComm->DisConnect() ;
						::PrintConsole("[ERROR] '!pMyMailList || !pMyFriendList' is NULL \n") ;
						continue ;
					}

					pos = 4 ;
					if( pMyInfo->IsLogon() == false ) {	// �α׿������̴�.
						pDSComm->DisConnect() ;
						::PrintConsole("[ERROR] pMyInfo->ds_com == NULL \n" ) ;
						continue ;
					}
					//if( Mainmsg != MMSG_NO_MY_STATE_UPDATE_C_S && pMyInfo->state == CHARAC_STATE_LOGOFF ){
					//	::PrintConsole("[ERROR] Mainmsg != MMSG_NO_MY_STATE_UPDATE_C_S && pMyInfo->state == CHARAC_STATE_LOGOFF \n" ) ;
					//	continue ;
					//}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION]  Get Queue : g_pPacketQueue->get() pos : %d, msg : %d\n", pos, Mainmsg ) ;
					continue ;
				}
				
								
			}				
		}
		

		try{
			switch( Mainmsg ) {		

			case MMSG_NO_MAKE_FRIEND_REQ_C_S_C :
				MMSG_MAKE_FRIEND_REQUEST * pMsgMakeFriendRequest ;
				pMsgMakeFriendRequest = (MMSG_MAKE_FRIEND_REQUEST*)pack->buf ;

				pMsgMakeFriendRequest->cCharacName[12] = 0 ;

				try{
						
#ifdef _TRACE_		
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_MAKE_FRIEND_REQUEST' Packet <%s>\n",pMyInfo->GetName() ) ;					
#endif
					
					if( strcmp( pMyInfo->GetName(), pMsgMakeFriendRequest->cCharacName ) == 0 )
					{
						bRes = true ;
					}
					else if( pMyInfo->GetName()[0] == 0 )
					{						
						pMyInfo->DisconnectForLogout() ;

						continue ;
					}
					else
					{
						bRes = pMyInfo->IsAlreadyFriend( pMsgMakeFriendRequest->cCharacName ) ;
					}				

					// ģ�� �α� ���� �̹� ��ϵǾ� �ִ��� Ȯ���ϰ� ������.. 
					if( bRes == true ) {
						// �̹� �����ϴ� ģ����� �뺸..
						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;				
						msgMakeFriendRes.cResult = 5 ;	// �̹� ������� ģ��.
						iRes = pMyInfo->SendMsg( (char*)&msgMakeFriendRes, msgMakeFriendRes.sLength ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pMyInfo->SendMsg code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
						}
						continue ;
					}

					pFriendInfo = g_pMemberQueue->FindNode(pMsgMakeFriendRequest->cCharacName) ;
					
					
					// ��� ģ���� ģ�� �αⰡ ���������� ���� �˻��Ѵ�..
					if( pFriendInfo == NULL ) {								// �α׿��� ��..

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[ERROR] �ش� ģ��(%s)�� ������ ���� �ʽ��ϴ�. \n", pMsgMakeFriendRequest->cCharacName) ;						
#endif

						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;				
						msgMakeFriendRes.cResult = 2 ;
						
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}
					else if( pFriendInfo->GetName()[0] == 0 )
					{
						g_pMemberQueue->DeleteMap( msgMakeFriendRes.cCharacName ) ;
						::PrintConsole( "[ERROR] %s, %d \n", __FILE__, __LINE__  ) ;

						pFriendInfo->DisconnectForLogout() ;
					}
					else if( pFriendInfo->IsGm() )
					{
						// GM ĳ���� �Դϴ�..
						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;				
						msgMakeFriendRes.cResult = 6 ;	// GM ĳ����..
						iRes = pMyInfo->SendMsg( (char*)&msgMakeFriendRes, msgMakeFriendRes.sLength ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pMyInfo->SendMsg code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
						}
						continue ;
						
					}
					else if( pFriendInfo->IsLogon() == false ) {				// �α׿��� ��..

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[ERROR] �ش� ģ��(%s)�� �α׿����� �Դϴ�. \n", pFriendInfo->GetName() ) ;
#endif

						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;
						msgMakeFriendRes.cResult = 2 ;

						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}					
					else if( pFriendInfo->GetOption() == 1 ) {					// ��밡 �źλ���..

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] �ش� ģ��(%s)�� �źλ��� �Դϴ�. \n", pFriendInfo->GetName() ) ;
#endif

						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;				
						msgMakeFriendRes.cResult = 1 ;
						
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
						
					}
					else if( pFriendInfo->GetFriendCurCount() >= MAX_MAKE_FRIEND || pFriendInfo->GetMemberCount() >= MAX_MEMBER ) {	// ģ���� �ʰ�

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] �ش� ģ��(%s) ������� ���� \n", pFriendInfo->GetName() ) ;
#endif

						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;				
						msgMakeFriendRes.cResult = 3 ;

						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}
					else if( pMyInfo->GetFriendCurCount() >= MAX_MAKE_FRIEND || pMyInfo->GetMemberCount() >= MAX_MEMBER ) {		// ���� �ʰ�

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] ��(%s)�� ������� ���� \n", pMyInfo->GetName() ) ;
#endif

						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;				
						msgMakeFriendRes.cResult = 4 ;

						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}
					
					// ģ������ �̹� ���� ��ϵǾ� �ִٸ� �ٷ� ��� �Ϸ� ��Ű��
					else if( pFriendInfo->IsAlreadyFriend( pMyInfo->GetName() ) == true )
					{

						if( pMyInfo->AddFriend( pFriendInfo->GetName(), pFriendInfo ) == true )
						{
							// ��� �ɼ� �ɱ�.
							pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;					
							g_pBackupReady->Insert(pMyInfo) ;	

							// ���ʿ� ��� �뺸			
							memcpy(msgMakeFriendRes.cCharacName, pFriendInfo->GetName(), 13) ;
							msgMakeFriendRes.cState = pFriendInfo->GetState() ;
							msgMakeFriendRes.cResult = 0 ;
							
							
							if ( pDSComm ) {
								iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									pDSComm->DisConnect() ;
									continue ;
								}
							}
						}

						continue ;
					}

					// ģ���α� ��û �ż��� ���~~ 
			
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] ģ�� �α� ��û ( %s -> %s ) \n",pMyInfo->GetName(), pMsgMakeFriendRequest->cCharacName) ;
#endif

					memcpy( pMsgMakeFriendRequest->cCharacName, pMyInfo->GetName(), 13 ) ;	// cCharacName �� ��~ �ٲ�ġ�� �ؾ��Ѵ�.
					iRes = pFriendInfo->SendMsg( (char*)pMsgMakeFriendRequest, pMsgMakeFriendRequest->sLength ) ;						
					if(iRes != 0){
						::PrintConsole( "[SO_ERR] pFriendInfo->SendMsg : code = %d\n", iRes ) ;
						::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
						continue ;
					}						
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_MAKE_FRIEND_REQ_C_S_C : \n") ;				
				}

		

				break;

		

			case MMSG_NO_MAKE_FRIEND_CON_C_S :
				MMSG_MAKE_FRIEND_CONFIRM * pMsgMakeFriendConfirm ;
				pMsgMakeFriendConfirm = (MMSG_MAKE_FRIEND_CONFIRM*) pack->buf ;

				pMsgMakeFriendConfirm->cCharacName[12] = 0 ;

				
				try{
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recv 'MMSG_MAKE_FRIEND_CONFIRM' packet<%s>\n",pMyInfo->GetName()) ;
#endif					
					
					if( pMsgMakeFriendConfirm->cCharacName[0] == 0 || pMsgMakeFriendConfirm->cCharacName[0] == ' ' ) {
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_ERROR, "[ERROR] pMsgMakeFriendReq->cToCharac[0] == 0 || pMsgMakeFriendReq->cToCharac[0] == ' ' \n") ;
#endif						

						continue ;
					}

					pFriendInfo = g_pMemberQueue->FindNode( pMsgMakeFriendConfirm->cCharacName ) ;

					
					// ģ���� ģ�� ��� ���ɿ��� �˻�..
					if( pFriendInfo  == NULL ) {
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_ERROR, "[ERROR] (%s) is logoff * \n",pMsgMakeFriendConfirm->cCharacName) ;
#endif

						memcpy( msgMakeFriendRes.cCharacName, pMsgMakeFriendConfirm->cCharacName, 13 ) ;				
						msgMakeFriendRes.cResult = 2 ;	// �ش� ĳ���Ͱ� �α׿������̰ų� ����..								
										
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}
					else if( pFriendInfo->IsLogon() == false ){

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_ERROR, "[ERROR] (%s) is logoff ** \n",pMsgMakeFriendConfirm->cCharacName) ;
#endif

						memcpy( msgMakeFriendRes.cCharacName, pMsgMakeFriendConfirm->cCharacName, 13 ) ;				
						msgMakeFriendRes.cResult = 2 ;	// �ش� ĳ���Ͱ� �α׿������̰ų� ����..
						
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
						
					}	
					

					else if( pMsgMakeFriendConfirm->cResult == 1 ) {	//�ź� ( �ź���Ŷ�� �˻� �Ұ͵� ���� �ٷ� ������..)
#ifdef _TRACE_
						// �� Ʈ���̽��� �� Ȯ���غ���..
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] ģ���α� �ź�. (%s ->%s) \n", pMyInfo->GetName(), pFriendInfo->GetName() ) ;
#endif			

						memcpy( msgMakeFriendRes.cCharacName, pMyInfo->GetName(), 13 ) ;				
						msgMakeFriendRes.cResult = 1 ;	// ��밡 �ź� (�źδ��� ĳ����(cFromCharacName)���� �뺸)
													
						iRes = pFriendInfo->SendMsg( (char*)&msgMakeFriendRes, msgMakeFriendRes.sLength ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pFriendInfo->SendMsg : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
						}
						
						continue ;
						
					}

					else if( pFriendInfo->GetFriendCurCount() >= MAX_MAKE_FRIEND || pFriendInfo->GetMemberCount() >= MAX_MEMBER ) {
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] (%s) ģ�� ������� ���� \n", pFriendInfo->GetName() ) ;
#endif


						memcpy( msgMakeFriendRes.cCharacName, pMsgMakeFriendConfirm->cCharacName, 13 ) ;				
						msgMakeFriendRes.cResult = 3 ;	// ���� ĳ������ ��� ģ�� �ο� �ʰ�..				
						
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}

					
					// ���� ģ�� ��� ���ɿ��� �˻�..
					else if( pMyInfo->GetFriendCurCount() >= MAX_MAKE_FRIEND || pMyInfo->GetMemberCount() >= MAX_MEMBER ) {				

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] (%s) ���� ������� ���� \n", pMyInfo->GetName() ) ;
#endif

						memcpy( msgMakeFriendRes.cCharacName, pMsgMakeFriendConfirm->cCharacName, 13 ) ;				
						msgMakeFriendRes.cResult = 4 ;	// ���� ĳ������ ��� ģ�� �ο� �ʰ�..				
						
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}

					
					// ���� ģ�� ������ ��� ģ�� �̹� ���� ���� �˻�	
					bRes = pMyInfo->IsAlreadyFriend( pMsgMakeFriendConfirm->cCharacName ) ;
					if( bRes == true ) {
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[ERROR] ��(%s) ģ��(%s) ������ �̹� �����ϴ� ģ���Դϴ�. \n", pMyInfo->GetName(), pFriendInfo->GetName() ) ;
#endif
						// �̹� �����ϴ� ģ����� �뺸..
						memcpy( msgMakeFriendRes.cCharacName, pMsgMakeFriendConfirm->cCharacName, 13 ) ;
						msgMakeFriendRes.cResult = 5 ;	// ���� ĳ������ ��� ģ�� �ο� �ʰ�..				
						
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop
						continue ;
					}


					// ģ���� ģ�� ������ ��� ģ�� �̹� ���� ���� �˻�

					bRes = pFriendInfo->IsAlreadyFriend( pMyInfo->GetName() ) ;
					if( bRes == true )
					{
						msgMakeFriendRes.cResult = 5 ;
						// ��� ģ������ ��� ��� ����(����� ���ʿ� ��� �����ش�..)			
						memcpy(msgMakeFriendRes.cCharacName, pMsgMakeFriendRequest->cCharacName, 13) ;						
						if ( pDSComm ) {
							iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
								continue ;
							}
						}

						memcpy( msgMakeFriendRes.cCharacName, pMyInfo->GetName(), 13 ) ;						
						iRes = pFriendInfo->SendMsg( (char*)&msgMakeFriendRes, msgMakeFriendRes.sLength ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE,"[ERROR] ģ��(%s)�� ģ�� ������ �̹� ��(%s)�� ������ �����մϴ�. \n", pFriendInfo->GetName(), pMyInfo->GetName() ) ;
#endif

						//Goto First Loop
						continue ;

					}

					
					bool bSuccess = true ;
					// ģ�� �߰�
					if( pMyInfo->AddFriend( pFriendInfo->GetName(), pFriendInfo ) == false )
					{
						bSuccess = false ;
					}
					

					// ģ�� �߰�
					if( bSuccess && pFriendInfo->AddFriend( pMyInfo->GetName(), pMyInfo ) == false )
					{
						bSuccess = false ;
					}

					if( bSuccess == true )
					{
						// ��� �ɼ� �ɱ�.
						pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;					
						g_pBackupReady->Insert(pMyInfo) ;						
						
						// ��� �ɼ� �ɱ�.
						pFriendInfo->_BackupState.bFrindListBackup = BACKUP_READY ;					
						g_pBackupReady->Insert(pFriendInfo) ;
						
						
						msgMakeFriendRes.cResult = 0 ;
						
						// ���ʿ� ��� �뺸			
						memcpy(msgMakeFriendRes.cCharacName, pFriendInfo->GetName(), 13) ;
						msgMakeFriendRes.cState = pFriendInfo->GetState() ;
						
						
						if ( pDSComm ) {
							iRes = pDSComm->m_pNetIO->SendRequest(msgMakeFriendRes.sLength, (char*)&msgMakeFriendRes) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
								continue ;
							}
						}
						
						
						// ��� ģ������ ��� ��� ����(����� ���ʿ� ��� �����ش�..)
						memcpy(msgMakeFriendRes.cCharacName, pMyInfo->GetName(), 13) ;
						msgMakeFriendRes.cState = pMyInfo->GetState() ;
						
						iRes = pFriendInfo->SendMsg( (char*)&msgMakeFriendRes, msgMakeFriendRes.sLength ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;		
							continue ;
						}
						
#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] ģ�� ��� ���� ( %s + %s ). \n", pMyInfo->GetName(), pFriendInfo->GetName() ) ;
#endif
					}
					else
					{
						::PrintConsole("[ERROR] Make friend is failed (%s <-> %s)\n", pMyInfo->GetName(), pFriendInfo->GetName() ) ;
					}
					
				}
				catch (...) {
					PrintConsole("[EXCEPTION] case MMSG_NO_MAKE_FRIEND_CON_C_S : \n") ;
				}
				
				break;

				

			case MMSG_NO_CUT_FRIEND_REQ_C_S :
				MMSG_CUT_FRIEND_REQ * pMsgCutFriendReq ;
				pMsgCutFriendReq = (MMSG_CUT_FRIEND_REQ*)pack->buf ;

				pMsgCutFriendReq->cFriendCharac[12] = 0 ;

				try{
	
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_CUT_FRIEND_REQ' Packet (%s -> %s)\n", pMyInfo->GetName(), pMsgCutFriendReq->cFriendCharac ) ;
#endif
					
					// MyInfo ���� ����..
					pMyInfo->DeleteFriend( pMsgCutFriendReq->cFriendCharac ) ;
					pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
					pMyInfo->SetMyChatFriend( NULL ) ;
					g_pBackupReady->Insert( pMyInfo ) ;
					
					
					// �ʿ� ģ���� ����Ÿ�� ����.. DB�� ���� �ܾ�;��Ѵ�..					
					pFriendInfo = g_pMemberQueue->FindNode( pMsgCutFriendReq->cFriendCharac ) ;
					
					
					if( pFriendInfo == NULL ) {						
						pFriendInfo = g_pMemberQueue->GetNode( pMsgCutFriendReq->cFriendCharac ) ;				
												
						if( pFriendInfo ) {							
							bRes = pFriendInfo->ReadFromDB( g_pDBCommand ) ;
														
							if( bRes == false ) {								
								g_pMemberQueue->PutNode( pFriendInfo ) ;
								
								// DB �� ĳ���Ͱ� ���°��̴�..(�Ƹ� �����ȵ�..)
								// ��ư.. �׷��� ���� �뺸�� ������..
								memcpy(msgCutFriendReport.cCharacName, pMsgCutFriendReq->cFriendCharac, 13) ;
								if( pDSComm ) {	// �¶���.. ��..				
									iRes = pDSComm->m_pNetIO->SendRequest(msgCutFriendReport.sLength, (char*)&msgCutFriendReport) ;
									if(iRes != 0){
										::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
										::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
										pDSComm->DisConnect() ;
									}
								}							
								continue ;
							}
						}
						else {
							continue ;
						}
					}
					
					if( pFriendInfo ) {

						pFriendInfo->DeleteFriend( pMyInfo->GetName() ) ;
						// Backup option checking
						//pFriendInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
						//pFriendInfo->SetMyChatFriend( NULL ) ;
						//g_pBackupReady->Insert(pFriendInfo) ;						

					}
									
					
					memcpy(msgCutFriendReport.cCharacName, pMsgCutFriendReq->cFriendCharac, 13) ;
					if( pDSComm ) {	// �¶���.. ��..				
						iRes = pDSComm->m_pNetIO->SendRequest(msgCutFriendReport.sLength, (char*)&msgCutFriendReport) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}			
					
					
					memcpy(msgCutFriendReport.cCharacName, pMyInfo->GetName(), 13) ;
					if(  pFriendInfo && pFriendInfo->IsLogon() ) {	// �¶���.. ��..				
						iRes = pFriendInfo->SendMsg( (char*)&msgCutFriendReport, msgCutFriendReport.sLength ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							continue ;
							
						}
					}
					else {	// ��������

						if( pFriendInfo->GetMailCurCount() < MAX_MAIL_COUNT ) {
							// ���� �ε��� ��ȣ�� ���´�.							
							u_int uiMailIndex = 0 ;
							
							
							// ���Ŀ� �α��� ������ �������� �˷���� �Ѵ�..
							// �� ���� ��������..
							if( g_pDBCommand->MSMessage( &uiMailIndex, pMyInfo->GetName(), MSMESSAGE_CUT_FRIEND) )
							{
								// �ش� ģ���� ���� ����Ʈ�� �ε����� �־���� �Ѵ�.

								memset( &mail, 0, sizeof(mail) ) ;
								mail.check = false ;
								mail.uiIndex = uiMailIndex ;
								pFriendInfo->AddMail( mail ) ;

								pFriendInfo->_BackupState.bMailListBackup = BACKUP_READY ;								

							}						
							
						}					
						
					}

					pFriendInfo->SetMyChatFriend( NULL ) ;
					pFriendInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
					g_pBackupReady->Insert( pFriendInfo ) ;
									

				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_CUT_FRIEND_REQ_C_S : \n") ;
				}

				break; 

			case MMSG_NO_FRIEND_LIST_REQ_C_S :
				MMSG_FRIEND_LIST_REQ * pMsgFriendListReq ;
				pMsgFriendListReq = (MMSG_FRIEND_LIST_REQ*)pack->buf ;

				try{
			
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recv 'MMSG_FRIEND_LIST_REQ' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif
					
					memset(&msgFriendListRes, 0, sizeof(MMSG_FRIEND_LIST_RES)) ;
					
					// ģ�� ����Ʈ�� ���� ���̷� ������� �Ѵ�..
					i = 0, j = 0 ;			
					// �������� ����Ʈ �����..
					pMyInfo->SetFriendListPakcet( &msgFriendListRes ) ;					
					if( pDSComm->m_pNetIO )
					{
						iRes = pDSComm->m_pNetIO->SendRequest(msgFriendListRes.sLength, (char*)&msgFriendListRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
							
						}
					}					
		
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Friend List Send (Response) (CharacName : %s, Count : %d) \n", pMyInfo->GetName(), msgFriendListRes.cCount ) ;
#endif

				}
				catch (...) {
					::PrintConsole("[EXCEPTION] ase MMSG_NO_FRIEND_LIST_REQ_C_S : \n") ;
				}
				
				

				break;
				

			case MMSG_NO_MY_STATE_UPDATE_C_S :
				MMSG_MY_STATE_UPDATE * pMsgMyStateUpdate ;
				pMsgMyStateUpdate = (MMSG_MY_STATE_UPDATE *)pack->buf ;
				
				try{
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_MY_STATE_UPDATE' Packet <%s>\n",pMyInfo->GetName() ) ;				
#endif
					
					pMyInfo->SetState( pMsgMyStateUpdate->cState ) ;
					
					memcpy(msgFriendStateReport.cCharacName, pMyInfo->GetName(), 13) ;
					msgFriendStateReport.cState = pMyInfo->GetState() ;
					
					// ģ���鿡�� �����϶�..
					iRes = pMyInfo->SendMsg( (char*)&msgFriendStateReport, msgFriendStateReport.sLength, _SEND_MODE_MY_FRIENDS ) ;
					if( iRes != 0 )
					{
						::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
						::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;	
						continue ;
					}
					
				}
				catch (...) {					
					::PrintConsole("[EXCEPTION] case MMSG_NO_MY_STATE_UPDATE_C_S : \n") ;
				}


				break;

			//#define MMSG_NO_MAKE_GROUP_REQ_C_S		// �׸� ����� ��û
			case MMSG_NO_MAKE_GROUP_REQ_C_S :

				MMSG_MAKE_GROUP_REQ * pMsgMakeGroupReq ;
				pMsgMakeGroupReq = (MMSG_MAKE_GROUP_REQ*)pack->buf ;

				pMsgMakeGroupReq->cGroupName[14] = 0 ;

				try{
#ifdef _TRACE_					
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_NO_MAKE_GROUP_REQ_C_S' Packet <ID : %s, GROUP : %s>\n",pMyInfo->GetName(), pMsgMakeGroupReq->cGroupName ) ;
#endif
					
					if(pMsgMakeGroupReq->cGroupName[0] == 0 
						|| pMsgMakeGroupReq->cGroupName[0] == ' ' 
						|| strcmp( pMsgMakeGroupReq->cGroupName, "ģ��") == 0 ) {
						continue ;
					}
					
					memcpy(msgMakeGroupRes.cGroupName, pMsgMakeGroupReq->cGroupName, 15 ) ;
					
										
					// �׸� ��� ���ɿ��� �˻�..
					iRes = 0 ;
					if( pMyInfo->GetFriendCurCount() >= MAX_MAKE_FRIEND || pMyInfo->GetGroupCurCount() >= MAX_GROUP ) 
					{
						msgMakeGroupRes.cResult = 1 ;	// ���� �޽���..				
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeGroupRes.sLength, (char*)&msgMakeGroupRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
						}
						//Goto First Loop						
						continue ;
					}			
					
					
					
					// �� ����Ʈ ������ ��� �׸� ���
					memset(&member, 0, sizeof(_Member)) ;
					member.characName[0] = _GROUP_PARSING_CHAR_ ;	// �׸� �̸����� ���н����ش�..
					strncpy(&member.characName[1], pMsgMakeGroupReq->cGroupName, 14) ;										
					pMyInfo->AddFriend( member.characName, NULL ) ;
					// ��� �ɼ� �ɱ�.
					pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;					
					g_pBackupReady->Insert(pMyInfo) ;
					
										
					// �׸� ���� ���� ��� 
					memcpy(msgMakeGroupRes.cGroupName, pMsgMakeGroupReq->cGroupName, 15 ) ;
					msgMakeGroupRes.cResult = 0 ;	// 0 .���� �޽���..
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] �׸� ���� ���� \n" ) ;
#endif
					if ( pDSComm ) {
						iRes = pDSComm->m_pNetIO->SendRequest(msgMakeGroupRes.sLength, (char*)&msgMakeGroupRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_MAKE_GROUP_REQ_C_S : \n " ) ;
				}

					

				break;		
				
			//#define MMSG_NO_CUT_GROUP_REQ_C_S			// �׸� ���� ��û
			case MMSG_NO_CUT_GROUP_REQ_C_S :
				MMSG_CUT_GROUP_REQ * pMsgCutGroup ;
				pMsgCutGroup = (MMSG_CUT_GROUP_REQ*)pack->buf ;

				pMsgCutGroup->cGroupName[14] = 0 ;

				try{
#ifdef _TRACE_
					g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_CUT_GROUP_REQ' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif
					
					msgCutGroupRes.cResult = 1 ;
					memcpy(msgCutGroupRes.cGroupName, pMsgCutGroup->cGroupName, 15) ;


					// MyInfo ���� ����..
					memset(&member, 0, sizeof(_Member)) ;
					member.characName[0] = _GROUP_PARSING_CHAR_ ;	// �׸� �̸����� ���н����ش�..
					strcpy(&member.characName[1], pMsgCutGroup->cGroupName ) ;

					if( pMyInfo->DeleteFriend( member.characName ) )
					{
						msgCutGroupRes.cResult = 0 ;
						pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
						g_pBackupReady->Insert(pMyInfo) ;
					}
					
					
					// �׸� ���� ��� 			
					if ( pDSComm ) {
						iRes = pDSComm->m_pNetIO->SendRequest(msgCutGroupRes.sLength, (char*)&msgCutGroupRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}	
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_CUT_GROUP_REQ_C_S : \n") ;
				}
				

				break;
						
			//#define MMSG_NO_JOIN_GROUP_REQ_C_S		// ģ���� �׸��� ����ϵ��� ��û..
			case MMSG_NO_JOIN_GROUP_REQ_C_S :
				MMSG_JOIN_GROUP_REQ * pMsgJoinGroup ;
				pMsgJoinGroup = (MMSG_JOIN_GROUP_REQ*)pack->buf ;

				pMsgJoinGroup->cFriendName[12] = 0 ;
				pMsgJoinGroup->cGroupName[14] = 0 ;

				try{	// label A
#ifdef _TRACE_
					g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_JOIN_GROUP_REQ' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif
					
					if( pMsgJoinGroup->cFriendName[0] == 0 || pMsgJoinGroup->cFriendName[0] == ' '){
						continue;
					}
					if( pMsgJoinGroup->cGroupName[0] == 0 || pMsgJoinGroup->cGroupName[0] == ' ' ) {
						continue ;
					}
					
					memcpy(msgJoinGroupRes.cFriendName, pMsgJoinGroup->cFriendName, 13) ;
					memcpy(msgJoinGroupRes.cGroupName, pMsgJoinGroup->cGroupName, 15) ;
					msgJoinGroupRes.cResult = 1 ;
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_JOIN_GROUP_REQ_C_S : label A \n") ;
				}
				
				
				memset(&member, 0, sizeof(_Member) ) ;
				member.characName[0] = _GROUP_PARSING_CHAR_ ;
				strcpy(&member.characName[1], pMsgJoinGroup->cGroupName ) ;

				if( pMyInfo->MoveFriend( pMsgJoinGroup->cFriendName, member.characName ) )
				{					
					msgJoinGroupRes.cResult = 0 ;

					pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
					g_pBackupReady->Insert( pMyInfo ) ;
				}

				try{	// label C
					if ( pDSComm ) {	
						iRes = pDSComm->m_pNetIO->SendRequest(msgJoinGroupRes.sLength, (char*)&msgJoinGroupRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}			
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_JOIN_GROUP_REQ_C_S : label C \n") ;
				}		
			


				break;


			case MMSG_NO_RENAME_GROUP_REQ_C_S :
				MMSG_RENAME_GROUP_REQ * pMsgRenameGroupReq ;
				pMsgRenameGroupReq = (MMSG_RENAME_GROUP_REQ*)pack->buf ;

				pMsgRenameGroupReq->cGroupName[14] = 0 ;
				pMsgRenameGroupReq->cReName[14] = 0 ;

				try{
					msgRenameGroupRes.cResult = 1 ;

					if( pMyInfo->GroupRename( pMsgRenameGroupReq->cGroupName, pMsgRenameGroupReq->cReName ) )
					{						
						msgRenameGroupRes.cResult = 0 ;

						pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
						g_pBackupReady->Insert( pMyInfo ) ;
					}					
					
					
					if ( pDSComm ) {
						iRes = pDSComm->m_pNetIO->SendRequest(msgRenameGroupRes.sLength, (char*)&msgRenameGroupRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_RENAME_GROUP_REQ_C_S : \n") ;
				}
				

				break; 


////////////////////////////
// Mail part
////////////////////////////

			case MMSG_NO_MAIL_LIST_REQ_C_S :
				MMSG_MAIL_LIST_REQ * pMailListReq ;
				pMailListReq = (MMSG_MAIL_LIST_REQ*)pack->buf ;

#ifdef _TRACE_
				g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recv 'MMSG_NO_MAIL_LIST_REQ_C_S' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif

				try{
									
					memset( msgMailListRes.mail, 0, sizeof(msgMailListRes.mail) ) ;
					pMyInfo->SetMailListPacket( &msgMailListRes ) ;
					

					if ( pDSComm ) {
						iRes = pDSComm->m_pNetIO->SendRequest(msgMailListRes.header.sLength, (char*)&msgMailListRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}


				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_MAIL_LIST_REQ_C_S : \n") ;
				}
				

				break ;


			case MMSG_NO_MAIL_TEXT_REQ_C_S :
				MMSG_MAIL_TEXT_REQ * pMailTextReq ;
				pMailTextReq = (MMSG_MAIL_TEXT_REQ*)pack->buf ;

#ifdef _TRACE_
				g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recv 'MMSG_NO_MAIL_TEXT_REQ_C_S' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif

				try{
					
					bRes = g_pDBCommand->GetMail( pMailTextReq->uiIndex, &msgMailTextRes.mailInfo ) ;
					if(bRes == false)	// ����
					{
						msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_TEXT_RES_S_C ;

						if ( pDSComm ) {
							iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
							}
						}
						continue ;
					}

					if ( pDSComm ) {
						// header(3) + MailHeader(23) + strLen
						msgMailTextRes.header.sLength = 26 + msgMailTextRes.mailInfo.cStrLen ;	
						iRes = pDSComm->m_pNetIO->SendRequest(msgMailTextRes.header.sLength, (char*)&msgMailTextRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}

						// ���� ���� ǥ��..
						for( mailListIter = pMyMailList->begin() ; mailListIter != pMyMailList->end() ; mailListIter++ )
						{
							if( mailListIter->uiIndex == pMailTextReq->uiIndex )
							{
								mailListIter->check = true ;
								
								// ��� �غ�..
								pMyInfo->_BackupState.bMailListBackup = BACKUP_READY ;								
								g_pBackupReady->Insert( pMyInfo ) ;
								
								break;
							}
						}					
						
						
					}

				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_MAIL_TEXT_REQ_C_S : \n") ;
				}

				break ;


			case MMSG_NO_MAIL_SEND_REQ_C_S :
				{
				MMSG_MAIL_SEND_REQ * pMailSendReq ;
				pMailSendReq = (MMSG_MAIL_SEND_REQ*)pack->buf ;

				pMailSendReq->cToCharac[12] = 0 ;
				pMailSendReq->cText[100] = 0 ;

#ifdef _TRACE_
				g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recv 'MMSG_NO_MAIL_SEND_REQ_C_S' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif

				try{

					list<_Mail> * pMailList = NULL ;
					int	uid ;

					pFriendInfo = g_pMemberQueue->FindNode( pMailSendReq->cToCharac ) ;
					
					//============================================================================
					// 1. Mail list �о����..
					// ����Ÿ �ִ�.
					if( pFriendInfo )
					{
						pMailList = pFriendInfo->GetMailListPtr() ;
						if( pMailList == NULL )
						{
							::PrintConsole("[error] MMSG_NO_MAIL_SEND_REQ_C_S::pMailList == NULL \n") ;
							continue ;
						}
						if( pFriendInfo->IsGm() )
						{
							MMSG_MAIL_FAIL msgMailFail = {0} ;
							msgMailFail.header.sLength = sizeof(MMSG_MAIL_FAIL) ;
							msgMailFail.cMessage = MMSG_NO_MAIL_FAIL_S_C ;
							msgMailFail.cCmd = 4 ;	// GM ���Դ� ������ ���� �� �����ϴ�.
							
							if ( pDSComm ) {					
								iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									pDSComm->DisConnect() ;
								}
							}							
							continue ;							
						}
						
					}
					// ����Ÿ ����.
					else
					{	
						// list<_Mail> mailList ;
						pMailList = g_pMailList ;
						pMailList->clear() ;
						
						bRes = g_pDBCommand->GetOffMailList(pMailSendReq->cToCharac, &uid, pMailList) ;
						if( bRes == false ){
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->GetMailList() (%s) \n", pMailSendReq->cToCharac) ;
							msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_RES_S_C ;
							msgMailFail.cResult = 0 ;
							if ( pDSComm ) {
								iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									pDSComm->DisConnect() ;
								}
							}					
							continue ;
						}
					}

					//============================================================================
					// 2. check size
					if( pMailList->size() >= MAX_MAIL_COUNT ) {
						msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_RES_S_C ;
						msgMailFail.cResult = 1 ;
						if ( pDSComm ) {					
							iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
							}
						}							
						continue ;
					}



					//============================================================================
					// 3. Send mail
					u_int uiMailIndex = 0 ;

					if( pMyInfo->IsGm() )
					{
						bRes = g_pDBCommand->SendMail( &uiMailIndex, pMyInfo->GetName(), pMailSendReq->cText, pMailSendReq->cStrLen, _MailInfo::_FROM_GM ) ;
					}
					else
					{
						bRes = g_pDBCommand->SendMail( &uiMailIndex, pMyInfo->GetName(), pMailSendReq->cText, pMailSendReq->cStrLen, _MailInfo::_PRIVATE ) ;
					}


					if( bRes == false ){
						g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->SendMail( iRes, pMyInfo->characName, pMailSendReq->cText, pMailSendReq->cStrLen )  \n") ;
						msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_RES_S_C ;
						msgMailFail.cResult = 3 ;
						if ( pDSComm ) {					
							iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
							}
						}						
						continue ;
					}			
					
					
					
					//============================================================================
					// 4. Mail list update
					memset( &mail, 0, sizeof(_Mail) ) ;
					mail.uiIndex = uiMailIndex ;
					mail.check = false ;
					
					if( pFriendInfo )
					{
						
						pFriendInfo->AddMail( mail ) ;
															
						pFriendInfo->_BackupState.bMailListBackup = BACKUP_READY ;						
						g_pBackupReady->Insert( pFriendInfo ) ;
												
						
						// ����� �α������̶�� ������ ���������� �˸���.
						if( pFriendInfo->IsLogon() )
						{
							msgMailRecv.uiIndex = mail.uiIndex ;
							iRes = pFriendInfo->SendMsg( (char*)&msgMailRecv, msgMailRecv.header.sLength, _SEND_MODE_ME_ ) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								continue ;
							}
						}
					}
					else
					{

						pMailList->push_back( mail ) ;

						bRes = g_pDBCommand->UpdateMailList( uid, pMailList ) ;
						if( bRes == false ) {
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->UpdateMailList() \n") ;
							msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_RES_S_C ;
							msgMailFail.cResult = 3 ;
							if ( pDSComm ) {					
								iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									pDSComm->DisConnect() ;
								}
							}							
							continue ;
						}

					}									


					// 4. result send
					if ( pDSComm ) {					
						iRes = pDSComm->m_pNetIO->SendRequest(msgMailSendRes.header.sLength, (char*)&msgMailSendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_MAIL_SEND_REQ_C_S : \n") ;
				}
				}
				break;

			case MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S :
				{
				MMSG_MAIL_PUBLIC_SEND_REQ * pMailPublicSendReq ;
				pMailPublicSendReq = (MMSG_MAIL_PUBLIC_SEND_REQ*)pack->buf ;

				
				pMailPublicSendReq->cText[100] = 0 ;

#ifdef _TRACE_
				g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recv 'MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S' Packet <%s>\n",pMyInfo->GetName() ) ;
#endif

				try{

					list<_Mail> * pMailList = NULL ;
					int	uid ;
					char * pToName ;

					for( int i =0 ; i<100 ; i++ )
					{

						if( pMailPublicSendReq->cNames[i][0] == 0 )
							break; 


						pToName = pMailPublicSendReq->cNames[i] ;

						pFriendInfo = g_pMemberQueue->FindNode( pToName ) ;

						//============================================================================
						// 1. Mail list �о����..
						// ����Ÿ �ִ�.
						if( pFriendInfo )
						{
							pMailList = pFriendInfo->GetMailListPtr() ;
							if( pMailList == NULL )
							{
								::PrintConsole("[error] MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S::pMailList == NULL \n") ;
								continue ;
							}
							
						}
						// ����Ÿ ����.
						else
						{	
							// list<_Mail> mailList ;
							pMailList = g_pMailList ;
							pMailList->clear() ;
							
							bRes = g_pDBCommand->GetOffMailList(pToName, &uid, pMailList) ;
							if( bRes == false ){
								g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->GetMailList() (%s) \n", pToName) ;
								continue ;
							}
						}
	
						//============================================================================
						// 2. check size
						if( pMailList->size() >= MAX_MAIL_COUNT ) {
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] pMailList->size() >= MAX_MAIL_COUNT (%s) \n", pToName) ;
							continue ;
						}
	
	
	
						//============================================================================
						// 3. Send mail
						u_int uiMailIndex = 0 ;						

						bRes = g_pDBCommand->SendMail( &uiMailIndex
							, pMyInfo->GetName()
							, pMailPublicSendReq->cText
							, static_cast<char>(strlen(pMailPublicSendReq->cText))
							, _MailInfo::_PUBLIC ) ;
						
						if( bRes == false ){
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->SendMail( iRes, pMyInfo->characName, pMailPublicSendReq->cText, pMailPublicSendReq->cStrLen )  \n") ;
							continue ;
						}			
						
						
						
						//============================================================================
						// 4. Mail list update
						memset( &mail, 0, sizeof(_Mail) ) ;
						mail.uiIndex = uiMailIndex ;
						mail.check = false ;
						
						if( pFriendInfo )
						{
							pFriendInfo->AddMail( mail ) ;

							pFriendInfo->_BackupState.bMailListBackup = BACKUP_READY ;
							g_pBackupReady->Insert( pFriendInfo ) ;


							// ����� �α������̶�� ������ ���������� �˸���.
							if( pFriendInfo->IsLogon() )
							{
								msgMailRecv.uiIndex = mail.uiIndex ;
								iRes = pFriendInfo->SendMsg( (char*)&msgMailRecv, msgMailRecv.header.sLength, _SEND_MODE_ME_ ) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									continue ;
								}
							}
						}
						else
						{
	
							pMailList->push_back( mail ) ;
	
							bRes = g_pDBCommand->UpdateMailList( uid, pMailList ) ;
							if( bRes == false ) {
								g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->UpdateMailList() \n") ;								
								continue ;
							}
	
						}
					}	// for(
					
					// 4. result send
					if ( pDSComm ) {					
						iRes = pDSComm->m_pNetIO->SendRequest(msgMailSendRes.header.sLength, (char*)&msgMailSendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}
					
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S : \n") ;
				}
				}
				break ;

			case MMSG_NO_MAIL_DROP_C_S :
				{
					MMSG_MAIL_DROP * pMsgMailDrop = (MMSG_MAIL_DROP*)pack->buf ;

					pMyInfo->MailDrop( pMsgMailDrop->uiIndex, g_pDBCommand ) ;
					
					pMyInfo->_BackupState.bMailListBackup = BACKUP_READY ;														
					g_pBackupReady->Insert( pMyInfo );

				}
				break;

			default :
					// ������� �Դٸ� ���������� ��Ŷ.. ���������� ��Ŷ�� ���� ������ ��������.. ��
					if(pDSComm) {
						::PrintConsole("[WORK_ERR] ������ ���� ��Ŷ ����/ ������ �������� \n" ) ;
						pDSComm->DisConnect() ;
					}
					

			}	// switch
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_workerthread \n") ;
		}


			

	} // while

	delete g_pDBCommand ;
	delete g_pMailList ;


	::PrintConsole( "[WARNING] th_WorkThread Exit \n") ;
	g_pSystemLog->LogFileWrite( "[WARNING] th_WorkThread Exit \n") ;
	return 0 ;

	
}



DWORD	WINAPI	th_ChatWorkThread( LPVOID lp) 
{

		

	_packet *	pack = NULL ;
	int			Mainmsg = 0 ;	
	_h_DS_Com * pDSComm = NULL ;
	
	
	_FriendInfo * pMyInfo = NULL ;	
	_FriendInfo * pFriendInfo = NULL ;	
	list<_Member> * pMyFriendList = NULL ;
	list<_Member>::iterator memberListIter ;
	//char	*	pMyCharacName = NULL;

	// GM MAIL �� ������ ���� �ʿ��� �͵� ==============
	list<_Mail> * g_pMailList = new list<_Mail>;
	MMSG_MAIL_FAIL msgMailFail = {0} ;
	msgMailFail.header.sLength = sizeof(MMSG_MAIL_FAIL) ;
	msgMailFail.header.sCrypto = false ;
	msgMailFail.header.sCompressed = false ;
	msgMailFail.cMessage = MMSG_NO_MAIL_FAIL_S_C ;
	
	MMSG_MAIL_RECV msgMailRecv = {0} ;
	msgMailRecv.header.sLength = sizeof(MMSG_MAIL_RECV) ;
	msgMailRecv.header.sCrypto = false ;
	msgMailRecv.header.sCompressed = false ;
	msgMailRecv.cMessage = MMSG_NO_MAIL_RECV_S_C ;
	
	MMSG_MAIL_SEND_RES msgMailSendRes = {0} ;
	msgMailSendRes.header.sLength = sizeof(MMSG_MAIL_SEND_RES) ;
	msgMailSendRes.header.sCrypto = false ;
	msgMailSendRes.header.sCompressed = false ;
	msgMailSendRes.cMessage = MMSG_NO_MAIL_SEND_RES_S_C ;
	
	list<_Mail> * pMailList = NULL ;
	list<_Mail>::iterator mailListIter ;
	//==================================================

	
	int			iRes = 0 ;

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;	
	LARGE_INTEGER li = {0, } ;
	SetWaitableTimer( hTimer, &li, 10, NULL, NULL, TRUE ) ;
		

	MMSG_CHAT_RESULT	msgChatResult = {0} ;
	msgChatResult.sLength = sizeof(MMSG_CHAT_RESULT) ;
	msgChatResult.cMessage = MMSG_CHAT_RESULT_S_C ;


	COLE_DB_Executer *	g_pDBCommand = new COLE_DB_Executer(g_pDBCon) ;
	if(g_pDBCommand == NULL )
	{	
		g_pSystemLog->LogFileWrite( "th_WorkThread _j_DB_Command Initalize failed\n" ) ;
		return 0 ;
	}	
	
		
	DWORD			dwLastChatTime = 0 ;


	while (g_pThreadRunning) {
		if( g_pChatPacketQueue->empty() ) {			
			WaitForSingleObject( hTimer, INFINITE ) ;
			continue ;
		}
		else {
			pack = g_pChatPacketQueue->get() ;			
			if( pack == NULL ) {
				WaitForSingleObject( hTimer, INFINITE ) ;
				continue ;
			}
			else {
				Mainmsg = (u_char)pack->buf[2] ;

				try{
					// ������ �������� ���� �����͵��̴�..
					pDSComm = pack->pDSComm ;
					if(!pDSComm){
						::PrintConsole("[ERROR] 'pDSComm' is NULL \n") ;
						continue ;
					}				
					//pMyCharacName= pDSComm->m_pFriendInfo->GetName() ;		// ĳ���̸�	
					if( !pDSComm->m_pFriendInfo ) {
						pDSComm->DisConnect() ;
						::PrintConsole("[ERROR] 'CHAT:pDSComm->m_pFriendInfo' is NULL \n") ;
						continue ;
					}
					pMyInfo = pDSComm->m_pFriendInfo ;		// �ڱ� ����				
					pMyFriendList = pMyInfo->GetFriendListPtr() ;				// ģ���� ���� ����Ʈ

					//if( !pMyInfo || !pMyFriendList )
					//{
					//	pDSComm->releaseDSCom() ;
					//	::PrintConsole("[ERROR] '!pMyInfo || !pMyFriendList' is NULL \n") ;
					//	continue ;
					//}

				}
				catch (...) {
					::PrintConsole("[EXCEPTION] g_pChatPacketQueue->get() ; \n") ;
					continue ;
				}
			}
		}

		dwLastChatTime = timeGetTime() ;


		try{
			switch(Mainmsg) {
			case MMSG_NO_CHAT:
				MMSG_CHAT * pMsgChat ;
				pMsgChat = (MMSG_CHAT*)pack->buf ;
				pMsgChat->cFromCharacName[12] = 0 ;
				pMsgChat->cToCharacName[12] = 0 ;
				pMsgChat->cChatMsg[80] = 0 ;

				if( pMyInfo == NULL )
					continue ;
				

				try{

//#ifdef _TRACE_
//					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_CHAT' packet <%s>\n",pMyInfo->GetName()  ) ;					
//#endif 
					

					pMyInfo->SetMyChatFriend( pMsgChat->cToCharacName ) ;	

					pFriendInfo = pMyInfo->GetMyChatFriend() ;
					
					if ( pFriendInfo ) {	// �α��� ���θ� �˻�..
						
						/*
						if( pFriendInfo->GetState() == _CHARAC_STATE_LOGOFF 
							|| pFriendInfo->GetState() == _CHARAC_STATE_MOVING		)
						{
#ifdef _TRACE_
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[ERROR] (CHAT) �α׿����� �Դϴ�. \n") ;
#endif
							pMyInfo->SetMyChatFriend( NULL ) ;

							
							memcpy( msgChatResult.cToCharacName, pMsgChat->cToCharacName, 13 ) ;
							msgChatResult.cCode = _NOT_FIND_USER_ ;
							
							iRes = pMyInfo->SendMsg( (char*)&msgChatResult, msgChatResult.sLength, _SEND_MODE_ME_ ) ;
							if( iRes != 0 )
							{
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							}


							break;
						}						
						*/

						if( pFriendInfo->dwLastChatTime + 1000 > dwLastChatTime )
						{
							pFriendInfo->dwLastChatTime = dwLastChatTime ;
							++pFriendInfo->dwBadChatCount ;
							if( pFriendInfo->dwBadChatCount > 10 )
							{
								pDSComm->DisConnect() ;
							}							
							continue ;
						}
						
						pFriendInfo->dwBadChatCount = 0 ;
						pFriendInfo->dwLastChatTime = dwLastChatTime ;
						
						try{
							
							if ( pFriendInfo->IsLogon() ) {						

								// ģ������ ä�ø޼��� �߼�
								iRes = pFriendInfo->SendMsg( (char*)pMsgChat, pMsgChat->sLength, _SEND_MODE_ME_ ) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									continue ;
								}								
							}							
							
							else
							{

								
								memcpy( msgChatResult.cToCharacName, pMsgChat->cToCharacName, 13 ) ;
								msgChatResult.cCode = _NOT_FIND_USER_ ;
								
								iRes = pMyInfo->SendMsg( (char*)&msgChatResult, msgChatResult.sLength, _SEND_MODE_ME_ ) ;
								if( iRes != 0 )
								{
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									continue ;

								}
								
							}
							
							
							
						}
						catch (...) {
							::PrintConsole("[EXCEPTION] Chat send exception error \n") ;
							pMyInfo->SetMyChatFriend( NULL ) ;
						}
						
					}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_CHAT: \n ") ;
				}
										
				break ;


			case MMSG_NO_CHAT_GM:		// GM �� User �� ä���̴�..
				MMSG_CHAT * pMsgChatGM ;
				pMsgChatGM = (MMSG_CHAT*)pack->buf ;			

				try{
#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'GM CHAT: MMSG_CHAT' packet <%s> -> <%s>\n"
						,pMyInfo->GetName(), pMsgChatGM->cToCharacName  ) ;
#endif 	

					
					pFriendInfo = g_pMemberQueue->FindNode( pMsgChatGM->cToCharacName ) ;
					
										
					if ( pFriendInfo && pFriendInfo->IsLogon() ) {

						// ģ������ ä�ø޼��� �߼�
						iRes = pFriendInfo->SendMsg( (char*)pMsgChatGM, pMsgChatGM->sLength, _SEND_MODE_ME_ ) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							continue ;
						}		
						
					}
					else
					{
						::PrintConsole("[ERROR] GM CHAT : if ( pFriendInfo && pFriendInfo->IsLogon() ) chr:%s\n", pMsgChatGM->cToCharacName ) ;
					}
					
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] Chat send exception error \n") ;					
				}

				break ;

			case MMSG_NO_CHAT_WHISPER :	// �ӼӸ� 
				try{
					MMSG_CHAT * pMsgChatWhisper = (MMSG_CHAT*)pack->buf ;
					
					pFriendInfo = g_pMemberQueue->FindNode( pMsgChatWhisper->cToCharacName ) ;


					// ��밡 �α����϶��� ������.
					if( pFriendInfo && pFriendInfo->IsLogon() )
					{

						if( pFriendInfo->dwLastChatTime + 1000 > dwLastChatTime )
						{
							pFriendInfo->dwLastChatTime = dwLastChatTime ;
							++pFriendInfo->dwBadChatCount ;
							if( pFriendInfo->dwBadChatCount > 10 )
							{
								pDSComm->DisConnect() ;
							}							
							continue ;
						}
						
						pFriendInfo->dwBadChatCount = 0 ;
						pFriendInfo->dwLastChatTime = dwLastChatTime ;


						// ��밡 GM �϶�..
						if( pFriendInfo->IsGm() )
						{
							// GM ���� �Ӹ��� �õ��ϸ�.. �ý��� �޽����� �ǵ����ش�.. 							
							pMsgChatWhisper->sLength = sizeof(MMSG_CHAT) ;
							sprintf( pMsgChatWhisper->cChatMsg,  _GM_RETURN_MESSAGE )  ;							
							memcpy( pMsgChatWhisper->cToCharacName, pMsgChatWhisper->cFromCharacName, 13 ) ;
							sprintf( pMsgChatWhisper->cFromCharacName, "[SYSTEM]" ) ;
							
//#ifdef _TRACE_
//							g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[CHAT_WHISPER] %s -> %s : %s \n", pMsgChatWhisper->cFromCharacName, pMsgChatWhisper->cToCharacName, pMsgChatWhisper->cChatMsg ) ;
//#endif
							// ģ������ ä�ø޼��� �߼�
							iRes = pMyInfo->SendMsg( (char*)pMsgChatWhisper, pMsgChatWhisper->sLength, _SEND_MODE_ME_ ) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								continue ;
							}		
							
						}
						else
						{
//#ifdef _TRACE_
//							g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[CHAT_WHISPER] %s -> %s : %s \n", pMsgChatWhisper->cFromCharacName, pMsgChatWhisper->cToCharacName, pMsgChatWhisper->cChatMsg ) ;
//#endif
							// ģ������ ä�ø޼��� �߼�
							iRes = pFriendInfo->SendMsg( (char*)pMsgChatWhisper, pMsgChatWhisper->sLength, _SEND_MODE_ME_ ) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								continue ;
							}		

						}

					}					
					else	// ��밡 �α׿������϶�..
					{

						memcpy( msgChatResult.cToCharacName, pMsgChatWhisper->cToCharacName, 13 ) ;
						msgChatResult.cCode = _NOT_FIND_USER_ ;
						
						iRes = pMyInfo->SendMsg( (char*)&msgChatResult, msgChatResult.sLength, _SEND_MODE_ME_ ) ;
						if( iRes != 0 )
						{
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							continue ;
						}
					}
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] MMSG_NO_CHAT_WHISPER \n") ;					
				}
				
				break;
				

			case MMSG_NO_MAIL_SEND_REQ_FROM_GM :
				try
				{
					MMSG_MAIL_SEND_REQ * pMsgMailSendReq = (MMSG_MAIL_SEND_REQ*)(pack->buf) ;

#ifdef _TRACE_
					g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[INFO] Recved 'GM MAIL: MMSG_NO_MAIL_SEND_REQ_FROM_GM' packet <%s> -> <%s>\n"
						,pMyInfo->GetName(), pMsgMailSendReq->cToCharac  ) ;
#endif 	
					
					
					pFriendInfo = g_pMemberQueue->FindNode( pMsgMailSendReq->cToCharac ) ;
										
					
					bool bRes = false ;
					int uid = 0;
					
					//============================================================================
					// 1. Mail list �о����..
					// ����Ÿ �ִ�.
					if( pFriendInfo )
					{
						pMailList = pFriendInfo->GetMailListPtr() ;
						if( pMailList == NULL )
						{
							::PrintConsole("[error] MMSG_NO_MAIL_SEND_REQ_C_S::pMailList == NULL \n") ;
							continue ;
						}
					}
					// ����Ÿ ����.
					else
					{	
						// list<_Mail> mailList ;
						pMailList = g_pMailList ;
						pMailList->clear() ;
						
						bRes = g_pDBCommand->GetOffMailList( pMsgMailSendReq->cToCharac, &uid, pMailList) ;
						if( bRes == false ){
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->GetMailList() (%s) \n", pMsgMailSendReq->cToCharac ) ;
							msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_REQ_FROM_GM ;
							msgMailFail.cResult = 0 ;
							if ( pDSComm ) {
								iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									pDSComm->DisConnect() ;
								}
							}					
							continue ;
						}
					}

					//============================================================================
					// 2. check size
					if( pMailList->size() >= MAX_MAIL_COUNT ) {
						msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_REQ_FROM_GM ;
						msgMailFail.cResult = 1 ;
						if ( pDSComm ) {					
							iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
							}
						}							
						continue ;
					}



					//============================================================================
					// 3. Send mail
					u_int uiMailIndex = 0 ;

					if( pMyInfo->IsGm() )
					{
						bRes = g_pDBCommand->SendMail( &uiMailIndex, pMyInfo->GetName(), pMsgMailSendReq->cText, pMsgMailSendReq->cStrLen, _MailInfo::_FROM_GM ) ;
					}
					else
					{
						bRes = g_pDBCommand->SendMail( &uiMailIndex, pMyInfo->GetName(), pMsgMailSendReq->cText, pMsgMailSendReq->cStrLen, _MailInfo::_PRIVATE ) ;
					}


					if( bRes == false ){
						g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->SendMail( iRes, pMyInfo->characName, pMailSendReq->cText, pMailSendReq->cStrLen )  \n") ;
						msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_REQ_FROM_GM ;
						msgMailFail.cResult = 3 ;
						if ( pDSComm ) {					
							iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								pDSComm->DisConnect() ;
							}
						}						
						continue ;
					}			
					
					
					
					//============================================================================
					// 4. Mail list update
					_Mail mail ;
					memset( &mail, 0, sizeof(_Mail) ) ;
					mail.uiIndex = uiMailIndex ;
					mail.check = false ;
					
					if( pFriendInfo )
					{
						
						pFriendInfo->AddMail( mail ) ;
															
						pFriendInfo->_BackupState.bMailListBackup = BACKUP_READY ;						
						g_pBackupReady->Insert( pFriendInfo ) ;
												
						
						// ����� �α������̶�� ������ ���������� �˸���.
						if( pFriendInfo->IsLogon() )
						{
							msgMailRecv.uiIndex = mail.uiIndex ;
							iRes = pFriendInfo->SendMsg( (char*)&msgMailRecv, msgMailRecv.header.sLength, _SEND_MODE_ME_ ) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								continue ;
							}
						}
					}
					else
					{

						pMailList->push_back( mail ) ;

						bRes = g_pDBCommand->UpdateMailList( uid, pMailList ) ;
						if( bRes == false ) {
							g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[MAIL] bRes = g_pDBCommand->UpdateMailList() \n") ;
							msgMailFail.cCmd = (u_char)MMSG_NO_MAIL_SEND_REQ_FROM_GM ;
							msgMailFail.cResult = 3 ;
							if ( pDSComm ) {					
								iRes = pDSComm->m_pNetIO->SendRequest(msgMailFail.header.sLength, (char*)&msgMailFail) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									pDSComm->DisConnect() ;
								}
							}							
							continue ;
						}

					}									


					// 4. result send
					if ( pDSComm ) {					
						iRes = pDSComm->m_pNetIO->SendRequest(msgMailSendRes.header.sLength, (char*)&msgMailSendRes) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}
				}
				catch (...) 
				{
					::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
				}
				break ;
				
			default :
				// ������� �Դٸ� ���������� ��Ŷ.. ���������� ��Ŷ�� ���� ������ ��������.. ��
				if(pDSComm) {
					::PrintConsole("[WORK_ERR] ������ ���� ��Ŷ ����/ ������ �������� \n" ) ;
					pDSComm->DisConnect() ;
				}
			}//switch
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_ChatWorkThread \n") ;
		}

		

	}


	::PrintConsole( "[WARNING] th_ChatWorkThread Exit \n") ;
	g_pSystemLog->LogFileWrite( "[WARNING] th_ChatWorkThread Exit \n") ;
	return 0 ;
	
		
}

extern int g_iCurLoginMsgNum ;
extern int g_iCurLoginMsgPos ;

DWORD WINAPI th_LoginWorkThread( LPVOID lp) 
{

	
	COLE_DB_Executer *		g_pDBCommand ;
	// ole-db	
	g_pDBCommand = new COLE_DB_Executer( g_pDBCon );
	if(g_pDBCommand == NULL ){	
		g_pSystemLog->LogFileWrite( "th_LoginWorkThread _j_DB_Command Initalize failed\n" ) ;
		return 0 ;
	}

	_packet *	pack = NULL ;
	int			Mainmsg = 0 ;	
	_h_DS_Com * pDSComm = NULL ;
	
	_FriendInfo * pMyInfo = NULL ;	
	_FriendInfo * pFriendInfo = NULL ;	

	list<_Member> * pMyFriendList = NULL ;
	list<_Member>::iterator memberListIter ;	
	list<_Member>::iterator FriendsListIter ;	
	//char	*	pMyCharacName = NULL;
	
	int			iRes = 0 ;
	bool		bRes = false ;

	
	int & pos = g_iCurLoginMsgPos ;

	
	MMSG_MSLOGIN_RESULT	msgLoginResult = {0} ;
	msgLoginResult.sLength = sizeof(MMSG_MSLOGIN_RESULT) ;
	msgLoginResult.sCrypto = false ;
	msgLoginResult.sCompressed = false ;
	msgLoginResult.cMessage = MMSG_NO_LOGIN_RESULT ;

	MMSG_LOGOUT_REPORT	msgLogoutReport = {0} ;
	msgLogoutReport.sLength = sizeof(MMSG_LOGOUT_REPORT) ;
	msgLogoutReport.sCrypto = false ;
	msgLogoutReport.sCompressed = false ;
	msgLogoutReport.cMessage = MMSG_NO_LOGOUT_REPORT_S_C ;


	MMSG_FRIEND_STATE_REPORT msgFriendStateReport = {0} ;
	msgFriendStateReport.sLength = sizeof(MMSG_FRIEND_STATE_REPORT) ;
	msgFriendStateReport.sCrypto = false ;
	msgFriendStateReport.sCompressed = false ;
	msgFriendStateReport.cMessage = MMSG_NO_FRIEND_STATE_REPORT_S_C ;
	msgFriendStateReport.cState = _CHARAC_STATE_LOGOFF ;


	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;	
	LARGE_INTEGER li = {0, } ;
	SetWaitableTimer( hTimer, &li, 10, NULL, NULL, TRUE ) ;
	
	
	while (g_pThreadRunning) {
		
		try{
			pack = g_pLoginPacketQueue->get() ;
			if( pack == NULL ) {
				WaitForSingleObject( hTimer, INFINITE ) ;
				pos = -1 ;
				g_iCurLoginMsgNum = Mainmsg = 0 ;
				continue ;
			}
			else {
				g_iCurLoginMsgNum = Mainmsg = (u_char)pack->buf[2] ;				
				
				// ������ �ʱ�ȭ..
				//pMyCharacName = NULL;
				pMyInfo = NULL ;	
				pFriendInfo = NULL ;					
				pMyFriendList = NULL ;
				
				// ������ �������� ���� �����͵��̴�..
				pDSComm = pack->pDSComm ;
				if(!pDSComm) {
					::PrintConsole("[ERROR] LoginWorkThread 'pDSComm' is NULL \n") ;
					continue ;
				}
				
				pos = -2 ;
				
			}		
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}		
		
		try{
			switch(Mainmsg) {
			
			case MMSG_NO_LOGIN_C_S :

				pos = -3 ;


				MMSG_MSLOGIN * pMsgMSLogin ;
				pMsgMSLogin = (MMSG_MSLOGIN*)pack->buf ;

				int uid ;
				
				pMsgMSLogin->cCharacName[12] = 0 ;
				try{
					if( pMsgMSLogin->cCharacName[0] == 0 || pMsgMSLogin->cCharacName[0] ==' '){
						if( pDSComm )
							pDSComm->DisConnect() ;
						continue ;
					}

					pos = -4 ;

					if( !pDSComm->m_pNetIO ) {
						if( pDSComm )
							pDSComm->DisConnect() ;
						continue ;
					}

					pos = 1 ;
					
#ifdef _TRACE_
					g_pTrace->OutputString(_TRACE_CLASS_LOGIN, "[INFO] Recv 'MMSG_MSLOGIN' (CharacName : %s) \n", pMsgMSLogin->cCharacName) ;
#endif

					// Login �˻�..			
					uid = g_pDBCommand->GetUID(pMsgMSLogin->cCharacName) ;
					if( uid == 0 ) {
						::PrintConsole("[ERROR] 'g_pDBCommand->Login(%s)' Failed \n", pMsgMSLogin->cCharacName) ;
						msgLoginResult.cResult = 4 ;
						// �α��� ���							
						iRes = pDSComm->m_pNetIO->SendRequest(msgLoginResult.sLength, (char*)&msgLoginResult) ;					
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;							
						}

						pDSComm->DisConnect() ;

						continue ;
					}

					pos = 2 ;

					
					pMyInfo = g_pMemberQueue->FindNode(pMsgMSLogin->cCharacName) ;

					pos = 1000 ;
									
					if( !pMyInfo ) {
						
						pMyInfo = g_pMemberQueue->GetNode(pMsgMSLogin->cCharacName) ;
						pos = 3 ;
						if( !pMyInfo ) {
							
							pos = 4 ;

							::PrintConsole("[ERROR] Login Failed C<%s> 'pMyInfo == NULL' \n", pMsgMSLogin->cCharacName ) ;
							msgLoginResult.cResult = 4 ;
							// �α��� ���					
							if( pDSComm->m_pNetIO )
							{
								iRes = pDSComm->m_pNetIO->SendRequest(msgLoginResult.sLength, (char*)&msgLoginResult) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
								}
							}							

							pDSComm->DisConnect() ;	
															
							continue ;
						}
						

						pos = 5 ;
						

						if( pMyInfo->GetFriendListPtr() == NULL || pMyInfo->GetMailListPtr() == NULL ){
							pDSComm->DisConnect() ;	
							continue ;
						}
						
						// �������.. ����Ÿ �ε��� �Ϸ�ɶ����� ���̴�~~						
						pos = 6 ;
						
						
						pMyInfo->SetUniqueID( uid ) ;						

						if( pMyInfo->ReadFromDB( g_pDBCommand ) == false ) 
						{
							
							::PrintConsole("[ERROR] Login Failed 'pMyInfo->ReadFromDB' charac : %s \n"
								, pMsgMSLogin->cCharacName) ;
							
							// �α��� ���
							msgLoginResult.cResult = 4 ;
							if( pDSComm->m_pNetIO )
							{
								iRes = pDSComm->m_pNetIO->SendRequest(msgLoginResult.sLength, (char*)&msgLoginResult) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d \n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d \n", __FILE__, __LINE__ ) ;
									continue ;
								}
							}
														
							pos = 7 ;
							
							//DISCONNECT MESSAGE
							pDSComm->m_pFriendInfo = NULL ;
							pDSComm->DisConnect() ;
							//Disconnect(pDSComm) ;						
													
							
							g_pMemberQueue->PutNode( pMyInfo ) ;
							

							pos = 8 ;
							
							// Goto First Loop
							continue ;
						}
						pos = 9 ;
					}

					if( pMyInfo->GetName()[0] == 0 )
					{
						g_pMemberQueue->DeleteMap( pMsgMSLogin->cCharacName ) ;
						::PrintConsole( "[ERROR] %s, %d \n", __FILE__, __LINE__  ) ;


						msgLoginResult.cResult = 4 ;
						if( pDSComm->m_pNetIO )
						{
							iRes = pDSComm->m_pNetIO->SendRequest(msgLoginResult.sLength, (char*)&msgLoginResult) ;
							if(iRes != 0){
								::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
								::PrintConsole( "[SEND_ERR] %s, %d (name:%s)\n", __FILE__, __LINE__, pMsgMSLogin->cCharacName ) ;
								pDSComm->DisConnect() ;
								continue ;
							}
						}


						continue ;
					}

					if( pMyInfo->IsLogon() ) {	// �̹� �α�����..

						pos = 10 ;


						::PrintConsole( "[LOGIN] Already loginer (%s/%s)\n", pMyInfo->GetName(), pMsgMSLogin->cCharacName ) ;
						// �γ�� ���´�..
						// �α� �������·� ��ȯ...
						pMyInfo->DisconnectForLogout() ;
						
						//pDSComm->DisConnect() ;				// ���� ���� ���� �׳� ���⸸..
						

						if( strcmp( pMyInfo->GetName(), pMsgMSLogin->cCharacName ) != 0 )
						{
							g_pMemberQueue->DeleteMap( pMsgMSLogin->cCharacName ) ;
							::PrintConsole( "[ERROR] if( strcmp( pMyInfo->GetName(), pMsgMSLogin->cCharacName ) != 0 ) \n" ) ;


							msgLoginResult.cResult = 4 ;
							if( pDSComm->m_pNetIO )
							{
								iRes = pDSComm->m_pNetIO->SendRequest(msgLoginResult.sLength, (char*)&msgLoginResult) ;
								if(iRes != 0){
									::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
									::PrintConsole( "[SEND_ERR] %s, %d (name:%s)\n", __FILE__, __LINE__, pMsgMSLogin->cCharacName ) ;
									pDSComm->DisConnect() ;
									continue ;
								}
							}

							
							continue ;
						}
						else
						{
							// �� ����̴�..
							pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
							pMyInfo->_BackupState.bMailListBackup = BACKUP_READY ;
							g_pBackupReady->Insert( pMyInfo ) ;
						}

						//continue ;
						
					}
					
					pos = 11 ;

					
					try{						
						pMyInfo->Login( pDSComm ) ;

						pos = 12 ;
						
						// �α����� ģ������ ã�� ������ �����Ѵ�..						
						pMyInfo->AllFriendsLink() ;
					}
					catch (...) {
						::PrintConsole("[EXCEPTION] \n") ;
					}
					

					pos = 14 ;
					
					// �α��� ���
					msgLoginResult.cResult = 0 ;
					if( pDSComm->m_pNetIO )
					{
						iRes = pDSComm->m_pNetIO->SendRequest(msgLoginResult.sLength, (char*)&msgLoginResult) ;
						if(iRes != 0){
							::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", iRes ) ;
							::PrintConsole( "[SEND_ERR] %s, %d (name:%s)\n", __FILE__, __LINE__, pMsgMSLogin->cCharacName ) ;
							pDSComm->DisConnect() ;
							continue ;
						}
					}
					
			
					pos = 15 ;

#ifdef _TRACE_
					g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Login Success (CharacName : %s) \n", pMsgMSLogin->cCharacName) ;					
					SYSTEMTIME	time ;
					GetLocalTime(&time ) ;

#ifdef _LOGIN_FILE_LOG_WRITE_
					g_pLoginLog->LogFileWrite("[%d:%d:%d][LOGIN_SUCCESS] CHR_NAME : %s \n", time.wHour, time.wMinute, time.wSecond, pMsgMSLogin->cCharacName ) ;					
#endif

#endif
				}
				catch (...) {										
					::PrintConsole("[EXCEPTION] case MMSG_NO_LOGIN_C_S : %d \n ", pos) ;
					if( pDSComm )
						pDSComm->DisConnect() ;

				}

				break;

			case MMSG_NO_GM_LOGIN :
				MMSG_GM_LOGIN * pMsgGmLogin ;
				pMsgGmLogin = (MMSG_GM_LOGIN*)pack->buf ;
				pMsgGmLogin->cCharacName[12] = 0 ;

				try{
					if( pMsgGmLogin->cCharacName[0] == 0 || pMsgGmLogin->cCharacName[0] ==' '){
						if( pDSComm )
							pDSComm->DisConnect() ;
						continue ;
					}
					
					
					
					pMyInfo = g_pMemberQueue->FindNode(pMsgGmLogin->cCharacName) ;				
					
					
					if( !pMyInfo ) {
					
						pMyInfo = g_pMemberQueue->GetNode(pMsgGmLogin->cCharacName) ;
						
						if( !pMyInfo ) {
							::PrintConsole("[ERROR] MMSG_NO_GM_LOGIN '!pMyInfo' \n") ;
							continue ;
						}
					}
					
					
					if( pMyInfo->IsLogon() ) {	// �̹� �α�����..
						
						// �γ�� ���´�..												
						//pMyInfo->Logout() ;
						pDSComm->DisConnect() ;
						//pMyInfo->ds_com = NULL ;

						// �� ����̴�..
						pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
						pMyInfo->_BackupState.bMailListBackup = BACKUP_READY ;
						g_pBackupReady->Insert( pMyInfo ) ;
						
						::PrintConsole("[ERROR] Login Failed 'Already login' \n") ;
						
						continue ;
					}					
					
					pMyInfo->Login( pDSComm ) ;
										

					::PrintConsole("[INFO] GM Login success (name : %s)\n ", pMsgGmLogin->cCharacName ) ;

				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_GM_LOGIN \n") ;
				}			


				break;



			case MMSG_NO_LOGOUT_C_S :

				MMSG_MSLOGOUT * pMsgMSLogout ;
				pMsgMSLogout = (MMSG_MSLOGOUT*)pack->buf ;

				pos = 0 ;


				try{
					
					if (pDSComm->m_pFriendInfo) {
						pMyInfo = (_FriendInfo*)pDSComm->m_pFriendInfo ;
					}
					else {
						pDSComm->DisConnect() ;
						
						// Goto First Loop				
						continue ;
					}

					if( pMyInfo == NULL )
					{
						continue ;
					}

#ifdef _TRACE_
					g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Recved 'MMSG_MSLOGOUT' Packet <%s> \n", pMyInfo->GetName() ) ;
#endif
					
						
					pos = 1 ;
					
					pMyFriendList = pMyInfo->GetFriendListPtr() ;
					if( pMyFriendList == NULL ) {
												
						g_pMemberQueue->PutNode(pMyInfo) ;
						
						// Goto First Loop				
						continue ;
					}
					
					pos = 2 ;
					
					//pMyInfo->Logout() ;						
					pDSComm->DisConnect() ;

					pos = 3 ;					
					
					// �� ����̴�..
					pMyInfo->_BackupState.bFrindListBackup = BACKUP_READY ;
					pMyInfo->_BackupState.bMailListBackup = BACKUP_READY ;
					g_pBackupReady->Insert( pMyInfo ) ;
					
#ifdef _TRACE_
					g_pTrace->OutputString(_TRACE_CLASS_MESSAGE, "[INFO] Logout success (%s) \n", pMyInfo->GetName()) ;
#endif
				}
				catch (...) {
					::PrintConsole("[EXCEPTION] case MMSG_NO_LOGOUT_C_S : pos.%d\n ", pos) ;
				}
				
				break;
								

			default :
					// ������� �Դٸ� ���������� ��Ŷ.. ���������� ��Ŷ�� ���� ������ ��������.. ��
					if(pDSComm) {
						::PrintConsole("[WORK_ERR] ������ ���� ��Ŷ ����/ ������ �������� \n" ) ;
						pDSComm->DisConnect() ;
					}
			
			
			} //switch
		}
		catch (...) {
			::PrintConsole("[EXCEPTION] th_LoginWorkThread \n") ;
		}
		
		
		
	} // while

	 
	::PrintConsole( "[WARNING] th_LoginWorkThread Exit \n") ;
	g_pSystemLog->LogFileWrite( "[WARNING] th_LoginWorkThread Exit \n") ;

	delete g_pDBCommand ;

	return 0 ;
}


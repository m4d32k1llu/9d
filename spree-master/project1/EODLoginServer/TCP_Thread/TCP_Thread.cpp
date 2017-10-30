#include "..\stdafx.h"
#include "..\Global\Global_Define.h"
#include "..\Global\Global_Function.h"
#include "..\VMManager\VMManager.h"
#include "..\Ole-DB\db_command.h"
#include "..\Packet_Queue\Packet_Queue.h"
//#include "..\DS_Com\DS_Com.h"
#include "..\MyServer\myServer.h"
#include "..\Billing\BillingSvrLink.h"
#include "../myServer/gGlobal.h"

#include "../RocWorks/RocAuthSession.h"

extern _h_ServerInfo *		g_pServerInfo ; 
//extern _h_DS_Com *			g_pDSCom ;
extern _queue_Packet *		g_pLSPacketQueue ;
extern _j_queue_UDP_Packet* g_pUDPSendPacketQ ;
extern _j_UserQueue *		g_pUserQueue ;
extern CDB_Connector	*	g_pDBCon ;

extern int nHeaderSize ;
extern void PrintConsole( const char *format,... ) ;
extern int SetVersionFile(char * fileName, int iVersion) ;
extern char g_cVersinFile[128] ;
extern bool	g_bContinue ;


extern bool bFreeLoginServer  ;
extern bool bTestLoginServer ;

extern u_int g_uiCompanyCode ;
extern int	 g_iUseAcclaimAuth ;

#ifdef _METEL_LOGIN_200608_
#include <list>
#include <map>
#include "../Odbc/ODBC_.h"

extern COdbc*	g_pDummyAccount;

extern std::list<METEL_BILLING_LOGIN*>			g_listMetelBillingLogin;
extern CRITICAL_SECTION							g_csMetelBillingLogin;
extern std::map<HANDLE, METEL_BILLING_INFO*>	g_mapMetelBillingInfo;
extern CRITICAL_SECTION							g_csMetelBillingInfo;
extern HANDLE									g_hMetelLoginProc;

BOOL IsPCBangUser(DWORD dwIp);
#endif //_METEL_LOGIN_200608_

#ifdef _METEL_LOGIN_200608_
void ReserveLogin(HANDLE hMetelBilling, char* loginID, char* loginPW, char* ip);
METEL_BILLING_INFO* GetBillingInfo(HANDLE hMetelBilling, METEL_BILLING_INFO* pBI);
void BackUpBillingInfo2User(_h_User* pUser, METEL_BILLING_INFO* pBI);
BOOL CheckPCBang(_h_User* pUser, char cForestSelectNum);
#endif //_METEL_LOGIN_200608_


#ifdef _PD_COMPANY_ACCLAIM_
#include "../SmallNet/Sock.h"

int		ConnectToAcclaim(CSock& sock);
int		AcclaimLoginAuth(CSock& sock, char* loginID, char* loginPW);
void	DisConnectFromAcclaim(CSock& sock);
#endif


DWORD WINAPI th_TCPWorkThread( LPVOID lp ) 
{
	COLE_DB_Executer * pDBExecutor = new COLE_DB_Executer(g_pDBCon) ;
	if( !pDBExecutor )
	{
		_endthreadex(0);
		return 0 ;
	}

	COLE_DB_Executer * pAcclaimDB = NULL;

	_Packet	*		pPacket ;		
	_h_User *		pUser ;	
	int				ret  ;
	char ip[17] ;

	char state ;
	char forest ;
	_sHeader * pHeader = NULL ;

	MSG_LOGIN * pMsgLogin = NULL ;
	MSG_LOGIN_FAIL	msgLoginFail = {0};
	msgLoginFail.Header.sLength = sizeof(MSG_LOGIN_FAIL);
	msgLoginFail.cMessage = MSG_NO_LOGIN_FAIL;	

	CND_LoginChecker *	pChecker = new CND_LoginChecker(g_uiCompanyCode) ; ;

	char	loginID[en_max_lil+1] ;
	char	loginPW[en_max_lpl+1] ;
	char	ndAccount[en_max_lil+1] ;		// ���� ���̵�. 
	short	nNDAccCount = 0 ;	// 
	int		iAccUid ;

	UMSG_LOGIN_LS_DS pkDSLogin = {0} ;		// DS�� ������ ��Ŷ ����Ÿ �̴�..
	pkDSLogin.iKey = PASS_KEY ;		
	pkDSLogin.cMessage = UMSG_LOGIN_LS_DS_NUM ;

	MSG_CMD_USERKILL msgCmdUserKill = {0} ;
	msgCmdUserKill.header.iKey = PASS_KEY ;
	msgCmdUserKill.header.cMessage = MSG_CMD_USERKILL_NUM ;
	sprintf( msgCmdUserKill.header.cGMName, "LOGIN" ) ;

	char sqlNameString[13] ={0} ;

	int	iPatchNum ;
	DWORD dwClientVer ;

#ifdef _PD_COMPANY_JAPAN_
	CRocAuthSession		rocSession;
	sRocAuthResponse *	pResponse = NULL;
#endif

#ifdef _METEL_LOGIN_200608_
	HANDLE				hMetelBilling = NULL;
	hMetelBilling = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif //_METEL_LOGIN_200608_

	while (g_bContinue) {	
	
		pPacket = g_pLSPacketQueue->get() ;
		if ( pPacket == NULL ) {			
			Sleep(20) ;
			continue ;
		}		

		pUser = pPacket->pUser ;
		if( pUser == NULL )
		{
			Sleep(20) ;
			continue ;
		}	

		ret = _LOGIN_ERROR_CODE_ETC ;

		ip_char(pUser->iIp, ip) ;

		if( pPacket->buf[2] == MSG_NO_GMLOGIN && bFreeLoginServer == false )
		{			
			::PrintConsole( "[WARNING] FREE LOGIN 'MSG_NO_GMLOGIN' IS ONLY FOR GM (from ip:%s)\n", ip) ;
			pPacket->pUser->DisConnect() ;
			continue ;
		}

		//============================================
		// Swiching message							 |
		//============================================
		
		// MSG_NO_LOGIN
		if ( pPacket->buf[2] == MSG_NO_LOGIN || pPacket->buf[2] == MSG_NO_GMLOGIN /*&& pUser->cStep == 1*/ )
		{
								
			pMsgLogin = reinterpret_cast<MSG_LOGIN *>( pPacket->buf ) ;
			
			// ����/ȸ�� �ڵ� �˻�
			//::PrintConsole("uiLMBC_Code=%d g_uiCompanyCode=%d \n", pMsgLogin->uiLMBC_Code, g_uiCompanyCode);
			if( pMsgLogin->uiLMBC_Code != g_uiCompanyCode )
			{
				::PrintConsole("pMsgLogin->uiLMBC_Code=%d g_uiCompanyCode=%d \n", pMsgLogin->uiLMBC_Code, g_uiCompanyCode);
				msgLoginFail.ucErrorCode = _LOGIN_ERROR_CODE_NOT_LMBC_CODE;

				if( pPacket->pUser->m_pNetIO )
					ret = pPacket->pUser->m_pNetIO->SendRequest((char*)&msgLoginFail, sizeof(MSG_LOGIN_FAIL)) ;				
				ret = 0;

				::PrintConsole("%d goto label_error \n", __LINE__); 
				goto label_error ;
			}

			g_pTrace->OutputString( _TRACE_CLASS_LOGIN,  "[Recv] Recv Login From IP %s\n", ip ) ;
							
			g_pServerInfo->GetVersion( iPatchNum, dwClientVer ) ;	// ���� ����.

			if( sizeof(MSG_LOGIN) == pMsgLogin->Header.sLength )
			{
				if( pMsgLogin->sVersionIndex != iPatchNum || pMsgLogin->dwClientVer != dwClientVer )
				{
					::PrintConsole("[ERROR] pMsgLogin->sVersionIndex=%d iPatchNum=%d pMsgLogin->dwClientVer=%d dwClientVer=%d",
						pMsgLogin->sVersionIndex, iPatchNum, pMsgLogin->dwClientVer, dwClientVer);

					msgLoginFail.ucErrorCode = _LOGIN_ERROR_CODE_BADVERSION;
					if( pPacket->pUser->m_pNetIO )
						ret = pPacket->pUser->m_pNetIO->SendRequest((char*)&msgLoginFail, sizeof(MSG_LOGIN_FAIL)) ;				
					ret = 0;

					goto label_error ;
				}
				else if( FALSE == pChecker->lc_CheckPakcet( g_uiCompanyCode, pMsgLogin->passport ) )
				{
					// Check Fail.
					PrintConsole( "  => lc_Check Fail\n" ) ;
					msgLoginFail.ucErrorCode = _LOGIN_ERROR_CODE_NOT_LMBC_CODE;
					if( pPacket->pUser->m_pNetIO )
						ret = pPacket->pUser->m_pNetIO->SendRequest((char*)&msgLoginFail, sizeof(MSG_LOGIN_FAIL)) ;				
					ret = 0;
					goto label_error ;
				}

				strncpy( loginID, pChecker->lc_GetID(), en_max_lil+1 ) ;
				strncpy( loginPW, pChecker->lc_GetPW(), en_max_lpl+1 ) ;

				g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "[Mid Check] Pass Passport : A<%s>\n", loginID ) ;
			}
			else
			{
				PrintConsole( "  ==> Size Mismatch \n" );
				msgLoginFail.ucErrorCode = _LOGIN_ERROR_CODE_NOT_LMBC_CODE;
				if( pPacket->pUser->m_pNetIO )
					ret = pPacket->pUser->m_pNetIO->SendRequest((char*)&msgLoginFail, sizeof(MSG_LOGIN_FAIL)) ;				
				ret = 0;
				goto label_error ;
			}
			
		
			//4. 5. ID, PW Check
			try {
				if( pPacket->buf[2] == MSG_NO_LOGIN )
				{	
					forest = pMsgLogin->cForestSelectNum;

					switch( g_uiCompanyCode )
					{
					case en_lmbc_nexon:
						iAccUid = 0 ;
						ZeroMemory( ndAccount, sizeof(ndAccount) ) ;
						ret = pDBExecutor->NexonLogin( IN OUT loginID, ndAccount, &pUser->cClass , &state, &forest, &nNDAccCount, &iAccUid ) ;
						if( iAccUid == 0 )
							ret = -1 ;
						break;
#ifdef _PD_COMPANY_METEL_
#ifdef _METEL_LOGIN_200608_
					case en_lmbc_metel:
						{
							// ret = 0 �̸�, ����
#ifdef _PD_INNER_TEST_

							memcpy(pUser->cID, loginID, en_max_lil+1);
							
							//ret = pDBExecutor->Login( loginID, loginPW, &pUser->cClass, &state, &forest ) ;
							ret = pDBExecutor->MetelLogin(loginID, &pUser->cClass, &state, &forest);
							::PrintConsole("MetelLoginRet=%d, loginID=%s, cClass=%d, state=%d, forest=%d \n",
								ret, loginID, pUser->cClass, state, forest);
							strncpy( ndAccount, loginID, en_max_lil ) ;
							ndAccount[en_max_lil] = 0 ;

#else	// _PD_INNER_TEST_

							METEL_BILLING_INFO* pBI = NULL;
							
							memcpy(pUser->cID, loginID, en_max_lil+1);
							ReserveLogin(hMetelBilling, loginID, loginPW, ip);

							if(WAIT_TIMEOUT == WaitForSingleObject(hMetelBilling, INFINITE)) {
							//if(WAIT_TIMEOUT == WaitForSingleObject(hMetelBilling, 16000)) {
								::PrintConsole("(%d) TIMEOUT(hMetelBilling) id=%s \n", __LINE__, loginID);
								ret = _LOGIN_ERROR_CODE_NOT_PCBANG_USER;
								::PrintConsole("%d goto label_error \n", __LINE__); goto label_error ;
							}
							else {
								pBI = GetBillingInfo(hMetelBilling, pBI);
								BackUpBillingInfo2User(pUser, pBI); // ���� ������ ���� ��ü�� ���
							}

							// Billing API ����� ���� ó��
							if(pBI->iCallbackResult == 0) {
								// ���� ����
								::PrintConsole("Success to Billing. iCallbackRes=%d id=%s pass=%s \n", pBI->iCallbackResult, loginID, loginPW, ip);

								// ���� ������ ���� ��ü�� ���
								//BackUpBillingInfo2User(pUser, pBI);

								// PC�� �˻�
								if(!CheckPCBang(pUser, pMsgLogin->cForestSelectNum)) {
									SAFE_DELETE(pBI);
									
									// PC�� ������ �ƴϸ鼭, PC�� ������ �����Ϸ��� ����
									ret = _LOGIN_ERROR_CODE_NOT_PCBANG_USER;
									::PrintConsole("%d goto label_error \n", __LINE__); goto label_error ;
								}

								//ret = pDBExecutor->Login( loginID, loginPW, &pUser->cClass, &state, &forest ) ;
								ret = pDBExecutor->MetelLogin(loginID, &pUser->cClass, &state, &forest);
								::PrintConsole("MetelLoginRet=%d, loginID=%s, cClass=%d, state=%d, forest=%d \n",
									ret, loginID, pUser->cClass, state, forest);
								strncpy( ndAccount, loginID, en_max_lil ) ;
								ndAccount[en_max_lil] = 0 ;
							}
							else {
								switch(pBI->iCallbackResult) {
								case _LOGIN_ERROR_CODE_METEL_UNKNOWN:
									pBI->iCallbackResult = _LOGIN_ERROR_CODE_ETC;
									//break;
								case _LOGIN_ERROR_CODE_METEL_E50: // 39
								case _LOGIN_ERROR_CODE_METEL_E51:
								case _LOGIN_ERROR_CODE_METEL_E52:
								case _LOGIN_ERROR_CODE_METEL_E53:
								case _LOGIN_ERROR_CODE_METEL_E54:
								case _LOGIN_ERROR_CODE_METEL_E55:
								case _LOGIN_ERROR_CODE_METEL_E56:
								case _LOGIN_ERROR_CODE_METEL_E57:
									ret = pBI->iCallbackResult;
									::PrintConsole("Fail to Billing, _LOGIN_ERROR_CODE_=%d \n", ret);
									break;
								default:
									pBI->iCallbackResult = _LOGIN_ERROR_CODE_ETC;
									ret = pBI->iCallbackResult;
									::PrintConsole("Fail to Billing, _LOGIN_ERROR_CODE_=%d \n", ret);
									break;
								}
								SAFE_DELETE(pBI);
								::PrintConsole("%d goto label_error \n", __LINE__); goto label_error ;
							}
							SAFE_DELETE(pBI);

#endif // _PD_INNER_TEST_
						}
						break;
#endif // _METEL_LOGIN_200608_
#endif
						
					case en_lmbc_acclaim:
#ifdef _PD_COMPANY_ACCLAIM_
						{

#ifdef _PD_INNER_TEST_

							// login_success
							ret = pDBExecutor->AcclaimLogin( loginID, &pUser->cClass, &state, &forest ) ;
							strncpy( ndAccount, loginID, en_max_lil ) ;
							ndAccount[en_max_lil] = 0 ;

#else	// _PD_INNER_TEST_


							CSock	sock;

							// connect to acclaim
							ret = ConnectToAcclaim(sock);
							if(ret != 0) {
								::PrintConsole("%d goto label_error \n", __LINE__); goto label_error ;
							}
#ifdef _PD_ACCLAIM_LOGIN_200612_LOG
							else {
								Logout("success to connect to acclaim");
								::PrintConsole("success to connect to acclaim \n");
							}
#endif

							// make a login_req packet with loginID & loginPW which are already known
							ret = AcclaimLoginAuth(sock, loginID, loginPW);
							if(ret != 0) {
								::PrintConsole("%d goto label_error \n", __LINE__); goto label_error ;
							}
#ifdef _PD_ACCLAIM_LOGIN_200612_LOG
							else {
								Logout("success to login");
								::PrintConsole("success to login \n");
							}
#endif

							// disconnect from acclaim
							DisConnectFromAcclaim(sock);
#ifdef _PD_ACCLAIM_LOGIN_200612_LOG
							Logout("success to disconnect from acclaim");
							::PrintConsole("success to disconnect from acclaim \n");
#endif

							// login_success
							ret = pDBExecutor->AcclaimLogin( loginID, &pUser->cClass, &state, &forest ) ;
							strncpy( ndAccount, loginID, en_max_lil ) ;
							ndAccount[en_max_lil] = 0 ;
#endif	// _PD_INNER_TEST_

						}

#endif // _PD_COMPANY_ACCLAIM_

						break;

					case en_lmbc_pwc:
						ret = pDBExecutor->Login( loginID, loginPW, &pUser->cClass, &state, &forest ) ;
						strncpy( ndAccount, loginID, en_max_lil ) ;
						ndAccount[en_max_lil] = 0 ;
						break;
						
					case en_lmbc_russia:
						ret = pDBExecutor->Login( loginID, loginPW, &pUser->cClass, &state, &forest ) ;
						strncpy( ndAccount, loginID, en_max_lil ) ;
						ndAccount[en_max_lil] = 0 ;
						break;
						
					case en_lmbc_rocworks:
#ifdef _PD_COMPANY_JAPAN_
						if( rocSession.IsAuthReady() )
						{
							
							try
							{
								if( ND_F_SUCCESS != rocSession.RocRequestLogin( loginID, loginPW, &pResponse ) )
								{
									ret = _LOGIN_DB_RETURN__NOT_FOUND_;
								}
								else 
								{
									if( false == pResponse->IsLoginSuccess() )
									{
										ret = _LOGIN_DB_RETURN__NOT_MATCH_PASSWORD;
										PrintConsole( "[RMS] A<%s> Login failed\n", loginID );
									}
									else
									{
										ret = pDBExecutor->RocLogin( loginID, loginPW, &pUser->cClass, &state, &forest, rocSession.IsAuthReady() ) ;
										PrintConsole( "[RMS] A<%s> Success and DB_Proc Result = %d\n", loginID, ret );
									}
								}
							}
							catch( ... )
							{
								PrintConsole( "[RMS] Exception in Login Process : Code %d\n", GetLastError() );
								ret = _LOGIN_DB_RETURN__ERROR_;
							}

							rocSession.Disconnect();
						}
						else
							ret = pDBExecutor->RocLogin( loginID, loginPW, &pUser->cClass, &state, &forest, rocSession.IsAuthReady() ) ;

						strncpy( ndAccount, loginID, en_max_lil ) ;
						ndAccount[en_max_lil] = 0 ;
#endif
						break;

					case en_lmbc_vina:
						ret = pDBExecutor->Login( loginID, loginPW, &pUser->cClass, &state, &forest ) ;
						strncpy( ndAccount, loginID, en_max_lil ) ;
						ndAccount[en_max_lil] = 0 ;
						break;

					default:
						PrintConsole( "[LOGIN SERVER] System Error : Company Code Error\n" ) ;
						ret = -1 ;
					}
					
					g_pTrace->OutputString( _TRACE_CLASS_LOGIN,  "[DB Login] Result %d\n", ret );
				}
				else if( pPacket->buf[2] == MSG_NO_GMLOGIN )
				{
					//if( strncmp( ip, "221.147.34.", 11 ) != 0 )
					//{
					//	::PrintConsole("[WARNING] BAD IP CONNECT (ACC:%s PW:%s FROM_IP:%s ) \n", pMsgLogin->cID, pMsgLogin->cPassword, ip ) ;
					//	continue ;
					//}
					::PrintConsole("[GM_LOGIN] ACC:%s FROM_IP:%s \n", loginID, ip ) ;
					switch( g_uiCompanyCode )
					{
					case en_lmbc_nexon:
						ret = pDBExecutor->NexonGMLogin( loginID, loginPW, ndAccount, &pUser->cClass, &state, &forest ) ;
						break;
					case en_lmbc_metel:
						ret = pDBExecutor->GMLogin( loginID, loginPW, &pUser->cClass, &state, &forest ) ;
						strncpy( ndAccount, loginID, en_max_lil+1 ) ;
						break;
					case en_lmbc_vina:
						ret = pDBExecutor->GMLogin( loginID, loginPW, &pUser->cClass, &state, &forest ) ;
						strncpy( ndAccount, loginID, en_max_lil+1 ) ;
						break;
					case en_lmbc_rocworks:
						ret = pDBExecutor->GMLogin( loginID, loginPW, &pUser->cClass, &state, &forest ) ;
						strncpy( ndAccount, loginID, en_max_lil+1 ) ;
						break;
					case en_lmbc_acclaim:
						ret = pDBExecutor->GMLogin( loginID, loginPW, &pUser->cClass, &state, &forest ) ;
						strncpy( ndAccount, loginID, en_max_lil+1 ) ;
						break;
					default:
						PrintConsole( "[LOGIN SERVER] System Error : Company Code Error\n" ) ;
						ret = -1 ;
					}
					
					// GM �α����� ���ڴ� ������ ������ ������Ű�� ���ؼ�..
					pUser->cClass = _CLASS_BLOCK_INSIDER_SEC ;
				}
				else 
				{
					g_pTrace->OutputString( _TRACE_CLASS_ERROR,  "[Error] Bad Packet No\n" );
				}

			}
			catch ( ... ) {						
				g_pErrMsg->LinePrint( HTML("[EXCEPTION] DB-Call LoginPro\n", _ns_trace_::_FONT_COLOR_RED, _BLINK_ ) ) ;				
				ret = _LOGIN_ERROR_CODE_ETC ;
			}

			/*=CLASS �˻�=============================================================
			// DB Result �� �� CLASS �˻�..
			// Result �� 0 �̸� 1�� ����
			// CLASS �� ���� �α��� �������� 2�� ����..
			========================================================================*/

			g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  => A<%s> DB Login Pass : Ret = %d, Class = %d\n", loginID, ret, pUser->cClass ) ;

			switch( pUser->cClass ) 
			{			
			case _CLASS_BLOCK_TAKING_ : // �з�����
				ret = _LOGIN_ERROR_CODE_BLOCK ;
				break;
			case _CLASS_BLOCK_OFF_ :	// ��������
				ret = _LOGIN_ERROR_CODE_BLOCK ;
				break;
			case _CLASS_BLOCK_STOP_ :	// �Ͻ���������
				ret = _LOGIN_ERROR_CODE_BLOCK ;
				break;
			case _CLASS_BLOCK_TEEN_ :	// ��������
				ret =  _LOGIN_ERROR_CODE_TEEN_USER ;					
				break;				
			case _CLASS_BLOCK_TESTER_ONLY :	// �׼�����ONLY
				ret = _LOGIN_ERROR_CODE_BLOCK ;
				break;	
			}
			
			/*
			if( _LOGIN_ERROR_CODE_BLOCK == ret || _LOGIN_ERROR_CODE_TEEN_USER == ret )
			{
				g_pTrace->OutputString( _TRACE_CLASS_ERROR, "[Error] Login failed (%d)\n", ret );
				goto label_error;
			}
			*/

			if( ret == 0 )	// ���� 
			{
				//#define _CLASS_BLOCK_NOTHING_				0		// �������� ���� ����
				//#define _CLASS_BLOCK_TAKING_				1		// �з�����
				//#define _CLASS_BLOCK_OFF_					2		// ������ ����
				//#define _CLASS_BLOCK_STOP_					3		// �Ͻ� ������ ����
				//#define _CLASS_BLOCK_TESTER_ONLY			5		// �׽�Ʈ ������ ���� ����
				//#define _CLASS_BLOCK_GENERAL				12		// �⺻���� �����
				//#define _CLASS_BLOCK_SUPPORTER				18		// ���»� ��������
				//#define _CLASS_BLOCK_INSIDER_SEC			19		// ���� �ο� ����
				//#define _CLASS_BLOCK_INSIDER_FIR			20		// ���� �ο� ����
				switch( pUser->cClass ) {
				case _CLASS_BLOCK_NOTHING_ :
					ret = _LOGIN_ERROR_CODE_BADUSER ;
					break;
				case _CLASS_BLOCK_TAKING_ :
					ret = _LOGIN_ERROR_CODE_BLOCK ;
					break;
				case _CLASS_BLOCK_OFF_ :
					ret = _LOGIN_ERROR_CODE_BLOCK ;
					break;
				case _CLASS_BLOCK_STOP_ :
					ret = _LOGIN_ERROR_CODE_BLOCK ;
					break;				
				case _CLASS_BLOCK_TESTER_ONLY :
					ret = _LOGIN_ERROR_CODE_BLOCK ;
					break;				
				case _CLASS_BLOCK_GENERAL :
					break;				
				case _CLASS_BLOCK_SUPPORTER :
					break;
				case _CLASS_BLOCK_INSIDER_SEC :
					break;
				case _CLASS_BLOCK_INSIDER_FIR :
					break;
				default:
					ret = _LOGIN_ERROR_CODE_BADUSER ;
					break;
				}
			} // if( ret == 0 )	
			else if( ret > _LOGIN_DB_RETURN__BLOCKING_CASE )
			{
				g_pTrace->OutputString( _TRACE_CLASS_LOGIN,  "[FAILED] This is blocked : <%s> , selectForestNum : %d \n", loginID, pMsgLogin->cForestSelectNum ) ;
			}
			else 
			{
				switch( ret ) 
				{
				case _LOGIN_DB_RETURN__NOT_FOUND_:				// ���ϵ��� ���� ����
					ret = _LOGIN_ERROR_CODE_NO_USER ;
					break;
				case _LOGIN_DB_RETURN__NOT_MATCH_PASSWORD:				// ��ȣ Ʋ��
					ret = _LOGIN_ERROR_CODE_BADPASSWORD ;
					break;
				case _LOGIN_DB_RETURN__AREADY_LOGINER:				// �̹� �α�����..					
					// �ش� �� DS �� ���� ���� ��û.
					{						
						memcpy( msgCmdUserKill.cAccount, loginID, sizeof(loginID) ) ;
						::PrintConsole( "[INFO] Already login (%s)\n", msgCmdUserKill.cAccount ) ;

						list<_sDSAddress*>::iterator itr = g_pServerInfo->m_lstSockAddr.begin();
						while( itr != g_pServerInfo->m_lstSockAddr.end() )
						{
							if( (*itr)->group != pMsgLogin->cForestSelectNum )
								g_pUDPSendPacketQ->insert( &(*itr)->addr, (char*)&msgCmdUserKill, sizeof(MSG_CMD_USERKILL) ) ;

							itr++;
						}

						ret = _LOGIN_ERROR_CODE_ALREADY ;

						pDBExecutor->DSLogout( loginID );		// 1/11 �� ����.
					}
					break;

				case _LOGIN_DB_RETURN__NOT_UNI_NEXON:
					g_pTrace->OutputString( _TRACE_CLASS_ERROR, "[Error] Not uni nexon user try to connect (acc:%s)\n", loginID );	
					ret = _LOGIN_ERROR_CODE_NOT_UNI_NEXON ;
					break;

				case _LOGIN_DB_RETURN__BLOCKING_USER :				// ��ŷ ���� ����
					g_pTrace->OutputString( _TRACE_CLASS_ERROR, "[Error] Blocking user try to connect (acc:%s)\n", loginID );	
					ret =  _LOGIN_ERROR_CODE_BLOCK ;					
					break;

				case _LOGIN_DB_RETURN__ERROR_:
					g_pTrace->OutputString( _TRACE_CLASS_ERROR, "[Error] DB Error (acc:%s)\n", loginID );	
					ret = _LOGIN_ERROR_CODE_LOGIN_FAIL;
					break;
					
				case -1:
					g_pTrace->OutputString( _TRACE_CLASS_ERROR, "[Error] DB Error (acc:%s)\n", loginID );	
					ret = _LOGIN_ERROR_CODE_NO_USER;
					break;
				default :
					ret = _LOGIN_ERROR_CODE_ETC ;
					break ;
				}				

			}

			// ret != 0 �̸� ����..
			if( ret != 0 )
			{
				g_pTrace->OutputString( _TRACE_CLASS_ERROR, "[Error] Login failed (%d)\n", ret );
				goto label_error;
			}
			else
			{
				memcpy(pUser->cID, ndAccount, en_max_lil+1) ;	// ID ����..
				pUser->cSelectForest = pMsgLogin->cForestSelectNum ;

				memcpy( msgCmdUserKill.cAccount, loginID, sizeof(loginID) ) ;

				list<_sDSAddress*>::iterator itr = g_pServerInfo->m_lstSockAddr.begin();
				while( itr != g_pServerInfo->m_lstSockAddr.end() )
				{
					if( (*itr)->group != pMsgLogin->cForestSelectNum )
						g_pUDPSendPacketQ->insert( &(*itr)->addr, (char*)&msgCmdUserKill, sizeof(MSG_CMD_USERKILL) ) ;

					itr++;
				}
			}
			
			
			/*====================================================
			// �ش� ��������  DS���°� ON ���� Ȯ���Ѵ�.  
			=====================================================*/			
			switch( g_pServerInfo->GetForestState( pMsgLogin->cForestSelectNum ) ) 
			{
			case _FOREST_STATE_OFF:
				ret = _LOGIN_ERROR_CODE_OFF ;
				break;
			case _FOREST_STATE_CHECKUP_:
				// ������ �������϶��� ���� ����ڰ� �ƴϸ� �� ������ �Ѵ�.
				if( ( _CLASS_BLOCK_INSIDER_SEC != pUser->cClass ) && ( _CLASS_BLOCK_INSIDER_FIR != pUser->cClass ) )
				{
					ret = _LOGIN_ERROR_CODE_CHECKUP ;
				}				
				break;
			case _FOREST_STATE_FULL_:
				if( ( _CLASS_BLOCK_INSIDER_SEC != pUser->cClass ) && ( _CLASS_BLOCK_INSIDER_FIR != pUser->cClass ) )
				{
					ret = _LOGIN_ERROR_CODE_OVERTIME ;
				}				
				break;				
			}
			//====================================================
			// ������ ����Ǿ� ���� �ʴٸ�.. ���� ������ ��ŵ�ϰ� DS�������� �Ѿ��.
			//=======================================================================
			//  ������ʹ� DS �� ����..
			pkDSLogin.cServerNum = pMsgLogin->cForestSelectNum ;
			memcpy( pkDSLogin.cAccount, ndAccount, en_max_lil+1 ) ;	
#ifdef _NXPCB_AWARD_200703_
			memcpy( pkDSLogin.cNXAcc, loginID, en_max_lil+1 ) ;	
#endif
			memcpy( pkDSLogin.cPass, loginPW, en_max_lpl+1 ) ;	
			pkDSLogin.iAccUid = iAccUid ;

			pkDSLogin.iTempID = pUser->get_id() ;
			pkDSLogin.iIp = pUser->iIp  ;
			pkDSLogin.iTime = GetTime_ByIntSec() ;
			pkDSLogin.cClass = pUser->cClass ;	
			
#ifdef _METEL_LOGIN_200608_
			pUser->bPCBang > 0 ? pkDSLogin.bPCBang = true : pkDSLogin.bPCBang = false;
			memcpy(&pkDSLogin.MetelBillingInfo, &pUser->MetelBillingInfo, sizeof(METEL_BILLING_INFO));
#endif //_METEL_LOGIN_200608_

#ifdef _PD_VINA_PCROOM_INFO_
			pkDSLogin.ucPCRoomInfo = pDBExecutor->VinaGetPCRommInfo( ip );
#endif

#ifdef _PD_VINA_PCROOM_INFO_
			g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  TO DS <%s> UID %d PCR(%u:IP %s) Send Loing Success\n", ndAccount, pUser->get_id(), pkDSLogin.ucPCRoomInfo, ip ) ;
			Logout( "TO DS <%s>, ND<%s> UID %d PCR(%u:IP %s) Send Loing Success", loginID, ndAccount, pUser->get_id(), pkDSLogin.ucPCRoomInfo, ip ) ;
#else
			g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  TO DS <%s>, ND<%s> UID %d Send To %d Loing Success\n", loginID, ndAccount, pUser->get_id(), pMsgLogin->cForestSelectNum ) ;
#endif

			SOCKADDR_IN * pAddr = g_pServerInfo->GetDsSockAddr_In( pMsgLogin->cForestSelectNum );

			if ( pAddr == NULL ) {
				ret = _LOGIN_ERROR_CODE_ETC ;
			}		
						
			g_pUDPSendPacketQ->insert( pAddr, (char*)&pkDSLogin, sizeof(pkDSLogin) ) ;			
			g_pUserQueue->InsertStandbyMap( pUser ) ;	// DS���� ���� ����� ������ �ش� ������ ã�� ���� �ʿ� ���..
			
		} //if ( pBuf[2] == MSG_NO_LOGIN )
		
		// ��ġ ������ �����Ѵ�.
		else if( pPacket->buf[2] == MSG_NO_SET_VERSION )
		{
			MSG_SET_VERSION * pMsgSetVersion = (MSG_SET_VERSION*)pPacket->buf ;
			if( pMsgSetVersion->Header.sLength == sizeof(MSG_SET_VERSION) )
			{

				g_pServerInfo->GetVersion( iPatchNum, dwClientVer ) ;

				if( (u_short)iPatchNum + 1 == pMsgSetVersion->usVersion ||
					(u_short)iPatchNum - 1 == pMsgSetVersion->usVersion ||					
					dwClientVer + 1 == pMsgSetVersion->dwClientVer ||
					dwClientVer - 1 == pMsgSetVersion->dwClientVer )
				{
					iPatchNum = pMsgSetVersion->usVersion ;					
					pDBExecutor->VersionControl( iPatchNum, pMsgSetVersion->dwClientVer, 1, pMsgSetVersion->cIsTest ) ;

					::PrintConsole("[SETVERSION] Set Version(%d, %d) \n", iPatchNum, pMsgSetVersion->dwClientVer ) ;
					printf("[SETVERSION] Set Version(%d, %d) \n", iPatchNum, pMsgSetVersion->dwClientVer ) ;
				}
				else
				{
					::PrintConsole( "[SETVERSION_FAILED] VERSION INFO ( S: %d, %d  T:%d, %d )  \n"
						, iPatchNum, dwClientVer, pMsgSetVersion->usVersion, pMsgSetVersion->dwClientVer ) ;
				}
			}			
		}

		else {		// ����� ���� �ҷ���Ŷ�̰ų� ���۵ȵ� ��Ŷ�� ���ɼ��� �ִ�..  �ٷ� �����..
			::PrintConsole("[ERROR] �ҷ���Ŷ (%d) \n", pPacket->buf[2] ) ;
			if( pUser ){
				pUser->DisConnect() ;
			}
		}

		
		continue ;	// GOTO : while( pTCPPacket ) 


label_error:	// error �߻���..
		::PrintConsole("goto label_error; Error while is trying to login ret=%d \n", ret);
		Logout("goto label_error : A<%s> ret=%d \n",  ret);
		if( ret != 0 ) 
		{
			msgLoginFail.ucErrorCode = ret ;

			if( pUser->m_pNetIO )
				ret = pUser->m_pNetIO->SendRequest((char*)&msgLoginFail, sizeof(MSG_LOGIN_FAIL)) ;				
			if( ret != 0 ) {						
				::PrintConsole("[ERROR] Send Request failed (%d)\n", WSAGetLastError()) ;				
			}
		}
		
		if( pUser ){
			pUser->DisConnect() ;
		}
		
	} // while(1)		


	delete pDBExecutor ;
#ifdef _METEL_LOGIN_200608_
	/*
	DeleteCriticalSection(&g_csMetelBillingLogin);
	DeleteCriticalSection(&g_csMetelBillingInfo);

	if(g_pDummyAccount != NULL) {
		g_pDummyAccount->Disconnect();
		delete g_pDummyAccount;
		g_pDummyAccount = NULL;
	}
	*/
#endif //_METEL_LOGIN_200608_
	printf( "[EXIT] th_TCPWorkThread exit \n" ) ;
	return 0 ;
}


//////////////////////////////////////////////////////////////////////////
#ifdef _METEL_LOGIN_200608_
void ReserveLogin(HANDLE hMetelBilling, char* loginID, char* loginPW, char* ip) {
	METEL_BILLING_LOGIN* pBL = new METEL_BILLING_LOGIN;
	METEL_BILLING_INFO* pBI = NULL;
	
	pBL->h = hMetelBilling;
	lstrcpy(pBL->pszId, loginID);
	lstrcpy(pBL->pszPass, loginPW);
	lstrcpy(pBL->pszIp, ip);
	EnterCriticalSection(&g_csMetelBillingLogin);
	g_listMetelBillingLogin.push_back(pBL);
	LeaveCriticalSection(&g_csMetelBillingLogin);
	SetEvent(g_hMetelLoginProc);
}

METEL_BILLING_INFO* GetBillingInfo(HANDLE hMetelBilling, METEL_BILLING_INFO* pBI) {
	EnterCriticalSection(&g_csMetelBillingInfo);
	pBI = g_mapMetelBillingInfo[hMetelBilling];
	g_mapMetelBillingInfo.erase(hMetelBilling);
	LeaveCriticalSection(&g_csMetelBillingInfo);
	return pBI;
}

void BackUpBillingInfo2User(_h_User* pUser, METEL_BILLING_INFO* pBI) {
	memcpy(&pUser->MetelBillingInfo, pBI, sizeof(METEL_BILLING_INFO));
}

BOOL CheckPCBang(_h_User* pUser, char cForestSelectNum) {
	// PC�� �����̸�, � ������ �����ϵ� �������,
	// PC�� ������ �ƴ� ��쿡��, ���� ���� ��ȿ�� �˻�
	pUser->bPCBang = IsPCBangUser((DWORD)pUser->iIp);
	if(pUser->bPCBang == FALSE) { // �Ǿ��� ���� �ƴ�
		switch(cForestSelectNum) {
		case -1: // �Ǿ��� ����_0
		//case 3:	// ���� ����( �Ǿ��� ���� ������ )
			// ���� ����
			return FALSE;
		default: // �Ǿ��� ���� �ƴ�
			// ok
			return TRUE;
		}
	}
	return TRUE;
}

#endif //_METEL_LOGIN_200608_

//////////////////////////////////////////////////////////////////////////
#ifdef _PD_COMPANY_ACCLAIM_

int ConnectToAcclaim(CSock& sock) {
	char	ip[IPV4_BUF_SIZE] = {0};
	const unsigned short	port = 8221;

	if(!sock.CreateTcpSocket()) {
		Logout("Acclaim:Login fail to create socket");
		return _LOGIN_ERROR_CODE_ETC;
	}
	Domain2StringIP("sysapp.acclaim.com", ip, IPV4_BUF_SIZE);
	if(!sock.Connect(ip, port)) {
		Logout("Acclaim:Login fail to connect to acclaim's server(%s:%d)", ip, WSAGetLastError() );
		return _LOGIN_ERROR_CODE_ETC;
	}

	return 0;
}

int AcclaimLoginAuth(CSock& sock, char* loginID, char* loginPW) {
	int		ret = 0;

	const int	iSendPktSize = 66;
	const int	iHeaderSize = 6;
	const int	iIDSize = 20;
	const int	iPWSize = 40;
	char	p[256] = {0};
	int		offset = 0;

	const int	iRetCodeSize = 2;
	const int	iRetMsgSize = 79;
	char	pRetCode[iRetCodeSize+1] = {0};
	char	pRetMsg[iRetMsgSize+1] = {0};
	
	for(int i = 0; i < iSendPktSize; ++i) p[i] = '*';
	memcpy(p+offset, "01999*", iHeaderSize); offset += iHeaderSize;
	memcpy(p+offset, loginID, strlen(loginID)); offset += iIDSize;
	memcpy(p+offset, loginPW, strlen(loginPW));

	// send a login_req packet to acclaim
	int iSentBytes = sock.Send(p, iSendPktSize);
	if(iSentBytes != iSendPktSize) {
		Logout("Acclaim:Login fail to send a login_auth packet");
		return _LOGIN_ERROR_CODE_ETC;
	}

	// recv a result packet from accliam
	memset(p, 0, 256);
	int iRecvedBytes = sock.Recv(p, 256);
	if(iRecvedBytes == 0) {
		Logout("Acclaim:Login fail to recv a return packet of login_auth from acclaim");
		return _LOGIN_ERROR_CODE_ETC;
	}

	// check a ret_val of AcclaimLoginAuth()
	offset = 0;
	memset(pRetCode, 0, iRetCodeSize+1);
	memset(pRetMsg, 0, iRetMsgSize+1);
	memcpy(pRetCode, p+offset, iRetCodeSize); offset += iRetCodeSize;
	memcpy(pRetMsg, p+offset, iRetMsgSize);
	if(0 != strcmp("00", pRetCode)) {
		// login_fail
		if(0 == strcmp("01", pRetCode)) {
			// wrong username
			Logout("Acclaim:Login wrong username");
			ret = _LOGIN_ERROR_CODE_NO_USER;
		}
		else if(0 == strcmp("02", pRetCode)) {
			// wrong password
			Logout("Acclaim:Login wrong password");
			ret = _LOGIN_ERROR_CODE_BADPASSWORD;
		}
		else if(0 == strcmp("03", pRetCode)) {
			// blocked user
			Logout("Acclaim:Login block user");
			ret = _LOGIN_ERROR_CODE_BADUSER;
		}
		else if(0 == strcmp("04", pRetCode)) {
			// pending user
			Logout("Acclaim:Login pending user");
			ret = _LOGIN_ERROR_CODE_ETC;
		}
		else if(0 == strcmp("09", pRetCode)) {
			// system error
			Logout("Acclaim:Login system error");
			ret = _LOGIN_ERROR_CODE_ETC;
		}
		else {
			// etc. error
			Logout("Acclaim:Login etc. error");
			ret = _LOGIN_ERROR_CODE_ETC;
		}
		return ret;
	}

	return 0;
}

void DisConnectFromAcclaim(CSock& sock) {
	sock.CloseSocket();
	return;
}
							
#endif
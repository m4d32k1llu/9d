#include "..\stdafx.h"

#include "..\MAPManager\MAPManager.h"
#include "..\Network\Network.h"
#include "..\Network\LogSvrLink.h"
#include "..\Global\Global_UniqueID.h"
#include "..\Global\Global_ServerInfo.h"
#include "..\Global\tcp_packet.h"
#include "..\Global\tcp_packets_party.h"
#include "..\Billing\BillingSvrLink.h"
#include "..\CS\party_manager.h"
#include "../Cs/Castle.h"
#include "../cs/MasterPupil.h"



extern _j_AccMAPManager *	g_pAccMapManager;		// Account ID KEY MAP
extern _j_ServerInfo		g_ServerInfo;			// ��������
extern _DSCom_Queue *		g_pDSComQueue ;
extern COrganizationManager *	g_pOrganizationManager ;
extern _TempEvent event[12] ;

extern u_int					g_uiCompanyCode ;	
// Backup ��� ��ü
extern _j_BackupReady *		g_pBackupReady ;

// Uinqueue Generater
extern _j_UniqueIDQueue *		g_pUniqueIDQueue ;

// udp send queue
extern _j_queue_UDP_Packet *	g_pUDPSendPacketQ ;

// mem save 
extern CUserMemSaveTimer *		g_pMemSaveTimer ;

// S2S log
//extern CTQueue<_CharacLog> * g_pCharacLogQueue ;
extern CLogSvr_Com *			g_pLogSvrCom ;

extern _j_ServerInfo *			g_pServerInfo ;

extern u_int					g_serverStateFlag ;


#ifdef _PD_MASTER_PUPIL_
extern CDSMasterSet *			g_pMPSet;
#endif


#ifdef	_NXPCB_AWARD_200703_
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "../Mylibrary/gGlobal.h"
#include "../SmallNet/Packet.h"
#include "../SmallNet/PktMgr.h"
#include "../SmallNet/PktQue.h"
#include "../SmallNet/Sock.h"
#include "../SmallNet/SmallNet.h"
#include "../SmallNet/PktProc/_PktDef.h"
#include "../SmallNet/PktProc/Nexon_PCBangAuth.h"

//extern CSock	gNexonPCB_sock;
//extern CS		gNexonPCB_cs;

//extern std::map<string/*strAccount*/, HANDLE /*hThreadSync*/>	gmapNXPCB_LoginSync;
//extern CS												gcsNXPCB_LoginSync;

extern std::map<string, CUser*>		g_mapNXAcc; // <�ؽ�����, ������ü>
extern CS							g_csNXAcc;
#endif

extern bool		g_bCashItemtest;


#ifdef _PD_COMPANY_JAPAN_
#include "../Japan RMS/RocAuthSession.h"
#endif

#ifdef _VINA_SHUTDOWN_20061023_
extern int		g_iShutdownHour;
#endif

bool SendLS(char * sendBuf, int bufSize, SOCKADDR_IN * lsAddr) ;


void EnterGsFunc(CMSG_ENTERSERVER_CHECK_REQ_GS_DS * pMsgSvEnterGame
				 , _h_DS_Com	* pDSComm
				 , CMSG_USER_CHARAC_LIST_DS_GS *msgUserCharacList
				 , COLE_DB_Executer * g_DB_Command
				 , C_USER_DB_Command * g_USER_DB_Command)
{
	// REQ : CMSG_ENTERSERVER_CHECK_REQ_GS_DS (GS -> DS)
	// RES : IF SUCCESS THEN SEND 'CMSG_USER_CHARAC_LIST_DS_GS' (DS -> GS)
	//		 IF FAILED THEN SEND 'CMSG_ENTERSERVER_CHARAC_RESULT_DS_GS (GS -> DS)


	CMSG_ENTERSERVER_CHECK_RESULT_DS_GS  msgSvEnterGameFail = {0} ;	
	msgSvEnterGameFail.usLength =  sizeof(CMSG_ENTERSERVER_CHECK_RESULT_DS_GS);	
	msgSvEnterGameFail.cMessage = CMSG_NO_ENTERSERVER_CHECK_RESULT_DS_GS;


	memset( msgUserCharacList->List, 0, sizeof(_CharacList) * 3 ) ;

	if( pMsgSvEnterGame->usUid == 0 ) 
	{
		PrintConsole( "[FAIL] A<%s> ENTERSERVER_CHECK_REQ (Packet uid is zero)\n", pMsgSvEnterGame->cAccount );
		return ;
	}

	pMsgSvEnterGame->cAccount[en_max_lil] = 0 ;

	bool bRet = true ;

	int ug_state_send = 0 ;	// 0 - not send, 1 - in_old, 2 - in_new

	PCHARAC				pCharac = NULL;				// Character info struct
	PUSER				pUser = NULL;				// User info struct	

	_sServer_InvenItem_v1 * pInven = NULL;
	
	int					sendError = 0;				//SendOK() ����
	int					iGlobalTime = CUser::get_time_by_sec() ;
	
	//PrintConsole( "[INFO] Recv EnterServer packet (ACC:%s : %d, SVR:%d)\n", 
	//	pMsgSvEnterGame->cAccount, pMsgSvEnterGame->usUid, pMsgSvEnterGame->cServerNum );

	pUser = g_pAccMapManager->Find_User(pMsgSvEnterGame->cAccount);    
	if(pUser == NULL){
		//������ �ʿ� ����.. ����ó���ض�..
		PrintConsole( "[ERROR] ENTERSERVER_CHECK_REQ (Not found data from map) A<%s> \n", pMsgSvEnterGame->cAccount );		
		bRet = false ;		

		goto label_error ;	
	}
	// ozzywow edit (������ �˻� �ӽ� ����)
	else if( ( pUser->usTempUniqueID != pMsgSvEnterGame->usUid ) 
		|| (pUser->cState == US_NONSTATE) 
		|| strncmp( pUser->cNDAccount, pMsgSvEnterGame->cAccount, en_max_lil+1) != 0 )
	{			
		PrintConsole( "[FAIL] ENTERSERVER_CHECK_REQ (LOGIN Check Time) A<%s>\n", pMsgSvEnterGame->cAccount );
				
		if( pUser->usTempUniqueID != pMsgSvEnterGame->usUid )
			PrintConsole( "  - Different 'tempUniqueID' (s:%u, t:%u)\n", pUser->usTempUniqueID, pMsgSvEnterGame->usUid );
		
		if( pUser->cState == US_NONSTATE )
			PrintConsole( "  - Is logouted\n" );

		bRet = false ;
		goto label_error ;
	}
	// ������ �ߺ� �˻� �̴�..
	else
	{
		switch( g_uiCompanyCode )
		{
		case en_lmbc_nexon:
			if( g_USER_DB_Command->DSNexonEnterCheck( pMsgSvEnterGame->cAccount, g_ForestNum, US_FIELD, pMsgSvEnterGame->usUid ) == false )
			{
				PrintConsole( "[DB:FAIL] DSNexonEnterCheck(??) == false, A<%s>, TempUID(%u) \n", pMsgSvEnterGame->cAccount, pMsgSvEnterGame->usUid );

				bRet = false ;
				goto label_error ;
			}
			break;
		default:
			if( g_USER_DB_Command->DSEnterCheck( pMsgSvEnterGame->cAccount, g_ForestNum, US_FIELD, pMsgSvEnterGame->usUid ) == false )
			{
				PrintConsole( "[DB:FAIL] DSEnterCheck() false, A<%s>, TempUID(%u), For(%d)\n", pMsgSvEnterGame->cAccount, pMsgSvEnterGame->usUid, g_ForestNum );

				bRet = false ;
				goto label_error ;
			}
		}
	}
	
	// uid�� ���ڴٰ�...
	if( g_pUniqueIDQueue->UseUID( pUser->usTempUniqueID ) == false )
	{
		PrintConsole( "[ERROR] Use UID fail, A<%s>, UID<%u>\n", pMsgSvEnterGame->cAccount, pUser->usTempUniqueID );
		bRet = false ;
		goto label_error ;
	}


label_error:
	// ����������� ó���κ�..
	if( bRet == false )
	{
		if( pUser )
		{
			pUser->usTempUniqueID = 0 ;
			pUser->cState = US_NONSTATE;
			pUser->iTime = 0 ;		
			memset(pUser->cSelChrName, 0, en_charac_name_length+1);	
		}

		memcpy(msgSvEnterGameFail.cAccount, pMsgSvEnterGame->cAccount, en_max_lil+1);
		msgSvEnterGameFail.usUid = pMsgSvEnterGame->usUid;		
		sendError = pDSComm->Send(msgSvEnterGameFail.usLength, (char*)&msgSvEnterGameFail);

		CMSG_USER_LOGOUT_DS_GS msgUserLogoutReq;
		msgUserLogoutReq.usLength = sizeof(CMSG_USER_LOGOUT_DS_GS) ;
		msgUserLogoutReq.cMessage = CMSG_NO_USER_LOGOUT_DS_GS;		

		memcpy(msgUserLogoutReq.cNDAccount, pMsgSvEnterGame->cAccount, en_max_lil+1);
		msgUserLogoutReq.cReason = 0;

		pDSComm->Send( msgUserLogoutReq.usLength, (char*)&msgUserLogoutReq ) ;

		PrintConsole( "[FAIL] Enter GS Failed! (A<%s> Server<%d>)\n", pMsgSvEnterGame->cAccount, pMsgSvEnterGame->cServerNum );

		return ;
	}

	// ������� ������ ���� �˻� �Ϸ�..
	// ���� ó���ϰ�.. ���� ������ �ٲ��ְ� ĳ���� ����Ʈ�� �������...
	//if(pUser->iServer == pMsgSvEnterGame->cServerNum){	// ���� ��ȣ�� �¾ƾ� �α��� ��Ŵ		
	pUser->iServer = pMsgSvEnterGame->cServerNum;	
	msgUserCharacList->usUid = pUser->usTempUniqueID;
	memcpy(msgUserCharacList->cSelChrName, pUser->cSelChrName, en_charac_name_length+1);

	g_DB_Command->SetLastServer(pUser->iUid, pUser->iServer) ;
	

	// ���Ӽ����� ���� ó�� �����Ҷ� ĳ���� ����Ʈ ��ȯ
	if(pUser->cState == US_MOVE){	// ���� ���°� ���� �̵����̸� ����ĳ���͸� �����ش�..

		//PrintConsole( _TRACE_CLASS_LOGIN, "[INFO] pUser->cState == US_MOVE ( Into form the filed )\n");

		// Send character info
		//pCharac = g_pAccMapManager->Find_Charac(pUser->cSelChrName);
		pCharac = pUser->characFind( pUser->cSelChrName )  ;

		if(pCharac == NULL ) 
		{
			CMSG_USER_LOGOUT_DS_GS msgUserLogoutReq;
			msgUserLogoutReq.usLength = sizeof(CMSG_USER_LOGOUT_DS_GS) ;
			msgUserLogoutReq.cMessage = CMSG_NO_USER_LOGOUT_DS_GS;		
			memcpy(msgUserLogoutReq.cNDAccount, pMsgSvEnterGame->cAccount, en_max_lil+1);
			msgUserLogoutReq.cReason = 0;

			pDSComm->Send( msgUserLogoutReq.usLength, (char*)&msgUserLogoutReq ) ;
		}
	
		pCharac->CHARAC_ETC.cState = CS_FIELD;
		pCharac->CHARAC_CUR_BASIC.sZone = pMsgSvEnterGame->cServerNum;	
		
		pUser->cState = US_FIELD;
	}

	// �κ�� ���� ó�� �����ϴ� ĳ����
	else{
		//PrintConsole( _TRACE_CLASS_LOGIN, "[INFO] pUser->cState != US_MOVE (Info from robby )\n");

		for(int i=0; i<3; i++){
			if( pUser->stCharac[i].CHARAC_BASIC.iUniqueID != 0 )
			{
				pUser->stCharac[i].CHARAC_ETC.cState = CS_STANDBY;				
				pUser->stCharac[i].CHARAC_STATE.sPartyIndex = 0 ;
			}		
		}	
		
		// ĳ���� ���� ������ ����.
		memset(msgUserCharacList->cSelChrName, 0, en_charac_name_length+1);

#ifdef _VINA_SHUTDOWN_20061023_
#ifdef _PD_SDS_GOSTOP_
		pUser->FatigueElement.ucFatigueMode = 0;		// ó�� ������ ��, Go ����..
#endif
#endif
	}

	for(int i = 0; i < 3 ; i++){

		pCharac = &pUser->stCharac[i];

		if( pCharac->CHARAC_BASIC.iUniqueID != 0 )
		{
			// Inventory convert
			pInven = reinterpret_cast<_sServer_InvenItem_v1*>(pCharac->CHARAC_INVENTORY.cInventory) ; 

			g_pTrace->OutputString( _TRACE_CLASS_LOGIN, "  - [%d] %s \n", i, pCharac->CHARAC_BASIC.cChrName);

			msgUserCharacList->List[i].iUniqueID = pCharac->CHARAC_BASIC.iUniqueID;
			memcpy(msgUserCharacList->List[i].cName, pCharac->CHARAC_BASIC.cChrName, 13);
			memcpy(msgUserCharacList->List[i].cNick, pCharac->CHARAC_BASIC.cChrNic, 13);

			msgUserCharacList->List[i].cZone = (char)pCharac->CHARAC_CUR_BASIC.sZone;
			msgUserCharacList->List[i].cSex = (unsigned char)pCharac->CHARAC_BASIC.cSex;
			msgUserCharacList->List[i].cFace = pCharac->CHARAC_BASIC.cFace;
			msgUserCharacList->List[i].cHair = pCharac->CHARAC_BASIC.cHair;
			msgUserCharacList->List[i].cGroup = pCharac->CHARAC_BASIC.cParty;


			msgUserCharacList->List[i].cAmorType = pInven[0].cType;
			msgUserCharacList->List[i].cPantsType = pInven[1].cType;
			msgUserCharacList->List[i].cAdaeType  = pInven[2].cType;
			msgUserCharacList->List[i].cShoesType = pInven[3].cType;
			msgUserCharacList->List[i].cCapType   = pInven[4].cType;			


			msgUserCharacList->List[i].cMaskType = pInven[7].cType ;		

			msgUserCharacList->List[i].cBackPackType = pInven[23].cType ;

			msgUserCharacList->List[i].cWeaponType = pInven[10].cType ;	
			msgUserCharacList->List[i].sSelectWeaponID = pInven[10].sID ;

			msgUserCharacList->List[i].sAmor = pInven[0].sID  ;
			msgUserCharacList->List[i].sPants = pInven[1].sID ;
			msgUserCharacList->List[i].sCap = pInven[4].sID ;
			msgUserCharacList->List[i].sAdae = pInven[2].sID ;
			msgUserCharacList->List[i].sShoes = pInven[3].sID ;


			msgUserCharacList->List[i].sMask = pInven[7].sID ;
			msgUserCharacList->List[i].ucWeaponStrength = pInven[10].ucStrength ;
			msgUserCharacList->List[i].sBackPack = pInven[23].sID ;			// �賶 


			//msgUserCharacList->List[i].sSelectWeaponID = pCharac->CHARAC_INVENTORY.sSelectWeaponID;


			//5������ġ
			msgUserCharacList->List[i].sConstitution = pCharac->CHARAC_LEVEL.sConstitution;
			msgUserCharacList->List[i].sZen = pCharac->CHARAC_LEVEL.sZen;
			msgUserCharacList->List[i].sIntelligence = pCharac->CHARAC_LEVEL.sIntelligence;
			msgUserCharacList->List[i].sDexterity = pCharac->CHARAC_LEVEL.sDexterity;
			msgUserCharacList->List[i].sStr = pCharac->CHARAC_LEVEL.sStr;

			msgUserCharacList->List[i].sInnerLevel = pCharac->CHARAC_STATE.sInnerLevel;	

			msgUserCharacList->List[i].cClass = pCharac->CHARAC_BASIC.cClass ;
			msgUserCharacList->List[i].cClassGrade = pCharac->CHARAC_BASIC.cClassGrade ;

			msgUserCharacList->List[i].fPosX = pCharac->CHARAC_CUR_BASIC.fX ;
			msgUserCharacList->List[i].fPosZ = pCharac->CHARAC_CUR_BASIC.fZ ;
			
			if( pCharac->CHARAC_STATE.iBlockingEndDate < CUser::get_time_by_sec() )
			{
				pCharac->CHARAC_BASIC.cGM_Check = 0 ;
				pCharac->CHARAC_STATE.iBlockingEndDate = 0 ;				
			}
			msgUserCharacList->List[i].iBlockingEndDate = pCharac->CHARAC_STATE.iBlockingEndDate ;

			msgUserCharacList->List[i].cGM = pCharac->CHARAC_BASIC.cGM_Check ;

			msgUserCharacList->List[i].ucChangeNameFlag = pCharac->CHARAC_BASIC.ucChangeName ;
		}					
	} // for		6												

	/*
	msgUserCharacList->ucMoveState = static_cast<u_char>(ug_state_send) ;
	*/
	CopyMemory( &msgUserCharacList->aius, &pUser->AIUS, sizeof(_ACCOUNT_ITEM_USE_STATUS) ) ;

#ifdef _PD_PCROOM_INFO_
#ifdef _PD_VINA_PCROOM_INFO_
	msgUserCharacList->ucWhereConnect = pUser->ucPCRoomInfo;
	msgUserCharacList->ucUserGrade = 0;
#else
	msgUserCharacList->ucWhereConnect = ( pUser->bPCBang ) ? 1 : 0;
	msgUserCharacList->ucUserGrade = 0;
#endif
#endif

#ifdef _VINA_SHUTDOWN_20061023_
	//DB���� FatigueWeight(�Ƿε�), �α׾ƿ� �ð� ������
	g_USER_DB_Command->GetFatigueInfo(pUser->iUid, pUser) ;

	//����ð��� �������α׾ƿ��ð� �� 1) �� != �� result : 0  2) �� == �� result : fatigue update
	SYSTEMTIME	curTime ;
	GetLocalTime(&curTime) ;
	int iResetTime = 0;
	int iCurTime = 0 ;
	int iLogoutTime = 0 ;

	if(curTime.wHour < g_iShutdownHour) {
		iResetTime = (curTime.wYear-1)*365*24 + (curTime.wMonth-1)*31*24 + (curTime.wDay-1)*24 + g_iShutdownHour;
	}
	else {
		iResetTime = (curTime.wYear-1)*365*24 + (curTime.wMonth-1)*31*24 + (curTime.wDay)*24 + g_iShutdownHour;
	}
	
	iCurTime = (curTime.wYear-1)*365*24 + (curTime.wMonth-1)*31*24 + (curTime.wDay-1)*24 + curTime.wHour;

	//iLogoutTime = g_DB_Command->Get_LogoutTime() ;

	if(pUser->FatigueElement.iLogoutTime >= iResetTime - 24 /*&& iCurTime < iResetTime*/) {
		msgUserCharacList->sFatigueWeight = pUser->FatigueElement.sFatigueWeight ;/*DB���� ���� iFatigueWeight   pUser->iFatigueWeight*/
	}
	else {
		msgUserCharacList->sFatigueWeight = pUser->FatigueElement.sFatigueWeight = 0 ;
	}
#ifdef _PD_SDS_GOSTOP_
	msgUserCharacList->ucFatigueMode = ( 0 == pUser->FatigueElement.ucFatigueMode ) ? 0 : 1 ;
#endif

	Logout("[VINA] A<%s> PC(%s) cur=%d/%d %dh iResetTime=%d iCurTime=%d iLogoutTime=%d sFatigueWeight=%d",
		pUser->cNDAccount, 
#ifdef _PD_VINA_PCROOM_INFO_
		( pUser->ucPCRoomInfo ? "PCRoom" : "Home" ),
#else
		"Home",
#endif
		curTime.wYear,
		curTime.wMonth,
		curTime.wDay,
		curTime.wHour,
		iResetTime,
		iCurTime,
		pUser->FatigueElement.iLogoutTime,
		pUser->FatigueElement.sFatigueWeight);

#endif	// _VINA_SHUTDOWN_20061023_

#ifdef	_NXPCB_AWARD_200703_

	std::string		strAcc(pUser->cNXAcc);
	g_csNXAcc.Lock();
	std::map<string, CUser*>::iterator it;
	while(TRUE) {
		it = g_mapNXAcc.find(strAcc);
		if(it == g_mapNXAcc.end()) break;
		else {
			g_mapNXAcc.erase(it);
		}
	}
	g_mapNXAcc[strAcc] = pUser;
	g_csNXAcc.Unlock();

	NEXONPCB_LoginReq(pUser->cNXAcc, "", (DWORD)pUser->iIp);

#endif	// _NXPCB_AWARD_200703_

	sendError = pDSComm->Send(msgUserCharacList->usLength, (char*)msgUserCharacList);
}

void GSConnect(CMSG_GSCONNECT_GS_DS * pMsgGsconnect, _h_DS_Com	* pDSComm)
{
	//UMSG_GS_STATE							umsgGsState = {0};
	// GS ���� �޽��� �ʱ�ȭ	
	g_ServerInfo.AddServer(pMsgGsconnect->cServerNum);				
	g_ServerInfo.m_ServerNetInfo[pMsgGsconnect->cServerNum].ds_com = pDSComm;

	::PrintConsole("[INFO] Send GS Server info(No:%d) \n",pMsgGsconnect->cServerNum);	

	CMSG_GSCONNECT_DS_GS msgConnectRes = {0} ;
	msgConnectRes.cMessage = CMSG_NO_GSCONNECT_DS_GS ;
	msgConnectRes.usLength = sizeof( CMSG_GSCONNECT_DS_GS ) ;

	for( int i = 0 ; i < GS_MAX ; i++ )
	{
		msgConnectRes.cServerList[i] = g_ServerInfo.m_ServerStateInfo[i].cState ;
	}

	msgConnectRes.uiServerStat = g_serverStateFlag ;


	// GS �� ����	
	g_pDSComQueue->GlobalSend( msgConnectRes.usLength, (char*)&msgConnectRes ) ;

	CMSG_ENV_SETTING pkEnv( en_env_setting_cashitem_period_test );
	if( g_bCashItemtest )
		pkEnv.iValue = 1;
	else
		pkEnv.iValue = 0;

	pDSComm->Send( pkEnv.usLength, (char*)&pkEnv );
	
}

void GSDisConnectInfoSend( char cServerNum )
{
	if ( GS_MAX < cServerNum ) {	
		return ;
	}

	CMSG_GSCONNECT_DS_GS msgConnectRes = {0} ;
	msgConnectRes.cMessage = CMSG_NO_GSCONNECT_DS_GS ;
	msgConnectRes.usLength = sizeof( CMSG_GSCONNECT_DS_GS ) ;

	for( int i = 0 ; i < GS_MAX ; i++ )
	{
		msgConnectRes.cServerList[i] = g_ServerInfo.m_ServerStateInfo[i].cState ;
	}


	msgConnectRes.uiServerStat = g_serverStateFlag ;


	// GS �� ����	
	g_pDSComQueue->GlobalSend( msgConnectRes.usLength, (char*)&msgConnectRes ) ;

}

void CharacSelect(CMSG_CHARAC_SELECT_GS_DS * pMsgCharacSelect
				  ,  _h_DS_Com * pDSComm
				  ,  COLE_DB_Executer *g_DB_Command
				  ,  CMSG_CHARAC_ALL_INFO_DS_GS * pMsgCharacAllInfo)
{
	// CMSG_CHARAC_SELECT_GS_DS (req): GS -> DS
	// CMSG_CHARAC_RESULT_DS_GS (res): DS -> GS (FAILED)
	// CMSG_CHARAC_ALL_INFO_DS_GS (res): DS -> GS (SUCCESSED)

	pMsgCharacSelect->cAccount[en_max_lil] = 0 ;
	pMsgCharacSelect->cChrName[en_charac_name_length] = 0 ;

	
	PCHARAC			pCharac = NULL;				// Character info struct
	PUSER			pUser = NULL;				// User info struct	
	int			sendError = 0;				//SendOK() ����
	
	g_pTrace->OutputString( _TRACE_CLASS_CHARAC
		, "[INFO] Recv CMSG_NO_CHARAC_SELECT_GS_DS (ACC:%s, CHR:%s)\n"
		,pMsgCharacSelect->cAccount,pMsgCharacSelect->cChrName);

		
	pUser = g_pAccMapManager->Find_User(pMsgCharacSelect->cAccount);
	if(pUser == NULL){
		::PrintConsole("[ME_ERR] Not found user form map (ACC:%s)\n", pMsgCharacSelect->cAccount);

		CMSG_CHARAC_RESULT_DS_GS msgCharacSelectResult = {0} ;
		msgCharacSelectResult.usLength = sizeof(CMSG_CHARAC_RESULT_DS_GS) ;
		msgCharacSelectResult.cMessage = CMSG_NO_CHARAC_RESULT_DS_GS ;
		msgCharacSelectResult.cPrevCommand = CMSG_NO_CHARAC_SELECT_GS_DS ;

		memcpy(msgCharacSelectResult.cChrName, pMsgCharacSelect->cChrName, 13);
		msgCharacSelectResult.cResult = ErrorMessageNum_CanNotSelect ;	//ĳ���͸� ������ �� ����(DS_Packets.h)
		msgCharacSelectResult.usUid = pMsgCharacSelect->usUid ;

		//// ��� ���� ����					
		pDSComm->Send(msgCharacSelectResult.usLength, (char*)&msgCharacSelectResult);

		CMSG_USER_LOGOUT_DS_GS msgUserLogout = {0} ;
		msgUserLogout.usLength = sizeof(CMSG_USER_LOGOUT_DS_GS) ;
		msgUserLogout.cMessage = CMSG_NO_USER_LOGOUT_DS_GS ;
		memcpy( msgUserLogout.cNDAccount, pMsgCharacSelect->cAccount, en_max_lil+1 ) ;
		msgUserLogout.cReason = 0 ;

		pDSComm->Send( msgUserLogout.usLength, (char*)&msgUserLogout ) ;

		return;
	}
	else if(pUser->cState == US_NONSTATE  ){
		PrintConsole("[ME_ERR] (ACC:%s)Is Logouted \n", pUser->cNDAccount);
		
		CMSG_CHARAC_RESULT_DS_GS msgCharacSelectResult = {0} ;
		msgCharacSelectResult.usLength = sizeof(CMSG_CHARAC_RESULT_DS_GS) ;
		msgCharacSelectResult.cMessage = CMSG_NO_CHARAC_RESULT_DS_GS ;
		msgCharacSelectResult.cPrevCommand = CMSG_NO_CHARAC_SELECT_GS_DS ;

		memcpy(msgCharacSelectResult.cChrName, pMsgCharacSelect->cChrName, 13);
		msgCharacSelectResult.cResult = ErrorMessageNum_CanNotSelect;	//ĳ���͸� ������ �� ����(DS_Packets.h)
		msgCharacSelectResult.usUid = pUser->usTempUniqueID;
		

		//// ��� ���� ����					
		pDSComm->Send(msgCharacSelectResult.usLength, (char*)&msgCharacSelectResult);
		
		CMSG_USER_LOGOUT_DS_GS msgUserLogout = {0} ;
		msgUserLogout.usLength = sizeof(CMSG_USER_LOGOUT_DS_GS) ;
		msgUserLogout.cMessage = CMSG_NO_USER_LOGOUT_DS_GS ;
		memcpy( msgUserLogout.cNDAccount, pUser->cNDAccount, en_max_lil+1 ) ;
		msgUserLogout.cReason = 0 ;

		pDSComm->Send( msgUserLogout.usLength, (char*)&msgUserLogout ) ;

		return;
	}
	
	// �ߺ��α����� ���ɼ��� �ִ�.. ���������..
	if( pUser->usTempUniqueID != pMsgCharacSelect->usUid 
		|| strncmp( pUser->cNDAccount, pMsgCharacSelect->cAccount, en_max_lil+1 ) != 0 )
	{
		PrintConsole( "[ERROR]  pUser->usTempUniqueID(%u) != pMsgCharacSelect->usUid(%u)  ( acc: %s, chr:%s)\n",
			pUser->usTempUniqueID, pMsgCharacSelect->usUid, pMsgCharacSelect->cAccount, pMsgCharacSelect->cChrName ) ;

		pUser->cState = US_NONSTATE	;

		for(int i=0; i<3; i++){
			if( pUser->stCharac[i].CHARAC_BASIC.iUniqueID != 0 )
			{
				pUser->stCharac[i].CHARAC_ETC.cState = CS_NONSTATE ;									
			}								
		}
		pUser->usTempUniqueID = 0 ;
		pUser->iTime = 0 ;

		CMSG_USER_LOGOUT_DS_GS msgUserLogout = {0} ;
		msgUserLogout.usLength = sizeof(CMSG_USER_LOGOUT_DS_GS) ;
		msgUserLogout.cMessage = CMSG_NO_USER_LOGOUT_DS_GS ;
		memcpy( msgUserLogout.cNDAccount, pUser->cNDAccount, en_max_lil+1 ) ;
		msgUserLogout.cReason = 0 ;

		pDSComm->Send( msgUserLogout.usLength, (char*)&msgUserLogout ) ;

		return ;
	}

	pCharac = pUser->characFind( pMsgCharacSelect->cChrName )  ;
	

	if(pCharac == NULL){	//�ʿ� ������..

		CMSG_CHARAC_RESULT_DS_GS msgCharacSelectResult = {0} ;
		msgCharacSelectResult.usLength = sizeof(CMSG_CHARAC_RESULT_DS_GS) ;
		msgCharacSelectResult.cMessage = CMSG_NO_CHARAC_RESULT_DS_GS ;
		msgCharacSelectResult.cPrevCommand = CMSG_NO_CHARAC_SELECT_GS_DS ;

		memcpy(msgCharacSelectResult.cChrName, pMsgCharacSelect->cChrName, en_charac_name_length+1);
		msgCharacSelectResult.cResult = ErrorMessageNum_CanNotSelect;	//ĳ���͸� ������ �� ����(DS_Packets.h)
		msgCharacSelectResult.usUid = pUser->usTempUniqueID;
		

		PrintConsole("[ME_ERR] Not found charac from map (CHR:%s) \n", pMsgCharacSelect->cChrName);

		pUser->user_lock() ;
		try{
			if( g_pAccMapManager->Erase( pUser) == false )
			{
				PrintConsole("[ERROR] if( g_pAccMapManager->Erase( pUser) == false ) \n %s, %d \n", __FILE__,__LINE__) ;
			}
		}
		catch(...){
			PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}		
		pUser->user_unlock() ;
		pUser = NULL ;

		//// ��� ���� ����					
		pDSComm->Send(msgCharacSelectResult.usLength, (char*)&msgCharacSelectResult);
		return ;
	}

	else {

		/*================================================================================
		// ��ŷ�� ĳ���� �ƴ��� Ȯ���ϰ� ��ŷ�� ������� ������Ŷ�� ������.
		================================================================================*/		
		if( pCharac->CHARAC_BASIC.cGM_Check < 0 )
		{
			// ���� ��ŷ ���ᰡ ������ �ʾҴ�.
			if( pCharac->CHARAC_STATE.iBlockingEndDate > CUser::get_time_by_sec() )
			{

				::PrintConsole( "[ERROR] Blocking character try to enter field (chr : %d)\n", pCharac->CHARAC_BASIC.cChrName  ) ;

				CMSG_CHARAC_RESULT_DS_GS msgCharacSelectResult = {0} ;
				msgCharacSelectResult.usLength = sizeof(CMSG_CHARAC_RESULT_DS_GS) ;
				msgCharacSelectResult.cMessage = CMSG_NO_CHARAC_RESULT_DS_GS ;
				msgCharacSelectResult.cPrevCommand = CMSG_NO_CHARAC_SELECT_GS_DS ;

				memcpy(msgCharacSelectResult.cChrName, pMsgCharacSelect->cChrName, 13);
				msgCharacSelectResult.cResult = ErrorMessageNum_CanNotSelect;	//ĳ���͸� ������ �� ����.
				msgCharacSelectResult.usUid = pUser->usTempUniqueID;

				sendError = pDSComm->Send( msgCharacSelectResult.usLength, (char*)&msgCharacSelectResult);
				if(sendError == 0){
					::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", sendError ) ;
				}	
				return;
			}
			// ��ŷ ����ð��� ������. ��ŷ ������ �ʱ�ȭ�϶�
			else
			{
				pCharac->CHARAC_BASIC.cGM_Check = 0 ;
				pCharac->CHARAC_STATE.iBlockingEndDate = 0 ;
			}
		}
		//================================================================================
		

		/*================================================================================
		 // �̵��Ϸ��� ���� ���°� ���������� Ȯ���ϰ� ������ �̶�� ������Ŷ�� ������	
		================================================================================*/		
		if( g_pServerInfo->m_ServerStateInfo[pCharac->CHARAC_CUR_BASIC.sZone].cState != GS_OK )
		{
			g_pTrace->OutputString( _TRACE_CLASS_CHARAC
				, "[INFO] if( g_pServerInfo->m_ServerStateInfo[pCharac->CHARAC_CUR_BASIC.sZone].cState != GS_OK )(%d)\n"
				, pCharac->CHARAC_CUR_BASIC.sZone ) ;				

			CMSG_CHARAC_RESULT_DS_GS msgCharacSelectResult = {0} ;
			msgCharacSelectResult.usLength = sizeof(CMSG_CHARAC_RESULT_DS_GS) ;
			msgCharacSelectResult.cMessage = CMSG_NO_CHARAC_RESULT_DS_GS ;
			msgCharacSelectResult.cPrevCommand = CMSG_NO_CHARAC_SELECT_GS_DS ;

			memcpy(msgCharacSelectResult.cChrName, pMsgCharacSelect->cChrName, 13);
			msgCharacSelectResult.cResult = ErrorMessageNum_ServerStateErr;	//�ش� ������ ���°� ���������Դϴ�.
			msgCharacSelectResult.usUid = pUser->usTempUniqueID;
			
			sendError = pDSComm->Send( msgCharacSelectResult.usLength, (char*)&msgCharacSelectResult);
			if(sendError == 0){
				::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", sendError ) ;
			}	
			return;
		}
		//================================================================================

		
		g_DB_Command->SelectCharac( pMsgCharacSelect->cChrName ) ;

		memcpy(pUser->cSelChrName, pMsgCharacSelect->cChrName, 13);	// ĳ���� ���� ���� �����ϰ�..
		pCharac->CHARAC_ETC.cState = CS_FIELD;		//ĳ���� ���� ����
		pUser->cState = US_FIELD;
		pUser->iServer = pCharac->CHARAC_CUR_BASIC.sZone;
		//pUser->cBackupState = BS_READY;		
		//g_pBackupReady->InsertAcc( pUser ) ;


		pMsgCharacAllInfo->usUid = pUser->usTempUniqueID;

		memcpy(&pMsgCharacAllInfo->charac, dynamic_cast<_CHARAC*>(pCharac), sizeof(_CHARAC) );
		memcpy(&pMsgCharacAllInfo->quickslot, &pCharac->CHARAC_ETC_QUICKSLOT, sizeof(_CHARAC_ETC_QUICKSLOT) );

#ifdef _PD_CHARAC_LTS_
		memcpy( &pMsgCharacAllInfo->characLts, &pCharac->CHARAC_LTS, sizeof(_CHARAC_LTS) );
#endif

		memcpy(&pMsgCharacAllInfo->storage, &pUser->stUserDepot, sizeof(_USER_DEPOT) );

		pMsgCharacAllInfo->uiCheckFlag_Login = pUser->bCheckCurEvent;

		pUser->bCheckCurEvent = 1;

		
#ifdef _PD_MASTER_PUPIL_
		// �Ʒ� �κ���, GetCharacAllInfo ���� ���� ä�����Ƿ�, �ּ� ó���Ѵ�. 20080916 - ����
		/*
        CDSMaster * pMaster = g_pMPSet->FindAndMakeMasterIndex( pCharac->CHARAC_BASIC.iUniqueID );
		if( pMaster )
		{
			// ������ �ִٸ�,.. 
			pCharac->CHARAC_MP.ucMPPosition = pMaster->GetRelationn(pCharac->CHARAC_BASIC.iUniqueID);
			pCharac->CHARAC_MP.ucRes = 0;
			pCharac->CHARAC_MP.usRes = 0;
			pCharac->CHARAC_MP.iMasterIndex = pMaster->GetIndex();

		}
		else
		{
			// ������ ���ٸ�, �ʱ�ȭ.
			pCharac->CHARAC_MP.ucMPPosition = en_mp_relation_none;
			pCharac->CHARAC_MP.ucRes = 0;
			pCharac->CHARAC_MP.usRes = 0;
			pCharac->CHARAC_MP.iMasterIndex = 0;
		}
		*/
		// 20080916 - ����

		// ���� ����.
		CopyMemory( &pMsgCharacAllInfo->charac.CHARAC_MP, &pCharac->CHARAC_MP, sizeof(_CHARAC_MP) );

#ifdef _PD_MP_DEBUG_
		PrintConsole( "[MP] C<%s> : Master Index = %d : Posistion = %u\n", 
			pMsgCharacAllInfo->charac.CHARAC_BASIC.cChrName, 
			pMsgCharacAllInfo->charac.CHARAC_MP.iMasterIndex,
			pMsgCharacAllInfo->charac.CHARAC_MP.ucMPPosition );
#endif

#endif	// _PD_MASTER_PUPIL_

		// ĳ�� ������ �����ϱ� ���� ����(����)�� ���� ���Ἲ�� �˻��϶�..
		_Organization * pOR = g_pOrganizationManager->FindOrganization( pCharac->CHARAC_STATE.iORIndex ) ;
		if( pOR ) 
		{
			_Unit * pUnit = pOR->FindUnit( pCharac->CHARAC_BASIC.cChrName ) ;
			// ����(����)�� ���� �α��� ������ �����Ѵ�.
			if( pUnit ) 
			{
				pUnit->m_bOnline = true ;
				pUnit->m_cZone = static_cast<char>(pCharac->CHARAC_CUR_BASIC.sZone) ;
				pUnit->m_sInnerLevel = pCharac->CHARAC_STATE.sInnerLevel ;

				SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS msgUnitControl = {0} ;
				msgUnitControl.usLength = sizeof(SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS ) ;
				msgUnitControl.cMessage = SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS ;
				msgUnitControl.iIndex = pCharac->CHARAC_STATE.iORIndex ;
				msgUnitControl._unit = *pUnit ;
				msgUnitControl.cCode = SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_UPDATE_ ;

				g_pDSComQueue->GlobalSend( msgUnitControl.usLength, (char*)&msgUnitControl ) ;

				
			}
			// ������ ���� ������ ���ٸ� �����߿� ���� �����ȰŴ�.. ���� ���� �ε����� 0���� �ֱ�ȭ�϶�..
			else
			{
				pCharac->CHARAC_STATE.iORIndex = 0 ;
				pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY ;
				g_pBackupReady->InsertCharac( pCharac ) ;
			}
		}
		// �ش� ������ ã�� ���ߴٸ� ������ �̹� �ػ�ȰŴ�. ���� ���� �ε����� 0���� �ʱ�ȭ �϶�..
		else
		{
			pMsgCharacAllInfo->charac.CHARAC_STATE.iORIndex = 0;

			pCharac->CHARAC_STATE.iORIndex = 0 ;

			pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY ;
			g_pBackupReady->InsertCharac( pCharac ) ;
		}


				
		
		sendError = pDSComm->Send(pMsgCharacAllInfo->usLength, (char*)pMsgCharacAllInfo);

		g_pTrace->OutputString( _TRACE_CLASS_CHARAC,"[INFO]Selected charac(%s) \n", pUser->cSelChrName);
	}				
}

void CharacCreate(CMSG_CHARAC_CREATE_GS_DS * pMsgCharacCreate
				  , _h_DS_Com * pDSComm
				  ,  COLE_DB_Executer * g_DB_Command
				  , C_USER_DB_Command * g_UserDB_Command  )
{


	if( pMsgCharacCreate->cAccount[0] == 0 || pMsgCharacCreate->cName[0] == 0 ) { return ; }
	pMsgCharacCreate->cAccount[en_max_lil] = 0 ;
	pMsgCharacCreate->cName[en_charac_name_length] = 0 ;
	
	
	// CMSG_CHARAC_CREATE_GS_DS (req): GS -> DS 
	// CMSG_CHARAC_RESULT_DS_GS (res): DS -> GS (if success then return zero)
	PCHARAC				pCharac = NULL;				// Character info struct
	PUSER				pUser = NULL;				// User info struct

	_sServer_InvenItem_v1 * pInven = NULL;
	

	int			sendError = 0;				//SendOK() ����
	int			nReturn = 0;	
	int			uid = 0;

#ifdef _TRACE_
	g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[INFO] Recv CMSG_NO_CHARAC_CREATE_GS_DS packet (ACC:%s) \n", pMsgCharacCreate->cAccount);
#endif

	CMSG_CHARAC_RESULT_DS_GS msgCharacCreateResult = {0} ;
	msgCharacCreateResult.usLength = sizeof(CMSG_CHARAC_RESULT_DS_GS) ;
	msgCharacCreateResult.cMessage = CMSG_NO_CHARAC_RESULT_DS_GS ;
	msgCharacCreateResult.cPrevCommand = CMSG_NO_CHARAC_CREATE_GS_DS ;	
	


	memcpy(msgCharacCreateResult.cChrName, pMsgCharacCreate->cAccount, 13);	//������ Account
	
	pUser = g_pAccMapManager->Find_User(pMsgCharacCreate->cAccount);


	if(pUser == NULL || pUser->cState == US_NONSTATE ) {
		msgCharacCreateResult.cResult = 1;	// ���̻� ���� �� ����.
	}
	else if(pUser->cCharacCount >= 3){
		msgCharacCreateResult.cResult = 5;	// ���̻� ���� �� ����.
	}
	else {					

		msgCharacCreateResult.usUid = pUser->usTempUniqueID;

		try{
			nReturn = g_DB_Command->pr_Charac_Create(pUser->cNDAccount, pMsgCharacCreate, pUser->iUid, rand()%10 , &uid);
		}
		catch (...) {
			::PrintConsole( "[EXCEPTION] DB-Call g_DB_User->pr_Charac_Create()\n" ) ;
			return;
		}

		
		if(nReturn == 1){			
			msgCharacCreateResult.cResult = 1;	// �ߺ��� �̸�
		}
		else if(nReturn == 2){			
			msgCharacCreateResult.cResult = 5;	// �ִ� ĳ���� �ʰ�
		}
		else  if(nReturn == -1 )
		{
			::PrintConsole("[DB_ERR] pr_Charac_Create db proc err \n") ;
		}
		else if(nReturn == 0){
			// �ʿ� �־��ְ� ĳ���� ������ GS ������� �Ѵ�.
			msgCharacCreateResult.cResult = 0;	// if Create character is succsess then set 'cRecult = 0'

			msgCharacCreateResult.iCharacUid = uid ;

			// ����ִ� �޸𸮸� ã�´�. 
			for(int i=0; i<3; i++){
				pCharac = &pUser->stCharac[i];
				if( pCharac->CHARAC_BASIC.iUniqueID != 0 )
				{
					continue ;
				}

				try{

					memcpy( pCharac->CHARAC_BASIC.cChrName, pMsgCharacCreate->cName, 13 ) ;

					bool bRet ;
					pCharac->charac_lock() ;
					try{
						bRet = g_DB_Command->GetCharacAllInfo( uid, pCharac) ;
					}
					catch(...){
						::PrintConsole( "[EXCEPTION] g_DB_Command->GetCharacAllInfo( uid, pCharac) chr:%s\n", pMsgCharacCreate->cName ) ;
						bRet = false ;
					}
					pCharac->charac_unlock() ;

					/*
					printf( "[Name Compare] Old,New Pair\n  => " ) ;
					for( int k = 0 ; k < 13 ; ++k )
						printf( "%d:%d  \n", pMsgCharacCreate->cName[k], pCharac->CHARAC_BASIC.cChrName[k] ) ;
					printf( "\n" ) ;
					*/
					
					// 2. Get character all info
					if( bRet == false){
						::PrintConsole( "[Error] : Get_Charac_Info fail - return result is false");
						// ĳ�� �о���⸦ �������� ��� ��ü ĳ���� �ٽ� �о�´�.
						if( g_DB_Command->Get_Charac_List( pUser->iUid, pUser) == false )
						{							
							pUser->user_lock() ;
							try{
								if( g_pAccMapManager->Erase( pUser) == false )
								{
									::PrintConsole("[ERROR] if( g_pAccMapManager->Erase( pUser) == false ) \n %s, %d \n", __FILE__,__LINE__) ;
								}
							}
							catch(...){
								::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
							}		
							pUser->user_unlock() ;
							pUser = NULL ;
							return ;
						}
					}
					else{

						// Update inventory						

						pInven = (_sServer_InvenItem_v1 *)pCharac->CHARAC_INVENTORY.cInventory;


						// ����
						if( pMsgCharacCreate->sAmor !=0 )
						{
							pInven[0].cType = 1 ;
							pInven[0].cSecond = 0 ;
							pInven[0].sID = pMsgCharacCreate->sAmor ;
							pInven[0].Item_Money.S_NormalItem.ucCount = 1 ;
							pInven[0].Item_Money.S_NormalItem.usCurDur = 3000 ;								
						}
						
						
						// ����
						if( pMsgCharacCreate->sPants != 0)
						{
							pInven[1].cType = 1 ;
							pInven[1].cSecond = 1 ;
							pInven[1].sID = pMsgCharacCreate->sPants;
							pInven[1].Item_Money.S_NormalItem.ucCount = 1 ;
							pInven[1].Item_Money.S_NormalItem.usCurDur = 2000 ;								
						}

						// �Ƶ�
						if( pMsgCharacCreate->sAdea != 0 )
						{
							pInven[2].cType =  1;
							pInven[2].cSecond = 2 ;
							pInven[2].sID = pMsgCharacCreate->sAdea ;
							pInven[2].Item_Money.S_NormalItem.ucCount = 1 ;
							pInven[2].Item_Money.S_NormalItem.usCurDur = 2000 ;
							
						}
						

						// �Ź�
						if( pMsgCharacCreate->sShoes != 0 )
						{
							pInven[3].cType = 1;
							pInven[3].cSecond = 3 ;
							pInven[3].sID = pMsgCharacCreate->sShoes ;
							pInven[3].Item_Money.S_NormalItem.ucCount = 1 ;
							pInven[3].Item_Money.S_NormalItem.usCurDur = 800 ;
							
						}

#ifdef _PD_CASTLE_EVENT_1_
						pInven[25].cType = 9;
						pInven[25].cSecond = 0;
						pInven[25].sID = 474 ;
						pInven[25].Item_Money.S_NormalItem.ucCount = 1 ;
						pInven[25].Item_Money.S_NormalItem.usCurDur = 0 ;

						CND_RealTime::CalcAfterDay( &pInven[25].uselimit, 7 );
#endif


						pCharac->CHARAC_INVENTORY.sVersion = 1 ;
						
						// party setting   ( 2004.6.15	 ozzywow add )
#ifdef _PARTY_SET
						if( pMsgCharacCreate->cInitZone == 1 )
							pCharac->CHARAC_BASIC.cParty = 3 ;
						else if ( pMsgCharacCreate->cInitZone == 2 )
							pCharac->CHARAC_BASIC.cParty = 1 ;
						else if ( pMsgCharacCreate->cInitZone == 3 )
							pCharac->CHARAC_BASIC.cParty = 4 ;
						else if ( pMsgCharacCreate->cInitZone == 4 )
							pCharac->CHARAC_BASIC.cParty = 2; 
						else
							pCharac->CHARAC_BASIC.cParty = 0 ;

#endif


						pUser->cCharacCount++;
						strncpy( pCharac->CHARAC_BASIC.cAccount, pUser->cNDAccount, en_max_lil+1 ) ;
						g_pAccMapManager->CharacInsert(pCharac);

						pCharac->CHARAC_ETC._backup.bBackupStateBasic = BS_READY ;	
						pCharac->CHARAC_ETC._backup.bBackupStateCurBasicInfo = BS_READY ;
#ifdef _PD_INVEN_SAVE_CHANGED_
						g_DB_Command->inven_update( uid, pCharac );
#else
						pCharac->CHARAC_ETC._backup.bBackupStateInven = BS_READY ;
#endif

						g_pBackupReady->InsertCharac( pCharac ) ;
						


						// �����̸� �α� �����..
						_CharacLog * pCharacLog =  g_pLogSvrCom->GetCharacLogSlot() ;
						if( pCharacLog )
						{
							memset( pCharacLog, 0, sizeof(_CharacLog) ) ;

							pCharacLog->code = _LOG_CHARAC_CREATE ;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
							memcpy( pCharacLog->cAccount, pUser->cNDAccount, en_max_lil);
							pCharacLog->cAccount[en_max_lil] = 0;
#else
							memcpy( pCharacLog->account, pUser->cNDAccount, 12);
							pCharacLog->account[12] = 0;
#endif
							memcpy( pCharacLog->charac, pCharac->CHARAC_BASIC.cChrName, en_charac_name_length);
							pCharacLog->charac[en_charac_name_length] = 0;
							pCharacLog->charac_uid = pCharac->CHARAC_BASIC.iUniqueID ;
							in_addr addr = {0};
							addr.S_un.S_addr = pUser->iIp ;
							memcpy( pCharacLog->ip, inet_ntoa( addr ), 17) ;
							pCharacLog->level = pCharac->CHARAC_STATE.sInnerLevel ;
							pCharacLog->point = pCharac->CHARAC_LEVEL.sLeftPoint ;								
							pCharacLog->zone = pUser->iServer ;						
							g_pLogSvrCom->InsertCharacLog( pCharacLog ) ;
						}
					}

				}
				catch (...) {
					::PrintConsole( "[EXCEPTION] DB-Call g_DB_User->Get_Charac_Info()\n" ) ;
					return;
				}

				break ;

									
			}//for					
		}
		else
		{
			// exception
			::PrintConsole("[EXCEPTION] Charac create db excption case (return value : %d) \n", nReturn) ;
		}

	}//else	

	// ����� ���� ������  send ������... 
	// ��� ���� ����
	sendError = pDSComm->Send(msgCharacCreateResult.usLength, (char*)&msgCharacCreateResult);
}


#ifdef _PD_COMPANY_JAPAN_

void CharacDelete(CMSG_CHARAC_DELETE_GS_DS * pMsgCharacDelete
				  , _h_DS_Com * pDSComm
				  ,  COLE_DB_Executer * g_DB_Command
				  ,  C_USER_DB_Command * g_UserDB_Command
				  ,  CRocAuthSession * pSession )

{
	// CMSG_CHARAC_DELETE_GS_DS (req) : GS -> DS
	// CMSG_CHARAC_RESULT_DS_GS (res) : DS -> GS

	pMsgCharacDelete->cAccount[en_max_lil] = 0 ;
	pMsgCharacDelete->cChrName[en_charac_name_length] = 0 ;

	PCHARAC				pCharac = NULL;				// Character info struct
	PUSER				pUser = NULL;				// User info struct

	int			sendError = 0;				//SendOK() ����	
	int			result = 0;					// Login DB result value;	


	CMSG_CHARAC_RESULT_DS_GS	msgCharacDeleteResult = {0} ;
	msgCharacDeleteResult.usLength = sizeof(CMSG_CHARAC_RESULT_DS_GS) ;
	msgCharacDeleteResult.cMessage = CMSG_NO_CHARAC_RESULT_DS_GS ;
	msgCharacDeleteResult.cPrevCommand = CMSG_NO_CHARAC_DELETE_GS_DS ;


	memcpy(msgCharacDeleteResult.cChrName, pMsgCharacDelete->cChrName, 13);											



#ifdef _TRACE_
	g_pTrace->OutputString( _TRACE_CLASS_CHARAC,"[Recv] CHARAC DELETE (ACC:%s, Chr:%s) \n",pMsgCharacDelete->cAccount, pMsgCharacDelete->cChrName ) ;
#endif			

	pUser = g_pAccMapManager->Find_User(pMsgCharacDelete->cAccount);

	if(pUser == NULL || pUser->cState == US_NONSTATE) 
	{
		PrintConsole( "[ERROR] Charac Delete = Not Found User\n" ) ;
		return;
	}



	pCharac = pUser->characFind( pMsgCharacDelete->cChrName )  ;


	// ���� : �ʿ� ����Ÿ�� �ִ��� Ȯ������
	if(pCharac == NULL)
	{
		PrintConsole( "[ERROR] Charac Delete = Not Found Charac\n" ) ;
		msgCharacDeleteResult.cResult = 2;	// ������ �� ����(DS_Packets.h)
		msgCharacDeleteResult.usUid = pUser->usTempUniqueID;
		if( pDSComm->Send(msgCharacDeleteResult.usLength, (char*)&msgCharacDeleteResult) == false ){
			::PrintConsole( "[SO_ERR] SendError : %s, %d \n", __FILE__, __LINE__ ) ;
		}	
		return ;
	}


	pUser->user_lock() ;
	pCharac->charac_lock() ;
	int	iAccUID = pUser->iUid ;
	int iCharacUID = pCharac->CHARAC_BASIC.iUniqueID ;
	pCharac->charac_unlock() ;
	pUser->user_unlock() ;

	// ���� : DB Password check ����
	if( pSession->IsAuthReady() )
	{
		sRocAuthResponse *	pResponse = NULL;
		bool bFail = false;

		try
		{
			if( ND_F_SUCCESS != pSession->RocRequestLogin( pMsgCharacDelete->cAccount, pMsgCharacDelete->cPassword, &pResponse ) )
			{
				msgCharacDeleteResult.usUid = pUser->usTempUniqueID;
				msgCharacDeleteResult.cResult = 1;	// ��ȣ�� Ʋ����
			}
			else 
			{
				if( false == pResponse->IsLoginSuccess() )
				{
					msgCharacDeleteResult.usUid = pUser->usTempUniqueID;
					msgCharacDeleteResult.cResult = 1;	// ��ȣ�� Ʋ����
				}
			}
		}
		catch( ... )
		{
			PrintConsole( "[DEL CHARAC] A<%s> Exception!!", pMsgCharacDelete->cAccount );
			msgCharacDeleteResult.usUid = pUser->usTempUniqueID;
			msgCharacDeleteResult.cResult = 1;	// ��ȣ�� Ʋ����
		}

		pSession->Disconnect();

		if( 1 == msgCharacDeleteResult.cResult )
		{
			PrintConsole( "[DEL CHARAC] A<%s> Rms Check fail", pMsgCharacDelete->cAccount );
			pDSComm->Send(msgCharacDeleteResult.usLength, (char*)&msgCharacDeleteResult);
			return;
		}
	}
	else
	{
		PrintConsole( "[DEL CHARAC] A<%s> Rms not connected ==> Success", pMsgCharacDelete->cAccount );
	}

	// ���� : DB Delete(����) Qry ����
	result = g_DB_Command->pr_Charac_Delete( iCharacUID ) ;
	if( result != 0 )
	{
		if( result == 2){
			msgCharacDeleteResult.usUid = pUser->usTempUniqueID;
			msgCharacDeleteResult.cResult = 1;	// ��ȣ�� Ʋ����
		}
		else{				
			msgCharacDeleteResult.usUid = pUser->usTempUniqueID;
			msgCharacDeleteResult.cResult = 2;	// ��Ÿ����
		}

#ifdef _TRACE_				
		g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[DB_ERR] : _prDS_CharacDelete fail - return result == %d \n", result);
#endif
		if( pDSComm->Send(msgCharacDeleteResult.usLength, (char*)&msgCharacDeleteResult) == false ){
			::PrintConsole( "[SO_ERR] SendError : %s, %d \n", __FILE__, __LINE__ ) ;
		}	
		return ;
	}


	// pCharac �޸� ���� �κ�
	pUser->user_lock() ;
	try
	{

		// �����̸� �α� �����..
		_CharacLog * pCharacLog =  g_pLogSvrCom->GetCharacLogSlot() ;
		if( pCharacLog )
		{
			memset( pCharacLog, 0, sizeof(_CharacLog) ) ;

			pCharacLog->code = _LOG_CHARAC_DELETE;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
			memcpy( pCharacLog->cAccount, pUser->cNDAccount, en_max_lil);
			pCharacLog->cAccount[en_max_lil] = 0;
#else
			memcpy( pCharacLog->account, pUser->cNDAccount, 12);
			pCharacLog->account[12] = 0;
#endif
			memcpy( pCharacLog->charac, pCharac->CHARAC_BASIC.cChrName, 13 ) ;
			pCharacLog->charac_uid = pCharac->CHARAC_BASIC.iUniqueID ;
			in_addr addr = {0};
			addr.S_un.S_addr = pUser->iIp ;
			memcpy( pCharacLog->ip, inet_ntoa( addr ), 17) ;
			pCharacLog->level = pCharac->CHARAC_STATE.sInnerLevel ;
			pCharacLog->point = pCharac->CHARAC_LEVEL.sLeftPoint ;								
			pCharacLog->zone = pUser->iServer ;						
			g_pLogSvrCom->InsertCharacLog( pCharacLog ) ;
		}

		// ���Ŀ� ���ԵǾ� �ִٸ� ���Ŀ��� �����ض�..
		if( pCharac->CHARAC_STATE.iORIndex > 0 )
		{
			_Organization * pOR = g_pOrganizationManager->FindOrganization( pCharac->CHARAC_STATE.iORIndex ) ;
			_Unit * pUnit = NULL ;
			if( pOR )
			{
				pUnit = pOR->FindUnit( pCharac->CHARAC_BASIC.cChrName ) ;
			}

			if( pUnit && pOR )
			{

				if( pUnit->m_cClass == _Unit::_UNIT_CALSS_CAPTAIN_UNIT_ )
				{							
					for( int i = 0 ; i < _MAX_OR_UNIT_NUM ; i++ )
					{
						if( pOR->m_MemberList[i].m_cName[0] != 0 && pOR->m_MemberList[i].m_cClass != _Unit::_UNIT_CALSS_CAPTAIN_UNIT_ )
						{
							g_pOrganizationManager->ExchangeCaptain( pOR->m_iIndex, pOR->m_MemberList[i].m_cName ) ;

							SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS msgORUnitControl = {0} ;
							msgORUnitControl.usLength = sizeof(SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS) ;
							msgORUnitControl.cMessage = SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS ;
							msgORUnitControl.iIndex = pOR->m_iIndex ;
							msgORUnitControl._unit = pOR->m_MemberList[i] ;
							msgORUnitControl.cCode = SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_ENTRUST_ ;

							g_pDSComQueue->GlobalSend( msgORUnitControl.usLength, (char*)&msgORUnitControl ) ;
							break ;
						}
					}

				}

				_Unit tempUnit = *pUnit ;
				int iRet = g_pOrganizationManager->PopORUnit( pCharac->CHARAC_STATE.iORIndex, pCharac->CHARAC_BASIC.cChrName ) ;
				if( iRet == -1 )
				{
#ifdef _PD_CASTLE_STEP_1_
					//! �������� ��ü�Ǿ���. ��� �˻絵 �Ѵ�.
					CDSCastleManager::GetObj().notice_group_breakup( en_cb_group_type_org, pCharac->CHARAC_STATE.iORIndex );
#endif
					SMSG_OR_RESULT_DS_GS msgORResult = {0} ;
					msgORResult.usLength = sizeof(SMSG_OR_RESULT_DS_GS) ;
					msgORResult.cMessage = SMSG_NO_OR_RESULT_DS_GS ;
					msgORResult.iIndex = pCharac->CHARAC_STATE.iORIndex ;
					msgORResult.iCode = SMSG_OR_RESULT_DS_GS::_OR_RESULT_DELETE_ ;
					g_pDSComQueue->GlobalSend( msgORResult.usLength, (char*)&msgORResult ) ;
				}
				else
				{
					SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS msgOrUnitControlReq = {0} ;
					msgOrUnitControlReq.usLength = sizeof(SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS) ;
					msgOrUnitControlReq.cMessage = SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS ;
					msgOrUnitControlReq.iIndex = pCharac->CHARAC_STATE.iORIndex ;
					msgOrUnitControlReq._unit = tempUnit ;
					msgOrUnitControlReq.cCode = SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_DELETE_ ;

					g_pDSComQueue->GlobalSend( msgOrUnitControlReq.usLength, (char*)&msgOrUnitControlReq ) ;						
				}
			}			

		}

		// MS ���� ��������� �뺸�ؾ��Ѵ�.
		UMSG_CHARAC_DELETE_DS_MS msgCharacDelete = {0} ;
		msgCharacDelete.iKey = PASS_KEY ;
		msgCharacDelete.cMessage = UMSG_CHARAC_DELETE_DS_MS_NUM ;

		memcpy( msgCharacDelete.cCharacName, pMsgCharacDelete->cChrName, 13 ) ;
		msgCharacDelete.iUid = pCharac->CHARAC_BASIC.iUniqueID ;
		SOCKADDR_IN	* pAddr = g_ServerInfo.GetMSSockAddrIn() ;

		g_pUDPSendPacketQ->insert_udp( pAddr, (char*)&msgCharacDelete, sizeof(msgCharacDelete) ) ;


		--pUser->cCharacCount;	//My character count --				

		// �ʿ��� �����Ѵ�.
		pCharac->charac_lock() ;
		try{
			if( g_pAccMapManager->CharacErase(pCharac) == false ) 
			{
				::PrintConsole( "[ERROR] if( g_pAccMapManager->CharacErase(pCharac) == false )  \n %s, %d \n", __FILE__, __LINE__ ) ;
			}
		}
		catch(...){
			::PrintConsole("[EXEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}
		pCharac->charac_unlock() ;

		msgCharacDeleteResult.cResult = 0;	// ����	
		msgCharacDeleteResult.usUid = pUser->usTempUniqueID;				

#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_CHARAC,"[INFO] Delete one character (%s) \n",pMsgCharacDelete->cChrName);
#endif

	}
	catch (...) {
		::PrintConsole( "[EXCEPTION] DB-Call g_DB_User->pr_Charac_Delete()\n" ) ;			
	}
	pUser->user_unlock() ;



	sendError = pDSComm->Send(msgCharacDeleteResult.usLength, (char*)&msgCharacDeleteResult);
	if(sendError == 0){
		::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", sendError ) ;
	}	
}

#else

void CharacDelete(CMSG_CHARAC_DELETE_GS_DS * pMsgCharacDelete
				  , _h_DS_Com * pDSComm
				  ,  COLE_DB_Executer * g_DB_Command
				  ,  C_USER_DB_Command * g_UserDB_Command )

{
	// CMSG_CHARAC_DELETE_GS_DS (req) : GS -> DS
	// CMSG_CHARAC_RESULT_DS_GS (res) : DS -> GS

	pMsgCharacDelete->cAccount[en_max_lil] = 0 ;
	pMsgCharacDelete->cChrName[en_charac_name_length] = 0 ;

	PCHARAC				pCharac = NULL;				// Character info struct
	PUSER				pUser = NULL;				// User info struct

	int			sendError = 0;				//SendOK() ����	
	int			result = 0;					// Login DB result value;	


	CMSG_CHARAC_RESULT_DS_GS	msgCharacDeleteResult = {0} ;
	msgCharacDeleteResult.usLength = sizeof(CMSG_CHARAC_RESULT_DS_GS) ;
	msgCharacDeleteResult.cMessage = CMSG_NO_CHARAC_RESULT_DS_GS ;
	msgCharacDeleteResult.cPrevCommand = CMSG_NO_CHARAC_DELETE_GS_DS ;

	
	memcpy(msgCharacDeleteResult.cChrName, pMsgCharacDelete->cChrName, 13);											



#ifdef _TRACE_
	g_pTrace->OutputString( _TRACE_CLASS_CHARAC,"[Recv] CHARAC DELETE (ACC:%s, Chr:%s) \n",pMsgCharacDelete->cAccount, pMsgCharacDelete->cChrName ) ;
#endif			

	pUser = g_pAccMapManager->Find_User(pMsgCharacDelete->cAccount);

	if(pUser == NULL || pUser->cState == US_NONSTATE) 
	{
		PrintConsole( "[ERROR] Charac Delete = Not Found User\n" ) ;
		return;
	}


	
	pCharac = pUser->characFind( pMsgCharacDelete->cChrName )  ;
	

	// ���� : �ʿ� ����Ÿ�� �ִ��� Ȯ������
	if(pCharac == NULL)
	{
		PrintConsole( "[ERROR] Charac Delete = Not Found Charac\n" ) ;
		msgCharacDeleteResult.cResult = 2;	// ������ �� ����(DS_Packets.h)
		msgCharacDeleteResult.usUid = pUser->usTempUniqueID;
		if( pDSComm->Send(msgCharacDeleteResult.usLength, (char*)&msgCharacDeleteResult) == false ){
			::PrintConsole( "[SO_ERR] SendError : %s, %d \n", __FILE__, __LINE__ ) ;
		}	
		return ;
	}


	pUser->user_lock() ;
	pCharac->charac_lock() ;
	int	iAccUID = pUser->iUid ;
	int iCharacUID = pCharac->CHARAC_BASIC.iUniqueID ;
	pCharac->charac_unlock() ;
	pUser->user_unlock() ;

	// ���� : DB Password check ����
	if( g_UserDB_Command->PWCheck( iAccUID, pMsgCharacDelete->cPassword ) == false ) 
	{
		msgCharacDeleteResult.usUid = pUser->usTempUniqueID;
		msgCharacDeleteResult.cResult = 1;	// ��ȣ�� Ʋ����
		if( pDSComm->Send(msgCharacDeleteResult.usLength, (char*)&msgCharacDeleteResult) == false ){
			::PrintConsole( "[SO_ERR] SendError : %s, %d \n", __FILE__, __LINE__ ) ;
		}	
		return ;
	}


	// ���� : DB Delete(����) Qry ����
	result = g_DB_Command->pr_Charac_Delete( iCharacUID ) ;
	if( result != 0 )
	{
		if( result == 2){
			msgCharacDeleteResult.usUid = pUser->usTempUniqueID;
			msgCharacDeleteResult.cResult = 1;	// ��ȣ�� Ʋ����
		}
		else{				
			msgCharacDeleteResult.usUid = pUser->usTempUniqueID;
			msgCharacDeleteResult.cResult = 2;	// ��Ÿ����
		}

#ifdef _TRACE_				
		g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[DB_ERR] : _prDS_CharacDelete fail - return result == %d \n", result);
#endif
		if( pDSComm->Send(msgCharacDeleteResult.usLength, (char*)&msgCharacDeleteResult) == false ){
			::PrintConsole( "[SO_ERR] SendError : %s, %d \n", __FILE__, __LINE__ ) ;
		}	
		return ;
	}
	

	// pCharac �޸� ���� �κ�
	pUser->user_lock() ;
	try
	{

		// �����̸� �α� �����..
		_CharacLog * pCharacLog =  g_pLogSvrCom->GetCharacLogSlot() ;
		if( pCharacLog )
		{
			memset( pCharacLog, 0, sizeof(_CharacLog) ) ;

			pCharacLog->code = _LOG_CHARAC_DELETE;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
			memcpy( pCharacLog->cAccount, pUser->cNDAccount, en_max_lil);
			pCharacLog->cAccount[en_max_lil] = 0;
#else
			memcpy( pCharacLog->account, pUser->cNDAccount, 12);
			pCharacLog->account[12] = 0;
#endif
			memcpy( pCharacLog->charac, pCharac->CHARAC_BASIC.cChrName, 13 ) ;
			pCharacLog->charac_uid = pCharac->CHARAC_BASIC.iUniqueID ;
			in_addr addr = {0};
			addr.S_un.S_addr = pUser->iIp ;
			memcpy( pCharacLog->ip, inet_ntoa( addr ), 17) ;
			pCharacLog->level = pCharac->CHARAC_STATE.sInnerLevel ;
			pCharacLog->point = pCharac->CHARAC_LEVEL.sLeftPoint ;								
			pCharacLog->zone = pUser->iServer ;						
			g_pLogSvrCom->InsertCharacLog( pCharacLog ) ;
		}

		// ���Ŀ� ���ԵǾ� �ִٸ� ���Ŀ��� �����ض�..
		if( pCharac->CHARAC_STATE.iORIndex > 0 )
		{
			_Organization * pOR = g_pOrganizationManager->FindOrganization( pCharac->CHARAC_STATE.iORIndex ) ;
			_Unit * pUnit = NULL ;
			if( pOR )
			{
				pUnit = pOR->FindUnit( pCharac->CHARAC_BASIC.cChrName ) ;
			}

			if( pUnit && pOR )
			{

				if( pUnit->m_cClass == _Unit::_UNIT_CALSS_CAPTAIN_UNIT_ )
				{							
					for( int i = 0 ; i < _MAX_OR_UNIT_NUM ; i++ )
					{
						if( pOR->m_MemberList[i].m_cName[0] != 0 && pOR->m_MemberList[i].m_cClass != _Unit::_UNIT_CALSS_CAPTAIN_UNIT_ )
						{
							g_pOrganizationManager->ExchangeCaptain( pOR->m_iIndex, pOR->m_MemberList[i].m_cName ) ;

							SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS msgORUnitControl = {0} ;
							msgORUnitControl.usLength = sizeof(SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS) ;
							msgORUnitControl.cMessage = SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS ;
							msgORUnitControl.iIndex = pOR->m_iIndex ;
							msgORUnitControl._unit = pOR->m_MemberList[i] ;
							msgORUnitControl.cCode = SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_ENTRUST_ ;

							g_pDSComQueue->GlobalSend( msgORUnitControl.usLength, (char*)&msgORUnitControl ) ;
							break ;
						}
					}
					
				}

				_Unit tempUnit = *pUnit ;
				int iRet = g_pOrganizationManager->PopORUnit( pCharac->CHARAC_STATE.iORIndex, pCharac->CHARAC_BASIC.cChrName ) ;
				if( iRet == -1 )
				{
#ifdef _PD_CASTLE_STEP_1_
					//! �������� ��ü�Ǿ���. ��� �˻絵 �Ѵ�.
					CDSCastleManager::GetObj().notice_group_breakup( en_cb_group_type_org, pCharac->CHARAC_STATE.iORIndex );
#endif
					SMSG_OR_RESULT_DS_GS msgORResult = {0} ;
					msgORResult.usLength = sizeof(SMSG_OR_RESULT_DS_GS) ;
					msgORResult.cMessage = SMSG_NO_OR_RESULT_DS_GS ;
					msgORResult.iIndex = pCharac->CHARAC_STATE.iORIndex ;
					msgORResult.iCode = SMSG_OR_RESULT_DS_GS::_OR_RESULT_DELETE_ ;
					g_pDSComQueue->GlobalSend( msgORResult.usLength, (char*)&msgORResult ) ;
				}
				else
				{
					SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS msgOrUnitControlReq = {0} ;
					msgOrUnitControlReq.usLength = sizeof(SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS) ;
					msgOrUnitControlReq.cMessage = SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS ;
					msgOrUnitControlReq.iIndex = pCharac->CHARAC_STATE.iORIndex ;
					msgOrUnitControlReq._unit = tempUnit ;
					msgOrUnitControlReq.cCode = SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_DELETE_ ;

					g_pDSComQueue->GlobalSend( msgOrUnitControlReq.usLength, (char*)&msgOrUnitControlReq ) ;						
				}
			}			

		}

		// MS ���� ��������� �뺸�ؾ��Ѵ�.
		UMSG_CHARAC_DELETE_DS_MS msgCharacDelete = {0} ;
		msgCharacDelete.iKey = PASS_KEY ;
		msgCharacDelete.cMessage = UMSG_CHARAC_DELETE_DS_MS_NUM ;

		memcpy( msgCharacDelete.cCharacName, pMsgCharacDelete->cChrName, 13 ) ;
		msgCharacDelete.iUid = pCharac->CHARAC_BASIC.iUniqueID ;
		SOCKADDR_IN	* pAddr = g_ServerInfo.GetMSSockAddrIn() ;

		g_pUDPSendPacketQ->insert_udp( pAddr, (char*)&msgCharacDelete, sizeof(msgCharacDelete) ) ;


		--pUser->cCharacCount;	//My character count --				
		
		// �ʿ��� �����Ѵ�.
		pCharac->charac_lock() ;
		try{
			if( g_pAccMapManager->CharacErase(pCharac) == false ) 
			{
				::PrintConsole( "[ERROR] if( g_pAccMapManager->CharacErase(pCharac) == false )  \n %s, %d \n", __FILE__, __LINE__ ) ;
			}
		}
		catch(...){
			::PrintConsole("[EXEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}
		pCharac->charac_unlock() ;
		
		msgCharacDeleteResult.cResult = 0;	// ����	
		msgCharacDeleteResult.usUid = pUser->usTempUniqueID;				

#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_CHARAC,"[INFO] Delete one character (%s) \n",pMsgCharacDelete->cChrName);
#endif
	
	}
	catch (...) {
		::PrintConsole( "[EXCEPTION] DB-Call g_DB_User->pr_Charac_Delete()\n" ) ;			
	}
	pUser->user_unlock() ;

				

	sendError = pDSComm->Send(msgCharacDeleteResult.usLength, (char*)&msgCharacDeleteResult);
	if(sendError == 0){
		::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", sendError ) ;
	}	
}

#endif

void CharacListReq(CMSG_CHARAC_LIST_REQ_GS_DS * pMsgChracListReq
				   , _h_DS_Com * pDSComm
				   , CMSG_USER_CHARAC_LIST_DS_GS * msgUserCharacList )
{
	pMsgChracListReq->cAccount[en_max_lil] = 0 ;
	//CMSG_USER_CHARAC_LIST_DS_GS				msgUserCharacList = {0};		// DS->GS ���Ӽ����� �α��� ��û�� ĳ������ ����Ʈ ��ȯ
	_CHARAC *			pCharac = NULL;				// Character info struct
	_USER *				pUser = NULL;				// User info struct

	_sServer_InvenItem_v1 * pInven = NULL;

	int			sendError = 0;				//SendOK() ����
	

#ifdef _TRACE_
	g_pTrace->OutputString( _TRACE_CLASS_CHARAC,"[INFO] Recv Request character list info (ACC:%s)\n",pMsgChracListReq->cAccount);
#endif
	

	pUser = g_pAccMapManager->Find_User(pMsgChracListReq->cAccount);

	if(pUser == NULL) return;

	msgUserCharacList->usUid = pUser->usTempUniqueID;
	memcpy(msgUserCharacList->cSelChrName, pUser->cSelChrName, 13);

	//EnterCriticalSection(&g_csCMap);
	//pCharac = g_pChrMapManager->find(pUser->cSelChrName);
	//LeaveCriticalSection(&g_csCMap);

	//if( pCharac == NULL ) return ;

	//pCharac->CHARAC_ETC.cState = CS_FIELD;

	memset( &msgUserCharacList->List, 0 , sizeof(_CharacList) * 3 ) ;

	for(int i = 0; i < 3 ; i++)
	{

		pCharac = (_CHARAC *)&pUser->stCharac[i];

		// Inventory convert			

		pInven = reinterpret_cast<_sServer_InvenItem_v1*>(pCharac->CHARAC_INVENTORY.cInventory) ; 


		msgUserCharacList->List[i].iUniqueID = pCharac->CHARAC_BASIC.iUniqueID;
		memcpy(msgUserCharacList->List[i].cName, pCharac->CHARAC_BASIC.cChrName, 13);
		memcpy(msgUserCharacList->List[i].cNick, pCharac->CHARAC_BASIC.cChrNic, 13);

		msgUserCharacList->List[i].cSex = (unsigned char)pCharac->CHARAC_BASIC.cSex;								
		msgUserCharacList->List[i].cFace = pCharac->CHARAC_BASIC.cFace;
		msgUserCharacList->List[i].cHair = pCharac->CHARAC_BASIC.cHair;
		msgUserCharacList->List[i].cGroup = pCharac->CHARAC_BASIC.cParty;


		msgUserCharacList->List[i].cAmorType = pInven[0].cType ;
		msgUserCharacList->List[i].cPantsType = pInven[1].cType ;
		msgUserCharacList->List[i].cAdaeType = pInven[2].cType ;
		msgUserCharacList->List[i].cShoesType = pInven[3].cType ;
		msgUserCharacList->List[i].cCapType = pInven[4].cType ;
		

		msgUserCharacList->List[i].cMaskType = pInven[7].cType ;		
		msgUserCharacList->List[i].cBackPackType = pInven[23].cType ;


		msgUserCharacList->List[i].cWeaponType = pInven[10].cType ;
		msgUserCharacList->List[i].sSelectWeaponID = pInven[10].sID ;

		msgUserCharacList->List[i].sAmor = pInven[0].sID  ;				// ����	0
		msgUserCharacList->List[i].sPants = pInven[1].sID ;				// ����	1
		msgUserCharacList->List[i].sAdae = pInven[2].sID ;				// �ƴ� 2
		msgUserCharacList->List[i].sShoes = pInven[3].sID ;				// �Ź� 3
		msgUserCharacList->List[i].sCap = pInven[4].sID ;				// ���� 4


		msgUserCharacList->List[i].sMask = pInven[7].sID ;
		msgUserCharacList->List[i].ucWeaponStrength = pInven[10].ucStrength ;
		msgUserCharacList->List[i].sBackPack = pInven[23].sID ;			// �賶 
		
		//msgUserCharacList->List[i].sSelectWeaponID = pCharac->CHARAC_INVENTORY.sSelectWeaponID;

		//5������ġ
		msgUserCharacList->List[i].sConstitution = pCharac->CHARAC_LEVEL.sConstitution;
		msgUserCharacList->List[i].sZen = pCharac->CHARAC_LEVEL.sZen;
		msgUserCharacList->List[i].sIntelligence = pCharac->CHARAC_LEVEL.sIntelligence;
		msgUserCharacList->List[i].sDexterity = pCharac->CHARAC_LEVEL.sDexterity;
		msgUserCharacList->List[i].sStr = pCharac->CHARAC_LEVEL.sStr;

		msgUserCharacList->List[i].sInnerLevel = pCharac->CHARAC_STATE.sInnerLevel;		

		msgUserCharacList->List[i].cClass = pCharac->CHARAC_BASIC.cClass ;
		msgUserCharacList->List[i].cClassGrade = pCharac->CHARAC_BASIC.cClassGrade ;

		if( pCharac->CHARAC_STATE.iBlockingEndDate < CUser::get_time_by_sec() )
		{
			pCharac->CHARAC_BASIC.cGM_Check = 0 ;
			pCharac->CHARAC_STATE.iBlockingEndDate = 0 ;
		}
		msgUserCharacList->List[i].iBlockingEndDate = pCharac->CHARAC_STATE.iBlockingEndDate ;
		msgUserCharacList->List[i].cGM = pCharac->CHARAC_BASIC.cGM_Check ;
		
	}	// for
		
	CopyMemory( &msgUserCharacList->aius, &pUser->AIUS, sizeof(_ACCOUNT_ITEM_USE_STATUS) ) ;

	sendError = pDSComm->Send(msgUserCharacList->usLength, (char*)msgUserCharacList);
	if(sendError == 0){
		::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", sendError ) ;
	}


#ifdef _TRACE_
	g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "  AIUS - Item %d-%d, End Time %u\n", 
		msgUserCharacList->aius.grade.cItemType, msgUserCharacList->aius.grade.sItemID, msgUserCharacList->aius.uiEndTime );
#endif

}

void LogoutFunc(CMSG_LOGOUT_GS_DS * pMsgLogout
				, _h_DS_Com * pDSComm
				,  C_USER_DB_Command * g_USER_DB_Command
				, COLE_DB_Executer * g_DB_Command) 
{	

	PCHARAC				pCharac = NULL;				// Character info struct
	PUSER				pUser = NULL;				// User info struct

    pMsgLogout->cAccount[en_max_lil] = 0 ;

	pUser = g_pAccMapManager->Find_User(pMsgLogout->cAccount);


	if(pUser == NULL){	//�α׾ƿ��� ����ũ ���̵� �������� ������..					
		::PrintConsole( "[error] Not found account to Logout <%s>.\n", pMsgLogout->cAccount ) ;
		return;
	}
	
	// ĳ���� ���
	pCharac = pUser->characFind(pUser->cSelChrName);

	pUser->user_lock() ;		
	try{
		if( pCharac )
		{
			pCharac->charac_lock() ;
			try{
				// 20080827 ���� 6��.
				pCharac->CHARAC_STATE.sPartyIndex = 0;
				pCharac->CHARAC_STATE.sPartySlotNo = 0;

				// ĳ�� ������ �����ϱ� ���� ����(����)�� ���� ���Ἲ�� �˻��϶�..
				if( pCharac->CHARAC_STATE.iORIndex > 0 )
				{
					_Organization * pOR = g_pOrganizationManager->FindOrganization( pCharac->CHARAC_STATE.iORIndex ) ;
					if( pOR ) 
					{
						_Unit * pUnit = pOR->FindUnit( pCharac->CHARAC_BASIC.cChrName ) ;
						// ����(����)�� ���� �α��� ������ �����Ѵ�.
						if( pUnit ) 
						{
							pUnit->m_bOnline = false ;
							pUnit->m_cZone = static_cast<char>(pCharac->CHARAC_CUR_BASIC.sZone) ;
							pUnit->m_sInnerLevel = pCharac->CHARAC_STATE.sInnerLevel ;

							SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS msgUnitControl = {0} ;
							msgUnitControl.usLength = sizeof(SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS ) ;
							msgUnitControl.cMessage = SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS ;
							msgUnitControl.iIndex = pCharac->CHARAC_STATE.iORIndex ;
							msgUnitControl._unit = *pUnit ;
							msgUnitControl.cCode = SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_UPDATE_ ;

							g_pDSComQueue->GlobalSend( msgUnitControl.usLength, (char*)&msgUnitControl ) ;
						}
						// ������ ���� ������ ���ٸ� �����߿� ���� �����ȰŴ�.. ���� ���� �ε����� 0���� �ֱ�ȭ�϶�..
						else
						{
							pCharac->CHARAC_STATE.iORIndex = 0 ;
							pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY ;			
						}
					}
					// �ش� ������ ã�� ���ߴٸ� ������ �̹� �ػ�ȰŴ�. ���� ���� �ε����� 0���� �ʱ�ȭ �϶�..
					else
					{
						pCharac->CHARAC_STATE.iORIndex = 0 ;
						pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY ;		
					}
				}

				for(int i=0; i<3; i++){
					pUser->stCharac[i].CHARAC_ETC.cState = CS_NONSTATE;		
				}

				pCharac->CHARAC_ETC._sBACKUP_STATE_CHECKED = short(0xffff) ;		
				if(g_DB_Command->Update_Charac_All(pCharac->CHARAC_BASIC.iUniqueID, pCharac))
				{
					pCharac->CHARAC_ETC._sBACKUP_STATE_CHECKED = BS_COMPLETED;
				}
				else{
					::PrintConsole("[INFO] Quick backup failed(acc:%s) \n", pMsgLogout->cAccount );
				}


				g_DB_Command->CharacLogout( pCharac->CHARAC_BASIC.iUniqueID ) ;


			}
			catch(...){
				::PrintConsole("[EXCEPTION] %s , %d \n", __FILE__, __LINE__ ) ;
			}			
			pCharac->charac_unlock() ;	
		}

		// â�� ���
		if( pUser && pUser->cDepotBackupState != BS_COMPLETED ) 
		{
			if( !g_DB_Command->UpdateAccountItemUseStatus( pUser->iUid, pUser ) )
			{
				::PrintConsole( "[DB_ERR] AIUS Backup failed (%s)\n ", pUser->cNDAccount );
			}

			if(!g_DB_Command->pr_UpdateDepot(pUser->iUid, pUser) ) {
				// �α׿� ������..
				::PrintConsole( "[DB_ERR] Save depot is failed  (%s) ", pUser->cNDAccount );
			}		
			else{							
				pUser->cDepotBackupState = BS_COMPLETED;							
				//g_pTrace->OutputString( _TRACE_CLASS_BACKUP
				//	, "[BACKUP] Backup Account's depot ( Charac name : %s ) \n"
				//	, pUser->cNDAccount ) ;		
			}						
		}	


#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[INFO] Return tempUID with Logout (%d)\n", pUser->usTempUniqueID) ;
#endif			
		// ����ũ ���̵� �ݳ�
		g_pUniqueIDQueue->RecycleUID( pUser->usTempUniqueID ) ;

		// �α� �ƿ� �ϱ� ���� ���������� ĳ���� ������ DB �� ����ؾ����� ������?
		pUser->usTempUniqueID = 0 ;
		pUser->cState = US_NONSTATE;
		pUser->iTime = CUser::get_time_by_sec() ;
		memset(pUser->cSelChrName, 0, 13);	
	}
	catch(...){
		::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
	}	
	pUser->user_unlock() ;
					
	
	// DB Logout
	g_USER_DB_Command->DSLogout( pUser->cNDAccount ) ;


	in_addr addr = {0};
	_CharacLog * pCharacLog =  g_pLogSvrCom->GetCharacLogSlot() ;
	if( pCharacLog )
	{
		memset( pCharacLog, 0, sizeof(_CharacLog) ) ;

		pCharacLog->code = _LOG_CHARAC_LOGOUT_ ;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
		memcpy( pCharacLog->cAccount, pUser->cNDAccount, en_max_lil);
		pCharacLog->cAccount[en_max_lil] = 0;
#else
		memcpy( pCharacLog->account, pUser->cNDAccount, 12);
		pCharacLog->account[12] = 0;
#endif
		addr.S_un.S_addr = pUser->iIp ;
		memcpy( pCharacLog->ip, inet_ntoa( addr ), 17) ;
		pCharacLog->zone = pUser->iServer ;

		
		CTime ct = CTime::GetCurrentTime() ;		
		pCharacLog->level = static_cast<short>( (ct.GetTime()-(pUser->dwLoginTime * 60)) / 60 )   ;
		g_pLogSvrCom->InsertCharacLog( pCharacLog ) ;
	}		

	if( pCharac )	// pCharac == NULL �̸� �κ� ���Դ� �����Ŵ�.
	{
		_LoginLog * pLoginLog = g_pLogSvrCom->GetLoginLogSlot() ;
		if( pLoginLog )
		{
			memset( pLoginLog, 0, sizeof(_LoginLog) ) ;

			pLoginLog->acc_uid = pUser->iUid ;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
			memcpy( pLoginLog->cAccount, pUser->cNDAccount, en_max_lil );
			pLoginLog->cAccount[en_max_lil] = 0;
			memcpy( pLoginLog->cCharac, pCharac->CHARAC_BASIC.cChrName, en_charac_name_length ) ;
			pLoginLog->cCharac[en_charac_name_length] = 0;
#else
			memcpy(pLoginLog->acc, pUser->cNDAccount, 11 );
			pLoginLog->acc[11] = 0 ;
			memcpy( pLoginLog->charac, pCharac->CHARAC_BASIC.cChrName, en_charac_name_length-1 ) ;
			pLoginLog->charac[en_charac_name_length-1] = 0;
#endif
			pLoginLog->inner_level = pCharac->CHARAC_STATE.sInnerLevel ;
			sprintf( pLoginLog->ip, "%s", inet_ntoa( addr ) ) ;

			CTime ct( pUser->dwLoginTime * 60  )  ;
			sprintf(pLoginLog->login_time, "%u-%u-%u %u:%u:%u"
				, ct.GetYear(), ct.GetMonth(), ct.GetDay(), ct.GetHour(), ct.GetMinute(), ct.GetSecond() ) ;
			ct = CTime::GetCurrentTime() ;
			sprintf(pLoginLog->logout_time, "%u-%u-%u %u:%u:%u"
				, ct.GetYear(), ct.GetMonth(), ct.GetDay(), ct.GetHour(), ct.GetMinute(), ct.GetSecond() ) ;
			pLoginLog->play_time = static_cast<int>( (ct.GetTime() - (pUser->dwLoginTime * 60)) / 60 ) ;
			pLoginLog->m_class = pCharac->CHARAC_BASIC.cClass ;
			pLoginLog->party = pCharac->CHARAC_BASIC.cParty ;

			pLoginLog->pe_pay_code[0] = (char)pUser->AIUS.grade.sItemID ;
			pLoginLog->pe_pay_code[1] = 0 ;

			pLoginLog->type = _LOG_TYPE_LOGIN ;
			pLoginLog->zone_no = g_ForestNum ;

			pLoginLog->pc_pay_code[0] = 'H' ;
			g_pLogSvrCom->InsertLoginLog( pLoginLog ) ;
		}
	}	


	// �޸� ���� ����..
	g_pMemSaveTimer->Reserve_save( pUser, pUser->iUid, CUserMemSaveTimer::get_time_by_sec() + _MEM_SAVE_DELAY_TIME_LOGOUT ) ;
	
	// ���� ���� �α׸� ���Ϸ� �����..			
	g_pTrace->OutputString( _TRACE_CLASS_CHARAC,  "[INFO] #LOG OUT# UID : %d ACC: %s\n", pUser->iUid, pUser->cNDAccount  ) ;

}

void MoveServer(CMSG_REQ_SV_MOVE_GS_DS * pMsgNoSvMoveReq
				, _h_DS_Com * pDSComm
				, COLE_DB_Executer * g_DB_Command
				, CMSG_SV_MOVE_RESULT_DS_GS * msgSvMoveResult)
{


	if( pDSComm == NULL )
	{
		::PrintConsole( "MoveServer( ..., pDSComm = NULL, ...) \n" ) ;
	}

	pMsgNoSvMoveReq->cAccount[en_max_lil] = 0 ;
	pMsgNoSvMoveReq->cSelectCharac[en_charac_name_length] = 0 ;

	PCHARAC				pCharac = NULL;				// Character info struct
	PUSER				pUser = NULL;				// User info struct

	int			sendError = 0;				//SendOK() ����


	int pos = 0 ;
	try
	{
#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_CHARAC, "[INFO] Recv CMSG_NO_REQ_SV_MOVE_GS_DS (ACC:%s, CHR:%s)\n",
			pMsgNoSvMoveReq->cAccount, pMsgNoSvMoveReq->cSelectCharac);
#endif
		pos = 1 ;

		pUser = g_pAccMapManager->Find_User(pMsgNoSvMoveReq->cAccount);

		pos = 2 ;

		if(pUser == NULL)	{
			::PrintConsole("[ME_ERR] Not found account from map(%s) \n", pMsgNoSvMoveReq->cAccount);		
			return;
		}
		// ozzywow edit 2004.6.24
		else if(pUser->cState == US_NONSTATE) {
			::PrintConsole("[ME_ERR] Account Is Logouted (%s) \n", pMsgNoSvMoveReq->cAccount);
			return;
		}
		else
		{

			pos = 3 ;
			pCharac = pUser->characFind( pMsgNoSvMoveReq->cSelectCharac )  ;
			

			if(!pCharac){
				::PrintConsole("[ME_ERR] Not found account from map<selected_charac:%s>(acc:%s) \n", 				
					pMsgNoSvMoveReq->cSelectCharac, pMsgNoSvMoveReq->cAccount);


				CMSG_USER_LOGOUT_DS_GS msgUserkill = {0} ;
				msgUserkill.usLength = sizeof(CMSG_USER_LOGOUT_DS_GS) ;
				msgUserkill.cMessage = CMSG_NO_USER_LOGOUT_DS_GS ;							

				msgUserkill.cReason = 2 ;				// ����( ����... )	

				memcpy( msgUserkill.cNDAccount,  pMsgNoSvMoveReq->cAccount, en_max_lil+1) ;

				// DS -> GS
				_h_DS_Com * pDSCom ;
				if( pDSCom = g_ServerInfo.GetDSCom( pUser->iServer ) )
				{									
					if( pDSCom->Send( msgUserkill.usLength, (char *)&msgUserkill ) == false ){
						::PrintConsole("[SO_ERR] MSG_CMD_USERKILL_NUM \n");
					}
				}		


				for( int i = 0 ; i < 3 ; i++ )
				{
					::PrintConsole( " - %s \n", pUser->stCharac[i].CHARAC_BASIC.cChrName ) ;
				}				

				pUser->user_lock() ;
				try{
					if( g_pAccMapManager->Erase( pUser) == false )
					{
						::PrintConsole("[ERROR] if( g_pAccMapManager->Erase( pUser) == false ) \n %s, %d \n", __FILE__,__LINE__) ;
					}
				}
				catch(...){
					::PrintConsole( "[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
				}		
				pUser->user_unlock() ;
				pUser = NULL ;

				return;						
			}		
		}				

		pos = 4 ;

		memcpy(pUser->cSelChrName, pMsgNoSvMoveReq->cSelectCharac, 13);
		pUser->iServer = pMsgNoSvMoveReq->cZone;
		pUser->cState = US_MOVE;	// ���� ���¸� ���� �̵������� ����
		//pUser->cBackupState = BS_READY;	
		//g_pBackupReady->InsertAcc( pUser ) ;

		pos = 5 ;

		pUser->iTime = CUser::get_time_by_sec() ;

		// ���� uniqueID �� ����ʿ� ��ȯ����..		
		g_pUniqueIDQueue->RecycleUID( pUser->usTempUniqueID ) ;

		pos = 6 ;


#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_CHARAC
			, "[INFO] Ready complete to zone move (ACC:%s, CHR:%s) \n"
			, pUser->cNDAccount
			, pMsgNoSvMoveReq->cSelectCharac);
#endif


		if(pMsgNoSvMoveReq->bUpdatePos){			
			pCharac->CHARAC_CUR_BASIC.sZone = pMsgNoSvMoveReq->cZone;
			pCharac->CHARAC_CUR_BASIC.fX = pMsgNoSvMoveReq->fGotoPosX;
			pCharac->CHARAC_CUR_BASIC.fZ = pMsgNoSvMoveReq->fGotoPosZ;

		}

		pos = 7 ;

		pCharac->CHARAC_ETC.cState = CS_MOVE;

		msgSvMoveResult->usUid = pUser->usTempUniqueID;
		sendError = pDSComm->Send(msgSvMoveResult->usLength, (char*)msgSvMoveResult);
		if(sendError == 0){
			::PrintConsole( "[SO_ERR] pDSComm->send_request : code = %d\n", sendError ) ;
		}	

		pos = 8 ;

	}
	catch (...) 
	{
		::PrintConsole("[EXCEPTION] MoveServer( .... ) pos:%d \n", pos ) ;
	}
}

void ReturnLobby(CMSG_CHARAC_RETURN_LOBBY_GS_DS * pMsgCharacReturnLobby
				 , _h_DS_Com * pDSComm
				 ,  COLE_DB_Executer * g_DB_Command)
{	
	PCHARAC				pCharac = NULL;				// Character info struct
	PUSER				pUser = NULL;				// User info struct

	pMsgCharacReturnLobby->cCName[en_charac_name_length] = 0 ;
			
	pCharac = g_pAccMapManager->Find_Charac(pMsgCharacReturnLobby->cCName);
	pUser = g_pAccMapManager->Find_User( pMsgCharacReturnLobby->cNDAccount);

	PrintConsole( "[Return Lobby] A<%s>\n", pMsgCharacReturnLobby->cNDAccount );

	if( pCharac){

		if( pUser )
		{
			PrintConsole( " ==> Set\n" );
			pUser->bCheckCurEvent = 0;
		}

		pCharac->CHARAC_ETC.cState = CS_STANDBY;		
		

		pCharac->CHARAC_ETC._backup.bBackupStateBasic = BS_READY ;
		pCharac->CHARAC_ETC._backup.bBackupStateLevel = BS_READY ;
		pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY ;
		pCharac->CHARAC_ETC._backup.bBackupStateCurBasicInfo = BS_READY ;
		pCharac->CHARAC_ETC._backup.bBackupStateInven = BS_READY ;
		pCharac->CHARAC_ETC._backup.bBackupStateQuest = BS_READY ;
		pCharac->CHARAC_ETC._backup.bBackupStateMartial = BS_READY ;
		pCharac->CHARAC_ETC._backup.bBackupStateNic = BS_READY ;

		pCharac->CHARAC_ETC._backup.bBackupStateEffect = BS_READY ;
		pCharac->CHARAC_ETC._backup.bBackupStateMatch = BS_READY ;
		pCharac->CHARAC_ETC._backup.bBackupStatePkRecord = BS_READY ;
		pCharac->CHARAC_ETC._backup.bBackupStateCharacItemUseStatus = BS_READY ;
		
		g_pBackupReady->InsertCharac( pCharac ) ;

		pCharac->charac_lock() ;
		try{
			// 20080827 ���� 6��.
			pCharac->CHARAC_STATE.sPartyIndex = 0;
			pCharac->CHARAC_STATE.sPartySlotNo = 0;

			// ĳ�� ������ �����ϱ� ���� ����(����)�� ���� ���Ἲ�� �˻��϶�..
			if( pCharac->CHARAC_STATE.iORIndex > 0 )
			{
				_Organization * pOR = g_pOrganizationManager->FindOrganization( pCharac->CHARAC_STATE.iORIndex ) ;
				if( pOR ) 
				{
					_Unit * pUnit = pOR->FindUnit( pCharac->CHARAC_BASIC.cChrName ) ;
					// ����(����)�� ���� �α��� ������ �����Ѵ�.
					if( pUnit ) 
					{
						pUnit->m_bOnline = false ;
						pUnit->m_cZone = static_cast<char>(pCharac->CHARAC_CUR_BASIC.sZone) ;
						pUnit->m_sInnerLevel = pCharac->CHARAC_STATE.sInnerLevel ;

						SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS msgUnitControl = {0} ;
						msgUnitControl.usLength = sizeof(SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS ) ;
						msgUnitControl.cMessage = SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS ;
						msgUnitControl.iIndex = pCharac->CHARAC_STATE.iORIndex ;
						msgUnitControl._unit = *pUnit ;
						msgUnitControl.cCode = SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS::_UNIT_CONTROL_UPDATE_ ;

						g_pDSComQueue->GlobalSend( msgUnitControl.usLength, (char*)&msgUnitControl ) ;
					}
					// ������ ���� ������ ���ٸ� �����߿� ���� �����ȰŴ�.. ���� ���� �ε����� 0���� �ֱ�ȭ�϶�..
					else
					{
						pCharac->CHARAC_STATE.iORIndex = 0 ;
						pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY ;			
					}
				}
				// �ش� ������ ã�� ���ߴٸ� ������ �̹� �ػ�ȰŴ�. ���� ���� �ε����� 0���� �ʱ�ȭ �϶�..
				else
				{
					pCharac->CHARAC_STATE.iORIndex = 0 ;
					pCharac->CHARAC_ETC._backup.bBackupStateState = BS_READY ;		
				}
			}

			// ĳ���� ������ ��ü������ ������Ʈ �����..
			if(!g_DB_Command->Update_Charac_All(pCharac->CHARAC_BASIC.iUniqueID, pCharac)){
				::PrintConsole( "[DB_ERR] : Update_Charac_All() return false    chr_name : %s \n", 
					pCharac->CHARAC_BASIC.cChrName  );
			}
		}
		catch (...) {
			::PrintConsole( "[EXCEPTION] DB-Call g_DB_User->Update_Charac_All()\n" ) ;			
		}
		pCharac->charac_unlock() ;

	}
	else{
		::PrintConsole( "[ME_ERR] : Not found charac from map.    chr_name : %s \n", 
			pMsgCharacReturnLobby->cCName  );
		return;
	}	
}


bool UpdateCharacAll( MSG_UPDATE_CHARAC_ALL * pMsgUpdateCharacAll )
{	
	pMsgUpdateCharacAll->cAccount[en_max_lil] = 0 ;
	pMsgUpdateCharacAll->cCharacName[en_charac_name_length] = 0 ;

	PCHARAC pCharac = NULL ;
	PUSER pUser = g_pAccMapManager->Find_User( pMsgUpdateCharacAll->cAccount ) ;
	if( pUser )
	{
		pCharac = pUser->characFind( pMsgUpdateCharacAll->cCharacName ) ;
		if( pCharac == NULL )
		{
			::PrintConsole( "[ERROR] Not find character (acc:%s, chr:%s) \n ", pMsgUpdateCharacAll->cAccount, pMsgUpdateCharacAll->cCharacName ) ;	
			return false;
		}
	}
	else
	{
		::PrintConsole( "[ERROR] Not find Account (acc:%s, chr:%s) \n ", pMsgUpdateCharacAll->cAccount, pMsgUpdateCharacAll->cCharacName ) ;	
		return false ;
	}

	
	pUser->user_lock() ;
	pCharac->charac_lock() ;
	try{

		short sVersion = pUser->stUserDepot.sVersion ;
		memcpy( &pUser->stUserDepot, &pMsgUpdateCharacAll->storage, sizeof( _USER_DEPOT ) ) ;
		pUser->stUserDepot.sVersion = sVersion ;
		if( pMsgUpdateCharacAll->aius.bChanged )
			memcpy( &pUser->AIUS, &pMsgUpdateCharacAll->aius, sizeof(_ACCOUNT_ITEM_USE_STATUS) ) ;

		pUser->cDepotBackupState = BS_READY ;
		g_pBackupReady->InsertAcc( pUser ) ;


		try{

			// ���� �̵��� ������ 5�� �̳��� ���� ��Ŷ�� Ư���ϰ� ó���ؾ� �Ѵ�..
#ifdef _PD_MASTER_PUPIL_
			_CHARAC_MP mpInfo;
			CopyMemory( &mpInfo, &pCharac->CHARAC_MP, sizeof(_CHARAC_MP) );
#endif
			if( pUser->cState == US_MOVE && pUser->iTime + _RELOGIN_ABLE_TIME_SEC > CUser::get_time_by_sec() && pMsgUpdateCharacAll)
			{
				short	sZone = pCharac->CHARAC_CUR_BASIC.sZone ;					//����  2
				short	sY = pCharac->CHARAC_CUR_BASIC.sY ;						//  4
				float	fX = pCharac->CHARAC_CUR_BASIC.fX ;						//��ġ��ǥ 8
				float	fZ = pCharac->CHARAC_CUR_BASIC.fZ ;						// 12		

				
				memcpy( pCharac, &pMsgUpdateCharacAll->charac, sizeof( _CHARAC ) - sizeof(_CHARAC_ETC) ) ;
				

				pCharac->CHARAC_CUR_BASIC.sZone = sZone ;
				pCharac->CHARAC_CUR_BASIC.sY = sY ;
				pCharac->CHARAC_CUR_BASIC.fX = fX ;
				pCharac->CHARAC_CUR_BASIC.fZ = fZ ;
			}
			else
			{				

				memcpy( pCharac, &pMsgUpdateCharacAll->charac, sizeof( _CHARAC ) - sizeof(_CHARAC_ETC) ) ;
				memcpy( &pCharac->CHARAC_ETC_QUICKSLOT, &pMsgUpdateCharacAll->quickslot, sizeof(_CHARAC_ETC_QUICKSLOT) ) ;

#ifdef _PD_CHARAC_LTS_
				memcpy( &pCharac->CHARAC_LTS, &pMsgUpdateCharacAll->characLts, sizeof(_CHARAC_LTS) );
#endif
			}

#ifdef _PD_MASTER_PUPIL_
			CopyMemory( &pCharac->CHARAC_MP, &mpInfo, sizeof(_CHARAC_MP) );
#endif

#ifdef _PD_EVENT_INVITE_FRIEND_
			pUser->iIF_DBIndex = pMsgUpdateCharacAll->iIF_DBIndex;
			pUser->iIF_EndTime = pMsgUpdateCharacAll->iIF_EndTime;
#endif


			pCharac->CHARAC_ETC._sBACKUP_STATE_CHECKED = short(0xffff) ;
			g_pBackupReady->InsertCharac( pCharac ) ;
		}
		catch(...){
			::PrintConsole( "[EXCEPTION] %s %d \n", __FILE__, __LINE__) ;
			::PrintConsole( "[EXCEPTION] UpdateCharacAll() - charac info copy \n" ) ;
		}		
	}
	catch(...){
		::PrintConsole( "[EXCEPTION] %s %d \n", __FILE__, __LINE__) ;
		::PrintConsole( "[EXCEPTION] UpdateCharacAll() - user depot info copy \n" ) ;
	}	
	pCharac->charac_unlock() ;
	pUser->user_unlock() ;

	return true ;
}

void ProcSerialEvent( CMSG_EVENT_AUTH_REQ_WITH_KEY * pSerialEvent, C_USER_DB_Command * pUserDB, _h_DS_Com * pDSCom )
{
	PUSER				pUser = NULL;				// User info struct	

	CMSG_ONE_EVENT_RESULT	pkEventResult;

	/*  CMSG_EVENT_RESULT 
		short				sEventID ;

		char				cCharacName[13] ;		// 

		char				cItemType ;				// ������ Ÿ��	: package type = -1;
		short				sItemID ;				// ������ ���̵�
		u_char				ucItemCount ;			// �⺻ 1

		u_char				ucResult ;				//
		int					index;					// 
	*/
	// Set Default Value.
	pkEventResult.sEventID = pSerialEvent->sEventID;
	strncpy(pkEventResult.cCharacName, pSerialEvent->cCharacName, 13 );
	pkEventResult.cItemType = -1;
	pkEventResult.sItemID = 0;
	pkEventResult.ucItemCount = 0;
	pkEventResult.ucResult = en_event_errno_blank;
	pkEventResult.index = 0;

	// ĳ���ͱ��� ã�� �ʿ� ����. �׳� �������� ã�´�.
	pUser = g_pAccMapManager->Find_User(pSerialEvent->cNDAccount);    
	if(pUser)
	{
		short sRewardPackage = 0;
		int index = 0;

#ifdef _NXPCB_AWARD_200703_
		short result = pUserDB->DSEventCheck( pSerialEvent->sEventID, pSerialEvent->cKey, pUser->cNXAcc, 
			pSerialEvent->cCharacName, g_ForestNum, index, sRewardPackage );
#else
		short result = pUserDB->DSEventCheck( pSerialEvent->sEventID, pSerialEvent->cKey, pUser->cNDAccount, 
			pSerialEvent->cCharacName, g_ForestNum, index, sRewardPackage );
#endif

		PrintConsole( "[S-EVENT] : A<%s> DSEventCheck %d:%s \n", pSerialEvent->cNDAccount, result, pSerialEvent->cKey );

		/*
		enum DB_RESULT_EVENT_CODE
		{
			_db_result_not_found = 0,		// No key
			_db_result_used = 1,			// used
			_db_result_wait_a_moment = 2,	// wait
			_db_result_success = 3,			// success
		};
		*/

		switch( result )
		{
		case _db_result_not_found:
			pkEventResult.ucResult = en_event_errno_no_key;
			break;
		case _db_result_used:
			pkEventResult.ucResult = en_event_errno_used;
			break;
		case _db_result_success:
			pkEventResult.ucResult = en_event_success;

			pkEventResult.cItemType = -1;		// for Reward Package
			pkEventResult.sItemID = sRewardPackage;
			pkEventResult.ucItemCount = 1;
			pkEventResult.index = index;			// �̰��� GS���� �������� ����Ѵ�.

			PrintConsole( "[S-EVENT] : A<%s> Success! - Reward %d, index = %d\n", pSerialEvent->cNDAccount, sRewardPackage, index );

			break;
		case _db_result_wait_a_moment:
			pkEventResult.ucResult = en_event_errno_wait;
			break;
		}
	}
	else
	{
		//������ �ʿ� ����.. ����ó���ض�..
		PrintConsole( "[S-EVENT] : Account Not Found A<%s>\n", pSerialEvent->cNDAccount );

		pkEventResult.ucResult = en_event_errno_no_charac;
	}

	pDSCom->Send( pkEventResult.usLength, (char*)&pkEventResult );


}

void ProcEventCheck( CMSG_EVENT_CHECK_REQ * pMsgCheck, C_USER_DB_Command * pUserDB, _h_DS_Com * pDSCom )
{
	PrintConsole( "[EVENT] A<%s> Recv Check req\n", pMsgCheck->cNDAccount );
	CMSG_PRE_NOTICE_EVENT_RESULT pkNoticeResult;

	PUSER				pUser = NULL;				// User info struct	
	pUser = g_pAccMapManager->Find_User(pMsgCheck->cNDAccount);   

	if( NULL == pUser )
		return;


	int i = 0; 
	for( ; i < _NDD_PRECHECK_EVENT_COUNT_ ; ++i )
	{
		pkNoticeResult.EventResult[i].sEventID = pMsgCheck->curEventList[i];
		if( pMsgCheck->curEventList[i] )
		{
#ifdef _NXPCB_AWARD_200703_
			if( _db_result_not_checked == pUserDB->DSEventPreCheck( pMsgCheck->curEventList[i], 
				pUser->cNXAcc, pMsgCheck->cCharacName, g_ForestNum ) )
#else
			if( _db_result_not_checked == pUserDB->DSEventPreCheck( pMsgCheck->curEventList[i], 
				pMsgCheck->cNDAccount, pMsgCheck->cCharacName, g_ForestNum ) )
#endif
				pkNoticeResult.EventResult[i].ucResult = en_event_is_going_on;
			else
				pkNoticeResult.EventResult[i].ucResult = en_event_errno_event_end;
		}
	}

	// ���� �������� ���� �̺�Ʈ�� �ִ� ���̴�.
	strncpy( pkNoticeResult.cAccount, pMsgCheck->cNDAccount, en_max_lil );
	pkNoticeResult.cAccount[en_max_lil] = 0;
	strncpy( pkNoticeResult.cCharacName, pMsgCheck->cCharacName, en_charac_name_length );
	pkNoticeResult.cCharacName[en_charac_name_length] = 0;

	pDSCom->Send( pkNoticeResult.usLength, (char*)&pkNoticeResult );

	PrintConsole( "[EVENT] C<%s> ==> Send Result\n", pkNoticeResult.cCharacName );
}


#ifdef _VINA_SHUTDOWN_20061023_
// UserDB�� sFatigueWeight ����
BOOL SaveFatigueWeight(int iUid, short sFatigueWeight, C_USER_DB_Command* g_USER_DB_Command)
{
	try {
		return g_USER_DB_Command->UpdateFatigueInfo( iUid, sFatigueWeight);
	}
	catch(...) {
		::PrintConsole("catch(UpdateFatigueInfo) \n");
		return FALSE;
	}
}
#endif

#ifdef _PD_LEVEL_PRESENT_
void ProcLP_GetNotRecv( CMSG_LP_GETNOTRECV_REQ * pReq, COLE_DB_Executer * pGameDB, _h_DS_Com * pDSCom )
{
	PrintConsole( "[LP] Recv GetNotRecv_Req : From %s\n", pReq->cNDAccount );

	short eid = pGameDB->LP_GetNotRecv( pReq->cNDAccount );

	CMSG_LP_GETNOTRECV_ANS pkAns;
	pkAns.SetAccount( pReq->cNDAccount );
	pkAns.sEventID = eid;
	pkAns.sLevel = 0;		// eid �� ����, GS ���� ä���ִ´�. 

	pDSCom->Send( pkAns.usLength, (char*)&pkAns );

	PrintConsole( "  ==> Eid = %d\n", pkAns.sEventID );
}


void ProcLP_Check( CMSG_LP_CHECK_REQ * pReq, COLE_DB_Executer * pGameDB, _h_DS_Com * pDSCom )
{
	PrintConsole( "[LP] Recv Check Req: From %s : Event %d\n", pReq->cNDAccount, pReq->sEventID );

	PUSER pUser = g_pAccMapManager->Find_User(pReq->cNDAccount);    
	if( pUser )
	{
		CMSG_LP_CHECK_ANS pkAns;
		pkAns.sEventID = pReq->sEventID;
		pkAns.SetAccount( pReq->cNDAccount );
		if( TRUE == pGameDB->LP_Check( pUser->iUid, pReq->cNDAccount, pReq->sEventID, pReq->sLevel, pReq->cName ) )
		{
			pkAns.sLevel = pReq->sLevel;
		}
		else
		{
			pkAns.sLevel = -1;
		}

		PrintConsole( "  ==> Level %d\n", pkAns.sLevel );

		pDSCom->Send( pkAns.usLength, (char*)&pkAns );
	}
}


void ProcLP_Recv( CMSG_LP_RECV_REQ * pReq, COLE_DB_Executer * pGameDB, _h_DS_Com * pDSCom )
{
	PrintConsole( "[LP] Recv Recv Req: From %s : Event %d\n", pReq->cNDAccount, pReq->sEventID );

	CMSG_LP_RECV_ANS pkAns;
	pkAns.sEventID = pReq->sEventID;
	pkAns.sLevel = pReq->sLevel;
	pkAns.SetAccount( pReq->cNDAccount );

	if( TRUE == pGameDB->LP_Recv( pReq->cNDAccount, pReq->sEventID, pReq->sLevel, pReq->cName ) )
	{
		pkAns.ucResult = 1;
	}
	else
	{
		pkAns.ucResult = 0;
	}

	PrintConsole( "  ==> Eevnt %d, Result = %u\n", pkAns.sEventID, pkAns.ucResult );

	pDSCom->Send( pkAns.usLength, (char*)&pkAns );
}


void ProcLP_Cancel( CMSG_LP_CANCEL * pCancel, COLE_DB_Executer * pGameDB )
{
	PrintConsole( "[LP] Recv Cancel : From %s : Event %d\n", pCancel->cNDAccount, pCancel->sEventID );

	pGameDB->LP_Cancel( pCancel->cNDAccount, pCancel->sEventID );
}


#endif	// _PD_LEVEL_PRESENT_


#ifdef _PD_EVENT_INVITE_FRIEND_
void ProcIF_InfoReq( CMSG_FRIEND_INFO_REQ * pFriendInfoReq, C_USER_DB_Command * pUserDB, _h_DS_Com * pDSCom )
{
	// Request : Invite Friend event information.
	PrintConsole( "[IF] Recv Info. Req. : From<%s>\n", pFriendInfoReq->cNDAccount );

	// DB ȣ��.IF_GetInfo( char * acc, int &index, int &endTime, short &checkState )
	int index = 0;
	int endTime = 0;
	short state = 0;

	if( pUserDB->IF_GetInfo( pFriendInfoReq->cNDAccount, index, endTime, state ) )
	{
		CMSG_FRIEND_INFO pkIFInfo;
		pkIFInfo.sEventID = pFriendInfoReq->sEventID;
		pkIFInfo.SetAccount( pFriendInfoReq->cNDAccount );

		if( 0 == state )
		{
			if( endTime == 0 )
			{
				// ģ�� ��õ �ް�, ó�� �α����̴�. �ð��� ������Ʈ�ϰ�, GS �� ������ ������.
				time_t curTime;
				time( &curTime );

				pkIFInfo.ucResult = 1;
				pkIFInfo.iDB_Index = index;
				pkIFInfo.end_time = (int)curTime + _SECOND_DAY_;

				pUserDB->IF_SetStart( index, pkIFInfo.end_time );
			}
			else 
			{
				time_t curTime;
				time( &curTime );
				if( endTime > (int)curTime )
				{
					// �ð��� ���Ҵ�. GS �� ������ ������.
					pkIFInfo.ucResult = 1;
					pkIFInfo.iDB_Index = index;
					pkIFInfo.end_time = endTime;
				}
				else 
				{
					// �ð��� ������. state = 1 �� ������Ʈ.
					pUserDB->IF_EndUse( index );

					pkIFInfo.ucResult = 0;
					pkIFInfo.iDB_Index = 0;
					pkIFInfo.end_time = 0;
				}
			}
		}
		else
		{
			pkIFInfo.ucResult = 0;
			pkIFInfo.iDB_Index = 0;
			pkIFInfo.end_time = 0;
		}

		PrintConsole( "  Result %u : end time = %d\n", pkIFInfo.ucResult, pkIFInfo.end_time );

		pDSCom->Send( pkIFInfo.usLength, (char*)&pkIFInfo );
	}
	else
	{
		// ȣ�� ����. �׳� �Ѿ��.
		return;
	}
	
}
#endif	// _PD_EVENT_INVITE_FRIEND_
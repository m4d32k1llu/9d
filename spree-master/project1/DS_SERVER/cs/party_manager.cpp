#include "..\stdafx.h"
#include "..\Network\Network.h"
#include "..\Global\Global_ServerInfo.h"

#include "..\MapManager\MAPManager.h"
#include "..\CS\party_manager.h"
#include "..\Global\tcp_packets_party.h"
#pragma warning(disable:4509)
extern _j_MemeberQueue *	g_pMemberQueue ;
extern _j_PartyQueue *		g_pPartyQueue  ;
extern _h_Party_Timer *		g_pPartyTimer ;
extern _j_PartyManager *	g_pPartyManager ;
extern _j_ServerInfo *		g_pServerInfo ;
extern _j_AccMAPManager *	g_pAccMapManager ;



_j_MemeberQueue::_j_MemeberQueue() : m_pMember(NULL)
{ 		
}
_j_MemeberQueue::~_j_MemeberQueue() 
{ 	
	delete [] m_pMember ;
}


// _j_NodeQueue Class
bool _j_MemeberQueue::Create( const size_t uiSize ) 
{	
	m_pMember = new _Member[uiSize] ;
	if(m_pMember == NULL){
#ifdef _DEBUG
		printf("-> [error]VitualMemory Commit fail\n");
#endif
		return false;
	}

	member_lock() ;
	for(int i = 0; i < _PERMISSIBLE_MAX_USER; i++ ){
		m_NodeQueue.push(&m_pMember[i]);
	}
	member_unlock() ;
	
#ifdef _DEBUG
	printf("-> Map USER DATA Memory zone Reserved (Bytes:%d)\n", sizeof(_Member)*uiSize ) ;
#endif

	return true;
}

_Member * const _j_MemeberQueue::GetNode( const u_short uid, _CHARAC * const pCharac ) 
{
	if( NULL == pCharac || 0 >= uid )
		return NULL ;

	_Member * pMember = NULL ;
	map<u_short, _Member*>::iterator memberMapIter;

	member_lock() ;
	TRY{
		memberMapIter = m_MemberMap.find(uid) ;

		// �ʿ� ������ ť���� ���ο� ��带 �����ش�.
		if ( m_MemberMap.end() == memberMapIter ) {

			if( !m_NodeQueue.empty() ) {
				pMember =  m_NodeQueue.front() ;
				if(pMember) 
				{
					pMember->reset() ;
					pMember->pCharac = pCharac ;			// ĳ������ �����͸� �����Ѵ�.
					m_NodeQueue.pop() ;	
					m_MemberMap[uid] = pMember ;		// �ʿ� ���
				}				
			}
			else
			{
				::PrintConsole("[ERROR] m_NodeQueue.empty() \n") ;
			}
		}
		else
		{
			pMember = memberMapIter->second ;	
		}		

	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] _j_MemeberQueue::GetNode \n") ;
		pMember = NULL ;
	}
	member_unlock() ;

	return pMember ;
}

void _j_MemeberQueue::PutNode( _Member * const Node )
{
	map<u_short, _Member*>::iterator memberMapIter;
	member_lock() ;
	TRY{
		//if ( Node->cCheck == 0  ) // ���̻� ������� �ʴ� ������� Ȯ��
		{			
			memberMapIter = m_MemberMap.find(Node->uid) ;
			if( memberMapIter != m_MemberMap.end() ) 
			{
				m_MemberMap.erase(memberMapIter) ;	// �ʿ��� ����
			}
			Node->reset() ;
			m_NodeQueue.push( const_cast<_Member*>(Node) ) ;
		}	
	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_MemeberQueue::PutNode \n") ;
	}	
	member_unlock() ;
}

_Member * const	_j_MemeberQueue::FindNode( const u_short uid )
{
	_Member * pMember = NULL ;
	map<u_short, _Member*>::iterator memberMapIter;
	member_lock() ;
	TRY{
		memberMapIter = m_MemberMap.find(uid) ;
		if ( m_MemberMap.end() == memberMapIter ) {
			pMember = NULL ;
		}
		else
		{
			pMember = memberMapIter->second ;
		}
	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_MemeberQueue::FindNode \n") ;
		pMember = NULL ;
	}
	member_unlock() ;

	return pMember ;
}

void _j_MemeberQueue::InsertMap( const u_short uid, const _Member * const Node) 
{
	member_lock() ;
	TRY{
		m_MemberMap[uid] = const_cast<_Member*>(Node) ;
	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_MemeberQueue::InsertMap \n") ;
	}
	member_unlock() ;

}

void _j_MemeberQueue::DeleteMap( const u_short uid) 
{
	map<u_short, _Member*>::iterator memberMapIter;
	member_lock() ;
	TRY{
		memberMapIter = m_MemberMap.find(uid) ;
		if( m_MemberMap.end() != memberMapIter ){
			m_MemberMap.erase(memberMapIter) ;
		}
	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_MemeberQueue::DeleteMap \n") ;
	}
	member_unlock() ;

}


/*===========================================================================
_j_PartyQueue : ��Ƽ���� ť�� �̸� �Ҵ��Ͽ� ����Ѵ�.
===========================================================================*/


// Queue�� �ִ� Party�����͸� ��ȯ�޴´�.
_Party * const _j_PartyQueue::GetNode() 
{
	_Party * pParty = NULL ;
	party_queue_lock() ;
	TRY{
		if( m_PartyQueue.size() > 0 )
		{
			pParty = m_PartyQueue.front() ;
			m_PartyQueue.pop() ;
			if( pParty ) pParty->Init() ;		
		}
	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] _Party * const _j_PartyQueue::GetNode() \n") ;
	}	
	party_queue_unlock() ;
	return pParty ;

}


// �پ� Party �����͸� Queue�� ��ȯ�Ѵ�.
void _j_PartyQueue::PutNode( const _Party * const pParty)
{

	if ( pParty == NULL ) return ;

	party_queue_lock() ;
	TRY{
		m_PartyQueue.push( const_cast<_Party*>(pParty) ) ;		
	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] void _j_PartyQueue::PutNode( const _Party * const pParty) \n") ;
	}	
	party_queue_unlock() ;
}



//============================================================================

/*===========================================================================
_j_MAP : ��Ƽ ����Ʈ ���� �����Ѵ�..
===========================================================================*/




// ��Ƽ ����Ʈ ����
/*
bool  _j_PartyManager::DeleteList(short sListIndex) 
{
	bool b = false ;
	map<short, _Party*>::iterator tempItr;	

	party_map_lock() ;
	TRY{
		tempItr = m_Map.find(sListIndex) ;

		if ( tempItr != m_Map.end() ) 
		{
			// Map���� List ����	
			m_Map.erase(tempItr) ;
			b = true ;
		}

	}
	CATCH_ALL
	{
		::PrintConsole("[EXCEPTION] bool  _j_PartyManager::DeleteList(short sListIndex) \n") ;
	}	
	party_map_unlock() ;	
	return b ;
}
*/

_Party *  _j_PartyManager::FindList(short sListIndex) 
{
	_Party * pTemp = NULL ;	

	party_map_lock() ;
	TRY{
		map<short, _Party*>::iterator tempItr = m_Map.find(sListIndex) ;		
		if ( tempItr != m_Map.end() ) {	
			pTemp = tempItr->second ;
		}
	}
	CATCH_ALL{
		PrintConsole("[EXCEPTION] _Party *  _j_PartyManager::FindList(short sListIndex) \n") ;
	}	
	party_map_unlock() ;
	return pTemp ;
}




/*===========================================================================
_j_PartyManager
===========================================================================*/
// PartyMap Manager

short _j_PartyManager::CreateParty(char cServerNum, u_short uid1, u_short uid2, char * name1, char * name2, OUT short & sProp )
{

	if(cServerNum <= 0 || uid1 <= 0 || uid2 <= 0)
		return 0 ;	

	_C_CHARAC * pCharac1 = g_pAccMapManager->Find_Charac( name1 ) ;
	_C_CHARAC * pCharac2 = g_pAccMapManager->Find_Charac( name2 ) ;

	if(NULL == pCharac1 || NULL == pCharac2 /*|| pCharac1->CHARAC_STATE.sPartyIndex > 0 || pCharac2->CHARAC_STATE.sPartyIndex > 2 */)
		return 0 ;

	short sProp1 = 0, sProp2 = 0, sPartyProp = 0;
	sProp1 = g_sMoonpaProperty[pCharac1->CHARAC_BASIC.cParty] ;
	sProp2 = g_sMoonpaProperty[pCharac2->CHARAC_BASIC.cParty] ;


	if( ::IsFriendly( sProp1, sProp2 ) == false )
		return 0 ;


	_Member *	pMember1 = NULL ;
	_Member *	pMember2 = NULL ;
	_Party *	pParty = NULL ;

	TRY{			

		pMember1 = g_pMemberQueue->GetNode(uid1, pCharac1) ;
		pMember2 = g_pMemberQueue->GetNode(uid2, pCharac2) ;

		pParty = g_pPartyQueue->GetNode() ;
		
		//if( (!pMember1 || pMember1->attribute.bParty == true) ||
		//	(!pMember2 || pMember2->attribute.bParty == true )|| 
			//!pParty ) // ������̶� NULL �̸� ����ó���ؾ��Ѵ�.
		if( pMember1 == NULL || pMember2 == NULL || pParty == NULL ) // ������̶� NULL �̸� ����ó���ؾ��Ѵ�.
		{
			
			
			// ��ȯ�Ұ��� �ִٸ�.. ��ȯ�Ѵ�..
			if( pMember1 ) 
			{				
				g_pMemberQueue->PutNode( pMember1 ) ;				
			}
			else
			{
				::PrintConsole("[ERROR] pMember1 == NULL \n") ;
			}
			if( pMember2 ) {				
				g_pMemberQueue->PutNode( pMember2 ) ;
			}
			else
			{
				::PrintConsole("[ERROR] pMember2 == NULL \n") ;
			}
			if( pParty ) {
				g_pPartyQueue->PutNode( pParty ) ;				
			}
			else
			{
				::PrintConsole("[ERROR] pParty == NULL \n") ;
			}

			return 0 ;
		}

		pParty->Init() ;


		// ��Ƽ��..			
		pMember1->attribute.bParty = true ;
		pMember1->attribute.bCaptian = true ;
		pMember1->uid = uid1 ;
		pMember1->pCharac = pCharac1 ;

		// ��Ƽ��
		pMember2->attribute.bParty = true ;
		pMember2->attribute.bCaptian = false ;
		pMember2->uid = uid2 ;
		pMember2->pCharac = pCharac2 ;

		pParty->SetPartyIndex( GetPartyIndex() ) ;


		::WhatIsMoonpaProp( sProp1, sProp2, OUT sPartyProp ) ;
		pParty->SetMoonpaProp( sPartyProp ) ;
		sProp = pParty->GetMoonpaProp() ;


		// ��Ƽ�� �Ӽ�(����) ����..
		pParty->CreateParty( pMember1, pMember2) ;

		// ���� ����(?) ����
		pParty->SetServer( cServerNum ) ;		

	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_PartyManager::CreateParty \n") ;
		return 0 ;
	}

	party_map_lock() ;	
	TRY{
		m_Map[pParty->GetPartyIndex()] = pParty ;	// ������ ��Ƽ ����Ʈ�� �ʿ� ����	
	}
	CATCH_ALL{
		::PrintConsole("[EXCEOTION] m_Map[pParty->GetPartyIndex()] = pParty  \n") ;
	}
	party_map_unlock() ;

#ifdef _TRACE_
	g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Create party success \n") ;
#endif

	return pParty->GetPartyIndex() ;	
}

bool _j_PartyManager::DeleteParty(_Party * pParty)
{	

	if( pParty == NULL ) {
		return false ;	// �ش� ��Ƽ �ε����� ã�� ���Ѱ���.
	}

	/*
	if ( pParty->GetMemberCount() == 0 ) {
		DeleteList(pParty->GetPartyIndex()) ;
		g_pPartyQueue->PutNode( pParty ) ;
		return false ;  // ��Ƽ ����Ʈ�� ����ִ�..
	}
	*/

	pParty->DeleteParty() ;
	// Map���� List ����	
	map<short, _Party*>::iterator tempItr;	
	bool b = false ;
	party_map_lock() ;
	TRY{		
		tempItr = m_Map.find(pParty->GetPartyIndex()) ;
		if ( tempItr != m_Map.end() ) {			
			m_Map.erase(tempItr) ;
			g_pPartyQueue->PutNode( pParty ) ;
			b = true ;
		}
		else
		{
			b = false ;
		}

	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_PartyManager::DeleteParty\n") ;
		b = false ;
	}
	party_map_unlock() ;
	

	return b ;	
}


int _j_PartyManager::JoinParty(_Party * party, u_short uid, char * name, OUT short & sProp )
{	
	int slot ;
	TRY{

		if( uid <= 0 || !party ) {
			::PrintConsole("[ERROR] 'cServerNum <= 0 || uid <= 0 || !party'\n") ;
			return -1 ;
		}
		if( party->GetMemberCount() >= MAX_PARTY_MEMBER ){
			::PrintConsole("[ERROR] party->cMemberCount >= MAX_PARTY_MEMBER \n") ;
			return -1 ;
		}

		_CHARAC * pCharac = g_pAccMapManager->Find_Charac( name ) ;
		if( NULL == pCharac )
		{
			::PrintConsole("[ERROR] _j_PartyManager::JoinParty \n") ;		
			return -1 ;
		}

		_Member * const pMember = g_pMemberQueue->GetNode(uid, pCharac ) ;
		if ( !pMember || pMember->attribute.bParty == true  ) {			
			::PrintConsole("[ERROR] if ( !pMember || pMember->attribute.bParty == true  )  \n") ;		
			return -1 ;
		}

		pMember->pCharac = pCharac ;
		pMember->uid = uid ;	
		pMember->attribute.bCaptian = false ;
		pMember->attribute.bParty = true ;

#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "->[JOIN] Joined (%s) \n", name) ;
#endif


		short sTempProp = g_sMoonpaProperty[pCharac->CHARAC_BASIC.cParty] ;		
		if( party->GetMoonpaProp() == 0 )
		{
			party->SetMoonpaProp( sTempProp ) ;
		}
		else if( ::IsFriendly( sTempProp, party->GetMoonpaProp() ) == false )
		{
			pMember->attribute.bParty = false ;
			g_pMemberQueue->PutNode( pMember ) ;
			//::PrintConsole("[ERROR] if( ::IsFriendly( %d, %d ) == false ) \n", sTempProp, party->GetMoonpaProp()) ;
			return -1 ;
		}		
		sProp = party->GetMoonpaProp() ;


		slot = party->SetMember( pMember ) ;	
	}
	CATCH_ALL
	{
		::PrintConsole("[EXCEPTION] int _j_PartyManager::JoinParty(_Party * party, char cServerNum, u_short uid, char * name) \n") ;
		slot = -1 ;
	}	
	return slot ;
}

int _j_PartyManager::KickParty(_Party * pParty, char slotNo)
{	
	int iRes ;
	int pos = 0 ;


	TRY{
		if( !pParty || slotNo<0 )
			return -1 ;

		pos = 2 ;
		// ��Ƽ �ο��� 2�� ������ ���� ��Ƽ�� �����Ѵ�...
		if ( pParty->GetMemberCount() < 3 ) {	

			//pParty->DeleteParty() ;
#ifdef _TRACE_
			g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] ��Ƽ�� �̴޷� �����Ǿ��� \n") ;
#endif
			//DeleteList(pParty->GetPartyIndex()) ;
			//g_pPartyQueue->PutNode( pParty ) ;
			DeleteParty( pParty ) ;
			return 1 ;
		}

		iRes = 0 ;

		pos = 3 ;	

		iRes = pParty->KickMember( slotNo ) ;
	}
	CATCH_ALL {		
		::PrintConsole( "[EXCEPTION] _j_PartyManager::KickParty (pos.%d)\n", pos) ;		
	}


	return iRes ;

	//return -3 ;	// ��Ƽ���� ã�� ���� ����.	
}


char _j_PartyManager::EntrustCaptain(_Party * pParty, char slotNo, u_short uid )
{	

	char  slot ;

	TRY {

		bool bCheck = (pParty == NULL) 
			|| ( pParty->GetMemberCount() == 0 ) 
			|| (slotNo >= MAX_PARTY_MEMBER)
			|| (slotNo < 0 ) ;

		if( bCheck ) {
			return -1 ;
		}

		if( uid == 0 )
		{
			slot = pParty->Entrust() ;
		}
		else
		{
			slot = pParty->Entrust( slotNo ) ;
		}		

	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_PartyManager::EntrustCaptain \n") ;
		return -1 ;	
	}

	return slot ;

}

bool _j_PartyManager::MoveParty(_Party * pParty, char cServerNum, u_short uid, char slotNo) 
{

	TRY{
		_Member *			pMember = NULL ;


		if(pParty == NULL) {
			return false ;
		}		

		pMember = g_pMemberQueue->FindNode(uid) ;
		if ( !pMember || pMember->attribute.bParty == false ) {
			return false ;
		}	

#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "-> [MOVE] Move Request (%s) \n", pMember->pCharac->CHARAC_BASIC.cChrName ) ;
#endif
		
		pMember->attribute.bIsMoveState = true ;


		_sParty_timerentry timerEnt ;

		timerEnt.pMember = pMember ;
		timerEnt.time = _sParty_timerentry::get_time_by_sec() + _PARTY_MOVE_OUT_TIME_SEC  ;
		timerEnt.work = _ns_party_::_TM_PARTY_MOVE_READY_ ;		

		timerEnt.index = pParty->GetPartyIndex() ;
		timerEnt.uid = ++pMember->usMoveCheckIndex ;	// �� �ε��� ���� ���ؼ� ������ �����̵��� ���� ��� ���� �����̵��� ���ؼ��� �˻��ϰ� �ȴ�.
		timerEnt.slot = slotNo ;	


		g_pPartyTimer->send_Message( timerEnt ) ;
#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Add timer (uid : %d) \n", uid) ;
#endif	


	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_PartyManager::MoveParty \n") ;
	}		

	return true ;
}


void _j_PartyManager::MoveComplete( u_short uid, char slotNo ) 
{
	TRY{		
		_Member *			pMember = NULL ;	


		pMember = g_pMemberQueue->FindNode(uid) ;


		if ( pMember == NULL || pMember->attribute.bParty == false ) {
#ifdef _TRACE_
			g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "->[MOVE] pMember == NULL || pMember->attribute.bParty == false  (%d) \n", uid ) ;
#endif
			return ;
		}		

#ifdef _TRACE_
		g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "->[MOVE] Move complete (%s) \n", pMember->pCharac->CHARAC_BASIC.cChrName) ;
#endif

		pMember->attribute.bIsMoveState = false ;	


	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_PartyManager::MoveComplete \n") ;
	}	
}


//void	_j_PartyManager::PutServer(list<_Server> * serverList, char cServerNum) 
//{
//	list<_Server>::iterator serverListItr ;
//	_Server * pServerInfo ;
//	
//	for( serverListItr = serverList->begin() ; serverListItr != serverList->end() ; serverListItr++)
//	{
//		pServerInfo = &(*serverListItr) ;
//		if( pServerInfo->serverNum == cServerNum ){
//			++pServerInfo->count ;
//			break ;
//		}		
//	}
//}
//void	_j_PartyManager::PopServer(list<_Server> * serverList, char cServerNum)
//{
//}
//void	_j_PartyManager::MoveServer(list<_Server> * serverList, char cFromServerNum, char cToServerNum)
//{



/*#################################################################################

CS Timer Thread

#################################################################################*/
DWORD WINAPI th_Party( LPVOID lp ) 
{
	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;

	LARGE_INTEGER li = {0, } ;
	DWORD curTime ;

	_sParty_timerentry entry ;
	int iRes ;

	char		g_cServerNumList[MAX_SERVER_COUNT] ;
	_h_DS_Com * pDSComm ;
	_Party *	pParty ;

	SMSG_KICK_PARTY_RES_CS_GS msgKickPartyRes = {0} ;
	msgKickPartyRes.usLength = sizeof(msgKickPartyRes) ;
	msgKickPartyRes.cMessage = SMSG_NO_KICK_PARTY_RES_CS_GS ;



	printf( "  => Start Party Timer\n" ) ;

	SetWaitableTimer( hTimer, &li, 1000, NULL, NULL, TRUE ) ;

	while(1) {
		WaitForSingleObject( hTimer, INFINITE ) ;

		curTime = _sParty_timerentry::get_time_by_sec() ;
		while ( g_pPartyTimer->get_TimerMessage( curTime, entry ) == true ) {
			// ��ó��. ó���� ���� ť�� Lock�� ���� �ʾƵ� �ȴ�. ^^

			TRY	{
				switch( entry.work ) 
				{
				case _ns_party_::_TM_PARTY_MOVE_READY_ :
					// entry.uid == entry.pMember->usMoveCheckIndex 
					// �� �ε��� ���� ���ؼ� ������ �����̵��� ���� ��� ���� �����̵��� ���ؼ��� �˻��ϰ� �ȴ�.
					if( entry.pMember->attribute.bIsMoveState == true && entry.uid == entry.pMember->usMoveCheckIndex )
					{								

#ifdef _TRACE_
						g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] Check Timer & Kick (uid : %d) \n", entry.uid ) ;
#endif
						pParty = g_pPartyManager->FindList( entry.index ) ;
						if( !pParty )
							continue ;	

						iRes = g_pPartyManager->KickParty(pParty, entry.slot) ;
						if ( iRes < 0 ) 
						{
#ifdef _TRACE_
							g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] KickParty fail (%d) \n", iRes) ;
#endif
						}
						else if( iRes == 1 )	// ��Ƽ�� �̴޷� ��Ƽ ����..
						{
#ifdef _TRACE_
							g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] ��Ƽ�� �̴޷� ��ü (partyIndex : %d)\n", pParty->GetPartyIndex() ) ;
#endif							
						}

						else if( iRes == 2)	// �ڵ� ����..
						{
							msgKickPartyRes.cCapNo = g_pPartyManager->EntrustCaptain(pParty	, 0, 0) ;
#ifdef _TRACE_
							g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "[PARTY] ��Ƽ�� �ڵ� ���� (partyIndex : %d, ��Ƽ�� : %u )\n", 
								pParty->GetPartyIndex(),  pParty->GetCaptain() ) ;
#endif													
						}


						msgKickPartyRes.sPartyIndex = entry.index ;
						msgKickPartyRes.uid = entry.uid ;
						msgKickPartyRes.cResult = (char)iRes ;
						msgKickPartyRes.cMode = 2 ;					// ����Ż��� ó��
						msgKickPartyRes.cSlotNo = entry.slot ;

						memcpy( g_cServerNumList, pParty->GetServerList(), sizeof(g_cServerNumList) ) ;	
						if ( iRes >= 0 ) {	// Ż�� �����߰ų�, ��Ƽ�� ���������
							for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ ) {
								if ( g_cServerNumList[i] == '&' ) {
									break ;
								}
								pDSComm = g_pServerInfo->GetDSCom(g_cServerNumList[i]) ;
								if(pDSComm){								
									if( pDSComm->Send(msgKickPartyRes.usLength, (char*)&msgKickPartyRes) == false ){
										::PrintConsole( "[SO_ERR] pDSComm->send_request \n" ) ;
									}						
								}
							}
						}

					}

					break; 
				}
			}
			CATCH_ALL{
				::PrintConsole("[EXCEPTION] DWORD WINAPI th_Party( LPVOID lp ) \n") ;
			}			
		} // while( g_pPartyTimer->get_TimerMessage( curTime, entry ) == true )
	}

	return 0 ;
}

//#################################################################################
//
//	_h_Party_Timer
//
//###################################################################### LHS Added....
bool _h_Party_Timer::create()
{
	HANDLE hTh ;
	DWORD dwID ;

	hTh = BEGINTHREADEX( NULL, 0, th_Party, NULL, 0, &dwID ) ;
	if( hTh == INVALID_HANDLE_VALUE ) {
		return false ;
	}
#ifdef _TRACE_
	printf("[PARTY] Begin Timer thread \n") ;
#endif
	CloseHandle( hTh ) ;

	return true ;
}

bool _h_Party_Timer::get_TimerMessage( DWORD time, _sParty_timerentry& Entry )
{
	bool b = false ;
	party_timer_lock() ;
	TRY{
		if( m_TimerQueue.empty() ) {			
			b = false  ;
		}
		else
		{
			map<_Member *, _sParty_timerentry>::iterator itr ;
			for( itr = m_TimerQueue.begin() ; itr != m_TimerQueue.end() ; itr++ )
			{
				if( itr->second.time <= time )
				{
					Entry = itr->second ;
					m_TimerQueue.erase( itr ) ;
					b = true ;
					break ;
				}
			}			
		}		
	}
	CATCH_ALL
	{
		::PrintConsole("[EXCEPTION] bool _h_Party_Timer::get_TimerMessage( DWORD time, _sParty_timerentry& Entry ) \n") ;
		b = false ;
	}
	party_timer_unlock() ;
	
	return b ;
}

void _h_Party_Timer::send_Message( _sParty_timerentry& en )
{
	party_timer_lock() ;
	TRY {
		//m_TimerQueue.push( en ) ;
		m_TimerQueue[en.pMember]= en ;	

	}
	CATCH_ALL{
		::PrintConsole( "[EXCEPTION] Party Timer - send_message\n" ) ;
	}
	party_timer_unlock() ;
}


//#################################################################################
//
//	_Party
//
//#################################################################################



void _Party::CreateParty( _Member * const p1, _Member * const p2 )
{
	TRY{
		m_pMember[0] = p1 ;
		m_pMember[1] = p2 ;
		m_cMemberCount = 2 ;
		m_cExpProperty = 0 ;
		m_usCaptainUID = p1->uid ;		
	}
	CATCH_ALL
	{
		::PrintConsole("[EXCEPTION] void _Party::CreateParty( _Member * const p1, _Member * const p2 ) \n") ;
	}
	
}

void _Party::DeleteParty()
{
	// ��Ƽ ������� ����ü�� ť�� ��ȯ�Ѵ�.
	party_lock() ;
	TRY{
		for ( int i = 0 ; i < MAX_PARTY_MEMBER ; i++ ) {
			if( m_pMember[i] )
			{
				m_pMember[i]->attribute.bParty = false ;
				m_pMember[i]->attribute.bCaptian = false ;			

				g_pMemberQueue->PutNode(m_pMember[i] ) ;	// Node ��ȯ

			}
		}	
	}
	CATCH_ALL	{
		::PrintConsole("[EXCEPTION] void _Party::DeleteParty() \n") ;
	}	
	party_unlock() ;
}

void _Party::SetServer(const char cServerNum )
{
	party_lock() ;
	TRY{
		// ��������Ʈ�� �˻��ؼ� ������ ��ϵǾ� ���� �ʴٸ� ���� �߰�..
		for( int i = 0 ; i < MAX_SERVER_COUNT ; i++ )
		{
			if( m_cServerNumList[i] == cServerNum )
			{
				break ;
			}

			if( m_cServerNumList[i] == 0 || m_cServerNumList[i] == '&' )
			{
				m_cServerNumList[i] = cServerNum ;
				m_cServerNumList[i+1] = '&' ;
				break ;
			}
		}
	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] void _Party::SetServer(const char cServerNum ) \n") ;
	}
	party_unlock() ;
}

const int _Party::SetMember( _Member * const pMember )
{
	int slot ;

	party_lock() ;	
	TRY{		
		for(slot = 0 ; slot < MAX_PARTY_MEMBER; slot++)
		{		
			if(m_pMember[slot] == NULL)
			{
				m_pMember[slot] = pMember ;
				m_cMemberCount++ ;				
				break ;
			}
		}		
	}
	CATCH_ALL {		
		::PrintConsole("[EXCEPTION] _j_PartyManager::JoinParty \n") ;
		slot = -1 ;
	}	
	party_unlock() ;

	return slot ;
}

const int _Party::KickMember(const char slot)
{
	int iRes  = 0 ;

	party_lock() ;
	TRY{
		if( m_pMember[slot] )
		{			

			if ( m_pMember[slot]->attribute.bCaptian == true ) {	// ��Ƽ���� ������쿡�� ������ �ڵ� ����					
				iRes = 2 ;	// ��Ƽ�� ���� �޼��� ��ȣ
			}

			//int svrNum = pParty->pMember[slotNo]->cServerNum ;

			m_pMember[slot]->attribute.bParty = false ;
			m_pMember[slot]->attribute.bCaptian = false ;


			g_pMemberQueue->PutNode(m_pMember[slot]) ;


			m_pMember[slot] = NULL ;		// ������ ����..	
			m_cMemberCount-- ;		// ī��带 �ٿ�ī��Ʈ �Ѵ�..				


#ifdef _TRACE_
			g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "->[KICK] Kicked (Slot : %d ) \n", slot) ;
#endif

		}
	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] const int _Party::KickMember(const char slot) \n") ;
	}	
	party_unlock() ;

	return iRes ;

}

const int _Party::Entrust( const char slot )
{

	party_lock() ;
	TRY	{
		// �켱 �ʱ�ȭ..
		for( int i = 0 ; i < MAX_PARTY_MEMBER; i++ )
		{
			if( m_pMember[i] )
			{
				m_pMember[i]->attribute.bCaptian = false ;
			}
		}

		//  ���� ����		
		if( m_pMember[slot] ) {
			m_pMember[slot]->attribute.bCaptian = true ;
			m_usCaptainUID = m_pMember[slot]->uid ;	// ��Ƽ �Ӽ��� ��Ƽ�� ���̵� ����..

#ifdef _TRACE_
			g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "->[ENTR] Directical Entrusted (%s) \n", m_pMember[slot]->pCharac->CHARAC_BASIC.cChrName ) ;
#endif

		}
	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] const int _Party::Entrust( const char slot ) \n") ;
	}	
	party_unlock() ;

	return slot;

}

const int _Party::Entrust()
{

	int i ;

	party_lock() ;
	TRY	{
		for( i =0; i < MAX_PARTY_MEMBER; i++)
		{
			if( m_pMember[i] )
			{
				m_pMember[i]->attribute.bCaptian = true ;
				break ;
			}			
		}	

		m_usCaptainUID = m_pMember[i]->uid ;		// ��Ƽ �Ӽ��� ��Ƽ�� ���̵� ����..		

#ifdef _TRACE_	
		g_pTrace->OutputString( _TRACE_CLASS_MESSAGE, "->[ENTR] Automatical Entrusted (%s) \n", m_pMember[i]->pCharac->CHARAC_BASIC.cChrName ) ;
#endif				
	}
	CATCH_ALL{
		::PrintConsole("[EXCEPTION] const int _Party::Entrust() \n") ;
	}
	party_unlock() ;

	return i ;

}

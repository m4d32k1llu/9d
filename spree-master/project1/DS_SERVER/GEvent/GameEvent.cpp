
#include "../stdafx.h"

#include "GameEvent.h"
#include "GameEventParser.h"

extern _j_ServerInfo		g_ServerInfo;			// ��������

#ifdef _PD_GAME_EVENT_

char * g_match_work_string[] = {
	"ready", 
		"start",
		"end",
		"notice",
		"notice_winner", 
		"notice_nowinner", 
		"to_winner", 
		"send_result",
		"reward_nick", 
		"reward_item",
		"buff_all",
		"clean",
};

void CFreeMatchModule::AddWork( int where, __fm_event_work * pWork )
{
	__fm_event_work * pFirst = NULL;
	pWork->pNext = NULL;

	switch( where )
	{
	case en_fm_step_ready:
		pFirst = &m_readyWork;
		break;
	case en_fm_step_start:
		pFirst = &m_startWork;
		break;
	case en_fm_step_end:
		pFirst = &m_endWork;
		break;
	}

	if( pFirst )
	{
		__fm_event_work * pAddPos = pFirst;
		while( pAddPos->pNext )
		{
			pAddPos = pAddPos->pNext;
		}

		pAddPos->pNext = new __fm_event_work;
		CopyMemory( pAddPos->pNext, pWork, sizeof(__fm_event_work) );

		pAddPos->pNext->usDiffPrevSecond = pAddPos->pNext->usOrgSecond - pAddPos->usOrgSecond;
	}
}


u_short CFreeMatchModule::GetReadyTime()
{
	if( NULL == m_readyWork.pNext )
		return 0;

	__fm_event_work * pLastReady = m_readyWork.pNext;

	while( pLastReady->pNext )
	{
		pLastReady = pLastReady->pNext;
	}

	return (pLastReady->usOrgSecond + 60);		// ������ Work �� �ð��� �� �غ� �ð��̴�. ��
}


void CFreeMatchModule::ReserveNextWork()
{
	if( m_pCurWork )
	{
		m_pCurWork = m_pCurWork->pNext;

		if( m_pCurWork )
		{
			m_FMArg.match_type = GEVENT_TYPE_FREE_MATCH;
			m_FMArg.work = m_pCurWork->work;

			PrintAll( "[FM] Reserve : %d( after %u )\n", m_pCurWork->work, m_pCurWork->usDiffPrevSecond );
			m_refManager.AddTimer( GEVENT_TYPE_FREE_MATCH, CND_TimeUnitFunc::Get_CurrentSecond()+m_pCurWork->usDiffPrevSecond, 
				&m_FMArg, sizeof(m_FMArg) );
		}
		/*
		else
		{
			if( en_fm_step_end == GetFMState() )		// end �� �������̹Ƿ�, 3�� �Ŀ� ������ �����Ѵ�.
			{
				// clean work �� �����Ѵ�.
				m_FMArg.match_type = GEVENT_TYPE_FREE_MATCH;
				m_FMArg.work = en_fm_work_clean;

				PrintAll( "[FM] Reserve : Clean up( after 3m\n" );

				m_refManager.AddTimer( GEVENT_TYPE_FREE_MATCH, CND_TimeUnitFunc::Get_CurrentSecond() + 180, 
					&m_FMArg, sizeof(m_FMArg) );
			}
		}
		*/
	}
}


void CFreeMatchModule::EventReceive( _sFreeMatch_Arg * pArg )
{
	switch( pArg->work )
	{
	case en_fm_work_ready:				// ready �̺�Ʈ ó��.
		PrintAll( "[FM] Event : work - Ready\n" );
		m_pCurWork = &m_readyWork;
		ProcWork_Ready();
		break;
	case en_fm_work_start:				// start �̺�Ʈ ó��.
		PrintAll( "[FM] Event : work - Start\n" );
		m_pCurWork = &m_startWork;
		ProcWork_Start();
		break;
	case en_fm_work_end:				// end �̺�Ʈ ó��
		PrintAll( "[FM] Event : work - End\n" );
		m_pCurWork = &m_endWork;
		ProcWork_End();
		break;
	case en_fm_work_notice:				// ���� ����
		if( m_pCurWork )
			PrintAll( "[FM] Event : work - Notice(%d-%d)\n", m_pCurWork->notice.textID, m_pCurWork->notice.arg );
		else
			PrintAll( "[FM] Event : work - Notice\n" );
		ProcWork_Notice();
		break;

	case en_fm_work_notice_winner:		// ���� : ����� ������ ����
		if( m_pCurWork )
			PrintAll( "[FM] Event : work - Notice winner(%d-%d)\n", m_pCurWork->notice.textID, m_pCurWork->notice.arg );
		else
			PrintAll( "[FM] Event : work - Notice winner\n" );
		ProcWork_NoticeWinner();
		break;
	case en_fm_work_notice_nowinner:	// ���� : ���º� ����
		if( m_pCurWork )
			PrintAll( "[FM] Event : work - Notice nowinner(%d-%d)\n", m_pCurWork->notice.textID, m_pCurWork->notice.arg );
		else
			PrintAll( "[FM] Event : work - Notice nowinner\n" );
		ProcWork_NoticeNowinner();
		break;

	case en_fm_work_to_winner:			// ���ڿ��Ը�.
		if( m_pCurWork )
			PrintAll( "[FM] Event : work - To winner(%d-%d)\n", m_pCurWork->notice.textID, m_pCurWork->notice.arg );
		else
			PrintAll( "[FM] Event : work - To winner\n" );
		ProcWork_ToWinner();
		break;
	case en_fm_work_send_result:		// ����� �����ش�.
		PrintAll( "[FM] Event : work - Send result\n" );
		ProcWork_SendResult();
		break;
	case en_fm_work_reward_nick:			// ��ȣ ���� ����,
		PrintAll( "[FM] Event : work - Reward nick\n" );
		ProcWork_RewardNick();
		break;
	case en_fm_work_reward_item:
		PrintAll( "[FM] Event : work - Reward item\n" );
		ProcWork_RewardItem();
		break;
	case en_fm_work_buff_all:			// �� ������ ��� ����.
		PrintAll( "[FM] Event : work - Buff all\n" );
		ProcWork_BuffAll();
		break;
	case en_fm_work_clean:				// ���� ����.
		PrintAll( "[FM] Event : work - Clean\n" );
		ProcWork_Clean();
		break;
	}

	ReserveNextWork();
}


void CFreeMatchModule::ProcWork_Ready()
{
	if( m_pCurWork )
	{
		SetFMState( en_fm_step_ready );

		m_lEventTime = CND_TimeUnitFunc::Get_CurrentSecond();

		// GS �� ���� : �غ�
		CMSG_FM_READY pkFMReady;
		CopyMemory( &pkFMReady.event_info, &m_curMatchEvent, sizeof(_sFreeMatchEvent) );
		pkFMReady.event_t_time = m_lEventTime;

		g_ServerInfo.send_to_all_gs( &pkFMReady, pkFMReady.usLength );
	}
}

void CFreeMatchModule::ProcWork_Start()
{
	if( m_pCurWork )
	{
		SetFMState( en_fm_step_start );

		m_lEventTime = CND_TimeUnitFunc::Get_CurrentSecond();

		// GS �� ���� : ����
		CMSG_FM_START pkFMStart;
		pkFMStart.event_t_time = m_lEventTime;

		g_ServerInfo.send_to_all_gs( &pkFMStart, pkFMStart.usLength );
	}
}


void CFreeMatchModule::ProcWork_End()
{
	if( m_pCurWork )
	{
		SetFMState( en_fm_step_end );

		m_lEventTime = CND_TimeUnitFunc::Get_CurrentSecond();

		// GS �� ���� : ���� �˸���.
		CMSG_FM_END pkFMEnd;
		pkFMEnd.event_t_time = m_lEventTime;
		g_ServerInfo.send_to_all_gs( &pkFMEnd, pkFMEnd.usLength );


		CMSG_FM_REQ_RESULT pkReq;
		g_ServerInfo.send_to_gs( &pkReq, pkReq.usLength, FREEMATCH_SERVER_INDEX );
	}
}


void CFreeMatchModule::ProcWork_Notice()
{
	if( m_pCurWork )
	{
		CMSG_FM_NOTICE pkFMNotice;
		pkFMNotice.text_id = m_pCurWork->notice.textID;
		pkFMNotice.text_arg1 = m_pCurWork->notice.arg;

		g_ServerInfo.send_to_all_gs( &pkFMNotice, pkFMNotice.usLength );
	}
}


void CFreeMatchModule::ProcWork_NoticeWinner()
{
	if( m_pCurWork )
	{
		if( en_fm_result_one_winer == m_refManager.GetResultInfo() )
		{
			CMSG_FM_NOTICE_WINNER pkFMNoticeWinner;

			pkFMNoticeWinner.text_id = m_pCurWork->notice.textID;
			strncpy( pkFMNoticeWinner.cWinnerName, m_refManager.GetWinnerName(), en_charac_name_length+1 );
			pkFMNoticeWinner.cWinnerName[en_charac_name_length] = 0;

			g_ServerInfo.send_to_all_gs( &pkFMNoticeWinner, pkFMNoticeWinner.usLength );
		}
	}
}


void CFreeMatchModule::ProcWork_NoticeNowinner()
{
	if( m_pCurWork )
	{
		if( en_fm_result_one_winer != m_refManager.GetResultInfo() )
		{
			CMSG_FM_NOTICE_NOWINNER pkFMNoticeNowinner;
			pkFMNoticeNowinner.text_id = m_pCurWork->notice.textID;

			g_ServerInfo.send_to_all_gs( &pkFMNoticeNowinner, pkFMNoticeNowinner.usLength );
		}
	}
}


void CFreeMatchModule::ProcWork_ToWinner()
{
	if( m_pCurWork )
	{
		if( en_fm_result_one_winer == m_refManager.GetResultInfo() )
		{
			CMSG_FM_TO_WINNER pkFMToWinner;

			pkFMToWinner.cuid = m_refManager.GetWinnerCUID();
			pkFMToWinner.text_id = m_pCurWork->notice.textID;

			g_ServerInfo.send_to_all_gs( &pkFMToWinner, pkFMToWinner.usLength );
		}
	}
}


void CFreeMatchModule::ProcWork_SendResult()
{
	if( m_pCurWork )
	{
		/* �պ� ��� ��û�� ������, 
		����� ����, �ٽ� SendResult �� ������, Ŭ���̾�Ʈ�� �����ϵ��� �Ѵ�.
		*/
		CMSG_FM_SEND_RESULT pkSendResult;
		pkSendResult.ucResult = static_cast<u_char>(m_refManager.GetResultInfo());
		m_refManager.GetResultInfo( &pkSendResult.winnerRecord );

		g_ServerInfo.send_to_all_gs( &pkSendResult, pkSendResult.usLength );

		/*
		CMSG_FM_REQ_RESULT pkReq;

		g_ServerInfo.send_to_gs( &pkReq, pkReq.usLength, FREEMATCH_SERVER_INDEX );
		*/
	}
}


void CFreeMatchModule::ProcWork_RewardNick()
{
	if( m_pCurWork && en_fm_result_one_winer == m_refManager.GetResultInfo() )
	{
		CMSG_FM_REWARD_NICK pkFMRewardNick;
		pkFMRewardNick.usRank = m_curMatchEvent.fm_rank;
		pkFMRewardNick.iWinnerCUID = m_refManager.GetWinnerCUID();

		// ���� ������ ���� �� �����Ͽ� ��Ŷ�� ������.
		__ge_reward * pNickReward = m_refManager.GetReward( m_curMatchEvent.reward_index );
		
		while( pNickReward )
		{
			if( en_ge_reward_nick == pNickReward->reward_type )
			{
				pkFMRewardNick.usNickID = pNickReward->nick.usNickID;
				break;
			}
			pNickReward = pNickReward->pNext;
		}

		m_refManager.SetWinnerNick( pkFMRewardNick.usNickID );

		m_refManager.ChangeNickOwner( m_curMatchEvent.fm_rank, pkFMRewardNick.usNickID );

		g_ServerInfo.send_to_all_gs( &pkFMRewardNick, pkFMRewardNick.usLength );
	}
	else
	{
		m_refManager.WriteFMResult_NoWinner( m_curMatchEvent.fm_rank );
	}
}


void CFreeMatchModule::ProcWork_RewardItem()
{
	if( m_pCurWork && en_fm_result_one_winer == m_refManager.GetResultInfo() )
	{
		CMSG_FM_REWARD_ITEM pkFMRewardItem;

		int i = 0;
		__ge_reward * pItemReward = m_refManager.GetReward( m_curMatchEvent.reward_index );

		while( pItemReward && i < 8 )
		{
			if( en_ge_reward_nick != pItemReward->reward_type )
			{
				pkFMRewardItem.Item[i].ucToWhom = static_cast<u_char>(pItemReward->reward_type);
				pkFMRewardItem.Item[i].cItemType = pItemReward->item.cType;
				pkFMRewardItem.Item[i].sItemID = pItemReward->item.sID;
				pkFMRewardItem.Item[i].ucCount = pItemReward->item.ucCount;

				++i;
			}

			pItemReward = pItemReward->pNext;
		}
		
		g_ServerInfo.send_to_all_gs( &pkFMRewardItem, pkFMRewardItem.usLength );
	}
}


void CFreeMatchModule::ProcWork_BuffAll()
{
	if( m_pCurWork )
	{
		if( m_curMatchEvent.buff_index )
		{
			CMSG_FM_BUFF_ALL pkFMBuffAll;

			pkFMBuffAll.sBuffIndex =  m_curMatchEvent.buff_index;
			pkFMBuffAll.ucBuffStep = m_curMatchEvent.buff_step;
			pkFMBuffAll.usBuffTime = m_curMatchEvent.buff_time;

			g_ServerInfo.send_to_all_gs( &pkFMBuffAll, pkFMBuffAll.usLength );
		}
	}
}


void CFreeMatchModule::ProcWork_Clean()
{
	SetFMState( en_fm_step_none );		// ���� ����
	m_pCurWork = NULL;

	m_refManager.CleanupCurEvent();
	

	// GS �� ���� : ���� ���� ��Ų��.
	CMSG_FM_CLEAN pkFMClean;

	// ���� ������ Ȯ���� ��, ä���� ������.
	if( m_refManager.ReserveNextEvent() )
		CopyMemory( &pkFMClean.event_info, &m_curMatchEvent, sizeof(_sFreeMatchEvent) );
	else
		ZeroMemory( &pkFMClean.event_info, sizeof(_sFreeMatchEvent) );

	g_ServerInfo.send_to_all_gs( &pkFMClean, pkFMClean.usLength );
}



void CFreeMatchModule::PrintAllInfo()
{
	printf( "== Free Match Module : Type %d, Rank %u == \n", m_iMatchType, m_curMatchEvent.fm_rank );

	__fm_event_work * pWork;

	for( int i = 0; i < 3; ++i )
	{
		switch( i )
		{
		case 0:
			pWork = m_readyWork.pNext;
			if( pWork )
				printf( " Ready Works\n" );
			else 
				printf( " No Ready Works\n" );
			break;
		case 1:
			pWork = m_startWork.pNext;
			if( pWork )
				printf( " Start Works\n" );
			else 
				printf( " No Start Works\n" );
			break;
		case 2:
			pWork = m_endWork.pNext;
			if( pWork )
				printf( " End Works\n" );
			else 
				printf( " No End Works\n" );
			break;
		}

		while( pWork )
		{
			printf( "   work : %s - ", g_match_work_string[pWork->work] );
			switch( pWork->work )
			{
			case en_fm_work_ready:				// ready �̺�Ʈ ó��.
				break;
			case en_fm_work_start:				// start �̺�Ʈ ó��.
				break;
			case en_fm_work_end:				// end �̺�Ʈ ó��
				break;
			case en_fm_work_notice:				// ���� ����
				printf( "Text ID %u - Arg %u", pWork->notice.textID, pWork->notice.arg );
				break;
			case en_fm_work_notice_winner:		// ���� : ����� ������ ����
				printf( "Text ID %u - Arg %u", pWork->notice.textID, pWork->notice.arg );
				break;
			case en_fm_work_notice_nowinner:	// ���� : ���º� ����
				printf( "Text ID %u - Arg %u", pWork->notice.textID, pWork->notice.arg );
				break;
			case en_fm_work_to_winner:			// ���ڿ��Ը�.
				printf( "Text ID %u - Arg %u", pWork->notice.textID, pWork->notice.arg );
				break;
			case en_fm_work_send_result:		// ����� �����ش�.
				break;
			case en_fm_work_reward_nick:			// ��ȣ ���� ����,
				break;
			case en_fm_work_reward_item:
				break;
			case en_fm_work_buff_all:			// �� ������ ��� ����.
				printf( "Buff %d", pWork->buff_all.id );
				break;
			case en_fm_work_clean:				// ���� ����.
				break;
			}
			printf( "\n" );

			pWork = pWork->pNext;
		}
	}
}	

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
NDQueueMemoryPool<struct __gevent_timer *> * NDDList<struct __gevent_timer *>::m_pPool = NULL;

bool CGameEventManager::CreateGEventManager( int pool_count, LONG period_ms )
{
	CGameEventParser parser;
	if( false == parser.LoadGameEventScript( *this, FILE_FREE_MATCH_SCRIPT ) )
	{
		printf( "[INIT ERROR] Parse '%s' File\n", FILE_FREE_MATCH_SCRIPT );
		g_pSystemLog->LogFileWrite( "[INIT ERROR] Parse '%s' File\n", FILE_FREE_MATCH_SCRIPT );
		return false;
	}

	if( false == CheckEventList() )
	{
		printf( "[INIT ERROR] Game Event List Error\n" );
		g_pSystemLog->LogFileWrite( "[INIT ERROR] Game Event List Error\n" );
		return false;
	}

	m_period_ms = period_ms;
	__gevent_timer * pTimer = NULL;
	if( false == m_timer_pool.InitializeMemoryPool(pool_count) )
	{
		printf( "[INIT ERROR] memory low. timer pool\n" );
		g_pSystemLog->LogFileWrite( "[INIT ERROR] memory low. timer pool\n" );
		return false;
	}

	for( int i = 0 ; i < pool_count ; ++i )
	{
		pTimer = new __gevent_timer;
		if( pTimer )
			m_timer_pool.insert_node( pTimer );
	}

	DWORD dw;
	HANDLE h = BEGINTHREADEX( NULL, 0, RunTimerThread, this, 0, &dw );

	if( h == NULL )
	{
		printf( "[INIT ERROR] G-Event Thread\n" );
		g_pSystemLog->LogFileWrite( "[INIT ERROR] G-Event Thread\n" );
		return false;
	}
	CloseHandle(h);

	m_pGDB->GetFMWinnerInfo( this );

	PrintModule();
	//PrintSchedule();
	PrintReward();

	ReserveNextEvent();


	return true;
}


bool CGameEventManager::CheckEventList()
{
	// ��� �̺�Ʈ�� �˻��Ͽ�, ������ �� ǥ���ϰ�, 
	// ������ �� �����Ѵ�. : ���� ��� �̺�Ʈ�� ���� �����Ѵ�.
	_sGEvent * pCurEvent = NULL;
	_sGEvent * pNextEvent = NULL;		// ������ �̺�Ʈ.

	time_t curTimeSec;
	time(&curTimeSec);
	_sND_TimeUnit curTime;
	_sND_TimeUnit startTime;

	time_t time1, time2;

	curTimeSec += 30;		// 30�� ������ �͵鸸 �����Ѵ�.
	CND_TimeUnitFunc::GetTime_FromSecond( curTime, curTimeSec );

	int size = static_cast<int>(m_vecEventList.size());
	for( int i = 0 ; i < size; ++i )
	{
		pCurEvent = m_vecEventList[i];

		// �ʱ� ȣ�� ���ĺ��ʹ�, ended �� �߻��ϹǷ�, not_reserve ���� �ƴ����� �˻��ϵ��� �ڵ� �߰��߽�.
		startTime = pCurEvent->ready_time;

		if( startTime.GetValueType() >= curTime.GetValueType() )
		{
			// ������ �� �˻����� �ʴ´�.

			// 1. start & end Ÿ��
			time1 = CND_TimeUnitFunc::Get_FromTimeUnit(pCurEvent->start_time);
			time2 = CND_TimeUnitFunc::Get_FromTimeUnit(pCurEvent->end_time);
			if( time1 + 60 >= time2 )
			{
				printf( "[FM] Event Time Error - Start & End time\n" );
				printf( " :: Event %d - %u-%u-%u %u:%u\n", 
					pCurEvent->event_temp_index, 
					pCurEvent->ready_time.uiYear+2000, pCurEvent->ready_time.uiMonth, 
					pCurEvent->ready_time.uiDay, pCurEvent->ready_time.uiHour, pCurEvent->ready_time.uiMinute );

				g_pSystemLog->LogFileWrite( "[FM] Event Time Error - Start & End time\n" );
				g_pSystemLog->LogFileWrite( " :: Event %d - %u-%u-%u %u:%u\n", 
					pCurEvent->event_temp_index, 
					pCurEvent->ready_time.uiYear+2000, pCurEvent->ready_time.uiMonth, 
					pCurEvent->ready_time.uiDay, pCurEvent->ready_time.uiHour, pCurEvent->ready_time.uiMinute );
				return false;
			}

			// 2. ���� �̺�Ʈ�� ��ġ���� �˻��Ѵ�.
			if( i < size-1 )
			{
				pNextEvent = m_vecEventList[i+1];
				time1 = CND_TimeUnitFunc::Get_FromTimeUnit(pCurEvent->end_time);
				time2 = CND_TimeUnitFunc::Get_FromTimeUnit(pNextEvent->ready_time);
				if( time1 + 60 >= time2 )
				{
					// 
					printf( "[FM] Event Time Error - Diff. Cur & Next Event\n" );
					printf( " :: Event %d - %u-%u-%u %u:%u\n", 
						pCurEvent->event_temp_index, 
						pCurEvent->ready_time.uiYear+2000, pCurEvent->ready_time.uiMonth, 
						pCurEvent->ready_time.uiDay, pCurEvent->ready_time.uiHour, pCurEvent->ready_time.uiMinute );

					g_pSystemLog->LogFileWrite( "[FM] Event Time Error - Diff. Cur & Next Event\n" );
					g_pSystemLog->LogFileWrite( " :: Event %d - %u-%u-%u %u:%u\n", 
						pCurEvent->event_temp_index, 
						pCurEvent->ready_time.uiYear+2000, pCurEvent->ready_time.uiMonth, 
						pCurEvent->ready_time.uiDay, pCurEvent->ready_time.uiHour, pCurEvent->ready_time.uiMinute );
					
					return false;
				}
			}
		}
	}

	return true;
}


bool CGameEventManager::ReserveNextEvent()
{
	// ��� �̺�Ʈ�� �˻��Ͽ�, ������ �� ǥ���ϰ�, 
	// ������ �� �����Ѵ�. : ���� ��� �̺�Ʈ�� ���� �����Ѵ�.
	_sGEvent * pEvent = NULL;
	_sGEvent * pFirstEvent = NULL;		// ������ �̺�Ʈ.

	time_t curTimeSec;
	time(&curTimeSec);
	_sND_TimeUnit curTime;
	_sND_TimeUnit startTime;


	curTimeSec += 30;		// 30�� ������ �͵鸸 �����Ѵ�.
	CND_TimeUnitFunc::GetTime_FromSecond( curTime, curTimeSec );

	int size = static_cast<int>(m_vecEventList.size());
	for( int i = 0 ; i < size; ++i )
	{
		pEvent = m_vecEventList[i];

		// �ʱ� ȣ�� ���ĺ��ʹ�, ended �� �߻��ϹǷ�, not_reserve ���� �ƴ����� �˻��ϵ��� �ڵ� �߰��߽�.
		if( 0 == pEvent->ready_time.uiYear )
			startTime = pEvent->start_time;
		else
			startTime = pEvent->ready_time;

		// ������ �Ͱ�, ���� ���� �з��Ѵ�. : STATE_ENDED, STATE_NOT_RESERVE 
		// STATE_NOT_RESERVE �� �� �� ù ��° ���� ����Ѵ�.��
		if( startTime.GetValueType() < curTime.GetValueType() )		// ��������.
			pEvent->event_state = GEVENT_STATE_ENDED;
		else 
		{
			pEvent->event_state = GEVENT_STATE_NOT_RESERVE;
			if( NULL == pFirstEvent )	// ù ��°�� ����Ѵ�.
				pFirstEvent = pEvent;
		}
	}

	// ���� �̺�Ʈ�� �ʱ�ȭ ��Ų��.
	if( m_pCurMatchModule )
	{
		m_pCurMatchModule->SetFMEventInfo( NULL );
		m_pCurMatchModule->SetFMState( en_fm_step_none );
	}

	if( pFirstEvent )
	{
		// �����Ѵ�.
		if( GEVENT_TYPE_FREE_MATCH == pFirstEvent->event_type )
		{
			_sFreeMatch_Arg arg;
			arg.event_temp_index = pFirstEvent->event_temp_index;
			arg.arg1 = 0;

			if( pFirstEvent->ready_time.uiYear )
			{
				arg.work = en_fm_work_ready;
				AddTimer( GEVENT_TYPE_FREE_MATCH, pFirstEvent->ready_time, &arg, sizeof(arg) );
			}

			arg.work = en_fm_work_start;
			AddTimer( GEVENT_TYPE_FREE_MATCH, pFirstEvent->start_time, &arg, sizeof(arg) );

			arg.work = en_fm_work_end;
			AddTimer( GEVENT_TYPE_FREE_MATCH, pFirstEvent->end_time, &arg, sizeof(arg) );


			// �̺�Ʈ ������ ��⿡ �����Ѵ�.
			_sFreeMatchEvent * pFMEvent = static_cast<_sFreeMatchEvent*>(pFirstEvent);

			m_pCurMatchModule = GetMatchModule( pFMEvent->match_type );
			if( m_pCurMatchModule )
			{
				m_pCurMatchModule->SetFMEventInfo( pFMEvent );
				m_pCurMatchModule->SetFMState( en_fm_step_none );
			}

			PrintConsole( "[GEVENT] Reserve Match : Type %u, Rank %u\n", pFMEvent->match_type, pFMEvent->fm_rank );
			g_pSystemLog->LogFileWrite( "[GEVENT] Reserve Match : Type %u, Rank %u\n", pFMEvent->match_type, pFMEvent->fm_rank );
			PrintConsole( "  %u-%u-%u %u:%u\n", pFMEvent->ready_time.uiYear+2000, pFMEvent->ready_time.uiMonth, pFMEvent->ready_time.uiDay, 
				pFMEvent->ready_time.uiHour, pFMEvent->ready_time.uiMinute );
			g_pSystemLog->LogFileWrite( "  %u-%u-%u %u:%u\n", pFMEvent->ready_time.uiYear+2000, pFMEvent->ready_time.uiMonth, pFMEvent->ready_time.uiDay, 
				pFMEvent->ready_time.uiHour, pFMEvent->ready_time.uiMinute );
		}

		pEvent->event_state = GEVENT_STATE_RESERVED;		// ���¸� Resered �� �ٲ۴�.

		return true;
	}

	return false;
}


bool CGameEventManager::ReserveRestartEvent()
{
	//  System Command : restart �� �����Ѵ�.
	_sSystem_Arg cmd;
	cmd.sys_cmd = en_gevent_syscmd_restart;

	AddTimer( GEVENT_TYPE_SYSTEM, 0, &cmd, sizeof(cmd) );

	return true;
}

void CGameEventManager::CancelAllEvent()
{
	LockTimer();

	__gevent_timer * p = NULL;
	while( false == m_timer.empty() )
	{
		p = m_timer.top();
		m_timer.pop();
		m_timer_pool.insert_node( p );
	}

	UnlockTimer();
}

void CGameEventManager::ResetEventManager()
{
	if( m_pCurMatchModule && en_fm_step_none != m_pCurMatchModule->GetFMState() )// ������
	{
		printf( " -> Free Match is progressing. No Operations.\n" );
		return;
	}

	// ���� �̺�Ʈ�� ��ϵǾ� ���� ���̴�. ���ش�.
	CancelAllEvent();

	// �����ٸ� �ʱ�ȭ �Ѵ�.
	int i;

	LockTimer();

	try
	{

		int size = static_cast<int>(m_vecEventList.size());
		for( i = 0; i < size; ++i )
		{
			if( m_vecEventList[i] )
				delete m_vecEventList[i];
		}
		m_vecEventList.clear();

		size = static_cast<int>(m_vecReward.size());
		for( i = 0; i < size; ++i )
		{
			if( m_vecReward[i] )
			{
				m_vecReward[i]->release();
				delete m_vecReward[i];
			}
		}
		m_vecReward.clear();
	}
	catch( ... )
	{
	}

	UnlockTimer();

	CGameEventParser parser;
	if( false == parser.LoadGameEventScript( *this, FILE_FREE_MATCH_SCRIPT, true ) )
	{
		printf( "[INIT ERROR] Parse '%s' File\n", FILE_FREE_MATCH_SCRIPT );
		return;
	}

	// GS �� ���� : ���� ���� ��Ų��.
	CMSG_FM_CLEAN pkFMClean;

	// ���� ������ Ȯ���� ��, ä���� ������.
	if( ReserveNextEvent() && m_pCurMatchModule )
		CopyMemory( &pkFMClean.event_info, m_pCurMatchModule->GetFMEventInfo(), sizeof(_sFreeMatchEvent) );
	else
		ZeroMemory( &pkFMClean.event_info, sizeof(_sFreeMatchEvent) );

	g_ServerInfo.send_to_all_gs( &pkFMClean, pkFMClean.usLength );
}


//////////////////////////////////////////////////////////////////////////
// �̺�Ʈ ó����
// 1. System Event Process.
//////////////////////////////////////////////////////////////////////////
void CGameEventManager::Proc_SystemEvent( _sSystem_Arg * pArg )
{
	printf( "Proc Event : System work(%d)\n", pArg->work );

	switch( pArg->work )
	{
	case en_gevent_syscmd_restart:
		ReserveNextEvent();
		break;
	case en_gevent_syscmd_reserve_next:
		break;
	}
}



//////////////////////////////////////////////////////////////////////////
// �̺�Ʈ ó����. 
// 2. Free Match Event Process
//////////////////////////////////////////////////////////////////////////

void CGameEventManager::Proc_FreeMatchEvent( _sFreeMatch_Arg * pArg )
{
	if( m_pCurMatchModule )
		m_pCurMatchModule->EventReceive( pArg );
}

void CGameEventManager::AddEvent( _sGEvent * p )
{
	if( p )
	{
		p->event_temp_index = GetLastTempIndex();
		IncLastTempIndex();

		LockTimer();
		m_vecEventList.push_back( p );
		UnlockTimer();
	}
}


unsigned int __stdcall CGameEventManager::RunTimerThread( LPVOID lp )
{
	if( lp )
	{
		CGameEventManager * t = reinterpret_cast<CGameEventManager*>(lp);
		return t->RunTimer();
	}

	return 0;
}


unsigned int CGameEventManager::RunTimer()
{
	unsigned int ret = 0;

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;
	LARGE_INTEGER li = {0, } ;

	if( 0 == m_period_ms )
		m_period_ms = 1000;		// �⺻���� 1�ʷ� ����.

	SetWaitableTimer( hTimer, &li, m_period_ms, NULL, NULL, TRUE ) ;

	__gevent_timer * pEvent = NULL;
	time_t cur;
	_sFreeMatch_Arg * pMatchArg;
	_sSystem_Arg * pSystemArg;

	printf( " - Free match timer start.\n" );

	while(1) {
		WaitForSingleObject( hTimer, INFINITE );

		time(&cur);

		if( IsEnd() )
			break;

		while( NULL != ( pEvent = GetEvent(cur) ) )
		{
			try
			{
				//////////////////////////////////////////////////////////////////////////

				//////////////////////////////////////////////////////////////////////////
				// Timer Event ó��.
				//////////////////////////////////////////////////////////////////////////
				switch( pEvent->event_type )
				{
				case GEVENT_TYPE_FREE_MATCH:
					pMatchArg = reinterpret_cast<_sFreeMatch_Arg*>( pEvent->GetInfo() );
					Proc_FreeMatchEvent( pMatchArg );
					PrintAll( "Event Time(%u) : match work(%d)\n", pEvent->event_time, pMatchArg->work );
					break;
				case GEVENT_TYPE_SYSTEM:
					pSystemArg = reinterpret_cast<_sSystem_Arg *>( pEvent->GetInfo() );
					Proc_SystemEvent( pSystemArg );
					PrintAll( "Event Time(%u) : system cmd(%d)\n", pEvent->event_time, pSystemArg->sys_cmd );
					break;
				}


				//////////////////////////////////////////////////////////////////////////
			}	// try.
			catch(...)
			{
			}

			// Release �� �ݵ�� �ҷ��� �Ѵ�. 
			ReleaseTimer(pEvent);
		}
	}

	_endthreadex(0);

	return ret;
}


void CGameEventManager::AddTimer( int type, _sND_TimeUnit& when, void * info, size_t info_size )
{
	if( info_size > EN_GEVENT_INFO_SIZE )
		return;

	// ���� �ð��� ���ϰ�, 
	// when ���� ���̸� ���� ��
	// ���̸� �ʷ� ��ȯ�Ͽ�, �߰��Ѵ�.

	time_t cur;
	time(&cur);
	tm * ptm = localtime( &cur );

	_sND_TimeUnit curTime;
	curTime.uiYear = ptm->tm_year-100;
	curTime.uiMonth = ptm->tm_mon+1;
	curTime.uiDay = ptm->tm_mday;
	curTime.uiHour = ptm->tm_hour;
	curTime.uiMinute = ptm->tm_min;
	curTime.uiSecond = ptm->tm_sec;

	printf( "Reserve : Cur = %u", cur );
	cur += CND_TimeUnitFunc::GetDiffSecond( curTime, when );
	printf( ", Reserver = %u\n", cur );

	AddTimer( type, cur, info, info_size );
}


void CGameEventManager::AddTimer( int type, time_t when, void * info, size_t info_size )
{
	if( info_size > EN_GEVENT_INFO_SIZE )
		return;

	__gevent_timer * p = m_timer_pool.pop_front();
	if( p )
	{
		p->event_time = when;
		p->event_type = type;
		if( NULL == info || 0 == info_size )
			ZeroMemory( p->arg, sizeof(p->arg) );
		else
			CopyMemory( p->arg, info, info_size );

		m_timer.push( p );
	}
}


//////////////////////////////////////////////////////////////////////////



void CGameEventManager::PrintModule( int type /* = -1  */ )
{
	if( -1 == type )
	{
		for( unsigned int i = 0; i < m_matchModule.size(); ++i )
		{
			if( m_matchModule[i].pMatch )
				m_matchModule[i].pMatch->PrintAllInfo();
		}
	}
	else
	{
		CFreeMatchModule * pModule = GetMatchModule( type );
		if( pModule )
		{
			pModule->PrintAllInfo();
		}
		else
		{
			printf( "Not found match module( type = %d )\n", type );
		}
	}
}

void CGameEventManager::PrintSchedule()
{
	_sGEvent * pEvent;
	_sFreeMatchEvent * pFM;
	for( unsigned int i = 0; i < m_vecEventList.size(); ++i )
	{
		pEvent = m_vecEventList[i];

		switch( pEvent->event_type )
		{
		case GEVENT_TYPE_FREE_MATCH:
			pFM = static_cast<_sFreeMatchEvent*>(pEvent);

			printf( "== Free Match Event : %d ==\n", pFM->event_temp_index );
			printf( " Type : %u, Rank : %u\n", pFM->match_type, pFM->fm_rank );
			printf( " Level : %d - %d\n", pFM->from_level, pFM->to_level );
			printf( " Reward : %d\n", pFM->reward_index );
			printf( " Position %d, Respawn %d\n", pFM->pos_region, pFM->respawn_region );

			// Time ����.
			if( pFM->ready_time.uiYear )
			{
				printf( " Ready : " );
				pFM->ready_time.Print();		printf( " \n" );
			}
			else 
				printf( " No Ready date\n" );

			if( pFM->start_time.uiYear )
			{
				printf( " Start : " );
				pFM->start_time.Print();		printf( " \n" );
			}
			else
				printf( " No Start date\n" );

			if( pFM->end_time.uiYear )
			{
				printf( " End : " );
				pFM->end_time.Print();		printf( " \n" );
			}
			else
				printf( " No End date\n" );

			printf( "======================================\n\n" );

			break;
		default:
			break;
		}
	}
}

void CGameEventManager::PrintReward( u_short index /* = 0 */ )
{
	__ge_reward * pReward = NULL;

	printf( "==  Reward List  ==\n" );

	if( index )
	{
		pReward = GetReward( index );
		if( pReward )
		{
			pReward->Print();
		}
	}
	else
	{
		for( unsigned int i = 0; i < m_vecReward.size(); ++i )
		{
			pReward = m_vecReward[i];
			if( pReward )
			{
				pReward->Print();
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// GS ��Ŷ ó��
//////////////////////////////////////////////////////////////////////////
void CGameEventManager::ProcGSPacket( CMSG_FREE_MATCH * pMatch, _h_DS_Com * pFromGS )
{
	switch( pMatch->ucFMWork )
	{
	//case en_dsfm_ready:				// ready �̺�Ʈ ó��.
	//	break;
	//case en_dsfm_start:					// start �̺�Ʈ ó��.
	//	break;
	//case en_dsfm_end:					// end �̺�Ʈ ó��
	//	break;
	//case en_dsfm_notice:				// ���� ����
	//	break;
	//case en_dsfm_notice_winner:			// ���� : ����� ������ ����
	//	break;
	//case en_dsfm_notice_nowinner:		// ���� : ���º� ����
	//	break;
	//case en_dsfm_to_winner:				// ���ڿ��Ը� ���� �޽���.
	//	break;
	//case en_dsfm_req_result:				// GS �� ����� ��û�Ѵ�.
	//	break;
	case en_dsfm_result_info:			// req_result �� �������� GS ���� ����� ������ ��´�.
		{
			CMSG_FM_RESULT_INFO * pInfo = static_cast<CMSG_FM_RESULT_INFO*>(pMatch);
			SetResultInfo( pInfo->ucResult );
			SetWinnerInfo( &pInfo->winnerRecord );

			PrintAll( "[FM] Result %u - winner %s\n", pInfo->ucResult, pInfo->winnerRecord.chr_name );

			/* proc_send.. �� �̵�.
			CMSG_FM_SEND_RESULT pkSendResult;
			pkSendResult.ucResult = pInfo->ucResult;
			CopyMemory( &pkSendResult.winnerRecord, &pInfo->winnerRecord, sizeof(_sFM_Record) );

			g_ServerInfo.send_to_all_gs( &pkSendResult, pkSendResult.usLength );
			*/
		}
		break;
	//case en_dsfm_send_result:			// ����� Ŭ���̾�Ʈ�� �������� �Ѵ�.
	//	break;
	//case en_dsfm_reward_nick:			// ��ȣ ���� ����,
	//	break;
	//case en_dsfm_reward_item:			// ������ ���� ����
	//	break;
	//case en_dsfm_buff_all:				// �� ������ ��� ����.
	//	break;
	//case en_dsfm_clean:					// ���� ����.
	//	break;

	case en_dsfm_req_cur_matchinfo:	// ���� ��ġ ������ ��û�Ѵ�. : �ʱ⿡ �ѹ� ��û�Ѵ�.
		{
			CMSG_FM_CUR_MATCHINFO pkInfo;
			
			if( m_pCurMatchModule )
			{
				pkInfo.event_state = static_cast<u_char>(m_pCurMatchModule->GetFMState());
                CopyMemory( &pkInfo.event_info, m_pCurMatchModule->GetFMEventInfo(), sizeof(_sFreeMatchEvent) );
				pkInfo.event_time = m_pCurMatchModule->GetFMEventTime();
			}
			else
			{
				pkInfo.event_state = en_fm_step_none;
				ZeroMemory( &pkInfo.event_info, sizeof(_sFreeMatchEvent) );
				pkInfo.event_time = 0;
			}

			pFromGS->Send( pkInfo.usLength, (char*)&pkInfo );

			SendNickOnwerList( pFromGS );
		}
		break;
	//case en_dsfm_cur_matchinfo:			// ���� ��ġ ������ �˷��ش�.
	//	break;
	}
}


void CGameEventManager::ChangeNickOwner( u_short rank, u_short nick_id )
{
	u_int i = 0; 
	while( i < m_NickOwnerList.size() )
	{
		if( rank == m_NickOwnerList[i].fm_rank )
			break;
		++i;
	}

	if( i < m_NickOwnerList.size() )
	{
		m_NickOwnerList[i].reward_nick_id = nick_id;
		m_NickOwnerList[i].winner_cuid = m_winnerInfo.chr_index;
	}
	else
	{
		_sFM_LastRewardNickInfo info;
		info.fm_rank = rank;
		info.reward_nick_id = nick_id;
		info.winner_cuid = m_winnerInfo.chr_index;

		m_NickOwnerList.push_back( info );
	}

	m_pGDB->UpdateFMWinner( static_cast<int>(rank), static_cast<int>(nick_id), 
		m_winnerInfo.chr_index, m_winnerInfo.chr_name );

	g_pSystemLog->LogFileWrite( "[FM] Change Nick Owner - C<%s> Rank(%u), NickID %d\n", m_winnerInfo.chr_name, rank, nick_id );
}

void CGameEventManager::WriteFMResult_NoWinner( u_short rank )
{
	m_pGDB->UpdateFMWinner( static_cast<int>(rank), 0, 0, "No Winner" );
}


void CGameEventManager::SendNickOnwerList( _h_DS_Com * pGS )
{
	CMSG_FM_CUR_NICK_OWNER	pkNickOwner;	

	pkNickOwner.rank_count = m_NickOwnerList.size();

	int i = 0;
	while( i < pkNickOwner.rank_count && i < FM_RANK_MAX_COUNT )
	{
		pkNickOwner.nick[i].fm_rank = m_NickOwnerList[i].fm_rank;
		pkNickOwner.nick[i].reward_nick_id = m_NickOwnerList[i].reward_nick_id;
		pkNickOwner.nick[i].winner_cuid = m_NickOwnerList[i].winner_cuid;
		++i;
	}

	pGS->Send( pkNickOwner.usLength, (char*)&pkNickOwner );
}


void CGameEventManager::AddWinnerInfoFromDB( int rank, int nick_id, int cuid )
{
	_sFM_LastRewardNickInfo info;

	u_int i = 0; 
	while( i < m_NickOwnerList.size() )
	{
		if( rank == static_cast<int>(m_NickOwnerList[i].fm_rank) )
			break;
		++i;
	}

	if( i < m_NickOwnerList.size() )
	{
		m_NickOwnerList[i].reward_nick_id = static_cast<u_short>(nick_id);
		m_NickOwnerList[i].winner_cuid = cuid;
	}
	else
	{
		info.fm_rank = static_cast<u_short>(rank);
		info.reward_nick_id = static_cast<u_short>(nick_id);
		info.winner_cuid = cuid;

		m_NickOwnerList.push_back( info );
	}
}

#endif _PD_GAME_EVENT_
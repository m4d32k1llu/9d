#pragma once

#ifdef _PD_GAME_EVENT_

#include <vector>
#include <queue>
#include <list>				// <== �̺�Ʈ ���� ���忡 ���δ�.
using namespace std;

#include "GEventStructure.h"

//////////////////////////////////////////////////////////////////////////
// ��� �̺�Ʈ�� CGameEventManager �� ����, 
// Ready, start, end �̺�Ʈ�� �ް� �ȴ�. 
// �׷���, ���� ������ �� �̺�Ʈ�� ���� ���( ��:CFreeMatchModule )�� �˾Ƽ�
// ó���Ѵ�.
//
//////////////////////////////////////////////////////////////////////////

class CGameEventManager;
//////////////////////////////////////////////////////////////////////////
// ���� �� ���� ���
//////////////////////////////////////////////////////////////////////////
class CFreeMatchModule
{
public:
	CFreeMatchModule( int match_type, CGameEventManager& mgr )
		: m_iMatchType( match_type ), m_dwLimitFlag(0), m_iMatchState(en_fm_step_none),
		m_refManager(mgr)
	{
		ZeroMemory( &m_readyWork, sizeof(__fm_event_work) );
		ZeroMemory( &m_startWork, sizeof(__fm_event_work) );
		ZeroMemory( &m_endWork, sizeof(__fm_event_work) );

		ZeroMemory( &m_FMArg, sizeof(_sFreeMatch_Arg) );

		m_readyWork.work = en_fm_work_ready;
		m_startWork.work = en_fm_work_start;
		m_endWork.work = en_fm_work_end;
	}

	void ResetEvent()
	{
		m_pCurWork = NULL;
		m_iMatchState = en_fm_step_none;
	}

	//////////////////////////////////////////////////////////////////////////

	// Constraints.
	void SetConstFlag( int en )
	{
		m_dwLimitFlag |= (0x01<<en);
	}
	bool IsConstFlag( int en )
	{
		return ( 0 != (m_dwLimitFlag & (0x01<<en)) );
	}
	DWORD GetConstraint()	{ return m_dwLimitFlag; }
	int GetMatchType()		{ return m_iMatchType; }

	// Work 
	void AddWork( int where, __fm_event_work * pWork );


	u_short GetReadyTime();

	void PrintAllInfo();

	//////////////////////////////////////////////////////////////////////////

public:
	void ReserveNextWork();
	void EventReceive( _sFreeMatch_Arg * pArg );

	void ProcWork_Ready();
	void ProcWork_Start();
	void ProcWork_End();
	void ProcWork_Notice();
	void ProcWork_NoticeWinner();
	void ProcWork_NoticeNowinner();
	void ProcWork_ToWinner();
	void ProcWork_SendResult();
	void ProcWork_RewardNick();
	void ProcWork_RewardItem();
	void ProcWork_BuffAll();
	void ProcWork_Clean();


public:
	void SetFMState( int state )	{ m_iMatchState = state; }
	int GetFMState()				{ return m_iMatchState; }

	void SetFMEventInfo( _sFreeMatchEvent * event )
	{
		if( event )
			CopyMemory( &m_curMatchEvent, event, sizeof(_sFreeMatchEvent) );
		else 
			ZeroMemory( &m_curMatchEvent, sizeof(_sFreeMatchEvent) );
	}
	_sFreeMatchEvent * GetFMEventInfo()
	{
		return &m_curMatchEvent;
	}
	time_t GetFMEventTime()	{ return m_lEventTime; }

protected:
	int			m_iMatchType;			//	Ÿ�� �ε���

	DWORD		m_dwLimitFlag;			// 

	int			m_iMatchState;			// FREE_MATCH_STEP

	__fm_event_work			m_readyWork;
	__fm_event_work			m_startWork;
	__fm_event_work			m_endWork;

	__fm_event_work *		m_pCurWork;

	_sFreeMatchEvent		m_curMatchEvent;

	time_t					m_lEventTime;



public:
	// Item Constraints.
	void AddUseItem( char type, short id )
	{
		__fm_use_item item( type, id );
		m_vecUseItem.push_back( item );
	}
	void AddCannotSkill( short skill )
	{
		m_vecCannotSkill.push_back( skill );
	}
	bool CanUseItem( char type, short id )
	{
		int size = static_cast<int>(m_vecUseItem.size());
		for( int itr=0; itr < size ; ++itr )
		{
			if( type == m_vecUseItem[itr].cType && id == m_vecUseItem[itr].sID )
				return true;
		}
		return false;
	}

protected:

	vector<__fm_use_item>		m_vecUseItem;
	vector<short>				m_vecCannotSkill;

	CGameEventManager&			m_refManager;

	_sFreeMatch_Arg				m_FMArg;
};



//////////////////////////////////////////////////////////////////////////
//
//  CGameEventManager 
//		- 
//////////////////////////////////////////////////////////////////////////
class CGameEventManager
{
	//////////////////////////////////////////////////////////////////////////
	// Create
	//////////////////////////////////////////////////////////////////////////
public:
	CGameEventManager( COLE_DB_Executer * pGDB )
	{
		if( FALSE == InitializeCriticalSectionAndSpinCount( &m_csTimer, 0x80001000 ) )
		{
			printf( "[INIT ERROR] memory low.( critical section )\n" );
			throw "";
		}

		m_bEnd = FALSE;
		m_usLastTempIndex = 1;
		m_sFreeMatchGS = FREEMATCH_SERVER_INDEX;			// �պ� ����.
		m_pCurMatchModule = NULL;
		m_winnerNick = 0;
		m_pGDB = pGDB;
	}
	virtual ~CGameEventManager()
	{
		m_timer_pool.ReleaseMemoryPool();
		DeleteCriticalSection(&m_csTimer);
	}
	bool CreateGEventManager( int pool_count, LONG period_ms );
	bool CheckEventList();

	//////////////////////////////////////////////////////////////////////////
	// ���� �� �ܺ� ��û �Լ���.
	bool ReserveNextEvent();
	bool ReserveRestartEvent();
	void CancelAllEvent();				// ����� �̺�Ʈ�� ����Ѵ�.
	void ResetEventManager();			// ���ο��� �Ľ̵� �Ѵ�.

	//////////////////////////////////////////////////////////////////////////
	// �̺�Ʈ ó����.
	// 1. System Event
	void Proc_SystemEvent( _sSystem_Arg * pArg );

	// 2. Free Match Event
	void Proc_FreeMatchEvent( _sFreeMatch_Arg * pArg );



	//////////////////////////////////////////////////////////////////////////
	// Timer Part
	//////////////////////////////////////////////////////////////////////////
public:
	inline void LockTimer()					{ EnterCriticalSection(&m_csTimer); }
	inline void UnlockTimer()				{ LeaveCriticalSection(&m_csTimer); }

	bool CheckEventInfo( size_t info_size )
	{
		// info �� ũ�Ⱑ 24 ����Ʈ�� ������ �ȵȴ�. 
		return ( info_size <= EN_GEVENT_INFO_SIZE );
	}

	// Ư�� �Ͻ÷� �̺�Ʈ �߰��ϴ� �Լ��� �ϳ� �� ������.
	void AddTimer( int type, _sND_TimeUnit& when, void * info, size_t info_size );
	void AddTimer( int type, time_t when, void * info, size_t info_size );
	__gevent_timer * GetEvent( time_t cur )
	{
		// �� ó�� ����� �ð��� üũ�Ͽ� �����ϰų�, ���ų�.
		__gevent_timer * p = NULL;
		LockTimer();
		if( false == m_timer.empty() )
		{
			p = m_timer.top();
			if( p )
			{
				if( p->event_time <= cur )
				{
					m_timer.pop();
				}
				else
					p = NULL;
			}
		}
		UnlockTimer();
		return p;
	};
	void ReleaseTimer( __gevent_timer * p )
	{
		if( p )
		{
			LockTimer();
			m_timer_pool.insert_node( p );
			UnlockTimer();
		}
	}

	static unsigned int __stdcall RunTimerThread( LPVOID lp );
	unsigned int RunTimer();		// <== ������ �Լ�.


	void PrintModule( int type = -1 );
	void PrintSchedule();
	void PrintReward(u_short index = 0);
	void PrintCurEvent()
	{
		if( m_pCurMatchModule )
			m_pCurMatchModule->PrintAllInfo();

		size_t count = m_NickOwnerList.size();
		for( size_t i = 0 ; i < count ; ++i )
		{
			_sFM_LastRewardNickInfo& r = m_NickOwnerList[i];
			printf( "Rank %u - Nick %d, Winner %d\n", r.fm_rank, r.reward_nick_id, r.winner_cuid );
		}
	}

protected:
	NDDList<__gevent_timer *>	m_timer_pool;

	priority_queue<__gevent_timer*, vector< __gevent_timer* >, _CmpTimer >		m_timer;
	CRITICAL_SECTION					m_csTimer;
	LONG								m_period_ms;

	COLE_DB_Executer *					m_pGDB;


	//////////////////////////////////////////////////////////////////////////
	// Event Part.
	//////////////////////////////////////////////////////////////////////////
public:
	inline BOOL IsEnd()			{ return m_bEnd; }

	u_short			GetLastTempIndex()	{ return m_usLastTempIndex; }
	void			IncLastTempIndex()	{ ++m_usLastTempIndex; }

	void AddEvent( _sGEvent * p );

	void CleanupCurEvent()
	{
	}

protected:
	BOOL					m_bEnd;
	u_short					m_usLastTempIndex;

	vector <_sGEvent *>		m_vecEventList;		// ��ũ���� ���ǵ� �̺�Ʈ�� �����Ѵ�.

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////


public:
	//////////////////////////////////////////////////////////////////////////
	// Match Module Functions.
	//////////////////////////////////////////////////////////////////////////
	void AddMatchModule( CFreeMatchModule * pModule )
	{
		__match_module_info info;
		info.pMatch = pModule;
		info.dwConst = pModule->GetConstraint();

		m_matchModule.push_back( info );
	}

	CFreeMatchModule * GetMatchModule( int match_type )
	{
		int size = static_cast<int>(m_matchModule.size());
		for( int i = 0; i < size; ++i )
		{
			if( m_matchModule[i].pMatch )
			{
				if( match_type == m_matchModule[i].pMatch->GetMatchType() )
					return m_matchModule[i].pMatch;
			}
		}

		return NULL;
	}

protected:

	struct __match_module_info
	{
		CFreeMatchModule *	pMatch;
		DWORD				dwConst;
	};

	vector<__match_module_info>		m_matchModule;

	CFreeMatchModule *			m_pCurMatchModule;

	short						m_sFreeMatchGS;			// �պ�� ����.

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
public:
	// Reward List
	void AddRewardInfo( __ge_reward * p )
	{
		if( p )
		{
			LockTimer();
			m_vecReward.push_back( p );
			UnlockTimer();
		}
	}
	__ge_reward * GetReward( u_short index )
	{
		__ge_reward * p = NULL;

		LockTimer();

		int size = static_cast<int>(m_vecReward.size());
		for( int itr = 0; itr < size; ++itr )
		{
			if( m_vecReward[itr] && index == m_vecReward[itr]->reward_index )
			{
				p = m_vecReward[itr];
				break;
			}
		}

		UnlockTimer();

		return p;
	}


protected:
	vector<__ge_reward *>		m_vecReward;


public:
	//////////////////////////////////////////////////////////////////////////
	// GS ��Ŷ ó����.
	void ProcGSPacket( CMSG_FREE_MATCH * pMatch, _h_DS_Com * pFromGS );




//////////////////////////////////////////////////////////////////////////
// ����� ����.
public:
	int GetWinnerCUID()							{ return m_winnerInfo.chr_index; }
	char * GetWinnerName()						{ return m_winnerInfo.chr_name; }
	int GetResultInfo()							{ return m_resultInfo; }			// EN_FM_RESULT
	void SetResultInfo( int result)				{ m_resultInfo = result; }
	void SetWinnerInfo( _sFM_Record * pInfo )	{ CopyMemory( &m_winnerInfo, pInfo, sizeof(_sFM_Record) ); }
	void GetResultInfo( _sFM_Record * pInfo )	{ if( pInfo ) CopyMemory( pInfo, &m_winnerInfo, sizeof(_sFM_Record) ); }
	void SetWinnerCuid( int cuid )				{ m_winnerInfo.chr_index = cuid; }
	void SetWinnerName( char * name )	
	{
		ZeroMemory( m_winnerInfo.chr_name, sizeof(en_charac_name_length+1) );
		strncpy( m_winnerInfo.chr_name, name, en_charac_name_length+1 );
	}
	void SetWinnerNick( u_short nick )
	{
		m_winnerNick = nick;
	}

	void ChangeNickOwner( u_short rank, u_short nick_id );
	void WriteFMResult_NoWinner( u_short rank );

	void SendNickOnwerList( _h_DS_Com * pGS );

	void AddWinnerInfoFromDB( int rank, int nick_id, int cuid );

protected:
	//////////////////////////////////////////////////////////////////////////
	// ����� ���� ( �ֱ� �������� ����� ���� )
	int							m_resultInfo;			// ��� ���� : 
	_sFM_Record					m_winnerInfo;			// 
	u_short						m_winnerNick;			// ���� ��ȣ  

	vector<_sFM_LastRewardNickInfo>		m_NickOwnerList;	// ���� ��ȣ�� ���� ����.
};

#endif	// _PD_GAME_EVENT_
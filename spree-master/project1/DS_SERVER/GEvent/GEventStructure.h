#pragma once

#ifdef _PD_GAME_EVENT_

//////////////////////////////////////////////////////////////////////////
// Game Event ���� ����
//////////////////////////////////////////////////////////////////////////

#define FREEMATCH_SERVER_INDEX			(17)

// ���� Ÿ��.
enum
{
	en_ge_reward_nick=0,			// ��ȣ ����
	en_ge_reward_item,				// ������ ���� : ���� ������ ��.
	en_ge_reward_item_to_male,		// ���ڿ��Ը� ����
	en_ge_reward_item_to_female,	//
};

// ���� ����.
struct __ge_reward
{
	u_short		reward_index;
	u_short		reward_type;		// en_fm_reward...

	union
	{
		struct 
		{
			char		cType;
			u_char		ucCount;
			short		sID;
		} item;

		struct 
		{
			u_short		usNickID;
			u_short		usValue1;

			u_short		usInfo1;
			u_short		usInfo2;
		} nick;
	};

	__ge_reward *	pNext;

	__ge_reward()
	{
		ZeroMemory( this, sizeof(__ge_reward) );
	}

	void release()
	{
		__ge_reward * pCur = pNext;
		__ge_reward * pDel = pNext;
		while( pCur )
		{
			pDel = pCur;
			pCur = pCur->pNext;
			delete pDel;			
		}
	}
	void add( __ge_reward * p )		// �ܺο���, ������ ��ü.
	{
		__ge_reward * pos = this;
		while( pos->pNext )
		{
			pos = pos->pNext;
		}
		pos->pNext = p;
		p->pNext = NULL;
	}

	void Print()
	{
		printf( "  Reward %2u\n", reward_index );

		__ge_reward * p = this;

		while( p )
		{
			switch( p->reward_type )
			{
			case en_ge_reward_nick:			// ��ȣ ����
				printf( "    Nick : %d\n", p->nick.usNickID );
				break;
			case en_ge_reward_item:			// ������ ���� : ���� ������ ��.
				printf( "    Item : %d - %d\n", p->item.cType, p->item.sID );
				break;
			case en_ge_reward_item_to_male:		// ���ڿ��Ը� ����
				printf( "    Item To Male : %d - %d\n", p->item.cType, p->item.sID );
				break;
			case en_ge_reward_item_to_female:	//
				printf( "    Item To Fema : %d - %d\n", p->item.cType, p->item.sID );
				break;
			}

			p = p->pNext;
		}

	}
};



//////////////////////////////////////////////////////////////////////////
// �̺�Ʈ Ÿ�԰� �̺�Ʈ�� ����
//////////////////////////////////////////////////////////////////////////
enum GEVENT_TYPE
{
	GEVENT_TYPE_SYSTEM=0,				// ��� ����� Ÿ�̸Ӹ� �����Ѵ�.
	GEVENT_TYPE_FREE_MATCH,				// 2007 09 12 ����� �̰� �ϳ��ۿ� ����.
	GEVENT_TYPE_COUNT,					// �� ���� �迭 ���� ������ ���δ�.
};

enum GEVENT_STATE
{
	GEVENT_STATE_NOT_RESERVE=0,			// ���� ������� ���� ����
	GEVENT_STATE_RESERVED,				// ����� �̺�Ʈ
	GEVENT_STATE_START,					// �������� �̺�Ʈ
	GEVENT_STATE_ENDED,					// ���� �̺�Ʈ.
};


//////////////////////////////////////////////////////////////////////////
// Ÿ�̸ӿ� ���� ���� ����.
//////////////////////////////////////////////////////////////////////////
enum
{
	EN_GEVENT_INFO_SIZE=56,			// ���ڴ� 56 ����Ʈ�� �Ѿ�� �ȵȴ�.
};

struct __gevent_timer
{
	time_t			event_time;
	int				event_type;		// << GEVENT_TYPE �� ����, arg �� ������ ����ü�� ĳ���� �Ѵ�.
	char			arg[EN_GEVENT_INFO_SIZE];

	char * GetInfo()	{ return arg; }
};

class _CmpTimer		// priority queue �� ���� ������ �����ϴ� Compare Ŭ����.
{
public:
	inline bool operator()(const __gevent_timer* p1, const __gevent_timer* p2) const
	{
		if( p1 && p2 )
		{
			return (p1->event_time >= p2->event_time);
		}
		else
			return true;
	}
};


//////////////////////////////////////////////////////////////////////////
// �����ٿ� �ִ� �̺�Ʈ���� �����ϴ� ����ü.
// �̺�Ʈ ����( GEVENT_TYPE ) �� ����, ��� �޾� ����Ѵ�.
//////////////////////////////////////////////////////////////////////////
// ��� event_state �� �����ϰ�, �������� ���� ������.
#pragma pack(push, gevent)
#pragma pack(1)

struct _sGEvent
{
	u_char				event_type;			// GEVENT_TYPE
	u_char				event_state;		// GEVENT_STATE
	u_short				event_temp_index;	// 

	// Ready/Start/End
	_sND_TimeUnit 		ready_time;
	_sND_TimeUnit 		start_time;
	_sND_TimeUnit 		end_time;

	_sGEvent()
		: event_type(0), event_state(GEVENT_STATE_NOT_RESERVE)
	{
		ZeroMemory( &ready_time, sizeof(_sND_TimeUnit) );
		ZeroMemory( &start_time, sizeof(_sND_TimeUnit) );
		ZeroMemory( &end_time, sizeof(_sND_TimeUnit) );
	}

	_sGEvent( u_char type )
		: event_type(type), event_state(GEVENT_STATE_NOT_RESERVE)
	{
		ZeroMemory( &ready_time, sizeof(_sND_TimeUnit) );
		ZeroMemory( &start_time, sizeof(_sND_TimeUnit) );
		ZeroMemory( &end_time, sizeof(_sND_TimeUnit) );
	}	

	_sGEvent( const _sGEvent& rh )
		: event_type(rh.event_type), event_state(rh.event_state)
	{
		CopyMemory( &ready_time, &rh.ready_time, sizeof(_sND_TimeUnit) );
		CopyMemory( &start_time, &rh.start_time, sizeof(_sND_TimeUnit) );
		CopyMemory( &end_time, &rh.end_time, sizeof(_sND_TimeUnit) );
	}
};

// �ý��� �̺�Ʈ�� _sGEvent�� �״�� ����Ѵ�. : GEVENT_TYPE_SYSTEM

// ������ �̺�Ʈ�� ���� ����ü : GEVENT_TYPE_FREE_MATCH 
// GS ��ſ��� ���̹Ƿ�, �ٲ��, GS�� �Բ� �ٲ�� �Ѵ�.
struct _sFreeMatchEvent : public _sGEvent
{
	u_char			match_type;
	u_char			match_index;			// �ƹ� �ǹ� ����. 

	short			from_level;
	short			to_level;

	u_short			fm_rank;				// ��ȸ�� ��� - ��ȣ�� ���� �ο��� ���� ���δ�.

	u_short			reward_index;

	short			pos_region;
	short			respawn_region;

	short			buff_index;
	u_short			buff_time;
	u_char			buff_step;
};


// ����� ����.
struct _sFM_Record
{
	int					chr_index;				// ĳ���� �ε���

	u_short				kill_count;				// ���� ȸ��
	u_short				dead_count;				// ���� ȸ��

	int					last_score;				// ���� ����.

	char				chr_name[en_charac_name_length+1];
	char				pad[3];

	_sFM_Record()
	{
		Reset();
	}

	void Reset()
	{
		ZeroMemory( this, sizeof(_sFM_Record) );
	}
};


enum EN_FM_RESULT		// �������� ���.
{
	en_fm_result_one_winer=0,	// One winner
	en_fm_result_more_winner,	// two or more winners.
	en_fm_result_none,		// no winner
};
// ��ȸ ��� ����
struct _sFM_Result
{
	int					result_info;		// EN_FM_RESULT
	_sFM_Record			winnerInfo;
};


// ���� ���� ��ȣ ������ ����.
// ��ȣ�� ������ �񹫴�ȸ�� ���(fm_rank)���� ���еǹǷ�, 
// fm_rank ���� ����ϰ�, ���� ��ȣ�� ������ �ٲ� �� �����Ƿ�, �Բ� �����Ѵ�.
struct _sFM_LastRewardNickInfo
{
	u_short				fm_rank;				// 
	u_short				reward_nick_id;

	int					winner_cuid;

	_sFM_LastRewardNickInfo()
		: fm_rank(0), reward_nick_id(0), winner_cuid(0)
	{
	}
};


#pragma pack( pop, gevent )

//////////////////////////////////////////////////////////////////////////
// Ÿ�̸� ���� ����
//////////////////////////////////////////////////////////////////////////
// �⺻ Ŭ���� : �̺�Ʈ Ÿ�Կ� ����, __timer_arg �� ��۹޾� ����Ѵ�.
struct __timer_arg
{
	u_short			event_temp_index;
	u_short			work;	
};



// 0. Event Type : GEVENT_TYPE_SYSTEM Ÿ�̸ӿ� �� ����
enum SYSTEM_EVENT_WORK	// GEVENT_TYPE_SYSTEM �� work ����
{
	en_gevent_syscmd_restart=0,				// �������� ����ϰ� �ٽ��Ѵ�.
	en_gevent_syscmd_reserve_next,			// ���� �̺�Ʈ�� �����Ѵ�.
};

struct _sSystem_Arg	: public __timer_arg
{
	int				sys_cmd;			// en_gevent_syscmd_...
};

//////////////////////////////////////////////////////////////////////////

// 1. Event Type : GEVENT_TYPE_FREE_MATCH Ÿ�̸ӿ� �� ����.
enum FREE_MATCH_WORK
{
	en_fm_work_ready=0,				// ready �̺�Ʈ ó��.
	en_fm_work_start,				// start �̺�Ʈ ó��.
	en_fm_work_end,					// end �̺�Ʈ ó��
	en_fm_work_notice,				// ���� ����
	en_fm_work_notice_winner,		// ���� : ����� ������ ����
	en_fm_work_notice_nowinner,		// ���� : ���º� ����
	en_fm_work_to_winner,			// ���ڿ��Ը�.
	en_fm_work_send_result,			// ����� �����ش�.
	en_fm_work_reward_nick,			// ��ȣ ���� ����,
	en_fm_work_reward_item,
	en_fm_work_buff_all,			// �� ������ ��� ����.
	en_fm_work_clean,				// ���� ����.
	en_fm_work_type_count,
};
struct _sFreeMatch_Arg : public __timer_arg
{
	int				arg1;			// ���� �� ����.
	u_char			match_type;		// 
	u_char			arg2;			// 
	u_short			arg3;			// 
};



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// ���� �񹫸� ���� ���� �� ����ü
//////////////////////////////////////////////////////////////////////////
// �������� ��� ���� (Constraints) ����
enum FREE_MATCH_CONST_FLAG
{
	// 32 �� �̸����� ��ƾ� �Ѵ�.
	en_fm_cf_hyperrun=0,
	en_fm_cf_breath,
	en_fm_cf_logout,
	en_fm_cf_escape,
	en_fm_cf_party,
	en_fm_cf_trade,
	en_fm_cf_info,
	en_fm_cf_recall, 
	en_fm_cf_shop,
	en_fm_cf_hide_me,	
	en_fm_cf_buff_reset,	// 10
	en_fm_cf_pvp_event,
};


// ��� ���� ������ ����
struct __fm_use_item
{
	char		cType;
	char		dummy;
	short		sID;

	__fm_use_item( char type, short id )
		: cType( type ), dummy(0), sID(id)
	{}
};


// �ܰ�����
enum FREE_MATCH_STEP		// event �ε� ����.
{
	en_fm_step_none=0,
	en_fm_step_ready,
	en_fm_step_start,
	en_fm_step_end
};

// GEventModule �� �ܰ躰 ���� �۾��� �����ϱ� ���� ����Ѵ�.
struct __fm_event_work
{
	int			work;
	u_short		usOrgSecond;		// ��ũ��Ʈ�� ���ǵ� �ʴ��� �ð�.
	u_short		usDiffPrevSecond;	// �� work ���� ���� �ð�( �� ���� )

	union
	{
		struct 
		{
			u_short		textID;
			u_short		arg;
		} notice;

		struct 
		{
			short id;
			u_short time;
		} buff_all;
	};

	__fm_event_work *	pNext;

	__fm_event_work()
	{
		ZeroMemory( this, sizeof(__fm_event_work) );
	}
};




#endif	// _PD_GAME_EVENT_
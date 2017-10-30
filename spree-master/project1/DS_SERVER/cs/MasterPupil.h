#pragma once

#include "../MyLibrary/NDUtilClass.h"

#ifdef _PD_MASTER_PUPIL_


class CMP_PupilPosRecalc
{
public:
	CMP_PupilPosRecalc()
	{
		Reset();
	}

	struct __temp_mp_rank_calc
	{
		u_int	point;
		u_char  rank;
		u_char  arr_idx;
		u_char	position;
		u_char  temp;
	};

public:
	void Reset()
	{
		m_count = 0;
		ZeroMemory( m_pupil, sizeof(m_pupil) );
	}

	int size()		{ return m_count; }

	void push_point( u_int point, u_char rank, u_char arr_idx );
	void recalc_pos();

	u_int get_point( int index )
	{
		int point = 0;

		if( index < NDD_MP_MAX_PUPIL_COUNT )
			point = m_pupil[index].point;

		return point;
	}

	void Print()
	{
		printf( "==== Recalc Object ====\n" );
		printf( " Count = %d\n", size() );
		for( int i = 0; i < size(); ++i )
		{
			printf( "Seq : %u, Rank %u, Point %u\n", m_pupil[i].arr_idx, m_pupil[i].rank, m_pupil[i].point );
		}
	}

protected:
	int						m_count;
	__temp_mp_rank_calc		m_pupil[NDD_MP_MAX_PUPIL_COUNT];
};



// ����ü�� ���, ��Ŷ���� ����ϴ� ���� cspk �� ���δ�.
// Ŭ���̾�Ʈ�� ����ϴ� ���, clpk �� ���δ�.

class CDSMaster
{
public:
	CDSMaster()
		: m_iIndex(0), m_uiTotalKillCount(0),m_uiMasterKillCount(0), m_ucBonusSkillStep(0)
	{
	}

	void Reset()
	{
		ZeroMemory( this, sizeof(CDSMaster) );
	}

	void ShowInfo();

	void		InitByDefault( char * master_name );

	void		SetIndex( int index )				{ m_iIndex = index; }
	void		SetTotalKillCount( u_int count )	{ m_uiTotalKillCount = count; }
	void		SetMasterKillCount( u_int count )	{ m_uiMasterKillCount = count; }
	void		SetMasterRank( u_char rank )		{ m_ucMasterRank = rank; }
	void		SetTotalPoint( u_int point )		{ m_uiMasterTotalPoint = point; }
	void		SetCurPoint( u_int point )			{ m_uiMasterCurPoint = point; }
	
	void		SetMasterLevel( u_char lv)			{ m_ucMasterLevel = lv; }
	void		SetMasterSex( u_char sex )			{ m_ucMasterSex = sex; }
	void		SetMasterMoonpa( u_char moonpa )	{ m_ucMasterMoonpa = moonpa; }
	void		SetMasterClass( u_char cls )		{ m_ucMasterClass = cls; }
	void		SetMasterGrade( u_char grade )		{ m_ucMasterGrade = grade; }
	void		SetMasterBonusSkillStep( u_char step )	{ m_ucBonusSkillStep = step; }

	void		SetMasterName(char * name )	
	{
		if( name )
		{
			strncpy( m_cMasterName, name, en_charac_name_length );
			m_cMasterName[en_charac_name_length] = 0;
		}	
		else
			ZeroMemory( m_cMasterName, en_charac_name_length+1 );
	}
	void		SetMasterNick( char * nick )
	{
		if( nick )
		{
			strncpy( m_cMasterNick, nick, NDD_MP_MASTERNICK_LENGTH );
			m_cMasterNick[NDD_MP_MASTERNICK_LENGTH] = 0;
		}
		else
			ZeroMemory( m_cMasterNick, NDD_MP_MASTERNICK_LENGTH+1 );
	}

	void		SetCreateDate( DBTIMESTAMP& create_date )
	{
		m_timeCreateDate.uiYear		= create_date.year - 2000;
		m_timeCreateDate.uiMonth	= create_date.month;
		m_timeCreateDate.uiDay		= create_date.day;
		m_timeCreateDate.uiHour		= create_date.hour;
		m_timeCreateDate.uiMinute	= create_date.hour;
		m_timeCreateDate.uiSecond	= create_date.second;

	}
	void		SetLastConDate( DBTIMESTAMP&  con_date )
	{
		m_timeLastCon.uiYear	= con_date.year - 2000;
		m_timeLastCon.uiMonth	= con_date.month;
		m_timeLastCon.uiDay		= con_date.day;
		m_timeLastCon.uiHour	= con_date.hour;
		m_timeLastCon.uiMinute	= con_date.hour;
		m_timeLastCon.uiSecond	= con_date.second;
	}
	void		UpdateLastConDate()		// ���� �ð�( systemtime ���� ������Ʈ )
	{
		time_t cur;
		time(&cur);
		tm * ptm = localtime( &cur );

		m_timeLastCon.uiYear		= ptm->tm_year-100;
		m_timeLastCon.uiMonth		= ptm->tm_mon+1;
		m_timeLastCon.uiDay			= ptm->tm_mday;
		m_timeLastCon.uiHour		= ptm->tm_hour;
		m_timeLastCon.uiMinute		= ptm->tm_min;
		m_timeLastCon.uiSecond		= ptm->tm_sec;
	}
	

	void		AddPupilFromDB( _sCSPK_Pupil& pupil );		// pupil �� ���� �߰��Ѵ�.
	
    int			GetIndex()							{ return m_iIndex; }
	u_int		GetTotalKillCount()					{ return m_uiTotalKillCount; }
	u_int		GetMasterKillCount()				{ return m_uiMasterKillCount; }
	u_char		GetMasterRank()						{ return m_ucMasterRank; }
	u_char		IncreaseMasterRank()
	{
		++m_ucMasterRank;
		m_ucMasterRank = min( m_ucMasterRank, NDD_MP_MAX_MASTER_LEVEL );
		return m_ucMasterRank;
	}

	u_char		GetBonusSkillStep()					{ return m_ucBonusSkillStep; }
	u_char		GetMasterConnectState()				{ return m_ucMasterConnectState; }

	bool		GetApplyEffect()					{ return m_bApplyMasterEffect; }

	u_int		GetTotalPoint()		{ return m_uiMasterTotalPoint;	}
	u_int		GetCurPoint()		{ return m_uiMasterCurPoint;	}
	char *		GetMasterName()		{ return m_cMasterName;			}
	char *		GetMasterNick()		{ return m_cMasterNick;			}

	u_char GetMasterSex()			{ return m_ucMasterSex; }
	u_char GetMasterMoonpa()		{ return m_ucMasterMoonpa; }
	u_char GetMasterClass()			{ return m_ucMasterClass; }
	u_char GetMasterClassGrade()	{ return m_ucMasterGrade; }
	u_char GetMasterLevel()			{ return m_ucMasterLevel; }
	u_char GetMasterCurZone()		{ return m_ucMasterZone; }

	_sND_TimeUnit * GetLastConTime()	{ return &m_timeLastCon; }
	_sND_TimeUnit * GetCreateTime()		{ return &m_timeCreateDate; }

	short GetMoonpaProp()	
	{
		return g_sMoonpaProperty[m_ucMasterMoonpa];
	}


	void MakeBoardInfo( _sPacketMPBoard * board )
	{
		board->ucMasterLevel = GetMasterLevel();
		board->ucMasterRank = GetMasterRank();
		board->ucPupilCount = static_cast<u_char>(GetPupilCount());
		board->ucMoonpa = GetMasterMoonpa();		// 1 - 6
		board->ucClass = GetMasterClass();
		board->ucClassGrade = GetMasterClassGrade();
		board->ucOrg = 0;		// 0,1,2
		board->ucEtc = 0;


		ZeroMemory( board->cMasterName, sizeof(board->cMasterName) );
		strncpy( board->cMasterName, m_cMasterName, en_charac_name_length );

		ZeroMemory( board->cMasterNick, sizeof(board->cMasterNick) );
		strncpy( board->cMasterNick, m_cMasterNick, NDD_MP_MASTERNICK_LENGTH );
	}

	u_char		GetRelationn( int uid )
	{
		// ä�� ���� ��.
		return 1;
	}

	u_char CanAddNewPupil( char * p_name );
	bool AddReservePupil( char * p_name );
	bool CheckNewPupil( char * p_name );
	bool IsFullPupil()
	{
		return ( m_Pupil[0].iPupilIndex && m_Pupil[1].iPupilIndex && m_Pupil[2].iPupilIndex && m_Pupil[3].iPupilIndex );
	}
	
	bool IsEmptyConnetion()	
	{
		// ���� NDD_MAX_PUPIL_COUNT ��ŭ �ݺ����� ��� ������, 4���ۿ� �ȵż� �׳� ��� �˻��Ѵ�. 
		bool ret = false;
		if( false == m_Pupil[0].IsConnected() &&
			false == m_Pupil[1].IsConnected() &&
			false == m_Pupil[2].IsConnected() &&
			false == m_Pupil[3].IsConnected() )
			ret = true;

		return ret;
	}

	int GetPupilCount()
	{
		int count = 0;
		if( m_Pupil[0].iPupilIndex )
			++count;
		if( m_Pupil[1].iPupilIndex )
			++count;
		if( m_Pupil[2].iPupilIndex )
			++count;
		if( m_Pupil[3].iPupilIndex )
			++count;

		return count;
	}

	int GetPupilSlotNumber( int cuid )
	{
		int slot = -1;
		for( int i = 0; i < NDD_MP_MAX_PUPIL_COUNT ; ++i )
		{
			if( cuid == m_Pupil[0].iPupilIndex )
			{
				slot = i;
				break;
			}
		}
		return slot;
	}


	int GetPupilID( char * name )
	{
		int cuid = 0;
		for( int i = 0; i < NDD_MP_MAX_PUPIL_COUNT; ++i )
		{
			if( m_Pupil[i].cPupilName[0] == name[0] &&
				0 == strncmp( m_Pupil[i].cPupilName, name, en_charac_name_length ) )
			{
				cuid = m_Pupil[i].iPupilIndex;
				break;
			}
		}
		return cuid;
	}

	int GetPupilArrIndex( char * name )
	{
		int arr = -1;
		for( int i = 0; i < NDD_MP_MAX_PUPIL_COUNT; ++i )
		{
			if( m_Pupil[i].cPupilName[0] == name[0] &&
				0 == strncmp( m_Pupil[i].cPupilName, name, en_charac_name_length ) )
			{
				arr = i;
				break;
			}
		}
		return arr;
	}
	int GetCharacIndex( char * name )
	{
		int cuid = 0;
		if( m_cMasterName[0] == name[0] && 
			0 == strncmp( m_cMasterName, name, en_charac_name_length ) )
		{
			cuid = GetIndex();
		}
		else
		{
			for( int i = 0; i < NDD_MP_MAX_PUPIL_COUNT; ++i )
			{
				if( m_Pupil[i].cPupilName[0] == name[0] &&
					0 == strncmp( m_Pupil[i].cPupilName, name, en_charac_name_length ) )
				{
					cuid = m_Pupil[i].iPupilIndex;
					break;
				}
			}
		}
		return cuid;
	}

	u_char GetCountConCharac()
	{
		// ���µ� �����Ѵ�.
		u_char count = 0;
		if( m_ucMasterConnectState )
			++count;

		if( m_Pupil[0].IsConnected() )
			++count;
		if( m_Pupil[1].IsConnected() )
			++count;
		if( m_Pupil[2].IsConnected() )
			++count;
		if( m_Pupil[3].IsConnected() )
			++count;

		return count;

	}

	bool IsFullConnetion()
	{
		// ���� NDD_MAX_PUPIL_COUNT ��ŭ �ݺ����� ��� ������, 4���ۿ� �ȵż� �׳� ��� �˻��Ѵ�. 
		bool ret = false;
		if( m_Pupil[0].IsConnected() &&
			m_Pupil[1].IsConnected() &&
			m_Pupil[2].IsConnected() &&
			m_Pupil[3].IsConnected() )
			ret = true;
		return ret;
	}

	void SendMPInfo_Again( char * to_name, u_char zone );
	void FillPacket_MyMasterInfo( CMSG_MP_INFO_MASTER * pInfo );
	void FillPacket_StrikeResult( CMSG_MP_STRIKE_RESULT * pRsult );

	// ���� ���¸� ������Ʈ�Ѵ�. ĳ���Ͱ� ���ٸ�, false ����.
	bool ConnectNotice( int cuid, u_char state, u_char zone );
	void ChangeMasterNick( char * nick );

	bool AddNewPupil( _sCSPK_Pupil& pupil );

	// ���� ���� ������ ��� �Ѹ���.
	void SendToAll( char * data, u_short length );

	// �Ĺ� ���� �Լ�. 
	u_char StrikeReq( char * pupil, COLE_DB_Executer * pGameBD );
	// �ϻ� ���� �Լ�
	u_char LeaveReq( char * pupil, COLE_DB_Executer * pGameBD );

	// ������ ���� ����� �ϴ� �Լ���. ���߿� �ϳ��� ����.
	void RecalcPosition( CMSG_MP_STRIKE_RESULT * pResult );
	void RecalcPosition( CMSG_MP_LEFT_PUPIL * pResult );


	void AddPoint( int cuid, int value );
	void AddKillCount( int cuid, int value );


	// ����Ʈ ���� �Լ�
	u_int SubTotalPoint( u_int sub )
	{
		if( m_uiMasterTotalPoint >= sub )
			m_uiMasterTotalPoint -= sub;
		else
			m_uiMasterTotalPoint = 0;

		if( m_uiMasterCurPoint >= sub )
			m_uiMasterCurPoint -= sub;
		else
			m_uiMasterCurPoint = 0;

		sub = m_uiMasterTotalPoint;

		return sub;
	}


	union 
	{
		int				m_iIndex;				// ���� �ε��� : ������ ĳ���� ����ũ ���̵�.
		CDSMaster *		pNext;					// for SimpleMemPool 
	};


	typedef struct __reserve_pupils
	{
		DWORD			dwApplyTime;			// ��û���� �ð�.
		char			cApplyName[en_charac_name_length+1];
		char			cPad0;
		short			sPad;
	} _sReservePupil;


protected:
	
	u_int			m_uiTotalKillCount;		// �� ųī��Ʈ.
	u_int			m_uiMasterKillCount;		// 

	u_int			m_uiMasterTotalPoint;		// ���� ����Ʈ. ( ������ ������ ���� �ʴ� ���� ��ġ. )
	u_int			m_uiMasterCurPoint;			// ������ ���Ǵ� ����Ʈ. 

	u_char			m_ucMasterRank;			// ���� ���� ( u_short ����, DB ���� short �� �����ϰ�, casting �Ѵ�. )

	u_char			m_ucMasterSex;
	u_char			m_ucMasterMoonpa;
	u_char			m_ucMasterClass;		// 
	u_char			m_ucMasterGrade;		// �������� Ŭ���� �׷��̵�
	u_char			m_ucMasterLevel;		// �������� ����

	u_char			m_ucBonusSkillStep;		// ������ ������ ����, ���ڵ��� �޴� ���ʽ� ��ų ����.
	u_char			m_ucMasterZone;				// ������ �ִ� ����.
	u_char			m_ucMasterConnectState;	// 0 - not connected, 1 - connected

	bool			m_bApplyMasterEffect;		// ���� ���� ������ ����ǰ� �ִ°�.

	_sReservePupil	m_ReserveApplyPupil[NDD_MP_MAX_RESERVE_PUPIL];		// �ִ� 5����� �ߺ������ϵ��� �Ѵ�.

	char			m_cMasterName[en_charac_name_length+1];
	u_char			m_ucMasterNickLength;		// ��ȣ�� ����( byte )
	char			m_cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];

	_sND_TimeUnit	m_timeCreateDate;			// ���� ������ ������ ��¥.
	_sND_TimeUnit	m_timeLastCon;				// ������ ������ �α��� �ð�.

	_sCSPK_Pupil	m_Pupil[NDD_MP_MAX_PUPIL_COUNT];		// ���� ����.
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct __master_board_page;

// ���� ����â�� ǥ�õǴ� ���� ���� ����ü.
// ������ �ε����� ����, ĳ���� �ε����� ����Ѵ�.
struct __master_board_unit
{
	u_int							uiTotalPoint;
	__master_board_page *			pParentPage;
	_sPacketMPBoard					BoardInfo;			// ���� ������ �����Ѵ�.


	__master_board_unit()
	{
		uiTotalPoint = 0;
		pParentPage = NULL;
		ZeroMemory( &BoardInfo, sizeof(BoardInfo) );
	}

	void Print()
	{
		printf( "N<%s>,Rank(%u),Point(%u)", BoardInfo.cMasterName, BoardInfo.ucMasterRank, uiTotalPoint );
	}

	void Set( CDSMaster * p )
	{
		if( p )
		{
			p->MakeBoardInfo( &BoardInfo );
			uiTotalPoint = p->GetTotalPoint();
		}
	}
	void SetParent( __master_board_page * p )	{ pParentPage = p; }
	__master_board_page * GetParentPage()	{ return pParentPage; }

	__master_board_unit( const __master_board_unit& unit )
	{
		uiTotalPoint = unit.uiTotalPoint;
		CopyMemory( &BoardInfo, &unit.BoardInfo, sizeof(BoardInfo) );
	}

	int Compare( u_int point, u_char rank )
	{
		int result = 0;
		u_int o_p = uiTotalPoint;
		u_char o_r = BoardInfo.ucMasterRank;

		if( rank > o_r )
			result = -1;		// ��������.
		else if( rank < o_r )
			result = 1;
		else 
		{
			if( point > o_p )
				result = -1;
			else if( point < o_p )
				result = 1;
		}
		return result;
	}
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
class _MPBoardList;

struct __master_board_page
{
	u_int					m_uiCount;	//������ ����.
	_node<__master_board_unit> *	m_pFirst;
	_MPBoardList *					m_pParent;

	__master_board_page()
		: m_uiCount(0), m_pFirst(NULL)
	{
		m_pParent = NULL;
	}

	u_char		GetFirstRank()
	{
		u_char rank = 0;
		if( m_pFirst )
			rank = m_pFirst->node.BoardInfo.ucMasterRank;
		return rank;
	}
	u_int		GetFirstPoint()
	{
		u_int point = 0;
		if( m_pFirst )
			point = m_pFirst->node.uiTotalPoint;
		return point;
	}
	int Compare( u_int point, u_char rank )
	{
		// ������, �ڷ� ���� �ϱ� ����, 0 || 1 �� ���, 1�� ����.
		int r = -1;		// �⺻�� ��.
		if( m_pFirst )
		{
			int com = m_pFirst->node.Compare( point, rank );
			if( com < 0 )
				r = -1;
			else 
				r = 1;
		}
		return r;
	}

	bool AddBoard( CDSMaster * p );
	void FreeNode( _node<__master_board_unit> * pNode )
	{
		if( pNode )
			m_pBoardPool->add_free_node( pNode );
	}

	int GetPage()				{ return m_iPage; }
	void SetPage( int page )	{ m_iPage = page; }

	u_int GetCount()			{ return m_uiCount; }
	void SetCount( u_int count ){ m_uiCount = count; }
	void DecreaseCount()		{ --m_uiCount; }
	void IncreaseCount()		{ ++m_uiCount; }

	void Reset()
	{
		m_uiCount = 0;
		m_pFirst = NULL;
	}

public:
	static bool CreateBoardUnit( int count );
protected:
	int						m_iPage;	//������ ����.
	static NDQueueMemoryPool<__master_board_unit>	* m_pBoardPool;
};



//////////////////////////////////////////////////////////////////////////
// _MPBoardList
//////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)
#include <vector>
#include <map>
using namespace std;

class _MPBoardList
{
public:
	_MPBoardList()
		: m_iTotalPage(0), m_iTotalBoardUnit(0) 
	{
		m_ListProperty = 0;
	}

	int GetProperty()					{ return m_ListProperty; }

	// ������. ������ ������ ����, ���� �������� ������ �Է�.
	bool CreateBoardList( int prop, int page_res, int board_res );



public:
	// 
	void AddMPBoard( CDSMaster * pMaster );
	void DelMPBoard( int master_index );
	void UpdateMPBoardNick( int master_index, char * nick );


	bool GetPageInfo( int page, CMSG_MP_BOARD_PAGE_INFO * pInfo );
	void GetInitialInfo( CMSG_MP_BOARD_INITIAL_OPEN * pInfo );

	typedef map<int,_node<__master_board_unit>* >		TPNodeMap;

	bool InsertNodeMap( int index, _node<__master_board_unit> * n ) 
	{
		pair< TPNodeMap::iterator, bool> ret_pair = m_nodeMap.insert( TPNodeMap::value_type( index, n ) );
		return ret_pair.second;
	}



	void PrintList();

	int GetPageCount()		{ return m_iTotalPage; }
	int GetUnitCount()		{ return m_iTotalBoardUnit; }


protected:
	// �� á�°�.
	bool IsFull()
	{
		return ( m_iTotalBoardUnit >= (static_cast<int>(m_pages.size())*NDD_MP_AVR_ITEMCOUNT_PER_BOARD ) );
	}

	void IncreaseCount()	{ ++m_iTotalBoardUnit; }
	void DecreaseCount()	{ --m_iTotalBoardUnit; }

	// ������ �������� �߰��ϰ�, �������� �����Ѵ�.
	__master_board_page * InsertPage();

	// start_page ���� �ڷ� ���鼭 ������ 10�� ���ܷ� �����.
	void ArrangePage( int start_page, bool Insert_Delete );

	


protected:
	int		m_ListProperty;		// -1 : �浵, 0 - all, 1 - �鵵 �Խ���.
	int		m_iTotalPage;		// ������� ��ü ������ ��.
	int		m_iTotalBoardUnit;	// 

	vector<__master_board_page>		m_pages;	// �ʱ⿡ 100�� ������Ų��.

	map<int,_node<__master_board_unit>* >	m_nodeMap;

	CRITICAL_SECTION			m_csLock;
};




class COLE_DB_Executer;
// �����͸� �����ϸ� �ȴ�. : DS 
class CDSMasterSet
{
public:
	CDSMasterSet()
		: m_pGameDB(NULL)
	{}
	virtual ~CDSMasterSet()
	{}

	bool CreateMasterPupilManager( COLE_DB_Executer * pDB, int init_master, int init_page, int init_master_board_count );
	void DestroyManager();

	// ĳ���� ���� �����, �ڽ��� ���� ������ ��û�ϴ� �Լ�.
	CDSMaster *		FindAndMakeMasterIndex( int cuid );
	// �׳� ã��
	CDSMaster *		FindMaster( int master_uid );
	CDSMaster *		AddMasterInfo_FromDB( int master_uid );
	void			AddToMasterMap( int master_uid, CDSMaster * p )
	{
		m_MasterMap.insert( map<int, CDSMaster*>::value_type(master_uid, p) );
	}
	


	inline void LockMasterMap()			{ EnterCriticalSection(&m_csMasterMap); }
	inline void UnlockMasterMap()		{ LeaveCriticalSection(&m_csMasterMap); }

	void DSProc_MasterInfoReq( CMSG_MP_INFO_REQ * pMP, _h_DS_Com * pGS );
	void DSProc_AddPoint( CMSG_MP_ADD_POINT * pMP );	
	void DSProc_AddKillCount( CMSG_MP_ADD_KILL_COUNT * pMP );	
	void DSProc_CreateMPReq( CMSG_MP_CREATE_MP_REQ * pMP, _C_CHARAC * pCharac, _h_DS_Com * pGS );
	void DSProc_NoticeCon( CMSG_MP_NOTICE_CONNECTION * pMP );
	void DSProc_ChangeMasterNick( CMSG_MP_CHANGE_MASTER_NICK_REQ * pMP );
	void DSProc_GroupChat( CMSG_MP_CHAT * pMP );
	// ĳ���Ϳ��� ���� ��û�� �޾Ҵ�. ������ ã�� ������ ��.
	void DSProc_ApplyForPupil( CMSG_MP_GD_APPLY_FOR_PUPIL * pMP, _h_DS_Com * pGS );
	void DSProc_ApplyForPupilError( CMSG_MP_APPLY_FOR_PUPIL_ERROR * pMP );
	void DSProc_ApplyAnswerYes( CMSG_MP_ANSWER_YES * pMP, _h_DS_Com * pMasterGS );
	void DSProc_ApplyAnswerNo( CMSG_MP_ANSWER_APPLY_TO_PUPIL_NO * pMP );
	void DSProc_ApplyError( CMSG_MP_APPLY_ERROR * pMP );
	void SendApplyError( char * name, u_char code, short gs, _h_DS_Com * pGS );
	
	//////////////////////////////////////////////////////////////////////////
	// �Ĺ�/�����߹�/�ϻ�.
	enum
	{
		en_mp_leave_reason_none = 0,
		en_mp_leave_reason_strike =1,
		en_mp_leave_reason_ban = 2,
		en_mp_leave_reason_leave = 3,
	};

	void DSProc_StrikeReq( CMSG_MP_STRIKE_PUPIL_REQ * pMP, _h_DS_Com * pGS );
	void DSProc_LeaveReq( CMSG_MP_LEAVE_REQ * pMP, _h_DS_Com * pDSComm );

	bool DBProc_StrikePupil( int strike_pupil );
	bool DBProc_BanPupil( int ban_pupil );
	bool DBProc_LeavePupil( int leave_pupil );

	void SendStrikeError( int master, char * pupil_name, u_char code, _h_DS_Com * pGS );
	void SendLeaveError( int master, char * pupil_name, u_char code, _h_DS_Com * pGS );

	void ShowMaster( int master );

protected:
	NDSimpleMemPool<CDSMaster>			m_MasterPool;
	map<int, CDSMaster *>				m_MasterMap;		// ���� ���� ����.
	COLE_DB_Executer *					m_pGameDB;			// 
	CRITICAL_SECTION					m_csMasterMap;

//////////////////////////////////////////////////////////////////////////
//  ���� �Խ��� ����.
//////////////////////////////////////////////////////////////////////////
public:
	bool CreateMPBoard( int page_res, int board_res );		// ������. �Լ� ���ο��� ������ ��������.

	// ���� ������ �߰�/����. �����ϸ� �߰�, �������� �����Ѵ�.
	bool AddBoard( CDSMaster * pMaster );
	void DelBoard( CDSMaster * pMaster );
	void BoardUpdateNick( CDSMaster * pMaster );

	// ����/����. ������ �Լ� ���ο��� �����Ѵ�.��
	bool GetBoardInfo( int page, int prop, CMSG_MP_BOARD_PAGE_INFO * pInfo );


	void PrintBoard();
	void PrintSimpleInfo();

	// en_dg_mp_mode_board_initial_req:
	void DSProc_BoardInitialReq( CMSG_MP_BOARD_INITIAL_REQ * pMP, _h_DS_Com * pDSComm );
	// en_dg_mp_mode_board_req_page:
	void DSProc_BoardPageReq( CMSG_MP_BOARD_REQ_PAGE * pMP, _h_DS_Com * pDSComm );

protected:
	_MPBoardList			m_total_board;		// �鵵/�浵. ������ ��û�� ���δ�.
	_MPBoardList			m_white_board;		// �鵵 ����Ʈ.
	_MPBoardList			m_black_board;		// �浵 ����Ʈ.



	//////////////////////////////////////////////////////////////////////////
	// ���� ����.
	//////////////////////////////////////////////////////////////////////////
public:
	inline static u_int GetBaseKillCount()			{ return m_uiBaseKillCount; }
	inline static u_int GetExtendKillCount()		{ return m_uiExtendKillCount; }
	inline static u_int GetMaxMasterTotalPoint()	{ return m_uiMaxMasterTotalPoint; }
	inline static u_int GetMaxPupilTotalPoint()		{ return m_uiMaxPupilTotalPoint; }



protected:
	static void SetBaseKillCount( u_int count )		{ m_uiBaseKillCount = count; }
	static void SetExtendKillCount( u_int count )	{ m_uiExtendKillCount = count; }

	static u_int m_uiBaseKillCount;
	static u_int m_uiExtendKillCount;

	static u_int m_uiMaxMasterTotalPoint;
	static u_int m_uiMaxPupilTotalPoint;
};



#endif

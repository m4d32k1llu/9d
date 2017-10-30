#pragma once
#include <map>


using namespace std ;


#define _EMPTY_UNIT_SLOT_			"\0\0\0\0\0\0\0\0\0\0\0\0\0"
#define _ORGANIZATION_INIT_SIZE_	10000
#define _MAX_OR_UNIT_NUM	100
#define _SIZE_OF_OR_MEM_LIST_DATA	sizeof(_Unit) * _MAX_OR_UNIT_NUM
#define _SIZE_OF_OR_OUTTIME_LIST_DATA	sizeof(_OutTime) * _MAX_OR_UNIT_NUM

// ������ ���� ����..

#pragma pack( push, enter_organization )
#pragma pack(1)


class _Unit
{
public:	
	char			m_cName[13] ;	
	bool			m_bOnline ;
	char			m_cClass ;		
	enum{
		_UNIT_CLASS_DEFAULT_UNIT_,			// �⺻ ������
		_UNIT_CLASS_SPECIAL_UNIT_,			// ��� ������
		_UNIT_CALSS_CAPTAIN_UNIT_,			// ���� ������..
	} ;
	short			m_sInnerLevel ;
	char			m_cZone ;


	_Unit & _Unit::operator=( const _Unit& rhs ) 
	{
		if( this == &rhs ) { return *this ; }

		memcpy( this, &rhs, sizeof(_Unit) ) ;
		return *this ;
	}

	inline void reset() { memset(this, 0, sizeof(_Unit) ) ; }
};


struct _OutTime
{
	u_char	year ;
	u_char	month ;
	u_char	day ;
	u_char	h ;
	u_char  m ;
	u_char	s ;
	
	_OutTime & _OutTime::operator=( const _OutTime & rhs )
	{
		if( this == &rhs ) { return *this ; }
		memcpy( this, &rhs, sizeof( _OutTime ) ) ;
		return *this ;
	}

} ;


// ������ �⺻ ����Ÿ��.. ��Ŷ���� ������ �̷� ����ü ������ ������ ó���ϱ� ���ϰڴ�..
#define _OR_NOTICE_SIZE		300
class _OrganizationBase
{
public:
	int				m_iIndex ;
	char			m_cOrName[en_or_name_length+1] ;			// ���� �̸�
	char			m_cProperty ;			// ���� �Ӽ�(���� | ��ȸ)
	char			m_cBaseParty ;			// �Ҽ� ����
	u_int			m_uiExperience ;		// ���� ����ġ
	u_int			m_uiContribution ;		// �� �⿩��~�� ����.<-������ ���� �⿩�� : �⿩�� �Ҷ� �� ã�ƺ��� �� �⿩���� ���Ž�Ű�� - `07.07.24
	u_short			m_usCurMemberNum ;		// ���� �ο�	
	char			m_cCaptainName[13] ;
#ifdef	_ORG_PROMOTION_  //_OrganizationBase �� ���� �߰�
	char			m_org_level;			//���� ���� ����(1 ~ 5)
	//u_int			m_contribution;			// �� �⿩�� �� ���ı⿩���� �� �⿩���� �ٲ� �׽�Ʈ�� ����	
	DWORD			m_dwBloodPoint_Buf_Time;// ������ ����Ͽ� ������ ���� �� �ִ� �ð� 
	_sND_TimeUnit	m_ally_withdraw_time;	// ȸ���� ���� �ð� �� �ð����� ���� 4���� ������ �Ѵ�.
#endif//_ORG_PROMOTION_ 

	_OrganizationBase & _OrganizationBase::operator=( const _OrganizationBase& rhs ) 
	{
		try
		{
			if( this == &rhs ) { return *this ; }

			m_iIndex = rhs.m_iIndex ;
			memcpy( m_cOrName, rhs.m_cOrName, sizeof(m_cOrName) ) ;	// ���� �̸�
			m_cProperty = rhs.m_cProperty ;							// ���� �Ӽ�(���� | ��ȸ)
			m_cBaseParty = rhs.m_cBaseParty ;						// �Ҽ� ����
			m_uiExperience = rhs.m_uiExperience ;					// ���� ����ġ
			m_uiContribution = rhs.m_uiContribution ;				// ������ ���� �⿩��
			m_usCurMemberNum = rhs.m_usCurMemberNum ;				// ���� �ο�	
			memcpy( m_cCaptainName, rhs.m_cCaptainName, sizeof( m_cCaptainName ) ) ;
#ifdef	_ORG_PROMOTION_ 
			m_iOrg_level			 = rhs.miOrg_level;
			//m_contribution			 = rhs.m_contribution;
			m_dwBloodPoint_Buf_Time	 = rhs.m_dwBloodPoint_Buf_Time;
			memcpy(&m_ally_withdraw_time,rhs.m_ally_disband_time,sizeof(_sND_TimeUnit));
#endif//_ORG_PROMOTION_ 
		}
		catch(...)
		{
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}
		
		return *this ;
	}	
};

class _Organization : public _OrganizationBase
{
public :
	_Unit				m_MemberList[_MAX_OR_UNIT_NUM] ;			// ���� ���..

	char				m_cNotice[_OR_NOTICE_SIZE] ;
	_OutTime			m_outTime[_MAX_OR_UNIT_NUM] ;

#ifdef _PD_ALLY_ORG_STEP_1_
	int					m_iAllyIndex;
#endif

	inline void SetOutTime( const int slot, const _OutTime& outTime )
	{
		if( slot >= 0 && slot < _MAX_OR_UNIT_NUM )
			m_outTime[slot] = outTime ;
	}


	_Organization & _Organization::operator=( const _Organization& rhs ) 
	{
		try
		{
			if( this == &rhs ) { return *this ; }

			_OrganizationBase::operator = ( rhs ) ;

			memcpy( m_MemberList, rhs.m_MemberList, sizeof( m_MemberList ) ) ;
			memcpy( m_cNotice, rhs.m_cNotice, sizeof( m_cNotice ) ) ;
			memcpy( m_outTime, rhs.m_outTime, sizeof( m_outTime ) ) ;

#ifdef _PD_ALLY_ORG_STEP_1_
			m_iAllyIndex = rhs.m_iAllyIndex;
#endif
			
		}
		catch(...)
		{
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}
		
		return *this ;
	}

	

	inline _Unit * FindUnit( const char * name, OUT int * pSlotNum = NULL ) 
	{		
		_Unit * pUnit = NULL ;
		try
		{
			for( int i = 0 ; i < _MAX_OR_UNIT_NUM ; i++ )
			{
				if( m_MemberList[i].m_cName[0] == name[0] )
				{
					if( strcmp( m_MemberList[i].m_cName, name ) == 0 )
					{
						pUnit = &m_MemberList[i] ;
						if( pSlotNum )
						{
							*pSlotNum = i ;
						}
						break ;
					}					
				}			
			}
		}
		catch(...)
		{
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}	

		return pUnit ;
	}
	#ifdef	_ORG_PROMOTION_
	//��ü�ð��� ����ϱ� ���� ������ ã�ƶ�~!!!
	inline _Unit * FindCaptain()
	{
		_Unit * pUnit = NULL ;
		try
		{
			for( int i = 0 ; i < _MAX_OR_UNIT_NUM ; i++ )
			{
				if( m_MemberList[i].m_cClass == _Unit::_UNIT_CALSS_CAPTAIN_UNIT_ )
				{
					pUnit = &m_MemberList[i] ;
					if( pSlotNum )
					{
						*pSlotNum = i ;
					}
					break ;
					}					
				}			
			}
		}
		catch(...)
		{
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}	

		return pUnit ;
	}


	#endif//_ORG_PROMOTION_

};

//en_ds_ally_info,					// 4 �� ���� ��û�� ����.
struct __ally_client_info
{
	int				iOrIndex;
	char			cOrName[en_or_name_length+1];
};


#pragma pack( pop, enter_organization )


class COLE_DB_Executer ;
class COrganizationManager 
{
private :	
	typedef map<int,_Organization*> ORMap ;
	size_t							m_InitSize ;
	_Organization *					m_pOrganization ;

	COLE_DB_Executer *				m_pGameDB ;

	ORMap							m_MapOR ;
	CSAFE_Vector<_Organization* >	m_ORPool ;	
	CTraceAbleLock					m_or_cs ;

#define or_lock()					m_or_cs.tlock( __FILE__, __LINE__ ) ;
#define or_unlock()					m_or_cs.tunlock( __FILE__, __LINE__ ) ;
	
	
public :
	COrganizationManager( size_t size ) ;
	~COrganizationManager() ;

	// Manager part
	bool ReadORDataFromDB( COLE_DB_Executer * GameDB ) ;
#ifdef _PD_ALLY_ORG_STEP_1_
	void CheckInitialAlly( COLE_DB_Executer * GameDB );
#endif
	const int CreateOR( IN OUT  _Organization & or ) ;			// ���ο� Organization�� �����ϰ� OR Index ��ȯ
	bool InsertOR( const _Organization & or ) ;			// �ʿ� ���Ը� �Ѵ�.
	bool DeleteOR( const int index ) ;							// Index�� ã�Ƽ� ����	
	_Organization * GetORInfo( const int index );

	// OR Part
	bool PutORUnit( const int index, const _Unit & unit ) ;	
	int PopORUnit( const int index, const char * name ) ;	// -1 : �Ļ�, 0: ����, 1:����

	bool UpdateORUnit( const int index, const _Unit & unit, const _OutTime & outTime ) ;
	bool SetUnitClass( const int index, const char * name, const char Class ) ;
	bool SetORExp( const  int index, const u_int exp) ;	
	bool ExchangeCaptain( const int index, const char * name) ;
	bool BaseUpdate( const  int index, const _OrganizationBase & or_base  ) ;	
	bool DBUpdate( const _Organization * pOr ) ;	
	inline _Organization * FindOrganization( const int index ) 
	{
		_Organization * pOR = NULL ;
		or_lock() ;
		try
		{
			ORMap::iterator OrMapItr =  m_MapOR.find( index ) ;
			if( OrMapItr != m_MapOR.end() )
			{
				pOR = OrMapItr->second ;
			}
		}
		catch (...)
		{
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}		
		or_unlock() ;

		return pOR ;
	}

	inline _Unit * FindUnit( _Organization * const pOr, const char * name, OUT int * pSlotNum = NULL ) 
	{		
		
		if( NULL == pOr ) return NULL ;
		_Unit * pUnit = pOr->FindUnit( name, pSlotNum ) ;		
		return pUnit ;
	}	

	inline bool IsUsingName( const char * name )
	{
		bool bRet = false ;
		or_lock() ;
		try
		{
			ORMap::iterator OrMapItr =  m_MapOR.begin() ;	
			_Organization * pOR ;
			while ( OrMapItr != m_MapOR.end() )
			{		
				pOR = OrMapItr->second ;
				if( name[0] == pOR->m_cOrName[0] )
				{
					if( strcmp( name, pOR->m_cOrName) == 0 )
					{
						bRet = true ;
						break ;
					}
				}
				++OrMapItr ;
			}
		}
		catch(...)
		{
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}		
		or_unlock() ;

		return bRet ;
	}

	void ShowAllOrganization();
};
	

#ifdef _PD_ALLY_ORG_STEP_1_

#include "..\Global\tcp_packets_party.h"

struct __org_state_in_ally
{
	int					iOrIndex;		// ������ �ε���.
	_Organization *		pOrg;			// ������ �ִٸ�, �� ������ ���µ�, ��¥�� ������ Ȯ������ �ʾ���.

	__org_state_in_ally()
		: iOrIndex(0), pOrg(NULL)
	{}
	__org_state_in_ally( int idx, _Organization * p )
		: iOrIndex(idx), pOrg(p)
	{}
};

class CAllyOrganization 
{
public:
	enum	// ������ ���� : m_iAllyState
	{
		en_ally_state_not_used=0,		// ������ �ʴ� ����.
		en_ally_state_ready_ally,		// �Ἲ�� ���� �غ����� ����
		en_ally_state_made_ally,		// �Ἲ�� �Ϸ�� ����.
	};

public:
	CAllyOrganization()
	{
		reset_ally( NULL );
		m_bInPool = false;
	}

	void reset_ally( COLE_DB_Executer * pDB );

	void copy_set( CAllyOrganization & refAlly )
	{
		set_index( refAlly.get_index() );
		CopyMemory( m_orgArrayInAlly, refAlly.m_orgArrayInAlly, sizeof(m_orgArrayInAlly) );
		m_sProperty = refAlly.get_property();
		CopyMemory( m_cMasterName, refAlly.m_cMasterName, sizeof(m_cMasterName) );
	}

	// �ڽ��� ���¸� üũ�Ͽ�, �߸��� ������ �ִٸ� �����ϰų�, 
	// ������ ��ü�� �� �ִ�.
	bool check_info( COrganizationManager * pOrManager );
	//////////////////////////////////////////////////////////////////////////
	inline int get_index()					{ return m_iIndex; }
	inline void set_index( int index )		{ m_iIndex = index; }
	inline short get_property()				{ return m_sProperty; }
	inline void set_property( short pr )	{ m_sProperty = pr; }

	inline void set_master_name( char * name )	
	{
		CopyMemory( m_cMasterName, name, en_charac_name_length+1 );
		m_cMasterName[en_charac_name_length] = 0;
	}

	int get_org_index( int slot )
	{
		return m_orgArrayInAlly[slot].iOrIndex;;
	}

	inline char * get_master_name()	{ return m_cMasterName; }

	// ���Ե� �������� ���� �����Ѵ�.
	int	get_org_count()
	{
		int n = 0;
		if( m_orgArrayInAlly[0].iOrIndex )		++n;
		if( m_orgArrayInAlly[1].iOrIndex )		++n;
		if( m_orgArrayInAlly[2].iOrIndex )		++n;
		if( m_orgArrayInAlly[3].iOrIndex )		++n;
		if( m_orgArrayInAlly[4].iOrIndex )		++n;
		return n;		
	}
	bool org_is_in_ally( int org )
	{
		if( org == m_orgArrayInAlly[0].iOrIndex ||
			org == m_orgArrayInAlly[1].iOrIndex ||
            org == m_orgArrayInAlly[2].iOrIndex ||
            org == m_orgArrayInAlly[3].iOrIndex ||
            org == m_orgArrayInAlly[4].iOrIndex )
			return true;
		else 
			return false;
	}

	// �� ������ �� �ִ°�.
	bool can_add_org( int org )
	{
		if( get_org_count() < _NDD_MAX_ORG_IN_ALLY_ && false == org_is_in_ally( org ) )
			return true;
		else 
			return false;
	}

	bool leave_org( int org );		// true : ��ü, false �״��.

	u_char add_org_info( __org_state_in_ally& refOrg )
	{
		u_char slot = 1;
		for( ; slot < _NDD_MAX_ORG_IN_ALLY_ ; ++slot )
		{
			if( 0 == m_orgArrayInAlly[slot].iOrIndex )
				break;
		}

		if( slot < _NDD_MAX_ORG_IN_ALLY_ )
		{
			CopyMemory( &m_orgArrayInAlly[slot], &refOrg, sizeof(__org_state_in_ally) );
		}

		return slot;
	}

	void db_update( COLE_DB_Executer * pDB );

	void get_org_info( u_char slot, __ally_client_info * pinfo  )
	{
		if( slot < _NDD_MAX_ORG_IN_ALLY_ )
		{
			pinfo->iOrIndex = m_orgArrayInAlly[slot].iOrIndex;
			if( m_orgArrayInAlly[slot].pOrg )
				strncpy( pinfo->cOrName, m_orgArrayInAlly[slot].pOrg->m_cOrName, en_or_name_length );
			else
				ZeroMemory( pinfo->cOrName, sizeof(pinfo->cOrName) );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// �ʱ�ȭ�� �Ҹ��� �Լ���.
	// org_info �� �����Ѵ�.
	void set_org_info( int slot, __org_state_in_ally & refOrg )
	{
		if( slot < _NDD_MAX_ORG_IN_ALLY_ )
		{
			CopyMemory( &m_orgArrayInAlly[slot], &refOrg, sizeof(__org_state_in_ally) );
		}
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// �ش� ������ ���� ������ �����ش�.
	void send_ally_info( _h_DS_Com * pCom, char * toName );
	void fill_answer_packet( SMSG_ALLY_ANSWER_INFO& info );

protected:
	int					m_iIndex;
	__org_state_in_ally	m_orgArrayInAlly[_NDD_MAX_ORG_IN_ALLY_];
	short				m_sProperty;		//
	char				m_cMasterName[13];

public:
	friend class CAllyManager;

	//////////////////////////////////////////////////////////////////////////
	// Pool ���� �Լ�.
	//////////////////////////////////////////////////////////////////////////
public:
	// �Ʒ� 3���� �Լ��� Manager �� Ǯ ������ ���δ�. 
	bool is_in_pool()		{ return m_bInPool; }
	void set_in_pool()		{ m_bInPool = true; }
	void reset_in_pool()	{ m_bInPool = false; }

protected:
	bool		m_bInPool;
};


// ó�� Ǯ�� ������ �� 50�� ������ ����. �������� ����ϴ�.
class CAllyManager
{
public:
	CAllyManager( int poolCount )
		: m_poolTotalCount(0)
	{
		if( FALSE == InitializeCriticalSectionAndSpinCount( &m_csPool, 0x80000100 ) )
			throw "(AllyManager) Create Critical Section Fail";

		if( FALSE == InitializeCriticalSectionAndSpinCount( &m_csMap, 0x80000100 ) )
			throw "(AllyManager) Create Critical Section Fail";

		CAllyOrganization * pAlly = NULL;

		for( int i = 0 ; i < poolCount ; ++i )
		{
			pAlly = new CAllyOrganization;
			if( pAlly )
			{
				push( pAlly );
				++m_poolTotalCount;
			}
		}

		m_pGameDB = NULL;
	}

	~CAllyManager()
	{
		LockPool();

		CAllyOrganization * p = NULL;
		list<CAllyOrganization*>::iterator itr = m_AllyPool.begin();
		while( itr != m_AllyPool.end() )
		{
			try
			{
				p = *itr;
				++itr;
				delete p;
			}
			catch(...)
			{
			}
		}

		UnlockPool();

		DeleteCriticalSection( &m_csPool );
	}
	//////////////////////////////////////////////////////////////////////////
	// read_ally_info( COLE_DB_Executer );
	//////////////////////////////////////////////////////////////////////////
	// �ʱ⿡�� DB���� ���� ������ ���� ��, ( read_ally_info() )
	// DB �����͸� �⺻���� ���� ������ �����ϰ�. ( insert_ally
	// �� ������ ����, �߸��� ���� ������ ���ܽ�Ų��.( check_initial_status() )
	bool read_ally_info( COrganizationManager * pOrManager, COLE_DB_Executer * pDB );
	void copy_and_insert_ally( CAllyOrganization& refAlly );
	void check_initial_status( COrganizationManager * pOrManager );
	void set_db_exe( COLE_DB_Executer * pDB )	{ m_pGameDB = pDB; }

	//////////////////////////////////////////////////////////////////////////
	// Packet ó�� �Լ�
	//////////////////////////////////////////////////////////////////////////
	void pk_request_create( SMSG_ALLY_REQUEST_CREATE * pCreate, _h_DS_Com * pCom );
	void pk_request_info( SMSG_ALLY_REQUEST_INFO * pInfo, _h_DS_Com * pCom );
	void pk_request_add_org( SMSG_ALLY_REQUEST_ADD_ORG * pAddOrg, _h_DS_Com * pCom );
	void pk_request_leave( SMSG_ALLY_REQUEST_LEAVE * pLeave, _h_DS_Com * pCom );
	void pk_request_breakup( SMSG_ALLY_REQUEST_BREAKUP * pBreakup, _h_DS_Com * pCom );
	void pk_request_banish( SMSG_ALLY_REQUEST_BANISH * pBanish, _h_DS_Com * pCom );
#ifdef _PD_CASTLE_STEP_2_
	void pk_request_all_ally( _h_DS_Com * pCom );
#endif
	//////////////////////////////////////////////////////////////////////////

	void show_all_ally();

public:
	inline void LockMap()		{ EnterCriticalSection(&m_csMap); }
	inline void UnlockMap()		{ LeaveCriticalSection(&m_csMap); }

	CAllyOrganization * find_ally( int index )
	{
		CAllyOrganization * p = NULL;
		LockMap();
		try
		{
			map<int, CAllyOrganization *>::iterator itr = m_mapAlly.find( index );
			if( itr != m_mapAlly.end() )
				p = itr->second;
			else
				p = NULL;
		}
		catch( ... )
		{
		}
		UnlockMap();
		return p;
	}

	int add_ally( int index, CAllyOrganization * p )
	{
		LockMap();
		if( index && p )
			m_mapAlly[index] = p;
		UnlockMap();

		return index;
	}

	void del_ally( int index )
	{
		LockMap();
		try
		{
			m_mapAlly.erase( index );
		}
		catch ( ... ) 
		{
		}
		UnlockMap();
	}
	

protected:
	CRITICAL_SECTION				m_csMap;

	map<int, CAllyOrganization *>	m_mapAlly;

	COLE_DB_Executer *				m_pGameDB;


//////////////////////////////////////////////////////////////////////////
public:
	//////////////////////////////////////////////////////////////////////////
	/// Pool //////////////////////////////////////////////////////////////////////////
	inline void LockPool()		{ EnterCriticalSection(&m_csPool); }
	inline void UnlockPool()	{ LeaveCriticalSection(&m_csPool); }

	void push( CAllyOrganization * p )
	{
		LockPool();
		if( !p->is_in_pool() )
		{
			p->set_in_pool();
			m_AllyPool.push_back( p );
		}
		UnlockPool();
	}

	CAllyOrganization * pop()
	{
		CAllyOrganization * p = NULL;
		LockPool();

		if( m_AllyPool.empty() )
		{
			p = new CAllyOrganization;
			++m_poolTotalCount;
		}
		else
		{
			p = m_AllyPool.front();
			m_AllyPool.pop_front();
		}

		if( p )
			p->reset_in_pool();

		UnlockPool();

		return p;
	}

	int get_pool_count()		{ return m_poolTotalCount; }
	int get_using_pool_count()	{ return m_poolTotalCount - (int)m_AllyPool.size(); }
	void print_status()	
	{
		printf( "Ally Pool : Total = %d, Using = %d\n", get_pool_count(), get_using_pool_count() );
	}

protected:
	list<CAllyOrganization*>	m_AllyPool;
	CRITICAL_SECTION			m_csPool;
	int							m_poolTotalCount;
	// Pool End //////////////////////////////////////////////////////////////////////////

};


#endif
#pragma once

#pragma warning(disable:4786)

//#define _PERMISSIBLE_MAX_USER				12000	// DS_SERVER ���� ����ϴ� �ִ� ������..(�ִ� ĳ���̶� �ٸ�)


#include <string>
#include <queue>
#include <map>
#include <list>
using namespace std;

class _Member ;




namespace _ns_party_
{	

	/* =====================================================================================
	��Ƽ Ÿ�̸� �޽���
	===================================================================================== */
	enum {
		_TM_PARTY_MOVE_READY_		//���� �̵� �غ���..
	} ;


	// ���� �÷���..
	enum
	{
		_FLAG_PARTY_SEND_EVERYONE_ = 0,
		_FLAG_PARTY_SEND_BOSS_READY,
		_FLAG_PARTY_SEND_NOT_BOSS,
		_FLAG_PARTY_SEND_CAPTAIN,
	};

	// ����ġ �й� ���
	enum
	{
		_EXP_SHARE_TYPE_PRIVATE = 0,
		_EXP_SHARE_TYPE_PUBLIC,
	};

	// ������ �й���
	enum
	{
		_ITEM_SHARE_TYPE_SOLO = 0,	
		_ITEM_SHARE_TYPE_RB,
		_ITEM_SHARE_TYPE_RANDOM,
	};

	// ������ �й迡��.. ��� ������.. ( 'WhoIsItemGetter( char cWhere )' �Լ����� �÷��׷� ����)
	enum
	{
		_FILED_DROP = 0 ,
		_BOSSROOM_DROP,
	};
}

class _Party 
{

protected :
	char			m_cServerNumList[MAX_SERVER_COUNT] ;		// ���� ����Ʈ�� �ӽ÷� ����, ���� '&' �� ���� (��� ������ ��Ƽ���� �����Ҷ� ����)	
	_Member *		m_pMember[MAX_PARTY_MEMBER] ;				// ��Ƽ�� ���� ���� * 9 ��(Max)
	char			m_cExpProperty ;				// ����ġ �й� ��� 0: 1:
	char			m_cItemProperty ;				// ������ �й� ��� 
	short			m_sPartyIndex ;
	u_short			m_usCaptainUID ;
	char			m_cMemberCount ;				// ��Ƽ���� ��� �ο���..


	short			m_sClassProp ;		// ���� �Ӽ�(������)


	CTraceAbleLock m_csPartyQueue ;
	
public :

	inline void party_lock( char * f, int l )
	{
		m_csPartyQueue.tlock( f, l ) ;
	}
	inline void party_unlock( char * f, int l )
	{
		m_csPartyQueue.tunlock( f, l );
	}

	#define party_lock()   party_lock( __FILE__, __LINE__ )
	#define party_unlock()	party_unlock( __FILE__, __LINE__ )


	inline const short GetMoonpaProp() {  return m_sClassProp ;	}
	inline void SetMoonpaProp( const short sProp ) { m_sClassProp = sProp ; }


	_Party() {			
		Init() ;
	}


	~_Party() {
	}

	inline void Init()
	{
		memset(m_cServerNumList, 0, MAX_SERVER_COUNT) ;		
		memset( m_pMember, 0, sizeof(m_pMember) ) ;
		m_cExpProperty = _ns_party_::_EXP_SHARE_TYPE_PRIVATE ;
		m_cItemProperty = _ns_party_::_ITEM_SHARE_TYPE_SOLO ;
		m_sPartyIndex = 0 ;		
		m_usCaptainUID = 0 ;
		m_cMemberCount = 0 ;
		m_sClassProp = 0 ;		// ���� �Ӽ�(������)

	}

	void DeleteParty() ;
	inline void SetPartyIndex( const short index ) { m_sPartyIndex = index ; }
	inline const short GetPartyIndex() { return m_sPartyIndex ; }
	inline const char GetMemberCount() { return m_cMemberCount ; }
	void CreateParty( _Member * const p1, _Member * const p2) ;

	void SetServer( const char cServerNum ) ;
	const int SetMember( _Member * const pMember ) ;
	const int KickMember( const char slot ) ;	
	const int Entrust( const char slot ) ;
	const int Entrust() ;
	inline const char * const GetServerList() { return m_cServerNumList ; }
	//void SetSErverList( char * cServerList ) { lock() ; memcpy( m_cServerNumList, cServerList, MAX_SERVER_COUNT ) ; unlock() ;}	
	inline const u_short GetCaptain() { return m_usCaptainUID ; }
	inline _Member * const GetPartyMemberInfo( char slot = 0 ) 
	{ 
		if( slot >= 0 && slot < MAX_PARTY_MEMBER )
			return m_pMember[slot] ; 
		else 
			return NULL;
	}
	inline const char GetExpShareType() { return m_cExpProperty ; }
	inline const char GetItemShareType() { return m_cItemProperty ; }
	inline void SetExpShareType( const char type ) { m_cExpProperty = type ; }
	inline void SetItemShareType( const char type ) { m_cItemProperty = type ; }	
};





// ������� �ʴ� Node �� Queue�� �־ �����Ѵ�.
class _j_MemeberQueue
{

private:

	CTraceAbleLock		m_csMemberQueue ;
	queue<_Member*>		m_NodeQueue ;
	map<u_short, _Member*> m_MemberMap ;	
	_Member *			m_pMember ;

#define member_lock()	m_csMemberQueue.tlock( __FILE__, __LINE__ ) ;
#define member_unlock() m_csMemberQueue.tunlock( __FILE__, __LINE__) ;

public:

	_j_MemeberQueue() ;	
	~_j_MemeberQueue() ;
	
	
	bool				Create( const size_t uiSize ) ;	// Node ���� ������ Ȯ���ϰ� �����͸� Queue�� ��Ͻ�Ų��.
	_Member *	const	GetNode( const u_short uid, _CHARAC * const pCharac) ;							// Queue�� �ִ� Node�����͸� ��ȯ�޴´�.
	void				PutNode( _Member * const Node ) ;					// �پ� Node �����͸� Queue�� ��ȯ�Ѵ�.
	_Member *	const	FindNode( const u_short uid) ;	
	void				InsertMap( const u_short uid, const _Member * const Node) ;
	void				DeleteMap( const u_short uid) ;

};



// ��Ƽ���� ť�� �̸� �Ҵ��Ͽ� ����Ѵ�.
class _j_PartyQueue
{

private:
	
	CTraceAbleLock		m_csPartyQueue ;
	queue<_Party*>		m_PartyQueue ;		
	_Party *			m_pParty ;

#define party_queue_lock()		m_csPartyQueue.tlock( __FILE__, __LINE__ ) ;
#define party_queue_unlock()	m_csPartyQueue.tunlock( __FILE__, __LINE__ ) ;

public:	

	// ��Ƽ���� �̸� �Ҵ��Ͽ� pointer�� ť�� �׾Ƶд�..
	_j_PartyQueue() : m_pParty(NULL)
	{		

		m_pParty = new _Party[_PERMISSIBLE_MAX_USER] ;						// ��Ƽ �Ҵ�

		try{
			party_queue_lock() ;
			for(int i = 0 ; i < _PERMISSIBLE_MAX_USER ; i++)
			{				
				m_PartyQueue.push( &m_pParty[i] ) ;				// ť�� ����				
			}
			party_queue_unlock() ;
		}
		catch (...) {
			throw "" ;
		}		
	}

	~_j_PartyQueue() {			
		delete [] m_pParty ;
	}


	_Party * const	GetNode() ;								// Queue�� �ִ� Party�����͸� ��ȯ�޴´�.
	void			PutNode( const _Party * const pParty) ;		// �پ� Party �����͸� Queue�� ��ȯ�Ѵ�.	
	size_t	GetSize() { return m_PartyQueue.size() ; }


private:
};

//=====================================================================

//=====================================================================

class _j_PartyManager
{

private:	
	map<short, _Party*> m_Map;	

	long m_PartyIndex ;
	short GetPartyIndex() 
	{		
		if( m_PartyIndex >= SHRT_MAX )
		{
			m_PartyIndex = 0 ;
		}		
		return  static_cast<short>(InterlockedIncrement( &m_PartyIndex ) ) ;
	}

	CTraceAbleLock	m_csPartyMap ;	
#define party_map_lock()	m_csPartyMap.tlock( __FILE__, __LINE__ ) ;
#define party_map_unlock()	m_csPartyMap.tunlock( __FILE__, __LINE__ ) ;


public:

	_j_PartyManager()
	{	
	}

	~_j_PartyManager()
	{ 	
	}

	bool Initalize() {
		m_PartyIndex = 0 ;		
		return true ;
	}



	//bool	DeleteList(short sListIndex) ;						// �ʿ��� ����Ʈ�� �����ϰ� ������ ��ȯ	
	//int		PopNode(short sListIndex, _Member * pNode) ;		// �ش� ����Ʈ�� ã�� ��Ʈ�� ����
	// -1 : fail
	// 0 : Defult success
	// 1 : ��Ƽ ����	

	_Party *  FindList(short sListIndex) ;	



	short	CreateParty(char cServerNum, u_short uid1, u_short uid2, char * name1, char * name2, OUT short & sProp ) ;	
	bool	DeleteParty(_Party * pParty ) ;	
	int		JoinParty(_Party * pParty, u_short uid, char * name, OUT short & sProp ) ;	//���� ��ȣ ��ȯ
	int		KickParty(_Party * pParty, char slotNo) ;	
	// -1 : fail
	// 0 : Defult success
	// 1 : ��Ƽ �ػ�
	// 2 : ��Ƽ�� ����

	char	EntrustCaptain(_Party * pParty, char slotNo, u_short uid = 0) ;
	// -1 : fail
	// uid ����
	bool	MoveParty(_Party * pParty, char cServerNum, u_short uid, char slotNo) ;
	void	MoveComplete( u_short uid, char slotNo ) ;

};



//#################################################################################
//
//	_h_Party_Timer : Party �� �����ϴ�  Timner �̴�.
//
//###################################################################### LHS Added....
struct _sParty_timerentry {	
	_Member *				pMember ;
	DWORD					time ;
	short					work ;
	short					sValue ;

	short					index ;
	u_short					uid ;
	char					slot ;

	bool operator<(const _sParty_timerentry& x) const { return time >= x.time ; } 

	_sParty_timerentry::_sParty_timerentry()
	{
		pMember = NULL ;
		time = 0 ;
		work = 0 ;
		sValue = 0 ;

		index = 0 ;
		uid = 0 ;
		slot = 0 ;
	}

	static int get_time_by_sec() { return GetTime_ByIntSec() ; } ;

};

class _h_Party_Timer
{
protected:

public:
	//priority_queue<_sParty_timerentry>	m_TimerQueue ;
	map<_Member *, _sParty_timerentry>				m_TimerQueue ;
	CTraceAbleLock m_party_timer_cs ;

#define party_timer_lock()		m_party_timer_cs.tlock( __FILE__, __LINE__ ) ;
#define party_timer_unlock()	m_party_timer_cs.tunlock( __FILE__, __LINE__ ) ;

public:
	_h_Party_Timer() {		
	}
	~_h_Party_Timer() {		
	}

	bool create() ;		// �����带 ���۽�Ű�°Ŵ�. 
	bool get_TimerMessage( DWORD time, _sParty_timerentry& Entry ) ;		// �ð��� �� ���� ������, true ����.
	void send_Message( _sParty_timerentry& en ) ;


} ;

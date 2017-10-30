#pragma once

#pragma warning(disable:4786)

//#define _PERMISSIBLE_MAX_USER				12000	// DS_SERVER ���� ����ϴ� �ִ� ������..(�ִ� ĳ���̶� �ٸ�)


#include "..\Global\Global_Define.h"
#include "..\Global\Global_Lock.h"
#include "..\Network\Network.h"
#include "..\Global\MSPacket_Msg.h"
#include <string>
#include <queue>
#include <map>
#include <list>
using namespace std;

class CTraceAbleLock ;

/*
#include <MY_QUEUE.h>
typedef CRationalMem<CTraceAbleLock> TLock ;
*/


struct _Member ;
struct _Mail ;

enum{
	_SEND_MODE_ME_,
	_SEND_MODE_MY_FRIENDS		
} ;


class COLE_DB_Executer ;
class _h_DS_Com ;
struct MMSG_FRIEND_LIST_RES ;
struct MMSG_MAIL_LIST_RES ;

class _FriendInfo
{

public:		
	
	_FriendInfo() //: m_pLock(new CTraceAbleLock)
	{
		
		//m_pLock = new TLock ;
		/*
		assert( m_pLock ) ;
		if( m_pLock == NULL )
		{
			throw ;
		}
		*/
		/*
		if( InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 ) == false )
		{
			throw ;
		}
		*/

		releas() ;
	}
	~_FriendInfo() {				
		//delete m_pLock ;
		/*
		DeleteCriticalSection( &cs ) ; 
		*/
	} 
	
protected :
	
	int			m_iUniqueID ;
	char		m_characName[13] ;		// ĳ���� �̸� �Ǵ� �׷� �̸����� ���δ�.	
	_h_DS_Com *	m_ds_com;				// NULL �̸� �α׿�����			
	char		m_state ;				// ĳ���� ����..
	char		m_option ;				// ģ�� �α� ��û �ź�, �Ӹ� �ź�.. ��..
	// 0 . ����.
	// 1 . ģ�� �α� �ź�
	// 2 . �Ӹ� �ź�..
	
	char		m_gm_class ;			// gm ���� Ȯ��
	//	int			Chat_Count ;		// Chat ����� ������ 
	//	_h_DS_Com * Chat_Com[50] ;
	
	// Chatting Character Info(2004.03.10 Add ozzywow)	
	_FriendInfo *	m_pChattingFriend ;
	
	list<_Member> FriendList ;
	list<_Mail>	  MailList ;
	
	typedef list<_Member>::iterator itrMember ;
	typedef list<_Mail>::iterator itrMail ;

	
	//CRITICAL_SECTION	cs ;
	CTraceAbleLock  m_Lock ;
	

#define user_lock()		m_Lock.tlock(__FILE__, __LINE__)
#define user_unlock()	m_Lock.tunlock(__FILE__, __LINE__)
	
	//void lock() { EnterCriticalSection(&cs) ; } ;
	//void unlock() { LeaveCriticalSection(&cs) ; } ;	
	
public:

	// ä�� ��ũ�� ����!
	DWORD		dwLastChatTime ;
	DWORD		dwBadChatCount ;

	union{
		char	cBackupState ;
		struct  {
			bool		bFrindListBackup : 1 ;		// 0. None, // 1. Backup Ready
			bool		bMailListBackup : 1 ;		// 0. None, // 1. Backup Ready
			bool		bBackup1 :1 ;
			bool		bBackup2 :1 ;
			bool		bBackup3 :1 ;
			bool		bBackup4 :1 ;
			bool		bBackup5 :1 ;
			bool		bBackup6 :1 ;
		} _BackupState ;
	} ;
	
	inline int	GetUniqueID() { return m_iUniqueID ; } 
	inline void SetUniqueID( int iUID ) { m_iUniqueID = iUID ; } 

	bool IsAlreadyFriend( char * name ) ;
	bool AddFriend( char * name , _FriendInfo * pFriend ) ;
	bool DeleteFriend( char * name ) ;
	bool MoveFriend( char * name , char * group ) ;
	bool GroupRename( char * oldName, char * newName ) ;
	bool AddMail( _Mail &mail ) ;
	bool DeleteMail( UINT uiIndex ) ;
	void FriendListConvertBinary( char * cList ) ;
	void SetFriendListPakcet( MMSG_FRIEND_LIST_RES * pMsgFriendListRes ) ;
	void SetMailListPacket( MMSG_MAIL_LIST_RES * pMsgMailListRes ) ;
	bool ReadFromDB( COLE_DB_Executer * pDB ) ;
	void UpdateDB( COLE_DB_Executer * pDB ) ;
	void DeleteCharacter( COLE_DB_Executer * pDB ) ;

	void Login( _h_DS_Com * ds_com ) ;	
	void Logout() ;	
	void DisconnectForLogout() ;	
	inline list<_Member> * GetFriendListPtr() { return &FriendList ; } 
	inline list<_Mail> * GetMailListPtr() { return &MailList ; } 
	inline char * GetName() { return m_characName ; }
	inline void SetName( char * name ) { memcpy( m_characName, name, 13 ) ; }
	inline void SetState( char state ) { m_state = state ;}
	inline char GetState( ) { return m_state ; }
	inline void SetOption( char option ) { m_option = option ; }
	inline char GetOption( ) { return m_option ; }
	inline bool IsLogon() { if( m_ds_com ) return true ; else return false ; }
	int	SendMsg( char * buf, int size, char mode = _SEND_MODE_ME_ ) ;
	void AllFriendsLink() ;
	void FriendLink( _FriendInfo * pFriend ) ;
	
	void SetMyChatFriend( char * name ) ;	
	inline _FriendInfo * GetMyChatFriend() { return m_pChattingFriend ; }	
	inline size_t GetFriendCurCount() 
	{ 
		size_t size ;
		user_lock() ;
		try{
			size = FriendList.size() ; 
		}
		catch (...) {
		}		
		user_unlock() ;
		return size ;
	}
	inline size_t GetMailCurCount() 
	{ 
		size_t size ;
		user_lock() ;
		try{
			size = MailList.size() ; 
		}
		catch (...) {
		}		
		user_unlock() ;
		return size ;
	}
	int GetGroupCurCount() ;
	int GetMemberCount() ;
	void MailDrop( 	u_int uiIndex, COLE_DB_Executer * pDB ) ;
	inline void SetGmClass( char gm ) { m_gm_class = gm ; } ;
	inline char GetGmClass() { return m_gm_class ; } ;
	inline bool IsGm() 
	{ 
		if( m_gm_class > 0 ) 
			return true ;
		else
			return false ;
	}
	

	void releas() ;
};


/*================================================================================
CRITACAL_SECTION TRACE Util
class CTraceAbleLock

����
CRITACAL_SECTION �� ���¸� �����Ѵ�.
lock/unlock �� �߻��Ҷ�����. �߻���ġ�� �����ϰ� ī�������ֹǷ�
dead lock �� �߻������� dead lock �߻� ��ġ�� ������ �ľ��ϱ� �����ϴ�.

by. ozzywow 

=================================================================================*/


#pragma once

#include <list>
using namespace std ;


class CTraceAbleLock
{

private:

	CRITICAL_SECTION 	cs ;
	int					m_iLockingCount ;					// lockking count
	char				m_cCurlockingPos[512] ;				// ���� locking ��ġ
	char				m_cLastlockingPos[512] ;				// ���ڸ� locking ��ġ
	int					m_iBadCheck ;						// check sum data

	static	list<CTraceAbleLock*>	g_LockList ;			// ��ϵ� Lock list

public:	
	static list<CTraceAbleLock*> * GetLockListPtr() { return &g_LockList ; }
	// ��ϵ� lock�� ��� �����Ѵ�.(���α׷� �����)
	static void Destroy()
	{		
		CTraceAbleLock * pCurLock = NULL ;				

		int i = 0 ;
		for( list<CTraceAbleLock*>::iterator itr = g_LockList.begin() 
			; itr != g_LockList.end()
			; ++itr)
		{
			pCurLock = *itr ;
			delete pCurLock ;
		}
	}


	CTraceAbleLock() : m_iLockingCount(0) , m_iBadCheck( 0 )
	{		
#if (_WIN32_WINNT > 0x0403)
		if( InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 ) == false )
		{
			throw "" ;
		}
#else
		InitializeCriticalSection( &cs ) ;
#endif

		memset( m_cCurlockingPos, 0, 512 ) ;
		memset( m_cLastlockingPos, 0, 512 ) ;	
		
		g_LockList.push_back( this ) ;						// CTraceAbleLock ��ü�� �ϳ� �����ɶ����� LockList �� �־��ش�
	} ;
	
	~CTraceAbleLock()
	{	
		try
		{	
			DeleteCriticalSection( &cs ) ; 
		}
		catch (...) 
		{		
		}		
	} ;		
	

protected:


public:
	// lock
	inline void tlock( const char * cFileName, const int iLine ) 
	{ 			
		try
		{
			if( m_iBadCheck != 0 )	// �ʱ�ȭ ���� ���� ��ü�� ����� ��츦 �˻��Ѵ�.
			{
				printf( "[lock_exception] %s, %d \n", cFileName, iLine ) ;
				return ;
			}

			sprintf(m_CurlockingPos, "ready(lock): file:%s, line:%d, count:%d\n", cFileName, iLine, m_lockingCount ) ;		

			EnterCriticalSection(&cs) ;

			++m_iLockingCount ;		
			//sprintf(m_CurlockingPos, "complete: file:%s, line:%d, count:%d\n", cFileName, iLine, m_lockingCount ) ;
#ifdef _CRITICAL_SECTION_DEBUG
			// locking count �� 1���� Ŭ��� ������ش�
			if( m_iLockingCount > 1 )
			{
				printf( m_cCurlockingPos ) ;
			}
#endif

			sprintf( m_cLastlockingPos, "last(lock): file:%s, line:%d, count:%d\n", cFileName, iLine, m_lockingCount ) ;
		}
		catch (...) 
		{
			printf("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}

	} ;

	// unlock
	inline void tunlock(  const char * cFileName, const int iLine ) 
	{ 
		try
		{

			if( m_iBadCheck != 0 )	// �ʱ�ȭ ���� ���� ��ü�� ����� ��츦 �˻��Ѵ�.
			{
				printf( "[lock_exception] %s, %d \n", cFileName, iLine ) ;
				return ;
			}

			--m_iLockingCount ;
			sprintf(m_cCurlockingPos, "ready(unlock): file:%s, line:%d, count:%d\n", cFileName, iLine, m_lockingCount ) ;
#ifdef _CRITICAL_SECTION_DEBUG
			// locking count �� 0���� ������� ������ش�
			if( m_iLockingCount < 0 )
			{
				printf( m_cCurlockingPos ) ;
			}
#endif

			LeaveCriticalSection(&cs) ; 

			sprintf( m_cLastlockingPos, "last(unlock): file:%s, line:%d, count:%d\n", cFileName, iLine, m_lockingCount ) ;
		}
		catch (...) 
		{
			printf("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}		
	} ;	

	// Lock ��ü �˻��ϴ� �Լ�
	inline int GetCurCount() { return m_iLockingCount ; } ;
	inline char * GetCurLockingPos() { return m_cCurlockingPos ; } ;
	inline char * GetLastLockingPos() { return m_cLastlockingPos ; } ;
};


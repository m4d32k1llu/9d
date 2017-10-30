/*================================================================================

CTraceAbleLock

�� Ŭ������ ũ��Ƽ�ü����� �����ϰ� ��� �����ϵ��� ��������ϴ�.
������� �ɷ������ �߻� ��θ� ���� �����ϸ� ���ø����̼ǿ� �����Ǿ� �ִ�
ũ��Ƽ�� ���ǵ��� ���¸� Ȯ���� �� �ֽ��ϴ�.
����ȭ�� ���ø����̼ǿ�����  _CRITICAL_SECTION_DEBUG �ɼ��� ���� ������ �Ͻñ� 
�ٶ��ϴ�. _CRITICAL_SECTION_DEBUG �ɼ��� ���� �ڼ��ϰ� ���� �����ϰ� ������
IO ���ϰ� �ټ� �߻��մϴ�.

by ozzywow


=================================================================================*/


#pragma once

#include <list>
using namespace std ;

class CTraceAbleLock
{

private:

	CRITICAL_SECTION 	cs ;
	int					m_lockingCount ;
	char				m_CurlockingPos[512] ;
	char				m_LastlockingPos[512] ;
	int					m_iBadCheck ;

	// ���� ������ ��ü�� �� ����Ʈ�� ���ԵǾ� �����˴ϴ�. ��ü�� �����ϱ� �Ʒ� ����Ʈ�� �����ؾ� �մϴ�.
	static	list<CTraceAbleLock*>	g_LockList ;

public:	
	static list<CTraceAbleLock*> * GetLockListPtr() { return &g_LockList ; }
	static void Destroy()
	{		
		CTraceAbleLock * pCurLock = NULL ;				

		int i = 0 ;
		for( list<CTraceAbleLock*>::iterator itr = g_LockList.begin() ; itr != g_LockList.end(); ++itr)
		{
			pCurLock = *itr ;
			delete pCurLock ;
		}
	}


	CTraceAbleLock() : m_lockingCount(0) , m_iBadCheck( 0 )
	{		
#if (_WIN32_WINNT > 0x0403)
		if( InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 ) == false )
		{
			throw "" ;
		}
#else
		InitializeCriticalSection( &cs ) ;
#endif

		memset( m_CurlockingPos, 0, 512 ) ;
		memset( m_LastlockingPos, 0, 512 ) ;

		
		// �����ɶ����� ����Ʈ�� ���Ե˴ϴ�.
		g_LockList.push_back( this ) ;
		
	} ;
	
	~CTraceAbleLock()
	{

		// �Ҹ�ȴٰ� �ؼ� Ư���� �ϴ����� �����ϴ�. ����Ʈ���� �״�� ���� �ֽ��ϴ�.
		// ���� �Ҹ�Ǵ� ������ ���ø����̼��� ����ɶ���� �����ϱ� �����Դϴ�.
		// ��� �� �κ��� ������ ������ �� �� ������.

		try{		
			DeleteCriticalSection( &cs ) ; 
		}
		catch (...) {
		}		
	} ;		
	

protected:


public:
	// ���� ȣ���� ��ġ�� �μ��� �Ѱܹ޽��ϴ�. ����� �߻��� ��������� ���ؼ���.
	void tlock( char * file, int line ) 
	{ 			
		try{
			if( m_iBadCheck != 0 )	// �ʱ�ȭ ���� ���� ��ü�� ����� ��츦 �˻��Ѵ�.
			{
				printf( "[lock_exception] %s, %d \n", file, line ) ;
				return ;
			}

#ifdef _CRITICAL_SECTION_DEBUG
			// ���ۿ� ���� ȣ�� ��ġ�� ������ ���Դϴ�. ���� �Ʒ� EnterCriticalSection() ���� ������� �����ٸ�
			// �� ���۸� Ȯ���ؼ� ���� ȣ�������� ã������.
			sprintf(m_CurlockingPos, "ready: file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;		
#endif

			EnterCriticalSection(&cs) ;


			// ��ŷ�� ī��Ʈ �Դϴ�. �� �����忡�� ���� �ι� �ɰ��� ������� �ƴ���.. ������ �������¿� ����
			// Ȯ���� ũ�⶧���� �����ؾ� �մϴ�. �� ī��Ʈ�� �׷� ��ŷ�� ã�Ƴ��µ� �����ϰ� ���ϰ̴ϴ�.
			++m_lockingCount ;		
			sprintf(m_CurlockingPos, "complete: file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;
#ifdef _CRITICAL_SECTION_DEBUG
			if( m_lockingCount > 1 )
			{
				printf( m_CurlockingPos ) ;
			}
#endif
#ifdef _CRITICAL_SECTION_DEBUG
			sprintf( m_LastlockingPos, "last(lock): file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;
#endif
		}
		catch (...) {
			printf("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}

	} ;

	// unlock�� ũ�� �ٸ����� ���⶧���� ������ �����մϴ�.
	void tunlock( char * file, int line ) 
	{ 
		try{

			if( m_iBadCheck != 0 )	// �ʱ�ȭ ���� ���� ��ü�� ����� ��츦 �˻��Ѵ�.
			{
				printf( "[lock_exception] %s, %d \n", file, line ) ;
				return ;
			}

			--m_lockingCount ;
#ifdef _CRITICAL_SECTION_DEBUG
			sprintf(m_CurlockingPos, "file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;
#endif
#ifdef _CRITICAL_SECTION_DEBUG
			if( m_lockingCount < 0 )
			{
				printf( m_CurlockingPos ) ;
			}
#endif

			LeaveCriticalSection(&cs) ; 
#ifdef _CRITICAL_SECTION_DEBUG
			sprintf( m_LastlockingPos, "last(unlock): file:%s, line:%d, count:%d\n", file, line, m_lockingCount ) ;
#endif
		}
		catch (...) {
			printf("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
		}		
	} ;	

	// �Ʒ� �޽�带 ���ؼ� ���� �� ���¸� �ľ��Ͻø� �˴ϴ�.

	// ���� �ɸ� ��ŷ ī��Ʈ
	inline int GetCurCount() { return m_lockingCount ; } ;
	// ������� �ɷȴٸ� ��ŷ ȣ������
	inline char * GetCurLockingPos() { return m_CurlockingPos ; } ;
	// ������� �ɷȴٸ� ���� ȣ�� ������ ȣ������
	inline char * GetLastLockingPos() { return m_LastlockingPos ; } ;
};


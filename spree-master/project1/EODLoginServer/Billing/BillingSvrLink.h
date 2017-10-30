#pragma		once
#include <Winsock2.h>
#include <mswsock.h>
#include <string>
#include <queue>
#include <map>
#include "..\lib_ND_Net\socket_pooling.h"




using namespace std;
using namespace ND_Net ;

template <class Type>
class CTQueue
{
protected : 
	queue<Type*>	ReadyQueue ;
	queue<Type*>	UseQueue ;
	Type  *			pNode ;

	inline void ReadyLock() { EnterCriticalSection( &m_ReadyQueue_cs ) ; } 
	inline void ReadyUnlock() { LeaveCriticalSection( &m_ReadyQueue_cs ) ; } 	

	inline void UseLock() { EnterCriticalSection( &m_UseQueue_cs ) ; } 
	inline void UseUnlock() { LeaveCriticalSection( &m_UseQueue_cs ) ; } 

public:

	CRITICAL_SECTION m_ReadyQueue_cs;	// ���ť ����
	CRITICAL_SECTION m_UseQueue_cs;		// ������� ť ����


	CTQueue(size_t slotCount ) 
	{
#if (_WIN32_WINNT > 0x0403)
		if( InitializeCriticalSectionAndSpinCount( &m_ReadyQueue_cs, (0x80000000)|2000 ) == FALSE ) {			
			throw ;
		}	

		if( InitializeCriticalSectionAndSpinCount( &m_UseQueue_cs, (0x80000000)|2000 ) == FALSE ) {			
			throw ;
		}	
#else
		InitializeCriticalSection( &m_ReadyQueue_cs ) ;
		InitializeCriticalSection( &m_UseQueue_cs ) ;
#endif

		pNode = new Type[slotCount] ;
		assert( pNode ) ;		
		for( u_int i=0; i<slotCount; i++)
		{			
			Ready_Put( &pNode[i] ) ;			
		}		
	};

	~CTQueue() {
		delete [] pNode ;
		DeleteCriticalSection( &m_ReadyQueue_cs) ;
		DeleteCriticalSection( &m_UseQueue_cs) ;
	};


	// ��� ť�� �ֱ� 
	inline void Ready_Put(Type * pNode) {
		if( pNode )
		{
			ReadyLock() ;
			try{
				ReadyQueue.push(pNode) ;
			}
			catch (...) {
				::PrintConsole("[EXCEPTION] %s, %d\n", __FILE__, __LINE__ ) ;
			}			
			ReadyUnlock() ;
		}		
	}

	// ��� ť���� ������
	inline Type * Ready_Pop() {		

		ReadyLock() ;
		try
		{			
			if( ReadyQueue.empty() ) {
				pNode = NULL ;
			}
			else
			{
				pNode = ReadyQueue.front() ;			
				ReadyQueue.pop() ;
			}			
		}
		catch(...)
		{			
			::PrintConsole("[EXCEPTION] inline Type * Ready_Pop() \n" ) ;
			pNode = NULL ;
		}
		ReadyUnlock() ;

		return pNode ;

	} 

	// ��� ť�� �ֱ�
	inline void Use_Put(Type * pNode) {
		if( pNode ) 
		{
			UseLock() ;
			try{
				UseQueue.push(pNode) ;
			}
			catch (...) {
				::PrintConsole("[EXCEPTION] %s, %d\n", __FILE__, __LINE__ ) ;
			}			
			UseUnlock() ;
		}		
	}

	// ��� ť���� ������
	inline Type * Use_Pop() {
		UseLock() ;
		try
		{			
			if( UseQueue.empty() ) {
				pNode = NULL ;
			}
			else
			{
				pNode = UseQueue.front() ;			
				UseQueue.pop() ;
			}
		}
		catch(...)
		{			
			::PrintConsole("[EXCEPTION] inline Type * Use_Pop() \n" ) ;
			pNode = NULL ;
		}
		UseUnlock() ;

		return pNode ;

	} 



	inline int	GetReadyCount() {
		return ReadyQueue.size() ;		
	}
	inline int	GetUseCount() {
		return UseQueue.size() ;
	}


};




#ifndef _ND_SERVER_TEST_ACCEPT_EX_EXAMPLE_
#define _ND_SERVER_TEST_ACCEPT_EX_EXAMPLE_

#pragma warning(disable:4786)

#include <mswsock.h>
#include <queue>
using namespace std;

#include "../lib_ND_Util/ND_Thread.h"


#define _RECV_BUF_SIZE_				8192		
#define _BIG_UNIT_SIZE_				1536

#ifndef _MEGA_
#define _MEGA_	1048576
#endif


#define _KEEPALIVE_TIME_					30000L
#define _KEEPALIVE_INTERVAL_				500L

namespace ND_Net {

//#define _MAX_PACKET_SLOT_			196608     

class CSocketBuffer ;

class CBaseUser
{
public:
	void SetNetBuffer( CSocketBuffer * pBuffer ) 
	{		
		m_pNetIO = pBuffer ;
	}

	virtual void OnConnectionClose() = 0 ;
	virtual bool Parsing( DWORD recvbytes ) = 0 ;

	CSocketBuffer *			m_pNetIO ;

protected:


} ;
/// Mode : IO
enum cp_mode
{
	mode_recv = 0,
	mode_send_complete,
	mode_accept_complete, 
	mode_close_socket, 
	mode_end
} ;


class COV_UNIT : public WSAOVERLAPPED		// _h_Overlapped
{
public:
	WSABUF		m_wsabuf ;
	DWORD		m_dwFlag ;			
	int			m_iMode ;			// 
	
#ifdef _DEBUG
	u_long		m_iCapacity ;
#endif
	
#ifdef _DEBUG
	void	Initialize( char * buf, int cap ) ;
#else
	void	Initialize( char * buf ) ;
#endif
	
	//void	SetSendData( char * buf, int length ) ;
	int		AddSendData( char * buf, int length ) ;
	int		SendPacket( const SOCKET sock ) ;
	void	SetOperation( int mode ) { m_iMode = mode ; }
} ;




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class CServerSocket ;

class CSocketBuffer : public COV_UNIT
{

public:	
	CServerSocket * const m_server ;
	SOCKET		socket ;
	SOCKADDR_IN	addr ;
	bool		m_bAccepted ;
	CBaseUser *	m_pUser ;					// ������ ����Ű�� ������	

	char *		pRealBuf ;	
	int			RemainBytes ;
	char		m_buf[_RECV_BUF_SIZE_] ;	// ���۷� ����� ����.
	

	static	LONG	m_glCreateCount ;

	/*
	long		m_lTotalSendByte ;
	long		m_lStartTime ;			//
	long		m_lEndTime ;		]
	*/
	
protected:
		
	
public:
	CSocketBuffer( CServerSocket * server) : m_server(server), m_bAccepted(false)		
	{ 
		::InterlockedIncrement(&m_glCreateCount); 
		release() ;
	}
	~CSocketBuffer()
	{ 
		::InterlockedDecrement(&m_glCreateCount) ; 
	} 

	static LONG GetUsingCount() 
	{ 		
		return m_glCreateCount ;
	}

	bool RecvRequest() ;	// Recv ��û�� �Ѵ�. 

	bool InitSocket() ;						// ���� �ڵ��� ���� �Ҵ��Ѵ�..
	bool ReleaseSocket() ;					// ���� �ڵ��� �����Ѵ�.

	inline void release()
	{		
		memset( &addr, 0 , sizeof(addr) ) ;
		m_bAccepted = false ;	
		m_pUser = NULL ;
		
		pRealBuf = m_buf ;
		RemainBytes = 0 ;
		memset( m_buf, 0, _RECV_BUF_SIZE_) ;
		
		COV_UNIT::Internal = InternalHigh	= 0;	
		COV_UNIT::Offset = OffsetHigh	= 0;
		COV_UNIT::hEvent = NULL;	
		COV_UNIT::m_wsabuf.buf = m_buf ;
		COV_UNIT::m_wsabuf.len = 0 ;
		COV_UNIT::m_dwFlag = 0 ;
		COV_UNIT::m_iMode = 0 ;	
	}

	
	void move( int remain )  ;	
	void SetUser( CBaseUser * pUser ) { m_pUser = pUser ; }
	void SetMode( int mode ) { m_iMode = mode ; }
	
	COV_UNIT * ReserveBuffer( int size ) ;		// 
	int  SendRequest( COV_UNIT * pOV ) ;		// Unit Data Send...
	int  SendRequest( char * buf, int size ) ;	// Single Send Request
	int  SendRequest( int size , char * buf) ;	// Single Send Request

	bool IsConnected() ;

	void Shutdown( int how ) ;

	/*
	void net_stat_start( DWORD time )	{ m_lStartTime = time ; m_lTotalSendByte = 0 ; }
	void net_stat_end( DWORD time )		{ m_lEndTime = time ; }
	float net_stat() ;
	void net_stat_reset()				{ ::InterlockedExchange( &m_lTotalSendByte, 0 ) ; }
	*/
} ;

class CSendOverlappedSlot
{
protected:
	int					m_iPosition ;
	int					m_iTotalSize ;
	int					m_iGetSlot ;		// 0~Max���� ��ȯ�Ѵ�. 
	int					m_nOVSlot ;			// OV ������ ����
	
	char *				m_pSendBuf ;
	COV_UNIT *			m_pOVSlot ; 
	
	CRITICAL_SECTION	m_csOLSlot ;
	
	inline void lock()		{ EnterCriticalSection( &m_csOLSlot ) ; }
	inline void unlock()	{ LeaveCriticalSection( &m_csOLSlot ) ; }
	
public:
	CSendOverlappedSlot() {
		m_iPosition = 0 ;
		m_iTotalSize = 0 ;
		m_pSendBuf = NULL ;
		m_iGetSlot = 0 ;
		
		if( InitializeCriticalSectionAndSpinCount( &m_csOLSlot, (0x80000000)|2000 )  == FALSE ) {
			printf( "[INIT ERROR] OL Slot cs Error Critical Section\n" ) ;
			throw ;
		}
	}
	
	~CSendOverlappedSlot() {		// �Ҹ���
		DeleteCriticalSection( &m_csOLSlot ) ; 
	}
	
	bool CreateOVSlot( int slotcount, int megasize ) ;					// Create Slot
	
	void Get_SendUnit( COV_UNIT ** ppSender, int reqSize ) ;		// �׻� ���� : 
	void Get_BigUnit( COV_UNIT ** ppSender ) ;		// 1536 ��ŭ�� ��������.

#ifdef _DEBUG
	void print_state() ;
#endif
} ;


/*#################################################################################
//
//  _h_CompletionPort
//
//###############################################################################*/
class _h_CompletionPort
{
protected:
	HANDLE				m_hIOCP ;
	
	COV_UNIT			m_OVUnit ;		// ���� �� �ʿ���.

	int					m_nWorker ;		// ��Ŀ ������ �� ��.
	
public:
	_h_CompletionPort(int nConcurrentWorker) : m_hIOCP(NULL), m_nWorker(nConcurrentWorker) 
	{
	}

	bool Create( int nWorker ) ;					// ���� ������ ���� �����Ѵ�. 
	bool AssociateSocket( CSocketBuffer * pB ) ;		// ������ ������Ű�µ�, �̶� Ű ���� �Ȱ��� ������ �����Ѵ�.
	bool AssociateSocket( SOCKET s ) ;
	
	// GetQueue~~ ��û
	int GetStatus(DWORD * pCompKey, PDWORD pdwNumBytes, WSAOVERLAPPED** ppo, DWORD dwMilliseconds = INFINITE) ;
	void PostStatus( DWORD CompKey, DWORD dwNumBytes, WSAOVERLAPPED * pOV ) ;
	
	void PostCloseWorkThread() ;
	void Close() {
		CloseHandle( m_hIOCP ) ;
	}
	
} ;


//
static BOOL LoadExtensionFunction( SOCKET s, GUID functionID, void **ppFunc ) ;

/*#################################################################################
//
//  CAcceptSocket :
//
//###############################################################################*/
class CServerSocket : public ND_Util::CThread
{
public :

	CServerSocket() : m_iocp(0), m_sendSlot(), m_iTempBuf(0)
	{
	}

	BOOL CreateServerSocket( 
		int nWorker,				// IO ó�� ��Ŀ �������� ����
		char * listenip,			// Accept ������ �ּ�
		u_short listenport,			// Accept ������ bind ��Ʈ
		int nSendSlotCount,			// ���� Overlapped �� ����
		int nBufMegaSize,			// ���� ��������� ũ��(Mega����)
		int nSockBufPool,			// ���� ������ Ǯ�� ����
		int backlog,				// ���� ������ ��α�
		int nAcceptPostMax,			// �ִ� Accept ȸ��
		int nAcceptPostMin			// �ּ� Accept ȸ��
		) ;		// Initialize

	void CloseListenSocket() ;		// 


	BOOL DisconnectEx( CSocketBuffer * pSockBuf, LPOVERLAPPED pUnit, DWORD flag ) 
	{
		return pDisconnectEx( pSockBuf->socket, pUnit, flag, 0 ) ;
	}

	BOOL TransmitFile( CSocketBuffer * pSockBuf, LPOVERLAPPED pUnit ) 
	{
		return pTransmitFile( pSockBuf->socket, 0, 0, 0, pUnit, 0, TF_DISCONNECT | TF_REUSE_SOCKET ) ;
	}
	

	void GetAcceptExSockaddrs(
		PVOID pOutputBuffer,       
		DWORD receiveDataLength,  
		DWORD localAddressLength,  
		DWORD remoteAddressLength,  
		LPSOCKADDR *ppLocalSockaddr,  
		LPINT pLocalSockaddrLength,  
		LPSOCKADDR *ppRemoteSockaddr,  
		LPINT pRemoteSockaddrLength ) 
	{
		pGetAcceptExSockaddrs( pOutputBuffer, receiveDataLength, localAddressLength,  
									remoteAddressLength, ppLocalSockaddr, pLocalSockaddrLength, 
									ppRemoteSockaddr, pRemoteSockaddrLength ) ;
	}

	bool AcceptCompleted( CSocketBuffer * pBuffer, DWORD dwReceiveByte ) ;


	void DisconnectForReuse( CSocketBuffer * pBuffer ) ;

	void AcceptRequest() ;


	//////////////////////////////////////////////////////////////////////////
	// Shutdown
	void Shutdown() ;


	//////////////////////////////////////////////////////////////////////////
	// Virtural

	//virtual void OnConnectionEstablished( CSocketBuffer * pSock ) ;	// �ٱ����� �̰� �����ε� : 

	//////////////////////////////////////////////////////////////////////////
	// Socket Attach/Detach, PostIOCP
	void PostStatus( DWORD dwNumBytes, CSocketBuffer * pBuffer ) ;
	void PostCloseWorkThread() ;

	//////////////////////////////////////////////////////////////////////////
	// 
	void Get_SendUnit( COV_UNIT ** ppOV, int size ) { m_sendSlot.Get_SendUnit( ppOV, size ) ; }
	void Get_BigUnit( COV_UNIT ** ppOV ) { m_sendSlot.Get_BigUnit( ppOV ) ; }


protected:
	BOOL LoadExtensionFunctions() ;
	BOOL LoadAcceptEx( SOCKET s ) ;
	BOOL LoadGetAcceptExSockaddrs( SOCKET s ) ;
	BOOL LoadDisconnectEx( SOCKET s ) ;
	BOOL LoadTransmitFile( SOCKET s ) ;
	
	BOOL CreateAcceptSocket() ;

	void Accept() ;

	int AcceptEx( CSocketBuffer * pSockBuf ) 
	{
		return pAcceptEx( m_listensocket, pSockBuf->socket, pSockBuf->m_buf, 0, 
			m_sizeofaddr, m_sizeofaddr, &m_byteReceived, static_cast<LPOVERLAPPED>(pSockBuf) ) ;
	}

	virtual int Run( void ) ;		// �� ��ü�� ������
	virtual int Worker( void ) ;	// iocp ó���� ��Ŀ ������.

	static unsigned int __stdcall WorkerThread( LPVOID lp ) ;

private:
	CRITICAL_SECTION		m_csPool ;				// pool ��.

protected:
	SOCKET					m_listensocket ;
	_h_CompletionPort		m_iocp ;
	CSendOverlappedSlot		m_sendSlot ;

	SOCKADDR_IN				m_addr ;

	int						m_sizeofaddr ;
	unsigned long			m_byteReceived ;

	long					m_nMaxPendingAccept ;		// Accept �� ���...
	long					m_nMinPendingAccept ;		// �ּ� ������ accept
	long					m_nCurPendingAccept ;		// ���� ����� Accept �� ȸ��.

	long					m_nMaxPoolCount ;
	
	int						m_nWorkerThread ;		// 

	HANDLE					m_hAcceptPostEvent ;	// Accept ����Ʈ ��û �̺�Ʈ
	HANDLE					m_hNoAcceptWaitEvent ;	// ������� Accept��û�� ���� �� �߻�.
	HANDLE					m_hShutDownEvent ;		// ���� ���� ��.

	//ND_Util::CQueueFixedSize<CSocketBuffer>		m_queuePool ;
	queue<CSocketBuffer*>	m_UsingPool ;			// ������� ��Ĺ Ǯ(socket handle �� �Ҵ��..)
	queue<CSocketBuffer*>	m_RecyclePool ;			// ��������� ���� ���� Ǯ..( closesocket ��..)

protected:
	inline void 			pool_lock() { EnterCriticalSection( &m_csPool ) ; }
	inline void				pool_unlock() { LeaveCriticalSection( &m_csPool ) ; }
	

public:
	int						m_iTempBuf ;
	
protected:
	static LPFN_ACCEPTEX				pAcceptEx ;
	static LPFN_GETACCEPTEXSOCKADDRS	pGetAcceptExSockaddrs ;
	static LPFN_DISCONNECTEX			pDisconnectEx ;
	static LPFN_TRANSMITFILE			pTransmitFile ;
} ;


} // namespace nd_net


#endif
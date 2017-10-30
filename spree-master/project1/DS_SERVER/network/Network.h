#pragma once

//#include <WinSock2.h>

#ifndef _NETWORK_H_AWEGFASDFAWE
#define _NETWORK_H_AWEGFASDFAWE

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

	void	SetSendData( char * buf, int length ) ;
	//int		AddSendData( char * buf, int length ) ;
	int		SendPacket( const SOCKET sock, const bool bZeroCheck = false ) ;
	void	SetOperation( int mode ) { m_iMode = mode ; }

	void    reset() 
	{
		m_wsabuf.buf = NULL ;
		m_wsabuf.len = 0 ;
		m_dwFlag = 0 ;
		m_iMode = 0 ;

#ifdef _DEBUG
		m_iCapacity = 0 ;
#endif
	}
} ;


/*

class CDSRecv_Overlapped : public COV_UNIT
{
public:
	char *		pRealBuf ;
	int			RemainBytes ;

	// �߰�
	char *		m_buf ;	// ���۷� ����� ����.
	size_t		m_bufSize ;

public:
	CDSRecv_Overlapped( size_t size ) : m_bufSize(size)
	{ 
		m_buf = new char[size] ;
	}
	~CDSRecv_Overlapped()	
	{
		delete [] m_buf ;
	} 
	
	bool RecvPacket( const SOCKET sock ) ;	// Recv ��û�� �Ѵ�. 
	void move( int remain ) ;
	void reset() ;
} ;

*/



class CSendOverlappedSlot
{
protected:
	int					m_iPosition ;
	int					m_iTotalSize ;
	int					m_iGetSlot ;		// 0~Max���� ��ȯ�Ѵ�. 
	int					m_nOVSlot ;			// OV ������ ����

	char *				m_pSendBuf ;
	COV_UNIT *			m_pOVSlot ; 

	CTraceAbleLock		m_csSendOvSlot ;

	//inline void lock()		{ EnterCriticalSection( &m_csOLSlot ) ; }
	//inline void unlock()	{ LeaveCriticalSection( &m_csOLSlot ) ; }
#define send_ov_lock()		m_csSendOvSlot.tlock( __FILE__, __LINE__ ) ;
#define send_ov_unlock()	m_csSendOvSlot.tunlock( __FILE__, __LINE__) ;
	
public:
	CSendOverlappedSlot() {
		m_iPosition = 0 ;
		m_iTotalSize = 0 ;
		m_iGetSlot = 0 ;
		m_nOVSlot = 0 ;

		m_pSendBuf = NULL ;		
		m_pOVSlot = NULL ;
		

		/*
		if( InitializeCriticalSectionAndSpinCount( &m_csOLSlot, (0x80000000)|2000 )  == FALSE ) {
			printf( "[INIT ERROR] OL Slot cs Error Critical Section\n" ) ;
			throw ;
		}
		*/
	}

	~CSendOverlappedSlot() {		// �Ҹ���		
		//DeleteCriticalSection( &m_csOLSlot ) ; 

		delete m_pSendBuf ;
		delete m_pOVSlot ;
	}

	bool CreateOVSlot( int slotcount, int megasize ) ;					// Create Slot

	void Get_SendUnit( COV_UNIT ** ppSender, int reqSize ) ;		// �׻� ���� : 
	//void Get_BigUnit( COV_UNIT ** ppSender ) ;		// 1536 ��ŭ�� ��������.


//#ifdef _DEBUG
//	void print_state() ;
//#endif
} ;


//#################################################################################
//
//	_h_Overlapped : Ŀ���� �������� ��Ŷ
//                  move ��Ŷ�� ������ ��, �� ������ �ʾҴµ� ����Ǵ� ��찡 ����
//                  �׷��� send �� ���� ������ nSend �� �������� Ƚ���� �����Ѵ�. 
//                  ����� �޾��� �� nSend �� 1 �� �Ǹ� �� ��Ŷ�� delete( ���� ������
//                  ����Ѵٸ� put ) �Ѵ�.  0726
//
//###################################################################### LHS Added....
// ũ�� 160 bytes ����

class _h_DS_Com ;
class _h_Overlapped : public WSAOVERLAPPED
{
public:
	WSABUF		m_wsabuf ;
	bool		m_bMode ;			// 
	DWORD		m_dwFlag ;			
	_h_DS_Com *	m_pDSComm;
} ;

class _queue_Packet ;
class _h_Recv_Overlapped : public _h_Overlapped
{
public:
	// ���� ���� ����ü..
	struct _GS_INFO 
	{
		unsigned long		ipAddr ;
		short				state ;
	} ;		

	// �߰�
	_GS_INFO	m_gs_info;					// ����� ��������
	SOCKET		m_socket;					// ����		

	
	u_short		m_usLastParsingSize;
	u_char		m_ucLastParsingMsg;

	DWORD		m_dwTotalRecvBytes;
	int			m_iLastErrorCode;

	int			iBufSize ;
	char *		pRealBuf ;
	int			RemainBytes ;

	//CRITICAL_SECTION m_csRecvOV ;		// ���� ����Ÿ ��û�� ����.
	char *		m_pBuf ;

public:
	_h_Recv_Overlapped()	
	{ 
		Internal			= InternalHigh	= 0;	
		Offset				= OffsetHigh	= 0;
		hEvent				= NULL;		
		
		RemainBytes = 0 ;		// Add...
		m_dwFlag = 0 ;
		m_bMode = true ;	

		m_gs_info.ipAddr = 0;
		m_gs_info.state = 0;

		m_usLastParsingSize = 0;
		m_ucLastParsingMsg = 0;

		m_dwTotalRecvBytes = 0;
		m_iLastErrorCode = 0;

		m_pDSComm = NULL;	
		pRealBuf = NULL ;
		m_pBuf = NULL ;

		/*
		if( InitializeCriticalSectionAndSpinCount( &m_csRecvOV, (0x80000000)|2000 )  == FALSE ) {
			printf( "INIT - Recv OV Critical section : Create Critical Section Failed\n" ) ;		
		}
		*/
	}
	~_h_Recv_Overlapped()	{
		//DeleteCriticalSection( &m_csRecvOV ) ;
		if(m_pBuf != NULL) {
			delete [] m_pBuf ;
			m_pBuf = NULL;
		}
	} 

	bool CreateRecvOV( int size ) {
		iBufSize = size ;

		m_pBuf = new char[iBufSize] ;
		if( m_pBuf )
		{
			m_wsabuf.buf = m_pBuf ;
			pRealBuf = m_pBuf ;		// Add...

			return true ;
		}

		return false ;
		
	}
	
	bool RecvPacket( const SOCKET sock ) ;	// Recv ��û�� �Ѵ�. 
	void move( int remain ) ;
	void reset() ;

	inline void SetLastParsingSize( u_short size )	{ m_usLastParsingSize = size; }
	inline void SetLastParsingMsg( u_char msg )		{ m_ucLastParsingMsg = msg; }

	inline u_short GetLastParsingSize()				{ return m_usLastParsingSize; }
	inline u_char GetLastParsingMsg()				{ return m_ucLastParsingMsg; }

	inline void SetErrorCode( int err )				{ m_iLastErrorCode = err; }
	inline int GetErrorCode()						{ return m_iLastErrorCode; }

	// Add jk.Hong
	bool Parsing(DWORD bufLen);

	//inline void Lock() { EnterCriticalSection(&m_csRecvOV); }
	//inline void UnLock() { LeaveCriticalSection(&m_csRecvOV); }

	int SendPacket(const SOCKET sock) {
		int err ;
		err = WSASend( sock, &m_wsabuf, 1, &m_wsabuf.len, 0, (LPWSAOVERLAPPED)this, NULL ) ;
		if( err == SOCKET_ERROR ) {
			err = WSAGetLastError() ;
			if( err != WSA_IO_PENDING ) 
			{
				m_iLastErrorCode = err;
				return err ;
			}
		}

		return 0 ;
	}

	void ShowState();
} ;
typedef _h_Recv_Overlapped	CSendPkt;





class _h_CompletionPort
{
protected:
	HANDLE				m_hIOCP ;

public:
	bool Create( int n ) ;					// ���� ������ ���� �����Ѵ�. 
	bool AssociateSocket( _h_Recv_Overlapped * pOv  ) ;		// ������ ������Ű�µ�, �̶� Ű ���� �Ȱ��� ������ �����Ѵ�.

	// GetQueue~~ ��û
	bool GetStatus(DWORD * pCompKey, PDWORD pdwNumBytes, WSAOVERLAPPED** ppo, DWORD dwMilliseconds = INFINITE) ;

} ;


//#define _DS_SEND_BUFFER_SIZE_			131072	//Send �� ����Ÿ�� ����ϴ� ����..
#include "../Packet_Queue/WebShopPktQueue.h"
class _h_DS_Com
{
	/* =====================================================================================
	36���� �̺�Ʈ�� �̿��Ͽ�, ���� �޽����� �����ϰ�, �������� 36���� �̺�Ʈ�� �����Ѵ�. 
	�Լ� �̸��� request/wait �� �����ϴ� �Լ��� ���� ��û�� �����.
	�������� ��ȿ �̺�Ʈ�� ��ٸ���, ť�δ� ��ȿ �̺�Ʈ�� �ε����� ��´�.
	===================================================================================== */
	// sync �κ� : 2003/08/21
	// sync �� ������ ���� �����忡���� ���̰�, ������ �� ���࿡���� async �� ó���Ѵ�. 
protected:

	SOCKET			m_dssock ;			// DS ������ ��� ����.	
	//CRITICAL_SECTION g_csSendData ;		// ���� ����Ÿ ��û�� ����.
	

	bool			m_bUse ;				// �۾����̳� �ƴϳ�
	bool			m_bSelect ;				// false = 1, true = 2 ; 

	u_char			m_ucLastProcMsg;		// 
	u_short			m_usLastProcSize;		// 

	DWORD			m_dwTotalSendBytes;


public:
	_h_Recv_Overlapped  m_RecvOV ;
	CSendOverlappedSlot	m_sendSlot ;	// send ov slot

	inline void set_last_msg( u_char msg )		{ m_ucLastProcMsg = msg; }
	inline void set_last_size( u_short size )	{ m_usLastProcSize = size; }

	inline u_char get_last_msg()				{ return m_ucLastProcMsg; }
	inline u_short get_last_size()				{ return m_usLastProcSize; }
	

	_h_DS_Com() {

		/*
		if( InitializeCriticalSectionAndSpinCount( &g_csSendData, (0x80000000)|2000 )  == FALSE ) {
			printf( "INIT - DS_Com : Create Critical Section Failed\n" ) ;
			throw ;
		}
		*/

		init() ;

#ifdef ITEMSHOP_PATCH_200607
		pWebShopPktQueue = new CWebShopPktQueue;
		assert(pWebShopPktQueue);
#endif
	}
	~_h_DS_Com() {
#ifdef ITEMSHOP_PATCH_200607
		SAFE_DELETE(pWebShopPktQueue);
#endif

		//DeleteCriticalSection(&g_csSendData);
	}

	void init() 
	{
		m_dssock = 0 ;

		m_bUse = false ;
		m_bSelect = false ;

		m_dwTotalSendBytes = 0;

		m_RecvOV.reset();
	}

	SOCKET get_sock() { return m_dssock ; }	

	bool createDSCom( const int iSendSlotMaxCount, const int iSendSlotMaxMegaSize, const int iRecvBufSize ) ;		// 
	void ConnectCom( SOCKET gsSock) ;
	void DisConnectCom() ;
	

	bool Send( int size, char * buf ) ;
	bool Recv( SOCKET sock ) {
		return m_RecvOV.RecvPacket( sock ) ;
	}

	void ShowState( int iServer );
	
#ifdef ITEMSHOP_PATCH_200607
public:
	CWebShopPktQueue* pWebShopPktQueue;
#endif
} ;


class _DSCom_Queue
{

protected:	
	_h_DS_Com	m_DSCom[GS_MAX] ;


public:

	
	bool Init( const int iSendSlotCount, const int iSendSlotMegaSize, const int iRecvBufSize )
	{
		for( int i = 0 ; i < GS_MAX ; i++ )
		{
			if( !m_DSCom[i].createDSCom( iSendSlotCount, iSendSlotMegaSize, iRecvBufSize ) ) 
			{
				return false ;
			}			
		}
		return true ;
	}

	_h_DS_Com * GetDSCom( )
	{
		for( int i = 0 ; i < GS_MAX ; i++ )
		{
			if( m_DSCom[i].get_sock() == 0 ) {
				m_DSCom[i].init() ;
				return &m_DSCom[i] ;
			}
		}
		return NULL ;
	}

	bool GlobalSend( int size, char * buf )
	{		
		for( int i = 0 ; i < GS_MAX ; i++ )
		{
			if( m_DSCom[i].get_sock() != 0 ) 
			{
				m_DSCom[i].Send( size, buf );
			}
		}
		return true ;
	}
};

#endif

#ifdef ITEMSHOP_PATCH_200607
struct WEB_SHOP_PKT_PROC_INFO {
	_h_DS_Com* pClient;
};

struct WEB_SHOP_PKT_PROC_PARAM {
	_h_DS_Com* pClient;
};
#endif

//#################################################################################
//
//	Net Base Structures
//
//###################################################################### LHS Added....
#define MAX_OF_RECV_BUFFER		(32768)
#define	MAX_OF_SEND_BUFFER		(16384)		// �׳� ū ��.
/// Mode : IO
enum io_mode
{
	iomode_recv = 0,
	iomode_send_complete,
	iomode_accept_complete, 
	iomode_close_socket, 
	iomode_end
};

struct CIO_UNIT : public WSAOVERLAPPED
{
	DWORD		m_dwFlag;		// Zero	
	int			m_iMode;		// io_mode
	u_long		m_ulTempSize;

	WSABUF		m_wsaBuf;

	char		m_data[MAX_OF_SEND_BUFFER];

	CIO_UNIT()
	{
		ZeroMemory( this, sizeof(CIO_UNIT) );
		m_wsaBuf.buf = m_data;
		m_wsaBuf.len = 0;
	}

	void SetData( void * pData, int length )
	{
		// ��� ��Ŷ�� �տ� 2����Ʈ u_short �� ���̸� ���δ�.
		m_iMode = iomode_send_complete;
		u_short usLen = static_cast<u_short>(length);
		CopyMemory( m_data, &usLen, sizeof(usLen) );
		CopyMemory( &m_data[sizeof(usLen)], pData, length );
		m_wsaBuf.len = (u_long)(length+sizeof(usLen));
	}

	WSABUF * GetWSABuf()	{ return &m_wsaBuf; }
	u_long * GetPLength()	{ return &m_ulTempSize; }

};



//#################################################################################
//
//	CNDIOCPObejct : IOCP ������.
//
//###################################################################### LHS Added....
class CNDIOCPObejct
{
public:
	bool Create( int n );					// ���� ������ ���� �����Ѵ�. 
	bool AssociateSocket( SOCKET s, DWORD CompKey );		// ������ ������Ű�µ�, �̶� Ű ���� �Ȱ��� ������ �����Ѵ�.

	// GetQueue~~ ��û
	int GetStatus(DWORD * pCompKey, PDWORD pdwNumBytes, WSAOVERLAPPED** ppo, DWORD dwMilliseconds = INFINITE);
	void PostStatus( DWORD CompKey, DWORD dwNumBytes, WSAOVERLAPPED * pOV );

	void PostCloseWorkThread();
	void Close() {
		CloseHandle( m_hIOCP );
	}


protected:
	HANDLE				m_hIOCP;

	CIO_UNIT			m_OVUnit ;		// ���� �� �ʿ���.
} ;

#include <list>
using namespace std;

/*
Send �� ������ Send ���� ���������� �������� ������ ����.
30�� �̻� �ǵ�, Send ���۰� �ߺ��� ���� ���� ���̴�. 
�׷��� 100���� ����. 

�� ��ü�� ��� CConnectionObject �� ����Ѵ�. 


�׷���, ������ Send Buffer�� ���/������ �ݺ��ϸ� �޸� Ǯ���� �̷����� �Ѵ�.
*/
class CSendBuffer
{
public:
	CSendBuffer( int nBuffer ) : m_iSendBufferPosition(0), m_iSendBufferSize(nBuffer), m_pBufferUnit(NULL)
	{
		m_pBufferUnit = new CIO_UNIT[m_iSendBufferSize];
		if( !m_pBufferUnit )
			throw "(Send Buffer) Create Buffer Fail";

		if( FALSE == InitializeCriticalSectionAndSpinCount( &m_csSendBuffer, 0x80000100 ) )
			throw "(Send Buffer) Create Critical Section Fail";
	};

	~CSendBuffer()
	{
		Sleep(100);		// ���� ����� �Ϸ�Ǵ� ������, �׳� ���� ��ٸ��� ������ ��ü�Ѵ�.
		if( m_pBufferUnit )
		{
			DeleteCriticalSection(&m_csSendBuffer);
			delete [] m_pBufferUnit;
			m_pBufferUnit = NULL;
		}
	}

	CIO_UNIT * GetSendBuffer()	
	{
		CIO_UNIT * pIO = NULL;
		int pos = 0;
		EnterCriticalSection(&m_csSendBuffer);
		pos = m_iSendBufferPosition;
		m_iSendBufferPosition = (m_iSendBufferPosition+1) % m_iSendBufferSize;
		pIO = &m_pBufferUnit[pos];
		LeaveCriticalSection(&m_csSendBuffer);
		return pIO;
	}

protected:
	int		m_iSendBufferPosition;		//
	int		m_iSendBufferSize;

	CIO_UNIT *		m_pBufferUnit;

protected:
	CRITICAL_SECTION	m_csSendBuffer;
};


// �Ľ̵� ����Ÿ�� �޾ƿ´�.
struct __mid_packet_unit
{
	int		length;
	char *	packet;
};


class CRecvBuffer
{
public:
	CRecvBuffer() : m_iCurStartPos(0), m_iCurEndPos(0)
	{
		ZeroMemory( &m_wsaBuf, sizeof(m_wsaBuf) );
		ZeroMemory( m_cbRecvBuffer, sizeof(m_cbRecvBuffer) );
	}

	void NoticeRecv( int RecvLength );
	bool GetPacket( __mid_packet_unit& unit );	// false �� ������ ��� �޾ƿ´�.


	char * GetEndPos()	{ return &m_cbRecvBuffer[m_iCurEndPos]; }
	int GetLeftBufferLength()	{ return (MAX_OF_RECV_BUFFER-m_iCurEndPos); }
	WSABUF * GetWSABuf()	
	{
		m_wsaBuf.buf = GetEndPos();
		m_wsaBuf.len = GetLeftBufferLength();
		ZeroMemory( m_wsaBuf.buf, m_wsaBuf.len );
		return &m_wsaBuf;
	}

protected:
	int			m_iCurStartPos;		// 
	int			m_iCurEndPos;

	WSABUF		m_wsaBuf;			// 

	char		m_cbRecvBuffer[MAX_OF_RECV_BUFFER];
};


class CConnectionObject
{
public:
	CConnectionObject( CSendBuffer& sendbuf) 
		: m_iConIndex(0), m_iConState(FALSE), 
		m_sockConnect(INVALID_SOCKET), m_refSendBuffer(sendbuf),
		m_iLastError(0)
	{
		ZeroMemory(m_ip, sizeof(m_ip));
		m_port = 0;

		::ZeroMemory( &m_ovRecv, sizeof(m_ovRecv) );
		m_ovRecv.m_iMode = iomode_recv ;
		m_ovRecv.m_dwFlag = 0 ;
	}

public:
	void Connect( char * ip, unsigned short port );		
	void SendRequest( void * data, u_short length );	// Request Sending Data.
	BOOL RecvRequest();	
	BOOL ParseAndProcess( int recvbytes );
	void Close();

public:
	inline int GetIndex()			{ return m_iConIndex; }
	inline void SetIndex( int idx )	{ m_iConIndex = idx; }

	inline int GetState()			{ return m_iConState; }
	inline void SetState( int state )	{ m_iConState = state; }

	inline SOCKET GetSocket()		{ return m_sockConnect; }
	inline void SetSocket( SOCKET s )	{ m_sockConnect = s; }

	inline char * GetIP()			{ return m_ip ; }
	inline unsigned short GetPort()	{ return m_port ; }

	inline int GetError()			{ return m_iLastError; }

protected:
	int				m_iConIndex;
	int				m_iConState;		// TRUE/FALSE : Connection State. Check this before send.
	int				m_iLastError;		// WSAGetLastError()

	SOCKET			m_sockConnect;		// Connected Socket ID

	char			m_ip[16];			// Target Information
	unsigned short	m_port;				//

	CRecvBuffer		m_recvBuffer;

	CIO_UNIT		m_ovRecv;
	CIO_UNIT		m_ovSend;

protected:	// Send Member
	CSendBuffer &	m_refSendBuffer;
};


class CConnectionManager
{
public:
	enum
	{
		en_work_mode_ds=0,		// ds���� ������ ��.
		en_work_mode_gms,		// gms���� ������ ��.
	};

	CConnectionManager()
	{}

	~CConnectionManager()
	{
		if( m_ConList.size() > 0 )
		{
			list<CConnectionObject *>::iterator itr = m_ConList.begin();
			while( itr != m_ConList.end() ) 
			{
				delete *itr;
				itr++;
			}
			m_ConList.clear();		// ���ص� �ȴ�. 
		}

		if( m_pSendBuffer )
		{
			delete m_pSendBuffer;
			m_pSendBuffer = NULL;
		}

		DeleteCriticalSection( &m_csLock );
	}

	// �� �Լ��� �� �����ؼ� ������. 
	bool InitConnectionManager( int workmode, int bufsize, char * ip, unsigned short port );

	// GMS
//	void ConnectAll( CDSInformation& dsinfo );
	void StartRecv();
	// ������ �ȵ� ��ü�� �ִٸ�, �ٽ� �õ��� ����. 
//	bool RetryConnect_NotConnectedObject();
//	void GMSRecv();


	// DS
	void StartAccept( char * ip, unsigned short port );
	void DSAccept();
	void DSRecv();

	void AddConObject( CConnectionObject * pCon )
	{
		bool bFind = false;

		EnterCriticalSection( &m_csLock );
		list<CConnectionObject *>::iterator itr = m_ConList.begin();


		while( itr != m_ConList.end() )
		{
			if( pCon == *itr )
			{
				bFind = true;
				break;
			}
			itr++;
		}

		if( !bFind )	// ������ �����Ѵ�.
			m_ConList.push_back( pCon );
		LeaveCriticalSection( &m_csLock );
	}
	CConnectionObject * GetConObject()
	{
		CConnectionObject * pCon = NULL;

		EnterCriticalSection( &m_csLock );

		if( m_ConList.size() > 0 )
		{
			pCon = m_ConList.front();
			m_ConList.pop_front();
		}
		else
		{
			try
			{
				pCon = new CConnectionObject( *m_pSendBuffer );
			}
			catch( ... )
			{
				ShowMsg( "[Connection Object] Create Fail" );
			}
		}
		LeaveCriticalSection( &m_csLock );
		return pCon;
	}

protected:
	// Recv Thread
	static unsigned int __stdcall th_GMSRecvThread( LPVOID lp );
	// Accept Thread
	static unsigned int __stdcall th_GMSAcceptThread( LPVOID lp );

public:	// Send
	void SendTo( int index, void * data, u_short length );

	CConnectionObject * FindConnection( int index );

public:
	// ���� ���¸� ������, ���� �Լ��� ȣ���Ѵ�. 
	// �� �Ʒ��� �� �Լ��� �ܼ��� ���� ������ ����� ���� ���� �� ���̴�. 
	void PrintState();
protected:
	void PrintStateToConsole();
	void PrintStateToMSGWIN();

protected:
	list<CConnectionObject*>	m_ConList;	
	CNDIOCPObejct				m_IOCP;		// IOCP Object
	CSendBuffer	*				m_pSendBuffer;

	SOCKET						m_sockAccept;	// Accept Socket

	char						m_AcceptIP[16];
	unsigned short				m_AcceptPort;

	int							m_iWorkMode;	// work mode

	bool						m_bAcceptThreadState;
	bool						m_bRecvThreadState;

	HANDLE						m_hInitEvent;

	CRITICAL_SECTION			m_csLock;
};
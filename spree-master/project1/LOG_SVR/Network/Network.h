#pragma once

#ifndef _NETWORK_H_AWEGFASDFAWE
#define _NETWORK_H_AWEGFASDFAWE


#define _MAX_CONNECTION_					100	// �α׼����� ����Ǵ� �ִ�


#define _RECV_BUF_SIZE_			1048576 * 2



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

class _h_Recv_Overlapped : public _h_Overlapped
{
public:
	int			iBufSize ;
	char *		pRealBuf ;
	int			RemainBytes ;
	
	// �߰�

	SOCKET		m_socket;					// ����		
	
	CRITICAL_SECTION m_csRecvOV ;		// ���� ����Ÿ ��û�� ����.
	   
    
	//char		m_buf[_RECV_BUF_SIZE_] ;	// ���۷� ����� ����.
	char *		m_pBuf ;
	
public:
	_h_Recv_Overlapped()	
	{ 
		Internal			= InternalHigh	= 0;	
		Offset				= OffsetHigh	= 0;
		hEvent				= NULL;		
		m_pBuf				= NULL ;

		RemainBytes = 0 ;		// Add...
		m_dwFlag = 0 ;
		m_bMode = true ;	
		
		m_pDSComm = NULL;	
		
		if( InitializeCriticalSectionAndSpinCount( &m_csRecvOV, (0xf0000000)|2000 )  == FALSE ) {
			printf( "INIT - Recv OV Critical section : Create Critical Section Failed\n" ) ;		
		}	
	}
	~_h_Recv_Overlapped()	{
		DeleteCriticalSection( &m_csRecvOV ) ;
		if( m_pBuf )
		{
			delete [] m_pBuf ;
		}
		
	} 
	
	bool CreateRecvOV( int size ) {
		iBufSize = size ;
		
		m_pBuf = NULL;		
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
	
	// Add jk.Hong
	bool Parsing(void * pPacketQ, DWORD bufLen);
	
	inline void Lock() { EnterCriticalSection(&m_csRecvOV); }
	inline void UnLock() { LeaveCriticalSection(&m_csRecvOV); }
} ;




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


#define _DS_SEND_BUFFER_SIZE_			4096	//Send �� ����Ÿ�� ����ϴ� ����..

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
	
	CRITICAL_SECTION g_csSendData ;		// ���� ����Ÿ ��û�� ����.

	/*
	���� 2���� ����, ó�� ��û�� ������, 1�� ���ۿ� ����, �ٷ� ������. 
	�ٽ� ��û�� ������, 1�� ������ �۾��� ������ ���, 2�� ���ۿ� ���� ������. 
	1�� �۾��� ���� ������ �ʾҴٸ�, �׳� 2�� ���ۿ� �ű�⸸ �ϰ�, ������ �ʴٰ�
	1�� �۾��� ������ �� 2�� ������ ������ ������, ������ ��û�� ������ 1�� ���ۿ�
	���� �ִٰ� 2�� �۾��� ������ ��, 1���� ������. �̷� ������ ������ ���� ���۸� ������. 
	���� ���� ����Ÿ�� ������ ��� ����. 
	*/
	int				m_iBuf_1_size ;		// ���� 1�� ����Ÿ ���� : 
	int 			m_iBuf_2_size ;		// ���� 2�� ����Ÿ ����
	int				m_i1_write ;		// ���� 1 ������ ����Ʈ �� : send ���Ŀ� ����
	int				m_i2_write ;		// ���� 2 ������ ����Ʈ ��

	char			m_buf_1[_DS_SEND_BUFFER_SIZE_] ;		// 64kbytes ����
	char			m_buf_2[_DS_SEND_BUFFER_SIZE_] ;

	bool			m_bUse ;				// �۾����̳� �ƴϳ�
	bool			m_bSelect ;				// false = 1, true = 2 ; 

	
public:
	
	//char			m_cCharacName[13] ;	

	SOCKADDR_IN		m_sockAddr ;	


	_h_Recv_Overlapped  m_RecvOV ;
	_h_Overlapped		m_SendOV ;				// send �� ���� OV : ����� ��ġ�� �ʴ´�. �׷��� �� �ϳ�.


	// ���� ������ �ִ� ���� �Ǵ� �ٷ� ������ ���´� ����.

	_h_DS_Com() {

		memset( &m_RecvOV, 0, sizeof(_h_Recv_Overlapped) ) ;	
		memset( &m_SendOV, 0, sizeof(_h_Overlapped) ) ;	
		m_dssock = 0 ;

		m_iBuf_1_size = 0 ;
		m_iBuf_2_size = 0 ;
		m_i1_write = 0 ;
		m_i2_write = 0 ;

		memset( m_buf_1, 0, _DS_SEND_BUFFER_SIZE_ ) ;
		memset( m_buf_2, 0, _DS_SEND_BUFFER_SIZE_ ) ;	
		m_bUse = false ;
		m_bSelect = false ;
	}

	void init() 
	{
		m_dssock = 0 ;
		
		m_bUse = false ;
		m_bSelect = false ;
	}

	SOCKET get_sock() { return m_dssock ; }		

	bool createDSCom() ;		// 

	void ConnectCom( SOCKET gsSock) ;
	void DisConnectCom() ;


	void releaseDSCom() ;		//
	int send_request( u_short size, char * pdata ) ;	// 0 : ����  1 : ���ۿ���  2 : send error
	bool send_complete( int iwrite ) ;		// �ٷ� ���� send �� ���� ����Ÿ ��.
	void CloseSock() { 
		shutdown( m_dssock, SD_BOTH ) ;
		closesocket( m_dssock ) ; 
	}
	

} ;

class _DSCom_Queue
{
	
protected:	
	_h_DS_Com	m_DSCom[_MAX_CONNECTION_] ;
	
	
public:
	
	
	bool Init( )
	{
		for( int i = 0 ; i < _MAX_CONNECTION_ ; i++ )
		{
			if( !m_DSCom[i].createDSCom() ) 
			{
				return false ;
			}			
		}
		return true ;
	}
	
	_h_DS_Com * GetDSCom( )
	{
		for( int i = 0 ; i < _MAX_CONNECTION_ ; i++ )
		{
			if( m_DSCom[i].get_sock() == 0 ) {
				m_DSCom[i].init() ;
				return &m_DSCom[i] ;
			}
		}
		return NULL ;
	}
	
	
};


#endif
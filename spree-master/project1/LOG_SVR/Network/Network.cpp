#include "..\stdafx.h"
#include "Network.h"
#include "..\VMManager\VMManager.h"
#include "..\Queue\packet_queue.h"



//#################################################################################
//
//	_h_Overlapped : �������� ����ü. 
//
//###################################################################### LHS Added....
void _h_Recv_Overlapped::reset()
{
	Internal			= InternalHigh	= 0;	
	Offset				= OffsetHigh	= 0;
	hEvent				= NULL;
	memset( m_pBuf, 0, iBufSize ) ;
	m_wsabuf.buf = m_pBuf ;
	pRealBuf = m_pBuf ;		// Add...
	RemainBytes = 0 ;		// Add...
	m_dwFlag = 0 ;
	m_bMode = true ;
}

void _h_Recv_Overlapped::move( int remain ) 
{
	try {
		RemainBytes = remain ;
		memmove( m_pBuf, pRealBuf, RemainBytes ) ;
		pRealBuf = m_pBuf;
	}
	catch ( ... ) {
#ifdef _TEST_
		::PrintConsole( "[EXCEPTION] Recv Overlapped::move\n" ) ;
#endif
		reset() ;
	}
}

// Send Request

// Recv Request
bool _h_Recv_Overlapped::RecvPacket( const SOCKET sock )
{
	int err ;
	
	DWORD len = 0 ;
	__try {
		m_wsabuf.len = iBufSize - RemainBytes ;
		m_wsabuf.buf = m_pBuf + RemainBytes ;
		memset( m_wsabuf.buf, 0, m_wsabuf.len ) ;
		m_bMode = true ;
	}
	__except ( EXCEPTION_EXECUTE_HANDLER ) {
		reset() ;
		return false ;
	}
	if( SOCKET_ERROR == WSARecv( sock, &m_wsabuf, 1, &len, &m_dwFlag, (LPWSAOVERLAPPED)this, NULL ) )
	{
		err = WSAGetLastError() ;
		if( err == WSA_IO_PENDING || err == WSAEWOULDBLOCK )
		{
			return true ;
		}
		else
		{
			return false ;
		}
	}
	
	return true ;
}

bool _h_Recv_Overlapped::Parsing(void  * _pPacketQ, DWORD bufLen)
{

	_queue_Packet	* pPacketQ;
	pPacketQ = (_queue_Packet *)_pPacketQ;

	short	head;
	int	copiedBytes = 0;		//ī�ǵ� ����Ÿ ����(��� ī�ǰ� �̷�������� ���θ� �Ǵ�..)
	bool state = true;			// false �̸� while loop �� �������´�..
	int availableBytes = 0;		//��Ŷ�� ���� HEADER_LEN + BODY(head.sLength)	
	int HEADER_LEN = sizeof(head);

	RemainBytes += bufLen;		//���� �̿ϵǾ�� ����Ÿ ���� + ���ι��� ����Ÿ����
	pRealBuf = m_pBuf;	//����Ŀ���� �� ������..

	while( state )	//������ ����Ÿ�� 0���� Ŭ������..
	{
		/*******************************************************/
		/* Header Empty */
		/*******************************************************/
		if( copiedBytes == 0 )	//ī�ǵ� ����Ʈ�� �������
		{
			if( RemainBytes >= HEADER_LEN )	// ������̸�ŭ�� �̴´�.
			{				
				memcpy(&head, pRealBuf, HEADER_LEN);	//��� ���̸�ŭ ����Ÿ�� �̾Ƴ���..				
				copiedBytes += HEADER_LEN ;	//ī�ǵ� ����Ÿ ����+�̾Ƴ� ����Ÿ ����				
				continue;
			}
			else	// ������ ������̺��� �������.. ���� ť�� �̷��..
			{
				try {
					memmove(m_pBuf, pRealBuf, RemainBytes);				
					pRealBuf = m_pBuf + RemainBytes;
					state = false;				
				}
				catch (...) {
					::PrintConsole( "[EXCEPTION] memmove : parsinglength = %d\n", RemainBytes ) ;
					return false ;
				}				
			}
		}

		/*******************************************************/
		/* Header FULL */		//�ɰ����� �ʰ� ���̴� �� ����..
		/*******************************************************/		
		else if (copiedBytes >= HEADER_LEN )
		{
			availableBytes = head;	// ����� �����ؼ� �ٵ���� ��Ŷ �ϳ�..
			if( (RemainBytes >= availableBytes) && ( 0 < availableBytes ) )	// �������� �����纸�� ũ�ų� ������..
			{					
				
				pPacketQ->insert(this, availableBytes);
												

				pRealBuf += availableBytes ;			// �����Ѹ�ŭ �޸� ������ �����ϳ�..
				RemainBytes -= availableBytes ;		// ������ ����Ÿ�� �����Ѹ�ŭ ����..
				copiedBytes = 0 ;				
				if(RemainBytes == 0){
					state = false;
				}
			}
			else	// �������� �����纸�� �������ϱ� ����Ÿ �ٵ� ©�� ���ڴ�..
			{				
				try {
					memmove(m_pBuf, pRealBuf, RemainBytes);				
					pRealBuf = m_pBuf + RemainBytes;
					state = false;				
				}
				catch (...) {
					::PrintConsole( "[EXCEPTION] memmove : parsinglength = %d\n", RemainBytes ) ;
					return false ;
				}				
			} // end else
		}// end else if

	} /* End While*/
	return true;
} // end parsing()




//#################################################################################
//
//	_h_CompletionPort ������
//
//###################################################################### LHS Added....
bool _h_CompletionPort::Create( int n )
{
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, n);

	return (m_hIOCP != NULL) ;
}

bool _h_CompletionPort::AssociateSocket( _h_Recv_Overlapped * pOv )
{	
	return (CreateIoCompletionPort((HANDLE)pOv->m_socket, m_hIOCP, (DWORD)pOv,0) == m_hIOCP);
}

bool _h_CompletionPort::GetStatus(DWORD * pCompKey, PDWORD pdwNumBytes, WSAOVERLAPPED** ppo, DWORD dwMilliseconds)
{
	return (GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, pCompKey, ppo, dwMilliseconds) == TRUE);
}


/*#################################################################################

DS_Com

#################################################################################*/


int _h_DS_Com::send_request( u_short size, char * pdata )
{
	int iRet =0;
	int iError =0;

	EnterCriticalSection( &g_csSendData ) ;

	try {
		if( m_bSelect ) 
		{	
			// true ���, 2�� ���۸� ������ ���̹Ƿ� 1 �� ���ۿ� ����.
			// 			
			if( m_iBuf_1_size + size > _DS_SEND_BUFFER_SIZE_ )
				iRet = 1 ;
			else 
			{
				memcpy( &m_buf_1[m_iBuf_1_size], pdata, size ) ;
				m_iBuf_1_size += size ;

				if( !m_bUse ) 
				{	// ���� �۾��� ���� ���̹Ƿ�, �ٷ� ������. 
					// send �Ѵ�.
					m_SendOV.m_bMode = false ;		// send = false 
					m_SendOV.m_wsabuf.buf = m_buf_1 ;
					m_SendOV.m_wsabuf.len = m_iBuf_1_size ;//m_i1_write ;
					m_i1_write = 0 ;

					if( SOCKET_ERROR == WSASend( m_dssock, &m_SendOV.m_wsabuf, 1, &m_SendOV.m_dwFlag, 0, (LPWSAOVERLAPPED)&m_SendOV, NULL ) ) 
					{
						iError = WSAGetLastError();
						if( WSAEWOULDBLOCK == iError || ERROR_IO_PENDING == iError) 
						{
							iRet = 0 ;
							m_bUse = true ;			// 
							m_bSelect = false ;
						}
						else {
							iRet = 2 ;
						}
					}
					else		// �ٷ� �����ߴ�. �׷��� wouldblock�� �����ϰ� ó��.
					{
						iRet = 0 ;
						m_bUse = true ;			// 
						m_bSelect = false ;
					}
				}
			}
		}
		else 
		{			
			// false ���, 1�� ���۸� ������ ���̹Ƿ� 2 �� ���ۿ� ����.
			// 
			if( m_iBuf_2_size + size > _DS_SEND_BUFFER_SIZE_ )
				iRet = 1 ;
			else 
			{
				memcpy( &m_buf_2[m_iBuf_2_size], pdata, size ) ;
				m_iBuf_2_size += size ;

				if( !m_bUse ) 
				{	// ���� �۾��� ���� ���̹Ƿ�, �ٷ� ������. 
					// send �Ѵ�.
					m_SendOV.m_bMode = false ;		// send = false 
					m_SendOV.m_wsabuf.buf = m_buf_2 ;
					m_SendOV.m_wsabuf.len = m_iBuf_2_size ;
					m_i2_write = 0 ;

					if( SOCKET_ERROR == WSASend( m_dssock, &m_SendOV.m_wsabuf, 1, &m_SendOV.m_dwFlag, 0, (LPWSAOVERLAPPED)&m_SendOV, NULL ) ) 
					{						
						iError = WSAGetLastError();
						if( WSAEWOULDBLOCK == iError || ERROR_IO_PENDING == iError) 
						{
							iRet = 0 ;
							m_bUse = true ;			// 
							m_bSelect = true ;
						}
						else {
							iRet = 2 ;
						}
					}
					else		// �ٷ� �����ߴ�. �׷��� wouldblock�� �����ϰ� ó��.
					{
						iRet = 0 ;
						m_bUse = true ;			// 
						m_bSelect = true ;
					}
				}
			}
		}
	}

	catch(...) {
		LeaveCriticalSection( &g_csSendData ) ;
		::PrintConsole("[EXCEPTION] send_request() \n");

	}
	LeaveCriticalSection( &g_csSendData ) ;

	return iRet ;
}

/* =====================================================================================
complete �Լ��� �ҷ��� ��, m_bUse �� false �ϸ� ����. 
===================================================================================== */
bool _h_DS_Com::send_complete( int iwrite )
{
	bool bRet =0;		// false �̸� send ����.
	int	iError =0;

	// ���� ó�� ������...

	EnterCriticalSection( &g_csSendData ) ;

	try {
		if( m_bSelect ) 
		{		
			m_i2_write += iwrite ;
			if( m_i2_write == m_iBuf_2_size ) 
			{
				m_iBuf_2_size = 0 ;
				m_i2_write = 0 ;
				m_bUse = false ;			// ��볡.
				bRet = true ;
			}
			else 
			{		// ���� �� �� ���´�. �ٽ� �ѹ� ������. 
				//m_i2_write -= iwrite ;
				m_SendOV.m_wsabuf.buf = &m_buf_2[m_i2_write] ;
				m_SendOV.m_wsabuf.len = m_iBuf_2_size - m_i2_write ;

				if( SOCKET_ERROR == WSASend( m_dssock, &m_SendOV.m_wsabuf, 1, &m_SendOV.m_dwFlag, 0, (LPWSAOVERLAPPED)&m_SendOV, NULL ) ) 
				{
					iError = WSAGetLastError();
					if( WSAEWOULDBLOCK == iError || WSA_IO_PENDING == iError ) 
					{
						bRet = true ;
					}
					else {
						bRet = false ;
					}
				}
			}

		}
		else 
		{		
			m_i1_write += iwrite ;
			if( m_i1_write == m_iBuf_1_size ) 
			{
				m_iBuf_1_size = 0 ;
				m_i1_write = 0 ;
				m_bUse = false ;			// ��볡.
				bRet = true ;
			}
			else 
			{		// ���� �� �� ���´�. �ٽ� �ѹ� ������. 
				//m_i1_write -= iwrite ;
				m_SendOV.m_wsabuf.buf = &m_buf_1[m_i1_write] ;
				m_SendOV.m_wsabuf.len = m_iBuf_1_size - m_i1_write ;

				if( SOCKET_ERROR == WSASend( m_dssock, &m_SendOV.m_wsabuf, 1, &m_SendOV.m_dwFlag, 0, (LPWSAOVERLAPPED)&m_SendOV, NULL ) ) 
				{
					iError = WSAGetLastError();
					if( WSAEWOULDBLOCK == iError || WSA_IO_PENDING == iError ) 
					{
						bRet = true ;
					}
					else {
						bRet = false ;
					}
				}
			}
		}
	}

	catch (...) {
		LeaveCriticalSection( &g_csSendData ) ;
		::PrintConsole("[EXCEPTION] send_request() \n");
	} 

	LeaveCriticalSection( &g_csSendData ) ;




	return bRet ;

}

/* =====================================================================================
createDSCom() : ����
===================================================================================== */
bool _h_DS_Com::createDSCom( )
{
	if( InitializeCriticalSectionAndSpinCount( &g_csSendData, (0xf0000000)|2000 )  == FALSE ) {
		printf( "INIT - DS_Com : Create Critical Section Failed\n" ) ;
		return false ;
	}	


	// Recv ov create
	if( false == m_RecvOV.CreateRecvOV( _RECV_BUF_SIZE_ ) )
	{
		printf( "InnerCom::Recv OV Create Failed\n" ) ;
		return false ;
	}		

	return true ;
}


/* =====================================================================================
releaseDSCom() : ����
===================================================================================== */
void _h_DS_Com::releaseDSCom()
{
	try{
		if( m_dssock ) {
			shutdown(m_dssock, SD_BOTH) ;
			closesocket( m_dssock ) ;
			m_dssock = NULL ;
		}
				
		DeleteCriticalSection(&g_csSendData);		
	}
	catch ( ...	) {
		::PrintConsole("[EXCEPTION] void _h_DS_Com::releaseDSCom() \n ") ;
	}	

}



void _h_DS_Com::ConnectCom( SOCKET gsSock )
{
	m_dssock = gsSock;
	m_RecvOV.m_socket = gsSock;	
	m_RecvOV.m_pDSComm = this;	
	m_SendOV.m_pDSComm = this;
}

/* =====================================================================================
releaseDSCom() : ����
===================================================================================== */
void _h_DS_Com::DisConnectCom()
{
	try{
		m_RecvOV.reset() ;
		if( m_dssock ) {
			closesocket( m_dssock ) ;
			m_dssock = NULL ;
		}
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION]  _h_DS_Com::releaseDSCom \n") ;
	}	
	
}
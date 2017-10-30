/*
TCP
	- server -
	CreateTcpSocket();
	Bind(server_port);
	Listen();
	Accept(pClientSock); // pClientSock->addr ���� ä������.
	Recv()/Send();
	CloseSocket();

	- client -
	CreateTcpSocket();
	Connect(server_ip, server_port);
	Recv()/Send();
	CloseSocket();

UDP
	CreateUdpSocket();
	Bind(SERV_PORT);
	RecvFrom(...)/SendTo(...);
	CloseSocket();
*/

#ifndef _SOCK_
#define _SOCK_

/* gGlobal.h�� include �Ǿ� ����
#ifdef INCLUDE_NETWORK_BASE_HEADER
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#endif
*/

#ifndef WINSOCK_LIB_EXPORTS // for DLL
#ifndef LINK_WINSOCK_LIB
#define LINK_WINSOCK_LIB
#pragma comment(lib, "ws2_32.lib")
#endif
#endif

#define IPv4	// 32bit: 8��Ʈ�� 4�κ� 10������ ex)202.30.64.22
//#define IPv6	// 128bit: 16��Ʈ�� 8�κ� 16������ ex)2001:0230:abcd:ffff:0000:0000:ffff:1111
#define IP_BUF_SIZE		16

//struct sockaddr_in	addr; // = SOCKADDR_IN
//struct sockaddr_in6	addr; // = SOCKADDR_IN6
//struct in_addr	ip; // = IN_ADDR
//struct in6_addr	ip; // = IN6_ADDR

class CSock {
public:
	static int iSockDllRefCnt;
protected:
	SOCKET hSock;
	SOCKADDR_IN addr;

	/* addrX�� Ȱ��ó ====================
	CSock Ŭ������
		TCP Server�� Server�� ���Ǿ��� ��� �� ���� Accept ���� ����(���� ��Ʈ)
		TCP Server�� Client�� ���Ǿ��� ��� �� Ŭ���̾�Ʈ�� ���� ���� ����(Ŭ���̾�Ʈ ������, Ŭ���̾�Ʈ�� ���ӵ� ������ ��Ʈ)
		TCP Client�� ���Ǿ��� ��� �� ������ ���� ���� ����(���� ������, ������ ���� ��Ʈ)
		UDP Recver�� ���Ǿ��� ��� �� ������ ����. sendto()�� ���� peer ������ ������ ����
		UDP	Sender�� ���Ǿ��� ��� �� ������ ����. peer ������ ��Ƽ������ ȯ���� ����� �������� ���
	===================================== */

	// ��� ������.. ���Ǽ��� ����(�α� ��½ÿ� ���� ��)
	TCHAR pszIP[IP_BUF_SIZE];

public:
	CSock();
	virtual ~CSock();
	BOOL Init();
	BOOL Finish();

public:
	BOOL CreateTcpSocket();
	BOOL CreateUdpSocket();
	BOOL Shutdown(IN int iFlag);
	void CloseSocket();
	BOOL Bind(IN USHORT usPort);
	BOOL Listen(IN int iBackLog = 100/*SOMAXCONN*/);
	BOOL Accept(OUT CSock* pClientSock);
	BOOL Connect(IN ULONG ulIP, IN USHORT usPort);		// ����ӵ� ���� ����
	BOOL Connect(IN LPCTSTR pszIP, IN USHORT usPort);	// ����ӵ� �߰�

	// for TCP
	int		Recv(OUT char* pRecvBuf, IN int iBufLen);
	int		Send(IN const char* pSendBuf, IN int iSendDataLen);
	
	// for UDP
	int		RecvFrom(OUT char* pRecvBuf, IN int iBufLen, OUT SOCKADDR_IN& addr);
	int		SendTo(IN char* pSendBuf, IN int iSendDataLen, IN SOCKADDR_IN& addr);				// ����ӵ� ���� ����
	int		SendTo(IN char* pSendBuf, IN int iSendDataLen, IN ULONG ulIP, IN USHORT usPort);	// ����ӵ� �߰�
	int		SendTo(IN char* pSendBuf, IN int iSendDataLen, IN LPCTSTR pszIP, IN USHORT usPort);	// ����ӵ� ���� ����

public: // Get/Set Methods
	SOCKET	GetSock() { return hSock; }
	void	SetSock(SOCKET s) { hSock = s; }
	LPCTSTR	GetIP();
	USHORT	GetPort();
	SOCKADDR_IN&	GetSockAddr() { return addr; }
	void			SetSockAddr(SOCKADDR_IN* pAddr) { memcpy(&addr, pAddr, sizeof(SOCKADDR_IN)); }
};

void	Domain2StringIP(IN LPCTSTR pszDomain, OUT LPTSTR pszIP, IN int iIPBufSize);
void	NumericIP2StringIP(IN ULONG ulIP, OUT LPTSTR pszIP, IN int iIPBufSize);
ULONG	StringIP2NumericIP(IN LPCTSTR pszIP);

#endif //_SOCK_

#ifndef _GLOBAL_SERVERINFO
#define _GLOBAL_SERVERINFO


#include "Global_struct.h"


// GS ���� ����
#define GS_NONSTATE							0		// ��������
#define GS_OK								1		// �������
#define GS_OVER								3		// ��ȭ
#define GS_ERROR							4		// ������ ����


// Forest state
#define  _FOREST_STATE_RUNNING				0					// ���� �۵���
#define  _FOREST_STATE_OFF					-1					// ���� ����
#define  _FOREST_STATE_CHECKUP_				-2					// ������ ������
#define  _FOREST_STATE_FULL_				-3					// ��ȭ����



class _j_ServerInfo
{

public:		

	_SERVER_SYSTEM_INFO		m_ServerSystemInfo[GS_MAX];
	_SERVER_NET_INFO		m_ServerNetInfo[GS_MAX];
	_SERVER_STATE_INFO		m_ServerStateInfo[GS_MAX];

	int						m_iForestUserCount ;
	int						m_iLastRBServer ;			// GS �κ� round robin ���� ���������� �����ϰ� �ϱ� ���ؼ�..
	char					m_cForestState ;

	u_short					m_usDSAcceptPort;		// Game Server �� ��ٸ��� Accept Port
	u_short					m_usDSUdpPort;			// DS �� UDP Recv Port
	u_short					m_usDS_GMSAcceptPort;	// GMS �� ��ٸ��� Accept Port

	u_short					m_usLoginUdpPort ;		// LS �� Recv Port ��ȣ

	char 					m_cGSAcceptIP[16];		//
	char					m_cGMSAcceptIP[16];		//

	char 					m_cMessangerIP[16] ;	//
	u_short					m_usMessangerPort ;		//

	SOCKADDR_IN				m_sockaddrMS ;			// 

	int						m_iServiceMode;			// ���� ���� ��� : EN_ND_SERVICE_MODE
	int						m_iBillingUse;			// Default : TRUE(use), FALSE(not use)

	int GetForestUserCount() { return m_iForestUserCount ;}
	void GetEachServerConPlayer( MSG_GM_SERVER_PLAYER * pPlayer )
	{
		for( int i = 1; i < GS_MAX; ++i )
		{
			pPlayer->Count[i].usConnectCount = static_cast<u_short>(m_ServerStateInfo[i].iConnectCount);
			pPlayer->Count[i].usPlayerCount = static_cast<u_short>(m_ServerStateInfo[i].iPlayingCount);
		}
	}

public:

	inline void SetForestState( char state ) { m_cForestState = state ; } //enum _FOREST_STATE
	inline char GetForestState() { return m_cForestState ; }	//enum _FOREST_STATE

	_j_ServerInfo() : m_cForestState(_FOREST_STATE_RUNNING), m_iLastRBServer(0), m_iForestUserCount(0)
	{
		memset(m_ServerSystemInfo, 0, sizeof(_SERVER_SYSTEM_INFO)*GS_MAX );
		memset(m_ServerNetInfo, 0, sizeof(_SERVER_NET_INFO)*GS_MAX );
		memset(m_ServerStateInfo, 0, sizeof(_SERVER_STATE_INFO)*GS_MAX );		

		memset(&m_sockaddrMS, 0, sizeof(m_sockaddrMS) ) ;
	}

	bool	Init( const char * const fileName, const int forestNum );

	inline u_short GetDSAcceptPort()	{ return m_usDSAcceptPort ; }
	inline u_short GetDSUdpPort()		{ return m_usDSUdpPort ; }

	inline u_short GetLSUdpPort()		{ return m_usLoginUdpPort ; }
	inline u_short GetGMSAcceptPort()	{ return m_usDS_GMSAcceptPort; }

	inline char * GetGSAcceptIP()		{ return m_cGSAcceptIP; }
	inline char * GetGMSAcceptIP()		{ return m_cGMSAcceptIP; }

	int get_service_mode()				{ return m_iServiceMode; }
	int get_billing_use()				{ return m_iBillingUse; } // true: �������, false: ����������
	
	//inline char * GetMSUdpIP()			{ return m_cMessangerIP ; }
	//inline u_short GetMSUdpPort()		{ return m_usMessangerPort ; }

	SOCKADDR_IN *	GetMSSockAddrIn()	{ return &m_sockaddrMS ; }

	inline void	AddServer(char cNum){	
		if ( GS_MAX < cNum ) {
			::PrintConsole("[MEM_ERR] 'inline void	AddServer(char cNum)'���� ������ �ʰ� (value : %d)\n", cNum);
			return ;
		}
		m_ServerStateInfo[cNum].cState = GS_OK;		
	}
	inline void	UpdateConnectCount(char cNum, int i){
		if (  GS_MAX < cNum ) {
			::PrintConsole("[MEM_ERR] 'inline void	UpdateInCount(char cNum, int i)'���� ������ �ʰ� (value : %d)\n", cNum);
			return ;
		}
		m_ServerStateInfo[cNum].iConnectCount = i;

		// ��Ż ī��Ʈ�� ����Ѵ�.
		m_iForestUserCount = 0 ;
		for( int i = 0 ; i < GS_MAX ; i++)
		{
			m_iForestUserCount = m_iForestUserCount + m_ServerStateInfo[i].iPlayingCount ;
		}

	}
	inline void	UpdatePlayCount(char cNum, int i){
		if (cNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline void	UpdateOutCount(char cNum, int i)'���� ������ �ʰ� (value : %d)\n", cNum);
			return ;
		}
		m_ServerStateInfo[cNum].iPlayingCount = i;
	}
	inline void UpdateState(char cNum, char state, int con, int ply){
		if (cNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline void UpdateState(char cNum, char state, int con, int ply)'���� ������ �ʰ� (value : %d)\n", cNum);
			return ;
		}
		m_ServerStateInfo[cNum].cState = state;
		m_ServerStateInfo[cNum].iConnectCount = con;
		m_ServerStateInfo[cNum].iPlayingCount = ply;
	}
	inline void UpdateInfo(char cNum, void * pServerInfo){        
		if (cNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline void UpdateInfo(char cNum, void * pServerInfo)'���� ������ �ʰ� (value : %d)\n", cNum);
			return ;
		}
		m_ServerStateInfo[cNum].iConnectCount = ((MSG_SYSTEM_INFO_RES *)pServerInfo)->_ServerStateInfo.iConnectCount;
		m_ServerStateInfo[cNum].iPlayingCount = ((MSG_SYSTEM_INFO_RES *)pServerInfo)->_ServerStateInfo.iPlayingCount;

	}

	inline int	SelectConCount(short sNum) { 
		if (sNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline int	SelectConCount(char cNum)'���� ������ �ʰ� (value : %d)\n", sNum);
			return -1;
		}
		return m_ServerStateInfo[sNum].iConnectCount; 
	}
	inline int	SelectPlyCount(short sNum) { 
		if (sNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline int	SelectPlyCount(char cNum)'���� ������ �ʰ� (value : %d)\n", sNum);
			return -1;
		}
		return m_ServerStateInfo[sNum].iPlayingCount; 		
	}	
	inline char SelectState(short sNum) {
		if (sNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline short SelectState(char cNum)'���� ������ �ʰ� (value : %d)\n", sNum);
			return -1;
		}
		return m_ServerStateInfo[sNum].cState; 
	}
	inline _SERVER_SYSTEM_INFO * SelectServerInfo(short sNum) { 
		if (sNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline _SERVER_SYSTEM_INFO * SelectServerInfo(char cNum)'���� ������ �ʰ� (value : %d)\n", sNum);
			return NULL;
		}
		return &m_ServerSystemInfo[sNum]; 
	}
	inline SOCKADDR_IN * GetSockAddr(short sNum) {
		if (sNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline SOCKADDR_IN * GetSockAddr(char cNum)'���� ������ �ʰ� (value : %d)\n", sNum);
			return NULL;
		}
		return &m_ServerNetInfo[sNum].sockAddr ; 
	}

	inline _h_DS_Com * GetDSCom( short sNum )
	{
		if (sNum > GS_MAX) {
			::PrintConsole("[MEM_ERR] 'inline _h_DS_Com * GetDSCom( char cNum )'���� ������ �ʰ� (value : %d)\n", sNum);
			return NULL;
		}
		return m_ServerNetInfo[sNum].ds_com ; 
	}

	inline _h_DS_Com * GetFirstCom()
	{
		for( int i = 1 ; i < GS_MAX ; ++i )
		{
			if( m_ServerNetInfo[i].ds_com )
				return m_ServerNetInfo[i].ds_com ;
		}

		return NULL ;
	}

	inline int GetRandServerNum() 
	{
		int iLastServer = ++m_iLastRBServer ;
		for( int i = 0 ; i < GS_MAX ; i++)
		{	
			
			if( GS_MAX <= iLastServer )
			{
				iLastServer = 1 ;
			}

			if( m_ServerStateInfo[iLastServer].cState == GS_OK ) 
			{				
				m_iLastRBServer = iLastServer ;
				return iLastServer ;
			}
			++iLastServer ;
		}
		return 1 ;
	}

	// ��� GS �� ���
	void send_to_all_gs( void * pData, u_short length );
	// except_server ���� ������
	void send_to_all_gs( void * pData, u_short length, short except_server );	// except_server �� �����ϰ� ������.
	// gs_index ���� ������
	void send_to_gs( void * pData, u_short length, short gs_index );
	//gs_array �鿡�Ը� ������.
	void send_to_some_gs( void * pData, u_short length, short gs_array[], int count );

	// Show Server settings in ServerEnv.inf file.
	void ShowSettings( bool bTo /* false : command pane. true : output pane */ );




private:
	bool			m_bTimeoutCheck[GS_MAX];

public:
	void ResetTimeoutCheck()
	{
		ZeroMemory( m_bTimeoutCheck, sizeof(m_bTimeoutCheck) );
	}
	void RecvTimeoutResponse( u_char server_index )
	{
		if( server_index < GS_MAX )
			m_bTimeoutCheck[server_index] = true;
	}

	void SendTimeoutCheck()
	{
		ResetTimeoutCheck();

		CMSG_CHECK_DSGS_PACKET pkCheck;
		ZeroMemory( &pkCheck, sizeof(pkCheck) );
		pkCheck.usLength = sizeof(pkCheck);
		pkCheck.cMessage = CMSG_NO_CHECK_DSGS_PACKET;
		pkCheck.ucMode = en_check_packet_timeout_request;

		send_to_all_gs( &pkCheck, pkCheck.usLength );
	}

	void ShowTimeoutCheckResult()
	{
		printf( "\n### Check Timeout Result ###\n" );
		for( int i = 1; i < GS_MAX; ++i )
		{
			if( GS_OK == m_ServerStateInfo[i].cState )
			{
				if( false == m_bTimeoutCheck[i] )
				{
					printf( "  [%2d] : no response\n", i );
				}
			}
		}
	}

#ifdef _PD_COMPANY_JAPAN_
	// RocWorks RMS ���� ����.
protected:
	char			m_rocAuthIP[16];
	u_short			m_rocAuthPort;

public:
	void set_roc_auth_ip( char * ip )		{ strcpy( m_rocAuthIP, ip ); }
	void set_roc_auth_port( u_short port )	{ m_rocAuthPort = port; }

	inline char * get_roc_auth_ip()			{ return m_rocAuthIP; }
	inline u_short get_roc_auth_port()		{ return m_rocAuthPort; }
#endif


};


#endif
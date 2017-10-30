#ifndef _GLOBAL_DEFINE_H
#define _GLOBAL_DEFINE_H


#define _COUNT_MAX_GROUP_				122

// ������ ��ȣ�� ������ 2���� �ε����� ���.
#define _GR_INDEX_TRAIN_				120
#define _GR_INDEX_UNION_				121


// ������ ��¥
#define  _COMPILE_DATE_	__TIMESTAMP__

// ���α׷� �ʱ�ȭ 

#define _GENERAL_QUEUE_MEMORY_SIZE			1024 //(ī��Ʈ��)
#define _UPDATE_QUEUE_MEMORY_SIZE			4096
#define _PARTY_QUEUE_MEMORY_SIZE			1024
#define _OR_QUEUE_MEMORY_SIZE				1024
#define _CHAT_QUEUE_MEMORY_SIZE				2048
#define _UDP_SEND_QUEUE_MEMORY_SIZE			2048
#define _UDP_RECV_QUEUE_MEMORY_SIZE			1024
#define _UDP_LOGIN_QUEUE_MEMORY_SIZE		2048

#define _PERMISSIBLE_MAX_USER				10000	// DS_SERVER ���� ����ϴ� �ִ� ������..(�ִ� ĳ���̶� �ٸ�)
#define _PERMISSIBLE_MAX_CHARAC				(_PERMISSIBLE_MAX_USER * 3)	// ��� ������ �ִ� ĳ���̴�
#define _UNIQUEID_MAX_COUNT					65000U

// DS Send OV Slot 
#define OV_SLOT_MAX_COUNT					10000U
#define OV_SLOT_MAX_SIZE					10U			// mega
#define _RECV_BUF_SIZE_						(4 * _MEGA_)



#define _FOREST_MAX_USER					2500 // �ʱ� ��� �ο� ������..
#define _GM_CLASS_NUM						19
#define _ROYAL_CLASS_NUM					18

// Thread ���� ���� 
#define _THREADNUMBER_PACKET_PROCESS_		4		//8	// ť�� �ִ� ��Ŷ�� ó���ϴ� �Լ�. �ظ��ϸ� ������ * 2
#define _THREADNUMBER_UPDATE_PROCESS_		1		//8	// ť�� �ִ� ��Ŷ�� ó���ϴ� �Լ�. �ظ��ϸ� ������ * 2
#define _THREADNUMBER_UDP_PACKET_RECV_		2		//4	// recv ó���ϴ� �Լ�.
#define _THREADNUMBER_UDP_PACKET_SEND_		1		//4	// secv ó���ϴ� �Լ�.
#define _THREADNUMBER_UDP_PACKET_WORK_		1		//8	// udp work ó���ϴ� �Լ�. (�ַ� GMS ��Ŷ�̴�.)
#define _THREADNUMBER_UDP_PACKET_LOGIN_		4		//8	// udp login ó���ϴ� �Լ�.
#define _THREADNUMBER_CHARAC_BACKUP_		2		// ĳ�� ��� ������ ����
#define _THREADNUMBER_USER_BACKUP_			1		// ���� ��� ������ ����
#define _THREADNUMBER_PARTY_WORK			1		// PARTY �۾� ������
#define _THREADNUMBER_OR_WORK				1		// OR �۾� ������
#define _THREADNUMBER_CS_CHAT_WORK			2		// CS Chat thread

// BACKUP �ֱ�
#define _BACKUP_TIME_USER_					50	// 
#define _BACKUP_TIME_CHARAC_				50	// 

#define _BACKUP_SYNC_OPTION					0
#define _BACKUP_UNSYNC_OPTION				1

// Memory size define
#define _MEGA_								1048576
#define _PAGE_SIZE_							4096

// UDP Socket ����
#define PASS_KEY							1003
#define PING_PORT							19999

#define UDP_PACKET_CUT_SIZE					300


// Buf ���� ����
#define UDP_SEND_BUF_SIZE					32767
#define UDP_RECV_BUF_SIZE					32767

// TCP MSG ����
#define HEAD_SIZE							2		// MSG ��Ŷ ��� ������

#define PWD_ALLLOGOUT						1111

// ��������, ���Ӽ��� �ִ� �迭��... 
#define SHOW_MAX							20	// �������� ����Ʈ ����
#define GS_MAX								35	// ���Ӽ��� �ִ�


#define CRYPTO_KEY_FILE						"Env\\lump.dat"
#define SQL_CONNECTION_STRING				"Env\\data.enc"

// GS Environment file path
#define SERVER_ENVIRONMENT_FILE				"Env\\ServerEnv.inf"
// GS Position list file path
#define SERVER_POSITION_FILE				"Env\\PosList.inf"

// Log server info file path
#define LOG_SVR_INFO_FILE					"Env\\LogServer.Inf"
// Log server info file path
#define BILLING_SVR_INFO_FILE				"Env\\BillingServer.Inf"

// Initialize value whitd Charac create 
#define INIT_VALUE_REF_FILE					"Env\\InitValue.inf"
#define INIT_EVENT_FILE						"Env\\event.txt"

#define _SYSTEM_LOG_FILE_					"logfile\\DS_SystemLogFile"
#define _PARTY_LOG_FILE_					"logfile\\Party"
#define _TEMP_LOG_FILE_						"logfile\\temp"
#define _GMS_LOG_FILE_						"logfile\\GMS"

#ifdef _PD_GAME_EVENT_
#define FILE_FREE_MATCH_SCRIPT				"Env\\FreeMatch.txt"
#endif
	


// Log ����
#define LOG_MAX_LINE						80000	// �α� �ִ� ����

// �κ��丮 ���� �ִ� ����
#define INVEN_MAX_SLOT						58

// ���� ���� �ִ� ����
#define MARTIAL_MAX_SLOT					60

/* ============================================================================
ĳ���� ����
============================================================================ */
#define CS_NONSTATE							0		//	�� ��� ����
#define CS_STANDBY							1		//	������ ��ٸ��� ����
//#define CS_SELECTED						2		//	ĳ���� ����â���� ���õǾ��� ����  //�Ⱦ����~ 
//#define CS_SV_ENTER						3		//	����(�ʵ�(��))�� �����ϴ���  // �̰͵� �Ⱦ����.. �����ϰ� ����.. 
#define CS_FIELD							3		//	�ʵ忡 ���� �Ϸ� ����(������)
#define CS_MOVE								4		//	���� �̵���
#define CS_ROBBY							5		//  �κ� ����..


/* ============================================================================
���� ����
============================================================================ */
#define US_NONSTATE						0		//	�α׾ƿ� ����(�� ���)
#define US_LOGIN_ING					1		//	�α��� ������
#define US_LOBBY						2		//	GS ����
#define US_FIELD						3		//	���� ������
#define US_MOVE							4		//	���� �̵���
#define US_LOCK							99		//	���� ��� ����

// ���� �޸� ������ ����
#define	US_SAVED						0		// �����
#define US_USING						1		// �����


/* ============================================================================
��� ����
============================================================================ */
#define BS_COMPLETED					0		// ��� �Ϸ�
#define BS_READY						1		// ��� ���


/* ============================================================================
����Ʈ ���
============================================================================ */
#define _MAX_CURQUEST_NUM_                  6           // ���ü��డ���� ����Ʈ �� (DB�� ����Ǵ� ����Ʈ�� ����)
#ifdef _PD_QUEST_MAX_600_
#define _QUEST_1_NUMBER_					(300)
#define _QUEST_2_NUMBER_					(300)
#define _QUEST_MAX_NUMBER_					(_QUEST_1_NUMBER_+_QUEST_2_NUMBER_)		    // �������� ������ ����Ʈ ����.

#else
#define _QUEST_MAX_NUMBER_					300		    // �������� ������ ����Ʈ ����.
#endif
//===============================================================================

/* ==============================================================================
���� �޽��� ����
=============================================================================== */
#define INIT		"INIT"		//�ʱ�ȭ ���� �޽���
#define INFO		"INFO"		//���� �˸� �޽���
#define BACKUP		"BACKUP"	//������� �޽���
#define DB_ERR		"DB_ERR"	// DB ����
#define SO_ERR		"SO_ERR"	//socket ����
#define ME_ERR		"ME_ERR"	//�޸� ���� ����
#define EXCEPTION	"EXCEPTION"	//����


// time ����
#define _HOUR_			3600		//�ѽð� = 3600��
#define _YEAR_			32140800	//1��
#define _SINCE_			(33 * _YEAR_)	// 1970~2003�� ���� ��..

//#define _MINUTE_MIL_SEC_ (1000 * 60)
//#define _HOUR_MIL_SEC_	(1000 * 60 * 60)	// 1�ð��� �и�������
//#define _DAY_MIL_SEC_	(1000 * 60 * 60 * 24)	// �Ϸ� �и�������


#define _DAY_SEC_	(60 * 60 * 24)			// �Ϸ� ������
#define _MINUTE_SEC_	(60)			// �Ϸ� ������


#define _SEC_SINCE_1970_ (31557600 * (2005-1970))



#define	_MEM_SAVE_DELAY_TIME_LOGOUT		(_MINUTE_SEC_ * 1)
#define _MEM_SAVE_DELAY_TIME_KICK		(_MINUTE_SEC_ * 3)
#define _UNIQUE_ID_LIVE_TIME_SEC		30
#define _RELOGIN_ABLE_TIME_SEC			5
#define _PARTY_MOVE_OUT_TIME_SEC		_MINUTE_SEC_ * 3
#define _GS_INFO_LOG_SAVE_TIME_SEC		_MINUTE_SEC_ * 5

/* ====================

party ����

=====================*/
#define MAX_PARTY_MEMBER					9		// ��Ƽ�� �ְ�
// GS �ִ� ����..
#define MAX_SERVER_COUNT	32


// �ͼ��� ó������.
#undef TRY
#undef CATCH_ALL

#define TRY					__try
#define CATCH_ALL			__except(g_pNDExceptionReport->GenerateExceptionReport(GetExceptionInformation()),EXCEPTION_EXECUTE_HANDLER)


// MS Transport Provider IOCTL to control
// reporting PORT_UNREACHABLE messages
// on UDP sockets via recv/WSARecv/etc.
// Path TRUE in input buffer to enable (default if supported),
// FALSE to disable.
#define SIO_UDP_CONNRESET_WSAIOW (IOC_VENDOR,12)

// ����� �ִ� ��.
#define MAX_CASTLE_MONEY		(4200000000U)


enum en_db_string_where
{
	game_db_string = 0,
	account_db_string,
	log_db_string,
	event_db_string,

#ifdef _CIS_20070326_
	cis_db_string,
#endif
} ;

// ���� ���� ��� : ��忡 ����, �޸� ��뷮�� �ٸ���.
enum EN_ND_SERVICE_MODE
{
	en_service_mode_standard=0,
	en_service_mode_outertest,
	en_service_mode_innertest,
};


/* =====================================================================================
���� ����
===================================================================================== */
#define _MOONPA_NO_NANGIN_				0		// ����
#define _MOONPA_NO_GAEBANG_				1		// ����
#define _MOONPA_NO_BEEGOONG_			2		// ���(���� �豳)
#define _MOONPA_NO_SORIM_				3		// �Ҹ�
#define _MOONPA_NO_NOKLIM_				4		// �츲
#define _MOONPA_NO_MOODANG_				5		// ����
#define _MOONPA_NO_MAGYO_				6		// ����
#define _MOONPA_NO_SEGA_				7		// ����
#define _MOONPA_NO_HEOKRYONG_			8		// ����

#define _NUM_MOONPA_					9		// �ִ� ���ļ�(����Ʈ�� ������ ���)		 
#define	_NUM_CLASS_						8		// �ִ� ��å��

// 0 : ����, 1 : �鵵, -1 : �浵
const short g_sMoonpaProperty[_NUM_MOONPA_] = 
{ 
	0, 	// ����0
		1,	// ����1
		-1,	// ���2
		1,	// �Ҹ�3
		-1,	// �츲4
		1,	// ����5
		-1,	// ����6
		0,	// ����7
		0	// ����8
} ;


enum LOGIN_MODE_BY_COMPANY
{
	en_lmbc_nexon = 0xa04b93f2,
	en_lmbc_metel = 0x3d732b0e, 
	en_lmbc_vina =  0x8c203d51,
	en_lmbc_acclaim = 0xef6c9aa1,
	en_lmbc_pwc = 0xef6c9aa2,
	en_lmbc_rocworks = 0xef6c9aa3,
	en_lmbc_russia = 0xad890bd0,
} ;

enum CHARAC_NAME_LENGTH
{
	en_charac_name_length = 12,
};

enum LOGIN_ID_LENGTH		// Login ID Lenght
{
	en_max_lil		= 24,
};

#define en_max_lpl		(24)

// Acclaim�� PWC ����.
#ifdef _PD_COMPANY_ACCLAIM_
#undef en_max_lpl
#define en_max_lpl		(32)
#endif

#ifdef _PD_COMPANY_PWC_
#undef en_max_lpl
#define en_max_lpl		(32)
#endif

#ifdef _PD_COMPANY_JAPAN_
#undef en_max_lpl
#define en_max_lpl		(32)
#endif

#ifdef _PD_COMPANY_RUSSIA_
#undef en_max_lpl
#define en_max_lpl		(32)
#endif

#define IPV4_BUF_SIZE		16


// ������ �̸��� ����.
enum
{
	en_or_name_length = 24, 
} ;

// ������ �ִ� �����ܼ�
#define _NDD_MAX_ORG_IN_ALLY_		(5)		// ���� ������ �ִ� ������ ���� 5�̴�.

#define __EXCEPT_STR_PRINT()		::PrintConsole("[EXCEPTION %s, %d \n", __FILE__, __LINE__ )

#ifndef _FREE_LOGIN_MODE
#define _FREE_LOGIN_MODE
extern bool g_bIsFreeLogin ;
#endif
#define IF_FREE_LOGIN			if( g_bIsFreeLogin == true )
#define IF_NOT_FREE_LOGIN		if( g_bIsFreeLogin == false )


#ifdef _METEL_LOGIN_200608_
struct METEL_BILLING_LOGIN {
	HANDLE	h;
	char	pszId[en_max_lil+1];
	char	pszPass[en_max_lpl+1];
	char	pszIp[IPV4_BUF_SIZE];
};

struct METEL_BILLING_INFO {
	// total (4*6) + (24*2) + 4 = 76 bytes

	int		iLoginSN;
	int		iBillingType;
	int		iUsedPoint;
	int		iPoint;
	int		iUsedFreePoint;
	int		iFreePoint;
	TGATime	Time;
	TGATime	EndTime;

	int		iCallbackResult;
};
#endif //_METEL_LOGIN_200608_


/* //////////////////////////////////////////////////////////////////////////
GS �� Ŭ���̾�Ʈ���� �ð��� ǥ���ϱ� ���� ����ϴ� ����ü��. 
�����, ������ �ٲ��� �� ��.!!!
*/ //////////////////////////////////////////////////////////////////////////
struct _sND_TimeUnit	// client �� ������ ��Ŷ�� ���Ե�. �ٲ�� �Ǹ� Ŭ���̾�Ʈ���� �˷���� �Ѵ�. 
{
	// ��/��/��/��/��/��. End Time
	u_int			uiYear		: 5 ;	// Base 2000. Year = 2000 + uiYear( 0-31 )
	u_int			uiMonth		: 4 ;	// Value 0 - 11 ( 0 - 15 )
	u_int			uiDay		: 6 ;	// Value 1 - 31 ( 0 - 63 )
	u_int			uiHour		: 5 ;	// Value 0 - 23 ( 0 - 31 ) 
	u_int			uiMinute	: 6 ;	// Value 0 - 59 ( 0 - 63 )
	u_int			uiSecond	: 6 ;	// Value 0 - 59 ( 0 - 63 )


	_sND_TimeUnit()
		: uiYear(0), uiMonth(0), uiDay(0), uiHour(0), uiMinute(0), uiSecond(0)
	{
	}

	_sND_TimeUnit( const _sND_TimeUnit& rh)
		: uiYear(rh.uiYear), uiMonth(rh.uiMonth), uiDay(rh.uiDay), 
		uiHour(rh.uiHour), uiMinute(rh.uiMinute), uiSecond(rh.uiSecond)
	{
	}

	void Print()
	{
		printf( "%u-%u-%u %u:%u:%u", uiYear+2000, uiMonth, uiDay, uiHour, uiMinute, uiSecond );
	}

	time_t GetValueType()
	{
		time_t value = uiSecond | (uiMinute<<6) | (uiHour<<12) | (uiDay<<17) | (uiMonth<<22) | (uiYear<<25) ;
		return value;
	}

	void SetTime( unsigned int y, unsigned int mon, unsigned int d, unsigned int h, unsigned int m, unsigned int s = 0 )
	{
		uiYear = y-2000;
		uiMonth = mon;
		uiDay = d;
		uiHour = h;
		uiMinute = m;
		uiSecond = s;
	}
} ;

//////////////////////////////////////////////////////////////////////////
// ��Ÿ �Լ� ������
//////////////////////////////////////////////////////////////////////////
class CND_TimeUnitFunc
{
public:
	static time_t	GetDiffSecond( _sND_TimeUnit& from, _sND_TimeUnit& to );
	static void		GetTime_BeforeSecond( _sND_TimeUnit& output, _sND_TimeUnit& from, u_short second );
	static int		GetElapsedDay( _sND_TimeUnit& t );
	static void		GetTime_Current( _sND_TimeUnit& output );
	static void		GetTime_FromSecond( _sND_TimeUnit& output, time_t cur );
	static time_t	Get_CurrentSecond();
	static time_t	Get_FromTimeUnit( _sND_TimeUnit& input );
};



#ifdef _PD_MASTER_PUPIL_

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

// ���� ���� �ִ밪. 0���� 4����. �� �ټ� �ܰ��.
#define NDD_MP_MAX_MASTER_LEVEL			(4)
// �ִ� ���ڼ�
#define NDD_MP_MAX_PUPIL_COUNT			(4)
// ���� ��û�� �ߺ������� ĳ���� ��.
#define NDD_MP_MAX_RESERVE_PUPIL		(5)		
// ���� ��û��, ��������� ��ȿ�ð� : ����/���� ���� ���δ�.
#define NDD_MP_RESERVE_WAIT_TIME		(12)		// s
// �⺻, Ȯ�� ������ ų�� ī��Ʈ
// ��ũ��Ʈ���� ���ǵ��� ������, �Ʒ� ���ڸ� ����. 
#define NDD_MP_BASE_KILLCOUNT			(100U)
#define NDD_MP_EXTEND_KILLCOUNT			(1000U)

// ��ȣ�� �ִ� ����.
#define NDD_MP_MASTERNICK_LENGTH		(36)		// NULL ������.

#define NDD_MP_MAX_POINT				(1000000000U)		// 10�� �̴�. ����� 0x8fffffff �� ���� �ʵ��� �� ��.

// ���忡 ���� �������� �ִ� ���� : 8 ~ 12 ���� ���������� �� �� �ִ�.
#define NDD_MP_MAX_ITEMCOUNT_PER_BOARD		(12)
#define NDD_MP_AVR_ITEMCOUNT_PER_BOARD		(10)
#define NDD_MP_MIN_ITEMCOUNT_PER_BOARD		(8)


// ��κ��� �������� DS������ ������̴�. ����, �����ϸ� �ݵ�� DS �� �Բ� ������ ��.
// charac :: m_masterpupil_relation ������ ���δ�. 
enum EN_MP_RELATION
{
	en_mp_relation_none=0,
	en_mp_relation_pupil,
	en_mp_relation_master,
};


// kill count �Լ��� ���ϰ����� ���δ�.
enum EN_KILLCOUNT_TYPE
{
	en_mp_killcount_type_none = 0,		// �ƹ����� ����. 
	en_mp_killcount_type_base,			// ù��° ����ġ ����.
	en_mp_killcount_type_extend,		// �ι��� ����ġ ����.
};


// ����ü�� ���, ��Ŷ���� ����ϴ� ���� cspk �� ���δ�.
// Ŭ���̾�Ʈ�� ����ϴ� ���, cspk �� ���δ�.

struct _sCSPK_Pupil
{
	int				iPupilIndex;
	u_int			uiMyKillCount;		// ���� ų�� ī��Ʈ.
	u_int			uiToMasterPoint;	// ���¿��� ���޵� ����Ʈ. ������ ������ ���� ������ �ö󰣴�.
	u_int			uiCurPoint;			// ���� ����Ʈ : ������ ������ ���� �ʾƵ� �ö󰣴�.

	_sND_TimeUnit	join_date;

	u_char			ucPupilPosition;	// 1���� 2����... 1 - 4 ����. 
	u_char			ucSex;				// ��. 0 - ��, 1 - ��.
	u_char			ucLevel;			// ���� ����.
	u_char			ucMoonpa;			// ����.
	u_char			ucClass;			// ��å
	u_char			ucClassGrade;		// ��å ���.
	u_char			ucConnectState;		// 0 - not, 1 - field, 2 - server move
	u_char			ucZone;				// ĳ���Ͱ� ��� �ִ°�. 0 �̸� �������� ���� ��.
	char			cPupilName[en_charac_name_length+1];

	_sCSPK_Pupil()
	{
		Reset();
	}

	void SetInfo( _sCSPK_Pupil& pupil )
	{
		CopyMemory( this, &pupil, sizeof(_sCSPK_Pupil) );
		uiMyKillCount		= 0;					// SetInfo �� DB ���� ���� �� �����ϹǷ�, 0����.
	}
	void Reset()
	{
		ZeroMemory( this, sizeof(_sCSPK_Pupil) );
	}
	void SetName( char * name )
	{
		if( name )
		{
			strncpy( cPupilName, name, en_charac_name_length );
			cPupilName[en_charac_name_length] = 0;
		}
	}
	void SetIndex( int cuid )	{ iPupilIndex = cuid; }

	void SetJoinDate( DBTIMESTAMP& join )
	{
		join_date.uiYear	= join.year - 2000;
		join_date.uiMonth	= join.month;
		join_date.uiDay		= join.day;
		join_date.uiHour	= join.hour;
		join_date.uiMinute	= join.minute;
		join_date.uiSecond	= join.second;
	}

	bool CanBeStruck()
	{
		_sND_TimeUnit cur;
		CND_TimeUnitFunc::GetTime_Current( cur );

		time_t diff = CND_TimeUnitFunc::GetDiffSecond( join_date, cur );
		
#ifdef _PD_MP_DEBUG_
		return (diff > 300);		// 5��.
#else
		return (diff > 72*3600);		// 72�ð�.
#endif
	}

	u_int SubTotalPoint( u_int point )
	{
		return point;
	}


	void SetMyKillCount( u_int count )	{ uiMyKillCount = 0; }

	// ���ϰ� : EN_KILLCOUNT_TYPE �� ����Ѵ�. 
	u_char AddMyKillCount();

	inline bool IsConnected()	{ return (ucConnectState!=0); }
};


// �Խ��ǿ� ǥ�õ� ���� ����
struct _sPacketMPBoard
{
	u_char				ucMasterLevel;			// ������ ����.
	u_char				ucMasterRank : 4;		// ���� ���
	u_char				ucPupilCount : 4;		// ���ڼ�( �ʿ��ϸ� ���� )
	u_char				ucMoonpa;				// ����.	
	u_char				ucClass;				// ��å
	u_char				ucClassGrade : 2;		// ��ä ���.
	u_char				ucOrg : 2;				// ������ �����ΰ�.
	u_char				ucEtc : 4;				// ��Ÿ ���� �ʿ��ϸ�.
	char				cMasterName[en_charac_name_length+1];
	char				cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];


	void SetName( char * name )
	{
		ZeroMemory( cMasterName, sizeof(cMasterName) );
		strncpy( cMasterName, name, en_charac_name_length );
	}

	void SetNick( char * nick )
	{
		ZeroMemory( cMasterNick, sizeof(cMasterNick) );
		strncpy( cMasterNick, nick, NDD_MP_MASTERNICK_LENGTH );
	}

};


#endif	// _PD_MASTER_PUPIL_

#endif 



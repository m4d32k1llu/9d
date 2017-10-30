#ifndef _GLOBAL_DEFINE_H
#define _GLOBAL_DEFINE_H



#ifdef _NDEBUG
// ���α׷� �ʱ�ȭ 
//#define _TOTAL_VITUAL_MEMORY_SIZE			100U
#define _QUEUE_MEMORY_SIZE					4096
#define _CHAT_QUEUE_MEMORY_SIZE				4096
#define _UDP_QUEUE_SIZE						4096
#define _PERMISSIBLE_MAX_USER				200	// DS_SERVER ���� ����ϴ� �ִ� ������..(�ִ� ĳ���̶� �ٸ�)
#define _SEND_SLOT_COUNT_					100
#define _SOCKET_POOL_COUNT_					100
#define _CRITICAL_SECTION_POOL_SIZE			1000
#else
// ���α׷� �ʱ�ȭ 
//#define _TOTAL_VITUAL_MEMORY_SIZE			200U
#define _QUEUE_MEMORY_SIZE					16384 //4096
#define _CHAT_QUEUE_MEMORY_SIZE				16384 //4096
#define _UDP_QUEUE_SIZE						4096
#define _PERMISSIBLE_MAX_USER				20000	// DS_SERVER ���� ����ϴ� �ִ� ������..(�ִ� ĳ���̶� �ٸ�)
#define _SEND_SLOT_COUNT_					100000
#define _SOCKET_POOL_COUNT_					10000
#endif

// Thread ���� ���� 
#define _THREADNUMBER_PACKET_RECV_			4		//4	// recv ó���ϴ� �Լ�.
//#define _THREADNUMBER_PACKET_PROCESS_		4		//8	// ť�� �ִ� ��Ŷ�� ó���ϴ� �Լ�. �ظ��ϸ� ������ * 2
#define _THREADNUMBER_TCP_WORK_				4		// �Ϲ� �޽��� ó���ϴ� �������..
#define _THREADNUMBER_UDP_WORK_				1		// �Ϲ� �޽��� ó���ϴ� �������..
#define _THREADNUMBER_CHATWORK_				8		// ä�� �޽��� ó���ϴ� ��ũ�������..
#define _THREADNUMBER_LOGINWORK_			8		// �α��� �޽��� ó���ϴ� ��ũ������ ��..
#define _THREADNUMBER_BACKUP_				2		// ��� ������..

#define _BACKUP_INTAERVAL_TIME_				100		// ��� �ð� ����

#define _ACCEPT_SUCCESS_TIME_				12000


enum en_db_string_where
{
	game_db_string = 0,
	account_db_string,
	log_db_string,
	event_db_string,
} ;


// ������..
#define _GROUP_PARSING_CHAR_				'\r'		// '\r'�׷��̸� ����
#define _CHARAC_PARSING_CHAR_				'\n'		// '\n'ĳ���̸� ����..



//TCP Socket ����	
#define ACCEPT_PORT							7411


// GS Environment file path
#define CRYPTO_KEY_FILE						"lump.dat"
#define SQL_CONNECTION_STRING				"data.enc"

	
// GS ���� ����
#define MAX_SERVER							25	//->GsSvMx
//#define MAX_USER							1000

// GS ���� ����
#define GS_NONSTATE							0		// ��������
#define GS_OK								1		// �������
#define GS_OVER								3		// ��ȭ
#define GS_ERROR							4		// ������ ����

// Log ����
#define LOG_MAX_LINE						0	// �α� �ִ� ����
#define _CONSOLE_OUPUT_CATCH_LOG_					// �ָܼ޽����� �α׷� ���� ��


/* ============================================================================
ĳ���� ����
============================================================================ */
/*
#define CHARAC_STATE_LOGOFF						0	//	�α׿��� ����
#define CHARAC_STATE_LOGON						1	// �α׿� ����
#define CHARAC_STATE_PLAY						2	// ������
#define CHARAC_STATE_3							3
#define CHARAC_STATE_4							4
*/



/* ============================================================================
Messenger Server
============================================================================ */
#define MAX_MAKE_FRIEND						58
#define MAX_MEMBER							50
#define MAX_GROUP							8
#define MAX_MAIL_COUNT						20

/* ============================================================================
Backup
============================================================================ */
#define BACKUP_NONE							0
#define BACKUP_READY						1


/* ============================================================================
�ý��ۿ��� �������� ���� ����ڿ��� �������� �޽��� ���
============================================================================ */
#define MSMESSAGE_CUT_FRIEND				1
#define MSMESSAGE_DEL_FRIEND				2		// ĳ���� ������

/* ============================================================================
���� ����
============================================================================ */


// time ����
#define _HOUR_			3600		//�ѽð� = 3600��
#define _YEAR_			32140800	//1��
#define _SINCE_			33 * _YEAR_	// 1970~2003�� ���� ��..


#define		_LOG_WRITE_


// UDP PASS KEY
#define  _SERVER_AUTHOR_	1003
#define  _PING_PORT_		10000 + ACCEPT_PORT

// GM CLASS NUM
#define  _GM_CLASS_NUM_			10
#define  _GM_RETURN_MESSAGE		"GM���Դ� �ӼӸ��� ������ �����ϴ�.\n"





// MS Transport Provider IOCTL to control
// reporting PORT_UNREACHABLE messages
// on UDP sockets via recv/WSARecv/etc.
// Path TRUE in input buffer to enable (default if supported),
// FALSE to disable.
#define SIO_UDP_CONNRESET_WSAIOW (IOC_VENDOR,12)




#endif 



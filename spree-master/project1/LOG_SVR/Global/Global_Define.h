#ifndef _GLOBAL_DEFINE_H
#define _GLOBAL_DEFINE_H

// ���α׷� �ʱ�ȭ 
#define _TOTAL_VITUAL_MEMORY_SIZE			400U
#define _QUEUE_MEMORY_SIZE					16384 //4096

// Thread ���� ���� 
#define _THREADNUMBER_PACKET_RECV_			2		//4	// recv ó���ϴ� �Լ�.
#define _THREADNUMBER_WORK_					4		// �Ϲ� �޽��� ó���ϴ� �������..
#define _THREADNUMBER_BACKUP_				16		// ��� ������..


// GS Environment file path
#define CRYPTO_KEY_FILE						"lump.dat"
#define SQL_CONNECTION_STRING				"log.enc"


//TCP Socket ����	
#define ACCEPT_PORT_BASE					30000

	
#define _COUNT_FIELD_						25

// Buf ���� ����
#define UDP_SEND_BUF_SIZE					4096
#define UDP_RECV_BUF_SIZE					4096

// TCP MSG ����
#define HEAD_SIZE							2		// MSG ��Ŷ ��� ������

	
// GS ���� ����
#define MAX_SERVER							25	//->GsSvMx
//#define MAX_USER							1000


// Log ����
#define LOG_MAX_LINE						10000	// �α� �ִ� ����
#define _CONSOLE_OUPUT_CATCH_LOG_					// �ָܼ޽����� �α׷� ���� ��


// time ����
#define _HOUR_			3600		//�ѽð� = 3600��
#define _YEAR_			32140800	//1��
#define _SINCE_			(33 * _YEAR_)	// 1970~2003�� ���� ��..

#define _TEST_
#define		_LOG_WRITE_


// UDP PASS KEY
#define  _SERVER_AUTHOR_	1003


// �α� ť ��� ����..
#define  _LOG_ITEM_NODE_MAX_NUM_		400000
#define  _LOG_CHARAC_NODE_MAX_NUM_		50000
#define  _LOG_STATE_NODE_MAX_NUM_		10000
#define  _LOG_LOGIN_NODE_MAX_NUM_		50000
#define _BACKUP_QUEUE_MAX_SIZE			(_LOG_ITEM_NODE_MAX_NUM_+_LOG_CHARAC_NODE_MAX_NUM_+_LOG_STATE_NODE_MAX_NUM_+_LOG_LOGIN_NODE_MAX_NUM_)


enum en_db_string_where
{
	game_db_string = 0,
	account_db_string,
	log_db_string,
} ;

#endif 
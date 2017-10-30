#pragma once

#pragma pack(push, enter_login)
#pragma pack(1)

struct sHeader;

#define MSG_NO_LOGIN					1
#define MSG_NO_LOGIN_SUCCESS			3
#define MSG_NO_LOGIN_FAIL				4

#ifdef _XADMINISTRATORMODE
#define MSG_NO_GMLOGIN					124
#endif

#define MSG_NO_SEQUENCE_NUMBER 0 

#define _PASSPORT_LENGTH_NEXON_		(96)

struct MSG_SEQUENCE_NUMBER 
{ 
	_sHeader Header;
	u_char ucMessage;

	u_char ucStartSeq;
};

// �α��� ���� : 1-5
// �α��� C->S
//#define MSG_NO_LOGIN						1



#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

enum LOGIN_MODE_BY_COMPANY
{
	en_lmbc_nexon	= 0xa04b93f2,
#ifdef _PREOBVERSION 		
	en_lmbc_metel	= 0x3d732b0f,
#else
	en_lmbc_metel	= 0x3d732b0e, 
#endif
	en_lmbc_vina	= 0x8c203d51,
	en_lmbc_acclaim	= 0xef6c9aa1,
	en_lmbc_pwc		= 0xef6c9aa2,
	en_lmbc_rocworks= 0xef6c9aa3,
	en_lmbc_russia = 0xad890bd0,
};

enum LOGIN_ID_LENGTH		// Login ID Length
{
	en_lil_nexon	= 24,
	en_lil_metel	= 24,
	en_lil_vina		= 24,
	en_lil_acclaim	= 24,
	en_lil_pwc		= 24,
	en_lil_rocworks	= 24,
	en_lil_playten	= 24,

	en_max_lil = 24,
};

enum LOGIN_PASSWORD_LENGTH
{
	en_lpl_nexon	= 24,
	en_lpl_metel	= 24,
	en_lpl_vina		= 24,
	en_lpl_acclaim	= 32,
	en_lpl_pwc		= 32,
	en_lpl_rocworks	= 32,
	en_lpl_playten  = 32,

#ifdef _XENGLISH
	en_max_lpl = 32,
#else
	
	#ifdef _XJAPANESE
		en_max_lpl = 32,
	#else
		#ifdef _XRUSSIAN
			en_max_lpl = 32,
		#else
			en_max_lpl = 24,
		#endif
	#endif

#endif
};


/*
#ifdef _XDEF_NEXONLOGIN

struct MSG_LOGIN
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_int				uiLMBC_Code ;		// Login Mode by Company
	
	u_short				sVersionIndex ;
	DWORD				dwClientVer ;
	char				cForestSelectNum ;		// ���� ��
} ;

struct MSG_LOGIN_NEXON : public MSG_LOGIN
{
	char				passport[_PASSPORT_LENGTH_NEXON_] ;
} ;

#else
*/

#define _PASSPORT_LENGTH_                  (96)

#define _PD_LOGIN_PACKET_MODE_

#ifdef _PD_LOGIN_PACKET_MODE_
struct MSG_LOGIN
{	
	_sHeader    header;	
	u_char      ucMessage;	
	
	u_int       uiLMBC_Code;        // Login Mode by Company	
	
	u_short     sVersionIndex;	
	DWORD       dwClientVer;
	
	char        cForestSelectNum;   // ���� ��	
	char        passport[_PASSPORT_LENGTH_];
};

#else

//#define MSG_NO_LOGIN  1

struct MSG_LOGIN 
{
	_sHeader			header;
	u_char				ucMessage;

	char				cID[13];			//	���̵�
	char				cPassword[13];		//	�н�����
	short				sVersionIndex;		//  Patch number
	DWORD				dwClientVer;		//  Client program version
	char				cForestSelectNum ;	//  ���� ��
};

#endif

// �α��� ��� : S->C
// ���� : �̵��� ������ ������, ��Ʈ �� ��ȿ.
//#define MSG_NO_LOGIN_SUCCESS				3
/*
struct MSG_LOGIN_SUCCESS
{
	_sHeader			header;
	u_char				ucMessage;

	unsigned short		usServerPort;		// Ŭ���̾�Ʈ htons �� �ٲ㼭 ����Ѵ�
	unsigned long		ulServerIP;			// Ŭ���̾�Ʈ �̰��� �״�� ����Ѵ�

	unsigned short		usMessengerPort;
	unsigned long		ulMessengerIP;

	u_short				usUniqueID;			// �� ���� ���� ���� �����Ѵ�. �ٲ� ����Ÿ�� 2�� �̻� ���� ���´�. 

#ifdef _XDEF_NEXONLOGIN
	char				cNDAccount[en_lil_nexon+1];	// �� ������ ���Ӽ��� ������ �˴ϴ�. SV_ENTER_GAME
#endif
};
*/

//#define MSG_NO_LOGIN_SUCCESS                                                             3

struct MSG_LOGIN_SUCCESS
{	
	_sHeader            Header;	
	u_char              cMessage;
		
	u_short             usServerPort;       // ������ GS Port
	u_long              ulServerIP;         // ������ GS IP
		
	u_short             usMessengerPort;    // ������ Messenger Server Port	
	u_long              ulMessengerIP;      // ������ Messenger Server IP
		
	u_short             usUniqueID;         // �� ���� ���� ���� �����Ѵ�.
		
#ifdef _PD_LOGIN_PACKET_MODE_	
	char                cNDAccount[en_max_lil+1] ;  // �� ������ ���Ӽ��� ������ �˴ϴ�. SV_ENTER_GAME
#endif	
};



// �α��� ����
#define MSG_ERROR_CODE_VERSION_FAILED		10
//#define MSG_NO_LOGIN_FAIL					4
struct MSG_LOGIN_FAIL 
{
	_sHeader			header;
	u_char				ucMessage;

	unsigned char		ucErrorCode;		// ���� �ڵ� - 10 : ���������� �ٸ�
};


// login error code
/*
#define _LOGIN_ERROR_CODE_BADUSER			1	// �α��� �� �� ���� ���� (���� ���� ����)
#define _LOGIN_ERROR_CODE_OVERTIME			3	// ���ѵ� �α��� �ð��� �ѱ�
#define _LOGIN_ERROR_CODE_BADPASSWORD		4	// �н����� Ʋ��
#define _LOGIN_ERROR_CODE_ALREADY			5	// �̹� �α�����
#define _LOGIN_ERROR_CODE_BADVERSION		10	// ������ �ٸ�
#define _LOGIN_ERROR_CODE_SERVERISFULL		12	// ������ �������� �̹� �� ��������..
#define _LOGIN_ERROR_CODE_ETC				13	// ��Ÿ ����..
*/
#define _LOGIN_ERROR_CODE_BADUSER		1	// ������ ���� ������ �ƴմϴ�. 
#define _LOGIN_ERROR_CODE_BLOCK			2	// �ش� ������ ����� ���� �Ǿ����ϴ�. 
#define _LOGIN_ERROR_CODE_OVERTIME		3	// ��뷮�� ���� ������ ��Ȱ���� �ʽ��ϴ�. 
#define _LOGIN_ERROR_CODE_BADPASSWORD	4	// ��й�ȣ�� ��ġ���� �ʽ��ϴ�. 
#define _LOGIN_ERROR_CODE_ALREADY		5	// �̹� �������̰ų� ���� �Ϸᰡ ��Ȱ�� �̷������ ���� �����Դϴ�. 
#define _LOGIN_ERROR_CODE_CHECKUP		6	// �׽�Ʈ �ð��� �ƴմϴ�.(���� �������Դϴ�.) 
#define _LOGIN_ERROR_CODE_OFF			7	// ���� �������Դϴ�.(�α��μ����� �����������) 
#define _LOGIN_ERROR_CODE_NO_USER		8	// ���� ����� 
#define _LOGIN_ERROR_CODE_TEEN_USER		9	// ����(15�� �̸�) 
#define _LOGIN_ERROR_CODE_BADVERSION	10	// ������ �ٸ� 
#define _LOGIN_ERROR_CODE_SERVERISFULL	12	// ������ �������� �̹� �� ��������.. 
#define _LOGIN_ERROR_CODE_ETC			13	// ��Ÿ ����.. 
#define _LOGIN_ERROR_CODE_BILL_NO_INFO	15	// �������� ����, ��ǰ���� �ٶ�
#define _LOGIN_ERROR_CODE_BILL_NO_MONEY	16	// �ܾ� ����
#define _LOGIN_ERROR_CODE_NOT_LMBC_CODE	17	// ���� �ڵ�( ȸ�� �ڵ� ) �̻�
#define _LOGIN_ERROR_CODE_NOT_UNI_NEXON	18	// �ؽ� ���� ������ �ƴմϴ�. ���� ���� �ٶ�.

// �븸 ������ message
#define _LOGIN_ERROR_CODE_NOT_PCBANG	20	// PC�� ������ �ƴ�	: �븸 ������
enum 
{
	_LOGIN_ERROR_CODE_METEL_E00 = 21,		// Can't open aplist.txt
	_LOGIN_ERROR_CODE_METEL_E01,			// Invalid data in aplist.txt
	_LOGIN_ERROR_CODE_METEL_E02,			// Fail to connect to SQLAP
	_LOGIN_ERROR_CODE_METEL_E03,			// Fail to create event
	_LOGIN_ERROR_CODE_METEL_E04,			// Calling API before InitBilling
	_LOGIN_ERROR_CODE_METEL_E05,			// Bad message format
	_LOGIN_ERROR_CODE_METEL_E06,			// Timeout for SQLAP response
	_LOGIN_ERROR_CODE_METEL_E07,			// Fail to send message to SQLAP
	_LOGIN_ERROR_CODE_METEL_E08,			// Too many user login, max 320000
	_LOGIN_ERROR_CODE_METEL_E09,			// IP address is incorrect
	_LOGIN_ERROR_CODE_METEL_E10,			// Port value is out of range
	_LOGIN_ERROR_CODE_METEL_E11,			// Fail to create thread
	_LOGIN_ERROR_CODE_METEL_E12,			// API is already running
	_LOGIN_ERROR_CODE_METEL_E13,			// Crypt key has not been set
	_LOGIN_ERROR_CODE_METEL_E14,			// Message queue is full
	_LOGIN_ERROR_CODE_METEL_E15,			// SQLAP is dead
	_LOGIN_ERROR_CODE_METEL_E16,			// Checksum error in received message
	_LOGIN_ERROR_CODE_METEL_E17,			// Crypt key is too long or too short

	_LOGIN_ERROR_CODE_METEL_E50,			// SQL Error
	_LOGIN_ERROR_CODE_METEL_E51,			// AP exception
	_LOGIN_ERROR_CODE_METEL_E52,			// Incorrect ID or Password
	_LOGIN_ERROR_CODE_METEL_E53,			// User has not registered the game
	_LOGIN_ERROR_CODE_METEL_E54,			// This account is forbidden
	_LOGIN_ERROR_CODE_METEL_E55,			// Points is not enough to play
	_LOGIN_ERROR_CODE_METEL_E56,			// The game registration is forbidden
	_LOGIN_ERROR_CODE_METEL_E57				// Can't find the LoginSN in DB
};

#define _LOGIN_ERROR_CODE_BLOCK_CODE_1  101 // ������ 
#define _LOGIN_ERROR_CODE_BLOCK_CODE_2  102 // �����з� 
#define _LOGIN_ERROR_CODE_BLOCK_CODE_3  103 // �ӽú�

#pragma pack(pop, enter_login)

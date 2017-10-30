#pragma once 

#pragma pack( push, enter_Login_Packet )
#pragma pack(1)

#define _COUNT_MAX_GROUP_				122

#ifdef _PD_UNI_SERVER_

	// ������ ��ȣ�� ������ 2���� �ε����� ���.
	#define _GR_INDEX_TRAIN_				120
	#define _GR_INDEX_UNION_				121

#else

	#define _GR_INDEX_TRAIN_				17

#endif



#define MSG_NO_DEQUENCE_NUMBER			0
#define MSG_NO_LOGIN					1
#define MSG_NO_GMLOGIN					124

#define MSG_NO_LOGIN_SUCCESS			3
#define MSG_NO_LOGIN_FAIL				4
#define MSG_NO_LOGIN_FOREST_LIST		5		// ps �� �����ϸ� �� ��Ŷ�� �����ְ� ������ ���´�.
#define MSG_NO_SET_VERSION				100

struct _sHeader {
	short		sLength : 12 ;
	short		sCrypto : 2 ;
	short		sCompressed : 2 ;
};

//#define MSG_NO_DEQUENCE_NUMBER			0
struct MSG_SEQUENCE_NUMBER
{
	_sHeader			Header ;
	u_char				cMessage ;
	u_char				ucStartSeq ;
};

#define _PASSPORT_LENGTH_		(96)

struct MSG_LOGIN
{
	_sHeader			Header ;
	u_char				ucMessage ;

	u_int				uiLMBC_Code ;		// Login Mode by Company

	u_short				sVersionIndex ;
	DWORD				dwClientVer ;
	char				cForestSelectNum ;		// ���� ��

	char				passport[_PASSPORT_LENGTH_] ;
} ;

//#define  _FOREST_STATE_RUNNING				0					// ���� �۵���
//#define  _FOREST_STATE_OFF					-1					// ���� ����
//#define  _FOREST_STATE_CHECKUP_				-2					// ������ ������
//#define  _FOREST_STATE_FULL_				-3					// ��ȭ����
struct _ForestInfo 
{
	int		nVersion;
	DWORD	dwClientVer ;
	bool	bServerRun ;
	char	cForestState[_COUNT_MAX_GROUP_] ;			//	-3, ��ȭ���� -2, ������ -1 : ������ ,  0~100 ����	
};



//#define MSG_NO_LOGIN_FOREST_LIST		5
struct MSG_LOGIN_FOREST_LIST
{	
	struct _ForestInfo_packet 
	{
		int		nVersion;		
		bool	bServerRun ;
		char	cForestState[_COUNT_MAX_GROUP_] ;			//	-3, ��ȭ���� -2, ������ -1 : ������ ,  0~100 ����	
	} ForestInfo ;	
};


//#define MSG_NO_LOGIN_SUCCESS					3
struct MSG_LOGIN_SUCCESS
{
	_sHeader			Header ;

	u_char				cMessage ;
	
	u_short				usServerPort ;		// ������ GS Port
	u_long				ulServerIP ;		// ������ GS IP

	u_short				usMessengerPort ;	// ������ Messenger Server Port
	u_long				ulMessengerIP ;		// ������ Messenger Server IP

	u_short				usUniqueID ;		// �� ���� ���� ���� �����Ѵ�.

	char				cNDAccount[en_max_lil+1] ;	// �� ������ ���Ӽ��� ������ �˴ϴ�. SV_ENTER_GAME
} ;



// Login Err code
#define _LOGIN_ERROR_CODE_BADUSER			1	// ������ ���� ������ �ƴմϴ�.
#define _LOGIN_ERROR_CODE_BLOCK				2	// �ش� ������ ����� ���� �Ǿ����ϴ�.
#define _LOGIN_ERROR_CODE_OVERTIME			3	// ��뷮�� ���� ������ ��Ȱ���� �ʽ��ϴ�.
#define _LOGIN_ERROR_CODE_BADPASSWORD		4	// ��й�ȣ�� ��ġ���� �ʽ��ϴ�.
#define _LOGIN_ERROR_CODE_ALREADY			5	// �̹� �������̰ų� ���� �Ϸᰡ ��Ȱ�� �̷������ ���� �����Դϴ�.
#define _LOGIN_ERROR_CODE_CHECKUP			6	// �׽�Ʈ �ð��� �ƴմϴ�.(���� �������Դϴ�.)
#define _LOGIN_ERROR_CODE_OFF				7	// ���� �������Դϴ�.(�α��μ����� �����������)
#define _LOGIN_ERROR_CODE_NO_USER			8	// ���� �����
#define _LOGIN_ERROR_CODE_TEEN_USER			9	// ����(15�� �̸�) ������.
#define _LOGIN_ERROR_CODE_BADVERSION		10	// ������ �ٸ�
#define _LOGIN_ERROR_CODE_SERVERISFULL		12	// ������ �������� �̹� �� ��������..
#define _LOGIN_ERROR_CODE_ETC				13	// ��Ÿ ����..
#define _LOGIN_ERROR_CODE_BILL_NO_INFO		15	// �������� ����, ��ǰ���� �ٶ�
#define _LOGIN_ERROR_CODE_BILL_NO_MONEY		16	// �ܾ� ����
#define _LOGIN_ERROR_CODE_NOT_LMBC_CODE		17	// ���� �ڵ�( ȸ�� �ڵ� ) �̻�
#define _LOGIN_ERROR_CODE_NOT_UNI_NEXON		18	// �ؽ� ���� ������ �ƴմϴ�. ���� ���� �ٶ�.
#define _LOGIN_ERROR_CODE_LOGIN_FAIL		19	// �α��� ����(ID or Pass �� ���� Ʋ�ȴ��� ��)
#define _LOGIN_ERROR_CODE_NOT_PCBANG_USER	20	// PC�� ���� �ƴ�
enum {
	_LOGIN_ERROR_CODE_METEL_E00 = 21,			// Can't open aplist.txt
	_LOGIN_ERROR_CODE_METEL_E01,				// Invalid data in aplist.txt
	_LOGIN_ERROR_CODE_METEL_E02,				// Fail to connect to SQLAP
	_LOGIN_ERROR_CODE_METEL_E03,				// Fail to create event
	_LOGIN_ERROR_CODE_METEL_E04,				// Calling API before InitBilling
	_LOGIN_ERROR_CODE_METEL_E05,				// Bad message format
	_LOGIN_ERROR_CODE_METEL_E06,				// Timeout for SQLAP response
	_LOGIN_ERROR_CODE_METEL_E07,				// Fail to send message to SQLAP
	_LOGIN_ERROR_CODE_METEL_E08,				// Too many user login, max 320000
	_LOGIN_ERROR_CODE_METEL_E09,				// IP address is incorrect
	_LOGIN_ERROR_CODE_METEL_E10,				// Port value is out of range
	_LOGIN_ERROR_CODE_METEL_E11,				// Fail to create thread
	_LOGIN_ERROR_CODE_METEL_E12,				// API is already running
	_LOGIN_ERROR_CODE_METEL_E13,				// Crypt key has not been set
	_LOGIN_ERROR_CODE_METEL_E14,				// Message queue is full
	_LOGIN_ERROR_CODE_METEL_E15,				// SQLAP is dead
	_LOGIN_ERROR_CODE_METEL_E16,				// Checksum error in received message
	_LOGIN_ERROR_CODE_METEL_E17,				// Crypt key is too long or too short

	_LOGIN_ERROR_CODE_METEL_E50,/*39*/			// SQL Error
	_LOGIN_ERROR_CODE_METEL_E51,				// AP exception
	_LOGIN_ERROR_CODE_METEL_E52,				// Incorrect ID or Password
	_LOGIN_ERROR_CODE_METEL_E53,				// User has not registered the game
	_LOGIN_ERROR_CODE_METEL_E54,				// This account is forbidden
	_LOGIN_ERROR_CODE_METEL_E55,				// Points is not enough to play
	_LOGIN_ERROR_CODE_METEL_E56,				// The game registration is forbidden
	_LOGIN_ERROR_CODE_METEL_E57,				// Can't find the LoginSN in DB

	_LOGIN_ERROR_CODE_METEL_UNKNOWN/*47*/
};
#define _LOGIN_ERROR_CODE_BLOCK_CODE_1		101	// ������
#define _LOGIN_ERROR_CODE_BLOCK_CODE_2		102	// �����з�
#define _LOGIN_ERROR_CODE_BLOCK_CODE_3		103	// �ӽú�



// �α��� ���� 
//#define MSG_NO_LOGIN_FAIL						4
struct MSG_LOGIN_FAIL
{
	_sHeader			Header ;

	u_char				cMessage ;
	u_char				ucErrorCode ;	// Login Err code
} ;




struct MSG_SET_VERSION {
	_sHeader			Header ;

	u_char				cMessage ; 
	u_short				usVersion ;
	DWORD				dwClientVer ;	

	char				cIsTest ;			// 0. ����, 1. �׽�Ʈ
};

#pragma pack( pop, enter_Login_Packet )
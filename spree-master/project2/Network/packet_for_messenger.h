#pragma once

#pragma pack( push, enter_ )
#pragma pack(1)

#include <list>
using namespace std;


// 2004.05.27->oneway48 insert : ������ 
struct _Mail {							// ����(������)
	UINT			uiIndex ;			// ���� �ε���
	bool			check ;				// true : ���� , false : ���ε���(������)	
};

struct _MailInfo {
	UINT			uiIndex ;			// ���� �ε���
	char			cFrom[13] ;			// �������
	struct _Date {						// �����ð�
		unsigned	year : 12 ;				
		unsigned	month : 4 ;
		unsigned	day : 5 ;
		unsigned	hour : 5 ;
		unsigned	minute : 6 ;
	} date;	// 4Byte
	char			cMode ;
	enum
	{
		_PRIVATE,
		_PUBLIC,
		_FROM_GM,
		_SYSTEM_MSG_OR_KICK,	//[%s��] ����\n �����߹� �Ǿ����ϴ�.\n
	};

	char			cStrLen ;
	char			cText[101] ;		// ����
};
// 2004.05.27->oneway48 insert end 

struct _Member {	
	char			characName[15] ;	// ĳ���� �̸� �Ǵ� �׷� �̸����� ���δ�.	
	void *			pFriendInfo ;		// �ش� _FriendList �� *(������)
};

struct _Friend {
	char			cChoiceName[15] ;	// ģ�� �̸�.. �Ǵ� �׷� �̸�..
	char			cState	;			// ����..
} ;


/*
struct _FriendInfo {
	char		characName[13] ;	// ĳ���� �̸� �Ǵ� �׷� �̸����� ���δ�.	
	_h_DS_Com *	ds_com;				// NULL �̸� �α׿�����		
	char		state ;				// ĳ���� ����..
	char		option ;			// ģ�� �α� ��û �ź�, �Ӹ� �ź�.. ��..
	// 0 . ����.
	// 1 . ģ�� �α� �ź�
	// 2 . �Ӹ� �ź�..
	
	//	int			Chat_Count ;		// Chat ����� ������ 
	//	_h_DS_Com * Chat_Com[50] ;
	
	// Chatting Character Info(2004.03.10 Add ozzywow)
	char		m_cChatting_CharacName[13] ;
	_h_DS_Com *	m_pChatting_DS_Com ;
	
	char		cBackupMode ;		// 0. None
	// 1. Backup Ready
	
	CRITICAL_SECTION	cs ;
	
	_FriendInfo() {
		InitializeCriticalSectionAndSpinCount( &cs, (0xf0000000)|2000 ) ;
	} ;
	~_FriendInfo() { DeleteCriticalSection( &cs ) ; } ;
	void lock() { EnterCriticalSection(&cs) ; } ;
	void unlock() { LeaveCriticalSection(&cs) ; } ;
	//void lock(){ return ;} ;
	//void unlock(){ return;} ;
	
	list<_Member> * pFriendList ;
};
*/

//==============================================================

/*
// ������ �������� ���������θ� ���� ��Ŷ
#define MSG_NO_DISCONNECT					-99
struct MSG_DISCONNECT						//# ������ ������ �������� ��Ŷ #
{
	unsigned short		sLength;
	char				cMessage ;	
	void*				pDSComm ;
} ;
*/

//==============================================================


//==============================================================
/*
���� : �޽��� ���� ��Ŷ�� MMSG �� �����Ѵ�..
		Ŭ���̾�Ʈ�� ������ ��Ŷ �ش��� sCrypto, sCompressed �� ���� �׻� Ȯ���ؾ� �Ѵ�..
		sCrypto ���� 1 �� ��쿡�� ��Ŷ�� ��ȣȭ �Ǿ� ������ �ǹ��Ѵ�.
		sCompressed ���� 1 �� ��쿡�� ��Ŷ�� ����Ǿ� �����̴�.(���̰� �� ä�� �޽��� ��� ������)		
*/		

#define MMSG_NO_CONNECT_PERM			0 //2004.06.22->oneway48 insert

#define MMSG_NO_LOGIN_C_S				161
#define MMSG_NO_LOGIN_RESULT			162		// �α��� ���..

#define MMSG_NO_LOGOUT_C_S				164		// �α׿��� ��û
#define MMSG_NO_LOGOUT_REPORT_S_C		165		// ģ���� �α׿��� ����..

#define MMSG_NO_MAKE_FRIEND_REQ_C_S_C	166		// ģ�� �α� ����..
#define MMSG_NO_MAKE_FRIEND_CON_C_S		168		// ģ�� �α� ���ȿ� ���� ���(Yes/No)
#define MMSG_NO_MAKE_FRIEND_RES_S_C		169		// ģ�� ��� ��û ���

#define MMSG_NO_CUT_FRIEND_REQ_C_S		170		// ģ�� ���� ��û..
#define MMSG_NO_CUT_FRIEND_REPORT_S_C	171		// ģ�� ���� �˸�~

#define MMSG_NO_FRIEND_LIST_REQ_C_S		172		// ������ ģ�� ����Ʈ ��û
#define MMSG_NO_FRIEND_LIST_RES_S_C		173		// ģ�� ����Ʈ ���

#define MMSG_NO_MY_STATE_UPDATE_C_S		174		// ���� ���� ���� ������Ʈ ��û
#define MMSG_NO_FRIEND_STATE_REPORT_S_C	175		// ģ���� ���� ���� ���� ����..
#define MMSG_NO_CHAT					176		// ä�� �޽���..
#define MMSG_NO_CHAT_GM					177		// 2004.05.20->oneway48 insert : ��� ä�� �޽���
//#define MMSG_NO_CHAT_GROUP_CHOICE_C_S	177		// �׸� ä��(Ȯ�� ���� ����..)

#define MMSG_NO_MAKE_GROUP_REQ_C_S		178		// �׸� ����� ��û
#define MMSG_NO_MAKE_GROUP_RES_S_C		179		// �׸� ����� ����

#define MMSG_NO_CUT_GROUP_REQ_C_S		180		// �׸� ���� ��û
#define MMSG_NO_CUT_GROUP_RES_S_C		181		// �׸� ���� ����

#define MMSG_NO_JOIN_GROUP_REQ_C_S		182		// ģ���� �׸��� ����ϵ��� ��û..
#define MMSG_NO_JOIN_GROUP_RES_S_C		183		// ģ���� �׸��� ����ϵ��� ��û..


#define MMSG_NO_RENAME_GROUP_REQ_C_S	184		// �׸� �̸� ���� ��û 
#define MMSG_NO_RENAME_GROUP_RES_S_C	185		// �׸� �̸� ���� ���
#define MMSG_NO_CHAT_WHISPER			186		// �ӼӸ�
#define MMSG_CHAT_RESULT_S_C			187		// �ӼӸ� ��� ���� ��� 


// 2004.07.23->oneway48 insert 
//======================================================================================
// Mail(������) 
#define MMSG_NO_MAIL_LIST_REQ_C_S		200		// ���� ����Ʈ�� ��û�Ѵ�
#define MMSG_NO_MAIL_LIST_RES_S_C		201		// ���� ����Ʈ�� ��ȯ�޴´�.
#define MMSG_NO_MAIL_TEXT_REQ_C_S		202		// ���� ������ ��û�Ѵ�.
#define MMSG_NO_MAIL_TEXT_RES_S_C		203		// ���� ������ ��ȯ�޴´�.
#define MMSG_NO_MAIL_SEND_REQ_C_S		204		// ������ �߼��Ѵ�.
#define MMSG_NO_MAIL_SEND_RES_S_C		205		// ���� �߼ۿ� ���� ���
#define MMSG_NO_MAIL_RECV_S_C			206		// �α��� �߿� ������ �������� ���..
#define MMSG_NO_MAIL_FAIL_S_C			207		// ���� ��û�� ���� ����
#define MMSG_NO_MAIL_DROP_C_S			208		// ���� ����
// insert end

// ������ ��ü ����
//======================================================================================
#define MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S 230 // ������ ���� �߼� ��û


// ģ�� ��ȯ
//======================================================================================
#define MSG_NO_RECALL_FRIEND			226		// ģ����ȯ.

// Ư�� ���� �˸���
//======================================================================================
#define MSG_NO_NOTIEFY_INFO				227		// ������ Ư�� ������ �˸���.


//#define MMSG_NO_LOGIN_C_S
struct MMSG_MSLOGIN {

	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cCharacName[13] ;	
};

//#define MMSG_NO_LOGIN_RESULT
struct MMSG_MSLOGIN_RESULT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cResult ;			// 0 : ����

											// 1 : �߸��� ĳ���� �̸�
											// 2 : ������ �ּҰ� �ٸ�
											// 3 : �̹� �α�����...		
											// 4 : (db)��Ÿ����		
};

//#define MMSG_NO_LOGOUT_C_S
struct MMSG_MSLOGOUT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;
};

//#define MMSG_NO_LOGOUT_REPORT_S_C
struct MMSG_LOGOUT_REPORT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cCharacName[13] ;
};

// #define MMSG_NO_MAKE_FRIEND_REQ_C_S_C	// ģ�� �α� ����..			<1>
struct MMSG_MAKE_FRIEND_REQUEST {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cCharacName[13] ;	
};



//#define MMSG_NO_MAKE_FRIEND_CON_C_S		// ģ�� �α� ���ȿ� ���� ����(Yes/No)
struct MMSG_MAKE_FRIEND_CONFIRM {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cCharacName[13] ;	
	char				cResult ;			//0 : �³�(yes) , 1 : �ź�(no)
};

//#define MMSG_NO_MAKE_FRIEND_RES_S_C
struct MMSG_MAKE_FRIEND_RES {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cCharacName[13] ;	
	char				cState;				// ���� ���� �ʿ�
	char				cResult ;			// 0 : ����	(���ʴ� �뺸)
											// 1 : ��밡 �ź� (�źδ��� ĳ����(cFromCharacName)���� �뺸)

											//(���� ���ʹ� 'MMSG_MAKE_FRIEND_REQ' Packet �� ���� ĳ���Ϳ��Ը� �뺸�Ѵ�.)
											// 2 : ��� ĳ���Ͱ� ���ų�.. �α׿�����..
											// 3 : ��� ĳ������ ģ�� ��� �ο� �ʰ�..
											// 4 : ���� ĳ���� ��� �ο� �ʰ�.
											// 5 : �̹� ��ϵ� ĳ����
											// 6 : GM ĳ���� �Դϴ�.
};

//#define MMSG_NO_CUT_FRIEND_REQ_C_S		// ģ�� ����..
struct MMSG_CUT_FRIEND_REQ {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;
	
	char				cFriendCharac[13] ;	// ������ ģ��..

};

//#define MMSG_NO_CUT_FRIEND_REPORT_S_C		// ģ�� ���� �˸�.. (���� ����ڿ��� �ڽ��� �����Ǿ��ٴ°� �˸�..)
struct MMSG_CUT_FRIEND_REPORT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cCharacName[13] ;	

};

//#define MMSG_NO_FRIEND_LIST_REQ_C_S
struct MMSG_FRIEND_LIST_REQ {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;
};

//#define MMSG_NO_FRIEND_LIST_RES_S_C
struct MMSG_FRIEND_LIST_RES {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cCount ;			// ������ ����..
	_Friend				_friends[58];
};

//#define MMSG_NO_MY_STATE_UPDATE_C_S
struct MMSG_MY_STATE_UPDATE {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cState ;
								// 0 �α׿���
								// 1 �¶���
								// 2 ���� ��
								// 3 ���� ��
								// 4 �����̵� ��								
};


//#define MMSG_NO_FRIEND_STATE_REPORT_S_C
struct MMSG_FRIEND_STATE_REPORT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cCharacName[13] ;
	char				cState ;
};

//#define MMSG_NO_CHAT
//#define MMSG_NO_CHAT_GM					// 2004.05.20->oneway48 insert : 177 ��� ä�� �޽���..
//#define MMSG_NO_CHAT_WHISPER			186		// �ӼӸ�
struct MMSG_CHAT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cFromCharacName[13] ;	// ê�� �޽����� ��쿡�� �����ϰ� From �� To �� ����.. ��..�Ѥ�;
	char				cToCharacName[13] ; 
	char				cChatMsg[_XDEF_CHATMAXLENGTH] ;
};

// 2005. 1. 28 insert --------------------------------------------------------------=
enum	// MMSG_CHAT_RESULT::cCode
{
	_NOT_FIND_USER_ = 0,		// ĳ���Ͱ� �α׿����� �̰ų� ���� ĳ���� �϶�..
} ;

struct MMSG_CHAT_RESULT
{
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cToCharacName[13] ;	//	��� ĳ���� �̸�
	char				cCode ;				//	_NOT_FIND_USER_
} ;
// ---------------------------------------------------------------------------------=

////#define MMSG_NO_CHAT_GROUP_CHOICE_C_S	13
//struct MMSG_CHAT_GROUP_CHOICE {
//	short				sLength : 12 ;		// -2048 ~ 2047
//	short				sCrypto : 2 ;		// 0 : false , �������� true
//	short				sCompressed : 2 ;	//0 : false , �������� true
//	char				cMessage ;
//	char				cChoiceNum ;
//	char				cChoiceName[13][58] ;	// �������� ģ�� ����Ʈ..
//};


//#define MMSG_NO_MAKE_GROUP_REQ_C_S		// �׸� ����� ��û
struct MMSG_MAKE_GROUP_REQ {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;
	
	char				cGroupName[15] ;
};

//#define MMSG_NO_MAKE_GROUP_RES_S_C		// �׸� ����� ����
struct MMSG_MAKE_GROUP_RES {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;
	
	char				cGroupName[15] ;
	char				cResult ;			// 0 ����, 1 ����
};											

//#define MMSG_NO_CUT_GROUP_REQ_C_S			// �׸� ���� ��û
struct MMSG_CUT_GROUP_REQ {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cGroupName[15] ;
};
//#define MMSG_NO_CUT_GROUP_RES_S_C			// �׸� ���� ����
struct MMSG_CUT_GROUP_RES {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;
	
	char				cGroupName[15] ;
	char				cResult ;
};

//#define MMSG_NO_JOIN_GROUP_REQ_C_S		// ģ���� �׸��� ����ϵ��� ��û..
struct MMSG_JOIN_GROUP_REQ {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;
	
	char				cGroupName[15] ;
	char				cFriendName[13] ;
};

//#define MMSG_NO_JOIN_GROUP_RES_S_C		// ģ���� �׸��� ����ϵ��� ��û..
struct MMSG_JOIN_GROUP_RES {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;
	
	char				cGroupName[15] ;
	char				cFriendName[13] ;
	char				cResult ;			// 0���� 1����
};


//#define MMSG_NO_RENAME_GROUP_REQ_C_S		// �׸� �̸� ����
struct MMSG_RENAME_GROUP_REQ {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;
	
	char				cGroupName[15] ;
	char				cReName[15] ;
};

//#define MMSG_NO_RENAME_GROUP_RES_S_C		// �׸� �̸� ���� ���
struct MMSG_RENAME_GROUP_RES {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;
	
	char				cResult ;			// 0: ����  1: ����
};

// 2004.07.23->oneway48 insert 
//======================================================================================
// Mail(������) 

//struct _Mail {							// ����(������)
//	UINT			uiIndex ;			// ���� �ε���
//	bool			check ;				// true : ���� , false : ���ε���(������)	
//};
//
//struct _MailInfo {
//	UINT			uiIndex ;			// ���� �ε���
//	char			cFrom[13] ;			// �������
//	int				iTime ;				// �ð�
//	char			cText[101] ;		// ����
//};




//#define MMSG_NO_MAIL_LIST_REQ_C_S		200		// ���� ����Ʈ�� ��û�Ѵ�
struct MMSG_MAIL_LIST_REQ {
	_sHeader			header ;
	u_char				cMessage ;	
};
//#define MMSG_NO_MAIL_LIST_RES_S_C		201		// ���� ����Ʈ�� ��ȯ�޴´�.
struct  MMSG_MAIL_LIST_RES{
	_sHeader			header ;
	u_char				cMessage ;

	_Mail				mail[20] ;	
};
//#define MMSG_NO_MAIL_TEXT_REQ_C_S		202		// ���� ������ ��û�Ѵ�.
struct  MMSG_MAIL_TEXT_REQ {
	_sHeader			header ;
	u_char				cMessage ;
	u_int				uiIndex ;	
};
//#define MMSG_NO_MAIL_TEXT_RES_S_C		203		// ���� ������ ��ȯ�޴´�.
struct  MMSG_MAIL_TEXT_RES {
	_sHeader			header ;
	u_char				cMessage ;
	
	_MailInfo			mailInfo ;
};
//#define MMSG_NO_MAIL_SEND_REQ_C_S		204		// ������ �߼��Ѵ�.
struct MMSG_MAIL_SEND_REQ {						// (��������)
	_sHeader			header ;
	u_char				cMessage ;				// header(3) + MailHeader(22) + strLen
	char				cToCharac[13] ;
	char				cStrLen ;

	char				cText[101] ;
};
//#define MMSG_NO_MAIL_SEND_RES_S_C		205		// ���� �߼ۿ� ���� ���
struct MMSG_MAIL_SEND_RES {
	_sHeader			header ;
	u_char				cMessage ;
};

//#define MMSG_NO_MAIL_RECV_S_C			206		// �α��� �߿� ������ �������� ���..
struct MMSG_MAIL_RECV {
	_sHeader			header ;
	u_char				cMessage ;
	u_int				uiIndex ;				// ������ ������ �ε���
};



//#define MMSG_NO_MAIL_FAIL_S_C			206		// ���� ��û�� ���� ����
// cCmd : MMSG_NO_MAIL_LIST_RES_S_C : cResult == NULL
// cCmd : MMSG_NO_MAIL_TEXT_RES_S_C : cResult == NULL
// cCmd : MMSG_NO_MAIL_SEND_RES_S_C : cResult == 0 : �ش� ĳ���� �̸� �������� ����
//				     							  1 : ���� ���Ϲڽ��� ������ ���̻� ������
//												  2 : ���� ���� �ź���
//												  3 : ��Ÿ ����
//												  4 : GM ���Դ� ������ ���� �� �����ϴ�.
struct MMSG_MAIL_FAIL {
	_sHeader			header ;
	u_char				cMessage ;
	u_char				cCmd ;			// define �� �޽��� ��ȣ�� �״�� ����Ѵ�.
	char				cResult ;		// �����.
};

//#define MMSG_NO_MAIL_DROP_C_S			208		// ���� ����
struct MMSG_MAIL_DROP {
	_sHeader			header ;
	u_char				cMessage ;
	u_int				uiIndex ;				// ������ ���� �ε���..
};

// insert end

//2004.06.22->oneway48 insert
//#define MMSG_NO_CONNECT_PERM			0
struct MMSG_CONNECT_PERM {
	short				sLength : 12 ;
	short				sCrypto : 2 ;
	short				sCompressed : 2 ;
	u_char				cMessage ;
};

//#define MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S 230 // ������ ���� �߼� ��û
struct MMSG_MAIL_PUBLIC_SEND_REQ {
 _sHeader   header ;
 u_char    cMessage ;

 char    cNames[100][13] ;
 char    cText[101] ;
};

//#define MSG_NO_RECALL_FRIEND			226		// ģ����ȯ.
struct MSG_RECALL_FRIEND
{
	_sHeader	header;
	u_char	ucMessage;

	u_char	ucFriendMode;	// EN_MSG_RECALL_FRIEND
};

enum EN_MSG_RECALL_FRIEND
{
	en_rf_send_request_recall=0,		// ģ�� ��ȯ ��û�� ������ ����
	en_rf_recv_request_recall,			// ģ�� ��ȯ ��û�� �������� ����.

	en_rf_send_request_goto,	// ģ���� �ִ� ������ ���ڴٴ� ��û.
	en_rf_recv_request_goto,	// �ٷ� ���� ��û�� �޾Ҵ�.

	en_rf_send_answer_yes=10,			// ģ�� ��ȯ ��û ����.
	en_rf_send_answer_no,				// ģ�� ��ȯ ��û �ź�.

	en_rf_send_goto_answer_yes,//
	en_rf_send_goto_answer_no,

	en_rf_recv_answer_yes=20,			// ��û ���� ��Ŷ�� ����. ==> �����ߴ�.
	en_rf_recv_answer_no,				// ��û �ź� ��Ŷ�� ����.

	en_rf_recv_goto_answer_yes,// 
	en_rf_recv_goto_answer_no,// 

	en_rf_show_me=30,	//��ȯ�Ǿ����� �˸�
	en_rf_show_animation,	// �̵� ����/�̵� ���Ŀ� �ֺ��� �˷�, �ִϸ��̼� �ϵ��� �Ѵ�.

	en_rf_fail_to_request=50,			// ��û�� �����ߴ�. 
};

// en_rf_send_request_recall=0,		// ģ�� ��ȯ ��û�� ������ ����
struct MSG_RF_SEND_REQUEST_RECALL : public MSG_RECALL_FRIEND
{
	u_char	cNeedItemSlot;	// �ʿ� ������ ������ �ε���. �������� �˻�ÿ� ���δ�.

	char	cTargetName[en_charac_name_length+1];	// ����� �̸�.

	float	fTargetX;	// Ŭ���̾�Ʈ���� �̸� ����Ͽ�, ��ǥ���� ������.
	float	fTargetZ;	// ���������� �Ÿ�(5) �˻縸 �Ѵ�.
};

// en_rf_recv_request_recall, // ģ�� ��ȯ ��û�� �������� ����.
struct MSG_RF_RECV_REQUEST_RECALL : public MSG_RECALL_FRIEND
{
	u_char	ucFromZoneIndex;	// ��� �������� ��ȯ�ϰ� �ִ°�.
	char	cFromName[en_charac_name_length+1];
};

//en_rf_send_request_goto, // ģ���� �ִ� ������ ���ڴٴ� ��û.
struct MSG_RF_SEND_REQUEST_GOTO : public MSG_RECALL_FRIEND
{
	u_char	ucNeedItemSlot;
	char	cTargetName[en_charac_name_length+1];
};

//en_rf_recv_request_goto, // �ٷ� ���� ��û�� �޾Ҵ�.
struct MSG_RF_RECV_REQUEST_GOTO : public MSG_RECALL_FRIEND
{
	char	cFromName[en_charac_name_length+1];
};
	
// en_rf_send_answer_yes=10, // ģ�� ��ȯ ��û ����.
struct MSG_RF_SEND_ANSWER_YES : public MSG_RECALL_FRIEND
{
	char cFromName[en_charac_name_length+1];
};


//	en_rf_send_answer_no, // ģ�� ��ȯ ��û �ź�.
struct MSG_RF_SEND_ANSWER_NO : public MSG_RECALL_FRIEND
{
	u_char	ucNoReason;		
	char	cFromName[en_charac_name_length+1];
	/*
		ucNoReason : 
		0 - ������ �������� ��.
		1 - ���� ��û�� ���� �� ���� ���´�. 
			( ��û�޾��� ��, ���°˻�� �����Ͽ�, �� �޴� �����̸� �� ������ ������. )
	*/
};

//en_rf_send_goto_answer_yes,//
struct MSG_RF_SEND_GOTO_ANSWER_YES : public MSG_RECALL_FRIEND
{
	char	cFromName[en_charac_name_length+1];
	float	fTargetX;	// ��ġ�� ä���� ������. 
	float	fTargetZ;
};

//en_rf_send_goto_answer_no,//
struct MSG_RF_SEND_GOTO_ANSWER_NO : public MSG_RECALL_FRIEND
{
	u_char	ucNoReason;
	char	cFromName[en_charac_name_length+1];
};

//	en_rf_recv_answer_yes=20, // ��û ���� ��Ŷ�� ����. ==> �����ߴ�.
struct MSG_RF_RECV_ANSWER_YES : public MSG_RECALL_FRIEND
{
	char	cTargetName[en_charac_name_length+1]; // �ʱ⿡ ��û���´� ����� �̸�.
	u_char	ucItemSlot;	// ģ�� ��ȯ �������� �ִ� �ڸ�.
	u_char	ucLeftCount;	// ���� ����. 0�̸� ���� ����.
};


//	en_rf_recv_answer_no, // ��û �ź� ��Ŷ�� ����.
struct MSG_RF_RECV_ANSWER_NO : public MSG_RECALL_FRIEND
{
	u_char	ucNoReason;	// MSG_RF_SEND_ANSWER_NO �� ucReason �� ����.
	char	cTargetName[en_charac_name_length+1];
};



//en_rf_recv_goto_answer_yes,                              // 
struct MSG_RF_RECV_GOTO_ANSWER_YES : public MSG_RECALL_FRIEND
{
	char	cTargetName[en_charac_name_length+1];
	u_char	ucItemSlot;
	u_char	ucLeftCount;
	u_char	ucTargetZone;	// �ʿ����� �𸣰���.

	float	fTargetX;
	float	fTargetZ;
};

//en_rf_recv_goto_answer_no,                                // 
struct MSG_RF_RECV_GOTO_ANSWER_NO : public MSG_RECALL_FRIEND
{
	u_char	ucNoReason;	// EN_RF_FAIL_REASON

	char	cTargetName[en_charac_name_length+1];
};

// en_rf_show_me=30,//��ȯ�Ǿ����� �˸�
struct MSG_RF_SHOW_ME : public MSG_RECALL_FRIEND
{
	u_short	uiUID;
	char	cName[en_charac_name_length+1];
	
	float	fPosX;
	float	fPosZ;
};

//en_rf_show_animation=31
struct MSG_RF_SHOW_ANIMATION : public MSG_RECALL_FRIEND
{
	u_char ucRFAni; // 0 - ��ȯ��(��ٸ���ĳ��) ������. 1 - �̵����� �ִϸ��̼�, 2 - �̵����� �ִϸ��̼�.
	u_short	usUID;	// �ڱ��� �����Ѵ�.
};

enum
{
	en_rf_answer_no=0,	// no ����.
	en_rf_fail_no_target=1,	// 1 Ÿ���� ����.
	en_rf_fail_no_item,	// ��ȯ ������ ����. 
	en_rf_fail_wrong_point,	// ��ȯ ������ ��ġ�� �̻��ϴ�. 
	en_rf_fail_enemy,	// ���� �����̴�.
 
	en_rf_fail_my_con_trade=10, // (�ڽ���) �ŷ����̴�.
	en_rf_fail_my_con_combat, // (�ڽ���) ����� �������̴�.
	en_rf_fail_my_con_zone,	// (�ڽ���) �пյ�/ȣ���� �� ���� �����̴�.
	en_rf_fail_my_con_exbattle, // (�ڽ���) �� �������̴�. 
	en_rf_fail_my_busy,	// (�ڽ���) �ٸ� ĳ���Ϳ� ģ�� ��ȯ ���̴�.   
    en_rf_fail_my_state_pk_mode,	// (�ڽ���) PK ����̴�.
    
	en_rf_fail_target_con_trade=20,	// �ŷ����̴�.
	en_rf_fail_target_con_combat, // ����� �������̴�.
	en_rf_fail_target_con_zone, // �пյ�/ȣ���� �� ���� �����̴�.
	en_rf_fail_target_con_exbattle, // �� �������̴�.
	en_rf_fail_target_busy, // �ٸ� ���� ģ�� ��ȯ ���̴�.
    en_rf_fail_target_state_pk_mode,// ����� ���� PK ����.
};

//	en_rf_fail_to_request=50, // ��û�� �����ߴ�. 
struct MSG_RF_FAIL_TO_REQUEST : public MSG_RECALL_FRIEND
{
	u_char	ucFail;			
	char	cTargetName[en_charac_name_length+1];
	/*
		ucFail
		  - 0 : ����� �α��� ���� �ƴϴ�.
		  - 1 : ��ȯ �������� ����.(�Ǵ� �ƴϴ�. )
		  - 2 : ��û�� ���� �� ���� ���´�.
		    ( Ŭ���̾�Ʈ���� ���� �˻��ϰ� �����Ƿ�, 2 ���� ������ ���� �� )
		  - 3 : ��ȯ ��ġ�� ĳ���Ϳ� �ʹ� �ִ�.
	*/
};

//#define MSG_NO_NOTIEFY_INFO				227		// ������ Ư�� ������ �˸���.
struct MSG_NOTIFY_INFO 
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			notify_type;		// �Ʒ��� EN_MSG_NOTIFY_INFO_TYPE  

};

enum EN_MSG_NOTIFY_INFO_TYPE
{
	en_notify_type_item_use = 0,
};

struct MSG_NI_ITEM_USE : public MSG_NOTIFY_INFO
{
	u_short			usUniqueID;			// ������� ���̵�

	char			cItemType;			// ���� ������ ����
	short			sItemID;
};

#pragma pack( pop, enter_ )
#pragma once

#pragma pack( push, enter_ )
#pragma pack(1)

#include "Global_Define.h"
#include "..\NetWork\NetWork.h"
#include <list>
using namespace std;


class _FriendInfo ;
class _h_DS_Com ;

struct _Member {	
	char			characName[16] ;	// ĳ���� �̸� �Ǵ� �׷� �̸����� ���δ�.	
	_FriendInfo *	pFriendInfo ;		// �ش� _FriendList �� *(������)
};

struct _Mail {							// ����(������)
	UINT			uiIndex ;			// ���� �ε���
	bool			check ;				// true : ���� , flase : ���ε���(������)	
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

	char			cMode ;				// ����
	enum
	{
		_PRIVATE,	
		_PUBLIC,
		_FROM_GM,
		_SYSTEM_MSG_OR_KICK,
	};

	char			cStrLen ;	
	char			cText[101] ;		// ����

};

struct _Friend {
	char			cChoiceName[15] ;	// ģ�� �̸�.. �Ǵ� �׷� �̸�..
	char			cState	;			// ����..
} ;


//==============================================================


// ������ �������� ���������θ� ���� ��Ŷ
#define MSG_NO_DISCONNECT					0
struct MSG_DISCONNECT						//# ������ ������ �������� ��Ŷ #
{
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	
	u_char				cMessage ;	
	void*				pDSComm ;
} ;


// DS �� ���� ĳ���� ������ �뺸�޴´�.
#define		UMSG_CHARAC_DELETE_DS_MS_NUM	125
struct UMSG_CHARAC_DELETE_DS_MS {
	int			iKey ;
	char		cMessage ;
	
	char		cCharacName[13] ;
	int			iUid ;
};


// GM Charac setting
#define		UMSG_GM_CHARAC_SET_DS_MS_NUM	126
struct UMSG_GM_CHARAC_SET_DS_MS {
	int			iKey ;
	char		cMessage ;
	
	char		cCharacName[13] ;
	char		cClass ;
};


enum
{
	en_or_name_length=24,
};

#define  UMSG_CHARAC_OR_KICK_NUM			127
struct UMSG_CHARAC_OR_KICK {
	int			iKey ;
	char		cMessage ;

	char		cCharacName[13] ;
	char		cORName[en_or_name_length+1] ;
};


#define		MSG_SYSTEM_OFF_REQ_NUM			10	// �ý��� ���� GMS -> DS
struct MSG_SYSTEM_OFF_REQ {
	int		iKey;
	char	cMessage;
	char	cServer_num;	
};



// GMS �α���
#define  MMSG_NO_GM_LOGIN		18
struct MMSG_GM_LOGIN {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true

	u_char				cMessage ;
	char				cCharacName[13] ;	
};




//==============================================================
/*
���� : �޽��� ���� ��Ŷ�� MMSG �� �����Ѵ�..
		Ŭ���̾�Ʈ�� ������ ��Ŷ �ش��� sCrypto, sCompressed �� ���� �׻� Ȯ���ؾ� �Ѵ�..
		sCrypto ���� 1 �� ��쿡�� ��Ŷ�� ��ȣȭ �Ǿ� ������ �ǹ��Ѵ�.
		sCompressed ���� 1 �� ��쿡�� ��Ŷ�� ����Ǿ� �����̴�.(���̰� �� ä�� �޽��� ��� ������)		
*/		


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
#define MMSG_NO_CHAT					176		// �Ϲ� ä�� �޽���..
#define MMSG_NO_CHAT_GM					177		// ��� ä�� �޽���..
//#define MMSG_NO_CHAT_GROUP_CHOICE_C_S	177		// �׸� ä��(Ȯ�� ���� ����..)

#define MMSG_NO_MAKE_GROUP_REQ_C_S		178		// �׸� ����� ��û
#define MMSG_NO_MAKE_GROUP_RES_S_C		179		// �׸� ����� ����

#define MMSG_NO_CUT_GROUP_REQ_C_S		180		// �׸� ���� ��û
#define MMSG_NO_CUT_GROUP_RES_S_C		181		// �׸� ���� ����

#define MMSG_NO_JOIN_GROUP_REQ_C_S		182		// ģ���� �׸��� ����ϵ��� ��û..
#define MMSG_NO_JOIN_GROUP_RES_S_C		183		// ģ���� �׸��� ����ϵ��� ��û..

#define MMSG_NO_RENAME_GROUP_REQ_C_S	184		// �׸� �̸� ����
#define MMSG_NO_RENAME_GROUP_RES_S_C	185		// �׸� �̸� ���� ���

#define MMSG_NO_CHAT_WHISPER			186		// �ӼӸ�
#define MMSG_CHAT_RESULT_S_C			187		// ä�� �޽��� �����Ŷ..(�������� ��츸 ����.)



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
#define MMSG_NO_MAIL_DROP_S_C			209		// ���� ���� ���
#define MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S	230	// ������ ���� �߼� ��û

#define MMSG_NO_MAIL_SEND_REQ_FROM_GM	231		// ������ �߼��Ѵ�.



struct _sHeader {
	short		sLength : 12 ;		// -2048 ~ 2047
	short		sCrypto : 2 ;		// 0 : false , �������� true
	short		sCompressed : 2 ;	//0 : false , �������� true
};


#define MMSG_NO_CONNECT_PERM			0
struct MMSG_CONNECT_PERM {
	short				sLength : 12 ;
	short				sCrypto : 2 ;
	short				sCompressed : 2 ;
	u_char				cMessage ;
};

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
	char				cState ;			// ģ�� ���� ����..
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


enum
{
	_CHARAC_STATE_LOGOFF,	// 0.��������
	_CHARAC_STATE_LOGON,	// 1.�¶���
	_CHARAC_STATE_FIGHT,	// 2.������
	_CHARAC_STATE_ABSENCE,	// 3.������
	_CHARAC_STATE_MOVING,	// 4.�����̵���
} ;


//#define MMSG_NO_MY_STATE_UPDATE_C_S
struct MMSG_MY_STATE_UPDATE {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cState ;
											// 0 �α׿���
											// 1 �¶���
											// 2 ������											
											// 3 �ڸ����
											// 4 ���� �̵���..
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
//#define MMSG_NO_CHAT_GM					177		// ��� ä�� �޽���..
struct MMSG_CHAT {
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;
	
	char				cFromCharacName[13] ;	// ê�� �޽����� ��쿡�� �����ϰ� From �� To �� ����.. ��..�Ѥ�;
	char				cToCharacName[13] ; 
	char				cChatMsg[81] ;
};


// #define MMSG_CHAT_RESULT_S_C				
enum							// MMSG_CHAT_RESULT::cCode
{
	_NOT_FIND_USER_ = 0,		// ĳ���Ͱ� �α׿����� �̰ų� ���� ĳ���� �϶�..
	//_LOGOFF_FRIEND_,			// �޽��� ä�ÿ��� ģ���� �α׿������� �϶�..
} ;
struct MMSG_CHAT_RESULT
{
	short				sLength : 12 ;		// -2048 ~ 2047
	short				sCrypto : 2 ;		// 0 : false , �������� true
	short				sCompressed : 2 ;	//0 : false , �������� true
	u_char				cMessage ;

	char				cToCharacName[13] ;	//	��� ĳ���� �̸�
	char				cCode ;				//	_NOT_FIND_USER_
};

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


//======================================================================================
// Mail(������) 

//struct _Mail {							// ����(������)
//	UINT			uiIndex ;			// ���� �ε���
//	bool			check ;				// false : ���� , thue : ���ε���(������)	
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

	_Mail				mail[MAX_MAIL_COUNT] ;
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
//#define MMSG_NO_MAIL_SEND_REQ_FROM_GM	231		// GM ������ �߼��Ѵ�.
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
//#define MMSG_NO_MAIL_FAIL_S_C			207		// ���� ��û�� ���� ����
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

//#define MMSG_NO_MAIL_DROP_S_C			209		// ���� ���� ���
struct MMSG_MAIL_DROP__RES {
	_sHeader			header ;
	u_char				cMessage ;
	u_int				uiIndex ;				// ������ ���� �ε���..
};

//#define MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S	230	// ������ ���� �߼� ��û
struct MMSG_MAIL_PUBLIC_SEND_REQ {
	_sHeader			header ;
	u_char				cMessage ;

	char				cNames[100][13] ;
	char				cText[101] ;
};



#pragma pack( pop, enter_ )


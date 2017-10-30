#pragma once

#include "Global_struct.h"
#include "..\Billing\Bill_Packet.h"

#pragma pack( push, enter_tcp_packet )
#pragma pack(1)

struct CMSG_PROTOTYPE {
	u_short				usLength ;
	u_char				cMessage;
};


#define MAX_CHAT_LENGTH			81

/*

0 ~ 179 ������ ����..

*/

// DS ������ ���� ������Ŷ
#define CMSG_NO_DISCONNECT					255
struct CMSG_DISCONNECT						//# ������ ������ �������� ��Ŷ #
{
	unsigned short		sLength;
	u_char				cMessage ;	
} ;


/* =====================================================================================
�α��ΰ� ���� �̵� ���� : 0 - 7 ���� - (���� �޽���)
===================================================================================== */
#define CMSG_NO_GSCONNECT_GS_DS					2
#define CMSG_NO_GSCONNECT_DS_GS					CMSG_NO_GSCONNECT_GS_DS
#define CMSG_NO_ENTERSERVER_CHECK_REQ_GS_DS		3
#define CMSG_NO_ENTERSERVER_CHECK_RESULT_DS_GS	4

// billing packet
#define CMSG_NO_BILL_REQ						6		// ���� ����  ��û �޽��� 
#define CMSG_NO_BILL_INFO						7		// �������� �˸�

/* =====================================================================================
�κ񿡼� - ĳ���� ����/����/���� ���� 8 - 15
===================================================================================== */
#define CMSG_NO_CHARAC_SELECT_GS_DS				8		// ĳ���� â���� ĳ���͸� �����ߴ�.
#define CMSG_NO_CHARAC_CREATE_GS_DS				9		// ĳ���� ����
#define CMSG_NO_CHARAC_DELETE_GS_DS				10		// ĳ���� ����
#define CMSG_NO_USER_CHARAC_LIST_DS_GS			11		// ĳ���� ����Ʈ
#define CMSG_NO_CHARAC_RESULT_DS_GS				12		// ���� 3���� ��Ŷ�� ���� �������� ���δ�.
// �ڼ��� �� ��Ŷ ���Ǻο� �ִ�.
#define CMSG_NO_CHARAC_LIST_REQ_GS_DS			13		// ĳ���� ����Ʈ�� ���û

/* =====================================================================================
�α׾ƿ��� ���� �̵� ��û : 16 - 23
===================================================================================== */
#define CMSG_NO_LOGOUT_GS_DS					16		// �ܼ� �α׾ƿ�
#define CMSG_NO_REQ_SV_MOVE_GS_DS				18		// ������ �̵� ��û
#define CMSG_NO_SV_MOVE_RESULT_DS_GS			19		// ���
#define CMSG_NO_UNI_GROUP_LOBBY					20
#define	CMSG_NO_CHECK_DSGS_PACKET				21
#define CMSG_NO_ACCOUNT_INFO					22		// ���� ���õ� ���� ��Ŷ : 0. �÷��� �ð�.


/* =====================================================================================
ĳ���� ���� ���� �Ǵ� �κ�� ���������  24 - 39
�κ��丮/����/����Ʈ.. ���� ��� ���� ������Ʈ�� ���⿡��.
===================================================================================== */
#define CMSG_NO_CHARAC_ALL_INFO_DS_GS			24		// DS->GS ALL CHARAC INFO REQUEST MESSAGE
#define CMSG_NO_DEPOT_REQ_GS_DS					25		// GS->DS Depot data request..
#define CMSG_NO_DEPOT_RES_GS_DS					26		// DS->GS Depot data result..


#define CMSG_NO_CHARAC_RETURN_LOBBY_GS_DS		39		// ������ �κ�� ����.

/* =====================================================================================
ä�� �޽��� : �/���� ����. 40 - 47
===================================================================================== */
#define CMSG_NO_ZONE_CHAT_WHISPER_GS_DS			40		// GS<>DS �ӼӸ�

#define CMSG_NO_SAVE_FATIGUE_WEIGHT				(44)

/* =====================================================================================
���� ���� ������Ʈ ����  48 ~
===================================================================================== */
#define CMSG_NO_UPDATE_CHARAC_ALL_				57			// ĳ������ ��� ���� ������Ʈ
#define CMSG_NO_ALL_UPDATE_RESULT_				57		// all update �� ���� ���

/* =====================================================================================
ȯ������ ������Ʈ  64 ~
===================================================================================== */
// ��ŷ ��ȣ�� �����Ͽ� DS <-> GS ����ȭ�� �����ϱ� ���� ��Ŷ�̴�.
// DS �� ���� �ֱ�� DB���� ��ŷ������ �����ϰ�.. �� ������ �����ϸ�.. GS�� ��û�� ����
// ��ŷ ������ �������ش�..
#define CMSG_NO_RANKING_INFO_REQ				64			// ��ŷ ������ �䱸
#define CMSG_NO_RANKING_INFO_RES				65			// ��ŷ ������ ����
#define CMSG_NO_RANKING_UPDATE					66			// ��ŷ ������ ������Ʈ �Ѵ�..(�񹫵�..)


#define CMSG_NO_QUESTRANK_DS_REQ				67			// ����Ʈ��ŷ ������ �䱸
#define CMSG_NO_QUESTRANK_DS_RES				CMSG_NO_QUESTRANK_DS_REQ	// ����Ʈ��ŷ ������ ����
#define CMSG_NO_QUESTRANK_DS_UPDATE				68			// ����Ʈ��ŷ ������ ������Ʈ

#define CMSG_NO_SHIFTSTATEFLAG					70			// ������ ���º����û(and ��� - �������� ��� ������ ��ε�ĳ���� �ؾ���)
#define CMSG_NO_QUESTCOUNT_REQ					71			// ����Ʈ ����Ʈ ��û(������Ʈ ��û ����)
#define CMSG_NO_QUESTCOUNT_RES					CMSG_NO_QUESTCOUNT_REQ	//����Ʈ ī��Ʈ ����

/* =====================================================================================
�̺�Ʈ ���� �޼���. 90 ~ 99 ������. ���.
===================================================================================== */
#define CMSG_NO_EVENT_AUTH_REQ					90			// �̺�Ʈ ���� ��û ��Ŷ : ������ �� ������ ��Ŷ���� ��ü��.
#define CMSG_NO_EVENT_RESULT					CMSG_NO_EVENT_AUTH_REQ


// ���� �� �̺�Ʈ ��Ŷ
#define CMSG_NO_FREE_MATCH						(91)
// GM �̺�Ʈ.
#define CMSG_NO_GM_EVENT						(92)		// GM �̺�Ʈ ��Ŷ.


#define CMSG_NO_PKEVENT							98		// ��� ���� ���� ��Ŷ.


/*==========================================================================================]
[				DS <-> GS																	]
[===========================================================================================]
[	���� :	UDP �޽��������͵��� TCP ��Ŷ ������ �����Ͽ��� ( 2005-03-8 )					]
[											jkHong Add...	-								]
[==========================================================================================*/
// DS -> GS
#define		CMSG_NO_GSINFO_REQ_DS_GS			100	// GS�� ���� ���� ��û
#define		CMSG_NO_GSINFO_RES_GS_DS			101	// ���� ���� ���� *
#define		CMSG_NO_USER_LOGOUT_DS_GS			102	// ���� �α׾ƿ�
#define		CMSG_SHOWTEXT_REQ_DS_GS				103	// GS�� �������� ��� ��û
#define		CMSG_NO_CREATE_ITEM_GS_DS			105 // ITEM ����
#define		CMSG_SYSTEM_OFF_REQ_NUM				110
#define		CMSG_N0_GS_ON_DS_GS					111	// �ý��� ����� DS -> GS
#define		CMSG_N0_GS_OFF_DS_GS				112	// �ý��� ���� DS -> GS

#define		CMSG_NO_ALL_SPELL_REQ				(113)	// �鵵/�浵 �Ǵ� ��ü�� �ش� ���� ����. :GS->DS
#define		CMSG_NO_ALL_SPELL_APPLY				CMSG_NO_ALL_SPELL_REQ	// 113 ����� DS -> GS �� ������ ��Ŷ. ������ ����.
#define		CMSG_NO_QUEST_REWARD_NOTICE			(114)	// ����Ʈ�� ���� ���, ������ �� ������ �˸� ��.
#define		CMSG_NO_ALL_SYSTEM_MSG				(115)	// GS->DS �� �������� �޼��� ���.

#define		CMSG_NO_LTS							(116)	// ��ġ ����.

//////////////////////////////////////////////////////////////////////////
// ģ�� ��ȯ : 20070604 �߰�.
//////////////////////////////////////////////////////////////////////////
#define		CMSG_NO_RECALL_FRIEND				(117)

#define		CMSG_NO_SETPOSITION_REQ_DS_GS		120	// ĳ���� ��ġ �ʱ�ȭ ��û

/*
#define MSG_NO_ITEM_SHOP						131
#define CMSG_NO_ITEM_SHOP						MSG_NO_ITEM_SHOP

(131)���� ���� ���� ���ǰ� ����.
(139)���� ���� ���� ���ǰ� ����.
ProjectRoot/ItemShop/ItemShop_Packet.h �� ���ǵǾ���
*/

#define CMSG_NO_COMMOM_REQ						(135)
#define CMSG_NO_COMMON_ACK						(135)
#define CMSG_NO_COMMON_NTF						(135)

#define		CMSG_NO_CONSOLE_COMMAND				(139)


//////////////////////////////////////////////////////////////////////////
//	135�� ������ [������Ŷ]���� ����մϴ�.
//	�� ��ȣ ã�� ���..
//////////////////////////////////////////////////////////////////////////



//  DS -> GS  (GMS command)  140~
#define		CMSG_GM_EDIT_COLLECTION_NUM		140		// ����(��ǰ) ������ ����Ʈ
#define		CMSG_GM_EDIT_EXPRATE_NUM		141		// ���� ���� ����

#define		CMSG_GM_EDIT_VAR_NO				142		// ����Ʈ ��ǥ ��ȣ.
#define		CMSG_GM_EDIT_ITEM_NUM			143		// ������ ���� ����
#define		CMSG_GM_EDIT_NICK_ID_NUM		144		// ��ȣ ����
#define		CMSG_GM_EDIT_INCHANTVALU_NUM	145		// ���� ����

#define		CMSG_ENV_SETTING_NUM			(146)	// DS ���� ���� ȯ���� ������ �� ���δ�.


// 142 ~ 149 �� ������ ray �� ��带 ���� ����մϴ�.  �Ʒ��ʿ� �ֽ��ϴ�.

#define		CMSG_GM_EDIT_LEVEL_NUM			150		//	���� ����Ʈ
#define		CMSG_GM_EDIT_VITAL_NUM			151		//	�� ����Ʈ
#define		CMSG_GM_EDIT_ZEN_NUM			152		//	�� ����Ʈ
#define		CMSG_GM_EDIT_SOUL_NUM			153		//	�� ����Ʈ
#define		CMSG_GM_EDIT_HEART_NUM			154		//	�� ����Ʈ
#define		CMSG_GM_EDIT_JUNGGI_NUM			155		//	�� ����Ʈ
#define		CMSG_GM_EDIT_LEFTPOINT_NUM		156		//	���� ����Ʈ ����Ʈ
#define		CMSG_GM_EDIT_ZONE_NUM			157		//	�� �ٲٱ�
#define		CMSG_GM_EDIT_CODE_NUM			158		//	�ڵ� �ٲٱ�
#define		CMSG_GM_EDIT_PARTY_NUM			159		//	��Ƽ �ٲٱ�
#define		CMSG_GM_EDIT_NAME_NUM			160		//	�̸� �ٲٱ�(���� �̸��� �����ϴ��� ����� �̹� ����)
#define		CMSG_GM_EDIT_NICK_NUM			161		//	�г��� �ٲٱ�(���� �г����� �����ϴ��� ����� �̹� ����)
#define		CMSG_GM_ADD_INVITEM_NUM			162		//	�κ��丮�� ������ �߰�
#define		CMSG_GM_REM_INVITEM_NUM			163		//	�κ��丮�� ������ ����
#define		CMSG_GM_ADD_DEPITEM_NUM			164		//	����ҿ� ������ �߰�
#define		CMSG_GM_REM_DEPITEM_NUM			165		//	����ҿ� ������ ����
#define		CMSG_GM_ADD_INVGOLD_NUM			166		//	�κ��丮�� �� ����
#define		CMSG_GM_ADD_DEPGOLD_NUM			167		//	������� �� ����
#define		CMSG_GM_ADD_SKILL_NUM			168		//	��ų �߰�
#define		CMSG_GM_REM_SKILL_NUM			169		//	��ų ����
#define		CMSG_GM_EDIT_SKILL_NUM			170		//	��ų ����Ʈ(��ų step��ȭ)
#define		CMSG_GM_EDIT_CONTRIBUTE_NUM		171		//	���� �⿩�� ����

#define		CMSG_GM_ADD_MASTERY_NUM			172		//	���� �߰�
#define		CMSG_GM_REM_MASTERY_NUM			173		//	���� ����
#define		CMSG_GM_EDIT_MASTERY_NUM			174		//	���� ����Ʈ(���� ����Ʈ ��ȭ)
#define		CMSG_GM_EDIT_LEFTMASTERYPOINT_NUM	175	//	���� ���� ����Ʈ ����Ʈ
#define		CMSG_GM_EDIT_CHARAC_ROLE_NUM		176		//	���� �ٲٱ�
#define		CMSG_GM_EDIT_CHARAC_CLASSGRADE_NUM	177	//	���ҵ�� �ٲٱ�
#define		CMSG_GM_EDIT_CHARAC_MUTE_TIME_NUM	178		// ä�ñ�����Ű��
#define		CMSG_GM_EDIT_JIN_NUM			179		// �� ����
// 180 �̸� 


// 180 ~ 209 ��Ƽ �޽���.





//#define CMSG_NO_UPDATE_CHARAC_ALL_				57			// ĳ������ ��� ���� ������Ʈ
//#define CMSG_NO_ALL_UPDATE_RESULT_				57		// all update �� ���� ���
struct MSG_UPDATE_CHARAC_ALL
{
	u_short				usLength;
	u_char				cMessage;

	char				cAccount[en_max_lil+1] ;			// ����
	char				cCharacName[13] ;		// ĳ���̸�

	_CHARAC				charac;					// ĳ���� �̸��� �� �ȿ� ���Ե�.
	_USER_DEPOT			storage;				// â�� ����..
	_ACCOUNT_ITEM_USE_STATUS	aius ;			// 

	_CHARAC_ETC_QUICKSLOT		quickslot;		// 

#ifdef _PD_CHARAC_LTS_
	_CHARAC_LTS			characLts;
#endif

	u_int				uiCheckFlag_Login : 1;			// Is Check Login
	u_int				uiCheckFlag : 31;				// Reserve

#ifdef _PD_EVENT_INVITE_FRIEND_
	int					iIF_DBIndex;
	int					iIF_EndTime;			//
#endif	// _PD_EVENT_INVITE_FRIEND_
};


struct CMSG_ALL_UPDATE_RESULT
{
	u_short				usLength;
	u_char				cMessage;

	char				cAccount[en_max_lil+1] ;			// ����
	char				cCharacName[13] ;		// ĳ���̸�

	char				cResult ;				// 0.����, 1.����	
};


//#define CMSG_NO_SHIFTSTATEFLAG					70			// ������ ���º����û(and ��� - �������� ��� ������ ��ε�ĳ���� �ؾ���)
struct MSG_SHIFTSTATEFLAG
{
	u_short				usLength;
	u_char				cMessage;

	u_int				uiStateFlag;
};



//#define CMSG_NO_QUESTCOUNT_REQ					71			// ����Ʈ ����Ʈ ��û(������Ʈ ��û ����)
//#define CMSG_NO_QUESTCOUNT_RES					CMSG_NO_QUESTCOUNT_REQ	//����Ʈ ī��Ʈ ����
struct MSG_QUESTCOUNT_REQ
{
	u_short				usLength;
	u_char				cMessage;

	u_short				usQuestID;
	u_char				ucQuestType: 4;	//ī��Ʈ Ÿ��. 0: ����, 1: �Ϸ�

	u_char				ucReqType: 4;	//0: �����Ϳ�û, 1: ������Ʈ��û

	int					iCount;
};

//#define CMSG_NO_QUESTCOUNT_RES					CMSG_NO_QUESTCOUNT_REQ	//����Ʈ ī��Ʈ ����
struct MSG_QUESTCOUNT_RES
{
	u_short				usLength;
	u_char				cMessage;

	u_short				usQuestID;
	u_char				ucQuestType;	//ī��Ʈ Ÿ��. 0: ����, 1: �Ϸ�

	int					iCount;
};

/* =====================================================================================
�̺�Ʈ ���� �޼���. 90 ~ 99 ������. ���.
===================================================================================== */
//#define CMSG_NO_EVENT_AUTH_REQ					90			// �̺�Ʈ ���� ��û ��Ŷ : ������ �� ������ ��Ŷ���� ��ü��.
//#define CMSG_NO_EVENT_RESULT					CMSG_NO_EVENT_AUTH_REQ

#define _EVENT_KEY_MAX_LENGTH_			32	// 31���ڱ����� ���.
struct CMSG_EVENT_AUTH_REQ
{
	u_short				usLength;
	u_char				cMessage ;

	u_char				ucMode;

	CMSG_EVENT_AUTH_REQ( u_short length, u_char mode )
		: usLength(length), cMessage(CMSG_NO_EVENT_AUTH_REQ), ucMode(mode)
	{
	}
};


enum EN_EVENT_AUTH_MODE
{
	en_event_auth_req_with_key = 0,	// 
	en_event_auth_cancel=1,
	en_event_auth_success=2,
	en_event_auth_check_req=3,

	// Level Preset ���� ���
	en_event_auth_lp_getnotrecv_req=5,	// gs->ds : ���� ���ӽÿ�, ���� �ȹ޾ư� ������ �ִ��� Ȯ�� ��û�Ѵ�.
	en_event_auth_lp_check_req=6,		// gs->ds : ���� �� �ÿ�, ds �˸���. DB �� ���� �ȹ޾ư� ������ ���.
	en_event_auth_lp_recv_req=7,		// gs->ds : Ŭ���̾�Ʈ���� '�ޱ�' ��ư ������, GS ���� DS ���� ���� �� �ִ��� Ȯ�� ��û�Ѵ�.
	en_event_auth_lp_cancel=8,			// gs->ds : ����, ���� �ֱ⿡ �����ϸ�, DS ��, �� ����/������ ���Ͽ�, �޾ư� ���� ����Ѵ�.

	// Invite Friend
	en_event_auth_friend_info_req=10,		// ģ�� �ʴ� : ��� �̺�Ʈ ���� ��û
	en_event_auth_friend_hyperrun_end=11,	// ģ�� �ʴ� : ��� ��� �ð��� �����
};


// mode 0 : check key and request
struct CMSG_EVENT_AUTH_REQ_WITH_KEY : public CMSG_EVENT_AUTH_REQ
{
	char				cNDAccount[en_max_lil+1];				// ���� ���̵�  17   <----��� �ٲ�����
	char				cCharacName[13] ;

	short				sEventID ;
	char				cKey[_EVENT_KEY_MAX_LENGTH_] ;

	CMSG_EVENT_AUTH_REQ_WITH_KEY() 
		: CMSG_EVENT_AUTH_REQ(sizeof(CMSG_EVENT_AUTH_REQ_WITH_KEY), en_event_auth_req_with_key)
	{
	}
};

// mode 1 : cancel key.,
struct CMSG_EVENT_CANCEL : public CMSG_EVENT_AUTH_REQ
{
	short				sEventID ;
	int					index;	

	CMSG_EVENT_CANCEL() 
		: CMSG_EVENT_AUTH_REQ(sizeof(CMSG_EVENT_CANCEL), en_event_auth_cancel)
	{
	}
};

// 2 : success
struct CMSG_EVENT_SUCCESS : public CMSG_EVENT_AUTH_REQ
{
	short				sEventID ;
	int					index;	

	CMSG_EVENT_SUCCESS() 
		: CMSG_EVENT_AUTH_REQ(sizeof(CMSG_EVENT_SUCCESS), en_event_auth_success)
	{
	}
};

#define _NDD_PRECHECK_EVENT_COUNT_				(24)

struct CMSG_EVENT_CHECK_REQ : public CMSG_EVENT_AUTH_REQ
{
	char				cNDAccount[en_max_lil+1];				// ���� ���̵�  17   <----��� �ٲ�����
	char				cCharacName[en_charac_name_length+1] ;

	short				curEventList[_NDD_PRECHECK_EVENT_COUNT_];		// �ִ� 8��������. 8�� �Ѿ���� ����.

	CMSG_EVENT_CHECK_REQ()
		: CMSG_EVENT_AUTH_REQ( sizeof(CMSG_EVENT_CHECK_REQ), en_event_auth_check_req )
	{
		ZeroMemory( cCharacName, sizeof(cCharacName) );
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
		ZeroMemory( curEventList, sizeof(curEventList) );
	}
};


// Level Preset ���� ���
// en_event_auth_lp_getnotrecv_req=5,	// gs->ds : ���� ���ӽÿ�, ���� �ȹ޾ư� ������ �ִ��� Ȯ�� ��û�Ѵ�.
struct CMSG_LP_GETNOTRECV_REQ : public CMSG_EVENT_AUTH_REQ
{
	char				cNDAccount[en_max_lil+1];				// ���� ���̵�  17   <----��� �ٲ�����

	// ������ ������ ��.
	CMSG_LP_GETNOTRECV_REQ()
		: CMSG_EVENT_AUTH_REQ( sizeof(CMSG_LP_GETNOTRECV_REQ), en_event_auth_lp_getnotrecv_req )
	{
	}

	void SetUserInfo( const char * acc )
	{
		strncpy( cNDAccount, acc, en_max_lil);
		cNDAccount[en_max_lil] = 0;
	}
};

// en_event_auth_lp_check_req=6,		// gs->ds : ���� �� �ÿ�, ds �˸���. DB �� ���� �ȹ޾ư� ������ ���.
struct CMSG_LP_CHECK_REQ : public CMSG_EVENT_AUTH_REQ
{
	short		sEventID;
	short		sLevel;

	char		cNDAccount[en_max_lil+1];				// ���� ���̵�  17   <----��� �ٲ�����
	char		cName[en_charac_name_length+1];

	CMSG_LP_CHECK_REQ()
		: CMSG_EVENT_AUTH_REQ( sizeof(CMSG_LP_CHECK_REQ), en_event_auth_lp_check_req )
	{
		sLevel = 0;
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
		ZeroMemory( cName, sizeof(cName) );
	}

	void SetUserInfo( const char * acc, const char * chr )
	{
		strncpy( cNDAccount, acc, en_max_lil);
		cNDAccount[en_max_lil] = 0;

		strncpy( cName, chr, en_charac_name_length );
		cName[en_charac_name_length] = 0;
	}
};


// en_event_auth_lp_recv_req=7,		// gs->ds : Ŭ���̾�Ʈ���� '�ޱ�' ��ư ������, GS ���� DS ���� ���� �� �ִ��� Ȯ�� ��û�Ѵ�.
struct CMSG_LP_RECV_REQ : public CMSG_EVENT_AUTH_REQ
{
	short		sEventID;
	short		sLevel;

	char		cNDAccount[en_max_lil+1];				// ���� ���̵�  17   <----��� �ٲ�����
	char		cName[en_charac_name_length+1];

	CMSG_LP_RECV_REQ()
		: CMSG_EVENT_AUTH_REQ( sizeof(CMSG_LP_RECV_REQ), en_event_auth_lp_recv_req )
	{
		sLevel = 0;
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
		ZeroMemory( cName, sizeof(cName) );
	}

	void SetUserInfo( const char * acc, const char * chr )
	{
		strncpy( cNDAccount, acc, en_max_lil);
		cNDAccount[en_max_lil] = 0;

		strncpy( cName, chr, en_charac_name_length );
		cName[en_charac_name_length] = 0;
	}
};



// en_event_auth_lp_cancel=8,			// gs->ds : ����, ���� �ֱ⿡ �����ϸ�, DS ��, �� ����/������ ���Ͽ�, �޾ư� ���� ����Ѵ�.
struct CMSG_LP_CANCEL : public CMSG_EVENT_AUTH_REQ
{
	short		sEventID;
	short		sLevel;			// �ش緹��.

	char		cNDAccount[en_max_lil+1];				// ���� ���̵�  17   <----��� �ٲ�����

	CMSG_LP_CANCEL()
		: CMSG_EVENT_AUTH_REQ( sizeof(CMSG_LP_CANCEL), en_event_auth_lp_cancel )
	{
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
	}

	void SetUserInfo( const char * acc )
	{
		strncpy( cNDAccount, acc, en_max_lil);
		cNDAccount[en_max_lil] = 0;
	}
};

// Invite Friend
// en_event_auth_friend_info_req=10,		// ģ�� �ʴ� : ��� �̺�Ʈ ���� ��û
struct CMSG_FRIEND_INFO_REQ : public CMSG_EVENT_AUTH_REQ 
{
	short		sEventID;

	char		cNDAccount[en_max_lil+1];

	CMSG_FRIEND_INFO_REQ()
		: CMSG_EVENT_AUTH_REQ( sizeof(CMSG_FRIEND_INFO_REQ), en_event_auth_friend_info_req )
	{
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
	}

	void SetUserInfo( const char * acc )
	{
		if( acc )
		{
			strncpy( cNDAccount, acc, en_max_lil );
			cNDAccount[en_max_lil] = 0;
		}
	}
};

// en_event_auth_friend_hyperrun_end=11,	// ģ�� �ʴ� : ��� ��� �ð��� �����.
struct CMSG_FRIEND_HYPERRUN_END : public CMSG_EVENT_AUTH_REQ
{
	int			index;

	CMSG_FRIEND_HYPERRUN_END()
		: CMSG_EVENT_AUTH_REQ(sizeof(CMSG_FRIEND_HYPERRUN_END), en_event_auth_friend_hyperrun_end )
	{
	}
};


enum DB_RESULT_EVENT_CODE
{
	_db_result_not_found = 0,		// No key
	_db_result_used = 1,			// used
	_db_result_wait_a_moment = 2,	// wait
	_db_result_success = 3,			// success
	_db_result_not_checked = 4,		// not checked, �̺�Ʈ�� ������ ���� ����.
};


// �̺�Ʈ ��� �� ���� �ڵ�.
enum EN_EVENT_ERRNO
{
	en_event_success = 0,
	en_event_errno_blank,			// ���Դϴ�. ������...
	en_event_errno_no_key,			// �߸��� Ű��.
	en_event_errno_wait,			// ����Ŀ� �ٽ� �õ����ּ���.
	en_event_errno_event_end,		// �̹� ���� �̺�Ʈ �Դϴ�.
	en_event_errno_no_event,		// �������� �̺�Ʈ�� �ƴմϴ�.
	en_event_errno_used,			// �̹� ����� Ű�� �Դϴ�.
	en_event_errno_no_item_slot,	// �κ��丮�� ��á��
	en_event_errno_no_charac,		// ������ �ð��.	: �α׸� ���ؼ��� ���δ�.
	en_event_errno_fail_insert_item,// ������ ����������, �������� ���� ���ߴ�.
	en_event_errno_not_premium,		// �����̾� ������ �ƴϴ�.

	en_event_is_going_on=50,		// �̺�Ʈ�� �������̴�.( �ű����� �̺�Ʈ�� ����, ������ �� �̸� �˷��ش�. )

	en_event_errno_system=100,		// ��Ŷ ���̵��� �ý��� ����.
	en_event_errno_over_insert,		// ���� �Է�Ƚ�� �ʰ�
} ;



struct CMSG_EVENT_RESULT		// �̺�Ʈ ���� �Ǵ� �����ÿ� ������. 
{
	u_short				usLength;
	u_char				cMessage ;				// CMSG_NO_EVENT_AUTH_REQ

	char				cCharacName[13] ;		// 

	u_char				ucEventResultMode;		// 0 - One, 1 - PreNotice

	CMSG_EVENT_RESULT( u_short size, u_char mode )
		: usLength(size), cMessage(CMSG_NO_EVENT_AUTH_REQ), ucEventResultMode(mode)
	{
		ZeroMemory( cCharacName, sizeof(cCharacName) );
	}
};


enum EN_EVENT_RESULT_MODE
{
	en_dsevent_result_one_event=0,
	en_dsevent_result_pre_notice=1,

	// Level Present
	en_dsevent_result_lp_getnotrecv_ans=5,	// ds->gs : ����
	en_dsevent_result_lp_check_ans=6,			// ds->gs : ����.
	en_dsevent_result_lp_recv_ans=7,			// ds->gs : ����

	// Invite Friend : Hyperrun 
	en_dsevent_result_friend_info=10,			// ds->gs : 
};


// mode 0
struct CMSG_ONE_EVENT_RESULT : public CMSG_EVENT_RESULT		// �̺�Ʈ ���� �Ǵ� �����ÿ� ������. 
{
	short				sEventID;

	char				cItemType ;				// ������ Ÿ��	: package type = -1;
	short				sItemID ;				// ������ ���̵�
	u_char				ucItemCount ;			// �⺻ 1

	u_char				ucResult ;				//
	int					index;					// 

	CMSG_ONE_EVENT_RESULT()
		: CMSG_EVENT_RESULT( sizeof(CMSG_ONE_EVENT_RESULT), en_dsevent_result_one_event )
	{
	}
};


// mode 1
struct CMSG_PRE_NOTICE_EVENT_RESULT : public CMSG_EVENT_RESULT		// �̺�Ʈ ���� �Ǵ� �����ÿ� ������. 
{
	char				cAccount[en_max_lil+1];
	struct 
	{
		short			sEventID;
		u_char			ucResult;
	} EventResult[_NDD_PRECHECK_EVENT_COUNT_];

	CMSG_PRE_NOTICE_EVENT_RESULT()
		: CMSG_EVENT_RESULT(sizeof(CMSG_PRE_NOTICE_EVENT_RESULT), en_dsevent_result_pre_notice )
	{
		ZeroMemory( cAccount, sizeof(cAccount) );
		ZeroMemory( EventResult, sizeof(EventResult) );
	}
};


// Level Present
// en_event_result_lp_getnotrecv_ans=5,	// ds->gs : ����
struct CMSG_LP_GETNOTRECV_ANS : public CMSG_EVENT_RESULT
{
	short		sEventID;
	short		sLevel;			// -1 : ����, >=0 �ִ�.

	char		cNDAccount[en_max_lil+1];

	CMSG_LP_GETNOTRECV_ANS()
		: CMSG_EVENT_RESULT( sizeof(CMSG_LP_GETNOTRECV_ANS), en_dsevent_result_lp_getnotrecv_ans )
	{
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
	}

	void SetAccount( const char * acc )
	{
		strncpy( cNDAccount, acc, en_max_lil );
		cNDAccount[en_max_lil] = 0;
	}
};

// en_event_result_lp_check_ans=6,			// ds->gs : ����.
struct CMSG_LP_CHECK_ANS : public CMSG_EVENT_RESULT
{
	short		sEventID;
	short		sLevel;			// -1 : fail. else OK

	char		cNDAccount[en_max_lil+1];

	CMSG_LP_CHECK_ANS()
		: CMSG_EVENT_RESULT( sizeof(CMSG_LP_CHECK_ANS), en_dsevent_result_lp_check_ans )
	{
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
	}

	void SetAccount( const char * acc )
	{
		strncpy( cNDAccount, acc, en_max_lil );
		cNDAccount[en_max_lil] = 0;
	}
};


// en_event_result_lp_recv_ans=7,			// ds->gs : ����
struct CMSG_LP_RECV_ANS : public CMSG_EVENT_RESULT
{
	short		sEventID;
	short		sLevel;			// �Է� ����

	u_char		ucResult;		// ��� 0 - fail, 1 - success.

	char		cNDAccount[en_max_lil+1];

	CMSG_LP_RECV_ANS()
		: CMSG_EVENT_RESULT( sizeof(CMSG_LP_RECV_ANS), en_dsevent_result_lp_recv_ans )
	{
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
	}

	void SetAccount( const char * acc )
	{
		strncpy( cNDAccount, acc, en_max_lil );
		cNDAccount[en_max_lil] = 0;
	}
};

// Invite Friend : Hyperrun 
//	en_event_result_friend_info=10,			// ds->gs : 
struct CMSG_FRIEND_INFO : public CMSG_EVENT_RESULT
{
	u_char			ucResult;		// 0 : no, 1 : ok.

	char			cNDAccount[en_max_lil+1];

	short			sEventID;

	int				iDB_Index;
	int				end_time;

	char			cFromWho[en_max_lil+1];

	CMSG_FRIEND_INFO()
		: CMSG_EVENT_RESULT( sizeof(CMSG_FRIEND_INFO), en_dsevent_result_friend_info )
	{
		ucResult = 0;
		end_time = 0;
		ZeroMemory( cNDAccount, sizeof(cNDAccount) );
		ZeroMemory( cFromWho, sizeof(cFromWho) );
	};

	void SetAccount( char * acc )
	{
		strncpy( cNDAccount, acc, en_max_lil );
		cNDAccount[en_max_lil] = 0;
	}
};


#ifdef _PD_GAME_EVENT_
//////////////////////////////////////////////////////////////////////////
// ���� �� : 
//////////////////////////////////////////////////////////////////////////
//#define CMSG_NO_FREE_MATCH						(91)	
struct CMSG_FREE_MATCH			// CMSG_FM_	���� ����.
{
	u_short				usLength;				// 
	u_char				cMessage;				//

	u_char				ucFMWork;				// FREE_MATCH_WORK �� �״�� ����.

	CMSG_FREE_MATCH( u_short length, u_char mode )
		: usLength( length ), cMessage( CMSG_NO_FREE_MATCH ), ucFMWork( mode )
	{
	}
};


enum FREE_MATCH_DS_PACKET
{
	en_dsfm_ready=0,				// ready �̺�Ʈ ó��.
	en_dsfm_start,					// start �̺�Ʈ ó��.
	en_dsfm_end,					// end �̺�Ʈ ó��
	en_dsfm_notice,					// ���� ����
	en_dsfm_notice_winner,			// ���� : ����� ������ ����
	en_dsfm_notice_nowinner,		// ���� : ���º� ����
	en_dsfm_to_winner,				// ���ڿ��Ը� ���� �޽���.

	en_dsfm_req_result,				// GS �� ����� ��û�Ѵ�.
	en_dsfm_result_info,			// req_result �� �������� GS ���� ����� ������ ��´�.

	en_dsfm_send_result,			// ����� Ŭ���̾�Ʈ�� �������� �Ѵ�.

	en_dsfm_reward_nick,			// ��ȣ ���� ����,
	en_dsfm_reward_item,			// ������ ���� ����
	en_dsfm_buff_all,				// �� ������ ��� ����.
	en_dsfm_clean,					// ���� ����.

	en_dsfm_req_cur_matchinfo=50,	// ���� ��ġ ������ ��û�Ѵ�. : �ʱ⿡ �ѹ� ��û�Ѵ�.
	en_dsfm_cur_matchinfo,			// ���� ��ġ ������ �˷��ش�.
	en_dsfm_cur_nick_owner,			// �񹫴�ȸ ��ȣ�� ���� ������ ������ �˷��ش�.
};


//en_dsfm_ready=0,				// ready �̺�Ʈ ó��.
struct CMSG_FM_READY : public CMSG_FREE_MATCH
{
	_sFreeMatchEvent		event_info;		

	time_t					event_t_time;

	CMSG_FM_READY()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_READY), en_dsfm_ready )
	{
	}
};


//en_dsfm_start,				// start �̺�Ʈ ó��.
struct CMSG_FM_START : public CMSG_FREE_MATCH
{
	// ���� �ʿ� ����.
	time_t					event_t_time;

	CMSG_FM_START()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_START), en_dsfm_start )
	{
	}
};


//en_dsfm_end,					// end �̺�Ʈ ó��
struct CMSG_FM_END : public CMSG_FREE_MATCH
{
	// ���� �ʿ� ����.
	time_t					event_t_time;

	CMSG_FM_END()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_END), en_dsfm_end )
	{
	}
};


//en_dsfm_notice,				// ���� ����
struct CMSG_FM_NOTICE : public CMSG_FREE_MATCH
{
	// ���� ����
	u_short					text_id;
	u_short					text_arg1;

	CMSG_FM_NOTICE()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_NOTICE), en_dsfm_notice )
	{
	}
};


//en_dsfm_notice_winner,		// ���� : ����� ������ ����
struct CMSG_FM_NOTICE_WINNER : public CMSG_FREE_MATCH
{
	// 
	u_short					text_id;

	char					cWinnerName[en_charac_name_length+1];

	CMSG_FM_NOTICE_WINNER()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_NOTICE_WINNER), en_dsfm_notice_winner )
	{
	}
};


//en_dsfm_notice_nowinner,		// ���� : ���º� ����
struct CMSG_FM_NOTICE_NOWINNER : public CMSG_FREE_MATCH
{
	u_short					text_id;

	CMSG_FM_NOTICE_NOWINNER()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_NOTICE_NOWINNER), en_dsfm_notice_nowinner )
	{
	}
};


//en_dsfm_to_winner,			// ���ڿ��Ը�.
struct CMSG_FM_TO_WINNER : public CMSG_FREE_MATCH
{
	int						cuid;
	u_short					text_id;				// DS -> GS

	CMSG_FM_TO_WINNER()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_TO_WINNER), en_dsfm_to_winner )
	{
	}
};


//	en_dsfm_req_result,
struct CMSG_FM_REQ_RESULT : public CMSG_FREE_MATCH
{
	// GS �� ��ŷ�� ����϶�� ��� ������ ��Ŷ�̴�.

	CMSG_FM_REQ_RESULT()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_REQ_RESULT), en_dsfm_req_result )
	{
	}
};

//en_dsfm_result_info
struct CMSG_FM_RESULT_INFO : public CMSG_FREE_MATCH
{
	u_char					ucResult;

	_sFM_Record				winnerRecord;

	CMSG_FM_RESULT_INFO()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_RESULT_INFO), en_dsfm_result_info )
	{
	}
};

//en_dsfm_send_result,			// ����� �����ش�.
struct CMSG_FM_SEND_RESULT : public CMSG_FREE_MATCH
{
	// ����� Client �� ������� ��� ������ ��Ŷ�̴�.
	u_char					ucResult;

	/*
	enum EN_FM_RESULT
	{
	en_fm_result_one_winner=0,	// One winner
	en_fm_result_more_winner,	// two or more winners.
	en_fm_result_none,		// no winner
	};
	*/

	_sFM_Record				winnerRecord;

	CMSG_FM_SEND_RESULT()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_SEND_RESULT), en_dsfm_send_result )
	{
	}
};


//en_dsfm_reward_nick,			// ��ȣ ���� ����,
struct CMSG_FM_REWARD_NICK : public CMSG_FREE_MATCH
{
	u_short					usRank;
	u_short					usNickID;

	int						iWinnerCUID;

	CMSG_FM_REWARD_NICK()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_REWARD_NICK), en_dsfm_reward_nick ), usNickID(0)
	{
	}
};


//en_dsfm_reward_item,
struct CMSG_FM_REWARD_ITEM : public CMSG_FREE_MATCH
{
	struct {
		u_char					ucToWhom;		// en_ge_reward_item, 
												// en_ge_reward_item_to_male,en_ge_reward_item_to_female
		char					cItemType;
		short					sItemID;
		u_char					ucCount;
	} Item[8] ;
	

	CMSG_FM_REWARD_ITEM()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_REWARD_ITEM), en_dsfm_reward_item )
	{
		ZeroMemory( Item, sizeof(Item) );
	}
};


//en_dsfm_buff_all,			// �� ������ ��� ����.
struct CMSG_FM_BUFF_ALL : public CMSG_FREE_MATCH
{
	short					sGameServerIndex;			// 17������ ������ ���̴�.

	short					sBuffIndex;					// 
	u_short					usBuffTime;					// 

	u_char					ucBuffStep;					// �Ƹ� 1�� ���̴�.

	CMSG_FM_BUFF_ALL()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_BUFF_ALL), en_dsfm_buff_all )
	{
	}
};


//en_dsfm_clean,				// ���� ����.
struct CMSG_FM_CLEAN : public CMSG_FREE_MATCH
{
	// ������.
	_sFreeMatchEvent		event_info;		

	CMSG_FM_CLEAN()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_CLEAN), en_dsfm_clean )
	{
	}
};


//en_dsfm_req_cur_matchinfo=50,	// ���� ��ġ ������ ��û�Ѵ�. : �ʱ⿡ �ѹ� ��û�Ѵ�.
struct CMSG_FM_REQ_CUR_MATCHINFO : public CMSG_FREE_MATCH
{

	CMSG_FM_REQ_CUR_MATCHINFO()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_REQ_CUR_MATCHINFO), en_dsfm_req_cur_matchinfo )
	{
	}
};


//en_dsfm_cur_matchinfo,			// ���� ��ġ ������ �˷��ش�.
struct CMSG_FM_CUR_MATCHINFO : public CMSG_FREE_MATCH
{
	u_char					event_state;		// EN_FM_STEP

	_sFreeMatchEvent		event_info;			// ���� �̺�Ʈ�� ����.

	time_t					event_time;

	CMSG_FM_CUR_MATCHINFO()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_CUR_MATCHINFO), en_dsfm_cur_matchinfo )
	{
		ZeroMemory( &event_info, sizeof(_sFreeMatchEvent) );
	}
};


#define FM_RANK_MAX_COUNT			(48)
//en_dsfm_cur_nick_owner
struct CMSG_FM_CUR_NICK_OWNER : public CMSG_FREE_MATCH
{
	u_short						rank_count;
	_sFM_LastRewardNickInfo		nick[FM_RANK_MAX_COUNT];

	CMSG_FM_CUR_NICK_OWNER()
		: CMSG_FREE_MATCH( sizeof(CMSG_FM_CUR_NICK_OWNER), en_dsfm_cur_nick_owner ), rank_count(0)
	{
	}
};

#endif	// _PD_GAME_EVENT_


// #define CMSG_NO_GM_EVENT						(92)		// GM �̺�Ʈ ��Ŷ.
// 20080803
struct CMSG_GM_EVENT 
{
	u_short				usLength;
	u_char				cMessage;

	u_char				ucMode;
};

enum 
{
	en_cgm_event_instant_buff=0,					// �ν�źƮ ����
};

struct CMSG_GM_EVENT_INSTANT_BUFF : public CMSG_GM_EVENT
{
	u_char				ucTarget;		// 0 - all, 1 - white, 2 - black

	short				buffIndex;		// 
	char				buffStep;		// 
};


//////////////////////////////////////////////////////////////////////////
// ��� ����
//////////////////////////////////////////////////////////////////////////
//#define CMSG_NO_PKEVENT							98		// ��� ���� ���� ��Ŷ.
struct CMSG_PKEVENT
{
	u_short				usLength;
	u_char				cMessage;

	u_char				event;
	// 0 - �������� ���ڸ� �˸��� ��Ŷ.
	// 

	u_char				value1;
	u_char				value2;

	u_short				value3;

	u_int				value4;
};

/* =====================================================================================
�α��ΰ� ���� �̵� ���� : 0 - 7 ����.	
#define CMSG_NO_GSCONNECT_GS_DS					2
#define CMSG_NO_ENTERSERVER_CHECK_REQ_GS_DS		3
#define CMSG_NO_ENTERSERVER_CHECK_RESULT_DS_GS	4

���� ����..
#define CMSG_NO_GAME_SIGNAL						5
===================================================================================== */
// �α��� ���� ��û
//#define CMSG_NO_LOGIN_LS_DS						0		// LS->DS LOGIN REQUEST MESSAGE

// GS�� DS�� Connect ���Ŀ� ������, ������ ������ �˷��ش�.
//#define CMSG_NO_GSCONNECT_GS_DS					2
struct CMSG_GSCONNECT_GS_DS					// # GS->DS GS�� ���ʷ� DS �� �����Ѵ�.
{
	u_short				usLength;
	u_char				cMessage;				// CMSG_NO_GSCONNECT_GS_DS(4)
	char				cServerNum;				// 
};

//#define CMSG_NO_GSCONNECT_DS_GS					2
struct CMSG_GSCONNECT_DS_GS					// # GS->DS GS�� ���ʷ� DS �� �����Ѵ�.
{
	u_short				usLength;
	u_char				cMessage;				// CMSG_NO_GSCONNECT_GS_DS(4)
	char				cServerList[GS_MAX] ; // GS_MAX == 35
	u_int				uiServerStat;		//32bit ���� ���� �÷���
	//1: ��������
};


// ���� ó�� ���� ���� ��û�� ���� �̵� ����
// �α��� ������ ����� ������ ó������ ���� ������ �������� ��
// ���� �������� DS �� ������ ������ ���� ������ ��Ŷ.
//#define CMSG_NO_ENTERSERVER_CHECK_REQ_GS_DS		3
struct CMSG_ENTERSERVER_CHECK_REQ_GS_DS
{
	u_short				usLength;
	u_char				cMessage;				// CMSG_NO_ENTERSERVER_CHECK_REQ_GS_DS 5
	char				cAccount[en_max_lil+1];			// 
	u_short				usUid;					// 
	u_int				iIP;					// Ȥ�� �������𸣴� ��ŷ�� ����ؼ� ������ �˻���� �����Ѵ�.
	int					iTime;					//
	char				cServerNum;
};


// ���� �����̳� ���� �̵��� ����(�˻�)�� ����� ������ ���, ĳ���� ����Ʈ ��Ŷ���� ��ü�ϸ�
// �������� ���� ���� ��Ŷ���� �Ѵ�.
//#define CMSG_NO_ENTERSERVER_CHECK_RESULT_DS_GS	4
struct CMSG_ENTERSERVER_CHECK_RESULT_DS_GS
{
	u_short				usLength;
	u_char				cMessage;				// CMSG_NO_ENTERSERVER_CHECK_RESULT_DS_GS 6
	char				cAccount[en_max_lil+1];			// 
	u_short				usUid;					// Account �����δ� �������� ����.
};




/* =====================================================================================
�κ񿡼� - ĳ���� ����/����/���� ���� 8 - 15
#define CMSG_NO_CHARAC_SELECT_GS_DS				8		// ĳ���� â���� ĳ���͸� �����ߴ�.
#define CMSG_NO_CHARAC_CREATE_GS_DS				9		// ĳ���� ����
#define CMSG_NO_CHARAC_DELETE_GS_DS				10		// ĳ���� ����
#define CMSG_NO_USER_CHARAC_LIST_DS_GS			11		// ĳ���� ����Ʈ
#define CMSG_NO_CHARAC_RESULT_DS_GS				12		// ���� 3���� ��Ŷ�� ���� �������� ���δ�.
#define CMSG_NO_CHARAC_LIST_REQ_GS_DS			13		// ĳ���� ����Ʈ�� ���û
===================================================================================== */														// �ڼ��� �� ��Ŷ ���Ǻο� �ִ�.

// ĳ���� ���� �޽���.  ĳ���� ���� Ȥ�� CHARAC_RESULT
struct CMSG_CHARAC_SELECT_GS_DS				// # GS->DS ĳ���� â���� ĳ���͸� �����ߴ�. #
{
	u_short				usLength;
	u_char				cMessage ;				//	CMSG_NO_CHARAC_SELECT_GS_DS(8)

	u_short				usUid ;					// uid
	char				cAccount[en_max_lil+1];			// 
	char				cChrName[13];			//	ĳ���� �̸�
} ;


// ĳ���� ���� �޽���. CHARAC_RESULT
struct CMSG_CHARAC_CREATE_GS_DS				// #  GS->DS ĳ���� ���� #
{
	u_short				usLength;
	u_char				cMessage ;				// CMSG_NO_CHARAC_CREATE_GS_DS(9)
	char				cAccount[en_max_lil+1];			// 

	char				cName[13];				// ĳ������ �̸�

	short				sConstitution;			// �ǰ� <= ����	: 5�⸦ ��ġ�� �޴´�. ( 8~18 �����̸�, �հ�� �ݵ�� 50�̾�� �Ѵ�. 
	short				sZen;					// ���� : �￩���� ���ġ�� ����. 
	short				sIntelligence;			// ���� <= ����
	short				sDexterity;				// ��ø <= �ɱ�
	short				sStr;					// �ٷ�

	char 				cSex;					// ���� 0 : ����, 1 : ����	

	char				cFace;					// ���� : max 16�� ������ �׳� 
	char				cHair;					// �Ӹ� ��Ÿ�� : max 16�� 

	short				sAmor;					// ���� : 2�� �ۿ� ���µ� �Ѥ�
	short				sPants;				// ���� : 2�� �ۿ� ����. 

	short				sAdea ;
	short				sShoes ;


	char				cInitZone;				// ó�� ��ġ�� ����
	float				fInitPosX;				// �ʱ� ��ġ
	float				fInitPosZ;				//
} ;


// ĳ���� ���� ��û �޽��� : ������ CHARAC_RESULT
struct CMSG_CHARAC_DELETE_GS_DS				// # GS->DS ĳ���� ���� #
{
	u_short				usLength;
	u_char				cMessage ;				// CMSG_NO_CHARAC_DELETE_GS_DS(10)
	char				cChrName[13];			//	ĳ���� �̸�

	char				cAccount[en_max_lil+1];			// 
	char				cPassword[13];			//	�н�����
} ;


// ĳ���� ����Ʈ
struct CMSG_USER_CHARAC_LIST_DS_GS
{
	u_short				usLength;			
	u_char				cMessage;

	u_short				usUid;					// 
	char				cSelChrName[13];		// ������ ĳ���� ����

	_CharacList			List[3];				//

	_ACCOUNT_ITEM_USE_STATUS aius ;				// 

#if defined(_PD_PCROOM_INFO_) || defined(_PD_VINA_PCROOM_INFO_)
	u_char				ucWhereConnect;		// ��𿡼� �����ߴ°�.
	u_char				ucUserGrade;		// ������ ���.
#endif
#ifdef _VINA_SHUTDOWN_20061023_
	short				sFatigueWeight ;	//��Ʈ�� shutdown
#ifdef _PD_SDS_GOSTOP_
	u_char				ucFatigueMode;
#endif
#endif
};

#define ErrorMessageNum_CanNotSelect		20
#define ErrorMessageNum_ServerStateErr		21
// �κ񿡼��� ĳ���� ������ ����.
struct CMSG_CHARAC_RESULT_DS_GS				// # DS->GS ĳ���� ����/����/������ ���� ��� #
{
	u_short				usLength;
	u_char				cMessage ;				// CMSG_NO_CHARAC_RESULT_DS_GS(12)		
	char				cChrName[13];			// �����̸� ������ �����̰� ����,���� �̸� ĳ���� ���̵�� ����.

	u_short				usUid;					// 
	int					iCharacUid ;			// ĳ���� ����ũ ���̵�.

	char				cPrevCommand;			// � �޼����� ����ΰ�. 
	char				cResult ;				// 0�̸� ����, �ƴϸ� ���� ����.
	// cPrevCommand : ĳ���� ������ ���� �����̸�, �� ���� MSG_NO_CHARAC_CREATE �̰�, 
	// �����̶��, ucResult �� 0�� �ɰ��̰�, �ƴϸ�, ���� ������ �ɰ��̴�.
	// MSG_CHARAC_CREATE �� ���� �������, �� 2~5 �� ������ Game Server���� �����Ѵ�. 
	//   1 : �̹� ������� �̸�.  2: ����� �� ���� �̸�. 3: ���ġ ����  4: �⺻ ������ ���� 5: ���̻� ����� ����.
	// MSG_CHARAC_DELETE �� ���� ��������
	//   1 : ��ȣ�� Ʋ���� ���, 2: �ٸ� ��� ����
	// MSG_CHARAC_SELECT �� ���� �������� 
	//   20 : ĳ���͸� ������ �� ����.
	// MSG_CHARAC_SELECT �� ���� �������� 
	//   21 : �ش� ������ ���������Դϴ�.
} ;


// ĳ���� ����Ʈ ��û
// CMSG_NO_CHARAC_LIST_REQ_GS_DS
struct CMSG_CHARAC_LIST_REQ_GS_DS
{
	u_short				usLength ;
	u_char				cMessage ;

	char				cAccount[en_max_lil+1];			// 
} ;


/* =====================================================================================
�α׾ƿ�, �����̵� ��û.  16 - 23
#define CMSG_NO_LOGOUT_GS_DS					16		// �ܼ� �α׾ƿ�
#define CMSG_NO_USER_LOGOUT_DS_GS				17		// ���� �α׾ƿ�
#define CMSG_NO_REQ_SV_MOVE_GS_DS				18		// ������ �̵� ��û
===================================================================================== */
// �α� �ƿ� ��Ŷ 
//#define CMSG_NO_LOGOUT_GS_DS					16
struct CMSG_LOGOUT_GS_DS
{
	u_short				usLength;
	u_char				cMessage;

	char				cAccount[en_max_lil+1];			// 
	u_short				usUid;					// �׳� ������ ������. ������ �񱳴� �� ��.
};

// ���� �̵� ��û 
// �� ��Ŷ�� ��û�� �ƴ϶�, '�뺸' �� �ٲ����. ������ �ʿ� ������, ������.
// SelectCharac �� �κ񿡼� �ٷ� �����̵��� �� ��� �ʿ���. 
// �� ���, ��ġ ������ ������Ʈ �ϸ� �ʵ�. ( bUpdatePos == false )
// �ʵ忡���� �̵��� ���, bUpdatePos == true �� �ǰ�, ��ġ ������ ������Ʈ��Ų��. 
struct CMSG_REQ_SV_MOVE_GS_DS
{
	u_short				usLength;
	u_char				cMessage;

	char				cAccount[en_max_lil+1];			// 
	char				cSelectCharac[13];		// ���õ� ĳ����
	int					iTime;					// �����̵� �ð� ������ �˻��ϱ� ����..
	bool				bUpdatePos;				// �Ʒ��� �����ǰ��� ������Ʈ �� ���ΰ�.
	char				cZone;					// � ������ ���°�. 0�� ���� ���� �� ����.
	float				fGotoPosX;				// ��ǥ ���������� ó�� ��ġ
	float				fGotoPosZ;				//
};

//#define CMSG_NO_SV_MOVE_RESULT_DS_GS			19		// ���
struct CMSG_SV_MOVE_RESULT_DS_GS
{
	u_short				usLength;
	u_char				cMessage;	
	u_short				usUid;
}; 


//#define CMSG_NO_ACCOUNT_INFO					22		// ���� ���õ� ���� ��Ŷ : 0. �÷��� �ð�.
enum 
{
	en_cai_add_play_time=0,				// Add Play Time.
};
struct CMSG_ACCOUNT_INFO
{
	u_short				usLength;
	u_char				cMessage;

	u_char				ucMode;			// 

	char				cNDAccount[en_max_lil+1];

	CMSG_ACCOUNT_INFO( u_short size, u_char mode )
		: usLength(size), cMessage(CMSG_NO_ACCOUNT_INFO), ucMode( mode )
	{
	}
};

struct CMSG_AI_ADD_PLAY_TIME : public CMSG_ACCOUNT_INFO
{
	int					iPlayTime;		// added time : unit - second

	CMSG_AI_ADD_PLAY_TIME( char * acc, int playtime )
		: CMSG_ACCOUNT_INFO( sizeof(CMSG_AI_ADD_PLAY_TIME), en_cai_add_play_time )
	{
		strncpy( cNDAccount, acc, en_max_lil+1 );
		cNDAccount[en_max_lil] = 0;
		iPlayTime = playtime;
	}
};

/* =====================================================================================
ĳ���� ���� ���� �Ǵ� �κ�� ���������  24 - 39
#define CMSG_NO_CHARAC_ALL_INFO_DS_GS			24		// DS->GS ALL CHARAC INFO REQUEST MESSAGE
#define CMSG_NO_DEPOT_REQ_GS_DS					25		// GS->DS Depot data request..
#define CMSG_NO_DEPOT_RES_GS_DS					26		// DS->GS Depot data result..

#define CMSG_NO_CHARAC_RETURN_LOBBY_GS_DS		39		// ������ �κ�� ����.
===================================================================================== */
// 
/*
#define _MAX_GAVE_ITEM_COUNT	16
struct _GaveItemList 
{
	size_t			uiCount ;
	_GaveItem		itemlist[_MAX_GAVE_ITEM_COUNT] ;
};
*/
struct CMSG_CHARAC_ALL_INFO_DS_GS
{
	u_short				usLength;
	u_char				cMessage;

	u_short				usUid;

	_CHARAC				charac;					// ĳ���� �̸��� �� �ȿ� ���Ե�.
	_USER_DEPOT			storage;				// â�� ����..

	_CHARAC_ETC_QUICKSLOT	quickslot;			//

#ifdef _PD_CHARAC_LTS_
	_CHARAC_LTS			characLts;
#endif

	u_int				uiCheckFlag_Login : 1;		// Is Check current event.
	u_int				uiCheckFlag : 31;				// Reserve

#ifdef _PD_EVENT_INVITE_FRIEND_
	int					iIF_DBIndex;
	int					iIF_EndTime;
#endif // _PD_EVENT_INVITE_FRIEND_
};

//#define CMSG_NO_DEPOT_REQ_GS_DS					25		// GS->DS Depot data request..
struct CMSG_DEPOT_REQ_GS_DS
{
	u_short				usLength ;
	u_char				cMessage ;

	char				cAccount[en_max_lil+1];			// 
};


//#define CMSG_NO_DEPOT_RES_GS_DS					26		// DS->GS Depot data result..
struct CMSG_DEPOT_RES_DS_GS
{
	u_short				usLength ;
	u_char				cMessage ;

	u_short				usTempUniqueID ;
	_USER_DEPOT			USER_DEPOT;
};



// �ʵ忡�� �κ��
// �� �޽����� �ܼ��� ���� �޽�����. ������ ����.
struct CMSG_CHARAC_RETURN_LOBBY_GS_DS
{
	u_short				usLength;
	u_char				cMessage;

	char				cCName[en_charac_name_length+1];			// 
	char				cNDAccount[en_max_lil+1];
};


//#define CMSG_NO_RANKING_INFO_REQ				64			
struct CMSG_RANKING_INFO_REQ {
	u_short				usLength ;
	u_char				cMessage ;	

	short				sType ;
};


struct _Rank {
	short			sRanking ;			// ��ŷ ����
	char			cCharacName[13] ;	// ĳ���̸�	( �Ǵ� ������ �̸� )
	short			sInnerLevel ;		// level
	u_int			sExp ;				// ����ġ ( �Ǵ� �������� ����ġ )
	u_int			sContribution ;		// �⿩�� ( �Ǵ� �������� ���� �⿩�� )
	char			cParty ;			// ���� ( �Ǵ� �������� �Ҽ� ���� )
	char			cClass ;			// ����

	_CHARAC_MATCH	m_MatchPoint ;	// ������( �񹫷�ŷ�� ��� )

	_Rank & _Rank::operator=( const _Rank& rhs ) 
	{
		try{
			if( this == &rhs ) { return *this ; }
			memcpy( this, &rhs, sizeof(_Rank) ) ;
		}
		catch(...){
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}
		return *this ;
	}	

	
};


//#define CMSG_NO_RANKING_INFO_RES				65			// ��ŷ ������ ����
struct CMSG_RANKING_INFO_RES {			// ��������...
	u_short				usLength ;		// Header + 2 + (sizeof(_Rank) * sRankCount)
	u_char				cMessage ;

	short				sType ;

	short				sRankCount ;
	_Rank				rank[100] ;	// ��������..
};

//#define CMSG_NO_RANKING_UPDATE					66
struct CMSG_RANKING_UPDATE {
	u_short				usLength ;		// Header + 2 + (sizeof(_Rank) * sRankCount)
	u_char				cMessage ;

	short				sType ;

	short				sRankCount ;
	_Rank				rank[100] ;	// ��������..
};




//����Ʈ ��ŷ ������
struct QUEST_RANK_RECORD
{
	unsigned char	m_ucClan;
	char			m_acOrgName[en_or_name_length+1];		//����: ��,			����: ��,		��: ������ �̸�
	char			m_acName[13];		//����: �̸�,		����: ����̸�,	��: �����̸�
	unsigned short	m_usLevelOrCount;	//����: ����ܰ�,	����: �ο���,	��: ���ο�
	unsigned short	m_usQuestData;		//KillCount, Time, ItemCount
};

#define MAX_QUEST_RANK_RECORD	(10)

struct _QUEST_RANK_DATA
{
	QUEST_RANK_RECORD	m_aRankRecord[MAX_QUEST_RANK_RECORD];
};


//#define CMSG_NO_QUESTRANK_DS_REQ				67			// ����Ʈ��ŷ ������ �䱸
struct MSG_QUESTRANK_DS_REQ
{
	u_short				usLength;
	u_char				cMessage;

	u_short				usQuestID;
};

//#define CMSG_NO_QUESTRANK_DS_RES				CMSG_NO_QUESTRANK_DS_REQ			// ����Ʈ��ŷ ������ ����
struct MSG_QUESTRANK_DS_RES
{
	u_short				usLength;
	u_char				cMessage;

	u_short				usQuestID;
	_QUEST_RANK_DATA	Data;
};

//#define CMSG_NO_QUESTRANK_DS_UPDATE				69			// ����Ʈ��ŷ ������ ������Ʈ
struct MSG_QUESTRANK_DS_UPDATE
{
	u_short				usLength;
	u_char				cMessage;

	u_short				usQuestID;
	_QUEST_RANK_DATA	Data;
};


/*==========================================================================================]
[				DS <-> GS																	]
[===========================================================================================]
[	���� :	UDP �޽��������͵��� TCP ��Ŷ ������ �����Ͽ��� ( 2005-03-8 )					]
[											jkHong Add...	-								]
[==========================================================================================*/
//#define CMSG_NO_GSINFO_REQ_DS_GS				100	// �� �ֱ����� ��û ��Ŷ�̴�..
struct CMSG_GSINFO_REQ_DS_GS					// # DS->GS GS�� ���� ������ ��û�Ѵ�.
{
	u_short				usLength ;	
	u_char				cMessage;				//�޼��� ��ȣ			
};


//#define CMSG_NO_GSINFO_RES_GS_DS				101	//�ֱ��� ����, �Ǵ� �� �ֱ��� ��û�� ���� ����
struct CMSG_GSINFO_RES_GS_DS					// # GS->DS GS�� ���� ������ DS �� �˸���(UPDATE)
{
	u_short				usLength ;
	u_char				cMessage;				// MSG_NO_GSINFO_RES_GS_DS(5)
	
	char				cRealSN;
	_SERVER_SYSTEM_INFO	_ServerSystemInfo;
	_SERVER_STATE_INFO	_ServerStatInfo;
}; 

// #define		CMSG_NO_USER_LOGOUT_DS_GS		102		// ���� �α׾ƿ�
struct CMSG_USER_LOGOUT_DS_GS
{
	u_short				usLength ;
	u_char				cMessage;

	char				cNDAccount[en_max_lil+1];			//����� ����
	char				cReason;				//����

	// �� ��Ŷ�� ���������� uid�� �������� �ʾƵ� ��.
	// 0 : �ٸ� ������ �α���
	// 1 : ��ڿ����� �α׾ƿ�
	// 2 : ����( ����... )	
	// 3 : ������ ĳ�� ��� �Ҹ�(������ؾ� ��)
};


// #define		CMSG_SHOWTEXT_REQ_DS_GS			103	// GS�� �������� ��� ��û
struct CMSG_SHOWTEXT_REQ{
	u_short				usLength ;
	u_char				cMessage;

	char				cString[MAX_CHAT_LENGTH];
};


//#define		CMSG_NO_CREATE_ITEM_GS_DS		105 // �ʵ忡 ITEM DROP(����)
struct CMSG_CREATE_ITEM_GS_DS {
	u_short				usLength ;
	u_char				cMessage;

	char				cFirstType ;		// ��з�
	short				sSecondType ;		// �Һз�
	float				fX ;				// x
	float				fZ ;				// y
};

// #define		CMSG_SYSTEM_OFF_REQ_NUM			110	// �ý��� ���� DS -> GS
struct CMSG_SYSTEM_OFF_REQ {
	u_short				usLength ;
	u_char				cMessage;	
};

//#define		CMSG_NO_SETPOSITION_REQ_DS_GS	120	// ĳ���� ��ġ �ʱ�ȭ ��û
struct CMSG_SETPOSITION_REQ{
	u_short				usLength ;
	u_char				cMessage;

	char				cCharacName[13];	// ĳ���� �̸�
	float				fInitPosX;			// �ʱ� ��ġ
	float				fInitPosZ;			//	
};


//#define		CMSG_NO_ALL_SPELL_REQ				113	// �鵵/�浵 �Ǵ� ��ü�� �ش� ���� ����. :GS->DS
//#define		CMSG_NO_ALL_SPELL_APPLY				114	// 113 ����� DS -> GS �� ������ ��Ŷ. ������ ����.
struct CMSG_ALL_SPELL_REQ
{
	u_short				usLength ;
	u_char				ucMessage ;

	char				cApplyTarget ;			// -1 �浵, 1 �鵵.

	short				spell ;					//
	char				step ;					//
} ;

typedef CMSG_ALL_SPELL_REQ	CMSG_ALL_SPELL_APPLY ;

struct CMSG_QUEST_REWARD_NOTICE
{
	u_short				usLength;
	u_char				ucMessage;

	char				cTargetMap;			// 100 �̸��� ����, �� �ε���, �̻��� ���� �׷� �ε���
	char				cTargetSex;			// 0 : ��ü, 1 : ����, 2 : ����.
	char				cTargetCommunity;	// 0 : ����. 1 : ����, 2 : ������.
	int					iTargetMoonpa;		// ���� �ε���. üũ�ϴ� �Լ� �����.

	int					iGroupIndex;		// �����̳�, ������ �ε���.

	_effect				effect;
};


//#define		CMSG_NO_ALL_SYSTEM_MSG				115	// GS->DS �� �������� �޼��� ���.
struct CMSG_ALL_SYSTEM_MSG
{
	u_short				usLength ;
	u_char				ucMessage ;

	char				cMode ;					//	0-15�Ϲ�(Į��), 16 ���� ,17 ����ê 32 �ͼӸ� 33 �����Ϲ�  34 ����Ӹ� 64 ��ε�, 127 ����� �޼���. ������ ��. -1 �� ä�� ��������.

	char				cName[13] ;
	char				cSay[MAX_CHAT_LENGTH];				//	ä�� �޽���( strlen + 1 : NULL ���� ) // 51�� �ִ���̴�..���� �� ©�� ����.

	char				cMoonpa ;
} ;



// 130 ~ 139 �� ���� �����. ����� ��, ��ȣ������ �����ּ���.
//#define		CMSG_NO_CONSOLE_COMMAND		139
enum CONSOLE_COMMAND {
	// Vina Shutdown
	CONSOLECMD_DECREASE_ACQUISITION_POINT_OF_JIN,		// ����ġ ����Ʈ�� �г�Ƽ ����
	CONSOLECMD_DECREASE_ALL_ACQUISITION_POINT,			// ��� ����Ʈ�� ���� �г�Ƽ ����
	CONSOLECMD_FORCED_LOGOFF,							// ���� ���� ����
	
	CONSOLECMD_TESTMODE_ON,								// �׽�Ʈ ��� on
	CONSOLECMD_TESTMODE_OFF,							// �׽�Ʈ ��� off

	// insert below for other command cases
};
struct		CMSG_CONSOLE_COMMAND {
	u_short			usLength ;
	u_char			ucMessage ;

	UCHAR			ucCommand;	// CONSOLE_COMMAND�� ����ȯ�ؼ� ���
	int					iUnion;			// �����Ӱ� ����ϴ� ����
};


//////////////////////////////////////////////////////////////////////////
//	135�� ������ [������Ŷ]���� ����մϴ�.
//	�� ��ȣ ã�� ���..
//////////////////////////////////////////////////////////////////////////
//#define MSG_NO_COMMOM_REQ		67
//#define MSG_NO_COMMON_ACK		67
//#define MSG_NO_COMMON_NTF		67

//#define CMSG_NO_COMMOM_REQ		135
//#define CMSG_NO_COMMON_ACK		135
//#define CMSG_NO_COMMON_NTF		135

#define PRESENT_MSG_SIZE		100

struct _sUser_Inven_v1_all {};	// ������ ������ DS�� Dummy Structure ����

typedef struct _MSG_COMMON {
	u_short			usLength;
	u_char			ucMessage;

	WORD			wMode;
/*
	//////////////////////////////////////////////////////////////////////////
	// �ؽ� �Ǿ��� ���� �� ����
	0: GS2C_NTF		[������ �Ǿ��� ����]
	1: GS2C_NTF		[������ ���� �ð� ����ȭ] 10�� ���� ������ Ŭ���̾�Ʈ ���� ������ ���� �ð��� ����ȭ �Ѵ�.

	2: DS2GS_NTF	[������ �Ǿ��� ����]
	
	//////////////////////////////////////////////////////////////////////////
	// ���� CIS(Cash Item Shop)
	10: C2GS_REQ	[������ ����Ʈ ��û] ĳ���κ��� ������ ����Ʈ ��û
	11: GS2C_ACK	[�ƾ��� ����Ʈ ����] ĳ���κ��� ������ ����Ʈ ����
	12: GS2DS_REQ	[������ ����Ʈ ��û] ĳ���κ��� ������ ����Ʈ ��û
	13: DS2GS_ACK	[�ƾ��� ����Ʈ ����] ĳ���κ��� ������ ����Ʈ ����

	15:	C2GS_REQ	[������ �̵� ��û] ĳ���κ����� �೶���� ������ �̵� ��û
	16:	GS2C_ACK	[������ �̵� ����] ĳ���κ����� �೶���� ������ �̵� ����
	17:	GS2DS_REQ	[������ �̵� ��û] ĳ���κ����� �೶���� ������ �̵� ��û
	18:	DS2GS_ACK	[������ �̵� ����] ĳ���κ����� �೶���� ������ �̵� ����

	20: C2GS_REQ	[���� ������ ������ ��û]
	21: GS2C_ACK	[���� ������ ������ ����]
	22: GS2DS_REQ	[���� ������ ������ ��û]
	23: DS2GS_ACK	[���� ������ ������ ����]
*/

	union {
		BYTE	data[1024];

#ifdef _NXPCB_AWARD_200703_
		// wMode = 0, 2
		struct _NXPCB_INFO {
			char			acc[en_max_lil+1];
			NEXON_PCB_INFO	info;			// �Ǿ��� ����
		} NXPCB_Info;

		// wMode = 1
		struct _NXPCB_TOUCH_ATTACK_REMAIN_MIN {
			WORD			wRemainMin;		// ������ ���� �ð� ��(min) ������ ����
		} NXPCBTouchAttackRemainMin;
#endif

#ifdef _CIS_20070326_
		// wMode = 10
		struct _CIS_CASH_INVEN_REQ {
			bool	bIsPresent;		// 0: �Ϲ� ������, 1: ���� ������
			WORD	wPageNum;		// ������ ��ȣ; �������� �������� �Ͱ� ����, 1 base-index
			WORD	wNumPerPage;	// ������ �� �ڷ��
		} CIS_CASH_INVEN_REQ;

		// wMode = 11
		struct _CIS_CASH_INVEN_ACK {
			USHORT	usTotalCnt;		// ĳ���κ��� (��� �������� ������) �� ������ ����; �� �������� �������� ����ϱ� ����
			UCHAR	ucRowCount;		// �������� ������ �� ������ ����
				// ucRowCount ��ŭ �ݺ�
				int		iPkgGuid;	// �������� Guid
				int		iPkgIdx;	// ��Ű�� �ε���
		} CIS_CASH_INVEN_ACK;

		// wMode = 12
		struct _GS2DS_CIS_CASH_INVEN_REQ {
			char	szAcc[en_max_lil+1];
			bool	bIsPresent;		// 0: �Ϲ� ������, 1: ���� ������
			WORD	wPageNum;		// ������ ��ȣ; �������� �������� �Ͱ� ����, 1 base-index
			WORD	wNumPerPage;	// ������ �� �ڷ��
		} GS2DS_CIS_CASH_INVEN_REQ;

		// wMode = 13
		struct _DS2GS_CIS_CASH_INVEN_ACK {
			char	szAcc[en_max_lil+1];
			USHORT	usTotalCnt;		// ĳ���κ��� (��� �������� ������) �� ������ ����; �� �������� �������� ����ϱ� ����
			UCHAR	ucRowCount;		// �������� ������ �� ������ ����
				// ucRowCount ��ŭ �ݺ�
				int		iPkgGuid;	// �������� Guid
				int		iPkgIdx;	// ��Ű�� �ε���
		} DS2GS_CIS_CASH_INVEN_ACK;

		// wMode = 15
		struct _CIS_MOVE_ITEM_REQ {
			UCHAR	ucPkgCnt;	// �̵��� ������ ����
				// ucPkgCnt ��ŭ �ݺ�
				int		iPkgGuid;	// �������� Guid
				int		iPkgIdx;	// ��Ű�� �ε���
		} CIS_MOVE_ITEM_REQ;

		// wMode = 16
		struct _CIS_MOVE_ITEM_ACK {
			UCHAR	ucPkgCnt;	// �ű�⿡ ������ ��Ű�� ������ ����
				// ucPkgCnt ��ŭ �ݺ�
				int		iPkgGuid;	// �������� Guid
			
			UCHAR	ucItemCnt;	// �೶���� �Ű��� ������ ����
				// ucItemCnt ��ŭ �ݺ�
				//char	cSlot;		// �೶�� ���� �ε���
				//char	cType;		// ������ �з�
				//short	sId;
				//UCHAR	ucItemNum;	// �������� ������ ����
				_sUser_Inven_v1_all	Item;
		} CIS_MOVE_ITEM_ACK;

		// wMode = 17
		struct _GS2DS_CIS_MOVE_ITEM_REQ {
			char	szAcc[en_max_lil+1];
			UCHAR	ucPkgCnt;	// �̵��� ������ ����; GS���� �󽽷� �˻��� ucPkgCnt �����Ͽ� DS�� ����
				// ucPkgCnt ��ŭ �ݺ�
				int		iPkgGuid;	// �������� Guid
		} GS2DS_CIS_MOVE_ITEM_REQ;

		// wMode = 18
		struct _DS2GS_CIS_MOVE_ITEM_ACK {
			char	szAcc[en_max_lil+1];
			UCHAR	ucPkgCnt;	// �ű�⿡ ������ ��Ű�� ������ ����; ���� ����� ���� ucPkgCnt �����Ͽ� GS�� ����
				// ucPkgCnt ��ŭ �ݺ�
				int		iPkgGuid;	// �������� Guid
				int		iPkgIdx;	// ��Ű�� �ε���
				bool	bRes;		// ĳ���κ����� ������ ���� ����/���� ����
		} DS2GS_CIS_MOVE_ITEM_ACK;

		// wMode = 20
		struct _CIS_PRESENT_MSG_REQ {
			int		iPkgGuid;	// �������� Guid
		} CIS_PRESENT_MSG_REQ;

		// wMode = 21
		struct _CIS_PRESENT_MSG_ACK {
			char		szFrom[en_max_lil+1];	// ������ ������ Account
			SYSTEMTIME	time;					// ���� ���� ��¥(��/��/��/��/��/��); DBTIMESTAMP
			char		szMsg[PRESENT_MSG_SIZE+1];	// ���� �޽���
		} CIS_PRESENT_MSG_ACK;

		// wMode = 22
		struct _GS2DS_CIS_PRESENT_MSG_REQ {
			char	szTo[en_max_lil+1];
			int		iPkgGuid;	// �������� Guid
		} GS2DS_CIS_PRESENT_MSG_REQ;

		// wMode = 23
		struct _DS2GS_CIS_PRESENT_MSG_ACK {
			char		szTo[en_max_lil+1];		// ���� ���� ������ Account
			char		szFrom[en_max_lil+1];	// ������ ������ Account
			SYSTEMTIME	time;					// ���� ���� ��¥(��/��/��/��/��/��); DBTIMESTAMP
			char		szMsg[PRESENT_MSG_SIZE+1];	// ���� �޽���
		} DS2GS_CIS_PRESENT_MSG_ACK;
#endif
	};
} MSG_COMMON, MSG_COMMON_REQ, MSG_COMMON_ACK, MSG_COMMON_NTF;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//#define		CMSG_GM_EDIT_COLLECTION_NUM		140		// ����(��ǰ) ������ ����Ʈ
struct CMSG_GM_EDIT_COLLECTION
{	
	u_short				usLength ;
	u_char				cMessage;

	char				sCharacName[13];
	u_char				ucEditFlag;		//0: ��� �÷����� �Ϸ��� �� �ּ���
	//1: ��� �÷����� ��� 0���� Ŭ���� �� �ּ���(�Ϸ�->�̿�)
	u_short				usCollectionIndex;
};

//#define		CMSG_GM_EDIT_EXPRATE_NUM		141		// ���� ���� ����
struct CMSG_GM_EDIT_EXPRATE
{
	u_short			usLength ;
	u_char			ucMessage ;
	
	int				iLevelExp;
	int				iSkillExp;
};



//#define		CMSG_GM_ADD_MASTERY_NUM			172		//	���� �߰�
struct	CMSG_GM_ADD_MASTERY
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	short				sMasteryID;
	char				cPoint;
	char				cPage;
};
//#define		CMSG_GM_REM_MASTERY_NUM			173		//	���� ����
struct	CMSG_GM_REM_MASTERY
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	short				sMasteryID;
};
//#define		CMSG_GM_EDIT_MASTERY_NUM			174		//	���� ����Ʈ(���� ����Ʈ ��ȭ)
struct	CMSG_GM_EDIT_MASTERY
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	short				sMasteryID;
	char				cPoint;
};
//#define		CMSG_GM_EDIT_LEFTMASTERYPOINT_NUM	175	//	���� ���� ����Ʈ ����Ʈ
struct	CMSG_GM_EDIT_LEFTMASTERYPOINT
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	short				sPoint;
};
//#define		CMSG_GM_EDIT_CHARAC_ROLE_NUM			176		//	���� �ٲٱ�
struct	CMSG_GM_EDIT_CHARAC_ROLE
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	char				cClass;
};
//#define		CMSG_GM_EDIT_CHARAC_CLASSGRADE_NUM	177	//	���ҵ�� �ٲٱ�
struct	CMSG_GM_EDIT_CHARAC_CLASSGRADE
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	char				cClassGrade;
};	




//#define	CMSG_NO_GM_EDIT_LEVEL_NUM						//	���� ����Ʈ
struct CMSG_GM_EDIT_LEVEL	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	int					iLevel;
};
//#define	CMSG_NO_GM_EDIT_VITAL_NUM						//	�� ����Ʈ
struct CMSG_GM_EDIT_VITAL	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	short				sVital;
	unsigned int		uiHP;					//�ٲ�� �Ǵ� ü��
};
//#define	CMSG_NO_GM_EDIT_ZEN_NUM							//	�� ����Ʈ
struct CMSG_GM_EDIT_ZEN	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sZen;
	unsigned int		uiMP;					//�ٲ�� �Ǵ� ����
};
//#define	CMSG_NO_GM_EDIT_SOUL_NUM							//	�� ����Ʈ
struct CMSG_GM_EDIT_SOUL	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sSoul;
	unsigned int		uiSoul;					//�ٲ�� �Ǵ� ����
};
//#define	CMSG_NO_GM_EDIT_HEART_NUM						//	�� ����Ʈ
struct CMSG_GM_EDIT_HEART	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sDexterity;
};
//#define	CMSG_NO_GM_EDIT_JUNGGI_NUM						//	�� ����Ʈ
struct CMSG_GM_EDIT_JUNGGI	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sStr;
};
//#define	CMSG_NO_GM_EDIT_LEFTPOINT_NUM					//	���� ����Ʈ ����Ʈ
struct CMSG_GM_EDIT_LEFTPOINT	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sLeftPoint;
};
//#define	CMSG_NO_GM_EDIT_ZONE_NUM							//	�� �ٲٱ�
struct CMSG_GM_EDIT_ZONE	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	int					iZone;
	float				fX;
	float				fZ;
};
//#define		CMSG_GM_EDIT_CODE_NUM					//	�ڵ� �ٲٱ�
struct CMSG_GM_EDIT_CODE	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	float				fX;
	float				fZ;
};
//#define	CMSG_NO_GM_EDIT_PARTY_NUM						//	��Ƽ �ٲٱ�
struct CMSG_GM_EDIT_PARTY	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	char				cParty;
};
//#define	CMSG_NO_GM_EDIT_NAME_NUM							//	�̸� �ٲٱ�(���� �̸��� �����ϴ��� ����� �̹� ����)
struct CMSG_GM_EDIT_NAME	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	char				Name[13];
};
//#define	CMSG_GM_EDIT_NICK_NUM							//	�г��� �ٲٱ�(���� �г����� �����ϴ��� ����� �̹� ����)
struct CMSG_GM_EDIT_NICK	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	char				Name[13];
};

//#define		CMSG_GM_ADD_INVITEM_NUM				//	�κ��丮�� ������ �߰�
struct CMSG_GM_ADD_INVITEM	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID

	unsigned __int64             ui64UniqueId; // 

	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	unsigned char		ucItemCount;
	unsigned short		usDurability;
	unsigned char		ucSlotCount;			//�����ۿ� ������ �ִ� ��� ���

	unsigned char		ucInchant;					//�������� ���ð�

};

//#define		CMSG_GM_REM_INVITEM_NUM			163		//	�κ��丮�� ������ ����
struct CMSG_GM_REM_INVITEM	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	u_char				ucSlotNo;
};

struct CMSG_GM_ADD_DEPITEM	{
	u_short				usLength ;
	u_char				cMessage;
	char				cAccount[en_max_lil+1];			//UniqueUserID

	unsigned __int64             ui64UniqueId; // 

	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	unsigned char		ucItemCount;
	unsigned char		ucSlotCount;			//�����ۿ� ������ �ִ� ��� ���
	unsigned char		ucInchant;					//�������� ���ð�
};




//#define	CMSG_NO_GM_REM_DEPITEM_NUM						//	����ҿ� ������ ����

struct CMSG_GM_REM_DEPITEM	{
	u_short				usLength ;
	u_char				cMessage;
	char				cAccount[en_max_lil+1];			// 
	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	char				cSlotNo;
};


//#define	CMSG_NO_GM_ADD_INVGOLD_NUM						//	�κ��丮�� �� ����
struct CMSG_GM_ADD_INVGOLD	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	int					iGold;
};


//#define	CMSG_NO_GM_ADD_DEPGOLD_NUM						//	������� �� ����
struct CMSG_GM_ADD_DEPGOLD	{
	u_short				usLength ;
	u_char				cMessage;
	char				cAccount[en_max_lil+1];			// 
	int					iGold;
};
//#define	CMSG_NO_GM_ADD_SKILL_NUM							//	��ų �߰�
struct CMSG_GM_ADD_SKILL	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sSkillID;
	short				sSkillStep;	
	int					iSkillExp;
};
//#define	CMSG_NO_GM_REM_SKILL_NUM							//	��ų ����
struct CMSG_GM_REM_SKILL	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sSkillID;
};
//#define	CMSG_NO_GM_EDIT_SKILL_NUM						//	��ų ����Ʈ(��ų st
struct CMSG_GM_EDIT_SKILL	{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];			//UniqueUserID
	short				sSkillID;
	short				sSkillStep;
	char				cSkillState ;			// ������ �ʿ� ����(0 �Ϸ�, 1 �ʿ�)
	int					iSkillExp;
};

//#define		CMSG_GM_EDIT_CONTRIBUTE_NUM		171		//	���� �⿩�� ����
struct CMSG_GM_EDIT_CONTRIBUTE
{
	u_short				usLength ;
	u_char				cMessage;
	char				cCharacName[13];
	int					iContribute;
};

//#define		CMSG_GM_EDIT_CHARAC_MUTE_TIME_NUM	178		// ä�ñ�����Ű��
struct CMSG_GM_EDIT_CHARAC_MUTE
{
	u_short				usLength ;
	u_char				cMessage ;

	char				cCharacName[13] ;
	int					iTime ;				// ���� �ð�(��)
};

//#define		CMSG_GM_EDIT_JIN_NUM	179		// �� ����
struct CMSG_GM_EDIT_JIN
{
	u_short				usLength ;
	u_char				cMessage ;

	char				cCharacName[13] ;
	u_int				uiJin ;				// ��
} ;


//#define		CMSG_GM_EDIT_NICK_ID_NUM		180		// ��ȣ ����
struct CMSG_GM_EDIT_NICK_ID	
{
	u_short				usLength ;
	u_char				ucMessage ;
	char				cCharacName[13] ;
	short				sSlotNum ;
	u_short				usNickID ;
};

//#define		CMSG_GM_EDIT_INCHANTVALU_NUM		182		// ��ȣ ����
struct CMSG_GM_EDIT_INCHANTVALUE
{
	u_short				usLength ;
	u_char				ucMessage ;	

	unsigned char		ucWhere;	//0: Inven, 1: Depot

	union{
		char			sCharacName[13];	//ucWhere�� Inven�� ��� ���

		char				cAccount[en_max_lil+1];			// 
	};

	short				sSlotNumber;
	unsigned __int64    ui64UniqueId;
	unsigned char		ucInchantValue;
};


// CMSG_ENV_SETTING_NUM		146
struct CMSG_ENV_SETTING
{
	u_short				usLength;
	u_char				ucMessage;

	u_char				ucMode;

	int					iValue;

	CMSG_ENV_SETTING( u_char mode )
		: usLength( sizeof(CMSG_ENV_SETTING)), ucMessage(CMSG_ENV_SETTING_NUM), ucMode(mode), iValue(0)
	{
	}
};

enum EN_ENV_SETTING_MODE
{
	en_env_setting_cashitem_period_test=0,
};



//#define CMSG_GM_EDIT_VAR_NO			142		// ����Ʈ ��ǥ ��ȣ.
// DS -> GS
struct CMSG_GM_EDIT_VAR
{
	u_short				usLength ;
	u_char				ucMessage ;

	u_char				ucMode ;			// ����Ʈ�� ���� ������ ���ȴ�. ������ �޿�.

	CMSG_GM_EDIT_VAR( u_short size, u_char mode )
		: usLength(size), ucMessage(CMSG_GM_EDIT_VAR_NO), ucMode(mode)
	{
	}
};

enum
{
	en_ds_gs_edit_mode_honor=0,			// �� ��ġ ����Ʈ
	en_ds_gs_edit_mode_fame,			// �Ǹ� ��ġ ����Ʈ
	en_ds_gs_edit_mode_blood,			// ���� ��ġ ����Ʈ
};

struct CMSG_GM_EDIT_HONOR_VAR : public CMSG_GM_EDIT_VAR
{
	char			cCharacName[13] ;
	int				iValue ;

	CMSG_GM_EDIT_HONOR_VAR()
		: CMSG_GM_EDIT_VAR(sizeof(CMSG_GM_EDIT_HONOR_VAR), en_ds_gs_edit_mode_honor )
	{
	}
};

struct CMSG_GM_EDIT_FAME_VAR : public CMSG_GM_EDIT_VAR
{
	char			cCharacName[13] ;
	int				iValue ;

	CMSG_GM_EDIT_FAME_VAR()
		: CMSG_GM_EDIT_VAR(sizeof(CMSG_GM_EDIT_FAME_VAR), en_ds_gs_edit_mode_fame )
	{
	}
};

struct CMSG_GM_EDIT_BLOOD : public CMSG_GM_EDIT_VAR
{
	char			cCharacName[13];
	short			sBloodPoint;

	CMSG_GM_EDIT_BLOOD()
		: CMSG_GM_EDIT_VAR(sizeof(CMSG_GM_EDIT_BLOOD), en_ds_gs_edit_mode_blood )
	{
	}
};


#include "udp_packet.h"
//#define		CMSG_GM_EDIT_ITEM_NUM			143		// ������ ���� ����
struct CMSG_GM_EDIT_ITEM 
{
	u_short				usLength ;
	u_char				ucMessage ;

	u_char				ucMode ;			// ����Ʈ�� ���� ������ ���ȴ�. ������ �޿�.

	CMSG_GM_EDIT_ITEM( u_short length, u_char mode )
		: usLength(length), ucMessage(CMSG_GM_EDIT_ITEM_NUM), ucMode(mode)
	{
	}
};

// mode 0 - en_gm_edit_item_aius
struct CMSG_GM_EDIT_AIUS : public CMSG_GM_EDIT_ITEM
{
	char						cAccount[en_max_lil+1];

	_ACCOUNT_ITEM_USE_STATUS	aius;

	CMSG_GM_EDIT_AIUS() 
		: CMSG_GM_EDIT_ITEM( sizeof(CMSG_GM_EDIT_AIUS), en_gm_edit_item_aius )
	{
		ZeroMemory( cAccount, sizeof(cAccount) );
		ZeroMemory( &aius, sizeof(aius) );
	}
};

// en_gm_edit_item_edit_item
struct CMSG_GM_EDIT_ONE_ITEM : public CMSG_GM_EDIT_ITEM
{
	char						cCharacName[en_charac_name_length+1];

	u_char						slot;
	_sServer_InvenItem_v1		item;

	CMSG_GM_EDIT_ONE_ITEM()
		: CMSG_GM_EDIT_ITEM( sizeof(CMSG_GM_EDIT_ONE_ITEM), en_gm_edit_item_edit_item )
	{
		ZeroMemory( cCharacName, sizeof(cCharacName) );
		slot = 0;
		ZeroMemory( &item, sizeof(item) );
	}
};

//en_gm_edit_item_edit_period
struct CMSG_GM_EDIT_PERIOD : public CMSG_GM_EDIT_ITEM
{
	char						cCharacName[en_charac_name_length+1];

	u_char						slot;
	__inven_uselimit_info_v1	info;

	CMSG_GM_EDIT_PERIOD()
		: CMSG_GM_EDIT_ITEM( sizeof(CMSG_GM_EDIT_PERIOD), en_gm_edit_item_edit_period )
	{
		ZeroMemory( cCharacName, sizeof(cCharacName) );
		slot = 0;
		ZeroMemory( &info, sizeof(info) );
	}
};


//en_gm_edit_item_del_period
struct CMSG_GM_EDIT_DEL_PERIOD : public CMSG_GM_EDIT_ITEM
{
	char						cCharacName[en_charac_name_length+1];

	u_char						slot;

	CMSG_GM_EDIT_DEL_PERIOD()
		: CMSG_GM_EDIT_ITEM( sizeof(CMSG_GM_EDIT_DEL_PERIOD), en_gm_edit_item_del_period )
	{
		ZeroMemory( cCharacName, sizeof(cCharacName) );
		slot = 0;
	}
};

//en_gm_edit_item_edit_skintear
struct CMSG_GM_EDIT_EDIT_SKINTEAR : public CMSG_GM_EDIT_ITEM
{
	char						cCharacName[en_max_lil+1];
	
	u_char						ucWhere;

	u_char						slot;
	__inven_uselimit_info_v1	info;

	CMSG_GM_EDIT_EDIT_SKINTEAR()
		: CMSG_GM_EDIT_ITEM( sizeof(CMSG_GM_EDIT_EDIT_SKINTEAR), en_gm_edit_item_edit_skintear )
	{
		ZeroMemory( cCharacName, sizeof(cCharacName) );
		ucWhere = 0;
		slot = 0;
		ZeroMemory( &info, sizeof(info) );
	}
};

//////////////////////////////////////////////////////////////////////////
//
//  ���� �κ� ��Ʈ�� ��Ŷ.
//
//////////////////////////////////////////////////////////////////////////
enum EN_UNION_GROUP_LOBBY_CTRL
{
	en_ug_simple_grp_info=0,			// ���� ������ ����. : UDS -> GS
	en_ug_select_simple_grp=1,			// ���� ������ ����.
	en_ug_select_simple_grp_fail=2,		// ���� ������ ���� ����.

	en_ug_detail_grp_info=10,			// ���� �������� ����. ĳ���� ���� ����.

	en_ug_move_request,					// 11 ĳ���Ϳ� â�� �̵� ��û�Ѵ�.
	en_ug_move_result_complete,			// 12 �̵��� ����.
	en_ug_move_result_change_name,		// 13 �̸��� �ٲ� �ʿ䰡 �ִ�.
	en_ug_move_result_fail,				// 14 �̵� ��û ����.


	en_ug_change_name_request=20,		// �̸� �ٲٱ� ��û.
	en_ug_change_name_result,			// 21 ���.

	en_ug_notyet_move_in_new=30,		// 30 �ż����� : ���� ������ �Ϸ����� �ʾҴ�.
	en_ug_force_use_charac_in_new,		// 31 �ż����� : ���� ������ �Ϸ����� ���� ����������, ������ ������ �Ϸ��ϵ��� ����.
	en_ug_already_move_in_old,			// 32 �̹� ������ �Ϸ�� ������������ ����. �޼��� �����ְ� ƨ��.
	en_ug_force_use_reseult,			// 33 ������ force_use ��Ŷ �ϳ��� �Ϸ��� ������, �ֹι�ȣ �Է��� ���ԵǾ� ��û/�������� ���´�.

	en_ug_insert_user_pointer=40,		// DS,GS �� ����. DS->GS : ���� �����͸� �����Ѵ�.(����Ʈ ��Ŷ�� ���� ��ó�� )
} ;

//#define CMSG_NO_UNI_GROUP_LOBBY				20

struct CMSG_UNI_GROUP_LOBBY		// ���� ������ ��Ŷ ���̽� : ��� ��Ŷ�� ucMode �� ����.
{
	u_short				usLength ;
	u_char				cMessage ;

	u_char				ucMode ;	// packet_for_lobby �� enum(EN_UNION_GROUP_LOBBY_CTRL) �� ���.

	char				cNDAccount[en_max_lil+1];			// 

	CMSG_UNI_GROUP_LOBBY( u_short len, u_char mode )
		: usLength(len), cMessage(CMSG_NO_UNI_GROUP_LOBBY), ucMode(mode)
	{}

	void SetAccount( char * acc )
	{
		if( acc )
		{
			strncpy( cNDAccount, acc, en_max_lil+1 ) ;
		}
	}
} ;


struct CMSG_UG_SIMPLE_GROUP_INFO : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_simple_grp_info
	u_char			ucGroup_0_NoCharac ;		// ��õ�� ĳ���� ��.

	struct {
		u_char			ucMoveState ;		// �̵� ���� : 0 - �̿�, 1 - �Ϸ�� ����(�̵���ų�� ����)
		u_char			ucNoCharac[3] ;		// �� �� �������� ĳ���� ��.
	} GroupInfo[3] ;		// �׷츶���� ���¿� ĳ���� ����.

	CMSG_UG_SIMPLE_GROUP_INFO() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_SIMPLE_GROUP_INFO), en_ug_simple_grp_info )
	{}
} ;


struct CMSG_UG_SELECT_SIMPLE_GRP : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_select_simple_grp
	u_char			ucSelectNewGroupNo ;	// ������ 

	CMSG_UG_SELECT_SIMPLE_GRP() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_SELECT_SIMPLE_GRP), en_ug_select_simple_grp )
	{}
} ;


struct CMSG_UG_SELECT_SIMPLE_GRP_FAIL : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_select_simple_grp_fail
	u_char			ucSelectSimpleGrpFail ;		// ���� ���� : "�̹� ���� �Ϸ�� �������Դϴ�."

	CMSG_UG_SELECT_SIMPLE_GRP_FAIL() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_SELECT_SIMPLE_GRP_FAIL), en_ug_select_simple_grp_fail )
	{}
} ;


struct CMSG_UG_DETAIL_GRP_INFO : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_detail_grp_info=10
	// ĳ���� �̸�. ����. (â���.. )
	u_char			ucCharacCount ;
	u_char			ucDepotCount ;

	struct {
		char		cOldGroupIndex ;// 1,2,3 / 4,5,6 / 7,8,9
		u_char		ucCharacIndex ;	// 0,1,2
		short		sLevel ;		// ���� �ܰ�
		char		cName[13] ;		// �̸� 
	} DetailInfo[9] ;	// Max 9

	CMSG_UG_DETAIL_GRP_INFO() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_DETAIL_GRP_INFO), en_ug_detail_grp_info )
	{}
} ;


struct CMSG_UG_MOVE_REQUEST : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_move_request,					// 11 ĳ���Ϳ� â�� �̵� ��û�Ѵ�.
	u_char			ucSelectCharac[3] ;		// ���� ĳ���� �ε���. ������. 0~2, 0~2 ( 0-1, 1-2 ) ������ 0xff
	u_char			ucSelectDepotGroup ;	// ���� â�� ������ �ε���. 0,1,2 ������ 0xff 
	char			cPassword[13] ;			// �ֹι�ȣ ���ڸ� �Ǵ� ��Ÿ.

	CMSG_UG_MOVE_REQUEST() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_MOVE_REQUEST), en_ug_move_request )
	{}
} ;


struct CMSG_UG_MOVE_RESULT_COMPLETE : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_move_result_complete
	// 
	u_char			ucCompleteName[3][13] ;		// �ű� ĳ���� �̸�.
	u_short			usInnerLevel[3] ;			// ĳ���� ����.
	u_char			ucState[3] ;				// 0 - �Ϸ�, 1 - �̸��� �ٲ�� �Ѵ�.
	u_char			ucCompleteDepotGroup ;		// ������, 0xff

	CMSG_UG_MOVE_RESULT_COMPLETE() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_MOVE_RESULT_COMPLETE), en_ug_move_result_complete )
	{}
} ;

struct CMSG_UG_MOVE_RESULT_FAIL : public CMSG_UNI_GROUP_LOBBY
{
	//	en_ug_move_result_fail,				// 14 �̵� ��û ����.
	u_char			ucFail ;					// �������...

	CMSG_UG_MOVE_RESULT_FAIL() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_MOVE_RESULT_FAIL), en_ug_move_result_fail )
	{}
} ;

struct CMSG_UG_CHANGE_NAME_REQUEST : public CMSG_UNI_GROUP_LOBBY
{
	//	en_ug_change_name_request=20,		// �̸� �ٲٱ� ��û.
	u_char			ucIndex ;				// 0,1,2 �ٲٷ��� ĳ������ �ε���.
	u_char			ucNewName[13] ;			// ���̸�

	CMSG_UG_CHANGE_NAME_REQUEST() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_CHANGE_NAME_REQUEST), en_ug_change_name_request )
	{}
} ;


struct CMSG_UG_CHANGE_NAME_RESULT : public CMSG_UNI_GROUP_LOBBY
{
	//	en_ug_change_name_result,			// 21 ���.
	u_char			ucSuccess ;				// 0 - Success, ������ ����.
	u_char			ucIndex ;				// 
	u_char			ucNewName[13] ;			// 

	CMSG_UG_CHANGE_NAME_RESULT() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_CHANGE_NAME_RESULT), en_ug_change_name_result )
	{}
} ;


struct CMSG_UG_NOTYET_MOVE_IN_NEW : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_notyet_move_in_new : Server -> Client �뺸.
	CMSG_UG_NOTYET_MOVE_IN_NEW()
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_NOTYET_MOVE_IN_NEW), en_ug_notyet_move_in_new ) 
	{}
} ;

struct CMSG_UG_FORCE_USE_CHARAC_IN_NEW : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_force_use_charac_in_new : Client -> Server �� ��û 
	char		cPW[13]	;		// �߰� 2006/06/24 : Ȯ�� ��

	CMSG_UG_FORCE_USE_CHARAC_IN_NEW()
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_FORCE_USE_CHARAC_IN_NEW), en_ug_force_use_charac_in_new ) 
	{}
} ;

struct CMSG_UG_ALREADY_MOVE_IN_OLD : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_already_move_in_old : Server -> Client
	CMSG_UG_ALREADY_MOVE_IN_OLD()
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_ALREADY_MOVE_IN_OLD), en_ug_already_move_in_old ) 
	{}
} ;

struct CMSG_UG_FORCE_USE_RESULT : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_force_use_reseult
	u_char				ucResult ;	// 0 - Success, 1 - ��� Ȯ��, 2 - ����.

	CMSG_UG_FORCE_USE_RESULT() 
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_FORCE_USE_RESULT), en_ug_force_use_reseult )
	{}
} ;

struct CMSG_UG_INSERT_USER_POINTER : public CMSG_UNI_GROUP_LOBBY
{
	// en_ug_insert_user_pointer
	u_short					usTempUID ;

	CMSG_UG_INSERT_USER_POINTER()
		: CMSG_UNI_GROUP_LOBBY( sizeof(CMSG_UG_INSERT_USER_POINTER), en_ug_insert_user_pointer ) 
	{}
} ;


//#define		CMSG_NO_CHECK_DSGS_PACKET		21
enum
{
	en_check_packet_test_request=0,
	en_check_packet_test_success,
	en_check_packet_test_fail,

	en_check_packet_timeout_request=10,		// Ÿ�� �ƿ� ��Ŷ ��û. ( ������, 10�� �ȿ� ������ �� )
	en_check_packet_timeout_response,		// ����.
};
struct CMSG_CHECK_DSGS_PACKET
{
	u_short				usLength ;
	u_char				cMessage ;

	u_char				ucMode ;	// 0 - request, 1 - result success, 2 - result different

	struct {
		u_char		msg ;
		short		length ;
	} check_pair[30] ;
} ;


#ifdef _VINA_SHUTDOWN_20061023_
/////////////////////////////////////////////////////
//�Ƿε� �� DB�� ����
//#define CMSG_NO_SAVE_FATIGUE_WEIGHT		44
////////////////////////////////////////////////////
struct MSG_SAVE_FATIGUE_WEIGHT
{
	u_short			usLength;
	u_char			ucMessage;

	short			sFatigueWeight;
	char			cAccount[en_max_lil+1];
#ifdef _PD_SDS_GOSTOP_
	u_char			ucFatigueMode;			// 0 : Go, 1 : Stop.
#endif
};
#endif

#ifdef _PD_CHARAC_LTS_

//#define CMSG_NO_LTS						116		// ��ġ ����.
////////////////
//�⺻
//��ġ���� �⺻ ��Ŷ
struct CMSG_LTS
{
	u_short					usLength;
	u_char					ucMessage;

	u_char					ucPktMode;

	CMSG_LTS(u_short length, u_char mode)
		: usLength(length),ucMessage(CMSG_NO_LTS),ucPktMode(mode)
	{}
};

enum EN_LTS_GS_DS
{
	//pk�� ��ġ����
	en_lts_pk_insert_req_gs_ds =0,
	en_lts_pk_insert_success_ds_gs,
	en_lts_pk_insert_failed_ds_gs,
	en_lts_pk_del_req_gs_ds,

	// Ư���� ����
	en_lts_sp_insert_req_gs_ds = 10,
	en_lts_sp_insert_success_ds_gs,
	en_lts_sp_insert_failed_ds_gs,
	en_lts_sp_del_req_gs_ds,
};

//Pk�� ��ġ���� en_lts_pk_insert_req_gs_ds,
struct CMSG_LTS_PK_INSERT_REQ_GS_DS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	_sLTS_Data		PkData;

	CMSG_LTS_PK_INSERT_REQ_GS_DS()
		:CMSG_LTS(sizeof(CMSG_LTS_PK_INSERT_REQ_GS_DS), en_lts_pk_insert_req_gs_ds)
	{}
};

//	en_lts_pk_insert_success_ds_gs,
struct CMSG_LTS_PK_INSERT_SUCCESS_DS_GS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	_sLTS_Data		PkData;

	CMSG_LTS_PK_INSERT_SUCCESS_DS_GS()
		: CMSG_LTS(sizeof(CMSG_LTS_PK_INSERT_SUCCESS_DS_GS), en_lts_pk_insert_success_ds_gs)
	{}
};

//	en_lts_pk_insert_failed_ds_gs,
struct CMSG_LTS_PK_INSERT_FAILED_DS_GS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	u_char ucRes;

	CMSG_LTS_PK_INSERT_FAILED_DS_GS()
		: CMSG_LTS(sizeof(CMSG_LTS_PK_INSERT_FAILED_DS_GS), en_lts_pk_insert_failed_ds_gs)
	{}
};

//	en_lts_pk_del_req_gs_ds,
//2. pk�� ��ġ����
struct CMSG_LTS_PK_DEL_REQ_GS_DS : public CMSG_LTS
{
	char	szCharacName[en_charac_name_length+1];
	char	szDelCharacName[en_charac_name_length+1];

	CMSG_LTS_PK_DEL_REQ_GS_DS()
		: CMSG_LTS(sizeof(CMSG_LTS_PK_DEL_REQ_GS_DS), en_lts_pk_del_req_gs_ds)
	{}
};

//Ư����
struct CMSG_LTS_SP_INSERT_REQ_GS_DS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	_sLTS_Data		SpData;

	CMSG_LTS_SP_INSERT_REQ_GS_DS()
		: CMSG_LTS(sizeof(CMSG_LTS_SP_INSERT_REQ_GS_DS), en_lts_sp_insert_req_gs_ds)
	{}
};


//	en_lts_sp_insert_success_ds_gs,
struct CMSG_LTS_SP_INSERT_SUCCESS_DS_GS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	_sLTS_Data SpData;

	CMSG_LTS_SP_INSERT_SUCCESS_DS_GS()
		: CMSG_LTS(sizeof(CMSG_LTS_SP_INSERT_SUCCESS_DS_GS), en_lts_sp_insert_success_ds_gs)
	{}
};


//	en_lts_sp_insert_failed_ds_gs,
struct CMSG_LTS_SP_INSERT_FAILED_DS_GS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	u_char	ucRes;

	CMSG_LTS_SP_INSERT_FAILED_DS_GS()
		: CMSG_LTS(sizeof(CMSG_LTS_SP_INSERT_FAILED_DS_GS), en_lts_sp_insert_failed_ds_gs)
	{}
};

//en_lts_sp_del_req_gs_ds,

struct CMSG_LTS_SP_DEL_REQ_GS_DS : public CMSG_LTS
{
	char szCharacName[en_charac_name_length+1];
	char		szDelCharacName[en_charac_name_length+1];

	CMSG_LTS_SP_DEL_REQ_GS_DS()
		: CMSG_LTS(sizeof(CMSG_LTS_SP_DEL_REQ_GS_DS), en_lts_sp_del_req_gs_ds)
	{}
};
#endif//#ifdef _PD_CHARAC_LTS_

//////////////////////////////////////////////////////////////////////////
// ģ�� ��ȯ : 20070604 �߰�.
//////////////////////////////////////////////////////////////////////////
//#define		CMSG_NO_RECALL_FRIEND				(117)
struct CMSG_RECALL_FRIEND
{
	u_short					usLength;
	u_char					ucMessage;

	u_char					ucFriendMode;		// EN_CMSG_RECALL_FRIEND

	u_char					ucRFMode;			// 0 - ģ�� ��ȯ, 1 - ģ�������� �̵�.

	char					cFromName[en_charac_name_length+1];
	char					cTargetName[en_charac_name_length+1];



	CMSG_RECALL_FRIEND( u_short size, u_char mode, u_char rf_mode )
		: usLength( size ), ucMessage( CMSG_NO_RECALL_FRIEND ), ucFriendMode(mode), ucRFMode(rf_mode)
	{
	}

	inline void set_from_name( char * name ) 
	{ 
		CopyMemory(cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}

	inline void set_target_name( char * name ) 
	{ 
		CopyMemory(cTargetName, name, en_charac_name_length );
		cTargetName[en_charac_name_length] = 0;
	}
};


enum EN_CMSG_RECALL_FRIEND
{
	en_crf_request_recall=0,		// gs->ds->gs ��û ó�� 
	en_crf_answer,					// gs->ds->gs ���� ó��
	en_crf_force_move,				// gs->ds->gs ���� �̵�.( �����ÿ� ���������� ���� �̵���Ų��.)
	en_crf_fail,					// ��� �ܰ迡���� ���� �޽���.
};


//	en_crf_request_recall=0,		// gs->ds->gs ��û ó�� 
struct CMSG_RF_REQUEST_RECALL : public CMSG_RECALL_FRIEND
{
	u_char				ucFromZoneIndex;		// ��� �������� ��ȯ�ϴ°�.

	char				cMoonpaIndex;			// 

	CMSG_RF_REQUEST_RECALL( u_char rf_mode )
		: CMSG_RECALL_FRIEND( sizeof(CMSG_RF_REQUEST_RECALL), en_crf_request_recall, rf_mode )
	{
	}
};


//	en_crf_answer,					// gs->ds->gs ���� ó��
struct CMSG_RF_ANSWER : public CMSG_RECALL_FRIEND
{
	u_char				ucYesNo;		// 0 - yes, 1 - no
	u_char				ucReason;

	u_char				ucZoneIndex;

	float				fTargetX;
	float				fTargetZ;

	CMSG_RF_ANSWER( u_char rf_mode )
		: CMSG_RECALL_FRIEND( sizeof(CMSG_RF_ANSWER), en_crf_answer, rf_mode )
	{
	}
};


//	en_crf_force_move,				// gs->ds->gs ���� �̵�.( �����ÿ� ���������� ���� �̵���Ų��.)
struct CMSG_RF_FORCE_MOVE : public CMSG_RECALL_FRIEND
{
	u_char				ucFromZoneIndex;

	float				fTargetX;
	float				fTargetZ;

	CMSG_RF_FORCE_MOVE( u_char rf_mode )
		: CMSG_RECALL_FRIEND( sizeof(CMSG_RF_FORCE_MOVE), en_crf_force_move, rf_mode )
	{
	}
};


enum EN_RF_FAIL_REASON
{
	en_rf_answer_no=0,				// no ����.

	en_rf_fail_no_target=1,			// 1 Ÿ���� ����.
	en_rf_fail_no_item,				// ��ȯ ������ ����. 
	en_rf_fail_wrong_point,			// ��ȯ ������ ��ġ�� �̻��ϴ�. 

	en_rf_fail_my_con_trade=10,		// (�ڽ���) �ŷ����̴�.

	en_rf_fail_my_con_combat,		// (�ڽ���) ����� �������̴�.
	en_rf_fail_my_con_zone,			// (�ڽ���) �пյ�/ȣ���� �� ���� �����̴�.
	en_rf_fail_my_con_exbattle,		// (�ڽ���) �� �������̴�.

	en_rf_fail_my_busy,				// (�ڽ���) �ٸ� ĳ���Ϳ� ģ�� ��ȯ ���̴�.


	en_rf_fail_target_con_trade=20,	// �ŷ����̴�.

	en_rf_fail_target_con_combat,	// ����� �������̴�.
	en_rf_fail_target_con_zone,		// �пյ�/ȣ���� �� ���� �����̴�.
	en_rf_fail_target_con_exbattle,	// �� �������̴�.

	en_rf_fail_target_busy,			// �ٸ� ���� ģ�� ��ȯ ���̴�.
};

//	en_crf_fail,					// ��� �ܰ迡���� ���� �޽���.
struct CMSG_RF_FAIL : public CMSG_RECALL_FRIEND
{
	u_char				ucFailReason;

	CMSG_RF_FAIL( u_char rf_mode )
		: CMSG_RECALL_FRIEND( sizeof(CMSG_RF_FAIL), en_crf_fail, rf_mode )
	{
	}
};


#pragma pack( pop, enter_tcp_packet )
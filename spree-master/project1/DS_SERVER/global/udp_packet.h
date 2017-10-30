#pragma once
#pragma pack( push, enter_udp_packet )
#pragma pack(1)


/*==========================================================================================]
[				LS MSG ��Ŷ MESSAGE ��ȣ													]
[===========================================================================================]
[	���� : DS<->LS  ��Ŷ�� U �� �����Ѵ�..													]
[			�ٸ� UDP ���� �޽��� ��ȣ�� �ߺ��� �����ϱ� ���� LS->DS �޽��� ��ȣ�� 200 ������]
[			�����մϴ�.																		]
[											jkHong Add...									]
[==========================================================================================*/
#define		UMSG_LS_DS_TEST_NUM				120		// LS/DS �� ��� �׽�Ʈ
#define		UMSG_LOGIN_LS_DS_NUM			121
#define		UMSG_LOGIN_DS_LS_RESULT_NUM		122
#define		UMSG_LOGIN_INFO_REQ_LS_DS_NUM	123
#define		UMSG_LOGIN_INFO_RES_DS_LS_NUM	124
#define		UMSG_CHARAC_DELETE_DS_MS_NUM	125		// �޽��� ������ ĳ���� ������ �˸���.
#define		UMSG_GM_CHARAC_SET_DS_MS_NUM	126		// �޽����� Ư�� ĳ������ GM �Ӽ��� �����Ѵ�.
#define		UMSG_CHARAC_OR_KICK_NUM			127

// #define UMSG_LOGIN_DS_NUM	121
struct UMSG_LOGIN_LS_DS						//# LS->DS �α� ��û #
{
	int			iKey;
	char		cMessage ;				//	CMSG_NO_LOGIN_LS_DS(0)
	char		cServerNum ;

	char		cAccount[en_max_lil+1];			//	���� ����
#ifdef _NXPCB_AWARD_200703_
	char		cNXAcc[en_max_lil+1];			//	�ؽ� ����
#endif
	char		cPass[en_max_lpl+1];				// password
	int			iAccUid ;				// 
	int			iTempUniqueID ;			//  LS���� �ο��� UID
	u_int		iIp;					//	��������  IP
	int			iTime;					//	���� �õ� �ð�
	char		cClass ;

#ifdef _PD_VINA_PCROOM_INFO_
	u_char		ucPCRoomInfo;			// 0 - Home, 1~ PC Room.
#endif

#ifdef _METEL_LOGIN_200608_
	bool		bPCBang;				// PCBang User or not?
	METEL_BILLING_INFO	MetelBillingInfo;
#endif //_METEL_LOGIN_200608_
};


// �α��� ��� ��Ŷ
// #define UMSG_LOGIN_DS_LS_RESULT_NUM				122		// DS->LS LOGIN RESPONSE MESSAGE SUCCESS
// ���� ��ȣ
#define _ERRNO_USING_ACCOUNT_					-2
#define _ERRNO_DB_ERROR_						-3
#define _ERRNO_STATE_ERROR_						-4
#define _ERRNO_IS_FULL_							-5
struct UMSG_LOGIN_DS_LS_RESULT
{
	int			iKey ;
	char		cMessage;				// 1
	char		cForestNo ;				// ������ ��ȣ

	char		cAccount[en_max_lil+1];			//
	u_short		usUid ;
	int			iTempID ;	
	// �õ��� �� �� �ֱ� ������, ���������δ� ������ �� ����. 

	char		cServerNo ;				// ������ ���( sServerNo > 0 )�� ������ ��ȣ�� 
	// ������ ��쿡�� ���� ��ȣ�� ����. 
	// 0���� -1�� ���Ӽ��������� ����.
	// -2 : ������� ����
	// -3 : DB ����
	// -4 : ���� ����/�׳� ���з� ������.
};


//#define		UMSG_LOGIN_INFO_REQ_LS_DS_NUM	123
struct UMSG_LOGIN_INFO_REQ_LS_DS
{
	int			iKey ;
	char		cMessage ;
	char		cServerNum ;
};

//_FOREST_STATE_RUNNING = 0,			// ���� �۵���
//_FOREST_STATE_ISFULL = -1,			// ���� ��ȭ����
//_FOREST_STATE_CHECKUP_ = -2,			// ������ ������
//#define		UMSG_LOGIN_INFO_RES_DS_LS_NUM	124
struct UMSG_LOGIN_INFO_RES_DS_LS
{
	int			iKey ;
	char		cMessage ;
	char		cForestNum ;

	char		cForestState ;	
};

//#define		UMSG_CHARAC_DELETE_DS_MS_NUM	125
struct UMSG_CHARAC_DELETE_DS_MS {
	int			iKey ;
	char		cMessage ;

	char		cCharacName[13] ;
	int			iUid ;
};


// GM Charac setting
//#define		UMSG_GM_CHARAC_SET_DS_MS_NUM	126
struct UMSG_GM_CHARAC_SET_DS_MS {
	int			iKey ;
	char		cMessage ;

	char		cCharacName[13] ;
	char		cClass ;
};


//#define  UMSG_CHARAC_OR_KICK_NUM			127
struct UMSG_CHARAC_OR_KICK {
	int			iKey ;
	char		cMessage ;

	char		cCharacName[13] ;
	char		cORName[en_or_name_length+1] ;
};



struct UMSG_LS_DS_TEST
{
	int			iKey;
	char	cMessage;
};


#define	MSG_NO_LOCAL_COMMANDER			15	// DS ���� ��� ��Ŷ�̴�. ��ȣ ��ġ�� ����.
struct MSG_LOCAL_COMMANDER
{
	int			iKey;
	char		cMessage;

	u_char		ucMode;			// EN_LC_DS_PACKET_MODE �� ����Ѵ�.

	MSG_LOCAL_COMMANDER()
		: iKey(PASS_KEY), cMessage(MSG_NO_LOCAL_COMMANDER), ucMode(0)
	{
	}

	MSG_LOCAL_COMMANDER( u_char mode )
		: iKey(PASS_KEY), cMessage(MSG_NO_LOCAL_COMMANDER), ucMode(mode)
	{
	}
};


// mode : 
enum EN_LC_DS_PACKET_MODE
{
	en_lc_ds_open_req=0,
	en_ds_lc_open_result,

	en_lc_ds_close_req,
	en_ds_lc_close_result,

	en_lc_ds_off_req,
	en_ds_lc_off_result,

	en_lc_ds_on_req,					//
	en_ds_lc_on_result,					//
};

/*==========================================================================================]
[				GMS MSG ��Ŷ MESSAGE ��ȣ													]
[===========================================================================================]
[	���� : GMS <-> DS �� ����ϴ� MSG ��Ŷ�� M ���� �����Ѵ�.								]
[											jkHong Add...									]
[==========================================================================================*/

//[GMS ����]===============================================================================================

// 121 ~ 127 ���� �α��� ���� �޽���.

// GMS -> DS
#define		MSG_SYSTEM_INFO_REQ_NUM			1	// �ý��� ���� ��û GMS -> DS *
#define		MSG_SYSTEM_INFO_RES_NUM			2	// �ý��� ���� ���� DS -> GMS
//#define		MSG_MESSAGE_INFO_REQ_NUM		3	// ���� ���� ��û GMS -> DS *
//#define		MSG_MESSAGE_INFO_NUM			4	// ���� ���� ���� DS -> GMS
//#define		MSG_MESSAGE_SEND_NUM			5	// ���� �߰� ��û GMS -> DS *
//#define		MSG_MESSAGE_EDIT_SEND_NUM		6	// ���� ���� ��û GMS -> DS *
//#define		MSG_MESSAGE_DELETE_SEND_NUM		7	// ���� ���� ��û GMS -> DS *
//#define		MSG_MESSAGE_RESULT_NUM			8	// �������� �߰�/����/���� �� ���� ��� DS->GMS
#define		MSG_MESSAGE_DIRECT_NUM			9	// �������� �ٷ� ������.. GMS->DS

#define		MSG_SYSTEM_OFF_REQ_NUM			10	// �ý��� ���� GMS -> DS *

#define		MSG_SYSTEM_TIME_REQ_NUM			11	// �ý��� �ð� ��û(�ð� ����ȭ) GMS->DS
#define		MSG_SYSTEM_TIME_RES_NUM			12	// �ý��� �ð� ���� DS->GMS
#define		MSG_GM_REQ_SERVER_PLAYER_NUM	13	// �� ������ �������� ��û�Ѵ�.
#define		MSG_GM_SERVER_PLAYER_NUM		14	// �� ������ �������� ��û�Ѵ�.

// 15 �� LC ��Ŷ���� �����.

#define		MSG_SETPOS_REQ_NUM				20	// ĳ���� ��ġ���� �ʱ�ȭ
#define		MSG_SETPOS_RES_NUM				21	// ĳ���� ��ġ���� �ʱ�ȭ ����
#define		MSG_CREATE_ITEM_REQ_NUM			22	// create item request
#define		MSG_CREATE_ITEM_RES_NUM			23	// create item result

#define		MSG_CMD_USERKILL_NUM			31	// ����� ���� ����
#define		MSG_CMD_USERKILL_RESULT_NUM		32	// ����� ���� ����

#define		MSG_CHARAC_REQ_NUM				41	// ĳ���� ���� ��û
#define		MSG_CHARAC_RES_NUM				42	// ĳ���� ���� ����
#define		MSG_CHARAC_UPDATE_NUM			43	// ĳ���� ���� ������Ʈ

#define		MSG_USER_DEPOT_REQ_NUM			44	// â�� ���� ��û
#define		MSG_USER_DEPOT_RES_NUM			45	// â�� ���� ����
#define		MSG_INVEN_REQ_NUM				46	// �κ��丮 ���� ��û
#define		MSG_INVEN_RES_NUM				47	// �κ��丮 ���� ����
#define		MSG_USER_DEPOT_UPDATE_NUM		48	// â�� ���� ������Ʈ
#define		MSG_INVEN_UPDATE_NUM			49	// �κ��丮 ���� ������Ʈ

#define		MSG_QUEST_REQ_NUM				50	// ����Ʈ ��û
#define		MSG_QUEST_RES_NUM				51	// ����Ʈ ����
#define		MSG_QUEST_UPDATE_NUM			52	// ����Ʈ ������Ʈ


#define		MSG_MARTIAL_REQ_NUM				53	// ���� ��û
#define		MSG_MARTIAL_RES_NUM				54	// ���� ����
#define		MSG_MARTIAL_UPDATE_NUM			55	// ���� ������Ʈ..


// CHANGKHAN's EDITION   !GMS �� Message Num �� 1~99 ���� ����ϼ���.
#define		MSG_GM_EDIT_PARTY_NUM			60		//	��Ƽ �ٲٱ�
#define		MSG_GM_EDIT_LEVEL_NUM			61		//	���� ����Ʈ
#define		MSG_GM_EDIT_VITAL_NUM			62		//	�� ����Ʈ
#define		MSG_GM_EDIT_ZEN_NUM				63		//	�� ����Ʈ
#define		MSG_GM_EDIT_SOUL_NUM			64		//	�� ����Ʈ
#define		MSG_GM_EDIT_HEART_NUM			65		//	�� ����Ʈ
#define		MSG_GM_EDIT_JUNGGI_NUM			66		//	�� ����Ʈ
#define		MSG_GM_EDIT_LEFTPOINT_NUM		67		//	���� ����Ʈ ����Ʈ
#define		MSG_GM_EDIT_ZONE_NUM			68		//	�� �ٲٱ�
#define		MSG_GM_EDIT_CODE_NUM			69		//	�ڵ� �ٲٱ�
#define		MSG_GM_EDIT_GMCLASS_NUM			70		//	GM CLASS �ٲٱ�
#define		MSG_GM_EDIT_NAME_NUM			71		//	�̸� �ٲٱ�(���� �̸��� �����ϴ��� ����� �̹� ����)
#define		MSG_GM_EDIT_NICK_NUM			72		//	�г��� �ٲٱ�(���� �г����� �����ϴ��� ����� �̹� ����)
#define		MSG_GM_ADD_INVITEM_NUM			73		//	�κ��丮�� ������ �߰�
#define		MSG_GM_REM_INVITEM_NUM			74		//	�κ��丮�� ������ ����
#define		MSG_GM_ADD_DEPITEM_NUM			75		//	����ҿ� ������ �߰�
#define		MSG_GM_REM_DEPITEM_NUM			76		//	����ҿ� ������ ����
#define		MSG_GM_ADD_INVGOLD_NUM			77		//	�κ��丮�� �� ����
#define		MSG_GM_ADD_DEPGOLD_NUM			78		//	������� �� ����
#define		MSG_GM_ADD_SKILL_NUM			79		//	��ų �߰�
#define		MSG_GM_REM_SKILL_NUM			80		//	��ų ����
#define		MSG_GM_EDIT_SKILL_NUM			81		//	��ų ����Ʈ(��ų step��ȭ)
#define		MSG_GM_EDIT_MAINQUEST_TRG_NUM	82		//	�������� ������ ����Ʈ�� Ʈ���Ÿ� �ٲ�
#define		MSG_GM_ADD_DOINGQUEST_NUM		83		//	�����߸���Ʈ�� ����Ʈ�� ����
#define		MSG_GM_REM_DOINGQUEST_NUM		84		//	�����߸���Ʈ���� ����Ʈ�� ����
#define		MSG_GM_EDIT_DOINGQUEST_TRG_NUM	85		//	�����߸���Ʈ�� ����Ʈ Ʈ���Ÿ� �ٲ�
#define		MSG_GM_REM_DONEQUEST_NUM		86		//	�������Ḯ��Ʈ���� ����Ʈ�� ����
#define		MSG_GM_EDIT_CONTRIBUTE_NUM		87		//	���ı⿩�� �ٲٱ�

#define		MSG_GM_ADD_MASTERY_NUM			88		//	���� �߰�
#define		MSG_GM_REM_MASTERY_NUM			89		//	���� ����
#define		MSG_GM_EDIT_MASTERY_NUM			90		//	���� ����Ʈ(���� ����Ʈ ��ȭ)
#define		MSG_GM_EDIT_LEFTMASTERYPOINT_NUM	91	//	���� ���� ����Ʈ ����Ʈ
#define		MSG_GM_EDIT_CHARAC_ROLE_NUM			92		//	���� �ٲٱ�
#define		MSG_GM_EDIT_CHARAC_CLASSGRADE_NUM	93	//	���ҵ�� �ٲٱ�

#define		MSG_GM_EDIT_MUTE_TIME_NUM			94		// ä�� ������Ű��
#define		MSG_GM_EDIT_LIMIT_CONNECTION_NUM	95		// ��������..
#define		MSG_GM_EDIT_JIN_NUM					96	// ��(����ġ) �ٲٱ�

#define		MSG_GM_EDIT_MAINQUESTID_NUM			97	//������ ����Ʈ ID �ٲٱ�
#define		MSG_GM_EDIT_INTOXICATION_NUM		98	//�ߵ� ����Ʈ
#define		MSG_GM_EDIT_ELIXIR_NUM				99	//���� �Ķ���� ����Ʈ
#define		MSG_GM_EDIT_NICK_ID_NUM				100	// ��ȣ ����
#define		MSG_GM_COPY_CHARAC_NUM				101	// ĳ���� ����


#define		MSG_GM_EDIT_COLLECTION_NUM			102	// ����(��ǰ) ������ ����Ʈ
#define		MSG_GM_EDIT_SERVER_EXPRATE_NUM		103	// ������ ����ġ ���� ����
#define		MSG_GM_EDIT_INCHANTVALUE_NUM		104	// �������� ��þƮ ���� ����
//#define MSG_GM_EDIT_VAR_NO			105			// ����Ʈ ��ǥ ��ȣ. �Ʒ���.
#define		MSG_GM_EVENT_REWARD_REQ_NUM			106 // �̺�Ʈ ���� ��û
#define		MSG_GM_EVENT_REWARD_RES_NUM			107 // �̺�Ʈ ���� ���

#ifdef _CHARACLIST_DELETE_CHARACTER_20061214_
	#define MSG_GM_DELETE_CHARAC_NUM			108		//ĳ���� ����
#endif

#define MSG_GM_EDIT_BLOOD_POINT_NUM				109		//��������Ʈ ����

// 20070703 �߰�
#define MSG_GM_EDIT_ITEM_PERIOD_NUM				110		// ���� �������� �Ⱓ ����.


// 20080803 �߰�.
#define MSG_GM_EVENT_NUM						(111)		// �̺�Ʈ : ���� + ���� + ���� ��� + ���� ���� + ���� ���� ��û.

#define MSG_GM_ORG_CTRL_NUM						(112)		// ������ ���� �޽���. 20081001


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 105 �� ���ϴ� �Ʒ��ʿ� Ray �� �߰��մϴ�.


struct _gmsHeader {
	int			iKey;					//��ȿ Ȯ�� Ű
	u_char		ucMessage;				// MSG_NO_GSINFO_RES_GS_DS(5)
	u_int		uiTime;
	char		cGMName[13];

	_gmsHeader()
		: iKey(PASS_KEY)
	{
	}
};



struct MSG_GM_EDIT_COLLECTION
{
	_gmsHeader		header ;
	char			sCharacName[13];
	u_char			ucEditFlag;		//0: ��� �÷����� �Ϸ��� �� �ּ���
									//1: ��� �÷����� ��� 0���� Ŭ���� �� �ּ���(�Ϸ�->�̿�)
	u_short			usCollectionIndex;
};


struct	MSG_GM_EDIT_CHARAC_CLASSGRADE
{
	_gmsHeader	header;
	char				cCharacName[13];
	char				cClassGrade;

};	

//#define		MSG_GM_ADD_MASTERY_NUM			88		//	���� �߰�
struct	MSG_GM_ADD_MASTERY
{
	_gmsHeader	header;
	char				cCharacName[13];
	short				sMasteryID;
	char				cPoint;
	char				cPage;
};
//#define		MSG_GM_REM_MASTERY_NUM			89		//	���� ����
struct	MSG_GM_REM_MASTERY
{
	_gmsHeader	header;
	char				cCharacName[13];
	short				sMasteryID;
};
//#define		MSG_GM_EDIT_MASTERY_NUM			90		//	���� ����Ʈ(���� ����Ʈ ��ȭ)
struct	MSG_GM_EDIT_MASTERY
{
	_gmsHeader	header;
	char				cCharacName[13];
	short				sMasteryID;
	char				cPoint;
};
//#define		MSG_GM_EDIT_LEFTMASTERYPOINT_NUM	91	//	���� ���� ����Ʈ ����Ʈ
struct	MSG_GM_EDIT_LEFTMASTERYPOINT
{
	_gmsHeader	header;
	char				cCharacName[13];
	short				sPoint;
};
//#define		MSG_GM_EDIT_CHARAC_ROLE_NUM		92		//	���� �ٲٱ�
struct	MSG_GM_EDIT_CHARAC_ROLE
{
	_gmsHeader	header;
	char				cCharacName[13];
	char				cClass;
};

// vitual message
#define		MSG_GS_SHUTDOWN_NUM				(-1)		//  GS�� ������ ������..
struct MSG_GS_SHUTDOWN
{
	int		iKey ;
	char	cMessage ;
	int		iServerNum ;
};


// #define		MSG_SYSTEM_INFO_REQ_NUM			1	// �ý��� ���� ��û GMS->DS
struct MSG_SYSTEM_INFO_REQ {				//���� �ý��� ���� ��û
	_gmsHeader	header;
	char	cServer_num;						//���� ��ȣ
};

// #define		MSG_SYSTEM_INFO_RES_NUM			2	// �ý��� ���� ���� DS->GMS
struct MSG_SYSTEM_INFO_RES					// # UMSG_GSINFO_RES_GS_DS �� ����..
{
	_gmsHeader			header;
	char				cServerNum;	

	_SERVER_SYSTEM_INFO		_ServerSystemInfo;				// ��������	
	_SERVER_NET_INFO_GMS	_ServerNetInfo;
	_SERVER_STATE_INFO		_ServerStateInfo;

	int					iCurLimitConnection ;
	bool				bServerState ;			// true : open, false : close 

	int					iLevelExp;
	int					iSkillExp;

};

#ifndef MAX_CHAT_LENGTH
#define MAX_CHAT_LENGTH		(81)
#endif
//#define		MSG_MESSAGE_DIRECT_NUM			9	// �������� �ٷ� ������.. GMS->DS
struct MSG_MESSAGE_DIRECT {
	_gmsHeader	header;
	char	cServerNum;		// 99 is brodcasting

	char	cString[MAX_CHAT_LENGTH];
};


// #define		MSG_SYSTEM_OFF_REQ_NUM			10	// �ý��� ���� GMS -> DS
struct MSG_SYSTEM_OFF_REQ {
	_gmsHeader	header;
	char	cServer_num;	
};

struct MSG_SYSTEM_OFF_REQ_FOR_GS {
	int		iKey ;
	char	cMessage ;
	char	cServer_num;	
};

//#define		MSG_SYSTEM_TIME_REQ_NUM			11	// �ý��� �ð� ��û(�ð� ����ȭ) GMS->DS
struct MSG_SYSTEM_TIME_REQ {
	_gmsHeader	header;
};

//#define		MSG_SYSTEM_TIME_RES_NUM			12	// �ý��� �ð� ���� DS->GMS
struct MSG_SYSTEM_TIME_RES {
	_gmsHeader	header;
	u_int		uiTime;

	u_short		usServerIndex;		//������ ��ȣ
};

//#define		MSG_GM_REQ_SERVER_PLAYER_NUM	13	// �� ������ ���� ��û.
struct MSG_GM_REQ_SERVER_PLAYER
{
	_gmsHeader	header;
	u_int		uiTime;
};


//#define		MSG_GM_SERVER_PLAYER_NUM		14	// �� ������ �������� ��û�Ѵ�.
struct MSG_GM_SERVER_PLAYER 
{
	_gmsHeader	header;
	u_int		uiTime;

	u_char		ucGreoupNo;					// Group Inodex

	u_short		usTotalConnectCount;		// ������ ����.

	struct 
	{
		u_short		usPlayerCount;			// Count of Connected Player.
		u_short		usConnectCount;
	} Count[35];
};

// ĳ���� ��ġ ����
//	#define		MSG_SETPOS_REQ_NUM				20	// ĳ���� ��ġ ���� �ʱ�ȭ GMS -> DS
struct MSG_SETPOS_REQ {
	_gmsHeader	header;
	char	cCharacName[13];

	float	fX ;
	float	fZ ;
};

// #define		MSG_SETPOS_RES_NUM				21	// ĳ���� ��ġ���� �ʱ�ȭ ����
struct MSG_SETPOS_RES {
	_gmsHeader	header;
	char	cCharacName[13];
	char	cResult;
};

//#define		MSG_CREATE_ITEM_REQ_NUM			22	// create item request
struct MSG_CREATE_ITEM_REQ {
	_gmsHeader	header;

	char	cFstType ;
	short	sScdType ;

	char	cPosType ;
	union {
		char	cCharacName[13] ;
		struct _pos {
			char	cServerNum ;
			float	fX ;
			float	fZ ;
		} pos ;		
	};
};

//#define		MSG_CREATE_ITEM_RES_NUM			23	// create item result
struct MSG_CREATE_ITEM_RES{
	_gmsHeader	header;

	char	cResult	;
};

//#define		MSG_CMD_USERKILL				31	// ����� ���� ����
struct MSG_CMD_USERKILL {
	_gmsHeader	header;
	char		cAccount[en_max_lil+1];
};

//#define		MSG_CMD_USERKILL_RESULT_NUM		32	// ����� ���� ����
struct MSG_CMD_USERKILL_RESULT {
	_gmsHeader	header;
	char		cAccount[en_max_lil+1];
	char		cResult;
};

//#define		MSG_CHARAC_REQ_NUM				41	// ĳ���� ���� ��û
struct MSG_CHARAC_REQ {
	_gmsHeader	header;
	char		cChrName[13];
};

//#define		MSG_CHARAC_RES_NUM				42	// ĳ���� ���� ����
struct MSG_CHARAC_RES {
	_gmsHeader	header;
	char		cChrName[13];
	_CHARAC_BASIC			CHARAC_BASIC;			//ND_V01_Charac
	_CHARAC_CUR_BASIC		CHARAC_CUR_BASIC;		//ND_V01_CurBasicInfo
	_CHARAC_LEVEL			CHARAC_LEVEL;			//ND_V01_5StarLevel
	_CHARAC_STATE			CHARAC_STATE;			//ND_V01_CharacState
};

//#define		MSG_CHARAC_UPDATE_NUM			43	// ĳ���� ���� ������Ʈ
struct MSG_CHARAC_UPDATE {
	_gmsHeader	header;
	char		cChrName[13];
	_CHARAC_BASIC			CHARAC_BASIC;			//ND_V01_Charac
	_CHARAC_CUR_BASIC		CHARAC_CUR_BASIC;		//ND_V01_CurBasicInfo
	_CHARAC_LEVEL			CHARAC_LEVEL;			//ND_V01_5StarLevel
	_CHARAC_STATE			CHARAC_STATE;			//ND_V01_CharacState
};


//#define		MSG_USER_DEPOT_REQ_NUM			44	// â�� ���� ��û
struct MSG_USER_DEPOT_REQ {
	_gmsHeader	header;
	char		cAccount[en_max_lil+1];
};

//#define		MSG_USER_DEPOT_RES_NUM			45	// â�� ���� ����
struct MSG_USER_DEPOT_RES {
	_gmsHeader	header;
	char		cAccount[en_max_lil+1];

	int			iSize;
	char		cNum;
	char		pInvData[UDP_PACKET_CUT_SIZE];
	//_USER_DEPOT stDepot ;
};

//#define		MSG_INVEN_REQ_NUM				46	// �κ��丮 ���� ��û
struct MSG_INVEN_REQ {
	_gmsHeader	header;
	char		cCharacName[13] ;
};

//#define		MSG_INVEN_RES_NUM				47	// �κ��丮 ���� ����
struct MSG_INVEN_RES {
	_gmsHeader	header;
	char		cCharacName[13] ;

	int			iSize;
	char		cNum;
	char		pInvData[UDP_PACKET_CUT_SIZE];
	//_CHARAC_INVENTORY stInven ;
};

//#define		MSG_USER_DEPOT_UPDATE_NUM		48	// â�� ���� ������Ʈ
struct MSG_USER_DEPOT_UPDATE {
	_gmsHeader	header;
	char		cAccount[en_max_lil+1];
	_USER_DEPOT stDepot ;
};

//#define		MSG_INVEN_UPDATE_NUM			49	// �κ��丮 ���� ������Ʈ
struct MSG_INVEN_UPDATE {
	_gmsHeader	header;
	char		cCharacName[13] ;
	_CHARAC_INVENTORY stInven ;
};

//#define		MSG_QUEST_REQ_NUM				50	// ����Ʈ ��û
struct MSG_QUEST_REQ {
	_gmsHeader	header;
	char		cCharacName[13] ;
};
//#define		MSG_QUEST_RES_NUM				51	// ����Ʈ ����
struct MSG_QUEST_RES {
	_gmsHeader	header;
	char		cCharacName[13];

	int			iSize;
	char		cNum;
	char		pQuestData[300];
	//	_CHARAC_QUEST_DATA	CHARAC_QUEST_DATA ;
};
//#define		MSG_QUEST_UPDATE_NUM			52	// ����Ʈ ������Ʈ
struct MSG_QUEST_UPDATE {
	_gmsHeader	header;
	char		cCharacName[13] ;

	_CHARAC_QUEST_DATA CHARAC_QUEST_DATA ;

};


//#define		MSG_GM_EDIT_MAINQUEST_TRG_NUM		82		//	�������� ������ ����Ʈ�� Ʈ���Ÿ� �ٲ�
struct MSG_GM_EDIT_MAINQUEST_TRG	{
	_gmsHeader	header;
	char				cCharacName[13];		//UniqueUserID
	u_char				cTrigger;				//�ٲ� Ʈ����(ID�� ������� �ٲ��ּ���~~)
};

//#define		MSG_GM_ADD_DOINGQUEST_NUM			83		//	�����߸���Ʈ�� ����Ʈ�� ����
struct MSG_GM_ADD_DOINGQUEST		{
	_gmsHeader	header;
	char				cCharacName[13];		//UniqueUserID
	unsigned short		sQuestID;
	u_char				cQuestTrigger;
	int					iTime;					//����Ʈ ���۽ð�
};


//#define		MSG_GM_REM_DOINGQUEST_NUM			84		//	�����߸���Ʈ���� ����Ʈ�� ����
struct	MSG_GM_REM_DOINGQUEST	{
	_gmsHeader	header;
	char				cCharacName[13];		//UniqueUserID
	unsigned short		sQuestID;	
};

//#define		MSG_GM_EDIT_DOINGQUEST_TRG_NUM		85		//	�����߸���Ʈ�� ����Ʈ Ʈ���Ÿ� �ٲ�
struct	MSG_EDIT_DOINGQUEST_TRG	{
	_gmsHeader	header;
	char				cCharacName[13];		//UniqueUserID
	unsigned short		sQuestID;
	u_char				cQuestTrigger;
};

//#define		MSG_GM_REM_DONEQUEST_NUM			86		//	�������Ḯ��Ʈ���� ����Ʈ�� ����
struct	MSG_GM_REM_DONEQUEST	{
	_gmsHeader	header;
	char				cCharacName[13];		//UniqueUserID
	unsigned short		sQuestID;	
};


//#define		MSG_MARTIAL_REQ_NUM				52	// ���� ��û
struct MSG_MARTIAL_REQ {
	_gmsHeader	header;
	char		cCharacName[13] ;	
};
//#define		MSG_MARTIAL_RES_NUM				53	// ���� ����
struct MSG_MARTIAL_RES {
	_gmsHeader	header;
	char		cCharacName[13] ;

	_CHARAC_MARTIAL_DATA CHARAC_MARTIAL_DATA ;
};
//#define		MSG_MARTIAL_UPDATE_NUM			54	// ���� ������Ʈ..
struct MSG_MARTIAL_UPDATE {
	_gmsHeader	header;
	char		cCharacName[13] ;

	_CHARAC_MARTIAL_DATA CHARAC_MARTIAL_DATA ;
};



//#define	MSG_NO_GM_EDIT_LEVEL_NUM						//	���� ����Ʈ
struct MSG_GM_EDIT_LEVEL	{
	_gmsHeader	header;
	char				cCharacName[13];
	int					iLevel;
};
//#define	MSG_NO_GM_EDIT_VITAL_NUM						//	�� ����Ʈ
struct MSG_GM_EDIT_VITAL	{
	_gmsHeader	header;
	char				cCharacName[13];
	short				sVital;
	unsigned int		uiHP;					//�ٲ�� �Ǵ� ü��
};
//#define	MSG_NO_GM_EDIT_ZEN_NUM							//	�� ����Ʈ
struct MSG_GM_EDIT_ZEN	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sZen;
	unsigned int		uiMP;					//�ٲ�� �Ǵ� ����
};
//#define	MSG_NO_GM_EDIT_SOUL_NUM							//	�� ����Ʈ
struct MSG_GM_EDIT_SOUL	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sSoul;
	unsigned int		uiSoul;					//�ٲ�� �Ǵ� ����
};
//#define	MSG_NO_GM_EDIT_HEART_NUM						//	�� ����Ʈ
struct MSG_GM_EDIT_HEART	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sDexterity;
};
//#define	MSG_NO_GM_EDIT_JUNGGI_NUM						//	�� ����Ʈ
struct MSG_GM_EDIT_JUNGGI	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sStr;
};
//#define	MSG_NO_GM_EDIT_LEFTPOINT_NUM					//	���� ����Ʈ ����Ʈ
struct MSG_GM_EDIT_LEFTPOINT	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sLeftPoint;
};
//#define	MSG_NO_GM_EDIT_ZONE_NUM							//	�� �ٲٱ�
struct MSG_GM_EDIT_ZONE	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	int					iZone;
	float				fX;
	float				fZ;
};
//#define		MSG_GM_EDIT_CODE_NUM					//	�ڵ� �ٲٱ�
struct MSG_GM_EDIT_CODE	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	float				fX;
	float				fZ;
};
//#define	MSG_NO_GM_EDIT_PARTY_NUM						//	��Ƽ �ٲٱ�
struct MSG_GM_EDIT_PARTY	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	char				cParty;
};
//#define	MSG_NO_GM_EDIT_NAME_NUM							//	�̸� �ٲٱ�(���� �̸��� �����ϴ��� ����� �̹� ����)
struct MSG_GM_EDIT_NAME	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	char				Name[13];
};
//#define	MSG_NO_GM_EDIT_NICK_NUM							//	�г��� �ٲٱ�(���� �г����� �����ϴ��� ����� �̹� ����)
struct MSG_GM_EDIT_NICK	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	char				Name[13];
};
//#define	MSG_NO_GM_ADD_INVITEM_NUM						//	�κ��丮�� ������ �߰�
/*struct MSG_GM_ADD_INVITEM	{
int					iKey;					//��ȿ Ȯ�� Ű
char				cMessage;				// MSG_NO_GSINFO_RES_GS_DS(5)
char				cCharacName[13];			//UniqueUserID
char				cFirstType;
char				cSecondType;
short				sItemID;
unsigned char		ucItemCount;
unsigned short		usDurability;
};*/


struct MSG_GM_ADD_INVITEM	
{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	unsigned char		ucItemCount;
	unsigned short		usDurability;
	unsigned char		ucSlotCount;			//�����ۿ� ������ �ִ� ��� ���
	unsigned char		ucInchant;					//�������� ���ð�

#ifdef _PD_GM_ADDITEM_MODIFY_
	char				cCashCheck;				// �Ⱓ�� ������ ������ ���� ����.
	u_short				usTimeValue;			// �Ⱓ�� ������ ������ ���� ����.
#endif
};

//#define	MSG_NO_GM_REM_INVITEM_NUM						//	�κ��丮�� ������ ����
struct MSG_GM_REM_INVITEM	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	u_char				ucSlotNo;
};


struct MSG_GM_ADD_DEPITEM	{
	_gmsHeader	header;
	char				cAccount[en_max_lil+1];
	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	unsigned char		ucItemCount;
	unsigned char		ucSlotCount;			//�����ۿ� ������ �ִ� ��� ���
	unsigned char		ucInchant;
#ifdef _PD_GMS_ADD_DEPOT_DUR_
	unsigned short		usDurability;
#endif

};




//#define	MSG_NO_GM_REM_DEPITEM_NUM						//	����ҿ� ������ ����

struct MSG_GM_REM_DEPITEM	{
	_gmsHeader	header;
	char				cAccount[en_max_lil+1];
	char				cFirstType;
	char				cSecondType;
	short				sItemID;
	char				cSlotNo;
};

//#define	MSG_NO_GM_ADD_INVGOLD_NUM						//	�κ��丮�� �� ����
struct MSG_GM_ADD_INVGOLD	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	int					iGold;
};


//#define	MSG_NO_GM_ADD_DEPGOLD_NUM						//	������� �� ����
struct MSG_GM_ADD_DEPGOLD	{
	_gmsHeader	header;
	char				cAccount[en_max_lil+1];
	int					iGold;
};
//#define	MSG_NO_GM_ADD_SKILL_NUM							//	��ų �߰�
struct MSG_GM_ADD_SKILL	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sSkillID;
	short				sSkillStep;
	int					iSkillExp;
};

//#define	MSG_NO_GM_REM_SKILL_NUM							//	��ų ����
struct MSG_GM_REM_SKILL	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sSkillID;
};

//#define	MSG_NO_GM_EDIT_SKILL_NUM						//	��ų ����Ʈ(��ų st
struct MSG_GM_EDIT_SKILL	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	short				sSkillID;
	short				sSkillStep;
	int					iSkillExp;
	char				cSkillState ;			// ������ �ʿ� ����(0 �Ϸ�, 1 �ʿ�)
};

//#define		MSG_GM_EDIT_GMCLASS_NUM			70		//	GM CLASS �ٲٱ�
struct MSG_GM_EDIT_GMCLASS	{
	_gmsHeader	header;
	char				cCharacName[13];
	char				cClass;
	int					iBlockingEndTime ;
};

//#define		MSG_GM_EDIT_CONTRIBUTE_NUM		87		//	���ı⿩�� �ٲٱ�
struct	MSG_GM_EDIT_CONTRIBUTE	{
	_gmsHeader	header;
	char				cCharacName[13];			//UniqueUserID
	int					iContribute;
};

//#define		MSG_GM_EDIT_MUTE_TIME_NUM			94		// ä�� ������Ű��
struct MSG_GM_EDIT_MUTE_TIME
{
	_gmsHeader	header;
	char				cCharacName[13] ;

	int					iMuteTime ;		
};

//#define		MSG_GM_EDIT_LIMIT_CONNECTION_NUM	95		// ��������..
struct MSG_GM_EDIT_LIMIT_CONNECTION
{
	_gmsHeader	header;

	int					iLimitCount ;
	bool				bServerState ;		// t:open f: close
};

//#define		MSG_GM_EDIT_JIN_NUM					96	// ��(����ġ) �ٲٱ�
struct MSG_GM_EDIT_JIN
{
	_gmsHeader	header;
	char		cCharacName[13];
	u_int		uiJin;
};

//#define		MSG_GM_EDIT_MAINQUESTID_NUM			97	//������ ����Ʈ ID �ٲٱ�
struct MSG_GM_EDIT_MAINQUESTID	{
	_gmsHeader		header;
	char			cCharacName[13];
	unsigned short	usQuestID;
	int				iTime;						//����Ʈ ���۽ð�
};

//#define		MSG_GM_EDIT_INTOXICATION_NUM			98	//�ߵ� ����Ʈ
struct MSG_GM_EDIT_INTOXICATION
{
	_gmsHeader		header;
	char			cCharacName[13];
	short			sIntoxication;
};
//#define		MSG_GM_EDIT_ELIXIR_NUM					99	//���� �Ķ���� ����Ʈ
struct MSG_GM_EDIT_ELIXIR
{
	_gmsHeader		header;
	char			cCharacName[13];
	char			cElixirType;	//0 �ٷ� //1 ���� //2 ���� //3 �ǰ�//4 ��ø
	char			cGrade;			//���� ���
	short			asData[10];		//���� ������ �迭
};



//#define		MSG_GM_EDIT_NICK_ID_NUM				100	// ��ȣ ����
struct MSG_GM_EDIT_NICK_ID
{
	_gmsHeader		header;
	char			cCharacName[13];
	short			sSlotNum ;				// ������ ����
	u_short			usNickID;				// ��ȣ ���̵�(0 �̸� ����)
};

//#define		MSG_GM_COPY_CHARAC_NUM				101	// ĳ���� ����
struct MSG_GM_COPY_CHARAC
{
	_gmsHeader		header;
	char			cSourceCharacName[13];
	char			cTargetCharacName[13] ;
};

//#define		#define		MSG_GM_EDIT_SERVER_EXPRATE_NUM		103	// ������ ����ġ ���� ����
struct MSG_GM_EDIT_EXPRATE
{
	_gmsHeader		header;

	char			cServerNum;
	int				iLevelExp;
	int				iSkillExp;
};


//#define		MSG_GM_EDIT_INCHANTVALUE_NUM	104	// �������� ��þƮ ���� ����
struct MSG_GM_EDIT_INCHANTVALUE
{
	_gmsHeader		header;

	unsigned char		ucWhere;	//0: Inven, 1: Depot

	union{
		char			sCharacName[13];	//ucWhere�� Inven�� ��� ���
		char			cAccount[en_max_lil+1];
	};

	short				sSlotNumber;
	unsigned __int64    ui64UniqueId;
	unsigned char		ucInchantValue;
};


// ����  : #define		UMSG_LOGIN_LS_DS_NUM			121 ��ȣ ���� ����.
// 105�� ���� ���⿡�� ������.
#define MSG_GM_EDIT_VAR_NO			105			// ����Ʈ ��ǥ ��ȣ.
struct MSG_GM_EDIT_VAR
{
	_gmsHeader		header ;

	u_char			ucMode ;			// ��忡 ����, �����Ѵ�.
};

enum
{
	en_gms_ds_edit_mode_honor=0,		// ��
	en_gms_ds_edit_mode_fame,			// �Ǹ�
};

struct MSG_GM_EDIT_HONOR : public MSG_GM_EDIT_VAR
{
	char				cCharacName[13];			//UniqueUserID
	int					iValue;
};

typedef MSG_GM_EDIT_HONOR		MSG_GM_EDIT_FAME		;


//#define		MSG_GM_EVENT_REWARD_REQ_NUM				106 // �̺�Ʈ ���� ��û
//#define		MSG_GM_EVENT_REWARD_RES_NUM				107 // �̺�Ʈ ���� ���

struct MSG_GM_EVENT_REWARD_REQ
{
	_gmsHeader			 header;

	int					iEventID;                          //�̺�Ʈ ���̵�
	char				pCharacName[13];         //ĳ����

	char				cItemType;                       //������Ÿ��
	char				cItemSecondType;
	short				sItemID;                            //������ ���̵�
	short				sItemCount;                     //������ ����

	unsigned short		usDurability;                     //������
	unsigned char		ucSlotCount;                    //�����ۿ� ������ �ִ� ��� ���
	unsigned char		ucInchant;                       //�������� ���ð�

	int					iResult;                                          //��� 0) ��ó��, 1) ����, 2) ����
};


struct MSG_GM_EVENT_REWARD_RES
{
	_gmsHeader			header;

	int					iEventID;             //�̺�Ʈ ���̵�
	int					iResult;  //��� 0) ��ó��, 1) ����, 2) ����
};

#ifdef _CHARACLIST_DELETE_CHARACTER_20061214_
//#define MSG_GM_DELETE_CHARAC_NUM					108	//ĳ���� ����
struct MSG_GM_DELETE_CHARAC{
	_gmsHeader	header;
	char 	pCharacName[13];//ĳ����
};
#endif


//#define MSG_GM_EDIT_BLOOD_POINT_NUM                                                               109//��������Ʈ ����
struct MSG_GM_EDIT_BLOOD_POINT
{
	_gmsHeader			header;
	char				szCharacName[13];
	short				sBloodPoint;                                 // ������ ��������Ʈ
};


//#define MSG_GM_EDIT_ITEM_PERIOD_NUM			110		// ���� �������� �Ⱓ ����.
struct MSG_GM_EDIT_ITEM
{
	_gmsHeader			header;
	u_char				ucMode;
};


enum
{
	en_gm_edit_item_aius=0,						// ����õ�� �Ⱓ ����.
	en_gm_edit_item_edit_item,					// ������ ����
	en_gm_edit_item_edit_period,				// �������� �Ⱓ ���� : �ٸ� �Ӽ��� �������� �ʴ´�.
	en_gm_edit_item_del_period,					// �������� �Ⱓ ������ ���ش�.
	en_gm_edit_item_edit_skintear,				// �븰/��� ����.
};

struct MSG_GM_EDIT_ITEM_AIUS : public MSG_GM_EDIT_ITEM
{
	// ��Ű�� ���� ����ü
	char							szAccount[en_max_lil+1];		// ������ ����.
	_ACCOUNT_ITEM_USE_STATUS		aius;
};

struct MSG_GM_EDIT_ONE_ITEM : public MSG_GM_EDIT_ITEM
{
	char						szCharacName[en_charac_name_length+1];

	u_char						slot;		// �� ������ 
	_sServer_InvenItem_v1		item;		// �������� �����Ѵ�.
};

struct MSG_GM_EDIT_ITEM_PERIOD : public MSG_GM_EDIT_ITEM
{
	char						szCharacName[en_charac_name_length+1];

	u_char						slot;
	__inven_uselimit_info_v1	info;		// �������� �Ⱓ �κи� �����Ѵ�.
};

struct MSG_GM_EDIT_DEL_PERIOD : public MSG_GM_EDIT_ITEM
{
	char						szCharacName[en_charac_name_length+1];

	u_char						slot;
};

// en_gm_edit_item_edit_skintear
struct MSG_GM_EDIT_EDIT_SKINTEAR : public MSG_GM_EDIT_ITEM
{
	char							szCharacName[en_max_lil+1];

	u_char							ucWhere;	// 0 Inven, 1 Depot
	u_char							slot;
	__inven_uselimit_info_v1		info;		// Protect �κи� �����Ѵ�.
};


//#define MSG_GM_EVENT_NUM					(111)		// �̺�Ʈ : ���� + ���� + ���� ��� + ���� ���� + ���� ���� ��û.
struct MSG_GM_EVENT 
{
	_gmsHeader			header;
	u_char				ucMode;
};

enum {
	en_gm_event_instant_buff=0,					// ���� �̺�Ʈ : �浵/�鵵/��� �� �����ؼ� ��û����.
	en_gm_event_request_reserved_events=10,		// ���� ����� �̺�Ʈ ������ ��û�Ѵ�.
	en_gm_event_reserved_events_info,			// ����� �̺�Ʈ���� �˷��ش�.
	en_gm_event_reserve_event,					// �����ϱ�.
	en_gm_event_cancel_event,					// ����ϱ�.

};

//en_gm_event_instant_buff
struct MSG_GM_EVENT_INSTANT_BUFF : public MSG_GM_EVENT
{
	u_char			ucTarget;		// 0 - all, 1 - white side, 2 - black side

	short			buffIndex;		// ���� ��ų�� �ε���
	char			buffStep;		// ������ ��.( 0 - 11 )

	u_char			group;			// �̰� �׳� Ȯ�ο����� �ִ´�.
	bool			TargetGS[23];	// true/false.
};


//#define MSG_GM_ORG_CTRL_NUM					(112)		// ������ ���� �޽���.
// 20081001 ����.
struct MSG_GM_ORG_CTRL
{
	_gmsHeader			header;					//
	u_char				ucMode;					//
};

// mode
enum
{
	en_org_ctrl_charac_reset=0,				// ĳ������ ������ ������ �ʱ�ȭ(Ż��)�Ѵ�.
	en_org_ctrl_entrust_captain=1,			// ����.
	en_org_ctrl_kick=2,						// Ż��
	en_org_ctrl_destroy_org=3,				// ������ ���ֱ�.

	en_org_ctrl_notice_org_destroyed=4,		// �������� ��ü�Ǿ���.
};


// en_org_ctrl_charac_reset=0,				// ĳ������ ������ ������ �ʱ�ȭ(Ż��)�Ѵ�.
struct MSG_GM_ORG_CHARAC_RESET : public MSG_GM_ORG_CTRL
{
	char			cCharacName[en_charac_name_length+1];
	int				iCuid;									// Ȯ�ο�

	int				iOrgIndex;								//
};

// en_org_ctrl_entrust_captain=1,			// ����.
struct MSG_GM_ORG_ENTRUST_CAPTAIN : public MSG_GM_ORG_CTRL
{
	char			cNewName[en_charac_name_length+1];		// 

	int				iOrgIndex;								//
};

// en_org_ctrl_kick=2,						// Ż��
struct MSG_GM_ORG_KICK : public MSG_GM_ORG_CTRL
{
	//
	char			cCharacName[en_charac_name_length+1];		// 

	int				iOrgIndex;								//
};

// en_org_ctrl_destroy_org=3,				// ������ ���ֱ�.
struct MSG_GM_ORG_DESTROY_ORG : public MSG_GM_ORG_CTRL
{
	//
	int				iOrgIndex;			// �������� ��ü�Ѵ�.
};

// en_org_ctrl_notice_org_destroyed=4,		// �������� ��ü�Ǿ���.
struct MSG_GM_ORG_NOTICE_ORG_DESTROYED : public MSG_GM_ORG_CTRL
{
	int				iOrgIndex;
	char			cOrgName[en_or_name_length+1];
};


#pragma pack( pop, enter_udp_packet )

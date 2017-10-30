#pragma once

// GS <-> CS
#pragma pack( push, enter_member )
#pragma pack(1)


//==============================================================
class _Member 
{
public :

	_CHARAC *		pCharac ;				// ĳ�� ������	
	u_short			uid ;
	u_short			usMoveCheckIndex ;		// �����̵� ��ȿ�� �˻� �ε���
	union {
		struct  _attribute {
			bool	bParty : 1 ;			// ��Ƽ ����
			bool	bCaptian : 1 ;			// ���� ����
			bool	bFriend : 1 ;
			u_char	ucAbsent : 2;			// ����(0) �ڸ����(1)
			bool	bTemp5 : 1 ;
			bool	bTemp6 : 1 ;
			bool	bIsMoveState : 1 ;
		} attribute ;
		char		cCheck ;
	};
public :
	_Member()
	{
		reset() ;		
	}
	~_Member()
	{
	}


	void reset()
	{
		pCharac = NULL ;				// ĳ�� ������	
		uid = 0 ;
		usMoveCheckIndex = 0 ;		// �����̵� ��ȿ�� �˻� �ε���
		cCheck = 0 ;		
	}
};




//==============================================================
// Party message
#define SMSG_NO_CREATE_PARTY_REQ_GS_CS		(181)
#define SMSG_NO_CREATE_PARTY_RES_CS_GS		(182)
#define SMSG_NO_DELETE_PARTY_REQ_GS_CS		(183)
#define SMSG_NO_DELETE_PARTY_RES_CS_GS		(184)
#define SMSG_NO_JOIN_PARTY_REQ_GS_CS		(185)
#define SMSG_NO_JOIN_PARTY_RES_CS_GS		(186)
#define SMSG_NO_KICK_PARTY_REQ_GS_CS		(187)
#define SMSG_NO_KICK_PARTY_RES_CS_GS		(188)
#define SMSG_NO_MOVE_PARTY_REQ_GS_CS		(189)
#define SMSG_NO_MOVE_PARTY_RES_CS_GS		(190)
#define SMSG_NO_UPDATE_LEVEL_REQ_GS_CS		(191)
#define SMSG_NO_UPDATE_LEVEL_RES_CS_GS		(192)
#define SMSG_NO_PARTY_INFO_REQ_GS_CS		(193)
#define SMSG_NO_PARTY_INFO_RES_CS_GS		(194)
#define SMSG_NO_ENTRUST_REQ_GS_CS			(195)
#define SMSG_NO_ENTRUST_RES_CS_GS			(196)
#define SMSG_NO_CHANGE_ITEM_PROP			(197)		// ������ �й� ��� ����
#define SMSG_NO_CHANGE_EXP_PROP				(198)		// ����ġ �й� ��� ����
#define SMSG_NO_MOVE_COMPLETE				(199)
#define SMSG_NO_UPDATE_STATE				(200)		// ���� ���� ��Ŷ : 0 - ����, 1 - �ڸ����.
#define CMSG_NO_MASTER_AND_PUPIL			(201)	// ���� ���� ��Ŷ.


//#define SMSG_NO_CREATE_PARTY_REQ_GS_CS
struct SMSG_CREATE_PARTY_REQ_GS_CS {
	u_short		usLength ;
	u_char		cMessage ;

	char		cServerNum ;
	u_short		uid1 ;			// ��Ƽ��	
	char		cCharacName1[13] ;
	short		sInnerLevel1 ;
	u_short		uid2 ;			// ��Ƽ��	
	char		cCharacName2[13] ;
	short		sInnerLevel2 ;
};

// #define SMSG_NO_CREATE_PARTY_RES_CS_GS
struct SMSG_CREATE_PARTY_RES_CS_GS {	// ���� ��û�� �������� ����
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;		// 0 : ��Ƽ ���� ����, !0 : ��Ƽ �ε���	
	short		sMoonpaProp ;		// 0���� 1�鵵 -1 �浵

	u_short		uid1 ;	
	u_short		uid2 ;	
};

// #define SMSG_NO_DELETE_PARTY_REQ_GS_CS	
struct SMSG_DELETE_PARTY_REQ_GS_CS {	// ��Ƽ ����(�ػ�) ��û
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;	
};
// #define SMSG_NO_DELETE_PARTY_RES_CS_GS	
struct SMSG_DELETE_PARTY_RES_CS_GS {	// ���� : ��Ƽ ���Ե� ��� ������ ����, ���� : ��û�� �������� ����
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	char		cResult ;			// 0: ����, 1: ����
};

// #define SMSG_NO_JOIN_PARTY_REQ_GS_CS
struct SMSG_JOIN_PARTY_REQ_GS_CS {
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	char		cServerNum ;
	u_short		uid ;
	char		cCharacName[13] ;
	short		sInnerLevel ;
};

// #define SMSG_NO_JOIN_PARTY_RES_CS_GS
struct SMSG_JOIN_PARTY_RES_CS_GS {	// ���� : ��Ƽ ���Ե� ��� ������ ����, ���� : ��û�� �������� ����
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;	
	short		sMoonpaProp ;		// 0���� 1�鵵 -1 �浵

	char		cServerNum ;
	u_short		uid ;
	char		cCharacName[13] ;
	short		sInnerLevel ;
	char		cSlotNo	;			// -1 ����, 0 < ����
};

// #define SMSG_NO_KICK_PARTY_REQ_GS_CS		
struct SMSG_KICK_PARTY_REQ_GS_CS {	
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	u_short		uid ;
	char		cSlotNo ;
	char		cMode ;			// 0: ����, 1: ����.. 2. ���� ����.. �Ǵ� �α׾ƿ�..
};
// #define SMSG_NO_KICK_PARTY_RES_CS_GS		
struct SMSG_KICK_PARTY_RES_CS_GS {	// ���� : ��Ƽ ���Ե� ��� ������ ����, ���� : ��û�� �������� ����	
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	u_short		uid ;		
	char		cSlotNo ;
	char		cMode ;			// 0: ����, 1: ����.. 2. ���� ����.. �Ǵ� �α׾ƿ�..
	char		cResult	;		//	-1:����, 0:����, 1:��Ƽ�� �̴޵����� ���� �ػ�, 2:��Ƽ�� ����
	char		cCapNo ;		// �ڵ� ���ӵ� ���.. ���� ���ӵ� ��Ƽ���� Slot��ȣ..
};

// #define SMSG_NO_MOVE_PARTY_REQ_GS_CS
struct SMSG_MOVE_PARTY_REQ_GS_CS {
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;	
	char		cServerNum ;
	char		cSlotNo ;
	u_short		uid ;	
};

// #define SMSG_NO_MOVE_PARTY_RES_CS_GS
struct SMSG_MOVE_PARTY_RES_CS_GS {	// ���� : ��Ƽ ���Ե� ��� ������ ����, ���� : ��û�� �������� ����
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	char		cServerNum ;	// 0 : Move ����
	u_short		uid ;	
	char		cSlotNo ;
	char		cCharacName[13] ;
};

// #define SMSG_NO_UPDATE_LEVEL_REQ_GS_CS	
struct SMSG_UPDATE_LEVEL_REQ_GS_CS {	// �������� ������Ʈ..
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	short		sInnerLevel ;
	u_short		uid ;	
	char		cSlotNo ;
};

// #define SMSG_NO_UPDATE_LEVEL_RES_CS_GS
struct SMSG_UPDATE_LEVEL_RES_CS_GS {		// SMSG_UPDATE_LEVEL_REQ_GS_CS �� ���� ����ü��
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	short		sInnerLevel ;	// 0 : level  update  ����
	u_short		uid ;	
	char		cSlotNo ;
};

// #define SMSG_NO_PARTY_INFO_REQ_GS_CS
struct SMSG_PARTY_INFO_REQ_GS_CS {
	u_short		usLength ;
	u_char		cMessage ;

	bool		bCreateAble ;	

	short		sPartyIndex ;
};

// #define SMSG_NO_PARTY_INFO_RES_CS_GS
struct SMSG_PARTY_INFO_RES_CS_GS {
	u_short		usLength ;
	u_char		cMessage ;

	bool		bCreateAble ;	

	short		sPartyIndex ;

	struct IN_MEMBER {	
		u_short	uid ;	
		char	cCharacName[13] ;
		char	cServerNum ;
		short	sInnerLevel ;
		char	cMoonpa ;
		char	cClass ;
#ifdef _PD_PARTY_INFO_CLASSGRADE_
		char	cClassGrade;
#endif
		int		iHonor ;
		

		union {
			struct  _attribute {
				bool	bParty : 1 ;
				bool	bCaptian : 1 ;	
				bool	bFriend : 1 ;
				u_char	ucAbsent : 2;			// ����(0) �ڸ����(1)
				bool	bTemp5 : 1 ;
				bool	bTemp6 : 1 ;
				bool	bIsMoveState : 1 ;
			} attribute ;
			char		cCheck ;
		} ;
	}  member[MAX_PARTY_MEMBER] ;	

};

//#define SMSG_NO_ENTRUST_REQ_GS_CS
struct SMSG_ENTRUST_REQ_GS_CS {
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	u_short		uid ;			//	0 : �ڵ� ����	
	char		cSlotNo ;
};
//#define SMSG_NO_ENTRUST_RES_CS_GS
struct SMSG_ENTRUST_RES_CS_GS {		// ���� : ��Ƽ ���Ե� ��� ������ ����, ���� : ��û�� �������� ����
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	u_short		uid ;			// ���ӹ����� uid, 0 : ����	
	char		cSlotNo ;

};

//#define SMSG_NO_CHANGE_ITEM_PROP			17
//#define SMSG_NO_CHANGE_EXP_PROP				16		// ����ġ ���� ��� ����
// Item, Exp �й� ��� �������� ����Ѵ�.
struct SMSG_CHANGE_PROP {
	u_short		usLength ;
	u_char		cMessage ;
	short		sPartyIndex ;
	char		cProperty ;
};

//#define SMSG_NO_MOVE_COMPLETE					17
struct SMSG_MOVE_COMPLETE {
	u_short		usLength ;
	u_char		cMessage ;
	short		sPartyIndex ;
	u_short		uid ;
	char		cSlotNo ;
};

//SMSG_NO_UPDATE_STATE
struct SMSG_UPDATE_STATE
{
	u_short		usLength;
	u_char		cMessage;

	u_char		ucMode;		// 0 - �������, 1 - �ڸ����.

	short		sPartyIndex;
	u_char		slot;
	u_short		usValue;
};



// 210~225 : �� �������� �� ��ų ��.
#define SMSG_NO_OR_SOME_REQ_GS_DS			210
#define SMSG_NO_OR_INFO_RES_DS_GS			211
#define SMSG_NO_OR_CREATE_REQ_GS_DS			212
#define SMSG_NO_OR_CTRL						(213)			// 20080630 : OR_CTRL �� ����.
#define SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS 214
#define SMSG_NO_OR_RESULT_DS_GS				215
#define SMSG_NO_OR_NOTICE_EDIT_GS_DS_GS		216
//#define SMSG_NO_ALLY_BASE					217		// ���� �⺻ ��Ŷ. �Ʒ��� ����.

//#define SMSG_NO_OR_SOME_REQ_GS_DS
struct SMSG_OR_SOME_REQ_GS_DS
{
	u_short		usLength ;
	u_char		cMessage ;

	int			iIndex ;
	char		cCode ;		
	enum
	{
		_OR_SOME_REQ_INFO_,
		_OR_SOME_REQ_DELETE,
	} ;
};

//#define SMSG_NO_OR_INFO_RES_DS_GS
struct SMSG_OR_INFO_RES_DS_GS
{
	u_short		usLength ;
	u_char		cMessage ;

	_Organization	_organization ;
	char		cCode ;
	enum{
		_INFO_REQ_,
		_CREATE_REQ_,
		_CREATE_FAIL_USING_NAME_,
		_CREATE_FAIL_ETC_,
	} ;
};

//#define SMSG_NO_OR_CREATE_REQ_GS_DS
struct SMSG_OR_CREATE_REQ_GS_DS
{
	u_short		usLength ;
	u_char		cMessage ;

	_Organization	_organization ;
};

//SMSG_NO_OR_BASE_INFO_UPDATE
struct SMSG_OR_BASE_INFO_UPDATE
{
	u_short		usLength ;
	u_char		cMessage ;

	_OrganizationBase _organizationBase ;
};

//#define SMSG_NO_OR_UNIT_CONTROL_REQ_GS_DS_GS
struct SMSG_OR_UNIT_CONTROL_REQ_GS_DS_GS
{
	u_short		usLength ;
	u_char		cMessage ;

	int			iIndex ;
	_Unit		_unit ;
	char		cCode ;
	enum{
		_UNIT_CONTROL_CREATE_,
		_UNIT_CONTROL_DELETE_,
		_UNIT_CONTROL_KICK_,
		_UNIT_CONTROL_UPDATE_,
		_UNIT_CONTROL_ENTRUST_,
	} ;

	_OutTime	outTime ;

};


//#define SMSG_NO_OR_RESULT_DS_GS
struct SMSG_OR_RESULT_DS_GS
{
	u_short		usLength ;
	u_char		cMessage ;
		
	int			iIndex ;
	int			iCode ;
	enum{
		_OR_RESULT_DELETE_,
	} ;
};



//#define SMSG_NO_OR_NOTICE_EDIT_GS_DS_GS		216
struct SMSG_OR_NOTICE_EDIT 
{
	u_short		usLength ;
	u_char		cMessage ;

	int			iIndex ;
	char		cNotice[_OR_NOTICE_SIZE] ;	/*#define _OR_NOTICE_SIZE		300*/
};



#ifdef _PD_ALLY_ORG_STEP_1_

#define SMSG_NO_ALLY_BASE					(217)		// ���� �⺻ ��Ŷ.
struct SMSG_ALLY_BASE 
{
	u_short			usLength;
	u_char			ucMessage;

	u_char			ucMode;					// ��忡 ���� �����Ͽ� ����� ��.

	SMSG_ALLY_BASE( u_short len, u_char mode )
		: usLength(len), ucMessage(SMSG_NO_ALLY_BASE), ucMode(mode)
	{
	}
};

// SMSG_ALLY_BASE::ucMode
enum EN_DS_ALLY_PACKET_MODE
{
	en_ds_ally_request_create=0,		// 0 ���� ��û
	en_ds_ally_create_success,			// 1 ���� ���� : ��û�� �������� ����.
	en_ds_ally_create_fail,				// 2 ���� ���� : ��Ŷ�� ���� ������ ���ִ�.
	en_ds_ally_create_notice,			// 3 ��� ������ �˸���. GS���� ��� ����� �˸��� ������ ���ȴ�.

	en_ds_ally_request_info=10,			// 10 ������û
	en_ds_ally_info,					// 11 �� ���� ��û�� ����.
	en_ds_ally_no_info,					// 12 �׷� ���� ����. 
	en_ds_ally_request_all_ally,		// 13 ��� �������� ������ ��û�Ѵ�. ���۽ÿ�.
	en_ds_ally_answer_info,				// 14 13���� ����.

	en_ds_ally_request_add_org=20,		// 20 ���� �߰� ��û
	en_ds_ally_add_success,				// 21 ���� �߰� ��û ����
	en_ds_ally_add_fail,				// 22 ���� �߰� ��û ����.
	en_ds_ally_add_notice,				// 23

	en_ds_ally_request_leave=30,		// 30 ���� Ż�� ��û
	en_ds_ally_leave_success,			// 31 ���� Ż�� ����
	en_ds_ally_leave_fail,				// 32 ���� Ż�� ���� - ������... ��������.

	en_ds_ally_request_breakup=40,		//  �ػ� ��û
	en_ds_ally_breakup_success,			// �ػ� ��û ����
	en_ds_ally_breakup_fail,			// �ػ� ����.

	en_ds_ally_request_banish=50,		// �߹� ��û. : ȸ�ָ��� �� �� �ִ�.
	en_ds_ally_banish_success,			// ����
	en_ds_ally_banish_fail,				// ����, ������ ������ �̻��ϸ�.. 
};

//en_ds_ally_request_create=0,		// 0 ���� ��û
struct SMSG_ALLY_REQUEST_CREATE : public SMSG_ALLY_BASE
{
	int				iOrg1;			//
	int				iOrg2;			//

	char			cName1[13];		// ���������� �̸�.
	char			cName2[13];		// ���������� �̸�.

	SMSG_ALLY_REQUEST_CREATE()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_CREATE), en_ds_ally_request_create )
	{
	}
};


//en_ds_ally_create_success,			// 1 ���� ���� : ������ ���Եȴ�.
struct SMSG_ALLY_CREATE_SUCCESS : public SMSG_ALLY_BASE
{
	int				iAllyIndex;		// ������ �������� ����

	int				iOrg1;
	int				iOrg2;

	char			cOrgName1[en_or_name_length+1];	// ������ �̸�.
	char			cOrgName2[en_or_name_length+1];	// ������ �̸�.

	char			cMasterName[13];		// ���� ȸ���� �̸�.

	short			sProperty;		// -1/1 �浵/�鵵

	char			cName1[13];		// ���������� �̸�.
	char			cName2[13];		// ���������� �̸�.

	SMSG_ALLY_CREATE_SUCCESS()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_CREATE_SUCCESS), en_ds_ally_create_success )
	{
	}
};

// en_ds_ally_create_fail,				// 2 ���� ���� : ��Ŷ�� ���� ������ ���ִ�.
struct SMSG_ALLY_CREATE_FAIL : public SMSG_ALLY_BASE
{
	int				iOrg1;
	int				iOrg2;

	char			cName1[13];		// ���������� �̸�.
	char			cName2[13];		// ���������� �̸�.

	SMSG_ALLY_CREATE_FAIL()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_CREATE_FAIL), en_ds_ally_create_fail )
	{
	}
};

// en_ds_ally_create_notice
struct SMSG_ALLY_CREATE_NOTICE : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	int				iOrg1;
	int				iOrg2;

	short			sProperty;

	char			cOrgName1[en_or_name_length+1];
	char			cOrgName2[en_or_name_length+1];

	char			cMasterName[13];

	SMSG_ALLY_CREATE_NOTICE()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_CREATE_NOTICE), en_ds_ally_create_notice )
	{
	}
};

//en_ds_ally_request_info,			//  ������û
struct SMSG_ALLY_REQUEST_INFO : public SMSG_ALLY_BASE
{
	char			cName[13];		// 

	int				iAllyIndex;

	SMSG_ALLY_REQUEST_INFO()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_INFO), en_ds_ally_request_info )
	{
	}
};


struct SMSG_ALLY_INFO : public SMSG_ALLY_BASE
{
	char			cName[13];		// 

	int				iAllyIndex;
	short			sProperty;
	// ���� 5���� ������ ������ ����.
	__ally_client_info		org[_NDD_MAX_ORG_IN_ALLY_];

	char			cMasterName[13];		// ���� ȸ���� �̸�.

	SMSG_ALLY_INFO()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_INFO), en_ds_ally_info )
	{
	}
};

//en_ds_ally_no_info,					// 5 �׷� ���� ����. 
struct SMSG_ALLY_NO_INFO : public SMSG_ALLY_BASE
{
	char			cName[13];		// 

	int				iAllyIndex;
	u_char			ucError;		// ���� �ڵ�.


	SMSG_ALLY_NO_INFO()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_NO_INFO), en_ds_ally_no_info )
	{
	}
};

//en_ds_ally_request_all_ally
struct SMSG_ALLY_REQUEST_ALL_ALLY : public SMSG_ALLY_BASE
{
	// ���� �ʿ� ����.
	SMSG_ALLY_REQUEST_ALL_ALLY()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_ALL_ALLY), en_ds_ally_request_all_ally )
	{
	}
};

// en_ds_ally_answer_info
struct SMSG_ALLY_ANSWER_INFO : public SMSG_ALLY_BASE
{
	u_char			ucContinueInfo;	// 0 - End of Infor. 1 - Continue..

	int				iAllyIndex;
	short			sProperty;
	// ���� 5���� ������ ������ ����.
	__ally_client_info		org[_NDD_MAX_ORG_IN_ALLY_];

	char			cMasterName[13];		// ���� ȸ���� �̸�.

	SMSG_ALLY_ANSWER_INFO()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_ANSWER_INFO), en_ds_ally_answer_info )
	{
		ucContinueInfo = 0;		// �⺻���� �־���� ���� �ڵ尡 �״�� ������ ���̴�.
	}
};


// en_ds_ally_request_add_org,			// 20 ���� �߰� ��û
struct SMSG_ALLY_REQUEST_ADD_ORG : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	int				iOrgAdd;		// �߰��Ǵ� ������ �ε���.

	char			cName[13];		// ���������� �̸�.
	char			cNameAdd[13];		// ���������� �̸�.

	SMSG_ALLY_REQUEST_ADD_ORG()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_ADD_ORG), en_ds_ally_request_add_org )
	{
	}
};


//en_ds_ally_add_success,				// 21 ���� �߰� ��û ����
struct SMSG_ALLY_ADD_SUCCESS : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	u_char				ucSlot;			// 0 ~ 4. �������� ���� ������ �����ϰ� �����ϱ� ���� ����Ѵ�.
	__ally_client_info	info;

	char			cName[13];		// ���������� �̸�.
	char			cNameAdd[13];		// ���������� �̸�.

	SMSG_ALLY_ADD_SUCCESS()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_ADD_SUCCESS), en_ds_ally_add_success )
	{
	}
};

//en_ds_ally_add_fail,				// 22 ���� �߰� ��û ����.
struct SMSG_ALLY_ADD_FAIL : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	int				iOrgAdd;

	u_char			ucError;

	char			cName[13];		// ���������� �̸�.
	char			cNameAdd[13];		// ���������� �̸�.

	SMSG_ALLY_ADD_FAIL()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_ADD_FAIL), en_ds_ally_add_fail )
	{
	}
};

//en_ds_ally_add_notice,				// 23
struct SMSG_ALLY_ADD_NOTICE : public SMSG_ALLY_BASE
{
	int				iAllyIndex;		// ���� �ε���.

	int				iOrg;			// �߰��� �������� �ε���
};

//en_ds_ally_request_leave,			// ���� Ż�� ��û
struct SMSG_ALLY_REQUEST_LEAVE : public SMSG_ALLY_BASE
{
	int				iAllyIndex;
	int				iOrgLeave;

	char			cName[13];

	SMSG_ALLY_REQUEST_LEAVE()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_LEAVE), en_ds_ally_request_leave )
	{
	}
};
//en_ds_ally_leave_success,			// ���� Ż�� ����
struct SMSG_ALLY_LEAVE_SUCCESS : public SMSG_ALLY_BASE
{
	int				iAllyIndex;
	int				iOrgLeave;

	char			cName[13];

	SMSG_ALLY_LEAVE_SUCCESS()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_LEAVE_SUCCESS), en_ds_ally_leave_success )
	{
	}
};
//en_ds_ally_leave_fail,				// ���� Ż�� ���� - ������... ��������.
struct SMSG_ALLY_LEAVE_FAIL : public SMSG_ALLY_BASE
{
	int				iAllyIndex;
	int				iOrgLeave;

	u_char			ucError;

	char			cName[13];

	SMSG_ALLY_LEAVE_FAIL()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_LEAVE_FAIL), en_ds_ally_leave_fail )
	{
	}
};

//en_ds_ally_request_breakup,			//  �ػ� ��û
struct SMSG_ALLY_REQUEST_BREAKUP : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	u_char			ucBreakupReason;	// 0 - request

	char			cName[13];

	SMSG_ALLY_REQUEST_BREAKUP()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_BREAKUP), en_ds_ally_request_breakup )
	{
	}
};
//en_ds_ally_breakup_success,			// �ػ� ��û ����
struct SMSG_ALLY_BREAKUP_SUCCESS : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	u_char			ucBreakupReason;	// 0 - request, 1 - leave

	char			cName[13];

	SMSG_ALLY_BREAKUP_SUCCESS()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_BREAKUP_SUCCESS), en_ds_ally_breakup_success )
	{
	}
};
//en_ds_ally_breakup_fail,			// �ػ� ����.
struct SMSG_ALLY_BREAKUP_FAIL : public SMSG_ALLY_BASE
{
	int				iAllyIndex;

	u_char			ucError;		//

	char			cName[13];

	SMSG_ALLY_BREAKUP_FAIL()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_BREAKUP_FAIL), en_ds_ally_breakup_fail )
	{
	}
};

//en_ds_ally_request_banish=50,		// �߹� ��û. : ȸ�ָ��� �� �� �ִ�.
struct SMSG_ALLY_REQUEST_BANISH : public SMSG_ALLY_BASE
{
	int				iAllyIndex;
	int				iOrgIndex;

	char			cName[13];

	SMSG_ALLY_REQUEST_BANISH()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_REQUEST_BANISH), en_ds_ally_request_banish )
	{
	}
};


//en_ds_ally_banish_success,			// ����
struct SMSG_ALLY_BANISH_SUCCESS : public SMSG_ALLY_BASE
{
	int				iAllyIndex;
	int				iOrgIndex;

	char			cName[13];

	SMSG_ALLY_BANISH_SUCCESS()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_BANISH_SUCCESS), en_ds_ally_banish_success )
	{
	}
};

//en_ds_ally_banish_fail,				// ����, ������ ������ �̻��ϸ�.. v
struct SMSG_ALLY_BANISH_FAIL : public SMSG_ALLY_BASE
{
	int				iAllyIndex;
	int				iOrgIndex;

	char			cName[13];

	SMSG_ALLY_BANISH_FAIL()
		: SMSG_ALLY_BASE( sizeof(SMSG_ALLY_BANISH_FAIL), en_ds_ally_banish_fail )
	{
	}
};

#endif	// ally

#define SMSG_NO_PARTY_CHAT					240		// ��Ƽä��
#define SMSG_NO_OR_CHAT						241		// ���� ä��

//#define SMSG_NO_PARTY_CHAT
struct SMSG_PARTY_CHAT {
	u_short		usLength ;
	u_char		cMessage ;

	short		sPartyIndex ;
	char		cName[13] ;
	char		cChatMsg[81] ;
};

struct SMSG_OR_CHAT 
{
	u_short		usLength ;
	u_char		cMessage ;

#ifdef _PD_ALLY_ORG_STEP_1_
	int			iAllyIndex;
	int			iOrgIndex;
#else
	int			index ;
#endif
	char		cName[13] ;
	char		cChatMsg[81] ;
};

#ifdef _PD_CASTLE_STEP_1_
//////////////////////////////////////////////////////////////////////////
// ����� ���� ��Ŷ.
//////////////////////////////////////////////////////////////////////////
#include "../Cs/CastleStructure.h"

#define CMSG_NO_COMBAT_PACKET					(218)		// ���� �⺻ ��Ŷ.

struct CMSG_COMBAT_PACKET
{
	u_short				usLength;
	u_char				ucMessage;

	u_char				ucMode;			// �ٷ� �Ʒ��� EN_DS_COMBAT

	CMSG_COMBAT_PACKET( u_short size, u_char mode )
		: usLength(size), ucMessage(CMSG_NO_COMBAT_PACKET), ucMode(mode)
	{
	}
};

enum EN_DS_COMBAT
{
	en_dscb_req_manor_information=0,	// 0 ������ ���� ��û.
	en_dscb_manor_information,			// ������ ���� : �޾��� ��, ��ġ�� �ʴ´ٸ�, �׻� �߰��ϸ� �ȴ�. 

	en_dscb_req_change_time,			// ����� �ð� ���� ��û. : ���� ��Ŷ ����.
	en_dscb_time_changed,				// ����� �������� ����Ǿ���. �뺸 ��Ŷ.

	en_dscb_challenge_insert,			// ���� �߰� : �뺸
	en_dscb_challenge_delete,			// ���� ���� : �뺸

	en_dscb_challenge_reset,			// ���� ������ ��� �����Ѵ�.

	en_dscb_owner_info_changed,			// ��� ���� ����.
	
	en_dscb_combat_start,				// ����� ����
	en_dscb_combat_end,					// ����� ��.

    en_dscb_record_info_req = 50,		//GS����  ���� ������ ��û
	en_dscb_record_info_res,			// ��� ���� ����
	en_dscb_record_update,				// ������� ������ ������� ������ �ܵ� ������Ʈ
	en_dscb_record_reset,				// ������� ���� �Ǹ� 


	en_dscb_error=100,					// ���� ����.
};

// en_dscb_req_manor_information
struct CMSG_CB_REQ_MANOR_INFORMATION : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;		// 0 - all info. etc - a info.

	_sND_TimeUnit			DefaultSchedule[3];

	/*
	��û�� ��, �⺻ ������ ������ ä���� ��û�ϰ�,
	DS ���� �̸� �⺻���� �������� �ʱ�ȭ�ؼ� �����ش�.
	*/

	CMSG_CB_REQ_MANOR_INFORMATION()
		: CMSG_COMBAT_PACKET(sizeof(CMSG_CB_REQ_MANOR_INFORMATION), en_dscb_req_manor_information )
	{
	}
};

// en_dscb_manor_information
struct CMSG_CB_MANOR_INFORMATION : public CMSG_COMBAT_PACKET
{
	// ���� ����� ������ ���� �ϹǷ�, �����ؼ� ������, ���������� 1�� ������.
	u_char					ucIsEnd;			// 0 : continue info. 1 : end info.

	u_char					ucMapID;			// �� �ε��� : 0 �̸�, ��� ������ �ϳ��� ���ٴ� �ǹ̴�.

	_sCastleInfo			castleInfo;			// ��� ����. ä��� �Լ��� ������ �Ѵ�.

	u_char					ucCount;			// ������ ����.
	__cb_challenge_info		challengeList[max_count_of_challenge_list*2];		//

	CMSG_CB_MANOR_INFORMATION()
		: CMSG_COMBAT_PACKET(sizeof(CMSG_CB_MANOR_INFORMATION), en_dscb_manor_information )
	{
	}

	inline void set_length()
	{
		usLength = sizeof(CMSG_CB_MANOR_INFORMATION) - ( sizeof(__cb_challenge_info) * (max_count_of_challenge_list*2 - ucCount) );
	}
};

// en_dscb_req_change_time,			// ����� �ð� ���� ��û.
struct CMSG_CB_REQ_CHANGE_TIME : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;

	_sND_TimeUnit			Schedule[3];		// 

	CMSG_CB_REQ_CHANGE_TIME()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_REQ_CHANGE_TIME), en_dscb_req_change_time )
	{
	}
};

// en_dscb_time_changed,			����� �������� ����Ǿ���. �뺸 ��Ŷ.
struct CMSG_CB_TIME_CHANGED : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;			// �ش� ��.

	_sND_TimeUnit			Schedule[3];		// ������

	CMSG_CB_TIME_CHANGED()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_TIME_CHANGED), en_dscb_time_changed )
	{
	}
};

// en_dscb_challenge_insert,		���� �߰� : �뺸 : 
struct CMSG_CB_CHALLENGE_INSERT : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;

	__cb_challenge_info		info;		// ������ ����.

	CMSG_CB_CHALLENGE_INSERT()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_CHALLENGE_INSERT), en_dscb_challenge_insert )
	{
	}
};

// en_dscb_challenge_delete,		���� ���� : �뺸
struct CMSG_CB_CHALLENGE_DELETE : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;

	__cb_challenge_info		info;

	CMSG_CB_CHALLENGE_DELETE()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_CHALLENGE_DELETE), en_dscb_challenge_delete )
	{
	}
};


//en_dscb_challenge_reset,			// ���� ������ ��� �����Ѵ�.
struct CMSG_CB_CHALLENGE_RESET : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;

	CMSG_CB_CHALLENGE_RESET()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_CHALLENGE_RESET), en_dscb_challenge_reset )
	{
	}
};


// en_dscb_owner_info_changed : gs->ds, ds->gs
// ������, ChangeOwner �� ����, ������ �����ϰ�
// ���� ����� �ʱ�ȭ�Ѵ�.
struct CMSG_CB_OWNER_INFO_CHANGED : public CMSG_COMBAT_PACKET
{
	u_char					ucChangeOwner;		// 0 - not changed, 1 - changed

	u_char					ucMapID;

	_sCastleInfo			castleInfo;

	CMSG_CB_OWNER_INFO_CHANGED()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_OWNER_INFO_CHANGED), en_dscb_owner_info_changed )
	{
	}
};


// en_dscb_combat_start,				// ����� ����
struct CMSG_CB_COMBAT_START : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;

	CMSG_CB_COMBAT_START()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_COMBAT_START), en_dscb_combat_start )
	{
	}
};


// en_dscb_combat_end,					// ����� ��.
struct CMSG_CB_COMBAT_END : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;

	CMSG_CB_COMBAT_END()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_COMBAT_END), en_dscb_combat_end )
	{
	}
};


// en_dscb_error
struct CMSG_CB_ERROR : public CMSG_COMBAT_PACKET
{
	u_char					ucPrevMode;			// 

	u_char					ucErrorCode;		// 


	CMSG_CB_ERROR()
		: CMSG_COMBAT_PACKET( sizeof(CMSG_CB_ERROR), en_dscb_error )
	{
	}
};


#endif	// _PD_CASTLE_STEP_1_


#ifdef _PD_CASTLE_STEP_2_
//=====================================
//
//��� ���� �ý��� ��Ŷ
//
//=====================================
//1.��� ���� ���� ���� ��û/����
//2.��� ���� �� ���� ���� ��û/����
//3.��� ���� ���� ���� ��û/����
//=====================================
#define CMSG_NO_MANOR_TAX	219

struct CMSG_MANOR_TAX
{
	u_short		usLength;
	u_char		ucMessage;

	u_char		ucMode;		//en_manor_tax_pkt_type

	CMSG_MANOR_TAX(u_short length, u_char mode)
		:usLength(length), ucMessage(CMSG_NO_MANOR_TAX), ucMode(mode)
	{}

};

enum EN_MANOR_TAX_PKT_GS_DS
{
	en_manor_tax_info_req_ds_gs=0,		// DS ���� GS �� �׿��� ���� ���� ��û
	en_manor_tax_info_gs_ds,			// 10���� �� ��Ŷ�� ������ DB�� ����, GS�� ��� ���� �ٽ� �ѷ���.

	en_manor_tax_req_start_compute_gs_ds,	// ����� �ѽð� ���� ���� ���� ��û.
	en_manor_tax_req_total_gs_ds,			// �� 30�� ��, ��ü ������ ��û�Ѵ�.
	en_manor_tax_notice_total_tax_ds_gs,	// ��ü ������ �˷���.
	en_manor_tax_reset_common_tax,			// �ŵֵ��� ������ �ʱ�ȭ�Ѵ�.

	en_manor_tax_set_total_money=30,		// �̰� �ʿ��Ѱǰ�...
	en_manor_tax_req_take,					// Ŭ���̾�Ʈ���� ��û�Դ�. DS �� ��û����.
	en_manor_tax_answer_take,				// ������ ����.


	en_manor_tax_challenge_gs_ds = 40,		//���� ����
};

// 1�ð� ���� ���� ��û
//en_manor_tax_info_req_ds_gs
struct CMSG_MANOR_TAX_INFO_REQ_DS_GS : public CMSG_MANOR_TAX
{
	//u_char		ucMapID;		// �� ��ȣ
	//u_char		ucResult;		// 10�� �Ŀ� ��� �� ���� ���� �˱� ���ؼ�

	CMSG_MANOR_TAX_INFO_REQ_DS_GS()
		:CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_INFO_REQ_DS_GS), en_manor_tax_info_req_ds_gs)
	{}
};


// en_manor_tax_info_gs_ds
struct CMSG_MANOR_TAX_INFO_GS_DS : public CMSG_MANOR_TAX
{
	u_char		ucMapID;
	u_char		ucManorLevelRate;	// ����� �ִ� �� ����
	u_char		ucCommonLevelRate;	// ���� ����
	u_char		ucRegionOrGlobal;	// ����/���� ����
	u_int		uiTotalMoney;		// �� ���� ����
	u_char		ucResult;			// GS���� ��� ������ �� �Դ��� �˱� ���ؼ�

	CMSG_MANOR_TAX_INFO_GS_DS()
		:CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_INFO_GS_DS), en_manor_tax_info_gs_ds)
	{}
};

//	en_manor_tax_req_start_compute_gs_ds,		// ����� �ѽð� ���� ���� ���� ��û.
struct CMSG_MANOR_TAX_REQ_START_COMPUTE_GS_DS : public CMSG_MANOR_TAX
{
	u_char		ucMapID;			// ��/��� ���̵�.

	CMSG_MANOR_TAX_REQ_START_COMPUTE_GS_DS()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_REQ_START_COMPUTE_GS_DS), en_manor_tax_req_start_compute_gs_ds )
	{
	}
};


//	en_manor_tax_req_total_gs_ds,		// �� 30�� ��, ��ü ������ ��û�Ѵ�.
struct CMSG_MANOR_TAX_REQ_TOTAL_GS_DS : public CMSG_MANOR_TAX
{
	u_char		ucMapID;			// ��/¡�� ���̵�.

	CMSG_MANOR_TAX_REQ_TOTAL_GS_DS()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_REQ_TOTAL_GS_DS), en_manor_tax_req_total_gs_ds )
	{
	}
};

//	en_manor_tax_reset_common_tax
struct CMSG_MANOR_TAX_RESET_COMMON_TAX : public CMSG_MANOR_TAX
{
	CMSG_MANOR_TAX_RESET_COMMON_TAX()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_RESET_COMMON_TAX), en_manor_tax_reset_common_tax )
	{
	}
};


//	en_manor_tax_set_total_money=30,		// �̰� �ʿ��Ѱǰ�...
struct CMSG_MANOR_TAX_SET_TOTAL_MONEY : public CMSG_MANOR_TAX
{
	u_char		ucMapID;			// ��� ���̵�.

	u_int		uiTotalMoney;		// 
	u_int		uiPartMoney;		// 

	u_char		ucType;		// 0 - from tax, 1 - from challenge, 2 - Take.

	CMSG_MANOR_TAX_SET_TOTAL_MONEY()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_SET_TOTAL_MONEY), en_manor_tax_set_total_money )
	{
	}
};


//	en_manor_tax_req_take,					// Ŭ���̾�Ʈ���� ��û�Դ�. DS �� ��û����.
struct CMSG_MANOR_TAX_REQ_TAKE : public CMSG_MANOR_TAX
{
	char		cCharacName[en_charac_name_length+1];

	u_char		ucMapID;			// ��� ���̵�.

	u_int		uiRequestMoney;

	CMSG_MANOR_TAX_REQ_TAKE()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_REQ_TAKE), en_manor_tax_req_take )
	{
	}
};


//	en_manor_tax_answer_take,				// ������ ����.
struct CMSG_MANOR_TAX_ANSWER_TAKE : public  CMSG_MANOR_TAX
{
	char		cCharacName[en_charac_name_length+1];

	u_char		ucMapID;			//

	u_char		ucResult;			// ���. 
	u_int		uiTakeMoeny;			// ������ : ���������� ��.
	u_int		uiLeftPartMoney;

	/*	ucResult
	0 - Success
	1 - ���� ����.
	2 - ���ְ� �ƴϴ�.
	3 - ������ ����� ����.
	*/

	CMSG_MANOR_TAX_ANSWER_TAKE()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_ANSWER_TAKE), en_manor_tax_answer_take )
	{
	}
};

//en_manor_tax_challenge_gs_ds = 40,		//���� ����
struct CMSG_MANOR_TAX_CHALLENGE_GS_DS : public CMSG_MANOR_TAX
{
	u_char	ucMapID;
	u_int	uiChallengeTax;

	CMSG_MANOR_TAX_CHALLENGE_GS_DS()
		: CMSG_MANOR_TAX(sizeof(CMSG_MANOR_TAX_CHALLENGE_GS_DS), en_manor_tax_challenge_gs_ds)
	{
	}
};

#endif //_PD_CASTLE_STEP_2_

#ifdef _PD_CASTLE_STEP_2_

// en_dscb_record_info_req
struct CMSG_RECORD_INFORMATION_REQ : public CMSG_COMBAT_PACKET
{
	int		m_org_index;		// 0 - all info. etc - a info.

	CMSG_RECORD_INFORMATION_REQ()
		: CMSG_COMBAT_PACKET(sizeof(CMSG_RECORD_INFORMATION_REQ), en_dscb_record_info_req )
	{
	}
};

// en_dscb_record_info_res
struct CMSG_RECORD_INFORMATION_DS_GS : public CMSG_COMBAT_PACKET
{
	int						m_org_index;
	_sCombatRecord			m_combat_record[MAX_MANOR];
	short					m_combat_org_count;		// �� ������� �ο� ���� ���� 
	_sOrgRecord				m_org_record[MAX_ORG_COUNT];


	CMSG_RECORD_INFORMATION_DS_GS()
		: CMSG_COMBAT_PACKET(sizeof(CMSG_RECORD_INFORMATION_DS_GS), en_dscb_record_info_res )
	{
	}

	inline void set_length()
	{
		usLength = sizeof(CMSG_RECORD_INFORMATION_DS_GS) - ( sizeof(_sOrgRecord) * (MAX_ORG_COUNT - m_combat_org_count) );
	}
};

//en_dscb_record_update
struct CMSG_RECORD_UPDATE_GS_DS : public CMSG_COMBAT_PACKET
{
	int						m_org_index;
	_sCombatRecord			m_combat_record[MAX_MANOR];
	short					m_combat_org_count;		// �� ������� �ο� ���� ���� 
	_sOrgRecord				m_org_record[MAX_ORG_COUNT];
    
	CMSG_RECORD_UPDATE_GS_DS()
		: CMSG_COMBAT_PACKET(sizeof(CMSG_RECORD_UPDATE_GS_DS), en_dscb_record_update )
	{
	}
	inline void set_length()
	{
		usLength = sizeof(CMSG_RECORD_UPDATE_GS_DS) - ( sizeof(_sOrgRecord) * (MAX_ORG_COUNT - m_combat_org_count) );
	}
};
//en_dscb_record_reset
struct CMSG_RECORD_RESET : public CMSG_COMBAT_PACKET
{
	u_char					ucMapID;    

	CMSG_RECORD_RESET()
		: CMSG_COMBAT_PACKET(sizeof(CMSG_RECORD_RESET), en_dscb_record_reset )
	{
	}	
};

#endif	//_PD_CASTLE_STEP_2_



#ifdef _PD_MASTER_PUPIL_

//////////////////////////////////////////////////////////////////////////
//	Master & Pupil : GS <=> DS �� ��Ŷ.
//////////////////////////////////////////////////////////////////////////
struct CMSG_MASTER_AND_PUPIL
{
	u_short				usLength;
	u_char				cMessage;

	u_char				ucMP_Mode;		// EN_DG_MP_PACKET_MODE

	CMSG_MASTER_AND_PUPIL( u_short size, u_char mode )
		: usLength(size), cMessage(CMSG_NO_MASTER_AND_PUPIL), ucMP_Mode(mode)
	{
	}
};


enum EN_DG_MP_PACKET_MODE
{
	// 0 - 50 : ���� ���� ���� ���� ��Ŷ.
	en_dg_mp_mode_info_master=0,				// ���� ���� ����. ( req �� �����̴�. )
	en_dg_mp_mode_info_req=1,					// ���� ���� ��û.
	en_dg_mp_mode_no_info=2,					// ���� ���� ������ ���ٸ�, �� ��Ŷ���� �˸���.

	en_dg_mp_mode_add_point=3,					// ����Ʈ ���� ��û.
	en_dg_mp_mode_update_point=4,				// ����Ʈ ���� �˸� ��Ŷ.
	en_dg_mp_mode_add_kill_count=5,				// ų ī��Ʈ ���� ��û.
	en_dg_mp_mode_update_kill_count=6,			// ų ī��Ʈ ���� �˸� ��Ŷ.
	en_dg_mp_mode_update_rank=7,				// GS->DS->GS ��ũ ������Ʈ

	en_dg_mp_mode_create_mp_req=8,				// ���� ���� ���� ��û.
	en_dg_mp_mode_create_success=9,				// ���� ���� ���� ����.
	en_dg_mp_mode_create_fail=10,				// ���� ���� ���� ����.

	en_dg_mp_mode_notice_connection=11,			// ����/���� �˸� ��Ŷ.
	en_dg_mp_mode_change_master_nick_req=12,	// ��ȣ ���� �˸�.
	en_dg_mp_mode_update_master_nick=13,		// ��ȣ �����϶�.

	en_dg_mp_mode_update_pupil_position=14,		// ������ ������ ���ߴ�. (������׸� ����)

	en_dg_mp_mode_chat=20,						// ������ �׷� ä��.

	en_dg_mp_mode_info_error=50,				// ���� ���� ���� �޽���.


	// 51 - 80 : ���� ���� �α� ���� ��Ŷ.
	en_dg_mp_mode_gd_apply_for_pupil=51,
	en_dg_mp_mode_dg_apply_for_pupil=52,
	en_dg_mp_mode_apply_for_pupil_error=53,		// ��û�ڿ��� �����ش�. 
	en_dg_mp_mode_answer_yes=54,				// '��'�� �����ߴ�.
	en_dg_mp_mode_answer_apply_to_pupil_no=55,	// '�ƴϿ�'�� �������� ��.
	en_dg_mp_mode_complete_join_pupil=56,		// ��û�ڿ��� ����.
	en_dg_mp_mode_add_new_pupil=57,				// ���� �� �ٸ� ���ڿ��� ����.
	en_dg_mp_mode_master_simple_info_to_pupil=58,	// �����ڿ��� ������ ���� ������ �˷��ش�.

	en_dg_mp_mode_apply_error=80,				// ���� ���� ����.

	// 81 - 100 : �Ĺ� �� �ϻ�.
	en_dg_mp_mode_strike_pupil_req=81,			// ���� : �Ĺ� ��û.
	en_dg_mp_mode_strike_result=82,				// �Ĺ� ������ ���� �� �ٸ� ���ڿ���.
	en_dg_mp_mode_you_struck=83,				// �Ĺ� ĳ���Ϳ��� ������.
	
	en_dg_mp_mode_strike_error=90,				// �Ĺ� ���н� ���� ����.



	en_dg_mp_mode_leave_req=91,					// �ϻ� ��û.
	en_dg_mp_mode_you_left=92,					// �ϻ� ������ ��û ���ڿ��� ��.
	en_dg_mp_mode_left_pupil=93,				// ���� �� �ٸ� ���ڿ��� �ϻ��� �˸���.

	en_dg_mp_mode_left_error=100,				// �ϻ� ������ ����.

	// �޸� �߰�/���� ���� ��ȯ.
	en_dg_mp_mode_del_from_memory=101,			// �޸𸮿��� ����.

	// 151 ~ 170 : ���� ��û �ź� �� �Խ��� ����
	en_dg_mp_mode_accept_pupil_mode = 151,		// ���� ��û �ź� ����.
	en_dg_mp_mode_board_initial_req = 152,		// ó�� ��û�� ��.( �Խ����� �� �� )
	en_dg_mp_mode_board_initial_open = 153,		// ó�� ��û���� ����. ( 1 ������ ������ ���Եȴ�. )

	en_dg_mp_mode_board_req_page=154,			// �Խ��� ���� ��û. ������ ����.
	en_dg_mp_mode_board_page_info=155,			// �Խ��� ����.

	en_dg_mp_mode_board_error = 170,			// ����.

};



// 0 - 50 : ���� ���� ���� ���� ��Ŷ.
//	en_dg_mp_mode_info_master=0,				// ���� ���� ����. ( req �� �����̴�. )
struct CMSG_MP_INFO_MASTER : public CMSG_MASTER_AND_PUPIL
{
	char					cFromName[en_charac_name_length+1];

	int						iMasterIndex;			// 

	//!// �Ʒ��� ���� ������ �´�.
	u_int					uiTotalKillCount;
	u_int					uiMasterKillCount;
	u_int					uiMasterTotalPoint;
	u_int					uiMasterCurPoint;

	u_char					ucMasterRank;

	u_char					ucMasterSex;
	u_char					ucMasterInnerLevel;
	u_char					ucMasterMoonpa;
	u_char					ucMasterClass;
	u_char					ucMasterClassgrade;

	u_char					ucMasterConZone;
	u_char					ucMasterConnected;
	u_char					ucBonusSkillStep;

	u_char					ucApplyMasterEffect;

	char					cMasterName[en_charac_name_length+1];
	char					cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];

	_sCSPK_Pupil			Pupils[NDD_MP_MAX_PUPIL_COUNT];

	CMSG_MP_INFO_MASTER()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_INFO_MASTER), en_dg_mp_mode_info_master )
	{
	}

	inline void SetFromName( char * name )
	{
		CopyMemory( cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}

	inline void SetMasterName( char * name )
	{
		ZeroMemory( cMasterName, en_charac_name_length+1 );
		strncpy( cMasterName, name, en_charac_name_length );
	}

	inline void SetMasterNick( char * nick )
	{
		ZeroMemory( cMasterNick, NDD_MP_MASTERNICK_LENGTH+1 );
		strncpy( cMasterNick, nick, NDD_MP_MASTERNICK_LENGTH );
	}
};

//	en_dg_mp_mode_info_req=1,					// ���� ���� ��û.
struct CMSG_MP_INFO_REQ : public CMSG_MASTER_AND_PUPIL
{
	char					cFromName[en_charac_name_length+1];

	int						iMasterIndex;	// 

	CMSG_MP_INFO_REQ()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_INFO_REQ), en_dg_mp_mode_info_req )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_no_info=2,					// ���� ���� ������ ���ٸ�, �� ��Ŷ���� �˸���.
struct CMSG_MP_NO_INFO : public CMSG_MASTER_AND_PUPIL
{
	char					cFromName[en_charac_name_length+1];

	int						iMasterIndex;		// �� ���� �ʿ䰡 ��������.

	CMSG_MP_NO_INFO()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_NO_INFO), en_dg_mp_mode_no_info )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_add_point=3,					// ����Ʈ ���� ��û.
struct CMSG_MP_ADD_POINT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	int						iFromCharac;
	int						iAddValue;			// ���϶�.

	CMSG_MP_ADD_POINT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_ADD_POINT), en_dg_mp_mode_add_point )
	{
	}
};

//	en_dg_mp_mode_update_point=4,				// ����Ʈ ���� �˸� ��Ŷ.
struct CMSG_MP_UPDATE_POINT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	u_int					uiMasterTotalPoint;			// ������.
	u_int					uiMasterCurPoint;			// ������


	int						iFromCharac;
	u_int					uiPupilCurPoint;			// 

	CMSG_MP_UPDATE_POINT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_UPDATE_POINT), en_dg_mp_mode_update_point )
	{
	}

};

//	en_dg_mp_mode_add_kill_count=5,				// ų ī��Ʈ ���� ��û.
struct CMSG_MP_ADD_KILL_COUNT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	int						iFromCharac;
	int						iAddValue;			// ���϶�

	CMSG_MP_ADD_KILL_COUNT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_ADD_KILL_COUNT), en_dg_mp_mode_add_kill_count )
	{
	}

};

//	en_dg_mp_mode_update_kill_count=6,			// ų ī��Ʈ ���� �˸� ��Ŷ.
struct CMSG_MP_UPDATE_KILL_COUNT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	u_int					uiTotalKillCount;

	int						iFromCharac;
	u_int					uiValue;			// ������

	CMSG_MP_UPDATE_KILL_COUNT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_UPDATE_KILL_COUNT), en_dg_mp_mode_update_kill_count )
	{
	}

};

// en_dg_mp_mode_update_rank=7,				// GS->DS->GS ��ũ ������Ʈ
struct CMSG_MP_UPDATE_RANK : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	u_char					ucRank;				// �� ������ ������Ʈ�ϰ�, Ŭ���̾�Ʈ�� �˸���. 

	CMSG_MP_UPDATE_RANK()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_UPDATE_RANK), en_dg_mp_mode_update_rank )
	{
	}
};


// en_dg_mp_mode_create_mp_req=8,				// ���� ���� ���� ��û.
struct CMSG_MP_CREATE_MP_REQ : public CMSG_MASTER_AND_PUPIL
{
	char					cFromName[en_charac_name_length+1];

	int						iMasterIndex;

	u_char					ucLevel;			// ���緹��/���� ���� ���� ����ȭ�� ���� �ʿ���.
	u_char					ucMoonpa;
	u_char					ucClass;
	u_char					ucClassGrade;

	CMSG_MP_CREATE_MP_REQ()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_CREATE_MP_REQ), en_dg_mp_mode_create_mp_req )
	{
	}

	void SetName( char * name )
	{
		CopyMemory( cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}
};

// en_dg_mp_mode_create_success=9,				// ���� ���� ���� ����.
struct CMSG_MP_CREATE_SUCCESS : public CMSG_MASTER_AND_PUPIL
{
	char					cFromName[en_charac_name_length+1];

	int						iMasterIndex;		// 

	u_char					ucMasterSex;
	u_char					ucMasterInnerLevel;
	u_char					ucMasterMoonpa;
	u_char					ucMasterClass;
	u_char					ucMasterClassgrade;


	// ���������� ���� ������ ������ ����.
	// �׳� �ʱ� ������ �����ϰ�, �˷��ش�.
	CMSG_MP_CREATE_SUCCESS()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_CREATE_SUCCESS), en_dg_mp_mode_create_success )
	{
	}

	void SetName( char * name )
	{
		CopyMemory( cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}
};

// en_dg_mp_mode_create_fail=10,				// ���� ���� ���� ����.
struct CMSG_MP_CREATE_FAIL : public CMSG_MASTER_AND_PUPIL
{
	char					cFromName[en_charac_name_length+1];

	int						iMasterIndex;

	u_char					ucFailCode;		// 

	// 

	CMSG_MP_CREATE_FAIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_CREATE_FAIL), en_dg_mp_mode_create_fail )
	{
	}

	void SetName( char * name )
	{
		CopyMemory( cFromName, name, en_charac_name_length );
		cFromName[en_charac_name_length] = 0;
	}
};

// en_dg_mp_mode_notice_connection=11,			// ����/���� �˸� ��Ŷ.
struct CMSG_MP_NOTICE_CONNECTION : public CMSG_MASTER_AND_PUPIL
{
	u_char					ucConnectState;		// 0 - logout, 1 - enter field, 2 - Server move.

	u_char					ucCurZone;			// Zone �ε����� �����ؼ� ������. 

	int						iMasterIndex;		// ���� ������ ã�µ� ���δ�.
	int						iCharacIndex;		// ���� �������� ĳ���͸� ã�´�.

	CMSG_MP_NOTICE_CONNECTION()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_NOTICE_CONNECTION), en_dg_mp_mode_notice_connection )
	{
	}
};


// en_dg_mp_mode_change_master_nick_req=12,	// ��ȣ ���� �˸�.
struct CMSG_MP_CHANGE_MASTER_NICK_REQ : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	char					cNick[NDD_MP_MASTERNICK_LENGTH+1];

	CMSG_MP_CHANGE_MASTER_NICK_REQ()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_CHANGE_MASTER_NICK_REQ), en_dg_mp_mode_change_master_nick_req )
	{
	}

	void SetNick( char * nick )
	{
		ZeroMemory( cNick, sizeof(cNick) );
		strncpy( cNick, nick, NDD_MP_MASTERNICK_LENGTH );
	}
};

// en_dg_mp_mode_update_master_nick=13,		// ��ȣ �����϶�.
struct CMSG_MP_UPDATE_MASTER_NICK : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	char					cNick[NDD_MP_MASTERNICK_LENGTH+1];

	CMSG_MP_UPDATE_MASTER_NICK()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_UPDATE_MASTER_NICK), en_dg_mp_mode_update_master_nick )
	{
	}

	void SetNick( char * nick )
	{
		ZeroMemory( cNick, sizeof(cNick) );
		strncpy( cNick, nick, NDD_MP_MASTERNICK_LENGTH );
	}
};

//en_dg_mp_mode_update_pupil_position=14,		// ������ ������ ���ߴ�. (������׸� ����)
struct CMSG_MP_UPDATE_PUPIL_POSITION : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterindex;
	struct {
		int					iPupilIndex;				//
		u_char				ucNewPos;
	} ChangePosition[NDD_MP_MAX_PUPIL_COUNT];

	CMSG_MP_UPDATE_PUPIL_POSITION()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_UPDATE_PUPIL_POSITION), en_dg_mp_mode_update_pupil_position )
	{
		ZeroMemory( ChangePosition, sizeof(ChangePosition) );
	}
};


// en_dg_mp_mode_chat
struct CMSG_MP_CHAT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	char					cFromName[en_charac_name_length+1];

	char					cChat[MAX_CHAT_LENGTH];

	CMSG_MP_CHAT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_CHAT), en_dg_mp_mode_chat )
	{
	}
};



//	en_dg_mp_mode_info_error=50,				// ���� ���� ���� �޽���.
struct CMSG_MP_INFO_ERROR : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	u_char					ucErrorCode;		// ����.

	CMSG_MP_INFO_ERROR()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_INFO_ERROR), en_dg_mp_mode_info_error )
	{
	}

};




// 51 - 80 : ���� ���� �α� ���� ��Ŷ.
//	en_dg_mp_mode_gd_apply_for_pupil=51,
struct CMSG_MP_GD_APPLY_FOR_PUPIL : public CMSG_MASTER_AND_PUPIL
{
	u_char					ucPupilMoonpa;			// ��/�鵵. 0/1
	u_char					ucPupilCurLevel;		// ������ 13�̻�.
	u_char					ucPupilClass;
	u_char					ucPupilClassGrade;

	char					cFromPupilName[en_charac_name_length+1];
	char					cToMasterName[en_charac_name_length+1];

	CMSG_MP_GD_APPLY_FOR_PUPIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_GD_APPLY_FOR_PUPIL), en_dg_mp_mode_gd_apply_for_pupil )
	{
	}

	inline void SetName( char * fromPupil, char * toMaster )
	{
		CopyMemory( cFromPupilName, fromPupil, en_charac_name_length );
		cFromPupilName[en_charac_name_length] = 0;
		CopyMemory( cToMasterName, toMaster, en_charac_name_length );
		cToMasterName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_dg_apply_for_pupil=52,
struct CMSG_MP_DG_APPLY_FOR_PUPIL : public CMSG_MASTER_AND_PUPIL
{
	u_char					ucPupilMoonpa;			// ��/�鵵. 0/1
	u_char					ucPupilCurLevel;		// ������ 13�̻�.
	u_char					ucPupilClass;
	u_char					ucPupilClassGrade;

	char					cFromPupilName[en_charac_name_length+1];
	char					cToMasterName[en_charac_name_length+1];
	// cToMasterName ĳ���͸� ã��, MSG_MP_GC_APPLY_FOR_PUPIL �� ������.

	CMSG_MP_DG_APPLY_FOR_PUPIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_DG_APPLY_FOR_PUPIL), en_dg_mp_mode_dg_apply_for_pupil )
	{
	}

	inline void SetName( char * fromPupil, char * toMaster )
	{
		CopyMemory( cFromPupilName, fromPupil, en_charac_name_length );
		cFromPupilName[en_charac_name_length] = 0;
		CopyMemory( cToMasterName, toMaster, en_charac_name_length );
		cToMasterName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_apply_for_pupil_error=53,		// ��û�ڿ��� �����ش�. 
struct CMSG_MP_APPLY_FOR_PUPIL_ERROR : public CMSG_MASTER_AND_PUPIL
{
	char					cFromPupilName[en_charac_name_length+1];
	char					cToMasterName[en_charac_name_length+1];

	u_char					ucErrorCode;	// ���� : 
	// 0 - none...
    // 1 - ������ ã�� �� ����.
	// 2 - ���� �ڰ��� ���� ĳ�����Դϴ�.
	// 3 - ���̻� ���ڸ� ���� �� �����ϴ�.

	CMSG_MP_APPLY_FOR_PUPIL_ERROR()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_APPLY_FOR_PUPIL_ERROR), en_dg_mp_mode_apply_for_pupil_error )
	{
	}

	inline void SetName( char * fromPupil, char * toMaster )
	{
		CopyMemory( cFromPupilName, fromPupil, en_charac_name_length );
		cFromPupilName[en_charac_name_length] = 0;
		CopyMemory( cToMasterName, toMaster, en_charac_name_length );
		cToMasterName[en_charac_name_length] = 0;
	}
};


//	en_dg_mp_mode_answer_yes=54,				// '��'�� �����ߴ�.
struct CMSG_MP_ANSWER_YES : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	char					cPupilName[en_charac_name_length+1];
	char					cFromMasterName[en_charac_name_length+1];

	CMSG_MP_ANSWER_YES()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_ANSWER_YES), en_dg_mp_mode_answer_yes )
	{
	}

	inline void SetName( char * pupil_name, char * master_name )
	{
		CopyMemory( cPupilName, pupil_name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
		CopyMemory( cFromMasterName, master_name, en_charac_name_length );
		cFromMasterName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_answer_apply_to_pupil_no=55,	// '�ƴϿ�'�� �������� ��.
struct CMSG_MP_ANSWER_APPLY_TO_PUPIL_NO : public CMSG_MASTER_AND_PUPIL
{
	char					cPupilName[en_charac_name_length+1];
	char					cFromMasterName[en_charac_name_length+1];

	// gs -> ds ��, ds���� cPupilName �� ã��, ds->gs �� �ٽ� ����. 
	// gs���� ������, cPupilName ���� no ����.

	CMSG_MP_ANSWER_APPLY_TO_PUPIL_NO()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_ANSWER_APPLY_TO_PUPIL_NO), en_dg_mp_mode_answer_apply_to_pupil_no )
	{
	}

	inline void SetName( char * pupil_name, char * master_name )
	{
		CopyMemory( cPupilName, pupil_name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
		CopyMemory( cFromMasterName, master_name, en_charac_name_length );
		cFromMasterName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_complete_join_pupil=56,		// ��û�ڿ��� ����.
struct CMSG_MP_COMPLETE_JOIN_PUPIL : public CMSG_MASTER_AND_PUPIL
{
	// cPupilName ���� �� ��Ŷ�� ������.
    char					cPupilName[en_charac_name_length+1];

	char					cMasterName[en_charac_name_length+1];

	CMSG_MP_COMPLETE_JOIN_PUPIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_COMPLETE_JOIN_PUPIL), en_dg_mp_mode_complete_join_pupil )
	{
	}

	inline void SetName( char * pupil, char * master )
	{
		ZeroMemory( cPupilName, sizeof(cPupilName) );
		strncpy( cPupilName, pupil, en_charac_name_length );

		ZeroMemory( cMasterName, sizeof(cMasterName) );
		strncpy( cMasterName, master, en_charac_name_length );
	}

};

//	en_dg_mp_mode_add_new_pupil=57,				// ���� �� �ٸ� ���ڿ��� ����.
struct CMSG_MP_ADD_NEW_PUPIL : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;		// 
	_sCSPK_Pupil			newPupilInfo;		// 

	CMSG_MP_ADD_NEW_PUPIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_ADD_NEW_PUPIL), en_dg_mp_mode_add_new_pupil )
	{
	}

	void SetInfo( _sCSPK_Pupil& pupil )
	{
		CopyMemory( &newPupilInfo, &pupil, sizeof(_sCSPK_Pupil) );
	}
};

// en_dg_mp_mode_master_simple_info_to_pupil
struct CMSG_MP_MASTER_SIMPLE_INFO_TO_PUPIL : public CMSG_MASTER_AND_PUPIL
{
	char					cFromPupilName[en_charac_name_length+1];
	char					cToMasterName[en_charac_name_length+1];

	u_char					ucMasterRank;
	u_char					ucMasterLevel;
	u_char					ucMasterMoonpa;
	u_char					ucMasterClass;
	u_char					ucMasterClassGrade;
	u_char					ucCurZone;

	char					cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];

	CMSG_MP_MASTER_SIMPLE_INFO_TO_PUPIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_MASTER_SIMPLE_INFO_TO_PUPIL), en_dg_mp_mode_master_simple_info_to_pupil )
	{
	}

	void SetName( char * pupil, char * master )
	{
		ZeroMemory( cFromPupilName, sizeof(cFromPupilName) );
		strncpy( cFromPupilName, pupil, en_charac_name_length );

		ZeroMemory( cToMasterName, sizeof(cToMasterName) );
		strncpy( cToMasterName, master, en_charac_name_length );
	}
	void SetNick( char * nick )
	{
		ZeroMemory( cMasterNick, sizeof(cMasterNick) );
		strncpy( cMasterNick, nick, NDD_MP_MASTERNICK_LENGTH );
	}

};


// en_cg_mp_mode_apply_error=80,				// ��û ������ ��� ����.
enum EN_MP_APPLY_ERROR		// MSG_MP_APPLY_ERROR
{
	// CMSG_MP_APPLY_FOR_PUPIL_ERROR �� �ڵ�� ���� ���� ����� ��.
	en_mp_apply_no_error=0,						// 0 ���� �׳� ����. �� �����δ� ��Ŷ�� ���� �ʴ´�.
	en_mp_apply_error_wait=1,					// 10 �� ���� : ����� �ٽ� �õ�.
	en_mp_apply_error_pupil_level_limit=2,		// 73 ���� �̻� ĳ���ʹ� ���ڰ� �� �� �����ϴ�.
	en_mp_apply_error_level_diff_limit=3,		// ���°� ���ڴ� 13���� �̻� ���̰� �����մϴ�.
	en_mp_apply_error_already_has_master=4,		// �̹� �ٸ� ������ �ֽ��ϴ�.
	en_mp_apply_error_pupils_over=5,			// �� �̻� ���ڸ� ���� �� �����ϴ�.
	en_mp_apply_error_you_are_master=6,			// ���� �����̴�.
	en_mp_apply_error_not_match_side=7,			// ��鵵 �Ӽ��� �ٸ���.
	en_mp_apply_error_target_cannot_master=8,	// ����� ���� �ڰ��� �����ϴ�.
	en_mp_apply_error_target_not_found=9,		// ����� ã�� �� �����ϴ�.
	en_mp_apply_error_fail_to_add_pupil=10,		// �߰��� ����.
};

//	en_dg_mp_mode_apply_error=80,				// ���� ���� ����.
struct CMSG_MP_APPLY_ERROR : public CMSG_MASTER_AND_PUPIL
{
	char					cFromPupilName[en_charac_name_length+1];

	u_char					ucErrorCode;

	CMSG_MP_APPLY_ERROR()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_APPLY_ERROR), en_dg_mp_mode_apply_error )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cFromPupilName, name, en_charac_name_length );
		cFromPupilName[en_charac_name_length] = 0;
	}
};



// 81 - 100 : �Ĺ� �� �ϻ�.
//	en_dg_mp_mode_strike_pupil_req=81,			// ���� : �Ĺ� ��û.
struct CMSG_MP_STRIKE_PUPIL_REQ : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	char					cPupilName[en_charac_name_length+1];

	CMSG_MP_STRIKE_PUPIL_REQ()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_STRIKE_PUPIL_REQ), en_dg_mp_mode_strike_pupil_req )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cPupilName, name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_strike_result=82,				// �Ĺ� ������ ���� �� �ٸ� ���ڿ���.
struct CMSG_MP_STRIKE_RESULT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	int						iPupilIndex;

	u_int					uiMasterTotalPoint;
	u_int					uiMasterCurPoint;
	u_char					ucBonusSkillStep;

	struct {
		int				iPupilIndex;				//
		u_char			ucNewPos;
	} ChangePosition[NDD_MP_MAX_PUPIL_COUNT];

	CMSG_MP_STRIKE_RESULT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_STRIKE_RESULT), en_dg_mp_mode_strike_result )
	{
		iMasterIndex = 0;
		iPupilIndex = 0;
		uiMasterTotalPoint = 0;
		uiMasterCurPoint = 0;
		ucBonusSkillStep = 0;

		ZeroMemory( ChangePosition, sizeof(ChangePosition) );
	}

};

//	en_dg_mp_mode_you_struck=83,				// �Ĺ� ĳ���Ϳ��� ������.
struct CMSG_MP_YOU_STRIKE : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	char					cPupilName[en_charac_name_length+1];	// �i�ܳ� ĳ������ �̸�.

	CMSG_MP_YOU_STRIKE()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_YOU_STRIKE), en_dg_mp_mode_you_struck )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cPupilName, name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
	}
};



//	en_dg_mp_mode_strike_error=90,				// �Ĺ� ���н� ���� ����.
struct CMSG_MP_STRIKE_ERROR : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;
	char					cPupilName[en_charac_name_length+1];

	u_char					ucErrorCode;		//

	// error code
	// 0 - not found master info.
	// 1 - pupil is not member.
	// 2 - progressing other strike
	// 3 - minimum 72 hours.
	// 4 - system error.

	CMSG_MP_STRIKE_ERROR()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_STRIKE_ERROR), en_dg_mp_mode_strike_error )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cPupilName, name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
	}
};


//	en_dg_mp_mode_leave_req=91,					// �ϻ� ��û.
struct CMSG_MP_LEAVE_REQ : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	char					cPupilName[en_charac_name_length+1];

	CMSG_MP_LEAVE_REQ()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_LEAVE_REQ), en_dg_mp_mode_leave_req )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cPupilName, name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_you_left=92,					// �ϻ� ������ ��û ���ڿ��� ��.
struct CMSG_MP_YOU_LEFT : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	char					cPupilName[en_charac_name_length+1];

	CMSG_MP_YOU_LEFT()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_YOU_LEFT), en_dg_mp_mode_you_left )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cPupilName, name, en_charac_name_length );
		cPupilName[en_charac_name_length] = 0;
	}
};

//	en_dg_mp_mode_left_pupil=93,				// ���� �� �ٸ� ���ڿ��� �ϻ��� �˸���.
struct CMSG_MP_LEFT_PUPIL : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	int						iPupilIndex;

	//!// ��Ÿ ������ ä�� �ʿ䰡 �ִ�.
	u_int					uiMasterTotalPoint;
	u_int					uiMasterCurPoint;
	u_char					ucBonusSkillStep;

	struct {
		int				iPupilIndex;				//
		u_char			ucNewPos;
	} ChangePosition[NDD_MP_MAX_PUPIL_COUNT];

	CMSG_MP_LEFT_PUPIL()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_LEFT_PUPIL), en_dg_mp_mode_left_pupil )
	{
		iMasterIndex = 0;
		iPupilIndex = 0;
		uiMasterTotalPoint = 0;
		uiMasterCurPoint = 0;
		ucBonusSkillStep = 0;

		ZeroMemory( ChangePosition, sizeof(ChangePosition) );
	}
};

//	en_dg_mp_mode_left_error=100,				// �ϻ� ������ ����.
struct CMSG_MP_LEFT_ERROR : public CMSG_MASTER_AND_PUPIL
{
	char					cPupilNane[en_charac_name_length+1];

	u_char					ucErrorCode;

	// error code
	// 0 - not found master info.
	// 1 - pupil is not member.
	// 2 - progressing other strike
	// 3 - minimum 72 hours.
	// 4 - system error.


	CMSG_MP_LEFT_ERROR()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_LEFT_ERROR), en_dg_mp_mode_left_error )
	{
	}

	inline void SetName( char * name )
	{
		CopyMemory( cPupilNane, name, en_charac_name_length );
		cPupilNane[en_charac_name_length] = 0;
	}
};

// 250 ���� : �޸� �߰�/���� ���� ��ȯ.
//	en_dg_mp_mode_del_from_memory=101,			// �޸𸮿��� ����.
struct CMSG_MP_DEL_FROM_MEMORY : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;

	CMSG_MP_DEL_FROM_MEMORY()
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_DEL_FROM_MEMORY), en_dg_mp_mode_del_from_memory )
	{
	}
};



// 151 ~ 170 : ���� ��û �ź� �� �Խ��� ����
// en_dg_mp_mode_accept_pupil_mode = 151,		// ���� ��û �ź� ����.
struct CMSG_MP_ACCEPT_PUPIL_MODE : public CMSG_MASTER_AND_PUPIL
{
	int						iMasterIndex;			// 

	char					cCharacName[en_charac_name_length+1];

	u_char					ucAcceptMode;			// 0 - Yes, 1 - No. 

	CMSG_MP_ACCEPT_PUPIL_MODE() 
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_ACCEPT_PUPIL_MODE), en_dg_mp_mode_accept_pupil_mode ) 
	{
	}

	void SetName( char * name )
	{
		if( name )
		{
			ZeroMemory( cCharacName, sizeof(cCharacName) );
			strncpy( cCharacName, name, en_charac_name_length );
		}
	}
};


// en_dg_mp_mode_board_initial_req = 152,		// ó�� ��û�� ��.( �Խ����� �� �� )
struct CMSG_MP_BOARD_INITIAL_REQ : public CMSG_MASTER_AND_PUPIL
{
	// ó������ ��û�ϴ� ����.
	// initial_open ���� ������ ��.
	char					cCharacName[en_charac_name_length+1];

	char					cSide;			// -1 �浵, 0 - ����, 1 - �鵵

	CMSG_MP_BOARD_INITIAL_REQ() 
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_BOARD_INITIAL_REQ), en_dg_mp_mode_board_initial_req ) 
	{
	}

	void SetName( char * name )
	{
		if( name )
		{
			ZeroMemory( cCharacName, sizeof(cCharacName) );
			strncpy( cCharacName, name, en_charac_name_length );
		}
	}
};


// en_dg_mp_mode_board_initial_open = 153,		// ó�� ��û���� ����. ( 1 ������ ������ ���Եȴ�. )
struct CMSG_MP_BOARD_INITIAL_OPEN : public CMSG_MASTER_AND_PUPIL
{
	char						cCharacName[en_charac_name_length+1];

	char						cSide;					// GS ���� �ٽ� �޾��� �� �ʿ��ϴ�.

	u_char						ucTotalPage;			// ������ ��.

	u_char						ucCount;				// 

	// ucCount �� �߶� ����.
	_sPacketMPBoard				board[NDD_MP_MAX_ITEMCOUNT_PER_BOARD];				// �ִ� 12��
	

	CMSG_MP_BOARD_INITIAL_OPEN() 
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_BOARD_INITIAL_OPEN), en_dg_mp_mode_board_initial_open ) 
	{
	}

	void SetName( char * name )
	{
		if( name )
		{
			ZeroMemory( cCharacName, sizeof(cCharacName) );
			strncpy( cCharacName, name, en_charac_name_length );
		}
	}

	void Reset()
	{
		ucTotalPage = 0;
		ucCount = 0;
		ZeroMemory( board, sizeof(board) );
	}

	void AddBoard( _sPacketMPBoard * b )
	{
		if( ucCount < NDD_MP_MAX_ITEMCOUNT_PER_BOARD )
		{
			CopyMemory( &board[ucCount], b, sizeof(_sPacketMPBoard) );
			++ucCount;
		}
	}


	void SetLength()
	{
		if( ucCount > NDD_MP_MAX_ITEMCOUNT_PER_BOARD )
			ucCount = NDD_MP_MAX_ITEMCOUNT_PER_BOARD;

		usLength = sizeof(CMSG_MP_BOARD_INITIAL_OPEN) - ( (NDD_MP_MAX_ITEMCOUNT_PER_BOARD - ucCount) * sizeof(_sPacketMPBoard) );
	}
};


// en_dg_mp_mode_board_req_page=154,			// �Խ��� ���� ��û. ������ ����.
struct CMSG_MP_BOARD_REQ_PAGE : public CMSG_MASTER_AND_PUPIL
{
	char						cCharacName[en_charac_name_length+1];

	char						cSide;					// 

	u_char						ucReqPage;				// ��û ������.

	CMSG_MP_BOARD_REQ_PAGE() 
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_BOARD_REQ_PAGE), en_dg_mp_mode_board_req_page ) 
	{
	}

	void SetName( char * name )
	{
		if( name )
		{
			ZeroMemory( cCharacName, sizeof(cCharacName) );
			strncpy( cCharacName, name, en_charac_name_length );
		}
	}
};


// en_dg_mp_mode_board_page_info=155,			// �Խ��� ����.
struct CMSG_MP_BOARD_PAGE_INFO : public CMSG_MASTER_AND_PUPIL
{
	char						cCharacName[en_charac_name_length+1];

	char						cSide;					// 
	u_char						ucTotalPage;		// ��ü ���������� �׻� ������.

	u_char						ucPage;				// ������ 
	u_char						ucCount;			// ������ ����

	_sPacketMPBoard				board[NDD_MP_MAX_ITEMCOUNT_PER_BOARD];

	CMSG_MP_BOARD_PAGE_INFO() 
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_BOARD_PAGE_INFO), en_dg_mp_mode_board_page_info ) 
	{
	}

	void SetName( char * name )
	{
		if( name )
		{
			ZeroMemory( cCharacName, sizeof(cCharacName) );
			strncpy( cCharacName, name, en_charac_name_length );
		}
	}

	void Reset( u_char page )
	{
		ucPage = page;
		ucCount = 0;
		ZeroMemory( board, sizeof(board) );
	}

	void AddBoard( _sPacketMPBoard * b )
	{
		if( ucCount < NDD_MP_MAX_ITEMCOUNT_PER_BOARD )
		{
			CopyMemory( &board[ucCount], b, sizeof(_sPacketMPBoard) );
			++ucCount;
		}
	}


	void SetLength()
	{
		if( ucCount > NDD_MP_MAX_ITEMCOUNT_PER_BOARD )
			ucCount = NDD_MP_MAX_ITEMCOUNT_PER_BOARD;

		usLength = sizeof(CMSG_MP_BOARD_PAGE_INFO) - ( (NDD_MP_MAX_ITEMCOUNT_PER_BOARD - ucCount) * sizeof(_sPacketMPBoard) );
	}
};


// en_dg_mp_mode_board_error = 170,			// ����.
struct CMSG_MP_BOARD_ERROR : public CMSG_MASTER_AND_PUPIL
{
	char						cCharacName[en_charac_name_length+1];

	u_char						ucError;			// 

	CMSG_MP_BOARD_ERROR() 
		: CMSG_MASTER_AND_PUPIL( sizeof(CMSG_MP_BOARD_ERROR), en_dg_mp_mode_board_error ) 
	{
	}

	void SetName( char * name )
	{
		if( name )
		{
			ZeroMemory( cCharacName, sizeof(cCharacName) );
			strncpy( cCharacName, name, en_charac_name_length );
		}
	}

	// 0 - ���̻� �������� �����ϴ�.
};


#endif	// _PD_MASTER_PUPIL_



#pragma pack( pop, enter_member )
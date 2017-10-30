#pragma once

struct _sHeader ;

/* =====================================================================================
   �̺�Ʈ ���� ��Ŷ : ��û�� ����(������ ��������, ���� ���� �ִ�)
===================================================================================== */

#define MSG_NO_EVENT_REQUEST      253	// �̺�Ʈ ���� ��û - �̺�Ʈ ������ ����, �ٸ� ��Ŷ�� ���� ���̴�.( ������� �̷�����. )
#define MSG_NO_EVENT_RESULT       MSG_NO_EVENT_REQUEST // �̺�Ʈ ���( ����� �ʿ��ϴٸ� )


#define MSG_NO_NATIONAL_DIST	254		//�������� �ٸ� �ǹ̷� ���Ǵ� ��Ŷ. ���� �����Ѵ�


struct MSG_EVENT_REQ
{
    _sHeader header ;
	u_char   ucMessage ;
    u_short  usEventID ;        // �̰��� �����ϰ�, ���� ���� ����� �ٸ�.
};

// ������ �̺�Ʈ ���̵� ���Դϴ�. �̺�Ʈ�� �ߺ��� ���, �����ϱ� ���� ���Դϴ�.
enum EN_EVENT_ID
{
    en_event_ministop_200508_1=1,								// 1 ����� SMS �̺�Ʈ  : 2005�� 8�� �̺�Ʈ.
		en_event_familymart_200511=2,							// 2 �йи� ��Ʈ �̺�Ʈ : 2005�� 11�� ����	
		en_event_sms_quiz_200511=3,								// 3 ���� ����	
}; 

// 1. �̴Ͻ��� �̺�Ʈ.
struct MSG_MINISTOP_EVENT_REQ : public MSG_EVENT_REQ
{
	char cKey[16] ;                       // 15�ڸ� Ű���� ����Ѵ�.
};

#define _EVENT_FAMILYMART_2_KEY_LENGTH_            9            // 8���� + NULL(1)
struct MSG_FAMILYMART_2_EVENT_REQ : public MSG_EVENT_REQ

{
	char cKey[_EVENT_FAMILYMART_2_KEY_LENGTH_] ;     // 8�ڸ� Ű��.������ ����Ʈ�� �׻� 0 �̾�� ��.
} ;


#define _EVENT_KEY_MAX_LENGTH_            32         // 31���� + NULL(1)
struct MSG_EVENT_REQ_WITH_KEY : public MSG_EVENT_REQ
{
	char cKey[_EVENT_KEY_MAX_LENGTH_] ;
} ;

enum EN_ENENT_RESULT
{
	en_event_result_one_event = 0,
	en_event_result_notice,
	en_event_result_invite_friend,		// �̱� : ģ�� �ʴ� �̺�Ʈ�� ��� �߰�
};

// ���н��� �����Դϴ�.
struct MSG_EVENT_RESULT                 // ���� ���и� ����Ѵ�.
{
    _sHeader  header ;
    u_char    ucMessage ;               // MSG_NO_EVENT_RESULT
	
	#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ����� //breif : �̺�Ʈ ������ ������	
	u_char ucEventResultMode;			//0 - �Ϲ� : ���� �̺�Ʈ�� ��� 1 - ��Ƽ�� : Type 2 �̺�Ʈ���� ���� ����
	#else
	u_short   usEventID ;
	u_char    ucResult ;                // Find EN_EVENT_ERRNO

	u_char		ucDetailInfo;			// ucResult ���� en_event_errno_no_item_slot �϶�, 
										// �ʿ� ������ ������ �ǹ��Ѵ�.
	#endif
};

// mode 0 == en_event_result_one_event
struct MSG_ONE_EVENT_RESULT : public MSG_EVENT_RESULT
{
	u_short	usEventID;
	u_char	ucResult;	// Find EN_EVENT_ERRNO

	u_char	ucDetailInfo;	// ucResult ���� en_event_errno_no_item_slot �϶�, // �ʿ� ������ ������ �ǹ��Ѵ�.
};

// mode 1 == en_event_result_notice
struct MSG_PRE_NOTICE_EVENT_RESULT : public MSG_EVENT_RESULT
{
	struct 
	{
		u_short	usEventID;
		u_char	ucResult;	// Find EN_EVENT_ERRNO
	}EventResult[8];
};

// en_event_result_invite_friend
struct MSG_INVITE_FRIEND_RESULT : public MSG_EVENT_RESULT
{
	short			sAddHyperRunID;		// �߰��Ǵ� ��� ID
	_sND_TimeUnit	EndTime;			// ����� ������� �ð�
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
	en_event_errno_no_premium_user, // �����̾� ������ �ƴմϴ�.
	en_event_is_going_on=50,		// �̺�Ʈ�� �������̴�.( �ű����� �̺�Ʈ�� ����, ������ �� �̸� �˷��ش�. )

	
	en_event_errno_system=100,		// ��Ŷ ���̵��� �ý��� ����..
	en_event_errno_over_insert,		// ���� �Է�Ƚ�� �ʰ�
	
};


#define MSG_NO_US_AD_REWARD_REQ                       157
#define MSG_NO_US_AD_REWARD_ACK                       157 

enum US_AD_REWARD_TYPE 
{
    UART_UNKNOWN			= 0,
    UART_DEAD				= 0x0001,
	UART_MEDITATION			= 0x0002,
	UART_AFTER_GAME_LOADING = 0x0004,
	UART_GOLD				= 0x1000,
	UART_ITEM				= 0x2000
};



struct MSG_US_AD_REWARD_REQ 
{
	_sHeader				header;
	u_char					ucMessage;
    US_AD_REWARD_TYPE       Type;		// ������ ������ ���� ���� Ÿ��
}; 

typedef struct	ADItemInfo
{									
	char                    cClass1;	// ��з�
	WORD					wClass3;    // �Һз�
	UCHAR					ucQty;      // ����
}_ADItemInfo;

struct MSG_US_AD_REWARD_ACK 
{
	_sHeader                header;
	u_char                  ucMessage;
	DWORD                   dwGold;     // ���� �ݾ�
	UCHAR                   ucItemCnt;  // ������ ���� 
	_ADItemInfo				aditeminfo;	// ucItemCnt ������ŭ �ݺ�
};


//#define MSG_NO_NATIONAL_DIST 254      // �������� �ٸ� �ǹ̷� ���Ǵ� ��Ŷ. ���� �����Ѵ�.

struct MSG_NATIONAL_DIST 
{
	_sHeader				header;
	u_char					ucMessage;

	u_char					ucMode;
};

enum EN_NATIONAL_DIST_MODE
{
	en_national_acl_see_ad = 0,		// acclaim : ���� �ٺ�������
	en_national_res_see_ad,

	en_national_vina_sds_ctrl = 20,	// vina : �˴ٿ� ���� ��Ʈ��
	
	en_national_metel_apex_kill_user = 40,	// metel(_XTAIWANESE) : apex kill message
};

// en_nationla_vina_sds_ctrl
struct MSG_NDIST_VINA_SDS_CTRL : public MSG_NATIONAL_DIST
{
	// 0:stop request & result	
	// 1:continue request & result 
	// 2:request fail
	u_char					ucStop;

	short					sCurFatigue;		// On �� ���� ��ȿ. �������� ����. - ����ð�(��)
	u_char					ucElapsedSecond;	// On �� ���� ��ȿ. �������� ����. - ����ð�(��)
};

// en_national_metel_apex_kill_user
struct MSG_NDIST_METEL_APEX_KILL_USER : public MSG_NATIONAL_DIST
{
	u_char					ucError;			// 0 - default
};


#define MSG_NO_PRESENT	201	// ���� �̺�Ʈ Ȥ��, ���� �޼� ���� ������ �˸��� ��Ŷ.

enum EN_EVENT_TYPE
{
	en_event_type_basic = 0,	// �⺻ ���� �̺�Ʈ 
	en_event_type_no_key_event,	// Ű ���� ����۰� ���� ó��
	en_event_type_pre_notice_no_key,	// ���ӽ� �̸� �̺�Ʈ ���� ��Ȳ�� �˷��ִ� �̺�Ʈ
	en_event_type_pre_notice,	// ������ 2���� ����.
	en_event_type_level_present	// ������ ����
};

struct MSG_PRESENT
{
	_sHeader header;
	u_char ucMessage;
	u_char ucMode;	// �Ʒ��� EN_PRESENT_MODE
	short sEventID;	// �̺�Ʈ ���̵�.
};

enum EN_PRESENT_MODE
{
	en_present_mode_notice = 0,	// � ������ �ִ� : ���� ������ ���� ���, �˷��ش�. 
	en_present_mode_req_present,	// c -> s : ������ ������ ��û�Ѵ�. : �̺�Ʈ Ÿ�Ե� ����.
	en_present_mode_recv_present,	// s -> c : ������ �ش�.
	
	en_present_mode_error = 255	// ���� �޽��� : req_present �� �������� ���ȴ�. 
};

//en_present_mode_notice=0
struct MSG_PRESENT_NOTICE : public MSG_PRESENT
{
	u_short usPresentPackageIndex;	// ���� ��Ű�� �ε��� : �ʿ� ������ �Ƚᵵ ��. 
	u_char ucNeedInvenSlotCount;	// ������ �ޱ� ���� �ʿ��� �κ��丮 ���� ��.
	int iInfo;	// ������ ������ ���, ����.
};

//          en_present_mode_req_present=1,                          // c -> s : ������ ������ ��û�Ѵ�. : �̺�Ʈ Ÿ�Ե� ����.
// event id �� ä���� ������ ��.
struct MSG_PRESENT_REQ_PRESENT : public MSG_PRESENT
{
	int iInfo;	// ���� �Ⱦ���.
};

//          en_present_mode_recv_present=2,                         // s -> c : ������ �ش�.
struct MSG_PRESENT_RECV_PRESENT : public MSG_PRESENT
{
	// �� ��Ŷ�� ���� ��Ű���� ���뿡 ����, ���� �� �� �� ����.
	int iInfo;	// ���� �޼� �̺�Ʈ �� ��, ����. 
	
	_sUser_Inven_v1_all UserInven;	// ������ ����
};

//          en_present_mode_error=255,                                  // ���� �޽��� : req_present �� �������� ���ȴ�. 
struct MSG_PRESENT_ERROR : public MSG_PRESENT
{
	u_char ucErrorCode;	// �Ʒ��� EN_PRESENT_ERROR 
	int iInfo;	// ������ ������ ���, ����
};

enum EN_PRESENT_ERROR 
{
	en_present_error_no_event = 0,	// �׷� �̺�Ʈ ����. 
	en_present_error_already_recv,	// �̹� �޾ư� ����.
	en_present_error_need_inven	// �κ� ������ ����. �ٽ� �õ��� ��.
};

 

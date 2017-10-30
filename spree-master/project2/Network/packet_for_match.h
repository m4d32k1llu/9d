#pragma once

// Match

struct _sHeader;

#define MSG_NO_MATCH_LIST					59		// �� : ����Ʈ ���� �� ����
#define MSG_NO_MATCH_READY					60		// �� �غ� : ��û/����/���� ���
#define MSG_NO_MATCH_CTRL					61		// �� : ����/�߰�/���
#define MSG_NO_MATCH_NOTICE					62		// �� ����/����� ������ �˸�

#define MSG_NO_FREE_MATCH					179		// ���� �� ��Ŷ

enum
{
	en_match_list_mode_register = 0,		// 0 ��� : ��û�� OK����
	en_match_list_mode_delete,				// 1 ���� ��û : ��û�� ����
	en_match_list_mode_not_map,				// 2 ��� ���и� �˸�
	en_match_list_mode_req_list,			// 3 ����Ʈ ��û : ���� ��û �� ������ ��û
	en_match_list_mode_list_update,			// 4 �ο����� ����Ʈ ������Ʈ : ����Ʈ�� �ʱ�ȭ�ϰ� ������Ʈ
	en_match_list_mode_list_continue		// 5 �ο����� ����Ʈ : �̾��� : �ʱ�ȭ ���� �߰�
};

//#define MST_NO_MATCH_LIST
struct MSG_MATCH_LIST
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;
};

struct __fighterInfo
{
	u_char			ucLength;				// �� ����ü�� ����
	char			cMoonpa;				// ����
	char			cClass;					// ����
	u_char			ucSex : 2;				// ����
	u_char			ucMatchGrade : 6;		// �� ���
	u_short			uid;					// ����ũ ���̵�
	u_short			usWinRate;				// �·�
	u_char			ucMatchState;			// �� ����
	u_char			ucInnerLevel;			// ���� ����
	char			cCharacName[13];		// �̸��� ���� + 1�� �ڸ���.
};

const int size_bimu_pack = 2040;

struct MSG_MATCH_LIST_INFO : public MSG_MATCH_LIST
{
	u_char			ucCount;				// �ο����� ����
	
	char			cPack[size_bimu_pack];	// __fighterInfo�� ucCount ��ŭ ���ԵǾ�����
};

enum
{
	en_match_ready_challenge_to_other = 0,		// 0 c->s ���������� �񹫸� ��û�Ѵ�
	en_match_ready_challenge_from_other,		// 1 s->c �������� ������ �񹫸� ��û�ϰ� �ִ�.
	en_match_ready_challenge_cancel,			// 2 c->s s->c : ����ϰų� ��ҵ����� �˷���
	en_match_ready_reply,						// 3 ��û�� ����, �Ǵ� �ٸ� ������ ����
	en_match_ready_start_make_table = 10,		// 10 ��ø �ۼ��� �����϶�
	en_match_ready_update_waittime,				// 11 ���ð� ������Ʈ
	en_match_ready_update_recordtype,			// 12 ����/����� ����
	en_match_ready_update_matchtype,			// 13 �� ���� ���� : ������������
	en_match_ready_update_matchtime,			// 14 �� �ð� ���� : 2�к��� ����
	en_match_ready_update_disable_hyperrun,		// 15 ��� ��� ���� ����
	en_match_ready_update_keep_buff,			// 16 ���� ȿ���� ���� ���� ����
	en_match_ready_update_balance,				// 17 �� ���� ����
	en_match_ready_req_add_item,				// 18 ������ �ø��� ��û : ���� 22 or 100
	en_match_ready_req_del_item,				// 19 ������ ������ ��û : ���� 22 or 100
	en_match_ready_add_item,					// 20 ��ø�� �������� �߰�
	en_match_ready_del_item,					// 21 ��ø���� �������� ����
	en_match_ready_set_my_item,					// 22 ��ø �������� �ø��ų� ���� ��� : �״�� ����
	en_match_ready_update_after_draw,			// 23 ������ ���º� �� ���, ���� ��� ���� 0����/1���� ����
	en_match_ready_req_select_member,			// 24 ������ : �ο� ���� ��û
	en_match_ready_select_member,				// 25 ������ : �ο� ���� ����
	en_match_ready_deselect_member,				// 26 ������ : �ο� ���� ��� ('---'�� ǥ��)
	
	en_match_ready_free_request_match = 60,		// 60 ������ ��û ��Ŷ
	en_match_ready_free_match_full,				// 61 ������ - �ο� �ʰ� ����
	en_match_ready_free_no_money,				// 62 ������ - ���� ��� ����

	en_match_ready_confirm = 80,				// 80 c->s s->c1,c2 Ȯ�� ��ư ������ ��, ������ ��û/���� �ްų� �˷���
	en_match_ready_cancel_all,					// 81 �� �غ� ������ ���� �Ѵ�
	
	en_match_ready_errno_cannotupdate = 100,	// 100 �� �غ� ���� : ������ �� ����
	en_match_ready_errno_no_item				// 101 �� �غ� ���� : �׷� ������ ����
};

//#define MSG_NO_MATCH_READY
struct MSG_MATCH_READY
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;
};

// Mode : en_match_ready_challenge_to_other
struct MSG_MATCH_READY_CHALLENGE_TO : public MSG_MATCH_READY
{
	u_char			ucMatchType;				// ����/���� ���� : ����0 ����1
	u_short			usTo;						// ����� ���̵�
};

// Mode : en_match_ready_challenge_from_other
struct MSG_MATCH_READY_CHALLENGE_FROM : public MSG_MATCH_READY
{
	u_char			ucMatchType;				// ����/���ܸ� ���� : ����0, ����1
	u_short			usFrom;						// ���� ����� ����ũ ���̵�
	char			cName[13];					// �̸�
};

// Mode : en_match_ready_challenge_cancel
struct MSG_MATCH_READY_CHALLENGE_CANCEL : public MSG_MATCH_READY
{
	u_short			usFrom;
	// ��û�ڰ� ������ ��Ҹ� �˸���
	// �ǽ�û�ڿ��� ��û���� ��Ҹ� �˸���
	// �ڽ��� ���̵� �϶� �������� ��ҽ�Ų�� 
};

// Mode : en_match_ready_reply
struct MSG_MATCH_READY_REPLY : public MSG_MATCH_READY
{
	u_char			ucReply;
	u_short			usFrom;			// �������� �����ϴ���

	/* ucReply : 
		0 - OK �¶�, c2->s
		1 - c2->s ���� : ����� �����߽��ϴ�.
		2 - s->c1 : ����� ���� �����Դϴ�.
		3 - s->c1 : ����� ���� �� �غ����Դϴ�.
		4 - s->c1 : ������ ���� ���̰� �ʹ� ŭ
		5 - s->c1 : ���� ��û�� ���� ����Ͻʽÿ�.
		6 - s->c1 : �ڽ��� �����̹Ƿ� ��û�� �� �����ϴ�.
		7 - s->c1 : ����� �����ϴ�.
		8 - s->c1 : �������� ����� ��û�� �� �ֽ��ϴ�.
		9 - s->c1 : ����� ����� �ƴմϴ�. (����������)
		100 - s->c1 : �� �񹫴� ���� �غ��߿� �ֽ��ϴ�.
	*/
};

// ��ø���� ���̴� ������ ����
struct __matchMemberInfo
{
	u_char		ucTeam : 2;			// 0:��û���� 1:�ǽ�û����
	u_char		ucNumber : 6;		// 0~8 ������ ���� : ������������ 0����

	u_char		ucSex : 2;
	u_char		ucMoonpa : 6;	

	char		cClass;				// ����
	u_char		ucLevel;			// ����
	u_short		usWinRate;			// �·�
	u_char		ucMatchGrade;		// �����
	u_char		ucVitalRate;		// ü��
	char		cName[13];			// �̸�
};

// Mode : en_match_ready_start_make_table
struct MSG_MATCH_READY_START_MAKE_TABLE : public MSG_MATCH_READY
{
	u_char				ucMatchType;		// ����/���ܿ� ���� ��ø�� �ۼ��� �����Ѵ�.

	u_short				usWaitTime;			// ��� �ð�

	__matchMemberInfo	MemberInfo[2];		// ���� (�Ǵ� ������) �� ĳ������ ����
};

// Mode : en_match_ready_update_waittime,					// 11 ���ð� ������Ʈ
// Mode : en_match_ready_update_recordtype,					// 12 ����/����� ����
// Mode : en_match_ready_update_matchtype,					// 13 �� ���� ���� : ������������
// Mode : en_match_ready_update_matchtime,					// 14 �� �ð� ���� : 2�к��� ����
// Mode : en_match_ready_update_disable_hyperrun,			// 15 ��� ��� ���� ����
// Mode : en_match_ready_update_keep_buff,					// 16 ���� ȿ���� ���� ���� ����
// Mode : en_match_ready_update_balance,					// 17 �� ���� ����
// Mode : en_match_ready_update_after_draw,					// 23 ������ ���º� �� ���, ���� ��� ���� 0����/1���� ����
struct MSG_MATCH_READY_UPDATE : public MSG_MATCH_READY
{
	u_short				usValue;

	/*
		waittime			-> 0 : ���ð� ���� �� 0xffff �˼� ���� (�ʹ� ���� �ɸ����)
		recordtype			-> 0 : ����0/�����1
		matchtype			-> ����1 ����10 : ������������ �ǹ� : �Ʒ��� �� ����
		matchtime			-> ����2�� ���� 3��
		disable_hyperrun	-> 0: ������0/�̻��1
		keep_buff			-> 0 : ����������0/�ʱ�ȭ 1 -> �ʱ�ȭ�� ������ �� �� �̷�� ��
		balance				->50 : 10-90���� 10-��û�ڿ��� �Ҹ� 90-�ǽ�û�ڿ��� �Ҹ�
	*/
};


#define _MAX_COUNT_OF_MATCH_ITEM_			6

// Mode : en_match_ready_req_add_item
struct MSG_MATCH_READY_ITEM_ADD_REQ : public MSG_MATCH_READY
{
	u_char				ucCount;

	_sitem_pair			AddItems[_MAX_COUNT_OF_MATCH_ITEM_];
};

// Mode : en_match_ready_req_del_item
struct MSG_MATCH_READY_ITEM_DEL_REQ : public MSG_MATCH_READY
{
	u_char				ucCount;

	_sitem_pair			AddItems[_MAX_COUNT_OF_MATCH_ITEM_];	// ���Թ�ȣ�� ��ø������ ����
};

// Mode : en_match_ready_add_item
struct MSG_MATCH_READY_ITEM_ADD_ITEM : public MSG_MATCH_READY
{
	u_char				ucCount;
	_sUser_Inven		AddItems[_MAX_COUNT_OF_MATCH_ITEM_];	// ���Թ�ȣ�� ��ø������ ����
};

// Mode : en_match_ready_del_item
typedef MSG_MATCH_READY_ITEM_DEL_REQ	MSG_MATCH_READY_DEL_ITEM;

// Mode : en_match_ready_set_my_item
struct MSG_MATCH_READY_SET_MY_ITEM : public MSG_MATCH_READY
{
	u_char				ucCount;
	_sUser_Inven		SetItems[_MAX_COUNT_OF_MATCH_ITEM_];	// ���� ��ȣ�� �κ��丮 ����
};

// Mode : en_match_ready_req_select_member
struct MSG_MATCH_READY_REQ_SELECT_MEMBER : public MSG_MATCH_READY
{
	u_char				ucNumber;			// ����
	char				cName[13];			// ������ ĳ���� �̸�
};

// Mode : en_match_ready_select_member
struct MSG_MATCH_READY_SELECT_MEMBER : public MSG_MATCH_READY
{
	__matchMemberInfo	info;
};

// Mode : en_match_ready_deselect_memeber
struct MSG_MATCH_READY_DESELECT_MEMBER : public MSG_MATCH_READY
{
	u_char				ucNumber;			// ������ ����
};

struct MSG_MATCH_READY_REQ_FREE_MATCH : public MSG_MATCH_READY
{
	u_int				uiNeedMoney;		// 100
};

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// ������ �� ����
enum
{
	en_matchstate_none = 0,					// 0 �� ���ϰ� ����
	en_matchstate_making_table,				// 1 ��ø �ۼ���
	en_matchstate_waiting,					// 2 �񹫸� ��ٸ��� ����
	en_matchstate_matching,					// 3 ���߿� ����
	en_matchstate_end,						// 4 �񹫰� ������ �����
	en_matchstate_result,					// 5 ����� ���� 10�ʰ� �ִϸ��̼� ��
	en_matchstate_wait_exit,				// 6 ������ �ִ� �����̰� 10���ĸ� ������ ������ �ȴ�
	en_matchstate_just_end					// 7 �񹫰� �������� Notice ��Ŷ���� �ѹ��� ��
};


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// �� ����
enum
{
	en_matchtype_none = 0,					// 0
	en_matchtype_pvp,						// 1 1vs1 ��
	en_matchtype_free_match,				// 2 ���� ��
	en_matchtype_group_deathmatch = 10,		// 10 ������ - ������
	en_matchtype_group_chief,				// 11 ������ - �����
	en_matchtype_group_serial,				// 12 ������ - ������
	en_matchtype_group_each,				// 13 ������ - �ܽ���
	en_matchtype_race_personal = 20,		// 20 �񹫴�ȸ - ������
	en_matchtype_race_group					// 21 �񹫴�ȸ - ������
};


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// MSG_MATCH_CTRL : ���� ���� ���۰� �ڸ����, �߰� ����, ��� ���� ���

struct MSG_MATCH_CTRL
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucMode;
};

enum
{
	en_match_ctrl_start = 0,				// 0 : �� ����
	en_match_ctrl_wait,						// 1 : ���忡 �ڸ� ����. ��ٸ���
	en_match_ctrl_start_count,				// 2 : ī��Ʈ �����϶�.
	en_match_ctrl_start_fight,				// 3 : ���� ���� ����.
	en_match_ctrl_before_5_sec,				// 4 : 5�� ������

	en_freematch_ctrl_enter = 40,			// 40 : �������� ���� : ��û�� ���� ���
	en_freematch_ctrl_req_into_field,		// 41 : �������� ������������ ���� ��û
	en_freematch_ctrl_into_field,			// 42 : ������������ ����
	en_freematch_ctrl_req_safezone,			// 43 : ��������� ���� ��û
	en_freematch_ctrl_go_safezone,			// 44 : ��������� ��
	en_freematch_ctrl_req_exit,				// 45 : ���� ������ ��û
	en_freematch_ctrl_exit,					// 46 : ���� ����
	en_freematch_ctrl_not_matching,			// 47 : ������ ���°� �ƴϹǷ� ��û�� �޾Ƶ��� �� ����

	en_match_ctrl_end = 100,				// 100 : �� ���� ����� ����
	en_match_ctrl_update_record_each,		// 101 : ���� ������Ʈ
	en_match_ctrl_request_exit,				// 102 : ������ ��û
	en_match_ctrl_cannot_exit,				// 103 : ���� ���� �� ����
	en_match_ctrl_exit,						// 104 : ������ - �ڱ� ��ġ ����

	en_match_ctrl_system_error = 200,		// 200 : �� �ý��� ����
};

struct MSG_MATCH_CTRL_START : public MSG_MATCH_CTRL
{
	u_char				ucStartWith;		// 0:�񹫽�����ġ 1:�񹫴����ġ
	float				fPosX;
	float				fPosZ;
	short				sMatchFieldIndex;	// ���� �ε���
};

struct MSG_MATCH_CTRL_WAIT : public MSG_MATCH_CTRL
{
	u_short				usLeftWaitTime;
};


struct MSG_MATCH_CTRL_END : public MSG_MATCH_CTRL
{
	u_char				ucWinTeam;			// 0:��û��, 1:�ǽ�û��, 2:���º�
	
	// �̰� ������, ���ڴ� �¸� �ִϸ��̼� ���ϰ�, ���ڴ� ���� �ִϸ��̼�.
	// 10�� ������, ������ ������, 
	// ���� 10�ʵ����� esc �� ���� �� �ְ�, 
	// Ÿ�ӿ����Ǹ�, �������� ������.
};

// en_match_ctrl_update_record_each,	// 101 : ���� ������Ʈ ��Ŷ�� ���ڿ��� ����.
struct MSG_MATCH_CTRL_UPDATE_RECORD : public MSG_MATCH_CTRL 
{
	// ���� ������Ʈ
};

// en_match_ctrl_request_exit,	102 : ������ ��û
struct MSG_MATCH_CTRL_REQUEST_EXIT : public MSG_MATCH_CTRL
{
	// ���� ����.
};


// en_match_ctrl_cannot_exit,			// 103 : ���� ���� �� ����. 
struct MSG_MATCH_CTRL_CANNOT_EXIT : public MSG_MATCH_CTRL
{
	// ���� ����.
};


// en_match_ctrl_exit,					// 104 : ������ - �ڽ��� ��ġ ����.
struct MSG_MATCH_CTRL_EXIT : public MSG_MATCH_CTRL
{
	// ��ġ �̵��� �� �����̴�. 
	float			fPosX ;
	float			fPosZ ;
	
};

struct MSG_MATCH_CTRL_SYSTEM_ERROR : public MSG_MATCH_CTRL
{
	u_char				ucReason ;	
	/*	Reason Code :
	0 : �׳� ����.
	*/
};

struct MSG_MATCH_CTRL_FREE_ENTER : public MSG_MATCH_CTRL
{
	float				fPosX;
	float				fPosZ;

	u_int				uiSetMoney;		// ���� ����� ġ�� �� ���� ��
};

struct MSG_MATCH_CTRL_FREE_INTO_FIELD : public MSG_MATCH_CTRL
{
	float				fPosX;
	float				fPosZ;
};

struct MSG_MATCH_CTRL_FREE_GO_SAFEZONE : public MSG_MATCH_CTRL
{
	float				fPosX;
	float				fPosZ;
};

struct MSG_MATCH_CTRL_FREE_EXIT : public MSG_MATCH_CTRL
{
	float				fPosX;
	float				fPosZ;
};


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// MSG_MATCH_NOTICE : �� ȫ���� ������ ���� �� ����� �˸��� ��Ŷ
struct MSG_MATCH_NOTICE 
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucMode;
};

enum
{
	en_match_ntc_inform = 0,			// �� ȫ�� ��Ŷ
	en_match_ntc_sb_info,				// 1 ������ �� (����/����/�ܽ�)
	en_match_ntc_lb_info,				// 2 ������ �� (����/���)
	en_match_ntc_sb_update_vital,		// 3 ������ �� (����/����/�ܽ�)
	en_match_ntc_lb_update_vital,		// 4 ������ �� (����/���)
	en_match_ntc_inform_end				// �� ȫ�� ��
	
};

struct MSG_MATCH_NOTICE_INFORM : MSG_MATCH_NOTICE
{
	u_short				usUniqueID;		// ����ũ ���̵� : Length = 20 + strlen(cInform) 
	u_char				ucLevel;		// ����
	char				cName[13];
	char				cInform[81];	// �˸��� : ���� '\0' ������ ���ڼ��� �߶��ּ���.
};

struct MSG_MATCH_NOTICE_INFORM_END : public MSG_MATCH_NOTICE
{
	u_short				usUniqueID ;
};

struct __matchRecord
{
	u_char				ucMember0 : 4;
	u_char				ucMember1 : 4;
};


// �� ��������, ������ �����ε����� ����Ͽ� ǥ���Ѵ�.
struct MSG_MATCH_NOTICE_SB_INFO : public MSG_MATCH_NOTICE 
{
	u_short				usMatchRegionIndex;		// ������ ���� �ε���

	u_char				ucMatchType;
	u_char				ucMatchState;

	char				cChiefName0[13];
	char				cChiefName1[13];

	__matchMemberInfo	Player[2];				// ���� ������ 2���

	u_char				ucMemberCount;			// ���� �ο� : ������ ����

	// 0 ~ 15 : 16����
	// 0-�����, 1-�¸�, 2-�й�, 3-���º�
	// 4-����, 5-����,6-����ȥ������
	__matchRecord		RecordList[9];
};

struct MSG_MATCH_NOTICE_LB_INFO : public MSG_MATCH_NOTICE
{
	u_short				usMatchRegionIndex;

	u_char				ucMatchType;

	char				cChiefName0[13];
	char				cChiefName1[13];

	__matchMemberInfo	Player[2];

	u_char				ucMemberCount;

	// 0 ~ 15 : 16����
	// 0-�����, 1-�¸�, 2-�й�, 3-���º�
	// 4-����, 5-����,6-����ȥ������
	__matchRecord		RecordList[9];
};

struct MSG_MATCH_NOTICE_SB_UPDATE_VITAL : public MSG_MATCH_NOTICE
{
	u_short				usMatchRegionIndex;

	u_char				ucVitalRate0;
	u_char				ucVitalRate1;
};

struct MSG_MATCH_NOTICE_LB_UPDATE_VITAL : public MSG_MATCH_NOTICE
{
	u_short				usMatchRegionIndex;

	u_char				ucVitalRate0;
	u_char				ucVitalRate1;
};


//#define MSG_NO_FREE_MATCH		179

struct MSG_FREE_MATCH
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucFMMode;
};


enum EN_GS_FREE_MATCH_PACKET
{
	en_gsfm_ctrl = 0,			// 0 �� �ܰ��� ������ �˸���
	en_gsfm_notice,				// 1 ��������
	en_gsfm_notice_winner,		// 2 ����� ���� ����
	en_gsfm_notice_nowinner,	// 3 ���º� ���� ����
	en_gsfm_to_winner,			// 4 �¸��ڿ��Ը� ���� �޽���
	en_gsfm_show_result,		// 5 ��� ������

	en_gsfm_apply_match = 20,	// 20 npc���� �񹫴�ȸ ���� ��û�ϱ�
	en_gsfm_apply_success,		// 21 ���:����
	en_gsfm_apply_fail,			// 22 ���:����
	en_gsfm_enter_match,		// 23 �� ��ȸ �����϶� (��ġ)
	en_gsfm_req_leave,			// 24 ������ ��û
	en_gsfm_leave_match,		// 25 req_leave�� ���� ���� (��ġ ����)
	en_gsfm_leave_fail,			// 26 req_leave�� ���� ����
	en_gsfm_reward_nick,		// 27 ��ȣ ����
	en_gsfm_reward_item,		// 28 ������ ����
	en_gsfm_set_nick,			// 29 �񹫴�ȸ ��ȣ����

	en_gsfm_req_camera = 40,	// 40 ���� ����Ʈ���� ������ ��û
	en_gsfm_goto_camera,		// 41 ���� ����Ʈ�� ���� (����Ʈ �ε��� ����)
	en_gsfm_camera_fail,		// 42 ���� ����Ʈ ���� ��û�� �����ߴ�
	en_gsfm_req_camera_move,	// 43 ���� ���� ����Ʈ���� �̵�

	en_gsfm_current_state = 60,	// 60 �񹫴�ȸ�� ���� ���� ���, ���� ���¸� ��Ÿ����.
	en_gsfm_private_score,		// 61 ���� �񹫴�ȸ������ ���� ����� ������.
	en_gsfm_next_match_schedule,// 62 ���� �񹫴�ȸ�� ������ ��û�Ѵ� ==> �������� MSG_FM_CURRENT_STATE �� ����
	en_gsfm_req_score,			// 63 ������ ������ ��û�Ѵ�.

};

struct __fm_info_in_packet		// �� ��ȸ ����
{
	u_char			match_type;		// �񹫴�ȸ Ÿ�� �ε���(��ũ��Ʈ�� ����)
	
	u_short			field_index;	// ��ȸ�� ����

	short			fm_level;		// ���� ���̸�, Ư�� �������� ���� �� ��ȸ
	short			to_level;

	u_short			reward_index;	// ���� ����

	_sND_TimeUnit	start_time;		// ���� �ð� ����
	_sND_TimeUnit	end_time;		// ������ �ð� ����
};

// en_gsfm_ctrl
struct MSG_FM_CTRL : public MSG_FREE_MATCH
{
	u_char			ucFM_State;			// ���� �ܰ�
	u_int			uiElapsedSecond;	// �غ�/����/���� ���� ���ʰ� ��������
	
	__fm_info_in_packet	info;
};

// en_gsfm_notice
struct MSG_FM_NOTICE : public MSG_FREE_MATCH
{
	u_short			usTextId;
	u_short			usTextArg;			// �ؽ�Ʈ�� ���� ����, 0�̸� ����
};

// en_gsfm_notice_winner
struct MSG_FM_NOTICE_WINNER : public MSG_FREE_MATCH
{
	u_short			usTextId;

	char			cWinnerName[13];
};

// en_gsfm_notice_nowinner
struct MSG_FM_NOTICE_NOWINNER : public MSG_FREE_MATCH
{
	u_short			usTextId;
};

// en_gsfm_to_winner
struct MSG_FM_TO_WINNER : public MSG_FREE_MATCH
{
	u_short			usTextId;
};

// en_gsfm_show_result
struct MSG_FM_SHOW_RESULT : public MSG_FREE_MATCH
{
};

// en_gsfm_apply_match
struct MSG_FM_APPLY_MATCH : public MSG_FREE_MATCH
{
};

// en_gsfm_apply_success
struct MSG_FM_APPLY_SUCCESS : public MSG_FREE_MATCH
{
};

enum
{
	_fm_apply_fail_not_time = 0,	// �� �ð��� �ƴϴ�
	_fm_apply_fail_party,			// ���� ���̹Ƿ� �񹫿� ������ �� ����
	_fm_apply_fail_level,			// ������ Ʋ����
	_fm_apply_fail_not_map,			// �պ� �ƴ�
	_fm_apply_fail_already_in_filed,// �̹� ����ȿ� �ִ�
	_fm_apply_fail_system,			// �ý��� ����
};

// en_gsfm_apply_fail
struct MSG_FM_APPLY_FAIL : public MSG_FREE_MATCH
{
	u_char			ucFail;
};

// en_gsfm_enter_match
struct MSG_FM_ENTER_MATCH : public MSG_FREE_MATCH
{
	FLOAT			fx;
	FLOAT			fz;
};

// en_gsfm_req_leave
struct MSG_FM_REQ_LEAVE : public MSG_FREE_MATCH
{
	u_char			ucReq;			// 0:������ ��û : �׳� 0���� ä���
};

// en_gsfm_leave_match
struct MSG_FM_LEAVE_MATCH : public MSG_FREE_MATCH
{
	FLOAT			fx;
	FLOAT			fz;
};

// en_gsfm_leave_fail
struct MSG_FM_LEAVE_FAIL : public MSG_FREE_MATCH
{
	u_char			ucFail;			// 0 : ������ �����ϴ�.
};

//en_gsfm_reward_nick
struct MSG_FM_REWARD_NICK : public MSG_FREE_MATCH
{
    u_short         usNickID;
};

//en_gsfm_reward_item
struct MSG_FM_REWARD_ITEM : public MSG_FREE_MATCH
{
	char			cItemType;
	short			sItemID;
};

//en_gsfm_set_nick
struct MSG_FM_SET_NICK : public MSG_FREE_MATCH
{
	u_short			usNick;			// 0:����, �������� ��
	u_short			usFMRank;
};

//en_gsfm_req_camera=40
struct MSG_FM_REQ_CAMERA : public MSG_FREE_MATCH
{
};

//en_gsfm_goto_camera
struct MSG_FM_GOTO_CAMERA : public MSG_FREE_MATCH
{
	u_char			ucCameraPos;	// 0 ������ ����.
};

//en_gsfm_camera_fail
struct MSG_FM_CAMERA_FAIL : public MSG_FREE_MATCH
{
	u_char			ucFail;			// 0 - �� �ڸ��� �����ϴ�.
};

//en_gsfm_req_camera_move,
struct MSG_FM_REQ_CAMERA_MOVE : public MSG_FREE_MATCH
{
	u_char			ucCameraPos;	// �׳� 0������ ������, �������� �˾Ƽ� ����.
};

//en_gsfm_current_state=60
struct MSG_FM_CURRENT_STATE : public MSG_FREE_MATCH
{
	__fm_info_in_packet	fm_info;		// �������� ����.

	u_char				ucFMCurState;	// FREE_MATCH_STEP 
	u_int				usElapsedTime;	// �� �ܰ� ����, �� �ʰ� �����°�.

	/*

	enum FREE_MATCH_STEP	// event �ε� ����.
	{
		en_fm_step_none=0,
		en_fm_step_ready,
		en_fm_step_start,
		en_fm_step_end
	}; 

	*/
};

struct __fm_private_record
{
	u_short		usKillCount;
	u_short		usDeadCount;
};

//en_gsfm_private_score
struct MSG_FM_PRIVATE_SCORE : public MSG_FREE_MATCH
{
	__fm_private_record	my_record;		// ������ Ư���Ŀ� ���� ��������.
};

//en_gsfm_req_score
struct MSG_FM_REQ_SCORE : public MSG_FREE_MATCH
{
	// ���� ����
};

//en_gsfm_next_match_schedule
struct MSG_FM_NEXT_MATCH_SCHEDULE : public MSG_FREE_MATCH
{
	// ���� ����
};
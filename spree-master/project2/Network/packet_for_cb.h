#pragma once

// �����

struct _sHeader;

#define MSG_NO_CB_SCHEDULE			176		// ����� : ������ ���� ��Ŷ

#define max_count_of_challenge_list		20		// ����� ������ �ִ� 20��

struct _sCastleInfo
{
	int				iOrIndex;			// default 0 : for monster, or Org/Ally Index
	u_char			ucType;				// 0-Organization, 1-Ally
	u_short			usMemberCount;		// �ο���

	u_char			ucLevel;			// ��� ����

	/* �Ʒ� ���� ucCurMatchCount, ucMatchState ����
		1���� �����̶�� : Count = 0, State = 0,
		2���� ���̶�� : Count = 1, state = 1,
		3���� ���̶�� : Count = 2, state = 2
	*/
	u_char			ucCurMatchCount;	// 0, 1, 2�� �����
	u_char			ucMatchState;		// 0:������, 1:������

	char			cOrName[_XDEF_ORGANIZATIONNAMELENGTH + 1];
	char			cMasterName[13];
	char			cMasterMonpa;

	u_int			uiTotalMoney;
	u_int			uiPartMoney;

	u_short			usWinCount;			// ���� ȸ��

	// ��� ���� ��¥ 
	_sND_TimeUnit	timeUnit;

	_sND_TimeUnit	Schedule[3];		// 3���������� ������
};

struct __cb_client_challenge_info
{
	u_short			usMemberCount;
	
	u_char			ucMoonpa : 5;		// ���� index
	u_char			ucRes1 : 1;
	u_char			ucState : 1;		// 0:normal   1:fighting state
	u_char			ucType : 1;			// 0:Org	1:Ally

	char			cGroupName[_XDEF_ORGANIZATIONNAMELENGTH + 1];
	char			cMasterName[13];
};

enum EN_CB_SCHEDULE
{
	en_cb_owner_req_schedule_list = 0,	// 0 ������ ������ ����Ʈ ��û ��Ŷ
	en_cb_owner_schedule_list,			// 1 ������ ������ ����Ʈ

	en_cb_req_schedule_list,			// 2 �Ϲ� ����(ȸ��) ������ ����Ʈ ��û ��Ŷ
	en_cb_schedule_list,				// 3 ������ ����Ʈ

	en_cb_req_challenge_list,			// 4 ���� ��� ��û
	en_cb_challenge_list,				// 5 ���� ���

	en_cb_req_change_time = 20,			// 20 ������ �ð� ���� ��û
	en_cb_change_time_success,			// 21 �ð� ���� ��û ����
	en_cb_change_time_fail,				// 22 �ð� ���� ��û ����

	en_cb_req_challenge = 40,			// 40 ȸ��(����)�� ���� ��û
	en_cb_challenge_success,			// 41 ���� ��û ����
	en_cb_challenge_fail,				// 42 ���� ��û ���� : ������,Full,�ߺ�,��������,�����,�ý��ۿ���
	en_cb_challenge_cancel,				// 43 ���� ��û ���

	en_cb_u_are_owner = 60,				// 60 �ڽ��� ����� ���������� �˷��ش�
	en_cb_u_are_not_owner,				// 61 ���̻� ����� �����ְ� �ƴ��� �˸��� (����� ������ �ٲ���� ��, ���� �����ֿ���)
	en_cb_not_owner,					// 62 owner_req_shcedule_list�� ���� �� �������� �� �� �ִ�
	en_cb_no_such_castle,				// 63 ��û�� ����� ���� ����̴�
	en_cb_cur_owner_info,				// 64 ����� �ִ� ������ �� ��, �� ����������� ������ �����ش�
	
	en_cb_map_tax_rate = 70,			// 70 ������ �ʵ忡 ������ �� ���� ���� ������
	en_cb_set_total_tax,				// 71  ��� ���� ����

	en_cb_tax_request_take,				// 72 ���ְ� ������ �ŵְ���
	en_cb_tax_answer_take,				// 73 72���� ����, ����/���� ���

	en_cb_schedule_error = 100,			// 100 ����
};

 
struct MSG_CB_SCHEDULE
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;

};

// en_cb_owner_req_schedule_list = 0,
struct MSG_CB_OWNER_REQ_SCHEDULE_LIST : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;			// �ڽ��� ������ �� �ε���
};

// en_cb_onwer_schedule_list
struct MSG_CB_OWNER_SCHEDULE_LIST : public MSG_CB_SCHEDULE
{
	u_int			uiSaveMoney;		// ��� â���� ����� ��

	_sCastleInfo	CastleInfo;

	__cb_client_challenge_info	info[max_count_of_challenge_list];	// �ִ� 20��
};

// en_cb_req_schedeule_list
struct MSG_CB_REQ_SCHEDULE_LIST : public MSG_CB_SCHEDULE
{
	//
};

// en_cb_schedule_list
struct MSG_CB_SCHEDULE_LIST : public MSG_CB_SCHEDULE
{
	_sCastleInfo	CastleInfo;			// ��� ���� (�ð�����)
};

// en_cb_req_challenge_list
struct MSG_CB_REQ_CHALLENGE_LIST : public MSG_CB_SCHEDULE
{
};

// en_cb_challenge_list
struct MSG_CB_CHALLENGE_LIST : public MSG_CB_SCHEDULE
{
	__cb_client_challenge_info		info[max_count_of_challenge_list];
};

// en_cb_req_change_time 
struct MSG_CB_REQ_CHANGE_TIME : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;

	_sND_TimeUnit	time[3];
};

// en_cb_change_time_success
struct MSG_CB_CHANGE_TIME_SUCCESS : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;

	_sND_TimeUnit	timeResult[3];		// ����� �ð�
};

// en_cb_change_time_fail
struct MSG_CB_CHANGE_TIME_FAIL : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;

	u_char			errorCode;		// 0 - ��ĥ�� �ִ� �ð��� ������
};


// en_cb_req_challenge
struct MSG_CB_REQ_CHALLENGE : public MSG_CB_SCHEDULE
{
	u_int			uiNeedMoney;	// �ʿ��� ���� Ȯ���� ����
};

// en_cb_challenge_success
struct MSG_CB_CHALLENGE_SUCCESS : public MSG_CB_SCHEDULE
{
	u_int			uiLeftMoney;	// ������

	__cb_client_challenge_info	challengeInfo;
};

// en_cb_challenge_fail
struct MSG_CB_CHALLENGE_FAIL : public MSG_CB_SCHEDULE
{
	u_char			errorCode;
	/*
		0 : �� ����
		1 : ���� ����� �� á��
		2 : ����/ȸ�ְ� �ƴϴ�
		3 : ���� �����̹Ƿ� ������ �� ����
		4 : ����Ŀ� �ٽ� �õ� �� ��
		5 : �ý��� ����
		6 : ��� ���� �ð��� �ƴմϴ�.
		7 : ���� ���� �Ǿ� ����
		8 : ����/ȸ���� ������ ����. // 2008.09.02 ��Ʈ�� ��û ����
	*/
};

// en_cb_challenge_cancel
struct MSG_CB_CHALLENGE_CANCEL : public MSG_CB_SCHEDULE
{
	// ���� ���� ��û�� �����
};

// en_cb_u_are_owner = 60
struct MSG_CB_U_ARE_OWNER : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;			// ���� ��� ���ִ�
	
	u_char			ucRes1 : 7;
	u_char			ucType : 1;			// 0-Org 1-Ally

	u_char			ucScheduleCanChange;	// 0-no 1-yes 

	/*
		������ ���� �ð�
		����� ����ÿ��� �ٷ� ����.
		�߰��� ���� ���, ucScheduleCanChange�� 1�� ������,
		�׳� 24�� ������ ����. ���Ĵ� ����
	*/
};

// en_cb_u_are_not_owner = 61
struct MSG_CB_U_ARE_NOT_OWNER : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;
};

// en_cb_not_owner		// 62 owner_req_shcedule_list�� ���� �� �������� �� �� �ִ�
struct MSG_CB_NOT_OWNER : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;		// ���� ��� ���ִ�
};

// en_cb_no_such_castle		// ���� ���
struct MSG_CB_NO_SUCH_CASTLE : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;
};

// en_cb_cur_owner_info		// ��� �� ������
struct MSG_CB_CUR_OWNER_INFO : public MSG_CB_SCHEDULE
{
	u_char			ucMapIndex;		// 0�̸� ����� ����

	_sCastleInfo	OwnerInfo;

	u_char			ucBonusExpRate;	// ���ʽ� ����ġ�� - ��/�鵵 �Ӽ��� ���� ���� ��ȿ,��鵵 �Ӽ��� OwnerInfo �� cMasterMoonpa �� ���� �Ǵ��ؾ� ��.
};

// en_cb_schedule_error : ���� ���� ���� ���� ����
struct MSG_CB_SCHEDULE_ERROR : public MSG_CB_SCHEDULE
{
	u_char			ucErrorCode;
};

// en_cb_map_tax_rate = 70	: ����
struct MSG_CB_NO_MAP_TAX_RATE : public MSG_CB_SCHEDULE
{
	u_char			ucMapRate;
};

// en_cb_tax_set_total_tax = 71,	: ��� ���� ����
struct MSG_CB_TAX_SET : public MSG_CB_SCHEDULE
{
	u_char			ucManorIndex;
	
	u_int			uiTotalMoney;		// display �뵵
	u_int			uiPartMoney;
};

// en_cb_tax_request_take = 72 : ���ְ� ������ �ŵְ���
struct MSG_CB_TAX_REQUEST_TAKE : public MSG_CB_SCHEDULE
{
	u_char			ucCastleIndex;		// ������ ��� �ε���

	u_int			uiRequestMoney;
};

// en_cb_tax_answer_take = 73 : 72�� ����
struct MSG_CB_TAX_ANSWER_TAKE : public MSG_CB_SCHEDULE
{
	u_char			ucResult;				// 0:Sucess
	u_int			uiTakeMoney;			// ������ ��, ã�ư��� ��
	u_int			uiLastMoney;			// ������ ��, ã�� �� ������ ��

	/*	
		ucResult
		0 - Success
		1 - ���� ����
		2 - ���ְ� �ƴϴ�
		3 - ������ ����� ����
	*/
};


// ����� ���� ����
enum EN_COMBAT_STATUS
{
	en_combat_status_none = 0,			// ������� ���� ���� ����
	en_combat_status_before,			// ���� �� 30�� ~ ���� ����
	en_combat_status_current,			// ���� �� ~ ���� ��
	en_combat_status_after,				// ���� ����
};

// ������� �ܰ踦 �˸��� �̺�Ʈ
enum EN_COMBAT_EVENT
{
	en_combat_event_ready = 0,			// ����� ���� �� �غ� �ܰ� �̺�Ʈ
	en_combat_event_start,				// ����� �������� �˸��� �̺�Ʈ
	en_combat_event_end,				// ����� ���� �̺�Ʈ
};


/* ����� ���¿� �̺�Ʈ
event :     ready -   start -   end
status : none - before - current - after - none
*/


#define MSG_NO_COMBAT					177			// ����� ���� ��Ŷ

struct MSG_COMBAT
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucMode;			// EN_COMBAT_PACKET
};

enum EN_COMBAT_PACKET
{
	// 0 - 19 ���� : en_cb_prog_
	// 20 - 39 ���� : en_cb_threat_
	// 40 - 59 ����� ���� : en_cb_info
	// 60 - ���� �� ������, ��Ÿ : en_cb_etc

	en_cb_prog_ready_event = 0,			// 0 �غ� �˸��� (30������ �뺸)
	en_cb_prog_start_event,				// 1 ����
	en_cb_prog_end_event,				// 2 ����

	en_cb_threat_start_req = 20,		// 20 ���� ���� ��û : c->s
	en_cb_threat_start_ok,				// 21 ���� ���� ��� : s->c
	en_cb_threat_start_error,			// 22 ���� ���� ���� : s->c
	en_cb_threat_start_notice,			// 23 ���� ������ �˸� : s-> all c
	en_cb_threat_cancel_req,			// 24 ���� ���� ��û : c->s
	en_cb_threat_cancel_ok,				// 25 ���� ���� ��� : s->c
	en_cb_threat_complete_req,			// 26 ���� �Ϸ� ��û : c->s
	en_cb_threat_complete_ok,			// 27 ���� �Ϸ� ��� : s->c
	en_cb_threat_complete_error,		// 28 ���� �Ϸ� ���� : s->c
	en_cb_threat_complete_notice,		// 29 ���� �ϷḦ �˸� : s-> all c
	
	en_cb_info_current_info = 40,		// 40 �߰��� ������ �������� ���� ���� ����� ����
	en_cb_dead_penalty,					// 41 ����� �� ������ ���� ���Ե�
	en_cb_threatpoint_set,				// 42 ����� ����

	en_cb_etc_loud_chat = 60,			// 60 ������
	en_cb_etc_notice,					// 61 ����� ���� ����
	en_cb_etc_cannot_loud,				// 61 ��ħ�� ����� �� ����.

	en_cb_record_pkt_info_req = 80,		// 80 �ڱ�� ��� ��Ȳ ���� ��û
	en_cb_record_pkt_info_res,			// 81 �ڱ���� ��� ��Ȳ ���� ����
	en_cb_record_pkt_add_combat,		// 82 ����� �� ������ ���� ����
	en_cb_record_pkt_pvp_award,			// 83 ����� �⿩����
	en_cb_record_pkt_manor_rank,		// 84 ����� �¸��� ���� ���� ����
	en_cb_record_pkt_winner_award,		// 85 ����� Ư�� ����
};

/*
struct __combat_both_side_info
{
	u_char			ucDefenderType;		// o-org 1-ally 2-monster
	int				iDefenderIndex;
	char			cDefenderGroupName[_XDEF_ORGANIZATIONNAMELENGTH + 1];

	u_char			ucAttackerType;
	int				iAttackerIndex;
	char			cAttackerGroupName[_XDEF_ORGANIZATIONNAMELENGTH + 1];
};*/

struct __combat_side_info 
{
	u_char			ucType;				// group type
	int				iIndex;
	char			cGroupName[_XDEF_ORGANIZATIONNAMELENGTH+1];
};


// en_cb_prog_ready_event = 0
struct MSG_CB_PROG_READY_EVENT : public MSG_COMBAT
{
	u_short					usLeftTimeSec;

	__combat_side_info		Defender;

	u_char					ucAttackerCount;	// �� ������ ���� �߶� ���ϴ�.
	__combat_side_info		Attackers[max_count_of_challenge_list];
};

// en_cb_prog_start_event
struct MSG_CB_PROG_START_EVENT : public MSG_COMBAT
{
	__combat_side_info		Defender;

	u_char					ucAttackerCount;
	__combat_side_info		Attackers[max_count_of_challenge_list];
};

// en_cb_prog_end_event
struct MSG_CB_PROG_END_EVENT : public MSG_COMBAT
{
	u_char					ucWinSide;		// 0:��������	1:��������

	u_char					ucGroupType;	// 0:org	1:ally		2:monster
	int						iIndex;
	char					cGroupName[_XDEF_ORGANIZATIONNAMELENGTH+1];
};

// en_cb_threat_start_req
struct MSG_CB_THREAT_START_REQ : public MSG_COMBAT
{

};

// en_cb_threat_start_ok
struct MSG_CB_THREAT_START_OK : public MSG_COMBAT
{
	u_char					ucProgressRate;		// 100���� ���� ����
};

// en_cb_threat_start_error
struct MSG_CB_THREAT_START_ERROR : public MSG_COMBAT
{
	u_char			ucError;
};

// en_cb_threat_start_notice
struct MSG_CB_THREAT_START_NOTICE : public MSG_COMBAT
{
	u_char			ucCurrOwnerSide;		// 0:defender 1:attacker
	
	u_char			ucGroupType;			// 0:org 1:ally
	int				iIndex;
};

// en_cb_threat_cancel_req
struct MSG_CB_THREAT_CANCEL_REQ : public MSG_COMBAT
{

};

// en_cb_threat_cancel_ok
struct MSG_CB_THREAT_CANCEL_OK : public MSG_COMBAT
{
	u_char			ucGroupType;			// 0:org 1:ally
	int				iIndex;
};

// en_cb_threat_complete_req
struct MSG_CB_THREAT_COMPLETE_REQ : public MSG_COMBAT
{

};

// en_cb_threat_complete_ok
struct MSG_CB_THREAT_COMPLETE_OK : public MSG_COMBAT
{

};

// en_cb_threat_complete_error
struct MSG_CB_THREAT_COMPLETE_ERROR : public MSG_COMBAT
{
	u_char			ucError;
};

// en_cb_threat_complete_notice
struct MSG_CB_THREAT_COMPLETE_NOTICE : public MSG_COMBAT
{
	u_char			ucNewOwnerSide;		// 0:defender 1:attacker

	u_char			ucGroupType;
	int				iIndex;
};

// en_cb_info_current_info
struct MSG_CB_INFO_CURRENT_INFO : public MSG_COMBAT
{
	u_char			ucCurrStatus;		// EN_COMBAT_STATUS

	u_char			ucGroupType;		// ���� �ӽ� ������
	int				iindex;

	u_short			usElapsedTime;

	__combat_side_info	Defender;

	u_char				ucAttackerCount;
	__combat_side_info	Attackers[max_count_of_challenge_list];
};

#define MAX_CHAT_LENGTH			81			// Zone_chat���� ���� DEFINE

// en_cb_etc_loud_chat
struct MSG_CB_ETC_LOUD_CHAT : public MSG_COMBAT
{
	u_char			ucChat;				// not used
	
	char			cCharName[13];

	char			msg[MAX_CHAT_LENGTH];
};

enum
{
	en_cb_loud_error_not_master = 0,		// �Ѵ����� �ƴϴ�
	en_cb_loud_error_over_count,			// ��ħ Ƚ���� �Ѿ���
	en_cb_loud_error_not_time,				// ��ħ ��� �Ұ� �ð�
};

struct MSG_CB_ETC_CANNOT_LOUD : public MSG_COMBAT
{
	u_char			ucErrorCode;
};

// en_cb_etc_notice
struct MSG_CB_ETC_NOTICE : public MSG_COMBAT
{
	u_char			ucChat;
	
	char			msg[MAX_CHAT_LENGTH];
};

// en_cb_dead_penalty
struct MSG_CB_DEAD_PENALTY : public MSG_COMBAT
{
	u_char			ucDeadCountInCombat;
	u_short			usLeftCombatPenalryTime;
};

struct MSG_CB_THREATPOINT_SET : public MSG_COMBAT
{
	u_short			usThreatPoint;
};

// ���� ����

// �ٸ� �ܰ��� ����
struct _PerOrgRecord
{
	int				iAllyIndex;
	char			vsOrgName[_XDEF_ORGANIZATIONNAMELENGTH+1];
	char			vsAllyName[_XDEF_ORGANIZATIONNAMELENGTH+1];
	unsigned short	usWinCount;
	unsigned short	usLoseCount;
	unsigned short	usPercentageOfWin;
};

// �Ѱ��� ��ŷ ����
struct _OneRank
{
	char			charName[13];		// ĳ���� �̸�
	short			sRank;
	short			sCombatPoint;		// ���� ����: �¼�-�м�
	unsigned int	uiExp;				// ����ġ
	int				iHonor;				// ��
	unsigned int	uiMoney;			// �����
};

// en_cb_record_pkt_info_req = 80			// 80 �ڱ���� �����Ȳ ���� ��û
struct MSG_CB_RECORD_INFO_REQ : public MSG_COMBAT
{
	int				index;					// ������ �ε��� - ��ȸ�� ������ �ε����� �ش�
};

// en_cb_record_pkt_info_res				// 81 �ڱ���� �����Ȳ ���� ����
struct MSG_CB_RECORD_INFO_RES : public MSG_COMBAT
{
	u_short			usEntryCount;			// ����� ���� Ƚ��

	u_short			usWinCount;
	u_short			usLoseCount;
	u_short			ucCombatCount;

	u_short			usCumulationPercentage;	// ���� �·�

	u_short			usManorIndex;

	_sND_TimeUnit	lastManorCombatDay;		// ���������� ġ�� ����� ��

	u_short			usPerOrgRecordCount;
	_PerOrgRecord	_vsOrgRecordList[100];	// Ÿ�ܰ��� ���� ���
};

// en_cb_record_pkt_add_combat				// 82 ����� �� ������ ���� ����
struct MSG_CB_RECORD_COMBAT_ADD : public MSG_COMBAT
{
	_PerOrgRecord	_vsOrgRecord;
};

// en_cb_record_pkt_pvp_award				// 83 ����� �⿩ ����
struct MSG_CB_RECORD_PVP_AWARD : public MSG_COMBAT
{
	short			sPvPPoint;			// �¼� - ��������
	int				iFame;				// ��
	u_int			uiMoney;			// �����
	short			sRank;				// �ռ�: 0�̸� ��� ����, 1-10�� ��� Ư�� ���� ����
};

// en_cb_record_pkt_manor_rank				// 84 ����� �¸��� �������� ����
struct MSG_CB_RECORD_RANK : public MSG_COMBAT
{
	_OneRank		_rankList[10];
};

// en_cb_record_pkt_winner_award
struct MSG_CB_RECORD_WINNER_AWARD : public MSG_COMBAT
{
	short			sRank;				// ����
	short			sCombatPoint;		// ��������
	u_int			uiExp;				// ����ġ
	int				iHonor;				// ��
	u_int			uiMoney;			// �����
};
#pragma once

struct _sHeader;

#define MSG_NO_MASTER_AND_PUPIL                                (166)                   // MSG_MP_

// ���� ���� �ִ밪. 0���� 4����. �� �ټ� �ܰ��.
#define NDD_MP_MAX_MASTER_LEVEL			(4)

// �ִ� ���ڼ�
#define NDD_MP_MAX_PUPIL_COUNT			(4)

// �⺻, Ȯ�� ������ ų�� ī��Ʈ
// ��ũ��Ʈ���� ���ǵ��� ������, �Ʒ� ���ڸ� ����. 
#define NDD_MP_BASE_KILLCOUNT			(100U)
#define NDD_MP_EXTEND_KILLCOUNT			(1000U)

// ��ȣ�� �ִ� ����.
#define NDD_MP_MASTERNICK_LENGTH		(36)	// NULL ������.
#define NDD_MP_MAX_POINT				(1000000000U)	// 10�� �̴�. ����� 0x8fffffff �� ���� �ʵ��� �� ��.
 

struct MSG_MASTER_AND_PUPIL  
{
	_sHeader	header;
	u_char	ucMessage;
	u_char	ucMP_Mode;                   // EN_CG_MP_PACKET_MODE
};

enum EN_CG_MP_PACKET_MODE	// CG = Client <=> Game Server. MP = Master&Pupil
{
	// 0 - 30 : ���� �� ���� ��Ŷ.
	en_cg_mp_mode_info_my_master=0,				// ���� ���� ����. �ʵ� ���Խ� ����.
	en_cg_mp_mode_info_req=1,					// ���� ���� ���� ��û. �ֺ����� ������.
	en_cg_mp_mode_info_res=2,					// req �� ��������, ���� ������ ��. 

	// ���� ������Ʈ
	en_cg_mp_mode_update_point=3,				// ���� �� ������ ����Ʈ ����.
	en_cg_mp_mode_update_kill_count=4,			// ų ī��Ʈ ������Ʈ
	en_cg_mp_mode_update_master_level=5,		// ������ ���� ���� ��Ŷ.
	en_cg_mp_mode_udpate_position=6,			// ��1,2,3,4, ������ ���� �ٲٱ�.
	en_cg_mp_mode_master_info_created=7,		// ���� ������ �����Ǿ���.( ����Ʈ : rank 1 ������. )
	 en_cg_mp_mode_update_con_state=8,			// ������ ���� ���� ������Ʈ

	en_cg_mp_mode_bonus_setting=10,				// ������ ���� On/Off
	en_cg_mp_mode_no_mp_info=11,				// ���� ���谡 ����.

	en_cg_mp_mode_change_master_nick_req=20,	// ���� ��û.
	en_cg_mp_mode_change_master_nick_res=21,	// ��ȣ ������ �˷��ش�.
	en_cg_mp_mode_update_master_nick=22,		// ���� �Ǵ� �ֺ��� �˸���.
	

	en_cg_mp_mode_info_error=50,				// Info : error message.


	// 51 - 80 : ���� ���� �α� ���� ��Ŷ.
	en_cg_mp_mode_cg_apply_for_pupil=51,		// ���� ��û�� GS �� ���� : ���ڰ� �Ǳ� ���ϴ� ĳ���Ͱ� ����.
	en_cg_mp_mode_gc_apply_for_pupil=52,		// ���� ��û�� ���� : ������ ����.
	en_cg_mp_mdoe_answer_apply_from_master=53,	// ���� : ���� ��û�� ���� ����.
	en_cg_mp_mode_answer_apply_no_to_pupil=54,	// ���ڰ� �� ĳ���Ϳ��� ������ ������. ( yes �� �������� ���, ������ complete_join_pupil �� ����. )
	en_cg_mp_mode_complete_join_pupil=55,		// ���ڰ� �Ƿ��� ĳ���Ϳ�����, ���� �߰��� �Ϸ�Ǿ����� �˸�. 
	en_cg_mp_mode_add_new_pupil=56,				// ���ο� ���ڰ� �߰��Ǿ����� ���� �� �ٸ� ���ڿ��� �˸�.
	en_cg_mp_mode_new_master_simple_info=57,	// ��û�� 1�� ��������, GS ���� �ٷ� ������ 
	
	en_cg_mp_mode_apply_error=80,				// ��û ������ ��� ����.

	// 81 - 110 : �Ĺ� �� �ϻ�.
	en_cg_mp_mode_strike_pupil_req=81,			// ���� �Ĺ� ��û
	en_cg_mp_mode_strike_result=82,				// ���� �� �ٸ� ���ڿ��� ��.
	en_cg_mp_mode_you_struck=83,				// �Ĺ� ���ڿ��� �Ĺ��� �˸�.

	en_cg_mp_mode_strike_error=90,				// �Ĺ� ����.

	en_cg_mp_mode_leave_req=91,					// �ϻ� ��û.
	en_cg_mp_mode_you_left=92,					// �ϻ꿡 ����.
	en_cg_mp_mode_left_pupil=93,				// ���ڰ� �ϻ��ߴ�. ���� �� �ٸ� ���ڿ��� ����.

	en_cg_mp_mode_left_error=100,				// �ϻ� ����.

	en_cg_mp_mode_ban_pupil_req=101,			// �����߹� ��û.
	en_cg_mp_mode_pupil_banned=102,				// �����߹�Ǿ���. ���� ������ �����ϸ�, �ٸ� ���ڿ��Ե� ����.
	en_cg_mp_mode_ban_error=110,				// �����߹� ����.

};

//////////////////////////////////////////////////////////////////////////

struct _sPacketPupil			// ��Ŷ�� ���̴� ���� ��Ʈ����.
{
	u_int				uiTotalPoint;
	u_int				uiCurPoint;
	u_char				ucSex;
	u_char				ucLevel;		//  
	u_char				ucMoonpa;		// 
	u_char				ucClass;
	u_char				ucClassGrade;
	u_char				ucPosition;		// 0 - ��1����, 1.2.3 - 2,3,4 ����.
	u_char				ucCurZone;		// �Ʒ� ConnectState �� 1 �϶��� �ǹ̰� ����.
	u_char				ucConnectState;	// 0 - �α׾ƿ�, 1 - ������.
	char				cName[en_charac_name_length+1];

};


//////////////////////////////////////////////////////////////////////////

// 0 - 30 : ���� �� ���� ��Ŷ.
//en_cg_mp_mode_info_my_master=0,				// ���� ���� ����. �ʵ� ���Խ� ����.
struct MSG_MP_INFO_MY_MASTER : public MSG_MASTER_AND_PUPIL
{
	int					iMasterIndex;

	u_char				ucMasterRank;

	u_int				uiTotalKillCount;
	u_int				uiMasterTotalPoint;
	u_int				uiMasterCurPoint;

	u_char				ucMasterCurZone;			// �Ʒ� ConnectState �� 1 �� ����, �Ǹ��� ����.
	u_char				ucMasterConnectState;		// 0 - �α׾ƿ�, 1 - ������.
	u_char				ucBonusSkillStep;

	u_char				ucSex;
	u_char				ucLevel;	
	u_char				ucMoonpa;
	u_char				ucClass;
	u_char				ucClassGrade;

	char				cMasterName[en_charac_name_length+1];
	char				cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];

	// ���δ� ������ ����, �Ʒ��δ� ���ڵ��� ����.
	_sPacketPupil		Pupils[NDD_MP_MAX_PUPIL_COUNT];
};

//en_cg_mp_mode_info_req=1,					// ���� ���� ���� ��û. �ֺ����� ������.
struct MSG_MP_INFO_REQ : public MSG_MASTER_AND_PUPIL
{
	int					iMasterIndex;		// ������ �ε���. 
};

//en_cg_mp_mode_info_res=2,					// req �� ��������, ���� ������ ��. 
struct MSG_MP_INFO_RES : public MSG_MASTER_AND_PUPIL 
{
	int					iMasterIndex;
	
	u_char				ucMasterLevel;

	u_char				ucMasterConnectState;		// 0 - �α׾ƿ�, 1 - ������.
	u_char				ucBonusSkillStep;

	u_char				ucSex;
	u_char				ucLevel;	
	u_char				ucMoonpa;
	u_char				ucClass;
	u_char				ucClassGrade;

	u_char				ucCountOfPupils;			// ���ڼ�.

	char				cMasterName[en_charac_name_length+1];

	char				cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];
};



// ���� ������Ʈ
//en_cg_mp_mode_update_point=3,				// ���� �� ������ ����Ʈ ����.
struct MSG_MP_UPDATE_POINT : public MSG_MASTER_AND_PUPIL
{
	u_int				uiMasterTotalPoint;
	u_int				uiMasterCurPoint;

	u_char				where;			// 0 -

	u_int				uiTotalPoint;
	u_char				uiCurPoint;
};

// en_cg_mp_mode_update_kill_count=4,			// ų ī��Ʈ ������Ʈ
struct MSG_MP_UPDATE_KILL_COUNT : public MSG_MASTER_AND_PUPIL
{
	u_int				uiMyKillCount;			// 
	u_int				uiTotalKillCount;		// 
};

//en_cg_mp_mode_update_master_level=5,		// ������ ���� ���� ��Ŷ.
struct MSG_MP_UPDATE_MASTER_LEVEL : public MSG_MASTER_AND_PUPIL
{
	u_char				ucMasterLevel;		// ���� ���� ������Ʈ.
};

//en_cg_mp_mode_udpate_position=6;			// ��1,2,3,4, ������ ���� �ٲٱ�.
struct MSG_MP_UPDATE_POSITION : public MSG_MASTER_AND_PUPIL
{
	// name1 �� position1 �� �ǰ�, name2 �� position2 �� �Ǿ���.
	char				cName1[en_charac_name_length+1];
	u_char				ucPosition1;

	char				cName2[en_charac_name_length+1];
	u_char				ucPosition2;
};

// en_cg_mp_mode_master_info_created=7,		// ���� ������ �����Ǿ���.( ����Ʈ : rank 1 ������. )
struct MSG_MP_MASTER_INFO_CREATED : public MSG_MASTER_AND_PUPIL
{
	int					iMasterIndex;
	
	u_char				ucMasterLevel;

	u_char				ucSex;
	u_char				ucLevel;	
	u_char				ucMoonpa;
	u_char				ucClass;
	u_char				ucClassGrade;
};

// en_cg_mp_mode_update_con_state=8,			// ������ ���� ���� ������Ʈ
struct MSG_MP_UPDATE_CON_STATE : public MSG_MASTER_AND_PUPIL
{
	u_char				ucConState;		// 0 - not, 1 - field, 2 - server move.
	u_char				ucZone;
	char				cName[en_charac_name_length+1];
};


//en_cg_mp_mode_bonus_setting=10,				// ������ ���� On/Off
struct MSG_MP_BONUS_SETTING : public MSG_MASTER_AND_PUPIL
{
	u_char				ucBonusSkillStep;			// ���� �� ���ʽ� 

	u_char				ucApplyMasterEffect;		// 0 - Off, 1 - On.
};


//en_cg_mp_mode_no_mp_info=11,				// ���� ���谡 ����.
struct MSG_MP_NO_MP_INFO : public MSG_MASTER_AND_PUPIL
{
	
};


// en_cg_mp_mode_change_master_nick_req=20,	// ���� ��û.
struct MSG_MP_CHANGE_MASTER_NICK_REQ : public MSG_MASTER_AND_PUPIL
{
	char				cNick[NDD_MP_MASTERNICK_LENGTH+1];
};


// en_cg_mp_mode_change_master_nick_res=21,	// ��ȣ ������ �˷��ش�.
// ��û�� ����(����)���Ը� ����, ���ڵ鿡�Դ� MSG_MP_UPDATE_MASTER_NICK �� ����. 
struct MSG_MP_CHANGE_MASTER_NICK_RES : public MSG_MASTER_AND_PUPIL
{
	u_char				ucSuccessFail;		// 0 - Fail, 1 - Success
	
	char				cNick[NDD_MP_MASTERNICK_LENGTH+1];
};


// en_cg_mp_mode_update_master_nick=22,		// ���� �Ǵ� �ֺ��� �˸���.
struct MSG_MP_UPDATE_MASTER_NICK : public MSG_MASTER_AND_PUPIL
{
	int					iMasterIndex;
	char				cNick[NDD_MP_MASTERNICK_LENGTH+1];
};


//en_cg_mp_mode_info_error=50,				// Info : error message.
struct MSG_MP_INFO_ERROR : public MSG_MASTER_AND_PUPIL
{
	u_char				ucErrorCode;			// ...
};



//////////////////////////////////////////////////////////////////////////
// 51 - 80 : ���� ���� �α� ���� ��Ŷ.
//en_cg_mp_mode_cg_apply_for_pupil=51,		// ���� ��û�� GS �� ���� : ���ڰ� �Ǳ� ���ϴ� ĳ���Ͱ� ����.
struct MSG_MP_CG_APPLY_FOR_PUPIL : public MSG_MASTER_AND_PUPIL
{
	// �ε����� �� �� �����Ƿ�, �̸����� ��û�Ѵ�.
	char				cMasterName[en_charac_name_length+1];
};

// en_cg_mp_mode_gc_apply_for_pupil=52,		// ���� ��û�� ���� : ������ ����.
struct MSG_MP_GC_APPLY_FOR_PUPIL : public MSG_MASTER_AND_PUPIL
{
	u_char				ucLevel;
	u_char				ucMoonpa;
	u_char				ucClass;
	u_char				ucClassGrade;

	char				cMasterName[en_charac_name_length+1];
	char				cFromPupil[en_charac_name_length+1];
};

// en_cg_mp_mdoe_answer_apply_from_master=53,	// ���� : ���� ��û�� ���� ����.
struct MSG_MP_ANSWER_APPLY_FROM_MASTER : public MSG_MASTER_AND_PUPIL
{
	u_char				ucNoYes;			// 0 - No, 1 - Yes.
	char				cNewPupil[en_charac_name_length+1];
};

//en_cg_mp_mode_answer_apply_no_to_pupil=54,	// ���ڰ� �� ĳ���Ϳ��� ������ ������. ( yes �� �������� ���, ������ complete_join_pupil �� ����. )
struct MSG_MP_ANSWER_APPLY_NO_TO_PUPIL : public MSG_MASTER_AND_PUPIL
{
	char				cFromMaster[en_charac_name_length+1];
};

// en_cg_mp_mode_complete_join_pupil=55,		// ���ڰ� �Ƿ��� ĳ���Ϳ�����, ���� �߰��� �Ϸ�Ǿ����� �˸�. 
struct MSG_MP_COMPLETE_JOIN_PUPIL : public MSG_MASTER_AND_PUPIL
{
};

//en_cg_mp_mode_add_new_pupil=56,				// ���ο� ���ڰ� �߰��Ǿ����� ���� �� �ٸ� ���ڿ��� �˸�.
struct MSG_MP_ADD_NEW_PUPIL : public MSG_MASTER_AND_PUPIL
{
	_sPacketPupil		NewPupil;
};

//en_cg_mp_mode_new_master_simple_info=57 // ��û�� 1�� ��������, GS ���� �ٷ� ������ 
struct MSG_MP_NEW_MASTER_SIMPLE_INFO : public MSG_MASTER_AND_PUPIL
{
	char					cMasterName[en_charac_name_length+1];

	u_char					ucMasterLevel;
	u_char					ucMasterMoonpa;
	u_char					ucMasterClass;
	u_char					ucMasterClassGrade;
	u_char					ucCurZone;

	char					cMasterNick[NDD_MP_MASTERNICK_LENGTH+1];
};

// en_cg_mp_mode_apply_error=80,				// ��û ������ ��� ����.
enum EN_MP_APPLY_ERROR		// MSG_MP_APPLY_ERROR
{
	en_mp_apply_no_error=0,						// 0 ���� �׳� ����. �� �����δ� ��Ŷ�� ���� �ʴ´�.
	en_mp_apply_error_wait=1,					// 10 �� ���� : ����� �ٽ� �õ�.
	en_mp_apply_error_pupil_level_limit=2,		// 73 ���� �̻� ĳ���ʹ� ���ڰ� �� �� �����ϴ�.
	en_mp_apply_error_level_diff_limit=3,		// ���°� ���ڴ� 13���� �̻� ���̰� �����մϴ�.
	en_mp_apply_error_already_has_master=4,		// �̹� �ٸ� ������ �ֽ��ϴ�.
	en_mp_apply_error_pupils_over=5,			// �� �̻� ���ڸ� ���� �� �����ϴ�.
	en_mp_apply_error_you_are_master=6,			// ���� �����̴�.
};

struct MSG_MP_APPLY_ERROR : public MSG_MASTER_AND_PUPIL
{
	// 
	u_char				ucErrorCode;
};


//////////////////////////////////////////////////////////////////////////
// 81 - 100 : �Ĺ� �� �ϻ�.
//en_cg_mp_mode_strike_pupil_req=81,			// ���� �Ĺ� ��û
struct MSG_MP_STRIKE_PUPIL_REQ : public MSG_MASTER_AND_PUPIL
{
	// �Ĺ��Ϸ��� ������ �̸��� �ִ´�.
	char				cPupilName[en_charac_name_length+1];
};

//en_cg_mp_mode_strike_result=82,				// ���� �� �ٸ� ���ڿ��� ��.
struct MSG_MP_STRIKE_RESULT : public MSG_MASTER_AND_PUPIL
{
	char				cStrikeCName[en_charac_name_length+1];

	u_int				uiMasterTotalPoint;
	u_int				uiMasterCurPoint;

	u_char				ucBonusSkillStep;

	u_char				ucChangedCount;		// ChangedPosition �� ucChangedCount ������ ����. �������� ��ä��.
											// �Ĺ����� ��1,2������ ��ȭ�� ���� ��쿣, 0 �̴�.

	struct
	{
		char			cPupilName[en_charac_name_length+1];
		u_char			ucNewPosition;		// 2 - ��1����.
	} ChangedPosition[NDD_MP_MAX_PUPIL_COUNT-1] ;
};

// en_cg_mp_mode_you_struck=83,				// �Ĺ� ���ڿ��� �Ĺ��� �˸�.
struct MSG_MP_YOU_STRUCK : public MSG_MASTER_AND_PUPIL
{
	// 	
};

// en_cg_mp_mode_strike_error=90,				// �Ĺ� ����.
struct MSG_MP_STRIKE_ERROR : public MSG_MASTER_AND_PUPIL
{
	u_char				ucErrorCode;

	enum
	{
		en_mp_strike_error_no=0,			// �ǹ� ����.
		en_mp_strike_error_not_master=1,	// �Ĺ� �ڰ��� ����( ������ �ƴ� )
		en_mp_strike_error_pupil_not_found=2,	// �Ĺ��� ���ڰ� ����Ǿ� �־�� �Ѵ�.
		en_mp_strike_error_system_error=255,	// �� �� ���� ������.
	};
};

//en_cg_mp_mode_leave_req=91,					// �ϻ� ��û.
struct MSG_MP_LEAVE_REQ : public MSG_MASTER_AND_PUPIL
{
	//
};

//en_cg_mp_mode_you_left=92,					// �ϻ꿡 ����.
struct MSG_MP_YOU_LEFT : public MSG_MASTER_AND_PUPIL
{
	
};

//en_cg_mp_mode_left_pupil=93,				// ���ڰ� �ϻ��ߴ�. ���� �� �ٸ� ���ڿ��� ����.
struct MSG_MP_LEFT_PUPIL : public MSG_MASTER_AND_PUPIL
{
	char				cLeftCName[en_charac_name_length+1];
	u_int				uiMasterTotalPoint;
	u_int				uiMasterCurPoint;

	u_char				ucBonusSkillStep;

	u_char				ucChangedCount;		// ChangedPosition �� ucChangedCount ������ ��ȿ. �������� ��ä��.
											// �Ĺ����� ��1,2������ ��ȭ�� ���� ��쿣, 0 �̴�.

	struct
	{
		char			cPupilName[en_charac_name_length+1];
		u_char			ucNewPosition;		// 2 - ��1����.
	}ChangedPosition[NDD_MP_MAX_PUPIL_COUNT-1] ;
};


//en_cg_mp_mode_left_error=100,				// �ϻ� ����.
struct MSG_MP_LEFT_ERROR : public MSG_MASTER_AND_PUPIL
{
	u_char				ucErrorCode;
	
	enum
	{
		en_mp_leave_error_no=0,				// �ǹ� ����.
		en_mp_leave_error_not_pupil=1,		// �ϻ��� �� ����. ���ڰ� �ƴ�.
		en_mp_leave_error_system_error=255,	// �� �� ���� ������.
	};
};


//	en_cg_mp_mode_ban_pupil_req=101,			// �����߹� ��û.
struct MSG_MP_BAN_PUPIL_REQ : public MSG_MASTER_AND_PUPIL
{
	char				cPupilName[en_charac_name_length+1];
};


//	en_cg_mp_mode_pupil_banned=102,				// �����߹�Ǿ���. ���� ������ �����ϸ�, �ٸ� ���ڿ��Ե� ����.
struct MSG_MP_PUPIL_BANNED : public MSG_MASTER_AND_PUPIL
{
	// �ڽ��̸�, ���� ������ �ʱ�ȭ�� ��.
	char				cPupilName[en_charac_name_length+1];
};

// en_cg_mp_mode_ban_error=110,				// �����߹� ����.
struct MSG_MP_BAN_ERROR : public MSG_MASTER_AND_PUPIL
{
	u_char				ucErrorCode;	// 
};

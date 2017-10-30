#pragma once

// Skill, Quick slot
// Main = 12
// 96 - 103

struct _sHeader;
struct _sCharac_Skill;

#ifdef _XDEF_ADD_MAX_LEARNSKILL_080522_KUKURI //Author : ����� //breif : ������ ��ų�� 80���� �þ
    #define _COUNT_LEARN_SKILL_				80
#else
    #define _COUNT_LEARN_SKILL_				60
#endif

#define MSG_NO_SKILL_INFO				96		// ĳ������ ���� ����
#define MSG_NO_SKILL_SELECT				97		// ���� ���� �޼���
#define MSG_NO_SKILL_CTRL_REQ			98		// ������ ���ڴ� C->S
#define MSG_NO_SKILL_CTRL				98		// ���� ����

#define MSG_NO_SKILL_INFO_MASTERY		99		// �����͸� ��ų�� ����
#define MSG_NO_SKILL_INFO_COOLTIME		100		// ��Ÿ�� ����

#define MSG_NO_SKILL_ACTION				101     // �ൿ ��ų�� ��� : ����ȿ�� ��� ���� ���� �Ѵ�.
#define MSG_NO_SKILL_INFO_REQ           102     // ��ų ���� ��û.

#define MSG_NO_MASTERY_POINT_USE		108		// �����͸� ����Ʈ ��� ��Ŷ
#define MSG_NO_MASTERY_POINT_RESULT		108		// ���� ����Ʈ ���� ����� ��ų���� ����Ʈ ��


struct MSG_SKILL_INFO
{
	_sHeader	header ;
	u_char		ucMessage ;
	
	u_char		ucLearnSkillCount ;		// ��� ��ų�� ����
	
	_sCharac_Skill skill_info[_COUNT_LEARN_SKILL_] ;
} ;

struct MSG_SKILL_SELECT
{
	_sHeader			header;
	u_char				ucMessage;

	char				cSelectLevel;			// ���� ����
	short				sSkillID;				// ���� ������ ���̵�
};

enum 
{
	en_skill_ctrl_req_learn_from_npc=0,		// 0	npc �κ��� ��ų�� ����.
	en_skill_ctrl_req_step_up,				// 1	��ų�� �ܰ踦 ��½�Ų��.
	en_skill_ctrl_req_passive,				// 2	�нú� ��ų�� on/off ��Ų��.
	en_skill_ctrl_req_reserve1,				// 3	
	en_skill_ctrl_req_reserve2,				// 4
	en_skill_ctrl_req_del_skill,			// 5	// ��ų�� ���� ��û
	en_skill_ctrl_req_complete_training		// 6	��ų�� �Ʒ� ���¸� �Ʒ� �Ϸ� ���·� �����Ѵ�.
} ;

union __how_to_skill
{
	struct 
	{										// for Mode 0 : ���� ��û.
		u_short		usNpcID ;
		u_char		ucIndexinList ;			// NPC ��ũ��Ʈ�� �p ��° �ε����ΰ�. Zero-Base.
		short		sSkillID ;				// index - skill id pair.
		u_char		ucClassIndex ;			// Ŭ���� �ε���. ������/ġ����.. ��� �����ε� ��ȣ
	} S_from_npc ;
	
	struct  
	{										// Mode 1 : �ܰ� ��� ��û
		u_char		ucSkillSlotIndex;		// ��ų ���Կ��� �ε���
	} S_step_up;
	
	struct 
	{										// Mode 5 : ����� ��û.
		u_char		ucSkillSlotIndex ;		// ��ų ���Կ����� �ε���
	} S_delete_skill ;

	struct {
		u_char		ucLearnSkillIndex ;		// ��ų ���������� �ε���.
		short		sSkillID ;				// ��ų�� ���̵� ( Ȯ�ο� )
	} S_complete_learn ;
	
	struct {
		u_char		ucLearnSkillIndex ;		// ��� �нú� ��ų�� ���� ��ȣ.
		short		sSkillID ;				// �������� ����ϱ� ����, �нú� ��ų�� ���̵� �ִ´�.
		
		char		cSelectStep ;			// ���� �ܰ�( ���߿� ������ �ȴٸ� �ʿ��� �� )
		char		cOnOff ;				// 0 : Off, 1 : On
	} S_Passive_OnOff ;
};

struct MSG_SKILL_CTRL_REQ
{
	_sHeader		header;
	u_char			ucMessage;

	char			cMode;					// 0 : NPC�κ��� ���� 1..

	__how_to_skill	howto;
};

// MSG_SKILL_CTRL : ��ų �߰� Ȥ�� ������Ʈ ��Ŷ
enum 
{
	en_skill_ctrl_mode_add=0,				//	0	��ų �߰� �϶�.( Learn�� ���� �Ǵ� �������� �߰�. )
	en_skill_ctrl_mode_learn_fail,			//	1	���� ����.
	en_skill_ctrl_mode_step_up_fail,		//	2	���� �� ���� ����
	en_skill_ctrl_mode_delete,				//	3	������...
	en_skill_ctrl_mode_update_step,			//	4	��ų ���� ������Ʈ : �������� �ڵ����� Ȥ�� ��û�� ����.
	en_skill_ctrl_mode_update_exp,			//	5	����ġ 
	en_skill_ctrl_mode_update_mission,		//	6	 
	en_skill_ctrl_mode_complete_learn,		//	7	 
	en_skill_ctrl_mode_update_training,		//	8	���� �Ϸ� ����.
	en_skill_ctrl_mode_down_exp,			//	9   ��ų ����ġ �谨 ����Ʈ���� ����.
	en_skill_ctrl_mode_off_use,				//	10  ��ų�� �Ͻ������� ���� ���ϰ� �Ѵ�.  
	en_skill_ctrl_mode_on_use,				//	11  ��ų�� �ٽ� ����ϰ� �Ѵ�. 
	en_skill_ctrl_mode_passive,				//	12	�нú� ��ų�� ���¸� �����϶�.	
	en_skill_ctrl_mode_cooltime,			//	13	��Ÿ�� ������Ʈ

	en_skill_ctrl_mode_bookinfo,			//	14	������ ���� : �ִٸ�, ������ ���ٸ�, ������ �ʴ´�.
	en_skill_ctrl_mode_bookend,				//	15	�������� �ð��� ������.
	en_skill_ctrl_mode_reset_mastery,		//  16	���Ǹ� ��� ������.	

	en_skill_ctrl_mode_del_bonus_hyperrun,	//  17  ���ʽ� ����� �����.(���Թ�ȣ�� 250�� ���) 
} ;

union __skill_ctrl
{
	struct									// Mode 0
	{			
		u_char		ucLearnSkillIndex ;		// ��ų ���� ��ȣ.
		short		sAddSkillID ;			// �߰��Ǵ� ���� ���̵�.
		char		cStep ;					// �ܰ� ����.
		u_char		ucSkillState ;			// 0 : ��� ���� 1 : ���� �ʿ�.
	} S_skill_add ;
	
	struct									// Mode Learn Fail, Step up fail
	{			
		u_char		ucFailReason ;			// ���� ����.
	} S_learn_fail ;
	
	struct									//  ���� ��û
	{
		u_char		ucLearnSkillIndex ;		// ������ ��ų ������ �ε���.
	} S_skill_delete ;
	
	struct									// �ܰ� ��� ���. Ȥ�� ���������� ����. 
	{					
		u_char		ucLearnSkillIndex ;
		char		cStep ;
	} S_skill_update_step ;
	
	struct	
	{					
		u_char		ucLearnSkillIndex ;		//
		int			iCurSkillExp ;			// ���� ����ġ
	} S_skill_update_exp ;
	
	struct
	{					
		u_char		ucLearnSkillIndex ;		//
		char		cCurMission ;			// �̼� ���� ī��Ʈ	
	} S_skill_update_mission_count ;

	struct {
		u_char		ucLearnSkillIndex ;		// ��ų ���������� �ε���.
		short		sSkillID ;				// ��ų�� ���̵� ( Ȯ�ο� )
	} S_complete_learn ;
	
	struct {
		u_char		ucLearnSkillIndex ;
		u_char		ucSkillState ;			// 0 : ��� ���� 1 : ���� �ʿ�.
	} S_update_training ;

	struct {
		u_char		ucLearnSkillIndex ;		// ��� �нú� ��ų�� ���� ��ȣ.
		short		sSkillID ;				// �������� ����ϱ� ����, �нú� ��ų�� ���̵� �ִ´�.
		
		char		cSelectStep ;			// ���� �ܰ�( ���߿� ������ �ȴٸ� �ʿ��� �� )
		char		cOnOff ;				// 0 : Off, 1 : On
	} S_Passive_OnOff ;

	struct {
		short		skillID ;				// �ش� ��ų 
		u_short		usTimeUpdate ;			// ���� �ð� ������Ʈ
	} S_Cooltime_Update ;

	struct {
		short		sBookID ;				// ������ ���̵� : 0�̸� ����.
		u_int		uiLeftSecond ;			// ���� �ð� : 0�̸� ����.
	} S_Bookinfo ;

	// en_skill_ctrl_mode_reset_mastery
	struct {
		short		sLeftMasteryPoint;		// ���� ����Ʈ ������Ʈ
	} S_Mastery_Reset;
	

	// en_skill_ctrl_mode_del_bonus_hyperrun
	struct {
		u_char		slot;
	} S_Del_Bonus_Hyperrun;
} ;

//#define MSG_NO_SKILL_CTRL				99		// ���� ����.
struct MSG_SKILL_CTRL
{
	_sHeader		header ;
	u_char			ucMessage ;
	
	char			cMode ;					// ctrl mode :
	
	__skill_ctrl	ctrl ;
} ;


// ���� ���� :

enum
{
	_SKILL_CAN_LEARN_SUCCESS_ =	0,		// ���� �ִ�.
	_SKILL_CAN_LEARN_CONTRIBUTION_,		// �⿩�� ����.
	_SKILL_CAN_LEARN_LEVEL_, 			// ���� �ܰ� ���ڸ�.
	_SKILL_CAN_LEARN_PRE1_,				// ���� ���� 1 ����.
	_SKILL_CAN_LEARN_PRE2_,				// ���� ���� 2 ����. 
	_SKILL_CAN_LEARN_CLAN_,				// ���� ���� �ȸ���.
	_SKILL_CAN_LEARN_SKILL_INDEX_,		// id�� �ش��ϴ� ��ų�� ����.
	_SKILL_CAN_LEARN_SKILL_FULL_,		// ���̻� ��ų ������ ����.
	_SKILL_CAN_LEARN_SKILL_ALREADY_,	// �̹� ������� �ִ�. 
	_SKILL_CAN_LEARN_NO_MONEY_,			// ���� ����. 
	_SKILL_CAN_LEARN_STEP_FULL_,		// �ܰ谡 12���� �����ߴ�.
	_SKILL_CAN_LEARN_MORE_SKILLEXP_,	// ��ų ����ġ�� �� �ʿ��ϴ�.
	_SKILL_CAN_LEARN_GRADE_,			// ��å ����� ���ڶ��
} ;


// Mastery Skill ==============================================================

// �������� ������ �����͸� ��ų�� ����Ʈ ����.
struct _sChr_Mastery_Info
{
	short		skillID ;			// �����͸� ��ų�� ���̵�
	char		cPoint ;			// ���� ����Ʈ. 0�� �� ����. 0�̸� ���� ����.
	char		cReserve ;			// ����. �Ⱦ���.
} ;


//#define MSG_NO_SKILL_INFO_MASTERY		99		// �����͸� ��ų�� ����
#define _COUNT_MASTERY_SKILL_			96			// �����͸� ��ų�� ����.

struct MSG_SKILL_INFO_MASTERY
{
	_sHeader	header ;
	u_char		ucMessage ;
	
	u_char		ucMasteryCount ;		// ����Ʈ ���� �����͸� ��ų�� ����
	
	_sChr_Mastery_Info	mastery_info[_COUNT_MASTERY_SKILL_] ;	// DB ���� _sDB_Mastery_Info ��. 
} ;


//#define MSG_NO_MASTERY_POINT_USE		108		// �����͸� ����Ʈ ��� ��Ŷ.
struct MSG_MASTERY_POINT_USE
{
	_sHeader			header ;
	u_char				ucMessage ;			// 1����Ʈ�� ����ϴ� ��Ŷ.
	
	u_char				ucPageNo : 3 ;		// 0,1,2�� ������ ��ȣ.
	u_char				ucPadding1 : 5 ;	//
	
	short				sMasteryID ;		// �����͸� ��ų�� ���̵�.
} ;

//#define MSG_NO_MASTERY_POINT_RESULT		108		// ���� ����Ʈ ���� ����� ��ų���� ����Ʈ ��.
struct MSG_MASTERY_POINT_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;			// 
	
	u_char				ucPageNo : 3 ;		// 
	u_char				ucFail : 5 ;		// 0 �� ����, 1�� ���� �� ����.
	
	short				sMasteryID ;
	char				ucMasteryStep ;		// ���� �����.
	
	short				sLeftMasteryPoint ;	// ���� �����͸� ����Ʈ.
} ;

//#define MSG_NO_SKILL_INFO_REQ                  102     // ��ų ���� ��û.
struct MSG_SKILL_INFO_REQ
{
	_sHeader	header ;
	u_char		ucMessage ;
};

#define _COUNT_COOL_TIME_		32

// ���� ���� ������ ��Ÿ�� ������ ���ϴ�. ( ���, ������  0���� �����ؼ� �����ϴ�. )
// �������� ������ ��Ÿ�� ����Ʈ
struct _sChr_Cool_Info
{
	short		skillID ;
	u_short		usLeftCoolTime ;		// ���� ��Ÿ��. �ʴ���.
} ;


struct MSG_SKILL_INFO_COOLTIME
{
	_sHeader	header ;
	u_char		ucMessage ;
	
	u_char		ucCoolTimeListCount ;		// ��Ÿ�� ������ ����. 0 �̸�, �Ľ� ��.
	
	_sChr_Cool_Info		cooltime[_COUNT_COOL_TIME_] ;
} ;


// ��� ��ų�� ��� :

enum EN_CANCEL_SKILL_INDEX 
{
	en_cancel_skill_cancel_mask=0,       // 0 �ڱ� ���� ���� ��û : MSG_ACTION_SKILL 
    en_cancel_skill_cancel_extra=1,      // 1 Extra ȿ������ ��� : slot = 0 ~ 9 : MSG_ACTION_SKILL_EXTRA_CANCEL
                                         // �켱 : 4�� ���Ը� ����ϵ��� �ϵ��ڵ��Ѵ�.
    en_cancel_skill_cannot=100,          // 100 : ����� �� ����. : MSG_CANCEL_SKILL_CANNOT
} ; 

//#define MSG_NO_SKILL_ACTION    101      // �ൿ ��ų�� ��� : ����ȿ�� ��� ���� ���� �Ѵ�.

struct MSG_CANCEL_SKILL                   // �ൿ ��ų ���� ��Ŷ
{
	_sHeader	header ;
	u_char		ucMessage ;
	u_char		ucMode ;                  // ���� EN_ACTION_SKILL_INDEX
} ; 

struct MSG_CANCEL_SKILL_EXTRA : public MSG_CANCEL_SKILL
{
	char		cSlotNo ;                 // 0 - 9 : ����� 4���� ���.
} ;

enum EN_CANCEL_SKILL_CANNOT_REASON 
{
	en_cannot_act_no_mask_effect=0,       // ���� ȿ�� ����.
	en_cannot_act_no_extra_effect,        // �ش� ����Ʈ�� ȿ�� ����.
} ; 

struct MSG_CANCEL_SKILL_CANNOT : public MSG_CANCEL_SKILL
{
	u_char		ucReason ;                      
} ;


//////////////////////////////////////////////////////////////////////////
// Quick slot and helper record

#define MSG_NO_QUICKSLOT                200     // ������ ���� �޽���.

/*////////////////////////////////////////////////////////////////////////

	_XQS_SLOTTYPE_NONE = 0,
	_XQS_SLOTTYPE_SKILL,
	_XQS_SLOTTYPE_ITEM,
	_XQS_SLOTTYPE_LIFESKILL				  
////////////////////////////////////////////////////////////////////////*/

struct __client_quickslot

{
	
	u_char      slotType;                        // _XQS_	
	short       skillID;	
	short       lifeSkillID;	
	char        itemCType;	
	short       itemSID;	
	u_short     invenNumber;     	
	u_char      reserved;                        // must be zero	
	u_char      reserved2;                       // must be zero	
};           // 12 bytes

enum EN_QUICKSLOT_MODE
{
	en_quickslotmode_allinfo=0,
	en_quickslotmode_update,
	en_quickslotmode_reset,
	
	en_helplog_allinfo=10,                   // ũ�Ⱑ �����Ƿ�, �Ѳ����� ���.
    en_helplog_update,
    en_helplog_reset,
};

struct MSG_QUICKSLOT
{	
	_sHeader    header;	
	u_char      ucMessage;						// MSG_NO_QUICKSLOT	
	u_char      ucQuickSlotMode;				//	
};


//allinfo - ó���� �ѹ�. ĳ���� ���� ���� ��? ������ ���� ���� ��? S->C
//update  - ���÷�. C->S
//reset   - ���÷�. C->S

#define _QUICKSLOT_COUNT_  30

struct MSG_QUICKSLOT_ALLINFO : public MSG_QUICKSLOT
{	
	__client_quickslot      m_QuickSlot[_QUICKSLOT_COUNT_];	
};

struct MSG_QUICKSLOT_UPDATE_UNIT : public MSG_QUICKSLOT
{	
	u_char					slotNumber;
	__client_quickslot      slotCont;
};

struct MSG_QUICKSLOT_RESET_UNIT : public MSG_QUICKSLOT
{	
	u_char					slotNumber;                     // ���� ���� �ε���.
};







//////////////////////////////////////////////////////////////////////////
// Help log

#define _HELPLOG_MAX_SIZE_                  (4*4)

struct MSG_HELPLOG_ALLINFO : public MSG_QUICKSLOT
{
       u_char				ucHelpLog[_HELPLOG_MAX_SIZE_];
       
	   MSG_HELPLOG_ALLINFO()
			: MSG_QUICKSLOT( /*sizeof(MSG_HELPLOG_ALLINFO), en_helplog_allinfo*/ )
       {
       }

};

struct MSG_HELPLOG_UPDATE : public MSG_QUICKSLOT
{
       u_char                ucHelpLog[_HELPLOG_MAX_SIZE_];

       MSG_HELPLOG_UPDATE()
			: MSG_QUICKSLOT( /*sizeof(MSG_HELPLOG_UPDATE), en_helplog_update*/ )
       {
       }

};
 
struct MSG_HELPLOG_RESET : public MSG_QUICKSLOT
{
       MSG_HELPLOG_RESET()
            : MSG_QUICKSLOT( /*sizeof(MSG_HELPLOG_RESET), en_helplog_reset*/ )
       {
       }
};

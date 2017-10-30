#pragma once

// Battle
// Main = 6, 7, 8
// 48 - 71

struct _sHeader;


#define MSG_NO_NPC_BUFF_REQ						48		// NPC ���� ��û ��Ŷ
#define MSG_NO_NPC_BUFF_RESULT					48		// NPC ���� ��� ��Ŷ

#define MSG_NO_B_ATKRES_FROM_MOB				50		// S->C :  ���Ͱ� ������ ����

#define MSG_NO_SELF_CHANNEL_START_REQUEST		64		// ���� ä�θ�( ������� ���� ) ��� ���� ��û.
#define MSG_NO_SELF_CHANNEL_CTRL				64		// ���� ä�θ�( ������� ���� ) ��� ���.
#define MSG_NO_SELF_CHANNEL_INFO				65

#define MSG_NO_REGION_DAMAGE					66		// ���� ������ ���

// �̴ϰ��� ��Ŷ ���� ��
#define MSG_NO_MINIGAME_START_REQ_C_S		51		// �̴ϰ��� ���� ��û
#define MSG_NO_MINIGAME_MISSION_STAGE_S_C	52		// ������ �ܰ� (���� �˸� �޽����ε� ����)
#define MSG_NO_MINIGAME_MAST_STAGE_C_S		52		// �Ϸ��� �ܰ� (���� ��û �޽����ε� ����)
#define MSG_NO_MINIGAME_FAIL_S_C			53		// ���� ��Ŷ
#define MSG_NO_MINIGAME_CTRL				54		// �㰡 Ȥ�� �Ұ�... S->C
#define MSG_NO_MINIGAME_RESULT				55		// ���.(�ΰ� ȿ���� ���� ���� ����. )

#define MSG_NO_PK_RECORD					58		// PK ����


// S->C ���Ͱ� ������ ����
//#define MSG_NO_B_ATKRES_FROM_MOB				34
// �������� ��Ŷ : ucAddPassiveType �� 0�� �ƴ� ���, ���� float 2 ���� ���� ��.
// 0�� ���, ��ġ���� ���� ����.
struct MSG_B_ATKRES_FROM_MOB
{
	_sHeader			header;
	u_char				ucMessage;
	
	u_short				usAttackerID;
	u_short				usDefenderID;

	u_char				ucToType : 1;			// 0 : ����   1 : ĳ����
	u_char				ucAtkInfo : 3;			// atkinfo : 0 for Hit, 1 Critical, 2 Evasion, 3 Block, 4 Shock
	u_char				ucAtkType : 2;			// ���Ͱ� � ������ ������ �ϴ°�

	u_char				ucAddPassiveType : 2;	// ���� �нú꿡 ���� �߰� ȿ��, 0:���� 1:��������

	short				sDamage;				// ���Ͱ� ���� ������
};

struct MSG_B_ATKRES_FROM_MOB_WITH_POS
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_short				usAttackerID ;
	u_short				usDefenderID ;
	
	// atkinfo : 0 for Hit, 1 Critical, 2 Evasion, 3 Block, 4 Shock, 5, 6, 7
	// 
	u_char				ucToType : 1 ;			// 0 ����, 1 ĳ����
	u_char				ucAtkInfo : 3 ;			// 0 ~ 7
	u_char				ucAtkType : 2 ;			// ���Ͱ� � ������ ������ �ϴ°�.
	u_char				ucAddPassiveType : 2 ;	// ���� �нú꿡 ���� �߰� ȿ�� : 0 - ����. 1 - �нú� ��ų �ߵ�.
	short				sDamage ;				// ���Ͱ� ���� ������.

	char				cPassiveTitle;			// �нú� ���
	float				fItemDropPosX ;			// �������� �ߵ��� �������� ������ ��ġ.
	float				fItemDropPosZ ;	
};

enum EN_MOB_PASSIVE_TITLE
{
	mob_passive_none = 0,			// ����
	mob_passive_release_arm,		// ���� ����
	mob_passive_steal_item,			// ������ ��Ż
	mob_passive_count
};

/* =====================================================================================
���� ������ ����� ������ �˸���.
===================================================================================== */

#define _COUNT_REGION_ATK_RESULT_			40	// ���� ������ ����� �ִ� 40�� ���� �� ����.

struct __regionAtkResult
{
	u_char			ucObjType : 4 ;			// 0 ����, 1 ĳ����, 2 �� ��Ÿ( ��,�ñ�,���... )
	u_char			ucMobDead : 1;			// 0 : alive, 1 : dead (���Ϳ��Ը� ��ȿ)
	u_char			ucResult : 3 ;			// atkinfo : 0 for Hit, 1 Critical, 2 Evasion, 3 Block, 4 Shock, 5, 6, 7
	
	char			cMobVitalRate ;			// ������ ����, ��ȿ. : ĳ���ʹ� �׳� �¾Ҵٴ� ������ ��.
											// �׸���, ĳ���Ϳ��Դ� life ��Ŷ�� ���� ���� ����.
	
	u_short			usID ;					// �������� ���̵�.
} ;

struct MSG_REGION_DAMAGE
{
	_sHeader			header ;			// ���̴� 6 + ucObjectCount * sizeof(__regionAtkResult) �� �ȴ�. 
	u_char				ucMessage ;
	
	u_char				ucAttackerType ;	// 0 ����, 1 ĳ����, 2 �� ��Ÿ( ��,�ñ�,���... )
	u_short				usAttackerID ;		// ���ͳ� ĳ������ ���̵�.
	
	short				sSkillID ;			// ���� ��ų�� ���̵�.
	
	u_char				ucObjectCount ;		// ���Ե� ��ü��.
	
	__regionAtkResult	Result[_COUNT_REGION_ATK_RESULT_] ;
} ;


// Self Channeling Attack.
//#define MSG_NO_SELF_CHANNEL_START_REQUEST		64		// ���� ä�θ�( ������� ���� ) ��� ���� ��û.
struct MSG_SELF_CHANNEL_START_REQUEST
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cMode;					// 0 : ���� ��û, 1 : ���, 2 : ����
	
	u_char				ucLearnSkillIndex ;		// ���� �ε����� ������
	short				skillID ;				// ���̵� ����. 
} ;

//#define MSG_NO_SELF_CHANNEL_START_CTRL	64		// ���� ä�θ�( ������� ���� ) ��� ���� ��û.
struct MSG_SELF_CHANNEL_CTRL		// �ڽſ��Ը� ��. �������� ����.
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cResultInfo ;		// 0 : �����϶�. 1 : �Ϸ�(Ÿ�Ӿƿ�) 
	// 2 : ���� � ���� ��� �Ǿ���.
	// 3 : �׷� ��ų ����(��ų ���̵� �ȸ���) 
	// 4 : ��ų�� ��� ���� ��ų�� �ƴϴ�. 
	
	short				skillID ;			// �� ��ų�� �����϶�.
} ;

//#define MSG_NO_SELF_CHANNEL					65
struct MSG_SELF_CHANNEL_INFO		// ������ �˷��ִ� ��Ŷ : �ڽ��� ����.
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cFlag ;			// 0 : ��ų ���� ����. 1 : ��ų ���� ����. 2: ������
	
	u_short				usID ;			// �ڽ��̸� ����.
	short				skillID ;		// 
} ;

//=======================================================================
// �̴ϰ��� ��Ŷ ����
//=======================================================================
//#define MSG_NO_MINIGAME_START_REQ_C_S		51		// �̴ϰ��� ���� ��û
//#define MSG_NO_MINIGAME_MISSION_STAGE_S_C	52		// ������ �ܰ� (���� �˸� �޽����ε� ����)
//#define MSG_NO_MINIGAME_MAST_STAGE_C_S		52		// �Ϸ��� �ܰ� (���� ��û �޽����ε� ����)
//#define MSG_NO_MINIGAME_FAIL_S_C			53		// ���� ��Ŷ
//#define MSG_NO_MINIGAME_CTRL				54		// �㰡 Ȥ�� �Ұ�... S->C
//#define MSG_NO_MINIGAME_RESULT				55		// ���.(�ΰ� ȿ���� ���� ���� ����. )
struct MSG_MINIGAME_START_REQ 
{
	_sHeader			header ;
	u_char				ucMessage ;

	short				sMiniGameID ;			// �̴ϰ��� ����
	u_char				ucLearnSkillIndex ;		// ������ ���� ��ȣ
	short				skillID ;				// ������ ���̵�.. 
};

struct MSG_MINIGAME_MISSION_STAGE
{
	_sHeader			header ;
	u_char				ucMessage ;

	int					iKey ;						// ��ŷ ������ ���� Ű �̴�.
	char				cMissionStage ;				// ������ �ܰ� (0 �̸� ����, -1 �̸� �������� ����)
};

struct MSG_MINIGAME_MAST_STAGE {
	_sHeader			header ;
	u_char				ucMessage ;

	int					iKey ;						// ����Ű �̴�.
	char				cMastStage ;				// �Ϸ��� �ܰ�(0 �̸� ���� ��û�ѰŴ�)
};


struct MSG_MINIGAME_FAIL {
	_sHeader			header ;
	u_char				ucMessage ;

	u_char				ucCommand ;					// ������ ��Ŷ ��ȣ
	u_char				ucCode ;					// ���� �ڵ�
};

enum {								// FAIL CODE
	_FAIL_CODE_HAVENT_SKILL,
	_FAIL_CODE_BED_KEY,
	_FAIL_CODE_ERROR,
	_FAIL_CODE_NEED_LAST_LEVEL,
	_FAIL_CODE_NEED_FULL_POWER
};

/*============================================================================

  MSG_MINIGAME_FAIL ����

	ucCommand : � ��Ŷ�� ���� ��� ��Ŷ���� �����ϱ� ����..
	ucCode	  : ���� �ڵ�

--------------------------------------------+-----------------------------------+---------------------------
 	ucCommand								|	iCode							|		����
--------------------------------------------+-----------------------------------+---------------------------
 	MSG_NO_MINIGAME_START_REQ_C_S			|	_FAIL_CODE_HAVENT_SKILL			|	�ش� ���� ����
 											|	_FAIL_CODE_ERROR				|	��Ÿ ����..					
											|	_FAIL_CODE_NEED_LAST_LEVEL		|	������ ������ �ƴ�..
											|	_FAIL_CODE_NEED_FULL_POWER		|	�ǰ� �Ǵ� ������ �� ���� �ʾ���
--------------------------------------------+-----------------------------+---------------------------------
	MSG_NO_MINIGAME_MAST_STAGE_C_S			|	_FAIL_CODE_BED_KEY				|	���� ����Ű.. (�Ѥ�;)
											|	_FAIL_CODE_ERROR				|	��Ÿ ����..											
--------------------------------------------+-----------------------------+---------------------------------

=============================================================================*/

enum {
	minigame_en_allow_start = 0,
	minigame_en_not_allow_or_finish,
	minigame_en_cancel_from_monster
} ;

// ����(�̴ϰ���) ���� ���
enum {
	_MODE_END_TRAINING_SELF_CANCEL,			// ����� ���..
	_MODE_END_TRAINING_COMPLETE			// ������ �Ϸ�
};

struct MSG_MINIGAME_CTRL
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucCtrl ;			// 0 : Yes(����), 1 : No,or �������� 2 : ���� ���� ����.
	short				skillID ;				// ������ ���̵�.. 
	short				sMiniGameID ;			// �̴ϰ��� ����
	
	u_short				usID ;					// 
} ;

struct MSG_MINIGAME_RESULT					// �ΰ� ȿ���� ������ �ڽſ��Ը�.
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	short				sAddBufEffect ;			// �߰� ȿ��.
	u_short				usApplyTime ;			// �ð�.
	u_char				ucEffectPercent ;		// ȿ�� %.. �̷��� �ʿ����� �𸣰���.
} ;


// ���� ���� ��Ŷ ���� 2004.08.17

#define MSG_NO_MARTIAL_READY					68
#define MSG_NO_MARTIAL_FIRE						69
#define MSG_NO_MARTIAL_RESULT					70
#define MSG_NO_MARTIAL_END						71

// ���� ��Ŷ�� martial type�� ����.
enum SKILL_USEFUL_TYPE_EN
{
	en_martial_ut_passive_skill=1,		// 01
	en_martial_ut_active_skill,			// 02
	en_martial_ut_charge_skill,			// 03
	en_martial_ut_passive_spell,		// 04
	en_martial_ut_casting_spell,		// 05
	en_martial_ut_channeling_spell,		// 06
	en_martial_ut_finish_skill,			// 07
	en_martial_ut_rounding_spell,		// 08
	en_martial_ut_hidden_ma,			// 09
	en_martial_ut_smashing_skill,		// 10
	en_martial_ut_action_skill			// 11
};

// Ÿ�� Ÿ��( TargetType )/ Ÿ�� Ÿ��( TargetType )
enum BATTLE_TARGET_TYPE_EN
{
	en_target_type_monster=0,	// 0 : ����
	en_target_type_npc,			// 1 : npc.
	en_target_type_dummy,		// 2 : ����( ����, ����.. )
	en_target_type_others,		// 3 : �ٸ� �÷��̾�.( PvP )
	en_target_type_self,		// 4 : �ڽſ��� �ϴ� ��.
	en_target_type_point,		// 5 : ä�θ� �Ǵ� ĳ���ÿ��� � ���� ������ ��.
	en_target_type_object		// 6 : Functional Object
};

enum EN_CHARAC_BATTLE_RESULT
{
	en_battle_result_normal=0,			// �������. ��� ����.
	en_battle_result_binsa_or_die,		// ĳ���� : ��翡 ������, ���� : ��� ����
	en_battle_result_honsu_or_dead,		// ĳ���� : ȥ�����·� ����, ���� : �̹� ���� ����.
	en_battle_result_dead_in_match,		// ���� ����.
	en_battle_result_dead_in_freematch	// �������� ����
};


// ���� ����
struct MARTIAL_READY
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucMartialType : 4 ;		// ���� Ÿ�� 
	u_char				ucTargetType : 3 ;		// ����� Ÿ��
	u_char				ucPadding1 : 1 ;		// �е�
};

// Fire
struct MARTIAL_FIRE
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucMartialType : 4 ;		// ���� Ÿ�� 
	u_char				ucTargetType : 3 ;		// ����� Ÿ��
	u_char				ucPadding1 : 1 ;		// �е�
};

// Result of fire
struct MARTIAL_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucMartialType : 4 ;		// ���� Ÿ�� 
	u_char				ucTargetType : 3 ;		// ����� Ÿ��
	u_char				ucPadding1 : 1 ;		// �е�
	
	short				sLeftForce ;			// ����� ���� ����
};

// martial end
struct MARTIAL_END
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucMartialType : 4 ;		// ���� Ÿ�� 
	u_char				ucTargetType : 3 ;		// ����� Ÿ��
	u_char				ucPadding1 : 1 ;		// �е�
	
	u_short				usAttackerID ;
	char				cReason ;
	
};

///////////////////////////////////////////////////////////////////////////////
// Active Skill
// Martial type : en_martial_ut_active_skill	// 02
///////////////////////////////////////////////////////////////////////////////
// 1. Fire
struct MSG_ACTIVE_FIRE : public MARTIAL_FIRE
{
	u_char				ucSequence : 2 ;		// ������.
	u_char				ucPadding : 6 ;			// 
	
	u_short				usTargetID ;			// ���.
	short				sAnimation ;			// �ִϸ��̼�.
	
} ;

// 2. Result
struct MSG_ACTIVE_RESULT_MOB : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	
	short				sAnimation ;
	
	u_short				ucSequence : 2 ;		//
	u_short				usAtkInfo_1 : 3 ;		// atkinfo : 0 for Hit, 1 Critical, 2 Evasion, 3 Block, 4 Shock, 5, 6, 7
	u_short				usAtkInfo_2 : 3 ;		//
	u_short				usAtkInfo_3 : 3 ;		//
	u_short				usPadding : 5 ;			// 
	
	u_short				usDamage_1 ;
	char				cMobVitalRate_1 ;		//
	
	u_short				usDamage_2 ;
	char				cMobVitalRate_2 ;		//
	
	u_short				usDamage_3 ;
	char				cMobVitalRate_3 ;		//
	
	char				cBattleResult ;			// ���� ���.
} ;


struct MSG_ACTIVE_RESULT_PVP : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	
	
	u_char				ucPadding : 6 ;
	short				sAnimation ;
	
	u_char				ucSequence : 2 ;
	u_short				usAtkInfo_1 : 3 ;		// 
	u_short				usAtkInfo_2 : 3 ;		//
	u_short				usAtkInfo_3 : 3 ;		//
	u_short				usPadding : 5 ;			// 
	
	u_short				usDamage_1 ;
	u_short				usDamage_2 ;
	u_short				usDamage_3 ;
	
	char				cBattleResult ;			// EN_CHARAC_BATTLE_RESULT
} ;


///////////////////////////////////////////////////////////////////////////////
// Charge Skill
// Martial type : en_martial_ut_charge_skill	// 03
///////////////////////////////////////////////////////////////////////////////
// 1. Ready
struct MSG_CHARGE_READY : public MARTIAL_READY
{
	u_short				usTargetID ;
	u_char				ucLearnSkillIndex ;		//
} ;

// 1-1 Ready Response
struct MSG_CHARGE_READY_RES : public MARTIAL_READY
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sChargeSkillID ;
} ;


// 2. Fire
struct MSG_CHARGE_FIRE : public MARTIAL_FIRE
{
	short				sChargeTime ;
} ;


// 3. Result
struct MSG_CHARGE_RESULT_MOB : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sChargeSkillID ;
	char				cSkillStep ;
	char				cMobVitalRate ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;			// 
	
	char				cBattleResult ;		// 
} ;

struct MSG_CHARGE_RESULT_PVP : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sChargeSkillID ;
	char				cSkillStep ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;			// 
	
	char				cBattleResult ;
} ;

// 4. End
struct MSG_CHARGE_END : public MARTIAL_END
{
} ;


///////////////////////////////////////////////////////////////////////////////
// Casting Spell
// Martial type : en_martial_ut_casting_spell	// 05
///////////////////////////////////////////////////////////////////////////////
// ����� pc �Ǵ� npc �϶�,
struct MSG_CASTING_READY_TO_PC : public MARTIAL_READY
{
	u_short				usTargetID ;
	u_char				ucLearnSkillIndex ;
	char				cStep ;
} ;

struct MSG_CASTING_READY_RES_TO_PC : public MARTIAL_READY
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sCastingSpellID ;
	char				cSpellStep ;
} ;



// ����Ʈ.
struct MSG_CASTING_READY_TO_POINT : public MARTIAL_READY
{
	u_char				ucLearnSkillIndex ;
	char				cSpellStep ;
	float				fTargetPosX ;
	float				fTargetPosZ ;
} ;

struct MSG_CASTING_READY_RES_TO_POINT : public MARTIAL_READY
{
	u_short				usAttackerID ;
	short				sCastingSpellID ;
	char				cSpellStep ;
	float				fTargetPosX ;
	float				fTargetPosZ ;
} ;


// 2. Fire
struct MSG_CASTING_FIRE : public MARTIAL_FIRE
{
	// ����. 
} ;


// 3. Result
struct MSG_CASTING_RESULT_TO_MOB : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sCastingSpellID ;
	char				cSpellStep ;
	char				cMobVitalRate ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;			// 
	
	char				cBattleResult ; 
} ;

struct MSG_CASTING_RESULT_TO_PVP : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sCastingSpellID ;
	char				cSpellStep ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;			// 
	
	char				cBattleResult ; 
} ;


struct MSG_CASTING_RESULT_TO_POINT : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	short				sCastingSpellID ;
	char				cSpellStep ;
	float				fTargetPosX ;
	float				fTargetPosZ ;	
} ;

// 4. End
struct MSG_CASTING_END : public MARTIAL_END
{
} ;


///////////////////////////////////////////////////////////////////////////////
// Channeling Spell
// Martial type : en_martial_ut_channeling_spell	// 06
// Ready ����, Fire & Result ����
///////////////////////////////////////////////////////////////////////////////
// 1. Ready
struct MSG_CHANNEL_READY_SELF : public MARTIAL_READY
{
	u_char				ucLearnSkillIndex;
	char				cSpellStep;
};

struct MSG_CHANNEL_READY_POINT : public MARTIAL_READY
{
	u_char				ucLearnSkillIndex ;
	char				cSpellStep ;
	float				fTargetPosX ;
	float				fTargetPosZ ;
} ;

struct MSG_CHANNEL_READY_PC : public MARTIAL_READY
{
	u_char				ucLearnSkillIndex ;
	char				cSpellStep ;
	u_short				usTargetID ;				// PC�� ���̵�.
} ;

struct MSG_CHANNEL_READY_RES_SELF : public MARTIAL_READY 
{
	u_short				usAttackerID;				// �� ������
	short				sChannelSpellID;			// �� ��ų�� 
	char				cSpellStep;					// �� �ܰ�� ���ڸ����� ���� ä�θ��� �����Ϸ� �Ѵ�.
};


struct MSG_CHANNEL_READY_RES_POINT : public MARTIAL_READY
{
	u_short				usAttackerID ;
	short				sChannelSpellID ;
	char				cSpellStep ;
	float				fTargetPosX ;
	float				fTargetPosZ ;
} ;

struct MSG_CHANNEL_READY_RES_PC : public MARTIAL_READY
{
	u_short				usAttackerID ;
	short				sChannelSpellID ;
	char				cSpellStep ;
	u_short				usTargetID ;
} ;


// 2. Fire
struct MSG_CHANNEL_FIRE : public MARTIAL_FIRE
{
	//
} ;


// 3. Result
struct MSG_CHANNEL_RESULT_POINT : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	short				sChannelSpellID ;
	char				cSpellStep ;
	float				fTargetPosX ;
	float				fTargetPosZ ;
	float				fRadius;			// ���� �ݰ�
} ;

// ĳ���Ϳ��� �� �� : en_target_type_others
struct MSG_CHANNEL_RESULT_PC : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	short				sChannelSpellID ;
	char				cSpellStep ;
	u_short				usTargetID ;
	float				fRadius;			// ���� �ݰ�
	char				cBattleResult;		// ����� �׾����� ��Ҵ���

	u_char				usAtkInfo : 3;
	u_char				ucPadding : 5;
	u_short				sDamage;
} ;

// en_target_type_monster, en_target_type_npc, en_target_type_dummy
struct MSG_CHANNEL_RESULT_NPC : public MARTIAL_RESULT 
{
	u_short				usAttackerID;
	short				sChannelSpellID;
	char				cSpellStep;
	u_short				usTargetID;
	float				fRadius;
	char				cVitalRate;
	char				cBattleResult;
	
	u_char				usAtkInfo : 3;
	u_char				ucPadding : 5;
	u_short				sDamage;
};


// 4. End
struct MSG_CHANNEL_END : public MARTIAL_END
{
} ;


///////////////////////////////////////////////////////////////////////////////
// Finish Skill
// Martial type : en_martial_ut_finish_skill	// 07
///////////////////////////////////////////////////////////////////////////////
// 1. Ready
struct MSG_FINISH_READY : public MARTIAL_READY
{
	u_short				usTargetID ;
	u_char				ucLearnSkillIndex ;
	char				cSkillStep ;
} ;


struct MSG_FINISH_READY_RES : public MARTIAL_READY
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sFinishSkillID ;
	char				cSkillStep ;
} ;


// 2. Fire
struct MSG_FINISH_FIRE : public MARTIAL_FIRE
{
	float				fTargetPosX ;
	float				fTargetPosZ ;
} ;


// 3. Result
struct MSG_FINISH_RESULT_TO_MOB : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sFinishSkillID ;
	char				cSkillStep ;
	
	float				fTargetPosX ;
	float				fTargetPosZ ;
	
	char				cMobVitalRate ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;
	
	char				cBattleResult ;
} ;

struct MSG_FINISH_RESULT_TO_PC : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sFinishSkillID ;
	char				cSkillStep ;
	
	float				fTargetPosX ;
	float				fTargetPosZ ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;
	
	char				cBattleResult ;
} ;

// 4. End
struct MSG_FINISH_END : public MARTIAL_END
{
} ;


///////////////////////////////////////////////////////////////////////////////
// Rounding Spell
// Martial type : en_martial_ut_rounding_spell	// 08
///////////////////////////////////////////////////////////////////////////////
// 2. Fire
struct MSG_ROUNDING_FIRE : public MARTIAL_FIRE
{
	u_short				usTargetID ;
	u_char				ucLearnSkillIndex ;
	char				cSpellStep ;
} ;


// 3. Result
struct MSG_ROUNDING_RESULT_TO_MOB : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sRoundingSpellID ;
	char				cSpellStep ;
	char				cMobVitalRate ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;
	
	char				cBattleResult ;
} ;

struct MSG_ROUNDING_RESULT_TO_PC : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sRoundingSpellID ;
	char				cSpellStep ;
	
	u_char				ucAtkInfo : 3 ;
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;
	
	char				cBattleResult ;
} ;

struct MSG_ROUNDING_FIRE_RES : public MARTIAL_FIRE  // Fire�� ���� ��, Fire�� ������ ���� �ٷ� ��
{
	//
	u_short				usAttackerID ;
	u_short				usTargetID ;
	
	short				sSkillID ;
	char				cSpellStep ;		// Result Step
	
	short				sLeftForce ;
} ;

struct MSG_ROUNDING_FIRE_TO_POINT : public MARTIAL_FIRE
{
	u_char				ucLearnSkillIndex;
	char				cSpellStep;

	float				fTargetX;
	float				fTargetZ;
};

struct MSG_ROUNDING_FIRE_RES_TO_POINT : public MARTIAL_FIRE
{
	u_short				usAttackerID;
	short				sSkillID;
	char				cSpellStep;

	float				fTargetX;
	float				fTargetZ;

	short				sLeftForce;
};

struct MSG_ROUNDING_RESULT_TO_POINT : public MARTIAL_RESULT
{
	u_short				usAttackerID;
	short				sRoundingSpellID;
	char				cSpellStep;

	float				fTargetX;
	float				fTargetZ;
};

//////////////////////////////////////////////////////////////////////////
// Smashing Skill : �ϰݱ�
// Martial Type : en_martial_ut_smashing_skill,		// 02
// Fire & Result : Fire �ϸ�, �ٷ� Result �� ����.
// vs Monster, vs Player �� ���� �ٸ��Ƿ� ��� ��Ŷ 2��.
//////////////////////////////////////////////////////////////////////////
// 1. Fire
struct MSG_SMASHING_FIRE : public MARTIAL_FIRE
{
	u_short				usTargetID ;			// ���.
	
	u_char				ucLearnSkillIndex ;		// ���� �ϰݱ��� ��ų ���� ��ȣ.
	char				cSpellStep ;
} ;


// 2. Result : To Monster : Rounding �� ����.
struct MSG_SMASHING_RESULT_MOB : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	
	short				sSmashingSkillID ;
	char				cSkillStep ;
	
	char				cMobVitalRate ;
	
	u_char				ucAtkInfo : 3 ;			// atkinfo : 0 for Hit, 1 Critical, 2 Evasion, 3 Block, 4 Shock, 5, 6, 7
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;
	
	char				cBattleResult ;
} ;

// To Other
struct MSG_SMASHING_RESULT_PVP : public MARTIAL_RESULT
{
	u_short				usAttackerID ;
	u_short				usTargetID ;
	short				sSmashingSkillID ;
	char				cSkillStep ;
	
	u_char				ucAtkInfo : 3 ;			// atkinfo : 0 for Hit, 1 Critical, 2 Evasion, 3 Block, 4 Shock, 5, 6, 7
	u_char				ucPadding : 5 ;
	
	u_short				usDamage ;
	
	char				cBattleResult ;
} ;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Action skill
// �ൿ�� ��Ÿ�̹Ƿ�, fire ������, �ٷ� result �� ���ϴ�.
struct MSG_ACTION_FIRE : public MARTIAL_FIRE
{
	u_short				usTargetID;				// ��� id
	u_char				ucLearnSkillIndex;		
	char                cSkillStep ;            // �ܰ�.
};

struct MSG_ACTION_RESULT : public MARTIAL_RESULT
{
	u_short				usAttackerID;
	u_short				usTargetID;
	short				sActionSkillID ;		// �ش� ��ų
	char                cSkillStep ;
	u_char				ucResult;				// 0���� 1���� 2���º��� 3������°� �ƴ� 4������
};

enum EN_ACTION_RESULT 
{
	en_action_result_success = 0,				// ����
	en_action_result_fail_or_avoid,				// ȸ��
	en_action_result_need_force,				// ���� ����
	en_action_result_no_mask,					// ����� ���� ���°� �ƴմϴ�.
	en_action_result_no_target,					// ����� ����.
	en_action_result_no_effect,					// 
	en_action_result_no_skill,					// �׷� ��ų ����.
	en_action_result_cool_time_error,			// 
	en_action_result_end						// 
};


// ���� ��� ���� ������ : charge, casting, finish ������ �и����ִ���, �ϳ��� ����.
enum EN_MARTIAL_END_ERROR
{
	en_martial_end_normal=0,			// ����.
	en_martial_end_no_martial,			// ���� ���� : �غ� �ܰ迡�� ����.
	en_martial_end_no_force,			// ���� ���ڸ�.
	en_martial_end_no_select,			// Ready �� ���� �ʾҰų�, ���� ����.
	en_martial_end_invalid_target,		// Ÿ���� ����. : 4
	en_martial_end_need_training,		// ������ �ʿ��ϴ�. 5
	en_martial_end_cooltime_error,		// ��Ÿ���� ������ �ʾҴ�. 6
	en_martial_end_distance_too_far		// �Ÿ��� �ʹ� �ִ�. 7
} ;

#define _INVALID_SKILL_SLOT_			0xff		// ��ȿ ��ų ���԰�����, skill add, find ���� ���� ���ϰ����� ���δ�. 


// ���� ȿ��
#define MSG_NO_EFFECT_CTRL					56			// ����Ʈ/���¸� �߰� ����
#define MSG_NO_EFFECT_INFO					57			// �ʵ� �� �� �ڽ��� ���� ������ �˷��ش�.

// Buff/Hostile�� ����
struct _sUser_BHEntry
{
	short				skillID;					// ��ų ���̵�
	unsigned short		usLeftTimeSec;				// ���� �ð�
	char				skillLevel;					// ��ų�� �ܰ�
};

// ȿ�� ��� ����
struct _sUser_EffectEntry
{
	short				id;
	unsigned short		usLeftTimeSec;				// ���� �ð� (��)
	short				value;						// ��
};

struct _sUser_ExtraEntry
{
	char				cWhere ;			// 0 - 9 : 10 ĭ ����� ����.( ����, ��, ��ȣǳ���.. )
	short				id ;
	unsigned short		usLeftTimeSec ;		// ���� �ð� ( '��', ms ���� ��Ȯ�� �ʿ� ����. )
	short				value ;				// ȿ�� ��Ҵ� �ܰ谡 ����. �׷��� ���� �ʿ�.
};

//Author : ����� //breif : ������ ���� �߰�
struct _sUser_SpecialBuff
{
	short	skillID;	// ��ų�� ���̵�
	unsigned short	usLeftTimeSec;	// ���� �ð�
	char	skillLevel;	// ��ų�� �ܰ�
	u_char	slot;
};

// ��ų ȿ���� ��Ƽ�� ���� - �Ѵ� ���� ��� �ι� ��.
// ���� ���� ��Ŷ cCount ��ŭ
// Buff���� Hostile������ Ŭ���̾�Ʈ���� �˾Ƽ� ó���ؾ���
enum
{
	eff_info_mode_my_bh_list = 0,
	eff_info_mode_my_entry_list,
	eff_info_mode_other_bh_list,
	eff_info_mode_my_extra_list,		// ����, ������ ��Ÿ ����ȿ��
	eff_info_mode_other_entry_list,		// �ٸ� ĳ������ ����ȿ�� ����Ʈ.
	eff_info_mode_special_buff_list	// ����� ���� ����Ʈ //Author : ����� //breif : ������ ���� �߰�
};

struct MSG_EFFECT_INFO
{
	_sHeader			header;
	u_char				ucMessage;

	char				cMode;
};

#define _MAX_CHARAC_BUFF_					16			// �ִ� Buff �� ����
#define _MAX_CHARAC_HOSTILE_				16			// �ִ� Hostile�� ����
#define _MAX_CHARAC_BH_ENTRY_				32			// ���� �ΰ� �� : ���� ���� ������ 

#define _MAX_CHARAC_EFFECT_ENTRY_			32			//
#define _MAX_CHARAC_CONDITION_ENTRY_		16			// 
#define _MAX_EXTRA_EFFECT_ENTRY_			10			// extra �ִ밪 10
#define _MAX_CHARAC_SPECIAL_BUFF_			1			// ����� ���� �ִ밪�ΰ�..


struct MSG_EFFECT_MY_BH_INFO : public MSG_EFFECT_INFO		// ���� ȿ�� ���� (����)
{
	char				cCount;
	_sUser_BHEntry		entry[_MAX_CHARAC_BH_ENTRY_];
};

struct MSG_EFFECT_MY_ENTRY_INFO : public MSG_EFFECT_INFO	// ȿ�� ����Ʈ	(����)
{
	char				cCount;
	_sUser_EffectEntry	entry[_MAX_CHARAC_EFFECT_ENTRY_];
};

struct MSG_EFFECT_OTHER_INFO : public MSG_EFFECT_INFO		// ��ų ����Ʈ�� �´�, ��Ƽ���� Zone charac info�� �����
{
	u_short				usID;
	char				cCount;
	short				id[_MAX_CHARAC_BH_ENTRY_];
};

// �ڽ��� ��Ÿ ����ȿ�� ����Ʈ.
struct MSG_EFFECT_MY_EXTRA_INFO : public MSG_EFFECT_INFO
{
	char				cCount ;		// ī��Ʈ�� �߶� ������.
	
	_sUser_ExtraEntry	entry[_MAX_EXTRA_EFFECT_ENTRY_] ;
} ;

//Author : ����� //breif : ������ ���� �߰�
struct MSG_EFFECT_SPECIAL_BUFF_INFO : public MSG_EFFECT_INFO
{
	char	cCount;
	_sUser_SpecialBuff	entry[_MAX_CHARAC_SPECIAL_BUFF_];
} ;

enum EN_EFFECT_CTRL_REQUEST_MODE
{
	en_eff_ctrl_rq_timeout_buff = 0,		// ���� ��ų Ÿ�Ӿƿ� ��û
	en_eff_ctrl_rq_timeout_hostile,			// �Ͻ� ��ų Ÿ�Ӿƿ� ��û
	en_eff_ctrl_rq_timeout_effect,			// ȿ�� ��� Ÿ�Ӿƿ� ��û
	en_eff_ctrl_rq_timeout_special,			// ����� ������ ����( id ����, ���� ��ȣ�� ���� ) //Author : �����
	
	en_eff_ctrl_rq_calc_buff,				// ������ �ִ� ���͹� ȿ���� ����� ��û�Ѵ�. ���� ���, 2�ʸ��� ü�� ȸ��.
	en_eff_ctrl_rq_calc_hostile,			// �Ͻ��� �ִ� ���͹� ȿ���� ����� ��û�Ѵ�. ���� ���, 3�ʸ��� ��. 
	en_eff_ctrl_rq_calc_effect				// ȿ�� ����� ���͹� ����� ��û�Ѵ�. ���� ���, 3�ʸ��� ��.
};

struct MSG_EFFECT_CTRL_REQUEST 
{
	_sHeader			header;
	u_char				ucMessage;

	char				cMode;

	short				id;
};

enum EN_EFFECT_CTRL_RESULT_MODE
{
	// ��ų
	en_eff_ctrl_rs_add_skill=0,				// 0	��ų �߰�. 
	en_eff_ctrl_rs_update_skill,			// 1	��ų �ð�, �ܰ� : ���� �̰�, ������ �������� ����.
	en_eff_ctrl_rs_del_skill,				// 2	ȿ�� ���� : Client �� ��û�� ����, �Ǵ� �������� ����.
	en_eff_ctrl_rs_del_skill_rq_fail,		// 3	�ð��� �ȸ��� : ������ ����, time���� ������Ʈ
	en_eff_ctrl_rs_del_all_buff,			// 4	��� ���� ȿ�� ����.
	en_eff_ctrl_rs_del_all_hostile,			// 5	��� �Ͻ� ǥ�� ����.
	en_eff_ctrl_rs_skill_reserve3,			// 6
	en_eff_ctrl_rs_skill_reserve4,			// 7
	
	// �ڱ� ȿ��
	en_eff_ctrl_rs_add_effect,				// 8	���� �߰� ( ������ ���̵� )
	en_eff_ctrl_rs_update_effect,			// 9	���� ������Ʈ
	en_eff_ctrl_rs_del_effect,				// 10	���� ����.
	en_eff_ctrl_rs_del_effect_rq_fail,		// 11	�ð� �ȸ���. : ������ ����, time���� ������Ʈ
	en_eff_ctrl_rs_del_all_effect,			// 12	�ɷ� �ִ� ��� ȿ�� ����. ���� �ʱ�ȭ.
	en_eff_ctrl_rs_update_value_only,		// 13	id�� �ش��ϴ� ȿ�� ���� ������Ʈ, �ð��� ����
	//en_eff_ctrl_rs_condition_reserve2,		// 13
	en_eff_ctrl_rs_condition_reserve3,		// 14
	en_eff_ctrl_rs_condition_reserve4,		// 15
	
	// �ٸ� ����� ���� ����.
	en_eff_ctrl_rs_other_add_skill,			// 16	��ų �߰�
	en_eff_ctrl_rs_other_del_skill,			// 17	��ų ����
	en_eff_ctrl_rs_other_del_all_skill,		// 18	��� ��ų ����
	en_eff_ctrl_rs_other_skill_res,			// 19	
	en_eff_ctrl_rs_other_add_effect,		// 20	ȿ�� �߰�
	en_eff_ctrl_rs_other_del_effect,		// 21	ȿ�� ����
	en_eff_ctrl_rs_other_del_all_effect,	// 22	��� ȿ�� ����
	en_eff_ctrl_rs_other_effect_res,			// 23

	// extra ȿ��
	en_eff_ctrl_rs_add_extra,				// 24	extra ȿ���� �߰�(����, ��ô������, ����Ʈ)
	en_eff_ctrl_rs_update_extra,			// 25	extra ȿ���� ����
	en_eff_ctrl_rs_del_extra,				// 26	extra ȿ���� ����
	
	// ����/��� ���
	en_eff_ctrl_rs_cancel_current = 30,		// 30	�������� ��ų(����) ���
	en_eff_ctrl_rs_cancel_hyperrun,			// 31	�������� ��� ���

	//Author : ����� //breif : ������ �߰�
	// special buff
	en_eff_ctrl_rs_add_special = 40,		// 40      ����� ������ �߰�
	en_eff_ctrl_rs_update_special,			// 41      ����� ������ ����(add �� ����)
	en_eff_ctrl_rs_del_special,				// 42      ����� ������ ����
} ;

/*
en_eff_ctrl_rs_cancel_current = 30,	// 30	�������� ��ų(����) ���
en_eff_ctrl_rs_cancel_hyperrun		// 31	�������� ��� ���
*/
struct MSG_EFFECT_CTRL_RESULT		// ����ȿ���� �߰�/����/������Ʈ�ϱ� ���� ����.
{
	_sHeader			header ;
	u_char				ucMessgae ;
	
	char				cMode ;				// 
} ;

/*
�Ʒ��� ���� Mode ���� ��ȿ.
en_eff_ctrl_rs_add_skill=0,			// 0	��ų �߰�. 
en_eff_ctrl_rs_update_skill,		// 1	��ų �ð�, �ܰ� : ���� �̰�, ������ �������� ����.
en_eff_ctrl_rs_del_skill,			// 2	ȿ�� ���� : Client �� ��û�� ����, �Ǵ� �������� ����.
en_eff_ctrl_rs_del_skill_rq_fail,	// 3	�ð��� �ȸ��� : ������ ����, time���� ������Ʈ
en_eff_ctrl_rs_del_all_buff,		// 4	��� ���� ȿ�� ����.
en_eff_ctrl_rs_del_all_hostile,		// 5	��� �Ͻ� ǥ�� ����.
en_eff_ctrl_rs_skill_reserve3,		// 6
en_eff_ctrl_rs_skill_reserve4,		// 7
*/
struct MSG_EFFECT_CTRL_RESULT_MY_SPELL : public MSG_EFFECT_CTRL_RESULT
{
	short				id;				// ���� ���̵�
	unsigned short		lefttimesec;	// �� ���� ���� �ð�
	char				cStep;
};

/*
�Ʒ��� ���� Mode ���� ��ȿ.
// �ڱ� ȿ��
en_eff_ctrl_rs_add_effect,			// 8	���� �߰� ( ������ ���̵� )
en_eff_ctrl_rs_update_effect,		// 9	���� ������Ʈ
en_eff_ctrl_rs_del_effect,			// 10	���� ����.
en_eff_ctrl_rs_del_effect_rq_fail,	// 11	�ð� �ȸ���. : ������ ����, time���� ������Ʈ
en_eff_ctrl_rs_del_all_effect,		// 12	�ɷ� �ִ� ��� ȿ�� ����. ���� �ʱ�ȭ.
en_eff_ctrl_rs_condition_reserve2,	// 13
en_eff_ctrl_rs_condition_reserve3,	// 14
en_eff_ctrl_rs_condition_reserve4,	// 15
*/
struct MSG_EFFECT_CTRL_RESULT_MY_EFFECT : public MSG_EFFECT_CTRL_RESULT
{
	short				id;
	unsigned short		lefttimesec;
	short				value;
};

/*
Mode�� �Ʒ��� ���� �� ��ȿ.
en_eff_ctrl_rs_other_add_skill,		// 16	��ų �߰�
en_eff_ctrl_rs_other_del_skill,		// 17	��ų ����
en_eff_ctrl_rs_other_del_all_skill,	// 18	��� ��ų ����
en_eff_ctrl_rs_other_skill_res,		// 19	����.
en_eff_ctrl_rs_other_add_effect,	// 20	ȿ�� �߰�
en_eff_ctrl_rs_other_del_effect,	// 21	ȿ�� ����
em_eff_ctrl_rs_other_del_all_effect,// 22	��� ȿ�� ����
en_eff_ctrl_rs_other_effect_res		// 23
*/
struct MSG_EFFECT_CTRL_RESULT_OTHER : public MSG_EFFECT_CTRL_RESULT
{
	unsigned short		uid;
	short				id;
	char				step;
};

/*
Mode�� �Ʒ��� ���� �� ��ȿ.
en_eff_ctrl_rs_add_extra,			// 24	����Ʈ�� ȿ���� �߰�.(���İ� ��ô������, ����Ʈ )
en_eff_ctrl_rs_update_extra,		// 25	����Ʈ�� ȿ���� ����.	
en_eff_ctrl_rs_del_extra			// 26	����Ʈ�� ȿ���� ����.
*/
struct MSG_EFFECT_CTRL_RESULT_EXTRA : public MSG_EFFECT_CTRL_RESULT
{
	u_char				slot ;			// 0 - 9 ���� ��ȣ.
	
	short				id ;
	short				value ;				// 
	u_short				usLeftTimeSec ;		// ���� �ð�(��)
} ;

/* 
Mode�� �Ʒ��� ���� �� ��ȿ	
en_eff_ctrl_rs_add_special			// 40	����� ���� �߰�
en_eff_ctrl_rs_update_special,		// 41	����� ������ ����(add �� ����)
en_eff_ctrl_rs_del_special,			// 42	����� ������ ����
*/
struct MSG_EFFECT_CTRL_RESULT_ADD_SPECIAL : public MSG_EFFECT_CTRL_RESULT
{
	u_char				slot;			// 0-3

	short				id;
	u_char				level;

	//Author : ����� //breif : ������ ���� 
	u_short				usLeftSecond;    // ���� �ð�( �� )
};

//Author : ����� //breif : ������ ���� 
struct MSG_EFFECT_CTRL_RESULT_UPDATE_SPECIAL : public MSG_EFFECT_CTRL_RESULT
{
	u_char	slot;
	
	short	id;	// ���� ���̵�
	u_char	level;	// ���� ����
	
	u_short	usLeftSecond;	// ���� �ð�( �� )
};

struct MSG_EFFECT_CTRL_RESULT_DEL_SPECIAL : public MSG_EFFECT_CTRL_RESULT
{	   
	u_char				slot;	// 0-3
};


// NPC Buff
// #define MSG_NO_NPC_BUFF_REQ 48 
// NPC ���� ��û ��Ŷ. 
struct MSG_NPC_BUFF_REQ
{
	_sHeader				header;
	u_char				ucMessage;

	u_char				ucReqIndex;			// ���ҽ������� ���� ����Ʈ���� ���° �ΰ�
	u_int				uiNeedMoney;
};

// #define MSG_NO_NPC_BUFF_RESULT 48 
// NPC ���� ��� ��Ŷ.
struct MSG_NPC_BUFF_RESULT
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucResult;			// 0:���� 1:������ 2:���Ѿ���(�����̾� �Ǿ���/���� �ƴ�)
	u_int				uiMoney;
};



// ===========================================================
// PK ����
#define _PK_RECORD_WIN_COUNT_				10
#define _PK_RECORD_LOSE_COUNT_				10
#define _PK_RECORD_ABUSING_CHECK_			20

enum
{
	en_pk_option_none = 0,					// 0 : no meaning
	en_pk_option_win_normal,				// 1 : win - normal
	en_pk_option_win_mask,					// 2 : win - mask
	en_pk_option_lose_normal,				// 3 : lose - normal
	en_pk_option_lose_mask					// 4 : lose_mask
};

struct __cntPkRecordEntry					// ���� ��� : ������ �̸��� ��¥, ����
{
	union 
	{
		struct 
		{
			u_int			uiOption : 3;	// 0:None 1:win-normal 2:win-mask 3:lose-normal 4:lose-mask 5:Res,..

			u_int			uiYear : 5;		// �⵵ 2006+year
			u_int			uiMonth : 6;	// 1 - 12
			u_int			uiDay : 6;		// 1 - 31
			u_int			uiHour : 6;
			u_int			uiMinute : 6;	// 0 - 59
		};

		u_int				uiValue;
	};

	char				name[13];			// �������� NULL
	char				cMoonpa;			// ���� ����
};

struct __cntPkCount							// �� ����, Ȥ�� �� ��å�� ���� ��/�� ��
{
	int					iWin;
	int					iLose;
};

struct MSG_PK_RECORD
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucMode;				// en_pk_record ��
};

enum EN_PK_RECORD
{
	en_pk_record_win_lose_record = 0,		// 0 ���� ����
	en_pk_record_moonpa_record,				// 1 ���� ����
	en_pk_record_class_record,				// 2 ��å ����
	en_pk_record_update_win,				// 3 �� ���� ����
	en_pk_record_update_lose,				// 4 �� ���� ����
	en_pk_record_update_moonpa_win = 5,		// 5 ���� �� ���� ����
	en_pk_record_update_moonpa_lose,		// 6 ���� �� ���� ����
	en_pk_record_update_class_win,			// 7 ��å �� ���� ����
	en_pk_record_update_class_lose,			// 8 ��å �� ���� ����
	en_pk_record_abuse_to_win,				// 9 Abuse to winner
	en_pk_record_abuse_to_lose,				// 10 Abuse to loser
	en_pk_record_lack_honor_to_win,			// 11 ���� �Ǹ��� ��� 9���� to winner
	en_pk_record_lack_honor_to_lose			// 12 ���� �Ǹ��� ��� 9���� to loser
};

// en_pk_record_win_lose_record=0,                      // 0 ���� ����.
struct MSG_PK_WIN_LOSE_RECORD : public MSG_PK_RECORD
{
	// Count�� ���� pkRecord�� ������ �߶� ����

	int					iKillCount;			// ��ü ���� ��
	int					iDeadCount;			// ��ü ���� ��

	u_char				ucWinCount : 4;		// win ������ ����
	u_char				ucLoseCount : 4;	// lose ������ ����

	__cntPkRecordEntry	pkRecord[20];		// �ִ� win 10 + lose 10
};

// en_pk_record_moonpa_record,				// 1 ���� ����
struct MSG_PK_MOONPA_RECORD : public MSG_PK_RECORD
{
	__cntPkCount		MoonpaRecord[9];	// �ִ� 9�� ����, ���ؽ��� ������ ����, ���� 2�� ����
};

// en_pk_record_class_record,				// 2 ��å ����
struct MSG_PK_CLASS_RECORD : public MSG_PK_RECORD
{
	// �ִ� 8�� - �տ� 4���� ����
	// 0:������  1:������  2:������  3:�����
	__cntPkCount		ClassRecord[8];
};

// en_pk_record_update_win,					// 3 �� ���� ����
struct MSG_PK_UPDATE_WIN : public MSG_PK_RECORD
{
	int					iKillCount;

	u_char				ucSlotNo;
	__cntPkRecordEntry	pkRecordEntry;		// ���Կ� �ش��ϴ� ������ �״�� ���.
	
};

// en_pk_record_update_lose,				// 4 �� ���� ����
struct MSG_PK_UPDATE_LOSE : public MSG_PK_RECORD
{
	int					iDeadCount;

	u_char				ucSlotNo;
	__cntPkRecordEntry	pkRecordEntry;
};

// en_pk_record_update_moonpa_win = 5,		// 5 ���� �� ���� ����
struct MSG_PK_UPDATE_MOONPA_WIN : public MSG_PK_RECORD
{
	char				cMoonpa;
	int					iValue;
};

// en_pk_record_update_moonpa_lose,		// 6 ���� �� ���� ����
struct MSG_PK_UPDATE_MOONPA_LOSE : public MSG_PK_RECORD
{
	char				cMoonpa;
	int					iValue;
};

// en_pk_record_update_class_win,			// 7 ��å �� ���� ����
struct MSG_PK_UPDATE_CLASS_WIN : public MSG_PK_RECORD 
{
	char				cClass;
	int					iValue;
};

// en_pk_record_update_class_lose			// 8 ��å �� ���� ����
struct MSG_PK_UPDATE_CLASS_LOSE : public MSG_PK_RECORD
{
	char				cClass;
	int					iValue;
};
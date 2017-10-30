// XSkillItem.h: interface for the _XSkillItem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XSKILLITEM_H_
#define _XSKILLITEM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XGameItemDefine.h"

#pragma warning (disable : 4786)
#include <map>
using namespace std;

#define _XDEF_MAXSKILLCOUNT							2000
#define _XDEF_SKILLTABLE_HEADERNAMELENGTH			48
#define _XDEF_MAX_LEARNMASTERYSKILL					96

#ifdef _XDEF_ADD_MAX_LEARNSKILL_080522_KUKURI
	#define _XDEF_MAX_LEARNSKILL						80
#else
	#define _XDEF_MAX_LEARNSKILL						60
#endif


typedef enum _XSI_SkillCType
{
	_XSI_CTYPE_MARTIALARTS = 1,		// ����
	_XSI_CTYPE_SPELL				// ���
};

/*
typedef enum _XSI_SkillSType
{
	_XSI_STYPE_FIST = 1,			// �ǰ�
	_XSI_STYPE_WEAPON,				// ����
	_XSI_STYPE_HIDDEN,				// �ϱ�
	_XSI_STYPE_GROUP,				// ����
	_XSI_STYPE_FORCE,				// ���� ���
	_XSI_STYPE_MAGIC,				// �⹮ ���
	_XSI_STYPE_MEDITATION,			// ���� �ɹ�
	_XSI_STYPE_SPEED				// ���
};*/


// skill stype ���� - 05.01.26
// [����] 1 ���� / 2 �����ɹ� / 3 ��� / 4 ������ / 5 �ֺ��� / 6 ���    / 7 ��ȭ�� / 8 ����� / 9 â��� / 10 �뼱�� / 11 ȥ���� / 12 ������ / 13 ����
// [���] 1 ���� / 2 �����ɹ� / 3 ��� / 4 ȣ���� / 5 ������ / 6 �ܰ˼� / 7 �ų�� / 8 ���Ѱ� / 9 -        / 10 õȯ�� / 11 ��ȯ�� / 12 ��ȯ�� / 13 ����
// [�Ҹ�] 1 ���� / 2 �����ɹ� / 3 ��� / 4 �İ�� / 5 ����� / 6 �ǰ��� / 7 ������ / 8 õ�Ұ� / 9 ���̰� / 10 -        / 11 �ݰ��� / 12 �׸��� / 13 ����
// [�츲] 1 ���� / 2 �����ɹ� / 3 ��� / 4 ��â�� / 5 ��â�� / 6 �ݺμ� / 7 ��°� / 8 ���հ� / 9 �пհ� / 10 -        / 11 ���Ű� / 12 �ļ�� / 13 ����

typedef enum _XSI_SkillStype
{
	_XSI_STYPE_01 = 1,
	_XSI_STYPE_02,
	_XSI_STYPE_03,
	_XSI_STYPE_04,
	_XSI_STYPE_05,
	_XSI_STYPE_06,
	_XSI_STYPE_07,
	_XSI_STYPE_08,
	_XSI_STYPE_09,
	_XSI_STYPE_10,
	_XSI_STYPE_11,
	_XSI_STYPE_12,
	_XSI_STYPE_13
};

typedef enum _XSI_ActionType
{
	_XSI_ACTIONTYPE_MASTERYSKILL = 1,
	_XSI_ACTIONTYPE_COMBATSKILL,
	_XSI_ACTIONTYPE_BUFFSPELL,
	_XSI_ACTIONTYPE_DEBUFFSPELL,
	_XSI_ACTIONTYPE_HOSTILESPELL,
	_XSI_ACTIONTYPE_DEHOSTILESPELL,
};

typedef enum _XSI_UsefulType
{
	_XSI_USEFULTYPE_PASSIVESKILL = 1,
	_XSI_USEFULTYPE_ACTIVESKILL,
	_XSI_USEFULTYPE_CHARGESKILL,
	_XSI_USEFULTYPE_PASSIVESPELL,
	_XSI_USEFULTYPE_CASTINGSPELL,
	_XSI_USEFULTYPE_CHANNELINGSPELL,
	_XSI_USEFULTYPE_FINISHSKILL,
	_XSI_USEFULTYPE_ROUNDINGSPELL,
	_XSI_USEFULTYPE_HIDDENMA,
	_XSI_USEFULTYPE_SMASHINGSKILL,
	_XSI_USEFULTYPE_ACTIONSKILL
};

typedef enum _XSI_TargetType
{
	_XSI_TARGETTYPE_SELF = 1,
	_XSI_TARGETTYPE_OTHER_ENEMY,
	_XSI_TARGETTYPE_OTHER_FRIEND,
	_XSI_TARGETTYPE_SELFOTHER_FRIEND,
	_XSI_TARGETTYPE_SELFOTHER_ENEMY,
	_XSI_TARGETTYPE_OTHER_ALL,
	_XSI_TARGETTYPE_ALL,
	_XSI_TARGETTYPE_PARTY,
	_XSI_TARGETTYPE_POINT,
	_XSI_TARGETTYPE_COMA
};

typedef enum _XSI_Combination
{
	_XSI_COMBINATION_MU = 1,		// ��
	_XSI_COMBINATION_KUA,			// ��
	_XSI_COMBINATION_JUNG,			// ��
	_XSI_COMBINATION_GANG,			// ��
	_XSI_COMBINATION_YU,			// ��
	_XSI_COMBINATION_JOONG,			// ��
	_XSI_COMBINATION_GYUNG,			// ��
	_XSI_COMBINATION_HU,			// ��
	_XSI_COMBINATION_SIL,			// ��
	_XSI_COMBINATION_PAE,			// ��
	_XSI_COMBINATION_GAE,			// ��
	_XSI_COMBINATION_JI,			// ��
	_XSI_COMBINATION_CHON			// õ
};

typedef enum _XSI_FERA
{
	_XSI_FERA_MU = 1,				// ��
	_XSI_FERA_MOK,					// ��
	_XSI_FERA_HWA,					// ȭ
	_XSI_FERA_TO,					// ��
	_XSI_FERA_KUM,					// ��
	_XSI_FERA_SU,					// ��
	_XSI_FERA_JI,					// ��
	_XSI_FERA_CHON					// õ
};

enum _XSI_PARAMETER
{
	_XSI_PARAM_NONE = 0,
	_XSI_PARAM_DISTANCE,
	_XSI_PARAM_RADIUS,
	_XSI_PARAM_MINATTACKDAMAGE,
	_XSI_PARAM_MAXATTACKDAMAGE,
	_XSI_PARAM_ATTACKRATING,
	_XSI_PARAM_AVOIDRATING,
	_XSI_PARAM_COSTFORCE,
	_XSI_PARAM_COSTMANA,
	_XSI_PARAM_COSTENERGY,
	_XSI_PARAM_ADDBONUS,
	_XSI_PARAM_NUMBER,
	_XSI_PARAM_EFFTIME,
	_XSI_PARAM_EFFPROB_1,
	_XSI_PARAM_EFFVALUE_1,
	_XSI_PARAM_ADDPROB_1,
	_XSI_PARAM_ADDCOUNT_1,
	_XSI_PARAM_ADDVALUE_1,
	_XSI_PARAM_EFFPROB_2,
	_XSI_PARAM_EFFVALUE_2,
	_XSI_PARAM_ADDPROB_2,
	_XSI_PARAM_ADDVALUE_2,
	_XSI_PARAM_ADDCOUNT_2,
	_XSI_PARAM_EFFPROB_3,
	_XSI_PARAM_EFFVALUE_3,
	_XSI_PARAM_ADDPROB_3,
	_XSI_PARAM_ADDVALUE_3,
	_XSI_PARAM_ADDCOUNT_3,
	_XSI_PARAM_EFFPROB_4,
	_XSI_PARAM_EFFVALUE_4,
	_XSI_PARAM_ADDPROB_4,
	_XSI_PARAM_ADDVALUE_4,
	_XSI_PARAM_ADDCOUNT_4,
	_XSI_PARAM_EFFPROB_5,
	_XSI_PARAM_EFFVALUE_5,
	_XSI_PARAM_EFFPROB_6,
	_XSI_PARAM_EFFVALUE_6,
	_XSI_PARAM_EFFPROB_7,
	_XSI_PARAM_EFFVALUE_7,
	_XSI_PARAM_EFFPROB_8,
	_XSI_PARAM_EFFVALUE_8,
	_XSI_PARAM_TOTALCOUNT
};

struct _XSI_SkillEffect
{
	short		id;
	short		prob;
	short		value;
};

struct _XSI_SkillAdditive
{
	short		id;
	short		prob;
	short		value;
	short		count;
};

typedef struct 
{
	short	minAttackDamage;		// �ּ� ���ݷ�
	short	maxAttackDamage;		// �ִ� ���ݷ�
	short	attackRating;			// ���� ����ġ
	short	avoidRating;			// ȸ�� ����ġ
	short	costForce;				// �Ҹ� ����
	short	costHeart;				// �Ҹ� ���� - ������� ����

	short	reqItemCategory;		// �ʿ� ������ ī�װ�
	short	costMind;				// �Ҹ� ����
	float	finishSpeed;			// �ǴϽ� ���ǵ�

	int		skillExp;				// �ʿ��� skill exp
	int		casting;				// ���� �䱸 �ð�
	int		delay;					// ���� ���� �ð�
	int		coolDown;				// �� ���� �ð�
	char	requirementMission;		// �̼� �䱸 ����
	short	objectID;				// �̼� ID
	short	addBonus;				// �߰� ���ʽ�
	short	number;					// �ִ� ȸ��
	short	requirementItemID;		// �ʿ� item ID
	short	itemConsumed;			// item ��뷮
	unsigned short		effectTime;	// ȿ�� ����ð�

	// ȿ��
	short	effectCount;
	_XSI_SkillEffect*	effect;

	// additive effect
	short	additiveCount;
	_XSI_SkillAdditive*	additiveEffect;

}_XSKILLDETAILINFOSTRUCTURE;


typedef struct 
{
#ifdef _XDEF_NEWSTRINGMANAGER	
	LPTSTR	skillName;					// ��� �̸�
	
#ifdef _XJAPANESE
	LPTSTR	skillHiraganaName;			// ��� �̸� : ���󰡳�
#endif

#else
	TCHAR	skillName[16];				// ��� �̸�
#endif
	char	skillGrade;					// ��� �ܰ�
	char	skillCType;					// ��� �з�
	char	skillSType;					// ��� ���� ����
	char	actionType;					// ���� ����
	char	usefulType;					// ��� ����
	char	targetType;					// ��� ����
	char	clanType;					// ���� �з�
	char	weaponCType;				// ��� ���� �ߺз�
	char	weaponSType;				// ��� ���� �Һз�
	char	classGrade;					// ���� �ܰ�
	short	requirementSkill01;			// �䱸 ��� 01
	short	requirementSkill02;			// �䱸 ��� 02
	short	requirementCondition;		// �䱸 �����
	short	insideType;					// �����ɹ� ���� �����
	short	requirementLevel;			// ����ܰ� �䱸ġ
	int		requirementContribution;	// ����� �Һ� ���� �⿩��
	short	effectApplyType;			// ���� ���
	
	short	angle;						// ���� �ݰ� - bsk�� �ٲٸ鼭 �̸� ����

	char	requirementExcer;			// ������ �ʿ��Ѱ�
	char	martialNumber;				// �ʽ� ����
	char	attackPointFor1;			// 1�� Ÿ�� ȸ��
	char	attackPointFor2;			// 2�� Ÿ�� ȸ��
	char	attackPointFor3;			// 3�� Ÿ�� ȸ��
	int		speed;						// ���� �ӵ�
	char	combination;				// ���� �迭
	float	distance;					// ���� �Ÿ�
	float	radius;						// ���� ����
	float	finishAttackRange;			// Finish attack range
	float	finishAttackSpeed;			// Finish attack speed
	char	FERA;						// ���� �Ӽ�
	char	FERP;						// ���� �Ӽ�
	char	specialType;				// ���� �Ӽ�
	short	visualEffect;				// ����Ʈ
	short	stance_male;				// �⺻ �ڼ� - ����
	short	stance_female;				// �⺻ �ڼ� - ����
	short	motion_male;				// ��� ���� - ����
	short	intermotion1_male;			// ���� ����1 - ����
	short	intermotion2_male;			// ���� ����2 - ����
	short	motion_female;				// ��� ���� - ����
	short	intermotion1_female;		// ���� ����1 - ����
	short	intermotion2_female;		// ���� ����2 - ����

#ifdef _XDEF_NEWSTRINGMANAGER	
	LPTSTR	concept;					// ��� ����
#else
	TCHAR	concept[128];				// ��� ����
#endif
	
	_XSKILLDETAILINFOSTRUCTURE		detailInfo[12];		// ���� ����

	// skill ���� ���� - for tooltip
#ifdef _XDEF_NEWSTRINGMANAGER
	LPTSTR	information;
	int		wordorder;
#else
	TCHAR	information[512];
#endif
	char	parameter[10];
	
}_XSKILLINFOSTRUCTURE;


enum
{
	en_mod_target_type = 0,
	en_mod_distance,
	en_mod_min_dam,
	en_mod_max_dam,
	en_mod_attackrate,
	en_mod_avoidrate,
	en_mod_costinner,
	en_mod_costmind,
	en_mod_finishspeed,
	en_mod_castingtime,
	en_mod_delay,
	en_mod_cooldown,
	en_mod_incre_effecttime,
	en_mod_additive_1_value,
	en_mod_additive_2_value,
	en_mod_additive_3_value,
	en_mod_additive_4_value,
	en_mod_additive_1_prob,
	en_mod_additive_2_prob,
	en_mod_additive_3_prob,
	en_mod_additive_4_prob,
	en_mod_additive_1_time,
	en_mod_additive_2_time,
	en_mod_additive_3_time,
	en_mod_additive_4_time,
	en_mod_total_count
};

// ����ȿ���� ���� ��ų �� ������ ���� ���̺� - Ŭ���̾�Ʈ���� ����� �͸� ����
typedef struct  
{
	short	minAttackDamage;	// �ּ� ���ݷ�
	short	maxAttackDamage;	// �ִ� ���ݷ�
	short	attackRating;		// ���� ����ġ
	short	avoidRating;		// ȸ�� ����ġ
	short	costForce;			// �Ҹ� ����
	short	costMind;			// �Ҹ� ����
	float	finishSpeed;		// Finish speed
	int		casting;			// ���� �䱸 �ð�
	int		delay;				// ���� ���� �ð�
	int		coolDown;			// �� ���� �ð�
	
	unsigned short			effectTime;				// ȿ�� ����ð�
	_XSI_SkillAdditive		additiveEffect[4];		// additive ȿ��

}_XSKILLDETAILINFO_MOD_STRUCTURE;

struct _XSKILLINFO_MOD_STRUCTURE
{
	char	targetType;				// ��� ����
	float	distance;				// ���� �Ÿ�

	_XSKILLDETAILINFO_MOD_STRUCTURE detailInfo[12];

	BOOL	bUse[en_mod_total_count];			// ��� ���� ����

	_XSKILLINFO_MOD_STRUCTURE() : targetType(0), distance(0)
	{
		memset(detailInfo, 0, sizeof(detailInfo));
		memset(bUse, 0, sizeof(bUse));
	}
};

// ����ȿ���� ���� ��ų ���̺� ���� ������ ���� ����ü
extern map <short, _XSKILLINFO_MOD_STRUCTURE*> g_Skill_Mod_Property;

// Skill  ���� ����
typedef struct _XSI_LearnSkill_Item
{
	int		nCategory;
	short	sID;
}_XSI_LEARNSKILL_ITEM;

typedef struct _XSI_LearnSkill_Package
{
	TCHAR					cPackageName[32];
	int						nSkillCount;
	_XSI_LEARNSKILL_ITEM*	skillItem;
}_XSI_LEARNSKILL_PACKAGE;


extern map <short, _XSKILLINFOSTRUCTURE*>		g_SkillProperty;
extern int										g_SkillGroupStartIndex[12];
extern map <int, _XSI_LEARNSKILL_PACKAGE>		g_SkillPackage;


class _XSkillItem
{	
public:
	_XSkillItem();
	virtual ~_XSkillItem();
	
	static BOOL			LoadSkillProperty( void );
	static BOOL			LoadSkillDetailProperty(FILE* fileptr, _XSKILLINFOSTRUCTURE* skillinfo);	
	static void			ReleaseSkillProperty(void);

	static void			ReleaseSkillModStructure(void);

	static BOOL			ReLoadSkillProperty( void );

	static BOOL			LoadSkillPackage(void);
	static void			ReleaseSkillPackage(void);

	static BOOL			LoadSkillMasteryProperty(void);
	static void			ReleaseSkillMasteryProperty(void);
	static BOOL			LoadSkillMasteryDisplayProperty(void);
	static void			InitializeStructure(void);

	static BOOL			LoadSkillConceptProperty(void);
	static BOOL			LoadMasterySkillConceptProperty(void);
	static BOOL			ParseClanConcept(FILE* fileptr);
	static BOOL			ParseClassConcept(FILE* fileptr);
	static BOOL			ParseSTypeConcept(FILE* fileptr);
	static BOOL			ParseSkillConcept(FILE* fileptr);
	static BOOL			ParseMasterySkillConcept(FILE* fileptr);
	static char			GetParameter(char* arg);

	static short		GetParameterValue(short skillid, char skilllevel, int param);
	static short		GetMasteryParameterValue(short skillid, char skilllevel, int param);
	
	static BOOL			FindSkillProperty(short skillid);
	static BOOL			FindMasterySkillProperty(short skillid);

	static void			ResetSkillModTable(void);

	static char			GetTargetType(short skillid);
	static float		GetDistance(short skillid);

	static short		GetMinAttackDamage(short skillid, char level);
	static short		GetMaxAttackDamage(short skillid, char level);
	static short		GetAttackRating(short skillid, char level);
	static short		GetAvoidRating(short skillid, char level);
	static short		GetCostForce(short skillid, char level);
	static short		GetCostMind(short skillid, char level);
	static float		GetFinishSpeed(short skillid, char level);
	static int			GetCasting(short skillid, char level);
	static int			GetDelay(short skillid, char level);
	static int			GetCoolDown(short skillid, char level);

	static unsigned short	GetEffectTime(short skillid, char level);
	static short			GetAdditiveValue(short skillid, char level, int index);
	static short			GetAdditiveProb(short skillid, char level, int index);
	static short			GetAdditiveCount(short skillid, char level, int index);
	
	static BOOL			LoadBattleParameter(void);
	static BOOL			GetCommand(char* cmd, FILE* fileptr, bool bnumber = false);
};


// ============================================================================
// Matsery skill
// ============================================================================
struct _XSI_MasterySkillDisplayStruct		// for mastery window
{
	int			skillid;
	int			needprev;
};

//extern _XSI_MasterySkillDisplayStruct	g_SkillMasteryDisplayProperty[2000];
//extern map <int, TCHAR*>		g_SkillMasteryNameProperty;


struct _XSI_MasteryDetailStruct
{
	short				sReqMp;				// ����Ʈ�� �ø��� ���� �ʿ��� �����͸� ����

	char				effectCount;
	_XSI_SkillEffect*	effect;				// �ִ� 20�ܰ��� ����Ʈ ����Ʈ�� ����	
	
	char				additiveCount;
	_XSI_SkillAdditive*	additiveEffect;		// �ִ� 20�ܰ��� additive ����Ʈ ����Ʈ�� ����
};


struct _XSI_MasterySkillStruct
{
	short			sMasteryID;				// ���� ���̵�
#ifdef _XDEF_NEWSTRINGMANAGER		
	LPTSTR			cName;					// ���� �̸�
#else
	char			cName[25];				// ���� �̸�
#endif

	short			sTreeLevel;				// ���� Ʈ�� �ܰ�

	char			cClanType;				// ���� �з�
	char			cClass;					// ���� �з�
	char			cClassGrade;			// ���� ���

	char			cMasteryType;			// ���� �з� - ������ 1, 2, 3

	char			cMaxMP;					// ���� �� �ִ� �ִ� ����Ʈ
	
	short			sNeedMasteryID;			// �䱸 ��� : �ݵ�� ������ �ؾ� ��
	short			sNeedPageTotalPoint;	// ó�� ���� �� �ʿ��� �������� ��Ż ����Ʈ
	
	char			cApplyType;				// ���� ���
	
#ifdef _XTS_MASTERY_04
	short			sAddSkill;				// �߰� ����
#endif

	char			cTargetSkill_Stype;		// ����� �Ǵ� �迭 ����

	short			sTargetSkill_1;			// ��� ���� 1
	short			sTargetSkill_2;			// ��� ���� 2
	short			sTargetSkill_3;			// ��� ���� 3
	short			sTargetSkill_4;			// ��� ���� 4

	char			cWeaponSType;			// ��� ���� Second Type
	char			cWeaponTType;			// ��� ���� Third Type

	char			cInside_Type;			// ���� �Ӽ�
	char			cSpecial_Type;			// Ư�� �Ӽ�

#ifdef _XDEF_NEWSTRINGMANAGER		
	LPTSTR			pContext;				// ����
#else
	char			pContext[128];			// ����
#endif

	_XSI_MasteryDetailStruct	detailInfo[20];

	// skill ���� ���� - for tooltip
#ifdef _XDEF_NEWSTRINGMANAGER		
	LPTSTR	information;
	int		wordorder;
#else
	TCHAR	information[512];
#endif
	char	parameter[14];
};

extern map <short, _XSI_MasterySkillStruct*>	g_SkillMasteryProperty;

class _XMasterySkillItem
{
public :
	short			skillid;
	char			cPoint;

public :
	_XMasterySkillItem()
	{
		skillid = 0;
		cPoint = 0;
	}
};

// ============================================================================
// Skill Concept - for skill window
// ============================================================================
struct _XSI_SkillConceptStruct
{
#ifdef _XDEF_NEWSTRINGMANAGER
	LPTSTR					clan_concept;
	LPTSTR					stype_concept[30];
	LPTSTR					stype_name[30];
#else
	TCHAR					clan_concept[512];
	TCHAR					stype_concept[30][512];
	TCHAR					stype_name[30][64];
#endif

	short					class_stype[10][30];

	_XSI_SkillConceptStruct()
	{
#ifdef _XDEF_NEWSTRINGMANAGER
		clan_concept	=	NULL;
		memset( stype_concept, 0, sizeof(LPTSTR) * 30 );
		memset( stype_name, 0, sizeof(LPTSTR) * 30 );
#else
		memset(clan_concept, 0, sizeof(clan_concept));
		memset(stype_concept, 0, sizeof(stype_concept));
		memset(stype_name, 0, sizeof(stype_name));
#endif
		memset(class_stype, 0, sizeof(class_stype));
	}
};

extern _XSI_SkillConceptStruct	g_SkillConceptProperty[9];

// ============================================================================
// ������ ������ ��ų
// ============================================================================
class _XUserSkill
{
public:
	short			m_sSkillID;					// ���� ���̵�
	char			m_cSkillLevel;				// ���� �ܰ�
	
	int				m_iSkillExp;				// ���� ����ġ
	char			m_cCurMission;				// �̼� ���� ī��Ʈ
	
	DWORD			m_dwCooldownStartTime;		// Cool down start time
	DWORD			m_dwCooldownTime;			// Cool down time

	unsigned char	m_ucSkillState;
	
	char			m_cSelectedSkillLevel;		// �����Կ��� ������ ��
	char			m_cOnOff;					// 0:Off  1:On	passive��

public :
	_XUserSkill();
	virtual ~_XUserSkill();
};

#endif // !defined(_XSKILLITEM_H_)
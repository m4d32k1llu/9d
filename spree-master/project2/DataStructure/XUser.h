// XUser.h: interface for the XUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XUSERINFO_H_
#define _XUSERINFO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XObjectManager.h"
#include "XLodTerrain.h"
#include "XActionTable.h"
#include "XBipedController.h"
#include "XGameItem.h"
#include "XSkillItem.h"
#include "XWindow_ChatBalloon.h"
#include "XCharacterState.h"
#include "XNetwork.h"

#include "XMemoryPool.h"
#include <map>

#ifdef _XDWDEBUG
#define VIEWBATTLELOG if(g_ViewBattleLog)
#else
#define VIEWBATTLELOG _XCOMMENT
#endif

// -------------------------------------------------------------------------------------------
// ���� ���� Define 

// ĳ���� �⺻ ������ 
#define _XDEF_MAX_CHARACTERCOUNT			3		// ������ �����Ҽ� �ִ� �ִ� ĳ���� ��	
#define _XDEF_MAX_USERNAMESTRINGLENGTH		13		// �ִ� ĳ���� �̸� ����

//	���ۺ� ü�� ������ 
#define _XDEF_HEALTHINCREASERATE_IDLE		4.0f	// ������ ������
#define _XDEF_HEALTHINCREASERATE_WALK		4.0f	// �ȱ�� ������
#define _XDEF_HEALTHDECREASERATE_RUN		1.0f	// �ٱ�� ������

// ����� ����Ҽ� �ִ� minimum ���� percentage
#define _XDEF_RUN_LIMITSTLENGTH				20.0f

//	�̵� �ӵ�/���� ���� 
#define _XDEF_USERPOSITION_EPSILON			0.1f
#define _XDEF_CHARACTERFRICTION				250.0f	// ĳ���Ϳ� ����� ������
#define _XDEF_CHARACTERGRAVITY				-16.0f	// ĳ���Ϳ� ����� �߷°� 
#define _XDEF_MAXJUMPVELOCITY				125.0f
#define _XDEF_MAXJUMPHEIGHT					6.0f

// ��ŷ ����
#define _XDEF_COLLIDERADIUS					0.55f
#define _XDEF_COLLIDEOFFSET_HUMAN			0.6f

// ���� ���� �ִ�/�ּ�ġ 
#define _XDEF_HELATHSYSTEM_VITALITYMIN		10.0f	// ���� �ּҰ�
#define _XDEF_HELATHSYSTEM_VITALITYMAX		150.0f	// ���� �ִ밪

#define _XDEF_HELATHSYSTEM_ZENMIN			10.0f	// ���� �ּҰ�
#define _XDEF_HELATHSYSTEM_ZENMAX			150.0f	// ���� �ִ밪

#define _XDEF_HELATHSYSTEM_SOULMIN			10.0f	// ���� �ּҰ�
#define _XDEF_HELATHSYSTEM_SOULMAX			150.0f	// ���� �ִ밪

#define _XDEF_HELATHSYSTEM_HEARTMIN			10.0f	// �ɱ� �ּҰ�
#define _XDEF_HELATHSYSTEM_HEARTMAX			150.0f	// �ɱ� �ִ밪

#define _XDEF_HELATHSYSTEM_JUNGGIMIN		10.0f	// ���� �ּҰ�
#define _XDEF_HELATHSYSTEM_JUNGGIMAX		150.0f	// ���� �ִ밪

// ĳ���� ���� ����	
#define _XDEF_SENDTIME_IDLEMOTION			8000	// ������ ������ ��Ŷ ���� ����
#define _XDEF_CHANGEIDLEMOTIONTIME			10000	// Ư�� ���� ���� ����

// �ִ� ��Ƽ����
#define _XDEF_MAXPARTYMEMBERCOUNT			9

// �ִ� ���� ��ȣ ��
#define _XDEF_NICKNAME_TOTALCOUNT		212
// ---------------------------------------------------------------------------------------
// ���� ���� enumeration type

// ����
typedef enum tagGroupType
{
	_XGROUP_NANGIN			= 0,
	_XGROUP_GAEBANG,
	_XGROUP_BEEGOONG,
	
	_XGROUP_SORIM,
	_XGROUP_NOCKRIM,
	_XGROUP_MOODANG,

	_XGROUP_MAKYO,
	_XGROUP_SEGA,
	_XGROUP_NOFIXED,
	
	_XGROUP_MAXGROUPCOUNT
} _XGROUPTYPE;

enum
{
	_XGROUPTYPE_NONE = 0,
	_XGROUPTYPE_WHITE,	// �鵵
	_XGROUPTYPE_BLACK	// �浵
};

typedef enum tagClassType
{
	_XROLLCLASS_NONE		=0, // 0 ���� 
	_XROLLCLASS_BATTLE,			// 1 ������
	_XROLLCLASS_HEALER,			// 2 ġ����
	_XROLLCLASS_COMPLETE,		// 3 ������
	_XROLLCLASS_SPELLER,		// 4 ���
	_XROLLCLASS_MURDER,			// 5 ���
	_XROLLCLASS_HOSIM,			// 6 ȣ��
	_XROLLCLASS_JUSOOL,			// 7 �ּ�
	_XROLLCLASS_WOIGONG,		// 8 �ܰ�
	_XROLLCLASS_SOHWAN,			// 9 ��ȯ
	_XROLLCLASS_SUNDO,			// 10 ����
	_XROLLCLASS_MADO,			// 11 ����
	_XROLLCLASS_MAXCOUNT
} _XROLLCLASSTYPE;

// ����
typedef enum tagGenderType
{
	_XGENDER_MAN	= 0,
	_XGENDER_WOMAN
} _XGENDERTYPE;

// �������/��ȭ���/PK���
typedef enum tagBattleType
{
	_XBATTLE_PEACE = 0,
	_XBATTLE_BATTLE,
	_XBATTLE_PK
} _XBATTLEMODETYPE;

// ���� Ÿ��
typedef enum tagPickingTargetType
{
	_XPICKINGTARGET_NONE = 0,
	_XPICKINGTARGET_PC,
	_XPICKINGTARGET_MOB,
	_XPICKINGTARGET_NPC,
	_XPICKINGTARGET_ITEM,
	_XPICKINGTARGET_DUMMY,
	_XPICKINGTARGET_SEAT,
	_XPICKINGTARGET_SELF,
	_XPICKINGTARGET_COLLECT,// by mahwang
	_XPICKINGTARGET_FUNCTIONALOBJECT
} _XPICKINGTARGETTYPE;

// �̵� ���� Ÿ��
typedef enum tagMoveType
{
	_XMOVETYPE_RUN = 0,
	_XMOVETYPE_KYUNGGONG
} _XMOTION_MOVETYPE;

// ���� ���ۺ� Ÿ��
typedef enum tagMotionclassType
{
	_XACTION_NONE	=	0,
	_XACTION_IDLE,
	_XACTION_MOVE,
	_XACTION_ATTACK,
	_XACTION_DEFENCE,
	_XACTION_BLOW,
	_XACTION_COLLAPSE,
	_XACTION_DIE,	
	_XACTION_REBIRTH,
	_XACTION_ENTERING,
	_XACTION_DYING,
	_XACTION_RESCUE,
	_XACTION_SPECIAL,
	_XACTION_TALK,
	_XACTION_MATCH_START,
	_XACTION_MATCH_CANCEL,
	// by mahwang
	_XACTION_COLLECT,
	_XACTION_FIREWOODCUT,
	_XACTION_WATERDRAW,
	_XACTION_LPS,
	_XACTION_INSENSIBLE			// ���� - ����ȿ��
	
} _XMOTIONCLASS_TYPE;

// ȸ�� ����
typedef enum tagRotateDirection
{
	_XROTATE_NONE = 0,
	_XROTATE_CLOCKWISE,
	_XROTATE_COUNTCLOCKWISE
}_XROTATE_DIRECTION;

typedef enum
{
	_XEN_4EFF_NONE,
	_XEN_4EFF_MINUS = 1,
	_XEN_4EFF_PLUS,
	_XEN_4EFF_HON,
	_XEN_4EFF_SIN
}_XEN_4EFF_SYS;

#define _XDEF_FASTMOVEINDEX_NONE			0
#define _XDEF_FASTMOVEINDEX_SHORTWEAPON		3
#define _XDEF_FASTMOVEINDEX_LONGWEAPON		6
#define _XDEF_FASTMOVEINDEX_SWORD			9
#define _XDEF_FASTMOVEINDEX_BATTLESWORD		12

// ---------------------------------------------------------------------------------------
// ���� LUT Structure
typedef struct
{
	DWORD level;					// ���� �ܰ�	�����Ա��� ~ XXX
	DWORD sublevel;					// ���� ����	1 ~ 12 ��
	DWORD innerlevel;				// �� ����		innerlevel...
	DWORD needjinexperience;		// �ʿ� �� ����ġ	
	DWORD needgongexperience;		// �ʿ� �� ����ġ	
	DWORD accumulatejinexperience;	// ���� �� ����ġ
} _XUSERLEVEL_LUTTYPE;

// ---------------------------------------------------------------------------------------
// Point table
typedef struct
{
	int vitality;
	int inner;
	int concenturate;
	
	int need_constitution;
	int need_zen;
	int need_intelligence;

	int need_strength;
	int need_dexterity;
} _XUSER_POINTTABLE;

// ---------------------------------------------------------------------------------------
// User model structure
typedef struct _XUSER_MODELDESCRIPTTYPE
{	
	short int 	face;				// ��	

	short int 	hair;				// �Ӹ���Ÿ��
	short int 	haircolor;			// �Ӹ� ����

	short int 	upperbody;			// ��ü
	short int 	lowerbody;			// ��ü 

	short int 	cap;				// ����

	short int 	lhand;				// �޼�
	short int 	rhand;				// ������
	
	short int 	lfoot;				// �޹�
	short int 	rfoot;				// ������ 

	short int 	adae;				// �ƴ�
	short int 	earring;			// �Ͱ���
	short int	backpack;			// �賶
	
	short int   mask;				// ����ũ, ���ȭ�ǻ�.

	short int 	item1;				// ���� ������ �ε���
	short int 	item2;
	short int 	item3;
	short int 	item4;
	short int 	item5;

	_XGI_FirstCategory 	upperbodyCategory;			// ��ü
	_XGI_FirstCategory 	lowerbodyCategory;			// ��ü 

	_XGI_FirstCategory 	capCategory;				// ����

	//_XGI_FirstCategory 	lhandCategory;				// �޼�
	//_XGI_FirstCategory 	rhandCategory;				// ������
	
	_XGI_FirstCategory 	lfootCategory;				// �޹�
	_XGI_FirstCategory 	rfootCategory;				// ������ 

	_XGI_FirstCategory 	adaeCategory;				// �ƴ�
	//_XGI_FirstCategory 	earringCategory;			// �Ͱ���
	_XGI_FirstCategory 	backpackCategory;			// �賶
	_XGI_FirstCategory 	maskCategory;				// ġ�� 
	_XGI_FirstCategory 	item1Category;				// ���� ������ �ε���
	
	
} _XUSER_MODELDESCRIPTTYPE;

// ---------------------------------------------------------------------------------------
// User animation structure
typedef struct 
{	
	_XMOTIONCLASS_TYPE		motionclass;		// �׼� Ŭ����
	
	_XBATTLEMODETYPE		idletype;			// ��� Ÿ��
	_XMOTION_MOVETYPE		movetype;			// �̵� Ÿ��
	DWORD					attacktype;			// ���� Ÿ��
} _XUSER_ANIMATIONDESCRIPTTYPE;

// ---------------------------------------------------------------------------------------
// ������ ���� ����
struct _XUSERORGANIZATIONINFO
{
	int				m_iIndex;				
	char			m_cName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;			// ���� �̸�	
	char			m_cProperty ;			// ���� �Ӽ�(���� | ��ȸ)	
	char			m_cClass ;				// ���
	enum{
		_UNIT_CLASS_DEFAULT_UNIT_,			// �⺻ ������
		_UNIT_CLASS_SPECIAL_UNIT_,			// ��� ������
		_UNIT_CALSS_CAPTAIN_UNIT_,			// ���� ������..
	} ;
	
	inline void Reset()
	{
		m_iIndex = 0;
		memset( m_cName, 0, sizeof( TCHAR ) * 9 );
		m_cProperty = 0;
		m_cClass = 0;
	};
};

//������ ��ȸ ���� (������ ����)
struct _XUSERALIENCEINFO
{
	int				m_iIndex;				
	char			m_cName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;			// ���� �̸�	
	char			m_cProperty ;			// ���� �Ӽ�(���� | ��ȸ)	
	char			m_cClass ;				// ���
	enum{
		_UNIT_CLASS_NO_UNIT_,				// ���� ����� ������ ���� �ʴ�. ������ �޾Ƽ� ����� �����ؾ� �Ѵ�.
		_UNIT_CLASS_DEFAULT_UNIT_,			// �⺻ ��ȸ��
			_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_,	// ������ ���� ��ȸ��(������ ����)
			_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_,		// ������ ���� ȸ��
	} ;
	
	inline void Reset()
	{
		m_iIndex = 0;
		memset( m_cName, 0, sizeof( TCHAR ) * 9 );
		m_cProperty = 0;
		m_cClass = 0;
	};
};

// ---------------------------------------------------------------------------------------
// ĳ���� ���� ���� ����
class _XCHARACTER_INFO
{
public:

	// ĳ���� �⺻ ������	
	TCHAR							charactername[_XDEF_MAX_USERNAMESTRINGLENGTH]; // �������̵�
	int								m_NickNameList[_XDEF_NICKNAME_TOTALCOUNT]; //�ڱ� �ڽ��� ������ �ִ� ��ȣ ����Ʈ
	TCHAR							m_NickName[256]; //�ڱ� �ڽ��� ������ �ִ� ��ȣ �̸�
	int								m_CurrentNIckIndex;	// ���õ� ��ȣ �ε���
	_XGROUPTYPE						groupindex;							 // ĳ���� ����
	_XGENDERTYPE					gender;								 // ĳ���� ����

	// �� ����
	DWORD							zoneuniqid;							 // �� ����ũ ���̵�

private:
		// ���� ����
	_XDEF_ENCVALUE(short, constitution )						 // �ǰ� <= ���� 
	_XDEF_ENCVALUE(short, zen )									 // ���� <= �״�� 
	_XDEF_ENCVALUE(short, intelligence )						 // ���� <= ���� 
	_XDEF_ENCVALUE(short, dexterity )							 // ��ø <= �ɱ� 
	_XDEF_ENCVALUE(short, strength )							 // �ٷ� <= ����	
	
	// ���� Power
	_XDEF_ENCVALUE(short, current_lifepower )					 // ���� ����
	_XDEF_ENCVALUE(short, current_forcepower )					 // ���� ����
	_XDEF_ENCVALUE(short, current_concentrationpower )			 // ���� ����
	
	// �ִ� Power
	_XDEF_ENCVALUE(short, max_lifepower )						 // �ִ� ����
	_XDEF_ENCVALUE(short, max_forcepower )						 // �ִ� ����
	_XDEF_ENCVALUE(short, max_concentrationpower )				 // �ִ� ����

	_XDEF_ENCVALUE(short, externalwoundrate )					 // �ܻ� 
	_XDEF_ENCVALUE(short, internalwoundrate )					 // ���� 
	_XDEF_ENCVALUE(WORD, fatiguerate )							 // �Ƿε� 
	
	_XDEF_ENCVALUE(short, furyparameter )						 // �г�
	
	_XDEF_ENCVALUE(short, leftpoint )							 // �ܿ� ������Ʈ
	_XDEF_ENCVALUE(short, leftmasterypoint )					 // �ܿ� �����͸� ����Ʈ

	_XDEF_ENCVALUE(short, retribution )							 //	����
	_XDEF_ENCVALUE(int, honor )	 								 //��  8
	_XDEF_ENCVALUE(short, showdown )							 // �� ����
	
	// ���� ����	
	_XDEF_ENCVALUE(DWORD, jinlevel )							 // �� ����ġ
#ifdef _XTS_FAME
	_XDEF_ENCVALUE(int, Notoriety )								 // �Ǹ�
#else
	_XDEF_ENCVALUE(DWORD, gonglevel )							 // �� ����ġ
#endif
	
	_XDEF_ENCVALUE(DWORD, level )								 // ���� ���� �ܰ� : �����Ա��� ~ XXX
	_XDEF_ENCVALUE(DWORD, sublevel )							 // ���� 12��
	_XDEF_ENCVALUE(DWORD, innerlevel )							 // ���� ���� : ���� ���̸� �ö�.4

	_XDEF_ENCVALUE(char, cClass )								 // ����
	_XDEF_ENCVALUE(char, cClassGrade )							 // ���� ���
	_XDEF_ENCVALUE(int, iContribution )			   				 // ���� �⿩��

	_XDEF_ENCVALUE(int, gm_level )								 // gm level.


/*
	// ���� ����
	short							constitution;						 // �ǰ� <= ���� 
	short							zen;								 // ���� <= �״�� 
	short							intelligence;						 // ���� <= ���� 
	short							dexterity;							 // ��ø <= �ɱ� 
	short							strength;							 // �ٷ� <= ����	
	
	// ���� Power
	short							current_lifepower;					 // ���� ����
	short							current_forcepower;					 // ���� ����
	short							current_concentrationpower;			 // ���� ����
	
	// �ִ� Power
	short							max_lifepower ;						 // �ִ� ����
	short							max_forcepower ;					 // �ִ� ����
	short							max_concentrationpower ;			 // �ִ� ����

	short							externalwoundrate;					 // �ܻ� 
	short							internalwoundrate;					 // ���� 
	unsigned short					fatiguerate;						 // �Ƿε� 
	
	short							furyparameter;						// �г�
	
	short							leftpoint;							 // �ܿ� ������Ʈ
	short							leftmasterypoint;					// �ܿ� �����͸� ����Ʈ

	short							retribution;						 //	����
	int								honor;								//��  8
	short							showdown;							 // �� ����
	
	// ���� ����	
	DWORD							jinlevel;							 // �� ����ġ
#ifdef _XTS_FAME
	int								Notoriety;							 // �Ǹ�
#else
	DWORD							gonglevel;							 // �� ����ġ
#endif
	
	DWORD							level;								 // ���� ���� �ܰ� : �����Ա��� ~ XXX
	DWORD							sublevel;							 // ���� 12��
	DWORD							innerlevel;							 // ���� ���� : ���� ���̸� �ö�.4

	char							cClass;								 // ����
	char							cClassGrade;						 // ���� ���
	int								iContribution;			   			 // ���� �⿩��
*/
public:
	// Zero Level ������ ���߿� ������ ��
	DWORD							maxattacklevel;						 // ���ݷ�
	DWORD							minattacklevel;						 // ���ݷ�
	DWORD							attacksuccesslevel;					 // ���� ����ġ
	DWORD							defencelevel;						 // ����
	
	// �� ����
	int								winscore;
	int								losescore;
	int								tiescore;
	int								seriesscore;
	int								max_series_wincount;
	int								max_series_losecount;
	int								abusecount;
	int								prevmatchresult;
	
	int								blockingenddate;
//#ifdef _XTS_SERVERUNIFICATION	// ���� �Ϸ� ���Ŀ��� ��� ����
#ifdef _XTS_NEWCHARACLIST
	int								ucChangeNameFlag;	///< �ߺ��̸� üũ�÷��� 0 - Not, 1 - Possible Change Name 
	int								ucLeftFlag;			// ���� �Ⱦ���.
#endif
	char							cHindingMode ;			// ��ŷ��ȣ ���� :  0 - ��������, 1 - ���ż�������
		
	D3DXVECTOR2						lastposition;						//	ĳ������ ������ġ
	unsigned char					weaponStrength;			// ���� ���� ��ġ 
	
	// ��
	_XUSER_MODELDESCRIPTTYPE		modeldescript;
	_XUSER_ANIMATIONDESCRIPTTYPE	animationdescript;
	
	DWORD							conditionState;						// Condition (����ȿ��)
	
	_XUSERORGANIZATIONINFO			m_OrganizationInfo;
	
	_XUSERALIENCEINFO				m_AlienceInfo;	//��ȸ ����(�����ܵ���)
	
	// ����ȥ�� ���ݷ��� �ִ밪
	_XEN_4EFF_SYS					m_4Eff_MaxDam;

	// ����ȥ�� ���׷�
	BOOL							m_b4Eff_ResPlus;
	BOOL							m_b4Eff_ResMinus;
	BOOL							m_b4Eff_ResHon;
	BOOL							m_b4Eff_ResSin;
	
	int								m_nClothView;

	// Constructor...
	_XCHARACTER_INFO()
	{
		memset( this, 0, sizeof(_XCHARACTER_INFO) );
		animationdescript.idletype	  = _XBATTLE_PEACE;
		animationdescript.motionclass = _XACTION_IDLE;
		animationdescript.attacktype  = _XAN_NI_ATTACK_KK;
		animationdescript.movetype    = _XMOVETYPE_RUN;
		m_OrganizationInfo.Reset();
		m_AlienceInfo.Reset();

		Set_constitution(0);
		Set_zen(0);						
		Set_intelligence(0);
		Set_dexterity(0);					
		Set_strength(0);
		Set_current_lifepower(0);
		Set_current_forcepower(0);
		Set_current_concentrationpower(0);
		Set_max_lifepower(0);
		Set_max_forcepower(0);
		Set_max_concentrationpower(0);
		Set_externalwoundrate(0);
		Set_internalwoundrate(0);			
		Set_fatiguerate(0);
		Set_furyparameter(0);
		Set_leftpoint(0);
		Set_leftmasterypoint(0);
		Set_retribution(0);
		Set_honor(0);
		Set_showdown(0);
		Set_jinlevel(0);
#ifdef _XTS_FAME
		Set_Notoriety(0);
#else
		Set_gonglevel(0);
#endif
		Set_level(0);
		Set_sublevel(0);					
		Set_innerlevel(0);
		Set_cClass(0);
		Set_cClassGrade(0);
		Set_iContribution(0);
		Set_gm_level(0);

		m_nClothView = 0;
	}

};

typedef struct _XUser_AttackResult
{
	unsigned short			usDefenderID;

	unsigned char			ucToType;
	unsigned char			ucSequence;	
	
	char					cSkillLevel;	// 0~11
	DWORD					dwChargeTime;   // ms

	int						skillType;
	short					sSkillID;
	FLOAT					fX;
	FLOAT					fZ;
	FLOAT					fRange;

	short					sAnimation;

	BOOL					display[3];
	unsigned short			usAtkInfo[3];
	
	unsigned short			sDamage[3];

	char					cDefenderVitalRate[3];

	short					sAddEffect[3];
	short					sEff_Time[3];
	
	char					cBattleResult;
}_XUSER_ATTACKRESULT;

typedef enum _XUser_AttackSequence
{
	_XDEF_ATK_SEQ_NONE = 0,
	_XDEF_ATK_SEQ_FIRST,
	_XDEF_ATK_SEQ_SECOND,
	_XDEF_ATK_SEQ_THIRD
}_XUSER_ATTACKSEQUENCE;

typedef enum _XUser_PersonalStoreState
{
	_XUSER_PERSONALSTORE_NONE = 0,
		_XUSER_PERSONALSTORE_OWNER_OPEN,
		_XUSER_PERSONALSTORE_OWNER_PREPARE,
		_XUSER_PERSONALSTORE_OWNER_MODIFY,
		_XUSER_PERSONALSTORE_CLIENT_INTEREST,
		_XUSER_PERSONALSTORE_CLIENT_MODIFY
}_XUSER_PERSONALSTORESTATE;

struct _XRegionAtkResult
{
	int					nObjectType;
	int					nID;
};

struct _XUser_RegionDamage
{
	BOOL				bValid;
	int					nObjectCnt;
	int					nSkillID;

	_XRegionAtkResult	sResult[_COUNT_REGION_ATK_RESULT_];

	_XUser_RegionDamage() : bValid(FALSE), nObjectCnt(0), nSkillID(0)
	{
		memset(sResult, 0, sizeof(sResult));
	}
};

struct MnDMasterInfo //������ ���� ����  //Author : ����� //breif : �����ý���
{
	int nMnDClass; //����, ���� ����

	int nMasterIndex; //������ �ε���
	int nMasterRank; //���� ���

	TCHAR szMasterName[64]; // ���� �̸�
	unsigned int nMasterContribution; //���¿��� ���� �⿩��
	unsigned int nMasterTotalContribution; //���� �⿩�� (������ �⿩��)
	unsigned int nTotalKillCount; //���������� ���� ų ī��Ʈ

	unsigned int nLocalUserContribution; //���� �⿩��
	unsigned int nCurKillCount; //������ ���� ī��Ʈ

	int nMasterConnectState;	// 0 - �α׾ƿ�, 1 - ������.
	int nBonusSkillStep;
	int nSex;
	int nLevel;              
	int nMoonpa;
	int nClass;
	int nClassGrade;                                                

	int nTotalNumber; //�ο���
	TCHAR szMasterNickName[64]; // ��ȣ

	MnDMasterInfo::MnDMasterInfo()
	{
		nMnDClass = 0;
		nMasterRank = 0;
		memset( szMasterNickName, 0, sizeof(szMasterNickName));
		nTotalNumber = 0;
		memset( szMasterName, 0, sizeof(szMasterName));
		nMasterContribution = 0;
		nLocalUserContribution = 0;
		nMasterTotalContribution = 0;
		nMasterRank = 0;

		nMasterConnectState = 0;
		nBonusSkillStep = 0;
		nSex = 0;
		nLevel = 0;
		nMoonpa = 0;
		nClass = 0;
		nClassGrade = 0;
	}
};

class _XMob;

// ---------------------------------------------------------------------------------------
// by mahwang
// ������ _XItem �� ��� ���� �ʾƵ� ��� ������ ���� ������ ����Ʈ üũ �ϴ� �κ��� �־ 
// �׳� ���� �д�.

// �⺻ ���� Ŭ����
class _XUser : public _XItem
{

// Construction/Destruction
public:
	_XUser();
	~_XUser();

	void InitializeData();
	void DestroyData();	

// Attributes
public:
	
	BOOL					m_RecievedCharacterInfo;
	
	// ĳ���� �⺻	
	_XCHARACTER_INFO		m_CharacterInfo;
	unsigned short			m_Uniqueid;						// �α��μ����κ��� �Ҵ�Ⱦ��̵�	
	
	// ĳ���� ����
	BOOL					m_KillSignal;					//	ĳ���� ���� ����
	DWORD					m_LastRecivedMessageTime;
	
	// ĳ���� �̵�
	D3DXVECTOR3				m_Position;						// ���� ��ġ
	D3DXVECTOR3				m_PrevPosition;					// ���� ��ġ
	D3DXVECTOR3				m_TargetPosition;				// �̵� ��ġ 
	_XOBBData				m_CharacterOBB;
	D3DXVECTOR3				m_Velocity;
	
	FLOAT					m_RotateAngle;					// ĳ���� ȸ����	
	short					m_HiperRunSkillID;
	unsigned char			m_HiperRunStep;
	FLOAT					m_PrevProcessTime;				// ���� ������ Ÿ�̸�
	BOOL					m_CheckInFrustum;				// �������� üũ
	D3DXVECTOR3				m_MoveVector;					// �̵� ���� ����
	D3DXVECTOR3				m_CurrentPositionTerrainNormal; // ����ġ ���� �������� �븻����
	_XOBBData*				m_pCollideOBB;
	_XOBBData*				m_pGoundCollideOBB;
		
	// ĳ���� ��	
	_XModelDescriptor		m_ModelDescriptor;
		
	// ĳ���� ����
	_XBATTLEMODETYPE		m_BattleMode;					// ���� ���
	FLOAT					m_AttackRange;					// ���� ������ ���� ���� ����
	BOOL					m_StopAnimation;
	_XUSER_ATTACKSEQUENCE	m_CurrentAttackSequence;		// ���� �ʽ� - ���° �ʽ��ΰ�
	
	// ����
	_XItem*					m_AttackTarget;					// ���� ������ ���� Ÿ��
	_XPICKINGTARGETTYPE		m_AttackTargetObjectType;		// ���� ������ ���� Ÿ�� ����

	// ���� ���
	_XUSER_ATTACKRESULT		m_AttackResult;
	_XUser_RegionDamage		m_RegionDamageResult;			// ���������� ���

	// Finish skill ����
	BOOL					m_bFinishDashStart;
	FLOAT					m_FinishSpeed;
		
	// Chatting window
	_XWindow_ChatBalloon	m_ChatBalloonObject;

	// ���� ���� ����
	FLOAT					m_LastRotateAngle;
	_XROTATE_DIRECTION		m_RotateDirection;
	
	int						m_nIdleToMoveCnt;				// MSG_ZONE_MOVE���� ���
	
	// ���� ���� ����
	_XDisarmamentItem*		m_DisarmamentItem;
	
	// stance motion
	short					m_StanceMotionIndex;
	BOOL					m_StanceMotionFirst;
	
	// ��� ���� ���� ���� ����
	short					m_CurrentWeaponID;

	int						m_JobIndexOfZeroLevel;	// hotblood->2004.09.17
	
	// ĳ���� ���� ȿ��
	_XCharacterStateList	m_StateList;

	//ĳ���� ���� ����
	MnDMasterInfo			m_pLocalMnDInfomation; //Autor : ����� //breif : �����ý���

private:	
	// �̵� �ӵ�, ���� �ӵ�
	short					m_MoveSpeedFromEffect;
	short					m_AttackSpeedFromEffect;
	FLOAT					m_fMoveSpeed;					// �̵� �ӵ�	
	FLOAT					m_fPrevMoveSpeed;				// ���� �̵� �ӵ�	
	FLOAT					m_fHiperMoveSpeed;				// ��� �̵� �ӵ�

public:	
	short					Get_m_MoveSpeedFromEffect(){	return _XMEMORYCHECKDATA_ENCSHORT(m_MoveSpeedFromEffect); }
	void					Set_m_MoveSpeedFromEffect(short speed){ m_MoveSpeedFromEffect = _XMEMORYCHECKDATA_ENCSHORT(speed); }
	short					Get_m_AttackSpeedFromEffect(){	return m_AttackSpeedFromEffect; }
	void					Set_m_AttackSpeedFromEffect(short speed){ m_AttackSpeedFromEffect = _XMEMORYCHECKDATA_ENCSHORT(speed); }

	FLOAT					Get_m_fMoveSpeed(){ return (m_fMoveSpeed - 113.141592f); }
	void					Set_m_fMoveSpeed(FLOAT speed){ m_fMoveSpeed = speed + 113.141592f; }

	FLOAT					Get_m_fPrevMoveSpeed(){ return (m_fPrevMoveSpeed - 123.141592f); }
	void					Set_m_fPrevMoveSpeed(FLOAT speed){ m_fPrevMoveSpeed = speed + 123.141592f; }

	FLOAT					Get_m_fHiperMoveSpeed(){ return (m_fHiperMoveSpeed - 133.141592f); }
	void					Set_m_fHiperMoveSpeed(FLOAT speed){ m_fHiperMoveSpeed= speed + 133.141592f; }
	
public:

	// �����ϰ� - GM
	_XDEF_ENCVALUE(BOOL, m_bInvisible )
		
	// ���� 
	BOOL					m_bFaceMaskMode;

	// ���� ���� ���� ǥ��  
	_XUSER_PERSONALSTORESTATE m_PersonalStoreState;
	TCHAR					  m_PersonalStoreName[37];

	// �� ȫ�� ǥ��
	BOOL					m_bMatchPRMode;
	TCHAR					m_stringMatchPR[81];
	
	BOOL					m_bInPartyMember;
	
	BOOL					m_bEnemy;
	
	// ������ ��
	BOOL					m_bPEMode;
	
	// �ڸ� ��� ����
	BOOL					m_bOut;

	// �� ��ȸ �������ΰ�?
	BOOL					m_bEnterMatchEvent;	
	int						m_nWinnerNickName;
	
// static functions
public:
	static void				CreateIntersectionPoly( void );
	static BOOL				IntersectTriangle( D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2 );
	

// Get/Set functions
public:		
	virtual void			SetPosition( D3DXVECTOR3 position );	
	virtual void			SetAngle( FLOAT angle );
	virtual void			ResetProcessTime( void );
	
	virtual void			SetMoveSpeed( FLOAT movespeed )
	{
							Set_m_fPrevMoveSpeed(Get_m_fMoveSpeed());
							Set_m_fMoveSpeed(movespeed);
	}

	virtual void			PlayAttackSound( void );
	virtual void			PlayCriticalAttackSound( void );

	virtual void			SetAttackTarget( _XItem* target, _XPICKINGTARGETTYPE targettype );
	virtual void			SetBattleMode(  _XBATTLEMODETYPE mode );
	virtual void			ResetAttackTarget(void);

	virtual void			SetStopAction( void );
	virtual void			SetAnimation( _XMOTIONCLASS_TYPE motionclass, bool changemotion = true );
	virtual void			SetAnimationIndex( int animationindex, bool changemotion = true );
	virtual void			SetCharacterModel( BOOL dntchangemotion = FALSE );
	virtual void			ResetAnimationState( void );
	virtual void			UpdatePosition( void );
	virtual void			ResetZoneData( void );

	// Animation
	inline _XMOTIONCLASS_TYPE	GetMotionClass( void ){ return m_CharacterInfo.animationdescript.motionclass; }
	virtual void			SetDieMotion(void);
	virtual void			SetBlowMotion(void);
	virtual void			SetDefenceMotion(void);
		
// Operation functions
public:
	virtual BOOL			CheckIntersectMouseRay( void );	
	virtual void			Process( void );
	virtual void			Render( void );

	virtual void			CheckInsideFrustum( void );

// Pathfinder functions
public:

	FLOAT					m_PrevMoveTime;

	virtual BOOL			GetNextMove( D3DXVECTOR3& next );
	virtual bool			ProcessCollision(const D3DXVECTOR3& contactpoint, D3DXVECTOR3& movevelocity, D3DXVECTOR3& nextposition);
	
// Battle functions
	virtual void			ProcessBattleActiveSkill(void);
	virtual void			ProcessBattleChargeSkill(void);
	virtual void			ProcessBattleRoundingSpell(void);

	virtual void			ProcessBattleChannelingSpell(void);
	virtual void			ProcessBattleTargetChannelingSpell(void);
	virtual void			ProcessBattleSelfChannelingSpell(void);

	virtual void			ProcessBattleFinishSkill(void);
	virtual void			ProcessBattleCastingSpell(void);
	virtual void			ProcessBattleSmashingSkill(void);
	virtual void			ProcessBattleHiddenMA(void);
	virtual void			ProcessBattleActionSkill(void);
	
	BOOL					DoNotChangeMotion(void);

	// ���� ���� ����
	virtual void			UpdateRotateAngle(void);
	virtual void			ProcessMobDie(void);

	// stance motion
	void					SetStanceMotion(short index);
	
	// ���� ó�� ����
	virtual void			SetDyingMode(void);
	virtual void			SetDieMode(void);
	virtual void			SetRebirthBinsaMode(void);
	virtual void			SetRebirthHonsuMode(void);
	
	// MISC
	virtual void			DeleteWeaponModel(short skillid);
	virtual void			DeleteWeaponModel(void);
	virtual void			ReloadWeaponModel(void);

	virtual int				GetGroupType(void);

	// �Ÿ� ��Ż üũ
	void					SetFarAway(bool state, int index);
	
	virtual	BOOL			GetEnterMatchEvent(void);	// ��ȸ �������ΰ�?
	BOOL					GetFreeMatchState(void);	// �պ� ������ ��ġ ���ο� ������� ������ȸ �ѹ� ��������

// Overrides  functions
public:

};

//------------------------------------------------------------------------------------------	
// ���� ����Ʈ
typedef std::map<unsigned short, _XUser*>	smdef_XUser;

struct _XUSER_ORGANIZATION_GLOBAL_INFO 
{
	int				m_iIndex ;
	char			m_cName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;			// ���� �̸�
	char			m_cProperty ;			// ���� �Ӽ�(���� | ��ȸ)
	char			m_cBaseParty ;			// �Ҽ� ����
	unsigned int	m_uiExperience ;		// ���� ����ġ	
};

// �����̸� ����Ʈ 
typedef std::map<int, _XUSER_ORGANIZATION_GLOBAL_INFO* > smdef_OtganizationInfo;

struct _XUSER_ALIENCE_GLOBAL_INFO 
{
	int				m_iIndex ;
	char			m_cMasterName[13] ;			// ȸ�� �̸�
	char			m_cName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;			// ���� �̸�
	char			m_cProperty ;			// ���� �Ӽ�(���� | ��ȸ)
	char			m_cBaseParty ;			// �Ҽ� ����
	unsigned int	m_uiExperience ;		// ���� ����ġ	
};

// ��ȸ�̸� ����Ʈ 
typedef std::map<int, _XUSER_ALIENCE_GLOBAL_INFO* > smdef_AlienceInfo;

class _XUserList
{
private:
public:
	XMemoryPool<_XUser>		m_UserPool;
	smdef_XUser				m_smUserList;
	smdef_OtganizationInfo	m_sOrganizationInfoList;
	smdef_AlienceInfo		m_sAlienceInfoList;

	int						m_RenderUserZSortListLength;
	int						m_RenderUserZSortListCurrentLength;
	_XUser**				m_pRenderUserZSortList;

	
public:

	_XUserList();
	~_XUserList();

	void	disposeList(void);
	void	disposeList_zero(void);

	void	CreatePool(void);
	void	DeletePool(void);

private:
	_XUser*	NewUser();
	void	DeleteUser(_XUser* pUser);
	BOOL	AddUser( _XUser* user	 );	

public:
	_XUser* CreateUser(unsigned short usUniqueID, short sAnimationNumber,
								   FLOAT fListAngle, D3DXVECTOR3& vMoveVector,
								   D3DXVECTOR3& vSrcPosition, D3DXVECTOR3& vDestPosition);

	BOOL	AddUser_zero ( _XUser* user	)		{ return AddUser(user); }

	_XUser*	FindUser( LPSTR charactername );
	_XUser*	FindUser( unsigned short uniqueid );

	void	DeleteUser( LPSTR charactername );
	void	DeleteUser( int uniqueid );

	void    Process( void );
	void	Render( void );
	void	Process_ZeroLevel( void );
	void	RenderUserEffectAndToolTip( void );

	void	RemoveEffectInstance(int templateid);

	void	CheckInsideFrustum( void );
	void	AddOrganizationInfo( _XUSER_ORGANIZATION_GLOBAL_INFO* info );
	_XUSER_ORGANIZATION_GLOBAL_INFO*	SearchOrganizationInfo(int index);
	void AddAlienceInfo( _XUSER_ALIENCE_GLOBAL_INFO* info );
	_XUSER_ALIENCE_GLOBAL_INFO*	SearchAlienceInfo(int index);

	void	RemoveSortBuffer( _XUser* pUser );
	// �� �ܰ� ���ϱ�
	int		CalcHonorStep(int count);
};


//------------------------------------------------------------------------------------------	
// _XUser global extern value
extern _XUSERLEVEL_LUTTYPE		g_UserLevelTable[241];
extern _XUSER_POINTTABLE		g_UsePointTable[12];
extern D3DXVECTOR3				g_UserIntersectionPoly[8];
extern TCHAR					g_CharacterRollNameTable[_XGROUP_MAXGROUPCOUNT][_XROLLCLASS_MAXCOUNT][5][64];

#endif // !defined(_XUSERINFO_H_)

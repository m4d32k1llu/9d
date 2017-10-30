#pragma once

#pragma pack(push, enter_game)
#pragma pack(1)

struct _sHeader
{
	u_short	sLength : 12;		// -2048 - 2047
	u_short	sCrypto : 2;		// 0 : false, �������� true
	u_short	sCompressed : 2;	// 0 : flase, �������� true
	
	_sHeader() {}
	_sHeader(u_short len) : sLength(len), sCrypto(0), sCompressed(0) {}
};

#define _MAX_QUEST_INVENTORY_				5


// �Ʒ� ����ü �ϳ��� ��� ���� ������ ����Ʈ �� �� �ֵ���, ���������� ��� �����Ѵ�. 
// ĳ���� ����Ʈ�� �� ����ü

struct _CharacList // # [����] ĳ������ �⺻ ���� # 
{ 
	int				iUniqueID ;				// ĳ������ ����ũ ���̵� 
	char			cName[13] ;				// ĳ���� �� �̸� 
	char			cNick[13] ;				// ĳ���� ��Ī 

	char			cZone ; 

	char			cSex ;					// ���� 0 : ����, 1 : ���� 
	char			cFace ;					// ���� : max 16�� 
	char			cHair ;					// �Ӹ� ��Ÿ�� : max 16�� 
	char			cGroup ;				// �Ҽ� ����

	char			cAmorType;				// ����	0
	char			cPantsType;				// ����	1
	char			cCapType;				// ���� 2
	char			cAdaeType;				// �ƴ� 3
	char			cShoesType;				// �Ź� 4
	char			cBackPackType;			// �賶 
	char            cWeaponType ;           // ���� Ÿ��
	char			cMaskType;				// ġ�� Ÿ��

	short			sAmor ;					// ���� 0 
	short			sPants ;				// ���� 1 
	short			sCap ;					// ���� 4 
	short			sAdae ;					// �ƴ� 2 
	short			sShoes ;				// �Ź� 3 
	short			sBackPack ;				// �賶 7 
	
	short			sSelectWeaponID ;		// ������ ���� �ε���. 0, 1 ( ���� ����1, 2 ) 

	short			sMask;					// ġ�� 8
	u_char			ucWeaponStrength ;		// ���� ������ ���� ����.

	short			sStr ;					// �ٷ� <= ������ ���� 
	short			sZen ;					// ���� <= �״�� 
	short			sIntelligence ;			// ���� <= ���� 
	short			sConstitution ;			// �ǰ� <= // �ǰ� <= ���� 
	short			sDexterity ;			// ��ø <= �ɱ� 

	short			sInnerLevel ;			// ���� ����

	// 2004/11/10 Ray �߰�. class, classgrade
	char			cClass ;				// ���� 
	char			cClassGrade ;			// ���� ���
	
	char			cGM ;					// GM ����..
	
	// 2005/01/07 Ray �߰� : ����â���� ĳ������ ��ġ ǥ��.
	float			fPosX ;					// ���� ��ġ.
	float			fPosZ ;					//

#ifdef _CHARAC_BLOCKING
	int				iBlockingEndDate ;		// ��ŷ ���� �ð�
#endif	
//#ifdef _XTS_SERVERUNIFICATION	// ���� �Ϸ� ���Ŀ��� ��� ����
#ifdef _XTS_NEWCHARACLIST
	u_char				ucChangeNameFlag : 2 ;	// 0 - Not, 1 - Possible Change Name
	u_char				ucLeftFlag : 6 ;		// ���� �Ⱦ���.
#endif
} ;

// ����Ʈ ����
struct _sCharacQuestState
{
	bool	bReword  : 1 ;	// ������ �޾Ҵ°�
	u_char	ucNodeID : 7 ;	// �� Node ���̵� ����ε� ĳ���ͷ� �ؾ��Ѵ�.

	// �߰��� ��.
	u_short usQuestID;      // �� ����, �⿬, ���� ����Ʈ.

	_sCharacQuestState()
	{
		bReword = false;
		ucNodeID = 0;
	}
} ;

//���� ����Ʈ ����� ���õ� ����ü. 
struct _sCharacMainQuestState
{
	bool			bReword  : 1 ;	// ������ �޾Ҵ°�
	unsigned char	ucNodeID : 7 ;	// �� Node ���̵�
	unsigned short  sQuestID;       // �� ������ ����Ʈ ���̵�. ����� �ϴ� ���� ���� ������?
	u_char			padding;        // ������ ����.
	
	_sCharacMainQuestState()
	{
		sQuestID = 0;
		ucNodeID = 0;
		bReword  = false;
	}
} ;

// ����Ʈ ������ ����ü
struct _sQuestItem
{
	short			sID;
	unsigned int	uiCount;

	_sQuestItem() : sID(0), uiCount(0)
	{}
};


#define QUEST_HISTORY_COUNT 50 

struct _sQuestHistory
{
	char cNodeHistory;
	char cBranchHistory;
};

struct _sRunningQuest
{
	u_short usQuestID;
	char  cNodeID;
	int   dwQuestStartTime;
	int	  dwQuestTimer;
	char  cTimerNodeNo;
	bool  bBeUse;
	
	char   cTFRetry;       // �� ������ �Ϸ� �Ҷ� Ʈ�� �׼� ��Ʈ�������� �����׼� ��Ʈ��������.
	u_char ucHistoryCount; // ������ 255��.9+
	
	_sQuestHistory QuestHistory[QUEST_HISTORY_COUNT]; // 50�������� �ٿ�����. 
	_sRunningQuest()
	{
		usQuestID        = 0;
		dwQuestTimer     = 0;
		dwQuestStartTime = 0;
		ucHistoryCount   = 0;  // ����Ʈ�� Ʈ���� ����� �����ϱ����� �����丮�� ����� ����.
		cTFRetry         = 0;  // �� ������ ��� �� ���� fail �׼��� ������ ����, true �׼��� ������ ����. 							    
		cNodeID          = 0;  
		cTimerNodeNo     = 0;
		bBeUse           = false;
	}
} ; 



// ��������Ʈ������ ���� ������ �����丮�� ���ԵǾ�� �Ѵ�. 
// �����丮 �����ʹ� �б�ʹ� ������� ��尪�� �����ָ� �ȴ�.
struct _sRunningQuestPacketData
{
	u_short usQuestID;
	int   dwQuestStartTime;
	int	  dwQuestTimer;
	char  cTimerNodeNo;
	u_char ucSelectNo; 
	u_char ucCounter[4];
	
	_sRunningQuestPacketData()
	{
		usQuestID		 = 0;
		dwQuestStartTime = 0;
		dwQuestTimer	 = 0;
		cTimerNodeNo	 = 0;
		ucSelectNo       = 0;
		ucCounter[0]     = 0;
		ucCounter[1]     = 0;
		ucCounter[2]     = 0;
		ucCounter[3]     = 0;
	}
	
} ;

#ifdef _XDEF_EXPAND_ORGANIZATIONNAME
#define					_XDEF_ORGANIZATIONNAMELENGTH			24
#else
#define					_XDEF_ORGANIZATIONNAMELENGTH			8
#endif
// ��ũ ����Ʈ ���� ����ü 
struct QUEST_RANK_RECORD
{
	unsigned char	m_ucClan;
	char			m_acOrgName[_XDEF_ORGANIZATIONNAMELENGTH+1];		//����: ��,			����: ��,		��: ������ �̸�
	char			m_acName[13];		//����: �̸�,		����: ����̸�,	��: �����̸�
	unsigned short	m_usLevelOrCount;	//����: ����ܰ�,	����: �ο���,	��: ���ο�
	unsigned short	m_usQuestData;		//KillCount, Time, ItemCount
};

#define MAX_QUEST_RANK_RECORD	(10)

struct _QUEST_RANK_DATA
{
	QUEST_RANK_RECORD	m_aRankRecord[MAX_QUEST_RANK_RECORD];
};


// 0 ~ 15 ������ ����.
struct _sCharac_Skill
{
	short			id;
	char			cStep;				// ���� ���� �ܰ�

	u_char			ucSkillState : 2;	// 0 : ���� �Ϸ�   1 : �̿Ϸ�
	u_char			ucPassiveState : 1;	// Passive on/off
	u_char			ucCurMission : 5;	// �̼� ���� ī��Ʈ 0-31

	int				iCurSkillExp;
};

struct _sSkill_pair
{
	short	    sSkillID; // ��ų ���̵�
	char        number;   // ��ų �� -1�� ����� ����.
};

// ������ ����
union __item_or_money
{
	struct
	{
		u_short	 usCurDur ;			  // ���� ������
	    char	 cIdentify     : 4;	  // ���� ����
	    u_char   ucSocketCount : 4;   // ������ ����� �ո� �� �ִ°�?
	    u_char   ucCount ;			  // �ִ� ���� 255��
	} S_NormalItem;
	struct 
	{
		u_int	 uiCount ;	 		  // �Ӵ�..
  	} S_MoneyItem;
} ;	// ==> ������ ����.

struct __inven_uselimit_info_v1	// ������� ���� ��Ÿ ����.
{
	u_char				ucFlag1 ;		// Flag1 = �������� �ִ°� ���°�..
#ifdef _XDEF_SMELT_ITEM //Author : �����
	u_short				usResaved		: 1;	// �⺻���� ������ ����� �Ǿ��°�, ó�� �ʱ�ȭ�ÿ� �ѹ� ���δ�. : ������.
	u_short				usUsedProtect	: 1;	// ��縦 ����� ���� �ִ°�.
	u_short				usCurProtect	: 5;	// ������ ���, �ٸ� �븰�� ��. 0 - 15
	u_short				usCurMaxProtect	: 5;	// �ִ밭��

	u_short				usReserved		: 4;	// �����.
#else
	u_char				ucFlag2 ;		// 
	u_char				ucFlag3 ;
#endif
	u_short				usValue1 ;		// 

	u_char				ucYear ;		// Year - 2000 : 5 ~ ����.
	u_char				ucMonth ;		// Month
	u_char				ucDay ;			// Day
	u_char				ucHour ;		// Hour
	u_char				ucMinute ;		// Minute 
} ;

// ����/�ǻ��� ������ �����۵��� ����.
struct _sUser_InvenItem
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    u_char					cSlot ;			// ���� ��ȣ.
#else
	char					cSlot ;			// ���� ��ȣ.
#endif
	char					cType ;			// ������ ��з� 9
	char					cSecond ;		// Second�� �ʱ�ȭ �� ������, ������ �����Ѵ�.
	short					sID ;			// ������ �Һз�. (������ ���̵�) 12
    
	__item_or_money			Item_Money ;		// 16 		

	u_char					ucIncludeLimit : 1 ;// �ڿ� ������� ������ ���°�. 0-no, 1-yes
	u_char					ucBooleanRes : 7 ;	// 
} ;

// ---------------------------------------------------------------------=
// ����/�ǻ��� �ƴ� ���, ucIncludeLimit �� 1�̸�, �� ����ü�̴�.
struct _sUser_Inven_v1_limit : public _sUser_InvenItem
{
	__inven_uselimit_info_v1	uselimit ;
} ;
// ---------------------------------------------------------------------=


// ---------------------------------------------------------------------=
// ����/�ǻ��� ������ �̰�����. 
struct _sUser_Inven_v1_extension : public _sUser_InvenItem
{
	short					sSocket[4];		// 24

	u_char					ucStrength ;	// ������ ���, ����.

	//Author : �����
	//u_char	ucCurProtect : 4;	// ������ ���, �ٸ� �븰�� ��. 0 - 15
	//u_char	ucIncProtect : 3;	// �ִ밭�� �߰���.
	//u_char	ucUsedProtect : 1;	// ��縦 ����� ���� �ִ°�.
	u_char					ucSharpness ;	// ������ ���, ����.
} ;

// ����/�ǻ��� ���, ucIncludeLimit �� 1�̸�, �� ����ü�� ����ȯ.
struct _sUser_Inven_v1_all : public _sUser_Inven_v1_extension
{
	__inven_uselimit_info_v1	uselimit ;
} ;
// ---------------------------------------------------------------------=
struct _sUser_Inven
{
	u_char ucIdx;
	_sUser_Inven_v1_all Item;
};

// ���丮��
struct _SM_Storage_Inven
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    u_char				cIdx;			// ������ ���丮�� �ε��� or ���� �κ��丮 �ε���.
#else
	char				cIdx;			// ������ ���丮�� �ε��� or ���� �κ��丮 �ε���.
#endif
	u_char      		usNumber;		// ������ ����. 
	char                cIsstackable;   // �� �� �ִ� ������ ��Ÿ���� ������. 
};

// ���Ͱ� �ٴڿ� ���� ������
struct _sItem_Drop						// �ٴڿ� �ִ� ������ �׸� �ϳ�. 
{
	int				iItemWorldID;
	// 2004.05.31->oneway48 modify
	float			fX;
	float			fZ;
	//	short			fX;
	//	short			fZ;
	// modify end 
	u_short			usOwner ;
	char			cType;				// ������ ��з� 2^4 : 16
	
	union {
		struct {
			char				cSecond ;		// ������ �ߺз�
			char				cIdentify ;		// 0,1,2,3 ������.
			short				sID ;			// ������ �Һз�. (������ ���̵�)  ** �Ⱥ��δٸ�.. �ʿ� ����.
		} S_detail ;
		u_int			uiCount ;
	};
	
	u_char				ucCount ;		// ����
};

struct _sitem_pair
{
	u_char			slot;				// ����
	u_char			number;				// ����
};

struct _tradeitem
{
	char			index;
	unsigned short	quantity;
};

//���� ������
#define COLLECTIONDATA_COMPLETEFLAG_INDEX	(15)
extern WORD							g_CollectonWordFilter[16];
class ckCollectionData
{
private:
	unsigned short	m_usData;
public:
	inline unsigned short	GetData()	{	return m_usData;	}	
	inline void ckCollectionData::SetTrue(unsigned char index) { m_usData = m_usData | g_CollectonWordFilter[index]; };	
	inline void ckCollectionData::SetFalse(unsigned char index) { m_usData = m_usData & (g_CollectonWordFilter[index] ^ USHRT_MAX); };
	inline void ckCollectionData::SetCompleted() { m_usData = m_usData | g_CollectonWordFilter[COLLECTIONDATA_COMPLETEFLAG_INDEX]; };
	
	inline bool ckCollectionData::IsThisTrue(unsigned char index)
	{
		if((m_usData & g_CollectonWordFilter[index]) == 0)
			return false;
		return true;
	};	
	inline bool ckCollectionData::IsThisCompleted()
	{
		if((m_usData & g_CollectonWordFilter[COLLECTIONDATA_COMPLETEFLAG_INDEX]) == 0)
			return false;
		return true;
	};	
};

#define MAX_COLLECTION_INDEX_COUNT	(512)
struct _CHARAC_COLLECTION_DATA	//DB�� �����ϴ� ������ ����
{
	ckCollectionData		m_aData[MAX_COLLECTION_INDEX_COUNT];
};

union __charac_state
{
	short				sWoundRate ;			// �ܻ� ���� 0 - 100
	short				sInsideWoundRate ;		// ���� ���� 0 - 100
	u_short				usFatigueRate ;			// �Ƿε�	 0 - 100
	
	short				sRetribution ;			// ���� 
	int					sHonor ;
	
	short				sShowdowm ;				// �� ����
	short				sFuryParameter;			// ���� �� (���� �г� ��ġ)
	
	short				power ;					// Power ��.
	short				stat ;					// Stat ���� ��.
	
	int					iMatchRecord;			// �� ���� �� ����
} ;

enum EN_STATE_PACKET_FLAG
{
	charac_state_en_wound_rate=0,		// �ܻ�
	charac_state_en_inside_rate,		// ���� 1
	charac_state_en_fatigue, 			// �Ƿε� 2
	charac_state_en_retribution,		// ���� 3
	charac_state_en_honor,				// �� 4
	charac_state_en_showdown,			// ������ 5
	charac_state_en_fury,				// �г� 6
	charac_state_en_fame,				// 7 �Ǹ�
	
	charac_state_en_res2,				// 8
	charac_state_en_res3,				// 9
	charac_state_en_res4,				// 10
	charac_state_en_life_power,			// ü�� ������Ʈ 11 
	charac_state_en_force_power,		// ���� ������Ʈ 12
	charac_state_en_concentration_power,// ���� ������Ʈ 13
	charac_state_en_max_life,			// �ִ� ü�� 14
	charac_state_en_max_force,			// �ִ� ���� 15
	charac_state_en_max_concentration,	// �ִ� ���� 16
	charac_state_en_constitution,		// �ǰ� 17
	charac_state_en_zen,				// ���� 18
	charac_state_en_intelligence,		// ���� 19
	charac_state_en_dexterity,			// ��ø 20
	charac_state_en_strength,			// �ٷ� 21

	charac_state_en_match_grade=30,		// �� ����� ������Ʈ 30
	charac_state_en_match_win,			// ���� �¼� ������Ʈ 31
	charac_state_en_match_lose,			// ���� �м� ������Ʈ 32
	charac_state_en_match_tie,			// ���� �м� ������Ʈ 33
	charac_state_en_match_max_win_sr,	// ���� �ִ� ���¼� ������Ʈ 34
	charac_state_en_match_cur_win_sr,	// ���� ���� ���¼� ������Ʈ 35
	
	
	charac_state_en_match_gr_win=40,		// �׷� �¼� ������Ʈ
	charac_state_en_match_gr_lose,			// 
	charac_state_en_match_gr_tie			//
} ;

struct _sND_TimeUnit
{
	// ��/��/��/��/��/�� EndTime
	u_int		uiYear		: 5;		// base 2000
	u_int		uiMonth		: 4;
	u_int		uiDay		: 6;
	u_int		uiHour		: 5;
	u_int		uiMinute	: 6;
	u_int		uiSecond	: 6;
};

//Author : ����� // breif : ģ����ȯ
//packet_for_pk.h ���� �̵� 
#define en_charac_name_length	12

#include "packet_for_login.h"
#include "packet_for_lobby.h"
#include "packet_for_zone_monster.h"
#include "packet_for_battle.h"
#include "packet_for_quest.h"
#include "packet_for_skill.h"
#include "packet_for_level.h"
#include "packet_for_ctrl_gms.h"

#include "packet_for_items_trade.h"
#include "packet_for_messenger.h"
#include "packet_for_svrmove.h"
#include "packet_for_party.h"
#include "packet_for_boss.h"	//�������� ���� ��Ŷ 
#include "packet_for_nickname.h" //��ȣ�� ���� ��Ŷ 
#include "packet_for_organization.h"	// ���Ŀ� ���� ��Ŷ
#include "packet_for_personalstore.h"	// ���� ������ ���� ��Ŷ

#include "packet_for_event.h"			// Event ���� ��Ŷ
#include "packet_for_collection.h"		// ����
#include "packet_for_unigrouplobby.h"	// ���ռ��� ����
#include "packet_for_cashitem.h"		// Cash Item Mall ���� ��Ŷ
#include "packet_for_cb.h"				// ����� ���� ��Ŷ
#include "packet_for_xtrap.h"
#include "packet_for_pk.h"				// PK�� ���õ� ��Ŷ (pk�� ��ġ ������..)
#include "packet_for_match.h"			// ��
#include "packet_for_MnD.h"				// ���� �ý���


#pragma pack(pop, enter_game)

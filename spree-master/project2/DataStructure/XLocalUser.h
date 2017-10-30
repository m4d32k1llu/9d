// XLocalUser.h
//
// Author : Iljun, Park  /  Sohyun, Park
//
//////////////////////////////////////////////////////////////////////

#ifndef _XLOCALUSER_H_
#define _XLOCALUSER_H_

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
#include "XUser.h"
#include "XPathFinder.h"
#include "XCharacterState.h"
#include "XCastleBaseType.h"

#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
	#define _XDEF_WAREHOUSE_TOTALCOUNT		60
#else
	#define _XDEF_WAREHOUSE_TOTALCOUNT		40
#endif

#define	_XDEF_IMPACTPOWER		0.5f
#define _XDEF_FOVINCREASERATE	3.0f
#define _XDEF_FOVDECREASERATE	5.0f					
#define _XDEF_MAXFOV			(g_DefaultCameraFov + 30.0f)

// ---------------------------------------------------------------------------------------
// ���� ���� enumeration type

// �κ��丮 ���� Ÿ��
typedef enum tagInventoryslotType
{
	_XINVENTORY_SLOTTYPE_AMOR=0,			// 0  ������ ��� Pocket ������ �����Ѵ�. 
	_XINVENTORY_SLOTTYPE_PANTS,				// 1  ������ ��� Pocket ������ �����Ѵ�. 
	_XINVENTORY_SLOTTYPE_ADAE,				// 2
	_XINVENTORY_SLOTTYPE_SHOES,				// 3
	_XINVENTORY_SLOTTYPE_CAP,				// 4
	_XINVENTORY_SLOTTYPE_GAP_UPPER,			// 5
	_XINVENTORY_SLOTTYPE_GAP_LOWER,			// 6
	_XINVENTORY_SLOTTYPE_MASK,				// 7  ����ũ
	_XINVENTORY_SLOTTYPE_CLOTH_RESERVE1,	// 8  �ǻ� ���� ��.
	_XINVENTORY_SLOTTYPE_CLOTH_RESERVE2,	// 9  �ǻ� ���� 2.
	
	_XINVENTORY_SLOTTYPE_WEAPON_1,			// 10
	_XINVENTORY_SLOTTYPE_WEAPON_2,			// 11
	
	_XINVENTORY_SLOTTYPE_ACCESSORY_1,		// 12 ���� 0,1�� Pocket������ ���� ���������� Ȱ��ȭ �Ǿ���Ѵ�.  ���ǿ�...
	_XINVENTORY_SLOTTYPE_ACCESSORY_2,		// 13
	_XINVENTORY_SLOTTYPE_ACCESSORY_3,		// 14
	_XINVENTORY_SLOTTYPE_ACCESSORY_4,		// 15
	_XINVENTORY_SLOTTYPE_ACCESSORY_5,		// 16 ���ǿ� �Ǽ��縮.
	_XINVENTORY_SLOTTYPE_ACCESSORY_6,		// 17
	_XINVENTORY_SLOTTYPE_ACCESSORY_7,		// 18
	_XINVENTORY_SLOTTYPE_ACCESSORY_8,		// 19	
	
	_XINVENTORY_SLOTTYPE_EXTRA_POCKET,		// 20 ����ǰ ���� ����.
	
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	_XINVENTORY_SLOTTYPE_CASHBACKPACK1,			// 21 //Author : ����� //breif : ĳ�� �೶ �߰�
	_XINVENTORY_SLOTTYPE_CASHBACKPACK2,			// 22
#else
	_XINVENTORY_SLOTTYPE_RESERVE1,				// 21
	_XINVENTORY_SLOTTYPE_RESERVE2,				// 22
#endif

	_XINVENTORY_SLOTTYPE_BACKPACK1,			// 23
	_XINVENTORY_SLOTTYPE_BACKPACK2,			// 24 ���� �ι�°.

	_XINVENTORY_SLOTTYPE_POCKETSTART = 25,				// 25	�ָӴ� ������ġ 
	_XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART = 50,		// 45	�賶   ������ġ
	_XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART = 75,		// 70	�賶2  ������ġ
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	_XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART = 100,		
	_XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART = 125,
	
	_XINVENTORY_SLOTTYPE_TOTALCOUNT = 150,
	_XINVENTORY_SLOTTYPE_MONEYINDEX = 224
#else
	_XINVENTORY_SLOTTYPE_TOTALCOUNT = 100,
	_XINVENTORY_SLOTTYPE_MONEYINDEX = 127 // �� �ε���
#endif		

} _XINVENTORY_SLOTTYPE;

// ���� ����
typedef enum tagUserStateSeq
{
	_XUSER_STATE_NONE = 0,
	_XUSER_STATE_1,
	_XUSER_STATE_2,
	_XUSER_STATE_3,
	_XUSER_STATE_4,
	_XUSER_STATE_5,
	_XUSER_STATE_6,
	_XUSER_STATE_7,
	_XUSER_STATE_8,
	_XUSER_STATE_9,
	_XUSER_STATE_10,
	_XUSER_STATE_LAST
}_XUSER_STATE_SEQ;

// �ִϸ��̼� ����
typedef enum tagAnimationOrder
{
	_XANI_ORDER_READY = 0,
	_XANI_ORDER_REPEAT,
	_XANI_ORDER_FIRE 
}_XANIMATION_ORDER;

typedef enum tagTroubleState
{
	_XDEF_CONVSTATE_NORMAL = 0,
	_XDEF_CONVSTATE_FURY,
	_XDEF_CONVSTATE_CONVERGENCE
}_XUSER_TROUBLE_STATE;

typedef enum tagUserState
{
	_XDEF_USERSTATE_NORMAL = 0,
	_XDEF_USERSTATE_TRAINING,				// ����
	
	_XDEF_USERSTATE_MATCH_REQUEST,			// �� ��û��/��û������

	_XDEF_USERSTATE_MATCH_MAKETABLE,		// ��ø �ۼ���
	_XDEF_USERSTATE_MATCH_WAITING,			// ��ٸ��� ����

	_XDEF_USERSTATE_MATCH_MATCHING,			// �� ����
	_XDEF_USERSTATE_MATCH_BOW,				// ����
	_XDEF_USERSTATE_MATCH_POSITIONING,		// �ڸ� �ο�
	_XDEF_USERSTATE_MATCH_COUNTING,			// ���� ī��Ʈ��
	_XDEF_USERSTATE_MATCH_FIGHT,			// ����

	_XDEF_USERSTATE_MATCH_END,				// �� ���� - �����
	_XDEF_USERSTATE_MATCH_RESULT,			// �� ���� - ����ް� 10�ʰ�
	_XDEF_USERSTATE_MATCH_WAITEXIT,			// �� ���� - ���� ������ ����

	_XDEF_USERSTATE_PK_READY,				// PK �غ� ����
	_XDEF_USERSTATE_PK_FIGHT,				// PK ����
	_XDEF_USERSTATE_PK_END					// PK End
}_XUSER_STATE;

// ���� ���� - ������ �¾ƾ� ��
typedef enum tagTrainingState
{
	_XTRAINING_NONE = 0,
	_XTRAINING_MEDITATION,		// ��� ����
	_XTRAINING_DUMMY,			// ���� ġ��
	_XTRAINING_SPEED,			// ���
	_XTRAINING_SPELL,			// ���
	_XTRAINING_SAND,
	_XTRAINING_THROW,
	_XTRAINING_GROUP,
	_XTRAINING_CLOSED,

	// by mahwang
	_XTRAINING_FIREWOODCUT,
	_XTRAINING_WATERDRAW,

}_XTRAINING_CLASS;

// �ִϸ��̼� Reference table
enum tagMatchAnimation
{
	_XAN_MATCH_START_MALE_NONE = 0,
	_XAN_MATCH_START_MALE_SHORTWEAPON,
	_XAN_MATCH_START_MALE_LONGWEAPON,
	_XAN_MATCH_MALE_CANCEL,

	_XAN_MATCH_START_FEMALE_NONE,
	_XAN_MATCH_START_FEMALE_SHORTWEAPON,
	_XAN_MATCH_START_FEMALE_LONGWEAPON,
	_XAN_MATCH_FEMALE_CANCEL,

	// ���� �ι� ��� �ڼ�
	_XAN_IDLE_AXE_MALE,
	_XAN_IDLE_AXE_FEMALE
};

// ��ġ�� mode
enum tagShoutMode
{
	_XSHOUT_MODE_NONE = 0,		// ��ġ�� �Ұ�
	_XSHOUT_MODE_WHITE = 96,		// �鵵 ��ġ��
	_XSHOUT_MODE_BLACK = 97,		// �浵 ��ġ��
	_XSHOUT_MODE_ALL = 98,		// ��ü ��ġ��
	_XSHOUT_MODE_CLAN = 99		// ���� ��ġ��
};


// ---------------------------------------------------------------------------------------
// ���� ������ �ൿ �α�
typedef struct _XUSERACTIONLOG
{
	// ���� ����
	BOOL		isAttack;
	int			attackTime;
	int			attackTargetType;
	
	// ���� ��Ʈ ����
	BOOL		isHit;
	int			hitTime;
	int			hitTargetType;
	
	// �� ���� ����( ������ )
	BOOL		isKill;
	int			killTime;
	int			killTargetType;
	
	// ������ �ı� ����( �������� ���� �ı� )
	BOOL		isDestroyItem;
	int			DestroyItemTime;
	char		DestroyItemType;
	short		DestroyItemID;
	
	// ���� ���� ����
	BOOL		isChange;
	int			changeTime;
	int			changeTargetType;
	int			changeTargetUniqueID;
	
	// ������
	BOOL		isLevelUp;
	int			levelupTime;
	
	// �ŷ�
	BOOL		isTrade;
	int			tradeTime;
	
	// ��ȭ
	BOOL		isTalk;
	int			talkTime;
	
	// ����
	BOOL		isDead;
	int			deadTime;
	
	// ��Ȱ
	BOOL		isRevive;
	int			reviveTime;
	
	_XUSERACTIONLOG()
	{
		isAttack = isLevelUp = isTrade = isTalk = isDead = isRevive = isKill = isHit = isChange = isDestroyItem =FALSE;
		attackTime = levelupTime = tradeTime = talkTime = deadTime = reviveTime = killTime = hitTime = killTime = DestroyItemTime = 0;
		killTargetType = attackTargetType = hitTargetType = changeTargetType = DestroyItemID = 0;
		changeTargetUniqueID = DestroyItemType = -1;
	}		
}_XUSERACTIONLOG;	


// ----------------------------------------------------------------------------------------------
// ��Ƽ ����
typedef struct _XUSERPARTYINFO
{
	unsigned short	playeruniqueid;
	
	short			posX;
	short			posZ;
	
	short			innerlevel;

	char			serverno;
		
	char			vitalpercent;
	char			zenpercent;

	char			clan;			// ����
	char			clanclass;		// ����
    short           nClassGrade;    // ��å ���

	bool			bOut;			// �ڸ� ��� flag
	bool			bFarAway;		// �Ÿ� ��Ż flag

	int				honor ;			// ��

	int				partycount;		// ��Ƽ ����Ʈ���� �ڱⰡ ������ �ִ� ��Ƽī������ ���� 
	
	BOOL			isLeader;		// ��Ƽ�������� �� �� �ִ� ���� 
	BOOL			isValid;		// ��Ƽ�� ������ �ִ����� �� �� �ִ� ���� 
	BOOL			isMVP;			// ��Ƽ ����Ʈ���� �ڱ� �ڽ��� MVP������ �� �� �ִ� ����

	TCHAR			playername[13];	
	
	_XUSERPARTYINFO()
	{
		memset(playername, 0, sizeof(TCHAR)*13);
		serverno = posX = posZ = innerlevel = vitalpercent = zenpercent = playeruniqueid = clan = clanclass = honor = 0;
		isLeader = isValid = isMVP = FALSE;
		partycount = 0;
		bOut = bFarAway = false;
	}
	
}_XUSERPARTYINFO;

// ������ Ÿ��
typedef struct _XTileItem
{
	short		sX;
	short		sZ;
} _XTILE_ITEM;

// ���� ġ�� ���� ����
struct _XDUMMY_INFO
{
	short		skillid;
	bool		bWarning;
	bool		bWarningSkillState;
	bool		bComplete;
	char		cTrainingGrade;
	char		cReserved;
};

// ��� ���� ����
struct _XSPELL_INFO : public _XDUMMY_INFO
{
	bool		bSetTargetPosition;
	void		Reset( void )
	{
				skillid = 0;
				bWarning = false;
				bWarningSkillState = false;
				bSetTargetPosition = false;
				cTrainingGrade = 0;
	}
};

class _XMovePathData : public _XItem
{
public:
	D3DXVECTOR3	nexttargetposition;
	/*
public:
	_XMovePathData()
	{		
		Reset();
	}
	void Reset( void )
	{
		nexttargetposition.x = 0.0f;
		nexttargetposition.y = 0.0f;
		nexttargetposition.z = 0.0f;
	}
	*/
};

// ������ �Ķ���� ����
class _XSU_Parameter
{
	_XDEF_ENCVALUE(short,org);
	_XDEF_ENCVALUE(short,result);

	_XDEF_ENCVALUE(char,bBuff);

	_XDEF_ENCVALUE(char,applyCount);
	_XDEF_ENCVALUE(char,applyCountAdditional);

	FLOAT		bonus;

#ifdef _XTS_BASEMINMAXWEAPONDAMAGE
	FLOAT		org_min;
	FLOAT		org_max;
	FLOAT		result_min;
	FLOAT		result_max;
	FLOAT		bonus_min;
	FLOAT		bonus_max;
#endif
};

// User ����ȿ�� (condition) ���� lock
struct _XUSERLOCK
{
	DWORD	movelock : 1;
	DWORD	tradelock : 1;
	DWORD	battlelock : 1;
	DWORD	skilllock : 1;
	DWORD	medlock : 1;
	DWORD	spelllock : 1;
	DWORD	speedlock : 1;
	DWORD	chatlock : 1;
	DWORD	modelock : 1;
	DWORD	partylock : 1;
	DWORD	camerarotatelock : 1;
	DWORD	camerazoomlock : 1;
	DWORD	camerareverse : 1;
	DWORD	adddelaytime : 1;
	DWORD	notdrawusername : 1;

	// 2006.01.17 �߰�
	DWORD	disableskill : 1;
	DWORD	disablestype : 1;

	DWORD	reserved : 15;

	short	disableskillindex;
	short	disablestypeindex;
};

// ���� ���� ����Ÿ
struct _XUSERELIXIRINFO
{
	struct _Data
	{
		short __Data[10]; 
		char  __Grade;	
	};

	_Data Data[5];	// ���Ǻ� ������. 
					// 0 �ٷ� 
					// 1 ���� 
					// 2 ����			
					// 3 �ǰ� 
					// 4 ��ø 
					// ���� ���� �Ծ��� ����� �����ʹ� ��� �� ���ΰ�???
	
	short _sIntoxication; // �ߵ� ��ġ.
	char  Temp[3];  // �������.

	void		Reset( void )
	{
		for( int i = 0; i < 5; ++i )
		{
			Data[i].__Grade = 0;
			for( int ii = 0; ii < 10; ++ii )
				Data[i].__Data[ii] = 0;
		}
		_sIntoxication = 0;
		for( i = 0; i < 3; ++i )
			Temp[i] = 0;
	}
	
	int GetTotalBonus( int index )
	{
		int result = 0;
		for( int i = 0; i < Data[index].__Grade; ++i )
		{
			result += Data[index].__Data[i];
		}
		return result;
	}

	_XUSERELIXIRINFO()
	{
		Reset();
	}
};

struct _XMatchMemberInfo
{
	unsigned char		ucTeam : 2;
	unsigned char		ucNumber : 6;

	unsigned char		ucSex : 2;
	unsigned char		ucMoonpa : 6;

	char				cClass;
	unsigned char		ucLevel;
	unsigned short		usWinRate;
	unsigned char		ucMatchGrade;
	unsigned char		ucVitalRate;
	char				cName[13];

	void Reset()
	{
		ucTeam = 0;
		ucNumber = 0;
		ucSex = 0;
		ucMoonpa = 0;
		cClass = 0;
		ucLevel = 0;
		usWinRate = 0;
		ucMatchGrade = 0;
		ucVitalRate  = 0;
		memset(cName, 0, sizeof(cName));
	}
};

struct _XMatchTableInfo
{
	unsigned short		usWaitTime;
	char				cRecordType;
	char				cMatchType;
	char				cMatchTime;
	char				cUseHyperrun;
	char				cUseBuff;
	char				cMatchBalance;

	_XUserItem			myBetItem[6];
	_XUserItem			targetBetItem[6];

	int					myBetMoney;
	int					targetBetMoney;

	DWORD				dwWaitStartTime;

	void Reset()
	{
		usWaitTime = 0;
		cRecordType = 0;
		cMatchType = 0;
		cMatchTime = 0;
		cUseHyperrun = 0;
		cUseBuff = 0;
		cMatchBalance = 0;
		dwWaitStartTime = 0;

		for(int i = 0 ; i < 6 ; ++i)
		{
			myBetItem[i].Reset();
			targetBetItem[i].Reset();
		}

		myBetMoney = 0;
		targetBetMoney = 0;
	}
};

//Author : �����
typedef enum tagPCroomState
{
	_XDEF_PCROOM_ERROR = 0,
	_XDEF_PCROOM_INTERNET,
	_XDEF_PCROOM_RATIOPC,
	_XDEF_PCROOM_RATIOCAFE,
	_XDEF_PCROOM_PREPAID,
	_XDEF_PCROOM_PREMIUM,
	_XDEF_PCROOM_NONE
} _XUSER_PCROOM_STATE;
//Last Updated : 07/03/15

// PK�� ��ġ ���� or Ư���� ��ġ ����
typedef struct 
{
	u_char		ucPKType;					// PK�� ��ġ ���� or Ư���� ��ġ ����
	int			nStartTrackingTime;		// ��ġ������û�ð�
	u_char		ucTimeMode;					// �Ƿ��� ��ġ���� ���� ( 1) 24�ð�, 2) 48�ð� ) 
	char		szCharacName[_XDEF_MAX_USERNAMESTRINGLENGTH];
}_XPKTRACING_INFO;

enum MasterNDisciple //Author : ����� //breif : ���� ������ ���� ��������,, �������� �ƹ��͵� �ƴ��� ǥ��
{
	_XDEF_NONE = 0,
	_XDEF_MASTER, 
	_XDEF_DESCIPLE
};


class _XLocalUser : public _XUser
{

// Construction/Destruction
public:
	_XLocalUser();
	~_XLocalUser();

// Attributes
private:

//Author : �����
	struct State
	{
		DWORD dwError:1;
		DWORD dwInternet:1;
		DWORD dwRatioPc:1;
		DWORD dwRatio:1;
		DWORD dwPrepaid:1;
		DWORD dwPremium:1;
		DWORD dwNone:1;
	};

	typedef struct 
	{
		State PCROOM_State;
		DWORD	dwRemain;
	}PCROOM_App;

	PCROOM_App m_dwPCroomInfo;
//Last Updated : 07/03/15

	short					m_FinallySelectedSkillID; //Author : ����� //breif : _XDEF_SKILLDEALY_MODIFY
	_XUserSkill*			m_FinallySelectedSkillItem;


	bool	m_bGuardMob; //Author : ����� //breif : ��Ʈ�� shift Ŭ��

	int	m_nNotoriousValue; //Author : ����� //breif : ���� �ý��� 

	BOOL m_bChangingWeapon; //Author : ����� //breif : ���⸦ ��ü ������ �Ǵ�
	BOOL m_bChangingWeaponPacket; //Author : ����� //breif : ��Ŷ �޾Ҵ� �� �Ǵ�
	int	m_nChangeWeaponModel;

public:
	
	
	int						m_iNetworkKey;					// ��ŷ ������ ���� Ű�� hotblood 20040824
	short					m_sMiniGameId;					// �̴ϰ��� ���� hotblood 20040824
	
	// ���� �⺻
#if defined(_XDEF_ITEMMALL_AUTOLOGIN_20061120) || defined(_XDEF_ITEMMALL_AUTOLOGIN_SE_20070514)
	TCHAR					m_ItemMallWebLoginID[33];	
#endif
	TCHAR					m_UserName[33];
	TCHAR					m_UserPW[33];

	_XCHARACTER_INFO		m_AllCharacterInfo[ _XDEF_MAX_CHARACTERCOUNT ];	
	int						m_CharacterCount;
	int						m_SelectedCharacter;
	D3DXVECTOR3				m_NextRespawnPosition;

	// ���� �ൿ �α�
	_XUSERACTIONLOG			m_CharacterLog;	

	// ��Ƽ ����
	int						m_TotalCountOfPartyQuestCounter;			// ��Ƽ����Ʈ���� ��Ƽ ī������ ������ ��Ÿ�� 
	int						m_PartySelfIndex;							// ��Ƽ ����Ʈ �迭���� �ڱ� �ڽ��� ��ġ�� ��Ÿ���� �ε���
	int						m_PartyMemberCount;							// ��Ƽ�� ����
	int						m_PartyMemberIndex;							// ��Ƽ���� ��Ŷ�� ������ ���̴� ��Ƽ�� ���� �ε���
	_XUSERPARTYINFO			m_PartyInfo[_XDEF_MAXPARTYMEMBERCOUNT];

	// ���� ����
	int						m_SkillCombo;
	int						m_SkillStarLevel;
	int						m_MartialStep;
		
	int						m_ExperiencePercentage;

	int						LifePowerPercentage;					 // ���� ���� %
	int						ForcePowerPercentage;					 // ���� ���� %
	int						ConcentrationPowerPercentage;			 // ���� ���� %

	// �κ��丮 ������
	_XUserItem				m_UserItemList[_XINVENTORY_SLOTTYPE_TOTALCOUNT];
	DWORD					m_Money;									// ����
	char		            m_cAmorPocketNum;					        // �������� ����
	char			        m_cPantsPocketNum;							// �������� ����
	char					m_cPocketNum;								// �ӽ� ���� ����
	
	char					m_cBackPackExtend;							// �賶      
	_XUSERELIXIRINFO		m_ElixirInfo;								// ������� ����Ÿ����
	DWORD					m_IntoxicationTimer;							// �ߵ� Ÿ�̸�
	
	_XUserItem*				m_SelectedWeaponItem;
	_XGameItem*				m_PickItem;

	int						m_nCashBackPackMaxValue;				//Author : ����� //breif : ĳ�� �೶ �ִ밪
	
	// â�� ������ ����
	_XUserItem				m_WarehouseItemList[_XDEF_WAREHOUSE_TOTALCOUNT];
	DWORD					m_WarehouseMoney;
	int						m_WarehouseSlotCount;
	
	// mastery ��ų ����
	_XMasterySkillItem		m_MasterySkillList[_XDEF_MAX_LEARNMASTERYSKILL];
			
	// ���� ����	
//	_XSkillItem				m_SkillList[_XDEF_MAX_LEARNSKILL];
	_XUserSkill				m_SkillList[_XDEF_MAX_LEARNSKILL];
	short					m_SelectedSkillID;
//	_XSkillItem*			m_SelectedSkillItem;
	_XUserSkill*			m_SelectedSkillItem;
	short					m_ActiveSlotID;

	short					m_NextSkillID;

	// �̺�Ʈ�� ��� �ֱ� �߰�
	_XUserSkill				m_EventSkillList;		// 1����
	_XTimeUnit				m_EventSkillEndTime;

	// BOOL ���� ==================================
	BOOL					m_bUsingSkill : 1;
	BOOL					m_bCheckDelayTime : 1;
	BOOL					m_bRestartActive : 1;
	BOOL					m_bStartCharge : 1;
	BOOL					m_bCheckFirstFrame : 1;
	BOOL					m_bWaitPassiveReq : 1;
	BOOL					m_bAddDelayTime : 1;

	BOOL					m_ChargeSendStartPacket : 1;
	BOOL					m_RoundingSendPacket : 1;
	BOOL					m_CastingSendPacket : 1;
	BOOL					m_HiddenSendStartPacket : 1;
	BOOL					m_HiperRunStopSignalSended : 1;			// ����� ������ �����(����==0) ������ ������ ��Ŷ ���� ����
	BOOL					m_HiperRunFailedFlag : 1;
	BOOL					m_FinishSendPacket : 1;
	BOOL					m_SmashingSendPacket : 1;
	BOOL					m_UseSpeedSkill : 1;
	BOOL					m_bLeftClickProcess : 1;
	BOOL					m_bRightClickProcess : 1;
	BOOL					m_bSaveRespawnPos : 1;
	BOOL					m_bSendTrainingStartPacket : 1;

	BOOL					m_bIsBossBattle : 1;     
	BOOL					m_bExitBossBattleRoom : 1;
	BOOL					m_bBossBattleEndProcess : 1;
	BOOL					m_bSuccessBossBattle : 1;
	BOOL					m_bWaitBossBattle : 1;
	BOOL					m_bWaitFlagInfo : 1;
	BOOL					m_bKeyLPSProcess : 1;
	BOOL					m_bPartyProgress : 1;				// ��Ƽ�� ���õ��ൿ�� �ϰ� �ִٴ� ���� �˷��ִ� ����(��Ƽ�α� ��...���)
	BOOL					m_bPartyQuestProgress : 1;			// ��Ƽ����Ʈ�� ���� ���̶�� ���� �˷��ִ� ����(��Ƽ�� ���õ� ������ ���´�)
	BOOL					m_InParty : 1;						// ��Ƽ�� �ΰ� �ִٴ� ���� �˷��ִ� ����
	BOOL					m_IsServerMoveInParty : 1;
	BOOL					m_bGrabItem : 1;				// ������ �ݱ� ���� - ���� Ȯ��
	BOOL					m_bVirtualMode : 1;				// Ʃ�丮�� ���� ���� ���� ���� 
	// =================================================

	BOOL					m_bWaitDyingSkip : 1;
	BOOL					m_PvpNPCLock : 1;					// �ŷ��� npc Lock ����
	BOOL					m_bSendFinishFirePacket : 1;		// Finish Fire packet�� ������
	BOOL					m_bSendFinishPosUpdatePacket : 1;	// Finish �� ��ġ update packet ������
	BOOL					m_bChannelingSendStartPacket : 1;	// Channeling start packet�� ������
	BOOL					m_bReadingBook : 1;					// ��� �д� ������ 
	BOOL					m_bInternalUse : 1;					// ������ ���� ������
	BOOL					m_bSocketPlugIn : 1;				// ������ �ٴ� ������
	BOOL					m_bSocketPlugInLock : 1;			// �������� ��Ÿ�� ���¿��� �ְ� ���� ���
	BOOL					m_bGambleBoxOpen : 1;				// �׺�ڽ��� ���� ������
	BOOL					m_bGambleBoxOpenLock : 1;			// �������� ��Ÿ�� ���¿��� �ְ� ���� ���
	BOOL					m_bViewMasterySkill : 1;			// ���� ����Ʈ�� �Ϸ��Ͽ� ���� ���̺��� ����
	BOOL					m_bHaveStance : 1;					// stance �ڼ��� ������ ���� (activeskill or ��)
	BOOL					m_bTargetInfoLock : 1;				// �ι����⸦ ��û������
	BOOL					m_bSocketPlugInEXLock : 1;			// �������� ��Ÿ�� ���¿��� �ְ� ���� ���
	BOOL					m_bUseMouseChargeSkill : 1;			// charge skill �����߿� ���콺�� ������ Ŭ��
	BOOL					m_ActionSendPacket : 1;				// action skill
	BOOL					m_bAutoMeditation : 1;				// �ڵ� ��� ����
	BOOL					reserved : 14;
	
	
	// ���� ����
	int						m_CurrentSelectedSkillType;
	_XUSER_STATE_SEQ		m_CurrentBattleState;
	_XStickGauge			m_MiniHpBorder; // 2004.06.18->oneway48 insert
	_XStickGauge			m_MonsterMiniHpBorder; // 2004.06.18->oneway48 insert
	_XImageStatic			m_imageMiniHpBackBorder;

	// ����
	DWORD					m_ChangeDelayStartTime;
	
	// Passive ����
	unsigned short			m_LastPassiveUsefulType : 8;
	unsigned short			m_LastPassiveSType : 8;
	DWORD					m_LastPassiveOnOffTime;
	
	// Charge skill ����
	DWORD					m_ChargeStartTime;
	int						m_ChargeIterateValue;

	// Rounding spell ����
	DWORD					m_RoundingDelayStartTime;

	// Casting spell ����
	DWORD					m_CastingSpellRepeatTime;

	// Hidden spell ����
	DWORD					m_HiddenSendLastPacketTime;
	DWORD					m_HiddenReceivePacketTime;

	// Channeling spell ����
	DWORD					m_ChannelingSendLastPacketTime;
	DWORD					m_ChannelingSpellRepeatTime;

	// ���� ���� ����
	FLOAT					m_RegionTargetPosX;
	FLOAT					m_RegionTargetPosZ;
		
	// Finish skill ���� - user.h�� define
//	FLOAT					m_FinishSpeed;	

	// Smashing skill ����

	// Spell ���� target - server�� ������ ����
	_XItem*					m_SpellAttackTarget;
	_XPICKINGTARGETTYPE		m_SpellAttackTargetType;
	
	// ���� ����	
	DWORD					m_SpecialActionTimer;
	DWORD					m_PrevPacketSendedTime;
	D3DXVECTOR3				m_LastInsertedMoveTargetEffectPosition;
	int						m_AttackPointNumber;
	int						m_CurrentAttackPoint;

	// Stance motion ó�� ���� - Battle/Peace Model change
	DWORD					m_LastModeChangeTime;

	// Targetting ����
	_XItem*					m_ViewTarget;
	_XPICKINGTARGETTYPE		m_ViewTargetType;

	// World item ����
	_XTILE_ITEM				m_CurrentTileIndex;
	DWORD					m_LastItemRequestSendTime;

	// ���� ó�� ����
	DWORD					m_DyingStartTime;
	int						m_LastCheckedDyingStartTime;
	DWORD					m_DyingTimer;
	TCHAR					m_RespawnName[13];
	
	// ���� ����
	_XUSER_STATE_SEQ		m_CurrentInternalUseState;
	_XUSER_STATE_SEQ		m_CurrentReadBookState;

	// ������ ���� ���� - �г�, ���� �� ���� �̻�
	_XUSER_TROUBLE_STATE	m_UserTroubleState;
	
	// ���� ����
	_XUSER_STATE			m_UserState;
	_XTRAINING_CLASS		m_SelectedTrainingMode;
	_XUSER_STATE_SEQ		m_CurrentTrainingState;

	// by mahwang
	int						m_nTrainingCount;

	// ���ÿ� ���Ǵ� ���� Ű
	int						m_ServerKey;
	_XDUMMY_INFO			m_TrainingDummyInfo;
	_XSPELL_INFO			m_TrainingSpellInfo;

	// Path finder	
	FLOAT					m_LeftFinalTargetLength;
	D3DXVECTOR3				m_FinalTargetPosition;
	int						m_PathNodeCount;
	int						m_CurrentNavigatePathNodeIndex;
	FINFO					m_PathPointArray[300];
	void					CreateMovePath( void );
	
	// ������ ���� 
	int						m_BossRoomID;
	DWORD					m_BossStartTime;
	DWORD					m_BossTimeLimit;
	short					m_BossRoomStageNumber;

	// �� ���� - OLD
	char					m_MatchType;
	unsigned short			m_MatchID;

	// �� ���� - NEW
	char					m_myMatchTeamID;
	_XMatchMemberInfo		m_myMatchMemberInfo;
	_XMatchTableInfo		m_myMatchTableInfo;	
	unsigned short			m_MatchTargetID;
	DWORD					m_MatchStateChangeTime;
	int						m_MatchResult;					// 0:�¸�, 1:�й�, 2:���º�
	int						m_MatchFieldIndex;				// ���� ����
	DWORD					m_MatchPRTimer;					// �� ȫ�� ��Ŷ�� 5�ʸ��� ������.
	unsigned char			m_MatchGrade;					// �� ���

	// ������ �Ķ���� ���� - CharacterInfo���� ���� ��
	_XSU_Parameter			m_P_AttackDamage;				// ���ݷ�
	_XSU_Parameter			m_P_AttackRate;					// ���� ����ġ
	_XSU_Parameter			m_P_Defence;					// ����
	_XSU_Parameter			m_P_AvoidRate;					// ȸ�Ƿ�
	_XSU_Parameter			m_M_AttackDamage;				// ��� ���ݷ�
	_XSU_Parameter			m_M_AttackRate;					// ��� ����ġ
	_XSU_Parameter			m_M_Defence;					// ��� ����
	_XSU_Parameter			m_M_AvoidRate;					// ��� ȸ�Ƿ�

	// characinfo�� ���� �������� ���� original �� - �̰��� Effect���� ��� �� ��
	_XSU_Parameter			m_Strength;						// �ٷ�
	_XSU_Parameter			m_Zen;							// ����
	_XSU_Parameter			m_Intelligence;					// ����
	_XSU_Parameter			m_Dex;							// ��ø
	_XSU_Parameter			m_Constitution;					// �ǰ�

	short					m_P_MinAttackDamage;
	short					m_P_MaxAttackDamage;
	short					m_M_MinAttackDamage;
	short					m_M_MaxAttackDamage;
	
	_XSU_Parameter			m_MaxLifePower;
	_XSU_Parameter			m_MaxForcePower;
	_XSU_Parameter			m_MaxConcentrationPower;

	// ����ȿ�� (condition)���� lock
	_XUSERLOCK				m_Lock;


	//Author : ����� //breif : �����ΰ� �����ΰ�
	int						m_nLocalMnDClass;
	
	// �̵� �ӵ�
private:	
	short					m_MoveSpeedFromCondition;
	
public:	
	short					Get_m_MoveSpeedFromCondition(){	return _XMEMORYCHECKDATA_ENCSHORT(m_MoveSpeedFromCondition); }
	void					Set_m_MoveSpeedFromCondition(short speed){ m_MoveSpeedFromCondition= _XMEMORYCHECKDATA_ENCSHORT(speed); };
	

	// Item���� ���� Penalty
	short					m_PenaltyValueFromItem[2];
	
	// ��� ���� ������ ����/���� Effect
	short					m_WeaponMinDamageBonus[2];	// 0:value, 1:percentage
	short					m_WeaponMaxDamageBonus[2];	// 0:value, 1:percentage
	
	int						m_ResistMinus;				// ���� ���׷�
	int						m_ResistPlus;				// ���� ���׷�
	int						m_ResistHon;				// ȥ�� ���׷�
	int						m_ResistSin;				// ���� ���׷�
	
	// 2007.06.26 �߰�---��
	int						m_AttackDamageMinus;		// ���� ���ݷ�
	int						m_AttackDamagePlus;			// ���� ���ݷ�
	int						m_AttackDamageHon;			// ȥ�� ���ݷ�
	int						m_AttackDamageSin;			// ���� ���ݷ�
	// ------------------��
	
	DWORD					m_ElixirSoundRepeatTime;	// ���� �Դ� ���� ���� �ݺ��� ����
	
	// ���� ���� ���� Request Timer
	DWORD					m_PrevRequestPacketSendedTime;	// ���� ����Ʈ�� �� ��û�ϴ� �ð� 


	int						m_ChatMuteTime;
	int						m_LastChatLockNoticeMinute;
	
	// PK time
	DWORD					m_PKModeLeftTime;
	DWORD					m_PKModeStartTime;
	
	// PE time
	DWORD					m_PEModeLeftTime;
	DWORD					m_PEModeStartTime;
	
	// ���� ���
	_XBATTLEMODETYPE		m_PrevBattleMode;				// ���� ���
	
#ifdef _XTS_BLOODPOINT
	// ����
	int						m_BloodPoint;
#endif

	int						m_FameAttrib;	// -1 : ���� 0 : �� 1 : �Ǹ�

	DWORD					m_SafetyZoneCheckTimer;			// ���� ���� üũ Ÿ�̸�
	int						m_InsideSafetyZoneIndex;		// 0:�Ϲ�����  1:��������  2:�������  3:�߹�����

	int						m_UseItemInvenNumber;	// (cash)����Ϸ��� ������ inven # - �޼��� �ڽ��� ��ġ�� ������ �����س��´�.

#ifdef _XDEF_CASHITEM_SHOUT
	int						m_ShoutMode;
	DWORD					m_ShoutStartTime;
	DWORD					m_ShoutLeftTime;

	void					ResetShout(void);
	void					ProcessShout(void);
#endif

	//
	// ����ġ/���õ�/������� type - item ctype/sid
	//
	int						m_AccountItemcType;
	int						m_AccountItemsID;
	
	// ������ �ð� 
	unsigned int			m_etYear		: 5;
	unsigned int			m_etMonth		: 4;
	unsigned int			m_etDay			: 6;
	unsigned int			m_etHour		: 5;
	unsigned int			m_etMinute		: 6;
	unsigned int			m_etSecond		: 6;

	void					SetExpItemEndTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);
	void					ResetExpItemEndTime(void);

	//
	// ����� ����
	//
	bool					m_bCastleBattle;				// ������� ����������
	bool					m_bSendThreatenPacket;			// ���� ���� ��û ��Ŷ ������
	bool					m_bThreatening;					// ���� ��

	int						m_nCastleMapId;					// ���� ���� �������� ����� �����ϰ� �ִ°�
	DWORD					m_dwRoleInCastleBattle	: 8;	// ����� �� ���� ����	1:�Ѵ���
	DWORD					m_dwAttackType			: 8;	// attacker:1  defender:2
	DWORD					m_dwResultCastleBattle	: 8;	// win:1	lose:2
	DWORD					m_dwDeadCount			: 8;	// ȥ�� ���� Ƚ��
	DWORD					m_dwDieStartTime;
	DWORD					m_dwDeadPenaltyTime;
	int						m_nCBMapIdInWar;				// ���� ���� ���� ����� map id
	
	// ������� �������� ��/ȸ ����
	int						m_nDefenderType;
	int						m_nDefenderIndex;

	int						m_nAttackerType[max_count_of_challenge_list];
	int						m_nAttackerIndex[max_count_of_challenge_list];
	
	
	list <_XPKTRACING_INFO*>		m_listPKTracing;		// pk�� ��ġ ���� ����
	
	// �� ��ȸ ����
	int						m_nKillCount;
	int						m_nDeadCount;
	
	// ������ - �пյ� ���Ա��� �����ð�
	DWORD					m_dwBWLeftTimeFromServer;		// ms
	DWORD					m_dwBWReceiveTimeFromServer;	// ms
	
	BOOL					m_bSendFunctionalMovePacket;	// fuctional object�� �̵� ó�� �߿� ���� ����� �޾ƾ� �ϴ°�?
	int						m_nFuctionalNormalAction;		// � action�� �����ؾ� �ϴ°�? (0:����   1:Normal  2:Exception)

	void					GetBWLeftTime(int& hour, int& min, int& sec);
	void					DrawBWLeftTime();


// Get/Set functions
public:
	void					SetNotorious(int nValue) { m_nNotoriousValue = nValue; }  //Author : ����� //breif : ���� �ý��� 
	const int				GetNotorious() const { return m_nNotoriousValue; }  //Author : ����� //breif : ���� �ý��� 

	void					ResetCastleBattleInfo();
	BOOL					IsEnemyInCB(int ortype, int orindex, int& attacktype);	// ���� ���ΰ�? (Defender or Attacker)

	BOOL					GetEnterMatchEvent(void);	// ��ȸ �������ΰ�?
	BOOL					GetEnterCastleBattle(void);	// ����� �������ΰ�?
	BOOL					GetEnterMatchReadyArea(void);	// ��ȸ �����߿� ��Ȱ ������ �ִ°�?
	
	
#ifdef _XDWDEBUG
	void					DrawMovePath( void );
#endif

	void					SelectCharacter( int characterindex );

	_XUserItem*				GetUserItem( int firstcategory, int id );
	_XUserItem*				GetInventoryEmptySlot( void );
	int						GetUserItemInvenSlot(int firstcategory, int id);
	
	_XUserItem*				GetUserItemFromSecondType(int cType, int sType);
	int						GetUserItemInvenSlotFromSecondType(int cType, int sType);
		
	void					ResetInventoryInfo( void );
	void					ResetWarehouseInfo( void );	

	_XUserSkill*			GetSkillItem( short uniqueid );
	int						GetLearnedSkillIndex(short uniqueid, BOOL bIncludeEventSkill = FALSE);
	void					ResetSkillList(void);
	int						GetLearnedMasterySkillIndex(short uniqueid);
	
	BOOL					HaveSkillType(char clan, char stype = 0);
	
// Static functions
public:
	static	void			InitializeUserDataTable( void );
	static	void			GetUserLevel( DWORD experience, DWORD& levelindex, DWORD& sublevel );
	
// Operation functions
public:
	virtual void			SetAttackTarget( _XItem* target, _XPICKINGTARGETTYPE targettype );
	void					SendAttackSignal( void );
	void					SetViewTargetToAttackTarget(void);
	
	void					MoveStartPosition( void );
	void					ResetLocalUserData( void );
	
	void					ResetLocalUserTempData(void);	
	void					RecalcLocalUserParameter( void );
	void					UpdateLocalUserLevel( void );	
	void					UpdateLocalUserAttackLevel( void );	
	void					GetLocalUserLevel( void );

	void					SetSkillAnimation(void);
	void					SetSkillAnimation(int motionindex);
	void					ResetSkillAnimation(bool changeskill = false);

	void					RequestBattleMode(_XBATTLEMODETYPE mode);
	void					SetBattleMode(_XBATTLEMODETYPE mode);
//	void					SetActiveSkill(void);
	void					ResetActiveSkillAnimationControl(void);

	void					ProcessBattle(void);
	
	void					DrawBattle(void);
	void					DrawBattleRoundingSpell(void);
	void					DrawBattleCastingSpell(void);

	void					ProcessCommonAction(void);
	void					ProcessNormalAction(void);

	virtual void			ProcessBattleActiveSkill(void);	// animation cutting

	virtual void			ProcessBattleChargeSkill(void);	// hotblood zerolevel
	void					ProcessBattleFinishSkill(void);
	void					ProcessBattleSmashingSkill(void);
	void					ProcessBattleActionSkill(void);

	void					ProcessBattleSelfRoundingSpell(void);
	void					ProcessBattleTargetRoundingSpell(void);
	void					ProcessBattleAreaRoundingSpell(void);
	
	void					ProcessBattleSelfCastingSpell(void);
	void					ProcessBattleTargetCastingSpell(void);
	void					ProcessBattleAreaCastingSpell(void);

	void					ProcessBattleHiddenMA(void);

	void					ProcessBattleSelfChannelingSpell(void);
	void					ProcessBattleTargetChannelingSpell(void);
	void					ProcessBattleAreaChannelingSpell(void);
	BOOL					ProcessBattleChannelingNextStep(void);
	
	BOOL					CheckTargetTypeFinal(void);

	void 					ProcessCollectEvent(void);

	void					GetTargetPosition(void);
	void					GetTargetPositionIdleMode(void);
	void					GetTargetPositionBattleMode(void);
	
	BOOL					CanNotMove(void);
	BOOL					CheckValidNextPosition(void);
	void					ProcessNextMovePosition(void);
	virtual void			ProcessAnimation(void);	// hotblood zerolevel
	void					ProcessParameter(FLOAT progresstime);
	void					ProcessInternalUse(void);
	void					ProcessReadBook(void);
	void					ProcessSpeedSkill(void);

	void					CheckDelayTime(void);
	void					ApplyNextSkill(void);

	BOOL					CheckDistanceToTarget(D3DXVECTOR3 targetposition, short skillid, FLOAT& range, unsigned short mobType = 0, bool bAttackAfterMove = false);
	void					SetTargetPosition(D3DXVECTOR3 targetposition, FLOAT range);
	void					InsertTargetPositionEffect( void );

//	void					SetActiveSlotSkill(void);

	void					SetViewTarget(_XItem* target, _XPICKINGTARGETTYPE targettype, BOOL bsetpkready = FALSE);
	void					ResetViewTarget(void);	
	void					ResetAttackTarget(void);

	void					SetSpellAttackTarget(_XItem* target, _XPICKINGTARGETTYPE targettype);
	void					ResetSpellAttackTarget(void);

	void					GetItemTileIndex(FLOAT x, FLOAT z, _XTILE_ITEM& itemtile);
	void					SetTargetPositionAngle(D3DXVECTOR3 targetposition);
	void					UpdateRotateAngle(void);					

	void					ProcessTraining(void);
	void					ProcessTrainingMeditation(void);
	void					ProcessTrainingDummy(void);
	void					ProcessTrainingSpell(void);
	void					ProcessTrainingFireWood(void);
	void					ProcessTrainingWaterDraw(void);
	void					ResetTrainingMode(void);

	void					ProcessMatch(void);

	// �г�-���� ����ó�� ����
	BOOL					IsTroubleStateNormal(void);
	BOOL					IsTroubleStateFury(void);
	BOOL					IsTroubleStateConvergence(void);
	
	int						CheckPartyMember( int partymemberuniqueid );

	void					ResetMatchState(void);

	// ������ ����
	void					GrabWorldItem(void);
	int						GetEmptyInventorySlot(_XGameItem* pItem);
	void					UseItem(int invennumber);

	// ĳ���� ���� ȿ�� ó��
	void					ProcessStateList(void);
	void					DrawStateList(void);
	DWORD					GetLeftTimeFromStateList(char fromtype, short effectid, short skillid = 0, char slotindex = 0);
	void					CharacterStateChatLock(TCHAR* message);
	void					ApplyCharacterStateBonus(void);
	void					ApplyCharacterStatePenalty(void);
	void					ApplyCharacterStateFinal(void);
	_XEN_4EFF_SYS			FindMaxDamage(void);
	
	void					SetMonsterWarningMessage(unsigned short mobtype);
	
	// MISC 
	void					DeleteWeaponModel(short skillid);
	void					DeleteWeaponModel(void);
	void					ReloadWeaponModel(void);
	
	// ��
	void					SetDefaultValueMatchTable(void);

	//��� ����� ǥ�� ó��
	void					ViewCastingDamage(void); //Author : ����� //breif : �̱� ��� ����� �ȳ����� ����
	
// Overrides  functions
public:
	void					UpdatePosition( void );
	void					SetAnimation( _XMOTIONCLASS_TYPE motionclass, bool changemotion = true );
	void					SetAnimationIndex( int animationindex, bool changemotion = true );

	void					Process( void );
	void					Render( void );

	BOOL					GetNextMove( D3DXVECTOR3& next );

	void					SetDieMotion(void);
	void					SetBlowMotion(void);
	void					SetDefenceMotion(void);

	void					ProcessRescueUser(void);
	void					ProcessMobDie(void);

	void					SetRebirthBinsaMode(void);
	void					SetDyingMode(void);
	
	short					GetInterpolateMotionIndex(short skillid);

	virtual short			GetStanceMotion(short activeslotid);	// hotblood Zerolevel
	virtual short			GetMotionIndex(short selectedskillid); // hotblood Zerolevel
	virtual char			GetUsefulType(char selectedskillid);	// hotblood Zerolevel

	// ��������� �� ó���ϱ�
	void					Process_VirtualMode( void );
	void					ProcessAnimation_VirtualMode( void );
	void					ProcessBattle_VirtualMode( void );
	void					ProcessActiveSkill_VirtualMode( void );
	void					ProcessMobDie_VirtualMode( void );
	
	// LPS �ý��� ó��
	BOOL					ProcessLPSSystem(_XMob* pMob);  
	
	void					CheckCollideStatusInIdle( void );
	
	void					ResetLock(BOOL bCondition = TRUE);

	BOOL					SetPKReadyMode(void);
	void					ProcessPKMode(void);
	void					ResetPKMode(void);
	void					ResetPKReadyMode(void);
	BOOL					CheckPKTarget(_XUser* _user);
	BOOL					CheckPKPenalty(_XUser* _user);

	DWORD					GetPEModeLeftTime(void);
	DWORD					GetShoutLeftTime(void);
	
	// �ڸ� ��� ó��
	void					SetOut(BOOL state);
	
	BOOL					GetInBossRoom(void);

//Author : �����
	const BOOL GetChangingWeapon() const { return m_bChangingWeapon; }
	void SetChangingWeapon( const BOOL bRes ) { m_bChangingWeapon = bRes; }
	const BOOL GetChangingWeaponPacket() const { return m_bChangingWeaponPacket; }
	const int GetChangeWeaponModel() const { return m_nChangeWeaponModel; }
	void SetChangingWeaponPacket( const BOOL bRes, const int nModel ) 
	{
		m_bChangingWeaponPacket = bRes; 
		m_nChangeWeaponModel = nModel;
	}

	inline const PCROOM_App & GetPCroomInfo() const { return m_dwPCroomInfo; }
	inline const DWORD & GetPCroomRemain() const { return m_dwPCroomInfo.dwRemain; }
	const DWORD GetPCroomState( const _XUSER_PCROOM_STATE PCroomState ) const;
	
	inline PCROOM_App SetPCroomInfo() { return m_dwPCroomInfo; }
	inline void SetPCroomRemainData( const DWORD dwData) { m_dwPCroomInfo.dwRemain = dwData; }
	void SetPCroomState( const _XUSER_PCROOM_STATE PCroomState, const DWORD dwResult );
//LastUpdated : 07/03/15

//Author : �����
#ifdef _XDEF_SKILLDEALY_MODIFY
	inline void SetFinallySelectedSkillID( const short selectedskillid ) { m_FinallySelectedSkillID = selectedskillid; }
	inline const short GetFinallySelectedSkillID() const { return m_FinallySelectedSkillID; }
	inline void SetFinallySelectedSkillItem( _XUserSkill* const SelectedSkillItem ) { m_FinallySelectedSkillItem = SelectedSkillItem ; }
	inline const _XUserSkill* GetFinallySelectedSkillItem() const { return m_FinallySelectedSkillItem; }
#endif
	//Last Updated : 07/03/22


};

#endif
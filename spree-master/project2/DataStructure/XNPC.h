// XNPC.h: interface for the XNPC class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XNPC_H_
#define _XNPC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XObjectManager.h"
#include "XUser.h"
#include "XWindow_NPCChatBalloon.h"

#include <set>
#include "XModelManager.h"

//////////////////////////////////////////////////////////////////////////////////////
////  NPC define section  ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
/////  NPC SubScript Action ���� /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define _XDEF_NPCSUBSCRIPTCLASS_TOTALCOUNT		    42		

#define _XDEF_NPCSUBSCRIPTCLASS_NONE			   -1		//
#define _XDEF_NPCSUBSCRIPTCLASS_TRADEITEM		    0		// ������ �Ÿ�
#define _XDEF_NPCSUBSCRIPTCLASS_REPAIRITEM			1		// ���� ����
#define _XDEF_NPCSUBSCRIPTCLASS_INFORMATION			2		// ��Ʈ ����
#define _XDEF_NPCSUBSCRIPTCLASS_BLACKMARKET			3		// �ϰŷ�
#define _XDEF_NPCSUBSCRIPTCLASS_SELLHERB		    4		// ���� ����
#define _XDEF_NPCSUBSCRIPTCLASS_SELLCHARM			5		// ���� ����
#define _XDEF_NPCSUBSCRIPTCLASS_JUDGEVALUE		    6		// ������ ����
#define _XDEF_NPCSUBSCRIPTCLASS_GAMBLE				7		// �̴ϰ��� ����
#define _XDEF_NPCSUBSCRIPTCLASS_MOVE				8		// ���� �̵�
#define _XDEF_NPCSUBSCRIPTCLASS_WARP				9		// ���� �̵�
#define _XDEF_NPCSUBSCRIPTCLASS_SAVEITEM			10		// â�� ��� : ������ ����
#define _XDEF_NPCSUBSCRIPTCLASS_EXITGAME			11		// ���� ����
#define _XDEF_NPCSUBSCRIPTCLASS_STARTQUEST		    12		// ���� ���ѵ� ����Ʈ �ֱ�
#define _XDEF_NPCSUBSCRIPTCLASS_ENDSCRIPT		    13		// ��ũ��Ʈ ����
#define _XDEF_NPCSUBSCRIPTCLASS_DIALOG				14		// ��ȭ
#define _XDEF_NPCSUBSCRIPTCLASS_QA					15		// ����
#define _XDEF_NPCSUBSCRIPTCLASS_ADDSAVEITEM			16		// â�� �߰�
#define _XDEF_NPCSUBSCRIPTCLASS_LEARNSKILL			17		// ���� ����
#define _XDEF_NPCSUBSCRIPTCLASS_REPAIRCLOTHES		18		// �ǻ� ����
#define _XDEF_NPCSUBSCRIPTCLASS_SAVEPOINT			19		// ��Ȱ ���� ����
#define _XDEF_NPCSUBSCRIPTCLASS_CLANSERVICEPOINT	20		// ���� �⿩�� 
#define _XDEF_NPCSUBSCRIPTCLASS_HEALPLAYER			21		// ġ���ϱ�
#define _XDEF_NPCSUBSCRIPTCLASS_EXPLAINCLASS		22		// ���Ҽ��� 
#define _XDEF_NPCSUBSCRIPTCLASS_MAKEORGANIZATION	23		// ������â�� 
#define _XDEF_NPCSUBSCRIPTCLASS_MINISTOPEVENT		24		// �̴Ͻ��� �̺�Ʈ
#define _XDEF_NPCSUBSCRIPTCLASS_SHOWRANKQUEST		25		// ����Ʈ ���� �����ֱ�
#define _XDEF_NPCSUBSCRIPTCLASS_PVPLIST				26		// �� ��� �����ֱ�
#define _XDEF_NPCSUBSCRIPTCLASS_SOCKETEXPANSION		27		// ���� Ȯ���ϱ�
#define _XDEF_NPCSUBSCRIPTCLASS_SOCKETDESTROY		28		// ���� �и�/�ı��ϱ�
#define _XDEF_NPCSUBSCRIPTCLASS_VIEWFREEMATCHINFO	29		// �񹫴�ȸ ��������
#define _XDEF_NPCSUBSCRIPTCLASS_COLLECTION			30		// ��ǰ����
#define _XDEF_NPCSUBSCRIPTCLASS_RESOURCE			31		// �ڿ�����
#define _XDEF_NPCSUBSCRIPTCLASS_INCHANT				32		// ���� ����
#define _XDEF_NPCSUBSCRIPTCLASS_NPCBUFF				33		// NPC Buff
#define _XDEF_NPCSUBSCRIPTCLASS_MOVEVILLAGE			34		// ǥ���̵�
#define _XDEF_NPCSUBSCRIPTCLASS_CASTLEINFO			35		// ������� 
#define _XDEF_NPCSUBSCRIPTCLASS_CASTLEATTACKLIST	36		// ��������ڸ��
#define _XDEF_NPCSUBSCRIPTCLASS_TRACING				37		// Ư���� ��ġ ����
#define _XDEF_NPCSUBSCRIPTCLASS_ENTERCONTEST		38		// �񹫴�ȸ ����
#define _XDEF_NPCSUBSCRIPTCLASS_VIEWCONTEST			39		// �񹫴�ȸ ����
#define _XDEF_NPCSUBSCRIPTCLASS_MIXWEAPON			40		// ��������
#define _XDEF_NPCSUBSCRIPTCLASS_CLANPOINTITEM		41		// ���ı⿩���� ������ ����
#define _XDEF_NPCSUBSCRIPTCLASS_MASTERNAME			42		// ��ȣ �Է�
#define _XDEF_NPCSUBSCRIPTCLASS_MNDINFO				43		// ���� ����
#define _XDEF_NPCSUBSCRIPTCLASS_MNDPOINTPURCHASE	44		// �����⿩���� ������ ���



//////////////////////////////////////////////////////////////////////////////////////
/////  NPC SubScript Condition ����  /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_NPCSUBSCRIPT_CONDITION_COUNT						5

#define ID_NPCSUBSCRIPT_CONDITION_CHECKCLANNPC				0		// �÷��̾�_NPC���İ˻�
#define ID_NPCSUBSCRIPT_CONDITION_CHECKCLANFRIENDSHIP		1		// �÷��̾�_NPC��ȣ���İ˻�
#define ID_NPCSUBSCRIPT_CONDITION_CHECKCLANHOSTILITY		2		// �÷��̾�_NPC���빮�İ˻�
#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR				3		// �÷��̾���˻�
#define ID_NPCSUBSCRIPT_CONDITION_NPCEXISTENCE				4		// NPC ���� or ���� ����
#define ID_NPCSUBSCRIPT_CONDITION_CHECKPLAYERROLE			5		// �÷��̾� ���� �˻�

// NPC SubScript Ȱ�� / ��Ȱ�� ---------------------------------------------------------------
#define ID_NPCSUBSCRIPT_DISABLE						0
#define ID_NPCSUBSCRIPT_ENABLE						1

// NPC SubScript ������ �ʱ�ȭ ---------------------------------------------------------------
#define ID_NPCSUBSCRIPT_SETDATA_NONE				-1

// NPC SubScript ���ǹ��� ��� ���� ----------------------------------------------------------
#define ID_NPCSUBSCRIPT_CONDITIONFUNCTION_OR		0
#define ID_NPCSUBSCRIPT_CONDITIONFUNCTION_AND		1


// NPC SubScript Condition ���
typedef enum
{
	NPC_CONDITION_RESULT_FAIL = 0,				// ����
	NPC_CONDITION_RESULT_SUCCESS,				// ����
}_XNPCCONDITION_RESULT;



//////////////////////////////////////////////////////////////////////////////////////
/////  NPC SubScript Condition ���� : ���� üũ  /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_NPC_CLAN_COUNT						9

#define ID_NPC_CLAN_NANGIN						0	// ����
#define ID_NPC_CLAN_GAEBANG						1	// ����
#define ID_NPC_CLAN_BEEGOONG					2	// ���
#define ID_NPC_CLAN_SORIM						3	// �Ҹ�
#define ID_NPC_CLAN_NOCKRIM						4	// �츲
#define ID_NPC_CLAN_MOODANG						5	// ����
#define ID_NPC_CLAN_MAKYO						6	// ����
#define ID_NPC_CLAN_SEGA						7	// ����
#define ID_NPC_CLAN_NOFIXED						8	// �̸�����


//////////////////////////////////////////////////////////////////////////////////////
///// NPC SubScript Condition ���� : �÷��̾���˻�  ///////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_HONOR_COUNT										6

#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_BUMIN			0	// ����
#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_KANGHO			1	// ��ȣ
#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_MULIM			2	// ����
#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_CHUNHA			3	// õ��
#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_JULSE			4	// ����
#define ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_ILDEA			5	// �ϴ�


// NPC ���� or ���� ���� ----------------------------------------------------------
#define ID_NPCSUBSCRIPT_NPCEXISTENCE_ON				0
#define ID_NPCSUBSCRIPT_NPCEXISTENCE_OFF			1

//////////////////////////////////////////////////////////////////////////////////////
///// NPC SubScript Condition ���� : NPC ���� �Ҹ� üũ  /////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_NPCSUBSCRIPT_CONDITION_CHECKONOFF_LEVEL			0	// ���� üũ 
#define ID_NPCSUBSCRIPT_CONDITION_CHECKONOFF_QUEST			1	// ����Ʈ üũ

//////////////////////////////////////////////////////////////////////////////////////
/////  NPC Timer ����  ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

#define _XDEF_NPCTIME_CHATBALLOON				5000	// NPC ��ǳ���� ���ִ� �ð� 5��							
#define _XDEF_NPCTIME_CHATBALLOONWAITING		30000	// ������ ��ǳ���� �� ������ ��� �ð� 30�� 					
#define _XDEF_NPCTIME_SOUNDPREVGREETING			15000	// ��ǳ���� �������� ȥ�ڸ� ���尡 �ߴ� �ð� 15��


typedef enum
{
	_XDEF_NPCMESSAGE_NORMAL = 0,
	_XDEF_NPCMESSAGE_QUEST,
	_XDEF_NPCMESSAGE_SAVEPOINT,
	_XDEF_NPCMESSAGE_ORGANIZATION,
	_XDEF_NPCMESSAGE_MINISTOPEVENT,
	_XDEF_NPCMESSAGE_HAVEQUESTLIST,
	_XDEF_NPCMESSAGE_TRACING,
	_XDEF_NPCMESSAGE_MASTERNAME //Author : ����� //��ȣ �Է�
}_XNPCMESSAGETYPE;

typedef enum _XNPC_MOTION
{
	// default motion...
	_XNPCMOTION_IDLE		= 0,	// �Ϲ� ���
	_XNPCMOTION_IDLEACTION_1,		// ����� ���� 1
	_XNPCMOTION_IDLEACTION_2,		// ����� ���� 2
	_XNPCMOTION_CONVERSATION,		// ��ȭ ���

	_XNPCMOTION_WALK,				// �ȱ�1
	_XNPCMOTION_WALK2,				// �ȱ�2
	_XNPCMOTION_RUN,				// �޸���

	_XNPCMOTION_ATTACK,				// ����
	_XNPCMOTION_DAMAGE,				// �±�
	_XNPCMOTION_DIE,				// �ױ�

	_XNPCMOTION_STANDUP,			// ���� ���¿��� �Ͼ�� (IDLE->STANDUP)
	
	_XNPCMOTION_TOTALCOUNT
};

typedef struct _XNPCMotionExistTable
{
	char	motion_idle;
	char	motion_idleaction1;
	char	motion_idleaction2;
	char	motion_conversation;

	char	motion_walk;
	char	motion_walk2;
	char	motion_run;

	char	motion_attack;
	char	motion_damage;
	char	motion_die;

	char	motion_standup;

	_XNPCMotionExistTable()
	{
		motion_idle				=	-1;
		motion_idleaction1		=	-1;
		motion_idleaction2		=	-1;
		motion_conversation		=	-1;
		motion_walk				=	-1;
		motion_walk2			=	-1;
		motion_run				=	-1;
		motion_attack			=	-1;
		motion_damage			=	-1;
		motion_die				=	-1;
		motion_standup			=	-1;
	}

} _XNPCMotionExistTable;

typedef enum
{
	_XNPCROLL_NONE = 0,
	_XNPCROLL_TRADE,
	_XNPCROLL_EVENT,
	_XNPCROLL_WAREHOUSE,	// 204.06.01->oneway48 insert
	_XNPCROLL_BLACKMARKET	// 204.06.01->oneway48 insert
}_XNPCROLLTYPE;

typedef enum
{
	_XNPC_HAVEQUEST_NONE = 0,
	_XNPC_HAVEQUEST_YELLOW,
	_XNPC_HAVEQUEST_GREEN,
	_XNPC_HAVEQUEST_BLUE,
	_XNPC_HAVEQUEST_ORANGE,
	_XNPC_HAVEQUEST_WHITE,
	_XNPC_HAVEQUEST_QUESTIONYELLOW,
	_XNPC_HAVEQUEST_QUESTIONGREEN,
	_XNPC_HAVEQUEST_QUESTIONBLUE,
	_XNPC_HAVEQUEST_QUESTIONORANGE,
	_XNPC_HAVEQUEST_QUESTIONWHITE,
}_XNPCQUESTTYPE;

typedef enum
{
	_XNPC_JOBCLASS_FOOD = 0,
		_XNPC_JOBCLASS_CLOTHES,
		_XNPC_JOBCLASS_REFINING,
		_XNPC_JOBCLASS_HOTEL,
		_XNPC_JOBCLASS_WAREHOUSE,
		_XNPC_JOBCLASS_DRUGSTORE,
		_XNPC_JOBCLASS_SHOP,
		_XNPC_JOBCLASS_WEAPON,
		_XNPC_JOBCLASS_COMMONPEOPLE,
		_XNPC_JOBCLASS_GAEBANGPEOPLE,
		_XNPC_JOBCLASS_SORIMPEOPLE,
		_XNPC_JOBCLASS_NOCKRIMPEOPLE,
		_XNPC_JOBCLASS_BEEGOONGPEOPLE,
		_XNPC_JOBCLASS_MOODANGPEOPLE,
		_XNPC_JOBCLASS_MAKYOPEOPLE,
		_XNPC_JOBCLASS_BLACKMARKET,
		_XNPC_JOBCLASS_MILITARYOFFICER,
		_XNPC_JOBCLASS_HEANGSANGIN,
		_XNPC_JOBCLASS_BUFF,
		_XNPC_JOBCLASS_RESOURCE,
		_XNPC_JOBCLASS_COLLECTION,
		_XNPC_JOBCLASS_GUARD,
		_XNPC_JOBCLASS_REPAIR,
		_XNPC_JOBCLASS_CASTLECAPTAIN,	// ���� (��� ����)
		_XNPC_JOBCLASS_CASTLEATTACT,	// ����ȳ���(������) 
		_XNPC_JOBCLASS_CASTLEDEFFENCE,	// ���������(������)
		_XNPC_JOBCLASS_TRACER,			// ������

}_XNPCJOBCLASS;

typedef enum
{
	_XNPC_CLASS_COMMON = 0,
		_XNPC_CLASS_CLAN,
		_XNPC_CLASS_ANIMAL,
		_XNPC_CLASS_HERO,
		_XNPC_CLASS_ONOFFTYPE		
}_XNPCCLASS;

class _XWindow_NPCScript;

class _XNPCObject : public _XObject
{
public :
	_XWindow_NPCScript*	m_pNPCScriptWindowPtr;
	int					m_UniqueID;
	_XNPCJOBCLASS		m_NPCJobClass;				// NPC ������ ��Ÿ�� : ����, ������, ���尣, ����, â��, ���, ����, ö��
	_XNPCROLLTYPE		m_NPCRollType;
	_XGROUPTYPE			m_NPCClanClass;				// NPC ���� �� ���� ���ý� ���� ���� -> ����, �豳, ����, ����, �Ҹ�, �츲
	_XNPCCLASS			m_NPCClass;					// NPC ���� -> �Ϲ�, ����, ����, ���� , ����or����
	int					m_TradeItemClass;
	LPTSTR				m_NPCName;
	LPTSTR				m_NPCRoll;
	
	BOOL				m_bCheckClanNPC;			// PC�� NPC�� ���� �������� �ƴ����� üũ
	BOOL				m_bCheckFriendship;			// PC�� NPC�� ��ȣ���� �������� �ƴ����� üũ
	BOOL				m_bCheckHostility;			// PC�� NPC�� �������� �������� �ƴ����� üũ
	
	int					m_SelectedMessageIndex;
	int					m_SelectedSubScriptIndex;
	int					m_SelectedMainScriptIndex;
	int					m_PrevMainScriptIndex;

	int					m_MainScriptIndex;
	
	_XModelDescriptor	m_ModelDescriptor;
	DWORD				m_SpecialActionTimer;	
	_XList				m_Triggerlist;
	BOOL				m_InsideFrustum;
	FLOAT				m_Distance;
	FLOAT				m_DistanceFromLocalUserToNPC;
	
	_XNPC_MOTION		m_CurrentMotion;
	
	_XNPCMESSAGETYPE	m_MessageType;
	LPTSTR				m_MessageString;
	BOOL				m_ChangeMessage;
	BOOL				m_ChangeButton;
	BOOL				m_ChangeNormalMessage;
	
	BOOL				m_bStartQuestDialog;	// NPC�� ����Ʈ�� ���� ��縦 ������ ����.

	_XWindow_NPCChatBalloon		m_NPCChatBalloonObject;
	
	// Quest
	_XNPCQUESTTYPE		m_HaveQuestType;	
	int					m_HaveQuestID;			// NPC�� ������ �ִ� ����Ʈ ���̵� 
	
	BOOL				m_bNPCDialogMode;
	
	BOOL				m_bDrawNPCChat;
	DWORD				m_NPCChatBallonTimer;	// NPC ȥ�ڸ� ���� �ð��� ���� ���� 
	
	BOOL				m_bShowNPCModel;		// NPC�� Rendering ���� �� ������ ���� 
	int					m_EventType;			//�̺�Ʈ ���� 0:������-�йи���Ʈ 1:�������� 2:������߱�

public :
	_XNPCObject();
	virtual ~_XNPCObject();	
	
	BOOL	LoadScript( LPSTR Filename );
	BOOL	LoadScript( FILE* FilePtr );	
	
	BOOL	CheckIntersectRay( void );
	
	void	Process(void);
	void	ProcessNPCNameAndChat(void);
	void	Render(void);	
	
	void	DrawNPCNameWindow(void);
	void	DrawNPCChat(void);
	void	PlaySoundNPCPrevGreeting(void);

	void	SetMessageString(LPCTSTR string);
	void	SetMessageType(_XNPCMESSAGETYPE messagetype);
	void	SetChangeMessage(BOOL change);
	void	SetChangeButton(BOOL button);

	void	SetConversationAction(void);
	void	SetIdleAction(void);
	void	SetSurrenderAction(void);
	
	void	SetHaveQuestID(int questid)	{	m_HaveQuestID = questid;	};
	int		GetHaveQuestID(void)		{	return m_HaveQuestID;	};

	void	SetNPCDialogMode(BOOL bdialogmode )	{	m_bNPCDialogMode = bdialogmode;	};
	
	void	CreateEffectManager( void );
};


class _XNPCObjectManager : public _XModelManager
{
public:
	int					m_NPCObjectCount;
	_XNPCObject*		m_pNPCObjectArray;
	
	int					m_NPCObjectRenderCount;
	_XNPCObject**		m_pNPCObjectSortRenderArray;
		
	typedef std::set<int> ssdef_INT;
	ssdef_INT		m_ssNpcIndex;
public:
	_XNPCObjectManager();
	virtual ~_XNPCObjectManager();
	
	void			disposeList( void  );
	
	BOOL			LoadNPCObjectList( FILE* Fileptr, LPSTR Filename = NULL );
	
	void			CheckInsideFrustum( void );
	void			CheckConditionOfNPCExistence( void );
	void			Render( void );
	void			RenderNameToolTip( void );
	void			RenderNPCChatBalloon( int index );

	void			RemoveEffectInstance(int templateid);
	
	int				GetNPCIndexByNPCJob(_XNPCJOBCLASS job);		// job�� unique �Ҷ��� valid
};



#endif // !defined(_XNPC_H_)

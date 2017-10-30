/** ����Ʈ ��ũ��Ʈ �ϳ��� ��Ÿ���� Ŭ����.
 *
 * \author Sohyun, Park
 * \date 2003.08.26
 */

#ifndef _KUQUESTSCRIPTITEM_H_
#define _KUQUESTSCRIPTITEM_H_

#include "XQuestScriptBaseType.h"
#include "XQuestScriptDefine.h"
#include "XTriggerConditionItem.h"
#include "XTriggerItem.h"
#include "XTriggerTreeItem.h"

typedef enum _XQuestStart_Mode
{
	_XQUESTSTART_NONE = 0,
		_XQUESTSTART_NPCTARGETTING,
		_XQUESTSTART_OBJECTCLICK,
		_XQUESTSTART_USINGITEM,
		_XQUESTSTART_WEARITEM,
		_XQUESTSTART_DESTROYITEM,
		_XQUESTSTART_LEVEL,
		_XQUESTSTART_ZONE
}_XQUESTSTART_MODE;

typedef struct _XQuest_CounterInfomation
{
	int		CounterType;
	int		CounterTargetType;
	int		CounterTargetID;
	int		CountTotalPoint;
	int		CountCurrentPoint;
	TCHAR	CounterTargetName[256];
	
}_XQUEST_COUNTINFOMATION;

typedef struct _XQuest_PartyCounterInfomation
{
	int		CounterType;
	int		CounterTargetType;
	int		CounterTargetID;
	int		CountTotalPoint;
	int		CountCurrentPoint;
	int		CounterPointPerMember[9];
	TCHAR	CounterTargetName[256];
	
}_XQUEST_PARTYCOUNTINFOMATION;

#pragma warning (disable: 4786)
#include <map>
using namespace std;

class _XQuestScriptItem
{
protected :
	BOOL			m_bEnable;
	BOOL			m_bAutorun;
	BOOL			m_bMainQuest;		//������ ����
	BOOL			m_bPartyQuest;		//��Ƽ ����Ʈ ����
	bool			m_bTempQuest;		//������� �ʴ� ����Ʈ ���� �ݺ� ������ ����Ʈ ����

	BOOL			m_bClientEnable;
	BOOL			m_bServerEnable;
	BOOL			m_bConfirmAll;
	
	BOOL			m_bComplete;
	BOOL			m_bInsertCompleteList;
	
	BOOL			m_bWaitingFailResult;
	BOOL			m_bCheckFirstStartCondition;
	
	BOOL			m_bRewardProcessNPCMarkCheck;

	LPTSTR			m_lpszQuestName;
	LPTSTR			m_lpszQuestSubName;

	BYTE			m_byQuestCancelCase;
	BYTE			m_QuestEmailType;
	BYTE			m_byStartConditionCompareClass;
	BYTE			m_byQuestOptionCompareClass;// �ɼ� ���ǽ��� ��θ���, �ϳ��̻� ���� 
	
	BYTE			m_RankQuestType1;		//0:��������Ʈ 1:��������Ʈ 2:������Ʈ 
	BYTE			m_RankQuestType2;		//0:KillCount 1:Timer 2:ItemCount	
	
	float			m_queststartprobability;
	
	int				m_nQuestID;
	int				m_QuestChapter;
	int				m_nStartTime;
	int				m_nQuestMailDelayTime;

	int				m_nNextTriggerID;
	int				m_nCurrentTriggerID;
	
	int				m_NextTreeNodeNumber;
	int				m_CurrentTreeNodeNumber;

	int				m_FailConditionTimer ;		// �������ǽ� �˻縦 ���� ������
	
	int				m_nLastTargettingNPCID;

	_XQUEST_COMPLETE_RESULT		m_QuestServerResult;
	_XQUEST_COMPLETE_RESULT		m_QuestCompleteResult;

	
	// ����Ʈ�� ���� ���� Ÿ���� �����Ͽ� Ÿ�Կ� �´� ������ �� ���� ������ �˻��Ͽ� ���ϸ� ���δ�.
	_XQUESTSTART_MODE			m_QuestStartMode;

	_XQUEST_TYPE				m_QuestType;// ����Ʈ Ÿ�� ( ������, ������, ������, �⿬��, �����һ� )
	
	int				m_nStartYear;
	int				m_nStartMonth;
	int				m_nStartDay;
	int				m_nStartHour;

	int				m_nEndYear;
	int				m_nEndMonth;
	int				m_nEndDay;
	int				m_nEndHour;

	unsigned int	m_unCountryCode;

public :
	LPTSTR			m_lpszStartQuestMessage;
	LPTSTR			m_lpszFailQuestMessage;
	LPTSTR			m_lpszOutlineMessage;
	LPTSTR			m_lpszOutlineMessage2;
	LPTSTR			m_startquestinformationmessage;
	LPTSTR			m_lpszQuestProgressinformation;//����Ʈ â�� ǥ�õ� ����Ʈ ���� ����

	int				m_SelectionNumber;
	BOOL			m_bSelectionNumberCheck;
	
	int				m_QuestScriptItemIndexNumber; // ����Ʈâ������ ����Ʈ ����Ʈ ��ġ�� ǥ���ϱ� ���� ����
	BOOL			m_bSelectedQuestScriptItem; // ����Ʈâ������ ����Ʈ ����Ʈ ��ġ�� ǥ���ϱ� ���� ����

	BOOL			m_bChangeQuestStartMode; // ����Ʈ ���۸�尡 ��������  

	// ����Ʈ ���� ������ ������ ������ �� �� �ִ� ���� 
	int				m_QuestStartConditionValue; // 100000 : ObjectClick
												// 10000  : Using Item
												// 1000	  : Wear Item
												// 100	  : Level
												// 10	  : Zone or None
												// 1	  : NPC targetting
												// 1000000: Destroy Item 
	
	int				m_SuccessStartConditionid[20];

	_XQUEST_COUNTINFOMATION m_CounterInformation[4];// ī���� ����Ʈ�� �����ϴ� �迭
	_XQUEST_COUNTINFOMATION m_ItemCounterInformation[8];
	_XQUEST_PARTYCOUNTINFOMATION m_PartyCounterInformation[4];// ī���� ����Ʈ�� �����ϴ� �迭

	map <int, _XTriggerConditionItem*> m_mapStartConditionList;
	map <int, _XTriggerConditionItem*> m_mapOptionConditionList;// �ɼ� ���ǽ� ����Ʈ
	map <int, _XTriggerConditionItem*> m_mapFailConditionList;// ������ �������� ������ ����
	map <int, _XTriggerItem*> m_mapSuccessTriggerList;
	map <int, _XTriggerTreeItem*> m_mapTriggerTreeList;
	map <int, int> m_mapTriggerTimer;

	int				m_LimitQuestCountStartType;		//�ο������� ����Ʈ ����
	int				m_LimitQuestCountCompleteType;	//�Ϸἱ���� ����Ʈ ����

	BOOL			m_bSendClientPacketAtPartyQuest; //��Ƽ����Ʈ���� �ڱ� �ڽ��� ���ʿ� �����ؼ� ��Ŷ�� ���´����� �� �� �ִ� ���� 

public :
	_XQuestScriptItem();
	~_XQuestScriptItem();
	
	void			SetStartTime(int starttime);
	int				GetStartTime(void);
	
	BOOL			GetPartyQuest(void)				{	return m_bPartyQuest;	};
	bool			GetTempQuest(void)				{	return m_bTempQuest;	};

	void			SetClientEnable(BOOL enable)	{	m_bClientEnable = enable;	};
	BOOL			GetClientEnable(void)			{	return m_bClientEnable;	};
	
	void			SetServerEnable(BOOL enable)	{	m_bServerEnable = enable;	};
	BOOL			GetServerEnable(void)			{	return m_bServerEnable;	};

	void			SetConfirmAll(BOOL enable);

	void			SetTriggerClientEnable(int triggerid, BOOL enable );
	void			SetTriggerServerEnable(int currenttreenumber, BOOL enable, BOOL jumpaction = FALSE );
	void			SetTriggerPlayActionAndServerEnable(int currenttreenumber, BOOL enable, BOOL bnormalaction, BOOL jumpaction = FALSE );
	void			SetTriggerConfirmAll(int triggerid, BOOL enable );

	void			SetProgressTrigger(int currenttreenumber, BOOL bNormal );
	void			SetTriggerComplete(int triggerid, BOOL enable);

	void			SetCurrentTriggerID(int triggerid);
	int				GetCurrentTriggerID(void);

	void			SetCurrentTreeNumber(int treenumber, BOOL bconditioncheck = TRUE );
	int				GetCurrentTreeNumber(void);
	
	void			SetNextTreeNumber(int nexttreenumber);
	int				GetNextTreeNumber(void);

	void			SetNextTriggerID(int triggerid);
	int				GetNextTriggerID(void);

	void			SetQuestID(int questid);
	int				GetQuestID(void);

	void			SetComplete(BOOL complete);
	BOOL			GetComplete(void);

	void			SetInsertCompleteList(BOOL complete);
	BOOL			GetInsertCompleteList(void);

	void			SetAutoRun(BOOL autorun);
	BOOL			GetAutoRun(void);

	void			SetWaitingFailResult(BOOL waiting);
	BOOL			GetWaitingFailResult(void);
	
	void			SetFirstStartCondition(BOOL checkfirststartcondition );
	BOOL			GetFirstStartCondition(void);
		
	void			ReSetQuestMark();
	void			ResetCounterInformation(void);
	void			ResetPartyCounterInformation(void);
	void			ResetItemCounterInformation(void);


	_XQUESTSTARTCONDITION_RESULT		CheckStartCondition(void);
	_XQUESTITEM_RESULT					Process(void);

	BOOL			ProcessRewordQuest(void);

	BOOL			Load(FILE* fileptr);
	int				LoadString(FILE* fileptr, LPTSTR& string);

	void							SetQuestServerResult(_XQUEST_COMPLETE_RESULT result);
	_XQUEST_COMPLETE_RESULT			GetQuestServerResult(void);

	void							SetQuestCompleteResult(_XQUEST_COMPLETE_RESULT result);
	_XQUEST_COMPLETE_RESULT			GetQuestCompleteResult(void);

	void			Reset(void);
	void			ReleasePrivateData(void);
	BOOL			GetTriggerComplete(int triggerid);

//	void			ProcessTest(int triggerid, int actionid);
//	void			RestoreTest(int triggerid, int actionid);

	int				GetQuestMailDelayTime(void) { return m_nQuestMailDelayTime;	};
	
	BOOL			GetMainQuest(void);
	
	LPTSTR			GetQuestName(void);
	LPTSTR			GetQuestSubName(void);

	void			SetLastTarggetingNPCID(int npcid)	{	m_nLastTargettingNPCID = npcid;	};
	int				GetLastTarggetingNPCID(void)		{	return m_nLastTargettingNPCID;	};

	int				GetOwnerNPCID(int triggerid);

	// ����Ʈ ���� ���� Ÿ���� ��ȯ�Ѵ�.
	void				SetQuestStartConditionMode(_XQUESTSTART_MODE queststartmode)	{	m_QuestStartMode = queststartmode;	};
	_XQUESTSTART_MODE	GetQuestStartConditionMode(void)	{	return m_QuestStartMode;	};
	
	// ����Ʈ Ÿ�� ( ������, ������, ������, �⿬��, �����һ� )
	_XQUEST_TYPE	GetQuestType(void)	{	return m_QuestType;	};

	BYTE			GetQuestCancleType(void)	{	return m_byQuestCancelCase;	};
	
	void			RemoveQuestStartConditionList(void);

	BOOL			CheckFailConditionAtOptionCondition(void);
	
	BYTE			GetQuestRankType1(void)		{ return m_RankQuestType1; };
	BYTE			GetQuestRankType2(void)		{ return m_RankQuestType2; };
	
	BOOL			CheckStartNEndTime(void);
	BOOL			CheckCountryCode(void);

};


#endif

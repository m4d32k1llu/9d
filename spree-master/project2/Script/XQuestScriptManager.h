/** ����Ʈ ��ũ��Ʈ�� �Ѱ��ϴ� Ŭ����.
 *
 * Main Function, ����/�Ϸ� ����Ʈ�� ���õ� ���� �ַ� �Ѵ�.
 * \author Sohyun, Park
 * \date 2003.08.26
 */

#ifndef _KUQUESTSCRIPTMANAGER_H_
#define _KUQUESTSCRIPTMANAGER_H_

#include "XQuestScriptDefine.h"
#include "XQuestScriptBaseType.h"
#include "XQuestScriptItem.h"

#pragma warning (disable: 4786)
#include <map>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

typedef struct 
{
	D3DXVECTOR3		vecCurrentPosition;
	D3DXVECTOR3		vecTargetPosition;
	FLOAT			fTargetDistance;
	FLOAT			fFov;
	FLOAT			fPitch;
	FLOAT			fYaw;
}_XCAMERA_ORIGINALINFO;

typedef struct 
{
	int		triggerid;
	int		needinven;
	int		starttime;
}_XQUEST_INVENFULL;

typedef struct 
{
	int				questid;
	int				npcid;
	FLOAT			MarkPosition_X;	
	FLOAT			MarkPosition_Y;	
	_XQUEST_TYPE	questtype; 
}_XQUEST_MARKINFO;

// ���� ���ǿ� ������ �˻簡 ������ �������� ������ �� �ٽ� �˻��ϱ� ���� ���� 
typedef struct 
{
	int				questid;
	char			itemtype;   // �ߺз�.
	short			itemid;		// ������ �Һз�. (������ ���̵�)
}_XQUEST_ITEMINFO;

// ���� ���ǿ� ���� �˻簡 ������ ���� ��½ÿ� �ٽ� �˻��ϱ� ���� ���� ( �̸��϶��� ��� üũ�ؾ� �Ѵ�.)
typedef struct 
{
	int				questid;
	int				maximumlevel;   // ���۰����� �ִ� ���� 
	int				equallevel;		// ������ ���� ��츸 ������ 
}_XQUEST_LEVELINFO;

// ǳ��� ��� üũ
typedef struct 
{
	int				questid;
	int				questmailtype;	// 0:Ȯ�� 1:����or�ź�
	int				resendstarttime;// ���� ���ǿ��� �ź��Ҷ� �ٽ� ������ ���� �ð��� ����
	int				delaytime;		// ���� ���ǿ��� ����Ŀ� �������� ���� �ð��� ���� 
	BOOL			bYESorNO;		// TRUE:���� FALSE:�ź� 
}_XQUESTMAIL_INFO;

// ������ ������ ���� ����ü
typedef struct 
{
	char			m_cHoldItemType;				// ��з�
	short			m_sHoldItemID;					// �Һз� (���̵�)
}_XQUEST_HOLDITEM;

// Ÿ���� ����Ʈ�� �����ϱ� ���� ����
typedef struct 
{
	int				questid;	// npc�� ������ �ִ� ����Ʈ 
	int				npcid;		// ����Ʈ���� ������ �ִ� NPC ���̵�
}_XQUEST_QUESTLIST;

typedef enum _XQuest_VirtualMode
{
	_XQUEST_VIRTUALMODE_ENTER_OTHERZONE = 0,
		_XQUEST_VIRTUALMODE_ENTER_CURRENTZONE,
		_XQUEST_VIRTUALMODE_ENTER_CURRENTZONEOTHERPOSITION,
		_XQUEST_VIRTUALMODE_LEAVE_COMEBACKZONE,
		_XQUEST_VIRTUALMODE_LEAVE_COMEBACKZONEOTHERPOSITION,
		_XQUEST_VIRTUALMODE_LEAVE_CURRENTZONE,
		_XQUEST_VIRTUALMODE_LEAVE_CURRENTZONEOTHERPOSITION,
		_XQUEST_VIRTUALMODE_LEAVE_OTHERZONEOTHERPOSITION,
}_XQUEST_VIRTUALMODE;
/** Quest script manager.
 * ����Ʈ ��Ʈ��Ʈ�� �Ѱ��ϴ� Ŭ����
 * \author Sohyun, Park
 * \date 2003.08.26 (revision)
 */
class _XQuestScriptManager
{
protected :
	
	int							m_nQuestScriptCount;			// Quest script ����
	int							m_nProgressQuestCount;			// �������� ����Ʈ ����
	int							m_nCheckQuestID;				// NPC script�� ���ؼ� ���� �����ؾ� �� ����Ʈ ID
	
	BOOL						m_bQuestCameraMode;				// Quest camera mode - TRUE�̸� ī�޶� �ڵ� �̵���
	BOOL						m_bCheckQuest;					// NPC script�� ���ؼ� ���� �������� �˷��ִ� ����
	BOOL						m_bQuestWideView;				// Wide ȭ�� ó��

public :
	
	DWORD						m_QuestTimerStartTime;
	DWORD						m_QuestTimerTimeLimit;	
	DWORD						m_QuestStartConditionTimer;		// ����Ʈ ���� ���� �����˻縦 ���� ������ 	
	DWORD						m_QuestSystemMessageStartTime;
	
	LPTSTR						m_lpszQuestSystemMessage;

	int							m_CurrentMainQuestID;			// ������ ����Ʈ�� �� ���� ����Ʈ ��ȣ
	int							m_NextMainQuestID;				// ������ ����Ʈ�� �� ���� ����Ʈ ��ȣ
	int							m_TimerStartQuestID;
	int							m_indexSelectNPCTargettingQuest;// �� NPC�� ���� ����Ʈ�� Ÿ������ �ɷ������� ������ ����Ʈ ��ȣ
	int							m_nVirtualZoneStartPositionX;	// ������������� ������ġ�� ������ �����Ѵ�.
	int							m_nVirtualZoneStartPositionZ;	// ������������� ������ġ�� ������ �����Ѵ�.
	int							m_nRealZoneStartPositionX;		// ����������� �ǵ��ư� ��ġ�� �����صд�.
	int							m_nRealZoneStartPositionZ;		// ����������� �ǵ��ư� ��ġ�� �����صд�.
	int							m_indexVirtualZone;				// ������� �����ε����� �����Ѵ�.
	int							m_indexRealZone;				// ����������� ���� ���� ���������� �����ε����� �����Ѵ�.
	int							m_ProgressSubQuestID[5];		// �������� �ƴ� ����Ʈ�� �� �������� ����Ʈ ��ȣ

	BOOL						m_bStartMessage;				// ��ȭ�� ���� �ߴٴ� �÷��� : �������� ��ȭ�� �����Ŀ� ���̱� ���ؼ� 
	BOOL						m_bStartQuestTimer;				// ����Ʈ Ÿ�̸� üũ ������ �˼� �ִ� �÷���
	BOOL						m_bStartQuestCounterMessage;	// ����Ʈ ī���� �޼��� ��� ������ �˼� �ִ� �÷���
	BOOL						m_bQuestStartConditionCheck;
	BOOL						m_bQuestValidCheck;				// ����Ʈ�� ��ȿ���� �ʾƼ� ���� ó���� ���� �� �� �ִ� ����
	BOOL						m_bProgressMainQuest;			// ������ ����Ʈ�� Process�� �����Ѵ�.
	BOOL						m_bReceiveQuestData;			// �������� ����Ʈ ����Ÿ�� ���� �޾Ҵ����� �� �� �ִ� ����
	BOOL						m_bCancelProcess;				// ����Ʈ ��� �Ŀ� ����Ʈ ����Ʈ�� �ٽ� �������� �� ����Ʈ ������ ���� ����
	
	_XQUEST_VIRTUALMODE			m_VirtualMode;					// ������������� �����Կ� ���õ� ��带 �����Ѵ�.

	_XCAMERA_ORIGINALINFO		m_OriginalCameraInfo;
		
	// ���� ���ǿ� ���� ����Ʈ ���̵� ���� ������ �˻��Ѵ�. 
	list <int>					m_listQuestStartTypeNPCTargetting;
	list <int>					m_listQuestStartTypeObjectClick;
	list <int>					m_listrQuestStartTypeUsingItem;
	list <int>					m_listQuestStartTypeWearItem;
	list <int>					m_listQuestStartTypeDestroyItem;
	list <int>					m_listQuestStartTypeLevel;
	list <int>					m_listQuestStartTypeZone;
	
	list <int>					m_listQuestNoSaveType;		// �������� ���� �����һ�� �����һ��� ����Ʈ ��ȣ�� �����ϴ� ����Ʈ 

	list <int>					m_listCheckInParty;		// ���� ���ǿ� ���� ���� �˻簡 ������ ���� Ż��ÿ� �ٽ� �˻��ϱ�
	list <_XQUEST_ITEMINFO>		m_listCheckItem;		// ���� ���ǿ� ������ �˻簡 ������ �������� ������ �� �ٽ� �˻��ϱ� ���� ����
	list <_XQUEST_ITEMINFO>		m_listCheckWearItem;	// ���� ���ǿ� ������ ���� �˻簡 ������ �������� ������ �� �ٽ� �˻��ϱ� ���� ����	
	list <_XQUEST_LEVELINFO>	m_listCheckLevel;		// ���� ���ǿ� ���� �˻簡 ������ ���� ��½ÿ� �ٽ� �˻��ϱ� ���� ���� ( �̸��϶��� ��� üũ�ؾ� �Ѵ�.)
	list <_XQUEST_ITEMINFO>		m_listCheckDestroyItem;	// ������ �ı� �˻�
	list <_XQUEST_HOLDITEM>		m_listHoldItem;			// ������ ������ ���� ����ü

	list <_XQUEST_MARKINFO>		m_listQuestMark;		// ����Ʈ ��ũ�� Ÿ���� �����϶� ���� ��ǥ���� ���� 
	list <_XQUEST_MARKINFO>		m_listHintMark;			// ��Ʈ ��ũ�� Ÿ���� �����϶� ���� ��ǥ���� ���� 	
	
	list <_XQUESTMAIL_INFO>		m_listQuestMailInfo;	// ǳ��� ��� üũ 	
	list <_XQUESTMAIL_INFO>		m_listQuestMailBuffer;	// ǳ��� ������ ���� ���ǿ��� �ź��Ҷ� 30���Ŀ� �ٽ� ������ ���� �ð��� ����	

	list <_XQUEST_QUESTLIST>	m_listNPCTargettingQuest;	// �� NPC�� ���� ����Ʈ�� Ÿ������ �ɷ������� ����Ʈ ��ȣ�� �����ϴ� ����Ʈ	

	/// Quest script list
	map <int, _XQuestScriptItem*>		m_mapQuestScriptList; // ������ ����Ʈ
	map <int, _XQuestScriptItem*>		m_mapSubQuestScriptList;// ������ �̿��� ����Ʈ

	map <int, _XQUEST_COMPLETE_RESULT>	m_mapCompleteQuestList;/// ����Ʈ �Ϸ� ����Ʈ <Quest id, Quest ���>
	map <int, _XQUEST_INVENFULL>		m_mapRewordQuestList;/// ������ ���� ���� ����Ʈ ����Ʈ
	  	
public:
	_XQuestScriptManager();
	~_XQuestScriptManager();

	// Release
	BOOL			Release(void);
	void			ReleasePrivateData(void);

	// Quest script load
	BOOL			LoadQuestScript(LPCTSTR filename);
	BOOL			LoadQuestScript(FILE* fileptr);

	// Main Function
	void			Process(void);

	// ���� ����Ʈ ���� Utility
	DWORD			GetCompareValue(int comparecount);
	int				GetCompareCount(DWORD comparevalue);

	// ���� �������� ����Ʈ ������ ���� ���� ���� �������� ������ üũ
	BOOL			CheckProgressQuestCount(void);
	// m_nProgressQuestCount ����
	void			IncreaseProgressQuestCount(void);
	// m_nProgressQuestCount ����
	void			DecreaseProgressQuestCount(void);
	// m_nProgressQuestCount return
	int				GetProgressQuestCount(void);

	// �Ϸ� ����Ʈ�� ����
	void			InsertQuestCompleteList(_XQuestScriptItem* currentquest);
	void			InsertQuestCompleteList(int questid, _XQUEST_COMPLETE_RESULT result);
	// �Ϸ� ����Ʈ���� ����Ʈ ��� �� �˾Ƴ���
	_XQUEST_COMPLETE_RESULT		FindQuestCompleteResult(int questid);

	// ����Ʈ�� �Ϸ� �Ǿ����� ���� �Ǻ�
	BOOL			CheckCompleteQuest(int questid);
	TCHAR*			GetQuestName(int questid);

	// ī�޶� ��� setting
	BOOL			GetQuestCameraMode(void);
	void			SetQuestCameraMode(BOOL mode);

	// ����Ʈ ��ü Reset
	void			ResetQuest(void);
	// Ư�� ����Ʈ Reset
	void			ResetQuest(int questid);
	// Ư�� Ʈ���� Reset
	void			ResetQuest(int questid, int triggerid );

	// �������� ���� ������ ����/�Ϸ� ����Ʈ �����
	void			QuestSuccessSetting(int questid); 
	void			QuestFailSetting(int questid);
	void			QuestRunningSetting(int questid, BYTE treestep);
	void			QuestRunningTimerSetting(int questid, int time, int treenumber, int triggertimer);
	BOOL			QuestCancleHistorySetting(int questid );
	BOOL			QuestHistorySetting(int questid, BYTE treestep);

	// ������ ���� ���� ����Ʈ ����Ʈ�� ����	
	void			InsertRewordQuestList(int questid, int treenumber, int needinven);
	// ����
	void			DeleteRewordQuestList(int questid);

	void			SetCheckQuestID(int questid);
	int				GetCheckQuestID(void);

	void			SetCheckQuest(BOOL check);
	BOOL			GetCheckQuest(void);

	void			ClearOriginalCameraInfo(void);

//	void			ProcessTest(int questid, int triggerid, int actionid);
//	void			RestoreTest(void);


	// Quest Wide view
	void			SetQuestWideView(BOOL bWide);
	BOOL			GetQuestWideView(void);

	// ��� ó��
	void			SetStartMessage(int questid);
	void			SetProgressMessage(int questid, int npcid, LPCTSTR string);
	void			SetFailMessage(int questid);

	BOOL			LoadString(FILE* fileptr, LPTSTR& string);
	
	// ����Ʈ ���� ���Ǻ��� �˻��ϴ� �Լ� 
	void			QuestStartConditionNPCTargettingMode();
	void			QuestStartConditionObjectClickMode();
	void			QuestStartConditionUsingItemMode();
	void			QuestStartConditionWearItemMode();
	void			QuestStartConditionDestroyItemMode();
	void			QuestStartConditionLevelMode();
	void			QuestStartConditionZoneMode();

	void			QuestStartConditionSort();
	void			SetQuestStartCondition( int questid );
	void			SetMainQuestStartCondition( int questid );
	
	BOOL			CheckItemCounter(int questid, int itemtype, int itemid);		// Ư�� ����Ʈ�� ������ ī���͸� ����ϴ� �Լ�
	BOOL			CheckPartyItemCounter(int questid, int itemtype, int itemid);	// Ư�� ����Ʈ�� ��Ƽ ������ ī���͸� ����ϴ� �Լ�
	
	BOOL			CheckAllItemCounter(int itemtype, int itemid);			// ���� ���� ��� ����Ʈ�� ������ ī���͸� ����Ѵ�.	

	void			ResetQuestStartCondition( void );
	void			SetQuestStartConditionAtVirtualSpace( void );			// ��������� �� �� ����Ʈ �������ǿ� ���� ó���� �ٽ��Ѵ�.

	// ���� ���ǿ� ������ �˻簡 ������ �������� ������ �� �ٽ� �˻��ϱ� ���� �Լ� 
	void			RecheckHaveItemTypeQuestStartCondition( char itemtype, short itemid ); 
	// ���� ���ǿ� ������ ���� �˻簡 ������ �������� ������ �� �ٽ� �˻��ϱ� ���� �Լ� 
	void			RecheckWearItemTypeQuestStartCondition( char itemtype, short itemid );
	// ���� ���ǿ� ���� �˻簡 ������ ���� ��½ÿ� �ٽ� �˻��ϱ� ���� ���� ( �̸��϶��� ��� üũ�ؾ� �Ѵ�.)
	void			RecheckLevelTypeQuestStartCondition( void );
	// ���� ���ǿ� ���డ�� �˻簡 ������ ���� Ż�� ���� �ٽ� �˻��ϱ� ���� ���� 
	void			RecheckPartyTypeQuestStartCondition( void );

	// ����Ʈ�� ���۵Ǹ� ������ ����Ʈ�� ���õ� ����Ʈ�� �����.
	void			DeleteRecheckingListAboutQuestStartCondition( int questid );
	
	int				CheckPracticableQuestNode( int questid, BYTE treestep, BOOL& bneeditemremove ); //���డ���� ��带 ��ȯ�Ѵ�.
	BYTE			GetQuestCurrentTreeNodeClass( int questid, BYTE treestep ); //���� ��尡 �б����� ������ ���������� �˾Ƴ���.
	BOOL			CheckHaveQuestJumpAction( int questid, BYTE treestep ); //���� ��忡 Goto�� �ִ��� üũ�Ѵ�.
	BOOL			CheckHaveQuestAutoNode( int questid ); //���� ��忡 Goto�� �ִ��� üũ�Ѵ�.
	
	// ��Ƽ ����Ʈ�� ��ҵǸ� ��Ƽ����Ʈ�� ����� �ٽ� ����Ʈ �������ǿ� �ִ´�.
	void			DeletePartyQuest( void );
};

#endif
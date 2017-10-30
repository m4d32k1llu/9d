// XWindow_NPCScript.h: interface for the XWindow_NPCScript class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_NPCSCRIPT_H_
#define _XWINDOW_NPCSCRIPT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "XKernel.h"
#include "XNPC.h"
#include "XWindow_WorldMinimap.h"
#include "XQuestScriptBaseType.h"

using namespace std;

typedef struct S_SUBSCRIPTITEM
{	
	int		index;
	BYTE	scriptclass;
	int		x;
	int		y;
	int		cx;
	int		cy;
}S_SUBSCRIPTITEM;

//2004.07.01->oneway48 insert
typedef enum _XNPC_Mode
{
	_XNPC_NONE = 0,
	_XNPC_ITEMTRADE,
	_XNPC_WAREHOUSE,
	_XNPC_CONTRACTWAREHOUSE,
	_XNPC_LEARNSKILL,
	_XNPC_REPAIRITEM,
	_XNPC_CONTRIBUTION,
	_XNPC_CREATEORGANIZATION,
	_XNPC_PVPBROKER,
	_XNPC_SOCKETPLUGINEX,
	_XNPC_COLLECTION,
	_XNPC_RESOURCE,
	_XNPC_INCHANT,
	_XNPC_BUFF,
	_XNPC_RANKQUEST,
	_XNPC_MOVEVILLAGE,
	_XNPC_CASTLEINFO,
	_XNPC_CHALLENGEINFO,
	_XNPC_MIXWEAPON,
	_XNPC_FREEMATCHINFO,
	_XNPC_MASTERINFO, //Author : ����� //breif : NPC ���� â
}_XNPC_MODE;

class _XWindow_NPCScript : public _XWindow  
{
public:	
	_XImageStatic*			m_NPCDialogBorder;
	_XImageStatic*			m_NPCDialogSubBorderA1;
	_XImageStatic*			m_NPCDialogSubBorderA2;
	_XImageStatic*			m_NPCDialogSubBorderB1;
	_XImageStatic*			m_NPCDialogSubBorderB2;
	_XImageStatic*			m_NPCDialogMainTitleBorder;
	_XImageStatic*			m_NPCDialogIMEBorder1;
	_XImageStatic*			m_NPCDialogIMEBorder2;
	_XImageStatic*			m_OrganizationNameIMEBorder1;
	_XImageStatic*			m_OrganizationNameIMEBorder2;
	_XImageStatic*			m_MiniStopEventIMEBorder1;
	_XImageStatic*			m_MiniStopEventIMEBorder2;
	_XImageStatic*			m_EventIMEBorder1;					
	_XImageStatic*			m_EventIMEBorder2;
	_XImageStatic*			m_imageDialogNextPageLeftBorder;
	_XImageStatic*			m_imageDialogNextPageRightBorder;
	_XImageStatic*			m_imageDialogNextPagePoint[2];
	_XImageStatic*			m_imageEventContent01;				// �̺�Ʈ ������ ��Ÿ���� �̹���(����ǹ����̺�Ʈ)
	_XImageStatic*			m_imageEventContent02;				// �̺�Ʈ ������ ��Ÿ���� �̹���(�ѹи���Ʈ�̺�Ʈ)
	_XImageStatic*			m_imageEventContent03;				// �̺�Ʈ ������ ��Ÿ���� �̹���(���డ���̺�Ʈ)
	_XImageStatic*			m_imageEventContent04;				// �̺�Ʈ ������ ��Ÿ���� �̹���(���ȭ�̺�Ʈ)
	_XImageStatic*			m_imageQuesLevelDisplay;	// ����Ʈ ���̵� ǥ�� �̹���
	_XImageStatic*			m_imageSavePoint[2];		// ���� ��ġ�� ��Ÿ���� �̹��� 
	
	// ����Ʈ ���� 
	_XButton*				m_btnQuestOK;				// ����Ʈ ���� Ȯ�� ��ư 
	_XButton*				m_btnBackToNPCMode;			// ����Ʈ ��ȭ ���� �Ŀ�  NPC ������� ���ư��� ��ư  
	_XButton*				m_btnDialogNextPage;		// NPC��簡 ������ ���� �������� �ѱ�� ��ư 
	_XButton*				m_btnNPCDialogModExit;		// ��ȭ ���� ��ư - NPC ��ȭâ�� ������.
	_XButton*				m_ScriptButton[4];
	_XButton*				m_CompleteNameInput;
	_XButton*				m_NampInputCancle;
	_XButton*				m_btnMiniStopEventInput;
	
	_XListBox*				m_listboxNPCDialogMessage;	// NPC ��ȭ�� ǥ���ϴ� ����Ʈ �ڽ� 
	_XListBox*				m_listboxQuestReward;		// ����Ʈ�� ���õǾ ������ �� ������ ǥ���ϴ� ����Ʈ �ڽ�
	_XListBox*				m_listboxSelectionItem;		// ����Ʈ���� �������� ǥ���� ����Ʈ �ڽ�
	_XListBox*				m_listboxHaveQuestList;		// ������ �ִ� ����Ʈ ����Ʈ�� ǥ���� ����Ʈ �ڽ�
	
	_XWindow_WorldMinimap*	m_pMiniMapWindowPtr;		// ����Ǯ�⿡�� ������ �׸��� ���ؼ� �̴ϸ� �������� ����Ʈ�� ���´�.
	
	LPTSTR					m_pDialogStringTotalBuffer;
	LPSTR					m_lpszNPCDialogMessage;
	
	TCHAR					m_pTitleString[256];
	TCHAR					m_HaveQuestName[256];		// ����Ʈ �̸�
	TCHAR					m_HaveQuestType[128];		// ����Ʈ Ÿ��
	TCHAR					m_QuestString[128];			// �Է�â�� �Էµ� �ؽ�Ʈ�� ����
	TCHAR					m_EventCodeString01[6];		// �ű� �̺�Ʈ �Է�â 3���� ����
	TCHAR					m_EventCodeString02[6];		// �ű� �̺�Ʈ �Է�â 3���� ����
	TCHAR					m_EventCodeString03[6];		// �ű� �̺�Ʈ �Է�â 3���� ����
			
	DWORD					m_ChatDisplayTimer;//2004.07.08->oneway48 insert
	DWORD					m_NPCDialogPositionTimeConter;
	DWORD					m_PrevDrawTime;
	
	FLOAT					m_SavePointPositionX;
	FLOAT					m_SavePointPositionY;
	FLOAT					m_EffectFrame;

	int						m_NPCDialogCount;// 2004.06.20->oneway48 close3-2
	int						m_DrawLineCount;//2004.07.08->oneway48 insert
	int						m_NPCDialogPositionCounter;
	int						m_NPCDialogLineCounter;
	int						m_HaveQuestID;				// ������ ǥ���� ����Ʈ�� ���̵� 
	int						m_pDialogStringTotalBufferLength;
	int						m_EventType;

	BOOL					m_bFirstDialog;// 2004.04.27->oneway48 insert
	BOOL					m_bNewChatDisplay;
	BOOL					m_bProgressNPCDialogMessage;
	BOOL					m_bPageClickQuestDialog;
	BOOL					m_bQuestStringInputComplete;// �Է�â���� Enter�� �������� 
	
	_XNPC_MODE				m_NPCMode; 
	_XQUEST_TYPE			m_QuestType;// ����Ʈ Ÿ�� ( ������, ������, ������, �⿬��, �����һ� )
	_XNPCMESSAGETYPE		m_MessageType;

	list <S_SUBSCRIPTITEM>	m_SubScriptList;
	

	_XStringSplitInfo		m_NPCDialogSplitinfo;

	_XIMEContainer			m_QuestIMEControl;			// ����Ʈ���� �ؽ�Ʈ �񱳽ÿ� ���̴� �Է�â
	_XIMEContainer			m_OrganizationNameIME;
	_XIMEContainer			m_MiniStopEventIMEControl;

public:
	_XWindow_NPCScript();
	virtual ~_XWindow_NPCScript();
	
	BOOL					Initialize( void );
	void					DestroyWindow( void );
	void					Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL					Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL					ProcessWheelMessage( short zDelta );
	void					ProcessNPCDialog(void);
	void					ProcessNPCDialogAtOnce(void);
	
	void					SetTitle( LPSTR titlestring );
	void					SetMessage( LPSTR messagestring );
	
	void					SetSubScriptList(int index, BYTE scriptclass);
	void					ClearSubScriptList(void);
	void					GenerateSubScriptList(void);
	
	void					ShowWindow(BOOL show);	
	void					SetDefaultPosition( void );
	
	BOOL					GetShowWindow(void);
	
	void					ChangeButton(void);
	
	void					InsertNPCChatString( LPSTR message, int messagemode,  BOOL mainchatwrite = TRUE );// 2004.04.28->oneway48 insert
	
	void					SetHaveQuestID(int questid);
	int						GetHaveQuestID(void)		{	return m_HaveQuestID;	};
	
	void					SetQuestListItem(void);
	void					SetMessageType(_XNPCMESSAGETYPE messagetype);

	BOOL					CheckQuestStringEditBoxArea(void);
	BOOL					CheckOrganizationEditBoxArea(void);
	BOOL					CheckEventEditBoxArea(void);			// �̺�Ʈ�� Ű ���� �Է��ϴ� �ڽ��� ���� üũ 
	BOOL					CheckEvent01EditBoxArea(void);			// ���డ�� �̺�Ʈ �� Ű���� �Է��ϴ� ������ �ڽ� �� �ϳ�
	BOOL					CheckEvent02EditBoxArea(void);			// ���డ�� �̺�Ʈ �� Ű���� �Է��ϴ� ������ �ڽ� �� �ϳ�
	BOOL					CheckEvent03EditBoxArea(void);			// ���డ�� �̺�Ʈ �� Ű���� �Է��ϴ� ������ �ڽ� �� �ϳ�
		
	void					SetMiniMapWindow(_XWindow_WorldMinimap* minimapwindow);	
	void					SetSavePointPosition( int npcid);
	
	// ���� ���� �Լ��� ----------------------------------=	
	void					ClickOrganizationBtn(void);
	// ---------------------------------------------------=
	
};

#endif // !defined(AFX_XWINDOW_NPCSCRIPT_H__C651EDD5_1E98_478A_85B7_902BDB82B0F4__INCLUDED_)

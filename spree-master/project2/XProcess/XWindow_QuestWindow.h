// XWindow_QuestWindow.h: interface for the _XWindow_QuestWindow class.
//
// Modified Date	: 2003.02.28
// Author			: Sohyun, Park
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_QUESTWINDOW_H__A5C9237C_4E1B_4210_8F97_F81E8D3C9148__INCLUDED_)
#define AFX_XWINDOW_QUESTWINDOW_H__A5C9237C_4E1B_4210_8F97_F81E8D3C9148__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "XWindow.h"

typedef enum
{
	_XQUESTTAB_QUEST1		= 0,
		_XQUESTTAB_QUEST2,
		_XQUESTTAB_QUEST3,
		_XQUESTTAB_QUEST4,
		_XQUESTTAB_QUEST5,
		_XQUESTTAB_TOTALCOUNT
}_XQuestWindowTab;

class _XWindow_QuestWindow : public _XWindow  
{
protected :
	
	// ����Ʈ ���� �ǹ�ư
	_XCheckButton*		m_QuestTab[ _XQUESTTAB_TOTALCOUNT ]; // ����Ʈ ���� : ������, ������, ������, �⿬�� , �����һ� 
	_XButton*			m_LeftButton;
	_XButton*			m_RightButton;
	
	// ����Ʈ �̸� ���� �̹��� 
	_XImageStatic*		m_QuestNameBorder1;
	_XImageStatic*		m_QuestNameBorder2;

	// ����Ʈ ���� ���� �̹��� 
	_XImageStatic*		m_QuestInfomationBorder1;
	_XImageStatic*		m_QuestInfomationBorder2;
	_XImageStatic*		m_QuestInfomationBorder3;

	// ����Ʈ �Ϸ� ���� �̹��� 
	_XImageStatic*		m_QuestCompleteIconImage;
		
	//���õ� ����Ʈ �̸� 
	TCHAR				m_SelectedQuestName[512];			// ���õ� ����Ʈ�� �̸�
	TCHAR				m_SelectedQuestSubName[512];		// ����Ʈ ������
	TCHAR				m_TempSelectedQuestSubName[512];	// ����Ʈ ������ �ӽ� �������
	
	// ����Ʈ ������ ���������� ����
	LPTSTR				m_lpszQuestProcessInfomation;
	LPTSTR				m_lpszQuestOutline;
	
	int					m_indexProgressQuestGroup;
	int					m_indexCompleteQuestGroup;
	BOOL				m_bSelectedProgressQuestGroup;
	BOOL				m_bSelectedCompleteQuestGroup;
	BOOL				m_bOverFlowQuestSubName;
	
public :
	
	// ����Ʈ ��� ��ư 
	_XButton*			m_QuestCancelButton;

	_XQuestWindowTab	m_SelectedTab;
	BOOL				m_ChangeTab;
	int					m_SelectedQuestID;
	int					m_SelectedItemIndex;
	int					m_PrevSelectedItemIndex;
	
	int					m_TotalCountQuestListIndex;
	int					m_GetTotalQuestCount; // ����Ʈ ���� ���� �� � �ΰ� (������ ����) //Author : �����
	
	_XListBox*			m_listboxQuestScriptItem;			// ����Ʈ �̸��� ǥ���ϴ� ����Ʈ �ڽ�
	_XListBox*			m_listboxQuestDesc;					// ����Ʈ�� ���� ������ ǥ���ϴ� ����Ʈ �ڽ� 
	_XListBox*			m_listboxQuestReward;				// ����Ʈ ������ ǥ���ϴ� ����Ʈ �ڽ�
			
	_XList				m_QuestGroupList;
	int					m_QuestGroupNumber; // ������ �������� ǥ�� 

public:
	_XWindow_QuestWindow();
	virtual ~_XWindow_QuestWindow();
	
	BOOL		Initialize( void );
	void		DestroyWindow(void);
	
	void		Draw(_XGUIObject*& pforcusobject = g_CurrentFocusedObject);
	BOOL		Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL		ProcessWheelMessage( short zDelta );
	void		ProcessSelectedListBox();
	void		ProcessSelectedTabButton();
	
	void		MoveWindow(int X, int Y);

	BOOL		CheckMousePositionQuestDescriptionBox(void);
	void		SetListItem(void);
	void		ParseQuestDescString(void);
	
	void		ShowWindow(BOOL show);
	void		SetDefaultPosition( void );
	void		ChangeTab(_XQuestWindowTab selectedtab);
	void		SetParameter(void);
	
	void		SetSelectedQuestID(int id)	{	m_SelectedQuestID = id;		}
	int			GetSelectedQuestID(void)	{	return m_SelectedQuestID;	}
	
	void		SetSelectedItemIndex(int index)		{	m_SelectedItemIndex = index;	}
	int			GetSelectedItemIndex(void)			{	return m_SelectedItemIndex;		}
	
	void		SetQuestGroupListItem(int mainquestid);
	int			GetQuestGroupNumber( int mainquestid);
	int			GetQuestGroupNumber1(int mainquestid);
	int			GetQuestGroupNumber2(int mainquestid);
	int			GetQuestGroupNumber3(int mainquestid);
	int			GetQuestGroupNumber4(int mainquestid);
	int			GetQuestGroupNumber5(int mainquestid);
	int			GetQuestGroupNumber6(int mainquestid);		
	
	void		SetQuestTabButton(void);
	void		SetTempQuestSubName( TCHAR* comsubname, TCHAR* srcsubname );	///< �� ����Ʈ ������ ���
};

#endif // !defined(AFX_XWINDOW_QUESTWINDOW_H__A5C9237C_4E1B_4210_8F97_F81E8D3C9148__INCLUDED_)
// XWindow_QuestMail.h: interface for the _XWindow_QuestMail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_QUESTMAIL_H__268291A2_5F36_4493_B362_F27C3E5BC5EB__INCLUDED_)
#define AFX_XWINDOW_QUESTMAIL_H__268291A2_5F36_4493_B362_F27C3E5BC5EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum QuestMailButton
{
	_XQUESTMAILBUTTON_OK	= 0,		
		_XQUESTMAILBUTTON_YES,			
		_XQUESTMAILBUTTON_NO,		
		_XQUESTMAILBUTTON_LISTBOXMESSAGE
}; 

typedef enum _XQuestMailMode
{
	_XQUESTMAILMODE_OK = 0,
	_XQUESTMAILMODE_YESNO,
	_XQUESTMAILMODE_TUTORIALONLY //Author : ����� //breif : ó�� ��ȣ ����� �� ���� �źθ� �ٲ۴�
		
}_XQUESTMAIL_MODE;

class _XWindow_QuestMail  : public _XWindow  
{
public:	
	int					m_SkinMode;				// 0:�Ϲ� ǳ��� 1:Ʃ�丮�� ǳ���
	int					m_QuestID;
	_XQUESTMAIL_MODE	m_QuestMailMode;
	BOOL				m_bStartQuest;

	_XListBox*			m_listboxQuestMessage;	// ǳ��� ������ ǥ���ϴ� ����Ʈ �ڽ� 

	_XButton*			m_btnQuestMailOK;		// ǳ��� Ȯ�� ��ư 
	_XButton*			m_btnQuestMailYES;		// ǳ��� ���� ��ư 
	_XButton*			m_btnQuestMailNO;		// ǳ��� �ź� ��ư 
	
public:
	_XWindow_QuestMail();
	virtual ~_XWindow_QuestMail();
	
	static _XWindow_QuestMail*	CreateQuestMail( _XWindowManager* pWindowManager, int questid, int skinmode = 0 );
	
	void			Create( _XWindowManager* pWindowManager, DWORD windowid, int skinmode = 0 );	
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			ProcessWheelMessage( short zDelta );
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
		
	void			SetMode( _XQUESTMAIL_MODE questmailmode );
	void			SetMessage( LPSTR messagestring );

};

#endif // !defined(AFX_XWINDOW_QUESTMAIL_H__268291A2_5F36_4493_B362_F27C3E5BC5EB__INCLUDED_)

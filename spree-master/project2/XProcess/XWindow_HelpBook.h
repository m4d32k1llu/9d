// XWindow_HelpBook.h: interface for the _XWindow_HelpBook class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_HELPBOOK_H__7119028D_8A33_47E7_B2FD_2DDFA70D7B7E__INCLUDED_)
#define AFX_XWINDOW_HELPBOOK_H__7119028D_8A33_47E7_B2FD_2DDFA70D7B7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief ������
 * f1 Ű�� ���� ��� �ߴ� â 
 * 
 * ���ӻ� �ʿ��� ��� ������� ����� �ִ�.
 * htmp �� �ۼ��Ǹ� helpsystem ���� ���� ����.
 * helpbook.txt �� Ʈ���������� �����ȴ�. 
*/

class _XWindow_HelpBook : public _XWindow  
{
public:
	_XWindow_HelpBook();
	virtual ~_XWindow_HelpBook();

	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				ProcessWheelMessage( short zDelta );

	void				MoveWindow( int X, int Y );

	BOOL				LoadHelpBookData( void );

	void				SetHelpHtml( int helpid );
	void				PrevPage();
	void				NextPage();
	void				ShowWindow(BOOL show);
public:	
	
	_XTreeBox*			m_pTreeBox;

	_XVScrollBar		m_HTMLListScrollBar;
	_XGUIObject*		m_pScrbarFocusedObject;

	_XImageStatic		m_HTMLViewStatic;
	int					m_HTMLListCount;
	TCHAR**				m_ppHTMLNameList;
};

#endif // !defined(AFX_XWINDOW_HELPBOOK_H__7119028D_8A33_47E7_B2FD_2DDFA70D7B7E__INCLUDED_)

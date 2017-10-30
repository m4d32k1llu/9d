// XWindow_Email.h: interface for the _XWindow_Email class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_EMAIL_H__46FF769C_400F_4CD5_A6A8_C5B2DDDD621A__INCLUDED_)
#define AFX_XWINDOW_EMAIL_H__46FF769C_400F_4CD5_A6A8_C5B2DDDD621A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum
{
	_XEMAILTAB_RECEIVE		= 0,
		_XEMAILTAB_SEND,
		_XEMAILTAB_KEEP
		
}_XEmailTab;

typedef struct {
	UINT			uiIndex ;			// ���� �ε���
	bool			check ;				// false : ���� , true : ���ε���(������)	
}_XEMAILLISTTABLE;


struct _XEMAILINFO {
	UINT			uiIndex ;			// ���� �ε���
	char			cFrom[13] ;			// �������
	struct _Date {						// �����ð�
		unsigned	year : 12 ;				
		unsigned	month : 4 ;
		unsigned	day : 5 ;
		unsigned	hour : 5 ;
		unsigned	minute : 6 ;
	} date;	// 4Byte
	char			cStrLen ;
	char			cText[101] ;		// ����
	char			cMode ;
	enum
	{
		_PRIVATE,
		_PUBLIC
	};
};
#define _XDEF_MAX_EMAILLENGTH		100		// �ִ� ������ ����

class _XWindow_Email   : public _XWindow  
{
	
public:
	
	_XImageStatic*		m_imageEmailTitleBar;					// ������ Ÿ��Ʋ ��
	_XImageStatic*		m_imageEmailDisplayBoxUpBorder;			// ������ ����ǥ�� �ڽ� ���� ���� 
	_XImageStatic*		m_imageEmailDisplayBoxDownBorder;		// ������ ����ǥ�� �ڽ� �Ʒ��� ���� 
	_XImageStatic*		m_imageEmailDisplayBoxLeftSideBorder;	// ������ ���� ���̵� �̹���
	_XImageStatic*		m_imageEmailDisplayBoxRightSideBorder;	// ������ ������ ���̵� �̹���
	_XImageStatic*		m_imageEmailDownBorder;					// ������ â �Ʒ���  ����
	_XImageStatic*		m_imageEmailDockingBorder;				// ������ ���� �޴�â�� ��ŷ������ �����Ǵ� ����
	_XImageStatic*		m_imageEmailDownEndLine;				// ������ â �Ʒ��� �� ���� 
	_XImageStatic*		m_imageEmailCountBoxBorder;				// ������ ������ ���� ǥ�� â �ڽ�  


	// ������ ���� ����
	_XEmailTab			m_SelectedTab;
	_XCheckButton*		m_TabButtonKeepingBox;					// ������ �ǹ�ư ������
	_XCheckButton*		m_TabButtonReceiveBox;					// ������ �ǹ�ư ������
	_XCheckButton*		m_TabButtonSendBox;						// ������ �ǹ�ư �߽���
	
	_XButton*			m_btnEmailClose;						// ������ â �ݱ� ��ư
	_XButton*			m_btnEmailHelp;							// ������ â ���� ��ư
	_XButton*			m_btnNext;								// ����� ���� ���� ���� ��ư 
	_XButton*			m_btnPrev;								// ����� ���� ���� ���� ��ư
	_XButton*			m_btnKeepingMailDelete;					// ����� ���� �����ϱ� ��ư
	_XButton*			m_btnReceiveReply;						// ���� ���� �����ϱ� ��ư
	_XButton*			m_btnReceiveSave;						// ���� ���� �����ϱ� ��ư
	_XButton*			m_btnReceiveDelete;						// ���� ���� �����ϱ� ��ư
	_XButton*			m_btnSendDelete;						// ���� ���� ���� ����� ��ư
	_XButton*			m_btnSendCancle;						// ���� ���� ��� ��ư
	_XButton*			m_btnSend;								// ���� ���� ������ ��ư
	
	
	_XIMEContainer		m_EmailIMEControl;
	TCHAR				m_EmailDisplayText[256][128];
	TCHAR				m_EmailText[101];
	int					m_EmailTextLineTotalCount;
	int					m_EmailTextCurrentLinePos;
	int					m_EmailTextLength;
	
	_XIMEContainer		m_ReceiverIMEControl;
	TCHAR				m_EmailReceiver[13];
	TCHAR				m_EmailSender[20];
	
	int					m_EmailTextScrollPos;
	_XVScrollBar*		m_EmailTextScrollBar;	
	_XGUIObject*		m_pScrbarFocusedObject;	
	
	_XEMAILINFO			m_TempEmailInfo[20];
	_XEMAILLISTTABLE	m_EmailListReceiveBox[20];
	_XEMAILLISTTABLE	m_EmailList[20];
	int					m_EmailTextTotalCount;
	int					m_indexEmailText;
	int					m_EmailReceiveTextTotalCount;
	int					m_indexEmailReceiveText;
	int					m_EmailYear;				
	int					m_EmailMonth;
	int					m_EmailDay;
	int					m_EmailHour;
	int					m_EmailMinute;
	unsigned long		m_EmailColor;

	BOOL				m_bDockingMode;
	

public:
	_XWindow_Email();
	virtual ~_XWindow_Email();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			ProcessWheelMessage( short zDelta );
	void			ProcessDrag( _XGUIObject*& pfocusobject );
	
	void			ResizeWindow( RECT winrect ){ ResizeWindow(winrect.left, winrect.top, winrect.right, winrect.bottom ); }
	void			ResizeWindow( int left, int top, int right, int bottom );

	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);

	
	void			ResetEmailList();
	void			ResetEmailText();
	void			ResetTempEmailInfo();
	void			ResetEmailListReceiveBox();
	
	void			SetEmailText( LPSTR emailtext );

	void			DeleteEmailListReceiveBox(int emailindex);
	void			DeleteEmailList( int emailindex );
	
	void			CalculateEmailLength();
	
	void			ChangeTab(_XEmailTab selectedtab);
	
	BOOL			CheckIMEBoxArea();
	BOOL			CheckReceiverIMEBoxArea();
	BOOL			CheckEmailScrollBarArea();
	void			SetDockingMode(BOOL flag);


};

#endif // !defined(AFX_XWINDOW_EMAIL_H__46FF769C_400F_4CD5_A6A8_C5B2DDDD621A__INCLUDED_)

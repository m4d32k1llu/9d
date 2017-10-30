// XWindow_CashBox.h: interface for the _XWindow_CashBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CASHBOX_H__A29E1817_1391_4878_A1B0_A2C7868D617E__INCLUDED_)
#define AFX_XWINDOW_CASHBOX_H__A29E1817_1391_4878_A1B0_A2C7868D617E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "XWindow.h"

// ��� ������ ������ ������ ����
typedef struct 
{
	int		indexBox;		// ������ �ε��� 
	int		OrderNumber;	// �ֹ���ȣ
	int		ProductNumber;	// ��ǰ��ȣ
	int		indexPackage;	// ��Ű�� �ε���
	BOOL	bCheck;			// ���� ���� 
	
	char		PresentName[en_max_lil+1];	// ������ ������ Account
	SYSTEMTIME	PresentTime;				// ���� ���� ��¥(��/��/��/��/��/��); DBTIMESTAMP
	char		PresentMsg[100+1];			// ���� �޽���
}_XCASHITEMBOX_INFO;

typedef enum
{
	_XCASHBOXTAB_PERSONAL		= 0,
		_XCASHBOXTAB_PRESENT,
		_XCASHBOXTAB_TOTALCOUNT
}_XCashBoxTab;

class _XWindow_CashBox : public _XWindow  
{
public:
	_XCashBoxTab	m_SelectedTab;
	
	_XButton*		m_btnMoveSelectedItem;	//���ÿű�
	_XButton*		m_btnMoveAllItem;		//���οű�
	_XButton*		m_btnRenew;		//���οű�

	_XButton*		m_btnFirst;				//ó������ 
	_XButton*		m_btnEnd;				//������
	_XButton*		m_btnAdjacent;			//����
	_XButton*		m_btnNext;				//����
	_XButton*		m_btnMsgClose;			//�޼��� �ݱ�
	_XButton*		m_btnPage[5];			//������ ���� //Author : �����
	_XButton*		m_btnPage1;				//������1
	_XButton*		m_btnPage2;				//������2
	_XButton*		m_btnPage3;				//������3
	_XButton*		m_btnPage4;				//������4
	_XButton*		m_btnPage5;				//������5
	
	_XButton*		m_btnMessage[9];		//�޼������� ��ư1
	
	_XCheckButton*	m_TabButton[ _XCASHBOXTAB_TOTALCOUNT ]; // ���� ������ , ������ 
	_XCheckButton*	m_Common_TabButton[ _XCASHBOXTAB_TOTALCOUNT ]; // ���� ������ , ������ 
	_XListBox*		m_listboxCashItem;	// ������ ������ ������ ĳ�� ������
	
	int				m_nCashItemTotalCount;
	int				m_PresentMessageBoxPosition;
	int				m_SendOrderNumberPresentMessage;
	int				m_Maxium5PageCount;
	int				m_Current5PageCount;
	int				m_CurrentPageNumber;
	BOOL			m_bShowPresentMessage;
	
	TCHAR			m_PresentName[128];
	TCHAR			m_PresentTime[128];
	TCHAR			m_PresentMessage[128];

	list <_XCASHITEMBOX_INFO*>		m_listCashItem;		// ��� ������ ���� ����Ʈ 

public:
	_XWindow_CashBox();
	virtual ~_XWindow_CashBox();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	
	void			SetListItem(void);
	void			GetIconRect(char cType, int iconnumber, int& imageindex, RECT& rect, BOOL smallicon);
	void			ChangeTab(_XCashBoxTab selectedtab);
	
	void			SetPageNumberButton( void );
	void			SetPresentMessage( int OrderNumber );
	void			SendPresentMessageReq( int MsgNumber );
};

#endif // !defined(AFX_XWINDOW_CASHBOX_H__A29E1817_1391_4878_A1B0_A2C7868D617E__INCLUDED_)

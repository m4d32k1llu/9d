// XWindow_MessengerDialog.h: interface for the _XWindow_MessengerDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MESSENGERDIALOG_H__0CBB024E_B76C_40B7_B9B0_370CE33C0E80__INCLUDED_)
#define AFX_XWINDOW_MESSENGERDIALOG_H__0CBB024E_B76C_40B7_B9B0_370CE33C0E80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XWindow.h"
#include "XMnDStructInfo.h"

#define _XDEF_MESSENGERDIALOGWINDOW_WIDTH		250
#define _XDEF_MESSENGERDIALOGWINDOW_HEIGHT		278

#define _XDEF_INSTANCEMESSAGEWINDOW_WIDTH		183
#define _XDEF_INSTANCEMESSAGEWINDOW_HEIGHT		46

typedef enum MessengerDialogButton
{
	_XMESSENGERDIALOGBUTTON_HELP	= 0,		
		_XMESSENGERDIALOGBUTTON_MINIMIZE,		
		_XMESSENGERDIALOGBUTTON_CLOSE,			
		_XMESSENGERDIALOGBUTTON_INFORMATION,		
		_XMESSENGERDIALOGBUTTON_PARTYREQUEST,	
		_XMESSENGERDIALOGBUTTON_FRIENDCUT,		
		_XMESSENGERDIALOGBUTTON_CHATINPUT,		
		_XMESSENGERDIALOGBUTTON_EMOTICON
}; 

typedef enum _XMessengerDialogMode
{
	_XMESSENGERDIALOGMODE_INSTANCE		= 0,
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
	_XMESSENGERDIALOGMODE_DIALOG,
	_XMESSENGERDIALOGMODE_MNDINSTANCE, //���� �ּ�ȭ â
	_XMESSENGERDIALOGMODE_MASTERDIALOG, //������ ���� ��ȭâ
	_XMESSENGERDIALOGMODE_DISCIPLEDIALOG, //���ڰ� ���� â 
	_XMESSENGERDIALOGMODE_MNDGROUPDIALOG //���� �׷� ��ȭ
#else
	_XMESSENGERDIALOGMODE_DIALOG
#endif

}_XMESSENGERDIALOG_MODE;

class _XWindow_MessengerDialog   : public _XWindow  
{	
public:	
	_XImageStatic*		m_imageInstanceMessageTitleBar;			// �޽��� �ν���Ʈ �޼��� â Ÿ��Ʋ ��	
	_XImageStatic*		m_imageInstanceMessageEndLine;			// �޽��� �ν���Ʈ �޼��� â �� ����

	_XImageStatic*		m_imageMessengerDialogTitleBar;			// �޽��� ��ȭâ Ÿ��Ʋ ��	
	_XImageStatic*		m_imageMessengerDialogLeftSideBorder;	// �޽��� ��ȭâ ���� ���̵� �̹���
	_XImageStatic*		m_imageMessengerDialogRightSideBorder;	// �޽��� ��ȭâ ������ ���̵� �̹���
	_XImageStatic*		m_imageMessengerDialogDownBorder;		// �޽��� ��ȭâ â �Ʒ���  ����
	_XImageStatic*		m_imageMessengerDialogEmoticonBox;		// �޽��� ��ȭâ �̸�Ƽ�� �ڽ� 


	_XImageStatic*		m_imageMessengerDialogAddTitleBar;		// Author :����� //�޽��� ��ȭâ Ÿ��Ʋ �� �Ʒ� �κ� Ȯ��	
	_XImageStatic*		m_pImageMessengerDialogBorder_1;		// Author :����� //���� ���� ��ȭ�� �׵θ�
	_XImageStatic*		m_pImageMessengerDialogBorder_2;		// Author :����� //���� ���� ��ȭ�� �׵θ�
	_XImageStatic*		m_pImageMessengerDialogBorder_3;		// Author :����� //���� ���� ��ȭ�� �׵θ�
	_XImageStatic*		m_pImageMessengerDialogBorder_4;		// Author :����� //���� ���� ��ȭ�� �׵θ�

	_XButton*			m_pBtnMessengerMnDAccept;				// Author :����� //���� ���� ��ư
	_XButton*			m_pBtnMessengerMnDReject;				// Author :����� //���� ���� ��ư
	
	_XButton*			m_btnMessengerDialogHelp;				// �޽��� ��ȭâ ���� ��ư 
	_XButton*			m_btnMessengerDialogMinimize;			// �޽��� ��ȭâ ��� ��ư 
	_XButton*			m_btnMessengerDialogClose;				// �޽��� ��ȭâ �ݱ� ��ư 
	
	_XButton*			m_btnMessengerDialogInformation;		// �޽��� ��ȭâ ��� ���� ��ư	 
	_XButton*			m_btnMessengerDialogPartyRequest;		// �޽��� ��ȭâ �����û ��ư	
	_XButton*			m_btnMessengerDialogFriendCut;			// �޽��� ��ȭâ ���� ��ư 	
	_XButton*			m_btnMessengerDialogChatInput;			// �޽��� ��ȭâ �޼��� �Է� ��ư 	
	_XButton*			m_btnMessengerDialogEmoticonInput;		// �޽��� ��ȭâ �̸�Ƽ�� �Է� ��ư 
	
	_XIMEContainer		m_IMEControlMessengerDialog;			// �޽��� ��ȭâ �޼��� �Է�â 
	
	_XList				m_MessengerChatList;					// �޽��� ��ȭâ ���� 
	
	int					m_MessengerChatListScrollPos;			
	_XVScrollBar*		m_MessengeChatListScrollBar;			// �޽��� ��ȭâ ǥ��â ��ũ�ѹ� 
	_XGUIObject*		m_pScrbarFocusedObject;	
	
	TCHAR				m_ToCharactername[13];					// �޽������� ������ ģ�� �̸�
	
	_XMESSENGERDIALOG_MODE	m_MessengerDialogMode;				// �޽��� ��ȭâ ���( �ν��Ͻ� �޼���, ��ȭâ )

	BOOL				m_bReady;								// �ν��Ͻ� �޼��� â�� ���콺�� �÷��� ������ 
	BOOL				m_bMessageReceive;						// �ν��Ͻ� �޼��� ����϶� �޼����� �����ϸ�  

	int					m_InstanceMessagePositionNumber;		// �ν��Ͻ� �޼��� â�� ��ġ�� ��Ÿ���� ���� 
	
	BOOL				m_bGMChatting;							// GM�� ä������ �ƴ����� ������.

	DiscipleInfo		m_DiscipleInfo;							//Author : ����� //breif : ������ ���� ���� ����

public:
	_XWindow_MessengerDialog();
	virtual ~_XWindow_MessengerDialog();
	
	static _XWindow_MessengerDialog*	CreateMessengerDialogBox( _XWindowManager* pWindowManager );

	void			Create( _XWindowManager* pWindowManager, DWORD windowid );	
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			ProcessWheelMessage( short zDelta );
	void			ProcessDblClick();
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	
	BOOL			CheckIMEBoxArea(void);		

	void			SetMode( _XMESSENGERDIALOG_MODE messengerdialogmode );
	
	void			SetGMChatting(BOOL GMchat) { m_bGMChatting = GMchat; }
};

#endif // !defined(AFX_XWINDOW_MESSENGERDIALOG_H__0CBB024E_B76C_40B7_B9B0_370CE33C0E80__INCLUDED_)

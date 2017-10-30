// XWindow_Messenger.h: interface for the _XWindow_Messenger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MESSENGER_H__4DA2D6F1_1226_4E91_8514_F66BA44ACA8D__INCLUDED_)
#define AFX_XWINDOW_MESSENGER_H__4DA2D6F1_1226_4E91_8514_F66BA44ACA8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XList.h"
#include "XMessengerGroup.h"
#include "XMessengerFriend.h"


class _XWindow_Messenger : public _XWindow  
{
public:	
	_XImageStatic*		m_imageMessengerTitleBar;			// �޽��� Ÿ��Ʋ ��	
	_XImageStatic*		m_imageMessengerStateBorder;		// �޽��� ���� ��ȯ, �α��� �α׾ƿ� ����	
	_XImageStatic*		m_imageMessengerLeftSideBorder;		// �޽��� ���� ���̵� �̹���
	_XImageStatic*		m_imageMessengerRightSideBorder;	// �޽��� ������ ���̵� �̹���
	_XImageStatic*		m_imageMessengerDownBorder;			// �޽��� â �Ʒ���  ����
	_XImageStatic*		m_imageMessengerDockingBorder;		// �޽����� ���� �޴�â�� ��ŷ������ �����Ǵ� ����
	_XImageStatic*		m_imageMessengerDownEndLine;		// �޽��� â �Ʒ��� �� ���� 
	
	_XButton*			m_btnMessengerClose;				// �޽��� �ݱ� ��ư 
	_XButton*			m_btnMessengerHelp;					// �޽��� ���� ��ư 
	_XButton*			m_btnMessengerLogIn;				// �޽��� �α��� ��ư	 
	_XButton*			m_btnMessengerLogOut;				// �޽��� �α׾ƿ� ��ư	
	_XButton*			m_btnMessengerAddFriend;			// �޽��� ģ���߰� ��ư 	
	_XButton*			m_btnMessengerAddFriendGroup;		// �޽��� �׷��߰� ��ư 
	_XButton*			m_btnChangeStatus;					//�޽��� ���� ��ȯ ��ư 
	_XButton*			m_btnMessengerRecallSystem;			// �޽��� ��ȯ ��ư
	_XButton*			m_btnMessengerMnDSystem;			// �޽��� ������ȭ ��ư

	// �޽��� �巡�� ���� ���� 
	_XToolTip*			m_DragingDisplayBox;				// �޽��� ����Ʈ���� �巡�� �Ǵ� ģ�� �̸��̳� �׷� �̸��� ǥ����

	// �޽��� ģ�� ����Ʈ ���� ������ 
	_XListBox*			m_listboxGroupList ;				//�޽��� �׷�� ģ������Ʈ�� ǥ���ϴ� ����Ʈ �ڽ� 
	_XList				m_MessengerGroupList;
		
	int					m_IsDragingCharacterNumber;
	int					m_IsDragingGroupNumber;
	
	int					m_CurrentUserStatus;				// ģ���� ���¸� �� �� �ִ� ���� 
	int					m_TotalCountFriends;				// ģ�� ������ count : �ִ� 50�� 
	
	int					m_SelectedItemIndex;
	// 
	int					m_MaxViewCount;
	_XWINDOW_RESIZEMODE m_ResizeMode;

	BOOL				m_bDragingDisplayBox;
	BOOL				m_bDrawDragingDisplayBox;
	BOOL				m_Resizing;
	BOOL				m_bDockingMode;
	BOOL				m_bSelectedGroup;
	BOOL				m_bClickRightButton;
	BOOL				m_bClickLeftButton; 
	BOOL				m_bDblClick;						//����Ŭ���� �� ���¸� �˷��ش�.
	
	float				m_DragStartPosX;					// ���콺 �巡�׽�  �ٸ� �ʿ��� Ŭ���ؼ� 
	float				m_DragStartPosY;	

	float				m_ClickRightStartPosX;				// ���콺 ������ Ŭ����  �ٸ� �ʿ��� Ŭ���ؼ� 
	float				m_ClickRightStartPosY;				//�������� ���� ���� ������ ���� �ʰ� �ϰ� ���� ����

	TCHAR				m_IsDragingText[256];
	TCHAR				m_IsDragingCharacterName[13];		// �巡�׸� ������ ��ġ�� �ִ� ģ�� �̸�
	TCHAR				m_IsDragingGroupName[15];			// �巡�׸� ������ ��ġ�� �ִ� ģ���� �����ϰ� �ִ� �׷� �̸� 
	// �޽��� ģ���߰�, �׷��߰� ���� ���� 
	TCHAR				m_CharacterName[15];				// ģ�� �߰��� �޽������� ���õ� ģ���� �̸��� �����Ѵ�.
	TCHAR				m_GroupName[15];					// ���ο� �׷������ ���,�׷� �̸� ����� ����� ���� ������ �ٲٱ� �� �̸��� �����ϴ� ����
	TCHAR				m_GroupReName[15];					// �׷� �̸� ����� ����� ���� ������ �ٲ� �̸��� �����ϴ� ����
	
	TCHAR				m_SelectedCharactername[13];		//�޽������� ������ ģ�� �̸�
	TCHAR				m_SelectedGroupname[15];			//�޽������� ������ �׷� �̸�
	
	
	
	
public:

	_XWindow_Messenger();
	virtual ~_XWindow_Messenger();
	
	BOOL Initialize( void );
	void DestroyWindow( void );
	void MoveWindow( int X, int Y );	
	
	void SetDefaultPosition( void );
	void SaveWindowProperty( void );
	void ShowWindow( BOOL show ); 
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL ProcessWheelMessage( short zDelta );
	void ProcessResizeAndDrag( _XGUIObject*& pfocusobject );
	void ProcessSelectedListboxLeftButton();
	void ProcessSelectedListboxRightButton();	
	void ProcessMoveListbox();
	void ProcessSelectMovingGroup(); 
	void ProcessDblClick();
	
	void ReleaseMessengerList();
	
	void ResizeWindow( RECT winrect ){ ResizeWindow(winrect.left, winrect.top, winrect.right, winrect.bottom ); }
	void ResizeWindow( int left, int top, int right, int bottom );
	
	void SetListItem();
	int	 SearchListNumber();
	
	void SortGroupName();
	void SortFriendName();
	
	BOOL CheckGroupListbox();
	void SetDockingMode(BOOL flag);
};

#endif // !defined(AFX_XWINDOW_MESSENGER_H__4DA2D6F1_1226_4E91_8514_F66BA44ACA8D__INCLUDED_)


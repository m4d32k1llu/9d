// XWindow_RecallItem.h: interface for the XWindow_RecallItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_RECALLITEM_H__28870DE7_4EF7_4898_83AF_71E9DF529334__INCLUDED_)
#define AFX_XWINDOW_RECALLITEM_H__28870DE7_4EF7_4898_83AF_71E9DF529334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

class _XWindow_RecallItem : public _XWindow
{
public:
	_XWindow_RecallItem();
	virtual ~_XWindow_RecallItem();

	BOOL	Initialize( void );
	void	DestroyWindow( void );
	
	void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void	Update(); //������ʹ� ������ Process ����

	void	ShowWindow(BOOL show);
	void	MoveWindow( int X, int Y );

	BOOL	SearchSlotItem( int nIndex ); //Author : ����� //breif : ģ����ȯ ������ �ִ��� �˻�
	BOOL	CharRecallCondition();
	BOOL	CheckCharacterName( LPCTSTR pString );
	
	LPCTSTR	GetCharacterName( int nIndex ); //������ ��Ʈ���� ����..
	
	void	SetMovingVariable( MSG_RECALL_FRIEND* pRecallresult, BOOL bMoving, int nEffectID );
	void	SetCharState( int nState );
	void	SetSelectedItem(int nParam);
	void	SetRecallIndex(int nIndex );
	void	SetFromName( LPTSTR pString );
	void	SetRecallButton( BOOL bEnable );

	LPCTSTR		GetFromName();
	const int	GetRecallIndex() const;
	const int	GetCharState() const; //No�϶� ��Ŷ�������� ���..
	const int	GetItemIndex() const;
	const int	GetSelectedItem() const;

private :
	void SetListItem();

private :
	// �޽��� ģ�� ����Ʈ ���� ������ 
	_XListBox*			m_listboxGroupList ;				//�޽��� �׷�� ģ������Ʈ�� ǥ���ϴ� ����Ʈ �ڽ� 
	int m_nRecallItemIndex; //���ݾ����� �κ��丮 ��ġ
	int m_nSelectedItem; //���õ� ����Ʈ ��ȣ
	int m_nCallIndex; //���� ������ ����Ʈ 
	int m_nCharState; // 0 �϶� ����.. 1�϶� �ٸ������� ����.. //���� �Ҷ��� ���δ�
	TCHAR	m_strRecallFromName[64];

	//���������� �ҽ���..
	MSG_RECALL_FRIEND m_Recallresult;
	BOOL	m_bMoving; //�̵��Ҳ���..
	int m_nEffectID; //����Ʈ ���̵�


	_XButton* m_pCallButton; //�� ��ư
	_XButton* m_pMoveButton; //�̵���ư
};




//inline------------------------------------------------------------------------------------------------------------------
inline void _XWindow_RecallItem::SetRecallButton( BOOL bEnable )
{
	//_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);

	//ģ�� �Ѹ��� ��ȯ�߿��� ��ȯ�� ���ϰ� �ؾ� ��
	if( bEnable )
	{
		//if(SearchSlotItem( 2801 ))
		//	pMessenger_Window->m_btnMessengerRecallSystem->EnableWindow(bEnable);
		//else
		//	pMessenger_Window->m_btnMessengerRecallSystem->EnableWindow(FALSE);
		m_pCallButton->EnableWindow( bEnable );
		m_pMoveButton->EnableWindow( bEnable );
	}
	else
	{
		//		pMessenger_Window->m_btnMessengerRecallSystem->EnableWindow(bEnable);
		m_pCallButton->EnableWindow( bEnable );
		m_pMoveButton->EnableWindow( bEnable );
	}
}

inline void	_XWindow_RecallItem::SetMovingVariable( MSG_RECALL_FRIEND* pRecallresult, BOOL bMoving, int nEffectID )
{
	m_Recallresult = *pRecallresult;
	m_bMoving = bMoving;
	m_nEffectID = nEffectID;
}
inline void _XWindow_RecallItem::SetFromName( LPTSTR pString )
{
	memset( m_strRecallFromName, 0, sizeof(m_strRecallFromName));
	strcpy( m_strRecallFromName, pString );
}

inline void _XWindow_RecallItem::SetRecallIndex( int nIndex )
{
	m_nCallIndex = nIndex;
}

inline void _XWindow_RecallItem::SetCharState( int nState )
{
	m_nCharState = nState;
}

inline void _XWindow_RecallItem::SetSelectedItem( int nParam )
{
	m_nSelectedItem = nParam;
}

inline LPCTSTR _XWindow_RecallItem::GetFromName()
{
	return &m_strRecallFromName[0];
}

inline const int _XWindow_RecallItem::GetRecallIndex() const
{
	return m_nCallIndex;
}

inline const int _XWindow_RecallItem::GetCharState() const
{
	return m_nCharState;
}

inline const int _XWindow_RecallItem::GetItemIndex() const
{
	return m_nRecallItemIndex;
}

inline const int _XWindow_RecallItem::GetSelectedItem() const
{
	return m_nSelectedItem;
}

#endif // !defined(AFX_XWINDOW_RECALLITEM_H__28870DE7_4EF7_4898_83AF_71E9DF529334__INCLUDED_)

// XWindow_NickName.h: interface for the _XWindow_NickName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NICKNAME_H__E8822EE2_93C9_47EC_8824_06CCA1D95C1B__INCLUDED_)
#define AFX_XWINDOW_NICKNAME_H__E8822EE2_93C9_47EC_8824_06CCA1D95C1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief ��ȣ â
 * 
 * ��ȣ����Ʈ ���÷���
 * ��ȣ, ���� ���� 
*/


class _XWindow_NickName  : public _XWindow
{
public:
	_XWindow_NickName();
	virtual ~_XWindow_NickName();
	
public:
	_XStatic				m_BorderTile;
	_XImageStatic			m_BorderGradationTitle;

	_XButton*				m_ApplyButton;
	_XButton*				m_OKButton;
	_XButton*				m_DeleteButton;
	
	_XListBox*				m_listboxSpecialNickName ;		///< Ư�� ��ȣ	
	_XListBox*				m_listboxUniqueNickName ;		///< ���� ��ȣ 
	_XListBox*				m_listboxGeneralNickName ;		///< �Ϲ� ��ȣ	
	_XListBox*				m_listboxInfoNickName;			///< ���õ� ��ȣ ����, ȿ��

	int						m_SelectedNickNameSlotNumber;	///< ���õ� ��ȣ
	int						m_listboxNickIndex[213];	///< ��ȣ �ε��� ���� 
	int						m_RemainChangeTime;				///< ��ȣ �缱�� ���� �ð�
	
public:
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void MoveWindow( int X, int Y );
	
	void ShowWindow( BOOL show );
	void SetDefaultPosition( void );
	void SaveWindowProperty( void );

	void SetNickNameList( void );		///< ��ȣ ����Ʈ ����
	void SetSelectedNickNameInfo( void );	///< ���� ���õ� ��ȣ�� ����
	void ShortenedNickName( TCHAR* comNick, TCHAR* srcNick );	///< �� ��ȣ�� ���
	
	void ResetNicknameInfo(void);		///< ��ȣ ����â ����
	void CheckDeleteNickname(void);		///< ���� �� �ִ� ��ȣ���� �˻�	
};

#endif // !defined(AFX_XWINDOW_NICKNAME_H__E8822EE2_93C9_47EC_8824_06CCA1D95C1B__INCLUDED_)

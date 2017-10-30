// XWindow_SocketPlugIn.h: interface for the _XWindow_SocketPlugIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_SOCKETPLUGIN_H__FDDC3519_1E9D_4942_96A8_EB1F5B2DF989__INCLUDED_)
#define AFX_XWINDOW_SOCKETPLUGIN_H__FDDC3519_1E9D_4942_96A8_EB1F5B2DF989__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief ���� ���� â
 * 
 * ���� ������ �ִ� �����ۿ� ���Ͼ������� �����Ѵ�.
*/

/// �ִ� ���� ���� ����
#define _XDEF_SOCKETITEM_MAXCOUNT 4

/// ���� �ܰ�
enum ListBoxTextMode 
{
	_BOXTEXTMODE_READY, _BOXTEXTMODE_STEP1, _BOXTEXTMODE_STEP2
};

class _XWindow_SocketPlugIn : public _XWindow  
{
private:
	/** ����Ʈ�ڽ��� �� ������ ��� �ӽ� �ؽ�Ʈ ���� ������� _XDEF_SOCKETITEM_MAXCOUNT*2 ��
	* ���� ���� �������� �Ӽ��� ���� ������ �Ѱ��� �ִ� 2���� ������ ���� �þ ��� �÷���� ��
	*/	
	TCHAR				m_ListBoxTextList[8][255];	

public:
	_XButton*			m_SocketPlugInButton;
	_XButton*			m_pApplyCancleButton;
	_XButton*			m_pClosebutton;
	
	_XUserItem*			m_IndependentItem;		///< ��� ������ (����, �ǻ�)
	_XUserItem			m_SocketItemList[_XDEF_SOCKETITEM_MAXCOUNT];	///< ���� ������ ����
	POINT				m_SlotPosition[_XDEF_SOCKETITEM_MAXCOUNT];		///< ���� ������ ���� ��ġ
	_XImageStatic*		m_SocketSlot[_XDEF_SOCKETITEM_MAXCOUNT];		///< ���� ������ ���� �̹���

	_XMouseSlotItem		m_TempSaveSlotItem;		///< ��� �������� ����Ǿ��� �� �ʿ��� �ӽ� ���� ����
	_XMouseSlotItem		m_MouseSlotItem;
	BOOL				m_bHaveMouseSlot;
	POINT				m_IconDragStartPos;
	BOOL				m_bDragState;
	BOOL				m_bDrawSelectedItem;	
	int					m_LastDraggingSlot;
	BOOL				m_bChangeButtonMode;
	BOOL				m_MsgSocketPuluInOpen;
	
	_XListBox*			m_pSocketitemListBox;	 
	ListBoxTextMode		m_ListBoxTextMode;	
	
public:
	_XWindow_SocketPlugIn();
	virtual ~_XWindow_SocketPlugIn();

	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void				ShowWindow(BOOL show);
	void				MoveWindow( int X, int Y );	
	int					GetSlotIndex(void);
	void				DrawDragIcon(void);
	void				DrawSelectedItem(void);
	void				MessagePrint(int msgindex);
	void				ChangeIndependentItem(void);
	void				ResetSocketItemList(void);
	void				RebuildListBoxText( ListBoxTextMode mode);
	
	void				SetDefaultPosition( void );

	FLOAT				m_SocketRotateAngle;
	D3DXMATRIX			m_SocketRHWMatrix;
	_X3PCamera			m_SocketModelViewCamera;
	D3DVIEWPORT9		m_SocketModelViewPort;	

	void				DrawSocketPlugInionItem( void );
	int					GetSocketModelIndex();
	void				ProcessDblClick(void);
	
	void				ResetData(void);
	
};

#endif // !defined(AFX_XWINDOW_SOCKETPLUGIN_H__FDDC3519_1E9D_4942_96A8_EB1F5B2DF989__INCLUDED_)

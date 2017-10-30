// XWindow_Gamble.h: interface for the _XWindow_Gamble class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_GAMBLE_H__062D5303_D83E_4643_B56D_D9BF9D0E3C8A__INCLUDED_)
#define AFX_XWINDOW_GAMBLE_H__062D5303_D83E_4643_B56D_D9BF9D0E3C8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief ���� �׺� â
 * �пձ˸� ����. �пսø� ����Ͽ� Ȯ���� ���� ���� �ְ� 
 * �пս� ���� ���� ���⵵ �� �� �ִ�.
 * 
*/

#define _XDEF_GAMBLEBOX_MAXCOUNT 3		///< �ִ� ����� �� �ִ� �пս� ����

class _XWindow_Gamble : public _XWindow  
{
public:
	_XButton*			m_pBoxOpenButton;
	_XButton*			m_pApplyCancleButton;
	_XButton*			m_pClosebutton;
	
	_XUserItem*			m_GambleBoxItem;							///< �пձ�
	_XUserItem			m_GambleKeyList[_XDEF_GAMBLEBOX_MAXCOUNT];	///< �пս� 
	POINT				m_SlotPosition[_XDEF_GAMBLEBOX_MAXCOUNT];	
	_XImageStatic*		m_GambleKeySlot[_XDEF_GAMBLEBOX_MAXCOUNT];	///< �пս� �� �̹���
	
	int					m_GambleKeyFromInvenNumber[_XDEF_GAMBLEBOX_MAXCOUNT];

	_XMouseSlotItem		m_MouseSlotItem;
	BOOL				m_bHaveMouseSlot;
	POINT				m_IconDragStartPos;
	BOOL				m_bDragState;
	BOOL				m_bDrawSelectedItem;	
	int					m_LastDraggingSlot;
	BOOL				m_MsgBoxOpen;			///< �޼��� â�� �����ִ��� 

	/// 3�� ���� ���� ������
	FLOAT				m_GambleItemRotateAngle;
	D3DXMATRIX			m_GambleItemRHWMatrix;
	_X3PCamera			m_GambleItemModelViewCamera;
	D3DVIEWPORT9		m_GambleItemModelViewPort;	
public:
	_XWindow_Gamble();
	virtual ~_XWindow_Gamble();

	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void				ShowWindow(BOOL show);
	void				MoveWindow( int X, int Y );

	int					GetSlotIndex(void);	
	void				DrawDragIcon(void);
	void				DrawSelectedItem(void);
	void				MessagePrint(int msgindex);		///< ���� �޼��� ���
	void				ResetSocketItemList(void);	
	
	void				SetDefaultPosition( void );

	void				DrawSocketPlugInionItem( void );	///< 3�� �� ���
	int					GetSocketModelIndex();
	void				ProcessDblClick(void);
	
	void				ResetData(void);
	
	BOOL				CheckAllSlot(int invennumber);

};

#endif // !defined(AFX_XWINDOW_GAMBLE_H__062D5303_D83E_4643_B56D_D9BF9D0E3C8A__INCLUDED_)

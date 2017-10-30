// XWindow_PCTrade.h: interface for the _XWindow_PCTrade class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_PCTRADE_H_
#define _XWINDOW_PCTRADE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief PC �ŷ�â 
 * 
 * ������ m_TargetConfirmIcon �̹����� �� ���¿��� �ڽ��� ok ��ư�� ������ �ŷ� ����
 * �ڽ��� m_ConfirmIcon �̹����� �� ���¿��� ������ �������� �߰��ϰų� �����ų� �ϸ�
 * �ڽ��� ���´� m_WaitIcon �� ����( ���浵 ��������)
*/

#define _XDEF_PCTRADE_MAX_SLOTNUMBER		10		///< �ŷ��� �� �ִ� �ִ� ������ ���� 


class _XWindow_PCTrade : public _XWindow  
{
public :
	_XImageStatic*			m_TargetWaitIcon;		///< ���� ��� �̹���
	_XImageStatic*			m_TargetConfirmIcon;	///< ���� �Ϸ� �̹���
	
	_XImageStatic*			m_WaitIcon;				///< �ڽ��� ��� �̹���
	_XImageStatic*			m_ConfirmIcon;			///< �ڽ��� �Ϸ� �̹���

	_XButton*				m_OKButton;
	_XButton*				m_CancelButton;
	
	_XButton*				m_TradeMoneyButton;			///< �ŷ�â�� �ݾ� ��ư
	_XButton*				m_InventoryMoneyButton;		///< �κ��� �ݾ� ��ư
	
	BOOL					m_bRequest;			///< ��û ������
	unsigned short			m_TargetID;
	TCHAR					m_TargetName[_XDEF_MAX_USERNAMESTRINGLENGTH];
	WORD					m_TradeServerSlotNo;	///< ������ ��ġ�ؾ� �ŷ� ����
	
	/// Target ����
	_XTradeItem				m_TargetTradeItem[_XDEF_PCTRADE_MAX_SLOTNUMBER];
	DWORD					m_TargetMoney;
	
	/// �� ����
	_XTradeItem				m_SelfTradeItem[_XDEF_PCTRADE_MAX_SLOTNUMBER];
	DWORD					m_SelfMoney;
	
	POINT					m_TargetSlotPosition[_XDEF_PCTRADE_MAX_SLOTNUMBER];
	POINT					m_SelfSlotPosition[_XDEF_PCTRADE_MAX_SLOTNUMBER];
	
	BOOL					m_bWaitTrade;
	
	_XMouseSlotItem			m_MouseSlotItem;
	BOOL					m_bHaveMouseSlot;
	BOOL					m_bDragState;
	POINT					m_IconDragStartPos;
	
	int						m_LastDraggingSlot;
	BOOL					m_bIsTarget;

	BOOL					m_bDrawSelectedItem;
	BOOL					m_bTrading;
	
public:
	_XWindow_PCTrade();
	virtual ~_XWindow_PCTrade();
	
	BOOL					Initialize( void );
	void					DestroyWindow( void );
	
	void					ResetTradeInfo(void);
	
	void					Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL					Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void					DrawDragIcon(void);
	void					DrawSelectedItem(void);
	void					DrawItemTooltip(int slotnumber, RECT iconrect);
	
	void					SaveWindowProperty( void );
	void					ShowWindow(BOOL show);
	
	int						SearchFullSlotIndex(void);
	int						SearchEmptySlotIndex(void);
	int						GetNextEmptySlot(void);
	
	void					RestoreTrade(void);
	void					ProcessDblClick(void);

	void					CopyItemToSlot(int slotnumber, int invennumber);	
	
	void					SetDefaultPosition( void );
	void					ResetTradeSetting( void );
};

#endif // !defined(_XWINDOW_PCTRADE_H_)

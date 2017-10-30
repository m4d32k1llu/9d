// XWindow_TradeConfirm.h: interface for the _XWindow_TradeConfirm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TRADECONFIRM_H__8CD96B77_63C7_4E40_9666_B18620B1771D__INCLUDED_)
#define AFX_XWINDOW_TRADECONFIRM_H__8CD96B77_63C7_4E40_9666_B18620B1771D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief ���� �Է� â 
 * �ش� ��忡 ���� ���� �Է�
 * 
*/

/// ���� �Է� ��� 
typedef enum _XTrade_Mode
{
	_XTRADE_NONE = 0,
	_XDROP_MONEY,						///< �� ������ 
	_XWAREHOUSE_TOWAREHOUSE_MONEY,		///< �κ� -> â��
	_XWAREHOUSE_FROMWAREHOUSE_MONEY,	///< â�� -> �κ�
	_XPCTRADE_TOTRADE_MONEY,			///< �κ� -> �ŷ�â
	_XPCTRADE_FROMTRADE_MONEY,			///< �ŷ� -> �κ�
	_XNPCTRADE_BUY,						///< ���� -> �κ�
	_XPCTRADE_FROMTRADE_ITEM,			///< �ŷ� -> �κ� 
	_XWAREHOUSE_FROMWAREHOUSE_ITEM,		///< â�� -> �κ�
	_XCONTRIBUTION_FROMCONTRI_ITEM,		
	_XNPCTRADE_SELL = 11,				///< �κ� -> ���� 
	_XPCTRADE_TOTRADE_ITEM,				///< �κ� -> �ŷ� 
	_XPERSONALSTORE_TOTRADE_ITEM,		///< �κ� -> ���� ����
	_XWAREHOUSE_TOWAREHOUSE_ITEM,		///< �κ� -> â�� 
	_XCONTRIBUTION_TOCONTRI_ITEM,		
	_XDROP_ITEM,						///< ������ ������

	_XCONTRIBUTION_TOCONTRIBUTION_MONEY,	///< �κ� -> �⿩
	_XCONTRIBUTION_TOCONTRIBUTION_ITEM,		///< �κ� -> �⿩
	
	_XDROP_WASTEBASKET_ITEM,			///< ������ ������ - ���� ������
	
}_XTRADE_MODE;

class _XWindow_TradeConfirm : public _XWindow  
{
public :
	_XButton*		m_AllButton;// 2004.05.17->oneway48 insert
	
	_XButton*		m_OKButton;
	_XButton*		m_CancelButton;
	
	_XIMEContainer	m_IMEControl;		///< �Է�â
	
	DWORD			m_Price;			///< ����
	DWORD			m_TradeCount;		///< ����
	
	char			m_SelectedCategory1;
	short			m_SelectedItemID;
	int				m_InventoryNumber;
	
	TCHAR			m_CountString[64];
	
	_XTRADE_MODE	m_TradeMode;
	
	/// PC�� �ŷ����� ���
	int				m_PCTradeSlotIndex;
	/// ���λ������� ���
	int				m_PersonalStoreSlotIndex;
	
public:
	_XWindow_TradeConfirm();
	virtual ~_XWindow_TradeConfirm();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	BOOL			CheckIMEBoxArea(void);
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	
	void			GenerateItemPrice(void);	///< ���� ����
	int				PriceCalc(); /// �Ǹ� ���� ���� Author : �����
	
	void			Reset(void);
};

#endif // !defined(AFX_XWINDOW_TRADECONFIRM_H__8CD96B77_63C7_4E40_9666_B18620B1771D__INCLUDED_)

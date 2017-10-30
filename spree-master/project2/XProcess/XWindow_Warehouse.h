// XWindow_Warehouse.h: interface for the _XWindow_Warehouse class.
//
// Author : Hangil, Yang
// Modify : Sohyun, Park
// Desc : User warehouse, Warehouse window class
// 
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_WAREHOUSE_H_
#define _XWINDOW_WAREHOUSE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable: 4786)

#include <list>
#include "XNetwork.h"
#include "XWindow_Inventory.h"
#include "XUser.h"
#include "XLocalUser.h"

using namespace std;

/**
 * \brief â�� 
 * ��ǰ �ñ��, ������, ����
 * 
*/

/// �ѹ��� �̵��� �� �ִ� �ִ� ����
#define _XDEF_WAREHOUSE_MAX_SENDCOUNT			20	

/// �̵� ���
typedef enum
{
	_XSENDMODE_NONE = 0,
	_XSENDMODE_TOWAREHOUSE_ITEM,	///< �κ� -> â��
	_XSENDMODE_TOINVEN_ITEM,		///< â�� -> �κ�
	_XSENDMODE_TOWAREHOUSE_MONEY,	///< �κ� -> â��
	_XSENDMODE_TOINVEN_MONEY		///< â�� -> �κ�
}_XSendMode;

class _XWindow_Warehouse : public _XWindow  
{
private:
	_XImageStatic		m_ItemEdgeIcon[3];
	int					m_SelectedTab;
public :
	_XCheckButton*		m_SelectTabButtton[3];	///< �ִ� 40�� �̹Ƿ� //Author : ����� //breif : 60�� Ȯ��

	_XButton*			m_MoveButton;
	_XButton*			m_SelectAllButton;
	_XButton*			m_CancelButton;
	_XButton*			m_StorageMoneyButton;
	_XButton*			m_InventoryMoneyButton;	

	BOOL				m_bWarehouseFirstLoad;
	BOOL				m_bWarehouseFirstView; ///< 2004.06.01->oneway48 insert : â�� ���� �������� �ѹ��� ��Ÿ������ �Ѵ�.
	int					m_LastDraggingSlot;

	list <_XWarehouseItem*>		m_TempItemList;		///< ���� ���õ� �����۵�
	POINT				m_SlotPosition[_XDEF_WAREHOUSE_MAX_SENDCOUNT];
	DWORD				m_TempMoney;

	_XSendMode			m_SendMode;
	BOOL				m_bWaitForServerResult;

	_XMouseSlotItem		m_MouseSlotItem;
	BOOL				m_bHaveMouseSlot;
	POINT				m_IconDragStartPos;
	BOOL				m_bDragState;

	BOOL				m_bProcessOneClickItem;
	BOOL				m_bDrawSelectedItem;
	
	int					m_UseSlotCount;		///<2004.05.20->oneway48 insert : ��� ������ ĭ��
	BOOL				m_bUseableWarehouse; ///< 2004.06.01->oneway48 insert : â�� ����� �� �ִ��� ������ üũ
	BOOL				m_bWarehouseCancleMode; ///< 2004.06.01->oneway48 insert : â���� ������� â������������� ����
	BOOL				m_bTerminateWarehouse; ///< 2004.06.02->oneway48 insert : �Ⱓ�� ����Ǿ��� �� 
	int					m_TerminateLastItemIndex;	///< â�� ���� �� �������� �ִ� ������ �ڸ� �̰����� â�� ���� ĭ���� ����.

	/// 2004.05.24->oneway48 insert : â�� ���Ⱓ üũ 
	int					m_TerminativeYear   ;
	int					m_TerminativeMonth ;
	int					m_TerminativeDay   ;
	int					m_TerminativeHour  ;  
	
	DWORD				m_TradeDelayTime;	///< �����ð� �� �ð��� ������ ���� ����� False �� �ٲ�
	
	DWORD				m_dwStorageInfoReqTime;		// â�� ������ ��û�� �ð�
	
public:
	_XWindow_Warehouse(); 
	virtual ~_XWindow_Warehouse();

	BOOL				Initialize(void);
	void				DestroyWindow(void);

	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void				DrawSelectedItem(void);
	void				DrawDragIcon(void);
	void				DrawItemTooltip(int slotnumber, RECT iconrect);

	void				ShowWindow(BOOL show);

	int					HaveItemFromInven(int invenindex);
	int					HaveItemFromWarehouse(int slotnumber);

	void				SetTempItemFromInven(int invenindex, int count = 0);
	void				ResetTempItemFromInven(int invenindex);

	void				SetTempItemFromWarehouse(int slotindex, int count = 0);
	void				ResetTempItemFromWarehouse(int slotindex);

	void				ReleaseTempItemList(void);

	void				ItemSaveResultToWarehouse(int resultcount);
	void				ItemSaveResultToInventory(int resultcount);

	int					GetSlotIndex(void);	
	void				ChangeTab( int tab );

	void				ProcessDblClick(void);
	void				SetTerminateLastItemIndex();
};

#endif

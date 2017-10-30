// XWindow_Inventory.h: interface for the _XWindow_Inventory class.
//
// Author		: Iljun, Park / Sohyun, Park
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_INVENTORY_H__F2E99A16_EC25_4EB8_93A5_947127E98C3C__INCLUDED_)
#define AFX_XWINDOW_INVENTORY_H__F2E99A16_EC25_4EB8_93A5_947127E98C3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XGameItem.h"

/**
 * \brief �κ��丮
 * 
 * 
 * 
 * 
 * 
*/

typedef enum
{
	_XINVENTORYBASETAB_POCKET	= 0,	///< �ָӴ�
	_XINVENTORYBASETAB_BACKPACK,		///< �賶
	_XINVENTORYBASETAB_BACKPACK2,		///< �賶2
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER
	_XINVENTORYBASETAB_CASHBACKPACK1,	///< Author : ����� //breif : ĳ���೶
	_XINVENTORYBASETAB_CASHBACKPACK2
#else
	_XINVENTORYBASETAB_CHARINFO,		///< ĳ��â	
#endif
}_XInventoryWindowBaseTab;

typedef enum
{
	_XINVENTORYSLOTTAB_1	= 0,
	_XINVENTORYSLOTTAB_2,	
}_XInventoryWindowSlotTab;

/// ���� �����޼��� �ڵ� 
typedef enum
{
	_XEQUIPERROR_TYPE = 0,			///< Ÿ���� Ʋ��
	_XEQUIPERROR_CLAN,				///< ���İ� Ʋ��
	_XEQUIPERROR_SEX,				///< ������ Ʋ��
	_XEQUIPERROR_POCKET,			///< ������ �������� �ָӴϰ� ������ �������� �ָӴϺ��� ���� ���
	_XEQUIPERROR_ACCESSORY,			///< �Ǽ����� ������ ���� �� 
	_XEQUIPERROR_OTHER,				///< ���� �������� �� 
	_XEQUIPERROR_NO,				///< ���
	_XEQUIPERROR_QUESTHOLDITEM,		///< ��ȣǳ������� ���ؼ� �������� �����Ǿ��ִ� ���
	_XEQUIPERROR_LEVEL,				///< ������ ���ڶ�
	_XEQUIPERROR_ACCOVERLAPPING,	///< �Ϲ� �Ź� �ߺ���
	_XEQUIPERROR_PERIODITEM_ACCOVERLAPPING,	///< �Ⱓ�� ������ �ߺ� �Ұ� �������� �ߺ���
	_XEQUIPERROR_FAMEOVERLAPPING,	///< ��, �Ǹ� ������ �ߺ���
	_XEQUIPERROR_NOOVERLAPPING,		///< �ߺ����� �Ұ� ������
	_XEQUIPERROR_STR,				///< �ٷ� ����
	_XEQUIPERROR_CONSTITUTION,		///< �ǰ� ����
	_XEQUIPERROR_ZEN,				///< ���� ����
	_XEQUIPERROR_INT,				///< ���� ����
	_XEQUIPERROR_DEX,				///< ��ø ����
	_XEQUIPERROR_FAME,				///< �� ����
	_XEQUIPERROR_INFAMY,			///< �Ǹ� ����
	_XEQUIPERROR_GRADE,				///< ����� �ٸ�
	_XEQUIPERROR_CLASS,				///< ��å�� �ٸ�
}_XInventoryEquipErrorCode;

typedef enum tagXINVENITEMEDGEICONTYPE
{
	_XITEMEDGEICONTYPE_EMPTY = 0,
	_XITEMEDGEICONTYPE_NORMAL,
	_XITEMEDGEICONTYPE_DISABLESLOT,
	_XITEMEDGEICONTYPE_DISABLEBACKPACK,
	
} _XINVENITEMEDGEICONTYPE;

class _XWindow_NPCTrade;
class _XWindow_PCTrade;
class _XWindow_Warehouse;
class _XWindow_RepairItem;
class _XWindow_EnterBossRoom;
class _XWindow_SocketPlugIn;
class _XWindow_SocketPlugInExp;
class _XWindow_CInfo;
class _XWindow_Contribution;
class _XWindow_Pocket;
class _XWindow_PersonalStore;
class _XWindow_TradeConfirm;
class _XWindow_Gamble;
class _XWindow_CashItem;
class _XWindow_Inchant;
class _XWindow_MixWeapon;
class _XWindow_NPCResource_New;

class _XWindow_Inventory : public _XWindow  
{
	
public :
	_XButton*				m_InvenList;
	_XButton*				m_InvenHat;
	_XButton*				m_InvenEarring;
	_XButton*				m_InvenNecklace;
	_XButton*				m_InvenJacket;
	_XButton*				m_InvenMantle;
	_XButton*				m_InvenRing;
	_XButton*				m_InvenWeapon;
	_XButton*				m_InvenFist;
	_XButton*				m_InvenPants;
	_XButton*				m_InvenSupport;
	_XButton*				m_InvenAmmunition;
	_XButton*				m_InvenFootwear;

	_XButton*				m_btnMoney;
		
	_XCheckButton*			m_BaseTabButton[5]; //Author : ����� //breif : �೶ Ȯ�� 
	_XButton*				m_CashItemButton;
	BOOL					m_bIteminCashBackPack[2];				//Author : ����� //breif : ĳ�� �೶ ������ �ִ��� ����
		
	_XInventoryWindowBaseTab m_SelectedBaseTab;		///< �⺻ �κ��丮 ����Ʈ ��
	_XInventoryWindowBaseTab m_LastDraggingBaseTab;	///< �������� �巡�� ���� ����� �� 

	// Mouse slot
	int						m_LastDraggingSlot;
	BOOL					m_bHaveMouseSlot;
	_XMouseSlotItem			m_MouseSlotItem;

	// Drag
	BOOL					m_bDragState;
	POINT					m_IconDragStartPos;

	_XWindow_NPCTrade*		m_pNPCTradeWindow;
	_XWindow_PCTrade*		m_pPCTradeWindow;
	_XWindow_Warehouse*		m_pWarehouseWindow;
	_XWindow_RepairItem*	m_pRepairItem_Window; 
	_XWindow_EnterBossRoom*	m_pEnterBossRoom_Window;
	_XWindow_SocketPlugIn*  m_pSocketPlugIn_Window;
	_XWindow_SocketPlugInExp*  m_pSocketPlugInEx_Window;
	_XWindow_Contribution*	m_pContribution_Window;
	_XWindow_Pocket*		m_pPocket_Window;
	_XWindow_PersonalStore* m_pPersonalStore_Window;
	_XWindow_TradeConfirm*	m_pTradeConfirm_Window;
	_XWindow_Gamble*		m_pGamble_Window;
	_XWindow_CashItem*		m_pCashItem_Window;
	_XWindow_Inchant*		m_pInchant_Window;
	_XWindow_MixWeapon*		m_pMixWeapon_Window;
	_XWindow_NPCResource_New* m_pNPCResource_New_Window;

	int						m_iSocketIndexList[5];		///< ���� ����â���� ������ ���� ������ ����Ʈ 
	int						m_UsedLifeItemIndex;		///< ���� ����â���� ������ ������ ������
	BOOL					m_bLockInventory;			///< �κ� ���(�̵� �Ұ�)
	BOOL					m_bDrawSelectedItem;	

	_XImageStatic			m_GrabItemImage;			///< ������ ����� ȿ�� �̹���
	BOOL					m_bDrawGrabItem;			///< ������ ����� ȿ�� �÷���
	LONG					m_SpeedCounter;				///< ������ ����� ȿ�� �ӵ�
	FLOAT					m_EffectFrame;				
	int						m_EffectPrevFrame;

	int						m_ElixirItemEffectFrame;			///< ���� ȿ�� ������ ����  ---��
	int						m_ElixirItemEffectFrameCount;
	int						m_ElixirItemEffectFrameChange;		/// ---------------------------��	

	_XStatic				m_BorderTile;
	_XImageStatic			m_ItemEdgeIcon[4];
	_XImageStatic*			m_ExpandDockingBorder;	
	_XImageStatic*			m_imageCashItemBorder;	
	
	BOOL					m_bExpandDockingBorderFlag;
	list <int>				m_TradeItemList;

	int						m_ContributionItemInvenNumber;
	int						m_ElixirInvenNumber;
	int						m_BookInvenNumber;
	
	int						m_nPriceMode;
	_XImageStatic*			m_pClanPointImage;
	_XImageStatic*			m_pImageWasteBasket; //Author : ����� //breif : �������� �̹��� ����Ȱ�
	BOOL					m_bWasteBasketLock; //Author : ����� //breif : ������ ���� ��,, �̹��� ����ִ´�
	
public:
	_XWindow_Inventory();
	virtual ~_XWindow_Inventory();
	
	BOOL					Initialize( void );
	void					DestroyWindow( void );
	
	void					Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL					Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);	
	
	void					MoveWindow(int X, int Y);
	void					ShowWindow(BOOL show);
	
	void					SetDefaultPosition( void );
	void					SaveWindowProperty( void );
			
	void					ChangeBaseTab(int selectedtab);
	void					ChangeSlotTab(_XInventoryWindowSlotTab selectedtab);	

	void					DrawPocketList( void );
	void					DrawBackPackList( void );
	void					DrawPassList( void );
	void					DrawCharEquipList( void );
	
	void					DrawGrabItem(); 
	void					DrawItemTooltip(int slotnumber, RECT iconrect);	

	char					GetSocketListCount();
	char					GetSocketListInsertPos();
	void					ResetSocketIndexList();
	void					ResetSocketIndexList_GambleBox();
	
	// Left click
	BOOL					ProcessNPCTrade(void);
	BOOL					ProcessFromWarehouse(void);
	BOOL					ProcessPCTrade(void);
	BOOL					ProcessItemDrop(void);
	BOOL					ProcessRepairItem(); 
	BOOL					ProcessFromSocketPlugIn(void);
	void					ProcessInvenMove(void);
	BOOL					ProcessGamble(void);
	BOOL					ProcessWasteBasket(void);

	// Right click
	BOOL					ProcessSellToNPC(void);
	BOOL					ProcessToWarehouse(void);
	BOOL					ProcessToPCTrade(void);
	BOOL					ProcessToRepair(void);
	BOOL					ProcessItemEquip(void);
	BOOL					ProcessToSocketPlugIn(void);
	BOOL					ProcessToSocketPlugInEx(void);
	BOOL					ProcessToContribution(void);
	BOOL					ProcessFromPocket(void);
	BOOL					ProcessToPersonalStore(void);
	BOOL					ProcessToGamble(void);
	BOOL					ProcessToEnterBossRoom(void);
	BOOL					ProcessToInchant(void);
	BOOL					ProcessToSmeltItem(void); //Author : �����
	BOOL					ProcessToSocketDetach(void); //Author : �����
	BOOL					ProcessToMixWeapon(void);
	BOOL					ProcessToNPCResourceNew(void);


	int						GetUserItemListEmptyCount();	///< ����ִ� �κ��丮 ĭ ã��
    int                     GetCashBackpackStartSlotPosition(); // Author : ����� //breif : ĳ���೶ ���� ����
	_XInventoryEquipErrorCode	CheckSlotType( int slotindex );		
	int						SearchSlotRegion( RECT* pRect = NULL );
    void                    SetTabButtonPosition(); //Author : ����� //breif : ��ư ��ġ ����

	void					DrawIcon( BOOL smallicon, int X, int Y, _XUserItem* pItem = NULL, BOOL bMarkStackable = TRUE , BOOL busingitem = TRUE ); // 2004.06.10->oneway48 insert
	void					DrawIcon_LifeSkill( BOOL smallicon, int X, int Y, _XCollectionResourceItem* pItem = NULL, BOOL bMarkStackable = TRUE , BOOL busingitem = TRUE ); // 2004.06.10->oneway48 insert
	void					DrawIcon_Gamble( BOOL smallicon, int X, int Y, _XUserItem* pItem = NULL, BOOL bMarkStackable = TRUE , BOOL busingitem = TRUE, BOOL checksex = TRUE ); 
	void					DrawDragIcon( void );
	
	void					GetIconRect(char cType, short sID, int& imageindex, RECT& rect, BOOL smallicon);
	char					GetMaxSocketCount(char ctype, short sID);
	void					DrawSelectedItem( void );
	void					DrawSocketItem( void );	
	void					DrawRepairItem(void);
	void					DrawMixWeaponItem(void);
	void					DrawNPCResource_New_Item(void);

	void					ProcessDblClick(void);
	void					OnLButtonUseWarehouse(void);
	void					OnRButtonUseWarehouse(void);

	BOOL					CheckIsRead( int sID );			///< ���� �� �ִ� ������� üũ 
	BOOL					CheckIsInternalUse( int sID );	///< ������ �� �ִ� �������� üũ
	BOOL					CheckDropArea();				///< ������ ������ üũ
	BOOL					CheckOverlapIndex(int index);	///< �ߺ��� �ε����� �ִ� ��� �ϳ��� �׸�
	BOOL					CheckItemEquipState(); //Author : ����� //brief : ������ ��ü ������ �����ΰ� üũ�Ѵ�.
	void					SetExpandDockingBorderFlag(BOOL flag);
	void					ResetMouseSlot(void);

	void					SetPriceMode(int mode);
	int						GetPriceMode()			{	return m_nPriceMode;		};

	void SetNPCTradeWindowPtr(_XWindow_NPCTrade* windowptr)		{	m_pNPCTradeWindow = windowptr;	}
	void SetPCTradeWindowPtr (_XWindow_PCTrade* windowptr)		{	m_pPCTradeWindow = windowptr;	}
	void SetWarehouseWindowPtr(_XWindow_Warehouse* windowptr)	{	m_pWarehouseWindow = windowptr;	}
	void SetRepairItemWindowPtr(_XWindow_RepairItem* windowptr)	{	m_pRepairItem_Window = windowptr;	}
	void SetEnterBossRoomWindowPtr(_XWindow_EnterBossRoom* windowptr)	{	m_pEnterBossRoom_Window = windowptr;	}
	void SetSocketPlugInWindowPtr( _XWindow_SocketPlugIn* windowptr) { m_pSocketPlugIn_Window = windowptr; }	
	void SetSocketPlugInExpWindowPtr( _XWindow_SocketPlugInExp* windowptr) { m_pSocketPlugInEx_Window = windowptr; }
	void SetContributionWindowPtr( _XWindow_Contribution* windowptr) { m_pContribution_Window = windowptr; }
	void SetPocketWindowPtr( _XWindow_Pocket* windowptr) { m_pPocket_Window = windowptr;	};
	void SetPersonalStoreWindowPtr( _XWindow_PersonalStore* windowptr) { m_pPersonalStore_Window = windowptr;	};
	void SetTradeConfirmWindowPtr( _XWindow_TradeConfirm* windowptr) { m_pTradeConfirm_Window = windowptr; 	};
	void SetGambleWindowPtr( _XWindow_Gamble* windowptr) { m_pGamble_Window = windowptr;	};
	void SetCashItemWindowPtr( _XWindow_CashItem* windowptr ) { m_pCashItem_Window = windowptr;	};
	void SetInchantWindowPtr( _XWindow_Inchant* windowptr ) { m_pInchant_Window = windowptr; };
	void SetMixWeaponWindowPtr(_XWindow_MixWeapon* windowptr)	{ m_pMixWeapon_Window = windowptr; };
	void SetNPCResource_New_WindowPtr(_XWindow_NPCResource_New* windowptr)	{m_pNPCResource_New_Window = windowptr; };
};

#endif // !defined(AFX_XWINDOW_INVENTORY_H__F2E99A16_EC25_4EB8_93A5_947127E98C3C__INCLUDED_)




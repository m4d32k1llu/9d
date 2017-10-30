 // XWindow_Inventory.cpp: implementation of the _XWindow_Inventory class.
//
// Author		: Kery Park / Sohyun, Park
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindow_Inventory.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_TradeConfirm.h"
#include "XWindow_NPCTrade.h"
#include "XWindow_Warehouse.h"
#include "XWindow_PCTrade.h"
#include "XWindow_RepairItem.h"
#include "XWindow_EnterBossRoom.h"
#include "XWindow_SocketPlugIn.h"
#include "XWindow_JinStatus.h"
#include "xwindow_cinfo.h"
#include "XWindow_MainFrame.h"
#include "xwindow_mainmenu.h"
#include "XWindow_Contribution.h"
#include "xwindow_chargegauge.h"
#include "XWindow_Pocket.h"
#include "XWindow_PersonalStore.h"
#include "XWindow_AddSaveItem.h"
#include "XWindow_Gamble.h"
#include "XWindow_SocketPlugInExp.h"
#include "XWindow_MatchPR.h"
#include "XWindow_CashItem.h"
#include "XWindow_Inchant.h"
#include "XWindow_BookOpen.h"
#include "XWindow_SystemMsgDefault.h"
#include "XMessageWindow.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"
#include "SoundEffectList.h"
#include "XWindow_BloodPoint.h"
#include "XWindow_SmeltItem.h" //Author : �����
#include "XWindow_SocketDetach.h" //Author : �����
#include "XWindow_MixWeapon.h"
#include "XWindow_NPCResource_New.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static int _XDEF_INVENTORYITEMPOSX = 11;
static int _XDEF_INVENTORYITEMPOSY = 71;


static int _XDEF_INVENTORYSLOT_WIDTH = 5;
static int _XDEF_INVENTORYSLOT_HEIGHT = 5;
static int _XDEF_INVENTORY_ONEBACKPACKCOUNT = 25;

extern POINT g_EquippedItemCoord[20];

unsigned long g_MoneyColor[6] = 
{
	0xFFFFFFFF,
	0xFF00D700,
	0xFF00B4FF,
	0xFFFFC800,
	0xFFFF96BE,
	0xFF00FFFF,	
};

_XImageStatic			g_InventoryIconImage;
_XImageStatic			g_InventoryGradeIconImage;
_XWindow_Inventory*		g_pInventory_Window = NULL;

_XWindow_Inventory::_XWindow_Inventory() 
: 	m_nPriceMode(0),
	m_pClanPointImage(NULL)
{		
	g_pInventory_Window = this;

	for(int i = 0 ; i < 5 ; i++) //Author : ����� //breif : �೶ Ȯ�� 
	{
		m_BaseTabButton[i]		= NULL;
	}	
	m_CashItemButton			= NULL;
	m_pNPCTradeWindow			= NULL;
	m_pPCTradeWindow			= NULL;
	m_pWarehouseWindow			= NULL;
	m_pRepairItem_Window		= NULL;
	m_pEnterBossRoom_Window		= NULL;
	m_pSocketPlugIn_Window		= NULL;
	m_pContribution_Window		= NULL;
	m_pPocket_Window			= NULL;
	m_pPersonalStore_Window		= NULL;
	m_pTradeConfirm_Window		= NULL;
	m_pGamble_Window			= NULL;
	m_pSocketPlugInEx_Window	= NULL;
	m_pCashItem_Window			= NULL;
	m_pInchant_Window			= NULL;
	m_pMixWeapon_Window			= NULL;
	m_pNPCResource_New_Window	= NULL;
	
	m_ExpandDockingBorder		= NULL;	
	m_imageCashItemBorder		= NULL;

	m_SelectedBaseTab = _XINVENTORYBASETAB_POCKET;
	m_LastDraggingBaseTab = _XINVENTORYBASETAB_POCKET;	
	m_LastDraggingSlot = -1;

	m_bHaveMouseSlot = FALSE;
	m_MouseSlotItem.ResetMouseSlotItem();

	m_btnMoney = NULL;
	m_bLockInventory = FALSE;

	m_bDragState	= FALSE;
	m_IconDragStartPos.x = m_IconDragStartPos.y = 0;

	m_bDrawSelectedItem = FALSE;	

//	m_bUsingPotionItem = FALSE;
//	m_bClickUsingItem = FALSE;
//	m_bDrawUsingItemTooltip = FALSE;
	m_bExpandDockingBorderFlag = FALSE;
//	m_UsingItemNumber = -1;
//	m_RecoveryPerSec = 0;
//	m_AmountOfRecovery = 0;
//	m_AfterExternalWoundRate = 0;
//	m_BeforeExternalWoundRate = 0;

	m_bDrawGrabItem = FALSE; //2004.06.22->oneway48 insert
	m_SpeedCounter = 0;
	m_EffectFrame = 0.0f;
	m_EffectPrevFrame = 0;
	m_ElixirItemEffectFrame = 0;
	m_ElixirItemEffectFrameCount = 1;
	m_ElixirItemEffectFrameChange = 1;
	m_WindowMouseState = _XW_STATE_NONE;
	for( i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT+1; i++ )
		m_iSocketIndexList[i] = 0;

	m_UsedLifeItemIndex	= 0;
	m_ContributionItemInvenNumber = 0;
	m_ElixirInvenNumber = -1;
	m_BookInvenNumber = -1;

	m_bIteminCashBackPack[0] = FALSE; //Author : ����� //breif : ĳ�� �೶ ������ �ִ��� ����
	m_bIteminCashBackPack[1] = FALSE;

	m_pImageWasteBasket = NULL;
	m_bWasteBasketLock = FALSE;
}

_XWindow_Inventory::~_XWindow_Inventory()
{
	SAFE_DELETE(m_imageCashItemBorder);
	SAFE_DELETE(m_pClanPointImage);
	//DestroyWindow();
}

BOOL _XWindow_Inventory::Initialize( void )
{	
	SetDrawBorder( FALSE );

	// Create GUI Object =================================================================
	int inventoryimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );

	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 184, 70,
					   &g_MainInterfaceTextureArchive, inventoryimageindex );	
	pTitleBar->SetClipRect( 0, 125, 183, 194 );
	InsertChildObject( pTitleBar );

	_XImageStatic* pLeftWindowLine = new _XImageStatic;
	pLeftWindowLine->Create( 0, 69, 8, 234,
					   &g_MainInterfaceTextureArchive, inventoryimageindex );
	pLeftWindowLine->SetClipRect( 184, 64, 193, 230 );
	InsertChildObject( pLeftWindowLine );


	_XImageStatic* pRightWindowLine = new _XImageStatic;
	pRightWindowLine->Create( 175, 69, 182, 234,
					   &g_MainInterfaceTextureArchive, inventoryimageindex );
	pRightWindowLine->SetClipRect( 194, 64, 202, 230 );
	InsertChildObject( pRightWindowLine );


	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create( 0, 235, 182, 269,
					   &g_MainInterfaceTextureArchive, inventoryimageindex );
	pBottomBorder->SetClipRect( 0, 195, 183, 230 );
	InsertChildObject( pBottomBorder );

	m_BorderTile.Create( m_WindowPosition.x + 3, m_WindowPosition.y + 69, m_WindowPosition.x + 179, m_WindowPosition.y + 235 );
	m_BorderTile.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_BorderTile.tga" ) );
	m_BorderTile.SetFaceColor( D3DCOLOR_ARGB( 128, 255,255,255 ) );
	
	SAFE_DELETE(m_ExpandDockingBorder);
	m_ExpandDockingBorder = new _XImageStatic;
//	m_ExpandDockingBorder->Create( 0, 267, 183, 288, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" ) );
//	m_ExpandDockingBorder->SetClipRect( 0, 79, 183, 100);	
	m_ExpandDockingBorder->Create( 0, 267, 183, 298, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" ) );
	m_ExpandDockingBorder->SetClipRect( 0, 26, 183, 57);	
	m_ExpandDockingBorder->ShowWindow( FALSE );
	InsertChildObject( m_ExpandDockingBorder );	

#ifdef _XTS_USECASHITEM
	SAFE_DELETE(m_imageCashItemBorder);
	m_imageCashItemBorder = new _XImageStatic;
	m_imageCashItemBorder->Create( 0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_CashItem.tga" ) );
	m_imageCashItemBorder->SetClipRect( 1, 1, 31, 31);
#endif

	FLOAT ufactor = 183.0f/128.0f;
	FLOAT vfactor = 167.0f/128.0f;	
	m_BorderTile.ChangeUV( 0.0f,ufactor,ufactor,0.0f, 0.0f,0.0f,vfactor,vfactor );	


	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].Create( 0, 0, 34, 34, &g_MainInterfaceTextureArchive, inventoryimageindex );
	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].SetClipRect( 213, 64, 247, 98 );

	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_NORMAL].Create( 0, 0, 34, 34, &g_MainInterfaceTextureArchive, inventoryimageindex );
	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_NORMAL].SetClipRect( 213, 30, 247, 64 );

	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_DISABLESLOT].Create( 0, 0, 30, 30, &g_MainInterfaceTextureArchive, inventoryimageindex );
	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_DISABLESLOT].SetClipRect( 216, 0, 246, 30 );

	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_DISABLEBACKPACK].Create( 0, 0, 42, 42, &g_MainInterfaceTextureArchive, inventoryimageindex );
	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_DISABLEBACKPACK].SetClipRect( 205, 97, 247, 139 );

	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_INV_CLOSEBUTTON,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );
	
	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 153, 2 }, { 12, 12 }, _XDEF_INV_HELPBUTTON,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );

	int inventoryimageindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back02.tga" );
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    _XCHECKBTN_STRUCTURE tabbtnstruct = 
    { TRUE, 
      { 4, 23 }, 
      { 32, 32 }, 
      _XDEF_INV_BASETAB_POCKET,
      inventoryimageindex2,
      inventoryimageindex2,
      &g_MainInterfaceTextureArchive };
	
	m_BaseTabButton[0] = new _XCheckButton;
	m_BaseTabButton[0]->Create( tabbtnstruct );		
	m_BaseTabButton[0]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
	m_BaseTabButton[0]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
	m_BaseTabButton[0]->SetButtonSwapMode(TRUE);
	m_BaseTabButton[0]->SetCheck( TRUE );
	InsertChildObject( m_BaseTabButton[0] );
	
	tabbtnstruct.position.x += 36;
	tabbtnstruct.commandid++;
	
	for( int i = 1; i < 3; i++ )
	{			
		m_BaseTabButton[i] = new _XCheckButton;
		m_BaseTabButton[i]->Create( tabbtnstruct );		
		m_BaseTabButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
		m_BaseTabButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
		m_BaseTabButton[i]->SetButtonSwapMode(TRUE);
		m_BaseTabButton[i]->SetCheck( i == 0 );
		InsertChildObject( m_BaseTabButton[i] );
		
		tabbtnstruct.position.x += 36;
		tabbtnstruct.commandid++;

        m_BaseTabButton[i]->ShowWindow(FALSE);
	}

	tabbtnstruct.commandid = _XDEF_INV_BASETAB_CASHBACKPACK1;

	m_BaseTabButton[3] = new _XCheckButton;
	m_BaseTabButton[3]->Create( tabbtnstruct );
    //m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
	//m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
	m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
	m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
	m_BaseTabButton[3]->SetButtonSwapMode(TRUE);
	m_BaseTabButton[3]->SetCheck( FALSE );
	InsertChildObject( m_BaseTabButton[3] );

    m_BaseTabButton[3]->ShowWindow(FALSE);
	
	tabbtnstruct.position.x += 36;
	tabbtnstruct.commandid++;

	m_BaseTabButton[4] = new _XCheckButton;
	m_BaseTabButton[4]->Create( tabbtnstruct );		
    //m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
	//m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
	m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
	m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
	m_BaseTabButton[4]->SetButtonSwapMode(TRUE);
	m_BaseTabButton[4]->SetCheck( FALSE );
	InsertChildObject( m_BaseTabButton[4] );

    m_BaseTabButton[4]->ShowWindow(FALSE);
#else
    _XCHECKBTN_STRUCTURE tabbtnstruct = { TRUE, { 7, 23 }, { 42, 42 }, 
		_XDEF_INV_BASETAB_POCKET,
		inventoryimageindex,
		inventoryimageindex,
		&g_MainInterfaceTextureArchive };
	
	m_BaseTabButton[0] = new _XCheckButton;
	m_BaseTabButton[0]->Create( tabbtnstruct );		
	m_BaseTabButton[0]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 138,  247, 180 );
	m_BaseTabButton[0]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  205, 181,  247, 223 );
	m_BaseTabButton[0]->SetButtonSwapMode(TRUE);
	m_BaseTabButton[0]->SetCheck( TRUE );
	InsertChildObject( m_BaseTabButton[0] );
	
	tabbtnstruct.position.x += 43;
	tabbtnstruct.commandid++;
	
	for( int i = 1; i < 3; i++ )
	{			
		m_BaseTabButton[i] = new _XCheckButton;
		m_BaseTabButton[i]->Create( tabbtnstruct );		
		m_BaseTabButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 205, 97,  247, 138 );
		m_BaseTabButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  205, 97,  247, 138 );
		m_BaseTabButton[i]->SetButtonSwapMode(TRUE);
		m_BaseTabButton[i]->SetCheck( i == 0 );
		InsertChildObject( m_BaseTabButton[i] );
		
		tabbtnstruct.position.x += 43;
		tabbtnstruct.commandid++;
	}

	tabbtnstruct.imageindex_checked = g_MainInterfaceTextureArchive.FindResource( "mi_npc_present.tga" );
	tabbtnstruct.imageindex_normal = g_MainInterfaceTextureArchive.FindResource( "mi_npc_present.tga" );

	m_BaseTabButton[3] = new _XCheckButton;
	m_BaseTabButton[3]->Create( tabbtnstruct );		
	m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 178, 143, 220, 184 );
	m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  178, 97,  220, 138 );
	m_BaseTabButton[3]->SetButtonSwapMode(TRUE);
	m_BaseTabButton[3]->SetCheck( FALSE );
	m_BaseTabButton[3]->ShowWindow( FALSE );
	InsertChildObject( m_BaseTabButton[3] );
#endif

	_XBTN_STRUCTURE cashitemstruct = { TRUE, { 136, 23 }, { 42, 42 }, _XDEF_INV_CASHITEMBUTTON,
		g_MainInterfaceTextureArchive.FindResource( "MI_Cash_01.tga" ),
		g_MainInterfaceTextureArchive.FindResource( "MI_Cash_01.tga" ),
		g_MainInterfaceTextureArchive.FindResource( "MI_Cash_01.tga" ),
		&g_MainInterfaceTextureArchive };
	m_CashItemButton = new _XButton;
	m_CashItemButton->Create( cashitemstruct );
	m_CashItemButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  2, 2, 44, 44);
	m_CashItemButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  48, 2, 90, 44);
	m_CashItemButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  94, 2, 136, 44);
#ifdef _XTS_CASHITEM
	m_CashItemButton->ShowWindow(TRUE);
#else
	m_CashItemButton->ShowWindow(FALSE);
#endif
	InsertChildObject( m_CashItemButton );

	g_InventoryIconImage.Create( 0, 0, 31, 31, &g_MainInterfaceTextureArchive, 0 );
	g_InventoryIconImage.SetClipRect( 0, 0, 32, 32 );

	g_InventoryGradeIconImage.Create( 0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_grade_01.tga" ) );
	g_InventoryGradeIconImage.SetClipRect( 1, 1, 31, 31 );
	
	// �����ݾ� ��ư
	_XBTN_STRUCTURE moneybtnstruct = 
	{
		TRUE, {14, 244}, {15, 12},
			_XDEF_INV_INVENTORYMONEY,
			inventoryimageindex,
			inventoryimageindex,
			inventoryimageindex,
			&g_MainInterfaceTextureArchive
	};
	m_btnMoney = new _XButton;
	m_btnMoney->Create(moneybtnstruct);
	m_btnMoney->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 185, 6, 200, 18);
	m_btnMoney->SetButtonImageClipRect(_XBUTTONIMAGE_READY,  185, 6, 200, 18);
	m_btnMoney->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK,  185, 6, 200, 18);
	m_btnMoney->EnableWindow(TRUE);
	InsertChildObject(m_btnMoney);

	_XTOOLTIP_STRUCTURE itemtooltipstruct = 
	{
		{ 0, 0 },
		{ 78, 13 },
		{ 1, 1 },
		-1,
		NULL
	};	

#ifdef _XDEF_WASTEBASKET_20070226
	m_pImageWasteBasket = new _XImageStatic;
	//pImageWasteBasket->Create( 106, 242, 121, 257, &g_MainInterfaceTextureArchive, inventoryimageindex );
	//pImageWasteBasket->SetClipRect( 184, 234, 199, 249 );
	m_pImageWasteBasket->Create( 154, 238, 178, 262, &g_MainInterfaceTextureArchive, inventoryimageindex2 );
	m_pImageWasteBasket->SetClipRect( 0, 81, 24, 105); // �⺻..���
	
	InsertChildObject( m_pImageWasteBasket );
#endif
	//2004.06.22->oneway48 insert
	m_GrabItemImage.Create( m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX -48, 
							 m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY -48, 
							 m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX + 48, 
							 m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY + 48,
							 &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "type1_256.tga" ) );	
	m_GrabItemImage.SetClipRect( 0, 0, 64, 64  );
	m_GrabItemImage.SetScale(2.0f,2.0f);

	m_pClanPointImage = new _XImageStatic;
	m_pClanPointImage->Create(0, 0, 13, 16, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_main_back01.tga"));
	m_pClanPointImage->SetClipRect(202, 3, 215, 19);

	ShowWindow( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWINVENTORY, FALSE, TRUE ) );
		
	return TRUE;
}

void _XWindow_Inventory::SetDefaultPosition( void )
{	
	if( m_bLockInventory )
		return;

	SetExpandDockingBorderFlag(TRUE);	
	m_ExpandDockingBorder->ShowWindow( TRUE );
	MoveWindow( gnWidth-183, gnHeight-336-10 );

	_XWindow_SystemMsgDefault* SystemMsgWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);
	if(m_ShowWindow)
	{
		if( m_bExpandDockingBorderFlag )
		{
			if( SystemMsgWindow )
				SystemMsgWindow->SetDockingMode(TRUE);
		}
	}
	else
	{		
		if( SystemMsgWindow )
			SystemMsgWindow->SetDockingMode(FALSE);
	}
}

void _XWindow_Inventory::SaveWindowProperty( void )
{
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWINVENTORY, GetShowStatus() );
}


void _XWindow_Inventory::DestroyWindow( void )
{
	ResetMouseSlot();

	// 2004.06.15->oneway48 insert
/*	list <_XUsingItem>::iterator iter_item;
	for(iter_item = m_UsingItemList.begin() ; iter_item != m_UsingItemList.end() ; )
	{
		iter_item = m_UsingItemList.erase(iter_item);
	}
	m_UsingItemList.clear();
	// insert end
*/
	_XWindow::DestroyWindow();
}

void _XWindow_Inventory::Draw(_XGUIObject*& pfocusobject)
{
	if(!this->m_ShowWindow)	return;

	if(this->m_ShowWindow)
	{
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB( 128, 255,255,255 ));
	}

	_XWindow::Draw(pfocusobject);
	
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��
	int count = _XDEF_INVENTORY_ONEBACKPACKCOUNT+ (m_SelectedBaseTab * _XINVENTORY_SLOTTYPE_POCKETSTART); //���� ���õ� �ǿ� ���� ��ġ

	int columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
	int rowpoint	= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;	

	for(int row = 0; row < _XDEF_INVENTORYSLOT_HEIGHT; row++)
	{
		for(int column = 0; column < _XDEF_INVENTORYSLOT_WIDTH; column++)
		{
			if(count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT) break;

			if(g_pLocalUser->m_UserItemList[count].Get_m_sID() > 0)//&& count != m_MouseSlotItem.m_InventoryNumber)
			{
				m_ItemEdgeIcon[_XITEMEDGEICONTYPE_NORMAL].Draw( columnpoint-2, rowpoint-2 );

                if( m_SelectedBaseTab == 3 ) 
                {
                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 ) //�೶�� �ִ�.
                        DrawIcon(TRUE, columnpoint, rowpoint, &g_pLocalUser->m_UserItemList[count]);
                    else
                        DrawIcon(TRUE, columnpoint, rowpoint, &g_pLocalUser->m_UserItemList[count], TRUE, FALSE); // �������� ��� ���ϰ�
                }
                else if ( m_SelectedBaseTab == 4 )
                {
                    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() > 0 ) //�೶�� �ִ�.
                        DrawIcon(TRUE, columnpoint, rowpoint, &g_pLocalUser->m_UserItemList[count]);
                    else
                        DrawIcon(TRUE, columnpoint, rowpoint, &g_pLocalUser->m_UserItemList[count], TRUE, FALSE); // �������� ��� ���ϰ�
                }
                else
                    DrawIcon(TRUE, columnpoint, rowpoint, &g_pLocalUser->m_UserItemList[count]);

				
			}
			else
			{
				m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].Draw( columnpoint-2, rowpoint-2 );
			}

			columnpoint+=33;
			count++;			
		}
		
		if(count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT) break;

		columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
		rowpoint += 33;
	}
#else
	switch( m_SelectedBaseTab ) 
	{
	case _XINVENTORYBASETAB_POCKET	: DrawPocketList();
		break;
	case _XINVENTORYBASETAB_BACKPACK: DrawBackPackList();
		break;
	case _XINVENTORYBASETAB_BACKPACK2	: DrawPassList();
		break;	
	case _XINVENTORYBASETAB_CHARINFO : 
		{
			DrawCharEquipList();
			g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 255, 196, 54  ) );
			g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 204, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_INVENTORY_2114) );		
			g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 220, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_INVENTORY_2115) );
		}
		break;
	}
#endif

	DrawSelectedItem();
	DrawSocketItem();
	DrawRepairItem();
	DrawMixWeaponItem();
	DrawNPCResource_New_Item();

	if( m_bDrawGrabItem )
		DrawGrabItem();

	TCHAR moneyStr[128];
	memset( moneyStr, 0, sizeof(moneyStr));

	if(m_nPriceMode == _MODE_CLANPOINT)
	{
		m_pClanPointImage->Draw(m_WindowPosition.x+14, m_WindowPosition.y+243);
		sprintf(moneyStr, "%u", g_pLocalUser->m_CharacterInfo.Get_iContribution());
	}
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
	else if( m_nPriceMode == _MODE_MNDPOINT )
	{
		//���⵵ ���� �ؾ� �Ѵ�
		m_pClanPointImage->Draw(m_WindowPosition.x+14, m_WindowPosition.y+243);
		sprintf(moneyStr, "%u", g_pLocalUser->m_CharacterInfo.Get_iContribution());
	}
#endif
	else
	{
		sprintf(moneyStr, "%u", g_pLocalUser->m_Money);
	}

	int colorindex = 0;

	if( _XGameItem::GetMoneyColor(moneyStr, colorindex) )
	{
		g_XBaseFont->SetColor( g_MoneyColor[colorindex] );
		g_XBaseFont->PrintAlign(m_WindowPosition.x+100, m_WindowPosition.y+245, 1.0f, _XFONT_ALIGNTYPE_RIGHT, moneyStr );
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFEFFFFF );
		g_XBaseFont->PrintAlign(m_WindowPosition.x+100, m_WindowPosition.y+245, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%u", g_pLocalUser->m_Money);
	}
	
	g_XBaseFont->Flush();
	
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
	g_XBaseFont->SetColor(0xFFFFFFFF);
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 3, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_INVENTORY_2116) );
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1)+1, m_WindowPosition.y + 3, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_INVENTORY_2116) );
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
}

int	_XWindow_Inventory::SearchSlotRegion(  RECT* pRect  )
{
	int selectedslot	= -1;
	ScrnPos* scrnpos	= gpInput->GetMouseVirScreenPos();
	int count			= _XINVENTORY_SLOTTYPE_AMOR;
	int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
	int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;	
	
	// �ָӴ� ����Ʈ �˻�
	if( selectedslot == -1 )
	{
		switch( m_SelectedBaseTab ) 
		{
		case _XINVENTORYBASETAB_POCKET	: count = _XINVENTORY_SLOTTYPE_POCKETSTART;
			break;
		case _XINVENTORYBASETAB_BACKPACK: count = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART;
			break;
		case _XINVENTORYBASETAB_BACKPACK2	: count = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART;
			break;
	#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��
		case _XINVENTORYBASETAB_CASHBACKPACK1 : count = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART;
			break;
		case _XINVENTORYBASETAB_CASHBACKPACK2 : count = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART;
			break;
	#endif
		}
				
		for( int row = 0; row < _XDEF_INVENTORYSLOT_HEIGHT; row++ )
		{
			for( int column = 0; column < _XDEF_INVENTORYSLOT_WIDTH; column++ )
			{
				if( count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT ) break;
				
				if( ((scrnpos->x > columnpoint ) && (scrnpos->x < columnpoint+32) &&
					 (scrnpos->z > rowpoint )    && (scrnpos->z < rowpoint+32) ) )
				{
					selectedslot = count;
					
					if( pRect )
					{
						pRect->left   = columnpoint;
						pRect->top    = rowpoint;
						pRect->right  = columnpoint + 30;
						pRect->bottom = rowpoint    + 30;
					}

					break;
				}				
				
				columnpoint+=33;
				count++;			
			}
			
			if( selectedslot != -1 ) break;
			if( count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT ) break;
			
			columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
			rowpoint += 33;
		}
	}
	
#ifndef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��
	// ĳ���� ��� ������ ó��
	if( m_SelectedBaseTab == _XINVENTORYBASETAB_CHARINFO )
	{
		if( selectedslot >= _XINVENTORY_SLOTTYPE_POCKETSTART )
		{
			selectedslot = -1;						
		}
	}
#endif

	// ���� ������ ó��
	if( m_pSocketPlugIn_Window )
	{
		if( m_pSocketPlugIn_Window->GetShowStatus() )
		{
			for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT+1; i ++ )
			{
				if( m_iSocketIndexList[i] == selectedslot )
					return -1;
			}
			if( m_UsedLifeItemIndex == selectedslot )
				return -1;
		}	
	}
	// �׺�ڽ� ������ ó��
	if( m_pGamble_Window )
	{
		if( m_pGamble_Window->GetShowStatus() )
		{
			for( int i = 0; i < _XDEF_GAMBLEBOX_MAXCOUNT+1; i++ )
			{
				if( m_iSocketIndexList[i] == selectedslot )
				{
					if( i == 0 || g_pLocalUser->m_UserItemList[selectedslot].Get_m_ucCount() == 0 )
						return -1;
				}
			}			
		}
	}
	// ���� ������ ó��
	if( m_pRepairItem_Window )
	{
		if( m_pRepairItem_Window->GetShowStatus() )
		{
			list<int>::iterator iter = m_pRepairItem_Window->m_SelectedSlotNoList.begin();
			for( ; iter != m_pRepairItem_Window->m_SelectedSlotNoList.end(); iter++ )
			{
				if( *iter == selectedslot )
					return -1;
			}		
		}
	}

	if( m_pInchant_Window )
	{
		if( m_pInchant_Window->GetShowStatus() )
		{
			if( m_iSocketIndexList[0] == selectedslot )
				return -1;
			if( m_UsedLifeItemIndex == selectedslot )
				return -1;
		}
	}

#ifdef _XDEF_SMELT_ITEM // Author : �����
	XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
	if( pSmletItemWindow )
	{
		if( pSmletItemWindow->GetShowStatus() )
		{
			if( m_iSocketIndexList[0] == selectedslot )
				return -1;
			if( m_UsedLifeItemIndex == selectedslot )
				return -1;
		}
	}
#endif

#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : �����
	_XWindow_SocketDetach* pSocketDetachWindow = (_XWindow_SocketDetach*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETDETACH_WINDOW);

	if( pSocketDetachWindow && pSocketDetachWindow->GetShowStatus() )
	{
		for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT+1; i ++ )
		{
			if( m_iSocketIndexList[i] == selectedslot )
				return -1;
		}
		if( m_UsedLifeItemIndex == selectedslot )
			return -1;
	}	
#endif

	return selectedslot;
}

#ifdef _XDWDEBUG

#include "SoundEffectList.h"
extern void ChangeModel( DWORD lparam, char firstType, char facemaskmode = 0 );

void SingleModeChangeItem( char cFrom, char cTo )
{
	_XUserItem temp = g_pLocalUser->m_UserItemList[ cFrom ];
	g_pLocalUser->m_UserItemList[ cFrom ] = g_pLocalUser->m_UserItemList[ cTo ];
	g_pLocalUser->m_UserItemList[ cTo ] = temp;

	_XWindow_Inventory* pInventory_Window = (_XWindow_Inventory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INVENTORY);
	if(pInventory_Window)
	{
		if( pInventory_Window->m_MouseSlotItem.Get_m_sID() > 0 )
		{				
			_XPlayInterfaceSound( ID_SR_INTERFACE_CHOICEICON01_WAV );
		}
		else
		{
			_XPlayInterfaceSound( ID_SR_INTERFACE_PICK_ITEM_001_WAV );
		}

		if( cFrom >= _XINVENTORY_SLOTTYPE_AMOR && cFrom <= _XINVENTORY_SLOTTYPE_ACCESSORY_8 )
		{
			DWORD lparam = (cFrom * 100000) + g_pLocalUser->m_UserItemList[cFrom].Get_m_sID();
			ChangeModel( lparam, g_pLocalUser->m_UserItemList[cFrom].Get_m_cType() );					
		}

		if( cTo >= _XINVENTORY_SLOTTYPE_AMOR && cTo <= _XINVENTORY_SLOTTYPE_ACCESSORY_8 )
		{
			DWORD lparam = (cTo * 100000) + g_pLocalUser->m_UserItemList[cTo].Get_m_sID();
			ChangeModel( lparam, g_pLocalUser->m_UserItemList[cTo].Get_m_cType() );
		}

		
		g_pInventory_Window->ResetMouseSlot();
	}
}
#endif

BOOL _XWindow_Inventory::Process(_XGUIObject*& pfocusobject)
{
//	ProcessUsingItemTooltip( pfocusobject); // 2004.06.15->oneway48 insert

	if(!this->m_Enable)		return FALSE;
	if(!this->m_ShowWindow) 
	{		
		return FALSE;
	}

//Author : �����
#ifdef _XDEF_SMELT_ITEM
	XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
#endif

#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : �����
	_XWindow_SocketDetach* pSocketDetachWindow = (_XWindow_SocketDetach*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETDETACH_WINDOW);
#endif

	_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
	if( pTradeMessageBox )
	{
		if(pfocusobject == _XDEF_DUMMYID_INVENTORY) pfocusobject = NULL;

		if(!_XWindow::Process(pfocusobject)) return FALSE;
		if( pTradeMessageBox->GetShowStatus() ) return FALSE;
	}	
	_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
	if( pTradeConfirm_Window->GetShowStatus() )
	{
		if(pfocusobject == _XDEF_DUMMYID_INVENTORY) pfocusobject = NULL;
		if(!_XWindow::Process(pfocusobject)) return FALSE;
	}	

	if(m_bLockInventory)
	{
		if(m_pContribution_Window && m_pContribution_Window->GetShowStatus() )
		{
			m_btnMoney->EnableWindow(TRUE);
		}
		else if( m_pPCTradeWindow && m_pPCTradeWindow->GetShowStatus() )
		{
			m_btnMoney->EnableWindow(TRUE);
		}
		else if( m_pWarehouseWindow && m_pWarehouseWindow->GetShowStatus() )
		{
			m_btnMoney->EnableWindow(TRUE);
		}
		else
			m_btnMoney->EnableWindow(FALSE);
	}
	else
		m_btnMoney->EnableWindow(TRUE);

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	BOOL	bInTrade = FALSE;

	if(mousestate)
	{
		if( pfocusobject == (_XGUIObject*)this )
		{
			if( m_bDragState && !m_bHaveMouseSlot )
			{
				if( m_WindowPosition.x > gnWidth-253 && m_WindowPosition.y > gnHeight - 406 )
				{
					if( !m_bExpandDockingBorderFlag )
					{
						SetExpandDockingBorderFlag(TRUE);

						m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
						m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
						MoveWindow( gnWidth-183, gnHeight-336-10 );
						m_bDragState = FALSE;

						if( CheckMousePosition() )
						{	
							if( pfocusobject == (_XGUIObject*)this )
							{
								if( m_CommandID )
								{						
									PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CommandID, 0), (LPARAM)gHWnd);
									mousestate->bButton[0] = 0;
								}						
								pfocusobject = NULL;
							}		
						}
						if( pfocusobject == (_XGUIObject*)this )
						{
							pfocusobject = NULL;
						}
					}
				}
				else
				{
					SetExpandDockingBorderFlag(FALSE);					
				}
			}
		}
		
		if(!pfocusobject)  
		{
			//Author : ����� //bref : ������ ��� Ȯ��
		#ifdef _XDEF_WASTEBASKET_20070226
			if( !m_bWasteBasketLock ) //������ �� �̹����� �����Ǿ� ����
			{
				if( m_bHaveMouseSlot && m_pImageWasteBasket->CheckMousePosition() ) //Author : ����� //breif : �ٲ���
					m_pImageWasteBasket->SetClipRect( 24, 81, 48, 105); // �ٲ� ������ ���
				else
					m_pImageWasteBasket->SetClipRect( 0, 81, 24, 105); // �⺻..���
			}
		#endif

			if(mousestate->bButton[0])
			{
				if(CheckMousePosition())
				{
					if(g_pLocalUser->m_bMatchPRMode) // �� ȫ���϶� �޼��� ó�� ����.if(g_pLocalUser->m_bMatchPRMode)
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2117), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
					else
					{
						// Focus
						if( scrnpos->x > m_WindowPosition.x+10 && scrnpos->x < m_WindowPosition.x+173 &&		 // Pocket area
							scrnpos->z > m_WindowPosition.y+70 && scrnpos->z < m_WindowPosition.y+233 )
						{
							if( !pfocusobject || pfocusobject == this ) 
								pfocusobject = _XDEF_DUMMYID_INVENTORY;
						}
						else
						{
							m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
							m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
						}
						

						bInTrade |= ProcessNPCTrade();
						bInTrade |= ProcessFromWarehouse();
						bInTrade |= ProcessPCTrade();
						bInTrade |= ProcessRepairItem(); 
						bInTrade |= ProcessFromSocketPlugIn();
						bInTrade |= ProcessFromPocket();
						bInTrade |= ProcessGamble();
#ifdef _XDEF_WASTEBASKET_20070226
						bInTrade |= ProcessWasteBasket();
#endif

						if(!bInTrade)
						{
							if(!m_bHaveMouseSlot && !g_pLocalUser->m_bReadingBook && !g_pLocalUser->m_bInternalUse )
							{
								// ���콺 down ��ġ ����
								m_IconDragStartPos.x = scrnpos->x;
								m_IconDragStartPos.y = scrnpos->z;
								m_LastDraggingSlot = SearchSlotRegion();
								m_bDragState = TRUE;
							}
							else
							{
								// click �� ��ġ slot ����
								m_LastDraggingSlot = SearchSlotRegion();
							}
						} // if(!bInTrade)

						// �ٸ� â�� �����ִ� tooltip �ݱ�
						if( g_pDefaultTooltip_Window )
						{
							if( g_pDefaultTooltip_Window->GetShowStatus() )
								g_pDefaultTooltip_Window->ShowWindow(FALSE);
						}
					} //if(g_pLocalUser->m_bMatchPRMode)
				} // if(CheckMousePosition())
				else
				{
					// �κ��丮 ���� Ŭ����
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )						
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
					}					

					//������ ó��
					if( m_pNPCTradeWindow && m_pNPCTradeWindow->GetShowStatus() && m_pNPCTradeWindow->CheckMousePosition())
					{
						// NPC �ŷ��ÿ��� �ŷ�â �����̸� ó���� ���� ����.
					}
					else if( m_pPCTradeWindow && m_pPCTradeWindow->GetShowStatus() && m_pPCTradeWindow->CheckMousePosition())
					{
						// PC �ŷ��ÿ��� �ŷ�â �����̸� ó���� ���� ����.
					}
					else if( m_pWarehouseWindow && m_pWarehouseWindow->GetShowStatus() && m_pWarehouseWindow->CheckMousePosition())
					{
						// â�� ����ÿ��� â��â �����̸� ó���� ���� ����.
					}
					else if( m_pEnterBossRoom_Window && m_pEnterBossRoom_Window->GetShowStatus() && m_pEnterBossRoom_Window->CheckMousePosition())
					{
						// ������ ���Խÿ��� ������ ����â �����̸� ó���� ���� ����.
					}
					else if( m_pRepairItem_Window && m_pRepairItem_Window->GetShowStatus() && m_pRepairItem_Window->CheckMousePosition() )
					{
						// �����ÿ��� ó���� ���� ����.
					}
					else if( m_pSocketPlugIn_Window && m_pSocketPlugIn_Window->GetShowStatus() && m_pSocketPlugIn_Window->CheckMousePosition() )
					{
						// ���� ������ ó������ ����
					}
					else if( m_pGamble_Window && m_pGamble_Window->GetShowStatus() && m_pGamble_Window->CheckMousePosition() )
					{
						// ���� �׺�� ó������ ����
					}
					else if(g_pQuickSlot_Window && g_pQuickSlot_Window->CheckMousePosition() )
					{
						// �����Կ� ������ ������ ���� ó������ ����
					}
					else if( g_pCInfo_Window->CheckEquipedArea() )
					{
						// ĳ���� ����â ����������� ó�� ����.
					}
					else if( m_pContribution_Window && m_pContribution_Window->GetShowStatus() && m_pContribution_Window->CheckMousePosition())
					{
						// ���ı⿩�� â �̸� ó������ ����.
					}
					else if( m_pPersonalStore_Window && m_pPersonalStore_Window->GetShowStatus() && m_pPersonalStore_Window->CheckMousePosition())
					{
						// ���� ���� â �̸� ó������ ����.
					}
					else if( m_pTradeConfirm_Window && m_pTradeConfirm_Window->GetShowStatus() )
					{
						// ����â�� �� ������ ó������ ����.
					}
                #ifdef _XTS_CASHITEM
					else if( m_pCashItem_Window && m_pCashItem_Window->GetShowStatus() )
					{
						// ĳ��������â�� �� ������ ó������ ����.
					}
                   #endif
					else if( g_pLifeSkill_Window && g_pLifeSkill_Window->GetShowStatus() && g_pLifeSkill_Window->CheckMousePosition() )
					{
						// ��Ȱ â�� â�� �� ������ ó������ ����.
					}
					else if( m_pInchant_Window && m_pInchant_Window->GetShowStatus() )
					{
						// ����â�� �� ������ ó������ ����.
					}					
                #ifdef _XDEF_SMELT_ITEM //Author : �����
					else if( pSmletItemWindow && pSmletItemWindow->GetShowStatus() && pSmletItemWindow->CheckMousePosition() )
					{
						// ��� �븰 â�� �������� ó����������.
					}
                #endif
                #ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : �����
					else if(  pSocketDetachWindow && pSocketDetachWindow->GetShowStatus() && pSocketDetachWindow->CheckMousePosition() )
					{
						// ���� ���� â�� �������� ó����������
					}
                #endif
                #ifdef _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI
					else if(m_pNPCResource_New_Window && m_pNPCResource_New_Window->GetShowStatus() && m_pNPCResource_New_Window->CheckMousePosition())
					{
						// ������ â�� �� ������ ó������ ����
					}
                #endif
            	//Author : ����� //breif : Ȥ�� ���� ���� �־
                #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
                    else if( m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
		                     m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
                             g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0)
	                {
	                }
                    else if( m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		                    m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_TOTALCOUNT &&
                            g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
	                {
                    }
                #endif
                    // ������� //Author : ����� //breif : 08.04.23
                    else if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING && g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_MEDITATION )
                    {
                    }
					else if( g_pLocalUser->m_bVirtualMode )
					{
						// ������� ����̸� ó������ ����.
						if( m_bHaveMouseSlot )
						{
							ResetMouseSlot();
							m_bDragState = FALSE;
							m_LastDraggingSlot = -1;//ID_STRING_NETWORK_1607
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_1607), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "������ ��ǰ�� ������ �����ϴ�."
						}
					}
					else
					{
						
						_XWindow_AddSaveItem* pAddSaveitem_Window = (_XWindow_AddSaveItem*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_ADDSAVEITEM );
						if(pAddSaveitem_Window && pAddSaveitem_Window->GetShowStatus())
						{
						}
						else
						{
							if( m_bHaveMouseSlot )
							{
								if(!ProcessItemDrop())
								{	
									if( !m_pRepairItem_Window || !m_pRepairItem_Window->GetShowStatus())
										m_MouseSlotItem.ResetMouseSlotItem();
									
									m_bHaveMouseSlot = FALSE;
									m_LastDraggingSlot = -1;							
								}				
							}							
						}
					}
				}
			}
			if(mousestate->bButton[1])
			{
				// ������ Ŭ��
				if(CheckMousePosition())
					m_WindowMouseState = _XW_STATE_RIGHTCLICK; 
				else
				{
					if(m_bHaveMouseSlot)
					{
						ResetMouseSlot();
					}
				}
			}
		}
		if(pfocusobject == _XDEF_DUMMYID_INVENTORY)
		{
			if(mousestate->bButton[0])
			{
				if( m_pWarehouseWindow )
				{
					if( m_pWarehouseWindow->GetShowStatus() )
						m_pWarehouseWindow->m_MoveButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_RECEIVE);	// "�ñ��"
				}
				
				// �巡�� ó��
				if(m_bDragState && !m_bHaveMouseSlot)
				{
					if(scrnpos->x != m_IconDragStartPos.x || scrnpos->z != m_IconDragStartPos.y)
					{
						if(m_LastDraggingSlot != -1)
						{
							if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0)
							{
								if(m_bLockInventory && m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_POCKETSTART)
								{
								}
							#ifndef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��
								else if( m_SelectedBaseTab == _XINVENTORYBASETAB_CHARINFO )
								{
								}
							#endif
								//Author : ����� //breif : ��� �߿� �巡�� �� �����Ѵ�
								//g_pLocalUser->SetAnimation(_XACTION_IDLE); ���� m_HiperRunFailedFlag �� -1 �� ����
								//�׷��� ProcessSpeedSkill() ���� ���ϰ� �����̰� ���� ���� �ʴ´�
								else if(  gpInput->CheckKeyPress(DIK_LMENU) && 
										   g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
										  g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
								{
								}
								//Author : ����� //breif : ĳ���೶ �߰� �Ǹ鼭 ������ �˻� ���� �ϸ� �ȵ�
                            #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
                                else if( m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
		                                 m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
                                         g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0)
	                            {
	                            }
                                else if( m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		                                m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_TOTALCOUNT &&
                                        g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
	                            {
                                }
                            #endif
								else
								{
									if( m_pWarehouseWindow )
										m_pWarehouseWindow->ReleaseTempItemList();
									m_bHaveMouseSlot = TRUE;
									m_MouseSlotItem.Copy(g_pLocalUser-> m_UserItemList[m_LastDraggingSlot]);									
									m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
									m_LastDraggingSlot = -1;
									
									// 2004.05.31->oneway48 insert : �巡�� ���۽� ������ ���� ����.							
									if( g_pDefaultTooltip_Window )
									{
										if( g_pDefaultTooltip_Window->GetShowStatus() )						
											g_pDefaultTooltip_Window->ShowWindow( FALSE );
									}
									
									// �巡�׸� �Ҷ� ���ڸ� �ٱ⿡ ���� ����ó��
									if(g_pLocalUser->GetMotionClass() == _XACTION_MOVE)
									{
										// ����ó�� - ���ڸ� �ٱ� ��
										g_pLocalUser->SetAnimation(_XACTION_IDLE);
										g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
									}
								}
							} // if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0)
						}
					}
				} // if(m_bDragState && !m_bHaveMouseSlot)
			}
			else //if(!mousestate->bButton[0])
			{
				m_bDrawSelectedItem = TRUE;

				if(gpInput->CheckKeyPress(DIK_LCONTROL))
				{
					if(scrnpos->x == m_IconDragStartPos.x && scrnpos->z == m_IconDragStartPos.y)
					{
						if( m_pWarehouseWindow )
						{
							if( m_pWarehouseWindow->GetShowStatus() )
							{
								/*if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].m_cType != _XGI_FC_QUEST )	
								{
									if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0)
									{									
										m_pWarehouseWindow->m_bProcessOneClickItem = FALSE;
										if(m_pWarehouseWindow->HaveItemFromInven(m_LastDraggingSlot))
										{
											// ���� �ִ� ��
											m_pWarehouseWindow->ResetTempItemFromInven(m_LastDraggingSlot);
										}
										else
										{
											m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
											m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
											m_pWarehouseWindow->SetTempItemFromInven(m_LastDraggingSlot, m_MouseSlotItem.m_ucCount);
										}
										m_LastDraggingSlot = -1;									
									} // if(m_pWarehouseWindow->GetShowStatus())
								}*/
								//Author : ����� //breif : ����
								if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_QUEST ) // ����Ʈ ������ ���� X
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTSAVEQUEST), TRUE);
								}
#ifdef _XDEF_WAREHOSE_RESTRAINT_OF_TRADE //Author : ����� //breif : â��ŷ� ����
								else if( _XGameItem::CheckItemTempField4( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() ,
															g_pLocalUser->m_UserItemList[m_LastDraggingSlot ].Get_m_sID() ) )
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3404), TRUE); //�ش� �������� â�� ������ �� �����ϴ�.
								}
#endif
								else
								{
									if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0)
									{									
										m_pWarehouseWindow->m_bProcessOneClickItem = FALSE;
										if(m_pWarehouseWindow->HaveItemFromInven(m_LastDraggingSlot))
										{
											// ���� �ִ� ��
											m_pWarehouseWindow->ResetTempItemFromInven(m_LastDraggingSlot);
										}
										else
										{
											m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
											m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
											m_pWarehouseWindow->SetTempItemFromInven(m_LastDraggingSlot, m_MouseSlotItem.Get_m_ucCount());
										}
										m_LastDraggingSlot = -1;									
									} // if(m_pWarehouseWindow->GetShowStatus())
								}
								//Last Updated : 07/04/23
							}						
						}
					} //if(scrnpos->x == m_IconDragStartPos.x && scrnpos->z == m_IconDragStartPos.y)
				} // if(gpInput->CheckKeyPress(DIK_LCONTROL))
				else
				{
					if(scrnpos->x == m_IconDragStartPos.x && scrnpos->z == m_IconDragStartPos.y)
					{					
						// ���ڸ����� Ŭ�� = ���� ����
						if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0)
						{
							if(m_pWarehouseWindow )
							{
								if( m_pWarehouseWindow->GetShowStatus())
								{
									m_pWarehouseWindow->ReleaseTempItemList();
									m_pWarehouseWindow->SetTempItemFromInven(m_LastDraggingSlot, m_MouseSlotItem.Get_m_ucCount());
									m_pWarehouseWindow->m_bProcessOneClickItem = TRUE;
								}
							}

							m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
							m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;							
							if( g_pDefaultTooltip_Window )
							{
								if(m_pNPCTradeWindow )
								{
									if( m_pNPCTradeWindow->GetShowStatus())
									{
										int type = g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType();
										int id = g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID();

										if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_NPCTRADE_INVENITEM, m_LastDraggingSlot, type, id ) )
										{
											g_pDefaultTooltip_Window->ShowWindow( TRUE );
										}
									}
									else
									{
										int type = g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType();
										int id = g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID();
										if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_DEFAULTITEM, m_LastDraggingSlot, type, id ) )									
										{
											g_pDefaultTooltip_Window->ShowWindow( TRUE );
										}
									}
								}
								else
								{
									int type = g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType();
									int id = g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID();
									if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_DEFAULTITEM, m_LastDraggingSlot, type, id ) )									
									{
										g_pDefaultTooltip_Window->ShowWindow( TRUE );
									}
								}
							}
							m_LastDraggingSlot = -1;
							m_bHaveMouseSlot = FALSE;
						}
						else
						{
							if(m_pWarehouseWindow )
							{
								if( m_pWarehouseWindow->GetShowStatus())
									m_pWarehouseWindow->ReleaseTempItemList();
							}
							
							m_MouseSlotItem.ResetMouseSlotItem();
							m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
							// icon�� ���� slot Ŭ��
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )						
									g_pDefaultTooltip_Window->ShowWindow( FALSE );
							}
							m_LastDraggingSlot = -1;
							m_bHaveMouseSlot = FALSE;
						}
					}
					else
					{
						if(m_bDragState && m_bHaveMouseSlot)
						{
							m_LastDraggingBaseTab = m_SelectedBaseTab;
							int selectedslot = SearchSlotRegion();
							if(selectedslot == m_MouseSlotItem.m_InventoryNumber)
							{
								// drag�� ���¿��� �� slot��  ���������� ���� - Reset
								ResetMouseSlot();
								m_bDragState = FALSE;								
							}	
							else
							{
								m_bDragState = FALSE;
							}
						}

						if(m_bHaveMouseSlot)
						{
							ProcessInvenMove();
						}
					}
				}// ctrl
			} // if(!mousestate->bButton[0])
		} // if(pfocusobject == _XDEF_DUMMYID_INVENTORY)

		if(m_pRepairItem_Window)
		{
			if( m_pRepairItem_Window->GetShowStatus() )
			{
				if(m_pRepairItem_Window->CheckMousePosition())
				{					
					if(mousestate->bButton[0])
					{
						if( m_pRepairItem_Window->m_RepairMode != RM_UNDERREPAIR )
						{
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )						
									g_pDefaultTooltip_Window->ShowWindow( FALSE );		
							}
						}
					}
				}
			}
		}

		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				// ������ ��ư ó��
				BOOL	bInTrade = FALSE;

            #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
                int selectedslot = SearchSlotRegion();

				if( selectedslot >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
					selectedslot < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART )
				{
					if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
						return FALSE;
				}

                if( selectedslot >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		            selectedslot < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
	            {
		            if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
			            return FALSE;
	            }
            #endif

				bInTrade |= ProcessSellToNPC();
				bInTrade |= ProcessToWarehouse();
				bInTrade |= ProcessToPCTrade();
				bInTrade |= ProcessToPersonalStore();
				bInTrade |= ProcessToRepair();				
				bInTrade |= ProcessToSocketPlugIn();
				bInTrade |= ProcessToContribution();
				bInTrade |= ProcessToEnterBossRoom();
				bInTrade |= ProcessToGamble();
				bInTrade |= ProcessToSocketPlugInEx();
				bInTrade |= ProcessToInchant();
				bInTrade |= ProcessToSmeltItem(); //Author : �����
				bInTrade |= ProcessToSocketDetach(); //Author : �����
				bInTrade |= ProcessToMixWeapon();
				bInTrade |= ProcessToNPCResourceNew();
				bInTrade |= ProcessItemEquip();// ������ ��ü�� ���� �ؿ� �־�� �Ѵ�. �Ǹ������� ���Կ� �����ۿ��ΰ� FALSE�� �ȴ�.

				if(!bInTrade)
				{
					// Mouse slot ���
					ResetMouseSlot();
				}
				
				m_WindowMouseState = _XW_STATE_NONE;
				m_LastDraggingSlot = -1;
			}
		}

		if(!mousestate->bButton[0])
		{
			//������ ���� �� ���� ���� ���¿��� �˻縦 �Ѵ�. 
			if( !g_bShowItemMallWindow ) 
			{
				if(pfocusobject == _XDEF_DUMMYID_INVENTORY)
					pfocusobject = NULL;	
				
				m_bDragState = FALSE;
				if( m_bExpandDockingBorderFlag )
				{
					MoveWindow( gnWidth-183, gnHeight-336-10 );
					if( CheckMousePosition() )
					{	
						if( pfocusobject == (_XGUIObject*)this )
						{
							if( m_CommandID )
							{						
								PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CommandID, 0), (LPARAM)gHWnd);
								mousestate->bButton[0] = 0;
							}						
							pfocusobject = NULL;
						}		
					}
					if( pfocusobject == (_XGUIObject*)this )
					{
						pfocusobject = NULL;
					}
				}
			}
		}

		if( m_bHaveMouseSlot )
		{
			if( m_BaseTabButton[0]->GetShowStatus() && m_BaseTabButton[0]->CheckMousePosition() )
				ChangeBaseTab(_XINVENTORYBASETAB_POCKET);
			else if( m_BaseTabButton[1]->GetShowStatus() && m_BaseTabButton[1]->CheckMousePosition() )
				ChangeBaseTab(_XINVENTORYBASETAB_BACKPACK);
			else if( m_BaseTabButton[2]->GetShowStatus() && m_BaseTabButton[2]->CheckMousePosition() )
				ChangeBaseTab(_XINVENTORYBASETAB_BACKPACK2);
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��
			else if( m_BaseTabButton[3]->GetShowStatus() && m_BaseTabButton[3]->CheckMousePosition() )
				ChangeBaseTab(_XINVENTORYBASETAB_CASHBACKPACK1);
			else if( m_BaseTabButton[4]->GetShowStatus() && m_BaseTabButton[4]->CheckMousePosition() )
				ChangeBaseTab(_XINVENTORYBASETAB_CASHBACKPACK2);
		#endif
		}		
	}

	if(!_XWindow::Process(pfocusobject)) 
		return FALSE;

	return TRUE;
}

void _XWindow_Inventory::MoveWindow(int X, int Y)
{			
	/*
	if( m_ChildTooltipWindow && m_ChildTooltipWindow->GetShowStatus())
		{
			POINT position;
			
			POINT winpos = GetWindowPos();
			POINT tooltipposition = m_ChildTooltipWindow->GetWindowPos();
			SIZE tooltipsize = m_ChildTooltipWindow->GetWindowSize();
			
			position.x = tooltipposition.x;
			position.y = tooltipposition.y;
			
			if( tooltipposition.x + tooltipsize.cx >= gnWidth )
			{
				position.x = tooltipposition.x - 12 - tooltipsize.cx;				
			}
			if( tooltipposition.y + tooltipsize.cy >= gnHeight )
			{
				position.y = tooltipposition.y - 12 - tooltipsize.cy ;
			}
			if( tooltipposition.x < 0 )
			{
				position.x = tooltipposition.x + 12 + tooltipsize.cx;
			}
			if( tooltipposition.y < 0 )
			{
				position.y = tooltipposition.y + 12 + tooltipsize.cy ;
			}
			
			m_ChildTooltipWindow->MoveWindow( X + position.x - winpos.x, Y + position.y- winpos.y  );	
		}*/


	_XWindow::MoveWindow(X, Y);

	m_BorderTile.MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + 69, m_WindowPosition.x + 179, m_WindowPosition.y + 235 );	
	
	if( m_bExpandDockingBorderFlag )
	{
		m_ExpandDockingBorder->ShowWindow( TRUE );
		_XWindow_MainMenu* pMainmenuwindow = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
		g_MainWindowManager.SetTopWindow(pMainmenuwindow);		
	}
	else
	{
		m_ExpandDockingBorder->ShowWindow( FALSE );		
	}
}

void _XWindow_Inventory::ShowWindow(BOOL show)
{	
	_XWindow_SystemMsgDefault* SystemMsgWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);

	if(show)
	{
        SetTabButtonPosition();

		if( m_pNPCTradeWindow )
		{
			if( m_pNPCTradeWindow->GetShowStatus() )
			{

			#ifndef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
				m_BaseTabButton[3]->ShowWindow( TRUE );
			#endif
#ifdef _XTS_CASHITEM
				m_CashItemButton->ShowWindow(FALSE);
#endif
			}
		}
		if( m_pRepairItem_Window )
		{
			if( m_pRepairItem_Window->GetShowStatus() )
			{
			#ifndef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
				m_BaseTabButton[3]->ShowWindow( TRUE );
			#endif
#ifdef _XTS_CASHITEM
				m_CashItemButton->ShowWindow(FALSE);
#endif
			}
		}
		if( m_bExpandDockingBorderFlag )
		{
			if( SystemMsgWindow )
				SystemMsgWindow->SetDockingMode(TRUE);
		}
		else
		{
			if( SystemMsgWindow )
				SystemMsgWindow->SetDockingMode(FALSE);
		}
	}
	else
	{
		m_bDrawSelectedItem = FALSE;
		m_bWasteBasketLock = FALSE;

	#ifndef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
		m_BaseTabButton[3]->ShowWindow( FALSE );
	#endif
#ifdef _XTS_CASHITEM
		m_CashItemButton->ShowWindow(TRUE);
#endif
		ChangeBaseTab( _XINVENTORYBASETAB_POCKET );

		SetExpandDockingBorderFlag(FALSE);
		m_ExpandDockingBorder->ShowWindow( FALSE );
		/*if( SystemMsgWindow )
			SystemMsgWindow->SetDockingMode(FALSE);*/

		// mode ����
		SetPriceMode(_MODE_NORMAL);
	}

	
	
	_XWindow::ShowWindow(show);
}


void _XWindow_Inventory::ChangeBaseTab(int selectedtab)
{
	if( g_pDefaultTooltip_Window )
	{
		if( g_pDefaultTooltip_Window->GetShowStatus() )						
			g_pDefaultTooltip_Window->ShowWindow( FALSE );
	}

	switch(selectedtab) 
	{
	case _XINVENTORYBASETAB_POCKET :
			m_SelectedBaseTab = _XINVENTORYBASETAB_POCKET;
		break;
	case _XINVENTORYBASETAB_BACKPACK :
		{
			if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 )
				m_SelectedBaseTab = _XINVENTORYBASETAB_BACKPACK;
		}
		break;
	case _XINVENTORYBASETAB_BACKPACK2 :
		{
			//if( g_LanguageType != _XLANGUAGE_TYPE_JAPANESE ) //Author : ����� //breif : �೶ ���� ���߿� ĳ�� ���������� ��ü �Ѵٰ��� 08/01/31 //Date 08/03/27 ����
			{
				if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0 )
					m_SelectedBaseTab = _XINVENTORYBASETAB_BACKPACK2;
			}
		}
		break;
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	case _XINVENTORYBASETAB_CASHBACKPACK1 : //Author : ����� //breif : �೶Ȯ��
		{
			if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 || m_bIteminCashBackPack[0])
					m_SelectedBaseTab = _XINVENTORYBASETAB_CASHBACKPACK1;
		}
		break;
	case _XINVENTORYBASETAB_CASHBACKPACK2 :
		{
			if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() > 0 || m_bIteminCashBackPack[1])
					m_SelectedBaseTab = _XINVENTORYBASETAB_CASHBACKPACK2;
		}
		break;
#else
	case _XINVENTORYBASETAB_CHARINFO :
		{                 
			if( (m_pNPCTradeWindow && m_pNPCTradeWindow->GetShowStatus()) || 
				(m_pRepairItem_Window && m_pRepairItem_Window->GetShowStatus()) )
			{
				m_SelectedBaseTab = _XINVENTORYBASETAB_CHARINFO;
			}
		}
		break;
#endif
	}

	for( int i = 0; i < 5; i++ )
	{
		if( m_BaseTabButton[i] )
			m_BaseTabButton[i]->SetCheck(i == m_SelectedBaseTab);
	}
}

void _XWindow_Inventory::SetTabButtonPosition()
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰� 
    int nBasePosX = this->m_WindowPosition.x + 40; // �⺻ 4�̰�,, 2��° ������ ���� ��ġ�� �����ϹǷ� y �� 23 �̴�.. ������ ��� �ͼ��ؾ��ϴµ�,,

    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 )
    {
        m_BaseTabButton[1]->MoveWindow( nBasePosX, this->m_WindowPosition.y + 23);
        m_BaseTabButton[1]->ShowWindow(TRUE);
        nBasePosX += 36;
    }
    else
        m_BaseTabButton[1]->ShowWindow(FALSE);

    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0 )
    {
        m_BaseTabButton[2]->MoveWindow( nBasePosX, this->m_WindowPosition.y + 23);
        m_BaseTabButton[2]->ShowWindow(TRUE);
        nBasePosX += 36;
    }
    else
        m_BaseTabButton[2]->ShowWindow(FALSE);

    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 || m_bIteminCashBackPack[0])
    {
        m_BaseTabButton[3]->MoveWindow( nBasePosX, this->m_WindowPosition.y + 23);
        m_BaseTabButton[3]->ShowWindow(TRUE);
        nBasePosX += 36;
    }
    else
        m_BaseTabButton[3]->ShowWindow(FALSE);

    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() > 0 || m_bIteminCashBackPack[1])
    {
        m_BaseTabButton[4]->MoveWindow( nBasePosX, this->m_WindowPosition.y + 23);
        m_BaseTabButton[4]->ShowWindow(TRUE);
        nBasePosX += 36;
    }
    else
        m_BaseTabButton[4]->ShowWindow(FALSE);
#endif
}

void _XWindow_Inventory::DrawSelectedItem( void )
{	
	RECT iconrect = {0,0,0,0};
	int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
	int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
	int column = 0, row = 0;
	int selectedslot = 0;

	if(!m_bDrawSelectedItem)	// draw tooltip
	{
		selectedslot = SearchSlotRegion(&iconrect);
		
		if(selectedslot == -1 )
			return;

		if(g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID() > 0)
		{
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
			DrawItemTooltip(selectedslot, iconrect);
		}
		return;
	}

	BOOL selectedslotarray[_XINVENTORY_SLOTTYPE_TOTALCOUNT];
	memset(selectedslotarray, 0, sizeof(BOOL)*_XINVENTORY_SLOTTYPE_TOTALCOUNT);
	
	if( m_pRepairItem_Window && m_pRepairItem_Window->GetShowStatus() )
	{
		if(!m_pRepairItem_Window->m_SelectedSlotNoList.empty())
		{
			list <int>::iterator iter = m_pRepairItem_Window->m_SelectedSlotNoList.begin();
			for( ; iter != m_pRepairItem_Window->m_SelectedSlotNoList.end() ; iter++)
			{				
				int iconposition = *iter - (m_SelectedBaseTab * _XDEF_INVENTORY_ONEBACKPACKCOUNT +_XINVENTORY_SLOTTYPE_POCKETSTART);
				
				if(iconposition >= 0 && iconposition < _XDEF_INVENTORY_ONEBACKPACKCOUNT )
				{
					row = 33 * (iconposition/5);
					column = 33 * (iconposition%5);
					
					iconrect.left = columnpoint + column;
					iconrect.top = rowpoint + row;
					iconrect.right = columnpoint + column + 29;
					iconrect.bottom = rowpoint + row + 29;
					
					_XDrawRectAngle( iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT );
				}
			}
		}	
	}
	else if(m_pWarehouseWindow && m_pWarehouseWindow->GetShowStatus())
	{
	#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ�� 
		//�ּ� ���� ��ġ(25 + (m_SelectedBaseTab*25)) ����ġ (25 + (m_SelectedBaseTab*25)) + 25
		for( selectedslot = (25 + (m_SelectedBaseTab*25)); selectedslot < (25 + (m_SelectedBaseTab*25)) + 25 ; selectedslot++ )
		{	
			if(m_pWarehouseWindow->HaveItemFromInven(selectedslot))
			{
				int iconposition = selectedslot - (25 + (m_SelectedBaseTab*25));
				
				if(iconposition < _XDEF_INVENTORY_ONEBACKPACKCOUNT) // �κ��丮�� ũ�� 25ĭ�� ������ �׸��� �ʴ´�.
				{					
					row = 33 * (iconposition/5);
					column = 33 * (iconposition%5);
					
					iconrect.left = columnpoint + column;
					iconrect.top = rowpoint + row;
					iconrect.right = columnpoint + column + 29;
					iconrect.bottom = rowpoint + row + 29;
					
					_XDrawRectAngle( iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT );
					selectedslotarray[selectedslot] = TRUE;
				}
			}
		}
	#else //���� case �� ���� �ʿ䰡 ���µ�..
		switch( m_SelectedBaseTab ) 
		{
		case _XINVENTORYBASETAB_POCKET	: 
			{
				for( selectedslot = _XINVENTORY_SLOTTYPE_POCKETSTART; selectedslot < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART ; selectedslot++ )
				{
					if(m_pWarehouseWindow->HaveItemFromInven(selectedslot))
					{
						RECT iconrect;
						int iconposition = selectedslot - _XINVENTORY_SLOTTYPE_POCKETSTART;
						
						if(iconposition < _XDEF_INVENTORY_ONEBACKPACKCOUNT ) // �κ��丮�� ũ�� 25ĭ�� ������ �׸��� �ʴ´�.
						{					
							row = 33 * (iconposition/5);
							column = 33 * (iconposition%5);
							
							iconrect.left = columnpoint + column;
							iconrect.top = rowpoint + row;
							iconrect.right = columnpoint + column + 29;
							iconrect.bottom = rowpoint + row + 29;
							
							_XDrawRectAngle( iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT );
							selectedslotarray[selectedslot] = TRUE;
						}
					}
				}
			}
			break;
		case _XINVENTORYBASETAB_BACKPACK: 
			{
				for( selectedslot = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; selectedslot < _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART ; selectedslot++ )
				{	
					if(m_pWarehouseWindow->HaveItemFromInven(selectedslot))
					{
						int iconposition = selectedslot - _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART;
						
						if(iconposition < _XDEF_INVENTORY_ONEBACKPACKCOUNT) // �κ��丮�� ũ�� 25ĭ�� ������ �׸��� �ʴ´�.
						{					
							row = 33 * (iconposition/5);
							column = 33 * (iconposition%5);
							
							iconrect.left = columnpoint + column;
							iconrect.top = rowpoint + row;
							iconrect.right = columnpoint + column + 29;
							iconrect.bottom = rowpoint + row + 29;
							
							_XDrawRectAngle( iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT );
							selectedslotarray[selectedslot] = TRUE;
						}
					}
				}	
			}
			break;
		case _XINVENTORYBASETAB_BACKPACK2	: 
			{
				for( selectedslot = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; selectedslot < _XINVENTORY_SLOTTYPE_TOTALCOUNT ; selectedslot++ )
				{	
					if(m_pWarehouseWindow->HaveItemFromInven(selectedslot))
					{
						int iconposition = selectedslot - _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART;
						
						if(iconposition < _XDEF_INVENTORY_ONEBACKPACKCOUNT) // �κ��丮�� ũ�� 25ĭ�� ������ �׸��� �ʴ´�.
						{					
							row = 33 * (iconposition/5);
							column = 33 * (iconposition%5);
							
							iconrect.left = columnpoint + column;
							iconrect.top = rowpoint + row;
							iconrect.right = columnpoint + column + 29;
							iconrect.bottom = rowpoint + row + 29;
							
							_XDrawRectAngle( iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT );
							selectedslotarray[selectedslot] = TRUE;
						}
					}
				}
			}
			break;	
		}
	#endif
	}
	else if( m_pContribution_Window && m_pContribution_Window->GetShowStatus() )
	{
		if( m_ContributionItemInvenNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART )
		{
			if( m_ContributionItemInvenNumber >= (_XINVENTORY_SLOTTYPE_POCKETSTART+m_SelectedBaseTab*25) &&
				m_ContributionItemInvenNumber < (_XINVENTORY_SLOTTYPE_POCKETSTART+m_SelectedBaseTab*25)+25 )
			{
				if(g_pLocalUser->m_UserItemList[m_ContributionItemInvenNumber].Get_m_sID() > 0 )
				{
					if(!selectedslotarray[m_ContributionItemInvenNumber])
					{
						int iconposition = m_ContributionItemInvenNumber - _XINVENTORY_SLOTTYPE_POCKETSTART ;
						iconposition -= m_SelectedBaseTab * 25;
						
						row = 33 * (iconposition/5);
						column = 33 * (iconposition%5);
						
						iconrect.left = columnpoint + column;
						iconrect.top = rowpoint + row;
						iconrect.right = columnpoint + column + 29;
						iconrect.bottom = rowpoint + row + 29;
						
						_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, 0xFF01FE06);
					}
				}
			}			
		}
	}
	else
	{		
		if(m_MouseSlotItem.Get_m_sID() > 0 /*&& (m_LastDraggingBaseTab == m_SelectedBaseTab)*/ )
		{
			int iconposition = m_MouseSlotItem.m_InventoryNumber - (m_SelectedBaseTab * _XDEF_INVENTORY_ONEBACKPACKCOUNT +_XINVENTORY_SLOTTYPE_POCKETSTART);
			
			if(iconposition >= 0 && iconposition < _XDEF_INVENTORY_ONEBACKPACKCOUNT )
			{
				row = 33 * (iconposition/5);
				column = 33 * (iconposition%5);
				
				iconrect.left = columnpoint + column;
				iconrect.top = rowpoint + row;
				iconrect.right = columnpoint + column + 29;
				iconrect.bottom = rowpoint + row + 29;

				if(m_bHaveMouseSlot)
				{
					// disable icon image
					_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
				}
				
				_XDrawRectAngle( iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT );
				selectedslotarray[m_MouseSlotItem.m_InventoryNumber] = TRUE;
			}		
		}
	}
	// mouse hover
	selectedslot = SearchSlotRegion(&iconrect);
	if(g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID() > 0 /*&& (m_LastDraggingBaseTab == m_SelectedBaseTab)*/)
	{
		if(!selectedslotarray[selectedslot])
		{
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
			DrawItemTooltip(selectedslot, iconrect);
		}
	}
}

void _XWindow_Inventory::DrawDragIcon(void)
{
//	if(g_MessageBox.m_ShowWindow) return;
	_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSAGEWINDOW);
	if(pMessageBox)
	{
		if(pMessageBox->GetShowStatus())
			return;
	}
	_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
	if(pTradeMessageBox)		return;	

	_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
	if(pTradeConfirm_Window)
	{
		if(pTradeConfirm_Window->GetShowStatus())
			return;
	}

	/*
	_XWindow_ItemDropConfirm* pItemDropConfirm_Window = (_XWindow_ItemDropConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ITEMDROPCONFIRM); 
		if(pItemDropConfirm_Window)
		{
			if(pItemDropConfirm_Window->GetShowStatus())
				return;
		}*/
	

	if(m_bHaveMouseSlot)
	{
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
		DrawIcon(TRUE, scrnpos->x - 16 , scrnpos->z - 16, NULL, TRUE, FALSE); //Author : ����� //breif : �巡�� �ϴ� ��������,, �����ϰ�		
		_XDrawRectAngle( scrnpos->x - 16, scrnpos->z - 16, scrnpos->x + 16, scrnpos->z + 16, 1.0f, D3DCOLOR_ARGB( 185, 255, 255, 0 ) ); // ����� ������ �ĺ���

		((CEmperorOfDragonsApp*)gApp)->ReadyCursor(_XMOUSETYPE_SELECT);
	}	
}

void _XWindow_Inventory::DrawPocketList(void)
{
	int count = _XINVENTORY_SLOTTYPE_POCKETSTART;

	int columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
	int rowpoint	= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;	

	for(int row = 0; row < _XDEF_INVENTORYSLOT_HEIGHT; row++)
	{
		for(int column = 0; column < _XDEF_INVENTORYSLOT_WIDTH; column++)
		{
			if(count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT) break;

			if(g_pLocalUser->m_UserItemList[count].Get_m_sID() > 0)//&& count != m_MouseSlotItem.m_InventoryNumber)
			{
				m_ItemEdgeIcon[_XITEMEDGEICONTYPE_NORMAL].Draw( columnpoint-2, rowpoint-2 );
				DrawIcon(TRUE, columnpoint, rowpoint, &g_pLocalUser->m_UserItemList[count]);
			}
			else
			{
				m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].Draw( columnpoint-2, rowpoint-2 );
			}
			
			//if( slot disable )
			//{
			//	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].Draw( columnpoint-1, rowpoint-1 );
			//	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_DISABLESLOT].Draw( columnpoint, rowpoint );
			//}

			columnpoint+=33;
			count++;			
		}
		
		if(count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT) break;

		columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
		rowpoint += 33;
	}
}

void _XWindow_Inventory::DrawBackPackList(void)
{
	int count = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART;
	
	int columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
	int rowpoint	= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;	
	
	for(int row = 0; row < _XDEF_INVENTORYSLOT_HEIGHT; row++)
	{
		for(int column = 0; column < _XDEF_INVENTORYSLOT_WIDTH; column++)
		{
			if(count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT) break;

			if(g_pLocalUser->m_UserItemList[count].Get_m_sID() > 0) // && count != m_MouseSlotItem.m_InventoryNumber)
			{
				m_ItemEdgeIcon[_XITEMEDGEICONTYPE_NORMAL].Draw( columnpoint-2, rowpoint-2 );
				DrawIcon(TRUE, columnpoint, rowpoint, &g_pLocalUser->m_UserItemList[count]);//, TRUE, FALSE);
			}
			else
			{
				m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].Draw( columnpoint-2, rowpoint-2 );
			}
			
			//if( slot disable )
			//{
			//	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].Draw( columnpoint-1, rowpoint-1 );
			//	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_DISABLESLOT].Draw( columnpoint, rowpoint );
			//}
			
			columnpoint+=33;
			count++;			
		}
		
		if(count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT) break;

		columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
		rowpoint += 33;
	}
}

void _XWindow_Inventory::DrawPassList(void)
{
	int count = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART;
	
	int columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
	int rowpoint	= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;	
	
	for(int row = 0; row < _XDEF_INVENTORYSLOT_HEIGHT; row++)
	{
		for(int column = 0; column < _XDEF_INVENTORYSLOT_WIDTH; column++)
		{
			if(count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT) break;

			if(g_pLocalUser->m_UserItemList[count].Get_m_sID() > 0) // && count != m_MouseSlotItem.m_InventoryNumber)
			{				
				m_ItemEdgeIcon[_XITEMEDGEICONTYPE_NORMAL].Draw( columnpoint-2, rowpoint-2 );
				DrawIcon(TRUE, columnpoint, rowpoint, &g_pLocalUser->m_UserItemList[count]);
			}
			else
			{
				m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].Draw( columnpoint-2, rowpoint-2 );
			}
			
			//if( slot disable )
			//{
			//	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].Draw( columnpoint-1, rowpoint-1 );
			//	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_DISABLESLOT].Draw( columnpoint, rowpoint );
			//}

			columnpoint+=33;
			count++;			
		}
		
		if(count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT) break;

		columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
		rowpoint += 33;
	}
}

void _XWindow_Inventory::DrawCharEquipList(void)
{
	int count = _XINVENTORY_SLOTTYPE_AMOR;
	
	int columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
	int rowpoint	= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;	
	
	for(int row = 0; row < _XDEF_INVENTORYSLOT_HEIGHT; row++)
	{
		for(int column = 0; column < _XDEF_INVENTORYSLOT_WIDTH; column++)
		{
			if( count >= 20 ) break;

			if(g_pLocalUser->m_UserItemList[count].Get_m_sID() > 0) // && count != m_MouseSlotItem.m_InventoryNumber)
			{				
				m_ItemEdgeIcon[_XITEMEDGEICONTYPE_NORMAL].Draw( columnpoint-2, rowpoint-2 );
				DrawIcon(TRUE, columnpoint, rowpoint, &g_pLocalUser->m_UserItemList[count]);
			}
			else
			{
				m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].Draw( columnpoint-2, rowpoint-2 );
			}

			columnpoint+=33;
			count++;			
		}
		
		if( count >= 20 ) break;

		columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
		rowpoint += 33;
	}
}

void _XWindow_Inventory::DrawIcon(BOOL smallicon, int X, int Y, _XUserItem* pItem, BOOL bMarkStackble,  BOOL busingitem)// 2004.06.10->oneway48 insert
{
	char			cType = 0;
	short			sID = 0;
	unsigned char	ucCount = 0;
	BOOL			bStackable = FALSE;

	int				iconimageindex;
	RECT			imagerect;

	if(!pItem)
	{
		cType = m_MouseSlotItem.Get_m_cType();
		sID = m_MouseSlotItem.Get_m_sID();
		ucCount = m_MouseSlotItem.Get_m_ucCount();
		bStackable = g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].IsStackable();
	}
	else
	{
		cType = pItem->Get_m_cType();
		sID = pItem->Get_m_sID();
		ucCount = pItem->Get_m_ucCount();
		bStackable = pItem->IsStackable();
	}
	if( sID == 0) return;
	
	GetIconRect(cType, sID, iconimageindex, imagerect, smallicon);

	if(iconimageindex != -1)
	{
        g_InventoryGradeIconImage.EnableWindow(busingitem);
		g_InventoryIconImage.EnableWindow(busingitem);
		m_imageCashItemBorder->EnableWindow(busingitem);
		
    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶�� �׻� ��� ����
        /*if( _XGI_FC_CLOTHES == GetEqualItemType(cType) )
		{
            _XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( tempClothesitem )
            {
            _XGI_SC_CLOTHES_BACKPACK
                if( (sID == 12084 || sID == 12085 || sID == 12086 || sID == 12087 ) &&
                    tempClothesitem[sID].cCashCheck != 0 )
                {
                    g_InventoryGradeIconImage.EnableWindow(TRUE);
		            g_InventoryIconImage.EnableWindow(TRUE);
		            m_imageCashItemBorder->EnableWindow(TRUE);
                }
            }
        }*/
    #endif

		g_InventoryIconImage.SetTexture(&g_MainInterfaceTextureArchive, iconimageindex);
		g_InventoryIconImage.DrawWithRegion(X, Y, imagerect);
		int grade = _XGameItem::GetItemGrade(cType, sID);
		if( grade > 0 && grade < 6 )	// ���� 6�� ����� ����
		{
			grade--;
			g_InventoryGradeIconImage.SetClipRect(1+32*grade, 1, 1+32*grade+31, 31 );
			g_InventoryGradeIconImage.Draw(X, Y);
		}

		if( cType == _XGI_FC_ELIXIR )
		{
			XGI_ElixirItem_Property* ElixirItem = (XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ElixirItem )
			{
				// ĳ�� ������ �����̸� �ִϸ��̼� ȿ���� ���� �ʰ� �µθ��� �׸���.
#ifdef _XTS_USECASHITEM
	#ifdef ALL_RB_DRAW_CASHCHECK_070820_KUKURI
				if( ElixirItem[sID].cCashCheck == 1 || ElixirItem[sID].cCashCheck == 2)
	#else
				if( ElixirItem[sID].cCashCheck != 0 )
	#endif
				{
					m_imageCashItemBorder->Draw(X, Y);
				}
				else // ĳ�� ������ ������ �ƴϸ� �ִϸ��̼� ȿ���� �ִ´�.
#endif
				{
					if( g_LocalSystemTime - m_ElixirItemEffectFrame > 100 )
					{
						m_ElixirItemEffectFrame = g_LocalSystemTime;
						if( m_ElixirItemEffectFrameCount == 7 || m_ElixirItemEffectFrameCount == 0 )
							m_ElixirItemEffectFrameChange *= -1;
						
						m_ElixirItemEffectFrameCount += m_ElixirItemEffectFrameChange;
					}
					g_InventoryGradeIconImage.SetClipRect(1+32*m_ElixirItemEffectFrameCount, 33, 1+32*m_ElixirItemEffectFrameCount+31, 63 );
					g_InventoryGradeIconImage.Draw(X, Y);
				}
			}
		}			
#ifdef _XTS_ITEM_V21
#ifdef _XTS_USECASHITEM
		else if( cType == _XGI_FC_CLOTHES)
		{
			XGI_ClothesItem_Property* ClothesItem = (XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ClothesItem )
			{
	#ifdef ALL_RB_DRAW_CASHCHECK_070820_KUKURI
				if(ClothesItem[sID].cCashCheck == 1 || ClothesItem[sID].cCashCheck == 2)
	#else
				if( ClothesItem[sID].cCashCheck != 0 )
	#endif
				{
					m_imageCashItemBorder->Draw(X, Y);
				}
			}
		}
		else if( cType == _XGI_FC_POTION)
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
			if( PotionItem )
			{
	#ifdef ALL_RB_DRAW_CASHCHECK_070820_KUKURI
				if(PotionItem[sID].cCashCheck == 1 || PotionItem[sID].cCashCheck == 2)
	#else
				if( PotionItem[sID].cCashCheck != 0 )
	#endif
					m_imageCashItemBorder->Draw(X, Y);
			}
		}
		else if( cType == _XGI_FC_RESOURCE )
		{
			_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);
			if( ResourceItem )
			{
	#ifdef ALL_RB_DRAW_CASHCHECK_070820_KUKURI
				if(ResourceItem[sID].cCashCheck == 1 || ResourceItem[sID].cCashCheck == 2)
	#else
				if( ResourceItem[sID].cCashCheck != 0 )
	#endif
					m_imageCashItemBorder->Draw(X, Y);
			}
			
		}
		else if( cType == _XGI_FC_ACCESSORY )
		{
			XGI_AccessoryItem_Property* AccessoryItem = (XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
			if( AccessoryItem )
			{
	#ifdef ALL_RB_DRAW_CASHCHECK_070820_KUKURI
				if(AccessoryItem[sID].cCashCheck == 1 || AccessoryItem[sID].cCashCheck == 2)
	#else
				if( AccessoryItem[sID].cCashCheck != 0 )
	#endif
					m_imageCashItemBorder->Draw(X, Y);
			}
			
		}
		else if(cType == _XGI_FC_LIFE)
		{
			_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
			if(LifeItem)
			{
	#ifdef ALL_RB_DRAW_CASHCHECK_070820_KUKURI
				if(LifeItem[sID].cCashCheck == 1 || LifeItem[sID].cCashCheck == 2)
	#else
				if(LifeItem[sID].cCashCheck != 0)
	#endif
					m_imageCashItemBorder->Draw(X, Y);
			}
		}
#endif
#endif

#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(cType) )
#else
		else if( cType == _XGI_FC_WEAPON || cType == _XGI_FC_WEAPON2 || cType == _XGI_FC_WEAPON3 )
#endif
		{
			if( pItem )
			{
				if( pItem->Get_m_ucStrength() > 0 )
				{
					
					g_XSmallFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
					g_XSmallFont->SetColor( 0xFFFFD901 );
					g_XSmallFont->PrintAlign(X+30, Y+32-g_XSmallFont->GetHeight(), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "+%d", pItem->Get_m_ucStrength() );
					g_XSmallFont->Flush();
					g_XSmallFont->DisableGrowMode();
				}	
				
				XGI_WeaponItem_Property* WeaponItem = (XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);
				if( WeaponItem  )
				{
#ifdef _XDEF_PERIODITEM_20061027
	#ifdef ALL_RB_DRAW_CASHCHECK_070820_KUKURI
				if(WeaponItem[sID].cCashCheck == 1 || WeaponItem[sID].cCashCheck == 2)
	#else
					if( WeaponItem[sID].cCashCheck != 0 )
	#endif
					{
						// �Ⱓ�� ������ or ĳ�� �������� �� �׵θ� �׷��� ��
						m_imageCashItemBorder->Draw(X, Y);
					}
					else if( WeaponItem[sID].cSecondType == _XGI_SC_WEAPON_SPECIAL && WeaponItem[sID].cThirdType == _XGI_TC_WEAPON_SERING )
#else
					if( WeaponItem[sID].cSecondType == _XGI_SC_WEAPON_SPECIAL && WeaponItem[sID].cThirdType == _XGI_TC_WEAPON_SERING )
#endif
					{
						// 19�ܰ� �⺸ �������  �׷��̵尡 51�� ǥ�õ�. �׵θ� ǥ�ø� ���־�� ��.
						if( grade == 51 )
						{
							g_InventoryGradeIconImage.SetClipRect(1, 1, 1+31, 31 );
							g_InventoryGradeIconImage.Draw(X, Y);
						}
					}
				}
			}
		}

		if(bStackable && bMarkStackble)
		{
			g_XSmallFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
			g_XSmallFont->SetColor( 0xFFEAEAEA );
			g_XSmallFont->PrintAlign(X+30, Y+32-g_XSmallFont->GetHeight(),
									 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", ucCount);
			g_XSmallFont->Flush();
			g_XSmallFont->DisableGrowMode();
		}
	}
	else
	{
		if(cType == -1 && sID == 0) return;
				
		g_InventoryIconImage.SetTexture(&g_MainInterfaceTextureArchive, iconimageindex);
		g_InventoryIconImage.DrawWithRegion(X, Y, imagerect);
		
		if(bStackable)
		{
			g_XSmallFont->SetColor(_XSC_INFORMATION);
			g_XSmallFont->PrintAlign(X+30, Y+32-g_XSmallFont->GetHeight(),
				1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", ucCount);
			g_XSmallFont->Flush();
		}
	}
}
void _XWindow_Inventory::DrawIcon_LifeSkill(BOOL smallicon, int X, int Y, _XCollectionResourceItem* pItem, BOOL bMarkStackble,  BOOL busingitem)// 2004.06.10->oneway48 insert
{
	char			cType = 0;
	short			sID = 0;
	unsigned char	ucCount = 0;
	BOOL			bStackable = FALSE;

	int				iconimageindex;
	RECT			imagerect;

	if(!pItem)
	{
		cType = m_MouseSlotItem.Get_m_cType();
		sID = m_MouseSlotItem.Get_m_sID();
		ucCount = m_MouseSlotItem.Get_m_ucCount();
		bStackable = g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].IsStackable();
	}
	else
	{
		cType = pItem->Get_m_cType();
		sID = pItem->Get_m_sID();
		ucCount = pItem->Get_m_ucCount();
		bStackable = pItem->IsStackable();
	}
	if( sID == 0) return;
	
	GetIconRect(cType, sID, iconimageindex, imagerect, smallicon);

	if(iconimageindex != -1)
	{
		g_InventoryIconImage.SetTexture(&g_MainInterfaceTextureArchive, iconimageindex);
		g_InventoryIconImage.DrawWithRegion(X, Y, imagerect);
		int grade = _XGameItem::GetItemGrade(cType, sID);
		if( grade > 0 && grade < 6 )	// ���� 6�� ����� ����
		{
			grade--;
			g_InventoryGradeIconImage.SetClipRect(1+32*grade, 1, 1+32*grade+31, 31 );
			g_InventoryGradeIconImage.Draw(X, Y);
		}

		if( cType == _XGI_FC_ELIXIR )
		{
			if( g_LocalSystemTime - m_ElixirItemEffectFrame > 100 )
			{
				m_ElixirItemEffectFrame = g_LocalSystemTime;
				if( m_ElixirItemEffectFrameCount == 7 || m_ElixirItemEffectFrameCount == 0 )
					m_ElixirItemEffectFrameChange *= -1;

				m_ElixirItemEffectFrameCount += m_ElixirItemEffectFrameChange;
			}
			g_InventoryGradeIconImage.SetClipRect(1+32*m_ElixirItemEffectFrameCount, 33, 1+32*m_ElixirItemEffectFrameCount+31, 63 );
			g_InventoryGradeIconImage.Draw(X, Y);
		}						
		
		if(bStackable && bMarkStackble)
		{
			g_XSmallFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
			g_XSmallFont->SetColor( 0xFFEAEAEA );
			g_XSmallFont->PrintAlign(X+30, Y+32-g_XSmallFont->GetHeight(),
									 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", ucCount);
			g_XSmallFont->Flush();
			g_XSmallFont->DisableGrowMode();
		}
	}
	else
	{
		if(cType == -1 && sID == 0) return;
				
		g_InventoryIconImage.SetTexture(&g_MainInterfaceTextureArchive, iconimageindex);
		g_InventoryIconImage.DrawWithRegion(X, Y, imagerect);
			
		if(bStackable)
		{
			g_XSmallFont->SetColor(_XSC_INFORMATION);
			g_XSmallFont->PrintAlign(X+30, Y+32-g_XSmallFont->GetHeight(),
				1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", ucCount);
			g_XSmallFont->Flush();
		}
	}
}

void _XWindow_Inventory::DrawIcon_Gamble(BOOL smallicon, int X, int Y, _XUserItem* pItem, BOOL bMarkStackble,  BOOL busingitem, BOOL checksex )
{
	char			cType = 0;
	short			sID = 0;
	unsigned char	ucCount = 0;
	BOOL			bStackable = FALSE;

	int				iconimageindex;
	RECT			imagerect;

	if(!pItem)
	{
		cType = m_MouseSlotItem.Get_m_cType();
		sID = m_MouseSlotItem.Get_m_sID();
		ucCount = m_MouseSlotItem.Get_m_ucCount();
		bStackable = g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].IsStackable();
	}
	else
	{
		cType = pItem->Get_m_cType();
		sID = pItem->Get_m_sID();
		ucCount = pItem->Get_m_ucCount();
		bStackable = pItem->IsStackable();
	}
	if( sID == 0) return;
	
	GetIconRect(cType, sID, iconimageindex, imagerect, smallicon);

	if(iconimageindex != -1)
	{
		g_InventoryIconImage.SetTexture(&g_MainInterfaceTextureArchive, iconimageindex);
		g_InventoryIconImage.DrawWithRegion(X, Y, imagerect);

		if( !checksex )
		{
			_XDrawSolidBar( X, Y, X+30, Y+30, D3DCOLOR_ARGB(102,237,27,35) );							
		}	
		
		g_InventoryGradeIconImage.SetClipRect(193, 1, 223, 31);
		g_InventoryGradeIconImage.Draw(X, Y-1);		

		// insert end
		
		if(bStackable && bMarkStackble)
			
		{
			g_XSmallFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
			g_XSmallFont->SetColor( 0xFFEAEAEA );
			g_XSmallFont->PrintAlign(X+30, Y+32-g_XSmallFont->GetHeight(),
									 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", ucCount);
			g_XSmallFont->Flush();
			g_XSmallFont->DisableGrowMode();
		}
	}
	else
	{
		if(cType == -1 && sID == 0) return;				
		
		g_InventoryIconImage.SetTexture(&g_MainInterfaceTextureArchive, iconimageindex);
		g_InventoryIconImage.DrawWithRegion(X, Y, imagerect);			
		
		// insert end
		
		if(bStackable)
		{
			g_XSmallFont->SetColor(_XSC_INFORMATION);
			g_XSmallFont->PrintAlign(X+30, Y+32-g_XSmallFont->GetHeight(),
				1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", ucCount);
			g_XSmallFont->Flush();
		}
	}
}

void _XWindow_Inventory::GetIconRect(char cType, short sID, int& imageindex, RECT& rect, BOOL smallicon)
{
	int		iconclipcolumn = 0, iconcliprow = 0;
	_XGENDERTYPE gender = _XGENDER_MAN;

	short sIconNo = -1;
	imageindex = g_MainInterfaceTextureArchive.FindResource("item_mon.tga");	// �̹��� ���� �����۵��� �� �̰ɷ�...
	
	if( sID > 0 )
	{
		TCHAR tempStr [128];
		memset( tempStr, 0, sizeof(TCHAR)*128);
		
#ifdef _XTS_ITEM_OPTIMIZATION
		char itemtype = GetEqualItemType(cType);
		
		if( itemtype != -1 )
		{
			switch(itemtype) 
			{
			case _XGI_FC_WEAPON :
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(cType);
					if( WeaponItem )
					{
						sIconNo = WeaponItem[sID].sIcon_No - 1;			
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;
						
						switch(WeaponItem[sID].cSecondType) 
						{			
						case _XGI_SC_WEAPON_SHORT :		// �ܺ���
							{
								switch(WeaponItem[sID].cThirdType) 
								{
								case _XGI_TC_WEAPON_SWORD :		// �˹�
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_13.tga");
									}
									break;
								case _XGI_TC_WEAPON_KNIFE :		// ����
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_14.tga");
									}
									break;
								case _XGI_TC_WEAPON_CLUB :		// Ÿ����
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_02.tga");
									}
									break;
								case _XGI_TC_WEAPON_HIT :		// Ÿ��
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_03.tga");
									}
									break;
								case _XGI_TC_WEAPON_FIST :		// �ǰ�
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_04.tga");
									}
									break;
								case _XGI_TC_WEAPON_SECRET :	// ��Ÿ
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_05.tga");
									}
									break;
								case _XGI_TC_WEAPON_AXE :		// �ι�
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_06.tga");
									}
									break;
								case _XGI_TC_WEAPON_RING :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_w22.tga");
									}
									break;
								}
							}
							break;
						case _XGI_SC_WEAPON_LONG :		// �庴��
							{
								switch( WeaponItem[sID].cThirdType )
								{
								case _XGI_TC_WEAPON_GLAIVE :	// �뵵
									{
									}
									break;
								case _XGI_TC_WEAPON_SPEAR :		// â��
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_11.tga");
									}
									break;
								case _XGI_TC_WEAPON_STAFF :		// ���
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_12.tga");
									}
									break;
								}
							}
							break;
						case _XGI_SC_WEAPON_SPECIAL :	// ������� 
							{
								switch( WeaponItem[sID].cThirdType )
								{
								case _XGI_TC_WEAPON_SERING :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_023.tga");
									}
									break;
								}
							}
							break;
						default :
							break;
						}
					}
				}
				break;
			case _XGI_FC_CLOTHES :
				{
					_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(cType);
					if( ClothesItem )
					{
						gender = (_XGENDERTYPE)ClothesItem[sID].cSexType;
						
						sIconNo = ClothesItem[sID].sIcon_No - 1;
						
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;
						
						switch(ClothesItem[sID].cSecondType)
						{
						case _XGI_SC_CLOTHES_UPPER :		// ����
							{
								switch( ClothesItem[sID].cClan )
								{
								case _XGROUP_NANGIN :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00a.tga");
									}
									break;
								case _XGROUP_GAEBANG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01a.tga");
									}
									break;
								case _XGROUP_BEEGOONG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02a.tga");
									}
									break;
								case _XGROUP_SORIM :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03a.tga");
									}
									break;
								case _XGROUP_NOCKRIM :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04a.tga");
									}
									break;
								case _XGROUP_MOODANG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05a.tga");
									}
									break;
								case _XGROUP_MAKYO :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06a.tga");
									}
									break;
								case _XGROUP_SEGA :
									{
									}
									break;
								case _XGROUP_NOFIXED :
									{
									}
									break;
								}
							}
							break;
						case _XGI_SC_CLOTHES_LOWER :		// ����
							{
								switch(ClothesItem[sID].cClan)
								{
								case _XGROUP_NANGIN :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00b.tga");
									}
									break;
								case _XGROUP_GAEBANG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01b.tga");
									}
									break;
								case _XGROUP_BEEGOONG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02b.tga");
									}
									break;
								case _XGROUP_SORIM :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03b.tga");
									}
									break;
								case _XGROUP_NOCKRIM :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04b.tga");
									}
									break;
								case _XGROUP_MOODANG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05b.tga");
									}
									break;
								case _XGROUP_MAKYO :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06b.tga");
									}
									break;
								case _XGROUP_SEGA :
									{
									}
									break;
								case _XGROUP_NOFIXED :
									{
									}
									break;
								}
							}
							break;
						case _XGI_SC_CLOTHES_CAP :			// ���� 
							{
								switch(ClothesItem[sID].cClan)
								{
								case _XGROUP_NANGIN :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00e.tga");
									}
									break;
								case _XGROUP_GAEBANG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01e.tga");
									}
									break;
								case _XGROUP_BEEGOONG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02e.tga");
									}
									break;
								case _XGROUP_SORIM :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03e.tga");
									}
									break;
								case _XGROUP_NOCKRIM :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04e.tga");
									}
									break;
								case _XGROUP_MOODANG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05e.tga");
									}
									break;
								case _XGROUP_MAKYO :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06e.tga");
									}
									break;
								case _XGROUP_SEGA :
									{
									}
									break;
								case _XGROUP_NOFIXED :
									{
									}
									break;
								}
							}
							break;
						case _XGI_SC_CLOTHES_GLOVES :		// �尩
							{
								switch(ClothesItem[sID].cClan)
								{
								case _XGROUP_NANGIN :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00c.tga");
									}
									break;
								case _XGROUP_GAEBANG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01c.tga");
									}
									break;
								case _XGROUP_BEEGOONG:
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02c.tga");
									}
									break;
								case _XGROUP_SORIM :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03c.tga");
									}
									break;
								case _XGROUP_NOCKRIM :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04c.tga");
									}
									break;
								case _XGROUP_MOODANG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05c.tga");
									}
									break;
								case _XGROUP_MAKYO :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06c.tga");
									}
									break;
								case _XGROUP_SEGA :
									{
									}
									break;
								case _XGROUP_NOFIXED :
									{
									}
									break;
								}
							}
							break;
						case _XGI_SC_CLOTHES_SHOES :		// �Ź�
							{
								switch(ClothesItem[sID].cClan)
								{
								case _XGROUP_NANGIN :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00d.tga");
									}
									break;
								case _XGROUP_GAEBANG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01d.tga");
									}
									break;
								case _XGROUP_BEEGOONG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02d.tga");
									}
									break;
								case _XGROUP_SORIM :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03d.tga");
									}
									break;
								case _XGROUP_NOCKRIM :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04d.tga");
									}
									break;
								case _XGROUP_MOODANG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05d.tga");
									}
									break;
								case _XGROUP_MAKYO :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06d.tga");
									}
									break;
								case _XGROUP_SEGA :
									{
									}
									break;
								case _XGROUP_NOFIXED :
									{
									}
									break;
								}						
							}
							break;
						case _XGI_SC_CLOTHES_BACKPACK :
							{
								switch(ClothesItem[sID].cClan)
								{
								case _XGROUP_NANGIN :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00h.tga");
									}
									break;
								case _XGROUP_GAEBANG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01h.tga");
									}
									break;
								case _XGROUP_BEEGOONG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02h.tga");
									}
									break;
								case _XGROUP_SORIM :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03h.tga");
									}
									break;
								case _XGROUP_NOCKRIM :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04h.tga");
									}
									break;
								case _XGROUP_MOODANG :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05h.tga");
									}
									break;
								case _XGROUP_MAKYO :
									{
										imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06h.tga");
									}
									break;
								case _XGROUP_SEGA :
									{
									}
									break;
								case _XGROUP_NOFIXED :
									{
									}
									break;
								}
							}
						case _XGI_SC_CLOTHES_CLOAK :
						case _XGI_SC_CLOTHES_ARMOR :
							break;
						case _XGI_SC_CLOTHES_MASK :
							{
								int i = sIconNo / 32;							

								sIconNo -= (32*i);
								iconclipcolumn = sIconNo % 8;
								iconcliprow    = sIconNo / 8;
#ifdef _XDEF_CLOTHMASK_DIVIDE
								// ĳ�� �������� ���� �ٸ� ������ �д´�.
								if( ClothesItem[sID].cCashCheck == 1 || ClothesItem[sID].cCashCheck == 2)
								{
									sprintf( tempStr, "Item_Clo_CashMask_0%d.tga", i+1 );
								}
								else

#endif
								{
									sprintf( tempStr, "Item_Clo_Mask_0%d.tga", i+1 );
								}

								imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);							
							}
							break;
						}
						break;
					}
				}
				break;
			case _XGI_FC_BOOK :
				{
					_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(cType);
					if( BookItem )
					{
						sIconNo = BookItem[sID].sIcon_No - 1;
						
						int i = sIconNo / 64;
						
						sIconNo -= (64*i);
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;
						
						sprintf( tempStr, "Item_Boo_00%d.tga", i+1 );
						imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);
					}
				}
				break;
			case _XGI_FC_POTION :
				{
					_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(cType);
					if( PotionItem )
					{
						sIconNo = PotionItem[sID].sIcon_No - 1;
						
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;
						
						switch(PotionItem[sID].cSecondType) {
						case _XGI_SC_POTION_HERT_IN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w1_001.tga");
							}
							break;
						case _XGI_SC_POTION_HERT_OUT :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w2_002.tga");
							}
							break;
						case _XGI_SC_POTION_NOTMERGE :
						case _XGI_SC_POTION_FATIGUE :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w3_003.tga");
							}
							break;
						case _XGI_SC_POTION_NOTFIXED :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w4_004.tga");
							}
							break;
						case _XGI_SC_POTION_FIREWORK :
						case _XGI_SC_POTION_FOOD :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w5_005.tga");
							}
							break;
						case _XGI_SC_POTION_COOLDOWN :
						case _XGI_SC_POTION_EVENT :
						case _XGI_SC_POTION_MASK :
						case _XGI_SC_POTION_ANTI :
							{
								int i = sIconNo / 64;

								sIconNo -= (64*i);
								iconclipcolumn = sIconNo % 8;
								iconcliprow    = sIconNo / 8;

								if(i == 0)
									imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w6_006.tga");
								else
									imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w9_009.tga");

							}
							break;
						case _XGI_SC_POTION_HWATA :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w7_007.tga");
							}
							break;
							
						case _XGI_SC_POTION_ADDEXP :
						case _XGI_SC_POTION_RESETPOINT :
						case _XGI_SC_POTION_NICKNAME :
						case _XGI_SC_POTION_INCHANTRATE :
						case _XGI_SC_POTION_SHOUT :
						case _XGI_SC_POTION_RECALL : //Author : ������߰�
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w8_008.tga");
							}
							break;
						case _XGI_SC_POTION_EVENT2 : //Author : ����� �߰�
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w10_010.tga");
							}
							break;
						}
					}
				}
				break;
			case _XGI_FC_CONSUMING :
				{
					_XGI_ConsumingItem_Property* ConsumingItem = (_XGI_ConsumingItem_Property*)GetFirstTypeItemPorperty(cType);
					if( ConsumingItem )
					{
						sIconNo = ConsumingItem[sID].sIcon_No - 1;
						
						int i = sIconNo / 64;
						
						sIconNo -= (64*i);
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;
						
						if( i < 9 )
							sprintf( tempStr, "Item_Con_00%d.tga", i+1 );
						else if( i < 99 )
							sprintf( tempStr, "Item_Con_0%d.tga", i+1 );
						else 
							sprintf( tempStr, "Item_Con_%d.tga", i+1 );
						
						imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);

/*						sIconNo = ConsumingItem[sID].sIcon_No - 1;
						
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;
						
						imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Con_001.tga");*/
					}
				}
				break;
			case _XGI_FC_ELIXIR :
				{
					_XGI_ElixirItem_Property* ElixirItem = (_XGI_ElixirItem_Property*)GetFirstTypeItemPorperty(cType);
					if( ElixirItem )
					{
						sIconNo = ElixirItem[sID].sIcon_No - 1;
						int i = sIconNo / 64;
						
						sIconNo -= (64*i);
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;
						
						sprintf( tempStr, "Item_Elixir_0%d.tga", i+1 );
						imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);
					}
				}
				break;			
			case _XGI_FC_RESOURCE :
				{
					_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(cType);
					if( ResourceItem )
					{
						sIconNo = ResourceItem[sID].sIcon_No - 1;
						
						int i = sIconNo / 64;
						
						sIconNo -= (64*i);
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;
						
						if( i < 9 )
							sprintf( tempStr, "Item_Res_00%d.tga", i+1 );
						else if( i < 99 )
							sprintf( tempStr, "Item_Res_0%d.tga", i+1 );
						else 
							sprintf( tempStr, "Item_Res_%d.tga", i+1 );
						
						imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);
					}				
				}
				break;
			case _XGI_FC_QUEST :
				{
					_XGI_QuestItem_Property* QuestItem = (_XGI_QuestItem_Property*)GetFirstTypeItemPorperty(cType);
					if( QuestItem )
					{
						sIconNo = QuestItem[sID].sIcon_No - 1;
						
						int i = sIconNo / 64;
						
						sIconNo -= (64*i);
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;
						
						if( i < 9 )
							sprintf( tempStr, "Item_que_00%d.tga", i+1 );
						else if( i < 99 )
							sprintf( tempStr, "Item_que_0%d.tga", i+1 );
						else 
							sprintf( tempStr, "Item_que_%d.tga", i+1 );				
						
						imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);
					}
				}
				break;
			case _XGI_FC_ACCESSORY :
				{
					_XGI_AccessoryItem_Property* AccessoryItem = (_XGI_AccessoryItem_Property*)GetFirstTypeItemPorperty(cType);
					if( AccessoryItem )
					{
						sIconNo = AccessoryItem[sID].sIcon_No - 1;
						
						int i = sIconNo / 64;
						
						sIconNo -= (64*i);
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;
						
						if( i < 9 )
							sprintf( tempStr, "Item_Acc_00%d.tga", i+1 );
						else 
							sprintf( tempStr, "Item_Acc_0%d.tga", i+1 );				
						
						imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);
					}
				}
				break;
			case _XGI_FC_LIFE :
				{
					_XGI_LifeItem_Property* LifeItem = (_XGI_LifeItem_Property*)GetFirstTypeItemPorperty(cType);
					if( LifeItem )
					{
						sIconNo = LifeItem[sID].sIcon_No - 1;
						
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;
						
						imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Life_001.tga");
					}
				}
				break;
			case _XGI_FC_SOCKET :
				{
					_XGI_SocketItem_Property* SocketItem = (_XGI_SocketItem_Property*)GetFirstTypeItemPorperty(cType);
					if( SocketItem )
					{
						sIconNo = SocketItem[sID].sIcon_No - 1;
						
						int i = sIconNo / 64;
						
						sIconNo -= (64*i);
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;
						
						if( i < 9 )
							sprintf( tempStr, "Item_Sck_00%d.tga", i+1 );
						else 
							sprintf( tempStr, "Item_Sck_0%d.tga", i+1 );				
						
						imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);
					}
				}
				break;
			case _XGI_FC_BOX :
				{
					_XGI_BoxItem_Property* BoxItem = (_XGI_BoxItem_Property*)GetFirstTypeItemPorperty(cType);
					if( BoxItem )
					{
						sIconNo = BoxItem[sID].sIcon_No - 1;
						
						if( sIconNo < 64 )
						{
							iconclipcolumn = sIconNo % 8;
							iconcliprow    = sIconNo / 8;
							
							imageindex = g_MainInterfaceTextureArchive.FindResource("Item_GBox_01.tga");
						}
					}
				}
				break;
			case _XGI_FC_BOXKEY :
				{
					_XGI_BoxKeyItem_Property* BoxkeyItem = (_XGI_BoxKeyItem_Property*)GetFirstTypeItemPorperty(cType);
					if( BoxkeyItem )
					{
						sIconNo = BoxkeyItem[sID].sIcon_No - 1;
						
						if( sIconNo < 64 )
						{
							iconclipcolumn = sIconNo % 8;
							iconcliprow    = sIconNo / 8;
							
							imageindex = g_MainInterfaceTextureArchive.FindResource("Item_GKey_01.tga");
						}
					}
				}
				break;
			case _XGI_FC_MONEY :
			{
				sIconNo = 0;

				rect.left = 0;
				rect.right = 32;
				rect.top = 0;
				rect.bottom = 32;
				
				rect.left += 1;
				rect.top += 1;
				rect.right -= 1;
				rect.bottom -= 1;
				
				imageindex = g_MainInterfaceTextureArchive.FindResource("item_mon.tga");
			}
			break;
			}
		}
#else
		switch(cType)
		{
		case _XGI_FC_WEAPON :	// ����
			{
				sIconNo = g_WeaponItemProperty[sID].sIcon_No - 1;			
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;
						
				switch(g_WeaponItemProperty[sID].cSecondType) 
				{			
				case _XGI_SC_WEAPON_SHORT :		// �ܺ���
					{
						switch(g_WeaponItemProperty[sID].cThirdType) 
						{
						case _XGI_TC_WEAPON_SWORD :		// �˹�
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_13.tga");
							}
							break;
						case _XGI_TC_WEAPON_KNIFE :		// ����
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_14.tga");
							}
							break;
						case _XGI_TC_WEAPON_CLUB :		// Ÿ����
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_02.tga");
							}
							break;
						case _XGI_TC_WEAPON_HIT :		// Ÿ��
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_03.tga");
							}
							break;
						case _XGI_TC_WEAPON_FIST :		// �ǰ�
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_04.tga");
							}
							break;
						case _XGI_TC_WEAPON_SECRET :	// ��Ÿ
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_05.tga");
							}
							break;
						case _XGI_TC_WEAPON_AXE :		// �ι�
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_06.tga");
							}
							break;
						case _XGI_TC_WEAPON_RING :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_w22.tga");
							}
							break;
						}
					}
					break;
				case _XGI_SC_WEAPON_LONG :		// �庴��
					{
						switch( g_WeaponItemProperty[sID].cThirdType )
						{
						case _XGI_TC_WEAPON_GLAIVE :	// �뵵
							{
							}
							break;
						case _XGI_TC_WEAPON_SPEAR :		// â��
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_11.tga");
							}
							break;
						case _XGI_TC_WEAPON_STAFF :		// ���
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_12.tga");
							}
							break;
						}
					}
					break;
				case _XGI_SC_WEAPON_SPECIAL :	// ������� 
					{
						switch( g_WeaponItemProperty[sID].cThirdType )
						{
						case _XGI_TC_WEAPON_SERING :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_023.tga");
							}
							break;
						}
					}
					break;
				default :
					break;
				}
			}
			break;
		case _XGI_FC_CLOTHES :		// �ǻ�
			{
				gender = (_XGENDERTYPE)g_ClothesItemProperty[sID].cSexType;

				sIconNo = g_ClothesItemProperty[sID].sIcon_No - 1;
				
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;

				switch(g_ClothesItemProperty[sID].cSecondType)
				{
				case _XGI_SC_CLOTHES_UPPER :		// ����
					{
						switch( g_ClothesItemProperty[sID].cClan )
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00a.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01a.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02a.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03a.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04a.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05a.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06a.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_LOWER :		// ����
					{
						switch(g_ClothesItemProperty[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00b.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01b.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02b.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03b.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04b.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05b.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06b.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_CAP :			// ���� 
					{
						switch(g_ClothesItemProperty[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00e.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01e.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02e.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03e.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04e.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05e.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06e.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_GLOVES :		// �尩
					{
						switch(g_ClothesItemProperty[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00c.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01c.tga");
							}
							break;
						case _XGROUP_BEEGOONG:
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02c.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03c.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04c.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05c.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06c.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_SHOES :		// �Ź�
					{
						switch(g_ClothesItemProperty[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00d.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01d.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02d.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03d.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04d.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05d.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06d.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}						
					}
					break;
				case _XGI_SC_CLOTHES_BACKPACK :
					{
						switch(g_ClothesItemProperty[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00h.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01h.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02h.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03h.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04h.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05h.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06h.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_CLOAK :
				case _XGI_SC_CLOTHES_ARMOR :
					break;
				case _XGI_SC_CLOTHES_MASK :
					{
						int i = sIconNo / 32;

						sIconNo -= (32*i);
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;

						sprintf( tempStr, "Item_Clo_Mask_0%d.tga", i+1 );
						imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);
					}
					break;
				}
				break;
			}		
			break;
		case _XGI_FC_WEAPON2 :	// ����
			{
				sIconNo = g_WeaponItemProperty2[sID].sIcon_No - 1;			
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;
						
				switch(g_WeaponItemProperty2[sID].cSecondType) 
				{			
				case _XGI_SC_WEAPON_SHORT :		// �ܺ���
					{
						switch(g_WeaponItemProperty2[sID].cThirdType) 
						{
						case _XGI_TC_WEAPON_SWORD :		// �˹�
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_13.tga");
							}
							break;
						case _XGI_TC_WEAPON_KNIFE :		// ����
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_14.tga");
							}
							break;
						case _XGI_TC_WEAPON_CLUB :		// Ÿ����
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_02.tga");
							}
							break;
						case _XGI_TC_WEAPON_HIT :		// Ÿ��
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_03.tga");
							}
							break;
						case _XGI_TC_WEAPON_FIST :		// �ǰ�
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_04.tga");
							}
							break;
						case _XGI_TC_WEAPON_SECRET :	// ��Ÿ
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_05.tga");
							}
							break;
						case _XGI_TC_WEAPON_AXE :		// �ι�
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_06.tga");
							}
							break;
						case _XGI_TC_WEAPON_RING :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_w22.tga");
							}
							break;
						}
					}
					break;
				case _XGI_SC_WEAPON_LONG :		// �庴��
					{
						switch( g_WeaponItemProperty2[sID].cThirdType )
						{
						case _XGI_TC_WEAPON_GLAIVE :	// �뵵
							{
							}
							break;
						case _XGI_TC_WEAPON_SPEAR :		// â��
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_11.tga");
							}
							break;
						case _XGI_TC_WEAPON_STAFF :		// ���
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_12.tga");
							}
							break;
						}
					}
					break;
				case _XGI_SC_WEAPON_SPECIAL :	// ������� 
					{
						switch( g_WeaponItemProperty2[sID].cThirdType )
						{
						case _XGI_TC_WEAPON_SERING :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_023.tga");
							}
							break;
						}
					}
					break;
				default :
					break;
				}
			}
			break;
		case _XGI_FC_CLOTHES2 :		// �ǻ�
			{
				gender = (_XGENDERTYPE)g_ClothesItemProperty2[sID].cSexType;

				sIconNo = g_ClothesItemProperty2[sID].sIcon_No - 1;
				
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;

				switch(g_ClothesItemProperty2[sID].cSecondType)
				{
				case _XGI_SC_CLOTHES_UPPER :		// ����
					{
						switch( g_ClothesItemProperty2[sID].cClan )
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00a.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01a.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02a.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03a.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04a.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05a.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06a.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_LOWER :		// ����
					{
						switch(g_ClothesItemProperty2[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00b.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01b.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02b.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03b.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04b.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05b.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06b.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_CAP :			// ���� 
					{
						switch(g_ClothesItemProperty2[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00e.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01e.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02e.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03e.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04e.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05e.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06e.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_GLOVES :		// �尩
					{
						switch(g_ClothesItemProperty2[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00c.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01c.tga");
							}
							break;
						case _XGROUP_BEEGOONG:
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02c.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03c.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04c.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05c.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06c.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_SHOES :		// �Ź�
					{
						switch(g_ClothesItemProperty2[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00d.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01d.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02d.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03d.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04d.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05d.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06d.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}						
					}
					break;
				case _XGI_SC_CLOTHES_BACKPACK :
					{
						switch(g_ClothesItemProperty2[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00h.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01h.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02h.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03h.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04h.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05h.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06h.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_CLOAK :
				case _XGI_SC_CLOTHES_ARMOR :
					break;
				case _XGI_SC_CLOTHES_MASK :
					{
						int i = sIconNo / 32;

						sIconNo -= (32*i);
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;

						sprintf( tempStr, "Item_Clo_Mask_0%d.tga", i+1 );
						imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);
					}
					break;
				}
				break;
			}		
			break;
		case _XGI_FC_WEAPON3 :	// ����
			{
				sIconNo = g_WeaponItemProperty3[sID].sIcon_No - 1;			
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;
						
				switch(g_WeaponItemProperty3[sID].cSecondType) 
				{			
				case _XGI_SC_WEAPON_SHORT :		// �ܺ���
					{
						switch(g_WeaponItemProperty3[sID].cThirdType) 
						{
						case _XGI_TC_WEAPON_SWORD :		// �˹�
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_13.tga");
							}
							break;
						case _XGI_TC_WEAPON_KNIFE :		// ����
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_14.tga");
							}
							break;
						case _XGI_TC_WEAPON_CLUB :		// Ÿ����
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_02.tga");
							}
							break;
						case _XGI_TC_WEAPON_HIT :		// Ÿ��
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_03.tga");
							}
							break;
						case _XGI_TC_WEAPON_FIST :		// �ǰ�
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_04.tga");
							}
							break;
						case _XGI_TC_WEAPON_SECRET :	// ��Ÿ
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_05.tga");
							}
							break;
						case _XGI_TC_WEAPON_AXE :		// �ι�
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_06.tga");
							}
							break;
						case _XGI_TC_WEAPON_RING :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_w22.tga");
							}
							break;
						}
					}
					break;
				case _XGI_SC_WEAPON_LONG :		// �庴��
					{
						switch( g_WeaponItemProperty3[sID].cThirdType )
						{
						case _XGI_TC_WEAPON_GLAIVE :	// �뵵
							{
							}
							break;
						case _XGI_TC_WEAPON_SPEAR :		// â��
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_11.tga");
							}
							break;
						case _XGI_TC_WEAPON_STAFF :		// ���
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_12.tga");
							}
							break;
						}
					}
					break;
				case _XGI_SC_WEAPON_SPECIAL :	// ������� 
					{
						switch( g_WeaponItemProperty3[sID].cThirdType )
						{
						case _XGI_TC_WEAPON_SERING :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Wea_023.tga");
							}
							break;
						}
					}
					break;
				default :
					break;
				}
			}
			break;
		case _XGI_FC_CLOTHES3 :		// �ǻ�
			{
				gender = (_XGENDERTYPE)g_ClothesItemProperty3[sID].cSexType;

				sIconNo = g_ClothesItemProperty3[sID].sIcon_No - 1;
				
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;

				switch(g_ClothesItemProperty3[sID].cSecondType)
				{
				case _XGI_SC_CLOTHES_UPPER :		// ����
					{
						switch( g_ClothesItemProperty3[sID].cClan )
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00a.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01a.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02a.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03a.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04a.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05a.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06a.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_LOWER :		// ����
					{
						switch(g_ClothesItemProperty3[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00b.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01b.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02b.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03b.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04b.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05b.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06b.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_CAP :			// ���� 
					{
						switch(g_ClothesItemProperty3[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00e.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01e.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02e.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03e.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04e.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05e.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06e.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_GLOVES :		// �尩
					{
						switch(g_ClothesItemProperty3[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00c.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01c.tga");
							}
							break;
						case _XGROUP_BEEGOONG:
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02c.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03c.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04c.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05c.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06c.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_SHOES :		// �Ź�
					{
						switch(g_ClothesItemProperty3[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00d.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01d.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02d.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03d.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04d.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05d.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06d.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}						
					}
					break;
				case _XGI_SC_CLOTHES_BACKPACK :
					{
						switch(g_ClothesItemProperty3[sID].cClan)
						{
						case _XGROUP_NANGIN :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_00h.tga");
							}
							break;
						case _XGROUP_GAEBANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_01h.tga");
							}
							break;
						case _XGROUP_BEEGOONG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_02h.tga");
							}
							break;
						case _XGROUP_SORIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_03h.tga");
							}
							break;
						case _XGROUP_NOCKRIM :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_04h.tga");
							}
							break;
						case _XGROUP_MOODANG :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_05h.tga");
							}
							break;
						case _XGROUP_MAKYO :
							{
								imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Clo_06h.tga");
							}
							break;
						case _XGROUP_SEGA :
							{
							}
							break;
						case _XGROUP_NOFIXED :
							{
							}
							break;
						}
					}
					break;
				case _XGI_SC_CLOTHES_CLOAK :
				case _XGI_SC_CLOTHES_ARMOR :
					break;
				case _XGI_SC_CLOTHES_MASK :
					{
						int i = sIconNo / 32;

						sIconNo -= (32*i);
						iconclipcolumn = sIconNo % 8;
						iconcliprow    = sIconNo / 8;
						
						sprintf( tempStr, "Item_Clo_Mask_0%d.tga", i+1 );
						imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);
					}
					break;
				}
				break;
			}		
			break;
		case _XGI_FC_BOOK :
			{
				sIconNo = g_BookItemProperty[sID].sIcon_No - 1;

				int i = sIconNo / 64;
				
				sIconNo -= (64*i);
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;
				
				sprintf( tempStr, "Item_Boo_00%d.tga", i+1 );
				imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);
			}
			break;
		case _XGI_FC_POTION :
			{
				// 2004.06.16->oneway48 insert
				sIconNo = g_PotionItemProperty[sID].sIcon_No - 1;
				
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;
				// insert end
				
				switch(g_PotionItemProperty[sID].cSecondType) {
				case _XGI_SC_POTION_HERT_IN :
					{
						imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w1_001.tga");
					}
					break;
				case _XGI_SC_POTION_HERT_OUT :
					{
						imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w2_002.tga");
					}
					break;
				case _XGI_SC_POTION_FATIGUE :
					{
						imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w3_003.tga");
					}
					break;
				case _XGI_SC_POTION_NOTFIXED :
					{
						imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w4_004.tga");
					}
					break;
				case _XGI_SC_POTION_FOOD :
					{
						imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w5_005.tga");					
					}
					break;
				case _XGI_SC_POTION_MASK :
				case _XGI_SC_POTION_ANTI :
				case _XGI_SC_POTION_EVENT :
					{
						imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w6_006.tga");
					}
					break;
				case _XGI_SC_POTION_HWATA :
					{
						imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w7_007.tga");
					}
					break;
					
				case _XGI_SC_POTION_ADDEXP :
				case _XGI_SC_POTION_RESETPOINT :
				case _XGI_SC_POTION_NICKNAME :
				case _XGI_SC_POTION_INCHANTRATE :
				case _XGI_SC_POTION_SHOUT :
				case _XGI_SC_POTION_INVENNOTUSE :
					{
						imageindex = g_MainInterfaceTextureArchive.FindResource("Item_pot_w8_008.tga");
					}
					break;
				}
			}
			break;
		case _XGI_FC_CONSUMING :
			{
				sIconNo = g_ConsumingItemProperty[sID].sIcon_No - 1;
				
				int i = sIconNo / 64;
				
				sIconNo -= (64*i);
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;
				
				if( i < 9 )
					sprintf( tempStr, "Item_Con_00%d.tga", i+1 );
				else if( i < 99 )
					sprintf( tempStr, "Item_Con_0%d.tga", i+1 );
				else 
					sprintf( tempStr, "Item_Con_%d.tga", i+1 );
				
				imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);

/*				sIconNo = g_ConsumingItemProperty[sID].sIcon_No - 1;
				
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;

				imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Con_001.tga");*/
			}
			break;
		case _XGI_FC_ELIXIR :
			{
				sIconNo = g_ElixirItemProperty[sID].sIcon_No - 1;
				int i = sIconNo / 64;
				
				sIconNo -= (64*i);
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;
				
				sprintf( tempStr, "Item_Elixir_0%d.tga", i+1 );
				imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);				
			}
			break;
		case _XGI_FC_RESOURCE :
			{
				// 2004.06.18->oneway48 insert
				sIconNo = g_ResourceItemProperty[sID].sIcon_No - 1;
				
				int i = sIconNo / 64;
				
				sIconNo -= (64*i);
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;
				
				if( i < 9 )
					sprintf( tempStr, "Item_Res_00%d.tga", i+1 );
				else if( i < 99 )
					sprintf( tempStr, "Item_Res_0%d.tga", i+1 );
				else 
					sprintf( tempStr, "Item_Res_%d.tga", i+1 );
				
				imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);				
			}
			break;
		case _XGI_FC_QUEST :
			{			
				sIconNo = g_QuestItemProperty[sID].sIcon_No - 1;

				int i = sIconNo / 64;
				
				sIconNo -= (64*i);
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;
				
				if( i < 9 )
					sprintf( tempStr, "Item_que_00%d.tga", i+1 );
				else 
					sprintf( tempStr, "Item_que_0%d.tga", i+1 );				
				
				imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);
			}
			break;
		case _XGI_FC_ACCESSORY :
			{
				sIconNo = g_AccessoryItemProperty[sID].sIcon_No - 1;
				
				int i = sIconNo / 64;
				
				sIconNo -= (64*i);
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;
				
				if( i < 9 )
					sprintf( tempStr, "Item_Acc_00%d.tga", i+1 );
				else 
					sprintf( tempStr, "Item_Acc_0%d.tga", i+1 );				
				
				imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);
			}
			break;
		case _XGI_FC_LIFE :
			{
				sIconNo = g_LifeItemProperty[sID].sIcon_No - 1;
				
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;

				imageindex = g_MainInterfaceTextureArchive.FindResource("Item_Life_001.tga");
			}
			break;
		case _XGI_FC_SOCKET :
			{
				sIconNo = g_SocketItemProperty[sID].sIcon_No - 1;

				int i = sIconNo / 64;
				
				sIconNo -= (64*i);
				iconclipcolumn = sIconNo % 8;
				iconcliprow    = sIconNo / 8;
				
				if( i < 9 )
					sprintf( tempStr, "Item_Sck_00%d.tga", i+1 );
				else 
					sprintf( tempStr, "Item_Sck_0%d.tga", i+1 );				
				
				imageindex = g_MainInterfaceTextureArchive.FindResource(tempStr);
			}
			break;
		case _XGI_FC_BOX :
			{
				sIconNo = g_BoxItemProperty[sID].sIcon_No - 1;

				if( sIconNo < 64 )
				{
					iconclipcolumn = sIconNo % 8;
					iconcliprow    = sIconNo / 8;
				
					imageindex = g_MainInterfaceTextureArchive.FindResource("Item_GBox_01.tga");
				}
			}
			break;
		case _XGI_FC_BOXKEY :
			{
				sIconNo = g_BoxKeyItemProperty[sID].sIcon_No - 1;

				if( sIconNo < 64 )
				{
					iconclipcolumn = sIconNo % 8;
					iconcliprow    = sIconNo / 8;
				
					imageindex = g_MainInterfaceTextureArchive.FindResource("Item_GKey_01.tga");
				}
			}
			break;
		case _XGI_FC_MONEY :
			{
				sIconNo = 0;

				rect.left = 0;
				rect.right = 32;
				rect.top = 0;
				rect.bottom = 32;
				
				rect.left += 1;
				rect.top += 1;
				rect.right -= 1;
				rect.bottom -= 1;
				
				imageindex = g_MainInterfaceTextureArchive.FindResource("item_mon.tga");
			}
			break;
		default :
			break;
		}
#endif
	}
	else
	{
		if( cType == _XGI_FC_MONEY)
		{
			sIconNo = 0;

			rect.left = 0;
			rect.right = 32;
			rect.top = 0;
			rect.bottom = 32;
			
			rect.left += 1;
			rect.top += 1;
			rect.right -= 1;
			rect.bottom -= 1;
			
			imageindex = g_MainInterfaceTextureArchive.FindResource("item_mon.tga");
		}
	}

	if(sIconNo != -1 && imageindex > -1)
	{
		rect.left    = iconclipcolumn * 32;
		rect.right   = rect.left + 32;
		
		rect.top     = iconcliprow	   * 32;
		rect.bottom  = rect.top  + 32;
		
		if(gender == _XGENDER_WOMAN)
		{
			rect.top     += 128;
			rect.bottom  += 128;
		}
		
		if(smallicon)
		{
			rect.left  += 1;
			rect.top   += 1;
			rect.right -= 1;
			rect.bottom-= 1;
		}
	}
	else
	{
		// icon ���� ������ ó��
		rect.left    = 33;
		rect.top	 = 1;
		
		rect.right   = 63;
		rect.bottom  = 31;
		
		imageindex = g_MainInterfaceTextureArchive.FindResource("item_pot_w3_003.tga");
	}
}

char _XWindow_Inventory::GetMaxSocketCount(char ctype, short sID)
{
	if( sID < 1 )	return -1;

	char itemtype = GetEqualItemType(ctype);
	switch(GetEqualItemType(ctype)) 
	{
	case _XGI_FC_WEAPON:
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(ctype);
			if( WeaponItem )
				return WeaponItem[sID].ucMaxSocket;
		}
		break;
	case _XGI_FC_CLOTHES:
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(ctype);
			if( ClothesItem )
				return ClothesItem[sID].ucMaxSocket;
		}
		break;
	}
		
	return -1;
}

_XInventoryEquipErrorCode _XWindow_Inventory::CheckSlotType(int slotindex)
{
	switch(slotindex)
	{
	case _XINVENTORY_SLOTTYPE_AMOR :			// 0	����
		{
			BOOL accCount = FALSE;
			if( _XGI_FC_CLOTHES != GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
			{
				return _XEQUIPERROR_TYPE;
			}

			_XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
			if( tempClothesitem )
			{
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSexType != g_pLocalUser->m_CharacterInfo.gender)
				{
					return _XEQUIPERROR_SEX;
				}
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
				{
					return _XEQUIPERROR_LEVEL;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSecondType != _XGI_SC_CLOTHES_UPPER)
				{
					return _XEQUIPERROR_TYPE;
				}
				//Author : ����� //breif : ����Ǿ� �ϴµ� ���� ���Ѵٰ� ��
				int nAccCount = 0;
				for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1; i <= _XINVENTORY_SLOTTYPE_ACCESSORY_4; i++ )
				{
					if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
					{
						nAccCount++;
					}
				}

                int nSID = m_MouseSlotItem.Get_m_sID();
				//if( tempClothesitem[nSID].cPocket < nAccCount )
                    //return _XEQUIPERROR_POCKET;
				
                _XGI_ClothesItem_Property* tempClothesitem2 = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_AMOR].Get_m_cType());
                int nSID1 = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_AMOR].Get_m_sID(); //���� �԰��ִ� ��

                if( nAccCount && tempClothesitem[nSID].cPocket != tempClothesitem2[nSID1].cPocket )
                    return _XEQUIPERROR_POCKET;							
#ifdef _XTS_FAME
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 0 )
				{
					int famecheck = (tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 100 ) ? 1 : 0;
					switch(g_NetworkKernel.CheckEmptyFameItem()) // ��, �Ǹ� ������ ���� ����
					{
					case 0 :
						{
							g_pLocalUser->m_FameAttrib = famecheck;
							return _XEQUIPERROR_NO;
						}
						break;
					case 1 :	// �Ǹ�,�� �������� �±�ȯ ������ ���
						{
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_AMOR].Get_m_sID() > 0 )
							{
								if( tempClothesitem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_AMOR].Get_m_sID()].ucFame > 0 ) 
								{
									g_pLocalUser->m_FameAttrib = famecheck;
									return _XEQUIPERROR_NO;
								}
							}
							else
								return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					default:
						{
							return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					}					
				}				
#endif
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan == _XGROUP_NANGIN)
				{
					// �����̸� ��� ����
					return _XEQUIPERROR_NO;
				}

				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex)
				{
					return _XEQUIPERROR_CLAN;
				}

#ifdef _XDEF_ITEM_V25
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != 0 &&
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != g_pLocalUser->m_CharacterInfo.Get_cClass()))
				{
					return _XEQUIPERROR_CLASS;
				}

				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != 0 && 
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != g_pLocalUser->m_CharacterInfo.Get_cClassGrade()))
				{
					return _XEQUIPERROR_GRADE;
				}
#endif
			}			
			return _XEQUIPERROR_NO;
		}
		break;
	case _XINVENTORY_SLOTTYPE_PANTS :			// 1	����
		{
			BOOL accCount = FALSE;
			if( _XGI_FC_CLOTHES != GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
			{
				return _XEQUIPERROR_TYPE;
			}

			_XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
			if( tempClothesitem )
			{
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSexType != g_pLocalUser->m_CharacterInfo.gender) 
				{
					return _XEQUIPERROR_SEX;
				}
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
				{
					return _XEQUIPERROR_LEVEL;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSecondType != _XGI_SC_CLOTHES_LOWER) 
				{
					return _XEQUIPERROR_TYPE;
				}

				//Author : ����� //breif : ����Ǿ� �ϴµ� ���� ���Ѵٰ� ��
				int nAccCount = 0;
				for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_5; i <= _XINVENTORY_SLOTTYPE_ACCESSORY_8; i++ )
				{
					if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
					{
						nAccCount++;
					}
				}
				
                int nSID = m_MouseSlotItem.Get_m_sID();
				//if( tempClothesitem[nSID].cPocket < nAccCount)
                  // return _XEQUIPERROR_POCKET;						

                _XGI_ClothesItem_Property* tempClothesitem2 = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_PANTS].Get_m_cType());
                int nSID1 = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_PANTS].Get_m_sID();

                if( nAccCount && tempClothesitem[nSID].cPocket != tempClothesitem2[nSID1].cPocket )
                    return _XEQUIPERROR_POCKET;		
#ifdef _XTS_FAME
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 0 )
				{
					int famecheck = (tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 100 ) ? 1 : 0;
					switch(g_NetworkKernel.CheckEmptyFameItem()) // ��, �Ǹ� ������ ���� ����
					{
					case 0 :
						{
							g_pLocalUser->m_FameAttrib = famecheck;
						}
						break;
					case 1 :	// �Ǹ�,�� �������� �±�ȯ ������ ���
						{
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_PANTS].Get_m_sID() > 0 )
							{
								if( tempClothesitem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_PANTS].Get_m_sID()].ucFame > 0 ) 
								{
									g_pLocalUser->m_FameAttrib = famecheck;
								}
							}
							else
								return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					default:
						{
							return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					}					
				}				
#endif
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan == _XGROUP_NANGIN)
				{
					// �����̸� ��� ����
					return _XEQUIPERROR_NO;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex)
				{
					return _XEQUIPERROR_CLAN;
				}
#ifdef _XDEF_ITEM_V25
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != 0 &&
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != g_pLocalUser->m_CharacterInfo.Get_cClass()))
				{
					return _XEQUIPERROR_CLASS;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != 0 && 
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != g_pLocalUser->m_CharacterInfo.Get_cClassGrade()))
				{
					return _XEQUIPERROR_GRADE;
				}
#endif
			}
			return _XEQUIPERROR_NO;
		}
		break;
	case _XINVENTORY_SLOTTYPE_CAP :				// 2	����
		{
			if( _XGI_FC_CLOTHES != GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
			{
				return _XEQUIPERROR_TYPE;
			}
			
			_XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
			if( tempClothesitem )
			{
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSexType != g_pLocalUser->m_CharacterInfo.gender) 
				{
					return _XEQUIPERROR_SEX;
				}
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
				{
					return _XEQUIPERROR_LEVEL;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSecondType != _XGI_SC_CLOTHES_CAP) 
				{
					return _XEQUIPERROR_TYPE;
				}
				
				// �Ҹ���� �Ҹ� ���ڸ� ���� �ִ�.
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM )
				{
					if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan != _XGROUP_SORIM &&
						tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan != _XGROUP_NANGIN ) 
						return _XEQUIPERROR_TYPE;
				}
							
#ifdef _XTS_FAME
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 0 )
				{
					int famecheck = (tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 100 ) ? 1 : 0;
					switch(g_NetworkKernel.CheckEmptyFameItem()) // ��, �Ǹ� ������ ���� ����
					{
					case 0 :
						{
							g_pLocalUser->m_FameAttrib = famecheck;
						}
						break;
					case 1 :	// �Ǹ�,�� �������� �±�ȯ ������ ���
						{
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CAP].Get_m_sID() > 0 )
							{
								if( tempClothesitem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CAP].Get_m_sID()].ucFame > 0 ) 
								{
									g_pLocalUser->m_FameAttrib = famecheck;
								}
							}
							else
								return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					default:
						{
							return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					}					
				}				
#endif
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan == _XGROUP_NANGIN)
				{
					// �����̸� ��� ����
					return _XEQUIPERROR_NO;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex)
				{
					return _XEQUIPERROR_CLAN;
				}
#ifdef _XDEF_ITEM_V25
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != 0 &&
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != g_pLocalUser->m_CharacterInfo.Get_cClass()))
				{
					return _XEQUIPERROR_CLASS;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != 0 && 
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != g_pLocalUser->m_CharacterInfo.Get_cClassGrade()))
				{
					return _XEQUIPERROR_GRADE;
				}
#endif
			}			
			return _XEQUIPERROR_NO;			
		}
		break;
	case _XINVENTORY_SLOTTYPE_ADAE :			// 3	�ƴ�
		{
			if( _XGI_FC_CLOTHES != GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
			{
				return _XEQUIPERROR_TYPE;
			}
			
			_XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
			if( tempClothesitem )
			{
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSexType != g_pLocalUser->m_CharacterInfo.gender) 
				{
					return _XEQUIPERROR_SEX;
				}
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
				{
					return _XEQUIPERROR_LEVEL;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSecondType != _XGI_SC_CLOTHES_GLOVES) 
				{
					return _XEQUIPERROR_TYPE;
				}
							
#ifdef _XTS_FAME
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 0 )
				{
					int famecheck = (tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 100 ) ? 1 : 0;
					switch(g_NetworkKernel.CheckEmptyFameItem()) // ��, �Ǹ� ������ ���� ����
					{
					case 0 :
						{
							g_pLocalUser->m_FameAttrib = famecheck;
						}
						break;
					case 1 :	// �Ǹ�,�� �������� �±�ȯ ������ ���
						{
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_ADAE].Get_m_sID() > 0 )
							{
								if( tempClothesitem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_ADAE].Get_m_sID()].ucFame > 0 ) 
								{
									g_pLocalUser->m_FameAttrib = famecheck;
								}
							}
							else
								return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					default:
						{
							return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					}					
				}				
#endif
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan == _XGROUP_NANGIN)
				{
					// �����̸� ��� ����
					return _XEQUIPERROR_NO;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex)  
				{
					return _XEQUIPERROR_CLAN;
				}
#ifdef _XDEF_ITEM_V25
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != 0 &&
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != g_pLocalUser->m_CharacterInfo.Get_cClass()))
				{
					return _XEQUIPERROR_CLASS;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != 0 && 
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != g_pLocalUser->m_CharacterInfo.Get_cClassGrade()))
				{
					return _XEQUIPERROR_GRADE;
				}
#endif
			}
			return _XEQUIPERROR_NO;			
		}
		break;
	case _XINVENTORY_SLOTTYPE_SHOES :			// 4	�Ź�
		{
			if( _XGI_FC_CLOTHES != GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
			{
				return _XEQUIPERROR_TYPE;
			}
			
			_XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
			if( tempClothesitem )
			{
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSexType != g_pLocalUser->m_CharacterInfo.gender)
				{
					return _XEQUIPERROR_SEX;
				}
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
				{
					return _XEQUIPERROR_LEVEL;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSecondType != _XGI_SC_CLOTHES_SHOES) 
				{
					return _XEQUIPERROR_TYPE;
				}
							
#ifdef _XTS_FAME
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 0 )
				{
					int famecheck = (tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 100 ) ? 1 : 0;
					switch(g_NetworkKernel.CheckEmptyFameItem()) // ��, �Ǹ� ������ ���� ����
					{
					case 0 :
						{
							g_pLocalUser->m_FameAttrib = famecheck;
						}
						break;
					case 1 :	// �Ǹ�,�� �������� �±�ȯ ������ ���
						{
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_SHOES].Get_m_sID() > 0 )
							{
								if( tempClothesitem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_SHOES].Get_m_sID()].ucFame > 0 ) 
								{
									g_pLocalUser->m_FameAttrib = famecheck;
								}
							}
							else
								return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					default:
						{
							return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					}					
				}				
#endif
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan == _XGROUP_NANGIN)
				{
					// �����̸� ��� ����
					return _XEQUIPERROR_NO;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex)
				{
					return _XEQUIPERROR_CLAN;
				}
#ifdef _XDEF_ITEM_V25
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != 0 &&
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != g_pLocalUser->m_CharacterInfo.Get_cClass()))
				{
					return _XEQUIPERROR_CLASS;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != 0 && 
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != g_pLocalUser->m_CharacterInfo.Get_cClassGrade()))
				{
					return _XEQUIPERROR_GRADE;
				}
#endif
			}			
			return _XEQUIPERROR_NO;			
		}
		break;
	case _XINVENTORY_SLOTTYPE_GAP_UPPER :		// 5	���� ��
	case _XINVENTORY_SLOTTYPE_GAP_LOWER :		// 6	���� ��
		{			
#ifdef _XDEF_PERIODITEM_20061027
			
			if( m_MouseSlotItem.Get_m_cType() != _XGI_FC_ACCESSORY )
			{
				return _XEQUIPERROR_TYPE;
			}
			
			if( g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType != _XGI_SC_ACCESSORY_ARMOR_MULTI 
				&& g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType != _XGI_SC_ACCESSORY_ARMOR )
			{
				return _XEQUIPERROR_TYPE;
			}
			
			if( g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
			{
				return _XEQUIPERROR_LEVEL;
			}

			if( m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART )
			{
				if( g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].cClan != 0 && 
					g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex && 
					g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].cClan != 20 )
				{
					return _XEQUIPERROR_CLAN;
				}

				if( g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_cSecond() == 0 && g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].cClan == 20 )
				{
					for( int i = _XINVENTORY_SLOTTYPE_GAP_UPPER ; i <= _XINVENTORY_SLOTTYPE_GAP_LOWER; i++ )
					{
						if( i != slotindex )
						{
							if( m_MouseSlotItem.Get_m_sID() == g_pLocalUser->m_UserItemList[i].Get_m_sID() )
							{
								return _XEQUIPERROR_ACCOVERLAPPING;
							}
						}
					}
				}
				
#ifdef _XTS_ITEM_V22
				int retvalue = _XGameItem::CheckAccessoryReq(m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID());
				if(retvalue != 0)
				{
					switch(retvalue)
					{
					case 1 :
						return _XEQUIPERROR_STR;
					case 2 : 
						return _XEQUIPERROR_CONSTITUTION;
					case 3 :
						return _XEQUIPERROR_ZEN;
					case 4 :
						return _XEQUIPERROR_INT;
					case 5 :
						return _XEQUIPERROR_DEX;
					case 6 :
						return _XEQUIPERROR_FAME;
					case 7 :
						return _XEQUIPERROR_INFAMY;
					}
				}
#endif
				
#ifdef _XTS_ACCESSORY_NEWTYPE
				if(g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].m_cSecond >= _XGI_SC_ACCESSORY_STRENGTH &&
					g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].m_cSecond <= _XGI_SC_ACCESSORY_DEX)
				{
					for(int i = _XINVENTORY_SLOTTYPE_GAP_UPPER ; i <= _XINVENTORY_SLOTTYPE_GAP_LOWER ; ++i)
					{
						if(i != slotindex)
						{
							// second type�� ���� �������� �ߺ� ���� �Ұ� 11~15
							if(g_pLocalUser->m_UserItemList[i].m_cSecond == g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].m_cSecond)
								return _XEQUIPERROR_NOOVERLAPPING;
						}
					}
				}
#endif
				
				// �ߺ� ���� �Ұ�
				if( g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType == _XGI_SC_ACCESSORY_ARMOR )
				{
					for( int i = _XINVENTORY_SLOTTYPE_GAP_UPPER; i <= _XINVENTORY_SLOTTYPE_GAP_LOWER; i++ )
					{								
						if( g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_MouseSlotItem.Get_m_sID() )
						{
							return _XEQUIPERROR_PERIODITEM_ACCOVERLAPPING;
						}							
					}							
				}
				
			}			
			
			return _XEQUIPERROR_NO;
#else
			return _XEQUIPERROR_OTHER;
#endif
		}
		break;
	case _XINVENTORY_SLOTTYPE_BACKPACK1 :		// 7	�賶	
		{
			if( _XGI_FC_CLOTHES != GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
				return _XEQUIPERROR_TYPE;

			_XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
			if( tempClothesitem )
			{
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSexType != g_pLocalUser->m_CharacterInfo.gender)
				{
					return _XEQUIPERROR_SEX;
				}
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
				{
					return _XEQUIPERROR_LEVEL;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSecondType != _XGI_SC_CLOTHES_BACKPACK)
				{
					return _XEQUIPERROR_TYPE;
				}
            #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
                if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cCashCheck != 0)
				{
					return _XEQUIPERROR_TYPE;
				}
            #endif
							
#ifdef _XTS_FAME
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 0 )
				{
					int famecheck = (tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 100 ) ? 1 : 0;
					switch(g_NetworkKernel.CheckEmptyFameItem()) // ��, �Ǹ� ������ ���� ����
					{
					case 0 :
						{
							g_pLocalUser->m_FameAttrib = famecheck;
						}
						break;
					case 1 :	// �Ǹ�,�� �������� �±�ȯ ������ ���
						{
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 )
							{
								if( tempClothesitem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID()].ucFame > 0 ) 
								{
									g_pLocalUser->m_FameAttrib = famecheck;
								}
							}
							else
								return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					default:
						{
							return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					}					
				}				
#endif
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan == _XGROUP_NANGIN)
				{
					// �����̸� ��� ����
					return _XEQUIPERROR_NO;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex)
				{
					return _XEQUIPERROR_CLAN;
				}
#ifdef _XDEF_ITEM_V25
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != 0 &&
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != g_pLocalUser->m_CharacterInfo.Get_cClass()))
				{
					return _XEQUIPERROR_CLASS;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != 0 && 
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != g_pLocalUser->m_CharacterInfo.Get_cClassGrade()))
				{
					return _XEQUIPERROR_GRADE;
				}
#endif
			}			
			return _XEQUIPERROR_NO;
		}
		break;
	case _XINVENTORY_SLOTTYPE_BACKPACK2 :		// 18	�賶2
		{
			if( _XGI_FC_CLOTHES != GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
			{
				return _XEQUIPERROR_TYPE;
			}

			_XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
			if( tempClothesitem )
			{
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSexType != g_pLocalUser->m_CharacterInfo.gender)
				{
					return _XEQUIPERROR_SEX;
				}
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
				{
					return _XEQUIPERROR_LEVEL;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSecondType != _XGI_SC_CLOTHES_BACKPACK)
				{
					return _XEQUIPERROR_TYPE;
				}
				if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() <= 0 )
				{
					return _XEQUIPERROR_OTHER;
				}
            #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
                if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cCashCheck != 0)
				{
					return _XEQUIPERROR_TYPE;
				}
            #endif
							
#ifdef _XTS_FAME
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 0 )
				{
					int famecheck = (tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 100 ) ? 1 : 0;
					switch(g_NetworkKernel.CheckEmptyFameItem()) // ��, �Ǹ� ������ ���� ����
					{
					case 0 :
						{
							g_pLocalUser->m_FameAttrib = famecheck;
						}
						break;
					case 1 :	// �Ǹ�,�� �������� �±�ȯ ������ ���
						{
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0 )
							{
								if( tempClothesitem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID()].ucFame > 0 ) 
								{
									g_pLocalUser->m_FameAttrib = famecheck;
								}
							}
							else
								return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					default:
						{
							return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					}					
				}				
#endif
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan == _XGROUP_NANGIN)
				{
					// �����̸� ��� ����
					return _XEQUIPERROR_NO;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex)
				{
					return _XEQUIPERROR_CLAN;
				}
#ifdef _XDEF_ITEM_V25
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != 0 &&
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != g_pLocalUser->m_CharacterInfo.Get_cClass()))
				{
					return _XEQUIPERROR_CLASS;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != 0 && 
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != g_pLocalUser->m_CharacterInfo.Get_cClassGrade()))
				{
					return _XEQUIPERROR_GRADE;
				}
#endif
			}
			return _XEQUIPERROR_NO;
		}
		break;
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	case _XINVENTORY_SLOTTYPE_CASHBACKPACK1 :
		{
			if( _XGI_FC_CLOTHES != GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
			{
				return _XEQUIPERROR_TYPE;
			}

			_XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
			if( tempClothesitem )
			{
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSexType != g_pLocalUser->m_CharacterInfo.gender)
				{
					return _XEQUIPERROR_SEX;
				}
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
				{
					return _XEQUIPERROR_LEVEL;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSecondType != _XGI_SC_CLOTHES_BACKPACK)
				{
					return _XEQUIPERROR_TYPE;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cCashCheck == 0)
				{
					return _XEQUIPERROR_TYPE;
				}
							
			#ifdef _XTS_FAME
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 0 )
				{
					int famecheck = (tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 100 ) ? 1 : 0;
					switch(g_NetworkKernel.CheckEmptyFameItem()) // ��, �Ǹ� ������ ���� ����
					{
					case 0 :
						{
							g_pLocalUser->m_FameAttrib = famecheck;
						}
						break;
					case 1 :	// �Ǹ�,�� �������� �±�ȯ ������ ���
						{
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 )
							{
								if( tempClothesitem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID()].ucFame > 0 ) 
								{
									g_pLocalUser->m_FameAttrib = famecheck;
								}
							}
							else
								return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					default:
						{
							return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					}					
				}				
			#endif
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan == _XGROUP_NANGIN)
				{
					// �����̸� ��� ����
					return _XEQUIPERROR_NO;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex)
				{
					return _XEQUIPERROR_CLAN;
				}
			#ifdef _XDEF_ITEM_V25
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != 0 &&
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != g_pLocalUser->m_CharacterInfo.Get_cClass()))
				{
					return _XEQUIPERROR_CLASS;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != 0 && 
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != g_pLocalUser->m_CharacterInfo.Get_cClassGrade()))
				{
					return _XEQUIPERROR_GRADE;
				}
			#endif
			}
			return _XEQUIPERROR_NO;
		}
		break;
	case _XINVENTORY_SLOTTYPE_CASHBACKPACK2 :
		{
            return _XEQUIPERROR_TYPE; //ĳ���೶ 2�� �켱 ���� ���´�

			if( _XGI_FC_CLOTHES != GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
			{
				return _XEQUIPERROR_TYPE;
			}

			_XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
			if( tempClothesitem )
			{
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSexType != g_pLocalUser->m_CharacterInfo.gender)
				{
					return _XEQUIPERROR_SEX;
				}
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
				{
					return _XEQUIPERROR_LEVEL;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSecondType != _XGI_SC_CLOTHES_BACKPACK)
				{
					return _XEQUIPERROR_TYPE;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cCashCheck == 0)
				{
					return _XEQUIPERROR_TYPE;
				}
                if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
				{
					return _XEQUIPERROR_OTHER;
				}
							
			#ifdef _XTS_FAME
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 0 )
				{
					int famecheck = (tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 100 ) ? 1 : 0;
					switch(g_NetworkKernel.CheckEmptyFameItem()) // ��, �Ǹ� ������ ���� ����
					{
					case 0 :
						{
							g_pLocalUser->m_FameAttrib = famecheck;
						}
						break;
					case 1 :	// �Ǹ�,�� �������� �±�ȯ ������ ���
						{
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() > 0 )
							{
								if( tempClothesitem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID()].ucFame > 0 ) 
								{
									g_pLocalUser->m_FameAttrib = famecheck;
								}
							}
							else
								return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					default:
						{
							return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					}					
				}				
			#endif
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan == _XGROUP_NANGIN)
				{
					// �����̸� ��� ����
					return _XEQUIPERROR_NO;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex)
				{
					return _XEQUIPERROR_CLAN;
				}
			#ifdef _XDEF_ITEM_V25
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != 0 &&
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != g_pLocalUser->m_CharacterInfo.Get_cClass()))
				{
					return _XEQUIPERROR_CLASS;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != 0 && 
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != g_pLocalUser->m_CharacterInfo.Get_cClassGrade()))
				{
					return _XEQUIPERROR_GRADE;
				}
			#endif
			}
			return _XEQUIPERROR_NO;
		}
		break;
#endif
	case _XINVENTORY_SLOTTYPE_MASK :
		{
			if( _XGI_FC_CLOTHES != GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
			{
				return _XEQUIPERROR_TYPE;
			}

			_XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
			if( tempClothesitem )
			{
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSexType != g_pLocalUser->m_CharacterInfo.gender) 
				{
					return _XEQUIPERROR_SEX;
				}
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
				{
					return _XEQUIPERROR_LEVEL;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cSecondType != _XGI_SC_CLOTHES_MASK) 
				{
					return _XEQUIPERROR_TYPE;
				}
							
#ifdef _XTS_FAME
				if( tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 0 )
				{
					int famecheck = (tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucFame > 100 ) ? 1 : 0;
					switch(g_NetworkKernel.CheckEmptyFameItem()) // ��, �Ǹ� ������ ���� ����
					{
					case 0 :
						{
							g_pLocalUser->m_FameAttrib = famecheck;
						}
						break;
					case 1 :	// �Ǹ�,�� �������� �±�ȯ ������ ���
						{
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_MASK].Get_m_sID() > 0 )
							{
								if( tempClothesitem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_MASK].Get_m_sID()].ucFame > 0 ) 
								{
									g_pLocalUser->m_FameAttrib = famecheck;
								}
							}
							else
								return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					default:
						{
							return _XEQUIPERROR_FAMEOVERLAPPING;
						}
						break;
					}					
				}				
#endif
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan == _XGROUP_NANGIN)
				{
					// �����̸� ��� ����
					return _XEQUIPERROR_NO;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex)
				{
					return _XEQUIPERROR_CLAN;
				}
#ifdef _XDEF_ITEM_V25
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != 0 &&
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].cClass != g_pLocalUser->m_CharacterInfo.Get_cClass()))
				{
					return _XEQUIPERROR_CLASS;
				}
				if(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != 0 && 
					(tempClothesitem[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != g_pLocalUser->m_CharacterInfo.Get_cClassGrade()))
				{
					return _XEQUIPERROR_GRADE;
				}
#endif
			}
			return _XEQUIPERROR_NO;
		}
		break;
	case _XINVENTORY_SLOTTYPE_WEAPON_1 :		// 8	���� 1
	case _XINVENTORY_SLOTTYPE_WEAPON_2 :		// 9	���� 2
		{				
#ifdef _XTS_FAME
			if( _XGI_FC_WEAPON == GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
			{	
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
				if( WeaponItem )
				{
					if( WeaponItem[m_MouseSlotItem.Get_m_sID()].ucFame > 0 )
					{
						int famecheck = (WeaponItem[m_MouseSlotItem.Get_m_sID()].ucFame > 100 ) ? 1 : 0;
						
						if( g_pLocalUser->m_UserItemList[slotindex].Get_m_sID() > 0 )
						{
							if( g_NetworkKernel.CheckEmptyFameItem() == 1 && 
								WeaponItem[g_pLocalUser->m_UserItemList[slotindex].Get_m_sID()].ucFame > 0 ) 
							{
								g_pLocalUser->m_FameAttrib = famecheck;
								return _XEQUIPERROR_NO;
							}
						}				
						
						if( g_pLocalUser->m_FameAttrib == -1 )
						{
							g_pLocalUser->m_FameAttrib = famecheck;						
						}
						else
						{
							if( g_pLocalUser->m_FameAttrib != famecheck )
								return _XEQUIPERROR_FAMEOVERLAPPING;
						}
					}		
				}				
			}
#endif
			if( _XGI_FC_WEAPON == GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
				return _XEQUIPERROR_NO;
			else
				return _XEQUIPERROR_TYPE;
		}
		break;

	case _XINVENTORY_SLOTTYPE_ACCESSORY_1 :
	case _XINVENTORY_SLOTTYPE_ACCESSORY_2 :
	case _XINVENTORY_SLOTTYPE_ACCESSORY_3 :
	case _XINVENTORY_SLOTTYPE_ACCESSORY_4 :
	case _XINVENTORY_SLOTTYPE_ACCESSORY_5 :
	case _XINVENTORY_SLOTTYPE_ACCESSORY_6 :
	case _XINVENTORY_SLOTTYPE_ACCESSORY_7 :
	case _XINVENTORY_SLOTTYPE_ACCESSORY_8 :
		{
			if( m_MouseSlotItem.Get_m_cType() != _XGI_FC_ACCESSORY )
				return _XEQUIPERROR_TYPE;
			//�� �ϰ���ġ�� �ִ� �������� �� �ָӴϿ� ������ �� ����.
			if( g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType == _XGI_SC_ACCESSORY_ARMOR_MULTI 
				|| g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType == _XGI_SC_ACCESSORY_ARMOR )
				return _XEQUIPERROR_TYPE;
			
			if( g_pLocalUser->m_cAmorPocketNum <= 0 && g_pLocalUser->m_cPantsPocketNum <= 0)
				return _XEQUIPERROR_ACCESSORY;

            //
			if( g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
				return _XEQUIPERROR_LEVEL;
			if( m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART )
			{
				if( g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].cClan != 0 && g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex 
					&& g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].cClan != 20 )
					return _XEQUIPERROR_CLAN;
				if( g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_cSecond() == 0 && g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].cClan == 20 )
				{
					for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1 ; i <= _XINVENTORY_SLOTTYPE_ACCESSORY_8; i++ )
					{
						if( i != slotindex )
						{
							if( m_MouseSlotItem.Get_m_sID() == g_pLocalUser->m_UserItemList[i].Get_m_sID() )
								return _XEQUIPERROR_ACCOVERLAPPING;
						}
					}
				}
			
#ifdef _XTS_ITEM_V22
				int retvalue = _XGameItem::CheckAccessoryReq(m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID());
				if(retvalue != 0)
				{
					switch(retvalue)
					{
					case 1 :
						return _XEQUIPERROR_STR;
					case 2 : 
						return _XEQUIPERROR_CONSTITUTION;
					case 3 :
						return _XEQUIPERROR_ZEN;
					case 4 :
						return _XEQUIPERROR_INT;
					case 5 :
						return _XEQUIPERROR_DEX;
					case 6 :
						return _XEQUIPERROR_INFAMY;
					case 7 :
						return _XEQUIPERROR_FAME;
					}
				}
#endif

#ifdef _XTS_ACCESSORY_NEWTYPE
				if(g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].m_cSecond >= _XGI_SC_ACCESSORY_STRENGTH &&
					g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].m_cSecond <= _XGI_SC_ACCESSORY_DEX)
				{
					for(int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1 ; i <= _XINVENTORY_SLOTTYPE_ACCESSORY_8 ; ++i)
					{
						if(i != slotindex)
						{
							// second type�� ���� �������� �ߺ� ���� �Ұ� 11~15
							if(g_pLocalUser->m_UserItemList[i].m_cSecond == g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].m_cSecond)
								return _XEQUIPERROR_NOOVERLAPPING;
						}
					}
				}
#endif
			}			
			
#ifdef _XTS_FAME
/*			if( g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].ucFame > 0 )
			{
				int famecheck = (g_AccessoryItemProperty[m_MouseSlotItem.Get_m_sID()].ucFame > 100 ) ? 1 : 0;
				switch(g_NetworkKernel.CheckEmptyFameItem()) // ��, �Ǹ� ������ ���� ����
				{
				case 0 :
					{
						g_pLocalUser->m_FameAttrib = famecheck;
					}
					break;
				case 1 :	// �Ǹ�,�� �������� �±�ȯ ������ ���
					{
						if( g_pLocalUser->m_UserItemList[slotindex].Get_m_sID() > 0 )
						{
							if( g_AccessoryItemProperty[g_pLocalUser->m_UserItemList[slotindex].Get_m_sID()].ucFame > 0 ) 
							{
								g_pLocalUser->m_FameAttrib = famecheck;
							}
						}
						else
							return _XEQUIPERROR_FAMEOVERLAPPING;
					}
					break;
				default:
					{
						return _XEQUIPERROR_FAMEOVERLAPPING;
					}
					break;
				}					
			}*/			
#endif
			return _XEQUIPERROR_NO;
		}
		break;
	}
	
	bool bexist = false;
	list <_XQUEST_HOLDITEM>::iterator iter_holditem;
	for(iter_holditem = g_QuestScriptManager.m_listHoldItem.begin() ; iter_holditem != g_QuestScriptManager.m_listHoldItem.end() ; )
	{
		_XQUEST_HOLDITEM questholditem = *iter_holditem;
		if( questholditem.m_cHoldItemType == m_MouseSlotItem.Get_m_cType() && questholditem.m_sHoldItemID == m_MouseSlotItem.Get_m_sID() )
		{
			bexist = true;
			return _XEQUIPERROR_QUESTHOLDITEM;
		}
		else
		{
			iter_holditem++;
		}										
	}
	
	return _XEQUIPERROR_NO;
}

void _XWindow_Inventory::ProcessDblClick(void)
{
	if( g_pLocalUser->m_bMatchPRMode ) // �� ȫ���� �ƴҶ���  
		return;

	_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
	if( pTradeMessageBox )	return;
	_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
	if( pTradeConfirm_Window->GetShowStatus() )		return;

    //Author : ����� //breif : m_LastDraggingSlot �� ������ ������ ���� �Ǿ �˻� �Ҷ��� ������ ����.. �������� �� �����ִ�.
	int selectedslot = SearchSlotRegion();	
	if( g_pLocalUser->m_UserItemList[ selectedslot ].Get_m_sID() <= 0 ) return;	

	//Author : ����� //breif : ĳ���೶ �߰� �Ǹ鼭 ������ �˻� ���� �ϸ� �ȵ�
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    if( selectedslot >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
		selectedslot < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART )
	{
        if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
            return;
        
        /*if( _XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType()) )
		{
            _XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType());
			if( tempClothesitem )
            {
            _XGI_SC_CLOTHES_BACKPACK
                int sID = g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID();
                if( (sID == 12084 || sID == 12085 || sID == 12086 || sID == 12087 ) &&
                    tempClothesitem[sID].cCashCheck != 0 )
                {
                    //ĳ�� �೶�� ��� �����ϴ�    
                }
                else if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
                    return;
            }
        }
        else if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
            return;*/
	}

    if( selectedslot >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		selectedslot < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
	{
        if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
            return;
		/*if( _XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType()) )
		{
            _XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType());
			if( tempClothesitem )
            {
                int sID = g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID();
                if( (sID == 12084 || sID == 12085 || sID == 12086 || sID == 12087 ) &&
                    tempClothesitem[sID].cCashCheck != 0 )
                {
                    //ĳ�� �೶�� ��� �����ϴ�    
                }
                else if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
                    return;
            }
        }
        else if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
            return;*/
	}
#endif
	
	if(m_pNPCTradeWindow)
	{
		if(m_pNPCTradeWindow->GetShowStatus())
		{
			if(CheckMousePosition())
			{
				if( selectedslot != -1 && g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID() > 0 && 
					selectedslot >= _XINVENTORY_SLOTTYPE_POCKETSTART )
				{
					if( _XGameItem::CheckNPCTrade( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType(), g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID() ) )
					{						
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2120), TRUE);
						return;
					}

					if(m_nPriceMode == _MODE_CLANPOINT)
					{
						_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
						pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3798), TRUE);//�ش� NPC���Դ� �������� �Ǹ��� �� �����ϴ�.
						return;
					}
				#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
					if( m_nPriceMode == _MODE_MNDPOINT )
					{
						_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
						pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3798), TRUE);//�ش� NPC���Դ� �������� �Ǹ��� �� �����ϴ�.
						return;
					}
				#endif
					
					m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[selectedslot]);
					m_MouseSlotItem.m_InventoryNumber = selectedslot;

					_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
					
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);
					int resultcashcheck = CheckCashItem(m_MouseSlotItem.Get_m_cType(),m_MouseSlotItem.Get_m_sID());
					if( resultcashcheck == 1 || resultcashcheck == 2 )
					{
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_2981) );	// ĳ�þ������� ������ �Ǹ� ������ �����Ǹ� ȯ��/���� ���� �ʽ��ϴ�.�Ǹ��Ͻðڽ��ϱ�?
					}
					else 
					{
						TCHAR		namestring[128];
						memset(namestring, 0, sizeof(TCHAR)*128);
						strcpy(namestring, _XGameItem::GetItemName(m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID()));
					
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INVENTORY_SELL), namestring);	// _T("%s(��)�� �Ǹ��Ͻðڽ��ϱ�?")									
					}
					
					pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_NPCTRADE_SELLOK, _XDEF_NPCTRADE_SELLNO);
				
					m_LastDraggingSlot = -1;
				}
			}
			return;
		}
	}

	if(m_pPCTradeWindow)
	{
		if(m_pPCTradeWindow->GetShowStatus())
		{
			if(CheckMousePosition())
			{
				if(selectedslot != -1 && selectedslot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
				{
					if( _XGameItem::CheckUserTrade( g_pLocalUser->m_UserItemList[ selectedslot ].Get_m_cType(), g_pLocalUser->m_UserItemList[ selectedslot ].Get_m_sID() ) )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2123), TRUE);
						return;
					}

					m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[selectedslot]);
					m_MouseSlotItem.m_InventoryNumber = selectedslot;
					if(m_MouseSlotItem.IsStackable())
					{
						_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
						if(pTradeConfirm_Window)
						{
							int nextslot = m_pPCTradeWindow->GetNextEmptySlot();
							if(nextslot != -1)
							{
								pTradeConfirm_Window->ShowWindow(TRUE);									
								pTradeConfirm_Window->m_IMEControl.SetFocus();
								pTradeConfirm_Window->m_SelectedCategory1 = m_MouseSlotItem.Get_m_cType();
								pTradeConfirm_Window->m_SelectedItemID = m_MouseSlotItem.Get_m_sID();
								pTradeConfirm_Window->m_InventoryNumber = m_MouseSlotItem.m_InventoryNumber; //�Ǹ� ������ ���� �߰��� //Author : �����
								m_pPCTradeWindow->m_SelfTradeItem[nextslot].m_InventoryNumber = m_MouseSlotItem.m_InventoryNumber;
								pTradeConfirm_Window->m_PCTradeSlotIndex = nextslot;
								pTradeConfirm_Window->m_TradeMode = _XPCTRADE_TOTRADE_ITEM;
							}
						}
					}
					else
					{
						int nextslot = m_pPCTradeWindow->GetNextEmptySlot();
						if(nextslot != -1)
						{
							m_pPCTradeWindow->CopyItemToSlot(nextslot, m_MouseSlotItem.m_InventoryNumber);
							m_pPCTradeWindow->m_SelfTradeItem[nextslot].m_InventoryNumber = m_MouseSlotItem.m_InventoryNumber;
							g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Reset();
							ResetMouseSlot();
							g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_ADD, nextslot, 1);
						}
					}
					m_LastDraggingSlot = -1;
				}
			}
			return;
		}
	}

	if(m_pWarehouseWindow)
	{
		if(m_pWarehouseWindow->GetShowStatus())
		{
			if(CheckMousePosition())
			{
				if( m_pWarehouseWindow->m_bWaitForServerResult ) return;	// ������ �� �� ���¿����� �ٽ� ������ �ʴ´�.

				if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID() <= 0 ) return;

				if(selectedslot != -1 && selectedslot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
				{
					if( g_pLocalUser->m_UserItemList[ selectedslot ].Get_m_cType() == _XGI_FC_QUEST )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTSAVEQUEST), TRUE);
						return;
					}
#ifdef _XDEF_WAREHOSE_RESTRAINT_OF_TRADE //Author : ����� //breif : â������� ���� �߰�
					else if( _XGameItem::CheckItemTempField4( g_pLocalUser->m_UserItemList[ selectedslot ].Get_m_cType() ,
															g_pLocalUser->m_UserItemList[selectedslot ].Get_m_sID() ) )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3404), TRUE); //�ش� �������� â�� ������ �� �����ϴ�.
						return;
					}
#endif

					m_pWarehouseWindow->ReleaseTempItemList();
					m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[selectedslot]);
					m_MouseSlotItem.m_InventoryNumber = selectedslot;
					if(m_MouseSlotItem.IsStackable())
					{
						_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)m_pMyManager->FindWindow(_XDEF_WTITLE_TRADECONFIRM);
						if(pTradeConfirm_Window)
						{
							pTradeConfirm_Window->ShowWindow(TRUE);
							pTradeConfirm_Window->m_IMEControl.SetFocus();
							pTradeConfirm_Window->m_SelectedCategory1 = m_MouseSlotItem.Get_m_cType();
							pTradeConfirm_Window->m_SelectedItemID = m_MouseSlotItem.Get_m_sID();
							pTradeConfirm_Window->m_InventoryNumber = m_MouseSlotItem.m_InventoryNumber; //�Ǹ� ������ ���� �߰��� //Author : �����
							pTradeConfirm_Window->m_TradeMode = _XWAREHOUSE_TOWAREHOUSE_ITEM;
						}
					}
					else
					{
						m_pWarehouseWindow->SetTempItemFromInven(m_MouseSlotItem.m_InventoryNumber, 1);
						m_pWarehouseWindow->m_SendMode = _XSENDMODE_TOWAREHOUSE_ITEM;
						PostMessage(gHWnd, WM_COMMAND, _XDEF_WAREHOUSEWINDOW_SENDADDITEM, (LPARAM)gHWnd);
					}
				}
			}
			return;
		}
	}
	
	// ����â ó��
	if(m_pRepairItem_Window)
	{
		if(m_pRepairItem_Window->GetShowStatus())
		{
			if( m_pRepairItem_Window->m_RepairMode != RM_UNDERREPAIR )
			{
				if(CheckMousePosition())
				{
					if( selectedslot >= _XINVENTORY_SLOTTYPE_POCKETSTART && g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID() > 0 )
					{
						if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON ||
							g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() ==  _XGI_FC_CLOTHES || 
							g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON2 ||
							g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() ==  _XGI_FC_CLOTHES2 ||
							g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON3 ||
							g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() ==  _XGI_FC_CLOTHES3 )
						{
							int shortdur = g_pLocalUser->m_UserItemList[selectedslot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[selectedslot].Get_m_usCurDur();
							if( shortdur > 0)
							{
								m_pRepairItem_Window->m_SelectedSlotNoList.push_back( selectedslot );
								m_pRepairItem_Window->SetRepairListItem();
							}
							else
							{
								// ������ �ʿ䰡 ���� ��ǰ�Դϴ�.
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTNECESSERYREPAIR), TRUE);
								pMessageBox->StartTimer(5000, TRUE);
								return;
							}
							
							ResetMouseSlot();
						}
						else
						{
							// ������ �� ���� ��ǰ�Դϴ�.
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTREPAIRITEM), TRUE);									
							pMessageBox->StartTimer(5000, TRUE);
							
							ResetMouseSlot();
							return;
						}									
						m_LastDraggingSlot = -1;
					}
				}
				return;
			}
		}
	}
	// ���� ������ �� �κ��丮�� ����Ŭ�� ���� �� ó�� 	
	if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE || g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY )
	{
		if(m_pPersonalStore_Window)
		{
			if(m_pPersonalStore_Window->GetShowStatus())
			{
				if(CheckMousePosition())
				{
					if(selectedslot != -1 && selectedslot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
					{
						if( _XGameItem::CheckUserTrade( g_pLocalUser->m_UserItemList[ selectedslot ].Get_m_cType(), g_pLocalUser->m_UserItemList[ selectedslot ].Get_m_sID() ) )
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2123), TRUE);
							return;
						}
			
						m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[selectedslot]);
						m_MouseSlotItem.m_InventoryNumber = selectedslot;
						if(m_MouseSlotItem.IsStackable())
						{
							_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
							if(pTradeConfirm_Window)
							{
								int nextslot = m_pPersonalStore_Window->GetNextEmptySlot();
								if(nextslot != -1)
								{
									pTradeConfirm_Window->ShowWindow(TRUE);									
									pTradeConfirm_Window->m_IMEControl.SetFocus();
									pTradeConfirm_Window->m_SelectedCategory1 = m_MouseSlotItem.Get_m_cType();
									pTradeConfirm_Window->m_SelectedItemID = m_MouseSlotItem.Get_m_sID();
									pTradeConfirm_Window->m_InventoryNumber = m_MouseSlotItem.m_InventoryNumber; //�Ǹ� ������ ���� �߰��� //Author : �����
									m_pPersonalStore_Window->m_TradeItem[nextslot].m_InventoryNumber = m_MouseSlotItem.m_InventoryNumber;
									pTradeConfirm_Window->m_PersonalStoreSlotIndex = nextslot;
									pTradeConfirm_Window->m_TradeMode = _XPERSONALSTORE_TOTRADE_ITEM;
									pTradeConfirm_Window->MoveWindow((gnWidth>>1)-102, 344);
								}
							}
						}
						else
						{
							int nextslot = m_pPersonalStore_Window->GetNextEmptySlot();
							if(nextslot != -1)
							{
								m_pPersonalStore_Window->CopyItemToSlot(nextslot, m_MouseSlotItem.m_InventoryNumber);
								m_pPersonalStore_Window->m_TradeItem[nextslot].m_InventoryNumber = m_MouseSlotItem.m_InventoryNumber;	
								m_pPersonalStore_Window->m_SelectedSlotNumber = nextslot;
								m_pPersonalStore_Window->m_TradeItemPrice[nextslot] = 0;
								m_pPersonalStore_Window->m_TradeItemPriceIMEControl.SetFocus();
								m_pPersonalStore_Window->m_btnInputOK->ShowWindow(TRUE);
								
								g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Reset();		
								ResetMouseSlot();
							}
						}
						m_LastDraggingSlot = -1;
					}
				}
				return;
			}
		}
	}
	
	ProcessToSocketPlugIn();
	ProcessToGamble();
	ProcessToSocketPlugInEx();	
	ProcessToInchant();
	ProcessToSmeltItem(); //Author : �����
	ProcessToSocketDetach(); //Author : �����
	ProcessToMixWeapon();
	ProcessToNPCResourceNew();

	// 2004.06.17->oneway48 insert : ���� ��ư�� ������ �� ���۰� ���� ���� 
	if(m_pRepairItem_Window)
	{
		if(m_pRepairItem_Window->GetShowStatus())
		{
			if(CheckMousePosition())
			{
			}
			return;
		}
	}

	// Inventory ���� ���� Ŭ��
	ProcessItemEquip();
}

void _XWindow_Inventory::OnLButtonUseWarehouse(void)
{
	_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	if(!pWarehouse_Window)
		return;

	// â�� ��� �� : �̵��� ������ ����
	int selectedslot = SearchSlotRegion();
	if(selectedslot == -1)
		return;

	if(pWarehouse_Window->GetShowStatus())
	{
		m_LastDraggingSlot = selectedslot;
		
		if(g_pLocalUser->m_UserItemList[selectedslot].IsStackable())
		{
			// Merge�� ������ ���� ��
			if(pWarehouse_Window->HaveItemFromInven(selectedslot))
			{
				// ���õ� ������, ���� ����
				pWarehouse_Window->ResetTempItemFromInven(selectedslot);
			}
			else
			{
				_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
				if(pTradeConfirm_Window)
				{
					pTradeConfirm_Window->ShowWindow(TRUE);									
					pTradeConfirm_Window->m_IMEControl.SetFocus();
					pTradeConfirm_Window->m_SelectedCategory1 = g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType();
					pTradeConfirm_Window->m_SelectedItemID = g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID();
					pTradeConfirm_Window->m_InventoryNumber = selectedslot; //�Ǹ� ������ ���� �߰��� //Author : �����
					pTradeConfirm_Window->m_TradeMode = _XWAREHOUSE_TOWAREHOUSE_ITEM;
				}
			}																			
		}
		else
		{
			if(pWarehouse_Window->HaveItemFromInven(selectedslot))
			{
				// ���õ� ������, ���� ����
				pWarehouse_Window->ResetTempItemFromInven(selectedslot);
			}
			else
			{
				pWarehouse_Window->SetTempItemFromInven(selectedslot);
			}
		}
	}
}

void _XWindow_Inventory::OnRButtonUseWarehouse(void)
{
	_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	if(pWarehouse_Window->GetShowStatus())
	{
		int selectedslot = SearchSlotRegion();
		if(selectedslot == -1)
			return;

		if(pWarehouse_Window->HaveItemFromInven(selectedslot))
		{
			// ���� ���
			pWarehouse_Window->ResetTempItemFromInven(selectedslot);
		}
	}
}

BOOL _XWindow_Inventory::ProcessNPCTrade(void)
{
	BOOL	rtnvalue = FALSE;

	if(m_pNPCTradeWindow)
	{
		if(m_pNPCTradeWindow->GetShowStatus())
		{
			if(m_pNPCTradeWindow->m_bHaveMouseSlot)
			{
				// NPC���� ���
				if(m_pNPCTradeWindow->m_MouseSlotItem.IsStackable())
				{
					_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)m_pMyManager->FindWindow(_XDEF_WTITLE_TRADECONFIRM);
					if(pTradeConfirm_Window)
					{
						pTradeConfirm_Window->ShowWindow(TRUE);
						pTradeConfirm_Window->m_IMEControl.SetFocus();
						pTradeConfirm_Window->m_SelectedCategory1 = m_pNPCTradeWindow->m_MouseSlotItem.Get_m_cType();
						pTradeConfirm_Window->m_SelectedItemID = m_pNPCTradeWindow->m_MouseSlotItem.Get_m_sID();
						pTradeConfirm_Window->m_InventoryNumber = m_pNPCTradeWindow->m_MouseSlotItem.m_InventoryNumber; //�Ǹ� ������ ���� �߰��� //Author : �����
						pTradeConfirm_Window->m_TradeMode = _XNPCTRADE_BUY;
						m_LastDraggingSlot = -1;
					}
				}
				else
				{
					_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
					TCHAR		namestring[128];
					memset(namestring, 0, sizeof(TCHAR)*128);
					strcpy(namestring, _XGameItem::GetItemName(m_pNPCTradeWindow->m_MouseSlotItem.Get_m_cType(), m_pNPCTradeWindow->m_MouseSlotItem.Get_m_sID()));
					
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INVENTORY_BUY), namestring);	// _T("%s(��)�� �����Ͻðڽ��ϱ�?")
					
					pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_NPCTRADE_BUYOK, _XDEF_NPCTRADE_BUYNO);
				}
				rtnvalue = TRUE;
			}
		}
	}
	return rtnvalue;
}

BOOL _XWindow_Inventory::ProcessFromWarehouse(void)
{
	if(m_pWarehouseWindow)
	{
		if(m_pWarehouseWindow->GetShowStatus())
		{
			if( m_pWarehouseWindow->m_bWaitForServerResult ) return FALSE;	// ������ �� �� ���¿����� �ٽ� ������ �ʴ´�.

			m_pWarehouseWindow->m_bDrawSelectedItem = FALSE;
			if(m_pWarehouseWindow->m_bHaveMouseSlot)
			{
				// â���� ������ ����
				if(m_pWarehouseWindow->m_MouseSlotItem.IsStackable())
				{
					_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)m_pMyManager->FindWindow(_XDEF_WTITLE_TRADECONFIRM);
					if(pTradeConfirm_Window)
					{
						pTradeConfirm_Window->ShowWindow(TRUE);
						pTradeConfirm_Window->m_IMEControl.SetFocus();
						pTradeConfirm_Window->m_SelectedCategory1 = m_pWarehouseWindow->m_MouseSlotItem.Get_m_cType();
						pTradeConfirm_Window->m_SelectedItemID = m_pWarehouseWindow->m_MouseSlotItem.Get_m_sID();
						pTradeConfirm_Window->m_InventoryNumber = m_pWarehouseWindow->m_MouseSlotItem.m_InventoryNumber; //�Ǹ� ������ ���� �߰��� //Author : �����
						pTradeConfirm_Window->m_TradeMode = _XWAREHOUSE_FROMWAREHOUSE_ITEM;
						m_LastDraggingSlot = -1;
					}
				}
				else
				{
					m_pWarehouseWindow->SetTempItemFromWarehouse(m_pWarehouseWindow->m_MouseSlotItem.m_InventoryNumber, m_MouseSlotItem.Get_m_ucCount());
					m_pWarehouseWindow->m_SendMode = _XSENDMODE_TOINVEN_ITEM;
					PostMessage(gHWnd, WM_COMMAND, _XDEF_WAREHOUSEWINDOW_SENDADDITEM, (LPARAM)gHWnd);
				}
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL _XWindow_Inventory::ProcessPCTrade(void)
{
	if(m_pPCTradeWindow)
	{
		if(m_pPCTradeWindow->GetShowStatus())
		{
			if(CheckMousePosition())
			{
				if(m_pPCTradeWindow->m_bHaveMouseSlot)
				{
					g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_DELETE, m_pPCTradeWindow->m_MouseSlotItem.m_InventoryNumber,
												m_pPCTradeWindow->m_MouseSlotItem.Get_m_ucCount());
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

// 2004.11.6->hotblood insert
BOOL _XWindow_Inventory::ProcessFromSocketPlugIn(void)
{
	if( m_pSocketPlugIn_Window )
	{
		if( m_pSocketPlugIn_Window->GetShowStatus() && !g_pLocalUser->m_bSocketPlugInLock )
		{
			if( m_pSocketPlugIn_Window->m_bHaveMouseSlot )
			{		
				if( m_pSocketPlugIn_Window->m_IndependentItem->Get_m_sSocket(m_pSocketPlugIn_Window->m_MouseSlotItem.m_InventoryNumber) > 0 )
				{
					m_pSocketPlugIn_Window->MessagePrint(8);
				}
				else
				{
					int tempslotindex = m_pSocketPlugIn_Window->m_MouseSlotItem.m_InventoryNumber;
					m_pSocketPlugIn_Window->m_SocketItemList[tempslotindex].Reset();
					m_iSocketIndexList[tempslotindex+1] = 0;

					// ���� �߰��� ������ ���
					for( int i = tempslotindex+1; i < m_pSocketPlugIn_Window->m_IndependentItem->Get_m_ucSocketCount(); i++ )
					{
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : ����� //breif : ġ���ǻ� ���� ���� ��� ����
						if( m_pSocketPlugIn_Window->m_SocketItemList[i].Get_m_sID() > 0 && m_pSocketPlugIn_Window->m_IndependentItem->Get_m_sSocket(i) < 1)
						{
							for( int j = 0; j < m_pSocketPlugIn_Window->m_IndependentItem->Get_m_ucSocketCount(); j++ )
							{
								if( m_pSocketPlugIn_Window->m_SocketItemList[j].Get_m_sID() < 1 )
								{
									m_pSocketPlugIn_Window->m_SocketItemList[j].Copy(m_pSocketPlugIn_Window->m_SocketItemList[i]);
									m_pSocketPlugIn_Window->m_SocketItemList[i].Reset();
									m_iSocketIndexList[j+1] = m_iSocketIndexList[i+1];
									m_iSocketIndexList[i+1] = 0;
									break;
								}
							}
						}
#else
						if( m_pSocketPlugIn_Window->m_SocketItemList[i].Get_m_sID() > 0 )
						{
							m_pSocketPlugIn_Window->m_SocketItemList[i-1].Copy(m_pSocketPlugIn_Window->m_SocketItemList[i]);
							m_pSocketPlugIn_Window->m_SocketItemList[i].Reset();
							m_iSocketIndexList[i] = m_iSocketIndexList[i+1];
							m_iSocketIndexList[i+1] = 0;
						}
#endif
					}

					
					// ��Ĺ��� �� ���ȴ��� �˻�
					BOOL tempflag = TRUE;
					for( i = 1; i < m_pSocketPlugIn_Window->m_IndependentItem->Get_m_ucSocketCount()+1; i++ )
					{
						if( m_iSocketIndexList[i] > 0 )
							tempflag = FALSE;
					}
					if( tempflag )
					{
						m_pSocketPlugIn_Window->m_bChangeButtonMode = FALSE;
						m_pSocketPlugIn_Window->m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_INVENTORY_ITEMDROP); // ��Ĺ� ������
						BOOL tempflag2 = TRUE;
						for( int i = 0; i < m_pSocketPlugIn_Window->m_IndependentItem->Get_m_ucSocketCount()+1; i++ )
						{
							if( m_pSocketPlugIn_Window->m_SocketItemList[i].Get_m_sID() > 0 )
								tempflag2 = FALSE;
						}
						if( tempflag2 )
							m_pSocketPlugIn_Window->RebuildListBoxText(_BOXTEXTMODE_STEP1);
						else
							m_pSocketPlugIn_Window->RebuildListBoxText(_BOXTEXTMODE_STEP2);
					}
					else
					{
						m_pSocketPlugIn_Window->RebuildListBoxText(_BOXTEXTMODE_STEP2);
					}
				}

				m_pSocketPlugIn_Window->m_bHaveMouseSlot = FALSE;
				m_pSocketPlugIn_Window->m_MouseSlotItem.ResetMouseSlotItem();
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL _XWindow_Inventory::ProcessGamble(void)
{
	if( m_pGamble_Window )
	{
		if( m_pGamble_Window->GetShowStatus() && !g_pLocalUser->m_bGambleBoxOpenLock )
		{
			if( m_pGamble_Window->m_bHaveMouseSlot )
			{
				{
					int tempslotindex = m_pGamble_Window->m_MouseSlotItem.m_InventoryNumber;
					m_pGamble_Window->m_GambleKeyList[tempslotindex].Reset();
					m_pGamble_Window->m_GambleKeyFromInvenNumber[tempslotindex] = 0;
#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
					// inven �ǵ帮�� ����
#else
					g_pLocalUser->m_UserItemList[m_iSocketIndexList[tempslotindex+1]].Set_m_ucCount( g_pLocalUser->m_UserItemList[m_iSocketIndexList[tempslotindex+1]].Get_m_ucCount()+1);
#endif
					m_iSocketIndexList[tempslotindex+1] = 0;
					
					// ���� �߰��� ������ ���
					for( int i = tempslotindex+1; i < _XDEF_GAMBLEBOX_MAXCOUNT; i++ )
					{
						if( m_pGamble_Window->m_GambleKeyList[i].Get_m_sID() > 0 )
						{
							m_pGamble_Window->m_GambleKeyList[i-1].Copy(m_pGamble_Window->m_GambleKeyList[i]);
							m_pGamble_Window->m_GambleKeyFromInvenNumber[i-1] = m_iSocketIndexList[i+1];
							m_pGamble_Window->m_GambleKeyList[i].Reset();
							m_pGamble_Window->m_GambleKeyFromInvenNumber[i] = 0;
							m_iSocketIndexList[i] = m_iSocketIndexList[i+1];
							m_iSocketIndexList[i+1] = 0;
						}
					}
					
					// ��Ĺ��� �� ���ȴ��� �˻�
					BOOL tempflag = TRUE;
					for( i = 1; i < _XDEF_GAMBLEBOX_MAXCOUNT+1; i++ )
					{
						if( m_iSocketIndexList[i] > 0 )
							tempflag = FALSE;
					}
					if( tempflag )
					{
						m_pGamble_Window->m_pBoxOpenButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORKITEMTRADE_1756);						
					}
				}
				
				m_pGamble_Window->m_bHaveMouseSlot = FALSE;
				m_pGamble_Window->m_MouseSlotItem.ResetMouseSlotItem();
				return TRUE;
			}
		}
	}
	return FALSE;
}
BOOL _XWindow_Inventory::ProcessWasteBasket(void)
{
#ifdef _XDEF_WASTEBASKET_20070226
	if( !m_bHaveMouseSlot)	
	{
		return FALSE;
	}
	
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	//���� ��ȣ�� 100�̸� ������   106, 242, 121, 257
	//if( ((scrnpos->x > m_WindowPosition.x + 106 ) && (scrnpos->x < m_WindowPosition.x + 121) &&
	//	(scrnpos->z > m_WindowPosition.y + 242 )    && (scrnpos->z < m_WindowPosition.y + 257) ) )
	if( m_pImageWasteBasket->CheckMousePosition() ) //Author : ����� //breif : �ٲ���
	{	
//		_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
//		if( pTradeConfirm_Window )
		{
			TCHAR messagestring[256];
			memset(messagestring, 0, sizeof(TCHAR)*256);
			if( g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_cType() == _XGI_FC_QUEST )
			{
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3332) );	// ��ȣǳ��� �������� ���� �� �����ϴ�
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, NULL, messagestring, TRUE);
				m_bHaveMouseSlot = FALSE;
				m_bDragState = FALSE;
				m_MouseSlotItem.ResetMouseSlotItem();
				return TRUE;
			}

			//�̹��� ���� ��Ű��
			m_bWasteBasketLock = TRUE;
			m_pImageWasteBasket->SetClipRect( 24, 81, 48, 105); // �ٲ� ������ ���

			int resultcashcheck = CheckCashItem(g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_cType(),g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_sID());
			if( resultcashcheck == 1 || resultcashcheck == 2 )
			{
				//pTradeConfirm_Window->m_TradeMode = _XDROP_WASTEBASKET_CHECKPASSWORD;//_XDROP_WASTEBASKET_CHECKPASSWORD;
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3333) );	// ĳ���������� ������ �Ǹ� ������ �����Ǹ� ȯ��/������ ���� �ʽ��ϴ�. �����Ͻðڽ��ϱ�?
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_INV_WASTBASKET_CHECKPASSWOAD, _XDEF_INV_WASTBASKET_CANCLE);
			}
			else
			{
				//pTradeConfirm_Window->m_TradeMode = _XDROP_WASTEBASKET_NORMALITEM;
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3334));	// �ش� �������� ������ ��� ������ ���� �ʽ��ϴ�. �����Ͻðڽ��ϱ�?
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_INV_WASTBASKET_NORMALITEM, _XDEF_INV_WASTBASKET_CANCLE);
			}
//			
//			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
//			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_TRADECONFIRM_OK,_XDEF_TRADECONFIRM_CLOSE);
		
		}
		return TRUE;
	}
	
//	if(m_MouseSlotItem.IsStackable())
//	{
//		_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
//		if(pTradeConfirm_Window)
//		{
//			pTradeConfirm_Window->ShowWindow(TRUE);
//			pTradeConfirm_Window->m_SelectedCategory1 = m_MouseSlotItem.Get_m_cType();
//			pTradeConfirm_Window->m_SelectedItemID = m_MouseSlotItem.Get_m_sID();
//			pTradeConfirm_Window->m_TradeMode = _XDROP_ITEM;
//			rtnvalue = TRUE;
//		}
//	}
//	else
//	{
//		_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
//		if(pTradeConfirm_Window)
//		{
//			pTradeConfirm_Window->m_TradeMode = _XDROP_ITEM;
//			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
//			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, ID_STRING_INVENTORY_WARNDROPITEM, TRUE, 
//				_XDEF_TRADECONFIRM_OK, _XDEF_TRADECONFIRM_CLOSE);
//			
//			rtnvalue = TRUE;
//		}
//	}
#endif
	return FALSE;
}

// 2005.02.25->hotblood ---------------------------------------------------------------------=
BOOL _XWindow_Inventory::ProcessFromPocket(void)
{
	if( m_pPocket_Window && m_pPocket_Window->GetShowStatus() )
	{
		if( m_pPocket_Window->m_bHaveMouseSlot )
		{
			if( m_pPocket_Window->m_MouseSlotItem.Get_m_sID() > 0 )
			{
				g_NetworkKernel.SendPacket( MSG_NO_TEMP_INVEN_CTRL, m_pPocket_Window->m_MouseSlotItem.m_InventoryNumber );
				m_pPocket_Window->m_bHaveMouseSlot = FALSE;
				m_pPocket_Window->m_MouseSlotItem.ResetMouseSlotItem();
				return TRUE;
			}
		}
	}
	return FALSE;
}
// ------------------------------------------------------------------------------------------=

BOOL _XWindow_Inventory::ProcessItemDrop(void)
{
	if( _XGameItem::CheckDump( m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID()) )
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1696), TRUE);
		return FALSE;
	}
	
	if( g_pLocalUser->m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
		g_pLocalUser->m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY )
	{
		//�λ糪 �ڼ� ���϶��� �̵� �Ұ� 
		// ��� �޼���?
		// ���� ������ ���¿����� �ٽ� �����ϱ� ������ �ٸ� �ൿ�� �� �� ����.
		return FALSE;
	}
	
	// �ָӴ� �˻�
	if( m_MouseSlotItem.m_InventoryNumber == _XINVENTORY_SLOTTYPE_AMOR )
	{
		for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1; i < _XINVENTORY_SLOTTYPE_ACCESSORY_1+g_pLocalUser->m_cAmorPocketNum; i++ )
		{
			if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_POCKET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // �ָӴϸ� ����� ���� ���� ���� �� �ֽ��ϴ�.
				
				ResetMouseSlot();
				m_bDragState = FALSE;
				m_LastDraggingSlot = -1;
				return FALSE;
			}
		}
	}
	else if( m_MouseSlotItem.m_InventoryNumber == _XINVENTORY_SLOTTYPE_PANTS )
	{
		for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_5; i < _XINVENTORY_SLOTTYPE_ACCESSORY_5+g_pLocalUser->m_cPantsPocketNum; i++ )
		{
			if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_POCKET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // �ָӴϸ� ����� ���� ���� ���� �� �ֽ��ϴ�.
				
				ResetMouseSlot();
				m_bDragState = FALSE;
				m_LastDraggingSlot = -1;
				return FALSE;			
			}
		}
	}
	//Author : ����� //breif : ĳ�� �೶ Ȯ�� // ĳ���������� ���� �� ����.
	else if( m_MouseSlotItem.m_InventoryNumber == _XINVENTORY_SLOTTYPE_BACKPACK1 )
	{
		// ù��° �೶�� �ι�° �೶�� ���� ��쿡�� ���� �� �ִ�.
		if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() <= 0 )
		{
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��
			for( int i = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; i < _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; i++ )
		#else
			for( int i = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend; i++ )
		#endif
			{
				if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_EMPTYIBACKPACK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�೶�� ������ ��� �� �õ� �Ͻʽÿ�."
					return FALSE;				
				}
			}
		}
		else
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2005), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
			return FALSE;
		}
	}
	// �ι�° �賶�� ����
	else if( m_MouseSlotItem.m_InventoryNumber == _XINVENTORY_SLOTTYPE_BACKPACK2 )
	{
	#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��
		for( int i = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; i < _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART; i++ )		
	#else
		for( int i = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; i++ )
	#endif
		{
			if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_EMPTYIBACKPACK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�೶�� ��ǰ�� ��� �� �õ� �Ͻʽÿ�."
				return FALSE;
			}
		}
	}

	BOOL rtnvalue = FALSE;
	if(!m_bLockInventory && m_bHaveMouseSlot)
	{
		
		if(m_MouseSlotItem.IsStackable())
		{
			_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
			if(pTradeConfirm_Window)
			{
				pTradeConfirm_Window->ShowWindow(TRUE);
				pTradeConfirm_Window->m_SelectedCategory1 = m_MouseSlotItem.Get_m_cType();
				pTradeConfirm_Window->m_SelectedItemID = m_MouseSlotItem.Get_m_sID();
				pTradeConfirm_Window->m_InventoryNumber = m_MouseSlotItem.m_InventoryNumber; //�Ǹ� ������ ���� �߰��� //Author : �����
				pTradeConfirm_Window->m_TradeMode = _XDROP_ITEM;
				rtnvalue = TRUE;
			}
		}
		else
		{
			_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
			if(pTradeConfirm_Window)
			{
				pTradeConfirm_Window->m_TradeMode = _XDROP_ITEM;
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, ID_STRING_INVENTORY_WARNDROPITEM, TRUE, 
					_XDEF_TRADECONFIRM_OK, _XDEF_TRADECONFIRM_CLOSE);

				rtnvalue = TRUE;
			}
		}
	}
	return rtnvalue;
}

void _XWindow_Inventory::ProcessInvenMove(void)
{
	if(m_LastDraggingSlot != -1)
	{
		//Author : ����� //breif : ������ �ű� �� �ִ� ���� ���� �˻�
		if( !CheckItemEquipState() )
			return;
		
		if( g_pLocalUser->m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
			g_pLocalUser->m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY )
		{
			//�λ糪 �ڼ� ���϶��� �̵� �Ұ� 
			// ��� �޼���?
			// ���� ������ ���¿����� �ٽ� �����ϱ� ������ �ٸ� �ൿ�� �� �� ����.
			return;
		}
		
		if(m_LastDraggingSlot == m_MouseSlotItem.m_InventoryNumber)
		{
			// �ڱ��ڸ��� �ٽ� �ֱ�
			ResetMouseSlot();
			m_bDragState = FALSE;
			m_LastDraggingSlot = -1;
			return;
		}
								
		if(m_bLockInventory)
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTMOVE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�ٸ� ���� �ϴ� �߿��� �೶�� ��ǰ�� �̵��� �� �����ϴ�."
			ResetMouseSlot();
			m_bDragState = FALSE;
			m_LastDraggingSlot = -1;
			return;
		}

		// �ָӴ� �˻�
		if( m_MouseSlotItem.m_InventoryNumber == _XINVENTORY_SLOTTYPE_AMOR )
		{
			bool flag = false;

			for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1; i < _XINVENTORY_SLOTTYPE_ACCESSORY_1+g_pLocalUser->m_cAmorPocketNum; i++ )
			{
				if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_POCKET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // �ָӴϸ� ����� ���� ���� ���� �� �ֽ��ϴ�.

					ResetMouseSlot();
					m_bDragState = FALSE;
					m_LastDraggingSlot = -1;
					flag = true;
					break;
				}
			}
			if( !flag )
				g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
		}
		else if( m_MouseSlotItem.m_InventoryNumber == _XINVENTORY_SLOTTYPE_PANTS )
		{
			bool flag = false;
			
			for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_5; i < _XINVENTORY_SLOTTYPE_ACCESSORY_5+g_pLocalUser->m_cPantsPocketNum; i++ )
			{
				if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_POCKET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // �ָӴϸ� ����� ���� ���� ���� �� �ֽ��ϴ�.

					ResetMouseSlot();
					m_bDragState = FALSE;
					m_LastDraggingSlot = -1;
					flag = true;
					break;					
				}
			}
			if( !flag )
				g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
		}
		else if ( m_MouseSlotItem.m_InventoryNumber == _XINVENTORY_SLOTTYPE_BACKPACK1 )
		{
			if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() <= 0 )
			{
				bool flag = false;			
				
				for( int i = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend; i++ )
				{
					if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_EMPTYIBACKPACK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�೶�� ������ ��� �� �õ� �Ͻʽÿ�."

						ResetMouseSlot();
						m_bDragState = FALSE;
						m_LastDraggingSlot = -1;
						flag = true;
						break;					
					}
				}

				// ù ��° �೶�� ���� ù��° & �ι�° �೶ ����Ʈ�� ���� ��� ���� ó��
			#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰� //ĳ�� �೶���� ���� �� �ִ�.
				if( m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART &&
					m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART )
			#else
				if( m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART &&
					m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
			#endif
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2118), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					
					ResetMouseSlot();
					m_bDragState = FALSE;
					m_LastDraggingSlot = -1;
					flag = true;
				}
				
				if( !flag )
					g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
			}
			else
			{
				// ù ��° �೶�� ���� ù��° & �ι�° �೶ ����Ʈ�� ���� ��� ���� ó��				
				if( m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART &&
					m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2118), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}				
				else
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2005), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}

				ResetMouseSlot();
				m_bDragState = FALSE;
				m_LastDraggingSlot = -1;
			}
		}
		else if ( m_MouseSlotItem.m_InventoryNumber == _XINVENTORY_SLOTTYPE_BACKPACK2 )
		{
			bool flag = false;			
				
			for( int i = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend; i++ )
			{
				if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_EMPTYIBACKPACK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�೶�� ������ ��� �� �õ� �Ͻʽÿ�."
					
					ResetMouseSlot();
					m_bDragState = FALSE;
					m_LastDraggingSlot = -1;
					flag = true;
					break;					
				}
			}

			// �� ��° �೶�� ���� �ι�° �೶ ����Ʈ�� ���� ��� ���� ó��
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰� //ĳ�� �೶���� ���� �� �ִ�.
			if( m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART &&
				m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART )
		#else
			if( m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART &&
				m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
		#endif
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2119), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				
				ResetMouseSlot();
				m_bDragState = FALSE;
				m_LastDraggingSlot = -1;
				flag = true;
			}
			//////////////////////////////////////////////////////////////////////////				
			// �� ��° �೶�� ���� ù��° �೶ ����Ʈ�� ���� ���� ��� 			
			//////////////////////////////////////////////////////////////////////////

			
			if( !flag )
				g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
		}
	#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
		else if ( m_MouseSlotItem.m_InventoryNumber == _XINVENTORY_SLOTTYPE_CASHBACKPACK1 )
		{
			bool flag = true;

            ResetMouseSlot();
			m_bDragState = FALSE;
			m_LastDraggingSlot = -1;
            g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3825), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//�Ⱓ�� �೶�� ���� ������ �� �����ϴ�.

			/*if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
			{
				//1ĳ���೶ �ȿ� ��ǰ���ִ�.				
				for( int i = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART; i < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART; i++ )
				{
					if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_EMPTYIBACKPACK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�೶�� ������ ��� �� �õ� �Ͻʽÿ�."
						
						ResetMouseSlot();
						m_bDragState = FALSE;
						m_LastDraggingSlot = -1;
						flag = true;
						break;					
					}
				}

				// 1ĳ�� �೶�� ��� �ڽ��� ��ġ�� 2ĳ�� �೶�� ���� ���
				if( m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
					m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2119), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					
					ResetMouseSlot();
					m_bDragState = FALSE;
					m_LastDraggingSlot = -1;
					flag = true;
				}
			
				//�⺻ �೶�� ĳ�� �೶�� ����
				if( !flag )
					g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
			}
			else
			{
				// ù ��° ĳ�� �೶�� ���� ù��° & �ι�° ĳ�� �೶�� ���� ���
				if( m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
					m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2118), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}				
				else //�ι�° �೶�� �ִµ� ù��° ĳ�� �೶�� ���� �ٸ� �೶�� ���� ��� 
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2005), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}

				ResetMouseSlot();
				m_bDragState = FALSE;
				m_LastDraggingSlot = -1;
			}*/
		}
		else if ( m_MouseSlotItem.m_InventoryNumber == _XINVENTORY_SLOTTYPE_CASHBACKPACK2 )
		{
			bool flag = true;	
            ResetMouseSlot();
			m_bDragState = FALSE;
			m_LastDraggingSlot = -1;
            g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3825), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//�Ⱓ�� �೶�� ���� ������ �� �����ϴ�.
			
			//2ĳ���೶ �ȿ� ��ǰ���ִ�.
			/*for( int i = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; i++ )
			{
				if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_EMPTYIBACKPACK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�೶�� ������ ��� �� �õ� �Ͻʽÿ�."
					
					ResetMouseSlot();
					m_bDragState = FALSE;
					m_LastDraggingSlot = -1;
					flag = true;
					break;					
				}
			}

			// �� ��° �೶�� ���� �ι�° �೶ ����Ʈ�� ���� ��� ���� ó��
			if( m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
				m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2119), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				
				ResetMouseSlot();
				m_bDragState = FALSE;
				m_LastDraggingSlot = -1;
				flag = true;
			}
			//////////////////////////////////////////////////////////////////////////				
			// �� ��° �೶�� ���� ù��° �೶ ����Ʈ�� ���� ���� ��� 			
			//////////////////////////////////////////////////////////////////////////
		
			if( !flag )
				g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );*/
		}
	#endif
		else
		{
			if(!m_bLockInventory)
			{
				// �Ϲ� �κ� = ��ü
				if(m_MouseSlotItem.IsStackable())
				{
					if(m_MouseSlotItem.Get_m_cType() == g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType() 
						&& m_MouseSlotItem.Get_m_sID() == g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID()
						&& m_MouseSlotItem.Get_m_cType() != _XGI_FC_CONSUMING )
					{
						if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_ucCount() >= 255)
						{
							g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
						}
						else if((m_MouseSlotItem.Get_m_ucCount()+g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_ucCount()) > 255)
						{
							m_MouseSlotItem.Set_m_ucCount(255 - g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_ucCount());
							g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 1, m_LastDraggingSlot );
						}
						else
						{
							g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 1, m_LastDraggingSlot );
						}
					}
					else
					{
						g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
					}
				}
				else
				{
					g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
				}
				m_LastDraggingSlot = -1;
#ifdef _XDWDEBUG
				if(g_SinglePlayMode)
					SingleModeChangeItem(m_MouseSlotItem.m_InventoryNumber, m_LastDraggingSlot);
#endif
			}
			else
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTMOVE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�ٸ� ���� �ϴ� �߿��� �೶�� ��ǰ�� �̵��� �� �����ϴ�."
				ResetMouseSlot();
				m_bDragState = FALSE;
				m_LastDraggingSlot = -1;
			}
		} // if(!m_bLockInventory)

	} // if(m_LastDraggingSlot != -1)
}

BOOL _XWindow_Inventory::ProcessSellToNPC(void)
{
	BOOL rtnvalue = FALSE;

	if(m_nPriceMode == _MODE_CLANPOINT)		// ���ı⿩���� ���Խÿ��� �ȱ� ����
	{
		_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
		pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3798), TRUE);//�ش� NPC���Դ� �������� �Ǹ��� �� �����ϴ�.

		return FALSE;
	}

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
	if( m_nPriceMode == _MODE_MNDPOINT )
	{
		_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
		pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3798), TRUE);//�ش� NPC���Դ� �������� �Ǹ��� �� �����ϴ�.

		return FALSE;
	}
#endif

	if(m_pNPCTradeWindow)
	{
		if(m_pNPCTradeWindow->GetShowStatus())
		{
			if(!m_pNPCTradeWindow->m_bHaveMouseSlot && !this->m_bHaveMouseSlot)
			{				
				m_LastDraggingSlot = SearchSlotRegion();
            
				if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0 && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
				{
					if( _XGameItem::CheckNPCTrade( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType(), g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_sID()) )
					{
						_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
						pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2120), TRUE);
						return FALSE;
					}
			
					m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
					m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
					
					
					//2004.06.22->oneway48 insert
					_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
					
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);
					int resultcashcheck = CheckCashItem(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType(),g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID());
					if( resultcashcheck == 1 || resultcashcheck == 2 )
					{
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_2981) );	// ĳ�þ������� ������ �Ǹ� ������ �����Ǹ� ȯ��/���� ���� �ʽ��ϴ�.�Ǹ��Ͻðڽ��ϱ�?
					}
					else
					{
						TCHAR		namestring[128];
						memset(namestring, 0, sizeof(TCHAR)*128);
						strcpy(namestring, _XGameItem::GetItemName(g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType(), g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_sID()));
					
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INVENTORY_SELL), namestring);	// _T("%s(��)�� �Ǹ��Ͻðڽ��ϱ�?")									
					}
					
					pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_NPCTRADE_SELLOK, _XDEF_NPCTRADE_SELLNO);					
					
					rtnvalue = TRUE;
				}
				else
				{
					// icon�� ���� slot Ŭ��
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )						
							g_pDefaultTooltip_Window->ShowWindow( FALSE );					
					}
				}			
			}
			else
			{
				m_pNPCTradeWindow->m_MouseSlotItem.ResetMouseSlotItem();
				m_pNPCTradeWindow->m_bHaveMouseSlot = FALSE;
				if( g_pDefaultTooltip_Window )
				{
					if( g_pDefaultTooltip_Window->GetShowStatus() )						
						g_pDefaultTooltip_Window->ShowWindow( FALSE );
				}
			}
		}
	}

	return rtnvalue;
}

int _XWindow_Inventory::GetUserItemListEmptyCount()
{
	for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; i++)
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
			return i;
	}
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ�� //�󽽷� ������ ĳ�� �೶ �˻�
	for(i = GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++)
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
			return i;
	}
#endif

	return -1;
}

int _XWindow_Inventory::GetCashBackpackStartSlotPosition()
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() > 0 )
    {
        if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 ) 
            return _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART;
        else
            return _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART; //ĳ���೶ 1 �� ���� ���� �ִ� ( �Ⱓ���� ���� ���� �� ���)
    }
    else  //2���� ���� 1���� ������, �Ǵ� 1���� ���� �� //�೶�� �� m_nCashBackPackMaxValue ĳ�� �೶�� �ϳ��� ������ m_nCashBackPackMaxValue == _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART
        return _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART;
#endif

    return 0;
}
// 2004.11.6->hotblood insert
BOOL _XWindow_Inventory::ProcessToSocketPlugIn(void)
{
	if( m_pSocketPlugIn_Window )
	{
		if( m_pSocketPlugIn_Window->GetShowStatus() && !g_pLocalUser->m_bSocketPlugInLock )
		{
			if( !m_pSocketPlugIn_Window->m_bHaveMouseSlot && !this->m_bHaveMouseSlot )
			{
				m_LastDraggingSlot = SearchSlotRegion();
				if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0  && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType()) ||
						_XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType()) )
#else
					if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_WEAPON ||
						g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_CLOTHES ||
						g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_WEAPON2 ||
						g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_CLOTHES2 ||
						g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_WEAPON3 ||
						g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_CLOTHES3 )
#endif
					{
						// error ó�� ------------------------------------------------------------------------------------=
						if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cSecond() == _XGI_SC_CLOTHES_BACKPACK )
						{
							m_pSocketPlugIn_Window->MessagePrint(7);
							return FALSE;
						}
						// -----------------------------------------------------------------------------------------------=

#ifdef _XDEF_NEW_SOCKET_071031_KUKURI
						int socketcheck = 0;
						if(_XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType()))
						{
							_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType());
							if( WeaponItem )
							{
								socketcheck = WeaponItem[g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID()].ucSocketCheck;
							}
						}
						else if(_XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType()))
						{
							_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType());
							if( ClothesItem )
							{
								socketcheck = ClothesItem[g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID()].ucSocketCheck;
							}
						}
						if(socketcheck == 0)
						{
							m_pSocketPlugIn_Window->MessagePrint(14);
							return FALSE;
						}

#endif

						if( m_pSocketPlugIn_Window->m_IndependentItem )		// independent item �� ���� ���
						{
							m_pSocketPlugIn_Window->MessagePrint(10);
							return FALSE;
						}
						else		// independent item �� ���� ���
						{
							if( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_ucSocketCount() == 0 )
							{
								m_pSocketPlugIn_Window->MessagePrint(1);								
								return FALSE;
							}
							bool bflag = false;
							m_pSocketPlugIn_Window->m_IndependentItem = &g_pLocalUser->m_UserItemList[m_LastDraggingSlot];
							m_iSocketIndexList[0] = m_LastDraggingSlot;
							m_pSocketPlugIn_Window->m_SocketPlugInButton->EnableWindow(TRUE);
							m_pSocketPlugIn_Window->m_SocketPlugInButton->ShowWindow(TRUE);
							m_pSocketPlugIn_Window->m_bChangeButtonMode = FALSE;
							m_pSocketPlugIn_Window->m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM); // ��ǰ ������
							PlayItemEquipSound( m_pSocketPlugIn_Window->m_IndependentItem->Get_m_cType(), m_pSocketPlugIn_Window->m_IndependentItem->Get_m_sID() );

							for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT/*g_pLocalUser->m_UserItemList[m_LastDraggingSlot].m_ucSocketCount*/; i++ )
							{
								if( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sSocket(i) )
								{
									bflag = true;
									m_pSocketPlugIn_Window->m_SocketItemList[i].Reset();
									m_pSocketPlugIn_Window->m_SocketItemList[i].Set_m_cType(_XGI_FC_SOCKET);
									m_pSocketPlugIn_Window->m_SocketItemList[i].Set_m_sID( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sSocket(i) );
								}
							}
							
							if( bflag )
								m_pSocketPlugIn_Window->RebuildListBoxText(_BOXTEXTMODE_STEP2);
							else
								m_pSocketPlugIn_Window->RebuildListBoxText(_BOXTEXTMODE_STEP1);

							return TRUE;
						}
					}
					else if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_SOCKET )	// dependent item
					{
						if( m_pSocketPlugIn_Window->m_IndependentItem )
						{							
							int insertindex = -1;
							for( int i = 0; i < m_pSocketPlugIn_Window->m_IndependentItem->Get_m_ucSocketCount(); i++ )
							{
								if( m_pSocketPlugIn_Window->m_SocketItemList[i].Get_m_sID() == 0 )
								{
									insertindex = i;									
									break;
								}
							}

							if( insertindex == -1 )
							{
								m_pSocketPlugIn_Window->MessagePrint(5);
								return FALSE;
							}
							else
							{
								// error ó�� ------------------------------------------------------------------------------------=
								char temp1 = m_pSocketPlugIn_Window->m_IndependentItem->Get_m_cType();
								char temp2 = g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cSecond();

#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : ����� //breif : ġ���ǻ� ���� ���� ��� ����
								//���� üũ
								if( !(g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel() >=
									g_SocketItemProperty[g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_sID()].sTempField5) )
								{
									m_pSocketPlugIn_Window->MessagePrint(13);
									return FALSE;
								}

								if( m_pSocketPlugIn_Window->m_IndependentItem->Get_m_cType() == _XGI_FC_CLOTHES ||
									m_pSocketPlugIn_Window->m_IndependentItem->Get_m_cType() == _XGI_FC_CLOTHES2 ||
									m_pSocketPlugIn_Window->m_IndependentItem->Get_m_cType() == _XGI_FC_CLOTHES3 )
								{
									if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cSecond() == 1 )
									{
										switch( g_SocketItemProperty[g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_sID()].ucThirdType )
										{
										case _XGI_SC_CLOTHES_UPPER :		// ���� = 0
										case _XGI_SC_CLOTHES_LOWER :			// ���� = 1
										case _XGI_SC_CLOTHES_GLOVES :			// �ƴ�(�尩) = 2
										case _XGI_SC_CLOTHES_SHOES :			// �Ź� = 3
										case _XGI_SC_CLOTHES_CAP :			// ���� = 4
										case _XGI_SC_CLOTHES_CLOAK :			// �䰩 = 5
										case _XGI_SC_CLOTHES_ARMOR :			// �Ű� = 6
										case _XGI_SC_CLOTHES_MASK :			// ġ�� = 8
											{
												if(m_pSocketPlugIn_Window->m_IndependentItem->Get_m_cSecond() != g_SocketItemProperty[g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_sID()].ucThirdType)
												{
													//�´� �������� �ƴ�
													m_pSocketPlugIn_Window->MessagePrint(11);
													return FALSE;
												}
											}
											break;
										case _XGI_SC_CLOTHES_BACKPACK :		// �賶 = 7
											{
												m_pSocketPlugIn_Window->MessagePrint(7);
												return FALSE;
											}
											break;
										case 10 : //���� ��Ĺ�
											{
											}
											break;
										default :
											{
												//���� Ÿ�� ����
												m_pSocketPlugIn_Window->MessagePrint(12);
												return FALSE;
											}
											break;
										}
									}
	#ifdef _XDEF_NEW_SOCKET_071031_KUKURI
									if(g_SocketItemProperty[g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID()].ucGrade > 0)
									{
										_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_pSocketPlugIn_Window->m_IndependentItem->Get_m_cType());
										if( ClothesItem )
										{
											if(ClothesItem[m_pSocketPlugIn_Window->m_IndependentItem->Get_m_sID()].ucSocketCheck !=
												g_SocketItemProperty[g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID()].ucGrade)
											{
												m_pSocketPlugIn_Window->MessagePrint(15);
												return FALSE;
											}
										}
									}
	#endif
								}
	#ifdef _XDEF_NEW_SOCKET_071031_KUKURI
								if(_XGI_FC_WEAPON == GetEqualItemType(m_pSocketPlugIn_Window->m_IndependentItem->Get_m_cType()))
								{
									if(g_SocketItemProperty[g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID()].ucGrade > 0)
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_pSocketPlugIn_Window->m_IndependentItem->Get_m_cType());
										if( WeaponItem )
										{
											if(WeaponItem[m_pSocketPlugIn_Window->m_IndependentItem->Get_m_sID()].ucSocketCheck !=
												g_SocketItemProperty[g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID()].ucGrade)
											{
												m_pSocketPlugIn_Window->MessagePrint(15);
												return FALSE;
											}
										}
									}
								}
	#endif
#endif
								
#ifdef _XTS_ITEM_OPTIMIZATION
								if( _XGI_FC_WEAPON == GetEqualItemType(temp1) )
								{
									if( _XGI_FC_CLOTHES == GetEqualItemType(temp2) )
									{
										m_pSocketPlugIn_Window->MessagePrint(9);
										return FALSE;
									}
								}
								if( _XGI_FC_CLOTHES == GetEqualItemType(temp1) )
								{
									if( _XGI_FC_WEAPON == GetEqualItemType(temp2) )
									{
										m_pSocketPlugIn_Window->MessagePrint(9);
										return FALSE;
									}
								}
#else
								if( temp1 == _XGI_FC_WEAPON || temp1 == _XGI_FC_WEAPON2 || temp1 == _XGI_FC_WEAPON3 )
								{
									if( temp2 == _XGI_FC_CLOTHES || temp2 == _XGI_FC_CLOTHES2 || temp2 == _XGI_FC_CLOTHES3 )
									{
										m_pSocketPlugIn_Window->MessagePrint(9);
										return FALSE;
									}
								}
								if( temp1 == _XGI_FC_CLOTHES || temp1 == _XGI_FC_CLOTHES2 || temp1 == _XGI_FC_CLOTHES3 )
								{
									if( temp2 == _XGI_FC_WEAPON || temp2 == _XGI_FC_WEAPON2 || temp2 == _XGI_FC_WEAPON3 )
									{
										m_pSocketPlugIn_Window->MessagePrint(9);
										return FALSE;
									}
								}
#endif
								// -----------------------------------------------------------------------------------------------=
								m_pSocketPlugIn_Window->m_SocketItemList[insertindex].Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
								m_iSocketIndexList[i+1] = m_LastDraggingSlot;
								m_pSocketPlugIn_Window->m_bChangeButtonMode = TRUE;
								m_pSocketPlugIn_Window->RebuildListBoxText(_BOXTEXTMODE_STEP2);
								m_pSocketPlugIn_Window->m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_APPLY);	// _T("��Ĺ� �ޱ�")
								PlayItemEquipSound( m_pSocketPlugIn_Window->m_SocketItemList[insertindex].Get_m_cType(), m_pSocketPlugIn_Window->m_SocketItemList[insertindex].Get_m_sID() );
								
								return TRUE;
							}
						}
						else
						{
							m_pSocketPlugIn_Window->MessagePrint(0);							
							return FALSE;
						}
					}
					else
					{
						m_pSocketPlugIn_Window->MessagePrint(2);						
						return FALSE;
					}
				}
			}
			else
			{
				m_pSocketPlugIn_Window->m_MouseSlotItem.ResetMouseSlotItem();
				m_pSocketPlugIn_Window->m_bHaveMouseSlot = FALSE;
			}
		}
	}
	return FALSE;
}

BOOL _XWindow_Inventory::ProcessToSocketPlugInEx(void)
{
	if( m_pSocketPlugInEx_Window )
	{
		if( m_pSocketPlugInEx_Window->GetShowStatus() )
		{
			if( !m_pSocketPlugInEx_Window->m_bHaveMouseSlot && !this->m_bHaveMouseSlot )
			{
				m_LastDraggingSlot = SearchSlotRegion();
				if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0  && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
				{
					if( m_pSocketPlugInEx_Window->SetSocketPlugInExpandStart(m_LastDraggingSlot) )
						return TRUE;
					else
						 return FALSE;
				}
			}
			else
			{
				m_pSocketPlugInEx_Window->m_MouseSlotItem.ResetMouseSlotItem();
				m_pSocketPlugInEx_Window->m_bHaveMouseSlot = FALSE;
			}
		}
	}

	return FALSE;
}

BOOL _XWindow_Inventory::ProcessToSocketDetach(void)
{
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : ����� //breif : ��������
	_XWindow_SocketDetach* pSocketDetachWindow = (_XWindow_SocketDetach*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETDETACH_WINDOW);

	if( !pSocketDetachWindow )
		return FALSE; 

	if( !pSocketDetachWindow->GetShowStatus() )
		return FALSE;

	if( this->m_bHaveMouseSlot != 0 )
		return FALSE;

	if( pSocketDetachWindow->GetLock() )
		return FALSE;

	m_LastDraggingSlot = SearchSlotRegion();

	if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0  && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType()) ||
			_XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType()) )
#else
		if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_WEAPON ||
			g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_CLOTHES ||
			g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_WEAPON2 ||
			g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_CLOTHES2 ||
			g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_WEAPON3 ||
			g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_CLOTHES3 )
#endif
		{
			// error ó�� ------------------------------------------------------------------------------------=
			if( pSocketDetachWindow->GetIndependentItem() ) //�̹� �����Ƿ�,, ������ ��ü�Ѵ�..
			{
				pSocketDetachWindow->ReBuild( MODE_NONE );
			}
			if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cSecond() == _XGI_SC_CLOTHES_BACKPACK || //�೶
				g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_ucSocketCount() == 0 ) //������ ���� ��ǰ,,
			{
				TCHAR messagestring[256];
				memset( messagestring, 0, sizeof(TCHAR)*256 );

				g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
			
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3691)); //"��Ĺ��� ������ �� ���� ��ǰ �Դϴ�."
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, messagestring, TRUE);	

				return FALSE;
			}

			//����
			pSocketDetachWindow->SetIndependentItem( g_pLocalUser->m_UserItemList[m_LastDraggingSlot] ); //������ ����
			pSocketDetachWindow->SetIndependentItemSlot( m_LastDraggingSlot );
			m_iSocketIndexList[0] = m_LastDraggingSlot;
			m_UsedLifeItemIndex = m_LastDraggingSlot;	//���Թ�ȣ ����

			_XUserItem* pItem = pSocketDetachWindow->GetIndependentItem();
			short sID = pItem->Get_m_sID();

			PlayItemEquipSound( pItem->Get_m_cType(), sID );

			for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT/*g_pLocalUser->m_UserItemList[m_LastDraggingSlot].m_ucSocketCount*/; i++ )
			{
				if( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sSocket(i) )
					pSocketDetachWindow->SetSocketItem( i, g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sSocket(i) );
			}

			pSocketDetachWindow->ReBuild( MODE_READY );
		}
	}
	

#endif
	return TRUE;
}

BOOL _XWindow_Inventory::ProcessToInchant(void)
{
	if( m_pInchant_Window )
	{
		if( m_pInchant_Window->GetShowStatus() )
		{
			if( !this->m_bHaveMouseSlot )
			{
				m_LastDraggingSlot = SearchSlotRegion();
				if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0  && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType()) )
#else
					if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_WEAPON ||
						g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_WEAPON2 ||
						g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_WEAPON3 )
#endif
					{
						// -----------------------------------------------------------------------------------------------=
						if( m_pInchant_Window->m_InchantItem )		// �̹� �������� �÷��� �ִ� ���
						{
							m_pInchant_Window->MessagePrint(5);
							return FALSE;
						}
						else		// ������ item �� ���� ���
						{
#ifdef _XDEF_PERIODITEM_20061027
							if(m_pInchant_Window->ProcessInchantCashItem(m_LastDraggingSlot))
							{
								// �Ⱓ�� �������̴�. - message handler���� ó��
								return TRUE;
							}
							else
							{
								// �Ⱓ�� �������� �ƴ�
								return m_pInchant_Window->ProcessInchant();
							}
#else
							// ������ ������ Item ���� �˻�
							if( m_pInchant_Window->CheckWeaponGrade(m_LastDraggingSlot) == 1 )
							{
								m_pInchant_Window->m_InchantItem = &g_pLocalUser->m_UserItemList[m_LastDraggingSlot];
								m_UsedLifeItemIndex = m_LastDraggingSlot;
								m_pInchant_Window->SetItemInchantStep(US_READY);			
							}
							else
								return FALSE;

							return TRUE;
#endif
						}
					}
					else if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_LIFE )
					{
						if( m_pInchant_Window->m_InchantItem )
						{							
							if( m_pInchant_Window->m_InchantStuff[1].Get_m_sID() != 0 )
							{
								// �̹� �ٸ� �������� ����
								m_pInchant_Window->MessagePrint(6);
								return FALSE;
							}
							else
							{
								// ��� ���� �˻� --------------------------------------------------------------------------------=
								int check = m_pInchant_Window->CheckStuffCount(m_LastDraggingSlot);
								if( check == 1 )
								{
									m_pInchant_Window->m_InchantStuff[1].Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
									m_pInchant_Window->m_InchantStuff[1].Set_m_ucCount( m_pInchant_Window->m_InchantStuff[0].Get_m_ucCount() );
									m_iSocketIndexList[0] = m_LastDraggingSlot;
									m_pInchant_Window->SetItemInchantStep(US_STEP1);
									PlayItemEquipSound( m_pInchant_Window->m_InchantStuff[1].Get_m_cType(), m_pInchant_Window->m_InchantStuff[1].Get_m_sID() );
								}
								else if( check == -1 )
								{
									m_pInchant_Window->MessagePrint(7);									
								}								
								else if( check == -2 )
								{
									m_pInchant_Window->MessagePrint(3);
								}
								// -----------------------------------------------------------------------------------------------=
								return TRUE;
							}
						}
						else
						{
							m_pInchant_Window->MessagePrint(0);							
							return FALSE;
						}
					}
					else
					{
						m_pInchant_Window->MessagePrint(0);						
						return FALSE;
					}
				}
			}
		}
	}
	return FALSE;
}

BOOL _XWindow_Inventory::ProcessToSmeltItem(void)
{
#ifdef _XDEF_SMELT_ITEM //Author : �����
	XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);

	if( !pSmletItemWindow )
		return FALSE; 

	if( !pSmletItemWindow->GetShowStatus() )
		return FALSE;

	if( !this->m_bHaveMouseSlot )
	{
		m_LastDraggingSlot = SearchSlotRegion();
		if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0  && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
		{
			if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType()) )
			{
				_XGI_WeaponItem_Property* tempWeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType());
				if( _XGameItem::GetNature( tempWeaponItem[g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID()].cType, 
										   tempWeaponItem[g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID()].sID) <= 0 ) //Author : ����� breif : ������ 0 �̸� ��� �Ұ�. //Date : 08/03/04
				{
					pSmletItemWindow->MessagePrint(6); // ����� �� ���� ���� �Դϴ�.
					return FALSE;
				}

				if( pSmletItemWindow->GetUserItem() )		// �̹� �������� �÷��� �ִ� ���
				{
					pSmletItemWindow->MessagePrint(5); //��ǰ�� ��ü�� �� �����ϴ�
					return FALSE;
				}
				else		
				{
#ifdef _XDEF_PERIODITEM_20061027
					if(pSmletItemWindow->ProcessSmeltCashItem(m_LastDraggingSlot))
					{
						// �Ⱓ�� �������̴�. - message handler���� ó��
						return TRUE;
					}
					else
					{
						return pSmletItemWindow->ProcessSmelt();
					}
#else
					//��� �븰 �ϋ� �˻� �ؾ� �Ѵ�
					if( pSmletItemWindow->CheckWeaponGrade(m_LastDraggingSlot) == 1 )
					{
						pSmletItemWindow->SetUserItem( g_pLocalUser->m_UserItemList[m_LastDraggingSlot] );
						pSmletItemWindow->SetUserItemindex( m_LastDraggingSlot ); //���Թ�ȣ ����
						m_UsedLifeItemIndex = m_LastDraggingSlot;	//���Թ�ȣ ����
						pSmletItemWindow->SetItemFlag( pSmletItemWindow->GetItemFlag(), ITEMSTEP_READY );
					}
					else
						return FALSE;

					return TRUE;
#endif
				}
			}
			else
			{
				pSmletItemWindow->MessagePrint(0); //�̰��� ���Ⱑ �ƴմϴ�.
				return FALSE;
			}
		}
	}

#endif
	return FALSE;
}

BOOL _XWindow_Inventory::ProcessToGamble(void)
{	
	if( m_pGamble_Window )
	{
		if( m_pGamble_Window->GetShowStatus() && !g_pLocalUser->m_bGambleBoxOpenLock )
		{
			if( !m_pGamble_Window->m_bHaveMouseSlot && !this->m_bHaveMouseSlot )
			{
				m_LastDraggingSlot = SearchSlotRegion();
				if( m_LastDraggingSlot != -1)
				{
					if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0  && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
					{
						if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_BOXKEY )	// dependent item
						{
							if( m_pGamble_Window->m_GambleBoxItem )
							{							
								int insertindex = -1;
								for( int i = 0; i < _XDEF_GAMBLEBOX_MAXCOUNT; i++ )
								{
									if( m_pGamble_Window->m_GambleKeyList[i].Get_m_sID() == 0 )
									{
										insertindex = i;									
										break;
									}
								}
								
								if( insertindex == -1 )
								{
									m_pGamble_Window->MessagePrint(5);
									return FALSE;
								}
								else
								{
									if(m_pGamble_Window->CheckAllSlot(m_LastDraggingSlot))
									{
										if( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_ucCount() > 0 )
										{
											m_pGamble_Window->m_GambleKeyList[insertindex].Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
											m_pGamble_Window->m_GambleKeyList[insertindex].Set_m_ucCount(1);
											m_pGamble_Window->m_GambleKeyFromInvenNumber[insertindex] = m_LastDraggingSlot;
#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
											// inven �ǵ帮�� �ʴ´�
#else
											g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Set_m_ucCount(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_ucCount() -1 );
#endif
										}
										
										m_iSocketIndexList[i+1] = m_LastDraggingSlot;
										m_pGamble_Window->m_pBoxOpenButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_INVENTORY_2121 );	
										PlayItemEquipSound( m_pGamble_Window->m_GambleKeyList[insertindex].Get_m_cType(), m_pGamble_Window->m_GambleKeyList[insertindex].Get_m_sID() );
										return TRUE;
									}
								}
							}
						}
						else
						{
							m_pGamble_Window->MessagePrint(3);						
							return FALSE;
						}
					}					
				}				
			}
			else
			{
				m_pGamble_Window->m_MouseSlotItem.ResetMouseSlotItem();
				m_pGamble_Window->m_bHaveMouseSlot = FALSE;
			}
		}
	}
	return FALSE;
}

BOOL _XWindow_Inventory::ProcessToContribution(void)
{
	if( m_pContribution_Window )
	{
		if( m_pContribution_Window->GetShowStatus() )
		{
			if( !this->m_bHaveMouseSlot )
			{
				m_LastDraggingSlot = SearchSlotRegion();
				if( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0 && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART )
				{
					if( _XGameItem::CheckNPCTrade( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType(), g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() ) )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2120), TRUE);
						return FALSE;
					}

					m_bDrawSelectedItem = TRUE;
					m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
					m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
					
					// �ø���				
					m_pContribution_Window->m_ContributionType = g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType();
					m_pContribution_Window->m_ContributionSecondType = g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cSecond();
					m_pContribution_Window->m_ContributionId = g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID();										
					m_pContribution_Window->m_ContributionIvenIndex = m_LastDraggingSlot;
					m_ContributionItemInvenNumber = m_LastDraggingSlot;
					m_pContribution_Window->m_ContributionItemCount = g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_ucCount();
					
					m_pContribution_Window->SetAddData();
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL _XWindow_Inventory::ProcessToWarehouse(void)
{
	if(m_MouseSlotItem.Get_m_cType() == _XGI_FC_QUEST)
		return FALSE;
	
	_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
	if( pTradeConfirm_Window->GetShowStatus() )		return FALSE;
	
	BOOL rtnvalue = FALSE;
	
	if(m_pWarehouseWindow)
	{
		if(m_pWarehouseWindow->GetShowStatus())
		{
			if( m_pWarehouseWindow->m_bWaitForServerResult ) return FALSE;	// ������ �� �� ���¿����� �ٽ� ������ �ʴ´�.

			if(!m_pWarehouseWindow->m_bHaveMouseSlot && !this->m_bHaveMouseSlot)
			{
				m_LastDraggingSlot = SearchSlotRegion();
				if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0  && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
				{
					if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_QUEST )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTSAVEQUEST), TRUE);
						return FALSE;
					}
#ifdef _XDEF_WAREHOSE_RESTRAINT_OF_TRADE //Author : ����� //breif : â��ŷ� ����
					else if( _XGameItem::CheckItemTempField4( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() ,
															g_pLocalUser->m_UserItemList[m_LastDraggingSlot ].Get_m_sID() ) )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3404), TRUE); //�ش� �������� â�� ������ �� �����ϴ�.
						return FALSE;
					}
#endif
					/*else if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].Get_m_cType() == _XGI_FC_POTION )
					{
						if( g_pLocalUser->m_UserItemList[ m_LastDraggingSlot ].m_cSecond == _XGI_SC_POTION_HWATA )
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2122), TRUE);
							return FALSE;
						}						
					}*/

					m_pWarehouseWindow->ReleaseTempItemList();
					m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
					m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
					m_pWarehouseWindow->SetTempItemFromInven(m_LastDraggingSlot, m_MouseSlotItem.Get_m_ucCount());
					m_pWarehouseWindow->m_SendMode = _XSENDMODE_TOWAREHOUSE_ITEM;
					PostMessage(gHWnd, WM_COMMAND, _XDEF_WAREHOUSEWINDOW_SENDADDITEM, (LPARAM)gHWnd);
					rtnvalue = TRUE;
				}
			}
			else
			{
				m_pWarehouseWindow->m_MouseSlotItem.ResetMouseSlotItem();
				m_pWarehouseWindow->m_bHaveMouseSlot = FALSE;
			}
		}
	}
	return rtnvalue;
}

BOOL _XWindow_Inventory::ProcessToPersonalStore(void)
{
	if(m_MouseSlotItem.Get_m_cType() == _XGI_FC_QUEST)
		return FALSE;
	
	BOOL rtnvalue = FALSE;
	
	if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_OWNER_PREPARE && g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_OWNER_MODIFY )
		return FALSE;
	
	if(m_pPersonalStore_Window)
	{
		if(m_pPersonalStore_Window->GetShowStatus())
		{
			if(CheckMousePosition())
			{
				if( !this->m_bHaveMouseSlot)
				{
					m_LastDraggingSlot = SearchSlotRegion();
					if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0 && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
					{
						if( _XGameItem::CheckUserTrade( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType(), g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() ) )
						{
							_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
							pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2123), TRUE);
							return FALSE;
						}

						if( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_ucCount() == 0 )
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);							
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_INVENTORY_2124), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
										
							g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Reset();
							return FALSE;
						}

						m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
						m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;						
						
						int nextslot = m_pPersonalStore_Window->GetNextEmptySlot();
						if(nextslot != -1)
						{
							if( m_pPersonalStore_Window->m_TradeItemPriceIMEControl.GetFocus() )
							{
								if( m_pPersonalStore_Window->m_Price > 0 )
								{
									m_pPersonalStore_Window->m_TradeItemPrice[m_pPersonalStore_Window->m_SelectedSlotNumber] = m_pPersonalStore_Window->m_Price;
									m_pPersonalStore_Window->m_Price = 0;
								}
								m_pPersonalStore_Window->m_StoreNameIMEControl.ClearBuffer();
								m_pPersonalStore_Window->m_TradeItemPriceIMEControl.ClearBuffer();
								_XIMEKERNEL.SetFocus( NULL ); 	
							}
							else if( m_pPersonalStore_Window->m_StoreNameIMEControl.GetFocus() )
							{
								TCHAR resultstring[256]; 
								memset( resultstring, 0, sizeof(TCHAR)*256 );
								strcpy( resultstring, m_pPersonalStore_Window->m_StoreNameIMEControl.GetText());
#ifdef _XDEF_NEWVERSONFILETERING
								g_WordFilteringManager.Filtering( resultstring,_XDEF_WORDFILETERINGTYPE_CHANGE | _XDEF_WORDFILETERINGTYPE_ALWAYS );
#else
								g_WordFilteringManager.Filtering( resultstring, _XDEF_WORDFILETERINGTYPE_ALWAYS );
#endif
								int length = strlen(resultstring);
								if( length > 0 )
								{
									strcpy( m_pPersonalStore_Window->m_PersonalStoreName, resultstring );
									if( g_pLocalUser )
										strcpy( g_pLocalUser->m_PersonalStoreName, resultstring );
									m_pPersonalStore_Window->m_PersonalStoreName[length] = 0;
									m_pPersonalStore_Window->m_StoreNameIMEControl.ClearBuffer();
									m_pPersonalStore_Window->m_TradeItemPriceIMEControl.ClearBuffer();
									_XIMEKERNEL.SetFocus( NULL ); 	
								}
							}

							m_pPersonalStore_Window->CopyItemToSlot(nextslot, m_MouseSlotItem.m_InventoryNumber);
							m_pPersonalStore_Window->m_TradeItem[nextslot].m_InventoryNumber = m_MouseSlotItem.m_InventoryNumber;
							m_pPersonalStore_Window->m_TradeItem[nextslot].m_ItemNo = g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_ucCount();
							
							m_pPersonalStore_Window->m_SelectedSlotNumber = nextslot;
							m_pPersonalStore_Window->m_TradeItemPrice[nextslot] = 0;
							m_pPersonalStore_Window->m_TradeItemPriceIMEControl.SetFocus();
							m_pPersonalStore_Window->m_btnInputOK->ShowWindow(TRUE);
							
							g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Reset();								
							ResetMouseSlot();
						}
						
						m_LastDraggingSlot = -1;
					}
				}
				else
				{
					ResetMouseSlot();
					m_LastDraggingSlot = -1;
				}
			}
		}
	}
	return rtnvalue;	
}

BOOL _XWindow_Inventory::ProcessToPCTrade(void)
{
	if(m_MouseSlotItem.Get_m_cType() == _XGI_FC_QUEST)
		return FALSE;
	
	BOOL rtnvalue = FALSE;
	
	if(m_pPCTradeWindow)
	{
		if(m_pPCTradeWindow->GetShowStatus())
		{
			if(CheckMousePosition())
			{
				if(!m_pPCTradeWindow->m_bHaveMouseSlot && !this->m_bHaveMouseSlot)
				{
					m_LastDraggingSlot = SearchSlotRegion();
					if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0 && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
					{
						if( _XGameItem::CheckUserTrade( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType(), g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() ) )
						{
							_XMessageWindow* pTradeMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
							pTradeMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2123), TRUE);
							return FALSE;
						}

						m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
						m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;						
						
						int nextslot = m_pPCTradeWindow->GetNextEmptySlot();
						if(nextslot != -1)
						{
							m_pPCTradeWindow->CopyItemToSlot(nextslot, m_MouseSlotItem.m_InventoryNumber);
							m_pPCTradeWindow->m_SelfTradeItem[nextslot].m_InventoryNumber = m_MouseSlotItem.m_InventoryNumber;
							g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_ADD, nextslot, m_MouseSlotItem.Get_m_ucCount() );
							
							g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Reset();								
							ResetMouseSlot();
						}
						
						m_LastDraggingSlot = -1;
					}
				}
				else
				{
					ResetMouseSlot();
				}
			}
		}
	}
	return rtnvalue;
}

BOOL _XWindow_Inventory::ProcessToRepair()
{
	if(m_MouseSlotItem.Get_m_cType() == _XGI_FC_QUEST)
		return FALSE;

	if(m_pRepairItem_Window)
	{
		if(m_pRepairItem_Window->GetShowStatus())
		{
			if(CheckMousePosition())
			{
				if(!m_pRepairItem_Window->m_bHaveMouseSlot && !this->m_bHaveMouseSlot)
				{
					m_LastDraggingSlot = SearchSlotRegion();
					if( m_LastDraggingSlot < _XINVENTORY_SLOTTYPE_POCKETSTART )
						return FALSE;

					if( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0 )
					{
#ifdef _XTS_ITEM_OPTIMIZATION
						if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType()) ||
							_XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType()) )
#else
						if( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType() == _XGI_FC_WEAPON ||
							g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType() ==  _XGI_FC_CLOTHES || 
							g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType() == _XGI_FC_WEAPON2 ||
							g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType() ==  _XGI_FC_CLOTHES2 ||
							g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType() == _XGI_FC_WEAPON3 ||
							g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType() ==  _XGI_FC_CLOTHES3 )
#endif
						{
							int shortdur = g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_usCurDur();
							if( shortdur > 0)
							{
								m_pRepairItem_Window->m_SelectedSlotNoList.push_back( m_LastDraggingSlot );
								m_pRepairItem_Window->SetRepairListItem();
							}
							else
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTNECESSERYREPAIR), TRUE);	// _T("������ �ʿ䰡 ���� ��ǰ�Դϴ�.")
							}
							
							ResetMouseSlot();
							return TRUE;							
						}
						else
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTREPAIRITEM), TRUE);		// _T("���� �� �� ���� ��ǰ�Դϴ�.")
							
							ResetMouseSlot();
						}
					}				
				}
			}
		}
	}
	return FALSE;
}

BOOL _XWindow_Inventory::ProcessRepairItem()
{
	if( m_pRepairItem_Window )
	{
		if( m_pRepairItem_Window->GetShowStatus() )
		{
			if( m_pRepairItem_Window->m_bHaveMouseSlot )
			{
				list<int>::iterator iter = m_pRepairItem_Window->m_SelectedSlotNoList.begin();
				advance( iter, m_pRepairItem_Window->m_MouseSlotItem.m_InventoryNumber );
				m_pRepairItem_Window->m_SelectedSlotNoList.erase( iter );				
				m_pRepairItem_Window->SetRepairListItem();

				m_pRepairItem_Window->m_MouseSlotItem.ResetMouseSlotItem();
				m_pRepairItem_Window->m_bHaveMouseSlot = FALSE;
			}
		}
	}	
	return FALSE;
}

BOOL _XWindow_Inventory::ProcessToEnterBossRoom()
{
	if(m_MouseSlotItem.Get_m_cType() == _XGI_FC_QUEST)
		return FALSE;
	
	if( m_pEnterBossRoom_Window )
	{
		if( m_pEnterBossRoom_Window->GetShowStatus() )
		{
			if(CheckMousePosition())
			{
				if( !this->m_bHaveMouseSlot)
				{
					m_LastDraggingSlot = SearchSlotRegion();
					if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0 && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
					{
						m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
						m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;	
						
						//�ʿ��� ���������� �˻�
						int reason = 0;
						if( m_pEnterBossRoom_Window->CheckNeedItem(m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID(), reason) )
						{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
							m_pEnterBossRoom_Window->m_CurrentSlotNumber = -1;

							for(int i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; i++)
							{
								if(!m_pEnterBossRoom_Window->m_ItemInSlot[i].bInsert)
								{
									if(i == 0)
									{
										if(m_MouseSlotItem.Get_m_cType() == _XGI_FC_RESOURCE)
										{
											if(g_ResourceItemProperty[m_MouseSlotItem.Get_m_sID()].cCashCheck == 1)
											{
												m_pEnterBossRoom_Window->m_CurrentSlotNumber = i;
												break;
											}
											else
											{
												continue;
											}
										}
									}
									else
									{
										m_pEnterBossRoom_Window->m_CurrentSlotNumber = i;
										break;
									}
								}
							}
							
							if( m_pEnterBossRoom_Window->m_CurrentSlotNumber != -1 )
							{
								m_pEnterBossRoom_Window->SetItemInSlot(m_pEnterBossRoom_Window->m_CurrentSlotNumber, m_MouseSlotItem.m_InventoryNumber);
								ResetMouseSlot();
								m_pEnterBossRoom_Window->ProcessItemInSlot();
							}
#else
							m_pEnterBossRoom_Window->m_CurrentSlotNumber = -1;
							for(int i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; i++)
							{
								if( m_pEnterBossRoom_Window->m_NeedItem[i].Get_m_cType() == -1 && m_pEnterBossRoom_Window->m_NeedItem[i].Get_m_sID() == 0 )
								{
									m_pEnterBossRoom_Window->m_CurrentSlotNumber = i;
									break;
								}
							}
							
							if( m_pEnterBossRoom_Window->m_CurrentSlotNumber != -1 )
							{
								if(m_MouseSlotItem.IsStackable())
								{									
									m_pEnterBossRoom_Window->CopyItemToSlot(m_pEnterBossRoom_Window->m_CurrentSlotNumber, m_MouseSlotItem.m_InventoryNumber);
									m_pEnterBossRoom_Window->m_NeedItem[m_pEnterBossRoom_Window->m_CurrentSlotNumber].m_ucCount = 1;
									m_pEnterBossRoom_Window->m_NeedItem[m_pEnterBossRoom_Window->m_CurrentSlotNumber].m_InventoryNumber = m_MouseSlotItem.m_InventoryNumber;
									g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].m_ucCount -= 1;
									
									if(g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].m_ucCount <= 0)
									{
										g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Reset();
									}
									ResetMouseSlot();
									
								}
								else
								{
									m_pEnterBossRoom_Window->CopyItemToSlot(m_pEnterBossRoom_Window->m_CurrentSlotNumber, m_MouseSlotItem.m_InventoryNumber);
									m_pEnterBossRoom_Window->m_NeedItem[m_pEnterBossRoom_Window->m_CurrentSlotNumber].m_InventoryNumber = m_MouseSlotItem.m_InventoryNumber;
									g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Reset();
									ResetMouseSlot();
								}
								
								// �������� �ø��� �� �Ŀ� �䱸�ϴ� �������� ��� ���� �Ǿ������� üũ�Ѵ�.
								if( !m_pEnterBossRoom_Window->m_bSendPartyRequest )
								{
									if( m_pEnterBossRoom_Window->CheckRequirement() ) // �䱸�ϴ� �������� ��� ���� �Ǿ��� �� 
									{			
										if( g_pLocalUser->m_InParty ) // ��Ƽ�� ��Ƽ���鿡�� ���´�.
										{
											if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
											{
												m_pEnterBossRoom_Window->m_bSendPartyRequest = TRUE;							
												g_pLocalUser->m_bWaitBossBattle = TRUE;
												
												g_NetworkKernel.SendPacket( MSG_NO_PARTY_ETC, 8 );
												
												TCHAR messagestring[256];
												memset( messagestring, 0, sizeof(TCHAR)*256 );
												sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_ENTERBOSSROOM_SENDMSG)); // ����鿡�� ����ῡ ���ڴ� �������� ���½��ϴ�.
												
												_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
												pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);									
											}
//											for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++)
//											{
//												if( g_pLocalUser->m_PartyInfo[i].isLeader )
//												{
//													if( strcmp(g_pLocalUser->m_PartyInfo[i].playername, g_pLocalUser->m_CharacterInfo.charactername) == 0) 
//													{						
//														m_pEnterBossRoom_Window->m_bSendPartyRequest = TRUE;							
//														g_pLocalUser->m_bWaitBossBattle = TRUE;
//														
//														g_NetworkKernel.SendPacket( MSG_NO_PARTY_ETC, 8 );
//														
//														TCHAR messagestring[256];
//														memset( messagestring, 0, sizeof(TCHAR)*256 );
//														sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_ENTERBOSSROOM_SENDMSG)); // ����鿡�� ����ῡ ���ڴ� �������� ���½��ϴ�.
//														
//														_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
//														pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);													
//														//pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_REQUESTPARTY_REQUESTBOSSBATTLE,_XDEF_REQUESTPARTY_SOLOBOSSBATTLE);													
//													}
//													break;
//												}
//											}				

										}
									}
								}
								else
								{
									m_pEnterBossRoom_Window->CheckRequirement();
								}									
							}
#endif

						}
						else
						{
							if(reason == 2)
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3641), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //"�̹� �и� ����ϰ� �ֽ��ϴ�."
							else
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_ENTERBOSSROOM_2071), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); 
						}
						
						
						
						m_LastDraggingSlot = -1;					
					}
				}
				else
				{
					ResetMouseSlot();
					m_LastDraggingSlot = -1;
				}
			}
		}
	}	
	return FALSE;
	
}

BOOL _XWindow_Inventory::ProcessToMixWeapon()
{
	BOOL rtnValue = FALSE;

	if( m_pMixWeapon_Window)
	{
		if( m_pMixWeapon_Window->GetShowStatus() )
		{
			if(CheckMousePosition())
			{
				if( !this->m_bHaveMouseSlot)
				{
					m_LastDraggingSlot = SearchSlotRegion();
					if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0 && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
					{
						m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
						m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;	

						if(m_pMixWeapon_Window->CheckItem(m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID(), m_LastDraggingSlot))
						{
							m_pMixWeapon_Window->SetItemSlot(m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID(), m_LastDraggingSlot);
							m_LastDraggingSlot = -1;
							rtnValue = TRUE;
						}
					}
				}
				else
				{
					ResetMouseSlot();
					m_LastDraggingSlot = -1;
				}
			}
		}
	}

	return rtnValue;
}

BOOL _XWindow_Inventory::ProcessToNPCResourceNew()
{
	BOOL rtnValue = FALSE;

	if(m_pNPCResource_New_Window)
	{
		if(m_pNPCResource_New_Window->GetShowStatus())
		{
			if(CheckMousePosition())
			{
				if(!this->m_bHaveMouseSlot)
				{
					m_LastDraggingSlot = SearchSlotRegion();
					if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0 && m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_POCKETSTART)
					{
						m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
						m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;	

						int index = 0;

						if(m_pNPCResource_New_Window->CheckItem(m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID(), m_LastDraggingSlot, index))
						{
							m_pNPCResource_New_Window->SetItem(index, m_LastDraggingSlot);
							ResetMouseSlot();
							m_LastDraggingSlot = -1;
							rtnValue = TRUE;
						}
					}
				}
				else
				{
					ResetMouseSlot();
					m_LastDraggingSlot = -1;
				}
			}
		}
	}

	return rtnValue;
}

#ifdef _XTS_ITEM_OPTIMIZATION
void _XWindow_Inventory::DrawItemTooltip(int slotnumber, RECT iconrect)
{
	char cType = g_pLocalUser->m_UserItemList[slotnumber].Get_m_cType();
	short sID = g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID();

	unsigned char cStrength = g_pLocalUser->m_UserItemList[slotnumber].Get_m_ucStrength();

	if( sID > 0 )
	{
		if( m_pNPCTradeWindow && m_pNPCTradeWindow->GetShowStatus() &&
            !_XGameItem::CheckNPCTrade(cType, sID)) //Author : ����� //breif : �ȼ����� ������ ������ ������ �ȵ�
		{
			int tempMoney = m_pNPCTradeWindow->PriceCalc( slotnumber );
			
#ifdef _XDEF_CASTLEBATTLE_MAPTAXRATE
			FLOAT correction = 0.4f;
			int tvalue = tempMoney % 10;
			if(tvalue == 6)
			{
				correction = 0.5f;
			}

			u_int tax_money = static_cast<u_int>( ((float)tempMoney*(float)(g_nMapTaxRate)/100.0f) + correction );	//<== �������� �ȴ� ��.
			tempMoney -= tax_money;

			// �� �� ���� ����
/*			FLOAT sell_tax = ( ((float)tempMoney*(float) g_nMapTaxRate /100.0f) );
			sell_tax += 0.4f;
			unsigned int tax_money = (unsigned int)sell_tax;
			tempMoney -= tax_money;*/
#endif
	
			TCHAR tempbuf[256];
			
			switch(GetEqualItemType(cType)) 
			{
			case _XGI_FC_WEAPON :			
				{
#ifdef _XTS_ITEM_V20
					if( cStrength > 0 )
						sprintf( tempbuf, "%s +%d : %d", _XGameItem::GetItemFullName(cType, sID), cStrength, tempMoney );
					else
						sprintf( tempbuf, "%s : %d", _XGameItem::GetItemFullName(cType, sID), tempMoney );
#else
					if( cStrength > 0 )
						sprintf( tempbuf, "%s %s +%d : %d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), cStrength, tempMoney );
					else
						sprintf( tempbuf, "%s %s : %d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), tempMoney );
#endif
				}
				break;
			case _XGI_FC_CLOTHES :			
				{
#ifdef _XTS_ITEM_V20
					sprintf( tempbuf, "%s : %d", _XGameItem::GetItemFullName(cType, sID), tempMoney );
#else
					sprintf( tempbuf, "%s %s : %d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), tempMoney );
#endif
				}
				break;
			default:
				{
					sprintf( tempbuf, "%s : %d", _XGameItem::GetItemName(cType, sID), tempMoney );
				}
				break;
			}
			
			g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, _XGameItem::GetGradeColor(cType, sID), TRUE);
		}
		else
		{
			TCHAR tempbuf[128];
			
			switch(GetEqualItemType(cType)) 
			{
			case _XGI_FC_WEAPON :			
				{
#ifdef _XTS_ITEM_V20
					if( cStrength > 0 )
						sprintf( tempbuf, "%s +%d", _XGameItem::GetItemFullName(cType, sID), cStrength );
					else
						sprintf( tempbuf, "%s", _XGameItem::GetItemFullName(cType, sID) );
#else
					if( cStrength > 0 )
						sprintf( tempbuf, "%s %s +%d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), cStrength );
					else
						sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
#endif
				}
				break;
			case _XGI_FC_CLOTHES :
				{
#ifdef _XTS_ITEM_V20
					sprintf( tempbuf, "%s", _XGameItem::GetItemFullName(cType, sID) );
#else
					sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
#endif
				}
				break;
			default:
				{
					sprintf( tempbuf, "%s", _XGameItem::GetItemName(cType, sID) );
				}
				break;
			}
			
			g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, _XGameItem::GetGradeColor(cType, sID), TRUE);
		}
	}
}
#else
void _XWindow_Inventory::DrawItemTooltip(int slotnumber, RECT iconrect)
{
	char cType = g_pLocalUser->m_UserItemList[slotnumber].Get_m_cType();
	short sID = g_pLocalUser->m_UserItemList[slotnumber].Get_m_sID();

	unsigned char cStrength = g_pLocalUser->m_UserItemList[slotnumber].m_ucStrength;

	if( sID > 0 )
	{
		if( m_pNPCTradeWindow && m_pNPCTradeWindow->GetShowStatus() )
		{
			int tempMoney = m_pNPCTradeWindow->PriceCalc( slotnumber );
			TCHAR tempbuf[512];
			
			switch(cType) 
			{
			case _XGI_FC_WEAPON :
			case _XGI_FC_WEAPON2 :
			case _XGI_FC_WEAPON3 :
				{
					if( cStrength > 0 )
						sprintf( tempbuf, "%s %s +%d : %d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), cStrength, tempMoney );
					else
						sprintf( tempbuf, "%s %s : %d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), tempMoney );
				}
				break;
			case _XGI_FC_CLOTHES :
			case _XGI_FC_CLOTHES2 :
			case _XGI_FC_CLOTHES3 :
				{
					sprintf( tempbuf, "%s %s : %d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), tempMoney );
				}
				break;
			default:
				{
					sprintf( tempbuf, "%s : %d", _XGameItem::GetItemName(cType, sID), tempMoney );
				}
				break;
			}
			
			g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, _XGameItem::GetGradeColor(cType, sID), TRUE);
		}
		else
		{
			TCHAR tempbuf[512];
			
			switch(cType) 
			{
			case _XGI_FC_WEAPON :
			case _XGI_FC_WEAPON2 :
			case _XGI_FC_WEAPON3 :
				{
					if( cStrength > 0 )
						sprintf( tempbuf, "%s %s +%d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), cStrength );
					else
						sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
				}
				break;
			case _XGI_FC_CLOTHES :
			case _XGI_FC_CLOTHES2 :
			case _XGI_FC_CLOTHES3 :
				{
					sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
				}
				break;
			default:
				{
					sprintf( tempbuf, "%s", _XGameItem::GetItemName(cType, sID) );
				}
				break;
			}
			
			g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, _XGameItem::GetGradeColor(cType, sID), TRUE);
		}
	}
}
#endif
BOOL _XWindow_Inventory::ProcessItemEquip(void)
{
	// Author : ����� //breif : �˻�� �������� �ű�
	if( !CheckItemEquipState() )
		return FALSE;

	// ���콺 ���Կ� �̹� �������� ������ ������ ���� ������ �� �� ����.
	// ���콺 ������ ���� �ٽ� �õ��ؾ� �Ѵ�. 
	if( m_bHaveMouseSlot )	return FALSE;

	int selectedslot = SearchSlotRegion();
	
	if(selectedslot != -1)
	{
		if(g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID() > 0)
		{
			m_bHaveMouseSlot = TRUE;
			m_MouseSlotItem.m_InventoryNumber = selectedslot;
			m_MouseSlotItem.Set_m_cType( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() );
			m_MouseSlotItem.Set_m_sID( g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID() );
			m_MouseSlotItem.Set_m_ucCount(g_pLocalUser->m_UserItemList[selectedslot].Get_m_ucCount());
			
			switch(m_MouseSlotItem.Get_m_cType())
			{
			case _XGI_FC_CLOTHES :
				{
					switch(g_ClothesItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType)
					{
					case _XGI_SC_CLOTHES_UPPER :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_AMOR;
						}
						break;
					case _XGI_SC_CLOTHES_LOWER :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_PANTS;
						}
						break;
					case _XGI_SC_CLOTHES_CAP :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CAP;
						}
						break;
					case _XGI_SC_CLOTHES_GLOVES :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_ADAE;
						}
						break;
					case _XGI_SC_CLOTHES_SHOES :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_SHOES;
						}
						break;
					case  _XGI_SC_CLOTHES_BACKPACK :
						{
						#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
							if( g_ClothesItemProperty[g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID()].cCashCheck != 0 )
							{
                                //�켱 ĳ�� �೶ �ϳ��� ����Ѵ�.
                                m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CASHBACKPACK1;

								/*if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 )
								{
									if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() > 0 )
										m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CASHBACKPACK1; // 2�� ������ 1�ڸ��� ��ü
									else
									{
										m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CASHBACKPACK2; //2�� ������ 2�ڸ��� ����
									}
								}
								else//1 �� ������ 1 �ڸ��� ����
								{
									m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CASHBACKPACK1;
								}*/
							}
							else
						#endif
							{
								if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 )
								{
									if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0 )
										m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_BACKPACK1;
									else
									{
										m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_BACKPACK2;
									}
								}
								else
								{
									m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_BACKPACK1;
								}
							}
						}
						break;
					case _XGI_SC_CLOTHES_MASK :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_MASK;
						}
						break;
					}
				}
				break;
			case _XGI_FC_WEAPON :
				{
					m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_WEAPON_1;
				}
				break;
			case _XGI_FC_CLOTHES2 :
				{
					switch(g_ClothesItemProperty2[m_MouseSlotItem.Get_m_sID()].cSecondType)
					{
					case _XGI_SC_CLOTHES_UPPER :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_AMOR;
						}
						break;
					case _XGI_SC_CLOTHES_LOWER :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_PANTS;
						}
						break;
					case _XGI_SC_CLOTHES_CAP :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CAP;
						}
						break;
					case _XGI_SC_CLOTHES_GLOVES :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_ADAE;
						}
						break;
					case _XGI_SC_CLOTHES_SHOES :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_SHOES;
						}
						break;
					case  _XGI_SC_CLOTHES_BACKPACK :
						{
						#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
							if( g_ClothesItemProperty2[g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID()].cCashCheck != 0 )
							{
                                m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CASHBACKPACK1;

								/*if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 )
								{
									if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() > 0 )
										m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CASHBACKPACK1;
									else
									{
										m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CASHBACKPACK2;
									}
								}
								else
								{
									m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CASHBACKPACK1;
								}*/
							}
							else
						#endif
							{
								if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 )
								{
									if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0 )
										m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_BACKPACK1;
									else
									{
										m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_BACKPACK2;
									}
								}
								else
								{
									m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_BACKPACK1;
								}
							}
						}
						break;
					case _XGI_SC_CLOTHES_MASK :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_MASK;
						}
						break;
					}
				}
				break;
			case _XGI_FC_WEAPON2 :
				{
					m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_WEAPON_1;
				}
				break;
			case _XGI_FC_WEAPON3 :
				{
					m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_WEAPON_1;
				}
				break;
			case _XGI_FC_CLOTHES3 :
				{
					switch(g_ClothesItemProperty3[m_MouseSlotItem.Get_m_sID()].cSecondType)
					{
					case _XGI_SC_CLOTHES_UPPER :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_AMOR;
						}
						break;
					case _XGI_SC_CLOTHES_LOWER :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_PANTS;
						}
						break;
					case _XGI_SC_CLOTHES_CAP :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CAP;
						}
						break;
					case _XGI_SC_CLOTHES_GLOVES :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_ADAE;
						}
						break;
					case _XGI_SC_CLOTHES_SHOES :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_SHOES;
						}
						break;
					case  _XGI_SC_CLOTHES_BACKPACK :
						{
						#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
							if( g_ClothesItemProperty3[g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID()].cCashCheck != 0 )
							{
                                m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CASHBACKPACK1;

								/*if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 )
								{
									if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() > 0 )
										m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CASHBACKPACK1;
									else
									{
										m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CASHBACKPACK2;
									}
								}
								else
								{
									m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_CASHBACKPACK1;
								}*/
							}
							else
						#endif
							{
								if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 )
								{
									if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0 )
										m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_BACKPACK1;
									else
									{
										m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_BACKPACK2;
									}
								}
								else
								{
									m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_BACKPACK1;
								}
							}
						}
						break;
					case _XGI_SC_CLOTHES_MASK :
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_MASK;
						}
						break;
					}
				}
				break;
			case _XGI_FC_ACCESSORY :
				{
					int acid = g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_sID();
					
					// �ٸ������� �Ź�
					if( g_AccessoryItemProperty[acid].cClan != 0 && g_AccessoryItemProperty[acid].cClan != g_pLocalUser->m_CharacterInfo.groupindex 
						&& g_AccessoryItemProperty[acid].cClan != 20 )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_CINFO_ERRORGROUP), TRUE);	
						return FALSE;						
					}
					
					if( g_AccessoryItemProperty[acid].sReqLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2003), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, FALSE );
						return FALSE;
					}

					#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : ����� //breif : ��Ʈ �������� ��� ���� ��Ʈ ������ �Ǽ��縮�� �Ѱ���..
					int nSetGroup = _XGameItem::GetSetItem(g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_cType(), g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_sID());
					if( nSetGroup )
					{
						for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1; i <= _XINVENTORY_SLOTTYPE_ACCESSORY_8; i++ )
						{
							if( g_pLocalUser->m_UserItemList[i].Get_m_cType() == g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_cType() &&
								g_pLocalUser->m_UserItemList[i].Get_m_sID() == g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_sID() )
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3793), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, FALSE ); //��Ʈ �������� �ߺ� ������ �� �� �����ϴ�.
								return FALSE;
							}
						}
					}
					#endif

					if( m_MouseSlotItem.m_InventoryNumber < _XINVENTORY_SLOTTYPE_POCKETSTART )	// ����â -> �κ�
					{
						for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1; i < _XINVENTORY_SLOTTYPE_ACCESSORY_1+g_pLocalUser->m_cAmorPocketNum; i++ )
						{
							if( m_MouseSlotItem.m_InventoryNumber == i )
							{
								m_LastDraggingSlot = m_MouseSlotItem.m_InventoryNumber;
								break;
							}
						}
						
						if( m_LastDraggingSlot == -1 )
						{
							for( i = _XINVENTORY_SLOTTYPE_ACCESSORY_5; i < _XINVENTORY_SLOTTYPE_ACCESSORY_5+g_pLocalUser->m_cPantsPocketNum; i++ )
							{
								if( m_MouseSlotItem.m_InventoryNumber == i )
								{
									m_LastDraggingSlot = m_MouseSlotItem.m_InventoryNumber;
									break;
								}
							}							
						}												
					}
					else	// �κ� -> ����â
					{
						if( g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_cSecond() == 0 && g_AccessoryItemProperty[acid].cClan == 20 )	// �Ϲ� �Ź��� ��� �ߺ����� ����
						{
							for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1; i <= _XINVENTORY_SLOTTYPE_ACCESSORY_8; i++ )
							{								
								if( g_pLocalUser->m_UserItemList[i].Get_m_sID() == acid )
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_CINFO_2004), TRUE);	
									return FALSE;								
								}							
							}							
						}
#ifdef _XTS_ITEM_V22
						int retvalue = _XGameItem::CheckAccessoryReq(m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID());
						if(retvalue != 0)
						{
							TCHAR messagestr[256];
							switch(retvalue)
							{
							case 1 :
								{
									sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3138));
								}
								break;
							case 2 :
								{
									sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3139));
								}
								break;
							case 3 :
								{
									sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3140));
								}
								break;
							case 4 :
								{
									sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3141));
								}
								break;
							case 5 :
								{
									sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3142));
								}
								break;
							case 6 :
								{
									sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3144));
								}
								break;
							case 7 :
								{
									sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3143));
								}
								break;
							}

							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestr, TRUE);	
							return FALSE;
						}
#endif
						
#ifdef _XTS_ACCESSORY_NEWTYPE
						if(g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].m_cSecond >= _XGI_SC_ACCESSORY_STRENGTH &&
							g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].m_cSecond <= _XGI_SC_ACCESSORY_DEX)
						{
							for(int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1 ; i <= _XINVENTORY_SLOTTYPE_ACCESSORY_8 ; ++i)
							{
								if(i != m_MouseSlotItem.m_InventoryNumber)
								{
									// second type�� ���� �������� �ߺ� ���� �Ұ� 11~15
									if(g_pLocalUser->m_UserItemList[i].m_cSecond == g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].m_cSecond)
									{
										_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
										// �ش� ��ǰ�� �ߺ��ؼ� ������ �� �����ϴ�.
										pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2985), TRUE);	
										return FALSE;
									}
								}
							}
						}
#endif
						
#ifdef _XDEF_PERIODITEM_20061027
						//�Ⱓ�� �������϶� �� �ϰ��� ���� �Ǽ��縮�� �ִ�.
						//Ȯ���ؼ� �־��ش�.
						if( g_AccessoryItemProperty[acid].cSecondType == _XGI_SC_ACCESSORY_ARMOR_MULTI 
							|| g_AccessoryItemProperty[acid].cSecondType == _XGI_SC_ACCESSORY_ARMOR )
						{
							for( int i = _XINVENTORY_SLOTTYPE_GAP_UPPER; i <= _XINVENTORY_SLOTTYPE_GAP_LOWER; i++ )
							{
								if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
								{
									m_LastDraggingSlot = i;
									break;
								}
							}
						}
						else
						{
							for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1; i < _XINVENTORY_SLOTTYPE_ACCESSORY_1+g_pLocalUser->m_cAmorPocketNum; i++ )
							{
								if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
								{
									m_LastDraggingSlot = i;
									break;
								}
							}						
							
							if( m_LastDraggingSlot == -1 )
							{
								for( i = _XINVENTORY_SLOTTYPE_ACCESSORY_5; i < _XINVENTORY_SLOTTYPE_ACCESSORY_5+g_pLocalUser->m_cPantsPocketNum; i++ )
								{
									if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
									{
										m_LastDraggingSlot = i;
										break;
									}						
								}							
							}
							if( m_LastDraggingSlot == -1 )
							{
								m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_ACCESSORY_1;
							}				
						}
						
#else
						
						for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1; i < _XINVENTORY_SLOTTYPE_ACCESSORY_1+g_pLocalUser->m_cAmorPocketNum; i++ )
						{
							if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
							{
								m_LastDraggingSlot = i;
								break;
							}
						}						
						
						if( m_LastDraggingSlot == -1 )
						{
							for( i = _XINVENTORY_SLOTTYPE_ACCESSORY_5; i < _XINVENTORY_SLOTTYPE_ACCESSORY_5+g_pLocalUser->m_cPantsPocketNum; i++ )
							{
								if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
								{
									m_LastDraggingSlot = i;
									break;
								}						
							}							
						}
						if( m_LastDraggingSlot == -1 )
						{
							m_LastDraggingSlot = _XINVENTORY_SLOTTYPE_ACCESSORY_1;
						}				
#endif
					}					
				}
				break;
				// 2004.06.10->oneway48 insert
			case _XGI_FC_POTION : //ȸ�� ������ ���.
				{
					g_pLocalUser->UseItem( m_MouseSlotItem.m_InventoryNumber );
					ResetMouseSlot();
					return TRUE;
				}
				break;
			case _XGI_FC_CONSUMING :
				{
					g_pLocalUser->UseItem( m_MouseSlotItem.m_InventoryNumber );
					ResetMouseSlot();
					return TRUE;
				}
				break;
			case _XGI_FC_ELIXIR :
				{	
					if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
						(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2131), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					}
#ifdef _XTS_PK
					else if( g_pLocalUser->m_BattleMode != _XBATTLE_PEACE ) 
#else
					else if( g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE ) 
#endif
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2132), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					}
					else if( g_pLocalUser->m_CharacterLog.isTrade )
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2133), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );									
					}
					else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);

						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2134), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );								
					}
					else if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);

						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2135), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
					}
#ifdef _XTS_PK
					else if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2644), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
					}
#endif
					else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);

						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1947), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
					}
					else
					{
						if( g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_cSecond() == 0 )	// ���� ������
						{
							if( !g_pLocalUser->m_bInternalUse )
							{
								g_pLocalUser->m_bInternalUse = TRUE;
								g_pLocalUser->UseItem( m_MouseSlotItem.m_InventoryNumber );
							}						
						}
						else
						{
							BOOL bUse = TRUE;
							if( !g_pLocalUser->m_bInternalUse )
							{
								if(g_ElixirItemProperty[g_pLocalUser->m_UserItemList[m_MouseSlotItem.m_InventoryNumber].Get_m_sID()].cCashCheck == 1)
								{
									// ĳ�� ������ ����
									if(g_pLocalUser->m_ElixirInfo._sIntoxication >= 100)
									{
										// �ߵ� ������ 100���� Ŭ ��� ����� �� ����
										_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
										pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3048), TRUE, 0 );
										pMessageBox->StartTimer(5000, TRUE);
										bUse = FALSE;
									}
								}
								
								if(bUse)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);								
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2136), TRUE, _XDEF_INV_USEELIXIRYES, _XDEF_INV_USEELIXIRNO);
									
									m_ElixirInvenNumber = m_MouseSlotItem.m_InventoryNumber;
									g_pLocalUser->m_bInternalUse = TRUE;
								}
							}						
						}
					}
					
					ResetMouseSlot();
					return TRUE;
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					ResetMouseSlot();
					return FALSE;
				}				
				break;
			case _XGI_FC_QUEST :
				{
					g_pLocalUser->UseItem( m_MouseSlotItem.m_InventoryNumber );
					ResetMouseSlot();
					return TRUE;
				}
				break;
			case _XGI_FC_BOOK :
				{
					switch(g_BookItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType)
					{
					case _XGI_SC_BOOK_SKILL :
					case _XGI_SC_BOOK_STUDY :
					case _XGI_SC_BOOK_SKILL2 :
						{
#ifdef _XTS_NEWSKILLWINDOW
							if(g_BookItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType == _XGI_SC_BOOK_SKILL)
							{
								int learnedskillcount = 0;
								for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
								{
									if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
										learnedskillcount++;
								}
								
								if(learnedskillcount == _XDEF_MAX_LEARNSKILL)
								{
									// ���� ���̻� ��� �� ����
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2986), TRUE); //�� �̻� ������ ���� �� �� �����ϴ�. ������ ������ �� �ٽ� �õ��Ͻñ� �ٶ��ϴ�.
									pMessageBox->StartTimer(10000, TRUE) ;
									break;
								}
							}
#endif

							if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
								(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2137), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
								_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							}
#ifdef _XTS_PK
							else if( g_pLocalUser->m_BattleMode != _XBATTLE_PEACE ) 
#else
							else if( g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE ) 
#endif
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_ERRORPEACE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
								_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							}
							else if( g_pLocalUser->m_CharacterLog.isTrade )
							{
								_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2138), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
							}
							else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
							{
								_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
								
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2139), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );				
							}
							else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
							{
								_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
								
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1947), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
							}
							else if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)
							{
								_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
								
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2140), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
							}
#ifdef _XTS_PK
							else if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
							{
								_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
								
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2645), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
							}
#endif
#ifdef _XDEF_ITEM_V25
							else if(g_BookItemProperty[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != 0 &&
								g_BookItemProperty[m_MouseSlotItem.Get_m_sID()].ucCharacGrade != g_pLocalUser->m_CharacterInfo.Get_cClassGrade())
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3796), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );	//_T("��å ����� �ٸ��ϴ�.")
							}
#endif
#ifdef _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI
							else if( (g_BookItemProperty[m_MouseSlotItem.Get_m_sID()].ucClass != 0) && 
								(g_BookItemProperty[m_MouseSlotItem.Get_m_sID()].ucClass != g_pLocalUser->m_CharacterInfo.Get_cClass()) )	// ���Һ�
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTJOB), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // ��å�� �ٸ��ϴ�.
							}
							else if( g_BookItemProperty[m_MouseSlotItem.Get_m_sID()].ucClan != 20 && 
								g_BookItemProperty[m_MouseSlotItem.Get_m_sID()].ucClan != g_pLocalUser->m_CharacterInfo.groupindex )		// ���ĺ�
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORGROUP), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
							}
#endif
							else
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);								
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2141), TRUE, _XDEF_INV_USEBOOKYES, _XDEF_INV_USEBOOKNO);	
								
								m_BookInvenNumber = m_MouseSlotItem.m_InventoryNumber;
								g_pLocalUser->m_bReadingBook = TRUE;
							}
						}
						break;
					case _XGI_SC_BOOK_SCROLL :
					case _XGI_SC_BOOK_STUFF :
					case _XGI_SC_BOOK_PAPER :
						{
							if( CheckIsRead(m_MouseSlotItem.Get_m_sID()) )
							{
								_XWindow_BookOpen* pBookOpenWindow = (_XWindow_BookOpen*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BOOKOPENWINDOW);
								if( pBookOpenWindow )
								{
									if( !pBookOpenWindow->GetShowStatus() )
									{
										if( pBookOpenWindow->SetBookOpenMode( g_BookItemProperty[m_MouseSlotItem.Get_m_sID()].usSkillID, g_BookItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType) )
										{
											pBookOpenWindow->SetTitleName(m_MouseSlotItem.Get_m_sID());
											pBookOpenWindow->ShowWindow(TRUE);
										}
									}
								}
							}
						}
						break;
					}					
					
					ResetMouseSlot();
					return TRUE;
				}				
				break;			
			case _XGI_FC_LIFE :
				{
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : �����
					if( g_LifeItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType == _XGI_SC_LIFE_SOCKET &&
						g_LifeItemProperty[m_MouseSlotItem.Get_m_sID()].sID == 1 ) //����
#else
					if( g_LifeItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType == _XGI_SC_LIFE_SOCKET )
#endif
					{
						if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
							(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2142), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						}
#ifdef _XTS_PK
						else if( g_pLocalUser->m_BattleMode != _XBATTLE_PEACE ) 
#else
						else if( g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE ) 
#endif
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2143), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						}
						else if( g_pLocalUser->m_CharacterLog.isTrade )
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2144), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
						}
						else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2145), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );					
						}
						else if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2146), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
						}
#ifdef _XTS_PK
						else if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2646), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
						}
#endif
						else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1947), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
						}	
						else
						{
							_XWindow_SocketPlugIn* pSocketPlugInWindow = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN);
							if( pSocketPlugInWindow )
							{
								switch(g_LifeItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType) 
								{
								case _XGI_SC_LIFE_SOCKET :
									{
										if( !pSocketPlugInWindow->GetShowStatus() )
										{
											g_pLocalUser->m_bSocketPlugIn = TRUE;
											m_UsedLifeItemIndex = m_MouseSlotItem.m_InventoryNumber;
											pSocketPlugInWindow->ShowWindow(TRUE);
											pSocketPlugInWindow->RebuildListBoxText(_BOXTEXTMODE_READY);
											pSocketPlugInWindow->SetDefaultPosition();							
											g_MainWindowManager.SetTopWindow( pSocketPlugInWindow );
											m_pSocketPlugIn_Window = pSocketPlugInWindow;
											POINT socketpos = pSocketPlugInWindow->GetWindowPos();
											SIZE socketsize = pSocketPlugInWindow->GetWindowSize();
											MoveWindow(socketpos.x + socketsize.cx, socketpos.y);
											SetExpandDockingBorderFlag(FALSE);											
											m_ExpandDockingBorder->ShowWindow( FALSE );										
										}
									}
									break;
								}						
							}
						}
						ResetMouseSlot();
						return TRUE;						
					}
#ifdef _XDEF_SMELT_ITEM //Author : �����
					else if( g_LifeItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType == _XGI_SC_LIFE_SMLETITEM_TEAR || 
						g_LifeItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType == _XGI_SC_LIFE_SMLETITEM_SCALES )
					{
						if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
							(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1580), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						}
#ifdef _XTS_PK
						else if( g_pLocalUser->m_BattleMode != _XBATTLE_PEACE ) 
#else
						else if( g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE ) 
#endif
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1586), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						}
						else if( g_pLocalUser->m_CharacterLog.isTrade )
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	//_T("��ȯ�߿��� ����� �� �����ϴ�.")
						}
						else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 ); //_T("�����߿��� ����� �� �����ϴ�.")
						}
						else if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000); //_T("���߿��� ����� �� �����ϴ�.")
						}
#ifdef _XTS_PK
						else if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000); //_T("���� �߿��� ����� �� �����ϴ�.")
						}
#endif
						else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1947), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
						}	
						else
						{
							XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
							if( pSmletItemWindow )
							{
								switch( g_LifeItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType )
								{
								case _XGI_SC_LIFE_SMLETITEM_TEAR :
									{
										if( !pSmletItemWindow->GetShowStatus() )
										{
											m_UsedLifeItemIndex = m_MouseSlotItem.m_InventoryNumber; //���Թ�ȣ ����
											m_iSocketIndexList[0] = m_LastDraggingSlot;
											pSmletItemWindow->SetSmeltItemindex( m_MouseSlotItem.m_InventoryNumber );  // ���Թ�ȣ����
											pSmletItemWindow->SetItemFlag( TEAR, ITEMSTEP_NONE ); // ��ġ�߿�... �ȿ��� �ʱ�ȭ �̷����
											
											pSmletItemWindow->ShowWindow(TRUE);
											
											SIZE size = pSmletItemWindow->GetWindowSize();

											if( m_WindowPosition.x - size.cx > 0 )
												pSmletItemWindow->MoveWindow( m_WindowPosition.x - size.cx , m_WindowPosition.y );
											else
												pSmletItemWindow->MoveWindow( m_WindowPosition.x + m_WindowSize.cx , m_WindowPosition.y );

											g_MainWindowManager.SetTopWindow( pSmletItemWindow );

											SetExpandDockingBorderFlag(FALSE);											
											m_ExpandDockingBorder->ShowWindow( FALSE );
										}
									}
									break;
								case _XGI_SC_LIFE_SMLETITEM_SCALES :
									{
										if( !pSmletItemWindow->GetShowStatus() )
										{
											m_UsedLifeItemIndex = m_MouseSlotItem.m_InventoryNumber;
											m_iSocketIndexList[0] = m_LastDraggingSlot;
											pSmletItemWindow->SetSmeltItemindex( m_MouseSlotItem.m_InventoryNumber );  
											pSmletItemWindow->SetItemFlag( SCALES, ITEMSTEP_NONE );
											pSmletItemWindow->ShowWindow(TRUE);

											SIZE size = pSmletItemWindow->GetWindowSize();
											
											if( m_WindowPosition.x - size.cx > 0 )
												pSmletItemWindow->MoveWindow( m_WindowPosition.x - size.cx , m_WindowPosition.y );
											else
												pSmletItemWindow->MoveWindow( m_WindowPosition.x + m_WindowSize.cx , m_WindowPosition.y );

											g_MainWindowManager.SetTopWindow( pSmletItemWindow );

											SetExpandDockingBorderFlag(FALSE);											
											m_ExpandDockingBorder->ShowWindow( FALSE );
										}
									}
									break;
								}						
							}
						}
						ResetMouseSlot();
						return TRUE;					
					}
#endif

#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : �����
					if( g_LifeItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType == _XGI_SC_LIFE_SOCKETSSTUFF &&
						g_LifeItemProperty[m_MouseSlotItem.Get_m_sID()].sID == 33 ) //����//��Ĺ� ���� ������
					{
						if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
							(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1580), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						}
#ifdef _XTS_PK
						else if( g_pLocalUser->m_BattleMode != _XBATTLE_PEACE ) 
#else
						else if( g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE ) 
#endif
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1586), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						}
						else if( g_pLocalUser->m_CharacterLog.isTrade )
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	//_T("��ȯ�߿��� ����� �� �����ϴ�.")
						}
						else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 ); //_T("�����߿��� ����� �� �����ϴ�.")
						}
						else if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000); //_T("���߿��� ����� �� �����ϴ�.")
						}
#ifdef _XTS_PK
						else if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000); //_T("���� �߿��� ����� �� �����ϴ�.")
						}
#endif
						else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1947), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
						}	
						else
						{
							//��Ĺ����� 
							_XWindow_SocketDetach* pSocketDetachWindow = (_XWindow_SocketDetach*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETDETACH_WINDOW);

							BOOL invenfull = FALSE;
							if(pSocketDetachWindow)
							{
								if(pSocketDetachWindow->VacancySlot() == -1)	// inventory full
								{
									invenfull = TRUE;

									_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_NOTENOUGHINV), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
								}
							}

							if(!invenfull)
							{
								switch(g_LifeItemProperty[m_MouseSlotItem.Get_m_sID()].cSecondType) 
								{
								case _XGI_SC_LIFE_SOCKETSSTUFF : //��Ĺ�����
									{
										if( !pSocketDetachWindow->GetShowStatus() )
										{
											m_UsedLifeItemIndex = m_MouseSlotItem.m_InventoryNumber;
											m_iSocketIndexList[0] = m_LastDraggingSlot;
											pSocketDetachWindow->ShowWindow(TRUE);
											pSocketDetachWindow->SetDetachItemSlot( m_MouseSlotItem.m_InventoryNumber ); //���Թ�ȣ ����
											pSocketDetachWindow->ReBuild( MODE_NONE );
											pSocketDetachWindow->SetDefaultPosition();
											
											SIZE size = pSocketDetachWindow->GetWindowSize();

											if( m_WindowPosition.x - size.cx > 0 )
												pSocketDetachWindow->MoveWindow( m_WindowPosition.x - size.cx , m_WindowPosition.y );
											else
												pSocketDetachWindow->MoveWindow( m_WindowPosition.x + m_WindowSize.cx , m_WindowPosition.y );

											g_MainWindowManager.SetTopWindow( pSocketDetachWindow );
											
											SetExpandDockingBorderFlag(FALSE);											
											m_ExpandDockingBorder->ShowWindow( FALSE );										
										}
									}
									break;
								}	
							}
						}

						ResetMouseSlot();
						return TRUE;
					}
#endif
				}
				break;
			case _XGI_FC_BOX :
				{
					if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
						(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2147), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					}
#ifdef _XTS_PK
					else if( g_pLocalUser->m_BattleMode != _XBATTLE_PEACE ) 
#else
					else if( g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE ) 
#endif
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2148), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					}
					else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);

						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2149), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );						
					}
					else if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);

						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2150), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
					}
#ifdef _XTS_PK
					else if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2647), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
					}
#endif
					else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);

						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1947), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
					}
					else 
					{
						_XWindow_Gamble* pGambleWindow = (_XWindow_Gamble*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GAMBLE);
						if( pGambleWindow )
						{
							if( !pGambleWindow->GetShowStatus() )
							{
								g_pLocalUser->m_bGambleBoxOpen = TRUE;
								m_UsedLifeItemIndex = m_MouseSlotItem.m_InventoryNumber;
								pGambleWindow->ShowWindow(TRUE);							
								pGambleWindow->SetDefaultPosition();
								pGambleWindow->m_GambleBoxItem = &g_pLocalUser->m_UserItemList[m_UsedLifeItemIndex];
								m_iSocketIndexList[0] = m_UsedLifeItemIndex;
								g_MainWindowManager.SetTopWindow( pGambleWindow );
								m_pGamble_Window = pGambleWindow;
								POINT socketpos = pGambleWindow->GetWindowPos();
								SIZE socketsize = pGambleWindow->GetWindowSize();
								MoveWindow(socketpos.x + socketsize.cx, socketpos.y);
								SetExpandDockingBorderFlag(FALSE);
								m_ExpandDockingBorder->ShowWindow( FALSE );
							}
						}
					}
					ResetMouseSlot();
					return TRUE;
				}
				break;
			}
		}
	}
	
	if(m_LastDraggingSlot != -1)
	{
		_XInventoryEquipErrorCode temperrorcode = CheckSlotType(m_LastDraggingSlot);

		if( temperrorcode == _XEQUIPERROR_NO )
		{
			g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
			return TRUE;
			
#ifdef _XDWDEBUG
			if( g_SinglePlayMode )
			{
				SingleModeChangeItem(selectedslot, m_LastDraggingSlot);
				return TRUE;
			}
#endif
		}
		else
		{
			switch(temperrorcode)
			{
			case _XEQUIPERROR_TYPE :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORPOS), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xffff2323);	// "��ǰ�� ���� ��ġ�� Ʋ���ϴ�."
				break;
			case _XEQUIPERROR_CLAN :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORGROUP), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xffff2323);
				break;
			case _XEQUIPERROR_SEX :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORSEX), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xffff2323);
				break;
			case _XEQUIPERROR_POCKET :
				//g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORPOCKET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xffff2323);
                g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_POCKET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xffff2323);	// �ָӴϸ� ����� ���� ���� ���� �� �ֽ��ϴ�.
				break;
			case _XEQUIPERROR_ACCESSORY :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERROREMPTYPOCKET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xffff2323); // �ָӴϰ� ���� á���ϴ�. 
				break;			
			case _XEQUIPERROR_QUESTHOLDITEM:
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2002), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xffff2323); 
				break;
			case _XEQUIPERROR_LEVEL :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2003), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xffff2323); 
				break;
			case _XEQUIPERROR_ACCOVERLAPPING :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2004), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); 
				break;
#ifdef _XDEF_PERIODITEM_20061027
			case _XEQUIPERROR_PERIODITEM_ACCOVERLAPPING :
				{
					TCHAR	messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);					
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3159), _XGameItem::GetItemName(m_MouseSlotItem.Get_m_cType(), m_MouseSlotItem.Get_m_sID()));	//%s��(��) �ߺ� ������ �Ұ��� �մϴ�.
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); 
				}
				break;
#endif
			case _XEQUIPERROR_FAMEOVERLAPPING :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2637), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xffff2323);	
				break;
			case _XEQUIPERROR_NOOVERLAPPING :
				// �ش� ��ǰ�� �ߺ��ؼ� ������ �� �����ϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2985), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xffff2323);
				break;
#ifdef _XTS_ITEM_V22
			case _XEQUIPERROR_STR :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3138), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				break;
			case _XEQUIPERROR_CONSTITUTION :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3139), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				break;
			case _XEQUIPERROR_ZEN :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3140), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				break;
			case _XEQUIPERROR_INT :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3141), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				break;
			case _XEQUIPERROR_DEX :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3142), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				break;
			case _XEQUIPERROR_FAME :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3143), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				break;
			case _XEQUIPERROR_INFAMY :
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3144), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				break;
#endif
			case _XEQUIPERROR_GRADE :
			case _XEQUIPERROR_CLASS :
			case _XEQUIPERROR_OTHER :
			default:
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_NOTEQUIP), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xffff2323);
				break;
			}
		}		
	}

	return FALSE;
}

BOOL _XWindow_Inventory::CheckItemEquipState()
{
	if(m_pNPCTradeWindow)
	{
		if(m_pNPCTradeWindow->GetShowStatus())
			return FALSE;		
	}
	if(m_pPCTradeWindow)
	{
		if(m_pPCTradeWindow->GetShowStatus())
			return FALSE;
	}
	if(m_pPersonalStore_Window)
	{
		if(m_pPersonalStore_Window->GetShowStatus())
			return FALSE;
	}
	if(m_pWarehouseWindow)
	{
		if(m_pWarehouseWindow->GetShowStatus())
			return FALSE;
	}
	
	// 2004.06.17->oneway48 insert
	if(m_pRepairItem_Window)
	{
		if(m_pRepairItem_Window->GetShowStatus()) // �����ϴ� ��Ŷ�� ����...
			return FALSE;
	}

	if( m_pContribution_Window )
	{
		if( m_pContribution_Window->GetShowStatus() )
			return FALSE;
	}

	// 2004.11.6->hotblood insert
	if( m_pSocketPlugIn_Window )
	{
		if( m_pSocketPlugIn_Window->GetShowStatus() )
			return FALSE;
	}

	if( m_pEnterBossRoom_Window )
	{			
		if( m_pEnterBossRoom_Window->GetShowStatus() )
			return FALSE;		
	}

	if( m_pGamble_Window )
	{
		if( m_pGamble_Window->GetShowStatus() )
			return FALSE;
	}

	if( m_pInchant_Window )
	{
		if( m_pInchant_Window->GetShowStatus() )
			return FALSE;
	}

#ifdef _XDEF_SMELT_ITEM // Author : �����
	XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
	
	if( pSmletItemWindow && pSmletItemWindow->GetShowStatus() )
		return FALSE;
#endif

#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : �����
	_XWindow_SocketDetach* pSocketDetachWindow = (_XWindow_SocketDetach*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETDETACH_WINDOW);

	if( pSocketDetachWindow && pSocketDetachWindow->GetShowStatus() )
		return FALSE;
#endif

	_XWindow_MatchPR* pMatchPR_Window = (_XWindow_MatchPR*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHPR);
	if( pMatchPR_Window )
	{
		if( pMatchPR_Window->GetShowStatus() )
		{
			if( g_pLocalUser->m_bMatchPRMode )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2125), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			return FALSE;					
		}
	}
	
	if( m_pSocketPlugInEx_Window )
	{
		if( m_pSocketPlugInEx_Window->GetShowStatus() )
			return FALSE;
	}

	if(m_pMixWeapon_Window)
	{
		if(m_pMixWeapon_Window->GetShowStatus())
			return FALSE;
	}

	if(m_pNPCResource_New_Window)
	{
		if(m_pNPCResource_New_Window->GetShowStatus())
			return FALSE;
	}

	if( g_pLocalUser->GetMotionClass() == _XACTION_MOVE )
	{
		g_pLocalUser->m_CharacterInfo.animationdescript.motionclass = _XACTION_IDLE;
		g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);				
	}

	if( g_pLocalUser->m_bReadingBook )	// ��� �д� ���̸� ����
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2126), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
		return FALSE;
	}
	if( g_pLocalUser->m_bSocketPlugIn )	// ���� ���� ���̸� ����
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2127), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
		return FALSE;
	}
	if( g_pLocalUser->m_bGambleBoxOpen )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2128), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
		return FALSE;
	}
	if( g_pLocalUser->m_UseSpeedSkill )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2129), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
		return FALSE;
	}
	if( g_pLocalUser->m_bInternalUse )	// ���� ���� ���̸� ����
	{
		_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSAGEWINDOW);
		if(pMessageBox)
		{
			if(!pMessageBox->GetShowStatus())
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2130), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return FALSE;
			}			
		}
		return FALSE;
	}		
	_XWindow_PCTrade*  m_pPCTradeWindowPtr = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PCTRADE );
	if( m_pPCTradeWindowPtr )
	{
		if( m_pPCTradeWindowPtr->m_bTrading )
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1895), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			return FALSE;
		}
	}
	
	_XWindow_AddSaveItem* pAddSaveitem_Window = (_XWindow_AddSaveItem*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_ADDSAVEITEM );
	if(pAddSaveitem_Window)
	{
		if(pAddSaveitem_Window->GetShowStatus())
			return FALSE;
	}
	
	// ��ų ����߿� ��ü �Ұ�
	if(g_pLocalUser->m_bUsingSkill)
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
		return FALSE;
	}
	
	if( g_pLocalUser->m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
		g_pLocalUser->m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY )
	{
		//�λ糪 �ڼ� ���϶��� �̵� �Ұ� 
		// ��� �޼���?
		// ���� ������ ���¿����� �ٽ� �����ϱ� ������ �ٸ� �ൿ�� �� �� ����.
		return FALSE;
	}
    // ������� //Author : ����� //breif : 08.04.23
    if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING && g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_MEDITATION )
        return FALSE;

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    int selectedslot = SearchSlotRegion();	
    if( selectedslot >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
		selectedslot < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART )
	{
		if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
			return FALSE;

        //ĳ�� �೶�� ĳ�� �κ��� ������ ����
        /*if( _XGI_FC_CLOTHES == GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
		{
            _XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
			if( tempClothesitem )
            {
            _XGI_SC_CLOTHES_BACKPACK
                if( (m_MouseSlotItem.Get_m_sID() == 12084 || m_MouseSlotItem.Get_m_sID() == 12085) &&
                    tempClothesitem[m_MouseSlotItem.Get_m_sID()].cCashCheck != 0 )
				{
                    m_LastDraggingSlot = -1;
                    ResetMouseSlot();
                    //tempClothesItem[m_ItemId].cName
                    g_NetworkKernel.InsertChatString(_T("�Ϳ������� �Ⱓ�� �೶�� ���� �� �����ϴ�."), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
					return FALSE;
				}
            }
		}*/
	}

    if( selectedslot >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		selectedslot < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
	{
		if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
			return FALSE;

        /*if( _XGI_FC_CLOTHES == GetEqualItemType(m_MouseSlotItem.Get_m_cType()) )
		{
            _XGI_ClothesItem_Property* tempClothesitem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_MouseSlotItem.Get_m_cType());
			if( tempClothesitem )
            {
                if( (m_MouseSlotItem.Get_m_sID() == 12084 || m_MouseSlotItem.Get_m_sID() == 12085) &&
                    tempClothesitem[m_MouseSlotItem.Get_m_sID()].cCashCheck != 0 )
				{
                    m_LastDraggingSlot = -1;
                    ResetMouseSlot();
                    g_NetworkKernel.InsertChatString(_T("�Ϳ������� �Ⱓ�� �೶�� ���� �� �����ϴ�."), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
					return FALSE;
				}
            }
		}*/
	}
#endif
	
	return TRUE;
}

// ���� �� �ִ� ������� üũ
BOOL _XWindow_Inventory::CheckIsRead( int sID )
{
	switch( g_BookItemProperty[sID].cSecondType ) 
	{
	case _XGI_SC_BOOK_SKILL :			// �������
		{
			for( int i = 0; i < _XDEF_MAX_LEARNSKILL; i++ )
			{
				if( g_pLocalUser->m_SkillList[i].m_sSkillID == g_BookItemProperty[sID].usSkillID )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_ALREADYLEARNBOOK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
					return FALSE;
				}
			}
			
			if( g_BookItemProperty[sID].usHiddenID )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTREADBOOK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			
			if( g_BookItemProperty[sID].ucClan != 20 && 
				g_BookItemProperty[sID].ucClan != g_pLocalUser->m_CharacterInfo.groupindex )		// ���ĺ�
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTREADBOOK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			
			if( (g_BookItemProperty[sID].usQuestID-1) != g_pLocalUser->m_CharacterInfo.gender &&
				g_BookItemProperty[sID].usQuestID != 0 )
			{
				if( g_pLocalUser->m_CharacterInfo.gender == 0 )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1715), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				}
				else
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1716), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				}
				return FALSE;
			}
			
			if( (g_BookItemProperty[sID].ucClass != 0) && (g_BookItemProperty[sID].ucClass != g_pLocalUser->m_CharacterInfo.Get_cClass()) )	// ���Һ�
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTJOB), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // ��å�� �ٸ��ϴ�.
				return FALSE;				
			}

#ifdef _XDEF_ITEM_V25
			if(g_BookItemProperty[sID].ucCharacGrade != 0 &&
				g_BookItemProperty[sID].ucCharacGrade != g_pLocalUser->m_CharacterInfo.Get_cClassGrade())
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3796), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );	//_T("��å ����� �ٸ��ϴ�.")
				return FALSE;
			}
#endif

			if( g_BookItemProperty[sID].usReqSkill1 )		// ��������1 ��
			{
				int tempskilllevel = g_pLocalUser->GetLearnedSkillIndex(g_BookItemProperty[sID].usReqSkill1);
				if( tempskilllevel < 0 )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
					return FALSE;
				}
				else
				{					
					if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel < g_BookItemProperty[sID].cReqSkill1Step-1 )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
						return FALSE;
					}
				}
				if( g_BookItemProperty[sID].usReqSkill2 )		// ��������2 ��
				{
					int tempskilllevel = g_pLocalUser->GetLearnedSkillIndex(g_BookItemProperty[sID].usReqSkill2);
					if( tempskilllevel < 0 )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
						return FALSE;
					}
					else
					{
						if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel < g_BookItemProperty[sID].cReqSkill2Step-1 )
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
							return FALSE;
						}
					}
					if( g_BookItemProperty[sID].usReqSkill3 )		// ��������3 ��
					{
						int tempskilllevel = g_pLocalUser->GetLearnedSkillIndex(g_BookItemProperty[sID].usReqSkill3);
						if( tempskilllevel < 0 )
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
							return FALSE;
						}
						else
						{
							if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel < g_BookItemProperty[sID].cReqSkill3Step-1 )
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
								return FALSE;
							}
						}
					}
				}
			}			
			
			if( g_BookItemProperty[sID].usReqLevel )		// ����ܰ� ��
			{
				int tempstep = g_pDefaultTooltip_Window->GetInnerLevel( g_BookItemProperty[sID].usReqLevel );				// �� ( 1 - 12 )
				int templevel = ( g_BookItemProperty[sID].usReqLevel + 11 ) / 12 ;		// �ܰ� ���ϱ�.( 0 - 20 )
				if( (g_pLocalUser->m_CharacterInfo.Get_level() < templevel) ||
					(g_pLocalUser->m_CharacterInfo.Get_level() == templevel && g_pLocalUser->m_CharacterInfo.Get_sublevel() < tempstep) )				
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
					return FALSE;					
				}
			}

			if( g_BookItemProperty[sID].usReq_STR > g_pLocalUser->m_CharacterInfo.Get_strength() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;				
			}
			if( g_BookItemProperty[sID].usReq_Force > g_pLocalUser->m_CharacterInfo.Get_zen() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usReq_Intell > g_pLocalUser->m_CharacterInfo.Get_intelligence() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usReq_Dex > g_pLocalUser->m_CharacterInfo.Get_dexterity() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usReq_Const > g_pLocalUser->m_CharacterInfo.Get_constitution() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}

			if( g_BookItemProperty[sID].usLife > g_pLocalUser->m_CharacterInfo.Get_current_lifepower() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usForce > g_pLocalUser->m_CharacterInfo.Get_current_forcepower() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usConcent > g_pLocalUser->m_CharacterInfo.Get_current_concentrationpower() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			// ��� ��� ������Ʈ
		}
		break;
	case _XGI_SC_BOOK_STUDY:			// ������
		{
			if( g_BookItemProperty[sID].usHiddenID )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2151), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			
			if( g_BookItemProperty[sID].ucClan != 20 && 
				g_BookItemProperty[sID].ucClan != g_pLocalUser->m_CharacterInfo.groupindex )		// ���ĺ�
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2151), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			
			if( g_BookItemProperty[sID].cThirdType == 2 && g_BookItemProperty[sID].ucClass != 0 )
			{
				if( g_BookItemProperty[sID].ucClass != g_pLocalUser->m_CharacterInfo.Get_cClass() )	// ���Һ�
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTJOB), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000); // ��å�� �ٸ��ϴ�.
					return FALSE;				
				}
			}			

			if( g_BookItemProperty[sID].usReqLevel )		// ����ܰ� ��
			{
				int tempstep = g_pDefaultTooltip_Window->GetInnerLevel( g_BookItemProperty[sID].usReqLevel );				// �� ( 1 - 12 )
				int templevel = ( g_BookItemProperty[sID].usReqLevel + 11 ) / 12 ;		// �ܰ� ���ϱ�.( 0 - 20 )
				if( (g_pLocalUser->m_CharacterInfo.Get_level() < templevel) ||
					(g_pLocalUser->m_CharacterInfo.Get_level() == templevel && g_pLocalUser->m_CharacterInfo.Get_sublevel() < tempstep) )				
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
					return FALSE;					
				}
			}

			if( g_BookItemProperty[sID].usReq_STR > g_pLocalUser->m_CharacterInfo.Get_strength() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;				
			}
			if( g_BookItemProperty[sID].usReq_Force > g_pLocalUser->m_CharacterInfo.Get_zen() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usReq_Intell > g_pLocalUser->m_CharacterInfo.Get_intelligence() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usReq_Dex > g_pLocalUser->m_CharacterInfo.Get_dexterity() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usReq_Const > g_pLocalUser->m_CharacterInfo.Get_constitution() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}

			if( g_BookItemProperty[sID].usLife > g_pLocalUser->m_CharacterInfo.Get_current_lifepower() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usForce > g_pLocalUser->m_CharacterInfo.Get_current_forcepower() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usConcent > g_pLocalUser->m_CharacterInfo.Get_current_concentrationpower() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
		}
		break;	
	case _XGI_SC_BOOK_SCROLL:	// �η縶��
	case _XGI_SC_BOOK_STUFF	:	// �Ϲ� å
	case _XGI_SC_BOOK_PAPER :	// ����
		{
			if( g_BookItemProperty[sID].ucClan != 20 && 
				g_BookItemProperty[sID].ucClan != g_pLocalUser->m_CharacterInfo.groupindex )		// ���ĺ�
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2151), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			
			if( g_BookItemProperty[sID].cThirdType == 2 && g_BookItemProperty[sID].ucClass != 0 )
			{
				if( g_BookItemProperty[sID].ucClass != g_pLocalUser->m_CharacterInfo.Get_cClass() )	// ���Һ�
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTJOB), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000); // ��å�� �ٸ��ϴ�.
					return FALSE;				
				}
			}			
			
			if( g_BookItemProperty[sID].usReqLevel )		// ����ܰ� ��
			{
				int tempstep = g_pDefaultTooltip_Window->GetInnerLevel( g_BookItemProperty[sID].usReqLevel );				// �� ( 1 - 12 )
				int templevel = ( g_BookItemProperty[sID].usReqLevel + 11 ) / 12 ;		// �ܰ� ���ϱ�.( 0 - 20 )
				if( (g_pLocalUser->m_CharacterInfo.Get_level() < templevel) ||
					(g_pLocalUser->m_CharacterInfo.Get_level() == templevel && g_pLocalUser->m_CharacterInfo.Get_sublevel() < tempstep) )				
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
					return FALSE;					
				}
			}			
		}
		break;			
	case _XGI_SC_BOOK_SKILL2 :
		{
			bool flag = false;
			for( int i = 0; i < _XDEF_MAX_LEARNSKILL; i++ )
			{
				if( g_pLocalUser->m_SkillList[i].m_sSkillID == g_BookItemProperty[sID].usSkillID )
					flag = true;
			}

			if( !flag )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2788), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	// _T("�ش� ������ ���� �ܰ踦 ������ �ʾ� ���� �� �����ϴ�.")
				return FALSE;
			}
			
			if( g_BookItemProperty[sID].usHiddenID )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTREADBOOK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			
			if( g_BookItemProperty[sID].ucClan != 20 && 
				g_BookItemProperty[sID].ucClan != g_pLocalUser->m_CharacterInfo.groupindex )		// ���ĺ�
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTREADBOOK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			
			if( (g_BookItemProperty[sID].usQuestID-1) != g_pLocalUser->m_CharacterInfo.gender &&
				g_BookItemProperty[sID].usQuestID != 0 )
			{
				if( g_pLocalUser->m_CharacterInfo.gender == 0 )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1715), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				}
				else
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1716), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				}
				return FALSE;
			}
			
			if( (g_BookItemProperty[sID].ucClass != 0) && (g_BookItemProperty[sID].ucClass != g_pLocalUser->m_CharacterInfo.Get_cClass()) )	// ���Һ�
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTJOB), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // ��å�� �ٸ��ϴ�.
				return FALSE;				
			}
			
			if( g_BookItemProperty[sID].usReqSkill1 )		// ��������1 ��
			{
				int tempskilllevel = g_pLocalUser->GetLearnedSkillIndex(g_BookItemProperty[sID].usReqSkill1);
				if( tempskilllevel < 0 )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
					return FALSE;
				}
				else
				{
					if( g_BookItemProperty[sID].cThirdType == _XGI_TC_BOOK_GROUP )
					{
						if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel != g_BookItemProperty[sID].cReqSkill1Step-1 )
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
							return FALSE;
						}
						else
						{
							int curskillindex = g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel;
							FLOAT exp = (FLOAT)g_SkillProperty[g_BookItemProperty[sID].usReqSkill1]->detailInfo[curskillindex+1].skillExp;
							
							if( ((FLOAT)g_pLocalUser->m_SkillList[tempskilllevel].m_iSkillExp / exp) < 1.0f )
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
								return FALSE;
							}								
						}
					}
					else
					{
						if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel != g_BookItemProperty[sID].cReqSkill1Step-1 )
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
							return FALSE;
						}
					}					
				}
				if( g_BookItemProperty[sID].usReqSkill2 )		// ��������2 ��
				{
					int tempskilllevel = g_pLocalUser->GetLearnedSkillIndex(g_BookItemProperty[sID].usReqSkill2);
					if( tempskilllevel < 0 )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
						return FALSE;
					}
					else
					{
						if( g_BookItemProperty[sID].cThirdType == _XGI_TC_BOOK_GROUP )
						{
							if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel != g_BookItemProperty[sID].cReqSkill2Step-1 )
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
								return FALSE;
							}
							else
							{
								int curskillindex = g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel;
								FLOAT exp = (FLOAT)g_SkillProperty[g_BookItemProperty[sID].usReqSkill2]->detailInfo[curskillindex+1].skillExp;
								
								if( ((FLOAT)g_pLocalUser->m_SkillList[tempskilllevel].m_iSkillExp / exp) < 1.0f )
								{
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
									return FALSE;
								}								
							}
						}
						else
						{
							if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel != g_BookItemProperty[sID].cReqSkill2Step-1 )
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
								return FALSE;
							}
						}
					}
					if( g_BookItemProperty[sID].usReqSkill3 )		// ��������3 ��
					{
						int tempskilllevel = g_pLocalUser->GetLearnedSkillIndex(g_BookItemProperty[sID].usReqSkill3);
						if( tempskilllevel < 0 )
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
							return FALSE;
						}
						else
						{
							if( g_BookItemProperty[sID].cThirdType == _XGI_TC_BOOK_GROUP )
							{
								if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel != g_BookItemProperty[sID].cReqSkill3Step-1 )
								{
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
									return FALSE;
								}
								else
								{
									int curskillindex = g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel;
									FLOAT exp = (FLOAT)g_SkillProperty[g_BookItemProperty[sID].usReqSkill3]->detailInfo[curskillindex+1].skillExp;
									
									if( ((FLOAT)g_pLocalUser->m_SkillList[tempskilllevel].m_iSkillExp / exp) < 1.0f )
									{
										g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
										return FALSE;
									}								
								}
							}
							else
							{
								if( g_pLocalUser->m_SkillList[tempskilllevel].m_cSkillLevel != g_BookItemProperty[sID].cReqSkill3Step-1 )
								{
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
									return FALSE;
								}
							}
						}
					}
				}
			}			
			
			if( g_BookItemProperty[sID].usReqLevel )		// ����ܰ� ��
			{
				int tempstep = g_pDefaultTooltip_Window->GetInnerLevel( g_BookItemProperty[sID].usReqLevel );				// �� ( 1 - 12 )
				int templevel = ( g_BookItemProperty[sID].usReqLevel + 11 ) / 12 ;		// �ܰ� ���ϱ�.( 0 - 20 )
				if( (g_pLocalUser->m_CharacterInfo.Get_level() < templevel) ||
					(g_pLocalUser->m_CharacterInfo.Get_level() == templevel && g_pLocalUser->m_CharacterInfo.Get_sublevel() < tempstep) )				
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
					return FALSE;					
				}
			}
			
			if( g_BookItemProperty[sID].usReq_STR > g_pLocalUser->m_CharacterInfo.Get_strength() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;				
			}
			if( g_BookItemProperty[sID].usReq_Force > g_pLocalUser->m_CharacterInfo.Get_zen() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usReq_Intell > g_pLocalUser->m_CharacterInfo.Get_intelligence() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usReq_Dex > g_pLocalUser->m_CharacterInfo.Get_dexterity() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usReq_Const > g_pLocalUser->m_CharacterInfo.Get_constitution() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			
			if( g_BookItemProperty[sID].usLife > g_pLocalUser->m_CharacterInfo.Get_current_lifepower() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usForce > g_pLocalUser->m_CharacterInfo.Get_current_forcepower() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			if( g_BookItemProperty[sID].usConcent > g_pLocalUser->m_CharacterInfo.Get_current_concentrationpower() )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2152), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return FALSE;
			}
			// ��� ��� ������Ʈ
		}
		break;
	}
		
	return TRUE;
}

// ������ �� �ִ� �������� üũ
BOOL _XWindow_Inventory::CheckIsInternalUse( int sID )
{
	if( g_ElixirItemProperty[sID].ucReq_Level > 0 )
	{
		int tempstep = g_pDefaultTooltip_Window->GetInnerLevel(g_ElixirItemProperty[sID].ucReq_Level);				// �� ( 1 - 12 )
		int templevel = ( g_ElixirItemProperty[sID].ucReq_Level + 11 ) / 12 ;		// �ܰ� ���ϱ�.( 0 - 20 )

		if( (g_pLocalUser->m_CharacterInfo.Get_level() < templevel) ||
			(g_pLocalUser->m_CharacterInfo.Get_level() == templevel && g_pLocalUser->m_CharacterInfo.Get_sublevel() < tempstep) )
		{
			TCHAR tempstr[512];
			memset( tempstr, 0, sizeof(TCHAR)*512 );
			
			strcpy( tempstr, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_INVENTORY_2153, g_LevelName[templevel], &tempstep ) );			
			g_NetworkKernel.InsertChatString(tempstr, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
			return FALSE;
		}
	}	

	switch(g_ElixirItemProperty[sID].cSecondType )
	{
	case _XGI_SC_ELIXIR_SUPPORT :
		{			
		}
		break;
	case _XGI_SC_ELIXIR_STRENGTH :
		{
			if( g_pLocalUser->m_ElixirInfo.Data[0].__Grade+1 < g_ElixirItemProperty[sID].ucStep )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1708), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				return FALSE;
			}
		}
		break;
	case _XGI_SC_ELIXIR_INNER :
		{
			if( g_pLocalUser->m_ElixirInfo.Data[1].__Grade+1 < g_ElixirItemProperty[sID].ucStep )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1708), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				return FALSE;
			}
		}
		break;
	case _XGI_SC_ELIXIR_HEART :
		{
			if( g_pLocalUser->m_ElixirInfo.Data[2].__Grade+1 < g_ElixirItemProperty[sID].ucStep )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1708), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				return FALSE;
			}
		}
		break;
	case _XGI_SC_ELIXIR_HEALTH :
		{
			if( g_pLocalUser->m_ElixirInfo.Data[3].__Grade+1 < g_ElixirItemProperty[sID].ucStep )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1708), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				return FALSE;
			}
		}		
		break;
	case _XGI_SC_ELIXIR_DEX :
		{
			if( g_pLocalUser->m_ElixirInfo.Data[4].__Grade+1 < g_ElixirItemProperty[sID].ucStep )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1708), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				return FALSE;
			}
		}
		break;
	}
	
	if( g_pLocalUser->m_ElixirInfo._sIntoxication >= 200 )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1709), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
		return FALSE;
	}
		
	return TRUE;
}
/*
void _XWindow_Inventory::DrawUserStateIcon()
{
	if( g_pLocalUser->m_CharacterInfo.externalwoundrate <= 0 ) return;

	FLOAT externalwoundrate = (FLOAT)g_pLocalUser->m_CharacterInfo.externalwoundrate/(FLOAT)g_pLocalUser->m_CharacterInfo.max_lifepower * 100.0f;
	if( externalwoundrate > 80.0f )
	{
		if( (gnFPS/3) < gnFrameCounter )
		{
			m_UserStateImage.Draw( 603, 74 );
			_XDrawRectAngle( 602, 73, 602+27, 73+27, 1.0f, _XSC_WARNING_HIGHLIGHT);				
		}
	}
	else if( externalwoundrate > 50.0f )
	{
		
		m_UserStateImage.Draw( 603, 74 );
		_XDrawRectAngle( 602, 73, 602+27, 73+27, 1.0f, _XSC_WARNING);
	}
	else
	{
		m_UserStateImage.Draw( 603, 74 );
		_XDrawRectAngle( 602, 73, 602+27, 73+27, 1.0f, _XSC_DEFAULT);
	}
	
	DrawUserStateIconTooltip();
}

void _XWindow_Inventory::DrawUserStateIconTooltip()
{	
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if( ((scrnpos->x > 602 ) && (scrnpos->x <602+27) &&
		(scrnpos->z > 73 ) && (scrnpos->z < 100) ) )
	{		
		_XDrawSolidBar( 602+20,  73+20 , 602+20+80 ,  73+20+18 , D3DCOLOR_ARGB(127,0,0,0) );
		
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Print(602+20 +14, 73+20+3, 1.0f, " �ܻ� %d", g_pLocalUser->m_CharacterInfo.externalwoundrate );		
		
		g_XBaseFont->Flush();
	}
}

// 2004.06.14->oneway48 insert
void _XWindow_Inventory::DrawUsingItem()
{
	if( !m_UsingItemList.empty() )
	{
		
		list <_XUsingItem>::iterator iter_item;
		
		int count = 0; 
		
		for(iter_item = m_UsingItemList.begin() ; iter_item != m_UsingItemList.end() ; )
		{
			_XUsingItem usingitem = *iter_item;
			
			if( usingitem.m_cType == _XGI_FC_POTION ) //ȸ���� ������
			{
				
				if( (g_LocalSystemTime - usingitem.m_StartTime) < g_PotionItemProperty[usingitem.Get_m_sID()].sCooldown*1000 )
				{
					if( m_BeforeExternalWoundRate != 0 )
					{
						g_pLocalUser->m_CharacterInfo.externalwoundrate = m_BeforeExternalWoundRate;
						g_pLocalUser->m_CharacterInfo.externalwoundrate = g_pLocalUser->m_CharacterInfo.externalwoundrate - m_RecoveryPerSec * (FLOAT)(g_LocalSystemTime - usingitem.m_StartTime);
						
						if( g_pLocalUser->m_CharacterInfo.externalwoundrate <= 0 )
						{
							g_pLocalUser->m_CharacterInfo.externalwoundrate = 0;

							_XWindow_JinStatus* pJinStatus_Window = (_XWindow_JinStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_JINSTATUSWINDOW);
							if(pJinStatus_Window)
							{
								pJinStatus_Window->SetParameter();	
							}
						}
					}

					if( (g_LocalSystemTime - usingitem.m_StartTime) > (DWORD)((FLOAT)g_PotionItemProperty[usingitem.Get_m_sID()].sCooldown*1000.0f*(2.0f/3.0f)) )
					{
						if( (gnFPS/3) < gnFrameCounter )
						{
							DrawIcon(TRUE,230+(count*30) ,74, &usingitem, FALSE, TRUE);
							_XDrawRectAngle(229+(count*30), 73, 229+(count*30)+27, 73+27, 1.0f, _XSC_WARNING_HIGHLIGHT);					
						}
					}
					else
					{
						DrawIcon(TRUE,230+(count*30) ,74, &usingitem, FALSE, TRUE);
						_XDrawRectAngle(229+(count*30), 73, 229+(count*30)+27, 73+27, 1.0f, _XSC_DEFAULT);	
					}
					iter_item++;
				}
				else
				{	
					g_pLocalUser->m_CharacterInfo.externalwoundrate = m_AfterExternalWoundRate;

					_XWindow_JinStatus* pJinStatus_Window = (_XWindow_JinStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_JINSTATUSWINDOW);
					if(pJinStatus_Window)
					{
						pJinStatus_Window->SetParameter();	
					}

					m_bUsingPotionItem = FALSE; //2004.06.17->oneway48 insert

					m_AfterExternalWoundRate = 0;
					m_BeforeExternalWoundRate = 0;
					m_AmountOfRecovery = 0;
					
					iter_item = m_UsingItemList.erase(iter_item);
					SetListUsingItem();
				}
			}
			else // ������ ���� ������ : ����, �ǻ�...
			{
				FLOAT percent = 0;
				if( usingitem.m_usMaxDur != 0 )
					percent = (FLOAT)usingitem.m_usCurDur / (FLOAT)usingitem.m_usMaxDur * 100.0f; // 2004.06.14->oneway48 insert
				
				if( percent <= 10 )
				{				
					if( (gnFPS/3) < gnFrameCounter )
					{
						DrawIcon(TRUE,230+(count*30) ,74, &usingitem, FALSE, TRUE);
						_XDrawRectAngle(229+(count*30), 73, 229+(count*30)+27, 73+27, 1.0f, _XSC_WARNING_HIGHLIGHT);					
					}
				}
				else
				{
					DrawIcon(TRUE,230+(count*30) ,74, &usingitem, FALSE, TRUE);
					_XDrawRectAngle(229+(count*30), 73, 229+(count*30)+27, 73+27, 1.0f, _XSC_DEFAULT);	
				}
				iter_item++;
			}			
			count++;		
		}
	}
	
	if(m_bDrawUsingItemTooltip && m_UsingItemNumber != -1 )
		DrawUsingItemTooltip(m_UsingItemNumber);
}
// insert end

void _XWindow_Inventory::ProcessUsingItemTooltip(_XGUIObject*& pfocusobject)
{
	if( !m_UsingItemList.empty() )
	{
		
		MouseState* mousestate = gpInput->GetMouseState();
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

		bool checkusingitem = FALSE;
		int usingno = -1;
		for( int i = 0 ; i < m_UsingItemList.size(); i++)
		{					
			if( ((scrnpos->x > 229+(i*30) ) && (scrnpos->x < 229+(i*30)+27) &&
				(scrnpos->z > 73 ) && (scrnpos->z < 100) ) )
			{
				usingno = i;
				checkusingitem = TRUE;
				break;					
			}							
		}

		if( checkusingitem )
		{
			if(mousestate->bButton[0])
			{
				m_UsingItemNumber = usingno;
				m_IconDragStartPos.x = scrnpos->x;
				m_IconDragStartPos.y = scrnpos->z;
				if( !pfocusobject || pfocusobject == this )
					pfocusobject = _XDEF_DUMMYID_INVENTORY;
				m_bClickUsingItem = TRUE;
			}
			else
			{
				if(m_bClickUsingItem)
				{					
					if(scrnpos->x == m_IconDragStartPos.x && scrnpos->z == m_IconDragStartPos.y)
						m_bDrawUsingItemTooltip = TRUE;
					
					m_bClickUsingItem = FALSE;
				}
			}
		}

		
		if(mousestate->bButton[0])
		{
			if( !checkusingitem )
			{
				m_bDrawUsingItemTooltip = FALSE;
				m_UsingItemNumber = -1;
			}
		}
		else //2004.06.18->oneway48 insert
		{
			if(!this->m_ShowWindow) 
			{
				if( pfocusobject == this || pfocusobject == _XDEF_DUMMYID_INVENTORY)
					pfocusobject = NULL;
			}
			
		}


	}

}

void _XWindow_Inventory::SetListUsingItem()
{
	if( !m_UsingItemList.empty() )
	{		
		list <_XUsingItem>::iterator iter_item;
		int count = 0;
		for(iter_item = m_UsingItemList.begin() ; iter_item != m_UsingItemList.end() ; iter_item++)
		{			
			iter_item->m_ItemNo = count;
			count++;
		}
	}
	// 2004.06.17->oneway48 insert
	m_bDrawUsingItemTooltip = FALSE;
	m_UsingItemNumber = -1;
}

void _XWindow_Inventory::DrawUsingItemTooltip(int itemno)
{
	_XUsingItem selectusingitem;
	memset(&selectusingitem, 0, sizeof(_XUsingItem));
	
	list <_XUsingItem>::iterator iter_item;
		
	for(iter_item = m_UsingItemList.begin() ; iter_item != m_UsingItemList.end() ; iter_item++ )
	{
		_XUsingItem usingitem = *iter_item;

		if( usingitem.m_ItemNo == itemno )
		{
			selectusingitem.m_cType = usingitem.m_cType;
			selectusingitem.Get_m_sID() = usingitem.Get_m_sID();
			selectusingitem.m_InventoryNumber = usingitem.m_InventoryNumber;
			selectusingitem.m_StartTime = usingitem.m_StartTime;
			selectusingitem.m_usCurDur =usingitem.m_usCurDur;
			selectusingitem.m_usMaxDur =usingitem.m_usMaxDur;
			break;
		}
		
	}

	_XDrawSolidBar( 229+(itemno*30)+20,  73+20 , 229+(itemno*30)+20+200 ,  73+20+63 , D3DCOLOR_ARGB(127,0,0,0) );
	
	switch(selectusingitem.m_cType) 
	{
	case _XGI_FC_POTION:
		{
			g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
			g_XBaseFont->Puts(229+(itemno*30)+20 + 14, 73+20 + 3, g_PotionItemProperty[selectusingitem.Get_m_sID()].cName);
			g_XBaseFont->SetColor(_XSC_DEFAULT);
			
			// 2004.06.17->oneway48 insert
			int count = 0;
			for(int i = 0; i < 57; i++)
			{
				if( g_PotionItemProperty[selectusingitem.Get_m_sID()].cItemInfo[i] == 0 )
					break;
				count++;
			}
			
			if( count <= 30 )
			{
				g_XBaseFont->Puts(229+(itemno*30)+20 + 14, 73+20 + 18, g_PotionItemProperty[selectusingitem.Get_m_sID()].cItemInfo);
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring,0,sizeof(TCHAR)*512);
				for(int i = 0; i < 30; i++)
					messagestring[i] = g_PotionItemProperty[selectusingitem.Get_m_sID()].cItemInfo[i];
				g_XBaseFont->Puts(229+(itemno*30)+20 + 14, 73+20 + 18, messagestring);
				
				memset(messagestring,0,sizeof(TCHAR)*512);
				for( i = 0; i < count-30; i++)
					messagestring[i] = g_PotionItemProperty[selectusingitem.Get_m_sID()].cItemInfo[30+i];
				g_XBaseFont->Puts(229+(itemno*30)+20 + 14, 73+20 + 33, messagestring);
			}
			// insert end

			// 2004.06.17->oneway48 modify
			g_XBaseFont->Print(229+(itemno*30)+20 + 14, 73+20 + 48, 1.0f, "���� �ð�: %d ��", 
				g_PotionItemProperty[selectusingitem.Get_m_sID()].sCooldown - (g_LocalSystemTime - selectusingitem.m_StartTime)/1000 );
		}
		break;
	case _XGI_FC_WEAPON:
		{
			int percent = 0;
			if( selectusingitem.m_usMaxDur != 0 )
				percent = (FLOAT)selectusingitem.m_usCurDur / (FLOAT)selectusingitem.m_usMaxDur * 100.0f; // 2004.06.14->oneway48 insert
			
			g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
			g_XBaseFont->Puts(229+(itemno*30)+20 + 14, 73+20 + 3, g_WeaponItemProperty[selectusingitem.Get_m_sID()].cName);
			g_XBaseFont->SetColor(_XSC_DEFAULT);

			// 2004.06.17->oneway48 insert
			int count = 0;
			for(int i = 0; i < 57; i++)
			{
				if( g_WeaponItemProperty[selectusingitem.Get_m_sID()].cItemInfo[i] == 0 )
					break;
				count++;
			}
			
			if( count <= 30 )
			{
				g_XBaseFont->Puts(229+(itemno*30)+20+ 14, 73+20 + 18, g_WeaponItemProperty[selectusingitem.Get_m_sID()].cItemInfo);
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring,0,sizeof(TCHAR)*512);
				for(int i = 0; i < 30; i++)
					messagestring[i] = g_WeaponItemProperty[selectusingitem.Get_m_sID()].cItemInfo[i];
				g_XBaseFont->Puts(229+(itemno*30)+20 + 14, 73+20 + 18, messagestring);
				
				memset(messagestring,0,sizeof(TCHAR)*512);
				for( i = 0; i < count-30; i++)
					messagestring[i] = g_WeaponItemProperty[selectusingitem.Get_m_sID()].cItemInfo[30+i];
				g_XBaseFont->Puts(229+(itemno*30)+20 + 14, 73+20 + 33, messagestring);
			}
			// insert end
			
			
			// 2004.06.17->oneway48 modify
			g_XBaseFont->Print(229+(itemno*30)+20 + 14, 73+20 + 48, 1.0f, "���� ������: %d %%", percent );
		}
		break;
	case _XGI_FC_CLOTHES:
		{
			int percent = 0;
			if( selectusingitem.m_usMaxDur != 0 )
				percent = (FLOAT)selectusingitem.m_usCurDur / (FLOAT)selectusingitem.m_usMaxDur * 100.0f; // 2004.06.14->oneway48 insert
			
			g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
			g_XBaseFont->Puts(229+(itemno*30)+20 + 14, 73+20 + 3, g_ClothesItemProperty[selectusingitem.Get_m_sID()].cName);
			g_XBaseFont->SetColor(_XSC_DEFAULT);

			// 2004.06.17->oneway48 insert
			int count = 0;
			for(int i = 0; i < 57; i++)
			{
				if( g_ClothesItemProperty[selectusingitem.Get_m_sID()].cItemInfo[i] == 0 )
					break;
				count++;
			}
			
			if( count <= 30 )
			{
				g_XBaseFont->Puts(229+(itemno*30)+20+ 14, 73+20 + 18, g_ClothesItemProperty[selectusingitem.Get_m_sID()].cItemInfo);
			}
			else
			{
				TCHAR messagestring[512];
				memset(messagestring,0,sizeof(TCHAR)*512);
				for(int i = 0; i < 30; i++)
					messagestring[i] = g_ClothesItemProperty[selectusingitem.Get_m_sID()].cItemInfo[i];
				g_XBaseFont->Puts(229+(itemno*30)+20+ 14, 73+20 + 18, messagestring);
				
				memset(messagestring,0,sizeof(TCHAR)*512);
				for( i = 0; i < count-30; i++)
					messagestring[i] = g_ClothesItemProperty[selectusingitem.Get_m_sID()].cItemInfo[30+i];
				g_XBaseFont->Puts(229+(itemno*30)+20+ 14, 73+20 + 33, messagestring);
			}
			// insert end
			
			// 2004.06.17->oneway48 modify
			g_XBaseFont->Print(229+(itemno*30)+20 + 14, 73+20 + 48, 1.0f, "���� ������: %d %%", percent );
		}
		break;
	}
	
	
	g_XBaseFont->Flush();
}
*/

void _XWindow_Inventory::DrawGrabItem()
{
	int  selectedslot = 0;
	int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
	int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;	
	
	if( m_SpeedCounter == 0 ) m_SpeedCounter = g_LocalSystemTime;
	int elapsedtimetick = g_LocalSystemTime - m_SpeedCounter;
	m_SpeedCounter = g_LocalSystemTime;
	
	m_EffectFrame += ((FLOAT)elapsedtimetick / 100.0f);
	int selectedframe = (int)m_EffectFrame; 
	if( selectedframe > 16 )
	{		
		selectedframe = 0;
		m_EffectFrame = 0 ;
		m_SpeedCounter = 0;
		m_EffectPrevFrame = 0;

		m_bDrawGrabItem = FALSE;
		
		for( int i = _XINVENTORY_SLOTTYPE_POCKETSTART; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; i++ )
		{
			if( g_pLocalUser->m_UserItemList[i].m_bNewItem )
			{
				g_pLocalUser->m_UserItemList[i].m_bNewItem = FALSE;
			}
		}
	}
	
	if( m_EffectPrevFrame != selectedframe )
	{	
		if( selectedframe <= 16 )
		{
			RECT	cliprect;
			int		column, row;
			column = selectedframe % 4;
			row	   = selectedframe / 4;
			cliprect.left  = column*64;
			cliprect.right = cliprect.left+64;
			cliprect.top  = row*64;
			cliprect.bottom = cliprect.top+64;
			m_GrabItemImage.SetClipRect( cliprect );
			m_GrabItemImage.SetScale(2.0f,2.0f);
		}
	}
	
	m_EffectPrevFrame = selectedframe;

	for( int i = _XINVENTORY_SLOTTYPE_POCKETSTART; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; i++ )
	{
		if( g_pLocalUser->m_UserItemList[i].m_bNewItem )
		{				
			if(g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0)
			{				
				RECT iconrect;
				int iconposition = i - (m_SelectedBaseTab * _XDEF_INVENTORY_ONEBACKPACKCOUNT +_XINVENTORY_SLOTTYPE_POCKETSTART);				
				
				if( iconposition >= 0 && iconposition < _XDEF_INVENTORY_ONEBACKPACKCOUNT ) // �κ��丮�� ũ�� 25ĭ�� ������ �׸��� �ʴ´�.
				{					
					int		column, row;
					row = 33 * (iconposition/5);
					column = 33 * (iconposition%5);
					
					iconrect.left = columnpoint + column;
					iconrect.top = rowpoint + row;
					
					m_GrabItemImage.MoveWindow( iconrect.left-46, iconrect.top-48 );
					m_GrabItemImage.DrawWithRegion();
				}
			}				
		}
	}
}

BOOL _XWindow_Inventory::CheckOverlapIndex(int index)	// �ߺ��� �ε����� �ִ� ��� �ϳ��� �׸�
{
	BOOL flag = FALSE;

	if( index > 0 )
	{		
		for( int i = 0; i < index; i++ )
		{
			if( m_iSocketIndexList[index] == m_iSocketIndexList[i] )
				flag = TRUE;
		}
	}
	if( flag )
		return FALSE;
	else
		return TRUE;
}

void _XWindow_Inventory::DrawSocketItem( void )	// 2004.11.8->hotblood
{
#ifdef _XDEF_SMELT_ITEM // Author : �����
	XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
#endif

#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : �����
	_XWindow_SocketDetach* pSocketDetachWindow = (_XWindow_SocketDetach*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETDETACH_WINDOW);
#endif

	if( m_pSocketPlugIn_Window && m_pSocketPlugIn_Window->GetShowStatus() )
	{
		for( int i = 0; i < 5; i++ )
		{
			if( m_iSocketIndexList[i] > 0 )
			{
				RECT iconrect;
				int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
				int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
				int column = 0, row = 0;
				int selectpos = m_iSocketIndexList[i]-_XINVENTORY_SLOTTYPE_POCKETSTART;				
				
				if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )				
				{			
					selectpos -= (m_SelectedBaseTab*25);
					
					row = 33 * (selectpos/5);
					column = 33 * (selectpos%5);
					
					iconrect.left = columnpoint + column;
					iconrect.top = rowpoint + row;
					iconrect.right = columnpoint + column + 30;
					iconrect.bottom = rowpoint + row + 30;
					
					// disable icon image
					_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
					_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
				}	
			}
		}
		// ��ĵ���
		RECT iconrect;
		int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
		int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
		int column = 0, row = 0;
		int selectpos = m_UsedLifeItemIndex-_XINVENTORY_SLOTTYPE_POCKETSTART;				
		
		if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )				
		{			
			selectpos -= (m_SelectedBaseTab*25);
			
			row = 33 * (selectpos/5);
			column = 33 * (selectpos%5);
			
			iconrect.left = columnpoint + column;
			iconrect.top = rowpoint + row;
			iconrect.right = columnpoint + column + 30;
			iconrect.bottom = rowpoint + row + 30;
			
			// disable icon image
			_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
		}
	}
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : �����
	else if( pSocketDetachWindow && pSocketDetachWindow->GetShowStatus() )
	{
		for( int i = 0; i < 5; i++ )
		{
			if( m_iSocketIndexList[i] > 0 )
			{
				RECT iconrect;
				int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
				int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
				int column = 0, row = 0;
				int selectpos = m_iSocketIndexList[i]-_XINVENTORY_SLOTTYPE_POCKETSTART;				
				
				if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )				
				{			
					selectpos -= (m_SelectedBaseTab*25);
					
					row = 33 * (selectpos/5);
					column = 33 * (selectpos%5);
					
					iconrect.left = columnpoint + column;
					iconrect.top = rowpoint + row;
					iconrect.right = columnpoint + column + 30;
					iconrect.bottom = rowpoint + row + 30;
					
					// disable icon image
					_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
					_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
				}	
			}
		}
		// ��ĵ���
		RECT iconrect;
		int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
		int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
		int column = 0, row = 0;
		int selectpos = m_UsedLifeItemIndex-_XINVENTORY_SLOTTYPE_POCKETSTART;				
		
		if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )				
		{			
			selectpos -= (m_SelectedBaseTab*25);
			
			row = 33 * (selectpos/5);
			column = 33 * (selectpos%5);
			
			iconrect.left = columnpoint + column;
			iconrect.top = rowpoint + row;
			iconrect.right = columnpoint + column + 30;
			iconrect.bottom = rowpoint + row + 30;
			
			// disable icon image
			_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
		}
	}
#endif
	else if( m_pGamble_Window && m_pGamble_Window->GetShowStatus() )
	{
		for( int i = 0; i < 5; i++ )
		{
			if( m_iSocketIndexList[i] > 0 )
			{
				if( i == 0 || g_pLocalUser->m_UserItemList[m_iSocketIndexList[i]].Get_m_ucCount() == 0 )
				{
					if( CheckOverlapIndex(i) )
					{
						RECT iconrect;
						int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
						int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
						int column = 0, row = 0;
						int selectpos = m_iSocketIndexList[i]-_XINVENTORY_SLOTTYPE_POCKETSTART;				
						
						if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )				
						{			
							selectpos -= (m_SelectedBaseTab*25);
							
							row = 33 * (selectpos/5);
							column = 33 * (selectpos%5);
							
							iconrect.left = columnpoint + column;
							iconrect.top = rowpoint + row;
							iconrect.right = columnpoint + column + 30;
							iconrect.bottom = rowpoint + row + 30;
							
							// disable icon image
							_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
							_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
						}
					}						
				}				
			}
		}				
		// ��ĵ���
		RECT iconrect;
		int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
		int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
		int column = 0, row = 0;
		int selectpos = m_UsedLifeItemIndex-_XINVENTORY_SLOTTYPE_POCKETSTART;				
		
		if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )				
		{			
			selectpos -= (m_SelectedBaseTab*25);
			
			row = 33 * (selectpos/5);
			column = 33 * (selectpos%5);
			
			iconrect.left = columnpoint + column;
			iconrect.top = rowpoint + row;
			iconrect.right = columnpoint + column + 30;
			iconrect.bottom = rowpoint + row + 30;
			
			// disable icon image
			_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
		}
	}
	else if( m_pSocketPlugInEx_Window && m_pSocketPlugInEx_Window->GetShowStatus() )
	{
		for( int i = 0; i < 5; i++ )
		{
			if( m_iSocketIndexList[i] > 0 )
			{
				RECT iconrect;
				int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
				int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
				int column = 0, row = 0;
				int selectpos = m_iSocketIndexList[i]-_XINVENTORY_SLOTTYPE_POCKETSTART;				
				
				if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )				
				{			
					selectpos -= (m_SelectedBaseTab*25);
					
					row = 33 * (selectpos/5);
					column = 33 * (selectpos%5);
					
					iconrect.left = columnpoint + column;
					iconrect.top = rowpoint + row;
					iconrect.right = columnpoint + column + 30;
					iconrect.bottom = rowpoint + row + 30;
					
					// disable icon image
					_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
					_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
				}	
			}
		}
		// ��ĵ���
		RECT iconrect;
		int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
		int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
		int column = 0, row = 0;
		int selectpos = m_UsedLifeItemIndex-_XINVENTORY_SLOTTYPE_POCKETSTART;				
		
		if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )				
		{			
			selectpos -= (m_SelectedBaseTab*25);
			
			row = 33 * (selectpos/5);
			column = 33 * (selectpos%5);
			
			iconrect.left = columnpoint + column;
			iconrect.top = rowpoint + row;
			iconrect.right = columnpoint + column + 30;
			iconrect.bottom = rowpoint + row + 30;
			
			// disable icon image
			_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
		}		
	}
	else if( m_pInchant_Window && m_pInchant_Window->GetShowStatus() )
	{
		if( m_iSocketIndexList[0] > 0 )
		{
			RECT iconrect;
			int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
			int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
			int column = 0, row = 0;
			int selectpos = m_iSocketIndexList[0]-_XINVENTORY_SLOTTYPE_POCKETSTART;				
			
			if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )				
			{			
				selectpos -= (m_SelectedBaseTab*25);
				
				row = 33 * (selectpos/5);
				column = 33 * (selectpos%5);
				
				iconrect.left = columnpoint + column;
				iconrect.top = rowpoint + row;
				iconrect.right = columnpoint + column + 30;
				iconrect.bottom = rowpoint + row + 30;
				
				// disable icon image
				_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
				_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
			}	
		}
		
		// ��ĵ���
		RECT iconrect;
		int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
		int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
		int column = 0, row = 0;
		int selectpos = m_UsedLifeItemIndex-_XINVENTORY_SLOTTYPE_POCKETSTART;				
		
		if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )				
		{			
			selectpos -= (m_SelectedBaseTab*25);
			
			row = 33 * (selectpos/5);
			column = 33 * (selectpos%5);
			
			iconrect.left = columnpoint + column;
			iconrect.top = rowpoint + row;
			iconrect.right = columnpoint + column + 30;
			iconrect.bottom = rowpoint + row + 30;
			
			// disable icon image
			_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
		}
	}

#ifdef _XDEF_SMELT_ITEM //Author : �����
	else if( pSmletItemWindow && pSmletItemWindow->GetShowStatus() )
	{
		if( m_iSocketIndexList[0] > 0 )
		{
			RECT iconrect;
			int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
			int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
			int column = 0, row = 0;
			int selectpos = m_iSocketIndexList[0]-_XINVENTORY_SLOTTYPE_POCKETSTART;				
			
			if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )				
			{			
				selectpos -= (m_SelectedBaseTab*25);
				
				row = 33 * (selectpos/5);
				column = 33 * (selectpos%5);
				
				iconrect.left = columnpoint + column;
				iconrect.top = rowpoint + row;
				iconrect.right = columnpoint + column + 30;
				iconrect.bottom = rowpoint + row + 30;
				
				// disable icon image
				_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
				_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
			}	
		}
		
		// ��ĵ���
		RECT iconrect;
		int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
		int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
		int column = 0, row = 0;
		int selectpos = m_UsedLifeItemIndex-_XINVENTORY_SLOTTYPE_POCKETSTART;				
		
		if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )				
		{			
			selectpos -= (m_SelectedBaseTab*25);
			
			row = 33 * (selectpos/5);
			column = 33 * (selectpos%5);
			
			iconrect.left = columnpoint + column;
			iconrect.top = rowpoint + row;
			iconrect.right = columnpoint + column + 30;
			iconrect.bottom = rowpoint + row + 30;
			
			// disable icon image
			_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
		}
	}
#endif
}

void _XWindow_Inventory::DrawRepairItem( void )
{
	if( m_pRepairItem_Window )
	{
		if( m_pRepairItem_Window->GetShowStatus() )
		{
			list<int>::iterator iter = m_pRepairItem_Window->m_SelectedSlotNoList.begin();
			for( ; iter != m_pRepairItem_Window->m_SelectedSlotNoList.end(); iter++ )
			{	
				RECT iconrect;
				int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
				int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
				int column = 0, row = 0;
				int selectpos = *iter -_XINVENTORY_SLOTTYPE_POCKETSTART;				
				
				if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )
				{
					selectpos -= (m_SelectedBaseTab*25);
					
					row = 33 * (selectpos/5);
					column = 33 * (selectpos%5);
					
					iconrect.left = columnpoint + column;
					iconrect.top = rowpoint + row;
					iconrect.right = columnpoint + column + 30;
					iconrect.bottom = rowpoint + row + 30;
					
					// disable icon image
					_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
					_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
				}			
			}
		}
	}
}

void _XWindow_Inventory::DrawMixWeaponItem()
{
	if(m_pMixWeapon_Window)
	{
		if(m_pMixWeapon_Window->GetShowStatus())
		{
			for(int i = 0 ; i < 3 ; ++i)
			{
				if(m_pMixWeapon_Window->m_ItemSlot[i].nInvenNumber > _XINVENTORY_SLOTTYPE_POCKETSTART)
				{
					RECT iconrect;
					int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
					int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
					int column = 0, row = 0;
					int selectpos = m_pMixWeapon_Window->m_ItemSlot[i].nInvenNumber -_XINVENTORY_SLOTTYPE_POCKETSTART;				
					
					if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )
					{
						selectpos -= (m_SelectedBaseTab*25);
						
						row = 33 * (selectpos/5);
						column = 33 * (selectpos%5);
						
						iconrect.left = columnpoint + column;
						iconrect.top = rowpoint + row;
						iconrect.right = columnpoint + column + 30;
						iconrect.bottom = rowpoint + row + 30;
						
						// disable icon image
						_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
						_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
					}
				}
				
			}
		}
	}
}

void _XWindow_Inventory::DrawNPCResource_New_Item()
{
	if(m_pNPCResource_New_Window)
	{
		if(m_pNPCResource_New_Window->GetShowStatus())
		{
			int itemslot[16];
			memset(itemslot, 0, sizeof(itemslot));
			
			m_pNPCResource_New_Window->GetItemSlotIndex(itemslot);

			for(int i = 0 ; i < 16 ; ++i)
			{
				if(itemslot[i] >= _XINVENTORY_SLOTTYPE_POCKETSTART)
				{
					RECT iconrect;
					int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
					int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;
					int column = 0, row = 0;
					int selectpos = itemslot[i] -_XINVENTORY_SLOTTYPE_POCKETSTART;				
					
					if( selectpos >= (m_SelectedBaseTab*25) && selectpos < (m_SelectedBaseTab*25)+25 )
					{
						selectpos -= (m_SelectedBaseTab*25);
						
						row = 33 * (selectpos/5);
						column = 33 * (selectpos%5);
						
						iconrect.left = columnpoint + column;
						iconrect.top = rowpoint + row;
						iconrect.right = columnpoint + column + 30;
						iconrect.bottom = rowpoint + row + 30;
						
						// disable icon image
						_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
						_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_SELECT);				
					}
				}
				
			}
		}
	}
}

char _XWindow_Inventory::GetSocketListCount()//2004.11.9->hotblood insert
{
	int tempcount = 0;
	for( int i = 1; i < _XDEF_SOCKETITEM_MAXCOUNT+1; i++ )
	{
		if( m_iSocketIndexList[i] > 0 )
			tempcount++;
	}
	return tempcount;
}

char _XWindow_Inventory::GetSocketListInsertPos()
{
	for( int i = 1; i < _XDEF_SOCKETITEM_MAXCOUNT+1; i++ )
	{
		if( m_iSocketIndexList[i] > 0 )
			return i;
	}
	return 0;
}

void _XWindow_Inventory::ResetSocketIndexList()//2004.11.9->hotblood insert
{
	for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT+1; i++ )
	{
		m_iSocketIndexList[i] = 0;		
	}
	ResetMouseSlot();
}

void _XWindow_Inventory::ResetSocketIndexList_GambleBox()
{
	for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT; i++ )
	{
#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
		// inven �ǵ帮�� �ʴ´�
#else
		if( m_iSocketIndexList[i] > 0 )
			g_pLocalUser->m_UserItemList[m_iSocketIndexList[i]].Set_m_ucCount(g_pLocalUser->m_UserItemList[m_iSocketIndexList[i]].Get_m_ucCount()+1);
#endif
		m_iSocketIndexList[i] = 0;
	}
	ResetMouseSlot();
}

BOOL _XWindow_Inventory::CheckDropArea()
{
	//������ ó��
	if( m_pNPCTradeWindow && m_pNPCTradeWindow->GetShowStatus() && m_pNPCTradeWindow->CheckMousePosition())
	{
		// NPC �ŷ��ÿ��� �ŷ�â �����̸� ó���� ���� ����.
		return FALSE;
	}
	else if( m_pPCTradeWindow && m_pPCTradeWindow->GetShowStatus() && m_pPCTradeWindow->CheckMousePosition())
	{
		// PC �ŷ��ÿ��� �ŷ�â �����̸� ó���� ���� ����.
		return FALSE;
	}
	else if( m_pWarehouseWindow && m_pWarehouseWindow->GetShowStatus() && m_pWarehouseWindow->CheckMousePosition())
	{
		// â�� ����ÿ��� â��â �����̸� ó���� ���� ����.
		return FALSE;
	}
	else if( m_pEnterBossRoom_Window && m_pEnterBossRoom_Window->GetShowStatus() && m_pEnterBossRoom_Window->CheckMousePosition())
	{
		// ������ ���Խÿ��� ������ ����â �����̸� ó���� ���� ����.
		return FALSE;
	}
	else if( m_pRepairItem_Window && m_pRepairItem_Window->GetShowStatus() && m_pRepairItem_Window->CheckMousePosition() )
	{
		// �����ÿ��� ó���� ���� ����.
		return FALSE;
	}
	else if( m_pSocketPlugIn_Window && m_pSocketPlugIn_Window->GetShowStatus() && m_pSocketPlugIn_Window->CheckMousePosition() )
	{
		// ���� ������ ó������ ����
		return FALSE;
	}
	else if(g_pQuickSlot_Window && g_pQuickSlot_Window->CheckMousePosition() )
	{
		// �����Կ� ������ ������ ���� ó������ ����
		return FALSE;
	}
	else if( g_pCInfo_Window->CheckEquipedArea() )
	{
		// ĳ���� ����â ����������� ó�� ����.
		return FALSE;
	}
	else if( m_pContribution_Window && m_pContribution_Window->GetShowStatus() && m_pContribution_Window->CheckMousePosition())
	{
		// ���ı⿩�� â �̸� ó������ ����.
		return FALSE;
	}
	else if( m_pPersonalStore_Window && m_pPersonalStore_Window->GetShowStatus() && m_pPersonalStore_Window->CheckMousePosition())
	{
		// ���� ���� â �̸� ó������ ����.
		return FALSE;
	}
	else if( m_pTradeConfirm_Window && m_pTradeConfirm_Window->GetShowStatus() )
	{
		// ����â�� �� ������ ó������ ����.
		return FALSE;
	}
	else if( g_pInventory_Window && g_pInventory_Window->GetShowStatus() && g_pInventory_Window->CheckMousePosition() )
	{
		// �κ�â ���̸� ó������ ����.
		return FALSE;
	}
	return TRUE;
}

void _XWindow_Inventory::SetExpandDockingBorderFlag(BOOL flag)
{
	m_bExpandDockingBorderFlag = flag;

	_XWindow_SystemMsgDefault* systemWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);
	if( systemWindow )
	systemWindow->SetDockingMode(flag);	
	
}

void _XWindow_Inventory::ResetMouseSlot(void)
{
	m_bHaveMouseSlot = FALSE;
	m_MouseSlotItem.ResetMouseSlotItem();	
}

void _XWindow_Inventory::SetPriceMode(int mode)
{
	m_nPriceMode = mode;

	if(m_nPriceMode == _MODE_CLANPOINT)
		m_btnMoney->ShowWindow(FALSE);
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
	else if( m_nPriceMode == _MODE_MNDPOINT )
		m_btnMoney->ShowWindow(FALSE);
#endif
	else
		m_btnMoney->ShowWindow(TRUE);
}
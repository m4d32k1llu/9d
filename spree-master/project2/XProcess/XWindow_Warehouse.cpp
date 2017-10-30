// XWindow_Warehouse.cpp: implementation of the _XWindow_Warehouse class.
//
// Author : Hangil, Yang
// Modify : Sohyun, Park
// Desc : User warehouse, Warehouse window class
//
//////////////////////////////////////////////////////////////////////

#pragma warning (disable: 4786)

#include "stdafx.h"
#include "XWindow_Warehouse.h"
#include "XWindow_TradeConfirm.h"
#include "XWindow_NPCScript.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XMessageWindow.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static int _XDEF_WAREHOUSEITEMPOSX = 11;
static int _XDEF_WAREHOUSEITEMPOSY = 72;

extern unsigned long g_MoneyColor[6];


_XWindow_Warehouse::_XWindow_Warehouse()
{
	for( int i = 0; i < 3; ++i )
		m_SelectTabButtton[i] = NULL;

	m_SelectedTab		= 0;
	m_MoveButton		= NULL;
	m_SelectAllButton	= NULL;
	m_CancelButton		= NULL;
	m_StorageMoneyButton = NULL;
	m_InventoryMoneyButton = NULL;	

	m_bWarehouseFirstLoad	= FALSE;
	m_bWarehouseFirstView	= TRUE; // 2004.06.01->oneway48 insert 
	m_LastDraggingSlot	= -1;
	m_TempMoney			= 0;

	m_SendMode			= _XSENDMODE_NONE;
	m_bWaitForServerResult	= FALSE;	
	
	m_MouseSlotItem.ResetMouseSlotItem();
	m_bDragState		= FALSE;
	m_bHaveMouseSlot	= FALSE;

	m_bProcessOneClickItem = FALSE;
	m_bDrawSelectedItem = FALSE;

	m_IconDragStartPos.x = m_IconDragStartPos.y = 0;
	
	m_UseSlotCount = 0; // 2004.05.20->oneway48 insert : �⺻ 10ĭ
	m_bUseableWarehouse = FALSE; // 2004.06.01->oneway48 insert
	m_bWarehouseCancleMode = FALSE; // 2004.06.01->oneway48 insert
	m_bTerminateWarehouse = FALSE; // 2004.06.02->oneway48 insert
	m_TerminateLastItemIndex = 0;
	m_TradeDelayTime = 0;
	
	m_WindowMouseState = _XW_STATE_NONE;
	m_dwStorageInfoReqTime = 0;
}

_XWindow_Warehouse::~_XWindow_Warehouse()
{
}

BOOL _XWindow_Warehouse::Initialize(void)
{
	SetDrawBorder( FALSE );
	// Create GUI Object =================================================================
	int resourceindexmainback01 = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	int resourceindexnpcquestwin01 = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
	int resourcechartexturename = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	int resourceindexsystem = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );

	// Border---------------------------------------------------------------------------------------------=

	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create(0, 0, 184, 70, &g_MainInterfaceTextureArchive, resourceindexmainback01 );
	pTitleBar->SetClipRect( 0, 125, 183, 194 );
	InsertChildObject(pTitleBar);

	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create( 0, 69, 184, 269, &g_MainInterfaceTextureArchive, 
						   g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" ));
	pBottomBorder->SetClipRect( 0, 10,  183, 211 );
	InsertChildObject( pBottomBorder );	

	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].Create( 0, 0, 34, 34, &g_MainInterfaceTextureArchive, resourceindexmainback01 );
	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_EMPTY].SetClipRect( 213, 64, 247, 98 );

	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_NORMAL].Create( 0, 0, 34, 34, &g_MainInterfaceTextureArchive, resourceindexmainback01 );
	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_NORMAL].SetClipRect( 213, 30, 247, 64 );

	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_DISABLESLOT].Create( 0, 0, 30, 30, &g_MainInterfaceTextureArchive, resourceindexmainback01 );
	m_ItemEdgeIcon[_XITEMEDGEICONTYPE_DISABLESLOT].SetClipRect( 216, 0, 246, 30 );
	// ---------------------------------------------------------------------------------------------------=

	// Line ----------------------------------------------------------------------------------------------=

	_XImageStatic* pTopWindowLine = new _XImageStatic;
	pTopWindowLine->Create( 5, 66, 178, 68, &g_MainInterfaceTextureArchive, resourceindexnpcquestwin01 );
	pTopWindowLine->SetClipRect( 21, 490, 195, 493 );
	InsertChildObject( pTopWindowLine );

	_XImageStatic* pLeftWindowLine = new _XImageStatic;
	pLeftWindowLine->Create( 5, 69, 7, 203, &g_MainInterfaceTextureArchive, resourceindexnpcquestwin01 );
	pLeftWindowLine->SetClipRect( 495, 283, 498, 418 );
	InsertChildObject( pLeftWindowLine );


	_XImageStatic* pRightWindowLine = new _XImageStatic;
	pRightWindowLine->Create( 176, 69, 178, 203, &g_MainInterfaceTextureArchive, resourceindexnpcquestwin01 );
	pRightWindowLine->SetClipRect( 501, 283, 504, 418 );
	InsertChildObject( pRightWindowLine );
	
	_XImageStatic* pBottomWindowLine = new _XImageStatic;
	pBottomWindowLine->Create( 5, 204, 178, 206, &g_MainInterfaceTextureArchive, resourceindexnpcquestwin01 );
	pBottomWindowLine->SetClipRect( 21, 500, 195, 503 );
	InsertChildObject( pBottomWindowLine );	
	// ---------------------------------------------------------------------------------------------------=

	// Icon Button ----------------------------------------------------------------------------=
	_XImageStatic* pWareHouseIcon = new _XImageStatic;
	// �������� ��ǥ�� ������ �ʴ´�.
	pWareHouseIcon->Create( 7, 25, 46, 64, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "mi_npcname.tga" ) );
	pWareHouseIcon->SetClipRect( 196, 4, 236, 44 );
	InsertChildObject( pWareHouseIcon );
	// ----------------------------------------------------------------------------------------=


	// ---------------------------------------------------------------------------------------------------=
	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_WAREHOUSEWINDOW_HELP,
		resourcechartexturename,resourcechartexturename,resourcechartexturename, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	// ----------------------------------------------------------------------------------------=
	
	// �ؿ� ������ �� 
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{	
		// Tab, �Ա�, ��� Button ----------------------------------------------------------------------------=	
		_XCHECKBTN_STRUCTURE tabbtnstruct = { TRUE, { 53, 43 }, { 20, 20 }, 
			_XDEF_WAREHOUSEWINDOW_ONE,
			resourceindexnpcquestwin01, resourceindexnpcquestwin01,		
			&g_MainInterfaceTextureArchive };
		
		TCHAR tempbuf[2];
		for( int i = 0; i < 3; ++i )
		{
			m_SelectTabButtton[i] = new _XCheckButton;
			m_SelectTabButtton[i]->Create( tabbtnstruct );		
			m_SelectTabButtton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 431, 10,  451, 30 );
			m_SelectTabButtton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  485, 10,  505, 30 );
			m_SelectTabButtton[i]->SetButtonSwapMode(TRUE);
			m_SelectTabButtton[i]->SetCheck( i == 0 );
			sprintf( tempbuf, "%d", i);
			m_SelectTabButtton[i]->SetButtonText( tempbuf );
			InsertChildObject( m_SelectTabButtton[i] );
			
			tabbtnstruct.position.x += 24;
			tabbtnstruct.commandid++;		
		}
	
		// �Ա� ��ư ( 79X16 )
		_XBTN_STRUCTURE inventorymoneybtnstruct = { TRUE, { 101, 207 }, { 79, 16 }, 
			_XDEF_WAREHOUSEWINDOW_INVENTORYMONEY,
			resourceindexnpcquestwin01, resourceindexnpcquestwin01, resourceindexnpcquestwin01,
			&g_MainInterfaceTextureArchive };	
		_XButton* pInventorymoneybutton = new _XButton;
		pInventorymoneybutton->Create( inventorymoneybtnstruct );	
		
		pInventorymoneybutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  429, 39, 508, 55);
		pInventorymoneybutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  429, 59, 508, 75 );	
		pInventorymoneybutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  429, 79, 508, 95 );
		pInventorymoneybutton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_SENDMONEY );	// _T("�Ա�")
		InsertChildObject( pInventorymoneybutton );
		
		// ��� ��ư ( 79X16 )
		_XBTN_STRUCTURE storagemoneybtnstruct = { TRUE, { 101, 224 }, { 79, 16 }, 
			_XDEF_WAREHOUSEWINDOW_STORAGEMONEY,
			resourceindexnpcquestwin01, resourceindexnpcquestwin01, resourceindexnpcquestwin01,		
			&g_MainInterfaceTextureArchive };	
		_XButton* pStorageMoneybutton = new _XButton;
		pStorageMoneybutton->Create( storagemoneybtnstruct );	
		
		pStorageMoneybutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  429, 39, 508, 55);
		pStorageMoneybutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  429, 59, 508, 75 );	
		pStorageMoneybutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  429, 79, 508, 95 );
		pStorageMoneybutton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_RECEIVEMONEY );	// _T("���")
		InsertChildObject( pStorageMoneybutton );	
		
		// Money ----------------------------------------------------------------------------------------------=
		_XImageStatic* pWarehouseMoneyBoxImg = new _XImageStatic;
		pWarehouseMoneyBoxImg->Create( 3, 213, 100, 229, &g_MainInterfaceTextureArchive, resourceindexmainback01 );
		pWarehouseMoneyBoxImg->SetClipRect(7, 201, 104, 219);
		InsertChildObject( pWarehouseMoneyBoxImg );
		
		// Money Icon ( 15X12 )
		_XImageStatic* pWarehouseMoneyImg = new _XImageStatic;
		pWarehouseMoneyImg->Create( 7, 216, 22, 228, &g_MainInterfaceTextureArchive, resourceindexmainback01 );
		pWarehouseMoneyImg->SetClipRect(185, 6, 200, 18);
		InsertChildObject( pWarehouseMoneyImg );
		// ----------------------------------------------------------------------------------------------------=
		
		int resourceindexquest02 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );

		//â�� �ؾ� 
		_XBTN_STRUCTURE Removalbtnstruct = { TRUE, { 103, 41 }, { 76,  22 }, _XDEF_WAREHOUSEWINDOW_REMOVAL,
			resourceindexquest02,resourceindexquest02,resourceindexquest02, 
			&g_MainInterfaceTextureArchive };
		_XButton* pRemovalButton = new _XButton;
		pRemovalButton->Create( Removalbtnstruct );	
		
		pRemovalButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 81, 192, 157, 214 );
		pRemovalButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  81, 192, 157, 214 );	
		pRemovalButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0, 190, 76, 212 );
		pRemovalButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_REMOVAL ); // â���ؾ�
		InsertChildObject( pRemovalButton );
				
		_XBTN_STRUCTURE SelectedAllbtnstruct = { TRUE, { 35, 242 }, { 61, 25 }, _XDEF_WAREHOUSEWINDOW_ALLSELECT,
			resourceindexsystem,resourceindexsystem,resourceindexsystem, 
			&g_MainInterfaceTextureArchive };
		_XButton* pSelectedAllButton = new _XButton;
		pSelectedAllButton->Create( SelectedAllbtnstruct );	
		
		pSelectedAllButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
		pSelectedAllButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );	
		pSelectedAllButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
		pSelectedAllButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_SELECTALL );	// _T("���μ���")
		InsertChildObject( pSelectedAllButton );
		
		_XBTN_STRUCTURE Movebtnstruct = { TRUE, { 95, 242 }, { 61,  25 }, _XDEF_WAREHOUSEWINDOW_SENDADDITEM,
			resourceindexsystem,resourceindexsystem,resourceindexsystem, 
			&g_MainInterfaceTextureArchive };
		m_MoveButton = new _XButton;
		m_MoveButton->Create( Movebtnstruct );	
		
		m_MoveButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
		m_MoveButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );	
		m_MoveButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
		m_MoveButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_RECEIVE );	// _T("�ñ��")
		InsertChildObject( m_MoveButton );
	}
	else //���� ������ ��
	{
		// Tab, �Ա�, ��� Button ----------------------------------------------------------------------------=	
		_XCHECKBTN_STRUCTURE tabbtnstruct = { TRUE, 
		#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
			{ 49, 43 }, 
		#else
			{ 59, 43 }, 
		#endif
			{ 20, 20 }, 
			_XDEF_WAREHOUSEWINDOW_ONE,
			resourceindexnpcquestwin01, resourceindexnpcquestwin01,		
			&g_MainInterfaceTextureArchive };
		
		TCHAR tempbuf[2];
		for( int i = 0; i < 3; ++i )
		{
			m_SelectTabButtton[i] = new _XCheckButton;
			m_SelectTabButtton[i]->Create( tabbtnstruct );		
			m_SelectTabButtton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 431, 10,  451, 30 );
			m_SelectTabButtton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  485, 10,  505, 30 );
			m_SelectTabButtton[i]->SetButtonSwapMode(TRUE);
			m_SelectTabButtton[i]->SetCheck( i == 0 );
			sprintf( tempbuf, "%d", i);
			m_SelectTabButtton[i]->SetButtonText( tempbuf );
			InsertChildObject( m_SelectTabButtton[i] );
			
			tabbtnstruct.position.x += 24;
			tabbtnstruct.commandid++;		
		}
	
		_XBTN_STRUCTURE inventorymoneybtnstruct = { TRUE, { 108, 214 }, { 33, 17 }, 
			_XDEF_WAREHOUSEWINDOW_INVENTORYMONEY,
			resourceindexnpcquestwin01, resourceindexnpcquestwin01, resourceindexnpcquestwin01,
			&g_MainInterfaceTextureArchive };	
		_XButton* pInventorymoneybutton = new _XButton;
		pInventorymoneybutton->Create( inventorymoneybtnstruct );	
		
		pInventorymoneybutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  452, 340, 485, 357);
		pInventorymoneybutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  452, 360, 485, 377 );	
		pInventorymoneybutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  452, 380, 485, 397 );
		pInventorymoneybutton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_SENDMONEY );	// _T("�Ա�")
		InsertChildObject( pInventorymoneybutton );
		
		_XBTN_STRUCTURE storagemoneybtnstruct = { TRUE, { 143, 214 }, { 33, 17 }, 
			_XDEF_WAREHOUSEWINDOW_STORAGEMONEY,
			resourceindexnpcquestwin01, resourceindexnpcquestwin01, resourceindexnpcquestwin01,		
			&g_MainInterfaceTextureArchive };	
		_XButton* pStorageMoneybutton = new _XButton;
		pStorageMoneybutton->Create( storagemoneybtnstruct );	
		
		pStorageMoneybutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  452, 340, 485, 357);
		pStorageMoneybutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  452, 360, 485, 377 );	
		pStorageMoneybutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  452, 380, 485, 397 );
		pStorageMoneybutton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_RECEIVEMONEY );	// _T("���")
		InsertChildObject( pStorageMoneybutton );	

		// Money ----------------------------------------------------------------------------------------------=
		_XImageStatic* pWarehouseMoneyBoxImg = new _XImageStatic;
		pWarehouseMoneyBoxImg->Create( 7, 213, 103, 229, &g_MainInterfaceTextureArchive, resourceindexmainback01 );
		pWarehouseMoneyBoxImg->SetClipRect(7, 201, 104, 219);
		InsertChildObject( pWarehouseMoneyBoxImg );
		
		_XImageStatic* pWarehouseMoneyImg = new _XImageStatic;
		pWarehouseMoneyImg->Create( 15, 218, 25, 225, &g_MainInterfaceTextureArchive, resourceindexmainback01 );
		pWarehouseMoneyImg->SetClipRect(185, 7, 196, 15);
		InsertChildObject( pWarehouseMoneyImg );
		// ----------------------------------------------------------------------------------------------------=
		
		//â�� �ؾ� 		
		_XBTN_STRUCTURE Removalbtnstruct = { TRUE, { 118, 41 }, { 61,  25 }, _XDEF_WAREHOUSEWINDOW_REMOVAL,
			resourceindexsystem,resourceindexsystem,resourceindexsystem, 
			&g_MainInterfaceTextureArchive };
		_XButton* pRemovalButton = new _XButton;
		pRemovalButton->Create( Removalbtnstruct );	
		
		pRemovalButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
		pRemovalButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );	
		pRemovalButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
		pRemovalButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_REMOVAL ); // â���ؾ�
		InsertChildObject( pRemovalButton );
		
		_XBTN_STRUCTURE SelectedAllbtnstruct = { TRUE, { 35, 238 }, { 61, 25 }, _XDEF_WAREHOUSEWINDOW_ALLSELECT,
			resourceindexsystem,resourceindexsystem,resourceindexsystem, 
			&g_MainInterfaceTextureArchive };
		_XButton* pSelectedAllButton = new _XButton;
		pSelectedAllButton->Create( SelectedAllbtnstruct );	
		
		pSelectedAllButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
		pSelectedAllButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );	
		pSelectedAllButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
		pSelectedAllButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_SELECTALL );	// _T("���μ���")
		InsertChildObject( pSelectedAllButton );
		
		_XBTN_STRUCTURE Movebtnstruct = { TRUE, { 95, 239 }, { 61,  25 }, _XDEF_WAREHOUSEWINDOW_SENDADDITEM,
			resourceindexsystem,resourceindexsystem,resourceindexsystem, 
			&g_MainInterfaceTextureArchive };
		m_MoveButton = new _XButton;
		m_MoveButton->Create( Movebtnstruct );	
		
		m_MoveButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
		m_MoveButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );	
		m_MoveButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
		m_MoveButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_RECEIVE );	// _T("�ñ��")
		InsertChildObject( m_MoveButton );
	}
	// ���μ���, ������, ���� Button ----------------------------------------------------------------------------=


	// ----------------------------------------------------------------------------------------------------=	
	
	for(int i = 0 ; i < _XDEF_WAREHOUSE_MAX_SENDCOUNT ; ++i)
	{
		m_SlotPosition[i].x = _XDEF_WAREHOUSEITEMPOSX + ((i%5)*33);
		m_SlotPosition[i].y = _XDEF_WAREHOUSEITEMPOSY + ((i/5)*33);
	}

	return TRUE;
}

void _XWindow_Warehouse::DestroyWindow(void)
{	
	_XWindow::DestroyWindow();
}

void _XWindow_Warehouse::ChangeTab( int tab )
{
	m_SelectedTab = tab;
	for( int i = 0; i < 3; ++i )
		m_SelectTabButtton[i]->SetCheck( i == tab );

	m_MouseSlotItem.ResetMouseSlotItem();
	m_bHaveMouseSlot = FALSE;
}

void _XWindow_Warehouse::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	int startnumber = m_SelectedTab * _XDEF_WAREHOUSE_MAX_SENDCOUNT;
	int count = 0;

	for( int i = 0; i < _XDEF_WAREHOUSE_MAX_SENDCOUNT; ++i )
	{
		if( m_bTerminateWarehouse )	// ����� �ִ� �����۸� �����ش�.
		{
			if(g_pLocalUser->m_WarehouseItemList[i+startnumber].Get_m_sID() > 0)
			{
				m_ItemEdgeIcon[_XNT_ITEMEDGEICONTYPE_NORMAL].Draw( m_WindowPosition.x+m_SlotPosition[i].x-2, 
					m_WindowPosition.y+m_SlotPosition[i].y-2 );
				g_pInventory_Window->DrawIcon( TRUE, m_WindowPosition.x+m_SlotPosition[i].x, m_WindowPosition.y+m_SlotPosition[i].y, 
					(_XUserItem*)&g_pLocalUser->m_WarehouseItemList[i+startnumber] );
			}
			else
			{
				m_ItemEdgeIcon[_XNT_ITEMEDGEICONTYPE_DISABLESLOT].Draw( m_WindowPosition.x+m_SlotPosition[i].x-2, m_WindowPosition.y+m_SlotPosition[i].y-2 );
			}
		}
		else
		{
			if( i+startnumber >= m_UseSlotCount )	// ������ ĭ
			{
				m_ItemEdgeIcon[_XNT_ITEMEDGEICONTYPE_DISABLESLOT].Draw( m_WindowPosition.x+m_SlotPosition[i].x, 
					m_WindowPosition.y+m_SlotPosition[i].y );						
			}
			else
			{
				if(g_pLocalUser->m_WarehouseItemList[i+startnumber].Get_m_sID() > 0)
				{
					m_ItemEdgeIcon[_XNT_ITEMEDGEICONTYPE_NORMAL].Draw( m_WindowPosition.x+m_SlotPosition[i].x-2, 
						m_WindowPosition.y+m_SlotPosition[i].y-2 );
					g_pInventory_Window->DrawIcon( TRUE, m_WindowPosition.x+m_SlotPosition[i].x, m_WindowPosition.y+m_SlotPosition[i].y, 
						(_XUserItem*)&g_pLocalUser->m_WarehouseItemList[i+startnumber] );
				}
				else
				{
					m_ItemEdgeIcon[_XNT_ITEMEDGEICONTYPE_EMPTY].Draw( m_WindowPosition.x+m_SlotPosition[i].x-2, m_WindowPosition.y+m_SlotPosition[i].y-2 );
				}
			}
		}		
	}

	DrawSelectedItem();

	TCHAR moneyStr[128];
	memset( moneyStr, 0, sizeof(moneyStr));
	sprintf(moneyStr, "%u", g_pLocalUser->m_WarehouseMoney);
//	itoa(g_pLocalUser->m_WarehouseMoney, moneyStr, 10 );
	int colorindex = 0;

	if( _XGameItem::GetMoneyColor(moneyStr, colorindex) )
	{
		g_XBaseFont->SetColor( g_MoneyColor[colorindex] );
		// �ؿ� ������ �� 
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+98, m_WindowPosition.y+217, 1.0f, _XFONT_ALIGNTYPE_RIGHT, moneyStr );
		}
		else// ���� ������ �� 
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+100, m_WindowPosition.y+217, 1.0f, _XFONT_ALIGNTYPE_RIGHT, moneyStr );
		}
	}
	else
	{
		g_XBaseFont->SetColor(_XSC_INFORMATION);
		g_XBaseFont->PrintAlign(m_WindowPosition.x + 100, m_WindowPosition.y + 217, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%u", // 2004.05.31->oneway48 modify
			g_pLocalUser->m_WarehouseMoney);
	}	
	g_XBaseFont->Flush();	
}

BOOL _XWindow_Warehouse::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
	if( pTradeConfirm_Window->GetShowStatus() )
	{
		if(pfocusobject == _XDEF_DUMMYID_WAREHOUSE) pfocusobject = NULL;
		if(!_XWindow::Process(pfocusobject)) return FALSE;
	}	

	// �������Ȯ�� ���� 1�� �Ŀ� �ڵ����� Ǯ�� -------------------------=
	if( m_bWaitForServerResult )
	{
		if( g_LocalSystemTime - m_TradeDelayTime > 1000 )
		{
			m_bWaitForServerResult = FALSE;
			m_TradeDelayTime = 0;
		}
	}
	// ---------------------------------------=

	BOOL	bUseWarehouse = FALSE;
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[1])
			{
				// mouse right down
				if(CheckMousePosition())
					m_WindowMouseState = _XW_STATE_RIGHTCLICK;
				else
				{
					if(m_bHaveMouseSlot)
					{
						m_bHaveMouseSlot = FALSE;
						m_MouseSlotItem.ResetMouseSlotItem();
					}
				}

			}
			if(mousestate->bButton[0])
			{
				// mouse left down
				if(CheckMousePosition())
				{
					// Focus
					if(scrnpos->x > m_WindowPosition.x+7 && scrnpos->x < m_WindowPosition.x+174)
					{
						if(scrnpos->z > m_WindowPosition.y+69 && scrnpos->z < m_WindowPosition.y+204)
						{
							pfocusobject = _XDEF_DUMMYID_WAREHOUSE;
						}
					}

					if(g_pInventory_Window)
					{
						if(g_pInventory_Window->GetShowStatus())
						{
							if( m_bWaitForServerResult ) return FALSE;	// ������ �� �� ���¿����� �ٽ� ������ �ʴ´�.
							
							if(g_pInventory_Window->m_bHaveMouseSlot && g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART)
							{
								if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() == _XGI_FC_QUEST )
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTSAVEQUEST), TRUE);									

									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
								}
								/*else if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].m_cType == _XGI_FC_POTION &&
										   g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].m_cSecond == _XGI_SC_POTION_HWATA )
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_INVENTORY_2122), TRUE);

									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
								}*/
#ifdef _XDEF_WAREHOSE_RESTRAINT_OF_TRADE //Author : ����� //breif : â��ŷ� ����
								else if( _XGameItem::CheckItemTempField4( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() ,
															g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sID() ) )
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3404), TRUE); //�ش� �������� â�� ������ �� �����ϴ�.

									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
								}
#endif
								else
								{
									// â�� �ֱ�
									if(g_pInventory_Window->m_MouseSlotItem.IsStackable())
									{
										_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)m_pMyManager->FindWindow(_XDEF_WTITLE_TRADECONFIRM);
										if(pTradeConfirm_Window)
										{
											pTradeConfirm_Window->ShowWindow(TRUE);
											pTradeConfirm_Window->m_IMEControl.SetFocus();
											pTradeConfirm_Window->m_SelectedCategory1 = g_pInventory_Window->m_MouseSlotItem.Get_m_cType();
											pTradeConfirm_Window->m_SelectedItemID = g_pInventory_Window->m_MouseSlotItem.Get_m_sID();
											pTradeConfirm_Window->m_TradeMode = _XWAREHOUSE_TOWAREHOUSE_ITEM;
										}
									}
									else
									{
										SetTempItemFromInven(g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber, 1);
										m_SendMode = _XSENDMODE_TOWAREHOUSE_ITEM;
										PostMessage(gHWnd, WM_COMMAND, _XDEF_WAREHOUSEWINDOW_SENDADDITEM, (LPARAM)gHWnd);
									}
									bUseWarehouse = TRUE;
								}								
							}
						}
					}

					if(!bUseWarehouse)
					{
						if(!m_bHaveMouseSlot)
						{
							// mouse down ��ġ ����
							m_IconDragStartPos.x = scrnpos->x;
							m_IconDragStartPos.y = scrnpos->z;
							m_LastDraggingSlot = GetSlotIndex();
							m_bDragState = TRUE;
						}
						else
						{
							m_MouseSlotItem.ResetMouseSlotItem();
							m_bHaveMouseSlot = FALSE;
						}
					}
					
					if(g_pInventory_Window )
					{
						if( g_pInventory_Window->GetShowStatus())
						{
							g_pInventory_Window->m_bDrawSelectedItem = FALSE;
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )						
									g_pDefaultTooltip_Window->ShowWindow( FALSE );
							}
						}
					}
				} // if(CheckMousePosition())
				else
				{
					// Tooltip show false
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )						
						{
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
							m_LastDraggingSlot = -1;
						}
					}
					
					if( g_pInventory_Window && g_pInventory_Window->GetShowStatus() )
					{

					}
					else
					{
						if(m_bHaveMouseSlot)
						{
							m_MouseSlotItem.ResetMouseSlotItem();
							m_bHaveMouseSlot = FALSE;
							m_LastDraggingSlot = -1;
						}
					}					
				}
			}
		}
		if(pfocusobject == _XDEF_DUMMYID_WAREHOUSE)
		{
			if(mousestate->bButton[0])
			{
				// �巡�� ó��
				if(m_bDragState && !m_bHaveMouseSlot)
				{
					if(scrnpos->x != m_IconDragStartPos.x || scrnpos->z != m_IconDragStartPos.y)
					{
						if(m_LastDraggingSlot != -1 && g_pLocalUser->m_WarehouseItemList[m_LastDraggingSlot].Get_m_sID() > 0 )
						{
							ReleaseTempItemList();
							m_bHaveMouseSlot = TRUE;
							m_MouseSlotItem.Copy(g_pLocalUser->m_WarehouseItemList[m_LastDraggingSlot]);
							m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
							m_LastDraggingSlot = -1;
							
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )						
									g_pDefaultTooltip_Window->ShowWindow( FALSE );
							}
						}
					}
				} // if(m_bDragState && !m_bHaveMouseSlot)
			}

			if(!mousestate->bButton[0])
			{
				m_bDrawSelectedItem = TRUE;				

				if(gpInput->CheckKeyPress(DIK_LCONTROL))
				{
					if(m_bDragState)
					{
						if(scrnpos->x == m_IconDragStartPos.x && scrnpos->z == m_IconDragStartPos.y)
						{
							// �ߺ� ����
							if(g_pLocalUser->m_WarehouseItemList[m_LastDraggingSlot].Get_m_sID() > 0)
							{
								m_bProcessOneClickItem = FALSE;
								if(HaveItemFromWarehouse(m_LastDraggingSlot))
								{
									// ���� �ִ���
									ResetTempItemFromWarehouse(m_LastDraggingSlot);
								}
								else
								{
									SetTempItemFromWarehouse(m_LastDraggingSlot, g_pLocalUser->m_WarehouseItemList[m_LastDraggingSlot].Get_m_ucCount());									
								}
								
								m_LastDraggingSlot = -1;
								if( g_pDefaultTooltip_Window )
								{
									if( g_pDefaultTooltip_Window->GetShowStatus() )						
										g_pDefaultTooltip_Window->ShowWindow( FALSE );
								}
							}
						}
					}
				}
				else
				{
					char	cType = 0;
					short	sID = 0;

					if(m_bDragState)
					{
						if(scrnpos->x == m_IconDragStartPos.x && scrnpos->z == m_IconDragStartPos.y)
						{
							// ���ڸ����� Ŭ�� - ���� �����ֱ�
							if(g_pLocalUser->m_WarehouseItemList[m_LastDraggingSlot].Get_m_sID() > 0)
							{
								ReleaseTempItemList();
								m_MouseSlotItem.Copy(g_pLocalUser->m_WarehouseItemList[m_LastDraggingSlot]);
								m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
								SetTempItemFromWarehouse(m_LastDraggingSlot, m_MouseSlotItem.Get_m_ucCount());								
								if( g_pDefaultTooltip_Window )
								{
									g_pDefaultTooltip_Window->SetWarehouseWindowPtr( this );
									
									if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_WAREHOUSE_WAREHOUSEITEM, m_LastDraggingSlot,
										g_pLocalUser->m_WarehouseItemList[m_LastDraggingSlot].Get_m_cType(), g_pLocalUser->m_WarehouseItemList[m_LastDraggingSlot].Get_m_sID() ) )
									{
										g_pDefaultTooltip_Window->ShowWindow( TRUE );
									}
								}
								m_LastDraggingSlot = -1;
								m_bProcessOneClickItem = TRUE;
							}
							else
							{
								ReleaseTempItemList();
								if( g_pDefaultTooltip_Window )
								{
									if( g_pDefaultTooltip_Window->GetShowStatus() )						
										g_pDefaultTooltip_Window->ShowWindow( FALSE );
								}
							}
						}
						else
						{
							if(m_bDragState && m_bHaveMouseSlot)
							{
								int selectedslot = GetSlotIndex();
								if(selectedslot == m_MouseSlotItem.m_InventoryNumber)
								{
									// drag�� ���¿��� �� slot�� ���������� ���� - Reset
									m_MouseSlotItem.ResetMouseSlotItem();
									m_bHaveMouseSlot = FALSE;
									m_bDragState = FALSE;
									m_LastDraggingSlot = -1;
								}
								else
								{
									// drag ����
									m_bDragState = FALSE;
								}
							}
						}
					} // if(m_bDragState)
				} // ctrl
			} //if(!mousestate->bButton[0])
		}

		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				BOOL resetmouse = TRUE;
				if(!m_bHaveMouseSlot)
				{
					_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
					if(pTradeConfirm_Window)
					{
						if(pTradeConfirm_Window->GetShowStatus())
							return FALSE;
					}
					
					if( m_bWaitForServerResult ) return FALSE;	// ������ �� �� ���¿����� �ٽ� ������ �ʴ´�.
					
					ReleaseTempItemList();
					m_LastDraggingSlot = GetSlotIndex();					
					if(g_pLocalUser->m_WarehouseItemList[m_LastDraggingSlot].Get_m_sID() > 0)
					{
						m_MouseSlotItem.Copy(g_pLocalUser->m_WarehouseItemList[m_LastDraggingSlot]);
						m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
						SetTempItemFromWarehouse(m_MouseSlotItem.m_InventoryNumber, m_MouseSlotItem.Get_m_ucCount());
						m_SendMode = _XSENDMODE_TOINVEN_ITEM;
						PostMessage(gHWnd, WM_COMMAND, _XDEF_WAREHOUSEWINDOW_SENDADDITEM, (LPARAM)gHWnd);
						m_WindowMouseState = _XW_STATE_NONE;
						m_LastDraggingSlot = -1;
						resetmouse = FALSE;
					}
				}
				if(g_pInventory_Window)
				{
					if(g_pInventory_Window->GetShowStatus() && g_pInventory_Window->CheckMousePosition())
					{
						resetmouse = FALSE;
					}
				}
				if(resetmouse)
				{
					m_bHaveMouseSlot = FALSE;
					m_MouseSlotItem.ResetMouseSlotItem();
					m_WindowMouseState = _XW_STATE_NONE;
					m_LastDraggingSlot = -1;
				}
			}
		}
		
		if(!mousestate->bButton[0])
		{
			if(pfocusobject == _XDEF_DUMMYID_WAREHOUSE)
				pfocusobject = NULL;
		}
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_Warehouse::ShowWindow(BOOL show)
{
	if(show)
	{
		ChangeTab(0);

		if(!m_bWarehouseFirstLoad)
			m_bWarehouseFirstLoad = TRUE;

		// â�� ����� Inventory lock
		if(g_pInventory_Window)
		{
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;
		}
	}
	else
	{
		ReleaseTempItemList();

		// â�� ��� ������ Inventory lock ����
		if(g_pInventory_Window)
		{
			if(g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = FALSE;
		}

		if( g_pDefaultTooltip_Window )
		{
			if( g_pDefaultTooltip_Window->GetShowStatus() )						
				g_pDefaultTooltip_Window->ShowWindow( FALSE );
		}
	}

	_XWindow::ShowWindow(show);
}

BOOL _XWindow_Warehouse::HaveItemFromInven(int invenindex)
{
	list <_XWarehouseItem*>::iterator iter_item;
	for(iter_item = m_TempItemList.begin() ; iter_item != m_TempItemList.end() ; ++iter_item)
	{
		_XWarehouseItem* pItem = *iter_item;
		if(pItem)
		{
			if(pItem->m_InventoryNumber == invenindex)
				return TRUE;
		}
	}

	return FALSE;
}

BOOL _XWindow_Warehouse::HaveItemFromWarehouse(int slotindex)
{
	list <_XWarehouseItem*>::iterator iter_item;
	for(iter_item = m_TempItemList.begin() ; iter_item != m_TempItemList.end() ; ++iter_item)
	{
		_XWarehouseItem* pItem = *iter_item;
		if(pItem)
		{
			if(pItem->m_WarehouseNumber == slotindex)
				return TRUE;
		}
	}
	
	return FALSE;
}


void _XWindow_Warehouse::SetTempItemFromInven(int invenindex, int count)
{
	m_MoveButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_RECEIVE);	// "�ñ��"

	if(m_bWaitForServerResult)
		return;

	if(g_pLocalUser->m_UserItemList[invenindex].Get_m_sID() == 0)
		return;

	if( g_pLocalUser->m_UserItemList[invenindex].Get_m_cType() == _XGI_FC_QUEST )
		return;

	if(m_SendMode == _XSENDMODE_TOINVEN_MONEY || m_SendMode == _XSENDMODE_TOWAREHOUSE_MONEY)
		return;

	if( m_TempItemList.size() >= _XDEF_WAREHOUSE_MAX_SENDCOUNT )
		return;

	m_SendMode = _XSENDMODE_TOWAREHOUSE_ITEM;

	BOOL	bClearAll = FALSE;
	if(!m_TempItemList.empty())
	{
		list <_XWarehouseItem*>::iterator iter_item = m_TempItemList.begin();
		_XWarehouseItem* pWarehouseItem = *iter_item;
		if(pWarehouseItem)
		{
			if(pWarehouseItem->m_WarehouseNumber != -1)
			{
				bClearAll = TRUE;
			}
		}
	}

	if(bClearAll)
	{
		list <_XWarehouseItem*>::iterator iter_item;
		for(iter_item = m_TempItemList.begin() ; iter_item != m_TempItemList.end() ; )
		{
			_XWarehouseItem* pWarehouseItem = *iter_item;
			if(pWarehouseItem)
			{
				// ������ â���� �� ��, clear �Ѵ�.
				SAFE_DELETE(pWarehouseItem);
				iter_item = m_TempItemList.erase(iter_item);
			}
		}
		m_TempItemList.clear();
	}

	if(m_TempItemList.size() >= _XDEF_WAREHOUSE_MAX_SENDCOUNT)
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_WAREHOUSE_EXCESS), TRUE);		// _T("�ѹ��� �ű� �� �ִ� ������ �ʰ� �߽��ϴ�.")
		return;
	}

	if(m_bProcessOneClickItem)
	{
		list <_XWarehouseItem*>::iterator iter_item;
		for(iter_item = m_TempItemList.begin() ; iter_item != m_TempItemList.end() ; ++iter_item)
		{
			_XWarehouseItem* pWarehouseItem = *iter_item;
			if(pWarehouseItem)
			{
				if(pWarehouseItem->m_InventoryNumber == invenindex)
				{
					pWarehouseItem->Set_m_ucCount(count);
					m_bProcessOneClickItem = FALSE;
					break;
				}
			}
		}
	}
	else
	{
		_XWarehouseItem* pWarehouseItem = NULL;

		pWarehouseItem = new _XWarehouseItem;
		pWarehouseItem->Copy(g_pLocalUser->m_UserItemList[invenindex]);
		pWarehouseItem->m_InventoryNumber = invenindex;
		pWarehouseItem->m_WarehouseNumber = -1;

		if(count > 0)
		{
			pWarehouseItem->Set_m_ucCount(count);
		}

		m_TempItemList.push_back(pWarehouseItem);
	}
}

void _XWindow_Warehouse::ResetTempItemFromInven(int invenindex)
{
	list <_XWarehouseItem*>::iterator iter_item;

	for(iter_item = m_TempItemList.begin() ; iter_item != m_TempItemList.end() ; )
	{
		_XWarehouseItem* pWarehouseItem = *iter_item;
		if(pWarehouseItem)
		{
			if(pWarehouseItem->m_InventoryNumber == invenindex)
			{
				SAFE_DELETE(pWarehouseItem);
				iter_item = m_TempItemList.erase(iter_item);
			}
			else
			{
				++iter_item;
			}
		}
	}
	m_bProcessOneClickItem = FALSE;
}

void _XWindow_Warehouse::SetTempItemFromWarehouse(int slotindex, int count)
{
	m_MoveButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_SEND);	// ã��

	if(m_bWaitForServerResult)
		return;

	if(g_pLocalUser->m_WarehouseItemList[slotindex].Get_m_sID() == 0)
		return;

	if(m_SendMode == _XSENDMODE_TOINVEN_MONEY || m_SendMode == _XSENDMODE_TOWAREHOUSE_MONEY)
		return;

	if( m_TempItemList.size() >= _XDEF_WAREHOUSE_MAX_SENDCOUNT )
		return;
	
	m_SendMode = _XSENDMODE_TOINVEN_ITEM;

	BOOL	bClearAll = FALSE;
	if(!m_TempItemList.empty())
	{
		list <_XWarehouseItem*>::iterator iter_item = m_TempItemList.begin();
		_XWarehouseItem* pWarehouseItem = *iter_item;
		if(pWarehouseItem)
		{
			if(pWarehouseItem->m_InventoryNumber != -1)
			{
				bClearAll = TRUE;
			}
		}
	}
	
	if(bClearAll)
	{
		list <_XWarehouseItem*>::iterator iter_item;
		for(iter_item = m_TempItemList.begin() ; iter_item != m_TempItemList.end() ; )
		{
			_XWarehouseItem* pWarehouseItem = *iter_item;
			if(pWarehouseItem)
			{
				// ������ �κ��丮���� �� ��, clear �Ѵ�.
				SAFE_DELETE(pWarehouseItem);
				iter_item = m_TempItemList.erase(iter_item);
			}
		}
		m_TempItemList.clear();
	}

	if(m_TempItemList.size() >= _XDEF_WAREHOUSE_MAX_SENDCOUNT)
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_WAREHOUSE_EXCESS), TRUE); // �ѹ��� �ű� �� �ִ� ������ �ʰ� �߽��ϴ�.
		return;
	}

	if(m_bProcessOneClickItem)
	{
		list <_XWarehouseItem*>::iterator iter_item;
		for(iter_item = m_TempItemList.begin() ; iter_item != m_TempItemList.end() ; ++iter_item)
		{
			_XWarehouseItem* pWarehouseItem = *iter_item;
			if(pWarehouseItem)
			{
				if(pWarehouseItem->m_WarehouseNumber == slotindex)
				{
					pWarehouseItem->Set_m_ucCount(count);
					m_bProcessOneClickItem = FALSE;
					break;
				}
			}
		}
	}
	else
	{
		_XWarehouseItem* pWarehouseItem = NULL;
		
		pWarehouseItem = new _XWarehouseItem;
		pWarehouseItem->Copy(g_pLocalUser->m_WarehouseItemList[slotindex]);
		pWarehouseItem->m_WarehouseNumber = slotindex;
		pWarehouseItem->m_InventoryNumber = -1;
		
		if(count != 0)
		{
			pWarehouseItem->Set_m_ucCount(count);
		}
	
		m_TempItemList.push_back(pWarehouseItem);
	}
}

void _XWindow_Warehouse::ResetTempItemFromWarehouse(int slotindex)
{
	list <_XWarehouseItem*>::iterator iter_item;
	
	for(iter_item = m_TempItemList.begin() ; iter_item != m_TempItemList.end() ; )
	{
		_XWarehouseItem* pWarehouseItem = *iter_item;
		if(pWarehouseItem)
		{
			if(pWarehouseItem->m_WarehouseNumber == slotindex)
			{
				SAFE_DELETE(pWarehouseItem);
				iter_item = m_TempItemList.erase(iter_item);
			}
			else
			{
				iter_item++;
			}
		}
	}
	m_bProcessOneClickItem = FALSE;
}

int _XWindow_Warehouse::GetSlotIndex(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	int PosX, PosY;
	PosX = scrnpos->x;
	PosY = scrnpos->z;

	PosX -= (m_WindowPosition.x + _XDEF_WAREHOUSEITEMPOSX);
	if( PosX < 0 )
		return -1;
	PosX /= 33;	
	PosY -= (m_WindowPosition.y + _XDEF_WAREHOUSEITEMPOSY);
	if( PosY < 0 )
		return -1;
	PosY /= 33;	

	if( PosX > 4 || PosY > 3 )
		return -1;

	int jumpcount = PosY * 5 + PosX;
	if( jumpcount > _XDEF_WAREHOUSE_MAX_SENDCOUNT ) return -1;

	int startnumber = m_SelectedTab * _XDEF_WAREHOUSE_MAX_SENDCOUNT;
	jumpcount += startnumber;

	return jumpcount;
	/*
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();

	for(int i = 0 ; i < _XDEF_WAREHOUSE_MAX_SENDCOUNT ; i++)
	{
		if(scrnpos->x > m_WindowPosition.x + m_SlotPosition[i].x && scrnpos->x < m_WindowPosition.x + m_SlotPosition[i].x + 30)
		{
			if(scrnpos->z > m_WindowPosition.y + m_SlotPosition[i].y && scrnpos->z < m_WindowPosition.y + m_SlotPosition[i].y + 30)
			{
				if(g_pLocalUser->m_WarehouseItemList[i].m_sID != 0)
					return i;
			}
		}
	}
	return -1;*/
}

void _XWindow_Warehouse::DrawSelectedItem(void)
{
	if(!m_bDrawSelectedItem)	// �ٸ��������� �������� ���õǾ� ���� ��
	{
		int selectedslot = GetSlotIndex();
		if( g_pLocalUser->m_WarehouseItemList[selectedslot].Get_m_sID() > 0 )
		{
			int selectedslot2 = selectedslot % _XDEF_WAREHOUSE_MAX_SENDCOUNT;
			if(selectedslot != -1)
			{
				RECT iconrect;
				iconrect.left = m_WindowPosition.x + m_SlotPosition[selectedslot2].x;
				iconrect.top = m_WindowPosition.y + m_SlotPosition[selectedslot2].y;
				iconrect.right = m_WindowPosition.x + m_SlotPosition[selectedslot2].x + 29;
				iconrect.bottom = m_WindowPosition.y + m_SlotPosition[selectedslot2].y + 29;
				
				_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
				DrawItemTooltip(selectedslot, iconrect);
			}
		}		
		return;
	}

	// â��� �κ��� ��� ��Ƽ ������ �����Ǳ� ���� 
	BOOL		selectedslotarray[_XDEF_WAREHOUSE_MAX_SENDCOUNT];
	memset(selectedslotarray, 0, sizeof(BOOL)*_XDEF_WAREHOUSE_MAX_SENDCOUNT);

	if(m_bHaveMouseSlot)
	{
		int selectedslot = m_MouseSlotItem.m_InventoryNumber;
		int selectedslot2 = selectedslot % _XDEF_WAREHOUSE_MAX_SENDCOUNT;

		_XDrawRectAngle(m_WindowPosition.x+m_SlotPosition[selectedslot2].x, 
						m_WindowPosition.y+m_SlotPosition[selectedslot2].y, 
						m_WindowPosition.x+m_SlotPosition[selectedslot2].x+29, 
						m_WindowPosition.y+m_SlotPosition[selectedslot2].y+29, 
						1.0f, _XSC_ITEM_SELECT);
		selectedslotarray[selectedslot2] = TRUE;

		_XDrawSolidBar( m_WindowPosition.x+m_SlotPosition[selectedslot2].x+1,
						m_WindowPosition.y+m_SlotPosition[selectedslot2].y+1,
						m_WindowPosition.x+m_SlotPosition[selectedslot2].x+29, 
						m_WindowPosition.y+m_SlotPosition[selectedslot2].y+29, D3DCOLOR_ARGB(160, 0, 0, 0));
	}

	for(int i = 0 ; i < _XDEF_WAREHOUSE_MAX_SENDCOUNT ; ++i)
	{
		if( HaveItemFromWarehouse(i+m_SelectedTab*_XDEF_WAREHOUSE_MAX_SENDCOUNT) )
		{
			_XDrawRectAngle(m_WindowPosition.x+m_SlotPosition[i].x, m_WindowPosition.y+m_SlotPosition[i].y, 
							m_WindowPosition.x+m_SlotPosition[i].x+29, m_WindowPosition.y+m_SlotPosition[i].y+29, 1.0f, _XSC_ITEM_SELECT);
			selectedslotarray[i] = TRUE;
		}
	}

	// mouse hover icon
	/*if(m_LastDraggingSlot == -1)
	{
		int selectedslot = GetSlotIndex();
		int selectedslot2 = selectedslot % _XDEF_WAREHOUSE_MAX_SENDCOUNT;
		if(selectedslot != -1 && !selectedslotarray[selectedslot2])
		{
			RECT iconrect;
			iconrect.left = m_WindowPosition.x + m_SlotPosition[selectedslot2].x;
			iconrect.top = m_WindowPosition.y + m_SlotPosition[selectedslot2].y;
			iconrect.right = m_WindowPosition.x + m_SlotPosition[selectedslot2].x + 31;
			iconrect.bottom = m_WindowPosition.y + m_SlotPosition[selectedslot2].y + 31;

			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
			DrawItemTooltip(selectedslot, iconrect);
		}
	}*/
	int selectedslot = GetSlotIndex();
	if( g_pLocalUser->m_WarehouseItemList[selectedslot].Get_m_sID() > 0 )
	{
		int selectedslot2 = selectedslot % _XDEF_WAREHOUSE_MAX_SENDCOUNT;
		if(selectedslot != -1 && !selectedslotarray[selectedslot2])
		{
			RECT iconrect;
			iconrect.left = m_WindowPosition.x + m_SlotPosition[selectedslot2].x;
			iconrect.top = m_WindowPosition.y + m_SlotPosition[selectedslot2].y;
			iconrect.right = m_WindowPosition.x + m_SlotPosition[selectedslot2].x+29;
			iconrect.bottom = m_WindowPosition.y + m_SlotPosition[selectedslot2].y+29;
			
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, 1.0f, _XSC_ITEM_CLICK);
			DrawItemTooltip(selectedslot, iconrect);
		}
	}	
}

void _XWindow_Warehouse::ReleaseTempItemList(void)
{
	m_SendMode = _XSENDMODE_NONE;
	//m_bWaitForServerResult = FALSE;
	m_bProcessOneClickItem = FALSE;
	
	if( m_bWaitForServerResult ) return;	// ������ �� �� ���¿����� �ٽ� ������ �ʴ´�.
	
	if(!m_TempItemList.empty())
	{
		list <_XWarehouseItem*>::iterator iter_item;
		for(iter_item = m_TempItemList.begin() ; iter_item != m_TempItemList.end() ; )
		{
			_XWarehouseItem* pWarehouseItem = *iter_item;
			if(pWarehouseItem)
			{
				SAFE_DELETE(pWarehouseItem);
				iter_item = m_TempItemList.erase(iter_item);
			}
		}
		m_TempItemList.clear();
	}

	m_TempMoney = 0;	
}

void _XWindow_Warehouse::ItemSaveResultToWarehouse(int resultcount)
{
	int i = 0;
	list <_XWarehouseItem*>::iterator iter_item;
	for(iter_item = m_TempItemList.begin() ; iter_item != m_TempItemList.end() ; ++iter_item)
	{
		if( i >= resultcount )
			break;

		_XWarehouseItem* pWarehouseItem = *iter_item;
		if(pWarehouseItem)
		{
			//_XLog( "WareHouse Save inven number %d warehouse number %d stack %d", pWarehouseItem->m_InventoryNumber, pWarehouseItem->m_WarehouseNumber, pWarehouseItem->IsStackable() );

			if(pWarehouseItem->IsStackable())
			{
				// ���վ����� ó��
				g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber].Copy(g_pLocalUser->m_UserItemList[pWarehouseItem->m_InventoryNumber]);
				g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber].Set_m_ucCount( pWarehouseItem->m_ReceiveCount );
				g_pLocalUser->m_UserItemList[pWarehouseItem->m_InventoryNumber].Set_m_ucCount( g_pLocalUser->m_UserItemList[pWarehouseItem->m_InventoryNumber].Get_m_ucCount() - pWarehouseItem->Get_m_ucCount());
				if(g_pLocalUser->m_UserItemList[pWarehouseItem->m_InventoryNumber].Get_m_ucCount() <= 0)
				{
					g_pLocalUser->m_UserItemList[pWarehouseItem->m_InventoryNumber].Reset();
				}

			}
			else
			{
				// �Ϲ� ������ ó��
				g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber].Copy(g_pLocalUser->m_UserItemList[pWarehouseItem->m_InventoryNumber]);
				g_pLocalUser->m_UserItemList[pWarehouseItem->m_InventoryNumber].Reset();
				
			}		

			TCHAR		namestring[128];
			memset(namestring, 0, sizeof(TCHAR)*128);
			strcpy(namestring, _XGameItem::GetItemName(g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber].Get_m_cType(), 
														g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber].Get_m_sID()));
			
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
			if(pNPCScript_Window)
			{
				TCHAR messagestring[256];
				memset(messagestring, 0, sizeof(TCHAR)*256);
				
				int param2  = pWarehouseItem->m_ReceiveCount;
				// _T("â�� %s %d���� ����Ǿ����ϴ�.")
				strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_WAREHOUSE_SAVEWAREHOUSE, namestring, &param2 ) );
				pNPCScript_Window->InsertNPCChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL );
			}
		} // if(pWarehouseItem)
		i++;
	}
	ReleaseTempItemList();
	SetTerminateLastItemIndex();
}

void _XWindow_Warehouse::ItemSaveResultToInventory(int resultcount)
{
	int i = 0;
	list <_XWarehouseItem*>::iterator iter_item;
	for(iter_item = m_TempItemList.begin() ; iter_item != m_TempItemList.end() ; ++iter_item)
	{
		if( i >= resultcount )
			break;

		_XWarehouseItem* pWarehouseItem = *iter_item;
		if(pWarehouseItem)
		{
			//_XLog( "Inven Save inven number %d warehouse number %d stack %d", pWarehouseItem->m_InventoryNumber, pWarehouseItem->m_WarehouseNumber, pWarehouseItem->IsStackable() );			
			if(pWarehouseItem->IsStackable())
			{
				// ���վ����� ó��
				g_pLocalUser->m_UserItemList[pWarehouseItem->m_InventoryNumber].Copy(g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber]);
				g_pLocalUser->m_UserItemList[pWarehouseItem->m_InventoryNumber].Set_m_ucCount( pWarehouseItem->m_ReceiveCount );
				g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber].Set_m_ucCount( g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber].Get_m_ucCount() - pWarehouseItem->Get_m_ucCount());
				if(g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber].Get_m_ucCount() <= 0)
				{
					g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber].Reset();
				}				
			}
			else
			{
				// �Ϲ� ������ ó��
				g_pLocalUser->m_UserItemList[pWarehouseItem->m_InventoryNumber].Copy(g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber]);
				g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber].Reset();
				
			}			

			TCHAR		namestring[128];
			memset(namestring, 0, sizeof(TCHAR)*128);
			strcpy(namestring, _XGameItem::GetItemName(g_pLocalUser->m_UserItemList[pWarehouseItem->m_InventoryNumber].Get_m_cType(),
														g_pLocalUser->m_UserItemList[pWarehouseItem->m_InventoryNumber].Get_m_sID()));			
			
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
			if(pNPCScript_Window)
			{
				TCHAR messagestring[256];
				memset(messagestring, 0, sizeof(TCHAR)*256);

				int param2 = pWarehouseItem->m_ReceiveCount;
				// %s %d���� �೶���� �����Խ��ϴ�.
				strcpy(messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_WAREHOUSE_SAVEINVEN, namestring, &param2 ) );
				pNPCScript_Window->InsertNPCChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			}			
		} // if(pWarehouseItem)
		i++;
	}
	ReleaseTempItemList();
	SetTerminateLastItemIndex();
}

void _XWindow_Warehouse::DrawDragIcon(void)
{
//	if(g_MessageBox.m_ShowWindow) return;
	_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSAGEWINDOW);
	if(pMessageBox)
	{
		if(pMessageBox->GetShowStatus())
			return;
	}
	
	_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
	if(pTradeConfirm_Window)
	{
		if(pTradeConfirm_Window->GetShowStatus())
			return;
	}
	
	if(m_bHaveMouseSlot)
	{
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
		g_pInventory_Window->DrawIcon(TRUE, scrnpos->x - 16 , scrnpos->z - 16, (_XUserItem*)&m_MouseSlotItem, FALSE);
		
		((CEmperorOfDragonsApp*)gApp)->ReadyCursor(_XMOUSETYPE_SELECT);
	}	
}

void _XWindow_Warehouse::ProcessDblClick(void)
{
	_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
	if( pTradeConfirm_Window->GetShowStatus() )		return;

	if(g_pInventory_Window)
	{
		if(g_pInventory_Window->GetShowStatus())
		{
			if(CheckMousePosition())
			{
				if( m_bWaitForServerResult ) return;	// ������ �� �� ���¿����� �ٽ� ������ �ʴ´�.

				ReleaseTempItemList();
				m_LastDraggingSlot = GetSlotIndex();
				if(m_LastDraggingSlot != -1 && g_pLocalUser->m_WarehouseItemList[m_LastDraggingSlot].Get_m_sID() > 0 )
				{
					m_MouseSlotItem.Copy(g_pLocalUser->m_WarehouseItemList[m_LastDraggingSlot]);
					m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
					if(m_MouseSlotItem.IsStackable())
					{
						_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)m_pMyManager->FindWindow(_XDEF_WTITLE_TRADECONFIRM);
						if(pTradeConfirm_Window)
						{
							pTradeConfirm_Window->ShowWindow(TRUE);
							pTradeConfirm_Window->m_IMEControl.SetFocus();
							pTradeConfirm_Window->m_SelectedCategory1 = m_MouseSlotItem.Get_m_cType();
							pTradeConfirm_Window->m_SelectedItemID = m_MouseSlotItem.Get_m_sID();
							pTradeConfirm_Window->m_TradeMode = _XWAREHOUSE_FROMWAREHOUSE_ITEM;
						}
					}
					else
					{
						SetTempItemFromWarehouse(m_MouseSlotItem.m_InventoryNumber, 1);
						m_SendMode = _XSENDMODE_TOINVEN_ITEM;
						PostMessage(gHWnd, WM_COMMAND, _XDEF_WAREHOUSEWINDOW_SENDADDITEM, (LPARAM)gHWnd);
					}
					m_LastDraggingSlot = -1;
				}
			}
		}
	}
}

#ifdef _XTS_ITEM_OPTIMIZATION
void _XWindow_Warehouse::DrawItemTooltip(int slotnumber, RECT iconrect)
{
	char cType = g_pLocalUser->m_WarehouseItemList[slotnumber].Get_m_cType();
	short sID = g_pLocalUser->m_WarehouseItemList[slotnumber].Get_m_sID();
	unsigned char cStrength = g_pLocalUser->m_WarehouseItemList[slotnumber].Get_m_ucStrength();
	
	if( sID > 0 )
	{
		TCHAR tempbuf[512];
		memset( tempbuf, 0, sizeof(tempbuf)); //Author : ����� //breif : �������� ���� ƨ�� ����..

		switch(GetEqualItemType(cType)) 
		{
		case _XGI_FC_WEAPON:		
			{		
#ifdef _XTS_ITEM_V20
				if( cStrength > 0 )
					sprintf( tempbuf, "%s +%d", _XGameItem::GetItemFullName(cType, sID), cStrength );
				else
					strcpy( tempbuf, _XGameItem::GetItemFullName(cType, sID) );
#else
				if( cStrength > 0 )
					sprintf( tempbuf, "%s %s +%d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), cStrength );
				else
					sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
#endif
			}
			break;
		case _XGI_FC_CLOTHES:
			{
#ifdef _XTS_ITEM_V20
				strcpy( tempbuf, _XGameItem::GetItemFullName(cType, sID) );
#else
				sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
#endif
				
			}
			break;
		default:
			{
				strcpy( tempbuf, _XGameItem::GetItemName(cType, sID));
			}
			break;
		}
		g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, _XGameItem::GetGradeColor(cType, sID), TRUE);
	}
}
#else
void _XWindow_Warehouse::DrawItemTooltip(int slotnumber, RECT iconrect)
{
	char cType = g_pLocalUser->m_WarehouseItemList[slotnumber].m_cType;
	short sID = g_pLocalUser->m_WarehouseItemList[slotnumber].m_sID;
	unsigned char cStrength = g_pLocalUser->m_WarehouseItemList[slotnumber].m_ucStrength;
	
	if( sID > 0 )
	{
		TCHAR tempbuf[64];
		switch(cType) 
		{
		case _XGI_FC_WEAPON:
		case _XGI_FC_WEAPON2:	
		case _XGI_FC_WEAPON3:
			{				
				if( cStrength > 0 )
					sprintf( tempbuf, "%s %s +%d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), cStrength );
				else
					sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
			}
			break;
		case _XGI_FC_CLOTHES:		
		case _XGI_FC_CLOTHES2:
		case _XGI_FC_CLOTHES3:
			{
				sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
				
			}
			break;
		default:
			{
				strcpy( tempbuf, _XGameItem::GetItemName(cType, sID));
			}
			break;
		}
		g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, _XGameItem::GetGradeColor(cType, sID), TRUE);
	}
}
#endif

void _XWindow_Warehouse::SetTerminateLastItemIndex()
{
	for( int i = 0; i < _XDEF_WAREHOUSE_TOTALCOUNT; ++i )
	{
		if( g_pLocalUser->m_WarehouseItemList[i].Get_m_sID() > 0 )
		{
			m_TerminateLastItemIndex = i;
		}
	}
}


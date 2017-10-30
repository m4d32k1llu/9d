// XWindow_RecallItem.cpp: implementation of the XWindow_RecallItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XProc_ProcessDefine.h"
#include "xsr_stringheader.h"

#include "XWindow_Messenger.h"
#include "XMessengerGroup.h"
#include "XMessengerFriend.h"

#include "XWindow_SystemMsgDefault.h"
#include "XWindow_BloodPoint.h"
#include "XWindow_MnDTotalPoint.h" //Author : ����� //breif :���� �ý���
#include "XWindow_MnDCurrentPoint.h" //Author : ����� //breif :���� �ý���

#include "XWindow_Escape.h"

#include "XWindow_RecallItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static int _XDEF_INVENTORYITEMPOSX = 11;
static int _XDEF_INVENTORYITEMPOSY = 71;

static int _XDEF_INVENTORYSLOT_WIDTH = 5;
static int _XDEF_INVENTORYSLOT_HEIGHT = 5;

_XWindow_RecallItem::_XWindow_RecallItem() : m_listboxGroupList(NULL), m_nRecallItemIndex(0), m_nSelectedItem(0), m_nCharState(0), m_bMoving(FALSE), m_pMoveButton(NULL), m_pCallButton(NULL)
{

}

_XWindow_RecallItem::~_XWindow_RecallItem()
{

}

BOOL _XWindow_RecallItem::Initialize()
{
	int mainbackresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	int messengerresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	int messengerresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" );

	// �޽��� Ÿ��Ʋ ��	( 183 X 18 )
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 183, 67, &g_MainInterfaceTextureArchive, mainbackresourceindex1 );
	pTitleBar->SetClipRect(0, 125, 183, 192);
	pTitleBar->ShowWindow(TRUE);
	InsertChildObject( pTitleBar );

	// �޽��� ���� ���̵� �̹��� ( 10 X 158 )
	_XImageStatic* pLeftSideBorder = new _XImageStatic;
	pLeftSideBorder->Create( 0, 55, 10, 213, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	pLeftSideBorder->SetClipRect(226, 0, 236, 158);
	pLeftSideBorder->ShowWindow(TRUE);
	InsertChildObject( pLeftSideBorder );

	// �޽��� ������ ���̵� �̹��� ( 20 X 158 )
	_XImageStatic* pRightSideBorder = new _XImageStatic;
	pRightSideBorder->Create( 163, 55, 183, 213, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	pRightSideBorder->SetClipRect(236, 0, 256, 158);
	pRightSideBorder->ShowWindow(TRUE);
	InsertChildObject( pRightSideBorder );
		
	// �޽��� â �Ʒ���  ����	 ( 183 X 52 )
	_XImageStatic* pDownBorder = new _XImageStatic;
	pDownBorder->Create( 0, 213, 183, 265, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	pDownBorder->SetClipRect( 0, 26, 183, 78);
	pDownBorder->ShowWindow(TRUE);
	InsertChildObject( pDownBorder );
		
	// �޽��� â �Ʒ��� �� ����	 ( 183 X 3 )
	_XImageStatic* pDownEndLine = new _XImageStatic;
	pDownEndLine->Create( 0, 266, 183, 269, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	pDownEndLine->SetClipRect( 0, 106, 183, 109);
	pDownEndLine->ShowWindow(TRUE);
	InsertChildObject( pDownEndLine );

	//Ŭ���� ��ư
	_XBTN_STRUCTURE closeminibtnstruct = 
	{ 
		TRUE,
		{ m_WindowSize.cx - 14, 2 }, 
		{ 12, 12 }, 
		_XDEF_RECALLITEM_MINICLOSEBUTTON,
		g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName),
		g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName),
		g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName), 
		&g_MainInterfaceTextureArchive };

	_XButton* pCloseMinibutton = new _XButton;
	pCloseMinibutton->Create( closeminibtnstruct );
	pCloseMinibutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 96,  238, 107, 249 );
	pCloseMinibutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 119, 249 );
	pCloseMinibutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 131, 249 );
	InsertChildObject( pCloseMinibutton );


	// �޽��� ��ư ( 60 X 24 )
	_XBTN_STRUCTURE callbtnstruct = 
	{
		TRUE, {21, 215}, {60, 24}, _XDEF_RECALLITEM_CALLBUTTON,
			messengerresourceindex1, messengerresourceindex1, messengerresourceindex1,
			&g_MainInterfaceTextureArchive
	};
	
	m_pCallButton = new _XButton;
	m_pCallButton->Create(callbtnstruct );
	m_pCallButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	m_pCallButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	m_pCallButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	m_pCallButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3489 );	// _T("ȣ��")
	m_pCallButton->ShowWindow(TRUE);
	InsertChildObject(m_pCallButton);

	// �޽��� ��ư ( 60 X 24 )
	_XBTN_STRUCTURE movebtnstruct = 
	{
		TRUE, {100, 215}, {60, 24}, _XDEF_RECALLITEM_MOVEBUTTON,
			messengerresourceindex1, messengerresourceindex1, messengerresourceindex1,
			&g_MainInterfaceTextureArchive
	};
	
	m_pMoveButton = new _XButton;
	m_pMoveButton->Create(movebtnstruct);
	m_pMoveButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	m_pMoveButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	m_pMoveButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	m_pMoveButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3490 );	// _T("�̵�")
	m_pMoveButton->ShowWindow(TRUE);
	InsertChildObject(m_pMoveButton);

	// �޽��� ��ư ( 60 X 24 )
	_XBTN_STRUCTURE closebtnstruct = 
	{
		TRUE, {62, 242}, {60, 24}, _XDEF_RECALLITEM_CLOSEBUTTON,
			messengerresourceindex1, messengerresourceindex1, messengerresourceindex1,
			&g_MainInterfaceTextureArchive
	};
	
	_XButton* pCloseButton = new _XButton;
	pCloseButton->Create(closebtnstruct);
	pCloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 194, 249, 218);
	pCloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 167, 249, 191);
	pCloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 221, 249, 245);
	pCloseButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CLOSE );	// _T("�ݱ�")
	pCloseButton->ShowWindow(TRUE);
	InsertChildObject(pCloseButton);

	int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );

	//�޽��� �׷�� ģ������Ʈ�� ǥ���ϴ� ����Ʈ �ڽ� 
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,											// ������ Ȱ��ȭ
		{ 8, 48  },										// ������ ��ǥ
		{ 168, 160 },									// ������ ������
		_XDEF_RECALLITEM_LISTBOX,						// ������ command ���̵�
		-1,												// ������ border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(""),											// ������ Ÿ��Ʋ
		16,												// ����Ʈ ������ ���� ��ġ
		10,												// ����Ʈ �ڽ��� ���̴� �ִ� ���μ�
		_XLSTYLE_RIGHTSCROLLBAR,						// ��ũ�ѹ��� ��ġ

		// scroll bar property
		{ 12, 160 },									// ��ũ�ѹ� ������
		12,												// ��ũ�ѹ� ���� ��ư ������
		22,												// ��ũ�ѹ� Ʈ���� ������
		0,												// ��ü ����Ʈ ����
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex
	};

	m_listboxGroupList = new _XListBox;
	m_listboxGroupList->Create(listboxstructure);

	_XVScrollBar* pScrollBar = m_listboxGroupList->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}
	
	POINT coloffset1 = { 15, 3 };
	POINT coloffset2 = { 25, 3 };
	POINT coloffset3 = { 2, 3 }; 
	POINT coloffset4 = { 12, 3 };
	m_listboxGroupList->SetColumnOffset(0, coloffset1);				// column offset
	m_listboxGroupList->SetColumnOffset(1, coloffset2);
	m_listboxGroupList->SetColumnOffset(2, coloffset3);				
	m_listboxGroupList->SetColumnOffset(3, coloffset4);
	
	m_listboxGroupList->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style 
	m_listboxGroupList->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_listboxGroupList->SetColumnStyle(2, _XLCOLUMNSTYLE_ICONRECT);		// column style 
	m_listboxGroupList->SetColumnStyle(3, _XLCOLUMNSTYLE_ICONRECT);
	
	m_listboxGroupList->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxGroupList->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_listboxGroupList->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxGroupList->SetAlignType(3, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxGroupList->SetColumnWidth(0, 155);						// column width (column offset�� ���� �ʰų� �����ص� �������)
	m_listboxGroupList->SetColumnWidth(1, 155 );
	m_listboxGroupList->SetColumnWidth(2, 15);						// column width (column offset�� ���� �ʰų� �����ص� �������)
	m_listboxGroupList->SetColumnWidth(3, 25 );
	
	m_listboxGroupList->SetSelectedImageArchive(-1, &g_MainInterfaceTextureArchive);
	m_listboxGroupList->SetSelectedImagePosition(0, 1);
	m_listboxGroupList->SetSelectedImageHeight(16);
	m_listboxGroupList->SetTrackWidth(155);
	
	m_listboxGroupList->SetScrollPos(0);
	
	InsertChildObject(m_listboxGroupList);

	m_listboxGroupList->SetAlwaysShowScrollBarMode(TRUE);
	//m_listboxGroupList->SetDrawBorder( TRUE );
	//m_listboxGroupList->SetBorderColorFactor( D3DCOLOR_ARGB( 160, 0, 0, 0 ) );

	// �޽��� ����Ʈ���� �巡�� �Ǵ� ģ�� �̸��̳� �׷� �̸��� ǥ����
	_XTOOLTIP_STRUCTURE draginglistboxstruct = 
	{
		{ 0, 0 },
		{ 140, 15 },
		{ 1, 1 },
		-1,
		NULL
	};
	/*m_DragingDisplayBox = new _XToolTip;
	m_DragingDisplayBox->Create(draginglistboxstruct);
	m_DragingDisplayBox->ShowWindow(FALSE);*/

	return TRUE;
}

void _XWindow_RecallItem::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_RecallItem::MoveWindow( int X, int Y )
{			
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_RecallItem::ShowWindow( BOOL show )
{
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
	_XWindow_SystemMsgDefault* systemWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);

	if( show )
	{
		SetListItem();
		
		systemWindow->MoveWindow(m_WindowPosition.x - 374, gnHeight-195);
		systemWindow->m_SystemMsgListScrollBar->MoveWindow(m_WindowPosition.x - 13,gnHeight - 182);

	#ifdef _XTS_BLOODPOINT
		// system msg â ������
		_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
		if(pBloodPoint_Window)
		{
			if(systemWindow->GetShowStatus())
				pBloodPoint_Window->MoveWindow(m_WindowPosition.x - 60, gnHeight - 120 - 96 );
			else
				pBloodPoint_Window->MoveWindow(m_WindowPosition.x - 60, gnHeight - 99 - 96 );
		}
	#endif
	
	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
		_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = (_XWindow_MnDCurrentPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDCURRENTPOINTWINDOW);
		if(pMnDCurrentPoint_Window)
		{
			pMnDCurrentPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 19 );
		}
		
		_XWindow_MnDTotalPoint* pMnDTotalPoint_Window = (_XWindow_MnDTotalPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDTOTALPOINTWINDOW);
		if(pMnDTotalPoint_Window)
		{
			pMnDTotalPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDCurrentPoint_Window->GetWindowPos().y - 19 );
		}
	#endif
	}
	else
	{
		m_listboxGroupList->DeleteAllItem();
		
		if( g_pInventory_Window->GetShowStatus() ||
			(pMessenger_Window->GetShowStatus() &&pMessenger_Window->m_bDockingMode) )
			 systemWindow->SetDockingMode(TRUE);	
		 else
			 systemWindow->SetDockingMode(FALSE);
	}

	m_nSelectedItem = 0;

	_XWindow::ShowWindow( show );
}

void _XWindow_RecallItem::Draw( _XGUIObject*& pfocusobject )
{
	//�׸��� ����
	if( !this->m_ShowWindow ) return;

	_XWindow::Draw(pfocusobject);

	m_listboxGroupList->Draw(pfocusobject);

	POINT listwinpos;
	SIZE listwinsize;
	
	listwinpos = m_listboxGroupList->GetWindowPos();
	listwinsize = m_listboxGroupList->GetWindowSize();
	
	//���� ������ �׸���
	_XDrawRectAngle( listwinpos.x, listwinpos.y+1,
		listwinpos.x+listwinsize.cx-13, listwinpos.y+listwinsize.cy+1, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0) );
}

BOOL _XWindow_RecallItem::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	//�ý��� �޽��� â�� ��ŷüũ�� ����
	_XWindow_SystemMsgDefault* systemWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);

	RECT curwindowrect = { m_WindowPosition.x, m_WindowPosition.y, 
						   m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy };

	//���� ���� �ϴ� �𼭸� ������ ��ǥ�� üũ�Ѵ�
	 if( curwindowrect.left >= gnWidth - 557 && curwindowrect.bottom >= gnHeight-195 ) 
	 {
		 systemWindow->MoveWindow(m_WindowPosition.x - 374, gnHeight-195);
		 systemWindow->m_SystemMsgListScrollBar->MoveWindow(m_WindowPosition.x - 13,gnHeight - 182);

#ifdef _XTS_BLOODPOINT
		// system msg â ������
		_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
		if(pBloodPoint_Window)
		{
			if(systemWindow->GetShowStatus())
				pBloodPoint_Window->MoveWindow(m_WindowPosition.x - 60, gnHeight - 120 - 96 );
			else
				pBloodPoint_Window->MoveWindow(m_WindowPosition.x - 60, gnHeight - 99 - 96 );
		}
#endif

	 #ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
		_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = (_XWindow_MnDCurrentPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDCURRENTPOINTWINDOW);
		if(pMnDCurrentPoint_Window)
		{
			pMnDCurrentPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 19 );
		}
		
		_XWindow_MnDTotalPoint* pMnDTotalPoint_Window = (_XWindow_MnDTotalPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDTOTALPOINTWINDOW);
		if(pMnDTotalPoint_Window)
		{
			pMnDTotalPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDCurrentPoint_Window->GetWindowPos().y - 19 );
		}
	 #endif
	 }
	 else
	 {
		 // �κ��丮�� �޽��� �����찡 �׷����� ������ ��ŷ
		 if( g_pInventory_Window->GetShowStatus() ||
			pMessenger_Window->GetShowStatus() && pMessenger_Window->m_bDockingMode )
			 systemWindow->SetDockingMode(TRUE);	
		 else
			 systemWindow->SetDockingMode(FALSE);	
	 }

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

BOOL _XWindow_RecallItem::SearchSlotItem( int nIndex )
{
	int count			= _XINVENTORY_SLOTTYPE_POCKETSTART; //ó���� ����
	int columnpoint		= g_pInventory_Window->GetWindowPos().x + _XDEF_INVENTORYITEMPOSX;
	int rowpoint		= g_pInventory_Window->GetWindowPos().y + _XDEF_INVENTORYITEMPOSY;	

	// �ָӴ� ����Ʈ �˻�
	count = _XINVENTORY_SLOTTYPE_POCKETSTART; //�κ��� ���� ó�� ������ġ (����ĭ �� 25 )

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
    for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == _XGI_FC_POTION)
		{
			if(g_PotionItemProperty[g_pLocalUser->m_UserItemList[i].Get_m_sID()].cSecondType == nIndex)
			{
				m_nRecallItemIndex = i;
				return TRUE;
			}
		}				
	}
				
    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == _XGI_FC_POTION)
		{
			if(g_PotionItemProperty[g_pLocalUser->m_UserItemList[i].Get_m_sID()].cSecondType == nIndex)
			{
				m_nRecallItemIndex = i;
				return TRUE;
			}
		}		
	}
#else
	//�κ��丮 ��ü �˻�
	for( int row = 0; row < _XDEF_INVENTORYSLOT_HEIGHT; row++ )
	{
		for( int column = 0; column < _XDEF_INVENTORYSLOT_WIDTH; column++ )
		{
			if( count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT ) break;

			// second type == 14�� ã���� ��ģ��
//			if( g_pLocalUser->m_UserItemList[count].m_sID == nIndex )
			if(g_pLocalUser->m_UserItemList[count].Get_m_cType() == _XGI_FC_POTION)
			{
				if(g_PotionItemProperty[g_pLocalUser->m_UserItemList[count].Get_m_sID()].cSecondType == nIndex)
				{
					m_nRecallItemIndex = count;
					return TRUE;
				}
			}
			columnpoint+=33;
			count++;			
		}

		if( count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT ) break;
		
		columnpoint = g_pInventory_Window->GetWindowPos().x + _XDEF_INVENTORYITEMPOSX;
		rowpoint += 33;

		//������ �� ���Ҵ�... �ָӴϸ� �ٲ��ְ� �ٽ� �˻縦 �Ѵ�
		if( count == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART || count == _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART)
		{
			columnpoint = g_pInventory_Window->GetWindowPos().x + _XDEF_INVENTORYITEMPOSX;
			rowpoint = g_pInventory_Window->GetWindowPos().y + _XDEF_INVENTORYITEMPOSY;

			row = -1;	//�ʱ�ȭ row++ ���ֱ⶧��
			column = 0; 
		}
	}
#endif

	return FALSE;
}

BOOL _XWindow_RecallItem::CharRecallCondition() //���� ȣ���Ҷ� ���� ���� üũ
{
	_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);	
	TCHAR tempString[256];
	memset(tempString, 0, sizeof(tempString));
    int areanumber = 0;

	CEmperorOfDragonsApp* pApp = ((CEmperorOfDragonsApp*)gApp);

	/*if( g_CurrentZoneInfoIndex == 21 || //�пյ�
		g_pLocalUser->m_bIsBossBattle || //�� ������
		g_pLocalUser->m_bCastleBattle || //�����������
		g_pLocalUser->m_bOut || //�ڸ����
		g_pLocalUser->m_bMatchPRMode || // ��ȫ��
		g_pLocalUser->m_CharacterLog.isTrade || //��ȯ��
#ifdef _XTS_PK
		g_pLocalUser->m_BattleMode != _XBATTLE_PEACE || //��������
#else
		g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE || 
#endif
#ifdef _XTS_PK
		g_pLocalUser->m_BattleMode == _XBATTLE_PK || // ������
#endif
		( g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
		(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST) ) || // �����ɹ� ������
		g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING || //������
		(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT) || //����
		g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE || //���λ�����
		( g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL && 
		g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 ) ||//������
		pEscape_Window->GetShowStatus() ) //Ż�� 
	{
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}*/

	if( g_CurrentZoneInfoIndex == 21 )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3652) );//_T("�пյ������� ȣ��/�̵��� �� �����ϴ�.") );
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}
	else if( g_pLocalUser->m_bIsBossBattle )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3653 ) );//_T("�� ���� �߿��� ȣ��/�̵��� �� �����ϴ�.") );
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}
	else if( g_pLocalUser->m_bCastleBattle )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3654) );//_T("����� ���� �߿��� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}
    else if( g_AreaManager.CheckInsideArea(g_pLocalUser->m_Position, _XDEF_AREACATEGORY_CASTLE, areanumber) )
    {
        sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3654) );//_T("����� ���� �߿��� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
    }
	else if( g_pLocalUser->m_bOut )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3655) );//_T("�ڸ���� ���¿����� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}
	else if( g_pLocalUser->m_bMatchPRMode )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3656) );//_T("��ȫ�� �߿��� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}
	else if( g_pLocalUser->m_CharacterLog.isTrade )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3657) );//_T("��ȯ �߿��� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}
/*#ifdef _XTS_PK
	else if( g_pLocalUser->m_BattleMode != _XBATTLE_PEACE ) //��������
#else
	else if( g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE )
#endif
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3831)); //"��ȭ ���¿����� ȣ��/�̵��� ���� �մϴ�.
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}*/
#ifdef _XTS_PK
	else if( g_pLocalUser->m_BattleMode == _XBATTLE_PK ) // ������
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3658) );//_T("���� �߿��� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}
#endif
	else if( ( g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
		(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST) ) ) //�����ɹ�
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3659) );//_T("�����ɹ� ���� �߿��� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}
	else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3660) );//_T("���� �߿��� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}
	else if( (g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT) )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3661) );//_T("�� �߿��� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}
	else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3662) );//_T("���λ��� ���� �߿��� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}
	/*else if( ( g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL && 
		g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 ) )
	{
		sprintf(tempString, _T("��� �߿��� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}*/
	else if( pEscape_Window->GetShowStatus() )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3663) );//_T("Ż�� �߿��� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}
	else if( pApp->m_proc_maingame.m_bNPCDialogMode || pApp->m_proc_maingame.m_bNPCDialogWideView )
	{
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3664) );//_T("NPC ��ȭ �߿��� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
    else if(g_pLocalUser->GetEnterMatchEvent())
    {
		sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3661) );//_T("�� �߿��� ȣ��/�̵��� �� �����ϴ�."));
		g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		m_nCharState = 1; //�����Ҷ��� ���δ�
		return FALSE;
	}           
#endif

	m_nCharState = 0; //�����Ҷ��� ���δ�

	return TRUE;
}

//ģ����Ͽ� �ɸ����� �̸��� ���� �ϴ���..
BOOL _XWindow_RecallItem::CheckCharacterName( LPCTSTR pString )
{
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);

	//�׷� �˻� �ϸ鼭.. ģ�� �̸��� ����
	if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
	{
		pMessenger_Window->m_MessengerGroupList.resetList();
		_XMessengerGroup* currentgroup = NULL;
		do 
		{
			currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
			if(currentgroup)
			{
				if(!currentgroup->m_FriendList.listEmpty())
				{
					currentgroup->m_FriendList.resetList();
					_XMessengerFriend* currentfriend = NULL;
					do 
					{
						currentfriend = (_XMessengerFriend*)currentgroup->m_FriendList.currentItem();
						if(!stricmp( currentfriend->m_FriendName, pString ))
							return TRUE;

						currentgroup->m_FriendList.nextItem();							
					} while(!currentgroup->m_FriendList.atHeadOfList());
				}
			}
			pMessenger_Window->m_MessengerGroupList.nextItem();
		} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());
	}

	return FALSE;
}

LPCTSTR	_XWindow_RecallItem::GetCharacterName( int nIndex )
{
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);

	//�׷� �˻� �ϸ鼭.. ģ�� �̸��� ����
	if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
	{
		pMessenger_Window->m_MessengerGroupList.resetList();
		_XMessengerGroup* currentgroup = NULL;
		do 
		{
			currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
			if(currentgroup)
			{
				if(!currentgroup->m_FriendList.listEmpty())
				{
					currentgroup->m_FriendList.resetList();
					_XMessengerFriend* currentfriend = NULL;
					do 
					{
						currentfriend = (_XMessengerFriend*)currentgroup->m_FriendList.currentItem();
						if( currentfriend->m_FriendNumber == nIndex )
							return currentfriend->m_FriendName;

						currentgroup->m_FriendList.nextItem();							
					} while(!currentgroup->m_FriendList.atHeadOfList());
				}
			}
			pMessenger_Window->m_MessengerGroupList.nextItem();
		} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());
	}

	return NULL;
}

void _XWindow_RecallItem::SetListItem()
{
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);

	TCHAR	groupnamebuffer[256];
	TCHAR	friendnamebuffer[256];
	TCHAR	friendnumberbuffer[64];
	TCHAR	friendstatusbuffer[128];
	
	
	memset(groupnamebuffer, 0, sizeof(groupnamebuffer));
	memset(friendnamebuffer, 0, sizeof(friendnamebuffer));
	memset(friendnumberbuffer, 0, sizeof(friendnumberbuffer));
	memset(friendstatusbuffer, 0, sizeof(friendstatusbuffer));
	
	m_listboxGroupList->DeleteAllItem();
	m_listboxGroupList->SetScrollPos(0);

	int count = 0 ;

	if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
	{
		pMessenger_Window->m_MessengerGroupList.resetList();
		_XMessengerGroup* currentgroup = NULL;
		do 
		{
			currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
			if(currentgroup)
			{
				int messengerresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );

				/*int friendnumber = currentgroup->m_FriendList.getitemcount();
				sprintf( friendnumberbuffer, " (%d)",friendnumber);				

				sprintf( groupnamebuffer, currentgroup->m_GroupName ); 
				strcat( groupnamebuffer, friendnumberbuffer );
				
				RECT rect = { 45, 186, 56, 197};
				if( !currentgroup->m_bSelected )
				{
					rect.left = 67;
					rect.right = 78;
				}
												
				m_listboxGroupList->InsertListItem(_T(""), 0, count+1);		
				
				m_listboxGroupList->SetIconArchive(count, &g_MainInterfaceTextureArchive);
				
				m_listboxGroupList->SetItemText(count , 0, groupnamebuffer);
				m_listboxGroupList->SetItemAttrib(count, 0, D3DCOLOR_ARGB( 255, 219, 196, 136 ));
				m_listboxGroupList->SetItemAttribHighlight(count, 0, D3DCOLOR_ARGB(255, 255, 255, 0));
				
				m_listboxGroupList->SetItemText(count , 1, _T(""));
				m_listboxGroupList->SetItemAttrib(count, 1, D3DCOLOR_ARGB( 255, 219, 196, 136 ));
				m_listboxGroupList->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 0));
				
				m_listboxGroupList->SetItemAttrib(count, 2, messengerresourceindex1, rect);
				m_listboxGroupList->SetItemAttrib(count, 3, -1 , rect);*/
				
				//currentgroup->m_GroupNumber = count; // ����Ʈ �ڽ��� ���ؽ� ���� ����ȭ
				//count++;

				//if( currentgroup->m_bSelected )
				{
					if(!currentgroup->m_FriendList.listEmpty())
					{
						currentgroup->m_FriendList.resetList();
						_XMessengerFriend* currentfriend = NULL;
						do 
						{
							currentfriend = (_XMessengerFriend*)currentgroup->m_FriendList.currentItem();
							if(currentfriend)
							{
								if(currentfriend->m_FriendName[0] != 0 )
								{
									sprintf( friendnamebuffer, currentfriend->m_FriendName ); 
									sprintf( friendstatusbuffer, "(%s)", g_UserStatus[currentfriend->m_FriendState]);				
									
									strcat( friendnamebuffer, friendstatusbuffer );
									
									RECT friendrect = { 0, 0, 0, 0 };
									switch( currentfriend->m_FriendState ) {
									case 0:	//�α� ����
										{
											friendrect.left = 35;
											friendrect.top	= 215;
											friendrect.right = 46;
											friendrect.bottom = 226;
										}
										break;
									case 1: // �¶���
										{
											friendrect.left = 46;
											friendrect.top	= 204;
											friendrect.right = 57;
											friendrect.bottom = 215;
										}
										break;
									case 2: // ���� �� 
										{
											friendrect.left = 35;
											friendrect.top	= 204;
											friendrect.right = 46;
											friendrect.bottom = 215;
										}
										break;
									case 3: // ���� �� 
										{
											friendrect.left = 46;
											friendrect.top	= 215;
											friendrect.right = 57;
											friendrect.bottom = 226;
										}
										break;
									case 4: // �����̵� ��
										{
											friendrect.left = 46;
											friendrect.top	= 215;
											friendrect.right = 57;
											friendrect.bottom = 226;
										}
										break;
									}

									m_listboxGroupList->InsertListItem(_T(""), 0, count+1);
									
									m_listboxGroupList->SetIconArchive(count, &g_MainInterfaceTextureArchive);

									m_listboxGroupList->SetItemText(count , 0,  _T(" "));
									m_listboxGroupList->SetItemAttrib(count, 0, D3DCOLOR_ARGB(255, 189, 188, 175));
									m_listboxGroupList->SetItemAttribHighlight(count, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
									
									m_listboxGroupList->SetItemText(count , 1, friendnamebuffer);
									m_listboxGroupList->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 189, 188, 175));
									m_listboxGroupList->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
									
									m_listboxGroupList->SetItemAttrib(count, 2, -1 , friendrect);
									m_listboxGroupList->SetItemAttrib(count, 3, messengerresourceindex1, friendrect);

									//ģ���ѹ��� �ϳ��� ���Ͽ� �����ϵ��� �Ѵ�..
									count++;
									currentfriend->m_FriendNumber = count;	
								}								
							}
							currentgroup->m_FriendList.nextItem();							
						} while(!currentgroup->m_FriendList.atHeadOfList());						
					}					
				}
			}
			pMessenger_Window->m_MessengerGroupList.nextItem();
		} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());
	}
}

void _XWindow_RecallItem::Update()
{
	//Process �� ������ ó��

	//�̵��� �Ϸ��� �ϴ°� ??
	if( m_bMoving )
	{
		//����Ʈ�� ���� �Ǿ��ٸ�..
		if( NULL == g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance(m_nEffectID) )
		{
			m_bMoving = FALSE;

			TCHAR tempString[128];
			memset(tempString, 0, sizeof(tempString));

			switch( m_Recallresult.ucFriendMode )
			{
			case en_rf_recv_answer_yes : //��û ���� ��Ŷ�� ���� //������� �������� ����Ǿ�����..
				{
					MSG_RF_RECV_ANSWER_YES* tempresult = (MSG_RF_RECV_ANSWER_YES*)&m_Recallresult;

					sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3541), tempresult->cTargetName ); //%s�Բ��� ��ȯ�Ǿ����ϴ�.
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );

					g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Set_m_ucCount(tempresult->ucLeftCount); //������ ���Կ�.. ���������� ����

					memset(tempString, 0, sizeof(tempString));
					sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3542) );//��ȯ�� 1���� �Ҹ� �Ǿ����ϴ�
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );

					//0��
					if( g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Get_m_ucCount() < 1 )
						g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Reset();
				}
				break;
			case en_rf_recv_goto_answer_yes : //�̵��Ͽ� ���� //�̵��Ͽ� ���� ����Ʈ�� ����Ǿ��ٸ�..
				{
					MSG_RF_RECV_GOTO_ANSWER_YES* tempresult = (MSG_RF_RECV_GOTO_ANSWER_YES*)&m_Recallresult;

					g_pLocalUser->m_TargetPosition.x = tempresult->fTargetX + 1;
					g_pLocalUser->m_TargetPosition.z = tempresult->fTargetZ + 1;

					g_pLocalUser->m_Position.x = tempresult->fTargetX;
					g_pLocalUser->m_Position.z = tempresult->fTargetZ;
					g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight(g_pLocalUser->m_Position, g_pLocalUser->m_pGoundCollideOBB);

					sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3546), tempresult->cTargetName ); //"%s�Կ��� �̵��Ǿ����ϴ�."
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );

					g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Set_m_ucCount(tempresult->ucLeftCount); //������ ���Կ�.. ���������� ����

					memset(tempString, 0, sizeof(tempString));
					sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3542) );//��ȯ�� 1���� �Ҹ� �Ǿ����ϴ�
					g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET );

					//0��
					if( g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Get_m_ucCount() < 1 )
						g_pLocalUser->m_UserItemList[tempresult->ucItemSlot].Reset();
				}
				break;
			}
		}
	}

}

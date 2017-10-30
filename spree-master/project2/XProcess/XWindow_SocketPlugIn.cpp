// XWindow_SocketPlugIn.cpp: implementation of the _XWindow_SocketPlugIn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XMessageWindow.h"
#include "XWindow_SocketPlugIn.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_SocketPlugIn::_XWindow_SocketPlugIn()
{
	m_SocketPlugInButton	= NULL;	
	m_pApplyCancleButton	= NULL;
	m_pClosebutton			= NULL;

	m_MouseSlotItem.ResetMouseSlotItem();
	m_TempSaveSlotItem.ResetMouseSlotItem();	
	m_bHaveMouseSlot		= FALSE;	
	m_IconDragStartPos.x	= m_IconDragStartPos.y = 0;	
	m_bDragState			= FALSE;
	m_bDrawSelectedItem		= FALSE;	
	m_LastDraggingSlot		= -1;
	m_pSocketitemListBox	= NULL;
	m_bChangeButtonMode		= FALSE;
	m_MsgSocketPuluInOpen	= FALSE;

	m_IndependentItem		= NULL;
	m_ListBoxTextMode		= _BOXTEXTMODE_READY;	
	for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT; i++ )
		m_SocketSlot[i] = NULL;
}

_XWindow_SocketPlugIn::~_XWindow_SocketPlugIn()
{

}

BOOL _XWindow_SocketPlugIn::Initialize( void )
{
	int inventoryimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );

	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 184, 71, &g_MainInterfaceTextureArchive, inventoryimageindex );	
	pTitleBar->SetClipRect( 0, 125, 183, 195 );
	InsertChildObject( pTitleBar );

	_XImageStatic* pBackBoard = new _XImageStatic;
	pBackBoard->Create( 0, 60, 184, 271, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" ) );	
	pBackBoard->SetClipRect( 0, 1, 183, 211 );
	InsertChildObject( pBackBoard );

	_XImageStatic* pSocketItemView = new _XImageStatic;
	pSocketItemView->Create( 7, 26, 176, 141, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	pSocketItemView->SetClipRect( 4, 140, 172, 254 );
	InsertChildObject( pSocketItemView );
	
	// ��Ĺ� ���� ----------------------------------------------------------------------------------------------------=
	int socketslotimg = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	m_SocketSlot[0] = new _XImageStatic;
	m_SocketSlot[0]->Create( 105, 70, 137, 102, &g_MainInterfaceTextureArchive, socketslotimg );
	m_SocketSlot[0]->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( m_SocketSlot[0] );
	m_SocketSlot[1] = new _XImageStatic;
	m_SocketSlot[1]->Create( 140, 70, 172, 102, &g_MainInterfaceTextureArchive, socketslotimg );
	m_SocketSlot[1]->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( m_SocketSlot[1] );

	m_SocketSlot[2] = new _XImageStatic;
	m_SocketSlot[2]->Create( 105, 105, 137, 137, &g_MainInterfaceTextureArchive, socketslotimg );
	m_SocketSlot[2]->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( m_SocketSlot[2] );
	m_SocketSlot[3] = new _XImageStatic;
	m_SocketSlot[3]->Create( 140, 105, 172, 137, &g_MainInterfaceTextureArchive, socketslotimg );
	m_SocketSlot[3]->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( m_SocketSlot[3] );
	// ----------------------------------------------------------------------------------------------------------------=

	// ��Ĺ� �Ӽ� ����Ʈ�ڽ� -----------------------------------------------------------------------------------------=	
	int scrollbarresourceindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,								// ������ Ȱ��ȭ
		{ 9,   144 },						// ������ ��ǥ
		{ 165, 92 },						// ������ ������
		NULL,								// ������ ���̵�
		-1,									// ������ border �̹��� �ε���
		&g_MainInterfaceTextureArchive,		// Texture ����Ʈ
		_T(" "),							// ������ Ÿ��Ʋ
		14,									// ����Ʈ ������ ���� ��ġ
		6,									// ����Ʈ �ڽ��� ���̴� �ִ� ���μ�
		_XLSTYLE_RIGHTSCROLLBAR,			// ��ũ�ѹ��� ��ġ
		
		// Scroll bar property
		{13, 92},
		12, 
		22,
		20,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex
	};
	
	m_pSocketitemListBox = new _XListBox;
	m_pSocketitemListBox->Create(listboxstructure);
	m_pSocketitemListBox->SetBorderColorFactor( 0x80000000 );
	
	POINT coloffset = { 5, 5 };
	m_pSocketitemListBox->SetColumnOffset(0, coloffset);
	m_pSocketitemListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_pSocketitemListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_pSocketitemListBox->SetColumnWidth(0, 160 );
	m_pSocketitemListBox->SetTrackWidth(160);
	m_pSocketitemListBox->SetTrackHeight(92);
	m_pSocketitemListBox->SetTrackBarColor( D3DCOLOR_ARGB(128,0,0,0) );	
	m_pSocketitemListBox->EnableSelectItem(FALSE);		
	
	_XVScrollBar* pScrollBar = m_pSocketitemListBox->GetScrollBarObject();
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
	
	InsertChildObject(m_pSocketitemListBox);	
	// ----------------------------------------------------------------------------------------------------------------=

	// ��ư�� ---------------------------------------------------------------------------------------------------------=
	int applybtnindex = g_MainInterfaceTextureArchive.FindResource( "mi_skillwin.tga" );
	_XBTN_STRUCTURE socketpluginbtnstruct = 
	{ 
		TRUE, { 9, 240  }, { 108, 24 }, 
		_XDEF_SOCKETPLUGIN_APPLY, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive 
	};
	m_SocketPlugInButton = new _XButton;
	m_SocketPlugInButton->Create( socketpluginbtnstruct );	
	m_SocketPlugInButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM); // ��ǰ ������
	m_SocketPlugInButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 176, 110, 200 );
	m_SocketPlugInButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 152, 110, 176 ); 
	m_SocketPlugInButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 201, 110, 225 );
	m_SocketPlugInButton->EnableWindow(FALSE);
	m_SocketPlugInButton->ShowWindow(FALSE);
	InsertChildObject( m_SocketPlugInButton );	

	applybtnindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	_XBTN_STRUCTURE applycanclebtnstruct = 
	{ 
		TRUE, { 117, 240  }, { 60, 24 }, 
		_XDEF_SOCKETPLUGIN_CANCLE, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive
	};
	m_pApplyCancleButton = new _XButton;
	m_pApplyCancleButton->Create( applycanclebtnstruct );	
	m_pApplyCancleButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pApplyCancleButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CANCLE);		// _T("���")
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	InsertChildObject( m_pApplyCancleButton );

	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_SOCKETPLUGIN_CLOSE,
									  charinfointerface,charinfointerface,charinfointerface, 
									  &g_MainInterfaceTextureArchive };
	m_pClosebutton = new _XButton;
	m_pClosebutton->Create( closebtnstruct );	
		
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( m_pClosebutton );

	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 153, 2 }, { 12, 12 }, _XDEF_SOCKETPLUGIN_HELP,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	// ----------------------------------------------------------------------------------------------------------------=
	for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT; i++ )
	{
		m_SocketItemList[i].Reset();
	}

	m_SlotPosition[0].x = 105;
	m_SlotPosition[0].y = 70;
	m_SlotPosition[1].x = 140;
	m_SlotPosition[1].y = 70;
	m_SlotPosition[2].x = 105;
	m_SlotPosition[2].y = 105;
	m_SlotPosition[3].x = 140;
	m_SlotPosition[3].y = 105;	

	// Model View Camera Setting =========================================
	m_SocketModelViewCamera.Set( 45, 83.0f/60.0f, 0.01f, 180.0f);
	m_SocketModelViewCamera.Set(0.0f, 0.0f, 0.0f);	
	m_SocketModelViewCamera.SetDistanceLimits( 0.01, 30.0f );
	m_SocketModelViewCamera.SetDistance( 1.0f );	
	m_SocketModelViewCamera.UpdateViewMatrix();
	m_SocketModelViewCamera.UpdateProjMatrix();

	// ViewPortReposition - for Model View
	m_SocketModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_SocketModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
	m_SocketModelViewPort.Width  = 83;
	m_SocketModelViewPort.Height = 60;
	m_SocketModelViewPort.MinZ = 0.0f;
	m_SocketModelViewPort.MaxZ = 1.0f;

	m_SocketRotateAngle = 0.0f;

	return TRUE;
}

void _XWindow_SocketPlugIn::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_SocketPlugIn::Draw( _XGUIObject*& pfocusobject )
{	
	_XWindow::Draw(pfocusobject);
	if(!this->m_ShowWindow)	return;

	int tempx = m_WindowPosition.x + 9;
	int tempy = m_WindowPosition.y + 68;

	if( m_IndependentItem )
	{
		for( int i = 0; i < m_IndependentItem->Get_m_ucSocketCount(); i++ )
		{
			tempx = m_WindowPosition.x+m_SlotPosition[i].x;
			tempy = m_WindowPosition.y+m_SlotPosition[i].y;
			_XDrawRectAngle( tempx, tempy, tempx+32, tempy+32, 0.0f,D3DCOLOR_ARGB(255,114,98,87) );
		}
		
		for( i = 0 ; i < _XDEF_SOCKETITEM_MAXCOUNT ; i++)
		{
			if( m_SocketItemList[i].Get_m_sID() )
				g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+m_SlotPosition[i].x+1, m_WindowPosition.y+m_SlotPosition[i].y+1, &m_SocketItemList[i]);
		}
	}
	
	DrawSelectedItem();
	if( m_IndependentItem )
	{
		DrawSocketPlugInionItem();

		g_XBaseFont->SetColor(_XSC_DEFAULT);
		int tempLength = strlen( _XGameItem::GetItemName(m_IndependentItem->Get_m_cType(), m_IndependentItem->Get_m_sID() ));
		if( tempLength > 16 )
		{

		}
		else
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+40, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemName(m_IndependentItem->Get_m_cType(), m_IndependentItem->Get_m_sID() ) );		
		}	

		g_XBaseFont->Flush();
	}
}

BOOL _XWindow_SocketPlugIn::Process( _XGUIObject*& pfocusobject )
{	
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;	

	// ������ư �̸� ó��
	if( !m_ObjectList.listEmpty() )
	{
		m_ObjectList.resetList();
		m_ObjectList.prevItem();
		_XGUIObject* currentObject = NULL;

		do
		{
			currentObject = (_XGUIObject*)m_ObjectList.currentItem();

			if(currentObject) 
			{
				currentObject->Process( pfocusobject );				
			}

			m_ObjectList.prevItem();
		}while( !m_ObjectList.atEndOfList() );
	}

	_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSAGEWINDOW);
	if( pMessageBox ) 
	{
		if(pfocusobject == _XDEF_DUMMYID_SOCKETPLUGIN ) pfocusobject = NULL;
			return FALSE;	
	}

	BOOL bUseSocketPlagIn = FALSE;
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();

	if(mousestate)
	{
		if(!pfocusobject || pfocusobject == this )
		{
			if(mousestate->bButton[1])
			{
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
				if(CheckMousePosition())
				{
					if(scrnpos->x > m_WindowPosition.x+9 && scrnpos->z < m_WindowPosition.x+173)
					{
						if(scrnpos->z > m_WindowPosition.y+68 && scrnpos->z < m_WindowPosition.y+138)
						{
							pfocusobject = _XDEF_DUMMYID_SOCKETPLUGIN;
						}
					}

					if(g_pInventory_Window)	// �κ��丮���� �巡�� ���� ���
					{
						if(g_pInventory_Window->GetShowStatus() && !g_pLocalUser->m_bSocketPlugInLock )
						{
							int ivindex = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
							if(g_pInventory_Window->m_bHaveMouseSlot && ivindex >= _XINVENTORY_SLOTTYPE_POCKETSTART)
							{
#ifdef _XTS_ITEM_OPTIMIZATION
								if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[ivindex].Get_m_cType()) ||
									_XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[ivindex].Get_m_cType()) )
#else
								if( g_pLocalUser->m_UserItemList[ivindex].Get_m_cType() == _XGI_FC_WEAPON || 
									g_pLocalUser->m_UserItemList[ivindex].Get_m_cType() == _XGI_FC_CLOTHES ||
									g_pLocalUser->m_UserItemList[ivindex].Get_m_cType() == _XGI_FC_WEAPON2 || 
									g_pLocalUser->m_UserItemList[ivindex].Get_m_cType() == _XGI_FC_CLOTHES2 ||
									g_pLocalUser->m_UserItemList[ivindex].Get_m_cType() == _XGI_FC_WEAPON3 ||
									g_pLocalUser->m_UserItemList[ivindex].Get_m_cType() == _XGI_FC_CLOTHES3 )
#endif
								{
									// error ó�� ------------------------------------------------------------------------------------=
									if( g_pLocalUser->m_UserItemList[ivindex].Get_m_cSecond() == _XGI_SC_CLOTHES_BACKPACK )
									{
										MessagePrint(7);
										return FALSE;
									}
									// -----------------------------------------------------------------------------------------------=

#ifdef _XDEF_NEW_SOCKET_071031_KUKURI
									int socketcheck = 0;
									if(_XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[ivindex].Get_m_cType()))
									{
										_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[ivindex].Get_m_cType());
										if( WeaponItem )
										{
											socketcheck = WeaponItem[g_pLocalUser->m_UserItemList[ivindex].Get_m_sID()].ucSocketCheck;
										}
									}
									else if(_XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[ivindex].Get_m_cType()))
									{
										_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[ivindex].Get_m_cType());
										if( ClothesItem )
										{
											socketcheck = ClothesItem[g_pLocalUser->m_UserItemList[ivindex].Get_m_sID()].ucSocketCheck;
										}
									}
									if(socketcheck == 0)
									{
										MessagePrint(14);
										return FALSE;
									}

#endif

									if( m_IndependentItem )
									{
										MessagePrint(10);
										return FALSE;
									}
									else
									{
										if( g_pLocalUser->m_UserItemList[ivindex].Get_m_ucSocketCount() == 0 )
										{
											MessagePrint(1);											
											return FALSE;
										}
										m_IndependentItem = &g_pLocalUser->m_UserItemList[ivindex];
										g_pInventory_Window->m_iSocketIndexList[0] = ivindex;
										PlayItemEquipSound( m_IndependentItem->Get_m_cType(), m_IndependentItem->Get_m_sID() );
										bool bflag = false;
										for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT/*g_pLocalUser->m_UserItemList[ivindex].m_ucSocketCount*/; i++ )
										{
											if( g_pLocalUser->m_UserItemList[ivindex].Get_m_sSocket(i) )
											{
												bflag = true;
												m_SocketItemList[i].Reset();
												m_SocketItemList[i].Set_m_cType(_XGI_FC_SOCKET);
												m_SocketItemList[i].Set_m_sID( g_pLocalUser->m_UserItemList[ivindex].Get_m_sSocket(i) );
											}
										}										
										g_pInventory_Window->m_bHaveMouseSlot = FALSE;
										g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();										
										if( bflag )
											RebuildListBoxText(_BOXTEXTMODE_STEP2);
										else
											RebuildListBoxText(_BOXTEXTMODE_STEP1);
										
										m_SocketPlugInButton->EnableWindow(TRUE);
										m_SocketPlugInButton->ShowWindow(TRUE);
										m_bChangeButtonMode = FALSE;
										m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM); // ��ǰ ������
									}																											
								}
								else if( g_pLocalUser->m_UserItemList[ivindex].Get_m_cType() == _XGI_FC_SOCKET )
								{
									if( m_IndependentItem )
									{
										int insertindex = -1;
										for( int i = 0; i < m_IndependentItem->Get_m_ucSocketCount(); i++ )
										{
											if( m_SocketItemList[i].Get_m_sID() == 0 )
											{
												insertindex = i;									
												break;
											}
										}
										
										if( insertindex == -1 )
										{
											MessagePrint(5);
											return FALSE;
										}
										else
										{
											// error ó�� ------------------------------------------------------------------------------------=
											char temp1 = m_IndependentItem->Get_m_cType();
											char temp2 = g_pLocalUser->m_UserItemList[ ivindex ].Get_m_cSecond();

#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : ����� //breif : ġ���ǻ� ���� ���� ��� ����
											//���� üũ
											if( !(g_pLocalUser->m_CharacterInfo.Get_level()*12+g_pLocalUser->m_CharacterInfo.Get_sublevel() >=
												g_SocketItemProperty[g_pLocalUser->m_UserItemList[ ivindex ].Get_m_sID()].sTempField5) )
											{
												MessagePrint(13);
												return FALSE;
											}

											if( m_IndependentItem->Get_m_cType() == _XGI_FC_CLOTHES ||
												m_IndependentItem->Get_m_cType() == _XGI_FC_CLOTHES2 ||
												m_IndependentItem->Get_m_cType() == _XGI_FC_CLOTHES3 )
											{
												if( g_pLocalUser->m_UserItemList[ ivindex ].Get_m_cSecond() == 1 )
												{
													switch( g_SocketItemProperty[g_pLocalUser->m_UserItemList[ ivindex ].Get_m_sID()].ucThirdType )
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
															if(m_IndependentItem->Get_m_cSecond() != 
																g_SocketItemProperty[g_pLocalUser->m_UserItemList[ ivindex ].Get_m_sID()].ucThirdType)
															{
																//�´� �������� �ƴ�
																MessagePrint(11);
																return FALSE;
															}
														}
														break;
													case _XGI_SC_CLOTHES_BACKPACK :		// �賶 = 7
														{
															MessagePrint(7);
															return FALSE;
														}
														break;
													case 10 : //���� ��Ĺ�
														{
															int nDebug = 0;
														}
														break;
													default :
														{
															//����Ÿ�� ����
															MessagePrint(12);
															return FALSE;
														}
														break;
													}
												}
	#ifdef _XDEF_NEW_SOCKET_071031_KUKURI
												if(g_SocketItemProperty[g_pLocalUser->m_UserItemList[ ivindex ].Get_m_sID()].ucGrade > 0)
												{
													_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_IndependentItem->Get_m_cType());
													if( ClothesItem )
													{
														if(ClothesItem[m_IndependentItem->Get_m_sID()].ucSocketCheck !=
															g_SocketItemProperty[g_pLocalUser->m_UserItemList[ ivindex ].Get_m_sID()].ucGrade)
														{
															MessagePrint(15);
															return FALSE;
														}
													}
												}
	#endif
											}

	#ifdef _XDEF_NEW_SOCKET_071031_KUKURI
											if(_XGI_FC_WEAPON == GetEqualItemType(m_IndependentItem->Get_m_cType()))
											{
												if(g_SocketItemProperty[g_pLocalUser->m_UserItemList[ ivindex ].Get_m_sID()].ucGrade > 0)
												{
													_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_IndependentItem->Get_m_cType());
													if( WeaponItem )
													{
														if(WeaponItem[m_IndependentItem->Get_m_sID()].ucSocketCheck !=
															g_SocketItemProperty[g_pLocalUser->m_UserItemList[ ivindex ].Get_m_sID()].ucGrade)
														{
															MessagePrint(15);
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
													MessagePrint(9);
													return FALSE;
												}
											}
											if( _XGI_FC_CLOTHES == GetEqualItemType(temp1) )
											{
												if( _XGI_FC_WEAPON == GetEqualItemType(temp2) )
												{
													MessagePrint(9);
													return FALSE;
												}
											}
#else
											if( temp1 == _XGI_FC_WEAPON || temp1 == _XGI_FC_WEAPON2 || temp1 == _XGI_FC_WEAPON3 )
											{
												if( temp2 == _XGI_FC_CLOTHES || temp2 == _XGI_FC_CLOTHES2 || temp2 == _XGI_FC_CLOTHES3 )
												{
													MessagePrint(9);
													return FALSE;
												}
											}
											if( temp1 == _XGI_FC_CLOTHES || temp1 == _XGI_FC_CLOTHES2 || temp1 == _XGI_FC_CLOTHES3 )
											{
												if( temp2 == _XGI_FC_WEAPON || temp2 == _XGI_FC_WEAPON2 || temp2 == _XGI_FC_WEAPON3 )
												{
													MessagePrint(9);
													return FALSE;
												}
											}
#endif
											// -----------------------------------------------------------------------------------------------=
											m_SocketItemList[insertindex].Copy( g_pLocalUser->m_UserItemList[ivindex] );
											g_pInventory_Window->m_iSocketIndexList[insertindex+1] = ivindex;
											g_pInventory_Window->m_bHaveMouseSlot = FALSE;
											g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
											m_bChangeButtonMode = TRUE;
											RebuildListBoxText(_BOXTEXTMODE_STEP2);
											m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_APPLY);	// _T("��Ĺ� �ޱ�")
											PlayItemEquipSound( m_SocketItemList[insertindex].Get_m_cType(), m_SocketItemList[insertindex].Get_m_sID() );
										}
									}
									else
									{
										MessagePrint(0);										
										return FALSE;
									}
								}
								else
								{
									MessagePrint(1);									
									return FALSE;
								}								
								bUseSocketPlagIn = TRUE;
							}							
						}
					}
					
					if( !bUseSocketPlagIn )
					{
						if( !m_bHaveMouseSlot )
						{
							// mouse ��ġ ����
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

					if( g_pInventory_Window )
					{
						if( g_pInventory_Window->GetShowStatus() )
						{
							g_pInventory_Window->m_bDrawSelectedItem = FALSE;
						}
					}
				} // if( CheckMousePosition() )
				else
				{
				}
			}			
		}
		if( pfocusobject == _XDEF_DUMMYID_SOCKETPLUGIN )
		{
			if( mousestate->bButton[0] )
			{
				//drag
				if( m_bDragState && !m_bHaveMouseSlot && !g_pLocalUser->m_bSocketPlugInLock)
				{
					if(scrnpos->x != m_IconDragStartPos.x || scrnpos->z != m_IconDragStartPos.y)
					{
						if(m_LastDraggingSlot != -1)
						{
							m_bHaveMouseSlot = TRUE;
							m_MouseSlotItem.Copy(m_SocketItemList[m_LastDraggingSlot]);
							m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
							m_LastDraggingSlot = -1;
						}
					}
				}
			}

			if( !mousestate->bButton[0] )
			{
				m_bDrawSelectedItem = TRUE;
				if( m_bDragState )
				{
					if(scrnpos->x != m_IconDragStartPos.x && scrnpos->z != m_IconDragStartPos.y)
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
				}
			} //if( !mousestate->bButton[0] )
		}

		if( !mousestate->bButton[1] )
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				BOOL resetmouse = TRUE;
				if( !m_bHaveMouseSlot && !g_pLocalUser->m_bSocketPlugInLock )
		
				{
					m_LastDraggingSlot = GetSlotIndex();
					if( m_LastDraggingSlot != -1)
					{
						/* ������ ������
						if( m_LastDraggingSlot == 0)
						{
							for( int i = 0; i < m_IndependentItem->m_ucSocketCount; i++ )
							{								
								m_SocketItemList[i].Reset();
								g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
							}
							m_IndependentItem = NULL;
							g_pInventory_Window->m_iSocketIndexList[0] = 0;
						}
						else*/
						
						{
							// �̹� ������ �ִ� �������� ���
							if( m_IndependentItem->Get_m_sSocket(m_LastDraggingSlot) > 0 )
							{
								MessagePrint(8);
								m_WindowMouseState = _XW_STATE_NONE;
								m_LastDraggingSlot = -1;
							}
							else
							{
								m_SocketItemList[m_LastDraggingSlot].Reset();
								g_pInventory_Window->m_iSocketIndexList[m_LastDraggingSlot+1] = 0;

								// ���� �߰��� ������ ���
								for( int i = m_LastDraggingSlot+1; i < m_IndependentItem->Get_m_ucSocketCount(); i++ )
								{
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : ����� //breif : ġ���ǻ� ���� ���� ��� ����
									if( m_SocketItemList[i].Get_m_sID() > 0 && m_IndependentItem->Get_m_sSocket(i) < 1)
									{
										for( int j = 0; j < m_IndependentItem->Get_m_ucSocketCount(); j++ )
										{
											if( m_SocketItemList[j].Get_m_sID() < 1 )
											{
												m_SocketItemList[j].Copy(m_SocketItemList[i]);
												m_SocketItemList[i].Reset();
												g_pInventory_Window->m_iSocketIndexList[j+1] = g_pInventory_Window->m_iSocketIndexList[i+1];
												g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
												break;
											}
										}
									}
#else
									if( m_SocketItemList[i].Get_m_sID() > 0 )
									{
										m_SocketItemList[i-1].Copy(m_SocketItemList[i]);
										m_SocketItemList[i].Reset();
										g_pInventory_Window->m_iSocketIndexList[i] = g_pInventory_Window->m_iSocketIndexList[i+1];
										g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
									}
#endif
									
								}

								// ��Ĺ��� �� ���ȴ��� �˻�
								BOOL tempflag = TRUE;
								for( i = 1; i < m_IndependentItem->Get_m_ucSocketCount()+1; i++ )
								{
									if( g_pInventory_Window->m_iSocketIndexList[i] > 0 )
										tempflag = FALSE;									
								}
								if( tempflag )
								{
									m_bChangeButtonMode = FALSE;
									m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM); // ��ǰ ������
									BOOL tempflag2 = TRUE;
									for( int i = 0; i < m_IndependentItem->Get_m_ucSocketCount()+1; i++ )
									{
										if( m_SocketItemList[i].Get_m_sID() > 0 )
											tempflag2 = FALSE;
									}
									if( tempflag2 )
										RebuildListBoxText(_BOXTEXTMODE_STEP1);
									else
										RebuildListBoxText(_BOXTEXTMODE_STEP2);
								}
								else
								{
									RebuildListBoxText(_BOXTEXTMODE_STEP2);
								}
								m_WindowMouseState = _XW_STATE_NONE;
								m_LastDraggingSlot = -1;
							}							
						}
					}					
				}
				if( g_pInventory_Window )
				{
					if( g_pInventory_Window->GetShowStatus() && g_pInventory_Window->CheckMousePosition() )
						resetmouse = FALSE;
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
			if(pfocusobject == _XDEF_DUMMYID_SOCKETPLUGIN)
				pfocusobject = NULL;
		}
	}
	
	if(!_XWindow::Process(pfocusobject)) return FALSE;

	return TRUE;
}

int _XWindow_SocketPlugIn::GetSlotIndex(void)
{
	if( m_MsgSocketPuluInOpen ) 
		return -1;
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();
	
	for(int i = 0 ; i < _XDEF_SOCKETITEM_MAXCOUNT ; i++)
	{
		if(scrnpos->x > m_WindowPosition.x + m_SlotPosition[i].x && scrnpos->x < m_WindowPosition.x + m_SlotPosition[i].x + 30)
		{
			if(scrnpos->z > m_WindowPosition.y + m_SlotPosition[i].y && scrnpos->z < m_WindowPosition.y + m_SlotPosition[i].y + 30)
			{
				if( m_SocketItemList[i].Get_m_sID() )
					return i;
			}
		}
	}
	return -1;
}

void _XWindow_SocketPlugIn::DrawDragIcon(void)
{
	_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSAGEWINDOW);
	if(pMessageBox)
	{
		if(pMessageBox->GetShowStatus())
			return;
	}	
	
	if(m_bHaveMouseSlot)
	{
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
		g_pInventory_Window->DrawIcon(TRUE, scrnpos->x - 16 , scrnpos->z - 16, (_XUserItem*)&m_MouseSlotItem, FALSE);
		
		((CEmperorOfDragonsApp*)gApp)->ReadyCursor(_XMOUSETYPE_SELECT);
	}	
}

void _XWindow_SocketPlugIn::DrawSelectedItem(void)
{
	if(m_MouseSlotItem.Get_m_sID() > 0)
	{
		int selectedslot = m_MouseSlotItem.m_InventoryNumber;
		
		if( selectedslot > -1 )
		{
			_XDrawRectAngle(m_WindowPosition.x+m_SlotPosition[selectedslot].x,
				m_WindowPosition.y+m_SlotPosition[selectedslot].y,
				m_WindowPosition.x+m_SlotPosition[selectedslot].x+31,
				m_WindowPosition.y+m_SlotPosition[selectedslot].y+31, 1.0f, _XSC_ITEM_SELECT);		
			
			if(m_bHaveMouseSlot)
			{
				// disable icon image
				_XDrawSolidBar( m_WindowPosition.x+m_SlotPosition[selectedslot].x+1,
					m_WindowPosition.y+m_SlotPosition[selectedslot].y+1,
					m_WindowPosition.x+m_SlotPosition[selectedslot].x+30, 
					m_WindowPosition.y+m_SlotPosition[selectedslot].y+30, D3DCOLOR_ARGB(160, 0, 0, 0));
			}					
		}		
	}
}

void _XWindow_SocketPlugIn::ChangeIndependentItem(void)
{	
	for( int i = 0; i < m_IndependentItem->Get_m_ucSocketCount(); i++ )
	{
		if( m_SocketItemList[i].Get_m_sID() )
		{
			m_SocketItemList[i].Reset();
			g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
		}
	}
	
	m_IndependentItem = &g_pLocalUser->m_UserItemList[m_TempSaveSlotItem.m_InventoryNumber];
	g_pInventory_Window->m_iSocketIndexList[0] = m_TempSaveSlotItem.m_InventoryNumber;
	
	for( i = 0; i < g_pLocalUser->m_UserItemList[m_TempSaveSlotItem.m_InventoryNumber].Get_m_ucSocketCount(); i++ )
	{
		if( g_pLocalUser->m_UserItemList[m_TempSaveSlotItem.m_InventoryNumber].Get_m_sSocket(i) )
		{
			m_SocketItemList[i].Reset();
			m_SocketItemList[i].Set_m_cType(_XGI_FC_SOCKET);			
			m_SocketItemList[i].Set_m_sID( g_pLocalUser->m_UserItemList[m_TempSaveSlotItem.m_InventoryNumber].Get_m_sSocket(i) );
		}
	}
}

void _XWindow_SocketPlugIn::MessagePrint(int msgindex)
{
	_XMessageWindow* pFindMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
	if(pFindMessageBox)
	{
		if(pFindMessageBox->GetShowStatus())
			return;
	}
	
	_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
	if( pMessageBox )
	{
		TCHAR messagestring[256];
		memset( messagestring, 0, sizeof(TCHAR)*256 );
		
		switch(msgindex) {
		case 0:
			{
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR1), TRUE, 0xFFFF0000 ); // "��Ĺ��� �����ϱ� ���ؼ���, ��ǰ�� ��Ĺ��� ��� �ʿ� �մϴ�."
			}			
			break;
		case 1:
			{
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR2), TRUE, 0xFFFF0000 ); // "��Ĺ��� ������ �� ���� ��ǰ�Դϴ�."
			}
			break;
		case 2:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR3));	// "�� ��ǰ�� ��Ĺ��� �ƴմϴ�."
			break;
		case 3:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR4)); // ��ǰ�� ��ü�Ͽ�, ���ο� ��Ĺ� ������ ���� �Ͻðڽ��ϱ�?
			break;
		case 4:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_APPLYEQUIP));		// "��Ĺ��� �����Ͻðڽ��ϱ�?"
			break;
		case 5:
			{
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR5), TRUE, 0xFFFF0000); // "�� �̻� ��Ĺ��� ������ �� �����ϴ�."
			}			
			break;
		case 6:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_STOPITEM)); // ��Ĺ� �ޱ⸦ �ߴ��Ͻðڽ��ϱ�?
			break;
		case 7 :
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR6)); // �೶���� ��Ĺ��� ������ �� �����ϴ�.
			break;	
		case 8 :
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR7));	// "�̹� �����Ǿ� �ִ� ��Ĺ��� �ǵ��� �� �����ϴ�."
			break;	
		case 9 :
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR8));	// "���� ��Ĺ��� �������� ������ �� �ֽ��ϴ�."
			break;
		case 10:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR9));	// "��ǰ�� ��ü�� �� �����ϴ�. "
			break;
		case 11 :
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3746));	//���� �ǻ� ������ �� ���� ��Ĺ� �Դϴ�.
			break;
		case 12 :
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3747));	//���� ��ǰ�� ������ �� ���� ��Ĺ� �Դϴ�.
			break;
		case 13 :
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3748));	//������ ���Ƽ� ���� �� �� �����ϴ�.
			break;
		case 14 :
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3749));//�������� (����/�ǻ�) socket check�� 0
			break;
		case 15 :
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3750));	//����/�ǻ��� socket check�� ������ socket grade�� �ٸ�
			break;
		}
		
		
		if( msgindex == 3 )
		{	
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_SOCKETPLUGIN_CHANGEOK, _XDEF_SOCKETPLUGIN_CHANGECANCLE);				
		}
		else if( msgindex == 4 )
		{
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_SOCKETPLUGIN_APPLYOK, _XDEF_SOCKETPLUGIN_APPLYCANCLE);				
		}
		else if( msgindex == 6 )
		{
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_SOCKETPLUGIN_CLOSEOK, _XDEF_SOCKETPLUGIN_CLOSECANCLE);
		}
		else
		{
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, messagestring, TRUE);		
		}
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
	}	
}

void _XWindow_SocketPlugIn::ResetSocketItemList(void)
{
	m_bChangeButtonMode		= FALSE;
	m_IndependentItem		= NULL;

	for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT; i++ )
	{
		m_SocketItemList[i].Reset();		
	}
	m_bHaveMouseSlot = FALSE;
	m_MouseSlotItem.ResetMouseSlotItem();
}

void _XWindow_SocketPlugIn::RebuildListBoxText( ListBoxTextMode mode)
{
	for( int i = 0; i < 8; i++ )
		memset( m_ListBoxTextList[i], 0, sizeof(TCHAR)*255 );
	
	switch(mode) 
	{
	case _BOXTEXTMODE_READY:
		{
			strcpy(m_ListBoxTextList[0], _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_SELECTITEM) ); // ��Ĺ��� �����Ͽ� �ֽʽÿ�.
		}
		break;
	case _BOXTEXTMODE_STEP1:
		{
			strcpy(m_ListBoxTextList[0], _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_SELECTITEM) );	// _T("����� ��ǰ�� �����Ͽ� �ֽʽÿ�.")
		}
		break;
	case _BOXTEXTMODE_STEP2:
		{
			for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT; i++ )
			{
				if( m_SocketItemList[i].Get_m_sID() > 0 )
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_IndependentItem->Get_m_cType()) )
#else
					if( m_IndependentItem->Get_m_cType() == _XGI_FC_WEAPON || m_IndependentItem->Get_m_cType() == _XGI_FC_WEAPON2 || m_IndependentItem->Get_m_cType() == _XGI_FC_WEAPON3 )
#endif
					{
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID1);
						if(iter_table != g_CharacterStateTable.end())
						{
							TCHAR temp_buf[255];
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per1 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per1 );	// "%d�ʸ��� "
								strcat( m_ListBoxTextList[i], temp_buf );
							}						
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb1 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb1 );	// "%d%%Ȯ���� "
								strcat( m_ListBoxTextList[i], temp_buf );
							}

							sprintf( temp_buf, g_CharacterStateTable[g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID1]->concept, 
								g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sValue1 );
							strcat( m_ListBoxTextList[i], temp_buf );
						}
						iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID2);
						if(iter_table != g_CharacterStateTable.end())
						{
							TCHAR temp_buf[255];
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per2 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per2 );	// "%d�ʸ��� "
								strcat( m_ListBoxTextList[i+4], temp_buf );
							}						
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb2 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb2 );	// "%d%%Ȯ���� "
								strcat( m_ListBoxTextList[i+4], temp_buf );
							}
							
							sprintf( temp_buf, 
									 g_CharacterStateTable[g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID2]->concept, 
									 g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sValue2 );
							strcat( m_ListBoxTextList[i+4], temp_buf );
						}
					}
					else if( m_IndependentItem->Get_m_cType() == _XGI_FC_CLOTHES || m_IndependentItem->Get_m_cType() == _XGI_FC_CLOTHES2 || m_IndependentItem->Get_m_cType() == _XGI_FC_CLOTHES3 )
					{
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID3);
						if(iter_table != g_CharacterStateTable.end())
						{
							TCHAR temp_buf[255];
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per3 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per3 );	// "%d�ʸ��� "
								strcat( m_ListBoxTextList[i], temp_buf );
							}						
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb3 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb3 );	// "%d%%Ȯ���� "
								strcat( m_ListBoxTextList[i], temp_buf );
							}
							
							sprintf( temp_buf, 
									 g_CharacterStateTable[g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID3]->concept, 
									 g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sValue3 );
							strcat( m_ListBoxTextList[i], temp_buf );
						}

						iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID4);
						if(iter_table != g_CharacterStateTable.end())
						{
							TCHAR temp_buf[255];
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per4 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per4 );	// "%d�ʸ��� "
								strcat( m_ListBoxTextList[i+4], temp_buf );
							}						
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb4 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb4 );	// "%d%%Ȯ���� "
								strcat( m_ListBoxTextList[i+4], temp_buf );
							}
							
							sprintf( temp_buf, 
									 g_CharacterStateTable[g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID4]->concept, 
									 g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sValue4 );
							strcat( m_ListBoxTextList[i+4], temp_buf );
						}
					}
				}
			}
		}
		break;	
	}	

	if( m_pSocketitemListBox )
	{
		m_pSocketitemListBox->DeleteAllItem();

		_XStringSplitInfo splitinfo;
		int viewsize = 146;

		for( int i = 0; i < 8; i++ )
		{
			if( strlen( m_ListBoxTextList[i] ) )
			{
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->SplitString(m_ListBoxTextList[i], viewsize, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);		
				}
				else
				{
					g_XBaseFont->SplitString(m_ListBoxTextList[i], viewsize, splitinfo, 0);		
				}
				TCHAR	splitstring[256];		
				for(int i = 0; i < splitinfo.splitcount ; i++)
				{
					memset(splitstring, 0, sizeof(TCHAR)*256);
					strncpy(splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
					
					m_pSocketitemListBox->InsertListItem( splitstring, _XSC_DEFAULT );
					m_pSocketitemListBox->SetItemCommandID( m_pSocketitemListBox->GetItemCount()-1, 0 );
					m_pSocketitemListBox->SetItemAttrib( m_pSocketitemListBox->GetItemCount()-1, 0, _XSC_DEFAULT );
				}
			}
		}		
	}
}

void _XWindow_SocketPlugIn::SetDefaultPosition( void )
{		
	MoveWindow( gnWidth-376 , 220 );
}

void _XWindow_SocketPlugIn::DrawSocketPlugInionItem( void )
{
	D3DXMATRIX			rotmat;
	D3DXMatrixIdentity(&rotmat);
	
	m_SocketRotateAngle++;
	D3DXMatrixRotationY(&rotmat, _X_RAD(m_SocketRotateAngle));

#ifdef _XTS_ITEM_OPTIMIZATION
	char cType = GetEqualItemType(m_IndependentItem->Get_m_cType());
	if( cType == _XGI_FC_WEAPON )
#else
	if( m_IndependentItem->Get_m_cType() == _XGI_FC_WEAPON || m_IndependentItem->Get_m_cType() == _XGI_FC_WEAPON2 || m_IndependentItem->Get_m_cType() == _XGI_FC_WEAPON3 )
#endif
	{
		D3DXMATRIX			zrotmat;
		D3DXMatrixRotationZ(&zrotmat, _X_RAD(45.0f));
		
		D3DXMatrixMultiply( &rotmat, &rotmat, &zrotmat );
	}	
	
	// Initialize Matrix
	int modelindex = GetSocketModelIndex();
	D3DXMatrixIdentity(&m_SocketRHWMatrix);
	
	// Camera/ViewPort Setting
	m_SocketModelViewCamera.UpdateViewMatrix( NULL, FALSE );
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&m_SocketModelViewCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&m_SocketModelViewCamera.GetProjMatrix());
	gpDev->SetViewport(&m_SocketModelViewPort);
	
	// Z Buffer�� clear �Ѵ�. - ���� viewport������ clear�ȴ�.
	gpDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
	
	// Rendering			
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );	
	
	
	
	FLOAT scalefactor;
#ifdef _XTS_ITEM_OPTIMIZATION
	switch(cType) 
#else
		switch(m_IndependentItem->m_cType) 
#endif
	{
#ifdef _XTS_ITEM_OPTIMIZATION
	case _XGI_FC_WEAPON:
#else
	case _XGI_FC_WEAPON:
	case _XGI_FC_WEAPON2:
	case _XGI_FC_WEAPON3:
#endif
		{
			D3DXMatrixTranslation( &m_SocketRHWMatrix, 
				-g_ItemModel_Weapon[0][modelindex]->OBB_Center.x,
				-g_ItemModel_Weapon[0][modelindex]->OBB_Center.y,
				-g_ItemModel_Weapon[0][modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_Weapon[0][modelindex]->m_ObjectRadius;
			D3DXMATRIX matRot;
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixScaling(&matRot, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply( &matRot, &matRot, &rotmat );							
			D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &matRot);
			
			g_ItemModel_Weapon[0][modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
		}		
		break;
#ifdef _XTS_ITEM_OPTIMIZATION
	case _XGI_FC_CLOTHES:
#else
	case _XGI_FC_CLOTHES:
	case _XGI_FC_CLOTHES2:
	case _XGI_FC_CLOTHES3:
#endif
		{
			switch( m_IndependentItem->Get_m_cSecond() ) 
			{
			case _XGI_SC_CLOTHES_UPPER:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_UBodyType[modelindex]->OBB_Center.x,
						-g_UserModel_UBodyType[modelindex]->OBB_Center.y,
						-g_UserModel_UBodyType[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_UBodyType[modelindex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_UBodyType[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_UBodyType[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_LOWER:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_LBodyType[modelindex]->OBB_Center.x,
						-g_UserModel_LBodyType[modelindex]->OBB_Center.y,
						-g_UserModel_LBodyType[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_LBodyType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_LBodyType[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_LBodyType[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_GLOVES:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_LHandType[modelindex]->OBB_Center.x,
						-g_UserModel_LHandType[modelindex]->OBB_Center.y,
						-g_UserModel_LHandType[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_LHandType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = 0.0f;
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_LHandType[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_SHOES:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_LFootType[modelindex]->OBB_Center.x,
						-g_UserModel_LFootType[modelindex]->OBB_Center.y,
						-g_UserModel_LFootType[modelindex]->OBB_Center.z	);
					scalefactor = 0.28f / g_UserModel_LFootType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = scalefactor*g_UserModel_LFootType[modelindex]->OBB_Center.x;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_LFootType[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_LFootType[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);					
				}
				break;
			case _XGI_SC_CLOTHES_CAP:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_Cap[modelindex]->OBB_Center.x,
						-g_UserModel_Cap[modelindex]->OBB_Center.y,
						-g_UserModel_Cap[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_Cap[modelindex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_Cap[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_Cap[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;			
			case _XGI_SC_CLOTHES_BACKPACK:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_BackPack[modelindex]->OBB_Center.x,
						-g_UserModel_BackPack[modelindex]->OBB_Center.y,
						-g_UserModel_BackPack[modelindex]->OBB_Center.z	);
					scalefactor = 0.4f / g_UserModel_BackPack[modelindex]->m_ObjectRadius; 
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_BackPack[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_BackPack[modelindex]->RenderInterfaceItem( &m_SocketRHWMatrix );
				}
				break;
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : ����� //breif : ġ���ǻ� ���� ���� ��� ����
			case _XGI_SC_CLOTHES_MASK :
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_Mask[modelindex]->OBB_Center.x,
						-g_UserModel_Mask[modelindex]->OBB_Center.y,
						-g_UserModel_Mask[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_Mask[modelindex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_Mask[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_Mask[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;				
#endif
			}			
		}		
		break;
	case _XGI_FC_POTION:
		{
			D3DXMatrixTranslation( &m_SocketRHWMatrix, 
				-g_ItemModel_Potion[modelindex]->OBB_Center.x,
				-g_ItemModel_Potion[modelindex]->OBB_Center.y,
				-g_ItemModel_Potion[modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_Potion[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
			m_SocketRHWMatrix._41 = 0.0;
			m_SocketRHWMatrix._42 = -(scalefactor*g_ItemModel_Potion[modelindex]->OBB_Center.y);;
			m_SocketRHWMatrix._43 = 0;
			
			g_ItemModel_Potion[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
		}
		break;
	}	
	
	// View Matrix, Projection Matrix, Viewport �ٽ� �ǵ�����. ====
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetProjMatrix());
	gpDev->SetViewport(&g_MainViewPort);

}

#ifdef _XTS_ITEM_OPTIMIZATION
int _XWindow_SocketPlugIn::GetSocketModelIndex()
{
	int index=0;
	switch( GetEqualItemType(m_IndependentItem->Get_m_cType()) ) 
	{
	case _XGI_FC_WEAPON:
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_IndependentItem->Get_m_cType());
			if( WeaponItem )
				index = WeaponItem[m_IndependentItem->Get_m_sID()].sModelIndex;															
		}		
		break;
	case _XGI_FC_CLOTHES:
		{
			if( m_IndependentItem->Get_m_cSecond() == _XGI_SC_CLOTHES_MASK )
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_IndependentItem->Get_m_cType());
				if( ClothesItem )
				{
					index = ClothesItem[m_IndependentItem->Get_m_sID()].sModelIndex;
					if( ClothesItem[m_IndependentItem->Get_m_sID()].cSexType ) // ���ڸ� �ε��� �߰�.
					{
						// ������ �迭�� �� �����̳� ������ ��� ����Ͽ� ���� �߰��� ���� �ε��� ���.
						if( index >= 140 )
						{
							// ���� �߰��� ���� �� �����̳� �� 220�� ������. ���ڴ� 140������ �̹Ƿ� �ι�° �� �����̳� ������ 80ĭ��
							index+= g_MaskModel_GenderSeparateIndex2;
						}
						else
						{
							index+= g_MaskModel_GenderSeparateIndex;
						}
					}
				}
			}
			else
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_IndependentItem->Get_m_cType());
				if( ClothesItem )
				{
					index = ClothesItem[m_IndependentItem->Get_m_sID()].sModelIndex;
					if( ClothesItem[m_IndependentItem->Get_m_sID()].cSexType ) // ���ڸ� �ε��� �߰�.
						index+= g_Model_GenderSeparateIndex;
				}			
			}
		}		
		break;	
	case _XGI_FC_POTION:
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(m_IndependentItem->Get_m_cType());
			if( PotionItem )
				index = PotionItem[m_IndependentItem->Get_m_sID()].sModelIndex;
		}
		break;
	}
	
	return index;
}
#else
int _XWindow_SocketPlugIn::GetSocketModelIndex()
{
	int index=0;
	switch(m_IndependentItem->m_cType) 
	{
	case _XGI_FC_WEAPON:
		{
			index = g_WeaponItemProperty[m_IndependentItem->m_sID].sModelIndex;															
		}		
		break;
	case _XGI_FC_CLOTHES:
		{
			
			index = g_ClothesItemProperty[m_IndependentItem->m_sID].sModelIndex;
			if( g_ClothesItemProperty[m_IndependentItem->m_sID].cSexType ) 
				index+= g_Model_GenderSeparateIndex;
		}		
		break;
	case _XGI_FC_WEAPON2:
		{
			index = g_WeaponItemProperty2[m_IndependentItem->m_sID].sModelIndex;															
		}		
		break;
	case _XGI_FC_CLOTHES2:
		{
			
			index = g_ClothesItemProperty2[m_IndependentItem->m_sID].sModelIndex;
			if( g_ClothesItemProperty2[m_IndependentItem->m_sID].cSexType ) 
				index+= g_Model_GenderSeparateIndex;
		}		
		break;
	case _XGI_FC_POTION:
		{			
			index = g_PotionItemProperty[m_IndependentItem->m_sID].sModelIndex;
		}
		break;
	case _XGI_FC_WEAPON3:
		{
			index = g_WeaponItemProperty3[m_IndependentItem->m_sID].sModelIndex;															
		}		
		break;
	case _XGI_FC_CLOTHES3:
		{
			
			index = g_ClothesItemProperty3[m_IndependentItem->m_sID].sModelIndex;
			if( g_ClothesItemProperty3[m_IndependentItem->m_sID].cSexType ) 
				index+= g_Model_GenderSeparateIndex;
		}		
		break;
	}
	
	return index;
}
#endif

void _XWindow_SocketPlugIn::MoveWindow( int X, int Y )
{	
	_XWindow::MoveWindow( X, Y );
	m_SocketModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_SocketModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
}

void _XWindow_SocketPlugIn::ShowWindow(BOOL show)
{
	if(show)
	{
		if(g_pInventory_Window)
		{
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;
		}
		
		m_SocketPlugInButton->EnableWindow(TRUE);
		m_pApplyCancleButton->EnableWindow(TRUE);
		m_pClosebutton->EnableWindow(TRUE);		
	}
	else
	{
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

#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : ����� //breif : ġ���ǻ� ���� ���� ��� ����
		ResetData();
#endif
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_SocketPlugIn::ProcessDblClick(void)
{
	if(g_pInventory_Window)
	{
		if(g_pInventory_Window->GetShowStatus() && !g_pLocalUser->m_bSocketPlugInLock )
		{
			if(CheckMousePosition())
			{
				m_LastDraggingSlot = GetSlotIndex();

				if( m_LastDraggingSlot != -1)
				{
					if( m_IndependentItem->Get_m_sSocket(m_LastDraggingSlot) > 0 )
					{
						MessagePrint(8);
						m_WindowMouseState = _XW_STATE_NONE;
						m_LastDraggingSlot = -1;
					}
					else
					{
						m_SocketItemList[m_LastDraggingSlot].Reset();
						g_pInventory_Window->m_iSocketIndexList[m_LastDraggingSlot+1] = 0;
						
						// ���� �߰��� ������ ���
						for( int i = m_LastDraggingSlot+1; i < m_IndependentItem->Get_m_ucSocketCount(); i++ )
						{
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : ����� //breif : ġ���ǻ� ���� ���� ��� ����
							if( m_SocketItemList[i].Get_m_sID() > 0 && m_IndependentItem->Get_m_sSocket(i) < 1)
							{
								for( int j = 0; j < m_IndependentItem->Get_m_ucSocketCount(); j++ )
								{
									if( m_SocketItemList[j].Get_m_sID() < 1 )
									{
										m_SocketItemList[j].Copy(m_SocketItemList[i]);
										m_SocketItemList[i].Reset();
										g_pInventory_Window->m_iSocketIndexList[j+1] = g_pInventory_Window->m_iSocketIndexList[i+1];
										g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
										break;
									}
								}
							}
#else
							if( m_SocketItemList[i].Get_m_sID() > 0 )
							{
								m_SocketItemList[i-1].Copy(m_SocketItemList[i]);
								m_SocketItemList[i].Reset();
								g_pInventory_Window->m_iSocketIndexList[i] = g_pInventory_Window->m_iSocketIndexList[i+1];
								g_pInventory_Window->m_iSocketIndexList[i+1] = 0;
							}
#endif
						}
						
						// ��Ĺ��� �� ���ȴ��� �˻�
						BOOL tempflag = TRUE;
						for( i = 1; i < m_IndependentItem->Get_m_ucSocketCount()+1; i++ )
						{
							if( g_pInventory_Window->m_iSocketIndexList[i] > 0 )
								tempflag = FALSE;									
						}
						if( tempflag )
						{
							m_bChangeButtonMode = FALSE;
							m_SocketPlugInButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM); // ��ǰ ������
							BOOL tempflag2 = TRUE;
							for( int i = 0; i < m_IndependentItem->Get_m_ucSocketCount()+1; i++ )
							{
								if( m_SocketItemList[i].Get_m_sID() > 0 )
									tempflag2 = FALSE;
							}
							if( tempflag2 )
								RebuildListBoxText(_BOXTEXTMODE_STEP1);
							else
								RebuildListBoxText(_BOXTEXTMODE_STEP2);
						}
						else
						{
							RebuildListBoxText(_BOXTEXTMODE_STEP2);
						}
						m_WindowMouseState = _XW_STATE_NONE;
						m_LastDraggingSlot = -1;

					}
				}
			}
		}
	}		
}

void _XWindow_SocketPlugIn::ResetData(void)
{
	m_bChangeButtonMode	= FALSE;
	m_MsgSocketPuluInOpen = FALSE;
	ResetSocketItemList();
	RebuildListBoxText(_BOXTEXTMODE_READY);
	if( g_pInventory_Window )
		g_pInventory_Window->ResetSocketIndexList();
}

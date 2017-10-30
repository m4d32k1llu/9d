// XWindow_SmeltItem.cpp: implementation of the XWindow_SmeltItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "SoundEffectList.h"
#include "XWindowObjectDefine.h"
#include "xsr_stringheader.h"

#include "XMessageWindow.h"
#include "XWindow_SystemMsgDefault.h"
#include "XWindow_BloodPoint.h"
#include "XWindow_SmeltItem.h"

#include "XWindow_MnDTotalPoint.h" //Author : ����� //breif :���� �ý���
#include "XWindow_MnDCurrentPoint.h" //Author : ����� //breif :���� �ý���

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static int _XDEF_INVENTORYITEMPOSX = 11;
static int _XDEF_INVENTORYITEMPOSY = 71;


static int _XDEF_INVENTORYSLOT_WIDTH = 5;
static int _XDEF_INVENTORYSLOT_HEIGHT = 5;

XWindow_SmeltItem::XWindow_SmeltItem() : m_ItemStep(ITEMSTEP_NONE) , m_ItemFlag(FLAG_NONE), m_pUserItem(NULL), m_pSmletItem(NULL), m_pConfirmButton(NULL), 
										 m_pChargeGaugeWindow(NULL), m_pCancleButton(NULL), m_pSocketItemLeftView(NULL), m_pSocketItemRightView(NULL), 
										 m_pLeftTopView(NULL), m_pRightTopView(NULL), m_pLeftDownView(NULL), m_pRightDownView(NULL), m_slotImage(NULL),
										 m_nCount(0), m_ucResult(0), m_nUserIndex(0),m_nSmeltIndex(0), m_nSmeltNextIndex(0)
{

}

XWindow_SmeltItem::~XWindow_SmeltItem()
{

}

BOOL XWindow_SmeltItem::Initialize()
{
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 184, 71, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" ) );	
	pTitleBar->SetClipRect( 0, 125, 183, 195 );
	InsertChildObject( pTitleBar );

	_XImageStatic* pBackBoard = new _XImageStatic;
	pBackBoard->Create( 0, 60, 184, 271, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" ) );	
	pBackBoard->SetClipRect( 0, 1, 183, 211 );
	InsertChildObject( pBackBoard );

	// �⺻ , ITEMSTEP_READY ȭ��
	m_pSocketItemLeftView = new _XImageStatic;
	m_pSocketItemLeftView->Create( 7, 26, 93, 141, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	m_pSocketItemLeftView->SetClipRect( 4, 140, 89, 254 );
	InsertChildObject( m_pSocketItemLeftView );

	m_pSocketItemRightView = new _XImageStatic;
	m_pSocketItemRightView->Create( 176, 26, 261, 141, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	m_pSocketItemRightView->SetClipRect( 4, 140, 88, 254 );
	m_pSocketItemRightView->SetScale( -1.0f, 1.0f );
	InsertChildObject( m_pSocketItemRightView );
	// �⺻ , ITEMSTEP_READY ȭ��

	// ITEMSTEP_CONFIRM, ITEMSTEP_PROGRESS ȭ��
	m_pLeftTopView = new _XImageStatic;
	m_pLeftTopView->Create( 7, 26, 99, 95, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	m_pLeftTopView->SetClipRect(  4, 180, 96, 249 );
	InsertChildObject( m_pLeftTopView );

	m_pRightTopView = new _XImageStatic;
	m_pRightTopView->Create( 176, 26, 253, 95, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	m_pRightTopView->SetClipRect(  4, 180, 81, 249);
	m_pRightTopView->SetScale( -1.0f, 1.0f );
	InsertChildObject( m_pRightTopView );

	m_pLeftDownView = new _XImageStatic;
	m_pLeftDownView->Create( 7, 95, 99, 165, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	m_pLeftDownView->SetClipRect(  4, 185, 96, 254 );
	InsertChildObject( m_pLeftDownView );

	m_pRightDownView = new _XImageStatic;
	m_pRightDownView->Create( 176, 95, 253, 165, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	m_pRightDownView->SetClipRect(  4, 185, 81, 254 );
	m_pRightDownView->SetScale( -1.0f, 1.0f );
	InsertChildObject( m_pRightDownView );
	// ITEMSTEP_CONFIRM, ITEMSTEP_PROGRESS ȭ��

	//���� �̹���
	m_slotImage = new _XImageStatic;
	m_slotImage->Create( 12, 172, 44, 204, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" ) );
	m_slotImage->SetClipRect( 214, 31, 246, 63 );
	InsertChildObject( m_slotImage );

	// ��ư�� ---------------------------------------------------------------------------------------------------------=
	int applybtnindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	_XBTN_STRUCTURE Confirmbtnstruct = 
	{ 
		TRUE, { 49, 242  }, { 60, 24 }, 
		_XDEF_SMELT_CONFIRMBUTTON, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive 
	};
	m_pConfirmButton = new _XButton;
	m_pConfirmButton->Create( Confirmbtnstruct );	
	m_pConfirmButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pConfirmButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_OK);		// _T("Ȯ��")
	m_pConfirmButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 187, 152, 247, 176 );
	m_pConfirmButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
	m_pConfirmButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	m_pConfirmButton->ShowWindow(FALSE);
	InsertChildObject( m_pConfirmButton );	


	_XBTN_STRUCTURE Canclebtnstruct = 
	{ 
		TRUE, { 117, 242  }, { 60, 24 }, 
		_XDEF_SMELT_CANCLEBUTTON, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive
	};
	m_pCancleButton = new _XButton;
	m_pCancleButton->Create( Canclebtnstruct );	
	m_pCancleButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pCancleButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CANCLE);		// _T("���")
	m_pCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 187, 152, 247, 176 );
	m_pCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  187, 127, 247, 151 );
	m_pCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	m_pCancleButton->ShowWindow(TRUE);
	InsertChildObject( m_pCancleButton );

	m_IMEControl.ResetContainer();

	if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
		m_IMEControl.MoveWindow(m_WindowPosition.x + 108, m_WindowPosition.y + 200);
	else
		m_IMEControl.MoveWindow(m_WindowPosition.x + 86, m_WindowPosition.y + 204);

	m_IMEControl.ResizeWindow(18, 12);
	m_IMEControl.ShowWindow(FALSE);
	m_IMEControl.SetMaxLength(1);	//ǥ�ñ���
	m_IMEControl.SetFocus();
	m_IMEControl.SetAlignType( _XFONT_ALIGNTYPE_RIGHT );
	m_IMEControl.SetNumericEditMode( TRUE, TRUE );

	// Model View Camera Setting =========================================
	m_ItemModelViewCamera.Set( 45, 154.0f/60.0f, 0.01f, 180.0f);
	m_ItemModelViewCamera.Set(0.0f, 0.0f, 0.0f);	
	m_ItemModelViewCamera.SetDistanceLimits( 0.01, 30.0f );
	m_ItemModelViewCamera.SetDistance( 1.0f );	
	m_ItemModelViewCamera.UpdateViewMatrix();
	m_ItemModelViewCamera.UpdateProjMatrix();

	// ViewPortReposition - for Model View
	m_ItemModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_ItemModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
	m_ItemModelViewPort.Width  = 154;
	m_ItemModelViewPort.Height = 60;
	m_ItemModelViewPort.MinZ = 0.0f;
	m_ItemModelViewPort.MaxZ = 1.0f;

	m_fItemtRotateAngle = 0.0f;
	

	return TRUE;
}

void XWindow_SmeltItem::DestroyWindow()
{
	_XWindow::DestroyWindow();

	m_ItemStep = ITEMSTEP_NONE;
	m_fItemtRotateAngle = 0.0f;
	memset(m_strInfoText, 0, sizeof(TCHAR)*128);
	m_nCount = 0;

	m_ltSlotNumber.clear();
}

void XWindow_SmeltItem::ShowWindow( BOOL show )
{	
	if(show)
	{
		if(g_pInventory_Window)
		{
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;
		}

		_XWindow_SystemMsgDefault* systemWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);
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
		// ��� ������ Inventory lock ����
		if(g_pInventory_Window)
		{
			if(g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = FALSE;
		}

		m_nCount = 0;
		m_nUserIndex = 0;
		m_nSmeltIndex = 0;
		m_nSmeltNextIndex = 0;
		m_ucResult = 0;
		m_IMEControl.ClearBuffer();
		_XIMEKERNEL.SetFocus( NULL );

		SetItemFlag( m_ItemFlag, ITEMSTEP_NONE );

		_XWindow_SystemMsgDefault* systemWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);
		
		if( g_pInventory_Window && g_pInventory_Window->GetShowStatus() )
			 systemWindow->SetDockingMode(TRUE);	
		 else
			 systemWindow->SetDockingMode(FALSE);	
	}

	_XWindow::ShowWindow( show );
}

void XWindow_SmeltItem::MoveWindow( int X, int Y )
{			
	// Clipping from screen size
	if( m_pChargeGaugeWindow )
	{
		POINT pos = this->GetWindowPos();
		pos.x += 12;
		pos.y += 216;
		m_pChargeGaugeWindow->SetDrawPosition(pos);
	}

	_XWindow::MoveWindow( X, Y );
	
	m_ItemModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_ItemModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
}

void XWindow_SmeltItem::Draw( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return;
	_XWindow::Draw(pfocusobject);

	switch( m_ItemFlag )
	{
	case TEAR :
		{
			Draw_Tear();
		}
		break;
	case SCALES :
		{
			Draw_Scales();
		}
		break;
	}		

	g_XBaseFont->Flush();

	if( m_pChargeGaugeWindow )
		m_pChargeGaugeWindow->Draw();

	g_MainWindowManager.SetTopWindow( m_pChargeGaugeWindow );
}

void XWindow_SmeltItem::Draw_Scales()
{
	switch( m_ItemStep )
	{
	case ITEMSTEP_NONE :
		{
			_XDrawSolidBar( m_WindowPosition.x+8, m_WindowPosition.y+144, m_WindowPosition.x+172, m_WindowPosition.y+235, 0xFF1A1819 );
			_XDrawRectAngle( m_WindowPosition.x+8, m_WindowPosition.y+144, m_WindowPosition.x+172, m_WindowPosition.y+235, 0.0f, 0xFF000000 );

			g_XBaseFont->SetColor(_XSC_DEFAULT);
			//g_XBaseFont->Puts_SeparateAlign( m_WindowPosition.x+15, m_WindowPosition.y+152, m_strInfoText, 158, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
			if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
				g_XBaseFont->Puts_Separate( m_WindowPosition.x+54, m_WindowPosition.y+154, m_strInfoText, 114, 1.0, 3 );
			else
				g_XBaseFont->Puts_Separate( m_WindowPosition.x+54, m_WindowPosition.y+174, m_strInfoText, 114, 1.0, 3 );
			g_XBaseFont->Flush();

			m_slotImage->Draw();
		}
		break;
	case ITEMSTEP_READY :
		{
			if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
				m_IMEControl.MoveWindow(m_WindowPosition.x + 108, m_WindowPosition.y + 200);
			else
				m_IMEControl.MoveWindow(m_WindowPosition.x + 86, m_WindowPosition.y + 205);

			_XDrawSolidBar( m_WindowPosition.x+8, m_WindowPosition.y+144, m_WindowPosition.x+172, m_WindowPosition.y+235, 0xFF1A1819 );
			_XDrawRectAngle( m_WindowPosition.x+8, m_WindowPosition.y+144, m_WindowPosition.x+172, m_WindowPosition.y+235, 0.0f, 0xFF000000 );

			g_XBaseFont->SetColor(_XSC_DEFAULT);
			//g_XBaseFont->Puts_SeparateAlign( m_WindowPosition.x+15, m_WindowPosition.y+152, m_strInfoText, 158, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
			if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
				g_XBaseFont->Puts_Separate( m_WindowPosition.x+54, m_WindowPosition.y+154, m_strInfoText, 114, 1.0, 3 );
			else
				g_XBaseFont->Puts_Separate( m_WindowPosition.x+54, m_WindowPosition.y+174, m_strInfoText, 114, 1.0, 3 );

			m_slotImage->Draw();

			if( m_pUserItem )
			{
				DrawItem();
				
				g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()));
		
				if( _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
				{

					g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+32, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) );
					g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+47, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) );
				}
				else
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+40, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) );
				}

				g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+13, m_WindowPosition.y+173, m_pUserItem, FALSE);
			}

			g_XBaseFont->Flush();

			m_IMEControl.Draw();
		}
		break;
	case ITEMSTEP_CONFIRM :
		{// ITEMSTEP_CONFIRM �� ITEMSTEP_PROGRESS ���� �ҽ�
			g_XBaseFont->SetColor(_XSC_DEFAULT);
			g_XBaseFont->Puts_SeparateAlign( m_WindowPosition.x+15, m_WindowPosition.y + m_WindowSize.cy - 90, m_strInfoText, 158, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );

			if( m_pUserItem )
			{
				int nCurrentDrawPosY = 34;

				g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3432) );// [�븰 �ٸ���]

				TCHAR tempString[128];
				
				memset(tempString, 0, sizeof(tempString));
				
				nCurrentDrawPosY += 20;

				if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
				{
					TCHAR weaponname[128];

					g_XBaseFont->SetColor(_XSC_DEFAULT);
					if( _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
					{
						memset(weaponname, 0, sizeof(weaponname));
						sprintf( weaponname, "%s %s", _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()) , 
															 _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()) );
					}
					else
					{
						memset(weaponname, 0, sizeof(weaponname));
						sprintf( weaponname, "%s", _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID())); 
					}

					_snprintf(tempString, sizeof(tempString), "%s %s", _XGETINTERFACETEXT(ID_STRING_NEW_3433), weaponname);

					g_XBaseFont->Puts_Separate(m_WindowPosition.x+18, m_WindowPosition.y+nCurrentDrawPosY, tempString, 150, 1.0f, 3);

					nCurrentDrawPosY += 15*(g_XBaseFont->GetRowCount(tempString, 150));

					//����
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3434) );//���� ����:

					//�׸� ���� �׸���
					int nSmallFontDraw = 0;
					for( int i = 0; i < m_pUserItem->m_usCurMaxProtect; i++)
					{
						if( m_pUserItem->m_usCurProtect > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 108 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+108+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 108 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+108+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}

					g_XSmallFont->SetColor( _XSC_DEFAULT );
					g_XSmallFont->Print(m_WindowPosition.x+118+nSmallFontDraw, m_WindowPosition.y+nCurrentDrawPosY, 1.0f, "(%d/%d)", m_pUserItem->m_usCurProtect, m_pUserItem->m_usCurMaxProtect);
					g_XSmallFont->Flush();

					// �ٸ� �븰 ����
					memset(tempString, 0, sizeof(tempString));
					_snprintf( tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3435), m_nCount); // �ٸ� �븰 ���� : %d��
					
					nCurrentDrawPosY += 15;
					g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+18, m_WindowPosition.y+nCurrentDrawPosY, tempString, 150, _XFONT_ALIGNTYPE_LEFT, 1.0f, 3);

					//���� �� ����
					nCurrentDrawPosY += 15 * g_XBaseFont->GetRowCount(tempString, 150);

					memset(tempString, 0, sizeof(tempString));
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3436)); //���� �� ����:
					
					g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+18, m_WindowPosition.y+nCurrentDrawPosY, tempString, 150, _XFONT_ALIGNTYPE_LEFT, 1.0f, 3);

					//�׸� ���� �׸���
					nCurrentDrawPosY += 15 * (g_XBaseFont->GetRowCount(tempString, 150) - 1);
					nSmallFontDraw = 0;
					for( i = 0; i < m_pUserItem->m_usCurMaxProtect ; i++)
					{
						if( m_pUserItem->m_usCurProtect + m_nCount > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 120 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+120+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 120 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+120+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}

					g_XSmallFont->Print(m_WindowPosition.x+130+nSmallFontDraw, m_WindowPosition.y+nCurrentDrawPosY, 1.0f, "(%d/%d)", m_pUserItem->m_usCurProtect + m_nCount, m_pUserItem->m_usCurMaxProtect);

					g_XSmallFont->Flush();

					g_XBaseFont->Flush();
				}
				else
				{
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3433) ); //����:

					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()));
					if( _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
					{
						memset(tempString, 0, sizeof(tempString));
						sprintf( tempString, "%s %s", _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()) , 
															 _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()) );

						g_XBaseFont->PutsAlign(m_WindowPosition.x + 50, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					}
					else
					{
						memset(tempString, 0, sizeof(tempString));
						sprintf( tempString, "%s", _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID())); 
						
						g_XBaseFont->PutsAlign(m_WindowPosition.x + 50, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					}

					//����
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3434) ); //���� ����:
					
					nCurrentDrawPosY += 16;
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					//�׸� ���� �׸���
					int nSmallFontDraw = 0;
					for( int i = 0; i < m_pUserItem->m_usCurMaxProtect; i++)
					{
						if( m_pUserItem->m_usCurProtect > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 85 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+85+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 85 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+85+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}
					g_XSmallFont->SetColor( _XSC_DEFAULT );
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, "(%d/%d)", m_pUserItem->m_usCurProtect, m_pUserItem->m_usCurMaxProtect ); 
													

					g_XSmallFont->PutsAlign(m_WindowPosition.x + 95 + nSmallFontDraw, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					g_XSmallFont->Flush();	

					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3435), m_nCount); // �ٸ� �븰 ���� : %d��
					
					nCurrentDrawPosY += 16;
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x+18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					//���� �� ����
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3436) ); //���� �� ����:
					
					nCurrentDrawPosY += 16;
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					//�׸� ���� �׸���
					nSmallFontDraw = 0;
					for( i = 0; i < m_pUserItem->m_usCurMaxProtect ; i++)
					{
						if( m_pUserItem->m_usCurProtect + m_nCount > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 105 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+105+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 105 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+105+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}
					g_XSmallFont->SetColor( _XSC_DEFAULT );
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, "(%d/%d)", m_pUserItem->m_usCurProtect + m_nCount, m_pUserItem->m_usCurMaxProtect);

					g_XSmallFont->PutsAlign(m_WindowPosition.x + 115 + nSmallFontDraw, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					g_XSmallFont->Flush();

					g_XBaseFont->Flush();
				}
			}
		}
		break;
	case ITEMSTEP_PROGRESS :
		{// ITEMSTEP_CONFIRM �� ITEMSTEP_PROGRESS ���� �ҽ�
			g_XBaseFont->SetColor(_XSC_DEFAULT);
			g_XBaseFont->Puts_SeparateAlign( m_WindowPosition.x+15, m_WindowPosition.y + m_WindowSize.cy - 90, m_strInfoText, 150, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );

			if( m_pUserItem )
			{
				int nCurrentDrawPosY = 34;
				g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3432) );// [�븰 �ٸ���]

				TCHAR tempString[128];
				memset(tempString, 0, sizeof(tempString));

				if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
				{
					nCurrentDrawPosY += 20;
					g_XBaseFont->SetColor(_XSC_DEFAULT);

					TCHAR weaponname[128];
					if( _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
					{
						memset(weaponname, 0, sizeof(weaponname));
						sprintf(weaponname, "%s %s", _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()) , 
															 _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()));
					}
					else
					{
						memset(weaponname, 0, sizeof(weaponname));
						sprintf(weaponname, "%s", _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()));
					}

					_snprintf(tempString, sizeof(tempString), "%s %s",  _XGETINTERFACETEXT(ID_STRING_NEW_3433), weaponname); //����:
					g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x + 18, m_WindowPosition.y+nCurrentDrawPosY, tempString, 150, _XFONT_ALIGNTYPE_LEFT, 1.0f, 3);

					//����
					nCurrentDrawPosY += 15 * g_XBaseFont->GetRowCount(tempString, 150);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y+nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3434));

					//�׸� ���� �׸���
					int nSmallFontDraw = 0;
					for( int i = 0; i < m_pUserItem->m_usCurMaxProtect; i++)
					{
						if( m_pUserItem->m_usCurProtect > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 108 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+108+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 108 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+108+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}

					g_XSmallFont->SetColor( _XSC_DEFAULT );
					g_XSmallFont->PrintAlign(m_WindowPosition.x+118+nSmallFontDraw, m_WindowPosition.y+nCurrentDrawPosY, 1.0f, 
						_XFONT_ALIGNTYPE_LEFT, "(%d/%d)", m_pUserItem->m_usCurProtect, m_pUserItem->m_usCurMaxProtect);

					memset(tempString, 0, sizeof(tempString));
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3435), m_nCount); // �ٸ� �븰 ���� : %d��
					
					nCurrentDrawPosY += 15;
					g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+18, m_WindowPosition.y+nCurrentDrawPosY, tempString, 150, 
						_XFONT_ALIGNTYPE_LEFT, 1.0f, 3);

					//���� �� ����
					nCurrentDrawPosY += 15 * g_XBaseFont->GetRowCount(tempString, 150);

					memset(tempString, 0, sizeof(tempString));
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3436));

					g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+18, m_WindowPosition.y+nCurrentDrawPosY, tempString,
						150, _XFONT_ALIGNTYPE_LEFT, 1.0f, 3);	//���� �� ����

					nCurrentDrawPosY += 15 + (g_XBaseFont->GetRowCount(tempString, 150) - 1);

					//�׸� ���� �׸���
					nSmallFontDraw = 0;
					for( i = 0; i < m_pUserItem->m_usCurMaxProtect ; i++)
					{
						if( m_pUserItem->m_usCurProtect + m_nCount > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 120 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+120+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 120 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+120+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}

					g_XSmallFont->Print(m_WindowPosition.x+130+nSmallFontDraw, m_WindowPosition.y+nCurrentDrawPosY, 1.0f, "(%d/%d)",
						m_pUserItem->m_usCurProtect + m_nCount, m_pUserItem->m_usCurMaxProtect);

					g_XSmallFont->Flush();
					g_XBaseFont->Flush();
				}
				else
				{
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3433) ); //����:
					
					nCurrentDrawPosY += 20;
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()));
					if( _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
					{
						memset(tempString, 0, sizeof(tempString));
						sprintf( tempString, "%s %s", _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()) , 
															 _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()) );

						g_XBaseFont->PutsAlign(m_WindowPosition.x + 50, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					}
					else
					{
						memset(tempString, 0, sizeof(tempString));
						sprintf( tempString, "%s", _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID())); 
						
						g_XBaseFont->PutsAlign(m_WindowPosition.x + 50, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					}

					//����
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3434) ); //���� ����:
					
					nCurrentDrawPosY += 16;
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					//�׸� ���� �׸���
					int nSmallFontDraw = 0;
					for( int i = 0; i < m_pUserItem->m_usCurMaxProtect; i++)
					{
						if( m_pUserItem->m_usCurProtect > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 85 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+85+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 85 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+85+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}
					g_XSmallFont->SetColor( _XSC_DEFAULT );
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, "(%d/%d)", m_pUserItem->m_usCurProtect, m_pUserItem->m_usCurMaxProtect ); 
													

					g_XSmallFont->PutsAlign(m_WindowPosition.x + 95 + nSmallFontDraw, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					g_XSmallFont->Flush();	

					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3435), m_nCount); // �ٸ� �븰 ���� : %d��
					
					nCurrentDrawPosY += 16;
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x+18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					//���� �� ����
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3436) ); //���� �� ����:
					
					nCurrentDrawPosY += 16;
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					//�׸� ���� �׸���
					nSmallFontDraw = 0;
					for( i = 0; i < m_pUserItem->m_usCurMaxProtect ; i++)
					{
						if( m_pUserItem->m_usCurProtect + m_nCount > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 105 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+105+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 105 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+105+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}
					g_XSmallFont->SetColor( _XSC_DEFAULT );
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, "(%d/%d)", m_pUserItem->m_usCurProtect + m_nCount, m_pUserItem->m_usCurMaxProtect);

					g_XSmallFont->PutsAlign(m_WindowPosition.x + 115 + nSmallFontDraw, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					g_XSmallFont->Flush();
				}
			}

			g_XBaseFont->Flush();
		}
		break;
	}
}

void XWindow_SmeltItem::Draw_Tear()
{
	switch( m_ItemStep )
	{
	case ITEMSTEP_NONE :
		{
			_XDrawSolidBar( m_WindowPosition.x+8, m_WindowPosition.y+144, m_WindowPosition.x+172, m_WindowPosition.y+235, 0xFF1A1819 );
			_XDrawRectAngle( m_WindowPosition.x+8, m_WindowPosition.y+144, m_WindowPosition.x+172, m_WindowPosition.y+235, 0.0f, 0xFF000000 );

			g_XBaseFont->SetColor(_XSC_DEFAULT);
			//g_XBaseFont->Puts_SeparateAlign( m_WindowPosition.x+15, m_WindowPosition.y+152, m_strInfoText, 158, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
			if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
				g_XBaseFont->Puts_Separate( m_WindowPosition.x+54, m_WindowPosition.y+154, m_strInfoText, 114, 1.0, 3 );
			else
				g_XBaseFont->Puts_Separate( m_WindowPosition.x+54, m_WindowPosition.y+174, m_strInfoText, 114, 1.0, 3 );

			m_slotImage->Draw();

			g_XBaseFont->Flush();
		}
		break;
	case ITEMSTEP_READY :
		{
			_XDrawSolidBar( m_WindowPosition.x+8, m_WindowPosition.y+144, m_WindowPosition.x+172, m_WindowPosition.y+235, 0xFF1A1819 );
			_XDrawRectAngle( m_WindowPosition.x+8, m_WindowPosition.y+144, m_WindowPosition.x+172, m_WindowPosition.y+235, 0.0f, 0xFF000000 );

			g_XBaseFont->SetColor(_XSC_DEFAULT);
			//g_XBaseFont->Puts_SeparateAlign( m_WindowPosition.x+15, m_WindowPosition.y+152, m_strInfoText, 158, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
			if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
				g_XBaseFont->Puts_Separate( m_WindowPosition.x+54, m_WindowPosition.y+148, m_strInfoText, 114, 1.0, 3 );
			else
				g_XBaseFont->Puts_Separate( m_WindowPosition.x+54, m_WindowPosition.y+174, m_strInfoText, 114, 1.0, 3 );

			m_slotImage->Draw();

			if( m_pUserItem )
			{
				DrawItem();
				
				g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()));
		
				if( _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
				{

					g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+32, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) );
					g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+47, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) );
				}
				else
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+40, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) );
				}

				g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+13, m_WindowPosition.y+173, m_pUserItem, FALSE);
			}

			g_XBaseFont->Flush();
		}
		break;
	case ITEMSTEP_CONFIRM :
		{// ITEMSTEP_CONFIRM �� ITEMSTEP_PROGRESS ���� �ҽ�
			g_XBaseFont->SetColor(_XSC_DEFAULT);
			g_XBaseFont->Puts_SeparateAlign( m_WindowPosition.x+15, m_WindowPosition.y + m_WindowSize.cy - 80, m_strInfoText, 158, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );

			if( m_pUserItem )
			{
				int nCurrentDrawPosY = 34;

				TCHAR tempString[128];	
				memset(tempString, 0, sizeof(tempString));

				if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x +91, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3437) ); //[��� �ٸ���]

					nCurrentDrawPosY += 20;
					g_XBaseFont->SetColor(_XSC_DEFAULT);

					TCHAR weaponname[128];
					if( _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
					{
						memset(weaponname, 0, sizeof(weaponname));
						_snprintf(weaponname, sizeof(weaponname), "%s %s", _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()) , 
															 _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()));
					}
					else
					{
						memset(weaponname, 0, sizeof(weaponname));
						_snprintf(weaponname, sizeof(weaponname), "%s", _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID())); 
					}

					_snprintf(tempString, sizeof(tempString), "%s %s", _XGETINTERFACETEXT(ID_STRING_NEW_3433), weaponname);
					g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+18, m_WindowPosition.y+nCurrentDrawPosY, tempString, 150, _XFONT_ALIGNTYPE_LEFT, 1.0f, 3);

					//����					
					nCurrentDrawPosY += 15 * g_XBaseFont->GetRowCount(tempString, 150);
					g_XBaseFont->Print(m_WindowPosition.x+18, m_WindowPosition.y+nCurrentDrawPosY, 1.0f, _XGETINTERFACETEXT(ID_STRING_NEW_3434));

					//�׸� ���� �׸���
					int nSmallFontDraw = 0;
					for( int i = 0; i < m_pUserItem->m_usCurMaxProtect; i++)
					{
						if( m_pUserItem->m_usCurProtect > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+108+(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+108+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+108+(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+108+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}
					g_XSmallFont->SetColor( _XSC_DEFAULT );
					g_XSmallFont->Print(m_WindowPosition.x+118+nSmallFontDraw, m_WindowPosition.y+nCurrentDrawPosY, 1.0f, "(%d/%d)",  m_pUserItem->m_usCurProtect, m_pUserItem->m_usCurMaxProtect);

					// �ٸ� �븰 ����
					nCurrentDrawPosY += 15;

					memset(tempString, 0, sizeof(tempString));
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3438));

					g_XBaseFont->Puts_Separate(m_WindowPosition.x+18, m_WindowPosition.y+nCurrentDrawPosY, tempString, 150, 1.0f, 3);

					//���� �� ����
					nCurrentDrawPosY += 15 * g_XBaseFont->GetRowCount(tempString, 150);

					memset(tempString, 0, sizeof(tempString));
					sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3436)); // ���� �� ����:
					
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->Puts_Separate(m_WindowPosition.x+18, m_WindowPosition.y+nCurrentDrawPosY, tempString, 150, 1.0f, 3);

					//�׸� ���� �׸���
					nCurrentDrawPosY += 15 * (g_XBaseFont->GetRowCount(tempString, 150) - 1);
					nSmallFontDraw = 0;

					//����� ������ 2�� �������ִ�
					for( i = 0; i < m_pUserItem->m_usCurMaxProtect + 2; i++)
					{
						if( m_pUserItem->m_usCurProtect > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 120 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+120+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 120 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+120+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}

					g_XSmallFont->Print(m_WindowPosition.x+130+nSmallFontDraw, m_WindowPosition.y+nCurrentDrawPosY, 1.0f, "(%d/%d)",
						m_pUserItem->m_usCurProtect, m_pUserItem->m_usCurMaxProtect + 2);

					g_XSmallFont->Flush();

					g_XBaseFont->Flush();
				}
				else
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x +50, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3437) ); //[��� �ٸ���]

					TCHAR tempString[128];
					
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3433) ); //����:
					
					nCurrentDrawPosY += 20;
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()));
					if( _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
					{
						memset(tempString, 0, sizeof(tempString));
						sprintf( tempString, "%s %s", _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()) , 
															 _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()) );

						g_XBaseFont->PutsAlign(m_WindowPosition.x + 50, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					}
					else
					{
						memset(tempString, 0, sizeof(tempString));
						sprintf( tempString, "%s", _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID())); 
						
						g_XBaseFont->PutsAlign(m_WindowPosition.x + 50, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					}

					//����
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3434) ); // ���� ����:
					
					nCurrentDrawPosY += 16;
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					//�׸� ���� �׸���
					int nSmallFontDraw = 0;
					for( int i = 0; i < m_pUserItem->m_usCurMaxProtect; i++)
					{
						if( m_pUserItem->m_usCurProtect > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 85 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+85+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 85 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+85+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}
					g_XSmallFont->SetColor( _XSC_DEFAULT );
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, "(%d/%d)", m_pUserItem->m_usCurProtect, m_pUserItem->m_usCurMaxProtect);

					g_XSmallFont->PutsAlign(m_WindowPosition.x + 95 + nSmallFontDraw, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					g_XSmallFont->Flush();

					memset(tempString, 0, sizeof(tempString));
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3438) ); // �ٸ� ��� ����: 1 ��
					
					nCurrentDrawPosY += 16;
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					//���� �� ����
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3436) ); // ���� �� ����:
					
					nCurrentDrawPosY += 16;
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					//�׸� ���� �׸���
					nSmallFontDraw = 0;
					//����� ������ 2�� �������ִ�
					for( i = 0; i < m_pUserItem->m_usCurMaxProtect + 2; i++)
					{
						if( m_pUserItem->m_usCurProtect > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 105 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+105+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 105 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+105+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}
					g_XSmallFont->SetColor( _XSC_DEFAULT );
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, "(%d/%d)", m_pUserItem->m_usCurProtect, m_pUserItem->m_usCurMaxProtect + 2 );

					g_XSmallFont->PutsAlign(m_WindowPosition.x + 115 + nSmallFontDraw, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					g_XSmallFont->Flush();
				}
			}

			g_XBaseFont->Flush();
		}
		break;
	case ITEMSTEP_PROGRESS :
		{// ITEMSTEP_CONFIRM �� ITEMSTEP_PROGRESS ���� �ҽ�
			g_XBaseFont->SetColor(_XSC_DEFAULT);
			g_XBaseFont->Puts_SeparateAlign( m_WindowPosition.x+15, m_WindowPosition.y + m_WindowSize.cy - 80, m_strInfoText, 158, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );

			if( m_pUserItem )
			{
				int nCurrentDrawPosY = 34;

				TCHAR tempString[128];
				memset(tempString, 0, sizeof(tempString));

				if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x +91, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3437) ); //[��� �ٸ���]
					
					nCurrentDrawPosY += 20;

					g_XBaseFont->SetColor(_XSC_DEFAULT);

					TCHAR weaponname[128];
					if( _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
					{
						memset(weaponname, 0, sizeof(weaponname));
						_snprintf(weaponname, sizeof(weaponname), "%s %s", _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()) , 
															 _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()));
					}
					else
					{
						memset(weaponname, 0, sizeof(weaponname));
						_snprintf(weaponname, sizeof(weaponname), "%s", _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID())); 
					}

					_snprintf(tempString, sizeof(tempString), "%s %s", _XGETINTERFACETEXT(ID_STRING_NEW_3433), weaponname); //����:
					g_XBaseFont->Puts_Separate(m_WindowPosition.x+18, m_WindowPosition.y+nCurrentDrawPosY, tempString, 150, 1.0f, 3);

					// ���� ����:
					nCurrentDrawPosY += 15 * g_XBaseFont->GetRowCount(tempString, 150);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3434));

					//�׸� ���� �׸���
					int nSmallFontDraw = 0;
					for( int i = 0; i < m_pUserItem->m_usCurMaxProtect; i++)
					{
						if( m_pUserItem->m_usCurProtect > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 108 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+108+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 108 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+108+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}

					g_XSmallFont->SetColor( _XSC_DEFAULT );
					g_XSmallFont->Print(m_WindowPosition.x+118+nSmallFontDraw, m_WindowPosition.y+nCurrentDrawPosY, 1.0f, "(%d/%d)",  m_pUserItem->m_usCurProtect, m_pUserItem->m_usCurMaxProtect);

					// �ٸ� ��� ����: 1 ��
					memset(tempString, 0, sizeof(tempString));
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3438)); // �ٸ� ��� ����: 1 ��
					
					nCurrentDrawPosY += 15;
					g_XBaseFont->Puts_Separate(m_WindowPosition.x+18, m_WindowPosition.y+nCurrentDrawPosY, tempString, 150, 1.0f, 3);

					//���� �� ����
					nCurrentDrawPosY += 15 * g_XBaseFont->GetRowCount(tempString, 150);

					memset(tempString, 0, sizeof(tempString));
					_snprintf(tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_NEW_3436)); // ���� �� ����:
					
					g_XBaseFont->Puts_Separate(m_WindowPosition.x+18, m_WindowPosition.y+nCurrentDrawPosY, tempString, 150, 1.0f, 3);

					//�׸� ���� �׸���
					nCurrentDrawPosY += 15 * (g_XBaseFont->GetRowCount(tempString, 150)-1);
					nSmallFontDraw = 0;

					//����� ������ 2�� �������ִ�
					for( i = 0; i < m_pUserItem->m_usCurMaxProtect + 2; i++)
					{
						if( m_pUserItem->m_usCurProtect > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 120 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+120+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 120 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+120+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}

					g_XSmallFont->Print(m_WindowPosition.x+130+nSmallFontDraw, m_WindowPosition.y+nCurrentDrawPosY, 1.0f, "(%d/%d)",  m_pUserItem->m_usCurProtect, m_pUserItem->m_usCurMaxProtect + 2);
					g_XSmallFont->Flush();
					g_XBaseFont->Flush();
				}
				else
				{
					g_XBaseFont->PutsAlign(m_WindowPosition.x +50, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3437) ); //[��� �ٸ���]

					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3433) ); //����:
					
					nCurrentDrawPosY += 20;
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()));
					if( _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
					{
						memset(tempString, 0, sizeof(tempString));
						sprintf( tempString, "%s %s", _XGameItem::GetItemNick(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()) , 
															 _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID()) );

						g_XBaseFont->PutsAlign(m_WindowPosition.x + 50, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					}
					else
					{
						memset(tempString, 0, sizeof(tempString));
						sprintf( tempString, "%s", _XGameItem::GetItemName(m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID())); 
						
						g_XBaseFont->PutsAlign(m_WindowPosition.x + 50, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					}

					//����
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3434) ); // ���� ����:
					
					nCurrentDrawPosY += 16;
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					//�׸� ���� �׸���
					int nSmallFontDraw = 0;
					for( int i = 0; i < m_pUserItem->m_usCurMaxProtect; i++)
					{
						if( m_pUserItem->m_usCurProtect > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 85 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+85+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 85 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+85+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}
					g_XSmallFont->SetColor( _XSC_DEFAULT );
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, "(%d/%d)", m_pUserItem->m_usCurProtect, m_pUserItem->m_usCurMaxProtect);

					g_XSmallFont->PutsAlign(m_WindowPosition.x + 95 + nSmallFontDraw, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					g_XSmallFont->Flush();

					memset(tempString, 0, sizeof(tempString));
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3438) ); // �ٸ� ��� ����: 1 ��
					
					nCurrentDrawPosY += 16;
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					//���� �� ����
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3436) ); // ���� �� ����:
					
					nCurrentDrawPosY += 16;
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

					//�׸� ���� �׸���
					nSmallFontDraw = 0;
					//����� ������ 2�� �������ִ�
					for( i = 0; i < m_pUserItem->m_usCurMaxProtect + 2; i++)
					{
						if( m_pUserItem->m_usCurProtect > i )
						{
							_XDrawSolidBar( m_WindowPosition.x+ 105 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+105+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
						}
						else
						{
							_XDrawSolidBar( m_WindowPosition.x+ 105 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
								m_WindowPosition.x+105+2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
						}
						nSmallFontDraw = (i*3);
					}
					g_XSmallFont->SetColor( _XSC_DEFAULT );
					memset(tempString, 0, sizeof(tempString));
					sprintf( tempString, "(%d/%d)", m_pUserItem->m_usCurProtect, m_pUserItem->m_usCurMaxProtect + 2 );

					g_XSmallFont->PutsAlign(m_WindowPosition.x + 115 + nSmallFontDraw, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
					g_XSmallFont->Flush();
				}
			}

			g_XBaseFont->Flush();
		}
		break;
	}
}

void XWindow_SmeltItem::DrawInterface(  ItemStep Step )
{
	switch( Step )
	{
	case ITEMSTEP_NONE :
	case ITEMSTEP_READY :
		{
			m_pSocketItemLeftView->ShowWindow( TRUE );
			m_pSocketItemRightView->ShowWindow( TRUE );
			m_pLeftTopView->ShowWindow( FALSE );
			m_pRightTopView->ShowWindow( FALSE );
			m_pLeftDownView->ShowWindow( FALSE );
			m_pRightDownView->ShowWindow( FALSE );
		}
		break;
	case ITEMSTEP_CONFIRM :
	case ITEMSTEP_PROGRESS :
		{
			m_pSocketItemLeftView->ShowWindow( FALSE );
			m_pSocketItemRightView->ShowWindow( FALSE );
			m_pLeftTopView->ShowWindow( TRUE );
			m_pRightTopView->ShowWindow( TRUE );
			m_pLeftDownView->ShowWindow( TRUE );
			m_pRightDownView->ShowWindow( TRUE );
		}
		break;
	}
}

BOOL XWindow_SmeltItem::Process( _XGUIObject*& pfocusobject )
{	
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	//�ý��� �޽��� â�� ��ŷüũ�� ����
	_XWindow_SystemMsgDefault* systemWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);
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
		 // �κ��丮�� �׷����� ���� �� �κ��丮 ���� �ý��������츦 ��ŷ
		 if( g_pInventory_Window && g_pInventory_Window->GetShowStatus() )
			 systemWindow->SetDockingMode(TRUE);	
		 else
			 systemWindow->SetDockingMode(FALSE);	
	 }

	if(mousestate && !pfocusobject )
	{
		if( mousestate->bButton[0])
		{
			// mouse left down
			if(CheckMousePosition())
			{
				// Focus
				if(scrnpos->x > m_WindowPosition.x+12 && scrnpos->x < m_WindowPosition.x+44)
				{
					if(scrnpos->z > m_WindowPosition.y+172 && scrnpos->z < m_WindowPosition.y+203)
					{
						m_WindowMouseState = _XW_STATE_NONE;
					}
				}
				else
				{
					m_WindowMouseState = _XW_STATE_LEFTCLICK; 
				}

				if(g_pInventory_Window && g_pInventory_Window->GetShowStatus() &&	// �κ��丮���� �巡�� ���� ���
					g_pInventory_Window->m_bHaveMouseSlot && g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART &&
					g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sID() > 0  && 
					g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART )
				{
					if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_cType()) )
					{
						_XGI_WeaponItem_Property* tempWeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_cType());
						if( _XGameItem::GetNature( tempWeaponItem[g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_sID()].cType, 
												   tempWeaponItem[g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_sID()].sID) <= 0 ) //Author : ����� breif : ������ 0 �̸� ��� �Ұ�. //Date : 08/03/04
						{
							MessagePrint(6); // ����� �� ���� ���� �Դϴ�.
							return FALSE;
						}

						if( m_pUserItem )		// �̹� �������� �÷��� �ִ� ���
						{
							MessagePrint(5); //��ǰ�� ��ü�� �� �����ϴ�
							return FALSE;
						}
						else
						{
#ifdef _XDEF_PERIODITEM_20061027
							if(ProcessSmeltCashItem(g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber))
							{
								// �Ⱓ�� �������̴�. - message handler���� ó��
								return TRUE;
							}
							else
							{
								return ProcessSmelt();
							}
#else
							//������ Item ���� �˻�
							if( CheckWeaponGrade(g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber) == 1 )
							{
								m_pUserItem = &g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber];
								m_nUserIndex = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber; //���Թ�ȣ ����
								g_pInventory_Window->m_UsedLifeItemIndex = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber; //���Թ�ȣ ����
								SetItemFlag( m_ItemFlag, ITEMSTEP_READY);
								g_pInventory_Window->m_bHaveMouseSlot = FALSE;
								g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();	
							}
							else
								return FALSE;

							return TRUE;
#endif
						}
					}
					else
					{
						MessagePrint(0); //�̰��� ���Ⱑ �ƴմϴ�.	 					
						return FALSE;
					}
				}
				
				if( g_pInventory_Window && g_pInventory_Window->GetShowStatus())
				{
					g_pInventory_Window->m_bDrawSelectedItem = FALSE;
					if( g_pDefaultTooltip_Window && g_pDefaultTooltip_Window->GetShowStatus() )
						g_pDefaultTooltip_Window->ShowWindow( FALSE );
				}
			}
			else
			{
				if( g_pDefaultTooltip_Window )
				{
					if( g_pDefaultTooltip_Window->GetShowStatus() )						
						g_pDefaultTooltip_Window->ShowWindow( FALSE );
				}
			}
		}

		switch( m_ItemFlag )
		{
		case TEAR:
			{
				Process_Tear();		
			}
			break;
		case SCALES:
			{
				Process_Scales();		
			}
			break;
		}
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void XWindow_SmeltItem::Process_Tear()
{
	switch( m_ItemStep )
	{
	case ITEMSTEP_NONE :
		{
			if( m_pUserItem && m_pUserItem->Get_m_sID() > 0 )
			{	
				MouseState* mousestate = gpInput->GetMouseState();
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

				if(mousestate)
				{
					if(CheckMousePosition())
					{
						if(scrnpos->x > m_WindowPosition.x+12 && scrnpos->x < m_WindowPosition.x+44)
						{
							if(scrnpos->z > m_WindowPosition.y+172 && scrnpos->z < m_WindowPosition.y+203)
							{
								if( g_pDefaultTooltip_Window )
								{							
									if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_INCHANT, m_nUserIndex, m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
									{
										g_pDefaultTooltip_Window->ShowWindow( TRUE );
									}
								}
							}
						}
						else
						{
							if( g_pDefaultTooltip_Window->GetShowStatus() )						
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
				}
			}
		}
		break;
	case ITEMSTEP_READY :
		{
			if( m_pUserItem && m_pUserItem->Get_m_sID() > 0 )
			{	
				MouseState* mousestate = gpInput->GetMouseState();
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

				if(mousestate)
				{
					if(CheckMousePosition())
					{
						if(scrnpos->x > m_WindowPosition.x+12 && scrnpos->x < m_WindowPosition.x+44)
						{
							if(scrnpos->z > m_WindowPosition.y+172 && scrnpos->z < m_WindowPosition.y+203)
							{
								if( g_pDefaultTooltip_Window )
								{							
									if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_INCHANT, m_nUserIndex, m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
									{
										g_pDefaultTooltip_Window->ShowWindow( TRUE );
									}
								}
							}
						}
						else
						{
							if( g_pDefaultTooltip_Window->GetShowStatus() )						
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
				}
			}
			
		}
		break;
	case ITEMSTEP_CONFIRM :

		break;
	case ITEMSTEP_PROGRESS :
		{
		}
		break;
	}
}

void XWindow_SmeltItem::Process_Scales()
{
	switch( m_ItemStep )
	{
	case ITEMSTEP_NONE :
		{
			if( m_pUserItem && m_pUserItem->Get_m_sID() > 0 )
			{	
				MouseState* mousestate = gpInput->GetMouseState();
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

				if(mousestate)
				{
					if(CheckMousePosition())
					{
						if(scrnpos->x > m_WindowPosition.x+12 && scrnpos->x < m_WindowPosition.x+44)
						{
							if(scrnpos->z > m_WindowPosition.y+172 && scrnpos->z < m_WindowPosition.y+203)
							{
								if( g_pDefaultTooltip_Window )
								{							
									if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_INCHANT, m_nUserIndex, m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
									{
										g_pDefaultTooltip_Window->ShowWindow( TRUE );
									}
								}
							}
						}
						else
						{
							if( g_pDefaultTooltip_Window->GetShowStatus() )						
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
				}
			}
		}
		break;
	case ITEMSTEP_READY :
		{
			if( m_pUserItem && m_pUserItem->Get_m_sID() > 0 )
			{	
				MouseState* mousestate = gpInput->GetMouseState();
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

				if(mousestate)
				{
					if(CheckMousePosition())
					{
						if(scrnpos->x > m_WindowPosition.x+12 && scrnpos->x < m_WindowPosition.x+44)
						{
							if(scrnpos->z > m_WindowPosition.y+172 && scrnpos->z < m_WindowPosition.y+203)
							{
								if( g_pDefaultTooltip_Window )
								{							
									if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_INCHANT, m_nUserIndex, m_pUserItem->Get_m_cType(), m_pUserItem->Get_m_sID() ) )
									{
										g_pDefaultTooltip_Window->ShowWindow( TRUE );
									}
								}
							}
						}
						else
						{
							if( g_pDefaultTooltip_Window->GetShowStatus() )						
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
				}
			}

			TCHAR tempStr[64];
			memset( tempStr, 0, sizeof(tempStr));
			MouseState* mousestate = gpInput->GetMouseState();
			if(mousestate)
			{
				if(mousestate->bButton[0])
				{
					if(CheckIMEBoxArea())
					{
						m_IMEControl.SetFocus();
					}
				}
			}

			strcpy(tempStr, m_IMEControl.GetText());
			m_nCount = atoi(tempStr);

			//�븰 �ִ� �������� ũ�ٸ� Ȯ��â�� �ѱ��� �ʴ´�
			if( m_pUserItem && m_pUserItem->m_usCurMaxProtect < m_pUserItem->m_usCurProtect + m_nCount ) 
			{
				m_pConfirmButton->EnableWindow(FALSE);
				break;
			}

			if( m_nCount >= 1 && m_nCount <= 9 )
				m_pConfirmButton->EnableWindow(TRUE);
			else
				m_pConfirmButton->EnableWindow(FALSE);
		}
		break;
	case ITEMSTEP_CONFIRM :
		break;
	case ITEMSTEP_PROGRESS :
		{
		}
		break;
	}
}

BOOL XWindow_SmeltItem::CheckIMEBoxArea()
{
	//IME �ڽ� üũ IME �� �׷��� ��ġ���� ���� ���� 10
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	POINT point = m_IMEControl.GetWindowPosition();
	SIZE size = m_IMEControl.GetWindowSize();

	if(scrnpos->x > point.x - 10 &&
		scrnpos->x < point.x + size.cx + 10 &&
		scrnpos->z > point.y - 10 &&
		scrnpos->z < point.y + size.cy + 10)
		return TRUE;

	return FALSE;
}

int XWindow_SmeltItem::CheckWeaponGrade(int searchslot)
{
	int grade = _XGameItem::GetItemGrade(g_pLocalUser->m_UserItemList[searchslot].Get_m_cType(), g_pLocalUser->m_UserItemList[searchslot].Get_m_sID());
	switch(grade) 
	{
	case 0:
	case 10:
	case 20:
	case 1 :
	case 2 :
	case 5 :
	case 50 :	// Weapon3 �⺸�� ������
	case 51 :	// Weapon3 �⺸�� ������
	case 30 :	// ���� �������� �߰��� ����
		{
				return 1;
		}
		break;
	case 3 :
	case 4 :
	case 6 :
		{
			MessagePrint(4); //�ش� �������� ���⿡ ����� �� �����ϴ�.
			return -1;
		}
	}
	return -1;
}

void XWindow_SmeltItem::MessagePrint(int msgindex)
{
	TCHAR messagestring[256];
	memset( messagestring, 0, sizeof(TCHAR)*256 );
	
	switch(msgindex) 
	{
	case 0:
		{
			g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
			
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2105) ); //�̰��� ���Ⱑ �ƴմϴ�.
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, messagestring, TRUE);		
		}
		break;
	case 1:
		{
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3439) ); // ���̻� �븰�� �ٸ� �� �����ϴ�.
			g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		}
		break;
	case 2:
		{
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3440) ); // ���̻� ��縦 �ٸ� �� �����ϴ�.
			g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		}
		break;
	case 3:
		{
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3441) ); // �븰�� �����մϴ�.
			g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		}
		break;
	case 4:
		{
			g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
			
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3442) ); // �ش� �������� ���⿡ ����� �� �����ϴ�.
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, messagestring, TRUE);		
		}
		break;
	case 5:
		{
			g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);

			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR9)); //��ǰ�� ��ü�� �� �����ϴ�
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, messagestring, TRUE);		
		}
		break;
	case 6:
		sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3794) ); // ����� �� ���� ���� �Դϴ�.
		g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		break;
	case 7:
		break;
	}	
	
	g_pInventory_Window->m_bHaveMouseSlot = FALSE;
	g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
}

void XWindow_SmeltItem::SetItemFlag( const ItemFlag flag, const ItemStep step )
{
	m_ItemStep = step;
	m_ItemFlag = flag;

	switch( m_ItemFlag )
	{
	case TEAR:
		{
			RebuildItemInfo_Tear();
			SetItemStep_Tear();
		}
		break;
	case SCALES:
		{
			RebuildItemInfo_Scales();
			SetItemStep_Scales();
		}
		break;
	}
}

void XWindow_SmeltItem::RebuildItemInfo_Scales()
{
	memset(m_strInfoText, 0, sizeof(TCHAR)*128);

	switch( m_ItemStep )
	{
	case ITEMSTEP_NONE :
		strcpy( m_strInfoText, _XGETINTERFACETEXT(ID_STRING_NEW_3424) ); //�븰�� �ٸ� ���⸦ ����� �ֽʽÿ�.
		break;
	case ITEMSTEP_READY :
		strcpy( m_strInfoText, _XGETINTERFACETEXT(ID_STRING_NEW_3425) ); //���⿡ �ٸ� �븰�� ������ �Է��� �ֽʽÿ�.     ��
		break;
	case ITEMSTEP_CONFIRM :
		strcpy( m_strInfoText, _XGETINTERFACETEXT(ID_STRING_NEW_3426) ); //�� ���� ��� �븰�� �ٸ��ϴ�. ���� �Ͻðڽ��ϱ�?
		break;
	case ITEMSTEP_PROGRESS :
		strcpy( m_strInfoText, _XGETINTERFACETEXT(ID_STRING_NEW_3427) ); //�븰�� �ٸ��� ���Դϴ�.
		break;
	}
}

void XWindow_SmeltItem::RebuildItemInfo_Tear()
{
	memset(m_strInfoText, 0, sizeof(TCHAR)*128);

	switch( m_ItemStep )
	{
	case ITEMSTEP_NONE :
		strcpy( m_strInfoText, _XGETINTERFACETEXT(ID_STRING_NEW_3428) ); //��縦 �ٸ� ���⸦ ����� �ֽʽÿ�.
		break;
	case ITEMSTEP_READY :
		strcpy( m_strInfoText, _XGETINTERFACETEXT(ID_STRING_NEW_3429) ); //���� �� ���⿡ �� �ѹ��� �ٸ� �� �ֽ��ϴ�.\n�ش� ���⿡ ��縦 �ٸ��ðڽ��ϱ�?
		break;
	case ITEMSTEP_CONFIRM :
		strcpy( m_strInfoText, _XGETINTERFACETEXT(ID_STRING_NEW_3430) ); //�� ������ ��縦 �ٸ��ϴ�. ���� �Ͻðڽ��ϱ�?
		break;
	case ITEMSTEP_PROGRESS :
		strcpy( m_strInfoText,  _XGETINTERFACETEXT(ID_STRING_NEW_3431) ); //����� �ٸ��� ���Դϴ�.
		break;
	}
}

void XWindow_SmeltItem::SetItemStep_Tear()
{
	switch( m_ItemStep )
	{
	case ITEMSTEP_NONE :
		{
			m_ltSlotNumber.clear();

			DrawInterface(ITEMSTEP_NONE );
			m_slotImage->ShowWindow(TRUE);

			m_nCount = 0;

			m_pUserItem = NULL;
			m_UserStuff[0].Reset();
			m_UserStuff[1].Reset();
			
			m_pConfirmButton->ShowWindow(FALSE);
			m_pConfirmButton->EnableWindow(FALSE);
			m_pCancleButton->EnableWindow(TRUE);

			if( g_pInventory_Window )
			{
				g_pInventory_Window->ResetSocketIndexList();
				g_pInventory_Window->m_UsedLifeItemIndex = 0;
			}

			if( m_pChargeGaugeWindow )
			{
				m_pChargeGaugeWindow->ShowWindow(FALSE);
				m_pChargeGaugeWindow = NULL;
			}
		}
		break;
	case ITEMSTEP_READY :
		{
			m_ltSlotNumber.clear();

			DrawInterface(ITEMSTEP_READY );
			m_slotImage->ShowWindow(TRUE);

			m_nCount = 0;

			m_pConfirmButton->ShowWindow(TRUE);
			m_pConfirmButton->EnableWindow(TRUE);
			m_pCancleButton->EnableWindow(TRUE);

			if( m_pChargeGaugeWindow )
			{
				m_pChargeGaugeWindow->ShowWindow(FALSE);
				m_pChargeGaugeWindow = NULL;
			}
		}
		break;
	case ITEMSTEP_CONFIRM :
		{
			DrawInterface( ITEMSTEP_CONFIRM );
			m_slotImage->ShowWindow(FALSE);

			m_nCount = 2;

			m_pConfirmButton->ShowWindow(TRUE);
			m_pConfirmButton->EnableWindow(TRUE);
			m_pCancleButton->EnableWindow(TRUE);

			if( m_pChargeGaugeWindow )
			{
				m_pChargeGaugeWindow->ShowWindow(FALSE);
				m_pChargeGaugeWindow = NULL;
			}

			// ��縦 ��������� �ִ� ���
			if( m_pUserItem && m_pUserItem->m_usUsedProtect )
			{
				MessagePrint(2); //���̻� ��縦 �ٸ� �� �����ϴ�
				ShowWindow(FALSE);
				SetItemFlag( m_ItemFlag, ITEMSTEP_NONE );
			}
		}
		break;
	case ITEMSTEP_PROGRESS :
		{
			DrawInterface( ITEMSTEP_PROGRESS );
			m_slotImage->ShowWindow(FALSE);

			m_pChargeGaugeWindow = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
			if( m_pChargeGaugeWindow )
			{
				if(!m_pChargeGaugeWindow->GetShowStatus())
				{
#ifdef _XDWDEBUG
					m_pChargeGaugeWindow->SetDelayTime( 12000.0f );
#else
					m_pChargeGaugeWindow->SetDelayTime( 12000.0f );
#endif
					
					POINT pos = this->GetWindowPos();
					pos.x += 12;
					pos.y += 216;
					m_pChargeGaugeWindow->SetDrawPosition(pos);
					m_pChargeGaugeWindow->SetGaugeMode( _XGAUGE_SMELTITEM );
					m_pChargeGaugeWindow->ShowWindow(TRUE);
					m_pChargeGaugeWindow->SetSelectedNumber(3);	// ��������� �̿�
					g_MainWindowManager.SetTopWindow( m_pChargeGaugeWindow );

					switch(m_ucResult) 
					{
					case 1:		// 75%
						_XPlayInterfaceSound(ID_SR_INTERFACE_INCHANT_02_WAV);
						break;
					case 2:		// 50%
						_XPlayInterfaceSound(ID_SR_INTERFACE_INCHANT_01_WAV);
						break;
					default:
						_XPlayInterfaceSound(ID_SR_INTERFACE_INCHANT_03_WAV);
						break;	// 100%
					}
				}
			}
			m_pConfirmButton->EnableWindow(FALSE);
			m_pCancleButton->EnableWindow(TRUE);
		}
		break;
	}
}

void XWindow_SmeltItem::SetItemStep_Scales()
{
	switch( m_ItemStep )
	{
	case ITEMSTEP_NONE :
		{
			m_ltSlotNumber.clear();

			DrawInterface( ITEMSTEP_NONE );
			m_slotImage->ShowWindow(TRUE);

			m_nCount = 0;
			m_IMEControl.ClearBuffer();

			m_pUserItem = NULL;
			m_UserStuff[0].Reset();
			m_UserStuff[1].Reset();
			
			m_pConfirmButton->ShowWindow(FALSE);
			m_pConfirmButton->EnableWindow(TRUE);
			m_pCancleButton->EnableWindow(TRUE);

			if( g_pInventory_Window )
			{
				g_pInventory_Window->ResetSocketIndexList();
				g_pInventory_Window->m_UsedLifeItemIndex = 0;
			}

			if( m_pChargeGaugeWindow )
			{
				m_pChargeGaugeWindow->ShowWindow(FALSE);
				m_pChargeGaugeWindow = NULL;
			}
		}
		break;
	case ITEMSTEP_READY :
		{
			m_ltSlotNumber.clear();

			DrawInterface( ITEMSTEP_READY );
			m_slotImage->ShowWindow(TRUE);

			m_nCount = 0;
			m_IMEControl.ClearBuffer();
			m_IMEControl.SetFocus();

			//m_pConfirmButton->MoveWindow( m_WindowPosition.x + 80, m_WindowPosition.x + 200 );
			m_pConfirmButton->ShowWindow(TRUE);
			m_pConfirmButton->EnableWindow(TRUE);
			m_pCancleButton->EnableWindow(TRUE);

			if( m_pChargeGaugeWindow )
			{
				m_pChargeGaugeWindow->ShowWindow(FALSE);
				m_pChargeGaugeWindow = NULL;
			}
		}
		break;
	case ITEMSTEP_CONFIRM :
		{
			DrawInterface( ITEMSTEP_CONFIRM );
			m_slotImage->ShowWindow(FALSE);

			m_pConfirmButton->ShowWindow(TRUE);
			m_pConfirmButton->EnableWindow(TRUE);
			m_pCancleButton->EnableWindow(TRUE);

			if( m_pChargeGaugeWindow )
			{
				m_pChargeGaugeWindow->ShowWindow(FALSE);
				m_pChargeGaugeWindow = NULL;
			}

			
			int nScaleMaxCount = 0;
			GetMaxScaleCount( nScaleMaxCount );

			//�븰�� �� ���� ���� �ٸ� ������ ���ٸ�
			if( m_nCount > nScaleMaxCount )
			{
				m_nCount = 0;
				MessagePrint(3); //�븰�� �����մϴ�
				ShowWindow(FALSE);
				SetItemFlag( m_ItemFlag, ITEMSTEP_NONE );
				break;
			}

			// �ٸ� ������ �ִ� ���� ���� Ŭ��� ������ ���ڸ� ����� // ���� ���� �κ�..
			if( m_pUserItem && m_pUserItem->m_usCurMaxProtect < m_pUserItem->m_usCurProtect + m_nCount ) 
			{
				m_nCount = (m_pUserItem->m_usCurMaxProtect) - m_pUserItem->m_usCurProtect;
				if ( m_nCount < 1 )
				{
					m_nCount = 0;
					MessagePrint(1); //���̻� �븰�� �ٸ� �� �����ϴ�
					ShowWindow(FALSE);
					SetItemFlag( m_ItemFlag, ITEMSTEP_NONE );
					break;
				}
			}
			
			//�븰�� ��ġ�� �������ش�.. ��ó�� ������ clear �� ������ ���������� �̰����� �������ش�
			m_ltSlotNumber.push_back(m_nSmeltIndex); 
		}
		break;
	case ITEMSTEP_PROGRESS :
		{
			DrawInterface( ITEMSTEP_PROGRESS );
			m_slotImage->ShowWindow(FALSE);

			m_pChargeGaugeWindow = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
			if( m_pChargeGaugeWindow )
			{
				if(!m_pChargeGaugeWindow->GetShowStatus())
				{
#ifdef _XDWDEBUG
					m_pChargeGaugeWindow->SetDelayTime( 12000.0f );
#else
					m_pChargeGaugeWindow->SetDelayTime( 12000.0f );
#endif
					
					POINT pos = this->GetWindowPos();
					pos.x += 12;
					pos.y += 216;
					m_pChargeGaugeWindow->SetDrawPosition(pos);
					m_pChargeGaugeWindow->SetGaugeMode( _XGAUGE_SMELTITEM );
					m_pChargeGaugeWindow->ShowWindow(TRUE);
					m_pChargeGaugeWindow->SetSelectedNumber(3);	// ��������� �̿�
					g_MainWindowManager.SetTopWindow( m_pChargeGaugeWindow );

					switch(m_ucResult) 
					{
					case 1:		// 75%
						_XPlayInterfaceSound(ID_SR_INTERFACE_INCHANT_02_WAV);
						break;
					case 2:		// 50%
						_XPlayInterfaceSound(ID_SR_INTERFACE_INCHANT_01_WAV);
						break;
					default:
						_XPlayInterfaceSound(ID_SR_INTERFACE_INCHANT_03_WAV);
						break;	// 100%
					}
				}
			}
			m_pConfirmButton->EnableWindow(FALSE);
			m_pCancleButton->EnableWindow(TRUE);
		}
		break;
	}
}

void XWindow_SmeltItem::DrawItem()
{
	D3DXMATRIX			rotmat;
	D3DXMatrixIdentity(&rotmat);
	
	m_fItemtRotateAngle++;
	D3DXMatrixRotationY(&rotmat, _X_RAD(m_fItemtRotateAngle));

	if( _XGI_FC_WEAPON == GetEqualItemType(m_pUserItem->Get_m_cType()) )
	{
		D3DXMATRIX			zrotmat;
		D3DXMatrixRotationZ(&zrotmat, _X_RAD(90.0f));
		
		D3DXMatrixMultiply( &rotmat, &rotmat, &zrotmat );
	}	
	
	// Initialize Matrix
	int modelindex = GetItemModelIndex();
	D3DXMatrixIdentity(&m_ItemRHWMatrix);
	D3DXMatrixTranslation( &m_ItemRHWMatrix, 
		-g_ItemModel_Weapon[0][modelindex]->OBB_Center.x,
		-g_ItemModel_Weapon[0][modelindex]->OBB_Center.y,
		-g_ItemModel_Weapon[0][modelindex]->OBB_Center.z	);
	
	// Camera/ViewPort Setting
	m_ItemModelViewCamera.UpdateViewMatrix( NULL, FALSE );
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&m_ItemModelViewCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&m_ItemModelViewCamera.GetProjMatrix());
	gpDev->SetViewport(&m_ItemModelViewPort);
	
	// Z Buffer�� clear �Ѵ�. - ���� viewport������ clear�ȴ�.
	gpDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
	
	// Rendering			
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );	
	
	FLOAT scalefactor;
	switch(m_pUserItem->Get_m_cType()) 
	{
	case _XGI_FC_WEAPON:
	case _XGI_FC_WEAPON2:
	case _XGI_FC_WEAPON3:
		{
			scalefactor = 1.0f / g_ItemModel_Weapon[0][modelindex]->m_ObjectRadius;
			D3DXMATRIX matRot;
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixScaling(&matRot, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply( &matRot, &matRot, &rotmat );							
			D3DXMatrixMultiply(&m_ItemRHWMatrix, &m_ItemRHWMatrix, &matRot);
			
			g_ItemModel_Weapon[0][modelindex]->RenderInterfaceItem(&m_ItemRHWMatrix);
		}		
		break;
	}	
	
	// View Matrix, Projection Matrix, Viewport �ٽ� �ǵ�����. ====
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetProjMatrix());
	gpDev->SetViewport(&g_MainViewPort);
}

int XWindow_SmeltItem::GetItemModelIndex()
{
	if( !m_pUserItem ) return 0;

	if( _XGI_FC_WEAPON == GetEqualItemType( m_pUserItem->Get_m_cType()) )
	{
		_XGI_WeaponItem_Property* tempWeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_pUserItem->Get_m_cType());
		if( tempWeaponItem )
			return tempWeaponItem[m_pUserItem->Get_m_sID()].sModelIndex;
	}
	return 0;
}

int XWindow_SmeltItem::SearchSlotSmeltItem()
{
	int count			= _XINVENTORY_SLOTTYPE_POCKETSTART; //ó���� ����
	int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
	int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;	

	int selectedslot	= -1;

	// �ָӴ� ����Ʈ �˻�
	if( selectedslot == -1 )
	{
    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
        for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	    {
		    switch( m_ItemFlag )
			{
			case SCALES :
				{
					if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 &&
						g_pLocalUser->m_UserItemList[i].Get_m_cType() == _XGI_FC_LIFE &&
						g_LifeItemProperty[g_pLocalUser->m_UserItemList[i].Get_m_sID()].cSecondType == _XGI_SC_LIFE_SMLETITEM_SCALES )
					{
						selectedslot = i;

						list<int>::iterator iter = m_ltSlotNumber.begin();
						for(; iter != m_ltSlotNumber.end(); ++iter )
						{
							if( selectedslot == (*iter) ) //�ٸ� ������ ã�ƾ� �Ѵ�.. //�̹� ��ϵ� ����Ʈ ����
								selectedslot = -1;
						}	
					}
				}
				break;	
            }
	    }
				    
        for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	    {
		    switch( m_ItemFlag )
			{
			case SCALES :
				{
					if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 &&
						g_pLocalUser->m_UserItemList[i].Get_m_cType() == _XGI_FC_LIFE &&
						g_LifeItemProperty[g_pLocalUser->m_UserItemList[i].Get_m_sID()].cSecondType == _XGI_SC_LIFE_SMLETITEM_SCALES )
					{
						selectedslot = i;

						list<int>::iterator iter = m_ltSlotNumber.begin();
						for(; iter != m_ltSlotNumber.end(); ++iter )
						{
							if( selectedslot == (*iter) ) //�ٸ� ������ ã�ƾ� �Ѵ�.. //�̹� ��ϵ� ����Ʈ ����
								selectedslot = -1;
						}	
					}
				}
				break;	
            }		
	    }
    #else
		count = _XINVENTORY_SLOTTYPE_POCKETSTART; //�κ��� ���� ó�� ������ġ (����ĭ �� 25 )
				
		for( int row = 0; row < _XDEF_INVENTORYSLOT_HEIGHT; row++ )
		{
			for( int column = 0; column < _XDEF_INVENTORYSLOT_WIDTH; column++ )
			{
				if( count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT ) break;

				switch( m_ItemFlag )
				{
				case SCALES :
					{
						selectedslot = count;
						
						if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID() > 0 &&
							g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_LIFE &&
							g_LifeItemProperty[g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID()].cSecondType == _XGI_SC_LIFE_SMLETITEM_SCALES )
						{
							selectedslot = count;

							list<int>::iterator iter = m_ltSlotNumber.begin();
							for(; iter != m_ltSlotNumber.end(); ++iter )
							{
								if( selectedslot == (*iter) ) //�ٸ� ������ ã�ƾ� �Ѵ�.. //�̹� ��ϵ� ����Ʈ ����
									selectedslot = -1;
							}	
						}
						else
							selectedslot = -1;
					}
					break;
				}

				if( selectedslot != -1 ) break;

				columnpoint+=33;
				count++;			
			}
			
			if( selectedslot != -1 ) break;
			if( count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT ) break;
			
			columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
			rowpoint += 33;

		//������ �� ���Ҵ�... �ָӴϸ� �ٲ��ְ� �ٽ� �˻縦 �Ѵ�
			if( count == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART || count == _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART)
			{
				columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
				rowpoint = m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;

				row = -1;	//�ʱ�ȭ row++ ���ֱ⶧��
				column = 0; 
			}
		}
    #endif
	}

	return selectedslot;
}

void XWindow_SmeltItem::GetMaxScaleCount( int &nMaxCount )
{
	int count			= _XINVENTORY_SLOTTYPE_POCKETSTART; //ó���� ����
	int columnpoint		= m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
	int rowpoint		= m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;	

	nMaxCount = 0;

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
    for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		switch( m_ItemFlag )
		{
		case SCALES :
			{		
				if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 &&
					g_pLocalUser->m_UserItemList[i].Get_m_cType() == _XGI_FC_LIFE &&
					g_LifeItemProperty[g_pLocalUser->m_UserItemList[i].Get_m_sID()].cSecondType == _XGI_SC_LIFE_SMLETITEM_SCALES )
				{
					//�븰�� ��, �븰�� ������ �����ش�
					nMaxCount += (int)(g_pLocalUser->m_UserItemList[i].Get_m_ucCount());
				}
			}
			break;
		}
	}
				
    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		switch( m_ItemFlag )
		{
		case SCALES :
			{		
				if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 &&
					g_pLocalUser->m_UserItemList[i].Get_m_cType() == _XGI_FC_LIFE &&
					g_LifeItemProperty[g_pLocalUser->m_UserItemList[i].Get_m_sID()].cSecondType == _XGI_SC_LIFE_SMLETITEM_SCALES )
				{
					//�븰�� ��, �븰�� ������ �����ش�
					nMaxCount += (int)(g_pLocalUser->m_UserItemList[i].Get_m_ucCount());
				}
			}
			break;
		}	
	}
#else
	// �ָӴ� ����Ʈ �˻�
	count = _XINVENTORY_SLOTTYPE_POCKETSTART; //�κ��� ���� ó�� ������ġ (����ĭ �� 25 )

	//�κ��丮 ��ü �˻�
	for( int row = 0; row < _XDEF_INVENTORYSLOT_HEIGHT; row++ )
	{
		for( int column = 0; column < _XDEF_INVENTORYSLOT_WIDTH; column++ )
		{
			if( count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT ) break;

			switch( m_ItemFlag )
			{
			case SCALES :
				{		
					if( g_pLocalUser->m_UserItemList[count].Get_m_sID() > 0 &&
						g_pLocalUser->m_UserItemList[count].Get_m_cType() == _XGI_FC_LIFE &&
						g_LifeItemProperty[g_pLocalUser->m_UserItemList[count].Get_m_sID()].cSecondType == _XGI_SC_LIFE_SMLETITEM_SCALES )
					{
						//�븰�� ��, �븰�� ������ �����ش�
						nMaxCount += (int)(g_pLocalUser->m_UserItemList[count].Get_m_ucCount());
					}
				}
				break;
			}
			columnpoint+=33;
			count++;			
		}

		if( count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT ) break;
		
		columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
		rowpoint += 33;

		//������ �� ���Ҵ�... �ָӴϸ� �ٲ��ְ� �ٽ� �˻縦 �Ѵ�
		if( count == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART || count == _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART)
		{
			columnpoint = m_WindowPosition.x + _XDEF_INVENTORYITEMPOSX;
			rowpoint = m_WindowPosition.y + _XDEF_INVENTORYITEMPOSY;

			row = -1;	//�ʱ�ȭ row++ ���ֱ⶧��
			column = 0; 
		}
	}
#endif
}

BOOL XWindow_SmeltItem::ProcessSmeltCashItem(int slotindex)
{
#ifdef _XDEF_PERIODITEM_20061027
	if(CheckCashItem(g_pLocalUser->m_UserItemList[slotindex].Get_m_cType(), g_pLocalUser->m_UserItemList[slotindex].Get_m_sID()) == 2 || 
		CheckCashItem(g_pLocalUser->m_UserItemList[slotindex].Get_m_cType(), g_pLocalUser->m_UserItemList[slotindex].Get_m_sID()) == 3)
	{
		// �Ⱓ�� �������� ��� Message box �����ֱ�
		g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3726), TRUE, _XDEF_SMELT_PERIODITEM_YES, _XDEF_SMELT_PERIODITEM_NO);	//�Ⱓ�� �������� ���ÿ� �����ص� �Ⱓ�� �����Ǹ� ���� ���и� �� ��� �ܿ� �Ⱓ�� ������� �������� ������ϴ�. ����Ͻðڽ��ϱ�?

		m_nUserIndex = slotindex;		// ���� ��ȣ ����
		g_pInventory_Window->m_UsedLifeItemIndex = slotindex;
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();

		return TRUE;
	}
	else
	{
		m_nUserIndex = slotindex;		// ���� ��ȣ ����
		g_pInventory_Window->m_UsedLifeItemIndex = slotindex;
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
		return FALSE;
	}
#else
	return TRUE;
#endif
}

BOOL XWindow_SmeltItem::ProcessSmelt()
{
	if( CheckWeaponGrade(g_pInventory_Window->m_UsedLifeItemIndex) == 1 )
	{
		m_pUserItem = &g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex];
		SetItemFlag( m_ItemFlag, ITEMSTEP_READY);
		return TRUE;
	}
	else
		return FALSE;
}
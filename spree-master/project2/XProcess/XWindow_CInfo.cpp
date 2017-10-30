// XWindow_CInfo.cpp: implementation of the _XWindow_CInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_CInfo.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XMessageWindow.h"
#include "XWindow_TradeConfirm.h"

POINT g_EquippedItemCoord[25] = 
{
	{  271,109 },	//  0 ����		
	{  271,142 },	//  1 ����		
	{  306,175 },	//  2 �ƴ�		
	{  271,175 },	//  3 �Ź�		
	{  289,76  },	//  4 ����		
	{  306,109 },	//  5 ���� ��	
	{  306,142 },	//  6 ���� ��	
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	{  195,76  },	//  7 ����ũ 	
	{  0,0  },	//  8 �ǻ� �����1	
	{  0,0 },	//  9 �ǻ� �����2	
	{  228,76 },	// 10 ����1			
	{  228,109 },	// 11 ����2		
#else
	{  195,93  },	//  7 ����ũ 	
	{  0,0  },	//  8 �ǻ� �����1	
	{  0,0 },	//  9 �ǻ� �����2	
	{  228,93 },	// 10 ����1			
	{  228,129 },	// 11 ����2			
#endif
	
	{  11,76   },	// 12 ��  �Ǽ��縮1	
	{  44,76   },	// 13 ��  �Ǽ��縮2	
	{  11,109  },	// 14 ��  �Ǽ��縮3	
	{  44,109  },	// 15 ��  �Ǽ��縮4	
	
	{  11,142  },	// 16 ��  �Ǽ��縮1	
	{  44,142  },	// 17 ��  �Ǽ��縮2	
	{  11,175  },	// 18 ��  �Ǽ��縮3	
	{  44,175  },   // 19 ��  �Ǽ��縮4	
	
	{  0,0 },	// 20 ����ǰ ���� ����.

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	{  195,142 },	// 21 ĳ�� �೶
	{  228,142 },	// 22 ĳ�� �೶
#else
	{  0,0 },
	{  0,0 },
#endif

	{  195,175 },	// 23 �賶1
	{  228,175 }	// 24 �賶2
};

extern _XImageStatic g_InventoryIconImage;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XStickGaugeCallback_FuryGauge( POINT& winpos, SIZE& winsize, FLOAT& factor )
{
	g_XSmallFont->SetColor( 0xFFFFFFFF );
	g_XSmallFont->PrintAlign( winpos.x + (winsize.cx>>1), winpos.y-1, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%3.2f %%",  factor );
	g_XSmallFont->Flush();		
}

void __stdcall _XStickGaugeCallback_GiGauge( POINT& winpos, SIZE& winsize, FLOAT& factor )
{
	g_XSmallFont->SetColor( 0xFF000000 );
	g_XSmallFont->PrintAlign( winpos.x + (winsize.cx>>1), winpos.y-1, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%3.2f %%",  factor );
	g_XSmallFont->Flush();		
}



_XWindow_CInfo::_XWindow_CInfo()
{
	m_pGiBorder					 =	NULL;
	m_pFuryBorder				 =	NULL;
	m_SelectedSlotIndex			 =  -1;
	m_bDragState				 =  FALSE;
	m_IconDragStartPos.x		 =  0;
	m_IconDragStartPos.y		 =  0;
	m_LastDraggingSlot			 =  -1;	
	m_FavoritesNickNameList		 =	NULL;

	m_ScrollFactor				 =  0;
	m_pListScrollBar			 =  NULL;

	memset( m_FavoritesNickNameIndexList, 0, sizeof(int)* 10 );
}

_XWindow_CInfo::~_XWindow_CInfo()
{
	for( int i = 0; i < 5; i++ )
	{
		m_ModifyButton[i] = NULL;
	}

	SAFE_DELETE(m_FavoritesNickNameList);
}

void _XWindow_CInfo::SetDefaultPosition( void )
{	
	MoveWindow( 0, (gnHeight>>1)-205 );
	CheckOutside();
}

void _XWindow_CInfo::ShowWindow(BOOL show)
{	
	if(show)
	{	
		// Window ��ġ ������ =============================
		//MoveWindow( 0, (gnHeight>>1)-205 );
		
//#ifdef _XDEF_SHUTDOWN_20061027
//		//�˴ٿ�ÿ� ���� ����
//		if( g_PlayLeftTime == 0 )
//		{
//			// ���� ����Ʈ �ο� �Ұ�
//			for( int i = 0; i < 5; i++ )
//			{		
//				m_ModifyButton[i]->EnableWindow( FALSE );
//			}
//		}
//		else
//		{
//			for( int i = 0; i < 5; i++ )
//			{		
//				m_ModifyButton[i]->EnableWindow( TRUE );
//			}
//		}		
//#endif

	}
	else
	{
		if( m_FavoritesNickNameList->GetShowStatus() )
			m_FavoritesNickNameList->ShowWindow(FALSE);
	}
	_XWindow::ShowWindow(show);
}

void _XWindow_CInfo::SaveWindowProperty( void )
{
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWCHARACTERINFO, GetShowStatus() );
}

BOOL _XWindow_CInfo::Initialize( void )
{
	SetDrawBorder( FALSE );
	
	int characterwindowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	int questwinimage = g_MainInterfaceTextureArchive.FindResource( "MI_NPCQuestwin_01.tga" );

	_XImageStatic* pCharacterNameBorder = new _XImageStatic;
	pCharacterNameBorder->Create( 7, 11, 168, 32, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	pCharacterNameBorder->SetClipRect( 0, 0, 162, 22 );
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pCharacterNameBorder->ShowWindow( FALSE );
	}
	InsertChildObject( pCharacterNameBorder );

	_XImageStatic* pCharacterNameTextureBorder = new _XImageStatic;
	pCharacterNameTextureBorder->Create( 9, 13, 166, 30, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	pCharacterNameTextureBorder->SetClipRect( 0, 23, 158, 41 );
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pCharacterNameTextureBorder->ShowWindow( FALSE );
	}

	InsertChildObject( pCharacterNameTextureBorder );

	_XImageStatic* pCharacterNickNameBorder = new _XImageStatic;
	pCharacterNickNameBorder->Create( 179, 11, 340, 32, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	pCharacterNickNameBorder->SetClipRect( 0, 0, 162, 22 );
	InsertChildObject( pCharacterNickNameBorder );

	_XImageStatic* pGiBorder = new _XImageStatic;
	pGiBorder->Create( 248, 333, 338, 361, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	pGiBorder->SetClipRect( 0, 73, 91, 102 );
	InsertChildObject( pGiBorder );

	_XImageStatic* pEdge_Top = new _XImageStatic;
	pEdge_Top->Create( 246, 249, 342, 250, &g_MainInterfaceTextureArchive, questwinimage );
	pEdge_Top->SetClipRect( 24, 492, 120, 493 );
	InsertChildObject( pEdge_Top );
	
	_XImageStatic* pEdge_Bottom = new _XImageStatic;
	pEdge_Bottom->Create( 246, 364, 342, 365, &g_MainInterfaceTextureArchive, questwinimage );
	pEdge_Bottom->SetClipRect( 24, 500, 120, 501 );
	InsertChildObject( pEdge_Bottom );
	
	_XImageStatic* pEdge_Left = new _XImageStatic;
	pEdge_Left->Create( 245, 250, 246, 364 , &g_MainInterfaceTextureArchive, questwinimage );
	pEdge_Left->SetClipRect( 497, 283, 498, 397 );
	InsertChildObject( pEdge_Left );
	
	_XImageStatic* pEdge_Right = new _XImageStatic;
	pEdge_Right->Create( 342, 250, 343, 364, &g_MainInterfaceTextureArchive, questwinimage );
	pEdge_Right->SetClipRect( 501, 283, 502, 397 );
	InsertChildObject( pEdge_Right );

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pEdge_Top->ShowWindow( FALSE );
		pEdge_Bottom->ShowWindow( FALSE );
		pEdge_Left->ShowWindow( FALSE );
		pEdge_Right->ShowWindow( FALSE );
	}

	
	_XImageStatic* pEquipedItemIndicateImage = NULL;

	int equippointindex[10] = { 0, 1, 2, 3, 4, 7, 10, 11, 23, 24 };
	POINT equipedimageuv[10] = 
	{
		{ 56,24 },	// 0 ���� 
		{ 0, 52 },  // 1 ���� 
		{ 56,52 },  // 2 �ƴ� 
		{ 28,52 },  // 3 �Ź� 
		{ 28,24 },  // 4 ���� 		
		{ 56,80 },  // 7 ����ũ 
		{ 0, 24 },  // 10 ���� 1
		{ 0, 24 },  // 11 ���� 2
		{ 0, 80 },  // 23 �賶 1
		{ 0, 80 }   // 24 �賶 2
	};

	int mainimagebackindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );

	/*for( int i = 0; i < 10; i++ )
	{
		pEquipedItemIndicateImage = new _XImageStatic;
		pEquipedItemIndicateImage->Create( g_EquippedItemCoord[equippointindex[i]].x, g_EquippedItemCoord[equippointindex[i]].y, 
										   g_EquippedItemCoord[equippointindex[i]].x+28, g_EquippedItemCoord[equippointindex[i]].y+28, 
										   &g_MainInterfaceTextureArchive, mainimagebackindex );
		pEquipedItemIndicateImage->SetClipRect( equipedimageuv[i].x, equipedimageuv[i].y,
												equipedimageuv[i].x+28, equipedimageuv[i].y+28 );

		InsertChildObject( pEquipedItemIndicateImage );
	}*/

	m_PocketImageStatic.Create( 0, 0, 28, 28, &g_MainInterfaceTextureArchive, mainimagebackindex );
	m_PocketImageStatic.SetClipRect( 28, 80, 28+28, 80+28 );


	_XSTICKGAUGE_STRUCTURE	gaugestruct = 
	{
		TRUE, { 51, 219 }, { 284, 8 },	
			characterwindowimageindex, -1, &g_MainInterfaceTextureArchive
	};

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		gaugestruct.windowsize.cx = 244;
		gaugestruct.position.x = 91;
	}
	m_pFuryBorder = new _XStickGauge;
	m_pFuryBorder->Create( gaugestruct );	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_pFuryBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 42, 122, 50 );
	}
	else
	{
		m_pFuryBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 42, 142, 50 );
	}
	m_pFuryBorder->SetFactorDraw(TRUE);
	m_pFuryBorder->SetGaugeImageScaleFactor( 2.0f );
	m_pFuryBorder->SetCallBackFunction_FactorDraw( _XStickGaugeCallback_FuryGauge );

	InsertChildObject( m_pFuryBorder );


	_XImageStatic* pFuryGaugeLeftBorder = new _XImageStatic;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pFuryGaugeLeftBorder->Create( 89, 217, 192, 228, &g_MainInterfaceTextureArchive, characterwindowimageindex );
		pFuryGaugeLeftBorder->SetClipRect( 0, 60, 144, 72 );
	}
	else
	{
		pFuryGaugeLeftBorder->Create( 49, 217, 192, 228, &g_MainInterfaceTextureArchive, characterwindowimageindex );
		pFuryGaugeLeftBorder->SetClipRect( 0, 60, 144, 72 );
	}
	InsertChildObject( pFuryGaugeLeftBorder );

	_XImageStatic* pFuryGaugeRightBorder = new _XImageStatic;
	pFuryGaugeRightBorder->Create( 336, 217, 193, 228, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	pFuryGaugeRightBorder->SetClipRect( 0, 60, 143, 72 );
	pFuryGaugeRightBorder->SetScale( -1.0f, 1.0f );
	InsertChildObject( pFuryGaugeRightBorder );

	
	gaugestruct.position.x		=	51;
	gaugestruct.position.y		=	233;
	gaugestruct.windowsize.cx	=   284;
	gaugestruct.windowsize.cy	=   8;
	m_pGiBorder = new _XStickGauge;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		gaugestruct.windowsize.cx = 244;
		gaugestruct.position.x = 91;
	}

	m_pGiBorder->Create( gaugestruct );	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_pGiBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 51, 122, 59 );
	}
	else
	{
		m_pGiBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 51, 142, 59 );
	}
	
	m_pGiBorder->SetFactorDraw(TRUE);
	m_pGiBorder->SetGaugeImageScaleFactor( 2.0f );
	m_pGiBorder->SetCallBackFunction_FactorDraw( _XStickGaugeCallback_GiGauge );
	InsertChildObject( m_pGiBorder );
		
	_XImageStatic* pGiGaugeLeftBorder = new _XImageStatic;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pGiGaugeLeftBorder->Create( 89, 231, 192, 242, &g_MainInterfaceTextureArchive, characterwindowimageindex );
		pGiGaugeLeftBorder->SetClipRect( 0, 60, 144, 72 );
	}
	else
	{
		pGiGaugeLeftBorder->Create( 49, 231, 192, 242, &g_MainInterfaceTextureArchive, characterwindowimageindex );
		pGiGaugeLeftBorder->SetClipRect( 0, 60, 144, 72 );
	}	
	InsertChildObject( pGiGaugeLeftBorder );
	
	_XImageStatic* pGiGaugeRightBorder = new _XImageStatic;
	pGiGaugeRightBorder->Create( 336, 231, 193, 242, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	pGiGaugeRightBorder->SetClipRect( 0, 60, 143, 72 );
	pGiGaugeRightBorder->SetScale( -1.0f, 1.0f );
	InsertChildObject( pGiGaugeRightBorder );

	_XImageStatic* pCharacterBodyBorder = new _XImageStatic;
	pCharacterBodyBorder->Create( 94, 77, 180, 205, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	pCharacterBodyBorder->SetClipRect( 167, 69, 253, 197 );
	InsertChildObject( pCharacterBodyBorder );

	int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	_XBTN_STRUCTURE characternicdropdownbtnstruct = { TRUE, { 324, 16 }, { 12, 12 }, _XDEF_CI_FAVORITESNICKNAMELIST_BTN,
													  chattingbuttonindex, chattingbuttonindex, chattingbuttonindex,
													  &g_MainInterfaceTextureArchive };
	
	_XButton* pCharacterNickDropDownButton = new _XButton;
	pCharacterNickDropDownButton->Create( characternicdropdownbtnstruct );	
	pCharacterNickDropDownButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,0,  12, 12, 24 );
	pCharacterNickDropDownButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 12, 12, 24, 24 );
	pCharacterNickDropDownButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 24, 12, 36, 24 );
	InsertChildObject( pCharacterNickDropDownButton );

	int tradeimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Trade.tga" );
	int szButtonstring[3];	
	szButtonstring[0] = ID_STRING_CINFO_MERRIAGE;
	szButtonstring[1] = ID_STRING_CINFO_SAJAE;
	szButtonstring[2] = ID_STRING_CINFO_NICKNAME;

	int questimageresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_quest2.tga");
	
	_XCHECKBTN_STRUCTURE nicktabbuttonstruct = { TRUE, { 180, 41 }, { 54, 24 }, 
												_XDEF_CI_NICKBUTTON_MARRIAGE,
												tradeimageindex,
												tradeimageindex, 
												&g_MainInterfaceTextureArchive };

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		nicktabbuttonstruct.windowsize.cx = 66;
		nicktabbuttonstruct.windowsize.cy = 26;

		nicktabbuttonstruct.imageindex_normal  = questimageresourceindex;
		nicktabbuttonstruct.imageindex_checked = questimageresourceindex;
	}

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		nicktabbuttonstruct.position.x -= 12;
	}
	
	for( int i = 0; i < 3; i++ )
	{	
		m_pNickNameTabbutton[i] = new _XCheckButton;
		
		m_pNickNameTabbutton[i]->Create( nicktabbuttonstruct );		
		m_pNickNameTabbutton[i]->SetButtonSwapMode( TRUE );
		m_pNickNameTabbutton[i]->SetCheck( FALSE );
		m_pNickNameTabbutton[i]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, szButtonstring[i] );
		m_pNickNameTabbutton[i]->SetButtonTextColor( 0xFFCECECE, 0xFFFFFFFF );
		m_pNickNameTabbutton[i]->SetGrowTextMode( TRUE, 0xFF222222 );
		
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			m_pNickNameTabbutton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 1, 229, 67, 255 );
			m_pNickNameTabbutton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  67, 229, 133, 255 );
		}
		else
		{
			m_pNickNameTabbutton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 188, 2, 242, 26 );
			m_pNickNameTabbutton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  188, 52, 242, 76 );		
		}
			
		InsertChildObject( m_pNickNameTabbutton[i] );

		nicktabbuttonstruct.position.x += 54;
		nicktabbuttonstruct.commandid++;
	}		

	// ����� ������.
	m_pNickNameTabbutton[0]->EnableWindow( FALSE );
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
	m_pNickNameTabbutton[1]->EnableWindow( TRUE );
#else
	m_pNickNameTabbutton[1]->EnableWindow( FALSE );
#endif
	m_pNickNameTabbutton[2]->EnableWindow( TRUE );

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_pNickNameTabbutton[0]->ShowWindow( FALSE );
	#ifndef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
		m_pNickNameTabbutton[1]->ShowWindow( FALSE );
	#endif
	}
	
	_XBTN_STRUCTURE cinfomodifybtnstruct = { TRUE, { 329, 254 }, { 12, 12 }, _XDEF_CI_MODIFYBUTTON1,
											characterwindowimageindex, characterwindowimageindex,characterwindowimageindex, 
											&g_MainInterfaceTextureArchive };
	
	for( i = 0; i < 5; i++ )
	{
		m_ModifyButton[i]= new _XButton;
		m_ModifyButton[i]->Create( cinfomodifybtnstruct );
		
		m_ModifyButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,178,0, 190,12 );
		m_ModifyButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 178,12,190,24 );
		m_ModifyButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 178,24,190,36 );
		
		InsertChildObject( m_ModifyButton[i] );
		
		cinfomodifybtnstruct.commandid++;
		cinfomodifybtnstruct.position.y += 16;
	}

	_XBTN_STRUCTURE matchbuttonstruct = { TRUE, { 235, 366 }, { 54, 24 }, _XDEF_CI_MATCHBUTTON, 
											tradeimageindex, tradeimageindex, tradeimageindex, 
											&g_MainInterfaceTextureArchive };
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		matchbuttonstruct.position.x = 180;
	}

	_XButton* pMatchbutton = new _XButton;		
	pMatchbutton->Create( matchbuttonstruct );
	pMatchbutton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_CINFO_PVP );	// _T("��")
	pMatchbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 188, 2, 242, 26 );
	pMatchbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  188, 27, 242, 51 );
	pMatchbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  188, 52, 242, 76 );	
	InsertChildObject( pMatchbutton );
	pMatchbutton->EnableWindow( FALSE );

	_XBTN_STRUCTURE invenbuttonstruct = { TRUE, { 290, 366 }, { 54, 24 }, _XDEF_CI_IVENTORYBUTTON,
											tradeimageindex, tradeimageindex, tradeimageindex, 
											&g_MainInterfaceTextureArchive };

	_XButton* pInvenbutton = new _XButton;
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		int imageindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");
			
		invenbuttonstruct.position.x = 235;
		invenbuttonstruct.windowsize.cx = 108;
		invenbuttonstruct.imageindex_click = imageindex;
		invenbuttonstruct.imageindex_normal = imageindex;
		invenbuttonstruct.imageindex_ready = imageindex;		
	}
	
	pInvenbutton->Create( invenbuttonstruct );
	pInvenbutton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_CINFO_BACKPACK );	// _T("�೶")

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pInvenbutton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 152, 110, 176);
		pInvenbutton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 176, 110, 200);
		pInvenbutton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 200, 110, 224);
	}
	else
	{
		pInvenbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,188, 2, 242, 26 );
		pInvenbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 188, 27, 242, 51 );
		pInvenbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 188, 52, 242, 76 );
	}
	InsertChildObject( pInvenbutton );

		_XLISTBOX_STRUCTURE favoriteslistboxstructure = 
	{
		TRUE,											// ������ Ȱ��ȭ
		{ 179, 34 },		// ������ ��ǥ
		{ 162, 0 },	// ������ ������
		_XDEF_CI_FAVORITESNICKNAMELIST,					// ������ command ���̵�
		-1,												// ������ border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(""),											// ������ Ÿ��Ʋ
		15,												// ����Ʈ ������ ���� ��ġ
		10,												// ����Ʈ �ڽ��� ���̴� �ִ� ���μ�
		_XLSTYLE_NOSCROLLBAR						// ��ũ�ѹ��� ��ġ
	};
	
	m_FavoritesNickNameList = new _XListBox;
	m_FavoritesNickNameList->Create(favoriteslistboxstructure);
	_XStatic* tempstatic = m_FavoritesNickNameList->GetBorderStatic();
	tempstatic->SetFaceColor(0xFF000000);		
	
	POINT coloffset = { 2, 2 };
	m_FavoritesNickNameList->SetColumnOffset(0, coloffset);				// column offset	
	m_FavoritesNickNameList->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style 	
	m_FavoritesNickNameList->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type	
	m_FavoritesNickNameList->SetColumnWidth(0, 160);						// column width (column offset�� ���� �ʰų� �����ص� �������)
	
	m_FavoritesNickNameList->SetSelectedImageArchive(-1, &g_MainInterfaceTextureArchive);
	m_FavoritesNickNameList->SetSelectedImagePosition(0, 1);
	m_FavoritesNickNameList->SetSelectedImageHeight(15);
	m_FavoritesNickNameList->SetTrackWidth(160);
	m_FavoritesNickNameList->SetScrollPos(0);
	
	m_FavoritesNickNameList->ShowWindow(FALSE);	
	// �ֱ� ����� ��ȣ ����Ʈ

	// Item border & select icon	
		
	m_EquipedItemEdgeIcon[0].Create( 0, 0, 32, 32, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	m_EquipedItemEdgeIcon[0].SetClipRect( 191, 0, 223, 32 );

	m_EquipedItemEdgeIcon[1].Create( 0, 0, 32, 32, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	m_EquipedItemEdgeIcon[1].SetClipRect( 191, 33, 223, 65 );

	m_EquipedItemEdgeIcon[2].Create( 0, 0, 32, 32, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	m_EquipedItemEdgeIcon[2].SetClipRect( 224, 33, 256, 65 );

	m_EquipedItemEdgeIcon[3].Create( 0, 0, 32, 32, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	m_EquipedItemEdgeIcon[3].SetClipRect( 224, 0, 256, 32 );

	m_EquipedItemEdgeIcon[4].Create( 0, 0, 30, 30, &g_MainInterfaceTextureArchive, mainimagebackindex );
	m_EquipedItemEdgeIcon[4].SetClipRect( 216, 0, 246, 30 );


	for( i = 0; i < 5; i++ )
	{
		m_PointUpArea[i].x = 289;
		m_PointUpArea[i].y = 253+(i*16);
	}

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		int systemmsgimgindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );

		m_pListScrollBar = new _XVScrollBar;
		_XSCRBAR_STRUCTURE listscrbarstructure = 
		{
			TRUE, {190,250 }, 
				12, 364-250, // width, height
				12,		     // btnysize
				22,		     // trackbar size
				19, 7, 0,
				systemmsgimgindex,
				systemmsgimgindex,
				systemmsgimgindex,
				systemmsgimgindex,
				systemmsgimgindex,
				&g_MainInterfaceTextureArchive
		};
		
		m_pListScrollBar->Create( listscrbarstructure );
		m_pListScrollBar->ShowWindow( TRUE );
		m_pListScrollBar->SetViewBorder( TRUE );
		m_pListScrollBar->SetViewNullBorder( FALSE );
		m_pListScrollBar->SetTotalLineCount(19);	
		m_pListScrollBar->UpdateData();

		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}
		};
		m_pListScrollBar->SetClipRect( cliprectstructure );
		m_pListScrollBar->SetPagecount( 1 );

		InsertChildObject( m_pListScrollBar );
	}

	return TRUE;
}


void _XWindow_CInfo::MoveWindow( int X, int Y )
{	
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_CInfo::Draw( _XGUIObject*& pfocusobject )
{	
	if( this->m_ShowWindow )
	{
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			_XDrawSolidBar( m_WindowPosition.x+5, m_WindowPosition.y+9, m_WindowPosition.x+341, m_WindowPosition.y+69, 0x7F000000 );
			_XDrawRectAngle( m_WindowPosition.x+4, m_WindowPosition.y+8, m_WindowPosition.x+341, m_WindowPosition.y+68, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

			_XDrawSolidBar( m_WindowPosition.x+8, m_WindowPosition.y+11, m_WindowPosition.x+174, m_WindowPosition.y+31, 0x7F000000 );
			_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+12, m_WindowPosition.x+174, m_WindowPosition.y+32, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

			_XDrawSolidBar( m_WindowPosition.x+8, m_WindowPosition.y+34, m_WindowPosition.x+270, m_WindowPosition.y+65, 0x7F000000 );
			_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+35, m_WindowPosition.x+270, m_WindowPosition.y+66, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

			// Parameter
			_XDrawSolidBar( m_WindowPosition.x+5, m_WindowPosition.y+250, m_WindowPosition.x+202, m_WindowPosition.y+364, 0x7F000000 );
			_XDrawRectAngle( m_WindowPosition.x+4, m_WindowPosition.y+249, m_WindowPosition.x+202, m_WindowPosition.y+364, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );


			_XDrawSolidBar( m_WindowPosition.x+204, m_WindowPosition.y+250, m_WindowPosition.x+341, m_WindowPosition.y+364, 0x7F000000 );
			_XDrawRectAngle( m_WindowPosition.x+203, m_WindowPosition.y+249, m_WindowPosition.x+341, m_WindowPosition.y+364, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		}
		else
		{
			_XDrawSolidBar( m_WindowPosition.x+5, m_WindowPosition.y+9, m_WindowPosition.x+171, m_WindowPosition.y+67, 0x7F000000 );
			_XDrawRectAngle( m_WindowPosition.x+4, m_WindowPosition.y+8, m_WindowPosition.x+171, m_WindowPosition.y+67, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			
			_XDrawSolidBar( m_WindowPosition.x+8, m_WindowPosition.y+50, m_WindowPosition.x+167, m_WindowPosition.y+65, 0x7F000000 );
			_XDrawRectAngle( m_WindowPosition.x+8, m_WindowPosition.y+50, m_WindowPosition.x+167, m_WindowPosition.y+65, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

			_XDrawSolidBar( m_WindowPosition.x+5, m_WindowPosition.y+250, m_WindowPosition.x+239, m_WindowPosition.y+364, 0x7F000000 );
			_XDrawRectAngle( m_WindowPosition.x+4, m_WindowPosition.y+249, m_WindowPosition.x+239, m_WindowPosition.y+364, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

			// ����ȥ��. ��.
			int xpos = 0;
			for( int i = 0; i < 4; i++ ) 
			{
				_XDrawSolidBar( m_WindowPosition.x+19+xpos, m_WindowPosition.y+371, m_WindowPosition.x+42+xpos, m_WindowPosition.y+385, 0xA0000000 );
				xpos += 40;
			}
			_XDrawSolidBar( m_WindowPosition.x+202, m_WindowPosition.y+371, m_WindowPosition.x+229, m_WindowPosition.y+385, 0xA0000000 );

			_XDrawRectAngle( m_WindowPosition.x+4, m_WindowPosition.y+368, m_WindowPosition.x+165, m_WindowPosition.y+388, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			_XDrawRectAngle( m_WindowPosition.x+169, m_WindowPosition.y+368, m_WindowPosition.x+233, m_WindowPosition.y+388, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		}
		
		_XDrawSolidBar( m_WindowPosition.x+5, m_WindowPosition.y+71, m_WindowPosition.x+341, m_WindowPosition.y+210, 0x7F000000 );
		_XDrawRectAngle( m_WindowPosition.x+4, m_WindowPosition.y+70, m_WindowPosition.x+341, m_WindowPosition.y+210, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		
		_XDrawSolidBar( m_WindowPosition.x+5, m_WindowPosition.y+214, m_WindowPosition.x+341, m_WindowPosition.y+246, 0x7F000000 );
		_XDrawRectAngle( m_WindowPosition.x+4, m_WindowPosition.y+213, m_WindowPosition.x+341, m_WindowPosition.y+246, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	
		int ypos = 0;
		for( int i = 0; i < 5; i++ ) // �ٷ� ���� ����....
		{
			_XDrawSolidBar( m_WindowPosition.x+274, m_WindowPosition.y+253+ypos, m_WindowPosition.x+285, m_WindowPosition.y+267+ypos, 0xA0000000 );
			_XDrawSolidBar( m_WindowPosition.x+288, m_WindowPosition.y+253+ypos, m_WindowPosition.x+307, m_WindowPosition.y+267+ypos, 0x7F000000 );
			_XDrawSolidBar( m_WindowPosition.x+308, m_WindowPosition.y+253+ypos, m_WindowPosition.x+326, m_WindowPosition.y+267+ypos, 0x7F000000 );
			ypos += 16;
		}
	}

	_XWindow::Draw(pfocusobject);

	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
	
	TCHAR	titlestring[64];
	memset(titlestring, 0, sizeof(TCHAR)*64); 

	strcpy(titlestring, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));	

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->Print( m_WindowPosition.x + 13, m_WindowPosition.y + 16, 1.0f, _T("%s : %s"), 
							_XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_NAME), g_pLocalUser->m_CharacterInfo.charactername );

		g_XBaseFont->SetColor( 0xFFDFC61F );
		g_XBaseFont->Puts( m_WindowPosition.x + 13, m_WindowPosition.y + 36, g_JoinGroupName[ g_pLocalUser->m_CharacterInfo.groupindex ] );

		g_XBaseFont->SetColor( 0xFFBCB96F );
		g_XBaseFont->PutsAlign( m_WindowPosition.x + 262, m_WindowPosition.y + 36, _XFONT_ALIGNTYPE_RIGHT,
						   g_CharacterRollNameTable[g_pLocalUser->m_CharacterInfo.groupindex][g_pLocalUser->m_CharacterInfo.Get_cClass()][g_pLocalUser->m_CharacterInfo.Get_cClassGrade()] );			

		g_XBaseFont->SetColor( 0xFFC9CAC6 );
		g_XBaseFont->Print( m_WindowPosition.x + 13, m_WindowPosition.y + 54, 1.0f, "%s %d%s",
							g_LevelName[ g_pLocalUser->m_CharacterInfo.Get_level() ], g_pLocalUser->m_CharacterInfo.Get_sublevel(), titlestring ); // ���� ����	
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFBCB96F );
		g_XBaseFont->Puts( m_WindowPosition.x + 13, m_WindowPosition.y + 16, 
						  g_CharacterRollNameTable[g_pLocalUser->m_CharacterInfo.groupindex][g_pLocalUser->m_CharacterInfo.Get_cClass()][g_pLocalUser->m_CharacterInfo.Get_cClassGrade()] );
				
		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->Puts( m_WindowPosition.x + 92, m_WindowPosition.y + 16, g_pLocalUser->m_CharacterInfo.charactername );

		g_XBaseFont->SetColor( 0xFFDFC61F );
		g_XBaseFont->Puts( m_WindowPosition.x + 17, m_WindowPosition.y + 36, g_JoinGroupName[ g_pLocalUser->m_CharacterInfo.groupindex ] );

		g_XBaseFont->SetColor( 0xFFC9CAC6 );
		g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + 36, 1.0f, "%s %d%s", 
			g_LevelName[ g_pLocalUser->m_CharacterInfo.Get_level() ], g_pLocalUser->m_CharacterInfo.Get_sublevel(), titlestring ); // ���� ����	

		#ifdef _XDEF_NOTORIOUS_SYSTEM_MAGVIPER //Author : ����� //breif : ���� �ý��� 
		TCHAR strNotorious[64];
		memset( strNotorious, 0, sizeof(strNotorious));
		sprintf( strNotorious, "����                %d", g_pLocalUser->GetNotorious() );
		
		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->Puts( m_WindowPosition.x + 17, m_WindowPosition.y + 52, strNotorious );
		#endif
	}	
	
	g_XBaseFont->Flush();

	// ��ȣ
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		g_XBaseFont->SetClippingArea( m_WindowPosition.x+181, m_WindowPosition.y+15, m_WindowPosition.x+324, m_WindowPosition.y+31 );
		g_XBaseFont->Print( m_WindowPosition.x+181, m_WindowPosition.y+16, 1.0f, g_pLocalUser->m_CharacterInfo.m_NickName );
		g_XBaseFont->Flush();
		g_XBaseFont->DisableClippingArea();
	}
	else
	{
		g_XBaseFont->PrintAlign( m_WindowPosition.x+320, m_WindowPosition.y+16, 1.0f, _XFONT_ALIGNTYPE_RIGHT, g_pLocalUser->m_CharacterInfo.m_NickName );
	}

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		memset(titlestring, 0, sizeof(TCHAR)*64);
		strcpy(titlestring,_XGETINTERFACETEXT(ID_STRING_CINFO_FURY));//	_T("�� ��")	
		g_XBaseFont->Puts( m_WindowPosition.x + 14, m_WindowPosition.y + 216, titlestring ); // �г�
		
		memset(titlestring, 0, sizeof(TCHAR)*64);
		strcpy(titlestring, _XGETINTERFACETEXT(ID_STRING_NEW_2630));// _T("��")
		g_XBaseFont->Puts( m_WindowPosition.x + 14, m_WindowPosition.y + 230, titlestring ); // ��
	}
	else
	{
		memset(titlestring, 0, sizeof(TCHAR)*64);
		strcpy(titlestring,_XGETINTERFACETEXT(ID_STRING_CINFO_FURY));//	_T("�� ��")	
		g_XBaseFont->Puts( m_WindowPosition.x + 14, m_WindowPosition.y + 216, titlestring ); // �г�

		memset(titlestring, 0, sizeof(TCHAR)*64);
		strcpy(titlestring, _XGETINTERFACETEXT(ID_STRING_NEW_2630));// _T("��")
		g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 230, titlestring ); // ��
	}

	g_XBaseFont->Flush();	

	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, D3DCOLOR_ARGB( 160, 0, 0, 0 ) );
		g_XBaseFont->SetColor( 0xFFAC7E36 );
		strcpy(titlestring, _XGETINTERFACETEXT(ID_STRING_CINFO_FORCE));
		g_XBaseFont->Puts( m_WindowPosition.x + 259, m_WindowPosition.y + 342, titlestring ); // ���� �� ��ġ
		g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	g_XBaseFont->SetColor( 0xFFFC832A );
	g_XBaseFont->PrintAlign( m_WindowPosition.x + 322, m_WindowPosition.y + 343, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.Get_leftpoint() );
	g_XBaseFont->Flush();
	

	int scrollpos = 0;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_ScrollFactor = m_pListScrollBar->GetLineScrollPos();		
		scrollpos = (m_ScrollFactor << 4); //* 16;

		int datastartpos = 190;

		g_XBaseFont->SetClippingArea( m_WindowPosition.x + 6, m_WindowPosition.y + 253, m_WindowPosition.x + 341, m_WindowPosition.y + 364 );

		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+254-scrollpos, _XGETINTERFACETEXT(ID_STRING_CINFO_HEALTH) );					// ����
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+270-scrollpos, _XGETINTERFACETEXT(ID_STRING_CINFO_INNER) );			// ����

		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+286-scrollpos, _XGETINTERFACETEXT(ID_STRING_CINFO_ATTACKDAMAGE) );			// ���ݷ�
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+302-scrollpos, _XGETINTERFACETEXT(ID_STRING_CINFO_ATTACKRATING) );		// ���ݼ���ġ

		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+318-scrollpos, _XGETINTERFACETEXT(ID_STRING_CINFO_DEPENSE) );				// ����
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+334-scrollpos, _XGETINTERFACETEXT(ID_STRING_CINFO_AVOID) );				// ȸ�Ƿ�

		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+350-scrollpos, _XGETINTERFACETEXT(ID_STRING_CINFO_FORCEATTACK) );			// ������ݷ�
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+366-scrollpos, _XGETINTERFACETEXT(ID_STRING_CINFO_FORCEATTACKRATE) );	// �������ġ

		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+382-scrollpos, _XGETINTERFACETEXT(ID_STRING_NEW_2635) );					// �������
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+398-scrollpos, _XGETINTERFACETEXT(ID_STRING_CINFO_FORCEAVOID) );		// ���ȸ�Ƿ�

		memset(titlestring, 0, sizeof(TCHAR)*32);												
		if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH  )
		{
			sprintf(titlestring, "%s/%s", _T("External"), _XGETINTERFACETEXT(ID_STRING_CINFO_INTERNAL) );
		}
		else
		{
			sprintf(titlestring, "%s/%s", _XGETINTERFACETEXT(ID_STRING_CINFO_EXTERNAL), _XGETINTERFACETEXT(ID_STRING_CINFO_INTERNAL) );
		}
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+414-scrollpos, titlestring );													// �ܻ�/����
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+430-scrollpos, _XGETINTERFACETEXT(ID_STRING_CINFO_FAME) );				// ��

		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+446-scrollpos, _XGETINTERFACETEXT(ID_STRING_CINFO_CONTRIBUTION));			// ���ı⿩��
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+462-scrollpos, _XGETINTERFACETEXT(ID_STRING_NEW_2636) );				// �Ǹ�

		g_XBaseFont->SetColor( 0xFFFC832A );
		g_XBaseFont->Print( m_WindowPosition.x + 7, m_WindowPosition.y + 478-scrollpos, 1.0f, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2631) ); // ��
		g_XBaseFont->Print( m_WindowPosition.x + 7, m_WindowPosition.y + 494-scrollpos, 1.0f, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2632) ); // ��
		g_XBaseFont->Print( m_WindowPosition.x + 7, m_WindowPosition.y + 510-scrollpos, 1.0f, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2633) ); // ȥ
		g_XBaseFont->Print( m_WindowPosition.x + 7, m_WindowPosition.y + 526-scrollpos, 1.0f, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2634) ); // ��
		g_XBaseFont->Flush();

#ifdef _XDEF_RESIST_DISPLAY
		if(g_pLocalUser->m_ResistMinus >= 0)
			g_XBaseFont->SetColor( 0xFFFC832A );
		else
			g_XBaseFont->SetColor(0xFFAA3232);

		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 478-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_ResistMinus); 
#else
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 478-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 0  ); 
#endif
		
#ifdef _XDEF_RESIST_DISPLAY
		if(g_pLocalUser->m_ResistPlus >= 0)
			g_XBaseFont->SetColor( 0xFFFC832A );
		else
			g_XBaseFont->SetColor(0xFFAA3232);

		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 494-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_ResistPlus); 
#else
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 494-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 0    ); 
#endif
		
#ifdef _XDEF_RESIST_DISPLAY
		if(g_pLocalUser->m_ResistHon >= 0)
			g_XBaseFont->SetColor( 0xFFFC832A );
		else
			g_XBaseFont->SetColor(0xFFAA3232);

		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 510-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_ResistHon); 
#else
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 510-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 0    ); 
#endif
		
		
#ifdef _XDEF_RESIST_DISPLAY
		if(g_pLocalUser->m_ResistSin >= 0)
			g_XBaseFont->SetColor( 0xFFFC832A );
		else
			g_XBaseFont->SetColor(0xFFAA3232);

		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 526-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_ResistSin); 
#else
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 526-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 0    ); 
#endif
		
		g_XBaseFont->SetColor( 0xFFFF2222 );
		memset(titlestring, 0, sizeof(TCHAR)*32);
		strcpy(titlestring, _XGETINTERFACETEXT(ID_STRING_CINFO_2001));
		g_XBaseFont->Print( m_WindowPosition.x + 7, m_WindowPosition.y + 542-scrollpos, 1.0f, "%s", titlestring ); // �ߵ�
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 542-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%u", g_pLocalUser->m_ElixirInfo._sIntoxication ); 	
		
		
		g_XBaseFont->SetColor( 0xFFBDBCBA );
		// ����
		if(g_pLocalUser->m_MaxLifePower.Get_applyCount() > 0 || g_pLocalUser->m_MaxLifePower.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_MaxLifePower.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
			
			if(g_pLocalUser->m_MaxLifePower.Get_result() < 0)
			{
				g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 254-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
					g_pLocalUser->m_CharacterInfo.Get_current_lifepower(), 0);
			}
			else
			{
				g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 254-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
					g_pLocalUser->m_CharacterInfo.Get_current_lifepower(), g_pLocalUser->m_MaxLifePower.Get_result());
			}
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC8C4C3 );
			g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 254-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
				g_pLocalUser->m_CharacterInfo.Get_current_lifepower(), g_pLocalUser->m_MaxLifePower.Get_result());
		}
		// ���ݷ�
		if(g_pLocalUser->m_P_AttackDamage.Get_applyCount() > 0 || g_pLocalUser->m_P_AttackDamage.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_P_AttackDamage.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 286-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d~%d", 
			g_pLocalUser->m_P_MinAttackDamage, g_pLocalUser->m_P_MaxAttackDamage );

		// ����
		if(g_pLocalUser->m_P_Defence.Get_applyCount() > 0 || g_pLocalUser->m_P_Defence.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_P_Defence.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 318-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_P_Defence.Get_result() );

		// ��� ���ݷ�
		if(g_pLocalUser->m_M_AttackDamage.Get_applyCount() > 0 || g_pLocalUser->m_M_AttackDamage.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_M_AttackDamage.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 350-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d~%d", 
			g_pLocalUser->m_M_MinAttackDamage, g_pLocalUser->m_M_MaxAttackDamage );

		// �������
		if(g_pLocalUser->m_M_Defence.Get_applyCount() > 0 || g_pLocalUser->m_M_Defence.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_M_Defence.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 382-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_M_Defence.Get_result() );

		// �ܻ�/����
		g_XBaseFont->SetColor( 0xFFBDBCBA );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 414-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
			g_pLocalUser->m_CharacterInfo.Get_externalwoundrate(), g_pLocalUser->m_CharacterInfo.Get_internalwoundrate() );
		
		// ���� �⿩��
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 446-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.Get_iContribution() );			

		
		// ����
		if(g_pLocalUser->m_MaxForcePower.Get_applyCount() > 0 || g_pLocalUser->m_MaxForcePower.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_MaxForcePower.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);

			if(g_pLocalUser->m_MaxForcePower.Get_result() < 0)
			{
				g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 270-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
					g_pLocalUser->m_CharacterInfo.Get_current_forcepower(), 0);
			}
			else
			{
				g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 270-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
					g_pLocalUser->m_CharacterInfo.Get_current_forcepower(), g_pLocalUser->m_MaxForcePower.Get_result());
			}
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC8C4C3 );
			g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 270-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
				g_pLocalUser->m_CharacterInfo.Get_current_forcepower(), g_pLocalUser->m_MaxForcePower.Get_result());
		}
		
		// ���� ����ġ
		if(g_pLocalUser->m_P_AttackRate.Get_applyCount() > 0 || g_pLocalUser->m_P_AttackRate.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_P_AttackRate.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 302-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_P_AttackRate.Get_result() );

		
		// ȸ�Ƿ�
		if(g_pLocalUser->m_P_AvoidRate.Get_applyCount() > 0 || g_pLocalUser->m_P_AvoidRate.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_P_AvoidRate.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 334-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_P_AvoidRate.Get_result() );	

		// ��� ����ġ
		if(g_pLocalUser->m_M_AttackRate.Get_applyCount() > 0 || g_pLocalUser->m_M_AttackRate.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_M_AttackRate.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 366-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT,"%d", g_pLocalUser->m_M_AttackRate.Get_result() );

		// ��� ȸ�Ƿ�
		if(g_pLocalUser->m_M_AvoidRate.Get_applyCount() > 0 || g_pLocalUser->m_M_AvoidRate.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_M_AvoidRate.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 398-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_M_AvoidRate.Get_result() );

		g_XBaseFont->SetColor( 0xFFBDBCBA );
		// ��
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 430-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.Get_honor() );
		// �Ǹ� 
		g_XBaseFont->PrintAlign( m_WindowPosition.x + datastartpos, m_WindowPosition.y + 462-scrollpos, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.Get_Notoriety() );					

		g_XBaseFont->Flush();
		g_XBaseFont->DisableClippingArea();
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFFC832A );
		g_XBaseFont->Print( m_WindowPosition.x + 7, m_WindowPosition.y + 372, 1.0f, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2631) ); // ��
		g_XBaseFont->Print( m_WindowPosition.x + 47, m_WindowPosition.y + 372, 1.0f, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2632) ); // ��
		g_XBaseFont->Print( m_WindowPosition.x + 87, m_WindowPosition.y + 372, 1.0f, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2633) ); // ȥ
		g_XBaseFont->Print( m_WindowPosition.x + 127, m_WindowPosition.y + 372, 1.0f, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2634) ); // ��
		g_XBaseFont->Flush();

#ifdef _XDEF_RESIST_DISPLAY
		if(g_pLocalUser->m_ResistMinus >= 0)
			g_XBaseFont->SetColor( 0xFFFC832A );
		else
			g_XBaseFont->SetColor(0xFFAA3232);

		g_XBaseFont->Print( m_WindowPosition.x + 23, m_WindowPosition.y + 372, 1.0f, "%-5d", g_pLocalUser->m_ResistMinus ); 
#else
		g_XBaseFont->Print( m_WindowPosition.x + 23, m_WindowPosition.y + 372, 1.0f, "%-5d", 0  ); 
#endif
		
#ifdef _XDEF_RESIST_DISPLAY
		if(g_pLocalUser->m_ResistPlus >= 0)
			g_XBaseFont->SetColor( 0xFFFC832A );
		else
			g_XBaseFont->SetColor(0xFFAA3232);

		g_XBaseFont->Print( m_WindowPosition.x + 63, m_WindowPosition.y + 372, 1.0f, "%-5d", g_pLocalUser->m_ResistPlus ); 
#else
		g_XBaseFont->Print( m_WindowPosition.x + 63, m_WindowPosition.y + 372, 1.0f, "%-5d", 0 ); 
#endif
		
#ifdef _XDEF_RESIST_DISPLAY
		if(g_pLocalUser->m_ResistHon >= 0)
			g_XBaseFont->SetColor( 0xFFFC832A );
		else
			g_XBaseFont->SetColor(0xFFAA3232);

		g_XBaseFont->Print( m_WindowPosition.x + 103, m_WindowPosition.y + 372, 1.0f, "%-5d", g_pLocalUser->m_ResistHon ); 
#else
		g_XBaseFont->Print( m_WindowPosition.x + 103, m_WindowPosition.y + 372, 1.0f, "%-5d", 0 ); 
#endif
		
		
#ifdef _XDEF_RESIST_DISPLAY
		if(g_pLocalUser->m_ResistSin >= 0)
			g_XBaseFont->SetColor( 0xFFFC832A );
		else
			g_XBaseFont->SetColor(0xFFAA3232);

		g_XBaseFont->Print( m_WindowPosition.x + 143, m_WindowPosition.y + 372, 1.0f, "%-5d", g_pLocalUser->m_ResistSin ); 
#else
		g_XBaseFont->Print( m_WindowPosition.x + 143, m_WindowPosition.y + 372, 1.0f, "%-5d", 0 ); 
#endif
		
		g_XBaseFont->SetColor( 0xFFFF2222 );
		memset(titlestring, 0, sizeof(TCHAR)*32);
		strcpy(titlestring, _XGETINTERFACETEXT(ID_STRING_CINFO_2001));
		g_XBaseFont->Print( m_WindowPosition.x + 176, m_WindowPosition.y + 372, 1.0f, "%s", titlestring ); // �ߵ�
		g_XBaseFont->Print( m_WindowPosition.x + 205, m_WindowPosition.y + 372, 1.0f, "%-5u", g_pLocalUser->m_ElixirInfo._sIntoxication ); 	
		g_XBaseFont->Flush();
	
		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+254, _XGETINTERFACETEXT(ID_STRING_CINFO_HEALTH) );					// ����
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+270, _XGETINTERFACETEXT(ID_STRING_CINFO_ATTACKDAMAGE) );			// ���ݷ�
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+286, _XGETINTERFACETEXT(ID_STRING_CINFO_DEPENSE) );				// ����
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+302, _XGETINTERFACETEXT(ID_STRING_CINFO_FORCEATTACK) );			// ������ݷ�
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+318, _XGETINTERFACETEXT(ID_STRING_NEW_2635) );					// �������
		memset(titlestring, 0, sizeof(TCHAR)*32);																		
		sprintf(titlestring, "%s/%s", _XGETINTERFACETEXT(ID_STRING_CINFO_EXTERNAL), _XGETINTERFACETEXT(ID_STRING_CINFO_INTERNAL) );
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+334, titlestring );													// �ܻ�/����
		g_XBaseFont->Puts( m_WindowPosition.x+7, m_WindowPosition.y+350, _XGETINTERFACETEXT(ID_STRING_CINFO_CONTRIBUTION));			// ���ı⿩��

		g_XBaseFont->SetColor( 0xFFBDBCBA );
		// ����
		if(g_pLocalUser->m_MaxLifePower.Get_applyCount() > 0 || g_pLocalUser->m_MaxLifePower.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_MaxLifePower.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
			
			if(g_pLocalUser->m_MaxLifePower.Get_result() < 0)
			{
				g_XBaseFont->PrintAlign( m_WindowPosition.x + 126, m_WindowPosition.y + 254, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
					g_pLocalUser->m_CharacterInfo.Get_current_lifepower(), 0);
			}
			else
			{
				g_XBaseFont->PrintAlign( m_WindowPosition.x + 126, m_WindowPosition.y + 254, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
					g_pLocalUser->m_CharacterInfo.Get_current_lifepower(), g_pLocalUser->m_MaxLifePower.Get_result());
			}
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC8C4C3 );
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 126, m_WindowPosition.y + 254, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
				g_pLocalUser->m_CharacterInfo.Get_current_lifepower(), g_pLocalUser->m_MaxLifePower.Get_result());
		}
		// ���ݷ�
		if(g_pLocalUser->m_P_AttackDamage.Get_applyCount() > 0 || g_pLocalUser->m_P_AttackDamage.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_P_AttackDamage.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 126, m_WindowPosition.y + 270, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d~%d", 
			g_pLocalUser->m_P_MinAttackDamage, g_pLocalUser->m_P_MaxAttackDamage );

		// ����
		if(g_pLocalUser->m_P_Defence.Get_applyCount() > 0 || g_pLocalUser->m_P_Defence.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_P_Defence.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 126, m_WindowPosition.y + 286, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_P_Defence.Get_result() );

		// ��� ���ݷ�
		if(g_pLocalUser->m_M_AttackDamage.Get_applyCount() > 0 || g_pLocalUser->m_M_AttackDamage.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_M_AttackDamage.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 126, m_WindowPosition.y + 302, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d~%d", 
			g_pLocalUser->m_M_MinAttackDamage, g_pLocalUser->m_M_MaxAttackDamage );

		// �������
		if(g_pLocalUser->m_M_Defence.Get_applyCount() > 0 || g_pLocalUser->m_M_Defence.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_M_Defence.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 126, m_WindowPosition.y + 318, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_M_Defence.Get_result() );

		// �ܻ�/����
		g_XBaseFont->SetColor( 0xFFBDBCBA );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 126, m_WindowPosition.y + 334, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
			g_pLocalUser->m_CharacterInfo.Get_externalwoundrate(), g_pLocalUser->m_CharacterInfo.Get_internalwoundrate() );
		
		// ���� �⿩��
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 126, m_WindowPosition.y + 350, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.Get_iContribution() );			

		g_XBaseFont->SetColor( 0xFFFFFFFF );

		g_XBaseFont->Puts( m_WindowPosition.x+133, m_WindowPosition.y+254 , _XGETINTERFACETEXT(ID_STRING_CINFO_INNER) );			// ����
		g_XBaseFont->Puts( m_WindowPosition.x+133, m_WindowPosition.y+270, _XGETINTERFACETEXT(ID_STRING_CINFO_ATTACKRATING) );		// ���ݼ���ġ
		g_XBaseFont->Puts( m_WindowPosition.x+133, m_WindowPosition.y+286, _XGETINTERFACETEXT(ID_STRING_CINFO_AVOID) );				// ȸ�Ƿ�
		g_XBaseFont->Puts( m_WindowPosition.x+133, m_WindowPosition.y+302, _XGETINTERFACETEXT(ID_STRING_CINFO_FORCEATTACKRATE) );	// �������ġ
		g_XBaseFont->Puts( m_WindowPosition.x+133, m_WindowPosition.y+318, _XGETINTERFACETEXT(ID_STRING_CINFO_FORCEAVOID) );		// ���ȸ�Ƿ�
		g_XBaseFont->Puts( m_WindowPosition.x+133, m_WindowPosition.y+334, _XGETINTERFACETEXT(ID_STRING_CINFO_FAME) );				// ��
		g_XBaseFont->Puts( m_WindowPosition.x+133, m_WindowPosition.y+350, _XGETINTERFACETEXT(ID_STRING_NEW_2636) );				// �Ǹ�

		// ����
		if(g_pLocalUser->m_MaxForcePower.Get_applyCount() > 0 || g_pLocalUser->m_MaxForcePower.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_MaxForcePower.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);

			if(g_pLocalUser->m_MaxForcePower.Get_result() < 0)
			{
				g_XBaseFont->PrintAlign( m_WindowPosition.x + 236, m_WindowPosition.y + 254, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
					g_pLocalUser->m_CharacterInfo.Get_current_forcepower(), 0);
			}
			else
			{
				g_XBaseFont->PrintAlign( m_WindowPosition.x + 236, m_WindowPosition.y + 254, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
					g_pLocalUser->m_CharacterInfo.Get_current_forcepower(), g_pLocalUser->m_MaxForcePower.Get_result());
			}
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC8C4C3 );
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 236, m_WindowPosition.y + 254, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d/%d", 
				g_pLocalUser->m_CharacterInfo.Get_current_forcepower(), g_pLocalUser->m_MaxForcePower.Get_result());
		}
		
		// ���� ����ġ
		if(g_pLocalUser->m_P_AttackRate.Get_applyCount() > 0 || g_pLocalUser->m_P_AttackRate.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_P_AttackRate.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 236, m_WindowPosition.y + 270, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_P_AttackRate.Get_result() );

		
		// ȸ�Ƿ�
		if(g_pLocalUser->m_P_AvoidRate.Get_applyCount() > 0 || g_pLocalUser->m_P_AvoidRate.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_P_AvoidRate.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 236, m_WindowPosition.y + 286, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_P_AvoidRate.Get_result() );	

		// ��� ����ġ
		if(g_pLocalUser->m_M_AttackRate.Get_applyCount() > 0 || g_pLocalUser->m_M_AttackRate.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_M_AttackRate.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 236, m_WindowPosition.y + 302, 1.0f, _XFONT_ALIGNTYPE_RIGHT,"%d", g_pLocalUser->m_M_AttackRate.Get_result() );

		// ��� ȸ�Ƿ�
		if(g_pLocalUser->m_M_AvoidRate.Get_applyCount() > 0 || g_pLocalUser->m_M_AvoidRate.Get_applyCountAdditional() > 0)
		{
			if(g_pLocalUser->m_M_AvoidRate.Get_bBuff())
				g_XBaseFont->SetColor(0xFF59C74E);
			else
				g_XBaseFont->SetColor(0xFFCD2323);
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFC4C4C2 );
		}
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 236, m_WindowPosition.y + 318, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_M_AvoidRate.Get_result() );

		g_XBaseFont->SetColor( 0xFFBDBCBA );
		// ��
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 236, m_WindowPosition.y + 334, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.Get_honor() );
		// �Ǹ� 
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 236, m_WindowPosition.y + 350, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.Get_Notoriety() );	
	}

	int fivepointbasepos = 248;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		fivepointbasepos = 207;
	}

	g_XBaseFont->SetColor( 0xFFC8C4C3 );
	g_XBaseFont->Puts( m_WindowPosition.x + fivepointbasepos, m_WindowPosition.y + 254,  _XGETINTERFACETEXT(ID_STRING_CINFO_STR));	// _T("�ٷ�")
	g_XBaseFont->Puts( m_WindowPosition.x + fivepointbasepos, m_WindowPosition.y + 270,  _XGETINTERFACETEXT(ID_STRING_CINFO_INNERENERGY)); 	// _T("����")
	g_XBaseFont->Puts( m_WindowPosition.x + fivepointbasepos, m_WindowPosition.y + 286,  _XGETINTERFACETEXT(ID_STRING_CINFO_HEART));	// _T("����")
	g_XBaseFont->Puts( m_WindowPosition.x + fivepointbasepos, m_WindowPosition.y + 302,  _XGETINTERFACETEXT(ID_STRING_CINFO_HEALTHENERGY));	// _T("�ǰ�")
	g_XBaseFont->Puts( m_WindowPosition.x + fivepointbasepos, m_WindowPosition.y + 318,  _XGETINTERFACETEXT(ID_STRING_CINFO_DEX));	// _T("��ø")
	
	g_XBaseFont->SetColor( 0xFF968573 );
	g_XBaseFont->PrintAlign( m_WindowPosition.x + 284, m_WindowPosition.y + 255, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_UsePointTable[ g_pLocalUser->m_CharacterInfo.Get_cClass() ].need_strength     ); // �ٷ� �ʿ�ġ 
	g_XBaseFont->PrintAlign( m_WindowPosition.x + 284, m_WindowPosition.y + 271, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_UsePointTable[ g_pLocalUser->m_CharacterInfo.Get_cClass() ].need_zen          ); // ���� �ʿ�ġ 
	g_XBaseFont->PrintAlign( m_WindowPosition.x + 284, m_WindowPosition.y + 287, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_UsePointTable[ g_pLocalUser->m_CharacterInfo.Get_cClass() ].need_intelligence ); // ���� �ʿ�ġ 
	g_XBaseFont->PrintAlign( m_WindowPosition.x + 284, m_WindowPosition.y + 303, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_UsePointTable[ g_pLocalUser->m_CharacterInfo.Get_cClass() ].need_constitution ); // �ǰ� �ʿ�ġ 
	g_XBaseFont->PrintAlign( m_WindowPosition.x + 284, m_WindowPosition.y + 319, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_UsePointTable[ g_pLocalUser->m_CharacterInfo.Get_cClass() ].need_dexterity    ); // ��ø �ʿ�ġ 

	// �ٷ�
	if( g_pLocalUser->m_ElixirInfo.Data[0].__Grade > 0 )
	{
		if( g_pLocalUser->m_ElixirInfo.GetTotalBonus(0) > 0 )
		{
			g_XBaseFont->SetColor(0xFFFFFF23);
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 325, m_WindowPosition.y + 255, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "+%d", 
				g_pLocalUser->m_ElixirInfo.GetTotalBonus(0));
		}
		else
		{
			g_XBaseFont->SetColor(0xFFCD2323);
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 325, m_WindowPosition.y + 255, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
				g_pLocalUser->m_ElixirInfo.GetTotalBonus(0));
		}
	}
	if(g_pLocalUser->m_Strength.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_Strength.Get_bBuff() == 1)
			g_XBaseFont->SetColor(0xFF59C74E);
		else
			g_XBaseFont->SetColor( 0xFFCD2323 );

		if(g_pLocalUser->m_Strength.Get_result() < 0)
		{
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 255, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 0);
		}
		else
		{
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 255, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
				g_pLocalUser->m_Strength.Get_result());
		}		
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFC8C4C3 );		
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 255, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
			g_pLocalUser->m_Strength.Get_org());
	}
	// ����	
	if( g_pLocalUser->m_ElixirInfo.Data[1].__Grade > 0 )
	{
		if( g_pLocalUser->m_ElixirInfo.GetTotalBonus(1) > 0 )
		{
			g_XBaseFont->SetColor(0xFFFFFF23);
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 325, m_WindowPosition.y + 271, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "+%d", 
				g_pLocalUser->m_ElixirInfo.GetTotalBonus(1));
		}
		else 
		{
			g_XBaseFont->SetColor(0xFFCD2323);		
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 325, m_WindowPosition.y + 271, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
				g_pLocalUser->m_ElixirInfo.GetTotalBonus(1));
		}		
	}
	if(g_pLocalUser->m_Zen.Get_applyCount() > 0 )
	{
		if(g_pLocalUser->m_Zen.Get_bBuff() == 1)
			g_XBaseFont->SetColor(0xFF59C74E);
		else
			g_XBaseFont->SetColor(0xFFCD2323);

		if(g_pLocalUser->m_Zen.Get_result() < 0)
		{
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 271, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 0);
		}
		else
		{
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 271, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
				g_pLocalUser->m_Zen.Get_result());
		}
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFC8C4C3 );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 271, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
			g_pLocalUser->m_Zen.Get_org());
	}
	// ����	
	if( g_pLocalUser->m_ElixirInfo.Data[2].__Grade > 0 )
	{
		if( g_pLocalUser->m_ElixirInfo.GetTotalBonus(2) > 0 )
		{
			g_XBaseFont->SetColor(0xFFFFFF23);
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 325, m_WindowPosition.y + 287, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "+%d", 
				g_pLocalUser->m_ElixirInfo.GetTotalBonus(2));
		}
		else 
		{
			g_XBaseFont->SetColor(0xFFCD2323);
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 325, m_WindowPosition.y + 287, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
				g_pLocalUser->m_ElixirInfo.GetTotalBonus(2));
		}				
	}
	if(g_pLocalUser->m_Intelligence.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_Intelligence.Get_bBuff()== 1)
			g_XBaseFont->SetColor(0xFF59C74E);
		else
			g_XBaseFont->SetColor(0xFFCD2323);

		if(g_pLocalUser->m_Intelligence.Get_result() < 0)
		{
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 287, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 0);
		}
		else
		{
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 287, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
				g_pLocalUser->m_Intelligence.Get_result());
		}
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFC8C4C3 );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 287, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
			g_pLocalUser->m_Intelligence.Get_org());
	}
	// �ǰ�
	if( g_pLocalUser->m_ElixirInfo.Data[3].__Grade > 0 )
	{
		if( g_pLocalUser->m_ElixirInfo.GetTotalBonus(3) > 0 )
		{
			g_XBaseFont->SetColor(0xFFFFFF23);
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 325, m_WindowPosition.y + 303, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "+%d", 
				g_pLocalUser->m_ElixirInfo.GetTotalBonus(3));
		}
		else 
		{
			g_XBaseFont->SetColor(0xFFCD2323);		
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 325, m_WindowPosition.y + 303, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
				g_pLocalUser->m_ElixirInfo.GetTotalBonus(3));
		}
	}
	if(g_pLocalUser->m_Constitution.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_Constitution.Get_bBuff() == 1)
			g_XBaseFont->SetColor(0xFF59C74E);
		else
			g_XBaseFont->SetColor(0xFFCD2323);
		
		if(g_pLocalUser->m_Constitution.Get_result() < 0)
		{
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 303, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 0);
		}
		else
		{
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 303, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
				g_pLocalUser->m_Constitution.Get_result());
		}
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFC8C4C3 );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 303, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
			g_pLocalUser->m_Constitution.Get_org());
	}
	// ��÷
	if( g_pLocalUser->m_ElixirInfo.Data[4].__Grade > 0 )
	{
		if( g_pLocalUser->m_ElixirInfo.GetTotalBonus(4) > 0 )
		{
			g_XBaseFont->SetColor(0xFFFFFF23);
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 325, m_WindowPosition.y + 319, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "+%d", 
				g_pLocalUser->m_ElixirInfo.GetTotalBonus(4));
		}
		else 
		{
			g_XBaseFont->SetColor(0xFFCD2323);		
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 325, m_WindowPosition.y + 319, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
				g_pLocalUser->m_ElixirInfo.GetTotalBonus(4));
		}		
	}
	if(g_pLocalUser->m_Dex.Get_applyCount() > 0 )
	{
		if(g_pLocalUser->m_Dex.Get_bBuff() == 1)
			g_XBaseFont->SetColor(0xFF59C74E);
		else
			g_XBaseFont->SetColor(0xFFCD2323);

		if(g_pLocalUser->m_Dex.Get_result() < 0)
		{
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 319, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 0);
		}
		else
		{
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 319, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
				g_pLocalUser->m_Dex.Get_result());
		}
	}
	else
	{
		g_XBaseFont->SetColor( 0xFFC8C4C3 );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 306, m_WindowPosition.y + 319, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
				g_pLocalUser->m_Dex.Get_org());
	}
	
	g_XBaseFont->Flush();

	DrawEquippedItemList();
	DrawSelectedItem();

	// Hovering mini item tooltip
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	if( scrnpos->x > m_WindowPosition.x+9 && scrnpos->x < m_WindowPosition.x+337 &&		 // Equiped item area
		scrnpos->z > m_WindowPosition.y+74 && scrnpos->z < m_WindowPosition.y+206 )
	{				
		RECT iconrect = {0,0,0,0};
		int hoveredslot = SearchSlotRegion(&iconrect);

		if( m_SelectedSlotIndex != hoveredslot )
		{
			if(hoveredslot != -1 && g_pLocalUser->m_UserItemList[hoveredslot].Get_m_sID() > 0) // ���Կ� �������� �ִ°�츸 �巡�� ����
			{
				g_pInventory_Window->DrawItemTooltip(hoveredslot, iconrect);
			}
		}
	}
	
	// �ٷ� ���� ... ����
	bool flag = false;
	int textwidth = 25;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		textwidth = 70;
	}

	if( scrnpos->x > m_WindowPosition.x+fivepointbasepos && scrnpos->x < m_WindowPosition.x+fivepointbasepos+textwidth &&
		scrnpos->z > m_WindowPosition.y+253 && scrnpos->z < m_WindowPosition.y+253+80 )
	{
		if( g_pMiniTooltip_Window )
		{
			int selectid = -1;
			selectid = scrnpos->z - (m_WindowPosition.y+253);
			selectid /= 16;
			if( selectid != -1 )
			{
				flag = true;
				if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_CHARINFO_ABILITY, 0, 0, selectid ) )
				{
					g_pMiniTooltip_Window->ShowWindow( TRUE );
				}
			}			
		}
	}

	// �ܰ� ����
	if( scrnpos->x > m_WindowPosition.x+275 && scrnpos->x < m_WindowPosition.x+275+50 &&
		scrnpos->z > m_WindowPosition.y+253 && scrnpos->z < m_WindowPosition.y+253+80 )
	{
		if( g_pMiniTooltip_Window )
		{
			int selectid = -1;
			selectid = scrnpos->z - (m_WindowPosition.y+253);
			selectid /= 16;
			if( selectid != -1 )
			{
				// ������ �Ѱ��� �Ծ��� ���� ǥ���Ѵ�.
				if( g_pLocalUser->m_ElixirInfo.Data[selectid].__Grade > 0 )
				{
					flag = true;
					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_CHARINFO_ABILITYSTEP, 0, 0, selectid ) )
					{
						g_pMiniTooltip_Window->ShowWindow( TRUE );
					}
				}				
			}			
		}
	}

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		// �ߵ� ���� ����
		if( scrnpos->x > m_WindowPosition.x+7 && scrnpos->x < m_WindowPosition.x+202 &&
			scrnpos->z > m_WindowPosition.y + 542-scrollpos && scrnpos->z < m_WindowPosition.y + 542 - scrollpos + 16)
		{
			if( g_pMiniTooltip_Window )
			{
				flag = true;
				if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_CHARINFO_INTOXICATION, 0, 0, 0 ) )
				{
					g_pMiniTooltip_Window->ShowWindow( TRUE );
				}
			}
		}
		
		// ��, �Ǹ� ����
		if( scrnpos->x > m_WindowPosition.x+7 && scrnpos->x < m_WindowPosition.x+202 )
		{
			if( scrnpos->z > m_WindowPosition.y + 430-scrollpos && scrnpos->z < m_WindowPosition.y + 430-scrollpos + 16 )
			{
				DrawFameTooltip( 0, scrnpos->x+10, scrnpos->z-16);
			}
			else if( scrnpos->z > m_WindowPosition.y + 462-scrollpos && scrnpos->z < m_WindowPosition.y + 462-scrollpos + 16 )
			{
				DrawFameTooltip( 1, scrnpos->x+10, scrnpos->z-16);
			}
		}
	}
	else
	{
		// �ߵ� ���� ����
		if( scrnpos->x > m_WindowPosition.x+169 && scrnpos->x < m_WindowPosition.x+233 &&
			scrnpos->z > m_WindowPosition.y+368 && scrnpos->z < m_WindowPosition.y+388 )
		{
			if( g_pMiniTooltip_Window )
			{
				flag = true;
				if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_CHARINFO_INTOXICATION, 0, 0, 0 ) )
				{
					g_pMiniTooltip_Window->ShowWindow( TRUE );
				}
			}
		}
		
		// ��, �Ǹ� ����
		if( scrnpos->x > m_WindowPosition.x+133 && scrnpos->x < m_WindowPosition.x+133+104 &&
			scrnpos->z > m_WindowPosition.y+333 && scrnpos->z < m_WindowPosition.y+333+32 )
		{
			int selectid = -1;
			selectid = scrnpos->z - (m_WindowPosition.y+333);
			selectid /= 16;
			if( selectid != -1 )
			{
				DrawFameTooltip(selectid, scrnpos->x+10, scrnpos->z-16);			
			}	
		}
	}

	#ifdef _XDEF_NOTORIOUS_SYSTEM_MAGVIPER //Author : ����� //breif : ���� �ý��� 
	/*if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		if( ((scrnpos->x > m_WindowPosition.x+12 ) && (scrnpos->x < m_WindowPosition.x+12+128) &&
			(scrnpos->z > m_WindowPosition.y+53 ) && (scrnpos->z < m_WindowPosition.y+53+13) ) )
		{
			TCHAR messagebuffer[256];
			memset( messagebuffer, 0 ,sizeof(messagebuffer));

			sprintf(messagebuffer, _XGETINTERFACETEXT(ID_STRING_NEW_2753), g_pLocalUser->m_CharacterInfo.Get_innerlevel());		// "���� %d"
			g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+12, m_WindowPosition.y+68, messagebuffer, _XSC_INFORMATION, TRUE);
		}
	}
	else*/
	{
		if( ((scrnpos->x > m_WindowPosition.x+17 ) && (scrnpos->x < m_WindowPosition.x+149) &&
			(scrnpos->z > m_WindowPosition.y+52 ) && (scrnpos->z < m_WindowPosition.y+52+13) ) )
		{
			TCHAR messagebuffer[256];
			memset( messagebuffer, 0 ,sizeof(messagebuffer));

			if( g_pMiniTooltip_Window )
			{
				flag = true;
				if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_NOTORIOUS, 0, 0, g_pLocalUser->GetNotorious() ) )
				{
					g_pMiniTooltip_Window->ShowWindow( TRUE );
				}
			}

			//sprintf(messagebuffer, _XGETINTERFACETEXT(ID_STRING_NEW_2753), g_pLocalUser->m_CharacterInfo.Get_innerlevel());		// "���� %d"
			//g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+50, m_WindowPosition.y+52, messagebuffer, _XSC_INFORMATION, TRUE);
		}
	}
	#endif

	if( flag == false )
	{
		if( g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_CHARINFO_ABILITY || 
			g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_CHARINFO_ABILITYSTEP ||
			g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_CHARINFO_INTOXICATION  )
		g_pMiniTooltip_Window->ShowWindow( FALSE );
	}
	
	for( int i = 0; i < 5; i++ )
	{
		if( m_ModifyButton[i]->GetShowStatus() && m_ModifyButton[i]->IsButtonEnabled() )
		{
			if( m_ModifyButton[i]->CheckMousePosition() )
			{
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_STATUS );
			}			
		}
	}
	// �ֱ� ������ ��ȣ ����Ʈ
	m_FavoritesNickNameList->Draw();
	
#ifdef _XTS_LEVELTOOLTIP
	// level tooltip
	MouseState* mousestate = gpInput->GetMouseState();
	scrnpos = gpInput->GetMouseVirScreenPos();

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		if( ((scrnpos->x > m_WindowPosition.x+12 ) && (scrnpos->x < m_WindowPosition.x+12+128) &&
			(scrnpos->z > m_WindowPosition.y+53 ) && (scrnpos->z < m_WindowPosition.y+53+13) ) )
		{
			TCHAR messagebuffer[256];
			sprintf(messagebuffer, _XGETINTERFACETEXT(ID_STRING_NEW_2753), g_pLocalUser->m_CharacterInfo.Get_innerlevel());		// "���� %d"
			g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+12, m_WindowPosition.y+68, messagebuffer, _XSC_INFORMATION, TRUE);
		}
	}
	else
	{
		if( ((scrnpos->x > m_WindowPosition.x+76 ) && (scrnpos->x < m_WindowPosition.x+76+76) &&
			(scrnpos->z > m_WindowPosition.y+36 ) && (scrnpos->z < m_WindowPosition.y+36+11) ) )
		{
			TCHAR messagebuffer[256];
			sprintf(messagebuffer, _XGETINTERFACETEXT(ID_STRING_NEW_2753), g_pLocalUser->m_CharacterInfo.Get_innerlevel());		// "���� %d"
			g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+149, m_WindowPosition.y+52, messagebuffer, _XSC_INFORMATION, TRUE);
		}
	}
#endif

	
	
#ifdef _XDEF_MODIFY_UI_070710_KUKURI
	// ��å tooltip
	mousestate = gpInput->GetMouseState();
	scrnpos = gpInput->GetMouseVirScreenPos();

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
	}
	else
	{
		if( ((scrnpos->x > m_WindowPosition.x+13 ) && (scrnpos->x < m_WindowPosition.x+13+36) &&
			(scrnpos->z > m_WindowPosition.y+16 ) && (scrnpos->z < m_WindowPosition.y+16+12) ) )
		{
			if(g_pLocalUser->m_CharacterInfo.Get_cClass() > 0)
			{
				int classid = g_pLocalUser->m_CharacterInfo.Get_cClass();
				if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG)
				{
					if(classid == 7)	// ��ȥ��
						classid = 4;
				}
				else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM)
				{
					if(classid == 8)	// ö����
						classid = 4;
				}

				g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+28, m_WindowPosition.y+30, 
					_XGETINTERFACETEXT(ID_STRING_NEW_2660+classid-1), _XSC_INFORMATION, TRUE);
			}
		}
	}
#endif
}

BOOL _XWindow_CInfo::CheckEquipedArea( void )
{
	if(!this->m_Enable)		return FALSE;
	if(!this->m_ShowWindow) 
	{		
		return FALSE;
	}

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	if(CheckMousePosition())
	{
		// Focus
		if( scrnpos->x > m_WindowPosition.x+9 && scrnpos->x < m_WindowPosition.x+337 &&		 // Equiped item area
			scrnpos->z > m_WindowPosition.y+74 && scrnpos->z < m_WindowPosition.y+206 )
			return TRUE;
	}

	return FALSE;
}

BOOL _XWindow_CInfo::Process( _XGUIObject*& pfocusobject  )
{
	if(!this->m_Enable)		return FALSE;
	if(!this->m_ShowWindow) 
	{		
		return FALSE;
	}

	// �ֱ� ������ ��ȣ ����Ʈ
	if( m_FavoritesNickNameList->GetShowStatus() )
	{
		if( m_FavoritesNickNameList->CheckMousePosition() )
			m_FavoritesNickNameList->Process(pfocusobject);
	}

	_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
	if( pTradeMessageBox )
	{
		if(pfocusobject == _XDEF_DUMMYID_EQUIPEDSLOT) pfocusobject = NULL;
		
		if(!_XWindow::Process(pfocusobject)) return FALSE;
		if( pTradeMessageBox->GetShowStatus() ) return FALSE;
	}
	
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if(CheckMousePosition())
				{
					// Focus
					if( scrnpos->x > m_WindowPosition.x+9 && scrnpos->x < m_WindowPosition.x+337 &&		 // Equiped item area
						scrnpos->z > m_WindowPosition.y+74 && scrnpos->z < m_WindowPosition.y+206 )
					{
						if( !pfocusobject || pfocusobject == this )
							pfocusobject = _XDEF_DUMMYID_EQUIPEDSLOT;
					}

					int clickedslot = SearchSlotRegion();
					
					if( g_pLocalUser->m_bMatchPRMode ) // �� ȫ���϶� ���� �κ� ó�� ����.
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2386), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
					else
					{
						if( clickedslot > -1 )
						{
							if(g_pLocalUser->m_UserItemList[clickedslot].Get_m_sID() > 0) // ���Կ� �������� �ִ°�츸 �巡�� ����
							{
								if(!g_pInventory_Window->m_bHaveMouseSlot)
								{
									// ���콺 down ��ġ ����
									m_IconDragStartPos.x = scrnpos->x;
									m_IconDragStartPos.y = scrnpos->z;
									m_LastDraggingSlot = clickedslot;
									m_bDragState = TRUE;
								}
								else
								{
									// click �� ��ġ slot ����
									m_LastDraggingSlot = clickedslot;
								}
							}
							else // �κ��丮�ʿ��� �巡��Ǿ�����
							{
								// click �� ��ġ slot ����
								m_LastDraggingSlot = clickedslot;
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
					// �κ��丮 ���� Ŭ����
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )						
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
					}					

					if( g_pInventory_Window->m_bHaveMouseSlot )
					{
						if( g_pInventory_Window )
						{
							if( g_pInventory_Window->CheckDropArea() )						
							{
								if(!g_pInventory_Window->ProcessItemDrop())
								{	
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();								
									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									m_LastDraggingSlot = -1;							
								}				
							}
						}
					}					
				}
			}
			if(mousestate->bButton[1])
			{
				BOOL clickedwindowregion = CheckMousePosition();

				int clickedslot = SearchSlotRegion();
				//Author : ����� //breif : ������ �巡�� �ÿ� �κ��丮���� �ϴ� �˻簡 �����ִ�. //Date : 08/04/02
				if( clickedslot != -1 && !g_pInventory_Window->CheckItemEquipState() )
					return FALSE;

				if( g_pLocalUser->m_bMatchPRMode ) // �� ȫ���϶� ���� �κ� ó�� ����.
				{
					if( clickedwindowregion )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2386), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
				}
				else
				{
					// ������ Ŭ��
					if( clickedwindowregion )
					{
						m_WindowMouseState = _XW_STATE_RIGHTCLICK; 
					}
					else
					{
						if(g_pInventory_Window->m_bHaveMouseSlot)
						{
							g_pInventory_Window->m_bHaveMouseSlot = FALSE;
							g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
						}
					}
				}
			}
		}
		else if(pfocusobject == _XDEF_DUMMYID_EQUIPEDSLOT)
		{
			if(mousestate->bButton[0])
			{
				//Author : ����� //breif : ������ �巡�� �ÿ� �κ��丮���� �ϴ� �˻簡 �����ִ�. //Date : 08/04/02
				if( !g_pInventory_Window->CheckItemEquipState() )
					return FALSE;

				// �巡�� ó��
				if(m_bDragState && !g_pInventory_Window->m_bHaveMouseSlot)
				{
					if(scrnpos->x != m_IconDragStartPos.x || scrnpos->z != m_IconDragStartPos.y)
					{
						if(m_LastDraggingSlot != -1)
						{
							if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0)
							{
								g_pInventory_Window->m_bHaveMouseSlot = TRUE;
								g_pInventory_Window->m_MouseSlotItem.Copy(g_pLocalUser-> m_UserItemList[m_LastDraggingSlot]);									
								g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
								m_LastDraggingSlot = -1;

								if( g_pDefaultTooltip_Window )
								{
									if( g_pDefaultTooltip_Window->GetShowStatus() )						
										g_pDefaultTooltip_Window->ShowWindow( FALSE );
								}
							} 
						}
					}
				} // if(m_bDragState && !m_bHaveMouseSlot)
			}
			else
			{
				if(scrnpos->x == m_IconDragStartPos.x && scrnpos->z == m_IconDragStartPos.y)
				{					
					// ���ڸ����� Ŭ�� = ���� ����
					if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0)
					{
						g_pInventory_Window->m_MouseSlotItem.Copy(g_pLocalUser->m_UserItemList[m_LastDraggingSlot]);
						g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;						
						
						if( g_pDefaultTooltip_Window )
						{
							int type = g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType();
							int id = g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID();
							if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_CINFOITEM, m_LastDraggingSlot, type, id ) )
							{
								g_pDefaultTooltip_Window->ShowWindow( TRUE );
							}
						}					

						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
						m_SelectedSlotIndex = m_LastDraggingSlot;
						m_LastDraggingSlot = -1;
					}
					else
					{
						g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
						g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
						
						// icon�� ���� slot Ŭ��						
						m_LastDraggingSlot = -1;
						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
						m_SelectedSlotIndex = -1;

						if( g_pDefaultTooltip_Window )
						{
							if( g_pDefaultTooltip_Window->GetShowStatus() )						
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
				}
				else
				{
					if(m_bDragState && g_pInventory_Window->m_bHaveMouseSlot)
					{
						int selectedslot = SearchSlotRegion();
						if(selectedslot == g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber)
						{
							// drag�� ���¿��� �� slot��  ���������� ���� - Reset
							g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
							g_pInventory_Window->m_bHaveMouseSlot = FALSE;
							m_SelectedSlotIndex = m_LastDraggingSlot;
							m_LastDraggingSlot = -1;
							m_bDragState = FALSE;
						}
						else
						{
							m_bDragState = FALSE;
						}
					}
					
					if(g_pInventory_Window->m_bHaveMouseSlot)
					{
						ProcessInvenMove();
					}
				}
			}
		}
		
		if(!mousestate->bButton[0])
		{
			if(pfocusobject == _XDEF_DUMMYID_EQUIPEDSLOT)
			{
			   pfocusobject = NULL;
			}
		}

		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				ProcessDblClick();
			}
		}
	}


	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	return TRUE;
}

void _XWindow_CInfo::SetParameter( void )
{
	FLOAT gaugefactor = 0;

	if(g_UserLevelTable[ g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1 ].needjinexperience > 0)
	{
		gaugefactor = ((FLOAT)g_pLocalUser->m_CharacterInfo.Get_jinlevel() / 
							 (FLOAT)g_UserLevelTable[ g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1 ].needjinexperience) * 100;
	}
	else
	{
		gaugefactor = 0;
	}
	m_pGiBorder->SetGaugeFactor( gaugefactor );

	FLOAT furyparameter = ((FLOAT)g_pLocalUser->m_CharacterInfo.Get_furyparameter() / 10000.0f)*100.0f;
	m_pFuryBorder->SetGaugeFactor( furyparameter );

	if( g_pLocalUser->m_CharacterInfo.Get_leftpoint() > 0 )
	{
		for( int i = 0; i < 5; i++ )
		{
			BOOL btnenable = TRUE;
			switch( i ) 
			{
			case 0 :
				if( g_UsePointTable[ g_pLocalUser->m_CharacterInfo.Get_cClass() ].need_strength > g_pLocalUser->m_CharacterInfo.Get_leftpoint() ) btnenable = FALSE;
				break;
			case 1 :
				if( g_UsePointTable[ g_pLocalUser->m_CharacterInfo.Get_cClass() ].need_zen > g_pLocalUser->m_CharacterInfo.Get_leftpoint() ) btnenable = FALSE;
				break;
			case 2 :
				if( g_UsePointTable[ g_pLocalUser->m_CharacterInfo.Get_cClass() ].need_intelligence > g_pLocalUser->m_CharacterInfo.Get_leftpoint() ) btnenable = FALSE;
				break;
			case 3 :
				if( g_UsePointTable[ g_pLocalUser->m_CharacterInfo.Get_cClass() ].need_constitution > g_pLocalUser->m_CharacterInfo.Get_leftpoint() ) btnenable = FALSE;
				break;
			case 4 :
				if( g_UsePointTable[ g_pLocalUser->m_CharacterInfo.Get_cClass() ].need_dexterity > g_pLocalUser->m_CharacterInfo.Get_leftpoint() ) btnenable = FALSE;
				break;
			}
			
			if( g_CurrentFocusedObject == (_XGUIObject*)m_ModifyButton[i] )
			{
				g_CurrentFocusedObject = NULL;
			}

			if( btnenable )
			{
				m_ModifyButton[i]->EnableWindow( TRUE );

				m_ModifyButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,178,0, 190,12 );
				m_ModifyButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 178,12,190,24 );
				m_ModifyButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 178,24,190,36 );
			}
			else
			{
				m_ModifyButton[i]->EnableWindow( FALSE );
				
				m_ModifyButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,166,0, 178,12 );
				m_ModifyButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 166,0, 178,12 );
				m_ModifyButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 166,0, 178,12 );
			}
		}
	}
	else
	{
		for( int i = 0; i < 5; i++ )
		{
			if( g_CurrentFocusedObject == (_XGUIObject*)m_ModifyButton[i] )
			{
				g_CurrentFocusedObject = NULL;
			}

			m_ModifyButton[i]->EnableWindow( FALSE );

			m_ModifyButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,166,0, 178,12 );
			m_ModifyButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 166,0, 178,12 );
			m_ModifyButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 166,0, 178,12 );
		}
	}
}


void _XWindow_CInfo::DrawEquippedItemList(void)
{
	// �ָӴ� �׸���   ----------------------------��
	if( g_pLocalUser->m_cAmorPocketNum >= 0 )
	{
		int pos = _XINVENTORY_SLOTTYPE_ACCESSORY_1;
		for( int i = 0; i < 4; i++ )
		{
			if( i < g_pLocalUser->m_cAmorPocketNum )
			{
				m_PocketImageStatic.Draw( m_WindowPosition.x + g_EquippedItemCoord[pos].x-1, m_WindowPosition.y + g_EquippedItemCoord[pos].y-1 );

				m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_CLOTHES].Draw( m_WindowPosition.x + g_EquippedItemCoord[pos].x-1,
					m_WindowPosition.y + g_EquippedItemCoord[pos].y-1 );
			}
			else
			{
				m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_DISABLESLOT].Draw( m_WindowPosition.x + g_EquippedItemCoord[pos].x,
					m_WindowPosition.y + g_EquippedItemCoord[pos].y );				
			}
			
			pos++;
		}		
	}
	if( g_pLocalUser->m_cPantsPocketNum >= 0 )
	{
		int pos = _XINVENTORY_SLOTTYPE_ACCESSORY_5;
		for( int i = 0; i < 4; i++ )
		{
			if( i < g_pLocalUser->m_cPantsPocketNum )
			{
				m_PocketImageStatic.Draw( m_WindowPosition.x + g_EquippedItemCoord[pos].x-1, m_WindowPosition.y + g_EquippedItemCoord[pos].y-1 );

				m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_CLOTHES].Draw( m_WindowPosition.x + g_EquippedItemCoord[pos].x-1,
					m_WindowPosition.y + g_EquippedItemCoord[pos].y-1 );
			}
			else
			{
				m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_DISABLESLOT].Draw( m_WindowPosition.x + g_EquippedItemCoord[pos].x,
					m_WindowPosition.y + g_EquippedItemCoord[pos].y );
			}
			
			pos++;
		}
	} //---------------------------------------��
	
	for( int i = _XINVENTORY_SLOTTYPE_AMOR; i < _XINVENTORY_SLOTTYPE_POCKETSTART; i++ )
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0)
		{
            BOOL bEnable = TRUE;
			switch(i) 
			{
			case _XINVENTORY_SLOTTYPE_AMOR :
			case _XINVENTORY_SLOTTYPE_PANTS :
			case _XINVENTORY_SLOTTYPE_ADAE :
			case _XINVENTORY_SLOTTYPE_SHOES :
			case _XINVENTORY_SLOTTYPE_CAP :
			case _XINVENTORY_SLOTTYPE_GAP_UPPER :
			case _XINVENTORY_SLOTTYPE_GAP_LOWER :
			case _XINVENTORY_SLOTTYPE_MASK :
			//case _XINVENTORY_SLOTTYPE_CLOTH_RESERVE1 :
			//case _XINVENTORY_SLOTTYPE_CLOTH_RESERVE2 :
			case _XINVENTORY_SLOTTYPE_BACKPACK1 :
			case _XINVENTORY_SLOTTYPE_BACKPACK2 :
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
			case _XINVENTORY_SLOTTYPE_CASHBACKPACK1 :		
			case _XINVENTORY_SLOTTYPE_CASHBACKPACK2 :
		#endif
				{
					m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_CLOTHES].Draw( m_WindowPosition.x + g_EquippedItemCoord[i].x-1, 
																			   m_WindowPosition.y + g_EquippedItemCoord[i].y-1 );
				}
				break;
			case _XINVENTORY_SLOTTYPE_WEAPON_1 :
			case _XINVENTORY_SLOTTYPE_WEAPON_2 :	
				{
					m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_WEAPON].Draw( m_WindowPosition.x + g_EquippedItemCoord[i].x-1, 
																			  m_WindowPosition.y + g_EquippedItemCoord[i].y-1 );
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
                    //Author : ����� //breif : �켱 ������ ..
                    int nUse = 0;
                    if( i >= _XINVENTORY_SLOTTYPE_ACCESSORY_1 &&
                        i <= _XINVENTORY_SLOTTYPE_ACCESSORY_4 )
                    {
                        nUse = i - 11; // �������� �������,, ����?  1 2 3 4
                        if( nUse > g_pLocalUser->m_cAmorPocketNum ) //���� ��Ĺ������,, �� ����Ǿ�����
                            bEnable = FALSE;
                    }
                    else
                    {
                        nUse = i - 15; // ���� 1 2 3 4
                        if( nUse > g_pLocalUser->m_cPantsPocketNum ) //���� ��Ĺ������,, �� ����Ǿ�����
                            bEnable = FALSE;
                    }
					m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_ACCESSORY].Draw( m_WindowPosition.x + g_EquippedItemCoord[i].x-1, 
																				 m_WindowPosition.y + g_EquippedItemCoord[i].y-1 );
				}
				break;
			case _XINVENTORY_SLOTTYPE_EXTRA_POCKET :	
				break;									
			}

			g_pInventory_Window->DrawIcon(TRUE,m_WindowPosition.x + g_EquippedItemCoord[i].x, 
				m_WindowPosition.y + g_EquippedItemCoord[i].y, &g_pLocalUser->m_UserItemList[i], TRUE, bEnable);
		}
		else
		{
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
			if( i != _XINVENTORY_SLOTTYPE_CLOTH_RESERVE1 && i != _XINVENTORY_SLOTTYPE_CLOTH_RESERVE2 && 
				i != _XINVENTORY_SLOTTYPE_EXTRA_POCKET )
			m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_EMPTY].Draw( m_WindowPosition.x + g_EquippedItemCoord[i].x-1, 
																		 m_WindowPosition.y + g_EquippedItemCoord[i].y-1 );
		#else
			if( i != _XINVENTORY_SLOTTYPE_CLOTH_RESERVE1 && i != _XINVENTORY_SLOTTYPE_CLOTH_RESERVE2 && 
				i != _XINVENTORY_SLOTTYPE_EXTRA_POCKET && i != _XINVENTORY_SLOTTYPE_RESERVE1 && 
				i != _XINVENTORY_SLOTTYPE_RESERVE2 )
			m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_EMPTY].Draw( m_WindowPosition.x + g_EquippedItemCoord[i].x-1, 
																		 m_WindowPosition.y + g_EquippedItemCoord[i].y-1 );
		#endif
		}		
	}	
	// �ι�° �賶 ����
	if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() <= 0 )
	{
		m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_DISABLESLOT].Draw( m_WindowPosition.x + g_EquippedItemCoord[_XINVENTORY_SLOTTYPE_BACKPACK2].x,
			m_WindowPosition.y + g_EquippedItemCoord[_XINVENTORY_SLOTTYPE_BACKPACK2].y );
	}

	#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    //�켱 ĳ�� �೶�� �ϳ��� ��
    m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_DISABLESLOT].Draw( m_WindowPosition.x + g_EquippedItemCoord[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].x,
                m_WindowPosition.y + g_EquippedItemCoord[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].y );

	if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
	{
        if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
        {
            m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_DISABLESLOT].Draw( m_WindowPosition.x + g_EquippedItemCoord[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].x,
                m_WindowPosition.y + g_EquippedItemCoord[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].y );
        }
	}
	#endif
}

void _XWindow_CInfo::DrawSelectedItem( void )
{
	RECT iconrect;
	int		selectedslot = 0;
	_XInventoryEquipErrorCode	checkslottype = _XEQUIPERROR_NO;

	selectedslot = SearchSlotRegion(&iconrect);

	if(selectedslot != -1)
	{
		checkslottype = g_pInventory_Window->CheckSlotType(selectedslot);
	
		if(g_pInventory_Window->m_bHaveMouseSlot)
		{
			_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right+1, iconrect.bottom+1, 1.0f, (checkslottype == _XEQUIPERROR_NO) ? _XSC_ITEM_SELECT : _XSC_WARNING );
		}
		
		// mouse hover
		if( (checkslottype != _XEQUIPERROR_NO) && g_pLocalUser->m_UserItemList[selectedslot].Get_m_sID() > 0 )
		{
			if( m_SelectedSlotIndex == selectedslot )
				_XDrawRectAngle(iconrect.left, iconrect.top, iconrect.right+1, iconrect.bottom+1, 1.0f, _XSC_ITEM_CLICK);
		}
	}

	if( g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_AMOR &&
		g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber < _XINVENTORY_SLOTTYPE_POCKETSTART )
	{
		if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sID() > 0 )
		{
			iconrect.left   = m_WindowPosition.x+g_EquippedItemCoord[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].x-1;
			iconrect.top    = m_WindowPosition.y+g_EquippedItemCoord[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].y-1;
			iconrect.right  = m_WindowPosition.x+g_EquippedItemCoord[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].x + 29;
			iconrect.bottom = m_WindowPosition.y+g_EquippedItemCoord[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].y + 29;
			
			if(g_pInventory_Window->m_bHaveMouseSlot)
			{
				// disable icon image
				_XDrawSolidBar(iconrect.left, iconrect.top, iconrect.right, iconrect.bottom, D3DCOLOR_ARGB(160, 0, 0, 0));
			}
			
			_XDrawRectAngle( iconrect.left, iconrect.top, iconrect.right+1, iconrect.bottom+1, 1.0f, _XSC_ITEM_SELECT );
		}		
	}
}


int	_XWindow_CInfo::SearchSlotRegion(  RECT* pRect  )
{
	ScrnPos* scrnpos	= gpInput->GetMouseVirScreenPos();
			
	// ���� ���� �˻�
	for( int iter = _XINVENTORY_SLOTTYPE_AMOR; iter < _XINVENTORY_SLOTTYPE_POCKETSTART; iter++ )
	{
		if( ((scrnpos->x > m_WindowPosition.x+g_EquippedItemCoord[iter].x )   && 
			 (scrnpos->x < m_WindowPosition.x+g_EquippedItemCoord[iter].x+32) &&
			 (scrnpos->z > m_WindowPosition.y+g_EquippedItemCoord[iter].y )   && 
			 (scrnpos->z < m_WindowPosition.y+g_EquippedItemCoord[iter].y+32) ) )
		{			
			if( pRect )
			{
				pRect->left   = m_WindowPosition.x+g_EquippedItemCoord[iter].x-1;
				pRect->top    = m_WindowPosition.y+g_EquippedItemCoord[iter].y-1;
				pRect->right  = m_WindowPosition.x+g_EquippedItemCoord[iter].x + 29;
				pRect->bottom = m_WindowPosition.y+g_EquippedItemCoord[iter].y + 29;
			}
			return iter;
		}		
	}

	return -1;
}

extern void SingleModeChangeItem( char cFrom, char cTo );

void _XWindow_CInfo::ProcessInvenMove(void)
{
	if(m_LastDraggingSlot != -1)
	{
		if(m_LastDraggingSlot == g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber)
		{
			// �ڱ��ڸ��� �ٽ� �ֱ�
			g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
			g_pInventory_Window->m_bHaveMouseSlot = FALSE;
			m_bDragState = FALSE;
			m_LastDraggingSlot = -1;
		}
		else if( (g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_ACCESSORY_1+g_pLocalUser->m_cAmorPocketNum &&
				  g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber <= _XINVENTORY_SLOTTYPE_ACCESSORY_4) ||
				 (g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_ACCESSORY_5+ g_pLocalUser->m_cPantsPocketNum &&
				  g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber <= _XINVENTORY_SLOTTYPE_ACCESSORY_8)	)
		{
			// ��Ȱ��ȭ�� �Ǽ����� �̵��Ұ� ó��
			g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
			g_pInventory_Window->m_bHaveMouseSlot = FALSE;
			m_bDragState = FALSE;
			m_LastDraggingSlot = -1;
		}	
		else
		{
			
			if( g_pLocalUser->m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
				g_pLocalUser->m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY )
			{
				//�λ糪 �ڼ� ���϶��� �̵� �Ұ� 
				// ��� �޼���?
				// ���� ������ ���¿����� �ٽ� �����ϱ� ������ �ٸ� �ൿ�� �� �� ����.
				return;
			}


			if(g_pInventory_Window->m_bLockInventory)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTMOVE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�ٸ� ���� �ϴ� �߿��� �೶�� ��ǰ�� �̵��� �� �����ϴ�."
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_bDragState = FALSE;
				g_pInventory_Window->m_LastDraggingSlot = -1;
				return;
			}
			
			if(m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_AMOR && m_LastDraggingSlot <= _XINVENTORY_SLOTTYPE_BACKPACK2 )
			{
				// ���� ����
				_XInventoryEquipErrorCode temperrorcode = g_pInventory_Window->CheckSlotType(m_LastDraggingSlot);
				
				if( temperrorcode == _XEQUIPERROR_NO )
				{
					BOOL bInvenMove = TRUE;

					// �Ǽ��縮 �� �� ���� ������ �ָӴ� ���� üũ
					if(m_LastDraggingSlot >= _XINVENTORY_SLOTTYPE_ACCESSORY_1 && m_LastDraggingSlot <= _XINVENTORY_SLOTTYPE_ACCESSORY_8)
					{
						
						#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : ����� //breif : ��Ʈ �������� ��� ���� ��Ʈ ������ �Ǽ��縮�� �Ѱ���..
						int nSetGroup = _XGameItem::GetSetItem(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType(), 
															   g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sID());
						if( nSetGroup )
						{
							for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1; i <= _XINVENTORY_SLOTTYPE_ACCESSORY_8; i++ )
							{
								if( g_pLocalUser->m_UserItemList[i].Get_m_cType() == g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() &&
									g_pLocalUser->m_UserItemList[i].Get_m_sID() == g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sID() )
								{
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3793), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, FALSE );
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_bDragState = FALSE;
									g_pInventory_Window->m_LastDraggingSlot = -1;
									return;
								}
							}
						}
						#endif

                        // Author : ����� //breif :�̻��ϴ�.... _XEQUIPERROR_NO �̰ɷ� �����µ�...
						// �Ǽ��縮 â�� ��������� �����Ѵ�. 
						/*if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() <= 0)
						{
							
							char itemtype	= g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].m_cType;
							short itemid	= g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sID();

							// �Ǽ������̸� secondŸ���� �˻��Ѵ�.
							if( itemtype != _XGI_FC_ACCESSORY)
							{								
								// ��ȯ�� �ƴ϶� ����
								int usePocketCount = 0;
								for(int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1 ; i <= _XINVENTORY_SLOTTYPE_ACCESSORY_8 ; ++i)
								{
									if(g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0)
										usePocketCount++;
								}
								if(((g_pLocalUser->m_cAmorPocketNum+g_pLocalUser->m_cPantsPocketNum)-usePocketCount) <= 0)
								{
									// ��/���� ���� ���� ���Ѱ� - ������ accessory ���� <= 0 �̸� ���� ������ ���� ����
									bInvenMove = FALSE;
								}
							}
							else// �Ǽ������̸� secondŸ���� �˻��Ѵ�.
							{
								if( g_AccessoryItemProperty[itemid].cSecondType != _XGI_SC_ACCESSORY_ARMOR_MULTI 
								&& g_AccessoryItemProperty[itemid].cSecondType != _XGI_SC_ACCESSORY_ARMOR )
								{					
									// ��ȯ�� �ƴ϶� ����
									int usePocketCount = 0;
									for(int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1 ; i <= _XINVENTORY_SLOTTYPE_ACCESSORY_8 ; ++i)
									{
										if(g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0)
											usePocketCount++;
									}
									if(((g_pLocalUser->m_cAmorPocketNum+g_pLocalUser->m_cPantsPocketNum)-usePocketCount) <= 0)
									{
										// ��/���� ���� ���� ���Ѱ� - ������ accessory ���� <= 0 �̸� ���� ������ ���� ����
										bInvenMove = FALSE;
									}
								}
								else
								{
									// secondŸ���� _XGI_SC_ACCESSORY_ARMOR_MULTI�̰ų� _XGI_SC_ACCESSORY_ARMOR�̸�
									// �Ǽ��縮 â�� ������ �� ���� �Ѵ�.
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORPOS), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // �ָӴϰ� ���� á���ϴ�.
									
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									m_bDragState = FALSE;
									m_LastDraggingSlot = -1;
									return;
								}
							}
						}
						else// ��ȯ�� ���� Ȯ���Ѵ�.
						{
							char itemtype	= g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].m_cType;
							short itemid	= g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sID();
							
							// �Ǽ������̸� secondŸ���� �˻��Ѵ�.
							if( itemtype != _XGI_FC_ACCESSORY)
							{								
								// ��ȯ�� �ƴ϶� ����
								int usePocketCount = 0;
								for(int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1 ; i <= _XINVENTORY_SLOTTYPE_ACCESSORY_8 ; ++i)
								{
									if(g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0)
										usePocketCount++;
								}
								if(((g_pLocalUser->m_cAmorPocketNum+g_pLocalUser->m_cPantsPocketNum)-usePocketCount) <= 0)
								{
									// ��/���� ���� ���� ���Ѱ� - ������ accessory ���� <= 0 �̸� ���� ������ ���� ����
									bInvenMove = FALSE;
								}
							}
							else// �Ǽ������̸� secondŸ���� �˻��Ѵ�.
							{								
								if( g_AccessoryItemProperty[itemid].cSecondType == _XGI_SC_ACCESSORY_ARMOR_MULTI 
									|| g_AccessoryItemProperty[itemid].cSecondType == _XGI_SC_ACCESSORY_ARMOR )
								{
									// secondŸ���� _XGI_SC_ACCESSORY_ARMOR_MULTI�̰ų� _XGI_SC_ACCESSORY_ARMOR�̸�
									// �Ǽ��縮 â�� ������ �� ���� �Ѵ�.
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORPOS), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // �ָӴϰ� ���� á���ϴ�.
									
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									m_bDragState = FALSE;
									m_LastDraggingSlot = -1;
									return;
								}
							}
						}*/
					}
					
					// ��ü
					if(bInvenMove)
					{
						g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
						m_LastDraggingSlot = -1;
#ifdef _XDWDEBUG
						if(g_SinglePlayMode)
							SingleModeChangeItem(g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber, m_LastDraggingSlot);
#endif
					}
					else
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERROREMPTYPOCKET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // �ָӴϰ� ���� á���ϴ�.

						g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
						m_bDragState = FALSE;
						m_LastDraggingSlot = -1;
					}
				}
				else
				{
					switch(temperrorcode) 
					{
					case _XEQUIPERROR_TYPE :
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORPOS), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "��ǰ�� ���� ��ġ�� Ʋ���ϴ�."
						break;
					case _XEQUIPERROR_CLAN :
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORGROUP), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�ٸ� ������ �����Դϴ�."
						break;
					case _XEQUIPERROR_SEX :
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORSEX), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "������ �ٸ� ��ǰ�Դϴ�."
						break;
					case _XEQUIPERROR_POCKET :
						//g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORPOCKET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�� ���� �ָӴϰ� ���ڶ� ��ü�� �� �����ϴ�."
                        g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_POCKET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// �ָӴϸ� ����� ���� ���� ���� �� �ֽ��ϴ�.
						break;
					case _XEQUIPERROR_ACCESSORY :
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERROREMPTYPOCKET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // �ָӴϰ� ���� á���ϴ�.
						break;					
					case _XEQUIPERROR_QUESTHOLDITEM:
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2002), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); 
						break;
					case _XEQUIPERROR_LEVEL :
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2003), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); 
						break;
					case _XEQUIPERROR_ACCOVERLAPPING :
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2004), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); 
						break;
					case _XEQUIPERROR_FAMEOVERLAPPING :
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2637), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xffff2323);
						break;
					case _XEQUIPERROR_NOOVERLAPPING :
						// �ش� ��ǰ�� �ߺ��ؼ� ������ �� �����ϴ�.
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
					case _XEQUIPERROR_OTHER :						
					default:
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_NOTEQUIP), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "��ǰ�� ������ �� �����ϴ�."
						break;
					}
					
					g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
					g_pInventory_Window->m_bHaveMouseSlot = FALSE;
					m_bDragState = FALSE;
					m_LastDraggingSlot = -1;
				}				
			}
		}
	} // if(m_LastDraggingSlot != -1)
}

void _XWindow_CInfo::ProcessDblClick(void)
{
	if( g_pLocalUser->m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
		g_pLocalUser->m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY )
	{
		//�λ糪 �ڼ� ���϶��� �̵� �Ұ� 
		// ��� �޼���?
		// ���� ������ ���¿����� �ٽ� �����ϱ� ������ �ٸ� �ൿ�� �� �� ����.
		return;
	}

	if(g_pInventory_Window->m_bLockInventory)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTMOVE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�ٸ� ���� �ϴ� �߿��� �೶�� ��ǰ�� �̵��� �� �����ϴ�."
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		m_bDragState = FALSE;
		m_LastDraggingSlot = -1;		
		m_WindowMouseState = _XW_STATE_NONE;
		return;
	}
	
	if( CheckMousePosition() )
	{
		// ������ ��ư ó��
		BOOL	resetmouse = TRUE;
		
		m_LastDraggingSlot = SearchSlotRegion();

		//Author : ����� //breif : ������ �巡�� �ÿ� �κ��丮���� �ϴ� �˻簡 �����ִ�. //Date : 08/04/02
		if( m_LastDraggingSlot != -1 && !g_pInventory_Window->CheckItemEquipState() )
			return;			
		
		if( m_LastDraggingSlot != -1 )
		{
			if(g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() > 0)
			{
				g_pInventory_Window->m_bHaveMouseSlot = TRUE;
				g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber = m_LastDraggingSlot;
				g_pInventory_Window->m_MouseSlotItem.Set_m_cType( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_cType() );
				g_pInventory_Window->m_MouseSlotItem.Set_m_sID( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_sID() );
				g_pInventory_Window->m_MouseSlotItem.Set_m_ucCount( g_pLocalUser->m_UserItemList[m_LastDraggingSlot].Get_m_ucCount() );
				
				// �ָӴ� �˻�
				if( m_LastDraggingSlot == _XINVENTORY_SLOTTYPE_AMOR )
				{
					for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_1; i < _XINVENTORY_SLOTTYPE_ACCESSORY_1+g_pLocalUser->m_cAmorPocketNum; i++ )
					{
						if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
						{
							resetmouse = FALSE;
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_POCKET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // �ָӴϸ� ����� ���� ���� ���� �� �ֽ��ϴ�. 
						}
					}							
				}
				else if( m_LastDraggingSlot == _XINVENTORY_SLOTTYPE_PANTS )
				{
					for( int i = _XINVENTORY_SLOTTYPE_ACCESSORY_5; i < _XINVENTORY_SLOTTYPE_ACCESSORY_5+g_pLocalUser->m_cPantsPocketNum; i++ )
					{
						if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
						{
							resetmouse = FALSE;
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_POCKET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // �ָӴϸ� ����� ���� ���� ���� �� �ֽ��ϴ�.
						}
					}
				}
				// ����ȿ� �������� ����ִ��� �˻�
				else if( m_LastDraggingSlot == _XINVENTORY_SLOTTYPE_BACKPACK1 )
				{
					if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0 )
					{
						resetmouse = FALSE;
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2005), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
					}
					else
					{
						//�ι�° �賶 ������ġ�� ����ġ
						int end = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; // g_pLocalUser->m_cBackPackExtend; ���Ŀ� �賶���Լ� ���� ����� �����Ұ�.
						
						for( int i = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; i < end; i++ )
						{
							if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
							{
								resetmouse = FALSE;
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_EMPTYIBACKPACK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�೶�� ��ǰ�� ��� �� �õ� �Ͻʽÿ�."
								break;
							}
						}

						if( resetmouse )
						{
							// ������ �೶�� ������ �� �⺻ �ָӴ� ������ �ִ��� �˻��Ѵ�.
							BOOL emptybaseslot = FALSE;

							for( i = _XINVENTORY_SLOTTYPE_POCKETSTART; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; i++ )
							{
								if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
								{
									emptybaseslot = TRUE;
									break;
								}
							}
					#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
							//ĳ�� �೶�� �˻��Ѵ�
							if( !emptybaseslot )
							{
								for( i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++ )
								{
									if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
									{
										emptybaseslot = TRUE;
										break;
									}
								}
							}
					#endif

							if( emptybaseslot == FALSE )
							{
								resetmouse = FALSE;
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2006), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
							}
						}
					}
				}
				else if( m_LastDraggingSlot == _XINVENTORY_SLOTTYPE_BACKPACK2 )
				{
					int end = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART+25;//+g_pLocalUser->m_cBackPackExtend; //���Ŀ� �賶���Լ� ���� ����� �����Ұ�.

					for( int i = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; i < end; i++ )
					{
						if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
						{
							resetmouse = FALSE;
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_CINFO_EMPTYIBACKPACK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�೶�� ��ǰ�� ��� �� �õ� �Ͻʽÿ�."
							break;
						}
					}

					if( resetmouse )
					{
						// ������ �೶�� ������ �� �⺻ �ָӴ� ������ �ִ��� �˻��Ѵ�.
						BOOL emptybaseslot = FALSE;
						
						if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 ) // Backpack 1 �� ������ ��� Backpack���� �˻�
						{
							for( i = _XINVENTORY_SLOTTYPE_POCKETSTART; i < _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; i++ )
							{
								if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
								{
									emptybaseslot = TRUE;
									break;
								}
							}
						} 
						else  // Backpack 1 �� �������� ���� ��� �⺻ �ָӴϸ� �˻� (���� ��Ȳ)
						{
							for( i = _XINVENTORY_SLOTTYPE_POCKETSTART; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; i++ )
							{
								if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
								{
									emptybaseslot = TRUE;
									break;
								}
							}
						}

					#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
						//�� ������ ������ ĳ�� �೶�� �˻��Ѵ�
						if( !emptybaseslot )
						{
							for( i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++ )
							{
								if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
								{
									emptybaseslot = TRUE;
									break;
								}
							}
						}
					#endif
						
						if( emptybaseslot == FALSE )
						{
							resetmouse = FALSE;
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2006), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
						}
					}
					
					// �� ��° �೶�� ���� �ι�° �೶ ����Ʈ�� ���� ��� ���� ó��
					{

					}
				}
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
				else if( m_LastDraggingSlot == _XINVENTORY_SLOTTYPE_CASHBACKPACK1 )
				{
                    resetmouse = FALSE;
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3825), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//�Ⱓ�� �೶�� ���� ������ �� �����ϴ�.

					/*if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() > 0 )
					{
						resetmouse = FALSE;
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2005), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
					}
					else
					{
						int end = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART + 25; // g_pLocalUser->m_cBackPackExtend; ���Ŀ� �賶���Լ� ���� ����� �����Ұ�.
						
						for( int i = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART; i < end; i++ ) //�������� �ֳ� �˻�
						{
							if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
							{
								resetmouse = FALSE;
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_EMPTYIBACKPACK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�೶�� ��ǰ�� ��� �� �õ� �Ͻʽÿ�."
								break;
							}
						}

						if( resetmouse )
						{
							// ������ �೶�� ������ �� �⺻ �ָӴ� ������ �ִ��� �˻��Ѵ�.
							BOOL emptybaseslot = FALSE;
							
							int nPackend = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; //�⺻���� ������ ��
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 )
							{
								nPackend = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; //�����ִ�.
								if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0 )
									nPackend = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART; //���� �� �ִ�.
							}

							for( i = _XINVENTORY_SLOTTYPE_POCKETSTART; i < nPackend; i++ )
							{
								if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
								{
									emptybaseslot = TRUE;
									break;
								}
							}

                            //�೶�� �� �� �ι�° ĳ���೶�� ������ ���� ���ϹǷ� �˻��� �ʿ� ����
							if( emptybaseslot == FALSE )
							{
								resetmouse = FALSE;
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2006), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
							}
						}
                    }*/
				}
				else if( m_LastDraggingSlot == _XINVENTORY_SLOTTYPE_CASHBACKPACK2 )
				{
                    resetmouse = FALSE;
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3825), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	

					/*int end = _XINVENTORY_SLOTTYPE_TOTALCOUNT;
					
					for( int i = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART; i < end; i++ )
					{
						if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
						{
							resetmouse = FALSE;
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_EMPTYIBACKPACK), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�೶�� ��ǰ�� ��� �� �õ� �Ͻʽÿ�."
							break;
						}
					}

					if( resetmouse )
					{
						// ������ �೶�� ������ �� �⺻ �ָӴ� ������ �ִ��� �˻��Ѵ�.
						BOOL emptybaseslot = FALSE;

						int nPackend = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; //�⺻���� ������ ��
						if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 )
						{
							nPackend = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; //�����ִ�.
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0 )
								nPackend = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART; //���� �� �ִ�.
						}

						for( i = _XINVENTORY_SLOTTYPE_POCKETSTART; i < nPackend; i++ )
						{
							if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
							{
								emptybaseslot = TRUE;
								break;
							}
						}

						//�� ������ ������ ĳ�� �೶�� �˻��Ѵ� 
						if( !emptybaseslot )
						{
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 ) //���⼱ ĳ���೶�� �ϳ���
								end = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART;
							else
								end = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART; // ĳ�� �೶�� �˻� ���ϰ� ��

							for( i = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART; i < end; i++ )
							{
								if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
								{
									emptybaseslot = TRUE;
									break;
								}
							}
						}

						if( emptybaseslot == FALSE )
						{
							resetmouse = FALSE;
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_2006), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
						}
					}*/
                }
		#endif
				if( resetmouse )
				{
					// inventory ���� �� ���� ã��
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
					int count			= _XINVENTORY_SLOTTYPE_POCKETSTART; //ó���� ����
					
					int totalcount = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; //�⺻ 50
					if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 && m_LastDraggingSlot != 23)
					{
						totalcount = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; //75
						if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0 && m_LastDraggingSlot != 24)
							totalcount =  _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART; //100
					}

					//�⺻ �೶ �˻�
					for(; count < totalcount; count++ )
					{
						if( g_pLocalUser->m_UserItemList[count].Get_m_sID() <= 0 )
						{
							m_LastDraggingSlot = count;
                            resetmouse = FALSE; //�⺻�೶�� ������ �����Ƿ� ĳ�� �೶�� �˻����� �ʴ´�.
							g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
							break;
						}
					}

                    if( resetmouse )
                    {
                        //�೶�� �κ����� ���� ó��,,
                        count = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART;
                        if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 && m_LastDraggingSlot != 21 ) //ĳ�� 2�೶�� ������ ĳ�� 1�೶�� ���� ����.
                        {
                            totalcount = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART; //�ٸ� ��ǰ�̳� ĳ��2 �೶�� ���ٸ� ���⼭ �ɸ� ��
                            if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() > 0 && m_LastDraggingSlot != 22 )
                                totalcount = _XINVENTORY_SLOTTYPE_TOTALCOUNT;
                        }
                        else
                        {
                            //��Ʈ ������ 21,22 �̸� ĳ���೶�� ���� ���� �׷��� ĳ���೶�� �˻��ϸ� �ȵ�
                            //���� ĳ�� 1�೶�� ���� ĳ�� 2�೶, �ٸ� ��ǰ�� �� ��
                            totalcount = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART;

                            if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 && m_LastDraggingSlot != 21) //ĳ�� 1�೶�� ���µ� �ٸ� �� �� ��
                            {
                                if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() > 0 && m_LastDraggingSlot != 22 )
                                {
                                    count = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART; //�ι��� ĳ���೶�� �˻����� �ٸ� ��ǰ�� ���Ƿ�
                                    totalcount = _XINVENTORY_SLOTTYPE_TOTALCOUNT;
                                }
                            }
                        }

					    for(count; count < totalcount; count++ )
					    {
						    if( g_pLocalUser->m_UserItemList[count].Get_m_sID() <= 0 ) //�����
						    {
							    m_LastDraggingSlot = count;
							    g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
							    break;
						    }
					    }
                    }
				#else
					if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0 ) // Backpack2 ������ 
					{
						for( int i = _XINVENTORY_SLOTTYPE_POCKETSTART; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; i++ )
						{
							if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
							{
								m_LastDraggingSlot = i;
								g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
								break;
							}
						}
					}
					else if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 ) // Backpack1 ������
					{
						for( int i = _XINVENTORY_SLOTTYPE_POCKETSTART; i < _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; i++ )
						{
							if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
							{
								m_LastDraggingSlot = i;
								g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
								break;
							}
						}
					}
					else
					{
						for( int i = _XINVENTORY_SLOTTYPE_POCKETSTART; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; i++ )
						{
							if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
							{
								m_LastDraggingSlot = i;
								g_NetworkKernel.SendPacket(MSG_NO_INVEN_MOVE, 0, m_LastDraggingSlot );
								break;
							}
						}
					}
				#endif
				}						
			}
		}
		
		// Mouse slot ���
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;		
		
		m_WindowMouseState = _XW_STATE_NONE;
		m_LastDraggingSlot = -1;				
	}
}

void _XWindow_CInfo::InsertNickNameIndex(int index)
{
	for( int i = 1; i < 212; i++ )
	{		
		if( g_pLocalUser->m_CharacterInfo.m_NickNameList[i] == index )
		{
			index = i;
			break;
		}		
	}

	i = 8;
	SIZE size = m_FavoritesNickNameList->GetWindowSize();
	size.cy = 0;
	while (i != -1) 
	{
		if( m_FavoritesNickNameIndexList[i] > 0 )
		{
			m_FavoritesNickNameIndexList[i+1] = m_FavoritesNickNameIndexList[i];			
			size.cy += 15;			
		}
		i--;
	}	
	m_FavoritesNickNameIndexList[0] = index;
	size.cy += 15;
	size.cy += 4;
	m_FavoritesNickNameList->SetWindowSize( size.cx, size.cy );
	m_FavoritesNickNameList->MoveWindow(m_WindowPosition.x+179,m_WindowPosition.y+34);
	_XStatic* tempstatic = m_FavoritesNickNameList->GetBorderStatic();
	tempstatic->MoveWindow(m_WindowPosition.x+179, m_WindowPosition.y+34, m_WindowPosition.x+179+size.cx, m_WindowPosition.y+34+size.cy);
}

void _XWindow_CInfo::SetFavoritesNickNameList(void)
{
	m_FavoritesNickNameList->DeleteAllItem();

	int count = 0;
	int tableindex = 0;

	for( int i = 0; i < 10; i++ )
	{
		if( m_FavoritesNickNameIndexList[i] > 0 )
		{
			for( int j = 0; j < g_MaxNickNameCount; j++)
			{
				if( g_pLocalUser->m_CharacterInfo.m_NickNameList[m_FavoritesNickNameIndexList[i]] > 0 )
				{
					if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[m_FavoritesNickNameIndexList[i]] )
					{
						tableindex = j;
						break;
					}
				}				
			}

			TCHAR tempnickstring[64];
			memset( tempnickstring, 0, sizeof(TCHAR)*64 );	
			int length = strlen( g_NickNameInfoTable[tableindex].cNickName );
			if( length > 24 )
			{
				if( g_NickNameInfoTable[tableindex].cNickName[22] & 0x80 ) //�ѱ��϶� 2����Ʈ 
				{
					strncpy( tempnickstring, g_NickNameInfoTable[tableindex].cNickName, sizeof(TCHAR)*21 );	
					strcat( tempnickstring, _T("..") );
				}
				else
				{
					strncpy( tempnickstring, g_NickNameInfoTable[tableindex].cNickName, sizeof(TCHAR)*22 );	
					strcat( tempnickstring, _T("..") );
				}
			}
			else
			{
				strcpy( tempnickstring, g_NickNameInfoTable[tableindex].cNickName );
			}
			
			m_FavoritesNickNameList->InsertListItem(_T(""), 0, i+1 );		
			m_FavoritesNickNameList->SetItemText(count, 0, tempnickstring);
			m_FavoritesNickNameList->SetItemAttrib(count, 0, 0xFFC9CAC6);
			m_FavoritesNickNameList->SetItemAttribHighlight(count, 0, 0xFFC9CAC6);				
			count++;
		}
	}	
}

void _XWindow_CInfo::DrawFameTooltip(int select, int posx, int posy )
{
	TCHAR tempbuf[128];
	memset( tempbuf, 0, sizeof(TCHAR) * 128);

	int rank = -1;
	if( select == 0 )
	{
		rank = g_FameStr->GetFameIndex(g_pLocalUser->m_CharacterInfo.Get_honor());
		if( rank != -1 )
		{
			sprintf( tempbuf, "%s %d%s   %s",	
				_XGETINTERFACETEXT(ID_STRING_NETWORK_REPUTATION), rank+1, 
				_XGETINTERFACETEXT(ID_STRING_QUEST_STEP), g_FameStr[rank].m_FameName );
		}
	}
	else if( select == 1 )
	{
		rank = g_FameStr->GetInfamyIndex(g_pLocalUser->m_CharacterInfo.Get_Notoriety());
		if( rank != -1 )
		{
			sprintf( tempbuf, "%s %d%s   %s", _XGETINTERFACETEXT(ID_STRING_NEW_2636), rank+1, //_T("�Ǹ�")
				_XGETINTERFACETEXT(ID_STRING_QUEST_STEP), g_FameStr[rank].m_InfamyName );
		}
	}
	if( rank != -1 )
		g_InterfaceTooltip.SetToolTipString(posx, posy, tempbuf, _XSC_INFORMATION, TRUE);
}

BOOL _XWindow_CInfo::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( gpInput->CheckMousePos( m_WindowPosition.x + 6, m_WindowPosition.y + 253, m_WindowPosition.x + 341, m_WindowPosition.y + 364 ) )
	{
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			int MoveDelta  = (FLOAT)zDelta / 40;			
			
			if( zDelta > 0 )
			{
				m_pListScrollBar->LineUp( abs(MoveDelta) );
			}
			else
			{
				m_pListScrollBar->LineDown( abs(MoveDelta) );
			}
		}
		return TRUE;
	}
	
	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}
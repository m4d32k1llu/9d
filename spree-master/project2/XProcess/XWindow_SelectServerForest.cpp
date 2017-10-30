// XWindow_SelectServerForest.cpp: implementation of the _XWindow_SelectServerForest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_SelectServerForest.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_SelectServerForest::_XWindow_SelectServerForest()
{

}

_XWindow_SelectServerForest::~_XWindow_SelectServerForest()
{

}

BOOL _XWindow_SelectServerForest::Initialize( _XTextureManager& texturearchive )
{	
	_XImageStatic* pLeftBorder = new _XImageStatic;
	pLeftBorder->Create( 0, 0, 162, 215,
					   &texturearchive, 
					   texturearchive.FindResource( "server_base_01.tga" ) );	
	pLeftBorder->SetClipRect( 0, 0, 163, 216 );
	InsertChildObject( pLeftBorder );
	
	_XImageStatic* pRightBorder = new _XImageStatic;
	pRightBorder->Create( 162, 0, 162+164, 213,
					   &texturearchive, 
					   texturearchive.FindResource( "server_base_02.tga" ) );	
	pRightBorder->SetClipRect( 0, 0, 165, 214 );
	InsertChildObject( pRightBorder );	

	int scrollbarresourceindex = g_MainInterfaceTextureArchive.FindResource( "scrollbar.tga" );
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,								// ������ Ȱ��ȭ
		{ 60,   42 },						// ������ ��ǥ
		{ 209, 100 },						// ������ ������
		_XDEF_SELECTSERVERLISTCTRL,			// ������ ���̵�
		-1,									// ������ border �̹��� �ε���
		&g_MainInterfaceTextureArchive,		// Texture ����Ʈ
		_T(" "),							// ������ Ÿ��Ʋ
		14,									// ����Ʈ ������ ���� ��ġ
		7,									// ����Ʈ �ڽ��� ���̴� �ִ� ���μ�
		_XLSTYLE_RIGHTSCROLLBAR,			// ��ũ�ѹ��� ��ġ
		
		// Scroll bar property
		{10, 101},
		10, 
		20,
		100,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex
	};
	
	m_pListBox = new _XListBox;
	m_pListBox->Create(listboxstructure);
	
	POINT coloffset = { 2, 2 };
	m_pListBox->SetColumnOffset(0, coloffset);	
	m_pListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_pListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);	
	m_pListBox->SetColumnWidth(0, 140 );

	coloffset.x = 144;
	m_pListBox->SetColumnOffset(1, coloffset);
	m_pListBox->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_pListBox->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_pListBox->SetColumnWidth(1, 65 );

	m_pListBox->SetTrackWidth(195);
	m_pListBox->SetTrackHeight(14);
	m_pListBox->SetTrackBarColor( D3DCOLOR_ARGB(128,0,0,0) );
	m_pListBox->EnableSelectItem(TRUE);
	
	_XVScrollBar* pScrollBar = m_pListBox->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 10, 10 },
			{ 20, 0, 30, 10 },
			{ 0, 10, 10, 20 },
			{ 20,10, 30, 20 },
			{ 20,20, 30, 40 }			
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}
	
	InsertChildObject(m_pListBox);	

	return TRUE;
}

void _XWindow_SelectServerForest::ShowWindow( BOOL show )
{
	m_pListBox->SelectItem( -1 );
	_XWindow::ShowWindow( show );
}

void _XWindow_SelectServerForest::MoveWindow( int X, int Y )
{		
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_SelectServerForest::DestroyWindow( void )
{	
	_XWindow::DestroyWindow();
}

void _XWindow_SelectServerForest::Draw( _XGUIObject*& pfocusobject )
{			
	_XWindow::Draw(pfocusobject);	
		
	if( !this->m_ShowWindow ) return;	

	DWORD lefttime = 10 - ((g_LocalSystemTime - ((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime) / 1000);
	TCHAR cancelstring[128];
	memset(cancelstring, 0, sizeof(TCHAR)*128);
	LoadString(gHInst, IDS_WARNLOGINCANCELSTRING, cancelstring, 127);

	g_XBaseFont->SetColor( _XSC_INFORMATION );
	g_XBaseFont->PrintAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + m_WindowSize.cy - 52,
							1.0f, _XFONT_ALIGNTYPE_CENTER, cancelstring, lefttime );
	g_XBaseFont->Flush();
}

BOOL _XWindow_SelectServerForest::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}





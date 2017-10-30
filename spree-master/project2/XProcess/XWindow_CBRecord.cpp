// XWindow_CBRecord.cpp: implementation of the _XWindow_CBRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_CBRecord.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_CBRecord::_XWindow_CBRecord()
{
	m_lstboxOrgRecord = NULL;
	m_nRecordCount = 0;
}

_XWindow_CBRecord::~_XWindow_CBRecord()
{

}

BOOL _XWindow_CBRecord::Initialize()
{
	int manorresourceindex	= g_MainInterfaceTextureArchive.FindResource("MI_manor03.tga");

	// Ÿ��Ʋ �� ( 187 X 19 )
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 187, 19, &g_MainInterfaceTextureArchive, manorresourceindex );
	pTitleBar->SetClipRect( 2, 214, 189, 233 );
	InsertChildObject( pTitleBar );
	
	// �ٴ� �� ( 187 X 19 )
	_XImageStatic* pBottomBar = new _XImageStatic;
	pBottomBar->Create( 0, 166, 187, 185, &g_MainInterfaceTextureArchive, manorresourceindex );
	pBottomBar->SetClipRect( 2, 235, 189, 254 );
	InsertChildObject( pBottomBar );

	// list box
	int scrollresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_chat_quick.tga");

	// ó�� �ʿ�
	_XLISTBOX_STRUCTURE recordlistbox = 
	{
		TRUE,
		{5, 19},										// position
		{177, 147},										// size
		_XDEF_CBRECORD_LISTBOX,							// command id
		-1,												// window texture index
		&g_MainInterfaceTextureArchive,					// texture archive
		_T(""),											// window title
		29,												// y pitch
		5,												// ���̴� �ִ� ���μ�
		_XLSTYLE_RIGHTSCROLLBAR,						// ��ũ�ѹ� ��ġ

		// scrollbar property
		{ 12, 147 },							// ��ũ�ѹ� ������
		12,										// ��ũ�ѹ� ���� ��ư ������
		22,										// ��ũ�ѹ� Ʈ���� ������
		255,									// ��ü ����Ʈ ����
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex
	};

	m_lstboxOrgRecord = new _XListBox;
	m_lstboxOrgRecord->Create(recordlistbox);

	_XVScrollBar* pScrollBar = m_lstboxOrgRecord->GetScrollBarObject();
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
	m_lstboxOrgRecord->SetAlwaysShowScrollBarMode(FALSE);
	
	// ó�� �ʿ�
	POINT offset1 = {   7,  2};
	POINT offset2 = {  25,  2};
	POINT offset3 = {  59, 16};
	POINT offset4 = { 100, 16};
	POINT offset5 = { 132, 16};

	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		offset2.x = 54;
		offset5.x = 137;
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		offset4.x = 106;
		offset5.x = 137;
	}

	m_lstboxOrgRecord->SetColumnOffset(0, offset1);
	m_lstboxOrgRecord->SetColumnOffset(1, offset2);
	m_lstboxOrgRecord->SetColumnOffset(2, offset3);
	m_lstboxOrgRecord->SetColumnOffset(3, offset4);
	m_lstboxOrgRecord->SetColumnOffset(4, offset5);

	m_lstboxOrgRecord->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_lstboxOrgRecord->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_lstboxOrgRecord->SetColumnStyle(2, _XLCOLUMNSTYLE_STRING);
	m_lstboxOrgRecord->SetColumnStyle(3, _XLCOLUMNSTYLE_STRING);
	m_lstboxOrgRecord->SetColumnStyle(4, _XLCOLUMNSTYLE_STRING);

	m_lstboxOrgRecord->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_lstboxOrgRecord->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_lstboxOrgRecord->SetAlignType(2, _XFONT_ALIGNTYPE_RIGHT);
	m_lstboxOrgRecord->SetAlignType(3, _XFONT_ALIGNTYPE_RIGHT);
	m_lstboxOrgRecord->SetAlignType(4, _XFONT_ALIGNTYPE_RIGHT);

	// ó�� �ʿ�
	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
		m_lstboxOrgRecord->SetColumnWidth(0, 42);
	else
		m_lstboxOrgRecord->SetColumnWidth(0, 12);

	m_lstboxOrgRecord->SetColumnWidth(1, 138);
	m_lstboxOrgRecord->SetColumnWidth(2, 34);
	m_lstboxOrgRecord->SetColumnWidth(3, 34);
	m_lstboxOrgRecord->SetColumnWidth(4, 34);

	// ó�� �ʿ�
	m_lstboxOrgRecord->SetTrackWidth(165);
	m_lstboxOrgRecord->SetTrackHeight(29);
	m_lstboxOrgRecord->EnableSelectItem(FALSE);
	InsertChildObject(m_lstboxOrgRecord);

	return TRUE;
}

void _XWindow_CBRecord::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_CBRecord::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		_XDrawSolidBar( m_WindowPosition.x+2, m_WindowPosition.y+19, m_WindowPosition.x+2+183, m_WindowPosition.y+19+147 , 0xFF000000 );

		// ����
		_XDrawLine2D( m_WindowPosition.x+3, m_WindowPosition.y+19, m_WindowPosition.x+3, m_WindowPosition.y+19+147, D3DCOLOR_ARGB(255,115,93,69), D3DCOLOR_ARGB(255,115,93,69));
		_XDrawLine2D( m_WindowPosition.x+183, m_WindowPosition.y+19, m_WindowPosition.x+183, m_WindowPosition.y+19+147, D3DCOLOR_ARGB(255,115,93,69), D3DCOLOR_ARGB(255,115,93,69));

		// ����
		_XDrawLine2D( m_WindowPosition.x+3, m_WindowPosition.y+47, m_WindowPosition.x+3+167, m_WindowPosition.y+47, D3DCOLOR_ARGB(255,115,93,69), D3DCOLOR_ARGB(255,115,93,69));
		_XDrawLine2D( m_WindowPosition.x+3, m_WindowPosition.y+76, m_WindowPosition.x+3+167, m_WindowPosition.y+76, D3DCOLOR_ARGB(255,115,93,69), D3DCOLOR_ARGB(255,115,93,69));
		_XDrawLine2D( m_WindowPosition.x+3, m_WindowPosition.y+105, m_WindowPosition.x+3+167, m_WindowPosition.y+105, D3DCOLOR_ARGB(255,115,93,69), D3DCOLOR_ARGB(255,115,93,69));
		_XDrawLine2D( m_WindowPosition.x+3, m_WindowPosition.y+134, m_WindowPosition.x+3+167, m_WindowPosition.y+134, D3DCOLOR_ARGB(255,115,93,69), D3DCOLOR_ARGB(255,115,93,69));
		_XDrawLine2D( m_WindowPosition.x+3, m_WindowPosition.y+163, m_WindowPosition.x+3+167, m_WindowPosition.y+163, D3DCOLOR_ARGB(255,115,93,69), D3DCOLOR_ARGB(255,115,93,69));
	}

	_XWindow::Draw(pfocusobject);
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// TODO
}

BOOL _XWindow_CBRecord::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;

	// TODO
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_CBRecord::DeleteAll(void)
{
	m_lstboxOrgRecord->DeleteAllItem();
	m_lstboxOrgRecord->SetScrollPos(0);
	m_nRecordCount = 0;
}

void _XWindow_CBRecord::InsertRecord(_PerOrgRecord orgrecord)
{
	TCHAR messagestr[128];

	BOOL bFound = FALSE;
	TCHAR namestr[128];
	int index = 0;

	TCHAR teststr[256];
	memset( teststr, 0, sizeof(teststr));

	memset(namestr, 0, sizeof(namestr));
	if(orgrecord.iAllyIndex == 0)
	{
		sprintf(namestr, _XGETINTERFACETEXT(ID_STRING_NEW_3634), orgrecord.vsOrgName);				// "%s��"
	}
	else
	{
		sprintf(namestr, _XGETINTERFACETEXT(ID_STRING_NEW_3634), orgrecord.vsOrgName);				// "%s��"
		strcat(namestr, "(");
		
		TCHAR tempstr[128];
		sprintf(tempstr, _XGETINTERFACETEXT(ID_STRING_NEW_3125), orgrecord.vsAllyName);				//%sȸ

		strcat(namestr, tempstr);
		strcat(namestr, ")");
	}

	for(int i = 0 ; i < m_nRecordCount ; ++i)
	{
		if(strcmp(m_lstboxOrgRecord->GetItemText(i, 1), namestr) == 0)
		{
			bFound = TRUE;
			index = i;
		}
	}

	if(!bFound)
	{
		index = m_nRecordCount;
		m_nRecordCount++;
	}

	m_lstboxOrgRecord->InsertListItem(_T(""), 0, index+1);

	m_lstboxOrgRecord->SetItemText(index, 0, _XGETINTERFACETEXT(ID_STRING_NEW_3635));		//"��"
	m_lstboxOrgRecord->SetItemAttrib(index, 0, D3DCOLOR_ARGB(255, 250, 2, 2));
	m_lstboxOrgRecord->SetItemAttribHighlight(index, 0, D3DCOLOR_ARGB(255, 250, 2, 2));

	m_lstboxOrgRecord->SetItemText(index, 1, namestr);
	m_lstboxOrgRecord->SetItemAttrib(index, 1, D3DCOLOR_ARGB(255, 206, 206, 208));
	m_lstboxOrgRecord->SetItemAttribHighlight(index, 1, D3DCOLOR_ARGB(255, 206, 206, 208));

	memset(messagestr, 0, sizeof(messagestr));
	sprintf(messagestr, "%%d");
	strcat(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_2656) ); //��
	sprintf(messagestr, messagestr, orgrecord.usWinCount);				// 

	m_lstboxOrgRecord->SetItemText(index, 2, messagestr);
	m_lstboxOrgRecord->SetItemAttrib(index, 2, D3DCOLOR_ARGB(255, 74, 208, 51));
	m_lstboxOrgRecord->SetItemAttribHighlight(index, 2, D3DCOLOR_ARGB(255, 74, 208, 51));

	memset(messagestr, 0, sizeof(messagestr));
	sprintf(messagestr, "%%d");
	strcat(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_2657) ); //��
	sprintf(messagestr, messagestr, orgrecord.usLoseCount);			
	//sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3564)"%d��", orgrecord.usLoseCount);				// 

	m_lstboxOrgRecord->SetItemText(index, 3, messagestr);
	m_lstboxOrgRecord->SetItemAttrib(index, 3, D3DCOLOR_ARGB(255, 253, 71, 68));
	m_lstboxOrgRecord->SetItemAttribHighlight(index, 3, D3DCOLOR_ARGB(255, 253, 71, 68));

	memset(messagestr, 0, sizeof(messagestr));
	sprintf(messagestr, "%d%%", orgrecord.usPercentageOfWin);				// 

	m_lstboxOrgRecord->SetItemText(index, 4, messagestr);
	m_lstboxOrgRecord->SetItemAttrib(index, 4, D3DCOLOR_ARGB(255, 229, 233, 60));
	m_lstboxOrgRecord->SetItemAttribHighlight(index, 4, D3DCOLOR_ARGB(255, 229, 233, 60));
}
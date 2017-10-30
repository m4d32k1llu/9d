// XListBox.h: interface for the _XListBox class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XLISTBOX_H_
#define _XLISTBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XStringItem.h"
#include "XScrollBar.h"
#include "XFont.h"
#include "XWindow.h"

#define _XDEF_MAXCOLUMN		7

typedef enum	
{
	_XLSTYLE_NOSCROLLBAR	=	0,
	_XLSTYLE_LEFTSCROLLBAR	=	1,
	_XLSTYLE_RIGHTSCROLLBAR	=	2	
} _XLISTBOX_STYLE;

typedef enum	
{
	_XLCOLUMNSTYLE_NONE		=	0,
	_XLCOLUMNSTYLE_STRING,
	_XLCOLUMNSTYLE_ICON,	
	_XLCOLUMNSTYLE_ICONRECT	
} _XLISTBOXCOLUMN_STYLE;

typedef struct
{
	// window property
	BOOL				enable;										// ������ Ȱ��ȭ
	POINT				position;									// ������ ��ǥ 
	SIZE				windowsize;									// ������ ������
	DWORD				commandid;									// ������ ���̵� <������>
	int					windowtextureindex;							// ������ ���� �̹��� �ε���
	_XTextureManager*	texturearchive;								// �ؽ��ĸ���Ʈ
	char				WindowTitle[_XDEF_WINDOWTITLESTRINGSIZE];	// ������ Ÿ��Ʋ 
	int					ypitch;										// ����Ʈ ������ ���� ��ġ
	int					maxviewlinecount;							// ����Ʈ �ڽ��� ���̴� �ִ� ���μ� 
	_XLISTBOX_STYLE		listboxstyle;								// ��ũ�ѹ��� ��ġ 

	// Scroll bar property
	SIZE				scrollbarsize;								// ��ũ�ѹ� ������
	int					btnysize;									// ��ũ�ѹ� ���Ϲ�ư ������
	int					trackbarysize;								// ��ũ�ѹ� Ʈ���� ������
	int					totallinecount;								// ��ü ����Ʈ ���� <���� �ʿ����>	

	int					imageindex_uparrow;							// ��ư �̹��� �ε���
	int					imageindex_uparrowclick;
	int					imageindex_downarrow;
	int					imageindex_downarrowclick;
	int					imageindex_trackbar;

} _XLISTBOX_STRUCTURE;

typedef	BOOL (__stdcall *_XGUIListBoxItemSelectCallback)( int param1, int param2 );
typedef	BOOL (__stdcall *_XGUIListBoxItemMouseHoverCallback)( int param1, int param2, int XPos, int YPos );
typedef	void (__stdcall *_XGUIListBoxItemSelectDrawCallback)( int XPos, int YPos, int Width, int height );

class _XListBox : public _XWindow  
{
protected:
	int						m_YPitch;
	int						m_TrackWidth;
	int						m_TrackHeight;
	int						m_SelectedItem;
	int						m_PrevSelectedItem;
	int						m_nMouseOverItem; //Author : �����
	int						m_HoverItem;
	int						m_MaxViewLineCount;
	int						m_CurrentScrollPos;	
	int						m_TextXOffset;							// ��ũ�ѹ��� ��ġ�� ���� Item offset
	BOOL					m_ItemSelectable;
	_XLISTBOX_STYLE			m_ListBoxStyle;

	BOOL					m_IconReportMode;
	int						m_IconModeRowCount;
	int						m_IconModeColumnCount;
	int						m_IconModeRowSize;
	int						m_IconModeColumnSize;
	
	_XList					m_ItemList;
	_XVScrollBar			m_ScrollBar;
	_XGUIObject*			m_pScrbarFocusedObject;	
	_XImageStatic			m_IconImageStatic;
	_XStatic				m_SelectStatic;
	_XStatic				m_SelectedStatic;

	BOOL					m_bAlwaysShowScrollBarMode;
	BOOL					m_bMoveScrollBarMode;
	POINT					m_ScrollBarPositon;
	
	BOOL					m_UnicodeDataMode;
	
	_XImageStatic			m_SelectedImageStatic;
	int						m_SelectedImageHeight;
	POINT					m_SelectedImagePos;
	
	BOOL					m_EnableMouseFocusing;

	_XFONTID				m_FontID;
		
	int						m_ColumnWidth[_XDEF_MAXCOLUMN];
	_XFONT_ALIGNTYPE		m_ColumnAlignType[_XDEF_MAXCOLUMN];
	POINT					m_ColumnOffset[_XDEF_MAXCOLUMN];
	_XLISTBOXCOLUMN_STYLE	m_ColumnStyle[_XDEF_MAXCOLUMN];
	_XFRECT					m_ColumnClipRect[_XDEF_MAXCOLUMN];

	_XGUIListBoxItemSelectCallback			m_ItemSelectCallBack;
	_XGUIListBoxItemMouseHoverCallback		m_ItemMouseHoverCallBack;
	_XGUIListBoxItemSelectDrawCallback		m_ItemSelectDrawCallBack;
	_XGUIListBoxItemSelectDrawCallback		m_ItemClickedDrawCallBack;
public:
	_XListBox();
	virtual ~_XListBox();

	BOOL			Create( _XLISTBOX_STRUCTURE listboxstructure );
	void			DestroyWindow( void );
				
	void			MoveWindow( int X, int Y );
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			ProcessWheelMessage( short zDelta );

	int				GetTextXOffset( void ){ return m_TextXOffset; }
	void			SetTextXOffset( int offset ){ m_TextXOffset = offset; }

	void			SetMaxViewLineCount( int viewlinecount );
	
	int				GetItemCount( void ){ return m_ItemList.getitemcount(); }

	BOOL			InsertListItem( LPTSTR itemstring, DWORD attrib = 0, int commandid = 0, 
									_XTextureManager* pIconArchive = NULL );
	
	void			SetIconArchive( int index, _XTextureManager* pIconArchive );
	_XTextureManager* GetIconArchive( int index );

	BOOL			SetItemText( int index, int column, LPTSTR itemstring, BOOL boldstring = FALSE );
	LPTSTR			GetItemText( int index, int column );

	//�ڵ� �ٹٲ� ��� ����
	BOOL			SetSeparateText( int index, int column, LPTSTR itemstring, int width, BOOL boldstring = FALSE );
	
	BOOL			SetItemAttrib( int index, int column, DWORD attrib );
	DWORD			GetItemAttrib( int index, int column );
	
	BOOL			SetItemAttrib(int index, int column, DWORD attrib, RECT rect);
	BOOL			SetItemAttribHighlight(int index, int column, DWORD attrib);
	
	BOOL			SetItemCommandID( int index, DWORD commandid = 0 );
	DWORD			GetItemCommandID( int index );

	POINT			GetColumnOffsetWindowPos( int count, int offsetnumber ); //Author : ����� 

	void			SetColumnWidth( int column, int width )
	{
					if( column >= _XDEF_MAXCOLUMN ) return;
					m_ColumnWidth[column] = width;

					m_ColumnClipRect[column].left	=	m_WindowPosition.x;
					m_ColumnClipRect[column].right	=	m_WindowPosition.x + m_ColumnClipRect[column].left + width;
	}

	void			SetAlignType( int column, _XFONT_ALIGNTYPE type )
	{
					if( column >= _XDEF_MAXCOLUMN ) return;
					m_ColumnAlignType[column] = type;
	}

	void			SetColumnOffset( int column, POINT offset )
	{
					if( column >= _XDEF_MAXCOLUMN ) return;
					m_ColumnOffset[column] = offset;

					m_ColumnClipRect[column].left	=	m_WindowPosition.x + offset.x;
					m_ColumnClipRect[column].right	=	m_WindowPosition.x + offset.x + m_ColumnWidth[column];
	}
	void			SetColumnOffset( int column, int x, int y )
	{
					if( column >= _XDEF_MAXCOLUMN ) return;
					m_ColumnOffset[column].x = x;
					m_ColumnOffset[column].y = y;
	}

	void			SetColumnStyle( int column, _XLISTBOXCOLUMN_STYLE style )
	{
					if( column >= _XDEF_MAXCOLUMN ) return;
					m_ColumnStyle[column] = style;
	}	

	int				CheckSelectedItem( void );

	void			SetTrackBarColor( D3DCOLOR color )
	{
					m_SelectStatic.SetFaceColor( color );
	}
	void			SetSelectedTrackBarColor( D3DCOLOR color )
	{
					m_SelectedStatic.SetFaceColor( color );
	}

	void			SetTrackWidth ( int width  )
	{				
					m_TrackWidth  = width;
					m_SelectStatic.MoveWindow( 0, 0, m_TrackWidth, m_TrackHeight );
					m_SelectedStatic.MoveWindow( 0, 0, m_TrackWidth, m_TrackHeight );
	}
	void			SetTrackHeight( int height )
	{				
					m_TrackHeight = height;
					m_SelectStatic.MoveWindow( 0, 0, m_TrackWidth, m_TrackHeight );
					m_SelectedStatic.MoveWindow( 0, 0, m_TrackWidth, m_TrackHeight );
	}	

	void			SetIconReportMode( BOOL iconreportmode )
	{
					m_IconReportMode = iconreportmode;
	}

	void			SetIconReportModeInfo( int rowcount, int columncount, int rowsize, int columnsize )
	{
					m_IconModeRowCount			=	rowcount;
					m_IconModeColumnCount		=	columncount;
					m_IconModeRowSize			=	rowsize;
					m_IconModeColumnSize		=	columnsize;
	}
	
	int				GetColumnCount(void)	{	return m_IconModeColumnCount;		};
	
	void			SetWindowCommandID(DWORD commandid)	
	{
					m_CommandID = commandid;
	}
	DWORD			GetWindowCommandID(void)
	{
					return m_CommandID;
	}

	void			SetSelectItemCallBack( _XGUIListBoxItemSelectCallback _callbackfunction )
	{
					m_ItemSelectCallBack = _callbackfunction;
	}
	void			SetSelectItemMouseHoverCallBack( _XGUIListBoxItemMouseHoverCallback _callbackfunction )
	{
					m_ItemMouseHoverCallBack = _callbackfunction;
	}
	void			SetItemSelectDrawCallBack( _XGUIListBoxItemSelectDrawCallback _callbackfunction )
	{	
					m_ItemSelectDrawCallBack = _callbackfunction;
	}
	void			SetItemClickedDrawCallBack( _XGUIListBoxItemSelectDrawCallback _callbackfunction )
	{	
					m_ItemClickedDrawCallBack = _callbackfunction;
	}

	void            EnableSelectItem( BOOL enable ){ m_ItemSelectable = enable; }
	void            EnableMouseFocusing( BOOL enable ){ m_EnableMouseFocusing = enable; }
	void			SelectItem( int index );
	int 			GetSelectedItem( void ){ return m_SelectedItem; }
	int				GetMouseOverItem( void ) const { return m_nMouseOverItem; } //Author : �����
	void			SetSelectedItem( int selectitem ) { m_SelectedItem = selectitem; 	}
	int 			GetPrevSelectedItem( void ){ return m_PrevSelectedItem; }
	int				GetHoverItem( void ) { return m_HoverItem ; }
	void			DeleteItem(  int index  );
	void			DeleteAllItem( void );

	_XVScrollBar*	GetScrollBarObject( void ){ return &m_ScrollBar; }
	int				GetScrollPos( void ){ return m_CurrentScrollPos; }
	void			SetScrollPos(int scrollpos)					{	m_ScrollBar.SetScrollPos(scrollpos);	}
	int				GetLineScrollPos( void ){ return m_ScrollBar.GetLineScrollPos(); }
	void			SetLineScrollPos(int scrollpos)					{	m_ScrollBar.SetLineScrollPos(scrollpos);	}
	
	_XImageStatic*	GetSelectBarObject( void ){ return &m_SelectedImageStatic; }

	// ��ũ�ѹ��� ��ġ�� ũ�⸦ ���Ƿ� �����ϱ� ���� �Լ� 
	void			SetMoveScrollBarMode( BOOL bmovescrollbarmode ) { m_bMoveScrollBarMode = bmovescrollbarmode; }
	void			SetAlwaysShowScrollBarMode( BOOL balwaysshowscrollbarmode ); 
	void			MoveScrollBarPos(int X, int Y);
	void			SetScrollBarHeight(int height){	m_ScrollBar.SetWindowHeight( height );	}

	int				GetSelectedItemCommandID(int selected);
	void			SetSelectedImageArchive(int index, _XTextureManager* pImageArchive);
	void			SetSelectedImageHeight(int height);
	void			SetSelectedImagePosition(int X, int Y);
	
	BOOL			IsValidItem(int index);
	
	void			SetFont( _XFONTID fontid );
	_XFONTID		GetFontID( void ){ return m_FontID; }

	void			SetUnicodeDataMode( BOOL uncodemode ){ m_UnicodeDataMode = uncodemode; }
	BOOL			GetUnicodeDataMode( void ){ return m_UnicodeDataMode; }
	
};

#endif // !defined(_XLISTBOX_H_)

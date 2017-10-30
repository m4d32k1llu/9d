// XTreeBox.h: interface for the _XTreeBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTREEBOX_H__DEE578C8_E8EA_4313_AE80_AAB9AED7F21E__INCLUDED_)
#define AFX_XTREEBOX_H__DEE578C8_E8EA_4313_AE80_AAB9AED7F21E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XStringItem.h"
#include "XScrollBar.h"
#include "XMString.h"
#include "XFont.h"
#include "XWindow.h"

#include <list>
using namespace std;

class _XNodeData 
{
public:
	int						NodeId;
	_XNodeData*				ParentNode;
	int						Index;
	bool					Opened;		
	_XMString				NodeString;
	_XMString				NodeIcon;
	int						Pos;
	list<_XNodeData*>		ChildIdList;
	
	_XNodeData();
	virtual ~_XNodeData();
	
	_XNodeData* FindNodeByIndex(_XNodeData*findNode, int index);
	_XNodeData* FindNodeByCommandId(_XNodeData*findNode, int commandid);
	int GrantIndex(int count);
	int ParentNodeCount(_XNodeData* parentnode, int count);
};

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

} _XTREEBOX_STRUCTURE;

typedef	BOOL (__stdcall *_XGUITreeBoxItemSelectCallback)( int param1, int param2 );
typedef	BOOL (__stdcall *_XGUITreeBoxItemMouseHoverCallback)( int param1, int param2, int XPos, int YPos );

class _XTreeBox : public _XWindow  
{
protected:
	int						m_YPitch;
	int						m_TrackWidth;
	int						m_TrackHeight;
	int						m_SelectedItem;
	int						m_PrevSelectedItem;
	int						m_MaxViewLineCount;
	int						m_CurrentScrollPos;	
	int						m_IndexListCount;
	int						m_TextXOffset;							// ��ũ�ѹ��� ��ġ�� ���� Item offset
	BOOL					m_ItemSelectable;	

	_XNodeData*				m_RootItem;	
	_XVScrollBar			m_ScrollBar;
	_XGUIObject*			m_pScrbarFocusedObject;	
	_XImageStatic			m_IconImageStatic;
	_XStatic				m_SelectStatic;
	_XStatic				m_SelectedStatic;

	BOOL					m_bAlwaysShowScrollBarMode;
	BOOL					m_bMoveScrollBarMode;
	POINT					m_ScrollBarPositon;
	
	_XImageStatic			m_SelectedImageStatic;
	int						m_SelectedImageHeight;
	POINT					m_SelectedImagePos;

	_XFONTID				m_FontID;
		
	int						m_ColumnWidth;
	_XFONT_ALIGNTYPE		m_ColumnAlignType;
	POINT					m_ColumnOffset;	
	_XFRECT					m_ColumnClipRect;

	int						m_IconImage;
	int						m_IconImageEx;
	RECT					m_IconRect;
	RECT					m_IconRectEx;
	int						m_SelectedCommandId;

	_XGUITreeBoxItemSelectCallback			m_ItemSelectCallBack;
	_XGUITreeBoxItemMouseHoverCallback		m_ItemMouseHoverCallBack;
public:
	_XTreeBox();
	virtual ~_XTreeBox();	

	BOOL			Create( _XTREEBOX_STRUCTURE treeboxstructure );
	void			DestroyWindow( void );
				
	void			MoveWindow( int X, int Y );
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			ProcessWheelMessage( short zDelta );

	void			UpdateTreeBox();
	int				GetTextXOffset( void ){ return m_TextXOffset; }
	void			SetTextXOffset( int offset ){ m_TextXOffset = offset; }

	void			SetMaxViewLineCount( int viewlinecount );
	
	int				GetItemCount( void ){ return m_IndexListCount; };

	BOOL			InsertListItem( LPTSTR itemstring, int commandid = 0, int parentid = 0, _XTextureManager* pIconArchive = NULL );
	
	void			SetIconArchive( int index, _XTextureManager* pIconArchive );
	_XTextureManager* GetIconArchive( int index );

	BOOL			SetItemText( int index, int column, LPTSTR itemstring, BOOL boldstring = FALSE );
	LPTSTR			GetItemText( int index, int column );
	
	BOOL			SetItemAttrib( int index, DWORD attrib );
	DWORD			GetItemAttrib( int index );
	
	BOOL			SetItemAttrib(int index, DWORD attrib, RECT rect);
	BOOL			SetItemAttribHighlight(int index, DWORD attrib);
	
	BOOL			SetItemCommandID( int index, DWORD commandid = 0 );
	DWORD			GetItemCommandID( int index );	

	void			SettingIcon(int image1, int image2, RECT rt1, RECT rt2 );

	void			SetColumnWidth( int width )
	{
					
					m_ColumnWidth = width;

					m_ColumnClipRect.left	=	m_WindowPosition.x;
					m_ColumnClipRect.right	=	m_WindowPosition.x + m_ColumnClipRect.left + width;
	}

	void			SetAlignType( _XFONT_ALIGNTYPE type )
	{
					m_ColumnAlignType = type;
	}

	void			SetColumnOffset( POINT offset )
	{
					m_ColumnOffset = offset;

					m_ColumnClipRect.left	=	m_WindowPosition.x + offset.x;
					m_ColumnClipRect.right	=	m_WindowPosition.x + offset.x + m_ColumnWidth;
	}
	void			SetColumnOffset( int x, int y )
	{					
					m_ColumnOffset.x = x;
					m_ColumnOffset.y = y;
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

	void			SetWindowCommandID(DWORD commandid)	
	{
					m_CommandID = commandid;
	}
	DWORD			GetWindowCommandID(void)
	{
					return m_CommandID;
	}

	void			SetSelectItemCallBack( _XGUITreeBoxItemSelectCallback _callbackfunction )
	{
					m_ItemSelectCallBack = _callbackfunction;
	}
	void			SetSelectItemMouseHoverCallBack( _XGUITreeBoxItemMouseHoverCallback _callbackfunction )
	{
					m_ItemMouseHoverCallBack = _callbackfunction;
	}
	int				GetSelectedCommandId() { return m_SelectedCommandId; };

	void            EnableSelectItem( BOOL enable ){ m_ItemSelectable = enable; }
	void			SelectItem( int index );
	int 			GetSelectedItem( void ){ return m_SelectedItem; }
	void			SetSelectedItem( int selectitem ) { m_SelectedItem = selectitem; 	};
	void			SetSelectItemChange( int flag );
	int 			GetPrevSelectedItem( void ){ return m_PrevSelectedItem; }
	void			DeleteItem(  int index  );

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
	
	void			SetFont( _XFONTID fontid );
};

#endif // !defined(AFX_XTREEBOX_H__DEE578C8_E8EA_4313_AE80_AAB9AED7F21E__INCLUDED_)

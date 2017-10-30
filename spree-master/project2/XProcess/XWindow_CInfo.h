// XWindow_CInfo.h: interface for the _XWindow_CInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_CINFO_H_
#define _XWINDOW_CINFO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

/**
 * \brief ���ΰ� �ι� ���� â - ����
 * ���ΰ��� �������� ������, �ɷ�ġ, �⺻���� ����� ������
 * 
*/

/// ���������� Ÿ��
typedef enum tagXEQUIPEDITEMEDGEICONTYPE
{
	_XEQUIPEDITEMEDGEICONTYPE_EMPTY = 0,
	_XEQUIPEDITEMEDGEICONTYPE_WEAPON,
	_XEQUIPEDITEMEDGEICONTYPE_CLOTHES,	
	_XEQUIPEDITEMEDGEICONTYPE_ACCESSORY,
	_XEQUIPEDITEMEDGEICONTYPE_DISABLESLOT,	
} _XEQUIPEDITEMEDGEICONTYPE;

class _XWindow_CInfo : public _XWindow  
{
public:

	_XImageStatic			m_EquipedItemEdgeIcon[5];	///< Ÿ�Կ� ���� �̹���
	_XImageStatic			m_PocketImageStatic;
	
	_XButton*				m_ModifyButton[5];			///< �ɷ�ġ ��� ��ư 
	
	_XStickGauge*			m_pGiBorder;	///< �� ������
	_XStickGauge*			m_pFuryBorder;	///< �г� ������

	_XCheckButton*			m_pNickNameTabbutton[3];	///< ���� ��ȣ��ư�� ����

	int						m_SelectedSlotIndex;
	int						m_LastDraggingSlot;

	// Drag
	BOOL					m_bDragState;
	POINT					m_IconDragStartPos;
	
	POINT					m_PointUpArea[5];
	_XListBox*				m_FavoritesNickNameList;	///< �ֱ� ������ ��ȣ ����Ʈ
	int						m_FavoritesNickNameIndexList[10]; ///< �ֱ� ������ ��ȣ �ε��� ����Ʈ �ִ� 10��
	
	int						m_ScrollFactor;
	_XVScrollBar*			m_pListScrollBar;

public:
	_XWindow_CInfo();
	virtual ~_XWindow_CInfo();
	
	BOOL 		Initialize( void );
	
	void 		MoveWindow( int X, int Y );

	void 		Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL		Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void 		SetDefaultPosition( void );
	void		SaveWindowProperty( void );

	void		ShowWindow(BOOL show);
	void		SetParameter( void );
	
	void		DrawEquippedItemList(void);		///< ������ �����۵� �׸���
	void		DrawIcon( BOOL smallicon, int X, int Y, _XUserItem* pItem = NULL, BOOL bMarkStackable = TRUE , BOOL busingitem = FALSE );
	void		DrawSelectedItem( void );
	int			SearchSlotRegion( RECT* pRect = NULL );	
	BOOL		CheckEquipedArea( void );		///< ������������ üũ
	void		ProcessInvenMove(void);			///< �κ�->����â
	void		ProcessDblClick(void);			
	BOOL		ProcessWheelMessage( short zDelta );
	void		InsertNickNameIndex(int index);	///< �ش� ��ȣ�� ���ã�� ��ȣ����Ʈ�� �߰�
	void		SetFavoritesNickNameList(void);	///< ���ã�� ��ȣ����Ʈ ������( ���ڱ��̰� �� �� ��� )
	
	void		DrawFameTooltip(int select, int posx, int posy); ///< ��, �Ǹ� �ܰ踦 ��Ÿ�� �ִ� ����( 1�ܰ� �̻��� ������ ǥ������)
};

#endif // !defined(AFX_XWINDOW_CINFO_H__554BEFE1_CE9A_49A3_B01E_BDF6A1DB70F9__INCLUDED_)

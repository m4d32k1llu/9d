// XWindow_AddSaveItem.h: interface for the _XWindow_AddSaveItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_ADDSAVEITEM_H__B491BE9D_2FFF_4C9C_B99D_D8A6BEC3867C__INCLUDED_)
#define AFX_XWINDOW_ADDSAVEITEM_H__B491BE9D_2FFF_4C9C_B99D_D8A6BEC3867C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

/**
 * \brief â�� ��� â
 * â���� ������ �Ⱓ ���� �����ϴ� Ŭ����
 *
 * â���� �� ���� �Ҷ� â���� ������ ������ �ε����� ���� â���� ������
 * �����ؾ� �Ѵ�.(������ â�� �ε����� �������� ������)
*/

class _XWindow_AddSaveItem;

typedef void (_XWindow_AddSaveItem::*pFuncProcess)();
typedef void (_XWindow_AddSaveItem::*pFuncDraw)();

class _XWindow_AddSaveItem : public _XWindow  
{
public:	
	
	_XButton*		m_btnSeleted;
	_XButton*		m_btnCancel;
	
	_XCheckButton*	m_checkbtnList[9]; //2004.05.20->oneway48 modify
	
	//	_XListBox*		m_listboxAddSaveItem; //2004.05.20->oneway48 delete
	
	int				m_SelectedItemIndex;	
	
	int				m_AddStorageSize;		///< �߰��ϴ� ĭ�� ũ��	
	int				m_AddStorageTerm;		///< �߰��ϴ� �Ⱓ	
	int				m_AddStorageCost;		///< �߰��ϴµ� ��� ���

private :
	//Auhtor : ����� //breif : â�� Ȯ��  //���� ������
	_XCheckButton* m_pTabButton[2]; //Author : ����� //breif : â�� �̹��� ��ư���� ����

	pFuncProcess m_CurrnetProcess;
	pFuncDraw m_CurrentDraw;
	int m_nSelectTab; //���� ������ ��.. 0 �� ����

    _XImageStatic* m_pUseImageBoder[6]; // ������ �ϳ��� ���ϱ� 3 �ٸ������ 2���� ����ؼ� 6

	void SetProcess( void(_XWindow_AddSaveItem::*pFunc)() ) { m_CurrnetProcess = pFunc;	}
	void SetDraw(void(_XWindow_AddSaveItem::*pFunc)() )	{ m_CurrentDraw = pFunc; }
	
public:
	void Reset();
	int GetListIndex();
	_XWindow_AddSaveItem();
	virtual ~_XWindow_AddSaveItem();
	
	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void				ShowWindow(BOOL show);
	
	/// üũ ��ư Ŭ�� ó��
	void				SetSelectedIndex( int index );
	/// ����� �� �ִ� ������ â�� �˻�
	void				CheckUseSlot(void);
	//	void				SetListItem();

	//Auhtor : ����� //breif : â�� Ȯ�� 
	void OnFirstTabProcess();
	void OnSecondTabProcess();
	void OnFirstDraw();
	void OnSecondDraw();

	void SetSelectTab( const int nNumber );
	const int GetSelectTab() const { return m_nSelectTab; }
	const int GetReserveType() const { return m_SelectedItemIndex; }
};

#endif // !defined(AFX_XWINDOW_ADDSAVEITEM_H__B491BE9D_2FFF_4C9C_B99D_D8A6BEC3867C__INCLUDED_)

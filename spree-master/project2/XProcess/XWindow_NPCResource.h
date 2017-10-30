// XWindow_NPCResource.h: interface for the _XWindow_NPCResource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NPCRESOURCE_H__0CE3C03E_6D26_4354_912B_3D746FE8EF0A__INCLUDED_)
#define AFX_XWINDOW_NPCRESOURCE_H__0CE3C03E_6D26_4354_912B_3D746FE8EF0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_NPCResource  : public _XWindow
{
public:	
	_XImageStatic*		m_imageNeeItemBorder;			// �ʿ������ ���� �̹���
	_XImageStatic*		m_imageCompleteNeeItemBorder;	// ���� ���� �ʿ������ ���� �̹���
	_XListBox*			m_listboxResourceItem;			// �ڿ��� ǥ���ϴ� ����Ʈ �ڽ�
	_XButton*			m_btnExchange;					// �ڿ����� ��ȯ�ϴ� ��ư 
	int					m_indexSelectedPackage;			// NPC�� ������ �ִ� ��Ű�� �ε���
	int					m_indexSelectedResourceItem;	// ���õ� �ڿ� ������ �ε���
	bool				m_bProcessExchange;				// ��ȯ������ �ƴ��� �� �� �ִ� ���� 
	_XStatic			m_BorderTile;
	
public:
	_XWindow_NPCResource();
	virtual ~_XWindow_NPCResource();
	
	BOOL			Initialize();
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	void			SetListItem(void);							// �ڿ� ����Ʈ�� �����Ѵ�.
	void			SetSelectedResourceItem(int resourceid);	// ���õ� �ڿ� �������� �ε����� ����.
	void			SetPackageIndex(int index)	{ m_indexSelectedPackage = index; };

	void			SetProgressExchange(bool bexchange )	{ m_bProcessExchange = bexchange; };
	bool			GetProgressExchange(void)		{ return m_bProcessExchange; };					
	
	bool			CheckInventorySpace(void);					// ������ ���� �� �ִ� ������ �ִ��� üũ�Ѵ�.

	inline void SetStrength( bool bRes ) { m_bStrength = bRes; }
	inline bool GetStrength() const { return m_bStrength; }

private:
	bool m_bStrength; //Author : ����� // breif : ���ÿ��ο� ���� ����
};

#endif // !defined(AFX_XWINDOW_NPCRESOURCE_H__0CE3C03E_6D26_4354_912B_3D746FE8EF0A__INCLUDED_)

// XWindow_NPCCollection.h: interface for the _XWindow_NPCCollection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NPCCOLLECTION_H__C76E0C72_0E4E_4F46_B780_BA9503C5FBDB__INCLUDED_)
#define AFX_XWINDOW_NPCCOLLECTION_H__C76E0C72_0E4E_4F46_B780_BA9503C5FBDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XCollectionItem.h"

class _XWindow_NPCCollection  : public _XWindow
{
public:	
	_XImageStatic*		m_imageSelectedRewardItemBorder;		// ���õ� ��������� ���� �̹���
	_XImageStatic*		m_imageRewardItemBorder;				// ��������� ���� �̹���
	_XButton*			m_btnRewardSet[4];						// ���� ��Ʈ�� �����ϴ� ��ư
	_XListBox*			m_listboxCollectionItem;				// ������ ǥ���ϴ� ����Ʈ �ڽ�
	_XCollectionItem*	m_pSelectedCollectionItem;				// ���õ� ���� ������
	int					m_indexSelectedRewardSet;				// ���õ� ���� ��Ʈ �ε���
	bool				m_bProcessSelectRewardSet;
	bool				m_bRewardSuccess;						// ����޾����� �ߴ� �޼��� ó��
	
	_XStatic			m_BorderTile;
	
private :
	int					m_nOwnerNPCId;							// â�� �� npc id
	
public:
	_XWindow_NPCCollection();
	virtual ~_XWindow_NPCCollection();
	
	BOOL			Initialize();
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
	void			SetListItem(void);							// ���� ����Ʈ�� �����Ѵ�.
	void			SetSelectedCollectionItemIndex(int index);	// ���õ� ���� �������� �����Ѵ�
	bool			CheckInventorySpace(int rewardsetindex);	// ������ ���� �� �ִ� ������ �ִ��� üũ�Ѵ�.

	BOOL			IsMyOwnerPackage(int collectionid);			// ���� npc�� ���� ��������ΰ�?
	
	void			SetRewardSetIndex(int index)	{ m_indexSelectedRewardSet = index; };
	int				GetRewardSetIndex(void)			{ return m_indexSelectedRewardSet; };
	
	void			SetProcessReward(bool bprocess)	{ m_bProcessSelectRewardSet = bprocess; };
	bool			GetProcessReward(void)			{ return m_bProcessSelectRewardSet; };

	void			SetOwnerNPCId(int id)			{ m_nOwnerNPCId = id;		};

};
#endif // !defined(AFX_XWINDOW_NPCCOLLECTION_H__C76E0C72_0E4E_4F46_B780_BA9503C5FBDB__INCLUDED_)

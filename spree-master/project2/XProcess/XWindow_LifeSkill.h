// XWindow_LifeSkill.h: interface for the _XWindow_LifeSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_LIFESKILL_H__BE89E98B_FF4C_4D3C_AB54_9543CEFAA9E7__INCLUDED_)
#define AFX_XWINDOW_LIFESKILL_H__BE89E98B_FF4C_4D3C_AB54_9543CEFAA9E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XCollectionItem.h"

typedef enum
{
	_XLIFETAB_LIFECLASS1		= 0,
		_XLIFETAB_LIFECLASS2,
		_XLIFETAB_LIFECLASS3,
		_XLIFETAB_LIFECLASS4,
		_XLIFETAB_LIFECLASS5,
		_XLIFETAB_TOTALCOUNT
}_XLifeWindowTab;

typedef enum
{
	_XCOLLECTION_TYPE1		= 0,	// ��ü ����
		_XCOLLECTION_TYPE2,			// ��ǰ
		_XCOLLECTION_TYPE3,			// ��ǰ
		_XCOLLECTION_TYPE4,			// ��ǰ
		_XCOLLECTION_TYPE5,			// ��ǰ
		_XCOLLECTION_TYPE6,			// ����
		_XCOLLECTION_TYPE7,			// ��ư �߰� 2007.10.18
		_XCOLLECTIONTYPE_TOTALCOUNT
}_XCollectionType;

class _XWindow_LifeSkill  : public _XWindow
{
public:
	// ����
	_XImageStatic*		m_imageSelectedButton;		// ���簢�� ��ư�� ���õǾ��� �� �׵θ� �̹���
	_XImageStatic*		m_imageTypeButtonBorder;	// ���簢�� ��ư�� ���� �̹���
	_XImageStatic*		m_imageItemBorder;			// ������ �׵θ� 
	_XImageStatic*		m_imageCompleteItemBorder;	// ���� ���� �ʿ������ ���� �̹���

	// ���� ���� �� 
	_XImageStatic*		m_imageCollectionTypeBorder1;		// ���� Ÿ�� ���� ���� 1 
	_XImageStatic*		m_imageCollectionTypeBorder2;		// ���� Ÿ�� ���� ���� 2 

	// �ڿ� ���� �� 

	// �ൿ ���� ��
	_XImageStatic*		m_imageStoreBorder1;		// ���� ���� ���� ���� 1 ( 44 X 46 )
	_XImageStatic*		m_imageStoreBorder2;		// ���� ���� ���� ���� 2 ( 100 X 46 )
	_XImageStatic*		m_imageStoreBorder3;		// ���� ���� ���� ���� 3 ( 12 X 46 )
	_XImageStatic*		m_imageStoreBorderEdge1;	// ���� ���� ���� �׵θ� 1 ( 44 X 46 )
	_XImageStatic*		m_imageStoreBorderEdge2;	// ���� ���� ���� �׵θ� 2 ( 100 X 46 )
	_XImageStatic*		m_imageStoreBorderEdge3;	// ���� ���� ���� �׵θ� 3 ( 12 X 46 )
	_XImageStatic*		m_imagePVPBorder1;			// �� ȫ�� ���� ���� 1 ( 44 X 46 )
	_XImageStatic*		m_imagePVPBorder2;			// �� ȫ�� ���� ���� 2 ( 100 X 46 )
	_XImageStatic*		m_imagePVPBorder3;			// �� ȫ�� ���� ���� 3 ( 12 X 46 )
	_XImageStatic*		m_imagePVPBorderEdge1;		// �� ȫ�� ���� �׵θ� 1 ( 44 X 46 )
	_XImageStatic*		m_imagePVPBorderEdge2;		// �� ȫ�� ���� �׵θ� 2 ( 100 X 46 )
	_XImageStatic*		m_imagePVPBorderEdge3;		// �� ȫ�� ���� �׵θ� 3 ( 12 X 46 )
	_XImageStatic*		m_imageEventBorder1;		// �̺�Ʈ ���� ���� 1 ( 44 X 46 )
	_XImageStatic*		m_imageEventBorder2;		// �̺�Ʈ ���� ���� 2 ( 100 X 46 )
	_XImageStatic*		m_imageEventBorder3;		// �̺�Ʈ ���� ���� 3 ( 12 X 46 )
	_XImageStatic*		m_imageEventBorderEdge1;	// �̺�Ʈ ���� �׵θ� 1 ( 44 X 46 )
	_XImageStatic*		m_imageEventBorderEdge2;	// �̺�Ʈ ���� �׵θ� 2 ( 100 X 46 )
	_XImageStatic*		m_imageEventBorderEdge3;	// �̺�Ʈ ���� �׵θ� 3 ( 12 X 46 )
	_XImageStatic*		m_imagePersonalStoreIcon;	// ���λ��� ������ (�ӽ÷� ���)
	_XImageStatic*		m_imagePVPMark;				// �� ��ũ ������ 
	_XImageStatic*		m_imageEventIcon;			// �̺�Ʈ ������ 
	


	// ����
	_XButton*			m_btnLifeSkillClassTab;		// ��Ȱ ����� Ȯ�� �Ǹ� �����ؾ� ��. ���� ����
	_XCheckButton*		m_btnLifeSkillTab[_XLIFETAB_TOTALCOUNT];				// ��Ȱ ��ų ���� : ���� �ڿ� ���� �ൿ ���
	_XButton*			m_btnType[_XCOLLECTIONTYPE_TOTALCOUNT];					// Ÿ�� ��ư : ��ǰ,��ǰ,��ǰ,��ǰor �뵿/������...

	// ���� ���� �� 
	_XButton*			m_btnCollectionNeedItemInput;	// ������ �ʿ��� �������� �� ������ ��ǰ���� �ֱ� ���� ��ư
	
	// �ڿ� ���� �� 

	// �ൿ ���� ��
	_XButton*			m_btnLifeSkillUse;			// ���λ�����ư
	_XButton*			m_btnPVPPR;					// ��ȫ����ư
	_XButton*			m_btnEvent;					// �̺�Ʈ��ư

	_XListBox*			m_listboxCollectionItem;	// ������ ǥ���ϴ� ����Ʈ �ڽ�
	_XListBox*			m_listboxResourceItem;		// �ڿ��� ǥ���ϴ� ����Ʈ �ڽ�
	_XListBox*			m_listboxResourceNeedItem;	// �ڿ��� ǥ���ϴ� ����Ʈ �ڽ�
	_XListBox*			m_lstboxCollectionExplain;	// ��ǰ ���� ���� ���� ǥ���ϴ� ����Ʈ �ڽ�

	_XCollectionItem*	m_pSelectedCollectionItem;	// ���õ� ���� ������
		
	POINT				m_IconDragStartPos;
	POINT				m_CollectionNeedItemSlotPosition[16];	// ��ǰ�������� �ʿ��� ������ ��ġ ����
	POINT				m_ResourcNeedItemSlotPosition[9];		// �ڿ��������� �ʿ��� ������ ��ġ ����

	int					m_indexSelectedResourceItem;	// ���õ� �ڿ� ������ �ε���
	int					m_indexCompleteNeedItem;		// ���� ��Ͽ� ���� �ʿ� ������ �ε��� 
	
	short				m_MouseSlotLifeSkillID;
	
	bool				m_bHaveMouseSlot;
	bool				m_bDragState;
	
	_XLifeWindowTab		m_SelectedTab;
	_XCollectionType	m_SelectedTypeClass;

	int					m_nResourceTabTotalCount;	// ������ ���� �� ���� (�� ���� ���ο� ���� �޶���)
	

public:
	_XWindow_LifeSkill();
	virtual ~_XWindow_LifeSkill();
	
	BOOL				Initialize(void);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void				ShowWindow(BOOL show);
	void				MoveWindow( int X, int Y );
	BOOL				ProcessWheelMessage( short zDelta );

	// ��ǰ ���� 
	BOOL				CheckMousePositionInCollectionItme(void);
	void				ProcessCollectionNeedItem(void);
	void				CheckCollectionCompleteNeedItem(void);
	int					CheckSelectedCollectionNeedItem(void);
	void				SetListItemCollectionExplain(void);				// ��ǰ ����� ����Ʈ�� �����Ѵ�.
	
	// �ڿ� ���� 
	int					CheckSelectedResourceNeedItem(void);

	// �ൿ ���� 
	void				ResetMouseSlot(void);
	int					CheckSelectedIconItem(void);
	void				DrawDragIcon(void);

	void				ChangeTab(_XLifeWindowTab selectedtab);
	void				ChangeType(_XCollectionType selectedtype);
	void				SetTypeButtonTexture(void);
	void				SetListCollectionItem(void);					// ���� ����Ʈ�� �����Ѵ�.
	void				SetListResourceItem(void);						// �ڿ� ����Ʈ�� �����Ѵ�.
	void				SetListResourceNeedItem(BOOL bRenewScroll = TRUE );					// �ڿ��� �����ϴµ� �ʿ��� ����Ʈ�� �����Ѵ�.
	void				SetSelectedCollectionItemIndex(int index);		// ���õ� ���� �������� �����Ѵ�.
	void				SetSelectedResourceItemOrGrupItem(DWORD commandid);				// ���õ� �ڿ� �������� �ε����� ����.
	
	int					SearchSlotRegion(RECT* pRect = NULL);
	
	inline short		GetMouseSlotLifeSkillID(void)	{	return m_MouseSlotLifeSkillID;	};	
	inline bool			GetHaveMouseSlot(void)	{	return m_bHaveMouseSlot;	};
};

#endif // !defined(AFX_XWINDOW_LIFESKILL_H__BE89E98B_FF4C_4D3C_AB54_9543CEFAA9E7__INCLUDED_)

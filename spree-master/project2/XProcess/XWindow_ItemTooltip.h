// XWindow_ItemTooltip.h: interface for the _XWindow_ItemTooltip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_ITEMTOOLTIP_H__09FCD649_CC62_4623_9FE5_F5248B3434DC__INCLUDED_)
#define AFX_XWINDOW_ITEMTOOLTIP_H__09FCD649_CC62_4623_9FE5_F5248B3434DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

class _XWindow_ItemTooltip : public _XWindow  
{
public:
	_XWindow*			m_ParentWindow;
	int					m_HoveredSlotIndex;

	int					m_Category;
	int					m_ID;
	
	// Item Description String ====================
	TCHAR				m_DescNameString[16];				// ���� �̸�
	
	TCHAR				m_DescGenderString[16];				// ���� 2004.06.11->oneway48
	TCHAR				m_DescPrice[16];					// ���� 2004.06.14->oneway48 insert
	
	TCHAR				m_DescClassString[16];				// �迭 -> ���� �з� 
	TCHAR				m_DescPowerString[16];				// ���ݷ�
	TCHAR				m_DescSuccessRateString[16];		// ���ݼ���ġ  X
	TCHAR				m_DescWeightString[16];				// ���� 
	TCHAR				m_DescRequireString[16];			// �䱸ġ X
	TCHAR				m_DescDurabilityString[16];			// ������
	TCHAR				m_DescStrengthString[16];			// ���� 
	TCHAR				m_DescBalanceString[16];			// ����
	
	TCHAR				m_DescDefenseString[16];			// ����
	TCHAR				m_DescWarmthString[16];				// ����ȿ�� 
	TCHAR				m_DescResistanceString[16];			// �Ӽ� ���׷�
	
	TCHAR				m_DescRollString[16];				// ����
	TCHAR				m_DescEffectString[16];				// ȿ�� 
	TCHAR				m_DescFeatureString[16];			// Ư¡
	
	TCHAR				m_DescHeartString[16];				// �ɱ�
	TCHAR				m_DescStaminaString[16];			// ���� 
	TCHAR				m_DescLevelString[16];				// ��������
	
	TCHAR				m_DescRefine1String[16];			// ���� �Ӽ� 1
	TCHAR				m_DescRefine2String[16];			// ���� �Ӽ� 2
	
	TCHAR				m_DescAdditionalEffect1String[16];	// �ΰ� ȿ�� 1 
	TCHAR				m_DescAdditionalEffect2String[16];	// �ΰ� ȿ�� 2
	TCHAR				m_DescAdditionalEffect3String[16];	// �ΰ� ȿ�� 3
	
	TCHAR				m_DescSetEffect1String[16];			// ��Ʈ ȿ�� 1 
	TCHAR				m_DescSetEffect2String[16];			// ��Ʈ ȿ�� 2
	TCHAR				m_DescSetEffect3String[16];			// ��Ʈ ȿ�� 3 
	TCHAR				m_DescSetEffect4String[16];			// ��Ʈ ȿ�� 4
	TCHAR				m_DescSetEffect5String[16];			// ��Ʈ ȿ�� 5 
	
public:
	_XWindow_ItemTooltip();
	virtual ~_XWindow_ItemTooltip();
	
	BOOL	Initialize(void);
	void	DestroyWindow(void);
	
	void	Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL	Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void	SetWindowPosition(void);
	void	CheckOutside(void);		
	void	ShowWindow(BOOL show);	
	void	SetParentWindow(_XWindow* parentwindow)		{	m_ParentWindow = parentwindow;		}
	
	void	MoveWindow(int X, int Y);
	
	// 2004.06.11->oneway48 insert
	BOOL CheckMuscularStrength(_XGI_FirstCategory type, int uniqid);
	BOOL CheckWisdom(_XGI_FirstCategory type, int uniqid);
	BOOL CheckLevel(_XGI_FirstCategory type, int uniqid);
};

#endif // !defined(AFX_XWINDOW_ITEMTOOLTIP_H__09FCD649_CC62_4623_9FE5_F5248B3434DC__INCLUDED_)

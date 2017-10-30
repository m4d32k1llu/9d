// XWindow_MasterySkill.h: interface for the _XWindow_MasterySkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MASTERYSKILL_H__111CE960_4053_481A_8CE4_05FEA30880C5__INCLUDED_)
#define AFX_XWINDOW_MASTERYSKILL_H__111CE960_4053_481A_8CE4_05FEA30880C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

enum _XMasteryProperty
{
	_XMS_PROP_NONE = 0,
	_XMS_PROP_OVER,				// max point �޼�
	_XMS_PROP_UNDER,			// max point ��ġ�� ����
	_XMS_PROP_NOTLEARN,			// ����� ����
	_XMS_PROP_CANNOTLEARN		// ��� �� ����
};

struct _XMasterySkillSlot
{
	short				skillid;
	int					learnedskillid;
	char				needprev;
	POINT				position;
	_XMasteryProperty	prop;

	int					prev;
	int					next;
};


class _XWindow_MasterySkill  : public _XWindow
{
private :
	_XCheckButton*		m_MasteryClassTab[4];
	_XImageStatic*		m_MasteryPointImage;
	
	_XMasterySkillSlot	m_MasterySkillSlot[24];		// ��â�� ��Ÿ���� �ִ� ���� 24

	_XImageStatic*		m_SlotImage[4];
	_XImageStatic*		m_ArrowImage;
	_XImageStatic*		m_ValidLineImage;
	_XImageStatic*		m_InvalidLineImage;
	_XImageStatic*		m_SelectedImage;

	_XImageStatic		m_MasterySkillIcon;
	BOOL				m_bMasteryPointClickable;
	int					m_iSelectedSlot;
	
	_XImageStatic		m_MasterySkillGrayIcon;

public :
	int					m_SelectedTab;
	int					m_TotalPoint;

public:
	_XWindow_MasterySkill();
	virtual ~_XWindow_MasterySkill();

	BOOL				Initialize(void);
	void				DestroyWindow(void);

	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void				ChangeTab(int selectedtab);

	void				ParseMasterySkill(void);
	void				CheckDependency(void);
	void				DrawMasterySkill(void);

	void				DrawMasterySkillIcon(short skillid, int X, int Y, bool bColor = true, bool smallicon = false);
	void				DrawTooltip(void);

	void				GetMasterySkillName(void);
	TCHAR*				GetMasterySkillTypeName(int cClass, int index);
	bool				GetMasterySkillProp(int index);
	
	void				RefreshData(void);
	void				DecisionMasteryPoint(void);
	void				RemoveTooltip(void);
	
	int					GetMasterySkillNameId();	// ���� ���� �̸� id

};

#endif // !defined(AFX_XWINDOW_MASTERYSKILL_H__111CE960_4053_481A_8CE4_05FEA30880C5__INCLUDED_)

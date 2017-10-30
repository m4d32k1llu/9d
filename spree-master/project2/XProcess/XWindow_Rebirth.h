// XWindow_Rebirth.h: interface for the _XWindow_Rebirth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_REBIRTH_H__DAB96BC9_6B9A_4677_912B_3E4F7A485F7C__INCLUDED_)
#define AFX_XWINDOW_REBIRTH_H__DAB96BC9_6B9A_4677_912B_3E4F7A485F7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

enum _XREBIRTHTYPE
{
	_XREBIRTH_NONE,
	_XREBIRTH_BINSA,
	_XREBIRTH_HONSU
};

// ��Ȱâ �������ϸ鼭 button type �߰�
//
// �� ��ư�� 5�� �����Ѵ�. 
//
//- _XREBIRTH_HONSU �϶�
//		1. ����(����)��Ȱ
//		2. ����/��������/����� ��Ȱ
//		3. ���ڸ� ��Ȱ
//		4. ȭŸ�Ŵ� ���
//		5. ���� ���� (�̱���)
//
// �� ��ư�� ���� ���� �ǹ̴� ������ ����.
//		0)	- 0 : ���� ��Ȱ(����)
//			- 1 : ���� ��Ȱ(���İ���)
//		1)	- 0 : ������� (����)
//			- 1 : ������� (����)
//			- 2 : ���� ����
//			- 3 : ���� ����
//		2)	- 0 : pk�� ����
//			- 1 : ���Ϳ��� ����
//		3)	- 0 : ȭŸ�Ŵ� ����
//			- 1 : ȭŸ�Ŵ� ����
//		4)	- 0 : ���� ����
//			- 1 : ���� ����
//
//- _XREBIRTH_BINSA �϶�
//	 ���� ó���Ѵ�.
//

class _XWindow_Rebirth : public _XWindow  
{
public :
	_XButton*			m_TownButton;
	_XButton*			m_FieldButton;
	_XButton*			m_UseItemButton;	
//	_XButton*			m_InCBButton;
	
	_XButton*			m_SavePointButton;

	_XImageStatic		m_CornerImage[2];
	
	_XREBIRTHTYPE		m_RebirthType;
	
	BOOL				m_bCastleBattle;

	BOOL				m_bDeadInPK;		// Pk �߿� �׾���
	BOOL				m_bDeadInCB;		// ����� �߿� �׾���
	BOOL				m_bEnablePKTracing;	// PK�� ��ġ ���� �������� ���� 

	int					m_nButtonType[5];
	int					m_nTotalButtonCount;	// �������� ��ư �� (window size ����)
	
#ifdef _ACCLAIM_IGAADSYSTEM
	_XButton*			m_AcclaimAdVideoButton;
	BOOL				m_ShowIGAADButton;
	int					m_ButtonShowTimer;
	BOOL				m_VideoADCanceled;
#endif
	
public:
	_XWindow_Rebirth();
	virtual ~_XWindow_Rebirth();
	
	BOOL				Initialize(void);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void				ShowWindow(BOOL show);
	void				SetWindowSize(int width, int height);

	void				SetCastleBattleState(BOOL battle);

	void				SetButton(void);

	inline void			SetRebirthMode(_XREBIRTHTYPE type)	{	m_RebirthType = type;		};
	inline BOOL			GetCastleBattleState(void)			{	return m_bCastleBattle;		};
	
};

#endif // !defined(AFX_XWINDOW_REBIRTH_H__DAB96BC9_6B9A_4677_912B_3E4F7A485F7C__INCLUDED_)

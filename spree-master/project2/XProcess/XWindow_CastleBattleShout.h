// XWindow_CastleBattleShout.h: interface for the _XWindow_CastleBattleShout class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CASTLEBATTLESHOUT_H__BC925BF2_3AE3_4FEB_9755_C163227D3331__INCLUDED_)
#define AFX_XWINDOW_CASTLEBATTLESHOUT_H__BC925BF2_3AE3_4FEB_9755_C163227D3331__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_CastleBattleShout : public _XWindow  
{
public:

	int					m_nTotalCount; //�����ĸ� ����� �� �ִ� �ִ밳��
	int					m_nCurrentCount; // ����� ������ ����

	_XStatic			m_BorderTile;
	_XIMEContainer		m_IMECastleBattleShout;		// ����� ������ ���� �Է�â
	
	_XButton*			m_pBtnShout;
public:
	_XWindow_CastleBattleShout();
	virtual ~_XWindow_CastleBattleShout();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
};

#endif // !defined(AFX_XWINDOW_CASTLEBATTLESHOUT_H__BC925BF2_3AE3_4FEB_9755_C163227D3331__INCLUDED_)

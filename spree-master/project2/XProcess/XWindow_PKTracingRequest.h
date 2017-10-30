// XWindow_PKTracingRequest.h: interface for the _XWindow_PKTracingRequest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_PKTRACINGREQUEST_H__5DDAA1D9_7113_450F_BE32_B83660887401__INCLUDED_)
#define AFX_XWINDOW_PKTRACINGREQUEST_H__5DDAA1D9_7113_450F_BE32_B83660887401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "XWindow.h"

class _XWindow_PKTracingRequest  : public _XWindow  
{
public:	
	_XButton*			m_btnRequest1;
	_XButton*			m_btnRequest2;
	_XButton*			m_btnRequest3;
	_XButton*			m_btnCancle;

	_XImageStatic		m_CornerImage[2];
	TCHAR				m_cTargetName[13];	// ����� PK�� �̸� 
	TCHAR				m_cSpecialTargetName[13]; // Ư���� �̸� //Author : �����
	TCHAR				m_cDeleteTargetName[13];	// ���� ����Ʈ���� ������ PK�� �̸�
	
public:
	_XWindow_PKTracingRequest();
	virtual ~_XWindow_PKTracingRequest();
	
	BOOL				Initialize(void);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void				ShowWindow(BOOL show);
};

#endif // !defined(AFX_XWINDOW_PKTRACINGREQUEST_H__5DDAA1D9_7113_450F_BE32_B83660887401__INCLUDED_)

// XWindow_MnDCurrentPoint.h: interface for the _XWindow_MnDCurrentPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MNDCURRENTPOINT_H__EF29780E_04D8_40D3_8FE4_D1FBFB94D2CF__INCLUDED_)
#define AFX_XWINDOW_MNDCURRENTPOINT_H__EF29780E_04D8_40D3_8FE4_D1FBFB94D2CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

class _XTextureAnimation;

class _XWindow_MnDCurrentPoint : public _XWindow
{
public:
	_XWindow_MnDCurrentPoint();
	~_XWindow_MnDCurrentPoint();

	BOOL	Initialize();
	void	DestroyWindow( void );
	
	void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void	ShowWindow(BOOL show);
	void	MoveWindow( int X, int Y );

	void	SetNumberProcess();

public:
	_XTextureAnimation* m_pFullPointStartAni; //ó���� �ѹ��� ������ �ִ�
	_XTextureAnimation* m_pFullPointAni;	  //999 �� �� ��� ����
	_XTextureAnimation* m_pNumberAni[3][10];  //3�ڸ� 10�� ����

private :
	_XImageStatic*	m_pTitleImage;
	int	m_nDrawNumber[3]; //3�ڸ��� �׷��� �Ǵ� ����

};

#endif // !defined(AFX_XWINDOW_MNDCURRENTPOINT_H__EF29780E_04D8_40D3_8FE4_D1FBFB94D2CF__INCLUDED_)

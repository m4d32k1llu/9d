// XWindow_Escape.h: interface for the _XWindow_Escape class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_ESCAPE_H__75E7A43D_3A3F_4964_A159_A1FD72ED0394__INCLUDED_)
#define AFX_XWINDOW_ESCAPE_H__75E7A43D_3A3F_4964_A159_A1FD72ED0394__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XWindow_ChargeGauge.h"

typedef struct XEscapeInfo
{
	int		currentmapid;			// ���� �� �ε���
	int		village_stringid_01;	// ����(����)
	int		village_stringid_02;	// �鵵
	int		village_stringid_03;	// �浵
	int		areaid_common;			//����
	int		areaid_white;			//�鵵
	int		areaid_black;			//�浵
}_XEscapeInfo;


class _XWindow_Escape  : public _XWindow
{
public:
	_XWindow_ChargeGauge*		m_pChargeGaugeWindow;
	_XStatic					m_BorderTile;
	
	int							m_SelectedVillageStringID;	//���� ���õ� �������� �̵��� ���� �̸�
	int							m_SelectedAreaID;			//���� ���õ� �����Լ� �̵��� ���� ���̵�
	
	TCHAR						m_SelectedVillageName[512];	// ���õ� ���� �̸�

	list <_XEscapeInfo*>		m_listEscapeInfo;			// Ż�� ������ ������ ����Ʈ 
public:
	_XWindow_Escape();
	virtual ~_XWindow_Escape();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);

	void			SetSelectedEscapeInfo(void);//���� ���õ� �������� �̵��� ���� �̸��� ���� ���̵� ��� �´�.
	
	BOOL			LoadEscapeInfo(void);	// ��ũ��Ʈ���� ���� ������ ���� Ż�� ������ ���´�.
};

#endif // !defined(AFX_XWINDOW_ESCAPE_H__75E7A43D_3A3F_4964_A159_A1FD72ED0394__INCLUDED_)

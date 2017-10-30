// XWindow_CastleBattlePersonalHistory.h: interface for the _XWindow_CastleBattlePersonalHistory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CASTLEBATTLEPERSONALHISTORY_H__ED0FFC91_FD85_4541_B48B_1E8A0EB24E9E__INCLUDED_)
#define AFX_XWINDOW_CASTLEBATTLEPERSONALHISTORY_H__ED0FFC91_FD85_4541_B48B_1E8A0EB24E9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

enum EN_THREAT_GAUGEMODE
{
	_XDEF_CB_GAUGEMODE_NONE = 0,
	_XDEF_CB_GAUGEMODE_THREAT,
	_XDEF_CB_GAUGEMODE_DELAY
};

class _XWindow_CastleBattlePersonalHistory : public _XWindow  
{
public:
	_XImageStatic*  m_pImageGreenPoint;		// �����
	_XImageStatic*  m_pImageRedPoint;		// ������
	_XImageStatic*  m_pImageGrayPoint;		// ȸ����
	
	_XStickGauge*	m_GaugeThreate;			// ���� ������
	_XStickGauge*	m_GaugeSettle;			// ���� ������

	int				m_ThreatePoint[10];		// ���� ����Ʈ 0;ȸ�� 1:��� 2:������

	//����: ��������
	float			m_GaugeFactor;
	
	int				m_nOwnerType;
	int				m_nOwnerIndex;
	TCHAR			m_szOwnerName[_XDEF_ORGANIZATIONNAMELENGTH+1];
	
	TCHAR			m_szThreatenString[256];

	int				m_nPoint;

	EN_THREAT_GAUGEMODE	m_GaugeMode;

	FLOAT			m_fThreatenGaugeFactor;
	FLOAT			m_fDelayGaugeFactor;
	

public:
	_XWindow_CastleBattlePersonalHistory();
	virtual ~_XWindow_CastleBattlePersonalHistory();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);

	void			SetOwner(int type, int index);
	void			SetThreatenString(int type, int index, BOOL bMyOrg = FALSE);
	void			SetPoint(int point);

	void			SetGaugeMode(EN_THREAT_GAUGEMODE mode);

	void			SetThreatenParameter(FLOAT parameter);
	void			SetDelayParameter(FLOAT parameter);
};

#endif // !defined(AFX_XWINDOW_CASTLEBATTLEPERSONALHISTORY_H__ED0FFC91_FD85_4541_B48B_1E8A0EB24E9E__INCLUDED_)

// XWindow_ChargeGauge.h: interface for the _XWindow_ChargeGauge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CHARGEGAUGE_H__FC078968_2F82_4DE0_882C_6964E26566F5__INCLUDED_)
#define AFX_XWINDOW_CHARGEGAUGE_H__FC078968_2F82_4DE0_882C_6964E26566F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief ��� ������ â
 * ���ӿ� ���̴� ��� �������ٸ� �����ش�.
 * 
 * �� ��� ���� �ش� �������� �����ָ� �����Ѵ�.
 * �������ٴ� �������̴�. �Ѱ��� Ÿ���� �������� ���ӻ� �����ش�.
 * 
*/

typedef enum 
{ 
	_XGAUGE_NONE = 0, 
	_XGAUGE_CHARGE,			///< ������ų ���� 
	_XGAUGE_READ,			///< ����� ������ 
	_XGAUGE_REPAIR,			///< ������ 
	_XGAUGE_SOCKET,			///< ����������
	_XGAUGE_SOCKETDEATCH,	///< ���� ���Ž�
	_XGAUGE_BINSA,			///< �����½�
	_XGAUGE_ELIXIR,			///< ���ູ���
	_XGAUGE_GAMBLE,			///< �׺��� �� ��
	_XGAUGE_INCHANT,		///< ������ �� ��
	_XGAUGE_SMELTITEM,		///< ���/ �븰 
//	_XGAUGE_THREATEN,		///< ����� ���ڽ�
	_XGAUGE_ESCAPE			///< Ż���
} _XGAUGE_MODE;


class _XWindow_ChargeGauge : public _XWindow 
{
private:
	_XGAUGE_MODE		m_CurrentGaugeMode;
	int					m_iGaugePointAlpha;
	BOOL				m_bGaugePointAlphaFlag;
	BOOL				m_bGaugeFadeOutMode;
	FLOAT				m_dwGaugeDelayTime;	
	FLOAT				m_dwElapsedTime;
	int					m_iSelectedNumber;
	POINT				m_DrawPos;
	DWORD				m_FadeOutElapsedTime;
	DWORD				m_FadeOutAlpha;
	BOOL				m_bReadySandMsg;	///< �� �÷��װ� true �϶��� ������ ��Ŷ�� ���� �� ����
	/// �� �÷��׷� ���������� ������ ���߸� �� �÷��װ� false �϶��� ������ ��Ŷ�� ���� �� ����
	BOOL				m_bGaugeStop;		

public :
	_XStickGauge*		m_DefaultGaugeBorder;
	_XStickGauge*		m_ReadBookGaugeBorder;
	_XStickGauge*		m_ChargeGaugeBorder;	
	_XImageStatic*		m_GaugePoint;
	
	_XStickGauge*		m_ThreatenGaugeBorder;
	
public:
	_XWindow_ChargeGauge();
	virtual ~_XWindow_ChargeGauge();

	BOOL				Initailize(void);
	void				DestroyWindow(void);

	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void				ShowWindow(BOOL show);
	void				MoveWindow(int X, int Y);

	void				SetSelectedNumber(int number) { m_iSelectedNumber = number;	};
	int					GetSelectedNumber() { return m_iSelectedNumber; };
	/// ��°����� ����( ��°������� 100%�� �Ǹ� ������ �ش� ��Ŷ�� ����)
	void				SetWindowPosition(int& X, int& Y);
	void				SetParameter(FLOAT percentage);
	void				SetGaugeMode( _XGAUGE_MODE mode );
	_XGAUGE_MODE		GetCurrentGaugeMode() { return m_CurrentGaugeMode;	};
	void				SetDelayTime( FLOAT time );
	void				SetDrawPosition(POINT pos) { m_DrawPos = pos; };
	void				StartFadeOutMode();
	void				SetGaugeStop();
	void				StatusBinsa();	///< ��� ���½� ���� ������ ��� �ݱ�
	
	DWORD				GetGaugeDelayTime() { return m_dwGaugeDelayTime; };
	DWORD				GetElapsedTime() { return m_dwElapsedTime; };
};

#endif // !defined(AFX_XWINDOW_CHARGEGAUGE_H__FC078968_2F82_4DE0_882C_6964E26566F5__INCLUDED_)

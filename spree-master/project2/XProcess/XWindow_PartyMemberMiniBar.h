// XWindow_PartyMemberMiniBar.h: interface for the _XWindow_PartyMemberMiniBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_PARTYMEMBERMINIBAR_H__ECE15E5D_595E_4D5E_8517_453FCAAD4413__INCLUDED_)
#define AFX_XWINDOW_PARTYMEMBERMINIBAR_H__ECE15E5D_595E_4D5E_8517_453FCAAD4413__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_PartyMemberMiniBar : public _XWindow  
{
public:	
	
	_XImageStatic*		m_imagePartyMemberBorder;				// ���� ���â ���� �̹��� 
	_XImageStatic*		m_pPartyMemberDanger[3];				// ���� ǥ�� �̹��� 
	
	_XStickGauge*		m_pPartyMemberStaminaGauge;
	_XStickGauge*		m_pPartyMemberLifePowerGauge;			// ���� ���â ü�� ������ 
	_XStickGauge*		m_pPartyMemberForcePowerGauge;			// ���� ���â ���� ������ 

	
	_XButton*			m_btnPartyMemberClose;					// ���� ���â �ݱ� ��ư 
	_XButton*			m_btnPartyMasterEntrust;				// ���� ���� ��ư 
	_XButton*			m_btnClose;

	
	DWORD				m_CloseButtoncommandid;	
	DWORD				m_EntrustButtoncommandid;
	DWORD				m_PrevDrawTime;	
	
	FLOAT				m_EffectFrame;
	
	TCHAR				m_PartyMemberName[_XDEF_MAX_USERNAMESTRINGLENGTH];
	
	int					m_PartyIndex;
	int					m_ZoneIndex;

	
	unsigned short		m_PlayerUniqueid;	
	
	BOOL				m_bIsCurrentUser;
	BOOL				m_bDeathUser;
	BOOL				m_bIsCurrentZone;
	BOOL				m_EffectOrderSub;
	BOOL				m_bDangerous;							// ������ �����϶� 
	
	_XImageStatic*		m_pFarActionIcon;						// ����� �ൿ ǥ�� - �Ÿ���Ż
	_XImageStatic*		m_pOutActionIcon;						// ����� �ൿ ǥ�� - �ڸ����
		
	bool				m_bOut;
	bool				m_bFarAway;

	_XImageStatic*		m_pClassBorderIcon;						// ��å ������ ����
	_XImageStatic*		m_pClassIcon;							// ��å ������	
	
#ifdef _XTS_TARGET_STATE
	_XImageStatic		m_StateImage[32];
	int					m_StateCount;
#endif
public:
	_XWindow_PartyMemberMiniBar();
	virtual ~_XWindow_PartyMemberMiniBar();
	
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void MoveWindow( int X, int Y );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void DrawDangerMode(void);	
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void ProcessDblClick();

	void SetPartyPlayerName(int partyindex, BOOL& isCurrentUser);
	void SetPartyPlayerVital(int partyindex, BOOL& isCurrentUser);
	
	void SetZoneIndex(int zoneindex);
	void SetPartyUserStateImage(void);
	
	void SetOut(bool state);
	void SetFarAway(bool state);

	void Reset(void);

	void SetClassIcon( void );
};

#endif // !defined(AFX_XWINDOW_PARTYMEMBERMINIBAR_H__ECE15E5D_595E_4D5E_8517_453FCAAD4413__INCLUDED_)

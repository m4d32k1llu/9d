// XWindow_PartyMasterMiniBar.h: interface for the _XWindow_PartyMasterMiniBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_PARTYMASTERMINIBAR_H__89928846_58AC_4E9C_ADBC_4CF71B096E46__INCLUDED_)
#define AFX_XWINDOW_PARTYMASTERMINIBAR_H__89928846_58AC_4E9C_ADBC_4CF71B096E46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum _XPartyMasterMode
{
	_XPARTYMASTERMODE_MAXIMUM		= 0,
		_XPARTYMASTERMODE_MINIMUM
		
}_XPARTYMASTER_MODE;

class _XWindow_PartyMasterMiniBar : public _XWindow  
{
public:	
	
	_XImageStatic*		m_imagePartyMasterBorder;				// ���� ���â ���� �̹��� 
	_XImageStatic*		m_imagePartyMasterMinimizeBorder;		// ���� ���â ��ҽ� ���� �̹��� 
	_XImageStatic*		m_pPartyMasterDanger[3];				// ���� ǥ�� �̹��� 
	
	_XStickGauge*		m_pPartyMasterLifePowerGauge;			// ���� ���â ü�� ������ 
	_XStickGauge*		m_pPartyMasterForcePowerGauge;			// ���� ���â ���� ������ 
	
	_XButton*			m_btnPartyMasterMinimize;				// ���� ���â ��� ��ư 
	_XButton*			m_btnPartyMasterMaximize;				// ���� ���â Ȯ�� ��ư 
	_XButton*			m_btnPartyMasterClose;					// ���� ���â �ݱ� ��ư 
	
	_XButton*			m_btnPartyExperienceSharing;			// ���� ���â ����ġ �й� ��ư 
	_XButton*			m_btnPartyItemSharing;					// ���� ���â ������ �й� ��ư 
	
	_XListBox*			m_listboxPartyExperienceSharing;
	_XListBox*			m_listboxPartyItemSharing;

	TCHAR				m_PartyMasterName[_XDEF_MAX_USERNAMESTRINGLENGTH];
	
	DWORD				m_CloseButtoncommandid;	
	DWORD				m_PrevDrawTime;
	
	FLOAT				m_EffectFrame;
	
	int					m_indexPartyExperienceSharing;	
	int					m_indexPartyItemSharing;
	int					m_ZoneIndex;	
	int					m_PartyIndex;

	unsigned short		m_PlayerUniqueid;
	
	BOOL				m_bDangerous;							// ������ �����϶� 
	BOOL				m_bIsCurrentZone;
	BOOL				m_EffectOrderSub;
	BOOL				m_bDeathUser;
	BOOL				m_bIsCurrentUser;

	_XPARTYMASTER_MODE	m_PartyMasterMode;
	
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
	_XWindow_PartyMasterMiniBar();
	virtual ~_XWindow_PartyMasterMiniBar();
	
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void MoveWindow( int X, int Y );
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void DrawDangerMode(void);	
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void ProcessDblClick();
	
	void SetPartyMasterName(int partyindex, BOOL& isCurrentUser);
	void SetPartyMasterVital(int partyindex, BOOL& isCurrentUser);
	void SetMode( _XPARTYMASTER_MODE partymastermode );
	
	void SetZoneIndex(int zoneindex);
	
	BOOL CheckMousePositionExperienceSharingBox(void);
	BOOL CheckMousePositionItemSharingBox(void);
	void SetPartyUserStateImage(void);
	
	void SetOut(bool state);
	void SetFarAway(bool state);
	void Reset(void);

	void SetClassIcon( void );
};

#endif // !defined(AFX_XWINDOW_PARTYMASTERMINIBAR_H__89928846_58AC_4E9C_ADBC_4CF71B096E46__INCLUDED_)

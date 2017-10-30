// XWindow_Hedek.h: interface for the _XWindow_Hedek class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_HEDEK_H__E5785D64_4AFA_4588_BC58_0C47F1C94BEC__INCLUDED_)
#define AFX_XWINDOW_HEDEK_H__E5785D64_4AFA_4588_BC58_0C47F1C94BEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief Hedek â
 * �� â�� ����ǥ�� Ŭ���� ��� ���� ��ܿ� �ߴ� â�̴�.
 * hedektext.txt �� ����� ������ �ҷ��� �����ش�.
 * m_HedekTypeName �� hedektext.txt �� ������ ���ƾ� �Ѵ�.
*/

typedef enum tagHedekType	// cpp�� m_HedekTypeName �� ������ ���ƾ� �� : �߰��� ���� ��
{
	_XHEDEK_NONE = 0,
	_XHEDEK_CINFO,
	_XHEDEK_INVEN,
	_XHEDEK_SKILL,
	_XHEDEK_QUEST,
	_XHEDEK_NPCBUSS,
	_XHEDEK_PRESENT,
	_XHEDEK_WAREHOUSE,
	_XHEDEK_REPAIR,
	_XHEDEK_EDUCATION,
	_XHEDEK_OUTFIT,
	_XHEDEK_OPTION,
	_XHEDEK_GRAPHIC,
	_XHEDEK_SOUND,
	_XHEDEK_GAME,
	_XHEDEK_COMMUNITY,
	_XHEDEK_MESSENGER,
	_XHEDEK_PAGE,
	_XHEDEK_PTOP,
	_XHEDEK_DECO,
	_XHEDEK_MESSENGERWIN,
	_XHEDEK_POCKET,
	_XHEDEK_MASTERY,
	_XHEDEK_GUILD,
	_XHEDEK_LIFE,
	_XHEDEK_SOLOSHOP,
	_XHEDEK_BONUSBOX,
	_XHEDEK_NICK,
	_XHEDEK_PLAYERINFO,
	_XHEDEK_PKTOTAL,
} _XHEDEKTYPE;

class _XWindow_Hedek : public _XWindow  
{
private:	
	_XListBox*			m_HedekListBox;
	_XHEDEKTYPE			m_HedekType;
	TCHAR				m_TitleText[64];

	BOOL				LoadHedekData();

public:
	_XWindow_Hedek();
	virtual ~_XWindow_Hedek();

	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );	

	void				ShowWindow(BOOL show);
	void				SetHedekType( _XHEDEKTYPE type );
};

#endif // !defined(AFX_XWINDOW_HEDEK_H__E5785D64_4AFA_4588_BC58_0C47F1C94BEC__INCLUDED_)

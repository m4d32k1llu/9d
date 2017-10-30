// XFunctionalObjectActionItem.h: interface for the _XFunctionalObjectActionItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFUNCTIONALOBJECTACTIONITEM_H__2AB0C924_028D_4935_97F1_70221BB0BDD1__INCLUDED_)
#define AFX_XFUNCTIONALOBJECTACTIONITEM_H__2AB0C924_028D_4935_97F1_70221BB0BDD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XQuestScriptBaseType.h"
#include "XQuestScriptDefine.h"
#include "XFunctionalObjectScriptDefine.h"

class _XFunctionalObjectActionItem  
{	
public :
	_XTRIGGERDATA_ACTION	m_TDA;						
	LPTSTR					m_lpszMessageString;
	TCHAR					m_ActionString[64];
	int						m_ActionID;

public:
	_XFunctionalObjectActionItem();
	virtual ~_XFunctionalObjectActionItem();
	
	// ���� ������
	const _XFunctionalObjectActionItem& operator =( const _XFunctionalObjectActionItem& actionitem )
	{
		if( &actionitem == this ) return *this;
		
		m_TDA = actionitem.m_TDA;
		this->SetActionString( actionitem.m_ActionString );
		this->SetMessageString( actionitem.m_lpszMessageString );
		m_ActionID = actionitem.m_ActionID;
		
		return *this;
	}
	
	void					ResetData(void);
	void					SetMessageString(LPCTSTR messagestring);
	void					SetActionString(LPCTSTR actionstring);
	
	BOOL					Load(FILE* fileptr);
	BOOL					LoadString(FILE* fileptr, LPTSTR& string);
	
	_XFUNCTIONALOBJECT_ACTION_RESULT		Process(void);
	
	_XFUNCTIONALOBJECT_ACTION_RESULT		ProcessPlayerMove(void);			// �÷��̾�_�̵�
	_XFUNCTIONALOBJECT_ACTION_RESULT		ProcessEnterBossRoom(void);			// �÷��̾�_�������̵�
	_XFUNCTIONALOBJECT_ACTION_RESULT		ProcessEnableAttack(void);			// �÷��̾�_������Ʈ���ݰ��ɻ��¼���	
	_XFUNCTIONALOBJECT_ACTION_RESULT		ProcessDisplayText(void);			// TEXT ���	
	_XFUNCTIONALOBJECT_ACTION_RESULT		ProcessMatchInfo(void);				// �� ������ ���
	_XFUNCTIONALOBJECT_ACTION_RESULT		ProcessMatchZoneEnterExit(void);	// ���� ����/����
	_XFUNCTIONALOBJECT_ACTION_RESULT		ProcessCondtionalAttactSetting(void);	// �÷��̾�_���Ǻ����ݼ���
};

#endif // !defined(AFX_XFUNCTIONALOBJECTACTIONITEM_H__2AB0C924_028D_4935_97F1_70221BB0BDD1__INCLUDED_)

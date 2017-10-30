// XFunctionalObjectConditionItem.h: interface for the _XFunctionalObjectConditionItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFUNCTIONALOBJECTCONDITIONITEM_H__24ABF86B_FCE6_408A_B033_A83AF14EBEBE__INCLUDED_)
#define AFX_XFUNCTIONALOBJECTCONDITIONITEM_H__24ABF86B_FCE6_408A_B033_A83AF14EBEBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XQuestScriptBaseType.h"
#include "XQuestScriptDefine.h"
#include "XFunctionalObjectScriptDefine.h"

class _XFunctionalObjectConditionItem  
{
public:	
	
	_XTRIGGERDATA_CONDITION m_TDC;	
	TCHAR					m_ConditionString[64];	
	int 					m_ConditionID;
	
public:
	_XFunctionalObjectConditionItem();
	virtual ~_XFunctionalObjectConditionItem();
	
	// ���� ������
	const _XFunctionalObjectConditionItem& operator =(const _XFunctionalObjectConditionItem& conditionitem)
	{
		if(&conditionitem == this) return *this;
		
		m_TDC = conditionitem.m_TDC;
		this->SetConditionString(conditionitem.m_ConditionString);
		
		m_ConditionID = conditionitem.m_ConditionID;
		
		return *this;
	}
	
	void					ResetData(void);
	void					SetConditionString(LPCTSTR conditionstring);
	
	BOOL					Load(FILE* fileptr);
	BOOL					LoadString(FILE* fileptr, LPTSTR& string);
		
	_XFUNCTIONALOBJECT_CONDITION_RESULT		Process(void);
	
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckClan(void);					// �÷��̾�_���ĺ�
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckItem(void);					// �÷��̾�_�����۰˻�
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckGender(void);				// �÷��̾�_�����˻�	
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckQuestComplete(void);		// ����Ʈ �Ϸ� �˻�
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckState(void);				// �÷��̾�_����ȿ���˻�
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckCastleBattleStage(void);	// �����_�ܰ�˻�	
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckCastleBattleThreat(void);	// ����Ʈ �����_���ڰ��ɻ��°˻� 
	_XFUNCTIONALOBJECT_CONDITION_RESULT		ProcessCheckPlayerRole(void);			// �÷��̾�_���Ұ˻�
};

#endif // !defined(AFX_XFUNCTIONALOBJECTCONDITIONITEM_H__24ABF86B_FCE6_408A_B033_A83AF14EBEBE__INCLUDED_)

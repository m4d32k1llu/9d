// XFunctionalObjectConditionItem.cpp: implementation of the _XFunctionalObjectConditionItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XFunctionalObjectConditionItem.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XFunctionalObjectConditionItem::_XFunctionalObjectConditionItem()
{
	ResetData();
}

_XFunctionalObjectConditionItem::~_XFunctionalObjectConditionItem()
{

}

void _XFunctionalObjectConditionItem::ResetData(void)
{
	m_TDC.m_enable			= ID_TRIGGER_ENABLE;
	m_TDC.m_conditionclass	= ID_TRIGGER_SETDATA_NONE;
	
	m_TDC.m_compareclass1	= ID_TRIGGER_SETDATA_NONE;
	m_TDC.m_compareclass2	= ID_TRIGGER_SETDATA_NONE;
	m_TDC.m_compareclass3	= ID_TRIGGER_SETDATA_NONE;
	m_TDC.m_compareclass4	= ID_TRIGGER_SETDATA_NONE;
	
	m_TDC.m_numericvalue1	= ID_TRIGGER_SETDATA_NONE;
	m_TDC.m_numericvalue2	= ID_TRIGGER_SETDATA_NONE;
	m_TDC.m_numericvalue3	= ID_TRIGGER_SETDATA_NONE;
	m_TDC.m_numericvalue4	= ID_TRIGGER_SETDATA_NONE;
	
	memset(m_ConditionString, 0 , sizeof(TCHAR)*64);
	
	m_ConditionID = 0;
}

void _XFunctionalObjectConditionItem::SetConditionString(LPCTSTR conditionstring)
{
	memset(m_ConditionString, 0 , sizeof(TCHAR)*64);
	
	if(!conditionstring)
		return;
	
	int strlength = strlen(conditionstring);
	if(strlength < 64)
	{
		strcpy(m_ConditionString, conditionstring);
	}
	else
	{
		strncpy(m_ConditionString, conditionstring, 63);
	}
}

BOOL _XFunctionalObjectConditionItem::Load(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;
	
	if(fread(&m_ConditionID, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	
	if(fread(m_ConditionString, sizeof(TCHAR)*64, 1, fileptr) < 1)
		return FALSE;
	
	if(fread(&m_TDC, sizeof(_XTRIGGERDATA_CONDITION), 1, fileptr) < 1)
		return FALSE;
	
	return TRUE;
}

_XFUNCTIONALOBJECT_CONDITION_RESULT _XFunctionalObjectConditionItem::Process(void)
{
	switch(m_TDC.m_conditionclass)
	{
	case ID_OBJECT_CONDITION_CHECKCLAN :				// �÷��̾�_���ĺ�
		return ProcessCheckClan();
		
	case ID_OBJECT_CONDITION_CHECKITEM :				// �÷��̾�_�����۰˻�
		return ProcessCheckItem();
		
	case ID_OBJECT_CONDITION_CHECKGENDER :				// �÷��̾�_�����˻�
		return ProcessCheckGender();
		
	case ID_OBJECT_CONDITION_CHECKQUESTCOMPLETE :		// ����Ʈ �Ϸ� �˻�
		return ProcessCheckQuestComplete();
		
	case ID_OBJECT_CONDITION_CHECKSTATE:				// �÷��̾�_����ȿ���˻�
		return ProcessCheckState();
		
	case ID_OBJECT_CONDITION_CHECKCASTLEBATTLESTAGE:	// �����_�ܰ�˻�
		return ProcessCheckCastleBattleStage();
		
	case ID_OBJECT_CONDITION_CHECKCASTLEBATTLETHREAT:	// �����_���ڰ��ɻ��°˻�
		return ProcessCheckCastleBattleThreat();
		
	case ID_OBJECT_CONDITION_CHECKPLAYERROLE:			// �÷��̾�_���Ұ˻�
		return ProcessCheckPlayerRole();
	
	default:
		break;
	}
		
	return FUNCTIONALOBJECT_CONDITION_RESULT_FAIL;
	
}
_XFUNCTIONALOBJECT_CONDITION_RESULT _XFunctionalObjectConditionItem::ProcessCheckClan(void)
{
	// m_compareclass1 : ����
	switch(m_TDC.m_compareclass1)
	{
	case ID_QUEST_JOINGROUP_NANGIN :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_GAEBANG :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_BEEGOONG :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_SORIM :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_NOCKRIM :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_MOODANG :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_MAKYO :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_SEGA :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SEGA)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_NOFIXED :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOFIXED)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	}

	return FUNCTIONALOBJECT_CONDITION_RESULT_FAIL;
	
}
_XFUNCTIONALOBJECT_CONDITION_RESULT _XFunctionalObjectConditionItem::ProcessCheckItem(void)
{
	// m_compareclass1 : Item �з�
	// m_compareclass2 : compare class (�񱳽�)
	// m_numericvalue1 : Item ID
	// m_numericvalue2 : Item Count
	
	int totalcount = 0;
	
	if( m_TDC.m_compareclass1 == _XGI_FC_MONEY )
	{		
		if(g_pLocalUser->m_Money >= 0)
		{
			switch(m_TDC.m_compareclass2)
			{
			case ID_TRIGGER_COMPARE_THELEAST :
				{
					if(g_pLocalUser->m_Money < m_TDC.m_numericvalue2)
						return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
				}
				break;
			case ID_TRIGGER_COMPARE_THEMOST :
				{
					if(g_pLocalUser->m_Money > m_TDC.m_numericvalue2)
						return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
				}
				break;
			case ID_TRIGGER_COMPARE_EXACTLY :
				{
					if(g_pLocalUser->m_Money == m_TDC.m_numericvalue2)
						return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
				}
				break;
			}
		}
	}
	else
	{
		for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
		{
			if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == m_TDC.m_compareclass1)
			{
				if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDC.m_numericvalue1)
					totalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();
			}
		}
	#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
		for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
		{
			if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == m_TDC.m_compareclass1)
			{
				if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDC.m_numericvalue1)
					totalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();
			}
		}
	#endif
		
		if(totalcount >= 0)
		{
			switch(m_TDC.m_compareclass2)
			{
			case ID_TRIGGER_COMPARE_THELEAST :
				{
					if(totalcount < m_TDC.m_numericvalue2)
						return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
				}
				break;
			case ID_TRIGGER_COMPARE_THEMOST :
				{
					if(totalcount > m_TDC.m_numericvalue2)
						return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
				}
				break;
			case ID_TRIGGER_COMPARE_EXACTLY :
				{
					if(totalcount == m_TDC.m_numericvalue2)
						return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
				}
				break;
			}
		}
	}					
	return FUNCTIONALOBJECT_CONDITION_RESULT_FAIL;
	
}
_XFUNCTIONALOBJECT_CONDITION_RESULT _XFunctionalObjectConditionItem::ProcessCheckGender(void)
{
	// m_compareclass1 : Gender
	switch(m_TDC.m_compareclass1)
	{
	case ID_QUEST_GENDER_MAN :
		if(g_pLocalUser->m_CharacterInfo.gender == _XGENDER_MAN)
			return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		else
			break;
		
	case ID_QUEST_GENDER_WOMAN :
		if(g_pLocalUser->m_CharacterInfo.gender == _XGENDER_WOMAN)
			return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		else
			break;
	}
		
	return FUNCTIONALOBJECT_CONDITION_RESULT_FAIL;	
}
_XFUNCTIONALOBJECT_CONDITION_RESULT _XFunctionalObjectConditionItem::ProcessCheckQuestComplete(void)
{
	// m_numericvalue1 : Quest ID
	
	if( m_TDC.m_numericvalue1 >= 10000 && m_TDC.m_numericvalue1 < 20000 )
	{
		if( m_TDC.m_numericvalue1 < g_QuestScriptManager.m_CurrentMainQuestID )
			return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
	}
	else
	{
		map <int, _XQUEST_COMPLETE_RESULT>::iterator iter;
		for(iter = g_QuestScriptManager.m_mapCompleteQuestList.begin() ; iter != g_QuestScriptManager.m_mapCompleteQuestList.end() ; ++iter)
		{				
			int questid = iter->first;
			if( m_TDC.m_numericvalue1 == questid )
			{
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
			}
		}
	}
	
	return FUNCTIONALOBJECT_CONDITION_RESULT_FAIL;	
}


_XFUNCTIONALOBJECT_CONDITION_RESULT _XFunctionalObjectConditionItem::ProcessCheckState(void)
{
	//m_TDC.m_compareclass1	= m_index1; 
	// �ܻ�, ����, ������ ������ ����, ����ȿ�� - ��ų, ����ȿ�� - �����,
	// ����ȿ�� - �нú�, ���� �ߵ�, ������, ��ȣ ����, ���� ���� ������
	//m_TDC.m_numericvalue1	= m_value1; // ���̵�
	
	switch(m_TDC.m_compareclass1+1)
	{
	case _XST_EXTERNAL : // �ܻ�
		{
		}
		break;		
	case _XST_INTERNAL : // ����
		{
		}
		break;		
	case _XST_ITEM : // ������ ����������
		{
		}
		break;		
	case _XST_SKILL : // ����ȿ��:��ų
		{
			if( g_pState_Window->FindPlayerStateList(_XST_SKILL, m_TDC.m_numericvalue1))
			{
#ifdef _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI
				if(g_CurrentZoneInfoIndex == 21)	// �пյ��� ��� ���� ����޾ƾ� ��
				{	
					g_pLocalUser->m_bSendFunctionalMovePacket = TRUE;
				}
#endif
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
			}
			if( g_pState_Window->FindBuffStateList(_XST_SKILL, m_TDC.m_numericvalue1))
			{
#ifdef _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI
				if(g_CurrentZoneInfoIndex == 21)	// �пյ��� ��� ���� ����޾ƾ� ��
				{	
					g_pLocalUser->m_bSendFunctionalMovePacket = TRUE;
				}
#endif
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
			}
			if( g_pState_Window->FindHostileStateList(_XST_SKILL, m_TDC.m_numericvalue1))
			{
#ifdef _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI
				if(g_CurrentZoneInfoIndex == 21)	// �пյ��� ��� ���� ����޾ƾ� ��
				{	
					g_pLocalUser->m_bSendFunctionalMovePacket = TRUE;
				}
#endif
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
			}

		}
		break;		
	case _XST_EFFECT : // ����ȿ��:�����
		{
			if( g_pState_Window->FindPlayerStateList(_XST_EFFECT, m_TDC.m_numericvalue1))
			{
#ifdef _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI
				if(g_CurrentZoneInfoIndex == 21)	// �пյ��� ��� ���� ����޾ƾ� ��
				{	
					g_pLocalUser->m_bSendFunctionalMovePacket = TRUE;
				}
#endif
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
			}
			if( g_pState_Window->FindBuffStateList(_XST_EFFECT, m_TDC.m_numericvalue1))
			{
#ifdef _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI
				if(g_CurrentZoneInfoIndex == 21)	// �пյ��� ��� ���� ����޾ƾ� ��
				{	
					g_pLocalUser->m_bSendFunctionalMovePacket = TRUE;
				}
#endif
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
			}
			if( g_pState_Window->FindHostileStateList(_XST_EFFECT, m_TDC.m_numericvalue1))
			{
#ifdef _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI
				if(g_CurrentZoneInfoIndex == 21)	// �пյ��� ��� ���� ����޾ƾ� ��
				{	
					g_pLocalUser->m_bSendFunctionalMovePacket = TRUE;
				}
#endif
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
			}
		}
		break;		
	case _XST_PASSIVE : // ����ȿ��:�нú�
		{
			if( g_pState_Window->FindPlayerStateList(_XST_PASSIVE, m_TDC.m_numericvalue1))
			{
#ifdef _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI
				if(g_CurrentZoneInfoIndex == 21)	// �пյ��� ��� ���� ����޾ƾ� ��
				{	
					g_pLocalUser->m_bSendFunctionalMovePacket = TRUE;
				}
#endif
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
			}
			if( g_pState_Window->FindBuffStateList(_XST_PASSIVE, m_TDC.m_numericvalue1))
			{
#ifdef _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI
				if(g_CurrentZoneInfoIndex == 21)	// �пյ��� ��� ���� ����޾ƾ� ��
				{	
					g_pLocalUser->m_bSendFunctionalMovePacket = TRUE;
				}
#endif
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
			}
			if( g_pState_Window->FindHostileStateList(_XST_PASSIVE, m_TDC.m_numericvalue1))
			{
#ifdef _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI
				if(g_CurrentZoneInfoIndex == 21)	// �пյ��� ��� ���� ����޾ƾ� ��
				{	
					g_pLocalUser->m_bSendFunctionalMovePacket = TRUE;
				}
#endif
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
			}
		}
		break;		
	case _XST_INTOXICATION : // �����ߵ�
		{
		}
		break;		
	case _XST_STUDYBOOK : // ������
		{
		}
		break;		
	case _XST_NICKBUFF : // ��ȣ����
		{
		}
		break;		
	case _XST_PEMODE : // ���� ���� ������
		{
		}
		break;		
	}
	
	return FUNCTIONALOBJECT_CONDITION_RESULT_FAIL;	
}
_XFUNCTIONALOBJECT_CONDITION_RESULT _XFunctionalObjectConditionItem::ProcessCheckCastleBattleStage(void)
{
	// �����_�ܰ�˻�
	// m_numericvalue1 : �ܰ� 
	//m_TDC.m_numericvalue1
	
	switch(m_TDC.m_compareclass1)
	{
	case 0 :
		{
			// none
			if(g_CBManager.m_enWarState == _XEN_CB_NONE)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case 1 :
		{
			// before + after
			if(g_CBManager.m_enWarState >= _XEN_CB_READY && g_CBManager.m_enWarState < _XEN_CB_END)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case 2 :
		{
			// after
			if(g_CBManager.m_enWarState >= _XEN_CB_PROGRESS && g_CBManager.m_enWarState < _XEN_CB_END)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case 3 :
		{
			// end
			if(g_CBManager.m_enWarState >= _XEN_CB_END)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	}

	return FUNCTIONALOBJECT_CONDITION_RESULT_FAIL;
}

_XFUNCTIONALOBJECT_CONDITION_RESULT _XFunctionalObjectConditionItem::ProcessCheckCastleBattleThreat(void)
{
	// ����Ʈ �����_���ڰ��ɻ��°˻�
	
	// ���� ���� ����
	if(g_CBManager.GetThreatenState() == 1) 
	{
		return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;	
	}
	else
	{
		return FUNCTIONALOBJECT_CONDITION_RESULT_FAIL;
	}
}

_XFUNCTIONALOBJECT_CONDITION_RESULT _XFunctionalObjectConditionItem::ProcessCheckPlayerRole(void)
{
	// �÷��̾�_���Ұ˻�
	// m_compareclass1 : ���� : �Ѵ��� , ������, ������
	
	switch(m_TDC.m_compareclass1)
	{
	case 0: // �Ѵ���
		{
			if(g_pLocalUser->m_dwRoleInCastleBattle == 1)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;	
		}
		break;
	case 1: // ������
		{
			if(g_pLocalUser->m_dwAttackType == 1)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case 2: // ������
		{
			if(g_pLocalUser->m_dwAttackType == 2)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case 3 : // ���� + ��� ������
		{
			if(g_pLocalUser->m_dwAttackType == 2 || g_pLocalUser->m_nCastleMapId == g_CurrentZoneInfoIndex)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
		break;
	case 4 : // ��� ������
		{
			if(g_pLocalUser->m_nCastleMapId == g_CurrentZoneInfoIndex)
				return FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS;
		}
	}
	return FUNCTIONALOBJECT_CONDITION_RESULT_FAIL;
}

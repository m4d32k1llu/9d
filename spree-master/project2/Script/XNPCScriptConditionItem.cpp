// XNPCScriptConditionItem.cpp: implementation of the _XNPCScriptConditionItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XNPCScriptConditionItem.h"
#include "EODEXTDEF_GlobalObject.h"

#include "XList.h"
#include "XNPC.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XNPCScriptConditionItem::_XNPCScriptConditionItem()
{
	m_comparestring = NULL;
	ResetData();
}

_XNPCScriptConditionItem::~_XNPCScriptConditionItem()
{
	if(m_comparestring)
		delete[] m_comparestring;
}

void _XNPCScriptConditionItem::ResetData(void)
{
	m_NPCSubscriptCondition.m_enable			= ID_NPCSUBSCRIPT_ENABLE;
	m_NPCSubscriptCondition.m_conditionclass	= ID_NPCSUBSCRIPT_SETDATA_NONE;

	m_NPCSubscriptCondition.m_compareclass1	= ID_NPCSUBSCRIPT_SETDATA_NONE;
	m_NPCSubscriptCondition.m_compareclass2	= ID_NPCSUBSCRIPT_SETDATA_NONE;
	m_NPCSubscriptCondition.m_compareclass3	= ID_NPCSUBSCRIPT_SETDATA_NONE;
	m_NPCSubscriptCondition.m_compareclass4	= ID_NPCSUBSCRIPT_SETDATA_NONE;

	m_NPCSubscriptCondition.m_numericvalue1	= ID_NPCSUBSCRIPT_SETDATA_NONE;
	m_NPCSubscriptCondition.m_numericvalue2	= ID_NPCSUBSCRIPT_SETDATA_NONE;
	m_NPCSubscriptCondition.m_numericvalue3	= ID_NPCSUBSCRIPT_SETDATA_NONE;
	m_NPCSubscriptCondition.m_numericvalue4	= ID_NPCSUBSCRIPT_SETDATA_NONE;

	memset(m_conditionstring, 0 , sizeof(TCHAR)*64);

	if(m_comparestring)
	{
		delete[] m_comparestring;
		m_comparestring = NULL;
	}

	m_conditionid = 0;
}

void _XNPCScriptConditionItem::SetCompareString(LPCTSTR comparestring)
{
	if(!comparestring)
		return;

	if(m_comparestring)
	{
		delete[] m_comparestring;
		m_comparestring = NULL;
	}

	int strlength = strlen(comparestring);
	if(strlength > 0)
	{
		m_comparestring = new TCHAR[strlength + 1];
		m_comparestring[strlength] = 0;
		strcpy(m_comparestring, comparestring);
	}
}

void _XNPCScriptConditionItem::SetConditionString(LPCTSTR conditionstring)
{
	memset(m_conditionstring, 0 , sizeof(TCHAR)*64);

	if(!conditionstring)
		return;

	int strlength = strlen(conditionstring);
	if(strlength < 64)
	{
		strcpy(m_conditionstring, conditionstring);
	}
	else
	{
		strncpy(m_conditionstring, conditionstring, 63);
	}
}

BOOL _XNPCScriptConditionItem::Load(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;

	if(fread(&m_conditionid, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(fread(m_conditionstring, sizeof(TCHAR)*64, 1, fileptr) < 1)
		return FALSE;

	if(fread(&m_NPCSubscriptCondition, sizeof(_XNPCSUBSCRIPT_CONDITION), 1, fileptr) < 1)
		return FALSE;

	if(!LoadString(fileptr, m_comparestring))
		return FALSE;

	return TRUE;
}

BOOL _XNPCScriptConditionItem::LoadString(FILE* fileptr, LPTSTR& string)
{
	int strlength = 0;

	if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(string)
	{
		delete[] string;
		string = NULL;
	}

	if(strlength > 0)
	{
		string = new TCHAR[strlength + 1];
		if(!string)
		{
			return FALSE;
		}

		memset(string, 0, sizeof(TCHAR)*(strlength + 1));
		if(fread(string, strlength, 1, fileptr) < 1)
			return FALSE;
	}
	return TRUE;
}



_XNPCCONDITION_RESULT _XNPCScriptConditionItem::Process(void)
{
	switch(m_NPCSubscriptCondition.m_conditionclass)
	{
	case ID_NPCSUBSCRIPT_CONDITION_CHECKCLANNPC :
		return ProcessCheckClanNPC();
		
	case ID_NPCSUBSCRIPT_CONDITION_CHECKCLANFRIENDSHIP :
		return ProcessCheckClanFriendship();
		
	case ID_NPCSUBSCRIPT_CONDITION_CHECKCLANHOSTILITY :
		return ProcessCheckClanHostility();
		
	case ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR :
		return ProcessCheckHonor();
		
	case ID_NPCSUBSCRIPT_CONDITION_CHECKPLAYERROLE :
		return ProcessCheckPlayerRole();
		
	default :
		break;
	}
	
	return NPC_CONDITION_RESULT_FAIL;
}

// ���� üũ : NPC�� �÷��̾�� ���İ� �������� üũ
_XNPCCONDITION_RESULT _XNPCScriptConditionItem::ProcessCheckClanNPC(void)
{
	if( g_LastNPCScriptOwner->m_NPCClanClass == g_pLocalUser->m_CharacterInfo.groupindex )
	{
		g_LastNPCScriptOwner->m_bCheckClanNPC = TRUE;
		return NPC_CONDITION_RESULT_SUCCESS;
	}

	return NPC_CONDITION_RESULT_FAIL;
}


// ���� üũ : NPC�� �÷��̾�� ���İ� ��ȣ ���迡 �ִ����� üũ
_XNPCCONDITION_RESULT _XNPCScriptConditionItem::ProcessCheckClanFriendship(void)
{
	// m_compareclass1 : ����
	// ���ĸ� �����Ҷ� +1�� �ߴ�. ���� �����͸� ������ -1�� �ؾ��Ѵ�.
	switch(m_NPCSubscriptCondition.m_compareclass1-1)
	{
	case ID_NPC_CLAN_NANGIN :
		{
			//�����̸� ���� ��ȣ�����̴�.
			g_LastNPCScriptOwner->m_bCheckFriendship = TRUE;
			return NPC_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_NPC_CLAN_GAEBANG :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG 
				||g_pLocalUser->m_CharacterInfo.groupindex == ID_NPC_CLAN_NANGIN)
			{
				g_LastNPCScriptOwner->m_bCheckFriendship = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case ID_NPC_CLAN_BEEGOONG :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG
				||g_pLocalUser->m_CharacterInfo.groupindex == ID_NPC_CLAN_NANGIN)
			{
				g_LastNPCScriptOwner->m_bCheckFriendship = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case ID_NPC_CLAN_SORIM :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM
				||g_pLocalUser->m_CharacterInfo.groupindex == ID_NPC_CLAN_NANGIN)
			{
				g_LastNPCScriptOwner->m_bCheckFriendship = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case ID_NPC_CLAN_NOCKRIM :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM
				||g_pLocalUser->m_CharacterInfo.groupindex == ID_NPC_CLAN_NANGIN)
			{
				g_LastNPCScriptOwner->m_bCheckFriendship = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case ID_NPC_CLAN_MOODANG :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG
				||g_pLocalUser->m_CharacterInfo.groupindex == ID_NPC_CLAN_NANGIN)
			{
				g_LastNPCScriptOwner->m_bCheckFriendship = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case ID_NPC_CLAN_MAKYO :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO
				||g_pLocalUser->m_CharacterInfo.groupindex == ID_NPC_CLAN_NANGIN)
			{
				g_LastNPCScriptOwner->m_bCheckFriendship = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case 7 :	// �鵵
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == ID_NPC_CLAN_NANGIN
				||g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG
				||g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM
				||g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG)
			{
				g_LastNPCScriptOwner->m_bCheckFriendship = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case 8 :	// �浵
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == ID_NPC_CLAN_NANGIN
				||g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG
				||g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM
				||g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO)
			{
				g_LastNPCScriptOwner->m_bCheckFriendship = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	}

	g_LastNPCScriptOwner->m_bCheckFriendship = FALSE;
	return NPC_CONDITION_RESULT_FAIL;
}


// ���� üũ : NPC�� �÷��̾�� ���İ� ���� ���迡 �ִ����� üũ
_XNPCCONDITION_RESULT _XNPCScriptConditionItem::ProcessCheckClanHostility(void)
{
	// m_compareclass1 : ����
	// ���ĸ� �����Ҷ� +1�� �ߴ�. ���� �����͸� ������ -1�� �ؾ��Ѵ�.
	switch(m_NPCSubscriptCondition.m_compareclass1-1)
	{
	case ID_NPC_CLAN_NANGIN :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN)
			{
				g_LastNPCScriptOwner->m_bCheckHostility = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case ID_NPC_CLAN_GAEBANG :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG)
			{
				g_LastNPCScriptOwner->m_bCheckHostility = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case ID_NPC_CLAN_BEEGOONG :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG)
			{
				g_LastNPCScriptOwner->m_bCheckHostility = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case ID_NPC_CLAN_SORIM :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM)
			{
				g_LastNPCScriptOwner->m_bCheckHostility = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case ID_NPC_CLAN_NOCKRIM :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM)
			{
				g_LastNPCScriptOwner->m_bCheckHostility = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case ID_NPC_CLAN_MOODANG :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG)
			{
				g_LastNPCScriptOwner->m_bCheckHostility = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case ID_NPC_CLAN_MAKYO :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO)
			{
				g_LastNPCScriptOwner->m_bCheckHostility = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case 7 :	// �鵵
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == ID_NPC_CLAN_NANGIN
				||g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG
				||g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM
				||g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG)
			{
				g_LastNPCScriptOwner->m_bCheckFriendship = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case 8 :	// �浵
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == ID_NPC_CLAN_NANGIN
				||g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG
				||g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM
				||g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO)
			{
				g_LastNPCScriptOwner->m_bCheckFriendship = TRUE;
				return NPC_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
//	case ID_NPC_CLAN_SEGA :
//		{
//			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SEGA)
//			{
//				g_LastNPCScriptOwner->m_bCheckHostility = TRUE;
//				return NPC_CONDITION_RESULT_SUCCESS;
//			}
//		}
//		break;
//	case ID_NPC_CLAN_NOFIXED :
//		{
//			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOFIXED)
//			{
//				g_LastNPCScriptOwner->m_bCheckHostility = TRUE;
//				return NPC_CONDITION_RESULT_SUCCESS;
//			}
//		}
//		break;
	}

	g_LastNPCScriptOwner->m_bCheckHostility = TRUE;
	return NPC_CONDITION_RESULT_FAIL;
}


_XNPCCONDITION_RESULT _XNPCScriptConditionItem::ProcessCheckHonor(void)
{
	// m_compareclass1 : ��
	// �� ���� ���� ��ġ�� �������� �ʾƼ� �ϴ� ���Ƿ� ���������.
	switch(m_NPCSubscriptCondition.m_compareclass1)
	{
	case ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_BUMIN :
		{
			if( g_pLocalUser->m_CharacterInfo.Get_honor() > 50 )
				return NPC_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_KANGHO :
		{
			if( g_pLocalUser->m_CharacterInfo.Get_honor() > 100 )
				return NPC_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_MULIM :
		{
			if( g_pLocalUser->m_CharacterInfo.Get_honor() > 150)
				return NPC_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_CHUNHA :
		{
			if( g_pLocalUser->m_CharacterInfo.Get_honor() > 200)
				return NPC_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_JULSE :
		{
			if( g_pLocalUser->m_CharacterInfo.Get_honor() > 250)
				return NPC_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_NPCSUBSCRIPT_CONDITION_CHECKHONOR_ILDEA :
		{
			if( g_pLocalUser->m_CharacterInfo.Get_honor() > 300)
				return NPC_CONDITION_RESULT_SUCCESS;
		}
		break;
	}
	
	return NPC_CONDITION_RESULT_FAIL;
}

_XNPCCONDITION_RESULT _XNPCScriptConditionItem::ProcessCheckConditionOfNPCExistence(void)
{
	// m_compareclass1  0:���� 1:�Ҹ� 
	// m_compareclass2  0:����üũ 1:����Ʈüũ
	// m_compareclass3  ����üũ�ÿ� 0:�̸� 1:�ʰ� 2:����
	// m_numericvalue1  ��

	switch( m_NPCSubscriptCondition.m_compareclass2 ) 
	{
	case ID_NPCSUBSCRIPT_CONDITION_CHECKONOFF_LEVEL:
		{
			switch(m_NPCSubscriptCondition.m_compareclass3)
			{
			case ID_TRIGGER_COMPARE_THELEAST :
				if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() <  m_NPCSubscriptCondition.m_numericvalue1)
				{
					return NPC_CONDITION_RESULT_SUCCESS;
				}
				break;
				
			case ID_TRIGGER_COMPARE_THEMOST :
				if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() > m_NPCSubscriptCondition.m_numericvalue1)
				{
					return NPC_CONDITION_RESULT_SUCCESS;
				}
				break;
				
			case ID_TRIGGER_COMPARE_EXACTLY :
				if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() == m_NPCSubscriptCondition.m_numericvalue1)
				{
					return NPC_CONDITION_RESULT_SUCCESS;
				}
				break;				
			}
		}
		break;
	case ID_NPCSUBSCRIPT_CONDITION_CHECKONOFF_QUEST:
		{
			map <int, _XQUEST_COMPLETE_RESULT>::iterator iter;
			for(iter = g_QuestScriptManager.m_mapCompleteQuestList.begin() ; iter != g_QuestScriptManager.m_mapCompleteQuestList.end() ; ++iter)
			{				
				int questid = iter->first;
				if( questid == m_NPCSubscriptCondition.m_numericvalue1 )
				{
					return NPC_CONDITION_RESULT_SUCCESS;
				}
			}
		}
		break;
	}

	
	return NPC_CONDITION_RESULT_FAIL;
}


_XNPCCONDITION_RESULT _XNPCScriptConditionItem::ProcessCheckPlayerRole(void)
{
	// m_compareclass1 : ������, ������
	
	// ���� ����� ���������� ���� üũ�Ѵ�.
	if(g_CBManager.m_enWarState >= _XEN_CB_READY && g_CBManager.m_enWarState < _XEN_CB_END)
	{
		switch(m_NPCSubscriptCondition.m_compareclass1)
		{
		case 0:	//����
			{
				if(g_pLocalUser->m_dwAttackType == 1)
				{
					return NPC_CONDITION_RESULT_SUCCESS;
				}
				else
				{
					// �������� �ƴϴ�.
					g_CBManager.m_nSavePointFailResult = 2;
				}
			}
			break;
		case 1:	//���� + ��� ������
			{
				if(g_pLocalUser->m_dwAttackType == 2)
				{
					return NPC_CONDITION_RESULT_SUCCESS;
				}
				else
				{
					// �������� �ƴϴ�.
					g_CBManager.m_nSavePointFailResult = 3;
				}
			}
			break;
		}
	}
	else
	{
		// ����� �������� �ƴ�
		g_CBManager.m_nSavePointFailResult = 1;
	}

	return NPC_CONDITION_RESULT_FAIL;
}
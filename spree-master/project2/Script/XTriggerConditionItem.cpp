/** ���ǽ��� ǥ���ϴ� Ŭ����.
 *
 * \author Sohyun, Park
 * \date 2003.08.28
 */

#include <stdafx.h>

#include <stdio.h>
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XTriggerConditionItem.h"
#include "XUser.h"
#include "XQuestScriptDefine.h"
#include "EmperorOfDragons.h"
#include "XWindowObjectDefine.h"
#include "XWindow_NPCScript.h"
#include "XParticleIDDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XUnicodeConv.h"

#pragma warning (disable: 4786)
#include <string>
using namespace std;

_XTriggerConditionItem::_XTriggerConditionItem()
{
	m_bHoldCheckingOfNPCTargetting	= FALSE;
	m_bMainQuest					= FALSE;
	m_QuestType						= _XQUEST_TYPE_NONE;
	m_LastCheckTime					= 0;
	m_lpszCompareString				= NULL;
	ResetData();
}

_XTriggerConditionItem::~_XTriggerConditionItem()
{
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
#else
	SAFE_DELETE_ARRAY(m_lpszCompareString);
#endif
}

void _XTriggerConditionItem::ResetData(void)
{
	m_TDC.m_enable			= ID_TRIGGER_DISABLE;
	m_TDC.m_conditionclass	= ID_TRIGGER_SETDATA_NONE;

	m_TDC.m_compareclass1	= ID_TRIGGER_SETDATA_NONE;
	m_TDC.m_compareclass2	= ID_TRIGGER_SETDATA_NONE;
	m_TDC.m_compareclass3	= ID_TRIGGER_SETDATA_NONE;
	m_TDC.m_compareclass4	= ID_TRIGGER_SETDATA_NONE;

	m_TDC.m_numericvalue1	= ID_TRIGGER_SETDATA_NONE;
	m_TDC.m_numericvalue2	= ID_TRIGGER_SETDATA_NONE;
	m_TDC.m_numericvalue3	= ID_TRIGGER_SETDATA_NONE;
	m_TDC.m_numericvalue4	= ID_TRIGGER_SETDATA_NONE;

	memset(m_szConditionString, 0, sizeof(m_szConditionString));

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
	m_lpszCompareString = NULL;
#else
	SAFE_DELETE_ARRAY(m_lpszCompareString);
#endif

	m_nConditionID			= 0;
	m_nOwnerQuestID			= 0;
	m_nOwnerTriggerID		= 0;
	m_byOwnerTriggerClass	= 0;
	
	m_bSuccessCancle		= FALSE;
	m_bStartCondition		= FALSE;
	m_bHoldCheckingOfNPCTargetting	= FALSE;
}

void _XTriggerConditionItem::SetCompareString(LPCTSTR comparestring)
{
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
	m_lpszCompareString = (LPTSTR)comparestring;
#else
	if(!comparestring)
		return;

	if(m_lpszCompareString)
	{
		delete[] m_lpszCompareString;
		m_lpszCompareString = NULL;
	}

	int strlength = strlen(comparestring);
	if(strlength > 0)
	{
		m_lpszCompareString = new TCHAR[strlength + 1];
		m_lpszCompareString[strlength] = 0;
		strncpy(m_lpszCompareString, comparestring, strlength);
	}
#endif
}

void _XTriggerConditionItem::SetConditionString(LPCTSTR conditionstring)
{
	memset(m_szConditionString, 0, sizeof(m_szConditionString));

	if(!conditionstring)
		return;

	int strlength = strlen(conditionstring);
	if(strlength < 64)
	{
		strncpy(m_szConditionString, conditionstring, strlength);
	}
	else
	{
		strncpy(m_szConditionString, conditionstring, 63);
	}
}

BOOL _XTriggerConditionItem::Load(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;

	//int conditionid;

	if(fread(&m_nConditionID, sizeof(int), 1, fileptr) < 1)
	{
		return FALSE;
	}

	if(fread(m_szConditionString, sizeof(TCHAR) * 64, 1, fileptr) < 1)
	{
		return FALSE;
	}

	if(fread(&m_TDC, sizeof(_XTRIGGERDATA_CONDITION), 1, fileptr) < 1)
	{
		return FALSE;
	}

	int readstringlength = 0;
	if( (readstringlength = LoadString(fileptr, m_lpszCompareString)) == -1 )
	{
		return FALSE;
	}
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
	if( readstringlength > 0 )
	{
		m_lpszCompareString = g_StringDataBase.GetTextString( _XSTRINGDB_SN_QUESTSCRIPTTEXT, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex++] );	
		//_XLog( "QSI\tCondition compare string\t...\t%d\t%d\t\t%s", m_nConditionID, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex-1], m_lpszCompareString );
	}
#endif

	return TRUE;
}

int	_XTriggerConditionItem::LoadString(FILE* fileptr, LPTSTR& string)
{
	int strlength = 0;
	if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
	{
		return -1;
	}

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
	fseek( fileptr, sizeof(TCHAR)*strlength, SEEK_CUR );
	string = NULL;
#else
	if(string)
	{
		delete[] string;
		string = NULL;
	}

	if(strlength > 0)
	{
		string = new TCHAR[strlength + 1];
		if(!string)
			return -1;

		memset(string, 0, sizeof(TCHAR)*(strlength + 1));
		if(fread(string, sizeof(TCHAR)*(strlength), 1, fileptr) < 1)
		{
			return -1;
		}
	}
#endif
	return strlength;
}

void _XTriggerConditionItem::SetConditionID(int conditionid)
{
	m_nConditionID = conditionid;
}

int _XTriggerConditionItem::GetConditionID(void)
{
	return m_nConditionID;
}

void _XTriggerConditionItem::SetOwnerQuestID(int questid)
{
	m_nOwnerQuestID = questid;
}

int _XTriggerConditionItem::GetOwnerQuestID(void)
{
	return m_nOwnerQuestID;
}

void _XTriggerConditionItem::SetOwnerTriggerID(int triggerid)
{
	m_nOwnerTriggerID = triggerid;
}

int _XTriggerConditionItem::GetOwnerTriggerID(void)
{
	return m_nOwnerTriggerID;
}


void _XTriggerConditionItem::SetOwnerTriggerClass(BYTE triggerclass)
{
	m_byOwnerTriggerClass = triggerclass;
}


BYTE _XTriggerConditionItem::GetOwnerTriggerClass(void)
{
	return m_byOwnerTriggerClass;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::Process(void)
{
	switch(m_TDC.m_conditionclass)
	{
	case ID_CONDITION_CHECKVALUE :
		return ProcessCheckValue();

	case ID_CONDITION_HAVEITEM :
		return ProcessHaveItem();

	case ID_CONDITION_HAVESKILL :
		return ProcessHaveSkill();

	case ID_CONDITION_CHECKSTATUS :
		return ProcessCheckStatus();

	case ID_CONDITION_CHECKTARGETTING :
		return ProcessCheckTargetting();

	case ID_CONDITION_CHECKAREA :
		return ProcessCheckArea();

	case ID_CONDITION_CHECKAREAOTHER :
		return ProcessCheckAreaOther();
		
	case ID_CONDITION_CHECKDISTANCE :
		return ProcessCheckDistance();

	case ID_CONDITION_CHECKMESSAGE :
		return ProcessCheckMessage();

	case ID_CONDITION_CHECKTIMER :
		return ProcessCheckTimer();

	case ID_CONDITION_CHECKPROCEDINGQUEST :
		return ProcessCheckProcedingQuest();

	case ID_CONDITION_PROGRESSQUEST :
		return ProcessCheckProgressQuest();

	case ID_CONDITION_ISPROGRESSQUEST :
		return ProcessCheckIsProgressQuest();

	case ID_CONDITION_CHECKPARTYCOUNT :
		return ProcessCheckPartyCount();

	case ID_CONDITION_CHECKPARTYSTATUS :
		return ProcessCheckPartyStatus();

	case ID_CONDITION_CHECKATTACK :
		return ProcessCheckAttack();

	case ID_CONDITION_CHECKPASSTIME :
		return ProcessCheckPassTime();

	case ID_CONDITION_CHECKGENDER :
		return ProcessCheckGender();

	case ID_CONDITION_CHECKJOINGROUP :
		return ProcessCheckJoinGroup();
		
	case ID_CONDITION_CHECKACTIONID :
		return ProcessCheckActionID();

	case ID_CONDITION_CHECKPROCEDINGTRIGGER :
		return ProcessCheckProcedingTrigger();

	case ID_CONDITION_CHECKMARTIAL :
		return ProcessCheckMartial();
		
	case ID_CONDITION_CHECKSELECTION :
		return ProcessCheckSelection();
		
	case ID_CONDITION_CHECKNPCDIALOGMODE :
		return ProcessCheckNPCDialogMode();

	case ID_CONDITION_CHECKWEARITEM :				// ������ ���� üũ
		return ProcessCheckWearItem();

	case ID_CONDITION_SETQUESTMARKAREA :			// QUEST��ũ(!) ǥ���ϱ� ����
		return ProcessSetQuestMarkArea();
		
	case ID_CONDITION_SETQUESTMARKNPC :				// QUEST��ũ(!) ǥ���ϱ� NPC 
		return ProcessSetQuestMarkNPC();
		
	case ID_CONDITION_SETHINTMARKAREA :				// HINT��ũ(?) ǥ���ϱ� ����
		return ProcessSetHintMarkArea();
		
	case ID_CONDITION_SETHINTMARKNPC :				// HINT��ũ(?) ǥ���ϱ� NPC 
		return ProcessSetHintMarkNPC();

	case ID_CONDITION_CHECKSELECTIONEND :			// ������ ���� üũ  
		return ProcessCheckSelectionEnd();
		
	case ID_CONDITION_CHECKCOUNTPOINT :				// ī���� ����Ʈ üũ�ϱ� 
		return ProcessCheckCountPoint();
		
	case ID_CONDITION_CHECKMOTION :					// Ư�� �ൿ üũ  
		return ProcessCheckMotion();
		
	case ID_CONDITION_CHECKABNORMALSTATE :			// �÷��̾�_�����̻�üũ  
		return ProcessCheckAbnormalstate();
		
	case ID_CONDITION_CHECKBATTLERECORD :			// �÷��̾�_������üũ 
		return ProcessCheckBattleRecord();
		
	case ID_CONDITION_CHECKPARTYJOINGROUP :			// ��Ƽ��_����üũ  
		return ProcessCheckPartyJoinGroup();
		
	case ID_CONDITION_CHECKPARTYVALUE :				//��Ƽ��_���Ⱥ�(��,����ܰ� �˻�, ��,����ܰ� ���հ˻�...)
		return ProcessCheckPartyValue();
		
	case ID_CONDITION_CHECKSKILLSTATE:				//�÷��̾�_�������º�
		return ProcessCheckSkillState();

	case ID_CONDITION_CHECKCLANSERVICEPOINT:		//�÷��̾�_���ı⿩��üũ
		return ProcessCheckClanServicePoint();
		
	case ID_CONDITION_CHECKQUESTEMAIL:				//ǳ���üũ
		return ProcessCheckQuestMail();

	case ID_CONDITION_CHECKBOSSBATTLESTART:			//����� ���� üũ
		return ProcessCheckBossBattleStart();

	case ID_CONDITION_CHECKBOSSBATTELSTAGENUMBER:	//����� �ܰ� üũ
		return ProcessCheckBossBattleStageNumber();

	case ID_CONDITION_CHECKPARTYMASTER:				//��Ƽ��_����˻�
		return ProcessCheckPartyMaster();
		
	case ID_CONDITION_CHECKPARTYMEMBERCLASS:		//��Ƽ��_��å�˻�
		return ProcessCheckPartyMemberClass();
		
	case ID_CONDITION_CHECKAREAPARTYMEMBER:			//��Ƽ��_�����˻�
		return ProcessCheckAreaPartyMember();
		
	case ID_CONDITION_CHECKRECORDNUMBER:			//�÷��̾�_���ǥ�˻�
		return ProcessCheckRecordNumber();

	case ID_CONDITION_CHECKLEVEL:					//�÷��̾�_���뵵�˻�
		return ProcessCheckExperience();
		
	case ID_CONDITION_CHECKPLAYERCLASS:				//�÷��̾�_��å�˻�
		return ProcessCheckPlayerClass();

	case ID_CONDITION_CHECKINVENSPACE:				//�÷��̾�_�೶�˻�
		return ProcessCheckInvenspace();
		
	case ID_CONDITION_CHECKPARTYMASTERDEAD:			//��������˻�(�ɼ����ǽĿ����� ���)
		return ProcessCheckPartyMasterDead();
		
	case ID_CONDITION_CHECKDESTORYITEM:				//�������ı��˻�(�ɼ����ǽ�OR��������Ÿ��)
		return ProcessCheckDestroyItem();
		
	case ID_CONDITION_CHECKSTARTLIMITQUEST:			//�ο������������˻�
		return ProcessCheckStartLimitQuest();
		
	case ID_CONDITION_CHECKCOMPLETELIMITQUEST:		//�Ϸἱ���������˻�
		return ProcessCheckCompleteLimitQuest();

	case ID_CONDITION_CHECKPARTYGENDER:				// ��Ƽ��_����üũ
		return ProcessCheckPartyMemberGender();
		
	case ID_CONDITION_CHECKSERVICETYPE:				// �÷��̾�_������˻�
		return ProcessCheckServiceType();
		
	case ID_CONDITION_CHECKPARTYORGANIZATION:		// ��Ƽ��_�����ܰ˻�
		return ProcessCheckPartyOrganization();
	
	case ID_CONDITION_PARTYHAVEITEM:
		return ProcessPartyHaveItem();				// ��Ƽ��_�����۰˻�
		
	case ID_CONDITION_CHECKNICKNAME:
		return QUEST_CONDITION_RESULT_SUCCESS;				// �÷��̾�_��ȣ�˻�
	
	case ID_CONDITION_CHECKGROUPLEADER:
		return QUEST_CONDITION_RESULT_SUCCESS;				// �������� �� ���� �˻�
	
	case ID_CONDITION_CHECKPARTYCLAN:
		return QUEST_CONDITION_RESULT_SUCCESS;				// ��Ƽ��_���İ˻�
	
	case ID_CONDITION_CHECKPARTYPREQUEST:
		return QUEST_CONDITION_RESULT_SUCCESS;				// ��Ƽ��_��������Ʈ�˻�

	case ID_CONDITION_CHECKPARTYCLASSOR:
		return QUEST_CONDITION_RESULT_SUCCESS;				// ��Ƽ��_��ü��å�˻�

	case ID_CONDITION_CHECK_MND_MASTERPOINT :		// �����ý���_���±⿩���˻�
		return PorcessCheckMasterPoint();

	case ID_CONDITION_CHECK_MND_MASTERRANK	:		// �����ý���_���µ�ް˻�
		return PorcessCheckMasterRank();

	case ID_CONDITION_CHECK_MND_STATUS	:			// �����ý���_���°˻�
		return PorcessCheckMasterStatus();		

	default :
		break;
	}

	return QUEST_CONDITION_RESULT_FAIL;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckValue(void)
{
	// m_compareclass1 : m_TargetClass
	// m_compareclass2 : m_CompareClass
	// m_numericvalue1 : m_ShameNumber

	switch(m_TDC.m_compareclass1)
	{
	case ID_QUEST_CHECKVALUETYPE_TOTALLEVEL :
		return ProcessCheckTotalLevel();

	case ID_QUEST_CHECKVALUETYPE_VITALITY :
		return ProcessCheckVitality();

	case ID_QUEST_CHECKVALUETYPE_ZEN :
		return ProcessCheckZen();

	case ID_QUEST_CHECKVALUETYPE_SOUL :
		return ProcessCheckSoul();

	case ID_QUEST_CHECKVALUETYPE_HEART :
		return ProcessCheckHeart();

	case ID_QUEST_CHECKVALUETYPE_STAMINA :
		return ProcessCheckStamina();

	case ID_QUEST_CHECKVALUETYPE_VITALPOWER :
		return ProcessCheckVitalPower();

	case ID_QUEST_CHECKVALUETYPE_INNERPOWER :
		return ProcessCheckInnerPower();

	case ID_QUEST_CHECKVALUETYPE_SOULPOWER :
		return ProcessCheckSoulPower();

	case ID_QUEST_CHECKVALUETYPE_HEARTPOWER :
		return ProcessCheckHeartPower();

	case ID_QUEST_CHECKVALUETYPE_STAMINAPOWER :
		return ProcessCheckStaminaPower();

	case ID_QUEST_CHECKVALUETYPE_HONOR :
		return ProcessCheckHonor();

	case ID_QUEST_CHECKVALUETYPE_RETRIBUTION :
		return ProcessCheckRetribution();

	//case ID_QUEST_CHECKVALUETYPE_EXPERIENCE :
	//	return ProcessCheckExperience();

	default :
		return QUEST_CONDITION_RESULT_FAIL;
	}
}

// ��������
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckTotalLevel(void)
{
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() <  m_TDC.m_numericvalue1)
		{
			if( m_bStartCondition )
			{
				// ���������϶� ���� üũ�� ������ ���� ������ ������ �ִ´�. 
				// ������ ���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
				if( !g_QuestScriptManager.m_listCheckLevel.empty() )
				{
					bool balreadyexist = false;
					list <_XQUEST_LEVELINFO>::iterator iter_checklevel;
					for(iter_checklevel = g_QuestScriptManager.m_listCheckLevel.begin() ; iter_checklevel != g_QuestScriptManager.m_listCheckLevel.end() ;++iter_checklevel )
					{
						_XQUEST_LEVELINFO checklevel = *iter_checklevel;
						if( checklevel.questid == m_nOwnerQuestID )
						{
							if( checklevel.maximumlevel == m_TDC.m_numericvalue1 )
							{
								balreadyexist = true;
								break;
							}
							else
							{
								checklevel.maximumlevel = m_TDC.m_numericvalue1;
								balreadyexist = true;
								break;
							}
						}
					}
					if( !balreadyexist )
					{
						_XQUEST_LEVELINFO checklevel;
						memset(&checklevel, 0, sizeof(_XQUEST_LEVELINFO));
						checklevel.questid	= m_nOwnerQuestID;
						checklevel.maximumlevel = m_TDC.m_numericvalue1;
						g_QuestScriptManager.m_listCheckLevel.push_back(checklevel);
					}
				}
				else
				{
					_XQUEST_LEVELINFO checklevel;
					memset(&checklevel, 0, sizeof(_XQUEST_LEVELINFO));
					checklevel.questid	= m_nOwnerQuestID;
					checklevel.maximumlevel = m_TDC.m_numericvalue1;
					g_QuestScriptManager.m_listCheckLevel.push_back(checklevel);
				}
			}
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;

	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;

	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() == m_TDC.m_numericvalue1)
		{
			if( m_bStartCondition )
			{
				// ���������϶� ���� üũ�� ������ ���� ������ ������ �ִ´�. 
				// ������ ���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
				if( !g_QuestScriptManager.m_listCheckLevel.empty() )
				{
					bool balreadyexist = false;
					list <_XQUEST_LEVELINFO>::iterator iter_checklevel;
					for(iter_checklevel = g_QuestScriptManager.m_listCheckLevel.begin() ; iter_checklevel != g_QuestScriptManager.m_listCheckLevel.end() ;++iter_checklevel )
					{
						_XQUEST_LEVELINFO checklevel = *iter_checklevel;
						if( checklevel.questid == m_nOwnerQuestID )
						{
							if( checklevel.equallevel == m_TDC.m_numericvalue1 )
							{
								balreadyexist = true;
								break;
							}
							else
							{
								checklevel.equallevel = m_TDC.m_numericvalue1;
								balreadyexist = true;
								break;
							}
						}
					}
					if( !balreadyexist )
					{
						_XQUEST_LEVELINFO checklevel;
						memset(&checklevel, 0, sizeof(_XQUEST_LEVELINFO));
						checklevel.questid	= m_nOwnerQuestID;
						checklevel.equallevel = m_TDC.m_numericvalue1;
						g_QuestScriptManager.m_listCheckLevel.push_back(checklevel);
					}
				}
				else
				{
					_XQUEST_LEVELINFO checklevel;
					memset(&checklevel, 0, sizeof(_XQUEST_LEVELINFO));
					checklevel.questid	= m_nOwnerQuestID;
					checklevel.equallevel = m_TDC.m_numericvalue1;
					g_QuestScriptManager.m_listCheckLevel.push_back(checklevel);
				}
			}
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;

	default :
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ����
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckVitality(void)
{
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.Get_constitution() < m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;

	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.Get_constitution() > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;

	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.Get_constitution() == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;

	default :
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ����
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckZen(void)
{
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.Get_zen() < m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.Get_zen() > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.Get_zen() == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ����
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckSoul(void)
{
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.Get_intelligence() < m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.Get_intelligence() > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.Get_intelligence() == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// �ɱ�
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckHeart(void)
{
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.Get_dexterity() < m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.Get_dexterity() > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.Get_dexterity() == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ����
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStamina(void)
{
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.Get_strength() < m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.Get_strength() > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.Get_strength() == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ü��
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckVitalPower(void)
{
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.Get_current_lifepower() < m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.Get_current_lifepower() > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.Get_current_lifepower() == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ����
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckInnerPower(void)
{
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.Get_current_forcepower() < m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.Get_current_forcepower() > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.Get_current_forcepower() == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ����
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckSoulPower(void)
{
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.Get_current_concentrationpower() < m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.Get_current_concentrationpower() > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.Get_current_concentrationpower() == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// �ɷ�
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckHeartPower(void)
{
	/*
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.heartpower < m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.heartpower > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.heartpower == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	*/
	return QUEST_CONDITION_RESULT_FAIL;
}

// ����
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStaminaPower(void)
{
	/*
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.junggipower < m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.junggipower > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.junggipower == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	*/
	return QUEST_CONDITION_RESULT_FAIL;
}

// ��
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckHonor(void)
{
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.Get_honor() < m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.Get_honor() > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.Get_honor() == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ����
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckRetribution(void)
{
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.Get_retribution() < m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.Get_retribution() > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.Get_retribution() == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}
// ���� ����
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessHaveSkill(void)
{
	// m_compareclass2 : compare class (�񱳽�)
	// m_numericvalue1 : skill id
	// m_numericvalue2 : level (����)

	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
	{
		if(g_pLocalUser->m_SkillList[i].m_sSkillID == 0)
			continue;

		if(g_pLocalUser->m_SkillList[i].m_sSkillID == m_TDC.m_numericvalue1)
		{
			switch(m_TDC.m_compareclass2)
			{
			case ID_TRIGGER_COMPARE_THELEAST :
				{
					if(g_pLocalUser->m_SkillList[i].m_cSkillLevel < m_TDC.m_numericvalue2-1)
						return QUEST_CONDITION_RESULT_SUCCESS;
				}
				break;
			case ID_TRIGGER_COMPARE_THEMOST :
				{
					if(g_pLocalUser->m_SkillList[i].m_cSkillLevel > m_TDC.m_numericvalue2-1)
						return QUEST_CONDITION_RESULT_SUCCESS;
				}
				break;
			case ID_TRIGGER_COMPARE_EXACTLY :
				{
					if(g_pLocalUser->m_SkillList[i].m_cSkillLevel ==m_TDC.m_numericvalue2-1)
						return QUEST_CONDITION_RESULT_SUCCESS;
				}
				break;
			}
		}
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� �˻�
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStatus(void)
{
	// m_compareclass1 : status
	switch(m_TDC.m_compareclass1)
	{
	case ID_QUEST_STATUSMYSELF_BATTLE :
		return ProcessCheckStatusBattle();

	case ID_QUEST_STATUSMYSELF_RIDEHORSE :
		return ProcessCheckStatusRideHorse();

	case ID_QUEST_STATUSMYSELF_INPARTY :
		return ProcessCheckStatusInParty();

	case ID_QUEST_STATUSMYSELF_NOTPARTY :
		return ProcessCheckStatusNotParty();

	case ID_QUEST_STATUSMYSELF_JOINGROUP :
		return ProcessCheckStatusJoinGroup();

	case ID_QUEST_STATUSMYSELF_NOTJOINGROUP :
		return ProcessCheckStatusNotJoinGroup();

	case ID_QUEST_STATUSMYSELF_LEVELUP :
		return ProcessCheckStatusLevelUp();

	case ID_QUEST_STATUSMYSELF_TRADENPC :
		return ProcessCheckStatusTradeNPC();

	case ID_QUEST_STATUSMYSELF_TALKNPC :
		return ProcessCheckStatusTalkNPC();

	case ID_QUEST_STATUSMYSELF_DEATH :
		return ProcessCheckStatusDeath();

	case ID_QUEST_STATUSMYSELF_REVIVE :
		return ProcessCheckStatusRevive();
	}

	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� ���� üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStatusBattle(void)
{
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;

	if(g_pLocalUser->m_CharacterLog.isAttack)
	{
		if(g_pLocalUser->m_CharacterLog.attackTime >= currentquest->GetStartTime() && currentquest->GetStartTime() != 0)
		{
			if((g_ServerTimeCode - g_pLocalUser->m_CharacterLog.attackTime) < 10000)
			{
				g_pLocalUser->m_CharacterLog.isAttack = FALSE;
				g_pLocalUser->m_CharacterLog.attackTime = 0;
				g_pLocalUser->m_CharacterLog.attackTargetType = 0;
				
#ifdef _XDWDEBUG	
				_XDWINPRINT("[QUESTCONDITION/ProcessCheckStatusBattle] Attack log clear");
#endif

				return QUEST_CONDITION_RESULT_SUCCESS;
			}
		}
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// �¸� ���� üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStatusRideHorse(void)
{
	return QUEST_CONDITION_RESULT_FAIL;
}

// ��Ƽ ���� ����
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStatusInParty(void)
{
	if(g_pLocalUser->m_InParty)
	{
		if( m_bStartCondition )
		{
			// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
			// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
			if( !g_QuestScriptManager.m_listCheckInParty.empty() )
			{
				bool balreadyexist = false;
				list <int>::iterator iter_checkinparty;
				for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
				{
					int partyquestid = *iter_checkinparty;
					if( partyquestid == m_nOwnerQuestID )
					{
						balreadyexist = true;
						break;
					}
				}
				if( !balreadyexist )
				{
					g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
				}
			}
			else
			{
				g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
			}
		}
		return QUEST_CONDITION_RESULT_SUCCESS;
	}
	else
		return QUEST_CONDITION_RESULT_FAIL;
}

// ��Ƽ ���� ����
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStatusNotParty(void)
{
	if(!g_pLocalUser->m_InParty)
		return QUEST_CONDITION_RESULT_SUCCESS;
	else
		return QUEST_CONDITION_RESULT_FAIL;
}

// ���� ����
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStatusJoinGroup(void)
{
	if(g_pLocalUser->m_CharacterInfo.groupindex != _XGROUP_NANGIN)
		return QUEST_CONDITION_RESULT_SUCCESS;
	else
		return QUEST_CONDITION_RESULT_FAIL;
}

// ���� �̰���
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStatusNotJoinGroup(void)
{
	if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN)
		return QUEST_CONDITION_RESULT_SUCCESS;
	else
		return QUEST_CONDITION_RESULT_FAIL;
}

// ���� �� ó��
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStatusLevelUp(void)
{
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
		
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;

	if(g_pLocalUser->m_CharacterLog.isLevelUp)
	{
		if(g_pLocalUser->m_CharacterLog.levelupTime >= currentquest->GetStartTime() && currentquest->GetStartTime() != 0)
		{
			g_pLocalUser->m_CharacterLog.isLevelUp = FALSE;
			g_pLocalUser->m_CharacterLog.levelupTime= 0;
			
#ifdef _XDWDEBUG	
			_XDWINPRINT("[QUESTCONDITION/ProcessCheckStatusLevelUp] Levelup log clear");
#endif

			return QUEST_CONDITION_RESULT_SUCCESS;
		}
	}

	return QUEST_CONDITION_RESULT_FAIL;
}

// NPC���� �ŷ� ó��
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStatusTradeNPC(void)
{
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;

	if(g_pLocalUser->m_CharacterLog.isTrade)
	{
		if(g_pLocalUser->m_CharacterLog.tradeTime >= currentquest->GetStartTime() && currentquest->GetStartTime() != 0)
		{
			g_pLocalUser->m_CharacterLog.isTrade = FALSE;
			g_pLocalUser->m_CharacterLog.tradeTime = 0;
			
#ifdef _XDWDEBUG	
			_XDWINPRINT("[QUESTCONDITION/ProcessCheckStatusTradeNPC] TradeNPC log clear");
#endif

			return QUEST_CONDITION_RESULT_SUCCESS;
		}
	}

	return QUEST_CONDITION_RESULT_FAIL;
}

// NPC���� ��ȭ ó��
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStatusTalkNPC(void)
{
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;

	if(g_pLocalUser->m_CharacterLog.isTalk)
	{
		if(g_pLocalUser->m_CharacterLog.talkTime >= currentquest->GetStartTime() && currentquest->GetStartTime() != 0)
		{
			g_pLocalUser->m_CharacterLog.isTalk = FALSE;
			g_pLocalUser->m_CharacterLog.talkTime = 0;
			
			g_pLocalUser->m_AttackTargetObjectType = _XPICKINGTARGET_NONE;
			
#ifdef _XDWDEBUG	
			_XDWINPRINT("[QUESTCONDITION/ProcessCheckStatusTalkNPC] Talk NPC log clear");
#endif

			return QUEST_CONDITION_RESULT_SUCCESS;
		}
	}

	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� ó��
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStatusDeath(void)
{
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;

	if(g_pLocalUser->m_CharacterLog.isDead)
	{
		if(g_pLocalUser->m_CharacterLog.deadTime >= currentquest->GetStartTime() && currentquest->GetStartTime() != 0)
		{
			g_pLocalUser->m_CharacterLog.isDead = FALSE;
			g_pLocalUser->m_CharacterLog.deadTime = 0;
			
#ifdef _XDWDEBUG	
			_XDWINPRINT("[QUESTCONDITION/ProcessCheckStatusDeath] Death log clear");
#endif

			return QUEST_CONDITION_RESULT_SUCCESS;
		}
	}
	
	return QUEST_CONDITION_RESULT_FAIL;
}

// ��Ȱ ó��
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStatusRevive(void)
{
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;

	if(g_pLocalUser->m_CharacterLog.isRevive)
	{
		if(g_pLocalUser->m_CharacterLog.reviveTime >= currentquest->GetStartTime() && currentquest->GetStartTime() != 0)
		{
			g_pLocalUser->m_CharacterLog.isRevive = FALSE;
			g_pLocalUser->m_CharacterLog.reviveTime = 0;
			
#ifdef _XDWDEBUG	
			_XDWINPRINT("[QUESTCONDITION/ProcessCheckStatusRevive] Revive log clear");
#endif

			return QUEST_CONDITION_RESULT_SUCCESS;
		}
	}

	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckGender(void)
{
	// m_compareclass1 : Gender
	switch(m_TDC.m_compareclass1)
	{
	case ID_QUEST_GENDER_MAN :
		if(g_pLocalUser->m_CharacterInfo.gender == _XGENDER_MAN)
			return QUEST_CONDITION_RESULT_SUCCESS;
		else
			break;

	case ID_QUEST_GENDER_WOMAN :
		if(g_pLocalUser->m_CharacterInfo.gender == _XGENDER_WOMAN)
			return QUEST_CONDITION_RESULT_SUCCESS;
		else
			break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

BOOL _XTriggerConditionItem::CheckJoinGroup(int groupindex )
{
	switch(groupindex)
	{
	case ID_QUEST_JOINGROUP_NANGIN :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_GAEBANG :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_BEEGOONG :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_SORIM :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_NOCKRIM :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_MOODANG :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_MAKYO :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_SEGA :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SEGA)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_QUEST_JOINGROUP_NOFIXED :
		{
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOFIXED)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	}

	return FALSE;
}
// ���� üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckJoinGroup(void)
{
	// m_compareclass1 : ����
	if( !CheckJoinGroup(m_TDC.m_compareclass1) )
	{
		if( !CheckJoinGroup(m_TDC.m_compareclass2) )
		{
			if( m_TDC.m_compareclass2 == -1 )
			{
				return QUEST_CONDITION_RESULT_FAIL;
			}
			else
			{
				if( !CheckJoinGroup(m_TDC.m_compareclass3) )
				{
					if( m_TDC.m_compareclass3 == -1 )
					{
						return QUEST_CONDITION_RESULT_FAIL;
					}
					else
					{
						if( !CheckJoinGroup(m_TDC.m_compareclass4) )
						{
							if( m_TDC.m_compareclass4 == -1 )
							{
								return QUEST_CONDITION_RESULT_FAIL;
							}
							else
							{
								if( !CheckJoinGroup(m_TDC.m_numericvalue1) )
								{
									if( m_TDC.m_numericvalue1 == -1 )
									{
										return QUEST_CONDITION_RESULT_FAIL;
									}
									else
									{
										if( !CheckJoinGroup(m_TDC.m_numericvalue2) )
										{
											if( m_TDC.m_numericvalue2 == -1 )
											{
												return QUEST_CONDITION_RESULT_FAIL;
											}
											else
											{
												if( !CheckJoinGroup(m_TDC.m_numericvalue3) )
												{
													if( m_TDC.m_numericvalue3 == -1 )
													{
														return QUEST_CONDITION_RESULT_FAIL;
													}
													else
													{
														if( !CheckJoinGroup(m_TDC.m_numericvalue4) )
														{
															if( m_TDC.m_numericvalue4 == -1 )
															{
																return QUEST_CONDITION_RESULT_FAIL;
															}
														}//m_TDC.m_numericvalue4
													}
												}//m_TDC.m_numericvalue3
											}
										}//m_TDC.m_numericvalue2
									}
								}//m_TDC.m_numericvalue1
							}
						}//m_TDC.m_compareclass4
					}
				}//m_TDC.m_compareclass3
			}	
		}//m_TDC.m_compareclass2
	}// m_TDC.m_compareclass1

	return QUEST_CONDITION_RESULT_SUCCESS;
}

// �÷��̾�_���� üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckArea(void)
{
	// m_compareclass1 : area category
	// m_numericvalue1 : area
	
	if( (g_LocalSystemTime - m_LastCheckTime) > 3000 )
	{
		m_LastCheckTime = g_LocalSystemTime;
		if(g_AreaManager.FindPositionInArea(g_pLocalUser->m_Position, m_TDC.m_numericvalue1, m_TDC.m_compareclass1))
		{			
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// �޼��� üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckMessage(void)
{
	// m_compareclass1 : Operation
	// m_comparestring : message
	// m_numericvalue1 : ������ �ܾ� ��

	_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
	
	if(!pNPCScript_Window) return QUEST_CONDITION_RESULT_PROCESS;
	if( !pNPCScript_Window->m_NPCDialogIMEBorder1->GetShowStatus() ) pNPCScript_Window->m_NPCDialogIMEBorder1->ShowWindow(TRUE);
	if( !pNPCScript_Window->m_NPCDialogIMEBorder2->GetShowStatus() ) pNPCScript_Window->m_NPCDialogIMEBorder2->ShowWindow(TRUE);
	if( pNPCScript_Window->m_btnQuestOK->GetShowStatus() ) pNPCScript_Window->m_btnQuestOK->ShowWindow(FALSE);	
	if( pNPCScript_Window->m_btnBackToNPCMode->GetShowStatus() ) pNPCScript_Window->m_btnBackToNPCMode->ShowWindow(FALSE);
	
	if( !pNPCScript_Window->m_QuestIMEControl.GetShowStatus() ) 
	{
		pNPCScript_Window->m_QuestIMEControl.ShowWindow(TRUE);
		pNPCScript_Window->m_QuestIMEControl.SetFocus();
	}
	
	if( !pNPCScript_Window->m_bQuestStringInputComplete ) return QUEST_CONDITION_RESULT_PROCESS;

	string strCompareString, strChatString;
	string word[ID_MAX_CHECKMESSAGE];

	int location1 = 0, location2 = 0;
	int i = 0;
	int findcount = 0;

	strCompareString = m_lpszCompareString;
	strChatString	= pNPCScript_Window->m_QuestString;

	pNPCScript_Window->m_bQuestStringInputComplete = FALSE;
	pNPCScript_Window->m_NPCDialogIMEBorder1->ShowWindow(FALSE);
	pNPCScript_Window->m_NPCDialogIMEBorder2->ShowWindow(FALSE);
	pNPCScript_Window->m_QuestIMEControl.ShowWindow(FALSE);
	_XIMEKERNEL.SetFocus( NULL );	
	_XIMEKERNEL.ResetCombinationBuffer();	

	int questscringlength= 0;
	TCHAR queststring[1024];
	TCHAR comparestring[1024];
	TCHAR wordstring[ID_MAX_CHECKMESSAGE][1024];
	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )// ��������϶� 
	{
		memset( queststring, 0, sizeof(queststring) );
		memset( comparestring, 0, sizeof(comparestring) );
		memset( wordstring, 0, sizeof(wordstring) );
		strncpy( queststring, pNPCScript_Window->m_QuestString, sizeof(queststring) );
		strncpy( comparestring, m_lpszCompareString, sizeof(comparestring) );
		
		for(int i = 0 ; i < ID_MAX_CHECKMESSAGE ; ++i)
		{
			for( int j = 0 ; j< 1024; ++j )
			{
				if( comparestring[j] == '{' )
					location1 = j;
				if( comparestring[j] == '}' )
				{
					location2 = j;
					break;
				}
			}
			
			if( location2 == 0 ) break;
			
			strncpy( wordstring[i], comparestring+(location1+1),(location2-location1-1) );
			comparestring[location1] = ' ';	
			comparestring[location2] = ' ';	
			location1 = location2 = 0;
		}
	}
	else
	{
		// parsing
		for(i = 0 ; i < ID_MAX_CHECKMESSAGE ; ++i)
		{
			location1 = strCompareString.find('{');
			location2 = strCompareString.find('}');

			if(location2 == -1)
				break;

			for(int j = location1 + 1 ; j < location2 ; ++j)
				word[i] += strCompareString[j];

			strCompareString.erase(location1, location2 + 1);
		}
	}

	// operation
	switch(m_TDC.m_compareclass1)
	{
	case ID_QUEST_MATCHMESSAGE_WHOLEWORD :
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )// ��������϶� 
			{
				for(i = 0 ; i < ID_MAX_CHECKMESSAGE ; ++i)
				{
					if( wordstring[i][0] == 0 )
						break;

					if( stricmp(queststring, wordstring[i]) != 0 )
					{
						return QUEST_CONDITION_RESULT_FAIL;
					}
				}
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )// ���þƾ�����϶� 
			{
				for(i = 0 ; i < ID_MAX_CHECKMESSAGE ; ++i)
				{
					if( wordstring[i][0] == 0 )
						break;

					if( stricmp(queststring, wordstring[i]) != 0 )
					{
						return QUEST_CONDITION_RESULT_FAIL;
					}
				}
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )// ��Ʈ�������϶� convert lowercase & strip accent compare
			{
				TCHAR	TempConvString[2048];
				memset( TempConvString, 0, sizeof(TCHAR) * 2048 );
				
				int inputlen = strlen(queststring);
				int maxlength = 2048;
				_XCP1258Conv( (LPBYTE)queststring, (LPBYTE)TempConvString, inputlen, maxlength, CONV_LOWERCASE | CONV_STRIPACCENT );

				for(i = 0 ; i < ID_MAX_CHECKMESSAGE ; ++i)
				{
					if( wordstring[i][0] == 0 )
						break;

					TCHAR	TempConvString2[2048];
					memset( TempConvString2, 0, sizeof(TCHAR) * 2048 );

					int inputlen2 = strlen(wordstring[i]);
					int maxlength2 = 2048;
					_XCP1258Conv( (LPBYTE)wordstring[i], (LPBYTE)TempConvString2, inputlen2, maxlength2, CONV_LOWERCASE | CONV_STRIPACCENT );
					
					if( stricmp(TempConvString, TempConvString2) != 0 )
					{
						return QUEST_CONDITION_RESULT_FAIL;
					}
				}
			}
			else
			{
				for(i = 0 ; i < ID_MAX_CHECKMESSAGE ; ++i)
				{
					if(word[i].empty())
						break;
					if(strChatString.find(word[i]) == -1)
					{
						return QUEST_CONDITION_RESULT_FAIL;
					}
				}
			}
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;

	case ID_QUEST_MATCHMESSAGE_PARTIALWORD :
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )// ��������϶� 
			{
				int templength = 0;
				TCHAR tempword[1024];
				memset( tempword, 0, sizeof(TCHAR)*1024 );
				for(i = 0 ; i < ID_MAX_CHECKMESSAGE ; ++i)
				{
					if( wordstring[i][0] == 0 )
						break;
					
					for( int j = 0 ; j< 1024; ++j )
					{
						if( queststring[j] == NULL ) break;

						if( queststring[j] == wordstring[i][0] )
						{
							templength = strlen( wordstring[i] );
							strncpy(tempword, queststring+j, templength );
							
							if( stricmp(tempword, wordstring[i]) == 0 )
								findcount++;
						
							memset( tempword, 0, sizeof(TCHAR)*1024 );
						}		
					}
				}
				
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )// ���þƾ�����϶� 
			{
				int templength = 0;
				TCHAR tempword[1024];
				memset( tempword, 0, sizeof(TCHAR)*1024 );
				for(i = 0 ; i < ID_MAX_CHECKMESSAGE ; ++i)
				{
					if( wordstring[i][0] == 0 )
						break;
					
					for( int j = 0 ; j< 1024; ++j )
					{
						if( queststring[j] == NULL ) break;

						if( queststring[j] == wordstring[i][0] )
						{
							templength = strlen( wordstring[i] );
							strncpy(tempword, queststring+j, templength );
							
							if( stricmp(tempword, wordstring[i]) == 0 )
								findcount++;
						
							memset( tempword, 0, sizeof(TCHAR)*1024 );
						}		
					}
				}
				
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )// ��Ʈ�������϶� 
			{
				int templength = 0;
				TCHAR tempword[1024];
				memset( tempword, 0, sizeof(TCHAR)*1024 );

				TCHAR	TempConvString[2048];
				memset( TempConvString, 0, sizeof(TCHAR) * 2048 );				
				int inputlen = strlen(queststring);
				int maxlength = 2048;
				_XCP1258Conv( (LPBYTE)queststring, (LPBYTE)TempConvString, inputlen, maxlength, CONV_LOWERCASE | CONV_STRIPACCENT );


				for(i = 0 ; i < ID_MAX_CHECKMESSAGE ; ++i)
				{
					if( wordstring[i][0] == 0 )
						break;
					
					for( int j = 0 ; j< 1024; ++j )
					{
						if( TempConvString[j] == NULL ) break;

						TCHAR	TempConvString2[2048];
						memset( TempConvString2, 0, sizeof(TCHAR) * 2048 );
						int inputlen2 = strlen(wordstring[i]);
						int maxlength2 = 2048;
						_XCP1258Conv( (LPBYTE)wordstring[i], (LPBYTE)TempConvString2, inputlen2, maxlength2, CONV_LOWERCASE | CONV_STRIPACCENT );

						if( TempConvString[j] == TempConvString2[0] )
						{
							templength = strlen( TempConvString2 );
							strncpy(tempword, TempConvString+j, templength );
							
							if( stricmp(tempword, TempConvString2) == 0 )
								findcount++;
							
							memset( tempword, 0, sizeof(TCHAR)*1024 );
						}		
					}
				}
				
			}
			else
			{
				for(i = 0 ; i < ID_MAX_CHECKMESSAGE ; ++i)
				{
					if(word[i].empty())
						break;
					if(strChatString.find(word[i]) != -1)
						findcount++;
				}
			}

			if(findcount >= m_TDC.m_numericvalue1)
			{
				return QUEST_CONDITION_RESULT_SUCCESS;
			}
			else
			{
				return QUEST_CONDITION_RESULT_FAIL;				
			}
		}
		break;
		
	default :
		return QUEST_CONDITION_RESULT_FAIL;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ������ üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessHaveItem(void)
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
						return QUEST_CONDITION_RESULT_SUCCESS;
				}
				break;
			case ID_TRIGGER_COMPARE_THEMOST :
				{
					if(g_pLocalUser->m_Money > m_TDC.m_numericvalue2)
						return QUEST_CONDITION_RESULT_SUCCESS;
				}
				break;
			case ID_TRIGGER_COMPARE_EXACTLY :
				{
					if(g_pLocalUser->m_Money == m_TDC.m_numericvalue2)
						return QUEST_CONDITION_RESULT_SUCCESS;
				}
				break;
			}
		}
	}
	else
	{
		//for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
        for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; ++i) // ������ �񱳴� �೶ �Ⱓ ���� �Ŀ��� �� �� �ִ�.
		{
			if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == m_TDC.m_compareclass1)
			{
				if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDC.m_numericvalue1)
					totalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();
			}
		}
	/*#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
		// ������ �񱳴� �೶ �Ⱓ ���� �Ŀ��� �� �� �ִ�.
		for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition() ; i < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART ; ++i)
//		for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
		{
			if(g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0)
			{
				if(g_pLocalUser->m_UserItemList[i].m_cType == m_TDC.m_compareclass1)
				{
					if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDC.m_numericvalue1)
						totalcount += g_pLocalUser->m_UserItemList[i].m_ucCount;
				}
			}
		}
	#endif*/

		if(totalcount >= 0)
		{
			switch(m_TDC.m_compareclass2)
			{
			case ID_TRIGGER_COMPARE_THELEAST :
				{
					if(totalcount < m_TDC.m_numericvalue2)
						return QUEST_CONDITION_RESULT_SUCCESS;
				}
				break;
			case ID_TRIGGER_COMPARE_THEMOST :
				{
					if(totalcount > m_TDC.m_numericvalue2)
					{
						if( m_bStartCondition )
						{
							// ���������϶� ������ üũ�� ������ ������ ������ ������ �ִ´�. 
							// �� �������� ������� ���������� �ٽ� �˻��ؾ� �Ѵ�.
							if( !g_QuestScriptManager.m_listCheckItem.empty() )
							{
								bool balreadyexist = false;
								list <_XQUEST_ITEMINFO>::iterator iter_checkitem;
								for(iter_checkitem = g_QuestScriptManager.m_listCheckItem.begin() ; iter_checkitem != g_QuestScriptManager.m_listCheckItem.end() ;++iter_checkitem )
								{
									_XQUEST_ITEMINFO checkitem = *iter_checkitem;
									if( checkitem.questid == m_nOwnerQuestID 
										&& checkitem.itemtype == m_TDC.m_compareclass1 
										&& checkitem.itemid	== m_TDC.m_numericvalue1	)
									{
										balreadyexist = true;
										break;
									}
								}
								if( !balreadyexist )
								{
									_XQUEST_ITEMINFO checkitem;
									memset(&checkitem, 0, sizeof(_XQUEST_ITEMINFO));
									checkitem.questid	= m_nOwnerQuestID;
									checkitem.itemtype	= m_TDC.m_compareclass1;
									checkitem.itemid	= m_TDC.m_numericvalue1;
									g_QuestScriptManager.m_listCheckItem.push_back(checkitem);
								}
							}
							else
							{
								_XQUEST_ITEMINFO checkitem;
								memset(&checkitem, 0, sizeof(_XQUEST_ITEMINFO));
								checkitem.questid	= m_nOwnerQuestID;
								checkitem.itemtype	= m_TDC.m_compareclass1;
								checkitem.itemid	= m_TDC.m_numericvalue1;
								g_QuestScriptManager.m_listCheckItem.push_back(checkitem);
							}
						}

						return QUEST_CONDITION_RESULT_SUCCESS;
					}
				}
				break;
			case ID_TRIGGER_COMPARE_EXACTLY :
				{
					if(totalcount == m_TDC.m_numericvalue2)
					{
						if( m_bStartCondition )
						{
							if( m_bStartCondition )
							{
								// ���������϶� ������ üũ�� ������ ������ ������ ������ �ִ´�. 
								// �� �������� ������� ���������� �ٽ� �˻��ؾ� �Ѵ�.
								if( !g_QuestScriptManager.m_listCheckItem.empty() )
								{
									bool balreadyexist = false;
									list <_XQUEST_ITEMINFO>::iterator iter_checkitem;
									for(iter_checkitem = g_QuestScriptManager.m_listCheckItem.begin() ; iter_checkitem != g_QuestScriptManager.m_listCheckItem.end() ;++iter_checkitem )
									{
										_XQUEST_ITEMINFO checkitem = *iter_checkitem;
										if( checkitem.questid == m_nOwnerQuestID 
											&& checkitem.itemtype == m_TDC.m_compareclass1 
											&& checkitem.itemid	== m_TDC.m_numericvalue1	)
										{
											balreadyexist = true;
											break;
										}
									}
									if( !balreadyexist )
									{
										_XQUEST_ITEMINFO checkitem;
										memset(&checkitem, 0, sizeof(_XQUEST_ITEMINFO));
										checkitem.questid	= m_nOwnerQuestID;
										checkitem.itemtype	= m_TDC.m_compareclass1;
										checkitem.itemid	= m_TDC.m_numericvalue1;
										g_QuestScriptManager.m_listCheckItem.push_back(checkitem);
									}
								}
								else
								{
									_XQUEST_ITEMINFO checkitem;
									memset(&checkitem, 0, sizeof(_XQUEST_ITEMINFO));
									checkitem.questid	= m_nOwnerQuestID;
									checkitem.itemtype	= m_TDC.m_compareclass1;
									checkitem.itemid	= m_TDC.m_numericvalue1;
									g_QuestScriptManager.m_listCheckItem.push_back(checkitem);
								}
							}
						}
						
						return QUEST_CONDITION_RESULT_SUCCESS;
					}
				}
				break;
			}
		}
	}
	
	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� ���
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckAttack(void)
{
	// m_compareclass1 : ����
	// m_compareclass2 = m_CounterType;
	// m_compareclass3 = m_ItemClass;//������ Ÿ���϶� 
	// m_numericvalue1 : ID(Type) (4000 ����)
	// m_numericvalue2 = m_MobNumber;
	// m_numericvalue3 = m _ItemID ;//������ Ÿ���϶� 

	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;

	int time = currentquest->GetStartTime();

	switch(m_TDC.m_compareclass1)
	{
	case ID_QUEST_CHECKATTACK_TARGETTING :
		{
			if(g_pLocalUser->m_CharacterLog.isAttack)
			{
				int mobtype = m_TDC.m_numericvalue1 - 4000;

				if(g_pLocalUser->m_CharacterLog.attackTargetType == mobtype)
				{
					if(g_pLocalUser->m_CharacterLog.attackTime >= time && time != 0)
					{
						if((g_ServerTimeCode - g_pLocalUser->m_CharacterLog.attackTime) < 10)
						{
							g_pLocalUser->m_CharacterLog.isAttack = FALSE;
							g_pLocalUser->m_CharacterLog.attackTime = 0;
							g_pLocalUser->m_CharacterLog.attackTargetType = 0;
							
#ifdef _XDWDEBUG	
							_XDWINPRINT("[QUESTCONDITION/ProcessCheckAttack] Attack log clear");
#endif

							return QUEST_CONDITION_RESULT_SUCCESS;
						}
					}
				}
			}
		}
		break;
	case ID_QUEST_CHECKATTACK_DEATH :
	case ID_QUEST_CHECKATTACK_CLIENTMOBDEATH : // Ŭ���̾�Ʈ ���� ���� �˻�
		{
			if( !m_bStartCondition )
			{					
				// ���� üũ ����...ī���Ͱ� �����ϸ�...�������� ó���Ѵ�...�ƴϸ� ������ �������� ���⼭ �����. 
				if( m_TDC.m_numericvalue2 != 0 )
				{
					if( m_TDC.m_compareclass2 < 8 )
					{
						if( m_TDC.m_compareclass2 < 4)
						{
							if( currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountCurrentPoint >=  m_TDC.m_numericvalue2 )
							{						
#ifdef _XDWDEBUG	
								_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CONDITION RESULT)     >>>>>>>>>>>>>>>>>>>>" );
								_XLog("Quest Condition Attack Death Success : QuestID[%d] SucceessNode[%d] ", m_nOwnerQuestID,currentquest->GetCurrentTreeNumber() );
#endif
								
								return QUEST_CONDITION_RESULT_SUCCESS;
							}
						}
						else //��Ƽ ī���� 
						{
							if( currentquest->m_PartyCounterInformation[m_TDC.m_compareclass2-4].CountCurrentPoint >=  m_TDC.m_numericvalue2 )
							{
								//��Ƽ ī���͸� �����ϴ� �κ��� �� ����
								
#ifdef _XDWDEBUG	
								_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CONDITION RESULT)     >>>>>>>>>>>>>>>>>>>>" );
								_XLog("Quest Condition Attack Death Success : QuestID[%d] SucceessNode[%d] ", m_nOwnerQuestID,currentquest->GetCurrentTreeNumber() );
#endif
								
								return QUEST_CONDITION_RESULT_SUCCESS;
							}
						}
					}
					//else if ( m_TDC.m_compareclass2 == 8 || m_TDC.m_compareclass2 == 9)// ������ Ÿ���϶� || ������ Ÿ��(���÷��� ����)�϶�
					//{
					//}
				}


				
				if(g_pLocalUser->m_CharacterLog.isKill)
				{
					int mobtype = m_TDC.m_numericvalue1 - 4000;
					
					if(g_pLocalUser->m_CharacterLog.killTargetType == mobtype)
					{
						if(g_pLocalUser->m_CharacterLog.killTime >= time && time != 0)
						{
							if((g_ServerTimeCode - g_pLocalUser->m_CharacterLog.killTime) < 10)
							{
								if( m_TDC.m_compareclass2 < 8 )
								{
									if( m_TDC.m_compareclass2 < 4)
									{

										g_pLocalUser->m_CharacterLog.isKill = FALSE;
										g_pLocalUser->m_CharacterLog.killTime = 0;
										g_pLocalUser->m_CharacterLog.killTargetType = 0;

											
										currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountCurrentPoint++;
										
										TCHAR	buffer[512];
										memset(buffer, 0, sizeof(buffer));
										if( currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountTotalPoint >= 1 )
										{
											if(currentquest->m_CounterInformation[m_TDC.m_compareclass2].CounterTargetName[0] != 0 )
											{
												if( currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountTotalPoint > currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountCurrentPoint)
												{
													_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
														currentquest->m_CounterInformation[m_TDC.m_compareclass2].CounterTargetName,
														currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountCurrentPoint,
														currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountTotalPoint );
													
													if( g_pQuest_Window )
													{
														if( g_pQuest_Window->GetShowStatus() )
														{
															if( g_pQuest_Window->m_SelectedQuestID == m_nOwnerQuestID  )
															{
																g_pQuest_Window->ParseQuestDescString();	
															}
														}
													}
												}
												else if( currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountTotalPoint == currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountCurrentPoint )
												{
													_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT),//_T("%s : %d/%d (�Ϸ�)")
														currentquest->m_CounterInformation[m_TDC.m_compareclass2].CounterTargetName,
														currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountTotalPoint,
														currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountTotalPoint );
													
													if( g_pQuest_Window )
													{
														if( g_pQuest_Window->GetShowStatus() )
														{
															if( g_pQuest_Window->m_SelectedQuestID == m_nOwnerQuestID  )
															{
																g_pQuest_Window->ParseQuestDescString();	
															}
														}
													}
												}	
												
												if( currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountTotalPoint >= currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountCurrentPoint )
												{
													if( buffer[0] != 0 )
													{
														int strlength = strlen(buffer);
														
														if(g_QuestScriptManager.m_lpszQuestSystemMessage )
														{
															delete g_QuestScriptManager.m_lpszQuestSystemMessage;
															g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
														}
														
														if(strlength > 0)
														{
															g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
															g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
															strncpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer, strlength);
														}
														
														g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
														g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
													}		
												}
															
											}							
										}	
										else if( currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountTotalPoint == 0 )
										{
											if(currentquest->m_CounterInformation[m_TDC.m_compareclass2].CounterTargetName[0] != 0 )
											{
												_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
													currentquest->m_CounterInformation[m_TDC.m_compareclass2].CounterTargetName,
													currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountCurrentPoint);
												
												if( g_pQuest_Window )
												{
													if( g_pQuest_Window->GetShowStatus() )
													{
														if( g_pQuest_Window->m_SelectedQuestID == m_nOwnerQuestID  )
														{
															g_pQuest_Window->ParseQuestDescString();	
														}
													}
												}
												
												if( buffer[0] != 0 )
												{
													int strlength = strlen(buffer);
													
													if(g_QuestScriptManager.m_lpszQuestSystemMessage )
													{
														delete g_QuestScriptManager.m_lpszQuestSystemMessage;
														g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
													}
													
													if(strlength > 0)
													{
														g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
														g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
														strncpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer, strlength);
													}
													
													g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
													g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
												}		
												
											}
										}
										
										g_NetworkKernel.SendPacket(MSG_NO_QUEST_SETCOUNT,m_nOwnerQuestID,m_TDC.m_compareclass2,1,currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountCurrentPoint);
															

										if( currentquest->m_CounterInformation[m_TDC.m_compareclass2].CountCurrentPoint >=  m_TDC.m_numericvalue2 )
										{					
#ifdef _XDWDEBUG	
											_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CONDITION RESULT)     >>>>>>>>>>>>>>>>>>>>" );
											_XLog("Quest Condition Attack Death Success : QuestID[%d] SucceessNode[%d] MOBID[%d]", m_nOwnerQuestID,currentquest->GetCurrentTreeNumber(), mobtype );
#endif	
											
											return QUEST_CONDITION_RESULT_SUCCESS;
										}
										return QUEST_CONDITION_RESULT_FAIL;
										
									
									}
									else//��Ƽ ����Ʈ �϶� ��Ƽ ī���͵� �÷��ش�.
									{
										g_pLocalUser->m_CharacterLog.isKill = FALSE;
										g_pLocalUser->m_CharacterLog.killTime = 0;
										g_pLocalUser->m_CharacterLog.killTargetType = 0;
															
										if( currentquest->m_PartyCounterInformation[m_TDC.m_compareclass2-4].CountCurrentPoint >=  m_TDC.m_numericvalue2 )
										{
											//��Ƽ ī���͸� �����ϴ� �κ��� �� ����
											
#ifdef _XDWDEBUG	
											_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CONDITION RESULT)     >>>>>>>>>>>>>>>>>>>>" );
											_XLog("Quest Condition Attack Death Success : QuestID[%d] SucceessNode[%d] MOBID[%d]", m_nOwnerQuestID,currentquest->GetCurrentTreeNumber(), mobtype );
#endif
											
											return QUEST_CONDITION_RESULT_SUCCESS;
										}

										g_NetworkKernel.SendPacket(MSG_NO_QUEST_SETCOUNT,m_nOwnerQuestID,m_TDC.m_compareclass2-4,1,currentquest->m_PartyCounterInformation[m_TDC.m_compareclass2-4].CountCurrentPoint);
										return QUEST_CONDITION_RESULT_FAIL;
									}
								}
								else if( m_TDC.m_compareclass2 == 8 || m_TDC.m_compareclass2 == 9)// ������ Ÿ���϶� || ������ Ÿ��(���÷��� ����)�϶�
								{
									g_pLocalUser->m_CharacterLog.isKill = FALSE;
									g_pLocalUser->m_CharacterLog.killTime = 0;
									g_pLocalUser->m_CharacterLog.killTargetType = 0;

									if( !currentquest->GetPartyQuest() )
									{
										int itemtotalcount = 0;
										for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
										{
											if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == m_TDC.m_compareclass3 )
											{
												if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDC.m_numericvalue3)
													itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();
											}
										}
									#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ�� 
										// ������ �񱳴� �೶ �Ⱓ ���� �Ŀ��� �� �� �ִ�.
										for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART; i++) 
//										for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
										{
											if(g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0)
											{
												if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == m_TDC.m_compareclass3 )
												{
													if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDC.m_numericvalue3)
														itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();
												}
											}
										}
									#endif
										
										if( itemtotalcount >= m_TDC.m_numericvalue2 )//�ʿ��� �������� ���� ������ ������ �ѱ��� �ʴ´�.
											return QUEST_CONDITION_RESULT_FAIL;
										
#ifdef _XDWDEBUG	
										_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CONDITION RESULT)     >>>>>>>>>>>>>>>>>>>>" );
										_XLog("Quest Condition Attack Death Success : QuestID[%d] SucceessNode[%d] MOBID[%d]", m_nOwnerQuestID,currentquest->GetCurrentTreeNumber(), mobtype );
#endif
										return QUEST_CONDITION_RESULT_SUCCESS;			
										
									}
									else// ���� �����һ��̸� �κ��丮�� �˻��ϴ� ���� �ƴ϶� ��Ƽī���͸� �˻��ؾ� �Ѵ�. ( ����� ������ �೶�� �˻��� ���� ����.)
									{
										int itemtotalcount = 0;
										for( int partycountindex = 0; partycountindex < 4; ++partycountindex )
										{
											if(currentquest->m_PartyCounterInformation[partycountindex].CounterTargetType == m_TDC.m_compareclass3 )
											{
												if(currentquest->m_PartyCounterInformation[partycountindex].CounterTargetID == m_TDC.m_numericvalue3)
												{
													itemtotalcount = currentquest->m_PartyCounterInformation[partycountindex].CountCurrentPoint;
													break;
												}
											}												
										}

										if( itemtotalcount >= m_TDC.m_numericvalue2 )//�ʿ��� �������� ���� ������ ������ �ѱ��� �ʴ´�.
											return QUEST_CONDITION_RESULT_FAIL;
										
#ifdef _XDWDEBUG	
										_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CONDITION RESULT)     >>>>>>>>>>>>>>>>>>>>" );
										_XLog("Quest Condition Attack Death Success : QuestID[%d] SucceessNode[%d] MOBID[%d]", m_nOwnerQuestID,currentquest->GetCurrentTreeNumber(), mobtype );
#endif									
										return QUEST_CONDITION_RESULT_SUCCESS;			

									}								
								}//else if( m_TDC.m_compareclass2 == 8 )// ������ Ÿ���϶� 
							}
						}
					}
				}
			}
			else //���������϶��� Ŭ���̾�Ʈ ��ü���� ���� ų�� üũ�ؾ��Ѵ�.
			{
				if(g_pLocalUser->m_CharacterLog.isKill)
				{
					int mobtype = m_TDC.m_numericvalue1 - 4000;
					
					if(g_pLocalUser->m_CharacterLog.killTargetType == mobtype)
					{
						if((g_ServerTimeCode - g_pLocalUser->m_CharacterLog.killTime) < 10)
						{								
							g_pLocalUser->m_CharacterLog.isKill = FALSE;
							g_pLocalUser->m_CharacterLog.killTime = 0;
							g_pLocalUser->m_CharacterLog.killTargetType = 0;
														
							return QUEST_CONDITION_RESULT_SUCCESS;
						
						}
					}
				}
										
			}
		}
		break;
		
	case ID_QUEST_CHECKATTACK_HIT :
		{
			if(g_pLocalUser->m_CharacterLog.isHit)
			{
				int mobtype = m_TDC.m_numericvalue1 - 4000;
				
				if(g_pLocalUser->m_CharacterLog.hitTargetType == mobtype)
				{
					if(g_pLocalUser->m_CharacterLog.hitTime >= time && time != 0)
					{
						if((g_ServerTimeCode - g_pLocalUser->m_CharacterLog.hitTime) < 10)
						{
							g_pLocalUser->m_CharacterLog.isHit = FALSE;
							g_pLocalUser->m_CharacterLog.hitTime = 0;
							g_pLocalUser->m_CharacterLog.hitTargetType = 0;
							
#ifdef _XDWDEBUG	
							_XDWINPRINT("[QUESTCONDITION/ProcessCheckAttack] Hit log clear");
#endif
							
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
					}
				}
			}
		}
		break;
	case ID_QUEST_CHECKATTACK_CHANGE : // ���� üũ 
		{
			if(g_pLocalUser->m_CharacterLog.isChange)
			{
				int mobtype = m_TDC.m_numericvalue1 - 4000;
				
				if(g_pLocalUser->m_CharacterLog.changeTargetType == mobtype)
				{
					if(g_pLocalUser->m_CharacterLog.changeTime >= time && time != 0)
					{
						if((g_ServerTimeCode - g_pLocalUser->m_CharacterLog.changeTime) < 10)
						{
							g_pLocalUser->m_CharacterLog.isChange = FALSE;
							g_pLocalUser->m_CharacterLog.changeTime = 0;
							g_pLocalUser->m_CharacterLog.changeTargetType = 0;
							g_pLocalUser->m_CharacterLog.changeTargetUniqueID = -1;
							
#ifdef _XDWDEBUG	
							_XDWINPRINT("[QUESTCONDITION/ProcessCheckAttack] Change log clear");
#endif
							
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
					}
				}
			}
		}
		break;
	}

	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� ���� ��
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckMartial(void)
{
	// m_compareclass1 : ����
	// m_numericvlaue1 : �񱳽�
	// m_numericvalue2 : ���� ����

	int findcount = 0;

	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
	{
		if(g_pLocalUser->m_SkillList[i].m_sSkillID == 0)
			continue;

		if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->clanType == m_TDC.m_compareclass1)
			findcount++;
	}
	switch(m_TDC.m_numericvalue1)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		{
			if(findcount < m_TDC.m_numericvalue2)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_TRIGGER_COMPARE_THEMOST :
		{
			if(findcount > m_TDC.m_numericvalue2)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_TRIGGER_COMPARE_EXACTLY :
		{
			if(findcount == m_TDC.m_numericvalue2)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ��Ƽ���� ��
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckPartyCount(void)
{
	// m_compareclass1 : �񱳽�
	// m_numericvalue1 : ��ġ
	
	switch(m_TDC.m_compareclass1)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		{
			if( g_pLocalUser->m_PartyMemberCount < m_TDC.m_numericvalue1)
			{
				if( m_bStartCondition )
				{
					// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
					// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
					if( !g_QuestScriptManager.m_listCheckInParty.empty() )
					{
						bool balreadyexist = false;
						list <int>::iterator iter_checkinparty;
						for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
						{
							int partyquestid = *iter_checkinparty;
							if( partyquestid == m_nOwnerQuestID )
							{
								balreadyexist = true;
								break;
							}
						}
						if( !balreadyexist )
						{
							g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
						}
					}
					else
					{
						g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
					}
				}
				return QUEST_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case ID_TRIGGER_COMPARE_THEMOST :
		{
			if( g_pLocalUser->m_PartyMemberCount > m_TDC.m_numericvalue1)
			{
				if( m_bStartCondition )
				{
					// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
					// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
					if( !g_QuestScriptManager.m_listCheckInParty.empty() )
					{
						bool balreadyexist = false;
						list <int>::iterator iter_checkinparty;
						for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
						{
							int partyquestid = *iter_checkinparty;
							if( partyquestid == m_nOwnerQuestID )
							{
								balreadyexist = true;
								break;
							}
						}
						if( !balreadyexist )
						{
							g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
						}
					}
					else
					{
						g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
					}
				}
				return QUEST_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	case ID_TRIGGER_COMPARE_EXACTLY :
		{
			if( g_pLocalUser->m_PartyMemberCount == m_TDC.m_numericvalue1)
			{
				if( m_bStartCondition )
				{
					// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
					// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
					if( !g_QuestScriptManager.m_listCheckInParty.empty() )
					{
						bool balreadyexist = false;
						list <int>::iterator iter_checkinparty;
						for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
						{
							int partyquestid = *iter_checkinparty;
							if( partyquestid == m_nOwnerQuestID )
							{
								balreadyexist = true;
								break;
							}
						}
						if( !balreadyexist )
						{
							g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
						}
					}
					else
					{
						g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
					}
				}
				return QUEST_CONDITION_RESULT_SUCCESS;
			}
		}
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ��Ƽ�� ���� �˻�
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckPartyStatus(void)
{
	if( g_pLocalUser->m_InParty )
	{
		// m_compareclass1 : 0:������ 1:���� 2:���ϸʰ˻�
		switch(m_TDC.m_compareclass1) {
		case 0://0:������
			{
			}
			break;
		case 1://1:����
			{
			}
			break;
		case 2://2:���ϸʰ˻�
			{
				int indexmap = -1;
				for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
				{
					if( g_pLocalUser->m_PartyInfo[i].isValid )
					{
						if( indexmap == -1)
						{
							indexmap = g_pLocalUser->m_PartyInfo[i].serverno;
						}
						else
						{
							if( indexmap != g_pLocalUser->m_PartyInfo[i].serverno )
								return QUEST_CONDITION_RESULT_FAIL;
						}
					}
				}
				
				if( m_bStartCondition )
				{
					// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
					// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
					if( !g_QuestScriptManager.m_listCheckInParty.empty() )
					{
						bool balreadyexist = false;
						list <int>::iterator iter_checkinparty;
						for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
						{
							int partyquestid = *iter_checkinparty;
							if( partyquestid == m_nOwnerQuestID )
							{
								balreadyexist = true;
								break;
							}
						}
						if( !balreadyexist )
						{
							g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
						}
					}
					else
					{
						g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
					}
				}				
				return QUEST_CONDITION_RESULT_SUCCESS;
				
			}
			break;
		}
	}
	
	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� �˻� - Ÿ�÷��̾�
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckAreaOther(void)
{
	// m_compareclass1 : TargetClass (Party, StaticNPC, DynamicNPC)
	// m_compareclass2 : area category
	// m_numericvalue1 : target id
	// m_numericvalue2 : area id
	switch(m_TDC.m_compareclass1)
	{
	case ID_QUEST_TARGETOTHER_STATICNPC :
		return ProcessCheckAreaStaticNPC();

	case ID_QUEST_TARGETOTHER_DYNAMICNPC :
		return ProcessCheckAreaDynamicNPC();

	case ID_QUEST_TARGETOTHER_MONSTER :
		return ProcessCheckAreaMob();

	case ID_QUEST_TARGETOTHER_PARTY :
		return ProcessCheckAreaParty();
	}

	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� �˻� - Party
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckAreaParty()
{
	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� �˻� - Static NPC
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckAreaStaticNPC()
{
	// m_compareclass1 : TargetClass (Party, StaticNPC, DynamicNPC)
	// m_compareclass2 : area category
	// m_numericvalue1 : target id
	// m_numericvalue2 : area id

	for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; ++i)
	{
		if(g_Npclist.m_pNPCObjectArray[i].m_UniqueID == m_TDC.m_numericvalue1)
		{
			D3DXVECTOR3 position;
			position.x = g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._41;
			position.y = g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._42;
			position.z = g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._43;

			if(g_AreaManager.FindPositionInArea(position, m_TDC.m_numericvalue2, m_TDC.m_compareclass2))
				return QUEST_CONDITION_RESULT_SUCCESS;
			else
				break;
		}
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� �˻� - Dynamic NPC
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckAreaDynamicNPC()
{
	// m_compareclass1 : TargetClass (Party, StaticNPC, DynamicNPC)
	// m_compareclass2 : area category
	// m_numericvalue1 : target id
	// m_numericvalue2 : area id

	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� �˻� - Mob
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckAreaMob()
{
	// m_compareclass1 : TargetClass (Party, StaticNPC, DynamicNPC)
	// m_compareclass2 : area category
	// m_numericvalue1 : target id
	// m_numericvalue2 : area id

	int mobtype = m_TDC.m_numericvalue1;

	if(!g_Moblist.m_smMobList.empty())
	{
		_XMob* pMob;
		smdef_XMob::iterator it;
		for (it = g_Moblist.m_smMobList.begin(); it != g_Moblist.m_smMobList.end(); ++ it)
		{
			pMob = (*it).second;
			if(pMob->m_MobType == mobtype)
			{
				D3DXVECTOR3		position;
				position = pMob->m_Position;

				if(g_AreaManager.FindPositionInArea(position, m_TDC.m_numericvalue2, m_TDC.m_compareclass2))
					return QUEST_CONDITION_RESULT_SUCCESS;
			}
		}
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// �Ÿ� üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckDistance(void)
{
	// m_compareclass1 : Target class
	// m_compareclass2 : area category
	// m_numericvalue1 : Diatance
	// m_numericvalue2 : Target / Area

	switch(m_TDC.m_compareclass1)
	{
	case ID_QUEST_TARGETOBJECT_STATICNPC :
		return ProcessCheckDistanceFromStaticNPC();

	case ID_QUEST_TARGETOBJECT_DYNAMICNPC :
		return ProcessCheckDistanceFromDynamicNPC();

	case ID_QUEST_TARGETOBJECT_AREA :
		return ProcessCheckDistanceFromArea();
	}

	return QUEST_CONDITION_RESULT_FAIL;
}

// �Ÿ�üũ - Static NPC
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckDistanceFromStaticNPC(void)
{
	// m_compareclass1 : Target class
	// m_compareclass2 : area category
	// m_numericvalue1 : Diatance
	// m_numericvalue2 : Target / Area
	
	for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; ++i)
	{
		if(g_Npclist.m_pNPCObjectArray[i].m_UniqueID == m_TDC.m_numericvalue2)
		{
			D3DXVECTOR3		position;
			position.x = g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._41;
			position.y = g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._42;
			position.z = g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._43;

			FLOAT distance;
			distance = _XFC_sqrt((position.x - g_pLocalUser->m_Position.x)*(position.x - g_pLocalUser->m_Position.x)
								+ (position.y - g_pLocalUser->m_Position.y)*(position.y - g_pLocalUser->m_Position.y)
								+ (position.z - g_pLocalUser->m_Position.z)*(position.z - g_pLocalUser->m_Position.z));

			if(distance <= m_TDC.m_numericvalue1)
				return QUEST_CONDITION_RESULT_SUCCESS;
			else 
				break;
		}
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// �Ÿ� üũ - Dynamic NPC
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckDistanceFromDynamicNPC(void)
{
	// m_compareclass1 : Target class
	// m_compareclass2 : area category
	// m_numericvalue1 : Diatance
	// m_numericvalue2 : Target / Area

	return QUEST_CONDITION_RESULT_FAIL;
}

// �Ÿ� üũ - Area
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckDistanceFromArea(void)
{
	// m_compareclass1 : Target class
	// m_compareclass2 : area category
	// m_numericvalue1 : Diatance
	// m_numericvalue2 : Target / Area

	D3DXVECTOR3	position;

	if(g_AreaManager.FindAreaCenterPosition( position, m_TDC.m_numericvalue2, m_TDC.m_compareclass2))
	{
		FLOAT distance;
		distance = _XFC_sqrt((position.x - g_pLocalUser->m_Position.x)*(position.x - g_pLocalUser->m_Position.x)
							+ (position.y - g_pLocalUser->m_Position.y)*(position.y - g_pLocalUser->m_Position.y)
							+ (position.x - g_pLocalUser->m_Position.x)*(position.z - g_pLocalUser->m_Position.z));

		if(distance <= m_TDC.m_numericvalue1)
			return QUEST_CONDITION_RESULT_SUCCESS;
	}

	return QUEST_CONDITION_RESULT_FAIL;
}

// Ÿ���� üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckTargetting(void)
{
	// m_compareclass1 : Target class
	// m_compareclass2 : area category
	// m_numericvalue1 : Target ID / Area

	switch(m_TDC.m_compareclass1)
	{
	case ID_QUEST_TARGETTING_AREA :
		return ProcessCheckTargettingArea();

	case ID_QUEST_TARGETTING_STATICNPC :
		return ProcessCheckTargettingStaticNPC();
		
	case ID_QUEST_TARGETTING_DYNAMICNPC :
		return ProcessCheckTargettingDynamicNPC();

	case ID_QUEST_TARGETTING_OBJECT :
		return ProcessCheckTargettingObject();
	}

	return QUEST_CONDITION_RESULT_FAIL;
}

// targetting - area
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckTargettingArea(void)
{
	// m_compareclass1 : Target class
	// m_compareclass2 : area category
	// m_numericvalue1 : Target ID / Area
	// m_numericvalue2 : Map Index
	
	if(  m_TDC.m_numericvalue2  <= 0 ) // ���� ����Ÿ �˻縦 ���� �ʴ´�.
	{
		if(g_AreaManager.FindPositionInArea(g_pLocalUser->m_TargetPosition, m_TDC.m_numericvalue1, m_TDC.m_compareclass2))
			return QUEST_CONDITION_RESULT_SUCCESS;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else // �� �������� �˻��ؾ� �ϴ� ����Ÿ 
	{
		if( g_CurrentZoneInfoIndex == m_TDC.m_numericvalue2 )
		{
			if(g_AreaManager.FindPositionInArea(g_pLocalUser->m_TargetPosition, m_TDC.m_numericvalue1, m_TDC.m_compareclass2))
				return QUEST_CONDITION_RESULT_SUCCESS;
			else
				return QUEST_CONDITION_RESULT_FAIL;
		}
	}
	
	return QUEST_CONDITION_RESULT_FAIL;
}

// targetting - static npc
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckTargettingStaticNPC(void)
{
	// m_compareclass1 : Target class
	// m_compareclass2 : area category
	// m_numericvalue1 : Target ID / Area
	// m_numericvalue2 : Map Index
	
	if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_NPC)
	{
		
		if( !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_bNPCDialogMode )//��ȭ��尡 �ƴϸ� ���� 
		{
			return QUEST_CONDITION_RESULT_FAIL;		
		}
		
		_XQuestScriptItem* currentquest = NULL;
		
		if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
		{
			map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
			if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
				currentquest = iter_quest->second;
			else
				return QUEST_CONDITION_RESULT_FAIL;
		}
		else
		{
			map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
			if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
				currentquest = iter_quest->second;
			else
				return QUEST_CONDITION_RESULT_FAIL;
		}
		
		if(!currentquest)
			return QUEST_CONDITION_RESULT_FAIL;

		if(  m_TDC.m_numericvalue2  > 0 ) // �������� �˻��ϴ� �κ�
		{
			if( g_CurrentZoneInfoIndex != m_TDC.m_numericvalue2 )
			{
				return QUEST_CONDITION_RESULT_FAIL;
			}
		}

#ifdef _XTS_HAVEQUESTLISTNPC
		if( !m_bHoldCheckingOfNPCTargetting )
		{			
			_XNPCObject* npcobject = NULL;
			if( g_pLocalUser->m_AttackTarget )
			{			
				npcobject = (_XNPCObject *)g_pLocalUser->m_AttackTarget;
			}
			else if( g_pLocalUser->m_ViewTarget )
			{
				npcobject = (_XNPCObject *)g_pLocalUser->m_ViewTarget;
			}
			
			
			if(npcobject)
			{
				if(npcobject->m_UniqueID == m_TDC.m_numericvalue1)
				{
					g_QuestScriptManager.m_indexSelectNPCTargettingQuest = -1;
					// NPC Ÿ���� �˻� ��� ���°� �ƴϸ� 
					// Ÿ���� ��� ����Ʈ�� �ø��� ��� �Ѵ�.					
					if( !g_QuestScriptManager.m_listNPCTargettingQuest.empty() )
					{
						bool balreadyexist = false;
						list <_XQUEST_QUESTLIST>::iterator iter_npctargetting;
						for(iter_npctargetting = g_QuestScriptManager.m_listNPCTargettingQuest.begin() ; iter_npctargetting != g_QuestScriptManager.m_listNPCTargettingQuest.end() ;++iter_npctargetting )
						{
							_XQUEST_QUESTLIST npctargetting = *iter_npctargetting;
							if( npctargetting.questid == m_nOwnerQuestID )
							{
								balreadyexist = true;
								break;
							}
						}
						if( !balreadyexist )
						{
							_XQUEST_QUESTLIST npctargetting;
							memset(&npctargetting, 0, sizeof(_XQUEST_QUESTLIST));
							npctargetting.questid	= m_nOwnerQuestID;
							npctargetting.npcid		= m_TDC.m_numericvalue1;
							g_QuestScriptManager.m_listNPCTargettingQuest.push_back(npctargetting);
						}
					}
					else
					{
						_XQUEST_QUESTLIST npctargetting;
						memset(&npctargetting, 0, sizeof(_XQUEST_QUESTLIST));
						npctargetting.questid	= m_nOwnerQuestID;
						npctargetting.npcid		= m_TDC.m_numericvalue1;
						g_QuestScriptManager.m_listNPCTargettingQuest.push_back(npctargetting);
					}

					// Ÿ���� ��� ����Ʈ�� �ö� ������ ������ �� ������ ����Ѵ�.
					m_bHoldCheckingOfNPCTargetting = TRUE;
				}
			}			
			
		}
		else
		{
			if( g_QuestScriptManager.m_indexSelectNPCTargettingQuest != -1 )
			{
				// ��� ���� ����Ʈ���� ������ �Ǹ� ��� ����Ʈ�� ���ְ� ��⸦ Ǯ�������.
				if( g_QuestScriptManager.m_indexSelectNPCTargettingQuest == m_nOwnerQuestID )
				{				
					// �� NPC�� ���� ����Ʈ�� Ÿ������ �ɷ������� ����Ʈ ��ȣ�� �����ϴ� ����Ʈ	
					list <_XQUEST_QUESTLIST>::iterator iter_npctargetting;
					for(iter_npctargetting = g_QuestScriptManager.m_listNPCTargettingQuest.begin() ; iter_npctargetting != g_QuestScriptManager.m_listNPCTargettingQuest.end() ; )
					{
						_XQUEST_QUESTLIST npctargetting = *iter_npctargetting;
						if( npctargetting.questid == m_nOwnerQuestID )
						{
							iter_npctargetting = g_QuestScriptManager.m_listNPCTargettingQuest.erase(iter_npctargetting);
						}
						else
						{
							iter_npctargetting++;
						}
					}
//					g_QuestScriptManager.m_listNPCTargettingQuest.clear();
					g_QuestScriptManager.m_indexSelectNPCTargettingQuest = -1;
					m_bHoldCheckingOfNPCTargetting = FALSE;					
					g_LastNPCScriptOwner->m_bStartQuestDialog = TRUE;					
					_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
					if( pNPCScript_Window )
					{
						pNPCScript_Window->m_btnNPCDialogModExit->ShowWindow(FALSE);
					}


					if( m_bStartCondition )
					{
						g_pLocalUser->m_CharacterLog.isTalk = FALSE;
						g_pLocalUser->m_CharacterLog.talkTime = 0;
						
						if( g_QuestScriptManager.m_listNPCTargettingQuest.empty() )
						{
							_XNPCObject* npcobject = NULL;
							if( g_pLocalUser->m_AttackTarget )
							{			
								npcobject = (_XNPCObject *)g_pLocalUser->m_AttackTarget;
							}
							else if( g_pLocalUser->m_ViewTarget )
							{
								npcobject = (_XNPCObject *)g_pLocalUser->m_ViewTarget;
							}
							
							
						//	if(npcobject)
						//	{
						//		npcobject->m_HaveQuestType = _XNPC_HAVEQUEST_NONE;	
						//	}			
						}		
						g_pLocalUser->m_AttackTargetObjectType = _XPICKINGTARGET_NONE;
						
						currentquest->SetLastTarggetingNPCID( m_TDC.m_numericvalue1 );
						
#ifdef _XDWDEBUG	
						_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CONDITION RESULT)     >>>>>>>>>>>>>>>>>>>>" );
						_XLog("Quest Condition Targetting Success : QuestID[%d] SucceessNode[%d] NPCID[%d]", m_nOwnerQuestID,currentquest->GetCurrentTreeNumber(), m_TDC.m_numericvalue1 );
						
#endif
						return QUEST_CONDITION_RESULT_SUCCESS;
					}
					else 
					{
						
						int starttime = currentquest->GetStartTime();
						
						BOOL bCheck;
						if(m_byOwnerTriggerClass == ID_TRIGGER_CLASS_STARTCONDITION)
						{
							bCheck = TRUE;
						}
						else
						{
							if(starttime != 0)
								bCheck = TRUE;
						}
						
						if(g_pLocalUser->m_CharacterLog.talkTime > starttime && bCheck)
						{
							g_pLocalUser->m_CharacterLog.isTalk = FALSE;
							g_pLocalUser->m_CharacterLog.talkTime = 0;
							
							if( g_QuestScriptManager.m_listNPCTargettingQuest.empty() )
							{
								_XNPCObject* npcobject = NULL;
								if( g_pLocalUser->m_AttackTarget )
								{			
									npcobject = (_XNPCObject *)g_pLocalUser->m_AttackTarget;
								}
								else if( g_pLocalUser->m_ViewTarget )
								{
									npcobject = (_XNPCObject *)g_pLocalUser->m_ViewTarget;
								}
								
								
							//	if(npcobject)
							//	{
							//		npcobject->m_HaveQuestType = _XNPC_HAVEQUEST_NONE;	
							//	}									
							}
							g_pLocalUser->m_AttackTargetObjectType = _XPICKINGTARGET_NONE;
							
							
							// ������ Ÿ������ NPC�� �����ϱ� ���� �ٸ� NPC�� �ʱ�ȭ 
							for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; ++i)
							{
								if( currentquest->GetLastTarggetingNPCID() == g_Npclist.m_pNPCObjectArray[i].m_UniqueID )
								{
									g_Npclist.m_pNPCObjectArray[i].SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
									
#ifdef _XDWDEBUG	
									_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CHECKING)     >>>>>>>>>>>>>>>>>>>>" );					
									_XLog("Setting Normal NPCDialogMode : QuestID[%d] NPCID[%d]" , m_nOwnerQuestID,  g_Npclist.m_pNPCObjectArray[i].m_UniqueID );
									
#endif
									break;
								}
							}
							currentquest->SetLastTarggetingNPCID( m_TDC.m_numericvalue1 );
							
#ifdef _XDWDEBUG	
							_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CONDITION RESULT)     >>>>>>>>>>>>>>>>>>>>" );
							_XLog("Quest Condition Targetting Success : QuestID[%d] SucceessNode[%d] NPCID[%d]", m_nOwnerQuestID,currentquest->GetCurrentTreeNumber(), m_TDC.m_numericvalue1 );
							
#endif
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
						else
							return QUEST_CONDITION_RESULT_FAIL;
						
					}				
				}
			}
			else
			{
			//	m_bHoldCheckingOfNPCTargetting = FALSE;
				return QUEST_CONDITION_RESULT_FAIL;
			}
		}
		return QUEST_CONDITION_RESULT_FAIL;
	}
	return QUEST_CONDITION_RESULT_FAIL;
#else
		
		_XNPCObject* npcobject = NULL;
		if( g_pLocalUser->m_AttackTarget )
		{			
			npcobject = (_XNPCObject *)g_pLocalUser->m_AttackTarget;
		}
		else if( g_pLocalUser->m_ViewTarget )
		{
			npcobject = (_XNPCObject *)g_pLocalUser->m_ViewTarget;
		}
		

		if(npcobject)
		{
			if(npcobject->m_UniqueID == m_TDC.m_numericvalue1)
			{
				if( m_bStartCondition )
				{
					g_pLocalUser->m_CharacterLog.isTalk = FALSE;
					g_pLocalUser->m_CharacterLog.talkTime = 0;
					
					npcobject->m_HaveQuestType = _XNPC_HAVEQUEST_NONE;						
					g_pLocalUser->m_AttackTargetObjectType = _XPICKINGTARGET_NONE;
					
					currentquest->SetLastTarggetingNPCID( m_TDC.m_numericvalue1 );
					
#ifdef _XDWDEBUG	
					_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CONDITION RESULT)     >>>>>>>>>>>>>>>>>>>>" );
					_XLog("Quest Condition Targetting Success : QuestID[%d] SucceessNode[%d] NPCID[%d]", m_nOwnerQuestID,currentquest->GetCurrentTreeNumber(), m_TDC.m_numericvalue1 );
					
#endif
					return QUEST_CONDITION_RESULT_SUCCESS;
				}
				else 
				{
					
					int starttime = currentquest->GetStartTime();
					
					BOOL bCheck;
					if(m_byOwnerTriggerClass == ID_TRIGGER_CLASS_STARTCONDITION)
					{
						bCheck = TRUE;
					}
					else
					{
						if(starttime != 0)
							bCheck = TRUE;
					}

					if(g_pLocalUser->m_CharacterLog.talkTime > starttime && bCheck)
					{
						g_pLocalUser->m_CharacterLog.isTalk = FALSE;
						g_pLocalUser->m_CharacterLog.talkTime = 0;

						npcobject->m_HaveQuestType = _XNPC_HAVEQUEST_NONE;				
						g_pLocalUser->m_AttackTargetObjectType = _XPICKINGTARGET_NONE;
						
						
						// ������ Ÿ������ NPC�� �����ϱ� ���� �ٸ� NPC�� �ʱ�ȭ 
						for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; ++i)
						{
							if( currentquest->GetLastTarggetingNPCID() == g_Npclist.m_pNPCObjectArray[i].m_UniqueID )
							{
								g_Npclist.m_pNPCObjectArray[i].SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
								
#ifdef _XDWDEBUG	
								_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CHECKING)     >>>>>>>>>>>>>>>>>>>>" );					
								_XLog("Setting Normal NPCDialogMode : QuestID[%d] NPCID[%d]" , m_nOwnerQuestID,  g_Npclist.m_pNPCObjectArray[i].m_UniqueID );
								
#endif
								break;
							}
						}
						currentquest->SetLastTarggetingNPCID( m_TDC.m_numericvalue1 );
						
#ifdef _XDWDEBUG	
						_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CONDITION RESULT)     >>>>>>>>>>>>>>>>>>>>" );
						_XLog("Quest Condition Targetting Success : QuestID[%d] SucceessNode[%d] NPCID[%d]", m_nOwnerQuestID,currentquest->GetCurrentTreeNumber(), m_TDC.m_numericvalue1 );
						
#endif
						return QUEST_CONDITION_RESULT_SUCCESS;
					}
					else
						return QUEST_CONDITION_RESULT_FAIL;
					

				}
			}
		}
	}
	return QUEST_CONDITION_RESULT_FAIL;	
#endif

}

// targetting - dynamic npc
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckTargettingDynamicNPC(void)
{
	// m_compareclass1 : Target class
	// m_compareclass2 : area category
	// m_numericvalue1 : Target ID / Area
	// m_numericvalue2 : Map Index

	return QUEST_CONDITION_RESULT_FAIL;
}

// targetting - dynamic npc
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckTargettingObject(void)
{
	// m_compareclass1 : Target class
	// m_compareclass2 : area category
	// m_numericvalue2 : Map Index
	// m_numericvalue1 : Target ID / Area
	
	if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
	{
		
		if( !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_bFunctionalObjectWideView )
			return QUEST_CONDITION_RESULT_FAIL;		
		
		if( g_CurrentZoneInfoIndex == m_TDC.m_numericvalue2 )
		{
			if( g_LastFunctionalObject->m_UniqueID == m_TDC.m_numericvalue1 )
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
	}	
	return QUEST_CONDITION_RESULT_FAIL;
}

// Ÿ�̸� üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckTimer(void)
{
	// m_compareclass1 : Compare class //�̸�, �ʰ�, ����
	// m_compareclass2 : Trigger ID
	// m_compareclass3 : //�����˻��ϱ�, �����˻���ϱ�
	// m_numericvalue1 : Minutes
	// m_numericvalue2 : Seconds
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;


	DWORD starttime = 0;
	if(m_TDC.m_compareclass2 == 0)
	{
		starttime = currentquest->GetStartTime();
		if(starttime == 0)
		{
			return QUEST_CONDITION_RESULT_FAIL;
		}
	}
	else
	{
		// trigger timer���� start time ��������
		starttime = currentquest->m_mapTriggerTimer[m_TDC.m_compareclass2];
		if(starttime == 0)
		{
			return QUEST_CONDITION_RESULT_FAIL;
		}
	}

	DWORD checktime = ((60*m_TDC.m_numericvalue1) + m_TDC.m_numericvalue2)*1000;
	DWORD elapsedtime = timeGetTime() - starttime;//g_ServerTimeCode - starttime;
	g_QuestScriptManager.m_QuestTimerTimeLimit = checktime/1000;
 
	switch(m_TDC.m_compareclass1)
	{
	case ID_TRIGGER_COMPARE_THELEAST ://�̸�
		{
			if(elapsedtime < checktime-(checktime*0.01))//���� �ð����� ������ ������ ó�� 0.8�� ���̸� ��
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_TRIGGER_COMPARE_THEMOST ://�ʰ�
		{
			if(elapsedtime > checktime+(checktime*0.01))//���� �ð����� ������ ������ ó�� 0.8�� ���̸� ��
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_TRIGGER_COMPARE_EXACTLY ://����
		{
			if(elapsedtime == checktime)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	}

	return QUEST_CONDITION_RESULT_FAIL;
}

// ��� �ð� üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckPassTime(void)
{
	// m_compareclass1 : Constraints(����)
	// m_compareclass2 : Compare class
	// m_numericvalue1 : Minutes
	// m_numericvalue2 : Seconds

	int starttime = 0;
	switch(m_TDC.m_compareclass1)
	{
	case ID_QUEST_CHECKTIME_DAWN :
		{
			starttime = ID_QUEST_CHECKTIME_DAWNTIME * 1000;
		}
		break;
	case ID_QUEST_CHECKTIME_DAY :
		{
			starttime = ID_QUEST_CHECKTIME_DAYTIME * 1000;
		}
		break;
	case ID_QUEST_CHECKTIME_NIGHTFALL :
		{
			starttime = ID_QUEST_CHECKTIME_NIGHTFALLTIME * 1000;
		}
		break;
	case ID_QUEST_CHECKTIME_NIGHT :
		{
			starttime = ID_QUEST_CHECKTIME_NIGHTTIME * 1000;
		}
		break;
	}

	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		{
			if(g_EnvironmentManager.m_EnvironmentTime < (starttime + m_TDC.m_numericvalue1*60*1000 + m_TDC.m_numericvalue2*1000))
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_TRIGGER_COMPARE_THEMOST :
		{
			if(g_EnvironmentManager.m_EnvironmentTime > (starttime + m_TDC.m_numericvalue1*60*1000 + m_TDC.m_numericvalue2*1000))
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_TRIGGER_COMPARE_EXACTLY :
		{
			if(g_EnvironmentManager.m_EnvironmentTime == (starttime + m_TDC.m_numericvalue1*60*1000 + m_TDC.m_numericvalue2*1000))
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� Ʈ���� üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckProcedingTrigger(void)
{
	// m_compareclass1 : Trigger ID
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;

	if(currentquest->GetTriggerComplete(m_TDC.m_compareclass1))
		return QUEST_CONDITION_RESULT_SUCCESS;
	else
		return QUEST_CONDITION_RESULT_FAIL;
}

// �׼� ID üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckActionID(void)
{
	// ��� ����
	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� ����Ʈ üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckProcedingQuest(void)
{
	// m_compareclass1 : result
	// m_numericvalue1 : Quest ID
	
	if( m_TDC.m_numericvalue1 >= 10000 && m_TDC.m_numericvalue1 < 20000 )
	{
		bool questsuccess = false;
		if( 11000 <= m_TDC.m_numericvalue1 && 12000 > m_TDC.m_numericvalue1 ) // ����
		{
			if( 11000 <= m_TDC.m_numericvalue1 && g_QuestScriptManager.m_CurrentMainQuestID > m_TDC.m_numericvalue1 )
			{
				// ���� ����Ʈ ����
				questsuccess = true;
			}
		}
		else if( 12000 <= m_TDC.m_numericvalue1 && 13000 > m_TDC.m_numericvalue1 ) // ���
		{
			if( 12000 <= m_TDC.m_numericvalue1 && g_QuestScriptManager.m_CurrentMainQuestID > m_TDC.m_numericvalue1 )
			{
				// ���� ����Ʈ ����
				questsuccess = true;
			}
		}
		else if( 13000 <= m_TDC.m_numericvalue1 && 14000 > m_TDC.m_numericvalue1 ) // �Ҹ�
		{
			if( 13000 <= m_TDC.m_numericvalue1 && g_QuestScriptManager.m_CurrentMainQuestID > m_TDC.m_numericvalue1 )
			{
				// ���� ����Ʈ ����
				questsuccess = true;
			}
		}
		else if( 14000 <= m_TDC.m_numericvalue1 && 15000 > m_TDC.m_numericvalue1 ) // �츲
		{
			if( 14000 <= m_TDC.m_numericvalue1 && g_QuestScriptManager.m_CurrentMainQuestID > m_TDC.m_numericvalue1 )
			{
				// ���� ����Ʈ ����
				questsuccess = true;
			}
		}
		else if( 15000 <= m_TDC.m_numericvalue1 && 16000 > m_TDC.m_numericvalue1 ) // ����
		{
			if( 15000 <= m_TDC.m_numericvalue1 && g_QuestScriptManager.m_CurrentMainQuestID > m_TDC.m_numericvalue1 )
			{
				// ���� ����Ʈ ����
				questsuccess = true;
			}
		}
		else if( 16000 <= m_TDC.m_numericvalue1 && 17000 > m_TDC.m_numericvalue1 ) // ����
		{
			if( 16000 <= m_TDC.m_numericvalue1 && g_QuestScriptManager.m_CurrentMainQuestID > m_TDC.m_numericvalue1 )
			{
				// ���� ����Ʈ ����
				questsuccess = true;
			}
		}

		
		switch(m_TDC.m_compareclass1)
		{
		case ID_QUEST_RESULT_SUCCESS :
			{
				if( questsuccess )
					return QUEST_CONDITION_RESULT_SUCCESS;
			}
			break;
			
		case ID_QUEST_RESULT_FAIL :
			{
				if( questsuccess )
					return QUEST_CONDITION_RESULT_SUCCESS;
			}
			break;
		}
	} 
	else
	{
		_XQUEST_COMPLETE_RESULT result = g_QuestScriptManager.FindQuestCompleteResult(m_TDC.m_numericvalue1);
		
		switch(m_TDC.m_compareclass1)
		{
		case ID_QUEST_RESULT_SUCCESS :
			{
				if(result == QUEST_COMPLETE_RESULT_SOLVE)
					return QUEST_CONDITION_RESULT_SUCCESS;
			}
			break;
			
		case ID_QUEST_RESULT_FAIL :
			{
				if(result == QUEST_COMPLETE_RESULT_MISS)
					return QUEST_CONDITION_RESULT_SUCCESS;
			}
			break;
		}
	
	}

	return QUEST_CONDITION_RESULT_FAIL;
}

// Ư�� ����Ʈ ���� üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckIsProgressQuest(void)
{
	//scriptdata->m_TDC.m_numericvalue1 = m_QuestID;
	//scriptdata->m_TDC.m_numericvalue2 = m_NodeID;	// m_NodeID�� 0�϶��� ����Ʈ ���� üũ

	if( g_QuestScriptManager.m_CurrentMainQuestID == m_TDC.m_numericvalue1 )
	{
		if( m_TDC.m_numericvalue2 <= 0 )
		{
			if( g_QuestScriptManager.m_bProgressMainQuest ) 
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
		{
			_XQuestScriptItem* currentquest = NULL;
			map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_TDC.m_numericvalue1);
			if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			{
				currentquest = iter_quest->second;
				if( currentquest )
				{
					if( m_TDC.m_numericvalue2 <= currentquest->GetCurrentTreeNumber() )
					{
						return QUEST_CONDITION_RESULT_SUCCESS;
					}
					else
					{
						return QUEST_CONDITION_RESULT_FAIL;
					}
				}
			}

		}
	}
	else
	{
		for(int i = 0; i < ID_QUEST_SYNC-1; ++i)
		{
			if( g_QuestScriptManager.m_ProgressSubQuestID[i] == m_TDC.m_numericvalue1 )
			{
				if( m_TDC.m_numericvalue2 <= 0 )
				{
					return QUEST_CONDITION_RESULT_SUCCESS;
				}
				else
				{
					_XQuestScriptItem* currentquest = NULL;
					map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_TDC.m_numericvalue1);
					if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
					{
						currentquest = iter_quest->second;
						if( currentquest )
						{
							if( m_TDC.m_numericvalue2 == currentquest->GetCurrentTreeNumber() )
							{
								return QUEST_CONDITION_RESULT_SUCCESS;
							}
							else
							{
								return QUEST_CONDITION_RESULT_FAIL;
							}
						}
					}
				}
			}
		}
		
		// �������� �������� �ʴ� ����Ʈ ����Ʈ�� �˻��Ѵ�.
		if( !g_QuestScriptManager.m_listQuestNoSaveType.empty() )
		{			
			list <int>::iterator iter_questnosavetype;
			for(iter_questnosavetype = g_QuestScriptManager.m_listQuestNoSaveType.begin() ; iter_questnosavetype != g_QuestScriptManager.m_listQuestNoSaveType.end() ; ++iter_questnosavetype)
			{
				int nosavequestid = *iter_questnosavetype;	
				if( nosavequestid == m_TDC.m_numericvalue1 )
				{
					if( m_TDC.m_numericvalue2 <= 0 )
					{
						return QUEST_CONDITION_RESULT_SUCCESS;
					}
					else
					{
						_XQuestScriptItem* currentquest = NULL;
						map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_TDC.m_numericvalue1);
						if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
						{
							currentquest = iter_quest->second;
							if( currentquest )
							{
								if( m_TDC.m_numericvalue2 == currentquest->GetCurrentTreeNumber() )
								{
									return QUEST_CONDITION_RESULT_SUCCESS;
								}
								else
								{
									return QUEST_CONDITION_RESULT_FAIL;
								}
							}
						}
				}
				}
			}
		}
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// ���� ����Ʈ ���� üũ
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckProgressQuest(void)
{
	// m_compareclass1 : Compare class
	// m_numericvalue1 : Quest count
	switch(m_TDC.m_compareclass1)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		{
			if(g_QuestScriptManager.GetProgressQuestCount() < m_TDC.m_numericvalue1)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		{
			if(g_QuestScriptManager.GetProgressQuestCount() > m_TDC.m_numericvalue1)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		{
			if(g_QuestScriptManager.GetProgressQuestCount() == m_TDC.m_numericvalue1)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}


_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckSelection(void)
{
	// m_numericvalue1 : m_SelectionNumber
	// ������ ������ �������� ó���ϰ� ���� ��带 �޾ƾ� �Ѵ�.
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;
	
	if( currentquest->m_SelectionNumber == m_TDC.m_numericvalue1 )
	{		
#ifdef _XDWDEBUG
		_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT)     >>>>>>>>>>>>>>>>>>>>" );
		_XLog("Success Check Selection Number Condition: QuestID[%d] QuestNodeID[%d] SelectionNumber[%d]", m_nOwnerQuestID, currentquest->GetCurrentTreeNumber(), currentquest->m_SelectionNumber );
#endif
		return QUEST_CONDITION_RESULT_SUCCESS;
	}

	return QUEST_CONDITION_RESULT_FAIL;
}


_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckSelectionEnd(void)
{
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;
	
	if( currentquest->m_bSelectionNumberCheck )
	{				
#ifdef _XDWDEBUG	
		_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT)     >>>>>>>>>>>>>>>>>>>>" );
		_XLog("Success Check SelectionEnd Condition: QuestID[%d] QuestNodeID[%d] ", m_nOwnerQuestID, currentquest->GetCurrentTreeNumber() );
#endif
		
		return QUEST_CONDITION_RESULT_SUCCESS;
	}
	
	return QUEST_CONDITION_RESULT_FAIL;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckNPCDialogMode(void)
{	
	if( !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_bNPCDialogWideView && !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_bNPCDialogMode)
		return QUEST_CONDITION_RESULT_SUCCESS;
	
	return QUEST_CONDITION_RESULT_FAIL;
}


_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckWearItem(void)
{
	// m_compareclass1 = m_ItemClass; 
	// m_compareclass2 = m_ItemCheckType; 0:���� üũ 1:���üũ 2:���� 3:������
	// m_numericvalue1 : m_ItemID
	// m_numericvalue2 : m_InventroyNumber

	if( m_TDC.m_compareclass2 == 0 ) //���� üũ
	{
		if(  g_pLocalUser->m_UserItemList[ m_TDC.m_numericvalue2 ].Get_m_cType() == m_TDC.m_compareclass1 
			&& g_pLocalUser->m_UserItemList[ m_TDC.m_numericvalue2 ].Get_m_sID() == m_TDC.m_numericvalue1 )
		{
			if( m_bStartCondition )
			{
				// ���������϶� ������ üũ�� ������ ������ ������ ������ �ִ´�. 
				// �� �������� ������� ���������� �ٽ� �˻��ؾ� �Ѵ�.
				if( !g_QuestScriptManager.m_listCheckWearItem.empty() )
				{
					bool balreadyexist = false;
					list <_XQUEST_ITEMINFO>::iterator iter_checkwearitem;
					for(iter_checkwearitem = g_QuestScriptManager.m_listCheckWearItem.begin() ; iter_checkwearitem != g_QuestScriptManager.m_listCheckWearItem.end() ;++iter_checkwearitem )
					{
						_XQUEST_ITEMINFO checkwearitem = *iter_checkwearitem;
						if( checkwearitem.questid == m_nOwnerQuestID 
							&& checkwearitem.itemtype == m_TDC.m_compareclass1 
							&& checkwearitem.itemid	== m_TDC.m_numericvalue1	)
						{
							balreadyexist = true;
							break;
						}
					}
					if( !balreadyexist )
					{
						_XQUEST_ITEMINFO checkwearitem;
						memset(&checkwearitem, 0, sizeof(_XQUEST_ITEMINFO));
						checkwearitem.questid	= m_nOwnerQuestID;
						checkwearitem.itemtype	= m_TDC.m_compareclass1;
						checkwearitem.itemid	= m_TDC.m_numericvalue1;
						g_QuestScriptManager.m_listCheckWearItem.push_back(checkwearitem);
					}
				}
				else
				{
					_XQUEST_ITEMINFO checkwearitem;
					memset(&checkwearitem, 0, sizeof(_XQUEST_ITEMINFO));
					checkwearitem.questid	= m_nOwnerQuestID;
					checkwearitem.itemtype	= m_TDC.m_compareclass1;
					checkwearitem.itemid	= m_TDC.m_numericvalue1;
					g_QuestScriptManager.m_listCheckWearItem.push_back(checkwearitem);
				}
			}

			return QUEST_CONDITION_RESULT_SUCCESS;
		}
	}
	else if( m_TDC.m_compareclass2 == 1 )// ������ ���üũ : ȸ���� ������...etc
	{
		if(g_pState_Window)
		{
			if(g_pState_Window->FindUsingItemFromID(_XST_ITEM, m_TDC.m_numericvalue1, m_TDC.m_compareclass1))
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
	}	
	else if( m_TDC.m_compareclass2 == 2 )// ����
	{
		if( g_pLocalUser->m_UserItemList[ m_TDC.m_numericvalue2 ].Get_m_sID() > 0 )
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		
	}	
	else if( m_TDC.m_compareclass2 == 3 )// ������
	{
		if( g_pLocalUser->m_UserItemList[ m_TDC.m_numericvalue2 ].Get_m_sID() <= 0 )
		{	
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		
	}	
	return QUEST_CONDITION_RESULT_FAIL;

}


_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessSetQuestMarkArea(void)
{
	// m_compareclass1 : m_AreaCategory
	// m_numericvalue1 : m_AreaID
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;
	
	
	if( m_TDC.m_compareclass1 != -1 )
	{
		_XArea* pArea = g_AreaManager.GetArea( m_TDC.m_compareclass1, m_TDC.m_numericvalue1 );
		if( pArea )
		{
			if( pArea->m_ShapeType == _XDEF_AREASHAPETYPE_BOX )
			{				
				bool isquestmark = false;
				list <_XQUEST_MARKINFO>::iterator iter_questmark;
				for(iter_questmark = g_QuestScriptManager.m_listQuestMark.begin() ; iter_questmark != g_QuestScriptManager.m_listQuestMark.end() ; ++iter_questmark)
				{
					_XQUEST_MARKINFO questmark = *iter_questmark;
					if( questmark.questid == currentquest->GetQuestID() && questmark.npcid == -1 && questmark.questtype == m_QuestType )
					{
						isquestmark = true;
						break;
					}
				}
				if( !isquestmark )
				{
					_XQUEST_MARKINFO questmark;
					memset(&questmark, 0, sizeof(_XQUEST_MARKINFO));
					questmark.MarkPosition_X = ( pArea->m_Point[0].x + pArea->m_Point[2].x )/2;
					questmark.MarkPosition_Y = ( pArea->m_Point[0].z + pArea->m_Point[2].z )/2;
					questmark.questid = currentquest->GetQuestID();
					questmark.npcid = -1;
					questmark.questtype = m_QuestType;
					g_QuestScriptManager.m_listQuestMark.push_back(questmark);
					
					//��ȣǳ��� ���� �޼��� ȭ�鿡 �����ֱ� 
					switch(m_QuestType) {
					case _XQUEST_TYPE_1:
						{
							if( m_bStartCondition )
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1771), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
							else
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1772), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
						}
						break;
					case _XQUEST_TYPE_2:
						{
							if( m_bStartCondition )
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1773), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
							else
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1774), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
						}
						break;
					case _XQUEST_TYPE_3:
						{
							if( m_bStartCondition )
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1775), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
							else
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1776), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
						}
						break;
					case _XQUEST_TYPE_4:
						{
							if( m_bStartCondition )
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1777), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
							else
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1778), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
						}
						break;
					}
				}
			}
			else
			{
				bool isquestmark = false;
				list <_XQUEST_MARKINFO>::iterator iter_questmark;
				for(iter_questmark = g_QuestScriptManager.m_listQuestMark.begin() ; iter_questmark != g_QuestScriptManager.m_listQuestMark.end() ; ++iter_questmark)
				{
					_XQUEST_MARKINFO questmark = *iter_questmark;
					if( questmark.questid == currentquest->GetQuestID() && questmark.npcid == -1 && questmark.questtype == m_QuestType )
					{
						isquestmark = true;
						break;
					}
				}
				if( !isquestmark )
				{
					_XQUEST_MARKINFO questmark;
					memset(&questmark, 0, sizeof(_XQUEST_MARKINFO));
					questmark.MarkPosition_X = pArea->m_CenterPoint.x;
					questmark.MarkPosition_Y = pArea->m_CenterPoint.z;
					questmark.questid = currentquest->GetQuestID();
					questmark.npcid = -1;
					questmark.questtype = m_QuestType;
					g_QuestScriptManager.m_listQuestMark.push_back(questmark);

					//��ȣǳ��� ���� �޼��� ȭ�鿡 �����ֱ� 
					switch(m_QuestType) {
					case _XQUEST_TYPE_1:
						{
							if( m_bStartCondition )
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1771), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
							else
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1772), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
						}
						break;
					case _XQUEST_TYPE_2:
						{
							if( m_bStartCondition )
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1773), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
							else
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1774), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
						}
						break;
					case _XQUEST_TYPE_3:
						{
							if( m_bStartCondition )
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1775), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
							else
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1776), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
						}
						break;
					case _XQUEST_TYPE_4:
						{
							if( m_bStartCondition )
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1777), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
							else
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1778), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
							}
						}
						break;
					}
				}
			}
		}							
	}

	return QUEST_CONDITION_RESULT_SUCCESS;
}


_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessSetQuestMarkNPC(void)
{
	// m_compareclass1 : ��ũ Ÿ�� 
	// m_numericvalue1 : m_NPCID
	
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
		
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;
	
	// �Ӹ����� ! ����ֱ�
	for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; ++i)
	{
		_XNPCObject* pNPC = &g_Npclist.m_pNPCObjectArray[i];
		if(pNPC)
		{
			if(pNPC->m_UniqueID == m_TDC.m_numericvalue1)
			{
				switch(m_TDC.m_compareclass1) {
				case ID_TRIGGER_MARKTYPE_WORLD:
					{			
						bool isquestmark = false;
						list <_XQUEST_MARKINFO>::iterator iter_questmark;
						for(iter_questmark = g_QuestScriptManager.m_listQuestMark.begin() ; iter_questmark != g_QuestScriptManager.m_listQuestMark.end() ; ++iter_questmark)
						{
							_XQUEST_MARKINFO questmark = *iter_questmark;
							if( questmark.questid == currentquest->GetQuestID() && questmark.npcid == m_TDC.m_numericvalue1 && questmark.questtype == m_QuestType )
							{
								isquestmark = true;
								break;
							}
						}
						
						if( !isquestmark )
						{
							_XQUEST_MARKINFO questmark;
							memset(&questmark, 0, sizeof(_XQUEST_MARKINFO));
							questmark.MarkPosition_X = -9999;
							questmark.MarkPosition_Y = -9999;
							questmark.questid = currentquest->GetQuestID();
							questmark.npcid = m_TDC.m_numericvalue1;
							questmark.questtype = m_QuestType;
							g_QuestScriptManager.m_listQuestMark.push_back(questmark);
							
						}

						if(currentquest->GetMainQuest())
						{
							pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_YELLOW;		
						}
						else 
						{						
							switch( m_QuestType ) {
							case _XQUEST_TYPE_2://������ 
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_BLUE;
								}
								break;
							case _XQUEST_TYPE_3://������
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_ORANGE;
								}
								break;
							case _XQUEST_TYPE_4://�⿬�� 
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_GREEN;	
								}
								break;
							case _XQUEST_TYPE_5://�����һ�
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_WHITE;	
								}
								break;
							}
						}		
					}
					break;
				case ID_TRIGGER_MARKTYPE_MAP:
					{	
						bool isquestmark = false;
						list <_XQUEST_MARKINFO>::iterator iter_questmark;
						for(iter_questmark = g_QuestScriptManager.m_listQuestMark.begin() ; iter_questmark != g_QuestScriptManager.m_listQuestMark.end() ; ++iter_questmark)
						{
							_XQUEST_MARKINFO questmark = *iter_questmark;
							if( questmark.questid == currentquest->GetQuestID() && questmark.npcid == m_TDC.m_numericvalue1 && questmark.questtype == m_QuestType )
							{
								isquestmark = true;
								break;
							}
						}
						
						if( !isquestmark )
						{
							_XQUEST_MARKINFO questmark;
							memset(&questmark, 0, sizeof(_XQUEST_MARKINFO));
							questmark.MarkPosition_X = pNPC->m_matWorldPosition._41;
							questmark.MarkPosition_Y = pNPC->m_matWorldPosition._43;
							questmark.questid = currentquest->GetQuestID();
							questmark.npcid = m_TDC.m_numericvalue1;
							questmark.questtype = m_QuestType;
							g_QuestScriptManager.m_listQuestMark.push_back(questmark);
							
							
							//��ȣǳ��� ���� �޼��� ȭ�鿡 �����ֱ� 
							switch(m_QuestType) {
							case _XQUEST_TYPE_1:
								{
									if( m_bStartCondition )
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1771), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
									else
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1772), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
								}
								break;
							case _XQUEST_TYPE_2:
								{
									if( m_bStartCondition )
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1773), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
									else
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1774), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
								}
								break;
							case _XQUEST_TYPE_3:
								{
									if( m_bStartCondition )
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1775), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
									else
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1776), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
								}
								break;
							case _XQUEST_TYPE_4:
								{
									if( m_bStartCondition )
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1777), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
									else
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1778), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
								}
								break;
							}
						}			
					}
					break;
				case ID_TRIGGER_MARKTYPE_WORLDANDMAP:
					{
						bool isquestmark = false;
						list <_XQUEST_MARKINFO>::iterator iter_questmark;
						for(iter_questmark = g_QuestScriptManager.m_listQuestMark.begin() ; iter_questmark != g_QuestScriptManager.m_listQuestMark.end() ; ++iter_questmark)
						{
							_XQUEST_MARKINFO questmark = *iter_questmark;
							if( questmark.questid == currentquest->GetQuestID() && questmark.npcid == m_TDC.m_numericvalue1 && questmark.questtype == m_QuestType )
							{
								isquestmark = true;
								break;
							}
						}
						if( !isquestmark )
						{
							_XQUEST_MARKINFO questmark;
							memset(&questmark, 0, sizeof(_XQUEST_MARKINFO));
							questmark.MarkPosition_X = pNPC->m_matWorldPosition._41;
							questmark.MarkPosition_Y = pNPC->m_matWorldPosition._43;
							questmark.questid = currentquest->GetQuestID();
							questmark.npcid = m_TDC.m_numericvalue1;
							questmark.questtype = m_QuestType;
							g_QuestScriptManager.m_listQuestMark.push_back(questmark);
							
							//��ȣǳ��� ���� �޼��� ȭ�鿡 �����ֱ� 
							switch(m_QuestType) {
							case _XQUEST_TYPE_1:
								{
									if( m_bStartCondition )
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1771), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
									else
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1772), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
								}
								break;
							case _XQUEST_TYPE_2:
								{
									if( m_bStartCondition )
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1773), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
									else
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1774), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
								}
								break;
							case _XQUEST_TYPE_3:
								{
									if( m_bStartCondition )
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1775), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
									else
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1776), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
								}
								break;
							case _XQUEST_TYPE_4:
								{
									if( m_bStartCondition )
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1777), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
									else
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1778), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
									}
								}
								break;
							}
						}
						
						if(currentquest->GetMainQuest())
						{
							pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_YELLOW;
						}
						else
						{							
							switch( m_QuestType ) {
							case _XQUEST_TYPE_2://������ 
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_BLUE;
								}
								break;
							case _XQUEST_TYPE_3://������
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_ORANGE;

								}
								break;
							case _XQUEST_TYPE_4://�⿬�� 
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_GREEN;	
								}
								break;
							case _XQUEST_TYPE_5://�����һ�
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_WHITE;	
								}
								break;
							}							
						}
					}
					break;
				}				
				break;
			}// if(pNPC->m_UniqueID == m_TDC.m_numericvalue1)
		}
	}

	map <int, _XTriggerItem*>::iterator iter_trigger;
	_XTriggerItem* currenttrigger = NULL;

	iter_trigger = currentquest->m_mapSuccessTriggerList.find(m_nOwnerTriggerID);
	if(iter_trigger != currentquest->m_mapSuccessTriggerList.end())
	{
		currenttrigger = iter_trigger->second;
		if(currenttrigger)
		{
			currenttrigger->SetOwnerNPCID(m_TDC.m_numericvalue1);
			//currentquest->SetLastTarggetingNPCID(m_TDC.m_numericvalue1);
		}
	}

	
	return QUEST_CONDITION_RESULT_SUCCESS;
}


_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessSetHintMarkArea(void)
{
	// m_compareclass1 : m_AreaCategory
	// m_numericvalue1 : m_AreaID
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;
	
	
	if( m_TDC.m_compareclass1 != -1 )
	{
		_XArea* pArea = g_AreaManager.GetArea( m_TDC.m_compareclass1, m_TDC.m_numericvalue1 );
		if( pArea )
		{
			if( pArea->m_ShapeType == _XDEF_AREASHAPETYPE_BOX )
			{				
				bool ishintmark = false;
				list <_XQUEST_MARKINFO>::iterator iter_hintmark;
				for(iter_hintmark = g_QuestScriptManager.m_listHintMark.begin() ; iter_hintmark != g_QuestScriptManager.m_listHintMark.end() ; ++iter_hintmark)
				{
					_XQUEST_MARKINFO questhint = *iter_hintmark;
					if( questhint.questid == currentquest->GetQuestID() && questhint.npcid == -1 && questhint.questtype == m_QuestType )
					{
						ishintmark = true;
						break;
					}
				}

				if( !ishintmark )
				{
					_XQUEST_MARKINFO hintmark;
					memset(&hintmark, 0, sizeof(_XQUEST_MARKINFO));
					hintmark.MarkPosition_X = ( pArea->m_Point[0].x + pArea->m_Point[2].x )/2;
					hintmark.MarkPosition_Y = ( pArea->m_Point[0].z + pArea->m_Point[2].z )/2;
					hintmark.questid = currentquest->GetQuestID();
					hintmark.npcid = -1;
					hintmark.questtype = m_QuestType;
					g_QuestScriptManager.m_listHintMark.push_back(hintmark);
					
					//��ȣǳ��� ���� �޼��� ȭ�鿡 �����ֱ� 
					switch(m_QuestType) {
					case _XQUEST_TYPE_1:
						{
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1779), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
						}
						break;
					case _XQUEST_TYPE_2:
						{
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1780), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
						}
						break;
					case _XQUEST_TYPE_3:
						{
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1781), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
						}
						break;
					case _XQUEST_TYPE_4:
						{
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1782), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
						}
						break;
					}
				}
				
			}
			else
			{
				bool ishintmark = false;
				list <_XQUEST_MARKINFO>::iterator iter_hintmark;
				for(iter_hintmark = g_QuestScriptManager.m_listHintMark.begin() ; iter_hintmark != g_QuestScriptManager.m_listHintMark.end() ; ++iter_hintmark)
				{
					_XQUEST_MARKINFO questhint = *iter_hintmark;
					if( questhint.questid == currentquest->GetQuestID() && questhint.npcid == -1 && questhint.questtype == m_QuestType )
					{
						ishintmark = true;
						break;
					}
				}
				
				if( !ishintmark )
				{
					_XQUEST_MARKINFO hintmark;
					memset(&hintmark, 0, sizeof(_XQUEST_MARKINFO));
					hintmark.MarkPosition_X = pArea->m_CenterPoint.x;
					hintmark.MarkPosition_Y = pArea->m_CenterPoint.z;
					hintmark.questid = currentquest->GetQuestID();
					hintmark.npcid = -1;
					hintmark.questtype = m_QuestType;
					g_QuestScriptManager.m_listHintMark.push_back(hintmark);
					
					//��ȣǳ��� ���� �޼��� ȭ�鿡 �����ֱ� 
					switch(m_QuestType) {
					case _XQUEST_TYPE_1:
						{
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1779), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
						}
						break;
					case _XQUEST_TYPE_2:
						{
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1780), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
						}
						break;
					case _XQUEST_TYPE_3:
						{
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1781), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
						}
						break;
					case _XQUEST_TYPE_4:
						{
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1782), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
						}
						break;
					}
				}
			}
		}							
	}
	
	return QUEST_CONDITION_RESULT_SUCCESS;
}


_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessSetHintMarkNPC(void)
{
	// m_compareclass1 : ��ũ Ÿ�� 
	// m_numericvalue1 : m_NPCID
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;
		
	// �Ӹ����� ! ����ֱ�
	for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; ++i)
	{
		_XNPCObject* pNPC = &g_Npclist.m_pNPCObjectArray[i];
		if(pNPC)
		{
			if(pNPC->m_UniqueID == m_TDC.m_numericvalue1)
			{
				switch(m_TDC.m_compareclass1) {
				case ID_TRIGGER_MARKTYPE_WORLD:
					{						
						bool ishintmark = false;
						list <_XQUEST_MARKINFO>::iterator iter_hintmark;
						for(iter_hintmark = g_QuestScriptManager.m_listHintMark.begin() ; iter_hintmark != g_QuestScriptManager.m_listHintMark.end() ; ++iter_hintmark)
						{
							_XQUEST_MARKINFO questhint = *iter_hintmark;
							if( questhint.questid == currentquest->GetQuestID() && questhint.npcid == m_TDC.m_numericvalue1 && questhint.questtype == m_QuestType )
							{
								ishintmark = true;
								break;
							}
						}
						
						if( !ishintmark )
						{
							_XQUEST_MARKINFO hintmark;
							memset(&hintmark, 0, sizeof(_XQUEST_MARKINFO));
							hintmark.MarkPosition_X = -9999;
							hintmark.MarkPosition_Y = -9999;
							hintmark.questid = currentquest->GetQuestID();
							hintmark.npcid = m_TDC.m_numericvalue1;
							hintmark.questtype = m_QuestType;
							g_QuestScriptManager.m_listHintMark.push_back(hintmark);			
						}
						
						
						if(currentquest->GetMainQuest())
						{
							pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_QUESTIONYELLOW;			
						}
						else 
						{
							switch( m_QuestType ) {
							case _XQUEST_TYPE_2://������ 
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_QUESTIONBLUE;		
								}
								break;
							case _XQUEST_TYPE_3://������
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_QUESTIONORANGE;		
								}
								break;
							case _XQUEST_TYPE_4://�⿬�� 
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_QUESTIONGREEN;		
								}
								break;
							case _XQUEST_TYPE_5://�����һ�
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_QUESTIONWHITE;		
								}
								break;
							}
						}
					}
					break;
				case ID_TRIGGER_MARKTYPE_MAP:
					{
						bool ishintmark = false;
						list <_XQUEST_MARKINFO>::iterator iter_hintmark;
						for(iter_hintmark = g_QuestScriptManager.m_listHintMark.begin() ; iter_hintmark != g_QuestScriptManager.m_listHintMark.end() ; ++iter_hintmark)
						{
							_XQUEST_MARKINFO questhint = *iter_hintmark;
							if( questhint.questid == currentquest->GetQuestID() && questhint.npcid == m_TDC.m_numericvalue1 && questhint.questtype == m_QuestType )
							{
								ishintmark = true;
								break;
							}
						}
						
						if( !ishintmark )
						{
							_XQUEST_MARKINFO hintmark;
							memset(&hintmark, 0, sizeof(_XQUEST_MARKINFO));
							hintmark.MarkPosition_X = pNPC->m_matWorldPosition._41;
							hintmark.MarkPosition_Y = pNPC->m_matWorldPosition._43;
							hintmark.questid = currentquest->GetQuestID();
							hintmark.npcid = m_TDC.m_numericvalue1;
							hintmark.questtype = m_QuestType;
							g_QuestScriptManager.m_listHintMark.push_back(hintmark);			
							
							//��ȣǳ��� ���� �޼��� ȭ�鿡 �����ֱ� 
							switch(m_QuestType) {
							case _XQUEST_TYPE_1:
								{
									g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1779), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
								}
								break;
							case _XQUEST_TYPE_2:
								{
									g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1780), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
								}
								break;
							case _XQUEST_TYPE_3:
								{
									g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1781), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
								}
								break;
							case _XQUEST_TYPE_4:
								{
									g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1782), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
								}
								break;
							}
						}
					}
					break;
				case ID_TRIGGER_MARKTYPE_WORLDANDMAP:
					{
						bool ishintmark = false;
						list <_XQUEST_MARKINFO>::iterator iter_hintmark;
						for(iter_hintmark = g_QuestScriptManager.m_listHintMark.begin() ; iter_hintmark != g_QuestScriptManager.m_listHintMark.end() ; ++iter_hintmark)
						{
							_XQUEST_MARKINFO questhint = *iter_hintmark;
							if( questhint.questid == currentquest->GetQuestID() && questhint.npcid == m_TDC.m_numericvalue1 && questhint.questtype == m_QuestType )
							{
								ishintmark = true;
								break;
							}
						}

						if( !ishintmark )
						{
							_XQUEST_MARKINFO hintmark;
							memset(&hintmark, 0, sizeof(_XQUEST_MARKINFO));
							hintmark.MarkPosition_X = pNPC->m_matWorldPosition._41;
							hintmark.MarkPosition_Y = pNPC->m_matWorldPosition._43;
							hintmark.questid = currentquest->GetQuestID();
							hintmark.npcid = m_TDC.m_numericvalue1;
							hintmark.questtype = m_QuestType;
							g_QuestScriptManager.m_listHintMark.push_back(hintmark);
							
							//��ȣǳ��� ���� �޼��� ȭ�鿡 �����ֱ� 
							switch(m_QuestType) {
							case _XQUEST_TYPE_1:
								{
									g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1779), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
								}
								break;
							case _XQUEST_TYPE_2:
								{
									g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1780), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
								}
								break;
							case _XQUEST_TYPE_3:
								{
									g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1781), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
								}
								break;
							case _XQUEST_TYPE_4:
								{
									g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_TRIGGERCONDITIONITEM_1782), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
								}
								break;
							}
						}
						
						
						if(currentquest->GetMainQuest())
						{
							pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_QUESTIONYELLOW;		
						}
						else
						{					
							switch( m_QuestType ) {
							case _XQUEST_TYPE_2://������ 
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_QUESTIONBLUE;	
								}
								break;
							case _XQUEST_TYPE_3://������
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_QUESTIONORANGE;		
								}
								break;
							case _XQUEST_TYPE_4://�⿬�� 
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_QUESTIONGREEN;		
								}
								break;
							case _XQUEST_TYPE_5://�����һ�
								{
									pNPC->m_HaveQuestType = _XNPC_HAVEQUEST_QUESTIONWHITE;	
								}
								break;
							}		
						}
					}
					break;
				}				
				break;
			}//if(pNPC->m_UniqueID == m_TDC.m_numericvalue1)
		}
	}
	
	return QUEST_CONDITION_RESULT_SUCCESS;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckCountPoint(void)
{		
	//	m_compareclass1 = m_CompareClass;
	//	m_compareclass2 = m_CounterType;
	//	m_numericvalue1 = m_Count;

	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_CONDITION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_CONDITION_RESULT_FAIL;

	if( m_TDC.m_compareclass2 < 4 )
	{
		switch(m_TDC.m_compareclass1) {		 
		case ID_TRIGGER_COMPARE_THELEAST :
			{
				if( currentquest->m_CounterInformation[ m_TDC.m_compareclass2 ].CountCurrentPoint < m_TDC.m_numericvalue1 )
					return QUEST_CONDITION_RESULT_SUCCESS;
			}
			break;
			
		case ID_TRIGGER_COMPARE_THEMOST :
			{
				if( currentquest->m_CounterInformation[ m_TDC.m_compareclass2 ].CountCurrentPoint > m_TDC.m_numericvalue1 )
					return QUEST_CONDITION_RESULT_SUCCESS;
			}
			break;
			
		case ID_TRIGGER_COMPARE_EXACTLY :
			{
				if( currentquest->m_CounterInformation[ m_TDC.m_compareclass2 ].CountCurrentPoint == m_TDC.m_numericvalue1 )
					return QUEST_CONDITION_RESULT_SUCCESS;
			}
			break;
		}
	}
	else//��Ƽ ī���� 
	{
		switch(m_TDC.m_compareclass1) {		 
		case ID_TRIGGER_COMPARE_THELEAST :
			{
				if( currentquest->m_PartyCounterInformation[ m_TDC.m_compareclass2-4 ].CountCurrentPoint < m_TDC.m_numericvalue1 )
					return QUEST_CONDITION_RESULT_SUCCESS;
			}
			break;
			
		case ID_TRIGGER_COMPARE_THEMOST :
			{
				if( currentquest->m_PartyCounterInformation[ m_TDC.m_compareclass2-4 ].CountCurrentPoint > m_TDC.m_numericvalue1 )
					return QUEST_CONDITION_RESULT_SUCCESS;
			}
			break;
			
		case ID_TRIGGER_COMPARE_EXACTLY :
			{
				if( currentquest->m_PartyCounterInformation[ m_TDC.m_compareclass2-4 ].CountCurrentPoint == m_TDC.m_numericvalue1 )
					return QUEST_CONDITION_RESULT_SUCCESS;
			}
			break;
		}
	}


	return QUEST_CONDITION_RESULT_FAIL;
}
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckMotion(void)
{	
	//m_numericvalue1 = m_MotionIndex;
	return QUEST_CONDITION_RESULT_SUCCESS;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckAbnormalstate(void)	// �÷��̾�_�����̻�üũ
{	
	//m_compareclass1 = m_StateIndex;
	return QUEST_CONDITION_RESULT_SUCCESS;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckBattleRecord(void)// �÷��̾�_������üũ
{	
	//m_compareclass1 = m_CompareTarget; //�¸�, �й�, ���º�, ����
	//m_compareclass2 = m_CompareClass; �̸�,�ʰ�,����
	//m_numericvalue1  = m_ShameNumber; ���� ����
	return QUEST_CONDITION_RESULT_SUCCESS;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckPartyJoinGroup(void)	// ��Ƽ��_����üũ
{	
	//m_compareclass1 = m_CheckJoinGroup;
	//m_compareclass2 = m_CompareClass; 
	//0:�������ü 1:1���̻� 2:2���̻� 3:3���̻� 4:4���̻� 5:5���̻� 6:6���̻� 7:7���̻� 8:8���̻� 
	if( g_pLocalUser->m_InParty )
	{
		int sameclancount = 0;
		for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
		{
			if( g_pLocalUser->m_PartyInfo[i].isValid )
			{
				if( g_pLocalUser->m_PartyInfo[i].clan == m_TDC.m_compareclass1 )
				{
					sameclancount++;
				}
			}
		}
		
		if( m_TDC.m_compareclass2 == 0 )
		{
			if( g_pLocalUser->m_PartyMemberCount != 0)
			{
				if( g_pLocalUser->m_PartyMemberCount == sameclancount )
				{					
					if( m_bStartCondition )
					{
						// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
						// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
						if( !g_QuestScriptManager.m_listCheckInParty.empty() )
						{
							bool balreadyexist = false;
							list <int>::iterator iter_checkinparty;
							for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
							{
								int partyquestid = *iter_checkinparty;
								if( partyquestid == m_nOwnerQuestID )
								{
									balreadyexist = true;
									break;
								}
							}
							if( !balreadyexist )
							{
								g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
							}
						}
						else
						{
							g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
						}
					}

					return QUEST_CONDITION_RESULT_SUCCESS;
				}
			}
		}
		else
		{
			if( sameclancount >= m_TDC.m_compareclass2 )
			{
				if( m_bStartCondition )
				{
					// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
					// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
					if( !g_QuestScriptManager.m_listCheckInParty.empty() )
					{
						bool balreadyexist = false;
						list <int>::iterator iter_checkinparty;
						for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
						{
							int partyquestid = *iter_checkinparty;
							if( partyquestid == m_nOwnerQuestID )
							{
								balreadyexist = true;
								break;
							}
						}
						if( !balreadyexist )
						{
							g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
						}
					}
					else
					{
						g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
					}
				}
				return QUEST_CONDITION_RESULT_SUCCESS;
			}
		}
		return QUEST_CONDITION_RESULT_FAIL;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckPartyValue(void)//��Ƽ��_���Ⱥ�(��,����ܰ� �˻�, ��,����ܰ� ���հ˻�...)
{	
	//m_compareclass1 = m_CompareTarget; // 0:�������� ���˻� 1: �������� ����ܰ�˻� 2:�����հ˻� 3:����ܰ����հ˻�
	// 0:�������� ����ܰ�˻� 1:����ܰ����հ˻� 2:�������� ���˻� 3:�����հ˻�
	//m_compareclass2 = m_CompareClass;
	//m_numericvalue1  = m_ShameNumber;
	if( g_pLocalUser->m_InParty)
	{		
		switch( m_TDC.m_compareclass1 ) {
		case 0:
			{
				switch(m_TDC.m_compareclass2) {		 
				case ID_TRIGGER_COMPARE_THELEAST :
					{
						int successcount = 0;
						for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
						{
							if( g_pLocalUser->m_PartyInfo[i].isValid )
							{
								if( g_pLocalUser->m_PartyInfo[i].innerlevel < m_TDC.m_numericvalue1 )
								{
									successcount++;
								}
							}
						}

						if( successcount >= g_pLocalUser->m_PartyMemberCount )
						{
							if( m_bStartCondition )
							{
								// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
								// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
								if( !g_QuestScriptManager.m_listCheckInParty.empty() )
								{
									bool balreadyexist = false;
									list <int>::iterator iter_checkinparty;
									for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
									{
										int partyquestid = *iter_checkinparty;
										if( partyquestid == m_nOwnerQuestID )
										{
											balreadyexist = true;
											break;
										}
									}
									if( !balreadyexist )
									{
										g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
									}
								}
								else
								{
									g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
								}
							}
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
					}
					break;
					
				case ID_TRIGGER_COMPARE_THEMOST :
					{
						int successcount = 0;
						for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
						{
							if( g_pLocalUser->m_PartyInfo[i].isValid )
							{
								if( g_pLocalUser->m_PartyInfo[i].innerlevel > m_TDC.m_numericvalue1 )
								{
									successcount++;
								}
							}
						}
						
						if( successcount >= g_pLocalUser->m_PartyMemberCount )
						{
							if( m_bStartCondition )
							{
								// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
								// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
								if( !g_QuestScriptManager.m_listCheckInParty.empty() )
								{
									bool balreadyexist = false;
									list <int>::iterator iter_checkinparty;
									for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
									{
										int partyquestid = *iter_checkinparty;
										if( partyquestid == m_nOwnerQuestID )
										{
											balreadyexist = true;
											break;
										}
									}
									if( !balreadyexist )
									{
										g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
									}
								}
								else
								{
									g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
								}
							}
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
					}
					break;
					
				case ID_TRIGGER_COMPARE_EXACTLY :
					{
						int successcount = 0;
						for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
						{
							if( g_pLocalUser->m_PartyInfo[i].isValid )
							{
								if( g_pLocalUser->m_PartyInfo[i].innerlevel == m_TDC.m_numericvalue1 )
								{
									successcount++;
								}
							}
						}

						if( successcount >= g_pLocalUser->m_PartyMemberCount )
						{
							
							if( m_bStartCondition )
							{
								// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
								// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
								if( !g_QuestScriptManager.m_listCheckInParty.empty() )
								{
									bool balreadyexist = false;
									list <int>::iterator iter_checkinparty;
									for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
									{
										int partyquestid = *iter_checkinparty;
										if( partyquestid == m_nOwnerQuestID )
										{
											balreadyexist = true;
											break;
										}
									}
									if( !balreadyexist )
									{
										g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
									}
								}
								else
								{
									g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
								}
							}
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
					}
					break;
				}					
				
			}
			break;
		case 1:
			{
				int totalinnerlevel = 0;
				for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
				{
					if( g_pLocalUser->m_PartyInfo[i].isValid )
					{
						totalinnerlevel += g_pLocalUser->m_PartyInfo[i].innerlevel;
					}
				}
				
				switch(m_TDC.m_compareclass2) {		 
				case ID_TRIGGER_COMPARE_THELEAST :
					{
						if( totalinnerlevel < m_TDC.m_numericvalue1 )
						{
							if( m_bStartCondition )
							{
								// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
								// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
								if( !g_QuestScriptManager.m_listCheckInParty.empty() )
								{
									bool balreadyexist = false;
									list <int>::iterator iter_checkinparty;
									for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
									{
										int partyquestid = *iter_checkinparty;
										if( partyquestid == m_nOwnerQuestID )
										{
											balreadyexist = true;
											break;
										}
									}
									if( !balreadyexist )
									{
										g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
									}
								}
								else
								{
									g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
								}
							}
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
					}
					break;
					
				case ID_TRIGGER_COMPARE_THEMOST :
					{
						if( totalinnerlevel > m_TDC.m_numericvalue1 )
						{
							
							if( m_bStartCondition )
							{
								// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
								// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
								if( !g_QuestScriptManager.m_listCheckInParty.empty() )
								{
									bool balreadyexist = false;
									list <int>::iterator iter_checkinparty;
									for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
									{
										int partyquestid = *iter_checkinparty;
										if( partyquestid == m_nOwnerQuestID )
										{
											balreadyexist = true;
											break;
										}
									}
									if( !balreadyexist )
									{
										g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
									}
								}
								else
								{
									g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
								}
							}
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
					}
					break;
					
				case ID_TRIGGER_COMPARE_EXACTLY :
					{
						if( totalinnerlevel == m_TDC.m_numericvalue1 )
						{
							if( m_bStartCondition )
							{
								// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
								// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
								if( !g_QuestScriptManager.m_listCheckInParty.empty() )
								{
									bool balreadyexist = false;
									list <int>::iterator iter_checkinparty;
									for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
									{
										int partyquestid = *iter_checkinparty;
										if( partyquestid == m_nOwnerQuestID )
										{
											balreadyexist = true;
											break;
										}
									}
									if( !balreadyexist )
									{
										g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
									}
								}
								else
								{
									g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
								}
							}
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
					}
					break;
				}					
				
			}
			break;
		case 2:
			{
				for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
				{
					if( g_pLocalUser->m_PartyInfo[i].isValid )
					{
						switch(m_TDC.m_compareclass2) {		 
						case ID_TRIGGER_COMPARE_THELEAST :
							{
								if( g_pLocalUser->m_PartyInfo[i].honor < m_TDC.m_numericvalue1 )
								{
									if( m_bStartCondition )
									{
										// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
										// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
										if( !g_QuestScriptManager.m_listCheckInParty.empty() )
										{
											bool balreadyexist = false;
											list <int>::iterator iter_checkinparty;
											for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
											{
												int partyquestid = *iter_checkinparty;
												if( partyquestid == m_nOwnerQuestID )
												{
													balreadyexist = true;
													break;
												}
											}
											if( !balreadyexist )
											{
												g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
											}
										}
										else
										{
											g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
										}
									}
									return QUEST_CONDITION_RESULT_SUCCESS;
								}
							}
							break;
							
						case ID_TRIGGER_COMPARE_THEMOST :
							{
								if( g_pLocalUser->m_PartyInfo[i].honor > m_TDC.m_numericvalue1 )
								{
									if( m_bStartCondition )
									{
										// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
										// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
										if( !g_QuestScriptManager.m_listCheckInParty.empty() )
										{
											bool balreadyexist = false;
											list <int>::iterator iter_checkinparty;
											for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
											{
												int partyquestid = *iter_checkinparty;
												if( partyquestid == m_nOwnerQuestID )
												{
													balreadyexist = true;
													break;
												}
											}
											if( !balreadyexist )
											{
												g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
											}
										}
										else
										{
											g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
										}
									}
									return QUEST_CONDITION_RESULT_SUCCESS;
								}
							}
							break;
							
						case ID_TRIGGER_COMPARE_EXACTLY :
							{
								if( g_pLocalUser->m_PartyInfo[i].honor == m_TDC.m_numericvalue1 )
								{
									if( m_bStartCondition )
									{
										// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
										// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
										if( !g_QuestScriptManager.m_listCheckInParty.empty() )
										{
											bool balreadyexist = false;
											list <int>::iterator iter_checkinparty;
											for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
											{
												int partyquestid = *iter_checkinparty;
												if( partyquestid == m_nOwnerQuestID )
												{
													balreadyexist = true;
													break;
												}
											}
											if( !balreadyexist )
											{
												g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
											}
										}
										else
										{
											g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
										}
									}
									return QUEST_CONDITION_RESULT_SUCCESS;
								}
							}
							break;
						}					
					}
				}
			}
			break;
		case 3:
			{
				int totalhonor = 0;
				for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
				{
					if( g_pLocalUser->m_PartyInfo[i].isValid )
					{
						totalhonor += g_pLocalUser->m_PartyInfo[i].honor;
					}
				}
				
				switch(m_TDC.m_compareclass2) {		 
				case ID_TRIGGER_COMPARE_THELEAST :
					{
						if( totalhonor < m_TDC.m_numericvalue1 )
						{
							if( m_bStartCondition )
							{
								// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
								// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
								if( !g_QuestScriptManager.m_listCheckInParty.empty() )
								{
									bool balreadyexist = false;
									list <int>::iterator iter_checkinparty;
									for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
									{
										int partyquestid = *iter_checkinparty;
										if( partyquestid == m_nOwnerQuestID )
										{
											balreadyexist = true;
											break;
										}
									}
									if( !balreadyexist )
									{
										g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
									}
								}
								else
								{
									g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
								}
							}
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
					}
					break;
					
				case ID_TRIGGER_COMPARE_THEMOST :
					{
						if( totalhonor > m_TDC.m_numericvalue1 )
						{
							if( m_bStartCondition )
							{
								// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
								// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
								if( !g_QuestScriptManager.m_listCheckInParty.empty() )
								{
									bool balreadyexist = false;
									list <int>::iterator iter_checkinparty;
									for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
									{
										int partyquestid = *iter_checkinparty;
										if( partyquestid == m_nOwnerQuestID )
										{
											balreadyexist = true;
											break;
										}
									}
									if( !balreadyexist )
									{
										g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
									}
								}
								else
								{
									g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
								}
							}
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
					}
					break;
					
				case ID_TRIGGER_COMPARE_EXACTLY :
					{
						if( totalhonor == m_TDC.m_numericvalue1 )
						{
							if( m_bStartCondition )
							{
								// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
								// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
								if( !g_QuestScriptManager.m_listCheckInParty.empty() )
								{
									bool balreadyexist = false;
									list <int>::iterator iter_checkinparty;
									for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
									{
										int partyquestid = *iter_checkinparty;
										if( partyquestid == m_nOwnerQuestID )
										{
											balreadyexist = true;
											break;
										}
									}
									if( !balreadyexist )
									{
										g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
									}
								}
								else
								{
									g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
								}
							}
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
					}
					break;
				}					
			}
			break;
		}

	}
	return QUEST_CONDITION_RESULT_FAIL;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckSkillState(void)//�÷��̾�_�������º�
{	
	//m_compareclass1 = m_CompareClass; // 0:�����ʿ� 1:������밡��
	//m_numericvalue1  = m_SkillID;
	
	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
	{
		if(g_pLocalUser->m_SkillList[i].m_sSkillID == 0)
			continue;
		
		if(g_pLocalUser->m_SkillList[i].m_sSkillID == m_TDC.m_numericvalue1)
		{
			switch(m_TDC.m_compareclass1)
			{
			case 0 :
				{
					if(g_pLocalUser->m_SkillList[i].m_ucSkillState == 1 )//���� �ʿ� ���� 
						return QUEST_CONDITION_RESULT_SUCCESS;
				}
				break;
			case 1 :
				{
					if(g_pLocalUser->m_SkillList[i].m_ucSkillState == 0 )//������� ���� ���� 
						return QUEST_CONDITION_RESULT_SUCCESS;
				}
				break;
			}
		}
	}
	return QUEST_CONDITION_RESULT_FAIL;
}


_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckClanServicePoint(void)//�÷��̾�_���ı⿩��üũ
{	
	
	//	scriptdata->m_TDC.m_compareclass1 = m_CompareClass; 0:�̸� 1:�ʰ� 2:����
	//	scriptdata->m_TDC.m_numericvalue1 = m_Count;
	
	switch(m_TDC.m_compareclass1)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(g_pLocalUser->m_CharacterInfo.Get_iContribution() < m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(g_pLocalUser->m_CharacterInfo.Get_iContribution() > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(g_pLocalUser->m_CharacterInfo.Get_iContribution() == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckQuestMail(void)
{	
	if( m_bStartCondition )
	{
		if( !g_QuestScriptManager.m_listQuestMailInfo.empty() )
		{
			list <_XQUESTMAIL_INFO>::iterator iter_questmail;
			for(iter_questmail = g_QuestScriptManager.m_listQuestMailInfo.begin() ; iter_questmail != g_QuestScriptManager.m_listQuestMailInfo.end() ; )
			{
				_XQUESTMAIL_INFO questmail = *iter_questmail;
				if( questmail.questid == m_nOwnerQuestID )
				{
					if( questmail.questmailtype == 0 )// Ȯ��
					{
						if( questmail.bYESorNO ) 
						{
							iter_questmail = g_QuestScriptManager.m_listQuestMailInfo.erase(iter_questmail);
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
					}
					else// ���� or �ź� 
					{
						if( questmail.bYESorNO ) 
						{							
							iter_questmail = g_QuestScriptManager.m_listQuestMailInfo.erase(iter_questmail);
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
						else
						{
							// 30�� �Ŀ� �ٽ� ǳ����� ���ƿ´�.
							// �ٽ� ��� ���� 
							
							_XQUESTMAIL_INFO questmailbuffer;
							memset(&questmailbuffer, 0, sizeof(_XQUESTMAIL_INFO));
							questmailbuffer.questid	= questmail.questid;
							questmailbuffer.questmailtype = questmail.questmailtype;
							questmailbuffer.resendstarttime = g_LocalSystemTime;
							questmailbuffer.delaytime = 1800000;

							g_QuestScriptManager.m_listQuestMailBuffer.push_back(questmailbuffer);	

							iter_questmail = g_QuestScriptManager.m_listQuestMailInfo.erase(iter_questmail);
							
							return QUEST_CONDITION_RESULT_PROCESS;
						}
					}
					iter_questmail = g_QuestScriptManager.m_listQuestMailInfo.erase(iter_questmail);
				}
				else
				{
					iter_questmail++;
				}
			}
		}
	}
	else
	{
		if( !g_QuestScriptManager.m_listQuestMailInfo.empty() )
		{
			list <_XQUESTMAIL_INFO>::iterator iter_questmail;
			for(iter_questmail = g_QuestScriptManager.m_listQuestMailInfo.begin() ; iter_questmail != g_QuestScriptManager.m_listQuestMailInfo.end() ; )
			{
				_XQUESTMAIL_INFO questmail = *iter_questmail;
				if( questmail.questid == m_nOwnerQuestID )
				{
					if( questmail.questmailtype == 0 )// Ȯ��
					{
						if( questmail.bYESorNO ) 
							iter_questmail = g_QuestScriptManager.m_listQuestMailInfo.erase(iter_questmail);
						return QUEST_CONDITION_RESULT_SUCCESS;
					}
					else// ���� or �ź� 
					{
						if( questmail.bYESorNO ) 
						{
							iter_questmail = g_QuestScriptManager.m_listQuestMailInfo.erase(iter_questmail);
							return QUEST_CONDITION_RESULT_SUCCESS;
						}
						else
						{
							iter_questmail = g_QuestScriptManager.m_listQuestMailInfo.erase(iter_questmail);
							return QUEST_CONDITION_RESULT_FAIL;
						}
						
					}
					iter_questmail = g_QuestScriptManager.m_listQuestMailInfo.erase(iter_questmail);
				}
				else
				{
					iter_questmail++;
				}
			}
		}
	}
	return QUEST_CONDITION_RESULT_PROCESS;
	
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckBossBattleStart(void)//����� ���� üũ
{	
	//	scriptdata->m_TDC.m_conditionclass = ID_CONDITION_CHECKBOSSBATTLESTART;	
	//	scriptdata->m_TDC.m_numericvalue1 = m_TriggerID;// ������ �ε��� 
	if( g_pLocalUser->m_bIsBossBattle )
	{
		if( g_CurrentZoneInfoIndex == m_TDC.m_numericvalue1 )
			return QUEST_CONDITION_RESULT_SUCCESS;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckBossBattleStageNumber(void)//����� �ܰ� üũ
{	
	//	scriptdata->m_TDC.m_conditionclass = ID_CONDITION_CHECKBOSSBATTELSTAGENUMBER;
	//	scriptdata->m_TDC.m_numericvalue1 = m_TriggerID; // �������� ��ȣ
	if( m_TDC.m_numericvalue1 != 100 )
	{
		if( g_pLocalUser->m_bIsBossBattle )
		{
			if( g_pLocalUser->m_BossRoomStageNumber == m_TDC.m_numericvalue1 )
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
	}
	else// ����� �������� üũ�� 
	{
		if( !g_pLocalUser->m_bIsBossBattle )//�������� �ƴϸ� �����̵� 
		{
			if( g_pLocalUser->m_bBossBattleEndProcess ) 
			{	
				if( g_pLocalUser->m_bSuccessBossBattle )
				{
					return QUEST_CONDITION_RESULT_SUCCESS;
				}
			}
		}
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckPartyMaster(void)//��Ƽ��_����˻�
{	
	//scriptdata->m_TDC.m_conditionclass = ID_CONDITION_CHECKPARTYMASTER;
	//scriptdata->m_TDC.m_compareclass1 = m_CompareClass; //0:����� ��� 1:����� �ƴ� ���
	if( g_pLocalUser->m_InParty)
	{		
		switch(m_TDC.m_compareclass1) {
		case 0:
			{	
				if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )  // �����.
				{
					if( m_bStartCondition )
					{
						// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
						// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
						if( !g_QuestScriptManager.m_listCheckInParty.empty() )
						{
							bool balreadyexist = false;
							list <int>::iterator iter_checkinparty;
							for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
							{
								int partyquestid = *iter_checkinparty;
								if( partyquestid == m_nOwnerQuestID )
								{
									balreadyexist = true;
									break;
								}
							}
							if( !balreadyexist )
							{
								g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
							}
						}
						else
						{
							g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
						}
					}
					return QUEST_CONDITION_RESULT_SUCCESS;
				}
				else
				{
					return QUEST_CONDITION_RESULT_FAIL;
				}
			}
			break;
		case 1:
			{
				if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )  // �����.
				{
					return QUEST_CONDITION_RESULT_FAIL;
				}
				else
				{
					if( m_bStartCondition )
					{
						// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
						// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
						if( !g_QuestScriptManager.m_listCheckInParty.empty() )
						{
							bool balreadyexist = false;
							list <int>::iterator iter_checkinparty;
							for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
							{
								int partyquestid = *iter_checkinparty;
								if( partyquestid == m_nOwnerQuestID )
								{
									balreadyexist = true;
									break;
								}
							}
							if( !balreadyexist )
							{
								g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
							}
						}
						else
						{
							g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
						}
					}
					return QUEST_CONDITION_RESULT_SUCCESS;
				}
			}
			break;
		}
//		bool checkleader = FALSE;
//		for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++)
//		{
//			if( g_pLocalUser->m_PartyInfo[i].isLeader )
//			{
//				if( strcmp(g_pLocalUser->m_PartyInfo[i].playername, g_pLocalUser->m_CharacterInfo.charactername) == 0 )
//				{
//					checkleader = TRUE;
//					return QUEST_CONDITION_RESULT_SUCCESS;
//				}
//				return QUEST_CONDITION_RESULT_FAIL;
//			}
//		}
//		return QUEST_CONDITION_RESULT_FAIL;

	}
	return QUEST_CONDITION_RESULT_FAIL;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckPartyMemberClass(void)//��Ƽ��_��å�˻�
{	
	//scriptdata->m_TDC.m_conditionclass = ID_CONDITION_CHECKPARTYMEMBERCLASS;
	//scriptdata->m_TDC.m_compareclass1 = m_PartyMemberClass;	//0:������ 1:������ 2:������ 3:�����
	//scriptdata->m_TDC.m_compareclass2 = m_CompareClass;		//0:�������ü 1:1���̻� 2:2���̻� 3:3���̻� 4:4���̻�
																//5:5���̻� 6:6���̻� 7:7���̻� 8:8���̻�
	return QUEST_CONDITION_RESULT_FAIL;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckAreaPartyMember(void)//��Ƽ��_�����˻�
{	
	//scriptdata->m_TDC.m_conditionclass = ID_CONDITION_CHECKAREAPARTYMEMBER;
	//scriptdata->m_TDC.m_compareclass2	= m_CheckServerPass; // 0:�����˻��ϱ� 1:�����˻���ϱ�
	//scriptdata->m_TDC.m_compareclass1	= m_AreaCategory;
	//scriptdata->m_TDC.m_numericvalue1	= m_AreaID;
	
	if( !g_pLocalUser->m_InParty )
		return QUEST_CONDITION_RESULT_FAIL;

	if( (g_LocalSystemTime - m_LastCheckTime) > 3000 )
	{
		m_LastCheckTime = g_LocalSystemTime;
		D3DXVECTOR3	partymemberposition;
		for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
		{
			partymemberposition.x = g_pLocalUser->m_PartyInfo[i].posX;
			partymemberposition.y = 0;
			partymemberposition.z = g_pLocalUser->m_PartyInfo[i].posZ;
			
			//��Ƽ�� �Ѹ��̶� ���� �ȿ� ������ ����
			if(!g_AreaManager.FindPositionInArea(partymemberposition, m_TDC.m_numericvalue1, m_TDC.m_compareclass1))
				return QUEST_CONDITION_RESULT_FAIL;			
		}

		g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		
		if( m_bStartCondition )
		{
			// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
			// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
			if( !g_QuestScriptManager.m_listCheckInParty.empty() )
			{
				bool balreadyexist = false;
				list <int>::iterator iter_checkinparty;
				for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
				{
					int partyquestid = *iter_checkinparty;
					if( partyquestid == m_nOwnerQuestID )
					{
						balreadyexist = true;
						break;
					}
				}
				if( !balreadyexist )
				{
					g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
				}
			}
			else
			{
				g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
			}
		}
		return QUEST_CONDITION_RESULT_SUCCESS;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckRecordNumber(void)//�÷��̾�_���ǥ�˻�
{	
	//scriptdata->m_TDC.m_conditionclass = ID_CONDITION_CHECKRECORDNUMBER;
	//scriptdata->m_TDC.m_compareclass1	= m_CompareClass;// 0:�ʰ� 1:�̸� 2;����
	//scriptdata->m_TDC.m_numericvalue1	= m_RecordNumber;//���ǥ ����
	return QUEST_CONDITION_RESULT_FAIL;
}

// �÷��̾�_���뵵�˻�
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckExperience(void)
{
	//m_TDC.m_compareclass1	= m_CompareClass; 
	//m_TDC.m_numericvalue1	= m_Level;		// ����ܰ�
	//m_TDC.m_numericvalue2	= m_InnerLevel;		// ����ġ %����
	if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() != m_TDC.m_numericvalue1)
		return QUEST_CONDITION_RESULT_FAIL;
	
	FLOAT levelpercetage = ((FLOAT)g_pLocalUser->m_CharacterInfo.Get_jinlevel() / 
		(FLOAT)g_UserLevelTable[ g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1 ].needjinexperience) * 100;
	
	switch(m_TDC.m_compareclass2)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(levelpercetage < m_TDC.m_numericvalue2)
		{			
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(levelpercetage > m_TDC.m_numericvalue2)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(levelpercetage == m_TDC.m_numericvalue2)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// �÷��̾�_��å�˻�
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckPlayerClass(void)
{
	//m_TDC.m_compareclass1 = m_Class;	//0:������ 1:������ 2:������ 3:�����

	// ����,����,�������̸� �״�� �˻�
	if( m_TDC.m_compareclass1 =! 3 )
	{
		if( g_pLocalUser->m_CharacterInfo.Get_cClass() == m_TDC.m_compareclass1+1 )
			return QUEST_CONDITION_RESULT_SUCCESS;
	}
	else
	{
		switch(g_pLocalUser->m_CharacterInfo.groupindex)
		{
		case _XGROUP_GAEBANG:
		case _XGROUP_SORIM:
		case _XGROUP_MOODANG:
		case _XGROUP_MAKYO:
			{
				if( g_pLocalUser->m_CharacterInfo.Get_cClass() == _XROLLCLASS_SPELLER )
					return QUEST_CONDITION_RESULT_SUCCESS;		
			}
			break;
		case _XGROUP_BEEGOONG:
			{
				if( g_pLocalUser->m_CharacterInfo.Get_cClass() == _XROLLCLASS_JUSOOL )
					return QUEST_CONDITION_RESULT_SUCCESS;		
			}
			break;
		case _XGROUP_NOCKRIM:
			{
				if( g_pLocalUser->m_CharacterInfo.Get_cClass() == _XROLLCLASS_WOIGONG )
					return QUEST_CONDITION_RESULT_SUCCESS;		
			}
			break;
		}
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// �೶���������˻�
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckInvenspace(void)
{
	//m_TDC.m_compareclass1	= m_CompareClass;
	//m_TDC.m_numericvalue1	= m_InvenNumber;
	
	int		emptyslotcount = 0;
	for( int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
		{
			emptyslotcount++;
		}
	}

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
	for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
			emptyslotcount++;
	}
#endif
	
	switch(m_TDC.m_compareclass1)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		if(emptyslotcount < m_TDC.m_numericvalue1)
		{			
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_THEMOST :
		if(emptyslotcount > m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else
			break;
		
	case ID_TRIGGER_COMPARE_EXACTLY :
		if(emptyslotcount == m_TDC.m_numericvalue1)
		{
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
		else break;
		
	default :
		break;
	}
	
	return QUEST_CONDITION_RESULT_FAIL;
}

// ��������˻�(�ɼ����ǽĿ����� ���)
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckPartyMasterDead(void)
{
	if(g_pLocalUser->m_InParty)
	{
		for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
		{
			if(	g_pLocalUser->m_PartyInfo[i].isLeader )
			{
				if( g_pLocalUser->m_PartyInfo[i].vitalpercent <= 0 )
					return QUEST_CONDITION_RESULT_SUCCESS;
				break;
			}
		}
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// �������ı��˻�(�ɼ����ǽ�OR��������Ÿ��)
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckDestroyItem(void)
{
	//m_TDC.m_compareclass1 = m_ItemType;		// ������ Ÿ��
	//m_TDC.m_numericvalue1 = m_ItemID;		// ������ ���̵�

	if( g_pLocalUser->m_CharacterLog.isDestroyItem )
	{
		if( g_pLocalUser->m_CharacterLog.DestroyItemType == m_TDC.m_compareclass1 
			&& g_pLocalUser->m_CharacterLog.DestroyItemID == m_TDC.m_numericvalue1 )
		{
			g_pLocalUser->m_CharacterLog.isDestroyItem		= FALSE;
			g_pLocalUser->m_CharacterLog.DestroyItemType	= -1;
			g_pLocalUser->m_CharacterLog.DestroyItemID		= 0;
			g_pLocalUser->m_CharacterLog.DestroyItemTime	= 0;
			return QUEST_CONDITION_RESULT_SUCCESS;
		}
	}
	return QUEST_CONDITION_RESULT_FAIL;
}

// �ο������������˻�
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckStartLimitQuest(void)
{
	//m_TDC.m_compareclass1  = m_TargetClass;		//0:����á���� �˻� 1:Ư����ġ �˻�
	//m_TDC.m_compareclass2  = m_CompareClass;		//0:�̸� 1:�ʰ� 2:����
	//m_TDC.m_numericvalue1  = m_Quantity;
	return QUEST_CONDITION_RESULT_FAIL;
	
}

// �Ϸἱ���������˻�
_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckCompleteLimitQuest(void)
{
	//m_TDC.m_compareclass1  = m_TargetClass;		//0:����á���� �˻� 1:Ư����ġ �˻�
	//m_TDC.m_compareclass2  = m_CompareClass;		//0:�̸� 1:�ʰ� 2:����
	//m_TDC.m_numericvalue1  = m_Quantity;
	return QUEST_CONDITION_RESULT_FAIL;
	
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckPartyMemberGender(void)
{
	//scriptdata->m_TDC.m_conditionclass = ID_CONDITION_CHECKPARTYGENDER;
	//scriptdata->m_TDC.m_compareclass1 = m_PartyMemberGender;	//0:���� 1:����
	//scriptdata->m_TDC.m_compareclass2 = m_CompareClass; //0:�������ü 1:1���̻� 2:2���̻� 3:3���̻� 4:4���̻� 5:5���̻� 6:6���̻� 7:7���̻� 8:8���̻�
	return QUEST_CONDITION_RESULT_FAIL;
	
}

_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckServiceType(void)
{	
	//scriptdata->m_TDC.m_conditionclass = ID_CONDITION_CHECKSERVICETYPE;
	//scriptdata->m_TDC.m_compareclass1  = m_indexServiceType;				// 0:������� 1:�����Թ� 2:����PC�� 3:�������+����PC�� 4:����

	switch(m_TDC.m_compareclass1) {
	case 0://�������
		{
			if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Premium )
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case 1://�����Թ�
		{
			if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Basic )
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case 2://����PC��
		{
			if( g_LocalUserBillingInformations.BillingStatusWho == _XB_BillingStatusWho_PC )
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case 3://�������+����PC��
		{
			if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Premium && g_LocalUserBillingInformations.BillingStatusWho == _XB_BillingStatusWho_PC)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case 4://����
		{
			if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Free )
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	}

	return QUEST_CONDITION_RESULT_FAIL;
}


_XQUESTCONDITION_RESULT _XTriggerConditionItem::ProcessCheckPartyOrganization(void)
{
	//scriptdata->m_TDC.m_conditionclass = ID_CONDITION_CHECKPARTYORGANIZATION;
	//scriptdata->m_TDC.m_compareclass1 = m_CompareClass; //0:�������ü 1:1���̻� 2:2���̻� 3:3���̻� 4:4���̻� 5:5���̻� 6:6���̻� 7:7���̻� 8:8���̻� 9:����
	if( g_pLocalUser->m_InParty)
	{		
		if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName[0] != 0 )
		{
			int countinorganizationmember = 0;
			for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)// 2004.06.01->oneway48 modify : 15->9
			{
				if( g_pLocalUser->m_PartyInfo[i].playername[0] != 0 )
				{
					list<_Unit*>::iterator findunit = g_pGroup_Window->m_OrganizationMemberList.begin();
					for( ; findunit != g_pGroup_Window->m_OrganizationMemberList.end(); ++findunit )
					{
						_Unit* tempunit = *findunit;
						if( tempunit )
						{
							if( strcmp( tempunit->m_cName , g_pLocalUser->m_PartyInfo[i].playername ) == 0 )
							{						
								countinorganizationmember++;
								break;
							}
						}
					}	
				}
			}

			if( m_TDC.m_compareclass1 == 0 )
			{
				if( g_pLocalUser->m_PartyMemberCount == countinorganizationmember )
				{
					if( m_bStartCondition )
					{
						// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
						// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
						if( !g_QuestScriptManager.m_listCheckInParty.empty() )
						{
							bool balreadyexist = false;
							list <int>::iterator iter_checkinparty;
							for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
							{
								int partyquestid = *iter_checkinparty;
								if( partyquestid == m_nOwnerQuestID )
								{
									balreadyexist = true;
									break;
								}
							}
							if( !balreadyexist )
							{
								g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
							}
						}
						else
						{
							g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
						}
					}
					return QUEST_CONDITION_RESULT_SUCCESS;
				}
			}
			else if( m_TDC.m_compareclass1 == 9 )
			{
				if( countinorganizationmember == 0 )
				{
					if( m_bStartCondition )
					{
						// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
						// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
						if( !g_QuestScriptManager.m_listCheckInParty.empty() )
						{
							bool balreadyexist = false;
							list <int>::iterator iter_checkinparty;
							for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
							{
								int partyquestid = *iter_checkinparty;
								if( partyquestid == m_nOwnerQuestID )
								{
									balreadyexist = true;
									break;
								}
							}
							if( !balreadyexist )
							{
								g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
							}
						}
						else
						{
							g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
						}
					}
					return QUEST_CONDITION_RESULT_SUCCESS;
				}
			}
			else
			{
				if( countinorganizationmember >= m_TDC.m_compareclass1 )
				{
					if( m_bStartCondition )
					{
						// ���������϶� ��Ƽ ���� �˻簡 ������ �� ����Ʈ ������ ������ �־�� �Ѵ�.
						// ��Ƽ�� Ż���ϸ� ���������� �ٽ� �˻��ؾ� �Ѵ�.
						if( !g_QuestScriptManager.m_listCheckInParty.empty() )
						{
							bool balreadyexist = false;
							list <int>::iterator iter_checkinparty;
							for(iter_checkinparty = g_QuestScriptManager.m_listCheckInParty.begin() ; iter_checkinparty != g_QuestScriptManager.m_listCheckInParty.end() ;++iter_checkinparty )
							{
								int partyquestid = *iter_checkinparty;
								if( partyquestid == m_nOwnerQuestID )
								{
									balreadyexist = true;
									break;
								}
							}
							if( !balreadyexist )
							{
								g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
							}
						}
						else
						{
							g_QuestScriptManager.m_listCheckInParty.push_back(m_nOwnerQuestID);
						}
					}
					return QUEST_CONDITION_RESULT_SUCCESS;
				}
			}
		}
	}


	return QUEST_CONDITION_RESULT_FAIL;
	
}

_XQUESTCONDITION_RESULT	_XTriggerConditionItem::ProcessPartyHaveItem(void)
{

	// �ϴ� ������ �������� ó��.. kerygma
	return QUEST_CONDITION_RESULT_SUCCESS;
}

_XQUESTCONDITION_RESULT	_XTriggerConditionItem::PorcessCheckMasterPoint()
{
	// �����ý���_���±⿩���˻�
	//	scriptdata->m_TDC.m_compareclass1 = m_CompareClass; 0:�̸� 1:�ʰ� 2:����
	//	scriptdata->m_TDC.m_numericvalue1 = m_Count;
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
	switch(m_TDC.m_compareclass1)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		{
			if(g_pLocalUser->m_pLocalMnDInfomation.nMasterContribution < m_TDC.m_numericvalue1)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_TRIGGER_COMPARE_THEMOST :
		{
			if(g_pLocalUser->m_pLocalMnDInfomation.nMasterContribution > m_TDC.m_numericvalue1)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_TRIGGER_COMPARE_EXACTLY :
		{
			if(g_pLocalUser->m_pLocalMnDInfomation.nMasterContribution == m_TDC.m_numericvalue1)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	default :
		break;
	}
#endif

	return QUEST_CONDITION_RESULT_FAIL;
}

_XQUESTCONDITION_RESULT	_XTriggerConditionItem::PorcessCheckMasterRank()
{
	// �����ý���_���µ�ް˻�
	//	scriptdata->m_TDC.m_compareclass1 = m_CompareClass; 0:�̸� 1:�ʰ� 2:����
	//	scriptdata->m_TDC.m_numericvalue1 = m_Count;
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
	switch(m_TDC.m_compareclass1)
	{
	case ID_TRIGGER_COMPARE_THELEAST :
		{
			if(g_pLocalUser->m_pLocalMnDInfomation.nMasterRank < m_TDC.m_numericvalue1)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_TRIGGER_COMPARE_THEMOST :
		{
			if(g_pLocalUser->m_pLocalMnDInfomation.nMasterRank > m_TDC.m_numericvalue1)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	case ID_TRIGGER_COMPARE_EXACTLY :
		{
			if(g_pLocalUser->m_pLocalMnDInfomation.nMasterRank == m_TDC.m_numericvalue1)
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	default :
		break;
	}
#endif

	return QUEST_CONDITION_RESULT_FAIL;
}

_XQUESTCONDITION_RESULT	_XTriggerConditionItem::PorcessCheckMasterStatus()
{
	// �����ý���_���°˻�
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
	switch( g_pLocalUser->m_pLocalMnDInfomation.nMnDClass )
	{
	case 0 : //�ƹ��͵� �ƴ� 
		{
			if( g_pLocalUser->m_pLocalMnDInfomation.nMnDClass == m_TDC.m_compareclass1 )
				return QUEST_CONDITION_RESULT_SUCCESS;
			else
			{
				//����Ʈ ��Ҹ� ������ �Ѵ�
			}
		}
		break;
	case 1 : //����
		{
			if( g_pLocalUser->m_pLocalMnDInfomation.nMnDClass == m_TDC.m_compareclass1 )
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	default : //���� 2�̻��� ��δ� ���� �̴�
		{
			if( g_pLocalUser->m_pLocalMnDInfomation.nMnDClass >= m_TDC.m_compareclass1 ) 
				return QUEST_CONDITION_RESULT_SUCCESS;
		}
		break;
	}
#endif
	
	return QUEST_CONDITION_RESULT_FAIL;
}
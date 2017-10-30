// XCharacterState.cpp: implementation of the _XCharacterState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XCharacterState.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XWindow_JinStatus.h"
#include "XParticleIDDefine.h"
#include "SoundEffectList.h"

#include "XSR_STRINGHEADER.h"

map <int, _XCharacterStateTable*>	g_CharacterStateTable;
_XCharacterCondition	g_CharacterConditionTable[32];

extern void ReplaceCharacter(LPTSTR _string, TCHAR srcchar, TCHAR changechar);
extern void ChangeModel(DWORD lparam, char firstType, char facemaskmode = 0 );

// local variable
struct str_effect_entry
{
	short	where;
	char*	effectname;
};

list <str_effect_entry*> lst_EffectDescription;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XCharacterState::_XCharacterState()
{
	m_usLeftTimeSec		= 0;
	m_sValue			= 0;
	m_dwStateStartTime	= 0;
	m_sSkillID			= 0;
	m_cStep				= 0;
	m_bDeleteReqPacket	= false;
	m_FromType			= 0;
}

_XCharacterState::~_XCharacterState()
{

}

//////////////////////////////////////////////////////////////////////
// _XCharacterStateList
//////////////////////////////////////////////////////////////////////

_XCharacterStateList::_XCharacterStateList()
{
	m_TotalListCount = 0;
}

_XCharacterStateList::~_XCharacterStateList()
{
	Release();
}

void _XCharacterStateList::Release(void)
{
	map <int, _XCharacterState*>::iterator iter_state;
	for(iter_state = m_mapCharacterStateList.begin() ; iter_state != m_mapCharacterStateList.end() ; ++iter_state)
	{
		_XCharacterState* pState = iter_state->second;
		if(pState)
		{
			SAFE_DELETE(pState);
		}
	}
	m_mapCharacterStateList.clear();
	
	m_TotalListCount = 0;
}

void _XCharacterStateList::InsertState(unsigned short uid, short id, unsigned short lefttime, char fromtype, short val, short skillid, char step)
{
	// ������ Insert

	_XCharacterState* pState = new _XCharacterState;

	pState->m_FromType		= fromtype;
	pState->m_sEffectID		= id;
	pState->m_sSkillID		= skillid;
	pState->m_cStep			= step;

	pState->m_usLeftTimeSec = lefttime;
	pState->m_sValue		= val;
	pState->m_dwStateStartTime	= g_LocalSystemTime;
	pState->m_bDeleteReqPacket	= false;

	m_mapCharacterStateList[m_TotalListCount] = pState;
	m_TotalListCount++;

	// insert condition
	if(pState->m_FromType == en_from_effect)
	{
		map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
		if(iter_table != g_CharacterStateTable.end())
		{
			_XCharacterStateTable* pSTable = iter_table->second;
			if(pSTable)
			{
				if(pSTable->condition_flag_id > 0)
				{
					InsertConditionState(uid, pSTable->condition_flag_id);
				}
				else
				{
					if(g_CharacterStateTable[id]->effectNumber != 0)
					{
						if(uid == g_pLocalUser->m_Uniqueid)
						{
							if(!g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance(g_CharacterStateTable[id]->effectNumber))
							{
								D3DXMATRIX offsetmat;
								D3DXMatrixIdentity(&offsetmat);
								g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									g_CharacterStateTable[id]->effectNumber, &offsetmat, NULL, TRUE);
							}
						}
						else
						{
							_XUser* pUser = g_Userlist.FindUser(uid);
							if(pUser)
							{
								if(!pUser->m_ModelDescriptor.m_EffectManager->FindInstance(g_CharacterStateTable[id]->effectNumber))
								{
									D3DXMATRIX offsetmat;
									D3DXMatrixIdentity(&offsetmat);
									pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										g_CharacterStateTable[id]->effectNumber, &offsetmat, NULL, TRUE);
								}
							} // if(pUser)
						}
					} // if(g_CharacterStateTable[id]->effectNumber != 0)
				}
			}
		}
	} // if(pState->m_FromType == en_from_effect)
}

void _XCharacterStateList::InsertStateFromSpell(short skillid, char skillstep, unsigned short lefttime, int effectindex)
{
	_XCharacterState* pState = new _XCharacterState;
	
	pState->m_FromType		= en_from_skill;
	pState->m_sSkillID		= skillid;
	pState->m_cStep			= skillstep;

	pState->m_cSlotIndex	= 0;

	pState->m_sEffectID		= g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].id;
	pState->m_sValue		= g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].value;
	
	pState->m_usLeftTimeSec = lefttime;
	pState->m_dwStateStartTime	= g_LocalSystemTime;
	pState->m_bDeleteReqPacket	= false;
	
	m_mapCharacterStateList[m_TotalListCount] = pState;
	m_TotalListCount++;
}

void _XCharacterStateList::InsertStateFromEffect(unsigned short uid, short effectid, unsigned short lefttime, short value)
{
	_XCharacterState* pState = new _XCharacterState;
	
	pState->m_FromType		= en_from_effect;

	pState->m_sEffectID		= effectid;
	pState->m_usLeftTimeSec = lefttime;
	pState->m_sValue		= value;

	pState->m_sSkillID		= 0;
	pState->m_cStep			= 0;
	pState->m_cSlotIndex	= 0;
	
	pState->m_dwStateStartTime	= g_LocalSystemTime;
	pState->m_bDeleteReqPacket	= false;
	
	m_mapCharacterStateList[m_TotalListCount] = pState;
	m_TotalListCount++;

	// insert condition
	map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectid);
	if(iter_table != g_CharacterStateTable.end())
	{
		_XCharacterStateTable* pSTable = iter_table->second;
		if(pSTable)
		{
			if(pSTable->condition_flag_id > 0)
			{
				InsertConditionState(uid, pSTable->condition_flag_id);
			}
			else
			{
				if(g_CharacterStateTable[effectid]->effectNumber != 0)
				{
					if(uid == g_pLocalUser->m_Uniqueid)
					{
						if(!g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance(g_CharacterStateTable[effectid]->effectNumber))
						{
							D3DXMATRIX offsetmat;
							D3DXMatrixIdentity(&offsetmat);
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								g_CharacterStateTable[effectid]->effectNumber, &offsetmat, NULL, TRUE);
						}
					}
					else
					{
						_XUser* pUser = g_Userlist.FindUser(uid);
						if(pUser)
						{
							if(!pUser->m_ModelDescriptor.m_EffectManager->FindInstance(g_CharacterStateTable[effectid]->effectNumber))
							{
								D3DXMATRIX offsetmat;
								D3DXMatrixIdentity(&offsetmat);
								pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									g_CharacterStateTable[effectid]->effectNumber, &offsetmat, NULL, TRUE);
							}
						} // if(pUser)
					}
				} // if(g_CharacterStateTable[id]->effectNumber != 0)
			}
		}
	}
}

void _XCharacterStateList::InsertStateFromExtra(unsigned short uid, short effectid, char slotindex, unsigned short lefttime, short value)
{
	_XCharacterState* pState = new _XCharacterState;
	
	pState->m_FromType		= en_from_extra;
	
	pState->m_sEffectID		= effectid;
	pState->m_cSlotIndex	= slotindex;

	pState->m_usLeftTimeSec = lefttime;
	pState->m_sValue		= value;
	
	pState->m_sSkillID		= 0;
	pState->m_cStep			= 0;
	
	pState->m_dwStateStartTime	= g_LocalSystemTime;
	pState->m_bDeleteReqPacket	= false;
	
	m_mapCharacterStateList[m_TotalListCount] = pState;
	m_TotalListCount++;
	
	// insert condition
	map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effectid);
	if(iter_table != g_CharacterStateTable.end())
	{
		_XCharacterStateTable* pSTable = iter_table->second;
		if(pSTable)
		{
			if(pSTable->condition_flag_id > 0)
			{
				InsertConditionState(uid, pSTable->condition_flag_id);
			}
			else
			{
				if(g_CharacterStateTable[effectid]->effectNumber != 0)
				{
					if(uid == g_pLocalUser->m_Uniqueid)
					{
						if(!g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance(g_CharacterStateTable[effectid]->effectNumber))
						{
							D3DXMATRIX offsetmat;
							D3DXMatrixIdentity(&offsetmat);
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								g_CharacterStateTable[effectid]->effectNumber, &offsetmat, NULL, TRUE);
						}
					}
					else
					{
						_XUser* pUser = g_Userlist.FindUser(uid);
						if(pUser)
						{
							if(!pUser->m_ModelDescriptor.m_EffectManager->FindInstance(g_CharacterStateTable[effectid]->effectNumber))
							{
								D3DXMATRIX offsetmat;
								D3DXMatrixIdentity(&offsetmat);
								pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									g_CharacterStateTable[effectid]->effectNumber, &offsetmat, NULL, TRUE);
							}
						} // if(pUser)
					}
				} // if(g_CharacterStateTable[id]->effectNumber != 0)
			}
		}
	}
}

void _XCharacterStateList::InsertStateFromPassive(short skillid, char skillstep, int effectindex)
{
	_XCharacterState* pState = new _XCharacterState;
	
	pState->m_FromType		= en_from_passive;
	pState->m_sSkillID		= skillid;
	pState->m_cStep			= skillstep;
	
	pState->m_cSlotIndex	= 0;
	
	pState->m_sEffectID		= g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].id;
	pState->m_sValue		= g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].value;
	
	pState->m_usLeftTimeSec = 0;
	pState->m_dwStateStartTime	= g_LocalSystemTime;
	pState->m_bDeleteReqPacket	= false;
	
	m_mapCharacterStateList[m_TotalListCount] = pState;
	m_TotalListCount++;
}


int _XCharacterStateList::FindState(char fromtype, short effectid, short skillid, char slotindex)
{
	map <int, _XCharacterState*>::iterator iter_state;
	for(iter_state = m_mapCharacterStateList.begin() ; iter_state != m_mapCharacterStateList.end() ; ++iter_state)
	{
		_XCharacterState* currentstate = iter_state->second;
		if(currentstate)
		{
			if(currentstate->m_FromType == fromtype)
			{
				if(currentstate->m_FromType == en_from_skill)
				{
					// skill
					if(currentstate->m_sSkillID == skillid)
					{
						if(currentstate->m_sEffectID == effectid)
						{
							int index = iter_state->first;
							return index;
						}
					}
				}
				else if(currentstate->m_FromType == en_from_effect)
				{
					// effect
					if(currentstate->m_sEffectID == effectid)
					{
						int index = iter_state->first;
						return index;
					}
				}
				else if(currentstate->m_FromType == en_from_extra)
				{
					// item
					if(currentstate->m_cSlotIndex == slotindex)
					{
						int index = iter_state->first;
						return index;
					}
				}
				else if(currentstate->m_FromType == en_from_passive)
				{
					// passive
					if(currentstate->m_sSkillID == skillid)
					{
						if(currentstate->m_sEffectID == effectid)
						{
							int index = iter_state->first;
							return index;
						}
					}
				}
				else if(currentstate->m_FromType == en_from_nickname)
				{
					if( currentstate->m_sEffectID == effectid )
					{
						int index = iter_state->first;
						return index;
					}
				}
				else if(currentstate->m_FromType == en_from_extraskill)
				{
					// extra skill - �������� ���� etc.
					if(currentstate->m_sSkillID == skillid && currentstate->m_cSlotIndex == slotindex)
					{
						if(effectid > 0)
						{
							if(currentstate->m_sEffectID == effectid)
							{
								int index = iter_state->first;
								return index;
							}
						}
						else
						{
							int index = iter_state->first;
							return index;
						}
					}
				}
				else if(currentstate->m_FromType == en_from_specialbuff)
				{
					// skill
					// �ٸ� ȿ�� �ϰ�� ��ų ���̵� �ٸ���찡 �ְ�, ����Ʈ ���̵� �ٸ���..
					//if(currentstate->m_cSlotIndex == slotindex )
					if(currentstate->m_cSlotIndex == slotindex )
					{
						int index = iter_state->first;
						return index;
					}
				}
			}
		}
	}

	return -1;
}

int _XCharacterStateList::FindStateExtraSkill(char fromtype, char slotindex)
{
	map <int, _XCharacterState*>::iterator iter_state;
	for(iter_state = m_mapCharacterStateList.begin() ; iter_state != m_mapCharacterStateList.end() ; ++iter_state)
	{
		_XCharacterState* currentstate = iter_state->second;
		if(currentstate)
		{
			if(currentstate->m_FromType == fromtype)
			{
				if(currentstate->m_FromType == en_from_extraskill)
				{
					if(currentstate->m_cSlotIndex == slotindex)
					{
						return currentstate->m_sSkillID;
					}
				}
			}
		}
	}
	return -1;
}

int _XCharacterStateList::FindStateSpecialBuff(char slotindex, int& skillid, int& skilllevel)
{
	// slot index�� skill id�� ã���ش�.
	map <int, _XCharacterState*>::iterator iter_state;
	for(iter_state = m_mapCharacterStateList.begin() ; iter_state != m_mapCharacterStateList.end() ; ++iter_state)
	{
		_XCharacterState* currentstate = iter_state->second;
		if(currentstate)
		{
			if(currentstate->m_FromType == en_from_specialbuff)
			{
				if(currentstate->m_cSlotIndex == slotindex)
				{
					skillid = currentstate->m_sSkillID;
					skilllevel = currentstate->m_cStep;
					return iter_state->first;
				}
			}
		}
	}
	return -1;
}


void _XCharacterStateList::DeleteState(unsigned short uid, char fromtype, short effectid, short skillid)
{
	int index = FindState(fromtype, effectid, skillid);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				// delete condition
				if(currentstate->m_FromType == en_from_effect)
				{
					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
					if(iter_table != g_CharacterStateTable.end())
					{
						_XCharacterStateTable* pSTable = iter_table->second;
						if(pSTable)
						{
							if(pSTable->condition_flag_id > 0)
							{
								DeleteConditionState(uid, pSTable->condition_flag_id);
							}
							else
							{
								if(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber != 0)
								{
									if(uid == g_pLocalUser->m_Uniqueid)
										g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber);
									else
									{
										_XUser* pUser = g_Userlist.FindUser(uid);
										if(pUser)
										{
											pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber);
										}
									}
								} // if(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber != 0)
							}
						} // if(pSTable)
					}
				} // if(currentstate->m_FromType == en_from_effect)

				SAFE_DELETE(currentstate);
				m_mapCharacterStateList.erase(iter_state);
				return;
			}
		}
	}
}

void _XCharacterStateList::DeleteStateFromSpell(short skillid, char skillstep, int effectindex)
{
	short effectid = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].id;

	int index = FindState(en_from_skill, effectid, skillid);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				SAFE_DELETE(currentstate);
				m_mapCharacterStateList.erase(iter_state);
				return;
			}
		}
	}
}

void _XCharacterStateList::DeleteStateFromEffect(unsigned short uid, short effectid)
{
	int index = FindState(en_from_effect, effectid, 0);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				// delete condition
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* pSTable = iter_table->second;
					if(pSTable)
					{
						if(pSTable->condition_flag_id > 0)
						{
							DeleteConditionState(uid, pSTable->condition_flag_id);
						}
						else
						{
							if(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber != 0)
							{
								if(uid == g_pLocalUser->m_Uniqueid)
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber);
								else
								{
									_XUser* pUser = g_Userlist.FindUser(uid);
									if(pUser)
									{
										pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber);
									}
								}
							} // if(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber != 0)
						}
					} // if(pSTable)
				}
				
				SAFE_DELETE(currentstate);
				m_mapCharacterStateList.erase(iter_state);
				return;
			}
		}
	}
}

void _XCharacterStateList::DeleteStateFromExtra(unsigned short uid, short effectid, char slotindex)
{
	int index = FindState(en_from_extra, effectid, 0, slotindex);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				// delete condition
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* pSTable = iter_table->second;
					if(pSTable)
					{
						if(pSTable->condition_flag_id > 0)
						{
							DeleteConditionState(uid, pSTable->condition_flag_id);
						}
						else
						{
							if(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber != 0)
							{
								if(uid == g_pLocalUser->m_Uniqueid)
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber);
								else
								{
									_XUser* pUser = g_Userlist.FindUser(uid);
									if(pUser)
									{
										pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber);
									}
								}
							} // if(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber != 0)
						}
					} // if(pSTable)
				}
				
				SAFE_DELETE(currentstate);
				m_mapCharacterStateList.erase(iter_state);
				return;
			}
		}
	}
}

void _XCharacterStateList::DeleteStateFromPassive(short skillid, char skillstep, int effectindex)
{
	short effectid = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].id;
	
	int index = FindState(en_from_passive, effectid, skillid);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				SAFE_DELETE(currentstate);
				m_mapCharacterStateList.erase(iter_state);
				return;
			}
		}
	}
}

void _XCharacterStateList::InsertStateFromNicknameEffect(short effectid, short value )
{
	_XCharacterState* pState = new _XCharacterState;
	
	pState->m_FromType		= en_from_nickname;

	pState->m_sEffectID		= effectid;
	pState->m_usLeftTimeSec = 0;
	pState->m_sValue		= value;

	pState->m_sSkillID		= 0;
	pState->m_cStep			= 0;
	pState->m_cSlotIndex	= 0;
	
	pState->m_dwStateStartTime	= g_LocalSystemTime;
	pState->m_bDeleteReqPacket	= false;
	
	m_mapCharacterStateList[m_TotalListCount] = pState;
	m_TotalListCount++;
}

void _XCharacterStateList::UpdateStateFromNicknameEffect(short effectid, short value )
{
	int index = FindState(en_from_nickname, effectid, 0);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				currentstate->m_usLeftTimeSec = 0;
				currentstate->m_sValue = value;
				currentstate->m_cStep = 0;
				currentstate->m_cSlotIndex = 0;
				currentstate->m_dwStateStartTime = g_LocalSystemTime;
			}
		}
	}
	else
	{
		InsertStateFromNicknameEffect(effectid, value);
	}
}

void _XCharacterStateList::DeleteStateFromNicknameEffect(void)
{
	map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.begin();
	while( iter_state != m_mapCharacterStateList.end() )
	{
		_XCharacterState* currentstate = iter_state->second;
		if(currentstate)
		{
			if( currentstate->m_FromType == en_from_nickname )
			{
				SAFE_DELETE(currentstate);
				iter_state = m_mapCharacterStateList.erase(iter_state);
			}		
			else
				iter_state++;
		}
	}
}

BOOL _XCharacterStateList::ReloadCharacterStateTable(void)
{
	map <int, _XCharacterStateTable*>::iterator iter_table;
	for(iter_table = g_CharacterStateTable.begin() ; iter_table != g_CharacterStateTable.end() ; ++iter_table)
	{
		_XCharacterStateTable* currenttable = iter_table->second;
		if(currenttable)
		{
#ifdef _XDEF_NEWSTRINGMANAGER
			currenttable->name		=	g_StringDataBase.GetTextString( _XSTRINGDB_SN_CHARACTERSTATE_XMS_NAME, currenttable->id );
			currenttable->concept	=	g_StringDataBase.GetTextString( _XSTRINGDB_SN_CHARACTERSTATE_XMS_INFO, currenttable->id );
#endif
		}
	}
	
	return TRUE;
}

BOOL _XCharacterStateList::LoadCharacterStateTable(void)
{
	FILE* fileptr = NULL;

	map <int, _XCharacterStateTable*>::iterator iter_table;
	for(iter_table = g_CharacterStateTable.begin() ; iter_table != g_CharacterStateTable.end() ; ++iter_table)
	{
		_XCharacterStateTable* currenttable = iter_table->second;
		if(currenttable)
		{
			SAFE_DELETE(currenttable);
		}
	}
	g_CharacterStateTable.clear();

	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;
	
	fileptr = g_ScriptArchive.GetPackedFile(_T("CharacterState.XMS"));	
	
	if(!fileptr)
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	int count = 0, index = 0;
	BOOL bReadCount = FALSE;
	TCHAR buffer[256];

	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(TCHAR)*256);
		if(!fgets(buffer, 255, fileptr))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;

		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			if(!bReadCount)
			{
				sscanf(buffer, "%d", &count);
				bReadCount = TRUE;
			}
			else
			{
				_XCharacterStateTable* pStateTable = new _XCharacterStateTable;
				memset(pStateTable, 0, sizeof(_XCharacterStateTable));

				sscanf(buffer, "%d", &index);
				pStateTable->id = index;
#ifdef _XDEF_NEWSTRINGMANAGER	
				TCHAR _tempbuffer[256];
				sscanf(buffer, "%d %s %d %d %d %s", &index, _tempbuffer, &pStateTable->type, 
													&pStateTable->iconNumber, &pStateTable->effectNumber, 
													_tempbuffer);

				pStateTable->name		=	g_StringDataBase.GetTextString( _XSTRINGDB_SN_CHARACTERSTATE_XMS_NAME, index );
				pStateTable->concept	=	g_StringDataBase.GetTextString( _XSTRINGDB_SN_CHARACTERSTATE_XMS_INFO, index );

#else
				sscanf(buffer, "%d %s %d %d %d %s", &index, pStateTable->name, &pStateTable->type, &pStateTable->iconNumber,
													&pStateTable->effectNumber, pStateTable->concept);

				ReplaceCharacter(pStateTable->name, _T('_'), _T(' '));
				ReplaceCharacter(pStateTable->concept, _T('_'), _T(' '));
#endif				
				g_CharacterStateTable[index] = pStateTable;
			}
		}
	} while(!feof(fileptr));
	
	// read server script format
	
	if(!LoadEffectScriptServerFormat(g_ScriptArchive))
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	// build condition table
	if(!LoadCharacterConditionTable(g_ScriptArchive))
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	g_ScriptArchive.ClosePackage();
	return TRUE;
}

BOOL _XCharacterStateList::LoadEffectScriptServerFormat(_XPackageArchive& scriptpackage)
{
	FILE* fileptr = NULL;

	fileptr = scriptpackage.GetPackedFile(_T("effect_script.txt"));

	if(!fileptr)
	{
		return FALSE;
	}

	char	cmd[120];
	bool	bEnd = false;
	bool	bReadyDescription = false;
	bool	start_script = false;
	bool	bComplete = false;

	while(1)
	{
		while(GetCommand(cmd, fileptr))
		{
			switch(cmd[0])
			{
			case 'e' :
				{
					if(strncmp(cmd, "effect_descript_where_table", strlen("effect_descript_where_table")) == 0)
					{
						if(!ParseDescriptor(fileptr))
							bEnd = true;
						else
							bReadyDescription = true;
					}
				}
				break;
			case 's' :
				{
					if(strncmp(cmd, "script_start", strlen("script_start")) == 0)
					{
						if(!bReadyDescription)
						{
							bEnd = true;
							bComplete = false;
							break;
						}
						start_script = true;
					}
					else if(strncmp(cmd, "script_end", strlen("script_end")) == 0)
					{
						if(bReadyDescription == false || start_script == false)
						{
							bEnd = true;
							bComplete = false;
						}
						else
						{
							bEnd = true;
							bComplete = true;
						}
					}
					else if(strncmp(cmd, "start", strlen("start")) == 0)
					{
						if(!ParseEffectScript(fileptr))
						{
							bEnd = true;
							bComplete = false;
						}
					}
				}
				break;
			}
			if(bEnd)
				break;
		}
		if(bEnd)
			break;
	}

	ReleaseEffectScriptName();

	return bComplete;
//	return TRUE;
}

BOOL _XCharacterStateList::ParseDescriptor(FILE* fileptr)
{
	char	cmd[120];
	char	arg1[40], arg2[40];

	bool	start = false, end = false;

	while(!feof(fileptr))
	{
		while(GetCommand(cmd, fileptr))
		{
			if(cmd[0] == '{')
			{
				start = true;
				break;
			}
		}
		if(start)
			break;
	}

	if(!start)
		return FALSE;

	while(!end && !feof(fileptr))
	{
		while(GetCommand(cmd, fileptr))
		{
			switch(cmd[0])
			{
			case '}' :
				end = true;
				break;

			default :
				{
					if(sscanf(cmd, "%s %s", arg1, arg2) == 2)
					{
						short where = (short)atoi(arg2);
						InsertEffectScriptName(arg1, where);
					}
					else
						break;
				}
				break;
			}
			if(end)
				break;
		}
	}

	return end;
}

BOOL _XCharacterStateList::ParseEffectScript(FILE* fileptr)
{
	char	cmd[120];
	char	arg1[40], arg2[40], arg3[40];

	bool	end = false;
	unsigned char	temp;

	// 0 : ���� �������� ����
	// 1 : eff name
	// 2 : property
	// 3 : value type
	// 4 : interval time
	// 5 : condition flag id
	int		pos = 0;
	short	eff_id = 0;

	while(!end && !feof(fileptr))
	{
		while(GetCommand(cmd, fileptr, true))
		{
			switch(cmd[0])
			{
			case '{' :
				{
					if(pos == 0)
						pos = 1;
					eff_id = 0;
				}
				break;
			case '}' :
				{
					if(pos == 7)
						pos = 8;
					end = true;
				}
				break;
			default :
				{
					if(pos == 1)		// effect id
					{
						sscanf(cmd, "%s", arg1);
						eff_id = (short)atoi(arg1);

						if(eff_id > 0)
							pos = 2;
						else 
							end = true;
					}
					else if(pos == 2)	// effect name : where
					{
						if( g_CharacterStateTable.find(eff_id) == g_CharacterStateTable.end() )
						{
							_XDWINPRINT( "CharacterState Data Error. [%d]", eff_id );
							pos = 3;
						}
						else
						{
							sscanf(cmd, "%s", arg1);
							temp = GetEffectWhereFromName(arg1);
							g_CharacterStateTable[eff_id]->where = temp;
							pos = 3;
						}						
					}
					else if(pos == 3)	// effect property
					{
						int n = sscanf(cmd, "%s %s %s", arg1, arg2, arg3);
						if(n <= 0)
							end = true;
						else
						{
							if( g_CharacterStateTable.find(eff_id) == g_CharacterStateTable.end() )
							{
								_XDWINPRINT( "CharacterState Data Error. [%d]", eff_id );
								pos = 4;
							}
							else
							{
								temp = GetEffectProperty(arg1);
								g_CharacterStateTable[eff_id]->effect_prop = temp;
								
								switch(g_CharacterStateTable[eff_id]->effect_prop)
								{
								case _XCL_EFFECT_PROP_REFLECTION :
									break;
								case _XCL_EFFECT_PROP_FEED :
									{
										if(n >= 2)
										{
											temp = GetEffectFromID(arg2);
											g_CharacterStateTable[eff_id]->from = temp;
										}
									}
									break;
								}
								pos = 4;
							}							
						}
					}
					else if(pos == 4)	// effect value type
					{
						if(g_CharacterStateTable.find(eff_id) == g_CharacterStateTable.end())
						{
							_XDWINPRINT( "CharacterState Data Error. [%d]", eff_id );
							pos = 5;
						}
						else
						{
							sscanf(cmd, "%s", arg1);
							g_CharacterStateTable[eff_id]->value_type = GetEffectValueType(arg1);
							pos = 5;
						}
					}
					else if(pos == 5)	// interval time
					{
						if(g_CharacterStateTable.find(eff_id) == g_CharacterStateTable.end())
						{
							_XDWINPRINT( "CharacterState Data Error. [%d]", eff_id );
							pos = 6;
						}
						else
						{
							sscanf(cmd, "%s", arg1);
							g_CharacterStateTable[eff_id]->count = (char)atoi(arg1);
							pos = 6;
						}
					}
					else if(pos == 6)	// condition flag id
					{
						if(g_CharacterStateTable.find(eff_id) == g_CharacterStateTable.end())
						{
							_XDWINPRINT( "CharacterState Data Error. [%d]", eff_id );
							pos = 7;
						}
						else
						{
							sscanf(cmd, "%s", arg1);
							g_CharacterStateTable[eff_id]->condition_flag_id = (unsigned short)atoi(arg1);
							pos = 7;
						}
					}
					else
						end = true;
				}
				break;
			}
			if(end)
				break;
		}
	}

	if(pos != 8)
		return FALSE;

	return end;
}

BOOL _XCharacterStateList::GetCommand(char* cmd, FILE* fileptr, bool bnumber)
{
	while(1)
	{
		if(!fgets(cmd, 120, fileptr))
			return FALSE;
		else
		{
			if(cmd[0] == '/' || cmd[0] == '\n' || cmd[0] == ' ')
				continue;

			int i = 0 ;
			int len = strlen(cmd);

			while((!isalnum(cmd[i]) && cmd[i] != '{' && cmd[i] != '}') && i < len)
			{
				i++;
			}
			if(i > 0 && i < len)
			{
				memmove(&cmd[0], &cmd[i], len-i+1);		// ���� '0' ����
			}

			len = strlen(cmd);

			while(len > 0)
			{
				if(cmd[len] == '\n' || cmd[len] == '/')
					cmd[len] = 0;
				--len;
			}

			if(cmd[0] == '{' || cmd[0] == '}' || bnumber || isalpha(cmd[0]))
				return true;
			else 
				return false;
		}
	}
}

void _XCharacterStateList::InsertEffectScriptName(char* eff_name, short eff_Where)
{
	if(eff_name)
	{
		int len = strlen(eff_name);

		str_effect_entry* eff_entry = new str_effect_entry;
		eff_entry->where = eff_Where;

		eff_entry->effectname = new char[len+1];
		strcpy(eff_entry->effectname, eff_name);

		lst_EffectDescription.push_back(eff_entry);
	}
}

void _XCharacterStateList::ReleaseEffectScriptName(void)
{
	list <str_effect_entry*>::iterator iter;
	for(iter = lst_EffectDescription.begin() ; iter != lst_EffectDescription.end() ; ++iter)
	{
		// 2007-01-09 ���� 
		str_effect_entry* currententry = *iter;
		if(currententry)
		{
			if(currententry->effectname)
			{
				SAFE_DELETE_ARRAY(currententry->effectname);
			}
			SAFE_DELETE(currententry);
		}

		/*
		char* eff_name = (*iter)->effectname;
		if(eff_name)
		{
			delete[] eff_name;
			eff_name = NULL;
		}*/
	}
	lst_EffectDescription.clear();
}

unsigned char _XCharacterStateList::GetEffectWhereFromName(char* eff_name)
{
	list <str_effect_entry*>::iterator iter = lst_EffectDescription.begin();

	while(iter != lst_EffectDescription.end())
	{
		if((*iter)->effectname && strcmp((*iter)->effectname, eff_name) == 0)
			return (*iter)->where;
		iter++;
	}
	return 0;
}

unsigned char _XCharacterStateList::GetEffectProperty(char* property)
{
	if(property)
	{
		switch(*property)
		{
		case 'a' :
			{
				if(strcmp(property, "apply_additive") == 0)
					return _XCL_EFFECT_PROP_ADDITIVE;
			}
			break;
		case 'f' :
			{
				if(strcmp(property, "feed") == 0)
					return _XCL_EFFECT_PROP_FEED;
			}
			break;
		case 'g' :
			{
				if(strcmp(property, "general") == 0)
					return _XCL_EFFECT_PROP_GENERAL;
			}
			break;
		case 'i' :
			{
				if(strcmp(property, "interval") == 0)
					return _XCL_EFFECT_PROP_INTERVAL;
				else if(strcmp(property, "initial_once") == 0)
					return _XCL_EFFECT_PROP_INITIAL_ONCE;
			}
			break;
		case 'n' :
			{
				if(strcmp(property, "none") == 0)
					return _XCL_EFFECT_PROP_NONE;
			}
			break;
		case 'o' :
			{
				if(strcmp(property, "only_once") == 0)
					return _XCL_EFFECT_PROP_ONLY_ONCE;
			}
			break;
		case 'r' :
			{
				if(strcmp(property, "reflection") == 0)
					return _XCL_EFFECT_PROP_REFLECTION;
			}
		}
	}
	return 0xff;
}

unsigned char _XCharacterStateList::GetEffectFromID(char* from)
{
	if(strcmp("hit", from) == 0)
		return en_eff_from_hit;
	else if(strcmp("life", from) == 0)
		return en_eff_from_life;
	else if(strcmp("force", from) == 0)
		return en_eff_from_force;
	else if(strcmp("concentration", from) == 0)
		return en_eff_from_concentration;
	else
		return en_eff_from_none;
}

char _XCharacterStateList::GetEffectValueType(char* value)
{
	if(value)
	{
		if(strcmp(value, "sub_value") == 0)
			return en_eff_vt_sub_value;
		else if(strcmp(value, "add_percent") == 0)
			return en_eff_vt_add_percent;
		else if(strcmp(value, "sub_percent") == 0)
			return en_eff_vt_sub_percent;
	}

	return en_eff_vt_add_value;
}

void _XCharacterStateList::ReleaseCharacterStateTable(void)
{
	map <int, _XCharacterStateTable*>::iterator iter_table;
	for(iter_table = g_CharacterStateTable.begin() ; iter_table != g_CharacterStateTable.end() ; ++iter_table)
	{
		_XCharacterStateTable* currenttable = iter_table->second;
		if(currenttable)
		{
			SAFE_DELETE(currenttable);
		}
	}
	g_CharacterStateTable.clear();
}

void _XCharacterStateList::ProcessState(void)
{
	map <int, _XCharacterState*>::iterator iter_state;
	for(iter_state = m_mapCharacterStateList.begin() ; iter_state != m_mapCharacterStateList.end() ; ++iter_state)
	{
		_XCharacterState* pState = iter_state->second;
		if(pState)
		{
			if(pState->m_FromType == en_from_skill || pState->m_FromType == en_from_specialbuff)		// ��ų���� ���� ����Ʈ�� req packet�� ������. 
			{
				if(((g_LocalSystemTime - pState->m_dwStateStartTime) > pState->m_usLeftTimeSec*1000) && !pState->m_bDeleteReqPacket)
				{
					char type = 0;

					if(pState->m_FromType ==  en_from_specialbuff)
					{
						//Author : ����� //breif : ����� ���� Ÿ�Ӿƿ� ����
						type = en_eff_ctrl_rq_timeout_special;
						//type = en_eff_ctrl_rq_timeout_buff;
					}
					else
					{
						if(g_SkillProperty[pState->m_sSkillID]->actionType == _XSI_ACTIONTYPE_BUFFSPELL)
							type = en_eff_ctrl_rq_timeout_buff;
						else if(g_SkillProperty[pState->m_sSkillID]->actionType == _XSI_ACTIONTYPE_HOSTILESPELL)
							type = en_eff_ctrl_rq_timeout_hostile;
					}

					g_NetworkKernel.SendPacket(MSG_NO_EFFECT_CTRL, type, pState->m_sSkillID);
					pState->m_bDeleteReqPacket = true;

					_XDWINPRINT("[CharacterStateList] Spell Delete Request Send ==== [%d/%d/%d]", pState->m_sSkillID, g_LocalSystemTime - pState->m_dwStateStartTime, pState->m_usLeftTimeSec*1000);
				}
			}
		}
	}
}

void _XCharacterStateList::UpdateState(unsigned short uid, short id, unsigned short lefttime, char fromtype, short val, short skillid, char step)
{
	int index = FindState(fromtype, id, skillid);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				currentstate->m_usLeftTimeSec = lefttime;
				currentstate->m_sValue = val;
				currentstate->m_cStep = step;
				currentstate->m_dwStateStartTime = g_LocalSystemTime;
			}
		}
	}
	else
	{
		InsertState(uid, id, lefttime, fromtype, val, skillid, step);
	}
}

void _XCharacterStateList::UpdateStateFromSpell(short skillid, char skillstep, unsigned short lefttime, int effectindex)
{
	short effectid = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].id;

	int index = FindState(en_from_skill, effectid, skillid);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				currentstate->m_usLeftTimeSec = lefttime;
				currentstate->m_cStep = skillstep;
				currentstate->m_sValue = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].value;
				currentstate->m_cSlotIndex = 0;
				currentstate->m_dwStateStartTime = g_LocalSystemTime;
				currentstate->m_bDeleteReqPacket	= false;
			}
		}
	}
	else
	{
		InsertStateFromSpell(skillid, skillstep, lefttime, effectindex);
	}
}

void _XCharacterStateList::UpdateStateFromEffect(unsigned short uid, short effectid, unsigned short lefttime, short value)
{
	int index = FindState(en_from_effect, effectid, 0);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				if( lefttime != 0 ) //Author : ����� //breif : ����ġ�� �ٲ� ��� �ð��� �ٲ��� �ʴ´�,
					currentstate->m_usLeftTimeSec = lefttime;

				currentstate->m_sValue = value;
				currentstate->m_cStep = 0;
				currentstate->m_cSlotIndex = 0;
				currentstate->m_dwStateStartTime = g_LocalSystemTime;
			}
		}
	}
	else
	{
		InsertStateFromEffect(uid, effectid, lefttime, value);
	}
}

void _XCharacterStateList::UpdateStateFromExtra(unsigned short uid, short effectid, char slotindex, unsigned short lefttime, short value)
{
	int index = FindState(en_from_extra, effectid, 0, slotindex);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				currentstate->m_usLeftTimeSec = lefttime;
				currentstate->m_sValue = value;
				currentstate->m_cStep = 0;
				currentstate->m_dwStateStartTime = g_LocalSystemTime;
			}
		}
	}
	else
	{
		InsertStateFromExtra(uid, effectid, slotindex, lefttime, value);
	}
}

// LocalUser ����
void _XCharacterStateList::DeleteAllBuff(void)
{
	map <int, _XCharacterState*>::iterator iter_state;
	for(iter_state = m_mapCharacterStateList.begin() ; iter_state != m_mapCharacterStateList.end() ; )
	{
		_XCharacterState* currentstate = iter_state->second;
		if(currentstate)
		{
			if(currentstate->m_FromType == en_from_skill)
			{
				// skill
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* currenttable = iter_table->second;
					if(currenttable)
					{
						if(currenttable->type == 0)		// buff
						{
							if(g_SkillProperty[currentstate->m_sSkillID]->visualEffect)
								g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[currentstate->m_sSkillID]->visualEffect);
							SAFE_DELETE(currentstate);
							iter_state = m_mapCharacterStateList.erase(iter_state);
						}
						else
							iter_state++;
					}
					else
						iter_state++;
				}
				else 
					iter_state++;
			}
/*			else if(currentstate->m_FromType == 1)
			{
				// Effect
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* currenttable = iter_table->second;
					if(currenttable)
					{
						if(currenttable->type == 0)		// buff
						{
							// effect ����
							if(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber != 0)
								g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber);							
							SAFE_DELETE(currentstate);							
							iter_state = m_mapCharacterStateList.erase(iter_state);
						}
						else
							iter_state++;
					}
					else
						iter_state++;
				}
				else
					iter_state++;
			}*/
			else
				iter_state++;
		}
		else
			iter_state++;
	}
}


// LocalUser ����
void _XCharacterStateList::DeleteAllHostile(void)
{
	map <int, _XCharacterState*>::iterator iter_state;
	for(iter_state = m_mapCharacterStateList.begin() ; iter_state != m_mapCharacterStateList.end() ; )
	{
		_XCharacterState* currentstate = iter_state->second;
		if(currentstate)
		{
			if(currentstate->m_FromType == en_from_skill)
			{
				// skill
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* currenttable = iter_table->second;
					if(currenttable)
					{
						if(currenttable->type == 1)		// hostile
						{
							if(g_SkillProperty[currentstate->m_sSkillID]->visualEffect)
								g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[currentstate->m_sSkillID]->visualEffect);
							SAFE_DELETE(currentstate);
							iter_state = m_mapCharacterStateList.erase(iter_state);
						}
						else
							iter_state++;
					}
					else
						iter_state++;
				}
				else 
					iter_state++;
			}
/*			else if(currentstate->m_FromType == 1)
			{
				// Effect
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* currenttable = iter_table->second;
					if(currenttable)
					{
						if(currenttable->type == 1)		// buff
						{
							// effect ����
							if(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber != 0)
								g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber);							
							SAFE_DELETE(currentstate);							
							iter_state = m_mapCharacterStateList.erase(iter_state);
						}
						else
							iter_state++;
					}
					else
						iter_state++;
				}
				else
					iter_state++;
			}*/
			else
				iter_state++;
		}
		else
			iter_state++;
	}
}

// ����
void _XCharacterStateList::DeleteAllEffect(unsigned short uid)
{
	map <int, _XCharacterState*>::iterator iter_state;
	for(iter_state = m_mapCharacterStateList.begin() ; iter_state != m_mapCharacterStateList.end() ; )
	{
		_XCharacterState* currentstate = iter_state->second;
		if(currentstate)
		{
			short id = currentstate->m_sEffectID;
			if(currentstate->m_FromType == en_from_effect)		// effect(condition)
			{
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* pSTable = iter_table->second;
					if(pSTable)
					{
						if(g_CharacterStateTable[id]->condition_flag_id > 0)
							DeleteConditionState(uid, g_CharacterStateTable[id]->condition_flag_id);
						else
						{
							if(g_CharacterStateTable[id]->effectNumber != 0)
							{
								if(uid == g_pLocalUser->m_Uniqueid)
									g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterStateTable[id]->effectNumber);
								else
								{
									_XUser* pUser = g_Userlist.FindUser(uid);
									if(pUser)
									{
										pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterStateTable[id]->effectNumber);
									}
								}
							}
						}
					}					
				}
				SAFE_DELETE(currentstate);
				iter_state = m_mapCharacterStateList.erase(iter_state);
			}
			else
			{
				iter_state++;
			}
		}
		else
		{
			iter_state++;
		}
	}
}

// ����
void _XCharacterStateList::DeleteAllSkill(unsigned short uid)
{
	map <int, _XCharacterState*>::iterator iter_state;
	for(iter_state = m_mapCharacterStateList.begin() ; iter_state != m_mapCharacterStateList.end() ; )
	{
		_XCharacterState* currentstate = iter_state->second;
		if(currentstate)
		{
			short id = currentstate->m_sEffectID;
			if(currentstate->m_FromType == en_from_skill)		// skill
			{
				// effect ����
				if(g_SkillProperty[currentstate->m_sSkillID]->visualEffect > 0)
				{
					if(uid == g_pLocalUser->m_Uniqueid)
						g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[currentstate->m_sSkillID]->visualEffect);
					else
					{
						_XUser* pUser = g_Userlist.FindUser(uid);
						if(pUser)
						{
							pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[currentstate->m_sSkillID]->visualEffect);
						}
					}
				}
				SAFE_DELETE(currentstate);
				iter_state = m_mapCharacterStateList.erase(iter_state);
			}
			else
			{
				iter_state++;
			}
		}
		else
		{
			iter_state++;
		}
	}
}

// LocalUser��
void _XCharacterStateList::DeleteAll(void)
{
	// ��� ����ȿ�� ����, Result, Effect, icon ��� ����
	map <int, _XCharacterState*>::iterator iter_state;
	for(iter_state = m_mapCharacterStateList.begin() ; iter_state != m_mapCharacterStateList.end() ; )
	{
		_XCharacterState* currentstate = iter_state->second;

		if(currentstate)
		{
			if(currentstate->m_FromType == en_from_skill)
			{
				// skill
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* currenttable = iter_table->second;
					if(currenttable)
					{
						if(g_SkillProperty[currentstate->m_sSkillID]->visualEffect)
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[currentstate->m_sSkillID]->visualEffect);
						SAFE_DELETE(currentstate);
						iter_state = m_mapCharacterStateList.erase(iter_state);
					}
					else
						iter_state++;
				}
				else 
					iter_state++;
			}
			else if(currentstate->m_FromType == en_from_effect)
			{
				// Effect
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* currenttable = iter_table->second;
					if(currenttable)
					{
						// effect ����
						if(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber != 0)
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber);							
						SAFE_DELETE(currentstate);							
						iter_state = m_mapCharacterStateList.erase(iter_state);
					}
					else
						iter_state++;
				}
				else
					iter_state++;
			}
			else if(currentstate->m_FromType == en_from_extra)
			{
				// Extra
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* currenttable = iter_table->second;
					if(currenttable)
					{
						// extra ����
						if(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber != 0)
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterStateTable[currentstate->m_sEffectID]->effectNumber);							
						SAFE_DELETE(currentstate);							
						iter_state = m_mapCharacterStateList.erase(iter_state);
					}
					else
						iter_state++;
				}
				else
					iter_state++;
			}
			else if(currentstate->m_FromType == en_from_passive)
			{
				// passive
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* currenttable = iter_table->second;
					if(currenttable)
					{
						if(g_SkillProperty[currentstate->m_sSkillID]->visualEffect)
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[currentstate->m_sSkillID]->visualEffect);
						SAFE_DELETE(currentstate);
						iter_state = m_mapCharacterStateList.erase(iter_state);
					}
					else
						iter_state++;
				}
				else 
					iter_state++;
			}
			else if(currentstate->m_FromType == en_from_extraskill)
			{
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* currenttable = iter_table->second;
					if(currenttable)
					{
						if(g_SkillProperty[currentstate->m_sSkillID]->visualEffect)
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[currentstate->m_sSkillID]->visualEffect);
						SAFE_DELETE(currentstate);
						iter_state = m_mapCharacterStateList.erase(iter_state);
					}
					else
						iter_state++;
				}
				else 
					iter_state++;
			}
			else if(currentstate->m_FromType == en_from_specialbuff)
			{
				map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
				if(iter_table != g_CharacterStateTable.end())
				{
					_XCharacterStateTable* currenttable = iter_table->second;
					if(currenttable)
					{
						if(g_SkillProperty[currentstate->m_sSkillID]->visualEffect)
							g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[currentstate->m_sSkillID]->visualEffect);
						SAFE_DELETE(currentstate);
						iter_state = m_mapCharacterStateList.erase(iter_state);
					}
					else
						iter_state++;
				}
				else 
					iter_state++;
			}
			else
				iter_state++;
		}
	}

	// icon ����
	if(g_pState_Window)
	{
		g_pState_Window->DeleteAllStateList();
	}
}

BOOL _XCharacterStateList::LoadCharacterConditionTable(_XPackageArchive& scriptpackage)
{
	FILE* fileptr = NULL;

	fileptr = scriptpackage.GetPackedFile(_T("CharacterCondition.XMS"));

	if(!fileptr)
	{
		_XFatalError( "Can't load character condition table" );
		return FALSE;
	}

	TCHAR	buffer[256], arg[64];
	int		index = 1;
	int		tempValue = 0;
	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(buffer));
		if(!fgets(buffer, 255, fileptr))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;

		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			sscanf(buffer, "%s", arg);
			if( strcmp(arg, _T("_XM_INDEX")) == 0 )
			{
				sscanf(buffer, "%s %d", arg, &index);
				g_CharacterConditionTable[index-1].id = index;
			}
			else if( strcmp(arg, _T("_XM_NAME")) == 0 )
			{
#ifdef _XDEF_NEWSTRINGMANAGER
				g_CharacterConditionTable[index-1].name = g_StringDataBase.GetTextString( _XSTRINGDB_SN_CHARACTERCONDITION_XMS_NAME, index );
#else
				sscanf(buffer, "%s %s", arg, g_CharacterConditionTable[index-1].name);
#endif
			}
			else if( strcmp(arg, _T("_XM_EFFECT")) == 0 )
			{
				for(int i = 0 ; i < 5 ; ++i)
				{
					g_CharacterConditionTable[index-1].visualEffect[i] = -1;
				}
				sscanf(buffer, "%s %d %d %d %d %d", arg, &g_CharacterConditionTable[index-1].visualEffect[0],
					&g_CharacterConditionTable[index-1].visualEffect[1], &g_CharacterConditionTable[index-1].visualEffect[2], 
					&g_CharacterConditionTable[index-1].visualEffect[3], &g_CharacterConditionTable[index-1].visualEffect[4]);
			}
			else if( strcmp(arg, _T("_XM_MOVELOCK")) == 0 )
			{
				sscanf(buffer, "%s %d", arg, &tempValue);
				g_CharacterConditionTable[index-1].movelock = tempValue;				
			}
			else if( strcmp(arg, _T("_XM_TRADELOCK")) == 0 )
			{
				sscanf(buffer, "%s %d", arg, &tempValue);
				g_CharacterConditionTable[index-1].tradelock = tempValue;
			}
			else if( strcmp(arg, _T("_XM_BATTLELOCK")) == 0 )
			{
				sscanf(buffer, "%s %d", arg, &tempValue);
				g_CharacterConditionTable[index-1].battlelock = tempValue;
			}
			else if( strcmp(arg, _T("_XM_SKILLLOCK")) == 0 )
			{
				sscanf(buffer, "%s %d", arg, &tempValue);
				g_CharacterConditionTable[index-1].skilllock = tempValue;
			}
			else if( strcmp(arg, _T("_XM_MEDLOCK")) == 0 )
			{
				sscanf(buffer, "%s %d", arg, &tempValue);
				g_CharacterConditionTable[index-1].medlock = tempValue;
			}
			else if( strcmp(arg, _T("_XM_SPELLLOCK")) == 0 )
			{
				sscanf(buffer, "%s %d", arg, &tempValue);
				g_CharacterConditionTable[index-1].spelllock = tempValue;
			}
			else if( strcmp(arg, _T("_XM_SPEEDLOCK")) == 0 )
			{
				sscanf(buffer, "%s %d", arg, &tempValue);
				g_CharacterConditionTable[index-1].speedlock = tempValue;
			}
			else if( strcmp(arg, _T("_XM_PARTYLOCK")) == 0 )
			{
				sscanf(buffer, "%s %d", arg, &tempValue);
				g_CharacterConditionTable[index-1].partylock = tempValue;
			}
			else if( strcmp(arg, _T("_XM_CHATLOCK")) == 0 )			
			{
				sscanf(buffer, "%s %d", arg, &tempValue);
				g_CharacterConditionTable[index-1].chattinglock = tempValue;
			}
			else if( strcmp(arg, _T("_XM_MODELOCK")) == 0 )			
			{
				sscanf(buffer, "%s %d", arg, &tempValue);
				g_CharacterConditionTable[index-1].modelock = tempValue;
			}			
			else if( strcmp(arg, _T("_XM_SOUND")) == 0 )
			{
				TCHAR soundfilename[5][32];
				memset(soundfilename, 0, sizeof(soundfilename));
				
				sscanf(buffer, "%s %d %s %d %s %d %s %d %s %d %s", arg, &g_CharacterConditionTable[index-1].soundprop[0], soundfilename[0],
					&g_CharacterConditionTable[index-1].soundprop[1], soundfilename[1],
					&g_CharacterConditionTable[index-1].soundprop[2], soundfilename[2],
					&g_CharacterConditionTable[index-1].soundprop[3], soundfilename[3],
					&g_CharacterConditionTable[index-1].soundprop[4], soundfilename[4]);
				
				// �ΰ� ó�� �ʿ�
				for(int i = 0 ; i < 5 ; ++i)
				{
					g_CharacterConditionTable[index-1].soundindex[i] = -1;
				}
			}
			else if( strcmp(arg, _T("_XM_CONCEPT")) == 0 )
			{
#ifdef _XDEF_NEWSTRINGMANAGER
				g_CharacterConditionTable[index-1].concept = g_StringDataBase.GetTextString( _XSTRINGDB_SN_CHARACTERCONDITION_XMS_CONCEPT, index );
#else
				sscanf(buffer, "%s %s", arg, g_CharacterConditionTable[index-1].concept);
				ReplaceCharacter(g_CharacterConditionTable[index-1].concept, _T('_'), _T(' '));
#endif
			}
		}
	} while(!feof(fileptr));

	return TRUE;
}

void _XCharacterStateList::InsertConditionState(unsigned short uid, int index)
{
	DWORD conditionstate = 0;
	conditionstate = 0x00000001<<(index-1); 

	_XDWINPRINT("Before insert : %d ", g_pLocalUser->m_CharacterInfo.conditionState);
	_XDWINPRINT("insert index : %d", index);

	if(uid == g_pLocalUser->m_Uniqueid)
	{
		g_pLocalUser->m_CharacterInfo.conditionState |= conditionstate;

		_XDWINPRINT("After insert : %d ", g_pLocalUser->m_CharacterInfo.conditionState);

		// lock ó��
		if(g_pLocalUser->m_Lock.movelock != 1)
			g_pLocalUser->m_Lock.movelock = g_CharacterConditionTable[index-1].movelock;
		if(g_pLocalUser->m_Lock.tradelock != 1)
			g_pLocalUser->m_Lock.tradelock = g_CharacterConditionTable[index-1].tradelock;
		if(g_pLocalUser->m_Lock.battlelock != 1)
			g_pLocalUser->m_Lock.battlelock = g_CharacterConditionTable[index-1].battlelock;
		if(g_pLocalUser->m_Lock.skilllock != 1)
			g_pLocalUser->m_Lock.skilllock = g_CharacterConditionTable[index-1].skilllock;
		if(g_pLocalUser->m_Lock.medlock != 1)
			g_pLocalUser->m_Lock.medlock = g_CharacterConditionTable[index-1].medlock;
		if(g_pLocalUser->m_Lock.spelllock != 1)
			g_pLocalUser->m_Lock.spelllock = g_CharacterConditionTable[index-1].spelllock;
		if(g_pLocalUser->m_Lock.speedlock != 1)
			g_pLocalUser->m_Lock.speedlock = g_CharacterConditionTable[index-1].speedlock;
		if(g_pLocalUser->m_Lock.chatlock != 1)
			g_pLocalUser->m_Lock.chatlock = g_CharacterConditionTable[index-1].chattinglock;
		if(g_pLocalUser->m_Lock.modelock != 1)
			g_pLocalUser->m_Lock.modelock = g_CharacterConditionTable[index-1].modelock;
		// sound ó��
		
		// animation ó��
		if(index == 1)
		{
			// ���� - ������ Animation���� ���� - ���� �ִϸ��̼����� �ٸ� mode �����
			g_pLocalUser->SetAnimation(_XACTION_INSENSIBLE);
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
		
		if(g_pLocalUser->m_Lock.battlelock == 1)
		{
			g_pLocalUser->ResetSkillAnimation();
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
		
		// �̵� �ӵ� ���� ó��
		if(index == 5 || /*index == 16 ||*/ index == 17 || index == 19 || index == 28)
		{
			// ����(5), ����(16), ����(17), ��ȭ�Ը�(19), ������(28) �����϶� �̵��ӵ� 30% ����
			g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_STATE_2598) , _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "���� �̻����� ���Ͽ� �̵��ӵ��� �����Ͽ����ϴ�."
		}
		
		// camera ó��
		if(index == 1 || index == 2 || index == 12)
		{
			// ����(1), ����(2), ����(12) - ī�޶� ȸ�� �Ұ���
			g_pLocalUser->m_Lock.camerarotatelock = 1;
		}
		if(index == 1 || index == 12)
		{
			// ����(1), ����(12) - ī�޶� ����/�ƿ� �Ұ���
			g_pLocalUser->m_Lock.camerazoomlock = 1;
		}
		if(index == 9)
		{
			// ȥ��(9) - ī�޶� ���������� ����
			g_pLocalUser->m_Lock.camerareverse = 1;
		}
		if(index == 17)
		{
			// ����(17) - ���� delay 200% ����
			g_pLocalUser->m_Lock.adddelaytime = 1;
		}
		if(index == 13)
		{
			// �Ǹ�(13) - �ֺ� ����� id�� �ĺ����� ����
			g_pLocalUser->m_Lock.notdrawusername = 1;
		}

		if(index == 5 || index == 12 || /*index == 15 || index == 16 ||*/ index == 17 || index == 19 || index == 28)
		{
			// ����(5), ����(12), ����(15), ����(16), ����(17), ��ȭ�Ը�(19), ������(28) �����϶� Parameter �ٽ� ���
			g_pLocalUser->UpdateLocalUserAttackLevel();
		}
		if( index == 14 )	
		{
			// ���� 
			g_pLocalUser->m_bFaceMaskMode = TRUE;
			ChangeModel(0, 0, 1);
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
									_XDEF_MONSTERID_TM252, &g_pLocalUser->m_ModelDescriptor.m_Position );
			_XPlayInterfaceSound(ID_SR_INTERFACE_MASK_ON);
		}

		// visual effect ó��
		for(int i = 0 ; i < 5 ; ++i)
		{
			if(g_CharacterConditionTable[index-1].visualEffect[i] > 0)
			{
				D3DXMATRIX offsetmat;
				D3DXMatrixIdentity(&offsetmat);
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
					g_CharacterConditionTable[index-1].visualEffect[i], &offsetmat);
			}
		}
	}
	else
	{
		_XUser* pUser = g_Userlist.FindUser(uid);
		if(pUser)
		{
			pUser->m_CharacterInfo.conditionState |= conditionstate;

			// visual effect ó��
			for(int i = 0 ; i < 5 ; ++i)
			{
				if(g_CharacterConditionTable[index-1].visualEffect[i] > 0)
				{
					D3DXMATRIX offsetmat;
					D3DXMatrixIdentity(&offsetmat);
					pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
						g_CharacterConditionTable[index-1].visualEffect[i], &offsetmat);
				}
			}

			if(/*index == 16 ||*/ index == 29)
			{
				// ����(16) / GM ����(29)
				pUser->Set_m_bInvisible(TRUE);
			}
			if( index == 14 )
			{
				// ����
				if(!pUser->m_bFaceMaskMode)
				{
					pUser->m_bFaceMaskMode = TRUE;
					pUser->SetCharacterModel(TRUE);
					if(pUser->m_ModelDescriptor.m_EffectManager)
					{
						pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
							_XDEF_MONSTERID_TM252, &pUser->m_ModelDescriptor.m_Position );
					}
				}
			}
		}
	}
}

// extra slot�̸鼭 skill id�� ����ϴ� ��� - ���� ���� ���� ���� ��
void _XCharacterStateList::InsertStateFromExtraSkill(unsigned short uid, short skillid, char skillstep, char slotindex, unsigned short lefttime, int effectindex)
{
	_XCharacterState* pState = new _XCharacterState;

	pState->m_FromType = en_from_extraskill;
	pState->m_sSkillID = skillid;
	pState->m_cStep = skillstep;

	pState->m_cSlotIndex = slotindex;

	pState->m_sEffectID = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].id;
	pState->m_sValue = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].value;

	pState->m_usLeftTimeSec = lefttime;
	pState->m_dwStateStartTime = g_LocalSystemTime;
	pState->m_bDeleteReqPacket = false;

	m_mapCharacterStateList[m_TotalListCount] = pState;
	m_TotalListCount++;
}

void _XCharacterStateList::UpdateStateFromExtraSkill(unsigned short uid, short skillid, char skillstep, char slotindex, unsigned short lefttime, int effectindex)
{
	short effectid = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].id;

	int index = FindState(en_from_extraskill, effectid, skillid, slotindex);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				currentstate->m_usLeftTimeSec = lefttime;
				currentstate->m_cStep = skillstep;
				currentstate->m_sValue = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].value;
				currentstate->m_cSlotIndex = slotindex;
				currentstate->m_dwStateStartTime = g_LocalSystemTime;
				currentstate->m_bDeleteReqPacket = false;
			}
		}
	}
	else
	{
		InsertStateFromExtraSkill(uid, skillid, skillstep, slotindex, lefttime, effectindex);
	}
}

void _XCharacterStateList::DeleteStateFromExtraSkill(unsigned short uid, short skillid, char skillstep, int effectindex,char slotindex)
{
	short effectid = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].id;

	int index = FindState(en_from_extraskill, effectid, skillid, slotindex);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				SAFE_DELETE(currentstate);
				m_mapCharacterStateList.erase(iter_state);
				return;
			}
		}
	}
}

// special buff - ������/Pc�� ���� ��
void _XCharacterStateList::InsertStateFromSpecialBuff(unsigned short uid, short skillid, char skillstep, char slotindex, unsigned short lefttime, int effectindex)
{
	_XCharacterState* pState = new _XCharacterState;

	pState->m_FromType = en_from_specialbuff;
	pState->m_sSkillID = skillid;
	pState->m_cStep = skillstep;

	pState->m_cSlotIndex = slotindex;

	pState->m_sEffectID = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].id;
	pState->m_sValue = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].value;

	pState->m_usLeftTimeSec = lefttime;
	pState->m_dwStateStartTime = g_LocalSystemTime;
	pState->m_bDeleteReqPacket = false;

	m_mapCharacterStateList[m_TotalListCount] = pState;
	m_TotalListCount++;
}

void _XCharacterStateList::UpdateStateFromSpecialBuff(unsigned short uid, short skillid, char skillstep, char slotindex, unsigned short lefttime, int effectindex)
{
	short effectid = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].id;

	int index = FindState(en_from_specialbuff, effectid, skillid, slotindex);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				currentstate->m_usLeftTimeSec = lefttime;
				currentstate->m_sSkillID = skillid;
				currentstate->m_cStep = skillstep;
				currentstate->m_sEffectID = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].id;
				currentstate->m_sValue = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].value;
				currentstate->m_cSlotIndex = slotindex;
				currentstate->m_dwStateStartTime = g_LocalSystemTime;
				currentstate->m_bDeleteReqPacket = false;
			}
		}
	}
	else // ����Ʈ �ε����� ã�� ���Ͽ� �߰����ָ� �����ִ� �ε��� �ܿ� �� �߰��ϴ� ��..
	{
		InsertStateFromSpecialBuff(uid, skillid, skillstep, slotindex, lefttime, effectindex);
	}
}

void _XCharacterStateList::DeleteStateFromSpecialBuff(unsigned short uid, short skillid, char skillstep, int effectindex,char slotindex)
{
	short effectid = g_SkillProperty[skillid]->detailInfo[skillstep].effect[effectindex].id;

	int index = FindState(en_from_specialbuff, effectid, skillid, slotindex);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_mapCharacterStateList.find(index);
		if(iter_state != m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				SAFE_DELETE(currentstate);
				m_mapCharacterStateList.erase(iter_state);
				return;
			}
		}
	}
}

void _XCharacterStateList::DeleteConditionState(unsigned short uid, int index)
{
	DWORD conditionstate = 0;
	conditionstate = 0x00000001<<(index-1);

	if(uid == g_pLocalUser->m_Uniqueid)
	{
		if(g_pLocalUser->m_CharacterInfo.conditionState == 0)
		{
			// delete�� condition ���� - �׳� ������
			return;
		}

		g_pLocalUser->m_CharacterInfo.conditionState ^= conditionstate;

		// Delete�Ǵ� condition ó��
		if(index == 1)
		{
			// ���� condition ����
			if(g_pLocalUser->GetMotionClass() != _XACTION_DYING && g_pLocalUser->GetMotionClass() != _XACTION_DIE && g_pLocalUser->GetMotionClass() != _XACTION_REBIRTH)
			{
				g_pLocalUser->SetAnimation(_XACTION_IDLE);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		if(index == 14)
		{
			g_pLocalUser->m_bFaceMaskMode = FALSE;
			ChangeModel(0, 0, 1);
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
									_XDEF_MONSTERID_TM253, &g_pLocalUser->m_ModelDescriptor.m_Position );
			_XPlayInterfaceSound(ID_SR_INTERFACE_MASK_OFF);
		}

		// lock ó��
//		memset(&g_pLocalUser->m_Lock, 0, sizeof(g_pLocalUser->m_Lock));
		g_pLocalUser->ResetLock();
		g_pLocalUser->Set_m_MoveSpeedFromCondition(0);
		
		DWORD result = 0;
		for(int i = 1 ; i < 33 ; ++i)
		{
			conditionstate = g_pLocalUser->m_CharacterInfo.conditionState>>(i-1);
			result = conditionstate & 0x00000001;
			if(result == 1)
			{
				if(g_pLocalUser->m_Lock.movelock != 1)
					g_pLocalUser->m_Lock.movelock = g_CharacterConditionTable[i-1].movelock;
				if(g_pLocalUser->m_Lock.tradelock != 1)
					g_pLocalUser->m_Lock.tradelock = g_CharacterConditionTable[i-1].tradelock;
				if(g_pLocalUser->m_Lock.battlelock != 1)
					g_pLocalUser->m_Lock.battlelock = g_CharacterConditionTable[i-1].battlelock;
				if(g_pLocalUser->m_Lock.skilllock != 1)
					g_pLocalUser->m_Lock.skilllock = g_CharacterConditionTable[i-1].skilllock;
				if(g_pLocalUser->m_Lock.medlock != 1)
					g_pLocalUser->m_Lock.medlock = g_CharacterConditionTable[i-1].medlock;
				if(g_pLocalUser->m_Lock.spelllock != 1)
					g_pLocalUser->m_Lock.spelllock = g_CharacterConditionTable[i-1].spelllock;
				if(g_pLocalUser->m_Lock.speedlock != 1)
					g_pLocalUser->m_Lock.speedlock = g_CharacterConditionTable[i-1].speedlock;
				if(g_pLocalUser->m_Lock.chatlock != 1)
					g_pLocalUser->m_Lock.chatlock = g_CharacterConditionTable[i-1].chattinglock;
				if(g_pLocalUser->m_Lock.modelock != 1)
					g_pLocalUser->m_Lock.modelock = g_CharacterConditionTable[i-1].modelock;
				
				// �̵� �ӵ� ���� ó��
				if(i == 5 || /*i == 16 ||*/ i == 17 || i == 19 || i == 28)
				{
					// ����(5), ����(16), ����(17), ��ȭ�Ը�(19), ������(28) �����϶� �̵��ӵ� 30% ����
					//g_pLocalUser->m_MoveSpeedFromCondition -= 30;
					g_pLocalUser->Set_m_MoveSpeedFromCondition( g_pLocalUser->Get_m_MoveSpeedFromCondition() - 30 );
				}
				
				// camera ó��
				if(i == 1 || i == 2 || i == 12)
				{
					// ����(1), ����(2), ����(12) - ī�޶� ȸ�� �Ұ���
					g_pLocalUser->m_Lock.camerarotatelock = 1;
				}
				if(i == 1 || i == 12)
				{
					// ����(1), ����(12) - ī�޶� ����/�ƿ� �Ұ���
					g_pLocalUser->m_Lock.camerazoomlock = 1;
				}
				if(i == 9)
				{
					// ȥ��(9) - ī�޶� ���������� ����
					g_pLocalUser->m_Lock.camerareverse = 1;
				}
				if(i == 17)
				{
					// ����(17) - ���� delay 200% ����
					g_pLocalUser->m_Lock.adddelaytime = 1;
				}
				if(i == 13)
				{
					// �Ǹ�(13) - �ֺ� ����� id�� �ĺ����� ����
					g_pLocalUser->m_Lock.notdrawusername = 1;
				}
			}
		}
		
		if(index == 5 || index == 12 || /*index == 15 || index == 16 ||*/ index == 17 || index == 19 || index == 28)
		{
			// ����(5), ����(12), ����(15), ����(16), ����(17), ��ȭ�Ը�(19), ������(28) �����϶� Parameter �ٽ� ���
			g_pLocalUser->UpdateLocalUserAttackLevel();
		}
		
		// sound ó��
		
		// animation ó��

		// visual effect ó��
		for(i = 0 ; i < 5 ; ++i)
		{
			if(g_CharacterConditionTable[index-1].visualEffect[i] > 0)
			{
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterConditionTable[index-1].visualEffect[i]);
			}
		}
	}
	else
	{
		_XUser* pUser = g_Userlist.FindUser(uid);
		if(pUser)
		{
			pUser->m_CharacterInfo.conditionState ^= conditionstate;

			// visual effect ó��
			for(int i = 0 ; i < 5 ; ++i)
			{
				if(g_CharacterConditionTable[index-1].visualEffect[i] > 0)
				{
					pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterConditionTable[index-1].visualEffect[i]);
				}
			}

			if(/*index == 16 ||*/index == 29)
			{
				// ����(16), GM����(29)
				pUser->Set_m_bInvisible( FALSE );
			}
			if( index == 14 )
			{
				// ����(14)
				pUser->m_bFaceMaskMode = FALSE;
				pUser->SetCharacterModel(TRUE);
				if(pUser->m_ModelDescriptor.m_EffectManager)
				{
					pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
						_XDEF_MONSTERID_TM253, &pUser->m_ModelDescriptor.m_Position );
				}
			}
		}
	}
}

BOOL _XCharacterStateList::FindConditionState(unsigned short uid, int index)
{
	DWORD conditionstate = 0;

	if(uid == g_pLocalUser->m_Uniqueid)
		conditionstate = g_pLocalUser->m_CharacterInfo.conditionState>>(index-1);
	else
	{
		_XUser* pUser = g_Userlist.FindUser(uid);
		if(pUser)
		{
			conditionstate = pUser->m_CharacterInfo.conditionState>>(index-1);
		}
	}
	
	DWORD result = conditionstate & 0x00000001;
	if(result == 1)
		return TRUE;
	else
		return FALSE;
}

void _XCharacterStateList::ApplyBonusPoint(void)
{
	// Skill, Effect
	map <int, _XCharacterState*>::iterator iter_state;
	for(iter_state = m_mapCharacterStateList.begin() ; iter_state != m_mapCharacterStateList.end() ; ++iter_state)
	{
		_XCharacterState* currentstate = iter_state->second;
		
		map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
		if(iter_table != g_CharacterStateTable.end())
		{
			if( g_CharacterStateTable[currentstate->m_sEffectID]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
				g_CharacterStateTable[currentstate->m_sEffectID]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE ||
				g_CharacterStateTable[currentstate->m_sEffectID]->effect_prop == _XCL_EFFECT_PROP_FEED)
			{
				switch(g_CharacterStateTable[currentstate->m_sEffectID]->where)
				{
				case en_eff_where_strength :		// �ٷ�
					{
						AddStrengthBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					}
					break;
				case en_eff_where_zen :				// ����
					{
						AddZenBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					}
					break;
				case en_eff_where_intelligence :	// ����
					{
						AddIntBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					}
					break;
				case en_eff_where_constitution :	// �ǰ�
					{
						AddConstitutionBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					}
					break;
				case en_eff_where_dexterity :		// ��ø
					{
						AddDexBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					}
					break;
				case en_eff_where_all_stat :
					{
						AddStrengthBonus(currentstate->m_sEffectID, currentstate->m_sValue);
						AddZenBonus(currentstate->m_sEffectID, currentstate->m_sValue);
						AddIntBonus(currentstate->m_sEffectID, currentstate->m_sValue);
						AddConstitutionBonus(currentstate->m_sEffectID, currentstate->m_sValue);
						AddDexBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					}
					break;
				}
			} // if(g_CharacterStateTable[effectid]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
		}
	} // for(stateList)

	// Condition
	short applyStrengthBonus = 0, applyZenBonus = 0, applyIntBonus = 0, applyConstitutionBonus = 0, applyDexBonus = 0;
	if(FindConditionState(g_pLocalUser->m_Uniqueid, 12))
	{
		// ���� - ��� parameter 20% ����
		applyStrengthBonus -= 20;
		applyZenBonus -= 20;
		applyIntBonus -= 20;
		applyConstitutionBonus -= 20;
		applyDexBonus -= 20;
	}
	if(FindConditionState(g_pLocalUser->m_Uniqueid, 19))
	{
		// ��ȭ�Ը� - ��� parameter 20% ����
		applyStrengthBonus -= 20;
		applyZenBonus -= 20;
		applyIntBonus -= 20;
		applyConstitutionBonus -= 20;
		applyDexBonus -= 20;
	}
	if(FindConditionState(g_pLocalUser->m_Uniqueid, 28))
	{
		// ������ - ��� parameter 30% ����
		applyStrengthBonus -= 30;
		applyZenBonus -= 30;
		applyIntBonus -= 30;
		applyConstitutionBonus -= 30;
		applyDexBonus -= 30;
	}
	
	// �ּ� �ִ밪 ����
	if(applyStrengthBonus > 100)
		applyStrengthBonus = 100;
	else if(applyStrengthBonus < -80)
		applyStrengthBonus = -80;

	if(applyZenBonus > 100)
		applyZenBonus = 100;
	else if(applyZenBonus < -80)
		applyZenBonus = -80;

	if(applyIntBonus > 100)
		applyIntBonus = 100;
	else if(applyIntBonus < -80)
		applyIntBonus = -80;

	if(applyConstitutionBonus > 100)
		applyConstitutionBonus = 100;
	else if(applyConstitutionBonus < -80)
		applyConstitutionBonus = -80;

	if(applyDexBonus > 100)
		applyDexBonus = 100;
	else if(applyDexBonus < -80)
		applyDexBonus = -80;

	if(applyStrengthBonus != 0)
	{
		AddStrengthBonus(applyStrengthBonus);
	}
	if(applyZenBonus != 0)
	{
		AddZenBonus(applyZenBonus);
	}
	if(applyIntBonus != 0)
	{
		AddIntBonus(applyIntBonus);
	}
	if(applyConstitutionBonus != 0)
	{
		AddConstitutionBonus(applyConstitutionBonus);
	}
	if(applyDexBonus != 0)
	{
		AddDexBonus(applyDexBonus);
	}

	// Item
	GetBaseBonusFromItem();

	// ����
	GetBaseBonusFromMastery();
}

void _XCharacterStateList::ApplyBonusAdditional(void)
{
	// character base �� ���
	GetCharacterBaseValue();
	
	// Skill, Effect
	map <int, _XCharacterState*>::iterator iter_state;
	for(iter_state = m_mapCharacterStateList.begin() ; iter_state != m_mapCharacterStateList.end() ; ++iter_state)
	{
		_XCharacterState* currentstate = iter_state->second;
		
		map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(currentstate->m_sEffectID);
		if(iter_table != g_CharacterStateTable.end())
		{
			if( g_CharacterStateTable[currentstate->m_sEffectID]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
				g_CharacterStateTable[currentstate->m_sEffectID]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE ||
				g_CharacterStateTable[currentstate->m_sEffectID]->effect_prop == _XCL_EFFECT_PROP_FEED)
			{
				switch(g_CharacterStateTable[currentstate->m_sEffectID]->where)
				{
				case en_eff_where_attack_rate :			// ���� ����
					AddPAttackRateBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_avoid_rate :			// ���� ȸ��
					AddPAvoidRateBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_attack_damage :		// ���� ����
					AddPAttackDamageBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_defence :				// ���� ���
					AddDefenceBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_m_attack_rate :		// ��� ����
					AddMAttackRateBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_m_avoid_rate :		// ��� ȸ��
					AddMAvoidRateBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_m_attack_damage :		// ��� ����
					AddMAttackDamageBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_m_defence :			// ��� ���
					AddMDefenceBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_max_life :			// �ִ� ����
					AddMaxLifeBonus(currentstate->m_sEffectID, currentstate->m_sValue);				
					break;
				case en_eff_where_max_force :			// �ִ� ����
					AddMaxForceBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_max_concentration :	// �ִ� ����
					AddMaxConcentrationBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_move_speed :			// �̵� �ӵ�
					AddMoveSpeedBonus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_weapon_dam :			// ���� ������ ����
					AddWeaponDamage(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_weapon_min_dam :		// ���� �ּ� ������ ����
					AddWeaponMinDamage(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_weapon_max_dam :		// ���� �ִ� ������ ����
					AddWeaponMaxDamage(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_disable_skill :		// Ư�� ���� ��� �Ұ�
					AddDisableSkill(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_disable_s_type :		// Ư�� s type ���� ��� �Ұ�
					AddDisableSType(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_resist_minus :		// ���� ���׷�
					AddResistMinus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_resist_plus :			// ���� ���׷�
					AddResistPlus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_resist_hon :			// ȥ�� ���׷�
					AddResistHon(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_resist_sin :			// ���� ���׷�
					AddResistSin(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_resist_all :			// ��� ���׷� ����
					AddResistAll(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_add_minus_damage :	// ���� ������ �߰�
					AddAttackDamageMinus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_add_plus_damage :		// ���� ������ �߰�
					AddAttackDamagePlus(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_add_hon_damage :		// ȥ�� ������ �߰�
					AddAttackDamageHon(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				case en_eff_where_add_sin_damage :		// ���� ������ �߰�
					AddAttackDamageSin(currentstate->m_sEffectID, currentstate->m_sValue);
					break;
				}
			} // if(g_CharacterStateTable[effectid]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
		}
	} // for(stateList)
	
	// Condition
	if(FindConditionState(g_pLocalUser->m_Uniqueid, 15))
	{
		// ���� - ���� ���� 20% ����
//		AddDefenceBonus(-20);
	}
	
	short movespeed = 0;
	if(FindConditionState(g_pLocalUser->m_Uniqueid, 5))
	{
		// ���� - �̵��ӵ� 30% ����
		movespeed -= 30;
	}
	if(FindConditionState(g_pLocalUser->m_Uniqueid, 16))
	{
		// ���� - �̵��ӵ� 30% ����
//		movespeed -= 30;
	}
	if(FindConditionState(g_pLocalUser->m_Uniqueid, 17))
	{
		// ���� - �̵��ӵ� 30% ����
		movespeed -= 30;
	}
	if(FindConditionState(g_pLocalUser->m_Uniqueid, 19))
	{
		// ��ȭ�Ը� - �̵��ӵ� 30% ����
		movespeed -= 30;
	}
	if(FindConditionState(g_pLocalUser->m_Uniqueid, 28))
	{
		// ������ - �̵��ӵ� 30% ����
		movespeed -= 30;
	}
	
	if(movespeed != 0)
	{
		AddMoveSpeedBonus(movespeed);
	}
	
	// Item
	GetAdditionalBonusFromItem();

	// ����
	GetAdditionalBonusFromMastery();

#ifdef _XTS_MASTERY
	// ���� - ��ų ��ȭ
	GetSkillModifiedFromMastery();
#endif
}

void _XCharacterStateList::ApplyFinalValue(void)
{
	// �ٷ�
	if(g_pLocalUser->m_Strength.Get_applyCount() > 0)
	{
		g_pLocalUser->m_Strength.Set_result( g_pLocalUser->m_Strength.Get_org() + (short)g_pLocalUser->m_Strength.bonus );

		if(g_pLocalUser->m_Strength.bonus > 0.f)
			g_pLocalUser->m_Strength.Set_bBuff( 1 );
		else if(g_pLocalUser->m_Strength.bonus < 0.f)
			g_pLocalUser->m_Strength.Set_bBuff( 0 );
	}
	else
	{
		g_pLocalUser->m_Strength.Set_result( g_pLocalUser->m_Strength.Get_org() );
	}	

	// ����
	if(g_pLocalUser->m_Zen.Get_applyCount() > 0)
	{
		g_pLocalUser->m_Zen.Set_result( g_pLocalUser->m_Zen.Get_org() + (short)g_pLocalUser->m_Zen.bonus );
		
		if(g_pLocalUser->m_Zen.bonus > 0.f)
			g_pLocalUser->m_Zen.Set_bBuff( 1 );
		else if(g_pLocalUser->m_Zen.bonus < 0.f)
			g_pLocalUser->m_Zen.Set_bBuff( 0 );
	}
	else
	{
		g_pLocalUser->m_Zen.Set_result( g_pLocalUser->m_Zen.Get_org() );
	}
	
	// ����
	if(g_pLocalUser->m_Intelligence.Get_applyCount() > 0)
	{
		g_pLocalUser->m_Intelligence.Set_result( g_pLocalUser->m_Intelligence.Get_org() + (short)g_pLocalUser->m_Intelligence.bonus );
		
		if(g_pLocalUser->m_Intelligence.bonus > 0.f)
			g_pLocalUser->m_Intelligence.Set_bBuff( 1 );
		else if(g_pLocalUser->m_Intelligence.bonus < 0.f)
			g_pLocalUser->m_Intelligence.Set_bBuff( 0 );
	}
	else
	{
		g_pLocalUser->m_Intelligence.Set_result( g_pLocalUser->m_Intelligence.Get_org() );
	}	

	// �ǰ�
	if(g_pLocalUser->m_Constitution.Get_applyCount() > 0)
	{
		g_pLocalUser->m_Constitution.Set_result( g_pLocalUser->m_Constitution.Get_org() + (short)g_pLocalUser->m_Constitution.bonus );
		
		if(g_pLocalUser->m_Constitution.bonus > 0.f)
			g_pLocalUser->m_Constitution.Set_bBuff( 1 );
		else if(g_pLocalUser->m_Constitution.bonus < 0.f)
			g_pLocalUser->m_Constitution.Set_bBuff( 0 );
	}
	else
	{
		g_pLocalUser->m_Constitution.Set_result( g_pLocalUser->m_Constitution.Get_org() );
	}	

	// ��ø
	if(g_pLocalUser->m_Dex.Get_applyCount() > 0)
	{
		g_pLocalUser->m_Dex.Set_result( g_pLocalUser->m_Dex.Get_org() + (short)g_pLocalUser->m_Dex.bonus );
		
		if(g_pLocalUser->m_Dex.bonus > 0.f)
			g_pLocalUser->m_Dex.Set_bBuff( 1 );
		else if(g_pLocalUser->m_Dex.bonus < 0.f)
			g_pLocalUser->m_Dex.Set_bBuff( 0 );
	}
	else
	{
		g_pLocalUser->m_Dex.Set_result( g_pLocalUser->m_Dex.Get_org() );
	}	

	// �ִ� ����
	if( g_pLocalUser->m_MaxLifePower.Get_applyCount() > 0 || g_pLocalUser->m_MaxLifePower.Get_applyCountAdditional() > 0 )
		
	{
		g_pLocalUser->m_MaxLifePower.Set_result( g_pLocalUser->m_MaxLifePower.Get_org() + (short)g_pLocalUser->m_MaxLifePower.bonus );
		g_pLocalUser->m_MaxLifePower.Set_result( g_pLocalUser->m_MaxLifePower.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(3) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].vitality );
		
		if(g_pLocalUser->m_MaxLifePower.bonus > 0.f)
			g_pLocalUser->m_MaxLifePower.Set_bBuff( true );
		else if(g_pLocalUser->m_MaxLifePower.bonus < 0.f)
			g_pLocalUser->m_MaxLifePower.Set_bBuff( false );
	}
	else
	{
		g_pLocalUser->m_MaxLifePower.Set_result( g_pLocalUser->m_MaxLifePower.Get_org() );
		g_pLocalUser->m_MaxLifePower.Set_result( g_pLocalUser->m_MaxLifePower.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(3) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].vitality );
	}

	// �ִ� ����
	if(g_pLocalUser->m_MaxForcePower.Get_applyCount() > 0 || g_pLocalUser->m_MaxForcePower.Get_applyCountAdditional() > 0)
	{
		g_pLocalUser->m_MaxForcePower.Set_result( g_pLocalUser->m_MaxForcePower.Get_org() + (short)g_pLocalUser->m_MaxForcePower.bonus );
		g_pLocalUser->m_MaxForcePower.Set_result( g_pLocalUser->m_MaxForcePower.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(1) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].inner );
		if(g_pLocalUser->m_MaxForcePower.bonus > 0.f)
			g_pLocalUser->m_MaxForcePower.Set_bBuff( true );
		else if(g_pLocalUser->m_MaxForcePower.bonus < 0.f)
			g_pLocalUser->m_MaxForcePower.Set_bBuff( false );
	}
	else
	{
		g_pLocalUser->m_MaxForcePower.Set_result( g_pLocalUser->m_MaxForcePower.Get_org() );
		g_pLocalUser->m_MaxForcePower.Set_result( g_pLocalUser->m_MaxForcePower.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(1) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].inner );
	}

	// �ִ� ����
	if(g_pLocalUser->m_MaxConcentrationPower.Get_applyCount() > 0 || g_pLocalUser->m_MaxConcentrationPower.Get_applyCountAdditional() > 0)
	{
		g_pLocalUser->m_MaxConcentrationPower.Set_result( g_pLocalUser->m_MaxConcentrationPower.Get_org() + (short)g_pLocalUser->m_MaxConcentrationPower.bonus );
		g_pLocalUser->m_MaxConcentrationPower.Set_result( g_pLocalUser->m_MaxConcentrationPower.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(2) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].vitality );
		
		if(g_pLocalUser->m_MaxConcentrationPower.bonus > 0.f)
			g_pLocalUser->m_MaxConcentrationPower.Set_bBuff( true );
		else if(g_pLocalUser->m_MaxConcentrationPower.bonus < 0.f)
			g_pLocalUser->m_MaxConcentrationPower.Set_bBuff( false );
	}
	else
	{
		g_pLocalUser->m_MaxConcentrationPower.Set_result( g_pLocalUser->m_MaxConcentrationPower.Get_org() );
		g_pLocalUser->m_MaxConcentrationPower.Set_result( g_pLocalUser->m_MaxConcentrationPower.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(2) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].vitality );
	}

	// ���ݷ� ========================================
	short skill_min_damage = 0, skill_max_damage = 0, item_min_damage = 0, item_max_damage = 0;

	// skill
	if(g_pLocalUser->m_SelectedSkillID > 0)
	{
		if(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillCType == _XSI_CTYPE_MARTIALARTS)
		{
			skill_min_damage = _XSkillItem::GetMinAttackDamage(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
			skill_max_damage = _XSkillItem::GetMaxAttackDamage(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
		}
	}

	// item
	// 1. ���� base damage, inchant damage ���ϱ�
	int inchantminaddpoint = 0;
	int inchantmaxaddpoint = 0;
	if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID() > 0)
	{
		if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cSecond() != _XGI_SC_WEAPON_SPECIAL )
		{
			if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType()) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType());
				if( WeaponItem )
				{
					item_min_damage = WeaponItem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID()].sMinPower;
					item_max_damage = WeaponItem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID()].sMaxPower;

					inchantminaddpoint = _XGameItem::GetInchantPlusPoint( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cSecond(), 
						item_min_damage, g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_ucStrength() );				
					inchantmaxaddpoint = _XGameItem::GetInchantPlusPoint( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cSecond(), 
						item_max_damage, g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_ucStrength() );				
				}
			}
		}		
	}

	// 2. item bonus damage	����
	short weaponbonus_min = 0;
	short weaponbonus_max = 0;

	// percentage ����
	if(g_pLocalUser->m_WeaponMinDamageBonus[1] != 0)
	{
		weaponbonus_min = (short)(item_min_damage * (g_pLocalUser->m_WeaponMinDamageBonus[1]/100.0f));
	}
	if(g_pLocalUser->m_WeaponMaxDamageBonus[1] != 0)
	{
		weaponbonus_max = (short)(item_max_damage * (g_pLocalUser->m_WeaponMaxDamageBonus[1]/100.0f));
	}
	
	// value ����
	if(g_pLocalUser->m_WeaponMinDamageBonus[0] != 0)
	{
		if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID() > 0)
		{
			if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cSecond() != _XGI_SC_WEAPON_SPECIAL )
				weaponbonus_min += g_pLocalUser->m_WeaponMinDamageBonus[0];
		}
	}
	if(g_pLocalUser->m_WeaponMaxDamageBonus[0] != 0)
	{
		if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID() > 0)
		{
			if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cSecond() != _XGI_SC_WEAPON_SPECIAL )
				weaponbonus_max += g_pLocalUser->m_WeaponMaxDamageBonus[0];
		}
	}

	// 3. �ٷ¿� ���� ���� ������ ���� ����
	float add_min = 0, add_max = 0;
	if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID() > 0)
	{
		if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType()) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType());
			if( WeaponItem )
			{
				if(WeaponItem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID()].cSecondType != _XGI_SC_WEAPON_SPECIAL)
				{
					// �����Ⱑ �ƴ� �� �ٷ¿� ���� ���� ���� - weapon base damage���� �����Ѵ�
					add_min = item_min_damage * 
						((g_pLocalUser->m_Strength.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(0)) * g_PD_Weapon_Str_Add[g_pLocalUser->m_CharacterInfo.Get_cClass()]
						+ 1);
					add_max = item_max_damage * ((g_pLocalUser->m_Strength.Get_result() + g_pLocalUser->m_ElixirInfo.GetTotalBonus(0)) * g_PD_Weapon_Str_Add[g_pLocalUser->m_CharacterInfo.Get_cClass()] + 1);
				}
			}
		}
	}

	item_min_damage = weaponbonus_min + (short)add_min;
	item_max_damage = weaponbonus_max + (short)add_max;

	// 4. ���� ��
	if( inchantminaddpoint > 0 || inchantmaxaddpoint > 0 )
	{
		item_min_damage += (short)inchantminaddpoint;
		item_max_damage += (short)inchantmaxaddpoint;
	}
	
	// 5. item penalty
	if(g_pLocalUser->m_PenaltyValueFromItem[0] != 0)
	{
		item_min_damage += (short)(item_min_damage * (g_pLocalUser->m_PenaltyValueFromItem[0]/100.f));
		item_max_damage += (short)(item_max_damage * (g_pLocalUser->m_PenaltyValueFromItem[0]/100.f));
	}

	if(g_pLocalUser->m_P_AttackDamage.Get_applyCount() > 0)
	{
#ifdef _XTS_BASEMINMAXWEAPONDAMAGE
		g_pLocalUser->m_P_AttackDamage.result_min = g_pLocalUser->m_P_AttackDamage.org_min + g_pLocalUser->m_P_AttackDamage.bonus_min;
		g_pLocalUser->m_P_AttackDamage.result_max = g_pLocalUser->m_P_AttackDamage.org_max + g_pLocalUser->m_P_AttackDamage.bonus_max;
		
		if(g_pLocalUser->m_P_AttackDamage.bonus_min > 0.f) 
			g_pLocalUser->m_P_AttackDamage.Set_bBuff( true );
		else if(g_pLocalUser->m_P_AttackDamage.bonus_min < 0.f)
			g_pLocalUser->m_P_AttackDamage.Set_bBuff( false );
#else
		g_pLocalUser->m_P_AttackDamage.result = g_pLocalUser->m_P_AttackDamage.org + (short)g_pLocalUser->m_P_AttackDamage.bonus;

		if(g_pLocalUser->m_P_AttackDamage.bonus > 0.f)
			g_pLocalUser->m_P_AttackDamage.Set_bBuff( true );
		else if(g_pLocalUser->m_P_AttackDamage.bonus < 0.f)
			g_pLocalUser->m_P_AttackDamage.Set_bBuff( false );
#endif
	}
	else if(g_pLocalUser->m_P_AttackDamage.Get_applyCountAdditional() > 0)
	{
#ifdef _XTS_BASEMINMAXWEAPONDAMAGE

		g_pLocalUser->m_P_AttackDamage.result_min = g_pLocalUser->m_P_AttackDamage.org_min;
		g_pLocalUser->m_P_AttackDamage.result_max = g_pLocalUser->m_P_AttackDamage.org_max;
		g_pLocalUser->m_P_AttackDamage.Set_bBuff( g_pLocalUser->m_Strength.Get_bBuff() );

#else
		g_pLocalUser->m_P_AttackDamage.result = g_pLocalUser->m_P_AttackDamage.org;
		g_pLocalUser->m_P_AttackDamage.Set_bBuff( g_pLocalUser->m_Strength.Get_bBuff() );
#endif
	}
	else
	{
#ifdef _XTS_BASEMINMAXWEAPONDAMAGE
		g_pLocalUser->m_P_AttackDamage.result_min = g_pLocalUser->m_P_AttackDamage.org_min;
		g_pLocalUser->m_P_AttackDamage.result_max = g_pLocalUser->m_P_AttackDamage.org_max;
#else
		g_pLocalUser->m_P_AttackDamage.result = g_pLocalUser->m_P_AttackDamage.org;
#endif
	}

#ifdef _XTS_BASEMINMAXWEAPONDAMAGE
	g_pLocalUser->m_P_MinAttackDamage = (short)g_pLocalUser->m_P_AttackDamage.result_min + skill_min_damage + item_min_damage;
	g_pLocalUser->m_P_MaxAttackDamage = (short)g_pLocalUser->m_P_AttackDamage.result_max + skill_max_damage + item_max_damage;
#else
	g_pLocalUser->m_P_MinAttackDamage = g_pLocalUser->m_P_AttackDamage.result + skill_min_damage + item_min_damage;
	g_pLocalUser->m_P_MaxAttackDamage = g_pLocalUser->m_P_AttackDamage.result + skill_max_damage + item_max_damage;
#endif
	
#ifdef _XTS_FURY
	if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_FURY)
	{
		// �г� - ���ݷ� 30% ����
		g_pLocalUser->m_P_MinAttackDamage += (short)(g_pLocalUser->m_P_MinAttackDamage*(30.0f/100.0f));
		g_pLocalUser->m_P_MaxAttackDamage += (short)(g_pLocalUser->m_P_MaxAttackDamage*(30.0f/100.0f));
	}
	else if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_CONVERGENCE)
	{	
		// ���� - ���ݷ� 30% ����
		g_pLocalUser->m_P_MinAttackDamage += (short)(g_pLocalUser->m_P_MinAttackDamage*(50.0f/100.0f));
		g_pLocalUser->m_P_MaxAttackDamage += (short)(g_pLocalUser->m_P_MaxAttackDamage*(50.0f/100.0f));
	}
#endif

	// ���� ����ġ ========================================
	short skill_attack_rate = 0, item_attack_rate = 0;

	// skill
	if(g_pLocalUser->m_SelectedSkillID > 0)
	{
		if(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillCType == _XSI_CTYPE_MARTIALARTS)
		{
			skill_attack_rate = _XSkillItem::GetAttackRating(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->detailInfo[g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel].attackRating;
		}
	}

	// item
	if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID() > 0)
	{
		if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType()) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType());
			if( WeaponItem )
				item_attack_rate += WeaponItem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID()].cAttackRate;
		}
	}

	// item penalty - ����
	if(g_pLocalUser->m_PenaltyValueFromItem[1] != 0)
	{
		item_attack_rate += item_attack_rate * (g_pLocalUser->m_PenaltyValueFromItem[1]/100.f);
	}
	
	if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_ADAE].Get_m_sID() > 0)
	{
		if( _XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_ADAE].Get_m_cType()) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_ADAE].Get_m_cType());
			if( ClothesItem )
				item_attack_rate += ClothesItem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_ADAE].Get_m_sID()].cAttackRate;
		}
	}

	if(g_pLocalUser->m_P_AttackRate.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_P_AttackRate.bonus > 0.f)
			g_pLocalUser->m_P_AttackRate.Set_bBuff(true);
		else if(g_pLocalUser->m_P_AttackRate.bonus < 0.f)
			g_pLocalUser->m_P_AttackRate.Set_bBuff(false);
	}
	else if(g_pLocalUser->m_P_AttackRate.Get_applyCountAdditional() > 0)
	{
		#ifdef _XTS_NEWBATTLEPARAMETER //Author : ����� breif : ������ ���� ����ġ�� ������ ��ġ�Ƿ�
		if( g_pLocalUser->m_Dex.Get_bBuff() || g_pLocalUser->m_Strength.Get_bBuff() )
			g_pLocalUser->m_P_AttackRate.Set_bBuff(true);
		else
			g_pLocalUser->m_P_AttackRate.Set_bBuff(false);
		#else
		g_pLocalUser->m_P_AttackRate.Set_bBuff( g_pLocalUser->m_Dex.Get_bBuff() );
		#endif
	}

	g_pLocalUser->m_P_AttackRate.Set_result( g_pLocalUser->m_P_AttackRate.Get_org() + skill_attack_rate + item_attack_rate + g_pLocalUser->m_P_AttackRate.bonus );

#ifdef _XTS_FURY
	if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_CONVERGENCE)
	{
		// ���� - ���� ����ġ 20% ����
		g_pLocalUser->m_P_AttackRate.result += (short)(g_pLocalUser->m_P_AttackRate.result * (20.0f/100.0f));
	}
#endif

	// ���� ========================================
	short item_defence = 0;

	for( int pos = _XINVENTORY_SLOTTYPE_AMOR; pos <= _XINVENTORY_SLOTTYPE_GAP_LOWER; ++pos )
	{
		if(g_pLocalUser->m_UserItemList[pos].Get_m_sID() > 0)
		{
			if( _XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[pos].Get_m_cType()) )
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[pos].Get_m_cType());
				if( ClothesItem )
					item_defence += ClothesItem[g_pLocalUser->m_UserItemList[pos].Get_m_sID()].sDefence;				
			}
		}
	}	

	if(g_pLocalUser->m_P_Defence.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_P_Defence.bonus > 0.f)
			g_pLocalUser->m_P_Defence.Set_bBuff( true ); 
		else if(g_pLocalUser->m_P_Defence.bonus < 0.f)
			g_pLocalUser->m_P_Defence.Set_bBuff( false );
	}
	else if(g_pLocalUser->m_P_Defence.Get_applyCountAdditional() > 0)
	{
		#ifdef _XTS_NEWBATTLEPARAMETER //Author : ����� breif : �ǰ��� ���¿� ������ ��ģ��
		if( g_pLocalUser->m_Strength.Get_bBuff() || g_pLocalUser->m_Constitution.Get_bBuff() )
			g_pLocalUser->m_P_Defence.Set_bBuff( true );
		else
			g_pLocalUser->m_P_Defence.Set_bBuff( false );
		#else
		g_pLocalUser->m_P_Defence.Set_bBuff( g_pLocalUser->m_Strength.Get_bBuff() );
		#endif
	}

	g_pLocalUser->m_P_Defence.Set_result( g_pLocalUser->m_P_Defence.Get_org() + item_defence + g_pLocalUser->m_P_Defence.bonus );

#ifdef _XTS_FURY
	if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_FURY)
	{
		// �г� - ���� 10% ����
		g_pLocalUser->m_P_Defence.result -= (short)(g_pLocalUser->m_P_Defence.result*(10.0f/100.0f));
	}
	else if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_CONVERGENCE)
	{	
		// ���� - ���� 20% ����
		g_pLocalUser->m_P_Defence.result -= (short)(g_pLocalUser->m_P_Defence.result*(20.0f/100.0f));
	}
#endif

	// ȸ�Ƿ� ========================================
	short skill_avoid_rate = 0, item_avoid_rate = 0;

	// skill
	if(g_pLocalUser->m_SelectedSkillID > 0)
	{
		if(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillCType == _XSI_CTYPE_MARTIALARTS)
		{
			skill_avoid_rate = _XSkillItem::GetAvoidRating(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
		}
	}

	// item
	if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CAP].Get_m_sID() > 0)
	{
		if( _XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CAP].Get_m_cType()) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CAP].Get_m_cType());
			if( ClothesItem )
				item_avoid_rate = ClothesItem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CAP].Get_m_sID()].cEvasionRate;
		}
	}	

	if(g_pLocalUser->m_P_AvoidRate.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_P_AvoidRate.bonus > 0.f)
			g_pLocalUser->m_P_AvoidRate.Set_bBuff( true );
		else if(g_pLocalUser->m_P_AvoidRate.bonus < 0.f)
			g_pLocalUser->m_P_AvoidRate.Set_bBuff( false );
	}
	else if(g_pLocalUser->m_P_AvoidRate.Get_applyCountAdditional() > 0)
	{
		g_pLocalUser->m_P_AvoidRate.Set_bBuff( g_pLocalUser->m_Dex.Get_bBuff() );
	}

	g_pLocalUser->m_P_AvoidRate.Set_result( g_pLocalUser->m_P_AvoidRate.Get_org() + skill_avoid_rate + item_avoid_rate + g_pLocalUser->m_P_AvoidRate.bonus );

#ifdef _XTS_FURY
	if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_CONVERGENCE)
	{	
		// ���� - ���� ȸ��ġ 10% ����
		g_pLocalUser->m_P_AvoidRate.result -= (short)(g_pLocalUser->m_P_AvoidRate.result*(10.0f/100.0f));
	}
#endif

	// ������ݷ� ========================================
	skill_min_damage = 0;
	skill_max_damage = 0;

	// skill
	if(g_pLocalUser->m_SelectedSkillID > 0)
	{
		if(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillCType == _XSI_CTYPE_SPELL)
		{
			if( g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType != _XSI_USEFULTYPE_HIDDENMA &&
				!(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillSType == _XSI_STYPE_03 && 
				g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL) )	// �˼� �ʿ�
			{
				skill_min_damage = _XSkillItem::GetMinAttackDamage(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
				skill_max_damage = _XSkillItem::GetMaxAttackDamage(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);

				// ������ ���� ������ ������ �����Ŀ���, ��� ���ϱ�
				FLOAT mul = g_MD_Spell_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()] * g_pLocalUser->m_Intelligence.Get_result() + g_MD_Spell_Add[g_pLocalUser->m_CharacterInfo.Get_cClass()];
				
				skill_min_damage *= mul;
				skill_max_damage *= mul;
			}
		}
	}

	// item
	item_min_damage = item_max_damage = 0;
	inchantmaxaddpoint = 0;
	inchantminaddpoint = 0;

	// 1. ���� base damage, inchant damage ���ϱ�
	if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID() > 0)
	{
		if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cSecond() == _XGI_SC_WEAPON_SPECIAL )
		{
			if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType()) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType());
				if( WeaponItem )
				{
					item_min_damage = WeaponItem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID()].sMinPower;
					item_max_damage = WeaponItem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID()].sMaxPower;
					inchantminaddpoint = _XGameItem::GetInchantPlusPoint( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cSecond(), 
						item_min_damage, g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_ucStrength() );
					inchantmaxaddpoint = _XGameItem::GetInchantPlusPoint( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cSecond(), 
						item_max_damage, g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_ucStrength() );
				}
			}
		}
	}

	// 2. item bonus damage
	weaponbonus_min = 0;
	weaponbonus_max = 0;
	
	// percentage ����
	if(g_pLocalUser->m_WeaponMinDamageBonus[1] != 0)
	{
		weaponbonus_min = (short)(item_min_damage * (g_pLocalUser->m_WeaponMinDamageBonus[1]/100.0f));
	}
	if(g_pLocalUser->m_WeaponMaxDamageBonus[1] != 0)
	{
		weaponbonus_max += (short)(item_max_damage * (g_pLocalUser->m_WeaponMaxDamageBonus[1]/100.0f));
	}
	
	// value ����
	if(g_pLocalUser->m_WeaponMinDamageBonus[0] != 0)
	{
		if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID() > 0)
		{
			if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cSecond() == _XGI_SC_WEAPON_SPECIAL )
				weaponbonus_min += g_pLocalUser->m_WeaponMinDamageBonus[0];
		}
	}
	if(g_pLocalUser->m_WeaponMaxDamageBonus[0] != 0)
	{
		if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID() > 0)
		{
			if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cSecond() == _XGI_SC_WEAPON_SPECIAL )
				weaponbonus_max += g_pLocalUser->m_WeaponMaxDamageBonus[0];
		}
	}

	item_min_damage += weaponbonus_min;
	item_max_damage += weaponbonus_max;

	// 3. ���ð�
	if( inchantminaddpoint > 0 || inchantmaxaddpoint > 0 )
	{
		item_min_damage += inchantminaddpoint;
		item_max_damage += inchantmaxaddpoint;
	}

	// 4. item penalty
	if(g_pLocalUser->m_PenaltyValueFromItem[0] != 0)
	{
		item_min_damage += (short)(item_min_damage * (g_pLocalUser->m_PenaltyValueFromItem[0]/100.f));
		item_max_damage += (short)(item_max_damage * (g_pLocalUser->m_PenaltyValueFromItem[0]/100.f));
	}

	if(g_pLocalUser->m_M_AttackDamage.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_M_AttackDamage.bonus > 0.f)
			g_pLocalUser->m_M_AttackDamage.Set_bBuff( true );
		else if(g_pLocalUser->m_M_AttackDamage.bonus < 0.f)
			g_pLocalUser->m_M_AttackDamage.Set_bBuff( false );
	}
	else if(g_pLocalUser->m_M_AttackDamage.Get_applyCountAdditional() > 0)
	{
		g_pLocalUser->m_M_AttackDamage.Set_bBuff( g_pLocalUser->m_Intelligence.Get_bBuff() );
	}

	g_pLocalUser->m_M_AttackDamage.Set_result( g_pLocalUser->m_M_AttackDamage.Get_org() + g_pLocalUser->m_M_AttackDamage.bonus );

	g_pLocalUser->m_M_MinAttackDamage = g_pLocalUser->m_M_AttackDamage.Get_result() + skill_min_damage + item_min_damage;
	g_pLocalUser->m_M_MaxAttackDamage = g_pLocalUser->m_M_AttackDamage.Get_result() + skill_max_damage + item_max_damage;	

#ifdef _XTS_FURY
	if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_FURY)
	{
		// �г� - ���ݷ� 30% ����
		g_pLocalUser->m_M_MinAttackDamage += (short)(g_pLocalUser->m_M_MinAttackDamage*(30.0f/100.0f));
		g_pLocalUser->m_M_MaxAttackDamage += (short)(g_pLocalUser->m_M_MaxAttackDamage*(30.0f/100.0f));
	}
	else if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_CONVERGENCE)
	{	
		// ���� - ���ݷ� 30% ����
		g_pLocalUser->m_M_MinAttackDamage += (short)(g_pLocalUser->m_M_MinAttackDamage*(50.0f/100.0f));
		g_pLocalUser->m_M_MaxAttackDamage += (short)(g_pLocalUser->m_M_MaxAttackDamage*(50.0f/100.0f));
	}
#endif

	// ��������� ========================================
	item_attack_rate = 0;

#ifdef _XDEF_ITEM_ADDVALUE_071024_KUKURI
	// item - �ƴ�
	if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_ADAE].Get_m_sID() > 0)
	{
		if( _XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_ADAE].Get_m_sID()) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_ADAE].Get_m_cType());
			if( ClothesItem )
				item_attack_rate += ClothesItem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_ADAE].Get_m_sID()].cAttackRate;
		}
	}
#endif

	skill_attack_rate = 0;
	if(g_pLocalUser->m_SelectedSkillID > 0)
	{
		if(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillCType == _XSI_CTYPE_SPELL)
		{
			if(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType != _XSI_USEFULTYPE_HIDDENMA)
			{
				skill_attack_rate = _XSkillItem::GetAttackRating(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
			}
		}
	}

	if(g_pLocalUser->m_M_AttackRate.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_M_AttackRate.bonus > 0.f)
			g_pLocalUser->m_M_AttackRate.Set_bBuff( true );
		else if(g_pLocalUser->m_M_AttackRate.bonus < 0.f)
			g_pLocalUser->m_M_AttackRate.Set_bBuff( false );
	}
	else if(g_pLocalUser->m_M_AttackRate.Get_applyCountAdditional() > 0)
	{
		#ifdef _XTS_NEWBATTLEPARAMETER //Author : ����� breif : ���� ����� ������ ��ģ��
		if( g_pLocalUser->m_Dex.Get_bBuff() || g_pLocalUser->m_Intelligence.Get_bBuff() )
			g_pLocalUser->m_M_AttackRate.Set_bBuff(true);
		else
			g_pLocalUser->m_M_AttackRate.Set_bBuff(false);
		#else
		g_pLocalUser->m_M_AttackRate.Set_bBuff( g_pLocalUser->m_Intelligence.Get_bBuff() );
		#endif
	}

	g_pLocalUser->m_M_AttackRate.Set_result( g_pLocalUser->m_M_AttackRate.Get_org() + skill_attack_rate + g_pLocalUser->m_M_AttackRate.bonus + item_attack_rate );

#ifdef _XTS_FURY
	if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_CONVERGENCE)
	{	
		// ���� - ��� ����ġ 20% ����
		g_pLocalUser->m_M_AttackRate.result += (short)(g_pLocalUser->m_M_AttackRate.result*(20.0f/100.0f));
	}
#endif

	// ������� ========================================

	item_defence = 0;

#ifdef _XDEF_ITEM_ADDVALUE_071024_KUKURI

	// Item-defence ���� ������¿� ���Ѵ�
	for( pos = _XINVENTORY_SLOTTYPE_AMOR; pos <= _XINVENTORY_SLOTTYPE_GAP_LOWER; ++pos )
	{
		if(g_pLocalUser->m_UserItemList[pos].Get_m_sID() > 0)
		{
			if( _XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[pos].Get_m_cType()) )
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[pos].Get_m_cType());
				if( ClothesItem )
					item_defence += ClothesItem[g_pLocalUser->m_UserItemList[pos].Get_m_sID()].sDefence;				
			}
		}
	}

#endif

	if(g_pLocalUser->m_M_Defence.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_M_Defence.bonus > 0.f)
			g_pLocalUser->m_M_Defence.Set_bBuff( true );
		else if(g_pLocalUser->m_M_Defence.bonus < 0.f)
			g_pLocalUser->m_M_Defence.Set_bBuff(false);
	}
	else if(g_pLocalUser->m_M_Defence.Get_applyCountAdditional() > 0)
	{
		g_pLocalUser->m_M_Defence.Set_bBuff( g_pLocalUser->m_Zen.Get_bBuff()+g_pLocalUser->m_Intelligence.Get_bBuff());
	}

	g_pLocalUser->m_M_Defence.Set_result( g_pLocalUser->m_M_Defence.Get_org() + g_pLocalUser->m_M_Defence.bonus + item_defence );

	// ���ȸ�Ƿ� ========================================
	item_avoid_rate = 0;

#ifdef _XDEF_ITEM_ADDVALUE_071024_KUKURI
	// item - ����
	if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CAP].Get_m_sID() > 0)
	{
		if( _XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CAP].Get_m_cType()) )
		{
			_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CAP].Get_m_cType());
			if( ClothesItem )
				item_avoid_rate = ClothesItem[g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CAP].Get_m_sID()].cEvasionRate;
		}
	}
#endif

	skill_avoid_rate = 0;
	
	// skill
	if(g_pLocalUser->m_SelectedSkillID > 0)
	{
		if(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->skillCType == _XSI_CTYPE_SPELL)
		{
			if(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType != _XSI_USEFULTYPE_HIDDENMA)
			{
				skill_avoid_rate = _XSkillItem::GetAvoidRating(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
			}
		}
	}

	if(g_pLocalUser->m_M_AvoidRate.Get_applyCount() > 0)
	{
		if(g_pLocalUser->m_M_AvoidRate.bonus > 0.f)
			g_pLocalUser->m_M_AvoidRate.Set_bBuff(true);
		else if(g_pLocalUser->m_M_AvoidRate.bonus < 0.f)
			g_pLocalUser->m_M_AvoidRate.Set_bBuff(false);
	}
	else if(g_pLocalUser->m_M_AvoidRate.Get_applyCountAdditional() > 0)
	{
		#ifdef _XTS_NEWBATTLEPARAMETER //Author : ����� breif : ���� ����� ������ ��ģ��
		if( g_pLocalUser->m_Dex.Get_bBuff() || g_pLocalUser->m_Intelligence.Get_bBuff() )
			g_pLocalUser->m_M_AvoidRate.Set_bBuff(true);
		else
			g_pLocalUser->m_M_AvoidRate.Set_bBuff(false);
		#else
		g_pLocalUser->m_M_AvoidRate.Set_bBuff(g_pLocalUser->m_Intelligence.Get_bBuff());
		#endif
	}

	g_pLocalUser->m_M_AvoidRate.Set_result( g_pLocalUser->m_M_AvoidRate.Get_org() + skill_avoid_rate + g_pLocalUser->m_M_AvoidRate.bonus + item_avoid_rate );

#ifdef _XTS_FURY
	if(g_pLocalUser->m_UserTroubleState == _XDEF_CONVSTATE_CONVERGENCE)
	{	
		// ���� - ��� ȸ��ġ 10% ����
		g_pLocalUser->m_M_AttackRate.result -= (short)(g_pLocalUser->m_M_AttackRate.result*(10.0f/100.0f));
	}
#endif
}


void _XCharacterStateList::GetCharacterBaseValue(void)
{
	DWORD step, level, innerlevel;
	step = g_pLocalUser->m_CharacterInfo.Get_level();
	level = g_pLocalUser->m_CharacterInfo.Get_sublevel();
	innerlevel = g_pLocalUser->m_CharacterInfo.Get_innerlevel();
	
	short str = 0, zen = 0, intel = 0, dex = 0, con = 0;
	
	if(g_pLocalUser->m_Strength.Get_applyCount() > 0)
	{
		str = g_pLocalUser->m_Strength.Get_org() + (short)g_pLocalUser->m_Strength.bonus;

		// ���� ����, ���� �� ����
		g_pLocalUser->m_P_AttackDamage.Set_applyCountAdditional( g_pLocalUser->m_P_AttackDamage.Get_applyCountAdditional() + 1 );
		g_pLocalUser->m_P_Defence.Set_applyCountAdditional( g_pLocalUser->m_P_Defence.Get_applyCountAdditional() + 1 );

#ifdef _XTS_NEWBATTLEPARAMETER
		// ���� ���� �߰�
		g_pLocalUser->m_P_AttackRate.Set_applyCountAdditional( g_pLocalUser->m_P_AttackRate.Get_applyCountAdditional() + 1 );
#endif
	}
	else
	{
		str = g_pLocalUser->m_Strength.Get_org();
	}
	if( g_pLocalUser->m_ElixirInfo.Data[0].__Grade > 0 )
	{
		str += g_pLocalUser->m_ElixirInfo.GetTotalBonus(0);
	}
	
	if(g_pLocalUser->m_Zen.Get_applyCount() > 0)
	{
		zen = g_pLocalUser->m_Zen.Get_org() + (short)g_pLocalUser->m_Zen.bonus;
		// ��� ���
		g_pLocalUser->m_M_Defence.Set_applyCountAdditional( g_pLocalUser->m_M_Defence.Get_applyCountAdditional() + 1 );
	}
	else
	{
		zen = g_pLocalUser->m_Zen.Get_org();
	}
	if( g_pLocalUser->m_ElixirInfo.Data[1].__Grade > 0 )
	{
		zen += g_pLocalUser->m_ElixirInfo.GetTotalBonus(1);
	}

	if(g_pLocalUser->m_Intelligence.Get_applyCount() > 0)
	{
		intel = g_pLocalUser->m_Intelligence.Get_org() + (short)g_pLocalUser->m_Intelligence.bonus;

		// ��� ����, ��� ȸ��, ��� ���, ��� ���ݿ� ����
		g_pLocalUser->m_M_AttackRate.Set_applyCountAdditional( g_pLocalUser->m_M_AttackRate.Get_applyCountAdditional() + 1 );
		g_pLocalUser->m_M_AvoidRate.Set_applyCountAdditional( g_pLocalUser->m_M_AvoidRate.Get_applyCountAdditional() + 1 );
		g_pLocalUser->m_M_Defence.Set_applyCountAdditional( g_pLocalUser->m_M_Defence.Get_applyCountAdditional() + 1 );
		g_pLocalUser->m_M_AttackDamage.Set_applyCountAdditional( g_pLocalUser->m_M_AttackDamage.Get_applyCountAdditional() + 1 );
	}
	else
	{
		intel = g_pLocalUser->m_Intelligence.Get_org();
	}
	if( g_pLocalUser->m_ElixirInfo.Data[2].__Grade > 0 )
	{
		intel += g_pLocalUser->m_ElixirInfo.GetTotalBonus(2);
	}

	if(g_pLocalUser->m_Dex.Get_applyCount() > 0)
	{
		dex = g_pLocalUser->m_Dex.Get_org() + (short)g_pLocalUser->m_Dex.bonus;

		// ���� ����, ���� ȸ�ǿ� ����
		g_pLocalUser->m_P_AttackRate.Set_applyCountAdditional( g_pLocalUser->m_P_AttackRate.Get_applyCountAdditional() + 1 );
		g_pLocalUser->m_P_AvoidRate.Set_applyCountAdditional( g_pLocalUser->m_P_AvoidRate.Get_applyCountAdditional() + 1 );

#ifdef _XTS_NEWBATTLEPARAMETER
		// ��� ����, ��� ȸ�� �߰�
		g_pLocalUser->m_M_AttackRate.Set_applyCountAdditional( g_pLocalUser->m_M_AttackRate.Get_applyCountAdditional() + 1 );
		g_pLocalUser->m_M_AvoidRate.Set_applyCountAdditional( g_pLocalUser->m_M_AvoidRate.Get_applyCountAdditional() + 1 );
#endif		
	}
	else
	{
		dex = g_pLocalUser->m_Dex.Get_org();
	}
	if( g_pLocalUser->m_ElixirInfo.Data[4].__Grade > 0 )
	{
		dex += g_pLocalUser->m_ElixirInfo.GetTotalBonus(4);
	}

	if(g_pLocalUser->m_Constitution.Get_applyCount() > 0)
	{
		con = g_pLocalUser->m_Constitution.Get_org() + (short)g_pLocalUser->m_Constitution.bonus;

#ifdef _XTS_NEWBATTLEPARAMETER
		// ���� ���¿� ����
		g_pLocalUser->m_P_Defence.Set_applyCountAdditional( g_pLocalUser->m_P_Defence.Get_applyCountAdditional() + 1 );
#endif
	}
	else
	{
		con = g_pLocalUser->m_Constitution.Get_org();
	}
	if( g_pLocalUser->m_ElixirInfo.Data[3].__Grade > 0 )
	{
		con += g_pLocalUser->m_ElixirInfo.GetTotalBonus(3);
	}
	
	// ���� ���� ����
#ifdef _XTS_NEWBATTLEPARAMETER
	#ifdef _XDEF_MONSTERREF_15	// +5.0 ����
		g_pLocalUser->m_P_AttackRate.Set_org( (short)(g_PAR_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
			g_PAR_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + g_PAR_Dex_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*dex +
			g_PAR_Str_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*str + 0.001f) );
	#else
		g_pLocalUser->m_P_AttackRate.Set_org( (short)(g_PAR_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
			g_PAR_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + 5.0f + g_PAR_Dex_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*dex +
			g_PAR_Str_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*str + 0.001f) );
	#endif
#else
	g_pLocalUser->m_P_AttackRate.Set_org( (short)(g_PAR_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
		g_PAR_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + 5.0f + g_PAR_Dex_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*dex + 0.001f) );
#endif
	
	// ���� ȸ��
	g_pLocalUser->m_P_AvoidRate.Set_org( (short)(g_PDR_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
		g_PDR_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + g_PDR_Dex_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*dex + 0.001f) );
	
	// ���� ����
#ifdef _XTS_BASEMINMAXWEAPONDAMAGE
	#ifdef _XDEF_MONSTERREF_15		//+5.0f ����
		FLOAT base = (g_PD_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
						g_PD_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step +  
						g_PD_Str_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*str + 0.001f);
	#else
		FLOAT base = (g_PD_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
						g_PD_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + 5.0f + 
						g_PD_Str_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*str + 0.001f);
	#endif

	// ������ ������ ������ ����
	int tblindex = 0;
	short itemid = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID();
	if(itemid > 0)
	{
		if(_XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType()))
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType());					
			if( WeaponItem )
			{
				char itemSType = WeaponItem[itemid].cSecondType;
				char itemTType = WeaponItem[itemid].cThirdType;
				if(itemSType == _XGI_SC_WEAPON_SHORT)
				{
					if(itemTType <= 2)
						tblindex = itemTType + 1;	// ��, ��, �ܺ�
					if(itemTType > 2)
						tblindex = itemTType;		// �ǰ�, �ܰ�, ��, ��ȯ
				}
				else if(itemSType == _XGI_SC_WEAPON_LONG)
				{
					if(itemTType == _XGI_TC_WEAPON_SPEAR)
						tblindex = 8;		// â
					if(itemTType == _XGI_TC_WEAPON_STAFF)
						tblindex = 9;		// ��
				}
				else if(itemSType == _XGI_SC_WEAPON_SPECIAL)
				{
					if(itemTType == _XGI_TC_WEAPON_SERING)
						tblindex = 0;		// ��
				}
			}
		}
	}
	else
	{
		// �Ǽ�
		tblindex = 0;
	}

	g_pLocalUser->m_P_AttackDamage.org_min = base * g_PD_Base_Min_Weapon_Mul[tblindex];
	g_pLocalUser->m_P_AttackDamage.org_max = base * g_PD_Base_Max_Weapon_Mul[tblindex];
	
#else
	g_pLocalUser->m_P_AttackDamage.Set_org( (short)(g_PD_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
		g_PD_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + 5.0f + g_PD_Str_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*str + 0.001f) );
#endif
	
	// ���� ���
#ifdef _XTS_NEWBATTLEPARAMETER
	g_pLocalUser->m_P_Defence.Set_org( (short)(g_PF_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
		g_PF_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + g_PF_Str_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*str  +
		g_PF_Vital_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*con  + 0.001f) );
#else
	g_pLocalUser->m_P_Defence.Set_org( (short)(g_PF_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
		g_PF_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + g_PF_Str_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*str  + 0.001f) ); 
#endif
	
	// ��� ���� ����
#ifdef _XTS_NEWBATTLEPARAMETER
	#ifdef _XDEF_MONSTERREF_15	//+5.0f ����
		g_pLocalUser->m_M_AttackRate.Set_org( (short)(g_MAR_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
			g_MAR_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + g_MAR_Int_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*intel +
			g_MAR_Dex_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*dex + 0.001f) );
	#else
		g_pLocalUser->m_M_AttackRate.Set_org( (short)(g_MAR_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
			g_MAR_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + 5.0f + g_MAR_Int_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*intel +
			g_MAR_Dex_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*dex + 0.001f) );
	#endif
#else
	g_pLocalUser->m_M_AttackRate.Set_org( (short)(g_MAR_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
		g_MAR_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + 5.0f + g_MAR_Int_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*intel + 0.001f) );
#endif
	
	// ��� ȸ��
#ifdef _XTS_NEWBATTLEPARAMETER
	g_pLocalUser->m_M_AvoidRate.Set_org( (short)(g_MDR_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
		g_MDR_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + g_MDR_Int_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*intel +
		g_MDR_Dex_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*dex + 0.001f) );
#else
	g_pLocalUser->m_M_AvoidRate.Set_org( (short)(g_MDR_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
		g_MDR_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + g_MDR_Int_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*intel + 0.001f) );
#endif
	
	// ��� ���
	g_pLocalUser->m_M_Defence.Set_org( (short)(g_MF_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
		g_MF_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + g_MF_Int_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*intel + 
		+ g_MF_Zen_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*zen + 0.001f) );
	
	// ��� ����
#ifdef _XDEF_MONSTERREF_15	// +5.0f ����
	g_pLocalUser->m_M_AttackDamage.Set_org( (short)(g_MD_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
		g_MD_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + g_MD_Int_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*intel 
		+ g_MD_Zen_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*zen + 0.001f) );	
#else
	g_pLocalUser->m_M_AttackDamage.Set_org( (short)(g_MD_Chr_Step_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*innerlevel + 
		g_MD_Chr_Level_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*step + 5.0f + g_MD_Int_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*intel 
		+ g_MD_Zen_Mul[g_pLocalUser->m_CharacterInfo.Get_cClass()]*zen + 0.001f) );	
#endif
}

void _XCharacterStateList::AddStrengthBonus(short id, short val)
{
	short str = 0;
	FLOAT bonus = 0.f;
	if(g_pLocalUser->m_Strength.Get_applyCount() > 0)
	{
		str = g_pLocalUser->m_Strength.Get_org();
		bonus = g_pLocalUser->m_Strength.bonus;
	}
	else
		str = g_pLocalUser->m_Strength.Get_org();

	if( g_pLocalUser->m_ElixirInfo.Data[0].__Grade > 0 )
	{
		str += g_pLocalUser->m_ElixirInfo.GetTotalBonus(0);
	}

	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += str * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= str * (val/100.0f);
		}
		break;
	}

	g_pLocalUser->m_Strength.bonus = bonus;
	g_pLocalUser->m_Strength.Set_applyCount( g_pLocalUser->m_Strength.Get_applyCount()+1 );
}

void _XCharacterStateList::AddStrengthBonus(short percentage)
{
	short str = 0;
	FLOAT bonus = 0.f;
	if(g_pLocalUser->m_Strength.Get_applyCount() > 0)
	{
		str = g_pLocalUser->m_Strength.Get_org();
		bonus = g_pLocalUser->m_Strength.bonus;
	}
	else
		str = g_pLocalUser->m_Strength.Get_org();

	if( g_pLocalUser->m_ElixirInfo.Data[0].__Grade > 0 )
	{
		str += g_pLocalUser->m_ElixirInfo.GetTotalBonus(0);
	}
	
	bonus += (FLOAT)str * ((FLOAT)percentage/100.0f);

	g_pLocalUser->m_Strength.bonus = bonus;	
	g_pLocalUser->m_Strength.Set_applyCount( g_pLocalUser->m_Strength.Get_applyCount()+1 );
}

void _XCharacterStateList::AddZenBonus(short id, short val)
{
	short zen = 0;
	FLOAT bonus = 0.f;
	if(g_pLocalUser->m_Zen.Get_applyCount() > 0)
	{
		zen = g_pLocalUser->m_Zen.Get_org();
		bonus = g_pLocalUser->m_Zen.bonus;
	}
	else 
		zen = g_pLocalUser->m_Zen.Get_org();

	if( g_pLocalUser->m_ElixirInfo.Data[1].__Grade > 0 )
	{
		zen += g_pLocalUser->m_ElixirInfo.GetTotalBonus(1);
	}

	// ����
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += zen * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= zen * (val/100.0f);
		}
		break;
	}

	//g_pLocalUser->m_Zen.bonus = bonus;
	g_pLocalUser->m_Zen.Set_applyCount( g_pLocalUser->m_Zen.Get_applyCount() + 1 );

	// �ִ� ����
	short force = 0;
	FLOAT forcebonus = 0;
	if(g_pLocalUser->m_MaxForcePower.Get_applyCount() > 0)
	{
		// �ִ� ���¿� ���ʽ� ���� ���
		force = g_pLocalUser->m_MaxForcePower.Get_org();
		forcebonus = g_pLocalUser->m_MaxForcePower.bonus;
		
		//Author : ����� //breif : ��������� ���� //Date : 08/02/15
		forcebonus -= (short)(g_pLocalUser->m_Zen.bonus) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].inner;	
	}
	else if(g_pLocalUser->m_MaxForcePower.Get_applyCountAdditional() > 0)
	{
		// ���⿡ ���ʽ� ���� ���
		force = g_pLocalUser->m_MaxForcePower.Get_org();
	}
	else
	{
		// ���ʽ� ����
		force = g_pLocalUser->m_MaxForcePower.Get_org();
	}

	g_pLocalUser->m_Zen.bonus = bonus;

	forcebonus += (short)(g_pLocalUser->m_Zen.bonus) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].inner;	

	g_pLocalUser->m_MaxForcePower.bonus = forcebonus;
	g_pLocalUser->m_MaxForcePower.Set_applyCountAdditional(g_pLocalUser->m_MaxForcePower.Get_applyCountAdditional()+1);
}

void _XCharacterStateList::AddZenBonus(short percentage)
{
	short zen = 0;
	FLOAT bonus = 0.f;
	if(g_pLocalUser->m_Zen.Get_applyCount() > 0)
	{
		zen = g_pLocalUser->m_Zen.Get_org();
		bonus = g_pLocalUser->m_Zen.bonus;
	}
	else
		zen = g_pLocalUser->m_Zen.Get_org();
	
	if( g_pLocalUser->m_ElixirInfo.Data[1].__Grade > 0 )
	{
		zen += g_pLocalUser->m_ElixirInfo.GetTotalBonus(1);
	}

	bonus += (FLOAT)zen * ((FLOAT)percentage/100.0f);
	
	g_pLocalUser->m_Zen.bonus = bonus;
	g_pLocalUser->m_Zen.Set_applyCount( g_pLocalUser->m_Zen.Get_applyCount() + 1 );

	// �ִ� ����
	short force = 0;
	FLOAT forcebonus = 0;
	if(g_pLocalUser->m_MaxForcePower.Get_applyCount() > 0)
	{
		// �ִ� ���¿� ���ʽ� ���� ���
		force = g_pLocalUser->m_MaxForcePower.Get_org();
		forcebonus = g_pLocalUser->m_MaxForcePower.bonus;
	}
	else if(g_pLocalUser->m_MaxForcePower.Get_applyCountAdditional() > 0)
	{
		// ���⿡ ���ʽ� ���� ���
		force = g_pLocalUser->m_MaxForcePower.Get_org();
	}
	else
	{
		// ���ʽ� ����
		force = g_pLocalUser->m_MaxForcePower.Get_org();
	}
	
	forcebonus += (short)(g_pLocalUser->m_Zen.bonus) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].inner;	
	g_pLocalUser->m_MaxForcePower.bonus = forcebonus;
	g_pLocalUser->m_MaxForcePower.Set_applyCountAdditional( g_pLocalUser->m_MaxForcePower.Get_applyCountAdditional() + 1 );
}

void _XCharacterStateList::AddIntBonus(short id, short val)
{
	short intel;
	FLOAT bonus = 0.f;
	if(g_pLocalUser->m_Intelligence.Get_applyCount() > 0)
	{
		intel = g_pLocalUser->m_Intelligence.Get_org();
		bonus = g_pLocalUser->m_Intelligence.bonus;
	}
	else
		intel = g_pLocalUser->m_Intelligence.Get_org();

	if( g_pLocalUser->m_ElixirInfo.Data[2].__Grade > 0 )
	{
		intel += g_pLocalUser->m_ElixirInfo.GetTotalBonus(2);
	}

	// ����
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += intel * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= intel * (val/100.0f);
		}
		break;
	}
	
	g_pLocalUser->m_Intelligence.bonus = bonus;
	g_pLocalUser->m_Intelligence.Set_applyCount( g_pLocalUser->m_Intelligence.Get_applyCount() + 1 );

	// �ִ� ����
	short concentration = 0;
	FLOAT conbonus = 0;
	if(g_pLocalUser->m_MaxConcentrationPower.Get_applyCount() > 0)
	{
		// �ִ� ������ ���ʽ� ���� ���
		concentration = g_pLocalUser->m_MaxConcentrationPower.Get_org();
		conbonus = g_pLocalUser->m_MaxConcentrationPower.bonus;
	}
	else if(g_pLocalUser->m_MaxConcentrationPower.Get_applyCountAdditional() > 0)
	{
		// ������ ���ʽ� ���� ���
		concentration = g_pLocalUser->m_MaxConcentrationPower.Get_org();
	}
	else
	{
		// ���ʽ� ����
		concentration = g_pLocalUser->m_MaxConcentrationPower.Get_org();
	}

	conbonus += (short)(g_pLocalUser->m_Intelligence.bonus) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].concenturate;	
	g_pLocalUser->m_MaxConcentrationPower.bonus = conbonus;
	g_pLocalUser->m_MaxConcentrationPower.Set_applyCountAdditional( g_pLocalUser->m_MaxConcentrationPower.Get_applyCountAdditional() + 1 );
}

void _XCharacterStateList::AddIntBonus(short percentage)
{
	short intel;
	FLOAT bonus = 0.f;
	if(g_pLocalUser->m_Intelligence.Get_applyCount() > 0)
	{
		intel = g_pLocalUser->m_Intelligence.Get_org();
		bonus = g_pLocalUser->m_Intelligence.bonus;
	}
	else
		intel = g_pLocalUser->m_Intelligence.Get_org();

	if( g_pLocalUser->m_ElixirInfo.Data[2].__Grade > 0 )
	{
		intel += g_pLocalUser->m_ElixirInfo.GetTotalBonus(2);
	}

	bonus += (FLOAT)intel * ((FLOAT)percentage/100.0f);

	g_pLocalUser->m_Intelligence.bonus = bonus;
	g_pLocalUser->m_Intelligence.Set_applyCount( g_pLocalUser->m_Intelligence.Get_applyCount() + 1 );

	// �ִ� ����
	short concentration = 0;
	FLOAT conbonus = 0;
	if(g_pLocalUser->m_MaxConcentrationPower.Get_applyCount() > 0)
	{
		// �ִ� ������ ���ʽ� ���� ���
		concentration = g_pLocalUser->m_MaxConcentrationPower.Get_org();
		conbonus = g_pLocalUser->m_MaxConcentrationPower.bonus;
	}
	else if(g_pLocalUser->m_MaxConcentrationPower.Get_applyCountAdditional() > 0)
	{
		// ������ ���ʽ� ���� ���
		concentration = g_pLocalUser->m_MaxConcentrationPower.Get_org();
	}
	else
	{
		// ���ʽ� ����
		concentration = g_pLocalUser->m_MaxConcentrationPower.Get_org();
	}
	
	conbonus += (short)(g_pLocalUser->m_Intelligence.bonus) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].concenturate;
	
	g_pLocalUser->m_MaxConcentrationPower.bonus = conbonus;
	g_pLocalUser->m_MaxConcentrationPower.Set_applyCountAdditional( g_pLocalUser->m_MaxConcentrationPower.Get_applyCountAdditional() + 1 );
}

void _XCharacterStateList::AddConstitutionBonus(short id, short val)
{
	short constitution = 0;
	FLOAT bonus = 0.f;
	if(g_pLocalUser->m_Constitution.Get_applyCount() > 0)
	{
		constitution = g_pLocalUser->m_Constitution.Get_org();
		bonus = g_pLocalUser->m_Constitution.bonus;
	}
	else
		constitution = g_pLocalUser->m_Constitution.Get_org();
	
#ifdef _XSTATE_DEBUG
	_XDWINPRINT("�ǰ� ���ʽ� ==============================");
	_XDWINPRINT("id:%d / value:%d", id, val);
	_XDWINPRINT("cons:%d / bonus:%f", constitution, bonus);
#endif

	if( g_pLocalUser->m_ElixirInfo.Data[3].__Grade > 0 )
	{
		constitution += g_pLocalUser->m_ElixirInfo.GetTotalBonus(3);
#ifdef _XSTATE_DEBUG
		_XDWINPRINT("����:%d / cons:%d", g_pLocalUser->m_ElixirInfo.GetTotalBonus(3), constitution);
#endif
	}

	// �ǰ�
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += constitution * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= constitution * (val/100.0f);
		}
		break;
	}

	//Author : ����� //breif : ����� ���� ������ �ٲ�
	g_pLocalUser->m_Constitution.Set_applyCount( g_pLocalUser->m_Constitution.Get_applyCount() + 1 );
	//g_pLocalUser->m_Constitution.bonus = bonus;

#ifdef _XSTATE_DEBUG
	_XDWINPRINT("cons applycount:%d / bonus:%f", g_pLocalUser->m_Constitution.Get_applyCount(), bonus);
#endif

	// �ִ� ����
	short life = 0;
	FLOAT lifebonus = 0;
	if(g_pLocalUser->m_MaxLifePower.Get_applyCount() > 0)
	{
		// �ִ� ���� ���ʽ� ���� ���
		life = g_pLocalUser->m_MaxLifePower.Get_org();
		lifebonus = g_pLocalUser->m_MaxLifePower.bonus;

		//Author : ����� //breif : ���� �޴� �κп���.. //Date : 08/02/15
		//���� ���ʽ� �޾Ҵµ� g_pLocalUser->m_MaxLifePower.bonus �̼�ġ�� �����ʽ�+�����������ʽ� ���Ե� ���� 
		//�׷��� ���Ŀ� �̰����� ������ �Ǹ� �߰��� ���⸸ ����ؼ� �ٿ������ (% �� �����Ǵ� ��쵵 �����Ƿ� )
		//���� ���� ��ġ ���� ����
		lifebonus -= (short)(g_pLocalUser->m_Constitution.bonus) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].vitality; 
	}
	else if(g_pLocalUser->m_MaxLifePower.Get_applyCountAdditional() > 0)
	{
		// �ǰ��� ���ʽ� ���� ���
		life = g_pLocalUser->m_MaxLifePower.Get_org();
	}
	else 
	{
		// ���ʽ� ����
		life = g_pLocalUser->m_MaxLifePower.Get_org();
	}

	g_pLocalUser->m_Constitution.bonus = bonus;

#ifdef _XSTATE_DEBUG
	_XDWINPRINT("�����ִ���� ==============================");
	_XDWINPRINT("life:%d / lifebonus:%f", life, lifebonus);
#endif

	lifebonus += (short)(g_pLocalUser->m_Constitution.bonus) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].vitality;	
	g_pLocalUser->m_MaxLifePower.bonus = lifebonus;
	g_pLocalUser->m_MaxLifePower.Set_applyCountAdditional( g_pLocalUser->m_MaxLifePower.Get_applyCountAdditional() + 1 );

#ifdef _XSTATE_DEBUG
	_XDWINPRINT("�ǰ�+�ִ���� ==============================");
	_XDWINPRINT("lifebonus:%f", lifebonus);
#endif
}

void _XCharacterStateList::AddConstitutionBonus(short percentage)
{
	short constitution = 0;
	FLOAT bonus = 0.f;
	if(g_pLocalUser->m_Constitution.Get_applyCount() > 0)
	{
		constitution = g_pLocalUser->m_Constitution.Get_org();
		bonus = g_pLocalUser->m_Constitution.bonus;
	}
	else
		constitution = g_pLocalUser->m_Constitution.Get_org();

	if( g_pLocalUser->m_ElixirInfo.Data[3].__Grade > 0 )
	{
		constitution += g_pLocalUser->m_ElixirInfo.GetTotalBonus(3);
	}

	bonus += (FLOAT)constitution * ((FLOAT)percentage/100.0f);

	g_pLocalUser->m_Constitution.Set_applyCount( g_pLocalUser->m_Constitution.Get_applyCount() + 1 );
	g_pLocalUser->m_Constitution.bonus = bonus;

	// �ִ� ����
	short life = 0;
	FLOAT lifebonus = 0;
	if(g_pLocalUser->m_MaxLifePower.Get_applyCount() > 0)
	{
		// �ִ� ���� ���ʽ� ���� ���
		life = g_pLocalUser->m_MaxLifePower.Get_org();
		lifebonus = g_pLocalUser->m_MaxLifePower.bonus;
	}
	else if(g_pLocalUser->m_MaxLifePower.Get_applyCountAdditional() > 0)
	{
		// �ǰ��� ���ʽ� ���� ���Ga
		life = g_pLocalUser->m_MaxLifePower.Get_org();
	}
	else 
	{
		// ���ʽ� ����
		life = g_pLocalUser->m_MaxLifePower.Get_org();
	}
	
	lifebonus += (short)(g_pLocalUser->m_Constitution.bonus) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.Get_cClass()].vitality;
	
	g_pLocalUser->m_MaxLifePower.bonus = lifebonus;
	g_pLocalUser->m_MaxLifePower.Set_applyCountAdditional( g_pLocalUser->m_MaxLifePower.Get_applyCountAdditional() + 1 );
}

void _XCharacterStateList::AddDexBonus(short id, short val)
{
	short dex = 0;
	FLOAT bonus = 0;
	if(g_pLocalUser->m_Dex.Get_applyCount() > 0)
	{
		dex = g_pLocalUser->m_Dex.Get_org();
		bonus = g_pLocalUser->m_Dex.bonus;
	}
	else
		dex = g_pLocalUser->m_Dex.Get_org();

	if( g_pLocalUser->m_ElixirInfo.Data[4].__Grade > 0 )
	{
		dex += g_pLocalUser->m_ElixirInfo.GetTotalBonus(4);
	}

	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += dex * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= dex * (val/100.0f);
		}
		break;
	}

	g_pLocalUser->m_Dex.Set_applyCount( g_pLocalUser->m_Dex.Get_applyCount() + 1 );
	g_pLocalUser->m_Dex.bonus = bonus;
}

void _XCharacterStateList::AddDexBonus(short percentage)
{
	short dex = 0;
	FLOAT bonus = 0;
	if(g_pLocalUser->m_Dex.Get_applyCount() > 0)
	{
		dex = g_pLocalUser->m_Dex.Get_org();
		bonus = g_pLocalUser->m_Dex.bonus;
	}
	else
		dex = g_pLocalUser->m_Dex.Get_org();

	if( g_pLocalUser->m_ElixirInfo.Data[4].__Grade > 0 )
	{
		dex += g_pLocalUser->m_ElixirInfo.GetTotalBonus(4);
	}

	bonus += (FLOAT)dex * ((FLOAT)percentage/100.0f);

	g_pLocalUser->m_Dex.Set_applyCount( g_pLocalUser->m_Dex.Get_applyCount() + 1 );
	g_pLocalUser->m_Dex.bonus = bonus;
}

void _XCharacterStateList::AddPAttackRateBonus(short id, short val)
{	
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_P_AttackRate.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_P_AttackRate.Get_org();
		bonus = g_pLocalUser->m_P_AttackRate.bonus;
	}
	else
	{
		org = g_pLocalUser->m_P_AttackRate.Get_org();
	}
	
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += org * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= org * (val/100.0f);
		}
		break;
	}
		
	g_pLocalUser->m_P_AttackRate.Set_applyCount( g_pLocalUser->m_P_AttackRate.Get_applyCount() + 1 );
	g_pLocalUser->m_P_AttackRate.bonus = bonus;
}

void _XCharacterStateList::AddPAttackRateBonus(short percentage)
{	
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_P_AttackRate.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_P_AttackRate.Get_org();
		bonus = g_pLocalUser->m_P_AttackRate.bonus;
	}
	else
	{
		org = g_pLocalUser->m_P_AttackRate.Get_org();
	}
	
	bonus += (FLOAT)org * ((FLOAT)percentage/100.0f);
	
	g_pLocalUser->m_P_AttackRate.Set_applyCount( g_pLocalUser->m_P_AttackRate.Get_applyCount() + 1 );
	g_pLocalUser->m_P_AttackRate.bonus = bonus;
}

void _XCharacterStateList::AddPAvoidRateBonus(short id, short val)
{
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_P_AvoidRate.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_P_AvoidRate.Get_org();
		bonus = g_pLocalUser->m_P_AvoidRate.bonus;
	}
	else 
	{
		org = g_pLocalUser->m_P_AvoidRate.Get_org();
	}
		
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += org * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= org * (val/100.0f);
		}
		break;
	}
	
	g_pLocalUser->m_P_AvoidRate.bonus = bonus;
	g_pLocalUser->m_P_AvoidRate.Set_applyCount( g_pLocalUser->m_P_AvoidRate.Get_applyCount() + 1 );
}

void _XCharacterStateList::AddPAvoidRateBonus(short percentage)
{
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_P_AvoidRate.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_P_AvoidRate.Get_org();
		bonus = g_pLocalUser->m_P_AvoidRate.bonus;
	}
	else 
	{
		org = g_pLocalUser->m_P_AvoidRate.Get_org();
	}
	
	bonus += (FLOAT)org * ((FLOAT)percentage/100.0f);
	
	g_pLocalUser->m_P_AvoidRate.bonus = bonus;
	g_pLocalUser->m_P_AvoidRate.Set_applyCount( g_pLocalUser->m_P_AvoidRate.Get_applyCount() + 1 );
}

void _XCharacterStateList::AddPAttackDamageBonus(short id, short val)
{
#ifdef _XTS_BASEMINMAXWEAPONDAMAGE

	FLOAT bonus_min = 0, bonus_max = 0;
	short org_min = 0, org_max = 0;

	if(g_pLocalUser->m_P_AttackDamage.Get_applyCount() > 0)
	{
		org_min = g_pLocalUser->m_P_AttackDamage.org_min;
		org_max = g_pLocalUser->m_P_AttackDamage.org_max;

		bonus_min = g_pLocalUser->m_P_AttackDamage.bonus_min;
		bonus_max = g_pLocalUser->m_P_AttackDamage.bonus_max;
	}
	else
	{
		org_min = g_pLocalUser->m_P_AttackDamage.org_min;
		org_max = g_pLocalUser->m_P_AttackDamage.org_max;
	}

	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus_min += val;
			bonus_max += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus_min += org_min * (val/100.0f);
			bonus_max += org_max * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus_min -= val;
			bonus_max -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus_min -= org_min * (val/100.0f);
			bonus_max -= org_max * (val/100.0f);
		}
		break;
	}

	g_pLocalUser->m_P_AttackDamage.bonus_min = bonus_min;
	g_pLocalUser->m_P_AttackDamage.bonus_max = bonus_max;
	g_pLocalUser->m_P_AttackDamage.Set_applyCount( g_pLocalUser->m_P_AttackDamage.Get_applyCount() + 1 );

#else //_XTS_BASEMINMAXWEAPONDAMAGE

	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_P_AttackDamage.applyCount > 0)
	{
		org = g_pLocalUser->m_P_AttackDamage.org;
		bonus = g_pLocalUser->m_P_AttackDamage.bonus;
	}
	else
	{
		org = g_pLocalUser->m_P_AttackDamage.org;
	}
	
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += org * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= org * (val/100.0f);
		}
		break;
	}
	
	g_pLocalUser->m_P_AttackDamage.bonus = bonus;
	g_pLocalUser->m_P_AttackDamage.applyCount++;

#endif //_XTS_BASEMINMAXWEAPONDAMAGE
}

void _XCharacterStateList::AddPAttackDamageBonus(short percentage)
{
#ifdef _XTS_BASEMINMAXWEAPONDAMAGE

	FLOAT bonus_min = 0, bonus_max = 0;
	short org_min = 0, org_max = 0;

	if(g_pLocalUser->m_P_AttackDamage.Get_applyCount() > 0)
	{
		org_min = g_pLocalUser->m_P_AttackDamage.org_min;
		org_max = g_pLocalUser->m_P_AttackDamage.org_max;

		bonus_min = g_pLocalUser->m_P_AttackDamage.bonus_min;
		bonus_max = g_pLocalUser->m_P_AttackDamage.bonus_max;
	}
	else
	{
		org_min = g_pLocalUser->m_P_AttackDamage.org_min;
		org_max = g_pLocalUser->m_P_AttackDamage.org_max;
	}
	
	bonus_min += (FLOAT)org_min * ((FLOAT)percentage/100.0f);
	bonus_max += (FLOAT)org_max * ((FLOAT)percentage/100.0f);
	
	g_pLocalUser->m_P_AttackDamage.bonus_min = bonus_min;
	g_pLocalUser->m_P_AttackDamage.bonus_max = bonus_max;
	g_pLocalUser->m_P_AttackDamage.Set_applyCount( g_pLocalUser->m_P_AttackDamage.Get_applyCount() + 1 );

#else //_XTS_BASEMINMAXWEAPONDAMAGE

	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_P_AttackDamage.applyCount > 0)
	{
		org = g_pLocalUser->m_P_AttackDamage.org;
		bonus = g_pLocalUser->m_P_AttackDamage.bonus;
	}
	else
	{
		org = g_pLocalUser->m_P_AttackDamage.org;
	}
	
	bonus += (FLOAT)org * ((FLOAT)percentage/100.0f);
	
	g_pLocalUser->m_P_AttackDamage.bonus = bonus;
	g_pLocalUser->m_P_AttackDamage.applyCount++;

#endif //_XTS_BASEMINMAXWEAPONDAMAGE
}

void _XCharacterStateList::AddDefenceBonus(short id, short val)
{
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_P_Defence.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_P_Defence.Get_org();
		bonus = g_pLocalUser->m_P_Defence.bonus;
	}
	else
	{
		org = g_pLocalUser->m_P_Defence.Get_org();
	}
	
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += org * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= org * (val/100.0f);
		}
		break;
	}
	
	g_pLocalUser->m_P_Defence.bonus = bonus;
	g_pLocalUser->m_P_Defence.Set_applyCount( g_pLocalUser->m_P_Defence.Get_applyCount() + 1 );
}

void _XCharacterStateList::AddDefenceBonus(short percentage)
{
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_P_Defence.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_P_Defence.Get_org();
		bonus = g_pLocalUser->m_P_Defence.bonus;
	}
	else
	{
		org = g_pLocalUser->m_P_Defence.Get_org();
	}
	
	bonus += (FLOAT)org * ((FLOAT)percentage/100.0f);
	
	g_pLocalUser->m_P_Defence.bonus = bonus;
	g_pLocalUser->m_P_Defence.Set_applyCount( g_pLocalUser->m_P_Defence.Get_applyCount() + 1 );
}

void _XCharacterStateList::AddMAttackRateBonus(short id, short val)
{
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_M_AttackRate.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_M_AttackRate.Get_org();
		bonus = g_pLocalUser->m_M_AttackRate.bonus;
	}
	else
	{
		org = g_pLocalUser->m_M_AttackRate.Get_org();
	}
	
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += org * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= org * (val/100.0f);
		}
		break;
	}
	
	g_pLocalUser->m_M_AttackRate.bonus = bonus;
	g_pLocalUser->m_M_AttackRate.Set_applyCount( g_pLocalUser->m_M_AttackRate.Get_applyCount() + 1 );
}

void _XCharacterStateList::AddMAttackRateBonus(short percentage)
{
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_M_AttackRate.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_M_AttackRate.Get_org();
		bonus = g_pLocalUser->m_M_AttackRate.bonus;
	}
	else
	{
		org = g_pLocalUser->m_M_AttackRate.Get_org();
	}
	
	bonus += (FLOAT)org * ((FLOAT)percentage/100.0f);
	
	g_pLocalUser->m_M_AttackRate.bonus = bonus;
	g_pLocalUser->m_M_AttackRate.Set_applyCount( g_pLocalUser->m_M_AttackRate.Get_applyCount() + 1 );
}

void _XCharacterStateList::AddMDefenceBonus(short id, short val)
{
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_M_Defence.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_M_Defence.Get_org();
		bonus = g_pLocalUser->m_M_Defence.bonus;
	}
	else
	{
		org = g_pLocalUser->m_M_Defence.Get_org();
	}
	
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += org * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= org * (val/100.0f);
		}
		break;
	}
	
	g_pLocalUser->m_M_Defence.bonus = bonus;
	g_pLocalUser->m_M_Defence.Set_applyCount( g_pLocalUser->m_M_Defence.Get_applyCount() + 1 );
}

void _XCharacterStateList::AddMDefenceBonus(short percentage)
{
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_M_Defence.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_M_Defence.Get_org();
		bonus = g_pLocalUser->m_M_Defence.bonus;
	}
	else
	{
		org = g_pLocalUser->m_M_Defence.Get_org();
	}
	
	bonus += (FLOAT)org * ((FLOAT)percentage/100.0f);
	
	g_pLocalUser->m_M_Defence.bonus = bonus;
	g_pLocalUser->m_M_Defence.Set_applyCount( g_pLocalUser->m_M_Defence.Get_applyCount() + 1 );
}

void _XCharacterStateList::AddMAvoidRateBonus(short id, short val)
{
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_M_AvoidRate.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_M_AvoidRate.Get_org();
		bonus = g_pLocalUser->m_M_AvoidRate.bonus;
	}
	else
	{
		org = g_pLocalUser->m_M_AvoidRate.Get_org();
	}
	
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += org * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= org * (val/100.0f);
		}
		break;
	}
	
	g_pLocalUser->m_M_AvoidRate.bonus = bonus;
	g_pLocalUser->m_M_AvoidRate.Set_applyCount( g_pLocalUser->m_M_AvoidRate.Get_applyCount() + 1 );
}

void _XCharacterStateList::AddMAvoidRateBonus(short percentage)
{
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_M_AvoidRate.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_M_AvoidRate.Get_org();
		bonus = g_pLocalUser->m_M_AvoidRate.bonus;
	}
	else
	{
		org = g_pLocalUser->m_M_AvoidRate.Get_org();
	}
	
	bonus += (FLOAT)org * ((FLOAT)percentage/100.0f);
	
	g_pLocalUser->m_M_AvoidRate.bonus = bonus;
	g_pLocalUser->m_M_AvoidRate.Set_applyCount( g_pLocalUser->m_M_AvoidRate.Get_applyCount() + 1 );
}

void _XCharacterStateList::AddMAttackDamageBonus(short id, short val)
{
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_M_AttackDamage.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_M_AttackDamage.Get_org();
		bonus = g_pLocalUser->m_M_AttackDamage.bonus;
	}
	else
	{
		org = g_pLocalUser->m_M_AttackDamage.Get_org();
	}
	
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += org * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= org * (val/100.0f);
		}
		break;
	}
	
	g_pLocalUser->m_M_AttackDamage.bonus = bonus;
	g_pLocalUser->m_M_AttackDamage.Set_applyCount( g_pLocalUser->m_M_AttackDamage.Get_applyCount() + 1 );
}

void _XCharacterStateList::AddMAttackDamageBonus(short percentage)
{
	FLOAT bonus = 0;
	short org = 0;
	if(g_pLocalUser->m_M_AttackDamage.Get_applyCount() > 0)
	{
		org = g_pLocalUser->m_M_AttackDamage.Get_org();
		bonus = g_pLocalUser->m_M_AttackDamage.bonus;
	}
	else
	{
		org = g_pLocalUser->m_M_AttackDamage.Get_org();
	}
	
	bonus += (FLOAT)org * ((FLOAT)percentage/100.0f);
	
	g_pLocalUser->m_M_AttackDamage.bonus = bonus;
	g_pLocalUser->m_M_AttackDamage.Set_applyCount( g_pLocalUser->m_M_AttackDamage.Get_applyCount() + 1 );
}

void _XCharacterStateList::AddMaxLifeBonus(short id, short val)
{
	short life = 0;
	FLOAT bonus = 0;
	if(g_pLocalUser->m_MaxLifePower.Get_applyCount() > 0 || g_pLocalUser->m_MaxLifePower.Get_applyCountAdditional() > 0)
	{
		life = g_pLocalUser->m_MaxLifePower.Get_org();
		bonus = g_pLocalUser->m_MaxLifePower.bonus;
	}
	else
		life = g_pLocalUser->m_MaxLifePower.Get_org();

#ifdef _XSTATE_DEBUG
	_XDWINPRINT("�ִ���� ���ʽ� ==============================");
	_XDWINPRINT("id:%d / value:%d", id, val);
	_XDWINPRINT("life:%d / bonus:%f", life, bonus);
#endif

	// �ִ� ������ % bonus ���� ���� ���� �����ؼ� ������� ����
//	if( g_pLocalUser->m_ElixirInfo.Data[3].__Grade > 0 )
//	{
//		life += g_pLocalUser->m_ElixirInfo.GetTotalBonus(3) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.cClass].vitality;
//	}

	
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += life * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= life * (val/100.0f);
		}
		break;
	}
	
	g_pLocalUser->m_MaxLifePower.Set_applyCount( g_pLocalUser->m_MaxLifePower.Get_applyCount() + 1 );
	g_pLocalUser->m_MaxLifePower.bonus = bonus;

#ifdef _XSTATE_DEBUG
	_XDWINPRINT("�ִ���� ���ʽ� ==============================");
	_XDWINPRINT("lifebonus:%f", bonus);
#endif
}

void _XCharacterStateList::AddMaxLifeBonus(short percentage)
{
	short life = 0;
	FLOAT bonus = 0;
	if(g_pLocalUser->m_MaxLifePower.Get_applyCount() > 0 || g_pLocalUser->m_MaxLifePower.Get_applyCountAdditional() > 0)
	{
		life = g_pLocalUser->m_MaxLifePower.Get_org();
		bonus = g_pLocalUser->m_MaxLifePower.bonus;
	}
	else
		life = g_pLocalUser->m_MaxLifePower.Get_org();

	// �ִ� ������ % bonus ���� ���� ���� �����ؼ� ������� ����
	//if( g_pLocalUser->m_ElixirInfo.Data[3].__Grade > 0 )
	//	{
	//		life += g_pLocalUser->m_ElixirInfo.GetTotalBonus(3) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.cClass].vitality;
	//	}
	
	
	bonus += (FLOAT)life * ((FLOAT)percentage/100.0f);
	
	g_pLocalUser->m_MaxLifePower.Set_applyCount( g_pLocalUser->m_MaxLifePower.Get_applyCount() + 1 );
	g_pLocalUser->m_MaxLifePower.bonus = bonus;
}

void _XCharacterStateList::AddMaxForceBonus(short id, short val)
{
	short force = 0;
	FLOAT bonus = 0;
	if(g_pLocalUser->m_MaxForcePower.Get_applyCount() > 0 || g_pLocalUser->m_MaxForcePower.Get_applyCountAdditional() > 0)
	{
		force = g_pLocalUser->m_MaxForcePower.Get_org();
		bonus = g_pLocalUser->m_MaxForcePower.bonus;
	}
	else
		force = g_pLocalUser->m_MaxForcePower.Get_org();

	// �ִ� ������ % bonus ���� ���� ���� �����ؼ� ������� ����
	//if( g_pLocalUser->m_ElixirInfo.Data[1].__Grade > 0 )
	//	{
	//		force += g_pLocalUser->m_ElixirInfo.GetTotalBonus(1) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.cClass].inner;
	//	}
	
	
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += force * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= force * (val/100.0f);
		}
		break;
	}
	
	g_pLocalUser->m_MaxForcePower.Set_applyCount( g_pLocalUser->m_MaxForcePower.Get_applyCount() + 1 );
	g_pLocalUser->m_MaxForcePower.bonus = bonus;
}

void _XCharacterStateList::AddMaxForceBonus(short percentage)
{
	short force = 0;
	FLOAT bonus = 0;
	if(g_pLocalUser->m_MaxForcePower.Get_applyCount() > 0 || g_pLocalUser->m_MaxForcePower.Get_applyCountAdditional() > 0)
	{
		force = g_pLocalUser->m_MaxForcePower.Get_org();
		bonus = g_pLocalUser->m_MaxForcePower.bonus;
	}
	else
		force = g_pLocalUser->m_MaxForcePower.Get_org();

	// �ִ� ������ % bonus ���� ���� ���� �����ؼ� ������� ����
	//if( g_pLocalUser->m_ElixirInfo.Data[1].__Grade > 0 )
	//	{
	//		force += g_pLocalUser->m_ElixirInfo.GetTotalBonus(1) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.cClass].inner;
	//	}
	
	
	bonus += (FLOAT)force * ((FLOAT)percentage/100.0f);
	
	g_pLocalUser->m_MaxForcePower.Set_applyCount( g_pLocalUser->m_MaxForcePower.Get_applyCount() + 1 );
	g_pLocalUser->m_MaxForcePower.bonus = bonus;
}

void _XCharacterStateList::AddMaxConcentrationBonus(short id, short val)
{
	short concentration = 0;
	FLOAT bonus = 0;
	if(g_pLocalUser->m_MaxConcentrationPower.Get_applyCount() > 0 || g_pLocalUser->m_MaxConcentrationPower.Get_applyCountAdditional() > 0)
	{
		concentration = g_pLocalUser->m_MaxConcentrationPower.Get_org();
		bonus = g_pLocalUser->m_MaxConcentrationPower.bonus;
	}
	else
		concentration = g_pLocalUser->m_MaxConcentrationPower.Get_org();

	// �ִ� ������ % bonus ���� ���� ���� �����ؼ� ������� ����
	//if( g_pLocalUser->m_ElixirInfo.Data[2].__Grade > 0 )
	//	{
	//		concentration += g_pLocalUser->m_ElixirInfo.GetTotalBonus(2) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.cClass].intelligence;
	//	}
	
	
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			bonus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			bonus += concentration * (val/100.0f);
		}
		break;
	case en_eff_vt_sub_value :
		{
			bonus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			bonus -= concentration * (val/100.0f);
		}
		break;
	}
	
	g_pLocalUser->m_MaxConcentrationPower.Set_applyCount( g_pLocalUser->m_MaxConcentrationPower.Get_applyCount() + 1 );
	g_pLocalUser->m_MaxConcentrationPower.bonus = bonus;
}

void _XCharacterStateList::AddMaxConcentrationBonus(short percentage)
{
	short concentration = 0;
	FLOAT bonus = 0;
	if(g_pLocalUser->m_MaxConcentrationPower.Get_applyCount() > 0 || g_pLocalUser->m_MaxConcentrationPower.Get_applyCountAdditional() > 0)
	{
		concentration = g_pLocalUser->m_MaxConcentrationPower.Get_org();
		bonus = g_pLocalUser->m_MaxConcentrationPower.bonus;
	}
	else
		concentration = g_pLocalUser->m_MaxConcentrationPower.Get_org();

	// �ִ� ������ % bonus ���� ���� ���� �����ؼ� ������� ����
	//if( g_pLocalUser->m_ElixirInfo.Data[2].__Grade > 0 )
	//	{
	//		concentration += g_pLocalUser->m_ElixirInfo.GetTotalBonus(2) * g_UsePointTable[g_pLocalUser->m_CharacterInfo.cClass].intelligence;
	//	}
	
	
	bonus += (FLOAT)concentration * ((FLOAT)percentage/100.0f);
	
	g_pLocalUser->m_MaxConcentrationPower.Set_applyCount( g_pLocalUser->m_MaxConcentrationPower.Get_applyCount() + 1 );
	g_pLocalUser->m_MaxConcentrationPower.bonus = bonus;
}

void _XCharacterStateList::AddMoveSpeedBonus(short id, short val)
{
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_percent :
		{
			//g_pLocalUser->m_MoveSpeedFromEffect += val;
			g_pLocalUser->Set_m_MoveSpeedFromEffect(  g_pLocalUser->Get_m_MoveSpeedFromEffect() + val );
		}
		break;
	case en_eff_vt_sub_percent :
		{
			//g_pLocalUser->m_MoveSpeedFromEffect += (val*(-1));
			g_pLocalUser->Set_m_MoveSpeedFromEffect(  g_pLocalUser->Get_m_MoveSpeedFromEffect() + (val*(-1)) );
		}
		break;
	}
}

void _XCharacterStateList::AddMoveSpeedBonus(short percentage)
{
	//g_pLocalUser->m_MoveSpeedFromEffect += percentage;
	g_pLocalUser->Set_m_MoveSpeedFromEffect(  g_pLocalUser->Get_m_MoveSpeedFromEffect() + percentage );
}

void _XCharacterStateList::AddWeaponDamage(short id, short val)
{
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			g_pLocalUser->m_WeaponMinDamageBonus[0] += val;
			g_pLocalUser->m_WeaponMaxDamageBonus[0] += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			g_pLocalUser->m_WeaponMinDamageBonus[1] += val;
			g_pLocalUser->m_WeaponMaxDamageBonus[1] += val;
		}
		break;
	case en_eff_vt_sub_value :
		{
			g_pLocalUser->m_WeaponMinDamageBonus[0] -= val;
			g_pLocalUser->m_WeaponMaxDamageBonus[0] -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			g_pLocalUser->m_WeaponMinDamageBonus[1] -= val;
			g_pLocalUser->m_WeaponMaxDamageBonus[1] -= val;
		}
		break;
	}
}

void _XCharacterStateList::AddWeaponDamage(short percentage)
{
	g_pLocalUser->m_WeaponMinDamageBonus[1] += percentage;
	g_pLocalUser->m_WeaponMaxDamageBonus[1] += percentage;
}

void _XCharacterStateList::AddWeaponMinDamage(short id, short val)
{
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			g_pLocalUser->m_WeaponMinDamageBonus[0] += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			g_pLocalUser->m_WeaponMinDamageBonus[1] += val;
		}
		break;
	case en_eff_vt_sub_value :
		{
			g_pLocalUser->m_WeaponMinDamageBonus[0] -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			g_pLocalUser->m_WeaponMinDamageBonus[1] -= val;
		}
		break;
	}
}

void _XCharacterStateList::AddWeaponMinDamage(short percentage)
{
	g_pLocalUser->m_WeaponMinDamageBonus[1] += percentage;
}

void _XCharacterStateList::AddWeaponMaxDamage(short id, short val)
{
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			g_pLocalUser->m_WeaponMaxDamageBonus[0] += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			g_pLocalUser->m_WeaponMaxDamageBonus[1] += val;
		}
		break;
	case en_eff_vt_sub_value :
		{
			g_pLocalUser->m_WeaponMaxDamageBonus[0] -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			g_pLocalUser->m_WeaponMaxDamageBonus[1] -= val;
		}
		break;
	}
}

void _XCharacterStateList::AddWeaponMaxDamage(short percentage)
{
	g_pLocalUser->m_WeaponMaxDamageBonus[1] += percentage;
}

void _XCharacterStateList::AddDisableSkill(short id, short val)
{
	g_pLocalUser->m_Lock.disableskill = 1;
	g_pLocalUser->m_Lock.disableskillindex = val;
}

void _XCharacterStateList::AddDisableSType(short id, short val)
{
	g_pLocalUser->m_Lock.disablestype = 1;
	g_pLocalUser->m_Lock.disablestypeindex = val;
}

void _XCharacterStateList::AddResistMinus(short id, short val)
{
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			g_pLocalUser->m_ResistMinus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			g_pLocalUser->m_ResistMinus += val;
		}
		break;
	case en_eff_vt_sub_value :
		{
			g_pLocalUser->m_ResistMinus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			g_pLocalUser->m_ResistMinus -= val;
		}
		break;
	}
}

void _XCharacterStateList::AddResistMinus(short percentage)
{
	FLOAT bonus = (FLOAT)g_pLocalUser->m_ResistMinus * ((FLOAT)percentage/100.0f);
	g_pLocalUser->m_ResistMinus = g_pLocalUser->m_ResistMinus + (int)bonus;
}

void _XCharacterStateList::AddResistPlus(short id, short val)
{
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			g_pLocalUser->m_ResistPlus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			g_pLocalUser->m_ResistPlus += val;
		}
		break;
	case en_eff_vt_sub_value :
		{
			g_pLocalUser->m_ResistPlus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			g_pLocalUser->m_ResistPlus -= val;
		}
		break;
	}
}

void _XCharacterStateList::AddResistPlus(short percentage)
{
	FLOAT bonus = (FLOAT)g_pLocalUser->m_ResistPlus * ((FLOAT)percentage/100.0f);
	g_pLocalUser->m_ResistPlus = g_pLocalUser->m_ResistPlus + (int)bonus;
}

void _XCharacterStateList::AddResistHon(short id, short val)
{
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			g_pLocalUser->m_ResistHon += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			g_pLocalUser->m_ResistHon += val;
		}
		break;
	case en_eff_vt_sub_value :
		{
			g_pLocalUser->m_ResistHon -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			g_pLocalUser->m_ResistHon -= val;
		}
		break;
	}
}

void _XCharacterStateList::AddResistHon(short percentage)
{
	FLOAT bonus = (FLOAT)g_pLocalUser->m_ResistHon * ((FLOAT)percentage/100.0f);
	g_pLocalUser->m_ResistHon = g_pLocalUser->m_ResistHon + (int)bonus;
}

void _XCharacterStateList::AddResistSin(short id, short val)
{
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			g_pLocalUser->m_ResistSin += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			g_pLocalUser->m_ResistSin += val;
		}
		break;
	case en_eff_vt_sub_value :
		{
			g_pLocalUser->m_ResistSin -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			g_pLocalUser->m_ResistSin -= val;
		}
		break;
	}
}

void _XCharacterStateList::AddResistSin(short percentage)
{
	FLOAT bonus = (FLOAT)g_pLocalUser->m_ResistSin * ((FLOAT)percentage/100.0f);
	g_pLocalUser->m_ResistSin = g_pLocalUser->m_ResistSin + (int)bonus;
}

void _XCharacterStateList::AddResistAll(short id, short val)
{
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			g_pLocalUser->m_ResistMinus += val;
			g_pLocalUser->m_ResistPlus += val;
			g_pLocalUser->m_ResistHon += val;
			g_pLocalUser->m_ResistSin += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			g_pLocalUser->m_ResistMinus += val;
			g_pLocalUser->m_ResistPlus += val;
			g_pLocalUser->m_ResistHon += val;
			g_pLocalUser->m_ResistSin += val;
		}
		break;
	case en_eff_vt_sub_value :
		{
			g_pLocalUser->m_ResistMinus -= val;
			g_pLocalUser->m_ResistPlus -= val;
			g_pLocalUser->m_ResistHon -= val;
			g_pLocalUser->m_ResistSin -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			g_pLocalUser->m_ResistMinus -= val;
			g_pLocalUser->m_ResistPlus -= val;
			g_pLocalUser->m_ResistHon -= val;
			g_pLocalUser->m_ResistSin -= val;
		}
		break;
	}
}

void _XCharacterStateList::AddResistAll(short percentage)
{
	FLOAT bonus = (FLOAT)g_pLocalUser->m_ResistMinus * ((FLOAT)percentage/100.0f);
	g_pLocalUser->m_ResistMinus = g_pLocalUser->m_ResistMinus + (int)bonus;

	bonus = (FLOAT)g_pLocalUser->m_ResistPlus * ((FLOAT)percentage/100.0f);
	g_pLocalUser->m_ResistPlus = g_pLocalUser->m_ResistPlus + (int)bonus;

	bonus = (FLOAT)g_pLocalUser->m_ResistHon * ((FLOAT)percentage/100.0f);
	g_pLocalUser->m_ResistHon = g_pLocalUser->m_ResistHon + (int)bonus;

	bonus = (FLOAT)g_pLocalUser->m_ResistSin * ((FLOAT)percentage/100.0f);
	g_pLocalUser->m_ResistSin = g_pLocalUser->m_ResistSin + (int)bonus;
}

void _XCharacterStateList::AddAttackDamageMinus(short id, short val)
{
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			g_pLocalUser->m_AttackDamageMinus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			g_pLocalUser->m_AttackDamageMinus += val;
		}
		break;
	case en_eff_vt_sub_value :
		{
			g_pLocalUser->m_AttackDamageMinus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			g_pLocalUser->m_AttackDamageMinus -= val;
		}
		break;
	}
}

void _XCharacterStateList::AddAttackDamageMinus(short percentage)
{
	FLOAT bonus = (FLOAT)g_pLocalUser->m_AttackDamageMinus * ((FLOAT)percentage/100.0f);
	g_pLocalUser->m_AttackDamageMinus = g_pLocalUser->m_AttackDamageMinus + (int)bonus;
}

void _XCharacterStateList::AddAttackDamagePlus(short id, short val)
{
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			g_pLocalUser->m_AttackDamagePlus += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			g_pLocalUser->m_AttackDamagePlus += val;
		}
		break;
	case en_eff_vt_sub_value :
		{
			g_pLocalUser->m_AttackDamagePlus -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			g_pLocalUser->m_AttackDamagePlus -= val;
		}
		break;
	}
}

void _XCharacterStateList::AddAttackDamagePlus(short percentage)
{
	FLOAT bonus = (FLOAT)g_pLocalUser->m_AttackDamagePlus * ((FLOAT)percentage/100.0f);
	g_pLocalUser->m_AttackDamagePlus = g_pLocalUser->m_AttackDamagePlus + (int)bonus;
}

void _XCharacterStateList::AddAttackDamageHon(short id, short val)
{
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			g_pLocalUser->m_AttackDamageHon += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			g_pLocalUser->m_AttackDamageHon += val;
		}
		break;
	case en_eff_vt_sub_value :
		{
			g_pLocalUser->m_AttackDamageHon -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			g_pLocalUser->m_AttackDamageHon -= val;
		}
		break;
	}
}

void _XCharacterStateList::AddAttackDamageHon(short percentage)
{
	FLOAT bonus = (FLOAT)g_pLocalUser->m_AttackDamageHon * ((FLOAT)percentage/100.0f);
	g_pLocalUser->m_AttackDamageHon = g_pLocalUser->m_AttackDamageHon + (int)bonus;
}

void _XCharacterStateList::AddAttackDamageSin(short id, short val)
{
	switch(g_CharacterStateTable[id]->value_type)
	{
	case en_eff_vt_add_value :
		{
			g_pLocalUser->m_AttackDamageSin += val;
		}
		break;
	case en_eff_vt_add_percent :
		{
			g_pLocalUser->m_AttackDamageSin += val;
		}
		break;
	case en_eff_vt_sub_value :
		{
			g_pLocalUser->m_AttackDamageSin -= val;
		}
		break;
	case en_eff_vt_sub_percent :
		{
			g_pLocalUser->m_AttackDamageSin -= val;
		}
		break;
	}
}

void _XCharacterStateList::AddAttackDamageSin(short percentage)
{
	FLOAT bonus = (FLOAT)g_pLocalUser->m_AttackDamageSin * ((FLOAT)percentage/100.0f);
	g_pLocalUser->m_AttackDamageSin = g_pLocalUser->m_AttackDamageSin + (int)bonus;
}

void _XCharacterStateList::GetBaseBonusFromMastery(void)
{
#ifdef _XTS_MASTERY
	for(int i = 0 ; i < _XDEF_MAX_LEARNMASTERYSKILL ; ++i)
	{
		if(g_pLocalUser->m_MasterySkillList[i].skillid > 0 && g_pLocalUser->m_MasterySkillList[i].cPoint > 0)
		{
			short skillid = g_pLocalUser->m_MasterySkillList[i].skillid;
			char cPoint = g_pLocalUser->m_MasterySkillList[i].cPoint;

			if(g_SkillMasteryProperty[skillid]->cApplyType == 2)
			{
				// ĳ���� ��ȭ
				if(g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect)
				{
					for(int ii = 0 ; ii < g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effectCount ; ++ii)
					{
						short id = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].id;
						short value = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].value;

						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
						if(iter_table != g_CharacterStateTable.end())
						{
							if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
							{
								switch(g_CharacterStateTable[id]->where)
								{
								case en_eff_where_strength :		// �ٷ�
									AddStrengthBonus(id, value);
									break;
								case en_eff_where_zen :				// ����
									AddZenBonus(id, value);
									break;
								case en_eff_where_intelligence :	// ����
									AddIntBonus(id, value);
									break;
								case en_eff_where_constitution :	// �ǰ�
									AddConstitutionBonus(id, value);
									break;
								case en_eff_where_dexterity :		// ��ø
									AddDexBonus(id, value);
									break;
								case en_eff_where_all_stat :
									{
										AddStrengthBonus(id, value);
										AddZenBonus(id, value);
										AddIntBonus(id, value);
										AddConstitutionBonus(id, value);
										AddDexBonus(id, value);
									}
									break;
								}
							} // if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
						}
					} // for(ii)
				}
			}

			if(g_SkillMasteryProperty[skillid]->cApplyType == 0)
			{
				// ���� ��ȭ
				char ms_SType = g_SkillMasteryProperty[skillid]->cWeaponSType;
				char ms_TType = g_SkillMasteryProperty[skillid]->cWeaponTType;

				short user_sID = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID();
#ifdef _XTS_ITEM_OPTIMIZATION
				if( user_sID > 0 )
				{
					if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType()) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType());					
						if( WeaponItem )
						{
							char user_SType = WeaponItem[user_sID].cSecondType;
							char user_TType = WeaponItem[user_sID].cThirdType;
							if(ms_SType == user_SType && ms_TType == user_TType)
							{
								// mastery ����� ���� ������ ������
								if(g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect)
								{
									for(int ii = 0 ; ii < g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effectCount ; ++ii)
									{
										short id = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].id;
										short value = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].value;
										
										map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
										if(iter_table != g_CharacterStateTable.end())
										{
											if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
											{
												switch(g_CharacterStateTable[id]->where)
												{
												case en_eff_where_strength :		// �ٷ�
													AddStrengthBonus(id, value);
													break;
												case en_eff_where_zen :				// ����
													AddZenBonus(id, value);
													break;
												case en_eff_where_intelligence :	// ����
													AddIntBonus(id, value);
													break;
												case en_eff_where_constitution :	// �ǰ�
													AddConstitutionBonus(id, value);
													break;
												case en_eff_where_dexterity :		// ��ø
													AddDexBonus(id, value);
													break;
												case en_eff_where_all_stat :
													{
														AddStrengthBonus(id, value);
														AddZenBonus(id, value);
														AddIntBonus(id, value);
														AddConstitutionBonus(id, value);
														AddDexBonus(id, value);
													}
													break;
												}
											} // if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
										}
									} // for(ii)
								} // if(g_SkillMasteryProperty[skillid]->effect[cPoint-1])
							}
						}
					}
				}
#else
				if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_cType == _XGI_FC_WEAPON)
				{
					char user_SType = g_WeaponItemProperty[user_sID].cSecondType;
					char user_TType = g_WeaponItemProperty[user_sID].cThirdType;
					if(ms_SType == user_SType && ms_TType == user_TType)
					{
						// mastery ����� ���� ������ ������
						if(g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect)
						{
							for(int ii = 0 ; ii < g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effectCount ; ++ii)
							{
								short id = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].id;
								short value = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].value;
								
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										switch(g_CharacterStateTable[id]->where)
										{
										case en_eff_where_strength :		// �ٷ�
											AddStrengthBonus(id, value);
											break;
										case en_eff_where_zen :				// ����
											AddZenBonus(id, value);
											break;
										case en_eff_where_intelligence :	// ����
											AddIntBonus(id, value);
											break;
										case en_eff_where_constitution :	// �ǰ�
											AddConstitutionBonus(id, value);
											break;
										case en_eff_where_dexterity :		// ��ø
											AddDexBonus(id, value);
											break;
										case en_eff_where_all_stat :
											{
												AddStrengthBonus(id, value);
												AddZenBonus(id, value);
												AddIntBonus(id, value);
												AddConstitutionBonus(id, value);
												AddDexBonus(id, value);
											}
											break;
										}
									} // if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
								}
							} // for(ii)
						} // if(g_SkillMasteryProperty[skillid]->effect[cPoint-1])
					}
				} // weapon
				if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_cType == _XGI_FC_WEAPON2)
				{
					char user_SType = g_WeaponItemProperty2[user_sID].cSecondType;
					char user_TType = g_WeaponItemProperty2[user_sID].cThirdType;
					if(ms_SType == user_SType && ms_TType == user_TType)
					{
						// mastery ����� ���� ������ ������
						if(g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect)
						{
							for(int ii = 0 ; ii < g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effectCount ; ++ii)
							{
								short id = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].id;
								short value = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].value;
								
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										switch(g_CharacterStateTable[id]->where)
										{
										case en_eff_where_strength :		// �ٷ�
											AddStrengthBonus(id, value);
											break;
										case en_eff_where_zen :				// ����
											AddZenBonus(id, value);
											break;
										case en_eff_where_intelligence :	// ����
											AddIntBonus(id, value);
											break;
										case en_eff_where_constitution :	// �ǰ�
											AddConstitutionBonus(id, value);
											break;
										case en_eff_where_dexterity :		// ��ø
											AddDexBonus(id, value);
											break;
										case en_eff_where_all_stat :
											{
												AddStrengthBonus(id, value);
												AddZenBonus(id, value);
												AddIntBonus(id, value);
												AddConstitutionBonus(id, value);
												AddDexBonus(id, value);
											}
											break;
										}
									} // if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
								}
							} // for(ii)
						} // if(g_SkillMasteryProperty[skillid]->effect[cPoint-1])
					}
				} // weapon2
				if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_cType == _XGI_FC_WEAPON3)
				{
					char user_SType = g_WeaponItemProperty3[user_sID].cSecondType;
					char user_TType = g_WeaponItemProperty3[user_sID].cThirdType;
					if(ms_SType == user_SType && ms_TType == user_TType)
					{
						// mastery ����� ���� ������ ������
						if(g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect)
						{
							for(int ii = 0 ; ii < g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effectCount ; ++ii)
							{
								short id = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].id;
								short value = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].value;
								
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										switch(g_CharacterStateTable[id]->where)
										{
										case en_eff_where_strength :		// �ٷ�
											AddStrengthBonus(id, value);
											break;
										case en_eff_where_zen :				// ����
											AddZenBonus(id, value);
											break;
										case en_eff_where_intelligence :	// ����
											AddIntBonus(id, value);
											break;
										case en_eff_where_constitution :	// �ǰ�
											AddConstitutionBonus(id, value);
											break;
										case en_eff_where_dexterity :		// ��ø
											AddDexBonus(id, value);
											break;
										case en_eff_where_all_stat :
											{
												AddStrengthBonus(id, value);
												AddZenBonus(id, value);
												AddIntBonus(id, value);
												AddConstitutionBonus(id, value);
												AddDexBonus(id, value);
											}
											break;
										}
									} // if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
								}
							} // for(ii)
						} // if(g_SkillMasteryProperty[skillid]->effect[cPoint-1])
					}
				} // weapon3
#endif
			}
		} // �����
	}
#endif
}

void _XCharacterStateList::GetAdditionalBonusFromMastery(void)
{
#ifdef _XTS_MASTERY
	for(int i = 0 ; i < _XDEF_MAX_LEARNMASTERYSKILL ; ++i)
	{
		if(g_pLocalUser->m_MasterySkillList[i].skillid > 0 && g_pLocalUser->m_MasterySkillList[i].cPoint > 0)
		{
			short skillid = g_pLocalUser->m_MasterySkillList[i].skillid;
			char cPoint = g_pLocalUser->m_MasterySkillList[i].cPoint;

			if(g_SkillMasteryProperty[skillid]->cApplyType == 2)
			{
				// ĳ���� ��ȭ
				if(g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect)
				{
					for(int ii = 0 ; ii < g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effectCount ; ++ii)
					{
						short id = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].id;
						short value = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].value;
						
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
						if(iter_table != g_CharacterStateTable.end())
						{
							if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
							{
								switch(g_CharacterStateTable[id]->where)
								{
								case en_eff_where_attack_rate :				// ���� ����
									AddPAttackRateBonus(id, value);
									break;
								case en_eff_where_avoid_rate :				// ���� ȸ��
									AddPAvoidRateBonus(id, value);
									break;
								case en_eff_where_attack_damage :			// ���� ����
									AddPAttackDamageBonus(id, value);
									break;
								case en_eff_where_defence :					// ���� ���
									AddDefenceBonus(id, value);
									break;
								case en_eff_where_m_attack_rate :			// ��� ����
									AddMAttackRateBonus(id, value);
									break;
								case en_eff_where_m_avoid_rate :			// ��� ȸ��
									AddMAvoidRateBonus(id, value);
									break;
								case en_eff_where_m_attack_damage :			// ��� ����
									AddMAttackDamageBonus(id, value);
									break;
								case en_eff_where_m_defence :				// ��� ���
									AddMDefenceBonus(id, value);
									break;
								case en_eff_where_max_life :				// �ִ� ����
									AddMaxLifeBonus(id, value);
									break;
								case en_eff_where_max_force :				// �ִ� ����
									AddMaxForceBonus(id, value);
									break;
								case en_eff_where_max_concentration :		// �ִ� ����
									AddMaxConcentrationBonus(id, value);
									break;
								case en_eff_where_move_speed :				// �̵� �ӵ�
									AddMoveSpeedBonus(id, value);
									break;
								case en_eff_where_weapon_dam :				// ���� ���ݷ� ����
									AddWeaponDamage(id, value);
									break;
								case en_eff_where_weapon_min_dam :			// ���� �ּ� ���ݷ� ����
									AddWeaponMinDamage(id, value);
									break;
								case en_eff_where_weapon_max_dam :			// ���� �ִ� ���ݷ� ����
									AddWeaponMaxDamage(id, value);
									break;
								case en_eff_where_resist_minus :			// ���� ���׷�
									AddResistMinus(id, value);
									break;
								case en_eff_where_resist_plus :				// ���� ���׷�
									AddResistPlus(id, value);
									break;
								case en_eff_where_resist_hon :				// ȥ�� ���׷�
									AddResistHon(id, value);
									break;
								case en_eff_where_resist_sin :				// ���� ���׷�
									AddResistSin(id, value);
									break;
								case en_eff_where_resist_all :			// ��� ���׷� ����
									AddResistAll(id, value);
									break;
								case en_eff_where_add_minus_damage :		// ���� ������ �߰�
									AddAttackDamageMinus(id, value);
									break;
								case en_eff_where_add_plus_damage :			// ���� ������ �߰�
									AddAttackDamagePlus(id, value);
									break;
								case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
									AddAttackDamageHon(id, value);
									break;
								case en_eff_where_add_sin_damage :			// ���� ������ �߰�
									AddAttackDamageSin(id, value);
									break;
								}
							} // if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
						}
					} // for(ii)
				} // if(g_SkillMasteryProperty[skillid]->effect[cPoint-1])
			}

			if(g_SkillMasteryProperty[skillid]->cApplyType == 0)
			{
				// ���� ��ȭ
				char ms_SType = g_SkillMasteryProperty[skillid]->cWeaponSType;
				char ms_TType = g_SkillMasteryProperty[skillid]->cWeaponTType;

				short user_sID = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID();
				
				if( user_sID > 0 )
				{
					if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType()) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType());					
						if( WeaponItem )
						{
							char user_SType = WeaponItem[user_sID].cSecondType;
							char user_TType = WeaponItem[user_sID].cThirdType;
							if(ms_SType == user_SType && ms_TType == user_TType)
							{
								// mastery ����� ���� ������ ������
								if(g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect)
								{
									for(int ii = 0 ; ii < g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effectCount ; ++ii)
									{
										short id = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].id;
										short value = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].value;
										
										map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
										if(iter_table != g_CharacterStateTable.end())
										{
											if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
											{
												switch(g_CharacterStateTable[id]->where)
												{
												case en_eff_where_attack_rate :				// ���� ����
													AddPAttackRateBonus(id, value);
													break;
												case en_eff_where_avoid_rate :				// ���� ȸ��
													AddPAvoidRateBonus(id, value);
													break;
												case en_eff_where_attack_damage :			// ���� ����
													AddPAttackDamageBonus(id, value);
													break;
												case en_eff_where_defence :					// ���� ���
													AddDefenceBonus(id, value);
													break;
												case en_eff_where_m_attack_rate :			// ��� ����
													AddMAttackRateBonus(id, value);
													break;
												case en_eff_where_m_avoid_rate :			// ��� ȸ��
													AddMAvoidRateBonus(id, value);
													break;
												case en_eff_where_m_attack_damage :			// ��� ����
													AddMAttackDamageBonus(id, value);
													break;
												case en_eff_where_m_defence :				// ��� ���
													AddMDefenceBonus(id, value);
													break;
												case en_eff_where_max_life :				// �ִ� ����
													AddMaxLifeBonus(id, value);
													break;
												case en_eff_where_max_force :				// �ִ� ����
													AddMaxForceBonus(id, value);
													break;
												case en_eff_where_max_concentration :		// �ִ� ����
													AddMaxConcentrationBonus(id, value);
													break;
												case en_eff_where_move_speed :				// �̵� �ӵ�
													AddMoveSpeedBonus(id, value);
													break;
												case en_eff_where_weapon_dam :				// ���� ���ݷ� ����
													AddWeaponDamage(id, value);
													break;
												case en_eff_where_weapon_min_dam :			// ���� �ּ� ���ݷ� ����
													AddWeaponMinDamage(id, value);
													break;
												case en_eff_where_weapon_max_dam :			// ���� �ִ� ���ݷ� ����
													AddWeaponMaxDamage(id, value);
													break;
												case en_eff_where_resist_minus :			// ���� ���׷�
													AddResistMinus(id, value);
													break;
												case en_eff_where_resist_plus :				// ���� ���׷�
													AddResistPlus(id, value);
													break;
												case en_eff_where_resist_hon :				// ȥ�� ���׷�
													AddResistHon(id, value);
													break;
												case en_eff_where_resist_sin :				// ���� ���׷�
													AddResistSin(id, value);
													break;
												case en_eff_where_resist_all :				// ��� ���׷� ����
													AddResistAll(id, value);
													break;
												case en_eff_where_add_minus_damage :		// ���� ������ �߰�
													AddAttackDamageMinus(id, value);
													break;
												case en_eff_where_add_plus_damage :			// ���� ������ �߰�
													AddAttackDamagePlus(id, value);
													break;
												case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
													AddAttackDamageHon(id, value);
													break;
												case en_eff_where_add_sin_damage :			// ���� ������ �߰�
													AddAttackDamageSin(id, value);
													break;
												}
											} // if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
										}
									} // for(ii)
								} // if(g_SkillMasteryProperty[skillid]->effect[cPoint-1])
							}
						}
					}
				}				
			}
		} // �����
	}
#else
	for(int i = 0 ; i < _XDEF_MAX_LEARNMASTERYSKILL ; ++i)
	{
		if(g_pLocalUser->m_MasterySkillList[i].skillid > 0 && g_pLocalUser->m_MasterySkillList[i].cPoint > 0)
		{
			char ms_SType = g_SkillMasteryProperty[g_pLocalUser->m_MasterySkillList[i].skillid]->cWeaponSType;
			char ms_TType = g_SkillMasteryProperty[g_pLocalUser->m_MasterySkillList[i].skillid]->cWeaponTType;
			
			short user_sID = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_sID;
			if( user_sID > 0 )
			{
				if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_cType) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_cType);					
					if( WeaponItem )
					{
						char user_SType = WeaponItem[user_sID].cSecondType;
						char user_TType = WeaponItem[user_sID].cThirdType;
						if(ms_SType == user_SType && ms_TType == user_TType)
						{
							// mastery ����� ���� ������ ������
							if(g_SkillMasteryProperty[g_pLocalUser->m_MasterySkillList[i].skillid]->detailInfo[g_pLocalUser->m_MasterySkillList[i].cPoint-1].effect)
							{
								short id = g_SkillMasteryProperty[g_pLocalUser->m_MasterySkillList[i].skillid]->detailInfo[g_pLocalUser->m_MasterySkillList[i].cPoint-1].effect->id;
								short value = g_SkillMasteryProperty[g_pLocalUser->m_MasterySkillList[i].skillid]->detailInfo[g_pLocalUser->m_MasterySkillList[i].cPoint-1].effect->value;
								
								// �ϴ� ������ ������ ���ݷ� up
								AddPAttackDamageBonus(id, value);
							}
						}
					}
				}
			}
		}
	}
#endif
}

void _XCharacterStateList::GetSkillModifiedFromMastery(void)
{
	for(int i = 0 ; i < _XDEF_MAX_LEARNMASTERYSKILL ; ++i)
	{
		if(g_pLocalUser->m_MasterySkillList[i].skillid > 0 && g_pLocalUser->m_MasterySkillList[i].cPoint > 0)
		{
			short skillid = g_pLocalUser->m_MasterySkillList[i].skillid;
			char cPoint = g_pLocalUser->m_MasterySkillList[i].cPoint;
			
			if(g_SkillMasteryProperty[skillid]->cApplyType == 1)
			{
				// ���� ��ȭ
				if(g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect)
				{
					for(int ii = 0 ; ii < g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effectCount ; ++ii)
					{
						short id = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].id;
						short value = g_SkillMasteryProperty[skillid]->detailInfo[cPoint-1].effect[ii].value;
						
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
						if(iter_table != g_CharacterStateTable.end())
						{
							if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
							{
								switch(g_CharacterStateTable[id]->where)
								{
								case en_eff_where_sk_target_modify :		// Ÿ��Ÿ�� ����
									ModifySkillTargetType(skillid, cPoint, id, value);
									break;
								case en_eff_where_sk_distance :				// ���� �Ÿ� ����
									ModifySkillDistance(skillid, cPoint, id, value);
									break;
								case en_eff_where_sk_min_dam :				// �ּ� ���ݷ� ����
									ModifySkillMinDamage(skillid, cPoint, id, value);
									break;
								case en_eff_where_sk_max_dam :				// �ִ� ���ݷ� ����
									ModifySkillMaxDamage(skillid, cPoint, id, value);
									break;
								case en_eff_where_sk_attackrate :			// ���� ����ġ ����
									ModifySkillAttackRate(skillid, cPoint, id, value);
									break;
								case en_eff_where_sk_avoidrate :			// ȸ�� ����ġ ����
									ModifySkillAvoidRate(skillid, cPoint, id, value);
									break;
								case en_eff_where_sk_costinner :			// �Ҹ� ���� ����
									ModifySkillCostInner(skillid, cPoint, id, value);
									break;
								case en_eff_where_sk_costmind :				// �Ҹ� ���� ����
									ModifySkillCostMind(skillid, cPoint, id, value);
									break;
								case en_eff_where_sk_finishspeed :			// finish speed ����
									ModifySkillFinishSpeed(skillid, cPoint, id, value);
									break;
								case en_eff_where_sk_castingtime :			// casting ����
									ModifySkillCasting(skillid, cPoint, id, value);
									break;
								case en_eff_where_sk_delay :				// delay ����
									ModifySkillDelay(skillid, cPoint, id, value);
									break;
								case en_eff_where_sk_cooldown :				// cooldown ����
									ModifySkillCooldown(skillid, cPoint, id, value);
									break;
								case en_eff_where_sk_incre_effecttime :		// ȿ�� ���� �ð� ����
									ModifySkillIncreEffectTime(skillid, cPoint, id, value);
									break;
								case en_eff_where_sk_additive_1_value :		// additive 1 ȿ�� ���� �� ����
									ModifySkillAdditiveValue(skillid, cPoint, 0, id, value);
									break;
								case en_eff_where_sk_additive_1_prob :		// additive 1 ȿ�� ���� Ȯ�� ����
									ModifySkillAdditiveProb(skillid, cPoint, 0, id, value);
									break;
								case en_eff_where_sk_additive_2_value :		// additive 2 ȿ�� ���� �� ����
									ModifySkillAdditiveValue(skillid, cPoint, 1, id, value);
									break;
								case en_eff_where_sk_additive_2_prob :		// additive 2 ȿ�� ���� Ȯ�� ����
									ModifySkillAdditiveProb(skillid, cPoint, 1, id, value);
									break;
								case en_eff_where_sk_additive_3_value :		// additive 3 ȿ�� ���� �� ����
									ModifySkillAdditiveValue(skillid, cPoint, 2, id, value);
									break;
								case en_eff_where_sk_additive_3_prob :		// additive 3 ȿ�� ���� Ȯ�� ����
									ModifySkillAdditiveProb(skillid, cPoint, 2, id, value);
									break;
								case en_eff_where_sk_additive_4_value :		// additive 4 ȿ�� ���� �� ����
									ModifySkillAdditiveValue(skillid, cPoint, 3, id, value);
									break;
								case en_eff_where_sk_additive_4_prob :		// additive 4 ȿ�� ���� Ȯ�� ����
									ModifySkillAdditiveProb(skillid, cPoint, 3, id, value);
									break;
								case en_eff_where_sk_additive_1_time :		// additive 1 ȿ�� ���� �ð� ����
									ModifySkillAdditiveTime(skillid, cPoint, 0, id, value);
									break;
								case en_eff_where_sk_additive_2_time :		// additive 2 ȿ�� ���� �ð� ����
									ModifySkillAdditiveTime(skillid, cPoint, 1, id, value);
									break;
								case en_eff_where_sk_additive_3_time :		// additive 3 ȿ�� ���� �ð� ����
									ModifySkillAdditiveTime(skillid, cPoint, 2, id, value);
									break;
								case en_eff_where_sk_additive_4_time :		// additive 4 ȿ�� ���� �ð� ����
									ModifySkillAdditiveTime(skillid, cPoint, 3, id, value);
									break;
								}
							}
						}
					} // for(ii)
				}
			} // ���� ��ȭ
		} // �����
	}
}

void _XCharacterStateList::ModifySkillTargetType(short skillid, char point, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));

	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;

	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					currentskill->bUse[en_mod_target_type] = TRUE;
					currentskill->targetType = value;
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				newskillinfo->bUse[en_mod_target_type] = TRUE;
				newskillinfo->targetType = value;
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		}
	}
	
	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
	for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
	{
		_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
		if(currentskilltable)
		{
			if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
			{
				short targetskillid = iter_skill->first;
				
				map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
				if(iter_mod != g_Skill_Mod_Property.end())
				{
					_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
					if(currentskill)
					{
						currentskill->bUse[en_mod_target_type] = TRUE;
						currentskill->targetType = value;
					}
				}
				else
				{
					_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
					memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
					newskillinfo->bUse[en_mod_target_type] = TRUE;
					newskillinfo->targetType = value;
					g_Skill_Mod_Property[targetskillid] = newskillinfo;
				}
			}

		}
	}	
}

void _XCharacterStateList::ModifySkillDistance(short skillid, char point, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						float org = g_SkillProperty[targetskill[i]]->distance;
						float bonus = 0.0f;
						if(currentskill->distance != 0)
							bonus = currentskill->distance;
						else
							bonus = org;
						
						switch(g_CharacterStateTable[id]->value_type)
						{
						case en_eff_vt_add_value :
							currentskill->distance = bonus + value;
							break;
						case en_eff_vt_add_percent :
							currentskill->distance = bonus + org*(value/100.0f);
							break;
						case en_eff_vt_sub_value :
							currentskill->distance = bonus - value;
							break;
						case en_eff_vt_sub_percent :
							currentskill->distance = bonus - (org*(value/100.0f));
							break;
						default :
							currentskill->distance = bonus;
							break;
						}
						currentskill->bUse[en_mod_distance] = TRUE;
						
						if(currentskill->distance < 0)
							currentskill->distance = 0;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));

				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					float org = g_SkillProperty[targetskill[i]]->distance;
					
					switch(g_CharacterStateTable[id]->value_type)
					{
					case en_eff_vt_add_value :
						newskillinfo->distance = org + value;
						break;
					case en_eff_vt_add_percent :
						newskillinfo->distance = org + org*(value/100.0f);
						break;
					case en_eff_vt_sub_value :
						newskillinfo->distance = org - value;
						break;
					case en_eff_vt_sub_percent :
						newskillinfo->distance = org - (org*(value/100.0f));
						break;
					default :
						newskillinfo->distance = org;
						break;
					}
					newskillinfo->bUse[en_mod_distance] = TRUE;

					if(newskillinfo->distance < 0)
						newskillinfo->distance = 0;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	}// for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType > 0)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							float org = g_SkillProperty[targetskillid]->distance;
							float bonus = 0.0f;
							if(currentskill->distance != 0)
								bonus = currentskill->distance;
							else
								bonus = org;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								currentskill->distance = bonus + value;
								break;
							case en_eff_vt_add_percent :
								currentskill->distance = bonus + org*(value/100.0f);
								break;
							case en_eff_vt_sub_value :
								currentskill->distance = bonus - value;
								break;
							case en_eff_vt_sub_percent :
								currentskill->distance = bonus - (org*(value/100.0f));
								break;
							default :
								currentskill->distance = bonus;
								break;
							}
							currentskill->bUse[en_mod_distance] = TRUE;

							if(currentskill->distance < 0)
								currentskill->distance = 0;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						float org = g_SkillProperty[targetskillid]->distance;
						
						switch(g_CharacterStateTable[id]->value_type)
						{
						case en_eff_vt_add_value :
							newskillinfo->distance = org + value;
							break;
						case en_eff_vt_add_percent :
							newskillinfo->distance = org + org*(value/100.0f);
							break;
						case en_eff_vt_sub_value :
							newskillinfo->distance = org - value;
							break;
						case en_eff_vt_sub_percent :
							newskillinfo->distance = org - (org*(value/100.0f));
							break;
						default :
							newskillinfo->distance = org;
							break;
						}
						newskillinfo->bUse[en_mod_distance] = TRUE;

						if(newskillinfo->distance < 0)
							newskillinfo->distance = 0;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillMinDamage(short skillid, char point, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].minAttackDamage;
							short bonus = 0;
							if(currentskill->detailInfo[ii].minAttackDamage != 0)
								bonus = currentskill->detailInfo[ii].minAttackDamage;
							else
								bonus = org;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								currentskill->detailInfo[ii].minAttackDamage = bonus + value;
								break;
							case en_eff_vt_add_percent :
								currentskill->detailInfo[ii].minAttackDamage = (short)(bonus + org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								currentskill->detailInfo[ii].minAttackDamage = bonus - value;
								break;
							case en_eff_vt_sub_percent :
								currentskill->detailInfo[ii].minAttackDamage = (short)(bonus - (org*(value/100.0f)));
								break;
							default :
								currentskill->detailInfo[ii].minAttackDamage = bonus;
								break;
							}

							if(currentskill->detailInfo[ii].minAttackDamage < 0)
								currentskill->detailInfo[ii].minAttackDamage = 0;

						} // for(12)
						currentskill->bUse[en_mod_min_dam] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].minAttackDamage;
						
						switch(g_CharacterStateTable[id]->value_type)
						{
						case en_eff_vt_add_value :
							newskillinfo->detailInfo[ii].minAttackDamage = org + value;
							break;
						case en_eff_vt_add_percent :
							newskillinfo->detailInfo[ii].minAttackDamage = (short)(org + org*(value/100.0f));
							break;
						case en_eff_vt_sub_value :
							newskillinfo->detailInfo[ii].minAttackDamage = org - value;
							break;
						case en_eff_vt_sub_percent :
							newskillinfo->detailInfo[ii].minAttackDamage = (short)(org - (org*(value/100.0f)));
							break;
						default :
							newskillinfo->detailInfo[ii].minAttackDamage = org;
							break;
						}

						if(newskillinfo->detailInfo[ii].minAttackDamage < 0)
							newskillinfo->detailInfo[ii].minAttackDamage = 0;
					} // for(12)
					newskillinfo->bUse[en_mod_min_dam] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} // for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType > 0)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{	
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								short org = currentskilltable->detailInfo[ii].minAttackDamage;
								short bonus = 0;
								if(currentskill->detailInfo[ii].minAttackDamage != 0)
									bonus = currentskill->detailInfo[ii].minAttackDamage;
								else
									bonus = org;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].minAttackDamage = bonus + value;
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].minAttackDamage = (short)(bonus + org*(value/100.0f));
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].minAttackDamage = bonus - value;
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].minAttackDamage = (short)(bonus - (org*(value/100.0f)));
									break;
								default :
									currentskill->detailInfo[ii].minAttackDamage = bonus;
									break;
								}

								if(currentskill->detailInfo[ii].minAttackDamage < 0)
									currentskill->detailInfo[ii].minAttackDamage = 0;
							} // for(12)
							currentskill->bUse[en_mod_min_dam] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							short org = currentskilltable->detailInfo[ii].minAttackDamage;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].minAttackDamage = org + value;
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].minAttackDamage = (short)(org + org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].minAttackDamage = org - value;
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].minAttackDamage = (short)(org - (org*(value/100.0f)));
								break;
							default :
								newskillinfo->detailInfo[ii].minAttackDamage = org;
								break;
							}

							if(newskillinfo->detailInfo[ii].minAttackDamage < 0)
								newskillinfo->detailInfo[ii].minAttackDamage = 0;
						} // for(12)
						newskillinfo->bUse[en_mod_min_dam] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillMaxDamage(short skillid, char point, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].maxAttackDamage;
							short bonus = 0;
							if(currentskill->detailInfo[ii].maxAttackDamage != 0)
								bonus = currentskill->detailInfo[ii].maxAttackDamage;
							else
								bonus = org;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								currentskill->detailInfo[ii].maxAttackDamage = bonus + value;
								break;
							case en_eff_vt_add_percent :
								currentskill->detailInfo[ii].maxAttackDamage = (short)(bonus + org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								currentskill->detailInfo[ii].maxAttackDamage = bonus - value;
								break;
							case en_eff_vt_sub_percent :
								currentskill->detailInfo[ii].maxAttackDamage = (short)(bonus - (org*(value/100.0f)));
								break;
							default :
								currentskill->detailInfo[ii].maxAttackDamage = bonus;
								break;
							}

							if(currentskill->detailInfo[ii].maxAttackDamage < 0)
								currentskill->detailInfo[ii].maxAttackDamage = 0;
						} // for(12)
						currentskill->bUse[en_mod_max_dam] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].maxAttackDamage;
						
						switch(g_CharacterStateTable[id]->value_type)
						{
						case en_eff_vt_add_value :
							newskillinfo->detailInfo[ii].maxAttackDamage = org + value;
							break;
						case en_eff_vt_add_percent :
							newskillinfo->detailInfo[ii].maxAttackDamage = (short)(org + org*(value/100.0f));
							break;
						case en_eff_vt_sub_value :
							newskillinfo->detailInfo[ii].maxAttackDamage = org - value;
							break;
						case en_eff_vt_sub_percent :
							newskillinfo->detailInfo[ii].maxAttackDamage = (short)(org - (org*(value/100.0f)));
							break;
						default :
							newskillinfo->detailInfo[ii].maxAttackDamage = org;
							break;
						}

						if(newskillinfo->detailInfo[ii].maxAttackDamage < 0)
							newskillinfo->detailInfo[ii].maxAttackDamage = 0;

					} // for(12)
					newskillinfo->bUse[en_mod_max_dam] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} // for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType > 0)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								short org = currentskilltable->detailInfo[ii].maxAttackDamage;
								short bonus = 0;
								if(currentskill->detailInfo[ii].maxAttackDamage != 0)
									bonus = currentskill->detailInfo[ii].maxAttackDamage;
								else
									bonus = org;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].maxAttackDamage = bonus + value;
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].maxAttackDamage = (short)(bonus + org*(value/100.0f));
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].maxAttackDamage = bonus - value;
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].maxAttackDamage = (short)(bonus - (org*(value/100.0f)));
									break;
								default :
									currentskill->detailInfo[ii].maxAttackDamage = bonus;
									break;
								}

								if(currentskill->detailInfo[ii].maxAttackDamage < 0)
									currentskill->detailInfo[ii].maxAttackDamage = 0;
							} // for(12)
							currentskill->bUse[en_mod_max_dam] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							short org = currentskilltable->detailInfo[ii].maxAttackDamage;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].maxAttackDamage = org + value;
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].maxAttackDamage = (short)(org + org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].maxAttackDamage = org - value;
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].maxAttackDamage = (short)(org - (org*(value/100.0f)));
								break;
							default :
								newskillinfo->detailInfo[ii].maxAttackDamage = org;
								break;
							}

							if(newskillinfo->detailInfo[ii].maxAttackDamage < 0)
								newskillinfo->detailInfo[ii].maxAttackDamage = 0;
						} // for(12)
						newskillinfo->bUse[en_mod_max_dam] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillAttackRate(short skillid, char point, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].attackRating;
							short bonus = 0;
							if(currentskill->detailInfo[ii].attackRating != 0)
								bonus = currentskill->detailInfo[ii].attackRating;
							else
								bonus = org;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								currentskill->detailInfo[ii].attackRating = bonus + value;
								break;
							case en_eff_vt_add_percent :
								currentskill->detailInfo[ii].attackRating = (short)(bonus + org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								currentskill->detailInfo[ii].attackRating = bonus - value;
								break;
							case en_eff_vt_sub_percent :
								currentskill->detailInfo[ii].attackRating = (short)(bonus - (org*(value/100.0f)));
								break;
							default :
								currentskill->detailInfo[ii].attackRating = bonus;
								break;
							}

							if(currentskill->detailInfo[ii].attackRating < 0)
								currentskill->detailInfo[ii].attackRating = 0;
						} // for(12)
						currentskill->bUse[en_mod_attackrate] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].attackRating;
						
						switch(g_CharacterStateTable[id]->value_type)
						{
						case en_eff_vt_add_value :
							newskillinfo->detailInfo[ii].attackRating = org + value;
							break;
						case en_eff_vt_add_percent :
							newskillinfo->detailInfo[ii].attackRating = (short)(org + org*(value/100.0f));
							break;
						case en_eff_vt_sub_value :
							newskillinfo->detailInfo[ii].attackRating = org - value;
							break;
						case en_eff_vt_sub_percent :
							newskillinfo->detailInfo[ii].attackRating = (short)(org - (org*(value/100.0f)));
							break;
						default :
							newskillinfo->detailInfo[ii].attackRating = org;
							break;
						}

						if(newskillinfo->detailInfo[ii].attackRating < 0)
							newskillinfo->detailInfo[ii].attackRating = 0;
					} // for(12)
					newskillinfo->bUse[en_mod_attackrate] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} // for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType > 0)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								short org = currentskilltable->detailInfo[ii].attackRating;
								short bonus = 0;
								if(currentskill->detailInfo[ii].attackRating != 0)
									bonus = currentskill->detailInfo[ii].attackRating;
								else
									bonus = org;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].attackRating = bonus + value;
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].attackRating = (short)(bonus + org*(value/100.0f));
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].attackRating = bonus - value;
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].attackRating = (short)(bonus - (org*(value/100.0f)));
									break;
								default :
									currentskill->detailInfo[ii].attackRating = bonus;
									break;
								}

								if(currentskill->detailInfo[ii].attackRating < 0)
									currentskill->detailInfo[ii].attackRating = 0;
							} // for(12)
							currentskill->bUse[en_mod_attackrate] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							short org = currentskilltable->detailInfo[ii].attackRating;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].attackRating = org + value;
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].attackRating = (short)(org + org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].attackRating = org - value;
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].attackRating = (short)(org - (org*(value/100.0f)));
								break;
							default :
								newskillinfo->detailInfo[ii].attackRating = org;
								break;
							}

							if(newskillinfo->detailInfo[ii].attackRating < 0)
								newskillinfo->detailInfo[ii].attackRating = 0;
						} // for(12)
						newskillinfo->bUse[en_mod_attackrate] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillAvoidRate(short skillid, char point, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].avoidRating;
							short bonus = 0;
							if(currentskill->detailInfo[ii].avoidRating != 0)
								bonus = currentskill->detailInfo[ii].avoidRating;
							else
								bonus = org;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								currentskill->detailInfo[ii].avoidRating = bonus + value;
								break;
							case en_eff_vt_add_percent :
								currentskill->detailInfo[ii].avoidRating = (short)(bonus + org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								currentskill->detailInfo[ii].avoidRating = bonus - value;
								break;
							case en_eff_vt_sub_percent :
								currentskill->detailInfo[ii].avoidRating = (short)(bonus - (org*(value/100.0f)));
								break;
							default :
								currentskill->detailInfo[ii].avoidRating = bonus;
								break;
							}

							if(currentskill->detailInfo[ii].avoidRating < 0)
								currentskill->detailInfo[ii].avoidRating = 0;
						} // for(12)
						currentskill->bUse[en_mod_avoidrate] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].avoidRating;
						
						switch(g_CharacterStateTable[id]->value_type)
						{
						case en_eff_vt_add_value :
							newskillinfo->detailInfo[ii].avoidRating = org + value;
							break;
						case en_eff_vt_add_percent :
							newskillinfo->detailInfo[ii].avoidRating = (short)(org + org*(value/100.0f));
							break;
						case en_eff_vt_sub_value :
							newskillinfo->detailInfo[ii].avoidRating = org - value;
							break;
						case en_eff_vt_sub_percent :
							newskillinfo->detailInfo[ii].avoidRating = (short)(org - (org*(value/100.0f)));
							break;
						default :
							newskillinfo->detailInfo[ii].avoidRating = org;
							break;
						}

						if(newskillinfo->detailInfo[ii].avoidRating < 0)
							newskillinfo->detailInfo[ii].avoidRating = 0;
					} // for(12)
					newskillinfo->bUse[en_mod_avoidrate] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} // for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType > 0)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								short org = currentskilltable->detailInfo[ii].avoidRating;
								short bonus = 0;
								if(currentskill->detailInfo[ii].avoidRating != 0)
									bonus = currentskill->detailInfo[ii].avoidRating;
								else
									bonus = org;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].avoidRating = bonus + value;
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].avoidRating = (short)(bonus + org*(value/100.0f));
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].avoidRating = bonus - value;
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].avoidRating = (short)(bonus - (org*(value/100.0f)));
									break;
								default :
									currentskill->detailInfo[ii].avoidRating = bonus;
									break;
								}

								if(currentskill->detailInfo[ii].avoidRating < 0)
									currentskill->detailInfo[ii].avoidRating = 0;
							} // for(12)
							currentskill->bUse[en_mod_avoidrate] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							short org = currentskilltable->detailInfo[ii].avoidRating;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].avoidRating = org + value;
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].avoidRating = (short)(org + org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].avoidRating = org - value;
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].avoidRating = (short)(org - (org*(value/100.0f)));
								break;
							default :
								newskillinfo->detailInfo[ii].avoidRating = org;
								break;
							}

							if(newskillinfo->detailInfo[ii].avoidRating < 0)
								newskillinfo->detailInfo[ii].avoidRating = 0;
						} // for(12)
						newskillinfo->bUse[en_mod_avoidrate] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillCostInner(short skillid, char point, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].costForce;
							short bonus = 0;
							if(currentskill->detailInfo[ii].costForce != 0)
								bonus = currentskill->detailInfo[ii].costForce;
							else
								bonus = org;
							
							// �Ҹ� ���� ���� - add/sub�� ���� ���� ������ ����
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								currentskill->detailInfo[ii].costForce = bonus - value;
								break;
							case en_eff_vt_add_percent :
								currentskill->detailInfo[ii].costForce = (short)(bonus - org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								currentskill->detailInfo[ii].costForce = bonus - value;
								break;
							case en_eff_vt_sub_percent :
								currentskill->detailInfo[ii].costForce = (short)(bonus - (org*(value/100.0f)));
								break;
							default :
								currentskill->detailInfo[ii].costForce = bonus;
								break;
							}

							if(currentskill->detailInfo[ii].costForce < 0)
								currentskill->detailInfo[ii].costForce= 0;
						} // for(12)
						currentskill->bUse[en_mod_costinner] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].costForce;
						
						// �Ҹ� ���� ���� - add/sub�� ���� ���� ������ ����
						switch(g_CharacterStateTable[id]->value_type)
						{
						case en_eff_vt_add_value :
							newskillinfo->detailInfo[ii].costForce = org - value;
							break;
						case en_eff_vt_add_percent :
							newskillinfo->detailInfo[ii].costForce = (short)(org - org*(value/100.0f));
							break;
						case en_eff_vt_sub_value :
							newskillinfo->detailInfo[ii].costForce = org - value;
							break;
						case en_eff_vt_sub_percent :
							newskillinfo->detailInfo[ii].costForce = (short)(org - (org*(value/100.0f)));
							break;
						default :
							newskillinfo->detailInfo[ii].costForce = org;
							break;
						}

						if(newskillinfo->detailInfo[ii].costForce < 0)
							newskillinfo->detailInfo[ii].costForce= 0;
					} // for(12)
					newskillinfo->bUse[en_mod_costinner] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} //for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType > 0)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								short org = currentskilltable->detailInfo[ii].costForce;
								short bonus = 0;
								if(currentskill->detailInfo[ii].costForce != 0)
									bonus = currentskill->detailInfo[ii].costForce;
								else
									bonus = org;
								
								// �Ҹ� ���� ���� - add/sub�� ���� ���� ������ ����
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].costForce = bonus - value;
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].costForce = (short)(bonus - org*(value/100.0f));
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].costForce = bonus - value;
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].costForce = (short)(bonus - (org*(value/100.0f)));
									break;
								default :
									currentskill->detailInfo[ii].costForce = bonus;
									break;
								}

								if(currentskill->detailInfo[ii].costForce < 0)
									currentskill->detailInfo[ii].costForce= 0;
							} // for(12)
							currentskill->bUse[en_mod_costinner] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							short org = currentskilltable->detailInfo[ii].costForce;
							
							// �Ҹ� ���� ���� - add/sub�� ���� ���� ������ ����
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].costForce = org - value;
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].costForce = (short)(org - org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].costForce = org - value;
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].costForce = (short)(org - (org*(value/100.0f)));
								break;
							default :
								newskillinfo->detailInfo[ii].costForce = org;
								break;
							}

							if(newskillinfo->detailInfo[ii].costForce < 0)
								newskillinfo->detailInfo[ii].costForce= 0;
						} // for(12)
						newskillinfo->bUse[en_mod_costinner] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillCostMind(short skillid, char point, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].costMind;
							short bonus = 0;
							if(currentskill->detailInfo[ii].costMind != 0)
								bonus = currentskill->detailInfo[ii].costMind;
							else
								bonus = org;
							
							// �Ҹ� ���� ���� - add/sub�� ���� ���� ������ ����
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								currentskill->detailInfo[ii].costMind = bonus - value;
								break;
							case en_eff_vt_add_percent :
								currentskill->detailInfo[ii].costMind = (short)(bonus - org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								currentskill->detailInfo[ii].costMind = bonus - value;
								break;
							case en_eff_vt_sub_percent :
								currentskill->detailInfo[ii].costMind = (short)(bonus - (org*(value/100.0f)));
								break;
							default :
								currentskill->detailInfo[ii].costMind = bonus;
								break;
							}

							if(currentskill->detailInfo[ii].costMind < 0)
								currentskill->detailInfo[ii].costMind= 0;
						} // for(12)
						currentskill->bUse[en_mod_costmind] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].costMind;
						
						// �Ҹ� ���� ���� - add/sub�� ���� ���� ������ ����
						switch(g_CharacterStateTable[id]->value_type)
						{
						case en_eff_vt_add_value :
							newskillinfo->detailInfo[ii].costMind = org - value;
							break;
						case en_eff_vt_add_percent :
							newskillinfo->detailInfo[ii].costMind = (short)(org - org*(value/100.0f));
							break;
						case en_eff_vt_sub_value :
							newskillinfo->detailInfo[ii].costMind = org - value;
							break;
						case en_eff_vt_sub_percent :
							newskillinfo->detailInfo[ii].costMind = (short)(org - (org*(value/100.0f)));
							break;
						default :
							newskillinfo->detailInfo[ii].costMind = org;
							break;
						}

						if(newskillinfo->detailInfo[ii].costMind < 0)
							newskillinfo->detailInfo[ii].costMind= 0;
					} // for(12)
					newskillinfo->bUse[en_mod_costmind] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} //for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								short org = currentskilltable->detailInfo[ii].costMind;
								short bonus = 0;
								if(currentskill->detailInfo[ii].costMind != 0)
									bonus = currentskill->detailInfo[ii].costMind;
								else
									bonus = org;
								
								// �Ҹ� ���� ���� - add/sub�� ���� ���� ������ ����
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].costMind = bonus - value;
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].costMind = (short)(bonus - org*(value/100.0f));
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].costMind = bonus - value;
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].costMind = (short)(bonus - (org*(value/100.0f)));
									break;
								default :
									currentskill->detailInfo[ii].costMind = bonus;
									break;
								}

								if(currentskill->detailInfo[ii].costMind < 0)
									currentskill->detailInfo[ii].costMind= 0;
							} // for(12)
							currentskill->bUse[en_mod_costmind] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							short org = currentskilltable->detailInfo[ii].costMind;
							
							// �Ҹ� ���� ���� - add/sub�� ���� ���� ������ ����
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].costMind = org - value;
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].costMind = (short)(org - org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].costMind = org - value;
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].costMind = (short)(org - (org*(value/100.0f)));
								break;
							default :
								newskillinfo->detailInfo[ii].costMind = org;
								break;
							}

							if(newskillinfo->detailInfo[ii].costMind < 0)
								newskillinfo->detailInfo[ii].costMind= 0;
						} // for(12)
						newskillinfo->bUse[en_mod_costmind] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillFinishSpeed(short skillid, char point, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							float org = g_SkillProperty[targetskill[i]]->detailInfo[ii].finishSpeed;
							float bonus = 0.0f;
							if(currentskill->detailInfo[ii].finishSpeed != 0)
								bonus = currentskill->detailInfo[ii].finishSpeed;
							else
								bonus = org;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								currentskill->detailInfo[ii].finishSpeed = bonus + value;
								break;
							case en_eff_vt_add_percent :
								currentskill->detailInfo[ii].finishSpeed = bonus + org*(value/100.0f);
								break;
							case en_eff_vt_sub_value :
								currentskill->detailInfo[ii].finishSpeed = bonus - value;
								break;
							case en_eff_vt_sub_percent :
								currentskill->detailInfo[ii].finishSpeed = bonus - (org*(value/100.0f));
								break;
							default :
								currentskill->detailInfo[ii].finishSpeed = bonus;
								break;
							}

							if(currentskill->detailInfo[ii].finishSpeed < 0)
								currentskill->detailInfo[ii].finishSpeed= 0;
						} // for(12)
						currentskill->bUse[en_mod_finishspeed] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						float org = g_SkillProperty[targetskill[i]]->detailInfo[ii].finishSpeed;
						
						switch(g_CharacterStateTable[id]->value_type)
						{
						case en_eff_vt_add_value :
							newskillinfo->detailInfo[ii].finishSpeed = org + value;
							break;
						case en_eff_vt_add_percent :
							newskillinfo->detailInfo[ii].finishSpeed = org + org*(value/100.0f);
							break;
						case en_eff_vt_sub_value :
							newskillinfo->detailInfo[ii].finishSpeed = org - value;
							break;
						case en_eff_vt_sub_percent :
							newskillinfo->detailInfo[ii].finishSpeed = org - (org*(value/100.0f));
							break;
						default :
							newskillinfo->detailInfo[ii].finishSpeed = org;
							break;
						}

						if(newskillinfo->detailInfo[ii].finishSpeed < 0)
							newskillinfo->detailInfo[ii].finishSpeed= 0;
					} // for(12)
					newskillinfo->bUse[en_mod_finishspeed] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} // for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType > 0)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								float org = currentskilltable->detailInfo[ii].finishSpeed;
								float bonus = 0.0f;
								if(currentskill->detailInfo[ii].finishSpeed != 0)
									bonus = currentskill->detailInfo[ii].finishSpeed;
								else
									bonus = org;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].finishSpeed = bonus + value;
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].finishSpeed = bonus + org*(value/100.0f);
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].finishSpeed = bonus - value;
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].finishSpeed = bonus - (org*(value/100.0f));
									break;
								default :
									currentskill->detailInfo[ii].finishSpeed = bonus;
									break;
								}

								if(currentskill->detailInfo[ii].finishSpeed < 0)
									currentskill->detailInfo[ii].finishSpeed= 0;
							} // for(12)
							currentskill->bUse[en_mod_finishspeed] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							float org = currentskilltable->detailInfo[ii].finishSpeed;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].finishSpeed = org + value;
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].finishSpeed = org + org*(value/100.0f);
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].finishSpeed = org - value;
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].finishSpeed = org - (org*(value/100.0f));
								break;
							default :
								newskillinfo->detailInfo[ii].finishSpeed = org;
								break;
							}

							if(newskillinfo->detailInfo[ii].finishSpeed < 0)
								newskillinfo->detailInfo[ii].finishSpeed= 0;
						} // for(12)
						newskillinfo->bUse[en_mod_finishspeed] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillCasting(short skillid, char point, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							int org = g_SkillProperty[targetskill[i]]->detailInfo[ii].casting;
							int bonus = 0;
							if(currentskill->detailInfo[ii].casting != 0)
								bonus = currentskill->detailInfo[ii].casting;
							else
								bonus = org;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								currentskill->detailInfo[ii].casting = bonus + value*1000;
								break;
							case en_eff_vt_add_percent :
								currentskill->detailInfo[ii].casting = bonus + org*(value/100.0f);
								break;
							case en_eff_vt_sub_value :
								currentskill->detailInfo[ii].casting = bonus - value*1000;
								break;
							case en_eff_vt_sub_percent :
								currentskill->detailInfo[ii].casting = bonus - (org*(value/100.0f));
								break;
							default :
								currentskill->detailInfo[ii].casting = bonus;
								break;
							}

							if(currentskill->detailInfo[ii].casting < 0)
								currentskill->detailInfo[ii].casting= 0;
						} // for(12)
						currentskill->bUse[en_mod_castingtime] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						int org = g_SkillProperty[targetskill[i]]->detailInfo[ii].casting;
						
						switch(g_CharacterStateTable[id]->value_type)
						{
						case en_eff_vt_add_value :
							newskillinfo->detailInfo[ii].casting = org + value*1000;
							break;
						case en_eff_vt_add_percent :
							newskillinfo->detailInfo[ii].casting = org + org*(value/100.0f);
							break;
						case en_eff_vt_sub_value :
							newskillinfo->detailInfo[ii].casting = org - value*1000;
							break;
						case en_eff_vt_sub_percent :
							newskillinfo->detailInfo[ii].casting = org - (org*(value/100.0f));
							break;
						default :
							newskillinfo->detailInfo[ii].casting = org;
							break;
						}

						if(newskillinfo->detailInfo[ii].casting < 0)
							newskillinfo->detailInfo[ii].casting= 0;
					} // for(12)
					newskillinfo->bUse[en_mod_castingtime] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} // for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType > 0)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								int org = currentskilltable->detailInfo[ii].casting;
								int bonus = 0;
								if(currentskill->detailInfo[ii].casting != 0)
									bonus = currentskill->detailInfo[ii].casting;
								else
									bonus = org;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].casting = bonus + value*1000;
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].casting = bonus + org*(value/100.0f);
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].casting = bonus - value*1000;
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].casting = bonus - (org*(value/100.0f));
									break;
								default :
									currentskill->detailInfo[ii].casting = bonus;
									break;
								}

								if(currentskill->detailInfo[ii].casting < 0)
									currentskill->detailInfo[ii].casting= 0;
							} // for(12)
							currentskill->bUse[en_mod_castingtime] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							int org = currentskilltable->detailInfo[ii].casting;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].casting = org + value*1000;
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].casting = org + org*(value/100.0f);
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].casting = org - value*1000;
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].casting = org - (org*(value/100.0f));
								break;
							default :
								newskillinfo->detailInfo[ii].casting = org;
								break;
							}

							if(newskillinfo->detailInfo[ii].casting < 0)
								newskillinfo->detailInfo[ii].casting= 0;
						} // for(12)
						newskillinfo->bUse[en_mod_castingtime] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillDelay(short skillid, char point, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							int org = g_SkillProperty[targetskill[i]]->detailInfo[ii].delay;
							int bonus = 0;
							if(currentskill->detailInfo[ii].delay != 0)
								bonus = currentskill->detailInfo[ii].delay;
							else
								bonus = org;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								currentskill->detailInfo[ii].delay = bonus + value*1000;
								break;
							case en_eff_vt_add_percent :
								currentskill->detailInfo[ii].delay = bonus + org*(value/100.0f);
								break;
							case en_eff_vt_sub_value :
								currentskill->detailInfo[ii].delay = bonus - value*1000;
								break;
							case en_eff_vt_sub_percent :
								currentskill->detailInfo[ii].delay = bonus - (org*(value/100.0f));
								break;
							default :
								currentskill->detailInfo[ii].delay = bonus;
								break;
							}

							if(currentskill->detailInfo[ii].delay < 0)
								currentskill->detailInfo[ii].delay= 0;
						} // for(12)
						currentskill->bUse[en_mod_delay] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						int org = g_SkillProperty[targetskill[i]]->detailInfo[ii].delay;
						
						switch(g_CharacterStateTable[id]->value_type)
						{
						case en_eff_vt_add_value :
							newskillinfo->detailInfo[ii].delay = org + value*1000;
							break;
						case en_eff_vt_add_percent :
							newskillinfo->detailInfo[ii].delay = org + org*(value/100.0f);
							break;
						case en_eff_vt_sub_value :
							newskillinfo->detailInfo[ii].delay = org - value*1000;
							break;
						case en_eff_vt_sub_percent :
							newskillinfo->detailInfo[ii].delay = org - (org*(value/100.0f));
							break;
						default :
							newskillinfo->detailInfo[ii].delay = org;
							break;
						}

						if(newskillinfo->detailInfo[ii].delay < 0)
							newskillinfo->detailInfo[ii].delay= 0;
					} // for(12)
					newskillinfo->bUse[en_mod_delay] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} // for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								int org = currentskilltable->detailInfo[ii].delay;
								int bonus = 0;
								if(currentskill->detailInfo[ii].delay != 0)
									bonus = currentskill->detailInfo[ii].delay;
								else
									bonus = org;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].delay = bonus + value*1000;
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].delay = bonus + org*(value/100.0f);
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].delay = bonus - value*1000;
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].delay = bonus - (org*(value/100.0f));
									break;
								default :
									currentskill->detailInfo[ii].delay = bonus;
									break;
								}

								if(currentskill->detailInfo[ii].delay < 0)
									currentskill->detailInfo[ii].delay= 0;
							} // for(12)
							currentskill->bUse[en_mod_delay] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							int org = currentskilltable->detailInfo[ii].delay;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].delay = org + value*1000;
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].delay = org + org*(value/100.0f);
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].delay = org - value*1000;
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].delay = org - (org*(value/100.0f));
								break;
							default :
								newskillinfo->detailInfo[ii].delay = org;
								break;
							}

							if(newskillinfo->detailInfo[ii].delay < 0)
								newskillinfo->detailInfo[ii].delay= 0;
						} // for(12)
						newskillinfo->bUse[en_mod_delay] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillCooldown(short skillid, char point, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							int org = g_SkillProperty[targetskill[i]]->detailInfo[ii].coolDown;
							int bonus = 0;
							if(currentskill->detailInfo[ii].coolDown != 0)
								bonus = currentskill->detailInfo[ii].coolDown;
							else
								bonus = org;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								currentskill->detailInfo[ii].coolDown = bonus + value*1000;
								break;
							case en_eff_vt_add_percent :
								currentskill->detailInfo[ii].coolDown = bonus + org*(value/100.0f);
								break;
							case en_eff_vt_sub_value :
								currentskill->detailInfo[ii].coolDown = bonus - value*1000;
								break;
							case en_eff_vt_sub_percent :
								currentskill->detailInfo[ii].coolDown = bonus - (org*(value/100.0f));
								break;
							default :
								currentskill->detailInfo[ii].coolDown = bonus;
								break;
							}

							if(currentskill->detailInfo[ii].coolDown < 0)
								currentskill->detailInfo[ii].coolDown = 0;
						} // for(12)
						currentskill->bUse[en_mod_cooldown] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						int org = g_SkillProperty[targetskill[i]]->detailInfo[ii].coolDown;
						
						switch(g_CharacterStateTable[id]->value_type)
						{
						case en_eff_vt_add_value :
							newskillinfo->detailInfo[ii].coolDown = org + value*1000;
							break;
						case en_eff_vt_add_percent :
							newskillinfo->detailInfo[ii].coolDown = org + org*(value/100.0f);
							break;
						case en_eff_vt_sub_value :
							newskillinfo->detailInfo[ii].coolDown = org - value*1000;
							break;
						case en_eff_vt_sub_percent :
							newskillinfo->detailInfo[ii].coolDown = org - (org*(value/100.0f));
							break;
						default :
							newskillinfo->detailInfo[ii].coolDown = org;
							break;
						}

						if(newskillinfo->detailInfo[ii].coolDown < 0)
							newskillinfo->detailInfo[ii].coolDown = 0;
					} // for(12)
					newskillinfo->bUse[en_mod_cooldown] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} //for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								int org = currentskilltable->detailInfo[ii].coolDown;
								int bonus = 0;
								if(currentskill->detailInfo[ii].coolDown != 0)
									bonus = currentskill->detailInfo[ii].coolDown;
								else
									bonus = org;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].coolDown = bonus + value*1000;
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].coolDown = bonus + org*(value/100.0f);
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].coolDown = bonus - value*1000;
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].coolDown = bonus - (org*(value/100.0f));
									break;
								default :
									currentskill->detailInfo[ii].coolDown = bonus;
									break;
								}

								if(currentskill->detailInfo[ii].coolDown < 0)
									currentskill->detailInfo[ii].coolDown = 0;
								
//								_XLog("skillid [%d/%d] cooldown[org:%d  mod:%d]", targetskillid, ii,org, currentskill->detailInfo[ii].coolDown);
							} // for(12)
							currentskill->bUse[en_mod_cooldown] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							int org = currentskilltable->detailInfo[ii].coolDown;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].coolDown = org + value*1000;
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].coolDown = org + org*(value/100.0f);
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].coolDown = org - value*1000;
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].coolDown = org - (org*(value/100.0f));
								break;
							default :
								newskillinfo->detailInfo[ii].coolDown = org;
								break;
							}

							if(newskillinfo->detailInfo[ii].coolDown < 0)
								newskillinfo->detailInfo[ii].coolDown = 0;
						} // for(12)
						newskillinfo->bUse[en_mod_cooldown] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillIncreEffectTime(short skillid, char point, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							unsigned short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].effectTime;
							unsigned short bonus = 0;
							if(currentskill->detailInfo[ii].effectTime != 0)
								bonus = currentskill->detailInfo[ii].effectTime;
							else
								bonus = org;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								currentskill->detailInfo[ii].effectTime = (unsigned short)(bonus + value);
								break;
							case en_eff_vt_add_percent :
								currentskill->detailInfo[ii].effectTime = (unsigned short)(bonus + org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								currentskill->detailInfo[ii].effectTime = (unsigned short)(bonus - value);
								break;
							case en_eff_vt_sub_percent :
								currentskill->detailInfo[ii].effectTime = (unsigned short)(bonus - (org*(value/100.0f)));
								break;
							default :
								currentskill->detailInfo[ii].effectTime = bonus;
								break;
							}

							if(currentskill->detailInfo[ii].effectTime < 0)
								currentskill->detailInfo[ii].effectTime = 0;
						} // for(12)
						currentskill->bUse[en_mod_incre_effecttime] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						unsigned short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].effectTime;
						
						switch(g_CharacterStateTable[id]->value_type)
						{
						case en_eff_vt_add_value :
							newskillinfo->detailInfo[ii].effectTime = (unsigned short)(org + value);
							break;
						case en_eff_vt_add_percent :
							newskillinfo->detailInfo[ii].effectTime = (unsigned short)(org + org*(value/100.0f));
							break;
						case en_eff_vt_sub_value :
							newskillinfo->detailInfo[ii].effectTime = (unsigned short)(org - value);
							break;
						case en_eff_vt_sub_percent :
							newskillinfo->detailInfo[ii].effectTime = (unsigned short)(org - (org*(value/100.0f)));
							break;
						default :
							newskillinfo->detailInfo[ii].effectTime = org;
							break;
						}

						if(newskillinfo->detailInfo[ii].effectTime < 0)
							newskillinfo->detailInfo[ii].effectTime = 0;
					} // for(12)
					newskillinfo->bUse[en_mod_incre_effecttime] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} //for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								unsigned short org = currentskilltable->detailInfo[ii].effectTime;
								unsigned short bonus = 0;
								if(currentskill->detailInfo[ii].effectTime != 0)
									bonus = currentskill->detailInfo[ii].effectTime;
								else
									bonus = org;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].effectTime = (unsigned short)(bonus + value);
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].effectTime = (unsigned short)(bonus + org*(value/100.0f));
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].effectTime = (unsigned short)(bonus - value);
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].effectTime = (unsigned short)(bonus - (org*(value/100.0f)));
									break;
								default :
									currentskill->detailInfo[ii].effectTime = bonus;
									break;
								}

								if(currentskill->detailInfo[ii].effectTime < 0)
									currentskill->detailInfo[ii].effectTime = 0;
							} // for(12)
							currentskill->bUse[en_mod_incre_effecttime] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							unsigned short org = currentskilltable->detailInfo[ii].effectTime;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].effectTime = (unsigned short)(org + value);
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].effectTime = (unsigned short)(org + org*(value/100.0f));
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].effectTime = (unsigned short)(org - value);
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].effectTime = (unsigned short)(org - (org*(value/100.0f)));
								break;
							default :
								newskillinfo->detailInfo[ii].effectTime = org;
								break;
							}

							if(newskillinfo->detailInfo[ii].effectTime < 0)
								newskillinfo->detailInfo[ii].effectTime = 0;
						} // for(12)
						newskillinfo->bUse[en_mod_incre_effecttime] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillAdditiveValue(short skillid, char point, int index, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							if(g_SkillProperty[targetskill[i]]->detailInfo[ii].additiveCount > index)
							{
								short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].additiveEffect[index].value;
								short bonus = 0;
								if(currentskill->detailInfo[ii].additiveEffect[index].value != 0)
									bonus = currentskill->detailInfo[ii].additiveEffect[index].value;
								else
									bonus = org;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].additiveEffect[index].value = bonus + value;
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].additiveEffect[index].value = bonus + org*(value/100.0f);
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].additiveEffect[index].value = bonus - value;
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].additiveEffect[index].value = bonus - (org*(value/100.0f));
									break;
								default :
									currentskill->detailInfo[ii].additiveEffect[index].value = bonus;
									break;
								}
							}

							if(currentskill->detailInfo[ii].additiveEffect[index].value < 0)
								currentskill->detailInfo[ii].additiveEffect[index].value = 0;
						} // for(12)
						currentskill->bUse[en_mod_additive_1_value+index] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						if(g_SkillProperty[targetskill[i]]->detailInfo[ii].additiveCount > index)
						{
							short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].additiveEffect[index].value;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].additiveEffect[index].value = org + value;
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].additiveEffect[index].value = org + org*(value/100.0f);
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].additiveEffect[index].value = org - value;
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].additiveEffect[index].value = org - (org*(value/100.0f));
								break;
							default :
								newskillinfo->detailInfo[ii].additiveEffect[index].value = org;
								break;
							}
						}

						if(newskillinfo->detailInfo[ii].additiveEffect[index].value < 0)
							newskillinfo->detailInfo[ii].additiveEffect[index].value = 0;
					} // for(12)
					newskillinfo->bUse[en_mod_additive_1_value+index] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} //for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								if(currentskilltable->detailInfo[ii].additiveCount > index)
								{
									short org = currentskilltable->detailInfo[ii].additiveEffect[index].value;
									short bonus = 0;
									if(currentskill->detailInfo[ii].additiveEffect[index].value != 0)
										bonus = currentskill->detailInfo[ii].additiveEffect[index].value;
									else
										bonus = org;
									
									switch(g_CharacterStateTable[id]->value_type)
									{
									case en_eff_vt_add_value :
										currentskill->detailInfo[ii].additiveEffect[index].value = bonus + value;
										break;
									case en_eff_vt_add_percent :
										currentskill->detailInfo[ii].additiveEffect[index].value = bonus + org*(value/100.0f);
										break;
									case en_eff_vt_sub_value :
										currentskill->detailInfo[ii].additiveEffect[index].value = bonus - value;
										break;
									case en_eff_vt_sub_percent :
										currentskill->detailInfo[ii].additiveEffect[index].value = bonus - (org*(value/100.0f));
										break;
									default :
										currentskill->detailInfo[ii].additiveEffect[index].value = bonus;
										break;
									}
								}

								if(currentskill->detailInfo[ii].additiveEffect[index].value < 0)
									currentskill->detailInfo[ii].additiveEffect[index].value = 0;
							} // for(12)
							currentskill->bUse[en_mod_additive_1_value+index] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							if(currentskilltable->detailInfo[ii].additiveCount > index)
							{
								short org = currentskilltable->detailInfo[ii].additiveEffect[index].value;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									newskillinfo->detailInfo[ii].additiveEffect[index].value = org + value;
									break;
								case en_eff_vt_add_percent :
									newskillinfo->detailInfo[ii].additiveEffect[index].value = org + org*(value/100.0f);
									break;
								case en_eff_vt_sub_value :
									newskillinfo->detailInfo[ii].additiveEffect[index].value = org - value;
									break;
								case en_eff_vt_sub_percent :
									newskillinfo->detailInfo[ii].additiveEffect[index].value = org - (org*(value/100.0f));
									break;
								default :
									newskillinfo->detailInfo[ii].additiveEffect[index].value = org;
									break;
								}
							}

							if(newskillinfo->detailInfo[ii].additiveEffect[index].value < 0)
								newskillinfo->detailInfo[ii].additiveEffect[index].value = 0;
						} // for(12)
						newskillinfo->bUse[en_mod_additive_1_value+index] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillAdditiveProb(short skillid, char point, int index, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							if(g_SkillProperty[targetskill[i]]->detailInfo[ii].additiveCount > index)
							{
								short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].additiveEffect[index].prob;
								short bonus = 0;
								if(currentskill->detailInfo[ii].additiveEffect[index].prob != 0)
									bonus = currentskill->detailInfo[ii].additiveEffect[index].prob;
								else
									bonus = org;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].additiveEffect[index].prob = bonus + value;
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].additiveEffect[index].prob = bonus + org*(value/100.0f);
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].additiveEffect[index].prob = bonus - value;
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].additiveEffect[index].prob = bonus - (org*(value/100.0f));
									break;
								default :
									currentskill->detailInfo[ii].additiveEffect[index].prob = bonus;
									break;
								}
							}

							if(currentskill->detailInfo[ii].additiveEffect[index].prob < 0)
								currentskill->detailInfo[ii].additiveEffect[index].prob = 0;
						} // for(12)
						currentskill->bUse[en_mod_additive_1_prob+index] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						if(g_SkillProperty[targetskill[i]]->detailInfo[ii].additiveCount > index)
						{
							short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].additiveEffect[index].prob;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].additiveEffect[index].prob = org + value;
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].additiveEffect[index].prob = org + org*(value/100.0f);
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].additiveEffect[index].prob = org - value;
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].additiveEffect[index].prob = org - (org*(value/100.0f));
								break;
							default :
								newskillinfo->detailInfo[ii].additiveEffect[index].prob = org;
								break;
							}
						}

						if(newskillinfo->detailInfo[ii].additiveEffect[index].prob < 0)
							newskillinfo->detailInfo[ii].additiveEffect[index].prob = 0;
					} // for(12)
					newskillinfo->bUse[en_mod_additive_1_prob+index] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} //for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								if(currentskilltable->detailInfo[ii].additiveCount > index)
								{
									short org = currentskilltable->detailInfo[ii].additiveEffect[index].prob;
									short bonus = 0;
									if(currentskill->detailInfo[ii].additiveEffect[index].prob != 0)
										bonus = currentskill->detailInfo[ii].additiveEffect[index].prob;
									else
										bonus = org;
									
									switch(g_CharacterStateTable[id]->value_type)
									{
									case en_eff_vt_add_value :
										currentskill->detailInfo[ii].additiveEffect[index].prob = bonus + value;
										break;
									case en_eff_vt_add_percent :
										currentskill->detailInfo[ii].additiveEffect[index].prob = bonus + org*(value/100.0f);
										break;
									case en_eff_vt_sub_value :
										currentskill->detailInfo[ii].additiveEffect[index].prob = bonus - value;
										break;
									case en_eff_vt_sub_percent :
										currentskill->detailInfo[ii].additiveEffect[index].prob = bonus - (org*(value/100.0f));
										break;
									default :
										currentskill->detailInfo[ii].additiveEffect[index].prob = bonus;
										break;
									}
								}

								if(currentskill->detailInfo[ii].additiveEffect[index].prob < 0)
									currentskill->detailInfo[ii].additiveEffect[index].prob = 0;
							} // for(12)
							currentskill->bUse[en_mod_additive_1_prob+index] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							if(currentskilltable->detailInfo[ii].additiveCount > index)
							{
								short org = currentskilltable->detailInfo[ii].additiveEffect[index].prob;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									newskillinfo->detailInfo[ii].additiveEffect[index].prob = org + value;
									break;
								case en_eff_vt_add_percent :
									newskillinfo->detailInfo[ii].additiveEffect[index].prob = org + org*(value/100.0f);
									break;
								case en_eff_vt_sub_value :
									newskillinfo->detailInfo[ii].additiveEffect[index].prob = org - value;
									break;
								case en_eff_vt_sub_percent :
									newskillinfo->detailInfo[ii].additiveEffect[index].prob = org - (org*(value/100.0f));
									break;
								default :
									newskillinfo->detailInfo[ii].additiveEffect[index].prob = org;
									break;
								}
							}

							if(newskillinfo->detailInfo[ii].additiveEffect[index].prob < 0)
								newskillinfo->detailInfo[ii].additiveEffect[index].prob = 0;
						} // for(12)
						newskillinfo->bUse[en_mod_additive_1_prob+index] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::ModifySkillAdditiveTime(short skillid, char point, int index, short id, short value)
{
	short targetskill[4];
	memset(targetskill, 0, sizeof(targetskill));
	
	targetskill[0] = g_SkillMasteryProperty[skillid]->sTargetSkill_1;
	targetskill[1] = g_SkillMasteryProperty[skillid]->sTargetSkill_2;
	targetskill[2] = g_SkillMasteryProperty[skillid]->sTargetSkill_3;
	targetskill[3] = g_SkillMasteryProperty[skillid]->sTargetSkill_4;
	
	for(int i = 0 ; i < 4 ; ++i)
	{
		if(targetskill[i] > 0)
		{
			map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskill[i]);
			if(iter_mod != g_Skill_Mod_Property.end())
			{
				_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
				if(currentskill)
				{
					if(_XSkillItem::FindSkillProperty(targetskill[i]))
					{
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							if(g_SkillProperty[targetskill[i]]->detailInfo[ii].additiveCount > index)
							{
								short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].additiveEffect[index].count;
								short bonus = 0;
								if(currentskill->detailInfo[ii].additiveEffect[index].count != 0)
									bonus = currentskill->detailInfo[ii].additiveEffect[index].count;
								else
									bonus = org;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									currentskill->detailInfo[ii].additiveEffect[index].count = bonus + value;
									break;
								case en_eff_vt_add_percent :
									currentskill->detailInfo[ii].additiveEffect[index].count = bonus + org*(value/100.0f);
									break;
								case en_eff_vt_sub_value :
									currentskill->detailInfo[ii].additiveEffect[index].count = bonus - value;
									break;
								case en_eff_vt_sub_percent :
									currentskill->detailInfo[ii].additiveEffect[index].count = bonus - (org*(value/100.0f));
									break;
								default :
									currentskill->detailInfo[ii].additiveEffect[index].count = bonus;
									break;
								}
							}

							if(currentskill->detailInfo[ii].additiveEffect[index].count < 0)
								currentskill->detailInfo[ii].additiveEffect[index].count = 0;
						} // for(12)
						currentskill->bUse[en_mod_additive_1_time+index] = TRUE;
					}
				}
			}
			else
			{
				_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
				memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
				
				if(_XSkillItem::FindSkillProperty(targetskill[i]))
				{
					for(int ii = 0 ; ii < 12 ; ++ii)
					{
						if(g_SkillProperty[targetskill[i]]->detailInfo[ii].additiveCount > index)
						{
							short org = g_SkillProperty[targetskill[i]]->detailInfo[ii].additiveEffect[index].count;
							
							switch(g_CharacterStateTable[id]->value_type)
							{
							case en_eff_vt_add_value :
								newskillinfo->detailInfo[ii].additiveEffect[index].count = org + value;
								break;
							case en_eff_vt_add_percent :
								newskillinfo->detailInfo[ii].additiveEffect[index].count = org + org*(value/100.0f);
								break;
							case en_eff_vt_sub_value :
								newskillinfo->detailInfo[ii].additiveEffect[index].count = org - value;
								break;
							case en_eff_vt_sub_percent :
								newskillinfo->detailInfo[ii].additiveEffect[index].count = org - (org*(value/100.0f));
								break;
							default :
								newskillinfo->detailInfo[ii].additiveEffect[index].count = org;
								break;
							}
						}

						if(newskillinfo->detailInfo[ii].additiveEffect[index].count < 0)
							newskillinfo->detailInfo[ii].additiveEffect[index].count = 0;
					} // for(12)
					newskillinfo->bUse[en_mod_additive_1_time+index] = TRUE;
				}
				g_Skill_Mod_Property[targetskill[i]] = newskillinfo;
			}
		} // if(targetskill[i] > 0)
	} //for(targetskill)

	// stype
	char cClanType = 0, cSType = 0;
	cClanType = g_SkillMasteryProperty[skillid]->cClanType;
	cSType = g_SkillMasteryProperty[skillid]->cTargetSkill_Stype;
	
	if(cSType)
	{
		map <short, _XSKILLINFOSTRUCTURE*>::iterator iter_skill;
		for(iter_skill = g_SkillProperty.begin() ; iter_skill != g_SkillProperty.end() ; ++iter_skill)
		{
			_XSKILLINFOSTRUCTURE* currentskilltable = iter_skill->second;
			if(currentskilltable)
			{
				if(currentskilltable->clanType == cClanType && currentskilltable->skillSType == cSType)
				{
					short targetskillid = iter_skill->first;
					
					map <short, _XSKILLINFO_MOD_STRUCTURE*>::iterator iter_mod = g_Skill_Mod_Property.find(targetskillid);
					if(iter_mod != g_Skill_Mod_Property.end())
					{
						_XSKILLINFO_MOD_STRUCTURE* currentskill = iter_mod->second;
						if(currentskill)
						{
							for(int ii = 0 ; ii < 12 ; ++ii)
							{
								if(currentskilltable->detailInfo[ii].additiveCount > index)
								{
									short org = currentskilltable->detailInfo[ii].additiveEffect[index].count;
									short bonus = 0;
									if(currentskill->detailInfo[ii].additiveEffect[index].count != 0)
										bonus = currentskill->detailInfo[ii].additiveEffect[index].count;
									else
										bonus = org;
									
									switch(g_CharacterStateTable[id]->value_type)
									{
									case en_eff_vt_add_value :
										currentskill->detailInfo[ii].additiveEffect[index].count = bonus + value;
										break;
									case en_eff_vt_add_percent :
										currentskill->detailInfo[ii].additiveEffect[index].count = bonus + org*(value/100.0f);
										break;
									case en_eff_vt_sub_value :
										currentskill->detailInfo[ii].additiveEffect[index].count = bonus - value;
										break;
									case en_eff_vt_sub_percent :
										currentskill->detailInfo[ii].additiveEffect[index].count = bonus - (org*(value/100.0f));
										break;
									default :
										currentskill->detailInfo[ii].additiveEffect[index].count = bonus;
										break;
									}
								}

								if(currentskill->detailInfo[ii].additiveEffect[index].count < 0)
									currentskill->detailInfo[ii].additiveEffect[index].count = 0;
							} // for(12)
							currentskill->bUse[en_mod_additive_1_prob+index] = TRUE;
						}
					}
					else
					{
						_XSKILLINFO_MOD_STRUCTURE* newskillinfo = new _XSKILLINFO_MOD_STRUCTURE;
						memset(newskillinfo, 0, sizeof(_XSKILLINFO_MOD_STRUCTURE));
						
						for(int ii = 0 ; ii < 12 ; ++ii)
						{
							if(currentskilltable->detailInfo[ii].additiveCount > index)
							{
								short org = currentskilltable->detailInfo[ii].additiveEffect[index].count;
								
								switch(g_CharacterStateTable[id]->value_type)
								{
								case en_eff_vt_add_value :
									newskillinfo->detailInfo[ii].additiveEffect[index].count = org + value;
									break;
								case en_eff_vt_add_percent :
									newskillinfo->detailInfo[ii].additiveEffect[index].count = org + org*(value/100.0f);
									break;
								case en_eff_vt_sub_value :
									newskillinfo->detailInfo[ii].additiveEffect[index].count = org - value;
									break;
								case en_eff_vt_sub_percent :
									newskillinfo->detailInfo[ii].additiveEffect[index].count = org - (org*(value/100.0f));
									break;
								default :
									newskillinfo->detailInfo[ii].additiveEffect[index].count = org;
									break;
								}
							}

							if(newskillinfo->detailInfo[ii].additiveEffect[index].count < 0)
								newskillinfo->detailInfo[ii].additiveEffect[index].count = 0;
						} // for(12)
						newskillinfo->bUse[en_mod_additive_1_time+index] = TRUE;

						g_Skill_Mod_Property[targetskillid] = newskillinfo;
					}
				}
			}
		} // for(skillproperty)
	}
}

void _XCharacterStateList::GetBaseBonusFromItem(void)
{
	short id = 0;

	// weapon 1
	id = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID();
	_XGI_FirstCategory type = (_XGI_FirstCategory)g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType();
	
	if(id > 0)
	{
		if( _XGI_FC_WEAPON == GetEqualItemType(type) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(type);					
			if( WeaponItem )
			{
#ifdef _XTS_FAME
				if( _XGameItem::CheckFame(WeaponItem[id].ucFame) )
#endif
				{
					for(int i = 0 ; i < 5 ; ++i)
					{			
						if(WeaponItem[id].usAdd_SEID[i] > 0)
						{
							map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(WeaponItem[id].usAdd_SEID[i]);
							if(iter_table != g_CharacterStateTable.end())
							{
								if(g_CharacterStateTable[WeaponItem[id].usAdd_SEID[i]]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
								{
									switch(g_CharacterStateTable[WeaponItem[id].usAdd_SEID[i]]->where)
									{
									case en_eff_where_strength :		// �ٷ�
										{
											AddStrengthBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										}
										break;
									case en_eff_where_zen :				// ����
										{
											AddZenBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										}
										break;
									case en_eff_where_intelligence :	// ����
										{
											AddIntBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										}
										break;
									case en_eff_where_constitution :	// �ǰ�
										{
											AddConstitutionBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										}
										break;
									case en_eff_where_dexterity :		// ��ø
										{
											AddDexBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										}
										break;
									case en_eff_where_all_stat :
										{
											AddStrengthBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
											AddZenBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
											AddIntBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
											AddConstitutionBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
											AddDexBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										}
										break;
									}
								}
							}
						}
					}
					// ���� Ȯ��
					if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_ucSocketCount() > 0)
					{
						// _XTS_sUser_Inven_v1_all ------------------------------------------------------------------��			
						int socketcount = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_ucSocketCount();
						short socketid = 0;
						for(i = 0 ; i < 4 ; ++i)
						{
							if(i >= socketcount)
								continue;
							
							socketid = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sSocket(i);

							if(g_SocketItemProperty[socketid].sSkill_Effect_ID1 > 0)
							{
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketid].sSkill_Effect_ID1);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID1]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										switch(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID1]->where)
										{
										case en_eff_where_strength :		// �ٷ�
											{
												AddStrengthBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											}
											break;
										case en_eff_where_zen :				// ����
											{
												AddZenBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											}
											break;
										case en_eff_where_intelligence :	// ����
											{
												AddIntBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											}
											break;
										case en_eff_where_constitution :	// �ǰ�
											{
												AddConstitutionBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											}
											break;
										case en_eff_where_dexterity :		// ��ø
											{
												AddDexBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											}
											break;
										case en_eff_where_all_stat :
											{
												AddStrengthBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
												AddZenBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
												AddIntBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
												AddConstitutionBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
												AddDexBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											}
											break;
										}
									}
								}
							}

							if(g_SocketItemProperty[socketid].sSkill_Effect_ID2 > 0)
							{
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketid].sSkill_Effect_ID2);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID2]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										switch(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID2]->where)
										{
										case en_eff_where_strength :		// �ٷ�
											{
												AddStrengthBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											}
											break;
										case en_eff_where_zen :				// ����
											{
												AddZenBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											}
											break;
										case en_eff_where_intelligence :	// ����
											{
												AddIntBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											}
											break;
										case en_eff_where_constitution :	// �ǰ�
											{
												AddConstitutionBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											}
											break;
										case en_eff_where_dexterity :		// ��ø
											{
												AddDexBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											}
											break;
										case en_eff_where_all_stat :
											{
												AddStrengthBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
												AddZenBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
												AddIntBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
												AddConstitutionBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
												AddDexBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											}
											break;
										}
									}
								}
							}							
						}
						// item mix �� ���� �߰��� ------------------------------------------------------------------------------��				
						int addBonus = _XGameItem::GetAddEffectFromSocket(  g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sSocket(0),
																			g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sSocket(1),
																			g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sSocket(2),
																			g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sSocket(3) );
						if( addBonus > 0 )
						{
							if(g_SocketItemProperty[addBonus].sSkill_Effect_ID1 > 0)
							{
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[addBonus].sSkill_Effect_ID1);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID1]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										switch(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID1]->where)
										{
										case en_eff_where_strength :		// �ٷ�
											{
												AddStrengthBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											}
											break;
										case en_eff_where_zen :				// ����
											{
												AddZenBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											}
											break;
										case en_eff_where_intelligence :	// ����
											{
												AddIntBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											}
											break;
										case en_eff_where_constitution :	// �ǰ�
											{
												AddConstitutionBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											}
											break;
										case en_eff_where_dexterity :		// ��ø
											{
												AddDexBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											}
											break;
										case en_eff_where_all_stat :
											{
												AddStrengthBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
												AddZenBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
												AddIntBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
												AddConstitutionBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
												AddDexBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											}
											break;
										}
									}
								}
							}
							if(g_SocketItemProperty[addBonus].sSkill_Effect_ID2 > 0)
							{
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[addBonus].sSkill_Effect_ID2);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID2]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										switch(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID2]->where)
										{
										case en_eff_where_strength :		// �ٷ�
											{
												AddStrengthBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											}
											break;
										case en_eff_where_zen :				// ����
											{
												AddZenBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											}
											break;
										case en_eff_where_intelligence :	// ����
											{
												AddIntBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											}
											break;
										case en_eff_where_constitution :	// �ǰ�
											{
												AddConstitutionBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											}
											break;
										case en_eff_where_dexterity :		// ��ø
											{
												AddDexBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											}
											break;
										case en_eff_where_all_stat :
											{
												AddStrengthBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
												AddZenBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
												AddIntBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
												AddConstitutionBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
												AddDexBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											}
											break;
										}
									}
								}
							}
						}	// item mix �� ���� �߰��� ------------------------------------------------------------------------------��
						// _XTS_sUser_Inven_v1_all ------------------------------------------------------------------��
					}
				}				
			}	
		}		
	}
	
	for( int pos = _XINVENTORY_SLOTTYPE_AMOR; pos <= _XINVENTORY_SLOTTYPE_MASK; ++pos )
	{
		if(pos == _XINVENTORY_SLOTTYPE_GAP_UPPER || pos == _XINVENTORY_SLOTTYPE_GAP_LOWER)
		{
			// ��/�ϰ� �ڸ����� ����ȭ ����ǰ(�Ǽ��縮) ����
			continue;
		}

		id = g_pLocalUser->m_UserItemList[pos].Get_m_sID();
		type = (_XGI_FirstCategory)g_pLocalUser->m_UserItemList[pos].Get_m_cType();
		
		if(id > 0)
		{
			if( _XGI_FC_CLOTHES == GetEqualItemType(type) )
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(type);					
				if( ClothesItem )
				{
#ifdef _XTS_FAME
					if( _XGameItem::CheckFame(ClothesItem[id].ucFame) )
#endif
					{
						for(int i = 0 ; i < 5 ; ++i)
						{				
							if(ClothesItem[id].usAdd_SEID[i] > 0)
							{
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(ClothesItem[id].usAdd_SEID[i]);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[ClothesItem[id].usAdd_SEID[i]]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										switch(g_CharacterStateTable[ClothesItem[id].usAdd_SEID[i]]->where)
										{
										case en_eff_where_strength :		// �ٷ�
											{
												AddStrengthBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											}
											break;
										case en_eff_where_zen :				// ����
											{
												AddZenBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											}
											break;
										case en_eff_where_intelligence :	// ����
											{
												AddIntBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											}
											break;
										case en_eff_where_constitution :	// �ǰ�
											{
												AddConstitutionBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											}
											break;
										case en_eff_where_dexterity :		// ��ø
											{
												AddDexBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											}
											break;
										case en_eff_where_all_stat :
											{
												AddStrengthBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
												AddZenBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
												AddIntBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
												AddConstitutionBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
												AddDexBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											}
											break;
										}
									}
								}
							}
						}
						// ���� Ȯ��
						if(g_pLocalUser->m_UserItemList[pos].Get_m_ucSocketCount() > 0)
						{
							// _XTS_sUser_Inven_v1_all ------------------------------------------------------------------��
							int socketcount = g_pLocalUser->m_UserItemList[pos].Get_m_ucSocketCount();
							short socketid = 0;
							for(i = 0 ; i < 4 ; ++i)
							{
								if(i >= socketcount)
									continue;

								socketid = g_pLocalUser->m_UserItemList[pos].Get_m_sSocket(i);
								if(g_SocketItemProperty[socketid].sSkill_Effect_ID3 > 0)
								{
									map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketid].sSkill_Effect_ID3);
									if(iter_table != g_CharacterStateTable.end())
									{
										if(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID3]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
										{
											switch(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID3]->where)
											{
											case en_eff_where_strength :		// �ٷ�
												{
													AddStrengthBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												}
												break;
											case en_eff_where_zen :				// ����
												{
													AddZenBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												}
												break;
											case en_eff_where_intelligence :	// ����
												{
													AddIntBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												}
												break;
											case en_eff_where_constitution :	// �ǰ�
												{
													AddConstitutionBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												}
												break;
											case en_eff_where_dexterity :		// ��ø
												{
													AddDexBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												}
												break;
											case en_eff_where_all_stat :
												{
													AddStrengthBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
													AddZenBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
													AddIntBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
													AddConstitutionBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
													AddDexBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												}
												break;
											}
										}
									}
								}
								if(g_SocketItemProperty[socketid].sSkill_Effect_ID4 > 0)
								{
									map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketid].sSkill_Effect_ID4);
									if(iter_table != g_CharacterStateTable.end())
									{
										if(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID4]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
										{
											switch(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID4]->where)
											{
											case en_eff_where_strength :		// �ٷ�
												{
													AddStrengthBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												}
												break;
											case en_eff_where_zen :				// ����
												{
													AddZenBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												}
												break;
											case en_eff_where_intelligence :	// ����
												{
													AddIntBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												}
												break;
											case en_eff_where_constitution :	// �ǰ�
												{
													AddConstitutionBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												}
												break;
											case en_eff_where_dexterity :		// ��ø
												{
													AddDexBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												}
												break;
											case en_eff_where_all_stat :
												{
													AddStrengthBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
													AddZenBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
													AddIntBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
													AddConstitutionBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
													AddDexBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												}
												break;
											}
										}
									}
								}
							}
							// item mix �� ���� �߰��� ------------------------------------------------------------------------------��				
							int addBonus = _XGameItem::GetAddEffectFromSocket(  g_pLocalUser->m_UserItemList[pos].Get_m_sSocket(0),
								g_pLocalUser->m_UserItemList[pos].Get_m_sSocket(1),
								g_pLocalUser->m_UserItemList[pos].Get_m_sSocket(2),
								g_pLocalUser->m_UserItemList[pos].Get_m_sSocket(3) );
							if( addBonus > 0 )
							{
								if(g_SocketItemProperty[addBonus].sSkill_Effect_ID3 > 0)
								{
									map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[addBonus].sSkill_Effect_ID3);
									if(iter_table != g_CharacterStateTable.end())
									{
										if(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID3]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
										{
											switch(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID3]->where)
											{
											case en_eff_where_strength :		// �ٷ�
												{
													AddStrengthBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												}
												break;
											case en_eff_where_zen :				// ����
												{
													AddZenBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												}
												break;
											case en_eff_where_intelligence :	// ����
												{
													AddIntBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												}
												break;
											case en_eff_where_constitution :	// �ǰ�
												{
													AddConstitutionBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												}
												break;
											case en_eff_where_dexterity :		// ��ø
												{
													AddDexBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												}
												break;
											case en_eff_where_all_stat :
												{
													AddStrengthBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
													AddZenBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
													AddIntBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
													AddConstitutionBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
													AddDexBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												}
												break;
											}
										}
									}
								}
								if(g_SocketItemProperty[addBonus].sSkill_Effect_ID4 > 0)
								{
									map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[addBonus].sSkill_Effect_ID4);
									if(iter_table != g_CharacterStateTable.end())
									{
										if(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID4]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
										{
											switch(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID4]->where)
											{
											case en_eff_where_strength :		// �ٷ�
												{
													AddStrengthBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												}
												break;
											case en_eff_where_zen :				// ����
												{
													AddZenBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												}
												break;
											case en_eff_where_intelligence :	// ����
												{
													AddIntBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												}
												break;
											case en_eff_where_constitution :	// �ǰ�
												{
													AddConstitutionBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												}
												break;
											case en_eff_where_dexterity :		// ��ø
												{
													AddDexBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												}
												break;
											case en_eff_where_all_stat :
												{
													AddStrengthBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
													AddZenBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
													AddIntBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
													AddConstitutionBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
													AddDexBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												}
												break;
											}
										}
									}
								}
							}	// item mix �� ���� �߰��� ------------------------------------------------------------------------------��
							// _XTS_sUser_Inven_v1_all ------------------------------------------------------------------��
						}
					}
				}
			}			
		}
	}			
	
#ifdef _XDEF_PERIODITEM_20061027
	// ��, �ϰ� ��ġ�� �ִ� �Ⱓ�� ������ ȿ�� ����	
	for( pos = _XINVENTORY_SLOTTYPE_GAP_UPPER; pos <= _XINVENTORY_SLOTTYPE_GAP_LOWER; ++pos )
	{
		id = g_pLocalUser->m_UserItemList[pos].Get_m_sID();
		if(id > 0)
		{
#ifdef _XTS_FAME
			if( _XGameItem::CheckFame(g_AccessoryItemProperty[id].ucFame) 
#ifdef _XTS_ITEM_V22
				//				&& _XGameItem::CheckAccessoryReq(g_pLocalUser->m_UserItemList[pos].m_cType, id)
#endif
				)
#endif
			{
				for(int i = 0 ; i < 5 ; ++i)
				{
					if(g_AccessoryItemProperty[id].usAdd_SEID[i] > 0)
					{
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_AccessoryItemProperty[id].usAdd_SEID[i]);
						if(iter_table != g_CharacterStateTable.end())
						{
							if(g_CharacterStateTable[g_AccessoryItemProperty[id].usAdd_SEID[i]]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
							{
								switch(g_CharacterStateTable[g_AccessoryItemProperty[id].usAdd_SEID[i]]->where)
								{
								case en_eff_where_strength :		// �ٷ�
									{
										AddStrengthBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									}
									break;
								case en_eff_where_zen :				// ����
									{
										AddZenBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									}
									break;
								case en_eff_where_intelligence :	// ����
									{
										AddIntBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									}
									break;
								case en_eff_where_constitution :	// �ǰ�
									{
										AddConstitutionBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									}
									break;
								case en_eff_where_dexterity :		// ��ø
									{
										AddDexBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									}
									break;
								case en_eff_where_all_stat :
									{
										AddStrengthBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
										AddZenBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
										AddIntBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
										AddConstitutionBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
										AddDexBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									}
									break;
								}
							}
						}
					}
				}
			}			
		}
	}
#endif
	// �Ǽ��縮
	for( pos = _XINVENTORY_SLOTTYPE_ACCESSORY_1; pos <= _XINVENTORY_SLOTTYPE_ACCESSORY_8; ++pos )
	{	
		id = g_pLocalUser->m_UserItemList[pos].Get_m_sID();
		if(id > 0)
		{
			// �Ǽ��縮 ȿ�� ���� üũ - Author : �ڼ���
			int index = pos - 11;
			if(index >= 1 && index <= 4)
			{
				// ���ǿ� �Ǽ��縮
				if(g_pLocalUser->m_cAmorPocketNum < index)
				{
					// �ָӴ� �������� ���� �Ǽ��縮 - ȿ�� �������� ����
					continue;
				}
			}
			else if(index >= 5 && index <= 8)
			{
				// ���ǿ� �Ǽ��縮
				index -= 4;
				if(g_pLocalUser->m_cPantsPocketNum < index)
				{
					// �ָӴ� �������� ���� �Ǽ��縮 - ȿ�� �������� ����
					continue;
				}
			}

#ifdef _XTS_FAME
			if( _XGameItem::CheckFame(g_AccessoryItemProperty[id].ucFame) 
#ifdef _XTS_ITEM_V22
//				&& _XGameItem::CheckAccessoryReq(g_pLocalUser->m_UserItemList[pos].m_cType, id)
#endif
				)
#endif
			{
				for(int i = 0 ; i < 5 ; ++i)
				{
					if(g_AccessoryItemProperty[id].usAdd_SEID[i] > 0)
					{
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_AccessoryItemProperty[id].usAdd_SEID[i]);
						if(iter_table != g_CharacterStateTable.end())
						{
							if(g_CharacterStateTable[g_AccessoryItemProperty[id].usAdd_SEID[i]]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
							{
								switch(g_CharacterStateTable[g_AccessoryItemProperty[id].usAdd_SEID[i]]->where)
								{
								case en_eff_where_strength :		// �ٷ�
									{
										AddStrengthBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									}
									break;
								case en_eff_where_zen :				// ����
									{
										AddZenBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									}
									break;
								case en_eff_where_intelligence :	// ����
									{
										AddIntBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									}
									break;
								case en_eff_where_constitution :	// �ǰ�
									{
										AddConstitutionBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									}
									break;
								case en_eff_where_dexterity :		// ��ø
									{
										AddDexBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									}
									break;
								case en_eff_where_all_stat :
									{
										AddStrengthBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
										AddZenBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
										AddIntBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
										AddConstitutionBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
										AddDexBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									}
									break;
								}
							}
						}
					}
				}
			}			
		}
	}
}


void _XCharacterStateList::GetAdditionalBonusFromItem(void)
{
	short id = 0;

	// weapon 1
	id = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID();
	_XGI_FirstCategory type = (_XGI_FirstCategory)g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType();
	
	if(id > 0)	// 1
	{
		if( _XGI_FC_WEAPON == GetEqualItemType(type) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(type);
			if( WeaponItem )
			{
#ifdef _XTS_FAME
				if( _XGameItem::CheckFame(WeaponItem[id].ucFame) )
#endif
				{
					for(int i = 0 ; i < 5 ; ++i)
					{
						if(WeaponItem[id].usAdd_SEID[i] > 0)
						{
							map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(WeaponItem[id].usAdd_SEID[i]);
							if(iter_table != g_CharacterStateTable.end())
							{
								if(g_CharacterStateTable[WeaponItem[id].usAdd_SEID[i]]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
								{
									switch(g_CharacterStateTable[WeaponItem[id].usAdd_SEID[i]]->where)
									{
									case en_eff_where_attack_rate :			// ���� ����
										AddPAttackRateBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_avoid_rate :			// ���� ȸ��
										AddPAvoidRateBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_attack_damage :		// ���� ����
										AddPAttackDamageBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_defence :				// ���� ���
										AddDefenceBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_m_attack_rate :		// ��� ����
										AddMAttackRateBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_m_avoid_rate :		// ��� ȸ��
										AddMAvoidRateBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_m_attack_damage :		// ��� ����
										AddMAttackDamageBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_m_defence :			// ��� ���
										AddMDefenceBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_max_life :			// �ִ� ����
										AddMaxLifeBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);				
										break;
									case en_eff_where_max_force :			// �ִ� ����
										AddMaxForceBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_max_concentration :	// �ִ� ����
										AddMaxConcentrationBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_move_speed :			// �̵� �ӵ�
										AddMoveSpeedBonus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_weapon_dam :			// ���� ���ݷ� ����
										AddWeaponDamage(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_weapon_min_dam :		// ���� �ּ� ���ݷ� ����
										AddWeaponMinDamage(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_weapon_max_dam :		// ���� �ִ� ���ݷ� ����
										AddWeaponMaxDamage(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_resist_minus :		// ���� ���׷�
										AddResistMinus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_resist_plus :			// ���� ���׷�
										AddResistPlus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_resist_hon :			// ȥ�� ���׷�
										AddResistHon(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_resist_sin :			// ���� ���׷�
										AddResistSin(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_resist_all :			// ��� ���׷� ����
										AddResistAll(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_add_minus_damage :		// ���� ������ �߰�
										AddAttackDamageMinus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_add_plus_damage :			// ���� ������ �߰�
										AddAttackDamagePlus(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
										AddAttackDamageHon(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									case en_eff_where_add_sin_damage :			// ���� ������ �߰�
										AddAttackDamageSin(WeaponItem[id].usAdd_SEID[i], WeaponItem[id].usAdd_Value[i]);
										break;
									}
								}
							}
						}											
					}
					// ���� Ȯ��
					if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_ucSocketCount() > 0)
					{
						// #ifdef _XTS_sUser_Inven_v1_all ----------------------------------------------------------------------��
						int socketcount = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_ucSocketCount();
						short socketid = 0;
						for(int i = 0 ; i < 4 ; ++i)
						{
							if(i >= socketcount)
								continue;

							socketid = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sSocket(i);
							
							if(g_SocketItemProperty[socketid].sSkill_Effect_ID1 > 0)
							{
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketid].sSkill_Effect_ID1);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID1]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										switch(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID1]->where)
										{
										case en_eff_where_attack_rate :			// ���� ����
											AddPAttackRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_avoid_rate :			// ���� ȸ��
											AddPAvoidRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_attack_damage :		// ���� ����
											AddPAttackDamageBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_defence :				// ���� ���
											AddDefenceBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_m_attack_rate :		// ��� ����
											AddMAttackRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_m_avoid_rate :		// ��� ȸ��
											AddMAvoidRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_m_attack_damage :		// ��� ����
											AddMAttackDamageBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_m_defence :			// ��� ���
											AddMDefenceBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_max_life :			// �ִ� ����
											AddMaxLifeBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);				
											break;
										case en_eff_where_max_force :			// �ִ� ����
											AddMaxForceBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_max_concentration :	// �ִ� ����
											AddMaxConcentrationBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_move_speed :			// �̵� �ӵ�
											AddMoveSpeedBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_weapon_dam :			// ���� ���ݷ� ����
											AddWeaponDamage(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_weapon_min_dam :		// ���� �ּ� ���ݷ� ����
											AddWeaponMinDamage(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_weapon_max_dam :		// ���� �ִ� ���ݷ� ����
											AddWeaponMaxDamage(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_resist_minus :		// ���� ���׷�
											AddResistMinus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_resist_plus :			// ���� ���׷�
											AddResistPlus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_resist_hon :			// ȥ�� ���׷�
											AddResistHon(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_resist_sin :			// ���� ���׷�
											AddResistSin(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_resist_all :			// ��� ���׷� ����
											AddResistAll(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_add_minus_damage :		// ���� ������ �߰�
											AddAttackDamageMinus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_add_plus_damage :			// ���� ������ �߰�
											AddAttackDamagePlus(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
											AddAttackDamageHon(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										case en_eff_where_add_sin_damage :			// ���� ������ �߰�
											AddAttackDamageSin(g_SocketItemProperty[socketid].sSkill_Effect_ID1, g_SocketItemProperty[socketid].sValue1);
											break;
										}
									}
								}
							}
							if(g_SocketItemProperty[socketid].sSkill_Effect_ID2 > 0)
							{
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketid].sSkill_Effect_ID2);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID2]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										switch(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID2]->where)
										{
										case en_eff_where_attack_rate :			// ���� ����
											AddPAttackRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_avoid_rate :			// ���� ȸ��
											AddPAvoidRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_attack_damage :		// ���� ����
											AddPAttackDamageBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_defence :				// ���� ���
											AddDefenceBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_m_attack_rate :		// ��� ����
											AddMAttackRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_m_avoid_rate :		// ��� ȸ��
											AddMAvoidRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_m_attack_damage :		// ��� ����
											AddMAttackDamageBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_m_defence :			// ��� ���
											AddMDefenceBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_max_life :			// �ִ� ����
											AddMaxLifeBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);				
											break;
										case en_eff_where_max_force :			// �ִ� ����
											AddMaxForceBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_max_concentration :	// �ִ� ����
											AddMaxConcentrationBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_move_speed :			// �̵� �ӵ�
											AddMoveSpeedBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_weapon_dam :			// ���� ���ݷ� ����
											AddWeaponDamage(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_weapon_min_dam :		// ���� �ּ� ���ݷ� ����
											AddWeaponMinDamage(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_weapon_max_dam :		// ���� �ִ� ���ݷ� ����
											AddWeaponMaxDamage(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_resist_minus :			// ���� ���׷�
											AddResistMinus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_resist_plus :				// ���� ���׷�
											AddResistPlus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_resist_hon :				// ȥ�� ���׷�
											AddResistHon(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_resist_sin :				// ���� ���׷�
											AddResistSin(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_resist_all :			// ��� ���׷� ����
											AddResistAll(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_add_minus_damage :		// ���� ������ �߰�
											AddAttackDamageMinus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_add_plus_damage :			// ���� ������ �߰�
											AddAttackDamagePlus(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
											AddAttackDamageHon(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										case en_eff_where_add_sin_damage :			// ���� ������ �߰�
											AddAttackDamageSin(g_SocketItemProperty[socketid].sSkill_Effect_ID2, g_SocketItemProperty[socketid].sValue2);
											break;
										}
									}
								}
							}
						} // for	
						
						// item mix �� ���� �߰��� -----------------------------------------------------------------��			
						int addBonus = _XGameItem::GetAddEffectFromSocket(  g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sSocket(0),
																			g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sSocket(1),
																			g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sSocket(2),
																			g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sSocket(3));
						
						if( addBonus > 0 )
						{
							if(g_SocketItemProperty[addBonus].sSkill_Effect_ID1 > 0)
							{
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[addBonus].sSkill_Effect_ID1);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID1]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										switch(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID1]->where)
										{
										case en_eff_where_attack_rate :			// ���� ����
											AddPAttackRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_avoid_rate :			// ���� ȸ��
											AddPAvoidRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_attack_damage :		// ���� ����
											AddPAttackDamageBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_defence :				// ���� ���
											AddDefenceBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_m_attack_rate :		// ��� ����
											AddMAttackRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_m_avoid_rate :		// ��� ȸ��
											AddMAvoidRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_m_attack_damage :		// ��� ����
											AddMAttackDamageBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_m_defence :			// ��� ���
											AddMDefenceBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_max_life :			// �ִ� ����
											AddMaxLifeBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);				
											break;
										case en_eff_where_max_force :			// �ִ� ����
											AddMaxForceBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_max_concentration :	// �ִ� ����
											AddMaxConcentrationBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_move_speed :			// �̵� �ӵ�
											AddMoveSpeedBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_weapon_dam :			// ���� ���ݷ� ����
											AddWeaponDamage(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_weapon_min_dam :		// ���� �ּ� ���ݷ� ����
											AddWeaponMinDamage(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_weapon_max_dam :		// ���� �ִ� ���ݷ� ����
											AddWeaponMaxDamage(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_resist_minus :			// ���� ���׷�
											AddResistMinus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_resist_plus :				// ���� ���׷�
											AddResistPlus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_resist_hon :				// ȥ�� ���׷�
											AddResistHon(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_resist_sin :				// ���� ���׷�
											AddResistSin(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_resist_all :			// ��� ���׷� ����
											AddResistAll(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_add_minus_damage :		// ���� ������ �߰�
											AddAttackDamageMinus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_add_plus_damage :			// ���� ������ �߰�
											AddAttackDamagePlus(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
											AddAttackDamageHon(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										case en_eff_where_add_sin_damage :			// ���� ������ �߰�
											AddAttackDamageSin(g_SocketItemProperty[addBonus].sSkill_Effect_ID1, g_SocketItemProperty[addBonus].sValue1);
											break;
										}
									}
								}
							}
							if(g_SocketItemProperty[addBonus].sSkill_Effect_ID2 > 0)
							{
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[addBonus].sSkill_Effect_ID2);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID2]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										switch(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID2]->where)
										{
										case en_eff_where_attack_rate :			// ���� ����
											AddPAttackRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_avoid_rate :			// ���� ȸ��
											AddPAvoidRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_attack_damage :		// ���� ����
											AddPAttackDamageBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_defence :				// ���� ���
											AddDefenceBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_m_attack_rate :		// ��� ����
											AddMAttackRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_m_avoid_rate :		// ��� ȸ��
											AddMAvoidRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_m_attack_damage :		// ��� ����
											AddMAttackDamageBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_m_defence :			// ��� ���
											AddMDefenceBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_max_life :			// �ִ� ����
											AddMaxLifeBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);				
											break;
										case en_eff_where_max_force :			// �ִ� ����
											AddMaxForceBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_max_concentration :	// �ִ� ����
											AddMaxConcentrationBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_move_speed :			// �̵� �ӵ�
											AddMoveSpeedBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_weapon_dam :			// ���� ���ݷ� ����
											AddWeaponDamage(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_weapon_min_dam :		// ���� �ּ� ���ݷ� ����
											AddWeaponMinDamage(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_weapon_max_dam :		// ���� �ִ� ���ݷ� ����
											AddWeaponMaxDamage(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_resist_minus :			// ���� ���׷�
											AddResistMinus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_resist_plus :				// ���� ���׷�
											AddResistPlus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_resist_hon :				// ȥ�� ���׷�
											AddResistHon(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_resist_sin :				// ���� ���׷�
											AddResistSin(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_resist_all :			// ��� ���׷� ����
											AddResistAll(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_add_minus_damage :		// ���� ������ �߰�
											AddAttackDamageMinus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_add_plus_damage :			// ���� ������ �߰�
											AddAttackDamagePlus(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
											AddAttackDamageHon(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										case en_eff_where_add_sin_damage :			// ���� ������ �߰�
											AddAttackDamageSin(g_SocketItemProperty[addBonus].sSkill_Effect_ID2, g_SocketItemProperty[addBonus].sValue2);
											break;
										}
									}
								}
							}
						} // item mix �� ���� �߰��� -----------------------------------------------------------------��
					}
				}				
			}
		}
	}
	
	// clothes
	for( int pos = _XINVENTORY_SLOTTYPE_AMOR; pos <= _XINVENTORY_SLOTTYPE_MASK; ++pos )		
	{
		if(pos == _XINVENTORY_SLOTTYPE_GAP_UPPER || pos == _XINVENTORY_SLOTTYPE_GAP_LOWER)
		{
			// ��/�ϰ� �ڸ����� ����ȭ ����ǰ(�Ǽ��縮) ����
			continue;
		}

		id = g_pLocalUser->m_UserItemList[pos].Get_m_sID();
		type = (_XGI_FirstCategory)g_pLocalUser->m_UserItemList[pos].Get_m_cType();
		
		if(id > 0)
		{
			if( _XGI_FC_CLOTHES == GetEqualItemType(type) )
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(type);					
				if( ClothesItem )
				{					
#ifdef _XTS_FAME
					if( _XGameItem::CheckFame(ClothesItem[id].ucFame) )
#endif
					{
						for(int i = 0 ; i < 5 ; ++i)
						{				
							if(ClothesItem[id].usAdd_SEID[i] > 0)
							{
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(ClothesItem[id].usAdd_SEID[i]);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[ClothesItem[id].usAdd_SEID[i]]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										switch(g_CharacterStateTable[ClothesItem[id].usAdd_SEID[i]]->where)
										{
										case en_eff_where_attack_rate :			// ���� ����
											AddPAttackRateBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_avoid_rate :			// ���� ȸ��
											AddPAvoidRateBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_attack_damage :		// ���� ����
											AddPAttackDamageBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_defence :				// ���� ���
											AddDefenceBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_m_attack_rate :		// ��� ����
											AddMAttackRateBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_m_avoid_rate :		// ��� ȸ��
											AddMAvoidRateBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_m_attack_damage :		// ��� ����
											AddMAttackDamageBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_m_defence :			// ��� ���
											AddMDefenceBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_max_life :			// �ִ� ����
											AddMaxLifeBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);				
											break;
										case en_eff_where_max_force :			// �ִ� ����
											AddMaxForceBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_max_concentration :	// �ִ� ����
											AddMaxConcentrationBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_move_speed :			// �̵� �ӵ�
											AddMoveSpeedBonus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_weapon_dam :			// ���� ���ݷ� ����
											AddWeaponDamage(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_weapon_min_dam :		// ���� �ּ� ���ݷ� ����
											AddWeaponMinDamage(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_weapon_max_dam :		// ���� �ִ� ���ݷ� ����
											AddWeaponMaxDamage(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_resist_minus :			// ���� ���׷�
											AddResistMinus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_resist_plus :				// ���� ���׷�
											AddResistPlus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_resist_hon :				// ȥ�� ���׷�
											AddResistHon(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_resist_sin :				// ���� ���׷�
											AddResistSin(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_resist_all :				// ��� ���׷� ����
											AddResistAll(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_add_minus_damage :		// ���� ������ �߰�
											AddAttackDamageMinus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_add_plus_damage :			// ���� ������ �߰�
											AddAttackDamagePlus(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
											AddAttackDamageHon(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										case en_eff_where_add_sin_damage :			// ���� ������ �߰�
											AddAttackDamageSin(ClothesItem[id].usAdd_SEID[i], ClothesItem[id].usAdd_Value[i]);
											break;
										}
									}
								}
							}
						}
						// ���� Ȯ��
						if(g_pLocalUser->m_UserItemList[pos].Get_m_ucSocketCount() > 0)
						{	// 2
							// #ifdef _XTS_sUser_Inven_v1_all ----------------------------------------------------------------------��
							int socketcount = g_pLocalUser->m_UserItemList[pos].Get_m_ucSocketCount();
							short socketid = 0;
							for(int i = 0 ; i < 4 ; ++i)
							{	// 3
								if(i >= socketcount)
									continue;

								socketid = g_pLocalUser->m_UserItemList[pos].Get_m_sSocket(i);
								
								if(g_SocketItemProperty[socketid].sSkill_Effect_ID3 > 0)
								{
									map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketid].sSkill_Effect_ID3);
									if(iter_table != g_CharacterStateTable.end())
									{
										if(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID3]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
										{
											switch(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID3]->where)
											{
											case en_eff_where_attack_rate :			// ���� ����
												AddPAttackRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_avoid_rate :			// ���� ȸ��
												AddPAvoidRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_attack_damage :		// ���� ����
												AddPAttackDamageBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_defence :				// ���� ���
												AddDefenceBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_m_attack_rate :		// ��� ����
												AddMAttackRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_m_avoid_rate :		// ��� ȸ��
												AddMAvoidRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_m_attack_damage :		// ��� ����
												AddMAttackDamageBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_m_defence :			// ��� ���
												AddMDefenceBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_max_life :			// �ִ� ����
												AddMaxLifeBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);				
												break;
											case en_eff_where_max_force :			// �ִ� ����
												AddMaxForceBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_max_concentration :	// �ִ� ����
												AddMaxConcentrationBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_move_speed :			// �̵� �ӵ�
												AddMoveSpeedBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_weapon_dam :			// ���� ���ݷ� ����
												AddWeaponDamage(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_weapon_min_dam :		// ���� �ּ� ���ݷ� ����
												AddWeaponMinDamage(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_weapon_max_dam :		// ���� �ִ� ���ݷ� ����
												AddWeaponMaxDamage(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_resist_minus :			// ���� ���׷�
												AddResistMinus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_resist_plus :				// ���� ���׷�
												AddResistPlus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_resist_hon :				// ȥ�� ���׷�
												AddResistHon(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_resist_sin :				// ���� ���׷�
												AddResistSin(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_resist_all :				// ��� ���׷� ����
												AddResistAll(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_add_minus_damage :		// ���� ������ �߰�
												AddAttackDamageMinus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_add_plus_damage :			// ���� ������ �߰�
												AddAttackDamagePlus(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
												AddAttackDamageHon(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											case en_eff_where_add_sin_damage :			// ���� ������ �߰�
												AddAttackDamageSin(g_SocketItemProperty[socketid].sSkill_Effect_ID3, g_SocketItemProperty[socketid].sValue3);
												break;
											}
										}
									}
								}
								if(g_SocketItemProperty[socketid].sSkill_Effect_ID4 > 0)
								{
									map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[socketid].sSkill_Effect_ID4);
									if(iter_table != g_CharacterStateTable.end())
									{
										if(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID4]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
										{
											switch(g_CharacterStateTable[g_SocketItemProperty[socketid].sSkill_Effect_ID4]->where)
											{
											case en_eff_where_attack_rate :			// ���� ����
												AddPAttackRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_avoid_rate :			// ���� ȸ��
												AddPAvoidRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_attack_damage :		// ���� ����
												AddPAttackDamageBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_defence :				// ���� ���
												AddDefenceBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_m_attack_rate :		// ��� ����
												AddMAttackRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_m_avoid_rate :		// ��� ȸ��
												AddMAvoidRateBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_m_attack_damage :		// ��� ����
												AddMAttackDamageBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_m_defence :			// ��� ���
												AddMDefenceBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_max_life :			// �ִ� ����
												AddMaxLifeBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);				
												break;
											case en_eff_where_max_force :			// �ִ� ����
												AddMaxForceBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_max_concentration :	// �ִ� ����
												AddMaxConcentrationBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_move_speed :			// �̵� �ӵ�
												AddMoveSpeedBonus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_weapon_dam :			// ���� ���ݷ� ����
												AddWeaponDamage(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_weapon_min_dam :		// ���� �ּ� ���ݷ� ����
												AddWeaponMinDamage(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_weapon_max_dam :		// ���� �ִ� ���ݷ� ����
												AddWeaponMaxDamage(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_resist_minus :			// ���� ���׷�
												AddResistMinus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_resist_plus :				// ���� ���׷�
												AddResistPlus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_resist_hon :				// ȥ�� ���׷�
												AddResistHon(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_resist_sin :				// ���� ���׷�
												AddResistSin(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_resist_all :				// ��� ���׷� ����
												AddResistAll(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_add_minus_damage :		// ���� ������ �߰�
												AddAttackDamageMinus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_add_plus_damage :			// ���� ������ �߰�
												AddAttackDamagePlus(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
												AddAttackDamageHon(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											case en_eff_where_add_sin_damage :			// ���� ������ �߰�
												AddAttackDamageSin(g_SocketItemProperty[socketid].sSkill_Effect_ID4, g_SocketItemProperty[socketid].sValue4);
												break;
											}
										}
									}
								}
							} // for	3
							
							// item mix �� ���� �߰��� -----------------------------------------------------------------��			
							int addBonus = _XGameItem::GetAddEffectFromSocket(  g_pLocalUser->m_UserItemList[pos].Get_m_sSocket(0),
																				g_pLocalUser->m_UserItemList[pos].Get_m_sSocket(1),
																				g_pLocalUser->m_UserItemList[pos].Get_m_sSocket(2),
																				g_pLocalUser->m_UserItemList[pos].Get_m_sSocket(3));
							
							if( addBonus > 0 )
							{	// 3
								if(g_SocketItemProperty[addBonus].sSkill_Effect_ID3 > 0)
								{
									map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[addBonus].sSkill_Effect_ID3);
									if(iter_table != g_CharacterStateTable.end())
									{
										if(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID3]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
										{
											switch(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID3]->where)
											{
											case en_eff_where_attack_rate :			// ���� ����
												AddPAttackRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_avoid_rate :			// ���� ȸ��
												AddPAvoidRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_attack_damage :		// ���� ����
												AddPAttackDamageBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_defence :				// ���� ���
												AddDefenceBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_m_attack_rate :		// ��� ����
												AddMAttackRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_m_avoid_rate :		// ��� ȸ��
												AddMAvoidRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_m_attack_damage :		// ��� ����
												AddMAttackDamageBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_m_defence :			// ��� ���
												AddMDefenceBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_max_life :			// �ִ� ����
												AddMaxLifeBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);				
												break;
											case en_eff_where_max_force :			// �ִ� ����
												AddMaxForceBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_max_concentration :	// �ִ� ����
												AddMaxConcentrationBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_move_speed :			// �̵� �ӵ�
												AddMoveSpeedBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_weapon_dam :			// ���� ���ݷ� ����
												AddWeaponDamage(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_weapon_min_dam :		// ���� �ּ� ���ݷ� ����
												AddWeaponMinDamage(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_weapon_max_dam :		// ���� �ִ� ���ݷ� ����
												AddWeaponMaxDamage(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_resist_minus :			// ���� ���׷�
												AddResistMinus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_resist_plus :				// ���� ���׷�
												AddResistPlus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_resist_hon :				// ȥ�� ���׷�
												AddResistHon(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_resist_sin :				// ���� ���׷�
												AddResistSin(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_resist_all :				// ��� ���׷� ����
												AddResistAll(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_add_minus_damage :		// ���� ������ �߰�
												AddAttackDamageMinus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_add_plus_damage :			// ���� ������ �߰�
												AddAttackDamagePlus(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
												AddAttackDamageHon(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											case en_eff_where_add_sin_damage :			// ���� ������ �߰�
												AddAttackDamageSin(g_SocketItemProperty[addBonus].sSkill_Effect_ID3, g_SocketItemProperty[addBonus].sValue3);
												break;
											}
										}
									}
								}
								if(g_SocketItemProperty[addBonus].sSkill_Effect_ID4 > 0)
								{
									map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[addBonus].sSkill_Effect_ID4);
									if(iter_table != g_CharacterStateTable.end())
									{
										if(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID4]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
										{
											switch(g_CharacterStateTable[g_SocketItemProperty[addBonus].sSkill_Effect_ID4]->where)
											{
											case en_eff_where_attack_rate :			// ���� ����
												AddPAttackRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_avoid_rate :			// ���� ȸ��
												AddPAvoidRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_attack_damage :		// ���� ����
												AddPAttackDamageBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_defence :				// ���� ���
												AddDefenceBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_m_attack_rate :		// ��� ����
												AddMAttackRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_m_avoid_rate :		// ��� ȸ��
												AddMAvoidRateBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_m_attack_damage :		// ��� ����
												AddMAttackDamageBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_m_defence :			// ��� ���
												AddMDefenceBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_max_life :			// �ִ� ����
												AddMaxLifeBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);				
												break;
											case en_eff_where_max_force :			// �ִ� ����
												AddMaxForceBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_max_concentration :	// �ִ� ����
												AddMaxConcentrationBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_move_speed :			// �̵� �ӵ�
												AddMoveSpeedBonus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_weapon_dam :			// ���� ���ݷ� ����
												AddWeaponDamage(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_weapon_min_dam :		// ���� �ּ� ���ݷ� ����
												AddWeaponMinDamage(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_weapon_max_dam :		// ���� �ִ� ���ݷ� ����
												AddWeaponMaxDamage(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_resist_minus :			// ���� ���׷�
												AddResistMinus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_resist_plus :				// ���� ���׷�
												AddResistPlus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_resist_hon :				// ȥ�� ���׷�
												AddResistHon(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_resist_sin :				// ���� ���׷�
												AddResistSin(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_resist_all :				// ��� ���׷� ����
												AddResistAll(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_add_minus_damage :		// ���� ������ �߰�
												AddAttackDamageMinus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_add_plus_damage :			// ���� ������ �߰�
												AddAttackDamagePlus(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
												AddAttackDamageHon(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											case en_eff_where_add_sin_damage :			// ���� ������ �߰�
												AddAttackDamageSin(g_SocketItemProperty[addBonus].sSkill_Effect_ID4, g_SocketItemProperty[addBonus].sValue4);
												break;
											}
										}
									}
								}
							} // 3 item mix �� ���� �߰��� -----------------------------------------------------------------��
							// #ifdef _XTS_sUser_Inven_v1_all ----------------------------------------------------------------------��
						}
					}
				}
			}
		}	// 2					
	}	
	
#ifdef _XDEF_PERIODITEM_20061027
	// ��, �ϰ� ��ġ�� �ִ� �Ⱓ�� ������ ȿ�� ����	
	for( pos = _XINVENTORY_SLOTTYPE_GAP_UPPER; pos <= _XINVENTORY_SLOTTYPE_GAP_LOWER; ++pos )
	{

		id = g_pLocalUser->m_UserItemList[pos].Get_m_sID();
		if(id > 0)
		{
#ifdef _XTS_FAME
			if( _XGameItem::CheckFame(g_AccessoryItemProperty[id].ucFame) 
#ifdef _XTS_ITEM_V22
//				&& _XGameItem::CheckAccessoryReq(g_pLocalUser->m_UserItemList[pos].m_cType, id)
#endif
				)
#endif
			{
				for(int i = 0 ; i < 5 ; ++i)
				{
					if(g_AccessoryItemProperty[id].usAdd_SEID[i] > 0)
					{
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_AccessoryItemProperty[id].usAdd_SEID[i]);
						if(iter_table != g_CharacterStateTable.end())
						{
							if(g_CharacterStateTable[g_AccessoryItemProperty[id].usAdd_SEID[i]]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
							{
								switch(g_CharacterStateTable[g_AccessoryItemProperty[id].usAdd_SEID[i]]->where)
								{
								case en_eff_where_attack_rate :			// ���� ����
									AddPAttackRateBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_avoid_rate :			// ���� ȸ��
									AddPAvoidRateBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_attack_damage :		// ���� ����
									AddPAttackDamageBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_defence :				// ���� ���
									AddDefenceBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_m_attack_rate :		// ��� ����
									AddMAttackRateBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_m_avoid_rate :		// ��� ȸ��
									AddMAvoidRateBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_m_attack_damage :		// ��� ����
									AddMAttackDamageBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_m_defence :			// ��� ���
									AddMDefenceBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_max_life :			// �ִ� ����
									AddMaxLifeBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);				
									break;
								case en_eff_where_max_force :			// �ִ� ����
									AddMaxForceBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_max_concentration :	// �ִ� ����
									AddMaxConcentrationBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_move_speed :			// �̵� �ӵ�
									AddMoveSpeedBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_weapon_dam :			// ���� ���ݷ� ����
									AddWeaponDamage(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_weapon_min_dam :		// ���� �ּ� ���ݷ� ����
									AddWeaponMinDamage(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_weapon_max_dam :		// ���� �ִ� ���ݷ� ����
									AddWeaponMaxDamage(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_resist_minus :			// ���� ���׷�
									AddResistMinus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_resist_plus :				// ���� ���׷�
									AddResistPlus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_resist_hon :				// ȥ�� ���׷�
									AddResistHon(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_resist_sin :				// ���� ���׷�
									AddResistSin(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_resist_all :				// ��� ���׷� ����
									AddResistAll(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_add_minus_damage :		// ���� ������ �߰�
									AddAttackDamageMinus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_add_plus_damage :			// ���� ������ �߰�
									AddAttackDamagePlus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
									AddAttackDamageHon(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_add_sin_damage :			// ���� ������ �߰�
									AddAttackDamageSin(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								}
							}
						}
					}
				}
			}			
		}
	}
#endif
	// �Ǽ��縮
	for( pos = _XINVENTORY_SLOTTYPE_ACCESSORY_1; pos <= _XINVENTORY_SLOTTYPE_ACCESSORY_8; ++pos )
	{
		id = g_pLocalUser->m_UserItemList[pos].Get_m_sID();
		if(id > 0)
		{
			// �Ǽ��縮 ȿ�� ���� üũ - Author : �ڼ���
			int index = pos - 11;
			if(index >= 1 && index <= 4)
			{
				// ���ǿ� �Ǽ��縮
				if(g_pLocalUser->m_cAmorPocketNum < index)
				{
					// �ָӴ� �������� ���� �Ǽ��縮 - ȿ�� �������� ����
					continue;
				}
			}
			else if(index >= 5 && index <= 8)
			{
				// ���ǿ� �Ǽ��縮
				index -= 4;
				if(g_pLocalUser->m_cPantsPocketNum < index)
				{
					// �ָӴ� �������� ���� �Ǽ��縮 - ȿ�� �������� ����
					continue;
				}
			}

#ifdef _XTS_FAME
			if( _XGameItem::CheckFame(g_AccessoryItemProperty[id].ucFame) 
#ifdef _XTS_ITEM_V22
//				&& _XGameItem::CheckAccessoryReq(g_pLocalUser->m_UserItemList[pos].m_cType, id)
#endif
				)
#endif
			{
				for(int i = 0 ; i < 5 ; ++i)
				{
					if(g_AccessoryItemProperty[id].usAdd_SEID[i] > 0)
					{
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_AccessoryItemProperty[id].usAdd_SEID[i]);
						if(iter_table != g_CharacterStateTable.end())
						{
							if(g_CharacterStateTable[g_AccessoryItemProperty[id].usAdd_SEID[i]]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
							{
								switch(g_CharacterStateTable[g_AccessoryItemProperty[id].usAdd_SEID[i]]->where)
								{
								case en_eff_where_attack_rate :			// ���� ����
									AddPAttackRateBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_avoid_rate :			// ���� ȸ��
									AddPAvoidRateBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_attack_damage :		// ���� ����
									AddPAttackDamageBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_defence :				// ���� ���
									AddDefenceBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_m_attack_rate :		// ��� ����
									AddMAttackRateBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_m_avoid_rate :		// ��� ȸ��
									AddMAvoidRateBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_m_attack_damage :		// ��� ����
									AddMAttackDamageBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_m_defence :			// ��� ���
									AddMDefenceBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_max_life :			// �ִ� ����
									AddMaxLifeBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);				
									break;
								case en_eff_where_max_force :			// �ִ� ����
									AddMaxForceBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_max_concentration :	// �ִ� ����
									AddMaxConcentrationBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_move_speed :			// �̵� �ӵ�
									AddMoveSpeedBonus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_weapon_dam :			// ���� ���ݷ� ����
									AddWeaponDamage(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_weapon_min_dam :		// ���� �ּ� ���ݷ� ����
									AddWeaponMinDamage(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_weapon_max_dam :		// ���� �ִ� ���ݷ� ����
									AddWeaponMaxDamage(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_resist_minus :			// ���� ���׷�
									AddResistMinus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_resist_plus :				// ���� ���׷�
									AddResistPlus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_resist_hon :				// ȥ�� ���׷�
									AddResistHon(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_resist_sin :				// ���� ���׷�
									AddResistSin(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_resist_all :			// ��� ���׷� ����
									AddResistAll(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_add_minus_damage :		// ���� ������ �߰�
									AddAttackDamageMinus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_add_plus_damage :			// ���� ������ �߰�
									AddAttackDamagePlus(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
									AddAttackDamageHon(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								case en_eff_where_add_sin_damage :			// ���� ������ �߰�
									AddAttackDamageSin(g_AccessoryItemProperty[id].usAdd_SEID[i], g_AccessoryItemProperty[id].usAdd_Value[i]);
									break;
								}
							}
						}
					}
				}
			}			
		}
	}
}


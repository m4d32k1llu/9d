// XSetItemInfomation.cpp: implementation of the XSetItemInfomation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XMLPaser.h"
#include "XSetItemInfomation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XSetItemInfomation::XSetItemInfomation(void) : m_nAccouteredItem(0), m_nSkillStringCount(0), m_nSetItemStringCount(0), 
											   m_mpData(), m_ltWearItem(), m_ltEffectID(), m_mpSkillInfo()
{
}

XSetItemInfomation::~XSetItemInfomation(void)
{
	map<int, SetInfo*>::iterator iter;

	if( m_mpData.size() > 0 )
	{
		for( iter = m_mpData.begin(); iter != m_mpData.end(); ++iter )
		{
			SetInfo* pSetInfo = iter->second;

			if( pSetInfo )
			{
                delete pSetInfo;
				pSetInfo = NULL;
			}
		}
		m_mpData.clear();
	}

	m_ltEffectID.clear();
	m_mpSkillInfo.clear();
	m_ltWearItem.clear();
}

HRESULT XSetItemInfomation::LoadXMLTable( XMLPaser& XMLPaser, LPTSTR pTableName )
{
	if( !XMLPaser.ND_isThereTable( pTableName ) )
	{
		return S_FALSE;
	}
	
	int TableRowCount = XMLPaser.ND_nGetTableRowCount( pTableName );

	if(TableRowCount <=0)	
		return S_FALSE;

    TableRowCount--;


	//���� ó�� �ؾ��ϴµ�.. ������..
	map<int, SetInfo*>::iterator infoiter;
	HRESULT hr;
    int nGroup = 0;
	m_ltEffectID.clear();

	for( int i = 0; i < TableRowCount; i++ )
	{
		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Group", i, &nGroup ) ))
			return (hr);

		//������ ����
		ItemInfo* pItemInfo = new ItemInfo;

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Item Type", i, &pItemInfo->nType) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Item ID", i, &pItemInfo->nID) ))
			return (hr);

		//������ ���� ����
		ItemDetailInfo* pDetailInfo = new ItemDetailInfo;
		SkillInfo* pSkillInfo_1 = new SkillInfo;
		SkillInfo* pSkillInfo_2 = new SkillInfo;
		SkillInfo* pSkillInfo_3 = new SkillInfo;

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "In Item", i, &pDetailInfo->nAccouteredItem ) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Effect ID", i, &pDetailInfo->nEffectID ) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Skill ID_1", i, &pSkillInfo_1->nSkillID) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Skill Value_1", i, &pSkillInfo_1->nSkillValue) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Skill ID_2", i, &pSkillInfo_2->nSkillID) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Skill Value_2", i, &pSkillInfo_2->nSkillValue) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Skill ID_3", i, &pSkillInfo_3->nSkillID) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Skill Value_3", i, &pSkillInfo_3->nSkillValue) ))
			return (hr);

		if( pDetailInfo->nEffectID )
			m_ltEffectID.push_back( pDetailInfo->nEffectID );

		pDetailInfo->ltSkillInfo.push_back( pSkillInfo_1 );
		pDetailInfo->ltSkillInfo.push_back( pSkillInfo_2 );
		pDetailInfo->ltSkillInfo.push_back( pSkillInfo_3 );

		infoiter = m_mpData.find( nGroup ); 

		//���� Ű�� ���� // ���׸� ��� �� �� ������ ��ũ��Ʈ�� ���� ��Ʋ���� �ٶ��
		if( infoiter != m_mpData.end() )
		{
			SetInfo* pSetInfo = infoiter->second;

			//������ ���� ����
			pSetInfo->nGroupID = nGroup;
			pSetInfo->vtItem.push_back( pItemInfo );
			pSetInfo->ltItemInfo.push_back( pDetailInfo );
		}
		else
		{
			SetInfo* pSetInfo = new SetInfo;

			pSetInfo->nGroupID = nGroup;
			pSetInfo->vtItem.push_back( pItemInfo );
			pSetInfo->ltItemInfo.push_back( pDetailInfo );

			m_mpData[nGroup] = pSetInfo;
		}
	}

	return S_OK;
}

int XSetItemInfomation::SearchGroupItem(int nGroup)
{
	map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return false;

	SetInfo* pSetInfo = iter->second;

	vector<ItemInfo*>::iterator vtiter = pSetInfo->vtItem.begin(); //������ ���� ��������
	m_nAccouteredItem = 0; //��� �԰��ִ���
	m_ltWearItem.clear();

	for(; vtiter != pSetInfo->vtItem.end(); ++vtiter)
	{
		for( int i = 0; i < _XINVENTORY_SLOTTYPE_POCKETSTART; i++)
		{
			if( i == 11 ) // i �� 11���϶�.. W ���� �ؿ� ���� ��.. �̶� �԰��ִ� �ɷ� ���� ���Ѵ�
				continue;

			if( g_pLocalUser->m_UserItemList[i].Get_m_cType() == (*vtiter)->nType &&
				g_pLocalUser->m_UserItemList[i].Get_m_sID() == (*vtiter)->nID ) // �̰� ������ ���� ��
			{
				ItemInfo info;
				info.nType = (*vtiter)->nType;
				info.nID = (*vtiter)->nID;
				m_ltWearItem.push_back( info ); //���� �������� ������ ����
				m_nAccouteredItem++;
			}
		}
	}

	return m_nAccouteredItem;
}

int XSetItemInfomation::SetGroupSkillString( int nGroup, int nAccouter, SIZE TooltipSize, _ITEMTOOLTIP_INFO& TooltipInfo )
{
	map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return 0;

	SetInfo* pSetInfo = iter->second;

	//iter �� �������� ����Ѵ�.. ��ų�� ���� ������ �Ǿ��ϱ⶧����..
	//��,, ���� ���� �����ϴ�..
	list<ItemDetailInfo*>::iterator Detailiter = pSetInfo->ltItemInfo.begin(); //�����ۼ�������..
	list<SkillInfo*>::iterator Skilliter; //�����������ȿ� ��ų����

	m_nSkillStringCount = 0;

	for(int i = 0; Detailiter != pSetInfo->ltItemInfo.end(); ++Detailiter)
	{
        //08�� 6�� 11�� ��ų�� ���δ� ������ �ٲ����..
		//if( (*Detailiter)->nAccouteredItem > nAccouter ) //������ ������ �ʰ��Ǹ� �����
		//	break;

		for( Skilliter = (*Detailiter)->ltSkillInfo.begin(); Skilliter != (*Detailiter)->ltSkillInfo.end(); ++Skilliter )
		{
			if((*Skilliter)->nSkillID > 0 ) // ��ų ���̵� 0 �� �ƴϸ�..
			{
				_snprintf( TooltipInfo.SetItemSkill_Data_Buf[i], sizeof(TooltipInfo.SetItemSkill_Data_Buf[i]),
					g_CharacterStateTable[(*Skilliter)->nSkillID]->concept, (*Skilliter)->nSkillValue);

				int length = g_XBaseFont->GetRowCount( TooltipInfo.SetItemSkill_Data_Buf[i], TooltipSize.cx-16 );
				i++;
				m_nSkillStringCount += length; //�����϶� length�� 1 �����ϋ��� 2
			}
		}
	}

	return m_nSkillStringCount;
}

int XSetItemInfomation::SetGroupItemString( int nGroup, _ITEMTOOLTIP_INFO& TooltipInfo )
{
	map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return 0;

	SetInfo* pSetInfo = iter->second;

	vector<ItemInfo*>::iterator vtiter = pSetInfo->vtItem.begin(); //������ ���� ��������

	m_nSetItemStringCount = 0; //��Ʈ�� ��ġ üũ

	for(; vtiter != pSetInfo->vtItem.end(); ++vtiter)
	{
		switch( (*vtiter)->nType ) //Ÿ�Կ� ���� �̸��� �ҷ����� ����� Ʋ��..
		{
		case _XGI_FC_WEAPON:
		case _XGI_FC_WEAPON2:
		case _XGI_FC_WEAPON3:
			{
				_XGI_WeaponItem_Property* tempWeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty((*vtiter)->nType);
				if( _XGameItem::GetItemNameAddtoString((*vtiter)->nType, (*vtiter)->nID) ) //  �̰� �Ƹ� _XTS_ITEM_V20 �̻��̸� �̷��� �Ǵµ�
				{
					_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]),
						"%s %s", _XGameItem::GetItemNameAddtoString((*vtiter)->nType,(*vtiter)->nID), tempWeaponItem[(*vtiter)->nID].cName );
				}
				else
				{
					if((*vtiter)->nType == _XGI_FC_WEAPON2)
					{
						_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]), 
						"%s", g_WeaponItemProperty2[(*vtiter)->nID].cName );
					}
					else if((*vtiter)->nType == _XGI_FC_WEAPON3)
					{
						_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]),
						"%s", g_WeaponItemProperty3[(*vtiter)->nID].cName );
					}
					else
					{
						_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]), 
							"%s", tempWeaponItem[(*vtiter)->nID].cName );
					}
				}
				
				m_nSetItemStringCount++;
			}
			break;
		case _XGI_FC_CLOTHES:
		case _XGI_FC_CLOTHES2:
		case _XGI_FC_CLOTHES3:
			{
				_XGI_ClothesItem_Property* tempClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty((*vtiter)->nType);
				if( _XGameItem::GetItemNameAddtoString((*vtiter)->nType, (*vtiter)->nID) )
				{
					_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]), 
						"%s %s", _XGameItem::GetItemNameAddtoString((*vtiter)->nType,(*vtiter)->nID), tempClothesItem[(*vtiter)->nID].cName);
				}
				else
				{
					if((*vtiter)->nType == _XGI_FC_CLOTHES2)
					{
						_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]), 
							"%s", g_ClothesItemProperty2[(*vtiter)->nID].cName  );
					}
					else if((*vtiter)->nType == _XGI_FC_CLOTHES3)
					{
						_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]),
							"%s", g_ClothesItemProperty3[(*vtiter)->nID].cName  );
					}
					else
					{
						_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]),
							"%s", tempClothesItem[(*vtiter)->nID].cName );
					}
				}		
				m_nSetItemStringCount++;
			}
			break;
		case _XGI_FC_ACCESSORY :
			{
				_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]), 
					"%s", g_AccessoryItemProperty[(*vtiter)->nID].cName );
				m_nSetItemStringCount++;
			}
			break;
		default :
			{
				//���� ���� �ȵ�..
			}
			break;
		}
	}

	return m_nSetItemStringCount;
}

void XSetItemInfomation::DrawToolTip( int nGroup, int DrawX, int DrawY, _ITEMTOOLTIP_INFO& TooltipInfo )
{
	map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return;

	SetInfo* pSetInfo = iter->second;

	vector<ItemInfo*>::iterator vtiter = pSetInfo->vtItem.begin(); //������ ���� ��������
	list<ItemInfo>::iterator ltiter;  //���� �԰��ִ� �� ����
	
	//���� iter -_-��������
	for(int i = 0; vtiter != pSetInfo->vtItem.end(); i++, ++vtiter)
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		for(ltiter = m_ltWearItem.begin(); ltiter != m_ltWearItem.end(); ++ltiter )
		{
			if( (*vtiter)->nType == (*ltiter).nType &&
				(*vtiter)->nID == (*ltiter).nID )
			{
				//������ ���� ���� ���ְ�.. �ҽ� ��´�..
				g_XBaseFont->SetColor(0xFFFFD901);	
				break;
			}
		}

		//������ �̸� ���ʴ�� �׸��°�..
		g_XBaseFont->PutsAlign( DrawX, DrawY, _XFONT_ALIGNTYPE_LEFT, TooltipInfo.SetItem_Data_Buf[i] );
		DrawY += 16;

		g_XBaseFont->Flush();
	}
}

void XSetItemInfomation::DrawSkillToolTip( int nGroup, int DrawX, int DrawY, SIZE TooltipSize, int &nCurrentDrawYModify, _ITEMTOOLTIP_INFO& TooltipInfo )
{
    map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return;

	SetInfo* pSetInfo = iter->second;

	//iter �� �������� ����Ѵ�.. ��ų�� ���� ������ �Ǿ��ϱ⶧����..
	//��,, ���� ���� �����ϴ�..
	list<ItemDetailInfo*>::iterator Detailiter = pSetInfo->ltItemInfo.begin(); //�����ۼ�������..
	list<SkillInfo*>::iterator Skilliter; //�����������ȿ� ��ų����

	for(int i = 0; Detailiter != pSetInfo->ltItemInfo.end(); ++Detailiter) 
	{
		for( Skilliter = (*Detailiter)->ltSkillInfo.begin(); Skilliter != (*Detailiter)->ltSkillInfo.end(); ++Skilliter )
		{
			if((*Skilliter)->nSkillID > 0 ) // ��ų ���̵� 0 �� �ƴϸ�..
			{
                if( (*Detailiter)->nAccouteredItem > m_nAccouteredItem ) //������ ������ �ʰ�
                    g_XBaseFont->SetColor(_XSC_DEFAULT);
                else
                    g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,3,253,62));

				//�ٹٲ��� �Ҽ� �ְ� �ٲ���Ѵ�. 08/09/04
				g_XBaseFont->Puts_Separate( DrawX, DrawY, TooltipInfo.SetItemSkill_Data_Buf[i], TooltipSize.cx - 16, 1.0f, 3 );
				int templength = g_XBaseFont->GetRowCount( TooltipInfo.SetItemSkill_Data_Buf[i], TooltipSize.cx - 16 );
         
                i++;
                DrawY = DrawY + (16 * templength);
                nCurrentDrawYModify = nCurrentDrawYModify + (16 * templength); //�̰�,,, ����Ʈ ������  m_CurrentDrawPosY �̰ǵ�,, ��ų �ؿ��� �ٸ� ��Ʈ���� �־ ������ ����Ѵ�
			}
		}
	}
}

bool XSetItemInfomation::GetWearItem( int nGroup, int nType, int nID )
{
	map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return false;

	SetInfo* pSetInfo = iter->second;

	vector<ItemInfo*>::iterator vtiter = pSetInfo->vtItem.begin(); //������ ���� ��������

	for(; vtiter != pSetInfo->vtItem.end(); ++vtiter)
	{
		if( ((*vtiter)->nType == nType) && ((*vtiter)->nID == nID) ) //Ÿ���̶� ���̵� ����
		{
			return true;
		}
	}

	return false;
}

void XSetItemInfomation::GroupItemSkillUpdate()
{
	map<int, int>::iterator iter;
	list<int>::iterator ltiter;
	m_mpSkillInfo.clear();

	for( int i = 0; i < _XINVENTORY_SLOTTYPE_POCKETSTART; i++)
	{
		if( i == 11 ) // i �� 11���϶�.. W ���� �ؿ� ���� ��.. �̶� �԰��ִ� �ɷ� ���� ���Ѵ�
			continue;

		int nGroup = _XGameItem::GetSetItem(g_pLocalUser->m_UserItemList[i].Get_m_cType(), g_pLocalUser->m_UserItemList[i].Get_m_sID());
		if( nGroup == 0 )
			continue; 

		iter = m_mpSkillInfo.find( nGroup ); //�ش� �׷��� �ִ���

		if( iter != m_mpSkillInfo.end() ) // �ش� �׷� �߰�
		{
			int nCount = iter->second; //�ش�׷��� ���� ���� ������ �޾ƿ���
			nCount++;
			m_mpSkillInfo[nGroup] = nCount; // ������ ���� �����ش�..
		}
		else
		{
			m_mpSkillInfo[nGroup] = 1; // X �׷��� ù��°�� �߰�..
		}
	} //���� ��..

	//��ü ����Ʈ ����
	for(ltiter = m_ltEffectID.begin(); ltiter != m_ltEffectID.end(); ++ltiter )
	{
		int nEffectID = (*ltiter);
		if( g_pLocalUser->m_ModelDescriptor.m_EffectManager )
		{
			if( g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance(nEffectID) )
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(nEffectID);
		}
	}

	for(iter = m_mpSkillInfo.begin(); iter != m_mpSkillInfo.end(); ++iter)
	{
		int nSetGroup = iter->first; // ���° �׷��ΰ�..
		int nAccoutered = iter->second; //�׷��� ���� � �԰� �ֳ�..

		UpdateGroupSkillInfo( nSetGroup, nAccoutered );
	}

	m_mpSkillInfo.clear();
}

void XSetItemInfomation::UpdateGroupSkillInfo(int nGroup, int nAccouter )
{
	map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return;

	SetInfo* pSetInfo = iter->second;

	//iter �� �������� ����Ѵ�.. ��ų�� ���� ������ �Ǿ��ϱ⶧����..
	//��,, ���� ���� �����ϴ�..
	list<ItemDetailInfo*>::iterator Detailiter = pSetInfo->ltItemInfo.begin(); //����������..
	list<SkillInfo*>::iterator Skilliter;  //�����������ȿ� ��ų����

	for(; Detailiter != pSetInfo->ltItemInfo.end(); ++Detailiter)
	{
		if( (*Detailiter)->nAccouteredItem > nAccouter ) //������ ������ �ʰ��Ǹ� �����
			break;

		if( (*Detailiter)->nEffectID > 0 ) // ����Ʈ ���
		{
			int nEffectID = (*Detailiter)->nEffectID;
			if( !g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance(nEffectID) )
			{
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
																					nEffectID, &g_pLocalUser->m_ModelDescriptor.m_Position, NULL, TRUE );
			}			
		}

		for(Skilliter = (*Detailiter)->ltSkillInfo.begin(); Skilliter != (*Detailiter)->ltSkillInfo.end(); ++Skilliter )
		{
			if((*Skilliter)->nSkillID <= 0 ) // ��ų ���̵� 0 �� �ƴϸ�..
				continue;

			int nID = (*Skilliter)->nSkillID;
			int nValue = (*Skilliter)->nSkillValue;
			
			//��ų ������Ʈ..
			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(nID);
			if(iter_table == g_CharacterStateTable.end())
				continue;
			
			if(!(g_CharacterStateTable[nID]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
			     g_CharacterStateTable[nID]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE))
				continue;

			switch(g_CharacterStateTable[nID]->where)
			{
			case en_eff_where_strength :		// �ٷ�
				{
					g_pLocalUser->m_StateList.AddStrengthBonus(nID, nValue);
				}
				break;
			case en_eff_where_zen :				// ����
				{
					g_pLocalUser->m_StateList.AddZenBonus(nID, nValue);
				}
				break;
			case en_eff_where_intelligence :	// ����
				{
					g_pLocalUser->m_StateList.AddIntBonus(nID, nValue);
				}
				break;
			case en_eff_where_constitution :	// �ǰ�
				{
					g_pLocalUser->m_StateList.AddConstitutionBonus(nID, nValue);
				}
				break;
			case en_eff_where_dexterity :		// ��ø
				{
					g_pLocalUser->m_StateList.AddDexBonus(nID, nValue);
				}
				break;
			case en_eff_where_all_stat :
				{
					g_pLocalUser->m_StateList.AddStrengthBonus(nID, nValue);
					g_pLocalUser->m_StateList.AddZenBonus(nID, nValue);
					g_pLocalUser->m_StateList.AddIntBonus(nID, nValue);
					g_pLocalUser->m_StateList.AddConstitutionBonus(nID, nValue);
					g_pLocalUser->m_StateList.AddDexBonus(nID, nValue);
				}
				break;
			case en_eff_where_attack_rate :			// ���� ����
				g_pLocalUser->m_StateList.AddPAttackRateBonus(nID, nValue);
				break;
			case en_eff_where_avoid_rate :			// ���� ȸ��
				g_pLocalUser->m_StateList.AddPAvoidRateBonus(nID, nValue);
				break;
			case en_eff_where_attack_damage :		// ���� ����
				g_pLocalUser->m_StateList.AddPAttackDamageBonus(nID, nValue);
				break;
			case en_eff_where_defence :				// ���� ���
				g_pLocalUser->m_StateList.AddDefenceBonus(nID, nValue);
				break;
			case en_eff_where_m_attack_rate :		// ��� ����
				g_pLocalUser->m_StateList.AddMAttackRateBonus(nID, nValue);
				break;
			case en_eff_where_m_avoid_rate :		// ��� ȸ��
				g_pLocalUser->m_StateList.AddMAvoidRateBonus(nID, nValue);
				break;
			case en_eff_where_m_attack_damage :		// ��� ����
				g_pLocalUser->m_StateList.AddMAttackDamageBonus(nID, nValue);
				break;
			case en_eff_where_m_defence :			// ��� ���
				g_pLocalUser->m_StateList.AddMDefenceBonus(nID, nValue);
				break;
			case en_eff_where_max_life :			// �ִ� ����
				g_pLocalUser->m_StateList.AddMaxLifeBonus(nID, nValue);				
				break;
			case en_eff_where_max_force :			// �ִ� ����
				g_pLocalUser->m_StateList.AddMaxForceBonus(nID, nValue);
				break;
			case en_eff_where_max_concentration :	// �ִ� ����
				g_pLocalUser->m_StateList.AddMaxConcentrationBonus(nID, nValue);
				break;
			case en_eff_where_move_speed :			// �̵� �ӵ�
				g_pLocalUser->m_StateList.AddMoveSpeedBonus(nID, nValue);
				break;
			case en_eff_where_weapon_dam :			// ���� ���ݷ� ����
				g_pLocalUser->m_StateList.AddWeaponDamage(nID, nValue);
				break;
			case en_eff_where_weapon_min_dam :		// ���� �ּ� ���ݷ� ����
				g_pLocalUser->m_StateList.AddWeaponMinDamage(nID, nValue);
				break;
			case en_eff_where_weapon_max_dam :		// ���� �ִ� ���ݷ� ����
				g_pLocalUser->m_StateList.AddWeaponMaxDamage(nID, nValue);
				break;
			case en_eff_where_resist_minus :		// ���� ���׷�
				g_pLocalUser->m_StateList.AddResistMinus(nID, nValue);
				break;
			case en_eff_where_resist_plus :			// ���� ���׷�
				g_pLocalUser->m_StateList.AddResistPlus(nID, nValue);
				break;
			case en_eff_where_resist_hon :			// ȥ�� ���׷�
				g_pLocalUser->m_StateList.AddResistHon(nID, nValue);
				break;
			case en_eff_where_resist_sin :			// ���� ���׷�
				g_pLocalUser->m_StateList.AddResistSin(nID, nValue);
				break;
			case en_eff_where_resist_all :			// ��� ���׷� ����
				g_pLocalUser->m_StateList.AddResistAll(nID, nValue);
				break;
			case en_eff_where_add_minus_damage :		// ���� ������ �߰�
				g_pLocalUser->m_StateList.AddAttackDamageMinus(nID, nValue);
				break;
			case en_eff_where_add_plus_damage :			// ���� ������ �߰�
				g_pLocalUser->m_StateList.AddAttackDamagePlus(nID, nValue);
				break;
			case en_eff_where_add_hon_damage :			// ȥ�� ������ �߰�
				g_pLocalUser->m_StateList.AddAttackDamageHon(nID, nValue);
				break;
			case en_eff_where_add_sin_damage :			// ���� ������ �߰�
				g_pLocalUser->m_StateList.AddAttackDamageSin(nID, nValue);
				break;
			}
		}
	}
	//��....
}

void XSetItemInfomation::UpdateAnotherUserEffect( _XUser* pUser, int nEffectID, int nEffectID1 )
{
	list<int>::iterator ltiter;

	//��ü ����Ʈ ����
	for(ltiter = m_ltEffectID.begin(); ltiter != m_ltEffectID.end(); ++ltiter )
	{
		int nTempEffectID = (*ltiter);
		if( pUser->m_ModelDescriptor.m_EffectManager )
		{
			if( pUser->m_ModelDescriptor.m_EffectManager->FindInstance(nTempEffectID) )
				pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(nTempEffectID);
		}
	}

	if( nEffectID > 0 )
	{
		if( !pUser->m_ModelDescriptor.m_EffectManager->FindInstance(nEffectID) )
			pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
																		nEffectID, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE );
	}
	if( nEffectID1 > 1 )
	{
		if( !pUser->m_ModelDescriptor.m_EffectManager->FindInstance(nEffectID1) )
			pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
																		nEffectID1, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE );
	}
}
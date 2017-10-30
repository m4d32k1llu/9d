// XWindow_ItemTooltip.cpp: implementation of the _XWindow_ItemTooltip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_ItemTooltip.h"
#include "XWindow_NPCTrade.h" // 2004.06.16->oneway48 insert
#include "XWindow_RepairItem.h" // 2004.06.29->oneway48 insert
#include "XSR_STRINGHEADER.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_ItemTooltip::_XWindow_ItemTooltip()
{
	m_ParentWindow = NULL;
	m_HoveredSlotIndex = -1;
	
	m_Category = -1;
	m_ID = -1;
}

_XWindow_ItemTooltip::~_XWindow_ItemTooltip()
{

}


BOOL _XWindow_ItemTooltip::Initialize(void)
{
	m_ParentWindow	= NULL;
	m_HoveredSlotIndex = -1;
	SetWindowMoveMode(_XWMOVE_FIXED);
		
	memset( m_DescClassString, 0, sizeof(TCHAR)*16 );// �迭 -> ���� �з�
	strncpy(m_DescClassString, _T("����з�"), 15);
	
	memset( m_DescPowerString, 0, sizeof(TCHAR)*16 );// ���ݷ�
	strncpy(m_DescPowerString, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_ATTACKDAMAGE), 15);
	
	memset( m_DescDurabilityString, 0, sizeof(TCHAR)*16 );// ������
	strncpy(m_DescDurabilityString, _T("������"), 15);
	
	memset( m_DescStrengthString, 0, sizeof(TCHAR)*16 );// ����
	strncpy(m_DescStrengthString, _T("����"), 15);
	
	memset( m_DescBalanceString, 0, sizeof(TCHAR)*16 );// �ϼ��� 2004.06.14->oneway48 modify
	strncpy(m_DescBalanceString, _T("�ϼ���"), 15);
	
	memset( m_DescWeightString, 0, sizeof(TCHAR)*16 );// ����
	strncpy(m_DescWeightString, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_WEIGHT), 15);

	// 2004.06.11->oneway48 insert 
	memset( m_DescGenderString, 0, sizeof(TCHAR)*16 );// ����
	strncpy(m_DescGenderString, _T("����"), 15);
	
	// 2004.06.14->oneway48 insert 
	memset( m_DescPrice, 0, sizeof(TCHAR)*16 );// ����
	strncpy(m_DescPrice, _T("����"), 15);

	// 2004.05.12->oneway48 insert
	memset( m_DescDefenseString, 0, sizeof(TCHAR)*16 );// ����
	strncpy(m_DescDefenseString, _T("����"), 15);
	
	memset( m_DescWarmthString, 0, sizeof(TCHAR)*16 );// ����ȿ��
	strncpy(m_DescWarmthString, _T("����ȿ��"), 15);
	
	memset( m_DescResistanceString, 0, sizeof(TCHAR)*16 );// �Ӽ� ���׷�
	strncpy(m_DescResistanceString, _T("�Ӽ� ���׷�"), 15);
	
	memset( m_DescRollString, 0, sizeof(TCHAR)*16 );// ����
	strncpy(m_DescRollString, _T("����"), 15);
	
	memset( m_DescEffectString, 0, sizeof(TCHAR)*16 );// ȿ��
	strncpy(m_DescEffectString, _T("ȿ��"), 15);
	
	memset( m_DescFeatureString, 0, sizeof(TCHAR)*16 );// Ư¡
	strncpy(m_DescFeatureString, _T("Ư¡"), 15);
	// insert end

	memset( m_DescHeartString, 0, sizeof(TCHAR)*16 );// �ɱ�
	strncpy(m_DescHeartString, _T("����"), 15);
	
	memset( m_DescStaminaString, 0, sizeof(TCHAR)*16 );// ���� 
	strncpy(m_DescStaminaString, _T("�ٷ�"), 15);
	
	memset( m_DescLevelString, 0, sizeof(TCHAR)*16 );// ��������
	strncpy(m_DescLevelString, _T("���� �ܰ�"), 15); // 2004.06.18->oneway48 insert
	
	memset( m_DescRefine1String, 0, sizeof(TCHAR)*16 );// ���� �Ӽ� 1
	strncpy(m_DescRefine1String, _T("���� �Ӽ� 1"), 15);
	
	memset( m_DescRefine2String, 0, sizeof(TCHAR)*16 );// ���� �Ӽ� 2
	strncpy(m_DescRefine2String, _T("���� �Ӽ� 2"), 15);
	
	memset( m_DescAdditionalEffect1String, 0, sizeof(TCHAR)*16 );// �ΰ� ȿ�� 1
	strncpy(m_DescAdditionalEffect1String, _T("�ΰ� ȿ�� 1"), 15);
	
	memset( m_DescAdditionalEffect2String, 0, sizeof(TCHAR)*16 );// �ΰ� ȿ�� 2
	strncpy(m_DescAdditionalEffect2String, _T("�ΰ� ȿ�� 2"), 15);
	
	memset( m_DescAdditionalEffect3String, 0, sizeof(TCHAR)*16 );// �ΰ� ȿ�� 3
	strncpy(m_DescAdditionalEffect3String, _T("�ΰ� ȿ�� 3"), 15);
	
	memset( m_DescSetEffect1String, 0, sizeof(TCHAR)*16 );// ��Ʈ ȿ�� 1 
	strncpy(m_DescSetEffect1String, _T("��Ʈ ȿ�� 1"), 15);
	
	memset( m_DescSetEffect2String, 0, sizeof(TCHAR)*16 );// ��Ʈ ȿ�� 2
	strncpy(m_DescSetEffect2String, _T("��Ʈ ȿ�� 2"), 15);
	
	memset( m_DescSetEffect3String, 0, sizeof(TCHAR)*16 );// ��Ʈ ȿ�� 3
	strncpy(m_DescSetEffect3String, _T("��Ʈ ȿ�� 3"), 15);
	
	memset( m_DescSetEffect4String, 0, sizeof(TCHAR)*16 );// ��Ʈ ȿ�� 4
	strncpy(m_DescSetEffect4String, _T("��Ʈ ȿ�� 4"), 15);
	
	memset( m_DescSetEffect5String, 0, sizeof(TCHAR)*16 );// ��Ʈ ȿ�� 5
	strncpy(m_DescSetEffect5String, _T("��Ʈ ȿ�� 5"), 15);
	
	return TRUE;
}

void _XWindow_ItemTooltip::DestroyWindow(void)
{

	_XWindow::DestroyWindow();
}

void _XWindow_ItemTooltip::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;

	// 2004.06.16->oneway48 insert
	_XWindow_NPCTrade* pNPCTrade_Window = (_XWindow_NPCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADE);
	_XWindow_RepairItem* pRepairItem_Window = (_XWindow_RepairItem*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REPAIRITEM ); // 2004.06.29->oneway48 insert

	// 2004.05.12->oneway48 insert
	_XDrawSolidBar( m_WindowPosition.x,  m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy , D3DCOLOR_ARGB(127,0,0,0) );
	
	// Border edge...
	_XDrawRectAngle(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, _XSC_DEFAULT_BLACK);
	_XDrawRectAngle(m_WindowPosition.x+1, m_WindowPosition.y+1, m_WindowPosition.x+m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f, D3DCOLOR_ARGB(255,96,96,96));
	_XDrawRectAngle(m_WindowPosition.x+2, m_WindowPosition.y+2, m_WindowPosition.x+m_WindowSize.cx-3, m_WindowPosition.y+m_WindowSize.cy-3, 0.0f, _XSC_DEFAULT_BLACK);
	
	// 2004.05.12->oneawy48 modify
	// ��ǰ �̸� ���� �ڽ� 
	_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+6, m_WindowPosition.x+194, m_WindowPosition.y+22 , D3DCOLOR_ARGB(127,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+5, m_WindowPosition.x+194, m_WindowPosition.y+22 , 0.0f, _XSC_DEFAULT );
	
	// ��ǰ ���� ���� �ڽ�
	_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+25, m_WindowPosition.x+194, m_WindowPosition.y+56 , D3DCOLOR_ARGB(100,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+24, m_WindowPosition.x+194, m_WindowPosition.y+56 , 0.0f, _XSC_DEFAULT_BLACK );
		
	if(m_HoveredSlotIndex > -1 || (m_Category > -1 && m_ID > -1) )
	{				
		if( m_HoveredSlotIndex > -1 )
		{
			m_Category	= g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_cType;
			m_ID		= g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_sID;
		}

		switch(m_Category)
		{
		case _XGI_FC_WEAPON :			
			{
				// ��ǰ �⺻ ���� �ڽ�
				_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+59, m_WindowPosition.x+194, m_WindowPosition.y+150 , D3DCOLOR_ARGB(100,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+58, m_WindowPosition.x+194, m_WindowPosition.y+150 , 0.0f, _XSC_DEFAULT_BLACK );
				
				// ��ǰ �䱸 ���� �ڽ�
				_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+153, m_WindowPosition.x+194, m_WindowPosition.y+199 , D3DCOLOR_ARGB(100,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+152, m_WindowPosition.x+194, m_WindowPosition.y+199 , 0.0f, _XSC_DEFAULT_BLACK );
			

				// ��ǰ �̸� ����
				g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_WeaponItemProperty[m_ID].cName);
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_WeaponItemProperty[m_ID].cName);

				
				// ��ǰ ���� ���� : 2004.06.16->oneway48 insert
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				
				g_XBaseFont->Puts_Separate( m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_WeaponItemProperty[m_ID].cItemInfo, 180 );

				// �⺻ ����
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 61, m_DescClassString );// �迭 -> ���� �з�
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 76, m_DescPowerString );// ���ݷ�
				//g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 91, m_DescDurabilityString );// ������
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 91, _T("����") );// ������
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 106, m_DescStrengthString );// ����
				//g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 121, m_DescBalanceString );// ����
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 121, _T("�ϼ���") );// ����
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 136, m_DescWeightString );// ����
				
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 155, m_DescHeartString );// �ɱ�
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 170, m_DescStaminaString );// ���� 
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 185, m_DescLevelString );// ��������

				TCHAR	classstring[16];			
				memset(classstring, 0, sizeof(TCHAR) * 16);
				switch(g_WeaponItemProperty[ m_ID ].cSecondType) 
				{			
				case _XGI_SC_WEAPON_SHORT :
					{
						switch(g_WeaponItemProperty[m_ID].cThirdType)
						{
						case _XGI_TC_WEAPON_SWORD		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_SWORD));	break;
						case _XGI_TC_WEAPON_KNIFE		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_KNIFE));	break;			
						case _XGI_TC_WEAPON_CLUB		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_CLUB));	break;		
						case _XGI_TC_WEAPON_HIT			 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_HIT));	break;
						case _XGI_TC_WEAPON_FIST		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_FIST));	break;
						case _XGI_TC_WEAPON_AXE			 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_AXE));	break;
						case _XGI_TC_WEAPON_SECRET		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_SECRETWEAPON)); break;			
						case _XGI_TC_WEAPON_RING		 : strcpy(classstring, "��"); break;
						}
					}				
					break;
				case _XGI_SC_WEAPON_LONG :
					{
						switch(g_WeaponItemProperty[m_ID].cThirdType)
						{
						case _XGI_TC_WEAPON_GLAIVE		: strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_GLAIVE));  break;					
						case _XGI_TC_WEAPON_SPEAR		: strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_SPEAR));	break;					
						case _XGI_TC_WEAPON_STAFF		: strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_STAFF));	break;					
						}
					}
					break;
				}
				
				g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, classstring); //���� �з�
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76, 1.0f, "%d - %d", //���ݷ� 
					g_WeaponItemProperty[m_ID].sMinPower, 
					g_WeaponItemProperty[m_ID].sMaxPower);			
				
				// 2004.06.19->oneway48 close3 
				FLOAT percent = 0;			
				FLOAT maxdur = g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur;	
//				FLOAT maxdur = 0;
//				maxdur = g_WeaponItemProperty[g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_sID].usDurability;

				
				if( g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur != 0 )
					percent = (FLOAT)g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usCurDur / maxdur * 100.0f; // 2004.06.14->oneway48 insert

				if( percent >= 80.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("����")); 
				}
				else if( percent < 80 && percent >= 60.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("���")); 
				}
				else if( percent < 60 && percent >= 40.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("��ȣ")); 
				}
				else if( percent < 40 && percent >= 20.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("�̻�")); 
				}
				else
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("����")); 
				}
				
				//g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 91, 1.0f, "%d/%d" , 
				//	g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usCurDur, g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur);// ������
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 106, 1.0f, "%d" , g_WeaponItemProperty[m_ID].cStrength);

				switch(g_WeaponItemProperty[m_ID].cBalance) {
				case 25:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 121, _T("õǰ")); 
					break;
				case 20:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 121, _T("��ǰ")); 
					break;
				case 15:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 121, _T("��ǰ")); 
					break;
				case 10:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 121, _T("��ǰ")); 
					break;
				case 5:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 121, _T("��ǰ")); 
					break;
				}
				//g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 121, 1.0f, "%d" , g_WeaponItemProperty[m_ID].cBalance);

				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 136, 1.0f, "%d" , g_WeaponItemProperty[m_ID].sClanPoint1); // 2004.06.14->oneway48 insert


				if( CheckWisdom( _XGI_FC_WEAPON, m_ID ) )
				{
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 155, _T("�䱸ġ ����")); 
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 0, 210, 255 ) );
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 155, _T("��Ȯ�� �϶�")); 
				}
				
				if( CheckMuscularStrength( _XGI_FC_WEAPON, m_ID ) )
				{
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 170, _T("�䱸ġ ����")); 
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 0, 210, 255 ) );
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 170, _T("������ ����")); 
				}
				
				if( CheckLevel( _XGI_FC_WEAPON, m_ID ) )
				{
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 185, _T("�䱸ġ ����")); 
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 0, 210, 255 ) );
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 185, _T("���ݷ� �϶�")); 
				}
				
				// 2004.06.14->oneway48 insert
				if(pNPCTrade_Window && pNPCTrade_Window->GetShowStatus())
				{
					// ��ǰ ���� �ڽ�
					g_XBaseFont->SetColor(_XSC_DEFAULT);	// 2004.06.17->oneway48 insert
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+202, m_WindowPosition.x+194, m_WindowPosition.y+220 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+201, m_WindowPosition.x+194, m_WindowPosition.y+220 , 0.0f, _XSC_DEFAULT_BLACK );
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 205, m_DescPrice );// ����
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 205, 1.0f, "%d" , g_WeaponItemProperty[m_ID].uiPrice/5);
					
				}
				else if( pRepairItem_Window && pRepairItem_Window->GetShowStatus())// 2004.06.29->oneway48 insert
				{
					// ��ǰ ���� ���� �ڽ�
					if( pRepairItem_Window->m_bRepairWeapon )
					{
						int shortdur = g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur - g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usCurDur;
						int needmoney = (shortdur-1)/15 + 1;
						
						g_XBaseFont->SetColor(_XSC_DEFAULT);	
						_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+202, m_WindowPosition.x+194, m_WindowPosition.y+220 , D3DCOLOR_ARGB(100,0,0,0) );
						_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+201, m_WindowPosition.x+194, m_WindowPosition.y+220 , 0.0f, _XSC_DEFAULT_BLACK );
						g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 205, _T("���� ����") );// ����
						
						if( needmoney == 1 )
						{
							g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 205, _T("���� �ʿ� ����") );
						}
						else
						{
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 205, 1.0f, "%d" , needmoney);
						}
					}

				}
				
		
				// 2004.05.18->oneway48 modify : ����Ÿ�� ������ ���� 
	/*
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				if(g_WeaponItemProperty[m_ID].���üӼ�)
				{	
					// ��ǰ ���� ���� �ڽ�
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+202, m_WindowPosition.x+194, m_WindowPosition.y+233 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+201, m_WindowPosition.x+194, m_WindowPosition.y+233 , 0.0f, _XSC_DEFAULT_BLACK );
					
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 204, m_DescRefine1String );// ���� �Ӽ� 1
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 219, m_DescRefine2String );// ���� �Ӽ� 2				
				}
				
				if(g_WeaponItemProperty[m_ID].�ΰ�ȿ��)
				{						
					// �ΰ� ȿ�� ���� �ڽ�
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+236, m_WindowPosition.x+194, m_WindowPosition.y+282 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+235, m_WindowPosition.x+194, m_WindowPosition.y+282 , 0.0f, _XSC_DEFAULT_BLACK );
					
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,219,196,136));
					
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 239, m_DescAdditionalEffect1String );// �ΰ� ȿ�� 1 
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 254, m_DescAdditionalEffect2String );// �ΰ� ȿ�� 2
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 269, m_DescAdditionalEffect3String );// �ΰ� ȿ�� 3
				}

				
				if(g_WeaponItemProperty[m_ID].��Ʈȿ��)
				{		
					// ��Ʈ ȿ�� ���� �ڽ�
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+285, m_WindowPosition.x+194, m_WindowPosition.y+361 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+284, m_WindowPosition.x+194, m_WindowPosition.y+361 , 0.0f, _XSC_DEFAULT_BLACK );
						
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,0,255,0));
					
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 288, m_DescSetEffect1String );// ��Ʈ ȿ�� 1
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 303, m_DescSetEffect2String );// ��Ʈ ȿ�� 2
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 318, m_DescSetEffect3String );// ��Ʈ ȿ�� 3
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 333, m_DescSetEffect4String );// ��Ʈ ȿ�� 4
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 348,m_DescSetEffect5String);// ��Ʈ ȿ�� 5
				}
	*/				
				// modify end
			}
			
			break;

 		case _XGI_FC_CLOTHES :
			{			
				// ��ǰ �⺻ ����
				_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+59, m_WindowPosition.x+194, m_WindowPosition.y+150 + 15 , D3DCOLOR_ARGB(100,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+58, m_WindowPosition.x+194, m_WindowPosition.y+150 + 15 , 0.0f, _XSC_DEFAULT_BLACK );
				
				// ��ǰ �䱸 ����
				_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+153 + 15, m_WindowPosition.x+194, m_WindowPosition.y+199 + 15, D3DCOLOR_ARGB(100,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+152 + 15, m_WindowPosition.x+194, m_WindowPosition.y+199 + 15 , 0.0f, _XSC_DEFAULT_BLACK );
		
				// ��ǰ �̸� ����
				g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_ClothesItemProperty[m_ID].cName);
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_ClothesItemProperty[m_ID].cName);
				
				// ��ǰ ���� ���� : 2004.06.16->oneway48 insert
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				
				g_XBaseFont->Puts_Separate( m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_ClothesItemProperty[m_ID].cItemInfo, 180 );
				
				// insert end

				// �⺻ ����
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 61, m_DescGenderString );// ���� 2004.06.11->oneway48 insert
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 61 + 15, m_DescStrengthString );// ����
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 76 + 15, m_DescWeightString );// ����
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 91 + 15, m_DescDefenseString );// ����
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 106 + 15, _T("���밨") );// ������
				//g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 106 + 15, m_DescDurabilityString );// ������
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 121 + 15, m_DescWarmthString );// ����ȿ��
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 136 + 15, m_DescResistanceString );// �Ӽ� ���׷�
				
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 155 + 15, m_DescHeartString );// �ɱ�
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 170 + 15, m_DescStaminaString );// ���� 
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 185 + 15, m_DescLevelString );// ��������

				// 2004.06.11->oneway48 
				if( g_ClothesItemProperty[m_ID].cSexType == 0 )  // ����
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("����")); 
				}
				else if(g_ClothesItemProperty[m_ID].cSexType == 1)  // ����
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("����")); 
				}
				else // ����
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("����")); 
				}

				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 61 + 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].cStrength); // ����

				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 + 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].sClanPoint1); // 2004.06.14->oneway48 insert

				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 91 + 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].sDefence); // ����
			
				
				// 2004.06.16->oneway48 insert 
				FLOAT percent = 0;				
				FLOAT maxdur = g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur;
				//maxdur = g_ClothesItemProperty[g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_sID].usDurability;
				
				if( maxdur != 0 )
					percent = (FLOAT)g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usCurDur / maxdur * 100.0f; // 2004.06.14->oneway48 insert

				if( percent >= 80.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 106+ 15, _T("��������")); 
				}
				else if( percent < 80 && percent >= 60.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 106+ 15, _T("����ϴ�")); 
				}
				else if( percent < 60 && percent >= 40.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 106+ 15, _T("��ȣ�ϴ�")); 
				}
				else if( percent < 40 && percent >= 20.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 106+ 15, _T("�����ϴ�")); 
				}
				else
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 106+ 15, _T("�������")); 
				}
				

			//	g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 106+ 15, 1.0f, "%d/%d" , 
			//		g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usCurDur, g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur);// ������
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 121 + 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].cWarm);// ����ȿ��
				
				
				// 2004.06.14->oneway48 insert
				if( CheckWisdom( _XGI_FC_CLOTHES, m_ID ) )
				{
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 155+ 15, _T("�䱸ġ ����")); 
					//g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 155+ 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].sReqJung);
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 0, 210, 255 ) );
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 155+ 15,  _T("ȸ�ǵ� �϶�")); 
				}

				if( CheckMuscularStrength( _XGI_FC_CLOTHES, m_ID ) )
				{
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 170+ 15, _T("�䱸ġ ����")); 
					//g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 170+ 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].sReqSim);
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 0, 210, 255 ) );
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 170+ 15,_T("������ ����")); 
				}

				g_XBaseFont->SetColor(_XSC_DEFAULT);
				
				g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 185+ 15, _T("�䱸ġ ����")); 
				//g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 185+ 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].sReqLevel);
				
				// 2004.06.14->oneway48 insert
				if(pNPCTrade_Window && pNPCTrade_Window->GetShowStatus())
				{
					// ��ǰ ���� �ڽ�
					g_XBaseFont->SetColor(_XSC_DEFAULT);	// 2004.06.17->oneway48 insert
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+217, m_WindowPosition.x+194, m_WindowPosition.y+235 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+216, m_WindowPosition.x+194, m_WindowPosition.y+235 , 0.0f, _XSC_DEFAULT_BLACK );
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 220, m_DescPrice );// ����
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 220, 1.0f, "%d" , g_ClothesItemProperty[m_ID].uiPrice/5);
				}
				else if( pRepairItem_Window && pRepairItem_Window->GetShowStatus())// 2004.06.29->oneway48 insert
				{
					// ��ǰ ���� ���� �ڽ�
					if( !pRepairItem_Window->m_bRepairWeapon )
					{
						int shortdur = g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur - g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usCurDur;
						int needmoney = (shortdur-1)/15 + 1;
						
						g_XBaseFont->SetColor(_XSC_DEFAULT);	// 2004.06.17->oneway48 insert
						_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+217, m_WindowPosition.x+194, m_WindowPosition.y+235 , D3DCOLOR_ARGB(100,0,0,0) );
						_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+216, m_WindowPosition.x+194, m_WindowPosition.y+235 , 0.0f, _XSC_DEFAULT_BLACK );
						g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 220, _T("���� ����") );// ����
						
						if( needmoney == 1 )
						{
							g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 220, _T("���� �ʿ� ����") );
						}
						else
						{
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 220, 1.0f, "%d" , needmoney);
						}				
					}
					
				}

				// 2004.05.18->oneway48 modify
	/*
				if(g_ClothesItemProperty[m_ID].���üӼ�)
				{	
					// ��ǰ ���� ����
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+202, m_WindowPosition.x+194, m_WindowPosition.y+233  + 15, D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+201, m_WindowPosition.x+194, m_WindowPosition.y+233  + 15, 0.0f, _XSC_DEFAULT_BLACK );
					
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 204 + 15, m_DescRefine1String );// ���� �Ӽ� 1
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 219 + 15, m_DescRefine2String );// ���� �Ӽ� 2
					  
				}
				
				if(g_ClothesItemProperty[m_ID].�ΰ�ȿ��)
				{						
					// �ΰ� ȿ�� ���� �ڽ�
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+236, m_WindowPosition.x+194, m_WindowPosition.y+282 + 15 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+235, m_WindowPosition.x+194, m_WindowPosition.y+282 + 15 , 0.0f, _XSC_DEFAULT_BLACK );
					
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,219,196,136));
					  
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 239 + 15, m_DescAdditionalEffect1String );// �ΰ� ȿ�� 1 
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 254 + 15, m_DescAdditionalEffect2String );// �ΰ� ȿ�� 2
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 269 + 15, m_DescAdditionalEffect3String );// �ΰ� ȿ�� 3
				}

				
				if(g_ClothesItemProperty[m_ID].��Ʈȿ��)
				{		
					// ��Ʈ ȿ�� ���� �ڽ�
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+285, m_WindowPosition.x+194, m_WindowPosition.y+361 + 15 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+284, m_WindowPosition.x+194, m_WindowPosition.y+361 + 15, 0.0f, _XSC_DEFAULT_BLACK );
				
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,0,255,0));
					  
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 288 + 15, m_DescSetEffect1String );// ��Ʈ ȿ�� 1
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 303 + 15, m_DescSetEffect2String );// ��Ʈ ȿ�� 2
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 318 + 15, m_DescSetEffect3String );// ��Ʈ ȿ�� 3
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 333 + 15, m_DescSetEffect4String );// ��Ʈ ȿ�� 4
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 348 + 15,m_DescSetEffect5String);// ��Ʈ ȿ�� 5
				}
	*/				
				// modify end
			}			
			break;

		case _XGI_FC_POTION :
				{
				// ��ǰ �⺻ ����
				_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+59, m_WindowPosition.x+194, m_WindowPosition.y+120 , D3DCOLOR_ARGB(100,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+58, m_WindowPosition.x+194, m_WindowPosition.y+120 , 0.0f, _XSC_DEFAULT_BLACK );
				

				// ��ǰ �̸� ����
				g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_PotionItemProperty[m_ID].cName);
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_PotionItemProperty[m_ID].cName);

				
				// ��ǰ ���� ���� : 2004.06.16->oneway48 insert
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				
				g_XBaseFont->Puts_Separate( m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_PotionItemProperty[m_ID].cItemInfo, 180 );

				// �⺻ ����1
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 61, m_DescRollString );// ����
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 76, m_DescEffectString );// ȿ��
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 91, m_DescFeatureString );// Ư¡
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 106, m_DescWeightString );// ����
				
				// 2004.06.15->oneway48 insert
				switch(g_PotionItemProperty[m_ID].cSecondType) 
				{
				case 0:
					{
						g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("���� ġ����") );
						switch(g_PotionItemProperty[m_ID].cApplyType) 
						{					
						case 0:
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 , 1.0f, "���� %d ����" , g_PotionItemProperty[m_ID].sApplyValue); // ���� 
							break;
						case 1:
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 , 1.0f, "���� %d%% ����" , g_PotionItemProperty[m_ID].sApplyValue); // ���� 
							break;
						}
					}
					break;
				case 1:
					{
						g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("�ܻ� ġ����") );
						switch(g_PotionItemProperty[m_ID].cApplyType) 
						{					
						case 0:
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 , 1.0f, "�ܻ� %d ����" , g_PotionItemProperty[m_ID].sApplyValue); // ���� 
							break;
						case 1:
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 , 1.0f, "�ܻ� %d%% ����" , g_PotionItemProperty[m_ID].sApplyValue); // ���� 
							break;
						}
					}
					break;
				case 2:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("�Ƿ� ȸ����") );
					break;
				}
				
				switch(g_PotionItemProperty[m_ID].cClan) 
				{
				case 0:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("���� ���") );
					break;
				case 1:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("���� ����") );
					break;
				case 2:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("��� ����") );
					break;
				case 3:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("�Ҹ� ����") );
					break;
				case 4:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("�츲 ����") );
					break;
				case 5:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("���� ����") );
					break;
				case 6:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("���� ����") );
					break;
				case 7:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("���� ����") );
					break;
				}

				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 106, 1.0f, "%d" , g_PotionItemProperty[m_ID].sClanPoint1);// ����

				if(pNPCTrade_Window && pNPCTrade_Window->GetShowStatus())
				{
					// ��ǰ ���� �ڽ�
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+123, m_WindowPosition.x+194, m_WindowPosition.y+141 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+122, m_WindowPosition.x+194, m_WindowPosition.y+141 , 0.0f, _XSC_DEFAULT_BLACK );
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 126, m_DescPrice );// ����
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 126, 1.0f, "%d" , g_PotionItemProperty[m_ID].uiPrice/5);
					
				}
				// insert end
			}

			break;
			
		case _XGI_FC_QUEST :
			{
				g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
				
				// ��ǰ �̸� ����
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_QuestItemProperty[m_ID].cName);
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_QuestItemProperty[m_ID].cName);
				
				// ��ǰ ���� ���� : 2004.06.16->oneway48 insert
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				
				g_XBaseFont->Puts_Separate( m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_QuestItemProperty[m_ID].cItemInfo, 180 );
			}

			break;
			
			//2004.06.20->oneway48 insert
		case _XGI_FC_RESOURCE :	
			{
				g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
				
				// ��ǰ �̸� ����
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_ResourceItemProperty[m_ID].cName);
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_ResourceItemProperty[m_ID].cName);
				
				// ��ǰ ���� ���� 
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				g_XBaseFont->Puts_Separate( m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_ResourceItemProperty[m_ID].cItemInfo, 180 );

				if(pNPCTrade_Window && pNPCTrade_Window->GetShowStatus())
				{
					// ��ǰ ���� �ڽ�
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+59, m_WindowPosition.x+194, m_WindowPosition.y+77 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+58, m_WindowPosition.x+194, m_WindowPosition.y+77 , 0.0f, _XSC_DEFAULT_BLACK );
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 62, m_DescPrice );// ����
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 62, 1.0f, "%d" , g_ResourceItemProperty[m_ID].uiPrice/5);
				}
			}
			break;
			
		default :
			g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);

			// ��ǰ �̸� ����
			g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, "NONAME");
			g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, "NONAME");
			
			break;
		}		
	}

	g_XBaseFont->Flush();
}

BOOL _XWindow_ItemTooltip::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if(CheckMousePosition())
					this->ShowWindow(FALSE);
			}
		}
	}
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_ItemTooltip::SetWindowPosition(void)
{
	POINT			position;
	POINT			parentposition = m_ParentWindow->GetWindowPos();
	SIZE			parentsize = m_ParentWindow->GetWindowSize();
	
	position.x = parentposition.x + parentsize.cx + 6;
	position.y = parentposition.y + 3;
	MoveWindow(position.x , position.y);
	
	CheckOutside();
}

void _XWindow_ItemTooltip::CheckOutside(void)
{
	BOOL		reposition = FALSE;
	POINT		position;
	POINT		parentposition = m_ParentWindow->GetWindowPos();
	SIZE		parentsize = m_ParentWindow->GetWindowSize();
	
	if(m_WindowPosition.x < 0)
	{
		// ���ʿ��� ©��
		position.x = parentposition.x + parentsize.cx + 6;
		position.y = parentposition.y + 3;
		reposition = TRUE;
	}
	if(m_WindowPosition.x + m_WindowSize.cx >= gnWidth)
	{
		// �����ʿ��� ©��
		position.x = parentposition.x - m_WindowSize.cx - 6;
		position.y = parentposition.y + 3;
		reposition = TRUE;
	}
	
	if(reposition)
	{
		MoveWindow(position.x, position.y);
	}
}

void _XWindow_ItemTooltip::ShowWindow(BOOL show)
{
	if(show)
	{
		m_pMyManager->SetTopWindow(this);		
	}
	
	_XWindow::ShowWindow(show);
}

void _XWindow_ItemTooltip::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);

	//m_ItemBorder.MoveWindow( X + 6, Y + 5 );
}


// 2004.06.11->oneway48 insert
BOOL _XWindow_ItemTooltip::CheckLevel(_XGI_FirstCategory type, int uniqid )
{
	if( type == _XGI_FC_WEAPON)
	{
		if(g_pLocalUser->m_CharacterInfo.level >= g_WeaponItemProperty[uniqid].sReqLevel) 
			return TRUE;
	}
	return FALSE;
}

BOOL _XWindow_ItemTooltip::CheckWisdom(_XGI_FirstCategory type, int uniqid)
{
	if( type == _XGI_FC_WEAPON )
	{
		if(g_pLocalUser->m_CharacterInfo.dexterity >= g_WeaponItemProperty[uniqid].sReqSim ) 
			return TRUE;
	}
	else if( type == _XGI_FC_CLOTHES )
	{
		if(g_pLocalUser->m_CharacterInfo.dexterity >= g_ClothesItemProperty[uniqid].sReqSim ) 
			return TRUE;
	}
	return FALSE;
}

BOOL _XWindow_ItemTooltip::CheckMuscularStrength(_XGI_FirstCategory type, int uniqid)
{
	if( type == _XGI_FC_WEAPON)
	{
		if(g_pLocalUser->m_CharacterInfo.strength >= g_WeaponItemProperty[uniqid].sReqJung) 
			return TRUE;
	}
	else if( type == _XGI_FC_CLOTHES )
	{
		if(g_pLocalUser->m_CharacterInfo.strength >= g_ClothesItemProperty[uniqid].sReqJung) 
			return TRUE;
	}
	return FALSE;
}

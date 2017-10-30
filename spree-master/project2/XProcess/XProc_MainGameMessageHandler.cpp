#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"
#include "XProc_ProcessDefine.h"
#include "XProc_MainGame.h"
#include "XMeshContainer.h"
#include "XWindow_ChattingDefault.h"
#include "XWindow_SystemMsgDefault.h"
#include "XWindow_Option.h"
#include "XWindow_OptionGraphic.h"
#include "XWindow_OptionGame.h"
#include "XWindow_NPCScript.h"
#include "XWindow_PCTrade.h"
#include "XWindow_Debug.h"
#include "XWindow_MaximumMap.h"
#include "XWindow_Party.h"
#include "XWindow_PartyMemberMiniBar.h"
#include "XWindow_PartyMasterMiniBar.h"
#include "XWindow_Skill.h"
#include "XWindow_TradeConfirm.h"
#include "XNPCScript.h"
#include "XWindow_NPCTrade.h"
#include "XWindow_RequestParty.h"
#include "XWindow_Warehouse.h"
#include "XWindow_AddSaveItem.h"
#include "XWindow_Messenger.h"
#include "XWindow_MessengerTooltip.h"
#include "XWindow_NameInput.h"
#include "XWindow_NPCSkill.h"
#include "XWindow_Rebirth.h"
#include "XWindow_RepairItem.h"// 2004.06.17->oneway48 insert
#include "Xwindow_TrainingMed.h"
#include "XWindow_TrainingDummy.h"
#include "XWindow_TrainingSpell.h"
#include "XWindow_Help.h"
#include "XWindow_EnterBossRoom.h"
#include "XWindow_NickName.h"
#include "XMessageWindow.h"
#include "XWindow_SocketPlugIn.h"//2004.11.5->hotblood
#include "xwindow_mainmenu.h"//2004.11.5->hotblood
#include "XWindow_Email.h"
#include "XWindow_MessengerDialog.h"
#include "XWindow_Contribution.h"
#include "xwindow_chargegauge.h"
#include "XWindow_Hedek.h"
#include "XWindow_Pocket.h"
#include "XWindow_Tutorial.h"
#include "XWindow_TutorialIcon.h"
#include "XWindow_HelpBook.h"
#include "XWindow_TrainingHelp.h"
#include "XWindow_PersonalStore.h"
#include "XWindow_QuestMail.h"
#include "XWindow_Gamble.h"
#include "XWindow_TargetInfo.h"
#include "XWindow_SocketPlugInExp.h"
#include "XWindow_Inchant.h"
#include "XWindow_GuideLine.h"

// GM Control windows
#include "XWindow_GMCommand.h"
#include "XWindow_GM_SendNoticeMessage.h"
#include "XWindow_GM_UserCoordination.h"
#include "XWindow_GM_MonsterManage.h"
#include "XWindow_GM_StatusControl.h"
#include "XWindow_GM_EventManager.h"

#include "XNPCScriptConditionItem.h"
#include "SoundEffectList.h"
#include "XSR_STRINGHEADER.H"
#include "XQuestScriptItemGroup.h"
#include "XWindow_MainFrame.h"
#include "XWindow_MonsterStatus.h"
#include "XWindow_JinStatus.h"
#include "Xwindow_PVPBroker.h"
#include "Xwindow_MatchTablePVP.h"
#include "XWindow_MatchNoticeLarge.h"
#include "XWindow_MatchPR.h"
#include "XWindow_MatchRecord.h"
#include "XWindow_CashItem.h"
#include "XWindow_NPCCollection.h"
#include "XWindow_NPCResource.h"
#include "XWindow_QuestRank.h"
#include "XWindow_NPCSearch.h"
#include "XWindow_MoveVillage.h"
#include "XWindow_Event.h"
#include "XWindow_CastleInfo.h"
#include "XWindow_ChallengerList.h"
#include "XWindow_CastleBattleShout.h"
#include "XWindow_CastleBattleTimeChange.h"
#include "XWindow_CBResultNotice.h"
#include "XWindow_Escape.h"
#include "XWindow_PKTracingRequest.h"
#include "XWindow_PK_ListBox.h" //Author : �����
#include "XWindow_SmeltItem.h" //Author : �����
#include "XWindow_WorldMap.h" //Author : ����� //breif : �ű�������
#include "XWindow_RecallItem.h" //Author : �����
#include "XWindow_CBOrgRecord.h"
#include "XWindow_CastleBattleRewardList.h"
#include "XWindow_CBRewardSmall.h"
#include "Xwindow_CBRecord.h"
#include "XWindow_BlackList.h"
#include "XWindow_SocketDetach.h" //Author : �����
#include "XWindow_Poison.h"
#include "XWindow_MixWeapon.h"
#include "XWindow_Info.h"
#include "XWindow_FreeMatchInfo.h"
#include "XWindow_NPCResource_New.h"
#include "XWindow_TargetInfoSelect.h" //Author : ����� //breif : Ÿ�� ���� ����
#include "XWindow_MnDGroup.h" // Author : ����� //breif : �������� ��
#include "XWindow_NPCMnDInfo.h" //Author : ����� //breif :���� �ý���

#ifdef _XTS_ITEMMALLBROWSER	
#include "XWindow_CashMall.h"	
#include "XWindow_CashBox.h"
#endif

#ifdef _XDEF_WEBBOARD_20061211
#include "XWindow_WebBoard.h"	
#endif
// win32 web browser api
#include "./Library/CWebPage/XWebPage.h"

#include "XWindowObjectDefine.h"

#include "XParticleList.h" //Author : ����� //����Ʈ
#include "XParticleIDDefine.h" //Author : ����� //����Ʈ

extern _map_NPCBuffStr				g_NPCBuffStr;

void SendBossCreat()
{	
	g_NetworkKernel.SendPacket( MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S, g_ChannelTempZoneInfoIndex );
	_XRemoveTimer(SendBossCreat);
}

extern void _XInsertCenterSystemMessage( LPTSTR message , D3DCOLOR messageinfo );

void ChangeModel( DWORD lparam, char firstType, char facemaskmode = 0 )
{
	if( facemaskmode )
	{
		// ���� ���� �� ��� 
		if(g_pLocalUser->m_bFaceMaskMode)
		{
			int extravalue = 0;
			if( g_pLocalUser->m_CharacterInfo.gender == _XGENDER_WOMAN ) extravalue = g_FaceMaskModel_GenderSeparateIndex;
			g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_FACEMASK, g_UserModel_FaceMask[extravalue] );			
		}
		else 
			g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_FACEMASK, NULL );
	}
	else
	{	
		int category = lparam / 100000;
		int index = lparam % 100000;
		
		switch( category ) 
		{
		case _XINVENTORY_SLOTTYPE_AMOR  :
			{
				// ����
				int extravalue = 0;
				if( g_pLocalUser->m_CharacterInfo.gender == _XGENDER_WOMAN ) extravalue = g_Model_GenderSeparateIndex;
				g_pLocalUser->m_CharacterInfo.modeldescript.upperbody = index;
				g_pLocalUser->m_CharacterInfo.modeldescript.upperbodyCategory = (_XGI_FirstCategory)firstType;

#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
				if(g_pLocalUser->m_CharacterInfo.m_nClothView == 0)	// ġ�� �ǻ󺸱�
#endif
				{
					if( g_pLocalUser->m_CharacterInfo.modeldescript.mask )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,	NULL );
						break;
					}
				}

				if( index == 0 )
				{
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,		g_UserModel_UBodyType[extravalue] );		
				}
				else
				{
					if( g_pLocalUser->m_CharacterInfo.modeldescript.upperbodyCategory == _XGI_FC_CLOTHES )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,		
							g_UserModel_UBodyType[extravalue+g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.upperbody].sModelIndex] );
					}
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.upperbodyCategory == _XGI_FC_CLOTHES2 )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,		
							g_UserModel_UBodyType[extravalue+g_ClothesItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.upperbody].sModelIndex] );
					}				
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.upperbodyCategory == _XGI_FC_CLOTHES3 )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,		
							g_UserModel_UBodyType[extravalue+g_ClothesItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.upperbody].sModelIndex] );
					}				
				}
			}
			break;	
		case _XINVENTORY_SLOTTYPE_PANTS :
			{
				// ����
				int extravalue = 0;
				if( g_pLocalUser->m_CharacterInfo.gender == _XGENDER_WOMAN ) extravalue = g_Model_GenderSeparateIndex;
				g_pLocalUser->m_CharacterInfo.modeldescript.lowerbody = index;
				g_pLocalUser->m_CharacterInfo.modeldescript.lowerbodyCategory = (_XGI_FirstCategory)firstType;

#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
				if(g_pLocalUser->m_CharacterInfo.m_nClothView == 0)	// ġ�� �ǻ󺸱�
#endif
				{
					if( g_pLocalUser->m_CharacterInfo.modeldescript.mask )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,	NULL );
						break;
					}
				}

				if( index == 0 )
				{
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,		g_UserModel_LBodyType[extravalue] );		
				}
				else
				{
					if( g_pLocalUser->m_CharacterInfo.modeldescript.lowerbodyCategory == _XGI_FC_CLOTHES )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,		
							g_UserModel_LBodyType[extravalue+g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.lowerbody].sModelIndex] );	
					}
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.lowerbodyCategory == _XGI_FC_CLOTHES2 )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,		
							g_UserModel_LBodyType[extravalue+g_ClothesItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.lowerbody].sModelIndex] );	
					}
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.lowerbodyCategory == _XGI_FC_CLOTHES3 )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,		
							g_UserModel_LBodyType[extravalue+g_ClothesItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.lowerbody].sModelIndex] );	
					}
				}
			}		
			break;
		case _XINVENTORY_SLOTTYPE_CAP :
			{
				// ����
				int extravalue = 0;
				if( g_pLocalUser->m_CharacterInfo.gender == _XGENDER_WOMAN ) extravalue = g_Model_GenderSeparateIndex;
				g_pLocalUser->m_CharacterInfo.modeldescript.cap = index;

				g_pLocalUser->m_CharacterInfo.modeldescript.capCategory = (_XGI_FirstCategory)firstType;

#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
				if(g_pLocalUser->m_CharacterInfo.m_nClothView == 0)	// ġ�� �ǻ󺸱�
#endif
				{
					if( g_pLocalUser->m_CharacterInfo.modeldescript.mask )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,	NULL );
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,	NULL );
						break;
					}
				}

				if( index == 0 )
				{
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, NULL );
				}
				else
				{	
					// ĳ���Ͱ� �Ҹ��̰� �������� �Ҹ��� �ƴѰ��� ���� ���� �Ұ�
					if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM )
					{
						if( g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.cap].cClan  != _XGROUP_SORIM )
						{
							if( g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.cap].cClan == _XGROUP_NANGIN &&
								g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sReqSim  == 1 )//�Ҹ� �Ҵ����� ����
							{
								if( g_pLocalUser->m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES )
								{
									g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,
									g_UserModel_Cap[extravalue +g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex] );
								}
								else if( g_pLocalUser->m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES2 )
								{
									g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,
									g_UserModel_Cap[extravalue +g_ClothesItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex] );
								}
								else if( g_pLocalUser->m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES3 )
								{
									g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,
									g_UserModel_Cap[extravalue +g_ClothesItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex] );
								}
							}
							else
							{
								g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, NULL );
							}
						}
						else
						{
							if( g_pLocalUser->m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES )
							{
								g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,
								g_UserModel_Cap[extravalue +g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex] );
							}
							else if( g_pLocalUser->m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES2 )
							{
								g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,
								g_UserModel_Cap[extravalue +g_ClothesItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex] );
							}
							else if( g_pLocalUser->m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES3 )
							{
								g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,
								g_UserModel_Cap[extravalue +g_ClothesItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex] );
							}
						}
					}
					else
					{
						// ���ھ���				
						if( g_pLocalUser->m_CharacterInfo.modeldescript.hair ) // �Ӹ�ī���� ������� �Ӹ�ī���� �����.
						{
							// ���ĸ��ڴ� ���ε��� �ٷ� ����
							/*if( g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.cap].cClan )
							{
								g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,
									g_UserModel_Cap[extravalue +g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex] );
							}
							else*/
							{
								// �Ӹ� ���� ���� ����
								int modelcolorindex = (g_pLocalUser->m_CharacterInfo.modeldescript.hair-1) % 3;

								if( g_pLocalUser->m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES )
								{
									g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,	// �Ӹ� ���� 3Ÿ���̹Ƿ� 3Ÿ�Խ� �ǳʶٰ� �Ӹ����� �´� �� ����	
												g_UserModel_Cap[extravalue + g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex + modelcolorindex ] );
								}
								else if( g_pLocalUser->m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES2 )
								{
									g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,	// �Ӹ� ���� 3Ÿ���̹Ƿ� 3Ÿ�Խ� �ǳʶٰ� �Ӹ����� �´� �� ����	
												g_UserModel_Cap[extravalue + g_ClothesItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex + modelcolorindex ] );
								}
								else if( g_pLocalUser->m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES3 )
								{
									g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,	// �Ӹ� ���� 3Ÿ���̹Ƿ� 3Ÿ�Խ� �ǳʶٰ� �Ӹ����� �´� �� ����	
												g_UserModel_Cap[extravalue + g_ClothesItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex + modelcolorindex ] );
								}
							}
						}
						else // �Ӹ�ī�� ������ ������� ( ����ó�� )
						{
							/*if( g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.cap].cClan )
							{
								g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP,
									g_UserModel_Cap[extravalue +g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex] );
							}
							else*/
							{
								if( g_pLocalUser->m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES )
								{
									g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, 
												g_UserModel_Cap[extravalue + g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex ] );
								}
								else if( g_pLocalUser->m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES2 )
								{
									g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, 
												g_UserModel_Cap[extravalue + g_ClothesItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex ] );
								}
								else if( g_pLocalUser->m_CharacterInfo.modeldescript.capCategory == _XGI_FC_CLOTHES3 )
								{
									g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, 
												g_UserModel_Cap[extravalue + g_ClothesItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.cap].sModelIndex ] );
								}
							}
						}
					}
				}

				// ���ڸ� ���� �Ǵ� ������ �Ӹ�ī�� �� ó��
				if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM )
				{
					if( g_pLocalUser->m_CharacterInfo.gender== _XGENDER_MAN ) 
					{
						// �Ҹ��� �Ӹ�ī���� ����.
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 NULL );
					}
					else
					{   // �Ҹ��翡 �����ϰ�� (�׷����� �������� �׷��� �⺻���� ����)
						if( g_pLocalUser->m_CharacterInfo.modeldescript.hair )
						{
							if( g_pLocalUser->m_CharacterInfo.modeldescript.cap == 0 ) // ���ڸ� �������� �Ӹ�ī�� ó�� ����. 
								g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 
								g_UserModel_HairType[extravalue+g_pLocalUser->m_CharacterInfo.modeldescript.hair-1] );
							else g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 NULL );
						}
						else 
						{
							if( g_pLocalUser->m_CharacterInfo.modeldescript.cap == 0 ) // ���ڸ� �������� �Ӹ�ī�� ó�� ����. 
								g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR, g_UserModel_HairType[extravalue] ); // �⺻ �Ӹ�ī�� ���� 
							else g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 NULL );
						}
					}
				}
				else
				{
					if( g_pLocalUser->m_CharacterInfo.modeldescript.hair )
					{
						if( g_pLocalUser->m_CharacterInfo.modeldescript.cap == 0 ) // ���ڸ� �������� �Ӹ�ī�� ó�� ����. 
							g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,
							g_UserModel_HairType	[extravalue+g_pLocalUser->m_CharacterInfo.modeldescript.hair-1] );
						else g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 NULL );
					}
					else 
					{
						if( g_pLocalUser->m_CharacterInfo.modeldescript.cap == 0 ) // ���ڸ� �������� �Ӹ�ī�� ó�� ����. 
							g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,	g_UserModel_HairType[extravalue] );
						else 
							g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR,		 NULL );
						
					}
				}
			}
			break;
		case _XINVENTORY_SLOTTYPE_ADAE :
			{
				// �ƴ� (�ǻ��� ��)
				g_pLocalUser->m_CharacterInfo.modeldescript.adae = index;
				g_pLocalUser->m_CharacterInfo.modeldescript.adaeCategory = (_XGI_FirstCategory)firstType;
				
#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
				if(g_pLocalUser->m_CharacterInfo.m_nClothView == 1)
#endif
				{
					if( g_pLocalUser->m_CharacterInfo.modeldescript.mask )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ADAE,	NULL );
						break;
					}
				}
				g_pLocalUser->m_ModelDescriptor.SetMeshModel( g_pLocalUser->m_CharacterInfo.modeldescript, g_pLocalUser->m_CharacterInfo.gender, g_pLocalUser->m_CharacterInfo.groupindex );
			}
			break;
		case _XINVENTORY_SLOTTYPE_SHOES :
			{
				// �Ź�
				int extravalue = 0;
				if( g_pLocalUser->m_CharacterInfo.gender== _XGENDER_WOMAN ) extravalue = g_Model_GenderSeparateIndex;
				
				g_pLocalUser->m_CharacterInfo.modeldescript.lfoot = index;
				g_pLocalUser->m_CharacterInfo.modeldescript.rfoot = index;

				g_pLocalUser->m_CharacterInfo.modeldescript.lfootCategory = (_XGI_FirstCategory)firstType;
				g_pLocalUser->m_CharacterInfo.modeldescript.rfootCategory = (_XGI_FirstCategory)firstType;

#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
				if(g_pLocalUser->m_CharacterInfo.m_nClothView == 0)	// ġ���ǻ󺸱�
#endif
				{
					if( g_pLocalUser->m_CharacterInfo.modeldescript.mask )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,	NULL );
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,	NULL );
						break;
					}
				}
				if(index == 0)
				{	
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		g_UserModel_LFootType[extravalue] );		
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		g_UserModel_RFootType[extravalue] );					
					
				}
				else
				{	
					if( g_pLocalUser->m_CharacterInfo.modeldescript.lfootCategory == _XGI_FC_CLOTHES )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		
							g_UserModel_LFootType[extravalue+g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.lfoot].sModelIndex] );
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		
							g_UserModel_RFootType[extravalue+g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.rfoot].sModelIndex] );		
					}
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.lfootCategory == _XGI_FC_CLOTHES2 )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		
							g_UserModel_LFootType[extravalue+g_ClothesItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.lfoot].sModelIndex] );
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		
							g_UserModel_RFootType[extravalue+g_ClothesItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.rfoot].sModelIndex] );		
					}
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.lfootCategory == _XGI_FC_CLOTHES3 )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,		
							g_UserModel_LFootType[extravalue+g_ClothesItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.lfoot].sModelIndex] );
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,		
							g_UserModel_RFootType[extravalue+g_ClothesItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.rfoot].sModelIndex] );		
					}
				}
			}
			break;
		case _XINVENTORY_SLOTTYPE_BACKPACK1 :
			{
/*	#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
				int nTempID = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID();
				int nTempType = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].m_cType;

				if( nTempID > 0 ) //ĳ�� �೶�� ������ �� ��ü�� ��������.
					break;
	#endif*/

				// �賶
				int extravalue = 0;
				if( g_pLocalUser->m_CharacterInfo.gender== _XGENDER_WOMAN ) extravalue = g_Model_GenderSeparateIndex;
				
				g_pLocalUser->m_CharacterInfo.modeldescript.backpack = index;

				g_pLocalUser->m_CharacterInfo.modeldescript.backpackCategory = (_XGI_FirstCategory)firstType;

#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
				if(g_pLocalUser->m_CharacterInfo.m_nClothView == 1)
#endif
				{
					if( g_pLocalUser->m_CharacterInfo.modeldescript.mask )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	NULL );
						break;
					}
				}
				
				if(index == 0)
				{	
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	NULL );
					
				}
				else
				{	
					if( g_pLocalUser->m_CharacterInfo.modeldescript.backpackCategory == _XGI_FC_CLOTHES )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,
							g_UserModel_BackPack[extravalue+g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.backpack].sModelIndex] );
					}
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.backpackCategory == _XGI_FC_CLOTHES2 )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,
							g_UserModel_BackPack[extravalue+g_ClothesItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.backpack].sModelIndex] );
					}
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.backpackCategory == _XGI_FC_CLOTHES3 )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,
							g_UserModel_BackPack[extravalue+g_ClothesItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.backpack].sModelIndex] );
					}
				}
			}
			break;
	#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
		case _XINVENTORY_SLOTTYPE_CASHBACKPACK1 :
        case _XINVENTORY_SLOTTYPE_CASHBACKPACK2 :
			{
				// �賶
                /*int nSlotCategory = category;

                if( category == _XINVENTORY_SLOTTYPE_CASHBACKPACK2 )
                {
                    int nID = g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID();
                    if( nID > 0 ) //1�� ĳ���೶�� ������ ��ü ���� ����
                        break;
                }

				int extravalue = 0;
				if( g_pLocalUser->m_CharacterInfo.gender== _XGENDER_WOMAN ) extravalue = g_Model_GenderSeparateIndex;
				
				g_pLocalUser->m_CharacterInfo.modeldescript.backpack = index;

				g_pLocalUser->m_CharacterInfo.modeldescript.backpackCategory = (_XGI_FirstCategory)firstType;

#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
				if(g_pLocalUser->m_CharacterInfo.m_nClothView == 1)
#endif
				{
					if( g_pLocalUser->m_CharacterInfo.modeldescript.mask )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	NULL );
						break;
					}
				}
				
				if(index == 0)
				{	
					int nTempID = g_pLocalUser->m_UserItemList[nSlotCategory].Get_m_sID();
					int nTempType = g_pLocalUser->m_UserItemList[nSlotCategory].m_cType;

					if( nTempID )
					{
						g_pLocalUser->m_CharacterInfo.modeldescript.backpack = nTempID;
						g_pLocalUser->m_CharacterInfo.modeldescript.backpackCategory = (_XGI_FirstCategory)nTempType;

						if( g_pLocalUser->m_CharacterInfo.modeldescript.backpackCategory == _XGI_FC_CLOTHES )
						{
							g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,
								g_UserModel_BackPack[extravalue+g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.backpack].sModelIndex] );
						}
						else if( g_pLocalUser->m_CharacterInfo.modeldescript.backpackCategory == _XGI_FC_CLOTHES2 )
						{
							g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,
								g_UserModel_BackPack[extravalue+g_ClothesItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.backpack].sModelIndex] );
						}
						else if( g_pLocalUser->m_CharacterInfo.modeldescript.backpackCategory == _XGI_FC_CLOTHES3 )
						{
							g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,
								g_UserModel_BackPack[extravalue+g_ClothesItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.backpack].sModelIndex] );
						}
					}
					else
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,	NULL );
					
				}
				else
				{	
					if( g_pLocalUser->m_CharacterInfo.modeldescript.backpackCategory == _XGI_FC_CLOTHES )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,
							g_UserModel_BackPack[extravalue+g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.backpack].sModelIndex] );
					}
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.backpackCategory == _XGI_FC_CLOTHES2 )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,
							g_UserModel_BackPack[extravalue+g_ClothesItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.backpack].sModelIndex] );
					}
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.backpackCategory == _XGI_FC_CLOTHES3 )
					{
						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK,
							g_UserModel_BackPack[extravalue+g_ClothesItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.backpack].sModelIndex] );
					}
				}*/
			}
			break;
	#endif
		case _XINVENTORY_SLOTTYPE_MASK :
			{
				// ġ��
				g_pLocalUser->m_CharacterInfo.modeldescript.mask = index;
				g_pLocalUser->m_CharacterInfo.modeldescript.maskCategory = (_XGI_FirstCategory)firstType;

#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
				if(g_pLocalUser->m_CharacterInfo.m_nClothView == 1)
				{
					// �Ϲ� �ǻ� ���� : ġ���ǻ� ���� ������ ó��
					index = 0;
				}
#endif
				
				if( index == 0 )
				{
					for( int i = _XINVENTORY_SLOTTYPE_AMOR; i < _XINVENTORY_SLOTTYPE_MASK; i++ )
					{
						DWORD lparam = (i*100000) + g_pLocalUser->m_UserItemList[i].Get_m_sID();
							ChangeModel(lparam, g_pLocalUser->m_UserItemList[i].Get_m_cType() );
					}

					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_MASK,	NULL );		
				}
				else
				{
					// ġ���ǻ� ����� �ٸ� �����۵��� ������ �ʰ� �Ѵ�.				
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_HAIR, NULL );
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_CAP, NULL );
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LOWERBODY,  NULL );
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_UPPERBODY,  NULL );
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LFOOT,	NULL );
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RFOOT,	NULL );
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_LHAND, NULL );
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_RHAND, NULL );
					g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_BACKPACK, NULL );				
					
					int genderextravalue = 0;
					int genderextravalue2 = 0;
					if( g_pLocalUser->m_CharacterInfo.gender== _XGENDER_WOMAN ) genderextravalue = g_MaskModel_GenderSeparateIndex;		

					if( g_pLocalUser->m_CharacterInfo.modeldescript.maskCategory == _XGI_FC_CLOTHES )
					{
						// ������ �迭�� �� �����̳� ������ ��� ����Ͽ� ���� �߰��� ���� �ε��� ���.
						if( g_pLocalUser->m_CharacterInfo.gender == _XGENDER_WOMAN )
						{
							if( g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.mask].sModelIndex >= 140 )
							{
								// ���� �߰��� ���� �� �����̳� �� 220�� ������. ���ڴ� 140������ �̹Ƿ� �ι�° �� �����̳� ������ 80ĭ��
								genderextravalue = 0;
								genderextravalue2 = g_MaskModel_GenderSeparateIndex2;
							}
						}

						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_MASK,
							g_UserModel_Mask[genderextravalue+genderextravalue2+g_ClothesItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.mask].sModelIndex] );
					}
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.maskCategory == _XGI_FC_CLOTHES2 )
					{
						// ������ �迭�� �� �����̳� ������ ��� ����Ͽ� ���� �߰��� ���� �ε��� ���.
						if( g_pLocalUser->m_CharacterInfo.gender == _XGENDER_WOMAN )
						{
							if( g_ClothesItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.mask].sModelIndex >= 140 )
							{
								// ���� �߰��� ���� �� �����̳� �� 220�� ������. ���ڴ� 140������ �̹Ƿ� �ι�° �� �����̳� ������ 80ĭ��
								genderextravalue = 0;
								genderextravalue2 = g_MaskModel_GenderSeparateIndex2;
							}
						}

						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_MASK,
							g_UserModel_Mask[genderextravalue+genderextravalue2+g_ClothesItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.mask].sModelIndex] );
					}
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.maskCategory == _XGI_FC_CLOTHES3 )
					{
						// ������ �迭�� �� �����̳� ������ ��� ����Ͽ� ���� �߰��� ���� �ε��� ���.
						if( g_pLocalUser->m_CharacterInfo.gender == _XGENDER_WOMAN )
						{
							if( g_ClothesItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.mask].sModelIndex >= 140 )
							{
								// ���� �߰��� ���� �� �����̳� �� 220�� ������. ���ڴ� 140������ �̹Ƿ� �ι�° �� �����̳� ������ 80ĭ��
								genderextravalue = 0;
								genderextravalue2 = g_MaskModel_GenderSeparateIndex2;
							}
						}

						g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_MASK,
							g_UserModel_Mask[genderextravalue+genderextravalue2+g_ClothesItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.mask].sModelIndex] );
					}
				}
			}
			break;
		case _XINVENTORY_SLOTTYPE_WEAPON_1 :
			if( index == 0 )
			{
				g_pLocalUser->m_CharacterInfo.modeldescript.item1 = 0;
				//g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,		NULL );
				g_pLocalUser->m_ModelDescriptor.SetMeshModel( g_pLocalUser->m_CharacterInfo.modeldescript, g_pLocalUser->m_CharacterInfo.gender, g_pLocalUser->m_CharacterInfo.groupindex, g_pLocalUser->m_CharacterInfo.m_nClothView );
				g_pLocalUser->m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;

				// ���� ����ϰ� �ִ� ��ų�� ������ delay time ����
				if(g_pLocalUser->m_bUsingSkill)
				{
					g_pLocalUser->m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
					g_pLocalUser->m_bCheckFirstFrame = FALSE;
					
					g_pLocalUser->m_bUsingSkill = FALSE;
					g_pLocalUser->m_bCheckDelayTime = TRUE;
					
					// delay time ��� ����
					g_pLocalUser->m_ChangeDelayStartTime = g_LocalSystemTime;
				}

				// ���� ������ ���� ������ ���̵� ���� �� ���ݷ� ����
				g_pLocalUser->m_SelectedWeaponItem = NULL;

				g_pLocalUser->m_CurrentWeaponID = 0;
				g_pLocalUser->ResetSkillAnimation();
				
				// ������ ���� ��ų ������ ����
				if(g_pQuickSlot_Window)
				{
					g_pQuickSlot_Window->SetCurrentSelectedActiveIndex(-1);
					g_pQuickSlot_Window->SetCurrentSelectedOtherIndex(-1);
				}
				g_pLocalUser->m_SelectedSkillID = 0;
				g_pLocalUser->m_ActiveSlotID = 0;
				g_pLocalUser->m_SelectedSkillItem = NULL;
			}
			else
			{
				g_pLocalUser->m_CharacterInfo.modeldescript.item1 = index;

				g_pLocalUser->m_CharacterInfo.modeldescript.item1Category = (_XGI_FirstCategory)firstType;


				//g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1,				
				//					g_ItemModel_Weapon[g_pLocalUser->m_CharacterInfo.gender][g_WeaponItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.item1].sModelIndex] );

				g_pLocalUser->m_ModelDescriptor.SetMeshModel( g_pLocalUser->m_CharacterInfo.modeldescript, g_pLocalUser->m_CharacterInfo.gender, g_pLocalUser->m_CharacterInfo.groupindex, g_pLocalUser->m_CharacterInfo.m_nClothView );

				g_pLocalUser->m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;

				if( g_pLocalUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					g_pLocalUser->m_SelectedWeaponItem = g_pLocalUser->GetUserItem( _XGI_FC_WEAPON, index );
				else if( g_pLocalUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					g_pLocalUser->m_SelectedWeaponItem = g_pLocalUser->GetUserItem( _XGI_FC_WEAPON2, index );
				else if( g_pLocalUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					g_pLocalUser->m_SelectedWeaponItem = g_pLocalUser->GetUserItem( _XGI_FC_WEAPON3, index );

				// ���� ����ϰ� �ִ� ��ų�� ������ delay time ����
/*				if(g_pLocalUser->m_bUsingSkill)
				{
					g_pLocalUser->m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
					g_pLocalUser->m_bCheckFirstFrame = FALSE;
					
					g_pLocalUser->m_bUsingSkill = FALSE;
					g_pLocalUser->m_bCheckDelayTime = TRUE;
					
					// delay time ��� ����
					g_pLocalUser->m_ChangeDelayStartTime = g_LocalSystemTime;
				}
*/
				g_pLocalUser->m_CurrentWeaponID = index;
				g_pLocalUser->ResetSkillAnimation();
				g_pLocalUser->m_NextSkillID = 0;
				
				// ������ ���� ��ų ������ ����
				if(g_pQuickSlot_Window)
				{
					g_pQuickSlot_Window->SetCurrentSelectedActiveIndex(-1);
					g_pQuickSlot_Window->SetCurrentSelectedOtherIndex(-1);
					
					if( g_pLocalUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON)
					{
						if( (g_WeaponItemProperty[index].cSecondType != g_SkillProperty[g_pLocalUser->m_ActiveSlotID]->weaponCType) ||			
						(g_WeaponItemProperty[index].cThirdType  != g_SkillProperty[g_pLocalUser->m_ActiveSlotID]->weaponSType ) )
						{
							g_pLocalUser->m_ActiveSlotID = 0;
						}
					}
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2)
					{
						if( (g_WeaponItemProperty2[index].cSecondType != g_SkillProperty[g_pLocalUser->m_ActiveSlotID]->weaponCType) ||			
						(g_WeaponItemProperty2[index].cThirdType  != g_SkillProperty[g_pLocalUser->m_ActiveSlotID]->weaponSType ) )
						{
							g_pLocalUser->m_ActiveSlotID = 0;
						}
					}
					else if( g_pLocalUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3)
					{
						if( (g_WeaponItemProperty3[index].cSecondType != g_SkillProperty[g_pLocalUser->m_ActiveSlotID]->weaponCType) ||			
						(g_WeaponItemProperty3[index].cThirdType  != g_SkillProperty[g_pLocalUser->m_ActiveSlotID]->weaponSType ) )
						{
							g_pLocalUser->m_ActiveSlotID = 0;
						}
					}
				}
				
				g_pLocalUser->m_SelectedSkillID = 0;
				g_pLocalUser->m_ActiveSlotID = 0;
				g_pLocalUser->m_SelectedSkillItem = NULL;		
			}		
			
	//		g_pLocalUser->UpdateLocalUserAttackLevel();

			break;	
		}

		// ���� ���� ������ ���� �ִϸ��̼��� ��ü���� �ʴ´�.
		if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_NONE )
			g_pLocalUser->SetAnimation( g_pLocalUser->m_CharacterInfo.animationdescript.motionclass );
	}
}


void XProc_MainGame::InventoryMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	// Inventory B.U.T.T.O.N Message Handling ==========================================
	case _XDEF_INV_CLOSEBUTTON :		
		{
			if(g_pInventory_Window)
			{
				if( m_bNPCDialogMode || m_bNPCDialogWideView || m_bFunctionalObjectWideView )
				{
					if(g_pInventory_Window->m_bHaveMouseSlot)
					{
						g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
					}
					
					_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHARGEGAUGE);
					if( pChargeGauge_Window )
					{
						if( !pChargeGauge_Window->GetShowStatus() )
						{
							if(m_pNPCTradeWindowPtr)
							{
								if(m_pNPCTradeWindowPtr->GetShowStatus())
								{
									m_pNPCTradeWindowPtr->ShowWindow(FALSE);
									_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
									if( pTradeMessageBox )
										g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
								}
							}
							
							if(m_pWarehouseWindowPtr)
							{
								if(m_pWarehouseWindowPtr->GetShowStatus())
									m_pWarehouseWindowPtr->ShowWindow(FALSE);
							}			
							
							if( m_pContributionWindowPtr )
							{
								if( m_pContributionWindowPtr->GetShowStatus() )
									m_pContributionWindowPtr->ShowWindow(FALSE);							
							}
							
							if(m_pRepairItemWindowPtr)
							{
								if(m_pRepairItemWindowPtr->GetShowStatus())
								{
									m_pRepairItemWindowPtr->ShowWindow(FALSE);
								}
							}
							if( m_pSocketPlugInExWindowPtr )
							{
								if( m_pSocketPlugInExWindowPtr->GetShowStatus() )
									m_pSocketPlugInExWindowPtr->ShowWindow(FALSE);
							}
							if( m_pInchantWindowPtr )
							{
								if( m_pInchantWindowPtr->GetShowStatus() )
								{
									m_pInchantWindowPtr->ShowWindow(FALSE);
									_XMessageWindow* pInchantMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INCHANTMESSAGEWINDOW);
									if( pInchantMessageBox )
									{
										g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_INCHANTMESSAGEWINDOW);
									}
								}
							}
							g_pInventory_Window->ShowWindow( FALSE );
							SetNPCMode( _XNPC_NONE );
						}
						else
						{
							g_MainWindowManager.SetTopWindow(pChargeGauge_Window);
						}
					}					
				}
				else
				{
					/*_XWindow_SocketPlugIn* pSocketPlugin = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN);
					if( pSocketPlugin )
					{
						if( pSocketPlugin->GetShowStatus() )
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_SOCKETPLUGIN_CLOSE, 0), 0 );
							break;
						}
					}

					_XWindow_Gamble* pGamble_Window = (_XWindow_Gamble*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GAMBLE);
					if( pGamble_Window )
					{
						if( pGamble_Window->GetShowStatus() )
						{
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_GAMBLE_CLOSE, 0), 0 );
							break;
						}
					}*/

					g_pInventory_Window->ShowWindow( FALSE );
					if(m_pPCTradeWindowPtr)
					{
						if(m_pPCTradeWindowPtr->GetShowStatus())
						{
							g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_CANCEL);
							m_pPCTradeWindowPtr->ShowWindow(FALSE);
						}
					}
				}
			}
		}
		break;
		
		// Base tab ----------------------------------------------------------------------------------------------------------
	case _XDEF_INV_BASETAB_POCKET :
		{			
			if(g_pInventory_Window)
			{
				g_pInventory_Window->ChangeBaseTab(_XINVENTORYBASETAB_POCKET);
			}
		}
		break;
	case _XDEF_INV_BASETAB_BACKPACK :
		{			
			if( g_pInventory_Window )
			{
				g_pInventory_Window->ChangeBaseTab(_XINVENTORYBASETAB_BACKPACK);
			}
		}
		break;
	case _XDEF_INV_BASETAB_BACKPACK2 :
		{			
			if( g_pInventory_Window )
			{
				g_pInventory_Window->ChangeBaseTab(_XINVENTORYBASETAB_BACKPACK2);
			}
		}
		break;
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	case _XDEF_INV_BASETAB_CASHBACKPACK1 : //Author : ����� //breif : �೶ Ȯ��
		{
			if( g_pInventory_Window )
			{
				g_pInventory_Window->ChangeBaseTab(_XINVENTORYBASETAB_CASHBACKPACK1);
			}
		}
		break;
	case _XDEF_INV_BASETAB_CASHBACKPACK2 :
		{
			if( g_pInventory_Window )
			{
				g_pInventory_Window->ChangeBaseTab(_XINVENTORYBASETAB_CASHBACKPACK2);
			}
		}
		break;
#else
	case _XDEF_INV_CHARINFO :
		{
			if( g_pInventory_Window )
			{
				g_pInventory_Window->ChangeBaseTab(_XINVENTORYBASETAB_CHARINFO);
			}
		}
		break;
#endif
	case _XDEF_INV_INVENTORYMONEY : // �����ݾ� ��ư
		{
			if(g_pInventory_Window)
			{
				if(g_pInventory_Window->GetShowStatus())
				{
					_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
					if( m_pPCTradeWindowPtr && m_pPCTradeWindowPtr->GetShowStatus() )	// �Ǿ��� �ŷ�â ������
					{
						if(pTradeConfirm_Window)
						{
							pTradeConfirm_Window->ShowWindow(TRUE);
							pTradeConfirm_Window->m_IMEControl.SetFocus();
							pTradeConfirm_Window->m_TradeMode = _XPCTRADE_TOTRADE_MONEY;
						}
					}
					else if( m_pContributionWindowPtr && m_pContributionWindowPtr->GetShowStatus() )	// ���â ������
					{
						if( pTradeConfirm_Window )
						{
							pTradeConfirm_Window->ShowWindow(TRUE);
							pTradeConfirm_Window->m_IMEControl.SetFocus();
							pTradeConfirm_Window->m_TradeMode = _XCONTRIBUTION_TOCONTRIBUTION_MONEY;
						}
					}
					else if( m_pWarehouseWindowPtr && m_pWarehouseWindowPtr->GetShowStatus() )		// â��â ������
					{
						// 2004.06.01->oneway48 insert
						if( m_pWarehouseWindowPtr->m_bUseableWarehouse )
						{
							_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
							if(pTradeConfirm_Window)
							{
								pTradeConfirm_Window->ShowWindow(TRUE);
								pTradeConfirm_Window->m_IMEControl.SetFocus();
								pTradeConfirm_Window->m_TradeMode = _XWAREHOUSE_TOWAREHOUSE_MONEY;
							}
						}
						else
						{	
							// â�� �߰��� �ؾ��Ѵ�.				
							m_pNPCScriptWindowPtr->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_WAREHOUSEADD), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);		// "â�� �߰����� â�� ��� ����� �Ͻʽÿ�."
							
						}
					}
					else		// ������
					{
#ifdef _XDEF_MONEYDROP_NO_20070102
#else
						if(pTradeConfirm_Window)
						{
							pTradeConfirm_Window->ShowWindow(TRUE);
							pTradeConfirm_Window->m_IMEControl.SetFocus();
							pTradeConfirm_Window->m_TradeMode = _XDROP_MONEY;
						}
#endif
					}					
				}
			}
		}
		break;
	case _XDEF_INV_HELPBUTTON :
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_INVEN );
		}
		break;
	case _XDEF_INV_USEELIXIRYES :
		{
			g_pLocalUser->m_CurrentInternalUseState = _XUSER_STATE_1;
		} 
		break;
	case _XDEF_INV_USEELIXIRNO :
		{
			if( g_pInventory_Window )
			{				
				g_pLocalUser->m_bInternalUse = FALSE;
				g_pInventory_Window->m_ElixirInvenNumber = -1;
			}
		}
		break;
	case _XDEF_INV_USEBOOKYES :
		{
			if( g_pInventory_Window )
			{
				g_pLocalUser->m_CurrentReadBookState = _XUSER_STATE_1;
			}			
		}
		break;
	case _XDEF_INV_USEBOOKNO :
		{
			if( g_pInventory_Window )
			{
				g_pLocalUser->m_bReadingBook = FALSE;
				g_pInventory_Window->m_BookInvenNumber = -1;
			}
		}
		break;
	case _XDEF_INV_CASHITEMBUTTON :
		{
			_XWindow_CashItem* pCashItem_Window = (_XWindow_CashItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASHITEM);
			if( pCashItem_Window )
			{
				if( pCashItem_Window->GetShowStatus() )
					pCashItem_Window->ShowWindow(FALSE);
				else
					pCashItem_Window->ShowWindow(TRUE);
			}
		}
		break;
		
#ifdef _XDEF_WASTEBASKET_20070226
	case _XDEF_INV_WASTBASKET_CANCLE			:
		{
			g_pInventory_Window->m_bWasteBasketLock = FALSE; //Author : ����� //breif : �������� �̹��� ��
			g_pInventory_Window->m_bHaveMouseSlot = FALSE;
			g_pInventory_Window->m_bDragState = FALSE;
			g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
		}
		break;
	case _XDEF_INV_WASTBASKET_CHECKPASSWOAD		:
		{
			g_MessageBox.SetPasswordMode( TRUE );
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_INPUTSTRING, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3335),TRUE, _XDEF_INV_WASTBASKET_CASHITEM,_XDEF_INV_WASTBASKET_CANCLE);
			
			g_MessageBox.m_IMEControl.m_VKeyboardTypedCount = 0;
			g_MessageBox.m_IMEControl.SetMaxLength(35);
			g_MessageBox.m_IMEControl.ResizeWindow(120, 13);
			g_MessageBox.m_IMEControl.SetWhiteRectAngle(TRUE);
			g_MessageBox.m_IMEControl.ShowWindow(TRUE);
			
			g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		}
		break;
	case _XDEF_INV_WASTBASKET_CASHITEM			:
		{
			if( strcmp(g_pLocalUser->m_UserPW,g_MessageBox.m_IMEControl.GetText()) != 0 )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3336), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				
				g_pInventory_Window->m_bWasteBasketLock = FALSE; //Author : ����� //breif : �������� �̹��� ��
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_bDragState = FALSE;
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
				break;
			}
			
			if(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].IsStackable())
			{
				_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
				if(pTradeConfirm_Window)
				{
					pTradeConfirm_Window->ShowWindow(TRUE);
					pTradeConfirm_Window->m_SelectedCategory1 = g_pInventory_Window->m_MouseSlotItem.Get_m_cType();
					pTradeConfirm_Window->m_SelectedItemID = g_pInventory_Window->m_MouseSlotItem.Get_m_sID();
					pTradeConfirm_Window->m_TradeMode = _XDROP_WASTEBASKET_ITEM;
				}			
			}
			else
			{
				if( g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber != -1)
					g_NetworkKernel.SendPacket(MSG_NO_INVEN_DROP, g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber, 1, 10);
				
				g_pInventory_Window->m_bWasteBasketLock = FALSE; //Author : ����� //breif : �������� �̹��� ��
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_bDragState = FALSE;
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
			}						
		}
		break;
	case _XDEF_INV_WASTBASKET_NORMALITEM		:
		{
			if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE ) // ������ ���õ� �ൿ�� �ϰ� ���� ��
			{				
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1821), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );

				g_pInventory_Window->m_bWasteBasketLock = FALSE; //Author : ����� //breif : �������� �̹��� ��
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_bDragState = FALSE;
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
				break;
			}
			else if( g_pLocalUser->m_CharacterLog.isTrade )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1821), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );

				g_pInventory_Window->m_bWasteBasketLock = FALSE; //Author : ����� //breif : �������� �̹��� ��
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_bDragState = FALSE;
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
				break;
			}						
			else if( g_pLocalUser->m_bGambleBoxOpen )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1821), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	

				g_pInventory_Window->m_bWasteBasketLock = FALSE; //Author : ����� //breif : �������� �̹��� ��
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_bDragState = FALSE;
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
				break;
			}
			else if( g_pLocalUser->m_bReadingBook )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1821), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	

				g_pInventory_Window->m_bWasteBasketLock = FALSE; //Author : ����� //breif : �������� �̹��� ��
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_bDragState = FALSE;
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
				break;
			}
			else if( g_pLocalUser->m_bInternalUse )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1821), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	

				g_pInventory_Window->m_bWasteBasketLock = FALSE; //Author : ����� //breif : �������� �̹��� ��
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_bDragState = FALSE;
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
				break;
			}

			if(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].IsStackable())
			{
				_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
				if(pTradeConfirm_Window)
				{
					pTradeConfirm_Window->ShowWindow(TRUE);
					pTradeConfirm_Window->m_SelectedCategory1 = g_pInventory_Window->m_MouseSlotItem.Get_m_cType();
					pTradeConfirm_Window->m_SelectedItemID = g_pInventory_Window->m_MouseSlotItem.Get_m_sID();
					pTradeConfirm_Window->m_TradeMode = _XDROP_WASTEBASKET_ITEM;
				}			
			}
			else
			{
				if( g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber != -1)
					g_NetworkKernel.SendPacket(MSG_NO_INVEN_DROP, g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber, 1, 10);
				
				g_pInventory_Window->m_bWasteBasketLock = FALSE; //Author : ����� //breif : �������� �̹��� ��
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_bDragState = FALSE;
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
			}					
		}
		break;
#endif
	}
}

void XProc_MainGame::PartyWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_PARTYWINDOW_CLOSE :
		{
			_XWindow_Party* partywindow = (_XWindow_Party *)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PARTY );
			if( partywindow )
			{
				// partynameEdit�� focus�� �ް� ������ _XDEF_PARTYWINDOW_PARTYNAMEEDIT �� �Ȱ��� ó���� ���ش�.
				
				partywindow->m_PartyTab[0]->EnableWindow(TRUE);
				partywindow->m_PartyTab[1]->EnableWindow(TRUE);
				partywindow->m_PartyTab[2]->EnableWindow(TRUE);
				partywindow->m_PartyTab[3]->EnableWindow(TRUE);				
								
				partywindow->ShowWindow( FALSE );
			}
		}
		break;

	case _XDEF_PARTYWINDOW_TAB_BANGHOI :
		{
			_XWindow_Party* partywindow = (_XWindow_Party *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PARTY);
			if(partywindow)
			{
				// play interface sound
				partywindow->ChangeTab(_XPARTYTAB_BANGHOI);
			}
		}
		break;

	case _XDEF_PARTYWINDOW_TAB_SEGA :
		{
			_XWindow_Party* partywindow = (_XWindow_Party *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PARTY);
			if(partywindow)
			{
				// play interface sound
				partywindow->ChangeTab(_XPARTYTAB_SEGA);
			}
		}
		break;
		
	case _XDEF_PARTYWINDOW_TAB_FRIEND :
		{
			_XWindow_Party* partywindow = (_XWindow_Party *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PARTY);
			if(partywindow)
			{
				// play interface sound
				partywindow->ChangeTab(_XPARTYTAB_FRIEND);
			}
		}
		break;
	
	case _XDEF_PARTYWINDOW_TAB_PARTY :
		{
			_XWindow_Party* partywindow = (_XWindow_Party *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PARTY);
			if(partywindow)
			{
				// play interface sound
				partywindow->ChangeTab(_XPARTYTAB_PARTY);
			}
		}
		break;	

	case _XDEF_PARTYWINDOW_MASTERMINI :
		{
			// ������ �̴� ��Ƽ ������ ����ֱ�
			BOOL check = (BOOL)lparam;
			_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
			
			if(check)
			{
				for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) // 2004.06.01->oneway48 modify : 15->9
				{
					if(g_pLocalUser->m_PartyInfo[i].isLeader )
						break;
				}
				if(i < _XDEF_MAXPARTYMEMBERCOUNT)
				{
					BOOL isCurrentUser = FALSE;
					pMiniBar_Window->SetPartyMasterName(i, isCurrentUser);
					pMiniBar_Window->ShowWindow(TRUE);
				}	
			}
			else
			{
				pMiniBar_Window->ShowWindow(FALSE);
			}
		}
		break;
		
		case _XDEF_PARTYWINDOW_LEAVE :
			{				
				if( g_pLocalUser->m_bIsBossBattle )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1842), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
					break;
				}
				else if( g_pLocalUser->m_bPartyQuestProgress )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1843), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
					break;
				}
				else
				{
					bool isbossroom = false;
					for( int i = 0; i <  _XDEF_MAXPARTYMEMBERCOUNT ; i++ )
					{
						switch(g_pLocalUser->m_PartyInfo[i].serverno) {
						case 50://���ֺ�����
						case 51://�����庸����
						case 52://�뵿������
						case 53://����������
						case 54://���ֳ�â
						case 55://���ֳ�â
						case 56://���ֳ�â
						case 57://���ֳ�â
							{
								isbossroom = true;
							}
							break;
						}
						if( isbossroom ) break;						
					}
					
					if( isbossroom )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1844), TRUE);
						pMessageBox->StartTimer(5000, TRUE) ;
						break;
					}				
				}

				if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(messagestring) );
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1151));	// "�������� �����Ͻð� Ż���Ͻʽÿ�."
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				else
				{
					// Ż��ó��
					_XWindow_Party* pParty_Window = (_XWindow_Party*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PARTY);
					if(pParty_Window)
					{	
						g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC, 5, g_pLocalUser->m_PartySelfIndex);
					}
				}
			}
			break;

		case _XDEF_PARTYWINDOW_DISSOLUTION :
			{
				if( g_pLocalUser->m_bIsBossBattle )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1845), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
					break;
				}
				else if( g_pLocalUser->m_bPartyQuestProgress )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1846), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
					break;
				}
				else
				{
					bool isbossroom = false;
					for( int i = 0; i <  _XDEF_MAXPARTYMEMBERCOUNT ; i++ )
					{
						switch(g_pLocalUser->m_PartyInfo[i].serverno) {
						case 50://���ֺ�����
						case 51://�����庸����
						case 52://�뵿������
						case 53://����������
							{
								isbossroom = true;
							}
							break;
						}
						if( isbossroom ) break;						
					}

					if( isbossroom )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1847), TRUE);
						pMessageBox->StartTimer(5000, TRUE) ;
						break;
					}				
				}

				if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
				{					
					// ��Ƽ ��ü ó��
					g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC, 0, 0);
				}
				else
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);

					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(messagestring) );
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_DONOTDESTROYPARTY));		// "������ ��ü�� ������ �����ϴ�."
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
			}
			break;
			
		case _XDEF_PARTYWINDOW_USERMINI :
			{
				// ������ �̴� ��Ƽ ������
				_XWindow_Party* pParty_Window = (_XWindow_Party*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PARTY);
				if(pParty_Window)
				{
					BOOL check = (BOOL)lparam;
					
					int selectedpos = pParty_Window->m_PartyList->GetSelectedItem();
					if( selectedpos >= 0 )
					{
						DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER2 + selectedpos;
						_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
						if(pMiniBar_Window)
						{
							if(check)
							{
								TCHAR partyplayername[13];
								memset( partyplayername, 0, sizeof(TCHAR)*13 );
								strcpy(partyplayername, pParty_Window->m_PartyList->GetItemText(selectedpos,1) );
								
								for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) 
								{
									if( strcmp( g_pLocalUser->m_PartyInfo[i].playername, partyplayername ) == 0 )
									{
										BOOL isCurrentUser = FALSE;
										pMiniBar_Window->SetPartyPlayerName(i, isCurrentUser);
										pMiniBar_Window->SetPartyPlayerVital(i, isCurrentUser);
										//if(!isCurrentUser)
										pMiniBar_Window->ShowWindow(TRUE);
										break;
									}
								}
							}
							else
							{
								pMiniBar_Window->ShowWindow(FALSE);
							}
						}
					}
				}
			}
			break;
			
		case _XDEF_PARTYWINDOW_USERSECESSION :
			{
				if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
				{
					
					if( g_pLocalUser->m_bIsBossBattle )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1848), TRUE);
						pMessageBox->StartTimer(5000, TRUE) ;
						break;
						
					}
					else if( g_pLocalUser->m_bPartyQuestProgress )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1849), TRUE);
						pMessageBox->StartTimer(5000, TRUE) ;
						break;
					}
					else
					{
						bool isbossroom = false;
						for( int i = 0; i <  _XDEF_MAXPARTYMEMBERCOUNT ; i++ )
						{
							switch(g_pLocalUser->m_PartyInfo[i].serverno) {
							case 50://���ֺ�����
							case 51://�����庸����
							case 52://�뵿������
							case 53://����������
								{
									isbossroom = true;
								}
								break;
							}
							if( isbossroom ) break;						
						}
						
						if( isbossroom )
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1850), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
							break;
						}				
					}
					
					// ��Ƽ�� ����
					_XWindow_Party* pParty_Window = (_XWindow_Party*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PARTY);
					if(pParty_Window)
					{
						int selectedpos = pParty_Window->m_PartyList->GetSelectedItem();
						if( selectedpos >= 0 )
						{
							TCHAR partyplayername[13];
							memset( partyplayername, 0, sizeof(TCHAR)*13 );
							strcpy(partyplayername, pParty_Window->m_PartyList->GetItemText(selectedpos,1) );
							
							for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) 
							{
								if( strcmp( g_pLocalUser->m_PartyInfo[i].playername, partyplayername ) == 0 )
								{
									g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC, 6, i);
									break;
								}
							}
						}
					}
				}
				else
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);

					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(messagestring) );
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_DONOTTRUSTCAPTAIN));		// "�������� ������ ������ �����ϴ�."
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
			}
			break;
			
		case _XDEF_PARTYWINDOW_USERLEAVE :
			{				
				if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
				{
					if( g_pLocalUser->m_bIsBossBattle )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1851), TRUE);
						pMessageBox->StartTimer(5000, TRUE) ;
						break;
					}
					else if( g_pLocalUser->m_bPartyQuestProgress )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1849), TRUE);
						pMessageBox->StartTimer(5000, TRUE) ;
						break;
					}					
					else
					{
						bool isbossroom = false;
						for( int i = 0; i <  _XDEF_MAXPARTYMEMBERCOUNT ; i++ )
						{
							switch(g_pLocalUser->m_PartyInfo[i].serverno) {
							case 50://���ֺ�����
							case 51://�����庸����
							case 52://�뵿������
							case 53://����������
								{
									isbossroom = true;
								}
								break;
							}
							if( isbossroom ) break;						
						}
						
						if( isbossroom )
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1852), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
							break;
						}				
					}
					// �߹�
					_XWindow_Party* pParty_Window = (_XWindow_Party*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PARTY);
					if(pParty_Window)
					{
						int selectedpos = pParty_Window->m_PartyList->GetSelectedItem();
						if( selectedpos >= 0 )
						{
							TCHAR partyplayername[13];
							memset( partyplayername, 0, sizeof(TCHAR)*13 );
							strcpy(partyplayername, pParty_Window->m_PartyList->GetItemText(selectedpos,1) );
							
							for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) 
							{
								if( strcmp( g_pLocalUser->m_PartyInfo[i].playername, partyplayername ) == 0 )
								{
									g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC, 4, i);
									break;
								}
							}
							//g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC, 4, pParty_Window->m_SelectedPartyMember);
						}
					}
				}
				else
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);

					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(messagestring) );
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_DONOTDROP));		// "������ �߹��� ������ �����ϴ�."
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
			}
			break;
			
		case _XDEF_PARTYWINDOW_LISTBOX :
			{
				_XWindow_Party* pParty_Window = (_XWindow_Party*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PARTY);
				if(pParty_Window)
				{
					pParty_Window->m_SelectedPartyMember = (int)lparam;
				}
			}
			break;

	// Mini party member info window-----------------------------------------------------------------------------------
	case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON1		:
		{
			if( g_pLocalUser->m_bIsBossBattle )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1845), TRUE);
				pMessageBox->StartTimer(5000, TRUE) ;
				break;
			}
			else if( g_pLocalUser->m_bPartyQuestProgress )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1846), TRUE);
				pMessageBox->StartTimer(5000, TRUE) ;
				break;
			}					
			else
			{
				bool isbossroom = false;
				for( int i = 0; i <  _XDEF_MAXPARTYMEMBERCOUNT ; i++ )
				{
					switch(g_pLocalUser->m_PartyInfo[i].serverno) {
					case 50://���ֺ�����
					case 51://�����庸����
					case 52://�뵿������
					case 53://����������
						{
							isbossroom = true;
						}
						break;
					}
					if( isbossroom ) break;						
				}
				
				if( isbossroom )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1847), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
					break;
				}				
			}	
			// ��Ƽ ��ü ó��
			// ��Ƽ�� ��ü�ϰڳİ� ���´�.
			if ( g_pLocalUser->m_PartyMemberIndex != -1) break;
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_DESTROYPARTY));	// "������ �ػ��Ͻðڽ��ϱ�?"
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_MINIPARTYWINDOW_YES_PARTYDISSOLUTION, _XDEF_MINIPARTYWINDOW_NO_PARTYREQUEST);	
					
		}
		break;
		
	case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON2		:	
	case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON3		:
	case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON4		:	
	case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON5		:	
	case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON6		:	
	case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON7		:	
	case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON8		:	
	case _XDEF_MINIPARTYWINDOW_CLOSEBUTTON9		:	
		{
			if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
			{				
				if( g_pLocalUser->m_bIsBossBattle )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1853), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
					break;
				}
				else if( g_pLocalUser->m_bPartyQuestProgress )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1854), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
					break;
				}				
				else
				{
					bool isbossroom = false;
					for( int i = 0; i <  _XDEF_MAXPARTYMEMBERCOUNT ; i++ )
					{
						switch(g_pLocalUser->m_PartyInfo[i].serverno) {
						case 50://���ֺ�����
						case 51://�����庸����
						case 52://�뵿������
						case 53://����������
							{
								isbossroom = true;
							}
							break;
						}
						if( isbossroom ) break;						
					}
					
					if( isbossroom )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1855), TRUE);
						pMessageBox->StartTimer(5000, TRUE) ;
						break;
					}				
				}
				
				if ( g_pLocalUser->m_PartyMemberIndex != -1) break;
				// ��Ƽ���� �����ư 
				DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER2 + (wparam - _XDEF_MINIPARTYWINDOW_CLOSEBUTTON2);
				
				_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);			
				if(pMiniBar_Window)
				{
					for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) 
					{
						if( !g_pLocalUser->m_PartyInfo[i].isLeader )
						{
							if( strcmp( g_pLocalUser->m_PartyInfo[i].playername, pMiniBar_Window->m_PartyMemberName ) == 0 )
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								
								TCHAR messagestring[256];
								memset( messagestring, 0, sizeof(messagestring) );
								_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_MEMVERDROP));	// "�� ������� ���࿡�� �����Ű�ڽ��ϱ�?"
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_MINIPARTYWINDOW_YES_PARTYMEMBERLEAVE, _XDEF_MINIPARTYWINDOW_NO_PARTYREQUEST);
								g_pLocalUser->m_PartyMemberIndex = i;
								
								break;
							}
						}
					}
				}
				break;
			
			}
			else
			{
				if( g_pLocalUser->m_bIsBossBattle )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1842), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
					break;
				}
				else if( g_pLocalUser->m_bPartyQuestProgress )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1856), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
					break;
				}			
				else
				{
					bool isbossroom = false;
					for( int i = 0; i <  _XDEF_MAXPARTYMEMBERCOUNT ; i++ )
					{
						switch(g_pLocalUser->m_PartyInfo[i].serverno) {
						case 50://���ֺ�����
						case 51://�����庸����
						case 52://�뵿������
						case 53://����������
							{
								isbossroom = true;
							}
							break;
						}
						if( isbossroom ) break;						
					}
					
					if( isbossroom )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1857), TRUE);
						pMessageBox->StartTimer(5000, TRUE) ;
						break;
					}				
				}


				DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER2 + (wparam - _XDEF_MINIPARTYWINDOW_CLOSEBUTTON2);
				
				_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);			
				if(pMiniBar_Window)
				{
					if( strcmp(pMiniBar_Window->m_PartyMemberName, g_pLocalUser->m_CharacterInfo.charactername) == 0 )
					{
						if ( g_pLocalUser->m_PartyMemberIndex != -1) break;
						// ��Ƽ Ż��						
						int partyindex = -1;
						for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) 
						{
							if( strcmp( g_pLocalUser->m_PartyInfo[i].playername, g_pLocalUser->m_CharacterInfo.charactername ) == 0 )
							{
								partyindex = i;
								break;
							}				
						}
						
						if( partyindex != -1 )
						{
							// Ż��ó��
							// Ż���ϰڳİ� ���´�.
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							
							TCHAR messagestring[256];
							memset( messagestring, 0, sizeof(messagestring) );
							_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_SELFDROP));		// "���࿡�� Ż���Ͻðڽ��ϱ�?"
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_MINIPARTYWINDOW_YES_PARTYLEAVE, _XDEF_MINIPARTYWINDOW_NO_PARTYREQUEST);
							
							g_pLocalUser->m_PartyMemberIndex = partyindex;
						}
					}
					else
					{
						pMiniBar_Window->ShowWindow(FALSE);
					}
				}		
				break;
				
			}
		}
		break;
		
		// ���ӹ�ư
	case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON1		:	
	case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON2		:
	case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON3		:	
	case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON4		:	
	case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON5		:	
	case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON6		:	
	case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON7		:	
	case _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON8		:	
		{
			
			if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
			{
				if( g_pLocalUser->m_bIsBossBattle )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1848), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
					break;
					
				}
				else if( g_pLocalUser->m_bPartyQuestProgress )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1849), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
					break;
				}					
				else
				{
					bool isbossroom = false;
					for( int i = 0; i <  _XDEF_MAXPARTYMEMBERCOUNT ; i++ )
					{
						switch(g_pLocalUser->m_PartyInfo[i].serverno) {
						case 50://���ֺ�����
						case 51://�����庸����
						case 52://�뵿������
						case 53://����������
							{
								isbossroom = true;
							}
							break;
						}
						if( isbossroom ) break;						
					}
					
					if( isbossroom )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1850), TRUE);
						pMessageBox->StartTimer(5000, TRUE) ;
						break;
					}				
				}
				
				if ( g_pLocalUser->m_PartyMemberIndex != -1) break;
				// ��Ƽ�� ����
				DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER2 + (wparam - _XDEF_MINIPARTYWINDOW_ENTRUSTBUTTON1);
				_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
				if(pMiniBar_Window)
				{
					for( int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) 
					{
						if( !g_pLocalUser->m_PartyInfo[i].isLeader )
						{				
							if( strcmp( g_pLocalUser->m_PartyInfo[i].playername, pMiniBar_Window->m_PartyMemberName ) == 0 )
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								
								TCHAR messagestring[256];
								memset( messagestring, 0, sizeof(messagestring) );
								_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_TRUSCAPTAIN));		// "�� ����� ����� �����Ͻðڽ��ϱ�?"
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_MINIPARTYWINDOW_YES_PARTYMASTERENTRUST, _XDEF_MINIPARTYWINDOW_NO_PARTYREQUEST);
								
								g_pLocalUser->m_PartyMemberIndex = i;
								break;
							}								
						}
					}
				}
			}
			else
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				
				TCHAR messagestring[256];
				memset( messagestring, 0, sizeof(messagestring) );
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_DOCAPTAIN));		// "���ӱ���� ����� ��� �����մϴ�."
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
				pMessageBox->StartTimer(5000, TRUE) ;
			}
			
		}
		break;
		
	case _XDEF_MINIPARTYWINDOW_MINIMIZEBUTTON	:	// partymaster
		{
			_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);			
			if(pMiniBar_Window)
			{
				pMiniBar_Window->SetMode(_XPARTYMASTERMODE_MINIMUM);
				
				g_pLocalUser->m_PartyMemberCount = 0;
				for( int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++)
				{						
					if( g_pLocalUser->m_PartyInfo[i].playeruniqueid != 0 ) g_pLocalUser->m_PartyMemberCount++;
					DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER2 + i;
					_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
					if(pMiniBar_Window)
					{
						if(pMiniBar_Window->GetShowStatus())
							pMiniBar_Window->ShowWindow(FALSE);
					}
				}
			}						
		}
		break;
	case _XDEF_MINIPARTYWINDOW_MAXIMIZEBUTTON	:	// partymaster
		{
			_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);			
			if(pMiniBar_Window)
			{
				pMiniBar_Window->SetMode(_XPARTYMASTERMODE_MAXIMUM);
				
				int ypos = 0 ;			
				int count = 0;

			#ifdef _ACCLAIM_RUBICONADSYSTEM	
				if( g_ViewRubiconAD)
					ypos = g_RubiconADWindowSize.cy;
			#endif

				for( int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) 
				{
					if( g_pLocalUser->m_PartyInfo[i].isLeader )
					{							
						if(g_pLocalUser->m_PartyInfo[i].playername[0] != 0)
						{
							BOOL isCurrentUser = FALSE;
							pMiniBar_Window->SetPartyMasterName(i, isCurrentUser);
							pMiniBar_Window->SetPartyMasterVital(i, isCurrentUser);
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
						#ifdef _ACCLAIM_RUBICONADSYSTEM	
							pMiniBar_Window->MoveWindow( 50, 114+((g_ViewRubiconAD)?g_RubiconADWindowSize.cy:0) );							
						#else
							pMiniBar_Window->MoveWindow( 50, 114);
						#endif
#else							
						#ifdef _ACCLAIM_RUBICONADSYSTEM	
							pMiniBar_Window->MoveWindow( 14, 114 +((g_ViewRubiconAD)?g_RubiconADWindowSize.cy:0) );
						#else
							pMiniBar_Window->MoveWindow( 14, 114);
						#endif
#endif
							
							pMiniBar_Window->ShowWindow(TRUE);
						}
					}
					else
					{						
						DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER2 + count;
						count++;
						_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
						if(pMiniBar_Window)
						{
							
							if(g_pLocalUser->m_PartyInfo[i].playername[0] != 0)
							{
								BOOL isCurrentUser = FALSE;
								pMiniBar_Window->SetPartyPlayerName(i, isCurrentUser);
								pMiniBar_Window->SetPartyPlayerVital(i, isCurrentUser);
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
								pMiniBar_Window->MoveWindow( 50, 170 + ypos );
#else
								pMiniBar_Window->MoveWindow( 14, 170 + ypos);
#endif
								ypos += 36;							
								
								pMiniBar_Window->ShowWindow(TRUE);
							}
						}
					}
				}
			}			
		}
		break;
	case _XDEF_MINIPARTYWINDOW_EXPSHARINGBUTTON	:	// partymaster
		{
			if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
			{
				_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);			
				if(pMiniBar_Window)
				{
					if( pMiniBar_Window->m_listboxPartyExperienceSharing->GetShowStatus() )
					{
						pMiniBar_Window->m_listboxPartyExperienceSharing->ShowWindow( FALSE );
					}
					else
					{
						pMiniBar_Window->m_listboxPartyItemSharing->ShowWindow( FALSE );
						pMiniBar_Window->m_listboxPartyExperienceSharing->ShowWindow( TRUE );
						g_MainWindowManager.SetTopWindow(pMiniBar_Window);
					}
				}
			}
		}
		break;
	case _XDEF_MINIPARTYWINDOW_ITEMSHARINGBUTTON:	// partymaster
		{
			if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
			{
				_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);			
				if(pMiniBar_Window)
				{
					if( pMiniBar_Window->m_listboxPartyItemSharing->GetShowStatus() )
					{
						pMiniBar_Window->m_listboxPartyItemSharing->ShowWindow( FALSE );
					}
					else
					{
						pMiniBar_Window->m_listboxPartyExperienceSharing->ShowWindow( FALSE );
						pMiniBar_Window->m_listboxPartyItemSharing->ShowWindow( TRUE );
						g_MainWindowManager.SetTopWindow(pMiniBar_Window);
					}
				}
			}
		}
		break;
	case _XDEF_MINIPARTYWINDOW_YES_PARTYDISSOLUTION:	// ��Ƽ��ü 
		{
			g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC, 0, 0);
			g_pLocalUser->m_PartyMemberIndex = -1;
		}
		break;
	case _XDEF_MINIPARTYWINDOW_YES_PARTYMEMBERLEAVE:	// ��Ƽ�� ����
		{
			g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC, 4, g_pLocalUser->m_PartyMemberIndex);
			g_pLocalUser->m_PartyMemberIndex = -1;
		}
		break;
	case _XDEF_MINIPARTYWINDOW_YES_PARTYLEAVE:	// ���� Ż��
		{
			g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC, 5, g_pLocalUser->m_PartyMemberIndex);
			g_pLocalUser->m_PartyMemberIndex = -1;
		}
		break;
	case _XDEF_MINIPARTYWINDOW_YES_PARTYMASTERENTRUST:	// ��� ����
		{
			g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC, 6, g_pLocalUser->m_PartyMemberIndex);
			g_pLocalUser->m_PartyMemberIndex = -1;
		}
		break;		
	case _XDEF_MINIPARTYWINDOW_EXPSHAERINGLISTBOX:	// partymaster
		{
			if( g_pLocalUser->m_bIsBossBattle )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1858), TRUE);
				pMessageBox->StartTimer(5000, TRUE) ;
				break;
				
			}
			else
			{
				bool isbossroom = false;
				for( int i = 0; i <  _XDEF_MAXPARTYMEMBERCOUNT ; i++ )
				{
					switch(g_pLocalUser->m_PartyInfo[i].serverno) {
					case 50://���ֺ�����
					case 51://�����庸����
					case 52://�뵿������
					case 53://����������
						{
							isbossroom = true;
						}
						break;
					}
					if( isbossroom ) break;						
				}
				
				if( isbossroom )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1859), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
					break;
				}				
			}
			_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);			
			if(pMiniBar_Window)
			{
				int selectionnumber = (DWORD)lparam -1;
				g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC,1, 0, selectionnumber);
				pMiniBar_Window->m_indexPartyExperienceSharing = selectionnumber+1;
				pMiniBar_Window->m_listboxPartyExperienceSharing->ShowWindow( FALSE );
			}
		}
		break;
	case _XDEF_MINIPARTYWINDOW_ITEMSHAERINGLISTBOX:	// partymaster
		{
			if( g_pLocalUser->m_bIsBossBattle )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1858), TRUE);
				pMessageBox->StartTimer(5000, TRUE) ;
				break;
				
			}
			else
			{
				bool isbossroom = false;
				for( int i = 0; i <  _XDEF_MAXPARTYMEMBERCOUNT ; i++ )
				{
					switch(g_pLocalUser->m_PartyInfo[i].serverno) {
					case 50://���ֺ�����
					case 51://�����庸����
					case 52://�뵿������
					case 53://����������
						{
							isbossroom = true;
						}
						break;
					}
					if( isbossroom ) break;						
				}
				
				if( isbossroom )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1859), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
					break;
				}				
			}
			_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);			
			if(pMiniBar_Window)
			{
				int selectionnumber = (DWORD)lparam -1;
				g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC,1, 1, selectionnumber);
				pMiniBar_Window->m_indexPartyItemSharing = selectionnumber+1;
				pMiniBar_Window->m_listboxPartyItemSharing->ShowWindow( FALSE );
			}
		}
		break;	
	case _XDEF_MINIPARTYWINDOW_NO_PARTYREQUEST:	// partymaster
		{
			g_pLocalUser->m_PartyMemberIndex = -1;
		}
		break;	
	}

}

void XProc_MainGame::QuestWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_QW_CLOSEBUTTON :
		{
			if(g_pQuest_Window)
			{
				g_pQuest_Window->ShowWindow(FALSE);
			}
		}
		break;
		
	case _XDEF_QW_QUESTCOMPLETETABBUTTON :	// complete tab button ó��
		{
//			_XWindow_QuestWindow* questwindow = (_XWindow_QuestWindow *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUESTWINDOW);
//			if(questwindow)
//			{
//				questwindow->ChangeTab(_XQUESTTAB_COMPLETE);
//				questwindow->SetListItem();
//			}

		}
		break;
		
	case _XDEF_QW_QUESTPROGRESSTABBUTTON :	// progress tab button ó��
		{
//			_XWindow_QuestWindow* questwindow = (_XWindow_QuestWindow *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUESTWINDOW);
//			if(questwindow)
//			{
//				questwindow->ChangeTab(_XQUESTTAB_PROGRESS);
//				questwindow->SetListItem();
//			}

		}
		break;
		
	case _XDEF_QW_QUESTCANCEL_BUTTON :
		{			
			int questid = g_pQuest_Window->m_SelectedQuestID;
			_XQuestScriptItem* currentquest = NULL;
			map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
			if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			{
				currentquest = iter_quest->second;
				if(currentquest)
				{
					if( currentquest->GetQuestCancleType() != ID_QUEST_CANCELCASE_NO )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_QUESTGIVEUP), TRUE, _XDEF_QW_QUESTCANCEL_OK, _XDEF_QW_QUESTCANCEL_CANCLE);			// _T("��ȣǳ����� �����Ͻðڽ��ϱ�?")
					}
					else
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1860), TRUE);// _T("���� �� �� ���� ��ȣǳ����Դϴ�.")
						pMessageBox->StartTimer(5000, TRUE) ;
					}					
				}
			}		
		}
		break;
		
	case _XDEF_QW_QUESTCANCEL_OK :	//����Ʈ ���� Ȯ�ι�ư 
		{
			int questid = g_pQuest_Window->m_SelectedQuestID;
			int nodenumber = 0;
			_XQuestScriptItem* currentquest = NULL;
			map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
			if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
				currentquest = iter_quest->second;
			if(currentquest)
			{
				nodenumber = currentquest->GetCurrentTreeNumber();
				g_NetworkKernel.SendPacket(MSG_NO_QUEST_USER_CANCEL, questid, nodenumber);
				g_pQuest_Window->m_QuestCancelButton->ShowWindow( FALSE );
			}
		}
		break;
		
	case _XDEF_QW_QUESTCANCEL_CANCLE :	//����Ʈ ���� ��ҹ�ư 
		{
			g_pQuest_Window->m_QuestCancelButton->ShowWindow( FALSE );
		}
		break;
		
	case _XDEF_QW_QUESTLISTBOX :		// list ó��
		{
			if(g_pQuest_Window)
			{
				g_pQuest_Window->SetSelectedItemIndex(lparam);
				g_pQuest_Window->ProcessSelectedListBox();
//				questwindow->SetSelectedItemIndex(questwindow->m_QuestListBox->GetSelectedItem());
//				questwindow->SetSelectedQuestID(lparam);
//				questwindow->ParseQuestDescString();
//				questwindow->SetQuestItem();
			}
		}
		break;
		
	case _XDEF_QW_QUESTTABBUTTON01				:
		{
			if(g_pQuest_Window)
			{
				g_pQuest_Window->ChangeTab(_XQUESTTAB_QUEST1);			
			}
		}
		break;
	case _XDEF_QW_QUESTTABBUTTON02				:
		{
			if(g_pQuest_Window)
			{
				g_pQuest_Window->ChangeTab(_XQUESTTAB_QUEST2);
			}
		}
		break;
	case _XDEF_QW_QUESTTABBUTTON03				:
		{
			if(g_pQuest_Window)
			{
				g_pQuest_Window->ChangeTab(_XQUESTTAB_QUEST3);
			}
		}
		break;
	case _XDEF_QW_QUESTTABBUTTON04				:
		{
			if(g_pQuest_Window)
			{
				g_pQuest_Window->ChangeTab(_XQUESTTAB_QUEST4);
			}
		}
		break;
	case _XDEF_QW_QUESTTABBUTTON05				:
		{
			if(g_pQuest_Window)
			{
				g_pQuest_Window->ChangeTab(_XQUESTTAB_QUEST5);
			}
		}
		break;
	case _XDEF_QW_QUESTTAB_PREV					:
		{
			if(g_pQuest_Window)
			{
				if( g_pQuest_Window->m_SelectedTab == _XQUESTTAB_QUEST1 )
				{
					g_pQuest_Window->ChangeTab(_XQUESTTAB_QUEST5);
				}
				else
				{
					g_pQuest_Window->ChangeTab((_XQuestWindowTab)(g_pQuest_Window->m_SelectedTab-1));
				}
			}
		}
		break;
	case _XDEF_QW_QUESTTAB_NEXT					:
		{
			if(g_pQuest_Window)
			{
				if( g_pQuest_Window->m_SelectedTab == _XQUESTTAB_QUEST5)
				{
					g_pQuest_Window->ChangeTab(_XQUESTTAB_QUEST1);
				}
				else
				{
					g_pQuest_Window->ChangeTab((_XQuestWindowTab)(g_pQuest_Window->m_SelectedTab+1));
				}
			}
		}
		break;
	default :	// _XDEF_QW_QUESTDESCLISTBOX ó��
		break;
	}
	
}

void XProc_MainGame::SkillWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if(!g_pSkill_Window)
		return;
	
	switch(wparam)
	{
	case _XDEF_SKILLWINDOW_LEFT :
		{
			int index = g_pSkill_Window->m_StartIndex-1;
			g_pSkill_Window->SetStartIndex(index);
			g_pSkill_Window->SetSkillButtonTexture();
		}
		break;
	case _XDEF_SKILLWINDOW_RIGHT :
		{
			int index = g_pSkill_Window->m_StartIndex+1;
			g_pSkill_Window->SetStartIndex(index);
			g_pSkill_Window->SetSkillButtonTexture();
		}
		break;
		
	case _XDEF_SKILLWINDOW_LISTBOX :
		break;

	case _XDEF_SKILLWINDOW_SKILL_1 :
	case _XDEF_SKILLWINDOW_SKILL_2 :
	case _XDEF_SKILLWINDOW_SKILL_3 :
	case _XDEF_SKILLWINDOW_SKILL_4 :
	case _XDEF_SKILLWINDOW_SKILL_5 :
	case _XDEF_SKILLWINDOW_SKILL_6 :
	case _XDEF_SKILLWINDOW_SKILL_7 :
	case _XDEF_SKILLWINDOW_SKILL_8 :
		{
			g_pSkill_Window->SetSelectedSkillButtonIndex(wparam - _XDEF_SKILLWINDOW_SKILL_1);
			g_pSkill_Window->SetListItem();
		}
		break;
#ifdef _XTS_NEWSKILLWINDOW
	case _XDEF_SKILLWINDOW_SKILLSORT_1 :
	case _XDEF_SKILLWINDOW_SKILLSORT_2 :
	case _XDEF_SKILLWINDOW_SKILLSORT_3 :
	case _XDEF_SKILLWINDOW_SKILLSORT_4 :
	case _XDEF_SKILLWINDOW_SKILLSORT_5 :
	case _XDEF_SKILLWINDOW_SKILLSORT_6 :
	case _XDEF_SKILLWINDOW_SKILLSORT_7 :
	case _XDEF_SKILLWINDOW_SKILLSORT_8 :
		{
			g_pSkill_Window->ChangeTab(wparam - _XDEF_SKILLWINDOW_SKILLSORT_1);
			g_pSkill_Window->SetSelectedSortIndex(wparam - _XDEF_SKILLWINDOW_SKILLSORT_1);
			g_pSkill_Window->SetListItemBySType();
		}
		break;
#endif
	}
}


void XProc_MainGame::FSWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	
}

void XProc_MainGame::QuickSlotMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if(!g_pQuickSlot_Window)
		return;

	switch(wparam)
	{
	case _XDEF_QS_BATTLEMODEBUTTON :
		{
			if(g_pLocalUser->GetMotionClass() == _XACTION_IDLE || g_pLocalUser->GetMotionClass() == _XACTION_MOVE)
			{
				if((g_LocalSystemTime - g_pLocalUser->m_LastModeChangeTime) > 1000)
				{
					if(g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
						g_pLocalUser->RequestBattleMode(_XBATTLE_PEACE);
					else if(g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
						g_pLocalUser->RequestBattleMode(_XBATTLE_BATTLE);
					else
						g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_2728), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "���� �߿��� �ٸ� ���� ���·� ������ �� �����ϴ�."
				}
			} // if(g_pLocalUser->GetMotionClass() == _XACTION_IDLE || g_pLocalUser->GetMotionClass() == _XACTION_MOVE)
		}
		break;
	case _XDEF_QS_PREVBUTTON :
		{
			if( g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHARGESKILL ) //Author : ����� //breif : �ʻ�� ������ �� ������ ���� ����
			{
				switch(g_pLocalUser->m_CurrentBattleState)
				{
				case _XUSER_STATE_1 :
				case _XUSER_STATE_2 :
				case _XUSER_STATE_3 :
				case _XUSER_STATE_4 :
				case _XUSER_STATE_5 :
				case _XUSER_STATE_6 :
					return;
				}
			}

			g_pQuickSlot_Window->SetSelectedSet(g_pQuickSlot_Window->GetSelectedSet()-1);
			if(g_pQuickSlot_Window->GetSelectedSet() < 0)
				g_pQuickSlot_Window->SetSelectedSet(2);
			g_pQuickSlot_Window->SetCurrentSelectedActiveIndex(-1);
			g_pQuickSlot_Window->SetCurrentSelectedItemIndex(-1);
			g_pQuickSlot_Window->SetCurrentSelectedOtherIndex(-1);
			g_pQuickSlot_Window->SetCurrentSelectedItemStartTime(0);
		}
		break;
	case _XDEF_QS_NEXTBUTTON :
		{
			if( g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHARGESKILL ) //Author : ����� //breif : �ʻ�� ������ �� ������ ���� ����
			{
				switch(g_pLocalUser->m_CurrentBattleState)
				{
				case _XUSER_STATE_1 :
				case _XUSER_STATE_2 :
				case _XUSER_STATE_3 :
				case _XUSER_STATE_4 :
				case _XUSER_STATE_5 :
				case _XUSER_STATE_6 :
					return;
				}
			}

			g_pQuickSlot_Window->SetSelectedSet(g_pQuickSlot_Window->GetSelectedSet()+1);
			if(g_pQuickSlot_Window->GetSelectedSet() > 2)
				g_pQuickSlot_Window->SetSelectedSet(0);
			g_pQuickSlot_Window->SetCurrentSelectedActiveIndex(-1);
			g_pQuickSlot_Window->SetCurrentSelectedItemIndex(-1);
			g_pQuickSlot_Window->SetCurrentSelectedOtherIndex(-1);
			g_pQuickSlot_Window->SetCurrentSelectedItemStartTime(0);
		}
		break;
	case _XDEF_QS_SHORTCUT1BUTTON :
	case _XDEF_QS_SHORTCUT2BUTTON :
	case _XDEF_QS_SHORTCUT3BUTTON :
	case _XDEF_QS_SHORTCUT4BUTTON :
	case _XDEF_QS_SHORTCUT5BUTTON :
	case _XDEF_QS_SHORTCUT6BUTTON :
	case _XDEF_QS_SHORTCUT7BUTTON :
	case _XDEF_QS_SHORTCUT8BUTTON :
	case _XDEF_QS_SHORTCUT9BUTTON :
	case _XDEF_QS_SHORTCUT0BUTTON :
		{
			if( !gpInput->CheckKeyPress(DIK_LMENU) ) // ����϶��� ������ ��� ����.
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_QUICK_USE_WAV);

				short skillid = (short)lparam;
				if(_XSkillSelectCheckCallBack(0, skillid))
				{
					if(SelectSkill(skillid, FALSE))
					{
						if(_XSkillItem::FindSkillProperty(skillid))
						{
							switch(g_SkillProperty[skillid]->usefulType)
							{
							case _XSI_USEFULTYPE_ACTIVESKILL :
								g_pQuickSlot_Window->SetCurrentSelectedActiveIndex(wparam - _XDEF_QS_SHORTCUT1BUTTON);
								break;
							case _XSI_USEFULTYPE_CHARGESKILL :
							case _XSI_USEFULTYPE_CASTINGSPELL :
							case _XSI_USEFULTYPE_CHANNELINGSPELL :
							case _XSI_USEFULTYPE_FINISHSKILL :
							case _XSI_USEFULTYPE_ROUNDINGSPELL :
							case _XSI_USEFULTYPE_SMASHINGSKILL :
							case _XSI_USEFULTYPE_ACTIONSKILL :
								g_pQuickSlot_Window->SetCurrentSelectedOtherIndex(wparam - _XDEF_QS_SHORTCUT1BUTTON);
								break;
							}
						}
					}
				}
			}
		}
		break;
	case _XDEF_QS_LEVELUPBUTTON :
		{
			//Author : ����� //breif : �����߿��� ����ġ ������ �Ұ����ϰ� �ؾ���..
			if( g_pLocalUser->GetMotionClass() == _XACTION_ATTACK )
				return;

			int index = g_pQuickSlot_Window->GetSelectedSet()*10 + g_pQuickSlot_Window->GetLevelButtonIndex();
			if(index >= 0 && index < 30)
			{
				if(g_pQuickSlot_Window->m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
				{
					int learnedindex = g_pLocalUser->GetLearnedSkillIndex(g_pQuickSlot_Window->m_QuickSlot[index].skillid);
					if(learnedindex < 0)
						break;

					g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel++;

					if(g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel > g_pLocalUser->m_SkillList[learnedindex].m_cSkillLevel)
						g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel = g_pLocalUser->m_SkillList[learnedindex].m_cSkillLevel;
					
					if(g_pLocalUser->m_SkillList[learnedindex].m_sSkillID == g_pLocalUser->m_SelectedSkillID)
					{
						if(g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
						{
							g_NetworkKernel.SendPacket(MSG_NO_SKILL_SELECT);
							if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_2)
								g_pLocalUser->SetSkillAnimation();
						}
					}
					if(g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESKILL ||
						g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL)
					{
						if(g_pLocalUser->m_SkillList[learnedindex].m_cOnOff == 1)
						{
							g_pLocalUser->m_SkillList[learnedindex].m_cOnOff = 0;

							// Off
							short effectcount = g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].effectCount;
							for(int i = 0 ; i < effectcount ; i++)
							{
								// state ����
								g_pLocalUser->m_StateList.DeleteStateFromPassive(g_pQuickSlot_Window->m_QuickSlot[index].skillid, 
									g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel, i);
								
								// Stat ���
								short id = g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].effect[i].id;
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										g_pLocalUser->UpdateLocalUserAttackLevel();
									}
								}
							}
							
							// visual effect ����
							if(g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->visualEffect > 0)
							{
								g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->visualEffect);
							}
							
							// icon ����
							if(g_pState_Window)
							{
								g_pState_Window->DeleteStateList(_XST_PASSIVE, g_pQuickSlot_Window->m_QuickSlot[index].skillid);
							}

							g_NetworkKernel.SendPacket(MSG_NO_SKILL_CTRL_REQ, 2, g_pLocalUser->m_SkillList[learnedindex].m_sSkillID);
						}
					}

					// Parameter �ٽ� ���
					g_pLocalUser->UpdateLocalUserAttackLevel();
				}
			}
		}
		break;
	case _XDEF_QS_LEVELDOWNBUTTON :
		{
			//Author : ����� //breif : �����߿��� ����ġ ������ �Ұ����ϰ� �ؾ���..
			if( g_pLocalUser->GetMotionClass() == _XACTION_ATTACK )
				return;

			int index = g_pQuickSlot_Window->GetSelectedSet()*10 + g_pQuickSlot_Window->GetLevelButtonIndex();
			if(index >= 0 && index < 30)
			{
				if(g_pQuickSlot_Window->m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
				{
					int learnedindex = g_pLocalUser->GetLearnedSkillIndex(g_pQuickSlot_Window->m_QuickSlot[index].skillid);
					if(learnedindex < 0)
						break;
					
					g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel--;
					
					if(g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel < 0)
						g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel = 0;

					if(g_pLocalUser->m_SkillList[learnedindex].m_sSkillID == g_pLocalUser->m_SelectedSkillID)
					{
						if(g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
						{
							g_NetworkKernel.SendPacket(MSG_NO_SKILL_SELECT);
							if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_2)
								g_pLocalUser->SetSkillAnimation();
						}
					}
					if(g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESKILL ||
						g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL)
					{
						if(g_pLocalUser->m_SkillList[learnedindex].m_cOnOff == 1)
						{
							g_pLocalUser->m_SkillList[learnedindex].m_cOnOff = 0;

							// Off
							short effectcount = g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].effectCount;
							for(int i = 0 ; i < effectcount ; i++)
							{
								// state ����
								g_pLocalUser->m_StateList.DeleteStateFromPassive(g_pQuickSlot_Window->m_QuickSlot[index].skillid, 
									g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel, i);
								
								// Stat ���
								short id = g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->detailInfo[g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel].effect[i].id;
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(id);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[id]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										g_pLocalUser->UpdateLocalUserAttackLevel();
									}
								}
							}
							
							// visual effect ����
							if(g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->visualEffect > 0)
							{
								g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[g_pQuickSlot_Window->m_QuickSlot[index].skillid]->visualEffect);
							}
							
							// icon ����
							if(g_pState_Window)
							{
								g_pState_Window->DeleteStateList(_XST_PASSIVE, g_pQuickSlot_Window->m_QuickSlot[index].skillid);
							}

							g_NetworkKernel.SendPacket(MSG_NO_SKILL_CTRL_REQ, 2, g_pLocalUser->m_SkillList[learnedindex].m_sSkillID);
						}
					}

					// Parameter �ٽ� ���
					g_pLocalUser->UpdateLocalUserAttackLevel();
				}
			}
		}
		break;
	}
}

void XProc_MainGame::OptionMenuWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_OPTIONMENU_CLOSEBUTTON			:
		{
			_XWindow* optionmenuwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
			if( optionmenuwindow )
			{				
				optionmenuwindow->ShowWindow( FALSE );
			}
		}
		break;
	case _XDEF_OPTIONMENU_HELPBUTTON			:
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_OPTION );
		}
		break;
	case _XDEF_OPTIONMENU_GRAPHICTABBUTTON		:
		{
			_XWindow* optionmenuwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
			if( optionmenuwindow )
			{
				optionmenuwindow->ShowWindow( FALSE );
			}
			
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{					
				optionwindow->ChangeTab( _XOPTIONTAB_GRAPHIC );
				if( !optionwindow->GetShowStatus() )
				{
					optionwindow->ShowWindow( TRUE );
				}
			}
		}
		break;
	case _XDEF_OPTIONMENU_SOUNDTABBUTTON		:
		{
			_XWindow* optionmenuwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
			if( optionmenuwindow )
			{
				optionmenuwindow->ShowWindow( FALSE );
			}

			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{					
				optionwindow->ChangeTab( _XOPTIONTAB_SOUND );
				if( !optionwindow->GetShowStatus() )
				{
					optionwindow->ShowWindow( TRUE );
				}
			}			
		}
		break;
	case _XDEF_OPTIONMENU_GAMETABBUTTON			:
		{
			_XWindow* optionmenuwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
			if( optionmenuwindow )
			{
				optionmenuwindow->ShowWindow( FALSE );
			}

			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{					
				optionwindow->ChangeTab( _XOPTIONTAB_GAME );
				if( !optionwindow->GetShowStatus() )
				{
					optionwindow->ShowWindow( TRUE );
				}
			}			
		}
		break;
	case _XDEF_OPTIONMENU_COMMUNITYTABBUTTON	:
		{
			_XWindow* optionmenuwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
			if( optionmenuwindow )
			{
				optionmenuwindow->ShowWindow( FALSE );
			}

			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{					
				optionwindow->ChangeTab( _XOPTIONTAB_CONMMUNITY );
				if( !optionwindow->GetShowStatus() )
				{
					optionwindow->ShowWindow( TRUE );
				}
			}
		}
		break;
	case _XDEF_OPTIONMENU_ANOTHORUSERTABBUTTON	:
		{
	#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_logout))
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3759), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"�� �߿��� ������ �� �����ϴ�."
				break;
			}
	#endif
	#ifdef _XTS_PK
			if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
			{
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_2751), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "������ ������� �ʾ� �ٸ� �ι��� ������ �� �����ϴ�."
				break;
			}
	#endif

			if(g_pLocalUser->GetMotionClass() == _XACTION_DIE)		// �׾��� �� �ٸ� �ι� ���� �Ұ�
			{
				break;
			}

	#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK����ġ����, ������ PK����
			//Author : ����� //breif : Ŭ����,
			_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
			if( pPKTracingRequest_Window )
			{
				memset(pPKTracingRequest_Window->m_cTargetName, 0, sizeof(pPKTracingRequest_Window->m_cTargetName));// ����� PK�� �̸� 
				memset(pPKTracingRequest_Window->m_cSpecialTargetName, 0, sizeof(pPKTracingRequest_Window->m_cSpecialTargetName));
				pPKTracingRequest_Window->ShowWindow(FALSE);
			}
	#endif

			if( !m_GameExitFlag )
			{
//				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
//				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_MESSAGEHANDLER_SELECTCHARACTER, TRUE, _XDEF_GOTOSELECTCHARACTER ); // �ι� ��������
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_MESSAGEHANDLER_SELECTCHARACTER, TRUE, _XDEF_GOTOSELECTCHARACTER );
			}
		}
		break;
	case _XDEF_OPTIONMENU_GOTOLOGINTABBUTTON	:
		{

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_logout))
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3760), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"�� �߿��� ���� ȭ������ ���ư� �� �����ϴ�."
				break;
			}
#endif

#ifdef _XTS_PK
			if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2752), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "������ ������� �ʾ� ���� ȭ������ ���ư� �� �����ϴ�."
				break;
			}
#endif
			if( !m_GameExitFlag )
			{
//				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
//				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_LOGINSERVER_GOTOLOGIN, TRUE, _XDEF_GOTOLOGINSERVER );
				
#ifdef _XNEXON_WEBLOGIN				
				extern BOOL g_OnetimeAutoLogin;

				if( g_OnetimeAutoLogin )
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _T("���� ���� ȭ������ ���ư��ðڽ��ϱ�?"), TRUE, _XDEF_GOTOLOGINSERVER);
				else
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, ID_STRING_LOGINSERVER_GOTOLOGIN, TRUE, _XDEF_GOTOLOGINSERVER);
#else
				g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, ID_STRING_LOGINSERVER_GOTOLOGIN, TRUE, _XDEF_GOTOLOGINSERVER);
#endif
				
				_XWindow* optionwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
				if( optionwindow )
				{
					if( optionwindow->GetShowStatus() )
					{
						optionwindow->ShowWindow( FALSE );
					}
				}
			}
		}
		break;
	case _XDEF_OPTIONMENU_EXITGAMETABBUTTON		:
		{

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_logout))
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3759), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"�� �߿��� ������ �� �����ϴ�."
				break;
			}
#endif

#ifdef _XTS_PK
			if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2624), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
				break;
			}
#endif
			if( !m_GameExitFlag )
			{
				m_ProgramExitFlag = TRUE;
//				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
//				pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_LOGINSERVER_EXITGAME, TRUE, _XDEF_GAMECLOSE );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_LOGINSERVER_EXITGAME, TRUE, _XDEF_GAMECLOSE );

				_XWindow* optionwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
				if( optionwindow )
				{
					if( optionwindow->GetShowStatus() )
					{
						optionwindow->ShowWindow( FALSE );
					}
				}
			}			
		}
		break;
	default:
		break;
	}
}


void XProc_MainGame::OptionWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow* optionwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
	switch(wparam)
	{
	case _XDEF_OPTIONWINDOW_CLOSE :
		{
			if( optionwindow )
			{				
				optionwindow->ShowWindow( FALSE );
			}
		}
		break;	
	case _XDEF_OPTIONWINDOW_HELPBUTTON :
		{
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );			
			if( m_pHedekWindowPtr && optionwindow )
			{
				switch( optionwindow->m_SelectedTab ) 
				{
				case _XOPTIONTAB_GRAPHIC :
					m_pHedekWindowPtr->SetHedekType( _XHEDEK_GRAPHIC );
					break;
				case _XOPTIONTAB_SOUND :
					m_pHedekWindowPtr->SetHedekType( _XHEDEK_SOUND );
					break;
				case _XOPTIONTAB_GAME :
					m_pHedekWindowPtr->SetHedekType( _XHEDEK_GAME );
					break;
				case _XOPTIONTAB_CONMMUNITY :
					m_pHedekWindowPtr->SetHedekType( _XHEDEK_COMMUNITY );
					break;
				}
			}
		}
		break;
	case _XDEF_DLGTAB_GRAPHIC		 :
		{
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{				
				optionwindow->ChangeTab( _XOPTIONTAB_GRAPHIC );
			}
		}
		break;	
	case _XDEF_DLGTAB_SOUND			 :
		{
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{				
				optionwindow->ChangeTab( _XOPTIONTAB_SOUND );
			}
		}
		break;
	case _XDEF_DLGTAB_GAME			 :
		{
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{				
				optionwindow->ChangeTab( _XOPTIONTAB_GAME );
			}
		}
		break;
	case _XDEF_DLGTAB_COMMUNITY		 : 
		{
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{				
				optionwindow->ChangeTab( _XOPTIONTAB_CONMMUNITY );
			}
		}
		break;
	
	// Screen option

	case _XDEF_OPTION_SCREEN_RESOLUTION		:
		{

		}
		break;
	case _XDEF_OPTION_SCREEN_DEPTHBUFFER		:
		{
			
		}
		break;
	case _XDEF_OPTION_SCREEN_GAMMA			:
		{
			FLOAT gammalevel = *((FLOAT*)&lparam) / 100.0f;

			g_fGamma = 0.5f + 1.5f * gammalevel;

			gApp->ApplyGammaRamp();

			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_GAMMA, (int)*((FLOAT*)&lparam) );
		}
		break;
	case _XDEF_OPTION_SCREEN_TERRAINLOD		:
		{
			FLOAT rate = *((FLOAT*)&lparam) / 100.0f;

			int modifyfactor = 0;
			if( rate > 0.75f )
			{
				modifyfactor = 50;
			}
			else if( rate > 0.5f )
			{
				modifyfactor = 0;				
			}
			else if( rate > 0.25f )
			{
				modifyfactor = -25;				
			}
			else
			{
				modifyfactor = -50;				
			}

			g_LodTerrain.m_LODLevel1 = g_LodTerrain.m_DefaultLODLevel1 + modifyfactor;
			g_LodTerrain.m_LODLevel2 = g_LodTerrain.m_DefaultLODLevel2 + modifyfactor;
			g_LodTerrain.m_LODLevel3 = g_LodTerrain.m_DefaultLODLevel3 + modifyfactor;

			g_XST_Wrapper.m_RenderLimitDistance = g_LodTerrain.m_LODLevel1;
			
			g_LodTerrain.RebuildLevel(FALSE);

			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_TERRAINLODLEVEL, (int)((FLOAT*)&lparam) );
		}
		break;		
	case _XDEF_OPTION_SCREEN_SIGHTRANGE		:
		{
			return;
			
			FLOAT rate = *((FLOAT*)&lparam) / 100.0f;
			rate += 0.5f;
			if( rate < 0.5f ) rate = 0.5f;
			if( rate > 1.0f ) rate = 1.0f;
									
			g_LodTerrain.m_LODFarPlane	= (250.0f * rate);
			//g_LodTerrain.m_FogFar	    = (250.0f * rate);
			//g_LodTerrain.m_FogDensity   = (0.0038f * rate);
			g_EnvironmentManager.m_EnvironmentLerpLevel = 2.0f;	

			g_LodTerrain.m_3PCamera.mp_fFarPlane = g_LodTerrain.m_LODFarPlane;
			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo( g_LodTerrain.m_3PCamera );
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel(FALSE);
		}
		break;
	case _XDEF_OPTION_SCREEN_GRASSQUALITY	:
		{
			FLOAT rate = *((FLOAT*)&lparam) / 100.0f;
			int count = (int)(64.0f * rate);
			
			if( count == 0 )
			{
				g_LodTerrain.m_RealTimeObjectGenerateCount = 0;
				g_bUseRealtimeGenerateObject = FALSE;
			}
			else
			{
				if( count < 4 ) count = 4;
				if( count > 15 ) g_LodTerrain.m_RealTimeObjectZDepthSorting = TRUE;
				else g_LodTerrain.m_RealTimeObjectZDepthSorting = FALSE;
				
				g_LodTerrain.m_RealTimeObjectGenerateCount = count;
				g_bUseRealtimeGenerateObject = TRUE;				
			}
			
			g_LodTerrain.GenerateGrassObject();

			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEREALTIMEGENERATEOBJECTRATE, (int)(*((FLOAT*)&lparam)) );
		}
		break;
	case _XDEF_OPTION_SCREEN_GRASSANIM		:
		{
			g_bUseRealtimeGenerateObjectAni = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEREALTIMEGENEOBJECTANI, g_bUseRealtimeGenerateObjectAni );
		}
		break;
	case _XDEF_OPTION_SCREEN_TREEANIM		:
		{
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_WORLDOBJECTANI, (BOOL)lparam );
		}
		break;
	case _XDEF_OPTION_SCREEN_CHARACTORSHADOW	:
		{
			g_bCharacterShadow = (BOOL)lparam;
			
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{
				optionwindow->ConfirmDevice();
			}

			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWCHARACTERSHADOW, g_bCharacterShadow );
		}
		break;
	case _XDEF_OPTION_SCREEN_USEBLOOMFILTER	:
		{
			int sliderfactor = (int)(*((FLOAT*)&lparam));

			if( sliderfactor == 0 )
			{
				g_PostProcessManager.EnableBloomFilter( FALSE );

				_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
				if( optionwindow )
				{
					((_XWindow_OptionGraphic*)optionwindow->m_pSubWindow[_XOPTIONTAB_GRAPHIC])->m_pUseBloomFilterButton->SetCheck( FALSE );
				}

				g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEBLOOMFILTER, FALSE );
			}
			else
			{
				FLOAT filterpower = 0.65f + ((FLOAT)sliderfactor/1000.0f);
				g_PostProcessManager.m_FinalPixelShaderConst1[0] = 
				g_PostProcessManager.m_FinalPixelShaderConst1[1] = 
				g_PostProcessManager.m_FinalPixelShaderConst1[2] = 
				g_PostProcessManager.m_FinalPixelShaderConst1[3] = filterpower;

				TCHAR RegValue[_REGSTRING_MAXLENGTH];
				_snprintf( RegValue, sizeof(RegValue), "%f", filterpower );
				g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BLOOMFILTERPOWER, RegValue );

				g_PostProcessManager.EnableBloomFilter( TRUE );

				_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
				if( optionwindow )
				{
					((_XWindow_OptionGraphic*)optionwindow->m_pSubWindow[_XOPTIONTAB_GRAPHIC])->m_pUseBloomFilterButton->SetCheck( TRUE );
				}

				g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEBLOOMFILTER, TRUE );
			}
		}
		break;
	case _XDEF_OPTION_SCREEN_ENABLEBLOOMFILTER :
		{
			g_PostProcessManager.EnableBloomFilter( (BOOL)lparam );
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEBLOOMFILTER, (BOOL)lparam );

			if( (BOOL)lparam )
			{
				_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
				if( optionwindow )
				{
					_XHSlider* pSlider = ((_XWindow_OptionGraphic*)optionwindow->m_pSubWindow[_XOPTIONTAB_GRAPHIC])->m_pUseBloomFilterSlider;
					if( pSlider )
					{
						if( pSlider->m_GaugeValue == 0.0f )
						{
							pSlider->SetGaugeValue( 0.01f );

							FLOAT filterpower = 0.65f;
							g_PostProcessManager.m_FinalPixelShaderConst1[0] = 
								g_PostProcessManager.m_FinalPixelShaderConst1[1] = 
								g_PostProcessManager.m_FinalPixelShaderConst1[2] = 
								g_PostProcessManager.m_FinalPixelShaderConst1[3] = filterpower;
							
							TCHAR RegValue[_REGSTRING_MAXLENGTH];
							_snprintf( RegValue, sizeof(RegValue), "%f", filterpower );
							g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BLOOMFILTERPOWER, RegValue );
						}
					}
				}
			}
		}
		break;
	case _XDEF_OPTION_SCREEN_EFFECTQUALITY	:
		{
			FLOAT lev = *((FLOAT*)&lparam);

			int tracecount = 2 + (int)(18.0f * (lev / 100.0f));
			_XEffect_SwordTrace::SetPointStep( tracecount );			
			
			extern FLOAT	g_VisualEffectQuality;

			if( lev >= 70.0f )
			{
				g_VisualEffectQuality = 1.0f;	
			}
			else
			{
#ifdef _XDEF_VEQUALITYCONTROL
				/*
				lev = lev * 1.2857f; // 70% to 90%
				if( lev < 0.0f )
					lev = 0.0f;
				else if( lev > 90.0f )
					lev = 90.0f;

				g_VisualEffectQuality = ( lev + 10.0f ) / 100.0f;

				if(g_VisualEffectQuality > 1.0f)
					g_VisualEffectQuality = 1.0f;
				else if( g_VisualEffectQuality < 0.1f )
					g_VisualEffectQuality = 0.1f;
				*/

				lev = lev * 1.42857f; // 70% to 90%
				if( lev < 0.0f )
					lev = 0.0f;
				else if( lev > 100.0f )
					lev = 100.0f;

				g_VisualEffectQuality = lev / 100.0f;

				if(g_VisualEffectQuality > 1.0f)
					g_VisualEffectQuality = 1.0f;
				else if( g_VisualEffectQuality < 0.0f )
					g_VisualEffectQuality = 0.0f;
#else				
				lev = lev * 0.7143f; // 70% to 50%
				if( lev > 50.0f )
					lev = 50.0f;
				else if( lev < 0.0f )
					lev = 0.0f;

				g_VisualEffectQuality = (lev / 50.0f) + 0.5f;

				if(g_VisualEffectQuality > 1.0f)
					g_VisualEffectQuality = 1.0f;
				else if( g_VisualEffectQuality < 0.5f )
					g_VisualEffectQuality = 0.5f;
#endif
			}

			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_EFFECTQUALITY, (int)(*((FLOAT*)&lparam)) );
		}
		break;
	case _XDEF_OPTION_SCREEN_REFLECTION		:
		{
			CWater::m_UseObjectReflection = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_REFLECTION, (BOOL)lparam );
		}
		break;
	case _XDEF_OPTION_SCREEN_LENSFLARE		:
		{
			g_EnvironmentManager.m_ViewLensFlare = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LENSFLARE, (BOOL)lparam );
		}
		break;
	case _XDEF_OPTION_SOUND_USEBGM				:
		{
			g_BgmPlayFlag = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMENABLE, g_BgmPlayFlag );

#ifdef _XUSEFMOD
			if( g_BgmPlayFlag )
			{
				if( m_CurrentBGMType == 0 )
					g_FMODWrapper.PlayBGM( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonebgmfilename1 );
				else
					g_FMODWrapper.PlayBGM( g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonebgmfilename2 );
			}
			else
			{
				g_FMODWrapper.StopBGM();
			}
#else			
			if( g_BgmPlayFlag )
			{
				if( g_pBGMSoundObject[m_CurrentBGMType+1] )
				{					
					g_pBGMSoundObject[m_CurrentBGMType+1]->Play();
				}						
			}
			else
			{
				if( g_pBGMSoundObject[m_CurrentBGMType+1] )
				{					
					g_pBGMSoundObject[m_CurrentBGMType+1]->Stop();
				}
			}
#endif
		}
		break;
	case _XDEF_OPTION_SOUND_BGMVOLUME			:
		{
			FLOAT volume = *((FLOAT*)&lparam) / 100.0f;
#ifdef _XUSEFMOD
			if( g_BgmPlayFlag )
			{
				//g_FMODWrapper.SetBGMMaxVolume( (BYTE)(255.0f * volume) );
				g_FMODWrapper.SetBGMMaxVolume( (BYTE)(191.0f * volume) );
			}
#else	
			if( g_pBGMSoundObject[m_CurrentBGMType+1] )
				g_pBGMSoundObject[m_CurrentBGMType+1]->SetVolume( volume );
#endif
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMVOLUME, (int)(*((FLOAT*)&lparam)) );	
		}
		break;
	case _XDEF_OPTION_SOUND_USEEFFECTSOUND		:
		{
			g_SoundEffectFlag = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SOUNDEFFECTENABLE, g_SoundEffectFlag );
#ifdef _XUSEFMOD
			if( !g_SoundEffectFlag )
			{
				g_FMODWrapper.StopAllSound();
			}
#endif
		}
		break;
	case _XDEF_OPTION_SOUND_EFFECTSOUNDVOLUME   :
		{
			FLOAT volume = *((FLOAT*)&lparam) / 100.0f;
#ifdef _XUSEFMOD
			g_FMODWrapper.SetVolume( (BYTE)(255.0f * volume) );
#else	
			for( int i = 0; i < g_MaxSoundEffectCount; i++ )
			{	
				g_pSoundEffect[i]->SetVolume( volume );
			}
#endif

			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SOUNDEFFECTVOLUME, (int)(*((FLOAT*)&lparam)) );
		}
		break;
	case _XDEF_OPTION_SOUND_USEENVSOUND			:
		{
			g_3DSoundEffectFlag = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_3DSOUNDENABLE, g_3DSoundEffectFlag );
#ifdef _XUSEFMOD
			if( !g_3DSoundEffectFlag )
			{
				g_FMODWrapper.FL_StopAllSound();
			}
#endif
		}
		break;
	case _XDEF_OPTION_SOUND_ENVVOLUMESLIDER		:
		{
			FLOAT volume = *((FLOAT*)&lparam) / 100.0f;
#ifdef _XUSEFMOD
			g_FMODWrapper.FL_SetVolume( (BYTE)(255.0f * volume) );
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_ENVSOUNDEFFECTVOLUME, (int)(*((FLOAT*)&lparam)) );
#else	
			
#endif
			
		}
		break;
	case _XDEF_OPTION_SOUND_USEGUIDESOUND		:
		{

		}
		break;
	case _XDEF_OPTION_COMMUNITYREJECTIONWHISPER :
		{
			g_bCommunityOptionRejectWhisper = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_REJECTWHISPER, g_bCommunityOptionRejectWhisper );
			
		}
		break;
	case _XDEF_OPTION_COMMUNITYREJECTFRIEND     :
		{
			g_bCommunityOptionRejectFriend = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_REJECTFRIEND, g_bCommunityOptionRejectFriend );
			
		}
		break;
	case _XDEF_OPTION_COMMUNITYREJECTORGANIZATION		:
		{
			g_bCommunityOptionRefuseOrganization = (BOOL)lparam;
		}
		break;
	case _XDEF_OPTION_COMMUNITYREJECTPARTY	:
		{
			g_bCommunityOptionRejectParty = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_REJECTPARTY, g_bCommunityOptionRejectParty );
			
		}
		break;
	case _XDEF_OPTION_COMMUNITYEXCHANGE		    :
		{
			g_bCommunityOptionRejectPvPTrade = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_REJECTPVPTRADE, g_bCommunityOptionRejectPvPTrade );			
		}
		break;
	case _XDEF_OPTION_COMMUNITYTARGETINFO :
		{
			g_bCommunityOptionRejectTargetInfo = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_REJECTTARGETINFO, g_bCommunityOptionRejectTargetInfo );
		}
		break;
	case _XDEF_OPTION_COMMUNITYCONNECTMESSENGER :
		{
			g_bCommunityOptionMessengerAutoLogin = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_MESSENGERAUTOLOGIN, g_bCommunityOptionMessengerAutoLogin );
			if( g_NetworkKernel.m_bMessengerServerConnected   )
			{
				if(!g_bCommunityOptionMessengerAutoLogin)
				{
					_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
					if( pMessenger_Window )
					{
						pMessenger_Window->m_CurrentUserStatus = 0;	
						pMessenger_Window->m_btnMessengerLogIn->ShowWindow(TRUE);
						pMessenger_Window->m_btnMessengerLogOut->ShowWindow(FALSE);
					}

					g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 0 );	// ������ �ص� �ڽ��� ���¸� �α׿����� �˸���.
				}
				else
				{
					_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
					if( pMessenger_Window )
					{
						pMessenger_Window->m_CurrentUserStatus = 1;	
						pMessenger_Window->m_btnMessengerLogIn->ShowWindow(FALSE);
						pMessenger_Window->m_btnMessengerLogOut->ShowWindow(TRUE);
					}
					
					g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 1 );	// ������ �ص� �ڽ��� ���¸� �α׿����� �˸���.
				}
			}
		}
		break;
	case _XDEF_OPTION_COMMUNITYDISABLEZONECHAT :
		{
#ifdef _XDEF_ZONECHATTING_20070108			
			g_bCommunityOptionDisableZoneChatting = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_COMMUNITYOPTION_DISABLEZONECHATTING, g_bCommunityOptionDisableZoneChatting );		
			
			
			_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
			_XCHATMODE tempmode = pChatDefault_Window->GetChatMode();
			if(tempmode == _XCHATMODE_ZONE)
			{
				pChatDefault_Window->SetChatMode(_XCHATMODE_NORMAL);
			}
			if( g_bCommunityOptionDisableZoneChatting )
			{
				pChatDefault_Window->m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(FALSE);	//������ȭ disable
			}
			else
			{
				pChatDefault_Window->m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(TRUE);	//������ȭ enable
			}
#endif
		}
		break;
		
	case _XDEF_OPTION_GAME_GAMEGUIDE			:
		{
			g_ViewGuide = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWGUIDE, g_ViewGuide );			
		}
		break;
	case _XDEF_OPTION_GAME_VIEWUSERTOOLTIP			:
		{
			g_ViewUserNameTooltip = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_ALWAYSVIEWTOOLTIP, g_ViewUserNameTooltip);

			if(!g_ViewUserNameTooltip)
			{
				g_UsernameTooltipViewTimer = 0;
			}
		}
		break;
	case _XDEF_OPTION_GAME_VIEWMYTOOLTIP :
		{
			g_ViewMyNameTooltip = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWMYTOOLTIP, g_ViewMyNameTooltip);
		}
		break;
	case _XDEF_OPTION_GAME_VIEWITEMTOOLTIP :
		{
			g_ViewItemNameTooltip = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWITEMTOOLTIP, g_ViewItemNameTooltip);
			if(!g_ViewItemNameTooltip)
			{
				g_ItemnameTooltipViewTimer = 0;
			}
		}
		break;
	case _XDEF_OPTION_GAME_HELPSYSTEM			:
		{
			
		}
		break;	
	case _XDEF_OPTION_GAME_CAMERAQUATERVIEW		:
		{
			g_LodTerrain.m_3PCamera.SetQuaterViewMode( (BOOL)lparam );
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEQUATERVIEW, (BOOL)lparam );
		}
		break;
	case _XDEF_OPTION_GAME_USEFIRSTPERSONVIEW	:			
		{
			g_LodTerrain.m_3PCamera.SetEnableFirstPersonView( (BOOL)lparam );
			
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEFIRSTPERSONVIEW, (BOOL)lparam );

			if( g_LodTerrain.m_3PCamera.GetEnableFirstPersonView() )
			{
				g_LodTerrain.m_3PCamera.SetDistanceLimits( 0.8f, g_LodTerrain.m_3PCamera.m_MaxDistance );
			}
			else
			{
				g_LodTerrain.m_3PCamera.SetDistanceLimits( 1.85f, g_LodTerrain.m_3PCamera.m_MaxDistance );
				
				if( g_LodTerrain.m_3PCamera.m_TargetDistance < 1.85f )
					g_LodTerrain.m_3PCamera.SetDistance( 1.85f );
			}

			g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain, TRUE );
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo( g_LodTerrain.m_3PCamera );
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
			g_LodTerrain.RebuildLevel(FALSE);
		}
		break;
	case _XDEF_OPTION_GAME_VIEWMONSTERGAUGE :
		{
			g_ViewMonsterBattleGauge = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWMONSTERHP, g_ViewMonsterBattleGauge);
		}
		break;
	case _XDEF_OPTION_GAME_VIEWBATTLEGAUGE :
		{
			g_ViewMyBattleGauge = (BOOL)lparam;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MINIHPBAR, g_ViewMyBattleGauge );
		}
		break;
	case _XDEF_OPTION_GAME_RANKHIDING :
		{
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{
				optionwindow->SendRankHidingPacket();				
			}
		}
		break;
	case _XDEF_OPTION_GAME_DENYMATCH :
		{
			g_DenyMatch = (BOOL)lparam;
			g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_DENYMATCH, g_DenyMatch);
		}
		break;
	case _XDEF_OPTION_GAME_CHATTINGAUTOCLOSE :
		{
			g_ChattingAutoClose = (BOOL)lparam;
		}
		break;

#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI

	case _XDEF_OPTION_GAME_SELECTCLOTHES :
		{
			if(lparam == 1)
				g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_clothview_select_deck);
			else
				g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_clothview_select_normal);
		}
		break;
#endif

#ifdef _XDEF_KEYCHANGE_20070115
	case _XDEF_OPTION_GAME_KEYTYPE_A :
		{
			g_bGameOptionKeyTypeASelected = TRUE;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_KEYTYPEASELECTED, g_bGameOptionKeyTypeASelected );
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{				
				if( optionwindow->m_pSubWindow[_XOPTIONTAB_GAME] )
				{
					_XWindow_OptionGame* pOptionGame_Window = (_XWindow_OptionGame*)optionwindow->m_pSubWindow[_XOPTIONTAB_GAME];
					if( pOptionGame_Window )
					{
						pOptionGame_Window->m_pCheckBtnKeyTypeA->SetCheck(g_bGameOptionKeyTypeASelected);
						pOptionGame_Window->m_pCheckBtnKeyTypeB->SetCheck(!g_bGameOptionKeyTypeASelected);
					}		
				}
			}
		}
		break;
	case _XDEF_OPTION_GAME_KEYTYPE_B :
		{
			g_bGameOptionKeyTypeASelected = FALSE;
			g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_KEYTYPEASELECTED, g_bGameOptionKeyTypeASelected );
			_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
			if( optionwindow )
			{				
				if( optionwindow->m_pSubWindow[_XOPTIONTAB_GAME] )
				{
					_XWindow_OptionGame* pOptionGame_Window = (_XWindow_OptionGame*)optionwindow->m_pSubWindow[_XOPTIONTAB_GAME];
					if( pOptionGame_Window )
					{
						pOptionGame_Window->m_pCheckBtnKeyTypeA->SetCheck(g_bGameOptionKeyTypeASelected);
						pOptionGame_Window->m_pCheckBtnKeyTypeB->SetCheck(!g_bGameOptionKeyTypeASelected);
					}		
				}
			}
		}
		break;
#endif
		}
		
}


void XProc_MainGame::ChatWindowMessageHandling( WPARAM wparam, LPARAM lparam )
{
	_XWindow_ChattingDefault* defaultchatwindow = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );			
	if( defaultchatwindow )
	{
		switch(wparam)
		{
		case _XDEF_CHATDEFAULT_FUNCTIONBUTTON :
			{
				defaultchatwindow->ShowChatFunctionMenu( !defaultchatwindow->m_btnChatFunctionMenuButton[0]->GetShowStatus() );
			}
			break;		
		case _XDEF_CHATDEFAULT_FUNCTION_EMOTIONBUTTON	:
			{
				defaultchatwindow->SetChatMode( _XCHATMODE_EMOTION );
				//defaultchatwindow->ShowChatFunctionMenu( FALSE );
			}
			break;
		case _XDEF_CHATDEFAULT_FUNCTION_ALLBUTTON		:
		case _XDEF_CHATDEFAULT_FUNCTION_WHISPERBUTTON	:
		case _XDEF_CHATDEFAULT_FUNCTION_PARTYBUTTON		:
		case _XDEF_CHATDEFAULT_FUNCTION_SUBCLANBUTTON	:
		case _XDEF_CHATDEFAULT_FUNCTION_CLANBUTTON		:
#ifdef _XDEF_CASHITEM_SHOUT
		case _XDEF_CHATDEFAULT_FUNCTION_SHOUTBUTTON		:
#endif //_XDEF_CASHITEM_SHOUT
#ifdef _XDEF_ZONECHATTING_20070108
		case _XDEF_CHATDEFAULT_FUNCTION_ZONEBUTTON		:
#endif //_XDEF_ZONECHATTING_20070108

			{
//#ifdef _XDEF_CASHITEM_SHOUT
				extern D3DCOLOR g_ChatFunctionButtonStringColor[_XCHATMODE_TOTALCOUNT];
				int selectedfunction = wparam - _XDEF_CHATDEFAULT_FUNCTION_SHOUTBUTTON;
//#else //_XDEF_CASHITEM_SHOUT
//				extern D3DCOLOR g_ChatFunctionButtonStringColor[6];
//				int selectedfunction = wparam - _XDEF_CHATDEFAULT_FUNCTION_CLANBUTTON;
//#endif //_XDEF_CASHITEM_SHOUT

#ifdef _XDEF_NEWSTRINGMANAGER
				defaultchatwindow->m_btnChatFunction->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, g_ChatFunctionButtonStringID[selectedfunction+1] );
#else
				defaultchatwindow->m_btnChatFunction->SetButtonText( g_ChatFunctionButtonString[selectedfunction+1] );
#endif
				defaultchatwindow->m_btnChatFunction->SetButtonTextColor( g_ChatFunctionButtonStringColor[selectedfunction+1] );
				defaultchatwindow->ShowChatFunctionMenu( FALSE );

				defaultchatwindow->SetChatMode( _XCHATMODE(selectedfunction+1) );
				defaultchatwindow->m_PrevChatMode = _XCHATMODE(selectedfunction+1);
				
#ifdef _XDEF_ZONECHATTING_20070108
				if( wparam == _XDEF_CHATDEFAULT_FUNCTION_ZONEBUTTON)
					g_bProgressZoneChatting = TRUE;	
				else
					g_bProgressZoneChatting = FALSE;	
				
				DWORD playelapsedtime = g_LocalSystemTime - g_DelayTimeZoneChatting;
				if(playelapsedtime >= 10000)		// 10���� 
					defaultchatwindow->m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(TRUE);
#endif //_XDEF_ZONECHATTING_200701088
			}
			break;
		case _XDEF_CHATDEFAULT_WHISPERTARGETBUTTON1 :
		case _XDEF_CHATDEFAULT_WHISPERTARGETBUTTON2 :
		case _XDEF_CHATDEFAULT_WHISPERTARGETBUTTON3 :
			{
				defaultchatwindow->SelectWhisperTargetTab( wparam - _XDEF_CHATDEFAULT_WHISPERTARGETBUTTON1 );
			}
			break;		
		case _XDEF_CHATDEFAULT_RESIZEBUTTON :
			{
				RECT winrect;
				SIZE size = defaultchatwindow->GetWindowSize();

				int bottomYPos = gnHeight;
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI				
				if(g_pLocalUser->GetMotionClass() == _XACTION_DIE || g_pLocalUser->GetMotionClass() == _XACTION_DYING)
				{
					// ���, �������� ��� ä��â �Ʒ��� ����
					bottomYPos = gnHeight;
				}
				else
				{
					bottomYPos = gnHeight - 47;
				}
#endif
				
				if( size.cy >= 574 )	// chat default�� _XDEF_CHATWINDOWMAXSIZE_HEIGHT�� �����ϰ� ����
				{				
					winrect.left   = 0; 
					winrect.top    = bottomYPos - 180;
					winrect.right  = 374;
					winrect.bottom = bottomYPos;
				}				
				else 
				{					
					winrect.left   = 0;
					winrect.top    = bottomYPos - size.cy - 180;
					winrect.right  = 374;
					winrect.bottom = bottomYPos;
				}	
				defaultchatwindow->ResizeWindow( winrect );
			}
			break;
		case _XDEF_CHATDEFAULT_DOCKINGSYSTEMMESSAGE		:
			{
				_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );
				if( defaultchatwindow->m_DockingSystemMessageButton.GetCheck() )
				{
					defaultsystemmsgwindow->ShowWindow( TRUE );
				}
				else
				{
					defaultsystemmsgwindow->ShowWindow( FALSE );
				}
			}
			break;
		case _XDEF_SYSTEMMSGDEFAULT_CLOSE				:
			{
				_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );				
				if( defaultsystemmsgwindow )
				{
					if( defaultsystemmsgwindow->GetShowStatus() )
					{
						if( !m_bFunctionalObjectWideView )
						{
							defaultsystemmsgwindow->ShowWindow( FALSE );						
							defaultchatwindow->m_DockingSystemMessageButton.SetCheck( FALSE );
						}
					}					
				}
			}
			break;
		case _XDEF_CHATDEFAULT_EMOTIONLISTBOX		:		
			{
				bool checkvalid = false;
				if( g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING )
					checkvalid = true;
							
				if( g_pLocalUser->m_CharacterLog.isTrade )
					checkvalid = true;
							
				if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
					checkvalid = true;
				
				if( g_pLocalUser->m_bSocketPlugIn || g_pLocalUser->m_bGambleBoxOpen || 
					g_pLocalUser->m_bReadingBook || g_pLocalUser->m_bInternalUse )
				{
					checkvalid = true;
				}
				
				if( g_pLocalUser->m_BattleMode != _XBATTLE_PEACE )
					checkvalid = true;

				if( g_pLocalUser->m_bMatchPRMode )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_2408), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					checkvalid = true;
				}
				
				// �޼��� �ڽ��� �� �ִ� ���¿����� �̸���Ƽ ǥ���� �� �� ����.
				_XMessageWindow* pMessageBoxWindow = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSAGEWINDOW);
				if( pMessageBoxWindow && pMessageBoxWindow->GetShowStatus() ) checkvalid = true;
				
				_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
				if( pTradeMessageBox && pTradeMessageBox->GetShowStatus() ) checkvalid = true;
				
				_XMessageWindow* pPCTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADEMESSAGEWINDOW);
				if( pPCTradeMessageBox && pPCTradeMessageBox->GetShowStatus() ) checkvalid = true;
				
				_XMessageWindow* pEventMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTMESSAGEWINDOW);
				if( pEventMessageBox && pEventMessageBox->GetShowStatus() ) checkvalid = true;
				
				_XMessageWindow* pHealMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_HEALMESSAGEWINDOW);
				if( pHealMessageBox && pHealMessageBox->GetShowStatus() ) checkvalid = true;
				
				_XMessageWindow* pInchantMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INCHANTMESSAGEWINDOW);
				if( pInchantMessageBox && pInchantMessageBox->GetShowStatus() ) checkvalid = true;
				
				_XMessageWindow* pSkillMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_DELETESKILLMESSAGEWINDOW);
				if( pSkillMessageBox && pSkillMessageBox->GetShowStatus() ) checkvalid = true;
				
				_XMessageWindow* pStateMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_STATECANCLEMESSAGEWINDOW);
				if( pStateMessageBox && pStateMessageBox->GetShowStatus() ) checkvalid = true;
				
				_XMessageWindow* pTargetInfoMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFONMESSAGEWINDOW);
				if( pTargetInfoMessageBox && pTargetInfoMessageBox->GetShowStatus() ) checkvalid = true;
				
				_XMessageWindow* pSocketMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
				if( pSocketMessageBox && pSocketMessageBox->GetShowStatus() ) checkvalid = true;
				
				if( checkvalid )
				{
					XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
					if( pMainGameProcess )
					{
						_XIMEKERNEL.ResetCombinationBuffer();
						pMainGameProcess->m_ChatEditContainer.ClearBuffer();
					}
					
#ifdef _XDEF_CASHITEM_SHOUT
					extern D3DCOLOR g_ChatFunctionButtonStringColor[7];
#else
					extern D3DCOLOR g_ChatFunctionButtonStringColor[6];
#endif
					int selectedfunction = (int)defaultchatwindow->m_PrevChatMode;	
					defaultchatwindow->m_btnChatFunction->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, g_ChatFunctionButtonStringID[selectedfunction] );
					defaultchatwindow->m_btnChatFunction->SetButtonTextColor( g_ChatFunctionButtonStringColor[selectedfunction] );
					defaultchatwindow->ShowChatFunctionMenu( FALSE );
					defaultchatwindow->SetChatMode( _XCHATMODE(selectedfunction) );
					return;
				}
				
				
				int selectline = defaultchatwindow->m_EmotionListBox->GetSelectedItem();
				
				if( selectline < 0 || selectline > defaultchatwindow->m_EmotionListBox->GetItemCount() )
					return;

				if( selectline >= 10 ) selectline+=1; // 10�� �������. ��ĭ �ӽ÷� �ǳʶ�.
				
				if( !( g_pLocalUser->m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_BOW && 
					g_pLocalUser->m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_CLAPPINGHAND ) )
				{
					if(	g_pLocalUser->GetMotionClass() != _XACTION_DYING	&&
						g_pLocalUser->GetMotionClass() != _XACTION_DIE		&& 
						g_pLocalUser->GetMotionClass() != _XACTION_REBIRTH  && 
						g_pLocalUser->GetMotionClass() != _XACTION_INSENSIBLE &&	// ���� ����
						g_pLocalUser->m_CharacterInfo.Get_current_lifepower() > 0 ) 
					{
						BOOL selfchanneling = FALSE;
						char usefultype = 0;
						char targettype = 0;
						if(g_pLocalUser->m_SelectedSkillID > 0)
						{
							usefultype = g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->usefulType;
							targettype = _XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID);//g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->targetType;
						}
						else
							usefultype = _XSI_USEFULTYPE_ACTIVESKILL;
						
						if( usefultype == _XSI_USEFULTYPE_HIDDENMA )
						{
//							if(targettype == _XSI_TARGETTYPE_SELF && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE )
							{
								selfchanneling = TRUE;
							}				
						}
#ifdef _XDEF_LOSTWEAPON_070821_MAGVIPER //@author ����� @breif ���� ������� ���� ����
						if( g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_NONE || g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST )
#else
						if( !selfchanneling )
#endif
						{
							g_pLocalUser->ResetAttackTarget();
							
							g_pLocalUser->SetAnimation( _XACTION_IDLE );
							g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
							
							extern _XCHARACTER_MOTION	g_EmotionActionTable[16];			
							XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
							if( pMainGameProcess )
							{
								g_pLocalUser->SetAnimationIndex( g_EmotionActionTable[selectline] );
								_XIMEKERNEL.ResetCombinationBuffer();
								pMainGameProcess->m_ChatEditContainer.ClearBuffer();
							}
							g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE ); 
							

#ifdef _XDEF_CASHITEM_SHOUT
							extern D3DCOLOR g_ChatFunctionButtonStringColor[7];
#else
							extern D3DCOLOR g_ChatFunctionButtonStringColor[6];
#endif
							
							int selectedfunction = (int)defaultchatwindow->m_PrevChatMode;							
#ifdef _XDEF_NEWSTRINGMANAGER
							defaultchatwindow->m_btnChatFunction->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, g_ChatFunctionButtonStringID[selectedfunction] );
#else
							defaultchatwindow->m_btnChatFunction->SetButtonText( g_ChatFunctionButtonString[selectedfunction] );
#endif
							defaultchatwindow->m_btnChatFunction->SetButtonTextColor( g_ChatFunctionButtonStringColor[selectedfunction] );
							defaultchatwindow->ShowChatFunctionMenu( FALSE );
							
							defaultchatwindow->SetChatMode( _XCHATMODE(selectedfunction) );
						}
					}
				}				
			}
			break; 
		case _XDEF_FUNCTONIONALOBJRECTVIEW_CLOSE		: 
			{
				m_bExitFunctionalObjectWideView = TRUE;

				_XWindow_EnterBossRoom* pEnterBossRoom_Window = (_XWindow_EnterBossRoom*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOM);
				if( pEnterBossRoom_Window )
				{
					if( pEnterBossRoom_Window->GetShowStatus() )
					{
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_BOSSROOM_CANCLE, 0), 0 );
					}
				}
			}
			break;
		case _XDEF_CHATDEFAULT_RETURNTOTOWN		 : 
			{
				if( !g_AreaManager.m_AreaTable[_XDEF_AREACATEGORY_PLAYERSPAWN].AreaList.empty() )
				{
					_XArea* currentarea = NULL;
					list <_XArea *>::iterator iter = g_AreaManager.m_AreaTable[_XDEF_AREACATEGORY_PLAYERSPAWN].AreaList.begin();
					
					if( *iter )
					{
						currentarea = *iter;
						
						g_pLocalUser->m_Position.x = g_pLocalUser->m_NextRespawnPosition.x = currentarea->m_Point[0].x + ((currentarea->m_Point[2].x - currentarea->m_Point[0].x) / 2.0f);
						g_pLocalUser->m_Position.z = g_pLocalUser->m_NextRespawnPosition.z = currentarea->m_Point[0].z + ((currentarea->m_Point[2].z - currentarea->m_Point[0].z) / 2.0f);

						_XOBBData* pOBBData = NULL;
						
						g_pLocalUser->SetAnimation(_XACTION_IDLE);
												
						g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndOBBHeight( g_pLocalUser->m_Position, pOBBData );
						g_pLocalUser->UpdatePosition();
												
						g_pLocalUser->m_TargetPosition			= g_pLocalUser->m_Position;
						g_pLocalUser->m_PrevPosition			= g_pLocalUser->m_Position;
						g_pLocalUser->m_FinalTargetPosition		= g_pLocalUser->m_Position;
						g_pLocalUser->m_LeftFinalTargetLength   = 0.0f;
						g_pLocalUser->m_CurrentNavigatePathNodeIndex	=	0;
						g_pLocalUser->m_PathNodeCount			= 0;	// �н� ���ε� ����Ʈ ������ �ʱ�ȭ �Ͽ� ���߰� ��
						
						g_pLocalUser->m_pGoundCollideOBB = pOBBData;
						
						g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3(g_pLocalUser->m_Position.x,
																			   g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
																			   g_pLocalUser->m_Position.z ) );

						g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
						g_LodTerrain.m_3PCamera.UpdateProjMatrix();
						g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
						g_LodTerrain.RebuildLevel(FALSE);
						
						_XWindow_WorldMinimap* pminimapwindow = 
							(_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
						if( pminimapwindow )
						{
							// Set direction to minimap arrow 
							pminimapwindow->SetRotateArrow( g_pLocalUser->m_RotateAngle + _X_PI /*_X_RAD(180)*/ );					
							// Set minimap status
							pminimapwindow->SetLocalUserPosition();
						}
						
						g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE );
					}

					_XLog( "User moved : player spawn area" );
				}
			}
			break;
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		case _XDEF_CHATDEFAULT_NORMALFILTERMENUBUTTON	:
			//break;
		case _XDEF_CHATDEFAULT_WHISPERFILTERMENUBUTTON	:
			//break;
		case _XDEF_CHATDEFAULT_PARTYFILTERMENUBUTTON	:
			//break;
		case _XDEF_CHATDEFAULT_HEROBANDFILTERMENUBUTTON	:
			{
				defaultchatwindow->m_ChatListScrollBar->SetTotalLineCount( defaultchatwindow->RecalcChattingListLength() );
				defaultchatwindow->m_ChatListScrollBar->UpdateData();
				defaultchatwindow->m_ChatListScrollBar->SetLineScrollPos( 999999 );
			}
			break;
		case _XDEF_CHATDEFAULT_LOCKFILTERMENUBUTTON		:
			defaultchatwindow->m_LockChatFilterMenu = (BOOL)lparam;
			break;
		case _XDEF_CHATDEFAULT_CHATBLACKLISTUBUTTON		:			
			{
				_XWindow_BlackList* pBlackList_Window = (_XWindow_BlackList*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_BLACKLISTWINDOW );
				if( pBlackList_Window )
				{
					BOOL bCheck = defaultchatwindow->m_pChatBlackListButton->GetCheck();
					pBlackList_Window->ShowWindow( bCheck );
				}
			}
			break;
		case _XDEF_CHATDEFAULT_USER_CHATBLACKLISTUBUTTON :
			{
				_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);

				if(pRequestParty_Window)
				{
					_XUser* pUser = NULL;
					pRequestParty_Window->ShowWindow(FALSE);
					pUser = (_XUser*)g_Userlist.FindUser(pRequestParty_Window->m_PickedCharacterID);

					if(pUser)
					{
						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
						{
							if( pUser->m_bFaceMaskMode ) //Author : ����� //breif : ��Ʈ�� ������ �߰� �ȵǰ� 
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3792), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// �������� �߰��� �� �����ϴ�.
								break;
							}
						}

						int length = strlen( pUser->m_CharacterInfo.charactername );
						if( length <= 0 )
						{
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NEW_3476, TRUE ); //_T("ĳ���� �̸��� �־��ּ���.")
						}
						else
						{
							_XWindow_BlackList* pBlackList_Window = (_XWindow_BlackList*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_BLACKLISTWINDOW );
							
							if( pBlackList_Window->FindCharacter(pUser->m_CharacterInfo.charactername) )
							{
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NEW_3477, TRUE );//"�̹� ������Ʈ�� �����ϴ� ĳ���� �Դϴ�."
							}
							else
							{
								pBlackList_Window->InsertCharacter( pUser->m_CharacterInfo.charactername );
								pBlackList_Window->RebuildListBox();
								pBlackList_Window->SaveList( g_pLocalUser->m_UserName );
								TCHAR message[512];
								_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3478), pUser->m_CharacterInfo.charactername );//_T("%s���� ������Ʈ�� �߰��Ǿ����ϴ�.")
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
							}
						}
					}
				}
				
			}
			break;
#endif
		}		
	}
}

#ifdef _XDWDEBUG
void XProc_MainGame::DebugWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_Debug* debugwindow = (_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );			
	if( debugwindow )
	{
		switch(wparam)
		{			
		case _XDEF_XDEBUGINFOMODE_BASE :
			{				
				debugwindow->SetDebugInfoMode( 0 );				
			}
			break;		
		case _XDEF_XDEBUGINFOMODE_SYSTEM :
			{				
				debugwindow->SetDebugInfoMode( 1 );
			}
			break;		
		case _XDEF_XDEBUGINFOMODE_VIDEOINFO :
			{				
				debugwindow->SetDebugInfoMode( 2 );
			}
			break;		
		case _XDEF_XDEBUGINFOMODE_WORLD :
			{				
				debugwindow->SetDebugInfoMode( 3 );
			}
			break;		
		}
	}
}
#endif

void XProc_MainGame::CIWindowMessageHandling( WPARAM wparam, LPARAM lparam )
{
	switch( wparam ) 
	{
	case _XDEF_CI_CLOSEBUTTON: 
		{					
			_XWindow* pciwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHARACTERINFO );
			if( pciwindow )
			{				
				pciwindow->ShowWindow( FALSE );
			}
		}
		break;		
		// 2004.06.19->oneway48 insert
	case _XDEF_CI_MODIFYBUTTON1 :  if( g_pLocalUser->m_CharacterInfo.Get_leftpoint() > 0  )
									   g_NetworkKernel.SendPacket( MSG_NO_POINT_USE, 0 );	// 0, ���� ->�ٷ�
		break;
	case _XDEF_CI_MODIFYBUTTON2 :  if( g_pLocalUser->m_CharacterInfo.Get_leftpoint() > 0  )
									   g_NetworkKernel.SendPacket( MSG_NO_POINT_USE, 1 );	// 1. ���� ->����
		break;
	case _XDEF_CI_MODIFYBUTTON3 :  if( g_pLocalUser->m_CharacterInfo.Get_leftpoint() > 0  )
									   g_NetworkKernel.SendPacket( MSG_NO_POINT_USE, 2 );	// 2. ���� ->����
		break;
	case _XDEF_CI_MODIFYBUTTON4 :  if( g_pLocalUser->m_CharacterInfo.Get_leftpoint() > 0  )
									   g_NetworkKernel.SendPacket( MSG_NO_POINT_USE, 3 );	// 3. ���� ->�ǰ� 
		break;
	case _XDEF_CI_MODIFYBUTTON5 :  if( g_pLocalUser->m_CharacterInfo.Get_leftpoint() > 0  )
									   g_NetworkKernel.SendPacket( MSG_NO_POINT_USE, 4 );	// 4. �ɷ� ->����
		break;
	case _XDEF_CI_MATCHBUTTON :
		{
			_XWindow_MatchRecord* pMatchRecord_Window = (_XWindow_MatchRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHRECORD);
			if(pMatchRecord_Window)
			{
				if(pMatchRecord_Window->GetShowStatus())
					pMatchRecord_Window->ShowWindow(FALSE);
				else
					pMatchRecord_Window->ShowWindow(TRUE);
			}
		}
		break;
	case _XDEF_CI_IVENTORYBUTTON :
		{
			if( g_pInventory_Window )
			{
				if( g_pInventory_Window->GetShowStatus() )
					g_pInventory_Window->ShowWindow( FALSE );
				else
				{
					_XWindow_SocketPlugIn* pSocketPlugInWindow = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN);
					_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
					_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);

					_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
					if( pMessenger_Window->GetShowStatus() ) pMessenger_Window->ShowWindow(FALSE);	
					
					_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
					if( pEmail_Window->GetShowStatus() ) pEmail_Window->ShowWindow(FALSE);
					
					g_pInventory_Window->ShowWindow( TRUE );
					
					if( pSocketPlugInWindow && pSocketPlugInWindow->GetShowStatus() )
					{
						POINT pos = pSocketPlugInWindow->GetWindowPos();
						SIZE size = pSocketPlugInWindow->GetWindowSize();
						g_pInventory_Window->MoveWindow( pos.x + size.cx, pos.y );
					}
					else if( pPCTrade_Window && pPCTrade_Window->GetShowStatus() )
					{
						POINT pos = pPCTrade_Window->GetWindowPos();
						SIZE size = pPCTrade_Window->GetWindowSize();
						g_pInventory_Window->MoveWindow( pos.x + size.cx, pos.y );						
					}
					else
					{
						POINT pos = pMainFrame_Window->GetWindowPos();
						SIZE size = pMainFrame_Window->GetWindowSize();
						g_pInventory_Window->MoveWindow( pos.x + size.cx, pos.y );
					}					
				}
			}
		}
		break;
	case _XDEF_CI_NICKBUTTON_NICKNAME :
		{
			_XWindow_NickName* pNickNameWindow = (_XWindow_NickName*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NICKNAME);
			if( pNickNameWindow )
			{
				if( pNickNameWindow->GetShowStatus() )
					pNickNameWindow->ShowWindow( FALSE );
				else
				{
					pNickNameWindow->SetDefaultPosition();
					pNickNameWindow->ShowWindow( TRUE );
				}
			}
		}
		break;
	case _XDEF_CI_FAVORITESNICKNAMELIST :
		{
			if( g_pCInfo_Window )
			{
				if( g_pCInfo_Window->GetShowStatus() )
				{
					if( g_pCInfo_Window->m_FavoritesNickNameList )
					{
						_XWindow_NickName* pNickName_Window = (_XWindow_NickName*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NICKNAME);
						if( pNickName_Window )
						{
							pNickName_Window->m_SelectedNickNameSlotNumber = g_pCInfo_Window->m_FavoritesNickNameIndexList[(int)lparam-1];

							if( g_pLocalUser->m_CharacterInfo.m_NickNameList[pNickName_Window->m_SelectedNickNameSlotNumber] != 0 )
							{
								if( pNickName_Window->m_RemainChangeTime == 0 )
									g_NetworkKernel.SendPacket( MSG_NO_NIC_SELECT_REQ_C_S, pNickName_Window->m_SelectedNickNameSlotNumber );
								else
								{
									TCHAR tempstr[128];
									memset( tempstr, 0, sizeof(tempstr) );
									_snprintf( tempstr, sizeof(tempstr), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1861), pNickName_Window->m_RemainChangeTime );
									g_NetworkKernel.InsertChatString( tempstr, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE); 
								}
							}
						}						

						if( g_pCInfo_Window->m_FavoritesNickNameList->GetShowStatus() )
							g_pCInfo_Window->m_FavoritesNickNameList->ShowWindow( FALSE );
					}
				}
			}
		}
		break;
	case _XDEF_CI_FAVORITESNICKNAMELIST_BTN :
		{
			if( g_pCInfo_Window )
			{
				if( g_pCInfo_Window->GetShowStatus() )
				{
					if( g_pCInfo_Window->m_FavoritesNickNameList )
					{
						if( g_pCInfo_Window->m_FavoritesNickNameList->GetShowStatus() )
							g_pCInfo_Window->m_FavoritesNickNameList->ShowWindow( FALSE );
						else
						{
							if( g_pCInfo_Window->m_FavoritesNickNameIndexList[0] > 0 )
							{
								g_pCInfo_Window->SetFavoritesNickNameList();
								g_pCInfo_Window->m_FavoritesNickNameList->ShowWindow( TRUE );
							}							
						}
					}
				}
			}
		}
		break;
	case _XDEF_CI_NICKBUTTON_MASTERANDPUPIL :
		{
		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
			_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
			pMainFrame_Window->ChangeWindowTab( _XDEF_MAINFRAME_COMMUNITYTAB );
			g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS5);

			g_pCInfo_Window->m_pNickNameTabbutton[1]->SetCheck(FALSE); //üũ�� Ǯ������
		#endif
		}
		break;
	}
}

void XProc_MainGame::MinimapWindowMessageHandling( WPARAM wparam, LPARAM lparam )
{
	switch(wparam)
	{
	case _XDEF_MINIMAP_CLOSEBUTTON :
		{
			_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
			if( pminimapwindow )
			{					
				pminimapwindow->ChangeMinimizeStatus();
				_XPlayInterfaceSound( g_WindowCloseSoundIndex, true );

				_XWindow_Poison* pPoison_Window = (_XWindow_Poison*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POISON_WINDOW);
				if(pPoison_Window && pPoison_Window->GetShowStatus())
				{
					if(pminimapwindow->m_MinimizeStatus)	// minimize �Ǿ���
					{
						POINT pos = pminimapwindow->GetWindowPos();
						pPoison_Window->MoveWindow(pos.x+2, pos.y+70);
					}
					else
					{
						if( pminimapwindow->m_MiniMapMode == _XMINIMAPMODE_SMALL )
						{
							if(pPoison_Window && pPoison_Window->GetShowStatus())
							{
								POINT pos = pminimapwindow->GetWindowPos();
								pPoison_Window->MoveWindow(pos.x+2, pos.y+214);
							}
						}
						else
						{
							if(pPoison_Window && pPoison_Window->GetShowStatus())
							{
								POINT pos = pminimapwindow->GetWindowPos();
								pPoison_Window->MoveWindow(pos.x+302, pos.y+513);
							}
						}
					}
				}

				_XWindow_Info* pInfo_Window = (_XWindow_Info*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INFOWINDOW);
				if(pInfo_Window && pInfo_Window->GetShowStatus())
				{
					POINT pos = pminimapwindow->GetWindowPos();
					pInfo_Window->MoveWindow(pos.x-50, pos.y+10);
				}
			}						 
		}
		break;		
	case _XDEF_MINIMAP_WORLDMAPBUTTON :
		{
			if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_2419), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
				_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_2419) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			else
			{
				if( !m_GameExitFlag ) 
				{	
					g_indexScene = 2;
					if( g_SceneList[g_indexScene ] )
					{
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_MAP;
						((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash);
					}
				}
			}			
		}
		break;
	case _XDEF_MINIMAP_ZOOMINBUTTON :
		{
			_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
			if( pminimapwindow )
			{					
				pminimapwindow->m_MinimapZoomRate -= 0.004f;
				
				if( pminimapwindow->m_MinimapZoomRate < 0.06f )
					pminimapwindow->m_MinimapZoomRate = 0.06f;
				
				pminimapwindow->SetLocalUserPosition();
			}
		}
		break;
	case _XDEF_MINIMAP_ZOOMOUTBUTTON :
		{
			_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
			if( pminimapwindow )
			{					
				pminimapwindow->m_MinimapZoomRate += 0.004f;
				
				if( pminimapwindow->m_MinimapZoomRate > 0.2f )
					pminimapwindow->m_MinimapZoomRate = 0.2f;
				
				pminimapwindow->SetLocalUserPosition();
			}
		}
		break;	
	case _XDEF_MINIMAP_ZOOMMAXBUTTON : //Author :����� //breif : �пյ��� ǥ�� ��ġ ���� //Date : 07/11/02
		{
			_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );

			_XWindow_Poison* pPoison_Window = (_XWindow_Poison*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POISON_WINDOW);

			if( pminimapwindow )
			{
				if( pminimapwindow->m_MiniMapMode == _XMINIMAPMODE_SMALL )
				{
					pminimapwindow->SetMode( _XMINIMAPMODE_LARGE );
					_XPlayInterfaceSound( g_WindowOpenSoundIndex, true );

					if(pPoison_Window && pPoison_Window->GetShowStatus())
					{
						POINT pos = pminimapwindow->GetWindowPos();
						if(pminimapwindow->m_MinimizeStatus)	// minimize �Ǿ��� 
						{
							pPoison_Window->MoveWindow(pos.x+2, pos.y+70);
						}
						else
						{
							pPoison_Window->MoveWindow(pos.x+302, pos.y+513);
						}
					}
				}
				else
				{
					pminimapwindow->SetMode( _XMINIMAPMODE_SMALL );
					_XPlayInterfaceSound( g_WindowCloseSoundIndex, true );

					if(pPoison_Window && pPoison_Window->GetShowStatus())
					{
						POINT pos = pminimapwindow->GetWindowPos();
						if(pminimapwindow->m_MinimizeStatus)	// minimize �Ǿ���
						{
							pPoison_Window->MoveWindow(pos.x+2, pos.y+70);
						}
						else
						{
							pPoison_Window->MoveWindow(pos.x+2, pos.y+214); 
						}
					}
				}

				_XWindow_Info* pInfo_Window = (_XWindow_Info*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INFOWINDOW);
				if(pInfo_Window && pInfo_Window->GetShowStatus())
				{
					POINT pos = pminimapwindow->GetWindowPos();
					pInfo_Window->MoveWindow(pos.x-50, pos.y+10);
				}
			}

//
//			_XWindow_MaximumMap* pmaxminimapwindow = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
//			if( pmaxminimapwindow )
//			{
//				pmaxminimapwindow->ShowWindow( !pmaxminimapwindow->GetShowStatus() );
//			}

		}
		break;
		
	case _XDEF_MINIMAP_NPCSEARCHBTN			:
		{
			_XWindow_NPCSearch* pNPCSearch_Window = (_XWindow_NPCSearch*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSEARCHWINDOW );	
			if( pNPCSearch_Window )
			{
				if(pNPCSearch_Window->GetShowStatus())
				{
					_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
					if( pminimapwindow )
					{			
						pminimapwindow->m_bDrawNPCSearchEffect = FALSE;
					}
				}
				else
				{
					_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
					if( pminimapwindow )
					{			
						pminimapwindow->m_bDrawNPCSearchEffect = FALSE;
					}
				}
				
				pNPCSearch_Window->ShowWindow( !pNPCSearch_Window->GetShowStatus() );
				
				if ( pNPCSearch_Window->GetShowStatus() )
				{
					_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
					if( pminimapwindow )
					{
						if( !pminimapwindow->m_MinimizeStatus )
						{
							if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
							{
								pNPCSearch_Window->MoveWindow( pminimapwindow->GetWindowPos().x - 112 - 42, pminimapwindow->GetWindowPos().y );
							}
							else
							{
								pNPCSearch_Window->MoveWindow( pminimapwindow->GetWindowPos().x - 112, pminimapwindow->GetWindowPos().y );
							}
						}	
					}
				}
			}
		}
		break;
	case _XDEF_MINIMAP_BTN_WEBBOARD			:
		{
#ifdef _XDEF_WEBBOARD_20061211
			_XWindow_WebBoard* pWebBoard_Window = (_XWindow_WebBoard*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WEBBOARD );	
			if( pWebBoard_Window  )
			{
				pWebBoard_Window->ShowWindow(!pWebBoard_Window->GetShowStatus());
				
				if( pWebBoard_Window->GetShowStatus() )
				{
					
					int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
					
					TCHAR tempStr[512];
					memset( tempStr, 0, sizeof(TCHAR)*512 );
					
//					if( !g_bTestWebPageAddress )
//					{
//						sprintf( tempStr, "http://nine.nexon.com/community/heroboard/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s"
//							,servernumber //������ȣ game_server=%d
//							,g_pLocalUser->m_CharacterInfo.innerlevel //�ɸ��ͷ��� game_lev=%d
//							,g_pLocalUser->m_CharacterInfo.gender //���� game_sex=%d
//							,g_pLocalUser->m_CharacterInfo.cClass // Ŭ���� ���ķ��� ���Ŀ� �����Ǹ� �߰� munpa_lev=%d
//							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //�������̸� game_guild=%s&
//							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //�����ܷ��� ���Ŀ� �����Ǹ� �߰�  game_guild_lev=%d
//							,g_pLocalUser->m_CharacterInfo.groupindex //���� munpa_lev=%d
//							,g_pLocalUser->m_CharacterInfo.charactername //ĳ�����̸� game_char=%s
//							,g_pLocalUser->m_UserName//�ؽ����̵� user_game_id=%s
//						);
//					}
//					else
//					{						
//						sprintf( tempStr, "http://9dtest.nexon.com/community/heroboard/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s"
//							,servernumber //������ȣ game_server=%d
//							,g_pLocalUser->m_CharacterInfo.innerlevel //�ɸ��ͷ��� game_lev=%d
//							,g_pLocalUser->m_CharacterInfo.gender //���� game_sex=%d
//							,g_pLocalUser->m_CharacterInfo.cClass // Ŭ���� ���ķ��� ���Ŀ� �����Ǹ� �߰� munpa_lev=%d
//							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //�������̸� game_guild=%s&
//							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //�����ܷ��� ���Ŀ� �����Ǹ� �߰�  game_guild_lev=%d
//							,g_pLocalUser->m_CharacterInfo.groupindex //���� munpa_lev=%d
//							,g_pLocalUser->m_CharacterInfo.charactername //ĳ�����̸� game_char=%s
//							,g_pLocalUser->m_UserName//�ؽ����̵� user_game_id=%s
//						);
//					}
                    #ifdef _XVIETNAMESE //Author : ����� //breif : ��Ʈ�� �Խ��� �߰�
					_snprintf( tempStr, sizeof(tempStr), "http://9dingame.zing.vn/9d/");
					#else
					if( !g_bTestWebPageAddress )
					{
						_snprintf( tempStr, sizeof(tempStr), "http://nine.nexon.com/community/heroboard/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s&game_hero_level1=%d&game_hero_name=%s&game_hero_level2=%d&hero_bbs=%d"
							,servernumber //������ȣ game_server=%d
							,g_pLocalUser->m_CharacterInfo.Get_innerlevel() //�ɸ��ͷ��� game_lev=%d
							,g_pLocalUser->m_CharacterInfo.gender //���� game_sex=%d
							,g_pLocalUser->m_CharacterInfo.Get_cClass() // Ŭ���� ���ķ��� ���Ŀ� �����Ǹ� �߰� munpa_lev=%d
							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //�������̸� game_guild=%s&
							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //�����ܷ��� ���Ŀ� �����Ǹ� �߰�  game_guild_lev=%d
							,g_pLocalUser->m_CharacterInfo.groupindex //���� munpa_lev=%d
							,g_pLocalUser->m_CharacterInfo.charactername //ĳ�����̸� game_char=%s
							,g_pLocalUser->m_UserName //����ID user_game_id=%s
							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass//�����ܷ��� game_hero_level1=%d (0:�ܿ� 1:�δ��� 2:����)
							,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName //����ȸ�̸� game_hero_name=%s
							,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass //����ȸ���� game_hero_level2=%d (0:�ӽ� 1:ȸ�� 2:��ȸ�� 3:ȸ��)
							,0 //�Խ��� �ڵ� �̵� ���� hero_bbs=%d (0:���ӰԽ��� 1:������ �Խ��� 2:����ȸ �Խ���)
							);
					}
					else
					{				
						_snprintf( tempStr, sizeof(tempStr), "http://9dtest.nexon.com/community/heroboard/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s&game_hero_level1=%d&game_hero_name=%s&game_hero_level2=%d&hero_bbs=%d"
							,servernumber //������ȣ game_server=%d
							,g_pLocalUser->m_CharacterInfo.Get_innerlevel() //�ɸ��ͷ��� game_lev=%d
							,g_pLocalUser->m_CharacterInfo.gender //���� game_sex=%d
							,g_pLocalUser->m_CharacterInfo.Get_cClass() // Ŭ���� ���ķ��� ���Ŀ� �����Ǹ� �߰� munpa_lev=%d
							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //�������̸� game_guild=%s&
							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //�����ܷ��� ���Ŀ� �����Ǹ� �߰�  game_guild_lev=%d
							,g_pLocalUser->m_CharacterInfo.groupindex //���� munpa_lev=%d
							,g_pLocalUser->m_CharacterInfo.charactername //ĳ�����̸� game_char=%s
							,g_pLocalUser->m_UserName //����ID user_game_id=%s
							,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass//�����ܷ��� game_hero_level1=%d (0:�ܿ� 1:�δ��� 2:����)
							,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName //����ȸ�̸� game_hero_name=%s
							,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass //����ȸ���� game_hero_level2=%d (0:�ӽ� 1:ȸ�� 2:��ȸ�� 3:ȸ��)
							,0 //�Խ��� �ڵ� �̵� ���� hero_bbs=%d (0:���ӰԽ��� 1:������ �Խ��� 2:����ȸ �Խ���)
							);
					}
					#endif
					
					DisplayHTMLPage(g_hIWebBoardWebBrowserObject, tempStr);


					g_MainWindowManager.SetTopWindow( pWebBoard_Window );					
					POINT pt = { 0, 0 };
					ClientToScreen( gHWnd, &pt );
					SetWindowPos(g_hWebBoardWindow, HWND_TOPMOST, pt.x + (gnWidth>>1)-343, pt.y + (gnHeight>>1)-280, 688, 450, 0);
					
					ShowWindow(g_hWebBoardWindow, SW_SHOW);
					g_bShowItemMallWindow = TRUE;
				}
				else
				{
					ShowWindow(g_hWebBoardWindow, SW_HIDE );
					g_bShowItemMallWindow = FALSE;
				}
			}
#endif
		}
		break;
	case _XDEF_MAXMINIMAP_CLOSEBUTTON		:
		{
			_XWindow_MaximumMap* pmaxminimapwindow = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
			if( pmaxminimapwindow )
			{
				pmaxminimapwindow->ShowWindow( FALSE );
			}
		}
		break;
	case _XDEF_MAXMINIMAP_NPCVIEWBUTTON		:
		{
			_XWindow_MaximumMap* pmaxminimapwindow = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
			if( pmaxminimapwindow )
			{
				
			}
		}
		break;
	case _XDEF_MAXMINIMAP_PARTYVIEWBUTTON	:
		{
			_XWindow_MaximumMap* pmaxminimapwindow = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
			if( pmaxminimapwindow )
			{
				
			}
		}
		break;
	case _XDEF_MAXMINIMAP_APPLYBUTTON		:
		{
			_XWindow_MaximumMap* pmaxminimapwindow = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
			if( pmaxminimapwindow )
			{
				
			}
		}
		break;
	case _XDEF_MINIMAP_BTN_PKTRACING : //Author : �����
		{
#ifdef _XDEF_PK_TRACINGSERVICES_20070402
			XWindow_PK_ListBox* pPk_TracingServicesWindow = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
			
			if( pPk_TracingServicesWindow )
				pPk_TracingServicesWindow->ShowWindow(!pPk_TracingServicesWindow->GetShowStatus());
#endif
		}
		break;
	case _XDEF_MAXMINIMAP_ALPHASLIDER : //Author : �����
		{
#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER 
			_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );

			FLOAT fResult = *((FLOAT*)&lparam) / 100.0f;

			//���� �ִ�� 80%
			fResult *= 0.8;

			//255 ��з�
			fResult *= 255;

			pminimapwindow->m_nTransparency = (int)fResult;
			pminimapwindow->SetMinimapTransparency( (int)fResult );
#endif
		}
		break;
	}
}

// NPCScript Message handler =======================================================
void XProc_MainGame::NPCScriptWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_NPCSCRIPT_CLOSE :
		{				

#ifdef _ACCLAIM_IGAADSYSTEM
			if( m_UseNPCAD )
			{
				if( m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_NPC || g_IGAADWrapper.GetShowStatus() )
				{
					return;
				}
			}
#endif				
			if( m_NPCWideViewExitTimer!=0 ) break;

			
			_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
			if(pPKTracingRequest_Window && pPKTracingRequest_Window->GetShowStatus() )
			{
				pPKTracingRequest_Window->ShowWindow(FALSE);
			}

			if( m_pNPCScriptWindowPtr )
			{
				if( m_pNPCScriptWindowPtr->m_listboxSelectionItem->GetShowStatus() ) break;

				_XWindow_ChargeGauge* pChargeGauge_WIndow = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
				if( pChargeGauge_WIndow && pChargeGauge_WIndow->GetShowStatus() )
					break;
				
				if( g_LastNPCScriptOwner )
				{
					if( g_LastNPCScriptOwner->m_MessageType == _XDEF_NPCMESSAGE_QUEST ) 
					{
						if( !m_pNPCScriptWindowPtr->m_btnNPCDialogModExit->GetShowStatus() )
						{							
							if( !m_pNPCScriptWindowPtr->m_btnQuestOK->GetShowStatus() ) break;
						}

						if( !g_QuestScriptManager.m_listNPCTargettingQuest.empty() )
						{
							// �� NPC�� ���� ����Ʈ�� Ÿ������ �ɷ������� ����Ʈ ��ȣ�� �����ϴ� ����Ʈ	
							list <_XQUEST_QUESTLIST>::iterator iter_npctargetting;
							for(iter_npctargetting = g_QuestScriptManager.m_listNPCTargettingQuest.begin() ; iter_npctargetting != g_QuestScriptManager.m_listNPCTargettingQuest.end() ; )
							{							
								// NPCTagetting �ߺ�üũ �÷��׸� �ʱ�ȭ ��Ų��.
								_XQUEST_QUESTLIST npctargetting = *iter_npctargetting;							
								_XQuestScriptItem* currentquest = NULL;
								map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(npctargetting.questid);
								if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
								{
									currentquest = iter_quest->second;
									if( currentquest )
									{
										if( currentquest->GetCurrentTreeNumber() == 0 )
										{
											map <int, _XTriggerConditionItem*>::iterator iter_condition;
											_XTriggerConditionItem* currentcondition = NULL;
											for(iter_condition = currentquest->m_mapStartConditionList.begin() ; iter_condition != currentquest->m_mapStartConditionList.end() ; iter_condition++)
											{
												currentcondition = iter_condition->second;
												if(currentcondition)
												{
													currentcondition->SetHoldCheckingOfNPCTargetting(FALSE);
												}
											}			
										}
										else
										{
											map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find( currentquest->GetCurrentTreeNumber() );
											if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
											{
												_XTriggerTreeItem* currenttree = iter_triggertree->second;
												if(currenttree)
												{
													if( currenttree->m_TriggerUniqueID )
													{
														map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
														_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
														if(currentsuccesstrigger)
														{
															map <int, _XTriggerConditionItem*>::iterator iter_condition;
															_XTriggerConditionItem* currentcondition = NULL;
															for(iter_condition = currentsuccesstrigger->m_mapConditionList.begin() ; iter_condition != currentsuccesstrigger->m_mapConditionList.end() ; iter_condition++)
															{
																currentcondition = iter_condition->second;
																if(currentcondition)
																{
																	currentcondition->SetHoldCheckingOfNPCTargetting(FALSE);
																}
															}			
														}
													}
												}
											}
										}
									}
								}
								++iter_npctargetting;
								//iter_npctargettingquest = g_QuestScriptManager.m_listNPCTargettingQuest.erase(iter_npctargettingquest);
							}
							//g_QuestScriptManager.m_listNPCTargettingQuest.clear();
						}
						g_QuestScriptManager.m_indexSelectNPCTargettingQuest = -1;
					}
					else if( g_LastNPCScriptOwner->m_MessageType == _XDEF_NPCMESSAGE_HAVEQUESTLIST )
					{
						if( !g_QuestScriptManager.m_listNPCTargettingQuest.empty() )
						{
							// �� NPC�� ���� ����Ʈ�� Ÿ������ �ɷ������� ����Ʈ ��ȣ�� �����ϴ� ����Ʈ	
							list <_XQUEST_QUESTLIST>::iterator iter_npctargetting;
							for(iter_npctargetting = g_QuestScriptManager.m_listNPCTargettingQuest.begin() ; iter_npctargetting != g_QuestScriptManager.m_listNPCTargettingQuest.end() ; )
							{							
								// NPCTagetting �ߺ�üũ �÷��׸� �ʱ�ȭ ��Ų��.
								_XQUEST_QUESTLIST npctargetting = *iter_npctargetting;							
								_XQuestScriptItem* currentquest = NULL;
								map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(npctargetting.questid);
								if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
								{
									currentquest = iter_quest->second;
									if( currentquest )
									{
										if( currentquest->GetCurrentTreeNumber() == 0 )
										{
											map <int, _XTriggerConditionItem*>::iterator iter_condition;
											_XTriggerConditionItem* currentcondition = NULL;
											for(iter_condition = currentquest->m_mapStartConditionList.begin() ; iter_condition != currentquest->m_mapStartConditionList.end() ; iter_condition++)
											{
												currentcondition = iter_condition->second;
												if(currentcondition)
												{
													currentcondition->SetHoldCheckingOfNPCTargetting(FALSE);
												}
											}			
										}
										else
										{
											map <int, _XTriggerTreeItem*>::iterator iter_triggertree = currentquest->m_mapTriggerTreeList.find( currentquest->GetCurrentTreeNumber() );
											if(iter_triggertree != currentquest->m_mapTriggerTreeList.end())
											{
												_XTriggerTreeItem* currenttree = iter_triggertree->second;
												if(currenttree)
												{
													if( currenttree->m_TriggerUniqueID )
													{
														map <int, _XTriggerItem*>::iterator iter_successtrigger = currentquest->m_mapSuccessTriggerList.find(currenttree->m_TriggerUniqueID);
														_XTriggerItem* currentsuccesstrigger = iter_successtrigger->second;
														if(currentsuccesstrigger)
														{
															map <int, _XTriggerConditionItem*>::iterator iter_condition;
															_XTriggerConditionItem* currentcondition = NULL;
															for(iter_condition = currentsuccesstrigger->m_mapConditionList.begin() ; iter_condition != currentsuccesstrigger->m_mapConditionList.end() ; iter_condition++)
															{
																currentcondition = iter_condition->second;
																if(currentcondition)
																{
																	currentcondition->SetHoldCheckingOfNPCTargetting(FALSE);
																}
															}			
														}
													}
												}
											}
										}
									}
								}
								++iter_npctargetting;
								//iter_npctargettingquest = g_QuestScriptManager.m_listNPCTargettingQuest.erase(iter_npctargettingquest);
							}
							//g_QuestScriptManager.m_listNPCTargettingQuest.clear();
						}
						g_QuestScriptManager.m_indexSelectNPCTargettingQuest = -1;

						m_pNPCScriptWindowPtr->SetMessageType( _XDEF_NPCMESSAGE_NORMAL );
					}
					
					//2004.07.06->oneway48 insert
					if( g_LastNPCScriptOwner->m_MessageType == _XDEF_NPCMESSAGE_NORMAL || g_LastNPCScriptOwner->m_MessageType == _XDEF_NPCMESSAGE_SAVEPOINT )
					{
						m_pNPCScriptWindowPtr->SetMessageType( _XDEF_NPCMESSAGE_NORMAL );

						_XNPCMainScript* npcmainscriptitem = (_XNPCMainScript*)g_LastNPCScriptOwner->m_Triggerlist.getItem(g_LastNPCScriptOwner->m_MainScriptIndex );
						if( npcmainscriptitem )
						{
							if( npcmainscriptitem->strGoodbyeGeneralMessage != NULL )
							{
								_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
								if(pMainFrame_Window)
								{
									if(pMainFrame_Window->GetShowStatus())
										pMainFrame_Window->ShowWindow(FALSE);
								}
								
								_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
																						
								if( g_pInventory_Window && m_pNPCTradeWindowPtr  && pTradeConfirm_Window && m_pPVPBrokerWindowPtr && 
									m_pWarehouseWindowPtr && m_pNPCSkillWindowPtr && m_pRepairItemWindowPtr && m_pNPCBuffWindowPtr &&
									m_pContributionWindowPtr && m_pSocketPlugInExWindowPtr && m_pNPCCollectionWindowPtr && 
									m_pNPCResourceWindowPtr && m_pInchantWindowPtr )
								{
									if(m_pNPCCollectionWindowPtr->GetShowStatus())
										m_pNPCCollectionWindowPtr->ShowWindow(FALSE);
									
									if(m_pNPCResourceWindowPtr->GetShowStatus())
										m_pNPCResourceWindowPtr->ShowWindow(FALSE);		

									if(g_pInventory_Window->GetShowStatus())
									{										
										g_pInventory_Window->SetWindowMoveMode( _XWMOVE_FREE );
										g_pInventory_Window->ShowWindow(FALSE);
									}
									
									if(m_pNPCTradeWindowPtr->GetShowStatus())
										m_pNPCTradeWindowPtr->ShowWindow(FALSE);
									
									if(m_pWarehouseWindowPtr->GetShowStatus())
										m_pWarehouseWindowPtr->ShowWindow(FALSE);
									
									if( pTradeConfirm_Window->GetShowStatus())
										pTradeConfirm_Window->ShowWindow(FALSE);
									
									if( m_pAddSaveItemWindowPtr->GetShowStatus())
										m_pAddSaveItemWindowPtr->ShowWindow(FALSE);
									
									if(m_pNPCSkillWindowPtr->GetShowStatus())
										m_pNPCSkillWindowPtr->ShowWindow(FALSE);
														
									if(m_pRepairItemWindowPtr->GetShowStatus())
										m_pRepairItemWindowPtr->ShowWindow(FALSE);

									if( m_pContributionWindowPtr->GetShowStatus() )
										m_pContributionWindowPtr->ShowWindow(FALSE);

									if( m_pHedekWindowPtr->GetShowStatus() )
										m_pHedekWindowPtr->ShowWindow( FALSE );

									if( m_pSocketPlugInExWindowPtr->GetShowStatus() )
										m_pSocketPlugInExWindowPtr->ShowWindow(FALSE);

									if( m_pInchantWindowPtr->GetShowStatus() )
										m_pInchantWindowPtr->ShowWindow(FALSE);

									if(m_pPVPBrokerWindowPtr->GetShowStatus())
										m_pPVPBrokerWindowPtr->ShowWindow(FALSE);

									if(m_pNPCBuffWindowPtr->GetShowStatus())
										m_pNPCBuffWindowPtr->ShowWindow(FALSE);
								}

								if(m_pNPCResource_New_windowptr && m_pNPCResource_New_windowptr->GetShowStatus())
									m_pNPCResource_New_windowptr->ShowWindow(FALSE);

								// 2004.11.3->hotblood insert -------------------------------------------------��
								_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
								if( pTradeMessageBox ) 
									g_MainWindowManager.DeleteWindow( _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );

								_XMessageWindow* pEventMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTMESSAGEWINDOW);
								if( pEventMessageBox ) 
									g_MainWindowManager.DeleteWindow( _XDEF_WTITLE_EVENTMESSAGEWINDOW );

								_XMessageWindow* pHealMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_HEALMESSAGEWINDOW);
								if( pHealMessageBox ) 
									g_MainWindowManager.DeleteWindow( _XDEF_WTITLE_HEALMESSAGEWINDOW );

								_XMessageWindow* pInchantMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INCHANTMESSAGEWINDOW);
								if( pInchantMessageBox )
									g_MainWindowManager.DeleteAllWindow( _XDEF_WTITLE_INCHANTMESSAGEWINDOW );

								_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSAGEWINDOW);
								if( pMessageBox ) 
									g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_MESSAGEWINDOW );

								if(m_pNPCTradeWindowPtr->m_bHaveMouseSlot)
								{
									m_pNPCTradeWindowPtr->m_bHaveMouseSlot = FALSE;
									m_pNPCTradeWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
								}
								
								if(g_pInventory_Window->m_bHaveMouseSlot)
								{
									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
								}	
								if(m_pWarehouseWindowPtr->m_bHaveMouseSlot )
								{
									m_pWarehouseWindowPtr->m_bHaveMouseSlot = FALSE;
									m_pWarehouseWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
								}	// ------------------------------------------------------------------------��

								
								// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 
								SetNPCMode( _XNPC_NONE );

								// NPC Select Effect�� �����Ѵ�.								
								if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_NPC)
									g_pLocalUser->ResetAttackTarget();
								if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_NPC)
									g_pLocalUser->ResetViewTarget();

								// ����� �� ��縦 �ִ´�.
								m_pNPCScriptWindowPtr->SetMessage( npcmainscriptitem->strGoodbyeGeneralMessage );
								m_NPCWideViewExitTimer = g_LocalSystemTime; //2004.07.05->oneway48 insert
								break;
							}
						}
					}

					//Author : ����� //breif : ����Ʈ �Է�â�� �ʱ�ȭ ������� //���� ģ������ ����
					if( m_pNPCScriptWindowPtr->m_QuestIMEControl.GetShowStatus() )
					{
						if( !m_pNPCScriptWindowPtr->m_QuestIMEControl.GetFocus() ) return;
						m_pNPCScriptWindowPtr->m_QuestIMEControl.ClearBuffer();
						_XIMEKERNEL.SetFocus( NULL );	
						_XIMEKERNEL.ResetCombinationBuffer();	
						m_pNPCScriptWindowPtr->m_bQuestStringInputComplete = TRUE;
					}
					
					m_pNPCScriptWindowPtr->ShowWindow(FALSE);										
					RepositionTopBottomVertex();
					
					g_LastNPCScriptOwner->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
					g_LastNPCScriptOwner->m_SelectedMainScriptIndex = 0;
					g_LastNPCScriptOwner->m_PrevMainScriptIndex = -1;
					g_LastNPCScriptOwner->m_SelectedSubScriptIndex = -1;
					g_LastNPCScriptOwner->m_bCheckHostility= FALSE;
					g_LastNPCScriptOwner->SetHaveQuestID(-1);
					
					g_LastNPCScriptOwner->SetIdleAction();				
					g_LastNPCScriptOwner->SetNPCDialogMode(FALSE);
					
					if(g_LastNPCScriptOwner->m_bStartQuestDialog) 
					{
						g_LastNPCScriptOwner->m_bStartQuestDialog = FALSE;
						m_pNPCScriptWindowPtr->m_btnNPCDialogModExit->ShowWindow(TRUE);
					}
					
					// NPC Select Effect�� �����Ѵ�.
					if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_NPC)
						g_pLocalUser->ResetAttackTarget();
					if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_NPC)
						g_pLocalUser->ResetViewTarget();
					
					
					g_LastNPCScriptOwner->m_pNPCScriptWindowPtr = NULL;
					g_LastNPCScriptOwner = NULL;
				}
				
				g_ScriptStarted = FALSE;
				
				if(m_bNPCDialogWideView)
				{
					m_bNPCDialogWideView = FALSE;
					
					if(lparam == 0)
					{
						g_NPCDialogScreenEffectFlag = TRUE;
						m_bNPCFadeInOutFlag = TRUE;
					}
					else
					{
						g_NPCDialogScreenEffectFlag = FALSE;
						m_bNPCFadeInOutFlag = FALSE;
					}
				}

				_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
				if(pMainFrame_Window)
				{
					if(pMainFrame_Window->GetShowStatus())
						pMainFrame_Window->ShowWindow(FALSE);
				}
				
				_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
								
				if(g_pInventory_Window && m_pNPCTradeWindowPtr  && pTradeConfirm_Window 
					&& m_pWarehouseWindowPtr && m_pNPCSkillWindowPtr && m_pRepairItemWindowPtr && 
					m_pContributionWindowPtr && m_pSocketPlugInExWindowPtr && m_pNPCCollectionWindowPtr && 
					m_pNPCResourceWindowPtr && m_pInchantWindowPtr )
				{
					if(m_pNPCCollectionWindowPtr->GetShowStatus())
						m_pNPCCollectionWindowPtr->ShowWindow(FALSE);
					
					if(m_pNPCResourceWindowPtr->GetShowStatus())
						m_pNPCResourceWindowPtr->ShowWindow(FALSE);
					
					if(g_pInventory_Window->GetShowStatus())
						g_pInventory_Window->ShowWindow(FALSE);
					
					if(m_pNPCTradeWindowPtr->GetShowStatus())
						m_pNPCTradeWindowPtr->ShowWindow(FALSE);
					
					if(m_pWarehouseWindowPtr->GetShowStatus())
						m_pWarehouseWindowPtr->ShowWindow(FALSE);
					
					if( pTradeConfirm_Window->GetShowStatus())
						pTradeConfirm_Window->ShowWindow(FALSE);
					
					if( m_pAddSaveItemWindowPtr->GetShowStatus())
						m_pAddSaveItemWindowPtr->ShowWindow(FALSE);
					
					if(m_pNPCSkillWindowPtr->GetShowStatus())
						m_pNPCSkillWindowPtr->ShowWindow(FALSE);
										
					if(m_pRepairItemWindowPtr->GetShowStatus())
						m_pRepairItemWindowPtr->ShowWindow(FALSE);

					if( m_pContributionWindowPtr->GetShowStatus() )
						m_pContributionWindowPtr->ShowWindow(FALSE);

					if( m_pHedekWindowPtr->GetShowStatus() )
						m_pHedekWindowPtr->ShowWindow( FALSE );

					if( m_pSocketPlugInExWindowPtr->GetShowStatus() )
						m_pSocketPlugInExWindowPtr->ShowWindow(FALSE);

					if( m_pInchantWindowPtr->GetShowStatus() )
						m_pInchantWindowPtr->ShowWindow(FALSE);
				}

				if(m_pNPCResource_New_windowptr && m_pNPCResource_New_windowptr->GetShowStatus())
					m_pNPCResource_New_windowptr->ShowWindow(FALSE);
	
				// 2004.11.3->hotblood insert -------------------------------------------------��
				_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
				if( pTradeMessageBox ) 
					g_MainWindowManager.DeleteWindow( _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
				
				_XMessageWindow* pEventMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTMESSAGEWINDOW);
				if( pEventMessageBox ) 
					g_MainWindowManager.DeleteWindow( _XDEF_WTITLE_EVENTMESSAGEWINDOW );
				
				_XMessageWindow* pHealMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_HEALMESSAGEWINDOW);
				if( pHealMessageBox ) 
					g_MainWindowManager.DeleteWindow( _XDEF_WTITLE_HEALMESSAGEWINDOW );

				if(m_pNPCTradeWindowPtr->m_bHaveMouseSlot)
				{
					m_pNPCTradeWindowPtr->m_bHaveMouseSlot = FALSE;
					m_pNPCTradeWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
				}
				
				if(g_pInventory_Window->m_bHaveMouseSlot)
				{
					g_pInventory_Window->m_bHaveMouseSlot = FALSE;
					g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
				}	
				if(m_pWarehouseWindowPtr->m_bHaveMouseSlot )
				{
					m_pWarehouseWindowPtr->m_bHaveMouseSlot = FALSE;
					m_pWarehouseWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
				}	// ------------------------------------------------------------------------��

				if(m_pPVPBrokerWindowPtr)
				{
					if(m_pPVPBrokerWindowPtr->GetShowStatus())
						m_pPVPBrokerWindowPtr->ShowWindow(FALSE);
				}

				if(m_pNPCBuffWindowPtr)
				{
					if(m_pNPCBuffWindowPtr->GetShowStatus())
						m_pNPCBuffWindowPtr->ShowWindow(FALSE);
				}

				_XWindow_MixWeapon* pMixWeapon_Window = (_XWindow_MixWeapon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MIXWEAPON_WINDOW);
				if(pMixWeapon_Window)
				{
					if(pMixWeapon_Window->GetShowStatus())
						pMixWeapon_Window->ShowWindow(FALSE);
				}
				
				// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 
				SetNPCMode( _XNPC_NONE );

				m_pNPCScriptWindowPtr->m_NPCDialogCount = 0;//2004.07.08->oneway48 insert
				m_pNPCScriptWindowPtr->m_bFirstDialog = TRUE;
				m_pNPCScriptWindowPtr->m_bProgressNPCDialogMessage = FALSE;
				
				m_pNPCScriptWindowPtr->m_btnQuestOK->ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_btnDialogNextPage->ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_btnBackToNPCMode->ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_listboxSelectionItem->ShowWindow(FALSE);
				
			//	m_pNPCScriptWindowPtr->m_btnNPCDialogModExit->ShowWindow(FALSE);
				if( m_pNPCScriptWindowPtr->m_pTitleString[0] != 0 ) 
					memset(m_pNPCScriptWindowPtr->m_pTitleString, 0, sizeof(TCHAR)*256);
				if( m_pNPCScriptWindowPtr->m_HaveQuestName[0] != 0 ) 
					memset(m_pNPCScriptWindowPtr->m_HaveQuestName, 0, sizeof(m_pNPCScriptWindowPtr->m_HaveQuestName));
				if( m_pNPCScriptWindowPtr->m_HaveQuestType[0] != 0 ) 
					memset(m_pNPCScriptWindowPtr->m_HaveQuestType, 0, sizeof(m_pNPCScriptWindowPtr->m_HaveQuestType));
				
				// �̺�Ʈ ���� �Է�â�� �ʱ�ȭ ��Ų��.
				m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.ClearBuffer();
				_XIMEKERNEL.SetFocus( NULL );	
				_XIMEKERNEL.ResetCombinationBuffer();			

				m_pWarehouseWindowPtr->m_bWarehouseFirstView = TRUE; // 2004.06.01->oneway48 insert
				
				_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
				pminimapwindow->m_LargeMinimapImage.SetScale( 0.87890625f, 0.87890625f);
			}
		}
		break;
	case _XDEF_NPCSCRIPT_RENEW :
		{	
			if( g_LastNPCScriptOwner )
			{
				g_LastNPCScriptOwner->SetConversationAction();
				g_LastNPCScriptOwner->m_SelectedSubScriptIndex = lparam;
				
				_XNPCMainScript* npcmainscriptitem = (_XNPCMainScript*)g_LastNPCScriptOwner->m_Triggerlist.getItem(g_LastNPCScriptOwner->m_MainScriptIndex );
				
				int	subscriptcount = 0;
				if(!npcmainscriptitem->m_SubScriptList.listEmpty())
				{
					_XNPCScriptItem*		npcsubscript;
					npcmainscriptitem->m_SubScriptList.resetList();
					m_pNPCScriptWindowPtr->ClearSubScriptList();
					do 
					{
						npcsubscript = (_XNPCScriptItem *)npcmainscriptitem->m_SubScriptList.currentItem();
						m_pNPCScriptWindowPtr->SetSubScriptList(subscriptcount, npcsubscript->scriptclass);	
						
						npcmainscriptitem->m_SubScriptList.nextItem();
						subscriptcount++;
					} while(!npcmainscriptitem->m_SubScriptList.atHeadOfList());
					m_pNPCScriptWindowPtr->m_EventType = npcmainscriptitem->m_EventType;
					m_pNPCScriptWindowPtr->GenerateSubScriptList();
				}
				else
				{
					m_pNPCScriptWindowPtr->ClearSubScriptList();
				}
			}
			
		}
		break;
	case _XDEF_NPCSCRIPT_QA :
		{	
			if( g_LastNPCScriptOwner )
			{
				g_LastNPCScriptOwner->SetConversationAction();
				
				_XNPCMainScript* npcmainscriptitem = (_XNPCMainScript*)g_LastNPCScriptOwner->m_Triggerlist.getItem(g_LastNPCScriptOwner->m_MainScriptIndex );
				_XNPCScriptItem* npcsubscript = (_XNPCScriptItem *)npcmainscriptitem->m_SubScriptList.getItem(m_indexNPCSubScriptQA);
				
				
				int selectedQuestion = 0;
				for( int i = 0; i < 4; i++)
				{
					if(m_pNPCScriptWindowPtr->m_ScriptButton[i]->CheckMousePosition())
						selectedQuestion = i;
				}
				
				m_pNPCScriptWindowPtr->SetMessage( npcsubscript->strMessage2[selectedQuestion]);
			}
		}
		break;
	case _XDEF_NPCSCRIPT_NEXTSCRIPT :
		{
			_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
			if( pChargeGauge_Window )
			{
				if( pChargeGauge_Window->GetShowStatus() )
					break;
			}
			_XMessageWindow* pInchantMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INCHANTMESSAGEWINDOW);
			if( pInchantMessageBox )
			{
				if( pInchantMessageBox->GetShowStatus() )
					break;
			}

			_XMessageWindow* pNPCTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
			if( pNPCTradeMessageBox )
			{
				if( pNPCTradeMessageBox->GetShowStatus() )
					break;
			}
			
			_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
			if(pPKTracingRequest_Window && pPKTracingRequest_Window->GetShowStatus() )
			{
				pPKTracingRequest_Window->ShowWindow(FALSE);
			}
						
			if(g_LastNPCScriptOwner)
			{
				// ����Ʈ ��� �߿� NPC ������� �Ѿ �� ����.
				if( g_LastNPCScriptOwner->m_MessageType == _XDEF_NPCMESSAGE_QUEST )
					break;
								
				if( m_pNPCScriptWindowPtr->m_listboxSelectionItem->GetShowStatus() )
					break;

				if( g_LastNPCScriptOwner->m_bStartQuestDialog )
					break;
		

				g_LastNPCScriptOwner->SetConversationAction();

				// owner���� lparam�� �ش�.
				g_LastNPCScriptOwner->m_SelectedSubScriptIndex = lparam;
				g_NPCDialogScreenEffectFlag = FALSE;

				_XNPCMainScript* npcmainscriptitem = (_XNPCMainScript*)g_LastNPCScriptOwner->m_Triggerlist.getItem(g_LastNPCScriptOwner->m_MainScriptIndex );
				_XNPCScriptItem* npcsubscript = (_XNPCScriptItem *)npcmainscriptitem->m_SubScriptList.getItem(g_LastNPCScriptOwner->m_SelectedSubScriptIndex);
			
				if(npcsubscript && !g_LastNPCScriptOwner->m_bCheckHostility) 
				{
					// 2004.11.3->hotblood insert  ------------------------------------------------��
					_XMessageWindow* pTradeMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADEMESSAGEWINDOW);
					if( pTradeMessageBox ) 
						g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_MESSAGEWINDOW );
					_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
					if( pTradeConfirm_Window->GetShowStatus() ) 
						pTradeConfirm_Window->ShowWindow(FALSE);
					
					if(m_pNPCTradeWindowPtr->m_bHaveMouseSlot)
					{
						m_pNPCTradeWindowPtr->m_bHaveMouseSlot = FALSE;
						m_pNPCTradeWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
					}

					if( g_pInventory_Window->m_bHaveMouseSlot)
					{
						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
						g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
					}
					if( g_pInventory_Window->m_bExpandDockingBorderFlag )
						g_pInventory_Window->SetExpandDockingBorderFlag(FALSE);

					if(m_pWarehouseWindowPtr->m_bHaveMouseSlot )
					{
						m_pWarehouseWindowPtr->m_bHaveMouseSlot = FALSE;
						m_pWarehouseWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
					}	// ------------------------------------------------------------------------��					
					
					if( m_pNPCScriptWindowPtr->m_imageDialogNextPageLeftBorder->GetShowStatus() )
					{
						m_pNPCScriptWindowPtr->m_btnDialogNextPage->ShowWindow(FALSE);
						m_pNPCScriptWindowPtr->m_imageDialogNextPageLeftBorder->ShowWindow(FALSE);
						m_pNPCScriptWindowPtr->m_imageDialogNextPageRightBorder->ShowWindow(FALSE);		
						m_pNPCScriptWindowPtr->m_bProgressNPCDialogMessage = FALSE;
					}

					switch(npcsubscript->scriptclass)
					{
					case _XDEF_NPCSUBSCRIPTCLASS_TRADEITEM :
						{
							// ������ �ŷ�
							if(m_pNPCTradeWindowPtr && g_pInventory_Window)
							{
								// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 								
								SetNPCMode( _XNPC_ITEMTRADE );
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_TRADE));	// _T("��ǰ �Ÿ�")
								m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);

								m_pNPCTradeWindowPtr->SetOwnerNPCID(g_LastNPCScriptOwner->m_UniqueID);
								m_pNPCTradeWindowPtr->SetItemPackageID(npcsubscript->condition1);
								if(npcsubscript->condition2 != -1) m_pNPCTradeWindowPtr->SetAdditionalItemPackageID(npcsubscript->condition2);
								if(npcsubscript->condition3 != -1) m_pNPCTradeWindowPtr->SetSpecialItemPackageID(npcsubscript->condition3);

								// �ϴ� ����
								if( g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_BLACKMARKET )  // �ϻ���
								{
									int index = g_ServerTimeCode / 9000;	// 2�ð� �� 9000
									index = index % 10+1;
									m_pNPCTradeWindowPtr->SetBlackMarketMode(TRUE);
									m_pNPCTradeWindowPtr->LoadGambleItemTradePackage(index);
									
								}
								else
								{									
									m_pNPCTradeWindowPtr->SetBlackMarketMode(FALSE);
									m_pNPCTradeWindowPtr->SetListItem();
								}

								g_pInventory_Window->SetNPCTradeWindowPtr(m_pNPCTradeWindowPtr);
							}
						}
						break;

						//2004.06.19->oneway48 insert
					case  _XDEF_NPCSUBSCRIPTCLASS_REPAIRCLOTHES :
						{
							// ������ ����				
							if( g_pInventory_Window && m_pRepairItemWindowPtr )
							{
								// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 
								SetNPCMode( _XNPC_REPAIRITEM );
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1862));	
								m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);

								m_pRepairItemWindowPtr->m_bRepairWeapon = FALSE;
								m_pRepairItemWindowPtr->ResetRepairListData();
								m_pRepairItemWindowPtr->m_RepairMode = RM_NONE; ;								

								g_pInventory_Window->SetRepairItemWindowPtr(m_pRepairItemWindowPtr);	
							}
						}
						break;

					case _XDEF_NPCSUBSCRIPTCLASS_REPAIRITEM :
						{
							// ������ ����				
							if( g_pInventory_Window && m_pRepairItemWindowPtr )
							{
								// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 
								SetNPCMode( _XNPC_REPAIRITEM );		
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1862));	
								m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);	

								m_pRepairItemWindowPtr->m_bRepairWeapon = TRUE;//2004.06.19->oneway48 insert
								m_pRepairItemWindowPtr->ResetRepairListData();
								m_pRepairItemWindowPtr->m_RepairMode = RM_NONE; 

								g_pInventory_Window->SetRepairItemWindowPtr(m_pRepairItemWindowPtr);
								
							}
						}
						break;

					case _XDEF_NPCSUBSCRIPTCLASS_ENDSCRIPT ://��� -> ���ư���
						{
							SetNPCMode( _XNPC_NONE );

							int count = 0;
							for(int i = 0 ; i < 5; i++)
							{															
								if( npcmainscriptitem->strGreetingGeneralMessage[i] != NULL )
									count++;
							}						
							
							if( count == 0 )
							{
								g_LastNPCScriptOwner->m_SelectedMessageIndex = 0;
							}
							else
							{
								g_LastNPCScriptOwner->m_SelectedMessageIndex = _XRandom() % count;
								g_LastNPCScriptOwner->m_SelectedMessageIndex++;
								if(g_LastNPCScriptOwner->m_SelectedMessageIndex >= count)
									g_LastNPCScriptOwner->m_SelectedMessageIndex = 0;
							}
							
							//���� ����Ʈ�� ���� �� 
							m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
							m_pNPCScriptWindowPtr->SetMessage(npcmainscriptitem->strGreetingGeneralMessage[g_LastNPCScriptOwner->m_SelectedMessageIndex]);
							g_LastNPCScriptOwner->SetMessageString(npcmainscriptitem->strGreetingGeneralMessage[g_LastNPCScriptOwner->m_SelectedMessageIndex]);
							
							// ��ȭ�ϱ� ó������ �ǵ����� 
							m_pNPCScriptWindowPtr->m_NPCDialogCount = 0;
							m_pNPCScriptWindowPtr->m_bFirstDialog = TRUE;
							m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_TALK));		// _T("��   ȭ")
							
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_INFORMATION :
						// ��Ʈ ����
						break;

					case _XDEF_NPCSUBSCRIPTCLASS_BLACKMARKET :
						// �ϰŷ�
						break;

					case _XDEF_NPCSUBSCRIPTCLASS_SELLHERB :
						// ü�� ȸ�� - ���� ����
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_SELLCHARM :
						// ü�� ȸ�� - ���� ����
						break;

					case _XDEF_NPCSUBSCRIPTCLASS_JUDGEVALUE :
						// ������ ���� 
						break;

					case _XDEF_NPCSUBSCRIPTCLASS_GAMBLE :
						// �̴� ����
						break;

					case _XDEF_NPCSUBSCRIPTCLASS_MOVE :
						// ���� �̵�
						break;

					case _XDEF_NPCSUBSCRIPTCLASS_WARP :
						{
							// ���� �̵�
							//npcsubscript->quantitynumber1 : �������̵�
							//npcsubscript->quantitynumber2 : ����
							
							_XArea* pCurrentFocusedArea = g_AreaManager.GetArea( _XDEF_AREACATEGORY_MAPWARP, npcsubscript->quantitynumber1 );
							
							if( pCurrentFocusedArea )
							{
								((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_PrevCheckedWarpingZoneTimer = g_LocalSystemTime;
								// ���� ���ִ� ���� ��Ż �ε��� ���� ���ۿ�
								((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_CurrentWarpZoneIndex = npcsubscript->quantitynumber1;	 
								((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_PrevZoneInfoIndex	   = g_CurrentZoneInfoIndex;
								
								
								g_CurrentZoneInfoIndex = pCurrentFocusedArea->m_TargetZone+1;    // �������� �������ε����� �����Ѵ�. �����ϰ�� �ǵ���.
								((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_NextWarpZoneIndex	   = pCurrentFocusedArea->m_TargetWarpzone;	 // ���� ���� ������ ���� �ε���
								
								((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_WarpStartTime = g_LocalSystemTime; 
								
								TCHAR tstr[256];
								_snprintf( tstr, sizeof(tstr), _XGETINTERFACETEXT(ID_STRING_MAIGAME_MOVING), g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonename );
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, " ", tstr, TRUE );
								
								
								g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 4 );// �����̵� ���̶�� ���¸� �˷��ش�.
								
								PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MG_WARPINGNEXTZONE, 0), 0 );
								
								((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LockMoveProcess = TRUE;
								
							}
						}
						break;

					case _XDEF_NPCSUBSCRIPTCLASS_SAVEITEM :
						{
							// â�� ���
							if(m_pWarehouseWindowPtr && g_pInventory_Window)
							{
								
								int count = 0;
								for(int i = 0 ; i < _XDEF_WAREHOUSE_TOTALCOUNT ; i++)
								{
									if(g_pLocalUser->m_WarehouseItemList[i].Get_m_sID() > 0)
										count++;
								}							
								
								
								if( m_pWarehouseWindowPtr->m_UseSlotCount == 0 && g_pLocalUser->m_WarehouseMoney == 0 )
								{
									if(count == 0)
									{
										// â�� �߰��� �ؾ��Ѵ�.
										m_pNPCScriptWindowPtr->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_WAREHOUSEADD), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);		// "â�� �߰����� â�� ��� ����� �Ͻʽÿ�."
										
									}
									else if( m_pWarehouseWindowPtr->m_UseSlotCount < count )
									{
										// ����Ⱓ�� ������ â�� ����� �� ����.
										m_pNPCScriptWindowPtr->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_WAREHOUSEEXPIRE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "����Ⱓ�� ������ â�� ����� �� �����ϴ�."
										m_pWarehouseWindowPtr->m_bTerminateWarehouse = TRUE; 										
										m_pWarehouseWindowPtr->m_bWarehouseCancleMode = FALSE;										
										g_pInventory_Window->SetWarehouseWindowPtr(m_pWarehouseWindowPtr);
										
										// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 
										SetNPCMode( _XNPC_WAREHOUSE );										
									}
								}
								else // 2004.05.31->oneway48 insert 
								{
									if(m_pWarehouseWindowPtr->m_bWarehouseFirstView)
									{
										m_pWarehouseWindowPtr->m_bTerminateWarehouse = FALSE; // 2004.06.02->oneway48 insert
										m_pWarehouseWindowPtr->m_bWarehouseFirstView = FALSE; // 2004.06.01->oneway48 insert
										// â�� ��� �������� %d�� %d�� %d�� �Դϴ�.
										TCHAR messagestring[256];
										
										int param1 = m_pWarehouseWindowPtr->m_TerminativeYear;
										int param2 = m_pWarehouseWindowPtr->m_TerminativeMonth;
										int param3 = m_pWarehouseWindowPtr->m_TerminativeDay;
										
										strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAINGAMEMESSAGEHANDLER_WAREHOUSETERMINATIONDAY,
																								 &param1, &param2, &param3 ) );
										
										m_pNPCScriptWindowPtr->InsertNPCChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET);
										m_pWarehouseWindowPtr->m_bUseableWarehouse = TRUE; // 2004.06.01->oneway48 insert
									}
									
									m_pWarehouseWindowPtr->m_bWarehouseCancleMode = FALSE;// 2004.06.01->oneway48 insert
									//m_pWarehouseWindowPtr->m_SelectAllButton->SetButtonText(_XGETINTERFACETEXT(ID_STRING_WAREHOUSE_SELECTALL));// 2004.06.01->oneway48 insert
									
									
									g_pInventory_Window->SetWarehouseWindowPtr(m_pWarehouseWindowPtr);
									
									
									// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 
									SetNPCMode( _XNPC_WAREHOUSE );
									m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
									m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_USEWAREHOUSE));		// _T("â�� ���")
								}							
							}
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_ADDSAVEITEM :
						// â�� �߰�
						{
							if( m_pAddSaveItemWindowPtr && g_pInventory_Window )
							{									
								m_pAddSaveItemWindowPtr->Reset();
                            #ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
                                m_pAddSaveItemWindowPtr->SetSelectTab(0);
                            #else
								m_pAddSaveItemWindowPtr->CheckUseSlot();
                            #endif
								

								g_pInventory_Window->ShowWindow(TRUE);
								g_pInventory_Window->SetWarehouseWindowPtr(m_pWarehouseWindowPtr);
								
								
								// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 
								SetNPCMode( _XNPC_CONTRACTWAREHOUSE );
								m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ADDWAREHOUSE));		// _T("â�� �߰�")
								
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1863), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE); 
							}
						
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_EXITGAME :
						// ���� ����
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_STARTQUEST :
						{
							SetNPCMode( _XNPC_NONE );
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_DIALOG :
						{
							if( g_LastNPCScriptOwner )
							{
								// ��ȭ 
								if(m_pNPCScriptWindowPtr)
								{
									SetNPCMode( _XNPC_NONE );
									// 2004.06.20->oneway48 modify close3-2
									if( m_pNPCScriptWindowPtr->m_bFirstDialog )
									{
										//m_pNPCScriptWindowPtr->m_bFirstDialog = FALSE; // 2004.05.01->oneway48 insert
										
										//int m_SelectedMessageIndex;
										// m_SelectedMessageIndex = _XRandom() % 5;
										// m_SelectedMessageIndex++;
										// if(m_SelectedMessageIndex >= 5)
										// m_SelectedMessageIndex = 0;
										
										//m_SelectedMessageIndex = 0;
										
										if(npcsubscript->strMessage1[m_pNPCScriptWindowPtr->m_NPCDialogCount] != NULL)
										{
											m_pNPCScriptWindowPtr->ShowWindow(TRUE);
											m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_TALK));		// _T("��   ȭ")
											
											if(npcsubscript->m_SubScriptConditionList.listEmpty())
											{
												// NPC SubScript�� ���Ǹ���Ʈ�� ������ ���� ���� �� : �Ϲ��� ��� ���
												m_pNPCScriptWindowPtr->SetMessage(npcsubscript->strMessage1[m_pNPCScriptWindowPtr->m_NPCDialogCount]);
												m_pNPCScriptWindowPtr->m_NPCDialogCount++;
											}
											else
											{
												if(npcsubscript->ProcessCondition() == NPC_CONDITION_RESULT_SUCCESS)
												{
													// ������ ������ �� : �Ϲ�, �ܰ�, �������� �÷� �ٽ� �и��ؾ� �� 
													m_pNPCScriptWindowPtr->SetMessage(npcsubscript->strMessage1[m_pNPCScriptWindowPtr->m_NPCDialogCount]);
													m_pNPCScriptWindowPtr->m_NPCDialogCount++;
												}
												else if(npcsubscript->ProcessCondition() == NPC_CONDITION_RESULT_FAIL)
												{
													// ������ �������� ���� �� 
												}
											}
										}
										else
										{
											m_pNPCScriptWindowPtr->m_NPCDialogCount = 0;// 2004.06.20->oneway48 close3-2
											m_pNPCScriptWindowPtr->m_bFirstDialog = FALSE; // 2004.05.01->oneway48 insert
										}
										
									} // if( m_pNPCScriptWindowPtr->m_bFirstDialog )
								} // if(m_pNPCScriptWindowPtr)
							}
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_QA :
						{
							// ���� & �亯
							if( g_LastNPCScriptOwner )
							{
								SetNPCMode( _XNPC_NONE );
								m_indexNPCSubScriptQA = lparam;
									
								if(m_pNPCScriptWindowPtr)
								{
									m_pNPCScriptWindowPtr->ShowWindow(TRUE);
									m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_QANDA));		// _T("���� & �亯")
									m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
									
									m_pNPCScriptWindowPtr->ClearSubScriptList();

									for( int i = 0; i < 4; i++)
									{
										if( npcsubscript->strMessage1[i] != NULL )
										{
											m_pNPCScriptWindowPtr->m_ScriptButton[i]->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
											m_pNPCScriptWindowPtr->m_ScriptButton[i]->SetButtonText(npcsubscript->strMessage1[i]);
											m_pNPCScriptWindowPtr->m_ScriptButton[i]->SetCommandID( _XDEF_NPCSCRIPT_QA );
											m_pNPCScriptWindowPtr->m_ScriptButton[i]->EnableWindow(TRUE);
										}
									}

									m_pNPCScriptWindowPtr->m_ScriptButton[4]->SetButtonTextColor(D3DCOLOR_ARGB(255, 214, 15, 15));
									m_pNPCScriptWindowPtr->m_ScriptButton[4]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_BACK);
									m_pNPCScriptWindowPtr->m_ScriptButton[4]->SetCommandID(_XDEF_NPCSCRIPT_RENEW);
									m_pNPCScriptWindowPtr->m_ScriptButton[4]->EnableWindow(TRUE);
								}
							}
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_LEARNSKILL :
						{
							// ���� ����
							_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
							if( m_pNPCSkillWindowPtr  && g_pSkill_Window && g_LastNPCScriptOwner)// && pMainFrame_Window)
							{
								if(pMainFrame_Window->GetShowStatus())
									pMainFrame_Window->ShowWindow(FALSE);

								// ���� ���İ� �ƴϸ� â ����� ����
								bool bSameClan = false;
								
								if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_MILITARYOFFICER)
								{
									bSameClan = true;
								}

								if( !bSameClan )
								{
									switch(g_pLocalUser->m_CharacterInfo.groupindex)
									{
									case _XGROUP_NANGIN : // 2004.03.23 oneway48 �߰� : ���߿� �Ϲ����� �ƴ϶� �������� �߰�����
										{
											if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_COMMONPEOPLE)
												bSameClan = true;
										}
										break;
									case _XGROUP_GAEBANG :
										{
											if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_GAEBANGPEOPLE)
												bSameClan = true;
										}
										break;
									case _XGROUP_BEEGOONG :
										{
											if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_BEEGOONGPEOPLE)
												bSameClan = true;
										}
										break;
									case _XGROUP_SORIM :
										{
											if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_SORIMPEOPLE)
												bSameClan = true;
										}
										break;
									case _XGROUP_NOCKRIM :
										{
											if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_NOCKRIMPEOPLE)
												bSameClan = true;
										}
										break;
									case _XGROUP_MOODANG :
										{
											if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_MOODANGPEOPLE)
												bSameClan = true;
										}
										break;
									case _XGROUP_MAKYO :
										{
											if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_MAKYOPEOPLE)
												bSameClan = true;
										}
										break;
									}
								}

								if(!bSameClan)
								{
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTLEARNSKILL), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "���� ���İ� �ƴϹǷ� ������ ���� ���� �� �����ϴ�."
									break;
								}
								
								m_pNPCSkillWindowPtr->SetOwnerNPCID(g_LastNPCScriptOwner->m_UniqueID);

								m_pNPCSkillWindowPtr->ResetAllSkillPackageID();
								m_pNPCSkillWindowPtr->SetSkillPackageID(0, npcsubscript->condition1);
								m_pNPCSkillWindowPtr->SetSkillPackageID(1, npcsubscript->condition2);
								
								switch(g_pLocalUser->m_CharacterInfo.Get_cClass()) {
								case _XROLLCLASS_NONE :
									{
										
									}
									break;
								case _XROLLCLASS_BATTLE:
									{
										if( npcsubscript->condition3 != -1 )
											m_pNPCSkillWindowPtr->SetSkillPackageID(2, npcsubscript->condition3);
									}
									break;	
								case _XROLLCLASS_HEALER:
									{
										if( npcsubscript->condition4 != -1 )
											m_pNPCSkillWindowPtr->SetSkillPackageID(3, npcsubscript->condition4);
									}
									break;	
								case _XROLLCLASS_COMPLETE:
									{
										if( npcsubscript->condition5 != -1 )
											m_pNPCSkillWindowPtr->SetSkillPackageID(4, npcsubscript->condition5);
									}
									break;
								case _XROLLCLASS_SPELLER:
									{
										if( npcsubscript->condition6 != -1 )
											m_pNPCSkillWindowPtr->SetSkillPackageID(5, npcsubscript->condition6);
									}
									break;
								case _XROLLCLASS_MURDER:
									{
										if( npcsubscript->condition7 != -1 )
											m_pNPCSkillWindowPtr->SetSkillPackageID(6, npcsubscript->condition7);
									}
									break;
								case _XROLLCLASS_HOSIM:
									{
										if( npcsubscript->condition8 != -1 )
											m_pNPCSkillWindowPtr->SetSkillPackageID(7, npcsubscript->condition8);
									}
									break;
								case _XROLLCLASS_JUSOOL:
									{
										if( npcsubscript->condition9 != -1 )
											m_pNPCSkillWindowPtr->SetSkillPackageID(8, npcsubscript->condition9);
									}
									break;
								case _XROLLCLASS_WOIGONG:
									{
										if( npcsubscript->condition10 != -1 )
											m_pNPCSkillWindowPtr->SetSkillPackageID(9, npcsubscript->condition10);
									}
									break;
								case _XROLLCLASS_SOHWAN:
									{
										if( npcsubscript->condition11 != -1 )
											m_pNPCSkillWindowPtr->SetSkillPackageID(10, npcsubscript->condition1);
									}
									break;
								case _XROLLCLASS_SUNDO:
									{
										if( npcsubscript->condition12 != -1 )
											m_pNPCSkillWindowPtr->SetSkillPackageID(11, npcsubscript->condition12);
									}
									break;
								case _XROLLCLASS_MADO:
									{
//										if( npcsubscript->condition13 != -1 )
//											m_pNPCSkillWindowPtr->SetSkillPackageID(12, npcsubscript->condition13);

									}
									break;
								}
								
								// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 
								SetNPCMode( _XNPC_LEARNSKILL );
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_LEARNSKILL));	// _T("���� ����")
								m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
								
							}
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_SAVEPOINT :
						{
							if( npcsubscript->m_SubScriptConditionList.listEmpty() )
							{
								// ���� Ǯ��
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_SAVEINN), TRUE, _XDEF_NPCSCRIPT_SAVEPOINT, 0);		// _T("�̰��� ������ Ǫ�ðڽ��ϱ�? ���ں�� ��10�� �ʿ��մϴ�.")
								
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_INN));		// _T("���� Ǯ��")
								_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
								m_pNPCScriptWindowPtr->SetMiniMapWindow( pminimapwindow );
								m_pNPCScriptWindowPtr->SetSavePointPosition( g_LastNPCScriptOwner->m_UniqueID );
								m_pNPCScriptWindowPtr->SetMessageType( _XDEF_NPCMESSAGE_SAVEPOINT );
							}
							else
							{
								_XNPCCONDITION_RESULT result = npcsubscript->ProcessCondition();
								if(result == NPC_CONDITION_RESULT_SUCCESS)
								{
									// ���� Ǯ��
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_SAVEINN), TRUE, _XDEF_NPCSCRIPT_SAVEPOINT, 0);		// _T("�̰��� ������ Ǫ�ðڽ��ϱ�? ���ں�� ��10�� �ʿ��մϴ�.")
									
									m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_INN));		// _T("���� Ǯ��")
									_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
									m_pNPCScriptWindowPtr->SetMiniMapWindow( pminimapwindow );
									m_pNPCScriptWindowPtr->SetSavePointPosition( g_LastNPCScriptOwner->m_UniqueID );
									m_pNPCScriptWindowPtr->SetMessageType( _XDEF_NPCMESSAGE_SAVEPOINT );
								}
								else if(result == NPC_CONDITION_RESULT_FAIL)
								{
									// ������ �������� ���� ��
									TCHAR messagestring[256];
									memset(messagestring, 0, sizeof(messagestring));
#ifdef _XDEF_CASTLEBATTLE
									if(g_CBManager.m_nSavePointFailResult == 1)
									{
										_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3306));//������� ���۵Ǿ������ ������ Ǯ �� �ֽ��ϴ�
									}
									else if(g_CBManager.m_nSavePointFailResult == 2)
									{
										_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3307));//��� �����ڸ� ������ Ǯ �� �ֽ��ϴ�
									}
									else if(g_CBManager.m_nSavePointFailResult == 3)
									{
										_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3308));//��� �����ڸ� ������ Ǯ �� �ֽ��ϴ�
									}
									else
									{
										_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3165), g_pLocalUser->m_CharacterInfo.charactername);//%s���� �� ���� ������ Ǯ �� �����ϴ�.
									}
#else
									_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3165), g_pLocalUser->m_CharacterInfo.charactername);//%s���� �� ���� ������ Ǯ �� �����ϴ�.
#endif
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); 
								}
							}
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_CLANSERVICEPOINT :
						{
							// ���� ���İ� �ƴϸ� â ����� ����
							bool bSameClan = false;
							switch(g_pLocalUser->m_CharacterInfo.groupindex)
							{
							case _XGROUP_GAEBANG :
								{
									if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_GAEBANGPEOPLE)
										bSameClan = true;
								}
								break;
							case _XGROUP_BEEGOONG :
								{
									if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_BEEGOONGPEOPLE)
										bSameClan = true;
								}
								break;
							case _XGROUP_SORIM :
								{
									if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_SORIMPEOPLE)
										bSameClan = true;
								}
								break;
							case _XGROUP_NOCKRIM :
								{
									if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_NOCKRIMPEOPLE)
										bSameClan = true;
								}
								break;
							case _XGROUP_MOODANG :
								{
									if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_MOODANGPEOPLE)
										bSameClan = true;
								}
								break;
							case _XGROUP_MAKYO :
								{
									if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_MAKYOPEOPLE)
										bSameClan = true;
								}
								break;
							}
							
							if(!bSameClan)
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTCONTRIBUTION), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);		// "���� ���İ� �ƴϹǷ� �⿩�� �� �� �����ϴ�"
								break;
							}
							// ���ı⿩�� 														
							if( m_pContributionWindowPtr )
							{
								m_pContributionWindowPtr->ResetContributionData();
								
							}

							if(g_pInventory_Window)
							{
								g_pInventory_Window->SetContributionWindowPtr(m_pContributionWindowPtr);
								
							}

							SetNPCMode( _XNPC_CONTRIBUTION );
							m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_DOCONTRIBUTION));		// _T("���� �⿩")
							m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_HEALPLAYER :
						{
							m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_HEALING));
							m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
							// ġ���ϱ�
							DWORD needmoney = g_pLocalUser->m_CharacterInfo.Get_externalwoundrate() + g_pLocalUser->m_CharacterInfo.Get_internalwoundrate();
							if( needmoney > 0 )
							{
								needmoney = (needmoney +1)/2;
							}
							
							if( needmoney == 0 )
							{
								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1864), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );			
							}
							else if( needmoney <= g_pLocalUser->m_Money )
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_HEALMESSAGEWINDOW);
								TCHAR messagestring[256];
								memset( messagestring, 0, sizeof(messagestring) );
								_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1865), needmoney );
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_NPCSCRIPT_HEALOK, 0);
								
							}
							else
							{
								TCHAR messagestring[256];
								memset( messagestring, 0, sizeof(messagestring) );
								_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOMONEY));		// "�������� �����մϴ�."
								g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );			
							}

						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_EXPLAINCLASS :
						{
							// ���Ҽ��� 
							switch( g_CurrentZoneInfoIndex-1 ) 
							{
							case 0: // �Ҹ�
								{
									if( !m_GameExitFlag ) 
									{	
										g_indexScene = 18;
										if( g_SceneList[g_indexScene ] )
										{
											g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
											((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_CLASSEXPLAIN;
											((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
										}
									}
								}
								break;
							case 1: // ����
								{
									if( !m_GameExitFlag ) 
									{	
										g_indexScene = 16;
										if( g_SceneList[g_indexScene ] )
										{
											g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
											((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_CLASSEXPLAIN;
											((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
										}
									}
								}
								break;
							case 2: // �츲
								{
									if( !m_GameExitFlag ) 
									{	
										g_indexScene = 19;
										if( g_SceneList[g_indexScene ] )
										{
											g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
											((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_CLASSEXPLAIN;
											((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
										}
									}
								}
								break;
							case 3: // ���
								{
									if( !m_GameExitFlag ) 
									{	
										g_indexScene = 17;
										if( g_SceneList[g_indexScene ] )
										{
											g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
											((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_CLASSEXPLAIN;
											((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
										}
									}
								}
								break;
							case 4: // ����
								{
									if( !m_GameExitFlag ) 
									{	
										g_indexScene = 31;
										if( g_SceneList[g_indexScene ] )
										{
											g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
											((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_CLASSEXPLAIN;
											((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
										}
									}
								}
								break;
							case 5: // ����
								{
									if( !m_GameExitFlag ) 
									{	
										g_indexScene = 32;
										if( g_SceneList[g_indexScene ] )
										{
											g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
											((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_CLASSEXPLAIN;
											((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
										}
									}
								}
								break;
							} 
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_MAKEORGANIZATION :		// ������ â��
						{
							SetNPCMode( _XNPC_CREATEORGANIZATION );
							m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CREATEORGANIZATION));

							// ���� ���İ� �ƴϸ� â ����� ����
							bool bSameClan = false;
							switch(g_pLocalUser->m_CharacterInfo.groupindex)
							{
							case _XGROUP_GAEBANG :
								{
									if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_GAEBANGPEOPLE)
										bSameClan = true;
								}
								break;
							case _XGROUP_BEEGOONG :
								{
									if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_BEEGOONGPEOPLE)
										bSameClan = true;
								}
								break;
							case _XGROUP_SORIM :
								{
									if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_SORIMPEOPLE)
										bSameClan = true;
								}
								break;
							case _XGROUP_NOCKRIM :
								{
									if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_NOCKRIMPEOPLE)
										bSameClan = true;
								}
								break;
							case _XGROUP_MOODANG :
								{
									if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_MOODANGPEOPLE)
										bSameClan = true;
								}
								break;
							case _XGROUP_MAKYO :
								{
									if(g_LastNPCScriptOwner->m_NPCJobClass == _XNPC_JOBCLASS_MAKYOPEOPLE)
										bSameClan = true;
								}
								break;
							}
	
							if(!bSameClan)
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1866), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
								break;
							}
							
							m_pNPCScriptWindowPtr->ClickOrganizationBtn();
						}
						break;

					case _XDEF_NPCSUBSCRIPTCLASS_MINISTOPEVENT:	// �̴Ͻ����̺�Ʈ
						{
							SetNPCMode( _XNPC_NONE );
							switch( npcsubscript->condition1 ) {
							case 0://SMS �̺�Ʈ
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1901));	
								break;
							case 1://������ �йи���Ʈ
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1868));	
								break;
							case 2://���� ���� �̺�Ʈ
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1869));	
								break;
							case 3://���ȭ �̺�Ʈ
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1870));
								break;
							}
							m_pNPCScriptWindowPtr->m_EventType = npcsubscript->condition1;
							m_pNPCScriptWindowPtr->SetMessageType( _XDEF_NPCMESSAGE_MINISTOPEVENT);
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_SHOWRANKQUEST:	// ����Ʈ ��ŷ���� 
						{
							SetNPCMode( _XNPC_RANKQUEST );
							m_pNPCScriptWindowPtr->SetTitle( _XGETINTERFACETEXT(ID_STRING_NEW_2625) );
							_XWindow_QuestRank* pQuestRank_Window = (_XWindow_QuestRank*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUESTRANKWINDOW);
							if(pQuestRank_Window)
							{
								pQuestRank_Window->SetRankQuestID(npcsubscript->quantitynumber1);// ��ŷ ����Ʈ ���̵�
							}
						}
						break;
						
						
					case _XDEF_NPCSUBSCRIPTCLASS_PVPLIST :
						{
							SetNPCMode(_XNPC_PVPBROKER);
							m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1871));							
							
						}
						break;
#ifdef _XTS_SOCKETEX
					case _XDEF_NPCSUBSCRIPTCLASS_SOCKETEXPANSION :	// ���� Ȯ��
						{
							if( m_pSocketPlugInExWindowPtr )
							{
								// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 
								SetNPCMode( _XNPC_SOCKETPLUGINEX );
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1872));
												
								m_pSocketPlugInExWindowPtr->SetSocketPlugInExMode(SM_EXPAND);
								

								if( g_pInventory_Window )
								{
									g_pInventory_Window->SetSocketPlugInExpWindowPtr(m_pSocketPlugInExWindowPtr);
									
								}							
							}
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_SOCKETDESTROY :	// ���� �и�/�ı�
						{
							if( m_pSocketPlugInExWindowPtr )
							{
								// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 
								SetNPCMode( _XNPC_SOCKETPLUGINEX );
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1873));
								
								m_pSocketPlugInExWindowPtr->SetSocketPlugInExMode(SM_SEPARATION);
								

								if( g_pInventory_Window )
								{
									g_pInventory_Window->SetSocketPlugInExpWindowPtr(m_pSocketPlugInExWindowPtr);
									
								}							
							}							
						}
						break;
#endif
					case _XDEF_NPCSUBSCRIPTCLASS_VIEWFREEMATCHINFO :		// �� ��ȸ ����
						{
							_XWindow_FreeMatchInfo* pFMInfo_Window = (_XWindow_FreeMatchInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_FREEMATCHINFO);
							if(pFMInfo_Window)
							{
								SetNPCMode(_XNPC_FREEMATCHINFO);
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_NEW_3776));//"��ȸ ����"
							}
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_COLLECTION :			// ��ǰ����
						{
							if(m_pNPCCollectionWindowPtr)
							{
								if(!m_pNPCCollectionWindowPtr->GetProcessReward())
								{
									SetNPCMode( _XNPC_COLLECTION );
									m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1877));
									
									if(g_LastNPCScriptOwner)
									{
										m_pNPCCollectionWindowPtr->SetOwnerNPCId(g_LastNPCScriptOwner->m_UniqueID);
									}
								}
							}
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_RESOURCE :				// ������
						{
#ifdef _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI
							if(m_pNPCResource_New_windowptr && !m_pNPCResource_New_windowptr->GetProgressExchange())
							{
								SetNPCMode( _XNPC_RESOURCE );
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1878));
								m_pNPCResource_New_windowptr->SetSelectedResourcePackageIndex( npcsubscript->quantitynumber1 ); // NPC�� ������ �ִ� �ڿ� ��Ű�� �ε���
								m_pNPCResource_New_windowptr->EnableExchangeButton(FALSE);
							}
#else
							if(m_pNPCResourceWindowPtr )
							{
								if (!m_pNPCResourceWindowPtr->GetProgressExchange())
								{
									SetNPCMode( _XNPC_RESOURCE );
									m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1878));
									m_pNPCResourceWindowPtr->SetPackageIndex( npcsubscript->quantitynumber1 ); // NPC�� ������ �ִ� �ڿ� ��Ű�� �ε���
									m_pNPCResourceWindowPtr->m_btnExchange->EnableWindow(FALSE);
									
								}
							}
#endif
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_INCHANT :
						{
							if( m_pInchantWindowPtr )
							{
								_XMessageWindow* pInchantMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INCHANTMESSAGEWINDOW);
								if( pInchantMessageBox )
								{
									if( pInchantMessageBox->GetShowStatus() )
										break;
								}

								// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 
								SetNPCMode( _XNPC_INCHANT );
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1879));								
								m_pInchantWindowPtr->SetItemInchantStep(US_NONE);	
							}							
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_NPCBUFF :
						{
							// ������
							if(m_pNPCBuffWindowPtr)
							{
								SetNPCMode(_XNPC_BUFF);
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1880));
							}
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_MOVEVILLAGE :
						{
							// ǥ�� �̵�
							if(g_pLocalUser->GetUserItemInvenSlotFromSecondType(_XGI_FC_RESOURCE, 4/*_XGI_SC_RESOURCE_MOVE*/) > 0)
							{
								SetNPCMode(_XNPC_MOVEVILLAGE);
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_NEW_3043));
								m_pNPCTradeWindowPtr->SetOwnerNPCID(g_LastNPCScriptOwner->m_UniqueID);
							}
							else
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
								pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3044), TRUE, 0 );	
								pMessageBox->StartTimer(5000, TRUE);
							}
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_CASTLEINFO :
						{
							// �������
							_XWindow_CastleInfo* pCastleInfo_Window = (_XWindow_CastleInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEINFO);
							if(pCastleInfo_Window)
							{
								SetNPCMode(_XNPC_CASTLEINFO);
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_NEW_3310));//��� ����
							}
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_CASTLEATTACKLIST :
						{
							// ��������ڸ��
							_XWindow_ChallengerList* pChallengerList_Window = (_XWindow_ChallengerList*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHALLENGERLIST);
							if(pChallengerList_Window)
							{
								SetNPCMode(_XNPC_CHALLENGEINFO);
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_NEW_3311));//������ ���
							}
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_TRACING :
						{
							// Ư���� ��ġ ����							
							SetNPCMode( _XNPC_NONE );
							m_pNPCScriptWindowPtr->SetMessageType( _XDEF_NPCMESSAGE_TRACING);
							
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_MIXWEAPON :
						{
#ifdef _XDEF_MIXWEAPON_071009_KUKURI
							// ���� ����
							SetNPCMode(_XNPC_MIXWEAPON);
							m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_NEW_3725));//����
#endif
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_ENTERCONTEST :
						{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
							// ��ȸ ����
							SetNPCMode( _XNPC_NONE );
							
							//send packet
							if(!g_pLocalUser->m_InParty)
							{
								g_NetworkKernel.SendPacket(MSG_NO_FREE_MATCH, en_gsfm_apply_match);
							}
							else
							{
								//������·δ� �񹫴�ȸ�� ������ �� �����ϴ�.
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3736), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
							}

							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_CLOSE, 0), 0);

#endif
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_CLANPOINTITEM :
						{
							// ���� �⿩���� ������ ����
							if(m_pNPCTradeWindowPtr && g_pInventory_Window)
							{
								// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 								
								SetNPCMode( _XNPC_ITEMTRADE );
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_TRADE));	// _T("��ǰ �Ÿ�")
								m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);

								m_pNPCTradeWindowPtr->SetOwnerNPCID(g_LastNPCScriptOwner->m_UniqueID);
								m_pNPCTradeWindowPtr->SetItemPackageID(npcsubscript->condition1);
								if(npcsubscript->condition2 != -1) m_pNPCTradeWindowPtr->SetAdditionalItemPackageID(npcsubscript->condition2);
								if(npcsubscript->condition3 != -1) m_pNPCTradeWindowPtr->SetSpecialItemPackageID(npcsubscript->condition3);
								
								m_pNPCTradeWindowPtr->SetBlackMarketMode(FALSE);
								m_pNPCTradeWindowPtr->SetListItem();

								m_pNPCTradeWindowPtr->SetPriceMode(_MODE_CLANPOINT);
								g_pInventory_Window->SetPriceMode(_MODE_CLANPOINT);
															
								g_pInventory_Window->SetNPCTradeWindowPtr(m_pNPCTradeWindowPtr);	
							}
						}
						break;
					#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
					case _XDEF_NPCSUBSCRIPTCLASS_MASTERNAME :
						{
							//��ȣ�Է�
							SetNPCMode( _XNPC_NONE );
							m_pNPCScriptWindowPtr->SetMessageType( _XDEF_NPCMESSAGE_MASTERNAME);
							m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_NEW_3875));	// _T("��ǰ �Ÿ�")
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_MNDINFO :
						{
							//���� ����
							_XWindow_NPCMnDInfo* pNPCMnDInfoWindow = (_XWindow_NPCMnDInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCMNDWINDOW);
							
							if(pNPCMnDInfoWindow)
							{
								SetNPCMode(_XNPC_MASTERINFO);
								m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_NEW_3851)); //���� ����
							}
						}
						break;
					case _XDEF_NPCSUBSCRIPTCLASS_MNDPOINTPURCHASE :
						{
							//���� �⿩���� ������ ����
							if(m_pNPCTradeWindowPtr && g_pInventory_Window)
							{
								// NPC ��ȭ��忡�� �ٸ� â�� �� ��� ȭ��ó���� ���ؼ� 								
								SetNPCMode( _XNPC_ITEMTRADE );
								m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_TRADE));	// _T("��ǰ �Ÿ�")
								m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);

								m_pNPCTradeWindowPtr->SetOwnerNPCID(g_LastNPCScriptOwner->m_UniqueID);
								m_pNPCTradeWindowPtr->SetItemPackageID(npcsubscript->condition1);
								if(npcsubscript->condition2 != -1) m_pNPCTradeWindowPtr->SetAdditionalItemPackageID(npcsubscript->condition2);
								if(npcsubscript->condition3 != -1) m_pNPCTradeWindowPtr->SetSpecialItemPackageID(npcsubscript->condition3);
								
								m_pNPCTradeWindowPtr->SetBlackMarketMode(FALSE);
								m_pNPCTradeWindowPtr->SetListItem();

								m_pNPCTradeWindowPtr->SetPriceMode(_MODE_MNDPOINT);
								g_pInventory_Window->SetPriceMode(_MODE_MNDPOINT);
															
								g_pInventory_Window->SetNPCTradeWindowPtr(m_pNPCTradeWindowPtr);	
							}
						}
						break;
					#endif
					default :
						break;
					}
				}// if(npcsubscript && !g_LastNPCScriptOwner->m_bCheckHostility) end : ���������� �ʰ� NPC�� ����� ������ �ִ� ���
			}// if(g_LastNPCScriptOwner) end
		}
		break;
	case _XDEF_NPCSCRIPT_SAVEPOINT :
		{
			// ���� Ǯ��
			if( g_LastNPCScriptOwner )
			{
				if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_LastNPCScriptOwner->SetConversationAction();
					g_NetworkKernel.SendPacket(MSG_NO_SAVE_RESPAWN_POS_REQUEST, g_LastNPCScriptOwner->m_UniqueID);
					break;
				}

				if( g_pLocalUser->m_Money >= 10)
				{
					g_LastNPCScriptOwner->SetConversationAction();
					g_NetworkKernel.SendPacket(MSG_NO_SAVE_RESPAWN_POS_REQUEST, g_LastNPCScriptOwner->m_UniqueID);
				}
				else
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOLODGINGMONEY), TRUE);	// _T("���ں� ���ڶ��ϴ�.")
					pMessageBox->StartTimer(5000, TRUE) ;
				}
			}
		}
		break;
	case _XDEF_NPCSCRIPT_HEALOK :
		{
			// ġ���ϱ� 
			if( g_LastNPCScriptOwner )
			{
				int needmoney = g_pLocalUser->m_CharacterInfo.Get_externalwoundrate() + g_pLocalUser->m_CharacterInfo.Get_internalwoundrate();
				if( needmoney > 0 )
				{
					needmoney = (needmoney +1)/2;
				}

				g_NetworkKernel.SendPacket(MSG_NO_NPC_TRADE_REQ, en_npc_trade_mode_heal, needmoney);
			}
		}
		break;		
	case _XDEF_NPCSCRIPT_MINISTOPOK	:
		{			
			if( m_pNPCScriptWindowPtr->m_MessageType == _XDEF_NPCMESSAGE_TRACING )
			{
				_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
				if(pPKTracingRequest_Window)
				{
					//if( strcmp( g_pLocalUser->m_CharacterInfo.charactername, pPKTracingRequest_Window->m_cTargetName ) != 0 )
					if( strcmp( g_pLocalUser->m_CharacterInfo.charactername, pPKTracingRequest_Window->m_cSpecialTargetName ) != 0 )
					{
						//��� �̸��� �����Ѵ�.
						//strcpy( pPKTracingRequest_Window->m_cTargetName,m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetText() );
						//Author : �����  �߰�
						memset(pPKTracingRequest_Window->m_cSpecialTargetName, 0, sizeof(pPKTracingRequest_Window->m_cSpecialTargetName));
						strcpy( pPKTracingRequest_Window->m_cSpecialTargetName,m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetText() ); //Ư����
						
						POINT pos = m_pNPCScriptWindowPtr->GetWindowPos();
						pPKTracingRequest_Window->MoveWindow(pos.x+100,pos.y+136);
						pPKTracingRequest_Window->ShowWindow(TRUE);
						
						m_pNPCScriptWindowPtr->m_MiniStopEventIMEBorder1->ShowWindow(FALSE);
						m_pNPCScriptWindowPtr->m_MiniStopEventIMEBorder2->ShowWindow(FALSE);
						m_pNPCScriptWindowPtr->m_btnMiniStopEventInput->ShowWindow(FALSE);
						m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.ClearBuffer();
						_XIMEKERNEL.SetFocus( NULL );	
						_XIMEKERNEL.ResetCombinationBuffer();		
					}
					else
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3403), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );//"�ڽ��� ������ ���� �����ϴ�."
					}		
				}
			}
		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
			else if( m_pNPCScriptWindowPtr->m_MessageType == _XDEF_NPCMESSAGE_MASTERNAME )
			{
				if(g_pLocalUser->m_pLocalMnDInfomation.nMnDClass != 1 )
				{
					g_NetworkKernel.InsertChatString(_T("������ �ƴմϴ�."), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );

					m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.ClearBuffer();
					_XIMEKERNEL.SetFocus( NULL );	
					_XIMEKERNEL.ResetCombinationBuffer();
					break;
				}
				/*if( strlen( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName ))
				{
					g_NetworkKernel.InsertChatString(_T("��ȣ ��û�� �ѹ��� �� �� �ֽ��ϴ�."), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
					
					m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.ClearBuffer();
					_XIMEKERNEL.SetFocus( NULL );	
					_XIMEKERNEL.ResetCombinationBuffer();	
				}
				else*/
				{
					TCHAR szTemp[128];
					memset(szTemp, 0, sizeof(szTemp));
					strcpy( szTemp, m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetText() );

					int nLen = strlen( szTemp );

					if( nLen > 36 )
					{
						g_NetworkKernel.InsertChatString(_T("���ڼ��� �ʰ� �Ͽ����ϴ�."), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
					}
					else
					{					
						memset(szTemp, 0, sizeof(szTemp));
						sprintf( szTemp, "��ȣ ��û�� �ѹ��� �� �� �ֽ��ϴ�.'%s'�� ��ȣ�� �����Ͻðڽ��ϱ�?", m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetText() );

						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), szTemp, TRUE, _XDEF_NPCSCRIPT_MASTERNICKNAMEYES, _XDEF_NPCSCRIPT_MASTERNICKNAMENO);
						pMessageBox->StartTimer(10000, TRUE) ;
					}
				}
			}
		#endif
			else
			{
					
				bool	invenfull = true;
				if( m_pNPCScriptWindowPtr->m_EventType != 2 )
				{
					for( int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; i++)
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
						{
							invenfull = false;
							break;					
						}
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
					if( invenfull )
					{
						for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
						{
							if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
							{
								invenfull = false;
								break;					
							}
						}
					}
				#endif
				}
				else // ���� ���� �̺�Ʈ 
				{
					int		emptyslotcount = 0;
					int		needinvencount = 5; // ���� ���� �̺�Ʈ�� 5ĭ�� ������ �ʿ��ϴ�.
					
					for( int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; i++)
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
						{
							emptyslotcount++;
							if(emptyslotcount == needinvencount)
								break;				
						}
					}

				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
						{
							emptyslotcount++;
							if(emptyslotcount == needinvencount)
								break;				
						}
					}
				#endif

					if(emptyslotcount == needinvencount)
					{
						invenfull = false;
					}
					else
					{
						invenfull = true;	
					}			
				}
				
				if(invenfull)
				{
					if( m_pNPCScriptWindowPtr->m_EventType != 2 )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1881), TRUE);
						pMessageBox->StartTimer(5000, TRUE) ;
						break;
					}
					else// ���� ���� �̺�Ʈ
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1882), TRUE);
						pMessageBox->StartTimer(5000, TRUE) ;
						break;
					}
				}
				
				// �̴Ͻ����̺�Ʈ ��ȣ ������ 
				if(m_pNPCScriptWindowPtr)
				{
					if( m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetText() )
					{
						int stringlength = 0;
						if( m_pNPCScriptWindowPtr->m_EventType != 2 && m_pNPCScriptWindowPtr->m_EventType != 3)
						{
							stringlength = strlen( m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetText() );
						}
						else// ���� ���� �̺�Ʈ , ���ȭ �̺�Ʈ 
						{
							POINT winpos = m_pNPCScriptWindowPtr->GetWindowPos();
							POINT pt = m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetWindowPosition();
							if( pt.x == winpos.x+47 )
							{
								strncpy( m_pNPCScriptWindowPtr->m_EventCodeString01,m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetText(),5);
							}
							else if( pt.x == winpos.x+97 )
							{
								strncpy( m_pNPCScriptWindowPtr->m_EventCodeString02,m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetText(),5);
							}
							else if( pt.x == winpos.x+147 )
							{
								strncpy( m_pNPCScriptWindowPtr->m_EventCodeString03,m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetText(),5);
							}
							int eventcodelength01 = strlen( m_pNPCScriptWindowPtr->m_EventCodeString01 );
							int eventcodelength02 = strlen( m_pNPCScriptWindowPtr->m_EventCodeString02 );
							int eventcodelength03 = strlen( m_pNPCScriptWindowPtr->m_EventCodeString03 );

							stringlength = eventcodelength01 + eventcodelength02 + eventcodelength03;
							
						}
						//int stringlength = strlen( m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetText() );
						if( stringlength == 0 )
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1883), TRUE);						
							pMessageBox->StartTimer(5000, TRUE) ;
						}
						else
						{
							int maxstringlength = 0;

							switch(m_pNPCScriptWindowPtr->m_EventType) {
							case 0://����� ���� �̺�Ʈ
								maxstringlength = 10;
								break;
							case 1://�ѹи���Ʈ ������ �̺�Ʈ
								maxstringlength = 8;
								break;
							case 2://���డ�� �̺�Ʈ 
								maxstringlength = 15;
								break;
							case 3://���ȭ �̺�Ʈ 
								maxstringlength = 15;
								break;
							default:
								maxstringlength = 15;
								break;
							}

							if( stringlength == maxstringlength)
							{
								//��Ŷ������ 
								if( m_pNPCScriptWindowPtr->m_EventType ==2 )// ���డ�� �̺�Ʈ
								{
									if( g_pLocalUser->m_CharacterInfo.gender == _XGENDER_MAN )// ����
									{
										_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_EVENTMESSAGEWINDOW);
										
										pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1884), TRUE, _XDEF_NPCSCRIPT_EVENTREWARDYES, _XDEF_NPCSCRIPT_EVENTREWARDNO);
										break;
									}
									else // ����
									{
										_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_EVENTMESSAGEWINDOW);
										
										pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1885), TRUE, _XDEF_NPCSCRIPT_EVENTREWARDYES, _XDEF_NPCSCRIPT_EVENTREWARDNO);
										break;
									}
								}
								else
								{
									g_NetworkKernel.SendPacket(MSG_NO_EVENT_REQUEST);
									m_pNPCScriptWindowPtr->m_MiniStopEventIMEBorder1->ShowWindow(FALSE);
									m_pNPCScriptWindowPtr->m_MiniStopEventIMEBorder2->ShowWindow(FALSE);
									m_pNPCScriptWindowPtr->m_btnMiniStopEventInput->ShowWindow(FALSE);
								}
							
							}
							else
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1886), TRUE);
								pMessageBox->StartTimer(5000, TRUE) ;
							}
						}
					}
				}				
			}
		}
		break;
	case _XDEF_NPCSCRIPT_EVENTREWARDYES		:		
		{
			if( m_pNPCScriptWindowPtr )
			{
				
				g_NetworkKernel.SendPacket(MSG_NO_EVENT_REQUEST);

				m_pNPCScriptWindowPtr->m_EventIMEBorder1->ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_EventIMEBorder2->ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_btnMiniStopEventInput->ShowWindow(FALSE);	
			}
		}
		break;
	case _XDEF_NPCSCRIPT_EVENTREWARDNO		:		
		{
			if( m_pNPCScriptWindowPtr )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1887), TRUE);
				pMessageBox->StartTimer(5000, TRUE) ;
				
				m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.ClearBuffer();
				_XIMEKERNEL.SetFocus( NULL );	
				_XIMEKERNEL.ResetCombinationBuffer();			
				
				m_pNPCScriptWindowPtr->m_EventIMEBorder1->ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_EventIMEBorder2->ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_btnMiniStopEventInput->ShowWindow(FALSE);			
			}
		}
		break;
	case _XDEF_NPCSCRIPT_NPCDIALOGNEXTPAGE		:		
		{
			if( m_pNPCScriptWindowPtr )
			{
				m_pNPCScriptWindowPtr->m_bPageClickQuestDialog = TRUE;
				
				m_pNPCScriptWindowPtr->m_btnDialogNextPage->ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_imageDialogNextPageLeftBorder->ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_imageDialogNextPageRightBorder->ShowWindow(FALSE);			
			}
		}
		break;
	case _XDEF_NPCSCRIPT_QUESTOK :
		{
			// ����Ʈ ��ȭ�� Ȯ�� ��ư
			
			if( g_LastNPCScriptOwner )
			{
				g_LastNPCScriptOwner->SetConversationAction();
				
				_XNPCMainScript* npcmainscriptitem = (_XNPCMainScript*)g_LastNPCScriptOwner->m_Triggerlist.getItem(g_LastNPCScriptOwner->m_MainScriptIndex );
				
				m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
				if(!npcmainscriptitem->m_SubScriptList.listEmpty())
				{
					int			subscriptcount = 0;
					_XNPCScriptItem*		subscript;
					npcmainscriptitem->m_SubScriptList.resetList();
					m_pNPCScriptWindowPtr->ClearSubScriptList();
					do 
					{
						subscript = (_XNPCScriptItem *)npcmainscriptitem->m_SubScriptList.currentItem();
						m_pNPCScriptWindowPtr->SetSubScriptList(subscriptcount, subscript->scriptclass);
						
						npcmainscriptitem->m_SubScriptList.nextItem();
						subscriptcount++;
					} while(!npcmainscriptitem->m_SubScriptList.atHeadOfList());
					m_pNPCScriptWindowPtr->m_EventType = npcmainscriptitem->m_EventType;
					m_pNPCScriptWindowPtr->GenerateSubScriptList();
				}
				else
				{
					m_pNPCScriptWindowPtr->ClearSubScriptList();
				}
			}
		}
		break;
	case _XDEF_NPCSCRIPT_BACKTONPCMODE :
		{
			// ����Ʈ ��ȭ ���� �Ŀ�  NPC ������� ���ư��� ��ư
			if( g_LastNPCScriptOwner )
			{
				g_LastNPCScriptOwner->m_MessageType = _XDEF_NPCMESSAGE_NORMAL;
				
				_XNPCMainScript* npcmainscriptitem = (_XNPCMainScript*)g_LastNPCScriptOwner->m_Triggerlist.getItem(g_LastNPCScriptOwner->m_MainScriptIndex );
				
				m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
				m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_TALK));		// _T("��   ȭ")
				if(!npcmainscriptitem->m_SubScriptList.listEmpty())
				{
					int			subscriptcount = 0;
					_XNPCScriptItem*		subscript;
					npcmainscriptitem->m_SubScriptList.resetList();
					m_pNPCScriptWindowPtr->ClearSubScriptList();
					do 
					{
						subscript = (_XNPCScriptItem *)npcmainscriptitem->m_SubScriptList.currentItem();
						m_pNPCScriptWindowPtr->SetSubScriptList(subscriptcount, subscript->scriptclass);
						
						npcmainscriptitem->m_SubScriptList.nextItem();
						subscriptcount++;
					} while(!npcmainscriptitem->m_SubScriptList.atHeadOfList());
					m_pNPCScriptWindowPtr->m_EventType = npcmainscriptitem->m_EventType;
					m_pNPCScriptWindowPtr->GenerateSubScriptList();
				}
				else
				{
					m_pNPCScriptWindowPtr->ClearSubScriptList();
				}

				// ����Ʈ ��ȭ�� �������� �˸���.
				if(g_LastNPCScriptOwner->m_bStartQuestDialog) 
				{
					g_LastNPCScriptOwner->m_bStartQuestDialog = FALSE;
					m_pNPCScriptWindowPtr->m_btnNPCDialogModExit->ShowWindow(TRUE);
				}
			}
			
		}
		break;
	case _XDEF_NPCSCRIPT_LISTBOXQUESTREWARD :
		{
			// ����Ʈ ���� ����Ʈ 
		}
		break;
	case _XDEF_NPCSCRIPT_LISTBOXSELECTION :
		{
			// ����Ʈ ������ ����Ʈ 
			
			int selectionnumber = (DWORD)lparam -1;
			int questid			= m_pNPCScriptWindowPtr->GetHaveQuestID();
			
			_XQuestScriptItem* currentquest = NULL;
			if( questid >= 10000 && questid < 20000  )
			{
				map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(questid);
				if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
					currentquest = iter_quest->second;
			}
			else
			{
				map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(questid);
				if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
					currentquest = iter_quest->second;
			}
			
			if( currentquest )
			{
				currentquest->m_SelectionNumber = selectionnumber;
				currentquest->m_bSelectionNumberCheck = TRUE ;	
				
				_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT->SERVER)     >>>>>>>>>>>>>>>>>>>>" );
				_XLog("Check Selection Number: QuestID[%d] QuestNodeID[%d] SelectionNumber[%d]", questid, currentquest->GetCurrentTreeNumber(), selectionnumber );
				
				// ������ ����Ʈ ��ȣ  ������ ��ȣ�� �ѱ��.
				g_NetworkKernel.SendPacket(MSG_NO_QUEST_SET_SELECTION_NO,questid, selectionnumber);
				m_pNPCScriptWindowPtr->m_listboxSelectionItem->DeleteAllItem();
				m_pNPCScriptWindowPtr->m_listboxSelectionItem->ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_btnQuestOK->ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_btnBackToNPCMode->ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_NPCDialogSubBorderB1->ShowWindow(FALSE);
				m_pNPCScriptWindowPtr->m_NPCDialogSubBorderB2->ShowWindow(FALSE);
			//	m_pNPCScriptWindowPtr->m_btnNPCDialogModExit->ShowWindow(FALSE);
			}
			
		}
		break;
	case _XDEF_NPCSCRIPT_LISTBOXHAVEQUESTLIST	:
		{			
#ifdef _XDEF_SHUTDOWN_20061027
			// �÷��� ���� �ð��� ������ ����Ʈ�� ���� �� �� ����.
			if( g_PlayLeftTime <= 0 ) break;// g_PlayLeftTime�� 0�� �ƴϸ� ����
#endif
			g_QuestScriptManager.m_indexSelectNPCTargettingQuest = (DWORD)lparam -1;
			g_QuestScriptManager.QuestStartConditionNPCTargettingMode();
		}
		break;
	case _XDEF_NPCSCRIPT_MASTERNICKNAMEYES :
		{
		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
			if( strlen(m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetText()) == 0 )
			{
				g_NetworkKernel.InsertChatString(_T("��ȣ�� �Է����� �ʾҽ��ϴ�."), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
				break;
			}

			memset( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, 0, sizeof(g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName));
			strcpy( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.GetText() );

			m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.ClearBuffer();
			_XIMEKERNEL.SetFocus( NULL );	
			_XIMEKERNEL.ResetCombinationBuffer();

			g_NetworkKernel.SendPacket(MSG_NO_MASTER_AND_PUPIL, en_cg_mp_mode_change_master_nick_req);
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_CLOSE, 0), 0);
		#endif
		}
		break;
	case _XDEF_NPCSCRIPT_MASTERNICKNAMENO :
		{
		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
			g_NetworkKernel.InsertChatString(_T("��ȣ ��û�� ����Ͽ����ϴ�."), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );

			m_pNPCScriptWindowPtr->m_MiniStopEventIMEControl.ClearBuffer();
			_XIMEKERNEL.SetFocus( NULL );	
			_XIMEKERNEL.ResetCombinationBuffer();

			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_CLOSE, 0), 0);
		#endif
		}
		break;
		
	}
}

void XProc_MainGame::NPCTradeWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_NPCTRADE_HELP :
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_NPCBUSS );
		}
		break;
	case _XDEF_NPCTRADE_CLOSE :
		{
			if(m_pNPCTradeWindowPtr)
			{				
				m_pNPCTradeWindowPtr->ShowWindow(FALSE);
				if(g_pInventory_Window)
				{
					if(g_pInventory_Window->GetShowStatus())
						g_pInventory_Window->ShowWindow(FALSE);
				}
			}
		}
		break;
	case _XDEF_NPCTRADE_ONE	:
		{
			m_pNPCTradeWindowPtr->ChangeTab(_XTRADETAB_ONE);			
		}
		break;
	case _XDEF_NPCTRADE_TWO	:
		{
			m_pNPCTradeWindowPtr->ChangeTab(_XTRADETAB_TWO);
		}
		break;
	case _XDEF_NPCTRADE_THREE :
		{
			m_pNPCTradeWindowPtr->ChangeTab(_XTRADETAB_THREE);
		}
		break;
	case _XDEF_NPCTRADE_FOUR :
		{
			m_pNPCTradeWindowPtr->ChangeTab(_XTRADETAB_FOUR);
		}
		break;
	case _XDEF_NPCTRADE_FIVE :
		{
			m_pNPCTradeWindowPtr->ChangeTab(_XTRADETAB_FIVE);
		}
		break;
	case _XDEF_NPCTRADE_BUYOK :
		{
			if(m_pNPCTradeWindowPtr)
			{
				m_pNPCTradeWindowPtr->m_bHaveMouseSlot = FALSE;
				m_pNPCTradeWindowPtr->ProcessBuy();
			}
		}
		break;		
	case _XDEF_NPCTRADE_SELLOK :
		{
			if(m_pNPCTradeWindowPtr)
			{
				if(g_pInventory_Window)
				{					
					if(g_pInventory_Window->m_MouseSlotItem.IsStackable())
					{						
						_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
						if(pTradeConfirm_Window)
						{
							pTradeConfirm_Window->ShowWindow(TRUE);
							pTradeConfirm_Window->m_IMEControl.SetFocus();
							pTradeConfirm_Window->m_SelectedCategory1 = g_pInventory_Window->m_MouseSlotItem.Get_m_cType();
							pTradeConfirm_Window->m_SelectedItemID = g_pInventory_Window->m_MouseSlotItem.Get_m_sID();
							pTradeConfirm_Window->m_InventoryNumber = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber; //�Ǹ� ������ ���� �߰��� //Author : �����
							pTradeConfirm_Window->m_TradeMode = _XNPCTRADE_SELL;
						}
						break;
					}
					g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				}
				m_pNPCTradeWindowPtr->ProcessSell();
				
			}
		}
		break;
		// 2004.05.31->oneway48 insert
	case _XDEF_NPCTRADE_BUYNO :
		{
			if(m_pNPCTradeWindowPtr)
			{	
				m_pNPCTradeWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
				m_pNPCTradeWindowPtr->m_bHaveMouseSlot = FALSE;

				if( g_pDefaultTooltip_Window )
				{
					if( g_pDefaultTooltip_Window->GetShowStatus() )						
						g_pDefaultTooltip_Window->ShowWindow( FALSE );
				}
			}
		}
		break;
		
	case _XDEF_NPCTRADE_SELLNO :
		{
			if(m_pNPCTradeWindowPtr)
			{
				if(g_pInventory_Window)
				{
					g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
					g_pInventory_Window->m_bHaveMouseSlot = FALSE;
					
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )						
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
					}
				}
			}
		}
		break;
		// insert end		
	default :
		break;
	}
}

void XProc_MainGame::TradeConfirmWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);

	switch(wparam)
	{
	case _XDEF_TRADECONFIRM_CLOSE :
		{
			if(pTradeConfirm_Window)
			{
				if(g_pInventory_Window)
				{
					//if(g_pInventory_Window->GetShowStatus() && g_pInventory_Window->m_bHaveMouseSlot)
					if(g_pInventory_Window->m_bHaveMouseSlot)
					{
						g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
						g_pInventory_Window->m_bWasteBasketLock = FALSE; //Author : ����� //breif : �������� �̹��� ��
					}
				}

				if(pTradeConfirm_Window->GetShowStatus())
				{
					pTradeConfirm_Window->ShowWindow(FALSE);

					if(g_pInventory_Window)
					{
						if(g_pInventory_Window->GetShowStatus() && g_pInventory_Window->m_bHaveMouseSlot)
						{
							g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
							g_pInventory_Window->m_bHaveMouseSlot = FALSE;
						}
					}
					if(m_pNPCTradeWindowPtr)
					{
						if(m_pNPCTradeWindowPtr->GetShowStatus() && m_pNPCTradeWindowPtr->m_bHaveMouseSlot)
						{
							m_pNPCTradeWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
							m_pNPCTradeWindowPtr->m_bHaveMouseSlot = FALSE;
						}
					}
					if(m_pPCTradeWindowPtr)
					{
						if(m_pPCTradeWindowPtr->GetShowStatus() && m_pPCTradeWindowPtr->m_bHaveMouseSlot)
						{
							m_pPCTradeWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
							m_pPCTradeWindowPtr->m_bHaveMouseSlot = FALSE;
						}
					}
					if(m_pWarehouseWindowPtr)
					{
						if(m_pWarehouseWindowPtr->GetShowStatus() && m_pWarehouseWindowPtr->m_bHaveMouseSlot)
						{
							m_pWarehouseWindowPtr->m_MouseSlotItem.ResetMouseSlotItem();
							m_pWarehouseWindowPtr->m_bHaveMouseSlot = FALSE;
						}
					}
					_XIMEKERNEL.SetFocus( NULL );
				}				
			}
		}
		break;
	case _XDEF_TRADECONFIRM_ALL :
		{
			if(pTradeConfirm_Window)
			{
				switch(pTradeConfirm_Window->m_TradeMode)
				{
				case _XDROP_ITEM :
				case _XDROP_WASTEBASKET_ITEM :	
					{
						if(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].IsStackable())
							pTradeConfirm_Window->m_TradeCount = g_pInventory_Window->m_MouseSlotItem.Get_m_ucCount();
					}
					break;
					
				case _XDROP_MONEY :				
					{
						pTradeConfirm_Window->m_TradeCount = g_pLocalUser->m_Money;
					}
					break;
					// NPC Trade -------------------
				case _XNPCTRADE_BUY :
				case _XNPCTRADE_SELL :
					{
						if(m_pNPCTradeWindowPtr)
						{
							if(m_pNPCTradeWindowPtr->GetShowStatus())
							{
								if(pTradeConfirm_Window->m_TradeMode == _XNPCTRADE_BUY)
								{
									pTradeConfirm_Window->m_TradeCount = 255;									
								}
								else if(pTradeConfirm_Window->m_TradeMode == _XNPCTRADE_SELL)
								{
									pTradeConfirm_Window->m_TradeCount = g_pInventory_Window->m_MouseSlotItem.Get_m_ucCount();
								}
							}
						}
					}
					break;
					
					// â�� -----------------
				case _XWAREHOUSE_TOWAREHOUSE_ITEM :
					{
						// �κ� -> â�� (������)
						if(m_pWarehouseWindowPtr && g_pInventory_Window)
						{
							if(m_pWarehouseWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								pTradeConfirm_Window->m_TradeCount = g_pInventory_Window->m_MouseSlotItem.Get_m_ucCount();
							}
						} // if(m_pWarehouseWindowPtr && g_pInventory_Window)
					}
					break;
				case _XWAREHOUSE_FROMWAREHOUSE_ITEM :
					{
						// â�� -> �κ� (������)
						if(m_pWarehouseWindowPtr && g_pInventory_Window)
						{
							if(m_pWarehouseWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								pTradeConfirm_Window->m_TradeCount = m_pWarehouseWindowPtr->m_MouseSlotItem.Get_m_ucCount();
							}
						} // if(m_pWarehouseWindowPtr && g_pInventory_Window)
					}
					break;
				case _XWAREHOUSE_TOWAREHOUSE_MONEY :
					{
						// �κ� -> â�� (��)
						if(m_pWarehouseWindowPtr && g_pInventory_Window)
						{
							if(m_pWarehouseWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								pTradeConfirm_Window->m_TradeCount = g_pLocalUser->m_Money;
							}
						}
					}
					break;
				case _XWAREHOUSE_FROMWAREHOUSE_MONEY :
					{
						// â�� -> �κ� (��)
						if(m_pWarehouseWindowPtr && g_pInventory_Window)
						{
							if(m_pWarehouseWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								pTradeConfirm_Window->m_TradeCount = g_pLocalUser->m_WarehouseMoney;
							}
						}
					}
					break;
					
					// PC�� �ŷ� ------------------------------
				case _XPCTRADE_FROMTRADE_ITEM :
					{
						if(m_pPCTradeWindowPtr && g_pInventory_Window)
						{
							if(m_pPCTradeWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								pTradeConfirm_Window->m_TradeCount = m_pPCTradeWindowPtr->m_MouseSlotItem.Get_m_ucCount();
							}
						}
					}
					break;
				case _XPCTRADE_TOTRADE_ITEM :
					{
						if(m_pPCTradeWindowPtr && g_pInventory_Window)
						{
							if(m_pPCTradeWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								pTradeConfirm_Window->m_TradeCount = g_pInventory_Window->m_MouseSlotItem.Get_m_ucCount();
								/*
								int invennumber = m_pPCTradeWindowPtr->m_SelfTradeItem[pTradeConfirm_Window->m_PCTradeSlotIndex].m_InventoryNumber;
								if( invennumber >= 20 )
								{
									pTradeConfirm_Window->m_TradeCount = g_pLocalUser->m_UserItemList[invennumber].m_ucCount;									
								}*/							
							}
						}
					}
					break;				
				case _XPCTRADE_TOTRADE_MONEY :
					{
						if(m_pPCTradeWindowPtr && g_pInventory_Window)
						{
							if(m_pPCTradeWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								pTradeConfirm_Window->m_TradeCount = g_pLocalUser->m_Money;
							}
						}
					}
					break;
				case _XPCTRADE_FROMTRADE_MONEY :
					{
						if( m_pPCTradeWindowPtr && g_pInventory_Window )
						{
							if(m_pPCTradeWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								pTradeConfirm_Window->m_TradeCount = m_pPCTradeWindowPtr->m_SelfMoney;
							}
						}
					}
					break;
				case _XCONTRIBUTION_TOCONTRIBUTION_ITEM :
					{
						if( m_pContributionWindowPtr && g_pInventory_Window )						
						{
							if( m_pContributionWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus() )
							{
								pTradeConfirm_Window->m_TradeCount = g_pInventory_Window->m_MouseSlotItem.Get_m_ucCount();
							}
						}
					}
					break;
				case _XCONTRIBUTION_TOCONTRIBUTION_MONEY :
					{
						if( m_pContributionWindowPtr && g_pInventory_Window )
						{
							if( m_pContributionWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus() )
							{
								pTradeConfirm_Window->m_TradeCount = g_pLocalUser->m_Money;								
							}
						}
					}
					break;
				// ���� ���� �ŷ� ------------------------------
				case _XPERSONALSTORE_TOTRADE_ITEM :
					{
						if(m_pPersonalStoreWindowPtr && g_pInventory_Window)
						{
							if(m_pPersonalStoreWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								pTradeConfirm_Window->m_TradeCount = g_pInventory_Window->m_MouseSlotItem.Get_m_ucCount();								
							}
						}
					}
					break;				
				} // switch(pTradeConfirm_Window->m_TradeMode)
				TCHAR tempstring[128];				
				memset( tempstring, 0, sizeof(tempstring) );
				//itoa(pTradeConfirm_Window->m_TradeCount, tempstring, 10);
				_snprintf( tempstring, sizeof(tempstring), _T("%u"), pTradeConfirm_Window->m_TradeCount );
				
				int length = strlen(tempstring);
				if( pTradeConfirm_Window->m_IMEControl.GetFocus() )
				{
					_XIMEKERNEL.ResetCombinationBuffer();
				}
				pTradeConfirm_Window->m_IMEControl.ClearBuffer();
				pTradeConfirm_Window->m_IMEControl.AddString(tempstring, length );
			}
		}
		break;
	case _XDEF_TRADECONFIRM_OK :
		{
			// send packet
			if(pTradeConfirm_Window)
			{
				switch(pTradeConfirm_Window->m_TradeMode)
				{
				case _XDROP_ITEM :
					{
						if(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].IsStackable())
						{
							if(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_ucCount() < pTradeConfirm_Window->m_TradeCount)
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_EXCESSHAVEITEM), TRUE);
							}
							else if(pTradeConfirm_Window->m_TradeCount <= 0)
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
							}
							else
							{
								if( g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber != -1)
									g_NetworkKernel.SendPacket(MSG_NO_INVEN_DROP, g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber, pTradeConfirm_Window->m_TradeCount);

								pTradeConfirm_Window->ShowWindow(FALSE);
								g_pInventory_Window->m_bHaveMouseSlot = FALSE;
								g_pInventory_Window->m_bDragState = FALSE;
								g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
							}
						}
						else
						{
							if( g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber != -1)
								g_NetworkKernel.SendPacket(MSG_NO_INVEN_DROP, g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber, 1);
							
							g_pInventory_Window->m_bHaveMouseSlot = FALSE;
							g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
						}						
					}
					break;
					
				case _XDROP_MONEY :
					{
						if(g_pLocalUser->m_Money < pTradeConfirm_Window->m_TradeCount)
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_EXCESSHAVEMONEY), TRUE);
						}
						else if(pTradeConfirm_Window->m_TradeCount <= 0)
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
						}
						else
						{
							g_NetworkKernel.SendPacket(MSG_NO_MONEY_DROP, pTradeConfirm_Window->m_TradeCount);
							pTradeConfirm_Window->ShowWindow(FALSE);
						}
					}
					break;
				// NPC Trade -------------------
				case _XNPCTRADE_BUY :
				case _XNPCTRADE_SELL :
					{
						if(m_pNPCTradeWindowPtr)
						{
							if(m_pNPCTradeWindowPtr->GetShowStatus())
							{
								if(pTradeConfirm_Window->m_TradeMode == _XNPCTRADE_BUY)
								{
									if(pTradeConfirm_Window->m_TradeCount > 255)
									{
										_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
										pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_EXCESS), TRUE);
//										g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_EXCESS), TRUE);
									}
									else if( pTradeConfirm_Window->m_TradeCount <= 0 )
									{
										_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
										pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
									}
									else
									{
										m_pNPCTradeWindowPtr->m_MouseSlotItem.Set_m_ucCount((unsigned char)pTradeConfirm_Window->m_TradeCount);
										m_pNPCTradeWindowPtr->ProcessBuy();
									}
								}
								else if(pTradeConfirm_Window->m_TradeMode == _XNPCTRADE_SELL)
								{
									if(pTradeConfirm_Window->m_TradeCount > 255)
									{
										_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
										pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_EXCESS), TRUE);
									}
									else if( pTradeConfirm_Window->m_TradeCount <= 0 )
									{
										_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
										pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
									}
									else
									{
										g_pInventory_Window->m_MouseSlotItem.Set_m_ucCount(pTradeConfirm_Window->m_TradeCount);
										m_pNPCTradeWindowPtr->ProcessSell();
									}
								}
								pTradeConfirm_Window->ShowWindow(FALSE);
							}
						}
					}
					break;

				// â�� -----------------
				case _XWAREHOUSE_TOWAREHOUSE_ITEM :
					{
						// �κ� -> â�� (������)
						if(m_pWarehouseWindowPtr && g_pInventory_Window)
						{
							if(m_pWarehouseWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								if(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_ucCount() < pTradeConfirm_Window->m_TradeCount)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_MANY, TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_MANY, TRUE);
								}
								else if(pTradeConfirm_Window->m_TradeCount <= 0)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
								}
								else
								{
									m_pWarehouseWindowPtr->SetTempItemFromInven(g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber, pTradeConfirm_Window->m_TradeCount);
									PostMessage(gHWnd, WM_COMMAND, _XDEF_WAREHOUSEWINDOW_SENDADDITEM, (LPARAM)gHWnd);
								}
							}
						} // if(m_pWarehouseWindowPtr && g_pInventory_Window)
					}
					break;
				case _XWAREHOUSE_FROMWAREHOUSE_ITEM :
					{
						// â�� -> �κ� (������)
						if(m_pWarehouseWindowPtr && g_pInventory_Window)
						{
							if(m_pWarehouseWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								if(g_pLocalUser->m_WarehouseItemList[m_pWarehouseWindowPtr->m_MouseSlotItem.m_InventoryNumber].Get_m_ucCount() < pTradeConfirm_Window->m_TradeCount)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_MANY, TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_MANY, TRUE);
								}
								else if(pTradeConfirm_Window->m_TradeCount <= 0)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
								}
								else
								{
									m_pWarehouseWindowPtr->SetTempItemFromWarehouse(m_pWarehouseWindowPtr->m_MouseSlotItem.m_InventoryNumber, pTradeConfirm_Window->m_TradeCount);
									PostMessage(gHWnd, WM_COMMAND, _XDEF_WAREHOUSEWINDOW_SENDADDITEM, (LPARAM)gHWnd);
								}
							} 
						} // if(m_pWarehouseWindowPtr && g_pInventory_Window)
					}
					break;
				case _XWAREHOUSE_TOWAREHOUSE_MONEY :
					{
						// �κ� -> â�� (��)
						if(m_pWarehouseWindowPtr && g_pInventory_Window)
						{
							if(m_pWarehouseWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								if(g_pLocalUser->m_Money < pTradeConfirm_Window->m_TradeCount)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
								}
								else if(pTradeConfirm_Window->m_TradeCount <= 0)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
								}
								else
								{
									ULONGLONG tempmoney;
									tempmoney = (ULONGLONG)g_pLocalUser->m_WarehouseMoney + (ULONGLONG)pTradeConfirm_Window->m_TradeCount;
									if( tempmoney >= 0xFFFFFFFF )
									{
										// overflowed dword boundary
										g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3789), TRUE);
									}
									else
									{
										m_pWarehouseWindowPtr->m_TempMoney = pTradeConfirm_Window->m_TradeCount;
										m_pWarehouseWindowPtr->m_SendMode = _XSENDMODE_TOWAREHOUSE_MONEY;
										g_NetworkKernel.SendPacket(MSG_NO_STR_ITEM_SAVE, 1);
									}
								}
							}
						}
					}
					break;
				case _XWAREHOUSE_FROMWAREHOUSE_MONEY :
					{
						// â�� -> �κ� (��)
						if(m_pWarehouseWindowPtr && g_pInventory_Window)
						{
							if(m_pWarehouseWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								if(g_pLocalUser->m_WarehouseMoney < pTradeConfirm_Window->m_TradeCount)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
								}
								else if(pTradeConfirm_Window->m_TradeCount <= 0)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
								}
								else
								{
									ULONGLONG tempmoney;
									tempmoney = (ULONGLONG)g_pLocalUser->m_Money + (ULONGLONG)pTradeConfirm_Window->m_TradeCount;
									if( tempmoney >= 0xFFFFFFFF )
									{
										// overflowed dword boundary										
										g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3789), TRUE);
									}
									else
									{
										m_pWarehouseWindowPtr->m_TempMoney = pTradeConfirm_Window->m_TradeCount;
										m_pWarehouseWindowPtr->m_SendMode = _XSENDMODE_TOINVEN_MONEY;
										
										
										// 2004.06.02->oneway48 insert
										if( !m_pWarehouseWindowPtr->m_bTerminateWarehouse )
											g_NetworkKernel.SendPacket(MSG_NO_STR_ITEM_EJECT, 1);
										else
											g_NetworkKernel.SendPacket(MSG_NO_STR_ITEM_EJECTEX, 1);;
									}
								}
							}
						}
					}
					break;

					// PC�� �ŷ� ------------------------------
				case _XPCTRADE_TOTRADE_ITEM :
					{
						if(m_pPCTradeWindowPtr && g_pInventory_Window)
						{
							if(m_pPCTradeWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								int restcount = 0, movecount = 0;
								int index = pTradeConfirm_Window->m_PCTradeSlotIndex;
								int invenindex = m_pPCTradeWindowPtr->m_SelfTradeItem[index].m_InventoryNumber;
								if(g_pLocalUser->m_UserItemList[invenindex].Get_m_ucCount() < pTradeConfirm_Window->m_TradeCount)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_MANY, TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_MANY, TRUE);
								}
								else if(pTradeConfirm_Window->m_TradeCount <= 0)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
								}
								else
								{
									g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_ADD, index, pTradeConfirm_Window->m_TradeCount);
									m_pPCTradeWindowPtr->CopyItemToSlot(index, invenindex);
									m_pPCTradeWindowPtr->m_SelfTradeItem[index].Set_m_ucCount(pTradeConfirm_Window->m_TradeCount);
									m_pPCTradeWindowPtr->m_SelfTradeItem[index].m_InventoryNumber = invenindex;
									g_pLocalUser->m_UserItemList[invenindex].Set_m_ucCount(g_pLocalUser->m_UserItemList[invenindex].Get_m_ucCount() - pTradeConfirm_Window->m_TradeCount);
									
									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
									if(g_pLocalUser->m_UserItemList[invenindex].Get_m_ucCount() <= 0)
									{
										g_pLocalUser->m_UserItemList[invenindex].Reset();
									}
								}
							}
						}
					}
					break;
				case _XPCTRADE_FROMTRADE_ITEM :
					{
						if(m_pPCTradeWindowPtr && g_pInventory_Window)
						{
							if(m_pPCTradeWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								int index = pTradeConfirm_Window->m_PCTradeSlotIndex;
								int invenindex = m_pPCTradeWindowPtr->m_SelfTradeItem[index].m_InventoryNumber;
								if(m_pPCTradeWindowPtr->m_SelfTradeItem[index].Get_m_ucCount() < pTradeConfirm_Window->m_TradeCount)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_MANY, TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_MANY, TRUE);
								}
								else if(pTradeConfirm_Window->m_TradeCount <= 0)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
								}
								else
								{
									g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_DELETE, index, pTradeConfirm_Window->m_TradeCount);
								}
							}
						}
					}
					break;
				case _XPCTRADE_TOTRADE_MONEY :
					{
						if(m_pPCTradeWindowPtr && g_pInventory_Window)
						{
							if(m_pPCTradeWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								if(g_pLocalUser->m_Money < pTradeConfirm_Window->m_TradeCount)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
								}
								else if( pTradeConfirm_Window->m_TradeCount <= 0 )
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
								}
								else
								{
									g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_MONEY, 1, pTradeConfirm_Window->m_TradeCount);
								}
							}
						}
					}
					break;
				case _XPCTRADE_FROMTRADE_MONEY :
					{	
						if(m_pPCTradeWindowPtr && g_pInventory_Window)
						{
							if(m_pPCTradeWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								if(m_pPCTradeWindowPtr->m_SelfMoney < pTradeConfirm_Window->m_TradeCount)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
//									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
								}
								else if( pTradeConfirm_Window->m_TradeCount <= 0 )
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
								}
								else
								{
									g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_MONEY, 0, pTradeConfirm_Window->m_TradeCount);
								}
							}
						}
					}
				break;				
				case _XCONTRIBUTION_TOCONTRIBUTION_ITEM :
					{
						if( m_pContributionWindowPtr && g_pInventory_Window )
						{
							if( m_pContributionWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus() )
							{
								if(g_pInventory_Window->m_MouseSlotItem.Get_m_ucCount() < pTradeConfirm_Window->m_TradeCount)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
								}
								else if( pTradeConfirm_Window->m_TradeCount <= 0 )
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
								}
								else
								{
									m_pContributionWindowPtr->m_ContributionType			= g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType();
									m_pContributionWindowPtr->m_ContributionSecondType		= g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cSecond();
									m_pContributionWindowPtr->m_ContributionId				= g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sID();
									m_pContributionWindowPtr->m_ContributionIvenIndex		= g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;										
									m_pContributionWindowPtr->m_ContributionItemCount		= pTradeConfirm_Window->m_TradeCount;
									
									g_pInventory_Window->m_ContributionItemInvenNumber = m_pContributionWindowPtr->m_ContributionIvenIndex;
									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();										
									
									m_pContributionWindowPtr->SetAddData();
								}
							}								
						}						
					}
					break;
				case _XCONTRIBUTION_TOCONTRIBUTION_MONEY :
					{
						if( m_pContributionWindowPtr && g_pInventory_Window )
						{
							if( m_pContributionWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus() )
							{
								if(g_pLocalUser->m_Money < pTradeConfirm_Window->m_TradeCount)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
								}
								else if( pTradeConfirm_Window->m_TradeCount < 5 )
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
								}
								else if( pTradeConfirm_Window->m_TradeCount <= 0 )
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
								}
								else
								{
									m_pContributionWindowPtr->m_ContributionType			= _XGI_FC_MONEY;					
									m_pContributionWindowPtr->m_ContributionId				= 0;
                                #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
                                    m_pContributionWindowPtr->m_ContributionIvenIndex		= _XINVENTORY_SLOTTYPE_MONEYINDEX - 1;
                                #else
									m_pContributionWindowPtr->m_ContributionIvenIndex		= 126;
                                #endif
									m_pContributionWindowPtr->m_ContributionItemCount		= pTradeConfirm_Window->m_TradeCount;
									m_pContributionWindowPtr->SetAddData();
								}
							}
						}
					}
					break;
				// ���� ���� �ŷ� ------------------------------
				case _XPERSONALSTORE_TOTRADE_ITEM :
					{
						if(m_pPersonalStoreWindowPtr && g_pInventory_Window)
						{
							if(m_pPersonalStoreWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus())
							{
								int restcount = 0, movecount = 0;
								int index = pTradeConfirm_Window->m_PersonalStoreSlotIndex;
								int invenindex = m_pPersonalStoreWindowPtr->m_TradeItem[index].m_InventoryNumber;
								if(g_pLocalUser->m_UserItemList[invenindex].Get_m_ucCount() < pTradeConfirm_Window->m_TradeCount)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_MANY, TRUE);
								}
								else if(pTradeConfirm_Window->m_TradeCount <= 0)
								{
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
								}
								else
								{
									m_pPersonalStoreWindowPtr->CopyItemToSlot(index, invenindex);
									m_pPersonalStoreWindowPtr->m_TradeItem[index].Set_m_ucCount( pTradeConfirm_Window->m_TradeCount );
									m_pPersonalStoreWindowPtr->m_TradeItem[index].m_InventoryNumber = invenindex;
									m_pPersonalStoreWindowPtr->m_SelectedSlotNumber = index;
									m_pPersonalStoreWindowPtr->m_TradeItemPrice[m_pPersonalStoreWindowPtr->m_SelectedSlotNumber] = 0;
									m_pPersonalStoreWindowPtr->m_TradeItemPriceIMEControl.SetFocus();
									m_pPersonalStoreWindowPtr->m_btnInputOK->ShowWindow(TRUE);
									
									g_pLocalUser->m_UserItemList[invenindex].Set_m_ucCount(g_pLocalUser->m_UserItemList[invenindex].Get_m_ucCount() - pTradeConfirm_Window->m_TradeCount);
									
									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
									if(g_pLocalUser->m_UserItemList[invenindex].Get_m_ucCount() <= 0)
									{
										g_pLocalUser->m_UserItemList[invenindex].Reset();
									}
								}
							}
						}
					}
					break;
#ifdef _XDEF_WASTEBASKET_20070226
				case _XDROP_WASTEBASKET_ITEM ://������ ������ - ĳ�� ������
					{	
						if(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_ucCount() < pTradeConfirm_Window->m_TradeCount)
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_EXCESSHAVEITEM), TRUE);
						}
						else if(pTradeConfirm_Window->m_TradeCount <= 0)
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_INPUTNUMBER), TRUE);
						}
						else
						{
							if( g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber != -1)
								g_NetworkKernel.SendPacket(MSG_NO_INVEN_DROP, g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber, pTradeConfirm_Window->m_TradeCount,10);
							
							pTradeConfirm_Window->ShowWindow(FALSE);
							g_pInventory_Window->m_bHaveMouseSlot = FALSE;
							g_pInventory_Window->m_bDragState = FALSE;
							g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
						}
						
						g_pInventory_Window->m_bWasteBasketLock = FALSE; //Author : ����� //breif : �������� �̹��� ��
					}
					break;
#endif
				} // switch(pTradeConfirm_Window->m_TradeMode)	

				pTradeConfirm_Window->ShowWindow(FALSE);
				if( g_pInventory_Window )
				{
					if( g_pInventory_Window->GetShowStatus() )
					{
						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
						g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
					}				
				}

			}
		} // case _XDEF_TRADECONFIRM_OK :
		break;
	}
}


void XProc_MainGame::RequestPartyWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
	
	switch(wparam)
	{	
	case _XDEF_REQUESTPARTY_CLOSE :
		{
			if(pRequestParty_Window)
			{
				if(pRequestParty_Window->GetShowStatus())				
				   pRequestParty_Window->ShowWindow(FALSE);				
			}
		}
		break;
	/////////////////////////////////////////////////////////////////////////////////////////
	// Party member
	case _XDEF_REQUESTPARTY_REQUESTPARTY :
		{
			if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_2420), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				break;
			}
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_party))
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_2420), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				break;
			}
#endif
#ifdef _XTS_PK
			if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2626), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
				break;
			}
#endif

			if( !g_pLocalUser->m_InParty )
			{
				if(pRequestParty_Window)
				{
					g_NetworkKernel.SendPacket(MSG_NO_PARTY_JOIN_REQ);
										
					if(pRequestParty_Window->GetShowStatus())
						pRequestParty_Window->ShowWindow(FALSE);
				}
			}
			else if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
			{
				if( g_pLocalUser->m_PartyMemberCount > 0 && g_pLocalUser->m_PartyMemberCount <= 9  )
				{
					if( !g_pLocalUser->m_bPartyQuestProgress )
					{
						g_NetworkKernel.SendPacket(MSG_NO_PARTY_JOIN_REQ);
						
						if(pRequestParty_Window->GetShowStatus())
							pRequestParty_Window->ShowWindow(FALSE);
					}
					else
					{
						g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1888), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
					}
				}
				else
				{				
					g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTADDPARTY), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );	// _T("�� �̻� ��Ƽ���� �߰��� �� �����ϴ�.")
				}
			}
			
		}
		break;

	case _XDEF_REQUESTPARTY_JOINPARTYOK :
		{
			g_NetworkKernel.SendPacket(MSG_NO_PARTY_RESPONSE, 0);
		}
		break;
	case _XDEF_REQUESTPARTY_JOINPARTYNO :
		{
			g_NetworkKernel.SendPacket(MSG_NO_PARTY_RESPONSE, 1);
		}
		break;

	/////////////////////////////////////////////////////////////////////////////////////////
	// Whisper chatting...
	case _XDEF_REQUESTPARTY_SENDMESSAGE :
		{
			_XWindow_ChattingDefault* defaultchatwindow = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );			
			if( defaultchatwindow )
			{
				if( pRequestParty_Window )
				{	
					memset( defaultchatwindow->m_WhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );
					
					if( strlen(pRequestParty_Window->m_PickedCharacterName) )
					{
#ifdef _XDEF_CASHITEM_SHOUT
						extern D3DCOLOR g_ChatFunctionButtonStringColor[7];
#else
						extern D3DCOLOR g_ChatFunctionButtonStringColor[6];
#endif

						if( !m_ChatEditContainer.GetShowStatus() || !m_ChatEditContainer.GetFocus() )
						{
							m_ChatEditContainer.ClearBuffer();						
							m_ChatEditContainer.ShowWindow( TRUE );
							m_ChatEditContainer.SetFocus();										
							defaultchatwindow->m_MovingChatWindow = TRUE;
							defaultchatwindow->SetDrawBorder(TRUE);
						}
#ifdef _XDEF_NEWSTRINGMANAGER
						defaultchatwindow->m_btnChatFunction->SetButtonTextID( ID_STRING_MAINGAMEMESSAGEHANDLER_2421,g_ChatFunctionButtonStringID[4] );
#else
						defaultchatwindow->m_btnChatFunction->SetButtonText( g_ChatFunctionButtonString[4] );
#endif
						defaultchatwindow->m_btnChatFunction->SetButtonTextColor( g_ChatFunctionButtonStringColor[4] );						
						defaultchatwindow->InsertRecentWhisperUserList(pRequestParty_Window->m_PickedCharacterName, 3);
						strcpy( defaultchatwindow->m_WhisperTargetName, pRequestParty_Window->m_PickedCharacterName );
						defaultchatwindow->SetChatMode(_XCHATMODE_WHISPER);
					}
				}
				pRequestParty_Window->ShowWindow(FALSE);//2004.06.22->oneway48 insert
			}			
		}
		break;

	/////////////////////////////////////////////////////////////////////////////////////////
	// PC Trade
	case _XDEF_REQUESTPARTY_REQUESTTRADE :
		{
			if( m_bNPCDialogMode || m_bNPCDialogWideView || m_bFunctionalObjectWideView )
			{
				// npc ��ȭ���̰ų�, functional wide view(������ ��)�� �� ��ȯ �Ұ���
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3693), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//������ ��ȯ ��û�� �� �� �����ϴ�.
				break;
			}
			
			if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_2421), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				break;
			}

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_trade))
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_2421),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//���߿��� ��ȯ�� ��û�� �� �����ϴ�
				break;
			}
#endif

#ifdef _XTS_PK
			if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2627), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
				pRequestParty_Window->ShowWindow(FALSE);
				break;
			}
#endif
			if(g_pLocalUser->m_bFaceMaskMode)	// ���� ��
			{
				if( g_pState_Window )
					g_pState_Window->PrintSystemMessage(9);
				pRequestParty_Window->ShowWindow(FALSE);
				break;
			}
			if( m_pPCTradeWindowPtr && m_pPCTradeWindowPtr->m_bTrading )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ERRORTRADE1), TRUE);	// "��ȯ�� �� ������� �� �� �ֽ��ϴ�."
				pMessageBox->StartTimer(10000, TRUE);
			}
			else
			{
				// �ŷ� ��û
				if(pRequestParty_Window && m_pPCTradeWindowPtr)
				{
					if(m_pPCTradeWindowPtr)
					{
						_XUser* pUser = g_Userlist.FindUser( pRequestParty_Window->m_PickedCharacterID );
						
						if( !pUser ) return;
						
						if( pUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE ) // ��� ������ ���� �������̸� �ŷ���û �Ұ�
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);							
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1889), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
							_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1889) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
							
							m_pPCTradeWindowPtr->m_bRequest = FALSE;
							m_pPCTradeWindowPtr->m_bTrading = FALSE;
							g_pLocalUser->m_PvpNPCLock = FALSE;
							return;
						}
						if( pUser->m_bFaceMaskMode )
						{
							if( g_pState_Window )
								g_pState_Window->PrintSystemMessage(9);
							pRequestParty_Window->ShowWindow(FALSE);
							return;
						}						
						// ��밡 ���, ����, ���� ���¸� ��ȯ��û �Ұ� 
						if( pUser->GetMotionClass() == _XACTION_DYING || pUser->GetMotionClass() == _XACTION_DIE || 
							pUser->GetMotionClass() == _XACTION_INSENSIBLE)
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ERRORTRADE2),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�����̻����� ���Ͽ� ����ڰ� �ŷ��� �Ұ��� �մϴ�."
							return;
						}
							
					}

					if( g_pLocalUser->m_Lock.tradelock == 1 )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ERRORTRADE2),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "�����̻����� ���Ͽ� ����ڰ� �ŷ��� �Ұ��� �մϴ�."
					}
					else if( g_pLocalUser->m_bInternalUse  )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1890),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
					else if( g_pLocalUser->m_bSocketPlugIn )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1891),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
					else if( g_pLocalUser->m_bGambleBoxOpen )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1892),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
					else if( g_pLocalUser->m_bIsBossBattle )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1893),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);	
					}
					else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						
						g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1894), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
						_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1894) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
					}
					else
					{
						if( m_pPCTradeWindowPtr->m_bTrading )
						{
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ERRORTRADE2), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );//_T("��ȯ ������Դϴ�.")
						}
						else
						{
							m_pPCTradeWindowPtr->m_TargetID = pRequestParty_Window->m_PickedCharacterID;
							g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_REQ, SUB_MSG_TRADE_REQEST);
							pRequestParty_Window->ShowWindow(FALSE);
							m_pPCTradeWindowPtr->m_bRequest = TRUE;
							g_pLocalUser->m_PvpNPCLock = TRUE;
							m_pPCTradeWindowPtr->m_bTrading = TRUE;
							g_pInventory_Window->SetPCTradeWindowPtr(m_pPCTradeWindowPtr);
							
							TCHAR	messagestring[256];
							memset(messagestring, 0, sizeof(messagestring));
							_XUser* pUser = g_Userlist.FindUser(m_pPCTradeWindowPtr->m_TargetID);
							_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_REQTRADE), pUser->m_CharacterInfo.charactername );	// "%s�Կ��� �ŷ��� ��û�ϴ� ���Դϴ�..."
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_PCTRADEMESSAGEWINDOW);
							if( pMessageBox )
							{
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_CANCEL, _T(""), messagestring, TRUE, _XDEF_PCTRADEWINDOW_NO  );
								pMessageBox->StartTimer(10000, TRUE);						
							}
						}						
					}					
				}
			}		
		}
		break;	
	case _XDEF_REQUESTPARTY_REQUESTFRIEND :
		{
			// ģ�� ��û
			_XUser* pUser = g_Userlist.FindUser( pRequestParty_Window->m_PickedCharacterID );						
			if( pUser ) 
			{
				if( pUser->m_bFaceMaskMode )
				{
					if( g_pState_Window )
						g_pState_Window->PrintSystemMessage(12);
					pRequestParty_Window->ShowWindow(FALSE);
					return;
				}
			}
			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			
			if( pMessenger_Window->m_TotalCountFriends <= 50) // �ִ� 50����� ����
			{
				_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
				if(pRequestParty_Window)
				{
					if(pRequestParty_Window->m_PickedCharacterName[0] != 0)
						strcpy( pMessenger_Window->m_CharacterName, pRequestParty_Window->m_PickedCharacterName);
				}
				
				// �߰��� ģ���� �̹� ��ϵǾ� �ִ� ģ������ �˻�.
				bool checkfriendname = FALSE;
				
				if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
				{
					pMessenger_Window->m_MessengerGroupList.resetList();
					_XMessengerGroup* currentgroup = NULL;
					do 
					{
						currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
						if(currentgroup)
						{
							if(!currentgroup->m_FriendList.listEmpty())
							{
								currentgroup->m_FriendList.resetList();
								_XMessengerFriend* currentfriend = NULL;
								do 
								{
									currentfriend = (_XMessengerFriend*)currentgroup->m_FriendList.currentItem();
									if(currentfriend)
									{
										if( strcmp( currentfriend->m_FriendName,  pMessenger_Window->m_CharacterName ) == 0 )
										{
											checkfriendname = TRUE;												
											break;									
										}
									}
									currentgroup->m_FriendList.nextItem();							
								} while(!currentgroup->m_FriendList.atHeadOfList());						
							}					
						}			
						pMessenger_Window->m_MessengerGroupList.nextItem();
					} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());
				}
				
				if( !checkfriendname )
				{
					g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAKE_FRIEND_REQ_C_S_C );
				}
				else
				{
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(messagestring));
					
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_REGISTALREADY));
					g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
			}
			else
			{
				TCHAR messagestring[256];
				memset(messagestring, 0, sizeof(messagestring));
				
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_EXCESSPERSON) );
				g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				
			}
			pRequestParty_Window->ShowWindow(FALSE);
			
		}
		break;
	case _XDEF_REQUESTPARTY_FRIENDOK :
		{
			g_NetworkKernel.SendMessengerPacket(MMSG_NO_MAKE_FRIEND_CON_C_S, 0);		
		}
		break;
	case _XDEF_REQUESTPARTY_FRIENDNO :
		{
			g_NetworkKernel.SendMessengerPacket(MMSG_NO_MAKE_FRIEND_CON_C_S, 1);
		}
		break;
	case _XDEF_REQUESTPARTY_JOINBOSSBATTLE :
		{
			BOOL bResult = FALSE;
			if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
			{
				bResult = TRUE;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3833), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_CharacterLog.isTrade )
			{
				bResult = TRUE;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3834), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
			{
				bResult = TRUE;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3835), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_bReadingBook )
			{
				bResult = TRUE;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3836), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_bInternalUse )
			{
				bResult = TRUE;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3837), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_bSocketPlugIn )
			{
				bResult = TRUE;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3838), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
			{
				bResult = TRUE;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3839), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
			{
				bResult = TRUE;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3840), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}

			if( bResult ) //���
				g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC, 9, 0);
			else
				g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC, 9, 1);
		}
		break;
	case _XDEF_REQUESTPARTY_REQUESTBOSSBATTLE : //������� �������� �ź��������� �ϴ� ���� ����.
		{
			g_NetworkKernel.SendPacket(MSG_NO_PARTY_ETC, 9, 0);
		}
		break;
	case _XDEF_REQUESTPARTY_SOLOBOSSBATTLE :
		{
			g_NetworkKernel.SendPacket( MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S, g_ChannelTempZoneInfoIndex );
		}
		break;
	case _XDEF_REQUESTPARTY_REQPVP :
		{
			if(pRequestParty_Window)
			{
				if(g_pLocalUser->m_bFaceMaskMode)	// ���� ���� ��
				{
					if( g_pState_Window )
						g_pState_Window->PrintSystemMessage(6);
					break;
				}		

				_XUser* pFindUser = g_Userlist.FindUser(pRequestParty_Window->m_PickedCharacterID);
				if( pFindUser )	// ������ ���� ��
				{
					if( pFindUser->m_bFaceMaskMode )
					{
						if( g_pState_Window )
							g_pState_Window->PrintSystemMessage(7);
						break;
					}
				}
				TCHAR messagestring[256];
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1896), pRequestParty_Window->m_PickedCharacterName);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_MATCH_REQUESTPVP, 0);
				
				g_pLocalUser->m_MatchTargetID = pRequestParty_Window->m_PickedCharacterID;
			}
			
			// 1:1 �⺻ �� ��û
/*			g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, 0, 0);

			_XUser* pUser = NULL;
			if(pRequestParty_Window)
			{
				pUser = (_XUser*)g_Userlist.FindUser(pRequestParty_Window->m_PickedCharacterID);
				pRequestParty_Window->ShowWindow(FALSE);
			}

			if(pUser)
			{
				g_pLocalUser->m_RotateAngle = _XMeshMath_GetRotateRadian(g_pLocalUser->m_Position, pUser->m_Position);
				g_pLocalUser->UpdatePosition();
			}
			g_pLocalUser->SetAnimation(_XACTION_MATCH_START);
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);*/
		}
		break;
	case _XDEF_REQUESTPARTY_REQPVPGROUP :
		{
			if(pRequestParty_Window)
			{
				TCHAR messagestring[256];
				_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1897), pRequestParty_Window->m_PickedCharacterName);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, _XDEF_MATCH_REQUESTPVPGROUP, 0);
			}
		}
		break;
	case _XDEF_REQUESTPARTY_REQGROUP :
		{
			if(pRequestParty_Window)
			{
				_XUser* pUser = NULL;
				pRequestParty_Window->ShowWindow(FALSE);
				pUser = (_XUser*)g_Userlist.FindUser(pRequestParty_Window->m_PickedCharacterID);			
			
				if(pUser)
				{
					if( pUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
					{
						if( strcmp(pUser->m_CharacterInfo.m_OrganizationInfo.m_cName, g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName ) == 0 )
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1898), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
						}
						else //if( strcmp(pUser->m_CharacterInfo.m_OrganizationInfo.m_cName, g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName ) != 0 )
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1899), TRUE);							
							pMessageBox->StartTimer(5000, TRUE) ;
						}						
					}
					else if( pUser->m_CharacterInfo.groupindex != g_pLocalUser->m_CharacterInfo.groupindex )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1900), TRUE);
						pMessageBox->StartTimer(5000, TRUE) ;
					}
					else
					{
						if( g_pGroup_Window )
						{
							if( g_pGroup_Window->GetTotalMemberCount() < 40 )
							{
								g_pGroup_Window->SetSendTargetName(pUser->m_CharacterInfo.charactername);
								g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_CLIENT, MSG_ORGANIZATION_CLIENT_::_OR_REQ_INVITE_);
							}
							else
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1659), TRUE);
								pMessageBox->StartTimer(5000, TRUE) ;
							}
						}
					}
				}
			}			
		}
		break;
		
#ifdef _XTS_ALIANCE_20061018
	case _XDEF_REQUESTPARTY_REQALIENCE			:		//������ ������ �����Ѵ�.
		{
			if(!pRequestParty_Window) break;
			_XUser* pUser = NULL;
			pRequestParty_Window->ShowWindow(FALSE);
			pUser = (_XUser*)g_Userlist.FindUser(pRequestParty_Window->m_PickedCharacterID);			
			if( !pUser)	break;
			
			// ������ ������ ������ ������ ���� ������ �������� �˻��Ѵ�.
			// ��밡 ������ �������� �˻縦 �Ѵ�.
			if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 )
			{
				if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex == pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex )
				{
					//���� ������ ���� �Ҽ��̴�.
					TCHAR message[256];
					memset( message, 0, sizeof(message) );
					_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3185));			//_T("���� ��ȸ �Ҽ��Դϴ�.") 
					g_NetworkKernel.InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					break;
				}

				if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass != _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_ )
				{
					//ȸ�ָ� ���� ������ �����ϴ�.
					TCHAR message[256];
					memset( message, 0, sizeof(message) );
					_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3186));			//_T("ȸ�ָ� ���� ������ �����մϴ�.") 
					g_NetworkKernel.InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					break;

				}
				else
				{
					if( g_pGroup_Window->m_listAlienceMember.size() > 5 )
					{
						//�ִ� 5���� �����ܱ����� ������ ���� �� �ֽ��ϴ�.
						TCHAR message[256];
						memset( message, 0, sizeof(message) );
						_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3187) );			//_T("�ִ� 5���� �����ܱ����� ������ ���� �� �ֽ��ϴ�.")
						g_NetworkKernel.InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
						break;
					}
				}
				
			}
			else	//�������̸� ������ 
			{
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
				{
					if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex == pUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex )
					{
						//���� ������ �Ҽ��̴�.
						TCHAR message[256];
						memset( message, 0, sizeof(message) );
						_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3188));			//_T("���� �����ܿ� �ҼӵǾ� �ֽ��ϴ�.") 
						g_NetworkKernel.InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
						break;
					}
					
					if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass != _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_  )
					{
						//���ָ� ���� ������ �����ϴ�.
						TCHAR message[256];
						memset( message, 0, sizeof(message));
						_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3189));			//_T("���ָ� ���� ������ �����մϴ�.") 
						g_NetworkKernel.InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
						break;
					}

				}
				else
				{
					//�����ܿ� �ҼӵǾ� ���� �ʴ�.
					TCHAR message[256];
					memset( message, 0, sizeof(message) );
					_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3183));			//_T("�����ܿ� �ҼӵǾ� ���� �ʽ��ϴ�.") 
					g_NetworkKernel.InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					break;
				}


			}
			if( pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex  > 0 )
			{
				//�̹� ������ ���Ϳ� �ҼӵǾ� �ִ�.
				TCHAR message[256];
				memset( message, 0, sizeof(message) );
				_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3190));			//_T("�̹� ��ȸ�� ���� �ֽ��ϴ�.") 
				g_NetworkKernel.InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				break;
			}

			if( pUser->m_CharacterInfo.m_OrganizationInfo.m_cClass != _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_  )
			{
				//����� ������ ���ְ� �ƴմϴ�.
				TCHAR message[256];
				memset( message, 0, sizeof(message) );
				_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3191) );			//_T("����� ������ ���ְ� �ƴմϴ�.")
				g_NetworkKernel.InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				break;
			}
			
			//���� ���� �˻�
			if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
				g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
				g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
			{
				if( pUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					pUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					pUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
				{
					TCHAR message[256];
					memset( message, 0, sizeof(message) );
					_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3192) );			//_T("�浵�� �鵵�� ��ȸ�� ���� �� �����ϴ�.")
					g_NetworkKernel.InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				}

			}
			else if( g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG || 
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM ||
					g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG )
			{
				if( pUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG || 
					pUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM ||
					pUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
				{
					TCHAR message[256];
					memset( message, 0, sizeof(message) );
					_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3184) );			//_T("�浵�� �鵵�� ��ȸ�� ���� �� �����ϴ�.")
					g_NetworkKernel.InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				}
			}
			 

			//��û�� ����� ����ũ ���̵� ������ ������.
			g_NetworkKernel.SendPacket(MSG_NO_ALLY_BASE, en_ally_mode_request_ally, pRequestParty_Window->m_PickedCharacterID);
		
		}
		break;
#endif
	}
}


void XProc_MainGame::SystemMenuWindowMessageHandling( WPARAM wparam, LPARAM lparam )
{
	// ��� ���³� ���� �����϶� �������̽� ���� ����
	if(g_pLocalUser->GetMotionClass() == _XACTION_DYING || g_pLocalUser->GetMotionClass() == _XACTION_DIE)
		return;

	switch(wparam)
	{
	case _XDEF_MAINMENU_MAINWINDOW				:
		{
			_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
			_XWindow_SocketPlugIn* pSocketPlugIn_Window = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN);
			
			if( pMainFrame_Window && g_pInventory_Window )
			{
				if( pMainFrame_Window->GetShowStatus() )
				{					
					pMainFrame_Window->ShowWindow( FALSE );			
					
					if( pPCTrade_Window && pPCTrade_Window->GetShowStatus() )
					{
						
					}
					else if( g_pInventory_Window->GetShowStatus() )
					{
						g_pInventory_Window->SetDefaultPosition();
					}
				}
				else
				{
					pMainFrame_Window->ShowWindow( TRUE );
					g_MainWindowManager.SetTopWindow( pMainFrame_Window );

					if( pPCTrade_Window && pPCTrade_Window->GetShowStatus() )
					{

					}
					else if( pSocketPlugIn_Window && pSocketPlugIn_Window->GetShowStatus() )
					{
						
					}
					else if( g_pInventory_Window->GetShowStatus() )
					{
						if( pMainFrame_Window->GetCurrentSelectedTab() == _XDEF_MAINFRAME_CHARACTORTAB )
						{
							POINT pos = pMainFrame_Window->GetWindowPos();
							SIZE size = pMainFrame_Window->GetWindowSize();
							g_pInventory_Window->MoveWindow( pos.x + size.cx, pos.y );
							g_pInventory_Window->SetExpandDockingBorderFlag(FALSE);
							g_pInventory_Window->m_ExpandDockingBorder->ShowWindow( FALSE );
						}
					}
				}
			}
		}
		break;
	case _XDEF_MAINMENU_INVENTORY				:
		{
			// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
			if( g_TutorialInfo )
			{
				if( !g_TutorialInfo->_XTutorial_023 )
				{
					_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
					pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_023);
					g_TutorialInfo->_XTutorial_023 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
					g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif					
				}
			}			
			// ----------------------------------------------------------------------------------------------------------------------------=			

			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			if( pMessenger_Window->GetShowStatus() ) pMessenger_Window->ShowWindow(FALSE);	
			
			_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
			if( pEmail_Window->GetShowStatus() ) pEmail_Window->ShowWindow(FALSE);
			
			if( g_pInventory_Window && g_pInventory_Window->GetShowStatus() )
			{
				g_pInventory_Window->SetExpandDockingBorderFlag(FALSE);
				g_pInventory_Window->ShowWindow( FALSE );				
				g_pInventory_Window->m_ExpandDockingBorder->ShowWindow( FALSE );
			}
			else
			{
				g_pInventory_Window->SetDefaultPosition();
				g_pInventory_Window->ShowWindow( TRUE );				
				_XWindow_MainMenu* pMainmenuwindow = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
				g_MainWindowManager.SetTopWindow(pMainmenuwindow);
				_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
				g_MainWindowManager.SetTopWindow(pMainFrame_Window);
			}
			
		}
		break;
	case _XDEF_MAINMENU_OPTION					:
		{
			_XWindow* optionwindow = g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTIONMENU );
			if( optionwindow )
			{
				if( optionwindow->GetShowStatus() )
				{
					optionwindow->ShowWindow( FALSE );
				}
				else
				{
					optionwindow->ShowWindow( TRUE );
					g_MainWindowManager.SetTopWindow( optionwindow );
				}					
			}
		}
		break;
	case _XDEF_MAINMENU_HEDEK					:
		{
			_XWindow_HelpBook* pHelpBookWindow = (_XWindow_HelpBook*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_HELPBOOK);	
			if( pHelpBookWindow )
			{
#ifdef _XTS_SHOWTUTORIAL
				pHelpBookWindow->ShowWindow( !pHelpBookWindow->GetShowStatus() );
#else
				pHelpBookWindow->ShowWindow( FALSE );
#endif
			}			
		}
		break;
	case _XDEF_MAINMENU_MESSENGER				:
		{
			
			if( !g_NetworkKernel.m_bMessengerServerConnected ) 
			{
				if( g_NetworkKernel.m_ConnectTryCount < 4)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAIGAME_RECONNECTINGMESSENGER),  _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				}
				else
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTCONNECTMSGSERVER),  _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);		// _T("�޽��� ������ ������ �� �����ϴ�.")
				}
				break;
			}
			
			if( !g_NetworkKernel.m_bMessengerServerLoginSuccess )
			{
				if( g_NetworkKernel.m_ConnectTryCount < 4)
				{
					g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAIGAME_CONNECTMESSENGER), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL );
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_LOGINGMSGSERVER),  _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);	// _T("�޽��� ������ �α��� �� �Դϴ�.")
				}
				else
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTLOGINMSGSERVER),  _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// _T("�޽��� ������ �α��� �� �� �����ϴ�.")
				}
				break;
			}
			
			_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
			if( pMessenger_Window)
			{
				if( g_pInventory_Window->GetShowStatus() ) g_pInventory_Window->ShowWindow(FALSE);
				
				_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
				if( pEmail_Window->GetShowStatus() ) pEmail_Window->ShowWindow(FALSE);

				if( pMessenger_Window->GetShowStatus() )	// click 2
				{					
					pMessenger_Window->ShowWindow( FALSE );
				}
				else	// click 1
				{						
					pMessenger_Window->ShowWindow( TRUE );			
					
					_XWindow_MainMenu* pMainmenuwindow = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
					g_MainWindowManager.SetTopWindow(pMainmenuwindow);				
				}			
				
			}
		}
		break;
	case _XDEF_MAINMENU_EMAIL					:		
		{
			if( !g_NetworkKernel.m_bMessengerServerConnected ) 
			{
				if( g_NetworkKernel.m_ConnectTryCount < 4)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAIGAME_RECONNECTINGMESSENGER),  _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
				}
				else
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTCONNECTMSGSERVER),  _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// _T("�޽��� ������ ������ �� �����ϴ�.")
				}
				break;
			}
			
			if( !g_NetworkKernel.m_bMessengerServerLoginSuccess )
			{
				if( g_NetworkKernel.m_ConnectTryCount < 4)
				{
					g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAIGAME_CONNECTMESSENGER), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL );
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_LOGINGMSGSERVER),  _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);	// _T("�޽��� ������ �α��� �� �Դϴ�.")
				}
				else
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTLOGINMSGSERVER),  _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// _T("�޽��� ������ �α��� �� �� �����ϴ�.")
				}
				break;
			}

			
			_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
			if( pEmail_Window)
			{
				if( g_pInventory_Window->GetShowStatus() ) g_pInventory_Window->ShowWindow(FALSE);
				
				_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
				if( pMessenger_Window->GetShowStatus() ) pMessenger_Window->ShowWindow(FALSE);	
				
				if( pEmail_Window->GetShowStatus() )	// click 2
				{					
					pEmail_Window->ShowWindow( FALSE );					
				}
				else	// click 1
				{						
					pEmail_Window->ShowWindow( TRUE );
					
					_XWindow_MainMenu* pMainmenuwindow = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
					g_MainWindowManager.SetTopWindow(pMainmenuwindow);
				}
			}			
		}
		break;
	case _XDEF_MAINMENU_MEDBUTTON :
		{			
			if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE ) // ������ ���õ� �ൿ�� �ϰ� ���� ��
			{				
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1902), TRUE);
				pMessageBox->StartTimer(5000, TRUE) ;
			}
			else if(g_pLocalUser->m_UseSpeedSkill || g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING)
			{
				// ��� ������/������ �϶� �ȵ�
			}
			else if( g_pLocalUser->m_CharacterLog.isTrade )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1903), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
			}						
			else if( g_pLocalUser->m_bSocketPlugIn )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1904), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
			}
			else if( g_pLocalUser->m_bGambleBoxOpen )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1905), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
			}
			else if( g_pLocalUser->m_bReadingBook )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1906), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
			}
			else if( g_pLocalUser->m_bInternalUse )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1907), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
			}
			else if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MATCHING && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_FIGHT)
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1908), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
			}
			else if(g_pLocalUser->m_bMatchPRMode)
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1908), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
			}
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			else if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_breath))
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1908), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
			}
#endif
			else if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
			{
			}
			else
			{
				_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
				if(pMainMenu_Window)
				{
					if(pMainMenu_Window->m_MeditationIndex > 0)
					{
						SelectSkill(pMainMenu_Window->m_MeditationIndex);

						if( g_TutorialInfo )
						{
							if( !g_TutorialInfo->_XTutorial_052 )
							{
								_XWindow_GuideLine* pGuideLine_Window = (_XWindow_GuideLine*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GUIDELINEWINDOW);
								if( pGuideLine_Window )
								{
									if( pGuideLine_Window->GetShowStatus() )
									{
										pGuideLine_Window->ShowWindow(FALSE);
										g_TutorialInfo->_XTutorial_052 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
										g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
									}
								}
							}
						}
					}
#ifdef _XTS_AUTOMEDITATION
					else
					{
						// ���� �ɹ� ����
						int skillid = 0;
						for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
						{
							if(g_pLocalUser->m_SkillList[i].m_sSkillID <= 0)
								continue;

							if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_PASSIVESKILL ||
								g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL)
							{
								if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->insideType > 0)
								{
									if(g_SkillProperty[skillid]->requirementLevel <= g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->requirementLevel)
									{
										if(g_pLocalUser->m_SkillList[i].m_ucSkillState == 0)	// ���� �Ϸ� �Ѱ� 
										{
											// req level�� ���� ���� �� ã��
											skillid = g_pLocalUser->m_SkillList[i].m_sSkillID;
										}
									}
								}
							}
						}

						if(skillid > 0)
						{
							g_NetworkKernel.SendPacket(MSG_NO_SKILL_CTRL_REQ, en_skill_ctrl_req_passive, skillid);
							g_pLocalUser->m_bAutoMeditation = TRUE;
						}
						else
						{
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_2628), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
						}
					}
#endif
				}
			}
		}
		break;
#ifdef _XTS_ITEMMALLBROWSER
	case _XDEF_MAINMENU_BTN_CASHMALL:
		{
 			if( !g_bShowItemMallWindow )
 			{
 				_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
 				if( pCashMall_Window )
				{
					
 					pCashMall_Window->ShowWindow( TRUE );					
					g_MainWindowManager.SetTopWindow( pCashMall_Window );
 					/*POINT pt = pCashMall_Window->GetWindowPos();
 					RECT rect;
 					GetWindowRect( gHWnd, &rect );*/
					
#if defined(_XDEF_ITEMMALL_AUTOLOGIN_20061120) || defined(_XDEF_ITEMMALL_AUTOLOGIN_SE_20070514)

					int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
					
	#ifdef _XDEF_ITEMMALL_AUTOLOGIN_SE_20070514
					TCHAR postdata[128];			
					_snprintf( postdata, sizeof(postdata), "AL_ID=%s&AL_PWD=%s&AL_Server=%d", g_pLocalUser->m_ItemMallWebLoginID, g_pLocalUser->m_UserPW, servernumber );
					_XDisplayHTMLPageAndPostData(g_hItemMallWebBrowserObject, _T("http://global1.indy21.co.kr:8080"), postdata );
	#else
					TCHAR tempStr[512];
					memset( tempStr, 0, sizeof(tempStr) );
					
					if( !g_bTestWebPageAddress )
						_snprintf( tempStr, sizeof(tempStr), "http://ninemall.nexon.com/?user_id=%s&select_server=%d", g_pLocalUser->m_ItemMallWebLoginID, servernumber );
					else
						_snprintf( tempStr, sizeof(tempStr), "http://panda.nexon.com/?user_id=%s&select_server=%d", g_pLocalUser->m_ItemMallWebLoginID, servernumber );
					
					DisplayHTMLPage(g_hItemMallWebBrowserObject, tempStr);
	#endif
					
#endif
					POINT pt = { 0, 0 };
					ClientToScreen( gHWnd, &pt );
					if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
					{						
						TCHAR tempStr[512];
						memset( tempStr, 0, sizeof(tempStr) );
						int lastconnectedservernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
						//sprintf( tempStr, "http://ninetest.gameleon.jp/item_mall/item_login.asp?user_id=%s&select_server=%d", g_pLocalUser->m_UserName, lastconnectedservernumber );
						_snprintf( tempStr, sizeof(tempStr), "http://nine.gameleon.jp/item_mall/item_login.asp?user_id=%s&select_server=%d", g_pLocalUser->m_UserName, lastconnectedservernumber );

						DisplayHTMLPage(g_hItemMallWebBrowserObject, tempStr);
						SetWindowPos(g_hItemMallWindow, /*HWND_TOPMOST*/NULL, pt.x + (gnWidth>>1)-400, pt.y + (gnHeight>>1)-300, 800, 600, 0);
					}
					else
					{
						#ifdef _ACCLAIM_RUBICONADSYSTEM	
						if( g_ViewRubiconAD )
						{
							::ShowWindow(g_hRubiconADWindow, SW_HIDE);
						}
						#endif
						
						SetWindowPos(g_hItemMallWindow, HWND_TOPMOST, pt.x + (gnWidth>>1)-400, pt.y + (gnHeight>>1)-300, 800, 600, 0);
					}
 					ShowWindow(g_hItemMallWindow, SW_SHOW);
					g_bShowItemMallWindow = TRUE;
					g_MainWindowManager.SetTopWindow( pCashMall_Window );
					
										
					_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
					if( pCashBox_Window )
					{
						pCashBox_Window->ShowWindow(FALSE);
					}
 				}		
 			}
 			else
 			{
 				_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
 				if( pCashMall_Window )
 				{
 					pCashMall_Window->ShowWindow( FALSE );
					
 					ShowWindow(g_hItemMallWindow, SW_HIDE);
 					g_bShowItemMallWindow = FALSE;
 				}		

				#ifdef _ACCLAIM_RUBICONADSYSTEM	
				if( g_ViewRubiconAD )
				{
					::ShowWindow(g_hRubiconADWindow, SW_SHOW);
				}
				#endif
 			}
			
		}
		break;
	case _XDEF_MAINMENU_BTN_CASHBOX:
		{
			_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
			if( pCashBox_Window )
			{
				pCashBox_Window->ShowWindow( !pCashBox_Window->GetShowStatus() );
				
				if( pCashBox_Window->GetShowStatus() )
				{
					_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
					if( pCashMall_Window )
					{
						pCashMall_Window->ShowWindow( FALSE );
						
						ShowWindow(g_hItemMallWindow, SW_HIDE);
						g_bShowItemMallWindow = FALSE;
					}		
				}
			}			
		}
		break;
#endif
		
	default :
			break;
	}
}

void XProc_MainGame::WarehouseWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_WAREHOUSEWINDOW_CLOSE :
		{
			if(m_pWarehouseWindowPtr && g_pInventory_Window)
			{
				if(m_pWarehouseWindowPtr->GetShowStatus())
					m_pWarehouseWindowPtr->ShowWindow(FALSE);
				
				if(g_pInventory_Window->GetShowStatus())
					g_pInventory_Window->ShowWindow(FALSE);
			}
		}
		break;
		
	case _XDEF_WAREHOUSEWINDOW_SENDADDITEM :
		{
			switch(m_pWarehouseWindowPtr->m_SendMode)
			{
			case _XSENDMODE_TOWAREHOUSE_ITEM :
				{
					// �� ���� üũ
					int emptycount = 0;
					
					for(int i = 0 ; i < m_pWarehouseWindowPtr->m_UseSlotCount ; i++) // 2004.05.24->oneway48 modify
					{
						if(g_pLocalUser->m_WarehouseItemList[i].Get_m_sID() == 0)
							emptycount++;
					}
					if(m_pWarehouseWindowPtr->m_TempItemList.size() <= emptycount)
					{
						if(m_pWarehouseWindowPtr->m_bProcessOneClickItem)
						{
							list <_XWarehouseItem*>::iterator iter_item = m_pWarehouseWindowPtr->m_TempItemList.begin();
							_XWarehouseItem* pWarehouseItem = *iter_item;
							if(pWarehouseItem)
							{
								if(pWarehouseItem->IsStackable())
								{
									_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
									if(pTradeConfirm_Window)
									{
										pTradeConfirm_Window->ShowWindow(TRUE);
										pTradeConfirm_Window->m_IMEControl.SetFocus();
										pTradeConfirm_Window->m_TradeMode = _XWAREHOUSE_TOWAREHOUSE_ITEM;
									}
								}
								else
								{
									g_NetworkKernel.SendPacket(MSG_NO_STR_ITEM_SAVE, 0);
									if( g_pDefaultTooltip_Window )
									{
										if( g_pDefaultTooltip_Window->GetShowStatus() )						
											g_pDefaultTooltip_Window->ShowWindow( FALSE );									
									}
								}
							}
							
						}
						else
						{
							g_NetworkKernel.SendPacket(MSG_NO_STR_ITEM_SAVE, 0);
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )						
									g_pDefaultTooltip_Window->ShowWindow( FALSE );
							}
						}
					}
					else
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_NOTENOUGHWAREHOUSE), TRUE);						
//						g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_NOTENOUGHWAREHOUSE), TRUE);
						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
						g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
					}
				}
				break;
			case _XSENDMODE_TOINVEN_ITEM :
				{
					int emptycount = 0;
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; i++)
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
							emptycount++;
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == 0)
							emptycount++;
					}
				#endif
					if(m_pWarehouseWindowPtr->m_TempItemList.size() <= emptycount)
					{
						if(m_pWarehouseWindowPtr->m_bProcessOneClickItem)
						{
							list <_XWarehouseItem*>::iterator iter_item = m_pWarehouseWindowPtr->m_TempItemList.begin();
							_XWarehouseItem* pWarehouseItem = *iter_item;
							if(pWarehouseItem)
							{
								if(pWarehouseItem->IsStackable())
								{
									_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
									if(pTradeConfirm_Window)
									{
										pTradeConfirm_Window->ShowWindow(TRUE);
										pTradeConfirm_Window->m_IMEControl.SetFocus();
										pTradeConfirm_Window->m_TradeMode = _XWAREHOUSE_FROMWAREHOUSE_ITEM;
									}
								}
								else
								{
									// 2004.06.02->oneway48 insert
									if( !m_pWarehouseWindowPtr->m_bTerminateWarehouse )
										g_NetworkKernel.SendPacket(MSG_NO_STR_ITEM_EJECT, 0);
									else
										g_NetworkKernel.SendPacket(MSG_NO_STR_ITEM_EJECTEX, 0);
									
									if( g_pDefaultTooltip_Window )
									{
										if( g_pDefaultTooltip_Window->GetShowStatus() )						
											g_pDefaultTooltip_Window->ShowWindow( FALSE );
									}
								}
							}
						}
						else
						{
							// 2004.06.02->oneway48 insert
							if( !m_pWarehouseWindowPtr->m_bTerminateWarehouse )
								g_NetworkKernel.SendPacket(MSG_NO_STR_ITEM_EJECT, 0);
							else
								g_NetworkKernel.SendPacket(MSG_NO_STR_ITEM_EJECTEX, 0);
							
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )						
									g_pDefaultTooltip_Window->ShowWindow( FALSE );
							}
						}
					}
					else
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_NOTENOUGHINVEN), TRUE);
//						g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_NOTENOUGHINVEN), TRUE);
					}

					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )						
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
					}
				}
				break;
			}
		}
		break;
		
	case _XDEF_WAREHOUSEWINDOW_STORAGEMONEY :
		{
			// 2004.06.01->oneway48 insert
			if( m_pWarehouseWindowPtr->m_bUseableWarehouse )
			{
				_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
				if(pTradeConfirm_Window)
				{
					pTradeConfirm_Window->ShowWindow(TRUE);
					pTradeConfirm_Window->m_IMEControl.SetFocus();
					pTradeConfirm_Window->m_TradeMode = _XWAREHOUSE_FROMWAREHOUSE_MONEY;
				}
			}
			else
			{	
				// 2004.06.02->oneway48 insert
				if( m_pWarehouseWindowPtr->m_bTerminateWarehouse )
				{
					_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
					if(pTradeConfirm_Window)
					{
						pTradeConfirm_Window->ShowWindow(TRUE);
						pTradeConfirm_Window->m_IMEControl.SetFocus();
						pTradeConfirm_Window->m_TradeMode = _XWAREHOUSE_FROMWAREHOUSE_MONEY;
					}
				}
				else
				{
					// â�� �߰��� �ؾ��Ѵ�.
					m_pNPCScriptWindowPtr->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_WAREHOUSEADD), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);		// "â�� �߰����� â�� ��� ����� �Ͻʽÿ�."
				}						
				
			}
			// insert end
		}
		break;

	case _XDEF_WAREHOUSEWINDOW_INVENTORYMONEY :
		{
			// 2004.06.01->oneway48 insert
			if( m_pWarehouseWindowPtr->m_bUseableWarehouse )
			{
				_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
				if(pTradeConfirm_Window)
				{
					pTradeConfirm_Window->ShowWindow(TRUE);
					pTradeConfirm_Window->m_IMEControl.SetFocus();
					pTradeConfirm_Window->m_TradeMode = _XWAREHOUSE_TOWAREHOUSE_MONEY;
				}
			}
			else
			{	
				// â�� �߰��� �ؾ��Ѵ�.				
				m_pNPCScriptWindowPtr->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_WAREHOUSEADD), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "â�� �߰����� â�� ��� ����� �Ͻʽÿ�."
				
			}
			// insert end
		}
		break;
		
	case _XDEF_WAREHOUSEWINDOW_ALLSELECT :
		{
			if( !m_pWarehouseWindowPtr ) break;
			// 2004.06.01->oneway48 modify
			if( !m_pWarehouseWindowPtr->m_bWarehouseCancleMode ) // â���� ���
			{
				m_pWarehouseWindowPtr->ReleaseTempItemList();			
				g_pInventory_Window->m_bDrawSelectedItem = TRUE; 
				
				// �κ��丮 ������ ���� ����
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ�� 
				//������ �ٸ� ���Ե� �˻��ϴµ� ���� ���Ը� �˻��ϰ� ����
				int tempitemcount = g_pInventory_Window->m_SelectedBaseTab * 25 + _XINVENTORY_SLOTTYPE_POCKETSTART;
				int totalcount = 0;
				for(int i = tempitemcount; i < tempitemcount+25; i++) //���� ���õ� �೶ ���� ���� ��
				{
					if( totalcount > 20 )
						break;

					if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
					{
						if(g_pLocalUser->m_UserItemList[i].IsStackable())
							m_pWarehouseWindowPtr->SetTempItemFromInven(i, g_pLocalUser->m_UserItemList[i].Get_m_ucCount());
						else
							m_pWarehouseWindowPtr->SetTempItemFromInven(i);

						totalcount++;
					}
				}
		#else
				int tempitemcount = g_pInventory_Window->m_SelectedBaseTab * 25 + _XINVENTORY_SLOTTYPE_POCKETSTART;
				int totalcount = 0;
				for( int i = tempitemcount ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; i++)
				{
					if( totalcount > 20 )
						break;

					if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
					{
						if(g_pLocalUser->m_UserItemList[i].IsStackable())
							m_pWarehouseWindowPtr->SetTempItemFromInven(i, g_pLocalUser->m_UserItemList[i].Get_m_ucCount());
						else
							m_pWarehouseWindowPtr->SetTempItemFromInven(i);

						totalcount++;
					}					
				}
				for( i = _XINVENTORY_SLOTTYPE_POCKETSTART; i < tempitemcount; i++ )
				{
					if( totalcount > 20 )
						break;

					if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
					{
						if(g_pLocalUser->m_UserItemList[i].IsStackable())
							m_pWarehouseWindowPtr->SetTempItemFromInven(i, g_pLocalUser->m_UserItemList[i].Get_m_ucCount());
						else
							m_pWarehouseWindowPtr->SetTempItemFromInven(i);

						totalcount++;
					}
				}
		#endif
			}
			/*else//â���������
			{
				_XWindow_NPCScript* m_pNPCScriptWindowPtr = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSCRIPT );
				
				int count = 0;
				for(int i = 0 ; i < _XDEF_WAREHOUSE_TOTALCOUNT ; i++)
				{
					if(g_pLocalUser->m_WarehouseItemList[i].m_sID > 0)
						count++;
				}
				
				
				if( count == 0 )
				{					
					if(g_pLocalUser->m_WarehouseMoney == 0)
					{
						g_NetworkKernel.SendPacket(MSG_NO_STR_BUY_SLOT_RELEASE);
						m_pNPCScriptWindowPtr->InsertNPCChatString(_T("â���� ��� ����� �����Ǿ����ϴ�."),  _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						m_pWarehouseWindowPtr->m_UseSlotCount = 0;
						m_pWarehouseWindowPtr->m_bUseableWarehouse = FALSE;
						m_pNPCScriptWindowPtr->GenerateSubScriptList();
						g_pLocalUser->ResetWarehouseInfo();
						
						if(m_pWarehouseWindowPtr->GetShowStatus())
							m_pWarehouseWindowPtr->ShowWindow(FALSE);
						
						if(g_pInventory_Window->GetShowStatus())
							g_pInventory_Window->ShowWindow(FALSE);
					}
					else
					{
						m_pNPCScriptWindowPtr->InsertNPCChatString(_T("â���� �ݾ��� �κ��丮�� �Ű��ֽʽÿ�."),  _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
				}
				else
				{
					m_pNPCScriptWindowPtr->InsertNPCChatString(_T("â�� ����ֽʽÿ�."),  _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}			
			}*/
			// modify end
		}
		break;
	case _XDEF_WAREHOUSEWINDOW_ONE :
		{
			m_pWarehouseWindowPtr->ChangeTab(0);
		}
		break;
	case _XDEF_WAREHOUSEWINDOW_TWO : 
		{
			m_pWarehouseWindowPtr->ChangeTab(1);
		}
		break;
    case _XDEF_WAREHOUSEWINDOW_THREE : //Author : ����� //breif : â��Ȯ��
        {
            m_pWarehouseWindowPtr->ChangeTab(2);
        }
        break;
	case _XDEF_WAREHOUSEWINDOW_REMOVAL :	// â�� ����
		{
				
			int count = 0;
			for(int i = 0 ; i < _XDEF_WAREHOUSE_TOTALCOUNT ; i++)
			{
				if(g_pLocalUser->m_WarehouseItemList[i].Get_m_sID() > 0)
					count++;
			}
			
			
			if( count == 0 )
			{					
				if(g_pLocalUser->m_WarehouseMoney == 0)
				{
					g_NetworkKernel.SendPacket(MSG_NO_STR_BUY_SLOT_RELEASE);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_DESTROYWAREHOUSE),  _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// _T("â���� ��� ����� �����Ǿ����ϴ�.")
					m_pWarehouseWindowPtr->m_UseSlotCount = 0;
					m_pWarehouseWindowPtr->m_bUseableWarehouse = FALSE;
					m_pNPCScriptWindowPtr->GenerateSubScriptList();
					g_pLocalUser->ResetWarehouseInfo();

					if( m_pAddSaveItemWindowPtr )
						m_pAddSaveItemWindowPtr->Reset();
					
					if(m_pWarehouseWindowPtr->GetShowStatus())
						m_pWarehouseWindowPtr->ShowWindow(FALSE);
					
					if( g_pInventory_Window->GetShowStatus())
						g_pInventory_Window->ShowWindow(FALSE);
				}
				else
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_TRADEMONEYTOINVEN), TRUE);	// _T("â���� �ݾ��� �೶���� �Ű��ֽʽÿ�.")
					pMessageBox->StartTimer(5000, TRUE) ;
				}
			}
			else
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_EMPTYWAREHOUSE), TRUE);	// _T("â�� ����ֽʽÿ�.")
				pMessageBox->StartTimer(5000, TRUE) ;
			}
		}
		break;
	case _XDEF_WAREHOUSEWINDOW_HELP :
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_WAREHOUSE );
		}
		break;
	}
}


void XProc_MainGame::AddSaveItemWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	
	switch(wparam)
	{	
	case _XDEF_ADDSAVEITEMWINDOW_CLOSE :
		{
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_CLOSE, 0), 0);
		}
		break;
		
	case _XDEF_ADDSAVEITEMWINDOW_OK :
		{
			if(m_pAddSaveItemWindowPtr)
			{
				if( m_pAddSaveItemWindowPtr->m_SelectedItemIndex == -1 )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_SELECTWAREHOUSE),  _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);		// _T("�Ⱓ�� ĭ���� ������ �ֽʽÿ�.")
				}
				else
				{
					if(m_pAddSaveItemWindowPtr->GetShowStatus() && g_pInventory_Window->GetShowStatus() )	
					{		
//						m_pAddSaveItemWindowPtr->ShowWindow(FALSE);
//						g_pInventory_Window->ShowWindow(FALSE);
						
						// 2004.05.24->oneway48 insert
						if( m_pAddSaveItemWindowPtr->m_AddStorageCost < g_pLocalUser->m_Money )
						{
							g_NetworkKernel.SendPacket( MSG_NO_STR_BUY_SLOT );
						}
						else
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
							//						g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NPCTRADE_NOTENOUGHMONEY, TRUE);
						}
						// insert end
					}

				}			
			}
		}
		break;
	case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN1		: 
		{
			if(m_pAddSaveItemWindowPtr)
			{
				if(m_pAddSaveItemWindowPtr->GetShowStatus())	
				{
					if( !m_pWarehouseWindowPtr->m_bTerminateWarehouse ) //â�� Ȯ���� ���� m_AddStorageSize, m_AddStorageTerm �� ������
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 20;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 120;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 2800;	
					#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
						if( m_pAddSaveItemWindowPtr->GetSelectTab() ) // �ι�° ���̸�
							m_pAddSaveItemWindowPtr->m_AddStorageCost = 6500;
					#endif
					}		
					else
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 20;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 120;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 3800;	

					#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
						if( m_pAddSaveItemWindowPtr->GetSelectTab() ) // �ι�° ���̸�
							m_pAddSaveItemWindowPtr->m_AddStorageCost = 7500;
					#endif
					}
					m_pAddSaveItemWindowPtr->SetSelectedIndex(0);
				}
			}
		}
		break;
	case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN2		:
		{
			if(m_pAddSaveItemWindowPtr)
			{
				if(m_pAddSaveItemWindowPtr->GetShowStatus())	
				{	
					if( !m_pWarehouseWindowPtr->m_bTerminateWarehouse )
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 20;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 240;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 5200;

					#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
						if( m_pAddSaveItemWindowPtr->GetSelectTab() ) // �ι�° ���̸�
							m_pAddSaveItemWindowPtr->m_AddStorageCost = 11900;
					#endif
					}
					else
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 20;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 240;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 6200;

					#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
						if( m_pAddSaveItemWindowPtr->GetSelectTab() ) // �ι�° ���̸�
							m_pAddSaveItemWindowPtr->m_AddStorageCost = 12900;
					#endif
					}
					m_pAddSaveItemWindowPtr->SetSelectedIndex(1);
				}
			}
		}
		break;
	case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN3		:
		{
			if(m_pAddSaveItemWindowPtr)
			{
				if(m_pAddSaveItemWindowPtr->GetShowStatus())	
				{
					if( !m_pWarehouseWindowPtr->m_bTerminateWarehouse )
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 20;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 365;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 7200;
					
					#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
						if( m_pAddSaveItemWindowPtr->GetSelectTab() ) // �ι�° ���̸�
							m_pAddSaveItemWindowPtr->m_AddStorageCost = 16300;
					#endif
					}
					else
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 20;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 365;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 8200;

					#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
						if( m_pAddSaveItemWindowPtr->GetSelectTab() ) // �ι�° ���̸�
							m_pAddSaveItemWindowPtr->m_AddStorageCost = 17300;
					#endif
					}
					m_pAddSaveItemWindowPtr->SetSelectedIndex(2);
				}
			}
		}
		break;
	case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN4		:
		{
			if(m_pAddSaveItemWindowPtr)
			{
				if(m_pAddSaveItemWindowPtr->GetShowStatus())	
				{
					if( !m_pWarehouseWindowPtr->m_bTerminateWarehouse )
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 30;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 120;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 4300;

					#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
						if( m_pAddSaveItemWindowPtr->GetSelectTab() ) // �ι�° ���̸�
							m_pAddSaveItemWindowPtr->m_AddStorageCost = 7800;
					#endif
					}
					else
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 30;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 120;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 5800;

					#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
						if( m_pAddSaveItemWindowPtr->GetSelectTab() ) // �ι�° ���̸�
							m_pAddSaveItemWindowPtr->m_AddStorageCost = 9300;
					#endif
					}
					m_pAddSaveItemWindowPtr->SetSelectedIndex(3);
				}
			}
		}
		break;
	case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN5		:
		{
			if(m_pAddSaveItemWindowPtr)
			{
				if(m_pAddSaveItemWindowPtr->GetShowStatus())	
				{
					if( !m_pWarehouseWindowPtr->m_bTerminateWarehouse )
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 30;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 240;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 7900;
					
					#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
						if( m_pAddSaveItemWindowPtr->GetSelectTab() ) // �ι�° ���̸�
							m_pAddSaveItemWindowPtr->m_AddStorageCost = 14300;
					#endif
					}
					else
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 30;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 240;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 9400;

					#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
						if( m_pAddSaveItemWindowPtr->GetSelectTab() ) // �ι�° ���̸�
							m_pAddSaveItemWindowPtr->m_AddStorageCost = 15800;
					#endif
					}
					m_pAddSaveItemWindowPtr->SetSelectedIndex(4);
				}
			}
		}
		break;
	case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN6		:
		{
			if(m_pAddSaveItemWindowPtr)
			{
				if(m_pAddSaveItemWindowPtr->GetShowStatus())	
				{
					if( !m_pWarehouseWindowPtr->m_bTerminateWarehouse )
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 30;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 365;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 10800;
					
					#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
						if( m_pAddSaveItemWindowPtr->GetSelectTab() ) // �ι�° ���̸�
							m_pAddSaveItemWindowPtr->m_AddStorageCost = 19600;
					#endif
					}
					else
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 30;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 365;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 12300;

					#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
						if( m_pAddSaveItemWindowPtr->GetSelectTab() ) // �ι�° ���̸�
							m_pAddSaveItemWindowPtr->m_AddStorageCost = 21100;
					#endif
					}
					m_pAddSaveItemWindowPtr->SetSelectedIndex(5);
				}
			}
		}
		break;
	case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN7		:
		{
			if(m_pAddSaveItemWindowPtr)
			{
				if(m_pAddSaveItemWindowPtr->GetShowStatus())	
				{
					if( !m_pWarehouseWindowPtr->m_bTerminateWarehouse )
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 40;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 120;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 5700;
					}
					else
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 40;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 120;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 7700;
					}
					m_pAddSaveItemWindowPtr->SetSelectedIndex(6);
				}
			}
		}
		break;
	case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN8		:
		{
			if(m_pAddSaveItemWindowPtr)
			{
				if(m_pAddSaveItemWindowPtr->GetShowStatus())	
				{
					if( !m_pWarehouseWindowPtr->m_bTerminateWarehouse )
					{						
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 40;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 240;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 10500;
					}
					else
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 40;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 240;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 12500;
					}
					m_pAddSaveItemWindowPtr->SetSelectedIndex(7);
					
				}
			}
		}
		break;
	case _XDEF_ADDSAVEITEMWINDOW_CHECKBTN9		: 
		{
			if(m_pAddSaveItemWindowPtr)
			{
				if(m_pAddSaveItemWindowPtr->GetShowStatus())	
				{
					if( !m_pWarehouseWindowPtr->m_bTerminateWarehouse )
					{						
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 40;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 365;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 14400;
					}
					else
					{
						m_pAddSaveItemWindowPtr->m_AddStorageSize = 40;
						m_pAddSaveItemWindowPtr->m_AddStorageTerm = 365;
						m_pAddSaveItemWindowPtr->m_AddStorageCost = 16400;
					}
					m_pAddSaveItemWindowPtr->SetSelectedIndex(8);
				}
			}
		}
		break;
	case _XDEF_ADDSAVEITEMWINDOW_HELP		:
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_WAREHOUSE );
		}
		break;
	case _XDEF_ADDSAVEITEMWINDOW_POCKET1 : //Author : ����� //breif : â�� Ȯ��, �̹����� ��ư����
		{
			//ù��° ��
			m_pAddSaveItemWindowPtr->SetSelectTab(0);
		}
		break;
	case _XDEF_ADDSAVEITEMWINDOW_POCKET2 :
		{
			//�ι�° ��
			m_pAddSaveItemWindowPtr->SetSelectTab(1);
		}
		break;
	}
}

void XProc_MainGame::PCTradeWindowMessageHanlding(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_PCTRADEWINDOW_CLOSE :
		{
			if( m_pPCTradeWindowPtr)
			{
				g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_CANCEL);
				m_pPCTradeWindowPtr->ShowWindow( FALSE );
			}
		}
		break;
		
	case _XDEF_PCTRADEWINDOW_YES :
		{
			if(m_pPCTradeWindowPtr)
			{
				g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_REQ, SUB_MSG_TRADE_ALLOW);
			}
		}
		break;
		
	case _XDEF_PCTRADEWINDOW_NO :
		{
			if(m_pPCTradeWindowPtr)
			{
				m_pPCTradeWindowPtr->m_bTrading = FALSE;
				g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_REQ, SUB_MSG_TRADE_REJECT);
			}
		}
		break;
		
	case _XDEF_PCTRADEWINDOW_OK :
		{
			if(m_pPCTradeWindowPtr)
			{
				g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_REQ, SUB_MSG_TRADE_CONFIRM);
				m_pPCTradeWindowPtr->m_OKButton->EnableWindow(FALSE);
				m_pPCTradeWindowPtr->m_bWaitTrade = TRUE;
				m_pPCTradeWindowPtr->m_WaitIcon->ShowWindow(FALSE);
				m_pPCTradeWindowPtr->m_ConfirmIcon->ShowWindow(TRUE);
			}
		}
		break;
		
	case _XDEF_PCTRADEWINDOW_CANCEL :
		{
			if(m_pPCTradeWindowPtr)
			{
				g_NetworkKernel.SendPacket(MSG_NO_PVP_TRADE_CANCEL);
			}
		}
		break;
	case _XDEF_PCTRADEWINDOW_HELP :
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_PTOP );
		}
		break;
	case _XDEF_PCTRADEWINDOW_MONEY :
		{
			_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
			if( m_pPCTradeWindowPtr && m_pPCTradeWindowPtr->GetShowStatus() )	// �Ǿ��� �ŷ�â ������
			{
				if(pTradeConfirm_Window)
				{
					pTradeConfirm_Window->ShowWindow(TRUE);
					pTradeConfirm_Window->m_IMEControl.SetFocus();
					pTradeConfirm_Window->m_TradeMode = _XPCTRADE_FROMTRADE_MONEY;
				}
			}
		}
		break;
	}
}

void XProc_MainGame::MessengerWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
	
	if( !pMessenger_Window ) return;

	switch(wparam)
	{		
	case _XDEF_MESSENGERWINDOW_CLOSEBUTTON:	
		{
			pMessenger_Window->ShowWindow(FALSE);
		}
		break;
	case _XDEF_MESSENGERWINDOW_HELPBUTTON:		
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_MESSENGER );
		}
		break;
	case _XDEF_MESSENGERWINDOW_MESSENGERLOGIN:
		{
			// �α��� ���·� ��ȯ 
			pMessenger_Window->m_CurrentUserStatus = 1;	
			pMessenger_Window->m_btnMessengerLogIn->ShowWindow(FALSE);
			pMessenger_Window->m_btnMessengerLogOut->ShowWindow(TRUE);
			g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 1 );
			_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
			if(pMainMenu_Window)
			{
				pMainMenu_Window->SetMessengerStatus( pMessenger_Window->m_CurrentUserStatus );
			}
		}
		break;
	case _XDEF_MESSENGERWINDOW_MESSENGERLOGOUT:
		{
			// �α׿��� ���·� ��ȯ
			pMessenger_Window->m_CurrentUserStatus = 0;	
			pMessenger_Window->m_btnMessengerLogIn->ShowWindow(TRUE);
			pMessenger_Window->m_btnMessengerLogOut->ShowWindow(FALSE);		
			g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 0 );
			_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
			if(pMainMenu_Window)
			{
				pMainMenu_Window->SetMessengerStatus( pMessenger_Window->m_CurrentUserStatus );
			}
		}
		break;
	case _XDEF_MESSENGERWINDOW_ADDGROUPBUTTON :
		{		
			_XWindow_MessengerTooltip* pMessengerTooltip_Window 
				= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
			
//			if(	!pMessengerTooltip_Window->m_bIsButtonProcess )
			{				
				if( pMessenger_Window->m_MessengerGroupList.getitemcount()  < 8 ) //�׷��� ����� �ִ� �ִ� ���� : 8�� 
				{
					_XWindow_NameInput* pNameInput_Window = (_XWindow_NameInput*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NAMEINPUTWINDOW);
					
					pNameInput_Window->m_NameInputMode = _XNAMEINPUT_GROUPNAME;
					pNameInput_Window->m_IMEControl.SetMaxLength(14);
					pNameInput_Window->ShowWindow( TRUE );
					pMessengerTooltip_Window->ShowWindow(FALSE);
//					pMessengerTooltip_Window->m_bIsButtonProcess = TRUE;
				}
				else
				{
					pMessengerTooltip_Window->ShowWindow(FALSE);
					
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(messagestring));
					
					_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_CANNOTCREATE) );
					g_NetworkKernel.InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); 
				}
			}			
		}
		break;
		
	case _XDEF_MESSENGERWINDOW_ADDFRIENDBUTTON :
		{
			
			_XWindow_MessengerTooltip* pMessengerTooltip_Window 
				= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
			
			
			// ģ�� ��û
//			if(	!pMessengerTooltip_Window->m_bIsButtonProcess )
			{				
				
				if( pMessenger_Window->m_TotalCountFriends <= 50) // �ִ� 50����� ����
				{
					_XWindow_NameInput* pNameInput_Window = (_XWindow_NameInput*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NAMEINPUTWINDOW);
					
					pNameInput_Window->m_NameInputMode = _XNAMEINPUT_CHARACTERNAME;
					pNameInput_Window->m_IMEControl.SetMaxLength(14);
					pNameInput_Window->ShowWindow( TRUE );
//					pMessengerTooltip_Window->m_bIsButtonProcess = TRUE;
				}
				else
				{					
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(messagestring));
					
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_EXCESSPERSON) );
					g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
			}			
		}
		break;
		
	case _XDEF_MESSENGERWINDOW_CHANGESTATUSBUTTON :
		{			
			_XWindow_MessengerTooltip* pMessengerTooltip_Window 
				= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
			
			if( pMessengerTooltip_Window->GetShowStatus() )
			{
				if( pMessengerTooltip_Window->m_MessengerTooltipMode == _XMESSENGERTOOLTIP_CHARACTERSTATE )
				{
					pMessengerTooltip_Window->m_MessengerTooltipMode = _XMESSENGERTOOLTIP_NONE;
					pMessengerTooltip_Window->m_listboxUserStatus->DeleteAllItem();
					pMessengerTooltip_Window->m_listboxUserStatus->SetWindowSize(0,0);
					pMessengerTooltip_Window->ShowWindow(FALSE);
				}
			}
			else
			{
				POINT winpos;
				SIZE winsize;
				winpos = pMessenger_Window->GetWindowPos();
				winsize = pMessenger_Window->GetWindowSize();
				
				pMessengerTooltip_Window->MoveWindow( winpos.x + 8, winpos.y + 42 );
				pMessengerTooltip_Window->SetMode(_XMESSENGERTOOLTIP_CHARACTERSTATE);
				pMessengerTooltip_Window->ShowWindow(TRUE);	 
				g_MainWindowManager.SetTopWindow(pMessengerTooltip_Window);	
			}
		}
		break;
	case _XDEF_MESSENGERWINDOW_RECALLBUTTON : //Author : ����� //breif : ģ����ȯ
		{
#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER
			_XWindow_RecallItem* pRecallItemWindow = (_XWindow_RecallItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_RECALLITEM_WINDOW);
			if( !pRecallItemWindow ) return;

			SIZE size = pRecallItemWindow->GetWindowSize();

			//����â�� �����ְ� �޽��� â����������..
			if( !pRecallItemWindow->GetShowStatus() && pMessenger_Window->GetShowStatus() )
			{
				//����â�� ���� ���� ��
				if( pMessenger_Window->GetWindowPos().x - size.cx > 0 )
					pRecallItemWindow->MoveWindow( pMessenger_Window->GetWindowPos().x - size.cx , pMessenger_Window->GetWindowPos().y );
				else
					pRecallItemWindow->MoveWindow( pMessenger_Window->GetWindowPos().x + pMessenger_Window->GetWindowSize().cx , pMessenger_Window->GetWindowPos().y );

				g_MainWindowManager.SetTopWindow( pRecallItemWindow );
			}

			pRecallItemWindow->ShowWindow( !pRecallItemWindow->GetShowStatus() );
#endif
		}
		break;
	}
}

void XProc_MainGame::EmailWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
	
	if( !pEmail_Window ) return;
	
	switch(wparam)
	{
	case _XDEF_EMAILWINDOW_EMAILRECEIVEBOX:		
		{					
			pEmail_Window->ChangeTab(_XEMAILTAB_RECEIVE);
		}
		break;
	case _XDEF_EMAILWINDOW_EMAILSENDBOX:
		{					
			pEmail_Window->ChangeTab(_XEMAILTAB_SEND);
		}
		break;
	case _XDEF_EMAILWINDOW_EMAILKEEPBOX:
		{					
			pEmail_Window->ChangeTab(_XEMAILTAB_KEEP);
		}
		break;
	case _XDEF_EMAILWINDOW_EMAILCLOSE:
		{				
			pEmail_Window->ShowWindow(FALSE);
		}
		break;
	case _XDEF_EMAILWINDOW_EMAILHELP:
		{					
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_PAGE );
		}
		break;
	case _XDEF_EMAILWINDOW_EMAILNEXT:
		{								
			if( pEmail_Window->m_indexEmailText+1 <= pEmail_Window->m_EmailTextTotalCount)
			{
				pEmail_Window->m_indexEmailText++;
				g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_TEXT_REQ_C_S, pEmail_Window->m_EmailList[pEmail_Window->m_indexEmailText-1].uiIndex );
			}
		}
		break;
	case _XDEF_EMAILWINDOW_EMAILPREV:
		{					
			if( pEmail_Window->m_indexEmailText-1 > 0 )
			{
				pEmail_Window->m_indexEmailText--;
				g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_TEXT_REQ_C_S, pEmail_Window->m_EmailList[pEmail_Window->m_indexEmailText-1].uiIndex );
			}
		}
		break;
	case _XDEF_EMAILWINDOW_EMAILKEEPINGMAILDELETE:
		{					
			if( pEmail_Window->m_indexEmailText > 0 && pEmail_Window->m_indexEmailText <= pEmail_Window->m_EmailTextTotalCount )
			{
				g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_DROP_C_S, pEmail_Window->m_EmailList[pEmail_Window->m_indexEmailText-1].uiIndex );
				pEmail_Window->DeleteEmailList( pEmail_Window->m_indexEmailText-1 );
			}
		}
		break;
	case _XDEF_EMAILWINDOW_EMAILRECEIVEREPLY:
		{					
			if( strlen(pEmail_Window->m_EmailSender) != 0 )
			{
				strcpy( pEmail_Window->m_EmailReceiver, pEmail_Window->m_EmailSender);
				pEmail_Window->ChangeTab(_XEMAILTAB_SEND);
				pEmail_Window->m_EmailTextScrollBar->SetLineScrollPos(0);
				pEmail_Window->m_EmailTextScrollBar->UpdateData();		
				pEmail_Window->m_EmailIMEControl.SetFocus();
			}
		}
		break;
	case _XDEF_EMAILWINDOW_EMAILRECEIVESAVE:
		{			
			if( pEmail_Window->m_indexEmailReceiveText > 0 && pEmail_Window->m_indexEmailReceiveText <= pEmail_Window->m_EmailReceiveTextTotalCount )
			{
				for( int i = 0; i < 20; i++ )
				{
					if( pEmail_Window->m_EmailList[i].uiIndex <= 0 )
					{
						pEmail_Window->m_EmailList[i].uiIndex = pEmail_Window->m_EmailListReceiveBox[pEmail_Window->m_indexEmailReceiveText-1].uiIndex;
						pEmail_Window->m_EmailList[i].check = FALSE;
						break;
					}
				}
				pEmail_Window->m_EmailTextTotalCount++;
				pEmail_Window->m_indexEmailText = pEmail_Window->m_EmailTextTotalCount;
				
				pEmail_Window->DeleteEmailListReceiveBox( pEmail_Window->m_indexEmailReceiveText-1 );
			}			
		}
		break;
	case _XDEF_EMAILWINDOW_EMAILRECEIVEDELETE:
		{							
			if( pEmail_Window->m_indexEmailReceiveText > 0 && pEmail_Window->m_indexEmailReceiveText <= pEmail_Window->m_EmailReceiveTextTotalCount )
			{
				g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_DROP_C_S, pEmail_Window->m_EmailListReceiveBox[pEmail_Window->m_indexEmailReceiveText-1].uiIndex );
				pEmail_Window->DeleteEmailListReceiveBox( pEmail_Window->m_indexEmailReceiveText-1 );
			}
		}
		break;
	case _XDEF_EMAILWINDOW_EMAILSENDDELETE:
		{					
			pEmail_Window->ResetEmailText();
		}
		break;
	case _XDEF_EMAILWINDOW_EMAILSENDCANCLE:
		{					
			pEmail_Window->ResetEmailText();
			pEmail_Window->ShowWindow(FALSE);
		}
		break;
	case _XDEF_EMAILWINDOW_EMAILSEND:
		{					
			memset( pEmail_Window->m_EmailText, 0, sizeof(pEmail_Window->m_EmailText) );
			
			for( int i = 0; i < pEmail_Window->m_EmailTextLineTotalCount ; i++)
			{
				strcat( pEmail_Window->m_EmailText, pEmail_Window->m_EmailDisplayText[i] );
				if( i != pEmail_Window->m_EmailTextLineTotalCount - 1)
					strcat( pEmail_Window->m_EmailText, "\n" );
			}
			pEmail_Window->CalculateEmailLength();
			pEmail_Window->m_EmailText[pEmail_Window->m_EmailTextLength] = 0;
			
			if( pEmail_Window->m_EmailTextLength != 0 )
			{
				if( pEmail_Window->m_EmailReceiver[0] == '@' )
				{
					TCHAR* tempReceiver = &pEmail_Window->m_EmailReceiver[1];
					TCHAR tempOrgaName[128];
					memset( tempOrgaName, 0, sizeof(tempOrgaName));
					_snprintf( tempOrgaName, sizeof(tempOrgaName), _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO2), g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName);
					
					if( strcmp(tempReceiver, tempOrgaName) == 0 )
					{
						// ������ ������
						g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S );
					}
					else
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1909), TRUE);	
						pMessageBox->StartTimer(5000, TRUE) ;
					}
				}				
				else 
					g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_SEND_REQ_C_S );
			}			
		}
		break;
	}
}



void XProc_MainGame::MessengerDialogWindowMessageHandling(WPARAM wparam, LPARAM lparam) 
{
	if( m_bNPCDialogMode || m_bNPCDialogWideView )	return;
	
	if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING && g_pLocalUser->m_SelectedTrainingMode == _XTRAINING_MEDITATION)
		return;
	
	DWORD windowid = (wparam & 0xFFFF0000) >> 16;
	DWORD commandid =(wparam & 0x0000FFFF);
	
	_XWindow_MessengerDialog* pMessengerDialog_Window = (_XWindow_MessengerDialog*)g_MainWindowManager.FindWindow(windowid);
	if( !pMessengerDialog_Window ) return;
	
	switch(commandid)
	{
	case _XMESSENGERDIALOGBUTTON_HELP:
		{					
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_MESSENGERWIN );
		}
		break;
	case _XMESSENGERDIALOGBUTTON_MINIMIZE:
		{					
			pMessengerDialog_Window->SetMode( _XMESSENGERDIALOGMODE_INSTANCE );
		}
		break;
	case _XMESSENGERDIALOGBUTTON_CLOSE:
		{					
			if( pMessengerDialog_Window->m_InstanceMessagePositionNumber != -1 )
			{
				g_NetworkKernel.m_bMessengerInstanceMessagePosition[ pMessengerDialog_Window->m_InstanceMessagePositionNumber ] = FALSE;
			}

			_XWindow_TargetInfoSelect* pInfoSelect_Window = (_XWindow_TargetInfoSelect*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOSELECTWINNDOW);
			_XWindow_TargetInfo* targetinfo_window = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);
			
			memset( targetinfo_window->m_SendName, 0, sizeof(targetinfo_window->m_SendName) );
			pInfoSelect_Window->ShowWindow(FALSE);

			g_MainWindowManager.DeleteWindow( windowid );
		}
		break;
	case _XMESSENGERDIALOGBUTTON_INFORMATION:
		{//Author : ����� //breif : ��������� ��������,,
			static int nMessengerWindowID = windowid; //������ ���̵�� ,, �˻縦 �ؾ� �Ѵ�

			_XWindow_TargetInfo* targetinfo_window = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);
			_XWindow_TargetInfoSelect* pInfoSelect_Window = (_XWindow_TargetInfoSelect*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOSELECTWINNDOW);

			if( nMessengerWindowID != windowid ) // �ٸ� �����쿡�� ��������, ������ Ų��
			{
				nMessengerWindowID = windowid;
				
				POINT pos = pMessengerDialog_Window->m_btnMessengerDialogInformation->GetWindowPos();
				SIZE size = pMessengerDialog_Window->m_btnMessengerDialogInformation->GetWindowSize();

				if( targetinfo_window && pInfoSelect_Window )
				{
					_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
					pMonsterStatus_Window->m_ShowTargetInfoPopUp = FALSE; //Ȥ�� �𸣴�,,

					memset( targetinfo_window->m_SendName, 0, sizeof(targetinfo_window->m_SendName) );
					strcpy( targetinfo_window->m_SendName, pMessengerDialog_Window->m_ToCharactername);
					
					pInfoSelect_Window->MoveWindow( pos.x + size.cx + 4, pos.y );
					pInfoSelect_Window->ShowWindow(TRUE);
				}
			}
			else //���� ������
			{
				_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );

				if( !pMonsterStatus_Window->m_ShowTargetInfoPopUp && pInfoSelect_Window->GetShowStatus() ) //���� â���� �� �������â�� �ƴϾ����
				{
					pInfoSelect_Window->ShowWindow(FALSE);
				}
				else
				{
					POINT pos = pMessengerDialog_Window->m_btnMessengerDialogInformation->GetWindowPos();
					SIZE size = pMessengerDialog_Window->m_btnMessengerDialogInformation->GetWindowSize();

					if( targetinfo_window && pInfoSelect_Window)
					{		
						pMonsterStatus_Window->m_ShowTargetInfoPopUp = FALSE; //Ȥ�� �𸣴�,,

						memset( targetinfo_window->m_SendName, 0, sizeof(targetinfo_window->m_SendName) );
						strcpy( targetinfo_window->m_SendName, pMessengerDialog_Window->m_ToCharactername);
						
						pInfoSelect_Window->MoveWindow( pos.x + size.cx + 4, pos.y );
						pInfoSelect_Window->ShowWindow(TRUE);
					}
				}
			}
		}
		break;
	case _XMESSENGERDIALOGBUTTON_PARTYREQUEST:
		{		
			if( !g_pLocalUser->m_bPartyProgress )
			{
				if( g_pLocalUser->m_InParty )
				{				
					if( pMessengerDialog_Window->m_ToCharactername[0] != 0 )
					{
						bool checkparty = FALSE;
						for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++)
						{
							if( strcmp(g_pLocalUser->m_PartyInfo[i].playername, pMessengerDialog_Window->m_ToCharactername) == 0 )
							{
								checkparty = TRUE;
								break;
							}
						}
						
						if( !checkparty )
						{
							_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
							if(pRequestParty_Window)
							{
								if( pMessengerDialog_Window->m_ToCharactername[0] != 0 )
								{
									memset(pRequestParty_Window->m_PickedCharacterName, 0, sizeof(pRequestParty_Window->m_PickedCharacterName) );
									strcpy(pRequestParty_Window->m_PickedCharacterName, pMessengerDialog_Window->m_ToCharactername);
									
									if( !g_pLocalUser->m_bPartyQuestProgress )
									{
										g_NetworkKernel.SendPacket(MSG_NO_PARTY_JOIN_REQ);
									}
									else
									{
										g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1888), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );	
									}
								}
							}
						}
						else
						{
							g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1910), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );	
						}
					}
					
				}
				else
				{				
					_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
					if(pRequestParty_Window)
					{
						if( pMessengerDialog_Window->m_ToCharactername[0] != 0 )
						{
							memset(pRequestParty_Window->m_PickedCharacterName, 0, sizeof(pRequestParty_Window->m_PickedCharacterName) );
							strcpy(pRequestParty_Window->m_PickedCharacterName, pMessengerDialog_Window->m_ToCharactername);
							
							g_NetworkKernel.SendPacket(MSG_NO_PARTY_JOIN_REQ);
						}
					}
				}
			}
		}
		break;
	case _XMESSENGERDIALOGBUTTON_FRIENDCUT:
		{					
		}
		break;
	case _XMESSENGERDIALOGBUTTON_CHATINPUT:
		{						
			BOOL flag = FALSE;
			
			TCHAR tbuf[_XDEF_CHATMAXLENGTH];
			memset( tbuf, 0, sizeof(TCHAR) * _XDEF_CHATMAXLENGTH );
			strcpy(tbuf, pMessengerDialog_Window->m_IMEControlMessengerDialog.GetText());
			
			if( tbuf[0] != 0 )
			{
				g_NetworkKernel.SendMessengerChatMessage( tbuf, windowid );
				_XIMEKERNEL.ResetCombinationBuffer();
				pMessengerDialog_Window->m_IMEControlMessengerDialog.ClearBuffer();
				pMessengerDialog_Window->m_IMEControlMessengerDialog.SetFocus();
			}
		}
		break;
	case _XMESSENGERDIALOGBUTTON_EMOTICON:
		{					
		}
		break;
	}
}

void XProc_MainGame::QuestMailWindowMessageHandling(WPARAM wparam, LPARAM lparam) 
{		
	DWORD windowid = (wparam & 0xFFFF0000) >> 16;
	DWORD commandid =(wparam & 0x0000FFFF);
	
	_XWindow_QuestMail* pQuestMail_Window = (_XWindow_QuestMail*)g_MainWindowManager.FindWindow(windowid);
	if( !pQuestMail_Window ) return;
	
	switch(commandid)
	{
	case _XQUESTMAILBUTTON_OK:
		{		
			_XQUESTMAIL_INFO questmail;
			memset(&questmail, 0, sizeof(_XQUESTMAIL_INFO));
			questmail.questid	= pQuestMail_Window->m_QuestID;
			questmail.questmailtype = 0;
			questmail.bYESorNO = TRUE;

			g_QuestScriptManager.m_listQuestMailInfo.push_back(questmail);
			if( pQuestMail_Window->m_bStartQuest )
			{
				g_QuestScriptManager.QuestStartConditionNPCTargettingMode();
			}
			g_MainWindowManager.DeleteWindow( windowid );
		}
		break;
	case _XQUESTMAILBUTTON_YES:
		{					
			_XQUESTMAIL_INFO questmail;
			memset(&questmail, 0, sizeof(_XQUESTMAIL_INFO));
			questmail.questid	= pQuestMail_Window->m_QuestID;
			questmail.questmailtype = 1;
			questmail.bYESorNO = TRUE;
			
			g_QuestScriptManager.m_listQuestMailInfo.push_back(questmail);
			if( pQuestMail_Window->m_bStartQuest )
			{
				g_QuestScriptManager.QuestStartConditionNPCTargettingMode();
			}
			g_MainWindowManager.DeleteWindow( windowid );
		}
		break;
	case _XQUESTMAILBUTTON_NO:
		{			
			_XQUESTMAIL_INFO questmail;
			memset(&questmail, 0, sizeof(_XQUESTMAIL_INFO));
			questmail.questid	= pQuestMail_Window->m_QuestID;
			questmail.questmailtype = 1;
			questmail.bYESorNO = FALSE;
			
			g_QuestScriptManager.m_listQuestMailInfo.push_back(questmail);	
			if( pQuestMail_Window->m_bStartQuest )
			{
				g_QuestScriptManager.QuestStartConditionNPCTargettingMode();
			}	
			g_MainWindowManager.DeleteWindow( windowid );
		}
		break;
	}
}

void XProc_MainGame::TutorialDialogWindowMessageHandling(WPARAM wparam, LPARAM lparam)	// 2005.03.03->hotblood
{
	DWORD windowid = (wparam & 0xFFFF0000) >> 16;

	_XWindow_Tutorial* pTutorialDialog_Window = (_XWindow_Tutorial*)g_MainWindowManager.FindWindow(windowid);
	_XWindow_TutorialIcon* pTutorialIcon_Window = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_TUTORIALICON );
	if( !pTutorialDialog_Window || !pTutorialIcon_Window ) return;
	g_MainWindowManager.DeleteWindow( windowid );
	pTutorialIcon_Window->DeleteTutorialIDlgList(windowid);
}

void XProc_MainGame::MessengerTooltipWindowMessageHandling(WPARAM wparam, LPARAM lparam) 
{
	_XWindow_MessengerTooltip* pMessengerTooltip_Window 
		= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
	
	
	switch(wparam)
	{	
	case _XDEF_MESSENGERTOOLTIPWINDOW_ADDGROUPBUTTON :
		{
//			if(	!pMessengerTooltip_Window->m_bIsButtonProcess )
			{
				_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
				
				if( pMessenger_Window->m_MessengerGroupList.getitemcount()  < 8 ) //�׷��� ����� �ִ� �ִ� ���� : 8�� 
				{
					_XWindow_NameInput* pNameInput_Window = (_XWindow_NameInput*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NAMEINPUTWINDOW);
					
					pNameInput_Window->m_NameInputMode = _XNAMEINPUT_GROUPNAME;
					pNameInput_Window->m_IMEControl.SetMaxLength(14);
					pNameInput_Window->ShowWindow( TRUE );
					pMessengerTooltip_Window->ShowWindow(FALSE);
//					pMessengerTooltip_Window->m_bIsButtonProcess = TRUE;
				}
				else
				{
					pMessengerTooltip_Window->ShowWindow(FALSE);

					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);
					
					sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_CANNOTCREATE) );
					g_NetworkKernel.InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // 2004.04.30->oneway48 modify	
				}
			}			
		}
		break;
		
	case _XDEF_MESSENGERTOOLTIPWINDOW_DELETEGROUPBUTTON :
		{
			if(pMessenger_Window->m_SelectedGroupname[0] != 0) 
			{
				pMessengerTooltip_Window->ShowWindow(FALSE);

				// 2004.04.24->oneway48 insert
				if( strcmp( pMessenger_Window->m_SelectedGroupname, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_FRIEND)) == 0 )	// "ģ��"
				{
					pMessengerTooltip_Window->ShowWindow(FALSE);
					
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);
					
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTDELETEGROUPNAME) );	// _T("������ �� ���� �׷� �̸��Դϴ�.")
					g_NetworkKernel.InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // 2004.04.30->oneway48 modify	
					
//					pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;	
					break;
				}
				// insert end
				
				// ������ �׷쿡 ģ������ ������ �������� �ʴ´�.
				bool havefriend = FALSE;
				if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
				{
					pMessenger_Window->m_MessengerGroupList.resetList();
					_XMessengerGroup* currentgroup = NULL;
					
					do 
					{
						currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
						if(currentgroup)
						{
							if( strcmp( currentgroup->m_GroupName, pMessenger_Window->m_SelectedGroupname ) == 0 )
							{
								if( !currentgroup->m_FriendList.listEmpty() )
									havefriend = TRUE;
								break;	
							}				
							
						}	
						pMessenger_Window->m_MessengerGroupList.nextItem();
					} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());			
				}			
				
				if( !havefriend )
				{
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_DELETE) );
					
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_MESSENGERTOOLTIPWINDOW_DELETEGROUPOK, _XDEF_MESSENGERTOOLTIPWINDOW_DELETEGROUPNO);					
//					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_MESSENGERTOOLTIPWINDOW_DELETEGROUPOK, _XDEF_MESSENGERTOOLTIPWINDOW_DELETEGROUPNO);					
				}
				else
				{
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);
					
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_CANNOTDELETE) );
					g_NetworkKernel.InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // 2004.04.30->oneway48 modify
				}
				
				
			}
			else
			{
				pMessengerTooltip_Window->ShowWindow(FALSE);
				
				TCHAR messagestring[256];
				memset(messagestring, 0, sizeof(TCHAR)*256);
				
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);

				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_SELECTNAME) );
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
				pMessageBox->StartTimer( 10000, TRUE );
//				g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
//				g_MessageBox.StartTimer( 10000, TRUE );
				
//				pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
			}
		}
		break;

	case _XDEF_MESSENGERTOOLTIPWINDOW_DELETEGROUPOK :
		{
//			if(	!pMessengerTooltip_Window->m_bIsButtonProcess )
			{
				g_NetworkKernel.SendMessengerPacket(MMSG_NO_CUT_GROUP_REQ_C_S);
				pMessengerTooltip_Window->ShowWindow(FALSE);
//				pMessengerTooltip_Window->m_bIsButtonProcess = TRUE;
			}
		}
		break;

	case _XDEF_MESSENGERTOOLTIPWINDOW_DELETEGROUPNO :
		{
//			pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
		}
		break;

	case _XDEF_MESSENGERTOOLTIPWINDOW_CHANGERGROUPNAMEBUTTON :
		{
			if(pMessenger_Window->m_SelectedGroupname[0] != 0) 
			{
				if( strcmp( pMessenger_Window->m_SelectedGroupname, _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_FRIEND)) != 0 )
				{
//					if(	!pMessengerTooltip_Window->m_bIsButtonProcess )
					{
						_XWindow_NameInput* pNameInput_Window = (_XWindow_NameInput*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NAMEINPUTWINDOW);
						
						pNameInput_Window->m_NameInputMode = _XNAMEINPUT_CHANGEGROUPNAME;
						pNameInput_Window->m_IMEControl.SetMaxLength(14);
						pNameInput_Window->ShowWindow( TRUE );
						pMessengerTooltip_Window->ShowWindow(FALSE);
//						pMessengerTooltip_Window->m_bIsButtonProcess = TRUE;
					}
				}
				else
				{
					pMessengerTooltip_Window->ShowWindow(FALSE);
					
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);
					
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_CANNOTRENAME) );
					g_NetworkKernel.InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // 2004.04.30->oneway48 modify
					
//					pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;

				}
			}
			else
			{
				pMessengerTooltip_Window->ShowWindow(FALSE);
				
				TCHAR messagestring[256];
				memset(messagestring, 0, sizeof(TCHAR)*256);
				
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_SELECTNAME) );
				g_NetworkKernel.InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // 2004.04.30->oneway48 modify
				
//				pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
			}
			
		}
		break;
		
	case _XDEF_MESSENGERTOOLTIPWINDOW_DELETEFRIENDBUTTON :
		{
			if(pMessenger_Window->m_SelectedCharactername[0] != 0 )
			{
				pMessengerTooltip_Window->ShowWindow(FALSE);
				
				TCHAR messagestring[256];
				memset(messagestring, 0, sizeof(TCHAR)*256);
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_DELETE) );
				
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_MESSENGERTOOLTIPWINDOW_DELETEFRIENDOK, _XDEF_MESSENGERTOOLTIPWINDOW_DELETEFRIENDNO);
				pMessageBox->StartTimer( 10000, TRUE );
//				g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_MESSENGERTOOLTIPWINDOW_DELETEFRIENDOK, _XDEF_MESSENGERTOOLTIPWINDOW_DELETEFRIENDNO);
//				g_MessageBox.StartTimer( 10000, TRUE );
			}
			else
			{
				pMessengerTooltip_Window->ShowWindow(FALSE);
				
				TCHAR messagestring[256];
				memset(messagestring, 0, sizeof(TCHAR)*256);
				
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_SELECTNAME) );
				g_NetworkKernel.InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // 2004.04.30->oneway48 modify
				
//				pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
			}
		}
		break;
		
	case _XDEF_MESSENGERTOOLTIPWINDOW_DELETEFRIENDOK :
		{
//			if(	!pMessengerTooltip_Window->m_bIsButtonProcess )
			{
				g_NetworkKernel.SendMessengerPacket(MMSG_NO_CUT_FRIEND_REQ_C_S);
				pMessengerTooltip_Window->ShowWindow(FALSE);
//				pMessengerTooltip_Window->m_bIsButtonProcess = TRUE;
			}
		}
		break;
		
	case _XDEF_MESSENGERTOOLTIPWINDOW_DELETEFRIENDNO :
		{
//			pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
		}
		break;

	case _XDEF_MESSENGERTOOLTIPWINDOW_CLOSEBUTTON :
		{
			if(pMessengerTooltip_Window)
			{
				if(pMessengerTooltip_Window->GetShowStatus())				
					pMessengerTooltip_Window->ShowWindow(FALSE);	
//				pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
			}
		}
		break;
	}
}

void XProc_MainGame::NameInputWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_NameInput* pNameInput_Window = (_XWindow_NameInput*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NAMEINPUTWINDOW);
	_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
	
	
	switch(wparam)
	{	
	case _XDEF_NAMEINPUTWINDOW_OK :
		{
			TCHAR resultstring[256]; 
			memset( resultstring, 0, sizeof(TCHAR)*256 );
			strcpy( resultstring, pNameInput_Window->m_IMEControl.GetText() );				
			
			if( strcmpi( resultstring, pNameInput_Window->m_IMEControl.GetText() ) == 0 )
			{ 

				switch(pNameInput_Window->m_NameInputMode)
				{
				case _XNAMEINPUT_GROUPNAME :
					{
						// �׷� �߰� ��û
						memset(pMessenger_Window->m_GroupName, 0, sizeof(TCHAR)*15);	
						memcpy(pMessenger_Window->m_GroupName, pNameInput_Window->m_IMEControl.GetText(), sizeof(TCHAR)*15);

						// 2004.06.01->oneway48 insert
						if( strcmp(pMessenger_Window->m_GroupName,_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_FRIEND)) == 0 )	// _T("ģ��")
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTCREATEGROUP),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// _T("������ �� ���� �׷��Դϴ�.")
							pNameInput_Window->ShowWindow(FALSE);	
							
//							_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//								= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
							
//							pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;	
							break;
						}
						// insert end

						if( pMessenger_Window->m_GroupName[0] != 0)
						{
							// �߰��� �׷� �̸��� �̹� �����ϴ� �׷� �̸����� üũ
							bool checkgroupname = FALSE;
							if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
							{
								pMessenger_Window->m_MessengerGroupList.resetList();
								_XMessengerGroup* currentgroup = NULL;
								
								do 
								{
									currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
									if(currentgroup)
									{
										if( strcmp( currentgroup->m_GroupName, pMessenger_Window->m_GroupName ) == 0 )
										{
											checkgroupname = TRUE;										
											break;	
										}				
										
									}	
									pMessenger_Window->m_MessengerGroupList.nextItem();
								} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());			
							}			
					

							if(!checkgroupname)
							{
								g_NetworkKernel.SendMessengerPacket(MMSG_NO_MAKE_GROUP_REQ_C_S);
							}
							else
							{
								TCHAR messagestring[256];
								memset(messagestring, 0, sizeof(TCHAR)*256);
								
								sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_ALREADYGROUP));
								g_NetworkKernel.InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // 2004.04.30->oneway48 modify

//								_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//									= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
								
//								pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;	
							}		
							pNameInput_Window->ShowWindow(FALSE);					
						}
						else
						{
							pNameInput_Window->ShowWindow(FALSE);
							
//							_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//								= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
							
//							pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
						}
					
					}
					break;
				case _XNAMEINPUT_CHARACTERNAME :
					{
						// ģ�� �߰� ��û 
						memset(pMessenger_Window->m_CharacterName, 0, sizeof(TCHAR) * 15);	
						strcpy( pMessenger_Window->m_CharacterName, pNameInput_Window->m_IMEControl.GetText() );

						// 2004.05.28->oneway48 insert
						if( strcmp(pMessenger_Window->m_CharacterName,g_pLocalUser->m_CharacterInfo.charactername) == 0 )
						{
							TCHAR messagestring[256];
							memset(messagestring, 0, sizeof(TCHAR)*256);
							
							sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTREGISTID));	// _T("����� �� ���� ���̵��Դϴ�.")
							g_NetworkKernel.InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); 
							
//							_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//								= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
							
//							pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
							pNameInput_Window->ShowWindow(FALSE);
							break;
						}
						//insert end

						if( pMessenger_Window->m_CharacterName[0] != 0 )
						{
							// �߰��� ģ���� �̹� ��ϵǾ� �ִ� ģ������ �˻�.
							bool checkfriendname = FALSE;
							
							if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
							{
								pMessenger_Window->m_MessengerGroupList.resetList();
								_XMessengerGroup* currentgroup = NULL;
								do 
								{
									currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
									if(currentgroup)
									{
										if(!currentgroup->m_FriendList.listEmpty())
										{
											currentgroup->m_FriendList.resetList();
											_XMessengerFriend* currentfriend = NULL;
											do 
											{
												currentfriend = (_XMessengerFriend*)currentgroup->m_FriendList.currentItem();
												if(currentfriend)
												{
													if( strcmp( currentfriend->m_FriendName,  pMessenger_Window->m_CharacterName ) == 0 )
													{
														checkfriendname = TRUE;												
														break;									
													}
												}
												currentgroup->m_FriendList.nextItem();							
											} while(!currentgroup->m_FriendList.atHeadOfList());						
										}					
									}			
									pMessenger_Window->m_MessengerGroupList.nextItem();
								} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());
							}

							if( !checkfriendname )
							{
								g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAKE_FRIEND_REQ_C_S_C );
							}
							else
							{
								TCHAR messagestring[256];
								memset(messagestring, 0, sizeof(TCHAR)*256);
								
								sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_ALREADYFRIEND));
								g_NetworkKernel.InsertChatString(messagestring,_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // 2004.04.30->oneway48 modify
								
//								_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//									= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
								
//								pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
							}						
							pNameInput_Window->ShowWindow(FALSE);
						}
						else
						{
							pNameInput_Window->ShowWindow(FALSE);
							
//							_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//								= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
							
//							pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
						}						
					}
					break;
				case _XNAMEINPUT_CHANGEGROUPNAME :
					{
						// �׷� �̸� ���� ��û 	
						
						memset(pMessenger_Window->m_GroupName, 0, sizeof(TCHAR)*15);	
						memset(pMessenger_Window->m_GroupReName, 0, sizeof(TCHAR)*15);	
						
						memcpy(pMessenger_Window->m_GroupReName, pNameInput_Window->m_IMEControl.GetText(), pNameInput_Window->m_IMEControl.GetLength());//sizeof(TCHAR)*14);
						memcpy(pMessenger_Window->m_GroupName, pMessenger_Window->m_SelectedGroupname, strlen(pMessenger_Window->m_SelectedGroupname));//sizeof(TCHAR)*14);	
					
						if( pMessenger_Window->m_GroupReName[0] != 0 )
						{
							if( strcmp(pMessenger_Window->m_GroupReName, _XGETINTERFACETEXT(ID_STRING_NETWORKTWO_FRIEND)) == 0 )
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								
								TCHAR messagestring[256];
								memset( messagestring, 0, sizeof(TCHAR)*256 );
								sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTCHANGEGROUPNAME));	// "������ �� ���� �׷��̸��Դϴ�."
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
								pMessageBox->StartTimer(5000, TRUE) ;
							}
							else
							{
								// ������ �׷� �̸��� �̹� �����ϴ� �׷� �̸����� üũ
								bool checkgroupname = FALSE;
								_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
								if( !pMessenger_Window->m_MessengerGroupList.listEmpty() )
								{
									pMessenger_Window->m_MessengerGroupList.resetList();
									_XMessengerGroup* currentgroup = NULL;
									
									do 
									{
										currentgroup = (_XMessengerGroup *)pMessenger_Window->m_MessengerGroupList.currentItem();
										if(currentgroup)
										{
											if( strcmp( currentgroup->m_GroupName, pMessenger_Window->m_GroupReName ) == 0 )
											{
												checkgroupname = TRUE;										
												break;	
											}				
											
										}	
										pMessenger_Window->m_MessengerGroupList.nextItem();
									} while(!pMessenger_Window->m_MessengerGroupList.atHeadOfList());			
								}
								
								if(!checkgroupname)
								{
									g_NetworkKernel.SendMessengerPacket(MMSG_NO_RENAME_GROUP_REQ_C_S);
								}
								else
								{
									
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									
									TCHAR messagestring[256];
									memset( messagestring, 0, sizeof(TCHAR)*256 );
									sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_ALREADYGROUP));
									pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);
									pMessageBox->StartTimer(5000, TRUE) ;
									
//									_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//										= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
//									pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
									
									
								}						
							}

							pNameInput_Window->ShowWindow(FALSE);
							
//							_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//								= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
//							pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
						}
						else
						{
							pNameInput_Window->ShowWindow(FALSE);
							
//							_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//								= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
							
//							pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
						}
						
					}
					break;
				}
			}				
			else
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				
				TCHAR warnstring[512];
				memset( warnstring, 0, sizeof(TCHAR)*512 );
				sprintf(warnstring, "%s [%s]", _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_VIOLATION), g_WordFilteringManager.GetLastFilterdWord());
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), warnstring, TRUE);
				pMessageBox->StartTimer(5000, TRUE) ;

			}
		}
		break;
	case _XDEF_NAMEINPUTWINDOW_CLOSE :
		{
			pNameInput_Window->ShowWindow(FALSE);
			
//			_XWindow_MessengerTooltip* pMessengerTooltip_Window 
//				= (_XWindow_MessengerTooltip*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERTOOLTIPWINDOW);
						
//			pMessengerTooltip_Window->m_bIsButtonProcess = FALSE;
		}
		break;
	}

}

void XProc_MainGame::NPCSkillWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_NPCSKILLWINDOW_LISTBOX :
		{
/*			if(m_pNPCSkillWindowPtr)
			{
				int selecteditem = m_pNPCSkillWindowPtr->CheckSelectedItem(true);

				if(selecteditem > 0)
				{
					m_pNPCSkillWindowPtr->SetDrawSelectedImage(true);

					DWORD cmdid = (DWORD)lparam;
					m_pNPCSkillWindowPtr->SetSelectedSkillPackageID((cmdid & 0xFF000000) >> 24);
					m_pNPCSkillWindowPtr->SetSelectedSkillNo((cmdid & 0x00FF0000) >> 16);
					m_pNPCSkillWindowPtr->SetSelectedSkillID((cmdid & 0x0000FFFF));
				}

				selecteditem = m_pNPCSkillWindowPtr->CheckSelectedIconItem();
				if(selecteditem > 0)
				{
					// Tooltip ����
				}
			}
	*/
		}
		break;
	case _XDEF_NPCSKILLWINDOW_REQLEARNSKILL :
		{
			if( g_pDefaultTooltip_Window )
			{
				if( g_pDefaultTooltip_Window->GetShowStatus() )						
					g_pDefaultTooltip_Window->ShowWindow( FALSE );
			}
			
			if(m_pNPCSkillWindowPtr)
			{				
				g_NetworkKernel.SendPacket(MSG_NO_SKILL_CTRL_REQ);
			}
		}
		break;
	case _XDEF_NPCSKILLWINDOW_REQLEARNSKILLBUTTON :
		{
			if(m_pNPCSkillWindowPtr->GetDrawSelectedImage())
			{
				short skillid = m_pNPCSkillWindowPtr->GetSelectedSkillID();
				if(skillid > 0)
				{
#ifdef _XTS_NEWSKILLWINDOW
					int learnedskillcount = 0;
					for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
					{
						if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
							learnedskillcount++;
					}

					if(learnedskillcount == _XDEF_MAX_LEARNSKILL)
					{
						// ���� ���̻� ��� �� ����
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2986), TRUE); //�� �̻� ������ ���� �� �� �����ϴ�. ������ ������ �� �ٽ� �õ��Ͻñ� �ٶ��ϴ�.
						pMessageBox->StartTimer(10000, TRUE) ;
						break;
					}
#endif
					if( g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESKILL || g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL )
					{						
						TCHAR skillnamestring[128];
						
						memset(skillnamestring, 0, sizeof(TCHAR)*128);
						sprintf(skillnamestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CANYOULEARN), g_SkillProperty[skillid]->skillName);	// "%s(��)�� ���ڽ��ϱ�?"
						
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), skillnamestring, TRUE, _XDEF_NPCSKILLWINDOW_REQLEARNSKILL, 0);
					}
					else
					{
						if( g_SkillProperty[skillid]->motion_male == -1 )//���� ������ ���� ��� : ���ڸ� ��� �� �ִ�.
						{
							if( g_pLocalUser->m_CharacterInfo.gender == _XGENDER_MAN )
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1911), TRUE);
								pMessageBox->StartTimer(5000, TRUE) ;
							}
							else
							{
								TCHAR skillnamestring[128];
								
								memset(skillnamestring, 0, sizeof(TCHAR)*128);
								sprintf(skillnamestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CANYOULEARN), g_SkillProperty[skillid]->skillName);	// "%s(��)�� ���ڽ��ϱ�?"
								
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), skillnamestring, TRUE, _XDEF_NPCSKILLWINDOW_REQLEARNSKILL, 0);
							}
						}
						else if( g_SkillProperty[skillid]->motion_female == -1 )//���� ������ ���� ��� : ���ڸ� ��� �� �ִ�.
						{
							if( g_pLocalUser->m_CharacterInfo.gender == _XGENDER_WOMAN )
							{
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1912), TRUE);
								pMessageBox->StartTimer(5000, TRUE) ;
							}
							else
							{
								TCHAR skillnamestring[128];
								
								memset(skillnamestring, 0, sizeof(TCHAR)*128);
								sprintf(skillnamestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CANYOULEARN), g_SkillProperty[skillid]->skillName);	//"%s(��)�� ���ڽ��ϱ�?"
								
								_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), skillnamestring, TRUE, _XDEF_NPCSKILLWINDOW_REQLEARNSKILL, 0);
							}
						}
						else
						{
							TCHAR skillnamestring[128];
							
							memset(skillnamestring, 0, sizeof(TCHAR)*128);
							sprintf(skillnamestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CANYOULEARN), g_SkillProperty[skillid]->skillName);	// "%s(��)�� ���ڽ��ϱ�?"
							
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), skillnamestring, TRUE, _XDEF_NPCSKILLWINDOW_REQLEARNSKILL, 0);
						}
					}
				}
			}
		}
		break;
	case _XDEF_NPCSKILLWINDOW_HELP :
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_EDUCATION );
		}
		break;
	case _XDEF_NPCSKILLWINDOW_LEFT :
		{
			int index = m_pNPCSkillWindowPtr->m_StartIndex-1;
			m_pNPCSkillWindowPtr->SetStartIndex(index);
			m_pNPCSkillWindowPtr->SetSkillTypeButtonTexture();
		}
		break;
	case _XDEF_NPCSKILLWINDOW_RIGHT :
		{
			int index = m_pNPCSkillWindowPtr->m_StartIndex+1;
			m_pNPCSkillWindowPtr->SetStartIndex(index);
			m_pNPCSkillWindowPtr->SetSkillTypeButtonTexture();
		}
		break;
		
	case _XDEF_NPCSKILLWINDOW_SKILL_1 :
	case _XDEF_NPCSKILLWINDOW_SKILL_2 :
	case _XDEF_NPCSKILLWINDOW_SKILL_3 :
	case _XDEF_NPCSKILLWINDOW_SKILL_4 :
	case _XDEF_NPCSKILLWINDOW_SKILL_5 :
	case _XDEF_NPCSKILLWINDOW_SKILL_6 :
	case _XDEF_NPCSKILLWINDOW_SKILL_7 :
	case _XDEF_NPCSKILLWINDOW_SKILL_8 :
		{
			m_pNPCSkillWindowPtr->SetSelectedSkillTypeIndex(wparam - _XDEF_NPCSKILLWINDOW_SKILL_1);
			m_pNPCSkillWindowPtr->SetListItem();
		}
		break;
	}
}

#ifdef _ACCLAIM_IGAADSYSTEM
extern BOOL	g_LoadingProcess_ViewAD;
#endif

void XProc_MainGame::RebirthWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_REBIRTHWINDOW_TOWN :
		{
			// ������ ���ư���
			_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
			if(pRebirth_Window)
			{
				g_NetworkKernel.SendPacket(MSG_NO_CHARAC_SELECT_MODE, en_charac_select_at_city);

#ifdef ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI
				pRebirth_Window->EnableWindow(FALSE);
#else
				pRebirth_Window->ShowWindow(FALSE);				
#endif

#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321 // PK����ġ����, ������ PK����	
				if(!g_pLocalUser->m_bCastleBattle)
				{
					pRebirth_Window->m_bEnablePKTracing = TRUE;
				}
#endif

#ifdef _ACCLAIM_IGAADSYSTEM

	#ifdef _ACCLAIM_IGAADSYSTEM_DISABLEDUPLICATE_AD_VIEW

	#else
				if( pRebirth_Window->m_VideoADCanceled )
				{
					m_AcclaimADMode_DeathPhase	= _XACCLAIMAD_DEATHPHASE_STATICAD;
					m_AcclaimADMode_MainGame	= _XACCLAIMAD_MAINGAME_DEATH;
					m_DeathStaticBackgroundAlphaLevel = 160;
					g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_DEATH );
					g_IGAADWrapper.ShowWindow( TRUE );
					g_IGAADWrapper.StartDisplay();
				}				
	#endif

#endif
			}			
		}
		break;

	case _XDEF_REBIRTHWINDOW_SAVEPOINT :
		{
			// ���� Ǭ ������ ��Ȱ�ϱ�
			_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
			if(pRebirth_Window)
			{
				if(g_pLocalUser->m_bCastleBattle)	// ����� �߿��� �ٸ� ���� ��û�Ѵ�.
					g_NetworkKernel.SendPacket(MSG_NO_CHARAC_SELECT_MODE, en_charac_select_in_castle);
				else
					g_NetworkKernel.SendPacket(MSG_NO_CHARAC_SELECT_MODE, en_charac_select_at_save_position);

#ifdef ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI
				pRebirth_Window->EnableWindow(FALSE);
#else
				pRebirth_Window->ShowWindow(FALSE);
#endif
				
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321 // PK����ġ����, ������ PK����
				if(!g_pLocalUser->m_bCastleBattle)
				{
					pRebirth_Window->m_bEnablePKTracing = TRUE;
				}
#endif

#ifdef _ACCLAIM_IGAADSYSTEM

	#ifdef _ACCLAIM_IGAADSYSTEM_DISABLEDUPLICATE_AD_VIEW

	#else
				if( pRebirth_Window->m_VideoADCanceled )
				{
					m_AcclaimADMode_DeathPhase	= _XACCLAIMAD_DEATHPHASE_STATICAD;
					m_AcclaimADMode_MainGame	= _XACCLAIMAD_MAINGAME_DEATH;					
					m_DeathStaticBackgroundAlphaLevel = 160;
					g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_DEATH );
					g_IGAADWrapper.ShowWindow( TRUE );
					g_IGAADWrapper.StartDisplay();
				}
	#endif

#endif
			}
		}
		break;
		
	case _XDEF_REBIRTHWINDOW_FIELD :
		{
			// �ʵ忡�� ��Ȱ�ϱ�
			_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
			if(pRebirth_Window)
			{
				g_NetworkKernel.SendPacket(MSG_NO_CHARAC_SELECT_MODE, en_charac_select_at_last_position);

#ifdef ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI
				pRebirth_Window->EnableWindow(FALSE);
#else
				pRebirth_Window->ShowWindow(FALSE);
#endif

#ifdef _ACCLAIM_IGAADSYSTEM
				if( pRebirth_Window->m_VideoADCanceled )
				{
					m_AcclaimADMode_DeathPhase	= _XACCLAIMAD_DEATHPHASE_STATICAD;
					m_AcclaimADMode_MainGame	= _XACCLAIMAD_MAINGAME_DEATH;
					m_DeathStaticBackgroundAlphaLevel = 160;
					g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_DEATH );
					g_IGAADWrapper.ShowWindow( TRUE );
					g_IGAADWrapper.StartDisplay();
				}
#endif
			}
		}
		break;
		
	case _XDEF_REBIRTHWINDOW_USEITEM :
		{
			// ȭŸ�Ŵ� ��� ��Ŷ ������
			_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
			int slot = g_pLocalUser->GetUserItemInvenSlotFromSecondType(_XGI_FC_POTION, _XGI_SC_POTION_HWATA);
			if(slot >= 0)
			{
				g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, slot);

#if defined(ALL_RB_REBIRTH_USERITEM_071108_KUKURI) || defined(ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI)
				pRebirth_Window->EnableWindow(FALSE);
#else
				pRebirth_Window->ShowWindow(FALSE);
#endif
			}
			
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321 // PK����ġ����, ������ PK����	
			if(!g_pLocalUser->m_bCastleBattle)
			{
				pRebirth_Window->m_bEnablePKTracing = TRUE;
			}
#endif

#ifdef _ACCLAIM_IGAADSYSTEM
			if( pRebirth_Window->m_VideoADCanceled )
			{
				m_AcclaimADMode_DeathPhase	= _XACCLAIMAD_DEATHPHASE_STATICAD;
				m_AcclaimADMode_MainGame	= _XACCLAIMAD_MAINGAME_DEATH;
				m_DeathStaticBackgroundAlphaLevel = 160;
				g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_DEATH );
				g_IGAADWrapper.ShowWindow( TRUE );
				g_IGAADWrapper.StartDisplay();
			}
#endif
		}
		break;
	case _XDEF_REBIRTHWINDOW_ACCLAIMVIDEOAD :
		{
#ifdef _ACCLAIM_IGAADSYSTEM
			m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_DEATH;
			m_AcclaimADMode_DeathPhase = _XACCLAIMAD_DEATHPHASE_VIDEOAD;			
			g_IGAADWrapper.DisableContentsUpdate( FALSE );
			g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_DEATHVIDEOAD );
			g_IGAADWrapper.ShowWindow( TRUE );
			g_IGAADWrapper.StartDisplay();
			_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
			if(pRebirth_Window)
			   pRebirth_Window->m_VideoADCanceled = FALSE;
#endif
		}
		break;
	}
}

//2004.06.17->oneway48 insert
void XProc_MainGame::RepairItemWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_REPAIRITEMWINDOW_ALLBUTTON :
		{
			if(g_pInventory_Window && m_pRepairItemWindowPtr)
			{
				DWORD needmoney = 0;
				int selectedslot = -1;
				int shortdur = 0;
				g_pInventory_Window->m_MouseSlotItem.Reset();
				m_pRepairItemWindowPtr->m_SelectedSlotNoList.clear();

				//2004.06.19->oneway48 insert
				for(  selectedslot = _XINVENTORY_SLOTTYPE_AMOR; selectedslot < _XINVENTORY_SLOTTYPE_POCKETSTART; selectedslot++ )
				{
					if( m_pRepairItemWindowPtr->m_bRepairWeapon )
					{
						if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON || 
							g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON2 ||
							g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON3 )
						{
							shortdur = g_pLocalUser->m_UserItemList[selectedslot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[selectedslot].Get_m_usCurDur();
							if( shortdur > 0)
							{
								m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back(selectedslot);
								needmoney += (shortdur-1)/15 + 1;							
							}
						}
					}
					else
					{
						if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES || 
							g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES2 ||
							g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES3 )
						{
							shortdur = g_pLocalUser->m_UserItemList[selectedslot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[selectedslot].Get_m_usCurDur();
							if( shortdur > 0)
							{
								m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back(selectedslot);
								needmoney += (shortdur-1)/15 + 1;							
							}
						}	
					}

				}

				switch( g_pInventory_Window->m_SelectedBaseTab ) 
				{
				case _XINVENTORYBASETAB_POCKET	: 
					{
						for( selectedslot = _XINVENTORY_SLOTTYPE_POCKETSTART; selectedslot < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART ; selectedslot++ )
						{
							if( m_pRepairItemWindowPtr->m_bRepairWeapon )
							{
								if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON || 
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON2 ||
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON3 )
								{
									shortdur = g_pLocalUser->m_UserItemList[selectedslot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[selectedslot].Get_m_usCurDur();
									if( shortdur > 0)
									{
										m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back(selectedslot);
										needmoney += (shortdur-1)/15 + 1;							
									}
								}	
							}
							else
							{
								if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES || 
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES2 ||
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES3 )
								{
									shortdur = g_pLocalUser->m_UserItemList[selectedslot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[selectedslot].Get_m_usCurDur();
									if( shortdur > 0)
									{
										m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back(selectedslot);
										needmoney += (shortdur-1)/15 + 1;							
									}
								}	
							}
							
						}
					}
					break;
				case _XINVENTORYBASETAB_BACKPACK: 
					{
						for( selectedslot = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART; selectedslot < _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART ; selectedslot++ )
						{	
							if( m_pRepairItemWindowPtr->m_bRepairWeapon )
							{
								if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON || 
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON2 ||
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON3 )
								{
									shortdur = g_pLocalUser->m_UserItemList[selectedslot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[selectedslot].Get_m_usCurDur();
									if( shortdur > 0)
									{
										m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back(selectedslot);
										needmoney += (shortdur-1)/15 + 1;							
									}
								}	
							}
							else
							{
								if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES || 
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES2 ||
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES3 )
								{
									shortdur = g_pLocalUser->m_UserItemList[selectedslot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[selectedslot].Get_m_usCurDur();
									if( shortdur > 0)
									{
										m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back(selectedslot);
										needmoney += (shortdur-1)/15 + 1;							
									}
								}	
							}
						}	
					}
					break;
				case _XINVENTORYBASETAB_BACKPACK2	: 
                    {
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ�� //�󽽷� ������ ĳ�� �೶ �˻�
                        for( selectedslot = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; selectedslot < _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART ; selectedslot++ )
                    #else
                        for( selectedslot = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART; selectedslot < _XINVENTORY_SLOTTYPE_TOTALCOUNT ; selectedslot++ )
                    #endif
						{	
							if( m_pRepairItemWindowPtr->m_bRepairWeapon )
							{
								if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON || 
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON2 ||
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON3 )
								{
									shortdur = g_pLocalUser->m_UserItemList[selectedslot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[selectedslot].Get_m_usCurDur();
									if( shortdur > 0)
									{
										m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back(selectedslot);
										needmoney += (shortdur-1)/15 + 1;							
									}
								}	
							}
							else
							{
								if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES || 
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES2 ||
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES3 )
								{
									shortdur = g_pLocalUser->m_UserItemList[selectedslot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[selectedslot].Get_m_usCurDur();
									if( shortdur > 0)
									{
										m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back(selectedslot);
										needmoney += (shortdur-1)/15 + 1;							
									}
								}	
							}
						}	
                    }
					break;
                #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ�� //�󽽷� ������ ĳ�� �೶ �˻�
                case _XINVENTORY_SLOTTYPE_CASHBACKPACK1	: 
                    {
                        if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
                            break;

                        for( selectedslot = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART; selectedslot < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART ; selectedslot++ )
						{	
							if( m_pRepairItemWindowPtr->m_bRepairWeapon )
							{
								if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON || 
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON2 ||
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON3 )
								{
									shortdur = g_pLocalUser->m_UserItemList[selectedslot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[selectedslot].Get_m_usCurDur();
									if( shortdur > 0)
									{
										m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back(selectedslot);
										needmoney += (shortdur-1)/15 + 1;							
									}
								}	
							}
							else
							{
								if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES || 
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES2 ||
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES3 )
								{
									shortdur = g_pLocalUser->m_UserItemList[selectedslot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[selectedslot].Get_m_usCurDur();
									if( shortdur > 0)
									{
										m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back(selectedslot);
										needmoney += (shortdur-1)/15 + 1;							
									}
								}	
							}
						}	
                    }
					break;	
                case _XINVENTORY_SLOTTYPE_CASHBACKPACK2	: 
                    {
                        if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
                            break;

                        for( selectedslot = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART; selectedslot < _XINVENTORY_SLOTTYPE_TOTALCOUNT ; selectedslot++ )
						{	
							if( m_pRepairItemWindowPtr->m_bRepairWeapon )
							{
								if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON || 
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON2 ||
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_WEAPON3 )
								{
									shortdur = g_pLocalUser->m_UserItemList[selectedslot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[selectedslot].Get_m_usCurDur();
									if( shortdur > 0)
									{
										m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back(selectedslot);
										needmoney += (shortdur-1)/15 + 1;							
									}
								}	
							}
							else
							{
								if( g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES || 
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES2 ||
									g_pLocalUser->m_UserItemList[selectedslot].Get_m_cType() == _XGI_FC_CLOTHES3 )
								{
									shortdur = g_pLocalUser->m_UserItemList[selectedslot].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[selectedslot].Get_m_usCurDur();
									if( shortdur > 0)
									{
										m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back(selectedslot);
										needmoney += (shortdur-1)/15 + 1;							
									}
								}	
							}
						}	
                    }
					break;	
                #endif
				}

				if( m_pRepairItemWindowPtr->m_SelectedSlotNoList.empty() )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTREPAIRITEM), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // ������ ��ǰ�� �����ϴ�.
				}
				else
				{
					if( g_pLocalUser->m_Money >= needmoney )
					{
						g_pInventory_Window->m_bDrawSelectedItem = TRUE; 
						
						m_pRepairItemWindowPtr->m_NeedMoney = needmoney;//2004.06.18->oneway48 insert
						
						//2004.06.29->oneway48 modify
						TCHAR messagestring[256];
						memset(messagestring, 0, sizeof(TCHAR)*256);
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ALLREPAIR), m_pRepairItemWindowPtr->m_NeedMoney);	// _T("    �ջ�� ��ǰ�� ���� �����Ͻðڽ��ϱ�?   ���� �ݾ� : %d ��")
						
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_REPAIRITEMWINDOW_ALLREPAIRYES, _XDEF_REPAIRITEMWINDOW_ALLREPAIRNO);					
//						g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_REPAIRITEMWINDOW_ALLREPAIRYES, _XDEF_REPAIRITEMWINDOW_ALLREPAIRNO);					
						
						// ��Ŷ�� ������.
					}
					else
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTENOUGHREPAIRMONEY), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "������ �����Ͽ� ������ �� �����ϴ�."
						
					}
				}
			}
		}
		break;
		
	case _XDEF_REPAIRITEMWINDOW_REPAIRBUTTON :
		{
			if(g_pInventory_Window && m_pRepairItemWindowPtr)
			{
				if( m_pRepairItemWindowPtr->m_SelectedSlotNoList.empty() )
				{
					m_pNPCScriptWindowPtr->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTREPAIRITEM), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // ������ ��ǰ�� �����ϴ�.
				}
				else
				{
					if( m_pRepairItemWindowPtr->m_NeedMoney == 0 )
					{
						m_pNPCScriptWindowPtr->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTREPAIRITEM), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // ������ ��ǰ�� �����ϴ�.
					}
					else
					{
						if( g_pLocalUser->m_Money >= m_pRepairItemWindowPtr->m_NeedMoney )
						{
							_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
							if( pChargeGauge_Window )
							{
								if(!pChargeGauge_Window->GetShowStatus())
								{
									FLOAT time = 1000 + ( m_pRepairItemWindowPtr->m_SelectedSlotNoList.size() % 10 ) * 1000;
									pChargeGauge_Window->SetDelayTime( time );
									POINT pos = m_pRepairItemWindowPtr->GetWindowPos();
									pos.x += 12;
									pos.y += 200;
									pChargeGauge_Window->SetDrawPosition(pos);
									pChargeGauge_Window->SetGaugeMode( _XGAUGE_REPAIR );
									pChargeGauge_Window->ShowWindow(TRUE);
									m_pRepairItemWindowPtr->m_RepairMode = RM_UNDERREPAIR;
									m_pRepairItemWindowPtr->m_SelectedAllButton->EnableWindow(FALSE);
									m_pRepairItemWindowPtr->m_RepairButton->EnableWindow(FALSE);
									m_pRepairItemWindowPtr->m_SelectedCancleButton->EnableWindow(FALSE);	
									g_MainWindowManager.SetTopWindow( pChargeGauge_Window );
								}
							}
							//g_NetworkKernel.SendPacket(MSG_NO_NPC_ITEM_REPAIR_REQUEST);
						}
						else
						{
							m_pNPCScriptWindowPtr->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTENOUGHREPAIRMONEY), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "������ �����Ͽ� ������ �� �����ϴ�."
							
						}
					}					
				}			
			}
		}
		break;		
	case _XDEF_REPAIRITEMWINDOW_ALLREPAIRYES :
		{
			if(g_pInventory_Window && m_pRepairItemWindowPtr)
			{
				g_NetworkKernel.SendPacket(MSG_NO_NPC_ITEM_REPAIR_REQUEST);// 2004.06.18->oneway48 insert
			}
		}
		break;
	case _XDEF_REPAIRITEMWINDOW_ALLREPAIRNO :
		{
			if(g_pInventory_Window && m_pRepairItemWindowPtr)
			{
				g_pInventory_Window->m_MouseSlotItem.Reset();
				m_pRepairItemWindowPtr->m_SelectedSlotNoList.clear();
				m_pNPCScriptWindowPtr->InsertNPCChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CANCLEREPAIR), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "��ǰ ������ ��ҵǾ����ϴ�."
			}
		}
		break;
	case _XDEF_REPAIRITEMWINDOW_CANCLE :
		{
			if(m_pRepairItemWindowPtr && g_pInventory_Window)
			{
				g_pInventory_Window->m_MouseSlotItem.Reset();
				m_pRepairItemWindowPtr->m_SelectedSlotNoList.clear();
				
				m_pRepairItemWindowPtr->ShowWindow(FALSE);
				g_pInventory_Window->ShowWindow(FALSE);
			}			
		}
		break;
	case _XDEF_REPAIRITEMWINDOW_CLOSE :
		{
			if( m_pRepairItemWindowPtr && g_pInventory_Window )
			{
				_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHARGEGAUGE);
				if( pChargeGauge_Window )
				{
					if( !pChargeGauge_Window->GetShowStatus() )
					{
						if(m_pRepairItemWindowPtr->GetShowStatus())
							m_pRepairItemWindowPtr->ShowWindow(FALSE);
						
						if(g_pInventory_Window->GetShowStatus())
							g_pInventory_Window->ShowWindow(FALSE);
					}
					else
					{
						g_MainWindowManager.SetTopWindow(pChargeGauge_Window);
					}
				}
			}						
		}
		break;
		
	case _XDEF_REPAIRITEMWINDOW_SELECTEDALL :
		{
			m_pRepairItemWindowPtr->m_NeedMoney = 0;
			if( !m_pRepairItemWindowPtr->m_SelectedSlotNoList.empty() )
				m_pRepairItemWindowPtr->m_SelectedSlotNoList.clear();

			for( int i = _XINVENTORY_SLOTTYPE_POCKETSTART; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend; i++ )
			{
				if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
				{
					int shortdur = g_pLocalUser->m_UserItemList[i].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[i].Get_m_usCurDur();
					if( shortdur > 0)
					{
						m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back( i );												
					}					
				}
			}
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ�� //�󽽷� ������ ĳ�� �೶ �˻�
			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++)
			{
				if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
				{
					int shortdur = g_pLocalUser->m_UserItemList[i].Get_m_usMaxDur() - g_pLocalUser->m_UserItemList[i].Get_m_usCurDur();
					if( shortdur > 0)
					{
						m_pRepairItemWindowPtr->m_SelectedSlotNoList.push_back( i );												
					}					
				}
			}
		#endif

			m_pRepairItemWindowPtr->SetRepairListItem();
		}
		break;	
	case _XDEF_REPAIRITEMWINDOW_SELECTEDCANCLE :
		{
			m_pRepairItemWindowPtr->m_NeedMoney = 0;
			m_pRepairItemWindowPtr->m_RepairMode = RM_ENDREPAIR;			
			m_pRepairItemWindowPtr->ResetRepairListData();
		}
		break;
	case _XDEF_REPAIRITEMWINDOW_HELP :
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_REPAIR );
		}
		break;
	}
}

void XProc_MainGame::MeditationMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
	
	if(pMeditation_Window)
	{
		switch(wparam) 
		{		
		case _XDEF_JINSTATUS_LEVELUPBUTTON		:	
			{
				g_pLocalUser->m_UserState = _XDEF_USERSTATE_TRAINING;
				g_pLocalUser->m_SelectedTrainingMode = _XTRAINING_MEDITATION;
				g_pLocalUser->m_CurrentTrainingState = _XUSER_STATE_1;
			}
			break;
		}
	}
}

void XProc_MainGame::TrainingMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_JINSTATUS_LEVELUPBUTTON :
		{
			// ��� ���� ����
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_STARTMEDITATION), TRUE, _XDEF_TRAINING_MEDITATION_START, _XDEF_TRAINING_MEDITATION_EXIT);	// _T("�� �ڸ����� ��������� �Ͻðڽ��ϱ�?")
		}
		break;
	case _XDEF_TRAINING_MEDITATION_START :
		{
			_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
			if(pTrainingHelp_Window)
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_NORMALSTART01_WAV);

				pTrainingHelp_Window->SetTrainingType(_XTRAINING_MEDITATION);
				pTrainingHelp_Window->ShowWindow(TRUE);
				g_MainWindowManager.SetTopWindow(pTrainingHelp_Window);

				// ����Ʈ�� �̴ϰ��� ����� ��ҹ�ư ��Ȱ��ȭ 
				_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
				if(pMeditation_Window)
				{
					if( pMeditation_Window->m_bQuestMiniGameStart )
					{
						pTrainingHelp_Window->m_EndButton->EnableWindow(FALSE);
					}
					else
					{
						pTrainingHelp_Window->m_EndButton->EnableWindow(TRUE);
						if(g_pJinStatus_Window)
							g_pJinStatus_Window->SetButtonStatus(FALSE);
					}
				}
			}
		}
		break;
	case _XDEF_TRAINING_MEDITATION_EXIT :
		{
			// ���� �� ��ư �ٽ� ����ֱ�
			if(g_pLocalUser->m_CharacterInfo.Get_jinlevel() >= g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience)
			{
				if(g_pJinStatus_Window)
				{
					g_pJinStatus_Window->SetButtonStatus(TRUE);
				}
			}
		}
		break;
	case _XDEF_TRAINING_DUMMY_START :
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_NORMALSTART01_WAV);

			_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
			if(pTrainingHelp_Window)
			{
				pTrainingHelp_Window->SetTrainingType(_XTRAINING_DUMMY);
				pTrainingHelp_Window->ShowWindow(TRUE);
			}

			_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
			if(pDummy_Window)
			{
				pDummy_Window->m_pDummy = (_XDummy*)g_pLocalUser->m_AttackTarget;
				pDummy_Window->ShowWindow(TRUE);
			}
		}
		break;
	case _XDEF_TRAINING_HELP_START :
		{
			_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
			if(pTrainingHelp_Window)
			{
				if(pTrainingHelp_Window->GetTrainingType() == _XTRAINING_DUMMY)
				{
					// ���� ����
					g_pLocalUser->m_UserState = _XDEF_USERSTATE_TRAINING;
					g_pLocalUser->m_SelectedTrainingMode = _XTRAINING_DUMMY;
					g_pLocalUser->m_CurrentTrainingState = _XUSER_STATE_1;
				}
				else if(pTrainingHelp_Window->GetTrainingType() == _XTRAINING_SPELL)
				{
					// ��� ����
					g_pLocalUser->m_UserState = _XDEF_USERSTATE_TRAINING;
					g_pLocalUser->m_SelectedTrainingMode = _XTRAINING_SPELL;
					g_pLocalUser->m_CurrentTrainingState = _XUSER_STATE_1;
				}
				else if(pTrainingHelp_Window->GetTrainingType() == _XTRAINING_MEDITATION)
				{
					// ��� ����
					_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
					if(pMeditation_Window)
					{
						if(!pMeditation_Window->m_bQuestMiniGameStart)
							pMeditation_Window->SetMeditationStep(g_pLocalUser->m_CharacterInfo.Get_level());
						pMeditation_Window->ShowWindow(TRUE);
					}

					g_pLocalUser->m_UserState = _XDEF_USERSTATE_TRAINING;
					g_pLocalUser->m_SelectedTrainingMode = _XTRAINING_MEDITATION;
					g_pLocalUser->m_CurrentTrainingState = _XUSER_STATE_1;
				}
			}
		}
		break;
	case _XDEF_TRAINING_HELP_END :
		{
			_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
			if(pTrainingHelp_Window)
			{
				if(pTrainingHelp_Window->GetTrainingType() == _XTRAINING_DUMMY)
				{
					// ���� ����
					_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
					if(pDummy_Window)
					{
						pDummy_Window->ShowWindow(FALSE);
						pDummy_Window->Reset();

						if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_DUMMY)
							g_pLocalUser->ResetAttackTarget();
						if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_DUMMY)
							g_pLocalUser->ResetViewTarget();
					}
				}
				else if(pTrainingHelp_Window->GetTrainingType() == _XTRAINING_SPELL)
				{
					// ��� ����
					_XWindow_TrainingSpell* pSpell_Window = (_XWindow_TrainingSpell*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGSPELL);
					if(pSpell_Window)
					{
						pSpell_Window->ShowWindow(FALSE);
						pSpell_Window->Reset();

						if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_SEAT)
							g_pLocalUser->ResetAttackTarget();
						if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_SEAT)
							g_pLocalUser->ResetViewTarget();
					}
				}
				else if(pTrainingHelp_Window->GetTrainingType() == _XTRAINING_MEDITATION)
				{
					// ��� ����
					_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
					if(pMeditation_Window)
					{
						pMeditation_Window->ShowWindow(FALSE);
						pMeditation_Window->Reset();
					}

					if(g_pLocalUser->m_CharacterInfo.Get_jinlevel() >= g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience)
					{
						if(g_pJinStatus_Window)
						{
							g_pJinStatus_Window->SetButtonStatus(TRUE);
						}
					}
				}
				
				pTrainingHelp_Window->ShowWindow(FALSE);
				g_pLocalUser->ResetTrainingMode();
			}
		}
		break;
	case _XDEF_TRAINING_CANCEL_OK :
		{
			if(g_pLocalUser->m_bSendTrainingStartPacket)
			{
				g_NetworkKernel.SendPacket(MSG_NO_MINIGAME_MAST_STAGE_C_S, 0);
			}
			else
			{
				
				_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
				if(pDummy_Window)
				{
					if(pDummy_Window->GetShowStatus())
					{
						pDummy_Window->ShowWindow(FALSE);
						pDummy_Window->Reset();
					}
				}
				_XWindow_TrainingSpell* pSpell_Window = (_XWindow_TrainingSpell*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGSPELL);
				if(pSpell_Window)
				{
					if(pSpell_Window->GetShowStatus())
					{
						pSpell_Window->ShowWindow(FALSE);
						pSpell_Window->Reset();
					}
				}

				_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
				if(pTrainingHelp_Window)
				{
					if(pTrainingHelp_Window->GetShowStatus())
						pTrainingHelp_Window->ShowWindow(FALSE);
				}

				g_pLocalUser->ResetTrainingMode();
			}
		}
		break;
	case _XDEF_TRAINING_CANCEL_NO :
		{
			switch(g_pLocalUser->m_SelectedTrainingMode)
			{
			case _XTRAINING_DUMMY :
				{
					_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
					if(!pDummy_Window)
						return;
					
					pDummy_Window->ResetMousePosition();
					g_pLocalUser->m_CurrentTrainingState = _XUSER_STATE_3;
					g_pLocalUser->SetAttackTarget(pDummy_Window->m_pDummy, _XPICKINGTARGET_DUMMY);
				}
				break;
			case _XTRAINING_SPELL :
				{
				}
				break;
			}
		}
		break;
	case _XDEF_TRAINING_SPELL_START :
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_NORMALSTART01_WAV);
			
			_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
			if(pTrainingHelp_Window)
			{
				pTrainingHelp_Window->SetTrainingType(_XTRAINING_SPELL);
				pTrainingHelp_Window->ShowWindow(TRUE);
			}

			_XWindow_TrainingSpell* pSpell_Window = (_XWindow_TrainingSpell*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGSPELL);
			if(pSpell_Window)
			{
				pSpell_Window->ShowWindow(TRUE);
			}
		}
		break;
	case _XDEF_TRAINING_HELP_LEFT :
		{
			_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
			if(pTrainingHelp_Window)
			{
				int startindex = pTrainingHelp_Window->GetStartIndex();
				startindex--;
				pTrainingHelp_Window->SetStartIndex(startindex);
				pTrainingHelp_Window->SetSkillButtonTexture();
			}
		}
		break;
	case _XDEF_TRAINING_HELP_RIGHT :
		{
			_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
			if(pTrainingHelp_Window)
			{
				int startindex = pTrainingHelp_Window->GetStartIndex();
				startindex++;
				pTrainingHelp_Window->SetStartIndex(startindex);
				pTrainingHelp_Window->SetSkillButtonTexture();
			}
		}
		break;
	case _XDEF_TRAINING_HELP_SKILL_1 :
	case _XDEF_TRAINING_HELP_SKILL_2 :
	case _XDEF_TRAINING_HELP_SKILL_3 :
	case _XDEF_TRAINING_HELP_SKILL_4 :
	case _XDEF_TRAINING_HELP_SKILL_5 :
	case _XDEF_TRAINING_HELP_SKILL_6 :
	case _XDEF_TRAINING_HELP_SKILL_7 :
	case _XDEF_TRAINING_HELP_SKILL_8 :
		{
			_XWindow_TrainingHelp* pTrainingHelp_Window = (_XWindow_TrainingHelp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGHELP);
			if(pTrainingHelp_Window)
			{
				pTrainingHelp_Window->SetSelectedSkillButtonIndex(wparam - _XDEF_TRAINING_HELP_SKILL_1);

				if(pTrainingHelp_Window->GetTrainingType() == _XTRAINING_DUMMY)
					g_pLocalUser->m_TrainingDummyInfo.skillid = pTrainingHelp_Window->GetSelectedSkillID();
				else if(pTrainingHelp_Window->GetTrainingType() == _XTRAINING_SPELL)
					g_pLocalUser->m_TrainingSpellInfo.skillid = pTrainingHelp_Window->GetSelectedSkillID();
					
			}
		}
		break;
	case _XDEF_JINSTATUS_ABILITYUPBUTTON :
		{
			_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
			if(pMainFrame_Window)
			{
				if( g_pLocalUser->m_CharacterInfo.Get_leftpoint() > 0 )
				{
					pMainFrame_Window->ChangeWindowTab(_XDEF_MAINFRAME_CHARACTORTAB);
					pMainFrame_Window->ShowWindow(TRUE);
					g_MainWindowManager.SetTopWindow( pMainFrame_Window );														
				}
				else
				{
					if( g_pLocalUser->m_CharacterInfo.Get_leftmasterypoint() > 0 )
					{
						pMainFrame_Window->ChangeWindowTab(_XDEF_MAINFRAME_MASTERYTAB);
						pMainFrame_Window->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow( pMainFrame_Window );							
					}
				}
			}			
		}
		break;		
	}
}


void XProc_MainGame::EnterBossRoomMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_EnterBossRoom* pEnterBossRoom_Window = (_XWindow_EnterBossRoom*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOM);
	
	if( pEnterBossRoom_Window )
	{
		switch(wparam) 
		{		
		case _XDEF_BOSSROOM_ENTER		:	
			{	
				// �����ϱ� ���� �������� �ٽ� �˻��Ѵ�.
				if( pEnterBossRoom_Window->CheckRequirement() )
				{
					if( g_pLocalUser->m_InParty )
					{
						int count = 0;
						for( int i = 0; i < _XDEF_MAXPARTYMEMBERCOUNT; i++ )
						{
							if( pEnterBossRoom_Window->m_EnterPartyMember[i].PartyMemberNo != -1 )
							{
								count++;
							}
						}
						int restnumber = g_pLocalUser->m_PartyMemberCount - count -1;
						
						if( restnumber != 0 )
						{
							TCHAR messagestring[256];				
							memset(messagestring, 0, sizeof(TCHAR)*256);
							sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1913), restnumber);
							
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessageColor(_XSC_INFORMATION);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_BOSSROOM_ENTEROK, 0);					
						}
						else
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_BOSSBUTTON01_WAV);
							g_NetworkKernel.SendPacket( MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S, g_ChannelTempZoneInfoIndex );
							pEnterBossRoom_Window->ResetNeedItemInfo();
							pEnterBossRoom_Window->ShowWindow(FALSE);
							g_pInventory_Window->ShowWindow(FALSE);
						}
					}
					else
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_BOSSBUTTON01_WAV);
						g_NetworkKernel.SendPacket( MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S, g_ChannelTempZoneInfoIndex );
						pEnterBossRoom_Window->ResetNeedItemInfo();
						pEnterBossRoom_Window->ShowWindow(FALSE);
						g_pInventory_Window->ShowWindow(FALSE);
					}				
				}
				else
				{
					// �������� �ʿ��� ������ �䱸�� ���� �ʴ´�.
					// �ʿ��� �������� �ٽ� �˷��ش�.
/*					for( int i = 0; i < 3 ; ++i )
					{
						TCHAR messagestring[256];
						memset(messagestring, 0, sizeof(TCHAR)*256);
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NETWORKBOSS_1621), _XGameItem::GetItemName(pEnterBossRoom_Window->m_NeedItemID[i].cType, pEnterBossRoom_Window->m_NeedItemID[i].sID) );
						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}*/
					pEnterBossRoom_Window->m_OKButton->ShowWindow(FALSE);
				}

			}
			break;
		case _XDEF_BOSSROOM_ENTEROK		:	
			{
				// �����ϱ� ���� �������� �ٽ� �˻��Ѵ�.
				if( pEnterBossRoom_Window->CheckRequirement() )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_BOSSBUTTON01_WAV);
					g_NetworkKernel.SendPacket( MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S, g_ChannelTempZoneInfoIndex );
					pEnterBossRoom_Window->ResetNeedItemInfo();
					pEnterBossRoom_Window->ShowWindow(FALSE);
					g_pInventory_Window->ShowWindow(FALSE);
				}
				else
				{
					// �������� �ʿ��� ������ �䱸�� ���� �ʴ´�.
					// �ʿ��� �������� �ٽ� �˷��ش�.
					/*for( int i = 0; i < _XDEF_BOSSROOM_NEEDITEMNUMBER; ++i )
					{
						TCHAR messagestring[256];
						memset(messagestring, 0, sizeof(TCHAR)*256);
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NETWORKBOSS_1621), _XGameItem::GetItemName(pEnterBossRoom_Window->m_NeedItemID[i].cType, pEnterBossRoom_Window->m_NeedItemID[i].sID) );
						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}*/
					pEnterBossRoom_Window->m_OKButton->ShowWindow(FALSE);
				}
			}
			break;
		case _XDEF_BOSSROOM_CANCLE		:
			{
				if( g_pLocalUser->m_InParty )
				{
					if( g_pLocalUser->m_bWaitBossBattle )
					{
						g_NetworkKernel.SendPacket( MSG_NO_PARTY_ETC, 10 ); //����� ������� ��� 
					}				
				}

#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
#else
				for( int i = 0; i < _XDEF_BOSSROOM_NEEDITEMNUMBER; i++ )
				{					
					if( pEnterBossRoom_Window->m_NeedItem[i].Get_m_sID() != 0 )
					{
						int inventorynumber = pEnterBossRoom_Window->m_NeedItem[i].m_InventoryNumber;
						if( pEnterBossRoom_Window->m_NeedItem[i].IsStackable() )
						{
							if(g_pLocalUser->m_UserItemList[inventorynumber].Get_m_sID() == 0)
							{
								pEnterBossRoom_Window->CopySlotToInventory( i, inventorynumber );																
							}
							else
							{
								g_pLocalUser->m_UserItemList[inventorynumber].m_ucCount += 1;								
							}							
						}
						else
						{
							if(g_pLocalUser->m_UserItemList[inventorynumber].Get_m_sID() == 0)
							{
								pEnterBossRoom_Window->CopySlotToInventory( i, inventorynumber );								
							}
						}
					}
				}
				pEnterBossRoom_Window->ResetNeedItemInfo();
#endif
				
				pEnterBossRoom_Window->ShowWindow(FALSE);
				g_pInventory_Window->ShowWindow(FALSE);
				m_LockMoveProcess = FALSE;
			}
			break;
		case _XDEF_BOSSROOM_ASKPARTYMEMBER :
			{
				//Author :  ����� //breif : �ῡ �� �� �޽��� ǥ��
				#if defined(_XTAIWANESE)
				if( g_pQuest_Window->m_GetTotalQuestCount >= 5 ) 
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3790), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //����Ʈ ����� ���� �� ������ ��ȣ ȹ���� �Ұ��� �մϴ�.
				#endif

				if(!g_pLocalUser->m_InParty)
				{
					pEnterBossRoom_Window->m_OKButton->ShowWindow(TRUE);
					_XPlayInterfaceSound(ID_SR_INTERFACE_BOSSBUTTON00_WAV);
				}
				else
				{
					if(g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader)	// ��� �϶�
					{
						pEnterBossRoom_Window->m_bSendPartyRequest = TRUE;							
						g_pLocalUser->m_bWaitBossBattle = TRUE;

						g_NetworkKernel.SendPacket( MSG_NO_PARTY_ETC, 8 );
						
						TCHAR messagestring[256];
						memset( messagestring, 0, sizeof(TCHAR)*256 );
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_ENTERBOSSROOM_SENDMSG)); // ����鿡�� ����ῡ ���ڴ� �������� ���½��ϴ�.

						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
					}
				}
			}
			break;
		case _XDEF_BOSSROOM_USEITEMCANCEL :
			{
				pEnterBossRoom_Window->ResetItemInSlot(0);
			}
			break;
		}
	}
}

void XProc_MainGame::NickNameMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_NickName* pNickName_Window = (_XWindow_NickName*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NICKNAME);
	
	if( pNickName_Window )
	{
		switch(wparam) 
		{		
		case _XDEF_NICKNAME_APPLY	:	
			{
				if(g_pLocalUser->m_nWinnerNickName == pNickName_Window->m_SelectedNickNameSlotNumber-10000)
				{
					g_NetworkKernel.SendPacket( MSG_NO_NIC_SELECT_REQ_C_S, -2);	//�� ��ȣ ����
				}
				else
				{
					if( g_pLocalUser->m_CharacterInfo.m_NickNameList[pNickName_Window->m_SelectedNickNameSlotNumber] != 0 )
					{
						g_NetworkKernel.SendPacket( MSG_NO_NIC_SELECT_REQ_C_S, pNickName_Window->m_SelectedNickNameSlotNumber );
					}
				}
			}
			break;
		case _XDEF_NICKNAME_NOTAPPLY		:	
			{
				g_NetworkKernel.SendPacket( MSG_NO_NIC_SELECT_REQ_C_S, 0 );
			}
			break;
		case _XDEF_NICKNAME_SPECIALNICKNAMELIST :
			{				
				pNickName_Window->m_SelectedNickNameSlotNumber = (int)lparam;
				pNickName_Window->m_listboxGeneralNickName->SelectItem( -1 );
				pNickName_Window->m_listboxUniqueNickName->SelectItem( -1 );
				pNickName_Window->SetSelectedNickNameInfo();
			}
			break;
		case _XDEF_NICKNAME_GENERALNICKNAMEIST :
			{				
				pNickName_Window->m_SelectedNickNameSlotNumber = (int)lparam;
				pNickName_Window->m_listboxSpecialNickName->SelectItem( -1 );
				pNickName_Window->m_listboxUniqueNickName->SelectItem( -1 );
				pNickName_Window->SetSelectedNickNameInfo();
			}
			break;
		case _XDEF_NICKNAME_UNIQUENICKNAMEIST :
			{				
				pNickName_Window->m_SelectedNickNameSlotNumber = -1;
				pNickName_Window->m_listboxSpecialNickName->SelectItem( -1 );
				pNickName_Window->m_listboxGeneralNickName->SelectItem( -1 );
				pNickName_Window->SetSelectedNickNameInfo();
			}
			break;
		case  _XDEF_NICKNAME_CLOSE :
			{
				pNickName_Window->ShowWindow(FALSE);
			}
			break;
		case _XDEF_NICKNAME_HELP :
			{
				if( m_pHedekWindowPtr )
					m_pHedekWindowPtr->SetHedekType( _XHEDEK_NICK );
			}
			break;
		case _XDEF_NICKNAME_DELETE :
			{
				if( pNickName_Window )
				{
					pNickName_Window->CheckDeleteNickname();
				}
			}
			break;
		case _XDEF_NICKNAME_DELETE_YES :
			{
				g_NetworkKernel.SendPacket( MSG_NO_NIC_DELETE_REQ_C_S, pNickName_Window->m_SelectedNickNameSlotNumber );
			}
			break;
		case _XDEF_NICKNAME_RANKHIDING : 
			{
			/*	if( pNickName_Window->m_RankHidingButton->GetCheck() )
					g_NetworkKernel.SendPacket( MSG_NO_RANK_HIDING, 1 );
				else
					g_NetworkKernel.SendPacket( MSG_NO_RANK_HIDING, 0 );*/
			}
			break;
		}
	}
}

void XProc_MainGame::MatchWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_PVPBROKER_CLOSE :
		{
			if(m_pPVPBrokerWindowPtr)
			{
				if(m_pPVPBrokerWindowPtr->GetShowStatus())
					m_pPVPBrokerWindowPtr->ShowWindow(FALSE);
			}
		}
		break;
	case _XDEF_PVPBROKER_LISTBOX :
		{
			g_pLocalUser->m_MatchTargetID = (unsigned short)lparam;
		}
		break;
	case _XDEF_PVPBROKER_REQUESTPVP :
		{
			if(g_pLocalUser->m_MatchTargetID > 0)
			{
				if(g_pLocalUser->m_MatchTargetID == g_pLocalUser->m_Uniqueid)
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1914), TRUE, 0);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				else
				{
					// ����ó���ϱ�
					bool pvpcancle = false;
					if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
						(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
					{
						pvpcancle = true;
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1915), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					else if( g_pLocalUser->m_CharacterLog.isTrade )
					{
						pvpcancle = true;
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1916), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
					{
						pvpcancle = true;
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1917), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					else if( g_pLocalUser->m_bReadingBook )
					{
						pvpcancle = true;
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1918), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					else if( g_pLocalUser->m_bInternalUse )
					{
						pvpcancle = true;
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1919), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					else if( g_pLocalUser->m_bSocketPlugIn )
					{
						pvpcancle = true;
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1920), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					else if( g_pLocalUser->m_bGambleBoxOpen )
					{
						pvpcancle = true;
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1921), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
					{
						pvpcancle = true;
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1922), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					else if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_REQUEST)
					{
						pvpcancle = true;
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1923), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}

					if(!pvpcancle)
					{
						if(!g_DenyMatch)
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1924), TRUE, _XDEF_MATCH_REQUESTPVP, 0);
						}
						else
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1925), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
						}
					}
				}
			}
			else
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1926), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
		}
		break;

	case _XDEF_MATCH_REGISTRATION :
		{
			if(!g_DenyMatch)
			{
				g_NetworkKernel.SendPacket(MSG_NO_MATCH_LIST, en_match_list_mode_register);
			}
			else
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1927), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
		}
		break;
	case _XDEF_MATCH_REQUESTPVP :
		{
			// ���� �� ��û
			// ����ó���ϱ�
			bool pvpcancle = false;
			if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1915), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_CharacterLog.isTrade )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1916), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1917), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_bReadingBook )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1918), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_bInternalUse )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1919), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_bSocketPlugIn )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1920), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_bGambleBoxOpen )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1921), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1922), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_REQUEST)
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1923), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_WAITING)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1928), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				break;
			}

			if(!pvpcancle)
			{
				if(!g_DenyMatch)
				{
					g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_challenge_to_other, en_matchtype_pvp);
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_CLOSE, 0), 0);

					g_pLocalUser->m_UserState = _XDEF_USERSTATE_MATCH_REQUEST;
					g_pLocalUser->m_MatchStateChangeTime = g_LocalSystemTime;
				}
				else
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1925), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				}
			}

			_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
			if(pRequestParty_Window)
			{
				if(pRequestParty_Window->GetShowStatus())
					pRequestParty_Window->ShowWindow(FALSE);
			}
		}
		break;
	case _XDEF_MATCH_REQUESTPVPGROUP :
		{
			// ���� �� ��û

			// ����ó���ϱ�
			bool pvpcancle = false;
			if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1915), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			else if( g_pLocalUser->m_CharacterLog.isTrade )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1916), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1917), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			else if( g_pLocalUser->m_bReadingBook )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1918), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			else if( g_pLocalUser->m_bInternalUse )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1919), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			else if( g_pLocalUser->m_bSocketPlugIn )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1920), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			else if( g_pLocalUser->m_bGambleBoxOpen )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1921), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1922), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			else if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_REQUEST)
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1923), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			else if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_WAITING)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1928), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				break;
			}

			if(!pvpcancle)
			{
				if(!g_DenyMatch)
				{
					g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_challenge_to_other, en_matchtype_group_deathmatch);

					g_pLocalUser->m_UserState = _XDEF_USERSTATE_MATCH_REQUEST;
					g_pLocalUser->m_MatchStateChangeTime = g_LocalSystemTime;
				}
				else
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1925), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				}
			}

			_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
			if(pRequestParty_Window)
			{
				if(pRequestParty_Window->GetShowStatus())
					pRequestParty_Window->ShowWindow(FALSE);
			}
		}
		break;
	case _XDEF_MATCH_REQUESTLIST :
		{
			// �� ��� ����Ʈ ��û
			g_NetworkKernel.SendPacket(MSG_NO_MATCH_LIST, en_match_list_mode_req_list);
		}
		break;
	case _XDEF_MATCH_REQUESTFROMOTHER_OK :
		{
			// �� ��û �޾Ƶ���
			g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_reply, 0);
		}
		break;
	case _XDEF_MATCH_REQUESTFROMOTHER_NO :
		{
			// �� ��û ����
			g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_reply, 1);
			g_pLocalUser->ResetMatchState();
		}
		break;
	case _XDEF_MATCH_WAITEXIT :
		{
			// ���忡�� ������ ��û
			g_NetworkKernel.SendPacket(MSG_NO_MATCH_CTRL, en_match_ctrl_request_exit);
		}
		break;
	case _XDEF_MATCH_REGISTCANCEL :
		{
			// �� ��� ����
			g_NetworkKernel.SendPacket(MSG_NO_MATCH_LIST, en_match_list_mode_delete);
		}
		break;
	case _XDEF_MATCHFREE_ENTER :
		{
			// ���� �������� ����
			// ����ó���ϱ�
			bool pvpcancle = false;
			if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1929), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_CharacterLog.isTrade )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1930), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1931), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_bReadingBook )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1932), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_bInternalUse )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1933), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_bSocketPlugIn )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1934), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_bGambleBoxOpen )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1935), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
			{
				pvpcancle = true;
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1936), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			
			if(!pvpcancle)
			{
				g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_free_request_match);	// ���� �� ��û
			}
		}
		break;
	}
}

void XProc_MainGame::MatchTablePVPWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_MatchTablePVP* pMatchTablePVP_Window = (_XWindow_MatchTablePVP*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHTABLEPVP);
	if(pMatchTablePVP_Window)
	{
		switch(wparam)
		{
		case _XDEF_MATCHTABLEPVP_CLOSE :
			{
				pMatchTablePVP_Window->ShowWindow(FALSE);
				g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_cancel_all); //Author : ����� //breif : �߰���
			}
			break;
		case _XDEF_MATCHTABLEPVP_MONEY :
			{
				// ���ø���
			}
			break;
		case _XDEF_MATCHTABLEPVP_CONFIRM :
			{
				if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_WAITING && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_END)
				{
					// â �ݴ´�
					pMatchTablePVP_Window->ShowWindow(FALSE);
				}
				else
				{
					// send confirm packet
					g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_confirm);
					pMatchTablePVP_Window->m_btnConfirm->EnableWindow(FALSE);
				}
			}
			break;
		case _XDEF_MATCHTABLEPVP_CANCEL :
			{
				// send cancel packet
				g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_cancel_all);
			}
			break;
		case _XDEF_MATCHTABLEPVP_MATCHCLASSBTN :
			{
				if(!pMatchTablePVP_Window->m_lboxRecordType->GetShowStatus())
					pMatchTablePVP_Window->m_lboxRecordType->ShowWindow(TRUE);
				else
					pMatchTablePVP_Window->m_lboxRecordType->ShowWindow(FALSE);

				//Author : ����� //breif : �ٸ��� ����
				pMatchTablePVP_Window->m_lboxMatchType->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchTime->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxUseHyperrun->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxUseBuff->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchBalance->ShowWindow(FALSE);
			}
			break;
		case _XDEF_MATCHTABLEPVP_MATCHTYPEBTN :
			{
				if(!pMatchTablePVP_Window->m_lboxMatchType->GetShowStatus())
					pMatchTablePVP_Window->m_lboxMatchType->ShowWindow(TRUE);
				else
					pMatchTablePVP_Window->m_lboxMatchType->ShowWindow(FALSE);

				//Author : ����� //breif : �ٸ��� ����
				pMatchTablePVP_Window->m_lboxRecordType->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchTime->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxUseHyperrun->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxUseBuff->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchBalance->ShowWindow(FALSE);
			}
			break;
		case _XDEF_MATCHTABLEPVP_MATCHTIMEBTN :
			{
				if(!pMatchTablePVP_Window->m_lboxMatchTime->GetShowStatus())
					pMatchTablePVP_Window->m_lboxMatchTime->ShowWindow(TRUE);
				else
					pMatchTablePVP_Window->m_lboxMatchTime->ShowWindow(FALSE);

				//Author : ����� //breif : �ٸ��� ����
				pMatchTablePVP_Window->m_lboxRecordType->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchType->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxUseHyperrun->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxUseBuff->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchBalance->ShowWindow(FALSE);
			}
			break;
		case _XDEF_MATCHTABLEPVP_USEHYPERRUNBTN :
			{
				if(!pMatchTablePVP_Window->m_lboxUseHyperrun->GetShowStatus())
					pMatchTablePVP_Window->m_lboxUseHyperrun->ShowWindow(TRUE);
				else
					pMatchTablePVP_Window->m_lboxUseHyperrun->ShowWindow(FALSE);

				//Author : ����� //breif : �ٸ��� ����
				pMatchTablePVP_Window->m_lboxRecordType->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchType->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchTime->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxUseBuff->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchBalance->ShowWindow(FALSE);
			}
			break;
		case _XDEF_MATCHTABLEPVP_USEBUFFBTN :
			{
				if(!pMatchTablePVP_Window->m_lboxUseBuff->GetShowStatus())
					pMatchTablePVP_Window->m_lboxUseBuff->ShowWindow(TRUE);
				else
					pMatchTablePVP_Window->m_lboxUseBuff->ShowWindow(FALSE);

				//Author : ����� //breif : �ٸ��� ����
				pMatchTablePVP_Window->m_lboxRecordType->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchType->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchTime->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxUseHyperrun->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchBalance->ShowWindow(FALSE);
			}
			break;
		case _XDEF_MATCHTABLEPVP_MATCHBALANCEBTN :
			{
				if(!pMatchTablePVP_Window->m_lboxMatchBalance->GetShowStatus())
					pMatchTablePVP_Window->m_lboxMatchBalance->ShowWindow(TRUE);
				else
					pMatchTablePVP_Window->m_lboxMatchBalance->ShowWindow(FALSE);

				//Author : ����� //breif : �ٸ��� ����
				pMatchTablePVP_Window->m_lboxRecordType->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchType->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxMatchTime->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxUseHyperrun->ShowWindow(FALSE);
				pMatchTablePVP_Window->m_lboxUseBuff->ShowWindow(FALSE);
			}
			break;
		case _XDEF_MATCHTABLEPVP_MATCHCLASSLBOX :
			{
				g_pLocalUser->m_myMatchTableInfo.cRecordType = (char)lparam-1;
				g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_update_recordtype, g_pLocalUser->m_myMatchTableInfo.cRecordType);

				strcpy(pMatchTablePVP_Window->m_strRecordType, pMatchTablePVP_Window->m_lboxRecordType->GetItemText(lparam-1, 0));
				pMatchTablePVP_Window->m_lboxRecordType->ShowWindow(FALSE);
			}
			break;
		case _XDEF_MATCHTABLEPVP_MATCHTYPELBOX :
			{
				g_pLocalUser->m_myMatchTableInfo.cMatchType = (char)lparam;
				g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_update_matchtype, g_pLocalUser->m_myMatchTableInfo.cMatchType);

				strcpy(pMatchTablePVP_Window->m_strMatchType, pMatchTablePVP_Window->m_lboxMatchType->GetItemText(lparam-1, 0));
				pMatchTablePVP_Window->m_lboxMatchType->ShowWindow(FALSE);
			}
			break;
		case _XDEF_MATCHTABLEPVP_MATCHTIMELBOX :
			{
				if(lparam == 1)
					g_pLocalUser->m_myMatchTableInfo.cMatchTime = 2;
				else
					g_pLocalUser->m_myMatchTableInfo.cMatchTime = 3;

				g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_update_matchtime, g_pLocalUser->m_myMatchTableInfo.cMatchTime);

				strcpy(pMatchTablePVP_Window->m_strMatchTime, pMatchTablePVP_Window->m_lboxMatchTime->GetItemText(lparam-1, 0));
				pMatchTablePVP_Window->m_lboxMatchTime->ShowWindow(FALSE);
			}
			break;
		case _XDEF_MATCHTABLEPVP_USEHYPERRUNLBOX :
			{
				g_pLocalUser->m_myMatchTableInfo.cUseHyperrun = (char)lparam-1;
				g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_update_disable_hyperrun, g_pLocalUser->m_myMatchTableInfo.cUseHyperrun);

				strcpy(pMatchTablePVP_Window->m_strUseHyperrun, pMatchTablePVP_Window->m_lboxUseHyperrun->GetItemText(lparam-1, 0));
				pMatchTablePVP_Window->m_lboxUseHyperrun->ShowWindow(FALSE);
			}
			break;
		case _XDEF_MATCHTABLEPVP_USEBUFFLBOX :
			{
				g_pLocalUser->m_myMatchTableInfo.cUseBuff = (char)lparam-1;
				g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_update_keep_buff, g_pLocalUser->m_myMatchTableInfo.cUseBuff);

				strcpy(pMatchTablePVP_Window->m_strUseBuff, pMatchTablePVP_Window->m_lboxUseBuff->GetItemText(lparam-1, 0));
				pMatchTablePVP_Window->m_lboxUseBuff->ShowWindow(FALSE);
			}
			break;
		case _XDEF_MATCHTABLEPVP_MATCHBALANCELBOX :
			{
				strcpy(pMatchTablePVP_Window->m_strMatchBalance, pMatchTablePVP_Window->m_lboxMatchBalance->GetItemText(lparam-1, 0));
				pMatchTablePVP_Window->m_lboxMatchBalance->ShowWindow(FALSE);
			}
			break;
		case _XDEF_MATCHTABLEPVP_MATCHBALANCESLIDER :
			{
				FLOAT percent = *((FLOAT*)&lparam) / 100.0f;
				g_pLocalUser->m_myMatchTableInfo.cMatchBalance = (char)(percent * 100);

//				g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_update_balance, g_pLocalUser->m_myMatchTableInfo.cMatchBalance);
			}
			break;
		}
	}
}

void XProc_MainGame::MatchNoticeWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_MATCHNOTICE_CLOSE :
		{
			_XWindow_MatchNoticeLarge* pMatchNoticeLarge_Window = (_XWindow_MatchNoticeLarge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICELARGE);
			if(pMatchNoticeLarge_Window)
			{
				pMatchNoticeLarge_Window->ShowWindow(FALSE);
			}
		}
		break;
	}
}

void XProc_MainGame::MatchPRWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_MatchPR* pMarthPR_Window = (_XWindow_MatchPR*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHPR);
	if(pMarthPR_Window)
	{		
		switch(wparam)
		{
		case _XDEF_MATCHPR_CLOSEBUTTON :
			{
				memset(pMarthPR_Window->m_stringPVPPR, 0, sizeof(pMarthPR_Window->m_stringPVPPR));
				memset(g_pLocalUser->m_stringMatchPR, 0, sizeof(g_pLocalUser->m_stringMatchPR));
				pMarthPR_Window->m_btnOpen->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAINGAMEMESSAGEHANDLER_1937);
				if( pMarthPR_Window->m_PVPIMEControl.GetFocus() )
				{
					pMarthPR_Window->m_PVPIMEControl.ClearBuffer();
					_XIMEKERNEL.SetFocus( NULL ); 	
				}
				pMarthPR_Window->ShowWindow(FALSE);
				g_pLocalUser->m_bMatchPRMode = FALSE;
				if( g_pLocalUser->m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_START ||
					g_pLocalUser->m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_REPEAT )
				{
					g_pLocalUser->SetAnimationIndex(_XAN_MATCH_PR_END);
				}
				
				//������ ��Ŷ������			
				g_NetworkKernel.SendPacket(MSG_NO_MATCH_NOTICE, en_match_ntc_inform_end);
			}
			break;
		case _XDEF_MATCHPR_OPENBUTTON :
			{
				if(g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1938), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					break;
				}
				if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1939), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					break;
				}
				if( g_pLocalUser->m_CharacterLog.isTrade )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);					
					g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1940), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
					_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1940) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
					break;
				}
				if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);					
					g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1941), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
					_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1941) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
					break;
				}
				if( g_pLocalUser->m_bReadingBook )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1942), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
					break;				
				}
				if( g_pLocalUser->m_bInternalUse )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1943), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
					break;
				}
				if( g_pLocalUser->m_bSocketPlugIn )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1944), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
					break;
				}
				if( g_pLocalUser->m_bGambleBoxOpen )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1945), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
					break;
				}
				if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1946), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
					break;
				}
				if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1947), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
					break;
				}
				if(g_DenyMatch)
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1948), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					break;
				}

				if( strlen(pMarthPR_Window->m_PVPIMEControl.GetText()) <= 0 )
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1949), TRUE );
					break;
				}

				// ���� �۰� ������ ��ŵ
				if( strcmp( pMarthPR_Window->m_PVPIMEControl.GetText(), pMarthPR_Window->m_stringPVPPR ) == 0 )
				{
					break;
				}

				TCHAR resultstring[128]; 
				memset( resultstring, 0, sizeof(TCHAR)*128 );
				strcpy( resultstring, pMarthPR_Window->m_PVPIMEControl.GetText());
#ifdef _XDEF_NEWVERSONFILETERING
				g_WordFilteringManager.Filtering( resultstring,_XDEF_WORDFILETERINGTYPE_CHANGE | _XDEF_WORDFILETERINGTYPE_ALWAYS );
#else
				g_WordFilteringManager.Filtering( resultstring, _XDEF_WORDFILETERINGTYPE_ALWAYS );
#endif
				
				if( strcmp( pMarthPR_Window->m_PVPIMEControl.GetText(), resultstring ) == 0 )
				{
					strcpy( pMarthPR_Window->m_stringPVPPR, resultstring );
					strcpy( g_pLocalUser->m_stringMatchPR, resultstring );
					//pMarthPR_Window->m_PVPIMEControl.ClearBuffer();
					_XIMEKERNEL.SetFocus( NULL ); 	
					pMarthPR_Window->m_btnOpen->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAINGAMEMESSAGEHANDLER_1937); 

					if( !g_pLocalUser->m_bMatchPRMode )
					{
						g_pLocalUser->m_bMatchPRMode = TRUE;
						g_pLocalUser->ResetSkillAnimation();
						g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
						g_pLocalUser->SetAnimationIndex(_XAN_MATCH_PR_START);
						g_pLocalUser->m_ModelDescriptor.SetLockEndFrame(TRUE);
						g_pLocalUser->DeleteWeaponModel();
						if(g_pLocalUser->m_CharacterInfo.gender == _XGENDER_MAN)
						{
							if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= 25 && g_pLocalUser->m_CharacterInfo.Get_innerlevel() <= 48)
								g_pLocalUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, g_FLAGMODEL[0]);
							else if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= 49 && g_pLocalUser->m_CharacterInfo.Get_innerlevel() <= 72)
								g_pLocalUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, g_FLAGMODEL[1]);
							else if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= 73)
								g_pLocalUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, g_FLAGMODEL[2]);
						}
						else
						{
							if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= 25 && g_pLocalUser->m_CharacterInfo.Get_innerlevel() <= 48)
								g_pLocalUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, g_FLAGMODEL[3]);
							else if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= 49 && g_pLocalUser->m_CharacterInfo.Get_innerlevel() <= 72)
								g_pLocalUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, g_FLAGMODEL[4]);
							else if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= 73)
								g_pLocalUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, g_FLAGMODEL[5]);
						}
					}
					
					//������ ��Ŷ������			
					g_pLocalUser->m_MatchPRTimer = g_LocalSystemTime;
					g_NetworkKernel.SendPacket(MSG_NO_MATCH_NOTICE, en_match_ntc_inform);
				}
				else 
				{
					TCHAR tstr[256];
					sprintf( tstr, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1951), g_WordFilteringManager.GetLastFilterdWord() );
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, " ", tstr, TRUE );
				}
			}
			break;
		case _XDEF_MATCHPR_HELPBUTTON :
			{
			}
			break;
		}
	}
}

void XProc_MainGame::MatchRecordWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_MatchRecord* pMatchRecord_Window = (_XWindow_MatchRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHRECORD);
	if(pMatchRecord_Window)
	{
		switch(wparam)
		{
		case _XDEF_MATCHRECORD_CLOSE :
			{
				pMatchRecord_Window->ShowWindow(FALSE);
				g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_cancel_all); //Author : ����� //breif : �߰���
			}
			break;
		case _XDEF_MATCHINFO_TAB :
			{
				_XWindow_MatchTablePVP* pMatchTablePVP_Window = (_XWindow_MatchTablePVP*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHTABLEPVP);
				if(pMatchTablePVP_Window)
				{
					if(pMatchTablePVP_Window->GetShowStatus())
						pMatchTablePVP_Window->ShowWindow(FALSE);
				}

				pMatchRecord_Window->ShowWindow(TRUE);
				pMatchRecord_Window->ChangeTab(0);
			}
			break;
		case _XDEF_MATCHTABLEPVP_TAB :
			{
				if(pMatchRecord_Window->GetShowStatus())
					pMatchRecord_Window->ShowWindow(FALSE);

				_XWindow_MatchTablePVP* pMatchTablePVP_Window = (_XWindow_MatchTablePVP*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHTABLEPVP);
				if(pMatchTablePVP_Window)
				{
					pMatchTablePVP_Window->ShowWindow(TRUE);
					pMatchTablePVP_Window->ChangeTab(1);
				}
			}
			break;
		}
	}
}

void XProc_MainGame::SocketPlugInWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
	if(pMessageBox)
	{
		if(pMessageBox->GetShowStatus())
			return;
	}
	
	switch(wparam) 
	{
	case _XDEF_SOCKETPLUGIN_APPLY				:
		{
			if( m_pSocketPlugInWindowPtr )
			{
				if( m_pSocketPlugInWindowPtr->m_bChangeButtonMode )
				{
					if( !m_pSocketPlugInWindowPtr->m_IndependentItem )
					{
						m_pSocketPlugInWindowPtr->MessagePrint(0);
					}
					else
					{
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : ����� //breif :���� ����
				_XWindow_SocketPlugIn* pSocketPlugIn_Window = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN);
				pSocketPlugIn_Window->m_bHaveMouseSlot = FALSE;
				pSocketPlugIn_Window->m_MouseSlotItem.ResetMouseSlotItem();
#endif
						g_pLocalUser->m_bSocketPlugInLock = TRUE;
						m_pSocketPlugInWindowPtr->m_MsgSocketPuluInOpen = TRUE;

						if( g_pInventory_Window->GetSocketListCount() == 0 )
						{
							m_pSocketPlugInWindowPtr->MessagePrint(0);
						}
						else
						{
							m_pSocketPlugInWindowPtr->MessagePrint(4);
						}
					}
				}
				else
				{
					g_pInventory_Window->ResetSocketIndexList();
					m_pSocketPlugInWindowPtr->ResetSocketItemList();
					m_pSocketPlugInWindowPtr->m_SocketPlugInButton->EnableWindow(FALSE);
					m_pSocketPlugInWindowPtr->m_SocketPlugInButton->ShowWindow(FALSE);					
					m_pSocketPlugInWindowPtr->RebuildListBoxText(_BOXTEXTMODE_READY);
				}
			}			
		}
		break;
	case _XDEF_SOCKETPLUGIN_CANCLE :
	case _XDEF_SOCKETPLUGIN_CLOSE :
		{
			if(m_pSocketPlugInWindowPtr)
			{
				if( m_pSocketPlugInWindowPtr->GetShowStatus() )
				{
					m_pSocketPlugInWindowPtr->MessagePrint(6);
					m_pSocketPlugInWindowPtr->m_MsgSocketPuluInOpen = TRUE;
				}
			}
		}
		break;
	case _XDEF_SOCKETPLUGIN_APPLYOK				:
		{
			if( g_pLocalUser->m_bSocketPlugIn )
			{
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : ����� //breif :���� ����
				_XWindow_SocketPlugIn* pSocketPlugIn_Window = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN);
				pSocketPlugIn_Window->m_bHaveMouseSlot = FALSE;
				pSocketPlugIn_Window->m_MouseSlotItem.ResetMouseSlotItem();
#endif

				_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
				if( pChargeGauge_Window )
				{
					if(!pChargeGauge_Window->GetShowStatus())
					{
						pChargeGauge_Window->SetDelayTime( 4500.0f );
						pChargeGauge_Window->SetGaugeMode( _XGAUGE_SOCKET );
						pChargeGauge_Window->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow(pChargeGauge_Window);					
					}
				}
				if( m_pSocketPlugInWindowPtr )
				{
					m_pSocketPlugInWindowPtr->m_MsgSocketPuluInOpen = FALSE;
					m_pSocketPlugInWindowPtr->m_SocketPlugInButton->EnableWindow(FALSE);
					m_pSocketPlugInWindowPtr->m_pApplyCancleButton->EnableWindow(FALSE);
					m_pSocketPlugInWindowPtr->m_pClosebutton->EnableWindow(FALSE);
				}
			}			
		}
		break;
	case _XDEF_SOCKETPLUGIN_APPLYCANCLE :
		{
			m_pSocketPlugInWindowPtr->m_MsgSocketPuluInOpen = FALSE;
			g_pLocalUser->m_bSocketPlugInLock = FALSE;
		}
		break;
	/*case _XDEF_SOCKETPLUGIN_CHANGEOK			:
		{
			if( m_pSocketPlugInWindowPtr )
				m_pSocketPlugInWindowPtr->ChangeIndependentItem();
		}
		break;	*/		
	case _XDEF_SOCKETPLUGIN_CLOSEOK :
		{
			if( g_pInventory_Window )
			{
				if( m_pSocketPlugInWindowPtr )
				{
					g_pLocalUser->m_bSocketPlugIn = FALSE;
					g_pLocalUser->m_bSocketPlugInLock = FALSE;
					m_pSocketPlugInWindowPtr->m_MsgSocketPuluInOpen = FALSE;
					m_pSocketPlugInWindowPtr->ShowWindow(FALSE);
					m_pSocketPlugInWindowPtr->ResetSocketItemList();					
					m_pSocketPlugInWindowPtr->m_SocketPlugInButton->EnableWindow(FALSE);
					m_pSocketPlugInWindowPtr->m_SocketPlugInButton->ShowWindow(FALSE);

					g_pInventory_Window->ResetSocketIndexList();
					if( g_pInventory_Window->GetShowStatus() )
						g_pInventory_Window->SetDefaultPosition();

					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_1609), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
				}
				_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
				if( pChargeGauge_Window )
				{
					if(pChargeGauge_Window->GetShowStatus())
						pChargeGauge_Window->SetGaugeStop();
				}
			}			
		}
		break;
	case _XDEF_SOCKETPLUGIN_CLOSECANCLE :
		{
			m_pSocketPlugInWindowPtr->m_MsgSocketPuluInOpen = FALSE;
			g_pLocalUser->m_bSocketPlugInLock = FALSE;
		}
		break;
	case _XDEF_SOCKETPLUGIN_HELP :
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_DECO );
		}
		break;
	}
}

void XProc_MainGame::SocketPlugInExpandWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_SocketPlugInExp* pSocketPlugInExpand_Window = (_XWindow_SocketPlugInExp*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGINEXP);
	if( !pSocketPlugInExpand_Window )		return;

	switch(wparam) 
	{
	case _XDEF_SOCKETPLUGINEXP_START :
		{
			pSocketPlugInExpand_Window->ClickStartButton();
		}
		break;
	case _XDEF_SOCKETPLUGINEXP_CANCLE :
		break;
	case _XDEF_SOCKETPLUGINEXP_SEPARATION :
		break;
	case _XDEF_SOCKETPLUGINEXP_DESTROY :
		break;
	case _XDEF_SOCKETPLUGINEXP_HELP :
		break;
	}
}

//2005.02.25->hotblood insert
void XProc_MainGame::PocketWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam) 
	{
	case _XDEF_POCKET_HELP:
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_POCKET );
		}
		break;
	case _XDEF_POCKET_CLOSE:
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(TCHAR)*256 );
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ALLDELETEITEM));	// "â�� ������ ��ǰ�� ��� ���� �˴ϴ�."
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_POCKET_CLOSEOK, _XDEF_POCKET_CLOSECANCLE);
		}
		break;
	case _XDEF_POCKET_CLOSEOK :
		{
			g_NetworkKernel.SendPacket( MSG_NO_TEMP_INVEN_CTRL, 0, 1 );			
		}
		break;
	}
}

void XProc_MainGame::TargetInfoWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
	if( !pMonsterStatus_Window ) return ;
	_XWindow_TargetInfo* pTargetInfo_Window = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW) ;
	if( !pTargetInfo_Window ) return ;

	switch( wparam ) 
	{	
	case _XDEF_MONSTERSTATUS_CLOSEBUTTON		:
		{			
			pMonsterStatus_Window->ShowWindow( FALSE );
			
			g_pLocalUser->ResetAttackTarget();
			g_pLocalUser->ResetViewTarget();

			_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
			if(pRequestParty_Window)
			{
				if(pRequestParty_Window->GetShowStatus())
					pRequestParty_Window->ShowWindow(FALSE);
			}
		}
		break;
	case _XDEF_MONSTERSTATUS_FUNCTIONBUTTON1	:
		{			
			if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)				
			{
				if(pMonsterStatus_Window)
				{
					// �ι� ��� �޴� ����
					_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
					if(pRequestParty_Window)
					{
						if(pRequestParty_Window->GetShowStatus())
							pRequestParty_Window->ShowWindow(FALSE);
					}
					
					_XWindow_TargetInfoSelect* pInfoSelect_Window = (_XWindow_TargetInfoSelect*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOSELECTWINNDOW);
					if( pMonsterStatus_Window->m_ShowTargetInfoPopUp && pInfoSelect_Window->GetShowStatus() ) // ���� â���� ����â�� �������� �ݴ´�
					{
						pMonsterStatus_Window->m_ShowTargetInfoPopUp = FALSE;
						pInfoSelect_Window->ShowWindow(FALSE);
					}
					else
					{
						POINT pos = pMonsterStatus_Window->GetWindowPos();
						pMonsterStatus_Window->m_ShowTargetInfoPopUp = TRUE;
						pInfoSelect_Window->MoveWindow( pos.x + 88, pos.y + 54 );
						pInfoSelect_Window->ShowWindow(TRUE);
					}
					/*if( pMonsterStatus_Window->m_CharInfoPopUpButton[0]->GetShowStatus())
					{
						for( int i = 0; i < 4; i++ )
							pMonsterStatus_Window->m_CharInfoPopUpButton[i]->ShowWindow(FALSE);
						
						pMonsterStatus_Window->m_ShowTargetInfoPopUp = FALSE;
					}
					else
					{
						POINT pos = pMonsterStatus_Window->GetWindowPos();
						for( int i = 0; i < 4; i++ )
						{
							pMonsterStatus_Window->m_CharInfoPopUpButton[i]->ShowWindow(TRUE);
							pMonsterStatus_Window->m_CharInfoPopUpButton[i]->MoveWindow(pos.x+88, pos.y+54+i*18);
						}														
						pMonsterStatus_Window->m_ShowTargetInfoPopUp = TRUE;
					}*/						
				}
			}
		}
		break;
	case _XDEF_MONSTERSTATUS_INFOPOPUPBUTTON1 :
		{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_info))
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3762), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"�� �߿��� ������ Ȯ���� �� �����ϴ�."
				break;
			}
#endif

			_XWindow_TargetInfo* targetinfo_window = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);
			if( targetinfo_window )
			{
				_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
				if(pUser)
				{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
					if(pUser->GetFreeMatchState())
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3763), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"������ �� ���̹Ƿ� ������ Ȯ���� �� �����ϴ�."
						break;
					}
					else
#endif
					{//���� �������� ���� ���� ������,, ������ ���õǾ�� Ÿ���˾��� ���������� �޽��� â���� ���Ŵ�,,
						if( pMonsterStatus_Window->m_ShowTargetInfoPopUp ) //Author : ����� //brief : Ÿ������ ���°� ���� �����̴��� â���� ������
						{
							memset( targetinfo_window->m_SendName, 0, sizeof(targetinfo_window->m_SendName) );
							strcpy( targetinfo_window->m_SendName, pUser->m_CharacterInfo.charactername );
						}
					}
				}
			}

			int nLen = strlen( targetinfo_window->m_SendName );

			if( !nLen ) //���� �̸��� ����,,
				break;
			
			g_NetworkKernel.SendPacket(MSG_NO_OBJECT_INFO_REQUEST, 2);
			//for( int i = 0; i < 4; i++ )
			//	pMonsterStatus_Window->m_CharInfoPopUpButton[i]->ShowWindow(FALSE);
			_XWindow_TargetInfoSelect* pInfoSelect_Window = (_XWindow_TargetInfoSelect*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOSELECTWINNDOW);
			pInfoSelect_Window->ShowWindow(FALSE);
			
			pMonsterStatus_Window->m_ShowTargetInfoPopUp = FALSE;
		}
		break;
	case _XDEF_MONSTERSTATUS_INFOPOPUPBUTTON2 :
		{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_info))
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3762), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"�� �߿��� ������ Ȯ���� �� �����ϴ�."
				break;
			}
#endif			
			if( g_pLocalUser->m_bTargetInfoLock )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1952), TRUE);	
				pMessageBox->StartTimer(5000, TRUE) ;
			}
			else
			{
				_XWindow_TargetInfo* targetinfo_window = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);
				if( targetinfo_window )
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
					if(pUser)
					{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
						if(pUser->GetFreeMatchState())
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3763), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"������ �� ���̹Ƿ� ������ Ȯ���� �� �����ϴ�."
							break;
						}
						else
#endif
						{
							if( pMonsterStatus_Window->m_ShowTargetInfoPopUp ) //Author : ����� //brief : Ÿ������ ���°� ���� �����̴��� â���� ������
							{
								memset( targetinfo_window->m_SendName, 0, sizeof(targetinfo_window->m_SendName) );
								strcpy( targetinfo_window->m_SendName, pUser->m_CharacterInfo.charactername );
							}
						}
					}
				}

				int nLen = strlen( targetinfo_window->m_SendName );

				if( !nLen ) //���� �̸��� ����,,
					break;
				
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_TARGETINFONMESSAGEWINDOW);
				if( pMessageBox )
				{
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_CANCEL, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1953), TRUE, _XDEF_MONSTERSTATUS_INFOPOPUPCANCLE  ); 
					pMessageBox->StartTimer(10000, TRUE);						
				}
				g_NetworkKernel.SendPacket(MSG_NO_OBJECT_INFO_REQUEST, 3);
				g_pLocalUser->m_bTargetInfoLock = TRUE;
			}			
			//for( int i = 0; i < 4; i++ )
			//	pMonsterStatus_Window->m_CharInfoPopUpButton[i]->ShowWindow(FALSE);
			
			_XWindow_TargetInfoSelect* pInfoSelect_Window = (_XWindow_TargetInfoSelect*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOSELECTWINNDOW);
			pInfoSelect_Window->ShowWindow(FALSE);
			pMonsterStatus_Window->m_ShowTargetInfoPopUp = FALSE;
		}
		break;
	case _XDEF_MONSTERSTATUS_INFOPOPUPBUTTON3 :
		{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_info))
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3762), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"�� �߿��� ������ Ȯ���� �� �����ϴ�."
				break;
			}
#endif	
			if( g_pLocalUser->m_bTargetInfoLock )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1954), TRUE);	
				pMessageBox->StartTimer(5000, TRUE) ;
			}
			else
			{
				_XWindow_TargetInfo* targetinfo_window = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);
				if( targetinfo_window )
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
					if(pUser)
					{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
						if(pUser->GetFreeMatchState())
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3763), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"������ �� ���̹Ƿ� ������ Ȯ���� �� �����ϴ�."
							break;
						}
						else
#endif
						{
							if( pMonsterStatus_Window->m_ShowTargetInfoPopUp ) //Author : ����� //brief : Ÿ������ ���°� ���� �����̴��� â���� ������
							{
								memset( targetinfo_window->m_SendName, 0, sizeof(targetinfo_window->m_SendName) );
								strcpy( targetinfo_window->m_SendName, pUser->m_CharacterInfo.charactername );
							}
						}
					}
				}

				int nLen = strlen( targetinfo_window->m_SendName );

				if( !nLen ) //���� �̸��� ����,,
					break;
			
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_TARGETINFONMESSAGEWINDOW);
				if( pMessageBox )
				{
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_CANCEL, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1955), TRUE, _XDEF_MONSTERSTATUS_INFOPOPUPCANCLE  );
					pMessageBox->StartTimer(10000, TRUE);						
				}
				g_NetworkKernel.SendPacket(MSG_NO_OBJECT_INFO_REQUEST, 4);
				g_pLocalUser->m_bTargetInfoLock = TRUE;
			}
			//for( int i = 0; i < 4; i++ )
			//	pMonsterStatus_Window->m_CharInfoPopUpButton[i]->ShowWindow(FALSE);
			
			_XWindow_TargetInfoSelect* pInfoSelect_Window = (_XWindow_TargetInfoSelect*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOSELECTWINNDOW);
			pInfoSelect_Window->ShowWindow(FALSE);
			pMonsterStatus_Window->m_ShowTargetInfoPopUp = FALSE;
		}
		break;
	case _XDEF_MONSTERSTATUS_INFOPOPUPBUTTON4 :
		{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_info))
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3762), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"�� �߿��� ������ Ȯ���� �� �����ϴ�."
				break;
			}
#endif
			
			if( g_pLocalUser->m_bTargetInfoLock )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1954), TRUE);					
				pMessageBox->StartTimer(5000, TRUE) ;
			}
			else
			{
				_XWindow_TargetInfo* targetinfo_window = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);
				if( targetinfo_window )
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
					if(pUser)
					{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
						if(pUser->GetFreeMatchState())
						{
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3763), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"������ �� ���̹Ƿ� ������ Ȯ���� �� �����ϴ�."
							break;
						}
						else
#endif
						{
							if( pMonsterStatus_Window->m_ShowTargetInfoPopUp ) //Author : ����� //brief : Ÿ������ ���°� ���� �����̴��� â���� ������
							{
								memset( targetinfo_window->m_SendName, 0, sizeof(targetinfo_window->m_SendName) );
								strcpy( targetinfo_window->m_SendName, pUser->m_CharacterInfo.charactername );
							}
						}
					}
				}

				int nLen = strlen( targetinfo_window->m_SendName );

				if( !nLen ) //���� �̸��� ����,,
					break;

				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_TARGETINFONMESSAGEWINDOW);
				if( pMessageBox )
				{
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_CANCEL, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1956), TRUE, _XDEF_MONSTERSTATUS_INFOPOPUPCANCLE  );
					pMessageBox->StartTimer(10000, TRUE);						
				}
				g_NetworkKernel.SendPacket(MSG_NO_OBJECT_INFO_REQUEST, 5);
				g_pLocalUser->m_bTargetInfoLock = TRUE;
			}
			//for( int i = 0; i < 4; i++ )
			//	pMonsterStatus_Window->m_CharInfoPopUpButton[i]->ShowWindow(FALSE);
			
			_XWindow_TargetInfoSelect* pInfoSelect_Window = (_XWindow_TargetInfoSelect*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOSELECTWINNDOW);
			pInfoSelect_Window->ShowWindow(FALSE);
			pMonsterStatus_Window->m_ShowTargetInfoPopUp = FALSE;
		}
		break;
	case _XDEF_MONSTERSTATUS_INFOPOPUPYES :
		{
			g_NetworkKernel.SendPacket(MSG_NO_OBJECT_INFO_REQUEST, pTargetInfo_Window->m_TargetInfoModefromRequest, MSG_CHARAC_MORE_INFO_REQ::_response_yes);
		}
		break;
	case _XDEF_MONSTERSTATUS_INFOPOPUPNO :
		{
			g_pLocalUser->m_bTargetInfoLock = FALSE ;
			g_NetworkKernel.SendPacket(MSG_NO_OBJECT_INFO_REQUEST, pTargetInfo_Window->m_TargetInfoModefromRequest, MSG_CHARAC_MORE_INFO_REQ::_response_no);
			pTargetInfo_Window->m_TargetInfoModefromRequest = tm_none ;
		}
		break;
	case _XDEF_MONSTERSTATUS_INFOPOPUPCANCLE :
		{
			g_pLocalUser->m_bTargetInfoLock = FALSE ;
			g_NetworkKernel.SendPacket(MSG_NO_OBJECT_INFO_REQUEST, pTargetInfo_Window->m_TargetInfoModeToRequest, MSG_CHARAC_MORE_INFO_REQ::_response_cancel);
		}
		break;
	case _XDEF_TARGETINFOWINDOW_CLOSEBUTTON :
		{
			if( pTargetInfo_Window->GetShowStatus() )
				pTargetInfo_Window->ShowWindow(FALSE) ;
		}
		break;
	case _XDEF_TARGETINFOWINDOW_HELPBUTTON :
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_PLAYERINFO );
		}
		break;
	case _XDEF_MONSTERSTATUS_FUNCTIONBUTTON2	:
		{
			if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)
			{
				_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
				if(pUser)
				{
					if(pUser->m_BattleMode != _XBATTLE_PK)
					{
						_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
						if(pRequestParty_Window)
						{
							if(pRequestParty_Window->GetShowStatus())
							{
								pRequestParty_Window->ShowWindow(FALSE);
							}
							else
							{
//								POINT pos = pMonsterStatus_Window->GetWindowPos();
//								pRequestParty_Window->MoveWindow(pos.x+84, pos.y+50);
								pRequestParty_Window->MoveWindow(pMonsterStatus_Window->GetWindowPos().x+pMonsterStatus_Window->GetWindowSize().cx-pRequestParty_Window->GetWindowSize().cx,
									pMonsterStatus_Window->GetWindowPos().y+pMonsterStatus_Window->GetWindowSize().cy+1);

								pRequestParty_Window->ShowWindow(TRUE);
								strcpy(pRequestParty_Window->m_PickedCharacterName, pUser->m_CharacterInfo.charactername);
								pRequestParty_Window->m_PickedCharacterID = pUser->m_Uniqueid;
								
								// �ι����� �޴� ���� 
								if( pMonsterStatus_Window->m_ShowTargetInfoPopUp )
								{
									_XWindow_TargetInfoSelect* pInfoSelect_Window = (_XWindow_TargetInfoSelect*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOSELECTWINNDOW);
									pInfoSelect_Window->ShowWindow(FALSE);
									pMonsterStatus_Window->m_ShowTargetInfoPopUp = FALSE;
									//pMonsterStatus_Window->m_CharInfoPopUpButton[0]->ShowWindow(FALSE);
									//pMonsterStatus_Window->m_CharInfoPopUpButton[1]->ShowWindow(FALSE);
									//pMonsterStatus_Window->m_CharInfoPopUpButton[2]->ShowWindow(FALSE);
									//pMonsterStatus_Window->m_CharInfoPopUpButton[3]->ShowWindow(FALSE);
								}	
								// ���Ա�����ư ó��
								if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass >= _XUSERORGANIZATIONINFO::_UNIT_CLASS_SPECIAL_UNIT_ )
									pRequestParty_Window->m_RequestGroupButton->EnableWindow(TRUE);
								else
									pRequestParty_Window->m_RequestGroupButton->EnableWindow(FALSE);
								
								// ��밡 ���� �����϶� ó��
								if( pUser->m_bFaceMaskMode )
									pRequestParty_Window->m_SendMessageButton->EnableWindow( FALSE );
								else
									pRequestParty_Window->m_SendMessageButton->EnableWindow( TRUE );
							}
						}
					}					
				}
			}			
		}
		break;
#ifdef _XTS_NEW_HONOR2
	case _XDEF_MONSTERSTATUS_FUNCTIONBUTTON3	:
		{
			if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)
			{
				_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
				if(pUser)
				{
					_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
					if(pMonsterStatus_Window)
					{
						if( pMonsterStatus_Window->m_PopUpButton[0]->GetShowStatus())
						{
							pMonsterStatus_Window->m_PopUpButton[0]->ShowWindow(FALSE);
							pMonsterStatus_Window->m_PopUpButton[1]->ShowWindow(FALSE);
							pMonsterStatus_Window->m_ShowPopUp = FALSE;
						}
						else
						{
							POINT pos = pMonsterStatus_Window->GetWindowPos();
							pMonsterStatus_Window->m_PopUpButton[0]->ShowWindow(TRUE);
							pMonsterStatus_Window->m_PopUpButton[0]->MoveWindow(pos.x+88, pos.y+54);
							pMonsterStatus_Window->m_PopUpButton[1]->ShowWindow(TRUE);			
							pMonsterStatus_Window->m_PopUpButton[1]->MoveWindow(pos.x+88, pos.y+72);
							pMonsterStatus_Window->m_ShowPopUp = TRUE;
						}						
					}
				}
			}

		}
		break;
	case _XDEF_MONSTERSTATUS_POPUPBUTTON1 :		// �������� �ֱ�
		{
			if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)
			{
				_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
				if(pUser)
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(TCHAR)*256 );
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1957), pUser->m_CharacterInfo.charactername);	
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_MONSTERSTATUS_POPUPBUTTON1YES, _XDEF_MONSTERSTATUS_POPUPBUTTON1NO);
				}
			}			
		}
		break;			
	case _XDEF_MONSTERSTATUS_POPUPBUTTON2 :		// �������� �ֱ�
		{
			if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)
			{
				_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
				if(pUser)
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(TCHAR)*256 );
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1958), pUser->m_CharacterInfo.charactername);	
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_MONSTERSTATUS_POPUPBUTTON2YES, _XDEF_MONSTERSTATUS_POPUPBUTTON2NO);
				}
			}			
		}
		break;
	case _XDEF_MONSTERSTATUS_POPUPBUTTON1YES :	// ���� ���� �ֱ�
		{
			g_NetworkKernel.SendPacket(MSG_NO_HONOR_PLAYER, 10);
		}
		break;	
	case _XDEF_MONSTERSTATUS_POPUPBUTTON2YES :	// ���� ���� �ֱ�
		{
			g_NetworkKernel.SendPacket(MSG_NO_HONOR_PLAYER, -10);
		}
		break;
#endif
	}
}

void XProc_MainGame::MainFrameWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
	if(!pMainFrame_Window) return;

	switch( wparam ) 
	{	
	case  _XDEF_MAINFRAME_CLOSEBUTTON			:
		{
			if( g_pDefaultTooltip_Window )
			{
				if( g_pDefaultTooltip_Window->GetShowStatus() )		// ��������
					g_pDefaultTooltip_Window->ShowWindow( FALSE );
			}

			pMainFrame_Window->ShowWindow( FALSE );
			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
			if( pPCTrade_Window && pPCTrade_Window->GetShowStatus() )
			{

			}
			else if( g_pInventory_Window->GetShowStatus() )
			{
				g_pInventory_Window->SetDefaultPosition();
			}
			
			// groupâ�� IME focus ����
			if(g_pGroup_Window)
			{
				if(g_pGroup_Window->m_NoticeIMEControl.GetFocus())
				{
					g_pGroup_Window->m_NoticeIMEControl.ClearBuffer();
					_XIMEKERNEL.SetFocus( NULL );	
					_XIMEKERNEL.ResetCombinationBuffer();
				}
			}
		}
		break;
	case  _XDEF_MAINFRAME_HELPBUTTON			:
		{
			if( m_pHedekWindowPtr )
			{
				switch( pMainFrame_Window->GetCurrentSelectedTab() ) 
				{
				case _XDEF_MAINFRAME_CHARACTORTAB :
					m_pHedekWindowPtr->SetHedekType( _XHEDEK_CINFO );
					break;
				case _XDEF_MAINFRAME_PKRECORDTAB :					
					m_pHedekWindowPtr->SetHedekType( _XHEDEK_PKTOTAL );
					break;	
				case _XDEF_MAINFRAME_MASTERYTAB :
					m_pHedekWindowPtr->SetHedekType( _XHEDEK_MASTERY );
					break;
				case _XDEF_MAINFRAME_SKILLTAB :
					m_pHedekWindowPtr->SetHedekType( _XHEDEK_SKILL );
					break;
				case _XDEF_MAINFRAME_LIFETAB :
					m_pHedekWindowPtr->SetHedekType( _XHEDEK_LIFE );
					break;
				case _XDEF_MAINFRAME_QUESTTAB :
					m_pHedekWindowPtr->SetHedekType( _XHEDEK_QUEST );
					break;
				case _XDEF_MAINFRAME_COMMUNITYTAB :
					m_pHedekWindowPtr->SetHedekType( _XHEDEK_GUILD );
					break;
				}
			}
		}
		break;
	case  _XDEF_MAINFRAME_CHARACTORTAB			:
	case  _XDEF_MAINFRAME_PKRECORDTAB			:
	case  _XDEF_MAINFRAME_MASTERYTAB			:
	case  _XDEF_MAINFRAME_SKILLTAB				:
	case  _XDEF_MAINFRAME_LIFETAB				:
	case  _XDEF_MAINFRAME_QUESTTAB				:
	case  _XDEF_MAINFRAME_COMMUNITYTAB			:
		pMainFrame_Window->ChangeWindowTab( wparam );
		break;
	}
}

void XProc_MainGame::MasterySkillWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if(!g_pMasterySkill_Window)
		return;

	switch(wparam)
	{
	case _XDEF_MASTERYSKILL_CLASS_1 :
	case _XDEF_MASTERYSKILL_CLASS_2 :
	case _XDEF_MASTERYSKILL_CLASS_3 :
	case _XDEF_MASTERYSKILL_CLASS_4 :
		{
			g_pMasterySkill_Window->ChangeTab(wparam - _XDEF_MASTERYSKILL_CLASS_1);
		}
		break;
	case _XDEF_MASTERYSKILL_APPLYOK	:
		{
			if( g_pMasterySkill_Window )
				g_pMasterySkill_Window->DecisionMasteryPoint();
		}
		break;
	case _XDEF_MASTERYSKILL_APPLYCANCLE :
		{
			if( g_pMasterySkill_Window )
				g_pMasterySkill_Window->RemoveTooltip();				
		}
		break;
	}
}

void XProc_MainGame::ContributionMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if( !m_pContributionWindowPtr )
		return;

	switch(wparam) 
	{	
	case _XDEF_CONTRIWINDOW_CLOSEBUTTON :
		{
			if(m_pContributionWindowPtr && g_pInventory_Window)
			{
				if(m_pContributionWindowPtr->GetShowStatus())
					m_pContributionWindowPtr->ShowWindow(FALSE);
				
				if(g_pInventory_Window->GetShowStatus())
					g_pInventory_Window->ShowWindow(FALSE);
			}
		}
		break;
	case _XDEF_CONTRIWINDOW_HELPBUTTON :
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_PRESENT );
		}
		break;
	case _XDEF_CONTRIWINDOW_APPLYBUTTON :
		{
			if( g_LocalSystemTime - m_pContributionWindowPtr->m_SendDelayTime > 1000 )
			{
				m_pContributionWindowPtr->m_SendDelayTime = g_LocalSystemTime;
				if( m_pContributionWindowPtr->m_ContributionIvenIndex >= 20 )
				{
					g_NetworkKernel.SendPacket( MSG_NO_NPC_TRADE_REQ, en_npc_trade_mode_contribute_moonpa );
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )
							g_pDefaultTooltip_Window->ShowWindow(FALSE);
					}
				}
				else
				{
					g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1959), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );	
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				}
			}			
		}
		break;
	case _XDEF_CONTRIWINDOW_CANCLE :
		{
			m_pContributionWindowPtr->ResetContributionData();
			g_pInventory_Window->m_ContributionItemInvenNumber = 0;
		}
		break;
	}
}

void XProc_MainGame::TutorialIconMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);	
	pTutorialIcon->RemoveTutorialIcon(wparam - _XDEF_TUTORIAL_ICON1);
}

void XProc_MainGame::HelpBookMessageHandling(WPARAM wparam, LPARAM lparam)
{
	switch( wparam ) 
	{
	case _XDEF_HELPBOOK_HELPLIST :
		{
			_XWindow_HelpBook* pHelpBookWindow = (_XWindow_HelpBook*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_HELPBOOK);	
			if( pHelpBookWindow )
			{
				pHelpBookWindow->SetHelpHtml( lparam );
			}
		}		
		break;
	case _XDEF_HELPBOOK_CLOSE	:
		{
			_XWindow_HelpBook* pHelpBookWindow = (_XWindow_HelpBook*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_HELPBOOK);	
			if( pHelpBookWindow )
				pHelpBookWindow->ShowWindow( FALSE );
		}
		break;
	case _XDEF_HELPBOOK_LEFTSTEP :
		{
			_XWindow_HelpBook* pHelpBookWindow = (_XWindow_HelpBook*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_HELPBOOK);	
			if( pHelpBookWindow )
				pHelpBookWindow->PrevPage();

		}
		break;
	case _XDEF_HELPBOOK_RIGHTSTEP :
		{
			_XWindow_HelpBook* pHelpBookWindow = (_XWindow_HelpBook*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_HELPBOOK);	
			if( pHelpBookWindow )
				pHelpBookWindow->NextPage();			
		}
		break;
	}
}


void XProc_MainGame::GMCommandMessageHandling(WPARAM wparam, LPARAM lparam)
{
#ifdef _XGMCLIENT

	//if( g_pLocalUser->m_CharacterInfo.Get_gm_level() == 0 ) return;

	_XWindow_GMCommand* pGMCommandWindow = (_XWindow_GMCommand*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMWINDOW);	
	if( !pGMCommandWindow ) return;	

	switch( wparam ) 
	{
	case _XDEF_GMWINDOW_OPENBUTTON :
		{
			if( pGMCommandWindow->GetWindowAnimationMode() == _XGMWINDOW_ANIMATIONMODE_CLOSE )
			{
				pGMCommandWindow->SetWindowAnimationMode( _XGMWINDOW_ANIMATIONMODE_OPEN );
			}
			else
			{
				pGMCommandWindow->SetWindowAnimationMode( _XGMWINDOW_ANIMATIONMODE_CLOSE );
			}
		}		
		break;
	case _XDEF_GMWINDOW_COMMANDBUTTON0	:		
		{
			_XWindow_GM_StatusControl* pWindow = (_XWindow_GM_StatusControl*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERSETTINGWINDOW);
			if( pWindow )
			{
				pWindow->ShowWindow( !pWindow->GetShowStatus() );
			}
		}
		break;
	case _XDEF_GMWINDOW_COMMANDBUTTON1	:			
		{
			_XWindow_GM_UserCoordination* pGMCoordinateControlWindow = (_XWindow_GM_UserCoordination*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERCOORDSETTINGWINDOW);
			if(pGMCoordinateControlWindow)
			{
				pGMCoordinateControlWindow->ShowWindow( !pGMCoordinateControlWindow->GetShowStatus() );
			}
		}
		break;
	case _XDEF_GMWINDOW_COMMANDBUTTON2	:
		break;
	case _XDEF_GMWINDOW_COMMANDBUTTON3	:
		break;
	case _XDEF_GMWINDOW_COMMANDBUTTON4	:
		break;
	case _XDEF_GMWINDOW_COMMANDBUTTON5	:
		{
			_XWindow_GM_SendNoticeMessage* pWindow = (_XWindow_GM_SendNoticeMessage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMNOTICEMESSAGEWINDOW);
			if( pWindow )
			{
				pWindow->ShowWindow( !pWindow->GetShowStatus() );
			}
		}
		break;
	case _XDEF_GMWINDOW_COMMANDBUTTON6	:
		{
			_XWindow_GM_MonsterManage* pWindow = (_XWindow_GM_MonsterManage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCONTROLMONSTERWINDOW);
			if( pWindow )
			{
				pWindow->ShowWindow( !pWindow->GetShowStatus() );
			}
		}
		break;
	case _XDEF_GMWINDOW_COMMANDBUTTON7	:
		{
			_XWindow_GM_EventManager* pWindow = (_XWindow_GM_EventManager*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMEVENTCONTROLWINDOW);
			if( pWindow )
			{
				pWindow->ShowWindow( !pWindow->GetShowStatus() );
			}
		}
		break;
	case _XDEF_GMWINDOW_COMMANDBUTTON8	:
		break;
	case _XDEF_GMWINDOW_COMMANDBUTTON9	:
		break;
	
	// GM Status
	case  _XDEF_GMCHARACTERSETTINGWINDOWCLOSEBTN		:
		{
			_XWindow_GM_StatusControl* pWindow = (_XWindow_GM_StatusControl*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERSETTINGWINDOW);
			if( pWindow )
			{
				pWindow->ShowWindow( !pWindow->GetShowStatus() );
			}
		}
		break;		
	case _XDEF_GMSTATUSCONTROLHIDDENBUTTON	:
		{
			_XWindow_GM_StatusControl* pWindow = (_XWindow_GM_StatusControl*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERSETTINGWINDOW);
			if( pWindow )
			{
				pWindow->ToggleHiddenStatus();
			}
		}
		break;
	case  _XDEF_GMSTATUSCONTROLCAMERASPEEDSLIDER		:
		{
			extern FLOAT g_FreeMoveModeSpeed;
			g_FreeMoveModeSpeed =  (*((FLOAT*)&lparam) / 100.0f) * 5.0f;
		}
		break;
	case  _XDEF_GMSTATUSCONTROLFOGDISTANCESLIDER		:
		{
			FLOAT fogfactor = (1.0f - (*((FLOAT*)&lparam) / 100.0f)) * 300.0f - 150.0f;
			g_EnvironmentManager.m_FogFar = g_EnvironmentManager.m_WeatherData[g_EnvironmentManager.m_Environment].FogEnd + fogfactor;
			if( g_EnvironmentManager.m_FogFar < g_EnvironmentManager.m_FogNear + 30.0f )
				g_EnvironmentManager.m_FogFar = g_EnvironmentManager.m_FogNear + 30.0f;

			gpDev->SetRenderState( D3DRS_FOGEND,		FtoDW(g_EnvironmentManager.m_FogFar) );
		}
		break;
	case  _XDEF_GMSTATUSCONTROLENVIRONMENTSLIDER		:
		{
			FLOAT envtimefactor = *((FLOAT*)&lparam);

			if( envtimefactor >= 75.0f )
			{
				g_EnvironmentManager.m_EnvironmentTime = _XDEF_ENVTIMESTARTTIME_NIGHT;
			}
			else if( envtimefactor >= 50.0f )
			{
				g_EnvironmentManager.m_EnvironmentTime = _XDEF_ENVTIMESTARTTIME_NIGHTFALL;				
			}
			else if( envtimefactor >= 25.0f )
			{
				g_EnvironmentManager.m_EnvironmentTime = _XDEF_ENVTIMESTARTTIME_DAY;				
			}
			else
			{
				g_EnvironmentManager.m_EnvironmentTime = _XDEF_ENVTIMESTARTTIME_DAWN;
			}
		}
		break;
	case  _XDEF_GMSTATUSCONTROLTIMEFLOWCHECKBUTTON		:
		{
			g_EnvTimeFlowStop = (BOOL)lparam;
		}
		break;
	case  _XDEF_GMSTATUSCONTROLFLYCAMMODE				:
		{
			g_FreeMoveMode = (BOOL)lparam;
		}
		break;
	case  _XDEF_GMSTATUSCONTROLCURSORVIEW				:
		{
			g_FreeMoveMode_CursorView = (BOOL)lparam;

			if(g_FreeMoveMode_CursorView)
			{
				::ShowCursor( TRUE );
				((CEmperorOfDragonsApp*)gApp)->SetCursor( _XMOUSETYPE_PEACE );
			}
			else
			{
				::ShowCursor( FALSE );
			}
		}
		break;

	// coord control system
	case  _XDEF_GMCHARACTERCOORDSETTINGWINDOWCLOSEBTN	:
		{
			_XWindow_GM_UserCoordination* pWindow = (_XWindow_GM_UserCoordination*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERCOORDSETTINGWINDOW);
			if( pWindow )
			{
				pWindow->ShowWindow( FALSE );
			}
		}
		break;	
	case  _XDEF_GMCHARACTERCOORDSETTINGGMMOVE			:
		{
			_XWindow_GM_UserCoordination* pWindow = (_XWindow_GM_UserCoordination*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERCOORDSETTINGWINDOW);
			if( pWindow )
			{
				pWindow->Run( _XGMCOORDCONTROLTYPE_GMMOVE );
			}		
		}
		break;
	case  _XDEF_GMCHARACTERCOORDSETTINGGMMOVETOUSER		:
		{
			_XWindow_GM_UserCoordination* pWindow = (_XWindow_GM_UserCoordination*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERCOORDSETTINGWINDOW);
			if( pWindow )
			{
				pWindow->Run( _XGMCOORDCONTROLTYPE_GMMOVETOUSER );
			}		
		}
		break;
	case  _XDEF_GMCHARACTERCOORDSETTINGMOVEUSER			:
		{
			_XWindow_GM_UserCoordination* pWindow = (_XWindow_GM_UserCoordination*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERCOORDSETTINGWINDOW);
			if( pWindow )
			{
				pWindow->Run( _XGMCOORDCONTROLTYPE_MOVEUSER );
			}
		}		
		break;
	//case  _XDEF_GMCHARACTERCOORDSETTINGWINDOWGMBTN		:
	//	break;

	case  _XDEF_GMCHARACTERINFOSETTINGWINDOWCLOSEBTN	:
		break;
	case  _XDEF_GMUSERMANAGEMENTWINDOWCLOSEBTN			:
		break;
	case  _XDEF_GMCONSULTATIONWINDOWCLOSEBTN			:
		break;
	
	// GM Notice system
	case  _XDEF_GMNOTICEMESSAGEWINDOWCLOSEBTN			:
		{
			_XWindow_GM_SendNoticeMessage* pWindow = (_XWindow_GM_SendNoticeMessage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMNOTICEMESSAGEWINDOW);
			if( pWindow )
			{
				pWindow->ShowWindow( FALSE );
			}
		}
		break;
	case  _XDEF_GMNOTICEMESSAGEINSERTBTN				:
		{
			_XWindow_GM_SendNoticeMessage* pWindow = (_XWindow_GM_SendNoticeMessage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMNOTICEMESSAGEWINDOW);
			if( pWindow )
			{
				pWindow->InsertMessage();
			}
		}
		break;
	case  _XDEF_GMNOTICEMESSAGEDELETEBTN				:
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_DELETESELECTMESSAGE), TRUE, _XDEF_GMNOTICEMESSAGECONFIRMDELETE );	// "���õ� �޼����� ���� �Ͻðڽ��ϱ� ?"
			g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
		}
		break;	
	case  _XDEF_GMNOTICEMESSAGECONFIRMDELETE			:
		{
			_XWindow_GM_SendNoticeMessage* pWindow = (_XWindow_GM_SendNoticeMessage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMNOTICEMESSAGEWINDOW);
			if( pWindow )
			{
				pWindow->DeleteMessage();
			}			
		}
		break;
	case  _XDEF_GMNOTICEMESSAGEDELETEALLBTN				:
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_DELETEALLMESSAGE), TRUE, _XDEF_GMNOTICEMESSAGECONFIRMDELETEALL );	// "��� �޼����� ���� �Ͻðڽ��ϱ� ?"
			g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
		}
		break;
	case  _XDEF_GMNOTICEMESSAGECONFIRMDELETEALL			:
		{
			_XWindow_GM_SendNoticeMessage* pWindow = (_XWindow_GM_SendNoticeMessage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMNOTICEMESSAGEWINDOW);
			if( pWindow )
			{
				pWindow->DeleteAllMessage();
			}						
		}
		break;
	case  _XDEF_GMCONTROLMONSTERWINDOWCLOSEBTN			:
		{
			_XWindow_GM_MonsterManage* pWindow = (_XWindow_GM_MonsterManage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCONTROLMONSTERWINDOW);
			if( pWindow )
			{
				pWindow->ShowWindow( !pWindow->GetShowStatus() );
			}
		}
		break;
	case _XDEF_GMMONSTERSPAWNBUTTON			:
		{
			_XWindow_GM_MonsterManage* pWindow = (_XWindow_GM_MonsterManage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCONTROLMONSTERWINDOW);
			if( pWindow )
			{
				int count = atoi( pWindow->m_SpawnCountIMEControl.GetText() );
				if( count <= 0 )
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1960), TRUE );
					g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );					
				}
				else
				{
					g_NetworkKernel.SendPacket( MSG_NO_CTRL_MONSTER_SPAWN, pWindow->m_SelectedMonsterType, count );
				}				
			}			
		}
		break;
	case _XDEF_GMMONSTERSPAWN_MOBLISTBOX	:
		{
			_XWindow_GM_MonsterManage* pWindow = (_XWindow_GM_MonsterManage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCONTROLMONSTERWINDOW);
			if( pWindow )
			{
				pWindow->m_SelectedMonsterType = (int)lparam;
			}			
		}
		break;
	case  _XDEF_GMDEBUGCOMMANDWINDOWCLOSEBTN			:
		break;
	case  _XDEF_GMCOMMANDLOGWINDOWCLOSEBTN				:
		break;

	case _XDEF_GMEVENTCONTROLWINDOWCLOSEBTN				:
		{
			_XWindow_GM_EventManager* pWindow = (_XWindow_GM_EventManager*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMEVENTCONTROLWINDOW);
			if( pWindow )
			{
				pWindow->ShowWindow( FALSE );
			}
		}
		break;
		
	case _XDEF_GMEVENTCONTROLWINDOW_BGM1				:
		{
			TCHAR messagebuffer[64];
			memset( messagebuffer, 0, sizeof(TCHAR) * 64 );
			strcpy( messagebuffer, _T("$%^entergroom") );		
			g_NetworkKernel.SendChatMessage( messagebuffer );
		}
		break;
	case _XDEF_GMEVENTCONTROLWINDOW_BGM2				:
		{
			TCHAR messagebuffer[64];
			memset( messagebuffer, 0, sizeof(TCHAR) * 64 );
			strcpy( messagebuffer, _T("$%^enterbride") );			
			g_NetworkKernel.SendChatMessage( messagebuffer );
		}
		break;
	case _XDEF_GMEVENTCONTROLWINDOW_BGM3				:
		{
			TCHAR messagebuffer[64];
			memset( messagebuffer, 0, sizeof(TCHAR) * 64 );
			strcpy( messagebuffer, _T("$%^beginmarch") );
			g_NetworkKernel.SendChatMessage( messagebuffer );
		}
		break;
	case _XDEF_GMEVENTCONTROLWINDOW_BGMSTOP				:
		{
			TCHAR messagebuffer[64];
			memset( messagebuffer, 0, sizeof(TCHAR) * 64 );
			strcpy( messagebuffer, _T("$%^enteredgroom") );
			g_NetworkKernel.SendChatMessage( messagebuffer );
		}
		break;
	case _XDEF_GMEVENTCONTROLWINDOW_FIREWORK			:
		{
			TCHAR messagebuffer[64];
			memset( messagebuffer, 0, sizeof(TCHAR) * 64 );
			strcpy( messagebuffer, _T("$%^salute") );
			g_NetworkKernel.SendChatMessage( messagebuffer );
		}
		break;
	case _XDEF_GMEVENTCONTROLWINDOW_FLOWER1				:
		{
			TCHAR messagebuffer[64];
			memset( messagebuffer, 0, sizeof(TCHAR) * 64 );
			strcpy( messagebuffer, _T("$%^flower1") );
			g_NetworkKernel.SendChatMessage( messagebuffer );
		}
		break;
	case _XDEF_GMEVENTCONTROLWINDOW_FLOWER2				:
		{
			TCHAR messagebuffer[64];
			memset( messagebuffer, 0, sizeof(TCHAR) * 64 );
			strcpy( messagebuffer, _T("$%^flower2") );
			g_NetworkKernel.SendChatMessage( messagebuffer );
		}
		break;
	case _XDEF_GMEVENTCONTROLWINDOW_FLOWER3				:
		{
			TCHAR messagebuffer[64];
			memset( messagebuffer, 0, sizeof(TCHAR) * 64 );
			strcpy( messagebuffer, _T("$%^flower3") );
			g_NetworkKernel.SendChatMessage( messagebuffer );
		}
		break;
	case _XDEF_GMEVENTCONTROLWINDOW_WEDDINGSTAGE		:
		{
			TCHAR messagebuffer[64];
			memset( messagebuffer, 0, sizeof(TCHAR) * 64 );
			strcpy( messagebuffer, _T("$%^createweddinghall") );
			g_NetworkKernel.SendChatMessage( messagebuffer );
		}
		break;
	case _XDEF_GMEVENTCONTROLWINDOW_WEDDINGSTAGEEND		:
		{
			TCHAR messagebuffer[64];
			memset( messagebuffer, 0, sizeof(TCHAR) * 64 );
			strcpy( messagebuffer, _T("$%^deleteweddinghall") );
			g_NetworkKernel.SendChatMessage( messagebuffer );
		}
		break;
	case _XDEF_GMEVENTCONTROLWINDOW_ENVDAY				:
		{
			TCHAR messagebuffer[64];
			memset( messagebuffer, 0, sizeof(TCHAR) * 64 );
			strcpy( messagebuffer, _T("$%^day") );
			g_NetworkKernel.SendChatMessage( messagebuffer );
		}
		break;
	case _XDEF_GMEVENTCONTROLWINDOW_NIGHTFALL			:
		{
			TCHAR messagebuffer[64];
			memset( messagebuffer, 0, sizeof(TCHAR) * 64 );
			strcpy( messagebuffer, _T("$%^nightfall") );
			g_NetworkKernel.SendChatMessage( messagebuffer );
		}
		break;
	case _XDEF_GMEVENTCONTROLWINDOW_NIGHT				:
		{
			TCHAR messagebuffer[64];
			memset( messagebuffer, 0, sizeof(TCHAR) * 64 );
			strcpy( messagebuffer, _T("$%^night") );
			g_NetworkKernel.SendChatMessage( messagebuffer );
		}
		break;
	}	

#endif

}

void XProc_MainGame::OrganizationWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if( g_pGroup_Window )
		g_pGroup_Window->ClosePopUpMenu();

	switch( wparam )
	{
		case _XDEF_GROUP_ORGANIZATION				:	
			{
				g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS1);
			}
			break;
		case _XDEF_GROUP_ALIENCE_ALIENCETAB		:	
			{
#ifdef _XTS_ALIANCE_20061018
				g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS2);
#endif
			}
			break;
		case _XDEF_GROUP_ALIENCE_ALIENCEDESTROY:// ������ ���� Ż�� ��ư 
			{
#ifdef _XTS_ALIANCE_20061018
				//�������̸� �ػ� 
				// ���������̸� Ż��	
				//�ػ��û
				if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_ )
				{
					g_NetworkKernel.SendPacket(MSG_NO_ALLY_BASE, en_ally_mode_req_scatter);
				}
				else if(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_ )
				{
					g_NetworkKernel.SendPacket(MSG_NO_ALLY_BASE, en_ally_mode_req_leave);
				}
#endif
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_LEAVEBTN		:		// Ż��
			{
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1961), TRUE, _XDEF_GROUP_ORGANIZATION_LEAVE_OK);
				}				
			}
			break;
		case _XDEF_GROUP_ALIENCE_BTN_ALIENCEMEMBER1	:	
		case _XDEF_GROUP_ALIENCE_BTN_ALIENCEMEMBER2	:
		case _XDEF_GROUP_ALIENCE_BTN_ALIENCEMEMBER3	:	
		case _XDEF_GROUP_ALIENCE_BTN_ALIENCEMEMBER4	:
		case _XDEF_GROUP_ALIENCE_BTN_ALIENCEMEMBER5	:
			break;
		case _XDEF_GROUP_ORGANIZATION_LEAVE_OK :
			{
				if( g_pGroup_Window )
					g_pGroup_Window->LeaveOrganization();
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_LINELEVEL		:
			{
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
				{
					if( g_pGroup_Window )
						g_pGroup_Window->LineUpLevel();
				}				
			}			
			break;
		case _XDEF_GROUP_ORGANIZATION_LINEZONE		:
			{
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
				{
					if( g_pGroup_Window )
						g_pGroup_Window->LineUpZone();
				}				
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_LINECONNECT	:
			{
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
				{
					if( g_pGroup_Window )
						g_pGroup_Window->LineUpOnline();
				}				
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_LISTBOX		:
			{				
				if( g_pGroup_Window )
					g_pGroup_Window->ProcessPopUp();
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_CREATEYES :
			{
				if( m_pNPCScriptWindowPtr )
				{
					m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_ORGANIZATION);
					m_pNPCScriptWindowPtr->SetTitle( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CREATEORGANIZATION) );	// _T("������ â��")
				}
			}			
			break;
		case _XDEF_GROUP_ORGANIZATION_CREATENO :
			break;
		case _XDEF_GROUP_ORGANIZATION_NAMECOMPLETE :
			{				
				if( m_pNPCScriptWindowPtr )
				{
					TCHAR zozicname[9];
					strcpy( zozicname, m_pNPCScriptWindowPtr->m_OrganizationNameIME.GetText() );
					if( strlen(zozicname) <= 0 )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_EMPTYNAME), TRUE);	// _T("�������� �̸��� �������� �ʾҽ��ϴ�")
						pMessageBox->StartTimer(5000, TRUE) ;
					}
					else if( strlen(zozicname) <= 1 )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_SHORTNAME), TRUE);		// _T("�������� �̸��� �ʹ� ª���ϴ�. 1�� �̻����� �Է��ϼ���.")
						pMessageBox->StartTimer(5000, TRUE) ;
					}
					else
					{
						TCHAR warnstring[512];
						strcpy(warnstring, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_FILTERNAME));

						wchar_t invalidcharacter = 0;
						_XCREATECHARACTERRESULT namestringcheckresult;
								
						if(g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
						   g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
						{
							namestringcheckresult = _XCheckUsableCharacterName_Unicode( m_pNPCScriptWindowPtr->m_OrganizationNameIME.GetUnicodeText(), invalidcharacter );
						}
						else
						{
							namestringcheckresult = _XCheckUsableCharacterName( m_pNPCScriptWindowPtr->m_OrganizationNameIME.GetText() );
						}
												
						switch( namestringcheckresult ) 
						{
						case _XCREATECHARACTERRESULT_STRINGISNULL :
							{
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_MEMORYERROR), TRUE );	// _T("�������� �̸��� �ʹ� ª�ų� �޸� �Ҵ翡 �����߽��ϴ�.")								
								m_pNPCScriptWindowPtr->m_OrganizationNameIME.SetFocus();
							}
							break;
						case _XCREATECHARACTERRESULT_LENGTHISZERO :
							{
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_VERYSHORTNAME), TRUE );	// _T("�������� �̸��� �ʹ� ª���ϴ�.")								
								m_pNPCScriptWindowPtr->m_OrganizationNameIME.SetFocus();
							}
							break;
						case _XCREATECHARACTERRESULT_INCLUDESPACE :
							{
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_BLANKNAME), TRUE ); // �������� �̸��� ������ ����Ͻ� �� �����ϴ�.								
								m_pNPCScriptWindowPtr->m_OrganizationNameIME.SetFocus();
							}
							break;
						case _XCREATECHARACTERRESULT_INVALIDCHARACTER :
							{
								TCHAR message[512];
								memset( message, 0x00, 512 );
								sprintf( message, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_SPECIALCHAR), g_LastErrorCheckedCharacterName );	// "[%s]��(��) ���� Ȯ�� �ѱ�, ����, Ư�����ڴ� ����Ͻ� �� �����ϴ�."
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );								
								m_pNPCScriptWindowPtr->m_OrganizationNameIME.SetFocus();
							}
							break;
						case _XCREATECHARACTERRESULT_SINGLEKOREANCHARACTER :
							{
								TCHAR message[512];
								memset( message, 0x00, 512 );
								sprintf( message, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ERRORCHAR), g_LastErrorCheckedCharacterName );	// "[%s]��(��) ���� �ѱ� ���ڴ� ����Ͻ� �� �����ϴ�."
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );								
								m_pNPCScriptWindowPtr->m_OrganizationNameIME.SetFocus();
							}
							break;
						case _XCREATECHARACTERRESULT_NOVALIDASCIICHARACTER :
							{
								TCHAR message[512];
								memset( message, 0x00, 512 );

								if(g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE)
								{
									TCHAR tempstring[10];
									memset( tempstring, 0, sizeof(TCHAR) * 10 );
									itoa( invalidcharacter, tempstring, 16 );
									TCHAR tempstring2[10];
									memset( tempstring2, 0, sizeof(TCHAR) * 10 );
									strcpy( tempstring2, _T("0X") );
									strcat( tempstring2, tempstring );

									sprintf( message, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ERRORASCII), tempstring2 );	// "[%s]��(��) ���� ASCII ���ڴ� ����Ͻ� �� �����ϴ�. (��ȣ���ڴ� -,_,[,]�� �����մϴ�)"
									g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );								
									m_pNPCScriptWindowPtr->m_OrganizationNameIME.SetFocus();
								}
								else
								{
									sprintf( message, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ERRORASCII), g_LastErrorCheckedCharacterName );	// "[%s]��(��) ���� ASCII ���ڴ� ����Ͻ� �� �����ϴ�. (��ȣ���ڴ� -,_,[,]�� �����մϴ�)"
									g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );								
									m_pNPCScriptWindowPtr->m_OrganizationNameIME.SetFocus();
								}
							}
							break;
						case _XCREATECHARACTERRESULT_NOERROR :
							{
								TCHAR resultstring[256]; 
								memset( resultstring, 0x00, 256 );
								strcpy( resultstring, m_pNPCScriptWindowPtr->m_OrganizationNameIME.GetText() );
								g_WordFilteringManager.Filtering( resultstring, _XDEF_WORDFILETERINGTYPE_GUILD | _XDEF_WORDFILETERINGTYPE_ALWAYS, TRUE );
								
								// ���͸� ���
								if( strcmpi( resultstring, m_pNPCScriptWindowPtr->m_OrganizationNameIME.GetText() ) == 0 )
								{
									m_pNPCScriptWindowPtr->m_ScriptButton[0]->ShowWindow(TRUE);
									m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
									PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_NEXTSCRIPT, 0), (LPARAM)2);
									g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_CREATE_REQ);
								}				
								else
								{
									TCHAR message[512];
									memset( message, 0x00, 512 );
									sprintf( message, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_FILTER), g_WordFilteringManager.GetLastFilterdWord() );	// "%s(��)�� ����Ͻ� �� ���� �ܾ��Դϴ�."
									g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );									
									m_pNPCScriptWindowPtr->m_OrganizationNameIME.SetFocus();
								}
							}
							break;
						}										
					}
				}
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_NAMECANCLE :		// �̸� �Է� ���
			{
				if( m_pNPCScriptWindowPtr )
				{
					m_pNPCScriptWindowPtr->m_ScriptButton[0]->ShowWindow(TRUE);
					m_pNPCScriptWindowPtr->m_OrganizationNameIME.ClearBuffer();
					m_pNPCScriptWindowPtr->m_OrganizationNameIME.ShowWindow(FALSE);
					_XIMEKERNEL.SetFocus( NULL );	
					_XIMEKERNEL.ResetCombinationBuffer();						
					m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_NEXTSCRIPT, 0), (LPARAM)2);
				}
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_FRIEND		:
			{
				_XWindow_RequestParty* ptrackpopupmenu = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REQUESTPARTY );
				if( ptrackpopupmenu && g_pGroup_Window )
				{
					strcpy(ptrackpopupmenu->m_PickedCharacterName, g_pGroup_Window->GetSendTargetName() );
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_REQUESTPARTY_REQUESTFRIEND, 0), (LPARAM)gHWnd);
				}								
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_EMAIL			:
			{
				_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
				if( pEmail_Window && g_pGroup_Window ) 
				{
					pEmail_Window->ShowWindow(TRUE);
					pEmail_Window->ChangeTab( _XEMAILTAB_SEND );
					if( g_pInventory_Window->GetShowStatus() ) g_pInventory_Window->ShowWindow(FALSE);					
					strcpy(	pEmail_Window->m_EmailReceiver, g_pGroup_Window->GetSendTargetName() );
					pEmail_Window->m_EmailIMEControl.SetFocus();

					_XWindow_MainMenu* pMainmenuwindow = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
					if( pMainmenuwindow )
						g_MainWindowManager.SetTopWindow(pMainmenuwindow);
				}
			}			
			break;
		case _XDEF_GROUP_ORGANIZATION_WHISPER		:
			{
                //Author : ����� //breif : �����ؼ� �ӼӸ� �߸� ������ ����
                _XWindow_RequestParty* ptrackpopupmenu = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REQUESTPARTY );
                memset(ptrackpopupmenu->m_PickedCharacterName, 0, sizeof(ptrackpopupmenu->m_PickedCharacterName) );
                strcpy(ptrackpopupmenu->m_PickedCharacterName, g_pGroup_Window->GetSendTargetName() );

				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_REQUESTPARTY_SENDMESSAGE, 0), (LPARAM)gHWnd);
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_PARTY			:
			{
				_XWindow_RequestParty* ptrackpopupmenu = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REQUESTPARTY );
				if( ptrackpopupmenu && g_pGroup_Window )
				{
					strcpy(ptrackpopupmenu->m_PickedCharacterName, g_pGroup_Window->GetSendTargetName() );
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_REQUESTPARTY_REQUESTPARTY, 0), (LPARAM)gHWnd);
				}
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_GRANT			:	// ���� �ο�
			{
				if( g_pGroup_Window )
					g_pGroup_Window->GrantUnit();				
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_GRANTCANCLE	:	// �ο� ���
			{
				if( g_pGroup_Window )
					g_pGroup_Window->GrantCancleUnit();
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_TRUST			:	// ����
			{
				if( g_pGroup_Window )
					g_pGroup_Window->TrustUnit();				
			}
			break;	
		case _XDEF_GROUP_ORGANIZATION_PRUGE			:	// �߹�
			{
				if( g_pGroup_Window )
					g_pGroup_Window->PrugeUnit();
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_INVITE_OK		:
			{
				if( g_pGroup_Window )
				{
					g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_CLIENT, MSG_ORGANIZATION_CLIENT_::_OR_REQ_INVITE_OK_);
				}
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_INVITE_NO		:
			{
				if( g_pGroup_Window )
				{
					g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_CLIENT, MSG_ORGANIZATION_CLIENT_::_OR_REQ_INVITE_NO_);
				}
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_ENTRUST_OK	:
			if( g_pGroup_Window )
			{
				g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_CLIENT, MSG_ORGANIZATION_CLIENT_::_OR_REQ_ENTRUST_OK_);
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_ENTRUST_NO	:
			if( g_pGroup_Window )
			{
				g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_CLIENT, MSG_ORGANIZATION_CLIENT_::_OR_REQ_ENTRUST_NO_);
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_SORTBTN		:
			{
				if( g_pGroup_Window )								
					g_pGroup_Window->ClickSortButton();
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_SORTLIST :
			{
				if( g_pGroup_Window )
					g_pGroup_Window->SelectSortType();
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_NOTICEWRITE	:
			{
				if( g_pGroup_Window )
				{
					if( g_pGroup_Window->m_NoticeMode == _XNOTICEMODE_READ )
						g_pGroup_Window->SelectNoticeMode(_XNOTICEMODE_WRITE);
					else
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1962), TRUE, 
							_XDEF_GROUP_ORGANIZATION_WRITEFINISH, _XDEF_GROUP_ORGANIZATION_WRITECANCLE);
					}
				}
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_NOTICEMODIFY	:
			{
				if( g_pGroup_Window )
				{
					if( g_pGroup_Window->m_NoticeMode == _XNOTICEMODE_READ )
						g_pGroup_Window->SelectNoticeMode(_XNOTICEMODE_MODIFY);
					else
					{
						if( g_pGroup_Window->m_NoticeMode != _XNOTICEMODE_READ )
							g_pGroup_Window->SelectNoticeMode(_XNOTICEMODE_READ);					
					}
				}
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_WRITEFINISH :
			{
				if( g_pGroup_Window )	
					g_pGroup_Window->CopySendNoticeText();
			}
			break;
		case _XDEF_GROUP_ORGANIZATION_WRITECANCLE :
			{
				if( g_pGroup_Window )					
				{
					if( g_pGroup_Window->m_NoticeMode != _XNOTICEMODE_READ )
						g_pGroup_Window->SelectNoticeMode(_XNOTICEMODE_READ);
				}
			}
			break;
			
#ifdef _XTS_ALIANCE_20061018
		case _XDEF_GROUP_ALIENCE_BTN_INVITE_OK		:
			{
				g_NetworkKernel.SendPacket(MSG_NO_ALLY_BASE, en_ally_mode_response_yes);
			}
			break;
		case _XDEF_GROUP_ALIENCE_BTN_INVITE_NO		:
			{
				g_NetworkKernel.SendPacket(MSG_NO_ALLY_BASE, en_ally_mode_response_no);
			}
			break;
#endif
			
#ifdef _XDEF_CASTLEBATTLE
		case _XDEF_GROUP_CASTLEBATTLE_CASTLEBATTLETAB:
			{				
				g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS3);
			}
			break;
		case _XDEF_GROUP_CASTLEBATTLE_BTN_TIMECHANGE:
			{
				_XWindow_CastleBattleTimeChange* pCBTimechange_Window = (_XWindow_CastleBattleTimeChange*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE);
				if(pCBTimechange_Window)
				{
					pCBTimechange_Window->ShowWindow(TRUE);
				}
			}
			break;
		case _XDEF_GROUP_CASTLEBATTLE_BTN_GETMONEY	:
			{
				g_NetworkKernel.SendPacket(MSG_NO_CB_SCHEDULE, en_cb_tax_request_take);
				g_pGroup_Window->m_pBtnCastleBattleGetMoney->EnableWindow(FALSE);
				
			}
			break;

		case _XDEF_GROUP_CASTLEBATTLE_WARSITUATIONTAB:
			{				
				g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS4);
			}
			break;
#endif
	#ifdef _XDEF_ALIANCE_WEBBOARD_20070130	
		case _XDEF_GROUP_ALIENCE_BTN_GOTOORGANIZATION	:
			{
				
				_XWindow_WebBoard* pWebBoard_Window = (_XWindow_WebBoard*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WEBBOARD );	
				if( pWebBoard_Window  )
				{
					pWebBoard_Window->ShowWindow(!pWebBoard_Window->GetShowStatus());
					
					if( pWebBoard_Window->GetShowStatus() )
					{
						
						int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
						
//						if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex <= 0 )
//						{
//						}

						TCHAR tempStr[512];
						memset( tempStr, 0, sizeof(TCHAR)*512 );
						
						if( !g_bTestWebPageAddress )
						{							
							sprintf( tempStr, "http://nine.nexon.com/community/heroboard/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s&game_hero_level1=%d&game_hero_name=%s&game_hero_level2=%d&hero_bbs=%d"
								,servernumber //������ȣ game_server=%d
								,g_pLocalUser->m_CharacterInfo.Get_innerlevel() //�ɸ��ͷ��� game_lev=%d
								,g_pLocalUser->m_CharacterInfo.gender //���� game_sex=%d
								,g_pLocalUser->m_CharacterInfo.Get_cClass() // Ŭ���� ���ķ��� ���Ŀ� �����Ǹ� �߰� munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //�������̸� game_guild=%s&
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //�����ܷ��� ���Ŀ� �����Ǹ� �߰�  game_guild_lev=%d
								,g_pLocalUser->m_CharacterInfo.groupindex //���� munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.charactername //ĳ�����̸� game_char=%s
								,g_pLocalUser->m_UserName //����ID user_game_id=%s
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass//�����ܷ��� game_hero_level1=%d (0:�ܿ� 1:�δ��� 2:����)
								,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName //����ȸ�̸� game_hero_name=%s
								,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass //����ȸ���� game_hero_level2=%d (0:�ӽ� 1:ȸ�� 2:��ȸ�� 3:ȸ��)
								,1 //�Խ��� �ڵ� �̵� ���� hero_bbs=%d (0:���ӰԽ��� 1:������ �Խ��� 2:����ȸ �Խ���)
								);
						}
						else
						{				
							sprintf( tempStr, "http://9dtest.nexon.com/community/heroboard/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s&game_hero_level1=%d&game_hero_name=%s&game_hero_level2=%d&hero_bbs=%d"
								,servernumber //������ȣ game_server=%d
								,g_pLocalUser->m_CharacterInfo.Get_innerlevel() //�ɸ��ͷ��� game_lev=%d
								,g_pLocalUser->m_CharacterInfo.gender //���� game_sex=%d
								,g_pLocalUser->m_CharacterInfo.Get_cClass() // Ŭ���� ���ķ��� ���Ŀ� �����Ǹ� �߰� munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //�������̸� game_guild=%s&
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //�����ܷ��� ���Ŀ� �����Ǹ� �߰�  game_guild_lev=%d
								,g_pLocalUser->m_CharacterInfo.groupindex //���� munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.charactername //ĳ�����̸� game_char=%s
								,g_pLocalUser->m_UserName //����ID user_game_id=%s
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass//�����ܷ��� game_hero_level1=%d (0:�ܿ� 1:�δ��� 2:����)
								,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName //����ȸ�̸� game_hero_name=%s
								,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass //����ȸ���� game_hero_level2=%d (0:�ӽ� 1:ȸ�� 2:��ȸ�� 3:ȸ��)
								,1 //�Խ��� �ڵ� �̵� ���� hero_bbs=%d (0:���ӰԽ��� 1:������ �Խ��� 2:����ȸ �Խ���)
								);
						}
						
						DisplayHTMLPage(g_hIWebBoardWebBrowserObject, tempStr);
						
						
						g_MainWindowManager.SetTopWindow( pWebBoard_Window );					
						POINT pt = { 0, 0 };
						ClientToScreen( gHWnd, &pt );
						SetWindowPos(g_hWebBoardWindow, HWND_TOPMOST, pt.x + (gnWidth>>1)-343, pt.y + (gnHeight>>1)-280, 688, 450, 0);
						
						ShowWindow(g_hWebBoardWindow, SW_SHOW);
						g_bShowItemMallWindow = TRUE;
					}
					else
					{
						ShowWindow(g_hWebBoardWindow, SW_HIDE );
						g_bShowItemMallWindow = FALSE;
					}
				}
			}
			break;
		case _XDEF_GROUP_ALIENCE_BTN_GOTOALIENCE:
			{
				
				_XWindow_WebBoard* pWebBoard_Window = (_XWindow_WebBoard*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WEBBOARD );	
				if( pWebBoard_Window  )
				{
					pWebBoard_Window->ShowWindow(!pWebBoard_Window->GetShowStatus());
					
					if( pWebBoard_Window->GetShowStatus() )
					{
						
						int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
						
						TCHAR tempStr[512];
						memset( tempStr, 0, sizeof(TCHAR)*512 );
						
						if( !g_bTestWebPageAddress )
						{
							sprintf( tempStr, "http://nine.nexon.com/community/heroboard/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s&game_hero_level1=%d&game_hero_name=%s&game_hero_level2=%d&hero_bbs=%d"
								,servernumber //������ȣ game_server=%d
								,g_pLocalUser->m_CharacterInfo.Get_innerlevel() //�ɸ��ͷ��� game_lev=%d
								,g_pLocalUser->m_CharacterInfo.gender //���� game_sex=%d
								,g_pLocalUser->m_CharacterInfo.Get_cClass() // Ŭ���� ���ķ��� ���Ŀ� �����Ǹ� �߰� munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //�������̸� game_guild=%s&
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //�����ܷ��� ���Ŀ� �����Ǹ� �߰�  game_guild_lev=%d
								,g_pLocalUser->m_CharacterInfo.groupindex //���� munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.charactername //ĳ�����̸� game_char=%s
								,g_pLocalUser->m_UserName //����ID user_game_id=%s
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass//�����ܷ��� game_hero_level1=%d (0:�ܿ� 1:�δ��� 2:����)
								,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName //����ȸ�̸� game_hero_name=%s
								,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass //����ȸ���� game_hero_level2=%d (0:�ӽ� 1:ȸ�� 2:��ȸ�� 3:ȸ��)
								,2 //�Խ��� �ڵ� �̵� ���� hero_bbs=%d (0:���ӰԽ��� 1:������ �Խ��� 2:����ȸ �Խ���)
								);
						}
						else
						{				
							sprintf( tempStr, "http://9dtest.nexon.com/community/heroboard/game_login.asp?game_server=%d&game_lev=%d&game_sex=%d&munpa_lev=%d&game_guild=%s&game_guild_lev=%d&game_munpa=%d&game_char=%s&user_game_id=%s&game_hero_level1=%d&game_hero_name=%s&game_hero_level2=%d&hero_bbs=%d"
								,servernumber //������ȣ game_server=%d
								,g_pLocalUser->m_CharacterInfo.Get_innerlevel() //�ɸ��ͷ��� game_lev=%d
								,g_pLocalUser->m_CharacterInfo.gender //���� game_sex=%d
								,g_pLocalUser->m_CharacterInfo.Get_cClass() // Ŭ���� ���ķ��� ���Ŀ� �����Ǹ� �߰� munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName //�������̸� game_guild=%s&
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass //�����ܷ��� ���Ŀ� �����Ǹ� �߰�  game_guild_lev=%d
								,g_pLocalUser->m_CharacterInfo.groupindex //���� munpa_lev=%d
								,g_pLocalUser->m_CharacterInfo.charactername //ĳ�����̸� game_char=%s
								,g_pLocalUser->m_UserName //����ID user_game_id=%s
								,g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass//�����ܷ��� game_hero_level1=%d (0:�ܿ� 1:�δ��� 2:����)
								,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName //����ȸ�̸� game_hero_name=%s
								,g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass //����ȸ���� game_hero_level2=%d (0:�ӽ� 1:ȸ�� 2:��ȸ�� 3:ȸ��)
								,2 //�Խ��� �ڵ� �̵� ���� hero_bbs=%d (0:���ӰԽ��� 1:������ �Խ��� 2:����ȸ �Խ���)
								);
						}
						
						DisplayHTMLPage(g_hIWebBoardWebBrowserObject, tempStr);
						
						
						g_MainWindowManager.SetTopWindow( pWebBoard_Window );					
						POINT pt = { 0, 0 };
						ClientToScreen( gHWnd, &pt );
						SetWindowPos(g_hWebBoardWindow, HWND_TOPMOST, pt.x + (gnWidth>>1)-343, pt.y + (gnHeight>>1)-280, 688, 450, 0);
						
						ShowWindow(g_hWebBoardWindow, SW_SHOW);
						g_bShowItemMallWindow = TRUE;
					}
					else
					{
						ShowWindow(g_hWebBoardWindow, SW_HIDE );
						g_bShowItemMallWindow = FALSE;
					}
				}
			}
			break;
	#endif
	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
		case _XDEF_GROUP_MASTERNDISCIPLETAB :
			{
				g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS5);
			}
			break;
		case _XDEF_GROUP_MASTERNDISCIPLE_NOTICEWRITE : //Author : ����� //breif : ���� �ý���
			{
				if( g_pGroup_Window )
				{
					if( g_pGroup_Window->m_pMnDWindow->m_nNoticeMode == _XMNDNOTICEMODE_READ )
						g_pGroup_Window->m_pMnDWindow->SelectNoticeMode(_XMNDNOTICEMODE_WRITE);
					else
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1962), TRUE, 
							_XDEF_GROUP_MASTERNDISCIPLE_WRITEFINISH, _XDEF_GROUP_MASTERNDISCIPLE_WRITECANCLE);
					}
				}
			}
			break;
		case _XDEF_GROUP_MASTERNDISCIPLE_NOTICEMODIFY : //Author : ����� //breif : ���� �ý���
			{
				if( g_pGroup_Window )
				{
					if( g_pGroup_Window->m_pMnDWindow->m_nNoticeMode == _XMNDNOTICEMODE_READ )
						g_pGroup_Window->m_pMnDWindow->SelectNoticeMode(_XMNDNOTICEMODE_MODIFY);
					else
					{
						if( g_pGroup_Window->m_pMnDWindow->m_nNoticeMode != _XMNDNOTICEMODE_READ )
							g_pGroup_Window->m_pMnDWindow->SelectNoticeMode(_XMNDNOTICEMODE_READ);					
					}
				}		
			}
			break;
		case _XDEF_GROUP_MASTERNDISCIPLE_WRITEFINISH :
			{
			}
			break;
		case _XDEF_GROUP_MASTERNDISCIPLE_WRITECANCLE :
			{
			}
			break;
	#endif
	}
}

void XProc_MainGame::GambleBoxWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if( !m_pGambleBoxWindowPtr || !m_pGambleBoxWindowPtr->GetShowStatus() )
		return;

	switch( wparam )
	{
	case _XDEF_GAMBLE_OPEN :
		{
			if( m_pGambleBoxWindowPtr )
			{
				if( m_pGambleBoxWindowPtr->GetShowStatus() )
				{
					g_pLocalUser->m_bGambleBoxOpenLock = TRUE;
					m_pGambleBoxWindowPtr->m_MsgBoxOpen = TRUE;
					
					if( g_pInventory_Window->GetSocketListCount() == 0 )	// ���� ����
					{
						m_pGambleBoxWindowPtr->MessagePrint(2);
					}
					else	// �ø� �Ἥ ����
					{
						m_pGambleBoxWindowPtr->MessagePrint(1);						
					}
				}			
			}
		}
		break;
	case _XDEF_GAMBLE_CANCLE :
	case _XDEF_GAMBLE_CLOSE :
		{
			if(m_pGambleBoxWindowPtr)
			{
				if( m_pGambleBoxWindowPtr->GetShowStatus() )
				{
					m_pGambleBoxWindowPtr->m_MsgBoxOpen = TRUE;
					m_pGambleBoxWindowPtr->MessagePrint(4);
				}
			}
		}	
		break;
	case _XDEF_GAMBLE_HELP :
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_BONUSBOX );
		}
		break;
	case _XDEF_GAMBLE_STOPOK :
		{
			g_pLocalUser->m_bGambleBoxOpen = FALSE;
			m_pGambleBoxWindowPtr->m_MsgBoxOpen = FALSE;
			if( g_pInventory_Window )
			{
				if( m_pGambleBoxWindowPtr )
				{
					m_pGambleBoxWindowPtr->ShowWindow(FALSE);
					m_pGambleBoxWindowPtr->ResetSocketItemList();					
					g_pInventory_Window->ResetSocketIndexList_GambleBox();
					if( g_pInventory_Window->GetShowStatus() )
						g_pInventory_Window->SetDefaultPosition();

					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_1610), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
				}
				_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
				if( pChargeGauge_Window )
				{
					if(pChargeGauge_Window->GetShowStatus())
					{
						pChargeGauge_Window->SetGaugeStop();
					}
					else
					{
						g_pLocalUser->m_bGambleBoxOpenLock = FALSE;
					}
				}
			}	
		}
		break;
	case _XDEF_GAMBLE_STOPNO :
		{
			g_pLocalUser->m_bGambleBoxOpenLock = FALSE;			
			m_pGambleBoxWindowPtr->m_MsgBoxOpen = FALSE;
		}
		break;
	case _XDEF_GAMBLE_OPENOK	:
		{
			if( g_pLocalUser->m_bGambleBoxOpen )
			{
				_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
				if( pChargeGauge_Window )
				{
					if(!pChargeGauge_Window->GetShowStatus())
					{
						pChargeGauge_Window->SetDelayTime( 4500.0f );
						pChargeGauge_Window->SetGaugeMode( _XGAUGE_GAMBLE );
						pChargeGauge_Window->ShowWindow(TRUE);
						g_MainWindowManager.SetTopWindow(pChargeGauge_Window);					
					}
				}
				if( m_pGambleBoxWindowPtr )
				{
					m_pGambleBoxWindowPtr->m_MsgBoxOpen = FALSE;
					m_pGambleBoxWindowPtr->m_pBoxOpenButton->EnableWindow(FALSE);
					m_pGambleBoxWindowPtr->m_pApplyCancleButton->EnableWindow(FALSE);
					m_pGambleBoxWindowPtr->m_pClosebutton->EnableWindow(FALSE);
				}
			}			
		}
		break;
	case _XDEF_GAMBLE_OPENNO :
		{
			g_pLocalUser->m_bGambleBoxOpenLock = FALSE;			
			m_pGambleBoxWindowPtr->m_MsgBoxOpen = FALSE;
		}
		break;
	default:
		break;
	}
}

void XProc_MainGame::PersonalStoreWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if( !m_pPersonalStoreWindowPtr ) return;
	
	switch( wparam )
	{
	case _XDEF_PERSONALSTORE_HELPBUTTON :
		{
			if( m_pHedekWindowPtr )
				m_pHedekWindowPtr->SetHedekType( _XHEDEK_SOLOSHOP );
		}
		break;
	case _XDEF_PERSONALSTORE_CLOSEBUTTON :
		{
			if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_OWNER_MODIFY )
			{
				if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_INTEREST )
				{
					if( !m_pPersonalStoreWindowPtr->m_btnOpen->IsButtonEnabled() )
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1963), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
						return;				
					}

				}
				
				m_pPersonalStoreWindowPtr->ShowWindow(FALSE);
				if(g_pInventory_Window->GetShowStatus()) 
					g_pInventory_Window->ShowWindow(FALSE);
				_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
				if(pTradeConfirm_Window)
				{
					if( pTradeConfirm_Window->GetShowStatus() )
					{
						if( pTradeConfirm_Window->m_TradeMode == _XPERSONALSTORE_TOTRADE_ITEM )
						{							
							PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_TRADECONFIRM_CLOSE, 0), (LPARAM)gHWnd); 
						}
					}
				}

				//Author : ����� //breif : ���� X ��ư���� ���� ���� ���� �������� �˸���. //Date : 08/03/18
				// ���� ������ �� �ݱ� ��ư�� ���� 
				if( m_pPersonalStoreWindowPtr->m_ServerPersonalStoreID != -1 )
				{
					if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE )
					{
						// ��� �ִ� ���� ����
						g_pLocalUser->ReloadWeaponModel();

						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
						g_pLocalUser->SetAnimation(_XACTION_IDLE );					
						g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE ); 

						for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; i++)
						{
							if( m_pPersonalStoreWindowPtr->m_TradeItem[i].Get_m_sID() != 0)
							{
								int inventoryindex = m_pPersonalStoreWindowPtr->m_TradeItem[i].m_InventoryNumber;
								if( m_pPersonalStoreWindowPtr->m_TradeItem[i].IsStackable() )
								{
									if( g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() == 0)
									{
										g_pLocalUser->m_UserItemList[inventoryindex].Copy( m_pPersonalStoreWindowPtr->m_TradeItem[i] );
									}
									else
									{
										g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() + m_pPersonalStoreWindowPtr->m_TradeItem[i].Get_m_ucCount());
									}
								}
								else
								{
									g_pLocalUser->m_UserItemList[inventoryindex].Copy( m_pPersonalStoreWindowPtr->m_TradeItem[i] );
								}
							}
						}
						
						m_pPersonalStoreWindowPtr->ResetTradeInfo();
						m_pPersonalStoreWindowPtr->ShowWindow(FALSE);
						if(g_pInventory_Window->GetShowStatus()) 
							g_pInventory_Window->ShowWindow(FALSE);
						
						_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
						if(pTradeConfirm_Window)
						{
							if( pTradeConfirm_Window->GetShowStatus() )
							{
								if( pTradeConfirm_Window->m_TradeMode == _XPERSONALSTORE_TOTRADE_ITEM )
								{
									PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_TRADECONFIRM_CLOSE, 0), (LPARAM)gHWnd); 
								}
							}
						}
						
						// ������ ������ Inventory lock ����
						if(g_pInventory_Window)
						{
							if(g_pInventory_Window->m_bLockInventory)
								g_pInventory_Window->m_bLockInventory = FALSE;
						}
						
						
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						TCHAR messagestring[256];
						memset( messagestring, 0, sizeof(TCHAR)*256 );
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CLOSESTORE));	// "������ �������ϴ�."
						g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
					}
					else if(  g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN 
						|| g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY )
					{
						m_pPersonalStoreWindowPtr->m_btnOpen->EnableWindow(FALSE);
						m_pPersonalStoreWindowPtr->m_btnCancle->EnableWindow(FALSE);
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_stop_req,m_pPersonalStoreWindowPtr->m_ServerPersonalStoreID, 0);
					}
				}
				else// ���� ������ �ƴ� �� �ݱ� ��ư�� ����
				{
					if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_INTEREST || g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_MODIFY )
					{
						m_pPersonalStoreWindowPtr->ShowWindow(FALSE);
						if(g_pInventory_Window->GetShowStatus()) 
							g_pInventory_Window->ShowWindow(FALSE);
						
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_shopitem_list_req,m_pPersonalStoreWindowPtr->m_OwnerUniqueID , 1 );
						g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
						
						m_pPersonalStoreWindowPtr->ResetTradeInfo();
											
						// ������ ������ Inventory lock ����
						if(g_pInventory_Window)
						{
							if(g_pInventory_Window->m_bLockInventory)
								g_pInventory_Window->m_bLockInventory = FALSE;
						}					
					}
					else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN 
						||  g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE 
						|| g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY)
					{
						// ��� �ִ� ���� ����
						g_pLocalUser->ReloadWeaponModel();

						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
						g_pLocalUser->SetAnimation(_XACTION_IDLE );					
						g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE ); 
						
						for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; i++)
						{
							if( m_pPersonalStoreWindowPtr->m_TradeItem[i].Get_m_sID() != 0)
							{
								int inventoryindex = m_pPersonalStoreWindowPtr->m_TradeItem[i].m_InventoryNumber;
								if( m_pPersonalStoreWindowPtr->m_TradeItem[i].IsStackable() )
								{
									if( g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() == 0)
									{
										g_pLocalUser->m_UserItemList[inventoryindex].Copy( m_pPersonalStoreWindowPtr->m_TradeItem[i] );
									}
									else
									{
										g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() + m_pPersonalStoreWindowPtr->m_TradeItem[i].Get_m_ucCount());
									}
								}
								else
								{
									g_pLocalUser->m_UserItemList[inventoryindex].Copy( m_pPersonalStoreWindowPtr->m_TradeItem[i] );
								}
							}
						}
						
						m_pPersonalStoreWindowPtr->ResetTradeInfo();
						m_pPersonalStoreWindowPtr->ShowWindow(FALSE);
						if(g_pInventory_Window->GetShowStatus()) 
							g_pInventory_Window->ShowWindow(FALSE);
						
						_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
						if(pTradeConfirm_Window)
						{
							if( pTradeConfirm_Window->GetShowStatus() )
							{
								if( pTradeConfirm_Window->m_TradeMode == _XPERSONALSTORE_TOTRADE_ITEM )
								{
									PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_TRADECONFIRM_CLOSE, 0), (LPARAM)gHWnd); 
								}
							}
						}
						
						// ������ ������ Inventory lock ����
						if(g_pInventory_Window)
						{
							if(g_pInventory_Window->m_bLockInventory)
								g_pInventory_Window->m_bLockInventory = FALSE;
						}
						

						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						TCHAR messagestring[256];
						memset( messagestring, 0, sizeof(TCHAR)*256 );
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CLOSESTORE));	// "������ �������ϴ�."
						g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
					}
				}
			}
			else
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1964), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				return;				
			}
		}
		break;
	case _XDEF_PERSONALSTORE_OPENBUTTON :
		{
			
			if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && 
				 g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1965), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				return;				
			}
			if( g_pLocalUser->m_CharacterLog.isTrade )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1966), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				return;
			}

			if( g_pLocalUser->m_bReadingBook || g_pLocalUser->m_bInternalUse || g_pLocalUser->m_bSocketPlugIn )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1967), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
				return;
			}

			if( g_pLocalUser->m_bReadingBook || g_pLocalUser->m_bInternalUse || g_pLocalUser->m_bSocketPlugIn ||
				g_pLocalUser->m_bGambleBoxOpen )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1967), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
				return;
			}
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_shop))
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3764), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"�� �߿��� ������ �� �� �����ϴ�."
				return;
			}
#endif

			switch(g_pLocalUser->m_PersonalStoreState) 
			{
			case _XUSER_PERSONALSTORE_OWNER_OPEN: // ���� ����
				{
					g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_OWNER_PREPARE;
					g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_stop_req,m_pPersonalStoreWindowPtr->m_ServerPersonalStoreID, 1);
					m_pPersonalStoreWindowPtr->m_btnOpen->EnableWindow(FALSE);
					m_pPersonalStoreWindowPtr->m_btnCancle->EnableWindow(FALSE);
				}
				break;
			case _XUSER_PERSONALSTORE_OWNER_PREPARE: // ���� ���� 
			case _XUSER_PERSONALSTORE_OWNER_MODIFY:
				{
					if( m_pPersonalStoreWindowPtr->m_PersonalStoreName[0] != 0 )
					{
						// �Ǹ� ������ ��� �����ߴ��� Ȯ���Ѵ�.
						bool binvalidprice = false;
						int totalcount = 0;
						for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; i++)
						{
							if( m_pPersonalStoreWindowPtr->m_TradeItem[i].Get_m_sID() != 0)
							{
								totalcount++;
								if( m_pPersonalStoreWindowPtr->m_TradeItemPrice[i] == 0 )
								{
									binvalidprice = true;
								}
							}
						}
						
						if( totalcount == 0 )
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);							
							TCHAR messagestring[256];
							memset( messagestring, 0, sizeof(TCHAR)*256 );
							sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_EMPTYSTOREITEM));	// "�Ź��� �÷��� �մϴ�."
							g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
						}
						else if( !binvalidprice )
						{
							if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE)
							{
								g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_start_req,0);
							}
							else if(g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY)
							{
								g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_start_req,1);
							}
							m_pPersonalStoreWindowPtr->m_btnOpen->EnableWindow(FALSE);
							m_pPersonalStoreWindowPtr->m_btnCancle->EnableWindow(FALSE);
						}
						else
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);						
							TCHAR messagestring[256];
							memset( messagestring, 0, sizeof(TCHAR)*256 );
							sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ITEMPRICE));		// "�Ź��� ������ ������ �ʾҽ��ϴ�."
							g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
						}
					}
					else
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);					
						TCHAR messagestring[256];
						memset( messagestring, 0, sizeof(TCHAR)*256 );
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_STORENAME));		// "���� �̸��� ����� �մϴ�."
						g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					}
				}
				break;
			case _XUSER_PERSONALSTORE_CLIENT_INTEREST: // ��ǰ ���� 
				{
					if( m_pPersonalStoreWindowPtr->m_SelectedSlotNumber > -1 )
					{
						if( m_pPersonalStoreWindowPtr->m_TradeItemPrice[m_pPersonalStoreWindowPtr->m_SelectedSlotNumber] <= g_pLocalUser->m_Money )
						{
							
							int emptyslot = 0;
							int end = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART;
						#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��
							if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0)
							{
								end = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART;
								if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0)
									end =  _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART;
							}
							//�⺻ �೶ �˻�
							for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART; i < end; i++ )
							{
								if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
									emptyslot++;
							}

							//ĳ�� �೶ �˻�
							for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++ )
							{
								if( g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0 )
									emptyslot++;
							}
						#else
							if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0 )
								end = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART + g_pLocalUser->m_cBackPackExtend;
							else
								end = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART;
							for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < end ; i++)
							{
								if(g_pLocalUser->m_UserItemList[i].Get_m_sID() <= 0)
								{
									emptyslot++;
								}
							}
						#endif
							
							if(emptyslot == 0)
							{
								TCHAR messagestring[256];
								strncpy(messagestring, _XGETINTERFACETEXT(ID_STRING_MESSAGEHANDLER_NOTENOUGHINVEN), 255);
								g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
								return;
							}
														

							char selectedslotnumber = m_pPersonalStoreWindowPtr->m_SelectedSlotNumber;
							char ctype = m_pPersonalStoreWindowPtr->m_TradeItem[selectedslotnumber].Get_m_cType();
							unsigned short id = m_pPersonalStoreWindowPtr->m_TradeItem[selectedslotnumber].Get_m_sID();
							int price = m_pPersonalStoreWindowPtr->m_TradeItemPrice[selectedslotnumber];

							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);					

							TCHAR		namestring[128];
							memset(namestring, 0, sizeof(TCHAR)*128);
							strcpy(namestring, _XGameItem::GetItemName(ctype, id));
							
							TCHAR messagestring[256];
							memset(messagestring, 0, sizeof(TCHAR)*256);
							// _T("%s(��)�� %d�� �����Ͻðڽ��ϱ�?")
							strcpy(messagestring, 
								   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAMEMESSAGEHANDLER_BUYITEM, namestring, &price ) );

							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_PERSONALSTORE_BUYOK, _XDEF_PERSONALSTORE_BUYNO );

							// ������ ���� �޼��� �ڽ��� �߸� �ٽ� ���Թ�ư�� ������ ���ϰ� ���� ������ �ϸ� �ٽ� ��ư�� ���� �� �ֵ��� �Ѵ�.
							m_pPersonalStoreWindowPtr->m_btnOpen->EnableWindow(FALSE);
							m_pPersonalStoreWindowPtr->m_btnCancle->EnableWindow(FALSE);
						}
						else
						{
							
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							
							int type = m_pPersonalStoreWindowPtr->m_TradeItem[ m_pPersonalStoreWindowPtr->m_SelectedSlotNumber ].Get_m_cType();
							int id = m_pPersonalStoreWindowPtr->m_TradeItem[ m_pPersonalStoreWindowPtr->m_SelectedSlotNumber ].Get_m_sID();
							TCHAR		namestring[128];
							memset(namestring, 0, sizeof(TCHAR)*128);
							strcpy(namestring, _XGameItem::GetItemName(type, id));
							
							TCHAR messagestring[256];
							memset(messagestring, 0, sizeof(TCHAR)*256);
							sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTENOUGHBUYMONEY), namestring);		// _T("%s(��)�� ��⿡�� �������� �����մϴ�.")
							g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
						}

					}	
					else
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						TCHAR messagestring[256];
						memset( messagestring, 0, sizeof(TCHAR)*256 );
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTSELECT));	// "������ ��ǰ�� �������� �ʾҽ��ϴ�."
						g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					}
				}
				break;
			case _XUSER_PERSONALSTORE_CLIENT_MODIFY: // ���� ���̹Ƿ� ������ �� ����
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(TCHAR)*256 );
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ITEMREFRESH)); // ���������� �Ź��� ���� �� �Դϴ�.
					g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				}
				break;
			}
		}
		break;
	case _XDEF_PERSONALSTORE_BUYOK :
		{
			unsigned short owneruniqueid = m_pPersonalStoreWindowPtr->m_OwnerUniqueID;
			char selectedslotnumber = m_pPersonalStoreWindowPtr->m_SelectedSlotNumber;
			char ctype = m_pPersonalStoreWindowPtr->m_TradeItem[selectedslotnumber].Get_m_cType();
			unsigned short id = m_pPersonalStoreWindowPtr->m_TradeItem[selectedslotnumber].Get_m_sID();
			if( id != 0)
			{
				DWORD typeandid = (DWORD)ctype<<16 | id;	
				g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_shopitem_buy_req,owneruniqueid, typeandid, selectedslotnumber );
			}						
		}
		break;
	case _XDEF_PERSONALSTORE_BUYNO :
		{
			m_pPersonalStoreWindowPtr->m_btnOpen->EnableWindow(TRUE);
			m_pPersonalStoreWindowPtr->m_btnCancle->EnableWindow(TRUE);
									
		}
		break;
	case _XDEF_PERSONALSTORE_INPUTOKBUTTON :
		{
			if( m_pPersonalStoreWindowPtr->m_Price > 0 )
			{
				m_pPersonalStoreWindowPtr->m_TradeItemPrice[m_pPersonalStoreWindowPtr->m_SelectedSlotNumber] = m_pPersonalStoreWindowPtr->m_Price;
				m_pPersonalStoreWindowPtr->m_Price = 0;
				m_pPersonalStoreWindowPtr->m_TradeItemPriceIMEControl.ClearBuffer();
				_XIMEKERNEL.SetFocus( NULL ); 
				m_pPersonalStoreWindowPtr->m_btnInputOK->ShowWindow(FALSE);
			}
		}
		break;
	case _XDEF_PERSONALSTORE_CANCLEBUTTON :
		{
			// ���� ������ �� �ݱ� ��ư�� ���� 
			if( m_pPersonalStoreWindowPtr->m_ServerPersonalStoreID != -1 )
			{
				if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE )
				{
					// ��� �ִ� ���� ����
					g_pLocalUser->ReloadWeaponModel();

					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
					g_pLocalUser->SetAnimation(_XACTION_IDLE );					
					g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE ); 

					for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; i++)
					{
						if( m_pPersonalStoreWindowPtr->m_TradeItem[i].Get_m_sID() != 0)
						{
							int inventoryindex = m_pPersonalStoreWindowPtr->m_TradeItem[i].m_InventoryNumber;
							if( m_pPersonalStoreWindowPtr->m_TradeItem[i].IsStackable() )
							{
								if( g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() == 0)
								{
									g_pLocalUser->m_UserItemList[inventoryindex].Copy( m_pPersonalStoreWindowPtr->m_TradeItem[i] );
								}
								else
								{
									g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() + m_pPersonalStoreWindowPtr->m_TradeItem[i].Get_m_ucCount());
								}
							}
							else
							{
								g_pLocalUser->m_UserItemList[inventoryindex].Copy( m_pPersonalStoreWindowPtr->m_TradeItem[i] );
							}
						}
					}
					
					m_pPersonalStoreWindowPtr->ResetTradeInfo();
					m_pPersonalStoreWindowPtr->ShowWindow(FALSE);
					if(g_pInventory_Window->GetShowStatus()) 
						g_pInventory_Window->ShowWindow(FALSE);
					
					_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
					if(pTradeConfirm_Window)
					{
						if( pTradeConfirm_Window->GetShowStatus() )
						{
							if( pTradeConfirm_Window->m_TradeMode == _XPERSONALSTORE_TOTRADE_ITEM )
							{
								PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_TRADECONFIRM_CLOSE, 0), (LPARAM)gHWnd); 
							}
						}
					}
					
					// ������ ������ Inventory lock ����
					if(g_pInventory_Window)
					{
						if(g_pInventory_Window->m_bLockInventory)
							g_pInventory_Window->m_bLockInventory = FALSE;
					}
					
					
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(TCHAR)*256 );
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CLOSESTORE));	// "������ �������ϴ�."
					g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				}
				else if(  g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN 
					|| g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY )
				{
					m_pPersonalStoreWindowPtr->m_btnOpen->EnableWindow(FALSE);
					m_pPersonalStoreWindowPtr->m_btnCancle->EnableWindow(FALSE);
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_stop_req,m_pPersonalStoreWindowPtr->m_ServerPersonalStoreID, 0);
				}
			}
			else// ���� ������ �ƴ� �� �ݱ� ��ư�� ����
			{
				if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_INTEREST || g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_MODIFY )
				{
					m_pPersonalStoreWindowPtr->ShowWindow(FALSE);
					if(g_pInventory_Window->GetShowStatus()) 
						g_pInventory_Window->ShowWindow(FALSE);
					
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_shopitem_list_req,m_pPersonalStoreWindowPtr->m_OwnerUniqueID , 1 );
					g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
					
					m_pPersonalStoreWindowPtr->ResetTradeInfo();
										
					// ������ ������ Inventory lock ����
					if(g_pInventory_Window)
					{
						if(g_pInventory_Window->m_bLockInventory)
							g_pInventory_Window->m_bLockInventory = FALSE;
					}					
				}
				else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN 
					||  g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE 
					|| g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY)
				{
					// ��� �ִ� ���� ����
					g_pLocalUser->ReloadWeaponModel();

					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
					g_pLocalUser->SetAnimation(_XACTION_IDLE );					
					g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE ); 
					
					for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; i++)
					{
						if( m_pPersonalStoreWindowPtr->m_TradeItem[i].Get_m_sID() != 0)
						{
							int inventoryindex = m_pPersonalStoreWindowPtr->m_TradeItem[i].m_InventoryNumber;
							if( m_pPersonalStoreWindowPtr->m_TradeItem[i].IsStackable() )
							{
								if( g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() == 0)
								{
									g_pLocalUser->m_UserItemList[inventoryindex].Copy( m_pPersonalStoreWindowPtr->m_TradeItem[i] );
								}
								else
								{
									g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() + m_pPersonalStoreWindowPtr->m_TradeItem[i].Get_m_ucCount());
								}
							}
							else
							{
								g_pLocalUser->m_UserItemList[inventoryindex].Copy( m_pPersonalStoreWindowPtr->m_TradeItem[i] );
							}
						}
					}
					
					m_pPersonalStoreWindowPtr->ResetTradeInfo();
					m_pPersonalStoreWindowPtr->ShowWindow(FALSE);
					if(g_pInventory_Window->GetShowStatus()) 
						g_pInventory_Window->ShowWindow(FALSE);
					
					_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
					if(pTradeConfirm_Window)
					{
						if( pTradeConfirm_Window->GetShowStatus() )
						{
							if( pTradeConfirm_Window->m_TradeMode == _XPERSONALSTORE_TOTRADE_ITEM )
							{
								PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_TRADECONFIRM_CLOSE, 0), (LPARAM)gHWnd); 
							}
						}
					}
					
					// ������ ������ Inventory lock ����
					if(g_pInventory_Window)
					{
						if(g_pInventory_Window->m_bLockInventory)
							g_pInventory_Window->m_bLockInventory = FALSE;
					}
					

					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(TCHAR)*256 );
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CLOSESTORE));	// "������ �������ϴ�."
					g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				}
			}
		}
		break;
	}
}

void XProc_MainGame::LifeSkillWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	
	if( !m_pPersonalStoreWindowPtr ) return;
	if( !g_pLifeSkill_Window ) return;

	switch( wparam )
	{
		case _XDEF_LIFESKILL_CLASSBUTTON1			:
			{
				g_pLifeSkill_Window->ChangeTab(_XLIFETAB_LIFECLASS1);

			}
			break;
		case _XDEF_LIFESKILL_CLASSBUTTON2			:
			{
				g_pLifeSkill_Window->ChangeTab(_XLIFETAB_LIFECLASS2);
			}
			break;
		case _XDEF_LIFESKILL_CLASSBUTTON3			:
			{
				g_pLifeSkill_Window->ChangeTab(_XLIFETAB_LIFECLASS3);
			}
			break;
		case _XDEF_LIFESKILL_CLASSBUTTON4			:
			{
				g_pLifeSkill_Window->ChangeTab(_XLIFETAB_LIFECLASS4);
			}
			break;
		case _XDEF_LIFESKILL_CLASSBUTTON5			:
			{
				g_pLifeSkill_Window->ChangeTab(_XLIFETAB_LIFECLASS5);
			}
			break;			
		case _XDEF_LIFESKILL_TYPEBUTTON1			:
			{
				g_pLifeSkill_Window->ChangeType( _XCOLLECTION_TYPE1 );
			}
			break;			
		case _XDEF_LIFESKILL_TYPEBUTTON2			:
			{
				g_pLifeSkill_Window->ChangeType( _XCOLLECTION_TYPE2 );
			}
			break;			
		case _XDEF_LIFESKILL_TYPEBUTTON3			:
			{
				g_pLifeSkill_Window->ChangeType( _XCOLLECTION_TYPE3 );
			}
			break;			
		case _XDEF_LIFESKILL_TYPEBUTTON4			:
			{
				g_pLifeSkill_Window->ChangeType( _XCOLLECTION_TYPE4 );
			}
			break;			
		case _XDEF_LIFESKILL_TYPEBUTTON5			:
			{
				g_pLifeSkill_Window->ChangeType( _XCOLLECTION_TYPE5 );
			}
			break;			
		case _XDEF_LIFESKILL_TYPEBUTTON6			:
			{
				g_pLifeSkill_Window->ChangeType( _XCOLLECTION_TYPE6 );
			}
			break;
		case _XDEF_LIFESKILL_TYPEBUTTON7			:
			{
				g_pLifeSkill_Window->ChangeType( _XCOLLECTION_TYPE7 );
			}
			break;
		case _XDEF_LIFESKILL_INPUTCOLLECTIONBUTTON		:
			{
				if( g_pInventory_Window )
				{
					if( g_pInventory_Window->GetShowStatus() )
						g_pInventory_Window->ShowWindow( FALSE );
					else
					{
						_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);
						
						g_pInventory_Window->ShowWindow( TRUE );
						POINT pos = pMainFrame_Window->GetWindowPos();
						SIZE size = pMainFrame_Window->GetWindowSize();
						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
						{	
							SIZE size1 = g_pInventory_Window->GetWindowSize();
							g_pInventory_Window->MoveWindow( pos.x + size.cx, pos.y + size.cy - size1.cy );
						}
						else
						{
							g_pInventory_Window->MoveWindow( pos.x + size.cx, pos.y );
						}
						
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1968), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);	
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1969), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);	
											
					}
				}

			}
			break;
		case _XDEF_LIFESKILL_INPUTCOLLECTION_OK			:
			{
#ifdef _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI
				if(g_pLifeSkill_Window)
				{
					g_NetworkKernel.SendPacket(MSG_NO_COLLECT_REQ, en_collect_mode_save_collection);
				}
#else
				if(g_pLifeSkill_Window)
				{
					if(g_pInventory_Window)
					{
						g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();// �ӽ÷� ��Ŷ ������ ���־� ��.
						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
					}
					// ��Ŷ ������ �κ� : �÷��� �ε��� , �ʿ��� ������ �ε��� 
					int collectionindex = g_pLifeSkill_Window->m_pSelectedCollectionItem->m_CollectionIndex;
					int needindex = g_pLifeSkill_Window->m_indexCompleteNeedItem;
					//0: �ڿ� ����, 1: ��ǰ save, 2: ��ǰ ����
					g_NetworkKernel.SendPacket(MSG_NO_COLLECT_REQ, 1, collectionindex, needindex);
				}
#endif
			}
			break;
		case _XDEF_LIFESKILL_INPUTCOLLECTION_NO			:
			{
				if(g_pLifeSkill_Window)
				{
					if(g_pInventory_Window)
					{
						g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
						
						if( g_pDefaultTooltip_Window )
						{
							if( g_pDefaultTooltip_Window->GetShowStatus() )						
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
				}
			}
			break;
		case _XDEF_LIFESKILL_COLLECTIONLISTBOX			:
			{
				if(g_pLifeSkill_Window)
				{
					g_pLifeSkill_Window->SetSelectedCollectionItemIndex(lparam);
				}
			}
			break;			
		case _XDEF_LIFESKILL_RESOURCELISTBOX			:
			{
				if(g_pLifeSkill_Window)
				{
					g_pLifeSkill_Window->SetSelectedResourceItemOrGrupItem(lparam);
				}
			}
			break;			
		case _XDEF_LIFESKILL_RESOURCENEEDITEMLISTBOX	:
			{
			}
			break;			
		case _XDEF_LIFESKILL_USEBUTTON :
		{	
			if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1970), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
				_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1970) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				return;
			}
			else if( g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE ) 
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1971), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1971) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				return;
			}
			else if( g_pLocalUser->m_CharacterLog.isTrade )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);				
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1966), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1966) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				return;
			}
			else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1972), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
				_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1972) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				return;
			}
			else if( g_pLocalUser->m_bReadingBook )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1973), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
				return;				
			}
			else if( g_pLocalUser->m_bInternalUse )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1974), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
				return;
			}
			else if( g_pLocalUser->m_bSocketPlugIn )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1975), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return;
			}
			else if( g_pLocalUser->m_bGambleBoxOpen )
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1976), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return;
			}
			else if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1977), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return;
			}
			else if(g_pLocalUser->m_bMatchPRMode)
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1978), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return;
			}
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
			if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_shop))
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3764), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"�� �߿��� ������ �� �� �����ϴ�."
				return;
			}
#endif
#ifdef _XTS_PK
			else if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2623), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );	
				return;
			}
#endif
			else if(g_pLocalUser->m_bFaceMaskMode)	// ���� ��
			{
				if( g_pState_Window )
					g_pState_Window->PrintSystemMessage(8);
				return;
			}
			else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE || g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY )
			{
				if( m_pPersonalStoreWindowPtr->GetShowStatus() )
				{
					//������ ������ ���� â�� ���� �� �����ϴ�
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					return;
				}
			}
			else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_INTEREST || g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_MODIFY)
			{
				if( m_pPersonalStoreWindowPtr->GetShowStatus() )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					return;
				}
			}
			
#ifdef _XDEF_ESCAPE_20070205
			_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
			if(pEscape_Window)
			{					
				if( pEscape_Window->GetShowStatus() )
				{
					// Ż�� ���̸� �̵��� �Ұ����ϴ�.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3315), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);// Ż�� �߿��� ������ �� �� �����ϴ�
					return;
				}
			}
#endif
					
			if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
			{
				m_pPersonalStoreWindowPtr->ShowWindow(TRUE);
				if(!g_pInventory_Window->GetShowStatus()) 
					g_pInventory_Window->ShowWindow(TRUE);
			}
			else
			{
				bool checkpersonalstore = false;
				if (!g_Userlist.m_smUserList.empty())
				{
					_XUser* pUser;
					smdef_XUser::iterator it;
					for (it = g_Userlist.m_smUserList.begin(); it != g_Userlist.m_smUserList.end(); ++ it)
					{
						pUser = (*it).second;
						if(pUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN || pUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY )
						{
							FLOAT dx = g_pLocalUser->m_Position.x - pUser->m_Position.x;
							FLOAT dz = g_pLocalUser->m_Position.z - pUser->m_Position.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);
							
							if(distance <= 2.0f) // 2.0f �ȿ� ������ ������ ������ �� ����.
							{
								checkpersonalstore = true;
							}
						}
					}
				}

				if( !checkpersonalstore )
				{
					if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_NONE )
					{
						/*// ��� �ִ� �������� ����
						if( g_pLocalUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
						{
							if(g_WeaponItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
							{
								g_pLocalUser->m_CurrentWeaponID = g_pLocalUser->m_CharacterInfo.modeldescript.item1;
								g_pLocalUser->m_CharacterInfo.modeldescript.item1 = 0;
								g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
								g_pLocalUser->m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
							}
						}
						else if( g_pLocalUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
						{
							if(g_WeaponItemProperty2[g_pLocalUser->m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
							{
								g_pLocalUser->m_CurrentWeaponID = g_pLocalUser->m_CharacterInfo.modeldescript.item1;
								g_pLocalUser->m_CharacterInfo.modeldescript.item1 = 0;
								g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
								g_pLocalUser->m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
							}
						}
						else if( g_pLocalUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
						{
							if(g_WeaponItemProperty3[g_pLocalUser->m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
							{
								g_pLocalUser->m_CurrentWeaponID = g_pLocalUser->m_CharacterInfo.modeldescript.item1;
								g_pLocalUser->m_CharacterInfo.modeldescript.item1 = 0;
								g_pLocalUser->m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
								g_pLocalUser->m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
							}
						}
*/
						g_pLocalUser->DeleteWeaponModel();	// 2005.12.15 ���� �ּ��ڵ� �̰ɷ� ����
						
						g_pLocalUser->ResetAttackTarget();
						g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
						extern _XCHARACTER_MOTION	g_EmotionActionTable[16];			
						g_pLocalUser->SetAnimationIndex( g_EmotionActionTable[4] );
						g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE ); 
						m_pPersonalStoreWindowPtr->ResetTradeInfo();

					}
					g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_OWNER_PREPARE;
					m_pPersonalStoreWindowPtr->m_btnOpen->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_BOOTH_OPEN);	// _T("��������")
					m_pPersonalStoreWindowPtr->ShowWindow(TRUE);					
					g_pInventory_Window->ShowWindow(TRUE);

					#ifdef _ACCLAIM_RUBICONADSYSTEM	
					if( g_ViewRubiconAD )
					{					
						m_pPersonalStoreWindowPtr->MoveWindow( (gnWidth>>1)-288, 74+g_RubiconADWindowSize.cy );
						g_pInventory_Window->MoveWindow( (gnWidth>>1),74+g_RubiconADWindowSize.cy );
					}
					else
					{
						m_pPersonalStoreWindowPtr->MoveWindow( (gnWidth>>1)-288, 74 );
						g_pInventory_Window->MoveWindow( (gnWidth>>1),74 );
					}
					#else
					m_pPersonalStoreWindowPtr->MoveWindow( (gnWidth>>1)-288, 74 );
					g_pInventory_Window->MoveWindow( (gnWidth>>1),74 );
					#endif
					g_pInventory_Window->SetPersonalStoreWindowPtr(m_pPersonalStoreWindowPtr);
				
				}
				else
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					
					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(TCHAR)*256 );
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_NOTCREATESTORE));	// "�α� ������ ��ġ�� �ʹ� ����� ������ ������ �� �����ϴ�.."
					g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
					_XInsertCenterSystemMessage(messagestring , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				}
			}					
		}
		break;

		case _XDEF_LIFESKILL_PVPPRBUTTON: // �� ȫ�� �ϱ� ��ư
			{
				// ���� ó���ϱ� 
				if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
					(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1979), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					return;
				}
				else if( g_pLocalUser->m_CharacterLog.isTrade )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1980), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
					return;
				}
				else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1981), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
					return;
				}
				else if( g_pLocalUser->m_bReadingBook )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1982), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
					return;				
				}
				else if( g_pLocalUser->m_bInternalUse )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1983), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
					return;
				}
				else if( g_pLocalUser->m_bSocketPlugIn )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1984), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					return;
				}
				else if( g_pLocalUser->m_bGambleBoxOpen )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1985), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					return;
				}
				else if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1986), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					return;
				}
				else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE)
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1987), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					return;					
				}
				else if( g_CurrentZoneInfoIndex != 17 )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1988), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					return;					

				}				
				else if( g_pLocalUser->m_CharacterInfo.Get_innerlevel() < 25 )
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1989), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					return;					
					
				}
				
#ifdef _XDEF_ESCAPE_20070205
				_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
				if(pEscape_Window)
				{					
					if( pEscape_Window->GetShowStatus() )
					{
						// Ż�� �߿��� ��ȫ���� �� �� �����ϴ�.
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3316), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);// Ż�� �߿��� ��ȫ���� �� �� �����ϴ�
						return;
					}
				}
#endif
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
				if(g_pLocalUser->GetEnterMatchEvent())
				{
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1986), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//�� �߿��� ȫ���� �� �� �����ϴ�.
					return;
				}
#endif
				
				_XWindow_MatchPR* pMatchPR_Window = (_XWindow_MatchPR*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MATCHPR);
				if( pMatchPR_Window)
				{
					if( !pMatchPR_Window->GetShowStatus() )
					{
						pMatchPR_Window->ShowWindow(TRUE);
					}
				}

		}
		break;
#ifdef _XDEF_EVENTWINDOW
		case _XDEF_LIFESKILL_EVENTBUTTON: // �̺�Ʈ ��ư
			{				
				_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
				if( pEvent_Window )
				{
					pEvent_Window->ShowWindow( !pEvent_Window->GetShowStatus() );

					if( pEvent_Window->GetShowStatus() )
					{
						pEvent_Window->MoveWindow(388,114);
					}
				}
			}
		break;	
#endif
		
	}
}

void XProc_MainGame::EventWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
	if( !pEvent_Window ) return;

	switch(wparam) {
	case _XDEF_EVENT_BTN_CLOSE						:
		{
			pEvent_Window->ShowWindow(FALSE);
		}
		break;	
	case _XDEF_EVENT_BTN_HELP						:
		{
		}
		break;	
	case _XDEF_EVENT_LISTBOX_TITLE					:
		{
			pEvent_Window->m_SelectedEventID = (int)lparam;
			pEvent_Window->SetSelectedListItme( pEvent_Window->m_SelectedEventID );
		}
		break;	
	case _XDEF_EVENT_BTN_GET : //Author : ����� �߰� //breif : ������ �ޱ� ��ư
		{
			bool checkvalid  = false;

		#ifdef _XDEF_ESCAPE_20070205
			_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
			//Ż��
			if(pEscape_Window && pEscape_Window->GetShowStatus())
				checkvalid = true;
		#endif
			// ��� or ���� ���� �϶�
			if(g_pLocalUser->GetMotionClass() == _XACTION_DYING || g_pLocalUser->GetMotionClass() == _XACTION_DIE || g_pLocalUser->GetMotionClass() == _XACTION_REBIRTH)
				checkvalid = true;

			// ���� ���� ���� ��
			if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
				checkvalid = true;

			// �� ȫ����
			if(g_pLocalUser->m_bMatchPRMode)
				checkvalid = true;

			//���
			if( g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING )
				checkvalid = true;

			//�ŷ���
			if( g_pLocalUser->m_CharacterLog.isTrade )
				checkvalid = true;

			if( g_pLocalUser->m_bSocketPlugIn || g_pLocalUser->m_bGambleBoxOpen || 
				g_pLocalUser->m_bReadingBook || g_pLocalUser->m_bInternalUse )
				checkvalid = true;

			//��ȭ����
			if( g_pLocalUser->m_BattleMode != _XBATTLE_PEACE )
				checkvalid = true;

			if( checkvalid )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, NULL, _XGETINTERFACETEXT(ID_STRING_NEW_3785), TRUE); //���� �ޱⰡ �Ұ����� �����Դϴ�
			}
			else 
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, _XGETINTERFACETEXT(ID_STRING_NEW_3780), TRUE, _XDEF_EVENT_BTN_APPLY);
			}
		}
		break;
	case _XDEF_EVENT_BTN_APPLY :
		{
		#ifdef _XDEF_LEVELEVENT_080319_MAGVIPER //Author : ����� //breif : ������ �̺�Ʈ
			if( pEvent_Window->m_SelectedEventType == 4 )
			{
				g_NetworkKernel.SendPacket(MSG_NO_PRESENT);
			}
			else
		#endif
			{
				g_NetworkKernel.SendPacket(MSG_NO_EVENT_REQUEST);
			}
		}
		break;
	case _XDEF_EVENT_LISTBOX_DESCRIPTION			:
		{
		}
		break;	
	case _XDEF_EVENT_BTN_OK							:
		{
			//�ޱ� ���¸� üũ�ؾ���.. //Author : �����
			bool checkmessage  = false;

			#ifdef _XDEF_ESCAPE_20070205
			_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
			//Ż��
			if(pEscape_Window && pEscape_Window->GetShowStatus())
				checkmessage = true;
			#endif
			// ��� or ���� ���� �϶�
			if(g_pLocalUser->GetMotionClass() == _XACTION_DYING || g_pLocalUser->GetMotionClass() == _XACTION_DIE || g_pLocalUser->GetMotionClass() == _XACTION_REBIRTH)
				checkmessage = true;

			// ���� ���� ���� ��
			if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
				checkmessage = true;

			// �� ȫ����
			if(g_pLocalUser->m_bMatchPRMode)
				checkmessage = true;

			//���
			if( g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING )
				checkmessage = true;

			//�ŷ���
			if( g_pLocalUser->m_CharacterLog.isTrade )
				checkmessage = true;

			if( g_pLocalUser->m_bSocketPlugIn || g_pLocalUser->m_bGambleBoxOpen || 
				g_pLocalUser->m_bReadingBook || g_pLocalUser->m_bInternalUse )
				checkmessage = true;

			//��ȭ����
			if( g_pLocalUser->m_BattleMode != _XBATTLE_PEACE )
				checkmessage = true;

			if( checkmessage )
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, NULL, _XGETINTERFACETEXT(ID_STRING_NEW_3785), TRUE); //���� �ޱⰡ �Ұ����� �����Դϴ�
				return;
			}
		
#ifdef _XDEF_EVENTWINDOW
			// �ڵ尡 ��� �Էµƴ��� ����ó���ϱ�
			bool checkvalid = true;
			for( int i=0; i < 5; ++i )
			{
				if( pEvent_Window->m_SerialNumber[i] )
				{
					if( pEvent_Window->m_SelectedMaxSerialNumber[i] > 0 )
					{
						if( pEvent_Window->m_SerialNumber[i][0] == 0 )
						{
							checkvalid = false;
							break;
						}
					}
				}
			}

			if( checkvalid )
				g_NetworkKernel.SendPacket(MSG_NO_EVENT_REQUEST);			
			else
			{				
				TCHAR messagestring[256];
				sprintf( messagestring, _XGETINTERFACETEXT( ID_STRING_MAINGAMEMESSAGEHANDLER_1886 ));
				
				g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
		
			}

				
#endif
		}
		break;	
	}
	
}

void XProc_MainGame::CashItemWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
#ifdef _XTS_CASHITEM
	_XWindow_CashItem* pCashItem_Window = (_XWindow_CashItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASHITEM);
	if( !pCashItem_Window ) return;
	
	switch(wparam) 
	{
	case _XDEF_CASHITEM_REFRESH_BTN :
		{
			
		}		
		break;
	case _XDEF_CASHITEM_ALLMOVE_BTN :
		{			
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1990), TRUE, _XDEF_CASHITEM_ALLMOVEYES_BTN, _XDEF_CASHITEM_ALLMOVENO_BTN);
		}
		break;
	case _XDEF_CASHITEM_BUYITEM_BTN	:
	case _XDEF_CASHITEM_PRESENTITEM_BTN	:
		{
			pCashItem_Window->SetMode((int)wparam);
		}		
		break;
	case _XDEF_CASHITEM_CLOSE_BTN :
		{
			if( pCashItem_Window->GetShowStatus() )
				pCashItem_Window->ShowWindow(FALSE);
		}
		break;
	case _XDEF_CASHITEM_HELPE_BTN :		
		break;
	case _XDEF_CASHITEM_ALLMOVEYES_BTN :
		{
			pCashItem_Window->AllMoveItem();
		}
		break;
	}
#endif
}
void XProc_MainGame::NPCCollectionWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if( !m_pNPCCollectionWindowPtr ) return;
	
	switch(wparam) 
	{
	case _XDEF_NPCCOLLECTION_HELP_BTN			:
		{
		}
		break;
	case _XDEF_NPCCOLLECTION_REWARDSET01_BTN	:
		{
			if( !m_pNPCCollectionWindowPtr->CheckInventorySpace(0) )
			{
				// �೶�� ������ �����մϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1991), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				break;
			}

			if( !m_pNPCCollectionWindowPtr->GetProcessReward() )
			{
				m_pNPCCollectionWindowPtr->SetRewardSetIndex(0);
				m_pNPCCollectionWindowPtr->SetProcessReward(true);
				
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
				if( pMessageBox )
				{
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1992), TRUE, _XDEF_NPCCOLLECTION_SELECTREWARD_OK, _XDEF_NPCCOLLECTION_SELECTREWARD_NO);
				}
			}
			else
			{
				// ������ �����ϰ� �ִ� �� �Դϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1993), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
		}
		break;
	case _XDEF_NPCCOLLECTION_REWARDSET02_BTN	:
		{
			if( !m_pNPCCollectionWindowPtr->CheckInventorySpace(1) )
			{
				// �೶�� ������ �����մϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1991), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				break;
			}
			
			if( !m_pNPCCollectionWindowPtr->GetProcessReward() )
			{
				m_pNPCCollectionWindowPtr->SetRewardSetIndex(1);
				m_pNPCCollectionWindowPtr->SetProcessReward(true);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
				if( pMessageBox )
				{
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1994), TRUE, _XDEF_NPCCOLLECTION_SELECTREWARD_OK, _XDEF_NPCCOLLECTION_SELECTREWARD_NO);
				}
			}
			else
			{
				// ������ �����ϰ� �ִ� �� �Դϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1993), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
		}
		break;
	case _XDEF_NPCCOLLECTION_REWARDSET03_BTN	:
		{
			if( !m_pNPCCollectionWindowPtr->CheckInventorySpace(2) )
			{
				// �೶�� ������ �����մϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1991), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				break;
			}

			if( !m_pNPCCollectionWindowPtr->GetProcessReward() )
			{
				m_pNPCCollectionWindowPtr->SetRewardSetIndex(2);
				m_pNPCCollectionWindowPtr->SetProcessReward(true);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
				if( pMessageBox )
				{
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1995), TRUE, _XDEF_NPCCOLLECTION_SELECTREWARD_OK, _XDEF_NPCCOLLECTION_SELECTREWARD_NO);
				}
			}
			else
			{
				// ������ �����ϰ� �ִ� �� �Դϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1993), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
		}
		break;
	case _XDEF_NPCCOLLECTION_REWARDSET04_BTN	:
		{
			if( !m_pNPCCollectionWindowPtr->CheckInventorySpace(3) )
			{
				// �೶�� ������ �����մϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1991), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				break;
			}

			if( !m_pNPCCollectionWindowPtr->GetProcessReward() )
			{
				m_pNPCCollectionWindowPtr->SetRewardSetIndex(3);
				m_pNPCCollectionWindowPtr->SetProcessReward(true);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
				if( pMessageBox )
				{
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1996), TRUE, _XDEF_NPCCOLLECTION_SELECTREWARD_OK, _XDEF_NPCCOLLECTION_SELECTREWARD_NO);
				}
			}
			else
			{
				// ������ �����ϰ� �ִ� �� �Դϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1993), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
		}
		break;
	case _XDEF_NPCCOLLECTION_COLLECTIONLISTBOX	:
		{
			if( !m_pNPCCollectionWindowPtr->GetProcessReward() )
			{
				m_pNPCCollectionWindowPtr->SetSelectedCollectionItemIndex(lparam);
			}
			else
			{
				TCHAR messagestring[256];
				memset( messagestring, 0, sizeof(TCHAR)*256 );
				sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_2629), m_pNPCCollectionWindowPtr->m_pSelectedCollectionItem->m_CollectionName );
			
				// ������ �����ϰ� �ִ� �� �Դϴ�.
				g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
			}
		}
		break;
	case _XDEF_NPCCOLLECTION_SELECTREWARD_OK	:
		{
			// ������ ���� �Ŀ��� ����Ʈ�� �ٽ� �����ؾ� �Ѵ�.
			int collectionindex = m_pNPCCollectionWindowPtr->m_pSelectedCollectionItem->m_CollectionIndex;
			int rewardsetindex = m_pNPCCollectionWindowPtr->GetRewardSetIndex();
			
			g_NetworkKernel.SendPacket(MSG_NO_COLLECT_REQ, en_collect_mode_complete_collection, collectionindex, rewardsetindex );
			m_pNPCCollectionWindowPtr->SetProcessReward(false); // ���߿� ����
		}
		break;
	case _XDEF_NPCCOLLECTION_SELECTREWARD_NO	:
		{
			m_pNPCCollectionWindowPtr->SetProcessReward(false);
		}
		break;
	}
}
void XProc_MainGame::NPCResourceWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if( !m_pNPCResourceWindowPtr ) return;
	
	switch(wparam) 
	{
	case _XDEF_NPCRESOURCE_HELP_BTN				:
		{
		}
		break;
	case _XDEF_NPCRESOURCE_EXCHANGE_BTN			:
		{
			if( m_pNPCResourceWindowPtr->CheckInventorySpace() )
			{
				m_pNPCResourceWindowPtr->m_btnExchange->EnableWindow(FALSE);
				m_pNPCResourceWindowPtr->SetProgressExchange(true);

				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
				if( pMessageBox )
				{
					if( m_pNPCResourceWindowPtr->GetStrength() ) //Author : ����� // breif : ���ÿ��� �߰� //date 07/11/13
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, _XGETINTERFACETEXT(ID_STRING_NEW_3755), TRUE, _XDEF_NPCRESOURCE_EXCHANGE_OK, _XDEF_NPCRESOURCE_EXCHANGE_NO);					
					else
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1997), TRUE, _XDEF_NPCRESOURCE_EXCHANGE_OK, _XDEF_NPCRESOURCE_EXCHANGE_NO);
				}
			}
			else
			{
				// �೶�� ������ �����մϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1991), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
			}
		}
		break;
	case _XDEF_NPCRESOURCE_RESOURCELISTBOX		:
		{				
			if( !m_pNPCResourceWindowPtr->GetProgressExchange() )
			{
				int index = lparam-1;
				m_pNPCResourceWindowPtr->SetSelectedResourceItem(index);
			}
			else
			{
				// ������ �����ϰ� �ִ� �� �Դϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1993), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
		}
		break;
	case _XDEF_NPCRESOURCE_EXCHANGE_OK		:
		{
			int resourceindex = m_pNPCResourceWindowPtr->m_indexSelectedResourceItem;
			//0: �ڿ� ����, 1: ��ǰ save, 2: ��ǰ ����
			g_NetworkKernel.SendPacket(MSG_NO_COLLECT_REQ, 0, resourceindex );
		}
		break;
	case _XDEF_NPCRESOURCE_EXCHANGE_NO		:
		{
			m_pNPCResourceWindowPtr->m_btnExchange->EnableWindow(TRUE);
			m_pNPCResourceWindowPtr->SetProgressExchange(false);
		}
		break;
	}
}

void XProc_MainGame::NewNPCResourceWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if(!m_pNPCResource_New_windowptr) return;

	switch(wparam)
	{
	case _XDEF_NEWRESOURCEWINDOW_RESOURCELIST :
		{
			if(!m_pNPCResource_New_windowptr->GetProgressExchange())
			{
				int index = lparam - 1;
				m_pNPCResource_New_windowptr->SetSelectedResourceItem(index);
				m_pNPCResource_New_windowptr->ChangeTab(_TAB_ITEM);
				m_pNPCResource_New_windowptr->SetItemBasicInfo();
			}
			else
			{
				// ������ �����ϰ� �ִ� �� �Դϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1993), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
		}
		break;
	case _XDEF_NEWRESOURCEWINDOW_EXCHANGE :
		{
			if(m_pNPCResource_New_windowptr->CheckInventorySpace())
			{
				m_pNPCResource_New_windowptr->EnableExchangeButton(FALSE);
				m_pNPCResource_New_windowptr->SetProgressExchange(TRUE);

				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
				if( pMessageBox )
				{
					if(m_pNPCResource_New_windowptr->GetStrength()) //Author : ����� // breif : ���ÿ��� �߰� //date 07/11/13
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, _XGETINTERFACETEXT(ID_STRING_NEW_3755), TRUE, _XDEF_NEWRESOURCEWINDOW_EXCHANGE_OK, _XDEF_NEWRESOURCEWINDOW_EXCHANGE_NO);
					else
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1997), TRUE, _XDEF_NEWRESOURCEWINDOW_EXCHANGE_OK, _XDEF_NEWRESOURCEWINDOW_EXCHANGE_NO);
				}
			}
			else
			{
				// �೶�� ������ �����մϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1991), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
			}
		}
		break;
	case _XDEF_NEWRESOURCEWINDOW_CLOSE :
		{
			if(!m_pNPCResource_New_windowptr->GetProgressExchange())
			{
				m_pNPCResource_New_windowptr->ChangeTab(_TAB_LIST);
			}
			else
			{
				// ������ �����ϰ� �ִ� �� �Դϴ�.
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1993), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
		}
		break;
	case _XDEF_NEWRESOURCEWINDOW_EXCHANGE_OK :
		{
			g_NetworkKernel.SendPacket(MSG_NO_COLLECT_REQ, en_collect_mode_complete_resource, m_pNPCResource_New_windowptr->GetSelectedResourceItem());
		}
		break;
	case _XDEF_NEWRESOURCEWINDOW_EXCHANGE_NO :
		{
			m_pNPCResource_New_windowptr->EnableExchangeButton(TRUE);
			m_pNPCResource_New_windowptr->SetProgressExchange(FALSE);
		}
		break;
	}
}

void XProc_MainGame::InchantWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_Inchant* pInchant_Window = (_XWindow_Inchant*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INCHANT);
	if( !pInchant_Window ) return;

	switch(wparam) 
	{
	case _XDEF_INCHANT_STARTBUTTON :
		{
			pInchant_Window->ClickStartButton();
		}
		break;
	case _XDEF_INCHANT_CANCLEBUTTON :
		{
			pInchant_Window->SetItemInchantStep(US_NONE);
		}
		break;
	case _XDEF_INCHANT_HELPBUTTON :
		{

		}
		break;
	case _XDEF_INCHANT_STARTYESBUTTON :
		{
			pInchant_Window->SetItemInchantStep(US_STEP3);			
		}
		break;
	case _XDEF_INCHANT_STARTNOBUTTON :
		{
			pInchant_Window->SetItemInchantStep(US_STEP1);			
		}
		break;
	case _XDEF_INCHANT_PERIODITEM_YES :
		{
			pInchant_Window->ProcessInchant();
		}
		break;
	case _XDEF_INCHANT_PERIODITEM_NO :
		{
			pInchant_Window->SetItemInchantStep(US_NONE);
		}
		break;
	}
}

void XProc_MainGame::NPCBuffWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if(!m_pNPCBuffWindowPtr)
		return;

	switch(wparam)
	{
	case _XDEF_NPCBUFF_CLOSE :
		{
			m_pNPCBuffWindowPtr->ShowWindow(FALSE);
		}
		break;
	case _XDEF_NPCBUFF_LISTBOX :
		{
			m_pNPCBuffWindowPtr->m_SelectedItem = (int)lparam;
		}
		break;
	case _XDEF_NPCBUFF_APPLY :
		{
			if(m_pNPCBuffWindowPtr->m_SelectedItem > 0)
			{
				_XNPCBUFFSTR* pNPCBuff = NULL;
				_map_NPCBuffStr::iterator iter_buff = g_NPCBuffStr.find(m_pNPCBuffWindowPtr->m_SelectedItem);
				if(iter_buff != g_NPCBuffStr.end())
				{
					pNPCBuff = iter_buff->second;
				}
	
				if(pNPCBuff)
				{
				
					if(pNPCBuff->type == 0)
					{
						TCHAR messagestring[256];
						int index = -1;

						for(int i = 371 ; i <= 376 ; i++)
						{
							index = g_pLocalUser->m_StateList.FindState(en_from_extra, i, 0, 0);	// �ߺ��Ұ� NPCBUff�� slot��ȣ�� 0
							if(index >= 0)
							{
								break;
							}
						}

						if(index >= 0)
						{
							// �ߺ� �Ұ� �޼��� ���
							sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1998));
						}
						else
						{
							sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1999), pNPCBuff->name);
						}

						g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_NPCBUFF_APPLY_OK, 0);
						g_MessageBox.StartTimer(5000, TRUE);
					}
					else
					{
						TCHAR messagestring[256];
						sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1999), pNPCBuff->name);

						g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_NPCBUFF_APPLY_OK, 0);
						g_MessageBox.StartTimer(5000, TRUE);
					}
				}
			}
			else
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_2000), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
		}
		break;
	case _XDEF_NPCBUFF_APPLY_OK :
		{
			if(m_pNPCBuffWindowPtr->m_SelectedItem > 0)
			{
				_XNPCBUFFSTR* pNPCBuff = NULL;
				_map_NPCBuffStr::iterator iter_buff = g_NPCBuffStr.find(m_pNPCBuffWindowPtr->m_SelectedItem);
				if(iter_buff != g_NPCBuffStr.end())
				{
					pNPCBuff = iter_buff->second;
				}
				if(pNPCBuff)
				{
					g_NetworkKernel.SendPacket(MSG_NO_NPC_BUFF_REQ, m_pNPCBuffWindowPtr->m_SelectedItem, pNPCBuff->price);
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_CLOSE, 0), 0);
				}
			}
			else
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_2000), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
		}
		break;
	}
}

void XProc_MainGame::PKRecordWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if(!g_pPKRecord_Window)
		return;

	switch(wparam)
	{
	case _XDEF_PKRECORD_TAB_1 :
	case _XDEF_PKRECORD_TAB_2 :
	case _XDEF_PKRECORD_TAB_3 :
	case _XDEF_PKRECORD_TAB_4 :
	case _XDEF_PKRECORD_TAB_5 :
		{
			g_pPKRecord_Window->ChangeTab(wparam);
		}
		break;
	}
}

// cash item ��� ����
void XProc_MainGame::CashItemUseMesssageHandling(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case _XDEF_CASHITEMUSE_CLEARNICKNAMEYES :
	case _XDEF_CASHITEMUSE_ADDEXPYES :
	case _XDEF_CASHITEMUSE_SHOUTYES :
	case _XDEF_CASHITEMUSE_RESETPOINTYES :
		{
			g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, g_pLocalUser->m_UseItemInvenNumber);
		}
		break;
	case _XDEF_CASHITEMUSE_CLEARNICKNAMENO :
	case _XDEF_CASHITEMUSE_ADDEXPNO :
	case _XDEF_CASHITEMUSE_SHOUTNO :
	case _XDEF_CASHITEMUSE_RESETPOINTNO :
		{
			g_pLocalUser->m_UseItemInvenNumber = 0;
		}
		break;
	}
}

void XProc_MainGame::MoveVillageWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	if(!m_pMoveVillageWindowPtr)
	{
		return;
	}

	switch(wparam)
	{
	case _XDEF_MOVEVILLAGE_MOVEBUTTON :
		{
			if(m_pMoveVillageWindowPtr->GetSelectedMoveVillageIndex() < 0)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3042), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			}
			else
			{
				g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3041), TRUE, _XDEF_MOVEVILLAGE_DECIDEMOVING); //ǥ�� �������� �̿��Ͽ� ������ �̵��Ͻðڽ��ϱ�? ���� �������� ���� �˴ϴ�.
			}
		}
		break;
	case _XDEF_MOVEVILLAGE_LISTBOX :
		{
			m_pMoveVillageWindowPtr->ProcessListItem(lparam);
		}
		break;
	case _XDEF_MOVEVILLAGE_DECIDEMOVING :
		{
			int invennumber = g_pLocalUser->GetUserItemInvenSlotFromSecondType(_XGI_FC_RESOURCE, 4/*_XGI_SC_RESOURCE_MOVE*/);
        #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
            if( invennumber >= _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART &&
		        invennumber < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART )
	        {
		        if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() <= 0 )
                {
                    g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                    m_pMoveVillageWindowPtr->ShowWindow(FALSE);
                    return;
                }
	        }

            if( invennumber >= _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART &&
		        invennumber < _XINVENTORY_SLOTTYPE_TOTALCOUNT )
	        {
		        if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() <= 0 )
                {
                    g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
                    m_pMoveVillageWindowPtr->ShowWindow(FALSE);
                    return;
                }
	        }
        #endif
			m_pMoveVillageWindowPtr->SetSelectedInvenNumber(invennumber);

			// send packet;
			g_NetworkKernel.SendPacket(MSG_NO_NPC_TRADE_REQ, en_npc_trade_mode_move_village);

			m_pMoveVillageWindowPtr->SetSelectedMoveVillageIndex(-1);
			m_pMoveVillageWindowPtr->SetSelectedInvenNumber(-1);

#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
			m_pMoveVillageWindowPtr->ShowWindow(FALSE);
#else
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_CLOSE, 0), 0);
#endif
		}
		break;
	case _XDEF_MOVEVILLAGE_CANCEL :
		{
			m_pMoveVillageWindowPtr->ShowWindow(FALSE);
		}
		break;
	}
}

void XProc_MainGame::CastleWarWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_CastleInfo* pCastleInfo_Window = (_XWindow_CastleInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEINFO);
	if(!pCastleInfo_Window)
	{
		return;
	}

	_XWindow_ChallengerList* pChallengerList_Window = (_XWindow_ChallengerList*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHALLENGERLIST);
	if(!pChallengerList_Window)
	{
		return;
	}

	switch(wparam)
	{
	case _XDEF_CASTLEINFO_REQWAR :
		{
			if(g_pLocalUser->m_nCastleMapId == g_CurrentZoneInfoIndex)
			{
				// ���� ������ ���� ������� ��û�� �� ����.
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3369), TRUE);	// "�ڽ��� ������ ������� ������ �� �����ϴ�."
				break;
			}

			if(g_pLocalUser->m_nCastleMapId > 0)
			{
				// �ٸ� ����� �����̴�
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3399), TRUE);	//"�ٸ� ����� �����ϰ� �����Ƿ� ������ �� �����ϴ�."
				break;
			}
			
			int count = 0;
#ifdef _XTS_ALIANCE_20061018
			if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 )
			{
				count = g_pGroup_Window->m_listAlienceMember.size();
			}
#endif
			if(count <= 0)
			{
				if(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0)
				{
					count = 1;
				}
			}
			if(count > 0)
			{
				int needmoney = g_CBManager.GetChallengeMoney(g_CurrentZoneInfoIndex, count);

				TCHAR messagestr[256];
				sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3283), needmoney);	// ��û�� %d���� �ʿ��մϴ�. ��û�Ͻðڽ��ϱ�?

				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestr, TRUE, _XDEF_CASTLEINFO_REQWAR_YES, 0);
			}
		}
		break;
	case _XDEF_CASTLEINFO_REQWAR_YES :
		{
			int count = 0;
#ifdef _XTS_ALIANCE_20061018
			if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 )
			{
				count = g_pGroup_Window->m_listAlienceMember.size();
			}
#endif
			if(count <= 0)
			{
				if(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0)
				{
					count = 1;
				}
			}
			if(count > 0)
			{
				int needmoney = g_CBManager.GetChallengeMoney(g_CurrentZoneInfoIndex, count);

//#ifdef _XDEF_CASTLEBATTLE_SENDPACKET
				g_NetworkKernel.SendPacket(MSG_NO_CB_SCHEDULE, en_cb_req_challenge, needmoney);
				pCastleInfo_Window->EnableButton(FALSE);
//#endif
			}
		}
		break;
	case _XDEF_CASTLEINFO_CANCELWAR :
		{
			g_NetworkKernel.SendPacket(MSG_NO_CB_SCHEDULE, en_cb_challenge_cancel);
		}
		break;
	case _XDEF_CHALLENGERLIST_REQLIST :
		{
			// ������ ��� ��û
//#ifdef _XDEF_CASTLEBATTLE_SENDPACKET
			g_NetworkKernel.SendPacket(MSG_NO_CB_SCHEDULE, en_cb_req_challenge_list);
			pChallengerList_Window->EnableButton(FALSE);
//#endif
		}
		break;

		//����� ������ ����
	case _XDEF_CASTLEBATTLE_SHOUT_BTN_CLOSE		:
		{
			_XWindow_CastleBattleShout* pCastleBattleShout_Window = (_XWindow_CastleBattleShout*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_SHOUT);
			if(pCastleBattleShout_Window)
			{
				// ��ġ�⸦ ��� �Ҹ����� ��� â�� ���� �� ����
				if(pCastleBattleShout_Window->m_nCurrentCount == g_CBManager.GetShoutMaxCount())
				{
					pCastleBattleShout_Window->ShowWindow(FALSE);
				}
			}
		}
		break;
	case _XDEF_CASTLEBATTLE_SHOUT_BTN_SHOUT		:
		{
			_XWindow_CastleBattleShout* pCastleBattleShout_Window = (_XWindow_CastleBattleShout*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_SHOUT);
			if(pCastleBattleShout_Window)
			{
				//����� ������
				if(pCastleBattleShout_Window->m_nCurrentCount < g_CBManager.GetShoutMaxCount())
				{
					g_NetworkKernel.SendPacket(MSG_NO_COMBAT, en_cb_etc_loud_chat);
					// Ƚ���� �� ��Ŷ�� ���� �� ����
//					pCastleBattleShout_Window->m_nCurrentCount++;
				}

				if(pCastleBattleShout_Window->m_nCurrentCount >= g_CBManager.GetShoutMaxCount())
				{
					pCastleBattleShout_Window->m_pBtnShout->EnableWindow(FALSE);
				}
			}
		}
		break;

		// ����� �ð����� ����
	case _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_CLOSE			:
		{			
			_XWindow_CastleBattleTimeChange* pCastleBattleTimechange_Window = (_XWindow_CastleBattleTimeChange*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE);
			
			if(pCastleBattleTimechange_Window)
			{
				pCastleBattleTimechange_Window->ShowWindow(FALSE);			
			}
		}
		break;
	case _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_CHANGE			:
		{
			// ��Ŷ ���� �� �� �����ϱ�
			_XWindow_CastleBattleTimeChange* pCBTimechange_Window = (_XWindow_CastleBattleTimeChange*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE);
			if(pCBTimechange_Window)
			{
				g_NetworkKernel.SendPacket(MSG_NO_CB_SCHEDULE, en_cb_req_change_time, g_pLocalUser->m_nCastleMapId);
				pCBTimechange_Window->m_btnTimeChange->EnableWindow(FALSE);	//�������� ��Ŷ ������ ���� ���ؼ� 
			}

			//����� ���� ����� �ð� 
			//pCastleBattleTimechange_Window->m_tempChangeTime[0]
			
			//����� ���� ����� �ð� 
			//pCastleBattleTimechange_Window->m_tempChangeTime[1]
			
			//����� ���� ����� �ð� 
			//pCastleBattleTimechange_Window->m_tempChangeTime[2]

			// ��Ŷ�� ������.
			//m_btnTimeChange->EnableWindow(FALSE);	//�������� ��Ŷ ������ ���� ���ؼ� 
			

			// ��Ŷ ������ ���� �κп��� â�� �ݴ´�.???
			// ���� �޴� �κп��� �ð��� �����Ѵ�.
			//m_btnTimeChange->EnableWindow(TRUE);
//			for( int i = 0; i < 3; ++i )
//			{
//				m_ChangeTime[i] = m_tempChangeTime[i]; //������ ���� �ð� 
//			}

		}
		break;
	case _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_LISTDOWN1		:
		{			
			_XWindow_CastleBattleTimeChange* pCastleBattleTimechange_Window = (_XWindow_CastleBattleTimeChange*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE);
			
			if(pCastleBattleTimechange_Window)
			{
				if( !pCastleBattleTimechange_Window->m_listboxTimeChange1->GetShowStatus() )
				{
					pCastleBattleTimechange_Window->m_listboxTimeChange1->ShowWindow(TRUE);
					pCastleBattleTimechange_Window->m_listboxTimeChange2->ShowWindow(FALSE);
					pCastleBattleTimechange_Window->m_listboxTimeChange3->ShowWindow(FALSE);
				}
				else
				{
					pCastleBattleTimechange_Window->m_listboxTimeChange1->ShowWindow(FALSE);
					pCastleBattleTimechange_Window->m_listboxTimeChange2->ShowWindow(FALSE);
					pCastleBattleTimechange_Window->m_listboxTimeChange3->ShowWindow(FALSE);
				}
			}

		}
		break;
	case _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_LISTDOWN2		:
		{			
			_XWindow_CastleBattleTimeChange* pCastleBattleTimechange_Window = (_XWindow_CastleBattleTimeChange*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE);
			
			if(pCastleBattleTimechange_Window)
			{
				if( !pCastleBattleTimechange_Window->m_listboxTimeChange2->GetShowStatus() )
				{
					pCastleBattleTimechange_Window->m_listboxTimeChange1->ShowWindow(FALSE);
					pCastleBattleTimechange_Window->m_listboxTimeChange2->ShowWindow(TRUE);
					pCastleBattleTimechange_Window->m_listboxTimeChange3->ShowWindow(FALSE);
				}
				else
				{
					pCastleBattleTimechange_Window->m_listboxTimeChange1->ShowWindow(FALSE);
					pCastleBattleTimechange_Window->m_listboxTimeChange2->ShowWindow(FALSE);
					pCastleBattleTimechange_Window->m_listboxTimeChange3->ShowWindow(FALSE);
				}
			}
			
		}
		break;
	case _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_LISTDOWN3		:
		{			
			_XWindow_CastleBattleTimeChange* pCastleBattleTimechange_Window = (_XWindow_CastleBattleTimeChange*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE);
			
			if(pCastleBattleTimechange_Window)
			{
				if( !pCastleBattleTimechange_Window->m_listboxTimeChange3->GetShowStatus() )
				{
					pCastleBattleTimechange_Window->m_listboxTimeChange1->ShowWindow(FALSE);
					pCastleBattleTimechange_Window->m_listboxTimeChange2->ShowWindow(FALSE);
					pCastleBattleTimechange_Window->m_listboxTimeChange3->ShowWindow(TRUE);
				}
				else
				{
					pCastleBattleTimechange_Window->m_listboxTimeChange1->ShowWindow(FALSE);
					pCastleBattleTimechange_Window->m_listboxTimeChange2->ShowWindow(FALSE);
					pCastleBattleTimechange_Window->m_listboxTimeChange3->ShowWindow(FALSE);
				}
			}
			
		}
		break;
	case _XDEF_CASTLEBATTLE_TIMECHANGE_LISTBOX_TIME1		:
		{			
			_XWindow_CastleBattleTimeChange* pCastleBattleTimechange_Window = (_XWindow_CastleBattleTimeChange*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE);
			
			if(pCastleBattleTimechange_Window)
			{
				pCastleBattleTimechange_Window->m_tempChangeTime[0] = (int)lparam;

				pCastleBattleTimechange_Window->m_listboxTimeChange1->ShowWindow(FALSE);
				pCastleBattleTimechange_Window->m_listboxTimeChange2->ShowWindow(FALSE);
				pCastleBattleTimechange_Window->m_listboxTimeChange3->ShowWindow(FALSE);
			}
			
		}
		break;
	case _XDEF_CASTLEBATTLE_TIMECHANGE_LISTBOX_TIME2		:
		{			
			_XWindow_CastleBattleTimeChange* pCastleBattleTimechange_Window = (_XWindow_CastleBattleTimeChange*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE);
			
			if(pCastleBattleTimechange_Window)
			{
				pCastleBattleTimechange_Window->m_tempChangeTime[1] = (int)lparam;

				pCastleBattleTimechange_Window->m_listboxTimeChange1->ShowWindow(FALSE);
				pCastleBattleTimechange_Window->m_listboxTimeChange2->ShowWindow(FALSE);
				pCastleBattleTimechange_Window->m_listboxTimeChange3->ShowWindow(FALSE);
			}
			
		}
		break;
	case _XDEF_CASTLEBATTLE_TIMECHANGE_LISTBOX_TIME3		:
		{			
			_XWindow_CastleBattleTimeChange* pCastleBattleTimechange_Window = (_XWindow_CastleBattleTimeChange*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_TIMECHANGE);
			
			if(pCastleBattleTimechange_Window)
			{
				pCastleBattleTimechange_Window->m_tempChangeTime[2] = (int)lparam;

				pCastleBattleTimechange_Window->m_listboxTimeChange1->ShowWindow(FALSE);
				pCastleBattleTimechange_Window->m_listboxTimeChange2->ShowWindow(FALSE);
				pCastleBattleTimechange_Window->m_listboxTimeChange3->ShowWindow(FALSE);
			}
			
		}
		break;
	case _XDEF_CBRESULTNOTICE_CLOSE :
		{
			_XWindow_CBResultNotice* pCBResultNotice_Window = (_XWindow_CBResultNotice*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBRESULTNOTICE);
			if(pCBResultNotice_Window)
			{
				pCBResultNotice_Window->ShowWindow(FALSE);
			}
		}
		break;
	case _XDEF_CBORGRECORD_OK :
		{
			_XWindow_CBOrgRecord* pCBOrgRecord_Window = (_XWindow_CBOrgRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBORGRECORD);
			if(pCBOrgRecord_Window)
			{
				pCBOrgRecord_Window->ShowWindow(FALSE);
			}
		}
		break;
	case _XDEF_CASTLEBATTLE_REWARDLIST_BTN_OK :
		{
			_XWindow_CastleBattleRewardList* pCBRewardList_Window = (_XWindow_CastleBattleRewardList*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEBATTLE_REWARDLIST);
			if(pCBRewardList_Window)
			{
				pCBRewardList_Window->ShowWindow(FALSE);
			}
		}
		break;
	case _XDEF_CBREWARDSMALL_OK :
		{
			_XWindow_CBRewardSmall* pCBRewardSmall_Window = (_XWindow_CBRewardSmall*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBREWARDSMALL);
			if(pCBRewardSmall_Window)
			{
				pCBRewardSmall_Window->ShowWindow(FALSE);
			}
		}
		break;
	case _XDEF_CBPERSONAL_VIEWRECORD :
		{
			_XWindow_CBRecord* pCBRecord_Window = (_XWindow_CBRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CBRECORD);
			if(pCBRecord_Window)
			{
				if(pCBRecord_Window->GetShowStatus())
					pCBRecord_Window->ShowWindow(FALSE);
				else
					pCBRecord_Window->ShowWindow(TRUE);
			}
		}
		break;
	}
}


#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321 // PK����ġ����, ������ PK����	
void XProc_MainGame::PKTracingRequestWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
	if(!pPKTracingRequest_Window)
	{
		return;
	}
	
	//Author : ����� //breif : ��ġ ���� ����.. Ÿ�ϳ����� ���� �� ó�� //Date 08/03/06
	switch(wparam)
	{
	case _XDEF_PKTRACINGREQUEST_BTN_REQUEST1_YES	:
		{
			//Ư���� ��ġ ����
			if( m_pNPCScriptWindowPtr && m_pNPCScriptWindowPtr->GetShowStatus() ) 
			{
				if( m_pNPCScriptWindowPtr->m_MessageType == _XDEF_NPCMESSAGE_TRACING )
				{
					//if( strcmp( pPKTracingRequest_Window->m_cTargetName, "") == 0 )
					if( strcmp( pPKTracingRequest_Window->m_cSpecialTargetName, "") == 0 )
					//if( pPKTracingRequest_Window->m_cTargetName[0] == 0 ) 
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3378), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
						pPKTracingRequest_Window->ShowWindow(FALSE);
						break;
					}
					else if( g_pLocalUser->m_Money < 500000 )
					{
						TCHAR messagestr[256];
						memset( messagestr, 0, sizeof(TCHAR)*256 );
						sprintf( messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3379), 500000 );
						g_NetworkKernel.InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
						memset(pPKTracingRequest_Window->m_cSpecialTargetName, 0, sizeof(pPKTracingRequest_Window->m_cSpecialTargetName));
						pPKTracingRequest_Window->ShowWindow(FALSE);
						break;
					}
					else
					{
						g_NetworkKernel.SendPacket( MSG_NO_LTS, en_lts_sp_insert_req_cl_gs, 1 );	//1�ð�
						pPKTracingRequest_Window->ShowWindow(FALSE);
					}
				}
			}
			else//PK�� ��ġ ����
			{
				if( strcmp( pPKTracingRequest_Window->m_cTargetName, "") == 0 )
				//if( pPKTracingRequest_Window->m_cTargetName[0] == 0 ) 
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3378), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
					pPKTracingRequest_Window->ShowWindow(FALSE);
					break;
				}
				else if( g_pLocalUser->m_Money < 10000 )
				{
					TCHAR messagestr[256];
					memset( messagestr, 0, sizeof(TCHAR)*256 );
					sprintf( messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3379), 10000 );
					g_NetworkKernel.InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
					memset(pPKTracingRequest_Window->m_cTargetName, 0, sizeof(pPKTracingRequest_Window->m_cTargetName));// ����� PK�� �̸� 
					pPKTracingRequest_Window->ShowWindow(FALSE);
					break;
				}
				else
				{
					g_NetworkKernel.SendPacket( MSG_NO_LTS, en_lts_pk_insert_req_cl_gs, 1 );	//1�ð�
					pPKTracingRequest_Window->ShowWindow(FALSE);
				}
			}
		}
		break;
	case _XDEF_PKTRACINGREQUEST_BTN_REQUEST2_YES	:
		{
			//Ư���� ��ġ ����
			if( m_pNPCScriptWindowPtr && m_pNPCScriptWindowPtr->GetShowStatus() ) 
			{
				if( m_pNPCScriptWindowPtr->m_MessageType == _XDEF_NPCMESSAGE_TRACING )
				{
					//if( strcmp( pPKTracingRequest_Window->m_cTargetName, "") == 0 )
					if( strcmp( pPKTracingRequest_Window->m_cSpecialTargetName, "") == 0 )
					//if( pPKTracingRequest_Window->m_cTargetName[0] == 0 ) 
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3378), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
						pPKTracingRequest_Window->ShowWindow(FALSE);
						break;
					}
					else if( g_pLocalUser->m_Money < 3000000 )
					{
						TCHAR messagestr[256];
						memset( messagestr, 0, sizeof(TCHAR)*256 );
						sprintf( messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3379), 3000000 );
						g_NetworkKernel.InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
						memset(pPKTracingRequest_Window->m_cSpecialTargetName, 0, sizeof(pPKTracingRequest_Window->m_cSpecialTargetName));
						pPKTracingRequest_Window->ShowWindow(FALSE);
						break;
					}
					else
					{
						g_NetworkKernel.SendPacket( MSG_NO_LTS, en_lts_sp_insert_req_cl_gs, 2 );	//24�ð�
						pPKTracingRequest_Window->ShowWindow(FALSE);
					}
				}
			}
			else//PK�� ��ġ ����
			{
				if( strcmp( pPKTracingRequest_Window->m_cTargetName, "") == 0 )
				//if( pPKTracingRequest_Window->m_cTargetName[0] == 0 ) 
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3378), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
					pPKTracingRequest_Window->ShowWindow(FALSE);
					break;
				}
				else if( g_pLocalUser->m_Money < 300000 )
				{
					TCHAR messagestr[256];
					memset( messagestr, 0, sizeof(TCHAR)*256 );
					sprintf( messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3379), 300000 );
					g_NetworkKernel.InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
					memset(pPKTracingRequest_Window->m_cTargetName, 0, sizeof(pPKTracingRequest_Window->m_cTargetName));// ����� PK�� �̸� 
					pPKTracingRequest_Window->ShowWindow(FALSE);
					break;
				}
				else
				{
					g_NetworkKernel.SendPacket( MSG_NO_LTS, en_lts_pk_insert_req_cl_gs, 2 );	//24�ð�
					pPKTracingRequest_Window->ShowWindow(FALSE);
				}
			}
		}
		break;
	case _XDEF_PKTRACINGREQUEST_BTN_REQUEST3_YES	:
		{
			//Ư���� ��ġ ����
			if( m_pNPCScriptWindowPtr && m_pNPCScriptWindowPtr->GetShowStatus() ) 
			{
				if( m_pNPCScriptWindowPtr->m_MessageType == _XDEF_NPCMESSAGE_TRACING )
				{
					//if( strcmp( pPKTracingRequest_Window->m_cTargetName, "") == 0 )
					if( strcmp( pPKTracingRequest_Window->m_cSpecialTargetName, "") == 0 )
					//if( pPKTracingRequest_Window->m_cTargetName[0] == 0 ) 
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3378), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
						pPKTracingRequest_Window->ShowWindow(FALSE);
						break;
					}
					else if( g_pLocalUser->m_Money < 5000000 )
					{
						TCHAR messagestr[256];
						memset( messagestr, 0, sizeof(TCHAR)*256 );
						sprintf( messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3379), 5000000 );
						g_NetworkKernel.InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
						memset(pPKTracingRequest_Window->m_cSpecialTargetName, 0, sizeof(pPKTracingRequest_Window->m_cSpecialTargetName));
						pPKTracingRequest_Window->ShowWindow(FALSE);
						break;
					}
					else
					{
						g_NetworkKernel.SendPacket( MSG_NO_LTS, en_lts_sp_insert_req_cl_gs, 3 );	//48�ð�
						pPKTracingRequest_Window->ShowWindow(FALSE);
					}
				}
			}
			else//PK�� ��ġ ����
			{
				if( strcmp( pPKTracingRequest_Window->m_cTargetName, "") == 0 )
				//if( pPKTracingRequest_Window->m_cTargetName[0] == 0 ) 
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3378), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
					pPKTracingRequest_Window->ShowWindow(FALSE);
					break;
				}
				else if( g_pLocalUser->m_Money < 500000 )
				{
					TCHAR messagestr[256];
					memset( messagestr, 0, sizeof(TCHAR)*256 );
					sprintf( messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3379), 500000 );
					g_NetworkKernel.InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
					memset(pPKTracingRequest_Window->m_cTargetName, 0, sizeof(pPKTracingRequest_Window->m_cTargetName));// ����� PK�� �̸� 
					pPKTracingRequest_Window->ShowWindow(FALSE);
					break;
				}
				else
				{
					g_NetworkKernel.SendPacket( MSG_NO_LTS, en_lts_pk_insert_req_cl_gs, 3 );	//48�ð�
					pPKTracingRequest_Window->ShowWindow(FALSE);
				}
			}
		}
		break;
	case _XDEF_PKTRACINGREQUEST_BTN_REQUESTCANCE	:
		{
			pPKTracingRequest_Window->m_btnRequest1->EnableWindow(TRUE);
			pPKTracingRequest_Window->m_btnRequest2->EnableWindow(TRUE);
			pPKTracingRequest_Window->m_btnRequest3->EnableWindow(TRUE);
			pPKTracingRequest_Window->m_btnCancle->EnableWindow(TRUE);
		}
		break;
	case _XDEF_PKTRACINGREQUEST_BTN_REQUEST1	:
		{			
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(TCHAR)*256 );
			//Ư���� ��ġ ����
			if( m_pNPCScriptWindowPtr && m_pNPCScriptWindowPtr->GetShowStatus() && m_pNPCScriptWindowPtr->m_MessageType == _XDEF_NPCMESSAGE_TRACING) 
			{
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3380), 500000);	
			} 
			else
			{
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3380), 10000);	
			}
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_PKTRACINGREQUEST_BTN_REQUEST1_YES, _XDEF_PKTRACINGREQUEST_BTN_REQUESTCANCE);
		
			
			pPKTracingRequest_Window->m_btnRequest1->EnableWindow(FALSE);
			pPKTracingRequest_Window->m_btnRequest2->EnableWindow(FALSE);
			pPKTracingRequest_Window->m_btnRequest3->EnableWindow(FALSE);
			pPKTracingRequest_Window->m_btnCancle->EnableWindow(FALSE);
		}	
		break;
	case _XDEF_PKTRACINGREQUEST_BTN_REQUEST2	:
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(TCHAR)*256 );
			//Ư���� ��ġ ����
			if( m_pNPCScriptWindowPtr && m_pNPCScriptWindowPtr->GetShowStatus() && m_pNPCScriptWindowPtr->m_MessageType == _XDEF_NPCMESSAGE_TRACING) 
			{
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3380), 3000000);	
			} 
			else
			{
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3380), 300000);	
			}
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_PKTRACINGREQUEST_BTN_REQUEST2_YES, _XDEF_PKTRACINGREQUEST_BTN_REQUESTCANCE);
		
			pPKTracingRequest_Window->m_btnRequest1->EnableWindow(FALSE);
			pPKTracingRequest_Window->m_btnRequest2->EnableWindow(FALSE);
			pPKTracingRequest_Window->m_btnRequest3->EnableWindow(FALSE);
			pPKTracingRequest_Window->m_btnCancle->EnableWindow(FALSE);
		}	
		break;
	case _XDEF_PKTRACINGREQUEST_BTN_REQUEST3	:
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(TCHAR)*256 );
			//Ư���� ��ġ ����
			if( m_pNPCScriptWindowPtr && m_pNPCScriptWindowPtr->GetShowStatus() && m_pNPCScriptWindowPtr->m_MessageType == _XDEF_NPCMESSAGE_TRACING) 
			{
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3380), 5000000);	
			} 
			else
			{
				sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3380), 500000);	
			}
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_PKTRACINGREQUEST_BTN_REQUEST3_YES, _XDEF_PKTRACINGREQUEST_BTN_REQUESTCANCE);
		
			pPKTracingRequest_Window->m_btnRequest1->EnableWindow(FALSE);
			pPKTracingRequest_Window->m_btnRequest2->EnableWindow(FALSE);
			pPKTracingRequest_Window->m_btnRequest3->EnableWindow(FALSE);
			pPKTracingRequest_Window->m_btnCancle->EnableWindow(FALSE);
		}	
		break;
	case _XDEF_PKTRACINGREQUEST_BTN_CANCLE		:	
		{
			//Auhtor : ����� //breif : ������� �� ������ �ʱ�ȭ�� ��� �Ѵ�. ������ �װ� ���� �� �� �ֱ� ���� Date : 08/03/11
			pPKTracingRequest_Window->m_btnRequest1->EnableWindow(FALSE);
			pPKTracingRequest_Window->m_btnRequest2->EnableWindow(FALSE);
			pPKTracingRequest_Window->m_btnRequest3->EnableWindow(FALSE);
			pPKTracingRequest_Window->m_btnCancle->EnableWindow(FALSE);
			memset(pPKTracingRequest_Window->m_cTargetName, 0, sizeof(pPKTracingRequest_Window->m_cTargetName));

			pPKTracingRequest_Window->ShowWindow(FALSE);
		}	
		break;

	//Author : �����
	case _XDEF_PK_TRACINGSERVICES_BTN_CLOSE:
		{
#ifdef _XDEF_PK_TRACINGSERVICES_20070402
			XWindow_PK_ListBox* pPk_TracingServicesWindow = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
			
			if( pPk_TracingServicesWindow )
				pPk_TracingServicesWindow->ShowWindow(FALSE);
#endif
		}
		break;

	case _XDEF_PK_TRACINGSERVICES_LISTBOX :
		{
#ifdef _XDEF_PK_TRACINGSERVICES_20070402
			XWindow_PK_ListBox* pPk_TracingServicesWindow = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
			
			if( pPk_TracingServicesWindow )
			{
				if( pPk_TracingServicesWindow->GetSelectedItem() == (int)lparam )
				{
					if( g_pMiniTooltip_Window )
					{
						if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_PK_TRACINGSERVICES, 0, 0, (int)lparam ) )
						{
							if(g_pMiniTooltip_Window->GetShowStatus())
								g_pMiniTooltip_Window->ShowWindow( FALSE );
							else
								g_pMiniTooltip_Window->ShowWindow( TRUE );
						}
					}
				}
				else
				{
					pPk_TracingServicesWindow->SetSelectedItem( (int)lparam );

					if( g_pMiniTooltip_Window )
					{
						if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_PK_TRACINGSERVICES, 0, 0, (int)lparam ) )
							g_pMiniTooltip_Window->ShowWindow( TRUE );
					}
				}
			}
#endif //Last Updated : 07/04/02
		}
		break;
	}
}
#endif

#ifdef _XTS_ITEMMALLBROWSER
void XProc_MainGame::CashItemBoxWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
	if( !pCashBox_Window ) return;
	switch(wparam)
	{
	case _XDEF_CASHMALL_BTN_CLOSE:
		{
			_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
			if( pCashMall_Window )
			{
#ifdef _XVIETNAMESE				
				if( g_hItemMallWebBrowserObject )
				{
					//DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://www.cuulong.com.vn/ktc/logout.aspx");	
					DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://cuulongid.zing.vn/ktc/logout.aspx");	
				}
#endif
				
				pCashMall_Window->ShowWindow( FALSE );					
				ShowWindow(g_hItemMallWindow, SW_HIDE);
				g_bShowItemMallWindow = FALSE;
			}

			#ifdef _ACCLAIM_RUBICONADSYSTEM	
			if( g_ViewRubiconAD )
			{
				::ShowWindow(g_hRubiconADWindow, SW_SHOW);
			}
			#endif
		}
		break;
	case _XDEF_CASHBOX_BTN_CLOSE:
		{
			
			pCashBox_Window->ShowWindow( FALSE );
		
			
		}
		break;
	case _XDEF_CASHBOX_BTN_RENEW:
		{		
#ifdef _XDEF_COMMONITEMMALL_20070402	// ���� �����۸�
			g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,pCashBox_Window->m_CurrentPageNumber+1,9); // m_CurrentPageNumber�� 0 ����
#else
			if( pCashBox_Window->m_SelectedTab == _XCASHBOXTAB_PERSONAL)
			{
				//������ ��Ŷ ������ 
				g_NetworkKernel.SendPacket(MSG_NO_ITEM_SHOP,PKTMODE_PERSONAL_CASH_INVEN_REQ);
			}
			else if( pCashBox_Window->m_SelectedTab == _XCASHBOXTAB_PRESENT)
			{
				//������ ��Ŷ ������ 	
				g_NetworkKernel.SendPacket(MSG_NO_ITEM_SHOP,PKTMODE_PRESENT_CASH_INVEN_REQ);
			}		
#endif
//			pCashBox_Window->m_btnMoveAllItem->EnableWindow(FALSE);
			pCashBox_Window->m_btnMoveSelectedItem->EnableWindow(FALSE);
			pCashBox_Window->m_btnRenew->EnableWindow(FALSE);
			pCashBox_Window->m_TabButton[0]->EnableWindow(FALSE);
			pCashBox_Window->m_TabButton[1]->EnableWindow(FALSE);
	
			
		}
		break;
	case _XDEF_CASHBOX_BTN_MOVE:
		{	
			if( pCashBox_Window->m_listCashItem.empty() ) break;
			int count = 0;
			_XCASHITEMBOX_INFO* cashitem = NULL;
			list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
			for(iter_cashitem = pCashBox_Window->m_listCashItem.begin() ; iter_cashitem != pCashBox_Window->m_listCashItem.end() ; ++iter_cashitem)
			{
				cashitem = *iter_cashitem;
				if( cashitem )
				{
					if( cashitem->bCheck )
					{
						++count;
					}
				}
			}
			if( count != 0 )
			{
				//�ű�� ������ 10���� �ʰ��ϸ� �ȵȴ�.
				if( count > 10 )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2984),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );//�ѹ��� �ű� �� �ִ� �������� ������ 10���� ���ѵǾ� �ֽ��ϴ�. �ٽ� ������ �ֽʽÿ�.
				}
				else
				{
                    pCashBox_Window->m_btnMoveSelectedItem->EnableWindow(FALSE);
					pCashBox_Window->m_btnRenew->EnableWindow(FALSE);
					pCashBox_Window->m_TabButton[0]->EnableWindow(FALSE);
					pCashBox_Window->m_TabButton[1]->EnableWindow(FALSE);
					
#ifdef _XDEF_COMMONITEMMALL_20070402
					g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,15,count);
#else
					g_NetworkKernel.SendPacket(MSG_NO_ITEM_SHOP,PKTMODE_MOVE_PERSONAL_PACKAGE_REQ,count);
#endif
//							pCashBox_Window->m_btnMoveAllItem->EnableWindow(FALSE);
				}
				
			}
		
		}
		break;
	case _XDEF_CASHBOX_BTN_ALLMOVE:
		{
//			if( pCashBox_Window->m_listCashItem.empty() ) break;
//
//			int count = 0;
//			_XCASHITEMBOX_INFO* cashitem = NULL;
//			list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
//			for(iter_cashitem = pCashBox_Window->m_listCashItem.begin() ; iter_cashitem != pCashBox_Window->m_listCashItem.end() ; ++iter_cashitem)
//			{
//				cashitem = *iter_cashitem;
//				if( cashitem )
//				{
//					cashitem->bCheck = TRUE;
//					++count;
//				}
//			}
//
//			if( count != 0 )
//			{						
//				g_NetworkKernel.SendPacket(MSG_NO_ITEM_SHOP,PKTMODE_MOVE_PERSONAL_PACKAGE_REQ,count);
////						pCashBox_Window->m_btnMoveAllItem->EnableWindow(FALSE);
//				pCashBox_Window->m_btnMoveSelectedItem->EnableWindow(FALSE);
//				pCashBox_Window->m_btnRenew->EnableWindow(FALSE);
//				pCashBox_Window->m_TabButton[0]->EnableWindow(FALSE);
//				pCashBox_Window->m_TabButton[1]->EnableWindow(FALSE);
//			}
//				
		
			
		}
		break;
	case _XDEF_CASHBOX_BTN_MOVECASHMALL:
		{
			if( !g_bShowItemMallWindow )
 			{
 				_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
 				if( pCashMall_Window )
				{
					
 					pCashMall_Window->ShowWindow( TRUE );					
					g_MainWindowManager.SetTopWindow( pCashMall_Window );
 					/*POINT pt = pCashMall_Window->GetWindowPos();
 					RECT rect;
 					GetWindowRect( gHWnd, &rect );*/
					
#if defined(_XDEF_ITEMMALL_AUTOLOGIN_20061120) || defined(_XDEF_ITEMMALL_AUTOLOGIN_SE_20070514)

					int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
					
	#ifdef _XDEF_ITEMMALL_AUTOLOGIN_SE_20070514
					TCHAR postdata[128];			
					sprintf( postdata, "AL_ID=%s&AL_PWD=%s&AL_Server=%d", g_pLocalUser->m_ItemMallWebLoginID, g_pLocalUser->m_UserPW, servernumber );
					_XDisplayHTMLPageAndPostData(g_hItemMallWebBrowserObject, _T("http://global1.indy21.co.kr:8080"), postdata );
	#else
					TCHAR tempStr[512];
					memset( tempStr, 0, sizeof(TCHAR)*512 );
					
					if( !g_bTestWebPageAddress )
						sprintf( tempStr, "http://ninemall.nexon.com/?user_id=%s&select_server=%d", g_pLocalUser->m_ItemMallWebLoginID, servernumber );
					else
						sprintf( tempStr, "http://panda.nexon.com/?user_id=%s&select_server=%d", g_pLocalUser->m_ItemMallWebLoginID, servernumber );
					
					DisplayHTMLPage(g_hItemMallWebBrowserObject, tempStr);
	#endif
					
#endif
					POINT pt = { 0, 0 };
					ClientToScreen( gHWnd, &pt );
					if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
					{						
						TCHAR tempStr[512];
						memset( tempStr, 0, sizeof(TCHAR)*512 );
						int lastconnectedservernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
						//sprintf( tempStr, "http://ninetest.gameleon.jp/item_mall/item_login.asp?user_id=%s&select_server=%d", g_pLocalUser->m_UserName, lastconnectedservernumber );
						sprintf( tempStr, "http://nine.gameleon.jp/item_mall/item_login.asp?user_id=%s&select_server=%d", g_pLocalUser->m_UserName, lastconnectedservernumber );

						DisplayHTMLPage(g_hItemMallWebBrowserObject, tempStr);
						SetWindowPos(g_hItemMallWindow, /*HWND_TOPMOST*/NULL, pt.x + (gnWidth>>1)-400, pt.y + (gnHeight>>1)-300, 800, 600, 0);
					}
					else
					{
						#ifdef _ACCLAIM_RUBICONADSYSTEM	
						if( g_ViewRubiconAD )
						{
							::ShowWindow(g_hRubiconADWindow, SW_HIDE);
						}
						#endif
						
						SetWindowPos(g_hItemMallWindow, HWND_TOPMOST, pt.x + (gnWidth>>1)-400, pt.y + (gnHeight>>1)-300, 800, 600, 0);
					}
 					ShowWindow(g_hItemMallWindow, SW_SHOW);
					g_bShowItemMallWindow = TRUE;
					g_MainWindowManager.SetTopWindow( pCashMall_Window );
					
										
					_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
					if( pCashBox_Window )
					{
						pCashBox_Window->ShowWindow(FALSE);
					}
 				}		
 			}
 			else
 			{
 				_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
 				if( pCashMall_Window )
 				{
 					pCashMall_Window->ShowWindow( FALSE );
					
 					ShowWindow(g_hItemMallWindow, SW_HIDE);
 					g_bShowItemMallWindow = FALSE;
 				}		

				#ifdef _ACCLAIM_RUBICONADSYSTEM	
				if( g_ViewRubiconAD )
				{
					::ShowWindow(g_hRubiconADWindow, SW_SHOW);
				}
				#endif
 			}
		}
		break;
	case _XDEF_CASHBOX_LISTBOX_CASHITEM:
		{
			
			int selecteditem =  (int)lparam;
			
			_XCASHITEMBOX_INFO* cashitem = NULL;
			list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
			for(iter_cashitem = pCashBox_Window->m_listCashItem.begin() ; iter_cashitem != pCashBox_Window->m_listCashItem.end() ; ++iter_cashitem)
			{
				cashitem = *iter_cashitem;
				if( cashitem )
				{
					if( cashitem->indexBox == selecteditem )
					{
						cashitem->bCheck = !cashitem->bCheck;
					
						int optionmenuinterface = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
						RECT rect2;
						rect2.left = 216;
						rect2.right = 230;
						rect2.top = 0;
						rect2.bottom = 14;
			
						if( cashitem->bCheck )
						{
							pCashBox_Window->m_listboxCashItem->SetIconArchive(selecteditem-1, &g_MainInterfaceTextureArchive);
							pCashBox_Window->m_listboxCashItem->SetItemAttrib(selecteditem-1, 1, optionmenuinterface, rect2);
						}
						else
						{
							pCashBox_Window->m_listboxCashItem->SetIconArchive(selecteditem-1, &g_MainInterfaceTextureArchive);
							pCashBox_Window->m_listboxCashItem->SetItemAttrib(selecteditem-1, 1, -1, rect2);
						}
					}
				}
			}
					
		}
		break;
	case _XDEF_CASHBOX_TABBUTTON_PERSONAL:
		{
			pCashBox_Window->ChangeTab(_XCASHBOXTAB_PERSONAL);

//					pCashBox_Window->m_btnMoveAllItem->EnableWindow(FALSE);
			pCashBox_Window->m_btnMoveSelectedItem->EnableWindow(FALSE);
			pCashBox_Window->m_btnRenew->EnableWindow(FALSE);
			//pCashBox_Window->m_TabButton[0]->EnableWindow(FALSE); //Author :����� //breif : �̰� ���� �ǵ������� ���Ҵµ�.. ĳ�� �κ� Ŭ�� //Date : 08/03/26
			//pCashBox_Window->m_TabButton[1]->EnableWindow(FALSE);
			
			
#ifdef _XDEF_COMMONITEMMALL_20070402	
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);
#endif
		}
		break;
	case _XDEF_CASHBOX_TABBUTTON_PRESENT:
		{
			pCashBox_Window->ChangeTab(_XCASHBOXTAB_PRESENT);
			
//					pCashBox_Window->m_btnMoveAllItem->EnableWindow(FALSE);
			pCashBox_Window->m_btnMoveSelectedItem->EnableWindow(FALSE);
			pCashBox_Window->m_btnRenew->EnableWindow(FALSE);
			//pCashBox_Window->m_TabButton[0]->EnableWindow(FALSE); //Author :����� //breif : �̰� ���� �ǵ������� ���Ҵµ�.. ĳ�� �κ� Ŭ�� //Date : 08/03/26
			//pCashBox_Window->m_TabButton[1]->EnableWindow(FALSE);
			
#ifdef _XDEF_COMMONITEMMALL_20070402
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);
#endif
		}
		break;
#ifdef _XDEF_COMMONITEMMALL_20070402
	case _XDEF_CASHBOX_BTN_FIRST			:
		{
			//pCashBox_Window->m_Current5PageCount = 0;
			
			pCashBox_Window->m_CurrentPageNumber = 0; //Author : ����� //breif : ó������ ��ư //Date : 08/03/28
			g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,1,9);
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);
		}
		break;
	case _XDEF_CASHBOX_BTN_END			:
		{
			int nTotalpage = pCashBox_Window->m_nCashItemTotalCount/9;
			if( pCashBox_Window->m_nCashItemTotalCount > 0 )
			{
				if( pCashBox_Window->m_nCashItemTotalCount%9 != 0 )
					nTotalpage += 1;
			}
			
			pCashBox_Window->m_CurrentPageNumber = nTotalpage; //Author : ����� //breif : ������ ��ư //Date : 08/03/28
			g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,pCashBox_Window->m_CurrentPageNumber + 1,9); //�������� ������ 1 �� ����
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);
			
			/*if( pCashBox_Window->m_Maxium5PageCount == 0 )
			{
				pCashBox_Window->m_Current5PageCount = 0;
				int page = (pCashBox_Window->m_nCashItemTotalCount/9)+1;
				g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,page,9);
			}
			else
			{
				pCashBox_Window->m_Current5PageCount = (pCashBox_Window->m_Maxium5PageCount) ;
				int page = ((pCashBox_Window->m_nCashItemTotalCount - pCashBox_Window->m_Maxium5PageCount*5*9)/9) +1;
				g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,page+(pCashBox_Window->m_Current5PageCount*5),9);
			}
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);*/
		}
		break;
	case _XDEF_CASHBOX_BTN_ADJACENT		:
		{
			//Auhtor :����� /breif : ���� ������
			pCashBox_Window->m_CurrentPageNumber--;

			if( pCashBox_Window->m_CurrentPageNumber < 0 ) // 0���� ������ 1������
				pCashBox_Window->m_CurrentPageNumber = 0;

			g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,pCashBox_Window->m_CurrentPageNumber+1,9); 
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);
			
			/*if( pCashBox_Window->m_Current5PageCount > 1)
			{
				--pCashBox_Window->m_Current5PageCount;
				g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,5+(pCashBox_Window->m_Current5PageCount*5),9);
			}
			else
			{
//				pCashBox_Window->m_Current5PageCount = 0;
//				//int page = (pCashBox_Window->m_nCashItemTotalCount/9)+1;
//				g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,1,9);
			}
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);*/
			
		}
		break;
	case _XDEF_CASHBOX_BTN_NEXT			:
		{
			int nTotalpage = pCashBox_Window->m_nCashItemTotalCount/9;
			if( pCashBox_Window->m_nCashItemTotalCount > 0 )
			{
				if( pCashBox_Window->m_nCashItemTotalCount%9 != 0 )
					nTotalpage += 1;
			}
			//nTotalpage �� 1 �� ����

			//m_CurrentPageNumber+1 ���� �������� �������� nTotalpage ��ü ������ ���� �۾ƾ� ������������ �Ѱ��ش�.
			//ex) ĳ���κ� ���� 2������(m_CurrentPageNumber+1 == 2)   ��ü ������ nTotalpage == 2
			//	  �׷��Ƿ� �������� �������� ����
			if( (pCashBox_Window->m_CurrentPageNumber+1) < nTotalpage ) 
				pCashBox_Window->m_CurrentPageNumber++; //m_CurrentPageNumber 0 �� ����

			g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,pCashBox_Window->m_CurrentPageNumber+1,9); 
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);
			/*if( pCashBox_Window->m_Maxium5PageCount > 0)
			{
				if( pCashBox_Window->m_Current5PageCount < pCashBox_Window->m_Maxium5PageCount && pCashBox_Window->m_Current5PageCount > 0)
				{
					++pCashBox_Window->m_Current5PageCount;
					g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,1+(pCashBox_Window->m_Current5PageCount*5),9);
				}
				else if( pCashBox_Window->m_Current5PageCount == pCashBox_Window->m_Maxium5PageCount )
				{
					pCashBox_Window->m_Current5PageCount = pCashBox_Window->m_Maxium5PageCount;
				}
			}
			else
			{
//				pCashBox_Window->m_Current5PageCount = 0;
//				int page = (pCashBox_Window->m_nCashItemTotalCount/9)+1;
//				g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,page,9);
			}
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);*/
		}
		break;
	case _XDEF_CASHBOX_BTN_PAGE1			:
		{
			//Author : ����� //breif : ������ Ŭ�� ��� ���� Date : 08/03/07
			if( pCashBox_Window->m_listCashItem.empty() ) break;
			
			int nStartPage = 5*pCashBox_Window->m_Current5PageCount; //���� �������� �������� �ΰ� ( 0���� )
			pCashBox_Window->m_CurrentPageNumber = nStartPage; //ù��°������ �̹Ƿ� �״�� �ִ´�

			g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,pCashBox_Window->m_CurrentPageNumber+1,9); // ���� 0 �����̹Ƿ� + 1
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);
		}
		break;
	case _XDEF_CASHBOX_BTN_PAGE2			:
		{
			if( pCashBox_Window->m_listCashItem.empty() ) break;
			//��Ŷ ������ 	

			int nStartPage = 5*pCashBox_Window->m_Current5PageCount; //���� �������� �������� �ΰ� ( 0���� )
			pCashBox_Window->m_CurrentPageNumber = nStartPage + 1; //�ι�°

			g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,pCashBox_Window->m_CurrentPageNumber+1,9);
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);
		}
		break;
	case _XDEF_CASHBOX_BTN_PAGE3			:
		{
			if( pCashBox_Window->m_listCashItem.empty() ) break;
			//��Ŷ ������ 	

			int nStartPage = 5*pCashBox_Window->m_Current5PageCount; //���� �������� �������� �ΰ� ( 0���� )
			pCashBox_Window->m_CurrentPageNumber = nStartPage + 2; //������

			g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,pCashBox_Window->m_CurrentPageNumber+1,9);
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);
		}
		break;
	case _XDEF_CASHBOX_BTN_PAGE4			:
		{
			if( pCashBox_Window->m_listCashItem.empty() ) break;
			//��Ŷ ������ 	
			int nStartPage = 5*pCashBox_Window->m_Current5PageCount; //���� �������� �������� �ΰ� ( 0���� )
			pCashBox_Window->m_CurrentPageNumber = nStartPage + 3; 

			g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,pCashBox_Window->m_CurrentPageNumber+1,9);
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);
		}
		break;
	case _XDEF_CASHBOX_BTN_PAGE5			:	
		{
			if( pCashBox_Window->m_listCashItem.empty() ) break;
			//��Ŷ ������ 	
			int nStartPage = 5*pCashBox_Window->m_Current5PageCount; //���� �������� �������� �ΰ� ( 0���� )
			pCashBox_Window->m_CurrentPageNumber = nStartPage + 4; 

			g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,pCashBox_Window->m_CurrentPageNumber+1,9);
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);
		}
		break;	
	case _XDEF_CASHBOX_BTN_MSG1			:
		{
			pCashBox_Window->SendPresentMessageReq(1);
		}
		break;
	case _XDEF_CASHBOX_BTN_MSG2			:
		{
			pCashBox_Window->SendPresentMessageReq(2);
		}
		break;
	case _XDEF_CASHBOX_BTN_MSG3			:
		{
			pCashBox_Window->SendPresentMessageReq(3);
		}
		break;
	case _XDEF_CASHBOX_BTN_MSG4			:
		{
			pCashBox_Window->SendPresentMessageReq(4);
		}
		break;
	case _XDEF_CASHBOX_BTN_MSG5			:
		{
			pCashBox_Window->SendPresentMessageReq(5);
		}
		break;
	case _XDEF_CASHBOX_BTN_MSG6			:
		{
			pCashBox_Window->SendPresentMessageReq(6);
		}
		break;
	case _XDEF_CASHBOX_BTN_MSG7			:
		{
			pCashBox_Window->SendPresentMessageReq(7);
		}
		break;
	case _XDEF_CASHBOX_BTN_MSG8			:
		{
			pCashBox_Window->SendPresentMessageReq(8);
		}
		break;
	case _XDEF_CASHBOX_BTN_MSG9			:
		{
			pCashBox_Window->SendPresentMessageReq(9);
		}
		break;
	case _XDEF_CASHBOX_BTN_MSGCLOSE		:
		{
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);
		}
		break;
#endif//#ifdef _XDEF_COMMONITEMMALL_20070402
	}
}
#endif

//Author : �����
void XProc_MainGame::SmeltItemWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
#ifdef _XDEF_SMELT_ITEM
	XWindow_SmeltItem* pSmeltItem_Window = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
	if( !pSmeltItem_Window ) return;

	switch(wparam) 
	{
	case _XDEF_SMELT_CONFIRMBUTTON :
		{
			switch( pSmeltItem_Window->GetItemStep() )
			{
			case ITEMSTEP_NONE :
				break;
			case ITEMSTEP_READY:
				pSmeltItem_Window->SetItemFlag( pSmeltItem_Window->GetItemFlag(), ITEMSTEP_CONFIRM);
				break;
			case ITEMSTEP_CONFIRM :
				pSmeltItem_Window->SetItemFlag( pSmeltItem_Window->GetItemFlag(), ITEMSTEP_PROGRESS);
				break;
			case ITEMSTEP_PROGRESS:
				break;	
			}
			
		}
		break;
	case _XDEF_SMELT_CANCLEBUTTON :
		{
			switch( pSmeltItem_Window->GetItemStep() )
			{
			case ITEMSTEP_NONE :
				pSmeltItem_Window->ShowWindow(FALSE);
				break;
			case ITEMSTEP_READY:
				pSmeltItem_Window->SetItemFlag( pSmeltItem_Window->GetItemFlag(), ITEMSTEP_NONE);
				break;
			case ITEMSTEP_CONFIRM :
				pSmeltItem_Window->SetItemFlag( pSmeltItem_Window->GetItemFlag(), ITEMSTEP_READY);
				break;
			case ITEMSTEP_PROGRESS:
				pSmeltItem_Window->SetItemFlag( pSmeltItem_Window->GetItemFlag(), ITEMSTEP_CONFIRM );
				break;	
			}
		}
		break;
	case _XDEF_SMELT_CLOSEBUTTON :
		{

		}
		break;
	case _XDEF_SMELT_PERIODITEM_YES :
		{
			pSmeltItem_Window->ProcessSmelt();
		}
		break;
	case _XDEF_SMELT_PERIODITEM_NO :
		{
			// �ʱ�ȭ
			pSmeltItem_Window->SetUserItemindex(0);

			switch( pSmeltItem_Window->GetItemStep() )
			{
			case ITEMSTEP_NONE :
				pSmeltItem_Window->ShowWindow(FALSE);
				break;
			case ITEMSTEP_READY:
				pSmeltItem_Window->SetItemFlag( pSmeltItem_Window->GetItemFlag(), ITEMSTEP_NONE);
				break;
			case ITEMSTEP_CONFIRM :
				pSmeltItem_Window->SetItemFlag( pSmeltItem_Window->GetItemFlag(), ITEMSTEP_READY);
				break;
			case ITEMSTEP_PROGRESS:
				pSmeltItem_Window->SetItemFlag( pSmeltItem_Window->GetItemFlag(), ITEMSTEP_CONFIRM );
				break;	
			}
		}
		break;
	}
#endif
}
//Last Updated :  07/04/17

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503

void XProc_MainGame::BlackListWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_BlackList* pBlackList_Window = (_XWindow_BlackList*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_BLACKLISTWINDOW );

	if( !pBlackList_Window ) return;

	switch( wparam )
	{
	case _XDEF_BLACKLISTWINDOW_CLOSEBUTTON				:
		if( pBlackList_Window )
		{
			_XWindow_ChattingDefault* defaultchatwindow = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );			
			defaultchatwindow->m_pChatBlackListButton->SetCheck(FALSE);
			pBlackList_Window->ShowWindow( FALSE );
		}
		break;
	case _XDEF_BLACKLISTWINDOW_HELPBUTTON				:
		break;
	case _XDEF_MESSENGERWINDOW_ADDBLACKLISTBUTTON		:
		{
			//g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_LOGINSERVER_EXITGAME, TRUE, _XDEF_MESSENGERWINDOW_ADD_MESSAGEBOX );
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_INPUTSTRING, ID_STRING_NEW_3474, TRUE, _XDEF_MESSENGERWINDOW_ADD_MESSAGEBOX );//_T("������Ʈ�� �߰��� ����� �̸��� �Է��Ͻʽÿ�.")
			g_MessageBox.m_IMEControl.SetMaxLength( 63 );
		}
		break;
	case _XDEF_MESSENGERWINDOW_DELETEBLACKLISTBUTTON	:
		{
			if(pBlackList_Window->m_listboxBlackList)
			{
				LPTSTR pSelectedCharacterName = pBlackList_Window->m_listboxBlackList->GetItemText( pBlackList_Window->m_listboxBlackList->GetSelectedItem(), 1 );
				if(pSelectedCharacterName)
				{
					TCHAR message[512];
					sprintf( message, _XGETINTERFACETEXT(ID_STRING_NEW_3475), pSelectedCharacterName ); //("%s���� ������Ʈ���� ���� �Ͻðڽ��ϱ�?")
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, _T(""),message, TRUE, _XDEF_MESSENGERWINDOW_DELETE_MESSAGEBOX );
				}
			}
		}
		break;
	case _XDEF_MESSENGERWINDOW_BLACKLISTBOX				:
		{

		}
		break;
	case _XDEF_MESSENGERWINDOW_ADD_MESSAGEBOX			:
		{
			int length = strlen( g_MessageBox.m_IMEControl.GetText() );
			if( length <= 0 )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NEW_3476, TRUE ); //_T("ĳ���� �̸��� �־��ּ���.")
			}
			else
			{
				if( pBlackList_Window->FindCharacter(g_MessageBox.m_IMEControl.GetText()) )
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NEW_3477, TRUE );//"�̹� ������Ʈ�� �����ϴ� ĳ���� �Դϴ�."
				}
				else
				{
					pBlackList_Window->InsertCharacter( g_MessageBox.m_IMEControl.GetText() );
					pBlackList_Window->RebuildListBox();
					pBlackList_Window->SaveList( g_pLocalUser->m_UserName );
					TCHAR message[512];
					sprintf( message, _XGETINTERFACETEXT(ID_STRING_NEW_3478), g_MessageBox.m_IMEControl.GetText() );//_T("%s���� ������Ʈ�� �߰��Ǿ����ϴ�.")
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
				}
			}
		}
		break;
	case _XDEF_MESSENGERWINDOW_DELETE_MESSAGEBOX		:
		{
			if(pBlackList_Window->m_listboxBlackList)
			{
				pBlackList_Window->DeleteCharacter( pBlackList_Window->m_listboxBlackList->GetSelectedItem() );
				pBlackList_Window->RebuildListBox();
				pBlackList_Window->SaveList( g_pLocalUser->m_UserName );
			}
		}
		break;
	}
}

#endif

void XProc_MainGame::NewWorldMapWindowMessageHandling(WPARAM wparam, LPARAM lparam )
{
#ifdef _XDEF_NEW_WORLDMAP_070528_MAGVIPER //Author : ����� //breif : �ű�������
	XWindow_WorldMap* pWorldMapWindow = (XWindow_WorldMap*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WORLDMAP);
	if( !pWorldMapWindow ) return;

	switch( wparam )
	{
	case _XDEF_WORLDMAP_STARTBUTTON :
		{
			if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
			{
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
				
				g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_2419), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
				_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_2419) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				break;
			}

			if( pWorldMapWindow->SetShowWindowProcess(!pWorldMapWindow->GetShowStatus()) ) //�Ѱ� ����
				WindowCloseBeforeWideView();
			else
				BaseInterfaceWindowOpen();

			//pWorldMapWindow->ShowWindow(!pWorldMapWindow->GetShowStatus()); //�Ѱ� ����

			/*if(pWorldMapWindow->GetShowStatus())
				WindowCloseBeforeWideView();
			else
				BaseInterfaceWindowOpen();*/
		}
		break;
	case _XDEF_WORLDMAP_CLOSEBUTTON:
		{
			//pWorldMapWindow->ShowWindow(FALSE); //����
			pWorldMapWindow->SetShowWindowProcess( FALSE );
			BaseInterfaceWindowOpen();
		}
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_01 :
		pWorldMapWindow->SetKey(0);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_02 :
		pWorldMapWindow->SetKey(1);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_03 :
		pWorldMapWindow->SetKey(2);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_04 :
		pWorldMapWindow->SetKey(3);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_05 :
		pWorldMapWindow->SetKey(4);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_06 :
		pWorldMapWindow->SetKey(5);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_07 :
		pWorldMapWindow->SetKey(6);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_08 :
		pWorldMapWindow->SetKey(7);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_09 :
		pWorldMapWindow->SetKey(8);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_10 :
		pWorldMapWindow->SetKey(9);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_11 :
		pWorldMapWindow->SetKey(10);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_12 :
		pWorldMapWindow->SetKey(11);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_13 :
		pWorldMapWindow->SetKey(12);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_14 :
		pWorldMapWindow->SetKey(13);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_15 :
		pWorldMapWindow->SetKey(14);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_16 :
		pWorldMapWindow->SetKey(15);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_17 :
		pWorldMapWindow->SetKey(16);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_18 :
		pWorldMapWindow->SetKey(17);
		break;
	case _XDEF_WORLDMAP_MAPBUTTON_19 :
		pWorldMapWindow->SetKey(18);
		break;
	}

#endif 
}

void XProc_MainGame::RecallItemWIndowMessageHandling( WPARAM wparam, LPARAM lparam ) //Author : ����� , ģ����ȯ ����
{
#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER
	_XWindow_RecallItem* pRecallItemWindow = (_XWindow_RecallItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_RECALLITEM_WINDOW);
	if( !pRecallItemWindow ) return;

	g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_MESSAGEWINDOW);

	switch( wparam )
	{
	case _XDEF_RECALLITEM_LISTBOX :
		{
			pRecallItemWindow->SetSelectedItem( (int)lparam );
		}
		break;
	case _XDEF_RECALLITEM_CALLBUTTON :
		{
			TCHAR tempString[128];
			memset(tempString, 0, sizeof(tempString));

			if( pRecallItemWindow->GetSelectedItem() == 0 ) //����Ʈ���� ���ÿ���
			{
				sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3531)); //ģ���� ���õ��� �ʾҽ��ϴ�.
				g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				break;
			}
			else if( pRecallItemWindow->CharRecallCondition() )  //���� �� ����
			{
				pRecallItemWindow->SetRecallIndex( pRecallItemWindow->GetSelectedItem() );// ���� ������ ��ȣ�� ����

				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3532),//_T("ȣ���� �Ͻø� ��ȯ�� 1���� �Ҹ�˴ϴ�. ȣ�� �Ͻðڽ��ϱ�?")
										TRUE, _XDEF_RECALLITEM_CALLBUTTON_YES );
				pMessageBox->StartTimer( 15000, TRUE );
			}
			else //pRecallItemWindow->CharRecallCondition() FALSE �϶�
			{
				sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3533) ); //"������ ȣ���� �� ���� ���� �Դϴ�.
				g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			}
		}
		break;
	case _XDEF_RECALLITEM_CLOSEBUTTON :
	case _XDEF_RECALLITEM_MINICLOSEBUTTON :
		{
			if(pRecallItemWindow->GetShowStatus())
				pRecallItemWindow->ShowWindow(FALSE);
		}
		break;
	case _XDEF_RECALLITEM_CALLBUTTON_YES : //Yes �� ���� �Ұ��..
		{
			//���԰˻縦 �ѹ� ���� //â��� �ű� �� ��������
			if(!pRecallItemWindow->SearchSlotItem( _XGI_SC_POTION_RECALL ))
			{
				TCHAR tempString[128];
				memset(tempString, 0, sizeof(tempString));

				sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3534) ); //"��ȯ���� ���� �մϴ�."
				g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				break;
			}

			//ȣ���û..��ȯ ��ư�� ��Ȱ��ȭ
			pRecallItemWindow->SetRecallButton( FALSE );

			g_NetworkKernel.SendPacket( MSG_NO_RECALL_FRIEND, en_rf_send_request_recall ); //ȣ���û ��Ŷ
		}
		break;
	case _XDEF_RECALLITEM_CALL_AGREE : //ȣ��䱸 ���� //�̵��Ͽ� �������� ����Ʈ.. //ȣ������̴� ��������
		{
            //��Ŷ ���������� �ѹ� �� �˻�
            //�� ���¸� �˻�����

            TCHAR tempString[128];
            memset(tempString, 0, sizeof(tempString));

			if( !pRecallItemWindow->CharRecallCondition() ) 
			{
				sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3562), pRecallItemWindow->GetFromName(), _XGETINTERFACETEXT(ID_STRING_NEW_3489) ); //�����̻����� %s���� %s��û�� ���� �մϴ�.   , ȣ��
				g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );

				//ȣ�� ���� �� ���� ����
				g_NetworkKernel.SendPacket( MSG_NO_RECALL_FRIEND, en_rf_send_answer_no );
				break;
			}

			//ģ���� ��ϵ��ִ��� Ȯ������..
			if(!pRecallItemWindow->CheckCharacterName( pRecallItemWindow->GetFromName() ))
			{
				sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3563), pRecallItemWindow->GetFromName(), _XGETINTERFACETEXT(ID_STRING_NEW_3489) ); // %s���� ģ����Ͽ� �����Ƿ� %s��û�� ���� �մϴ�.   , ȣ��
				g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );

				//ģ��������� �ȵǾ�����
				g_NetworkKernel.SendPacket( MSG_NO_RECALL_FRIEND, en_rf_send_answer_no );
				break;
			}

			//����Ʈ 
			D3DXMATRIX matWorld;
			D3DXMatrixIdentity(&matWorld);

			//ĳ����, ���̵�, ���
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_warp_call2, &g_pLocalUser->m_ModelDescriptor.m_Position );

			//����
			_XPlayInterfaceSound(ID_SR_INTERFACE_RECALL_WAV);

			g_NetworkKernel.SendPacket( MSG_NO_RECALL_FRIEND, en_rf_send_answer_yes ); //������ ���� ȣ�������� ����
		}
		break;
	case _XDEF_RECALLITEM_CALL_REJECT : //ȣ��䱸 ����
		{
			TCHAR tempString[128];
			memset(tempString, 0, sizeof(tempString));

			pRecallItemWindow->SetCharState(0); //������ 0 
			sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3535) );// "ȣ���� �����Ͽ����ϴ�.
			g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );

			//���� ��Ŷ
			g_NetworkKernel.SendPacket( MSG_NO_RECALL_FRIEND, en_rf_send_answer_no );
		}
		break;
	//�̵��� ����..............
	case _XDEF_RECALLITEM_MOVEBUTTON :
		{
			//�̵��ҋ�
			TCHAR tempString[128];
			memset(tempString, 0, sizeof(tempString));

			if( pRecallItemWindow->GetSelectedItem() == 0 )
			{
				sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3531) ); //"ģ���� ���õ��� �ʾҽ��ϴ�."
				g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				break;
			}

			else if( pRecallItemWindow->CharRecallCondition() ) 
			{
				// ���� ������ ����Ʈ ��ȣ�� ���� //���⼭�� �̵��� ����Ʈ��ȣ�� ����..
				pRecallItemWindow->SetRecallIndex( pRecallItemWindow->GetSelectedItem() );

				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3536), //_T("�̵��� �Ͻø� ��ȯ�� 1���� �Ҹ�˴ϴ�. �̵� �Ͻðڽ��ϱ�?"), 
										TRUE, _XDEF_RECALLITEM_MOVEBUTTON_YES );
				pMessageBox->StartTimer( 15000, TRUE );
			}
			else //pRecallItemWindow->CharRecallCondition() FALSE �϶�
			{
				sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3537) );//"���濡�� �̵��� �� ���� ���� �Դϴ�.
				g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			}
		}
		break;
	case _XDEF_RECALLITEM_MOVEBUTTON_YES :
		{
			//���԰˻縦 �ѹ� ���� //â��� �ű� �� ��������
			if(!pRecallItemWindow->SearchSlotItem( _XGI_SC_POTION_RECALL ))
			{
				TCHAR tempString[128];
				memset(tempString, 0, sizeof(tempString));

				sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3534) ); //��ȯ���� ���� �մϴ�.
				g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				break;
			}

			//��ȯ��ư ��Ȱ��ȭ
			pRecallItemWindow->SetRecallButton( FALSE );

			g_NetworkKernel.SendPacket( MSG_NO_RECALL_FRIEND, en_rf_send_request_goto ); //�̵���û ��Ŷ
		}
		break;
	case _XDEF_RECALLITEM_MOVE_AGREE : //�̵����� //������ ������ �̵��� ��û�ؿ°� ���� �Ͽ����� �������� �����
		{
			//����Ʈ
			D3DXMATRIX matWorld;
			D3DXMatrixIdentity(&matWorld);
			matWorld._43 = -2.0f;
			
			D3DXMATRIX matrotation;
			D3DXMatrixRotationY( &matrotation, g_pLocalUser->m_RotateAngle );

			D3DXMatrixMultiply( &matWorld, &matWorld, &matrotation );

			matWorld._41 += g_pLocalUser->m_Position.x;
			matWorld._42 += g_pLocalUser->m_Position.y;
			matWorld._43 += g_pLocalUser->m_Position.z;			

			//ĳ����, ���̵�, ���
			//g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_warp_call, &g_pLocalUser->m_ModelDescriptor.m_Position );
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_warp_call, &matWorld );

			//����
			_XPlayInterfaceSound(ID_SR_INTERFACE_RECALL_WAV);

			g_NetworkKernel.SendPacket( MSG_NO_RECALL_FRIEND, en_rf_send_goto_answer_yes ); //������ �̵� �϶�..
		}
		break;
	case _XDEF_RECALLITEM_MOVE_REJECT : //�̵�����
		{
			TCHAR tempString[128];
			memset(tempString, 0, sizeof(tempString));

			pRecallItemWindow->SetCharState(0); //������ 0 
			sprintf(tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3539) );//�̵���û�� ���� �Ͽ����ϴ�.
			g_NetworkKernel.InsertChatString( tempString, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );

			//���� ��Ŷ
			g_NetworkKernel.SendPacket( MSG_NO_RECALL_FRIEND, en_rf_send_goto_answer_no );
		}
		break;
	}
#endif
}

void XProc_MainGame::SocketDetachWIndowMessageHandling( WPARAM wparam, LPARAM lparam ) //Author : ����� , ���� ����
{
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER
	_XWindow_SocketDetach* pSocketDetachWindow = (_XWindow_SocketDetach*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETDETACH_WINDOW);
	
	if( !pSocketDetachWindow )
		return;

	TCHAR messagestring[128];
	memset(messagestring, 0, sizeof(messagestring));

	switch( wparam )
	{
	case _XDEF_SOCKETDETACH_PROCESSBUTTON :
		{
			g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
				
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3678)); //�����Ͻ� ��Ĺ��� �и��Ͻðڽ��ϱ�?
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO,  _T(""), messagestring, TRUE, _XDEF_SOCKETDETACH_YESBUTTON);	
			pMessageBox->StartTimer( 15000, TRUE );
		}
		break;
	case _XDEF_SOCKETDETACH_YESBUTTON :
		{
			_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
			if( pChargeGauge_Window )
			{
				if(!pChargeGauge_Window->GetShowStatus())
				{
					pChargeGauge_Window->SetDelayTime( 4500.0f );
					pChargeGauge_Window->SetGaugeMode( _XGAUGE_SOCKETDEATCH );
					pChargeGauge_Window->ShowWindow(TRUE);
					g_MainWindowManager.SetTopWindow(pChargeGauge_Window);					
				}
			}
			
			pSocketDetachWindow->SetEnableProcessButton( FALSE );
			pSocketDetachWindow->SetLock( TRUE );

			//g_NetworkKernel.SendPacket( MSG_NO_ITEM_SOCKET_GAMBLE, en_socket_gamble_mode_separate_socket ); //���Ϻи� ��û
		}
		break;
	case _XDEF_SOCKETDETACH_CANCEL :
	case _XDEF_SOCKETDETACH_CLOSE :
		{
			pSocketDetachWindow->ReBuild( MODE_NONE );
			pSocketDetachWindow->ShowWindow(FALSE);
		}
		break;
	case _XDEF_SOCKETDETACH_HELP :
		break;
	}
#endif
}

void XProc_MainGame::MixWeaponWindowMessageHandling(WPARAM wparam, LPARAM lparam)
{
	_XWindow_MixWeapon* pMixWeapon_Window = (_XWindow_MixWeapon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MIXWEAPON_WINDOW);
	if(!pMixWeapon_Window)
		return;

	switch( wparam )
	{
	case _XDEF_MIXWEAPON_OK :
		{
			g_NetworkKernel.SendPacket(MSG_NO_INCHANT_CTRL, inchant_mode_combine_item_req);
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_CLOSE, 0), 0);
		}
		break;
	case _XDEF_MIXWEAPON_CANCEL :
		{
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_CLOSE, 0), 0);
		}
		break;
	}
}
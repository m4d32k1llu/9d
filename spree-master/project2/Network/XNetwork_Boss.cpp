// XNetwork_Boss.cpp : ������ ����
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "XWindow_EnterBossRoom.h"
#include "XWindow_JinStatus.h"
#include "XWindow_SystemMsgDefault.h"
#include "XMessageWindow.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_Poison.h"


extern void ChangeModel(DWORD lparam, char firstType, char facemaskmode = 0 );

BOOL _XNetwork::Send_MSGBoss(int type, int param1, int param2, int param3)
{
	switch(type)
	{
	case MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S : // ������ ���� ��û
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Creat Boss Area");
			
			int size = sizeof(MSG_EX_BATTLE_ROOM_CREATE_REQ);
			MSG_EX_BATTLE_ROOM_CREATE_REQ creatbossarea;
			ZeroMemory(&creatbossarea, size);
			creatbossarea.header.sLength = size;
			creatbossarea.ucMessage = MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S;

			_XWindow_EnterBossRoom* pEnterBossRoom_Window = (_XWindow_EnterBossRoom*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOM);			
			if( pEnterBossRoom_Window )
			{
				creatbossarea.cLevel = pEnterBossRoom_Window->m_BossRoomLevel;// ���߿� �����ۿ� ���� ���̵� ����

#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
				if(pEnterBossRoom_Window->m_ItemInSlot[0].bInsert)
				{
					// cash ���
					creatbossarea.cTicket = 1;
					creatbossarea.cEnterItemSlotNum[0] = pEnterBossRoom_Window->m_ItemInSlot[0].nInvenNumber;
					creatbossarea.cEnterItemSlotNum[1] = 0;
					creatbossarea.cEnterItemSlotNum[2] = 0;
				}
				else
				{
					// �Ϲ� ������ ���
					creatbossarea.cTicket = 0;
					for(int i = 1 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; ++i)
					{
						creatbossarea.cEnterItemSlotNum[i-1] = pEnterBossRoom_Window->m_ItemInSlot[i].nInvenNumber;
					}
				}

				// ���� �ʱ�ȭ
				pEnterBossRoom_Window->ResetData();

#else
				for(int i = 0 ; i < _XDEF_BOSSROOM_NEEDITEMNUMBER ; ++i)
				{
					creatbossarea.cEnterItemSlotNum[i] = pEnterBossRoom_Window->m_NeedItem[i].m_InventoryNumber;
				}
				pEnterBossRoom_Window->ResetNeedItemInfo();
#endif

				pEnterBossRoom_Window->ShowWindow(FALSE);
				g_pInventory_Window->ShowWindow(FALSE);
			}
												
			_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );
			if( defaultsystemmsgwindow )
			{
				defaultsystemmsgwindow->m_btnFunctionalObjectViewMode->ShowWindow(FALSE);
			}
			
			creatbossarea.cType = (char)param1;
			if( g_pLocalUser->m_InParty )
			{
				creatbossarea.uMode = 2;
			}
			else
			{
				creatbossarea.uMode = 1;
			}
			
			return _XSendPacket_Encapsulate( (char*)&creatbossarea );
		}
		break;
		
	case MSG_NO_EX_BATTLE_ENTER_REQ_C_S : // �ʴ���� ĳ���Ͱ� �����濡 �����Ҷ�..
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Enter Boss Area");
			
			int size = sizeof(MSG_EX_BATTLE_ENTER);
			MSG_EX_BATTLE_ENTER enterbossarea;
			ZeroMemory(&enterbossarea, size);
			enterbossarea.header.sLength = size;
			enterbossarea.ucMessage = MSG_NO_EX_BATTLE_ENTER_REQ_C_S;
			
			enterbossarea.iRoomID	= g_pLocalUser->m_BossRoomID;

			g_pLocalUser->m_BossRoomStageNumber = 1;// ����� 1�ܰ� ���� 
			return _XSendPacket_Encapsulate( (char*)&enterbossarea );
		}
		break;
		
	case MSG_NO_EX_BATTLE_LEAVE_REQ_C_S : // �濡 �ִ� ĳ���Ͱ� ������..
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Leave Boss Area");
			
			int size = sizeof(MSG_EX_BATTLE_LEAVE);
			MSG_EX_BATTLE_LEAVE leavebossarea;
			ZeroMemory(&leavebossarea, size);
			leavebossarea.header.sLength = size;
			leavebossarea.ucMessage = MSG_NO_EX_BATTLE_LEAVE_REQ_C_S;
			
			leavebossarea.cCause = (char)param1;			
			
			g_pLocalUser->m_bSuccessBossBattle = TRUE;			
			
			return _XSendPacket_Encapsulate( (char*)&leavebossarea );
		}
		break;
		
		
	case MSG_NO_EX_BATTLE_PLAY_VIDEO : // �÷��� ���� ��ŵ�ϰų� ����մϴ�
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Skip Movie ");
			
			int size = sizeof(MSG_EX_BATTLE_PLAY_VIDEO);
			MSG_EX_BATTLE_PLAY_VIDEO bossskipmovie;
			ZeroMemory(&bossskipmovie, size);
			bossskipmovie.header.sLength = size;
			bossskipmovie.ucMessage = MSG_NO_EX_BATTLE_PLAY_VIDEO;
			
			bossskipmovie.uCommand = (char)param1;
			
			return _XSendPacket_Encapsulate( (char*)&bossskipmovie );			
		}
		break;
		
	case MSG_NO_EX_BATTLE_TYPE_INFO_REQ_C_S : // �� Ÿ�Կ� ���� ������ ���´�
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Boss Room Type ");
			
			int size = sizeof(MSG_EX_BATTLE_TYPE_INFO_REQ);
			MSG_EX_BATTLE_TYPE_INFO_REQ bossroomtype;
			ZeroMemory(&bossroomtype, size);
			bossroomtype.header.sLength = size;
			bossroomtype.ucMessage = MSG_NO_EX_BATTLE_TYPE_INFO_REQ_C_S;
			
			// �����ʹ�ȣ
			bossroomtype.cType = (char)param1;
			
			_XWindow_EnterBossRoom* pEnterBossRoom_Window = (_XWindow_EnterBossRoom*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOM);			
			if( pEnterBossRoom_Window )
			{
				pEnterBossRoom_Window->SetSlotPosition(static_cast<int>(param1));
			}
			
			return _XSendPacket_Encapsulate( (char*)&bossroomtype );
		}
		break;
		
	}
	
	return TRUE;
}

void _XNetwork::MSGBossInviate(MSG_EX_BATTLE_INVITE* bossinvite)
{
	//������ �������� �ݴ´�.
	if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_PERSONALSTORE_CANCLEBUTTON, 0), (LPARAM)gHWnd);
	}
	
	g_ChannelTempZoneInfoIndex = g_CurrentZoneInfoIndex; // �������� �鰡�� ���� ��� ������ ����Ѵ�.
	g_CurrentZoneInfoIndex = bossinvite->cType;
	
	g_pLocalUser->m_NextRespawnPosition.x		= bossinvite->fX;
	g_pLocalUser->m_NextRespawnPosition.y		= 0.0f;
	g_pLocalUser->m_NextRespawnPosition.z		= bossinvite->fZ;
	g_pLocalUser->m_TargetPosition				= g_pLocalUser->m_NextRespawnPosition;
	g_pLocalUser->m_BossRoomID					= bossinvite->iRoomID;
	g_pLocalUser->m_bIsBossBattle				= TRUE;
	g_pLocalUser->m_bWaitBossBattle			    = FALSE;

	((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer = TRUE;
	((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MovingGameServer = TRUE;
	((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LockMoveProcess = TRUE;	
	g_LodTerrain.m_bInvisiblePortalMouseView = TRUE;// ��Ż������ ����Ʈ �����͵� ���´�.		
	
	SendPacket(MSG_NO_ZONE_MOVE);
	((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_loading);
	
}


void _XNetwork::MSGBossResult(MSG_EX_BATTLE_RESULT* bossresult)
{
	
/*============================================================================

  MSG_NO_EX_BATTLE_RESULT_S_C ����

	ucCommand : � ��Ŷ�� ���� ��� ��Ŷ���� �����ϱ� ����..
	iCode	  : ���� �Ǵ� ���� �ڵ��̴�..
	
  --------------------------------------------+-----------------------------------+---------------------------
  ucCommand								|	iCode							|		��
  --------------------------------------------+-----------------------------------+---------------------------
  MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S	|	_RESULT_CODE_SUCCESS			|	�ѽ�~ ����
									    |	_RESULT_CODE_NOTCASE			|	���� �Ҹ���
										|	_RESULT_CODE_ERROR				|	���̻� �� ���� �Ұ�
										|	_RESULT_CODE_ALREADY_PARTY		|	��Ƽ�� �̹� ���������̴�.											
  --------------------------------------------+-----------------------------+---------------------------------
  MSG_NO_EX_BATTLE_ENTER_REQ_C_S		|	_RESULT_CODE_SUCCESS			|	�ѽ�~ ����
										|	_RESULT_CODE_NOTCASE			|	���� �Ҹ���
										|	_RESULT_CODE_ERROR				|	��Ÿ ����
  --------------------------------------------+-----------------------------+---------------------------------
  MSG_NO_EX_BATTLE_LEAVE_REQ_C_S		|	_RESULT_CODE_SUCCESS			|	�ѽ�~ ����
										|	_RESULT_CODE_NOTCASE			|	���� (�̶� ��¼��?  ��.��)
										|	_RESULT_CODE_ERROR				|	��Ÿ ����
  --------------------------------------------+-----------------------------+---------------------------------
  MSG_NO_EX_BATTLE_TYPE_INFO_REQ_C_S	|	_RESULT_CODE_ALREADY_PARTY		|	��Ƽ�� �̹� ���������̴�.
  --------------------------------------------+-----------------------------+---------------------------------


=============================================================================*/

	switch( bossresult->ucCommand ) {
	case MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S:
		{			
			switch(bossresult->iCode) {
			case _RESULT_CODE_SUCCESS:
				{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
					for(int i = 0 ; i < 3 ; ++i)
					{
						if(bossresult->_sUsedItem[i].itemSlotNum >= _XINVENTORY_SLOTTYPE_POCKETSTART)
						{
							if(bossresult->_sUsedItem[i].itemCount == 0)
							{
								g_pLocalUser->m_UserItemList[bossresult->_sUsedItem[i].itemSlotNum].Reset();
							}
							else
							{
								g_pLocalUser->m_UserItemList[bossresult->_sUsedItem[i].itemSlotNum].Set_m_ucCount(bossresult->_sUsedItem[i].itemCount);
							}
						}
					}
#endif
				}
				break;
			case _RESULT_CODE_NOTCASE:
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKBOSS_1619), TRUE);
					pMessageBox->StartTimer(10000, TRUE);	
				}
				break;
			case _RESULT_CODE_ERROR:
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKBOSS_1619), TRUE);
					pMessageBox->StartTimer(10000, TRUE);	
					
				}
				break;
			case _RESULT_CODE_ALREADY_PARTY:
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKBOSS_1620), TRUE);
					pMessageBox->StartTimer(10000, TRUE);	
				}
				break;
			}
		}
		break;

	case MSG_NO_EX_BATTLE_ENTER_REQ_C_S:
		{
			switch(bossresult->iCode) {
			case _RESULT_CODE_SUCCESS:
				{
					g_pLocalUser->m_bExitBossBattleRoom = FALSE; // �������� ������ ������ ���� ������.
					g_LodTerrain.m_bInvisiblePortalMouseView = TRUE;// ��Ż������ ����Ʈ �����͵� ���´�.
					
					g_ViewPoisonFog = TRUE;	// ������ �Ѹ���.
					
//#ifdef _XTESTSERVER //Author : ����� //breif : 08/01/15 �׽�Ʈ �������� ���� �Ǵ��� ����
					if( g_CurrentZoneInfoIndex == 50 || g_CurrentZoneInfoIndex == 51)
					{
						// �����, ���հ� ���� �÷���
						g_indexScene = 24;
						if( g_SceneList[g_indexScene ] )
						{
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
							((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_BOSS;
							((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
						}
					}
					else if( g_CurrentZoneInfoIndex == 52 || g_CurrentZoneInfoIndex == 53)
					{
						// �����, ���հ� ���� �÷���
						g_indexScene = 45;
						if( g_SceneList[g_indexScene ] )
						{
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
							((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_BOSS;
							((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
						}
					}

/*#else
					if( g_CurrentZoneInfoIndex >= 50 && g_CurrentZoneInfoIndex < 54)
					{
						// �����, ���հ� ���� �÷���
						g_indexScene = 24;
						if( g_SceneList[g_indexScene ] )
						{
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
							((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_BOSS;
							((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
						}
					}

#endif*/
					else if( g_CurrentZoneInfoIndex == 55 )
					{
						// ��â ���� �÷���
						g_indexScene = 35;
						if( g_SceneList[g_indexScene ] )
						{
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
							((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_BOSS;
							((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
						}
					}
					else if( g_CurrentZoneInfoIndex == 57 )
					{
						// ���� ���� �÷���
						g_indexScene = 44;
						if( g_SceneList[g_indexScene ] )
						{
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
							((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_BOSS;
							((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
						}
					}
					
				}
				break;
			case _RESULT_CODE_NOTCASE:
			case _RESULT_CODE_ERROR:
			case _RESULT_CODE_ALREADY_PARTY:
				ReleaseNetwork();
				break;
			}
		}
		break;

	case MSG_NO_EX_BATTLE_LEAVE_REQ_C_S:
		{ 
			switch(bossresult->iCode) {
			case _RESULT_CODE_SUCCESS:
				{
					g_pLocalUser->m_bSuccessBossBattle = TRUE;
					g_LodTerrain.m_bInvisiblePortalMouseView = FALSE;
					g_pLocalUser->m_bExitBossBattleRoom = FALSE;
					g_ScrrenMotionBlurMode = FALSE;
					
					g_LodTerrain.m_3PCamera.SetCameraShakeMode( FALSE );					
				}
				break;
			case _RESULT_CODE_NOTCASE:
				break;
			case _RESULT_CODE_ERROR:
				break;
			}
		}
		break;
	case MSG_NO_EX_BATTLE_TYPE_INFO_REQ_C_S:
		{ 
			switch(bossresult->iCode) {
			case _RESULT_CODE_ALREADY_PARTY:
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKBOSS_1620), TRUE);
					pMessageBox->StartTimer(10000, TRUE);	
				}
				break;
			}
		}
		break;
	}
}

void _XNetwork::MSGBossPlayMovie(MSG_EX_BATTLE_PLAY_VIDEO* bossplaymovie)
{
	switch(bossplaymovie->uCommand) {
	case _SKIP_VIDEO_:		
		{			
//			if( g_pLocalUser->m_InParty )
//			{
//				bool checkleader = FALSE;
//				for(int i = 0 ; i < 9 ; i++)
//				{
//					if( g_pLocalUser->m_PartyInfo[i].isLeader )
//					{
//						if( strcmp(g_pLocalUser->m_PartyInfo[i].playername, g_pLocalUser->m_CharacterInfo.charactername) == 0) 
//						{
//							checkleader = TRUE;										
//						}
//						break;				 
//					} 
//				}
//				if( !checkleader)
//				{
//					if( ((CEmperorOfDragonsApp*)gApp)->m_proc_flash.flashWin->IsPlaying() )
//					{
//						((CEmperorOfDragonsApp*)gApp)->m_proc_flash.flashWin->ControlClose();
//						((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ResumeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_maingame); 
//					}
//				}
//			}
		}
		break;
	case _PLAY_VIDEO_:
		{
			//bossplaymovie->uStep //���� ��ȣ�� �ٲ� ����
			if( bossplaymovie->uStep != 100)
			{
				g_indexScene = bossplaymovie->uStep;
				if( g_SceneList[g_indexScene ] )
				{
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_BOSS;
					((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
				}
			}
			else //100���̸� ������ �ܰ�
			{				
				g_LodTerrain.m_bInvisiblePortalMouseView = FALSE;
				g_pLocalUser->m_bExitBossBattleRoom = TRUE;
				g_ScrrenMotionBlurMode = TRUE;
			
				g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, 99999.0f, TRUE );
			}
		}
		break;
	}
	
}


void _XNetwork::MSGBossTimer(MSG_EX_BATTLE_TIME* bosstimer)
{	
//	_XLog("RECEIVE MESSAGE : EX_BATTLE_TIME");

	g_pLocalUser->m_BossStartTime = g_LocalSystemTime;
	g_pLocalUser->m_BossTimeLimit = bosstimer->usTimeCount - 5;
	
	if( g_pJinStatus_Window)
	{
		if( g_pLocalUser->m_BossRoomStageNumber == 1 )
		{
			g_pJinStatus_Window->SetBossroomPoisonTimer(TRUE);

			_XWindow_Poison* pPoison_Window = (_XWindow_Poison*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POISON_WINDOW);
			if(pPoison_Window)
			{
			//	_XLog("Poison window mode == boss");
				pPoison_Window->SetMode(_MODE_BOSS);
				pPoison_Window->ShowWindow(TRUE);
			}
		}
		else
		{
			g_pJinStatus_Window->SetBossroomPoisonTimer(FALSE);

			_XWindow_Poison* pPoison_Window = (_XWindow_Poison*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POISON_WINDOW);
			if(pPoison_Window)
			{
				pPoison_Window->SetMode(_MODE_NONE);
				pPoison_Window->ShowWindow(FALSE);
			}
		}
	}	
}


void _XNetwork::MSGBossRoomType(MSG_EX_BATTLE_TYPE_INFO* bossroomtpye)
{
	_XWindow_EnterBossRoom* pEnterBossRoom_Window = (_XWindow_EnterBossRoom*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOM);
	
	if( pEnterBossRoom_Window )
	{
		for( int i = 0; i < _XDEF_BOSSROOM_NEEDITEMTYPE ; ++i )
		{
#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
			pEnterBossRoom_Window->SetNeedNormalItemRef(i, bossroomtpye->_sItem[i].cType, bossroomtpye->_sItem[i].sID);

			if(bossroomtpye->isTicket == 1)
			{
				pEnterBossRoom_Window->SetNeedCashItemRef(i, bossroomtpye->_sChargedItem[i].cType, bossroomtpye->_sChargedItem[i].sID);
			}
#else
			pEnterBossRoom_Window->m_NeedItemID[i].sID = bossroomtpye->_sItem[i].sID;
			pEnterBossRoom_Window->m_NeedItemID[i].cType = bossroomtpye->_sItem[i].cType;
#endif
			TCHAR messagestring[256];
			memset(messagestring, 0, sizeof(messagestring));
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORKBOSS_1621), _XGameItem::GetItemName(bossroomtpye->_sItem[i].cType, bossroomtpye->_sItem[i].sID) );
			InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		}

		pEnterBossRoom_Window->ShowWindow(TRUE);
		pEnterBossRoom_Window->m_OKButton->ShowWindow(FALSE);
		g_pInventory_Window->MoveWindow( 740, 170 );
		g_pInventory_Window->ShowWindow(TRUE);
		g_pInventory_Window->SetEnterBossRoomWindowPtr(pEnterBossRoom_Window);
		g_LodTerrain.m_bInvisiblePortalMouseView = TRUE;// ��Ż������ ����Ʈ �����͵� ���´�.
		
		_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );
		defaultsystemmsgwindow->m_btnFunctionalObjectViewMode->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORKBOSS_1622);
	}
	
	
}

#define _BOSS_STAGE_TEST_

void _XNetwork::MSGBossStageNumber(MSG_EX_BATTLE_CUR_STAGE* bossstagenumber)
{
	//bossstagenumber->cCurStage
	g_pLocalUser->m_BossRoomStageNumber = bossstagenumber->cCurStage;
	switch(bossstagenumber->cCurStage) {
	case 0:
		{
		}
		break;
	case 1:
		{
			g_ViewPoisonFog = TRUE;
		}
		break;
	case 2:
		{
#ifdef _BOSS_STAGE_TEST_
			// zone index == 54�� ��� stage7 ���� ���� ���ֱ�
			if(g_CurrentZoneInfoIndex != 54)
#endif
			{
				g_DeleteStartViewPoisonFog = TRUE;
			}
		}
		break;
	case 3:
		{
		}
		break;
	case 4:
		{
		}
		break;
	case 5:
		{
		}
	case 8 :
		{
#ifdef _BOSS_STAGE_TEST_
			// zone index == 54�� ��� stage8 ���� ���� ���ֱ�
			if(g_CurrentZoneInfoIndex == 54)
				g_DeleteStartViewPoisonFog = TRUE;
#endif
		}
		break;
	}
}


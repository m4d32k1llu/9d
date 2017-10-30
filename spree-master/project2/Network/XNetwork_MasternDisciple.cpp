#include "stdafx.h"
#include "XMnDStructInfo.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "XwindowObjectDefine.h"
#include "SoundEffectList.h"
#include "XMessageWindow.h"
#include "XParticleidDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_MessengerDialog.h"
#include "XWindow_MnDGroup.h" // Author : ����� //breif : �������� ��
#include <algorithm>

BOOL _XNetwork::Send_MnDSystem(int type, int param1, int param2, int param3, int param4)
{
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER		//Author : ����� //breif : ���� �ý���
	switch( param1 )
	{
	case en_cg_mp_mode_info_req : // ���� ���� ���� ��û. �ֺ����� ������.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Master and Disciple info");

			int size = sizeof(MSG_MP_INFO_REQ);
			MSG_MP_INFO_REQ mndreq;
			ZeroMemory(&mndreq, size);
			mndreq.header.sLength = size;
			mndreq.ucMessage = MSG_NO_MASTER_AND_PUPIL;
			mndreq.ucMP_Mode = param1;

			return _XSendPacket_Encapsulate((char*)&mndreq);
		}
		break;
	case en_cg_mp_mode_change_master_nick_req : // ��ȣ ���� ��û.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Master and Disciple change nick name");

			int size = sizeof(MSG_MP_CHANGE_MASTER_NICK_REQ);
			MSG_MP_CHANGE_MASTER_NICK_REQ mndreq;
			ZeroMemory(&mndreq, size);
			mndreq.header.sLength = size;
			mndreq.ucMessage = MSG_NO_MASTER_AND_PUPIL;
			mndreq.ucMP_Mode = param1;

			strcpy( mndreq.cNick, g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName );

			return _XSendPacket_Encapsulate((char*)&mndreq);
		}
		break;
	case en_cg_mp_mode_cg_apply_for_pupil: // // ���� ��û�� GS �� ���� : ���ڰ� �Ǳ� ���ϴ� ĳ���Ͱ� ����.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Master and Disciple apply disciple");

			int size = sizeof(MSG_MP_CG_APPLY_FOR_PUPIL);
			MSG_MP_CG_APPLY_FOR_PUPIL mndreq;
			ZeroMemory(&mndreq, size);
			mndreq.header.sLength = size;
			mndreq.ucMessage = MSG_NO_MASTER_AND_PUPIL;
			mndreq.ucMP_Mode = param1;

			return _XSendPacket_Encapsulate((char*)&mndreq);
		}
		break;
	case en_cg_mp_mdoe_answer_apply_from_master : // ���� : ���� ��û�� ���� ����.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Master and Disciple apply master");

			int size = sizeof(MSG_MP_ANSWER_APPLY_FROM_MASTER);
			MSG_MP_ANSWER_APPLY_FROM_MASTER mndreq;
			ZeroMemory(&mndreq, size);
			mndreq.header.sLength = size;
			mndreq.ucMessage = MSG_NO_MASTER_AND_PUPIL;
			mndreq.ucMP_Mode = param1;

			//param2 �� ������ ID�� �޾Ƽ� ó������
			//_XWindow_MessengerDialog* pMessengerDialog_Window = (_XWindow_MessengerDialog*)g_MainWindowManager.FindWindow(windowid);

			return _XSendPacket_Encapsulate((char*)&mndreq);
		}
		break;
	case en_cg_mp_mode_strike_pupil_req : // ���� �Ĺ� ��û
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Master and Disciple strike disciple");

			int size = sizeof(MSG_MP_STRIKE_PUPIL_REQ);
			MSG_MP_STRIKE_PUPIL_REQ mndreq;
			ZeroMemory(&mndreq, size);
			mndreq.header.sLength = size;
			mndreq.ucMessage = MSG_NO_MASTER_AND_PUPIL;
			mndreq.ucMP_Mode = param1;

			return _XSendPacket_Encapsulate((char*)&mndreq);
		}
		break;
	case en_cg_mp_mode_leave_req :	// �ϻ� ��û.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Master and Disciple leave");

			int size = sizeof(MSG_MP_LEAVE_REQ);
			MSG_MP_LEAVE_REQ mndreq;
			ZeroMemory(&mndreq, size);
			mndreq.header.sLength = size;
			mndreq.ucMessage = MSG_NO_MASTER_AND_PUPIL;
			mndreq.ucMP_Mode = param1;

			return _XSendPacket_Encapsulate((char*)&mndreq);
		}
		break;

	}
#endif

	return TRUE;
}

void _XNetwork::MSGMasternDiscipleResult(MSG_MASTER_AND_PUPIL *pMnDResult)
{
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER		//Author : ����� //breif : ���� �ý���

//#ifdef _XDWDEBUG
	_XLog("���� ��Ŷ �޾��� ��� %d", pMnDResult->ucMP_Mode );
//#endif

	switch(pMnDResult->ucMP_Mode)
	{
	case en_cg_mp_mode_no_mp_info :  // ���� ���谡 ����.
		{
			g_pGroup_Window->m_pMnDWindow->ResetData();
			memset(&g_pLocalUser->m_pLocalMnDInfomation, 0, sizeof(g_pLocalUser->m_pLocalMnDInfomation));
		}
		break;
	case en_cg_mp_mode_info_my_master : // ���� ���� ����. �ʵ� ���Խ� ����.
		{
			MSG_MP_INFO_MY_MASTER* pInfoMyMaster = (MSG_MP_INFO_MY_MASTER*)pMnDResult;

			g_pLocalUser->m_pLocalMnDInfomation.nMasterIndex = pInfoMyMaster->iMasterIndex; //������ ĳ���� �ε��� ��
			g_pLocalUser->m_pLocalMnDInfomation.nMasterRank = pInfoMyMaster->ucMasterRank; //������ ����
			g_pLocalUser->m_pLocalMnDInfomation.nTotalKillCount = pInfoMyMaster->uiTotalKillCount; //�� ų ��
			g_pLocalUser->m_pLocalMnDInfomation.nMasterTotalContribution = pInfoMyMaster->uiMasterTotalPoint;//���� ���� ����Ʈ
			g_pLocalUser->m_pLocalMnDInfomation.nMasterContribution = pInfoMyMaster->uiMasterCurPoint;//������ ���� ����Ʈ
			g_pLocalUser->m_pLocalMnDInfomation.nMasterConnectState = pInfoMyMaster->ucMasterConnectState; //������ ���� ���� ����
			g_pLocalUser->m_pLocalMnDInfomation.nBonusSkillStep = pInfoMyMaster->ucBonusSkillStep; // ��ȣ ȿ�� ��ų??
			
			g_pLocalUser->m_pLocalMnDInfomation.nSex = pInfoMyMaster->ucSex;
			g_pLocalUser->m_pLocalMnDInfomation.nLevel = pInfoMyMaster->ucLevel;              
			g_pLocalUser->m_pLocalMnDInfomation.nMoonpa = pInfoMyMaster->ucMoonpa;
			g_pLocalUser->m_pLocalMnDInfomation.nClass = pInfoMyMaster->ucClass;
			g_pLocalUser->m_pLocalMnDInfomation.nClassGrade = pInfoMyMaster->ucClassGrade;

			_XLog("�ʵ� ���� ���� �̸� %s\n �ʵ� ���� ��ȣ %s\n", pInfoMyMaster->cMasterName, pInfoMyMaster->cMasterNick );

			//������ �̸�
			if( strlen(pInfoMyMaster->cMasterName) )
			{
				memset(g_pLocalUser->m_pLocalMnDInfomation.szMasterName, 0, sizeof(g_pLocalUser->m_pLocalMnDInfomation.szMasterName));
				strcpy(g_pLocalUser->m_pLocalMnDInfomation.szMasterName, pInfoMyMaster->cMasterName);

				if(stricmp( g_pLocalUser->m_pLocalMnDInfomation.szMasterName, g_pLocalUser->m_CharacterInfo.charactername ) == 0 ) //�̸��� ������
					g_pLocalUser->m_pLocalMnDInfomation.nMnDClass = 1;
			}

			//��ȣ
			if( strlen(pInfoMyMaster->cMasterNick) )
			{
				memset( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, 0, sizeof(g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName));
				strcpy( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, pInfoMyMaster->cMasterNick );
			}

			//����Ʈ�� �־���� �Ѵ�. �켱 ���� ���� ���� �־��� �� ���� ������ �־��ش�
			if( g_pGroup_Window->m_pMnDWindow )
			{
				g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.clear();

				MnDGroupList* pMaster = new MnDGroupList;
				pMaster->nLevel = pInfoMyMaster->ucLevel; //���� ����
				pMaster->nCurrentZone = pInfoMyMaster->ucMasterCurZone; //���� ����
				pMaster->nPlayerState = pInfoMyMaster->ucMasterConnectState; //���� ����
				pMaster->nMnDClass = 1; //Ŭ����  ����
				memset(pMaster->szName, 0, sizeof(pMaster->szName));
				strcpy( pMaster->szName, pInfoMyMaster->cMasterName );

				g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.push_back( pMaster );

				for( int i = 0; i < 4; i++ )
				{
					_XLog("�ʵ� ���� ���� �̸� %s\n �ʵ� ���� ��� ���� �ΰ� %d\n", pInfoMyMaster->Pupils[i].cName, (int)pInfoMyMaster->Pupils[i].ucPosition );

					if(pInfoMyMaster->Pupils[i].ucPosition == 0 ) //�������� 0 �̸� �ƹ��͵� �ƴϴ�
						continue;

					MnDGroupList* pDisciple = new MnDGroupList;

					memset(pDisciple->szName, 0, sizeof(pDisciple->szName));

					strcpy(pDisciple->szName, pInfoMyMaster->Pupils[i].cName ); //�̸�
					pDisciple->nLevel = pInfoMyMaster->Pupils[i].ucLevel; //����
					pDisciple->nDiscipleContribution = pInfoMyMaster->Pupils[i].uiCurPoint; //�⿩��
					pDisciple->nPlayerState = pInfoMyMaster->Pupils[i].ucConnectState; //���ӻ���
					pDisciple->nMnDClass = pInfoMyMaster->Pupils[i].ucPosition; //������� �ΰ�

					if(stricmp( pInfoMyMaster->Pupils[i].cName, g_pLocalUser->m_CharacterInfo.charactername ) == 0 ) //�̸��� ������
						g_pLocalUser->m_pLocalMnDInfomation.nMnDClass = pInfoMyMaster->Pupils[i].ucPosition;

					pDisciple->nCurrentZone = pInfoMyMaster->Pupils[i].ucCurZone; //������ ����

					g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.push_back( pDisciple );
				}
			}			

			//����,
			std::sort(g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.begin(), g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.end(), MnDGroupClassSort);

			g_pGroup_Window->m_pMnDWindow->SetListItem();
			
			if( g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS5)
				g_pGroup_Window->m_pMnDWindow->ShowMnDWindow(TRUE);
			
		}
		break;
	case en_cg_mp_mode_info_res :	// ���� ���� ���� ��û. ���� ���� ����
		{
			MSG_MP_INFO_RES* pSimpleInfo = (MSG_MP_INFO_RES*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_update_point : // ���� �� ������ ����Ʈ ����.
		{
			MSG_MP_UPDATE_POINT* pUpdatePoint = (MSG_MP_UPDATE_POINT*)pMnDResult;

			_XLog("����Ʈ ���� ���� ����Ʈ %d\n ����Ʈ ���� ��Ż ����Ʈ %d\n ����Ʈ ���� �⿩�� %d\n", 
				pUpdatePoint->uiMasterCurPoint, pUpdatePoint->uiMasterTotalPoint, pUpdatePoint->uiCurPoint);

			g_pLocalUser->m_pLocalMnDInfomation.nMasterContribution = pUpdatePoint->uiMasterCurPoint; //���� ���� ����Ʈ
			g_pLocalUser->m_pLocalMnDInfomation.nMasterTotalContribution = pUpdatePoint->uiMasterTotalPoint; //���� ��Ż ����Ʈ
			g_pLocalUser->m_pLocalMnDInfomation.nLocalUserContribution = pUpdatePoint->uiCurPoint; //�⿩��,

			//�׷� ����Ʈ ���� �ؾ���,
		}
		break;
	case en_cg_mp_mode_update_kill_count : // ų ī��Ʈ ������Ʈ
		{
			MSG_MP_UPDATE_KILL_COUNT* pUpdateKillCount = (MSG_MP_UPDATE_KILL_COUNT*)pMnDResult;

			_XLog("ųī��Ʈ �� ų ī��Ʈ %d\n ųī��Ʈ ��Ż ī��Ʈ %d\n",
				pUpdateKillCount->uiMyKillCount, pUpdateKillCount->uiTotalKillCount);

			g_pLocalUser->m_pLocalMnDInfomation.nCurKillCount = pUpdateKillCount->uiMyKillCount; //�� ų ī��Ʈ
			g_pLocalUser->m_pLocalMnDInfomation.nTotalKillCount = pUpdateKillCount->uiTotalKillCount; //�� ų ī��Ʈ
		}
		break;
	case en_cg_mp_mode_update_master_level : //���� ��� ����
		{

			MSG_MP_UPDATE_MASTER_LEVEL* pUpdateLevel = (MSG_MP_UPDATE_MASTER_LEVEL*)pMnDResult;

			_XLog("��� ���� %d\n", pUpdateLevel->ucMasterLevel);

			g_pLocalUser->m_pLocalMnDInfomation.nMasterRank = pUpdateLevel->ucMasterLevel; //���� ��� ����
		}
		break;
	case en_cg_mp_mode_udpate_position : // ��1,2,3,4, ������ ���� �ٲٱ�.
		{
			MSG_MP_UPDATE_POSITION* pUpdateLevel = (MSG_MP_UPDATE_POSITION*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_master_info_created : // ���� ������ �����Ǿ���.( ����Ʈ : rank 1 ������. )
		{
			MSG_MP_MASTER_INFO_CREATED* pCreateMaster = (MSG_MP_MASTER_INFO_CREATED*)pMnDResult;

			g_pLocalUser->m_pLocalMnDInfomation.nMasterIndex = pCreateMaster->iMasterIndex;
			g_pLocalUser->m_pLocalMnDInfomation.nMasterRank = pCreateMaster->ucMasterLevel;
			g_pLocalUser->m_pLocalMnDInfomation.nMnDClass = 1;

			g_pLocalUser->m_pLocalMnDInfomation.nSex = pCreateMaster->ucSex;
			g_pLocalUser->m_pLocalMnDInfomation.nLevel = pCreateMaster->ucLevel;	
			g_pLocalUser->m_pLocalMnDInfomation.nMoonpa = pCreateMaster->ucMoonpa;
			g_pLocalUser->m_pLocalMnDInfomation.nClass = pCreateMaster->ucClass;
			g_pLocalUser->m_pLocalMnDInfomation.nClassGrade= pCreateMaster->ucClassGrade;

			_XLog("���� ���� ���� ���� �ε��� %d\n, ���� ���� ���� ��� %d\n", pCreateMaster->iMasterIndex, pCreateMaster->ucMasterLevel);

			if( g_pGroup_Window->m_pMnDWindow )
			{
				g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.clear();

				MnDGroupList* pMaster = new MnDGroupList;
				pMaster->nLevel = pCreateMaster->ucLevel; //���� ����
				pMaster->nCurrentZone = g_CurrentZoneInfoIndex; //���� ���� ���� ����� ���� �ϱ� ����ġ�� �־��ָ� �ȴ�
				pMaster->nPlayerState = 1;//���� ������ ���� �ȰŴϱ�,, ���� ���̴�
				pMaster->nMnDClass = 1; //Ŭ����  ����
				//������ ��Ͽ� �⿩���� ǥ������ �ʴ´�

				strcpy( pMaster->szName, g_pLocalUser->m_CharacterInfo.charactername ); //��������Ŵ�,, ���̸� �־��ش�

				g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.push_back( pMaster );
			}

			g_pGroup_Window->m_pMnDWindow->SetListItem();
			
			if( g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS5)
				g_pGroup_Window->m_pMnDWindow->ShowMnDWindow(TRUE);
		}
		break;
	case en_cg_mp_mode_update_con_state : // ������ ���� ���� ������Ʈ
		{
			MSG_MP_UPDATE_CON_STATE* pUpdateState = (MSG_MP_UPDATE_CON_STATE*)pMnDResult;
			_XLog("���� ���� ����������Ʈ �̸� %s, ��ġ��ȣ %d, ���ӻ���%d\n", pUpdateState->cName, pUpdateState->ucZone, pUpdateState->ucConState );

			if( g_pGroup_Window->m_pMnDWindow )
			{
				vector<MnDGroupList*>::iterator MnDiter = g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.begin();
				for(; MnDiter != g_pGroup_Window->m_pMnDWindow->m_vtMnDGroup.end(); ++MnDiter)
				{
					MnDGroupList* pGroup = (*MnDiter);
					if(pGroup)
					{
						if(stricmp( pGroup->szName, pUpdateState->cName ) == 0 ) //�ִ�
						{
							pGroup->nPlayerState = pUpdateState->ucConState;
							pGroup->nCurrentZone = pUpdateState->ucZone;
							break;
						}
					}
				}
			}

			g_pGroup_Window->m_pMnDWindow->SetListItem();
			
			if( g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS5)
				g_pGroup_Window->m_pMnDWindow->ShowMnDWindow(TRUE);
		}
		break;
	case en_cg_mp_mode_bonus_setting : // ������ ���� On/Off
		{
			MSG_MP_BONUS_SETTING* pUpdateLevel = (MSG_MP_BONUS_SETTING*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_change_master_nick_res :// ��ȣ ������ �˷��ش�.// ��û�� ����(����)���Ը� ����, ���ڵ鿡�Դ� MSG_MP_UPDATE_MASTER_NICK �� ����. 
		{
			MSG_MP_CHANGE_MASTER_NICK_RES* pChangeNick = (MSG_MP_CHANGE_MASTER_NICK_RES*)pMnDResult;

			_XLog("����� ��ȣ %s\n", pChangeNick->cNick);

			if( pChangeNick->ucSuccessFail ) // 0 - Fail, 1 - Success
			{
				if( strlen(pChangeNick->cNick) )
				{
					memset( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, 0, sizeof(g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName));
					strcpy( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, pChangeNick->cNick);
				}

				//��ȣ�� �����Ͽ����ϴ�.
				g_NetworkKernel.InsertChatString(_T("��ȣ�� �����Ͽ����ϴ�."), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );
			}
			else
			{
				g_NetworkKernel.InsertChatString(_T("��ȣ ������ �����߽��ϴ�."), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
			}
		}
		break;
	case en_cg_mp_mode_update_master_nick : // ���� �Ǵ� �ֺ��� �˸���.
		{
			MSG_MP_UPDATE_MASTER_NICK* pUpdateNick = (MSG_MP_UPDATE_MASTER_NICK*)pMnDResult;

			_XLog("�ٸ� ������� ����� ��ȣ �˸� %s\n", pUpdateNick->cNick);

			if( strlen(pUpdateNick->cNick) )
			{
				memset( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, 0, sizeof(g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName));
				strcpy( g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName, pUpdateNick->cNick);
			}
		}
		break;
	case en_cg_mp_mode_info_error : // Info : error message.
		{
			MSG_MP_INFO_ERROR* pInfoError = (MSG_MP_INFO_ERROR*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_gc_apply_for_pupil : // ���� ��û�� ���� : ������ ����.
		{
			MSG_MP_GC_APPLY_FOR_PUPIL* pDiscipleInfo = (MSG_MP_GC_APPLY_FOR_PUPIL*)pMnDResult;

			//���� �� �����츦 �������Ѵ�
			int windowid = 0;
			bool bfindmessengerdialog = FALSE; 
			for( windowid = 60000; windowid < 60050; ++windowid )
			{
				_XWindow_MessengerDialog* pMessengerDialog_Window = (_XWindow_MessengerDialog*)g_MainWindowManager.FindWindow(windowid);
				if( pMessengerDialog_Window )
				{
					// ���� ������� �����츦 ã�´� 
					if(strcmpi(pDiscipleInfo->cFromPupil, pMessengerDialog_Window->m_ToCharactername) == 0 &&
						strcmpi(pDiscipleInfo->cMasterName, g_pLocalUser->m_CharacterInfo.charactername) == 0 )//������ ���� �����찡 �ִ�.
					{
						if( pMessengerDialog_Window->m_MessengerDialogMode == _XMESSENGERDIALOGMODE_MNDINSTANCE || //����â�̴� ������ ���õ� ��ȭâ�̾�� �Ѵ�
							pMessengerDialog_Window->m_MessengerDialogMode == _XMESSENGERDIALOGMODE_MASTERDIALOG ||
							pMessengerDialog_Window->m_MessengerDialogMode == _XMESSENGERDIALOGMODE_DISCIPLEDIALOG )
						{
							//�ִ� ���� ��û�� ���� ������ ���ΰ�,,, �׷� ����� �� �� �ִ�.
							bfindmessengerdialog = TRUE;
							
							if( pMessengerDialog_Window->m_MessengerDialogMode == _XMESSENGERDIALOGMODE_MNDINSTANCE )
							{
								g_MainWindowManager.IsTopWindow( pMessengerDialog_Window );
								pMessengerDialog_Window->m_bMessageReceive = TRUE;
							}
							else
							{
								g_MainWindowManager.IsTopWindow( pMessengerDialog_Window );						
							}
							break;
						}
					}
				}
			}

			if( !bfindmessengerdialog )// �޽��� ��ȭâ�� ���� �����
			{		
				_XWindow_MessengerDialog* pMessengerDialog_Window = _XWindow_MessengerDialog::CreateMessengerDialogBox( &g_MainWindowManager );
				if( pMessengerDialog_Window )
				{
					for( int i = 0; i < 50; ++i)
					{
						if( !g_NetworkKernel.m_bMessengerInstanceMessagePosition[i] )
						{
							g_NetworkKernel.m_bMessengerInstanceMessagePosition[i] = TRUE;
							pMessengerDialog_Window->m_InstanceMessagePositionNumber = i;
							pMessengerDialog_Window->MoveWindow( gnWidth - _XDEF_INSTANCEMESSAGEWINDOW_WIDTH - (4*i), 214 + (27*i) );
							break;
						}
					}

					strcpy( pMessengerDialog_Window->m_ToCharactername, pDiscipleInfo->cFromPupil );			
					if(strcmpi(pDiscipleInfo->cMasterName, g_pLocalUser->m_CharacterInfo.charactername) == 0)
					{				
						if(pMessengerDialog_Window->m_MessengerChatList.getitemcount() > _XDEF_CHATLIST_QUESIZE)
						{
							pMessengerDialog_Window->m_MessengerChatList.deleteItem(0);
						}
						
						pMessengerDialog_Window->SetMode( _XMESSENGERDIALOGMODE_MNDINSTANCE );
						g_MainWindowManager.IsTopWindow( pMessengerDialog_Window );
						pMessengerDialog_Window->m_bMessageReceive = TRUE;
					}

					pMessengerDialog_Window->m_DiscipleInfo.nClassInfo = pDiscipleInfo->ucMoonpa; //��å ����
					pMessengerDialog_Window->m_DiscipleInfo.nLevel = pDiscipleInfo->ucLevel; //����
					
					memset( pMessengerDialog_Window->m_DiscipleInfo.szName, 0, sizeof(pMessengerDialog_Window->m_DiscipleInfo.szName) );
					strcpy( pMessengerDialog_Window->m_DiscipleInfo.szName, pDiscipleInfo->cFromPupil );
				}
			}	
		}
		break;
	case en_cg_mp_mode_answer_apply_no_to_pupil : // ���ڰ� �� ĳ���Ϳ��� ������ ������. ( yes �� �������� ���, ������ complete_join_pupil �� ����. )
		{
			MSG_MP_ANSWER_APPLY_NO_TO_PUPIL* pApplyNo = (MSG_MP_ANSWER_APPLY_NO_TO_PUPIL*)pMnDResult;
			
			//pApplyNo->cFromMaster; %s���� ���� ��û�� �ź� �Ͽ����ϴ�.
		}
		break;
	case en_cg_mp_mode_complete_join_pupil : // ���ڰ� �Ƿ��� ĳ���Ϳ�����, ���� �߰��� �Ϸ�Ǿ����� �˸�. 
		{
			MSG_MP_COMPLETE_JOIN_PUPIL* pApplyYes = (MSG_MP_COMPLETE_JOIN_PUPIL*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_add_new_pupil : // ���ο� ���ڰ� �߰��Ǿ����� ���� �� �ٸ� ���ڿ��� �˸�.
		{
			MSG_MP_ADD_NEW_PUPIL* pNewDisciple = (MSG_MP_ADD_NEW_PUPIL*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_apply_error :
		{
			MSG_MP_APPLY_ERROR* pError = (MSG_MP_APPLY_ERROR*)pMnDResult;

			switch(pError->ucErrorCode)
			{
			default :
				break;
			}
/*			en_mp_apply_no_error=0,						// 0 ���� �׳� ����. �� �����δ� ��Ŷ�� ���� �ʴ´�.
			en_mp_apply_error_wait=1,					// 10 �� ���� : ����� �ٽ� �õ�.
			en_mp_apply_error_pupil_level_limit=2,		// 73 ���� �̻� ĳ���ʹ� ���ڰ� �� �� �����ϴ�.
			en_mp_apply_error_level_diff_limit=3,		// ���°� ���ڴ� 13���� �̻� ���̰� �����մϴ�.
			en_mp_apply_error_already_has_master=4,		// �̹� �ٸ� ������ �ֽ��ϴ�.
			en_mp_apply_error_pupils_over=5,			// �� �̻� ���ڸ� ���� �� �����ϴ�.
			en_mp_apply_error_you_are_master=6,			// ���� �����̴�.*/
		}
		break;
	case en_cg_mp_mode_strike_result :	// ���� �� �ٸ� ���ڿ��� ��.
		{
			MSG_MP_STRIKE_RESULT* pStrikeRes = (MSG_MP_STRIKE_RESULT*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_you_struck : // �Ĺ� ���ڿ��� �Ĺ��� �˸�.
		{
			MSG_MP_YOU_STRUCK* pStrikeRes = (MSG_MP_YOU_STRUCK*)pMnDResult;

			//�ϻ� �Ͽ����ϴ�.
		}
		break;
	case en_cg_mp_mode_strike_error :	// �Ĺ� ����.
		{
			MSG_MP_STRIKE_ERROR* pStrikeError = (MSG_MP_STRIKE_ERROR*)pMnDResult;

			switch(pStrikeError->ucErrorCode)
			{
			default:
				break;
			}

			/*en_mp_strike_error_no=0,			// �ǹ� ����.
			en_mp_strike_error_not_master=1,	// �Ĺ� �ڰ��� ����( ������ �ƴ� )
			en_mp_strike_error_pupil_not_found=2,	// �Ĺ��� ���ڰ� ����Ǿ� �־�� �Ѵ�.
			en_mp_strike_error_system_error=255,	// �� �� ���� ������.*/
		}
		break;
	case en_cg_mp_mode_you_left :	// �ϻ꿡 ����.
		{
			MSG_MP_YOU_LEFT* pLeaveRes = (MSG_MP_YOU_LEFT*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_left_pupil : // ���ڰ� �ϻ��ߴ�. ���� �� �ٸ� ���ڿ��� ����.
		{
			MSG_MP_LEFT_PUPIL* pLeaveRes = (MSG_MP_LEFT_PUPIL*)pMnDResult;
		}
		break;
	case en_cg_mp_mode_left_error :	// �ϻ� ����.
		{
			MSG_MP_LEFT_ERROR* pLeaveError = (MSG_MP_LEFT_ERROR*)pMnDResult;

			switch(pLeaveError->ucErrorCode)
			{
			default:
				break;
			}

			/*en_mp_leave_error_no=0,				// �ǹ� ����.
			en_mp_leave_error_not_pupil=1,		// �ϻ��� �� ����. ���ڰ� �ƴ�.
			en_mp_leave_error_system_error=255,	// �� �� ���� ������.*/
		}
		break;

	
	}
#endif
}
// XNetwork_Match.cpp : �� ���� 
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "XWindow_PVPBroker.h"
#include "XWindow_MatchTablePVP.h"
#include "XMessageWindow.h"
#include "XWindow_MatchNoticeLarge.h"
#include "XWindow_WorldMinimap.h"
#include "Xwindow_MatchNoticeSmall.h"
#include "XWindow_MatchMessage.h"
#include "Xwindow_MatchRecord.h"
#include "XWindow_MatchPR.h"
#include "XWindow_PK.h"
#include "XWindow_MatchEvent.h"
#include "XWindowObjectDefine.h"
#include "SoundEffectList.h"
#include "XParticleIDDefine.h"
#include "XSR_STRINGHEADER.h"
#include "XWindow_NickName.h"
#include "XWindow_FreeMatchInfo.h"
#include "XWindow_MoveVillage.h"

BOOL _XNetwork::Send_MatchList(int type, int param1, int param2, int param3, int param4)
{
	int size = sizeof(MSG_MATCH_LIST);
	MSG_MATCH_LIST matchlist;
	ZeroMemory(&matchlist, size);

	matchlist.header.sLength = size;
	matchlist.ucMessage = MSG_NO_MATCH_LIST;
	matchlist.ucMode = (unsigned char)param1;

	return _XSendPacket_Encapsulate((char*)&matchlist);
}

void _XNetwork::MSGMatchListModeRegister(MSG_MATCH_LIST* matchlist)
{
	// �� ��� ��Ͽ� ��ϵ�
	InsertChatString(_XGETINTERFACETEXT(ID_STRING_BOOTH_REGISTER1), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//��� �Ǿ����ϴ�.
	
	_XWindow_PVPBroker* pPVPBroker_Window = (_XWindow_PVPBroker*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PVPBROKER);
	if(pPVPBroker_Window)
	{
		if(pPVPBroker_Window->GetShowStatus())
		{
			// �� ��� ����Ʈ ��û
			g_NetworkKernel.SendPacket(MSG_NO_MATCH_LIST, en_match_list_mode_req_list);
		}
	}
}

void _XNetwork::MSGMatchListModeNotMap(MSG_MATCH_LIST* matchlist)
{
	// �� ��� ��� ��� ����
	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2922), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//����� ���� �Ͽ����ϴ�. (�������� �ƴ�)
}

void _XNetwork::MSGMatchListModeListUpdate(MSG_MATCH_LIST_INFO* matchlistinfo)
{
	// �� ��� ��� �ޱ�
	__fighterInfo* fightinfo;
	unsigned char length;

	fightinfo = new __fighterInfo[matchlistinfo->ucCount];

	char* pBuf = &matchlistinfo->cPack[0];

	for(int i = 0 ; i < matchlistinfo->ucCount ; ++i)
	{
		length = ((__fighterInfo*)pBuf)->ucLength;

		memcpy(&fightinfo[i], pBuf, length);

		pBuf += length;
	}

	_XWindow_PVPBroker* pPVPBroker_Window = (_XWindow_PVPBroker*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PVPBROKER);
	if(pPVPBroker_Window)
	{
		pPVPBroker_Window->SetListItem(fightinfo, matchlistinfo->ucCount);
	}

	delete[] fightinfo;
}

void _XNetwork::MSGMatchListModeListContinue(MSG_MATCH_LIST_INFO* matchlistinfo)
{
	// �� ��� ��� �ޱ�
	__fighterInfo* fightinfo;
	unsigned char length;
	
	fightinfo = new __fighterInfo[matchlistinfo->ucCount];
	
	char* pBuf = &matchlistinfo->cPack[0];
	
	for(int i = 0 ; i < matchlistinfo->ucCount ; ++i)
	{
		length = ((__fighterInfo*)pBuf)->ucLength;
		
		memcpy(&fightinfo[i], pBuf, length);
		
		pBuf += length;
	}

	_XWindow_PVPBroker* pPVPBroker_Window = (_XWindow_PVPBroker*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PVPBROKER);
	if(pPVPBroker_Window)
	{
		pPVPBroker_Window->SetListItem(fightinfo, matchlistinfo->ucCount, TRUE);	// continue
	}

	delete[] fightinfo;
}

void _XNetwork::MSGMatchListModeDelete(MSG_MATCH_LIST* matchlist)
{
	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2923), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"�� ��Ͽ��� �����Ǿ����ϴ�."

	// ��� �ٽ� ��û
	SendPacket(MSG_NO_MATCH_LIST, en_match_list_mode_req_list);
}

BOOL _XNetwork::Send_MatchReady(int type, int param1, int param2, int param3, int param4)
{
	switch(param1)
	{
	case en_match_ready_challenge_to_other :
		{
			int size = sizeof(MSG_MATCH_READY_CHALLENGE_TO);
			MSG_MATCH_READY_CHALLENGE_TO matchready;
			ZeroMemory(&matchready, size);
			
			matchready.header.sLength = size;
			matchready.ucMessage = MSG_NO_MATCH_READY;
			matchready.ucMode = (unsigned char)param1;

			matchready.ucMatchType = (unsigned char)param2;
			matchready.usTo = g_pLocalUser->m_MatchTargetID;

			return _XSendPacket_Encapsulate((char*)&matchready);
		}
		break;
	case en_match_ready_challenge_cancel :
		{
			int size = sizeof(MSG_MATCH_READY_CHALLENGE_CANCEL);
			MSG_MATCH_READY_CHALLENGE_CANCEL matchready;
			ZeroMemory(&matchready, size);
			
			matchready.header.sLength = size;
			matchready.ucMessage = MSG_NO_MATCH_READY;
			matchready.ucMode = (unsigned char)param1;

			matchready.usFrom = g_pLocalUser->m_Uniqueid;

			return _XSendPacket_Encapsulate((char*)&matchready);
		}
		break;
	case en_match_ready_reply :
		{
			int size = sizeof(MSG_MATCH_READY_REPLY);
			MSG_MATCH_READY_REPLY matchreply;
			ZeroMemory(&matchreply, size);

			matchreply.header.sLength = size;
			matchreply.ucMessage = MSG_NO_MATCH_READY;
			matchreply.ucMode = (unsigned char)param1;

			matchreply.ucReply = (unsigned char)param2;
			matchreply.usFrom = g_pLocalUser->m_MatchTargetID;

			return _XSendPacket_Encapsulate((char*)&matchreply);
		}
		break;
	case en_match_ready_update_recordtype :
	case en_match_ready_update_matchtype :
	case en_match_ready_update_matchtime :
	case en_match_ready_update_disable_hyperrun :
	case en_match_ready_update_keep_buff :
	case en_match_ready_update_balance :
		{
			int size = sizeof(MSG_MATCH_READY_UPDATE);
			MSG_MATCH_READY_UPDATE matchready;
			ZeroMemory(&matchready, size);
			
			matchready.header.sLength = size;
			matchready.ucMessage = MSG_NO_MATCH_READY;
			matchready.ucMode = (unsigned char)param1;

			matchready.usValue = (unsigned short)param2;
			
			return _XSendPacket_Encapsulate((char*)&matchready);
		}
		break;
	case en_match_ready_req_add_item :
		{
			int size = sizeof(MSG_MATCH_READY_ITEM_ADD_REQ);
			MSG_MATCH_READY_ITEM_ADD_REQ matchready;
			ZeroMemory(&matchready, size);
			
			matchready.header.sLength = size;
			matchready.ucMessage = MSG_NO_MATCH_READY;
			matchready.ucMode = (unsigned char)param1;

			// item ó��
						
			return _XSendPacket_Encapsulate((char*)&matchready);
		}
		break;
	case en_match_ready_req_del_item :
		{
			int size = sizeof(MSG_MATCH_READY_ITEM_DEL_REQ);
			MSG_MATCH_READY_ITEM_DEL_REQ matchready;
			ZeroMemory(&matchready, size);
			
			matchready.header.sLength = size;
			matchready.ucMessage = MSG_NO_MATCH_READY;
			matchready.ucMode = (unsigned char)param1;
			
			// item ó��
			
			return _XSendPacket_Encapsulate((char*)&matchready);
		}
		break;
	case en_match_ready_req_select_member :
		{
			int size = sizeof(MSG_MATCH_READY_REQ_SELECT_MEMBER);
			MSG_MATCH_READY_REQ_SELECT_MEMBER matchready;
			ZeroMemory(&matchready, size);
			
			matchready.header.sLength = size;
			matchready.ucMessage = MSG_NO_MATCH_READY;
			matchready.ucMode = (unsigned char)param1;
			
			// �ο�����
			
			return _XSendPacket_Encapsulate((char*)&matchready);
		}
		break;
	case en_match_ready_confirm :
		{
			int size = sizeof(MSG_MATCH_READY);
			MSG_MATCH_READY matchready;
			ZeroMemory(&matchready, size);
			
			matchready.header.sLength = size;
			matchready.ucMessage = MSG_NO_MATCH_READY;
			matchready.ucMode = (unsigned char)param1;
						
			return _XSendPacket_Encapsulate((char*)&matchready);
		}
		break;
	case en_match_ready_cancel_all :
		{
			int size = sizeof(MSG_MATCH_READY);
			MSG_MATCH_READY matchready;
			ZeroMemory(&matchready, size);

			matchready.header.sLength = size;
			matchready.ucMessage = MSG_NO_MATCH_READY;
			matchready.ucMode = (unsigned char)param1;

			return _XSendPacket_Encapsulate((char*)&matchready);
		}
		break;
	case en_match_ready_free_request_match :
		{
			int size = sizeof(MSG_MATCH_READY_REQ_FREE_MATCH);
			MSG_MATCH_READY_REQ_FREE_MATCH matchready;
			ZeroMemory(&matchready, size);

			matchready.header.sLength = size;
			matchready.ucMessage = MSG_NO_MATCH_READY;
			matchready.ucMode = (unsigned char)param1;
			
			matchready.uiNeedMoney = 100;

			return _XSendPacket_Encapsulate((char*)&matchready);
		}
		break;
	}

	return FALSE;
}

void _XNetwork::MSGMatchReadyChallengeFromOther(MSG_MATCH_READY_CHALLENGE_FROM* matchreadyfrom)
{
	// ����ó���ϱ�
	bool pvpcancle = false;
	if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
		(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
	{
		pvpcancle = true;
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1915), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"����������̹Ƿ� �� ��û�� �޾Ƶ��� �� �����ϴ�."
	}
	else if( g_pLocalUser->m_CharacterLog.isTrade )
	{
		pvpcancle = true;
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1916), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"��ȯ���̹Ƿ� �� ��û�� �޾Ƶ��� �� �����ϴ�."
	}
	else if( g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING ) 
	{
		pvpcancle = true;
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1917), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"�������̹Ƿ� �� ��û�� �޾Ƶ��� �� �����ϴ�."
	}
	else if( g_pLocalUser->m_bReadingBook )
	{
		pvpcancle = true;
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1918), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"����� �д� ���̹Ƿ� �� ��û�� �޾Ƶ��� �� �����ϴ�."
	}
	else if( g_pLocalUser->m_bInternalUse )
	{
		pvpcancle = true;
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1919), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"������ �������̹Ƿ� �� ��û�� �޾Ƶ��� �� �����ϴ�."
	}
	else if( g_pLocalUser->m_bSocketPlugIn )
	{
		pvpcancle = true;
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1920), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"��Ĺ��� �������̹Ƿ� �� ��û�� �޾Ƶ��� �� �����ϴ�."
	}
	else if( g_pLocalUser->m_bGambleBoxOpen )
	{
		pvpcancle = true;
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1921), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"�пձ˸� ���� ���̹Ƿ� �� ��û�� �޾Ƶ��� �� �����ϴ�."
	}
	else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
	{
		pvpcancle = true;
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1922), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"������ �������Ƿ� �� ��û�� �޾Ƶ��� �� �����ϴ�."
	}
	else if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_REQUEST)
	{
		pvpcancle = true;
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1923), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"�̹� ��û���� �񹫰� �����Ƿ� �� ��û�� �޾Ƶ��� �� �����ϴ�."
	}
	if(g_DenyMatch)
	{
		pvpcancle = true;
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1925), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"�񹫽�û�ź����̹Ƿ� �� ��û�� �޾Ƶ��� �� �����ϴ�."
	}

	if( pvpcancle )
	{
		//�񹫽�û�� ���� �� ���� ��Ȳ...�񹫽�û�� �ڵ����� �����Ѵ�.
		g_NetworkKernel.SendPacket(MSG_NO_MATCH_READY, en_match_ready_reply, 1);
		g_pLocalUser->ResetMatchState();
		return;
	}
	
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if( pProc_Maingame->m_bNPCDialogMode || pProc_Maingame->m_bNPCDialogWideView || pProc_Maingame->m_bFunctionalObjectWideView)
	{
		// wide view�� �� wide view �ݱ�
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NPCSCRIPT_CLOSE, 0), 0);
	}

	TCHAR matchtype[128];
	memset(matchtype, 0, sizeof(matchtype));

	switch(matchreadyfrom->ucMatchType)
	{
	case en_matchtype_pvp :
		{
			_snprintf(matchtype, sizeof(matchtype), "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_2494));//"������"
		}
		break;
	case en_matchtype_group_deathmatch :
		{
			_snprintf(matchtype, sizeof(matchtype), "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_2495));//"������"
		}
		break;
	}

	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(messagestring));

	_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2924), matchreadyfrom->cName, matchtype); //"%s���� %s�� ��û�ϰ� �ֽ��ϴ�. �¶��Ͻðڽ��ϱ�?"

	_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
	pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_MATCH_REQUESTFROMOTHER_OK, _XDEF_MATCH_REQUESTFROMOTHER_NO);
	pMessageBox->StartTimer(10000, TRUE);

	g_pLocalUser->m_MatchTargetID = matchreadyfrom->usFrom;
	
	g_pLocalUser->m_UserState = _XDEF_USERSTATE_MATCH_REQUEST;
	g_pLocalUser->m_MatchStateChangeTime = g_LocalSystemTime;
}

void _XNetwork::MSGMatchReadyChallengeCancel(MSG_MATCH_READY_CHALLENGE_CANCEL* matchreadycancel)
{
	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2925), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"�񹫰� ��ҵǾ����ϴ�."
	g_pLocalUser->ResetMatchState();
}

void _XNetwork::MSGMatchReadyReply(MSG_MATCH_READY_REPLY* matchreadyreply)
{
	switch(matchreadyreply->ucReply)
	{
	case 0 :
		{
			// �¶�	
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2926), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"������ �¶��߽��ϴ�."
		}
		break;
	case 1 :
		{
			// ����
			g_pLocalUser->ResetMatchState();
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2927), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"������ �����߽��ϴ�."
		}
		break;
	case 2 :
	case 3 :
		{
			g_pLocalUser->ResetMatchState();
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2928), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"������ ���� �����Դϴ�."
		}
		break;
	case 4 :
		{
			g_pLocalUser->ResetMatchState();
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2929), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"������� ����ܰ� ���̰� �ʹ� Ů�ϴ�."
		}
		break;
	case 5 :
		{
			g_pLocalUser->ResetMatchState();
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2930), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"�񹫽�û�� �ߺ��Ǿ����ϴ�."
		}
		break;
	case 6 :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2931), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"�ڽ��� ���� �����̹Ƿ� ��û�� �� �����ϴ�"
		}
		break;
	case 7 :
		{
			g_pLocalUser->ResetMatchState();
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2932), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"����� �����ϴ�."
		}
		break;
	case 8 :
		{
			g_pLocalUser->ResetMatchState();
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2933), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"�������� ����� ��û�� �� �ֽ��ϴ�."
		}
		break;
	case 9 :
		{
			g_pLocalUser->ResetMatchState();
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2934), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"������ ����� �ƴմϴ�."
		}
		break;
	case 10 :
		{
			g_pLocalUser->ResetMatchState();
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2935), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"������ �ŷ����Դϴ�."
		}
		break;
	case 100 :
		{
			g_pLocalUser->ResetMatchState();
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2936), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"�� �񹫴� ���� �غ��߿� �ֽ��ϴ�."
		}
		break;
	}
}

void _XNetwork::MSGMatchReadyStartMakeTable(MSG_MATCH_READY_START_MAKE_TABLE* matchreadymaketable)
{
	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2937), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL); //"��ø �ۼ� ����"
	
	// �� ȫ�����̸� ȫ�� �ݱ�
	if(g_pLocalUser->m_bMatchPRMode)
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MATCHPR_CLOSEBUTTON, 0), (LPARAM)gHWnd);
	}

	g_pLocalUser->m_UserState = _XDEF_USERSTATE_MATCH_MAKETABLE;
	
	for(int i = 0 ; i < 2 ; ++i)
	{
		if(strcmp(g_pLocalUser->m_CharacterInfo.charactername, matchreadymaketable->MemberInfo[i].cName) == 0)
		{
			// �ڱ� �ڽ��̴� - ����ȣ�� ���� 
			g_pLocalUser->m_myMatchTeamID = matchreadymaketable->MemberInfo[i].ucTeam;
			g_pLocalUser->m_MatchGrade = matchreadymaketable->MemberInfo[i].ucMatchGrade;
		}
		else
		{
			// �� ���
			g_pLocalUser->m_myMatchMemberInfo.ucTeam = matchreadymaketable->MemberInfo[i].ucTeam;
			g_pLocalUser->m_myMatchMemberInfo.ucNumber = matchreadymaketable->MemberInfo[i].ucNumber;
			g_pLocalUser->m_myMatchMemberInfo.cClass = matchreadymaketable->MemberInfo[i].cClass;
			g_pLocalUser->m_myMatchMemberInfo.ucLevel = matchreadymaketable->MemberInfo[i].ucLevel;
			g_pLocalUser->m_myMatchMemberInfo.usWinRate = matchreadymaketable->MemberInfo[i].usWinRate;
			g_pLocalUser->m_myMatchMemberInfo.ucMatchGrade = matchreadymaketable->MemberInfo[i].ucMatchGrade;
			strncpy(g_pLocalUser->m_myMatchMemberInfo.cName, matchreadymaketable->MemberInfo[i].cName, sizeof(g_pLocalUser->m_myMatchMemberInfo.cName));
		}
	}

	if(matchreadymaketable->ucMatchType == en_matchtype_pvp)
	{
		_XWindow_MatchTablePVP* pMatchTablePVP_Window = (_XWindow_MatchTablePVP*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHTABLEPVP);
		if(pMatchTablePVP_Window)
		{
			pMatchTablePVP_Window->ResetData();
			pMatchTablePVP_Window->ShowWindow(TRUE);
		}
	}

	_XDWINPRINT("��ø �ۼ� ���� ==");
	_XDWINPRINT("�񹫻�� : [%d/%s] Team[%d]", g_pLocalUser->m_MatchTargetID, g_pLocalUser->m_myMatchMemberInfo.cName, g_pLocalUser->m_myMatchTeamID);
}

void _XNetwork::MSGMatchReadyUpdate(MSG_MATCH_READY_UPDATE* matchreadyupdate)
{
	_XWindow_MatchTablePVP* pMatchTablePVP_Window = (_XWindow_MatchTablePVP*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHTABLEPVP);

	switch(matchreadyupdate->ucMode)
	{
	case en_match_ready_update_waittime :
		{
			g_pLocalUser->m_myMatchTableInfo.usWaitTime = matchreadyupdate->usValue;
			g_pLocalUser->m_myMatchTableInfo.dwWaitStartTime = g_LocalSystemTime;

			g_pLocalUser->m_UserState = _XDEF_USERSTATE_MATCH_WAITING;

			if(matchreadyupdate->usValue == 3600)
			{
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2938), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"�ſ� ���� ��ٸ��ž� �մϴ�."
			}
			
//			if(pMatchTablePVP_Window)
//				pMatchTablePVP_Window->ShowWindow(FALSE);

		}
		break;
	case en_match_ready_update_recordtype :
		{
			g_pLocalUser->m_myMatchTableInfo.cRecordType = (char)matchreadyupdate->usValue;
			if(pMatchTablePVP_Window)
			{
				if(g_pLocalUser->m_myMatchTableInfo.cRecordType == 0)
					_snprintf(pMatchTablePVP_Window->m_strRecordType, sizeof(pMatchTablePVP_Window->m_strRecordType), _XGETINTERFACETEXT(ID_STRING_NEW_2939));//_T("���ĺ�")
				else if(g_pLocalUser->m_myMatchTableInfo.cRecordType == 1)
					_snprintf(pMatchTablePVP_Window->m_strRecordType, sizeof(pMatchTablePVP_Window->m_strRecordType), _XGETINTERFACETEXT(ID_STRING_NEW_2940));	//_T("����ĺ�")
			}

			if(pMatchTablePVP_Window)
				pMatchTablePVP_Window->m_btnConfirm->EnableWindow(TRUE);
		}
		break;
	case en_match_ready_update_matchtype :
		{
			g_pLocalUser->m_myMatchTableInfo.cMatchType = (char)matchreadyupdate->usValue;

			if(g_pLocalUser->m_myMatchTableInfo.cMatchType == 1)
			{
				if(pMatchTablePVP_Window)
				{
					_snprintf(pMatchTablePVP_Window->m_strMatchType, sizeof(pMatchTablePVP_Window->m_strMatchType), _XGETINTERFACETEXT(ID_STRING_MATCH_2486));//_T("������")
				}
			}

			if(pMatchTablePVP_Window)
				pMatchTablePVP_Window->m_btnConfirm->EnableWindow(TRUE);
		}
		break;
	case en_match_ready_update_matchtime :
		{
			g_pLocalUser->m_myMatchTableInfo.cMatchTime = (char)matchreadyupdate->usValue;
			if(pMatchTablePVP_Window)
			{
				if(g_pLocalUser->m_myMatchTableInfo.cMatchTime == 2)
					_snprintf(pMatchTablePVP_Window->m_strMatchTime, sizeof(pMatchTablePVP_Window->m_strMatchTime), _XGETINTERFACETEXT(ID_STRING_MATCH_2511));//_T("2��")
				else if(g_pLocalUser->m_myMatchTableInfo.cMatchTime == 3)
					_snprintf(pMatchTablePVP_Window->m_strMatchTime, sizeof(pMatchTablePVP_Window->m_strMatchTime), _XGETINTERFACETEXT(ID_STRING_MATCH_2512));//_T("3��")
			}

			if(pMatchTablePVP_Window)
				pMatchTablePVP_Window->m_btnConfirm->EnableWindow(TRUE);
		}
		break;
	case en_match_ready_update_disable_hyperrun :
		{
			g_pLocalUser->m_myMatchTableInfo.cUseHyperrun = (char)matchreadyupdate->usValue;
			if(pMatchTablePVP_Window)
			{
				if(g_pLocalUser->m_myMatchTableInfo.cUseHyperrun == 0)
					_snprintf(pMatchTablePVP_Window->m_strUseHyperrun, sizeof(pMatchTablePVP_Window->m_strUseHyperrun),_XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_USE));//_T("���")
				else if(g_pLocalUser->m_myMatchTableInfo.cUseHyperrun == 1)
					_snprintf(pMatchTablePVP_Window->m_strUseHyperrun, sizeof(pMatchTablePVP_Window->m_strUseHyperrun), _XGETINTERFACETEXT(ID_STRING_MATCH_2513));//_T("����")
				else if(g_pLocalUser->m_myMatchTableInfo.cUseHyperrun == 2)
					_snprintf(pMatchTablePVP_Window->m_strUseHyperrun, sizeof(pMatchTablePVP_Window->m_strUseHyperrun), _XGETINTERFACETEXT(ID_STRING_NEW_2941));//_T("�ǰݽ� ����")
			}

			if(pMatchTablePVP_Window)
				pMatchTablePVP_Window->m_btnConfirm->EnableWindow(TRUE);
		}
		break;
	case en_match_ready_update_keep_buff :
		{
			g_pLocalUser->m_myMatchTableInfo.cUseBuff = (char)matchreadyupdate->usValue;
			if(pMatchTablePVP_Window)
			{
				if(g_pLocalUser->m_myMatchTableInfo.cUseBuff == 0)
					_snprintf(pMatchTablePVP_Window->m_strUseBuff, sizeof(pMatchTablePVP_Window->m_strUseBuff), _XGETINTERFACETEXT(ID_STRING_MATCH_2514));//_T("�� ���� ����")
				else if(g_pLocalUser->m_myMatchTableInfo.cUseBuff == 1)
					_snprintf(pMatchTablePVP_Window->m_strUseBuff, sizeof(pMatchTablePVP_Window->m_strUseBuff), _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_2074));	//_T("�ʱ�ȭ")
			}

			if(pMatchTablePVP_Window)
				pMatchTablePVP_Window->m_btnConfirm->EnableWindow(TRUE);
		}
		break;
	case en_match_ready_update_balance :
		{
			g_pLocalUser->m_myMatchTableInfo.cMatchBalance = (char)matchreadyupdate->usValue;

			if(pMatchTablePVP_Window)
				pMatchTablePVP_Window->m_btnConfirm->EnableWindow(TRUE);
		}
		break;
	}
}

void _XNetwork::MSGMatchReadySelectMember(MSG_MATCH_READY_SELECT_MEMBER* matchreadyselectmember)
{
}

void _XNetwork::MSGMatchReadyDeselectMember(MSG_MATCH_READY_DESELECT_MEMBER* matchreadydeselectmember)
{
}

void _XNetwork::MSGMatchReadyConfirm(MSG_MATCH_READY* matchready)
{
	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2942), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"������ ��ø�� ������ �¶��߽��ϴ�."
}

void _XNetwork::MSGMatchReadyCancelAll(MSG_MATCH_READY* matchready)
{
	g_pLocalUser->ResetMatchState();

	_XWindow_MatchTablePVP* pMatchTablePVP_Window = (_XWindow_MatchTablePVP*) g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHTABLEPVP);
	if(pMatchTablePVP_Window)
	{
		pMatchTablePVP_Window->ResetData();
		pMatchTablePVP_Window->ShowWindow(FALSE);
	}

	_XWindow_MatchRecord* pMatchRecord_Window = (_XWindow_MatchRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHRECORD);
	if(pMatchRecord_Window)
	{
		pMatchRecord_Window->EnableTablePVPButton(FALSE);
	}

	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2925), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//"�񹫰� ��ҵǾ����ϴ�."
}

void _XNetwork::MSGMatchReadyError(MSG_MATCH_READY* matchready)
{
}

void _XNetwork::MSGMatchCtrlStart(MSG_MATCH_CTRL_START* matchctrlstart)
{
	if(matchctrlstart->ucStartWith == 0)
	{
		_XWindow_MatchTablePVP* pMatchTablePVP_Window = (_XWindow_MatchTablePVP*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHTABLEPVP);
		if(pMatchTablePVP_Window)
			pMatchTablePVP_Window->ShowWindow(FALSE);

		// �� ���� ��ġ�� �̵�
		g_LastPickedObject = NULL;
		g_LastPickedObjectType = _XPICKINGTARGET_NONE;
		g_pLocalUser->ResetAttackTarget();
		g_pLocalUser->ResetViewTarget();
		g_pLocalUser->m_bMatchPRMode = FALSE;

		_XUser* pUser = g_Userlist.FindUser(g_pLocalUser->m_MatchTargetID);
		if(pUser) // ������ �� ȫ�� ���̾����� ��� ����
		{
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, NULL);
			pUser->m_bMatchPRMode = FALSE;
		}

		_XWindow_MatchPR* pMatchPR_Window = (_XWindow_MatchPR*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MATCHPR);
		if( pMatchPR_Window )
		{
			pMatchPR_Window->ShowWindow( FALSE );
		}

		g_pLocalUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, NULL);
		g_pLocalUser->ReloadWeaponModel();
		g_pLocalUser->SetAnimation(_XACTION_IDLE);

		g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position = D3DXVECTOR3(matchctrlstart->fPosX, 0, matchctrlstart->fPosZ);

		_XOBBData* pLastPickedOBB = NULL;
		FLOAT height = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, pLastPickedOBB );
		g_pLocalUser->m_TargetPosition.y = g_pLocalUser->m_Position.y = height;
		g_pLocalUser->m_pGoundCollideOBB = pLastPickedOBB;
		
		g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
		g_pLocalUser->UpdatePosition();
		
		SendPacket(MSG_NO_ZONE_MOVE);

		g_LodTerrain.m_3PCamera.mp_fYaw = 0.0f;
		g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
		g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(g_pLocalUser->m_Position.x, 
			g_pLocalUser->m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
			g_pLocalUser->m_Position.z));

		D3DXVECTOR3 CameraPos;
		CameraPos.x = g_LodTerrain.m_3PCamera.m_TargetPosition.x;
		CameraPos.y = g_LodTerrain.m_3PCamera.m_TargetPosition.y + 1;
		CameraPos.z = g_LodTerrain.m_3PCamera.m_TargetPosition.z + 1;
		g_LodTerrain.m_3PCamera.SetCameraPosition(CameraPos);
		
		g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
		g_LodTerrain.RebuildLevel(FALSE);

		_XWindow_WorldMinimap* pMinimap_Window = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIMAPWINDOW);
		if(pMinimap_Window)
		{
			pMinimap_Window->SetRotateArrow(g_pLocalUser->m_RotateAngle + _X_PI);
			pMinimap_Window->SetLocalUserPosition();
		}

		g_pLocalUser->m_UserState = _XDEF_USERSTATE_MATCH_MATCHING;
		g_pLocalUser->m_MatchStateChangeTime = g_LocalSystemTime;
		g_pLocalUser->m_MatchFieldIndex = matchctrlstart->sMatchFieldIndex;

		TCHAR messagestring[256];
		memset(messagestring, 0, sizeof(messagestring));

		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2943), g_AreaManager.GetAreaName(_XDEF_AREACATEGORY_PVPAREA, g_pLocalUser->m_MatchFieldIndex));//"'%s'���� �񹫰� ���۵˴ϴ�."

		InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);

		_XWindow_MatchNoticeSmall* pMatchNoticeSmall_Window = (_XWindow_MatchNoticeSmall*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICESMALL);
		if(pMatchNoticeSmall_Window)
		{
			pMatchNoticeSmall_Window->ShowWindow(TRUE);
		}

		// ǥ���̵�â �ݱ�
		_XWindow_MoveVillage* pMoveVillage_Window = (_XWindow_MoveVillage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MOVEVILLAGE);
		if(pMoveVillage_Window && pMoveVillage_Window->GetShowStatus())
		{
			pMoveVillage_Window->ShowWindow(FALSE);
		}

		_XDWINPRINT("�� ���� ==");
	}
}

void _XNetwork::MSGMatchCtrlWait(MSG_MATCH_CTRL_WAIT* matchctrlwait)
{
	// lefttime update
	g_pLocalUser->m_UserState = _XDEF_USERSTATE_MATCH_WAITING;
	g_pLocalUser->m_myMatchTableInfo.usWaitTime = matchctrlwait->usLeftWaitTime;
	g_pLocalUser->m_myMatchTableInfo.dwWaitStartTime = g_LocalSystemTime;

	if(matchctrlwait->usLeftWaitTime == 3600)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2938), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"�ſ� ���� ��ٸ��ž� �մϴ�."
	}
}

void _XNetwork::MSGMatchCtrlStartCount(MSG_MATCH_CTRL* matchctrl)
{
	// count down ����
	g_pLocalUser->m_MatchStateChangeTime = g_LocalSystemTime;
	g_pLocalUser->m_UserState = _XDEF_USERSTATE_MATCH_COUNTING;

	_XWindow_MatchMessage* pMatchMessage_Window = (_XWindow_MatchMessage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHMESSAGE);
	if(pMatchMessage_Window)
	{
		pMatchMessage_Window->ShowWindow(TRUE);
	}

	_XDWINPRINT("�� ī��Ʈ �ٿ� ==");
}

void _XNetwork::MSGMatchCtrlStartFight(MSG_MATCH_CTRL* matchctrl)
{
	// fight
	g_pLocalUser->m_MatchStateChangeTime = g_LocalSystemTime;
	g_pLocalUser->m_UserState = _XDEF_USERSTATE_MATCH_FIGHT;
	
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if(pProc_Maingame)
	{
		pProc_Maingame->m_NoDisplayZoneInfo = TRUE;

		//int index = g_FMODWrapper.FindSound( _T("DUEL_BGM_MAIN_00.wav") );
		//g_FMODWrapper.PlaySound2D( index );
		//pProc_Maingame->SetSpecialBGMPlay( "DUEL_BGM_MAIN_00.ogg" );
	}

#ifdef _XDWDEBUG
	_XDWINPRINT("�� �ο�� ���� ==");
#endif
	_XUser* pUser = g_Userlist.FindUser(g_pLocalUser->m_MatchTargetID);
	if(pUser)
	{
		if(pUser->m_CurrentWeaponID > 0)
		{
			// Ȥ�� �������� ����� �ִٸ�, �ٽ� ����
			pUser->ReloadWeaponModel();
		}

#ifdef _XDWDEBUG
		_XDWINPRINT("id�� �� �� ��� - [%d/%s]", g_pLocalUser->m_MatchTargetID, pUser->m_CharacterInfo.charactername);
		_XDWINPRINT("���� ��ϵ� �� ��� - [%s]", g_pLocalUser->m_myMatchMemberInfo.cName);
#endif
	}
}

void _XNetwork::MSGMatchCtrlBeforeCountDown(MSG_MATCH_CTRL* matchctrl)
{
	// 5�� ������
	_XWindow_MatchMessage* pMatchMessage_Window = (_XWindow_MatchMessage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHMESSAGE);
	if(pMatchMessage_Window)
	{
		pMatchMessage_Window->m_bCountdown = TRUE;
		pMatchMessage_Window->m_dwCountdownStartTime = g_LocalSystemTime;
	}

	_XDWINPRINT("���� ī��Ʈ �ٿ� ==");
}

void _XNetwork::MSGMatchCtrlEnd(MSG_MATCH_CTRL_END* matchctrlend)
{
	_XDWINPRINT("End Packet ���� ==");
	
	_XPlayInterfaceSound(g_PVPBattleBGMEndIndex);

//	_XDWINPRINT("BGMEnd play ���� ==");

	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if(pProc_Maingame)
	{
//		pProc_Maingame->StopSpecialBGMPlay();
		int index = g_FMODWrapper.FindSound( _T("DUEL_BGM_MAIN_00.wav") );
		g_FMODWrapper.StopSound( index );
	}

//	_XDWINPRINT("Stop special BGM ==");

	// ��ø clear
	_XWindow_MatchTablePVP* pMatchTablePVP_Window = (_XWindow_MatchTablePVP*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHTABLEPVP);
	if(pMatchTablePVP_Window)
	{
		pMatchTablePVP_Window->ResetData();
	}

	_XWindow_MatchNoticeSmall* pMatchNoticeSmall_Window = (_XWindow_MatchNoticeSmall*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICESMALL);
	if(pMatchNoticeSmall_Window)
	{
		pMatchNoticeSmall_Window->ShowWindow(FALSE);
	}

	_XWindow_MatchRecord* pMatchRecord_Window = (_XWindow_MatchRecord*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHRECORD);
	if(pMatchRecord_Window)
	{
		pMatchRecord_Window->EnableTablePVPButton(FALSE);
	}

	g_pLocalUser->m_MatchStateChangeTime = g_LocalSystemTime;
	g_pLocalUser->m_UserState = _XDEF_USERSTATE_MATCH_END;

	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(messagestring));

	_XUser* pUser = g_Userlist.FindUser(g_pLocalUser->m_MatchTargetID);

	if(matchctrlend->ucWinTeam == 2)
	{
		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2944), g_pLocalUser->m_CharacterInfo.charactername, g_pLocalUser->m_myMatchMemberInfo.cName);//"%s�԰� %s���� �񹫰� ���ºη� ����Ǿ����ϴ�."
		g_pLocalUser->m_MatchResult = 2;
	}	
	else if(g_pLocalUser->m_myMatchTeamID == matchctrlend->ucWinTeam)
	{
		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2945), g_pLocalUser->m_CharacterInfo.charactername);//"%s���� �¸��ϼ̽��ϴ�."

		if(pUser)
		{
			g_pLocalUser->m_RotateAngle = _XMeshMath_GetRotateRadian(g_pLocalUser->m_Position, pUser->m_Position);
			g_pLocalUser->UpdatePosition();
			
			pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, g_pLocalUser->m_Position);
			pUser->UpdatePosition();
		}

		g_pLocalUser->SetAnimationIndex(_XAN_MATCH_WINNER_START);
		SendPacket(MSG_NO_ZONE_MOVE);
		g_pLocalUser->m_MatchResult = 0;
	}
	else 
	{
		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2945), g_pLocalUser->m_myMatchMemberInfo.cName);//"%s���� �¸��ϼ̽��ϴ�."

		if(pUser)
		{
			g_pLocalUser->m_RotateAngle = _XMeshMath_GetRotateRadian(g_pLocalUser->m_Position, pUser->m_Position);
			g_pLocalUser->UpdatePosition();
			
			pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, g_pLocalUser->m_Position);
			pUser->UpdatePosition();
		}

		g_pLocalUser->SetAnimationIndex(_XAN_MATCH_LOSER_START);
		SendPacket(MSG_NO_ZONE_MOVE);
		g_pLocalUser->m_MatchResult = 1;
	}

	// visual effect ����
	D3DXMATRIX offsetmat = g_pLocalUser->m_ModelDescriptor.m_Position;
	offsetmat._42 -= 1.98f;
	g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_firework_001, &offsetmat );
	if(pUser)
	{
		offsetmat = pUser->m_ModelDescriptor.m_Position;
		offsetmat._42 -= 1.98f;
		g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_firework_002, &offsetmat );
	}

	InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);

	_XDWINPRINT("�� ���� ==");
	_XDWINPRINT(messagestring);
}

void _XNetwork::MSGMatchCtrlUpdateRecord(MSG_MATCH_CTRL* matchctrl)
{
	// ���� ���
	g_pLocalUser->m_MatchStateChangeTime = g_LocalSystemTime;
	g_pLocalUser->m_UserState = _XDEF_USERSTATE_MATCH_RESULT;
}

void _XNetwork::MSGMatchCtrlCannotExit(MSG_MATCH_CTRL* matchctrl)
{
	InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_2948), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "���� ������ ���� �� �����ϴ�."
}

void _XNetwork::MSGMatchCtrlExit(MSG_MATCH_CTRL_EXIT* matchctrlexit)
{
	// ���忡�� ������
	g_LastPickedObject = NULL;
	g_LastPickedObjectType = _XPICKINGTARGET_NONE;
	g_pLocalUser->ResetAttackTarget();
	g_pLocalUser->ResetViewTarget();
	
	g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position = D3DXVECTOR3(matchctrlexit->fPosX, 0, matchctrlexit->fPosZ);
	
	_XOBBData* pLastPickedOBB = NULL;
	FLOAT height = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, pLastPickedOBB );
	g_pLocalUser->m_TargetPosition.y = g_pLocalUser->m_Position.y = height;
	g_pLocalUser->m_pGoundCollideOBB = pLastPickedOBB;
	
	g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
	g_pLocalUser->UpdatePosition();
	
	SendPacket(MSG_NO_ZONE_MOVE);
	
	g_LodTerrain.m_3PCamera.mp_fYaw = 0.0f;
	g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
	g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(g_pLocalUser->m_Position.x, 
		g_pLocalUser->m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
		g_pLocalUser->m_Position.z));
	
	D3DXVECTOR3 CameraPos;
	CameraPos.x = g_LodTerrain.m_3PCamera.m_TargetPosition.x;
	CameraPos.y = g_LodTerrain.m_3PCamera.m_TargetPosition.y + 1;
	CameraPos.z = g_LodTerrain.m_3PCamera.m_TargetPosition.z + 1;
	g_LodTerrain.m_3PCamera.SetCameraPosition(CameraPos);
	
	g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
	g_LodTerrain.RebuildLevel(FALSE);
	
	_XWindow_WorldMinimap* pMinimap_Window = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIMAPWINDOW);
	if(pMinimap_Window)
	{
		pMinimap_Window->SetRotateArrow(g_pLocalUser->m_RotateAngle + _X_PI);
		pMinimap_Window->SetLocalUserPosition();
	}
	
	g_pLocalUser->ResetMatchState();

	_XWindow_MatchMessage* pMatchMessage_Window = (_XWindow_MatchMessage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHMESSAGE);
	if(pMatchMessage_Window)
	{
		pMatchMessage_Window->ShowWindow(FALSE);
	}

	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2946), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//"�񹫰� ���� �Ǿ����ϴ�."

	_XDWINPRINT("���忡�� ���� ==");

	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if(pProc_Maingame)
	{
		pProc_Maingame->m_NoDisplayZoneInfo = FALSE;
	}
}

void _XNetwork::MSGMatchCtrlSystemError(MSG_MATCH_CTRL* matchctrl)
{
	// clear - ctrl_end ��Ŷ�� ���Ƿ� ó������ �ʾƵ� ��
//	g_pLocalUser->ResetMatchState();
//	InsertChatString("�񹫰� ������������ ���� �Ǿ����ϴ�.", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);

	_XDWINPRINT("�� ������������ ���� [system error] ==");
}

void _XNetwork::MSGMatchNoticeInform(MSG_MATCH_NOTICE_INFORM* matchnoticeinform)
{
	_XUser* pUser = g_Userlist.FindUser(matchnoticeinform->usUniqueID);
	if(pUser)
	{
		// �� ȫ�� �������� ��ȯ
		if( !pUser->m_bMatchPRMode )
		{
			pUser->SetAnimationIndex(_XAN_MATCH_PR_START);
			pUser->m_ModelDescriptor.SetLockEndFrame(TRUE);
			pUser->DeleteWeaponModel();
			if(pUser->m_CharacterInfo.gender == _XGENDER_MAN)
			{
				if(matchnoticeinform->ucLevel >= 25 && matchnoticeinform->ucLevel <= 48)
					pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, g_FLAGMODEL[0]);
				else if(matchnoticeinform->ucLevel >= 49 && matchnoticeinform->ucLevel <= 72)
					pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, g_FLAGMODEL[1]);
				else if(matchnoticeinform->ucLevel >= 73)
					pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, g_FLAGMODEL[2]);
			}
			else
			{
				if(matchnoticeinform->ucLevel >= 25 && matchnoticeinform->ucLevel <= 48)
					pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, g_FLAGMODEL[3]);
				else if(matchnoticeinform->ucLevel >= 49 && matchnoticeinform->ucLevel <= 72)
					pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, g_FLAGMODEL[4]);
				else if(matchnoticeinform->ucLevel >= 73)
					pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, g_FLAGMODEL[5]);
			}
		}
		pUser->m_bMatchPRMode = TRUE;
		strncpy( pUser->m_stringMatchPR, matchnoticeinform->cInform, sizeof(pUser->m_stringMatchPR));
	}
}
void _XNetwork::MSGMatchNoticeInformEnd(MSG_MATCH_NOTICE_INFORM_END* matchnoticeinformend)
{
	_XUser* pUser = g_Userlist.FindUser(matchnoticeinformend->usUniqueID);
	if(pUser)
	{
		// ȫ�� ��
		pUser->m_bMatchPRMode = FALSE;
		if( pUser->m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_START ||
			pUser->m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_REPEAT )
		{
			pUser->SetAnimationIndex(_XAN_MATCH_PR_END);
			pUser->m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, NULL);
			pUser->ReloadWeaponModel();
		}
	}
}

void _XNetwork::MSGMatchNoticeSBInfo(MSG_MATCH_NOTICE_SB_INFO* matchnoticesbinfo)
{
	_XWindow_MatchNoticeLarge* pNoticeLarge_Window = (_XWindow_MatchNoticeLarge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICELARGE);

	if(pNoticeLarge_Window)
	{
		map <int, _XMatchNoticeInfo*>::iterator iter_notice = pNoticeLarge_Window->m_mapMatchNoticeInfo.find(matchnoticesbinfo->usMatchRegionIndex);
		if(iter_notice != pNoticeLarge_Window->m_mapMatchNoticeInfo.end())
		{
			// update
			_XMatchNoticeInfo* currentnotice = iter_notice->second;
			if(currentnotice)
			{
				currentnotice->ucMatchType = matchnoticesbinfo->ucMatchType;
				currentnotice->ucMatchState = matchnoticesbinfo->ucMatchState;
				strncpy(currentnotice->cChiefName0, matchnoticesbinfo->cChiefName0, sizeof(currentnotice->cChiefName0));
				strncpy(currentnotice->cChiefName1, matchnoticesbinfo->cChiefName1, sizeof(currentnotice->cChiefName1));
				currentnotice->ucMemberCount = matchnoticesbinfo->ucMemberCount;
				currentnotice->dwLastRecieveTime = g_LocalSystemTime;

				for(int i = 0 ; i < 9 ; ++i)
				{
					currentnotice->ucMatchRecord0[i] = matchnoticesbinfo->RecordList[i].ucMember0;
					currentnotice->ucMatchRecord1[i] = matchnoticesbinfo->RecordList[i].ucMember1;
				}

				for(i = 0 ; i < 2 ; ++i)
				{
					currentnotice->Player[i].ucTeam = matchnoticesbinfo->Player[i].ucTeam;
					currentnotice->Player[i].ucNumber = matchnoticesbinfo->Player[i].ucNumber;
					currentnotice->Player[i].ucSex = matchnoticesbinfo->Player[i].ucSex;
					currentnotice->Player[i].ucMoonpa = matchnoticesbinfo->Player[i].ucMoonpa;
					currentnotice->Player[i].cClass = matchnoticesbinfo->Player[i].cClass;
					currentnotice->Player[i].ucLevel = matchnoticesbinfo->Player[i].ucLevel;
					currentnotice->Player[i].usWinRate = matchnoticesbinfo->Player[i].usWinRate;
					currentnotice->Player[i].ucMatchGrade = matchnoticesbinfo->Player[i].ucMatchGrade;
					currentnotice->Player[i].ucVitalRate = matchnoticesbinfo->Player[i].ucVitalRate;
					strncpy(currentnotice->Player[i].cName, matchnoticesbinfo->Player[i].cName, sizeof(currentnotice->Player[i].cName));

					if(strcmp(g_pLocalUser->m_CharacterInfo.charactername, matchnoticesbinfo->Player[i].cName) == 0)
					{
						if(i == 0)
							g_pLocalUser->m_myMatchMemberInfo.ucVitalRate = matchnoticesbinfo->Player[1].ucVitalRate;
						else
							g_pLocalUser->m_myMatchMemberInfo.ucVitalRate = matchnoticesbinfo->Player[0].ucVitalRate;

						_XWindow_MatchNoticeSmall* pMatchNoticeSmall_Window = (_XWindow_MatchNoticeSmall*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICESMALL);
						if(pMatchNoticeSmall_Window)
						{
							pMatchNoticeSmall_Window->SetParameter();
						}
					}
				}
			}
		}
		else
		{
			// Create
			_XMatchNoticeInfo* newnotice = new _XMatchNoticeInfo;

			newnotice->ucMatchType = matchnoticesbinfo->ucMatchType;
			newnotice->ucMatchState = matchnoticesbinfo->ucMatchState;
			strncpy(newnotice->cChiefName0, matchnoticesbinfo->cChiefName0, sizeof(newnotice->cChiefName0));
			strncpy(newnotice->cChiefName1, matchnoticesbinfo->cChiefName1, sizeof(newnotice->cChiefName1));
			newnotice->ucMemberCount = matchnoticesbinfo->ucMemberCount;
			newnotice->dwLastRecieveTime = g_LocalSystemTime;
			
			for(int i = 0 ; i < 9 ; ++i)
			{
				newnotice->ucMatchRecord0[i] = matchnoticesbinfo->RecordList[i].ucMember0;
				newnotice->ucMatchRecord1[i] = matchnoticesbinfo->RecordList[i].ucMember1;
			}
			
			for(i = 0 ; i < 2 ; ++i)
			{
				newnotice->Player[i].ucTeam = matchnoticesbinfo->Player[i].ucTeam;
				newnotice->Player[i].ucNumber = matchnoticesbinfo->Player[i].ucNumber;
				newnotice->Player[i].ucSex = matchnoticesbinfo->Player[i].ucSex;
				newnotice->Player[i].ucMoonpa = matchnoticesbinfo->Player[i].ucMoonpa;
				newnotice->Player[i].cClass = matchnoticesbinfo->Player[i].cClass;
				newnotice->Player[i].ucLevel = matchnoticesbinfo->Player[i].ucLevel;
				newnotice->Player[i].usWinRate = matchnoticesbinfo->Player[i].usWinRate;
				newnotice->Player[i].ucMatchGrade = matchnoticesbinfo->Player[i].ucMatchGrade;
				newnotice->Player[i].ucVitalRate = matchnoticesbinfo->Player[i].ucVitalRate;
				strncpy(newnotice->Player[i].cName, matchnoticesbinfo->Player[i].cName, sizeof(newnotice->Player[i].cName));

				if(strcmp(g_pLocalUser->m_CharacterInfo.charactername, matchnoticesbinfo->Player[i].cName) == 0)
				{
					if(i == 0)
						g_pLocalUser->m_myMatchMemberInfo.ucVitalRate = matchnoticesbinfo->Player[1].ucVitalRate;
					else
						g_pLocalUser->m_myMatchMemberInfo.ucVitalRate = matchnoticesbinfo->Player[0].ucVitalRate;
					
					_XWindow_MatchNoticeSmall* pMatchNoticeSmall_Window = (_XWindow_MatchNoticeSmall*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICESMALL);
					if(pMatchNoticeSmall_Window)
					{
						pMatchNoticeSmall_Window->SetParameter();
					}
				}
			}

			_snprintf(newnotice->strLevelNamePlayer0, sizeof(newnotice->strLevelNamePlayer0), "%s %d%s",
							g_LevelName[g_UserLevelTable[newnotice->Player[0].ucLevel].level], g_UserLevelTable[newnotice->Player[0].ucLevel].sublevel, 
							_XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));
			_snprintf(newnotice->strLevelNamePlayer1, sizeof(newnotice->strLevelNamePlayer1), "%s %d%s",
							g_LevelName[g_UserLevelTable[newnotice->Player[1].ucLevel].level], g_UserLevelTable[newnotice->Player[1].ucLevel].sublevel, 
							_XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));

			pNoticeLarge_Window->m_mapMatchNoticeInfo[matchnoticesbinfo->usMatchRegionIndex] = newnotice;
		}
	}
}

void _XNetwork::MSGMatchNoticeLBInfo(MSG_MATCH_NOTICE_LB_INFO* matchnoticelbinfo)
{
	_XWindow_MatchNoticeLarge* pNoticeLarge_Window = (_XWindow_MatchNoticeLarge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICELARGE);
	
	if(pNoticeLarge_Window)
	{
		map <int, _XMatchNoticeInfo*>::iterator iter_notice = pNoticeLarge_Window->m_mapMatchNoticeInfo.find(matchnoticelbinfo->usMatchRegionIndex);
		if(iter_notice != pNoticeLarge_Window->m_mapMatchNoticeInfo.end())
		{
			// update
			_XMatchNoticeInfo* currentnotice = iter_notice->second;
			if(currentnotice)
			{
				currentnotice->ucMatchType = matchnoticelbinfo->ucMatchType;
				strcpy(currentnotice->cChiefName0, matchnoticelbinfo->cChiefName0);
				strcpy(currentnotice->cChiefName1, matchnoticelbinfo->cChiefName1);
				currentnotice->ucMemberCount = matchnoticelbinfo->ucMemberCount;
				currentnotice->dwLastRecieveTime = g_LocalSystemTime;
				
				for(int i = 0 ; i < 9 ; ++i)
				{
					currentnotice->ucMatchRecord0[i] = matchnoticelbinfo->RecordList[i].ucMember0;
					currentnotice->ucMatchRecord1[i] = matchnoticelbinfo->RecordList[i].ucMember1;
				}
				
				for(i = 0 ; i < 2 ; ++i)
				{
					currentnotice->Player[i].ucTeam = matchnoticelbinfo->Player[i].ucTeam;
					currentnotice->Player[i].ucNumber = matchnoticelbinfo->Player[i].ucNumber;
					currentnotice->Player[i].ucSex = matchnoticelbinfo->Player[i].ucSex;
					currentnotice->Player[i].ucMoonpa = matchnoticelbinfo->Player[i].ucMoonpa;
					currentnotice->Player[i].cClass = matchnoticelbinfo->Player[i].cClass;
					currentnotice->Player[i].ucLevel = matchnoticelbinfo->Player[i].ucLevel;
					currentnotice->Player[i].usWinRate = matchnoticelbinfo->Player[i].usWinRate;
					currentnotice->Player[i].ucMatchGrade = matchnoticelbinfo->Player[i].ucMatchGrade;
					currentnotice->Player[i].ucVitalRate = matchnoticelbinfo->Player[i].ucVitalRate;
					strcpy(currentnotice->Player[i].cName, matchnoticelbinfo->Player[i].cName);
				}
			}
		}
		else
		{
			// Create
			_XMatchNoticeInfo* newnotice = new _XMatchNoticeInfo;
			
			newnotice->ucMatchType = matchnoticelbinfo->ucMatchType;
			strcpy(newnotice->cChiefName0, matchnoticelbinfo->cChiefName0);
			strcpy(newnotice->cChiefName1, matchnoticelbinfo->cChiefName1);
			newnotice->ucMemberCount = matchnoticelbinfo->ucMemberCount;
			newnotice->dwLastRecieveTime = g_LocalSystemTime;
			
			for(int i = 0 ; i < 9 ; ++i)
			{
				newnotice->ucMatchRecord0[i] = matchnoticelbinfo->RecordList[i].ucMember0;
				newnotice->ucMatchRecord1[i] = matchnoticelbinfo->RecordList[i].ucMember1;
			}
			
			for(i = 0 ; i < 2 ; ++i)
			{
				newnotice->Player[i].ucTeam = matchnoticelbinfo->Player[i].ucTeam;
				newnotice->Player[i].ucNumber = matchnoticelbinfo->Player[i].ucNumber;
				newnotice->Player[i].ucSex = matchnoticelbinfo->Player[i].ucSex;
				newnotice->Player[i].ucMoonpa = matchnoticelbinfo->Player[i].ucMoonpa;
				newnotice->Player[i].cClass = matchnoticelbinfo->Player[i].cClass;
				newnotice->Player[i].ucLevel = matchnoticelbinfo->Player[i].ucLevel;
				newnotice->Player[i].usWinRate = matchnoticelbinfo->Player[i].usWinRate;
				newnotice->Player[i].ucMatchGrade = matchnoticelbinfo->Player[i].ucMatchGrade;
				newnotice->Player[i].ucVitalRate = matchnoticelbinfo->Player[i].ucVitalRate;
				strcpy(newnotice->Player[i].cName, matchnoticelbinfo->Player[i].cName);
			}

			_snprintf(newnotice->strLevelNamePlayer0, sizeof(newnotice->strLevelNamePlayer0), _T("%s %d%s"),
							g_LevelName[g_UserLevelTable[newnotice->Player[0].ucLevel].level], g_UserLevelTable[newnotice->Player[0].ucLevel].sublevel, 
							_XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));
			_snprintf(newnotice->strLevelNamePlayer1, sizeof(newnotice->strLevelNamePlayer1), _T("%s %d%s"),
							g_LevelName[g_UserLevelTable[newnotice->Player[1].ucLevel].level], g_UserLevelTable[newnotice->Player[1].ucLevel].sublevel, 
							_XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));
			
			pNoticeLarge_Window->m_mapMatchNoticeInfo[matchnoticelbinfo->usMatchRegionIndex] = newnotice;
		}
	}
}

void _XNetwork::MSGMatchNoticeSBUpdateVital(MSG_MATCH_NOTICE_SB_UPDATE_VITAL* matchnoticesbvital)
{
	_XWindow_MatchNoticeLarge* pNoticeLarge_Window = (_XWindow_MatchNoticeLarge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICELARGE);
	
	if(pNoticeLarge_Window)
	{
		map <int, _XMatchNoticeInfo*>::iterator iter_notice = pNoticeLarge_Window->m_mapMatchNoticeInfo.find(matchnoticesbvital->usMatchRegionIndex);
		if(iter_notice != pNoticeLarge_Window->m_mapMatchNoticeInfo.end())
		{
			// update
			int areaid = iter_notice->first;
			_XMatchNoticeInfo* currentnotice = iter_notice->second;
			if(currentnotice)
			{	
				currentnotice->Player[0].ucVitalRate = matchnoticesbvital->ucVitalRate0;
				currentnotice->Player[1].ucVitalRate = matchnoticesbvital->ucVitalRate1;
				currentnotice->dwLastRecieveTime = g_LocalSystemTime;

				if(strcmp(g_pLocalUser->m_CharacterInfo.charactername, currentnotice->Player[0].cName) == 0)
				{
					g_pLocalUser->m_myMatchMemberInfo.ucVitalRate = matchnoticesbvital->ucVitalRate1;
				}
				else if(strcmp(g_pLocalUser->m_CharacterInfo.charactername, currentnotice->Player[1].cName) == 0)
				{
					g_pLocalUser->m_myMatchMemberInfo.ucVitalRate = matchnoticesbvital->ucVitalRate0;
				}
					
				_XWindow_MatchNoticeSmall* pMatchNoticeSmall_Window = (_XWindow_MatchNoticeSmall*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICESMALL);
				if(pMatchNoticeSmall_Window)
				{
					pMatchNoticeSmall_Window->SetParameter();
				}

				_XWindow_MatchNoticeLarge* pMatchNoticeLarge_Window = (_XWindow_MatchNoticeLarge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICELARGE);
				if(pMatchNoticeLarge_Window)
				{
					if(pMatchNoticeLarge_Window->GetSelectedAreaID() == areaid)
						pMatchNoticeLarge_Window->SetParameter();
				}

				_XDWINPRINT("player_1 Name[%s] vital[%d] ", currentnotice->Player[0].cName, matchnoticesbvital->ucVitalRate0);
				_XDWINPRINT("player_1 Name[%s] vital[%d] ", currentnotice->Player[1].cName, matchnoticesbvital->ucVitalRate1);
			}
		}
	}
}

void _XNetwork::MSGMatchNoticeLBUpdateVital(MSG_MATCH_NOTICE_LB_UPDATE_VITAL* matchnoticelbvital)
{
	_XWindow_MatchNoticeLarge* pNoticeLarge_Window = (_XWindow_MatchNoticeLarge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICELARGE);
	
	if(pNoticeLarge_Window)
	{
		map <int, _XMatchNoticeInfo*>::iterator iter_notice = pNoticeLarge_Window->m_mapMatchNoticeInfo.find(matchnoticelbvital->usMatchRegionIndex);
		if(iter_notice != pNoticeLarge_Window->m_mapMatchNoticeInfo.end())
		{
			// update
			_XMatchNoticeInfo* currentnotice = iter_notice->second;
			if(currentnotice)
			{	
				currentnotice->Player[0].ucVitalRate = matchnoticelbvital->ucVitalRate0;
				currentnotice->Player[1].ucVitalRate = matchnoticelbvital->ucVitalRate1;
			}
		}
	}
}

void _XNetwork::MSGMatchCtrlFreeEnter(MSG_MATCH_CTRL_FREE_ENTER* matchctrl)
{
	// ������ ������������ ����
	g_LastPickedObject = NULL;
	g_LastPickedObjectType = _XPICKINGTARGET_NONE;
	g_pLocalUser->ResetAttackTarget();
	g_pLocalUser->ResetViewTarget();
		
	g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position = D3DXVECTOR3(matchctrl->fPosX, 0, matchctrl->fPosZ);
	
	_XOBBData* pLastPickedOBB = NULL;
	FLOAT height = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, pLastPickedOBB );
	g_pLocalUser->m_TargetPosition.y = g_pLocalUser->m_Position.y = height;
	g_pLocalUser->m_pGoundCollideOBB = pLastPickedOBB;
	
	g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
	g_pLocalUser->UpdatePosition();
	
	SendPacket(MSG_NO_ZONE_MOVE);
	
	g_LodTerrain.m_3PCamera.mp_fYaw = 0.0f;
	g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
	g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(g_pLocalUser->m_Position.x, 
		g_pLocalUser->m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
		g_pLocalUser->m_Position.z));
	
	D3DXVECTOR3 CameraPos;
	CameraPos.x = g_LodTerrain.m_3PCamera.m_TargetPosition.x;
	CameraPos.y = g_LodTerrain.m_3PCamera.m_TargetPosition.y + 1;
	CameraPos.z = g_LodTerrain.m_3PCamera.m_TargetPosition.z + 1;
	g_LodTerrain.m_3PCamera.SetCameraPosition(CameraPos);
	
	g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
	g_LodTerrain.RebuildLevel(FALSE);
	
	_XWindow_WorldMinimap* pMinimap_Window = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIMAPWINDOW);
	if(pMinimap_Window)
	{
		pMinimap_Window->SetRotateArrow(g_pLocalUser->m_RotateAngle + _X_PI);
		pMinimap_Window->SetLocalUserPosition();
	}

	g_pLocalUser->m_Money = matchctrl->uiSetMoney;

#ifdef _XDWDEBUG
	_XDWINPRINT(_XGETINTERFACETEXT(ID_STRING_NEW_2947));
#endif
	
	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2947), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);//"���� �� ������������ ����"
}

BOOL _XNetwork::Send_FreeMatch(int type, int param1, int param2, int param3, int param4)
{
	switch(param1)
	{
	case en_gsfm_apply_match :
		{
			// �� ��ȸ ���� ��û
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : Free Match - req apply match");

			int size = sizeof(MSG_FM_APPLY_MATCH);

			MSG_FM_APPLY_MATCH fm;
			ZeroMemory(&fm, size);

			fm.header.sLength = size;
			fm.ucMessage = MSG_NO_FREE_MATCH;
			fm.ucFMMode = (unsigned char)param1;

			return _XSendPacket_Encapsulate((char*)&fm);
		}
		break;
	case en_gsfm_req_score :
		{
			// score ��û
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : Free Match - req Score");

			int size = sizeof(MSG_FM_REQ_SCORE);

			MSG_FM_REQ_SCORE fm;
			ZeroMemory(&fm, size);

			fm.header.sLength = size;
			fm.ucMessage = MSG_NO_FREE_MATCH;
			fm.ucFMMode = (unsigned char)param1;

			return _XSendPacket_Encapsulate((char*)&fm);
		}	
		break;
	case en_gsfm_next_match_schedule :
		{
			// ���� �񹫴�ȸ ������ ��û
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : Free Match - req next match schedule");

			int size = sizeof(MSG_FM_NEXT_MATCH_SCHEDULE);

			MSG_FM_NEXT_MATCH_SCHEDULE fm;
			ZeroMemory(&fm, size);

			fm.header.sLength = size;
			fm.ucMessage = MSG_NO_FREE_MATCH;
			fm.ucFMMode = (unsigned char)param1;

			return _XSendPacket_Encapsulate((char*)&fm);
		}
		break;
	}

	return FALSE;
}

void _XNetwork::MSGFMApplyFail(MSG_FM_APPLY_FAIL* fmfail)
{
	VIEWPACKETLOG
		_XDWPACKETLOG("RECEIVE MESSAGE : Free Match - Apply fail [%d]", fmfail->ucFail);

	switch(fmfail->ucFail)
	{
	case _fm_apply_fail_not_time :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3735), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//�񹫴�ȸ �ð��� Ȯ���Ͽ� �ֽñ� �ٶ��ϴ�.
		}
		break;
	case _fm_apply_fail_party :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3736), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//������·δ� �񹫴�ȸ�� ������ �� �����ϴ�.
		}
		break;
	case _fm_apply_fail_level :
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3737), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//��ȸ ���� ������ Ȯ���Ͽ� �ֽñ� �ٶ��ϴ�.
		}
		break;
	case _fm_apply_fail_not_map :
		{
			_XDWINPRINT("Free match - apply fail : Not Hapbi map");
		}
		break;
	case _fm_apply_fail_already_in_filed :
		{
			_XDWINPRINT("Free match - apply fail : already in field");
		}
		break;
	case _fm_apply_fail_system :
		{
			_XDWINPRINT("Free match - apply fail : System Error");
		}
		break;
	}
}

void _XNetwork::MSGFMEnterMatch(MSG_FM_ENTER_MATCH* fmenter)
{
	VIEWPACKETLOG
		_XDWPACKETLOG("RECEIVE MESSAGE : Free Match - Enter match [%f/%f]", fmenter->fx, fmenter->fz);

	// MSG_ZONE_POSITIONING�� ����
	g_pLocalUser->m_TargetPosition.x = g_pLocalUser->m_Position.x = fmenter->fx;
	g_pLocalUser->m_TargetPosition.y = g_pLocalUser->m_Position.y = 0.0f;
	g_pLocalUser->m_TargetPosition.z = g_pLocalUser->m_Position.z = fmenter->fz;

	g_pLocalUser->SetAnimation(_XACTION_IDLE);

	_XOBBData* pLastPickedOBB = NULL;
	FLOAT height = g_LodTerrain.GetTerrainAndRideOBBHeight(g_pLocalUser->m_Position, pLastPickedOBB);
	g_pLocalUser->m_Position.y = height;
	g_pLocalUser->m_pGoundCollideOBB = pLastPickedOBB;
	g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;

	g_pLocalUser->UpdatePosition();

	g_LodTerrain.m_3PCamera.mp_fYaw = 0.0f;
	g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
	g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(g_pLocalUser->m_Position.x, 
														g_pLocalUser->m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
														g_pLocalUser->m_Position.z));

	D3DXVECTOR3 CameraPos;
	CameraPos.x = g_LodTerrain.m_3PCamera.m_TargetPosition.x;
	CameraPos.y = g_LodTerrain.m_3PCamera.m_TargetPosition.y + 1;
	CameraPos.z = g_LodTerrain.m_3PCamera.m_TargetPosition.z + 1;
	g_LodTerrain.m_3PCamera.SetCameraPosition(CameraPos);
	g_LodTerrain.m_3PCamera.SetDistance( 5.0f );

	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if(pProc_Maingame->m_bNPCFadeInOutFlag)
	{
		// NPC â Close �� �� camera setting �ǰ� �ϱ� ���ؼ�
		pProc_Maingame->m_vTempCameraPosition = g_LodTerrain.m_3PCamera.m_CameraPosition;
		pProc_Maingame->m_vTempTargetPosition = g_LodTerrain.m_3PCamera.m_TargetPosition;
		pProc_Maingame->m_fTempTargetDistance = g_LodTerrain.m_3PCamera.m_TargetDistance;
		pProc_Maingame->m_fTempFov = g_LodTerrain.m_3PCamera.mp_fFov;
		pProc_Maingame->m_fTempPitch = g_LodTerrain.m_3PCamera.mp_fPitch;
		pProc_Maingame->m_fTempYaw = g_LodTerrain.m_3PCamera.mp_fYaw;
	}
	else
	{
		g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
		g_LodTerrain.RebuildLevel(FALSE);
	}

	_XWindow_WorldMinimap* pMinimap_Window = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIMAPWINDOW);
	if(pMinimap_Window)
	{
		pMinimap_Window->SetRotateArrow(g_pLocalUser->m_RotateAngle + _X_PI);
		pMinimap_Window->SetLocalUserPosition();
	}

	// �� ��ȸ ������
	g_pLocalUser->m_bEnterMatchEvent = TRUE;

	_XWindow_PK* pPK_Window = (_XWindow_PK*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKWINDOW);
	if(pPK_Window)
	{
		pPK_Window->ShowWindow(TRUE);
	}

	_XWindow_MatchEvent* pMatchEvent_Window = (_XWindow_MatchEvent*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHEVENT);
	if(pMatchEvent_Window)
	{
		pMatchEvent_Window->ShowWindow(TRUE);
	}
	
	// ǥ���̵�â �ݱ�
	_XWindow_MoveVillage* pMoveVillage_Window = (_XWindow_MoveVillage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MOVEVILLAGE);
	if(pMoveVillage_Window && pMoveVillage_Window->GetShowStatus())
	{
		pMoveVillage_Window->ShowWindow(FALSE);
	}

	// score ��û
	SendPacket(MSG_NO_FREE_MATCH, en_gsfm_req_score);
	
	pProc_Maingame->SetSpecialBGMPlay("zerolevelBGM.ogg");
}

void _XNetwork::MSGFMCtrl(MSG_FM_CTRL* fmctrl)
{
	VIEWPACKETLOG
		_XDWPACKETLOG("RECEIVE MESSAGE : Free Match - Ctrl [State : %d]", fmctrl->ucFM_State);

	switch(fmctrl->ucFM_State)
	{
	case 1:
		{
			// ready
			g_MatchEventManager.SetCurrentMatchInfo(&fmctrl->info);
			g_MatchEventManager.SetCurrentState(1);

			g_MatchEventManager.SetEventStartTime(0);
		}
		break;
	case 2:
		{
			// start
			g_MatchEventManager.SetCurrentMatchInfo(&fmctrl->info);
			g_MatchEventManager.SetCurrentState(2);

			g_MatchEventManager.SetEventStartTime(g_LocalSystemTime);
		}
		break;
	case 3:
		{
			// end

			// target ���ֱ�
			_XUser* pUser = NULL;
			if(g_pLocalUser->m_AttackTargetObjectType == _XPICKINGTARGET_PC)
			{
				pUser = (_XUser*)g_pLocalUser->m_AttackTarget;
				if(pUser)
				{
					if(pUser->GetEnterMatchEvent())
					{
						g_pLocalUser->ResetAttackTarget();
					}
				}
			}

			pUser = NULL;
			if(g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_PC)
			{
				pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
				if(pUser)
				{
					if(pUser->GetEnterMatchEvent())
					{
						g_pLocalUser->ResetViewTarget();
					}
				}
			}

			pUser = NULL;
			if(g_LastPickedObjectType == _XPICKINGTARGET_PC)
			{
				pUser = (_XUser*)g_LastPickedObject;
				if(pUser)
				{
					if(pUser->GetEnterMatchEvent())
					{
						g_LastPickedObject = NULL;
					}
				}
			}

			g_MatchEventManager.SetCurrentMatchInfo(&fmctrl->info);
			g_MatchEventManager.SetCurrentState(3);
			
			g_pLocalUser->m_bEnterMatchEvent = FALSE;

			g_MatchEventManager.SetEventStartTime(0);

			_XWindow_PK* pPK_Window = (_XWindow_PK*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKWINDOW);
			if(pPK_Window)
			{
				pPK_Window->ShowWindow(FALSE);
			}

			_XWindow_MatchEvent* pMatchEvent_Window = (_XWindow_MatchEvent*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHEVENT);
			if(pMatchEvent_Window)
			{
				pMatchEvent_Window->ShowWindow(FALSE);
			}

			// bgm ����
			XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
			pProc_Maingame->StopSpecialBGMPlay();

			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3743), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);	//"�񹫴�ȸ�� �Ϸ�Ǿ����ϴ�."
		}
		break;
	}
}

void _XNetwork::MSGFMNotice(MSG_FM_NOTICE* fmnotice)
{
	if(fmnotice->usTextArg != 0)
	{
		TCHAR messagestr[256];
		memset(messagestr, 0, sizeof(messagestr));

		_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3729), fmnotice->usTextArg);//%d�� �Ŀ� �񹫴�ȸ�� ���۵˴ϴ�.
		InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);

		int fromlevel = g_UserLevelTable[g_MatchEventManager.GetMinLevel()].level;
		int tolevel = g_UserLevelTable[g_MatchEventManager.GetMaxLevel()].level;

		if(fromlevel == tolevel)
		{
			TCHAR messagestr[512];
			memset(messagestr, 0, sizeof(messagestr));

			_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3740), g_LevelName[fromlevel]);//%s ����ܰ�  �е��� �պ� �������� ���ֽñ�  �ٶ��ϴ�.
			InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
		}
		else
		{
			TCHAR messagestr[512];
			memset(messagestr, 0, sizeof(messagestr));

			_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3741), g_LevelName[fromlevel], g_LevelName[tolevel]);	//%s-%s ����ܰ� �е��� �պ� �������� ���ֽñ� �ٶ��ϴ�.
			InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
		}

		g_MatchEventManager.SetDrawReadyMessage(TRUE);
		g_MatchEventManager.SetReadyMessageStartTime(g_LocalSystemTime);
		g_MatchEventManager.SetTextArg(fmnotice->usTextArg);
	}
	else
	{
		InsertChatString(_XGETINTERFACETEXT(fmnotice->usTextId), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
	}
}

void _XNetwork::MSGFMNoticeWinner(MSG_FM_NOTICE_WINNER* packet)
{
	TCHAR messagestr[256];
	memset(messagestr, 0, sizeof(messagestr));

	_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3731), packet->cWinnerName);//%s���� �񹫴�ȸ���� ����Ͽ����ϴ�.

	InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);

	if(strcmp(g_pLocalUser->m_CharacterInfo.charactername, packet->cWinnerName) == 0)
	{
		g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
														_XDEF_CONDITON033, &g_pLocalUser->m_ModelDescriptor.m_Position, NULL, TRUE );
	}
	else
	{
		_XUser* pUser = g_Userlist.FindUser(packet->cWinnerName);
		if(pUser)
		{
			if(pUser->m_ModelDescriptor.m_EffectManager)
			{
				pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
															_XDEF_CONDITON033, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE );
			}
		}
	}

}

void _XNetwork::MSGFMNoticeNowinner(MSG_FM_NOTICE_NOWINNER* packet)
{
	InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3732), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);//�̹� �񹫴�ȸ�� ����� ���º��Դϴ�.
}

void _XNetwork::MSGFMToWinner(MSG_FM_TO_WINNER* packet)
{
	if(packet->usTextId == 3733 || packet->usTextId == 3766)
	{
		_XMessageWindow* pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		pMessageWindow->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(packet->usTextId), TRUE, 0 );
		pMessageWindow->StartTimer(5000, TRUE);
	}
	else
	{	
		InsertChatString(_XGETINTERFACETEXT(packet->usTextId), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
	}
}

void _XNetwork::MSGFMRewardNick(MSG_FM_REWARD_NICK* packet)
{
	// ó�� �ʿ�
}

void _XNetwork::MSGFMRewardItem(MSG_FM_REWARD_ITEM* packet)
{
//	TCHAR messagestr[256];
//	sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NETWORK_GETSOMETHING), _XGameItem::GetItemFullName(packet->cItemType, packet->sItemID));
//	InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
}

void _XNetwork::MSGFMSetNick(MSG_FM_SET_NICK* packet)
{
	_XWindow_NickName* pNickName_Window = (_XWindow_NickName*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NICKNAME );
	if( pNickName_Window )
	{
		if(packet->usNick == 0)//����
		{
			pNickName_Window->m_SelectedNickNameSlotNumber = -1;

			TCHAR tempNick[256];	
			memset(tempNick, 0, sizeof(tempNick) );

			for(int i = 0; i < g_MaxNickNameCount; ++i)
			{
				if(g_NickNameInfoTable[i].usIndex == g_pLocalUser->m_nWinnerNickName)
				{
					if(strcmp(g_pLocalUser->m_CharacterInfo.m_NickName, g_NickNameInfoTable[i].cNickName) == 0)
					{
						//���� �ް� �ִ� ��ȣ����
						_snprintf(tempNick, sizeof(tempNick), _XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1629), g_NickNameInfoTable[i].cNickName);
						memset( g_pLocalUser->m_CharacterInfo.m_NickName, 0, sizeof(g_pLocalUser->m_CharacterInfo.m_NickName));
						
						g_pLocalUser->m_StateList.DeleteStateFromNicknameEffect();
						
						// icon ����
						if( g_pState_Window )	
							g_pState_Window->DeleteNicknameBuff();	// ���� ����
						
						g_pLocalUser->UpdateLocalUserAttackLevel();
					}
					else
						_snprintf(tempNick, sizeof(tempNick), _XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1630), g_NickNameInfoTable[i].cNickName);
					
					InsertChatString(tempNick, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);

					// nick name window���� ����
					pNickName_Window->SetNickNameList();
				}
			}
		}
		else//�ο�
		{
			TCHAR tempNick[256];	
			memset(tempNick, 0, sizeof(tempNick) );
			for(int i = 0; i < g_MaxNickNameCount; ++i)
			{
				if( g_NickNameInfoTable[i].usIndex == packet->usNick)
				{
					_snprintf(tempNick, sizeof(tempNick), _XGETINTERFACETEXT(ID_STRING_NETWORKNICKNAME_1631), g_NickNameInfoTable[i].cNickName);
					InsertChatString(tempNick, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);

					// nick name window�� �߰�
					pNickName_Window->SetNickNameList();
					
					if(g_pLocalUser->m_CharacterInfo.m_CurrentNIckIndex == -2)
					{
						strncpy( g_pLocalUser->m_CharacterInfo.m_NickName , g_NickNameInfoTable[i].cNickName, sizeof(g_pLocalUser->m_CharacterInfo.m_NickName) );

						// ���� �߰�
						for( int j = 0; j < 5; ++j )
						{
							if( g_NickNameInfoTable[i]._Effect[j].usEffectIndex > 0 )
							{
								g_pLocalUser->m_StateList.InsertStateFromNicknameEffect( g_NickNameInfoTable[i]._Effect[j].usEffectIndex, g_NickNameInfoTable[i]._Effect[j].usEffectValue);
								
								map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_NickNameInfoTable[i]._Effect[j].usEffectIndex);
								if(iter_table != g_CharacterStateTable.end())
								{
									if(g_CharacterStateTable[g_NickNameInfoTable[i]._Effect[j].usEffectIndex]->effect_prop == _XCL_EFFECT_PROP_GENERAL)
									{
										g_pLocalUser->UpdateLocalUserAttackLevel();
									}
								}						

							}
						}
						if( g_pState_Window )
							g_pState_Window->InsertStateListFromOther(_XST_NICKBUFF, i );

						break;
					}
				}
			}
		}
	}

	g_pLocalUser->m_nWinnerNickName = packet->usNick;
}

void _XNetwork::MSGFMCurrentState(MSG_FM_CURRENT_STATE* packet)
{
	switch(packet->ucFMCurState)
	{
	case 1:
		{
			// ready
			g_MatchEventManager.SetCurrentMatchInfo(&packet->fm_info);
			g_MatchEventManager.SetCurrentState(1);

			g_MatchEventManager.SetEventStartTime(0);
			
			_XWindow_FreeMatchInfo* pFMInfo_Window = (_XWindow_FreeMatchInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_FREEMATCHINFO);
			if(pFMInfo_Window && pFMInfo_Window->GetShowStatus())
			{
				int year = packet->fm_info.start_time.uiYear + 2000;
				int month = packet->fm_info.start_time.uiMonth;
				int day = packet->fm_info.start_time.uiDay;
				int hour = packet->fm_info.start_time.uiHour;
				int minute = packet->fm_info.start_time.uiMinute;

				_snprintf(pFMInfo_Window->m_szDate, sizeof(pFMInfo_Window->m_szDate), g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_2654, 
					&year ,&month, &day, &hour, &minute));

				int fromlevel = g_UserLevelTable[packet->fm_info.fm_level].level;
				int tolevel = g_UserLevelTable[packet->fm_info.to_level].level;

				if(fromlevel == tolevel)
				{
					_snprintf(pFMInfo_Window->m_szLevel, sizeof(pFMInfo_Window->m_szLevel), "%s", g_LevelName[fromlevel]);
				}
				else
				{
					_snprintf(pFMInfo_Window->m_szLevel, sizeof(pFMInfo_Window->m_szLevel), "%s-%s", g_LevelName[fromlevel], g_LevelName[tolevel]);
				}
			}
		}
		break;
	case 2:
		{
			// start
			g_MatchEventManager.SetCurrentMatchInfo(&packet->fm_info);
			g_MatchEventManager.SetCurrentState(2);

			int fromlevel = g_UserLevelTable[packet->fm_info.fm_level].level;
			int tolevel = g_UserLevelTable[packet->fm_info.to_level].level;

			if(fromlevel == tolevel)
			{
				TCHAR messagestr[512];
				memset(messagestr, 0, sizeof(messagestr));

				_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3740), g_LevelName[fromlevel]);//%s ����ܰ�  �е��� �պ� �������� ���ֽñ�  �ٶ��ϴ�.
				InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
			}
			else
			{
				TCHAR messagestr[512];
				memset(messagestr, 0, sizeof(messagestr));

				_snprintf(messagestr, sizeof(messagestr), _XGETINTERFACETEXT(ID_STRING_NEW_3741), g_LevelName[fromlevel], g_LevelName[tolevel]);	//%s-%s ����ܰ� �е��� �պ� �������� ���ֽñ� �ٶ��ϴ�.
				InsertChatString(messagestr, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
			}
			
			DWORD starttime = g_LocalSystemTime - (packet->usElapsedTime*1000);	// ���۽ð� = ����ð� - �������ð�
			g_MatchEventManager.SetEventStartTime(starttime);

			_XWindow_FreeMatchInfo* pFMInfo_Window = (_XWindow_FreeMatchInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_FREEMATCHINFO);
			if(pFMInfo_Window && pFMInfo_Window->GetShowStatus())
			{
				int year = packet->fm_info.start_time.uiYear + 2000;
				int month = packet->fm_info.start_time.uiMonth;
				int day = packet->fm_info.start_time.uiDay;
				int hour = packet->fm_info.start_time.uiHour;
				int minute = packet->fm_info.start_time.uiMinute;

				_snprintf(pFMInfo_Window->m_szDate, sizeof(pFMInfo_Window->m_szDate), g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_2654, 
					&year ,&month, &day, &hour, &minute));

				int fromlevel = g_UserLevelTable[packet->fm_info.fm_level].level;
				int tolevel = g_UserLevelTable[packet->fm_info.to_level].level;

				if(fromlevel == tolevel)
				{
					_snprintf(pFMInfo_Window->m_szLevel, sizeof(pFMInfo_Window->m_szLevel), "%s", g_LevelName[fromlevel]);
				}
				else
				{
					_snprintf(pFMInfo_Window->m_szLevel, sizeof(pFMInfo_Window->m_szLevel), "%s-%s", g_LevelName[fromlevel], g_LevelName[tolevel]);
				}
			}
		}
		break;
	case 3:
		{
			// end
			g_MatchEventManager.SetCurrentMatchInfo(&packet->fm_info);
			g_MatchEventManager.SetCurrentState(3);

			g_pLocalUser->m_bEnterMatchEvent = FALSE;
			g_MatchEventManager.SetEventStartTime(0);

			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3743), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);	//"�񹫴�ȸ�� �Ϸ�Ǿ����ϴ�."

			_XWindow_PK* pPK_Window = (_XWindow_PK*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKWINDOW);
			if(pPK_Window)
			{
				pPK_Window->ShowWindow(FALSE);
			}

			_XWindow_MatchEvent* pMatchEvent_Window = (_XWindow_MatchEvent*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHEVENT);
			if(pMatchEvent_Window)
			{
				pMatchEvent_Window->ShowWindow(FALSE);
			}

			_XWindow_FreeMatchInfo* pFMInfo_Window = (_XWindow_FreeMatchInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_FREEMATCHINFO);
			if(pFMInfo_Window && pFMInfo_Window->GetShowStatus())
			{
				int year = packet->fm_info.start_time.uiYear + 2000;
				int month = packet->fm_info.start_time.uiMonth;
				int day = packet->fm_info.start_time.uiDay;
				int hour = packet->fm_info.start_time.uiHour;
				int minute = packet->fm_info.start_time.uiMinute;

				_snprintf(pFMInfo_Window->m_szDate, sizeof(pFMInfo_Window->m_szDate), g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_2654, 
					&year ,&month, &day, &hour, &minute));

				int fromlevel = g_UserLevelTable[packet->fm_info.fm_level].level;
				int tolevel = g_UserLevelTable[packet->fm_info.to_level].level;

				if(fromlevel == tolevel)
				{
					_snprintf(pFMInfo_Window->m_szLevel, sizeof(pFMInfo_Window->m_szLevel), "%s", g_LevelName[fromlevel]);
				}
				else
				{
					_snprintf(pFMInfo_Window->m_szLevel, sizeof(pFMInfo_Window->m_szLevel), "%s-%s", g_LevelName[fromlevel], g_LevelName[tolevel]);
				}
			}
		}
		break;
	default :
		{
			// next schedule
			_XWindow_FreeMatchInfo* pFMInfo_Window = (_XWindow_FreeMatchInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_FREEMATCHINFO);
			if(pFMInfo_Window)
			{
				int year = packet->fm_info.start_time.uiYear + 2000;
				int month = packet->fm_info.start_time.uiMonth;
				int day = packet->fm_info.start_time.uiDay;
				int hour = packet->fm_info.start_time.uiHour;
				int minute = packet->fm_info.start_time.uiMinute;

				_snprintf(pFMInfo_Window->m_szDate, sizeof(pFMInfo_Window->m_szDate), g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_2654, 
					&year ,&month, &day, &hour, &minute));

				int fromlevel = g_UserLevelTable[packet->fm_info.fm_level].level;
				int tolevel = g_UserLevelTable[packet->fm_info.to_level].level;

				if(fromlevel == tolevel)
				{
					_snprintf(pFMInfo_Window->m_szLevel, sizeof(pFMInfo_Window->m_szLevel), "%s", g_LevelName[fromlevel]);
				}
				else
				{
					_snprintf(pFMInfo_Window->m_szLevel, sizeof(pFMInfo_Window->m_szLevel), "%s-%s", g_LevelName[fromlevel], g_LevelName[tolevel]);
				}
			}
		}
		break;
	}
}

void _XNetwork::MSGFMPrivateScore(MSG_FM_PRIVATE_SCORE* packet)
{
	g_pLocalUser->m_nKillCount = packet->my_record.usKillCount;
	g_pLocalUser->m_nDeadCount = packet->my_record.usDeadCount;
}

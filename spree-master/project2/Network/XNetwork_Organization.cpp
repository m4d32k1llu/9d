#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "XWindowObjectDefine.h"
#include "XNPC.h"
#include "xuser.h"
#include "XWindow_TutorialIcon.h"
#include "xmessagewindow.h"
#include "XSR_STRINGHEADER.H"

void _XNetwork::MSGOrganizationUnitUpdate(MSG_ORGANIZATION_UNIT_UPDATE_* unitupdate)
{
	if( g_pGroup_Window )	
	{
		switch( unitupdate->iCode ) 
		{
		case MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_JOIN_ :
			g_pGroup_Window->JoinOrganizationMember(&unitupdate->_unit, &unitupdate->m_outTime);
			break;
		case MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_DROP_ :		
			g_pGroup_Window->DeleteOrganizationMember(&unitupdate->_unit, &unitupdate->m_outTime);
			break;		
		case MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_KICK_ :
			g_pGroup_Window->DeleteCompulsionOrganizationMember(&unitupdate->_unit, &unitupdate->m_outTime);
			break;
		case MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_ENTRUST_ :
			g_pGroup_Window->UpdateOrganizationEntrust(&unitupdate->_unit);
			break;
		case MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_UPDATE_ :
			{
				g_pGroup_Window->UpdateOrganizationMember(&unitupdate->_unit, &unitupdate->m_outTime);
			}
			break;
		}
	}
}

void _XNetwork::MSGOrganizationInfoResult(MSG_ORGANIZATION_INFO_RES* inforesult)
{
	if( inforesult->bCreateAble )	// ���� â��
	{
		if( g_TutorialInfo )
		{
			if( !g_TutorialInfo->_XTutorial_045 )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_045);
				g_TutorialInfo->_XTutorial_045 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif				
			}
		}

		if( _strcmpi( g_pLocalUser->m_CharacterInfo.charactername , inforesult->_organizationBase.m_cCaptainName ) == 0 )
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1651), TRUE);
		}
		else
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1652), inforesult->_organizationBase.m_cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE);
		}		
	}
	if( g_pGroup_Window )
	{
		g_pGroup_Window->CopyBaseInfo( &inforesult->_organizationBase );
		g_pGroup_Window->CopyMemberList( inforesult->cMemberCount, inforesult->_memberList );
		g_pGroup_Window->CopyMemberOutTimeList( inforesult->cMemberCount, inforesult->outTimeList );
	}
}

void _XNetwork::MSGOrganizationResult(MSG_ORGANIZATION_RESULT* result)
{
	switch(result->iCode) 
	{
	case MSG_ORGANIZATION_RESULT::_OR_DESTROY_ :		// ������ �����Ǿ���..(ucFromMessage �� ��� ����)
		{
			g_pGroup_Window->BreakUpOrganization();
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_ALREADY_INNER_ :				// �̹� ������ ������..
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1653), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_NEED_MORE_MEMBER_ :			// �ʱ� �߱� �ο��� ������(5�� �̻�)
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ERROR3), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_NOT_PARTY_JOINNER_ :			// ��Ƽ ���°� �ƴմϴ�.
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1654), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_CANT_PARTY_LEADER :			// ��Ƽ�常 �����մϴ�.
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1655), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_UNLIKE_MOONPA :				// �ٸ� ���İ� �ֽ��ϴ�.
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1656), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_UNLIKE_ZONE :				// �ٸ� ������ �ֽ��ϴ�.
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_ERROR1), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_USING_NAME :					// ������� �̸��Դϴ�.
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1657), TRUE);
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_ETC_ :						// ��Ÿ �������� ����
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1658), TRUE);
		}		
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_FULL :
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1659), TRUE);	
		}
		break;
	case MSG_ORGANIZATION_RESULT::_FAILED_CODE_NEED_MORE_LEVEL :
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1660), TRUE);	
		}
		break;
	}
}

void _XNetwork::MSGOrganizationClient( MSG_ORGANIZATION_CLIENT_* clientresult )
{
	switch(clientresult->iCode) 
	{
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_INVITE_ :					// Ÿ���� ¯�� �ڱ�� ������ ������� ����
		{
			if(g_bCommunityOptionRefuseOrganization)
			{
				// �ڵ� ����
				SendPacket(MSG_NO_ORGANIZATION_CLIENT, MSG_ORGANIZATION_CLIENT_::_OR_REQ_INVITE_NO_);
			}
			else
			{
				TCHAR message[256];
				memset( message, 0, sizeof(message));
				_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1661), clientresult->cName );
				if( g_pGroup_Window )
					g_pGroup_Window->SetSendTargetName(clientresult->cName);
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), message, TRUE, _XDEF_GROUP_ORGANIZATION_INVITE_OK, _XDEF_GROUP_ORGANIZATION_INVITE_NO);
				pMessageBox->StartTimer(10000, TRUE);
			}
			
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_ENTRUST_ :					// �ϰ� ¯ ������� ����..
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1662), g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName );
			if( g_pGroup_Window )
				g_pGroup_Window->SetSendTargetName(clientresult->cName);
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), message, TRUE, _XDEF_GROUP_ORGANIZATION_ENTRUST_OK, _XDEF_GROUP_ORGANIZATION_ENTRUST_NO);
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_MOONPA_CHECK_ :				// ��Ƽ������ ���İ� �������� �˻��Ѵ�.	
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_INVITE_OK_ :					// ������ �����ϰڴ�.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1663), clientresult->cName, g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_INVITE_NO_ :					// ������ �������� �ʰڴ�.		
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1664), clientresult->cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_ENTRUST_OK_ :				// ¯ �ϰڴ�.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1665), clientresult->cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_ENTRUST_NO_ :				// ¯ ���ϰڴ�.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1666), clientresult->cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_LOGOFF_ :					// ������ �α׿�����..
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1667), clientresult->cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_ALREADY_INNER_ :				// �̹� �ٸ������� ����� �ִ�.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1668), clientresult->cName );
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE );
		}
		break;
	case MSG_ORGANIZATION_CLIENT_::_OR_REQ_MOONPA_OK_ :					// ���İ� �����̴�.
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORKORGANIZATION_1669), TRUE, 
									_XDEF_GROUP_ORGANIZATION_CREATEYES, _XDEF_GROUP_ORGANIZATION_CREATENO);
		}
		break;	
	}
}

void _XNetwork::MSGOrganizationGrobalInfo(MSG_ORGANIZATION_GLOBAL_INFO* grobalinfo)
{
	if( grobalinfo->m_iIndex > 0 )
	{
		_XUSER_ORGANIZATION_GLOBAL_INFO* fineUserInfo = g_Userlist.SearchOrganizationInfo(grobalinfo->m_iIndex);

		if( fineUserInfo )
		{
			fineUserInfo->m_iIndex			= grobalinfo->m_iIndex;
			strcpy( fineUserInfo->m_cName, grobalinfo->m_cName );
			fineUserInfo->m_cProperty		= grobalinfo->m_cProperty;
			fineUserInfo->m_cBaseParty		= grobalinfo->m_cBaseParty;
			fineUserInfo->m_uiExperience	= grobalinfo->m_uiExperience;			
		}
		else
		{
			_XUSER_ORGANIZATION_GLOBAL_INFO* tempInfo = new  _XUSER_ORGANIZATION_GLOBAL_INFO;
			tempInfo->m_iIndex			= grobalinfo->m_iIndex;
			strcpy( tempInfo->m_cName, grobalinfo->m_cName );
			tempInfo->m_cProperty		= grobalinfo->m_cProperty;
			tempInfo->m_cBaseParty		= grobalinfo->m_cBaseParty;
			tempInfo->m_uiExperience	= grobalinfo->m_uiExperience;			
			g_Userlist.AddOrganizationInfo(tempInfo);
		}
	}	
}

void _XNetwork::MSGOrganizationNoticeInfo(MSG_ORGANIZATION_NOTICE* noticeinfo)
{
	if( g_pGroup_Window )	
	{
		g_pGroup_Window->m_NoticeText.reset();
		memcpy( g_pGroup_Window->m_NoticeText.writer, noticeinfo->m_NoticeData.writer, 13 );
		memcpy( g_pGroup_Window->m_NoticeText.writedate, noticeinfo->m_NoticeData.writedate, 15 );
		memcpy( g_pGroup_Window->m_NoticeText.notice, noticeinfo->m_NoticeData.notice, 272 );
		g_pGroup_Window->SelectNoticeMode(_XNOTICEMODE_READ);
	}
}



#ifdef _XTS_ALIANCE_20061018
//////////////////////////////////////////////////////////////////////////
// ���� ���� ��Ŷ.
//////////////////////////////////////////////////////////////////////////
BOOL _XNetwork::Send_Alience(int type, int param1, int param2, int param3, int param4)
{
	switch(type)
	{
	case MSG_NO_ALLY_BASE:
		{			
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Alience Req");
			
			switch(param1) 
			{
			case en_ally_mode_request_ally:		// ������ ��û�� ��,
				{
					int size = sizeof(MSG_ALLY_REQUEST_ALLY);
					MSG_ALLY_REQUEST_ALLY aliencereq;
					ZeroMemory(&aliencereq, size);
					
					aliencereq.header.sLength = size;
					aliencereq.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq.ucMode = (unsigned char)en_ally_mode_request_ally;
					
					//��û�ϴ� ����� ���� ���̵�
					aliencereq.usWhom = (unsigned short)param2;
					
					return _XSendPacket_Encapsulate((char*)&aliencereq);
				}
				break;
			case en_ally_mode_response_yes:		// ���� ������ �ް� Yes�� ������ �� 
				{
					int size = sizeof(MSG_ALLY_MODE_RESPONSE_YES);
					MSG_ALLY_MODE_RESPONSE_YES alienceresponse_yes;
					ZeroMemory(&alienceresponse_yes, size);
					
					alienceresponse_yes.header.sLength = size;
					alienceresponse_yes.ucMessage = MSG_NO_ALLY_BASE;
					alienceresponse_yes.ucMode = (unsigned char)en_ally_mode_response_yes;
					
					return _XSendPacket_Encapsulate((char*)&alienceresponse_yes);
				}
				break;
			case en_ally_mode_response_no:		// ���� ������ �ް� No�� ������ �� 
				{
					int size = sizeof(MSG_ALLY_MODE_RESPONSE_NO);
					MSG_ALLY_MODE_RESPONSE_NO alienceresponse_no;
					ZeroMemory(&alienceresponse_no, size);
					
					alienceresponse_no.header.sLength = size;
					alienceresponse_no.ucMessage = MSG_NO_ALLY_BASE;
					alienceresponse_no.ucMode = (unsigned char)en_ally_mode_response_no;
					
					return _XSendPacket_Encapsulate((char*)&alienceresponse_no);
				}
				break;
			case en_ally_mode_request_ally_cancel:		// ���� ������ ���´ٰ� ������ �ޱ����� ���� ������ ����� ��
				{
					int size = sizeof(MSG_ALLY_MODE_REQUEST_ALLY_CANCEL);
					MSG_ALLY_MODE_REQUEST_ALLY_CANCEL aliencereq_cancle;
					ZeroMemory(&aliencereq_cancle, size);
					
					aliencereq_cancle.header.sLength = size;
					aliencereq_cancle.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq_cancle.ucMode = (unsigned char)en_ally_mode_request_ally_cancel;
					
					return _XSendPacket_Encapsulate((char*)&aliencereq_cancle);
				}
				break;								
			case en_ally_mode_request_ally_info:		// ������ �� ���� ��û
				{
					int size = sizeof(MSG_ALLY_MODE_REQUEST_ALLY_INFO);
					MSG_ALLY_MODE_REQUEST_ALLY_INFO aliencereq_info;
					ZeroMemory(&aliencereq_info, size);
					
					aliencereq_info.header.sLength = size;
					aliencereq_info.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq_info.ucMode = (unsigned char)en_ally_mode_request_ally_info;
					
					aliencereq_info.iAllyIndex	= (int)param2;	// ��û�ϴ� ������ �ε���.
					
					return _XSendPacket_Encapsulate((char*)&aliencereq_info);
				}
				break;		
			case en_ally_mode_req_leave:		// Ż�� ��û
				{
					int size = sizeof(MSG_ALLY_MODE_REQ_LEAVE);
					MSG_ALLY_MODE_REQ_LEAVE aliencereq_leave;
					ZeroMemory(&aliencereq_leave, size);
					
					aliencereq_leave.header.sLength = size;
					aliencereq_leave.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq_leave.ucMode = (unsigned char)en_ally_mode_req_leave;
					
					return _XSendPacket_Encapsulate((char*)&aliencereq_leave);
				}
				break;
			case en_ally_mode_req_scatter:		// �ػ� ��û
				{
					int size = sizeof(MSG_ALLY_MODE_REQ_SCATTER);
					MSG_ALLY_MODE_REQ_SCATTER aliencereq_scatter;
					ZeroMemory(&aliencereq_scatter, size);
					
					aliencereq_scatter.header.sLength = size;
					aliencereq_scatter.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq_scatter.ucMode = (unsigned char)en_ally_mode_req_scatter;
					
					return _XSendPacket_Encapsulate((char*)&aliencereq_scatter);
				}
				break;
			case en_ally_mode_request_banish:		// �߹� ��û	
				{
					int size = sizeof(MSG_ALLY_MODE_REQUEST_BANISH);
					MSG_ALLY_MODE_REQUEST_BANISH aliencereq_leave;
					ZeroMemory(&aliencereq_leave, size);
					
					aliencereq_leave.header.sLength = size;
					aliencereq_leave.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq_leave.ucMode = (unsigned char)en_ally_mode_request_banish;
					
					//�߹��Ϸ��� �������� �ε���
					aliencereq_leave.iOrgIndex = (int)param2;

					return _XSendPacket_Encapsulate((char*)&aliencereq_leave);
				}
				break;
			case en_ally_mode_request_other_ally:		// �ٸ� ������ ���� ��û	
				{
					int size = sizeof(MSG_ALLY_MODE_REQUEST_OTHER_ALLY);
					MSG_ALLY_MODE_REQUEST_OTHER_ALLY aliencereq_otherinfo;
					ZeroMemory(&aliencereq_otherinfo, size);
					
					aliencereq_otherinfo.header.sLength = size;
					aliencereq_otherinfo.ucMessage = MSG_NO_ALLY_BASE;
					aliencereq_otherinfo.ucMode = (unsigned char)en_ally_mode_request_other_ally;
					
					// ���� �ε���
					aliencereq_otherinfo.iAllyIndex = (int)param2;
					
					return _XSendPacket_Encapsulate((char*)&aliencereq_otherinfo);
				}
				break;
			}
			
		}
		break;
	}
	
	return TRUE;
}

// ������ ���� ��û�� �Դ�.
void _XNetwork::MSGAlienceRequestFrom(MSG_ALLY_REQEUST_FROM* alience_request_from)
{
//	u_short         usFrom;
//	u_char          ucType;   // 0 : ������, 1 :����
//	char            cName[_XDEF_ORGANIZATIONNAMELENGTH+1];
	if( alience_request_from->ucType == 0 )
	{
		TCHAR message[256];
		memset( message, 0, sizeof(message) );
		_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3110), alience_request_from->cName );

		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		if( pMessageBox )	
		{			
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), message, TRUE, _XDEF_GROUP_ALIENCE_BTN_INVITE_OK, _XDEF_GROUP_ALIENCE_BTN_INVITE_NO);
			pMessageBox->StartTimer(14000, TRUE);
		}

	}
	else if( alience_request_from->ucType == 1 )
	{
		TCHAR message[256];
		memset( message, 0, sizeof(message) );
		_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3114), alience_request_from->cName );
		
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		if( pMessageBox )	
		{			
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), message, TRUE, _XDEF_GROUP_ALIENCE_BTN_INVITE_OK, _XDEF_GROUP_ALIENCE_BTN_INVITE_NO);
			pMessageBox->StartTimer(14000, TRUE);
		}

	}
}

//������ �������� No
void _XNetwork::MSGAlienceRequestNo(MSG_ALLY_MODE_RESPONSE_NO* alience_request_no)
{
	TCHAR message[256];
	memset( message, 0, sizeof(message) );
	_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3417)); //"���� ��û�� �ź��Ͽ����ϴ�."
	InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
	
}

// ������ ������ ���������.
void _XNetwork::MSGAlienceMade(MSG_ALLY_MODE_IS_MADE* alience_made)
{
	//  int				iAllyIndex;     // ������� ������ �ε���.
	//	int				iOr1Index;		// ������ ������ ������ ������ �̸�
	//	char			iOr1Name[_XDEF_ORGANIZATIONNAMELENGTH+1];
	//	
	//	int				iOr2Index;		// ������ ���Ϳ� �����ϴ� ������ �̸�
	//	char			iOr2Name[_XDEF_ORGANIZATIONNAMELENGTH+1];

	if( g_pGroup_Window )
	{
		// �����ϴ� ������ �϶� 
		if( g_pGroup_Window->m_OrganizationBaseInfo.m_iIndex == alience_made->iOr2Index )
		{
			// ������ ���Ϳ� ���ԵǾ��ٴ� ���� �˸���.
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3115), alience_made->iOr1Name );			
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
		}
		else // ������ ���Ϳ��϶� 
		{
			// �����ϴ� �����ܰ� ������ �ξ��ٴ� ���� �˸���.
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3113), alience_made->iOr2Name );			
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
		}
	}

}

//  �ڽ��� �������� ���Ϳ� ���ԉ��.
void _XNetwork::MSGAlienceEnter(MSG_ALLY_MODE_ORG_ENTER_ALLY* alience_enter)
{
	
	//	// ucNoOrg �� ����, ��������.ucNoOrg �� �߶� �Ľ��� ��.
	//	int		iAllyIndex;		// ���� �ε���.
	//	char					cMasterName[13];	// ȸ�� �̸�
	//	u_char	ucNoOrg;
	//	__ally_client_info	info[5];               //
	//		
	if( g_pGroup_Window )
	{		
		//������ �ޱ� ���� ������ ������ �����.
		//���߿� AlienceReset()�̶�� �Լ��� �����.
		g_pGroup_Window->ResetAlienceInfo();

		g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex = g_pGroup_Window->m_UniqueIndexAlience = alience_enter->iAllyIndex;
		strcpy(g_pGroup_Window->m_AlienceMasterName, alience_enter->cMasterName );
		
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );

		if( pChatDefault_Window->m_pChatFilterButton[3] )
		{
			if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 || g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
			{
				pChatDefault_Window->m_pChatFilterButton[3]->EnableWindow( TRUE );
			}
			else
			{
				pChatDefault_Window->m_pChatFilterButton[3]->EnableWindow( FALSE );
			}
		}
#endif
				

		for(int i =0; i < alience_enter->ucNoOrg; ++i) 
		{
			__ally_client_info* pAlienceMember = new __ally_client_info;
			pAlienceMember->iOrIndex = alience_enter->info[i].iOrIndex;
			strcpy(pAlienceMember->iOrName, alience_enter->info[i].iOrName );
			
			// �� ó���� ��ȸ �̸��̴�.
			if( i == 0 )
			{
				// ��ȸ�� �̸��� �ִ´�.
				strcpy(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName, alience_enter->info[i].iOrName );
				
				// ȸ������ �˻��Ѵ�.
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex == alience_enter->info[i].iOrIndex  )
				{
					if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_)
					{
						//ȸ���̴�.
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_;
					}
					else//�⺻ ��ȸ��
					{
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
					}
				}
			}
			else
			{		
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex == alience_enter->info[i].iOrIndex  )
				{
					if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_  )		
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_;
					else	
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
				}
			}
			
			g_pGroup_Window->m_listAlienceMember.push_back(pAlienceMember);
		}
		
		if( g_pGroup_Window->GetShowStatus() && g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS2 )
			g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS2);
	}
}

//  ������ ��� ����.
void _XNetwork::MSGAlienceAllInfo(MSG_ALLY_MODE_ALLY_ALL_INFO* alience_allinfo)
{
	
//	// ucNoOrg �� ����, ��������.ucNoOrg �� �߶� �Ľ��� ��.
//	int		iAllyIndex;		// ���� �ε���.
//	char					cMasterName[13];	// ȸ�� �̸�
//	u_char	ucNoOrg;
//	__ally_client_info	info[5];               //
//		
	if( g_pGroup_Window )
	{
		
		//������ �ޱ� ���� ������ ������ �����.
		//���߿� AlienceReset()�̶�� �Լ��� �����.
		g_pGroup_Window->ResetAlienceInfo();

		g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex = g_pGroup_Window->m_UniqueIndexAlience = alience_allinfo->iAllyIndex;
		
		strcpy(g_pGroup_Window->m_AlienceMasterName, alience_allinfo->cMasterName );

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );

		if( pChatDefault_Window->m_pChatFilterButton[3] )
		{
			if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 || g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
			{
				pChatDefault_Window->m_pChatFilterButton[3]->EnableWindow( TRUE );
			}
			else
			{
				pChatDefault_Window->m_pChatFilterButton[3]->EnableWindow( FALSE );
			}
		}
#endif

		for(int i =0; i < alience_allinfo->ucNoOrg; ++i) 
		{
			__ally_client_info* pAlienceMember = new __ally_client_info;
			pAlienceMember->iOrIndex = alience_allinfo->info[i].iOrIndex;
			strcpy(pAlienceMember->iOrName, alience_allinfo->info[i].iOrName );
			
			// �� ó���� ��ȸ �̸��̴�.
			if( i == 0 )
			{
				// ��ȸ�� �̸��� �ִ´�.
				strcpy(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName, alience_allinfo->info[i].iOrName );

				// ȸ������ �˻��Ѵ�.
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex == alience_allinfo->info[i].iOrIndex  )
				{
					if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_)
					{
						//ȸ���̴�.
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_;
					}
					else//�⺻ ��ȸ��
					{
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
					}
				}
			}
			else
			{
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex == alience_allinfo->info[i].iOrIndex  )
				{
					if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_  )		
						g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_;
					else	
					g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
				}
			}

			g_pGroup_Window->m_listAlienceMember.push_back(pAlienceMember);
		}
		
		if( g_pGroup_Window->GetShowStatus() && g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS2 )
			g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS2);
	}
}

// ���ο� �������� ���Դ�.
void _XNetwork::MSGAlienceNewMember(MSG_ALLY_MODE_NOTICE_NEW_ORG* alience_newmember)
{
	//__ally_client_info	addInfo;              // �߰��Ǵ� �������� ����.
	
	if( g_pGroup_Window )
	{
		__ally_client_info* pAlienceMember = new __ally_client_info;
		pAlienceMember->iOrIndex = alience_newmember->addInfo.iOrIndex;
		strcpy(pAlienceMember->iOrName, alience_newmember->addInfo.iOrName );
		
		g_pGroup_Window->m_listAlienceMember.push_back(pAlienceMember);
	}
}

// �������� Ż���ߴ�.
void _XNetwork::MSGAlienceLeaveMember(MSG_ALLY_MODE_NOTICE_LEAVE_ORG* alience_leavemember)
{
	//int	iLeaveOrgIndex;               // Ż���ϴ� �������� �ε���.


	if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex == alience_leavemember->iLeaveOrgIndex )
	{
		// ��ȸ���� Ż���Ͽ����ϴ�.
		TCHAR message[256];
		memset( message, 0, sizeof(message) );
		_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3122), g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName );			
		InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);

		g_pGroup_Window->ResetAlienceInfo();

		//Author : ����� //breif: â�� ������ �־ ���� //Date : 08/03/06
		//if( g_pGroup_Window->GetShowStatus() && g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS2 )
		//	g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS2);
		
	}
	else
	{
		__ally_client_info* aliencemember = NULL;
		list <__ally_client_info*>::iterator iter_alience;
		for(iter_alience = g_pGroup_Window->m_listAlienceMember.begin() ; iter_alience != g_pGroup_Window->m_listAlienceMember.end() ; )
		{
			aliencemember = *iter_alience;
			if( aliencemember )
			{
				if( aliencemember->iOrIndex == alience_leavemember->iLeaveOrgIndex )
				{				
					// ��ȸ���� �������� Ż���Ͽ����ϴ�.
					TCHAR message[256];
					memset( message, 0, sizeof(message) );
					_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3121), aliencemember->iOrName );			
					InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					
					SAFE_DELETE(aliencemember);
					iter_alience = g_pGroup_Window->m_listAlienceMember.erase(iter_alience);
					break;
				}
				else
				{
					++iter_alience;
				}
			}
		}

		//if( g_pGroup_Window->GetShowStatus() && g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS2 )
		//	g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS2);
	}

	g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS2);
}

// ������ ��ü�Ǿ���.
void _XNetwork::MSGAlienceScattered(MSG_ALLY_MODE_SCATTERED* alience_scattered)
{
	//u_char	ucScatterReason;            // 0 - �������� ��ü. 1 - ������ Ż�� ���� ��ü.
	//	if( alience_scattered->ucScatterReason == 0 )
	//	{
	//	}
	//	else if( alience_scattered->ucScatterReason == 1 )
	//	{
	//	}
	//	TCHAR message[256];
	//	memset( message, 0, 256 );
	//	sprintf( message, _XGETINTERFACETEXT(ID_STRING_NEW_3123), );			
	//	InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE);
	
	//���߿� AlienceReset()�̶�� �Լ��� �����.
	if( g_pGroup_Window )
	{
		// ��ȸ�� ��ü�Ǿ����ϴ�.
		TCHAR message[256];
		memset( message, 0, sizeof(message) );
		_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3123), g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName );			
		InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		
		g_pGroup_Window->ResetAlienceInfo();
		if( g_pGroup_Window->GetShowStatus() && g_pGroup_Window->m_SelectedTab == _XGROUPTAB_GROUPCLASS2 )
			g_pGroup_Window->ChangeTab(_XGROUPTAB_GROUPCLASS2);
	}
}

// Ż�� ��û ����
void _XNetwork::MSGAlienceAcceptLeave(MSG_ALLY_MODE_ALLOW_LEAVE* alienceaccept_leave)
{	
}

// �ػ� ��û ����
void _XNetwork::MSGAlienceAcceptScatter(MSG_ALLY_MODE_ALLOW_SCATTER* alienceaccept_scatter)
{	
}
// �߹�Ǿ��ٰ�, �����ֿ��� �˷���.
void _XNetwork::MSGAlienceLeaveResult(MSG_ALLY_MODE_BANISH_COMPLETE* alience_leaveresult)
{	
	//int		iOrgIndex;                      // 
}
// �ٸ� ������ ����	
void _XNetwork::MSGAlienceOtherInfo(MSG_ALLY_MODE_OTHER_ALLY_INFO * alience_otherinfor)
{	
	//int		iAllyIndex;
	//char	cAllyName[_XDEF_ORGANIZATIONNAMELENGTH+1];
	//�ֺ� �ι��� ��ȸ ������ �����ϰų� �߰��Ѵ�.
	if( alience_otherinfor->iAllyIndex > 0 )
	{
		_XUSER_ALIENCE_GLOBAL_INFO* fineUserInfo = g_Userlist.SearchAlienceInfo(alience_otherinfor->iAllyIndex);
		
		if( fineUserInfo )
		{
			fineUserInfo->m_iIndex	= alience_otherinfor->iAllyIndex;
			strcpy( fineUserInfo->m_cMasterName, alience_otherinfor->cAllyMaster);	
			strcpy( fineUserInfo->m_cName, alience_otherinfor->cAllyName );	
		}
		else
		{
			_XUSER_ALIENCE_GLOBAL_INFO* tempInfo = new  _XUSER_ALIENCE_GLOBAL_INFO;
			tempInfo->m_iIndex	= alience_otherinfor->iAllyIndex;
			strcpy( tempInfo->m_cMasterName, alience_otherinfor->cAllyMaster);	
			strcpy( tempInfo->m_cName, alience_otherinfor->cAllyName );
			g_Userlist.AddAlienceInfo(tempInfo);
		}
	}
	
}
// �������� �߹�Ǿ���.
void _XNetwork::MSGAlienceNoticeLeave(MSG_ALLY_MODE_NOTICE_BANISH_ORG* alience_noticeleave)
{	
	//int		iOrgIndex;
	//char	cOrgName[_XDEF_ORGANIZATIONNAMELENGTH+1];

}
// ������ ������ ����.
void _XNetwork::MSGAlienceNoInfo(MSG_ALLY_MODE_NO_ALLY_INFO* alience_noinfo)
{	
	//int	iAllyIndex;			// �ش� ������ ������ ����. 
	TCHAR message[256];
	memset( message, 0, sizeof(message) );
	_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3175) );			
	InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
}

//������ ���Ϳ� ���õ� �����޼���
void _XNetwork::MSGAlienceErrorMessage(MSG_ALLY_ERROR* alience_error)
{
	switch(alience_error->ucMode) {
	case en_ally_mode_error_we_already_ally :		
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3176) );			//�̹� ��ȸ�� ���� �ֽ��ϴ�
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_they_already_ally :		// ��� �������� �̹� ������ �ΰ� �ִ�.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3177) );			//��� �������� �̹� ��ȸ�� ���� �ֽ��ϴ�.
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_cannot_ally :			// ��/�鵵 �� ������ ������ ���� �� ����.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3178) );			//�浵�� �鵵�� ������ ���� �� �����ϴ�
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_not_found_info :		// ��û�� ������ ã�� �� �����ϴ�.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3179) );			//��û�� ��ȸ�� ã�� �� �����ϴ�.
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;		
	case en_ally_mode_error_cannot_find_user :		// ������ ã�� �� ����.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3180) );			//������ ã�� �� �����ϴ�.
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_i_am_busy :				// ���� ��û��..
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3181)  );			//_T("���� ��ȸ ��û �� �Դϴ�.")
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_busy :					// ����� �ٸ� �� ��.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3182) );			//_T("����� �ٸ� ���� �ϰ� �ֽ��ϴ�.")
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_cannot_request :		// �������� ���ų�, ���ְ� �ƴϴ�.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3183) );			//_T("�����ܿ� �ҼӵǾ� ���� �ʽ��ϴ�.")
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_not_hero :				// ����� ������ ���ְ� �ƴϴ�.
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3184) );			//_T("������ ���ְ� �ƴմϴ�.")
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_in_combat :
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3394) );	//"���� ����� ��û �����̹Ƿ� ��ȸ�� �Ἲ�� �� �����ϴ�."
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;
	case en_ally_mode_error_target_in_combat :
		{
			TCHAR message[256];
			memset( message, 0, sizeof(message) );
			_snprintf( message, sizeof(message), _XGETINTERFACETEXT(ID_STRING_NEW_3395) );	//"��� �������� ���� ����� ��û �����̹Ƿ� ��ȸ�� �Ἲ�� �� �����ϴ�."
			InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		break;

	}

}

#endif
// XNetwork_PKPenalty.cpp : PK �г�Ƽ ���� 
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "SoundEffectList.h"
#include "XSR_STRINGHEADER.H"
#include "XMessageWindow.h"
#include "XWindowObjectDefine.h"

#include "XWindow_WorldMinimap.h"
#include "XWindow_PKTracingRequest.h"
#include "XWindow_PK_ListBox.h"
#include "XWindow_Rebirth.h"

#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK����ġ����, ������ PK����

void _XNetwork::MSGPKLevelPenaltyInfo(MSG_LTS_LVPK_INFO_GS_CL* pklevelinfo)						//������ �������
{
	if( pklevelinfo->ucCount == 0 ) return;	
	if( !g_pLocalUser->m_listPKTracing.empty() )
	{
		_XPKTRACING_INFO* pktracing = NULL;
		list <_XPKTRACING_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
		{
			pktracing = *iter_pktracing;
			if( pktracing->ucPKType == 1 )
			{
				SAFE_DELETE( pktracing );
				iter_pktracing = g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
			}
			else
				++iter_pktracing;
		}
	}

	for(int i =0; i < pklevelinfo->ucCount; ++i) 
	{		
		_XPKTRACING_INFO* pPKTracingItem = new _XPKTRACING_INFO;
		pPKTracingItem->ucPKType =  1;																				// 1:PK������ �г�Ƽ 2:PK���� 3:Ư���� ���� 
		pPKTracingItem->ucTimeMode = pklevelinfo->LvPkData[i].ucTimeMode;											// 0:1�ð� 1:24�ð� 2:48�ð�
		pPKTracingItem->nStartTrackingTime = pklevelinfo->LvPkData[i].iStartTrackingTime;							//���� ���� �ð�
		strncpy( pPKTracingItem->szCharacName, pklevelinfo->LvPkData[i].szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//������ �̸�
		
		g_pLocalUser->m_listPKTracing.push_back(pPKTracingItem);
	}
}


void _XNetwork::MSGPKLevelPenaltyInsert(MSG_LTS_LVPK_INSERT_GS_CL* pklevel)						//������ �г�Ƽ ������ ����Ʈ �߰�
{
	BOOL bExistData = FALSE;
	if( !g_pLocalUser->m_listPKTracing.empty() )
	{
		_XPKTRACING_INFO* pktracing = NULL;
		list <_XPKTRACING_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
		{
			pktracing = *iter_pktracing;
			if( pktracing->ucPKType == 1 )
			{
				if( strcmp( pklevel->PkData.szCharacName, pktracing->szCharacName ) == 0 )
				{
					//�̹� �ִ� ����Ÿ�̴�.
					pktracing->ucTimeMode = pklevel->PkData.ucTimeMode;						// 0:1�ð� 1:24�ð� 2:48�ð�
					pktracing->nStartTrackingTime = pklevel->PkData.iStartTrackingTime;		//���� ���� �ð�
					bExistData = TRUE;
					break;
				}
				else
					++iter_pktracing;
			}
			else
				++iter_pktracing;
		}
	}
	if( !bExistData )
	{
		_XPKTRACING_INFO* pPKTracingItem = new _XPKTRACING_INFO;
		pPKTracingItem->ucPKType =  1;																			// 1:PK������ �г�Ƽ 2:PK���� 3:Ư���� ���� 
		pPKTracingItem->ucTimeMode = pklevel->PkData.ucTimeMode;											// 0:1�ð� 1:24�ð� 2:48�ð�
		pPKTracingItem->nStartTrackingTime = pklevel->PkData.iStartTrackingTime;							//���� ���� �ð�
		strncpy( pPKTracingItem->szCharacName, pklevel->PkData.szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//������ �̸�
		
		g_pLocalUser->m_listPKTracing.push_back(pPKTracingItem);
	}
}

void _XNetwork::MSGPKLevelPenaltyDelete(MSG_LTS_LVPK_DEL_REQ_GS_CL* pklevel)			//���������Ƽ ����Ʈ ���� ����
{
	
	_XPKTRACING_INFO* pktracing = NULL;
	list <_XPKTRACING_INFO*>::iterator iter_pktracing;
	for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
	{
		pktracing = *iter_pktracing;
		if( pktracing->ucPKType = 1 )
		{
			if( strcmp( pktracing->szCharacName, pklevel->szDelCharacName) == 0 )
			{
				SAFE_DELETE( pktracing );
				iter_pktracing = g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
				break;
			}
			else
				++iter_pktracing;
		}
		else
			++iter_pktracing;
	}
	
	_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
	if(pPKTracingRequest_Window)
	{
		memset(pPKTracingRequest_Window->m_cDeleteTargetName,0,sizeof(TCHAR)*13);
	}
}

void _XNetwork::MSGPKTracingInfo(MSG_LTS_PK_INFO_GS_CL* pktracinginfo)							//PK�� �������
{	
	if( pktracinginfo->ucCount == 0 ) return;	
	if( !g_pLocalUser->m_listPKTracing.empty() )
	{
		_XPKTRACING_INFO* pktracing = NULL;
		list <_XPKTRACING_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
		{
			pktracing = *iter_pktracing;
			if( pktracing->ucPKType == 2 )
			{
				SAFE_DELETE( pktracing );
				iter_pktracing = g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
			}
			else
				++iter_pktracing;
		}
	}
	for(int i =0; i < pktracinginfo->ucCount; ++i) 
	{
		_XPKTRACING_INFO* pPKTracingItem = new _XPKTRACING_INFO;
		pPKTracingItem->ucPKType =  2;																		// 1:PK������ �г�Ƽ 2:PK���� 3:Ư���� ���� 
		pPKTracingItem->ucTimeMode = pktracinginfo->PkData[i].ucTimeMode;												// 0:1�ð� 1:24�ð� 2:48�ð�
		pPKTracingItem->nStartTrackingTime = pktracinginfo->PkData[i].iStartTrackingTime;								//���� ���� �ð�
		strncpy( pPKTracingItem->szCharacName, pktracinginfo->PkData[i].szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//������ �̸�

		g_pLocalUser->m_listPKTracing.push_back(pPKTracingItem);
	}

#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->SetListItem();
		if(!pPKListbox_Window->GetShowStatus())
			pPKListbox_Window->ShowWindow(TRUE);
	}
#endif
}




void _XNetwork::MSGPKTracingInsertSuccess(MSG_LTS_PK_INSERT_SUCCESS_GS_CL* pktracinginfo)		//PK�� ����Ʈ �߰� ����
{
	
	BOOL bExistData = FALSE;
	if( !g_pLocalUser->m_listPKTracing.empty() )
	{
		_XPKTRACING_INFO* pktracing = NULL;
		list <_XPKTRACING_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
		{
			pktracing = *iter_pktracing;
			if( pktracing->ucPKType == 2 )
			{
				if( strcmp( pktracinginfo->PkData.szCharacName, pktracing->szCharacName ) == 0 )
				{
					//�̹� �ִ� ����Ÿ�̴�.
					pktracing->ucTimeMode = pktracinginfo->PkData.ucTimeMode;						//0:�׽�Ʈ ��� 3�� 1:1�ð� 2:24�ð� 3:48�ð�
					pktracing->nStartTrackingTime = pktracinginfo->PkData.iStartTrackingTime;		//���� ���� �ð�
					bExistData = TRUE;
					break;
				}
				else
					++iter_pktracing;
			}
			else
				++iter_pktracing;
		}
	}
	
	if( !bExistData )
	{
		_XPKTRACING_INFO* pPKTracingItem = new _XPKTRACING_INFO;
		pPKTracingItem->ucPKType =  2;																				// 1:PK������ �г�Ƽ 2:PK���� 3:Ư���� ���� 
		pPKTracingItem->ucTimeMode = pktracinginfo->PkData.ucTimeMode;												// 0:1�ð� 1:24�ð� 2:48�ð�
		pPKTracingItem->nStartTrackingTime = pktracinginfo->PkData.iStartTrackingTime;								//���� ���� �ð�
		strncpy( pPKTracingItem->szCharacName, pktracinginfo->PkData.szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//������ �̸�
		g_pLocalUser->m_listPKTracing.push_back(pPKTracingItem);
	}
	
	_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
	if(pPKTracingRequest_Window)
	{
		memset(pPKTracingRequest_Window->m_cTargetName,0,sizeof(TCHAR)*13);
	}

#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->SetListItem();
		if(!pPKListbox_Window->GetShowStatus())
			pPKListbox_Window->ShowWindow(TRUE);
	}
#endif

	//�������� ���� ������ �����Ѵ�.
	g_pLocalUser->m_Money = pktracinginfo->uiRemainMoney;
	
}


void _XNetwork::MSGPKTracingInsertFail(MSG_LTS_PK_INSERT_FAILED_GS_CL* pktracinginfo)			//PK�� ����Ʈ �߰� ����
{
	//���� �޼���
	if( pktracinginfo->ucRes == 1 )//���� �����Ҷ� 
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NPCTRADE_NOTENOUGHMONEY), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//�������� �����մϴ�.
	}
	else if( pktracinginfo->ucRes == 2 )//����� �̸��� ����.
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//���õ� ����� ���ų� �߸��� ����Դϴ�.
	}
	else if( pktracinginfo->ucRes == 3 )//���� PK����Ʈ�� ��ϵǾ� �ִ�.
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_3464), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// _T("�̹� ���� ���� ��Ͽ� ��ϵǾ� �ֽ��ϴ�.")
	}
	else // �׿� ����
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_3465), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// _T("������ �� �����ϴ�.")
	}
}


void _XNetwork::MSGPKTracingDeleteSuccess(MSG_LTS_PK_DEL_SUCCESS_GS_CL* pktracinginfo)			//PK�� ����Ʈ ���� ����
{
	_XWindow_WorldMinimap* pMapWindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
	if( !pMapWindow ) return;
	
	_XPKTRACING_INFO* pktracing = NULL;
	list <_XPKTRACING_INFO*>::iterator iter_pktracing;
	for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
	{
		pktracing = *iter_pktracing;
		if( pktracing->ucPKType == 2)
		{
			if( strcmp( pktracing->szCharacName, pktracinginfo->szDelCharacName ) == 0 )
			{
				SAFE_DELETE( pktracing );
				iter_pktracing = g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
				break;
			}
			else
				++iter_pktracing;
		}
		else
			++iter_pktracing;

	}
	
	
	//������ ǥ�õ� �͵� �����.
	_XPKTRACING_POSITION_INFO* pktracingposition = NULL;
	list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition;
	for(iter_pktracingposition = pMapWindow->m_listPKTracingPosition.begin() ; iter_pktracingposition != pMapWindow->m_listPKTracingPosition.end(); )
	{
		pktracingposition = *iter_pktracingposition;
		if( pktracingposition )
		{
			if( strcmp( pktracingposition->szCharacName, pktracinginfo->szDelCharacName ) == 0 )
			{
				SAFE_DELETE( pktracingposition );
				iter_pktracingposition = pMapWindow->m_listPKTracingPosition.erase(iter_pktracingposition);
				break;
			}
			else
				++iter_pktracingposition;
			
		}
		else
			++iter_pktracingposition;
	}

	if( pMapWindow->m_listPKTracingPosition.empty() )
	{
		pMapWindow->m_PKWarningLevel = 0;
		pMapWindow->m_bDrawPKWarning = FALSE;	
	}

#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->SetListItem();
		if(!pPKListbox_Window->GetShowStatus())
			pPKListbox_Window->ShowWindow(TRUE);
	}
#endif


}


void _XNetwork::MSGSomeOneTracingInfo(MSG_LTS_SP_INFO_GS_CL* tracinginfo)						//Ư���� �������
{
	if( tracinginfo->ucCount == 0 ) return;	
	if( !g_pLocalUser->m_listPKTracing.empty() )
	{
		_XPKTRACING_INFO* pktracing = NULL;
		list <_XPKTRACING_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
		{
			pktracing = *iter_pktracing;
			if( pktracing->ucPKType == 3 )
			{
				SAFE_DELETE( pktracing );
				iter_pktracing = g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
			}
			else
				++iter_pktracing;
		}
	}
	for(int i =0; i < tracinginfo->ucCount; ++i) 
	{
		_XPKTRACING_INFO* pPKTracingItem = new _XPKTRACING_INFO;
		pPKTracingItem->ucPKType =  3;																		// 1:PK������ �г�Ƽ 2:PK���� 3:Ư���� ���� 
		pPKTracingItem->ucTimeMode = tracinginfo->SpData[i].ucTimeMode;												// 0:1�ð� 1:24�ð� 2:48�ð�
		pPKTracingItem->nStartTrackingTime = tracinginfo->SpData[i].iStartTrackingTime;								//���� ���� �ð�
		strncpy( pPKTracingItem->szCharacName, tracinginfo->SpData[i].szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//������ �̸�
		
		g_pLocalUser->m_listPKTracing.push_back(pPKTracingItem);
	}

#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->SetListItem();
		if(!pPKListbox_Window->GetShowStatus())
			pPKListbox_Window->ShowWindow(TRUE);
	}
#endif
}


void _XNetwork::MSGSomeOneTracingInsertSuccess(MSG_LTS_SP_INSERT_SUCCESS_GS_CL* tracinginfo)	//Ư���� ����Ʈ �߰� ����
{
	BOOL bExistData = FALSE;
	if( !g_pLocalUser->m_listPKTracing.empty() )
	{
		_XPKTRACING_INFO* pktracing = NULL;
		list <_XPKTRACING_INFO*>::iterator iter_pktracing;
		for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
		{
			pktracing = *iter_pktracing;
			if( pktracing->ucPKType == 3 )
			{
				if( strcmp( tracinginfo->SpData.szCharacName, pktracing->szCharacName ) == 0 )
				{
					//�̹� �ִ� ����Ÿ�̴�.
					pktracing->ucTimeMode = tracinginfo->SpData.ucTimeMode;						//0:�׽�Ʈ ��� 3�� 1:1�ð� 2:24�ð� 3:48�ð�
					pktracing->nStartTrackingTime = tracinginfo->SpData.iStartTrackingTime;		//���� ���� �ð�
					bExistData = TRUE;
					break;
				}
				else
					++iter_pktracing;
			}
			else
				++iter_pktracing;
		}
	}
	
	if( !bExistData )
	{
		_XPKTRACING_INFO* pPKTracingItem = new _XPKTRACING_INFO;
		pPKTracingItem->ucPKType =  3;																				// 1:PK������ �г�Ƽ 2:PK���� 3:Ư���� ���� 
		pPKTracingItem->ucTimeMode = tracinginfo->SpData.ucTimeMode;												// 0:1�ð� 1:24�ð� 2:48�ð�
		pPKTracingItem->nStartTrackingTime = tracinginfo->SpData.iStartTrackingTime;								//���� ���� �ð�
		strncpy( pPKTracingItem->szCharacName, tracinginfo->SpData.szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//������ �̸�
		g_pLocalUser->m_listPKTracing.push_back(pPKTracingItem);
	}
	
	_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
	if(pPKTracingRequest_Window)
	{
		//Author : �����  �߰�
		memset(pPKTracingRequest_Window->m_cSpecialTargetName, 0, sizeof(pPKTracingRequest_Window->m_cSpecialTargetName));
		//memset(pPKTracingRequest_Window->m_cTargetName,0,sizeof(TCHAR)*13);
	}
	
#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->SetListItem();
		if(!pPKListbox_Window->GetShowStatus())
			pPKListbox_Window->ShowWindow(TRUE);
	}
#endif
	
	//�������� ���� ������ �����Ѵ�.
	g_pLocalUser->m_Money = tracinginfo->uiRemainMoney;
	
}


void _XNetwork::MSGSomeOneTracingInsertFail(MSG_LTS_SP_INSERT_FAILED_GS_CL* tracinginfo)		//Ư���� ����Ʈ �߰� ����
{
	if( tracinginfo->ucRes == 1 )//���� �����Ҷ� 
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NPCTRADE_NOTENOUGHMONEY), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//�������� �����մϴ�.
	}
	else if( tracinginfo->ucRes == 2 )//����� �̸��� ����.
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAME_ERRORTARGET), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//���õ� ����� ���ų� �߸��� ����Դϴ�.
	}
	else if( tracinginfo->ucRes == 3 )//���� PK����Ʈ�� ��ϵǾ� �ִ�.
	{
		g_NetworkKernel.InsertChatString(  _XGETINTERFACETEXT(ID_STRING_NEW_3464), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
	}
	else // �׿� ����
	{
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_3465), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
	}
}


void _XNetwork::MSGSomeOneTracingDeleteSuccess(MSG_LTS_SP_DEL_SUCCESS_GS_CL* tracinginfo)		//Ư���� ����Ʈ ���� ����
{
	_XWindow_WorldMinimap* pMapWindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
	if( !pMapWindow ) return;
	
	_XPKTRACING_INFO* pktracing = NULL;
	list <_XPKTRACING_INFO*>::iterator iter_pktracing;
	for(iter_pktracing = g_pLocalUser->m_listPKTracing.begin() ; iter_pktracing != g_pLocalUser->m_listPKTracing.end() ;)
	{
		pktracing = *iter_pktracing;
		if( pktracing->ucPKType == 3)
		{
			if( strcmp( pktracing->szCharacName, tracinginfo->szDelCharacName ) == 0 )
			{
				SAFE_DELETE( pktracing );
				iter_pktracing = g_pLocalUser->m_listPKTracing.erase(iter_pktracing);
				break;
			}
			else
				++iter_pktracing;
		}
		else
			++iter_pktracing;
		
	}
	
	
	//������ ǥ�õ� �͵� �����.
	_XPKTRACING_POSITION_INFO* pktracingposition = NULL;
	list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition;
	for(iter_pktracingposition = pMapWindow->m_listPKTracingPosition.begin() ; iter_pktracingposition != pMapWindow->m_listPKTracingPosition.end(); )
	{
		pktracingposition = *iter_pktracingposition;
		if( pktracingposition )
		{
			if( strcmp( pktracingposition->szCharacName, tracinginfo->szDelCharacName ) == 0 )
			{
				SAFE_DELETE( pktracingposition );
				iter_pktracingposition = pMapWindow->m_listPKTracingPosition.erase(iter_pktracingposition);
				break;
			}
			else
				++iter_pktracingposition;
			
		}
		else
			++iter_pktracingposition;
	}
	
	if( pMapWindow->m_listPKTracingPosition.empty() )
	{
		pMapWindow->m_PKWarningLevel = 0;
		pMapWindow->m_bDrawPKWarning = FALSE;	
	}

#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->SetListItem();
		if(pPKListbox_Window->GetListBox().GetItemCount() == 0)
		{
			if(pPKListbox_Window->GetShowStatus())
				pPKListbox_Window->ShowWindow(FALSE);
		}
	}
#endif

}


void _XNetwork::MSGTracingPositionInfo(MSG_LTS_POS_INFO_GS_CL* tracinginfo)						//3�ʸ��� PK�� Ư���� (ĳ���� + ��ġ����)
{
	
	_XWindow_WorldMinimap* pMapWindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
	if( !pMapWindow ) return;

	//����Ʈ�� ���ٴ� �ǹ��̹Ƿ� ���� �����Ѵ�.
	if( tracinginfo->ucPkCount == 0 )
	{
		if( !pMapWindow->m_listPKTracingPosition.empty() )
		{
			_XPKTRACING_POSITION_INFO* pktracingposition = NULL;
			list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition;
			for(iter_pktracingposition = pMapWindow->m_listPKTracingPosition.begin() ; iter_pktracingposition != pMapWindow->m_listPKTracingPosition.end(); )
			{
				pktracingposition = *iter_pktracingposition;
				if( pktracingposition )
				{
					SAFE_DELETE( pktracingposition );
					iter_pktracingposition = pMapWindow->m_listPKTracingPosition.erase(iter_pktracingposition);
				}
				else
					++iter_pktracingposition;
			}
			pMapWindow->m_bDrawPKWarning = FALSE;
		}
		return;
	}
	
	_XPKTRACING_POSITION_INFO* pktracingposition = NULL;
	list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition;
	if( !pMapWindow->m_listPKTracingPosition.empty() )
	{
		for(iter_pktracingposition = pMapWindow->m_listPKTracingPosition.begin() ; iter_pktracingposition != pMapWindow->m_listPKTracingPosition.end(); ++iter_pktracingposition)
		{
			pktracingposition = *iter_pktracingposition;
			if( pktracingposition )
			{
				pktracingposition->bRenewalData = FALSE;
			}
		}
	}

	BOOL bExistData = FALSE;
	for(int i =0; i < tracinginfo->ucPkCount; ++i) 
	{		
		bExistData = FALSE;
		//���� ����Ʈ�� �˻��ؼ� �ִ� ����Ÿ�� ��ġ������ �����Ѵ�.
		if( !pMapWindow->m_listPKTracingPosition.empty() )
		{
			pktracingposition = NULL;
			list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition1;
			for(iter_pktracingposition1 = pMapWindow->m_listPKTracingPosition.begin() ; iter_pktracingposition1 != pMapWindow->m_listPKTracingPosition.end(); ++iter_pktracingposition1)
			{
				pktracingposition = *iter_pktracingposition1;
				if( pktracingposition )
				{
					if(strcmp( pktracingposition->szCharacName, tracinginfo->LtsPosData[i].szCharacName ) == 0 )
					{
						pktracingposition->bRenewalData = TRUE;
						pktracingposition->ucPKType = tracinginfo->LtsPosData[i].ucPkOrSpMode;
						pktracingposition->sPosX = tracinginfo->LtsPosData[i].sPosX;
						pktracingposition->sPosZ = tracinginfo->LtsPosData[i].sPosZ;
						
						bExistData = TRUE;
						break;
					}
				}
			}
		}

		//���� ����Ʈ�� ������ ������ ���� �߰��Ѵ�.
		if( !bExistData )
		{
			_XPKTRACING_POSITION_INFO* pPKTracingPositionItem = new _XPKTRACING_POSITION_INFO;
			pPKTracingPositionItem->bRenewalData = TRUE;
			pPKTracingPositionItem->ucPKType = tracinginfo->LtsPosData[i].ucPkOrSpMode;						// 1:PK������ �г�Ƽ 2:PK���� 3:Ư���� ���� 
			pPKTracingPositionItem->sPosX = tracinginfo->LtsPosData[i].sPosX;
			pPKTracingPositionItem->sPosZ = tracinginfo->LtsPosData[i].sPosZ;
			strncpy( pPKTracingPositionItem->szCharacName, tracinginfo->LtsPosData[i].szCharacName, _XDEF_MAX_USERNAMESTRINGLENGTH );//������ �̸�
			
			pMapWindow->m_listPKTracingPosition.push_back(pPKTracingPositionItem);
		}

	}

	
	//������ �ִ� ����Ÿ�� ������ ���� �ʾҰų� ���ο� ����Ÿ�� �ƴ� ��쿡�� ����Ʈ���� �����Ѵ�.
	pktracingposition = NULL;
	//list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition;
	for(iter_pktracingposition = pMapWindow->m_listPKTracingPosition.begin() ; iter_pktracingposition != pMapWindow->m_listPKTracingPosition.end(); )
	{
		pktracingposition = *iter_pktracingposition;
		if( pktracingposition )
		{
			if( !pktracingposition->bRenewalData )
			{
				SAFE_DELETE( pktracingposition );
				iter_pktracingposition = pMapWindow->m_listPKTracingPosition.erase(iter_pktracingposition);
			}
			else
				++iter_pktracingposition;

		}
		else
			++iter_pktracingposition;
	}
	
	if( !pMapWindow->m_listPKTracingPosition.empty() ) 
		pMapWindow->ProcessPKTracingDistance();
	else
		pMapWindow->m_bDrawPKWarning = FALSE;

#ifdef _XDEF_PK_TRACINGSERVICES_20070402
	XWindow_PK_ListBox* pPKListbox_Window = (XWindow_PK_ListBox*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PK_TRACINGSERVICES);
	if(pPKListbox_Window)
	{
		pPKListbox_Window->UpdateLeftTime();
	}
#endif

}

void _XNetwork::MSGPKTargetName(MSG_LTS_PK_INSERT_REQ_GS_CL* pkname)
{
	_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
	if(pPKTracingRequest_Window)
	{
		memset(pPKTracingRequest_Window->m_cTargetName, 0, sizeof(pPKTracingRequest_Window->m_cTargetName));
		strcpy( pPKTracingRequest_Window->m_cTargetName, pkname->szInsertCharacName );
	}

	_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
	if(pRebirth_Window)
		pRebirth_Window->m_bDeadInPK = TRUE;
}

#endif //#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK����ġ����, ������ PK����
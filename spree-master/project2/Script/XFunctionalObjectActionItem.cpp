// XFunctionalObjectActionItem.cpp: implementation of the _XFunctionalObjectActionItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XFunctionalObjectActionItem.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XMessageWindow.h"

#include "XWindow_EnterBossRoom.h"
#include "XWindow_MatchNoticeLarge.h"
#include "XWindow_SystemMsgDefault.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XFunctionalObjectActionItem::_XFunctionalObjectActionItem()
{
	m_lpszMessageString = NULL;
	ResetData();
}

_XFunctionalObjectActionItem::~_XFunctionalObjectActionItem()
{
	if(m_lpszMessageString)
	{
		delete[] m_lpszMessageString;
		m_lpszMessageString = NULL;
	}

}


void _XFunctionalObjectActionItem::ResetData(void)
{
	m_TDA.m_enable			= ID_TRIGGER_ENABLE;
	m_TDA.m_actionclass		= ID_TRIGGER_SETDATA_NONE;

	m_TDA.m_actiondata1		= ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_actiondata2		= ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_actiondata3		= ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_actiondata4		= ID_TRIGGER_SETDATA_NONE;

	m_TDA.m_numericvalue1	= ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_numericvalue2	= ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_numericvalue3	= ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_numericvalue4	= ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_numericvalue5	= ID_TRIGGER_SETDATA_NONE;

	if(m_lpszMessageString)
	{
		delete[] m_lpszMessageString;
		m_lpszMessageString = NULL;
	}

	memset(m_ActionString, 0, sizeof(TCHAR)*64);

	m_ActionID = 0;
}

void _XFunctionalObjectActionItem::SetMessageString(LPCTSTR messagestring)
{
	if(!messagestring)
		return ;

	if(m_lpszMessageString)
	{
		delete[] m_lpszMessageString;
		m_lpszMessageString = NULL;
	}

	int strlength = strlen(messagestring);
	if(strlength > 0)
	{
		m_lpszMessageString = new TCHAR[strlength + 1];
		m_lpszMessageString[strlength] = 0;
		strcpy(m_lpszMessageString, messagestring);
	}
}

void _XFunctionalObjectActionItem::SetActionString(LPCTSTR actionstring)
{
	memset(m_ActionString, 0, sizeof(TCHAR)*64);

	if(!actionstring)
		return;

	int strlength = strlen(actionstring);
	if(strlength < 64)
	{
		strcpy(m_ActionString, actionstring);
	}
	else
	{
		strncpy(m_ActionString, actionstring, 63);
	}
}

BOOL _XFunctionalObjectActionItem::Load(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;

	if(fread(&m_ActionID, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(fread(m_ActionString, sizeof(TCHAR)*64, 1, fileptr) < 1)
		return FALSE;

	if(fread(&m_TDA, sizeof(_XTRIGGERDATA_ACTION), 1, fileptr) < 1)
		return FALSE;

	if(!LoadString(fileptr, m_lpszMessageString))
		return FALSE;

	return TRUE;
}

BOOL _XFunctionalObjectActionItem::LoadString(FILE* fileptr, LPTSTR& string)
{
	int strlength = 0;
	
	if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	
	if(string != NULL)
	{
		delete[] string;
		string = NULL;
	}
	
	if(strlength > 0)
	{
		string = new TCHAR[strlength + 1];
		if(!string)
		{
			return FALSE;
		}
		
		memset(string, 0, sizeof(TCHAR)*(strlength + 1));
		if(fread(string, strlength, 1, fileptr) < 1)
			return FALSE;
	}
	return TRUE;
}

_XFUNCTIONALOBJECT_ACTION_RESULT _XFunctionalObjectActionItem::Process(void)
{
	switch(m_TDA.m_actionclass)
	{
	case ID_OBJECT_ACTION_PLAYERMOVE :					// �÷��̾�_�̵�
		return ProcessPlayerMove();
		
	case ID_OBJECT_ACTION_BOSSMOVE :					// �÷��̾�_�������̵�
		return ProcessEnterBossRoom();
		
	case ID_OBJECT_ACTION_SETATTACKOBJECTSTATE :		// �÷��̾�_������Ʈ���ݰ��ɻ��¼���
		return ProcessEnableAttack();
		
	case ID_OBJECT_ACTION_TEXTDISPLAY :					// TEXT ���
		return ProcessDisplayText();
		
	case ID_OBJECT_ACTION_MATCHINFO :					// �� ������ ���
		return ProcessMatchInfo();
		
	case ID_OBJECT_ACTION_MATCHZONEENTEROREXIT :		// ���� ����/����
		return ProcessMatchZoneEnterExit();

	case ID_OBJECT_ACTION_CONDITIONALATTACTSETTING:		// �÷��̾�_���Ǻ����ݼ���
		return ProcessCondtionalAttactSetting();
				
		
	default:
		break;
	}
	return FUNCTIONALOBJECT_ACTION_RESULT_SUCCESS;
	
}
_XFUNCTIONALOBJECT_ACTION_RESULT _XFunctionalObjectActionItem::ProcessPlayerMove(void)
{
//	m_TDA.m_actiondata1			= m_AreaCategory;
//	m_TDA.m_numericvalue1		= m_AreaID;
//	m_TDA.m_numericvalue2		= m_MapID;
	
	// ���� �ʿ��� �̵��� �� 
	if( g_CurrentZoneInfoIndex == m_TDA.m_numericvalue2)
	{	
		//�������� �ƴҶ��� ���������� �̵���Ų��.
		if( m_TDA.m_actiondata1 != _XDEF_AREACATEGORY_MAPWARP )
		{
			D3DXVECTOR3 v0,v1,		//������ ��Ÿ���� v0,v1,v2,v3�� 4���� 
				vNormal,			//������ ���ƺк��� ���� NormalVector�� vNormal
				vPoint;				//����� ��ġ�� ��Ÿ���� vPoint
			
			D3DXVECTOR3 vTemp;		//���� �Ѻ��� �������� �ٶ󺸴� ����� ��ġ 
				
			
			_XFunctionalObject* pFunctionalObject = NULL;
			if( g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT )
			{
				pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_ViewTarget;
			}
			
			_XArea* pArea = g_AreaManager.GetArea( m_TDA.m_actiondata1, m_TDA.m_numericvalue1 );

			if( pFunctionalObject && pArea )
			{
				
				v0 = D3DXVECTOR3(pFunctionalObject->m_Position.x,0,pFunctionalObject->m_Position.z);
				v1 = D3DXVECTOR3( ((pArea->m_Point[0].x+pArea->m_Point[2].x)/2) ,0, ((pArea->m_Point[0].z+pArea->m_Point[2].z)/2) );
				
				vPoint = g_pLocalUser->m_Position;
				vPoint.y = 0.0f;

				vNormal = v0 - v1;
				vTemp = v0 - vPoint;

				FLOAT dx = ( v0.x - v1.x);
				FLOAT dz = ( v0.z - v1.z);
				FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );

				if( _distance < 32.0f )
				{
					if(D3DXVec3Dot(&vNormal,&vTemp)>0) // �� �߽ɰ� �̵��ҷ��� ��ġ ���̿� �ִ�. �̵����� �ʰ� �н� 
					{
						return FUNCTIONALOBJECT_ACTION_RESULT_FAIL;
					}
				}

				/*if(D3DXVec3Dot(&vNormal,&vTemp)>0) // �� �߽ɰ� �̵��ҷ��� ��ġ ���̿� �ִ�. �̵����� �ʰ� �н� 
				{
					return FUNCTIONALOBJECT_ACTION_RESULT_FAIL;
				}
				else*/ // �� �߽ɰ� �̵��ҷ��� ��ġ ���̿� ����. �̵��Ѵ�.
				{
					g_DrawFadeInOutFlag = TRUE;
					
					for( g_FadeInOutLevel = 0;  g_FadeInOutLevel <= 255;  g_FadeInOutLevel += 24 )
					{			
						((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.Draw();
					}
					
					if(g_FadeInOutLevel >= 255)
					{
						g_FadeInOutLevel = 255;			
					}
					g_DrawFadeInOutFlag = FALSE;
					
					if( pArea->m_ShapeType == _XDEF_AREASHAPETYPE_BOX )
					{				
						g_pLocalUser->m_Position.x = ( pArea->m_Point[0].x + pArea->m_Point[2].x )/2;
						g_pLocalUser->m_Position.z = ( pArea->m_Point[0].z + pArea->m_Point[2].z )/2;
					}
					else
					{
						g_pLocalUser->m_Position.x = pArea->m_CenterPoint.x;
						g_pLocalUser->m_Position.z = pArea->m_CenterPoint.z;
					}
					
					_XOBBData*  pUnderObb = NULL;
					g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, pUnderObb );
					g_pLocalUser->m_Velocity.y = 0.0f;
					
					g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pFunctionalObject->m_Position, g_pLocalUser->m_Position);

					g_pLocalUser->SetAnimation(_XACTION_IDLE);
					g_pLocalUser->UpdatePosition();
					
					g_LodTerrain.m_3PCamera.SetTargetPosition( 
						D3DXVECTOR3(g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, g_pLocalUser->m_Position.z ) );
					
					// reset camera rotate & zoom velocity...
					g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
					g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	
					
					g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
					g_LodTerrain.RebuildLevel(FALSE);
					
					_XWindow_WorldMinimap* pMinimap_Window = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIMAPWINDOW);
					if(pMinimap_Window)
					{
						float rotateangle = _XMeshMath_GetRotateRadian(g_pLocalUser->m_Position, g_pLocalUser->m_TargetPosition);
						pMinimap_Window->SetRotateArrow(rotateangle + _X_PI);
						pMinimap_Window->SetLocalUserPosition();
					}
					
					g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
					g_pLocalUser->m_pGoundCollideOBB = pUnderObb;
					g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
					
					g_DrawFadeInOutFlag = TRUE;
					for( g_FadeInOutLevel = 255;  g_FadeInOutLevel >= 0;  g_FadeInOutLevel -= 24 )
					{		
						((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.Draw();
					}
					
					if(g_FadeInOutLevel <= 0)
					{
						g_FadeInOutLevel = 0;	
					}
					g_DrawFadeInOutFlag = FALSE;
					
					return FUNCTIONALOBJECT_ACTION_RESULT_SUCCESS;
				}
			}
			else
			{
				return FUNCTIONALOBJECT_ACTION_RESULT_FAIL;
			}
		}
		else
		{	
			// �̵��� ���� ������ �������̸� ������ ��Ų��.
			// �� �ʿ� �ݵ�� �������� �����־�� �ϸ� ������ ������ �־�� �Ѵ�.
			_XArea* pCurrentFocusedArea = g_AreaManager.GetArea( _XDEF_AREACATEGORY_MAPWARP, m_TDA.m_numericvalue1 );

			if( pCurrentFocusedArea )
			{
				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_PrevCheckedWarpingZoneTimer = g_LocalSystemTime;
				// ���� ���ִ� ���� ��Ż �ε��� ���� ���ۿ�
				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_CurrentWarpZoneIndex = m_TDA.m_numericvalue1;	 
				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_PrevZoneInfoIndex	   = g_CurrentZoneInfoIndex;


				g_CurrentZoneInfoIndex = pCurrentFocusedArea->m_TargetZone+1;    // �������� �������ε����� �����Ѵ�. �����ϰ�� �ǵ���.
				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_NextWarpZoneIndex	   = pCurrentFocusedArea->m_TargetWarpzone;	 // ���� ���� ������ ���� �ε���

				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_WarpStartTime = g_LocalSystemTime; 

				TCHAR tstr[256];
				memset(tstr, 0, sizeof(tstr));
				_snprintf( tstr, sizeof(tstr), _XGETINTERFACETEXT(ID_STRING_MAIGAME_MOVING), g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonename );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, " ", tstr, TRUE );

								
				g_NetworkKernel.SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 4 );// �����̵� ���̶�� ���¸� �˷��ش�.
				
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_MG_WARPINGNEXTZONE, 0), 0 );

				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LockMoveProcess = TRUE;

				return FUNCTIONALOBJECT_ACTION_RESULT_SUCCESS;
			}
			else
			{
				return FUNCTIONALOBJECT_ACTION_RESULT_FAIL;
			}
		}		
	}

	return FUNCTIONALOBJECT_ACTION_RESULT_FAIL;	
}

_XFUNCTIONALOBJECT_ACTION_RESULT _XFunctionalObjectActionItem::ProcessEnterBossRoom(void)
{
	//	m_TDA.m_numericvalue1		= m_MapID;
	
	if( g_pLocalUser->m_InParty )
	{
		if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
		{
			//�����常 ������ �� �ִ�.
			g_ChannelTempZoneInfoIndex = m_TDA.m_numericvalue1;
			g_NetworkKernel.SendPacket( MSG_NO_EX_BATTLE_TYPE_INFO_REQ_C_S, m_TDA.m_numericvalue1 );//�������� ������ �䱸�Ѵ�.
		}
		else
		{
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAME_NECESSITYPASS)); // ����� ���ؼ� �� �� �ֽ��ϴ�.
			
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE);
			
			_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );
			defaultsystemmsgwindow->SetDockingMode(FALSE);
			defaultsystemmsgwindow->m_btnFunctionalObjectViewMode->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORKBOSS_1622);
		}
	}
	else
	{
		g_ChannelTempZoneInfoIndex = m_TDA.m_numericvalue1;
		g_NetworkKernel.SendPacket( MSG_NO_EX_BATTLE_TYPE_INFO_REQ_C_S, m_TDA.m_numericvalue1 );//�������� ������ �䱸�Ѵ�.					
	}
						
	return FUNCTIONALOBJECT_ACTION_RESULT_SUCCESS;
}


_XFUNCTIONALOBJECT_ACTION_RESULT _XFunctionalObjectActionItem::ProcessEnableAttack(void)
{
	_XFunctionalObject* pFunctionalObject = NULL;
	if( g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT )
	{
		pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_ViewTarget;
		if( pFunctionalObject )
		{
			pFunctionalObject->m_bAttackable = TRUE;

			if( pFunctionalObject->m_ObjectType >= 0 ) // -1�� ��찡 ����.
				g_Moblist.AddReferenceModel( pFunctionalObject->m_ObjectType );
		}
	}
	return FUNCTIONALOBJECT_ACTION_RESULT_SUCCESS;
	
}
_XFUNCTIONALOBJECT_ACTION_RESULT _XFunctionalObjectActionItem::ProcessDisplayText(void)
{
	return FUNCTIONALOBJECT_ACTION_RESULT_SUCCESS;
	
}

_XFUNCTIONALOBJECT_ACTION_RESULT _XFunctionalObjectActionItem::ProcessMatchInfo(void)
{
	//scriptdata->m_TDA.m_numericvalue1		= m_AreaID;
	_XWindow_MatchNoticeLarge* pMatchNoticeLarge_Window = (_XWindow_MatchNoticeLarge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICELARGE);
	if(pMatchNoticeLarge_Window)
	{
		pMatchNoticeLarge_Window->SetSelectedAreaID(m_TDA.m_numericvalue1);// AreaID ������ �˷��ش�.
		pMatchNoticeLarge_Window->ShowWindow(TRUE);
	}
	
	return FUNCTIONALOBJECT_ACTION_RESULT_SUCCESS;	
}

_XFUNCTIONALOBJECT_ACTION_RESULT _XFunctionalObjectActionItem::ProcessMatchZoneEnterExit(void)
{
	// ���� �񹫿� - �ٸ� �뵵�� ������ �� ��
	
	//scriptdata->m_TDA.m_actiondata1		= m_Target; 0:���� 1:����
	switch(m_TDA.m_actiondata1) {
	case 0:
		{
		}
		break;
	case 1:
		{
		}
		break;
	}
	
	return FUNCTIONALOBJECT_ACTION_RESULT_SUCCESS;	
}
_XFUNCTIONALOBJECT_ACTION_RESULT _XFunctionalObjectActionItem::ProcessCondtionalAttactSetting(void)
{
	// �÷��̾�_���Ǻ����ݼ���
	
	//scriptdata->m_TDA.m_actiondata1		= m_indexTarget; 0:����� �����Ҷ� 
	switch(m_TDA.m_actiondata1) {
	case 0://����� �����Ҷ� 
		{
			if(g_CBManager.m_enWarState >= _XEN_CB_PROGRESS && g_CBManager.m_enWarState < _XEN_CB_END)
			{
				_XFunctionalObject* pFunctionalObject = NULL;
				if( g_pLocalUser->m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT )
				{
					pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_ViewTarget;
					if( pFunctionalObject )
					{
						pFunctionalObject->m_bAttackable = TRUE;
						
						if( pFunctionalObject->m_ObjectType >= 0 ) // -1�� ��찡 ����.
							g_Moblist.AddReferenceModel( pFunctionalObject->m_ObjectType );
					}
				}
				return FUNCTIONALOBJECT_ACTION_RESULT_SUCCESS;
			}
		}
		break;
	}
	
	return FUNCTIONALOBJECT_ACTION_RESULT_FAIL;	
}

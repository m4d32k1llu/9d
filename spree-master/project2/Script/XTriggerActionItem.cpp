/** �ൿ���� ǥ���ϴ� Ŭ����.
 *
 * \author Sohyun, Park
 * \date 2003.09.02
 */

#include "stdafx.h"

#pragma warning (disable: 4786)

#include <stdio.h>
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XTriggerActionItem.h"
#include "XWindow_NPCScript.h"
#include "XWindow_MaximumMap.h"
#include "Xwindow_TrainingMed.h"
#include "Xwindow_QuestMail.h"

#include "XWindowObjectDefine.h"

#include "XSR_STRINGHEADER.h"

extern  D3DCOLOR g_ChattingStringColor[16];

extern void _XInsertCenterSystemMessage( LPTSTR message , D3DCOLOR messageinfo );

_XTriggerActionItem::_XTriggerActionItem()
{
	
	m_bMainQuest		= FALSE;
	m_QuestType			= _XQUEST_TYPE_NONE;
	m_lpszMessageString = NULL;
	ResetData();
}

_XTriggerActionItem::~_XTriggerActionItem()
{
#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
#else
	if(m_lpszMessageString)
	{
		delete[] m_lpszMessageString;
		//m_lpszMessageString = NULL;
	}
#endif
}

void _XTriggerActionItem::ResetData(void)
{
	m_TDA.m_enable		= ID_TRIGGER_DISABLE;

	m_TDA.m_actionclass	= ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_actiondata1 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_actiondata2 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_actiondata3 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_actiondata4 = ID_TRIGGER_SETDATA_NONE;

	m_TDA.m_numericvalue1 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_numericvalue2 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_numericvalue3 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_numericvalue4 = ID_TRIGGER_SETDATA_NONE;
	m_TDA.m_numericvalue5 = ID_TRIGGER_SETDATA_NONE;

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
	m_lpszMessageString = NULL;
#else
	if(m_lpszMessageString)
	{
		delete[] m_lpszMessageString;
		m_lpszMessageString = NULL;
	}
#endif


	memset(m_szActionString, 0, sizeof(TCHAR)*64);

	m_nOwnerQuestID		= -1;
	m_nOwnerTriggerID	= -1;

	m_bSuccessClient	= FALSE;
	m_bSuccessServer	= FALSE;
	m_bConfirmAll		= FALSE;
}

void _XTriggerActionItem::SetMessageString(LPCTSTR messagestring)
{

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
	m_lpszMessageString = (LPTSTR)messagestring;
#else
	if(!messagestring)
		return;

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
#endif
}

void _XTriggerActionItem::SetActionString(LPCTSTR actionstring)
{
	memset(m_szActionString, 0, sizeof(TCHAR)*64);

	if(!actionstring)
		return;

	int strlength = strlen(actionstring);
	if(strlength < 64)
	{
		strcpy(m_szActionString, actionstring);
	}
	else
	{
		strncpy(m_szActionString, actionstring, 63);
	}
}

BOOL _XTriggerActionItem::Load(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;

	int actionid;

	if(fread(&actionid, sizeof(int), 1, fileptr) < 1)
		return FALSE;

	if(fread(m_szActionString, sizeof(TCHAR)*64, 1, fileptr) < 1)
		return FALSE;

	if(fread(&m_TDA, sizeof(_XTRIGGERDATA_ACTION), 1, fileptr) < 1)
		return FALSE;

	int readstringlength = 0;
	if( (readstringlength = LoadString(fileptr, m_lpszMessageString)) == -1 )
		return FALSE;

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM	
	if( readstringlength > 0 )
	{
		m_lpszMessageString = g_StringDataBase.GetTextString( _XSTRINGDB_SN_QUESTSCRIPTTEXT, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex++] );	
		//_XLog( "QSI\tQuest action message string\t...\t%d\t%d\t\t%s", actionid, g_LoadQuestScriptText_IndexTable[g_LoadQuestScriptText_LastIndex-1], m_lpszMessageString );
	}
#endif


	return TRUE;
}

int _XTriggerActionItem::LoadString(FILE* fileptr, LPTSTR& string)
{
	int strlength = 0;

	if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		return -1;

#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
	fseek( fileptr, sizeof(TCHAR)*strlength, SEEK_CUR );
	string = NULL;
#else	
	if(string)
	{
		delete[] string;
		string = NULL;
	}    

	if(strlength > 0)
	{
		string = new TCHAR[strlength + 1];
		if(!string)
			return -1;

		memset(string, 0, sizeof(TCHAR)*(strlength + 1));

		if(fread(string, strlength, 1, fileptr) < 1)
			return -1;
	}
#endif

	return strlength;
}

void _XTriggerActionItem::SetActionID(int actionid)
{
	m_nActionID = actionid;
}

int _XTriggerActionItem::GetActionID(void)
{
	return m_nActionID;
}

void _XTriggerActionItem::SetOwnerQuestID(int questid)
{
	m_nOwnerQuestID = questid;
}

int _XTriggerActionItem::GetOwnerQuestID(void)
{
	return m_nOwnerQuestID;
}

void _XTriggerActionItem::SetOwnerTriggerID(int triggerid)
{
	m_nOwnerTriggerID = triggerid;
}

int _XTriggerActionItem::GetOwnerTriggerID(void)
{
	return m_nOwnerTriggerID;
}

void _XTriggerActionItem::Reset(void)
{
	m_mapCameraValue.clear();
}

void _XTriggerActionItem::SaveCameraInfo(void)
{
	g_QuestScriptManager.m_OriginalCameraInfo.vecCurrentPosition = g_LodTerrain.m_3PCamera.m_CameraPosition;
	g_QuestScriptManager.m_OriginalCameraInfo.vecTargetPosition = g_LodTerrain.m_3PCamera.m_TargetPosition;
	g_QuestScriptManager.m_OriginalCameraInfo.fTargetDistance = g_LodTerrain.m_3PCamera.m_TargetDistance;
	g_QuestScriptManager.m_OriginalCameraInfo.fFov = g_LodTerrain.m_3PCamera.mp_fFov;
	g_QuestScriptManager.m_OriginalCameraInfo.fPitch = g_LodTerrain.m_3PCamera.mp_fPitch;
	g_QuestScriptManager.m_OriginalCameraInfo.fYaw = g_LodTerrain.m_3PCamera.mp_fYaw;
}

void _XTriggerActionItem::RestoreCameraInfo(void)
{
	g_LodTerrain.m_3PCamera.SetCameraPosition(g_QuestScriptManager.m_OriginalCameraInfo.vecCurrentPosition);
	g_LodTerrain.m_3PCamera.SetTargetPosition(g_QuestScriptManager.m_OriginalCameraInfo.vecTargetPosition);
	g_LodTerrain.m_3PCamera.SetDistance(g_QuestScriptManager.m_OriginalCameraInfo.fTargetDistance);
	g_LodTerrain.m_3PCamera.mp_fFov = g_QuestScriptManager.m_OriginalCameraInfo.fFov;
	g_LodTerrain.m_3PCamera.mp_fPitch = g_QuestScriptManager.m_OriginalCameraInfo.fPitch;
	g_LodTerrain.m_3PCamera.mp_fYaw = g_QuestScriptManager.m_OriginalCameraInfo.fYaw;

	g_QuestScriptManager.ClearOriginalCameraInfo();
}

/*
void _XTriggerActionItem::SetOwnerTriggerClass(BYTE triggerclass)
{
	m_byOwnerTriggerClass = triggerclass;
}

BYTE _XTriggerActionItem::GetOwnerTriggerClass(void)
{
	return m_byOwnerTriggerClass;
}
*/

BOOL _XTriggerActionItem::NeedClientProcess(void)
{
	switch(m_TDA.m_actionclass)
	{
	case ID_ACTION_ADDSKILL :							// �÷��̾�_��������
	case ID_ACTION_DELETESKILL :						// �÷��̾�_��������
	case ID_ACTION_GIVEITEM :							// �÷��̾�_�������ֱ�
	case ID_ACTION_TAKEITEM :							// �÷��̾�_�����ۻ���
	case ID_ACTION_GIVEEXPERIENCE :						// �÷��̾�_����ġ �ֱ� �� �Ǹ�
	case ID_ACTION_MOVE :								// �÷��̾�_�̵�
	case ID_ACTION_SETSTATUS :							// �÷��̾�_���ȼ���
	case ID_ACTION_JOINGROUP :							// ���İ���
	case ID_ACTION_GOTOTRIGGER :						// �ش� Ʈ���ŷ� �̵�
	case ID_ACTION_DEFAULT :							// �ƹ��ൿ�� ���ϴ� Ʈ����
	case ID_ACTION_SETWEARITEM :						// ������ ���� ����
//	case ID_ACTION_SETMOBSPAWN :						// ���� �ҷ����� 
	case ID_ACTION_SETOBJECTSPAWN :						// ������Ʈ �ҷ�����
	case ID_ACTION_GIVENICKNAME :						// ��ȣ�ֱ�
	case ID_ACTION_SETSKILLEXP :						// �÷��̾�_�������õ�(�ֱ�,���ѱ�)
	case ID_ACTION_SETSKILLSTATE :						// �÷��̾�_��������(�������,��밡��)
	case ID_ACTION_SETABNORMALSTATE :					// �÷��̾�_�����̻���(�ֱ�, ����)
	case ID_ACTION_GIVESETTINGITEM:						// �̸� �����Ǿ��ִ� ������ �ֱ�
	case ID_ACTION_RESETVALUE:							// ���� �ʱ�ȭ
	case ID_ACTION_SETPLAYERCLASS:						// �÷��̾�_Ŭ��������(�ֱ�,���ѱ�)
	case ID_ACTION_SETCLANSERVICEPOINT:					// �÷��̾�_���ı⿩��(�ֱ�,���ѱ�)
	case ID_ACTION_SETCONDITIONEFFECT:					// �÷��̾�_����ȿ���ο�		
	case ID_ACTION_PARTYGIVEITEM:						// ��Ƽ��_�������ֱ�or����(���,���,MVP)
	case ID_ACTION_PARTYGIVEEXPERIENCE:					// ��Ƽ��_�����ֱ�or����(���,���,MVP)   
	case ID_ACTION_PARTYSETABNORMALSTATE:				// ��Ƽ��_�����̻��ֱ�		
	case ID_ACTION_PARTYGIVEEQUALLYREWARD:				// ��Ƽ��_�յ��Ѻ����ֱ�(����/��Ƽ����)
	case ID_ACTION_INSERTRANK:							// ���������ϱ�		
	case ID_ACTION_CHANGESERVERGROUP:					// ���������º���(��������)
	case ID_ACTION_GIVEEXPERIENCEBYPERCENTAGE:			// �÷��̾�_n%����ġ�ֱ�
	case ID_ACTION_GIVECLASSITEM:						// ���Һ� �������ֱ�(����,����)
	case ID_ACTION_PARTYGIVENICKNAME:					// ��Ƽ��_��ȣ�ֱ�
	case ID_ACTION_PARTYTAKEITEM:						// ��Ƽ��_�����ۻ���(��Ƽ��ü)
	case ID_ACTION_GIVEEXPERIENCEBYLEVEL:				// �÷��̾�_����%����ġ�ֱ�
	case ID_ACTION_SETWIDECONDITIONEFFECT:				// �÷��̾�_��������ȿ���ο�
	case ID_ACTION_SETCLASSGRADE :						// ��å ��� ����
	case ID_ACTION_MND_GIVEMASTER_RANK :				// �����ý��� ���µ�޼���
	case ID_ACTION_MND_SETMASTERPOINT :					// �����ý��� ���±⿩��(�ֱ�,����)
		return FALSE;		
		
	case ID_ACTION_SOUNDSTART :
	case ID_ACTION_MUSICSTART :
	case ID_ACTION_SETENV :
	case ID_ACTION_EFFECTSTART :
	case ID_ACTION_TIMERSTART :
	case ID_ACTION_SENDMESSAGE :
	case ID_ACTION_AUTOSTART :
	case ID_ACTION_RESULTQUEST :
	case ID_ACTION_MOVECAMERA :
	case ID_ACTION_ROTATECAMERA :
	case ID_ACTION_CAMERASETTING :
	case ID_ACTION_SHAKECAMERA :
	case ID_ACTION_SCREENEFFECT :
	case ID_ACTION_SETWEATHER :
	case ID_ACTION_BLOCKTRIGGER :
	case ID_ACTION_QUESTPROGRESSINFO :
	case ID_ACTION_SELECTION :
	case ID_ACTION_STARTMINIGAME :
	case ID_ACTION_TALKINGTOONESELF :
	case ID_ACTION_SETMOVEVELOCITY :
	case ID_ACTION_STARTOPTIONCONDITION :
	case ID_ACTION_ENDOPTIONCONDITION :
	case ID_ACTION_GIVECOUNTPOINT :				// ī���� ����Ʈ �ֱ� 
	case ID_ACTION_SETMOBSPAWN :				// ���� �ҷ����� 
	case ID_ACTION_SETMOTION :					// ������ �ൿ ��Ű��
	case ID_ACTION_SETMOVIE :					// ���� �����ֱ�
	case ID_ACTION_SETMAP :						// ���� ON/OFF	
	case ID_ACTION_MONSTERTALKINGTOONESELF :	// ���� ȥ�ڸ��ϱ� 
	case ID_ACTION_SETITMECOUNTERSTART :		// ������ ī���� �����ϱ�	
	case ID_ACTION_STOPCOUNTER :				// ī���� ���߱� 
	case ID_ACTION_SETSYSTEMMESSAGE :			// �ý��۸޼������ 
	case ID_ACTION_SETNPCONOFF :				// NPC ���� & ���� 
	case ID_ACTION_SENDQUESTEMAIL :				// ǳ��� ������ 
	case ID_ACTION_HOLDITEM:					// ���������������Ұ�����
	case ID_ACTION_PARTYMESSAGE:				// ��Ƽ��_�޼������
	case ID_ACTION_DISPLAYQUESTRANKING:			// Ư������Ʈ��ŷ�����ֱ�
	case ID_ACTION_STARTPARTYITMECOUNTER:		// ��Ƽ������ī���ͽ����ϱ�
	case ID_ACTION_SETVIRTUALMODE:				// ������������ϱ�
		return TRUE;				
	}
	return FALSE;
}

_XQUESTACTION_RESULT _XTriggerActionItem::Process(void)
{
	if(NeedClientProcess())
	{
		switch(m_TDA.m_actionclass)
		{
		case ID_ACTION_SOUNDSTART :
			return ProcessSoundSetting();
			
		case ID_ACTION_MUSICSTART :
			return ProcessMusicSetting();

		case ID_ACTION_RESULTQUEST :
			return ProcessResultQuest();
			
		case ID_ACTION_MOVECAMERA :
			return ProcessMoveCameraUsePath();
			
		case ID_ACTION_SHAKECAMERA :
			return ProcessShakeCamera();
			
		case ID_ACTION_ROTATECAMERA :
			return ProcessRotateCamera();
			
		case ID_ACTION_EFFECTSTART :
			return ProcessEffectSetting();
			
		case ID_ACTION_TIMERSTART :
			return ProcessTimerSetting();
			
		case ID_ACTION_SENDMESSAGE :
			return ProcessSendMessage();
			
		case ID_ACTION_CAMERASETTING :
			return ProcessCameraSetting();
			
		case ID_ACTION_SCREENEFFECT :
			return ProcessScreenEffect();
			
		case ID_ACTION_SETENV :
			return ProcessEnvStart();
			
		case ID_ACTION_SETWEATHER :
			return ProcessWeatherStart();

		case ID_ACTION_AUTOSTART :
			return ProcessAutoStart();

		case ID_ACTION_BLOCKTRIGGER :
			return ProcessBlockTrigger();
			
		case ID_ACTION_QUESTPROGRESSINFO :
			return ProcessQuestProgressInfo();

		case ID_ACTION_SELECTION :			
			return ProcessSelection();
			
		case ID_ACTION_STARTMINIGAME :			
			return ProcessStartMiniGame();
			
		case ID_ACTION_TALKINGTOONESELF :			
			return ProcessTalkingToOneself();

		case ID_ACTION_SETMOVEVELOCITY :		
			return ProcessMoveVelocity();			

		case ID_ACTION_STARTOPTIONCONDITION :		
			return ProcessStartOptionCondition();
			
		case ID_ACTION_ENDOPTIONCONDITION :		
			return ProcessEndOptionCondition();
			
		case ID_ACTION_GIVECOUNTPOINT :		
			return ProcessGiveCountPoint();			
			
		case ID_ACTION_SETMOBSPAWN :		
			return ProcessSpawnMonster();			
			
		case ID_ACTION_SETMOTION :		
			return ProcessSetMotion();
			
		case ID_ACTION_SETMOVIE :		
			return ProcessSetMovie();
						
		case ID_ACTION_SETMAP :// ���� ON/OFF	
			return ProcessSetMap();
	
		case ID_ACTION_MONSTERTALKINGTOONESELF :// ���� ȥ�ڸ��ϱ�
			return ProcessMobtalkingToOneself();
			
		case ID_ACTION_SETITMECOUNTERSTART :// ������ ī���� �����ϱ�	
			return ProcessStartItemCounter();

		case ID_ACTION_STOPCOUNTER :// ī���� ���߱� 
			return ProcessStopCounter();
			
		case ID_ACTION_SETSYSTEMMESSAGE :// �ý��۸޼������ 
			return ProcessSetSystemMessage();
			
		case ID_ACTION_SETNPCONOFF :// NPC ���� & ���� 
			return ProcessSetNPCOnOff();

		case ID_ACTION_SENDQUESTEMAIL :// ǳ��� ������ 
			return ProcessSendQuestMail();

		case ID_ACTION_HOLDITEM :// ���������������Ұ����� 
			return ProcessHoldItem();
			
		case ID_ACTION_PARTYMESSAGE:				// ��Ƽ��_�޼������
			return ProcessPartyMessage();

		case ID_ACTION_DISPLAYQUESTRANKING:				// Ư������Ʈ��ŷ�����ֱ�
			return ProcessDisplayQuestRanking();

		case ID_ACTION_STARTPARTYITMECOUNTER:				// ��Ƽ������ī���ͽ����ϱ�
			return ProcessStartPartyItemCounter();
			
		case ID_ACTION_SETVIRTUALMODE:
			return ProcessSetVirtualMode();

		default :
			return QUEST_ACTION_RESULT_PROCESS;
		}
	}
	else
	{
		return QUEST_ACTION_RESULT_PROCESS;
	}
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSoundSetting(void)
{
	int soundfileindex = g_FMODWrapper.InsertSound(m_lpszMessageString, FSOUND_HW2D);
	_XPlayInterfaceSound(soundfileindex);
	
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessMusicSetting(void)
{
	switch(m_TDA.m_numericvalue2)
	{
	case ID_QUEST_SOUNDTARGET_START :
		return ProcessMusicStart();

	case ID_QUEST_SOUNDTARGET_STOP :
		return ProcessMusicStop();
	}
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessMusicStart(void)
{
	// m_actiondata1 : Music file index
	// m_actiondata2 : play type
	// m_numericvalue1 : volume
	// m_numericvalue2 : target class

	// music start
	if(!g_BgmPlayFlag)
		return QUEST_ACTION_RESULT_SUCCESS;

	switch(m_TDA.m_actiondata2)
	{
	case ID_QUEST_PLAYTYPE_LOOP :
		{
			g_BgmPlayOnce = FALSE;
		}
		break;
	case ID_QUEST_PLAYTYPE_ONCE :
		{
			g_BgmPlayOnce = TRUE;
		}
		break;

	default :
		return QUEST_ACTION_RESULT_SUCCESS;
	}

/*	if(g_BgmPlayFlag && g_BgmObject.Getm_dwDeviceID())
		g_BgmObject.Close();

	g_BgmObject.OpenDevice(gHWnd, g_BgmList[m_TDA.m_actiondata1]);
	g_BgmObject.Play(gHWnd, 1);

	long volume = g_MixerVolume.Getm_lmaxVol() - g_MixerVolume.Getm_lminVol();
	long value = long(volume * (m_TDA.m_numericvalue1/100.));

	g_CurrentMixerVolume = value;
	g_MixerVolume.SetFaderValue(value);*/

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessMusicStop(void)
{
	// m_actiondata1 : Music file index

/*	// music stop
	if(!g_BgmPlayFlag)
		return QUEST_ACTION_RESULT_SUCCESS;
	if(g_BgmPlayFlag && g_BgmObject.Getm_dwDeviceID())
		g_BgmObject.Close();*/

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessEnvStart(void)
{
	// env setting
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessMoveCameraUsePath(void)
{
	// move camera
	D3DXVECTOR3 baseaxis = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 currentaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	FLOAT		rad_theta = 0.0f, deg_theta = 0.0f;

	map <int, _XCAMERA_INFO*>::iterator iter_camera = m_mapCameraInfo.find(m_nActionID);
	if(iter_camera == m_mapCameraInfo.end())
	{
		// ó�� ����
		_XCAMERA_INFO* currentcamerainfo = new _XCAMERA_INFO;
		memset(currentcamerainfo, 0, sizeof(_XCAMERA_INFO));

		currentcamerainfo->bStart = TRUE;
		currentcamerainfo->dwTotalTime = (DWORD)(m_TDA.m_numericvalue1*60*1000 + m_TDA.m_numericvalue2*1000);		// ms
		g_LodTerrain.m_PathObjectManager->GeneratePathIterate(m_TDA.m_actiondata2, _XDEF_MAXNUM_PATHPOINT);
		currentcamerainfo->fVelocity = 1000. / currentcamerainfo->dwTotalTime;
		currentcamerainfo->dwStartTime = g_LocalSystemTime;
		currentcamerainfo->dwElapsedTime = currentcamerainfo->dwStartTime;
		currentcamerainfo->nPointPosition = 0;
		m_mapCameraInfo[m_nActionID] = currentcamerainfo;

		if(!g_QuestScriptManager.GetQuestCameraMode())
		{
			g_QuestScriptManager.SetQuestCameraMode(TRUE);
			SaveCameraInfo();
		}

		g_LodTerrain.m_3PCamera.SetCameraPosition(g_LodTerrain.m_PathObjectManager->m_vPathPoint[currentcamerainfo->nPointPosition]);
		g_LodTerrain.m_3PCamera.SetTargetPosition(g_LodTerrain.m_PathObjectManager->m_vPathPoint[currentcamerainfo->nPointPosition+1]);
		currentaxis = g_LodTerrain.m_3PCamera.m_TargetPosition - g_LodTerrain.m_3PCamera.m_CameraPosition;
		currentaxis.y = 0.0f;
		rad_theta = _XGetAngleFromVectors(&baseaxis, &currentaxis);
		deg_theta = _X_DEG(rad_theta);
		if(currentaxis.x > 0)
		{
			deg_theta = 180 - deg_theta;
			deg_theta += 180;
		}
		g_LodTerrain.m_3PCamera.mp_fYaw = deg_theta;
		g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;

		g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
		g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
		g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
		g_LodTerrain.RebuildLevel(FALSE);

		return QUEST_ACTION_RESULT_PROCESS;
	}
	else
	{
		_XCAMERA_INFO* currentcamerainfo = iter_camera->second;
		if(!currentcamerainfo)
		{
			m_mapCameraInfo.erase(iter_camera);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}

		if((currentcamerainfo->dwElapsedTime - currentcamerainfo->dwStartTime) < currentcamerainfo->dwTotalTime)
		{
			// ������
			DWORD elapsedtime = g_LocalSystemTime - currentcamerainfo->dwElapsedTime;
			currentcamerainfo->nPointPosition += (int)(elapsedtime * currentcamerainfo->fVelocity);

			g_LodTerrain.m_3PCamera.SetCameraPosition(g_LodTerrain.m_PathObjectManager->m_vPathPoint[currentcamerainfo->nPointPosition]);
			g_LodTerrain.m_3PCamera.SetTargetPosition(g_LodTerrain.m_PathObjectManager->m_vPathPoint[currentcamerainfo->nPointPosition+1]);
			currentaxis = g_LodTerrain.m_3PCamera.m_TargetPosition - g_LodTerrain.m_3PCamera.m_CameraPosition;
			currentaxis.y = 0.0f;
			rad_theta = _XGetAngleFromVectors(&baseaxis, &currentaxis);
			deg_theta = _X_DEG(rad_theta);
			if(currentaxis.x > 0)
			{
				deg_theta = 180 - deg_theta;
				deg_theta += 180;
			}
			g_LodTerrain.m_3PCamera.mp_fYaw = deg_theta;
			g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
			
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);

			currentcamerainfo->dwElapsedTime += elapsedtime;
			
			return QUEST_ACTION_RESULT_PROCESS;
		}
		else
		{
			if(!currentcamerainfo->bEnd)
			{
				// ������ frame
				currentcamerainfo->bEnd = TRUE;
				currentcamerainfo->nPointPosition = _XDEF_MAXNUM_PATHPOINT - 1;

				g_LodTerrain.m_3PCamera.SetCameraPosition(g_LodTerrain.m_PathObjectManager->m_vPathPoint[currentcamerainfo->nPointPosition]);
				g_LodTerrain.m_3PCamera.SetTargetPosition(g_LodTerrain.m_PathObjectManager->m_vPathPoint[currentcamerainfo->nPointPosition+1]);
				currentaxis = g_LodTerrain.m_3PCamera.m_TargetPosition - g_LodTerrain.m_3PCamera.m_CameraPosition;
				currentaxis.y = 0.0f;
				rad_theta = _XGetAngleFromVectors(&baseaxis, &currentaxis);
				deg_theta = _X_DEG(rad_theta);
				if(currentaxis.x > 0)
				{
					deg_theta = 180 - deg_theta;
					deg_theta += 180;
				}
				g_LodTerrain.m_3PCamera.mp_fYaw = deg_theta;
				g_LodTerrain.m_3PCamera.mp_fPitch = 0.0f;
				
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
				
				return QUEST_ACTION_RESULT_PROCESS;
			}
			else
			{
				//  �� ������. ������ �ǵ�����
				delete currentcamerainfo;
				m_mapCameraInfo.erase(iter_camera);
				if(m_TDA.m_numericvalue4 == ID_QUEST_CAMERA_RESTORE)
				{
					if(g_QuestScriptManager.GetQuestCameraMode())
						g_QuestScriptManager.SetQuestCameraMode(FALSE);

					RestoreCameraInfo();
					g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
					g_LodTerrain.RebuildLevel(FALSE);
				}

				return QUEST_ACTION_RESULT_SUCCESS;
			}
		}
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessShakeCamera(void)
{
	// m_actiondata1 : Intensity (��鸲�� ����)
	// m_actiondata2 : Direction (��鸲�� ����)
	// m_numericvalue1 : Minutes
	// m_numericvalue2 : Seconds
	// m_numericvalue3 : Setting (������ ����)

	// shake camera
	map <int, _XCAMERA_INFO*>::iterator iter_camerainfo = m_mapCameraInfo.find(m_nActionID);
	if(iter_camerainfo == m_mapCameraInfo.end())
	{
		// ó�� ����
		_XCAMERA_INFO* currentcamerainfo = new _XCAMERA_INFO;
		currentcamerainfo->bStart = TRUE;

		if(!g_QuestScriptManager.GetQuestCameraMode())
		{
			g_QuestScriptManager.SetQuestCameraMode(TRUE);
			SaveCameraInfo();
		}

		currentcamerainfo->dwTotalTime = (DWORD)(m_TDA.m_numericvalue1*60*1000 + m_TDA.m_numericvalue2*1000);
		FLOAT velocity;
		switch(m_TDA.m_actiondata1)
		{
		case ID_QUEST_CAMERAINTENSITY_FIRST :
			{
				velocity = ID_QUEST_SHAKECAMERA_UNITHEIGHT / 5;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_SECOND :
			{
				velocity = ID_QUEST_SHAKECAMERA_UNITHEIGHT / 4;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_THIRD :
			{
				velocity = ID_QUEST_SHAKECAMERA_UNITHEIGHT / 3;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_FORTH :
			{
				velocity = ID_QUEST_SHAKECAMERA_UNITHEIGHT / 2;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_FIFTH :
			{
				velocity = ID_QUEST_SHAKECAMERA_UNITHEIGHT;
			}
			break;
		default :
			{
				velocity = ID_QUEST_SHAKECAMERA_UNITHEIGHT;
			}
			break;
		}
		currentcamerainfo->fVelocity = (velocity*gnFPS) / 1000;
		currentcamerainfo->dwStartTime = g_LocalSystemTime;
		currentcamerainfo->dwElapsedTime = currentcamerainfo->dwStartTime;
		currentcamerainfo->nShakeDirection = 0;

		m_mapCameraInfo[m_nActionID] = currentcamerainfo;
		return QUEST_ACTION_RESULT_PROCESS;
	}
	else
	{
		_XCAMERA_INFO* currentcamerainfo = iter_camerainfo->second;
		if(!currentcamerainfo)
		{
			m_mapCameraInfo.erase(iter_camerainfo);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}

		if((currentcamerainfo->dwElapsedTime - currentcamerainfo->dwStartTime) < currentcamerainfo->dwTotalTime)
		{
			// ������
			switch(m_TDA.m_actiondata2)
			{
			case ID_QUEST_CAMERASHAKE_TOPDOWN :
				return ProcessShakeCameraTopDown(currentcamerainfo);
				
			case ID_QUEST_CAMERASHAKE_LEFTRIGHT :
				return ProcessShakeCameraLeftRight(currentcamerainfo);

			default :
				{
					delete currentcamerainfo;
					m_mapCameraInfo.erase(iter_camerainfo);
					if(m_mapCameraInfo.empty())
					{
						g_QuestScriptManager.SetQuestCameraMode(FALSE);
						RestoreCameraInfo();
						g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
						g_LodTerrain.m_3PCamera.UpdateProjMatrix();
						g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
						g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
						g_LodTerrain.RebuildLevel(FALSE);
					}
					return QUEST_ACTION_RESULT_SUCCESS;
				}
			}
		}
		else
		{
			// ��
			currentcamerainfo->bEnd = TRUE;
			delete currentcamerainfo;
			m_mapCameraInfo.erase(iter_camerainfo);
			if(m_TDA.m_numericvalue3 == ID_QUEST_CAMERA_RESTORE || m_mapCameraInfo.empty())
			{
				if(g_QuestScriptManager.GetQuestCameraMode())
					g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessShakeCameraTopDown(_XCAMERA_INFO* camerainfo)
{
	if(!camerainfo)
	{
//		if(m_mapCameraInfo.empty())
		{
			g_QuestScriptManager.SetQuestCameraMode(FALSE);
			RestoreCameraInfo();
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
		}
		return QUEST_ACTION_RESULT_SUCCESS;
	}
	
	D3DXVECTOR3	currentcameraposition = g_LodTerrain.m_3PCamera.m_CameraPosition;
	D3DXVECTOR3	currenttargetposition = g_LodTerrain.m_3PCamera.m_TargetPosition;
	D3DXVECTOR3	originalcameraposition = g_QuestScriptManager.m_OriginalCameraInfo.vecCurrentPosition;
	
	DWORD elapsedtime = g_LocalSystemTime - camerainfo->dwElapsedTime;
	
	if(camerainfo->nShakeDirection == ID_QUEST_SHAKECAMERA_UP)
	{
		if(fabs(currentcameraposition.y - originalcameraposition.y) < ID_QUEST_SHAKECAMERA_MAXHEIGHT)
		{
			currentcameraposition.y = currentcameraposition.y + (camerainfo->fVelocity*elapsedtime);
			currenttargetposition.y = currenttargetposition.y + (camerainfo->fVelocity*elapsedtime);
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
		}
		else
		{
			currentcameraposition.y = currentcameraposition.y - (camerainfo->fVelocity*elapsedtime);
			currenttargetposition.y = currenttargetposition.y - (camerainfo->fVelocity*elapsedtime);
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
			camerainfo->nShakeDirection = ID_QUEST_SHAKECAMERA_DOWN;
		}
	}
	else if(camerainfo->nShakeDirection == ID_QUEST_SHAKECAMERA_DOWN)
	{
		if(fabs(currentcameraposition.y - originalcameraposition.y) < ID_QUEST_SHAKECAMERA_MAXHEIGHT)
		{
			currentcameraposition.y = currentcameraposition.y - (camerainfo->fVelocity*elapsedtime);
			currenttargetposition.y = currenttargetposition.y - (camerainfo->fVelocity*elapsedtime);
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
		}
		else
		{
			currentcameraposition.y = currentcameraposition.y + (camerainfo->fVelocity*elapsedtime);
			currenttargetposition.y = currenttargetposition.y + (camerainfo->fVelocity*elapsedtime);
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
			camerainfo->nShakeDirection = ID_QUEST_SHAKECAMERA_UP;
		}
	}
	camerainfo->dwElapsedTime += elapsedtime;
	
	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessShakeCameraLeftRight(_XCAMERA_INFO* camerainfo)
{
	if(!camerainfo)
	{
//		if(m_mapCameraInfo.empty())
		{
			g_QuestScriptManager.SetQuestCameraMode(FALSE);
			RestoreCameraInfo();
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
		}
		return QUEST_ACTION_RESULT_SUCCESS;
	}

	D3DXVECTOR3 currentcameraposition = g_LodTerrain.m_3PCamera.m_CameraPosition;
	D3DXVECTOR3	currenttargetposition = g_LodTerrain.m_3PCamera.m_TargetPosition;
	D3DXVECTOR3	originalcameraposition = g_QuestScriptManager.m_OriginalCameraInfo.vecCurrentPosition;

	D3DXVECTOR3 vDistance = currentcameraposition - originalcameraposition;
	FLOAT distance = D3DXVec3Length(&vDistance);
	DWORD elapsedtime = g_LocalSystemTime - camerainfo->dwElapsedTime;

	if(camerainfo->nShakeDirection == ID_QUEST_SHAKECAMERA_LEFT)
	{
		if(fabs(distance) < ID_QUEST_SHAKECAMERA_MAXHEIGHT)
		{
			currentcameraposition = currentcameraposition - (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			currenttargetposition = currenttargetposition - (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
		}
		else
		{
			currentcameraposition = currentcameraposition + (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			currenttargetposition = currenttargetposition + (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
			camerainfo->nShakeDirection = ID_QUEST_SHAKECAMERA_RIGHT;
		}
	}
	else if(camerainfo->nShakeDirection == ID_QUEST_SHAKECAMERA_RIGHT)
	{
		if(fabs(distance) < ID_QUEST_SHAKECAMERA_MAXHEIGHT)
		{
			currentcameraposition = currentcameraposition + (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			currenttargetposition = currenttargetposition + (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
		}
		else
		{
			currentcameraposition = currentcameraposition - (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			currenttargetposition = currenttargetposition - (camerainfo->fVelocity*elapsedtime)*g_LodTerrain.m_3PCamera.m_RightVector;
			g_LodTerrain.m_3PCamera.SetCameraPosition(currentcameraposition);
			g_LodTerrain.m_3PCamera.SetTargetPosition(currenttargetposition);
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel(FALSE);
			camerainfo->nShakeDirection = ID_QUEST_SHAKECAMERA_LEFT;
		}
	}
	camerainfo->dwElapsedTime += elapsedtime;

	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessRotateCamera(void)
{
	// m_actiondata1 : area category
	// m_actiondata2 : Direction (����)
	// m_actiondata3 : Distance (�Ÿ�)
	// m_actiondata4 : Height (����)
	// m_numericvalue1 : Minutes
	// m_numericvalue2 : Seconds
	// m_numericvalue3 : Intensity (ȸ���� ����)
	// m_numericvalue4 : Setting (���� �� ī�޶� ����/�ǵ�����)
	// m_numericvalue5 : Center (ȸ���� �߽�)

	// rotate camera
	map <int, _XCAMERA_INFO*>::iterator iter_camera = m_mapCameraInfo.find(m_nActionID);
	if(iter_camera == m_mapCameraInfo.end())
	{
		// ó�� ����
		_XCAMERA_INFO* currentcamerainfo = new _XCAMERA_INFO;
		currentcamerainfo->bStart = TRUE;

		if(!g_QuestScriptManager.GetQuestCameraMode())
		{
			g_QuestScriptManager.SetQuestCameraMode(TRUE);
			SaveCameraInfo();
		}

		currentcamerainfo->dwTotalTime = (DWORD)(60*m_TDA.m_numericvalue1*1000 + m_TDA.m_numericvalue2*1000);
		FLOAT velocity;
		switch(m_TDA.m_numericvalue3)
		{
		case ID_QUEST_CAMERAINTENSITY_FIRST :
			{
				velocity = ID_QUEST_ROTATECAMERA_ANGLE / 5;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_SECOND :
			{
				velocity = ID_QUEST_ROTATECAMERA_ANGLE / 4;	
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_THIRD :
			{
				velocity = ID_QUEST_ROTATECAMERA_ANGLE / 3;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_FORTH :
			{
				velocity = ID_QUEST_ROTATECAMERA_ANGLE / 2;
			}
			break;
		case ID_QUEST_CAMERAINTENSITY_FIFTH :
			{
				velocity = ID_QUEST_ROTATECAMERA_ANGLE;
			}
			break;

		default :
			{
				velocity = ID_QUEST_ROTATECAMERA_ANGLE;
			}
			break;
		}
		currentcamerainfo->fVelocity = (velocity*gnFPS) / 1000;

		switch(m_TDA.m_actiondata3)
		{
		case ID_QUEST_CAMERADISTANCE_HIGH :
			{
				currentcamerainfo->fDistance = 15.0f;
			}
			break;
		case ID_QUEST_CAMERADISTANCE_MIDDLE :
			{
				currentcamerainfo->fDistance = 10.0f;
			}
			break;
		case ID_QUEST_CAMERADISTANCE_LOW :
			{
				currentcamerainfo->fDistance = 5.0f;
			}
			break;

		default :
			{
				currentcamerainfo->fDistance = 10.0f;
			}
			break;
		}

		switch(m_TDA.m_actiondata4)
		{
		case ID_QUEST_CAMERAHEIGHT_HIGH :
			{
				currentcamerainfo->fHeight = _XDEF_CHARACTERAVERAGEHEIGHT * 2.5f;
			}
			break;
		case ID_QUEST_CAMERAHEIGHT_MIDDLE :
			{
				currentcamerainfo->fHeight = _XDEF_CHARACTERAVERAGEHEIGHT;
			}
			break;
		case ID_QUEST_CAMERAHEIGHT_LOW :
			{
				currentcamerainfo->fHeight = _XDEF_CHARACTERAVERAGEHEIGHT / 2;
			}
			break;

		default :
			{
				currentcamerainfo->fHeight = _XDEF_CHARACTERAVERAGEHEIGHT;
			}
			break;
		}

		if(!g_AreaManager.FindAreaCenterPosition(currentcamerainfo->vecCenterPosition ,m_TDA.m_numericvalue5, m_TDA.m_actiondata1))
		{
			delete currentcamerainfo;
			m_mapCameraInfo.erase(iter_camera);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}

		currentcamerainfo->dwStartTime = g_LocalSystemTime;
		currentcamerainfo->dwElapsedTime = currentcamerainfo->dwStartTime;
		m_mapCameraInfo[m_nActionID] = currentcamerainfo;
		return QUEST_ACTION_RESULT_PROCESS;
	}
	else
	{
		_XCAMERA_INFO* currentcamerainfo = iter_camera->second;
		if(!currentcamerainfo)
		{
			m_mapCameraInfo.erase(iter_camera);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}

		if((currentcamerainfo->dwElapsedTime - currentcamerainfo->dwStartTime) < currentcamerainfo->dwTotalTime)
		{
			// ȸ����
			if(m_TDA.m_actiondata2 == ID_QUEST_CAMERADIRECTION_LEFT)
			{
				return ProcessRotateCameraLeft(currentcamerainfo);
			}
			else
			{
				return ProcessRotateCameraRight(currentcamerainfo);
			}
		}
		else
		{
			// ��
			currentcamerainfo->bEnd = TRUE;
			delete currentcamerainfo;
			m_mapCameraInfo.erase(iter_camera);
			if(m_TDA.m_numericvalue4 == ID_QUEST_CAMERA_RESTORE)
			{
				if(g_QuestScriptManager.GetQuestCameraMode())
					g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}


_XQUESTACTION_RESULT _XTriggerActionItem::ProcessRotateCameraLeft(_XCAMERA_INFO* camerainfo)
{
	DWORD elapsedtime = g_LocalSystemTime - camerainfo->dwElapsedTime;
	camerainfo->fRotationAngle += camerainfo->fVelocity*elapsedtime;

	D3DXVECTOR3 position;
	position.x = 0.0f;
	position.y = camerainfo->fHeight;
	position.z = camerainfo->fDistance;
	
	D3DXMATRIX rotmat;
	D3DXMatrixIdentity(&rotmat);
	D3DXMatrixRotationY(&rotmat, _X_RAD(camerainfo->fRotationAngle));
	D3DXVec3TransformCoord(&position, &position, &rotmat);
	
	position.x += camerainfo->vecCenterPosition.x;
	position.y += camerainfo->vecCenterPosition.y;
	position.z += camerainfo->vecCenterPosition.z;
	g_LodTerrain.m_3PCamera.SetCameraPosition(position);
	g_LodTerrain.m_3PCamera.SetTargetPosition(camerainfo->vecCenterPosition);
	g_LodTerrain.m_3PCamera.mp_fYaw = camerainfo->fRotationAngle;
	g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
	g_LodTerrain.RebuildLevel(FALSE);
	
	camerainfo->dwElapsedTime += elapsedtime;

	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessRotateCameraRight(_XCAMERA_INFO* camerainfo)
{
	DWORD elapsedtime = g_LocalSystemTime - camerainfo->dwElapsedTime;
	camerainfo->fRotationAngle -= camerainfo->fVelocity*elapsedtime;
	
	D3DXVECTOR3 position;
	position.x = 0.0f;
	position.y = camerainfo->fHeight;
	position.z = camerainfo->fDistance;
	
	D3DXMATRIX rotmat;
	D3DXMatrixIdentity(&rotmat);
	D3DXMatrixRotationY(&rotmat, _X_RAD(camerainfo->fRotationAngle));
	D3DXVec3TransformCoord(&position, &position, &rotmat);
	
	position.x += camerainfo->vecCenterPosition.x;
	position.y += camerainfo->vecCenterPosition.y;
	position.z += camerainfo->vecCenterPosition.z;
	g_LodTerrain.m_3PCamera.SetCameraPosition(position);
	g_LodTerrain.m_3PCamera.SetTargetPosition(camerainfo->vecCenterPosition);
	g_LodTerrain.m_3PCamera.mp_fYaw = camerainfo->fRotationAngle;
	g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
	g_LodTerrain.RebuildLevel(FALSE);
	
	camerainfo->dwElapsedTime += elapsedtime;
	
	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessEffectSetting(void)
{
	// m_actiondata1 : target class
	// m_actiondata2 : area category
	// m_numericvalue1 : area id
	// m_numericvalue2 : visual effect id

	switch(m_TDA.m_actiondata1)
	{
	case 0 :
		{
			// �ڱ� �ڽ�
			D3DXMATRIX offsetmat;
			D3DXMatrixIdentity(&offsetmat);
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, m_TDA.m_numericvalue2, &offsetmat);
		}
		break;
	case 1 :
		{
			// ���� Ÿ��
			switch(g_pLocalUser->m_ViewTargetType)
			{
			case _XPICKINGTARGET_MOB :
				{
					_XMob* pMob = (_XMob*)g_pLocalUser->m_ViewTarget;
					if(pMob)
					{
						D3DXMATRIX offsetmat;
						D3DXMatrixIdentity(&offsetmat);
						pMob->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, m_TDA.m_numericvalue2, &offsetmat);
					}
				}
				break;
			case _XPICKINGTARGET_NPC :
				{
					_XNPCObject* pNPC = (_XNPCObject*)g_pLocalUser->m_ViewTarget;
					if(pNPC)
					{
						D3DXMATRIX offsetmat;
						D3DXMatrixIdentity(&offsetmat);
						pNPC->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, m_TDA.m_numericvalue2, &offsetmat);
					}
				}
				break;
			case _XPICKINGTARGET_PC :
				{
					_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
					if(pUser)
					{
						D3DXMATRIX offsetmat;
						D3DXMatrixIdentity(&offsetmat);
						pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, m_TDA.m_numericvalue2, &offsetmat);
					}
				}
				break;
			case _XPICKINGTARGET_FUNCTIONALOBJECT :
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)g_pLocalUser->m_ViewTarget;
					if(pFunctionalObject)
					{
						D3DXMATRIX offsetmat;
						D3DXMatrixIdentity(&offsetmat);
						pFunctionalObject->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, m_TDA.m_numericvalue2, &offsetmat);
					}
				}
				break;
			}
		}
		break;
	case 2 :
		{
			// ����
			D3DXVECTOR3 position;
			g_AreaManager.FindAreaCenterPosition(position, m_TDA.m_numericvalue1, m_TDA.m_actiondata2);

			D3DXMATRIX offsetmat;
			D3DXMatrixIdentity(&offsetmat);
			offsetmat._41 = position.x;
			offsetmat._42 = position.y;
			offsetmat._43 = position.z;
			
			g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, m_TDA.m_numericvalue2, &offsetmat );
		}
		break;
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessTimerSetting(void)
{
	// m_actiondata1 : target class

	switch(m_TDA.m_actiondata1)
	{
	case ID_QUEST_SOUNDTARGET_START :
		return ProcessTimerStart();

	case ID_QUEST_SOUNDTARGET_STOP :
		return ProcessTimerStop();
	}
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessTimerStart(void)
{
	// timer start	
	//	scriptdata->m_TDA.m_actiondata2	= m_TargetClass2;	
	//	scriptdata->m_TDA.m_numericvalue1 = m_Minutes;
	//	scriptdata->m_TDA.m_numericvalue2 = m_Seconds;
	
	_XQuestScriptItem* currentquest = NULL;
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}

	if(currentquest)
	{
		//����ȣ �־��ֱ� 
		DWORD starttime = timeGetTime();
		currentquest->m_mapTriggerTimer[currentquest->GetCurrentTreeNumber()] =  starttime;// g_ServerTimeCode;
				
		if( m_TDA.m_actiondata2 == 0 )//ǥ���ϱ�
		{
			g_QuestScriptManager.m_TimerStartQuestID	= currentquest->GetQuestID();
			g_QuestScriptManager.m_bStartQuestTimer		= TRUE;
			g_QuestScriptManager.m_QuestTimerStartTime	= g_ServerTimeCode;
			if( m_TDA.m_numericvalue1 == 0 && m_TDA.m_numericvalue2 == 0 )
			{
				g_QuestScriptManager.m_QuestTimerTimeLimit	= 0;
			}
			else
			{
				g_QuestScriptManager.m_QuestTimerTimeLimit	= 0;
				g_QuestScriptManager.m_QuestTimerTimeLimit	= (60*m_TDA.m_numericvalue1) + m_TDA.m_numericvalue2;
			}
		}
		else//ǥ������ �ʱ�
		{
		}
	}


	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessTimerStop(void)
{
	// timer stop
	_XQuestScriptItem* currentquest = NULL;
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{
		//����ȣ �־��ֱ� 
		currentquest->m_mapTriggerTimer[currentquest->GetCurrentTreeNumber()] = 0;

		g_QuestScriptManager.m_TimerStartQuestID	= -1;
		g_QuestScriptManager.m_bStartQuestTimer		= FALSE;
		g_QuestScriptManager.m_QuestTimerStartTime	= 0;
		g_QuestScriptManager.m_QuestTimerTimeLimit	= 0;
	}
	
	
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSendMessage(void)
{
	_XQuestScriptItem* currentquest = NULL;
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{
		if( m_lpszMessageString )
		{
			g_QuestScriptManager.SetProgressMessage(m_nOwnerQuestID, currentquest->GetLastTarggetingNPCID(), m_lpszMessageString);
		}
		g_QuestScriptManager.m_bStartMessage = TRUE;

	}
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessCameraSetting(void)
{
	// m_actiondata1 : Camera mode (Zoom in / Zoom out)
	// m_actiondata2 : Setting (���� �� ī�޶� ����/�ǵ�����)
	// m_actiondata3 : Target (���)
	// m_numericvalue1 : Area (����)
	// m_numericvalue2 : Area category 

	// camera setting
	map <int, _XCAMERA_INFO*>::iterator iter_camera = m_mapCameraInfo.find(m_nActionID);
	if(iter_camera == m_mapCameraInfo.end())
	{
		// ó��
		_XCAMERA_INFO* currentcamerainfo = new _XCAMERA_INFO;
		currentcamerainfo->bStart = TRUE;
		currentcamerainfo->dwTotalTime = 2 * 1000;		// total time = 3sec

		if(!g_QuestScriptManager.GetQuestCameraMode())
		{
			g_QuestScriptManager.SetQuestCameraMode(TRUE);
			SaveCameraInfo();
		}

		switch(m_TDA.m_actiondata3)
		{
		case ID_QUEST_CAMERASETTING_PLAYER :
			{
				currentcamerainfo->vecCenterPosition = g_pLocalUser->m_Position;
				currentcamerainfo->vecCenterPosition.y += _XDEF_CHARACTERAVERAGEHEIGHT;
			}
			break;
		case ID_QUEST_CAMERASETTING_AREA :
			{
				if(!g_AreaManager.FindAreaCenterPosition(currentcamerainfo->vecCenterPosition, m_TDA.m_numericvalue1, m_TDA.m_numericvalue2))
				{
					if(m_mapCameraInfo.empty())
					{
						g_QuestScriptManager.SetQuestCameraMode(FALSE);
						RestoreCameraInfo();
						g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
						g_LodTerrain.m_3PCamera.UpdateProjMatrix();
						g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
						g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
						g_LodTerrain.RebuildLevel(FALSE);
					}
					return QUEST_ACTION_RESULT_SUCCESS;
				}
				currentcamerainfo->vecCenterPosition.y += _XDEF_CHARACTERAVERAGEHEIGHT;
			}
			break;
		default :
			{
				currentcamerainfo->vecCenterPosition = g_pLocalUser->m_Position;
				currentcamerainfo->vecCenterPosition.y += _XDEF_CHARACTERAVERAGEHEIGHT;
			}
			break;
		}
		currentcamerainfo->fTargetDistanceVelocity = (0.5*gnFPS) / 1000;
		currentcamerainfo->fFovVelocity = (1.0*gnFPS) / 1000;

		currentcamerainfo->dwStartTime = g_LocalSystemTime;
		currentcamerainfo->dwElapsedTime = currentcamerainfo->dwStartTime;

		m_mapCameraInfo[m_nActionID] = currentcamerainfo;
		return QUEST_ACTION_RESULT_PROCESS;
	}
	else
	{
		_XCAMERA_INFO* currentcamerainfo = iter_camera->second;
		if(!currentcamerainfo)
		{
			m_mapCameraInfo.erase(iter_camera);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}

		if((currentcamerainfo->dwElapsedTime - currentcamerainfo->dwStartTime) < currentcamerainfo->dwTotalTime)
		{
			// ������
			switch(m_TDA.m_actiondata1)
			{
			case ID_QUEST_CAMERASETTING_ZOOMIN :
				return ProcessCameraSettingZoomIn(currentcamerainfo);
			case ID_QUEST_CAMERASETTING_ZOOMOUT :
				return ProcessCameraSettingZoomOut(currentcamerainfo);
			default :
				{
					delete currentcamerainfo;
					m_mapCameraInfo.erase(iter_camera);
//					if(m_mapCameraInfo.empty())
					{
						g_QuestScriptManager.SetQuestCameraMode(FALSE);
						RestoreCameraInfo();
						g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
						g_LodTerrain.m_3PCamera.UpdateProjMatrix();
						g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
						g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
						g_LodTerrain.RebuildLevel(FALSE);
					}
					return QUEST_ACTION_RESULT_SUCCESS;
				}
			}
		}
		else
		{
			// ��
			currentcamerainfo->bEnd = TRUE;
			delete currentcamerainfo;
			m_mapCameraInfo.erase(iter_camera);
			if(m_TDA.m_actiondata2 == ID_QUEST_CAMERA_RESTORE)
			{
				if(g_QuestScriptManager.GetQuestCameraMode())
					g_QuestScriptManager.SetQuestCameraMode(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			return QUEST_ACTION_RESULT_SUCCESS;
		}
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessCameraSettingZoomIn(_XCAMERA_INFO* camerainfo)
{
	DWORD elapsedtime = g_LocalSystemTime - camerainfo->dwElapsedTime;

	g_LodTerrain.m_3PCamera.SetTargetPosition(camerainfo->vecCenterPosition);
	if(g_LodTerrain.m_3PCamera.m_TargetDistance > 2.0f)
		g_LodTerrain.m_3PCamera.m_TargetDistance -= camerainfo->fTargetDistanceVelocity*elapsedtime;
	if(g_LodTerrain.m_3PCamera.mp_fFov > 10.0f)
		g_LodTerrain.m_3PCamera.mp_fFov -= camerainfo->fFovVelocity*elapsedtime;

	g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
	g_LodTerrain.RebuildLevel(FALSE);

	camerainfo->dwElapsedTime += elapsedtime;

	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessCameraSettingZoomOut(_XCAMERA_INFO* camerainfo)
{
	DWORD elapsedtime = g_LocalSystemTime - camerainfo->dwElapsedTime;
	
	g_LodTerrain.m_3PCamera.SetTargetPosition(camerainfo->vecCenterPosition);
	if(g_LodTerrain.m_3PCamera.m_TargetDistance < 15.0f)
		g_LodTerrain.m_3PCamera.m_TargetDistance += camerainfo->fTargetDistanceVelocity*elapsedtime;
	if(g_LodTerrain.m_3PCamera.mp_fFov < 50.0f)
		g_LodTerrain.m_3PCamera.mp_fFov += camerainfo->fFovVelocity*elapsedtime;
	
	g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
	g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
	g_LodTerrain.RebuildLevel(FALSE);
	
	camerainfo->dwElapsedTime += elapsedtime;
	
	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessScreenEffect(void)
{
	// m_actiondata1 : Screen Effect (Fade in / Fade out)

	// screen effect
	map <int, _XCAMERA_INFO*>::iterator iter_camera = m_mapCameraInfo.find(m_nActionID);
	if(iter_camera == m_mapCameraInfo.end())
	{
		// ó��
		_XCAMERA_INFO* currentcamerainfo = new _XCAMERA_INFO;
		currentcamerainfo->bStart = TRUE;
		
		if(!g_QuestScriptManager.GetQuestCameraMode())
		{
			g_QuestScriptManager.SetQuestCameraMode(TRUE);
		}
		
		currentcamerainfo->dwStartTime = g_LocalSystemTime;
		currentcamerainfo->dwElapsedTime = currentcamerainfo->dwStartTime;
		currentcamerainfo->dwTotalTime = 1*1000;		// 3 sec
		
		if(m_TDA.m_actiondata1 == ID_QUEST_SCREENEFFECT_FADEOUT)
			g_FadeInOutLevel = 0;
		else
			g_FadeInOutLevel = 255;
		
		g_DrawFadeInOutFlag = TRUE;
		
		m_mapCameraInfo[m_nActionID] = currentcamerainfo;
		return QUEST_ACTION_RESULT_PROCESS;
	}
	else
	{
		_XCAMERA_INFO* currentcamerainfo = iter_camera->second;
		if(!currentcamerainfo)
		{
			m_mapCameraInfo.erase(iter_camera);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				g_FadeInOutLevel = 0;
				g_DrawFadeInOutFlag = FALSE;
				return QUEST_ACTION_RESULT_SUCCESS;
			}
		}
		
		if((currentcamerainfo->dwElapsedTime - currentcamerainfo->dwStartTime) < currentcamerainfo->dwTotalTime)
		{
			DWORD elapsedtime = g_LocalSystemTime - currentcamerainfo->dwElapsedTime;
			g_DrawFadeInOutFlag = TRUE;
			
			switch(m_TDA.m_actiondata1)
			{
			case ID_QUEST_SCREENEFFECT_FADEOUT :
				{
					g_FadeInOutLevel += _XDEF_SCREENFADE_LEVEL;
					if(g_FadeInOutLevel >= 255)
						g_FadeInOutLevel = 255;
				}
				break;
			case ID_QUEST_SCREENEFFECT_FADEIN :
				{
					g_FadeInOutLevel -= _XDEF_SCREENFADE_LEVEL;
					if(g_FadeInOutLevel <= 0)
						g_FadeInOutLevel = 0;
				}
				break;
			}
			currentcamerainfo->dwElapsedTime += elapsedtime;
			return QUEST_ACTION_RESULT_PROCESS;
		}
		else
		{
			currentcamerainfo->bEnd = TRUE;
			delete currentcamerainfo;
			m_mapCameraInfo.erase(iter_camera);
//			if(m_mapCameraInfo.empty())
			{
				g_QuestScriptManager.SetQuestCameraMode(FALSE);
				g_QuestScriptManager.SetQuestWideView(FALSE);
				RestoreCameraInfo();
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel(FALSE);
			}
			g_FadeInOutLevel = 0;
			g_DrawFadeInOutFlag = FALSE;
			
			return QUEST_ACTION_RESULT_SUCCESS;
		}
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessWeatherStart(void)
{
	// weather start
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessBlockTrigger(void)
{
	// block trigger
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessAutoStart(void)
{
	
	_XQuestScriptItem* currentquest = NULL;
	if( m_TDA.m_numericvalue1 >= 10000 && m_TDA.m_numericvalue1 < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_TDA.m_numericvalue1);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_TDA.m_numericvalue1);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{
		currentquest->SetAutoRun(TRUE);
	}


	return QUEST_ACTION_RESULT_SUCCESS;
}

void _XTriggerActionItem::ReleasePrivateData(void)
{
	m_bSuccessClient		= FALSE;
	m_bSuccessServer		= FALSE;
	m_bConfirmAll			= FALSE;

	m_mapCameraValue.clear();

	map <int, _XCAMERA_INFO*>::iterator iter_camera;
	for(iter_camera = m_mapCameraInfo.begin() ; iter_camera != m_mapCameraInfo.end() ; ++iter_camera)
	{
		_XCAMERA_INFO* currentcamera = iter_camera->second;
		if(currentcamera)
			SAFE_DELETE(currentcamera);
	}
	m_mapCameraInfo.clear();
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessQuestProgressInfo(void)
{
	//����Ʈ â�� ���� ���� ǥ��
	_XQuestScriptItem* currentquest = NULL;
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{
		if(!m_lpszMessageString)
			return QUEST_ACTION_RESULT_ERROR;
		
		int strlength = strlen(m_lpszMessageString);
		
		if(strlength > 0)
		{
			currentquest->m_lpszQuestProgressinformation = m_lpszMessageString;
			if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
		}		
	}


	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSelection(void)
{	
	//	scriptdata->m_TDA.m_actiondata1 = m_ItemClass1;
	//	scriptdata->m_TDA.m_actiondata2 = m_ItemClass2;
	//	scriptdata->m_TDA.m_actiondata3 = m_ItemClass3;
	//	scriptdata->m_TDA.m_actiondata4 = m_ItemClass4;
	//	scriptdata->m_TDA.m_numericvalue1 = m_ItemID1;
	//	scriptdata->m_TDA.m_numericvalue2 = m_ItemID2;
	//	scriptdata->m_TDA.m_numericvalue3 = m_ItemID3;
	//	scriptdata->m_TDA.m_numericvalue4 = m_ItemID4;

	
	_XQuestScriptItem* currentquest = NULL;
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{
		if(!m_lpszMessageString)
			return QUEST_ACTION_RESULT_ERROR;
		
		if( g_QuestScriptManager.m_bStartMessage )
		{
			return QUEST_ACTION_RESULT_PROCESS;
		}
		else
		{			
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
			if( pNPCScript_Window )
			{
				if( !pNPCScript_Window->m_bNewChatDisplay )
				{
					pNPCScript_Window->m_listboxSelectionItem->DeleteAllItem();

					// ������ ������ �� 
					if( m_TDA.m_actiondata1 > -1 || m_TDA.m_actiondata2 > -1 || m_TDA.m_actiondata3 > -1 || m_TDA.m_actiondata4 > -1  )
					{

						pNPCScript_Window->m_listboxSelectionItem->SetIconReportMode( TRUE );
						pNPCScript_Window->m_listboxSelectionItem->SetIconReportModeInfo( 2, 2, 45, 181 );
						
						pNPCScript_Window->m_listboxSelectionItem->SetTrackWidth(181);
						pNPCScript_Window->m_listboxSelectionItem->SetTrackHeight(45);
						
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);	// column style 	
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);	// column style 	
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(2, _XLCOLUMNSTYLE_ICONRECT);	// column style 	
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(3, _XLCOLUMNSTYLE_ICONRECT);	// column style 
						
						POINT selectioncoloffset1 = { 0, 0 };
						POINT selectioncoloffset2 = { 67, 17 };
						POINT selectioncoloffset3 = { 27, 8 };
						POINT selectioncoloffset4 = { 24, 5 };
						pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(0, selectioncoloffset1);	// column offset
						pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(1, selectioncoloffset2);	// column offset
						pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(2, selectioncoloffset3);	// column offset
						pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(3, selectioncoloffset4);	// column offset
						
						TCHAR	buffer[2048];
						_XStringSplitInfo splitinfo;

						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
						{
							g_XBaseFont->SplitString(m_lpszMessageString, 342, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR );
						}
						else
						{
							g_XBaseFont->SplitStringByCR(m_lpszMessageString, splitinfo);
						}
						
						for(int i = 0 ; i < splitinfo.splitcount ; ++i)
						{
							memset(buffer, 0, sizeof(buffer));
							strncpy(buffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
							
							
							int npcresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
							RECT rect1 = {6, 412, 187, 457};	
							RECT rect2 = {0, 0, 0, 0};							
							RECT rect3 = {450, 240, 486, 276};	

							int imageiconindex = 0, category = -1, itemid = -1;															
							
							switch(i) {
							case 0:
								{
									category = m_TDA.m_actiondata1;
									itemid = m_TDA.m_numericvalue1;
								}
								break;
							case 1:
								{
									category = m_TDA.m_actiondata2;
									itemid = m_TDA.m_numericvalue2;
								}
								break;
							case 2:
								{
									category = m_TDA.m_actiondata3;
									itemid = m_TDA.m_numericvalue3;
								}
								break;
							case 3:
								{
									category = m_TDA.m_actiondata4;
									itemid = m_TDA.m_numericvalue4;
								}
								break;
							}
							g_pInventory_Window->GetIconRect(category, itemid, imageiconindex, rect2, TRUE);
							

							pNPCScript_Window->m_listboxSelectionItem->InsertListItem(_T(""), 0, i+1);
							
							pNPCScript_Window->m_listboxSelectionItem->SetIconArchive(i, &g_MainInterfaceTextureArchive);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 0, npcresourceindex1, rect1);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 2, imageiconindex, rect2);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 3, npcresourceindex1, rect3);

							if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
							{
								pNPCScript_Window->m_listboxSelectionItem->SetSeparateText(i, 1, buffer, 100 );
							}
							else
							{
								pNPCScript_Window->m_listboxSelectionItem->SetItemText(i, 1, buffer);
							}
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 1, _XSC_DEFAULT);	
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttribHighlight(i, 1, _XSC_DEFAULT_HIGHLIGHT);		
							
						}
						
						POINT winpos = pNPCScript_Window->GetWindowPos();
						pNPCScript_Window->m_listboxSelectionItem->MoveWindow( winpos.x + 35, winpos.y + 300 );

						pNPCScript_Window->m_NPCDialogSubBorderB1->ShowWindow(TRUE);
						pNPCScript_Window->m_NPCDialogSubBorderB2->ShowWindow(TRUE);
						pNPCScript_Window->m_btnQuestOK->ShowWindow(FALSE);
						pNPCScript_Window->m_btnBackToNPCMode->ShowWindow(FALSE);
						pNPCScript_Window->m_listboxSelectionItem->ShowWindow(TRUE);
						pNPCScript_Window->m_btnNPCDialogModExit->ShowWindow(FALSE);
						
						currentquest->m_bSelectionNumberCheck = FALSE;
						currentquest->m_SelectionNumber = -1;
						return QUEST_ACTION_RESULT_SUCCESS;

					}
					else // ������ ������ �� 					
					{
						pNPCScript_Window->m_listboxSelectionItem->SetIconReportMode( FALSE );
						
						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
						{
							POINT selectioncoloffset1 = { 0, 3 };
							POINT selectioncoloffset2 = { 14, 2 };
							POINT selectioncoloffset3 = { 27, 8 };
							POINT selectioncoloffset4 = { 24, 5 };
							
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(0, selectioncoloffset1);	// column offset
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(1, selectioncoloffset2);	// column offset
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(2, selectioncoloffset3);	// column offset
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(3, selectioncoloffset4);	// column offset
						}
						else
						{
							POINT selectioncoloffset1 = { 0, 3 };
							POINT selectioncoloffset2 = { 14, 4 };
							POINT selectioncoloffset3 = { 27, 8 };
							POINT selectioncoloffset4 = { 24, 5 };
							
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(0, selectioncoloffset1);	// column offset
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(1, selectioncoloffset2);	// column offset
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(2, selectioncoloffset3);	// column offset
							pNPCScript_Window->m_listboxSelectionItem->SetColumnOffset(3, selectioncoloffset4);	// column offset
						}

						pNPCScript_Window->m_listboxSelectionItem->SetTrackWidth(400);
						pNPCScript_Window->m_listboxSelectionItem->SetTrackHeight(20);
						
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);	// column style 	
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);	// column style 	
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(2, _XLCOLUMNSTYLE_ICONRECT);	// column style 	
						pNPCScript_Window->m_listboxSelectionItem->SetColumnStyle(3, _XLCOLUMNSTYLE_ICONRECT);	// column style 
						
//						pNPCScript_Window->m_listboxSelectionItem->SetSelectedImageArchive(-1, &g_MainInterfaceTextureArchive);
//						pNPCScript_Window->m_listboxSelectionItem->SetSelectedImagePosition(0, 1);
//						pNPCScript_Window->m_listboxSelectionItem->SetSelectedImageHeight(20);
//						pNPCScript_Window->m_listboxSelectionItem->SetTrackWidth(330);


						TCHAR	buffer[2048];
						_XStringSplitInfo splitinfo;
						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
						{
							g_XBaseFont->SplitString(m_lpszMessageString, 600, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR );
							//g_XBaseFont->SplitStringByCR(m_lpszMessageString, splitinfo);
						}
						else
						{
							g_XBaseFont->SplitStringByCR(m_lpszMessageString, splitinfo);
						}
						
						for(int i = 0 ; i < splitinfo.splitcount ; ++i)
						{
							memset(buffer, 0, sizeof(buffer));
							strncpy(buffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
							
							
							int npcresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
							RECT rect = {0, 487, 18, 499};												
							
							pNPCScript_Window->m_listboxSelectionItem->InsertListItem(_T(""), 0, i+1);
							
							pNPCScript_Window->m_listboxSelectionItem->SetIconArchive(i, &g_MainInterfaceTextureArchive);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 0, npcresourceindex1, rect);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 2, -1, rect);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 3, -1, rect);
							
							pNPCScript_Window->m_listboxSelectionItem->SetItemText(i, 1, buffer);
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttrib(i, 1, _XSC_DEFAULT_BLACK);	
							pNPCScript_Window->m_listboxSelectionItem->SetItemAttribHighlight(i, 1, _XSC_INFORMATION);		
							
						}

						POINT winpos = pNPCScript_Window->GetWindowPos();
						
						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
						{
							pNPCScript_Window->m_listboxSelectionItem->MoveWindow( winpos.x + 19, winpos.y + 338 - ( 8 * splitinfo.splitcount ) );
						}
						else
						{
							pNPCScript_Window->m_listboxSelectionItem->MoveWindow( winpos.x + 65, winpos.y + 338 - ( 8 * splitinfo.splitcount ) );
						}

						pNPCScript_Window->m_NPCDialogSubBorderB1->ShowWindow(TRUE);
						pNPCScript_Window->m_NPCDialogSubBorderB2->ShowWindow(TRUE);
						pNPCScript_Window->m_btnQuestOK->ShowWindow(FALSE);
						pNPCScript_Window->m_btnBackToNPCMode->ShowWindow(FALSE);
						pNPCScript_Window->m_btnNPCDialogModExit->ShowWindow(FALSE);
						pNPCScript_Window->m_listboxSelectionItem->ShowWindow(TRUE);
						
						currentquest->m_bSelectionNumberCheck = FALSE;
						currentquest->m_SelectionNumber = -1;
						return QUEST_ACTION_RESULT_SUCCESS;
					}
				}
				else
				{
					return QUEST_ACTION_RESULT_PROCESS;
				}
			}			
		}
		
	}
	return QUEST_ACTION_RESULT_PROCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessStartMiniGame(void)
{
	//m_actiondata1 0:�������
	switch(m_TDA.m_actiondata1) {
	case 0: // �������
		{
			_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
			if(pMeditation_Window)
			{
				pMeditation_Window->m_bQuestMiniGameStart = TRUE;
				pMeditation_Window->SetMeditationStep(0);
			}
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_TRAINING_MEDITATION_START, 0), (LPARAM)gHWnd);
/*			g_pLocalUser->m_UserState = _XDEF_USERSTATE_TRAINING;
			g_pLocalUser->m_SelectedTrainingMode = _XTRAINING_MEDITATION;
			g_pLocalUser->m_CurrentTrainingState = _XUSER_STATE_10;
			
			_XWindow_TrainingMeditation* pMeditation_Window = (_XWindow_TrainingMeditation*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
		
			if( pMeditation_Window )
			{
				pMeditation_Window->GetMeditationSkillid();
				pMeditation_Window->GetPathIndex();
				pMeditation_Window->ShowWindow(TRUE);
				pMeditation_Window->SetMeditationMode(MM_READY2);
				pMeditation_Window->m_bMustGameStart = TRUE;				
				
				XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;		
				pProc_Maingame->m_bMiniGameStart = TRUE;	
				
				extern BOOL g_PrevSoundEffectFlag;
				g_PrevSoundEffectFlag = g_SoundEffectFlag;
				g_SoundEffectFlag = FALSE;
			}*/
		}
		break;
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessTalkingToOneself(void)
{
	// send message
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{		
		if( m_lpszMessageString )
		{
			g_pLocalUser->m_ChatBalloonObject.m_MessageColor = g_ChattingStringColor[7];//�ӼӸ� ����
			g_pLocalUser->m_ChatBalloonObject.SetMessage(m_lpszMessageString);		
		}
	}

	
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessMoveVelocity(void)
{
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessStartOptionCondition(void)
{
	// m_actiondata1 : m_TargetClass
	// ID_QUEST_OPTIONRESULT_CANCLERESET				0
	// ID_QUEST_OPTIONRESULT_CANCLEFAIL					1
	// m_numericvalue1 : m_OptionConditionID
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{		
		_XTriggerConditionItem* currentconditiontrigger = NULL;
		map <int, _XTriggerConditionItem*>::iterator iter_trigger;
		iter_trigger = currentquest->m_mapOptionConditionList.find(m_TDA.m_numericvalue1);
		if(iter_trigger != currentquest->m_mapOptionConditionList.end())
		{
			currentconditiontrigger = iter_trigger->second;	
			if(currentconditiontrigger)
			{				
				
				_XTriggerConditionItem* failcondition = new _XTriggerConditionItem;

				if(failcondition)
				{
					failcondition = currentconditiontrigger;
					
					if( m_TDA.m_actiondata1 == ID_QUEST_OPTIONRESULT_CANCLESUCCESS )
					{
						failcondition->m_bSuccessCancle = TRUE;
					}
					else if( m_TDA.m_actiondata1 == ID_QUEST_OPTIONRESULT_CANCLEFAIL )
					{
						failcondition->m_bSuccessCancle = FALSE;
					}

					currentquest->m_mapFailConditionList[ m_TDA.m_numericvalue1 ] = failcondition;
				}
			}	
		}
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessEndOptionCondition(void)
{
	// m_numericvalue1 : m_OptionConditionID
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{
		if( !currentquest->m_mapFailConditionList.empty() )
		{
			
			map <int, _XTriggerConditionItem*>::iterator iter_trigger;
			iter_trigger = currentquest->m_mapFailConditionList.find(m_TDA.m_numericvalue1);
			if(iter_trigger != currentquest->m_mapFailConditionList.end())
			{
				_XTriggerConditionItem* failcondition = iter_trigger->second;	
				if(failcondition)
				{
					//delete failcondition;
					//failcondition = NULL;
					currentquest->m_mapFailConditionList.erase(iter_trigger);
				}
			}
		}
	}
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessGiveCountPoint(void)
{	
	//m_actiondata1 = m_CounterType;	
	//m_actiondata2 = m_DataSet;	// 0: �߰� 1: ����
	//m_numericvalue1  = m_CountPoint;	


	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_ACTION_RESULT_SUCCESS;

	// ��Ƽ ����Ʈ�� �������� ���� ������ �����ؾ� �Ѵ�.
	if( m_TDA.m_actiondata1 >= 4 )
		return QUEST_ACTION_RESULT_SUCCESS;

	switch(m_TDA.m_actiondata2) {
	case ID_TRIGGER_DATASET_ADD:		
		{
			currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint += m_TDA.m_numericvalue1;	
						
			TCHAR	buffer[256];
			memset(buffer, 0, sizeof(buffer));
			if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint >= 1 )
			{
				if(currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName[0] != 0 )
				{
					if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint > currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint)
					{
						_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint );						
					}
					else if(currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint == currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint )
					{
						_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT),//_T("%s : %d/%d (�Ϸ�)")
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint );							
					}	
					if( g_pQuest_Window )
					{
						if( g_pQuest_Window->GetShowStatus() )
						{
							if( g_pQuest_Window->m_SelectedQuestID == m_nOwnerQuestID  )
							{
								g_pQuest_Window->ParseQuestDescString();	
							}
						}
					}				
					
					if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint >= currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint)
					{
						if( buffer[0] != 0 )
						{
							int strlength = strlen(buffer);
							
							if(g_QuestScriptManager.m_lpszQuestSystemMessage )
							{
								delete g_QuestScriptManager.m_lpszQuestSystemMessage;
								g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
							}
							
							if(strlength > 0)
							{
								g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
								g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
								strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
							}
							
							g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
							g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
						}				
					}					
				}							
			}				
			else if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint == 0 )
			{		
				_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
					currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName,
					currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint );
				
				if( g_pQuest_Window )
				{
					if( g_pQuest_Window->GetShowStatus() )
					{
						if( g_pQuest_Window->m_SelectedQuestID == m_nOwnerQuestID  )
						{
							g_pQuest_Window->ParseQuestDescString();	
						}
					}
				}				

				if( buffer[0] != 0 )
				{
					int strlength = strlen(buffer);
					
					if(g_QuestScriptManager.m_lpszQuestSystemMessage )
					{
						delete g_QuestScriptManager.m_lpszQuestSystemMessage;
						g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
					}
					
					if(strlength > 0)
					{
						g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
						g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
						strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
					}
					
					g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
					g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
				}								
			}// else if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint == 0 )		
		}
		break;
	case ID_TRIGGER_DATASET_SUBTRACT:
		{
			currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint -= m_TDA.m_numericvalue1;

			TCHAR	buffer[256];
			memset(buffer, 0, sizeof(TCHAR)*256);
			if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint >= 1 )
			{
				if(currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName[0] != 0 )
				{
					if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint > currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint)
					{
						_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint );	
						
						if( g_pQuest_Window )
						{
							if( g_pQuest_Window->GetShowStatus() )
							{
								if( g_pQuest_Window->m_SelectedQuestID == m_nOwnerQuestID  )
								{
									g_pQuest_Window->ParseQuestDescString();	
								}
							}
						}					
					}
					else if(currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint == currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint )
					{
						_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT), //_T("%s : %d/%d (�Ϸ�)")
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint,
							currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint );	
						
						if( g_pQuest_Window )
						{
							if( g_pQuest_Window->GetShowStatus() )
							{
								if( g_pQuest_Window->m_SelectedQuestID == m_nOwnerQuestID  )
								{
									g_pQuest_Window->ParseQuestDescString();	
								}
							}
						}										
					}	
					
					if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint >= currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint)
					{
						if( buffer[0] != 0 )
						{
							int strlength = strlen(buffer);
							
							if(g_QuestScriptManager.m_lpszQuestSystemMessage )
							{
								delete g_QuestScriptManager.m_lpszQuestSystemMessage;
								g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
							}
							
							if(strlength > 0)
							{
								g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
								g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
								strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
							}
							
							g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
							g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
						}				
					}					
				}	
			}
			else if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint == 0 )
			{		
				_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
					currentquest->m_CounterInformation[m_TDA.m_actiondata1].CounterTargetName,
					currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountCurrentPoint );

				if( g_pQuest_Window )
				{
					if( g_pQuest_Window->GetShowStatus() )
					{
						if( g_pQuest_Window->m_SelectedQuestID == m_nOwnerQuestID  )
						{
							g_pQuest_Window->ParseQuestDescString();	
						}
					}
				}				
				
				if( buffer[0] != 0 )
				{
					int strlength = strlen(buffer);
					
					if(g_QuestScriptManager.m_lpszQuestSystemMessage )
					{
						delete g_QuestScriptManager.m_lpszQuestSystemMessage;
						g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
					}
					
					if(strlength > 0)
					{
						g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
						g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
						strncpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer, strlength);
					}
					
					g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
					g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
				}							
			}// else if( currentquest->m_CounterInformation[m_TDA.m_actiondata1].CountTotalPoint == 0 )		
		}
		break;
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSpawnMonster(void)// ���� �ҷ�����
{
	//scriptdata->m_TDA.nActionData1  = m_index1;				// �÷��̾�,����,DNPC�÷��̾�,DNPC����,DNPC�÷��̾�(RANDOM),DNPC����(RANDOM)
	// Ŭ���̾�Ʈ���ͺθ���(����),Ŭ���̾�ƮDNPC�θ���(����),Ŭ���̾�ƮDNPC�����ϱ�
	//scriptdata->m_TDA.nActionData2  = m_value6;				// �Ÿ�
	//scriptdata->m_TDA.nActionData3  = m_index2;				// ����ī�װ�
	//scriptdata->m_TDA.nActionData4  = m_value3;				// �н� Ÿ��
	//scriptdata->m_TDA.nNumericValue1  = m_value1;			// �������̵�
	//scriptdata->m_TDA.nNumericValue2  = m_value2;			// ���� ���ݷ�
	//scriptdata->m_TDA.nNumericValue3  = m_value4;			// ���;��̵�
	//scriptdata->m_TDA.nNumericValue4  = m_value5;			// ���� ����

	if( m_TDA.m_actiondata1 == 6 )//Ŭ���̾�Ʈ���ͺθ���(����)
	{
		_XArea* pArea = g_AreaManager.GetArea( m_TDA.m_actiondata3, m_TDA.m_numericvalue1 );
		
		if( pArea )
		{
			int monstercount = m_TDA.m_numericvalue4;
			int monsterindex = m_TDA.m_numericvalue3;
			FLOAT position_x = 0;
			FLOAT position_z = 0;
			FLOAT x = 0;
			FLOAT z = 0;
			D3DXVECTOR3 monsterposition;
			for( int i = 0; i < monstercount; ++i )
			{				
				position_x = _XRandom()%100;
				position_z = _XRandom()%100;

				x = (pArea->m_Point[2].x - pArea->m_Point[0].x)*((FLOAT)position_x/(FLOAT)100.0f);
				z = (pArea->m_Point[2].z - pArea->m_Point[0].z)*((FLOAT)position_z/(FLOAT)100.0f);

				monsterposition.x = pArea->m_Point[0].x + x;
				monsterposition.y = 0;
				monsterposition.z = pArea->m_Point[0].z + z;
			
				int clientmobID = g_Moblist.FindClientMobID();
				if( clientmobID != 0 )
				{
					_XMob* pMob = g_Moblist.CreateMob(clientmobID, monsterindex, 0, 100, monsterposition,monsterposition);	
					if( pMob )
					{
						pMob->m_bClientMob = TRUE;
						pMob->m_OriginPosition = monsterposition;
						pMob->m_AttackHitValue = g_MobAttribTable[monsterindex-4000].attackpowermax; 
						pMob->m_CurrentLife = pMob->m_MaxLife = 100;
					}
				}
			}
		}
	}
	else if( m_TDA.m_actiondata1 == 7 )//Ŭ���̾�ƮDNPC�θ���(����)
	{
		_XArea* pArea = g_AreaManager.GetArea( m_TDA.m_actiondata3, m_TDA.m_numericvalue1 );
		
		if( pArea )
		{
			int monstercount = m_TDA.m_numericvalue4;
			int monsterindex = m_TDA.m_numericvalue3;
			FLOAT position_x = 0;
			FLOAT position_z = 0;
			FLOAT x = 0;
			FLOAT z = 0;
			D3DXVECTOR3 monsterposition;
			for( int i = 0; i < monstercount; ++i )
			{								
				x = (pArea->m_Point[2].x - pArea->m_Point[0].x)*(0.5f);
				z = (pArea->m_Point[2].z - pArea->m_Point[0].z)*(0.5f);
				
				monsterposition.x = pArea->m_Point[0].x + x;
				monsterposition.y = 0;
				monsterposition.z = pArea->m_Point[0].z + z;
				
				int clientmobID = g_Moblist.FindClientMobID();
				if( clientmobID != 0 )
				{
					_XMob* pMob = g_Moblist.CreateMob(clientmobID, monsterindex, 0, 100, monsterposition,monsterposition);	
					if( pMob )
					{
						pMob->m_bClientMob = TRUE;
						pMob->m_bClientDNPC = TRUE;
						pMob->m_OriginPosition = monsterposition;
						pMob->m_AttackHitValue = g_MobAttribTable[monsterindex-4000].attackpowermax; 
						pMob->m_CurrentLife = pMob->m_MaxLife = 100;
						
						
						pMob->m_EndPosition.x = -67.574074f;
						pMob->m_EndPosition.y = 0.0f;
						pMob->m_EndPosition.z = -61.161400f;
					}
				}
			}
		}
	}
	else if( m_TDA.m_actiondata1 == 8 )//Ŭ���̾�ƮDNPC�����ϱ�
	{
		_XMob* pMob = NULL;
		for (smdef_XMob::iterator it = g_Moblist.m_smMobList.begin(); it != g_Moblist.m_smMobList.end(); ++ it)
		{
			pMob = (*it).second;
			
			if( pMob->m_bClientMob ) // Ŭ���̾�Ʈ ���ͷ� �ٲ۴�.
			{
				if( pMob->m_bClientDNPC )//Ŭ���̾�Ʈ ����
				{
					if( pMob->m_MobType == (m_TDA.m_numericvalue3) )
					{
						pMob->m_KillSignal = _XDEF_MOB_KILLSIGNAL_END;
					}
				}
			}
		}
	}

	
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSetMotion(void)
{
	// m_numericvalue1 : m_MotionIndex
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSetMovie(void)
{	
	// m_numericvalue1 : m_MovieIndex
	g_indexScene = m_TDA.m_numericvalue1;
	if( g_SceneList[g_indexScene ] )
	{
		g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		((CEmperorOfDragonsApp*)gApp)->m_proc_flash.m_MovieMode = _XMOVIE_QUEST;
		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.SuspendProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_flash); 
	}
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSetMap(void)// ���� ON/OFF
{
	// m_actiondata1 : 0:ON, 1:OFF
	switch(m_TDA.m_actiondata1) {
	case 0://ON
		{
			_XWindow_MaximumMap* pMaximumMap_Window = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
			if( pMaximumMap_Window ) pMaximumMap_Window->ShowWindow(TRUE);			
		}
		break;
	case 1://OFF
		{
			_XWindow_MaximumMap* pMaximumMap_Window = (_XWindow_MaximumMap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MAXMINIMAPWINDOW );
			if( pMaximumMap_Window ) pMaximumMap_Window->ShowWindow(FALSE);		
		}
		break;
	}
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessMobtalkingToOneself(void)// ���� ȥ�ڸ��ϱ�
{
	// m_numericvalue1 : Mob ID
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessResultQuest(void)
{
	// m_actiondata1 : ����Ʈ ��� 0:���� 1:���� 2:���;

	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		return QUEST_ACTION_RESULT_FAIL;
//		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
//		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
//			currentquest = iter_quest->second;
//		else
//			return QUEST_ACTION_RESULT_FAIL;

	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_ACTION_RESULT_FAIL;
	
	if( m_TDA.m_actiondata1 == 2 )
	{
		g_NetworkKernel.SendPacket(MSG_NO_QUEST_USER_CANCEL, m_nOwnerQuestID, currentquest->GetCurrentTreeNumber() );	
		return QUEST_ACTION_RESULT_SUCCESS;
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessStartItemCounter(void)// ������ ī���� �����ϱ�
{
	
	//scriptdata->m_TDA.m_actiondata1  = m_ItemClass;	
	//scriptdata->m_TDA.m_numericvalue1  = m_ItemID;
	//scriptdata->m_TDA.m_numericvalue2  = m_Count;
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	
	if( m_TDA.m_actiondata1 <= _XGI_FC_SOCKET )
	{
		bool bexist = false;
		for( int i = 0; i < 8; ++i )
		{
			if( currentquest->m_ItemCounterInformation[i].CounterTargetType == m_TDA.m_actiondata1 
				&& currentquest->m_ItemCounterInformation[i].CounterTargetID == m_TDA.m_numericvalue1
				&& currentquest->m_ItemCounterInformation[i].CountTotalPoint == m_TDA.m_numericvalue2 )
			{
				bexist = true;
				break;
			}
		}
		
		if( !bexist )
		{	
			for( int itemcounterindex = 0; itemcounterindex < 8; ++itemcounterindex )
			{
				if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType == -1 )
				{	
					// ī���� ����
					currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType = m_TDA.m_actiondata1; // Ÿ��
					currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID = m_TDA.m_numericvalue1;   // ���̵� 
					currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint = m_TDA.m_numericvalue2;	 // �� ���� 
					currentquest->m_ItemCounterInformation[itemcounterindex].CountCurrentPoint = 0;
					
					if( m_lpszMessageString )
					{
						int strlength = strlen(m_lpszMessageString);
						
						if(strlength > 0)
						{
							memset( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, 0, sizeof(TCHAR)*256 );
							strcpy( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, m_lpszMessageString );
						}		
						else
						{			
							char cType = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType;
							short sID = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID;
							
							memset(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, 0, sizeof(TCHAR)*256);
							strcpy(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, _XGameItem::GetItemName(cType, sID) );
						}	
					}
					else
					{
						char cType = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType;
						short sID = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID;
						
						memset(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, 0, sizeof(TCHAR)*256);
						strcpy(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, _XGameItem::GetItemName(cType, sID) );			
						
					}
					
					TCHAR	buffer[256];
					memset(buffer, 0, sizeof(TCHAR)*256);
					
					int itemtotalcount = 0;
					for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == m_TDA.m_actiondata1
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDA.m_numericvalue1)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();														
					}

				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == m_TDA.m_actiondata1
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDA.m_numericvalue1)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();		
					}
				#endif
					
					if( itemtotalcount != 0 )
					{
						if( currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint != 0 )
						{
							if( itemtotalcount < currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
							{
								_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
									currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
									itemtotalcount,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint );
							}
							else if( itemtotalcount >= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
							{
								_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT), //_T("%s : %d/%d (�Ϸ�)")
									currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint);
							}
							
							if( itemtotalcount <= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
							{
								if( buffer[0] != 0 )
								{
									int strlength = strlen(buffer);
									
									if(g_QuestScriptManager.m_lpszQuestSystemMessage )
									{
										SAFE_DELETE(g_QuestScriptManager.m_lpszQuestSystemMessage);
									}
									
									if(strlength > 0)
									{
										g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
										g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
										strncpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer, strlength);
									}
									
									g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
									g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
								}		
							}			

						}
						else
						{
							_snprintf(buffer, sizeof(buffer), _T("%s : %d"),
								currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, itemtotalcount);

							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(g_QuestScriptManager.m_lpszQuestSystemMessage )
								{
									delete g_QuestScriptManager.m_lpszQuestSystemMessage;
									g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
								}
								
								if(strlength > 0)
								{
									g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
									strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
								}
								
								g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
								g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}	
					}
					
					
					break;
				}
				
			}
		}
		if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
	}
	else if( m_TDA.m_actiondata1 > 14 && m_TDA.m_actiondata1 < 19 ) //15,16,17,18 <-ī��Ʈ A B C D 
	{
		
		int countertype = m_TDA.m_actiondata1 - _XGI_FC_SOCKET -1;
		
		// ī���� ����
		currentquest->m_CounterInformation[countertype].CounterType = countertype;
		currentquest->m_CounterInformation[countertype].CounterTargetID = m_TDA.m_numericvalue1;
		currentquest->m_CounterInformation[countertype].CountTotalPoint = m_TDA.m_numericvalue2;
		currentquest->m_CounterInformation[countertype].CountCurrentPoint = 0;
		
		if( m_lpszMessageString )
		{
			int strlength = strlen(m_lpszMessageString);
			
			if(strlength > 0)
			{
				memset( currentquest->m_CounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
				strcpy( currentquest->m_CounterInformation[countertype].CounterTargetName, m_lpszMessageString );
			}		
			else
			{			
				int mobid = currentquest->m_CounterInformation[countertype].CounterTargetID- 4000;
				memset( currentquest->m_CounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
				if( g_MonsterName[mobid] )
					strcpy( currentquest->m_CounterInformation[countertype].CounterTargetName, g_MonsterName[mobid] );
				else
					strcpy( currentquest->m_CounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR) );
			}
		}
		else
		{
			int mobid = currentquest->m_CounterInformation[countertype].CounterTargetID- 4000;
			memset( currentquest->m_CounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
			if( g_MonsterName[mobid] )
				strcpy( currentquest->m_CounterInformation[countertype].CounterTargetName, g_MonsterName[mobid] );
			else
				strcpy( currentquest->m_CounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR) );
		}
		if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
	}
	else if( m_TDA.m_actiondata1 > 18 && m_TDA.m_actiondata1 < 23 ) //19,20,21,22 <-��Ƽ ī��Ʈ A B C D 
	{
		int countertype = m_TDA.m_actiondata1 - _XGI_FC_SOCKET - 5;
		
		// ī���� ����
		currentquest->m_PartyCounterInformation[countertype].CounterType		= countertype;
		currentquest->m_PartyCounterInformation[countertype].CounterTargetType	= -1;
		currentquest->m_PartyCounterInformation[countertype].CounterTargetID	= m_TDA.m_numericvalue1;
		currentquest->m_PartyCounterInformation[countertype].CountTotalPoint	= m_TDA.m_numericvalue2;
		currentquest->m_PartyCounterInformation[countertype].CountCurrentPoint	= 0;
		
		if( m_lpszMessageString )
		{
			int strlength = strlen(m_lpszMessageString);
			
			if(strlength > 0)
			{
				memset( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
				strcpy( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, m_lpszMessageString);			
			}		
			else
			{			
				int mobid = currentquest->m_PartyCounterInformation[countertype].CounterTargetID- 4000;
				memset( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
				if( g_MonsterName[mobid] )
					strcpy( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, g_MonsterName[mobid] );
				else
					strcpy( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR) );
			}
		}
		else
		{
			int mobid = currentquest->m_PartyCounterInformation[countertype].CounterTargetID- 4000;
			memset( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
			if( g_MonsterName[mobid] )
				strcpy( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, g_MonsterName[mobid] );
			else
				strcpy( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR) );
		}
		if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
	}
	else if( m_TDA.m_actiondata1 > 22  )
	{
		bool bexist = false;
		for( int i = 0; i < 8; ++i )
		{
			if( currentquest->m_ItemCounterInformation[i].CounterTargetType == (m_TDA.m_actiondata1-8) 
				&& currentquest->m_ItemCounterInformation[i].CounterTargetID == m_TDA.m_numericvalue1
				&& currentquest->m_ItemCounterInformation[i].CountTotalPoint == m_TDA.m_numericvalue2 )
			{
				bexist = true;
				break;
			}
		}
		
		if( !bexist )
		{	
			for( int itemcounterindex = 0; itemcounterindex < 8; ++itemcounterindex )
			{
				if( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType == -1 )
				{	
					// ī���� ����
					currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType = (m_TDA.m_actiondata1-8); // Ÿ��
					currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID = m_TDA.m_numericvalue1;   // ���̵� 
					currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint = m_TDA.m_numericvalue2;	 // �� ���� 
					currentquest->m_ItemCounterInformation[itemcounterindex].CountCurrentPoint = 0;
					
					if( m_lpszMessageString )
					{
						int strlength = strlen(m_lpszMessageString);
						
						if(strlength > 0)
						{
							memset( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, 0, sizeof(TCHAR)*256 );
							strcpy( currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, m_lpszMessageString );			
						}		
						else
						{			
							char cType = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType;
							short sID = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID;
							
							memset(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, 0, sizeof(TCHAR)*256);
							strcpy(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, _XGameItem::GetItemName(cType, sID) );
						}	
					}
					else
					{
						char cType = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetType;
						short sID = currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetID;
						
						memset(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, 0, sizeof(TCHAR)*256);
						strcpy(currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, _XGameItem::GetItemName(cType, sID) );			
						
					}
					
					TCHAR	buffer[256];
					memset(buffer, 0, sizeof(TCHAR)*256);
					
					int itemtotalcount = 0;
					for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == m_TDA.m_actiondata1
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDA.m_numericvalue1)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();														
					}

				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == m_TDA.m_actiondata1
							&& g_pLocalUser->m_UserItemList[i].Get_m_sID() == m_TDA.m_numericvalue1)
							itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();	
					}
				#endif
					
					if( itemtotalcount != 0 )
					{
						if( currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint != 0 )
						{
							if( itemtotalcount < currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
							{
								_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
									currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
									itemtotalcount,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint );
							}
							else if( itemtotalcount >= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
							{
								_snprintf(buffer, sizeof(buffer), _XGETINTERFACETEXT(ID_STRING_QUEST_COUNT), //_T("%s : %d/%d (�Ϸ�)")
									currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint,
									currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint);
							}
							
							if( itemtotalcount <= currentquest->m_ItemCounterInformation[itemcounterindex].CountTotalPoint )
							{
								if( buffer[0] != 0 )
								{
									int strlength = strlen(buffer);
									
									if(g_QuestScriptManager.m_lpszQuestSystemMessage )
									{
										delete g_QuestScriptManager.m_lpszQuestSystemMessage;
										g_QuestScriptManager.m_lpszQuestSystemMessage = NULL;
									}
									
									if(strlength > 0)
									{
										g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
										g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
										strcpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer);
									}
									
									g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
									g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
								}		
							}				
						}
						else
						{
							_snprintf(buffer, sizeof(buffer), _T("%s : %d/%d"),
								currentquest->m_ItemCounterInformation[itemcounterindex].CounterTargetName, itemtotalcount);

							if( buffer[0] != 0 )
							{
								int strlength = strlen(buffer);
								
								if(g_QuestScriptManager.m_lpszQuestSystemMessage )
								{
									SAFE_DELETE(g_QuestScriptManager.m_lpszQuestSystemMessage);
								}
								
								if(strlength > 0)
								{
									g_QuestScriptManager.m_lpszQuestSystemMessage = new TCHAR[strlength + 1];
									g_QuestScriptManager.m_lpszQuestSystemMessage[strlength] = 0;
									strncpy(g_QuestScriptManager.m_lpszQuestSystemMessage, buffer, strlength);
								}
								
								g_QuestScriptManager.m_bStartQuestCounterMessage = TRUE;
								g_QuestScriptManager.m_QuestSystemMessageStartTime = g_LocalSystemTime;
							}		
						}
					}
					
					
					break;
				}
				
			}
		}
		if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
	}
	
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessStopCounter(void)// ī���� ���߱�
{
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	
	if(!currentquest)
		return QUEST_ACTION_RESULT_FAIL;

	
	currentquest->ResetCounterInformation();	
	currentquest->ResetPartyCounterInformation();
	currentquest->ResetItemCounterInformation();


	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSetSystemMessage(void)// �ý��۸޼������
{
	// send message
	_XQuestScriptItem* currentquest = NULL;
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
	}
	
	if(currentquest)
	{		
		if(m_lpszMessageString)
		{
			g_NetworkKernel.InsertChatString( m_lpszMessageString, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}		
	}	
	
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSetNPCOnOff(void)// NPC ���� & ����
{
	//	m_TDA.m_actiondata1		= m_indexTargetClass; 0:ON 1:OFF
	//	m_TDA.m_numericvalue1	= m_NPCID;

	for( int _i = 0; _i < g_Npclist.m_NPCObjectCount; ++_i )
	{			
		if( g_Npclist.m_pNPCObjectArray[_i].m_UniqueID == m_TDA.m_numericvalue1 )
		{
			if( g_Npclist.m_pNPCObjectArray[_i].m_NPCClass == _XNPC_CLASS_ONOFFTYPE )
			{
				if( m_TDA.m_actiondata1 == 0 ) // ON
				{
					g_Npclist.m_pNPCObjectArray[_i].m_bShowNPCModel = TRUE;
				}
				else // OFF
				{
					g_Npclist.m_pNPCObjectArray[_i].m_bShowNPCModel = FALSE;
				}
				break;
			}	
		}
	}
	
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSendQuestMail(void)// ǳ��� ������
{
	//	m_TDA.m_actiondata1 = m_index1; // ���� 0:Ȯ�� 1:����or�ź�
	//	m_TDA.m_actiondata2 = m_index2; // ��Ų 0:�Ϲ�ǳ��� 1:Ʃ�丮��ǳ���
	//	m_TDA.m_numericvalue1	= m_Seconds;  // �����ð� 
	
	if(!m_lpszMessageString)
		return QUEST_ACTION_RESULT_ERROR;
	
	if( m_TDA.m_numericvalue1 == 0 )
	{
		_XWindow_QuestMail* pQuestMail_Window= _XWindow_QuestMail::CreateQuestMail( &g_MainWindowManager, m_nOwnerQuestID, m_TDA.m_actiondata1 );
		if( pQuestMail_Window )
		{
			if( m_TDA.m_actiondata1 == 0 )
				pQuestMail_Window->SetMode( _XQUESTMAILMODE_OK );
			else
			{ //Author : ����� //breif: Ʃ�丮�� �� �� �ƴϿ� �� ������
				if( m_nOwnerTriggerID == 2 )
					pQuestMail_Window->SetMode( _XQUESTMAILMODE_TUTORIALONLY );
				else
					pQuestMail_Window->SetMode( _XQUESTMAILMODE_YESNO );
			}
			
			pQuestMail_Window->SetMessage( m_lpszMessageString );
		}
	}
	else
	{		
		_XQUESTMAIL_INFO questmailbuffer;
		memset(&questmailbuffer, 0, sizeof(_XQUESTMAIL_INFO));
		questmailbuffer.questid	= m_nOwnerQuestID;
		questmailbuffer.questmailtype = m_TDA.m_actiondata1+2;// ���� 0:Ȯ�� 1:����or�ź� (2�̻��� ���������� �ƴ� ǳ���) 2:Ȯ�� 3:����or�ź�
		questmailbuffer.resendstarttime = g_LocalSystemTime;
		questmailbuffer.delaytime = m_TDA.m_numericvalue1*1000;
		g_QuestScriptManager.m_listQuestMailBuffer.push_back(questmailbuffer);	
	}

	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessHoldItem(void)// ���������������Ұ�����
{
	//m_TDA.m_actiondata1  = m_ItemClass;	
	//m_TDA.m_actiondata2  = m_TargetClass;	//0:�����۰������� 1:�����۰�����
	//m_TDA.m_numericvalue1  = m_ItemID;
	
	if( m_TDA.m_actiondata2 == 0 )//0:�����۰�������
	{
		if( !g_QuestScriptManager.m_listHoldItem.empty() )
		{
			_XQUEST_HOLDITEM	questholditem;
			memset(&questholditem, 0, sizeof(_XQUEST_HOLDITEM));
			questholditem.m_cHoldItemType = m_TDA.m_actiondata1;
			questholditem.m_sHoldItemID = m_TDA.m_numericvalue1;
			g_QuestScriptManager.m_listHoldItem.push_back(questholditem);
		}
		else
		{
			// ���� ������ ���� ����Ʈ�� �����ϴ��� �˻�
			bool alreadyexist = false;
			list <_XQUEST_HOLDITEM>::iterator iter_holditem;
			for(iter_holditem = g_QuestScriptManager.m_listHoldItem.begin() ; iter_holditem != g_QuestScriptManager.m_listHoldItem.end() ; )
			{
				_XQUEST_HOLDITEM questholditem = *iter_holditem;
				if( questholditem.m_cHoldItemType == m_TDA.m_actiondata1 && questholditem.m_sHoldItemID == m_TDA.m_numericvalue1 )
				{
					alreadyexist = true;
					break;
				}
				else
				{
					iter_holditem++;
				}										
			}
			
			// �����ϰ� ���� �ʴٸ� ����Ʈ�� �߰�
			if( !alreadyexist )
			{
				_XQUEST_HOLDITEM	questholditem;
				memset(&questholditem, 0, sizeof(_XQUEST_HOLDITEM));
				questholditem.m_cHoldItemType = m_TDA.m_actiondata1;
				questholditem.m_sHoldItemID = m_TDA.m_numericvalue1;
				g_QuestScriptManager.m_listHoldItem.push_back(questholditem);
			}
		}
	}
	else//1:�����۰�����
	{
		if( !g_QuestScriptManager.m_listHoldItem.empty() )
		{
			list <_XQUEST_HOLDITEM>::iterator iter_holditem;
			for(iter_holditem = g_QuestScriptManager.m_listHoldItem.begin() ; iter_holditem != g_QuestScriptManager.m_listHoldItem.end() ; )
			{
				_XQUEST_HOLDITEM questholditem = *iter_holditem;
				if( questholditem.m_cHoldItemType == m_TDA.m_actiondata1 && questholditem.m_sHoldItemID == m_TDA.m_numericvalue1 )
				{
					iter_holditem = g_QuestScriptManager.m_listHoldItem.erase(iter_holditem);
					break;
				}
				else
				{
					iter_holditem++;
				}										
			}
		}
	}
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessPartyMessage(void)// ��Ƽ��_�޼������
{
	//m_TDA.m_actiondata1  = m_TargetClass2;	// 0:��� 1:��� 2:MVP 3:MVP�����	
	//m_TDA.m_actiondata2  = m_TargetClass;		// 0: �ý��۸޼��� 1:ȥ�㸻�ϱ�
	
	bool bsystemmessage = false;
	bool bshowmessage	= false;

	if( m_TDA.m_actiondata2 == 0 )
	{
		bsystemmessage = true;
	}

	switch( m_TDA.m_actiondata1 ) {
	case 0://��� 
		{
			if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
				bshowmessage = true;
		}
		break;
	case 1://���
		{
			if( !g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
				bshowmessage = true;
		}
		break;
	case 2://MVP
		{
			if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isMVP )
				bshowmessage = true;
		}
		break;
	case 3://MVP�� �����
		{
			if( !g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isMVP )
				bshowmessage = true;
		}
		break;		
	}
	
	if(m_lpszMessageString)
	{
		if( bshowmessage )
		{
			if( bsystemmessage )//�ý��� �޼����϶� 
			{
					g_NetworkKernel.InsertChatString( m_lpszMessageString, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			else//ȥ�㸻�϶�
			{
					g_pLocalUser->m_ChatBalloonObject.m_MessageColor = g_ChattingStringColor[7];//�ӼӸ� ����
					g_pLocalUser->m_ChatBalloonObject.SetMessage(m_lpszMessageString);		
			}
		}
	}
	
	
	return QUEST_ACTION_RESULT_SUCCESS;
}
_XQUESTACTION_RESULT _XTriggerActionItem::ProcessDisplayQuestRanking(void)// Ư������Ʈ��ŷ�����ֱ�
{
	//scriptdata->m_TDA.m_actiondata1		= m_TargetClass; // 0:���� 1:����
	//scriptdata->m_TDA.m_numericvalue1	= m_QuestID;
	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessStartPartyItemCounter(void)// ��Ƽ������ī���ͽ����ϱ�
{        
//	m_TDA.m_actionclass = ID_ACTION_STARTPARTYITMECOUNTER;
//	m_TDA.m_actiondata1  = m_indexCountType;                    // 0:��ƼAŸ�� 1:��ƼBŸ�� 2:��ƼCŸ�� 3:��ƼDŸ��
//	m_TDA.m_actiondata2  = m_indexItemClass;                    // ������ �з� 
//	m_TDA.m_numericvalue1  = m_ItemID;							// ������ ���̵�
//	m_TDA.m_numericvalue2  = m_Count;                           // �� ���� 
	
	_XQuestScriptItem* currentquest = NULL;
	
	if( m_bMainQuest ) //if( m_nOwnerQuestID >= 10000 && m_nOwnerQuestID < 20000  )
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}
	else
	{
		map <int, _XQuestScriptItem*>::iterator iter_quest = g_QuestScriptManager.m_mapSubQuestScriptList.find(m_nOwnerQuestID);
		if(iter_quest != g_QuestScriptManager.m_mapSubQuestScriptList.end())
			currentquest = iter_quest->second;
		else
			return QUEST_ACTION_RESULT_FAIL;
	}

	if( currentquest )
	{
		int countertype = m_TDA.m_actiondata1;

		// ī���� ����
		currentquest->m_PartyCounterInformation[countertype].CounterType		= countertype;
		currentquest->m_PartyCounterInformation[countertype].CounterTargetType	= m_TDA.m_actiondata2;
		currentquest->m_PartyCounterInformation[countertype].CounterTargetID	= m_TDA.m_numericvalue1;
		currentquest->m_PartyCounterInformation[countertype].CountTotalPoint	= m_TDA.m_numericvalue2;
		currentquest->m_PartyCounterInformation[countertype].CountCurrentPoint	= 0;

		if( m_lpszMessageString )
		{
			int strlength = strlen(m_lpszMessageString);
			
			if(strlength > 0)
			{
				memset( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256 );
				strcpy( currentquest->m_PartyCounterInformation[countertype].CounterTargetName, m_lpszMessageString );			
			}		
			else
			{			
				char cType = currentquest->m_PartyCounterInformation[countertype].CounterTargetType;
				short sID = currentquest->m_PartyCounterInformation[countertype].CounterTargetID;
				
				memset(currentquest->m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256);
				strcpy(currentquest->m_PartyCounterInformation[countertype].CounterTargetName, _XGameItem::GetItemName(cType, sID) );
			}	
		}
		else
		{
			char cType = currentquest->m_PartyCounterInformation[countertype].CounterTargetType;
			short sID = currentquest->m_PartyCounterInformation[countertype].CounterTargetID;
			
			memset(currentquest->m_PartyCounterInformation[countertype].CounterTargetName, 0, sizeof(TCHAR)*256);
			strcpy(currentquest->m_PartyCounterInformation[countertype].CounterTargetName, _XGameItem::GetItemName(cType, sID) );			
			
		}
		
		if( g_pQuest_Window->GetShowStatus() ) g_pQuest_Window->ParseQuestDescString();
	}
	

	return QUEST_ACTION_RESULT_SUCCESS;
}

_XQUESTACTION_RESULT _XTriggerActionItem::ProcessSetVirtualMode(void)// ������� �����ϱ�
{
	//scriptdata->m_TDA.nActionData1  = m_index1;		// �����������
	// �����������(�ٸ������ٸ���ġ), �����������(��������������ġ),�����������(���������ٸ���ġ),
	// �������������(��������������ġ), �������������(���������ٸ���ġ), �������������(��������������ġ),�������������(���������ٸ���ġ),
	// �������������(�ٸ������ٸ���ġ)
	//scriptdata->m_TDA.nActionData2  = m_index2;		// ����ī�װ�
	//scriptdata->m_TDA.nNumericValue1  = m_value1;		// ���ε���
	//scriptdata->m_TDA.nNumericValue2  = m_value2;		// �������̵�
	//scriptdata->m_TDA.nNumericValue3  = m_value3;		// ���� X
	//scriptdata->m_TDA.nNumericValue4  = m_value4;		// ���� Z

	switch( m_TDA.m_actiondata1 ) {
	case 0: // �����������(�ٸ������ٸ���ġ) �ʷε�
		{
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_ENTER_OTHERZONE;
			g_QuestScriptManager.m_indexVirtualZone = m_TDA.m_numericvalue1;
			g_QuestScriptManager.m_nVirtualZoneStartPositionX = m_TDA.m_numericvalue3;
			g_QuestScriptManager.m_nVirtualZoneStartPositionZ = m_TDA.m_numericvalue4;
			//����������� �������� �ִ� �����ε����� �����Ѵ�.
			g_QuestScriptManager.m_indexRealZone = g_CurrentZoneInfoIndex;
			//����������� ���� ���� �ִ���ġ�� �����Ѵ�.
			g_QuestScriptManager.m_nRealZoneStartPositionX = g_pLocalUser->m_Position.x;
			g_QuestScriptManager.m_nRealZoneStartPositionZ = g_pLocalUser->m_Position.z;
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_enter_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 1:// �����������(��������������ġ)
		{
			// ������ ���� �������� ��������� �ǵ��� �����Ѵ�.
			// ������������� �ٸ� ������ ������ ���� �������� ���� �ʴ´�.			
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_ENTER_CURRENTZONE;
			g_Moblist.disposeList();
			g_Userlist.disposeList();
			g_GameItemlist.disposeList();
			
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_enter_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 2:// �����������(���������ٸ���ġ)
		{
			// ������ ���� �������� ��������� �ǵ��� �����Ѵ�.
			// ������������� �ٸ� ������ ������ ���� �������� ���� �ʴ´�.			
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_ENTER_CURRENTZONEOTHERPOSITION;
			
			_XArea* pArea = g_AreaManager.GetArea( m_TDA.m_actiondata2, m_TDA.m_numericvalue2 );
			if( pArea )
			{				
				g_QuestScriptManager.m_nVirtualZoneStartPositionX = ( pArea->m_Point[0].x + pArea->m_Point[2].x )/2;
				g_QuestScriptManager.m_nVirtualZoneStartPositionZ = ( pArea->m_Point[0].z + pArea->m_Point[2].z )/2;
			}

			//����������� ���� ���� �ִ���ġ�� �����Ѵ�.
			g_QuestScriptManager.m_nRealZoneStartPositionX = g_pLocalUser->m_Position.x;
			g_QuestScriptManager.m_nRealZoneStartPositionZ = g_pLocalUser->m_Position.z;

			g_Moblist.disposeList();
			g_Userlist.disposeList();
			g_GameItemlist.disposeList();
			
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_enter_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 3:// �������������(��������������ġ) �ʷε�
		{
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_LEAVE_COMEBACKZONE;
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_leave_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 4:// �������������(���������ٸ���ġ) �ʷε�
		{
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_LEAVE_COMEBACKZONEOTHERPOSITION;
			g_QuestScriptManager.m_nRealZoneStartPositionX = m_TDA.m_numericvalue3;
			g_QuestScriptManager.m_nRealZoneStartPositionZ = m_TDA.m_numericvalue4;

			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_leave_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 5:// �������������(��������������ġ)
		{
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_LEAVE_CURRENTZONE;

			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_leave_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 6:// �������������(���������ٸ���ġ)
		{
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_LEAVE_CURRENTZONEOTHERPOSITION;
			_XArea* pArea = g_AreaManager.GetArea( m_TDA.m_actiondata2, m_TDA.m_numericvalue2 );
			if( pArea )
			{				
				g_QuestScriptManager.m_nRealZoneStartPositionX = ( pArea->m_Point[0].x + pArea->m_Point[2].x )/2;
				g_QuestScriptManager.m_nRealZoneStartPositionZ = ( pArea->m_Point[0].z + pArea->m_Point[2].z )/2;
			}

			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_leave_tutorial_req);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	case 7:// �������������(�ٸ������ٸ���ġ)
		{
			g_QuestScriptManager.m_VirtualMode = _XQUEST_VIRTUALMODE_LEAVE_OTHERZONEOTHERPOSITION;
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_leave_tutorial_req,m_TDA.m_numericvalue1,m_TDA.m_actiondata2,m_TDA.m_numericvalue2);
			return QUEST_ACTION_RESULT_SUCCESS;
		}
		break;
	}
	return QUEST_ACTION_RESULT_FAIL;
}
//////////////////////////////////////////////////////////////////////
// XLocalUser.h
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XMob.h"
#include "XUser.h"
#include "SoundEffectList.h"
#include "XProc_MainGameDef.h"
#include "XWindow_Skill.h"
#include "XPathFinder.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_ChargeGauge.h"
#include "XWindow_Warehouse.h"
#include "XWindow_JinStatus.h"
#include "XSR_STRINGHEADER.H"
#include "XParticleIDDefine.h"
#include "XWindow_Rebirth.h"
#include "XMessageWindow.h"
#include "XWindowObjectDefine.h"
#include "XWindow_MonsterStatus.h"
#include "XWindow_TrainingMed.h"
#include "XWindow_TrainingDummy.h"
#include "XWindow_TrainingSpell.h"
#include "Xwindow_TrainingComplete.h"
#include "xwindow_repairitem.h"
#include "XWindow_TutorialIcon.h"
#include "XWindow_MainMenu.h"
#include "Xwindow_MatchNoticeLarge.h"
#include "Xwindow_NickName.h"
#include "XWindow_Escape.h"

#include "XWindow_PersonalStore.h"
#include "XWindow_MnDGroup.h" // Author : ����� //breif : �������� ��

#include "XParticleAction.h"

// by mahwang
#include "XLifeEventManager.h"

//#define _XDEF_RENDERNUMBEREFFECT
FLOAT				g_fWaterDepth = 0.28f; // ���� ����ͱ��� ���� �ִ�.
_XMOTION_MOVETYPE	g_PrevMoveType = _XMOVETYPE_RUN;
_XCHARACTER_MOTION	g_EmotionActionTable[16] = 
{
	_XAN_EMOTION_BOW,						// 60 ������ �λ�
	_XAN_EMOTION_BOWPOLITELY,				// 61 ������ �λ�	
	_XAN_EMOTION_EXCITEMENT,				// 62 ����
	_XAN_EMOTION_RUSH,						// 63 ����
		
	_XAN_EMOTION_CRINGESTART,				// 64 �����ݱ� ����
		
	_XAN_EMOTION_CHEER,						// 67 ȯȣ
	_XAN_EMOTION_BREAKDOWN,					// 68 ����
	_XAN_EMOTION_CRY,						// 69 ���
	_XAN_EMOTION_LAUGH,						// 70 ����	
	_XAN_EMOTION_ANGRY,						// 71 ȭ��
	_XAN_EMOTION_DOZ,						// 72 ����

	// Emotion���� ���� - ������� ���ÿ��� ���
//	_XAN_EMOTION_SITDOWNSTART,				// 73 �ɱ� ����
	_XAN_EMOTION_MARRYCLAPPINGHAND,			// 76 �ڼ�	
	_XAN_EMOTION_MARRYBOX,					// 77 �λ�
	_XAN_EMOTION_MARRYBOWPOLITELY,			// 78 ū��	
	
	_XAN_EMOTION_CLAPPINGHAND,				// 79 �ڼ�
};

extern void ChangeModel(DWORD lparam, char firstType, char facemaskmode = 0 );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
_XLocalUser::_XLocalUser()
{
#if defined(_XDEF_ITEMMALL_AUTOLOGIN_20061120) || defined(_XDEF_ITEMMALL_AUTOLOGIN_SE_20070514)
	memset( m_ItemMallWebLoginID, 0, sizeof(TCHAR) * 33 );
#endif
	memset( m_UserName, 0, sizeof(TCHAR) * 33 );
	memset( m_UserPW, 0, sizeof(TCHAR) * 33 );

	m_PickItem = NULL;
	m_LastInsertedMoveTargetEffectPosition = D3DXVECTOR3( 9999.0f, 9999.0f, 9999.0f );
	m_NextRespawnPosition.x =   0;
	m_NextRespawnPosition.y =   0;
	m_NextRespawnPosition.z =   0;
	ResetLocalUserData();
}

_XLocalUser::~_XLocalUser()
{	
	//2004.06.18->oneway48 insert
	m_MiniHpBorder.DestroyWindow();
	m_MonsterMiniHpBorder.DestroyWindow();
	m_imageMiniHpBackBorder.DestroyWindow();
}

//#define _XDEF_CHECKGROUNDNORMALVECTOR

void _XLocalUser::UpdatePosition( void )
{	
	D3DXMATRIX	rotmat;
	//D3DXMatrixIdentity( &rotmat );
	D3DXMatrixRotationY( &rotmat, m_RotateAngle );
	
	/*if( m_pGoundCollideOBB )
	{
		m_Position.y = m_pGoundCollideOBB->m_kCenter.y + m_pGoundCollideOBB->m_afExtent[1];
		FLOAT terrainheight = g_LodTerrain.GetTerrainHeight( m_Position ); 
		if( terrainheight > m_Position.y ) m_Position.y = terrainheight;
		m_PrevPosition.y = m_Position.y;
	}
	else 
	{
		m_Position.y = g_LodTerrain.GetTerrainHeight( m_Position );
		m_PrevPosition.y = m_Position.y;
	}	*/

	D3DXMatrixTranslation( &m_ModelDescriptor.m_Position, m_Position.x, m_Position.y, m_Position.z );
	
#ifdef _XDEF_CHECKGROUNDNORMALVECTOR
	
	if( m_CharacterInfo.animationdescript.motionclass == _XACTION_MOVE )
	{
		D3DXMATRIX  rotmatrix;
		D3DXQUATERNION normalrotquat;
		D3DXVECTOR3	upvector = D3DXVECTOR3(0.0f,1.0f,0.0f);
		
		if( m_CurrentPositionTerrainNormal.x != 0.0f || m_CurrentPositionTerrainNormal.y != 1.0f || m_CurrentPositionTerrainNormal.z != 0.0f ) // check exactly upvector...
		{			
#ifdef _XDWDEBUG			

			if( g_SpiderMode )
			{
				normalrotquat = _XRotationArcNormalized( &upvector, &m_CurrentPositionTerrainNormal  );
			}
			else
			{
#endif	
				// rotate from terrain normal vector...					
				FLOAT theta = _XGetAngleFromVectors( &upvector, &m_CurrentPositionTerrainNormal );
				
				if( theta > 0.122173022f/* deg 7' */ )
				{
					FLOAT dev = theta / 0.122173022f; //0.0872664444f(5');
					
					D3DXVECTOR3 modvector = m_CurrentPositionTerrainNormal;
					
					modvector.x /= dev;
					modvector.z /= dev;
					
					normalrotquat = _XRotationArcNormalized( &upvector, &modvector );
				}
				else
				{
					normalrotquat = _XRotationArcNormalized( &upvector, &m_CurrentPositionTerrainNormal  );
				}			

#ifdef _XDWDEBUG	
			}
#endif
			D3DXMatrixRotationQuaternion( &rotmatrix, &normalrotquat );
			D3DXMatrixMultiply( &m_ModelDescriptor.m_Position, &rotmatrix, &m_ModelDescriptor.m_Position );
		}				
	}
	
#endif
	
	D3DXMatrixMultiply( &m_ModelDescriptor.m_Position, &rotmat, &m_ModelDescriptor.m_Position );

	g_UserWorldMatrix = m_ModelDescriptor.m_Position;
	
}

void _XLocalUser::UpdateRotateAngle(void)
{
	if((m_LastRotateAngle - m_RotateAngle) <= EPSILON1 && (m_LastRotateAngle - m_RotateAngle) >= -EPSILON1)
	{
		m_RotateAngle = m_LastRotateAngle;
		return;
	}

	FLOAT deg_LastRotateAngle = _X_DEG(m_LastRotateAngle);
	FLOAT deg_RotateAngle = _X_DEG(m_RotateAngle);

	FLOAT addunit = _X_RAD(30);
	if(fabs(deg_LastRotateAngle - deg_RotateAngle) <= 30.0f)
		addunit = _X_RAD(5);
	/*
	FLOAT addunit = _X_RAD(10);
	if(fabs(deg_LastRotateAngle - deg_RotateAngle) <= 10.0f)
		addunit = _X_RAD(2);
		*/

	if(m_RotateDirection == _XROTATE_CLOCKWISE)
	{
		m_RotateAngle += addunit;
		if(m_RotateAngle >= _X_PI)
			m_RotateAngle = -_X_PI;
	}
	else
	{
		m_RotateAngle -= addunit;
		if(m_RotateAngle <= -_X_PI)
			m_RotateAngle = _X_PI;
	}
}

// �׾����� �������� �̵� 
void _XLocalUser::MoveStartPosition( void )
{	
	m_PrevProcessTime			=	-1.0f;
	ResetAttackTarget();
	ResetViewTarget();

	m_TargetPosition			=   m_Position;

	if(g_CurrentZoneInfoIndex == 18 || g_CurrentZoneInfoIndex == 19)
		m_RotateAngle			= _X_RAD(-90);
	else
		m_RotateAngle			=	0.0f;
			
	m_StopAnimation				=	FALSE;
}

void _XLocalUser::ResetLocalUserData( void )
{
	// ó���� �� �� reset�Ǵ� data
	m_iNetworkKey				=	-1;	// hotblood 20040824
	m_sMiniGameId				=	0;	// �̴ϰ��� ���� hotblood 20040824
	m_CharacterCount			=	0;
	m_PrevPacketSendedTime		=	0;
	m_PickItem					=	NULL;

	m_SkillCombo				=	0;
	m_SkillStarLevel			=	11;
	m_MartialStep				=	0;
	m_SelectedSkillID			=	0;
	m_ActiveSlotID				=	0;

	//Author : �����
#ifdef _XDEF_SKILLDEALY_MODIFY
	m_FinallySelectedSkillID	=	0;
	m_FinallySelectedSkillItem	=	NULL;
#endif
//Last Updated : 07/03/22

	m_SelectedSkillItem			=	NULL;	
	m_SelectedWeaponItem		=   NULL;

	m_NextSkillID				=	0;
	m_bUsingSkill				=	FALSE;
	m_bCheckDelayTime			=	FALSE;

	m_bRestartActive			=	FALSE;
	m_bStartCharge				=	FALSE;
	m_bUseMouseChargeSkill		=	FALSE;
	
	m_ExperiencePercentage		=	0;
	LifePowerPercentage			=	0;	
	ForcePowerPercentage		=	0;
	ConcentrationPowerPercentage=	0;
				
	m_SpecialActionTimer		=   0;
	
	m_PrevProcessTime			=	-1.0f;
	m_PrevMoveTime				=    0;
		
	m_TargetPosition			=   m_Position;
	GetItemTileIndex(m_Position.x, m_Position.z, m_CurrentTileIndex);
	m_LastItemRequestSendTime	= 0;
	
	m_RotateAngle				=	0.0f;	
	m_FinalTargetPosition		=	D3DXVECTOR3( 0.0f,0.0f,0.0f );
	m_LeftFinalTargetLength		=   0.0f;
		
	m_StopAnimation				=	FALSE;
		
	for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i) // 2004.06.01->oneway48 modify : 15->9
	{
		memset(m_PartyInfo[i].playername, 0, sizeof(TCHAR)*13);
		m_PartyInfo[i].serverno = m_PartyInfo[i].posX = m_PartyInfo[i].posZ = 0;
		m_PartyInfo[i].innerlevel = m_PartyInfo[i].vitalpercent = m_PartyInfo[i].zenpercent = 0;
		m_PartyInfo[i].isLeader = FALSE;
	}
	
	m_TotalCountOfPartyQuestCounter = 0	;			// ��Ƽ����Ʈ���� ��Ƽ ī������ ������ ��Ÿ�� 
	m_PartySelfIndex				= -1;			// ��Ƽ ����Ʈ �迭���� �ڱ� �ڽ��� ��ġ�� ��Ÿ���� �ε���
	m_PartyMemberCount				= 0	;
	m_PartyMemberIndex				= -1;
	m_InParty						= FALSE;
	m_bPartyQuestProgress			= FALSE;
	m_bPartyProgress				= FALSE;
	m_IsServerMoveInParty			= FALSE;

	ResetInventoryInfo();
	ResetSkillList();
	ResetWarehouseInfo();

	m_ChargeStartTime				= 0;
	m_ChargeIterateValue			= 0;
	m_ChargeSendStartPacket			= FALSE;

	m_RoundingDelayStartTime		= 0;
	m_RoundingSendPacket			= FALSE;

	m_bChannelingSendStartPacket	= FALSE;
	m_ChannelingSendLastPacketTime	= 0;
	m_ChannelingSpellRepeatTime		= 0;
	m_RegionTargetPosX				= 0.0f;
	m_RegionTargetPosZ				= 0.0f;

	m_ChangeDelayStartTime			= 0;

	m_CurrentSelectedSkillType		= 0;
	m_CurrentBattleState			= _XUSER_STATE_NONE;
	m_StanceMotionFirst				= FALSE;
	m_LastModeChangeTime			= 0;

	m_FinishSpeed					= 0;
	m_FinishSendPacket				= FALSE;
	m_bSendFinishFirePacket			= FALSE;

	m_CastingSpellRepeatTime		= 0;
	m_CastingSendPacket				= FALSE;

	m_bLeftClickProcess				= FALSE;
	m_bRightClickProcess			= FALSE;
	
	ResetAttackTarget();
	ResetViewTarget();
	ResetSpellAttackTarget();

	m_LastRotateAngle				= 0.0f;
	m_RotateDirection				= _XROTATE_NONE;
	
	m_UseSpeedSkill					= FALSE;
	m_AttackPointNumber				= 0;
	m_CurrentAttackPoint			= 0;

	m_DyingStartTime				= 0;
	m_LastCheckedDyingStartTime		= -1;
	m_DyingTimer					= 0;

	m_bCheckFirstFrame				= FALSE;
	m_bWaitPassiveReq				= FALSE;
	
	m_LastPassiveUsefulType			= 0;
	m_LastPassiveSType				= 0;
	m_LastPassiveOnOffTime			= 0;

	memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
	memset(&m_RegionDamageResult, 0, sizeof(m_RegionDamageResult));

	m_PathNodeCount					= 0;
	m_CurrentNavigatePathNodeIndex	= 0;
	
	m_DisarmamentItem = NULL;
	m_CurrentWeaponID				= 0;

	m_SelectedTrainingMode			= _XTRAINING_NONE;
	m_CurrentTrainingState			= _XUSER_STATE_NONE;
	m_CurrentInternalUseState		= _XUSER_STATE_NONE;
	m_CurrentReadBookState			= _XUSER_STATE_NONE;
	m_bSendTrainingStartPacket		= FALSE;
	m_ServerKey						= 0;
	memset(&m_TrainingDummyInfo, 0, sizeof(_XDUMMY_INFO));
	m_TrainingSpellInfo.Reset();

	
	if(g_pJinStatus_Window)
		g_pJinStatus_Window->SetButtonStatus(FALSE);

	m_bSaveRespawnPos				= FALSE;
	memset(m_RespawnName, 0, sizeof(TCHAR)*13);
	
	m_BossRoomID	= -1;
	m_BossRoomStageNumber = 0;
	m_bIsBossBattle = FALSE;
	m_bBossBattleEndProcess = FALSE;
	m_bSuccessBossBattle	= FALSE;
	m_bExitBossBattleRoom = FALSE;
	m_bWaitBossBattle = FALSE;
	m_BossStartTime	= 0;
	m_BossTimeLimit	= 0;
	m_bVirtualMode = FALSE;

	// OLD
	m_MatchType = 0;
	m_MatchID = 0;
	m_MatchTargetID = 0;
	m_bWaitFlagInfo = FALSE;

	m_ChatMuteTime = 0;
	m_LastChatLockNoticeMinute = 0;
	
	// NEW
	ResetMatchState();
	m_MatchPRTimer		= 0; // �� ȫ�� ��Ŷ�� 5�ʸ��� ������.
	
	m_bGrabItem = FALSE;
	m_StateList.Release();
	memset(&m_Lock, 0, sizeof(m_Lock));
	Set_m_MoveSpeedFromCondition(0);
	m_PvpNPCLock = FALSE;

	memset(&m_P_AttackDamage, 0, sizeof(_XSU_Parameter));
	memset(&m_P_AttackRate, 0, sizeof(_XSU_Parameter));
	memset(&m_P_Defence, 0, sizeof(_XSU_Parameter));
	memset(&m_P_AvoidRate, 0, sizeof(_XSU_Parameter));
	memset(&m_M_AttackDamage, 0, sizeof(_XSU_Parameter));
	memset(&m_M_AttackRate, 0, sizeof(_XSU_Parameter));
	memset(&m_M_Defence, 0, sizeof(_XSU_Parameter));	
	memset(&m_M_AvoidRate, 0, sizeof(_XSU_Parameter));

	memset(&m_Strength, 0, sizeof(_XSU_Parameter));
	memset(&m_Zen, 0, sizeof(_XSU_Parameter));
	memset(&m_Intelligence, 0, sizeof(_XSU_Parameter));
	memset(&m_Dex, 0, sizeof(_XSU_Parameter));
	memset(&m_Constitution, 0, sizeof(_XSU_Parameter));
	
	memset(&m_MaxLifePower, 0, sizeof(_XSU_Parameter));
	memset(&m_MaxForcePower, 0, sizeof(_XSU_Parameter));
	memset(&m_MaxConcentrationPower, 0, sizeof(_XSU_Parameter));

	m_P_MinAttackDamage = 0;
	m_P_MaxAttackDamage = 0;
	m_M_MinAttackDamage = 0;
	m_M_MaxAttackDamage = 0;

	Set_m_MoveSpeedFromEffect(0);
	Set_m_AttackSpeedFromEffect(0);
	
	m_ResistMinus		= 0;
	m_ResistPlus		= 0;
	m_ResistHon			= 0;
	m_ResistSin			= 0;
	
	m_AttackDamageMinus	= 0;
	m_AttackDamagePlus	= 0;
	m_AttackDamageHon	= 0;
	m_AttackDamageSin	= 0;
	
	m_bKeyLPSProcess	= FALSE;
	m_HiperRunFailedFlag = FALSE;
	
	m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
	
	m_bWaitDyingSkip		= FALSE;
	m_bHaveStance			= FALSE;
	reserved				= FALSE;

	m_PrevRequestPacketSendedTime = 0;
	
	m_PKModeLeftTime		= 0;
	m_PKModeStartTime		= 0;

	m_PEModeLeftTime		= 0;
	m_PEModeStartTime		= 0;
	
#ifdef _XTS_MASKSYSTEM
	m_bFaceMaskMode			= FALSE;
#endif
	
#ifdef _XTS_BLOODPOINT
	m_BloodPoint			= 0;
#endif
	m_FameAttrib			= -1;	
	m_bPEMode				= FALSE;

	m_UseItemInvenNumber	= 0;
	
#ifdef _XDEF_CASHITEM_SHOUT
	ResetShout();
#endif

	m_AccountItemcType		= 0;
	m_AccountItemsID		= 0;

	ResetExpItemEndTime();
	
	m_InsideSafetyZoneIndex	= 0;
	m_SafetyZoneCheckTimer	= 0;
	
	m_bCastleBattle			= false;
	m_bSendThreatenPacket	= false;
	m_bThreatening			= false;

	m_nCastleMapId			= 0;
	m_dwRoleInCastleBattle	= 0;
	m_dwAttackType			= 0;
	m_dwResultCastleBattle	= 0;
	m_dwDeadCount			= 0;
	m_dwDieStartTime		= 0;
	m_dwDeadPenaltyTime		= 0;

	m_bOut					= FALSE;

	m_bGuardMob = false; //Author : ����� //breif : ��Ʈ�� shift Ŭ��

	m_nNotoriousValue = 0; //Author : ����� //breif : ���� �ý��� 

	m_bChangingWeapon = FALSE; //Author : ����� //breif : ���⸦ ��ü ������ �Ǵ�
	m_bChangingWeaponPacket = FALSE;
	m_nChangeWeaponModel = 0;

	memset( &m_dwPCroomInfo, 0, sizeof(PCROOM_App) );	//Author : ����� //breif : PC ��

	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK����ġ����, ������ PK����
	_XPKTRACING_INFO* pktracing = NULL;
	list <_XPKTRACING_INFO*>::iterator iter_pktracing;
	for(iter_pktracing = m_listPKTracing.begin() ; iter_pktracing != m_listPKTracing.end() ; ++iter_pktracing)
	{
		pktracing = *iter_pktracing;
		SAFE_DELETE( pktracing );
	}
	m_listPKTracing.clear();
#endif
	
	m_bEnterMatchEvent		= FALSE;
	m_nWinnerNickName		= 0;
	m_nKillCount			= 0;
	m_nDeadCount			= 0;
	
	m_dwBWLeftTimeFromServer	= 0;
	m_dwBWReceiveTimeFromServer = 0;
	
	m_bSendFunctionalMovePacket = FALSE;
	m_nFuctionalNormalAction = 0;

	m_nLocalMnDClass = _XDEF_DESCIPLE;//_XDEF_NONE; //Author : ����� //breif : �ڽ��� �������� ��������

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
	memset(&m_pLocalMnDInfomation, 0, sizeof(m_pLocalMnDInfomation)); //Author : ����� //breif : ���� �ʱ�ȭ
#endif

}

void _XLocalUser::ResetInventoryInfo( void )
{
	m_Money = 0;			
	m_cAmorPocketNum = 0;
	m_cPantsPocketNum = 0;
	m_cPocketNum = 0;		// ���� ����
	m_cBackPackExtend = 0;	
	m_nCashBackPackMaxValue = 0;				//Author : ����� //breif : ĳ�� �೶ �ִ밪

	for( int i = 0; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; ++i )
	{
		m_UserItemList[i].Reset();
	}
}

void _XLocalUser::ResetWarehouseInfo( void )
{
	for( int i = 0; i < _XDEF_WAREHOUSE_TOTALCOUNT; ++i )
	{
		m_WarehouseItemList[i].Reset();
	}
}

void _XLocalUser::ResetSkillList(void)
{
	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
	{
		m_SkillList[i].m_sSkillID = 0;
		m_SkillList[i].m_cSkillLevel = -1;

		m_SkillList[i].m_cCurMission = 0;
		m_SkillList[i].m_iSkillExp = 0;

		m_SkillList[i].m_dwCooldownStartTime = 0;
		m_SkillList[i].m_ucSkillState = 0;
	}
}

void _XLocalUser::ResetLocalUserTempData(void)
{	
	// ���� �ѳ��� �� reset �Ǵ� data

//	m_CharacterCount			=	0;
	m_PrevPacketSendedTime		=	0;
	m_PickItem					=	NULL;
	
	m_ExperiencePercentage		=	0;
	LifePowerPercentage			=	0;	
	ForcePowerPercentage		=	0;
	ConcentrationPowerPercentage=	0;
			
	m_SpecialActionTimer		=   0;
	
	m_PrevProcessTime			=	-1.0f;
	m_PrevMoveTime				=    0;
		
	m_TargetPosition			=	m_Position;
	GetItemTileIndex(m_Position.x,	m_Position.z, m_CurrentTileIndex);
	m_LastItemRequestSendTime	=	0;
	
	m_RotateAngle				=	0.0f;	
	m_FinalTargetPosition		=	D3DXVECTOR3( 0.0f,0.0f,0.0f );
	m_LeftFinalTargetLength		=   m_LeftFinalTargetLength;
		
	m_StopAnimation				=	FALSE;
	
	if( ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MovingGameServer == FALSE )
	{	
		for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i) 
		{
			memset(m_PartyInfo[i].playername, 0, sizeof(TCHAR)*13);
			m_PartyInfo[i].serverno = m_PartyInfo[i].posX = m_PartyInfo[i].posZ = 0;
			m_PartyInfo[i].innerlevel = m_PartyInfo[i].vitalpercent = m_PartyInfo[i].zenpercent = 0;
			m_PartyInfo[i].isLeader = FALSE;
		}
		
		m_PartyMemberCount		= 0;
		m_PartyMemberIndex		= -1;
		m_PartySelfIndex		= -1;
		m_InParty				= FALSE;
		m_bPartyQuestProgress	= FALSE;
	}

	memset( &m_CharacterLog, 0, sizeof(_XUSERACTIONLOG) );
	_XDWINPRINT("[LOCALUSER/ResetLocalUserTempData] All log clear");

	m_bReadingBook					= FALSE;		// 2005.1.17
	m_bInternalUse					= FALSE;	
	m_bSocketPlugIn					= FALSE;
	m_bGambleBoxOpen				= FALSE;
	m_bSocketPlugInLock				= FALSE;
	m_bGambleBoxOpenLock			= FALSE;
	m_bTargetInfoLock				= FALSE;
	m_bSocketPlugInEXLock			= FALSE;

	m_ChargeStartTime				= 0;
	m_ChargeIterateValue			= 0;
	m_ChargeSendStartPacket			= FALSE;

	m_RoundingDelayStartTime		= 0;
	m_RoundingSendPacket			= FALSE;

	m_bChannelingSendStartPacket	= FALSE;
	m_ChannelingSendLastPacketTime	= 0;
	m_ChannelingSpellRepeatTime		= 0;
	m_RegionTargetPosX				= 0.0f;
	m_RegionTargetPosZ				= 0.0f;

	m_ChangeDelayStartTime			= 0;
	m_CurrentSelectedSkillType		= 0;
	m_CurrentBattleState			= _XUSER_STATE_NONE;
	m_StanceMotionFirst				= FALSE;
	m_LastModeChangeTime			= 0;

	m_FinishSpeed					= 0;
	m_FinishSendPacket				= FALSE;
	m_bSendFinishFirePacket			= FALSE;

	m_CastingSpellRepeatTime		= 0;
	m_CastingSendPacket				= FALSE;

	m_bLeftClickProcess				= FALSE;
	m_bRightClickProcess			= FALSE;

	m_bUsingSkill					= FALSE;
	m_bCheckDelayTime				= FALSE;

	m_bRestartActive				= FALSE;
	m_bStartCharge					= FALSE;
	m_bUseMouseChargeSkill			= FALSE;

	m_LastRotateAngle				= 0.0f;
	m_RotateDirection				= _XROTATE_NONE;
	
	m_UseSpeedSkill					= FALSE;
	m_AttackPointNumber				= 0;
	m_CurrentAttackPoint			= 0;

	m_DyingStartTime				= 0;
	m_LastCheckedDyingStartTime		= -1;
	m_DyingTimer					= 0;

	m_bWaitPassiveReq				= FALSE;

	m_LastPassiveUsefulType			= 0;
	m_LastPassiveSType				= 0;
	m_LastPassiveOnOffTime			= 0;

	m_bCheckFirstFrame				= FALSE;
	m_CurrentTrainingState			= _XUSER_STATE_NONE;
	m_CurrentInternalUseState		= _XUSER_STATE_NONE;
	m_CurrentReadBookState			= _XUSER_STATE_NONE;
	m_bSendTrainingStartPacket		= FALSE;
	memset(&m_TrainingDummyInfo, 0, sizeof(_XDUMMY_INFO));
	m_TrainingSpellInfo.Reset();
	
	ResetAttackTarget();
	ResetViewTarget();
	ResetSpellAttackTarget();
	
	if(g_pQuickSlot_Window)
	{
		if( g_MainWindowManager.GetWindowCount() )
		{
			g_pQuickSlot_Window->ChangePeaceBattleMode();
		}
		
	}

	memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
	memset(&m_RegionDamageResult, 0, sizeof(m_RegionDamageResult));

	m_PathNodeCount					=	0;
	m_CurrentNavigatePathNodeIndex	=	0;

	m_DisarmamentItem				= NULL;
	m_CurrentWeaponID				= 0;

	// OLD
	m_MatchType = 0;
	m_MatchID = 0;
	m_MatchTargetID = 0;
	m_bWaitFlagInfo = FALSE;

	// NEW
	ResetMatchState();
	m_MatchPRTimer		= 0; // �� ȫ�� ��Ŷ�� 5�ʸ��� ������.

	m_bGrabItem = FALSE;

	if( !m_bIsBossBattle && !m_bBossBattleEndProcess)//�������̸� ����ȿ���� �ٽ� ���� ����.->����
	{
		// ����ȿ�� ��� ���� - �ٽ� ����
		m_StateList.DeleteAll();
		memset(&m_Lock, 0, sizeof(m_Lock));
	}
	Set_m_MoveSpeedFromCondition(0);
	m_PvpNPCLock = FALSE;

	Set_m_MoveSpeedFromEffect(0);
	Set_m_AttackSpeedFromEffect(0);

	m_HiperRunFailedFlag = FALSE;
	m_bWaitDyingSkip		= FALSE;
	
	// ����Ʈ ����Ÿ�� �ٽ� �����Ƿ� reset
	m_bViewMasterySkill		= FALSE;

	m_bHaveStance			= FALSE;
	reserved				= FALSE;

	m_bPEMode				= FALSE;
	m_PKModeLeftTime		= 0;
	m_PKModeStartTime		= 0;

#ifdef _XDEF_CASHITEM_SHOUT
	ResetShout();
#endif

	// �� �Ѿ�� ���� ���� üũ Ÿ�̸� �ʱ�ȭ
	m_InsideSafetyZoneIndex	= 0;
	m_SafetyZoneCheckTimer	= 0;

	m_bOut					= FALSE;
	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK����ġ����, ������ PK����
	_XPKTRACING_INFO* pktracing = NULL;
	list <_XPKTRACING_INFO*>::iterator iter_pktracing;
	for(iter_pktracing = m_listPKTracing.begin() ; iter_pktracing != m_listPKTracing.end() ; ++iter_pktracing)
	{
		pktracing = *iter_pktracing;
		SAFE_DELETE( pktracing );
	}
	m_listPKTracing.clear();
#endif

	m_bEnterMatchEvent		= FALSE;
	m_nWinnerNickName		= 0;
	m_nKillCount			= 0;
	m_nDeadCount			= 0;

	m_dwBWLeftTimeFromServer	= 0;
	m_dwBWReceiveTimeFromServer = 0;

	m_bSendFunctionalMovePacket = FALSE;
	m_nFuctionalNormalAction = 0;
}

void _XLocalUser::ResetCastleBattleInfo()
{
	m_nCastleMapId			= 0;

	m_bCastleBattle			= false;
	m_bSendThreatenPacket	= false;
	m_bThreatening			= false;

	m_dwRoleInCastleBattle	= 0;
	m_dwAttackType			= 0;
	m_dwResultCastleBattle	= 0;
	m_dwDeadCount			= 0;
	m_dwDieStartTime		= 0;
	m_dwDeadPenaltyTime		= 0;
	m_nCBMapIdInWar			= 0;
	m_nDefenderType			= 0;
	m_nDefenderIndex		= 0;
	memset(m_nAttackerType, 0, sizeof(m_nAttackerType));
	memset(m_nAttackerIndex, 0, sizeof(m_nAttackerIndex));
}


_XUserItem*	_XLocalUser::GetUserItem( int firstcategory, int id )
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
    for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if( m_UserItemList[i].Get_m_cType() == firstcategory && 
			m_UserItemList[i].Get_m_sID() == id )
		{
			return &m_UserItemList[i];
		}															
	}
				
    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		if( m_UserItemList[i].Get_m_cType() == firstcategory && 
			m_UserItemList[i].Get_m_sID() == id )
		{
			return &m_UserItemList[i];
		}		
	}
#else
	for( int i = 0; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; ++i )
	{
		if( m_UserItemList[i].Get_m_cType() == firstcategory && 
			m_UserItemList[i].Get_m_sID() == id )
		{
			return &m_UserItemList[i];
		}		
	}
#endif
	
	return NULL;
}

_XUserItem* _XLocalUser::GetInventoryEmptySlot( void )
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
    for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if( m_UserItemList[i].Get_m_cType() == -1 )
		{
			return &m_UserItemList[i];
		}					
	}
				
    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		if( m_UserItemList[i].Get_m_cType() == -1 )
		{
			return &m_UserItemList[i];
		}		
	}
#else
	for( int i = 0; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; ++i )
	{
		if( m_UserItemList[i].Get_m_cType() == -1 )
		{
			return &m_UserItemList[i];
		}		
	}
#endif
	
	return NULL;
}

int _XLocalUser::GetUserItemInvenSlot(int firstcategory, int id)
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
    for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if( m_UserItemList[i].Get_m_cType() == firstcategory && 
			m_UserItemList[i].Get_m_sID() == id )
		{
			return i;
		}		
	}
				
    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		if( m_UserItemList[i].Get_m_cType() == firstcategory && 
			m_UserItemList[i].Get_m_sID() == id )
		{
			return i;
		}		
	}
#else
	for( int i = 0; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; ++i )
	{
		if( m_UserItemList[i].Get_m_cType() == firstcategory && 
			m_UserItemList[i].Get_m_sID() == id )
		{
			return i;
		}		
	}
#endif
	
	return -1;
}

_XUserItem* _XLocalUser::GetUserItemFromSecondType(int cType, int sType)
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
    for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if(m_UserItemList[i].Get_m_cType() == cType)
		{
			switch(cType)
			{
			case _XGI_FC_POTION :
				{
					if(g_PotionItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return &m_UserItemList[i];
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					if(g_ResourceItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return &m_UserItemList[i];
				}
				break;
			default :
				break;
			}
		}		
	}
				
    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		if(m_UserItemList[i].Get_m_cType() == cType)
		{
			switch(cType)
			{
			case _XGI_FC_POTION :
				{
					if(g_PotionItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return &m_UserItemList[i];
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					if(g_ResourceItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return &m_UserItemList[i];
				}
				break;
			default :
				break;
			}
		}		
	}
#else
	for( int i = 0; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; ++i )
	{
		if(m_UserItemList[i].Get_m_cType() == cType)
		{
			switch(cType)
			{
			case _XGI_FC_POTION :
				{
					if(g_PotionItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return &m_UserItemList[i];
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					if(g_ResourceItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return &m_UserItemList[i];
				}
				break;
			default :
				break;
			}
		}
	}
#endif

	return NULL;
}

int _XLocalUser::GetUserItemInvenSlotFromSecondType(int cType, int sType)
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
    for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if(m_UserItemList[i].Get_m_cType() == cType)
		{
			switch(cType)
			{
			case _XGI_FC_POTION :
				{
					if(g_PotionItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return i;
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					if(g_ResourceItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return i;
				}
				break;
			default :
				break;
			}
		}		
	}
				
    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		if(m_UserItemList[i].Get_m_cType() == cType)
		{
			switch(cType)
			{
			case _XGI_FC_POTION :
				{
					if(g_PotionItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return i;
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					if(g_ResourceItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return i;
				}
				break;
			default :
				break;
			}
		}		
	}
#else
	for( int i = 0; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; ++i )
	{
		if(m_UserItemList[i].Get_m_cType() == cType)
		{
			switch(cType)
			{
			case _XGI_FC_POTION :
				{
					if(g_PotionItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return i;
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					if(g_ResourceItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return i;
				}
				break;
			default :
				break;
			}
		}
	}
#endif

	return -1;
}

_XUserSkill* _XLocalUser::GetSkillItem( short uniqueid )
{
	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
	{
		if(m_SkillList[i].m_sSkillID == uniqueid)
		{
			return &m_SkillList[i];
		}
	}

	return NULL;
}

int _XLocalUser::GetLearnedSkillIndex(short uniqueid, BOOL bIncludeEventSkill)
{
	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
	{
		if(m_SkillList[i].m_sSkillID == uniqueid)
		{
			return i;
		}
	}

#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
	if(bIncludeEventSkill)
	{
		if(m_EventSkillList.m_sSkillID == uniqueid)
		{
			return _XDEF_MAX_LEARNSKILL;
		}
	}
#endif

	return -1;
}

int _XLocalUser::GetLearnedMasterySkillIndex(short uniqueid)
{
	for(int i = 0 ; i < _XDEF_MAX_LEARNMASTERYSKILL ; ++i)
	{
		if(m_MasterySkillList[i].skillid == uniqueid)
		{
			return i;
		}
	}
	
	return -1;
}

void _XLocalUser::Process(void)
{
#ifdef _XDWDEBUG 
	if( g_UserTracing )
	{
		if( GetMotionClass() == _XACTION_IDLE )
		{
			_XUser* pUser = g_Userlist.FindUser( g_UserTraceTargetName );
			if( pUser )
			{
				D3DXVECTOR3 nexttargetdir = m_Position - pUser->m_Position;
				D3DXVec3Normalize( &nexttargetdir, &nexttargetdir );
				SetTargetPositionAngle( pUser->m_Position + (nexttargetdir * 1.5f) );
			}			
		}
	}	
#endif

#ifdef _XTS_SAFETYZONE
#ifdef _XTS_PKZONE_SERVERSETTING
	if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkserverinfo == 1)
#else
	if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkclientinfo == 1)
#endif
	{
		if(m_SafetyZoneCheckTimer == 0)
		{
			m_SafetyZoneCheckTimer = g_LocalSystemTime;
		}
		else
		{
			DWORD safetyzoneelapsedtime = g_LocalSystemTime - m_SafetyZoneCheckTimer;
			if(safetyzoneelapsedtime >= 5000)		// 5�ʸ��� üũ
			{
				int areanumber = 0;
				BOOL bCheckSafetyZone = TRUE;
				int	extraskillindex = 0;
#ifdef _XDWDEBUG
//				D3DXVECTOR3 position;
//				g_AreaManager.GetMyAreaCenterPosition(g_pLocalUser->m_Position, position);
#endif
				if(m_BattleMode == _XBATTLE_PK)
				{
					if(g_pLocalUser->m_PKModeStartTime > 0)
					{
						if((g_pLocalUser->m_PKModeStartTime + g_pLocalUser->m_PKModeLeftTime) >= g_LocalSystemTime)
						{
							DWORD lefttime = (g_pLocalUser->m_PKModeStartTime + g_pLocalUser->m_PKModeLeftTime) - g_LocalSystemTime;	
							lefttime /= 1000;
							if(lefttime >= 6)
							{
								// PKī��Ʈ�� 6�� �̻� �������� ��� ���������� �����ϴ��� ���� ������ ���� �� ����.
//								bCheckSafetyZone = FALSE;
								// ���� ���� ��� ��� ��û�Ѵ�.
								extraskillindex = 2;
							}
						}
					}
				}
				
				if(bCheckSafetyZone)
				{
					if(g_AreaManager.CheckInsideArea(g_pLocalUser->m_Position, _XDEF_AREACATEGORY_SAFE, areanumber))
					{
						// ���� ���� ����
						if(m_InsideSafetyZoneIndex == 0)
						{
							// ���� �� ��
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3031), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);

							// ������ �˸�
							if(extraskillindex > 0)
								g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_saferegion_req, extraskillindex);
							else
								g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_saferegion_req, 1);
						}
					}
					else
					{
						// ���� ���� �ٱ���
						if(m_InsideSafetyZoneIndex == 1)
						{
							// ������
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3033), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);

							// ������ �˸�
							g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_saferegion_req, 0);
						}
					}
				}
				
#ifdef _XDEF_CASTLEBATTLE_MINIMAP
				if(m_bCastleBattle)
				{
					if(g_AreaManager.CheckInsideArea(g_pLocalUser->m_Position, _XDEF_AREACATEGORY_CASTLE, areanumber))
					{
						// ������� ���� ���̸� ��� �ȿ� ����
						if(g_pMinimap_Window)
						{
							g_pMinimap_Window->SetMinimapRateInCastleBattle();
						}
					}
					else
					{
						if(g_pMinimap_Window)
						{
							if(g_pMinimap_Window->m_bDisableZoomButton)
								g_pMinimap_Window->SetMinimapRate();
						}
					}
				}
				else
				{
					if(g_pMinimap_Window)
					{
						if(g_pMinimap_Window->m_bDisableZoomButton)
							g_pMinimap_Window->SetMinimapRate();
					}
				}
#endif

				m_SafetyZoneCheckTimer = g_LocalSystemTime;
			}
		}
	}
#endif

	if(m_PrevProcessTime == -1.0f)
	{
		m_PrevProcessTime = g_fLocalSystemTime;
	}

	FLOAT progresstime = (g_fLocalSystemTime - m_PrevProcessTime) / 1000.0f;
	if(progresstime <= 0.0f)
		return;

	m_PrevProcessTime = g_fLocalSystemTime;

	///////////////////////////////////////////////////////////////////////
	// Apply gravity
	
	if(!g_FreeMoveMode )
	{
		_XOBBData*	pCollideOBB = NULL;
		FLOAT terrainheight = g_LodTerrain.GetTerrainAndOBBHeight( m_Position, pCollideOBB );
		
		if( fabs(m_Position.y - terrainheight) > EPSILON3 )
		{
			if( m_Position.y < terrainheight && (terrainheight - m_Position.y) < _XDEF_RIDEOBBLIMITHEIGHT )
			{
				m_Position.y = terrainheight;
				
				if( m_Velocity.y > 0.0f )
				{
					m_Velocity.y += _XDEF_CHARACTERGRAVITY * g_fElapsedFrameMilisecondTime;

					FLOAT Length = _XFC_sqrt(m_Velocity.y * m_Velocity.y);
					if ( Length > _XDEF_MAXJUMPVELOCITY )
					{
						m_Velocity.y *= ( _XDEF_MAXJUMPVELOCITY / Length );
					}

					m_Position.y += m_Velocity.y * g_fElapsedFrameMilisecondTime;
					
					if( terrainheight+_XDEF_MAXJUMPHEIGHT < m_Position.y )  // Ư�� ���� �̻� ���� �Ұ� 
					{
						m_Position.y = terrainheight+_XDEF_MAXJUMPHEIGHT;
						m_Velocity.y = 0.0f;
					}

					D3DXVECTOR3 vecDec = -m_Velocity;
					D3DXVec3Normalize( &vecDec, &vecDec );

					Length = D3DXVec3Length( &m_Velocity );
					FLOAT Dec = (_XDEF_CHARACTERFRICTION * g_fElapsedFrameMilisecondTime);
					if ( Dec > Length ) Dec = Length;					
					m_Velocity += vecDec * Dec;					
					if( m_Velocity.y < 0.0f ) m_Velocity.y = 0.0f;
				}

				UpdatePosition();
				g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(m_Position.x, m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, m_Position.z) );
				g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain, FALSE );

			}
			else if( m_Position.y > terrainheight )
			{
				if( m_UseSpeedSkill )
				{
					m_Position.y += (_XDEF_CHARACTERGRAVITY * 4.0f) * g_fElapsedFrameMilisecondTime;
				}
				else
				{
					m_Position.y += _XDEF_CHARACTERGRAVITY * g_fElapsedFrameMilisecondTime;
				}

				if( m_Velocity.y > 0.0f )
				{
					m_Velocity.y += _XDEF_CHARACTERGRAVITY * g_fElapsedFrameMilisecondTime;

					FLOAT Length = _XFC_sqrt(m_Velocity.y * m_Velocity.y);
					if ( Length > _XDEF_MAXJUMPVELOCITY )
					{
						m_Velocity.y *= ( _XDEF_MAXJUMPVELOCITY / Length );
					}

					m_Position.y += m_Velocity.y * g_fElapsedFrameMilisecondTime;

					if( terrainheight+_XDEF_MAXJUMPHEIGHT < m_Position.y ) // Ư�� ���� �̻� ���� �Ұ� 
					{
						m_Position.y = terrainheight+_XDEF_MAXJUMPHEIGHT;
						m_Velocity.y = 0.0f;
					}

					D3DXVECTOR3 vecDec = -m_Velocity;
					D3DXVec3Normalize( &vecDec, &vecDec );

					Length = D3DXVec3Length( &m_Velocity );
					FLOAT Dec = (_XDEF_CHARACTERFRICTION * g_fElapsedFrameMilisecondTime);
					if ( Dec > Length ) Dec = Length;
					m_Velocity += vecDec * Dec;					
					if( m_Velocity.y < 0.0f ) m_Velocity.y = 0.0f;
				}
				
				if( m_Position.y < terrainheight ) m_Position.y = terrainheight;
				
				UpdatePosition();
				g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(m_Position.x, m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, m_Position.z) );
				g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain, FALSE );
			}
		}
		else
		{
			m_Velocity.y = 0.0f;
		}
	}
	
	///////////////////////////////////////////////////////////////////////
	// Animation ��ó��
	int eventlevel = 0; // Effect�� ������ ������.
		
	switch(m_CurrentSelectedSkillType)
	{
	case _XSI_USEFULTYPE_PASSIVESKILL :
		break;
	case _XSI_USEFULTYPE_ACTIVESKILL :
		break;
	case _XSI_USEFULTYPE_CHARGESKILL :
		{				
			if( m_CurrentBattleState == _XUSER_STATE_8 )
			{
				if( m_ChargeIterateValue >= 0 && m_ChargeIterateValue < 2 ) eventlevel = 0;
				else if( m_ChargeIterateValue >= 2 && m_ChargeIterateValue < 4 ) eventlevel = 1;
				else if( m_ChargeIterateValue >= 4 && m_ChargeIterateValue <= 11 ) eventlevel = 2;
				if( g_ZeroLevelMode )		// hotblood zerolevel
					eventlevel = 0;
			}
		}
		break;
	case _XSI_USEFULTYPE_PASSIVESPELL :
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		{
			if(m_CurrentBattleState == _XUSER_STATE_6)
				eventlevel = 0;
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{				
			if(  m_CurrentBattleState == _XUSER_STATE_4 )
				eventlevel = 0;
		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		break;
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
		{
			if(  m_CurrentBattleState == _XUSER_STATE_4 )
				eventlevel = 0;
		}
		break;
	}
	
	// animation ��ó��
	bool soundplay = true;

	if( m_SelectedSkillID )
	{
		// ��ų�� ���� �ݰ��� ������ ��� ����Ʈ�� ���� �ִ� �ݰ��� �����Ѵ�.
		if( g_SkillProperty[m_SelectedSkillID]->radius > 0.0f )
		{
			m_ModelDescriptor.Advance( soundplay, eventlevel, g_SkillProperty[m_SelectedSkillID]->radius * 2.0f );
		}
		else
		{
			m_ModelDescriptor.Advance(soundplay, eventlevel);
		}
	}
	else
	{
		m_ModelDescriptor.Advance(soundplay, eventlevel);
	}
	
	// by mahwang
	if (m_UserState == _XDEF_USERSTATE_TRAINING && m_SelectedTrainingMode == _XTRAINING_WATERDRAW)
		g_LifeEventManager.Process(g_fElapsedFrameTime / 1000.f);

	GetTargetPosition();

	if(m_bGrabItem)
		GrabWorldItem();

	// ������ �̵��� ��ġ ó�� - ���¿� ���� �������� ����
	BOOL	bCanNotMove = CanNotMove();	
	if(!bCanNotMove)
		ProcessNextMovePosition();
	else
	{
		m_TargetPosition = m_Position;
		if(GetMotionClass() == _XACTION_MOVE) //Author : ����� //breif : ProcessNextMovePosition�ȿ� �ִ� ���� ó��..
		{
			// ����ó�� - ���ڸ� �ٱ� ��
			SetAnimation(_XACTION_IDLE);
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
	}

	// ���� ó��
	ProcessBattle();

	// Delay tiem check
	CheckDelayTime();
	
	// ���� ��ų ���� 
	ApplyNextSkill();
	
	// ���� ó��
	ProcessTraining();

	// �� ó��
	ProcessMatch();

	// PK ó��
	ProcessPKMode();

#ifdef _XDEF_CASHITEM_SHOUT
	// ��ġ�� ó��
	ProcessShout();
#endif

	// Animaiton ó��
	ProcessAnimation();

	// ���� ȿ�� ó��
	ProcessStateList();

	// parameteró��
	ProcessParameter(progresstime);
	
	// ���� ���� �غ� �ڼ� ó��
	ProcessInternalUse();

	// ��� �б� ó��
	ProcessReadBook();

	// PE mode ó��
	if(g_LocalSystemTime - m_PEModeStartTime > m_PEModeLeftTime && m_PEModeStartTime > 0)
	{
		g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_req_pe_end);
		m_PEModeStartTime = 0;		// 1���� ��������
	}

	// ���� �ߵ� ���� ó��
	if( g_LocalSystemTime - m_IntoxicationTimer > 60000 )	// 20�ʸ��� ����
	{
		if( m_ElixirInfo._sIntoxication -1 >= 0 )
		{
			m_IntoxicationTimer = g_LocalSystemTime;
			int tempinto = m_ElixirInfo._sIntoxication;
			m_ElixirInfo._sIntoxication--;
			if( tempinto == 100 && m_ElixirInfo._sIntoxication < 100 )
			{
				g_pState_Window->DeleteIntoxication(_XST_INTOXICATION, 2003);
			}			
		}
		else 
			m_ElixirInfo._sIntoxication = 0;
	}
	
	// ���ִ� ���¿����� ���� �ð� �������� �ñ׳��� ����
	if(GetMotionClass() == _XACTION_IDLE || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_DYING)
	{
		if(m_UserState >= _XDEF_USERSTATE_MATCH_MATCHING && m_UserState <= _XDEF_USERSTATE_MATCH_END)
		{
			// ���� �ȿ� ���� ���� 3�ʿ� �ѹ��� ������.
			if((g_LocalSystemTime - m_PrevPacketSendedTime) > 3000)
			{
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				m_PrevPacketSendedTime = g_LocalSystemTime;
			}
		}
		else
		{
			if((g_LocalSystemTime - m_PrevPacketSendedTime) > _XDEF_SENDTIME_IDLEMOTION)
			{
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				m_PrevPacketSendedTime = g_LocalSystemTime;
			}
		}

		CheckCollideStatusInIdle();
	}

	if( !m_bIsBossBattle ) // �������϶��� ������ ������ ���� �ʴ´�.
	{
		// world item broadcasting request
		_XTILE_ITEM prevtileindex;
		prevtileindex = m_CurrentTileIndex;
		GetItemTileIndex(m_Position.x, m_Position.z, m_CurrentTileIndex);
		if((m_CurrentTileIndex.sX != prevtileindex.sX) || (m_CurrentTileIndex.sZ != prevtileindex.sZ))
		{
			if((g_LocalSystemTime - m_LastItemRequestSendTime) > 3000)
			{
				// 8�� �̻��� �� ������.
				g_NetworkKernel.SendPacket(MSG_NO_WORLD_ITEM_REQUEST);
				m_LastItemRequestSendTime = g_LocalSystemTime;
			}
		}
		else // 2004.05.28->oneway48 insert : ���� Ÿ���϶��� ���� �ð� �������� �ñ׳��� ����
		{
			if((g_LocalSystemTime - m_LastItemRequestSendTime) > 8000)
			{
				g_NetworkKernel.SendPacket(MSG_NO_WORLD_ITEM_REQUEST);
				m_LastItemRequestSendTime = g_LocalSystemTime;
			}
		} // insert end	
	}
	

	if( m_bMatchPRMode )// �� ȫ�� ��Ŷ�� 5�ʸ��� ������.
	{
		if((g_LocalSystemTime - m_MatchPRTimer) > 5000)
		{
			g_NetworkKernel.SendPacket(MSG_NO_MATCH_NOTICE, en_match_ntc_inform);
			m_MatchPRTimer = g_LocalSystemTime;
		}
	}

	//  ���� �Ϸ� ǥ��
	_XWindow_TrainingComplete* pComplete_Window = (_XWindow_TrainingComplete*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGCOMPLETE);
	if(pComplete_Window)
	{
		if((g_LocalSystemTime - pComplete_Window->m_CompleteTime > 3000) && pComplete_Window->m_CompleteTime > 0)
		{
			switch(m_SelectedTrainingMode)
			{
			case _XTRAINING_MEDITATION :
				{
				}
				break;
			case _XTRAINING_DUMMY :
				{
				}
				break;
			}
			pComplete_Window->ShowWindow(FALSE);
		}
	}	
}

void _XLocalUser::SetAttackTarget( _XItem* target, _XPICKINGTARGETTYPE targettype )
{
	if( m_AttackTarget )
	{
		switch(m_AttackTargetObjectType)
		{
		case _XPICKINGTARGET_MOB :
			{
				if(((_XMob*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XMob*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		case _XPICKINGTARGET_NPC :
			{
				if(((_XNPCObject*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XNPCObject*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_PC :
			{
				if(((_XUser*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XUser*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		case _XPICKINGTARGET_SELF :
			{
				if(m_ModelDescriptor.m_EffectManager)
					m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		case _XPICKINGTARGET_DUMMY :
			{
				if(((_XDummy*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XDummy*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		}
	}
	
	
	m_AttackTarget			 = target;
	m_AttackTargetObjectType = targettype;
	
	// �̵������ð� �ʱ�ȭ
	m_PrevMoveTime			 = 0;

	if(m_ViewTarget)
	{
		if(m_ViewTarget == m_AttackTarget)
		{
			switch(m_ViewTargetType)
			{
			case _XPICKINGTARGET_PC :
				{
					if(((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
						((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
				}
				break;
			case _XPICKINGTARGET_MOB :
				{
					if(((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
						((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
				}
				break;
			case _XPICKINGTARGET_NPC :
				{
					if(((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
						((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
				}
				break;
			case _XPICKINGTARGET_DUMMY :
				{
					if(((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					{
						((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
					}
				}
				break;
			case _XPICKINGTARGET_SELF :
				{
					if(m_ModelDescriptor.m_EffectManager)
						m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
				}
				break;
			}
		}
	}

	if(m_AttackTarget)
	{
		switch(targettype)
		{
		case _XPICKINGTARGET_PC :
			{
				if(((_XUser*)target)->m_ModelDescriptor.m_EffectManager)
					((_XUser*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_LOCKON,
																				&((_XUser*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);
			}
			break;
		case _XPICKINGTARGET_MOB :
			{
				if(((_XMob*)target)->m_ModelDescriptor.m_EffectManager)
					((_XMob*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_LOCKON,
					&((_XMob*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);

				m_CharacterLog.changeTargetUniqueID = ((_XMob*)target)->m_MobID;
			}
			break;
		case _XPICKINGTARGET_NPC :
			{
				if(((_XNPCObject*)target)->m_ModelDescriptor.m_EffectManager)
					((_XNPCObject*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
																				&((_XNPCObject*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);
			}
			break;
		case _XPICKINGTARGET_DUMMY :
			{
				if(((_XDummy*)target)->m_ModelDescriptor.m_EffectManager)
				{
					((_XDummy*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_LOCKON,
						&((_XDummy*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);
				}
			}
			break;
		case _XPICKINGTARGET_SELF :
			{
				if(m_ModelDescriptor.m_EffectManager)
					m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_LOCKON,
					&m_ModelDescriptor.m_Position, NULL, TRUE);
			}
			break;
		}
	}

	//if( targettype != _XPICKINGTARGET_FUNCTIONALOBJECT )
	{
		_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
		if(pMonsterStatus_Window)
		{
			pMonsterStatus_Window->ShowWindow( ( target ) ? TRUE : FALSE );
		}
	}
}

void _XLocalUser::ResetAttackTarget(void)
{
	if(m_AttackTarget)
	{
		switch(m_AttackTargetObjectType)
		{
		case _XPICKINGTARGET_PC :
			{
				if(((_XUser*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XUser*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		case _XPICKINGTARGET_MOB :
			{
				if(((_XMob*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XMob*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		case _XPICKINGTARGET_NPC :
			{
				if(((_XNPCObject*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XNPCObject*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_DUMMY :
			{
				if(((_XDummy*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
				{
					((_XDummy*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
				}
			}
			break;
		case _XPICKINGTARGET_SELF :
			{
				if(m_ModelDescriptor.m_EffectManager)
					m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		case _XPICKINGTARGET_FUNCTIONALOBJECT :
			{
//				_XWindow_MatchNoticeLarge* pMatchNoticeLarge_Window = (_XWindow_MatchNoticeLarge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICELARGE);
//				if(pMatchNoticeLarge_Window)
//				{
//					if(pMatchNoticeLarge_Window->GetShowStatus())
//						pMatchNoticeLarge_Window->ShowWindow(FALSE);
//				}
			}
			break;
		}

		if(m_AttackTarget == m_ViewTarget)
		{
			// view target effect �־��ֱ�
			switch(m_ViewTargetType)
			{
			case _XPICKINGTARGET_PC :
				{
					if(((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
						((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_Position, NULL, TRUE );
				}
				break;
			case _XPICKINGTARGET_MOB :
				{
					if(((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
						((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_Position, NULL, TRUE);
					
					_XPlayInterfaceSound(ID_SR_INTERFACE_TARGETING00_WAV);
					g_NetworkKernel.SendPacket(MSG_NO_REQ_SPELLEFFECT, ((_XMob*)m_ViewTarget)->m_MobID);
				}
				break;
			case _XPICKINGTARGET_NPC :
				{
					if(((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
						((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_Position, NULL, TRUE);
				}
				break;
			case _XPICKINGTARGET_DUMMY :
				{
					if(((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					{
						((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
							&((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_Position, NULL, TRUE);
					}
				}
				break;
			case _XPICKINGTARGET_SELF :
				{
					if(m_ModelDescriptor.m_EffectManager)
						m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&m_ModelDescriptor.m_Position, NULL, TRUE);
				}
				break;
			}
		}

		m_AttackTarget = NULL;
		m_AttackTargetObjectType = _XPICKINGTARGET_NONE;
		
		_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
		if(pMonsterStatus_Window)
		{
			if(!m_ViewTarget)
			{
				if(pMonsterStatus_Window->GetShowStatus())
					pMonsterStatus_Window->ShowWindow(FALSE);
			}
		}
	}
}

void _XLocalUser::InsertTargetPositionEffect( void )
{
	if( m_TargetPosition != m_LastInsertedMoveTargetEffectPosition )
	{
		m_LastInsertedMoveTargetEffectPosition = m_FinalTargetPosition;
		D3DXMATRIX	MoveTargetMatrix;
		D3DXMatrixTranslation( &MoveTargetMatrix, m_FinalTargetPosition.x,	m_FinalTargetPosition.y, m_FinalTargetPosition.z );
		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );		
		g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_PICKING, &MoveTargetMatrix );
	}
}

/** GetTargetPosition(void)
 *	���콺 Ŭ�� ó��, Game screen focus ó��
 *	Local User�� Target position ó��
 */
void _XLocalUser::GetTargetPosition(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
		return;

	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	
	MouseState* mousestate = gpInput->GetMouseState();
	
	if(mousestate)
	{
		if(!g_CurrentFocusedObject)
		{
			// ��� ��� ��尡 �ƴϰ� ���콺 L ��ư�� ������� �̵� Ÿ�� ����
			if( !gpInput->CheckKeyPress(DIK_LMENU) && mousestate->bButton[0])
			{
				if( g_CurrentFocusedObject != _XDEF_DUMMYID_FUNCTIONALOBJECT )
				{
					g_CurrentFocusedObject = _XDEF_DUMMYID_GAMEBACKGROUND;
				}
			}
		}
		
		if(!mousestate->bButton[0])
		{
			m_bLeftClickProcess = FALSE;
			
			if(g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND || g_CurrentFocusedObject == _XDEF_DUMMYID_FUNCTIONALOBJECT )
			{
				g_CurrentFocusedObject = NULL;
			}
			
		}
	}
	
	if( !pProc_Maingame->m_bNPCDialogMode  && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
	{
		GetTargetPositionIdleMode();
		GetTargetPositionBattleMode();
	}
}

void _XLocalUser::GetTargetPositionIdleMode(void)
{
	if(m_BattleMode)
		return;

	MouseState* mousestate = gpInput->GetMouseState();
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

	if(g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND && mousestate->bButton[0])
	{
		if(g_LastPickedObject)
		{
			if(g_LastPickedObjectType == _XPICKINGTARGET_ITEM)
			{
				// ������ Ŭ�� - ������ ������ �̵�
				_XGameItem* pickeditem = (_XGameItem*)g_LastPickedObject;
				m_PickItem = pickeditem;
				m_bGrabItem = TRUE;
				// �̵������ð� �ʱ�ȭ
				m_PrevMoveTime			 = 0;
			}
			else
			{
				// �ٸ� ���� Ŭ������ ��� ������ ���� ���
				m_bGrabItem = FALSE;

				if(!m_ViewTarget)
				{
					// Ÿ���� ���� - Ÿ�� ����
					SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
					m_bLeftClickProcess = TRUE;

					// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=
					if( g_TutorialInfo )
					{
						if( !g_TutorialInfo->_XTutorial_003 && g_LastPickedObjectType == _XPICKINGTARGET_MOB )
						{
							_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
							pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_029);
							g_TutorialInfo->_XTutorial_003 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
							g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
						}
					
						if( !g_TutorialInfo->_XTutorial_016 && g_LastPickedObjectType == _XPICKINGTARGET_MOB && 
							(g_MobAttribTable[((_XMob*)m_ViewTarget)->m_MobType].party > 16) && 
							(g_MobAttribTable[((_XMob*)m_ViewTarget)->m_MobType].party < 25) )
						{
							_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
							pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_016);
							g_TutorialInfo->_XTutorial_016 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
							g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
						}
					}					
					// ----------------------------------------------------------------------------------------------------------------------------=
				}
				else if(!m_bLeftClickProcess)
				{
					// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=
					if( g_TutorialInfo )
					{
						if( !g_TutorialInfo->_XTutorial_016 && g_LastPickedObjectType == _XPICKINGTARGET_MOB && 
							(g_MobAttribTable[((_XMob*)m_ViewTarget)->m_MobType].party > 16) && 
							(g_MobAttribTable[((_XMob*)m_ViewTarget)->m_MobType].party < 25) )
						{
							_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
							pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_016);
							g_TutorialInfo->_XTutorial_016 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
							g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
						}
					}
					// ----------------------------------------------------------------------------------------------------------------------------=
					
					// Ÿ���� ���� - �ι�° Ŭ���̰ų� Ȥ�� �ٸ� Ÿ��
					if(m_ViewTargetType == _XPICKINGTARGET_NPC)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// �ι�° Ŭ��
							if(!pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
							{
								// NPC ��ȭ��尡 �ƴҶ���
								_XNPCObject* pNPC = (_XNPCObject*)m_ViewTarget;
								D3DXVECTOR3 position = D3DXVECTOR3(pNPC->m_matWorldPosition._41, pNPC->m_matWorldPosition._42, pNPC->m_matWorldPosition._43);
								int ret = pProc_Maingame->StartNPCScript();
								SetAttackTarget((_XItem*)pNPC, _XPICKINGTARGET_NPC);

								if(ret == 0)
								{
									// NPC������ �̵�
									D3DXVECTOR3 directionvector = m_Position - position;
									D3DXVec3Normalize(&directionvector, &directionvector);
									
									D3DXVECTOR3 targetposition = position + (directionvector*(_XDEF_DISTANCEOFSTARTNPCSCRIPT*0.95f));
									targetposition.y = position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									SetTargetPositionAngle(m_Position);
									
									// ��ȭ �α� ���
									if(g_ScriptStarted)
									{
										m_CharacterLog.isTalk = TRUE;
										m_CharacterLog.talkTime = g_ServerTimeCode;

										_XDWINPRINT("[LOCAL USER/GetTargetPositionIdleMode] NPC[%d] talk", pNPC->m_UniqueID);
									}
								}
							} // ��ȭ���
						}
						else
						{
							// �ٸ� Ÿ��
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					} // if(m_ViewTargetType == _XPICKINGTARGET_NPC)
					else if( m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT )
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// �ι�° Ŭ��
							_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_ViewTarget;
							if( pFunctionalObject->m_HealthPercentage > 0 )
							{
								int ret = pProc_Maingame->StartFunctionalObjectScript();
									
								//SetAttackTarget((_XItem*)pFunctionalObject, _XPICKINGTARGET_FUNCTIONALOBJECT);
								
								if( ret == 0 )
								{
									// OBJECT ������ �̵�
									D3DXVECTOR3 position = pFunctionalObject->m_Position;
									D3DXVECTOR3 directionvector = m_Position - position;
									D3DXVec3Normalize(&directionvector, &directionvector);
									
									D3DXVECTOR3 targetposition = position + (directionvector*(g_LodTerrain.m_MeshObjectManager.m_ModelList[ pFunctionalObject->m_ObjectIndex ]->OBB_Extent[2]*2 + 2.5f));
									targetposition.y = position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{									
									g_CurrentFocusedObject = _XDEF_DUMMYID_FUNCTIONALOBJECT;
									SetTargetPositionAngle(m_Position);								
								}
							}
						
						}
						else
						{
							// �ٸ� Ÿ��
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					}//else if( m_ViewTargetType = _XPICKINGTARGET_FUNCTIONALOBJECT )
					else if(m_ViewTargetType == _XPICKINGTARGET_MOB)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// �ι�° Ŭ��
							_XMob* pMob = (_XMob*)m_ViewTarget;
							FLOAT range = m_AttackRange + g_MobAttribTable[pMob->m_MobType].collideradius;
							FLOAT dx = m_Position.x - pMob->m_Position.x;
							FLOAT dz = m_Position.z - pMob->m_Position.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

//							SetAttackTarget((_XItem*)pMob, _XPICKINGTARGET_MOB);
							
							if(distance > range)
							{
								
								D3DXVECTOR3 directionvector = m_Position - pMob->m_Position;
								D3DXVec3Normalize(&directionvector, &directionvector);

								D3DXVECTOR3 targetposition = pMob->m_Position + (directionvector*(range*0.7f));
								targetposition.y = pMob->m_Position.y;
								SetTargetPositionAngle(targetposition);
								InsertTargetPositionEffect();
							}
							else
							{
								ProcessLPSSystem(pMob);
								UpdatePosition();								
							}

							// LPS�� �޼����� ������� �ʴ´� 
							if( g_MobAttribTable[pMob->m_MobType].scripttype < 81 || g_MobAttribTable[pMob->m_MobType].scripttype > 85)
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2621), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// ��ȭ��忡���� ������ ����� �� �����ϴ�.
							}
						}
						else
						{
							// �ٸ� Ÿ��
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}	
					} // else if(m_ViewTargetType == _XPICKINGTARGET_MOB)
					else if(m_ViewTargetType == _XPICKINGTARGET_PC)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// �ι�° Ŭ��
							_XUser* pUser = (_XUser*)m_ViewTarget;
							FLOAT range = m_AttackRange + 1.0f;
							FLOAT dx = m_Position.x - pUser->m_Position.x;
							FLOAT dz = m_Position.z - pUser->m_Position.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);
							
//							SetAttackTarget(pUser, _XPICKINGTARGET_PC);
							bool jump = false;
							if( this->m_PersonalStoreState == _XUSER_PERSONALSTORE_NONE )
							{
								// PC�� ���� ������ ������ ��� 
								if( pUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
								{
									jump = true;
									if(distance > 1.0f ) // 1.0f �ȿ� ���;� �ŷ��� �� �� �ִ�.
									{
										// PC������ �̵�
										D3DXVECTOR3 directionvector = m_Position - pUser->m_Position;
										D3DXVec3Normalize(&directionvector, &directionvector);
										
										D3DXVECTOR3 targetposition = pUser->m_Position + (directionvector);
										targetposition.y = pUser->m_Position.y;
										SetTargetPositionAngle(targetposition);
										InsertTargetPositionEffect();
									}
									else
									{
										if((g_LocalSystemTime - m_PrevRequestPacketSendedTime) > 1000)
										{		
											g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_shopitem_list_req,pUser->m_Uniqueid);
											m_PrevRequestPacketSendedTime  = g_LocalSystemTime;
										}
										
										_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
										if( pPersonalStore_Window )
										{
											pPersonalStore_Window->m_OwnerUniqueID = pUser->m_Uniqueid;
											memset(pPersonalStore_Window->m_OwnerName, 0, sizeof(TCHAR)*_XDEF_MAX_USERNAMESTRINGLENGTH);
											memset(pPersonalStore_Window->m_PersonalStoreName, 0, sizeof(TCHAR)*37);
											strcpy( pPersonalStore_Window->m_OwnerName, pUser->m_CharacterInfo.charactername);
											strcpy( pPersonalStore_Window->m_PersonalStoreName, pUser->m_PersonalStoreName );
										}
									}
								}
							}
							
							if( !jump )
							{
								if(distance > range)
								{
									// PC������ �̵�
									D3DXVECTOR3 directionvector = m_Position - pUser->m_Position;
									D3DXVec3Normalize(&directionvector, &directionvector);
									
									D3DXVECTOR3 targetposition = pUser->m_Position + (directionvector*(range*0.7f));
									targetposition.y = pUser->m_Position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pUser->m_Position);
									UpdatePosition();
									
									if(pUser->GetMotionClass() == _XACTION_DYING)
										ProcessRescueUser();
								}
							}
							
						}
						else
						{
							// �ٸ� Ÿ��
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					} // else if(m_ViewTargetType == _XPICKINGTARGET_PC)
					else if(m_ViewTargetType == _XPICKINGTARGET_DUMMY)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// �ι�° Ŭ��
							_XDummy* pDummy = (_XDummy*)m_ViewTarget;
							FLOAT range = m_AttackRange + 1.0f;
							FLOAT dx = m_Position.x - pDummy->m_Position.x;
							FLOAT dz = m_Position.z - pDummy->m_Position.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

							SetAttackTarget(pDummy, _XPICKINGTARGET_DUMMY);

							if(distance > range)
							{
								D3DXVECTOR3 directionvector = m_Position - pDummy->m_Position;
								D3DXVec3Normalize(&directionvector, &directionvector);

								D3DXVECTOR3 targetposition = pDummy->m_Position + (directionvector*range);
								targetposition.y = pDummy->m_Position.y;
								SetTargetPositionAngle(targetposition);
								InsertTargetPositionEffect();
							}
							else
							{
								if(m_UserState != _XDEF_USERSTATE_TRAINING)
								{
									_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
									if(pDummy_Window)
									{
										if(!pDummy_Window->m_bComplete)
											g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2896), TRUE); //_T("��ȭ���¿����� ������ ������ �� �����ϴ�.")
									}
								}
							}
						}
						else
						{
							// �ٸ� Ÿ��
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					} // else if(m_ViewTargetType == _XPICKINGTARGET_DUMMY)
					else if(m_ViewTargetType == _XPICKINGTARGET_SEAT)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// �ι�° Ŭ��
							_XSeat* pSeat = (_XSeat*)m_ViewTarget;
							FLOAT range = 1.0f;
							FLOAT dx = m_Position.x - pSeat->m_Position.x;
							FLOAT dz = m_Position.z - pSeat->m_Position.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);
							
							SetAttackTarget(pSeat, _XPICKINGTARGET_SEAT);
							
							if(distance > range)
							{
								D3DXVECTOR3 directionvector = m_Position - pSeat->m_Position;
								D3DXVec3Normalize(&directionvector, &directionvector);
								
								D3DXVECTOR3 targetposition = pSeat->m_Position + (directionvector*range);
								targetposition.y = pSeat->m_Position.y;
								SetTargetPositionAngle(targetposition);
								InsertTargetPositionEffect();
							}
							else
							{
								if(m_UserState != _XDEF_USERSTATE_TRAINING)
								{
									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2897), TRUE); //_T("��ȭ���¿����� ��� ������ ������ �� �����ϴ�.")
								}
							}
						}
						else
						{
							// �ٸ� Ÿ��
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					} // else if(m_ViewTargetType == _XPICKINGTARGET_SEAT)
					else if(m_ViewTargetType == _XPICKINGTARGET_SELF)
					{
						if(m_ViewTarget != g_LastPickedObject)
						{
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					}
					
					// by mahwang
					else if(m_ViewTargetType == _XPICKINGTARGET_COLLECT)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							_XOctreeObject* pObject = (_XOctreeObject*)m_ViewTarget;

							D3DXVECTOR3 vObjPosition(pObject->m_matWorldPosition._41, pObject->m_matWorldPosition._42, pObject->m_matWorldPosition._43);

							FLOAT range = 1.0f;
							FLOAT dx = m_Position.x - vObjPosition.x;
							FLOAT dz = m_Position.z - vObjPosition.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

							SetAttackTarget(pObject, _XPICKINGTARGET_COLLECT);

							if(distance > range)
							{
								D3DXVECTOR3 directionvector = m_Position - vObjPosition;
								D3DXVec3Normalize(&directionvector, &directionvector);

								D3DXVECTOR3 targetposition = vObjPosition + (directionvector*range);
								targetposition.y = vObjPosition.y;
								SetTargetPositionAngle(targetposition);
								InsertTargetPositionEffect();
							}
							else
							{
								if(GetMotionClass() != _XACTION_COLLECT)
								{
									switch(pObject->m_nEventType)
									{
									case OBJECT_EVENT_COLLECT_COTTON:
									case OBJECT_EVENT_COLLECT_TEA:
									case OBJECT_EVENT_COLLECT_MUSHROOM:
									case OBJECT_EVENT_COLLECT_TREE:
										{
											// ��� �ִ� ������ ����
											m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
											m_CharacterInfo.modeldescript.item1 = 0;
											m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
											m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;

											_XAnimMeshAbstractClass* pEventItem = NULL;
											if (pObject->m_nEventType == OBJECT_EVENT_COLLECT_TREE)
												pEventItem = g_LifeEventManager.GetEventItem(m_CharacterInfo.gender, LIFE_EVENT_ITEM_TYPE_AXE);
											else if (pObject->m_nEventType == OBJECT_EVENT_COLLECT_MUSHROOM)
												pEventItem = g_LifeEventManager.GetEventItem(m_CharacterInfo.gender, LIFE_EVENT_ITEM_TYPE_HOMI);

											m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, pEventItem );


											int nIndex = g_LifeEventManager.GetaniIndex(m_CharacterInfo.gender, pObject->m_nEventType - 1);
											if (nIndex < 0) break;
											SetAnimationIndex(nIndex);
										}
										break;
									}
								}
								if(m_UserState != _XDEF_USERSTATE_TRAINING)
								{
									switch(pObject->m_nEventType)
									{
									case OBJECT_EVENT_FIREWOOD_POINT:
										{
											m_nTrainingCount = 0;
											m_UserState = _XDEF_USERSTATE_TRAINING;
											m_SelectedTrainingMode = _XTRAINING_FIREWOODCUT;
											m_CurrentTrainingState = _XUSER_STATE_1;

											D3DXVECTOR3 vD;
											vD.x = pObject->m_matWorldPosition._41;
											vD.y = pObject->m_matWorldPosition._42;
											vD.z = pObject->m_matWorldPosition._43;

											m_Position.x = vD.x + pObject->m_matWorldPosition._31 * 1.f;
											m_Position.y = vD.y + pObject->m_matWorldPosition._32 * 1.f;
											m_Position.z = vD.z + pObject->m_matWorldPosition._33 * 1.f;

											m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, vD);
											UpdatePosition();
										}
										break;
									case OBJECT_EVENT_WATER_POINT:
										m_nTrainingCount = 0;
										m_UserState = _XDEF_USERSTATE_TRAINING;
										m_SelectedTrainingMode = _XTRAINING_WATERDRAW;
										m_CurrentTrainingState = _XUSER_STATE_1;
										break;
									}
								}
								else 
								{
									switch(pObject->m_nEventType)
									{
										case OBJECT_EVENT_WATER_POINT:
											if (m_SelectedTrainingMode == _XTRAINING_WATERDRAW && m_CurrentTrainingState == _XUSER_STATE_2)
											{
												m_CurrentTrainingState = _XUSER_STATE_3;
											}
											break;
										case OBJECT_EVENT_WATER_TUBE:
											if (m_SelectedTrainingMode == _XTRAINING_WATERDRAW && m_CurrentTrainingState == _XUSER_STATE_5)
											{
												m_CurrentTrainingState = _XUSER_STATE_6;
											}
											break;
									}
								}
							}
						}
						else
						{
							// �ٸ� Ÿ��
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					} // else if(m_ViewTargetType == _XPICKINGTARGET_COLLECT)
				}
			}
		} // if(g_LastPickedObject)
		else if(!m_bLeftClickProcess)
		{
			if( !pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
			{
				// NPC ��ȭ��尡 �ƴҶ���
				pProc_Maingame->m_bAutoNPCScriptPopUp = FALSE;

				// �ٸ� ���� Ŭ������ ��� ������ ���� ���
				m_bGrabItem = FALSE;

				// ����ȿ���� �̵� �Ҵ��� ��� �޼��� ���
				if(m_Lock.movelock == 1)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2898), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //"���� �̻����� ���Ͽ� �̵��� �Ұ��� �մϴ�."
				}

				if(CheckValidNextPosition())
				{
					SetTargetPositionAngle(g_vMousePosition);
					InsertTargetPositionEffect();
				}
				else
				{
					if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE && GetMotionClass() != _XACTION_REBIRTH )
					{
						m_TargetPosition = m_Position;
					}
				}
			}
		}
	} // if(g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
}


void _XLocalUser::SetViewTargetToAttackTarget(void)
{
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	
	if( !m_ViewTarget ) return;
	if( !m_BattleMode ) return;

	// ����ȿ���� �Ǽ� ���� �Ұ���
	if(m_Lock.battlelock == 1 && m_SelectedSkillID == 0)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2736), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //"���� �̻����� ���Ͽ� ������ �Ұ��� �մϴ�."
		return;
	}

	if(m_ViewTargetType == _XPICKINGTARGET_MOB)
	{
		_XMob* pMob = (_XMob*)m_ViewTarget;

		if(g_MobAttribTable[pMob->m_MobType].scripttype != 82)	// Key LPS�� ���� ó������ ����
		{
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : ����� //breif : ��Ʈ�� shift Ŭ��
			if(g_MobAttribTable[pMob->m_MobType].scripttype >= 91 &&
			   g_MobAttribTable[pMob->m_MobType].scripttype <= 95 )
			{
				if(pMob->m_Action.action != _XMOBACTION_DIE)
				{
					if( gpInput->CheckKeyPress( DIK_LSHIFT ) )
						m_bGuardMob = true;

					if( pMob->m_Action.action == _XMOBACTION_ATTACK || m_bGuardMob)
					{
						FLOAT range = 0.0f;

						if(m_AttackTarget != m_ViewTarget)
						{
							SetAttackTarget(pMob, _XPICKINGTARGET_MOB);

							if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
							{
								// �� ���󰡱�
								if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
								{
									SetTargetPosition(pMob->m_Position, range);
								}
							}
							else
							{
								// �� ������ ���� Ŭ��
								if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
								{
									// active skill ����
								}
							}
						}
						else
						{
							if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
							{
								// �� ���󰡱�
								if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
								{
									SetTargetPosition(pMob->m_Position, range);
								}
							}
							else
							{
								if(m_CurrentBattleState == _XUSER_STATE_NONE || m_CurrentBattleState == _XUSER_STATE_LAST)
								{
									// �� ������ ���� Ŭ��
									if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
									{
										// active skill ����
									}
								}
							}
						}
					}
				}
			}
			else
			{
#endif
				if(pMob->m_Action.action != _XMOBACTION_DIE)
				{
					FLOAT range = 0.0f;

					if(m_AttackTarget != m_ViewTarget)
					{
						SetAttackTarget(pMob, _XPICKINGTARGET_MOB);

						if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
						{
							// �� ���󰡱�
							if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
							{
								SetTargetPosition(pMob->m_Position, range);
							}
						}
						else
						{
							// �� ������ ���� Ŭ��
							if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
							{
								// active skill ����
							}
						}
					}
					else
					{
						if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
						{
							// �� ���󰡱�
							if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
							{
								SetTargetPosition(pMob->m_Position, range);
							}
						}
						else
						{
							if(m_CurrentBattleState == _XUSER_STATE_NONE || m_CurrentBattleState == _XUSER_STATE_LAST)
							{
								// �� ������ ���� Ŭ��
								if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
								{
									// active skill ����
								}
							}
						}
					}
				} // if(pMob->m_Action.action != _XMOBACTION_DIE)
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : ����� //breif : ��Ʈ�� shift Ŭ��
			}
#endif
		}
	}
	else if(m_ViewTargetType == _XPICKINGTARGET_PC)
	{
		_XUser* pUser = (_XUser*)m_ViewTarget;
		if(!pUser)
			return;

		if((m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && m_MatchTargetID == pUser->m_Uniqueid) ||
			(m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) ||
			(m_UserState == _XDEF_USERSTATE_PK_FIGHT && CheckPKTarget(pUser)))
		{
			// ���� ����
			if(pUser->GetMotionClass() != _XACTION_DYING && pUser->GetMotionClass() != _XACTION_DIE)
			{
				FLOAT range = 0.0f;
				
				if(m_AttackTarget != m_ViewTarget)
				{
					SetAttackTarget(pUser, _XPICKINGTARGET_PC);
					
					if(!CheckDistanceToTarget(pUser->m_Position, m_ActiveSlotID, range, 0, true))
					{
						// �� ���󰡱�
						if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
						{
							SetTargetPosition(pUser->m_Position, range);
						}
					}
					else
					{
						// �� ������ ���� Ŭ��
						if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
						{
							// active skill ����
						}
					}
				}
				else
				{
					if(!CheckDistanceToTarget(pUser->m_Position, m_ActiveSlotID, range, 0, true))
					{
						// �� ���󰡱�
						if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
						{
							SetTargetPosition(pUser->m_Position, range);
						}
					}
					else
					{
						if(m_CurrentBattleState == _XUSER_STATE_NONE)
						{
							// �� ������ ���� Ŭ��
							if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
							{
								// active skill ����
							}
						}
					}
				}
			} // if(pUser->GetMotionClass() != _XACTION_DYING || pUser->GetMotionClass() != _XACTION_DIE)
		} // ���� ����
	} // _XPICKINGTARGET_PC
}


void _XLocalUser::GetTargetPositionBattleMode(void)
{
	if(!m_BattleMode)
		return;

	MouseState* mousestate = gpInput->GetMouseState();
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	
	if(g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND && mousestate->bButton[0])
	{
		if(g_LastPickedObject)
		{
			if(g_LastPickedObjectType == _XPICKINGTARGET_ITEM)
			{
				// ������ Ŭ�� - ������������ �̵�
				_XGameItem* pickeditem = (_XGameItem*)g_LastPickedObject;
				m_PickItem = pickeditem;
				m_bGrabItem = TRUE;

				// �̵������ð� �ʱ�ȭ
				m_PrevMoveTime			 = 0;
			}
			else
			{
				// �ٸ� ���� Ŭ������ ��� ������ ���� ���
				m_bGrabItem = FALSE;

				BOOL bChangeTarget = FALSE;
				if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_ACTIVESKILL || m_SelectedSkillID == 0)
				{
					bChangeTarget = TRUE;
				}
				else
				{
					if(m_CurrentBattleState == _XUSER_STATE_NONE || m_CurrentBattleState == _XUSER_STATE_LAST)
					{
						bChangeTarget = TRUE;
					}
				}

				if(!m_ViewTarget)
				{					
					// Ÿ���� ���� - Ÿ�� ����
#ifdef _XTS_PK
					SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
					SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
					m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : ����� //breif : ��Ʈ�� shift Ŭ��
					m_bGuardMob =false;
#endif

					// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
					if( g_TutorialInfo )
					{
						if( !g_TutorialInfo->_XTutorial_003 && g_LastPickedObjectType == _XPICKINGTARGET_MOB )
						{
							_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
							pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_029);
							g_TutorialInfo->_XTutorial_003 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
							g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
						}
					}					
					// ----------------------------------------------------------------------------------------------------------------------------=
				}
				else if(!m_bLeftClickProcess && bChangeTarget)
				{
					// Ÿ���� ���� - �ι�° Ŭ���̰ų� �ٸ� Ÿ�� ����
					if(m_ViewTargetType == _XPICKINGTARGET_NPC)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// �ι�° Ŭ��
							if(!pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
							{
								// NPC ��ȭ��尡 �ƴҶ�
								_XNPCObject* pNPC = (_XNPCObject*)m_ViewTarget;
								D3DXVECTOR3 position = D3DXVECTOR3(pNPC->m_matWorldPosition._41, pNPC->m_matWorldPosition._42, pNPC->m_matWorldPosition._43);
								int ret = pProc_Maingame->StartNPCScript();

								SetAttackTarget((_XItem*)pNPC, _XPICKINGTARGET_NPC);

								if(ret == 0)
								{
									// NPC ������ �̵�
									D3DXVECTOR3 directionvector = m_Position - position;
									D3DXVec3Normalize(&directionvector, &directionvector);

									D3DXVECTOR3 targetposition = position + (directionvector*(_XDEF_DISTANCEOFSTARTNPCSCRIPT*0.95f));
									targetposition.y = position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									SetTargetPositionAngle(m_Position);

									// ��ȭ �α� ���
									if(g_ScriptStarted)
									{
										m_CharacterLog.isTalk = TRUE;
										m_CharacterLog.talkTime = g_ServerTimeCode;

										_XDWINPRINT("[LOCAL USER/GetTargetPositionBattleMode] NPC[%d] talk", pNPC->m_UniqueID);
									}
								}
							}
						} // if(m_ViewTarget == g_LastPickedObject)
						else
						{
							// �ٸ� Ÿ��
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : ����� //breif : ��Ʈ�� shift Ŭ��
							m_bGuardMob =false;
#endif
						}
					} // if(m_ViewTargetType == _XPICKINGTARGET_NPC)
					else if(m_ViewTargetType == _XPICKINGTARGET_PC)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// �ι�° Ŭ��
							_XUser* pUser = (_XUser*)m_ViewTarget;
							FLOAT range = m_AttackRange + 0.5f;
							FLOAT dx = m_Position.x - pUser->m_Position.x;
							FLOAT dz = m_Position.z - pUser->m_Position.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

							if((m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && m_MatchTargetID == pUser->m_Uniqueid) ||
								(m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) ||
								(m_UserState == _XDEF_USERSTATE_PK_FIGHT && CheckPKTarget(pUser)))
							{
								// ������ ������ ���
								
								// debug message �̹Ƿ� string table�� ���� �ʿ� �����ϴ�.
								if(m_UserState == _XDEF_USERSTATE_MATCH_FIGHT)
								{
									_XDWINPRINT("���ݰ��� Ÿ��(����Ŭ��) : localuserstate[����]  matchtargetid[%d] userid[%d]", m_UserState, m_MatchTargetID, pUser->m_Uniqueid);
								}
								else if(m_UserState == _XDEF_USERSTATE_PK_READY || m_UserState == _XDEF_USERSTATE_PK_FIGHT)
								{
									_XDWINPRINT("���ݰ��� Ÿ��(����Ŭ��) : localuserstate[PK��] userid[%d]", m_UserState, pUser->m_Uniqueid);
								}

								if(m_Lock.battlelock == 1 && m_SelectedSkillID == 0)
								{
									// ����ȿ������ �Ǽ� ���� �Ұ� - GetTargetPositionIdleMode()�� �����ϰ� ����
									if(distance > range)
									{
										D3DXVECTOR3 directionvector = m_Position - pUser->m_Position;
										D3DXVec3Normalize(&directionvector, &directionvector);

										D3DXVECTOR3 targetposition = pUser->m_Position + (directionvector*(range*0.7f));
										targetposition.y = pUser->m_Position.y;
										SetTargetPositionAngle(targetposition);
										InsertTargetPositionEffect();
									}
									else
									{
										UpdatePosition();
									}
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT( ID_STRING_NEW_2736 ), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // "���� �̻����� ���Ͽ� ������ �Ұ��� �մϴ�."
								}
								else
								{
									if(pUser->GetMotionClass() != _XACTION_DYING && pUser->GetMotionClass() != _XACTION_DIE)
									{
										FLOAT range = 0.0f;

										if(m_AttackTarget != g_LastPickedObject)
										{
											SetAttackTarget(pUser, _XPICKINGTARGET_PC);

											if(!CheckDistanceToTarget(pUser->m_Position, m_ActiveSlotID, range, 0, true))
											{
												// ���󰡱�
												if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
												{
													SetTargetPosition(pUser->m_Position, range);
												}
											}
											else
											{
												// ������ ����Ŭ��
												if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
												{
													// active skill ����
												}
											}
										}
										else
										{
											if(!CheckDistanceToTarget(pUser->m_Position, m_ActiveSlotID, range, 0, true))
											{
												// ���󰡱�
												if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
												{
													SetTargetPosition(pUser->m_Position, range);
												}
											}
											else
											{
												// ������ ����Ŭ��
												if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
												{
													// active skill ����
												}
											}
										}
									} // if(pUser->GetMotionClass() != _XACTION_DYING || pUser->GetMotionClass() != _XACTION_DIE)
								}
							}	// ���� ����
							else
							{
								// ���� �Ұ���

								// debug message �̹Ƿ� string table�� ���� �ʿ� �����ϴ�.
								if(m_UserState == _XDEF_USERSTATE_MATCH_FIGHT)
								{
									_XDWINPRINT("���ݺҰ��� Ÿ��(����Ŭ��) : localuserstate[����]  matchtargetid[%d] userid[%d]", m_UserState, m_MatchTargetID, pUser->m_Uniqueid);
								}
								else if(m_UserState == _XDEF_USERSTATE_PK_READY || m_UserState == _XDEF_USERSTATE_PK_FIGHT)
								{
									_XDWINPRINT("���ݺҰ��� Ÿ��(����Ŭ��) : localuserstate[PK��] userid[%d]", m_UserState, pUser->m_Uniqueid);
								}

								// pk check
								SetPKReadyMode();

								bool jump = false;
								if( m_PersonalStoreState == _XUSER_PERSONALSTORE_NONE )
								{
									// PC�� ���� ������ ������ ��� 
									if( pUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
									{
										jump = true;
										if(distance > 1.0f ) // 1.0f �ȿ� ���;� �ŷ��� �� �� �ִ�.
										{
											// PC������ �̵�
											D3DXVECTOR3 directionvector = m_Position - pUser->m_Position;
											D3DXVec3Normalize(&directionvector, &directionvector);
											
											D3DXVECTOR3 targetposition = pUser->m_Position + (directionvector);
											targetposition.y = pUser->m_Position.y;
											SetTargetPositionAngle(targetposition);
											InsertTargetPositionEffect();
										}
										else
										{
											if((g_LocalSystemTime - m_PrevRequestPacketSendedTime) > 1000)
											{		
												g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_shopitem_list_req,pUser->m_Uniqueid);
												m_PrevRequestPacketSendedTime  = g_LocalSystemTime;
											}

											_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
											if( pPersonalStore_Window )
											{
												pPersonalStore_Window->m_OwnerUniqueID = pUser->m_Uniqueid;
												memset(pPersonalStore_Window->m_OwnerName, 0, sizeof(TCHAR)*_XDEF_MAX_USERNAMESTRINGLENGTH);
												memset(pPersonalStore_Window->m_PersonalStoreName, 0, sizeof(TCHAR)*37);
												strcpy( pPersonalStore_Window->m_OwnerName, pUser->m_CharacterInfo.charactername);
												strcpy( pPersonalStore_Window->m_PersonalStoreName, pUser->m_PersonalStoreName );
											}
										}
									} // ���λ���
								}
								if( !jump )
								{
									if(distance > range)
									{
										
										D3DXVECTOR3 directionvector = m_Position - pUser->m_Position;
										D3DXVec3Normalize(&directionvector, &directionvector);
										
										D3DXVECTOR3 targetposition = pUser->m_Position + (directionvector*(range*0.7f));
										targetposition.y = pUser->m_Position.y;
										SetTargetPositionAngle(targetposition);
										InsertTargetPositionEffect();
									}
									else
									{
										m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pUser->m_Position);
										UpdatePosition();
										
										if(pUser->GetMotionClass() == _XACTION_DYING)
											ProcessRescueUser();
									}
								}
							} // ���� �Ұ���							
						}
						else
						{
							// �ٸ� Ÿ��
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : ����� //breif : ��Ʈ�� shift Ŭ��
							m_bGuardMob =false;
#endif
						}
					} // else if(m_ViewTargetType == _XPICKINGTARGET_PC)
					else if(m_ViewTargetType == _XPICKINGTARGET_MOB)
					{
						XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;	// hotblood zerolevel
						if( pZeroLevelProcess->GetCurrentTutorial() == 3 )
						{
							pZeroLevelProcess->SetTutorialModeOn(FALSE);
						}

						if(m_ViewTarget == g_LastPickedObject)
						{
							// �ι�° Ŭ��
							_XMob* pMob = (_XMob*)g_LastPickedObject;


							// ����ȿ������ �Ǽ� ���� �Ұ�
							if(m_Lock.battlelock == 1 && m_SelectedSkillID == 0)
							{
								// GetTargetPositionIdleMode()�� �����ϰ� ����
								FLOAT range = m_AttackRange + g_MobAttribTable[pMob->m_MobType].collideradius;
								FLOAT dx = m_Position.x - pMob->m_Position.x;
								FLOAT dz = m_Position.z - pMob->m_Position.z;
								FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);
								
								if(distance > range)
								{
									
									D3DXVECTOR3 directionvector = m_Position - pMob->m_Position;
									D3DXVec3Normalize(&directionvector, &directionvector);
									
									D3DXVECTOR3 targetposition = pMob->m_Position + (directionvector*(range*0.7f));
									targetposition.y = pMob->m_Position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									UpdatePosition();								
								}

								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_2736), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //���� �̻����� ���Ͽ� ������ �Ұ��� �մϴ�.
							}
							else
							{
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : ����� //breif : ��Ʈ�� shift Ŭ��
								if(g_MobAttribTable[pMob->m_MobType].scripttype >= 91 &&
								   g_MobAttribTable[pMob->m_MobType].scripttype <= 95 )
								{
									if(pMob->m_Action.action != _XMOBACTION_DIE)
									{
										if( gpInput->CheckKeyPress( DIK_LSHIFT ) )
											m_bGuardMob = true;

										if( pMob->m_Action.action == _XMOBACTION_ATTACK || m_bGuardMob)
										{
											FLOAT range = 0.0f;

											if(m_AttackTarget != g_LastPickedObject)
											{
												SetAttackTarget(pMob, _XPICKINGTARGET_MOB);
												
												if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
												{
													// �� ���󰡱�
													if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
													{
														SetTargetPosition(pMob->m_Position, range);
													}
												}
												else
												{
													// �� ������ ���� Ŭ��
													if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
													{
														// active skill ����
													}
												}
											}
											else
											{
												{
													if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
													{
														// �� ���󰡱�
														if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
														{
															SetTargetPosition(pMob->m_Position, range);
														}
													}
													else
													{
														// �� ������ ���� Ŭ��
														if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
														{
															// active skill ����
														}
													}
												}
											}
										}								
									}
								}
								else
								{
#endif
									if(pMob->m_Action.action != _XMOBACTION_DIE)
									{
										FLOAT range = 0.0f;

										if(m_AttackTarget != g_LastPickedObject)
										{
											SetAttackTarget(pMob, _XPICKINGTARGET_MOB);
											
											if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
											{
												// �� ���󰡱�
												if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
												{
													SetTargetPosition(pMob->m_Position, range);
												}
											}
											else
											{
												// �� ������ ���� Ŭ��
												if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
												{
													// active skill ����
												}
											}
										}
										else
										{
											{
												if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
												{
													// �� ���󰡱�
													if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
													{
														SetTargetPosition(pMob->m_Position, range);
													}
												}
												else
												{
													// �� ������ ���� Ŭ��
													if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
													{
														// active skill ����
													}
												}
											}
											
										}
									} // if(pMob->m_Action.action != _XMOBACTION_DIE)
#ifdef _XDEF_NPC_ATTACK_CHECK
								}
#endif
							}
						} // if(m_ViewTarget == g_LastPickedObject)
						else
						{
							// �ٸ� Ÿ��
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : ����� //breif : ��Ʈ�� shift Ŭ��
							m_bGuardMob =false;
#endif
						}
					} // else if(m_ViewTargetType == _XPICKINGTARGET_MOB)
					else if(m_ViewTargetType == _XPICKINGTARGET_DUMMY)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// �ι�° Ŭ��
							_XDummy* pDummy = (_XDummy*)g_LastPickedObject;
							if(pDummy)
							{
								FLOAT dx = m_Position.x - pDummy->m_Position.x;
								FLOAT dz = m_Position.z - pDummy->m_Position.z;
								FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);
								FLOAT range = m_AttackRange + 1.0f;

								SetAttackTarget(pDummy, _XPICKINGTARGET_DUMMY);

								if(distance > range)
								{
									D3DXVECTOR3 directionvector = m_Position - pDummy->m_Position;
									D3DXVec3Normalize(&directionvector, &directionvector);

									D3DXVECTOR3 targetposition = pDummy->m_Position + (directionvector*range);
									targetposition.y = pDummy->m_Position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pDummy->m_Position);
									UpdatePosition();

									// ������ �����ϰڽ��ϱ�?
									if(m_UserState != _XDEF_USERSTATE_TRAINING)
									{
										if(m_CurrentBattleState == _XUSER_STATE_NONE || m_CurrentBattleState == _XUSER_STATE_LAST)
										{
											_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
											if(pDummy_Window)
											{
												if(!pDummy_Window->m_bComplete)
													g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2899), TRUE, _XDEF_TRAINING_DUMMY_START); //_T("���� ������ �����ϰڽ��ϱ�?")
											}
										}
									}
								}
							}
						}
						else
						{
							// �ٸ� Ÿ��
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : ����� //breif : ��Ʈ�� shift Ŭ��
							m_bGuardMob =false;
#endif
						}
					} //  else if(m_ViewTargetType == _XPICKINGTARGET_DUMMY)
					else if(m_ViewTargetType == _XPICKINGTARGET_SEAT)
					{
						// �ι�° Ŭ��
						if(m_ViewTarget == g_LastPickedObject)
						{
							_XSeat* pSeat = (_XSeat*)g_LastPickedObject;
							if(pSeat)
							{
								FLOAT dx = m_Position.x - pSeat->m_Position.x;
								FLOAT dz = m_Position.z - pSeat->m_Position.z;
								FLOAT distance= _XFC_sqrt(dx*dx + dz*dz);
								FLOAT range = 0.0f;

								SetAttackTarget(pSeat, _XPICKINGTARGET_SEAT);

								if(distance > range)
								{
									D3DXVECTOR3 directionvector = m_Position - pSeat->m_Position;
									D3DXVec3Normalize(&directionvector, &directionvector);

									D3DXVECTOR3 targetposition = pSeat->m_Position + (directionvector*range);
									targetposition.y = pSeat->m_Position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pSeat->m_Position);
									UpdatePosition();

									if(m_UserState != _XDEF_USERSTATE_TRAINING)
									{
										if(m_CurrentBattleState == _XUSER_STATE_NONE || m_CurrentBattleState == _XUSER_STATE_LAST)
										{
											g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2900), TRUE, _XDEF_TRAINING_SPELL_START); //_T("��� ������ �����ϰڽ��ϱ�?")
										}
									}
								}
							}
						}
						else
						{
							// �ٸ� Ÿ��
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : ����� //breif : ��Ʈ�� shift Ŭ��
							m_bGuardMob =false;
#endif
						}
					} // // else if(m_ViewTargetType == _XPICKINGTARGET_SEAT)
					else if(m_ViewTargetType == _XPICKINGTARGET_SELF)
					{
						if(m_ViewTarget != g_LastPickedObject)
						{
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : ����� //breif : ��Ʈ�� shift Ŭ��
							m_bGuardMob =false;
#endif
						}
					}
					else if( m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT )
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// �ι�° Ŭ��							
							_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_ViewTarget;
							if( pFunctionalObject->m_HealthPercentage > 0 )
							{
								int ret = pProc_Maingame->StartFunctionalObjectScript();
								
								SetAttackTarget((_XItem*)pFunctionalObject, _XPICKINGTARGET_FUNCTIONALOBJECT);
								
								if( ret == 0 )
								{
									// OBJECT ������ �̵�
									D3DXVECTOR3 position = pFunctionalObject->m_Position;
									D3DXVECTOR3 directionvector = m_Position - position;
									D3DXVec3Normalize(&directionvector, &directionvector);
									
									D3DXVECTOR3 targetposition = position + (directionvector*(g_LodTerrain.m_MeshObjectManager.m_ModelList[ pFunctionalObject->m_ObjectIndex ]->OBB_Extent[2]*2 + 2.5f));
									targetposition.y = position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									if( !pFunctionalObject->m_bAttackable )
									{
										g_CurrentFocusedObject = _XDEF_DUMMYID_FUNCTIONALOBJECT;
										SetTargetPositionAngle(m_Position);								
									}
									else
									{										
										// �� ������ ���� Ŭ��
										if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
										{
											// active skill ����
										}
									}									
								}
							}
						}
						else
						{
							// �ٸ� Ÿ��
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : ����� //breif : ��Ʈ�� shift Ŭ��
							m_bGuardMob =false;
#endif
						}
					}//else if( m_ViewTargetType = _XPICKINGTARGET_FUNCTIONALOBJECT )
				}
			}
		} // if(g_LastPickedObject)
		else if(!m_bLeftClickProcess)
		{
			if( !pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView) // 2004.06.03->oneway48 insert
			{
				if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL && 
					(m_CurrentBattleState == _XUSER_STATE_6 || m_CurrentBattleState == _XUSER_STATE_7))
				{
					// ���콺 Ŭ������ ������ �� ����
				}
				else
				{
					// �ٸ� ���� Ŭ������ ��� ������ ���� ���
					m_bGrabItem = FALSE;

#ifdef _XDEF_NPC_ATTACK_CHECK //Author : ����� //breif : ��Ʈ�� shift Ŭ��
					m_bGuardMob = false;
#endif

					ResetAttackTarget();
					pProc_Maingame->m_bAutoNPCScriptPopUp = FALSE;
					if(CheckValidNextPosition())
					{
						SetTargetPositionAngle(g_vMousePosition);
						InsertTargetPositionEffect();
					}
					else
					{
						if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE && GetMotionClass() != _XACTION_REBIRTH)
						{
							m_TargetPosition = m_Position;
						}
					}

					// ����ȿ���� �̵� �Ҵ��� ��� �޼��� ���
					if(m_Lock.movelock == 1)
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2898), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //"���� �̻����� ���Ͽ� �̵��� �Ұ��� �մϴ�."
					}
				}
			}
		}

		ProcessBattleChannelingNextStep();

	} // if(g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND && mousestate->bButton[0] && !m_bLeftClickProcess)
}

BOOL _XLocalUser::CheckValidNextPosition(void)
{
	if(((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.GetMousePosition())
	{
		// ��ŷ����Ʈ�� ����Ѵ�. ���콺�� ���� �������� ���� ����.
		g_LodTerrain.GetPickedPoint();

		if(g_vMousePosition.x > 0.0f)
		{
			if(g_vMousePosition.x > (g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingWidth))
				g_vMousePosition.x = g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingWidth;
		}
		else
		{
			if(g_vMousePosition.x < (-g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingWidth))
				g_vMousePosition.x = -g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingWidth;
		}

		if(g_vMousePosition.z > 0.0f)
		{
			if(g_vMousePosition.z > (g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingHeight))
				g_vMousePosition.z = g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingHeight;
		}
		else
		{
			if(g_vMousePosition.z < (-g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingHeight))
				g_vMousePosition.z = -g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingHeight;
		}

		FLOAT minimumdistance;
		if(m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_RUN)
			minimumdistance = _XDEF_MINIMUMRUNDISTANCE;

		FLOAT dx = g_vMousePosition.x - m_Position.x;
		FLOAT dy = g_vMousePosition.y - m_Position.y;
		FLOAT dz = g_vMousePosition.z - m_Position.z;
		FLOAT distance = _XFC_sqrt(dx*dx + dy*dy + dz*dz);

		if(distance < minimumdistance)
		{
			FLOAT ddx = g_vMousePosition.x - m_TargetPosition.x;
			FLOAT ddy = g_vMousePosition.y - m_TargetPosition.y;
			FLOAT ddz = g_vMousePosition.z - m_TargetPosition.z;
			FLOAT ddis = _XFC_sqrt(ddx*ddx + ddz*ddz + ddy*ddy);

			if(ddis < _XDEF_MINIMUMRUNDISTANCE && GetMotionClass() == _XACTION_MOVE)
			{
			}
			else
			{
				m_PrevMoveTime = 0;
				return TRUE; // Change 2005/11/15 kerygma FALSE -> TRUE ///////????
			}
		}

		return TRUE;
	}

	return FALSE;
}

void _XLocalUser::ProcessNextMovePosition(void)
{
	if( GetMotionClass () == _XACTION_DIE || GetMotionClass () == _XACTION_DYING )
		return;
	
	// position�� targetposition�� ����
	
	if( GetMotionClass() == _XACTION_MOVE &&
		(m_Position.x == m_TargetPosition.x && m_Position.z == m_TargetPosition.z) )   // Ÿ�� �����ǰ� ��ǥ�� ������ ���� �Ǵ� ���� �н� �̵� 
	{
		if(!gpInput->GetMouseState()->bButton[0])
		{
			if(m_LastRotateAngle == m_RotateAngle && !m_UseSpeedSkill)
			{
				if( m_PathNodeCount > 0 ) // �н��� ���Ǿ��ٸ� 
				{
					if( m_CurrentNavigatePathNodeIndex < m_PathNodeCount-1 ) // ���� �н� ����Ʈ�� �ִٸ� ���� ����Ʈ �����ϰ� ��� �̵�
					{
						m_CurrentNavigatePathNodeIndex++;
						
						if( m_CurrentNavigatePathNodeIndex == m_PathNodeCount-1 )
						{
							m_TargetPosition.x = m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.x;
							m_TargetPosition.z = m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.y;
						}
						else
						{
							m_TargetPosition.x = ( m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.x + g_PathFinder.m_CollideArrayStartPosition.x) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
							m_TargetPosition.z = ( m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.y + g_PathFinder.m_CollideArrayStartPosition.y) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
						}
						
						m_LastRotateAngle = _XMeshMath_GetRotateRadian(m_Position, m_TargetPosition);
						
						FLOAT elapsedangle_1 = fabs(m_LastRotateAngle - m_RotateAngle);
						FLOAT elapsedangle_2 = (_X_PI - fabs(m_LastRotateAngle)) + (_X_PI - fabs(m_RotateAngle));
						
						if(elapsedangle_1 <= elapsedangle_2)
						{
							if(m_LastRotateAngle >= m_RotateAngle)
								m_RotateDirection = _XROTATE_CLOCKWISE;
							else
								m_RotateDirection = _XROTATE_COUNTCLOCKWISE;
						}
						else
						{
							if(m_LastRotateAngle >= m_RotateAngle)
								m_RotateDirection = _XROTATE_COUNTCLOCKWISE;
							else
								m_RotateDirection = _XROTATE_CLOCKWISE;
						}
					}
					else
					{
						UpdatePosition();
						SetAnimation(_XACTION_IDLE);
//						_XDWINPRINT("[LocalUser/ProcessNextMovePosition1] Send Move Pakcet");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				}
				else
				{
					UpdatePosition();

					if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL || m_SelectedSkillID == 0)
					{
						if(m_CurrentBattleState != _XUSER_STATE_NONE && m_CurrentBattleState != _XUSER_STATE_LAST)
						{
							// motion change ���� ����
						}
						else
						{
							SetAnimation(_XACTION_IDLE);
//							_XDWINPRINT("[LocalUser/ProcessNextMovePosition2] Send Move Pakcet");
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						}
					}
					else
					{
						SetAnimation(_XACTION_IDLE);
//						_XDWINPRINT("[LocalUser/ProcessNextMovePosition2] Send Move Pakcet");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				}
				
				if(g_pMinimap_Window)
				{
					g_pMinimap_Window->SetRotateArrow(m_RotateAngle + _X_PI);
					g_pMinimap_Window->SetLocalUserPosition();
				}
			}			
			else 
			{			
				UpdateRotateAngle();
				UpdatePosition();
			}
		} // if(GetMotionClass() == _XACTION_MOVE)
		else
		{
			UpdatePosition();
			
			if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL || m_SelectedSkillID == 0)
			{
				if(m_CurrentBattleState != _XUSER_STATE_NONE && m_CurrentBattleState != _XUSER_STATE_LAST)
				{
					// motion change ���� ����
				}
				else
				{
					SetAnimation(_XACTION_IDLE);
//					_XDWINPRINT("[LocalUser/ProcessNextMovePosition2] Send Move Pakcet");
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				}
			}
			else
			{
				SetAnimation(_XACTION_IDLE);
//				_XDWINPRINT("[LocalUser/ProcessNextMovePosition2] Send Move Pakcet");
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
	}
	
	if( m_Position.x != m_TargetPosition.x || m_Position.z != m_TargetPosition.z ) // Ÿ�� �����ǰ� ��ǥ�� �ٸ��� �̵� ó��
	{
		D3DXVECTOR3 nextposition = m_Position;
		if(!GetNextMove(nextposition))
		{
			if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL && (m_CurrentBattleState == _XUSER_STATE_6 || m_CurrentBattleState == _XUSER_STATE_7))
			{
				// finish skill�� - �ִϸ��̼� �������� ����
			}
			else
			{
				if(GetMotionClass() != _XACTION_IDLE)
				{
					// �� �̻� �� �� ����
					SetAnimation(_XACTION_IDLE);
//					_XDWINPRINT("============= GetNextMove Fail : Send Move packet");
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				}
			}
		}
		// �̵����� ��� ���� position�� ����
		if(GetMotionClass() == _XACTION_MOVE)
		{
			// Target�� �־��� ��� Target ����
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_NPC)
				{
					_XNPCObject* pNPC = (_XNPCObject*)m_AttackTarget;
					if(pNPC->m_Distance > _XDEF_NPC_CULLINGRADIUS)
					{
						ResetAttackTarget();
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_AttackTarget;
					if(pUser->m_ModelDescriptor.m_CameraDistance > _XDEF_USER_CULLINGRADIUS)
					{
						ResetAttackTarget();
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					if(pMob->m_ModelDescriptor.m_CameraDistance > g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_MOB_CULLINGRADIUS)
					{
						ResetAttackTarget();
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
					if(pFunctionalObject->m_ModelDescriptor.m_CameraDistance > g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_MOB_CULLINGRADIUS)
					{
						ResetAttackTarget();
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_DUMMY)
				{
					_XDummy* pDummy = (_XDummy*)m_AttackTarget;
					if(pDummy->m_ModelDescriptor.m_CameraDistance > _XDEF_USER_CULLINGRADIUS)
					{
						ResetAttackTarget();
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_SEAT)
				{
					_XSeat* pSeat = (_XSeat*)m_AttackTarget;
					if(pSeat->m_CameraDistance > _XDEF_USER_CULLINGRADIUS)
					{
						ResetAttackTarget();
					}
				}
			}
			if(m_ViewTarget)
			{
				if(m_ViewTargetType == _XPICKINGTARGET_NPC)
				{
					_XNPCObject* pNPC = (_XNPCObject*)m_ViewTarget;
					if(pNPC->m_Distance > _XDEF_NPC_CULLINGRADIUS)
					{
						ResetViewTarget();
					}
				}
				else if(m_ViewTargetType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_ViewTarget;
					if(pUser->m_ModelDescriptor.m_CameraDistance > _XDEF_USER_CULLINGRADIUS)
					{
						ResetViewTarget();
					}
				}
				else if(m_ViewTargetType == _XPICKINGTARGET_MOB)
				{
					_XMob* pMob = (_XMob*)m_ViewTarget;
					if(pMob->m_ModelDescriptor.m_CameraDistance > g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_MOB_CULLINGRADIUS)
					{
						ResetViewTarget();
					}
				}
				else if(m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_ViewTarget;
					if(pFunctionalObject->m_ModelDescriptor.m_CameraDistance > g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_MOB_CULLINGRADIUS)
					{
						ResetViewTarget();
					}
				}
				else if(m_ViewTargetType == _XPICKINGTARGET_DUMMY)
				{
					_XDummy* pDummy = (_XDummy*)m_ViewTarget;
					if(pDummy->m_ModelDescriptor.m_CameraDistance > _XDEF_USER_CULLINGRADIUS)
					{
						ResetViewTarget();
					}
				}
				else if(m_ViewTargetType == _XPICKINGTARGET_SEAT)
				{
					_XSeat* pSeat = (_XSeat*)m_ViewTarget;
					if(pSeat->m_CameraDistance > _XDEF_USER_CULLINGRADIUS)
					{
						ResetViewTarget();
					}
				}
			}

			if( g_FreeMoveMode )
			{
				m_Position.x = nextposition.x;
				if(m_Position.y < nextposition.y )
					m_Position.y = nextposition.y;
				m_Position.z = nextposition.z;
			}
			else
			{
				m_Position = nextposition;
			}

			UpdateRotateAngle();
			UpdatePosition();

			if(g_pMinimap_Window)
			{
				g_pMinimap_Window->SetRotateArrow(m_RotateAngle + _X_PI);
				g_pMinimap_Window->SetLocalUserPosition();
			}

			// ����� ����ϸ� 0.3�ʿ� �ѹ��� ��Ŷ �����ϵ��� ����
			if(m_UseSpeedSkill)
			{
				if((g_LocalSystemTime - m_PrevPacketSendedTime) >= 300)
				{
					if(GetMotionClass() == _XACTION_MOVE && m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG)
					{
//						_XDWINPRINT(" SEND MOVE PACKET [LocalUSer->ProcessBattle] -use speed skill 0.3 Delay");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				}
			}
			else
			{
				// 0.5�ʸ��� �ѹ��� ��Ŷ ����
//				if(g_GameTimer.FrameSkip(XTIMER_SENDMOVEMESSAGE, XTIMER_SENDMOVEMESSAGETIME))
				if((g_LocalSystemTime - m_PrevPacketSendedTime) >= 500)
				{
					if(GetMotionClass() == _XACTION_MOVE)
					{
//						_XDWINPRINT(" SEND MOVE PACKET [LocalUSer->ProcessBattle] 0.5 Delay");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				}
			}

			if(!g_QuestScriptManager.GetQuestCameraMode() )
			{
				g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(m_Position.x, 
																	m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, 
																	m_Position.z));
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel();
			}
		}
		else if(GetMotionClass() == _XACTION_DYING)
		{
			m_Position = nextposition;
			UpdatePosition();
			
			if(g_pMinimap_Window)
			{
				g_pMinimap_Window->SetRotateArrow(m_RotateAngle + _X_PI);
				g_pMinimap_Window->SetLocalUserPosition();
			}

			g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(m_Position.x, 
																m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, 
																m_Position.z));
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel();
		}
		else if(GetMotionClass() == _XACTION_ATTACK)
		{
			if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL)
			{
				if(m_CurrentBattleState == _XUSER_STATE_6 || m_CurrentBattleState == _XUSER_STATE_7)
				{
					m_Position = nextposition;
					UpdatePosition();
					
					if(g_pMinimap_Window)
					{
						g_pMinimap_Window->SetRotateArrow(m_RotateAngle + _X_PI);
						g_pMinimap_Window->SetLocalUserPosition();
					}
					
					g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(m_Position.x, 
						m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, 
						m_Position.z));
					g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
					g_LodTerrain.RebuildLevel();
				}
			}
		}
	}
	else
	{
		if(GetMotionClass() == _XACTION_MOVE)
		{
			// ����ó�� - ���ڸ� �ٱ� ��
			SetAnimation(_XACTION_IDLE);
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
	}
}

// by mahwang
void _XLocalUser::ProcessCollectEvent(void)
{
	XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	POINT mousept = { gpInput->GetMouseVirScreenPos()->x, gpInput->GetMouseVirScreenPos()->z };
	pMainGameProcess->GetVectorFromMousePos( mousept, g_vPickRayDir, g_vPickRayOrig );

	g_LodTerrain.m_MeshObjectManager.IntersectionEventObject(g_vPickRayOrig, g_vPickRayDir);

}
void _XLocalUser::ProcessAnimation(void)
{
	if(GetMotionClass() == _XACTION_IDLE)
	{
		// Emotion
		if( m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_CRINGESTART )
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimationIndex(_XAN_EMOTION_CRINGEREPEAT,FALSE);// Repeate
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else if( 
#ifdef ALL_RB_REMOVEWEAPON_EMOTIONANI_070622_KERYGMA
				 m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_BOW ||
				 m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_BOWPOLITELY ||
#endif
				 (m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
				  m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY ) )
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				ReloadWeaponModel();
				SetAnimation( _XACTION_IDLE );
			}			
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_KNEE_SWORD_START)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimationIndex(_XAN_KNEE_SWORD_REPEAT, TRUE);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART ||
			m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNREPEAT ||
			m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNEND ||
			m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_CRINGEREPEAT ||
			m_ModelDescriptor.GetCurMotion() == _XAN_KNEE_SWORD_REPEAT ||
			m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_REPEAT)
		{
			// ó�� �ϸ� �ȵ� - ���� ��
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_START)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				m_ModelDescriptor.SetLockEndFrame(FALSE);
				SetAnimationIndex(_XAN_MATCH_PR_REPEAT, TRUE);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_END)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, NULL);
				ReloadWeaponModel();
				SetAnimation( _XACTION_IDLE );
			}
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_WINNER_START)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimationIndex(_XAN_MATCH_WINNER_REPEAT);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_LOSER_START)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimationIndex(_XAN_MATCH_LOSER_REPEAT);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else 
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
#ifdef _XDEF_NEWANIMATIONLIST
				/*
				if( m_SelectedSkillID )
				{
					if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
						_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF	&&
						g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
					{
						int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
											  g_SkillProperty[m_SelectedSkillID]->motion_male : 
											  g_SkillProperty[m_SelectedSkillID]->motion_female;
						
						if( m_CharacterInfo.modeldescript.item1 == 0 )
						{
						}
						else
						{
							_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
							
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
								pWeaponItemProperty = g_WeaponItemProperty;
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
								pWeaponItemProperty = g_WeaponItemProperty2;
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
								pWeaponItemProperty = g_WeaponItemProperty3;
							
							switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
							{
							case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
								if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
								{
								}
								else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
									else
										baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
								}
								else
								{
									baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
								}
								break;
							case _XGI_SC_WEAPON_LONG				:				// �庴��
								baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
								break;				
							case _XGI_SC_WEAPON_SOFT				:				// ������
								baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
								break;
							case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
								break;
							case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
								break;
							case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
								break;
							}
						}
						
						if( m_ModelDescriptor.m_CurMotion != baseanimindex+2 ) // ��� ������ ���� 
						{
							SetAnimation( _XACTION_IDLE );
						}
					}
					{
						SetAnimation( _XACTION_IDLE );
					}
				}
				else
				*/
#endif
				{
					SetAnimation( _XACTION_IDLE );
				}
			}
		}		

		if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
		{
			if(m_SpecialActionTimer == 0)
				m_SpecialActionTimer = g_LocalSystemTime;

			if((g_LocalSystemTime - m_SpecialActionTimer) > _XDEF_CHANGEIDLEMOTIONTIME)
			{
				m_SpecialActionTimer = g_LocalSystemTime;

				switch(m_ModelDescriptor.GetCurMotion())
				{
				case _XAN_IDLE_NONE :
					{
						SetAnimationIndex(_XAN_IDLE_NONE_ACTION, TRUE);
					}
					break;				
				case _XAN_IDLE_SHORTWEAPON :
					{
						SetAnimationIndex(_XAN_IDLE_SHORTWEAPON_ACTION, TRUE);
					}
					break;				
				case _XAN_IDLE_LONGWEAPON :
					{
						SetAnimationIndex(_XAN_IDLE_LONGWEAPON_ACTION, TRUE);
					}
					break;
				case _XAN_IDLE_SWORD_PEACE :
					{
						SetAnimationIndex(_XAN_IDLE_SWORD_ACTION, TRUE);
					}
					break;
				}
			}

			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				switch(m_ModelDescriptor.GetCurMotion())
				{
				case _XAN_IDLE_NONE_ACTION :
					{
						SetAnimation(_XACTION_IDLE);
					}
					break;
				case _XAN_IDLE_SHORTWEAPON_ACTION :
					{
						SetAnimation(_XACTION_IDLE);
					}
					break;				
				case _XAN_IDLE_LONGWEAPON_ACTION :
					{
						SetAnimation(_XACTION_IDLE);
					}
					break;
				case _XAN_IDLE_SWORD_ACTION :
					{
						SetAnimation(_XACTION_IDLE);
					}
					break;
				}
			}
		}

		// display ���� ���� ���� ��� ó��
		ProcessMobDie();
	}
	else if(GetMotionClass() == _XACTION_MOVE)
	{
		if(m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG)
		{
#ifdef _XDEF_NEWANIMATIONLIST

			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{

				int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
									  g_SkillProperty[m_SelectedSkillID]->motion_male : 
									  g_SkillProperty[m_SelectedSkillID]->motion_female;


				if( m_CharacterInfo.modeldescript.item1 == 0 )
				{
				}
				else
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
							{
							case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
								if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
								{
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
									else
										baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
								}
								else
								{
									baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
								}
								break;
							case _XGI_SC_WEAPON_LONG				:				// �庴��
								baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
								break;				
							case _XGI_SC_WEAPON_SOFT				:				// ������
								baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
								break;
							case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
								break;
							case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
								break;
							case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
								break;
							}
						}
					}
				}
#else
					  _XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
					  
					  if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
						  pWeaponItemProperty = g_WeaponItemProperty;
					  else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
						  pWeaponItemProperty = g_WeaponItemProperty2;
					  else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
						  pWeaponItemProperty = g_WeaponItemProperty3;
					  
					  switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					  {
					  case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						  if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						  {
						  }
						  else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						  {
							  if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								  baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
							  else
								  baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
						  }
						  else
						  {
							  baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
						  }
						  break;
					  case _XGI_SC_WEAPON_LONG				:				// �庴��
						  baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
						  break;				
					  case _XGI_SC_WEAPON_SOFT				:				// ������
						  baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
						  break;
					  case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						  break;
					  case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						  break;
					  case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						  break;
					  }
				}
#endif
				if( m_ModelDescriptor.m_CurMotion == baseanimindex )
				{
					SetAnimationIndex(m_ModelDescriptor.m_CurMotion+1);// Repeate
				}
				//else if( m_ModelDescriptor.m_CurMotion == baseanimindex+1 )
				//			{
				//
				//			}			
				else if( m_ModelDescriptor.m_CurMotion == baseanimindex+2 )
				{
					SetAnimation(_XACTION_IDLE);
					m_UseSpeedSkill = FALSE;
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
					m_TargetPosition = m_Position;
					
					if( m_SelectedSkillID )
					{
						if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
							_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF	&&
							g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
						{
							m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
							m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);
						}
					}
				}
			
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}

#else //_XDEF_NEWANIMATIONLIST
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				switch(m_ModelDescriptor.m_CurMotion)
				{
				case _XAN_FASTMOVE_NONE_START		 :
				case _XAN_FASTMOVE_SHORTWEAPON_START :
				case _XAN_FASTMOVE_LONGWEAPON_START  :
				case _XAN_FASTMOVE_SOFTWEAPON_START  :
				case _XAN_FASTMOVE_SWORD_START_PEACE :
				case _XAN_FASTMOVE_SWORD_START_BATTLE :
					{
						SetAnimationIndex(m_ModelDescriptor.m_CurMotion+1);// Repeate
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				break;
				/*
				case _XAN_FASTMOVE_NONE_REPEAT		 :
				case _XAN_FASTMOVE_SHORTWEAPON_REPEAT:
				case _XAN_FASTMOVE_LONGWEAPON_REPEAT :
				case _XAN_FASTMOVE_SOFTWEAPON_REPEAT :
					{
					}
				break;*/

				case _XAN_FASTMOVE_NONE_END			 :
				case _XAN_FASTMOVE_SHORTWEAPON_END	 :
				case _XAN_FASTMOVE_LONGWEAPON_END	 :
				case _XAN_FASTMOVE_SOFTWEAPON_END	 :
				case _XAN_FASTMOVE_SWORD_END_PEACE :
				case _XAN_FASTMOVE_SWORD_END_BATTLE :
					{
						SetAnimation(_XACTION_IDLE);
						m_UseSpeedSkill = FALSE;
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
						m_TargetPosition = m_Position;
						
						if( m_SelectedSkillID )
						{
							if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
								_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
								g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
							{
								m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
								m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
							}
						}
					}
					break;
				}
			}
#endif //_XDEF_NEWANIMATIONLIST
		}
		else if(m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_RUN)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				switch(m_ModelDescriptor.m_CurMotion)
				{
				case _XAN_MOVE_NONE_START		 :
				case _XAN_MOVE_SHORTWEAPON_START :
				case _XAN_MOVE_LONGWEAPON_START  :
				case _XAN_MOVE_SOFTWEAPON_START  :
					{
						SetAnimationIndex(m_ModelDescriptor.m_CurMotion+1);// Repeate						
					}
					break;
					
				/*case _XAN_MOVE_NONE_REPEAT		 :
				case _XAN_MOVE_SHORTWEAPON_REPEAT:
				case _XAN_MOVE_LONGWEAPON_REPEAT :
				case _XAN_MOVE_SOFTWEAPON_REPEAT :
					{						
					}
					break;*/
					
				case _XAN_MOVE_NONE_END			 :
				case _XAN_MOVE_SHORTWEAPON_END	 :
				case _XAN_MOVE_LONGWEAPON_END	 :
				case _XAN_MOVE_SOFTWEAPON_END	 :
					{
						SetAnimation(_XACTION_IDLE);						
					}
					break;
				}
			}
		}

		// display ���� ���� ���� ��� ó��
		ProcessMobDie();
	}
	else if(GetMotionClass() == _XACTION_DYING)
	{
		m_ModelDescriptor.SetLockEndFrame(TRUE);

		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			m_ModelDescriptor.SetLockEndFrame(FALSE);
			switch(m_ModelDescriptor.m_CurMotion)
			{
			case _XAN_DYINGREADY_NONE :
				{
					SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
				}
				break;
			case _XAN_DYINGREADY_SHORTWEAPON :
				{
					SetAnimationIndex(_XAN_DYINGREPEAT_SHORTWEAPON);
				}
				break;
			case _XAN_DYINGREADY_LONGWEAPON :
				{
					SetAnimationIndex(_XAN_DYINGREPEAT_LONGWEAPON);
				}
				break;
			case _XAN_DYINGREADY_SWORD_PEACE :
				SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_PEACE);
				break;
			case _XAN_DYINGREADY_SWORD_BATTLE :
				SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_BATTLE);
				break;
			}
			
			if( m_ModelDescriptor.m_EffectManager )
			{
				m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );
				m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_PLAYERID_DYINGWAIT, &m_ModelDescriptor.m_Position );
			}
		}

		// display ���� ���� ���� ��� ó��
		ProcessMobDie();
	}
	else if(GetMotionClass() == _XACTION_DIE)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
		{
			if( m_ModelDescriptor.m_EffectManager )
			{
				m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );
			}

			int rand = _XRandom() % 2;
			
			if( m_CharacterInfo.gender == _XGENDER_MAN )
				_XPlaySoundEffect( g_DieEffectSoundIndex[0][rand] , m_Position );
			else 
				_XPlaySoundEffect( g_DieEffectSoundIndex[1][rand] , m_Position );
		}

		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			m_ModelDescriptor.SetLockEndFrame(TRUE);
		}
	}
	else if(GetMotionClass() == _XACTION_RESCUE)
	{		
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if( m_ModelDescriptor.m_EffectManager )
			{
				m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );
			}
			SetAnimation(_XACTION_IDLE);
		}
		
		// display ���� ���� ���� ��� ó��
		ProcessMobDie();

	}
	else if(GetMotionClass() == _XACTION_LPS)
	{

		if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE )
		{
			// ���߿� LPS���� ó���� ���� ���� �и��Ѵ�.
			if(m_ViewTargetType == _XPICKINGTARGET_MOB)
			{
				// �ι�° Ŭ��
				_XMob* pMob = (_XMob*)m_ViewTarget;
				ProcessLPSSystem(pMob);
			}
		}

		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			SetAnimation(_XACTION_IDLE);
		}
	}		
	else if(GetMotionClass() == _XACTION_REBIRTH)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if( m_ModelDescriptor.m_EffectManager )
			{
				m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );
			}

#ifdef _XDEF_USESCREEENMOTIONBLUR 
			if(g_ScrrenMotionBlurMode)
				g_ScrrenMotionBlurMode = FALSE;
#endif
			
			SetAnimation(_XACTION_IDLE);
		}

		// display ���� ���� ���� ��� ó��
		ProcessMobDie();
	}
	else if(GetMotionClass() == _XACTION_MATCH_START)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			SetAnimation(_XACTION_IDLE);
		}
	}
	else if(GetMotionClass() == _XACTION_MATCH_CANCEL)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			SetAnimation(_XACTION_IDLE);
	}
#ifdef _XTS_NEWINSENSIBLE
	else if(GetMotionClass() == _XAN_EMOTION_CLAPPINGHAND)
	{
		// ���� - ó�� ���� ����
	}
#endif

	// by mahwang
	else if(GetMotionClass() == _XACTION_COLLECT)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if(m_CurrentWeaponID > 0)
			{
				m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);

#ifdef _XDEF_MODELTHREADLOADING
					if( WeaponItem && WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
					}
#else
					if( WeaponItem )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
#endif
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
#ifdef _XDEF_MODELTHREADLOADING
					if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );			
					}

#else
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
#ifdef _XDEF_MODELTHREADLOADING
					if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
					}
#else
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
#ifdef _XDEF_MODELTHREADLOADING
					if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex > 0  )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
					}
#else
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				}
#endif
				m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
				m_CurrentWeaponID = 0;
			}
			else
			{
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
			}

			SetAnimation(_XACTION_IDLE);
		}
	}
}

void _XLocalUser::ProcessParameter(FLOAT progresstime)
{
	// ������ parameter ó�� �� ��
	/*	_XWindow_FSWindow* pFSWindow = (_XWindow_FSWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_FSWINDOW);
	if(!pFSWindow)
	return;
	
	  if(GetMotionClass() == _XACTION_MOVE)
	  {
	  if(m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_RUN)
	  {
	  if((int)m_CharacterInfo.junggipower <= 0)
	  m_CharacterInfo.junggipower = 0.0f;
	  else
	  {
	  m_CharacterInfo.junggipower -= (_XDEF_HEALTHDECREASERATE_RUN * progresstime);
	  }
	  }
	  else
	  {
	  if((int)m_CharacterInfo.junggipower >= m_CharacterInfo.max_junggipower)
	  m_CharacterInfo.junggipower = m_CharacterInfo.max_junggipower;
	  else
	  m_CharacterInfo.junggipower += (_XDEF_HEALTHINCREASERATE_WALK * progresstime);
	  }
	  
		if((int)m_CharacterInfo.junggipower <= 0)
		{
		if(m_CharacterInfo.animationdescript.movetype != _XMOVETYPE_WALK)
		{
		m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_WALK;
		SetAnimation(_XACTION_MOVE);
		SetMoveSpeed(_XDEF_CHARACTERMOVESPEED_WALK);
		g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
		}
		pFSWindow->SetParameter();
		}
		else if(GetMotionClass() == _XACTION_IDLE)
		{
		if(m_CharacterInfo.junggipower >= (FLOAT)m_CharacterInfo.max_junggipower)
		{
		m_CharacterInfo.junggipower = m_CharacterInfo.max_junggipower;
		pFSWindow->SetParameter();
		}
		else
		{
		m_CharacterInfo.junggipower += (_XDEF_HEALTHINCREASERATE_IDLE * progresstime);
		pFSWindow->SetParameter();
		}
	}*/
}

void _XLocalUser::ProcessInternalUse(void)
{	
	switch(m_CurrentInternalUseState)
	{
	case _XUSER_STATE_1 :
		{
			SetAnimationIndex(_XAN_EMOTION_SITDOWNSTART);
			// ��� �ִ� �������� ����
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					if(WeaponItem[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
					{
						m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
						m_CharacterInfo.modeldescript.item1 = 0;
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
						m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
					}
				}
			}
#else
			if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				if(g_WeaponItemProperty[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
					m_CharacterInfo.modeldescript.item1 = 0;
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
					m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
				}
			}	
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				if(g_WeaponItemProperty2[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
					m_CharacterInfo.modeldescript.item1 = 0;
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
					m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				if(g_WeaponItemProperty3[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
					m_CharacterInfo.modeldescript.item1 = 0;
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
					m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
				}
			}
#endif
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			m_CurrentInternalUseState = _XUSER_STATE_2;			
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
				{
					SetAnimationIndex(_XAN_EMOTION_SITDOWNREPEAT, FALSE);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					if( m_bInternalUse )
						m_CurrentInternalUseState = _XUSER_STATE_3;					
					else 
						m_CurrentInternalUseState = _XUSER_STATE_5;

					g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
									_XDEF_MONSTERID_YAK, &g_pLocalUser->m_ModelDescriptor.m_Position );
				}
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			if( g_pInventory_Window )
			{
				g_pLocalUser->UseItem( g_pInventory_Window->m_ElixirInvenNumber );
				g_pInventory_Window->m_ElixirInvenNumber = -1;				
				m_ElixirSoundRepeatTime = g_LocalSystemTime;
				_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_RUN_WAV);
			}
		}
		break;
	case _XUSER_STATE_4 :
		{
			if( g_LocalSystemTime - m_ElixirSoundRepeatTime > 2000 )
				_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_RUN_WAV);
		}
		break;
	case _XUSER_STATE_5 :
		{
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_MONSTERID_YAK);

			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNREPEAT || m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					SetAnimationIndex(_XAN_EMOTION_SITDOWNEND);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CurrentInternalUseState = _XUSER_STATE_6;
				}
			}
			else
				m_CurrentInternalUseState = _XUSER_STATE_6;
		}
		break;
	case _XUSER_STATE_6 :
		{
			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNEND)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					// ��� �ִ� ���� ����
					ReloadWeaponModel();
					SetAnimation(_XACTION_IDLE);
					m_CurrentInternalUseState = _XUSER_STATE_NONE;
					m_bInternalUse = FALSE;
				}
			}
			else
			{
				m_bInternalUse = FALSE;
				m_CurrentInternalUseState = _XUSER_STATE_NONE;
			}
		}
		break;
	case _XUSER_STATE_7 :
		{	
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_MONSTERID_YAK);
			
			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNREPEAT || m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					SetAnimationIndex(_XAN_ELIXIR_FAIL);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CurrentInternalUseState = _XUSER_STATE_8;
				}
			}
			else
				m_CurrentInternalUseState = _XUSER_STATE_8;
		}
		break;
	case _XUSER_STATE_8 :
		{
			if(m_ModelDescriptor.GetCurMotion() == _XAN_ELIXIR_FAIL )
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					SetAnimationIndex(_XAN_EMOTION_SITDOWNEND);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CurrentInternalUseState = _XUSER_STATE_6;
				}
			}
			else
				m_CurrentInternalUseState = _XUSER_STATE_6;
		}
		break;
	}
}

void _XLocalUser::ProcessReadBook(void)
{
	switch(m_CurrentReadBookState)
	{
	case _XUSER_STATE_1 :
		{
			SetAnimationIndex(_XAN_EMOTION_SITDOWNSTART);
			DeleteWeaponModel();
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			m_CurrentReadBookState = _XUSER_STATE_2;			
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
				{
					SetAnimationIndex(_XAN_EMOTION_SITDOWNREPEAT, FALSE);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					if( m_bReadingBook )
						m_CurrentReadBookState = _XUSER_STATE_3;					
					else 
						m_CurrentReadBookState = _XUSER_STATE_5;
				}
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			if( g_pInventory_Window )
			{
				g_pLocalUser->UseItem( g_pInventory_Window->m_BookInvenNumber );
				g_pInventory_Window->m_BookInvenNumber = -1;
			}
		}
		break;
	case _XUSER_STATE_4 :
		{
		}
		break;
	case _XUSER_STATE_5 :
		{
			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNREPEAT || m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					SetAnimationIndex(_XAN_EMOTION_SITDOWNEND);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CurrentReadBookState = _XUSER_STATE_6;
				}
			}
			else
				m_CurrentReadBookState = _XUSER_STATE_6;
		}
		break;
	case _XUSER_STATE_6 :
		{
			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNEND)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					// ��� �ִ� ���� ����
					ReloadWeaponModel();
					SetAnimation(_XACTION_IDLE);
					m_CurrentReadBookState = _XUSER_STATE_NONE;
					m_bReadingBook = FALSE;
				}
			}
			else
			{
				m_bReadingBook = FALSE;
				m_CurrentReadBookState = _XUSER_STATE_NONE;
			}
		}
		break;
	}
}

void _XLocalUser::ProcessSpeedSkill(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
	{
		m_CurrentBattleState = _XUSER_STATE_NONE;
		m_HiperRunFailedFlag = TRUE;
		return;
	}

	if(!g_QuestScriptManager.GetQuestCameraMode())
	{
		XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;		
		if( !pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
		{
			if( gpInput->CheckKeyPress(DIK_LMENU) && 
				!m_HiperRunFailedFlag && 
				pProc_Maingame->SelectSkill( m_SelectedSkillID ) )
			{
				

				if( m_CharacterInfo.Get_current_forcepower() <= 0 )
				{
					if( m_UseSpeedSkill )
					{
						ResetSkillAnimation();
						m_UseSpeedSkill = FALSE;
						m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
						SetAnimation(_XACTION_IDLE);
//						_XDWINPRINT("============= ��� �� ");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						
						if( m_SelectedSkillID )
						{
							if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
								_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
								g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
							{
								m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
								m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
							}
						}
					}
				}
				// Check hiper run cooldown time
				else if( m_SelectedSkillItem->m_dwCooldownStartTime > 0 )
				{
					if( m_UseSpeedSkill )
					{
						ResetSkillAnimation();
						m_UseSpeedSkill = FALSE;
						m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
						SetAnimation(_XACTION_IDLE);
//						_XDWINPRINT("============= ��� �� ");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				}
				else if( m_bReadingBook )	// ��� �д� ���̸� ����
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2901), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 ); //"����� �д� �߿��� ���(�Ź�)�� ����� �� �����ϴ�."
				}
				else if( m_bSocketPlugIn )	// ���� ���� ���̸� ����
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2902), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 ); //"��Ĺ��� �ٴ� �߿��� ���(�Ź�)�� ����� �� �����ϴ�."
				}
				else if( m_bGambleBoxOpen )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2903), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 ); //"�пձ˸� ���� �߿��� ���(�Ź�)�� ����� �� �����ϴ�."
				}
				else if( g_pLocalUser->m_bInternalUse )	// ���� ���� ���̸� ����
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2904), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 ); //"���ູ�� �߿��� ���(�Ź�)�� ����� �� �����ϴ�."
				}
				else
				{
					if(g_LodTerrain.m_3PCamera.mp_fFov == g_DefaultCameraFov)
					{
						if( !m_UseSpeedSkill )//m_CharacterInfo.animationdescript.movetype != _XMOVETYPE_KYUNGGONG)
						{
							m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_KYUNGGONG;
							m_UseSpeedSkill = TRUE;

							if( m_SelectedSkillID && m_SelectedSkillItem )
							{
								if( m_SelectedSkillItem->m_cSkillLevel >= 11 ) // 11�� �̻��϶��� Radial blur effect ����
								{
									g_ObjectBlurProcessManager.SetBlurStep( 1 );
								}
							}
						}

						g_PrevMoveType = m_CharacterInfo.animationdescript.movetype;
						SetAttackTarget(NULL, _XPICKINGTARGET_NONE);
						g_LastPickedObject = NULL;
						g_LastPickedObjectType = _XPICKINGTARGET_NONE;
						SetAnimation(_XACTION_MOVE);
//						_XDWINPRINT("============= ��� ���� ");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
					if(g_LodTerrain.m_3PCamera.mp_fFov < _XDEF_MAXFOV)
					{
						g_LodTerrain.m_3PCamera.mp_fFov += _XDEF_FOVINCREASERATE;

						if( m_SelectedSkillID && m_SelectedSkillItem )
						{
							if( m_SelectedSkillItem->m_cSkillLevel >= 11 ) // 11�� �̻��϶��� Radial blur effect ����
							{
								if( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps < 30 )
								{
									if( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps + 2 <= 30 )
									{
										g_ObjectBlurProcessManager.SetBlurStep( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps + 2 );
									}
									else
									{
										g_ObjectBlurProcessManager.SetBlurStep( 30 );
									}
								}
								else
								{
									g_ObjectBlurProcessManager.SetBlurStep( 30 );
								}
							}
						}
					}

					g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);

					/*
					if(m_UseSpeedSkill)
					{
						if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
						{
							switch(m_ModelDescriptor.m_CurMotion)
							{
							case _XAN_MOVE_NONE_START :
								{
									SetAnimationIndex(_XAN_MOVE_NONE_REPEAT);
									g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
								}
								break;
							case _XAN_MOVE_NONE_REPEAT :
								{
								}
								break;
							case _XAN_MOVE_SHORTWEAPON_START :
								{
									SetAnimationIndex(_XAN_MOVE_SHORTWEAPON_REPEAT);
				
									g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
								}
								break;
							case _XAN_MOVE_SHORTWEAPON_REPEAT :
								{
								}
								break;
							case _XAN_MOVE_LONGWEAPON_START :
								{
									SetAnimationIndex(_XAN_MOVE_LONGWEAPON_REPEAT);
									g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
								}
								break;
							case _XAN_MOVE_LONGWEAPON_REPEAT :
								{
								}
								break;
							}
						}
					}
					*/
					
					D3DXVECTOR3 targetposition = m_Position + (g_LodTerrain.m_3PCamera.m_LookVector * 3.0f);
					SetTargetPositionAngle(targetposition);
				}
			}
			else
			{
				m_CurrentBattleState = _XUSER_STATE_NONE;
				
				if(g_LodTerrain.m_3PCamera.mp_fFov > g_DefaultCameraFov)
				{

#ifdef _XDEF_NEWANIMATIONLIST			
					int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
										  g_SkillProperty[m_SelectedSkillID]->motion_male : 
										  g_SkillProperty[m_SelectedSkillID]->motion_female;
					
					if( m_CharacterInfo.modeldescript.item1 == 0 )
					{
					}
					else
					{
#ifdef _XTS_ITEM_OPTIMIZATION
						if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
						{
							_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
							if( WeaponItem )
							{
								switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
									}
									else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
										else
											baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
									}
									else
									{
										baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��
									baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// ������
									baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
									break;
								}
							}
						}
#else
						_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
						
						if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							pWeaponItemProperty = g_WeaponItemProperty;
						else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							pWeaponItemProperty = g_WeaponItemProperty2;
						else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							pWeaponItemProperty = g_WeaponItemProperty3;
						
						switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
							}
							else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
								else
									baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
							}
							else
							{
								baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��
							baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// ������
							baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
							break;
						}
#endif
					}
					
					// Alt�� ��¦ ������ ��������
					if( m_ModelDescriptor.m_CurMotion == baseanimindex ) 
					{
						D3DXVECTOR3 targetposition;
						targetposition = m_Position + (g_LodTerrain.m_3PCamera.m_LookVector * 3.0f);
						SetTargetPositionAngle(targetposition);
						m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
						m_UseSpeedSkill = FALSE;
						SetAnimation(_XACTION_IDLE);
//						_XDWINPRINT("============= ��� �� ");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						
						if( m_SelectedSkillID )
						{
							if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
								_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
								g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
							{
								m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
								m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
							}
						}
					}
					// Repeat mode ����
					else if( m_ModelDescriptor.m_CurMotion == baseanimindex+1 )
					{
						SetAnimationIndex(m_ModelDescriptor.m_CurMotion+2);
//						_XDWINPRINT("============= ��� �� ");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);

						if( m_SelectedSkillID )
						{
							if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
								_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
								g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
							{
								m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
								m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
							}
						}
					}
#else
				
					switch( m_ModelDescriptor.m_CurMotion ) 
					{
					case  _XAN_FASTMOVE_NONE_REPEAT				:
						SetAnimationIndex(_XAN_FASTMOVE_NONE_END);
						//m_UseSpeedSkill = FALSE;
						break;
					case  _XAN_FASTMOVE_SHORTWEAPON_REPEAT		:	
						SetAnimationIndex(_XAN_FASTMOVE_SHORTWEAPON_END);
						//m_UseSpeedSkill = FALSE;
						break;
					case  _XAN_FASTMOVE_LONGWEAPON_REPEAT		:
						SetAnimationIndex(_XAN_FASTMOVE_LONGWEAPON_END);
						//m_UseSpeedSkill = FALSE;
						break;
					case  _XAN_FASTMOVE_SOFTWEAPON_REPEAT		:
						SetAnimationIndex(_XAN_FASTMOVE_SOFTWEAPON_END);
						//m_UseSpeedSkill = FALSE;
						break;
					case _XAN_FASTMOVE_SWORD_REPEAT_PEACE		:
						SetAnimationIndex(_XAN_FASTMOVE_SWORD_END_PEACE);
						break;
					case _XAN_FASTMOVE_SWORD_REPEAT_BATTLE		:
						SetAnimationIndex(_XAN_FASTMOVE_SWORD_END_BATTLE);
						break;
					// Alt�� ��¦ ������ ��������
					case _XAN_FASTMOVE_NONE_START :
					case _XAN_FASTMOVE_SHORTWEAPON_START :
					case _XAN_FASTMOVE_LONGWEAPON_START  :
					case _XAN_FASTMOVE_SOFTWEAPON_START  :
					case _XAN_FASTMOVE_SWORD_START_PEACE :
					case _XAN_FASTMOVE_SWORD_START_BATTLE :
						{
							D3DXVECTOR3 targetposition;
							targetposition = m_Position + (g_LodTerrain.m_3PCamera.m_LookVector * 3.0f);
							SetTargetPositionAngle(targetposition);
							m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
							m_UseSpeedSkill = FALSE;
							SetAnimation(_XACTION_IDLE);
							
							if( m_SelectedSkillID )
							{
								if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
									_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
									g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
								{
									m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
									m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
								}
							}
						}
						break;
					}
#endif					
					g_LodTerrain.m_3PCamera.mp_fFov -= _XDEF_FOVDECREASERATE;
					if(g_LodTerrain.m_3PCamera.mp_fFov <= g_DefaultCameraFov)
					{
						g_LodTerrain.m_3PCamera.mp_fFov = g_DefaultCameraFov;
						if(m_UseSpeedSkill)
						{
							D3DXVECTOR3 targetposition;
							targetposition = m_Position + (g_LodTerrain.m_3PCamera.m_LookVector * 3.0f);
							SetTargetPositionAngle(targetposition);
//							_XDWINPRINT("============= ��� �� ");
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
							
							if( m_SelectedSkillID && m_SelectedSkillItem )
							{
								if( m_SelectedSkillItem->m_cSkillLevel >= 11 ) // 11�� �̻��϶��� Radial blur effect ����
								{
									if( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps > 3 )
									{
										g_ObjectBlurProcessManager.SetBlurStep( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps - 2 );
									}
									else
									{
										g_ObjectBlurProcessManager.SetBlurStep( 1 );
									}
								}
							}
						}
					}
					
					g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				} // if(g_LodTerrain.m_3PCamera.mp_fFov > g_DefaultCameraFov)			
			}
		} // if( !pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView ) 
	} // if(!g_QuestScriptManager.GetQuestCameraMode())
	else
	{
		m_SelectedSkillID = 0;
		m_UseSpeedSkill = FALSE;
		m_CurrentBattleState = _XUSER_STATE_NONE;
	}
}

void _XLocalUser::Render( void )
{
	if(m_StopAnimation)
	{
		m_ModelDescriptor.SetLastFrame();
	}
	else
	{
		m_ModelDescriptor.Animation();
	}

	_XParticleActionBase::bLocalUser = TRUE;
	if( g_LodTerrain.m_3PCamera.m_CurrentTargetDistance >= 1.2f )
	{
		if( m_CharacterInfo.animationdescript.motionclass != _XACTION_DIE /*&& !m_pGoundCollideOBB*/ )
		{
			if( !g_FreeMoveMode )
				m_ModelDescriptor.RenderShadow();
		}

		// Camera collide alpha blending
		if( g_LodTerrain.m_3PCamera.m_CurrentTargetDistance < 1.5f )
		{
			g_RenderAlphaBlendFlag = TRUE;
			g_RenderAlphaBlendUseAlphaChannelFlag = TRUE;
			g_LastUsedRenderStyle = NULL;
			
			FLOAT distfactor = 255.0f * ( (g_LodTerrain.m_3PCamera.m_CurrentTargetDistance - 1.2f) / 0.3f );

#ifdef _XDWDEBUG
			extern BOOL g_MouseLockFlag;
			if( !g_MouseLockFlag )
			{
				g_RenderAlphaBlendTextureFactor = ( ( ((BYTE)distfactor) <<24) | 0xFFFFFF );
			}
			else
			{
				g_RenderAlphaBlendTextureFactor = 0xFFFFFFFF;
			}
#else
			g_RenderAlphaBlendTextureFactor = ( ( ((BYTE)distfactor) <<24) | 0xFFFFFF );
#endif
			
		}
		else
		{
			if( m_UseSpeedSkill || g_ObjectBlurProcessManager.m_iNumRadialBlurSteps > 1 )
			{
				if( m_UserTroubleState == _XDEF_CONVSTATE_FURY )
				{
					g_RenderAlphaBlendFlag = TRUE;
					g_RenderAlphaBlendUseAlphaChannelFlag = TRUE;
					g_LastUsedRenderStyle = NULL;
					g_RenderAlphaBlendTextureFactor = 0xA0FFFFFF;
				}
				else
				{
					if( m_SelectedSkillID )
					{
						if( g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
							_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
							g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )	// ���
						{
							if( m_SelectedSkillItem->m_cSkillLevel >= 11 ) // 11�� �̻��϶��� Radial blur effect ���� 
							{
								g_RenderAlphaBlendFlag = TRUE;
								g_RenderAlphaBlendUseAlphaChannelFlag = TRUE;
								g_LastUsedRenderStyle = NULL;
								g_RenderAlphaBlendTextureFactor = ( ((30-g_ObjectBlurProcessManager.m_iNumRadialBlurSteps)<<27) | 0xFFFFFF );					
							}
						}
					}
				}
			}
		}

		if( !(g_FreeMoveMode && g_InnerUser) )
		{
			if( m_CharacterInfo.animationdescript.motionclass == _XACTION_BLOW )
			{	
				if( m_ModelDescriptor.GetCurFrame() >= 2.0f && m_ModelDescriptor.GetCurFrame() <= 4.0f )
				{	
					g_pCurrentSelectMaterial = &g_DamagedObjectMaterial;
					m_ModelDescriptor.RenderCharacter(this);
					g_pCurrentSelectMaterial = NULL;
				}
				else
				{			
					g_pCurrentSelectMaterial = NULL;
					m_ModelDescriptor.RenderCharacter(this);	
				}	
			}
			else if( m_CharacterInfo.animationdescript.motionclass == _XACTION_DEFENCE )
			{	
				if( m_ModelDescriptor.GetCurFrame() >= 2.0f && m_ModelDescriptor.GetCurFrame() <= 4.0f )
				{
					g_pCurrentSelectMaterial = &g_DamagedObjectMaterial;
					g_pCurrentSelectMaterial->Emissive.g = 0.8f;
					g_pCurrentSelectMaterial->Emissive.b = 0.8f;
					m_ModelDescriptor.RenderCharacter(this);
					g_pCurrentSelectMaterial->Emissive.g = 0.0f;
					g_pCurrentSelectMaterial->Emissive.b = 0.0f;
					g_pCurrentSelectMaterial = NULL;
				}
				else
				{
					g_pCurrentSelectMaterial = NULL;
					m_ModelDescriptor.RenderCharacter(this);
				}
			}
			else
			{			
				g_pCurrentSelectMaterial = NULL;
				m_ModelDescriptor.RenderCharacter(this);	
			}
		}
	}

	_XParticleActionBase::bLocalUser = FALSE;
	
	if( g_RenderAlphaBlendFlag )
	{
		g_RenderAlphaBlendFlag = FALSE;
		g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
		g_LastUsedRenderStyle = NULL;
	}

	// ���� ���� Rendering
	DrawBattle();
	
	// ĳ���� ���� ȿ�� Rendering
	DrawStateList();

#ifdef _XDWDEBUG	
	if( g_LodTerrain.m_ViewOBBObject )
	{
		gpDev->SetRenderState( D3DRS_ZENABLE, g_RS_UseZBuffer  );
		gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
		m_CharacterOBB.m_OBBModel.Render();
	}
#endif		

	// by mahwang
	if (m_UserState == _XDEF_USERSTATE_TRAINING && m_SelectedTrainingMode == _XTRAINING_WATERDRAW)
	{
		_XAnimMeshAbstractClass* pEventItem = NULL;
		pEventItem = g_LifeEventManager.GetEventItem(m_CharacterInfo.gender, LIFE_EVENT_ITEM_TYPE_HOMI);
		switch(m_CurrentTrainingState)
		{
		case _XUSER_STATE_2:
			{
				D3DXMATRIX matWorld = g_LifeEventManager.m_matPointWorld;
				float fSrcHeight = matWorld._42;
				matWorld._42 += g_LifeEventManager.m_fAxisTime;
				pEventItem->Render(matWorld);
				matWorld._42 = fSrcHeight;
				break;
			}
		case _XUSER_STATE_5:
			{
				D3DXMATRIX matWorld = g_LifeEventManager.m_matTubeWorld;
				float fSrcHeight = matWorld._42;
				matWorld._42 += g_LifeEventManager.m_fAxisTime;
				pEventItem->Render(matWorld);
				matWorld._42 = fSrcHeight;
				break;
			}
		}
	}
}

void _XLocalUser::RequestBattleMode(_XBATTLEMODETYPE mode)
{
	// ����ȿ������ mode�� ������ �� ����
	if(m_Lock.modelock == 1)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_2325), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "���� �̻����� ���Ͽ� �ڼ� ��ȯ�� �Ұ��� �մϴ�."
		return;
	}
	else if( m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN 
		|| m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE 
		|| m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY)
	{
		_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
		TCHAR messagestring[512];
		memset( messagestring, 0, sizeof(messagestring) );
		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1571));// "������ ���� ä�δ� ���º����� �Ұ��� �մϴ�."
		g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
		return;
	}
	else if( m_bSocketPlugIn )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1572), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}
	else if( m_bReadingBook )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1573), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}
	
	else if( m_bInternalUse )	
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1574), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}	
	else if( m_bGambleBoxOpen )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1575), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}
	else if(m_bMatchPRMode)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1576), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}
	else if(m_UserState == _XDEF_USERSTATE_TRAINING)
	{
		// ���, ������� �����߿��� �ڼ� ��ȯ �Ұ�
		if(m_SelectedTrainingMode == _XTRAINING_MEDITATION || m_SelectedTrainingMode == _XTRAINING_SPELL)
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_2602), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			return;
		}
	}
	// ��� ��� ��� ���� �� ���� �Ұ� //Author : ����� �߰�
	else if( gpInput->CheckKeyPress(DIK_LMENU) && 
			g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
			g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2129), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}

	// ��û�� ���� �����ϵ��� �Ѵ�
	if(mode == _XBATTLE_PEACE)
		g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_peace_mode);	// ��ȭ���� ����
	else
		g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_battle_mode);	// �������� ����
}

void _XLocalUser::SetBattleMode(_XBATTLEMODETYPE mode)
{
	g_pLocalUser->m_LastModeChangeTime = g_LocalSystemTime;

	m_BattleMode = mode;
	
	if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE)
	{
		if(m_BattleMode == _XBATTLE_PEACE)
			m_CharacterInfo.animationdescript.idletype = _XBATTLE_PEACE;
		else
			m_CharacterInfo.animationdescript.idletype = _XBATTLE_BATTLE;
	}

	if(m_BattleMode == _XBATTLE_BATTLE)
	{
		if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE)
		{
			m_bHaveStance = TRUE;
			m_StanceMotionFirst = TRUE;
		}
		
		if(g_ViewMyBattleGauge)
		{
			m_MiniHpBorder.SetGaugeFactor(LifePowerPercentage);
		}
		((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_BATTLE );

		if(GetMotionClass() != _XACTION_MOVE)
			SetAnimation( m_CharacterInfo.animationdescript.motionclass );
	}
	else if(m_BattleMode == _XBATTLE_PK)
	{
		// 1st stance motion�� ����

		if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE)
		{
			m_bHaveStance = TRUE;
		}

		if(g_ViewMyBattleGauge)
		{
			m_MiniHpBorder.SetGaugeFactor(LifePowerPercentage);
		}
		((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_BATTLE );
		
		if(GetMotionClass() != _XACTION_MOVE)
			SetAnimation( m_CharacterInfo.animationdescript.motionclass );
	}
	else
	{
		m_bHaveStance = TRUE;
		m_StanceMotionFirst = TRUE;
		((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_PEACE );

		ResetSkillAnimation();
	}
	
	g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);

	if(g_pQuickSlot_Window)
		g_pQuickSlot_Window->ChangePeaceBattleMode();
}

void _XLocalUser::SetAnimation( _XMOTIONCLASS_TYPE motionclass, bool changemotion )
{
	if(motionclass < 0)
		return;

	/*
	if( m_SelectedSkillID )
	{
		if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
			_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF	&&
			g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
		{
			
			int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
								  g_SkillProperty[m_SelectedSkillID]->motion_male : 
							      g_SkillProperty[m_SelectedSkillID]->motion_female;
			
			if( m_CharacterInfo.modeldescript.item1 == 0 )
			{
			}
			else
			{
				_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
				
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					pWeaponItemProperty = g_WeaponItemProperty;
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					pWeaponItemProperty = g_WeaponItemProperty2;
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					pWeaponItemProperty = g_WeaponItemProperty3;
				
				switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
					if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
					}
					else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
							baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
						else
							baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
					}
					else
					{
						baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// �庴��
					baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
					break;				
				case _XGI_SC_WEAPON_SOFT				:				// ������
					baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
					break;
				}
			}
			
			if( m_ModelDescriptor.GetCurMotion() == baseanimindex+2 ) // ��� ������ ���� ���� �� ���� ��ü.
			{
				if( !(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME) )
					return;
			}
		}
	}
	*/


	// �̵� ���� �Ǵ� ���� ���۱�ü�ô� �̵����۽ð� �ʱ�ȭ 
	m_PrevMoveTime = 0;

	switch( motionclass )
	{
	case _XACTION_IDLE :
		// ���� �̿��� ������ �����Ǹ� ���ָ�� ����
		if( m_UseSpeedSkill )
		{
			m_CurrentBattleState = _XUSER_STATE_NONE;
			m_UseSpeedSkill = FALSE;
			m_HiperRunFailedFlag = TRUE;
		}

		if( m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE )
		{
			if(m_ActiveSlotID > 0 || m_bHaveStance)
			{
				short stancemotion = GetStanceMotion(m_ActiveSlotID);
				
				if(m_StanceMotionFirst)
				{
					if(stancemotion >= 0)
					{
						m_ModelDescriptor.SetCurMotion(stancemotion, TRUE);
						m_StanceMotionFirst = FALSE;
						m_bHaveStance = FALSE;
					}
					else
					{	
						ReloadWeaponModel();
						
						if( m_CharacterInfo.modeldescript.item1 == 0 )
						{					
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{
									case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// �庴��						
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
										break;
									case _XGI_SC_WEAPON_SOFT				:				// ������						
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
										break;
									case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// ������						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// ������						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// ������						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								}
							}
#endif
						}
						
						m_StanceMotionFirst = FALSE;
						m_bHaveStance = FALSE;
					}
				}
				else
				{
					if(stancemotion >= 0)
					{
						// ��ȭ���¿��� stance�� �ִ� ���� �� �ۿ� ���� - ������ �� ��ȭ�ڼ��� ���ư�
						m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, changemotion);
					}
					else
					{
						if( m_CharacterInfo.modeldescript.item1 == 0 )
						{					
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{
									case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// �庴��						
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
										break;
									case _XGI_SC_WEAPON_SOFT				:				// ������						
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
										break;
									case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// ������						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// ������						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// ������						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								}
							}
#endif
						}
					}
				}
			}
			else
			{
				if( m_CharacterInfo.modeldescript.item1 == 0 )
				{					
					m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
				}
				else
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
							{
							case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
								if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
								{
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
								}
								else
								{
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
								}
								break;
							case _XGI_SC_WEAPON_LONG				:				// �庴��						
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
								break;
							case _XGI_SC_WEAPON_SOFT				:				// ������						
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
								break;
							case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
								break;
							}							
						}
					}
#else					
					if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					{
						switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
							break;
						case _XGI_SC_WEAPON_SOFT				:				// ������						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
							break;
						case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					{
						switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
							break;
						case _XGI_SC_WEAPON_SOFT				:				// ������						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
							break;
						case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					{
						switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
							break;
						case _XGI_SC_WEAPON_SOFT				:				// ������						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
							break;
						case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						}
					}
#endif
				}
			}
		}
		else
		{
			if(m_ActiveSlotID > 0 || m_bHaveStance)
			{
				short stancemotion = GetStanceMotion(m_ActiveSlotID);
				
				if(m_StanceMotionFirst)
				{
					if(stancemotion >= 0)
					{
						m_ModelDescriptor.SetCurMotion(stancemotion, TRUE);
						m_StanceMotionFirst = FALSE;
						m_bHaveStance = FALSE;
					}
					else
					{
						ReloadWeaponModel();
						
						if( m_CharacterInfo.modeldescript.item1 == 0 )
						{					
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{
									case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
										{
											m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// �庴��						
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
										break;
									case _XGI_SC_WEAPON_SOFT				:				// ������						
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
										break;
									case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// ������						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// ������						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// ������						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								}
							}
#endif
						}
						
						m_StanceMotionFirst = FALSE;
						m_bHaveStance = FALSE;
					}
				}
				else
				{
					if(stancemotion >= 0)
						m_ModelDescriptor.SetCurMotion(stancemotion + _XANI_ORDER_REPEAT, changemotion);
					else
					{
						if( m_CharacterInfo.modeldescript.item1 == 0 )
						{					
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{
									case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
										{
											m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// �庴��						
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
										break;
									case _XGI_SC_WEAPON_SOFT				:				// ������						
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
										break;
									case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// ������						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// ������						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// ������						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								}
							}
#endif
						}
					}
				}
			}
			else
			{
				if( m_CharacterInfo.modeldescript.item1 == 0 )
				{					
					m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
				}
				else
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
							{
							case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
								if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
								{
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
								{
									m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
								}
								else
								{
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
								}
								break;
							case _XGI_SC_WEAPON_LONG				:				// �庴��						
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
								break;
							case _XGI_SC_WEAPON_SOFT				:				// ������						
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
								break;
							case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
								break;
							}
						}
					}
#else
					if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					{
						switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
							{
									m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
							}
							else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
							break;
						case _XGI_SC_WEAPON_SOFT				:				// ������						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
							break;
						case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					{
						switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
							}
							else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
							break;
						case _XGI_SC_WEAPON_SOFT				:				// ������						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
							break;
						case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					{
						switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
							}
							else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
							break;
						case _XGI_SC_WEAPON_SOFT				:				// ������						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN		:						// �ϱ�						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
							break;
						case _XGI_SC_WEAPON_MUSICAL		:						// �Ǻ���
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL		:						// Ư������
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						}
					}
#endif
				}
			}
		}


		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );

		break;
	
	case _XACTION_MOVE :
		if( m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_RUN )
		{			
			// ���� �̿��� ������ �����Ǹ� ���ָ�� ����
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;

				if( m_SelectedSkillID )
				{
					if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
						_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
						g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
					{
						m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
						m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
					}
				}
			}

			SetMoveSpeed( _XDEF_CHARACTERMOVESPEED_RUN );
			
			// �տ� ���� ���� ����
			if( m_CharacterInfo.modeldescript.item1 == 0 )
			{
				m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_BATTLE, TRUE);
							}
							else
							{						
								m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SHORTWEAPON_REPEAT, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��					
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_LONGWEAPON_REPEAT, TRUE );					
							break;
						case _XGI_SC_WEAPON_SOFT				:				// ������
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SOFTWEAPON_REPEAT, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						}
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_BATTLE, TRUE);
						}
						else
						{						
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SHORTWEAPON_REPEAT, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��					
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_LONGWEAPON_REPEAT, TRUE );					
						break;
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SOFTWEAPON_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						}
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_BATTLE, TRUE);
						}
						else
						{						
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SHORTWEAPON_REPEAT, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��					
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_LONGWEAPON_REPEAT, TRUE );					
						break;
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SOFTWEAPON_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						}
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_BATTLE, TRUE);
						}
						else
						{						
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SHORTWEAPON_REPEAT, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��					
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_LONGWEAPON_REPEAT, TRUE );					
						break;
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SOFTWEAPON_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					}
				}
#endif
			}
		}
		else if( m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG )
		{
#ifdef _XDEF_NEWANIMATIONLIST
			SetMoveSpeed( 	_XDEF_CHARACTERMOVESPEED_RUN  );
			{
				int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
									  g_SkillProperty[m_SelectedSkillID]->motion_male : 
									  g_SkillProperty[m_SelectedSkillID]->motion_female;

				if( m_CharacterInfo.modeldescript.item1 == 0 )
				{
					m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
				}
				else
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
							{
							case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
								if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
								{
									m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SWORD, TRUE);
									else
										m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_BATTLESWORD, TRUE);
								}
								else
								{
									m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SHORTWEAPON, TRUE);
								}
								break;
							case _XGI_SC_WEAPON_LONG				:				// �庴��
								m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_LONGWEAPON, TRUE);
								break;				
							case _XGI_SC_WEAPON_SOFT				:				// ������
								m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SHORTWEAPON, TRUE);
								break;
							case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
								m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
								break;
							case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
								m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
								break;
							case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
								m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
								break;
							}
						}
					}
#else
					_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;

					if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
						pWeaponItemProperty = g_WeaponItemProperty;
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
						pWeaponItemProperty = g_WeaponItemProperty2;
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
						pWeaponItemProperty = g_WeaponItemProperty3;
					
					switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
						}
						else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SWORD, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_BATTLESWORD, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SHORTWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_LONGWEAPON, TRUE);
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SHORTWEAPON, TRUE);
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
						break;
					}
#endif
				}
			}
#else
			SetMoveSpeed( 	_XDEF_CHARACTERMOVESPEED_RUN  );

			if( m_CharacterInfo.modeldescript.item1 == 0 )
			{
				m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_BATTLE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SHORTWEAPON_START, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_LONGWEAPON_START, TRUE );
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// ������
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SOFTWEAPON_START, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						}
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SHORTWEAPON_START, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_LONGWEAPON_START, TRUE );
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SOFTWEAPON_START, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					}					
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						}
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SHORTWEAPON_START, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_LONGWEAPON_START, TRUE );
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SOFTWEAPON_START, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					}					
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						}
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SHORTWEAPON_START, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_LONGWEAPON_START, TRUE );
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SOFTWEAPON_START, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					}					
				}
#endif
			}
#endif
		}

		break;
	
	case _XACTION_BLOW :
		// ���� �̿��� ������ �����Ǹ� ���ָ�� ����
		if( m_UseSpeedSkill )
		{
			ResetSkillAnimation();
			m_UseSpeedSkill = FALSE;
			m_HiperRunFailedFlag = TRUE;			
		}

		if( m_CharacterInfo.modeldescript.item1 == 0 ) // �ƹ��͵� �ȵ��� ��� 
		{
			m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
		}
		else
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SHORTWEAPON, TRUE );								
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_LONGWEAPON, TRUE );							
						break;			
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SOFTWEAPON, TRUE );			
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
						break;
					}
				}
			}
#else
			if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
					if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					}
					else
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SHORTWEAPON, TRUE );								
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// �庴��
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_LONGWEAPON, TRUE );							
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// ������
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SOFTWEAPON, TRUE );			
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
					if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					}
					else
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SHORTWEAPON, TRUE );								
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// �庴��
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_LONGWEAPON, TRUE );							
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// ������
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SOFTWEAPON, TRUE );			
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
					if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					}
					else
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SHORTWEAPON, TRUE );								
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// �庴��
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_LONGWEAPON, TRUE );							
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// ������
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SOFTWEAPON, TRUE );			
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				}
			}
#endif
		}		
		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		break;

	case _XACTION_DIE :
		// ���� �̿��� ������ �����Ǹ� ���ָ�� ����
		if( m_UseSpeedSkill )
		{
			ResetSkillAnimation();
			m_UseSpeedSkill = FALSE;
			m_HiperRunFailedFlag = TRUE;			
		}

		if( m_CharacterInfo.modeldescript.item1 == 0 ) // �ƹ��͵� �ȵ��� ��� 
		{
			m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
		}
		else
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )	// �ǰ�
							m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );
						else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_BATTLE, TRUE);
						}
						else
							m_ModelDescriptor.SetCurMotion( _XAN_DIE_SHORTWEAPON, TRUE );								
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_DIE_LONGWEAPON, TRUE );							
						break;			
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					}
				}
			}
#else
			if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
					if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )	// �ǰ�
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );
					else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
							m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_PEACE, TRUE);
						else
							m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_BATTLE, TRUE);
					}
					else
						m_ModelDescriptor.SetCurMotion( _XAN_DIE_SHORTWEAPON, TRUE );								
					break;
				case _XGI_SC_WEAPON_LONG				:				// �庴��
					m_ModelDescriptor.SetCurMotion( _XAN_DIE_LONGWEAPON, TRUE );							
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// ������
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
					if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )	// �ǰ�
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );
					else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
							m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_PEACE, TRUE);
						else
							m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_BATTLE, TRUE);
					}
					else
						m_ModelDescriptor.SetCurMotion( _XAN_DIE_SHORTWEAPON, TRUE );								
					break;
				case _XGI_SC_WEAPON_LONG				:				// �庴��
					m_ModelDescriptor.SetCurMotion( _XAN_DIE_LONGWEAPON, TRUE );							
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// ������
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
					if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )	// �ǰ�
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );
					else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
							m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_PEACE, TRUE);
						else
							m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_BATTLE, TRUE);
					}
					else
						m_ModelDescriptor.SetCurMotion( _XAN_DIE_SHORTWEAPON, TRUE );								
					break;
				case _XGI_SC_WEAPON_LONG				:				// �庴��
					m_ModelDescriptor.SetCurMotion( _XAN_DIE_LONGWEAPON, TRUE );							
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// ������
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				}
			}
#endif
		}

		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		break;

	case _XACTION_DYING :
		{
			// ���� �̿��� ������ �����Ǹ� ���ָ�� ����
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}

			if(m_CharacterInfo.modeldescript.item1 == 0)
			{
				m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType)
						{
						case _XGI_SC_WEAPON_SHORT :
							{
								if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_PEACE, TRUE);
									else
										m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_BATTLE, TRUE);
								}
								else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
							}
							break;
						case _XGI_SC_WEAPON_LONG :
							{
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_LONGWEAPON, TRUE);
							}
							break;
						case _XGI_SC_WEAPON_HIDDEN :
							{
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							}
							break;
						case _XGI_SC_WEAPON_MUSICAL :
							{
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							}
							break;
						case _XGI_SC_WEAPON_SPECIAL :
							{
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							}
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_BATTLE, TRUE);
							}
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_LONGWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_BATTLE, TRUE);
							}
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_LONGWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_BATTLE, TRUE);
							}
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_LONGWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					}
				}
#endif
			}

			{
				int rand = _XRandom() % 3;
				if( m_CharacterInfo.gender == _XGENDER_MAN )
					_XPlaySoundEffect( g_BlowEffectSoundIndex[0][rand] , m_Position );
				else 
					_XPlaySoundEffect( g_BlowEffectSoundIndex[1][rand] , m_Position );
			}

			g_WorldEffectInstanceManager.DeleteInstance(_XDEF_INTERFACEID_PICKING);
		}
		break;

	case _XACTION_ATTACK :		
		{
			// ���� �̿��� ������ �����Ǹ� ���ָ�� ����
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;
			}

			if(m_SelectedSkillID == 0)
			{
				if(m_CharacterInfo.modeldescript.item1 == 0)
				{
					m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;
				}
				else
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							switch(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType)
							{
							case _XGI_SC_WEAPON_SHORT :
								{
									if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// �ǰ�
									else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
										m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;
									else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SWORD;
									else
										m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// �ܺ���
								}
								break;
							case _XGI_SC_WEAPON_LONG :
								{
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �庴��
								}
								break;
							case _XGI_SC_WEAPON_SOFT :
							case _XGI_SC_WEAPON_HIDDEN :
							case _XGI_SC_WEAPON_MUSICAL :						
								{
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �ӽ�
								}
								break;
							case _XGI_SC_WEAPON_SPECIAL :
								{
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;				// ������
								}
								break;
							}
						}
					}
#else
					if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					{
						switch(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType)
						{
						case _XGI_SC_WEAPON_SHORT :
							{
								if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// �ǰ�
								else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;
								else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SWORD;
								else
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// �ܺ���
							}
							break;
						case _XGI_SC_WEAPON_LONG :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �庴��
							}
							break;
						case _XGI_SC_WEAPON_SOFT :
						case _XGI_SC_WEAPON_HIDDEN :
						case _XGI_SC_WEAPON_MUSICAL :						
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �ӽ�
							}
							break;
						case _XGI_SC_WEAPON_SPECIAL :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;				// ������
							}
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					{
						switch(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType)
						{
						case _XGI_SC_WEAPON_SHORT :
							{
								if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// �ǰ�
								else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;
								else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SWORD;
								else
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// �ܺ���
							}
							break;
						case _XGI_SC_WEAPON_LONG :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �庴��
							}
							break;
						case _XGI_SC_WEAPON_SOFT :
						case _XGI_SC_WEAPON_HIDDEN :
						case _XGI_SC_WEAPON_MUSICAL :						
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �ӽ�
							}
							break;
						case _XGI_SC_WEAPON_SPECIAL :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// ������ == �ǰ� 
							}
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					{
						switch(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType)
						{
						case _XGI_SC_WEAPON_SHORT :
							{
								if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// �ǰ�
								else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;
								else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SWORD;
								else
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// �ܺ���
							}
							break;
						case _XGI_SC_WEAPON_LONG :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �庴��
							}
							break;
						case _XGI_SC_WEAPON_SOFT :
						case _XGI_SC_WEAPON_HIDDEN :
						case _XGI_SC_WEAPON_MUSICAL :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �ӽ�
							}
							break;
						case _XGI_SC_WEAPON_SPECIAL :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;		// ������
							}
							break;
						}
					}
#endif
				}
			}

			m_ModelDescriptor.SetCurMotion( m_CharacterInfo.animationdescript.attacktype, changemotion );			
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		}				
		break;
	case _XACTION_DEFENCE :
		{			
			// ���� �̿��� ������ �����Ǹ� ���ָ�� ����
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}

			if( m_CharacterInfo.modeldescript.item1 == 0 )// �ƹ��͵� �ȵ��� ��� 
			{				
				m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{				
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );						
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SHORTWEAPON, TRUE );						
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_LONGWEAPON, TRUE );	
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// ������
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SOFTWEAPON, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SOFTWEAPON, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SOFTWEAPON, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SOFTWEAPON, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					}
				}
#endif
			}
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		}
		break;
	case _XACTION_RESCUE :
		{
			// ���� �̿��� ������ �����Ǹ� ���ָ�� ����
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}

			if( m_CharacterInfo.modeldescript.item1 == 0 )// �ƹ��͵� �ȵ��� ��� 
			{				
				m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{				
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );						
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_BATTLE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_SHORTWEAPON, TRUE );						
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_LONGWEAPON, TRUE );	
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// ������
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					}
				}
#endif
			}
		}
		break;
	case _XACTION_REBIRTH :
		{
			// ���� �̿��� ������ �����Ǹ� ���ָ�� ����
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}

			if( m_CharacterInfo.modeldescript.item1 == 0 )// �ƹ��͵� �ȵ��� ��� 
			{				
				m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{				
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );						
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_BATTLE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_SHORTWEAPON, TRUE );						
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_LONGWEAPON, TRUE );	
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// ������
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					}
				}
#endif
			}
		}
		break;
	case _XACTION_MATCH_START :
		{
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}

			if(m_CharacterInfo.modeldescript.item1 == 0)// �ƹ��͵� �ȵ��� ��� 
			{				
				m_ModelDescriptor.SetCurMotion(_XAN_MATCH_NONE, TRUE);
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType)
						{				
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_MATCH_NONE, TRUE);						
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_MATCH_SHORTWEAPON, TRUE );						
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_LONGWEAPON, TRUE );	
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// ������
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType)
					{				
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
						{
							m_ModelDescriptor.SetCurMotion(_XAN_MATCH_NONE, TRUE);						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType)
					{				
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
						{
							m_ModelDescriptor.SetCurMotion(_XAN_MATCH_NONE, TRUE);						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType)
					{				
					case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
						if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
						{
							m_ModelDescriptor.SetCurMotion(_XAN_MATCH_NONE, TRUE);						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// �庴��
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// ������
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					}
				}
#endif
			}
		}
		break;
	case _XACTION_MATCH_CANCEL :
		{
			// ���� �̿��� ������ �����Ǹ� ���ָ�� ����
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}
			m_ModelDescriptor.SetCurMotion(_XAN_MATCH_CANCEL);
		}
		break;
	
	case _XACTION_LPS :
		{
			m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
			
			_XMob* pMob = NULL;
			if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE )
			{
				// ���߿� LPS���� ó���� ���� ���� �и��Ѵ�.
				if(m_ViewTargetType == _XPICKINGTARGET_MOB)
				{
					// �ι�° Ŭ��
					pMob = (_XMob*)m_ViewTarget;
				}
			}
			else
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					// �ι�° Ŭ��
					pMob = (_XMob*)m_AttackTarget;
				}
			}
			if( pMob )
			{
				switch( g_MobAttribTable[pMob->m_MobType].scripttype ) {
				case 81:// Normal LPS
					{
						if( m_CharacterInfo.modeldescript.item1 == 0 )// �ƹ��͵� �ȵ��� ��� 
						{				
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{				
									case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
												m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
											else
												m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// �庴��
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
										break;				
									case _XGI_SC_WEAPON_SOFT				:				// ������
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// ������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// ������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// ������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
#endif
						}
					}
					break;
				case 82:// Key LPS
					{
						if( m_CharacterInfo.modeldescript.item1 == 0 )// �ƹ��͵� �ȵ��� ��� 
						{				
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{				
									case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
												m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
											else
												m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// �庴��
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
										break;				
									case _XGI_SC_WEAPON_SOFT				:				// ������
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// ������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// ������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// ������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
#endif
						}
					}
					break;
				case 83:// Transform LPS
					{
					}
					break;
				case 84:// HP LPS
					{
					}
					break;
				case 85:// Optional LPS
					{
						if( m_CharacterInfo.modeldescript.item1 == 0 )// �ƹ��͵� �ȵ��� ��� 
						{				
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{				
									case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
												m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
											else
												m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// �庴��
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
										break;				
									case _XGI_SC_WEAPON_SOFT				:				// ������
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// ������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// ������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// ������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
#endif
						}
					}
					break;
				}
			}
		}
		break;

	case _XACTION_INSENSIBLE :		// ���� - ����ȿ��
		{
			// ���� �̿��� ������ �����Ǹ� ���ָ�� ����
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}

			if(m_CharacterInfo.modeldescript.item1 == 0)
			{
#ifdef _XTS_NEWINSENSIBLE
				m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
				m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType)
						{
						case _XGI_SC_WEAPON_SHORT :
							{
								if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									{
#ifdef _XTS_NEWINSENSIBLE
										m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
										m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_PEACE, TRUE);
#endif
									}
									else
									{
#ifdef _XTS_NEWINSENSIBLE
										m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
										m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_BATTLE, TRUE);
#endif
									}
								}
								else
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SHORTWEAPON, TRUE);
#endif
								}
							}
							break;
						case _XGI_SC_WEAPON_LONG :
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_LONGWEAPON, TRUE);
#endif
							}
							break;
						case _XGI_SC_WEAPON_HIDDEN :
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
							}
							break;
						case _XGI_SC_WEAPON_MUSICAL :
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
							}
							break;
						case _XGI_SC_WEAPON_SPECIAL :
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
							}
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
							}
							else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_PEACE, TRUE);
#endif
								}
								else
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_BATTLE, TRUE);
#endif
								}
							}
							else
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SHORTWEAPON, TRUE);
#endif
							}
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_LONGWEAPON, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
							}
							else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_PEACE, TRUE);
#endif
								}
								else
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_BATTLE, TRUE);
#endif
								}
							}
							else
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SHORTWEAPON, TRUE);
#endif
							}
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_LONGWEAPON, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
							}
							else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_PEACE, TRUE);
#endif
								}
								else
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_BATTLE, TRUE);
#endif
								}
							}
							else
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SHORTWEAPON, TRUE);
#endif
							}
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_LONGWEAPON, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					}
				}
#endif
			}
			
			{
				int rand = _XRandom() % 3;
				if( m_CharacterInfo.gender == _XGENDER_MAN )
					_XPlaySoundEffect( g_BlowEffectSoundIndex[0][rand] , m_Position );
				else 
					_XPlaySoundEffect( g_BlowEffectSoundIndex[1][rand] , m_Position );
			}
			
			g_WorldEffectInstanceManager.DeleteInstance(_XDEF_INTERFACEID_PICKING);
		}
		break;

	default:
		// ���� �̿��� ������ �����Ǹ� ���ָ�� ����
		if( m_UseSpeedSkill )
		{
			ResetSkillAnimation();
			m_UseSpeedSkill = FALSE;
			m_HiperRunFailedFlag = TRUE;			
		}
		break;
	}	
		
	m_SpecialActionTimer = g_LocalSystemTime;

	m_CharacterInfo.animationdescript.motionclass = motionclass;

	m_ModelDescriptor.m_FrameResult = _XDEF_ACI_NONE;
	m_ModelDescriptor.m_ViewLeftSwordEffect = (motionclass == _XACTION_ATTACK);
}

void _XLocalUser::SetAnimationIndex( int animationindex, bool changemotion )
{	
	// �̵� ���� �Ǵ� ���� ���۱�ü�ô� �̵����۽ð� �ʱ�ȭ 
	m_PrevMoveTime = 0;
	
	if(animationindex < 0)
		return;

	if( g_LocalSystemTime == 0.0f )
		 m_PrevProcessTime = -1.0f;
	else m_PrevProcessTime = g_fLocalSystemTime;

#ifdef _XDEF_NEWANIMATIONLIST

	BOOL checkfastmove = FALSE;

	if( m_SelectedSkillID )
	{
		if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
			_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF	&&
			g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
		{
	
			int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
								  g_SkillProperty[m_SelectedSkillID]->motion_male : 
								  g_SkillProperty[m_SelectedSkillID]->motion_female;
		
			if( m_CharacterInfo.modeldescript.item1 == 0 )
			{
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
								else
									baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
							}
							else
							{
								baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��
							baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// ������
							baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
							break;
						}
					}
				}
#else
				_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
				
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					pWeaponItemProperty = g_WeaponItemProperty;
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					pWeaponItemProperty = g_WeaponItemProperty2;
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					pWeaponItemProperty = g_WeaponItemProperty3;
				
				switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
					if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
					}
					else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
							baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
						else
							baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
					}
					else
					{
						baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// �庴��
					baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
					break;				
				case _XGI_SC_WEAPON_SOFT				:				// ������
					baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
					break;
				}
#endif
			}
			
			if( animationindex == baseanimindex )
			{
				checkfastmove = TRUE;					

				m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
				m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
				SetMoveSpeed( 	_XDEF_CHARACTERMOVESPEED_RUN  );
			}
			else if( animationindex == baseanimindex+1 )
			{
				checkfastmove = TRUE;

				m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
				m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
				SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_RUN );
			}			
			else if( animationindex == baseanimindex+2 )
			{
				checkfastmove = TRUE;

				m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
				m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
				SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			}
		}
	}

	if( !checkfastmove )	

#endif

	{	
		switch( animationindex )
		{	
		case _XAN_IDLE_NONE					:	
		case _XAN_IDLE_NONE_ACTION			:		
		case _XAN_IDLE_SHORTWEAPON			:	
		case _XAN_IDLE_SHORTWEAPON_ACTION	:		
		case _XAN_IDLE_LONGWEAPON			:	
		case _XAN_IDLE_LONGWEAPON_ACTION	:
		case _XAN_IDLE_SWORD_PEACE			:
		case _XAN_IDLE_SWORD_ACTION			:
			m_CharacterInfo.animationdescript.idletype			= _XBATTLE_PEACE;
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			break;	

		case _XAN_BATTLEIDLE_NONE			: 
		case _XAN_BATTLEIDLE_SHORTWEAPON	:
		case _XAN_BATTLEIDLE_LONGWEAPON		:
		case _XAN_BATTLEIDLE_SWORD_REPEAT :
			m_CharacterInfo.animationdescript.idletype			= _XBATTLE_BATTLE;
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			break;
			
		case _XAN_EMOTION_CRINGESTART :
			{
				if(m_CharacterInfo.modeldescript.item1 != 0)
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							if(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
								WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									animationindex = _XAN_KNEE_SWORD_START;
							}
						}
					}
#else
					if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON)
					{
						if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_START;
						}
						
					}
					else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2)
					{
						if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_START;
						}
					}
					else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3)
					{
						if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_START;
						}
					}
#endif
				}

				m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
				g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			}
			break;
		case _XAN_EMOTION_CRINGEREPEAT :
			{
				if(m_CharacterInfo.modeldescript.item1 != 0)
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							if(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
								WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									animationindex = _XAN_KNEE_SWORD_REPEAT;
							}
						}
					}
#else
					if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON)
					{
						if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_REPEAT;
						}
						
					}
					else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2)
					{
						if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_REPEAT;
						}
					}
					else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3)
					{
						if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_REPEAT;
						}
					}
#endif
				}
				
				m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
				g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			}
			break;
		case _XAN_EMOTION_CRINGEEND :
			{
				if(m_CharacterInfo.modeldescript.item1 != 0)
				{
#ifdef _XTS_ITEM_OPTIMIZATION		
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							if(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
								WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									animationindex = _XAN_KNEE_SWORD_END;
							}
						}
					}
#else
					if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON)
					{
						if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_END;
						}
						
					}
					else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2)
					{
						if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_END;
						}
					}
					else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3)
					{
						if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_END;
						}
					}
#endif
				}

				m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
				g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			}
			break;

#ifdef ALL_RB_REMOVEWEAPON_EMOTIONANI_070622_KERYGMA
		case _XAN_EMOTION_BOW				:
		case _XAN_EMOTION_BOWPOLITELY		:
#endif

		case _XAN_EMOTION_MARRYCLAPPINGHAND	:		
		case _XAN_EMOTION_MARRYBOX			:		
		case _XAN_EMOTION_MARRYBOWPOLITELY	:
			{
				DeleteWeaponModel();
				m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
				g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			}			
			break;
		case _XAN_KNEE_SWORD_START			:
		case _XAN_KNEE_SWORD_REPEAT			:
		case _XAN_KNEE_SWORD_END			:
	
#ifndef ALL_RB_REMOVEWEAPON_EMOTIONANI_070622_KERYGMA
		case _XAN_EMOTION_BOW				:
		case _XAN_EMOTION_BOWPOLITELY		:
#endif			

		case _XAN_EMOTION_EXCITEMENT		:
		case _XAN_EMOTION_RUSH				:	
		case _XAN_EMOTION_CHEER				:
		case _XAN_EMOTION_BREAKDOWN			:
		case _XAN_EMOTION_CRY				:
		case _XAN_EMOTION_LAUGH				:				
		case _XAN_EMOTION_ANGRY				:
		case _XAN_EMOTION_DOZ				:
		case _XAN_EMOTION_SITDOWNSTART		:
		case _XAN_EMOTION_SITDOWNREPEAT		:
		case _XAN_EMOTION_SITDOWNEND		:							
		case _XAN_EMOTION_CLAPPINGHAND		:

		case _XAN_MATCH_PR_START			:
		case _XAN_MATCH_PR_REPEAT			:
		case _XAN_MATCH_PR_END				:
		case _XAN_MATCH_WINNER_START		:
		case _XAN_MATCH_WINNER_REPEAT		:
		case _XAN_MATCH_LOSER_START			:
		case _XAN_MATCH_LOSER_REPEAT		:

#ifdef _XDEF_CASTLEBATTLE
		case _XAN_MANOR_WIN					:
		case _XAN_MANOR_SHOUT				:
#endif
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			break;

		case  _XAN_MOVE_NONE_START				:
		case  _XAN_MOVE_SHORTWEAPON_START		:
		case  _XAN_MOVE_LONGWEAPON_START		:
		case  _XAN_MOVE_SOFTWEAPON_START		:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_RUN;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			break;

		case  _XAN_MOVE_NONE_REPEAT				:
		case  _XAN_MOVE_SHORTWEAPON_REPEAT		:	
		case  _XAN_MOVE_LONGWEAPON_REPEAT		:
		case  _XAN_MOVE_SOFTWEAPON_REPEAT		:
		case  _XAN_MOVE_SWORD_REPEAT_PEACE		:
		case  _XAN_MOVE_SWORD_REPEAT_BATTLE		:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_RUN;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_RUN );
			break;

		case  _XAN_MOVE_NONE_END				:
		case  _XAN_MOVE_SHORTWEAPON_END			:
		case  _XAN_MOVE_LONGWEAPON_END			:
		case  _XAN_MOVE_SOFTWEAPON_END			:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_RUN;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			break;
		
		case  _XAN_FASTMOVE_NONE_START			:	
		case  _XAN_FASTMOVE_SHORTWEAPON_START	:
		case  _XAN_FASTMOVE_LONGWEAPON_START	:
		case  _XAN_FASTMOVE_SOFTWEAPON_START	:
		case  _XAN_FASTMOVE_SWORD_START_PEACE	:
		case  _XAN_FASTMOVE_SWORD_START_BATTLE	:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			break;

		case  _XAN_FASTMOVE_NONE_REPEAT			:	
		case  _XAN_FASTMOVE_SHORTWEAPON_REPEAT	:	
		case  _XAN_FASTMOVE_LONGWEAPON_REPEAT	:	
		case  _XAN_FASTMOVE_SOFTWEAPON_REPEAT	:
		case  _XAN_FASTMOVE_SWORD_REPEAT_PEACE	:
		case  _XAN_FASTMOVE_SWORD_REPEAT_BATTLE	:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_RUN );
			break;

		case  _XAN_FASTMOVE_NONE_END			:	
		case  _XAN_FASTMOVE_SHORTWEAPON_END		:
		case  _XAN_FASTMOVE_LONGWEAPON_END		:
		case  _XAN_FASTMOVE_SOFTWEAPON_END		:
		case  _XAN_FASTMOVE_SWORD_END_PEACE		:
		case  _XAN_FASTMOVE_SWORD_END_BATTLE	:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			break;

		case _XAN_DYINGREADY_NONE :
		case _XAN_DYINGREADY_SHORTWEAPON :
		case _XAN_DYINGREADY_LONGWEAPON :
		case _XAN_DYINGREADY_SWORD_PEACE :
		case _XAN_DYINGREADY_SWORD_BATTLE :
			{
				int rand = _XRandom() % 3;			
				if( m_CharacterInfo.gender == _XGENDER_MAN )
					_XPlaySoundEffect( g_BlowEffectSoundIndex[0][rand] , m_Position );
				else 
					_XPlaySoundEffect( g_BlowEffectSoundIndex[1][rand] , m_Position );
			}

			m_CharacterInfo.animationdescript.motionclass		= _XACTION_DYING;
			g_WorldEffectInstanceManager.DeleteInstance(_XDEF_INTERFACEID_PICKING);
			break;
			
		case _XAN_DYINGREPEAT_NONE :
		case _XAN_DYINGREPEAT_SHORTWEAPON :
		case _XAN_DYINGREPEAT_LONGWEAPON :
		case _XAN_DYINGREPEAT_SWORD_PEACE :
		case _XAN_DYINGREPEAT_SWORD_BATTLE :
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_DYING;
			g_WorldEffectInstanceManager.DeleteInstance(_XDEF_INTERFACEID_PICKING);
			break;

		case _XAN_DIE :
		case _XAN_DIE_SHORTWEAPON :
		case _XAN_DIE_LONGWEAPON :
		case _XAN_DIE_SWORD_PEACE :
		case _XAN_DIE_SWORD_BATTLE :
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_DIE;		
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			break;

		case _XAN_HITBACK_NONE :
		case _XAN_HITBACK_SHORTWEAPON :
		case _XAN_HITBACK_LONGWEAPON :
		case _XAN_HITBACK_SWORD_PEACE :
		case _XAN_HITBACK_SWORD_BATTLE :
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_RESCUE;
			break;
			
		case _XAN_WAKEUP_NONE :
		case _XAN_WAKEUP_SHORTWEAPON :
		case _XAN_WAKEUP_LONGWEAPON :
		case _XAN_WAKEUP_SWORD_PEACE :
		case _XAN_WAKEUP_SWORD_BATTLE :
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_REBIRTH;
			break;

		case _XAN_DEFENCE_NONE			:
		case _XAN_DEFENCE_SHORTWEAPON	:
		case _XAN_DEFENCE_LONGWEAPON	:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_DEFENCE;	
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			break;
			
		case _XAN_BLOW_NONE				:
		case _XAN_BLOW_SHORTWEAPON		:
		case _XAN_BLOW_LONGWEAPON		:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_BLOW;		
			break;	

		case _XAN_LPS_HANDTOUCH				:			
		case _XAN_LPS_SHORTWEAPON_HANDTOUCH :			
		case _XAN_LPS_LONGWEAPON_HANDTOUCH	:			
		case _XAN_LPS_FOOTTOUCH				:			
		case _XAN_LPS_SHORTWEAPON_FOOTTOUCH :			
		case _XAN_LPS_LONGWEAPON_FOOTTOUCH	:			
		case _XAN_LPS_USEKEY_HAND			:
		case _XAN_LPS_USEKEY_SHORTWEAPON	:
		case _XAN_LPS_USEKEY_LONGWEAPON		:
		case _XAN_LPS_USEKEY_SWORD_PEACE	:
		case _XAN_LPS_USEKEY_SWORD_BATTLE	:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_LPS;
			break;	

		case _XAN_MATCH_NONE :
		case _XAN_MATCH_SHORTWEAPON :
		case _XAN_MATCH_LONGWEAPON :
			m_CharacterInfo.animationdescript.motionclass = _XACTION_MATCH_START;
			break;

		case _XAN_MATCH_CANCEL :
			m_CharacterInfo.animationdescript.motionclass = _XACTION_MATCH_CANCEL;
			break;
			
		default :
			m_CharacterInfo.animationdescript.motionclass = _XACTION_ATTACK;
			m_CharacterInfo.animationdescript.attacktype = animationindex;
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			break;
		}
	}

	// ���� �̿��� ������ �����Ǹ� ���ָ�� ����
	if( m_CharacterInfo.animationdescript.motionclass != _XACTION_MOVE || 
	    m_CharacterInfo.animationdescript.movetype	  != _XMOVETYPE_KYUNGGONG )
	{
		if( m_UseSpeedSkill )
		{
			ResetSkillAnimation();
			m_UseSpeedSkill = FALSE;
		}		
	}

	m_ModelDescriptor.SetCurMotion( animationindex, changemotion );

	m_ModelDescriptor.m_FrameResult = _XDEF_ACI_NONE;
	m_SpecialActionTimer = g_LocalSystemTime;
	m_ModelDescriptor.m_ViewLeftSwordEffect = (m_CharacterInfo.animationdescript.motionclass == _XACTION_ATTACK);
}

void _XLocalUser::InitializeUserDataTable( void )
{

	int	jinlevellut[241];
	int	gonglevellut[241];

	if( !g_ScriptArchive.ReOpenPackage() )
		return;

	FILE* pScriptFile = NULL;

	pScriptFile = g_ScriptArchive.GetPackedFile( _T("Level_Table.XMS") );
		
	if( !pScriptFile )
	{
		_XFatalError( "Load Level LUT( %s ) File open", _T("Level_Table.XMS") );
		g_ScriptArchive.ClosePackage();
		return; 
	}
	
	TCHAR parsingString[512];
	int index = 0;
	while( 1 )
	{			
		if( feof(pScriptFile) ) break;
		
		memset( parsingString, 0, 512 );
		if( !fgets( parsingString, 511, pScriptFile ) ) break;
		if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;
		
		if( strncmp( parsingString, "_XM_EOS", 7 ) == 0 )
		{
			break;			
		}
		
		sscanf( parsingString, "%d", &index );

		sscanf( parsingString, "%d %d %d", &index, &jinlevellut[index], &gonglevellut[index] );
	}
		
	memset( g_UserLevelTable, 0, sizeof(_XUSERLEVEL_LUTTYPE) * 241 );

	//_XLog( "Notice : Level table loaded" );

	int		pos = 1;
	DWORD	sum = 0;

	for (int i=1; i<=20 ; i++ ) 
	{
		for (int  j=1; j<=12 ; j++) 
		{
			g_UserLevelTable[pos].level				= i;								// �����ܰ�
			g_UserLevelTable[pos].sublevel			= j;								// ����
			g_UserLevelTable[pos].innerlevel		= (i-1) * 12 + j;					// �Ѽ���

			g_UserLevelTable[pos].needjinexperience	= jinlevellut[pos-1];
			g_UserLevelTable[pos].needgongexperience = gonglevellut[pos-1];
			
			sum += g_UserLevelTable[pos].needjinexperience;								// ��������ġ			
			g_UserLevelTable[pos].accumulatejinexperience = sum;

//			_XLog( "%d %d %d %d %d", g_UserLevelTable[pos].level, g_UserLevelTable[pos].sublevel, g_UserLevelTable[pos].totalsublevel, g_UserLevelTable[pos].needexperience, g_UserLevelTable[pos].accumulateexperience );

			pos++;
		}
	}

	//_XLog( "Notice : Level table calculated" );

	pScriptFile = g_ScriptArchive.GetPackedFile( _T("PointTable.xms") );
	
	if( !pScriptFile )
	{
		_XFatalError( "Load Point table( %s ) File open", _T("PointTable.xms") );
		g_ScriptArchive.ClosePackage();
		return; 
	}
		
	index = 0;
	while( 1 )
	{			
		if( feof(pScriptFile) ) break;
		
		memset( parsingString, 0, 512 );
		if( !fgets( parsingString, 511, pScriptFile ) ) break;
		if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;
		
		if( strncmp( parsingString, "_XM_EOS", 7 ) == 0 )
		{
			break;			
		}
		
		DWORD dummy;
		sscanf( parsingString, "%d %d %d %d %d %d %d %d %d", &dummy, 
							   &g_UsePointTable[index].vitality,
							   &g_UsePointTable[index].inner,
							   &g_UsePointTable[index].concenturate,
							   
			                   &g_UsePointTable[index].need_constitution, 
							   &g_UsePointTable[index].need_zen,
							   &g_UsePointTable[index].need_intelligence,
							   &g_UsePointTable[index].need_strength,
							   &g_UsePointTable[index].need_dexterity) ;
		index++;
	}

	//_XLog( "Notice : Point table loaded" );

	pScriptFile = NULL;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		pScriptFile = g_ScriptArchive.GetPackedFile( _T("vn_RollName_Table.xms") );
		
		if( !pScriptFile )
		{
			_XFatalError( "Load Roll name table( %s ) File open", _T("vn_RollName_Table.xms") );
			g_ScriptArchive.ClosePackage();
			return; 
		}

		int groupindex = 0;
		int rollclass = 0;
		int rollclassgrade = 0;
		TCHAR tempparsingbuffer[256];
		TCHAR tempparsingbuffer2[256];
		
		memset( g_CharacterRollNameTable, 0, sizeof(TCHAR) * _XGROUP_MAXGROUPCOUNT * _XROLLCLASS_MAXCOUNT * 5 * 64 );
		
		while( 1 )
		{			
			if( feof(pScriptFile) ) break;
			
			memset( parsingString, 0, 512 );
			if( !fgets( parsingString, 511, pScriptFile ) ) break;
			if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;
			
			if( strncmp( parsingString, "_XM_EOS", 7 ) == 0 )
			{
				break;			
			}
			
			if( strncmp( parsingString, _T("_XC_GROUP"), strlen(_T("_XC_GROUP")) ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &groupindex );
			}
			else if( strncmp( parsingString, _T("_XC_ROLLNAME"), strlen(_T("_XC_ROLLNAME"))  ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &rollclassgrade );
				int dummy = rollclassgrade;
				sscanf( parsingString, "%s %d %s", tempparsingbuffer, &dummy, tempparsingbuffer2 );
				
				LPTSTR pszPtr = strstr( parsingString, tempparsingbuffer2 );

				strcpy( g_CharacterRollNameTable[groupindex][rollclass][rollclassgrade], pszPtr );
			}
			else if( strncmp( parsingString, _T("_XC_ROLL"), strlen(_T("_XC_ROLL"))  ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &rollclass );
			}		
		}
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
			pScriptFile = g_ScriptArchive.GetPackedFile( _T("us_RollName_Table.xms") );
		else if(g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN)
			pScriptFile = g_ScriptArchive.GetPackedFile( _T("rs_RollName_Table.xms") );			
				
		if( !pScriptFile )
		{
			_XFatalError( "Load Roll name table( %s ) File open", _T("us_RollName_Table.xms") );
			g_ScriptArchive.ClosePackage();
			return; 
		}
		
		int groupindex = 0;
		int rollclass = 0;
		int rollclassgrade = 0;
		TCHAR tempparsingbuffer[256];
		TCHAR tempparsingbuffer2[256];
		
		memset( g_CharacterRollNameTable, 0, sizeof(TCHAR) * _XGROUP_MAXGROUPCOUNT * _XROLLCLASS_MAXCOUNT * 5 * 64 );
		
		while( 1 )
		{			
			if( feof(pScriptFile) ) break;
			
			memset( parsingString, 0, 512 );
			if( !fgets( parsingString, 511, pScriptFile ) ) break;
			if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;
			
			if( strncmp( parsingString, "_XM_EOS", 7 ) == 0 )
			{
				break;			
			}
			
			if( strncmp( parsingString, _T("_XC_GROUP"), strlen(_T("_XC_GROUP")) ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &groupindex );
			}
			else if( strncmp( parsingString, _T("_XC_ROLLNAME"), strlen(_T("_XC_ROLLNAME"))  ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &rollclassgrade );
				int dummy = rollclassgrade;
				sscanf( parsingString, "%s %d %s", tempparsingbuffer, &dummy, tempparsingbuffer2 );
				
				LPTSTR pszPtr = strstr( parsingString, tempparsingbuffer2 );
				
				strcpy( g_CharacterRollNameTable[groupindex][rollclass][rollclassgrade], pszPtr );
			}
			else if( strncmp( parsingString, _T("_XC_ROLL"), strlen(_T("_XC_ROLL"))  ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &rollclass );
			}		
		}
	}
	else
	{
		if(g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE)
			pScriptFile = g_ScriptArchive.GetPackedFile(_T("TW_Rollname_Table.xms"));
		else if(g_LanguageType == _XLANGUAGE_TYPE_JAPANESE)
			pScriptFile = g_ScriptArchive.GetPackedFile(_T("JP_Rollname_Table.xms"));
		else
			pScriptFile = g_ScriptArchive.GetPackedFile( _T("RollName_Table.xms") );
		
		if( !pScriptFile )
		{
			_XFatalError( "Load Roll name table( %s ) File open", _T("RollName_Table.xms") );
			g_ScriptArchive.ClosePackage();
			return; 
		}

		int groupindex = 0;
		int rollclass = 0;
		int rollclassgrade = 0;
		TCHAR tempparsingbuffer[256];

		memset( g_CharacterRollNameTable, 0, sizeof(TCHAR) * _XGROUP_MAXGROUPCOUNT * _XROLLCLASS_MAXCOUNT * 5 * 64 );
		
		while( 1 )
		{			
			if( feof(pScriptFile) ) break;
			
			memset( parsingString, 0, 512 );
			if( !fgets( parsingString, 511, pScriptFile ) ) break;
			if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;
			
			if( strncmp( parsingString, "_XM_EOS", 7 ) == 0 )
			{
				break;			
			}
		
			if( strncmp( parsingString, _T("_XC_GROUP"), strlen(_T("_XC_GROUP")) ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &groupindex );
			}
			else if( strncmp( parsingString, _T("_XC_ROLLNAME"), strlen(_T("_XC_ROLLNAME"))  ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &rollclassgrade );
				int dummy = rollclassgrade;
				sscanf( parsingString, "%s %d %s", tempparsingbuffer, &dummy, g_CharacterRollNameTable[groupindex][rollclass][rollclassgrade] );
			}
			else if( strncmp( parsingString, _T("_XC_ROLL"), strlen(_T("_XC_ROLL"))  ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &rollclass );
			}		
		}
	}

	g_ScriptArchive.ClosePackage();

	//_XLog( "Notice : roll name table loaded" );
}

void _XLocalUser::RecalcLocalUserParameter( void )
{	
	UpdateLocalUserLevel();

	if( g_pJinStatus_Window )
	{
		g_pJinStatus_Window ->SetParameter();
	}
}

int _XLocalUser::CheckPartyMember( int partymemberuniqueid )
{	
	if( m_InParty )
	{
		for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)// 2004.06.01->oneway48 modify : 15->9
		{
			if( m_PartyInfo[i].playeruniqueid == partymemberuniqueid )
			{
				return i;
			}
		}
	}
	else
	{
		return -1;
	}
	return -1;
}

// ���� ������ ������ ������Ʈ
void _XLocalUser::UpdateLocalUserLevel( void )
{		
	m_CharacterInfo.Set_level( g_UserLevelTable[m_CharacterInfo.Get_innerlevel()].level );
	m_CharacterInfo.Set_sublevel( g_UserLevelTable[m_CharacterInfo.Get_innerlevel()].sublevel );
	
	UpdateLocalUserAttackLevel();	
}

void _XLocalUser::UpdateLocalUserAttackLevel(void)
{
	// ���� Data clear
	m_P_AttackDamage.Set_applyCount(0);				// ���ݷ�
	m_P_AttackRate.Set_applyCount(0);					// ���� ����ġ
	m_P_Defence.Set_applyCount(0);						// ����
	m_P_AvoidRate.Set_applyCount(0);					// ȸ�Ƿ�
	m_M_AttackDamage.Set_applyCount(0);				// ��� ���ݷ�
	m_M_AttackRate.Set_applyCount(0);					// ��� ����ġ
	m_M_Defence.Set_applyCount(0);						// ��� ����
	m_M_AvoidRate.Set_applyCount(0);					// ��� ȸ�Ƿ�

	m_P_AttackDamage.Set_applyCountAdditional(0);				// ���ݷ�
	m_P_AttackRate.Set_applyCountAdditional(0);				// ���� ����ġ
	m_P_Defence.Set_applyCountAdditional(0);					// ����
	m_P_AvoidRate.Set_applyCountAdditional(0);					// ȸ�Ƿ�
	m_M_AttackDamage.Set_applyCountAdditional(0);				// ��� ���ݷ�
	m_M_AttackRate.Set_applyCountAdditional(0);				// ��� ����ġ
	m_M_Defence.Set_applyCountAdditional(0);					// ��� ����
	m_M_AvoidRate.Set_applyCountAdditional(0);					// ��� ȸ�Ƿ�
	
	m_Strength.Set_applyCount(0);						// �ٷ�
	m_Strength.Set_bBuff(0);
	m_Zen.Set_applyCount(0);							// ����
	m_Zen.Set_bBuff(0);
	m_Intelligence.Set_applyCount(0);					// ����
	m_Intelligence.Set_bBuff(0);
	m_Dex.Set_applyCount(0);							// ��ø
	m_Dex.Set_bBuff(0);
	m_Constitution.Set_applyCount(0);					// �ǰ�
	m_Constitution.Set_bBuff(0);
	
	m_MaxLifePower.Set_applyCount(0);					// �ִ� ����
	m_MaxForcePower.Set_applyCount(0);					// �ִ� ����
	m_MaxConcentrationPower.Set_applyCount(0);			// �ִ� ����
	
	m_MaxLifePower.Set_applyCountAdditional(0);			// �ִ� ����
	m_MaxForcePower.Set_applyCountAdditional(0);			// �ִ� ����
	m_MaxConcentrationPower.Set_applyCountAdditional(0);	// �ִ� ����
	
#ifdef _XTS_BASEMINMAXWEAPONDAMAGE
	m_P_AttackDamage.bonus_min = 0.f;				// �ּ� ���ݷ�
	m_P_AttackDamage.bonus_max = 0.f;				// �ִ� ���ݷ�
#else
	m_P_AttackDamage.bonus = 0.f;					// ���ݷ�
#endif
	m_P_AttackRate.bonus = 0.f;						// ���� ����ġ
	m_P_Defence.bonus = 0.f;						// ����
	m_P_AvoidRate.bonus = 0.f;						// ȸ�Ƿ�
	m_M_AttackDamage.bonus = 0.f;					// ��� ���ݷ�
	m_M_AttackRate.bonus = 0.f;						// ��� ����ġ
	m_M_Defence.bonus = 0.f;						// ��� ����
	m_M_AvoidRate.bonus = 0.f;						// ��� ȸ�Ƿ�
	
	m_Strength.bonus = 0.f;							// �ٷ�
	m_Zen.bonus = 0.f;								// ����
	m_Intelligence.bonus = 0.f;						// ����
	m_Dex.bonus = 0.f;								// ��ø
	m_Constitution.bonus = 0.f;						// �ǰ�
	
	m_MaxLifePower.bonus = 0.f;						// �ִ� ����
	m_MaxForcePower.bonus = 0.f;					// �ִ� ����
	m_MaxConcentrationPower.bonus = 0.f;			// �ִ� ����
	
	Set_m_MoveSpeedFromEffect(0);						// �̵� �ӵ�

	m_ResistMinus		= 0;						// ���� ���׷�
	m_ResistPlus		= 0;						// ���� ���׷�
	m_ResistHon			= 0;						// ȥ�� ���׷�
	m_ResistSin			= 0;						// ���� ���׷�

	m_AttackDamageMinus	= 0;
	m_AttackDamagePlus	= 0;
	m_AttackDamageHon	= 0;
	m_AttackDamageSin	= 0;

	memset(m_PenaltyValueFromItem, 0, sizeof(m_PenaltyValueFromItem));	// penalty from item
	memset(m_WeaponMinDamageBonus, 0, sizeof(m_WeaponMinDamageBonus));	// weapon min damage bonus
	memset(m_WeaponMaxDamageBonus, 0, sizeof(m_WeaponMaxDamageBonus));	// weapon max damage bonus
	
	// disable skill / disable stype
	m_Lock.disableskill = 0;
	m_Lock.disableskillindex = 0;
	m_Lock.disablestype = 0;
	m_Lock.disablestypeindex = 0;
	
	// Reset Skill mod Table
#ifdef _XTS_MASTERY
	_XSkillItem::ResetSkillModTable();
#endif

	// Bonus ���
	ApplyCharacterStateBonus();

	// ���������� ���� Penalty ���
	ApplyCharacterStatePenalty();

	// ���� �� ���
	ApplyCharacterStateFinal();

	if( g_pJinStatus_Window )
	{
		g_pJinStatus_Window->SetParameter();
	}
	
	// �ܻ�/���� ���� �ٽ� ����ϱ� - 08.08.19 Author:�ڼ���

	// �ܻ��� ���� ���
	if(g_pLocalUser->m_CharacterInfo.Get_externalwoundrate() > 0)
	{
		if(g_pState_Window)
		{
			FLOAT percentage = (((FLOAT)g_pLocalUser->m_CharacterInfo.Get_externalwoundrate())/(FLOAT)g_pLocalUser->m_MaxLifePower.Get_result())*100.0f;
			if(percentage >= 40.0f)
				g_pState_Window->InsertStateListFromExIn(_XST_EXTERNAL, 0, g_pLocalUser->m_CharacterInfo.Get_externalwoundrate());
			else
				g_pState_Window->DeleteStateList(_XST_EXTERNAL, 0);
		}
	}

	// ������ ���� ���
	if(g_pLocalUser->m_CharacterInfo.Get_internalwoundrate() > 0)
	{
		if(g_pState_Window)
		{
			FLOAT percentage = (((FLOAT)g_pLocalUser->m_CharacterInfo.Get_internalwoundrate())/(FLOAT)g_pLocalUser->m_MaxForcePower.Get_result())*100.0f;
			if(percentage >= 40.0f)
				g_pState_Window->InsertStateListFromExIn(_XST_INTERNAL, 0, g_pLocalUser->m_CharacterInfo.Get_internalwoundrate());
			else
				g_pState_Window->DeleteStateList(_XST_INTERNAL, 0);
		}
	}
}

void _XLocalUser::ApplyCharacterStateBonus(void)
{
#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : ����� //breif : ��Ʈ ������
	g_pSetIteminfo->GroupItemSkillUpdate();
#endif

	// ���� ���
	// Skill, Effect, Condition, Item, ����
	m_StateList.ApplyBonusPoint();

	// ������ ���
	// Skill, Effect, Condition, Item, ����
	m_StateList.ApplyBonusAdditional();
}

void _XLocalUser::ApplyCharacterStatePenalty(void)
{
	// ���� - �䱸 ���� ������ ���� �ʴ� ��� 
	short reqLevel = 0;

#ifdef _XTS_ITEM_OPTIMIZATION
	if( _XGI_FC_WEAPON == GetEqualItemType(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType()) )
	{
		_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType());
		if( WeaponItem )
		{
			reqLevel = WeaponItem[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID()].sReqLevel;

			int nPercent = 0;
			int diff = reqLevel - m_CharacterInfo.Get_innerlevel();
			if(diff > 0)
			{
				if( reqLevel != 0 )
				{
					nPercent = diff * 100 / reqLevel;
					if(nPercent <= 20)
					{
						// 20%���� ���ݷ� 10% �϶�
						m_PenaltyValueFromItem[0] -= 10;
					}
					else if(nPercent <= 50)
					{
						// 50%���� ���ݷ� 30% �϶�
						m_PenaltyValueFromItem[0] -= 30;
					}
					else
					{
						// ���ݷ� 50% �϶�
						m_PenaltyValueFromItem[0] -= 50;
					}
				}
			}
			
			// ���� - �䱸 ��ø�� ���� �ʴ� ���
			short reqDex = 0;
			reqDex = WeaponItem[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID()].sReqSim;
			
			short dex = 0;
			nPercent = 0;
			diff = 0;
			
			if(g_pLocalUser->m_Dex.Get_applyCount() > 0)
			{
				dex = g_pLocalUser->m_Dex.Get_org() + g_pLocalUser->m_Dex.bonus;
			}
			else
			{
				dex = g_pLocalUser->m_Dex.Get_org();
			}
			
			// ���� �� �����ֱ�
			dex += m_ElixirInfo.GetTotalBonus(4);
			
			diff = reqDex - dex;
			if(diff > 0)
			{
				if( reqDex != 0 )
				{
					nPercent = diff * 100 / reqDex;
					if(nPercent <= 20)
					{
						// 20%���� ���ݼ���ġ 5% �϶� (���� ����)
						m_PenaltyValueFromItem[1] += 5;
					}
					else if(nPercent <= 50)
					{
						// 50%���� ���ݼ���ġ 10% �϶� (���� ����)
						m_PenaltyValueFromItem[1] += 10;
					}
					else
					{
						// ���ݼ���ġ 20% �϶� (���� ����)
						m_PenaltyValueFromItem[1] += 20;
					}
				}
			}
		}
	}	
#else
	if(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType() == _XGI_FC_WEAPON)
		reqLevel = g_WeaponItemProperty[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_sID].sReqLevel;
	else if(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType() == _XGI_FC_WEAPON2)
		reqLevel = g_WeaponItemProperty2[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_sID].sReqLevel;
	else if(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType() == _XGI_FC_WEAPON3)
		reqLevel = g_WeaponItemProperty3[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_sID].sReqLevel;

	int nPercent = 0;
	int diff = reqLevel - m_CharacterInfo.innerlevel;
	if(diff > 0)
	{
		if( reqLevel != 0 )
		{
			nPercent = diff * 100 / reqLevel;
			if(nPercent <= 20)
			{
				// 20%���� ���ݷ� 10% �϶�
				m_PenaltyValueFromItem[0] -= 10;
			}
			else if(nPercent <= 50)
			{
				// 50%���� ���ݷ� 30% �϶�
				m_PenaltyValueFromItem[0] -= 30;
			}
			else
			{
				// ���ݷ� 50% �϶�
				m_PenaltyValueFromItem[0] -= 50;
			}
		}
	}

	// ���� - �䱸 ��ø�� ���� �ʴ� ���
	short reqDex = 0;
	if(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType() == _XGI_FC_WEAPON)
		reqDex = g_WeaponItemProperty[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_sID].sReqSim;
	else if(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType() == _XGI_FC_WEAPON2)
		reqDex = g_WeaponItemProperty2[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_sID].sReqSim;
	else if(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType() == _XGI_FC_WEAPON3)
		reqDex = g_WeaponItemProperty3[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_sID].sReqSim;

	short dex = 0;
	nPercent = 0;
	diff = 0;

	if(g_pLocalUser->m_Dex.applyCount > 0)
	{
		dex = g_pLocalUser->m_Dex.org + g_pLocalUser->m_Dex.bonus;
	}
	else
	{
		dex = g_pLocalUser->m_Dex.org;
	}

	// ���� �� �����ֱ�
	dex += m_ElixirInfo.GetTotalBonus(4);

	diff = reqDex - dex;
	if(diff > 0)
	{
		if( reqDex != 0 )
		{
			nPercent = diff * 100 / reqDex;
			if(nPercent <= 20)
			{
				// 20%���� ���ݼ���ġ 5% �϶� (���� ����)
				m_PenaltyValueFromItem[1] += 5;
			}
			else if(nPercent <= 50)
			{
				// 50%���� ���ݼ���ġ 10% �϶� (���� ����)
				m_PenaltyValueFromItem[1] += 10;
			}
			else
			{
				// ���ݼ���ġ 20% �϶� (���� ����)
				m_PenaltyValueFromItem[1] += 20;
			}
		}
	}
#endif

	// ���� - �䱸 �ٷ¿� ���� �ʴ� ��� : ġ��Ÿ�� �϶� (���÷����� �ʿ� ����)

	// ���� - ������ �϶����� ���
	FLOAT fPercent = 0;
	if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_usMaxDur() != 0 )
	{
		fPercent = (FLOAT)g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_usCurDur() / 
						(FLOAT)g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_usMaxDur() * 100.0f;
	}
	if(fPercent > 0.0f)
	{
		if(fPercent <= 10)
		{
			// ������ 10% ���� : ���ݷ� 40% ����, ���ݼ���ġ 4��(300%�϶�)
			m_PenaltyValueFromItem[0] -= 40;
			m_PenaltyValueFromItem[1] += 300;
		}
		else if(fPercent <= 20)
		{
			// ������ 20% ���� : ���ݷ� 30% ����, ���ݼ���ġ 3��(200%�϶�)
			m_PenaltyValueFromItem[0] -= 30;
			m_PenaltyValueFromItem[1] += 200;
		}
		else if(fPercent <= 40)
		{
			// ������ 40% ���� : ���ݷ� 10% ����, ���ݼ���ġ 2��(100%�϶�)
			m_PenaltyValueFromItem[0] -= 10;
			m_PenaltyValueFromItem[1] += 100;
		}
	}
}

void _XLocalUser::ApplyCharacterStateFinal(void)
{
	// �ִ�, �ּҰ� ����
	if(Get_m_MoveSpeedFromEffect() < -90)
		Set_m_MoveSpeedFromEffect(-90);
	if(Get_m_MoveSpeedFromEffect() > 100)
		Set_m_MoveSpeedFromEffect(100);
	
	// ���� �� ���
	m_StateList.ApplyFinalValue();

	// �ּҰ� ����
	if(m_MaxLifePower.Get_applyCount() > 0 || m_MaxLifePower.Get_applyCountAdditional() > 0)
	{
		if(m_MaxLifePower.Get_result() < g_UsePointTable[m_CharacterInfo.Get_cClass()].vitality)
		{
			m_MaxLifePower.Set_result( g_UsePointTable[m_CharacterInfo.Get_cClass()].vitality );
		}
	}
	if(m_MaxForcePower.Get_applyCount() > 0 || m_MaxForcePower.Get_applyCountAdditional() > 0)
	{
		if(m_MaxForcePower.Get_result() < g_UsePointTable[m_CharacterInfo.Get_cClass()].inner)
		{
			m_MaxForcePower.Set_result( g_UsePointTable[m_CharacterInfo.Get_cClass()].inner );
		}
	}
	if(m_MaxConcentrationPower.Get_applyCount() > 0 || m_MaxConcentrationPower.Get_applyCountAdditional() > 0)
	{
		if(m_MaxConcentrationPower.Get_result() < g_UsePointTable[m_CharacterInfo.Get_cClass()].concenturate)
		{
			m_MaxConcentrationPower.Set_result( g_UsePointTable[m_CharacterInfo.Get_cClass()].concenturate );
		}
	}

	// ���� ȥ�� ������ ����
#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
	m_P_MinAttackDamage += m_AttackDamageMinus;
	m_P_MinAttackDamage += m_AttackDamagePlus;
	m_P_MinAttackDamage += m_AttackDamageHon;
	m_P_MinAttackDamage += m_AttackDamageSin;

	m_P_MaxAttackDamage += m_AttackDamageMinus;
	m_P_MaxAttackDamage += m_AttackDamagePlus;
	m_P_MaxAttackDamage += m_AttackDamageHon;
	m_P_MaxAttackDamage += m_AttackDamageSin;

	m_M_MinAttackDamage += m_AttackDamageMinus;
	m_M_MinAttackDamage += m_AttackDamagePlus;
	m_M_MinAttackDamage += m_AttackDamageHon;
	m_M_MinAttackDamage += m_AttackDamageSin;

	m_M_MaxAttackDamage += m_AttackDamageMinus;
	m_M_MaxAttackDamage += m_AttackDamagePlus;
	m_M_MaxAttackDamage += m_AttackDamageHon;
	m_M_MaxAttackDamage += m_AttackDamageSin;
	
	// ����ȥ�� ������ �� �ִ밪 ����
	m_CharacterInfo.m_4Eff_MaxDam = FindMaxDamage();

	// ���� ȥ�� ���� �г�Ƽ ����
	m_ResistMinus += g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonepenalty;
	m_ResistPlus += g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonepenalty;
	m_ResistHon += g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonepenalty;
	m_ResistSin += g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonepenalty;
	
#endif

}

_XEN_4EFF_SYS _XLocalUser::FindMaxDamage()
{
	int damage[4];
	int maxindex = 0;

	damage[0] = m_AttackDamageMinus;
	damage[1] = m_AttackDamagePlus;
	damage[2] = m_AttackDamageHon;
	damage[3] = m_AttackDamageSin;

	int max = damage[0];

	for(int i = 1 ; i < 4 ; ++i)
	{
		if(max < damage[i])
		{
			max = damage[i];
			maxindex = i;
		}
		else if(max == damage[i])
		{
			int rad = _XRandom() % 2;
			if(rad == 1)
				maxindex = i;
		}
	}
	
	if(max <= 0)
	{
		return _XEN_4EFF_NONE;
	}
	else
	{
		switch(maxindex)
		{
		case 0 :
			return _XEN_4EFF_MINUS;
		case 1 :
			return _XEN_4EFF_PLUS;
		case 2 :
			return _XEN_4EFF_HON;
		case 3 :
			return _XEN_4EFF_SIN;
		}
	}

	return _XEN_4EFF_NONE;
}

void _XLocalUser::GetUserLevel( DWORD innerlevel, DWORD& level, DWORD& sublevel )
{
	level	   = g_UserLevelTable[innerlevel].level;
	sublevel   = g_UserLevelTable[innerlevel].sublevel;
}

void _XLocalUser::SelectCharacter( int characterindex )
{
	if( characterindex >= m_CharacterCount )
	{
		_XDWINPRINT( "WARNING : _XLocalUser::SelectCharacter : Invalid character index");
		return;
	}

	m_SelectedCharacter =	characterindex;

	memcpy( &m_CharacterInfo, &m_AllCharacterInfo[m_SelectedCharacter], sizeof( _XCHARACTER_INFO ) );

	RecalcLocalUserParameter();	

	if( m_CharacterInfo.gender == _XGENDER_MAN )	
		m_ModelDescriptor.LinkBipedObject( &g_MaleBipedObject );
	else
		m_ModelDescriptor.LinkBipedObject( &g_FemaleBipedObject );

	SetCharacterModel();	
}

void _XLocalUser::SetDieMotion(void)
{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
	if(!m_bIsBossBattle && !GetEnterMatchEvent()) 
#else
	if( !m_bIsBossBattle )
#endif
	{
		// ��Ȱ ������ ����
		_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
		if( pRebirth_Window && !g_ZeroLevelMode && m_UserState != _XDEF_USERSTATE_MATCH_FIGHT)
		{
			pRebirth_Window->SetRebirthMode(_XREBIRTH_HONSU);
//			pRebirth_Window->SetCastleBattleState(bCBMode);

#ifdef _ACCLAIM_IGAADSYSTEM			
			if( g_IGAADWrapper.CheckExistContent( _XIGAADZONE_DEATHVIDEOAD ) )
			{
				pRebirth_Window->m_ShowIGAADButton = TRUE;
			}
			else
			{
				pRebirth_Window->m_ShowIGAADButton = FALSE;
			}
#endif

			pRebirth_Window->ShowWindow(TRUE);
		}
	}

	SetAnimation( _XACTION_DIE );
	m_ModelDescriptor.SetLockEndFrame( TRUE );

	// display ���� ���� ���� ��� ó��
	ProcessMobDie();
	
	ResetViewTarget();
	ResetAttackTarget();
	ResetSkillAnimation();

#ifdef _XDEF_PARTY_NOTAVAILABLE
	// �ڸ���� �̸� ����
	if(m_bOut)
	{
		SetOut(FALSE);
	}
#endif

}

void _XLocalUser::SetBlowMotion(void)
{
	if(GetMotionClass() == _XACTION_DIE)
		return;

	ProcessMobDie();

	if(GetMotionClass() == _XACTION_RESCUE)
		return;

	char usefultype = 0;
	if(m_SelectedSkillID > 0)
		usefultype = g_SkillProperty[m_SelectedSkillID]->usefulType;
	else 
		usefultype = _XSI_USEFULTYPE_ACTIVESKILL;

	switch(usefultype)
	{
	case _XSI_USEFULTYPE_CHARGESKILL :
	case _XSI_USEFULTYPE_CASTINGSPELL :
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
	case _XSI_USEFULTYPE_FINISHSKILL :
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
	case _XSI_USEFULTYPE_HIDDENMA :
		return;
	default :
		break;
	}

	SetAnimation(_XACTION_BLOW);
}

void _XLocalUser::SetDefenceMotion(void)
{
	if(GetMotionClass() == _XACTION_DIE)
		return;

	ProcessMobDie();

	if(GetMotionClass() == _XACTION_RESCUE)
		return;

	char usefultype = 0;
	if(m_SelectedSkillID > 0)
		usefultype = g_SkillProperty[m_SelectedSkillID]->usefulType;
	else 
		usefultype = _XSI_USEFULTYPE_ACTIVESKILL;

	switch(usefultype)
	{
	case _XSI_USEFULTYPE_CHARGESKILL :
	case _XSI_USEFULTYPE_CASTINGSPELL :
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
	case _XSI_USEFULTYPE_FINISHSKILL :
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
	case _XSI_USEFULTYPE_HIDDENMA :
		return;
	default :
		break;
	}

	SetAnimation(_XACTION_DEFENCE);

	// Defence �ϴ� sound	
	if( m_CharacterInfo.modeldescript.item1 == 0 )
	{
		_XPlaySoundEffect( g_WeaponSoundIndexList[ 4 ], m_Position );
	}
	else
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
			if( WeaponItem )
			{
				int weaponmaterial = WeaponItem[m_CharacterInfo.modeldescript.item1].ucQuality2;		
				if( weaponmaterial < g_WeaponSoundCount )
				{
					_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
				}
			}
		}
#else
		if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
		{
			int weaponmaterial = g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].ucQuality2;		
			if( weaponmaterial < g_WeaponSoundCount )
			{
				_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )		
		{
			int weaponmaterial = g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].ucQuality2;
			if( weaponmaterial < g_WeaponSoundCount )
			{
				_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )		
		{
			int weaponmaterial = g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].ucQuality2;
			if( weaponmaterial < g_WeaponSoundCount )
			{
				_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
			}
		}
#endif
	}
}

void _XLocalUser::SetRebirthBinsaMode(void)
{
	m_DyingStartTime = 0;
	m_DyingTimer = 0;
	m_PrevMoveTime = 0;
	m_LastCheckedDyingStartTime = -1;

	ResetSkillAnimation();
	ResetTrainingMode();
	_XUser::SetRebirthBinsaMode();

	_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
	if(pRebirth_Window)
	{
		if(pRebirth_Window->GetShowStatus())
		{
			pRebirth_Window->ShowWindow(FALSE);
		}
	}
}


BOOL _XLocalUser::GetNextMove(D3DXVECTOR3& next)
{
	if( !gApp->m_bHasFocus				   ||
		GetMotionClass() == _XACTION_DYING || 
		GetMotionClass() == _XACTION_DIE   || 
		GetMotionClass() == _XACTION_REBIRTH )
	{
		m_HiperRunFailedFlag = TRUE;
		m_TargetPosition = m_Position;
		next = m_Position;

        //Author : ����� //breif : ��� ��� �� ��Ŀ���� �Ҿ��� �� ������ ó��.. �̰��� ���� ���ڴ�.
        if( m_SelectedSkillID && m_UseSpeedSkill )
		{
			if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
				_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF &&
				g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
			{
                m_UseSpeedSkill = FALSE;
				m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
				m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);
			}
		}
		return FALSE;
	}

	FLOAT dx = fabs(m_TargetPosition.x - m_Position.x);
	FLOAT dz = fabs(m_TargetPosition.z - m_Position.z);
	FLOAT leftlength = _XFC_sqrt(dx*dx + dz*dz);	

	if( leftlength < EPSILON3 )
	{
		if( m_PathNodeCount > 0 ) // �н��� ���Ǿ��ٸ� 
		{
			if( m_CurrentNavigatePathNodeIndex < m_PathNodeCount-1 ) // ���� �н� ����Ʈ�� �ִٸ� ���� ����Ʈ �����ϰ� ��� �̵�
			{
				m_CurrentNavigatePathNodeIndex++;
				
				if( m_CurrentNavigatePathNodeIndex == m_PathNodeCount-1 )
				{
					m_TargetPosition.x = m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.x;
					m_TargetPosition.z = m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.y;
				}
				else
				{
					m_TargetPosition.x = ( m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.x + g_PathFinder.m_CollideArrayStartPosition.x) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
					m_TargetPosition.z = ( m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.y + g_PathFinder.m_CollideArrayStartPosition.y) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
				}
				
				m_LastRotateAngle = _XMeshMath_GetRotateRadian(m_Position, m_TargetPosition);
				
				FLOAT elapsedangle_1 = fabs(m_LastRotateAngle - m_RotateAngle);
				FLOAT elapsedangle_2 = (_X_PI - fabs(m_LastRotateAngle)) + (_X_PI - fabs(m_RotateAngle));
				
				if(elapsedangle_1 <= elapsedangle_2)
				{
					if(m_LastRotateAngle >= m_RotateAngle)
						m_RotateDirection = _XROTATE_CLOCKWISE;
					else
						m_RotateDirection = _XROTATE_COUNTCLOCKWISE;
				}
				else
				{
					if(m_LastRotateAngle >= m_RotateAngle)
						m_RotateDirection = _XROTATE_COUNTCLOCKWISE;
					else
						m_RotateDirection = _XROTATE_CLOCKWISE;
				}

				//m_PrevMoveTime = 0;
			}
			else
			{
				m_PathNodeCount = 0;	// �н� ���ε� ����Ʈ ������ �ʱ�ȭ �Ͽ� ���߰� ��
				m_TargetPosition = m_Position;
				next = m_Position;
				m_PrevMoveTime = 0;
				return FALSE;
			}
		}
		else
		{
			m_TargetPosition = m_Position;
			next = m_Position;
			m_PrevMoveTime = 0;
			return FALSE;
		}
	}	

	FLOAT movespeed = 0;	


	// ��� ��� 	
	if( g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
		g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )	// ���
	{
		if(gpInput->CheckKeyPress(DIK_LMENU) && !m_HiperRunFailedFlag )
		{
			if(m_SelectedSkillItem)
			{
				if( m_SelectedSkillItem->m_dwCooldownStartTime > 0 )
				{
					m_HiperRunFailedFlag = TRUE;
					return FALSE;
				}
			}

			if( m_CharacterInfo.Get_current_forcepower() <= 0 )
			{
				if( m_UseSpeedSkill )
				{
					m_UseSpeedSkill = FALSE;
					ResetSkillAnimation();
					m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
					SetAnimation( _XACTION_IDLE );
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_PathNodeCount = 0;	// �н� ���ε� ����Ʈ ������ �ʱ�ȭ �Ͽ� ���߰� ��
					m_TargetPosition = m_Position;
					next = m_Position;			
					m_HiperRunFailedFlag = TRUE;
					//m_HiperRunStopSignalSended = TRUE;
				}
				return FALSE;
			}
			else
			{
				//m_HiperRunStopSignalSended = FALSE;

				XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;		
				if( !pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView ) 
				{


#ifdef _XDEF_NEWANIMATIONLIST
					
					int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
										  g_SkillProperty[m_SelectedSkillID]->motion_male : 
							              g_SkillProperty[m_SelectedSkillID]->motion_female;
							
					if( m_CharacterInfo.modeldescript.item1 == 0 )
					{
					}
					else
					{
#ifdef _XTS_ITEM_OPTIMIZATION
						if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
						{
							_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
							if( WeaponItem )
							{
								switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
									if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
									}
									else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
										else
											baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
									}
									else
									{
										baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// �庴��
									baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// ������
									baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
									break;
								}
							}
						}
#else
						_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
						
						if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							pWeaponItemProperty = g_WeaponItemProperty;
						else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							pWeaponItemProperty = g_WeaponItemProperty2;
						else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							pWeaponItemProperty = g_WeaponItemProperty3;
						
						switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// �ܺ���
							if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
							}
							else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
								else
									baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
							}
							else
							{
								baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// �庴��
							baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// ������
							baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// �ϱ�
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// �Ǻ���
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// Ư������
							break;
						}
#endif
					}
					
					if( m_ModelDescriptor.GetCurMotion() == baseanimindex ) // start
					{
						movespeed = 0.0f; // ��� �غ��߿��� ����...	
					}
					else if( m_ModelDescriptor.GetCurMotion() == baseanimindex+1 ) // repeat
					{
						if( m_CharacterInfo.Get_current_forcepower() > 0 )
						{
							if( m_SelectedSkillID )
							{
								movespeed = _XSkillItem::GetFinishSpeed(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillItem->m_sSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].finishSpeed; //_XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL1;
							}
							else
							{
								movespeed = _XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL1;
							}
						}
						else
						{
							//movespeed = m_fMoveSpeed;
							ResetSkillAnimation();
							m_UseSpeedSkill = FALSE;
							
							SetAnimation( _XACTION_IDLE );
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
							m_PathNodeCount = 0;	// �н� ���ε� ����Ʈ ������ �ʱ�ȭ �Ͽ� ���߰� ��
							m_TargetPosition = m_Position;
							next = m_Position;
							return FALSE;							
						}
						
						if( m_ChannelingSendLastPacketTime == 0 )
						{
							m_ChannelingSendLastPacketTime = g_LocalSystemTime;
						}
						else
						{
							if( (g_LocalSystemTime - m_ChannelingSendLastPacketTime) > 1000 ) // 1�ʿ� �ѹ��� ���� 
							{
								m_ChannelingSendLastPacketTime = g_LocalSystemTime;// + (g_LocalSystemTime - m_ChannelingSendLastPacketTime - 1000);
								if( m_CharacterInfo.Get_current_forcepower() >= _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel)/*g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce*/ )
								{
									m_CharacterInfo.Set_current_forcepower( m_CharacterInfo.Get_current_forcepower() - _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel) );//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce;
								}
								else
								{
									m_CharacterInfo.Set_current_forcepower(0);
									
									ResetSkillAnimation();
									m_UseSpeedSkill = FALSE;
									
									m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
									SetAnimation( _XACTION_IDLE );
									g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
									m_TargetPosition = m_Position;
									m_PathNodeCount = 0;	// �н� ���ε� ����Ʈ ������ �ʱ�ȭ �Ͽ� ���߰� ��
									next = m_Position;
									return FALSE;
								}
								
								if( g_pJinStatus_Window )
								{
									g_pJinStatus_Window->SetParameter();	
								}		
								
								g_NetworkKernel.SendPacket(MSG_NO_SELF_CHANNEL_START_REQUEST);
							}
						}	
					}			
					else if( m_ModelDescriptor.GetCurMotion() == baseanimindex+2 )  // finish
					{
						movespeed = 0.0f; // ��� �غ��߿��� ����...
					}
								
#else //#ifdef _XDEF_NEWANIMATIONLIST
					
					if( g_LodTerrain.m_3PCamera.mp_fFov >= _XDEF_MAXFOV )
					{
						if( m_CharacterInfo.current_forcepower > 0 )
						{
							if( m_SelectedSkillID )
							{
								movespeed = _XSkillItem::GetFinishSpeed(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillItem->m_sSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].finishSpeed; //_XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL1;
							}
							else
							{
								movespeed = _XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL1;
							}
						}
						else
						{
							//movespeed = m_fMoveSpeed;
							ResetSkillAnimation();
							m_UseSpeedSkill = FALSE;
							
							SetAnimation( _XACTION_IDLE );
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
							m_PathNodeCount = 0;	// �н� ���ε� ����Ʈ ������ �ʱ�ȭ �Ͽ� ���߰� ��
							m_TargetPosition = m_Position;
							next = m_Position;
							return FALSE;
						}

						if( m_ChannelingSendLastPacketTime == 0 )
						{
							m_ChannelingSendLastPacketTime = g_LocalSystemTime;
						}
						else
						{
							if( (g_LocalSystemTime - m_ChannelingSendLastPacketTime) > 1000 ) // 1�ʿ� �ѹ��� ���� 
							{
								m_ChannelingSendLastPacketTime = g_LocalSystemTime;// + (g_LocalSystemTime - m_ChannelingSendLastPacketTime - 1000);
								if( m_CharacterInfo.current_forcepower >= _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel)/*g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce*/ )
									m_CharacterInfo.current_forcepower -= _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce;
								else
								{
									m_CharacterInfo.current_forcepower = 0;
									
									ResetSkillAnimation();
									m_UseSpeedSkill = FALSE;
									
									m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
									SetAnimation( _XACTION_IDLE );
									g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
									m_TargetPosition = m_Position;
									m_PathNodeCount = 0;	// �н� ���ε� ����Ʈ ������ �ʱ�ȭ �Ͽ� ���߰� ��
									next = m_Position;
									return FALSE;
								}

								if( g_pJinStatus_Window )
								{
									g_pJinStatus_Window->SetParameter();	
								}		

								g_NetworkKernel.SendPacket(MSG_NO_SELF_CHANNEL_START_REQUEST);
							}
						}

					}
					else if(g_LodTerrain.m_3PCamera.mp_fFov > g_DefaultCameraFov)
					{
						movespeed = 0.0f; // ��� �غ��߿��� ����...
					}
					else
					{
						//movespeed = m_fMoveSpeed;
						movespeed = 0.0f; // ��� �غ��߿��� ����...
					}
#endif
				}
				else
				{
					//movespeed = m_fMoveSpeed;
					movespeed = 0.0f; // ��� �غ��߿��� ����...
				}
			}
		}
		else
		{
			//m_HiperRunStopSignalSended = FALSE;

			// by mahwang
/*			if (m_UserState == _XDEF_USERSTATE_TRAINING && m_SelectedTrainingMode == _XTRAINING_WATERDRAW)
				movespeed = m_fMoveSpeed * 0.2f;
			else*/

			if(Get_m_MoveSpeedFromEffect() != 0)
			{
				movespeed = Get_m_fMoveSpeed() + (Get_m_fMoveSpeed() * ((FLOAT)Get_m_MoveSpeedFromEffect()/100.0f));
			}
			else
				movespeed = Get_m_fMoveSpeed();
		}
	}
	else
	{
		// by mahwang
/*		if (m_UserState == _XDEF_USERSTATE_TRAINING && m_SelectedTrainingMode == _XTRAINING_WATERDRAW)
			movespeed = m_fMoveSpeed * 0.2f;
		else*/

		if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL && 
			(m_CurrentBattleState == _XUSER_STATE_6 || m_CurrentBattleState == _XUSER_STATE_7))
		{
			movespeed = m_FinishSpeed;
		}
		else if(Get_m_MoveSpeedFromEffect() != 0)
		{
			movespeed = Get_m_fMoveSpeed() + (Get_m_fMoveSpeed() * ((FLOAT)Get_m_MoveSpeedFromEffect()/100.0f));
		}
		else
		{
			movespeed = Get_m_fMoveSpeed();
		}
	}
			
	FLOAT		elapsedtime;
	D3DXVECTOR3 directionvector;
	FLOAT		curtimemovelength;

	if(m_PrevMoveTime == 0.0f)
	{
		m_PrevMoveTime = g_fLocalSystemTime;
		elapsedtime = 0;
		return TRUE;
	}
	else
	{
		XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;	// hotblood zerolevel
		if( pZeroLevelProcess  )
		{
			if( pZeroLevelProcess->GetTutorialModeOn() )
				m_PrevMoveTime = g_fLocalSystemTime;
		}
		elapsedtime= g_fLocalSystemTime - m_PrevMoveTime;
		curtimemovelength  = elapsedtime * movespeed;	// �̹� �����ӿ� �� �Ÿ�

		if( curtimemovelength > leftlength )
			curtimemovelength = leftlength;
	}
	
	if( leftlength < 0.00001f ) // Ÿ�ϰŸ��� ���Ÿ� ���� ª���� �����Ÿ���ŭ ����.
	{	// �̹�Ÿ�ӿ� Ÿ�ٿ� �̵��ȴ�. (��������)
		
		m_TargetPosition = next = m_Position;
		//m_PrevMoveTime = 0.0f;
		
		if(m_AttackTarget)
		{
			if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
			{
				if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
				{
					if(GetMotionClass() != _XACTION_DYING)
					{
//						_XDWINPRINT("[GetNextMotion] Set Attack");
						SetAnimation(_XACTION_ATTACK);
						m_PathNodeCount = 0;	// �н� ���ε� ����Ʈ ������ �ʱ�ȭ �Ͽ� ���߰� ��
					}
				}
			}
			else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
			{
				_XUser* pUser = (_XUser*)m_AttackTarget;
				
				m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pUser->m_Position);
				UpdatePosition();
				
				if(pUser->GetMotionClass() == _XACTION_DYING)
				{
					ProcessRescueUser();
					m_PathNodeCount = 0;
				}
			}
		}
		else
		{
			if(m_LastRotateAngle == m_RotateAngle && !m_UseSpeedSkill)
			{
				if( GetMotionClass() == _XACTION_MOVE )
				{
//					SetAnimation(_XACTION_IDLE);
				}
			} // if(m_LastRotateAngle == m_RotateAngle)
		}

		return FALSE;
	}
	
	// Time slicing...
//	int repeatcollidechecktime = (int) (curtimemovelength / (_XDEF_CHARACTERMOVESPEED_RUN * 30.0f) );
//	FLOAT lastdistance = fmod( curtimemovelength, (_XDEF_CHARACTERMOVESPEED_RUN * 30.0f) );

	int repeatcollidechecktime = (int) (curtimemovelength / (movespeed * 30.0f) );
	FLOAT lastdistance = fmod( curtimemovelength, (movespeed * 30.0f) );
	if( lastdistance > EPSILON4 )
	{
		repeatcollidechecktime++;
	}

	POINT		pt;
	FLOAT		terrainheight = m_Position.y;
	D3DXVECTOR3 nextposition, nextjumpposition, contactpoint, contactfunctionalobjectpoint;
	_XOBBData*  pUnderObb = NULL;
	_XOBBData*  pPreviousGroundCollideOBB = m_pGoundCollideOBB;		

	m_PrevMoveTime = g_fLocalSystemTime;
	
	/*
	if( m_Velocity.y > 0.0f )
	{
		m_Velocity.y += _XDEF_CHARACTERGRAVITY * g_fElapsedFrameMilisecondTime;
		if( m_Velocity.y < 0.0f ) m_Velocity.y = 0.0f;

		FLOAT Length = sqrtf(m_Velocity.y * m_Velocity.y);
		if ( Length > _XDEF_MAXJUMPVELOCITY )
		{
			m_Velocity.y *= ( _XDEF_MAXJUMPVELOCITY / Length );
		}

		nextjumpposition.y += m_Velocity.y * g_fElapsedFrameMilisecondTime;
		
		D3DXVECTOR3 vecDec = -m_Velocity;
		D3DXVec3Normalize( &vecDec, &vecDec );

		Length = D3DXVec3Length( &m_Velocity );
		FLOAT Dec = (_XDEF_CHARACTERFRICTION * g_fElapsedFrameMilisecondTime);
		if ( Dec > Length ) Dec = Length;

		m_Velocity += vecDec * Dec;
		if( m_Velocity.y < 0.0f ) m_Velocity.y = 0.0f;
	}
	
	nextposition = nextjumpposition;

	*/

	// ������ġ
	D3DXVECTOR3 currentposition = m_Position;	
	
	// �� ������ ���� ����	
	directionvector = D3DXVECTOR3(m_TargetPosition.x - currentposition.x, 0.0f, m_TargetPosition.z - currentposition.z) ;
	D3DXVec3Normalize(&directionvector, &directionvector);
	

	// �ּ� �ð������� �ɰ���...
//	FLOAT separatedmovelength = (_XDEF_CHARACTERMOVESPEED_RUN * 30.0f);

	FLOAT separatedmovelength = (movespeed * 30.0f);
	for( int repeat = 0; repeat < repeatcollidechecktime; ++repeat )
	{
		if( (lastdistance > EPSILON4) && repeat == repeatcollidechecktime-1 ) separatedmovelength = lastdistance;

		// ��� Ÿ�ٱ��� �����ϸ� ���� ��ġ ���
		nextposition = currentposition + (directionvector * separatedmovelength);
				 
		_XLODTILE*	pTerrainTile = g_LodTerrain.GetTerrainTile( nextposition , &pt );

		if( !pTerrainTile )
		{
			next = m_Position;
			m_PrevMoveTime = 0.0f;
			return FALSE;
		}

		terrainheight = g_LodTerrain.GetTerrainAndOBBHeightWithCurrentHeight( nextposition, pUnderObb, m_CurrentPositionTerrainNormal );
		
		// ���� �߷°� ���� ���̰� �������� ������쳪 �������� ������ ��ġ ����
		if( terrainheight > currentposition.y && terrainheight < currentposition.y + _XDEF_RIDEOBBLIMITHEIGHT )
		{
			directionvector = D3DXVECTOR3(nextposition.x - currentposition.x, terrainheight - currentposition.y, nextposition.z - currentposition.z) ;
			D3DXVec3Normalize(&directionvector, &directionvector);
			nextposition.y = terrainheight;
		}
		else
		{
			directionvector = D3DXVECTOR3(nextposition.x - currentposition.x, 0.0f, nextposition.z - currentposition.z) ;
			D3DXVec3Normalize(&directionvector, &directionvector);
			nextposition.y = currentposition.y;
		}
		
		m_pGoundCollideOBB = NULL;
		m_pCollideOBB = NULL;
			
		// Rotate obb axis
		D3DXMATRIX	rotmat, newobbmat;
		D3DXMatrixRotationY( &rotmat, 6.28318530717958647692528676624f - m_LastRotateAngle );
		D3DXMatrixMultiply( &newobbmat, &rotmat, &g_BaseOBBAxisMatrix );	
		m_CharacterOBB.m_akAxis[0].x = newobbmat._11;
		m_CharacterOBB.m_akAxis[0].y = newobbmat._21;
		m_CharacterOBB.m_akAxis[0].z = newobbmat._31;
		m_CharacterOBB.m_akAxis[1].x = newobbmat._12;
		m_CharacterOBB.m_akAxis[1].y = newobbmat._22;
		m_CharacterOBB.m_akAxis[1].z = newobbmat._32;
		m_CharacterOBB.m_akAxis[2].x = newobbmat._13;
		m_CharacterOBB.m_akAxis[2].y = newobbmat._23;
		m_CharacterOBB.m_akAxis[2].z = newobbmat._33;
		
		// Expand obb
		FLOAT halfmovelength = separatedmovelength / 2.0f;
		m_CharacterOBB.m_afExtent[2] = 0.32f + halfmovelength;
		m_CharacterOBB.m_kCenter = currentposition + ((nextposition - currentposition) / 2.0f);
		m_CharacterOBB.m_kCenter.y = nextposition.y + 0.89f;
		m_CharacterOBB.RecalcRadius();
		
		// Check collision
		//int collidedetectedcount = 0; // ����� �浹�ϸ� �����

		if( !pTerrainTile->svObbList.empty() && !g_FreeMoveMode )
		{
			_XOBBData* pObbData;
			svdef_OBBData::iterator it;
			for (it = pTerrainTile->svObbList.begin(); it != pTerrainTile->svObbList.end(); ++ it)
			{
				pObbData = (*it);
				if( m_CharacterOBB.IntersectOBBvsOBB( *pObbData, contactpoint ) )
				{
					if( pObbData->m_OBBLevel == 1 )
						m_pCollideOBB = pObbData;

					ProcessCollision(contactpoint, directionvector, nextposition );
					m_CharacterOBB.m_kCenter.x = nextposition.x;
					m_CharacterOBB.m_kCenter.y = nextposition.y + 0.89f;
					m_CharacterOBB.m_kCenter.z = nextposition.z;

					/*
					// Regenerate axis
					FLOAT rotateangle = _XMeshMath_GetRotateRadian(m_Position, nextposition);
					D3DXMatrixRotationY( &rotmat, 6.28318530717958647692528676624f - rotateangle );
					D3DXMatrixMultiply( &newobbmat, &rotmat, &g_BaseOBBAxisMatrix );
					m_CharacterOBB.m_akAxis[0].x = newobbmat._11;
					m_CharacterOBB.m_akAxis[0].y = newobbmat._21;
					m_CharacterOBB.m_akAxis[0].z = newobbmat._31;
					m_CharacterOBB.m_akAxis[1].x = newobbmat._12;
					m_CharacterOBB.m_akAxis[1].y = newobbmat._22;
					m_CharacterOBB.m_akAxis[1].z = newobbmat._32;
					m_CharacterOBB.m_akAxis[2].x = newobbmat._13;
					m_CharacterOBB.m_akAxis[2].y = newobbmat._23;
					m_CharacterOBB.m_akAxis[2].z = newobbmat._33;

					FLOAT dx = fabs(nextposition.x - m_Position.x);
					FLOAT dz = fabs(nextposition.z - m_Position.z);
					FLOAT movelength = _XFC_sqrt(dx*dx + dz*dz);
					
					m_CharacterOBB.m_afExtent[2] = 0.16f + (movelength / 2.0f);
					*/
										
#ifdef _XDWDEBUG
					pObbData->m_Collide	= TRUE;
#endif				
					//collidedetectedcount++;
				}
#ifdef _XDWDEBUG
				else pObbData->m_Collide = FALSE;
#endif					
			}
		}

		if( g_LodTerrain.m_FunctionalObjectManager.m_countFunctionalObject != 0 && !g_FreeMoveMode )
		{
			for( int i = 0; i < g_LodTerrain.m_FunctionalObjectManager.m_countFunctionalObject; ++i )
			{
				if( g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_bCollidable )// �浹 ������ ���� �浹 ó���� �Ѵ�. ( ���� �μ����� �浹ó���� ���� �ʴ´�.)
				{
					if( m_CharacterOBB.IntersectOBBvsOBB( g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i], contactfunctionalobjectpoint ) )
					{
						m_pCollideOBB = &g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i];
						
						ProcessCollision(contactfunctionalobjectpoint, directionvector, nextposition );
						m_CharacterOBB.m_kCenter.x = nextposition.x;
						m_CharacterOBB.m_kCenter.y = nextposition.y + 0.89f;
						m_CharacterOBB.m_kCenter.z = nextposition.z;
					}
#ifdef _XDWDEBUG
					g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_Collide = TRUE;
#endif				
				}
#ifdef _XDWDEBUG
				else g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_Collide = FALSE;
#endif					
			}
		}
		
		currentposition = nextposition;

		terrainheight = g_LodTerrain.GetTerrainAndOBBHeightWithCurrentHeight( currentposition, m_pGoundCollideOBB, m_CurrentPositionTerrainNormal );
		
		if( m_pGoundCollideOBB )
		{
			if( m_Velocity.y < EPSILON4 )
			{
				if( currentposition.y < terrainheight || currentposition.y + _XDEF_RIDEOBBLIMITHEIGHT > terrainheight )
				{
					currentposition.y = terrainheight;
					m_CharacterOBB.m_kCenter.y = currentposition.y + 0.89f;
				}
			}
		}
	}

	if( !m_pGoundCollideOBB )
	{
		if( currentposition.y < terrainheight || currentposition.y + _XDEF_RIDEOBBLIMITHEIGHT > terrainheight )
		{
			currentposition.y = terrainheight;
			m_CharacterOBB.m_kCenter.y = currentposition.y + 0.89f;
		}
	}

	nextposition = currentposition;

#ifdef _XDWDEBUG	
	m_CharacterOBB.CreateOBBModel();
#endif	
	
	if( m_pCollideOBB )
	{
		next = m_Position;
		m_TargetPosition = m_Position;
		m_PrevMoveTime = 0.0f;
		//m_PrevMoveTime = 0.0f;
		m_PathNodeCount = 0;	// �н� ���ε� ����Ʈ ������ �ʱ�ȭ �Ͽ� ���߰� ��

		if( m_UseSpeedSkill )
		{			
			if(m_SelectedSkillItem)
			{
				m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
				m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
			}

			m_HiperRunFailedFlag = TRUE;			
		}

		return FALSE;
	}

	// Check water object
	S_WATER* curwater = g_LodTerrain.m_pWaterObjectManager->GetWaterObject(pt.y + (pt.x * g_LodTerrain.m_TileCount));
	
	if(curwater && curwater->enabled && (curwater->height > (nextposition.y + g_fWaterDepth)))
	{
		next = m_Position;
		m_TargetPosition = m_Position;
		//m_PrevMoveTime = 0.0f;
		m_PathNodeCount = 0;	// �н� ���ε� ����Ʈ ������ �ʱ�ȭ �Ͽ� ���߰� ��

		if( m_UseSpeedSkill )
		{
			if(m_SelectedSkillItem)
			{
				m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
				m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
			}

			m_HiperRunFailedFlag = TRUE;
		}

		return FALSE;
	}
	
	int	xcoord = _XFC_FloatToIntRet( ( nextposition.x + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
	int zcoord = _XFC_FloatToIntRet( ( nextposition.z + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
	if( xcoord < 0 ) xcoord = 0;
	if( zcoord < 0 ) zcoord = 0;
	if( xcoord >= (g_LodTerrain.m_TileCount<<4) ) xcoord = (g_LodTerrain.m_TileCount<<4) - 1;
	if( zcoord >= (g_LodTerrain.m_TileCount<<4) ) zcoord = (g_LodTerrain.m_TileCount<<4) - 1;		

	if( g_LodTerrain.GetPathFindAreaNeighborInfo(zcoord, xcoord ) > 7 )
	{
		next = m_Position;
		m_TargetPosition = m_Position;
		m_PrevMoveTime = 0.0f;
		m_PathNodeCount = 0;	// �н� ���ε� ����Ʈ ������ �ʱ�ȭ �Ͽ� ���߰� ��

		if( m_UseSpeedSkill )
		{
			if(m_SelectedSkillItem)
			{
				m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
				m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
			}

			m_HiperRunFailedFlag = TRUE;
		}

		return FALSE;
	}

	/*
	if( !m_pGoundCollideOBB )
	{
		D3DXQUATERNION normalrotquat;
		D3DXVECTOR3	upvector = D3DXVECTOR3(0.0f,1.0f,0.0f);
		
		// Check terrain slant rate
		if( m_CurrentPositionTerrainNormal.x != 0.0f || m_CurrentPositionTerrainNormal.y != 1.0f || m_CurrentPositionTerrainNormal.z != 0.0f ) // check exactly upvector...
		{			
			// get angle from terrain normal vector...
			FLOAT theta = _XGetAngleFromVectors( &upvector, &m_CurrentPositionTerrainNormal );
			
			if( theta > 0.523598f )
			{
				if( nextposition.y > m_Position.y ) // ���� ����Ʈ�� ���̰� �������� ���� ���Ѵ�.
				{
					FLOAT slowfactor = ( theta - 0.523598f ) / _X_PI_DEV180 * 4.0f;
					nextposition   = m_Position + ( directionvector * (curtimemovelength/slowfactor) );
					nextposition.y = g_LodTerrain.GetTerrainHeight( nextposition , &m_CurrentPositionTerrainNormal );
				}
			}
		}
	}
	*/
	
	m_PrevPosition = m_Position;
	next = nextposition;
	
	if(GetMotionClass() == _XACTION_DYING)
	{
		if(nextposition.x != m_Position.x && nextposition.z != m_Position.z)
		{
			m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
		}
		else return FALSE;
	}
	else if(GetMotionClass() != _XACTION_MOVE)
	{
		if(nextposition.x != m_Position.x || nextposition.z != m_Position.z)
		{
			if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL && 
				(m_CurrentBattleState == _XUSER_STATE_6 || m_CurrentBattleState == _XUSER_STATE_7))
			{
			}
/*#ifdef _XDEF_BLOCK_MOVINGATTACK
			else if(GetMotionClass() == _XACTION_ATTACK) //Author : ����� //breif : �Ѵ�ġ�� ���ϸ��̼��� ����� ��� //date 07/12/03
			{
				//���� �߿� �̵��� ���� ����..
			}
#endif		*/
			else if( m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
				m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY )
			{
				//�λ糪 �ڼ� ���϶��� �̵� �Ұ� 
				// ��� �޼���?
				// ���� ������ ���¿����� �ٽ� �����ϱ� ������ �ٸ� �ൿ�� �� �� ����.
			}
			else
			{
				m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
				SetAnimation(_XACTION_MOVE);

				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else return FALSE;
	}	

	return TRUE;
}

// SetSkillAnimation - Active skill ��
void _XLocalUser::SetSkillAnimation(void)
{	
	if( m_SelectedSkillID == 0 )
	{
		m_MartialStep = 0;
		m_SkillStarLevel = 0;
		
		if(m_CharacterInfo.modeldescript.item1 == 0)
		{
			m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;
		}
		else
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					switch(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// �ǰ�
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;			// �ι�
							else
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// �ܺ���
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �庴��
						}
						break;
					case _XGI_SC_WEAPON_SOFT :
					case _XGI_SC_WEAPON_HIDDEN :
					case _XGI_SC_WEAPON_MUSICAL :
						{
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �ӽ�
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;				// �뺴��
						}
						break;
					}
				}
			}
#else
			if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				switch(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType)
				{
				case _XGI_SC_WEAPON_SHORT :
					{
						if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// �ǰ�
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;			// �ι�
						else
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// �ܺ���
					}
					break;
				case _XGI_SC_WEAPON_LONG :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �庴��
					}
					break;
				case _XGI_SC_WEAPON_SOFT :
				case _XGI_SC_WEAPON_HIDDEN :
				case _XGI_SC_WEAPON_MUSICAL :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �ӽ�
					}
					break;
				case _XGI_SC_WEAPON_SPECIAL :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;				// �뺴��
					}
					break;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				switch(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType)
				{
				case _XGI_SC_WEAPON_SHORT :
					{
						if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// �ǰ�
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;			// �ι�
						else
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// �ܺ���
					}
					break;
				case _XGI_SC_WEAPON_LONG :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �庴��
					}
					break;
				case _XGI_SC_WEAPON_SOFT :
				case _XGI_SC_WEAPON_HIDDEN :
				case _XGI_SC_WEAPON_MUSICAL :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �ӽ�
					}
					break;
				case _XGI_SC_WEAPON_SPECIAL :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;				// �뺴��
					}
					break;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				switch(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType)
				{
				case _XGI_SC_WEAPON_SHORT :
					{
						if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// �ǰ�
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;			// �ι�
						else
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// �ܺ���
					}
					break;
				case _XGI_SC_WEAPON_LONG :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �庴��
					}
					break;
				case _XGI_SC_WEAPON_SOFT :
				case _XGI_SC_WEAPON_HIDDEN :
				case _XGI_SC_WEAPON_MUSICAL :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// �ӽ�
					}
					break;
				case _XGI_SC_WEAPON_SPECIAL :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;				// �뺴��
					}
					break;
				}
			}
#endif
		}
	}
	else
	{
		short motionindex = GetMotionIndex(m_SelectedSkillID);
		
		int maxactioncount = g_SkillProperty[m_SelectedSkillID]->martialNumber;		
		if( maxactioncount > 0 )
		{
			if(m_SelectedSkillItem->m_cSelectedSkillLevel < 2)		// 0, 1 : 1�ʽ�
				m_MartialStep = 0;
			else if(m_SelectedSkillItem->m_cSelectedSkillLevel < 4)	// 2, 3 : 2�ʽ�
			{
				if(maxactioncount >=2)
					m_MartialStep = 1;
				else
					m_MartialStep = 0;
			}
			else if(m_SelectedSkillItem->m_cSelectedSkillLevel < 12) // 4-11 : 3�ʽ�
			{
				if(maxactioncount >= 3)
					m_MartialStep = 2;
				else if(maxactioncount >=2)
					m_MartialStep = 1;
				else
					m_MartialStep = 0;
			}
		}
		else 
		{
			m_MartialStep = 0;
		}
		
		// �ִϸ��̼� �ڸ� �� 
		m_CharacterInfo.animationdescript.attacktype = motionindex;
	}

	m_ModelDescriptor.SetCurMotion( m_CharacterInfo.animationdescript.attacktype);
}

// SetSkillAnimation - �ٸ� skill ��
void _XLocalUser::SetSkillAnimation(int motionindex)
{
	if(motionindex == 0)
	{
		m_MartialStep = 0;
		m_SkillStarLevel = 0;
		m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;		
		m_ModelDescriptor.SetCurMotion(_XAN_NI_ATTACK_KK, TRUE);

		// �̵� ���� �Ǵ� ���� ���۱�ü�ô� �̵����۽ð� �ʱ�ȭ 
		m_PrevMoveTime = 0;
	}
	else
	{
		m_CharacterInfo.animationdescript.attacktype = motionindex;
	}
}

void _XLocalUser::ResetSkillAnimation(bool changeskill)
{
	m_CurrentBattleState = _XUSER_STATE_NONE;
	m_bUsingSkill = FALSE;

	ResetSpellAttackTarget();

	g_WorldEffectInstanceManager.DeleteInstance( _XDEF_SKILLID_1076 );
	//g_WorldEffectInstanceManager.DeleteInstance( m_SelectedSkillID + 40000 );
	if( m_ModelDescriptor.m_EffectManager )
	{
		m_ModelDescriptor.m_EffectManager->DeleteInstance(  m_SelectedSkillID + 40000  );
	}
	
	switch(m_CurrentSelectedSkillType)
	{
	case _XSI_USEFULTYPE_PASSIVESKILL :
		break;
	case _XSI_USEFULTYPE_ACTIVESKILL :
		break;
	case _XSI_USEFULTYPE_CHARGESKILL :
		{
			m_ChargeStartTime = 0;
			m_ChargeIterateValue = 0;
			m_ChargeSendStartPacket = FALSE;
			m_bStartCharge = FALSE;
			m_bUseMouseChargeSkill = FALSE;
		}
		break;
	case _XSI_USEFULTYPE_PASSIVESPELL :
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		{
			m_CastingSpellRepeatTime = 0;
			m_CastingSendPacket = FALSE;
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{
			m_bChannelingSendStartPacket = FALSE;
			m_ChannelingSendLastPacketTime = 0;
			m_ChannelingSpellRepeatTime = 0;
			m_RegionTargetPosX = 0.0f;
			m_RegionTargetPosZ = 0.0f;
		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		{
			m_FinishSpeed = 0;
			m_FinishSendPacket = FALSE;
			m_bSendFinishFirePacket = FALSE;
		}
		break;
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
		{
			m_RoundingDelayStartTime = 0;
			m_RoundingSendPacket = FALSE;
		}
		break;
	case _XSI_USEFULTYPE_HIDDENMA :
		{
			m_HiddenSendLastPacketTime = 0;
			m_HiddenReceivePacketTime = 0;
			m_HiddenSendStartPacket = FALSE;

			_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
			if(pMainMenu_Window)
				pMainMenu_Window->m_bDrawMedEffectInfinite = FALSE;
		}
		break;
	case _XSI_USEFULTYPE_SMASHINGSKILL :
		{
			m_SmashingSendPacket = FALSE;
		}
		break;
	case _XSI_USEFULTYPE_ACTIONSKILL :
		{
			m_ActionSendPacket = FALSE;
		}
		break;
	}
	
	if(GetMotionClass() == _XACTION_ATTACK || GetMotionClass() == _XACTION_IDLE)
	{
		if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL)
			SetAnimation( _XACTION_IDLE, FALSE );
		else 
		{
			SetAnimation(_XACTION_IDLE);
		}

		if(m_CurrentSelectedSkillType != _XSI_USEFULTYPE_ACTIVESKILL)
		{
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
	}
	m_ModelDescriptor.SetLockEndFrame(FALSE);

	// ��� �ִ� ���Ⱑ ���� ��� �������ֱ�
	ReloadWeaponModel();
}

void _XLocalUser::DrawBattle(void)
{
	//2004.06.18->oneway48 insert
	if(m_BattleMode == _XBATTLE_BATTLE)
	{
		
		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );
		
		gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE );	
		gpDev->SetRenderState( D3DRS_DEPTHBIAS , 0 );
		
		D3DXVECTOR3 rvec;
		D3DXVECTOR3	position = m_Position;
		position.y = m_Position.y + 0.25f;
		D3DXVec3Project(&rvec, &position, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(),
			&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);
		
		int X = (int)(rvec.x);
		int Y = (int)(rvec.y);
		
		if(X < 0)
			X = 0;
		if(Y < 0)
			Y = 0;
		
		if( !g_FreeMoveMode )
		{
			if(g_ViewMyBattleGauge)
			{
				m_imageMiniHpBackBorder.Draw( X-54, Y-1 );
				m_MiniHpBorder.MoveWindow( X-45, Y );
				m_MiniHpBorder.Process();
				
				if( !(g_FreeMoveMode && g_InnerUser) )
				{
					m_MiniHpBorder.Draw();
				}
			}
		}
	}

	if(m_SelectedSkillID == 0)
		return;
	
	switch(g_SkillProperty[m_SelectedSkillID]->usefulType)
	{
	case _XSI_USEFULTYPE_PASSIVESKILL :
		break;
	case _XSI_USEFULTYPE_ACTIVESKILL :
		break;
	case _XSI_USEFULTYPE_CHARGESKILL :
		{
			// draw charge skill
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_5 :
			case _XUSER_STATE_6 :
				{
					// draw gauge
					_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
					if(pChargeGauge_Window)
					{
						if(m_ChargeStartTime > 0)
						{
							if(!pChargeGauge_Window->GetShowStatus())
							{
								pChargeGauge_Window->SetGaugeMode(_XGAUGE_CHARGE);
								pChargeGauge_Window->ShowWindow(TRUE);
							}

							FLOAT chargepercentage = 0;
							int casting = 0;
							if(m_SelectedSkillItem->m_cSkillLevel == 0)
								casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].casting;
							else
								casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel)
											- _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel-1);
								//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].casting 
										//	- g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel-1].casting;

							if(casting > 0)
								chargepercentage = ((g_LocalSystemTime - m_ChargeStartTime) / (FLOAT)casting) * 100;
							else
								chargepercentage = 100;

							pChargeGauge_Window->SetParameter(chargepercentage);

							if(m_ChargeIterateValue < m_SelectedSkillItem->m_cSelectedSkillLevel)
							{
								if(chargepercentage >= 100.0f)
								{
									m_ChargeIterateValue++;
									m_ChargeStartTime = g_LocalSystemTime;
									pChargeGauge_Window->ShowWindow(FALSE);
								}
							}
						}
					}
				}
				break;
			default :
				{
					if(GetMotionClass() != _XACTION_DYING )
					{
						_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
						if(pChargeGauge_Window)
						{
							if(pChargeGauge_Window->GetShowStatus())
							{
								if( pChargeGauge_Window->GetCurrentGaugeMode() == _XGAUGE_CHARGE )
									pChargeGauge_Window->ShowWindow(FALSE);
							}
						}
					}
				}
				break;
			}
		}
		break;
	case _XSI_USEFULTYPE_PASSIVESPELL :
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		{
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_5 :
				DrawBattleCastingSpell();
				break;
			default :
				{
					if(GetMotionClass() != _XACTION_DYING )
					{
						_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
						if(pChargeGauge_Window)
						{
							if(pChargeGauge_Window->GetShowStatus())
							{
								if( pChargeGauge_Window->GetCurrentGaugeMode() == _XGAUGE_CHARGE )
									pChargeGauge_Window->ShowWindow(FALSE);
							}
						}
					}
				}
				break;
			}
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{
			// draw channeling spell - other or all 
/*			if(g_SkillProperty[m_SelectedSkillID]->targetType != _XSI_TARGETTYPE_SELF)
			{
				switch(m_CurrentBattleState)
				{
					case _XUSER_STATE_4 :
					case _XUSER_STATE_5 :
					case _XUSER_STATE_6 :
					{
						// draw gauge
						_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
						if(pChargeGauge_Window)
						{
							if(!pChargeGauge_Window->GetShowStatus())
							{
								pChargeGauge_Window->ShowWindow(TRUE);
								pChargeGauge_Window->SetGaugeMode(_XGAUGE_CHARGE);
							}
	
							FLOAT channelpercent = ((g_LocalSystemTime - m_ChannelingReceivePacketTime) / 4000.) * 100.0f;
							pChargeGauge_Window->SetParameter(channelpercent);
	
							if(channelpercent >= 100.0f)
							{
								m_ChannelingReceivePacketTime = g_LocalSystemTime;
								pChargeGauge_Window->ShowWindow(FALSE);
							}
						}
					}
					break;
				default :
					{
						if(GetMotionClass() != _XACTION_DYING)
						{
							_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
							if(pChargeGauge_Window)
							{
								if(pChargeGauge_Window->GetShowStatus())
									pChargeGauge_Window->ShowWindow(FALSE);
							}
						}
					}
					break;
				}
			}*/

		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		break;
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
		{
			if(_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF)
			{
				switch(m_CurrentBattleState)
				{
				case _XUSER_STATE_4 :
					DrawBattleRoundingSpell();
					break;
				default :
					{
						if(GetMotionClass() != _XACTION_DYING)
						{
							_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
							if(pChargeGauge_Window)
							{
								if(pChargeGauge_Window->GetShowStatus())
								{
									if( pChargeGauge_Window->GetCurrentGaugeMode() == _XGAUGE_CHARGE )
										pChargeGauge_Window->ShowWindow(FALSE);
								}
							}
						}
					}
					break;
				}
			}// target type - self
			else
			{
				switch(m_CurrentBattleState)
				{
				case _XUSER_STATE_3 :
				case _XUSER_STATE_4 :
					DrawBattleRoundingSpell();
					break;
				default :
					{
						if(GetMotionClass() != _XACTION_DYING)
						{
							_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
							if(pChargeGauge_Window)
							{
								if(pChargeGauge_Window->GetShowStatus())
								{
									if( pChargeGauge_Window->GetCurrentGaugeMode() == _XGAUGE_CHARGE )
										pChargeGauge_Window->ShowWindow(FALSE);
								}
							}
						}
					}
					break;
				}
			} // target type - other/all
		} // rounding spell
		break;
	}
}

void _XLocalUser::DrawBattleRoundingSpell(void)
{
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
	if(pChargeGauge_Window)
	{
		if(m_RoundingDelayStartTime > 0)
		{
			if(!pChargeGauge_Window->GetShowStatus())
			{
				pChargeGauge_Window->SetGaugeMode(_XGAUGE_CHARGE);
				pChargeGauge_Window->ShowWindow(TRUE);
			}
			
			FLOAT delaypercentage = 0.0f;
			int casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].casting;
			if(casting > 0)
				delaypercentage = ((g_LocalSystemTime - m_RoundingDelayStartTime) / (FLOAT)casting) * 100;
			else
				delaypercentage = 100;
			
			pChargeGauge_Window->SetParameter(delaypercentage);
		}
	}
}

void _XLocalUser::DrawBattleCastingSpell(void)
{
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
	if(pChargeGauge_Window)
	{
		if(m_CastingSpellRepeatTime > 0)
		{
			if(!pChargeGauge_Window->GetShowStatus())
			{
				pChargeGauge_Window->SetGaugeMode(_XGAUGE_CHARGE);
				pChargeGauge_Window->ShowWindow(TRUE);
			}

			FLOAT delaypercentage = 0.0f;
			int casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].casting;
			if(casting > 0)
				delaypercentage = ((g_LocalSystemTime - m_CastingSpellRepeatTime) / (FLOAT)casting) * 100;
			else 
				delaypercentage = 100;

			pChargeGauge_Window->SetParameter(delaypercentage);
		}
	}
}

void _XLocalUser::ProcessCommonAction(void)
{
	short stancemotion = GetStanceMotion(m_ActiveSlotID);	

	if(GetMotionClass() == _XACTION_IDLE)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if((m_ModelDescriptor.GetCurMotion() == stancemotion) && (stancemotion != 0))
			{
				SetAnimation(_XACTION_IDLE);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}

		ResetActiveSkillAnimationControl();
	}
	else if(GetMotionClass() == _XACTION_DEFENCE)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
		{
			if(g_LodTerrain.m_3PCamera.m_ImpactValue == 0.0f)
				g_LodTerrain.m_3PCamera.m_ImpactValue = _XDEF_IMPACTPOWER;
			
			int rand = _XRandom() % 3;
			_XPlaySoundEffect(g_DefenceEffectSoundIndex[m_CharacterInfo.gender][rand], m_Position);
		}
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			SetAnimation(_XACTION_IDLE);
//			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}

		ResetActiveSkillAnimationControl();
	}
	else if(GetMotionClass() == _XACTION_BLOW)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
		{
			if(g_LodTerrain.m_3PCamera.m_ImpactValue == 0.0f)
				g_LodTerrain.m_3PCamera.m_ImpactValue = _XDEF_IMPACTPOWER * 1.4f;
			
			int rand = _XRandom() % 3;
			_XPlaySoundEffect(g_BlowEffectSoundIndex[m_CharacterInfo.gender][rand], m_Position);
		}
		else if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			m_SpecialActionTimer = g_LocalSystemTime;

			if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL || m_SelectedSkillID == 0)
			{
				if(m_AttackTarget && m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
//					_XDWINPRINT("[ProcessCommonAction] Set Attack");
					m_CurrentBattleState = _XUSER_STATE_1;
//					SetAnimation(_XACTION_ATTACK);
				}
				else
				{
					m_CurrentBattleState = _XUSER_STATE_LAST;
//					SetAnimation(_XACTION_IDLE);
//					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				}
			}
			else
			{
				SetAnimation(_XACTION_IDLE);
//				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}

		ResetActiveSkillAnimationControl();
	}
	else if(GetMotionClass() == _XACTION_DYING)
	{
		/*
		if(m_ModelDescriptor.GetCurMotion() == _XAN_DYINGREADY_NONE			||
			m_ModelDescriptor.GetCurMotion() == _XAN_DYINGREADY_SHORTWEAPON	||
			m_ModelDescriptor.GetCurMotion() == _XAN_DYINGREADY_LONGWEAPON		)
		{
			
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
			{
				// �ڷ� �о��ֱ� ����
				if( m_AttackTarget )
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					D3DXVECTOR3 viewdirection;
					viewdirection.x = pMob->m_Position.x - m_Position.x;
					viewdirection.y = 1.0f;
					viewdirection.z = pMob->m_Position.z - m_Position.z;
					
					m_TargetPosition.x = m_Position.x + viewdirection.x*(-0.5f);
					m_TargetPosition.z = m_Position.z + viewdirection.z*(-0.5f);
					
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position);
					m_LastRotateAngle = m_RotateAngle;
					UpdatePosition();
					
					ResetViewTarget();
					ResetAttackTarget();
				}
			}
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{
				// �����
				m_TargetPosition = m_Position;
				UpdatePosition();
			}			
		}
		*/

		ResetActiveSkillAnimationControl();
	}
	else if(GetMotionClass() == _XACTION_DIE)
	{
		ResetActiveSkillAnimationControl();
	}
	else if(GetMotionClass() == _XACTION_REBIRTH)
	{
#ifdef _XDEF_USESCREEENMOTIONBLUR 
		if(g_ScrrenMotionBlurMode)
		{
			g_ScreenMotionBlurEffectManager.AddBlurAlpha(-0.002f);
			
			if(g_ScreenMotionBlurEffectManager.GetBlurAlpha() <= g_ScreenMotionBlurEffectManager.GetMinBlurAlpha())
				g_ScrrenMotionBlurMode = FALSE;
		}
#endif

		ResetActiveSkillAnimationControl();
	}
}

void _XLocalUser::ProcessNormalAction(void)
{
	if(m_CharacterInfo.animationdescript.motionclass == _XACTION_ATTACK)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if(!m_AttackTarget)
			{
				SetAnimation(_XACTION_IDLE);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				m_AttackTargetObjectType = _XPICKINGTARGET_NONE;
			}
		}
	}
}

// CheckDistanceToTarget() : target�� ���ݰŸ��� ���������� TRUE, �ƴϸ� FALSE
BOOL _XLocalUser::CheckDistanceToTarget(D3DXVECTOR3 targetposition, short skillid, FLOAT& range, unsigned short mobType, bool bAttackAfterMove)
{
	FLOAT dx = m_Position.x - targetposition.x;
	FLOAT dz = m_Position.z - targetposition.z;
	FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

	FLOAT targetdistance;
	FLOAT smallrange = 0.0f;

	if(mobType == 0)
	{
		// PC
		targetdistance = 0.7f;
		
		if(skillid == 0)
			range = m_AttackRange + targetdistance;
		else
			range = m_AttackRange + _XSkillItem::GetDistance(skillid); //g_SkillProperty[m_SelectedSkillID]->distance;
		
		if(bAttackAfterMove)
		{
			smallrange = range;
		}
		else
		{
			smallrange = range * 1.2f;
		}
	} // pc
	else
	{
		if( g_MobAttribTable[mobType].scripttype > 80 && g_MobAttribTable[mobType].scripttype < 85)// Key LPS �� LPS
		{
			if( g_MobAttribTable[mobType].scripttype != 84 )
			{
				range = smallrange = 0.7f;
			}
			else // HP LPS�϶��� ���� ���ݰŸ��� �����Ѵ�.
			{
				targetdistance = g_MobAttribTable[mobType].collideradius;

				if(skillid == 0)
					range = m_AttackRange + targetdistance;
				else
					range = m_AttackRange + _XSkillItem::GetDistance(skillid);//g_SkillProperty[m_SelectedSkillID]->distance;
				
				if(bAttackAfterMove)
				{
					smallrange = range;
				}
				else
				{
					smallrange = range * 1.2f;
				}
			}
		}
		else if( g_MobAttribTable[mobType].scripttype == 85 || // optional lps (trap)
				 g_MobAttribTable[mobType].scripttype == 90 )// ���ݰ����� ������Ʈ 
		{
			range = smallrange = 2.5f;
		}
		else //�Ϲݸ���
		{
			if(mobType > 0)
				targetdistance = g_MobAttribTable[mobType].collideradius;
			else
				targetdistance = 0.7f;		// PC

			if(skillid == 0)
				range = m_AttackRange + targetdistance;
			else
				range = m_AttackRange + _XSkillItem::GetDistance(skillid); //g_SkillProperty[m_SelectedSkillID]->distance;

			if(bAttackAfterMove)
			{
				smallrange = range;
			}
			else
			{
				smallrange = range * 1.2f;
			}
		}
	} // monster

	if(distance <= smallrange)
	{
//		_XDWINPRINT("[CheckDistanceToTarget] return TRUE - distance : %f  range :%f  smallrange:%f", distance , range, smallrange);
		m_TargetPosition = m_Position;
		return TRUE;
	}
	else
	{
//		_XDWINPRINT("[CheckDistanceToTarget] return FALSE - distance : %f  range :%f  smallrange:%f", distance , range, smallrange);
		return FALSE;
	}
}

// SetTargetPosition() : m_TargetPosition ����, �̵��� ������ �������ִ� �Լ�
void _XLocalUser::SetTargetPosition(D3DXVECTOR3 targetposition, FLOAT range)
{
	D3DXVECTOR3 directionvector = m_Position - targetposition;
	D3DXVec3Normalize(&directionvector, &directionvector);
	
	D3DXVECTOR3 position = targetposition + (directionvector*(range*0.7f));
	position.y = targetposition.y;
	SetTargetPositionAngle(position);
}

void _XLocalUser::CheckDelayTime(void)
{
	if(m_bCheckDelayTime)
	{
		if(m_ChangeDelayStartTime == 0)
			m_bCheckDelayTime = FALSE;

		int delaytime = 0;
//Author : �����
#ifdef _XDEF_SKILLDEALY_MODIFY
		if(m_NextSkillID > 0)
		{
			// ����� skill ����
			if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
			{
				if(g_SkillProperty[m_NextSkillID]->usefulType != _XSI_USEFULTYPE_ACTIVESKILL)
				{
#ifdef _XDEF_BLOCK_MOVINGATTACK
					// ���� (Other - All)
					delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;

					if(m_Lock.adddelaytime == 1)
					{
						// ����ȿ�� - dealy time ����
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Other[%d] - All[����] delay[%d]", m_FinallySelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
#else
					// delay time �������� ���� (Active - Other)
					m_ChangeDelayStartTime = 0;
					m_bCheckDelayTime = FALSE;
					m_bAddDelayTime = FALSE;

					_XDWINPRINT("[CheckDelayTime] Active[%d] - Other[%d] ", m_SelectedSkillID, m_NextSkillID);
#endif
				}
				else
				{
					// ���� (Active - Active)
					if(m_SelectedSkillID > 0)
						delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;
					else
						delaytime = 500;

					if(m_Lock.adddelaytime == 1)
					{
						// ����ȿ�� - dealy time ����
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Active[%d] - Active[%d] delay[%d]", m_SelectedSkillID, m_NextSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
				}
			}
			else
			{
				// ���� (Other - All)
				if(m_SelectedSkillID > 0)
					delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;
				else
					delaytime = 500;

				if(m_Lock.adddelaytime == 1)
				{
					// ����ȿ�� - dealy time ����
					delaytime *= 2;
				}
				else if(m_bAddDelayTime)
				{
					delaytime += 1000;
				}
				
				if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
				{
					if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
					{
						_XDWINPRINT("[CheckDelayTime] Active[%d] - Active[%d] delay[%d]", m_SelectedSkillID, m_NextSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
						m_ChangeDelayStartTime = 0;
						m_bCheckDelayTime = FALSE;
						m_bAddDelayTime = FALSE;
					}
				}
			}
		} // if(m_NextSkillID > 0)
		else
		{
			// ����� ��ų ���� (Active - Active ���� ����)
			if(m_FinallySelectedSkillID > 0)
			{
				if(g_SkillProperty[m_FinallySelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
				{
					// ���� (Active - Active)
					delaytime = _XSkillItem::GetDelay(m_FinallySelectedSkillID, m_FinallySelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;

					if(m_Lock.adddelaytime == 1)
					{
						// ����ȿ�� - dealy time ����
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Active[%d] - Active[����] delay[%d]", m_FinallySelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_FinallySelectedSkillID = 0;
							m_FinallySelectedSkillItem = NULL;
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
				}
				else
				{
					// ���� (Other - All)
					delaytime = _XSkillItem::GetDelay(m_FinallySelectedSkillID, m_FinallySelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;

					if(m_Lock.adddelaytime == 1)
					{
						// ����ȿ�� - dealy time ����
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Other[%d] - All[����] delay[%d]", m_FinallySelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_FinallySelectedSkillID = 0;
							m_FinallySelectedSkillItem = NULL;
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
				}
			}
			else
			{
				// ���� (Active - Active)
				delaytime = 500;

				if(m_bAddDelayTime)
				{
					delaytime += 1000;
				}
				
				if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
				{
					if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
					{
						_XDWINPRINT("[CheckDelayTime] Axtive[%d] - Active[����] delay[%d]", m_SelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
						m_ChangeDelayStartTime = 0;
						m_bCheckDelayTime = FALSE;
						m_bAddDelayTime = FALSE;
					}
				}
			}
		}
#else
//Last Updated : 07/03/23

		if(m_NextSkillID > 0)
		{
			// ����� skill ����
			if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
			{
				if(g_SkillProperty[m_NextSkillID]->usefulType != _XSI_USEFULTYPE_ACTIVESKILL)
				{
#ifdef _XDEF_BLOCK_MOVINGATTACK //Author : �����
					// ���� (Other - All)
					delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;

					if(m_Lock.adddelaytime == 1)
					{
						// ����ȿ�� - dealy time ����
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Other[%d] - All[����] delay[%d]", m_FinallySelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
#else //Last Updated : 07/04/19
					// delay time �������� ���� (Active - Other)
					m_ChangeDelayStartTime = 0;
					m_bCheckDelayTime = FALSE;
					m_bAddDelayTime = FALSE;

					_XDWINPRINT("[CheckDelayTime] Active[%d] - Other[%d] ", m_SelectedSkillID, m_NextSkillID);
#endif
				}
				else
				{
					// ���� (Active - Active)
					if(m_SelectedSkillID > 0)
						delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;
					else
						delaytime = 500;

					if(m_Lock.adddelaytime == 1)
					{
						// ����ȿ�� - dealy time ����
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Active[%d] - Active[%d] delay[%d]", m_SelectedSkillID, m_NextSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
				}
			}
			else
			{
				// ���� (Other - All)
				if(m_SelectedSkillID > 0)
					delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;
				else
					delaytime = 500;

				if(m_Lock.adddelaytime == 1)
				{
					// ����ȿ�� - dealy time ����
					delaytime *= 2;
				}
				else if(m_bAddDelayTime)
				{
					delaytime += 1000;
				}
				
				if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
				{
					if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
					{
						_XDWINPRINT("[CheckDelayTime] Active[%d] - Active[%d] delay[%d]", m_SelectedSkillID, m_NextSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
						m_ChangeDelayStartTime = 0;
						m_bCheckDelayTime = FALSE;
						m_bAddDelayTime = FALSE;
					}
				}
			}
		} // if(m_NextSkillID > 0)
		else
		{
			// ����� ��ų ���� (Active - Active ���� ����)
			if(m_SelectedSkillID > 0)
			{
				if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
				{
					// ���� (Active - Active)
					delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;

					if(m_Lock.adddelaytime == 1)
					{
						// ����ȿ�� - dealy time ����
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Active[%d] - Active[����] delay[%d]", m_SelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
				}
				else
				{
					// ���� (Other - All)
					delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;

					if(m_Lock.adddelaytime == 1)
					{
						// ����ȿ�� - dealy time ����
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Other[%d] - All[����] delay[%d]", m_SelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
				}
			}
			else
			{
				// ���� (Active - Active)
				delaytime = 500;

				if(m_bAddDelayTime)
				{
					delaytime += 1000;
				}
				
				if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
				{
					if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
					{
						_XDWINPRINT("[CheckDelayTime] Axtive[%d] - Active[����] delay[%d]", m_SelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
						m_ChangeDelayStartTime = 0;
						m_bCheckDelayTime = FALSE;
						m_bAddDelayTime = FALSE;
					}
				}
			}
		}
#endif
		
	} // if(m_bCheckDelayTime)
}

void _XLocalUser::ApplyNextSkill(void)
{
	if(!m_bUsingSkill && !m_bCheckDelayTime)
	{
		if(m_NextSkillID > 0)
		{
			XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

			switch(g_SkillProperty[m_NextSkillID]->usefulType)
			{
			case _XSI_USEFULTYPE_ACTIVESKILL :
				{
					pProc_Maingame->ChangeToActive(m_NextSkillID);
				}
				break;
			case _XSI_USEFULTYPE_CHANNELINGSPELL :
				{
					pProc_Maingame->ChangeToChanneling(m_NextSkillID, TRUE);
				}
				break;
			case _XSI_USEFULTYPE_CHARGESKILL :
				{
					pProc_Maingame->ChangeToOther(m_NextSkillID, TRUE);
					m_bStartCharge = TRUE;
				}
				break;
			case _XSI_USEFULTYPE_CASTINGSPELL :
			case _XSI_USEFULTYPE_FINISHSKILL :
			case _XSI_USEFULTYPE_ROUNDINGSPELL :
				{	
					pProc_Maingame->ChangeToOther(m_NextSkillID, TRUE);
				}
				break;
			case _XSI_USEFULTYPE_SMASHINGSKILL :
				{
					pProc_Maingame->ChangeToOther(m_NextSkillID, TRUE);
					m_bRestartActive = FALSE;
				}
				break;
			case _XSI_USEFULTYPE_ACTIONSKILL :
				{
					pProc_Maingame->ChangeToOther(m_NextSkillID, TRUE);
				}
				break;
			case _XSI_USEFULTYPE_HIDDENMA :
				{
					pProc_Maingame->ChangeToHiddenMA(m_NextSkillID, TRUE);
				}
				break;
			}

			m_NextSkillID = 0;
		}
		else
		{
			if(m_bRestartActive)
			{
				if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL || m_SelectedSkillID == 0)
				{
					m_CurrentBattleState = _XUSER_STATE_1;
					m_bRestartActive = FALSE;
				}
				else if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_SMASHINGSKILL)
				{
					XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
					pProc_Maingame->ChangeToActive(m_ActiveSlotID);
					m_bRestartActive = FALSE;
				}
			}
		}
	}
}

/*void _XLocalUser::SetActiveSlotSkill(void)
{
	int skillid = m_ActiveSlotID;
	if(((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.CheckDelayTime(skillid))
	{
		m_CurrentSelectedSkillType = g_SkillProperty[skillid]->usefulType;
		m_SelectedSkillID = skillid;
		m_SelectedSkillItem = GetSkillItem(skillid);
		
//		SetSkillAnimation();
		UpdateLocalUserAttackLevel();
		g_NetworkKernel.SendPacket(MSG_NO_SKILL_SELECT);
	}
}*/

void _XLocalUser::SetViewTarget(_XItem* target, _XPICKINGTARGETTYPE targettype, BOOL bsetpkready)
{
	if( m_ViewTarget )
	{
		switch(m_ViewTargetType)
		{
		case _XPICKINGTARGET_MOB :
			{
				if(((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_NPC :
			{
				if(((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_PC :
			{
				if(((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_SELF :
			{
				if(m_ModelDescriptor.m_EffectManager)
					m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_DUMMY :
			{
				if(((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		}
	}
	
	m_ViewTarget = target;
	m_ViewTargetType = targettype;
	// �̵������ð� �ʱ�ȭ
	m_PrevMoveTime			 = 0;

	// pk ready ���� �ʱ�ȭ
	if(!bsetpkready)
	{
		if(m_UserState == _XDEF_USERSTATE_PK_READY)
		{
			ResetPKReadyMode();
//			m_UserState = _XDEF_USERSTATE_NORMAL;

			if(targettype == _XPICKINGTARGET_PC)
			{
				_XUser* pUser = (_XUser*)target;
				if(pUser)
				{
					pUser->m_bEnemy = FALSE;
				}
			}
		}
	}

	// Functional Object�� Ŭ�������� ���ݰ��������� �Ǻ��Ѵ�.
	if( m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT )
	{
		if( m_ViewTarget )
		{							
			_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_ViewTarget;
			if( pFunctionalObject )
			{
				pFunctionalObject->SetAttackableFunctionalObject();
			}
		}
	}
	else	// Functional Object�� ����Ʈ�� �߰����� �ʴ´�.
	{
		BOOL bAddEffect = FALSE;
		if(m_ViewTarget)
		{
			if(m_AttackTarget)
			{
				if(m_ViewTarget != m_AttackTarget)
					bAddEffect = TRUE;
			}
			else
			{
				bAddEffect = TRUE;
			}
		}
		
		if(bAddEffect)
		{
			switch(targettype)
			{
			case _XPICKINGTARGET_PC :
				{
					if(((_XUser*)target)->m_ModelDescriptor.m_EffectManager)
						((_XUser*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&((_XUser*)target)->m_ModelDescriptor.m_Position, NULL, TRUE );
				}
				break;
			case _XPICKINGTARGET_MOB :
				{
					if(((_XMob*)target)->m_ModelDescriptor.m_EffectManager)
						((_XMob*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&((_XMob*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);
					
					_XPlayInterfaceSound(ID_SR_INTERFACE_TARGETING00_WAV);
					g_NetworkKernel.SendPacket(MSG_NO_REQ_SPELLEFFECT, ((_XMob*)target)->m_MobID);
					
					SetMonsterWarningMessage( ((_XMob*)target)->m_MobType );
				}
				break;
			case _XPICKINGTARGET_NPC :
				{
					if(((_XNPCObject*)target)->m_ModelDescriptor.m_EffectManager)
						((_XNPCObject*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&((_XNPCObject*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);
				}
				break;
			case _XPICKINGTARGET_DUMMY :
				{
					if(((_XDummy*)target)->m_ModelDescriptor.m_EffectManager)
					{
						((_XDummy*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
							&((_XDummy*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);
					}
				}
				break;
			case _XPICKINGTARGET_SELF :
				{
					if(m_ModelDescriptor.m_EffectManager)
						m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&m_ModelDescriptor.m_Position, NULL, TRUE);
				}
				break;
			}
		}
	}

	
	_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
	if(pMonsterStatus_Window)
	{
		pMonsterStatus_Window->ShowWindow( ( target ) ? TRUE : FALSE );
	}	
}

void _XLocalUser::ResetViewTarget(void)
{
	if(m_ViewTarget)
	{
		switch(m_ViewTargetType)
		{
		case _XPICKINGTARGET_PC :
			{
				if(((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_MOB :
			{
				if(((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_NPC :
			{
				if(((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_DUMMY :
			{
				if(((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
				{
					((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
				}
			}
			break;
		case _XPICKINGTARGET_SELF :
			{
				if(m_ModelDescriptor.m_EffectManager)
					m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_FUNCTIONALOBJECT :
			{
				_XWindow_MatchNoticeLarge* pMatchNoticeLarge_Window = (_XWindow_MatchNoticeLarge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICELARGE);
				if(pMatchNoticeLarge_Window)
				{
					if(pMatchNoticeLarge_Window->GetShowStatus())
						pMatchNoticeLarge_Window->ShowWindow(FALSE);
				}
			}
			break;
		}

		// pk ready ���� �ʱ�ȭ
		if(m_UserState == _XDEF_USERSTATE_PK_READY)
		{
			ResetPKReadyMode();
//			m_UserState = _XDEF_USERSTATE_NORMAL;
			
			if(m_ViewTargetType == _XPICKINGTARGET_PC)
			{
				_XUser* pUser = (_XUser*)m_ViewTarget;
				if(pUser)
				{
					pUser->m_bEnemy = FALSE;
				}
			}
		}

		m_ViewTarget = NULL;
		m_ViewTargetType = _XPICKINGTARGET_NONE;
		
		_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
		if(pMonsterStatus_Window)
		{
			if(!m_AttackTarget)
			{
				if(pMonsterStatus_Window->GetShowStatus())
					pMonsterStatus_Window->ShowWindow( FALSE );
			}
		}
	}
}

void _XLocalUser::SetSpellAttackTarget(_XItem* target, _XPICKINGTARGETTYPE targettype)
{
	m_SpellAttackTarget = target;
	m_SpellAttackTargetType = targettype;
}

void _XLocalUser::ResetSpellAttackTarget(void)
{
	m_SpellAttackTarget = NULL;
	m_SpellAttackTargetType = _XPICKINGTARGET_NONE;
}

void _XLocalUser::GetItemTileIndex(FLOAT x, FLOAT z, _XTILE_ITEM& itemtile)
{
	int ix = (int)x + (gnWidth / 2);
	int iz = (int)z + (gnWidth / 2);
	
	if(ix > gnWidth)
		ix = gnWidth - 1;
	else if(ix < 0)
		ix = 1;
	
	if(iz > gnWidth)
		iz = gnWidth - 1;
	else if(iz < 0)
		iz = 1;

	int NumWidthTile = gnWidth / 16;
	int NumHeightTile = gnHeight / 16;

	if(ix < 0 || iz < 0 || (ix/16) >= (NumWidthTile*2) || (iz/16) >= (NumHeightTile*2))
		return;
	
	itemtile.sX = (short)(ix / 16);
	itemtile.sZ = (short)(iz / 16);
}

void _XLocalUser::SetTargetPositionAngle(D3DXVECTOR3 targetposition)
{
	if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE && GetMotionClass() != _XACTION_REBIRTH )
	{
		m_TargetPosition = m_FinalTargetPosition = targetposition;

		FLOAT dx = m_Position.x - m_FinalTargetPosition.x;
		FLOAT dz = m_Position.z - m_FinalTargetPosition.z;
		m_LeftFinalTargetLength = _XFC_sqrt(dx*dx + dz*dz);

		//m_PrevMoveTime = 0;

		//FLOAT dx = m_Position.x - m_TargetPosition.x;
		//FLOAT dz = m_Position.z - m_TargetPosition.z;		
		//if( _XFC_sqrt(dx*dx + dz*dz) > 4.0f ) // 8.0 �̻� ��ŷ�Ȱ�츸 ��ã��
		{
			// ����� �ƴҶ��� �н� ���ε�
			if( !( g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL)||
				!( _XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF		   )||
				!( g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03			   )||
				!gpInput->CheckKeyPress(DIK_LMENU) )
			{
				CreateMovePath();
				
				if(m_PathNodeCount > 0)
				{	
					if( m_PathNodeCount == 1 )
					{
						m_TargetPosition.x = m_PathPointArray[0].pos.x;
						m_TargetPosition.z = m_PathPointArray[0].pos.y;
					}
					else
					{
						m_TargetPosition.x = ( m_PathPointArray[0].pos.x + g_PathFinder.m_CollideArrayStartPosition.x) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
						m_TargetPosition.z = ( m_PathPointArray[0].pos.y + g_PathFinder.m_CollideArrayStartPosition.y) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;				
						
						FLOAT dx = m_Position.x - m_TargetPosition.x;
						FLOAT dz = m_Position.z - m_TargetPosition.z;
						if( _XFC_sqrt(dx*dx + dz*dz) < 3.0f ) // ���� ����Ʈ�� ��ó Ÿ���� ��ġ��� ��ŵ
						{
							m_TargetPosition.x = ( m_PathPointArray[1].pos.x + g_PathFinder.m_CollideArrayStartPosition.x) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
							m_TargetPosition.z = ( m_PathPointArray[1].pos.y + g_PathFinder.m_CollideArrayStartPosition.y) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
							m_CurrentNavigatePathNodeIndex = 1;
						}
					}
				}
				else
				{
					if(m_PathNodeCount <= 0) // �н� ��尡 0�̸� ���� ���ϸ� �н����ε� ����
					{
						m_PathNodeCount = 0; // Path find ��� ����.
						m_CurrentNavigatePathNodeIndex = 0;
					}
				}
			}
		}		
		/*else
		{
			m_PathNodeCount = 0; // Path find ��� ����.
			m_CurrentNavigatePathNodeIndex = 0;
		}*/

		m_LastRotateAngle = _XMeshMath_GetRotateRadian(m_Position, m_TargetPosition);
		
		FLOAT elapsedangle_1 = fabs(m_LastRotateAngle - m_RotateAngle);
		FLOAT elapsedangle_2 = (_X_PI - fabs(m_LastRotateAngle)) + (_X_PI - fabs(m_RotateAngle));
		
		if(elapsedangle_1 <= elapsedangle_2)
		{
			if(m_LastRotateAngle >= m_RotateAngle)
				m_RotateDirection = _XROTATE_CLOCKWISE;
			else
				m_RotateDirection = _XROTATE_COUNTCLOCKWISE;
		}
		else
		{
			if(m_LastRotateAngle >= m_RotateAngle)
				m_RotateDirection = _XROTATE_COUNTCLOCKWISE;
			else
				m_RotateDirection = _XROTATE_CLOCKWISE;
		}
	}
}

void _XLocalUser::ProcessRescueUser(void)
{
	g_NetworkKernel.SendPacket(MSG_NO_CHARAC_REBIRTH_TARGET, 0);
	SetAnimation(_XACTION_RESCUE);
	ResetAttackTarget();
}

void _XLocalUser::CreateMovePath( void )
{
	int	xcoord = _XFC_FloatToIntRet( ( m_Position.x + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
	int zcoord = _XFC_FloatToIntRet( ( m_Position.z + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );		
	if( xcoord < 0 ) xcoord = 0;
	if( zcoord < 0 ) zcoord = 0;
	if( xcoord >= (g_LodTerrain.m_TileCount<<4) ) xcoord = (g_LodTerrain.m_TileCount<<4) - 1;
	if( zcoord >= (g_LodTerrain.m_TileCount<<4) ) zcoord = (g_LodTerrain.m_TileCount<<4) - 1;	
	
	int startxpoint = xcoord;
	int startzpoint = zcoord;
		
	xcoord = _XFC_FloatToIntRet( ( m_FinalTargetPosition.x + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
	zcoord = _XFC_FloatToIntRet( ( m_FinalTargetPosition.z + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );	
	if( xcoord < 0 ) xcoord = 0;
	if( zcoord < 0 ) zcoord = 0;
	if( xcoord >= (g_LodTerrain.m_TileCount<<4) ) xcoord = (g_LodTerrain.m_TileCount<<4) - 1;
	if( zcoord >= (g_LodTerrain.m_TileCount<<4) ) zcoord = (g_LodTerrain.m_TileCount<<4) - 1;	
	
	int endxpoint = xcoord;
	int endzpoint = zcoord;
	
	if( !g_LodTerrain.GetPathFindAreaInfo( startzpoint, startxpoint ) && 
		!g_LodTerrain.GetPathFindAreaInfo( endzpoint, endxpoint ) ) // Ÿ���õ� ������ ���� �ִ� �����϶��� �˻�.
	{	
		if( !g_LodTerrain.CheckDirectPath( startxpoint, startzpoint, xcoord, zcoord ) ) // ������ �н��� �����ϸ� ��ã�� ����.
		{
			m_PathNodeCount = g_PathFinder.FindPath( D3DXVECTOR2(startxpoint,startzpoint), D3DXVECTOR2(endxpoint,endzpoint), -1.0f, m_PathPointArray );
			
			if( m_PathNodeCount > 0 )
			{
				m_PathPointArray[m_PathNodeCount-1].pos.x = m_FinalTargetPosition.x;
				m_PathPointArray[m_PathNodeCount-1].pos.y = m_FinalTargetPosition.z;
				g_LodTerrain.m_3PCamera.m_AutoBackTrace = TRUE;
			}		
			else
			{
				m_PathNodeCount = 0;
			}
		}
		else
		{
			m_PathNodeCount = 0;
		}
	}
	else
	{
		m_PathNodeCount = 0;
	}

	m_CurrentNavigatePathNodeIndex = 0;
}


#ifdef _XDWDEBUG
void _XLocalUser::DrawMovePath( void )
{
	if( m_PathNodeCount <= 0 ) return;
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	gpDev->SetTexture(0, NULL); 
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX );
	gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
	
	AXIS_VERTEX	line[2];
	line[0].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
	line[1].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
	D3DXVECTOR3 curpos = m_Position;
	curpos.y += 0.3f;
	
	for( int i = 0; i < m_PathNodeCount -1; ++i )
	{						
		line[0].v = curpos;
		
		line[1].v.x = ( m_PathPointArray[i].pos.x + g_PathFinder.m_CollideArrayStartPosition.x) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
		line[1].v.z = ( m_PathPointArray[i].pos.y + g_PathFinder.m_CollideArrayStartPosition.y) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
		line[1].v.y = g_LodTerrain.GetTerrainHeight( line[1].v ) + 0.3f;
		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
		
		curpos = line[1].v;
	}
	
	
	line[0].v = curpos;
	line[1].v = m_FinalTargetPosition;
	line[1].v.y = g_LodTerrain.GetTerrainHeight( line[1].v ) + 0.3f;
	
	gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));	
}
#endif

void _XLocalUser::ProcessMobDie(void)
{
	_XMob* pAttackTarget = NULL;
	_XMob* pMob = NULL;

	for(int i = 0 ; i < 3 ; ++i)
	{
		if(m_AttackResult.display[i])
		{
			if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
			{
				if(m_AttackTarget && m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					pAttackTarget = reinterpret_cast<_XMob*>(m_AttackTarget);
				}
				if(pAttackTarget)
				{
					if(pAttackTarget->m_MobID == m_AttackResult.usDefenderID)
						pMob = pAttackTarget;
					else
						pMob = (_XMob*)g_Moblist.FindMob(m_AttackResult.usDefenderID);
				}
				else
				{
					pMob = (_XMob*)g_Moblist.FindMob(m_AttackResult.usDefenderID);
				}

				if(pMob)
				{
					if( g_ZeroLevelMode )
						pMob->SetCharacterMobAnimation(_XMOBACTION_DIE);
					else
						pMob->SetAnimation(_XMOBACTION_DIE);
					pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
					
					if(m_AttackResult.sDamage[i] == -1)
					{
//						m_CharacterLog.isKillClient = TRUE;
//						m_CharacterLog.killTimeClient = g_ServerTimeCode;
//						m_CharacterLog.killTargetTypeClient = pMob->m_MobType;


						_XDWINPRINT("[LOCAL USER/ProcessMobDie] Mob[%d] kill", pMob->m_MobType);
					}
					m_AttackResult.cBattleResult = 0;
					m_AttackResult.display[i] = FALSE;
					break;
				}

				if(i == 2)
				{
					// ������ Ÿ�� �׾���� ���͸� ��ã����
					if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						m_AttackResult.cBattleResult = 0;
				}
			}
			m_AttackResult.display[i] = FALSE;
		}
	}
}

/*void _XLocalUser::SetActiveSkill(void)
{
	BOOL	bIdle = FALSE;

	m_CurrentBattleState = _XUSER_STATE_NONE;
	m_CurrentSelectedSkillType = _XSI_USEFULTYPE_ACTIVESKILL;

	short prevskillid = m_SelectedSkillID;
	m_SelectedSkillID = m_NextSkillID;
	short prevactiveslot = m_ActiveSlotID;
	m_ActiveSlotID = m_NextSkillID;

	if(m_SelectedSkillID == 0)
		m_SelectedSkillItem = NULL;
	else
		m_SelectedSkillItem = GetSkillItem(m_SelectedSkillID);

	if(m_ActiveSlotID != prevactiveslot)
	{
		if(m_CharacterInfo.animationdescript.motionclass == _XACTION_IDLE)
		{
			SetBattleMode(_XBATTLE_BATTLE);
			if(g_pQuickSlot_Window)
				g_pQuickSlot_Window->ChangePeaceBattleMode();

			bIdle = TRUE;
		}
	}

	if(m_CharacterInfo.animationdescript.motionclass == _XACTION_ATTACK)
		SetSkillAnimation();

	UpdateLocalUserAttackLevel();

	if(prevskillid != m_SelectedSkillID)
		g_NetworkKernel.SendPacket(MSG_NO_SKILL_SELECT);

	if((m_NextSkillID!= prevskillid && g_SkillProperty[prevskillid]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL) || prevskillid == 0)
	{
		ResetAttackTarget();
		if(!bIdle)
			SetAnimation(_XACTION_IDLE);
	}

	VIEWBATTLELOG
		_XDWINPRINT("Select Active skill [%d]", m_SelectedSkillID);
}*/

void _XLocalUser::ResetActiveSkillAnimationControl(void)
{
	if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_ACTIVESKILL || m_SelectedSkillID == 0)
	{
		m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
		
		// �ִϸ��̼� �ڸ� ��
		if(m_CharacterInfo.gender == _XGENDER_MAN)
			m_CharacterInfo.animationdescript.attacktype = g_SkillProperty[m_SelectedSkillID]->motion_male;
		else
			m_CharacterInfo.animationdescript.attacktype = g_SkillProperty[m_SelectedSkillID]->motion_female;
	}
	m_bCheckFirstFrame = FALSE;
}

BOOL _XLocalUser::IsTroubleStateNormal(void)
{
	DWORD result;
	result = m_UserTroubleState ^ _XDEF_USER_TROUBLESTATE_NORMAL;

	if(result == 0)
		return TRUE;
	else
		return FALSE;
}

BOOL _XLocalUser::IsTroubleStateFury(void)
{
	DWORD result;
	result = m_UserTroubleState || _XDEF_USER_TROUBLESTATE_FURY;

	if(result == 0)
		return TRUE;
	else 
		return FALSE;
}

BOOL _XLocalUser::IsTroubleStateConvergence(void)
{
	return FALSE;
}

BOOL _XLocalUser::CanNotMove(void)
{
	switch(m_CurrentSelectedSkillType)
	{
	case _XSI_USEFULTYPE_CHARGESKILL :
		{
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
			case _XUSER_STATE_6 :
			case _XUSER_STATE_7 :
			case _XUSER_STATE_8 :
					return TRUE;
			}
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{
			if(m_SelectedSkillID > 0)
			{
				if(_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_POINT)
				{
					switch(m_CurrentBattleState)
					{
					case _XUSER_STATE_2 :
					case _XUSER_STATE_3 :
					case _XUSER_STATE_4 :
					case _XUSER_STATE_5 :
					case _XUSER_STATE_6 :
					case _XUSER_STATE_7 :
					case _XUSER_STATE_8 :
					case _XUSER_STATE_9 :
							return TRUE;
					}
				}
				else
				{
					switch(m_CurrentBattleState)
					{
					case _XUSER_STATE_2 :
					case _XUSER_STATE_3 :
					case _XUSER_STATE_4 :
					case _XUSER_STATE_5 :
					case _XUSER_STATE_6 :
					case _XUSER_STATE_7 :
						return TRUE;
					}
				}
			}
		}
		break;
	case _XSI_USEFULTYPE_SMASHINGSKILL :
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
	case _XSI_USEFULTYPE_ACTIONSKILL :
		{
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
					return TRUE;
			}
		}
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		{
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
			case _XUSER_STATE_6 :
			case _XUSER_STATE_7 :
				return TRUE;
			}
		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		{
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
					return TRUE;
			}
		}
		break;
	case _XSI_USEFULTYPE_HIDDENMA :
		{
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
			case _XUSER_STATE_6 :
			case _XUSER_STATE_7 :
			case _XUSER_STATE_8 :
			case _XUSER_STATE_9 :
				return TRUE;
			}
		}
		break;
	/*default : //Author : ����� //breif :6 /20 ������ ��
		{
			_XWindow_Inventory* pInventory_Window = (_XWindow_Inventory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INVENTORY);
			if(pInventory_Window)
			{
				if(pInventory_Window->m_bHaveMouseSlot)
					return TRUE;
			}
			_XWindow_NPCTrade* pNPCTrade_Window = (_XWindow_NPCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADE);
			if(pNPCTrade_Window)
			{
				if(pNPCTrade_Window->m_bHaveMouseSlot)
					return TRUE;
			}
			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
			if(pPCTrade_Window)
			{
				if(pPCTrade_Window->m_bHaveMouseSlot)
					return TRUE;
			}
			_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
			if(pWarehouse_Window)
			{
				if(pWarehouse_Window->m_bHaveMouseSlot)
					return TRUE;
			}
			_XWindow_RepairItem* pRepairitem_Window = (_XWindow_RepairItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REPAIRITEM);
			if( pRepairitem_Window )
			{
				if( pRepairitem_Window->m_bHaveMouseSlot )
					return TRUE;
			}
			if(g_pSkill_Window)
			{
				if(g_pSkill_Window->GetHaveMouseSlot())
					return TRUE;
			}
			if( g_ZeroLevelMode )	// hotblood zerolevel
			{
				XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;	
				if( pZeroLevelProcess  )
				{
					if( pZeroLevelProcess->GetEndEventSceneStart() )
						return TRUE;
					if( !pZeroLevelProcess->GetFirstEventSceneEnd() )
						return TRUE;
					if( pZeroLevelProcess->GetAllEventEnd() )
						return FALSE;
				}
			}
		}
		break;*/
	}

	if(m_UserState == _XDEF_USERSTATE_TRAINING)
	{
		if(m_SelectedTrainingMode == _XTRAINING_MEDITATION)
		{
			switch(m_CurrentTrainingState)
			{
			case _XUSER_STATE_1 :
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
			case _XUSER_STATE_6 :
			case _XUSER_STATE_7 :
				return TRUE;
			}
		}
		if(m_SelectedTrainingMode == _XTRAINING_DUMMY)
		{
			switch(m_CurrentTrainingState)
			{
			case _XUSER_STATE_1 :
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
				return TRUE;
			}
		}
		if(m_SelectedTrainingMode == _XTRAINING_SPELL)
		{
			switch(m_CurrentTrainingState)
			{
			case _XUSER_STATE_1 :
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
				return TRUE;
			}
		}

		// by mahwang
		if(m_SelectedTrainingMode == _XTRAINING_FIREWOODCUT)
		{
			switch(m_CurrentTrainingState)
			{
			case _XUSER_STATE_1 :
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
				return TRUE;
			}
		}
		
		// by mahwang
		// ������ �̵��� �����ؾ� �Ѵ�.
		if(m_SelectedTrainingMode == _XTRAINING_WATERDRAW)
		{
			switch(m_CurrentTrainingState)
			{
				// ���� ��� ���۰� ���� �״� ������ �������� ���ϵ��� �Ѵ�.
			case _XUSER_STATE_4 :
			case _XUSER_STATE_7 :
				return TRUE;
			}
		}
	}
	
    _XWindow_Inventory* pInventory_Window = (_XWindow_Inventory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INVENTORY);
	if(pInventory_Window)
	{
		if(pInventory_Window->m_bHaveMouseSlot)
			return TRUE;
	}
	_XWindow_NPCTrade* pNPCTrade_Window = (_XWindow_NPCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADE);
	if(pNPCTrade_Window)
	{
		if(pNPCTrade_Window->m_bHaveMouseSlot)
			return TRUE;
	}
	_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
	if(pPCTrade_Window)
	{
		if(pPCTrade_Window->m_bHaveMouseSlot)
			return TRUE;
	}
	_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	if(pWarehouse_Window)
	{
		if(pWarehouse_Window->m_bHaveMouseSlot)
			return TRUE;
	}
	_XWindow_RepairItem* pRepairitem_Window = (_XWindow_RepairItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REPAIRITEM);
	if( pRepairitem_Window )
	{
		if( pRepairitem_Window->m_bHaveMouseSlot )
			return TRUE;
	}
	if(g_pSkill_Window)
	{
		if(g_pSkill_Window->GetHaveMouseSlot())
			return TRUE;
	}
	if( g_ZeroLevelMode )	// hotblood zerolevel
	{
		XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;	
		if( pZeroLevelProcess  )
		{
			if( pZeroLevelProcess->GetEndEventSceneStart() )
				return TRUE;
			if( !pZeroLevelProcess->GetFirstEventSceneEnd() )
				return TRUE;
			if( pZeroLevelProcess->GetAllEventEnd() )
				return FALSE;
		}
	}

	// Ż�� ���̸� �̵��� �Ұ����ϴ�.
#ifdef _XDEF_ESCAPE_20070205
	_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
	if(pEscape_Window)
	{					
		if( pEscape_Window->GetShowStatus() )
		{
			// Ż�� ���̸� �̵��� �Ұ����ϴ�.
			// �޽��� ���� - 2007.07.13 KUKURI
//			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3313), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);// Ż�� ���̸� �̵��� �Ұ����ϴ�
			return TRUE;
		}
	}
#endif

	// ���� ������ ������ ���� �̵��� �Ұ����ϴ�
	if( m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
	{
		return TRUE;
	}

	// by mahwang
	// ä���Ҷ��� �̵��� �Ұ����ϴ�
	if (GetMotionClass() == _XACTION_COLLECT)
	{
		return TRUE;
	}

	// ����ȿ������ �̵� �� �� ����
	if(m_Lock.movelock == 1)
	{
		return TRUE;
	}	

	// å�� �д� �����̸� �̵� �Ұ�
	if( m_bReadingBook )
		return TRUE;
	// ���� ���� ���̸� �̵� �Ұ�
	if( m_bInternalUse )
		return TRUE;

	// ���� ���� ���̸� �̵� �Ұ�
	if( m_bSocketPlugIn )
		return TRUE; 

	// �׺� �ڽ� ���� ���̸� �̵� �Ұ�
	if( m_bGambleBoxOpen )
		return TRUE;

	// ���� Ư�� �������̸� �̵� �Ұ�
	if(m_UserState == _XDEF_USERSTATE_MATCH_MATCHING || m_UserState == _XDEF_USERSTATE_MATCH_BOW)
		return TRUE;

	// �� ȫ��/ȫ������ ������ ���̸� �̵� �Ұ�
	if( m_bMatchPRMode || m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_END)
		return TRUE;
	
	// ����� �����߿� ���� ���� ���̸� �̵� �Ұ�
	if(m_bThreatening)
	{
		return TRUE;
	}
	
	return FALSE;
}

#define _XDEF_ITEMPICKINGENABLETIME 500

void _XLocalUser::GrabWorldItem(void)
{
	if(m_PickItem)
	{
		if( m_PvpNPCLock )	// PVP �� Lock 
		{
			g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1578), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			return;
		}

		if( m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
		{
			g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1579), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			return;
		}

        //��Ʈ���� ���龲�� ������ �� �ݰ�..
    #if defined(_XVIETNAMESE)
        if( m_bFaceMaskMode )
        {
            g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_3829), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS ); //���� ���� �߿��� ��ǰ�� ������ �� �����ϴ�.
			return;
        }
    #endif

		FLOAT dx = m_Position.x - m_PickItem->m_Position.x;
		FLOAT dz = m_Position.z - m_PickItem->m_Position.z;
		FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

		if(distance < 1.3f)
		{
			DWORD lastpickeditemtime = ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LastPickedItemTime;
			if((g_LocalSystemTime - lastpickeditemtime) > _XDEF_ITEMPICKINGENABLETIME)
			{
				_XWindow_Inventory* pInventory_Window = (_XWindow_Inventory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INVENTORY);
				if(pInventory_Window)
				{
					if(!pInventory_Window->m_bHaveMouseSlot)
					{
						((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LastPickedItemTime = g_LocalSystemTime;

						int slotnumber = GetEmptyInventorySlot(m_PickItem);

						if(slotnumber != -1)
							g_NetworkKernel.SendPacket(MSG_NO_ITEM_GRAB_REQ, m_PickItem->m_ItemWorldID, slotnumber);
						else
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAIGAME_NOTEMPTY), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);

						m_PickItem = NULL;
						m_bGrabItem = FALSE;
					} // if(!pInventory_Window->m_bHaveMouseSlot)
				}
			}
		} // if(distance < 1.3f)
		else
		{
			D3DXVECTOR3 directionvector = m_Position - m_PickItem->m_Position;
			D3DXVec3Normalize(&directionvector, &directionvector);
			SetTargetPositionAngle(m_PickItem->m_Position);
		}
	}
}

#ifdef _XTS_ITEM_OPTIMIZATION
int _XLocalUser::GetEmptyInventorySlot(_XGameItem* pItem)
{
	if(pItem->m_cType == _XGI_FC_MONEY)
	{
		return _XINVENTORY_SLOTTYPE_MONEYINDEX;
	}

	char itemType = GetEqualItemType(pItem->m_cType);
	if( itemType == -1 )
		return -1;

	switch(itemType)
	{
	case _XGI_FC_CLOTHES :
	case _XGI_FC_BOOK :
	case _XGI_FC_ACCESSORY :
	case _XGI_FC_MUSIC :	
	case _XGI_FC_SOCKET :
	case _XGI_FC_ELIXIR :
	case _XGI_FC_BOX :
		{
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if(m_UserItemList[i].Get_m_sID() == 0)
					return i;
			}
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if(m_UserItemList[i].Get_m_sID() == 0)
					return i;
			}
		#endif
		}
		break;
	case _XGI_FC_WEAPON :
		{
			if(pItem->m_cSecond == _XGI_SC_WEAPON_HIDDEN)
			{
			#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			    {
				    if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				    {
					    if(m_UserItemList[i].Get_m_ucCount() < 255)
						    return i;
				    }
			    }
                for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			    {
				    if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                    {
                        if(m_UserItemList[i].Get_m_ucCount() < 255)
						    return i;
                    }
                }

                for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			    {
                    if( m_UserItemList[i].Get_m_sID() == 0 )
					    return i;
                }

			    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			    {
				    if( m_UserItemList[i].Get_m_sID() == 0 )
					    return i;
			    }
			#else
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
				{
					if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
					{
						if(m_UserItemList[i].Get_m_ucCount() >= 255)
							continue;
						
						return i;
					}
				}
				if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
				{
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(m_UserItemList[i].Get_m_sID() == 0)
							return i;
					}
				}
			#endif
			}
			else
			{
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
				{
					if(m_UserItemList[i].Get_m_sID() == 0)
						return i;
				}
			#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
				for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
				{
					if(m_UserItemList[i].Get_m_sID() == 0)
						return i;
				}
			#endif
			}
		}
		break;
	case _XGI_FC_LIFE :
		{
			switch(pItem->m_cSecond) 
			{
			case _XGI_SC_LIFE_SOCKET :
				{
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(m_UserItemList[i].Get_m_sID() == 0)
							return i;
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(m_UserItemList[i].Get_m_sID() == 0)
							return i;
					}
				#endif
				}
				break;
/*			case _XGI_SC_LIFE_CONSUMING :
			case _XGI_SC_LIFE_INCHANTSTUFF :
			case _XGI_SC_LIFE_SOCKETSSTUFF :*/
			default :
				{
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			        {
				        if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				        {
					        if(m_UserItemList[i].Get_m_ucCount() < 255)
						        return i;
				        }
			        }
                    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			        {
				        if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                        {
                            if(m_UserItemList[i].Get_m_ucCount() < 255)
						        return i;
                        }
                    }

                    for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			        {
                        if( m_UserItemList[i].Get_m_sID() == 0 )
					        return i;
                    }

			        for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			        {
				        if( m_UserItemList[i].Get_m_sID() == 0 )
					        return i;
			        }
				#else
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
						{
							if(m_UserItemList[i].Get_m_ucCount() >= 255)
								continue;
							
							return i;
						}
					}
					if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
					{
						for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
						{
							if(m_UserItemList[i].Get_m_sID() == 0)
								return i;
						}
					}
				#endif
				}
				break;
			}
		}
		break;
	case _XGI_FC_POTION :
		{
			switch(pItem->m_cSecond)
			{
			case _XGI_SC_POTION_NOTMERGE :
				{
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(m_UserItemList[i].Get_m_sID() == 0)
							return i;
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(m_UserItemList[i].Get_m_sID() == 0)
							return i;
					}
				#endif
				}
				break;
			default :
				{
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			        {
				        if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				        {
					        if(m_UserItemList[i].Get_m_ucCount() < 255)
						        return i;
				        }
			        }
                    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			        {
				        if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                        {
                            if(m_UserItemList[i].Get_m_ucCount() < 255)
						        return i;
                        }
                    }

                    for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			        {
                        if( m_UserItemList[i].Get_m_sID() == 0 )
					        return i;
                    }

			        for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			        {
				        if( m_UserItemList[i].Get_m_sID() == 0 )
					        return i;
			        }
				#else
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
						{
							if(m_UserItemList[i].Get_m_ucCount() >= 255)
								continue;
							
							return i;
						}
					}
					if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
					{
						for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
						{
							if(m_UserItemList[i].Get_m_sID() == 0)
								return i;
						}
					}
				#endif
				}
				break;
			}
		}
		break;
	case _XGI_FC_CONSUMING :			
	case _XGI_FC_COUNTERACT :
	case _XGI_FC_RESOURCE :
	case _XGI_FC_QUEST :	
	case _XGI_FC_PASS :
	case _XGI_FC_BOXKEY :
		{
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				{
					if(m_UserItemList[i].Get_m_ucCount() < 255)
						return i;
				}
			}
            for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                {
                    if(m_UserItemList[i].Get_m_ucCount() < 255)
						return i;
                }
            }

            for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
                if( m_UserItemList[i].Get_m_sID() == 0 )
					return i;
            }

			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if( m_UserItemList[i].Get_m_sID() == 0 )
					return i;
			}
		#else
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				{
					if(m_UserItemList[i].Get_m_ucCount() >= 255)
						continue;
					
					return i;
				}
			}
			if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
			{
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
				{
					if(m_UserItemList[i].Get_m_sID() == 0)
						return i;
				}
			}
		#endif
		}
		break;
	default :
		{
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if(m_UserItemList[i].Get_m_sID() == 0)
					return i;
			}
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if(m_UserItemList[i].Get_m_sID() == 0)
					return i;
			}
		#endif
		}
		break;
	}

	return -1;
}
#else
int _XLocalUser::GetEmptyInventorySlot(_XGameItem* pItem)
{
	if(pItem->m_cType == _XGI_FC_MONEY)
	{
		return _XINVENTORY_SLOTTYPE_MONEYINDEX;
	}

	switch(pItem->m_cType)
	{
	case _XGI_FC_CLOTHES :
	case _XGI_FC_CLOTHES2 : 
	case _XGI_FC_CLOTHES3 : 
	case _XGI_FC_BOOK :
	case _XGI_FC_ACCESSORY :
	case _XGI_FC_MUSIC :	
	case _XGI_FC_SOCKET :
	case _XGI_FC_ELIXIR :
	case _XGI_FC_BOX :
		{
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if(m_UserItemList[i].m_sID == 0)
					return i;
			}
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if(m_UserItemList[i].m_sID == 0)
					return i;
			}
		#endif
		}
		break;
	case _XGI_FC_WEAPON :
	case _XGI_FC_WEAPON2 :
	case _XGI_FC_WEAPON3 :
		{
			if(pItem->m_cSecond == _XGI_SC_WEAPON_HIDDEN)
			{
			#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			    {
				    if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				    {
					    if(m_UserItemList[i].m_ucCount < 255)
						    return i;
				    }
			    }
                for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			    {
				    if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                    {
                        if(m_UserItemList[i].m_ucCount < 255)
						    return i;
                    }
                }

                for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			    {
                    if( m_UserItemList[i].Get_m_sID() == 0 )
					    return i;
                }

			    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			    {
				    if( m_UserItemList[i].Get_m_sID() == 0 )
					    return i;
			    }
			#else
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
				{
					if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].m_sID == pItem->m_sID))
					{
						if(m_UserItemList[i].m_ucCount >= 255)
							continue;
						
						return i;
					}
				}
				if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
				{
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(m_UserItemList[i].m_sID == 0)
							return i;
					}
				}
			#endif
			}
			else
			{
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
				{
					if(m_UserItemList[i].m_sID == 0)
						return i;
				}

			#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
				for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
				{
					if(m_UserItemList[i].m_sID == 0)
						return i;
				}
			#endif
			}
		}
		break;
	case _XGI_FC_LIFE :
		{
			switch(pItem->m_cSecond) 
			{
			case _XGI_SC_LIFE_SOCKET :
				{
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(m_UserItemList[i].m_sID == 0)
							return i;
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(m_UserItemList[i].m_sID == 0)
							return i;
					}
				#endif
				}
				break;
			case _XGI_SC_LIFE_CONSUMING :
			case _XGI_SC_LIFE_INCHANTSTUFF :
			case _XGI_SC_LIFE_SOCKETSSTUFF :
				{
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			        {
				        if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				        {
					        if(m_UserItemList[i].m_ucCount < 255)
						        return i;
				        }
			        }
                    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			        {
				        if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                        {
                            if(m_UserItemList[i].m_ucCount < 255)
						        return i;
                        }
                    }

                    for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			        {
                        if( m_UserItemList[i].Get_m_sID() == 0 )
					        return i;
                    }

			        for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			        {
				        if( m_UserItemList[i].Get_m_sID() == 0 )
					        return i;
			        }
				#else
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].m_sID == pItem->m_sID))
						{
							if(m_UserItemList[i].m_ucCount >= 255)
								continue;
							
							return i;
						}
					}
					if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
					{
						for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
						{
							if(m_UserItemList[i].m_sID == 0)
								return i;
						}
					}
				#endif
				}
				break;
			}
		}
		break;
	case _XGI_FC_POTION :
	case _XGI_FC_CONSUMING :			
	case _XGI_FC_COUNTERACT :
	case _XGI_FC_RESOURCE :
	case _XGI_FC_QUEST :	
	case _XGI_FC_PASS :
	case _XGI_FC_BOXKEY :
		{
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				{
					if(m_UserItemList[i].m_ucCount < 255)
						return i;
				}
			}
            for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                {
                    if(m_UserItemList[i].m_ucCount < 255)
						return i;
                }
            }

            for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
                if( m_UserItemList[i].Get_m_sID() == 0 )
					return i;
            }

			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if( m_UserItemList[i].Get_m_sID() == 0 )
					return i;
			}
		#else
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].m_sID == pItem->m_sID))
				{
					if(m_UserItemList[i].m_ucCount >= 255)
						continue;
					
					return i;
				}
			}
			if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
			{
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
				{
					if(m_UserItemList[i].m_sID == 0)
						return i;
				}
			}
		#endif
		}
		break;
	default :
		{
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if(m_UserItemList[i].m_sID == 0)
					return i;
			}

		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if(m_UserItemList[i].m_sID == 0)
					return i;
			}
		#endif

		}
		break;
	}

	return -1;
}
#endif

void _XLocalUser::ProcessStateList(void)
{
	m_StateList.ProcessState();
}

void _XLocalUser::DrawStateList(void)
{
#ifdef _XDWDEBUG
#ifdef _XTS_PK
/*
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));

	if(m_UserState == _XDEF_USERSTATE_PK_READY)
		g_XBaseFont->Print(gnWidth/2, gnHeight/2, 1.0f, "userstate : pk ready");
	else if(m_UserState == _XDEF_USERSTATE_PK_FIGHT)
	{
		g_XBaseFont->Print(gnWidth/2, gnHeight/2, 1.0f, "userstate : pk fight");

		DWORD lefttime = (m_PKModeStartTime + m_PKModeLeftTime) - g_LocalSystemTime;
		g_XBaseFont->Print(gnWidth/2, gnHeight/2+15, 1.0f, "lefttime : %d", lefttime);
	}
	else if(m_UserState  == _XDEF_USERSTATE_PK_END)
		g_XBaseFont->Print(gnWidth/2, gnHeight/2, 1.0f, "userstate : pk end");
*/
#endif
#endif

#ifdef _XDWDEBUG

	extern BOOL	g_ShowUserStatus;
	
	if( g_ShowUserStatus )
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		
		int i = 0;
		DWORD lefttime = 0;

		map <int, _XCharacterState*>::iterator iter_state;
		for(iter_state = m_StateList.m_mapCharacterStateList.begin() ; iter_state != m_StateList.m_mapCharacterStateList.end() ; ++iter_state)
		{
			_XCharacterState* pState = iter_state->second;

			if(pState)
			{
				lefttime = (pState->m_dwStateStartTime + pState->m_usLeftTimeSec*1000) - g_LocalSystemTime;
				
				g_XBaseFont->Print(gnWidth/2, gnHeight/2 + i*15, 1.0f, "id[%d] / skillid[%d] / lefttimefromserver [%ds] /leftime[%d ms]", 
					pState->m_sEffectID, pState->m_sSkillID, pState->m_usLeftTimeSec, lefttime);
				i++;
			}
		}
		g_XBaseFont->Flush();
	}		
#endif
}

DWORD _XLocalUser::GetLeftTimeFromStateList(char fromtype, short effectid, short skillid, char slotindex)
{
	int index = m_StateList.FindState(fromtype, effectid, skillid, slotindex);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_StateList.m_mapCharacterStateList.find(index);
		if(iter_state != m_StateList.m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				DWORD lefttime = currentstate->m_usLeftTimeSec * 1000;

				if((currentstate->m_dwStateStartTime + lefttime) <= g_LocalSystemTime)
					return 0;
				else
					return ((currentstate->m_dwStateStartTime + lefttime) - g_LocalSystemTime);
			}
		}
	}
	
	return 0;
}

void _XLocalUser::CharacterStateChatLock(TCHAR* message)
{
	int msglength = strlen(message);
	int randomvalue = 0;
	TCHAR changeStr[] = {"!@#$%^&*()_+=-~`<>?/.,"};

	for( int i = 0; i < msglength; ++i )
	{
		randomvalue = _XRandom() % 22;
		message[i] = changeStr[randomvalue];
	}
}

//	#ifdef _XTS_USECASHITEM -�׽�Ʈ��
void _XLocalUser::UseItem(int invennumber)
{
	if(!g_pState_Window)
		return;
	
	if(invennumber < 0)
		return;

	if(m_UserItemList[invennumber].Get_m_sID() <= 0)
		return;
	
#ifdef _XDEF_ESCAPE_20070205
	// Ż�� �߿��� ����, ���� Ű�� ����� �� ����.
	_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
	if(pEscape_Window)
	{
		if(pEscape_Window->GetShowStatus())	return;
	}
#endif

#ifdef _XDEF_CASTLEBATTLE
	if(m_bThreatening)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3355), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "���� ���� �߿��� ����� �� �����ϴ�."
		return;
	}
#endif
	
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
	if(g_pLocalUser->GetEnterMatchEvent())
	{
		if(!g_MatchEventManager.CanUseItem(invennumber))
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3684), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//�� �߿��� ����� �� �����ϴ�.
			return;
		}
	}
#endif
	
	// errcode
	// 0 : ����
	// 1 : ���� ����� �� �����ϴ�.
	// 2 : ���� ���¿����� ����� �� �����ϴ�.
	// 3 : ������ ����� �� �����ϴ�.
	// 4 : �ٸ� ������ �����Դϴ�.
	// 5 : �ٸ� ���������� ȿ���� ���� �ֽ��ϴ�(������ �� ����)
	// 6 : ���� ����� �� �����ϴ�.(����ð� ���� ��������)
	// 7 : Ÿ ������ �����Ĵ� ����� �� �����ϴ�.
	// 8 : ������ ���Ŀ� �����Ĵ� ����� �� �����ϴ�.
	int errcode = 0;

	switch(m_UserItemList[invennumber].Get_m_cType())
	{
	case _XGI_FC_POTION :
		{
			// ���� ���� ó�� -----------------------------------------------��
			if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(m_CurrentBattleState != _XUSER_STATE_NONE && m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1580), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return;
			}
			
			if( m_bReadingBook )	// ��� �д� ���̸� ����
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1581), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return;
			}
			if( m_bSocketPlugIn )	// ���� ���� ���̸� ����
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1582), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return ;
			}
			if( m_bGambleBoxOpen )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1583), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return;
			}
			if( g_pLocalUser->m_bInternalUse )	// ���� ���� ���̸� ����
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1584), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return ;
			}
#ifdef _XTS_MASKSYSTEM
			if( g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].cSecondType == _XGI_SC_POTION_MASK )
			{
				if(m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)	// ����
				{
					if(g_pState_Window )
						g_pState_Window->PrintSystemMessage(5);
					return;
				}
				
				if( g_pLocalUser->m_bPEMode )
				{
					if(g_pState_Window )
						g_pState_Window->PrintSystemMessage(13);
					return;
				}
			}			
#endif
			// ------------------------------------------------------------------��
			
#ifdef _XDEF_EXPANSION_POTION_070709_KUKURI
			if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sReqLevel > m_CharacterInfo.Get_innerlevel())
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2333), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				return;
			}
#endif

			switch(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].cSecondType)
			{
			case _XGI_SC_POTION_HERT_IN :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_HERT_IN))
						errcode = 1;
				}
				break;
			case _XGI_SC_POTION_HERT_OUT :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_HERT_OUT))
						errcode = 1;
				}
				break;
			case _XGI_SC_POTION_FATIGUE :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_FATIGUE))
						errcode = 1;
				}
				break;
			case _XGI_SC_POTION_FOOD :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_FOOD))
						errcode = 1;

					if(m_BattleMode != _XBATTLE_PEACE)
						errcode = 2;
				}
				break;
			case _XGI_SC_POTION_EVENT :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_EVENT))
						errcode = 1;
				}
				break;
			case _XGI_SC_POTION_HWATA :
				{
					errcode = 3;
				}
				break;
#ifdef _XTS_MASKSYSTEM
			case _XGI_SC_POTION_MASK :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_MASK))
						errcode = 1;
				}
				break;
			case _XGI_SC_POTION_ANTI :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_ANTI))
						errcode = 1;
				}
				break;
#endif
			case _XGI_SC_POTION_COOLDOWN :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_COOLDOWN))
						errcode = 1;
				}
				break;
			case _XGI_SC_POTION_ADDEXP :
				{
					if(g_PotionItemProperty[m_AccountItemsID].sSimgi == 4)
					{
						// ���� ���� �����
						errcode = 9;
						break;
					}

					if(g_PotionItemProperty[m_AccountItemsID].sSimgi == 5)
					{
						// ���ο� ����õ�� �����
						errcode = 12;
						break;
					}
					if(g_PotionItemProperty[m_AccountItemsID].sSimgi == 6)
					{
						// ���� ���� �����
						errcode = 9;
						break;
					}

					TCHAR msgbuffer[512];
					memset(msgbuffer, 0, sizeof(msgbuffer));

					if(m_AccountItemsID > 0)
					{
						// ���� ������ �����
#ifdef _XDEF_AVOID_OVERLAPPING_CASHITEM_080721_KUKURI	// ���� ������ �ߺ� ��� �Ұ�
						errcode = 15;
						break;
#else
						_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2959));	//������ ����, ���� ������̴� ���� ������ ���񽺴� ������ϴ�. ����Ͻðڽ��ϱ�?
#endif
					}
					else
					{
						if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 1)
						{
							_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2960));	//����ġ ���� �������� ����Ͻðڽ��ϱ�? ������ �������� ��� �� �����˴ϴ�.
						}
						else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 2)
						{
							_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2961));	//���õ� ���� �������� ����Ͻðڽ��ϱ�? ������ �������� ��� �� �����˴ϴ�.
						}
						else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 3)
						{
							_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2962));	//����ġ/���õ� ���� �������� ����Ͻðڽ��ϱ�? ������ �������� ��� �� �����˴ϴ�.
						}
						else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 4)
						{
							_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2971));	//����õ�ϸ� ����Ͻðڽ��ϱ�? ������ �������� ��� �� �����˴ϴ�.
						}
						else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 5)
						{
							_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_3415));	//����õ��(��Ī)�� ����Ͻðڽ��ϱ�? ������ �������� ��� �� �����˴ϴ�.
						}
						else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 6) //Author :����� //breif : ����õ�� 6 �߰�
						{
							_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2971));	//����õ�ϸ� ����Ͻðڽ��ϱ�? ������ �������� ��� �� �����˴ϴ�.
						}
					}

					if(errcode == 0)
					{
						m_UseItemInvenNumber = invennumber;

#ifdef _XTS_USECASHITEM
						_XMessageWindow* pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
#else
						_XMessageWindow* pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
#endif
						pMessageWindow->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), msgbuffer,TRUE, _XDEF_CASHITEMUSE_ADDEXPYES, _XDEF_CASHITEMUSE_ADDEXPNO);
						//pMessageWindow->StartTimer(5000, TRUE);
						
						return;
					}
				}
				break;
			case _XGI_SC_POTION_RESETPOINT :
				{
					TCHAR msgbuffer[512];
					memset(msgbuffer, 0, sizeof(msgbuffer));
					
					if( g_pLocalUser->m_CharacterInfo.Get_cClassGrade() > g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sJungGi )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3351), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
						return;
					}
					
					if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 1)
					{
						_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2963));	//���� �ʱ�ȭ �������� ����Ͻðڽ��ϱ�? ���� ��� ���Ⱑ �ʱ�ȭ �˴ϴ�.
					}
					else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 2)
					{
						_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2964)); //���� �ʱ�ȭ �������� ����Ͻðڽ��ϱ�? ���� ��� ���ǰ� �ʱ�ȭ �˴ϴ�.
					}
					else
					{
						errcode = 1;
					}

					if(errcode == 0)
					{
						m_UseItemInvenNumber = invennumber;

#ifdef _XTS_USECASHITEM
						_XMessageWindow* pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
#else
						_XMessageWindow* pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
#endif

						pMessageWindow->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), msgbuffer,TRUE, _XDEF_CASHITEMUSE_RESETPOINTYES, _XDEF_CASHITEMUSE_RESETPOINTNO);
						//pMessageWindow->StartTimer(5000, TRUE);

						return;
					}
				}
				break;
			case _XGI_SC_POTION_NICKNAME :
				{
					if(g_PotionItemProperty[m_AccountItemsID].sSimgi == 4 || g_PotionItemProperty[m_AccountItemsID].sSimgi == 5 ||
						g_PotionItemProperty[m_AccountItemsID].sSimgi == 6)
					{
						// ���� ���� �����
						errcode = 10;
						break;
					}

					_XWindow_NickName* pNickName_Window = (_XWindow_NickName*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NICKNAME);
					if(pNickName_Window)
					{
						if(pNickName_Window->m_RemainChangeTime == 0)
						{
							// ��ü �ð��� �̹� 0
							errcode = 11;
							break;
						}
					}

					if(errcode == 0)
					{
						m_UseItemInvenNumber = invennumber;

						// system message �߰�
#ifdef _XTS_USECASHITEM
						_XMessageWindow* pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
#else
						_XMessageWindow* pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
#endif
						pMessageWindow->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2968),TRUE, _XDEF_CASHITEMUSE_CLEARNICKNAMEYES, _XDEF_CASHITEMUSE_CLEARNICKNAMENO);
						//pMessageWindow->StartTimer(5000, TRUE);
						
						return;
					}
				}
				break;
#ifdef _XDEF_CASHITEM_SHOUT
			case _XGI_SC_POTION_SHOUT :
				{
					if(g_pLocalUser->m_bVirtualMode)
					{
						errcode = 14;	// Ʃ�丮�󿡼� ��� �Ұ�
					}

					if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 1)
					{
						// �鵵 ��ġ��
						if(GetGroupType() != _XGROUPTYPE_WHITE)
						{
							errcode = 7;
						}
					}
					else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 2)
					{
						// �浵 ��ġ��
						if(GetGroupType() != _XGROUPTYPE_BLACK)
						{
							errcode = 7;
						}
					}
					else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 4)
					{
						// ���� ��ġ��
						if(m_CharacterInfo.groupindex == _XGROUP_NANGIN)
						{
							errcode = 8;
						}
					}

					if(g_pState_Window->FindUsingItemFromSecondType(_XST_SHOUT, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_SHOUT))
					{
						// ���� ����� �� �����ϴ�. - �����ð� üũ
						errcode = 1;
					}
					
					if(errcode == 0)
					{
						// �����̸� inven# ���� �� �޼��� �ڽ� ����
						m_UseItemInvenNumber = invennumber;

						TCHAR msgbuffer[512];
						memset(msgbuffer, 0, sizeof(msgbuffer));

						int time = g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sJungGi / 60;

						_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2958), time); // �����ĸ� ����Ͻðڽ��ϱ�? ���� %d�а� ��ġ�Ⱑ �����մϴ�.

#ifdef _XTS_USECASHITEM
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
#else
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
#endif
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), msgbuffer, TRUE, _XDEF_CASHITEMUSE_SHOUTYES, _XDEF_CASHITEMUSE_SHOUTNO);
						//pMessageBox->StartTimer(5000, TRUE);

						return;
					}
				}
				break;
#endif
			case _XGI_SC_POTION_RECALL : //Author : ����� //��ȯ��/ȣ���
				{
					errcode = 13; //�ƹ��͵� ������� �ʴ´�
				}
				break;
			case _XGI_SC_POTION_EVENT2 : //Author : ����� //breif : �ž� ������ ���߶� �߰�
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_EVENT2))
						errcode = 1;
				}
				break;
			}//SECONDTYPE

			// second type�� ������� ü���� 0���� ���� ���� ��� ���� ��� ����
			if(g_pLocalUser->m_CharacterInfo.Get_current_lifepower() <= 0)
			{
				errcode = 3;
			}

			switch(errcode)
			{
			case 0 :
				{
					// ����
					g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, invennumber);
				}
				break;
			case 1 :
				{
					// ���� ����� �� �����ϴ�.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				}
				break;
			case 2 :
				{
					// �������¿����� ����� �� �����ϴ�.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1586), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case 3 :
				{
					// ������ ����� �� �����ϴ�.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				}
				break;
			case 7 :
				{
					// Ÿ ������ �����Ĵ� ����� �� �����ϴ�.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2956), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case 8 :
				{
					// ������ ���Ŀ� �����Ĵ� ����� �� �����ϴ�.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2957), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case 9 :
				{
					// ���� õ�� ����ڴ� �� �������� ����� �� �����ϴ�.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2965), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case 10 :
				{
					// ��ȣ ������ �����ο� ���񽺸� �̹� �̿����Դϴ�.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2966), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case 11 :
				{
					// �������� ������� �ʾƵ� ��ȣ�� ��ü�� �� �ֽ��ϴ�.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2967), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case 12 :
				{
					// ���ο� ��Ʈ�� ����õ��
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3416), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//���� õ��(��Ī) ����ڴ� �� �������� ����� �� �����ϴ�.
				}
				break;
			case 13 ://Author : ����� �߰�
				{
					//�ƹ��͵� ������� ����
				}
				break;
			case 14 :
				{
					// Ʃ�丮�󿡼��� ����� �� ����
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3692), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"Ʃ�丮�󿡼��� ����� �� �����ϴ�."
				}
				break;
			case 15 : 
				{
					// ������ ȿ���� ������ ��� �߿��� �̿��� �� �����ϴ�.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3843), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//������ ȿ���� ������ ��� �߿��� �̿��� �� �����ϴ�.
				}
				break;
			default :
				{
					// ���� ����� �� �����ϴ�.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				}
				break;
			}
		}
		break;//POTION
	case _XGI_FC_CONSUMING :
			{
				// ���� ���� ó�� -----------------------------------------------��
				if( m_bReadingBook )	// ��� �д� ���̸� ����
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1581), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
					return;
				}
				if( m_bSocketPlugIn )	// ���� ���� ���̸� ����
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1582), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
					return ;
				}
				if( m_bGambleBoxOpen )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1583), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
					return;
				}
				if( g_pLocalUser->m_bInternalUse )	// ���� ���� ���̸� ����
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1584), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
					return ;
				}
				// ------------------------------------------------------------------��

				if( g_ConsumingItemProperty[m_UserItemList[invennumber].Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex )
					errcode = 4;
				
				switch(errcode)
				{
				case 0 :
					{

						// ����
						g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, invennumber);
					}
					break;
				case 4 :
					{
						// �ٸ� ������ �����Դϴ�.
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORGROUP), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					break;
				default :
					{
						// ���� ����� �� �����ϴ�.
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					break;
				}
			}
			break;//CONSUMING
		case _XGI_FC_ELIXIR :
			{
				switch(g_ElixirItemProperty[m_UserItemList[invennumber].Get_m_sID()].cSecondType)
				{
				case _XGI_SC_ELIXIR_SUPPORT :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_ELIXIR_SUPPORT))
							errcode = 5;
					}
					break;
				case _XGI_SC_ELIXIR_STRENGTH :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_ELIXIR_STRENGTH))
							errcode = 5;
					}
					break;
				case _XGI_SC_ELIXIR_INNER :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_ELIXIR_INNER))
							errcode = 5;
					}
					break;
				case _XGI_SC_ELIXIR_HEART :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_ELIXIR_HEART))
							errcode = 5;
					}
					break;
				case _XGI_SC_ELIXIR_HEALTH :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_ELIXIR_HEALTH))
							errcode = 5;
					}
					break;
				case _XGI_SC_ELIXIR_DEX :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_ELIXIR_DEX))
							errcode = 5;
					}
					break;
				}

				switch(errcode)
				{
				case 0 :
					{
						if( g_pInventory_Window )
						{
							if(g_ElixirItemProperty[m_UserItemList[invennumber].Get_m_sID()].cCashCheck == 1)
							{
								// ĳ�� ������ ����
								if(m_ElixirInfo._sIntoxication >= 100)
								{
									// �ߵ� ������ 100���� Ŭ ��� ����� �� ����
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3048), TRUE, 0 );
									pMessageBox->StartTimer(5000, TRUE);

									// ���� ó��
									g_pInventory_Window->m_ElixirInvenNumber = -1;
									m_CurrentInternalUseState = _XUSER_STATE_5;
									break;
								}
							}

							if( g_pInventory_Window->CheckIsInternalUse(m_UserItemList[invennumber].Get_m_sID()) )
							{
								// ����
								_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
								if( pChargeGauge_Window )
								{
									if(!pChargeGauge_Window->GetShowStatus())	// �������������� �������� �ؾ���. ���� ö��
									{
										pChargeGauge_Window->SetDelayTime( FLOAT(g_ElixirItemProperty[m_UserItemList[invennumber].Get_m_sID()].usCoolDownDelay*1000.0f) );
										pChargeGauge_Window->SetGaugeMode( _XGAUGE_ELIXIR );
										pChargeGauge_Window->SetSelectedNumber( invennumber);								
										pChargeGauge_Window->ShowWindow(TRUE);
										m_CurrentInternalUseState = _XUSER_STATE_4;
										_XPlayInterfaceSound(ID_SR_INTERFACE_USE_ELIXIR01_WAV);								
									}
								}
							}
							else
							{
								// ����
								g_pInventory_Window->m_ElixirInvenNumber = -1;						
								m_CurrentInternalUseState = _XUSER_STATE_5;
							}
						}
					}
					break;
				case 5 :
					{
						// �ٸ� ���������� ȿ���� ���� �ֽ��ϴ�(������ �� ����)
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1588), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
						g_pInventory_Window->m_ElixirInvenNumber = -1;
						m_CurrentInternalUseState = _XUSER_STATE_5;
						
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					}
					break;
				default :
					{
						// ���� ����� �� �����ϴ�.
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					break;
				}
			}
			break;//ELIXIR
		case _XGI_FC_QUEST :
			{
				if( g_QuestItemProperty[g_pLocalUser->m_UserItemList[invennumber].Get_m_sID()].cSecondType == 1 )
				{
					g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, invennumber);	
				}
			}
			break;//QUEST
		case _XGI_FC_BOOK :
			{
				switch(g_BookItemProperty[m_UserItemList[invennumber].Get_m_sID()].cSecondType)
				{
				case _XGI_SC_BOOK_SKILL :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_BOOK_SKILL))
							errcode = 6;
					}
					break;
				case _XGI_SC_BOOK_STUDY :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_BOOK_STUDY))
							errcode = 6;
					}
					break;
				case _XGI_SC_BOOK_ABILITY :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_BOOK_ABILITY))
							errcode = 6;
					}
					break;
				case _XGI_SC_BOOK_SCROLL :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_BOOK_SCROLL))
							errcode = 6;
					}
					break;
				case _XGI_SC_BOOK_STUFF :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_BOOK_STUFF))
							errcode = 6;
					}
					break;
				case _XGI_SC_BOOK_PAPER :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_BOOK_PAPER))
							errcode = 6;
					}
					break;
				}

				switch(errcode)
				{
				case 0 :
					{
						if( g_pInventory_Window )
						{
							if( g_pInventory_Window->CheckIsRead(m_UserItemList[invennumber].Get_m_sID()) )
							{
								// ����
								_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
								if( pChargeGauge_Window )
								{
									if(!pChargeGauge_Window->GetShowStatus())
									{
										pChargeGauge_Window->SetDelayTime( FLOAT(g_BookItemProperty[m_UserItemList[invennumber].Get_m_sID()].usDelay*1000.0f) );
										pChargeGauge_Window->SetGaugeMode( _XGAUGE_READ );
										pChargeGauge_Window->SetSelectedNumber( invennumber);
										pChargeGauge_Window->ShowWindow(TRUE);
										m_CurrentReadBookState = _XUSER_STATE_4;
									}
								}
							}
							else
							{
								// ����
								g_pInventory_Window->m_BookInvenNumber = -1;
								m_CurrentReadBookState = _XUSER_STATE_5;
							}
						}
					}
					break;
				case 6 :
					{
						// ���� ����� �� �����ϴ�.(����ð� ���� ��������)
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTUSE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
						g_pInventory_Window->m_BookInvenNumber = -1;
						m_CurrentReadBookState = _XUSER_STATE_5;
					}
					break;
				default :
					{
						// ���� ����� �� �����ϴ�.
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					break;
				}
			}
			break;//BOOK
	}
}
/*
#else
void _XLocalUser::UseItem(int invennumber)
{
	if(!g_pState_Window)
		return;

	bool bUse = true;
	bool bFood = true;

	if(m_UserItemList[invennumber].m_sID > 0)
	{
		if(m_UserItemList[invennumber].m_cType == _XGI_FC_POTION)
		{
			if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(m_CurrentBattleState != _XUSER_STATE_NONE && m_CurrentBattleState != _XUSER_STATE_LAST))	// �����ɹ� ������
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1580), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return;
			}
			
			if( m_bReadingBook )	// ��� �д� ���̸� ����
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1581), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return;
			}
			if( m_bSocketPlugIn )	// ���� ���� ���̸� ����
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1582), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return ;
			}
			if( m_bGambleBoxOpen )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1583), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return;
			}
			if( g_pLocalUser->m_bInternalUse )	// ���� ���� ���̸� ����
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1584), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return ;
			}
#ifdef _XTS_MASKSYSTEM
			if( g_PotionItemProperty[m_UserItemList[invennumber].m_sID].cSecondType == _XGI_SC_POTION_MASK )
			{
				if(m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)	// ����
				{
					if(g_pState_Window )
						g_pState_Window->PrintSystemMessage(5);
					return;
				}

				if( g_pLocalUser->m_bPEMode )
				{
					if(g_pState_Window )
						g_pState_Window->PrintSystemMessage(13);
					return;
				}
			}			
#endif			
			switch(g_PotionItemProperty[m_UserItemList[invennumber].m_sID].cSecondType)
			{
			case _XGI_SC_POTION_HERT_IN :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_POTION_HERT_IN))
						bUse = false;
				}
				break;
			case _XGI_SC_POTION_HERT_OUT :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_POTION_HERT_OUT))
						bUse = false;
				}
				break;
			case _XGI_SC_POTION_FATIGUE :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_POTION_FATIGUE))
						bUse = false;
				}
				break;
			case _XGI_SC_POTION_FOOD :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_POTION_FOOD))
						bUse = false;
#ifdef _XTS_PK
					if(m_BattleMode != _XBATTLE_PEACE)
#else
					if(m_BattleMode == _XBATTLE_BATTLE)
#endif
						bFood = false;
				}
				break;
			case _XGI_SC_POTION_HWATA :
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
					return;
				}
				break;
#ifdef _XTS_MASKSYSTEM
			case _XGI_SC_POTION_MASK :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_POTION_MASK))
						bUse = false;
				}
				break;
			case _XGI_SC_POTION_ANTI :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_POTION_ANTI))
						bUse = false;
				}
				break;
#endif
			}
			
			if(!bUse || !bFood)
			{
				if(!bFood)
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1586), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				else
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else
				g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, invennumber);
		} // if(m_UserItemList[invennumber].m_cType == _XGI_FC_POTION)	
		else if(m_UserItemList[invennumber].m_cType == _XGI_FC_CONSUMING)
		{
			if( m_bReadingBook )	// ��� �д� ���̸� ����
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1581), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return;
			}
			if( m_bSocketPlugIn )	// ���� ���� ���̸� ����
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1582), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return ;
			}
			if( m_bGambleBoxOpen )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1583), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return;
			}
			if( g_pLocalUser->m_bInternalUse )	// ���� ���� ���̸� ����
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1584), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return ;
			}

			if( g_ConsumingItemProperty[m_UserItemList[invennumber].m_sID].cClan == g_pLocalUser->m_CharacterInfo.groupindex )
				g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, invennumber);						
			else 
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORGROUP), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		}
		else if( m_UserItemList[invennumber].m_cType == _XGI_FC_ELIXIR )
		{
			switch(g_ElixirItemProperty[m_UserItemList[invennumber].m_sID].cSecondType)
			{
			case _XGI_SC_ELIXIR_SUPPORT :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_ELIXIR_SUPPORT))
						bUse = false;
				}
				break;
			case _XGI_SC_ELIXIR_STRENGTH :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_ELIXIR_STRENGTH))
						bUse = false;
				}
				break;
			case _XGI_SC_ELIXIR_INNER :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_ELIXIR_INNER))
						bUse = false;
				}
				break;
			case _XGI_SC_ELIXIR_HEART :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_ELIXIR_HEART))
						bUse = false;
				}
				break;
			case _XGI_SC_ELIXIR_HEALTH :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_ELIXIR_HEALTH))
						bUse = false;
				}
				break;
			case _XGI_SC_ELIXIR_DEX :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_ELIXIR_DEX))
						bUse = false;
				}
				break;
			}

			if(!bUse)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1588), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				g_pInventory_Window->m_ElixirInvenNumber = -1;
				m_CurrentInternalUseState = _XUSER_STATE_5;

				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
			}
			else
			{				
				if( g_pInventory_Window )
				{
					if( g_pInventory_Window->CheckIsInternalUse(m_UserItemList[invennumber].m_sID) )
					{
						_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
						if( pChargeGauge_Window )
						{
							if(!pChargeGauge_Window->GetShowStatus())	// �������������� �������� �ؾ���. ���� ö��
							{
								pChargeGauge_Window->SetDelayTime( FLOAT(g_ElixirItemProperty[m_UserItemList[invennumber].m_sID].usCoolDownDelay*1000.0f) );
								pChargeGauge_Window->SetGaugeMode( _XGAUGE_ELIXIR );
								pChargeGauge_Window->SetSelectedNumber( invennumber);								
								pChargeGauge_Window->ShowWindow(TRUE);
								m_CurrentInternalUseState = _XUSER_STATE_4;
								_XPlayInterfaceSound(ID_SR_INTERFACE_USE_ELIXIR01_WAV);								
							}
						}							
					}
					else
					{
						g_pInventory_Window->m_ElixirInvenNumber = -1;						
						m_CurrentInternalUseState = _XUSER_STATE_5;
					}
				}				
			}
		}
		else if( m_UserItemList[invennumber].m_cType == _XGI_FC_QUEST )
		{
			if( g_QuestItemProperty[g_pLocalUser->m_UserItemList[invennumber].m_sID].cSecondType == 1 )
			{
				g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, invennumber);	
			}
		}
		else if(m_UserItemList[invennumber].m_cType == _XGI_FC_BOOK)
		{
			switch(g_BookItemProperty[m_UserItemList[invennumber].m_sID].cSecondType)
			{
			case _XGI_SC_BOOK_SKILL :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_BOOK_SKILL))
						bUse = false;
				}
				break;
			case _XGI_SC_BOOK_STUDY :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_BOOK_STUDY))
						bUse = false;
				}
				break;
			case _XGI_SC_BOOK_ABILITY :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_BOOK_ABILITY))
						bUse = false;
				}
				break;
			case _XGI_SC_BOOK_SCROLL :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_BOOK_SCROLL))
						bUse = false;
				}
				break;
			case _XGI_SC_BOOK_STUFF :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_BOOK_STUFF))
						bUse = false;
				}
				break;
			case _XGI_SC_BOOK_PAPER :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_BOOK_PAPER))
						bUse = false;
				}
				break;
			}
			if(!bUse)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTUSE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
				g_pInventory_Window->m_BookInvenNumber = -1;
				m_CurrentReadBookState = _XUSER_STATE_5;
			}
			else
			{
				
				if( g_pInventory_Window )
				{
					if( g_pInventory_Window->CheckIsRead(m_UserItemList[invennumber].m_sID) )
					{
						_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
						if( pChargeGauge_Window )
						{
							if(!pChargeGauge_Window->GetShowStatus())
							{
								pChargeGauge_Window->SetDelayTime( FLOAT(g_BookItemProperty[m_UserItemList[invennumber].m_sID].usDelay*1000.0f) );
								pChargeGauge_Window->SetGaugeMode( _XGAUGE_READ );
								pChargeGauge_Window->SetSelectedNumber( invennumber);
								pChargeGauge_Window->ShowWindow(TRUE);
								m_CurrentReadBookState = _XUSER_STATE_4;
							}
						}
					}
					else
					{
						g_pInventory_Window->m_BookInvenNumber = -1;
						m_CurrentReadBookState = _XUSER_STATE_5;
					}
				}				
			}			
		}  // BOOK		
	}
}
#endif
*/
BOOL _XLocalUser::HaveSkillType(char clan, char stype)
{
	if(clan >= 0)
	{
		if(stype == 0)
		{
			// ���� ���� üũ
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
			{
				if(g_SkillProperty[m_SkillList[i].m_sSkillID]->clanType == clan)
					return TRUE;
			}
		}
		else
		{
			// ���� stype üũ
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
			{
				if(g_SkillProperty[m_SkillList[i].m_sSkillID]->clanType == clan && 
					g_SkillProperty[m_SkillList[i].m_sSkillID]->skillSType == stype)
					return TRUE;
			}
		}
	}
	
	return FALSE;
}

void _XLocalUser::SetDyingMode(void)
{
	if(GetMotionClass() != _XACTION_DYING)
	{
		// ��Ȱ ������ ����
		_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
		if( pRebirth_Window && !g_ZeroLevelMode && m_UserState != _XDEF_USERSTATE_MATCH_FIGHT)
		{
			if(!pRebirth_Window->GetShowStatus())
			{
				pRebirth_Window->SetRebirthMode(_XREBIRTH_BINSA);
#ifdef _ACCLAIM_IGAADSYSTEM
				if( g_IGAADWrapper.CheckExistContent( _XIGAADZONE_DEATHVIDEOAD ) )
				{
					pRebirth_Window->m_ShowIGAADButton = TRUE;
				}
				else
				{
					pRebirth_Window->m_ShowIGAADButton = FALSE;
				}
#endif
				pRebirth_Window->ShowWindow(TRUE);
			}
		}

		SetAnimation(_XACTION_DYING);
		_XPlayInterfaceSound(ID_SR_INTERFACE_LEVELFAIL_WAV);

#ifdef _XDEF_PARTY_NOTAVAILABLE
		// �ڸ���� �����̸� ����
		if(m_bOut)
		{
			SetOut(FALSE);
		}
#endif
	}
}


void InsertAttackResultFactorEffect( int resultfactor )
{

}

short _XLocalUser::GetStanceMotion(short activeslotid)
{
	if(m_BattleMode == _XBATTLE_BATTLE || m_BattleMode == _XBATTLE_PK)
	{
		short stancemotion = -1;
		
		if(activeslotid > 0)
		{
			if(m_CharacterInfo.gender == _XGENDER_MAN)
				stancemotion = g_SkillProperty[activeslotid]->stance_male;
			else
				stancemotion = g_SkillProperty[activeslotid]->stance_female;
		}

		if(stancemotion < 0)
		{
			if(m_CharacterInfo.modeldescript.item1 != 0)
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						if(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							stancemotion = _XAN_BATTLEIDLE_SWORD_START;
						}
					}
				}
#else
				if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON)
				{
					if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
						g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						stancemotion = _XAN_BATTLEIDLE_SWORD_START;
					}
				}
				else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2)
				{
					if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
						g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						stancemotion = _XAN_BATTLEIDLE_SWORD_START;
					}
				}
				else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3)
				{
					if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
						g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						stancemotion = _XAN_BATTLEIDLE_SWORD_START;
					}
				}
#endif
			}
		}

		return stancemotion;
	}
	else
	{
		short stancemotion = -1;
		
		if(m_CharacterInfo.modeldescript.item1 != 0)
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					if(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
						WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(activeslotid > 0)
						{
							if(m_CharacterInfo.gender == _XGENDER_MAN)
								stancemotion = g_SkillProperty[activeslotid]->stance_male+2;
							else
								stancemotion = g_SkillProperty[activeslotid]->stance_female+2;
						}
						else
							stancemotion = _XAN_BATTLEIDLE_SWORD_END;
					}
				}
			}
#else
			if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON)
			{
				if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
					g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
				{
					if(activeslotid > 0)
					{
						if(m_CharacterInfo.gender == _XGENDER_MAN)
							stancemotion = g_SkillProperty[activeslotid]->stance_male+2;
						else
							stancemotion = g_SkillProperty[activeslotid]->stance_female+2;
					}
					else
						stancemotion = _XAN_BATTLEIDLE_SWORD_END;
				}
			}
			else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2)
			{
				if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
					g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
				{
					if(activeslotid > 0)
					{
						if(m_CharacterInfo.gender == _XGENDER_MAN)
							stancemotion = g_SkillProperty[activeslotid]->stance_male+2;
						else
							stancemotion = g_SkillProperty[activeslotid]->stance_female+2;
					}
					else
						stancemotion = _XAN_BATTLEIDLE_SWORD_END;
				}
			}
			else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3)
			{
				if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
					g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
				{
					if(activeslotid > 0)
					{
						if(m_CharacterInfo.gender == _XGENDER_MAN)
							stancemotion = g_SkillProperty[activeslotid]->stance_male+2;
						else
							stancemotion = g_SkillProperty[activeslotid]->stance_female+2;
					}
					else
						stancemotion = _XAN_BATTLEIDLE_SWORD_END;
				}
			}
#endif
		}
		
		return stancemotion;
	}
}

short _XLocalUser::GetMotionIndex(short selectedskillid)
{
	short motionindex = -1;
	if(m_CharacterInfo.gender == _XGENDER_MAN)
		motionindex = g_SkillProperty[selectedskillid]->motion_male;
	else 
		motionindex = g_SkillProperty[selectedskillid]->motion_female;

	return motionindex;	
}

char _XLocalUser::GetUsefulType(char selectedskillid)
{
	return g_SkillProperty[m_SelectedSkillID]->usefulType;
}

BOOL _XLocalUser::ProcessLPSSystem(_XMob* pMob)
{
	switch( g_MobAttribTable[pMob->m_MobType].scripttype ) {
	case 81:// Normal LPS
		{
			if(GetMotionClass() == _XACTION_LPS)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
				
					short tempselectedskillid = g_pLocalUser->m_SelectedSkillID;
					g_pLocalUser->m_SelectedSkillID = 0;
					g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE);
					
					g_pLocalUser->m_SelectedSkillID = tempselectedskillid;
					
					
					if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
					{
						if(pMob == (_XMob*)g_pLocalUser->m_AttackTarget)
							g_pLocalUser->ResetAttackTarget();
					}
					
					m_bUsingSkill = FALSE;
					m_bCheckDelayTime = TRUE;
					m_CurrentBattleState = _XUSER_STATE_LAST;
					// delay time ��� ����
					m_ChangeDelayStartTime = g_LocalSystemTime;
					
					pMob->SetAnimation(_XMOBACTION_DIE);
					SetAnimation(_XACTION_IDLE);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);				
				}
			}
			else
			{
				
				SetAnimation(_XACTION_LPS);
				m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position );
				UpdatePosition();
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				
				if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
				{
					SetAttackTarget((_XItem*)pMob, _XPICKINGTARGET_MOB);
				}
				else
				{
					m_CurrentBattleState = _XUSER_STATE_1;
				}
			}
			return TRUE;
		}
		break;
	case 82:// Key LPS
		{
			if(GetMotionClass() == _XACTION_LPS)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					// ������ üũ �ϱ� 
					bool bisitem = false;
					int	 inventorynumber = -1;
					for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == g_MobAttribTable[pMob->m_MobType].key_type )
						{
							if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == g_MobAttribTable[pMob->m_MobType].key_id )
							{
								inventorynumber = i;
								bisitem = true;
								break;
							}
						}
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��  
					if( !bisitem )
					{
						for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
						{
							if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == g_MobAttribTable[pMob->m_MobType].key_type )
							{
								if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == g_MobAttribTable[pMob->m_MobType].key_id )
								{
									inventorynumber = i;
									bisitem = true;
									break;
								}
							}
						}
					}
				#endif

					if( bisitem )
					{
						// ��Ŷ������ 
						m_bKeyLPSProcess = TRUE;
						g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, inventorynumber, 1, pMob->m_MobID );
						// ���ʹ� �������.
						pMob->SetAnimation(_XMOBACTION_DIE);
						pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);	
						
					} 
					else
					{												
						TCHAR messagestring[512];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_2326), 
							_XGameItem::GetItemName(g_MobAttribTable[pMob->m_MobType].key_type, 
							g_MobAttribTable[pMob->m_MobType].key_id));
						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
						m_bLeftClickProcess = TRUE;
					}
					
					if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
					{
						if(pMob == (_XMob*)g_pLocalUser->m_AttackTarget)
							g_pLocalUser->ResetAttackTarget();
					}
					
					m_bUsingSkill = FALSE;
					m_bCheckDelayTime = TRUE;
					m_CurrentBattleState = _XUSER_STATE_LAST;
					// delay time ��� ����
					m_ChangeDelayStartTime = g_LocalSystemTime;
					
					SetAnimation(_XACTION_IDLE);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				}
			}
			else
			{
				// ���� ���� �ൿ
				SetAnimation(_XACTION_LPS);
			}
			return TRUE;		
		}
		break;
	case 83:// Transform LPS
		{
			//short tempselectedskillid = g_pLocalUser->m_SelectedSkillID;
			//g_pLocalUser->m_SelectedSkillID = 0;
			//g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE);
			//SetAnimation(_XACTION_IDLE);
			//g_pLocalUser->m_SelectedSkillID = tempselectedskillid;
			return FALSE;
		}
		break;
	case 84:// HP LPS
		{	
			// �̺�Ʈ ������ �ӽ÷� ó���� �ڵ� : ������ ������ ��.
			bool biscondition = true;
			if( pMob->m_MobType == 1055 )
			{
				biscondition = false;
				if( g_pState_Window->FindPlayerStateList(_XST_EFFECT, 380))
					biscondition = true;
				
				if( !biscondition )
					if( g_pState_Window->FindBuffStateList(_XST_EFFECT, 380))
						biscondition = true;
					
				if( !biscondition )
					if( g_pState_Window->FindHostileStateList(_XST_EFFECT, 380))
						biscondition = true;
			}
			
			if( biscondition )
			{
				return FALSE;

			}
			else
			{
			
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT( ID_STRING_NEW_2905 ), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//_T("�� ���屺�� �Ⱑ �����մϴ�.")
				SetAnimation(_XACTION_IDLE);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				
				return TRUE;
			}
			//short tempselectedskillid = g_pLocalUser->m_SelectedSkillID;
			//g_pLocalUser->m_SelectedSkillID = 0;
			//g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE);
			//SetAnimation(_XACTION_IDLE);
			//g_pLocalUser->m_SelectedSkillID = tempselectedskillid;
			return FALSE;
		}
		break;
	case 85:// Optional LPS
		{
			/*
			short tempselectedskillid = g_pLocalUser->m_SelectedSkillID;
			g_pLocalUser->m_SelectedSkillID = 0;
			g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE);
			SetAnimation(_XACTION_IDLE);
			g_pLocalUser->m_SelectedSkillID = tempselectedskillid;
			
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;
			m_CurrentBattleState = _XUSER_STATE_LAST;
			// delay time ��� ����
			m_ChangeDelayStartTime = g_LocalSystemTime;*/
			return TRUE;
		}
		break;
	}
	
	return FALSE;
}

BOOL _XLocalUser::CheckTargetTypeFinal(void)
{
	if(m_SelectedSkillID <= 0)		// active skill������ ������� �����Ƿ� 0�� false�� ó��
		return FALSE;

	switch(_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/)
	{
	case _XSI_TARGETTYPE_SELF :
		{
			return TRUE;
		}
		break;
	case _XSI_TARGETTYPE_OTHER_ENEMY :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					return TRUE;
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					_XFunctionalObject* pObj = (_XFunctionalObject*)m_AttackTarget;
					if(pObj)
					{
						if(pObj->m_bAttackable)
							return TRUE;
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_AttackTarget;
					if((m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && m_MatchTargetID == pUser->m_Uniqueid) ||
						(m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) ||
						(m_UserState == _XDEF_USERSTATE_PK_FIGHT && CheckPKTarget(pUser)))
					{
						if(pUser->GetMotionClass() == _XACTION_DYING || pUser->GetMotionClass() == _XACTION_DIE)
							return FALSE;

						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_OTHER_FRIEND :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_AttackTarget;
					if((m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && m_MatchTargetID == pUser->m_Uniqueid) ||
						(m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) || 
						(m_UserState == _XDEF_USERSTATE_PK_FIGHT && CheckPKTarget(pUser)))
					{
						return FALSE;
					}
					else
					{
						return TRUE;
					}
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_SELFOTHER_FRIEND :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_SELF)
				{
					return TRUE;
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_AttackTarget;
					if((m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && m_MatchTargetID == pUser->m_Uniqueid) ||
						(m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) || 
						(m_UserState == _XDEF_USERSTATE_PK_FIGHT && CheckPKTarget(pUser)))
					{
						return FALSE;
					}
					else
					{
						return TRUE;
					}
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_SELFOTHER_ENEMY :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_SELF || m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					return TRUE;
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					_XFunctionalObject* pObj = (_XFunctionalObject*)m_AttackTarget;
					if(pObj)
					{
						if(pObj->m_bAttackable)
							return TRUE;
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_AttackTarget;
					if((m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && m_MatchTargetID == pUser->m_Uniqueid) ||
						(m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) ||
						(m_UserState == _XDEF_USERSTATE_PK_FIGHT && CheckPKTarget(pUser)))
					{
						if(pUser->GetMotionClass() == _XACTION_DYING || pUser->GetMotionClass() == _XACTION_DIE)
							return FALSE;

						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_OTHER_ALL :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_PC || m_AttackTargetObjectType == _XPICKINGTARGET_MOB || m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					return TRUE;
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_ALL :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_SELF || m_AttackTargetObjectType == _XPICKINGTARGET_PC ||
					m_AttackTargetObjectType == _XPICKINGTARGET_MOB || m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					return TRUE;
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_POINT :
		{
			return TRUE;
		}
		break;
#ifdef _XTS_SKILLCOMATYPE
	case _XSI_TARGETTYPE_COMA :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_AttackTarget;
					if(pUser)
					{
						if(pUser->GetMotionClass() == _XACTION_DIE)
						{
							return TRUE;
						}
						else
							return FALSE;
					}
				}
			}
		}
		break;
#endif
	}
	
	return FALSE;
}

void _XLocalUser::SetMonsterWarningMessage(unsigned short mobtype)
{
	if( g_MobAttribTable[mobtype].explevel != 1 )
	{
		int difflevel = g_MobAttribTable[mobtype].level - g_pLocalUser->m_CharacterInfo.Get_innerlevel();

		if(difflevel < -4)
		{
			// LPS�� �޼����� ������� �ʴ´� 
			if( g_MobAttribTable[mobtype].scripttype < 81 || g_MobAttribTable[mobtype].scripttype > 85)
#ifdef _XDEF_MONSTERWARNING_MESSAGE_20070102
            {
                //Author : ����� //�̱� ��� �޽��� �ȳ�����..
                if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH && (mobtype == 1931 || mobtype == 1932) )
                {
                }
                else
                    g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_2327), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
            }
#else
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_2327), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE, D3DCOLOR_ARGB( 255, 250, 230, 170 ));
#endif

		}
	}

	int droplevelcount = 0;
	for( int i = 0; i < 8; ++i )
	{
		droplevelcount += g_MobAttribTable[mobtype].droplevel[i];
	}
	if( droplevelcount <= 4 )
	{
		int difflevel = g_MobAttribTable[mobtype].level - g_pLocalUser->m_CharacterInfo.Get_innerlevel();

		if(difflevel < -4)
		{
			// LPS�� �޼����� ������� �ʴ´� 
			if( g_MobAttribTable[mobtype].scripttype < 81 || g_MobAttribTable[mobtype].scripttype > 85)
#ifdef _XDEF_MONSTERWARNING_MESSAGE_20070102
			{
                //Author : ����� //�̱� ��� �޽��� �ȳ�����..
                if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH && (mobtype == 1931 || mobtype == 1932) )
                {
                }
                else
                    g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_2327), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
            }
#else
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_2328), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE, D3DCOLOR_ARGB( 255, 250, 230, 170 ));
#endif

		}
	}
}

void _XLocalUser::DeleteWeaponModel(short skillid)
{
	// ��� ���� type�̸� ��� �ִ� ���� ����
	if(g_SkillProperty[skillid]->weaponCType == -1 && g_SkillProperty[skillid]->weaponSType == 0)
	{
		int effectid = _XGameItem::GetInchantEffectId( m_CharacterInfo.modeldescript.item1Category,
						m_CharacterInfo.modeldescript.item1, m_CharacterInfo.weaponStrength );
		
		if( effectid > 0 )
		{
			m_ModelDescriptor.m_EffectManager->DeleteInstance(effectid);
		}
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
#ifdef _XDEF_LOSTWEAPON_070821_MAGVIPER //Author : ����� //breif ���� ������� ����
			if( WeaponItem && m_CurrentWeaponID == 0)
#else
			if( WeaponItem )
#endif
			{
				if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
					m_CharacterInfo.modeldescript.item1 = 0;
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
					m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
				}
			}
		}
#else
		if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
		{
			if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
				m_CharacterInfo.modeldescript.item1 = 0;
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
				m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
		{
			if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
				m_CharacterInfo.modeldescript.item1 = 0;
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
				m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
		{
			if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
				m_CharacterInfo.modeldescript.item1 = 0;
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
				m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
			}
		}
#endif
	}
}

void _XLocalUser::DeleteWeaponModel(void)
{
	int effectid = _XGameItem::GetInchantEffectId( m_CharacterInfo.modeldescript.item1Category,
					m_CharacterInfo.modeldescript.item1, m_CharacterInfo.weaponStrength );
	
	if( effectid > 0 )
	{
		m_ModelDescriptor.m_EffectManager->DeleteInstance(effectid);
	}
	// ��� �ִ� ���� ����
#ifdef _XTS_ITEM_OPTIMIZATION
	if(_XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
	{
		_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
#ifdef _XDEF_LOSTWEAPON_070821_MAGVIPER //Author : ����� //breif ���� ������� ����
		if( WeaponItem && m_CurrentWeaponID == 0)
#else
		if( WeaponItem )
#endif
		{
			if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
				m_CharacterInfo.modeldescript.item1 = 0;
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
				m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
			}
		}
	}
#else
	if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
	{
		if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
		{
			m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
			m_CharacterInfo.modeldescript.item1 = 0;
			m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
			m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
		}
	}
	else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
	{
		if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
		{
			m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
			m_CharacterInfo.modeldescript.item1 = 0;
			m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
			m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
		}
	}
	else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
	{
		if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
		{
			m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
			m_CharacterInfo.modeldescript.item1 = 0;
			m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
			m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
		}
	}
#endif
}

void _XLocalUser::ReloadWeaponModel(void)
{
	// ��� �ִ� ���Ⱑ ���� ��� �������ֱ�
	if(m_CurrentWeaponID > 0)
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
			if( WeaponItem )
			{
				if(WeaponItem[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;

#ifdef _XDEF_MODELTHREADLOADING
					if( WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
					}
#else
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
					m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
					m_CurrentWeaponID = 0;
				}
			}
		}
#else
		if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
		{
			if(g_WeaponItemProperty[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;

#ifdef _XDEF_MODELTHREADLOADING
				if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
												   g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex] );
				}
				else
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
												   
				}
#else
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
					g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
				m_CurrentWeaponID = 0;
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
		{
			if(g_WeaponItemProperty2[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;
#ifdef _XDEF_MODELTHREADLOADING
				if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex] );
				}
				else
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
					
				}

#else
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
					g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
				m_CurrentWeaponID = 0;
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
		{
			if(g_WeaponItemProperty3[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;

#ifdef _XDEF_MODELTHREADLOADING
				if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex] );
				}
				else
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
				}
#else
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
					g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
				m_CurrentWeaponID = 0;
			}
		}
#endif
		int effectid = _XGameItem::GetInchantEffectId( m_CharacterInfo.modeldescript.item1Category,
						m_CharacterInfo.modeldescript.item1, m_CharacterInfo.weaponStrength );

		if( effectid > 0 )
		{
			if( !m_ModelDescriptor.m_EffectManager->FindInstance(effectid) )
			{
				m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
					effectid, &m_ModelDescriptor.m_Position, NULL, TRUE );
			}			
		}
	}
}

short _XLocalUser::GetInterpolateMotionIndex(short skillid)
{
	short motionindex = -1;
	if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_FIRST)
	{
		if(m_CharacterInfo.gender == _XGENDER_MAN)
		{
			if(skillid > 0)
				motionindex = g_SkillProperty[skillid]->intermotion1_male;
		}
		else
		{
			if(skillid > 0)
				motionindex = g_SkillProperty[skillid]->intermotion1_female;
		}
	}
	else if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_SECOND)
	{
		if(m_CharacterInfo.gender == _XGENDER_MAN)
		{
			if(skillid > 0)
				motionindex = g_SkillProperty[skillid]->intermotion2_male;
		}
		else
		{
			if(skillid > 0)
				motionindex = g_SkillProperty[skillid]->intermotion2_female;
		}
	}

	return motionindex;
}

bool ProcessXZCollision(const D3DXVECTOR3& contactpoint, D3DXVECTOR3& movevelocity, D3DXVECTOR3& nextposition)
{
	// Collision plane
	D3DXVECTOR3 N = contactpoint;
	
	// move the object
	nextposition += N;
	
	float nv = D3DXVec3Dot( &N, &movevelocity );    // impact velocity	
	if (nv > 0.0f)									// don't reflect
		return false;
	
    float n2 = D3DXVec3Dot( &N, &N );				// the normal of collision length squared
	
	if (n2 < 0.00001f)								// to small, can't be of any use
		return false;
	
	static float fElasticity = 2.8f;				// coefficient of elasticity
	static float fFriction   = 0.005f;				// coefficient of friciton
	//static float fElasticity = 1.4f;				// coefficient of elasticity
	//static float fFriction   = 0.010f;				// coefficient of friciton
	
	// collision response. calcualte velocity...
	D3DXVECTOR3 Vn = N * (nv / n2);					// relative velocity alon the normal of collision
	D3DXVECTOR3 Vt = movevelocity - Vn;				// tangencial velocity
	
	// V = -Vn . (1.0f + CoR) + Vt . CoF
	movevelocity -= (1.0f + fElasticity) * Vn + Vt * fFriction;	// reflect
	
	return true;
}

void _XLocalUser::CheckCollideStatusInIdle( void )
{
	if( g_FreeMoveMode ) return;

	if( !m_pCollideOBB ) return;
	if( gpInput->GetMouseState()->bButton[0] ) return;
	if( gpInput->CheckKeyPress(DIK_LMENU) ) return;
	
	POINT pt;
	_XLODTILE*	pTerrainTile = g_LodTerrain.GetTerrainTile( m_Position , &pt );	
	if( !pTerrainTile )
	{
		return;
	}
	
	D3DXVECTOR3 directionvector = m_pCollideOBB->m_kCenter - m_Position;
	D3DXVec3Normalize( &directionvector, &directionvector );

	m_CharacterOBB.m_kCenter.x = m_Position.x;
	m_CharacterOBB.m_kCenter.y = m_Position.y + 0.89f;
	m_CharacterOBB.m_kCenter.z = m_Position.z;
	
	m_pGoundCollideOBB = NULL;
	m_pCollideOBB = NULL;
	D3DXVECTOR3 contactpoint;
	
	// Check collision
	if( !pTerrainTile->svObbList.empty() )
	{
		_XOBBData* pObbData;
		svdef_OBBData::iterator it;
		for (it = pTerrainTile->svObbList.begin(); it != pTerrainTile->svObbList.end(); ++ it)
		{
			pObbData = (*it);
			if( m_CharacterOBB.IntersectOBBvsOBB( *pObbData, contactpoint ) )
			{
				if( pObbData->m_OBBLevel == 1 )
					m_pCollideOBB = pObbData;
				
				D3DXVECTOR3 prevPosition = m_Position;
				ProcessXZCollision(contactpoint, directionvector, m_Position );

				if( prevPosition.x == m_Position.x && prevPosition.y != m_Position.y && prevPosition.z == m_Position.z )
				{
					m_Position = prevPosition + (directionvector * -0.03f);
				}

				m_CharacterOBB.m_kCenter.x = m_Position.x;
				m_CharacterOBB.m_kCenter.y = m_Position.y + 0.89f;
				m_CharacterOBB.m_kCenter.z = m_Position.z;
				
				m_FinalTargetPosition = m_TargetPosition = m_Position;
				
				UpdatePosition();
				
#ifdef _XDWDEBUG
				m_CharacterOBB.CreateOBBModel();
				pObbData->m_Collide	= TRUE;
#endif
			}
#ifdef _XDWDEBUG
			else pObbData->m_Collide = FALSE;
#endif
		}
	}

	if( g_LodTerrain.m_FunctionalObjectManager.m_countFunctionalObject != 0 )
	{
		for( int i = 0; i < g_LodTerrain.m_FunctionalObjectManager.m_countFunctionalObject; ++i )
		{
			if( g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_bCollidable )// �浹 ������ ���� �浹 ó���� �Ѵ�. ( ���� �μ����� �浹ó���� ���� �ʴ´�.)
			{

				if( m_CharacterOBB.IntersectOBBvsOBB( g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i], contactpoint ) )
				{
					D3DXVECTOR3 prevPosition = m_Position;
					ProcessXZCollision(contactpoint, directionvector, m_Position );

					if( prevPosition.x == m_Position.x && prevPosition.y != m_Position.y && prevPosition.z == m_Position.z )
					{
						m_Position = prevPosition + (directionvector * -0.03f);
					}

					m_CharacterOBB.m_kCenter.x = m_Position.x;
					m_CharacterOBB.m_kCenter.y = m_Position.y + 0.89f;
					m_CharacterOBB.m_kCenter.z = m_Position.z;
					
					m_FinalTargetPosition = m_TargetPosition = m_Position;
					
					UpdatePosition();
#ifdef _XDWDEBUG
					g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_Collide = TRUE;
#endif				
				}
			}
#ifdef _XDWDEBUG
			else g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_Collide = FALSE;
#endif					
		}
	}	
}

void _XLocalUser::SetDefaultValueMatchTable()
{
	m_myMatchTableInfo.usWaitTime = 0;
	m_myMatchTableInfo.cRecordType = 0;
	m_myMatchTableInfo.cMatchType = 1;
	m_myMatchTableInfo.cMatchTime = 2;
	m_myMatchTableInfo.cUseHyperrun = 0;
	m_myMatchTableInfo.cUseBuff = 0;
	m_myMatchTableInfo.cMatchBalance = 50;

	m_myMatchTableInfo.myBetMoney = 0;
	m_myMatchTableInfo.targetBetMoney = 0;

	for(int i = 0 ; i < 6 ; ++i)
	{
		m_myMatchTableInfo.myBetItem[i].Reset();
		m_myMatchTableInfo.targetBetItem[i].Reset();
	}
}

void _XLocalUser::ResetLock(BOOL bCondition)
{
	if(bCondition)
	{
		// condition lock�� reset
		m_Lock.movelock			= 0;
		m_Lock.tradelock		= 0;
		m_Lock.battlelock		= 0;
		m_Lock.skilllock		= 0;
		m_Lock.medlock			= 0;
		m_Lock.spelllock		= 0;
		m_Lock.speedlock		= 0;
		m_Lock.chatlock			= 0;
		m_Lock.modelock			= 0;
		m_Lock.partylock		= 0;
		m_Lock.camerarotatelock	= 0;
		m_Lock.camerazoomlock	= 0;
		m_Lock.camerareverse	= 0;
		m_Lock.adddelaytime		= 0;
		m_Lock.notdrawusername	= 0;
	}
	else
	{
		// ��ü reset
		memset(&m_Lock, 0, sizeof(m_Lock));
	}
}

void _XLocalUser::SetOut(BOOL state)
{
	if(state)
	{
		// �ڸ���� setting
		if(!m_bOut)
		{
			// ��� or ���� ���� �϶�
			if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
			{
				return;
			}
			// ���� ���� ���� ��
			if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
			{
				return;
			}
			// �� ȫ����
			if(g_pLocalUser->m_bMatchPRMode)
			{
				return;
			}

			m_bOut = state;
			
			// send packet
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_to_absent_state);

			// �ɱ�� ����
			g_pLocalUser->SetAnimationIndex(g_EmotionActionTable[4]);
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
	}
	else
	{
		// �ڸ���� ���
		if(m_bOut)
		{
			m_bOut = state;

			// send packet
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_to_normal_state);

			// idle motion���� ����
			if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE && GetMotionClass() != _XACTION_REBIRTH)
			{
				g_pLocalUser->SetAnimation(_XACTION_IDLE);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
	}
}

//Author : �����
const DWORD _XLocalUser::GetPCroomState( const _XUSER_PCROOM_STATE PCroomState ) const
{
	switch(PCroomState)
	{
	case _XDEF_PCROOM_ERROR :
		return m_dwPCroomInfo.PCROOM_State.dwError;
		break;
	case _XDEF_PCROOM_INTERNET:
		return m_dwPCroomInfo.PCROOM_State.dwInternet;
		break;
	case _XDEF_PCROOM_RATIOCAFE :
		return m_dwPCroomInfo.PCROOM_State.dwRatio;
		break;
	case  _XDEF_PCROOM_PREPAID:
		return m_dwPCroomInfo.PCROOM_State.dwPrepaid;
		break;
	case _XDEF_PCROOM_PREMIUM:
		return m_dwPCroomInfo.PCROOM_State.dwPremium;
		break;
	case _XDEF_PCROOM_NONE :
		return m_dwPCroomInfo.PCROOM_State.dwNone;
		break;
	default :
		return 0;
		break;
	}
}

void _XLocalUser::SetPCroomState( const _XUSER_PCROOM_STATE PCroomState, const DWORD dwResult )
{
	switch(PCroomState)
	{
	case _XDEF_PCROOM_ERROR :
		m_dwPCroomInfo.PCROOM_State.dwError = dwResult;
		break;
	case _XDEF_PCROOM_INTERNET:
		m_dwPCroomInfo.PCROOM_State.dwInternet = dwResult;
		break;
	case _XDEF_PCROOM_RATIOCAFE :
		m_dwPCroomInfo.PCROOM_State.dwRatio = dwResult;
		break;
	case _XDEF_PCROOM_PREPAID:
		m_dwPCroomInfo.PCROOM_State.dwPrepaid = dwResult;
		break;
	case _XDEF_PCROOM_PREMIUM:
		m_dwPCroomInfo.PCROOM_State.dwPremium = dwResult;
		break;
	case _XDEF_PCROOM_NONE :
		m_dwPCroomInfo.PCROOM_State.dwNone = dwResult;
		break;
	default :
		break;
	}
}

//Last Updated : 07/03/15

BOOL _XLocalUser::GetEnterMatchEvent()
{
	if(g_MatchEventManager.GetCurrentState() == 2	// start ����
		&& this->m_bEnterMatchEvent == TRUE			// ������
		&& g_AreaManager.FindPositionInArea(this->m_Position, g_MatchEventManager.GetCurrentEventArea(), _XDEF_AREACATEGORY_PVPAREA))//���� �ȿ� ����
	{
		return TRUE;
	}

	return FALSE;
}

BOOL _XLocalUser::GetEnterCastleBattle()
{
	//Author : ����� //breif : ����� 
	int areanumber = 0;
	if( m_bCastleBattle && (g_AreaManager.CheckInsideArea(g_pLocalUser->m_Position, _XDEF_AREACATEGORY_CASTLE, areanumber) ||
		g_AreaManager.CheckInsideArea(g_pLocalUser->m_Position, _XDEF_AREACATEGORY_PUBLICPVPAREA, areanumber)) )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL _XLocalUser::GetEnterMatchReadyArea()
{
	if(g_MatchEventManager.GetCurrentState() == 2	// start ����
		&& this->m_bEnterMatchEvent == TRUE			// ������
		&& g_AreaManager.FindPositionInArea(this->m_Position, 94, _XDEF_AREACATEGORY_PLAYERSPAWN))//���� �� ��Ȱ ���� �ȿ� ����
	{
		return TRUE;
	}

	return FALSE;
}

void _XLocalUser::GetBWLeftTime(int& hour, int& min, int& sec)
{
	if(m_dwBWLeftTimeFromServer == 0)
	{
		// ���� ����
		hour = 0;
		min = 0;
		sec = 0;
	}
	else
	{
		DWORD elapsedtime = g_LocalSystemTime - m_dwBWReceiveTimeFromServer;

		if(m_dwBWLeftTimeFromServer >= elapsedtime)
		{
			DWORD lefttime = m_dwBWLeftTimeFromServer - elapsedtime;

			min = (lefttime / 1000) / 60;
			sec = (lefttime / 1000) % 60;

			hour = min / 60;
			min = min % 60;
		}
		else
		{
			// �ð� �ʰ�
			hour = 0;
			min = 0;
			sec = 0;
		}
	}
}

void _XLocalUser::DrawBWLeftTime()
{
	int hour = 0, min = 0, sec = 0;
	GetBWLeftTime(hour, min, sec);
	
	if(hour == 0 && min == 0 && sec == 0)
		return;

	g_XLargeFont->SetColor(0xFFF5E88A);
	g_XLargeFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);

	int y = (gnHeight>>1) - 145 -70;

#ifdef _XTS_PKZONE_SERVERSETTING
	if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkserverinfo)
#else
	if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkclientinfo)
#endif
	{
		DWORD elapsedtime = g_LocalSystemTime - ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_dwMapEnteringTime;
		if(elapsedtime <= 30000)
		{
			y = (gnHeight>>1) - 145 -30;
		}
	}

	g_XLargeFont->PutsAlign((gnWidth>>1), y, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3805));	// �пյ� ���� ���� �ð�
	
	if(g_LanguageType == _XLANGUAGE_TYPE_KOREAN)
		g_XLargeFont->PrintAlign((gnWidth>>1), y+20, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%02d;%02d;%02d", hour, min, sec);
	else
		g_XLargeFont->PrintAlign((gnWidth>>1), y+20, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%02d:%02d:%02d", hour, min, sec);

	g_XLargeFont->Flush();
	g_XLargeFont->DisableGrowMode();
}
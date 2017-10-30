#ifndef _EODEXTDEF_GLOBALOBJECT
#define _EODEXTDEF_GLOBALOBJECT

#include "XSTWrapper.h"
#include "XSTWrapperCore.h"

#ifdef _XUSEFMOD
	#include "XFMODWrapper.h"
#else
	#include "XAudioSystem.h"
	using namespace Audio;
#endif


#include "XKernel.h"
#include "XRegistryDef.h"
#include "XLodTerrain.h"
#include "XLocalUser.h"
#include "XHeroLocalUser.h"	// hotblood zerolevel
#include "XMob.h"
#include "XNpc.h"
#include "XGameItem.h"
#include "XSkillItem.h"
#include "XBipedController.h"
#include "XAnimMeshContainer.h"
#include "XMeshContainer.h"
#include "XNetwork.h"
#include "XMessageBox.h"
#include "XWindow_NPCName.h"
#include "XProc_MainGameDef.h"
#include "EODEXTDEF_ModelData.h"
#include "XQuestScriptManager.h"
#include "XAreaManager.h"
#include "XWordFilteringManager.h"
#include "XWindow_ChatBalloon.h"
#include "XStringResourceManager.h"
#include "XParticleList.h"
#include "XScreenMotionBlur.h"
#include "XPostProcessCore.h"
#include "XPathFinder.h"
#include "XDummy.h"
#include "XSeat.h"
#include "XCollectionManager.h"
#include "XResourceManager.h"

#include "XParticleEffectManager.h"
#include "XWindow_Tutorial.h"	// 2005.03.03->hotblood

#include "XHTMLWrapper.h"
#include "XMADWrapper.h"
#include "XWindow_NPCBuff.h"
#include "XFame.h"
#include "XBattleLog.h"
#include "XCastleManager.h"
#include "XMatchEventManager.h"
#include "XSetItemInfomation.h" //Author : �����
#include "XSkillMastery.h"

#ifdef _XTESTSERVER	
#define _XDEF_MODELARCHIVENAME						"Model\\tsMR_BASEMODEL.XP"
#define _XDEF_ANIARCHIVENAME						"Model\\tsAR_BASEANIMATION.XP"
#else
#define _XDEF_MODELARCHIVENAME						"Model\\MR_BASEMODEL.XP"
#define _XDEF_ANIARCHIVENAME						"Model\\AR_BASEANIMATION.XP"
#endif

#define _XDEF_MAXSOUNDEFFECTCOUNT				    500
#define _XDEF_MAXINTERFACESOUND					    100
#define _XDEF_MAXENVIRONMENTSOUND					100
#define _XDEF_LIMITSOUNDPLAY_DISTANCE				5.0f
#define _XDEF_MAXBGMCOUNT							20
#define _XDEF_MAXBGMCHAR							256

#define _XDEF_TOTALLEVELNAME						21
#define _XDEF_SCREENFADE_LEVEL						5

#define _XDEF_NPCDIALOGMODE_FADE_LEVEL				20

//Word filtering
extern _XWordFilteringManager	g_WordFilteringManager;

//#define _XDEF_USEHARDWARECURSOR	// �ϵ���� Ŀ���� �Ѱ� ����.
extern BOOL							g_UseHardwareCursor;

extern int							g_MaxSoundEffectCount;				
extern int							g_MaxInterfaceSoundEffectCount;	
extern int							g_MaxEnvSoundEffectCount;

extern WORD							g_CriticalEffectSoundIndex[2];

extern WORD							g_DefenceEffectSoundIndex[2][3];
extern WORD							g_BlowEffectSoundIndex[2][3];
extern WORD							g_DieEffectSoundIndex[2][2];
extern WORD							g_WalkEffectSoundIndex[2];

//=================================================================================
// Texture mip level	
extern int							g_TextureMipLevel_TerrainDiffuse;
extern int							g_TextureMipLevel_TerrainDetail;
extern int							g_TextureMipLevel_Water;
extern int							g_TextureMipLevel_Object;
extern int							g_TextureMipLevel_CharacterModel;

//=================================================================================
// Graphic resource
extern _XPackageArchive				g_ModelArchive;
extern _XPackageArchive				g_ThreadModelArchive;
extern _XPackageArchive				g_AniArchive;
extern _XPackageArchive				g_ScriptArchive;

extern _XTextureManager				g_TerrainTextureArchive;
extern _XTextureManager				g_WorldObjectTextureArchive;
extern _XTextureManager				g_RTimeObjectTextureArchive;
extern _XTextureManager				g_DummyObjectTextureArchive;
extern _XTextureManager				g_EffectTextureArchive;
extern _XTextureManager				g_MainInterfaceTextureArchive;
extern _XTextureManager				g_SpeedTreeTextureArchive;
//=================================================================================
// Cursor

extern HRESULT						_XSetDeviceCursor( POINT hotspot, _XBitmap* bmp );

extern HCURSOR						g_hGameCursor[_XMOUSETYPE_TOTALCOUNT];
extern _XBitmap*					g_MouseImage[_XMOUSETYPE_TOTALCOUNT];
extern _XMOUSETYPE					g_ReadyMouseCursorMode;
extern int							g_MousePointerPriorityTable[_XMOUSETYPE_TOTALCOUNT];
extern POINT						g_MousePointerHotspotCoord[_XMOUSETYPE_TOTALCOUNT];
extern _XMOUSETYPE					g_CurrentMouseMode;
extern _XMOUSETYPE					g_PrevMouseMode;

extern sDate						g_ServerGameTime;
extern int							g_ServerTimeCode;
extern DOUBLE						g_fServerTimeCode;
extern DWORD						g_LocalServerTimeCode;

extern int							g_LastRequestCharacterNameID;

//=================================================================================
// Effect manager
extern _XParticleEffectManager			g_ParticleEffectManager;
extern _XParticleGroupManager			g_CharacterEffectGroupManager;
extern _XParticleGroupManager			g_WorldEffectGroupManager;
extern _XParticleGroupInstanceManager	g_WorldEffectInstanceManager;
extern _XParticleGroupInstanceManager	g_PostProcessWorldEffectInstanceManager;
extern _XEffect_SwordTrace				g_SwordEffectController;

#define _XDEF_USESCREEENMOTIONBLUR 

#ifdef _XDEF_USESCREEENMOTIONBLUR 
extern _XScreenMotionBlur			g_ScreenMotionBlurEffectManager;
extern BOOL							g_ScrrenMotionBlurMode;
#endif

extern _XPostProcessCore			g_PostProcessManager;
extern _XScreenBlurCore				g_ObjectBlurProcessManager;

//=================================================================================
// Picking terrain
extern BOOL							g_MouseFirstClickPassed;
extern D3DXVECTOR3					g_vPickRayDir;
extern D3DXVECTOR3					g_vPickInfinityRayDir;
extern D3DXVECTOR3					g_vPickRayOrig;
extern FLOAT						g_fBary1;
extern FLOAT						g_fBary2;
extern FLOAT						g_fDist;

extern D3DXVECTOR3					g_vMousePosition;
extern D3DXMATRIX					g_IdentMatrix;

//=================================================================================
// Sound effect & bgm
#ifdef _XUSEFMOD	
	
#else
	extern _XAudioSystem				g_XAudioSystem;
	extern ISound*						g_pBGMSoundObject[3];
	extern _XAudioPackageArchive		g_SoundEffectArchive;
	extern _XAudioPackageArchive		g_EnvSoundEffectArchive;
	extern _XAudioPackageArchive		g_InterfaceSoundArchive;
	extern ISound3D*					g_pSoundEffect[ _XDEF_MAXSOUNDEFFECTCOUNT ];
	extern ISound**						g_pInterfaceSound;		
	extern _XAudioPackageArchive 		g_NPCSoundArchive;
	extern ISound3D*			 		g_NPCSoundObject;
#endif

extern FLOAT						g_pBGMSoundMaxVolume;
extern IEnvSound3D					g_pEnvironmentSound[_XDEF_MAXENVIRONMENTSOUND];
extern TCHAR						g_BgmList[_XDEF_MAXBGMCOUNT][_XDEF_MAXBGMCHAR];	
extern int*							g_ItemUseSoundIndexTable;	
extern int							g_ItemUseSoundIndexTableSize;
extern BOOL							g_BgmPlayFlag;
extern BOOL							g_BgmPlayOnce;
extern BOOL							g_InterfaceSoundEffectFlag;
extern BOOL							g_SoundEffectFlag;
extern BOOL							g_3DSoundEffectFlag;

extern int							g_WeaponSoundCount;
extern int*							g_WeaponSoundIndexList;

extern int							g_MobWeaponSoundCount;
extern int*							g_MobWeaponSoundIndexList;

//=================================================================================
// Network
extern _XNetwork					g_NetworkKernel;
extern BOOL							g_SinglePlayMode;
extern BOOL							g_ZeroLevelMode;	

//=================================================================================
// Global object
extern	_XLodTerrain				g_LodTerrain;
extern	_XLocalUser					g_CurrentUser;
extern	_XHeroLocalUser				g_HeroCurrentUser;	// hotblood zerolevel
extern	_XLocalUser*				g_pLocalUser;		// hotblood zerolevel
extern  int							g_CharacternamePointSize;
extern	_XList						g_Chatlist;
extern	_XList						g_SystemMsgList;
extern	_XList						g_CenterSystemMessageList;

extern	_XUserList					g_Userlist;
extern	_XMobList					g_Moblist;
extern	_XNPCObjectManager			g_Npclist;
extern	_XGameItemList				g_GameItemlist;

extern _XDummyList					g_DummyList;
extern _XSeatList					g_SeatList;

extern	_XPICKINGTARGETTYPE			g_LastPickedObjectType;
extern	FLOAT						g_LastPickedObjectDistance;
extern  POINT						g_LastPickedPoint;
extern  _XItem*						g_LastPickedObject;

extern _XQuestScriptManager			g_QuestScriptManager;			//����Ʈ ��ũ��Ʈ
extern _XAreaManager				g_AreaManager;
extern BOOL							g_LMenuButtonClicked;

extern _XCollectionManager			g_CollectionManager;
extern _XResourceManager			g_ResourceManager;

//=================================================================================
// Script

extern BOOL							g_ScriptStarted;
extern _XNPCObject*					g_LastNPCScriptOwner;

#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : ����� //breif : ��Ʈ������
extern XSetItemInfomation*			g_pSetIteminfo;
#endif 

//=================================================================================
// FunctionalObject Script

extern BOOL							g_bFunctionalObjectScriptStarted;
extern _XFunctionalObject*			g_LastFunctionalObject;

//=================================================================================
// Game information table...
//=================================================================================
// Interface String pointer

#define _XTARGET_MAX 51

#ifdef _XDEF_NEWSTRINGMANAGER

extern LPTSTR					g_JoinGroupName[9];
extern LPTSTR					g_LevelName[_XDEF_TOTALLEVELNAME];
extern LPTSTR					g_LevelName_CH[_XDEF_TOTALLEVELNAME];
extern LPTSTR					g_stringExperienceSharingForm[5];
extern LPTSTR					g_stringItemSharingForm[5];

extern LPTSTR					g_MonsterName[_XDEF_MODELCLASSCOUNT_MOB];		// Monster name
#ifdef _XJAPANESE
extern LPTSTR					g_MonsterHiraganaName[_XDEF_MODELCLASSCOUNT_MOB];		// Monster hiragana name
#endif

extern TCHAR					g_NPCName[_XDEF_MODELCLASSCOUNT_NPC][41];		// NPC Name
extern LPTSTR					g_TargetFullName[_XTARGET_MAX];
extern LPTSTR					g_UserStatus[5];
extern LPTSTR					g_SunMoonIconText[4];
extern LPTSTR					g_QuestGroupNameString[30];
extern LPTSTR					g_ChatEmotionString[13];
//#ifdef _XDEF_CASHITEM_SHOUT
extern int						g_ChatFunctionButtonStringID[_XCHATMODE_TOTALCOUNT];
//#else
//extern int						g_ChatFunctionButtonStringID[6];
//#endif
extern LPTSTR					g_pClothesItemClanName[9];
extern LPTSTR					g_pClothesItemSecondName[8];
extern LPTSTR					g_pSkillUsefulTypeName[11];
extern LPTSTR					g_BillingStatusString[4];		// HO:���λ����, PC:�Ǿ�������, SP:ISP�����, PM:�����̾� �����
extern LPTSTR					g_BillingPaymentTypeString[2];	// ���ҹ�� AO:�ĺ�, DO:����, PO:����	
extern LPTSTR					g_BillingMethodString[2];		// ���ݹ�� FM:������, FW:�ָ� �� ����
extern LPTSTR					g_HonorName[9];
extern LPTSTR					g_ZoneExplaneString[9];
extern LPTSTR					g_szServerNameString[_COUNT_MAX_GROUP_];

#else

extern TCHAR					g_JoinGroupName[9][16];
extern TCHAR					g_LevelName[_XDEF_TOTALLEVELNAME][16];
extern TCHAR					g_LevelName_CH[_XDEF_TOTALLEVELNAME][16];
extern TCHAR					g_stringExperienceSharingForm[5][32];
extern TCHAR					g_stringItemSharingForm[5][32];
extern TCHAR					g_MonsterName[_XDEF_MODELCLASSCOUNT_MOB][41];
extern TCHAR					g_NPCName[_XDEF_MODELCLASSCOUNT_NPC][41];
extern TCHAR					g_TargetFullName[_XTARGET_MAX][16];
extern TCHAR					g_UserStatus[5][20];
extern TCHAR					g_SunMoonIconText[4][16];
extern TCHAR					g_QuestGroupNameString[30][16];
extern TCHAR					g_ChatEmotionString[13][32];
extern TCHAR					g_ChatFunctionButtonString[6][16];
extern TCHAR					g_pClothesItemClanName[9][16];
extern TCHAR					g_pClothesItemSecondName[8][16];
extern TCHAR					g_pSkillUsefulTypeName[11][16];
extern TCHAR					g_BillingStatusString[4][16];		// HO:���λ����, PC:�Ǿ�������, SP:ISP�����, PM:�����̾� �����
extern TCHAR					g_BillingPaymentTypeString[2][16];	// ���ҹ�� AO:�ĺ�, DO:����, PO:����	
extern TCHAR					g_BillingMethodString[2][16];		// ���ݹ�� FM:������, FW:�ָ� �� ����
extern TCHAR					g_HonorName[9][16];
extern TCHAR					g_ZoneExplaneString[9][256]; 
extern TCHAR					g_szServerNameString[_COUNT_MAX_GROUP_][32];

#endif

extern _XNPCMotionExistTable		g_NPCMotionExistTable[_XDEF_MODELCLASSCOUNT_NPC];
extern  D3DCOLOR					g_HonorColor[9];


extern  BOOL						g_FreeMoveMode;
extern  BOOL						g_FreeMoveMode_CursorView;
extern	BOOL						g_EnvTimeFlowStop;
extern  BOOL						g_InnerUser;

//=================================================================================
// Zone informations...
#define	_XDEF_MAXZONECOUNT			70 
typedef struct 
{
	int				zoneid;
#ifdef _XDEF_NEWSTRINGMANAGER
	LPTSTR			zoneshortname;
	LPTSTR			zonename;
	LPTSTR			zonename2;
#else
	TCHAR			zoneshortname[32];
	TCHAR			zonename[32];
	TCHAR			zonename2[32];
#endif

	TCHAR			zonefilename[32];
	D3DXVECTOR3		basepoint;
	BOOL			servicezone;
	TCHAR			zonebgmfilename1[32];
	TCHAR			zonebgmfilename2[32];
	int				mapsize;
	int				clantype;

#ifdef _XTS_PK
	int				pkclientinfo;
	int				pkserverinfo;
#endif
	
	int				zonepenalty;
	

} _XZone_Info;

extern _XZone_Info					g_ZoneInfoTable[_XDEF_MAXZONECOUNT];
extern DWORD						g_CurrentZoneInfoIndex;
extern int							g_CurrentHoveredWarpzoneIndex;
extern int							g_ClickedWarpZoneIndex;
extern int							g_ChannelTempZoneInfoIndex;


//////////////////////////////////////////////////////////////////////////
// ���� ��� PreCaching ����
#define _XDEF_GUARDMOBMAXPRECACHINGCOUNT		10

typedef struct _XGuardMobPrecachingInfo
{
	int count;
	int spawnlist[_XDEF_GUARDMOBMAXPRECACHINGCOUNT];
}XGuardMobPrecachingInfo;

extern _XGuardMobPrecachingInfo g_GuardMobPrecachingInfo[_XDEF_MAXZONECOUNT];

//=================================================================================
// Flash ���ϸ� ����Ʈ 
#define	_XDEF_MAXFLASHCOUNT			64 
extern TCHAR*						g_SceneList[_XDEF_MAXFLASHCOUNT];
//=================================================================================
// Global utility object...
extern _XMessageBox					g_MessageBox;
extern WORD							g_CommonIntersectionIndices[24];

extern WORD							g_CollectonWordFilter[16];

//=================================================================================
// AI Handler
extern _XPathFinder					g_PathFinder;

//=================================================================================
// GUI Object
extern BOOL							g_ViewUserNameTooltip;
extern BOOL							g_ViewMyNameTooltip;
extern BOOL							g_ViewItemNameTooltip;
extern BOOL							g_ViewMyBattleGauge;
extern BOOL							g_ViewMonsterBattleGauge;
extern BOOL							g_DenyMatch;
extern BOOL							g_ChattingAutoClose;
extern BOOL							g_bSelectClothesMask;

extern _XToolTip					g_InterfaceTooltip;
extern BOOL							g_ViewGuide;
extern BOOL							g_AlwaysViewUserNameAndItemTooltip;
extern _XToolTip					g_WorldObjectTooltip;
extern _XToolTip					g_OperaterTooltip;
extern _XToolTip					g_FameTooltip;
extern _XWindow_NPCName				g_NPCNameWindow;

extern _XToolTip					g_ItemTooltip;

extern int							g_UsernameTooltipViewTimer;
extern int							g_ItemnameTooltipViewTimer;

extern _XTLVERTEX					g_ScreenBurnOut[4];

//=================================================================================
// Interface
extern D3DCOLOR						g_InterfaceColorFactor;
extern BOOL							g_ViewMainInterface;
extern _XWindowManager				g_MainWindowManager;

//=================================================================================
// extern 3d device object...
extern LPDIRECT3DSURFACE9			g_lpShadowSurface;
extern LPDIRECT3DTEXTURE9			g_lpShadowTexture; 
extern D3DXMATRIX					g_ShadowDefaultoffset;

extern DWORD						g_ShadowTextureSize;
extern DWORD				 		g_CloudShadowTextureStage;
extern BOOL					 		g_bCharacterShadow;
extern BOOL					 		g_bBuildingCharacterShadow;
extern BOOL					 		g_bCloudShadow;

extern D3DCOLOR				 		g_ShadowTextureClearColor;
extern D3DCOLOR				 		g_ShadowTextureRenderColor;

extern BOOL					 		g_bUseRealtimeGenerateObject;
extern BOOL					 		g_bUseRealtimeGenerateObjectAni;

extern int					 		g_FadeInOutLevel;
extern BOOL				 	 		g_DrawFadeInOutFlag;
extern BOOL							g_WarpProcessFadeInOutFlag;
extern BOOL					 		g_CameraWork;
extern BOOL							g_NPCDialogScreenEffectFlag;
extern BOOL							g_bNewClanQuest;

// =====================================================================
// ���� ���� ( Game Option )
#ifdef _XDEF_KEYCHANGE_20070115
extern BOOL							g_bGameOptionKeyTypeASelected;	
#endif

// =====================================================================
// Ŀ�´�Ƽ ���� ( Community Option )
extern BOOL							g_bCommunityOptionRejectWhisper;
extern BOOL							g_bCommunityOptionRejectFriend;
extern BOOL							g_bCommunityOptionRejectParty;
extern BOOL							g_bCommunityOptionRefuseOrganization;
extern BOOL							g_bCommunityOptionRejectPvPTrade;
extern BOOL							g_bCommunityOptionRejectTargetInfo;

extern BOOL							g_bCommunityOptionMessengerAutoLogin;
#ifdef _XDEF_ZONECHATTING_20070108
extern BOOL							g_bCommunityOptionDisableZoneChatting;
#endif

// =====================================================================
// NPC Sound Lookup Table
extern TCHAR*				 		g_NPCSoundAllTable[150];
extern TCHAR*				 		g_NPCSoundLUT[150];
extern int					 		g_NPCSoundUsedTable[150];
extern int					 		g_NPCSoundCount;

// =====================================================================
// Scene
extern int							g_indexScene;

// =====================================================================
// Debug object
#ifdef _XDWDEBUG

extern HINSTANCE			 		g_RichEditDLL;
extern HWND					 		g_hEffectScriptWindow;		// Effect script editor window handle
extern BOOL					 		g_ViewScriptWindow;

extern _XList				 		g_DebugOutputlist[4];
extern _XIMEContainer		 		g_DebugCommandEditContainer;
extern void					 		_XDebugOutputString( LPSTR message, int attrib, int slot = 0 );
extern void					 		DrawDebugWindow( void );

extern BOOL							g_ViewPathFindInfo;

extern BOOL							g_EnableUserPacketFiltering;
extern TCHAR						g_PacketFilteringTargetName[13];

extern BOOL							g_UserTracing;
extern TCHAR						g_UserTraceTargetName[13];

#endif

// �����۸� �� ������ 
extern HWND							g_hItemMallWindow;		// Item Mall Web Browser window handle
extern HWND							g_hItemMallWebBrowserObject;		// Item Mall Web Browser window handle
extern BOOL							g_bShowItemMallWindow;
extern BOOL							g_bItemMallLogOff;
extern BOOL							g_bTestWebPageAddress;

// �� �Խ���
#ifdef _XDEF_WEBBOARD_20061211
extern HWND							g_hWebBoardWindow;					// WebBoard Web Browser window handle
extern HWND							g_hIWebBoardWebBrowserObject;		// WebBoard Web Browser window handle
#endif

// �̺�Ʈâ
#ifdef _XDEF_EVENTWINDOW_WEBPAGE_20070214
extern HWND							g_hWebEventWindow;					// WebEvent Web Browser window handle
extern HWND							g_hEventWebBrowserObject;			// WebEvent Web Browser window handle
#endif

#ifdef _ACCLAIM_RUBICONADSYSTEM
extern HWND							g_hRubiconADWindow;
extern HWND							g_hRubiconADWebBrowserObject;
extern POINT						g_RubiconADWindowPosition;
extern SIZE							g_RubiconADWindowSize;
extern BOOL							g_ViewRubiconAD;
extern DWORD						g_RubiconADRefreshTimer;
extern BOOL							g_RubiconADFirstTimeView;
extern int							g_RubiconADFreeLevel;
#endif

// ======================================================================
// Speed Tree
extern BOOL							g_UseSpeedTreeShader;
extern _XST_WrapperCore				g_XST_Wrapper;

// ======================================================================
// String Resource Manager
extern	_XStringResourceManager		g_StrManager;

// =====================================================================
// HTML Script parser
extern	_XHTMLWrapper				g_HTMLWrapper;
extern	_XHTMLWrapper				g_EventHTMLWrapper;
extern  _XHTMLWrapper				g_BookOpenHTMLWrapper;

// =====================================================================
// MAD Wrapper
#ifdef _XDEF_MADSYSTEM
extern _XMADWrapper					g_MadWrapper;
#endif

#ifdef _ACCLAIM_IGAADSYSTEM
#include "XIGAADWrapperCore.h"
extern _XIGAADWrapperCore			g_IGAADWrapper;
#endif

// =====================================================================
// Log
extern BOOL							g_ViewPacketLog;
extern BOOL							g_ViewBattleLog;
extern BOOL							g_ViewBattleDetail;
extern BOOL							g_ViewDamageValue;

// =====================================================================
// for Speed Hack Check
extern DWORD						g_SendTimerPacketTime;

// =====================================================================
// Tutorial On/Off Check
extern _XTUTORIALINFO*				g_TutorialInfo;

// =====================================================================
// Tutorial On/Off Check

typedef struct _tagEventNotice_Old
{
	TCHAR UserName[13];
	TCHAR LastConnectdate[16];
	
	void Reset()
	{
		memset( UserName, 0, sizeof(TCHAR)*13 );
		memset( LastConnectdate, 0, sizeof(TCHAR)*16 );
	};	
} _XEVENTNOTICE_OLD;


typedef struct _tagEventNotice
{
	TCHAR UserName[25];
	TCHAR LastConnectdate[16];
	
	void Reset()
	{
		memset( UserName, 0, sizeof(TCHAR)*25 );
		memset( LastConnectdate, 0, sizeof(TCHAR)*16 );
	};
} _XEVENTNOTICE;

// =====================================================================
// Poison Fog On/Off Check
extern BOOL							g_ViewPoisonFog;
extern BOOL							g_DeleteStartViewPoisonFog;

typedef enum _XBilling_Gender
{
	_XB_BillingGender_Male = 0,
	_XB_BillingGender_Female,
	_XB_BillingGender_ETC,
	_XB_BillingGender_Unknown
};

typedef enum _XBilling_BillingStatusWho
{
	_XB_BillingStatusWho_Home = 0,	// ���� ��ǰ ������
	_XB_BillingStatusWho_PC,		// PC�� ��ǰ ������
	_XB_BillingStatusWho_ISP,		// ISP �����
	_XB_BillingStatusWho_Nothing	// ��������(����ð��� ��� ����Ͽ����ϴ�.)
};

typedef enum _XBilling_BillingStatusWhere
{
	_XB_BillingStatusWhere_Home = 0,
	_XB_BillingStatusWhere_PCRoom,
	_XB_BillingStatusWhere_ISP
};

typedef enum _XBilling_BillingStatusETC
{
	_XB_BillingStatusETC_Nothing = 0,
};

typedef enum _XBilling_BillingPaymentType
{
	_XB_BillingPaymentType_DO = 0,	// ����
	_XB_BillingPaymentType_FO,		// ����
};

typedef enum _XBilling_BillingMethodType
{
	_XB_BillingMethod_Free = 0,		// ���� ��ǰ
	_XB_BillingMethod_Basic,		// �⺻ ��ǰ
	_XB_BillingMethod_Premium,		// �����̾� ��ǰ
	_XB_BillingMethod_Fixed			// ���� ��ǰ
};


// Packet_Result
#define _BILLINGPACKET_RESULT_NONERROR_		0
#define _BILLINGPACKET_RESULT_AUTH_OK_		1

typedef struct XBillingInfo 
{
	DWORD				BillingStatusWho	:	3;
	DWORD				BillingStatusWhere	:	3;
	DWORD				BillingETCData		:	3;
	DWORD				BillPayType			:	3;
	DWORD				BillMethod			:	3;		

	DWORD				reserved			:  17;
	
	DWORD				BillExpireYear;				// ���� ������
	DWORD				BillExpireMonth;			// ���� ������
	DWORD				BillExpireDay;				// ���� ������	
//#ifdef _XVNBILLINGSYSTEM
	DWORD				BillExpireTime;				// ���� ������
	DWORD				BillExpireMinute;			// ���� ������
//#endif
	
	int					BillRemain;					// ���� �ð� �� ����Ʈ
	
	int					AlertCode;
	DWORD				PCRoomBillExpireYear;		// PC�� ���� ������
	DWORD				PCRoomBillExpireMonth;		// PC�� ���� ������
	DWORD				PCRoomBillExpireDay;		// PC�� ���� ������
	DWORD				PCRoomBillRemainTime;
} _XBillingInfo;

extern _XBillingInfo	g_LocalUserBillingInformations;
extern int				g_BillingRemainTimeCounter;
extern BOOL				g_SendBillingRequestPacket;

extern BOOL				g_ViewLocalUserBillingInfo;

// =====================================================================
// �� ����
#ifdef _XDEF_NEWSTRINGMANAGER
	extern LPTSTR				g_MatchGradeName[10];
#else
	extern TCHAR				g_MatchGradeName[10][32];
#endif

extern int					g_MatchFightMoney[6];

// =====================================================================
// NPC Buff ����
extern int					g_NPCBuffCount;
typedef map<int, _XNPCBUFFSTR*> _map_NPCBuffStr;
//extern _XNPCBUFFSTR*		g_NPCBuffStr;
//extern _map_NPCBuffStr		g_NPCBuffStr;
extern _XNPCBUFFLIST		g_NPCBuffList;

// =====================================================================
// ��/�Ǹ� ����
extern _XFame*				g_FameStr;
extern int					g_FameStrLength;

// =====================================================================
// ���� �α� ����
extern _XBattleLog			g_BattleLog;

// =====================================================================
// Mastery Skill
// _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI �� define �Ǿ��� �� ���
extern _XSkillMastery		g_SkillMasteryManager;

// =====================================================================
// Mastery Skill
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_00_00[24];		// ����(00) - ����(00)

#ifdef _XTS_MASTERYOPTIONAL
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_00[24];		// ����(01) - ����(00)
#endif
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_01[3][24];		// ����(01) - ����(01)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_02[3][24];		// ����(01) - ġ��(02)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_03[3][24];		// ����(01) - ����(03)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_04[3][24];		// ����(01) - ���(04)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_05[3][24];		// ����(01) - ���(05)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_06[3][24];		// ����(01) - reserved(06)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_07[3][24];		// ����(01) - reserved(07)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_08[3][24];		// ����(01) - reserved(08)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_09[3][24];		// ����(01) - reserved(09)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_10[3][24];		// ����(01) - reserved(10)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_01_11[3][24];		// ����(01) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_00[24];		// ���(02) - ����(00)
#endif
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_01[3][24];		// ���(02) - ����(01)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_02[3][24];		// ���(02) - ġ��(02)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_03[3][24];		// ���(02) - ����(03)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_04[3][24];		// ���(02) - ���(04)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_05[3][24];		// ���(02) - ���(05)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_06[3][24];		// ���(02) - reserved(06)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_07[3][24];		// ���(02) - reserved(07)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_08[3][24];		// ���(02) - reserved(08)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_09[3][24];		// ���(02) - reserved(09)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_10[3][24];		// ���(02) - reserved(10)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_02_11[3][24];		// ���(02) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_00[24];		// �Ҹ�(03) - ����(00)
#endif
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_01[3][24];		// �Ҹ�(03) - ����(01)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_02[3][24];		// �Ҹ�(03) - ġ��(02)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_03[3][24];		// �Ҹ�(03) - ����(03)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_04[3][24];		// �Ҹ�(03) - ���(04)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_05[3][24];		// �Ҹ�(03) - reserved(05)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_06[3][24];		// �Ҹ�(03) - reserved(06)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_07[3][24];		// �Ҹ�(03) - reserved(07)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_08[3][24];		// �Ҹ�(03) - reserved(08)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_09[3][24];		// �Ҹ�(03) - reserved(09)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_10[3][24];		// �Ҹ�(03) - reserved(10)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_03_11[3][24];		// �Ҹ�(03) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_00[24];		// �츲(04) - ����(00)
#endif
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_01[3][24];		// �츲(04) - ����(01)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_02[3][24];		// �츲(04) - ġ��(02)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_03[3][24];		// �츲(04) - ����(03)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_04[3][24];		// �츲(04) - ���(04)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_05[3][24];		// �츲(04) - reserved(05)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_06[3][24];		// �츲(04) - reserved(06)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_07[3][24];		// �츲(04) - reserved(07)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_08[3][24];		// �츲(04) - reserved(08)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_09[3][24];		// �츲(04) - reserved(09)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_10[3][24];		// �츲(04) - reserved(10)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_04_11[3][24];		// �츲(04) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_00[24];		// ����(05) - ����(00)
#endif
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_01[3][24];		// ����(05) - ����(01)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_02[3][24];		// ����(05) - ġ��(02)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_03[3][24];		// ����(05) - ����(03)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_04[3][24];		// ����(05) - ���(04)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_05[3][24];		// ����(05) - reserved(05)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_06[3][24];		// ����(05) - reserved(06)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_07[3][24];		// ����(05) - reserved(07)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_08[3][24];		// ����(05) - reserved(08)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_09[3][24];		// ����(05) - reserved(09)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_10[3][24];		// ����(05) - reserved(10)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_05_11[3][24];		// ����(05) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_00[24];		// ����(06) - ����(00)
#endif
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_01[3][24];		// ����(06) - ����(01)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_02[3][24];		// ����(06) - ġ��(02)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_03[3][24];		// ����(06) - ����(03)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_04[3][24];		// ����(06) - ���(04)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_05[3][24];		// ����(06) - reserved(05)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_06[3][24];		// ����(06) - reserved(06)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_07[3][24];		// ����(06) - reserved(07)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_08[3][24];		// ����(06) - reserved(08)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_09[3][24];		// ����(06) - reserved(09)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_10[3][24];		// ����(06) - reserved(10)
extern _XSI_MasterySkillDisplayStruct				g_SkillMasteryDisplayProperty_06_11[3][24];		// ����(06) - reserved(11)

// Mastery skill name
extern TCHAR*				g_SkillMasteryNameProperty_00_00;			// ����(00) - ����(00)

#ifdef _XTS_MASTERYOPTIONAL
extern TCHAR*				g_SkillMasteryNameProperty_01_00;			// ����(01) - ����(00)
#endif
extern TCHAR*				g_SkillMasteryNameProperty_01_01[3];		// ����(01) - ����(01)
extern TCHAR*				g_SkillMasteryNameProperty_01_02[3];		// ����(01) - ġ��(02)
extern TCHAR*				g_SkillMasteryNameProperty_01_03[3];		// ����(01) - ����(03)
extern TCHAR*				g_SkillMasteryNameProperty_01_04[3];		// ����(01) - ���(04)
extern TCHAR*				g_SkillMasteryNameProperty_01_05[3];		// ����(01) - ���(05)
extern TCHAR*				g_SkillMasteryNameProperty_01_06[3];		// ����(01) - reserved(06)
extern TCHAR*				g_SkillMasteryNameProperty_01_07[3];		// ����(01) - reserved(07)
extern TCHAR*				g_SkillMasteryNameProperty_01_08[3];		// ����(01) - reserved(08)
extern TCHAR*				g_SkillMasteryNameProperty_01_09[3];		// ����(01) - reserved(09)
extern TCHAR*				g_SkillMasteryNameProperty_01_10[3];		// ����(01) - reserved(10)
extern TCHAR*				g_SkillMasteryNameProperty_01_11[3];		// ����(01) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
extern TCHAR*				g_SkillMasteryNameProperty_02_00;			// ���(02) - ����(00)
#endif
extern TCHAR*				g_SkillMasteryNameProperty_02_01[3];		// ���(02) - ����(01)
extern TCHAR*				g_SkillMasteryNameProperty_02_02[3];		// ���(02) - ġ��(02)
extern TCHAR*				g_SkillMasteryNameProperty_02_03[3];		// ���(02) - ����(03)
extern TCHAR*				g_SkillMasteryNameProperty_02_04[3];		// ���(02) - ���(04)
extern TCHAR*				g_SkillMasteryNameProperty_02_05[3];		// ���(02) - ���(05)
extern TCHAR*				g_SkillMasteryNameProperty_02_06[3];		// ���(02) - reserved(06)
extern TCHAR*				g_SkillMasteryNameProperty_02_07[3];		// ���(02) - reserved(07)
extern TCHAR*				g_SkillMasteryNameProperty_02_08[3];		// ���(02) - reserved(08)
extern TCHAR*				g_SkillMasteryNameProperty_02_09[3];		// ���(02) - reserved(09)
extern TCHAR*				g_SkillMasteryNameProperty_02_10[3];		// ���(02) - reserved(10)
extern TCHAR*				g_SkillMasteryNameProperty_02_11[3];		// ���(02) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
extern TCHAR*				g_SkillMasteryNameProperty_03_00;			// �Ҹ�(03) - ����(00)
#endif
extern TCHAR*				g_SkillMasteryNameProperty_03_01[3];		// �Ҹ�(03) - ����(01)
extern TCHAR*				g_SkillMasteryNameProperty_03_02[3];		// �Ҹ�(03) - ġ��(02)
extern TCHAR*				g_SkillMasteryNameProperty_03_03[3];		// �Ҹ�(03) - ����(03)
extern TCHAR*				g_SkillMasteryNameProperty_03_04[3];		// �Ҹ�(03) - ���(04)
extern TCHAR*				g_SkillMasteryNameProperty_03_05[3];		// �Ҹ�(03) - reserved(05)
extern TCHAR*				g_SkillMasteryNameProperty_03_06[3];		// �Ҹ�(03) - reserved(06)
extern TCHAR*				g_SkillMasteryNameProperty_03_07[3];		// �Ҹ�(03) - reserved(07)
extern TCHAR*				g_SkillMasteryNameProperty_03_08[3];		// �Ҹ�(03) - reserved(08)
extern TCHAR*				g_SkillMasteryNameProperty_03_09[3];		// �Ҹ�(03) - reserved(09)
extern TCHAR*				g_SkillMasteryNameProperty_03_10[3];		// �Ҹ�(03) - reserved(10)
extern TCHAR*				g_SkillMasteryNameProperty_03_11[3];		// �Ҹ�(03) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
extern TCHAR*				g_SkillMasteryNameProperty_04_00;			// �츲(04) - ����(00)
#endif
extern TCHAR*				g_SkillMasteryNameProperty_04_01[3];		// �츲(04) - ����(01)
extern TCHAR*				g_SkillMasteryNameProperty_04_02[3];		// �츲(04) - ġ��(02)
extern TCHAR*				g_SkillMasteryNameProperty_04_03[3];		// �츲(04) - ����(03)
extern TCHAR*				g_SkillMasteryNameProperty_04_04[3];		// �츲(04) - ���(04)
extern TCHAR*				g_SkillMasteryNameProperty_04_05[3];		// �츲(04) - reserved(05)
extern TCHAR*				g_SkillMasteryNameProperty_04_06[3];		// �츲(04) - reserved(06)
extern TCHAR*				g_SkillMasteryNameProperty_04_07[3];		// �츲(04) - reserved(07)
extern TCHAR*				g_SkillMasteryNameProperty_04_08[3];		// �츲(04) - reserved(08)
extern TCHAR*				g_SkillMasteryNameProperty_04_09[3];		// �츲(04) - reserved(09)
extern TCHAR*				g_SkillMasteryNameProperty_04_10[3];		// �츲(04) - reserved(10)
extern TCHAR*				g_SkillMasteryNameProperty_04_11[3];		// �츲(04) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
extern TCHAR*				g_SkillMasteryNameProperty_05_00;			// ����(05) - ����(00)
#endif
extern TCHAR*				g_SkillMasteryNameProperty_05_01[3];		// ����(05) - ����(01)
extern TCHAR*				g_SkillMasteryNameProperty_05_02[3];		// ����(05) - ġ��(02)
extern TCHAR*				g_SkillMasteryNameProperty_05_03[3];		// ����(05) - ����(03)
extern TCHAR*				g_SkillMasteryNameProperty_05_04[3];		// ����(05) - ���(04)
extern TCHAR*				g_SkillMasteryNameProperty_05_05[3];		// ����(05) - reserved(05)
extern TCHAR*				g_SkillMasteryNameProperty_05_06[3];		// ����(05) - reserved(06)
extern TCHAR*				g_SkillMasteryNameProperty_05_07[3];		// ����(05) - reserved(07)
extern TCHAR*				g_SkillMasteryNameProperty_05_08[3];		// ����(05) - reserved(08)
extern TCHAR*				g_SkillMasteryNameProperty_05_09[3];		// ����(05) - reserved(09)
extern TCHAR*				g_SkillMasteryNameProperty_05_10[3];		// ����(05) - reserved(10)
extern TCHAR*				g_SkillMasteryNameProperty_05_11[3];		// ����(05) - reserved(11)

#ifdef _XTS_MASTERYOPTIONAL
extern TCHAR*				g_SkillMasteryNameProperty_06_00;			// ����(06) - ����(00)
#endif
extern TCHAR*				g_SkillMasteryNameProperty_06_01[3];		// ����(06) - ����(01)
extern TCHAR*				g_SkillMasteryNameProperty_06_02[3];		// ����(06) - ġ��(02)
extern TCHAR*				g_SkillMasteryNameProperty_06_03[3];		// ����(06) - ����(03)
extern TCHAR*				g_SkillMasteryNameProperty_06_04[3];		// ����(06) - ���(04)
extern TCHAR*				g_SkillMasteryNameProperty_06_05[3];		// ����(06) - reserved(05)
extern TCHAR*				g_SkillMasteryNameProperty_06_06[3];		// ����(06) - reserved(06)
extern TCHAR*				g_SkillMasteryNameProperty_06_07[3];		// ����(06) - reserved(07)
extern TCHAR*				g_SkillMasteryNameProperty_06_08[3];		// ����(06) - reserved(08)
extern TCHAR*				g_SkillMasteryNameProperty_06_09[3];		// ����(06) - reserved(09)
extern TCHAR*				g_SkillMasteryNameProperty_06_10[3];		// ����(06) - reserved(10)
extern TCHAR*				g_SkillMasteryNameProperty_06_11[3];		// ����(06) - reserved(11)

// ���� ������
extern FLOAT				g_PD_Chr_Step_Mul[11];
extern FLOAT				g_PD_Chr_Level_Mul[11];
extern FLOAT				g_PD_Str_Mul[11];
extern FLOAT				g_PD_Weapon_Str_Add[11];

// ��� ������
extern FLOAT				g_MD_Chr_Step_Mul[11];
extern FLOAT				g_MD_Chr_Level_Mul[11];
extern FLOAT				g_MD_Int_Mul[11];
extern FLOAT				g_MD_Spell_Mul[11];
extern FLOAT				g_MD_Spell_Add[11];
extern FLOAT				g_MD_Zen_Mul[11];

// ���� ����
extern FLOAT				g_PF_Chr_Step_Mul[11];
extern FLOAT				g_PF_Chr_Level_Mul[11];
extern FLOAT				g_PF_Str_Mul[11];

// ��� ����
extern FLOAT				g_MF_Chr_Step_Mul[11];
extern FLOAT				g_MF_Chr_Level_Mul[11];
extern FLOAT				g_MF_Int_Mul[11];
extern FLOAT				g_MF_Zen_Mul[11];

// ���� ���� ������
extern FLOAT				g_PAR_Chr_Step_Mul[11];
extern FLOAT				g_PAR_Chr_Level_Mul[11];
extern FLOAT				g_PAR_Dex_Mul[11];

// ��� ���� ������
extern FLOAT				g_MAR_Chr_Step_Mul[11];
extern FLOAT				g_MAR_Chr_Level_Mul[11];
extern FLOAT				g_MAR_Int_Mul[11];

// ���� ȸ����
extern FLOAT				g_PDR_Chr_Step_Mul[11];
extern FLOAT				g_PDR_Chr_Level_Mul[11];
extern FLOAT				g_PDR_Dex_Mul[11];

// ��� ȸ����
extern FLOAT				g_MDR_Chr_Step_Mul[11];
extern FLOAT				g_MDR_Chr_Level_Mul[11];
extern FLOAT				g_MDR_Int_Mul[11];

#ifdef _XTS_BASEMINMAXWEAPONDAMAGE
// ���� ���� �⺻ ���ݷ�
extern FLOAT				g_PD_Base_Min_Weapon_Mul[10];
extern FLOAT				g_PD_Base_Max_Weapon_Mul[10];
#endif

#ifdef _XTS_NEWBATTLEPARAMETER
extern FLOAT				g_PAB_Str_Mul[11];
extern FLOAT				g_PAB_dex_Mul[11];
extern FLOAT				g_MAB_Int_Mul[11];
extern FLOAT				g_CRI_AR_Dex_Mul[11];
extern FLOAT				g_CRI_AR_Int_Mul[11];

extern FLOAT				g_PF_Vital_Mul[11];		// ���� ����
extern FLOAT				g_PAR_Str_Mul[11];		// ���� ���� ������
extern FLOAT				g_MAR_Dex_Mul[11];		// ��� ���� ������
extern FLOAT				g_MDR_Dex_Mul[11];		// ��� ȸ����
extern FLOAT				g_PAB_Int_Mul[11];		// ���� ���� ����
extern FLOAT				g_CRI_MAR_Dex_Mul[11];	// ġ��Ÿ ���� Ȯ��
#endif

#ifdef _XTS_ITEMMALLBROWSER
extern _XCashItemPackageManager			g_CashItemPackageManger;
#endif

// ��Ʈ�� �˴ٿ� 
#ifdef _XDEF_SHUTDOWN_20061027
extern DWORD				g_PlayTime;
extern DWORD				g_PlayLeftTime;
#endif

// ���� ��ȭ ������ Ÿ�� ( �ѹ� ���� 10�� �Ŀ� �ٽ� ����� �� �ִ�. )
#ifdef _XDEF_ZONECHATTING_20070108
extern DWORD				g_DelayTimeZoneChatting;
extern BOOL					g_bEnableZoneChatting;
extern BOOL					g_bProgressZoneChatting;
#endif

// ����� DS
extern _XCastleManager		g_CBManager;

// ���� ����
extern int					g_nMapTaxRate;

// �� ��ȸ 
extern _XMatchEventManager	g_MatchEventManager;


#ifdef _XCHECKMEMORYUSAGE
	long GetMemoryUsageInfo( void );
	#define _XMEMORYUSECHECKREADY long startfreesize = 0; long endfreesize = 0;
	#define _XMEMORYUSECHECKSTART startfreesize = GetMemoryUsageInfo();
	#define _XMEMORYUSECHECKEND( logstring )   endfreesize = GetMemoryUsageInfo(); _XLog( "Memory report : %s section used size : %f [%d]", logstring, (endfreesize-startfreesize) / 1048576.0f, endfreesize-startfreesize );
#else
	#define _XMEMORYUSECHECKREADY _XCOMMENT
	#define _XMEMORYUSECHECKSTART _XCOMMENT
	#define _XMEMORYUSECHECKEND   _XCOMMENT
#endif
	
// ������ ���� �̻� Define
#define _XDEF_USER_TROUBLESTATE_NORMAL			0x00000000
#define _XDEF_USER_TROUBLESTATE_FURY			0x00000001
#define _XDEF_USER_TROUBLESTATE_CONVERGENCE		0x00000010

	
//////////////////////////////////////////////////////////////////////////
// Interface texture name
extern TCHAR				g_MI_CharTextureName[32];
extern TCHAR				g_MI_KiUpTextureName[32];
extern TCHAR				g_MI_LifeSkillTextureName[32];

#endif
	
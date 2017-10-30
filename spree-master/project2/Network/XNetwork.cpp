// XNetwork.cpp: implementation of the _XNetwork class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XRegistry.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "EmperorOfDragons.h"
#include "XNetwork.h"
#include "XStringItem.h"
#include "XGameItem.h"
#include "SoundEffectList.h"

#include "XWindow_ChattingDefault.h"
#include "XWindow_SystemMsgDefault.h"
#include "XWindow_WorldMinimap.h"
#include "XWindow_MaximumMap.h"
#include "XWindow_Skill.h"
#include "XWindow_RequestParty.h"
#include "XWindow_Party.h"
#include "XWindow_NPCTrade.h"
#include "XWindow_PartyMemberMiniBar.h"
#include "XWindow_PartyMasterMiniBar.h"
#include "XWindow_AdminNotice.h"
#include "XWindow_DeathBlow.h"
#include "XWindow_SelectLoginServer.h"
#include "XWindow_SelectServerForest.h"
#include "XWindow_Warehouse.h"
#include "XWindow_PCTrade.h"
#include "XWindow_Messenger.h"
#include "XWindow_MessengerDialog.h"
#include "XWindow_MessengerTooltip.h"
#include "XWindow_NameInput.h" 
#include "XWindow_TradeConfirm.h"
#include "XWindow_NPCScript.h"
#include "XWindow_AddSaveItem.h"
#include "XWindow_RepairItem.h"
#include "XWindow_JinStatus.h"
#include "XWindow_Rebirth.h"
#include "XWindow_EnterBossRoom.h"
#include "XWindow_Contribution.h"
#include "XMessageWindow.h"
#include "XWindowObjectDefine.h"
#include "xwindow_mainmenu.h"
#include "XWindow_TutorialIcon.h"
#include "XWindow_Option.h"
#include "XWindow_TargetInfo.h"
#include "XWindow_EventNotice.h"
#include "XWindow_PaymentWindow.h"
#include "XWindow_BloodPoint.h"
#include "XWindow_Email.h"
#include "XWindow_ServerUnification.h"
#include "XWindow_ChangeName.h"
#include "XWindow_CashBox.h"
#include "XWindow_Event.h"
#include "XWindow_MoveVillage.h"
#include "XWindow_MonsterStatus.h"
#include "XWindow_PKTracingRequest.h"
#include "XWindow_SmeltItem.h" //Author : �����
#include "XWindow_RecallItem.h" //Author : �����
#include "XWindow_SocketDetach.h" //Author : �����
#include "XWindow_EventIcon.h" //Author : �����
#include "XWindow_BlackList.h"
#include "XWindow_MixWeapon.h"
#include "XWindow_TrainingMed.h"

#include "XWindow_GM_UserCoordination.h"

#ifdef _XTS_ITEMMALLBROWSER	
#include "XWindow_CashMall.h"	
#endif

#ifdef _XDEF_WEBBOARD_20061211
#include "XWindow_WebBoard.h"	
#endif

#include "XCrypto.h"
#include "XSR_STRINGHEADER.H"
#include "XParticleIDDefine.h"

#include "Request.h"

#ifdef _XADMINISTRATORMODE
#include "ws2tcpip.h"
#endif

#ifdef _XTS_ITEMMALLBROWSER	
// win32 web browser api
#include "./Library/CWebPage/XWebPage.h"
#endif


//Author : ����� 

//#define _USE_NPROTECTLOG
//#define _XDEF_NPROTECT_GUARD_TIME_LOG

#ifdef _XUSEAPEXMODULE // �븸 Apex module ���� functions.

	#define _XAPEXMODULE_LOG

	#pragma message( "_XTAIWANESE APEX MODULE" )
	#include "ApexClient.h"
	#pragma comment(lib, "./Library/AHClientInterface.lib")

	_FUNC_C_REC	g_pfRec = NULL;
	
	BOOL		g_ApexInitialized = FALSE;

	long _ApexNetSendToGameServer(const char * pBuffer,int nLen)
	{
		#ifdef _XAPEXMODULE_LOG
			_XLog( "Apex _ApexNetSendToGameServer, data length = %d", nLen );
		#endif

		if( !g_NetworkKernel.SendPacket( MSG_NO_SECURITY, en_apex_client_data, (int)pBuffer, nLen ) ) return -1;

		return 0;
	}

	int	_ApexNotice_UserData(const char * pBuf,int nBufLen)
	{
		if(g_pfRec)
		{
			g_pfRec(pBuf,nBufLen);
		}

		#ifdef _XAPEXMODULE_LOG
			_XLog( "Apex _ApexNotice_UserData, data length = %d", nBufLen );
		#endif

		return 0;
	};

#endif

#if defined(_ACCLAIM_VERSION) /*|| defined(_XVIETNAMESE)*/ || defined(_XJAPANESE)
	#define _XENABLE_GAMEGUARD
#endif

#ifdef _XUSEAPEXMODULE
	#undef _XENABLE_GAMEGUARD
#endif
	
#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG) && !defined(_XGMCLIENT)
	#ifdef _XENABLE_GAMEGUARD
		#define _XUSE_GAMEGUARD
		extern struct MSG_GGAUTH g_GameGuardData;
	#endif
#endif
//Last updated: 07/01/18

// �ӽ�
BOOL g_nmcoInit = FALSE;

TCHAR g_szLastUsedCheatEngine[64];

extern char   g_szAcclaimID[64];
extern char   g_szAcclaimPW[64];

//////////////////////////////////////////////////////////////////////
// Pre.. define & grobal value

#ifdef _XDWDEBUG
void	_XDWINBATTLELOG( TCHAR *msg, ... )
{
	if(!g_ViewPacketLog) return;

	TCHAR szBuff[1024];
	int retValue;
	va_list argptr;
	
	va_start( argptr, msg );
	retValue = _vsnprintf( szBuff, sizeof(szBuff), msg, argptr );
	va_end( argptr );

	_XDebugOutputString( szBuff, 1, 2 );
}
void	_XDWINBATTLERESULTLOG( TCHAR *msg, ... )
{
	if(!g_ViewPacketLog) return;
	
	TCHAR szBuff[1024];
	int retValue;
	va_list argptr;
	
	va_start( argptr, msg );
	retValue = _vsnprintf( szBuff, sizeof(szBuff), msg, argptr );
	va_end( argptr );
	
	_XDebugOutputString( szBuff, 1, 3 );
}
#endif

#define	_XMACRODEF_SENDERRORMESSAGEBOX		g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );							\
											TCHAR messagestring[256];																			\
											memset(messagestring, 0, sizeof(TCHAR)*256);														\
											strcpy(messagestring, _XGETINTERFACETEXT((ID_STRING_NETWORK_FAILSENDDATA));							\
											g_MessageBox.SetMessage( "%s [_ECode:%d]", messagestring, WSAGetLastError() );						\
											g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );									\
											_XUpdateWindow();

int		g_nPI = 0;
int		g_nRI = 0;
BYTE	g_szPacket[_XDEF_PACKET_MAXBUFFER];
short	g_sPacketLen = 0;
int		g_mode = _X_PACKET_RECIEVE_PACKET_LENGTH;
extern  D3DCOLOR g_ChattingStringColor[16];
CCapsulateCrypto g_PacketCrypto;

extern void ChangeModel( DWORD lparam, char firstType, char facemaskmode = 0 );

extern void _XInsertCenterSystemMessage( LPTSTR message , D3DCOLOR messageinfo );

int HangleCheck( BYTE *Data, int MaxSize )
{
    int LastByte;
    for( int i = 0 ; i <  MaxSize ;  )
    {
        LastByte = i;
        if( !Data[i] )
            break;
        if( Data[i] >= 0x80 )
            i+=2;
        else
            i++;
    }
    return LastByte;
}

void GetGameTime( int time, sDate * Date )
{
	Date->Year = time / _GAME_TIME_YEAR_ ;
	time = time % _GAME_TIME_YEAR_ ;
	
	Date->Month = time / _GAME_TIME_MONTH_ ;
	time = time % _GAME_TIME_MONTH_ ;
	
	Date->Day = time / _GAME_TIME_DAY_ ;
	Date->Second = time % _GAME_TIME_DAY_ ;
}

extern void _XInsertSystemMessage( int messagemode, TCHAR* args, ... );

void _XInsertChatString( LPSTR message, int messagemode )
{
	g_NetworkKernel.InsertChatString( message, messagemode );
}

_XServerAddressManager	g_ServerAddressManager;

void _XServerAddressManager::XOREncrypt( void )
{
	LPBYTE pServerInfo = (LPBYTE)&m_ServerInfo;

	for( int i = 0; i < sizeof(_XSERVERINFOSTRUCT); ++i )
	{
		pServerInfo[i] ^= _XADDRESSENCODEKEY;
	}
}

BOOL _XServerAddressManager::GetServerInfo( void )
{
	memset( &m_ServerInfo, 0, sizeof(_XSERVERINFOSTRUCT) );

#ifdef _XTESTSERVER
	m_ServerInfo.testserverindicator = _XDEF_TESTSERVERSTRUCTINDICATOR;

#ifdef _XTAIWANESE
#ifdef _XTW_CSVERSION
	strcpy( m_ServerInfo.patchserveraddress, "219.84.173.197" );
	strcpy( m_ServerInfo.serveraddress, "219.84.173.197" );
	strcpy( m_ServerInfo.cdnserverurl, "patch.9ds.com.tw" );
#else
	strcpy( m_ServerInfo.patchserveraddress, "login.9ds.com.tw" );
	strcpy( m_ServerInfo.serveraddress, "login.9ds.com.tw" );
	strcpy( m_ServerInfo.cdnserverurl, "patch.9ds.com.tw" );
#endif //#ifdef _XTW_CSVERSION
#else //#ifdef _XTAIWANESE

	#ifdef _XVIETNAMESE
		strcpy( m_ServerInfo.patchserveraddress, "login.cuulong.com.vn" );
		strcpy( m_ServerInfo.serveraddress, "login.cuulong.com.vn" );
		strcpy( m_ServerInfo.cdnserverurl, "dl.sg.vinagame.com.vn" );
	#else
		#ifdef _XENGLISH
			#ifdef _ACCLAIM_VERSION			
				strcpy( m_ServerInfo.patchserveraddress, "64.93.77.158" );
				strcpy( m_ServerInfo.serveraddress, "64.93.77.158" );
				strcpy( m_ServerInfo.cdnserverurl, "9dragons.acclaimdownloads.com/9dragons" );
			#else // PWC Version
				strcpy( m_ServerInfo.patchserveraddress, "85.17.55.207" );
				strcpy( m_ServerInfo.serveraddress, "85.17.55.207" );
				strcpy( m_ServerInfo.cdnserverurl, "85.17.55.204" );
			#endif
		#else
			#ifdef _XJAPANESE
				strcpy( m_ServerInfo.patchserveraddress, "login.9dragon.jp" );
				strcpy( m_ServerInfo.serveraddress, "login.9dragon.jp" );
				strcpy( m_ServerInfo.cdnserverurl, "rocworks.nefficient.jp/rocworks/9dragons" );
			#else
				#ifdef _XRUSSIAN
					strcpy( m_ServerInfo.patchserveraddress, "77.221.141.138" );
					strcpy( m_ServerInfo.serveraddress, "77.221.141.138" );
					strcpy( m_ServerInfo.cdnserverurl, "download.indy21.lgcdn.com" );
				#else
					strcpy( m_ServerInfo.patchserveraddress, _T("211.39.155.196") );
					strcpy( m_ServerInfo.serveraddress, _T("211.39.155.196") );
					strcpy( m_ServerInfo.cdnserverurl, _T("download.indy21.lgcdn.com") );
				#endif
			#endif
		#endif
	#endif //#ifdef _XVIETNAMESE

#endif //#ifdef _XTAIWANESE

	m_ServerInfo.patchserverport = 50001;
	m_ServerInfo.serverport = 48300;
	
	XOREncrypt();
	BOOL retval = g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT), TRUE );	
	XOREncrypt();

	if( m_ServerInfo.testserverindicator != _XDEF_TESTSERVERSTRUCTINDICATOR )
	{
		memset( &m_ServerInfo, 0, sizeof(_XSERVERINFOSTRUCT) );
		_XFatalError( "Invalid server information : Modified registry data" );
		return retval;
	}	
	return retval;
#else //#ifdef _XTESTSERVER
	
#ifdef _XTAIWANESE
	#ifdef _XTW_CSVERSION
		strcpy( m_ServerInfo.patchserveraddress, "219.84.173.197" );
		strcpy( m_ServerInfo.serveraddress, "219.84.173.197" );
		strcpy( m_ServerInfo.cdnserverurl, "patch.9ds.com.tw" );
	#else
		strcpy( m_ServerInfo.patchserveraddress, "login.9ds.com.tw" );
		strcpy( m_ServerInfo.serveraddress, "login.9ds.com.tw" );
		strcpy( m_ServerInfo.cdnserverurl, "patch.9ds.com.tw" );
	#endif
#else
	#ifdef _XVIETNAMESE
		strcpy( m_ServerInfo.patchserveraddress, "login.cuulong.com.vn" );
		strcpy( m_ServerInfo.serveraddress, "login.cuulong.com.vn" );
		strcpy( m_ServerInfo.cdnserverurl, "dl.sg.vinagame.com.vn" );
	#else
		#ifdef _XENGLISH
			#ifdef _ACCLAIM_VERSION
				strcpy( m_ServerInfo.patchserveraddress, "64.93.77.158" );
				strcpy( m_ServerInfo.serveraddress, "64.93.77.158" );
				strcpy( m_ServerInfo.cdnserverurl, "9dragons.acclaimdownloads.com/9dragons" );
			#else // PWC Version
				strcpy( m_ServerInfo.patchserveraddress, "85.17.55.207" );
				strcpy( m_ServerInfo.serveraddress, "85.17.55.207" );
				strcpy( m_ServerInfo.cdnserverurl, "85.17.55.204" );
			#endif
		#else
			#ifdef _XJAPANESE
				strcpy( m_ServerInfo.patchserveraddress, "203.178.123.81" );
				strcpy( m_ServerInfo.serveraddress, "203.178.123.81" );
				strcpy( m_ServerInfo.cdnserverurl, "rocworks.nefficient.jp/rocworks/9dragons" );
			#else
				#ifdef _XRUSSIAN
					strcpy( m_ServerInfo.patchserveraddress, "77.221.141.138" );
					strcpy( m_ServerInfo.serveraddress, "77.221.141.138" );
					strcpy( m_ServerInfo.cdnserverurl, "download.indy21.lgcdn.com" );
				#else
					strcpy( m_ServerInfo.patchserveraddress, _T("login.ninedragons.co.kr") );
					strcpy( m_ServerInfo.serveraddress, _T("login.ninedragons.co.kr") );
					strcpy( m_ServerInfo.cdnserverurl, _T("download.ninedragons.co.kr") );
				#endif
			#endif
		#endif
	#endif
#endif


	m_ServerInfo.patchserverport = 50001;
	m_ServerInfo.serverport = 48300;

	XOREncrypt();
	BOOL retval = g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ndc info"), (LPBYTE)&m_ServerInfo, (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT), TRUE );	
	XOREncrypt();

#ifdef _XADMINISTRATORMODE
	if( _XCmdLineOption("xgmlogin",0) != 0 )
	{
#ifdef _XTAIWANESE
		strcpy( m_ServerInfo.patchserveraddress, _T("219.84.173.5") );
		strcpy( m_ServerInfo.serveraddress, _T("219.84.173.5") );
#else
		strcpy( m_ServerInfo.patchserveraddress, _T("211.115.88.137") );
		strcpy( m_ServerInfo.serveraddress, _T("211.115.88.137") );
#endif
	}
#endif
	
	return retval;
#endif
}

void _XServerAddressManager::SetServerAddress( LPTSTR address )
{
	GetServerInfo();
	strcpy( m_ServerInfo.serveraddress, address );

	XOREncrypt();
#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif
	XOREncrypt();
}

void _XServerAddressManager::SetPatchServerAddress( LPTSTR address )
{
	GetServerInfo();	
	strcpy( m_ServerInfo.patchserveraddress, address );
	
	XOREncrypt();
#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif
	XOREncrypt();
}

void _XServerAddressManager::SetServerPort( DWORD port )
{
	GetServerInfo();	
	m_ServerInfo.serverport = port;
	
	XOREncrypt();
#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif
	XOREncrypt();
}

void _XServerAddressManager::SetPatchServerPort( DWORD port )
{
	GetServerInfo();	
	m_ServerInfo.patchserverport = port;
	
	XOREncrypt();
#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif
	XOREncrypt();
}

void _XServerAddressManager::SetCDNServerUrl( LPTSTR url )
{
	GetServerInfo();	
	strcpy( m_ServerInfo.cdnserverurl , url );
	
	XOREncrypt();
#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ts ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("ndc info"), (LPBYTE)&m_ServerInfo, sizeof(_XSERVERINFOSTRUCT) );
#endif
	XOREncrypt();
}


STDMETHODIMP _XGetExeFileVersion( LPSTR sFilename, DWORD& h_1, DWORD& h_2, DWORD& l_1, DWORD& l_2 )
{
    // TODO: Add your implementation code here
    
	DWORD nInfoSize = GetFileVersionInfoSize(sFilename, 0);
	
	BYTE* pBlock = new BYTE[nInfoSize+1];
	ZeroMemory(pBlock, nInfoSize+1);
	
	GetFileVersionInfo(sFilename, 0, nInfoSize, pBlock);
	
	UINT cbTranslate;
	
	struct VS_FIXEDFILEINFO { 
		DWORD dwSignature; 
		DWORD dwStrucVersion; 
		DWORD dwFileVersionMS; 
		DWORD dwFileVersionLS; 
		DWORD dwProductVersionMS; 
		DWORD dwProductVersionLS; 
		DWORD dwFileFlagsMask; 
		DWORD dwFileFlags; 
		DWORD dwFileOS; 
		DWORD dwFileType; 
		DWORD dwFileSubtype; 
		DWORD dwFileDateMS; 
		DWORD dwFileDateLS; 
	} *lpTranslate;
	
	// Read the list of languages and code pages.
	
	VerQueryValue(pBlock, TEXT("\\"), (LPVOID*)&lpTranslate, &cbTranslate);
	
	h_1 = HIWORD(lpTranslate->dwFileVersionMS);
	h_2 = LOWORD(lpTranslate->dwFileVersionMS);
	l_1 = HIWORD(lpTranslate->dwFileVersionLS);
	l_2 = LOWORD(lpTranslate->dwFileVersionLS);
	
	delete pBlock;
	
	return S_OK;
}

#ifdef _XTESTSERVER	

	#define _XDEF_CURRENTMINORVERSION_HIGH		219
	#define _XDEF_CURRENTMINORVERSION_LOW		( _XDEF_CURRENTMINORVERSION_HIGH + 121 )

#else

	#define _XDEF_CURRENTMINORVERSION_HIGH		219
	#define _XDEF_CURRENTMINORVERSION_LOW		( _XDEF_CURRENTMINORVERSION_HIGH + 121 )

#endif


_XPatchVersionManager	g_PatchVersionManager;

void _XPatchVersionManager::XOREncrypt( void )
{
	LPBYTE pServerInfo = (LPBYTE)&m_VersionInfo;
	
	for( int i = 0; i < sizeof(_XPATCHVERSIONINFOSTRUCT); ++i )
	{
		pServerInfo[i] ^= _XPATCHVERSIONENCODEKEY;
	}
}

DWORD _XPatchVersionManager::GetExeVersion( void )
{
	return (((BYTE)g_PatchVersionManager.m_VersionInfo.exeversion_1) << 24) |
		   (((BYTE)g_PatchVersionManager.m_VersionInfo.exeversion_2) << 16) |
		   (((SHORT)g_PatchVersionManager.m_VersionInfo.exeversion_4) );
}

BOOL _XPatchVersionManager::GetVersionInfo( void )
{
	memset( &m_VersionInfo, 0, sizeof(_XPATCHVERSIONINFOSTRUCT) );

#ifdef _XTESTSERVER	

	m_VersionInfo.testserverindicator = _XDEF_TESTSERVERSTRUCTINDICATOR;

	if( !g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("_xkern_tsndvinfo"), (LPBYTE)&m_VersionInfo, (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT), FALSE ) )
	{
#ifdef _XTAIWANESE		
		m_VersionInfo.patchversion_1 = 0;
#else
	#ifdef _XVIETNAMESE
		m_VersionInfo.patchversion_1 = 0;
	#else
		#ifdef _XENGLISH
			#ifdef _ACCLAIM_VERSION
				m_VersionInfo.patchversion_1 = 2;
			#else // PWC Version
				m_VersionInfo.patchversion_1 = 0;
			#endif
		#else
			#ifdef _XJAPANESE
				m_VersionInfo.patchversion_1 = 0;
			#else
				#ifdef _XRUSSIAN
					m_VersionInfo.patchversion_1 = 0;
				#else
					m_VersionInfo.patchversion_1 = 0;
				#endif
			#endif
		#endif
	#endif
#endif
	}
	else
	{
		XOREncrypt();
		if( m_VersionInfo.testserverindicator != _XDEF_TESTSERVERSTRUCTINDICATOR )
		{
			memset( &m_VersionInfo, 0, sizeof(_XPATCHVERSIONINFOSTRUCT) );
			_XFatalError( "Invalid version information : Modified registry data" );
			return FALSE;
		}
	}
#else
	if( !g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("_xkern_ndvinfo"), (LPBYTE)&m_VersionInfo, (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT), FALSE ) )
	{
		//m_VersionInfo.patchversion_1 = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_PATCHNUMBER, 0, TRUE);
		#ifdef _XTAIWANESE		
			#ifdef _XTW_CSVERSION
				m_VersionInfo.patchversion_1 = 0;
			#else
				m_VersionInfo.patchversion_1 = 0;
			#endif
		#else
			#ifdef _XVIETNAMESE
				m_VersionInfo.patchversion_1 = 0;
			#else
				#ifdef _XENGLISH
					#ifdef _ACCLAIM_VERSION
						m_VersionInfo.patchversion_1 = 2;
					#else // PWC Version
						m_VersionInfo.patchversion_1 = 0;
					#endif
				#else
					#ifdef _XJAPANESE
						m_VersionInfo.patchversion_1 = 0;
					#else
						#ifdef _XRUSSIAN
							m_VersionInfo.patchversion_1 = 0;
						#else
							m_VersionInfo.patchversion_1 = 256;
						#endif
					#endif
				#endif
			#endif
		#endif
	}
	else
	{

	}
#endif

	m_VersionInfo.patchversion_2 = rand() % 32700;

	TCHAR ModuleFile[ 512 ];
	memset( ModuleFile, 0, sizeof(TCHAR) * 512 );
	GetModuleFileName( NULL, ModuleFile, 511 );
	DWORD h1,h2,l1,l2;
	_XGetExeFileVersion( ModuleFile, h1,h2,l1,l2 );
	
	m_VersionInfo.exeversion_1 = h1;
	m_VersionInfo.exeversion_2 = h2;
	m_VersionInfo.exeversion_3 = l1;
	m_VersionInfo.exeversion_4 = l2;
		
	XOREncrypt();
#ifdef _XTESTSERVER
	BOOL retval = g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("_xkern_tsndvinfo"), (LPBYTE)&m_VersionInfo, (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT), TRUE );	
#else
	BOOL retval = g_RegManager._XRegReadBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("_xkern_ndvinfo"), (LPBYTE)&m_VersionInfo, (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT), TRUE );	
#endif
	XOREncrypt();
	
	if( m_VersionInfo.exeversion_1 != h1 ||
		m_VersionInfo.exeversion_2 != h2 ||
		m_VersionInfo.exeversion_4 != l2 )
	{
		m_VersionInfo.exeversion_1 = h1;
		m_VersionInfo.exeversion_2 = h2;
		m_VersionInfo.exeversion_3 = l1;
		m_VersionInfo.exeversion_4 = l2;

		XOREncrypt();
#ifdef _XTESTSERVER
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("_xkern_tsndvinfo"), (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("_xkern_ndvinfo"), (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#endif
		XOREncrypt();
	}

	// Check inner define minor version
	if( m_VersionInfo.exeversion_4 != (_XDEF_CURRENTMINORVERSION_LOW - _XDEF_CURRENTMINORVERSION_HIGH) )
	{
		m_VersionInfo.exeversion_1 = 0;
		m_VersionInfo.exeversion_2 = 0;
		m_VersionInfo.exeversion_3 = 0;
		m_VersionInfo.exeversion_4 = 0;
		XOREncrypt();
#ifdef _XTESTSERVER
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("_xkern_tsndvinfo"), (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
		g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("_xkern_ndvinfo"), (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#endif
		XOREncrypt();
	}	

#ifdef _XDWDEBUG
	_XDWINPRINT("version : %d/%d/%d/%d/%d/%d", m_VersionInfo.patchversion_1, m_VersionInfo.patchversion_2, m_VersionInfo.exeversion_1, m_VersionInfo.exeversion_2, m_VersionInfo.exeversion_3, m_VersionInfo.exeversion_4);
#endif

	return retval;
}

void _XPatchVersionManager::SetPatchNumber( DWORD version )
{
	GetVersionInfo();	
	m_VersionInfo.patchversion_1 = version;
	m_VersionInfo.patchversion_2 = rand() % 32700;
	
	XOREncrypt();
#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("_xkern_tsndvinfo"), (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("_xkern_ndvinfo"), (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#endif
	XOREncrypt();
}

void _XPatchVersionManager::SetExeVersionnumber( DWORD v1, DWORD v2, DWORD v3, DWORD v4 )
{
	GetVersionInfo();	
	
	m_VersionInfo.exeversion_1 = v1;
	m_VersionInfo.exeversion_2 = v2;
	m_VersionInfo.exeversion_3 = v3;
	m_VersionInfo.exeversion_4 = v4;
	
	XOREncrypt();
#ifdef _XTESTSERVER
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("_xkern_tsndvinfo"), (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#else
	g_RegManager._XRegWriteBinary( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, _T("_xkern_ndvinfo"), (LPBYTE)&m_VersionInfo, sizeof(_XPATCHVERSIONINFOSTRUCT) );
#endif
	XOREncrypt();
}

BOOL _XIsIPv4Address(char *pHost, int nLen)
{	
    int i, nIndex;	
    bool bDigitOK;	
    int nDigit;
	
    if (pHost == NULL)		
        goto _false;
	
    i = 0;	
    for (nIndex = 1; nIndex <= 4; nIndex++)		
    {		
        bDigitOK = false;		
        nDigit = 0;
		
        while (i < nLen)			
        {			
            if (pHost[i] < '0' || pHost[i] > '9')				
                break;
			
            bDigitOK = true;			
            nDigit = nDigit * 10 + (pHost[i] - '0');			
            i++;
			
            if (nDigit > 255)				
                goto _false;			
        }
		
        if (!bDigitOK)			
            goto _false;
		
        if (nDigit > 255)			
            goto _false;
		
        if (nIndex < 4)			
        {			
            if (pHost[i] != '.')				
            {				
                goto _false;				
            }			
            i++; // skip '.'			
        } 
		else 
		{			
            if (i != nLen)				
            {				
                goto _false;				
            }			
        }		
    }
	
    return TRUE;
	
_false:	
    return FALSE;
}

BOOL _XGetHostName( LPCTSTR pHostName, LPTSTR pIPAdress, int _length ) 
{
	PHOSTENT pHostInfo;

	if((pHostInfo = gethostbyname(pHostName)) != NULL)
	{
		memset( pIPAdress, 0, _length );
		strcpy( pIPAdress, inet_ntoa(*(struct in_addr *)*pHostInfo->h_addr_list) );

		return TRUE;
	}

	//DWORD errcode = GetLastError();
	return FALSE;		
}

BOOL _XCheckLoginServerIP( LPCTSTR szHostName ) // check virtual server
{
	TCHAR		szName[255];
	TCHAR		sAddress[255];
	PHOSTENT    pHostEntry;
	IN_ADDR        inAddr;
	memset( szName, 0, sizeof(szName));
	memset( sAddress, 0, sizeof(sAddress));

	if( gethostname( szName, 255 ) != 0 ) 
	{
		return FALSE;
	}
	else
	{
		if( ( pHostEntry = gethostbyname( szName ) ) != NULL )
		{
			memcpy( &inAddr, pHostEntry->h_addr, 4 );
			_snprintf( sAddress, sizeof(sAddress), _T("%s"), inet_ntoa( inAddr ) );
		}
		else
		{
			return FALSE;
		}
	}

	TCHAR szHostIPAddress[256];
	if( _XGetHostName( szHostName, szHostIPAddress, 256 ) )
	{
		if( strcmp( szHostIPAddress, sAddress ) == 0 )
		{
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}
	return FALSE;
}

BOOL CND_LoginChecker::lc_MakePacket( u_int lmbcMode IN, char * passport OUT, int passlength IN, const char * pID IN, const char * pPW IN, const char * pIP IN )
{
     BOOL bRet = TRUE;
     int count;
     int nRand;
     int Key;
     int temp;

     CopyMemory( &count, pPW, 4 );
     CopyMemory( &nRand, pID, 4 );
     Key = count + nRand;                    // ==> Key Value

     count = 0;
     nRand = 0;

	 CopyMemory( &passport[0], pID, 8 );
	 CopyMemory( &passport[8], pPW, 8 ); 

     switch( lmbcMode )
     {
		case en_lmbc_nexon:
		{
			passport[7] += passport[0];
			passport[6] += passport[1];
			passport[5] += passport[2];
			passport[4] += passport[3];

			passport[15] += passport[8];
			passport[14] += passport[9];
			passport[13] += passport[10];
			passport[12] += passport[11];

			for( int i = 0 ; i < 4 ; ++i )
			{
				CopyMemory( &temp, &passport[i*4], 4 ) ;
				temp ^= Key ;
				CopyMemory( &passport[i*4], &temp, 4 ) ;
			}

			count += 16;

			nRand = rand() % 8 + 1;
			passport[count] = static_cast<char>(nRand);
			count += nRand;
			CopyMemory( &passport[count], pPW, en_max_lpl + 1 );
			count += (en_max_lpl + 1); 

			nRand = rand() % 8 + 1;
			passport[count] = static_cast<char>(nRand);
			count += nRand;
			CopyMemory( &passport[count], pID, en_max_lil + 1 );
			count += (en_max_lil + 1);
		}
		break;
		
		case en_lmbc_metel:
        {
            for( int i = 0 ; i < 4 ; ++i )
            {
				CopyMemory( &temp, &passport[i*4], 4 ) ;
				temp ^= Key ;
                CopyMemory( &passport[i*4], &temp, 4 ) ;
            }
            count += 16;
            nRand = rand() % 8 + 1;
            passport[count] = static_cast<char>(nRand);
            count += nRand;
            CopyMemory( &passport[count], pPW, en_max_lpl + 1 );
            count += (en_max_lpl + 1);

            nRand = rand() % 8 + 1;
            passport[count] = static_cast<char>(nRand);
            count += nRand;
            CopyMemory( &passport[count], pID, en_max_lil + 1 );
            count += (en_max_lil + 1);
        }
		break ;
		
		case en_lmbc_vina:
        {
            for( int i = 0 ; i < 4 ; ++i )
            {
                 CopyMemory( &temp, &passport[i*4], 4 ) ;
                 temp ^= Key ;
                 CopyMemory( &passport[i*4], &temp, 4 ) ;
            }
            count += 16;
            nRand = rand() % 8 + 1;
            passport[count] = static_cast<char>(nRand);
            count += nRand;
            CopyMemory( &passport[count], pPW, en_max_lpl + 1 );
            count += (en_max_lpl + 1);

            nRand = rand() % 8 + 1;
            passport[count] = static_cast<char>(nRand);
            count += nRand;
            CopyMemory( &passport[count], pID, en_max_lil + 1 );
            count += (en_max_lil + 1);
        }
        break ;
     case en_lmbc_acclaim:
        {
            for( int i = 0 ; i < 4 ; ++i )
            {
                CopyMemory( &temp, &passport[i*4], 4 ) ;
                temp ^= Key ;
                CopyMemory( &passport[i*4], &temp, 4 ) ;
            }
            count += 16;
            nRand = rand() % 8 + 1;
            passport[count] = static_cast<char>(nRand);
            count += nRand;
            CopyMemory( &passport[count], pPW, en_max_lpl + 1 );
            count += (en_max_lpl + 1);

            nRand = rand() % 8 + 1;
            passport[count] = static_cast<char>(nRand);
            count += nRand;
            CopyMemory( &passport[count], pID, en_max_lil + 1 );
            count += (en_max_lil + 1);
        }
		break ;
	 case en_lmbc_pwc:
		 {
			 for( int i = 0 ; i < 4 ; ++i )
			 {
				 CopyMemory( &temp, &passport[i*4], 4 ) ;
				 temp ^= Key ;
				 CopyMemory( &passport[i*4], &temp, 4 ) ;
			 }
			 count += 16;
			 nRand = rand() % 8 + 1;
			 passport[count] = static_cast<char>(nRand);
			 count += nRand;
			 CopyMemory( &passport[count], pPW, en_max_lpl + 1 );
			 count += (en_max_lpl + 1);
			 
			 nRand = rand() % 8 + 1;
			 passport[count] = static_cast<char>(nRand);
			 count += nRand;
			 CopyMemory( &passport[count], pID, en_max_lil + 1 );
			 count += (en_max_lil + 1);
		 }
		 break ;
	case en_lmbc_rocworks:
		 {
			for( int i = 0 ; i < 4 ; ++i )
            {
                 CopyMemory( &temp, &passport[i*4], 4 ) ;
                 temp ^= Key ;
                 CopyMemory( &passport[i*4], &temp, 4 ) ;
            }
            count += 16;
            nRand = rand() % 8 + 1;
            passport[count] = static_cast<char>(nRand);
            count += nRand;
            CopyMemory( &passport[count], pPW, en_max_lpl + 1 );
            count += (en_max_lpl + 1);

            nRand = rand() % 8 + 1;
            passport[count] = static_cast<char>(nRand);
            count += nRand;
            CopyMemory( &passport[count], pID, en_max_lil + 1 );
            count += (en_max_lil + 1);
		 }
		 break ;
	 case en_lmbc_russia:
        {
            for( int i = 0 ; i < 4 ; ++i )
            {
                CopyMemory( &temp, &passport[i*4], 4 ) ;
                temp ^= Key ;
                CopyMemory( &passport[i*4], &temp, 4 ) ;
            }
            count += 16;
            nRand = rand() % 8 + 1;
            passport[count] = static_cast<char>(nRand);
            count += nRand;
            CopyMemory( &passport[count], pPW, en_max_lpl + 1 );
            count += (en_max_lpl + 1);

            nRand = rand() % 8 + 1;
            passport[count] = static_cast<char>(nRand);
            count += nRand;
            CopyMemory( &passport[count], pID, en_max_lil + 1 );
            count += (en_max_lil + 1);
        }
		break ;
     default:
        bRet = FALSE;
     }

     return bRet;
}

/*
BOOL CND_LoginChecker::lc_MakePacket( u_int lmbcMode IN, char * passport OUT, int passlength IN, const char * pID IN, const char * pPW IN, const char * pIP IN )
{
	BOOL bRet = TRUE;
	int count;
	int nRand;		
	
	int Key;
	int temp;
	
	CopyMemory( &count, pPW, 4 );
	CopyMemory( &nRand, pID, 4 );
	Key = count + nRand;		// ==> Key Value
	
	count = 0;
	nRand = 0;
	
	switch( lmbcMode )
	{
	case en_lmbc_nexon:
		{
			CopyMemory( &passport[0], pID, 8 );
			CopyMemory( &passport[8], pPW, 8 );
			
			passport[7] += passport[0];
			passport[6] += passport[1];
			passport[5] += passport[2];
			passport[4] += passport[3];
			
			passport[15] += passport[8];
			passport[14] += passport[9];
			passport[13] += passport[10];
			passport[12] += passport[11];
			
			for( int i = 0 ; i < 4 ; ++i )
			{
				CopyMemory( &temp, &passport[i*4], 4 ) ;
				temp ^= Key ;
				CopyMemory( &passport[i*4], &temp, 4 ) ;
			}
			count += 16;
			
			nRand = rand() % 8 + 1;
			passport[count] = static_cast<char>(nRand);
			count += nRand;
			CopyMemory( &passport[count], pPW, en_lpl_nexon + 1 );
			count += en_lpl_nexon + 1;
			
			nRand = rand() % 8 + 1;
			passport[count] = static_cast<char>(nRand);
			count += nRand;
			CopyMemory( &passport[count], pID, en_lil_nexon + 1 );
			count += en_lil_nexon + 1;
		}
		break;
	default:
		bRet = FALSE;
	}
	
	return bRet;
}
*/

BOOL CND_LoginChecker::lc_CheckPakcet( u_int mode IN, const char * passport IN )
{
	int			length = 0;
	int			current = 0;
	BOOL bRet = FALSE ;

	switch(mode)
	{
	case en_lmbc_nexon:
		length = static_cast<int>(passport[current]);
		if( 0 < length && length <= 8 )
		{
			current = length;
			strncpy( m_ip, &passport[current], 16 );
			if( m_ip[15] != 0 )
				printf( "ip errorn" ) ;
			current += 16;
		}
		else
			break;

		length = static_cast<int>(passport[current]);
		if( 0 < length && length <= 8 )
		{
			current += length;
			strncpy( m_password, &passport[current], en_lpl_nexon+1 );
			if( m_password[en_lpl_nexon] != 0 )
				printf( "password error\n" ) ;
			current += en_lpl_nexon+1;
			
		}
		else
			break;

		length = static_cast<int>(passport[current]);
		if( 0 < length && length <= 8 )
		{
			current += length;
			strncpy( m_id, &passport[current], en_lil_nexon+1 );
			if( m_id[en_lil_nexon] != 0 )
				printf( "id error\n" );
			current += en_lpl_nexon+1;
		}
		else
			break;
		bRet = TRUE;
		break;
	case en_lmbc_metel:
		break;
	case en_lmbc_vina:
		break;
	case en_lmbc_acclaim:
		break;
	case en_lmbc_pwc:
		break;
	case en_lmbc_rocworks:
		break;
	case en_lmbc_russia:
		break;
	}	

	return bRet;
}

bool Decryption(char * pData, const USHORT usDataLen, const u_char key )
{	

	// Get key string
	char * cValKey ;
	UINT * uiValKey ;
	cValKey = "83kck3ks92 82drd";		
	uiValKey = (UINT*)cValKey ;	

	try {	
		
		int x, i,j,k,l ;
		
		UINT * puiData = (UINT *) pData ;						// 'int *' type data �� ��ȯ		
		
		j = 0 ;							// line step
		k = usDataLen / 16 ;	// line count
		l = usDataLen % 16 ;	// char count (������ ����Ÿ)			
		
		
		// line data block Encrypto
		for( i = 0, x = 0, j = 0 ; i < k ; i++, x += 4, j += 16 ) {	// Key �� pData �� ��ġ�Ѵ�.				
			puiData[x+0] = uiValKey[0] ^ puiData[x+0] ;
			puiData[x+1] = ~(uiValKey[2] ^ puiData[x+1]) ;
			puiData[x+2] = uiValKey[1] ^ puiData[x+2] ;
			puiData[x+3] = ~(uiValKey[3] ^ puiData[x+3]) ;
		}
		
		// char data Encrypto (������ ����Ÿ)
		//j += KEYVALLENTH ;
		for(int h = 0 ; h < l ; h++ ) {
			pData[j+h] = ~(cValKey[h] ^ pData[j+h]) ;
		}

	}// try
	catch (...) {
		return false ;
	}
	return true ;		
}

bool Encryption( IN OUT char * pData, const USHORT usDataLen, const u_char key )
{

	// Get key string
	char * cValKey ;
	UINT * uiValKey ;
	cValKey = "83kck3ks92 82drd";		
	uiValKey = (UINT*)cValKey ;	
	

	try {

		int x, i,j,k,l ;
		
		UINT * puiData = (UINT *) pData ;						// 'int *' type data �� ��ȯ		

		j = 0 ;							// line step
		k = usDataLen / 16 ;	// line count
		l = usDataLen % 16 ;	// char count (������ ����Ÿ)			
	
		
		// line data block Encrypto
		for( i = 0, x = 0, j = 0 ; i < k ; i++, x += 4, j += 16 ) {	// Key �� pData �� ��ġ�Ѵ�.				
			puiData[x+0] = uiValKey[0] ^ puiData[x+0] ;
			puiData[x+1] = ~(uiValKey[2] ^ puiData[x+1]) ;
			puiData[x+2] = uiValKey[1] ^ puiData[x+2] ;
			puiData[x+3] = ~(uiValKey[3] ^ puiData[x+3]) ;
		}
		
		// char data Encrypto (������ ����Ÿ)
		//j += KEYVALLENTH ;
		for(int h = 0 ; h < l ; h++ ) {
			pData[j+h] = ~(cValKey[h] ^ pData[j+h]) ;
		}
		
	} // end try
	catch (...) {
		return false ;
	}	
	return true ;
	
}

#ifdef _XDEF_NEXONLOGIN // Nexon login

#include "NMPCH.h"
#include "NMManager.h"
//#include "NMEventHandler.h"

#pragma warning ( disable:4786 )
#pragma warning ( disable:4018 )

#define		_nmman			CNMManager::GetInstance()
#define		_nmco			CNMCOClientObject::GetInstance()

BOOL	g_LoginPacketAlreadySended = FALSE;

#define WM_MESSENGER_NOTIFY	 (WM_USER + 693 + 7)

#if defined(_XNEXON_WEBLOGIN) || defined(_XDEF_NEXONNEWTESTSERVERLOGIN_071125_KERYGMA)

void _NMLoginErrorMessage( NMLoginAuthReplyCode resultAuth )
{
	g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
	((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
	((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;

	extern BOOL g_OnetimeAutoLogin;
	g_OnetimeAutoLogin = FALSE;

	switch(resultAuth)
	{
	case kLoginAuth_ServiceShutdown			://	���� ����
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_SERVEROFF, TRUE, _XDEF_LS_LOGINFAILED );
		}
		break;
	case kLoginAuth_BlockedIP				://	���ѵ� ������
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "���ѵ� IP�ּ� �Դϴ�. �����ǿ� ������ �ּ���." ), TRUE, _XDEF_LS_LOGINFAILED);
		}
		break;
	case kLoginAuth_NotAllowedLocale		://	�ý��ۿ� ���� �ʴ� ������
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "���� ������ ���� �ʽ��ϴ�. �����ǿ� ������ �ּ���." ), TRUE, _XDEF_LS_LOGINFAILED);			
		}
		break;
	case kLoginAuth_ServerFailed			://	�������� ����
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "���������� ������ �� �����ϴ�. ��� �Ŀ� �ٽ� �õ��� �ּ���." ), TRUE, _XDEF_LS_LOGINFAILED);			
		}
		break;
	case kLoginAuth_WrongID					://	�ش� ���̵� ���ų� ������ ����
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_NOTEXISTACCOUNT, TRUE, _XDEF_LS_LOGINFAILED );			
		}
		break;
	case kLoginAuth_WrongPassword			://	��ȣ�� ���� ����
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_WRONGPASSWD, TRUE, _XDEF_LS_LOGINFAILED );			
		}
		break;
	case kLoginAuth_WrongOwner				://	���� �ƴ� ������ ����
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "���� ���̵�� ���� �ƴ��� Ȯ�εǾ� ����Ͻ� �� �����ϴ�." ), TRUE, _XDEF_LS_LOGINFAILED);			
		}
		break;
	case kLoginAuth_WithdrawnUser			://	Ż���� ȸ��
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "���� ���̵�� Ż���� ���̵�� Ȯ�εǾ� ����Ͻ� �� �����ϴ�." ), TRUE, _XDEF_LS_LOGINFAILED);			
		}
		break;
	case kLoginAuth_UserNotExists			://	�������� �ʴ� ���̵�
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "�������� �ʴ� ���̵� �Դϴ�. �ٽ� Ȯ�� �� �α��� �� �ּ���." ), TRUE, _XDEF_LS_LOGINFAILED);			
		}
		break;
	case kLoginAuth_TempBlockedByLoginFail	://	���� ���� �α��� ���з� ���� �ӽ÷� ��� ����
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "�߸��� ��й�ȣ 3ȸ �Է����� 10�а� �α����� ���ѵ˴ϴ�." ), TRUE, _XDEF_LS_LOGINFAILED);			
		}
		break;
	case kLoginAuth_TempBlockedByWarning	://	��ڿ� ���� �ӽ÷� ��� ����
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "���� ���Ͽ� ���̵� ����� ���ѵǾ����ϴ�." ), TRUE, _XDEF_LS_LOGINFAILED);			
		}
		break;
	case kLoginAuth_BlockedByAdmin			://	��ڿ� ���� ��� ����
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "�����ڿ� ���� ���̵� ����� �������� �Ǿ����ϴ�." ), TRUE, _XDEF_LS_LOGINFAILED);			
		}
		break;
	case kLoginAuth_NotAllowedServer		://	������������ �޾Ƶ����� ����
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "���� ������ �����߽��ϴ�. ��� �Ŀ� �ٽ� �õ��� �ּ���." ), TRUE, _XDEF_LS_LOGINFAILED);			
		}
		break;
	case kLoginAuth_InvalidPassport			://	�н���Ʈ�� ���� ����			
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "�н���Ʈ ������ �����Ͽ����ϴ�. ��� �Ŀ� �ٽ� �õ��� �ּ���." ), TRUE, _XDEF_LS_LOGINFAILED);			
		}
		break;
	case kLoginAuth_ModuleNotInitialized	://	�޽��� ��� �ʱ�ȭ�� �ȵǾ� ����
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "�ؽ� �޽��� ����� �ʱ�ȭ �Ǿ� ���� �ʽ��ϴ�." ), TRUE, _XDEF_LS_LOGINFAILED);			
		}
		break;
	case kLoginAUth_ModuleInitializeFailed	://	�޽��� ��� �ʱ�ȭ�� ����
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "�ؽ� �޽��� ����� �ʱ�ȭ�� �����Ͽ����ϴ�." ), TRUE, _XDEF_LS_LOGINFAILED);			
		}
		break;
	default:
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "�� �� ���� ������ ���Ͽ� �ؽ����� �α��ο� �����Ͽ����ϴ�." ), TRUE, _XDEF_LS_LOGINFAILED);
		}
		break;
	}	
}

#endif


// Nexon messanger login
BOOL _XNetwork::InitNMCO( void )
{
	TCHAR	szDomain[ 128 ];
	//LPTSTR	pChar;

	g_LoginPacketAlreadySended = FALSE;

	_nmman.Init();

	// ����Ī ����� �ƴ� ���, ������ ��Ʈ���� �����Ͽ� ���������� ����մϴ�.
	memset( szDomain, 0, sizeof(TCHAR) * 128 );
	_stprintf( szDomain, _T( "nd%s_%08x" ), g_pLocalUser->m_UserName,::GetTickCount() );

	// �Ʒ��� ���� �ڵ�� �� ���ӿ��� �ο����� �ڵ带 ������ �˴ϴ�.
	// ������ ��Ʈ���� nmco ���̺귯���� �ʱ�ȭ �Ҷ� ������ �����ϴ� �뵵�� ����մϴ�.
	// �� ���ڴ� Ŀ�ǵ������ �� ������ ���ڷ� �׻� �Ѱܹް� �˴ϴ�.
	// uSupportFlag�� �ش� ���ӿ��� �޽����� ���� ���� �� � �������� �����ϴ����� ��Ÿ���ϴ�.
	
#ifdef _XNEXON_WEBLOGIN
	NMLOCALEID	uLocaleId = kLocaleID_KR;
	NMGameCode	uGameCode = kGameCode_ninedragons;
/*
	if ( !_nmco.SetLocale( uLocaleId ) )
	{
		return FALSE;
	}
*/
	if( !_nmco.Initialize( uGameCode ) )
	{
		_XLog( _T( "ERROR : _nmco initialize failed" ) );
		g_MessageBox.SetMessage( _T("�ؽ� �޽����� �ʱ�ȭ �� �� �������ϴ�.[E_Code:0xEC10]") );
		return FALSE;
	}


	// nmco ���κ���, �޽����� ������ ����ϴ� ������ �ڵ�� �޽��� ID�� ����մϴ�.
	// none blocking function��� �� ���� notify �޽������� �޴µ� ����մϴ�
	if ( _nmco.RegisterCallbackMessage( gHWnd, WM_MESSENGER_NOTIFY ) )
	{
		_XLog( _T( "_nmco RegisterCallbackMessage success" ) );
	}
	else
	{
		_XLog( _T( "ERROR : _nmco RegisterCallbackMessage failed" ) );
		g_MessageBox.SetMessage( _T("�ؽ� �޽����� �ʱ�ȭ �� ������ �߻��Ͽ����ϴ�. [E_Code:0xEC11]") );
		return FALSE;
	}

	extern BOOL   g_OnetimeAutoLogin; // �̱� ���� ����.

	if( g_OnetimeAutoLogin )
	{
		TCHAR passportstring[2048];
		memset( passportstring, 0, sizeof(TCHAR) * 2048 );

		int parampos = _XCmdLineOption("passport=",0,8);
		if( parampos != 0 )
		{
			strcpy( passportstring, _XArgv(parampos)+10 );
		}
		
		/*
		FILE* pFile = fopen("NexonPassport.txt","wt");
		fputs(passportstring, pFile);
		fclose( pFile );
		*/

		NMLoginAuthReplyCode resultAuth = _nmco.AttachAuth( passportstring );

		if ( resultAuth == kLoginAuth_OK )
		{
			_XLog( _T( "_nmco AttachAuth success" ) );

			g_MessageBox.SetMessage( _T("�ؽ����Ŀ� �α��� �Ͽ����ϴ�. ������ �����մϴ�.") );
				
			//m_ServerLoginRequestTimeCheckFlag	= TRUE;
			//m_ServerLoginRequestTime			= 0;

			memset( g_pLocalUser->m_UserName, 0, sizeof(TCHAR) * 33 );
			memset( g_pLocalUser->m_UserPW, 0, sizeof(TCHAR) * 33 );
			
			_nmco.GetMyInfo();

			strcpy( g_pLocalUser->m_UserName, _nmco.GetMyLoginId() );
			strcpy( g_pLocalUser->m_UserPW, _T("Unknown") );

			_XLog( "Try to login" );
			g_NetworkKernel.UDPSendPacket( MSG_NO_LOGIN );
		}
		else
		{	
			_XLog( _T("_nmco.AttachAuth failed!") );		
			_NMLoginErrorMessage( resultAuth );
			return FALSE;
		}
		
		// ��� �ڵ��α���. g_OnetimeAutoLogin = FALSE;
	}
	else
	{
		NMLoginAuthReplyCode resultAuth = _nmco.LoginAuth( (LPCTSTR)g_pLocalUser->m_UserName, (LPCTSTR)g_pLocalUser->m_UserPW );

		if ( resultAuth == kLoginAuth_OK )
		{
			_XLog( _T( "_nmco LoginAuth success" ) );

			g_MessageBox.SetMessage( _T("�ؽ����Ŀ� �α��� �Ͽ����ϴ�. ������ �����մϴ�.") );
				
			//m_ServerLoginRequestTimeCheckFlag	= TRUE;
			//m_ServerLoginRequestTime			= 0;

			_XLog( "Try to login" );
			g_NetworkKernel.UDPSendPacket( MSG_NO_LOGIN );
		}
		else
		{
			_XLog( _T("_nmco.LoginAuth failed!") );		
			_NMLoginErrorMessage( resultAuth );

			return FALSE;			
		}
	}

#else

	#ifdef	_XDEF_NEXONNEWTESTSERVERLOGIN_071125_KERYGMA

		NMLOCALEID	uLocaleId = kLocaleID_KR;
		NMGameCode	uGameCode = kGameCode_ninedragons;
	/*
		if ( !_nmco.SetLocale( uLocaleId ) )
		{
			return FALSE;
		}
	*/
		if( !_nmco.Initialize( uGameCode ) )
		{
			_XLog( _T( "ERROR : _nmco initialize failed" ) );
			g_MessageBox.SetMessage( _T("�ؽ� �޽����� �ʱ�ȭ �� �� �������ϴ�.[E_Code:0xEC10]") );
			return FALSE;
		}


		// nmco ���κ���, �޽����� ������ ����ϴ� ������ �ڵ�� �޽��� ID�� ����մϴ�.
		// none blocking function��� �� ���� notify �޽������� �޴µ� ����մϴ�
		if ( _nmco.RegisterCallbackMessage( gHWnd, WM_MESSENGER_NOTIFY ) )
		{
			_XLog( _T( "_nmco RegisterCallbackMessage success" ) );
		}
		else
		{
			_XLog( _T( "ERROR : _nmco RegisterCallbackMessage failed" ) );
			g_MessageBox.SetMessage( _T("�ؽ� �޽����� �ʱ�ȭ �� ������ �߻��Ͽ����ϴ�. [E_Code:0xEC11]") );
			return FALSE;
		}

		
		NMLoginAuthReplyCode resultAuth = _nmco.LoginAuth( (LPCTSTR)g_pLocalUser->m_UserName, (LPCTSTR)g_pLocalUser->m_UserPW );
		if ( resultAuth == kLoginAuth_OK )
		{
			_XLog( _T( "_nmco LoginAuth success" ) );

			g_MessageBox.SetMessage( _T("�ؽ����Ŀ� �α��� �Ͽ����ϴ�. ������ �����մϴ�.") );
				
			//m_ServerLoginRequestTimeCheckFlag	= TRUE;
			//m_ServerLoginRequestTime			= 0;

			_XLog( "Try to login" );
			g_NetworkKernel.UDPSendPacket( MSG_NO_LOGIN );
		}
		else
		{
			// �α��� ����

			_XLog( _T("_nmco.LoginAuth failed!") );		
			_NMLoginErrorMessage( resultAuth );

			return FALSE;			
		}

	#else
		#ifdef	_XDEF_NEWNEXONLOGIN

			if( _nmco.InitClientObject( kGameCode_ninedragons, szDomain, 
										//kSupportFlag_AddFriendConfirm |
										//kSupportFlag_ChatSession |
										//kSupportFlag_GeneralNoteMsg |
										//kSupportFlag_GameInviteNoteMsg |
										//kSupportFlag_ItemGiftNoteMsg |
										//kSupportFlag_GuildNoticeNoteMsg |
										//kSupportFlag_GuildChatRoomInviteNoteMsg |
										//kSupportFlag_WarningNoteMsg |
										//kSupportFlag_GuildNote |
										//kSupportFlag_GuildChatRoom |
										kSupportFlag_ServerMessage, FALSE ) )
			{
				_XLog( _T( "_nmco initialize success" ) );
			}
			else
			{
				_XLog( _T( "ERROR : _nmco initialize failed" ) );

				g_MessageBox.SetMessage( _T("�ؽ� �޽����� �ʱ�ȭ �� �� �������ϴ�.[E_Code:0xEC10]") );

				return FALSE;
			}
		#else
			
			if ( _nmco.Init( kGameCode_ninedragons, szDomain, 
					//kSupportFlag_AddFriendConfirm |
					//kSupportFlag_ChatSession |
					//kSupportFlag_GeneralNoteMsg |
					//kSupportFlag_GameInviteNoteMsg |
					//kSupportFlag_ItemGiftNoteMsg |
					//kSupportFlag_GuildNoticeNoteMsg |
					//kSupportFlag_GuildChatRoomInviteNoteMsg |
					//kSupportFlag_WarningNoteMsg |
					//kSupportFlag_GuildNote |
					//kSupportFlag_GuildChatRoom |
					kSupportFlag_ServerMessage ) )	
			{
				_XLog( _T( "_nmco initialize success" ) );
			}
			else
			{
				_XLog( _T( "ERROR : _nmco initialize failed" ) );

				g_MessageBox.SetMessage( _T("�ؽ� �޽����� �ʱ�ȭ �� �� �������ϴ�.[E_Code:0xEC10]") );

				return FALSE;
			}
		#endif

			// nmco ���κ���, �޽����� ������ ����ϴ� ������ �ڵ�� �޽��� ID�� ����մϴ�.
			// none blocking function��� �� ���� notify �޽������� �޴µ� ����մϴ�
			if ( _nmco.RegisterCallbackMessage( gHWnd, WM_MESSENGER_NOTIFY ) )
			{
				_XLog( _T( "_nmco RegisterCallbackMessage success" ) );
			}
			else
			{
				_XLog( _T( "ERROR : _nmco RegisterCallbackMessage failed" ) );

				g_MessageBox.SetMessage( _T("�ؽ� �޽����� �ʱ�ȭ �� ������ �߻��Ͽ����ϴ�. [E_Code:0xEC11]") );
				return FALSE;
			}

			_XLog( _T( "Try to login..." ) );
			
			if( _nmco.Login( (LPCTSTR)g_pLocalUser->m_UserName, (LPCTSTR)g_pLocalUser->m_UserPW ) )
			{
				_XLog( _T( "_nmco login success" ) );
			}
			else
			{
				_XLog( _T( "ERROR : _nmco login failed" ) );
				g_MessageBox.SetMessage( _T("�ؽ� �޽����� �α����� �� �� �������ϴ�. [E_Code:0xEC12]") );
			}
	#endif // #ifdef else	_XDEF_NEXONNEWTESTSERVERLOGIN_071125_KERYGMA
#endif //#ifdef else _XNEXON_WEBLOGIN

		g_nmcoInit = TRUE;
		return TRUE;
}

BOOL _XNetwork::ReleaseNMCO( void )
{
#if defined(_XNEXON_WEBLOGIN) || defined(_XDEF_NEXONNEWTESTSERVERLOGIN_071125_KERYGMA)

	if(g_nmcoInit)
	{
		BOOL bResult = _nmco.LogoutAuth();
		_XLog( _T("_nmco LogoutAuth %d"), bResult );

		_nmco.ResetCallbackMessage( gHWnd );
		_nmman.Finalize();

		g_nmcoInit = FALSE;
	}

#else
	
	if(g_nmcoInit)
	{
		_XLog("mystatus : %d", _nmco.GetMyStatus());

		// status�� ������ ���� - �ϴ� üũ ���� �ʵ���
//		if ( _nmco.GetMyStatus() != kStatus_Offline )
		{
			_XLog("Logout nmco");

			_nmco.Logout();
		}

		_nmman.Finalize();

		g_nmcoInit = FALSE;
	}
#endif

	return TRUE;
}

#endif

_XNetwork::_XNetwork()
{
	// Init data member -----------------------------------------------------------------------------------------				
	
	m_hLoginUDPSocket	=	NULL;
	m_LOGINSERVER_PORT	=	0;
	m_bLoginServerConnected = FALSE;
	m_LoginProcessMode	=	_XDEF_LOGINPROCESS_READY;
	
	m_hClientSocket		=	NULL;
	m_bConnected		=	FALSE;	
	m_SERVER_PORT		=	0;
	
	m_NextSERVER_ADDRESS = 0;
	m_NextSERVER_PORT    = 0;
	
	m_LastConfirmID		=	-1;
	
	m_hGetHostTask		=	NULL;
	memset( m_HostBuffer, 0, sizeof(TCHAR)*MAXGETHOSTSTRUCT );
	
	//g_ServerAddressManager.GetServerInfo();
	//g_PatchVersionManager.GetVersionInfo();

	memset( m_LOGINSERVER_ADDRESS, 0, sizeof(TCHAR)*32 );
	//strcpy( m_LOGINSERVER_ADDRESS, g_ServerAddressManager.m_ServerInfo.serveraddress );	
	//m_LOGINSERVER_PORT = g_ServerAddressManager.m_ServerInfo.serverport;
	
	m_hMessengerSocket = NULL;
	m_MESSENGERSERVER_PORT = 0;
	m_bMessengerServerConnected = FALSE;
	m_bMessengerServerLoginSuccess = FALSE;
	m_ConnectTryCount= 0;
	
	m_MESSENGERSERVER_ADDRESS = 0;
	m_MESSENGERSERVER_PORT = 0;
	
	InitializeMessengerPacketBuffer();
	for( int i = 0; i < 50; ++i )
	{
		m_bMessengerInstanceMessagePosition[i] = FALSE;
	}
	
	memset( m_RunningQuestData, 0, sizeof(_sRunningQuestPacketData)*_MAX_CURQUEST_NUM_ );

	m_bDeleteCharacter = FALSE;
}

_XNetwork::~_XNetwork()
{

}

//////////////////////////////////////////////////////////////////////
// Initialize section...

BOOL _XNetwork::InitializeNetwork( void )
{	
	g_ServerAddressManager.GetServerInfo();
	g_PatchVersionManager.GetVersionInfo();

	memset( m_LOGINSERVER_ADDRESS, 0, sizeof(TCHAR)*32 );
	strcpy( m_LOGINSERVER_ADDRESS, g_ServerAddressManager.m_ServerInfo.serveraddress );	
	m_LOGINSERVER_PORT = g_ServerAddressManager.m_ServerInfo.serverport;

	InitializePacketBuffer();

#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode )
	{
		return TRUE;
	}
#endif

	// Check winsock version-------------------------------------------------------------------------------------
	WORD		wVersion = MAKEWORD(2, 2);
    WSADATA		wd;
	int			nError;

    nError = WSAStartup(wVersion, &wd);

	if (nError != 0)
	{		
		_XFatalError( "Can't initialize winsock. erro=%d", nError );
		return FALSE;
	}

	if (LOBYTE(wd.wVersion) != 2 || HIBYTE(wd.wVersion) != 2)
	{
		_XFatalError( "Can't find winsock 2.2 dll" );
		return FALSE;
	}		

	_XDWINPRINT( "Network initialize..." );
	
#ifdef _XDEF_MADSYSTEM
	if( !g_MadWrapper.Initialize() )
	{
		return FALSE;
	}
#endif

	g_PacketCrypto.InitSeqNum( 0 );

#ifdef _XVIETNAMESE
	extern BOOL _XCheckLoginServerIP( LPCTSTR szHostName );
	if( _XCheckLoginServerIP( _T("login.cuulong.com.vn") ) )
	{
		_XFatalError( "Can't initialize socket. error=0x%x", 0xFE32 );
		return FALSE;
	}
#endif
		
	return TRUE;
}

BOOL _XNetwork::ReleaseNetwork( void )
{
#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode )
	{
		return TRUE;
	}
#endif

#ifdef _XDEF_MADSYSTEM
	g_MadWrapper.Release();
#endif
	
 	DisconnectServer();
	ReleaseUDPSocket();

	ReleaseMessengerSocket();

	WSACleanup();
	
	m_hLoginUDPSocket	=	NULL;
	m_LOGINSERVER_PORT	=	0;
	m_bLoginServerConnected = FALSE;
	m_LoginProcessMode	=	_XDEF_LOGINPROCESS_READY;
	
	m_hClientSocket		=	NULL;
	m_bConnected		=	FALSE;	
	m_SERVER_PORT		=	0;

	m_hMessengerSocket = NULL;
	m_MESSENGERSERVER_PORT = 0 ;
	m_bMessengerServerConnected = FALSE;
	m_bMessengerServerLoginSuccess = FALSE;
	m_ConnectTryCount= 0;
	
	m_MESSENGERSERVER_ADDRESS = 0;
	m_MESSENGERSERVER_PORT = 0;

	m_LastConfirmID		=	-1;
		
	m_hGetHostTask		=	NULL;
	memset( m_HostBuffer, 0, sizeof(TCHAR)*MAXGETHOSTSTRUCT );/**/
	
	//g_ServerAddressManager.GetServerInfo();
	//g_PatchVersionManager.GetVersionInfo();

	memset( m_LOGINSERVER_ADDRESS, 0, sizeof(TCHAR)*32 );
	strcpy( m_LOGINSERVER_ADDRESS, g_ServerAddressManager.m_ServerInfo.serveraddress );	
	m_LOGINSERVER_PORT = g_ServerAddressManager.m_ServerInfo.serverport;
	
	InitializePacketBuffer();
	InitializeMessengerPacketBuffer();

	g_PacketCrypto.InitSeqNum( 0 );

	_XDWINPRINT( "Network released..." );

	return TRUE;
}


void _XNetwork::InitializePacketBuffer( void )
{
	g_nPI = 0;
	g_nRI = 0;
	memset( g_szPacket, 0, sizeof(BYTE) * _XDEF_PACKET_MAXBUFFER );
	g_sPacketLen = 0;
	g_mode = _X_PACKET_RECIEVE_PACKET_LENGTH;

	m_MovePacket_lastSendedTime = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
////                             �α��� ���� ���� �κ�                                    ////
//////////////////////////////////////////////////////////////////////////////////////////////

BOOL _XNetwork::InitializeUDPSocket( void )
{

#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode )
	{
		return TRUE;
	}
#endif

	ReleaseUDPSocket();
	InitializePacketBuffer();

	m_LoginProcessMode	=	_XDEF_LOGINPROCESS_READY;
	
	m_hLoginUDPSocket = socket(PF_INET, SOCK_STREAM, 0);
	
	if (m_hLoginUDPSocket == INVALID_SOCKET) 
	{
		m_hLoginUDPSocket = NULL;
		_XFatalError( "Can't create login socket." );
		return FALSE;
	}
	
	struct sockaddr_in addr;
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = 0;
	
	_XDWINPRINT( "Connecting login server" );
	
	if( WSAAsyncSelect(m_hLoginUDPSocket, gHWnd, WM_USER_UDPSOCKET, FD_CONNECT) == SOCKET_ERROR )
	{
		int nError = WSAGetLastError();
	
		_XFatalError( _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILSELECT), nError);
		return FALSE;
	}

	TCHAR loginipadress[256];
	memset( loginipadress, 0, sizeof(TCHAR) * 256 );

	
	if( !_XIsIPv4Address( m_LOGINSERVER_ADDRESS, strlen(m_LOGINSERVER_ADDRESS) ) )
	{
		if( !_XGetHostName( m_LOGINSERVER_ADDRESS, loginipadress, 256 ) )
		{
			_XFatalError( "Failed to get host name" );
			return FALSE;
		}
	}
	else
	{
		strcpy( loginipadress, m_LOGINSERVER_ADDRESS );
	}

	#ifdef _X_US_EXPANDSERVERLIST

	XProc_LoginServer* pLoginProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver;
	if( pLoginProcess->m_LastSelectedRegion_Server == 1)
	{
		// login address ���� : 2008.05.15 �ڼ���
		strcpy( loginipadress, _T("85.17.144.236") );
//		strcpy( loginipadress, _T("85.17.144.237") );
	}

	#endif
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_LOGINSERVER_PORT);
	addr.sin_addr.s_addr = inet_addr( loginipadress ); 
	
	//_XLog( "Login server : %s:%d", m_LOGINSERVER_ADDRESS, m_LOGINSERVER_PORT );
	
	if (connect(m_hLoginUDPSocket, (struct sockaddr*) &addr, sizeof(addr)) == SOCKET_ERROR) 
	{
		int nError = WSAGetLastError();
		
		if( nError != WSAEWOULDBLOCK )
		{
			_XFatalError( _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILCONNECT), nError);
			closesocket( m_hLoginUDPSocket ) ;
			return FALSE;
		}
	}
	
	_XDWINPRINT( "Connected login server..." );

	return TRUE;
}

BOOL _XNetwork::ReleaseUDPSocket( void )
{	
#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode )
	{
		return TRUE;
	}
#endif

	//WSAAsyncSelect(m_hMessengerSocket, gHWnd, WM_USER_UDPSOCKET, 0 );

	m_LoginProcessMode	=	_XDEF_LOGINPROCESS_READY;

	if( m_hLoginUDPSocket )
	{
		shutdown(m_hLoginUDPSocket, SD_BOTH);
		closesocket(m_hLoginUDPSocket);
		m_hLoginUDPSocket = NULL;
	}

	m_bLoginConnected = FALSE;

	if( !m_hClientSocket )
	{
		g_PacketCrypto.InitSeqNum( 0 );
	}

	_XDWINPRINT( "Login socket released..." );

	return TRUE;	
}

void _XNetwork::UDPReadPacket( void )
{
	if( !m_bLoginConnected ) return;
	
#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode ) return;
#endif	
	_XDWPACKETLOG( "{## recieve login socket message" );
	
#define _XDEF_LOGINPACKETPARSING_
#ifdef  _XDEF_LOGINPACKETPARSING_
	int		 nRead;
	_sHeader head;
		
	while ((nRead = recv(m_hLoginUDPSocket, (char*)g_szPacket + g_nRI, _XDEF_PACKET_MAXBUFFER - g_nRI, 0)) > 0)
	{   
		g_nRI += nRead;

		//*(g_szPacket + g_nRI) = 0;

		while( g_nPI < g_nRI ) 
		{
			if(g_mode == _X_PACKET_RECIEVE_PACKET_LENGTH) 
			{
				if( g_nPI < g_nRI-1 ) 
				{
					memcpy( &head, (g_szPacket+g_nPI), sizeof(_sHeader) );
					
					if( head.sLength <= 0 )
					{
						g_sPacketLen = 0;
						_XLog( "WARNING : Login Packet header invalid length [%d %d %d]", head.sLength, head.sCrypto, head.sCompressed );
#ifdef _XDWDEBUG
 						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
						g_MessageBox.SetMessage( "WARNING : Login packet header invalid length [%d %d %d]", head.sLength, head.sCrypto, head.sCompressed );
						g_MessageBox.ShowWindow( TRUE );
#endif
						break;
					}
					else
					{
						g_sPacketLen = head.sLength;
						g_mode = _X_PACKET_RECIEVE_PACKET_BODY;
					}
				}
				else 
				{
					break;
				}
			}
			else if (g_mode == _X_PACKET_RECIEVE_PACKET_BODY) 
			{
				if (g_nPI + g_sPacketLen <= g_nRI)
				{
					ParsingLoginPacket(g_szPacket + g_nPI, g_sPacketLen);
					g_nPI += g_sPacketLen;
					g_mode = _X_PACKET_RECIEVE_PACKET_LENGTH;
				}
				else
				{
					break;
				}
			}
		}
		if (g_nPI == g_nRI) 
		{
			g_nPI = g_nRI = 0;
		}
		else if (g_nPI > 0) 
		{
			if( g_nRI - g_nPI < 1 )
			{
				if( g_nRI - g_nPI < 0 )
				{
					InitializePacketBuffer();
					_XLog( "WARNING : Invalid Incompletion login packet size [RI : %d / PI : %d]", g_nRI, g_nPI );
				}
				else
				{
					_XLog( "WARNING : Invalid Incompletion login packet size zero [RI : %d / PI : %d]", g_nRI, g_nPI );
				}
#ifdef _XDWDEBUG
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
				g_MessageBox.SetMessage( "WARNING : Invalid Incompletion login packet size [RI : %d / PI : %d]", g_nRI, g_nPI );
				g_MessageBox.ShowWindow( TRUE );
#endif				
			}
			else
			{
				if( g_nRI >= _XDEF_PACKET_MAXBUFFER )
				{
					InitializePacketBuffer();
#ifdef _XDWDEBUG
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
					g_MessageBox.SetMessage( "WARNING : Overflow Incompletion login packet size [%d]", g_nRI );
					g_MessageBox.ShowWindow( TRUE );
#endif
					_XLog( "WARNING : Overflow Incompletion login packet size [%d]", g_nRI );
				}
				else
				{
					int leftsize = g_nRI - g_nPI;

					BYTE	g_szTempPacket[_XDEF_PACKET_MAXBUFFER];
					memset( g_szTempPacket, 0, _XDEF_PACKET_MAXBUFFER );
					memcpy( g_szTempPacket, g_szPacket + g_nPI, leftsize );
					memset( g_szPacket, 0, _XDEF_PACKET_MAXBUFFER );
					memcpy( g_szPacket, g_szTempPacket, leftsize );

					//memmove(g_szPacket, g_szPacket + g_nPI, g_nRI - g_nPI);	
					
					g_nRI = leftsize;
					g_nPI = 0;

					if( g_nRI <= -1 )
					{
#ifdef _XDWDEBUG
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
						g_MessageBox.SetMessage( "WARNING : Invalid login packet recieve point  [%d]", g_nRI );
						g_MessageBox.ShowWindow( TRUE );
#endif
						_XLog( "WARNING : Invalid login packet recieve point  [%d]", g_nRI );
					}
				}
			}
		}
	}
#else
	int nRead = 0 ;
	char buf[255] ;
	nRead = recv(m_hLoginUDPSocket, buf, 255, 0) ;
	if( nRead == SOCKET_ERROR || nRead == 0 )
	{
		ProcessNetworkError();
		
		g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
		g_MessageBox.SetMessage( _XGETINTERFACETEXT((ID_STRING_NETWORK_FAILRECEIVEDATA), WSAGetLastError() );
		g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
		_XUpdateWindow();		
		
		return;
	}
	
	ParsingLoginPacket( buf );
#endif		
	_XDWPACKETLOG( "  End of recieve login message ##}" );	
}


void _XNetwork::ParsingLoginPacket(LPBYTE pPacket, UINT packetsize)
{
#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode ) return;
#endif
		
	_XDWPACKETLOG( "Parsing recieved login message Type : [%d]", pPacket[2] );
	
	switch ( pPacket[2] )
	{
	// Crypto sequence number
	case MSG_NO_SEQUENCE_NUMBER :
		{
			MSG_SEQUENCE_NUMBER * pDequenceNumber = (MSG_SEQUENCE_NUMBER*)pPacket;
			g_PacketCrypto.InitSeqNum( pDequenceNumber->ucStartSeq );

#ifdef _XDEF_NEXONLOGIN

#ifdef _XDEF_NEXONLOGINFINAL	// �ؽ� �޽��� �α���
			if(strncmp(m_LOGINSERVER_ADDRESS, _T("221.147.34."), 11) != 0)
			{
				if(strncmp(m_LOGINSERVER_ADDRESS, _T("10.10.0."), 8) != 0) //Author : �����// ���� �˻�,,
                    InitNMCO();
                else
                {
                    //�系 ����
                    g_LoginPacketAlreadySended = FALSE;
				    UDPSendPacket(MSG_NO_LOGIN);
                }
			}
			else
			{
				// ������õ
				g_LoginPacketAlreadySended = FALSE;
				UDPSendPacket(MSG_NO_LOGIN);
			}
#else
			g_LoginPacketAlreadySended = FALSE;			
			
			UDPSendPacket(MSG_NO_LOGIN);
#endif

#else			
			UDPSendPacket( MSG_NO_LOGIN );
#endif
			_XLog( "\n" );
		}
		break;
	case MSG_NO_LOGIN_SUCCESS:
		{
			MSG_LOGIN_SUCCESS * pLogin ;
			pLogin = (MSG_LOGIN_SUCCESS*)pPacket ;

			g_pLocalUser->m_Uniqueid	= pLogin->usUniqueID;
			m_SERVER_ADDRESS			= pLogin->ulServerIP;
			m_SERVER_PORT				= pLogin->usServerPort;

//			#ifdef _XDWDEBUG
//				struct sockaddr_in addr;
//				addr.sin_family = AF_INET;
//				addr.sin_port = htons(m_SERVER_PORT);
//				addr.sin_addr.s_addr = m_SERVER_ADDRESS; 
//
//				TCHAR ipaddress[256];
//				memset( ipaddress, 0, sizeof(TCHAR) * 256 );
//				strcpy( ipaddress, inet_ntoa( addr.sin_addr ) );
//			#endif			


			m_MESSENGERSERVER_ADDRESS	= pLogin->ulMessengerIP;
			m_MESSENGERSERVER_PORT		= pLogin->usMessengerPort;

			//_XLog( "Messager port : %d", m_MESSENGERSERVER_PORT );

#ifdef _PD_LOGIN_PACKET_MODE_

#if defined(_XDEF_ITEMMALL_AUTOLOGIN_20061120) || defined(_XDEF_ITEMMALL_AUTOLOGIN_SE_20070514)
			// ������ ���� ������ �� �ִ� ���̵�� ó�� �Է��� ID�̹Ƿ�
			// �������� ���ο� ID�� �ޱ� ���� �����Ѵ�.
			memset( g_pLocalUser->m_ItemMallWebLoginID, 0, sizeof(TCHAR) * 33 );
			strcpy(g_pLocalUser->m_ItemMallWebLoginID, g_pLocalUser->m_UserName);
			
			int servernumber = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
			
			TCHAR tempStr[256];
			memset( tempStr, 0, sizeof(TCHAR)*256 );

	#ifdef _XDEF_ITEMMALL_AUTOLOGIN_SE_20070514
			TCHAR postdata[128];			
			_snprintf( postdata, sizeof(postdata), "AL_ID=%s&AL_PWD=%s&AL_Server=%d", g_pLocalUser->m_ItemMallWebLoginID, g_pLocalUser->m_UserPW, servernumber );
			_XDisplayHTMLPageAndPostData(g_hItemMallWebBrowserObject, _T("http://global1.indy21.co.kr:8080"), postdata );
	#else
			if( !g_bTestWebPageAddress )
				_snprintf( tempStr, sizeof(tempStr), "http://ninemall.nexon.com/?user_id=%s&select_server=%d", g_pLocalUser->m_ItemMallWebLoginID, servernumber );
			else
				_snprintf( tempStr, sizeof(tempStr), "http://panda.nexon.com/?user_id=%s&select_server=%d", g_pLocalUser->m_ItemMallWebLoginID, servernumber );			

			DisplayHTMLPage(g_hItemMallWebBrowserObject, tempStr);
	#endif			
			
#endif	//_XDEF_ITEMMALL_AUTOLOGIN_20061120

			strcpy(g_pLocalUser->m_UserName, pLogin->cNDAccount);
#endif
						
			_XDWINPRINT( "Login success[UID: %d]", pLogin->usUniqueID );			
			_XDWINPRINT( "Connect to game server, IP: %ld, PORT: %d", pLogin->ulServerIP, pLogin->usServerPort );
			
#ifdef _XUSE_GAMEGUARD
	#ifdef _XJAPANESE
			g_GameGuardInstance.Send( g_pLocalUser->m_UserName );
	#endif
#endif

			SendMessage( gHWnd, WM_COMMAND, _XDEF_LS_DOCONNECTGAMESERVER, NULL );
			
			m_LoginProcessMode = _XDEF_LOGINPROCESS_LOGINSUCCESS;

			SendXTrapSetOptGameInfo();
			
			_XLog( "Login success." );
		}
		break ;
		
	case MSG_NO_LOGIN_FAIL:
		{
			MSG_LOGIN_FAIL * pFail ;
			pFail = (MSG_LOGIN_FAIL*)pPacket ;

			_XLog( "Error : Failed to login [%d]",pFail->ucErrorCode );
			
			switch( pFail->ucErrorCode )	
			{
			case _LOGIN_ERROR_CODE_BADUSER : 	_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Invalid ID>" );				
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_INCORRECTID, TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				break;
			case _LOGIN_ERROR_CODE_BLOCK :	_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Block account>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK,ID_STRING_NETWORK_BLOCKACCOUNT, TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_OVERTIME :	_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <time out>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_OVERTIME,TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_BADPASSWORD :	_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Invalid password>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_WRONGPASSWD , TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				break;
			case _LOGIN_ERROR_CODE_ALREADY :	_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Already login>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_ALREADYLOGINUSER, TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;						
			case _LOGIN_ERROR_CODE_CHECKUP :	_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Inspecting server>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_INSPECTSERVER, TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				break;				
			case _LOGIN_ERROR_CODE_OFF :	_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Inspecting server>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_SERVEROFF, TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				break;
			case _LOGIN_ERROR_CODE_NO_USER : _XDWINPRINT( "RECIEVE MESSAGE : Failed to login <not exist account>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_NOTEXISTACCOUNT, TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				break;
			case _LOGIN_ERROR_CODE_TEEN_USER :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <teen user>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_TEENUSER, TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );				
				break;
			case _LOGIN_ERROR_CODE_BADVERSION :	_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Invalid client version>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK,  ID_STRING_NETWORK_DIFFVERSION, TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_SERVERISFULL :	_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Too many users>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK,  ID_STRING_NETWORKTWO_TOOMANYUSERS, TRUE, _XDEF_LS_LOGINFAILED );				
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_BILL_NO_INFO :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <No account info>" );				
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NEW_2913), TRUE, _XDEF_LS_LOGINFAILED );//_T("���� ������ �����ϴ�.")
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_BILL_NO_MONEY :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <No payment>" );				
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NEW_2914), TRUE, _XDEF_LS_LOGINFAILED ); //_T("����ð��� ��� ����ϼ̰ų� �����Ⱓ�� ����Ǿ����ϴ�.")
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_NOT_LMBC_CODE :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Invalid local code>" );				
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("ERROR : Invalid local code."), TRUE, _XDEF_LS_LOGINFAILED ); 
				g_MessageBox.SetMessageColor( _XSC_WARNING );				
				break;
			case _LOGIN_ERROR_CODE_NOT_UNI_NEXON :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Not unificated id>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("�ؽ� ���� ������ �ƴմϴ�. Ȩ�������� �����Ͽ� ������ �����Ͻñ� �ٶ��ϴ�."), TRUE, _XDEF_LS_LOGINFAILED ); 
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_NOT_PCBANG :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Not PCbang user>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3049), TRUE, _XDEF_LS_LOGINFAILED ); 
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_BLOCK_CODE_1  :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <account block type 1>" );				
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NEW_2915), TRUE, _XDEF_LS_LOGINFAILED );//_T("�ش� ������ ���� ���� �����Դϴ�. �ڼ��� ������ GM���� ���� �Ͽ� �ֽñ� �ٶ��ϴ�.")
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_BLOCK_CODE_2  :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <account block type 1>" );				
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "),  _XGETINTERFACETEXT(ID_STRING_NEW_2916), TRUE, _XDEF_LS_LOGINFAILED ); //_T("�ش� ������ ���� �з� �����Դϴ�. �ڼ��� ������ GM���� ���� �Ͽ� �ֽñ� �ٶ��ϴ�.")
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_BLOCK_CODE_3  :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <account block type 1>" );				
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NEW_2917), TRUE, _XDEF_LS_LOGINFAILED ); //_T("�ش� ������ �ӽ� ���� ���� �����Դϴ�. �ڼ��� ������ GM���� ���� �Ͽ� �ֽñ� �ٶ��ϴ�.")
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_METEL_E50 :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <SQL error>" );				
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NEW_3054), TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_METEL_E51 :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <AP exception>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NEW_3055), TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_METEL_E52 :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Incorrect ID or Password>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NEW_3056), TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_METEL_E53 :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <User has not registered the game>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NEW_3057), TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_METEL_E54 :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <This account is forbidden>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NEW_3058), TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_METEL_E55 :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Points is not enough to play>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NEW_3059), TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_METEL_E56 :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <The game registration is forbidden>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NEW_3060), TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			case _LOGIN_ERROR_CODE_METEL_E57 :
				_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Can't find the LoginSN in DB>" );
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NEW_3061), TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;
			default:
			case _LOGIN_ERROR_CODE_ETC :	_XDWINPRINT( "RECIEVE MESSAGE : Failed to login <Unknown error>" );				
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_UNKNOWNNETWORKERROR, TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( _XSC_WARNING );
				break;			
			}						
			//  close login request dlg
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.CloseLoginRequestDialog();
			
			ReleaseUDPSocket();
			g_NetworkKernel.InitializeNetwork();
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ConnectMode = _XDEF_SERVERCONNECTMODE_LOGIN;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_LoginProcess = LOGINPROC_START;
			
			m_LoginProcessMode = _XDEF_LOGINPROCESS_READY; //_XDEF_LOGINPROCESS_LOGINFAILED;
		}
		
		break;
		
	default :
		{
			TCHAR messagebuffer[256];
			memset(messagebuffer, 0, sizeof(messagebuffer));
			_snprintf( messagebuffer, sizeof(messagebuffer), "Network error : invalid login packet number [%d]", pPacket[2] );
			g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK,  _T(""), messagebuffer, TRUE, _XDEF_LS_LOGINFAILED );

			_XLog( "Error : Network error : invalid login p/n [%d]", pPacket[2] );
		}
		break;
	}
}



BOOL _XNetwork::UDPSendPacket( int type, int param1, int param2 )
{	
	_XLog("LS Send packet start [%d]", m_bLoginConnected);

	if( !m_bLoginConnected ) return FALSE;
	
#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode ) return TRUE;
#endif

	int ret = 0;
	switch( type )
	{
	case MSG_NO_LOGIN:
		{

#ifdef _PD_LOGIN_PACKET_MODE_

	#ifdef _XDEF_NEXONLOGIN // Nexon login
			if( g_LoginPacketAlreadySended )
			{
				_XLog( "Login : already sended" );
				return TRUE;
			}

			g_LoginPacketAlreadySended = TRUE;
	#endif
			_XLog( "Login : send login packet" );

			MSG_LOGIN Login;
			memset( &Login, 0, sizeof(Login) );
			Login.header.sLength = sizeof(Login);
			Login.header.sCrypto = true;
			Login.header.sCompressed = false;
			
	#ifdef _XADMINISTRATORMODE
			if( _XCmdLineOption("xgmlogin",0) != 0 )
			{
				Login.ucMessage = MSG_NO_GMLOGIN;
			}
			else
			{
				Login.ucMessage = MSG_NO_LOGIN;
			}
	#else
			Login.ucMessage = MSG_NO_LOGIN;
	#endif
			
			g_PatchVersionManager.GetVersionInfo();

	#ifdef _XDEF_NEXONLOGIN
			Login.uiLMBC_Code = en_lmbc_nexon;
	#else
		#ifdef _XVIETNAMESE
			Login.uiLMBC_Code = en_lmbc_vina;
		#else
			#ifdef _XTAIWANESE
			Login.uiLMBC_Code = en_lmbc_metel;
			#else
				#ifdef _XENGLISH
					#ifdef _ACCLAIM_VERSION
						Login.uiLMBC_Code = en_lmbc_acclaim;
					#else // PWC Version
						Login.uiLMBC_Code = en_lmbc_pwc;
					#endif
				#else
					#ifdef _XJAPANESE
						Login.uiLMBC_Code = en_lmbc_rocworks;
					#else
						#ifdef _XRUSSIAN
							Login.uiLMBC_Code = en_lmbc_russia;
						#endif
					#endif
				#endif
			#endif
		#endif
	#endif

			Login.sVersionIndex = g_PatchVersionManager.m_VersionInfo.patchversion_1;

			Login.dwClientVer = g_PatchVersionManager.GetExeVersion();
			
			/*
			_XLog( "local version = %d.%d.%d.%d,%d,%d", g_PatchVersionManager.m_VersionInfo.exeversion_1,
												  g_PatchVersionManager.m_VersionInfo.exeversion_2,
												  g_PatchVersionManager.m_VersionInfo.exeversion_3,
												  g_PatchVersionManager.m_VersionInfo.exeversion_4, Login.dwClientVer,Login.sVersionIndex);*/
												  

			if( _XCmdLineOption("xuseoldversion",0) != 0 )
			{
				Login.dwClientVer = 393336;
			}
			
//#if defined(_XNEXON_WEBLOGIN) // ���α��ΰ� �� ���� �׽�Ʈ ���� �α����� ��� ������ ������.
//			Login.dwClientVer = 393336;
//#endif

	#ifdef _XTESTSERVER
			if( _XCmdLineOption("xinnertestserverlogin",0) == 0 )
			{
				Login.cForestSelectNum = 1;//_COUNT_MAX_GROUP_-1;
			}
			else
			{
				Login.cForestSelectNum = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
			}
	#else
			Login.cForestSelectNum = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
			
			// �ӽ�....���� Ŭ���̾�Ʈ�� �׽�Ʈ���� ���ӽ� ���
			if( _XCmdLineOption("xforcetestserverconnect",0) != 0 )
			{
				Login.cForestSelectNum = _COUNT_MAX_GROUP_-1;
			}
	#endif

			char hostname[256];
			memset(hostname, 0, sizeof(hostname));
/*
#ifdef _XDEF_OLDIP
			gethostname(hostname, 256);			

			if( strlen(hostname) <= 0 )
			{
				g_MessageBox.SetMessage( _T("ERROR : Host not found! [0xE0CD]") );
				return FALSE;
			}

			HOSTENT FAR* lphostent = ::gethostbyname(hostname);
			if(lphostent == NULL)
			{
				g_MessageBox.SetMessage( _T("ERROR : Host not found! [0xE0CF]") );
				return FALSE;
			}

			sprintf(hostname,"%s",inet_ntoa(*((struct in_addr*)lphostent->h_addr)));
#else
			sockaddr_in sad;
			int	len;

			if(getsockname(m_hLoginUDPSocket, (sockaddr *)&sad, &len) < 0)
			{
				return FALSE;
			}

			sprintf(hostname, "%s", inet_ntoa(sad.sin_addr));

			_XLog( "Gethostname" );
#endif
*/			

			char buf[96];
			memset( buf, 0, sizeof(char) * 96 );

	#ifdef _XDEF_NEXONLOGIN			
			CND_LoginChecker checker(en_lmbc_nexon) ;
			checker.lc_MakePacket( en_lmbc_nexon, buf, 96, g_pLocalUser->m_UserName, g_pLocalUser->m_UserPW, hostname );
	#else
		#ifdef _XVIETNAMESE
			CND_LoginChecker checker(en_lmbc_vina) ;
			checker.lc_MakePacket( en_lmbc_vina, buf, 96, g_pLocalUser->m_UserName, g_pLocalUser->m_UserPW, hostname );
		#else
			#ifdef _XTAIWANESE
				CND_LoginChecker checker(en_lmbc_metel) ;
				checker.lc_MakePacket( en_lmbc_metel, buf, 96, g_pLocalUser->m_UserName, g_pLocalUser->m_UserPW, hostname );
			#else
				#ifdef _XENGLISH
					#ifdef _ACCLAIM_VERSION
						CND_LoginChecker checker(en_lmbc_acclaim) ;
						checker.lc_MakePacket( en_lmbc_acclaim, buf, 96, g_szAcclaimID, g_szAcclaimPW, hostname );
					#else // PWC Version
						CND_LoginChecker checker(en_lmbc_pwc) ;
						checker.lc_MakePacket( en_lmbc_pwc, buf, 96, g_pLocalUser->m_UserName, g_pLocalUser->m_UserPW, hostname );
					#endif
				#else
					#ifdef _XJAPANESE
						CND_LoginChecker checker(en_lmbc_rocworks) ;
						checker.lc_MakePacket( en_lmbc_rocworks, buf, 96, g_pLocalUser->m_UserName, g_pLocalUser->m_UserPW, hostname );
					#else
						#ifdef _XRUSSIAN
							CND_LoginChecker checker(en_lmbc_russia) ;
							checker.lc_MakePacket( en_lmbc_russia, buf, 96, g_pLocalUser->m_UserName, g_pLocalUser->m_UserPW, hostname );
						#endif
					#endif
				#endif
			#endif
		#endif
	#endif
//			Encryption( buf, 96, 10 );
			memcpy( Login.passport, buf, sizeof(char) * 96 );

//			char* pPacket = (char*)&Login;
//			Encryption((char*)&pPacket[2], Login.header.sLength, 10);
			
			return _XUDPSendPacket_Encapsulate( (char*)&Login );
		}

#else	// ETC Login
		{
			MSG_LOGIN Login ;
			memset( &Login, 0, sizeof(Login) );
			Login.header.sLength = sizeof(Login);
			Login.header.sCrypto = true;
			Login.header.sCompressed = false;

#ifdef _XADMINISTRATORMODE
			/*
			int __mCnt = 0;
			DWORD	aCnt = 0;
			INTERFACE_INFO	aInfo[ 200 ];
			int aRv = WSAIoctl(m_hLoginUDPSocket, SIO_GET_INTERFACE_LIST, NULL, 0, aInfo, sizeof(aInfo), &aCnt, NULL, NULL);
			if (SOCKET_ERROR == aRv) return (WSAGetLastError());
			
			int aMax = aCnt / sizeof(aInfo[0]);
			for ( int ii = 0; ii < aMax; ++ii )
			{
				// Loop-Back�� �����Ѵ�.
				if ((IFF_UP & aInfo[ii].iiFlags) && (!(IFF_LOOPBACK & aInfo[ii].iiFlags))) 
				{ 
					__mList[__mCnt++] = &aInfo[ii].iiAddress.Address; 
				}
			}
			*/
			if( _XCmdLineOption("xgmlogin",0) != 0 )
			{
				Login.ucMessage = MSG_NO_GMLOGIN;
			}
			else
			{
				Login.ucMessage = MSG_NO_LOGIN;
			}
#else
			Login.ucMessage = MSG_NO_LOGIN;
#endif

			g_PatchVersionManager.GetVersionInfo();

			Login.sVersionIndex = g_PatchVersionManager.m_VersionInfo.patchversion_1;
			Login.dwClientVer = g_PatchVersionManager.GetExeVersion();

#ifdef _XTESTSERVER
			if( _XCmdLineOption("xinnertestserverlogin",0) == 0 )
			{
				Login.cForestSelectNum = _COUNT_MAX_GROUP_-1;
			}
			else
			{
				Login.cForestSelectNum = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
			}
#else
			Login.cForestSelectNum = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
			
			// �ӽ�....���� Ŭ���̾�Ʈ�� �׽�Ʈ���� ���ӽ� ���
			if( _XCmdLineOption("xforcetestserverconnect",0) != 0 )
			{
				Login.cForestSelectNum = _COUNT_MAX_GROUP_-1;
			}				
#endif

			strcpy( Login.cID, g_pLocalUser->m_UserName );
			strcpy( Login.cPassword, g_pLocalUser->m_UserPW );
			
			_XDWINPRINT( "SEND: Login Packet" );
			return _XUDPSendPacket_Encapsulate( (char*)&Login );
		} 
#endif

		break ;
	default:
		break;
	}
	return TRUE;
}

BOOL _XNetwork::_XUDPSendPacket_Encapsulate( char* pPacket )
{
	if( g_PacketCrypto.GetSeqNum() == 0 )
	{
		//_XFatalError( "Send error : not intialize crypto sequence" );
		_XLog( "Send error : 2306" );
		return FALSE;
	}

	_Encapsule_info	cryptoinfo = g_PacketCrypto.Encapsulate( pPacket );

	if( !cryptoinfo.buf )
	{
		_XFatalError( "Send error : 2307" );
		return FALSE;
	}

	int ret = send( m_hLoginUDPSocket, (char*)cryptoinfo.buf, cryptoinfo.length, 0 );
	if( ret == SOCKET_ERROR ) 
	{
		ret = WSAGetLastError();
		ProcessUDPNetworkError();					
		return FALSE;
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////
////                               ���� ���� ���� �κ�                                    ////
//////////////////////////////////////////////////////////////////////////////////////////////

BOOL _XNetwork::ConnectServer( void )
{
	
#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode )
	{
		return TRUE;
	}
#endif

	// ���� ���Ӽ����� ���� ��
	DisconnectServer(TRUE);

	m_hClientSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (m_hClientSocket == INVALID_SOCKET) 
	{
		m_hClientSocket = NULL;
		_XFatalError( "Can't create client socket." );
		return FALSE;
	}

	struct sockaddr_in addr;
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = 0;

	_XDWINPRINT( "Connecting server" );                     
	
	if( WSAAsyncSelect(m_hClientSocket, gHWnd, WM_USER_SOCKET, FD_CONNECT) == SOCKET_ERROR )
	{
		int nError = WSAGetLastError();
		_XFatalError( _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILSELECT), nError);
		return FALSE;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_SERVER_PORT);
	addr.sin_addr.s_addr = m_SERVER_ADDRESS; 

	//TCHAR ipaddress[256];
	//memset( ipaddress, 0, sizeof(TCHAR) * 256 );
	//strcpy( ipaddress, inet_ntoa( addr.sin_addr ) );
	//_XLog( "Game server : %s:%d", ipaddress, m_SERVER_PORT );

//#ifdef _XDWDEBUG
//	TCHAR ipaddress[256];
//	memset( ipaddress, 0, sizeof(TCHAR) * 256 );
//	strcpy( ipaddress, inet_ntoa( addr.sin_addr ) );
//#endif
	
	if (connect(m_hClientSocket, (struct sockaddr*) &addr, sizeof(addr)) == SOCKET_ERROR) 
	{
		int nError = WSAGetLastError();

		if( nError != WSAEWOULDBLOCK )
		{
			_XFatalError( _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILCONNECT), nError);
			closesocket( m_hClientSocket ) ;
			return FALSE;
		}
	}
	
	_XDWINPRINT( "Connected server..." );

	return TRUE;
}

BOOL _XNetwork::DisconnectServer( BOOL bNextConnect )
{	
	int ret = WSAGetLastError();
	
#ifdef _XDWDEBUG
	if( g_SinglePlayMode  || g_ZeroLevelMode )
	{
		return TRUE;
	}
#endif
	Logout(bNextConnect);	

#ifdef _XUSEAPEXMODULE
	if( g_ApexInitialized )
	{
		CHCEnd();
		g_ApexInitialized = FALSE;
		#ifdef _XAPEXMODULE_LOG
		_XLog( "CHCEnd" );
		#endif
	}	
#endif

	if( m_hClientSocket )
	{
		shutdown(m_hClientSocket, SD_BOTH);
		closesocket(m_hClientSocket);	
		m_hClientSocket = NULL;

		_XLog( "Disconnected. [GS,_ECode:%d].", ret );
	}
	else
	{
		_XLog( "Disconnected." );
	}

	m_bConnected = FALSE;
	g_PacketCrypto.InitSeqNum( 0 );
	//WSAAsyncSelect(m_hMessengerSocket, gHWnd, WM_USER_SOCKET, 0 );	
	return TRUE;
}


void _XNetwork::Login( void )
{
	if( !m_bConnected ) return;

#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode ) return;
#endif

	//SendPacket( MSG_NUMBER_LOGIN );	
}

void _XNetwork::Logout( BOOL bNextConnect )
{
#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode ) return;
#endif
	
	SendPacket( MSG_NO_LOGOUT ) ;

#ifdef _XDEF_NEXONLOGIN // Nexon login	

#ifdef _XDEF_NEXONLOGINFINAL	// �ؽ� �޽��� �α���
	if(strcmp(m_LOGINSERVER_ADDRESS, _T("221.147.34.19")) != 0)
	{
		if(!bNextConnect)
			ReleaseNMCO();
	}
#endif

#endif

	m_bConnected = FALSE;
}

void _XNetwork::InsertChatString( LPSTR message, int messagemode, BOOL bCenterMessage, D3DCOLOR centermessageinfo )
{
	if( strlen( message ) <= 0 ) return;
	
	
	if( g_Chatlist.getitemcount() >= _XDEF_CHATLIST_QUESIZE )
	{
		g_Chatlist.deleteItem( 0 );
	}

	if( g_SystemMsgList.getitemcount() >= _XDEF_CHATLIST_QUESIZE )
	{
		g_SystemMsgList.deleteItem( 0 );
	}
	
	_XWindow_ChattingDefault* pChattingWindowDefault = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
	_XWindow_SystemMsgDefault* pSystemMsgWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );

//	Author : ����� 
//���Ӱ��� ��/�� �˻�
#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
	static int igameguardcheckcounter = 0;

	if( igameguardcheckcounter > 10 ) // chatting message�� 10���̻� ������ gameguard check.
	{
		igameguardcheckcounter = 0;

		if( g_GameGuardInstance.Check() != NPGAMEMON_SUCCESS ) 
		{
			extern LPTSTR g_nProtectModuleCheckMessage;
			_XFatalError(g_nProtectModuleCheckMessage);
		}
	}
	else
	{
		igameguardcheckcounter++;
	}
#endif
//Last updated: 07/01/22

	if( pChattingWindowDefault )	
	{
		/*if( pChattingWindowDefault->m_DockingSystemMessageButton.GetCheck() )	// �Բ�
		{
			_XStringSplitInfo splitinfo;
			int viewsize = pChattingWindowDefault->GetWindowSize().cx - 28;
			g_XBaseFont->SplitString( message, viewsize, splitinfo, 0 );
			
			TCHAR splitstring[256];
			
			for( int i = 0; i < splitinfo.splitcount; i++ )
			{			
				memset( splitstring, 0, 256 );
				strncpy( splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i] );					
				
				_XStringItem* chattingmessage = new _XStringItem( splitstring, messagemode, g_LocalSystemTime );
				if( chattingmessage )
				{				
					g_Chatlist.resetList();
					g_Chatlist.insertItem( chattingmessage );
				}
			}
		}
		else	// ����*/
		{
			_XStringSplitInfo splitinfo;
			int viewsize = pChattingWindowDefault->GetWindowSize().cx - 28;
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->SplitString( message, viewsize, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR );
			}
			else
			{
				g_XBaseFont->SplitString( message, viewsize, splitinfo, 0 );
			}
			
			TCHAR splitstring[256];
			for( int i = 0; i < splitinfo.splitcount; ++i )
			{			
				memset( splitstring, 0, 256 );
				strncpy( splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i] );					

				_XStringItem* chattingmessage = new _XStringItem( splitstring, messagemode, g_LocalSystemTime );
				if( chattingmessage )
				{
					if( messagemode < 6 )	// �Ϲ� �޽���
					{
						g_Chatlist.resetList();
						g_Chatlist.insertItem( chattingmessage );						
					}
					else					// �ý��� �޽���
					{
 						if( !g_SystemMsgList.listEmpty() )
						{
							g_SystemMsgList.resetList();
							_XStringItem* currentItem = (_XStringItem*)g_SystemMsgList.currentItem();

                            //Author : ����� //breif : �ý��� �޽��� ���ٷ� �Ѿ�� �ߺ������ ��.. ����Ʈ�� �̻���..
                            if( splitinfo.splitcount > 1 )
                            {
                                for( int j = i; j < splitinfo.splitcount; j++ )
                                {
                                    currentItem = (_XStringItem*)currentItem->getleft(); //������ ������ŭ �������� ����,, ��Ʈ���� �ִ�..
                                }
                            }
                            else
                                currentItem = (_XStringItem*)currentItem->getleft();

							if( currentItem )
							{
								if( strcmpi( currentItem->Get_String(), splitstring ) == 0 )
									currentItem->Set_Attribute2( g_LocalSystemTime );
								else
								{
									g_SystemMsgList.resetList();
									g_SystemMsgList.insertItem( chattingmessage );
								}
							}
						}
						else
						{
							g_SystemMsgList.resetList();
							g_SystemMsgList.insertItem( chattingmessage );
						}
						// ��� ���� �޼���
						if( bCenterMessage )
						{		
							_XInsertCenterSystemMessage( message , centermessageinfo );
						}
					}


				}
			}
		}
		
		if( messagemode < 6 )	// �Ϲ� �޽���
		{
			if( pChattingWindowDefault && pChattingWindowDefault->m_ChatListScrollBar )
			{			
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
				pChattingWindowDefault->m_ChatListScrollBar->SetTotalLineCount( pChattingWindowDefault->RecalcChattingListLength() );
#else
				pChattingWindowDefault->m_ChatListScrollBar->SetTotalLineCount( g_Chatlist.getitemcount() );
#endif
				pChattingWindowDefault->m_ChatListScrollBar->UpdateData();
				pChattingWindowDefault->m_ChatListScrollBar->SetLineScrollPos( 999999 ); 
			}					
		}
		else					// �ý��� �޽���
		{
			if( pSystemMsgWindow && pSystemMsgWindow->m_SystemMsgListScrollBar )
			{
				pSystemMsgWindow->m_SystemMsgListScrollBar->SetTotalLineCount( g_SystemMsgList.getitemcount() );
				pSystemMsgWindow->m_SystemMsgListScrollBar->UpdateData();
				pSystemMsgWindow->m_SystemMsgListScrollBar->SetLineScrollPos( 999999 ); 
			}
		}
		
	}
}

void _XNetwork::SendChatMessage( LPSTR chatmessage, int messagemode  )
{
//	Author : ����� 
//���Ӱ��� ���� �˻�
#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)

	if( g_GameGuardInstance.Check() != NPGAMEMON_SUCCESS ) 
	{
		extern LPTSTR g_nProtectModuleCheckMessage;
		_XFatalError(g_nProtectModuleCheckMessage);
	}

#endif
//Last updated: 07/01/22

	if( g_pLocalUser->m_Lock.chatlock == 1)
	{
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2332),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//�����̻����� ���Ͽ� �ٸ� ����ڿ� ��ȭ�� �Ұ��� �մϴ�.
		g_pLocalUser->CharacterStateChatLock(chatmessage);
	}
	
	if( g_pLocalUser->m_ChatMuteTime > 0 )
	{
		int minutes = g_pLocalUser->m_ChatMuteTime / 60000;
		TCHAR messagestring[512];
		memset(messagestring, 0, sizeof(messagestring));

		if( minutes > 0 )
		{
			_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_2009), minutes );
		}
		else
		{
			int seconds = (g_pLocalUser->m_ChatMuteTime % 60000) / 1000;
			_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_2010), seconds );
		}

		InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_GMBLOCKSTATUS); //����ڴ��� ���� ä�� ���� �����Դϴ�.
		return;
	}

	if( strncmp( chatmessage, _T("$%^"), 3 ) == 0 )
	{
#ifdef _XGMCLIENT
		if( g_pLocalUser->m_CharacterInfo.Get_gm_level() >= 2 )
		{
			InsertChatString("Send special code",_XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);
			MSG_ZONE_CHAT_NORMAL chatnormal;
			memset( chatnormal.cName, 0, sizeof(TCHAR) * 13 );
			strcpy( chatnormal.cSay, chatmessage );
			ProcessChatSpecialCode( &chatnormal );
		}	
		else
		{
			return;
		}
#endif
	}

#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode )		return;		
#endif

	if( !m_bConnected ) return;

	if( strlen( chatmessage ) <= 0 ) return;
	
	if( g_Chatlist.getitemcount() >= _XDEF_CHATLIST_QUESIZE )
	{
		g_Chatlist.deleteItem( 0 );
	}
	_XWindow_ChattingDefault* pChattingWindowDefault = 
		(_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );

	if( !pChattingWindowDefault ) return;

	// �ͼӸ� ��� üũ 2004.05.04->oneway48 modify
	if( messagemode == _XDEF_CHATMESSAGECOLOR_WHISPER )
	{
		if( strlen( pChattingWindowDefault->m_WhisperTargetName ) <= 0 )
		{
			_XStringItem* chattingmessage = new _XStringItem( _XGETINTERFACETEXT(ID_STRING_NETWORK_WARNWISPER), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			if( chattingmessage )
			{				
				g_Chatlist.resetList();
				g_Chatlist.insertItem( chattingmessage );
			}
			
			if( pChattingWindowDefault->m_ChatListScrollBar )
			{
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
				pChattingWindowDefault->m_ChatListScrollBar->SetTotalLineCount( pChattingWindowDefault->RecalcChattingListLength() );
#else
				pChattingWindowDefault->m_ChatListScrollBar->SetTotalLineCount( g_Chatlist.getitemcount() );
#endif
				pChattingWindowDefault->m_ChatListScrollBar->UpdateData();
				pChattingWindowDefault->m_ChatListScrollBar->SetLineScrollPos( 999999 ); 
			}	
			
			return;
		}
	}

	TCHAR tcChatData[_XDEF_CHATMAXLENGTH]; 
	memset( tcChatData, 0x00, _XDEF_CHATMAXLENGTH );
	strcpy( tcChatData, chatmessage );
	
#ifdef _XDEF_NEWVERSONFILETERING
	g_WordFilteringManager.Filtering( tcChatData,_XDEF_WORDFILETERINGTYPE_CHANGE | _XDEF_WORDFILETERINGTYPE_ALWAYS );
#else
	g_WordFilteringManager.Filtering( tcChatData, _XDEF_WORDFILETERINGTYPE_ALWAYS );
#endif

	TCHAR tempbuffer[256];
	int tabwidth = 0;
	memset( tempbuffer, 0, 256 );

	strncpy( tempbuffer, g_pLocalUser->m_CharacterInfo.charactername, sizeof(tempbuffer) );	
	strcat( tempbuffer, _T(":") );

/*  //Author : ����� //breif : �̱��� ������ �޽��� ���̵� �ȳ����� ���� 08/08/22
	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH  )
	{
		if( g_pLocalUser->m_bFaceMaskMode )
		{
			memset( tempbuffer, 0, sizeof(tempbuffer) );
			sprintf( tempbuffer, _XGETINTERFACETEXT(ID_STRING_NEW_2659));	
			strcat( tempbuffer, _T(":") );
		}
	}
*/
	if( messagemode == _XDEF_CHATMESSAGECOLOR_WHISPER )
	{
		TCHAR tempidbuffer[256];
		memset(tempidbuffer, 0, sizeof(tempidbuffer));
		_snprintf(tempidbuffer, sizeof(tempidbuffer), "(%s) ", pChattingWindowDefault->m_WhisperTargetName );
		strncat( tempbuffer, tempidbuffer, sizeof(tempbuffer) );
	}
	tabwidth = g_XBaseFont->GetWidth(tempbuffer);
	strncat( tempbuffer, tcChatData, sizeof(tempbuffer) );	
		
	_XStringSplitInfo splitinfo;
	int viewsize = pChattingWindowDefault->GetWindowSize().cx - 28;

	g_XBaseFont->SplitString( tempbuffer, viewsize, splitinfo, tabwidth, _XSTRINGSPLITFUNCTION_SPACECHAR );
	

	TCHAR splitstring[256];
	int   ChatUserStatus = 0; 
	
	for( int i = 0; i < splitinfo.splitcount; ++i )
	{			
		memset( splitstring, 0, 256 );
		strncpy( splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i] );
		
		DWORD attrib;			
		if( i != 0 ) attrib = (tabwidth << 16) | (messagemode + ((messagemode == _XDEF_CHATMESSAGEMODE_NORMAL) ? _XDEF_CHATMESSAGECOLOR_MYMESSAGE : 0) );
		else attrib = (messagemode + ((messagemode == _XDEF_CHATMESSAGEMODE_NORMAL) ? _XDEF_CHATMESSAGECOLOR_MYMESSAGE : 0) );
		
		// 2004.04.29->oneway48 insert 
		if( messagemode == _XDEF_CHATMESSAGECOLOR_WHISPER )		
			attrib = _XDEF_CHATMESSAGECOLOR_WHISPER;
		else if( messagemode == _XDEF_CHATMESSAGECOLOR_USERCLAN )
			attrib = _XDEF_CHATMESSAGECOLOR_USERCLAN;
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		else if( messagemode == _XDEF_CHATMESSAGECOLOR_ALLY )
			attrib = _XDEF_CHATMESSAGECOLOR_ALLY;
#endif
		else if( messagemode == _XDEF_CHATMESSAGECOLOR_PARTY )		
			attrib = _XDEF_CHATMESSAGECOLOR_PARTY;
#ifdef _XDEF_CASHITEM_SHOUT
		else if( messagemode == _XDEF_CHATMESSAGECOLOR_SHOUT)
			attrib = _XDEF_CHATMESSAGECOLOR_SHOUT;
#endif
#ifdef _XDEF_ZONECHATTING_20070108
		else if( messagemode == _XDEF_CHATMESSAGECOLOR_ZONECHAT)
			attrib = _XDEF_CHATMESSAGECOLOR_ZONECHAT;
#endif

		_XStringItem* chattingmessage = new _XStringItem( splitstring, attrib );
		if( chattingmessage )
		{				
			g_Chatlist.resetList();
			g_Chatlist.insertItem( chattingmessage );
			
			// �׽�Ʈ ��
//			_XStringItem* systemmessage = new _XStringItem( splitstring, 255, g_LocalSystemTime );
//			if( systemmessage )
//			{				
//				if( g_CenterSystemMessageList.getitemcount() >= 3 )
//				{
//					g_CenterSystemMessageList.deleteItem( 0 );
//				}
//				
//				if( !g_CenterSystemMessageList.listEmpty() )
//				{
//					g_CenterSystemMessageList.resetList();
//					_XStringItem* currentItem = (_XStringItem*)g_CenterSystemMessageList.currentItem();
//					currentItem = (_XStringItem*)currentItem->getleft();							
//					if( currentItem )
//					{
//						if( strcmpi( currentItem->Get_String(), splitstring ) == 0 )
//							currentItem->Set_Attribute2( g_LocalSystemTime );
//						else
//						{
//							g_CenterSystemMessageList.resetList();
//							g_CenterSystemMessageList.insertItem( systemmessage );
//						}
//					}
//				}
//				else
//				{
//					g_CenterSystemMessageList.resetList();
//					g_CenterSystemMessageList.insertItem( systemmessage );
//				}
//			}
			// �׽�Ʈ ��
		}
	}
	
	if( pChattingWindowDefault->m_ChatListScrollBar )
	{
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		pChattingWindowDefault->m_ChatListScrollBar->SetTotalLineCount( pChattingWindowDefault->RecalcChattingListLength() );
#else
		pChattingWindowDefault->m_ChatListScrollBar->SetTotalLineCount( g_Chatlist.getitemcount() );
#endif
		pChattingWindowDefault->m_ChatListScrollBar->UpdateData();
		pChattingWindowDefault->m_ChatListScrollBar->SetLineScrollPos( 999999 ); 
	}	
		
	_XDWPACKETLOG( "SEND MESSAGE : Chat message [%s]", tcChatData );	
	
	memset( tempbuffer, 0, 256 );
	int stringlength = strlen( tcChatData );

	if( stringlength > _XDEF_CHATMAXLENGTH-1 )	
	{
		memcpy( tempbuffer, tcChatData, _XDEF_CHATMAXLENGTH-1 );	
		stringlength = _XDEF_CHATMAXLENGTH-1;
	}
	else	
	{
		memcpy( tempbuffer, tcChatData, stringlength );
	}
	
	// ģ�� ��� �޽��� ä���� �� ���� ������ �ƴϹǷ� Packet�� �ٸ���.
	/*
	if( pChattingWindowDefault->m_ChatMode == _XCHATMODE_FRIEND )  
	{
		_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
		
		if( pMessenger_Window->m_SelectedCharactername[0] != 0 )
		{
			if( pMessenger_Window->m_CurrentUserStatus != 0 )	// �ڽ��� �α׿��������̸� �޼����� ������ �ʴ´�.
			{
				if( ChatUserStatus != 0 )
				{
					MMSG_CHAT messengerchat ;
					memset( &messengerchat, 0, sizeof(MMSG_CHAT) );
					
					int size = sizeof( MMSG_CHAT );
					messengerchat.cMessage = MMSG_NO_CHAT;
					strcpy( messengerchat.cFromCharacName, g_pLocalUser->m_CharacterInfo.charactername ); 
					strcpy( messengerchat.cToCharacName, pMessenger_Window->m_SelectedCharactername  );	
					
					strcpy( messengerchat.cChatMsg , tempbuffer );
					messengerchat.sLength = sizeof(MMSG_CHAT) - ( _XDEF_CHATMAXLENGTH - stringlength ) ;
					messengerchat.cChatMsg[ stringlength ] = 0 ;
					
					_XDWPACKETLOG( "SEND MESSENGER MESSAGE : Messenger Chatting message [%s]", tcChatData );
					
					return _XSendPacket_Encapsulate( (char*)&messengerchat );
				}
			}
		}		
		
	} // insert end 
	else
	*/

	// �ӼӸ��� �޽��� �������� ������.. -------------------------------------------------------------------------------=  
	if( messagemode == _XDEF_CHATMESSAGECOLOR_WHISPER )
	{
		MMSG_CHAT packet;
		memset( &packet, 0, sizeof( MMSG_CHAT ) );

		int size = sizeof( MMSG_CHAT );
		packet.cMessage = MMSG_NO_CHAT_WHISPER;
		strncpy( packet.cFromCharacName, g_pLocalUser->m_CharacterInfo.charactername, sizeof(packet.cFromCharacName) );
		strncpy( packet.cToCharacName, pChattingWindowDefault->m_WhisperTargetName, sizeof(packet.cToCharacName) );

		strncpy( packet.cChatMsg, tempbuffer, sizeof(packet.cChatMsg) );
		packet.sLength = sizeof(MMSG_CHAT) - ( _XDEF_CHATMAXLENGTH - stringlength ) ;
		packet.cChatMsg[ stringlength ] = 0 ;

		strncpy( packet.cToCharacName, pChattingWindowDefault->m_WhisperTargetName, sizeof(packet.cToCharacName) );
		pChattingWindowDefault->InsertRecentWhisperUserList( pChattingWindowDefault->m_WhisperTargetName, 3 );
		pChattingWindowDefault->SetChangeWhisperName(0);

		if( send ( m_hMessengerSocket, (char*)&packet, packet.sLength , 0 ) == SOCKET_ERROR )
		{		
			ProcessMessengerNetworkError();		
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_REQUESTLATER), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			_XUpdateWindow();
			_XLog("Messenger send error : [_ECode:%d]", WSAGetLastError());
			return;
		}

		// ������ ä�� �޽��� ���� �ð� ���
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LastSendChatMessageTime = g_LocalSystemTime;  
	}
	else
	{
		MSG_ZONE_CHAT_NORMAL packet ;
		memset( &packet, 0, sizeof(MSG_ZONE_CHAT_NORMAL) );

		int size = sizeof( MSG_ZONE_CHAT_NORMAL ) ;
		packet.ucMessage		= MSG_NO_ZONE_CHAT_NORMAL ;	
		packet.cMode		= messagemode ;		
		
		// ��Ƽ ���
		if( messagemode == _XDEF_CHATMESSAGECOLOR_PARTY )
		{
			if( g_pLocalUser->m_InParty )
			{			
				packet.cMode		= 16;	// Party mode...
			}

			strncpy( packet.cName, g_pLocalUser->m_CharacterInfo.charactername, sizeof(packet.cName) );
		}
		else if( messagemode == _XDEF_CHATMESSAGECOLOR_USERCLAN )
		{
			if( strlen(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName) )
			{
				packet.cMode = 17;		// ���� ���
			}
			strncpy( packet.cName, g_pLocalUser->m_CharacterInfo.charactername, sizeof(packet.cName) );
		}
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		else if( messagemode == _XDEF_CHATMESSAGECOLOR_ALLY )
		{
			if( strlen(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName) )
			{
				packet.cMode = 18;		// ��ȸ ���
			}
			strncpy( packet.cName, g_pLocalUser->m_CharacterInfo.charactername, sizeof(packet.cName) );
		}
#endif
#ifdef _XDEF_CASHITEM_SHOUT
		else if( messagemode == _XDEF_CHATMESSAGECOLOR_SHOUT)
		{
			// item ������ ���� �ٸ��� �����ؾ� ��
			packet.cMode = g_pLocalUser->m_ShoutMode;
			strncpy(packet.cName, g_pLocalUser->m_CharacterInfo.charactername, sizeof(packet.cName));
		}
#endif
#ifdef _XDEF_ZONECHATTING_20070108
		else if( messagemode == _XDEF_CHATMESSAGECOLOR_ZONECHAT)
		{
			// item ������ ���� �ٸ��� �����ؾ� ��
			packet.cMode = 15;			// ������ȭ ��� (������ ������ Ȯ��)
			strncpy(packet.cName, g_pLocalUser->m_CharacterInfo.charactername, sizeof(packet.cName));
		}
#endif
		else
		{
			if( g_pLocalUser->m_bFaceMaskMode )
				packet.cMode = 33;		// ���� �Ϲ�
			strncpy( packet.cName, g_pLocalUser->m_CharacterInfo.charactername, sizeof(packet.cName) );
		}
		
		packet.header.sLength = stringlength + 18 ;	
		strncpy( packet.cSay, tempbuffer, sizeof(packet.cSay) );

		// ������ ä�� �޽��� ���� �ð� ���
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LastSendChatMessageTime = g_LocalSystemTime;  

		_XSendPacket_Encapsulate( (char*)&packet );
	}
}

void _XNetwork::ReadSocket( void )
{	
	
	if( !m_bConnected ) return;

#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode ) return;	
#endif
	
	VIEWPACKETLOG
		_XDWPACKETLOG( "{## recieve socket message" );
	

#define _XDEF_PACKETPARSING_
#ifdef  _XDEF_PACKETPARSING_
	int		 nRead;
	_sHeader head;

	while (1)
	{
		nRead = recv(m_hClientSocket, (char*)g_szPacket + g_nRI, _XDEF_PACKET_MAXBUFFER - g_nRI, 0);
		if( nRead <= 0 ) // empty or blocked packet buffer.
		{
			if( nRead == SOCKET_ERROR )
			{
				int errorcode = WSAGetLastError();
				if( errorcode == WSAEWOULDBLOCK )
				{
					Sleep(0);
					//_XInsertSystemMessage( 0, "recv would block [%d]", g_LocalSystemTime );
				}
				else
				{
					g_NetworkKernel.ProcessNetworkError();
					return;
				}
			}
			break;
		}
		//else if( nRead == 0 )// empty packet buffer.
		//{
		//	break;
		//}

		g_nRI += nRead;

		//*(g_szPacket + g_nRI) = 0;

		while( g_nPI < g_nRI ) 
		{
			if(g_mode == _X_PACKET_RECIEVE_PACKET_LENGTH) 
			{
				if( g_nPI < g_nRI-1 ) 
				{
					memcpy( &head, (g_szPacket+g_nPI), sizeof(_sHeader) );
					
					if( head.sLength <= 0 )
					{
						g_sPacketLen = 0;
						_XLog( "WARNING : Packet header invalid length [%d %d %d]", head.sLength, head.sCrypto, head.sCompressed );
#ifdef _XDWDEBUG
 						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
						g_MessageBox.SetMessage( "WARNING : Packet header invalid length [%d %d %d]", head.sLength, head.sCrypto, head.sCompressed );						
						g_MessageBox.ShowWindow( TRUE );
#endif
						break;
					}
					else
					{
						g_sPacketLen = head.sLength;
						g_mode = _X_PACKET_RECIEVE_PACKET_BODY;
					}
				}
				else 
				{
					break;
				}
			}
			else if (g_mode == _X_PACKET_RECIEVE_PACKET_BODY) 
			{
				if (g_nPI + g_sPacketLen <= g_nRI)
				{

					if( g_szPacket[g_nPI+2] >= _XDEF_SERVERMESSAGE_MAX )
					{
						break;
					}
					else
					{
						if( !ParsingPacket(g_szPacket + g_nPI) )
						{
							VIEWPACKETLOG
								_XDWPACKETLOG( "  end of recieve message [new connect]##}" );
							return;
						}

					}
					g_nPI += g_sPacketLen;
					g_mode = _X_PACKET_RECIEVE_PACKET_LENGTH;
				}
				else
				{
					break;
				}
			}
		}
		if (g_nPI == g_nRI) 
		{
			g_nPI = g_nRI = 0;
		}
		else if (g_nPI > 0) 
		{
			if( g_nRI - g_nPI < 1 )
			{
				if( g_nRI - g_nPI < 0 )
				{
					InitializePacketBuffer();
					_XLog( "WARNING : Invalid Incompletion packet size [RI : %d / PI : %d]", g_nRI, g_nPI );
				}
				else
				{
					_XLog( "WARNING : Invalid Incompletion packet size zero [RI : %d / PI : %d]", g_nRI, g_nPI );
				}
#ifdef _XDWDEBUG
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
				g_MessageBox.SetMessage( "WARNING : Invalid Incompletion packet size [RI : %d / PI : %d]", g_nRI, g_nPI );
				g_MessageBox.ShowWindow( TRUE );
#endif				
			}
			else
			{
				if( g_nRI >= _XDEF_PACKET_MAXBUFFER )
				{
					InitializePacketBuffer();
#ifdef _XDWDEBUG
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
					g_MessageBox.SetMessage( "WARNING : Overflow Incompletion packet size [%d]", g_nRI );
					g_MessageBox.ShowWindow( TRUE );
#endif
					_XLog( "WARNING : Overflow Incompletion packet size [%d]", g_nRI );
				}
				else
				{
					int leftsize = g_nRI - g_nPI;

					BYTE	g_szTempPacket[_XDEF_PACKET_MAXBUFFER];
					memset( g_szTempPacket, 0, _XDEF_PACKET_MAXBUFFER );
					memcpy( g_szTempPacket, g_szPacket + g_nPI, leftsize );
					memset( g_szPacket, 0, _XDEF_PACKET_MAXBUFFER );
					memcpy( g_szPacket, g_szTempPacket, leftsize );

					//memmove(g_szPacket, g_szPacket + g_nPI, g_nRI - g_nPI);	
					
					g_nRI = leftsize;
					g_nPI = 0;

					if( g_nRI <= -1 )
					{
#ifdef _XDWDEBUG
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
						g_MessageBox.SetMessage( "WARNING : Invalid packet recieve point  [%d]", g_nRI );
						g_MessageBox.ShowWindow( TRUE );
#endif
						_XLog( "WARNING : Invalid packet recieve point  [%d]", g_nRI );
					}
				}
			}
		}
	}
#else
	int nRead = 0 ;
	char buf[255] ;
	nRead = recv(m_hClientSocket, buf, 255, 0) ;
	if( nRead == SOCKET_ERROR || nRead == 0 )
	{
		ProcessNetworkError();

		g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
		g_MessageBox.SetMessage( _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILRECEIVEDATA), WSAGetLastError() );
		g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
		_XUpdateWindow();		

		return;
	}

	ParsingPacket( buf );
#endif

	VIEWPACKETLOG
		_XDWPACKETLOG( "  end of recieve message ##}" );
}




#define _XPACKETSIZEERROR( pheader, type ) 	CheckPacketSizeError( pheader, sizeof(type), #type );

void CheckPacketSizeError( _sHeader* pheader, int size, LPTSTR string )
{
	if( pheader->sLength != size )
	{		
		//_XDWINPRINT( "PACKET SIZE ERROR : [Type : %s] [header length : %d] [org length : %d]", string, pheader->sLength, size );
		_XFatalError( "PACKET SIZE ERROR : [Type : %s] [header length : %d] [org length : %d]", string, pheader->sLength, size );
	}
}


BOOL _XNetwork::ParsingPacket(LPBYTE pPacket)
{	
#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode ) return TRUE;
#endif
	
#ifdef _XDWDEBUG
	_sHeader* header = (_sHeader*)pPacket;
	if( header )
	{
		VIEWPACKETLOG
			_XDWPACKETLOG( "Parsing recieved message Type : [%d]  [%d]:[%d]:[%d]", pPacket[2], header->sCompressed, header->sCrypto, header->sLength );

	}
#endif
	switch ( pPacket[2] )
	{
	// Crypto sequence number
	case MSG_NO_SEQUENCE_NUMBER :
		{
			MSG_SEQUENCE_NUMBER * pDequenceNumber = (MSG_SEQUENCE_NUMBER*)pPacket;
			g_PacketCrypto.InitSeqNum( pDequenceNumber->ucStartSeq );

#ifdef _XUSEENCRYPTPATTERN
			extern unsigned char g_SelectedEncryptSequance;
			g_SelectedEncryptSequance = pDequenceNumber->ucStartSeq;

#ifdef _XDWDEBUG
			_XLog("Seq:%d",g_SelectedEncryptSequance);
#endif

#endif

			//g_NetworkKernel.SendPacket( MSG_NO_SV_ENTER_GAME );
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Crypto sequence number & send enter gserver" );
			g_NetworkKernel.SendPacket( MSG_NO_SV_ENTER_GSERVER );
		}
		break;
	// ���� ���� ����
	case MSG_NO_SV_ENTER_RESULT :
		{
			MSG_SV_ENTER_RESULT* serverenteringresult = (MSG_SV_ENTER_RESULT*)pPacket;
			_XPACKETSIZEERROR( &serverenteringresult->header, MSG_SV_ENTER_RESULT );

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Server Enter Result [%d]", g_LocalSystemTime);

			MSGSVEnterResult(serverenteringresult);
		}
		break;

	// ĳ���� ����Ʈ
	case MSG_NO_CHARAC_LIST :
		{
			MSG_CHARAC_LIST* characterlist = (MSG_CHARAC_LIST*)pPacket;
			_XPACKETSIZEERROR( &characterlist->header, MSG_CHARAC_LIST);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : All Character List [%d]", g_LocalSystemTime);

			MSGCharacList(characterlist);
		}
		break;
		
		// ��������(ex: ��������)�� ����Ǿ��ٴ°��� �˸��� ��Ŷ. S->C
	case MSG_NO_SHIFT_STATE :
		{
			MSG_SHIFT_STATE* changeservergroup = (MSG_SHIFT_STATE*)pPacket;
			_XPACKETSIZEERROR( &changeservergroup->header, MSG_SHIFT_STATE);
			
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Change ServerGroup [%d]", g_LocalSystemTime);
			
			MSGChangeServerGroup(changeservergroup);
		}
		break;
	case MSG_NO_CHARAC_RESULT :
		{
			MSG_CHARAC_RESULT * characterstatusresult = (MSG_CHARAC_RESULT *)pPacket;
			_XPACKETSIZEERROR( &characterstatusresult->header, MSG_CHARAC_RESULT);

			VIEWPACKETLOG
				_XDWPACKETLOG( "RECEIVE MESSAGE : Character create/delete/select result [%d]", g_LocalSystemTime);

			MSGCharacResult(characterstatusresult);
		}
		break;

	case MSG_NO_CHARAC_INFO :
		{
			MSG_CHARAC_INFO* characterinfo = (MSG_CHARAC_INFO*)pPacket;
			_XPACKETSIZEERROR( &characterinfo->header, MSG_CHARAC_INFO);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Select character detail properies [%d]", g_LocalSystemTime);

			MSGCharacInfo(characterinfo);
		}
		break;

#ifdef _XTS_NEWCHARACLIST // _XTS_SERVERUNIFICATION �� ������ �������� ĳ���� �����ɸ� �����.
	case MSG_NO_UNI_GROUP_LOBBY :
	{
		MSG_UNI_GROUP_LOBBY* unigrouplobby = (MSG_UNI_GROUP_LOBBY*)pPacket;
		switch(unigrouplobby->ucMode)
		{
		case en_ug_change_name_result :
			{
				_XPACKETSIZEERROR( &unigrouplobby->header, MSG_UG_CHANGE_NAME_RESULT);
				
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : ServerUnification - nameresult [%d]", g_LocalSystemTime);	
			}
			break;		
		default :
			{
				//_XPACKETSIZEERROR( &unigrouplobby->header, MSG_UNI_GROUP_LOBBY);
				//
				//VIEWPACKETLOG
				//	_XDWPACKETLOG("RECEIVE MESSAGE : ServerUnification - base [%d]", g_LocalSystemTime);	
			}
			break;
		}
		MSGServerUnification(unigrouplobby);
	}
	break;
#else

#ifdef _XTS_SERVERUNIFICATION
	case MSG_NO_UNI_GROUP_LOBBY :
		{
			MSG_UNI_GROUP_LOBBY* unigrouplobby = (MSG_UNI_GROUP_LOBBY*)pPacket;
			switch(unigrouplobby->ucMode)
			{
			case en_ug_simple_grp_info :
				{
					_XPACKETSIZEERROR( &unigrouplobby->header, MSG_UG_SIMPLE_GROUP_INFO);
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : ServerUnification - simpleinfo [%d]", g_LocalSystemTime);					
				}
				break;
			case en_ug_detail_grp_info :
				{
					_XPACKETSIZEERROR( &unigrouplobby->header, MSG_UG_DETAIL_GRP_INFO);
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : ServerUnification - detailinfo [%d]", g_LocalSystemTime);					
				}
				break;
			case en_ug_move_result_complete :
				{
					_XPACKETSIZEERROR( &unigrouplobby->header, MSG_UG_MOVE_RESULT_COMPLETE);
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : ServerUnification - movecomplete [%d]", g_LocalSystemTime);					
				}
				break;
			case en_ug_change_name_result :
				{
					_XPACKETSIZEERROR( &unigrouplobby->header, MSG_UG_CHANGE_NAME_RESULT);
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : ServerUnification - nameresult [%d]", g_LocalSystemTime);	
				}
				break;
			case en_ug_force_use_result :
				{
					_XPACKETSIZEERROR( &unigrouplobby->header, MSG_UG_FORCE_USE_RESULT);
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : ServerUnification - forceuseresult [%d]", g_LocalSystemTime);	
				}
				break;
			default :
				{
					//_XPACKETSIZEERROR( &unigrouplobby->header, MSG_UNI_GROUP_LOBBY);
					//
					//VIEWPACKETLOG
					//	_XDWPACKETLOG("RECEIVE MESSAGE : ServerUnification - base [%d]", g_LocalSystemTime);	
				}
				break;
			}
			MSGServerUnification(unigrouplobby);
		}
		break;
#endif //#ifdef _XTS_SERVERUNIFICATION

#endif //#ifdef _XTS_NEWCHARACLIST

	case MSG_NO_SET_LEVEL :
		{
			MSG_SET_LEVEL* setlevel = (MSG_SET_LEVEL*)pPacket;
			_XPACKETSIZEERROR( &setlevel->header, MSG_SET_LEVEL);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Set level [%d]", g_LocalSystemTime);

			MSGSetLevel(setlevel);
		}
		break;

	case MSG_NO_SET_POWER :
		{
			MSG_SET_POWER* setpower = (MSG_SET_POWER*)pPacket;
			_XPACKETSIZEERROR( &setpower->header, MSG_SET_POWER);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Set Power [%d]", g_LocalSystemTime);

			MSGSetPower(setpower);
		}
		break;

	case MSG_NO_LEVEL_UP_REQ : 
		{
			MSG_LEVEL_UP_RESULT* levelupresult = (MSG_LEVEL_UP_RESULT*)pPacket;
			_XPACKETSIZEERROR( &levelupresult->header, MSG_LEVEL_UP_RESULT);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Level up REQ [%d]", g_LocalSystemTime);
			
			MSGLevelUpReq(levelupresult);
		}
		break;
		
	case MSG_NO_OTHER_CHARAC_LEVEL_UP :
		{
			MSG_OTHER_CHARAC_LEVEL_UP* otherlevelup = (MSG_OTHER_CHARAC_LEVEL_UP*)pPacket;
			_XPACKETSIZEERROR(&otherlevelup->header, MSG_OTHER_CHARAC_LEVEL_UP);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Other Character Level Up [%d]", g_LocalSystemTime);

			MSGOtherCharacLevelUp(otherlevelup);
		}
		break;

	case MSG_NO_CHARAC_SET_STATE :
		{
			MSG_CHARAC_SET_STATE* characsetstate = (MSG_CHARAC_SET_STATE*)pPacket;
			_XPACKETSIZEERROR(&characsetstate->header, MSG_CHARAC_SET_STATE);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Charac Set State [%d]", g_LocalSystemTime);

			MSGCharacSetState(characsetstate);
		}
		break;

	case MSG_NO_STATE_CHANGE_RESULT :
		{
#ifdef _XTS_PK
			MSG_STATE_CHANGE* statechange = (MSG_STATE_CHANGE*)pPacket;

			if(statechange->ucMode >= state_change_en_to_pk_mode && statechange->ucMode <= state_change_en_to_battle_mode)
			{
				MSG_STATE_CHANGE_TO_PK* statechangeresult = (MSG_STATE_CHANGE_TO_PK*)pPacket;
				_XPACKETSIZEERROR(&statechangeresult->header, MSG_STATE_CHANGE_TO_PK);
				
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : State change to PK [%d]", g_LocalSystemTime);
				
				MSGStateChangeToPK(statechangeresult);

			}
			else if(statechange->ucMode == state_change_en_notice_to_peace_mode || statechange->ucMode == state_change_en_notice_to_battle_mode ||
					statechange->ucMode == state_change_en_notice_to_pk_mode ||
					statechange->ucMode == state_change_en_notice_on_pe_mode || statechange->ucMode == state_change_en_notice_off_pe_mode)
			{
				MSG_STATE_CHANGE_NOTICE* statechangenotice = (MSG_STATE_CHANGE_NOTICE*)pPacket;
				_XPACKETSIZEERROR(&statechangenotice->header, MSG_STATE_CHANGE_NOTICE);
				
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : State change Notice [%d]", g_LocalSystemTime);
				
				MSGStateChangeNotice(statechangenotice);
			}
			else if(statechange->ucMode == state_change_en_on_pe_mode || statechange->ucMode == state_change_en_res_not_pe_end)
			{
				MSG_STATE_CHANGE_TO_PE* statechangetope = (MSG_STATE_CHANGE_TO_PE*)pPacket;
				_XPACKETSIZEERROR(&statechangetope->header, MSG_STATE_CHANGE_TO_PE);

				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : State change to PE [%d]", g_LocalSystemTime);

				MSGStateChangeToPE(statechangetope);
			}
			else if(statechange->ucMode == state_change_en_enter_tutorial )
			{
				// Ʃ�丮�� ��忡 ������� ��� ��Ŷ 
				MSG_STATE_CHANGE_ENTER_TURIAL* entertutorial = (MSG_STATE_CHANGE_ENTER_TURIAL*)pPacket;
				_XPACKETSIZEERROR(&entertutorial->header, MSG_STATE_CHANGE_ENTER_TURIAL);
				
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : Tutorial Mode Start [%d]", g_LocalSystemTime);
				
				MSGStateEnterTutorial(entertutorial);
			}
			else if(statechange->ucMode == state_change_en_leave_tutorial )
			{
				// Ʃ�丮�� ��忡�� ������� ��� ��Ŷ 
				MSG_STATE_CHANGE_LEAVE_TUTORIAL* leavetutorial = (MSG_STATE_CHANGE_LEAVE_TUTORIAL*)pPacket;
				_XPACKETSIZEERROR(&leavetutorial ->header, MSG_STATE_CHANGE_LEAVE_TUTORIAL);
				
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : Tutorial Mode Exit [%d]", g_LocalSystemTime);
				
				MSGStateLeaveTutorial(leavetutorial);
			}
			else if(statechange->ucMode == state_change_en_saferegion_notice)
			{
				// ���� ����
				MSG_STATE_CHANGE_SAFEREGION_NOTICE* saferegionnotice = (MSG_STATE_CHANGE_SAFEREGION_NOTICE*)pPacket;
				_XPACKETSIZEERROR(&saferegionnotice->header, MSG_STATE_CHANGE_SAFEREGION_NOTICE);

				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : Safe region notice");

				MSGStateChangeSafeRegionNotice(saferegionnotice);
			}
	#ifdef _XDEF_PARTY_NOTAVAILABLE
			else if(statechange->ucMode == state_change_en_to_normal_state ||statechange->ucMode == state_change_en_to_absent_state)
			{
				// �ڸ� ��� ó��
				MSG_STATE_CHANGE_ABSENT* statechangeabsent = (MSG_STATE_CHANGE_ABSENT*)pPacket;
				_XPACKETSIZEERROR(&statechangeabsent->header, MSG_STATE_CHANGE_ABSENT);

				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : State change absent ");

				MSGStateChangeAbsent(statechangeabsent);
			}
	#endif

	#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
			else if(statechange->ucMode == state_change_en_clothview_select_deck || statechange->ucMode == state_change_en_clothview_select_normal)
			{
				MSG_STATE_CHANGE_CLOTHVIEW_SELECT_DECK* statechangeclothes = (MSG_STATE_CHANGE_CLOTHVIEW_SELECT_DECK*)pPacket;

				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : state change select deck");

				MSGStateChangeSelectClothes(statechangeclothes);
			}
	#endif

			else
			{
				MSG_STATE_CHANGE_RESULT* statechangeresult = (MSG_STATE_CHANGE_RESULT*)pPacket;
				_XPACKETSIZEERROR(&statechangeresult->header, MSG_STATE_CHANGE_RESULT);
				
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : State change result [%d]", g_LocalSystemTime);
				
				MSGStateChangeResult(statechangeresult);
			}

#else
			MSG_STATE_CHANGE_RESULT* statechangeresult = (MSG_STATE_CHANGE_RESULT*)pPacket;
			_XPACKETSIZEERROR(&statechangeresult->header, MSG_STATE_CHANGE_RESULT);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : State change result [%d]", g_LocalSystemTime);

			MSGStateChangeResult(statechangeresult);
#endif
		}
		break;
		
	case MSG_NO_POINT_USE :
		{			
			MSG_POINT_USE_RESULT* pointuseresult = (MSG_POINT_USE_RESULT*)pPacket;
			_XPACKETSIZEERROR( &pointuseresult->header, MSG_POINT_USE_RESULT);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE :  point use [ %d %d %d %d ] [%d]", pointuseresult->cWhere, pointuseresult->sPoint, pointuseresult->sPower, pointuseresult->sLeftPoint, g_LocalSystemTime);
			
			MSGPointUse(pointuseresult);
		}
		break;

	case MSG_NO_SKILL_INFO :
		{
			MSG_SKILL_INFO* skillinfolist = (MSG_SKILL_INFO*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Skill info [%d]", g_LocalSystemTime);			

			MSGSkillInfo(skillinfolist);
		}		
		break;

	case MSG_NO_STORAGE_INFO :
		{
			MSG_STR_ITEM_INFO* storagelist = (MSG_STR_ITEM_INFO*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Storage info");
			
			MSGStorageInfo(storagelist);
		}
		break;	
		
	case MSG_NO_STR_ITEM_SAVE_RESULT :
		{
			MSG_STR_ITEM_SAVE* stritemsave = (MSG_STR_ITEM_SAVE*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Iventory -> Storage");
			
			MSGStrItemSaveResult(stritemsave);
		}
		break;	
		
	case MSG_NO_STR_ITEM_EJECT_RESULT :
		{			
			MSG_STR_ITEM_EJECT* stritemsave = (MSG_STR_ITEM_EJECT*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Storage -> Inventory");
			
			MSGStrItemEjectResult(stritemsave);
		}
		break;

	case MSG_NO_STR_ITEM_EJECTEX_RESULT	:
		{			
			MSG_STR_ITEM_EJECTEX* stritemeject = (MSG_STR_ITEM_EJECTEX*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Storage -> Inventory : Terminate State");
			
			MSGStrItemEjectTerminateResult(stritemeject);
		}
		break;
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : ����� breif : ��������
	case MSG_NO_ITEM_SOCKET_GAMBLE :
		{
			MSG_SOCKET_GAMBLE* socketresult = (MSG_SOCKET_GAMBLE*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Item Equip Socket Result [%d]", g_LocalSystemTime);

			MSGItemEquipSocketResult(socketresult);
		}
		break;
#else
	// 2004.11.9->hotblood insert
	case MSG_NO_ITEM_EQUIP_SOCKET_RESULT :
		{
			MSG_ITEM_EQUIP_SOCKET_RESULT* socketresult = (MSG_ITEM_EQUIP_SOCKET_RESULT*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Item Equip Socket Result [%d]", g_LocalSystemTime);

			MSGItemEquipSocketResult(socketresult);
		}
		break;
#endif
	case MSG_NO_INVEN_INFO :
		{
			MSG_INVEN_INFO* inventorylist = (MSG_INVEN_INFO*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Inventory info [%d]", g_LocalSystemTime);

			MSGInvenInfo(inventorylist);
		}
		break;	

	case MSG_NO_INVEN_MOVE :
		{
			MSG_INVEN_MOVE* invenmove = (MSG_INVEN_MOVE*)pPacket;
			_XPACKETSIZEERROR(&invenmove->header, MSG_INVEN_MOVE);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Inventory move [%d]", g_LocalSystemTime);

			MSGInvenMove(invenmove);
		}
		break;

	case MSG_NO_INVEN_DROP :
		{
			MSG_INVEN_DROP* inventoryitemdrop = (MSG_INVEN_DROP *)pPacket;
			_XPACKETSIZEERROR(&inventoryitemdrop->header, MSG_INVEN_DROP);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Inventory item drop [%d]", g_LocalSystemTime);

			MSGInvenDrop(inventoryitemdrop);
		}		
		break;

	// TempInven ���� ��Ŷ 2005.02.23->hotblood ---------------------------------------------------=	
	case MSG_NO_TEMP_INVEN :
		{
			MSG_TEMP_INVEN* tempinven = (MSG_TEMP_INVEN *)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : TempInven [%d]", g_LocalSystemTime);

#ifdef _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI
			if(tempinven->cTo == 0)
			{
				MSG_TEMP_INVEN_MOVE* invenmove = (MSG_TEMP_INVEN_MOVE*)pPacket;
				MSGTempInvenMove(invenmove);
			}
			else
			{
				MSG_TEMP_INVEN_INFO* inveninfo = (MSG_TEMP_INVEN_INFO*)pPacket;
				MSGTempInvenInfo(inveninfo);
			}

#else
			MSGTempInven(tempinven);
#endif
		}
		break;
	case MSG_NO_TEMP_INVEN_CTRL_RESULT :		
		{
			MSG_TEMP_INVEN_CTRL_RESULT* tempinvenresult = (MSG_TEMP_INVEN_CTRL_RESULT *)pPacket;
			_XPACKETSIZEERROR(&tempinvenresult->header, MSG_TEMP_INVEN_CTRL_RESULT);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : TempInven Result [%d]", g_LocalSystemTime);

			MSGTempInvenCtrlResult(tempinvenresult);
		}
		break;
	// --------------------------------------------------------------------------------------------=
	// ���� ���� ����
	case MSG_NO_ZONE_RESTART :
		{	
			MSG_ZONE_RESTART* zonerestart = (MSG_ZONE_RESTART *)pPacket;
			_XPACKETSIZEERROR(&zonerestart->header, MSG_ZONE_RESTART);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Zone Restart [%d]", g_LocalSystemTime);
			
			MSGZoneRestart(zonerestart);
		}		
		break;

	case MSG_NO_LOGOUT :
		{
			MSG_LOGOUT * logout = (MSG_LOGOUT *)pPacket;
			_XPACKETSIZEERROR( &logout->header, MSG_LOGOUT);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Logout [%d]", g_LocalSystemTime);

			MSGLogout(logout);
		}
		break;

	case MSG_NO_SV_TOGSERVER :
		{
			MSG_SV_TOGSERVER movetoserver;
			memcpy(&movetoserver, pPacket, sizeof(MSG_SV_TOGSERVER));
			_XPACKETSIZEERROR( &movetoserver.header, MSG_SV_TOGSERVER);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Move Server [%d]", g_LocalSystemTime);

			MSGSVToGServer(movetoserver);

			return FALSE;
		}
		break;

	case MSG_NO_ZONE_POSITIONING :
		{
			MSG_ZONE_POSITIONING* zonepositioning = (MSG_ZONE_POSITIONING*)pPacket;
			_XPACKETSIZEERROR( &zonepositioning->header, MSG_ZONE_POSITIONING);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Zone positioning [%d]", g_LocalSystemTime);
						
			MSGZonePositioning(zonepositioning);			
		}
		break;

	case MSG_NO_ZONE_MOVE :
		{
			MSG_ZONE_MOVE* charactermove = (MSG_ZONE_MOVE*)pPacket;
			_XPACKETSIZEERROR( &charactermove->header, MSG_ZONE_MOVE);

			MSGZoneMove(charactermove);
		}
		break;

	case MSG_NO_ZONE_CHARAC_INFO :
		{
			MSG_ZONE_CHARAC_INFO* characterinfo = (MSG_ZONE_CHARAC_INFO*)pPacket;
			_XPACKETSIZEERROR( &characterinfo->header, MSG_ZONE_CHARAC_INFO);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Zone character info [%d]", g_LocalSystemTime);
			
			MSGZoneCharacInfo(characterinfo);
		}
		break;

	case MSG_NO_MONSTER_MOVE :
		{
			MSG_MONSTER_MOVE* monstermove = (MSG_MONSTER_MOVE*)pPacket;
			_XPACKETSIZEERROR( &monstermove->header, MSG_MONSTER_MOVE);

			MSGMonsterMove(monstermove);
		}
		break;

	case MSG_NO_MONSTER_VITAL :
		{
			MSG_MONSTER_VITAL* monstervital = (MSG_MONSTER_VITAL*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Monster vital [%d / %d] [%d]", monstervital->usMobID, monstervital->cMobVitalRate, g_LocalSystemTime);

			MSGMonsterVital(monstervital);
		}
		break;
		
	case MSG_NO_MONSTER_TRANSFORM :
		{
			MSG_MONSTER_TRANSFORM* monstertransform = (MSG_MONSTER_TRANSFORM*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Monster transform id: %d md:%d t:%d v:%d ", monstertransform->usMobID, monstertransform->cChangeMode, monstertransform->usChangeType, monstertransform->cMobVitalRate );

			MSGMonsterTransform(monstertransform);
		}
		break;
		
	case MSG_NO_MONSTER_EXIST :
		{
			MSG_MONSTER_EXIST* monsterexist = (MSG_MONSTER_EXIST*)pPacket;
			MSGMonsterExist(monsterexist);
		}
		break;

	case MSG_NO_RES_SPELLEFFECT :
		{
			MSG_RES_SPELLEFFECT* spelleffect = (MSG_RES_SPELLEFFECT *)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Monster Spell Effect  id:%d ", spelleffect->usMobID);

			MSGResSpellEffect(spelleffect);
		}
		break;

	case MSG_NO_SPELLLIST_CHANGED :
		{
			MSG_SPELLLIST_CHANGED* spellchanged = (MSG_SPELLLIST_CHANGED*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Monster Spell List Changed   id:%d", spellchanged->usMobID);

			MSGSpellListChanged(spellchanged);
		}
		break;

	case MSG_NO_CONDITION_CHANGED :
		{
			MSG_CONDITION_CHANGED* conditionchanged = (MSG_CONDITION_CHANGED*)pPacket;

			VIEWPACKETLOG 
				_XDWPACKETLOG("RECEIVE MESSAGE : Monster Condition Changed   id:%d", conditionchanged->usMobID);

			MSGConditionChanged(conditionchanged);
		}
		break;
		
	case MSG_NO_MONSTER_CHASE_NOTICE :
		{
			MSG_MONSTER_CHASE_NOTICE* monsterchase = (MSG_MONSTER_CHASE_NOTICE*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Monster Chase id:%d", monsterchase->usDNPCID);

			MSGMonsterChaseNotice(monsterchase);
		}
		break;

	case MSG_NO_B_ATKRES_FROM_MOB :
		{
			MSG_B_ATKRES_FROM_MOB_WITH_POS* monsterattack = (MSG_B_ATKRES_FROM_MOB_WITH_POS*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Monster Attack[%d] Defender[%d] Damage[%d] [%d]", monsterattack->usAttackerID, monsterattack->usDefenderID, monsterattack->sDamage, g_LocalSystemTime);

			MSGBattleAtkResFromMobWithPos(monsterattack);
		}
		break;

	case MSG_NO_REGION_DAMAGE :
		{
			MSG_REGION_DAMAGE* regiondamage = (MSG_REGION_DAMAGE*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Region Damage [%d]", g_LocalSystemTime);

			MSGRegionDamage(regiondamage);
		}
		break;

	case MSG_NO_SELF_CHANNEL_CTRL :
		{
			MSG_SELF_CHANNEL_CTRL* selfchannelctrl = (MSG_SELF_CHANNEL_CTRL*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Self Channel Ctrl [%d]", g_LocalSystemTime);
			
			MSGSelfChannelCtrl(selfchannelctrl);
		}
		break;

	case MSG_NO_SELF_CHANNEL_INFO :
		{
			MSG_SELF_CHANNEL_INFO* selfchannelinfo = (MSG_SELF_CHANNEL_INFO*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Self Channel Info [%d]", g_LocalSystemTime);
			
			MSGSelfChannelInfo(selfchannelinfo);
		}
		break;
		// �̴� ���� ����
	case MSG_NO_MINIGAME_CTRL :
		{
			MSG_MINIGAME_CTRL* minigamectrl = (MSG_MINIGAME_CTRL*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : MiniGame Ctrl [%d]", g_LocalSystemTime);

			MSGMiniGameCtrl(minigamectrl);
		}
		break;

	case MSG_NO_MINIGAME_MISSION_STAGE_S_C :
		{
			MSG_MINIGAME_MISSION_STAGE* missionstage = (MSG_MINIGAME_MISSION_STAGE*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : MiniGame Mission stage [%d]", g_LocalSystemTime);

			MSGMiniGameMissionStage(missionstage);
		}
		break;

	case MSG_NO_MINIGAME_FAIL_S_C :
		{
			MSG_MINIGAME_FAIL* minigamefail = (MSG_MINIGAME_FAIL*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : MiniGame Fail [%d]", g_LocalSystemTime);

			MSGMiniGameFail(minigamefail);
		}
		break;

	case MSG_NO_MINIGAME_RESULT :
		{
			MSG_MINIGAME_RESULT* minigameresult = (MSG_MINIGAME_RESULT*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : MiniGame Result [%d]", g_LocalSystemTime);

			MSGMiniGameResult(minigameresult);
		}
		break;

		// ���� ��Ŷ ����
	case MSG_NO_MARTIAL_READY :
		{
			MARTIAL_READY* martialready = (MARTIAL_READY*)pPacket;

			switch(martialready->ucMartialType)
			{
			case en_martial_ut_charge_skill :
				{
					MSG_CHARGE_READY_RES* chargeready = (MSG_CHARGE_READY_RES*)pPacket;

					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Charge Ready Result [%d]", g_LocalSystemTime);

					MSGMartialChargeReadyResult(chargeready);
				}
				break;
			case en_martial_ut_channeling_spell :
				{
					if(martialready->ucTargetType == en_target_type_self)
					{
						MSG_CHANNEL_READY_RES_SELF* channelreadyres = (MSG_CHANNEL_READY_RES_SELF*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Channel Ready Result Self");

						MSGMartialChannelReadyResultSelf(channelreadyres);
					}
					else if(martialready->ucTargetType == en_target_type_point)
					{
						MSG_CHANNEL_READY_RES_POINT* channelreadyres = (MSG_CHANNEL_READY_RES_POINT*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Channel Ready Result Point");

						MSGMartialChannelReadyResultPoint(channelreadyres);
					}
					else
					{
						MSG_CHANNEL_READY_RES_PC* channelreadyres = (MSG_CHANNEL_READY_RES_PC*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Channel Ready Result PC");

						MSGMartialChannelReadyResultPC(channelreadyres);
					}
					
					/*if(martialready->ucTargetType == en_target_type_point)
					{
						MSG_CHANNEL_READY_RES_POINT* channelreadyresult = (MSG_CHANNEL_READY_RES_POINT*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Channel Ready Result Point [%d]", g_LocalSystemTime);

						MSGMartialChannelReadyResultPoint(channelreadyresult);
					}
					else
					{
						MSG_CHANNEL_READY_RES_PC* channelreadyresult = (MSG_CHANNEL_READY_RES_PC*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Channel Ready Result PC [%d]", g_LocalSystemTime);
						
						MSGMartialChannelReadyResultPC(channelreadyresult);
					}*/
				}
				break;
			case en_martial_ut_finish_skill :
				{
					MSG_FINISH_READY_RES* finishreadyresult = (MSG_FINISH_READY_RES*)pPacket;

					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Finish Ready Result [%d]", g_LocalSystemTime);

					MSGMartialFinishReadyResult(finishreadyresult);
				}
				break;
			case en_martial_ut_casting_spell :
				{
					if(martialready->ucTargetType == en_target_type_point)
					{
						MSG_CASTING_READY_RES_TO_POINT* castingreadyresult = (MSG_CASTING_READY_RES_TO_POINT*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Casting Ready Result TO POINT [%d]", g_LocalSystemTime);
						
						MSGMartialCastingReadyResultPoint(castingreadyresult);
					}
					else
					{
						MSG_CASTING_READY_RES_TO_PC* castingreadyresult = (MSG_CASTING_READY_RES_TO_PC*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Casting Ready Result TO PC [%d]", g_LocalSystemTime);
						
						MSGMartialCastingReadyResultPC(castingreadyresult);
					}
				}
				break;
			}
		}
		break;
	case MSG_NO_MARTIAL_FIRE :
		{
			MARTIAL_FIRE* martialfire = (MARTIAL_FIRE*)pPacket;

			switch(martialfire->ucMartialType)
			{
			case en_martial_ut_rounding_spell :
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Rounding Fire Result [%d]", g_LocalSystemTime);

					if(martialfire->ucTargetType == en_target_type_point)
					{
						MSG_ROUNDING_FIRE_RES_TO_POINT* roundingfireresult = (MSG_ROUNDING_FIRE_RES_TO_POINT*)pPacket;
						MSGMartialRoundingFireResToPoint(roundingfireresult);
					}
					else
					{
						MSG_ROUNDING_FIRE_RES* roundingfireresult = (MSG_ROUNDING_FIRE_RES*)pPacket;
						MSGMartialRoundingFireResult(roundingfireresult);
					}
				}
				break;
			}
		}
		break;
	case MSG_NO_MARTIAL_RESULT :
		{
			MARTIAL_RESULT* martialresult = (MARTIAL_RESULT*)pPacket;
			
			switch(martialresult->ucMartialType)
			{
			case en_martial_ut_passive_skill :
				break;
			case en_martial_ut_active_skill :
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Active Martial Result [%d]", g_LocalSystemTime);
					
					if(martialresult->ucTargetType == en_target_type_others)
					{
						MSG_ACTIVE_RESULT_PVP* activeresultpvp = (MSG_ACTIVE_RESULT_PVP*)pPacket;
						MSGMartialActiveResultPVP(activeresultpvp);
					}
					else
					{
						MSG_ACTIVE_RESULT_MOB* activeresult = (MSG_ACTIVE_RESULT_MOB*)pPacket;
						MSGMartialActiveResultMob(activeresult);
					}
				}
				break;
			case en_martial_ut_charge_skill :
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Charge Martial Result [%d]", g_LocalSystemTime);

					if(martialresult->ucTargetType == en_target_type_others)
					{
						MSG_CHARGE_RESULT_PVP* chargeresult = (MSG_CHARGE_RESULT_PVP*)pPacket;
						MSGMartialChargeResultPVP(chargeresult);
					}
					else
					{
						MSG_CHARGE_RESULT_MOB* chargeresult = (MSG_CHARGE_RESULT_MOB*)pPacket;
						MSGMartialChargeResultMob(chargeresult);
					}
				}
				break;
			case en_martial_ut_passive_spell :
				break;
			case en_martial_ut_casting_spell :
				{
					if(martialresult->ucTargetType == en_target_type_point)
					{
						MSG_CASTING_RESULT_TO_POINT* castingresultpoint = (MSG_CASTING_RESULT_TO_POINT*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Casting Martial Result To Point [%d]", g_LocalSystemTime);

						MSGMartialCastingResultToPoint(castingresultpoint);
					}
					else if(martialresult->ucTargetType == en_target_type_others)
					{
						MSG_CASTING_RESULT_TO_PVP* castingresult = (MSG_CASTING_RESULT_TO_PVP*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Cating Martial Result To PVP [%d]", g_LocalSystemTime);

						MSGMartialCastingResultToPVP(castingresult);
					}
					else
					{
						MSG_CASTING_RESULT_TO_MOB* castingresult = (MSG_CASTING_RESULT_TO_MOB*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Casting Martial Result To Mob [%d]", g_LocalSystemTime);
						
						MSGMartialCastingResultToMob(castingresult);
					}
				}
				break;
			case en_martial_ut_channeling_spell :
				{
					if(martialresult->ucTargetType == en_target_type_point)
					{
						MSG_CHANNEL_RESULT_POINT* channelresult = (MSG_CHANNEL_RESULT_POINT*)pPacket; 

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Channeling Martial Result Point");

						MSGMartialChannelingResultPoint(channelresult);
					}
					else if(martialresult->ucTargetType == en_target_type_others || martialresult->ucTargetType == en_target_type_self)
					{
						MSG_CHANNEL_RESULT_PC* channelresult = (MSG_CHANNEL_RESULT_PC*)pPacket;
						
						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Channeling Martial Result PC");
						
						MSGMartialChannelingResultPC(channelresult);
					}
					else
					{
						MSG_CHANNEL_RESULT_NPC* channelresult = (MSG_CHANNEL_RESULT_NPC*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Channeling Martial Result NPC");

						MSGMartialChannelingResultNPC(channelresult);
					}
				}
				break;
			case en_martial_ut_finish_skill :
				{
					if(martialresult->ucTargetType == en_target_type_others)
					{
						MSG_FINISH_RESULT_TO_PC* finishresult = (MSG_FINISH_RESULT_TO_PC*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Finish Martial Result To PC [%d]", g_LocalSystemTime);

						MSGMartialFinishResultPC(finishresult);
					}
					else
					{
						MSG_FINISH_RESULT_TO_MOB* finishresult = (MSG_FINISH_RESULT_TO_MOB*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Finish Martial Result To Mob [%d]", g_LocalSystemTime);
						
						MSGMartialFinishResultMob(finishresult);
					}
				}
				break;
			case en_martial_ut_rounding_spell :
				{
					if(martialresult->ucTargetType == en_target_type_others || martialresult->ucTargetType == en_target_type_self)
					{
						MSG_ROUNDING_RESULT_TO_PC* roundingresult = (MSG_ROUNDING_RESULT_TO_PC*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Rounding Martial Result To PC [%d]", g_LocalSystemTime);

						MSGMartialRoundingResultPC(roundingresult);
					}
					else if(martialresult->ucTargetType == en_target_type_monster || martialresult->ucTargetType == en_target_type_object)
					{
						MSG_ROUNDING_RESULT_TO_MOB* roundingresult = (MSG_ROUNDING_RESULT_TO_MOB*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Rounding Martial Result To Mob [%d]", g_LocalSystemTime);
						
						MSGMartialRoundingResultMob(roundingresult);
					}
					else if(martialresult->ucTargetType == en_target_type_point)
					{
						MSG_ROUNDING_RESULT_TO_POINT* roundingresult = (MSG_ROUNDING_RESULT_TO_POINT*)pPacket;

						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Rounding Martial Result to Point");

						MSGMartialRoundingResultPoint(roundingresult);
					}
				}
				break;
			case en_martial_ut_smashing_skill :
				{
					if(martialresult->ucTargetType == en_target_type_monster || martialresult->ucTargetType == en_target_type_object)
					{
						MSG_SMASHING_RESULT_MOB* smashingresult = (MSG_SMASHING_RESULT_MOB*)pPacket;
						
						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Smashing Fire Result To Mob [%d]", g_LocalSystemTime);
						
						MSGMartialSmashingResultMob(smashingresult);
					}
					else if(martialresult->ucTargetType == en_target_type_others)
					{
						MSG_SMASHING_RESULT_PVP* smashingresult = (MSG_SMASHING_RESULT_PVP*)pPacket;
						
						VIEWPACKETLOG
							_XDWPACKETLOG("RECEIVE MESSAGE : Smashing Fire Result To PVP [%d]", g_LocalSystemTime);
						
						MSGMartialSmashingResultPVP(smashingresult);
					}
				}
				break;
			case en_martial_ut_action_skill :
				{
					MSG_ACTION_RESULT* actionresult = (MSG_ACTION_RESULT*)pPacket;

					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Action Result [%d]", g_LocalSystemTime);

					MSGMartialActionResult(actionresult);

				}
				break;
			}
		}
		break;
	case MSG_NO_MARTIAL_END :
		{
			MARTIAL_END* martialend = (MARTIAL_END*)pPacket;

			switch(martialend->ucMartialType)
			{
			case en_martial_ut_charge_skill :
				{
					MSG_CHARGE_END* chargeend = (MSG_CHARGE_END*)pPacket;

					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Charge Skill End [%d]", g_LocalSystemTime);

					MSGMartialChargeEnd(chargeend);
				}
				break;
			case en_martial_ut_channeling_spell :
				{
					MSG_CHANNEL_END* channelend = (MSG_CHANNEL_END*)pPacket;

					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Channel Spell End [%d]", g_LocalSystemTime);
					
					MSGMartialChannelEnd(channelend);
				}
				break;
			case en_martial_ut_finish_skill :
				{
					MSG_FINISH_END* finishend = (MSG_FINISH_END*)pPacket;

					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Finish Skill End [%d]", g_LocalSystemTime);

					MSGMartialFinishEnd(finishend);
				}
				break;
			case en_martial_ut_casting_spell :
				{
					MSG_CASTING_END* castingend = (MSG_CASTING_END*)pPacket;

					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Casting Spell End [%d]", g_LocalSystemTime);
					
					MSGMartialCastingEnd(castingend);
				}
				break;
			case en_martial_ut_rounding_spell :
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Roudning Spell End [%d]", g_LocalSystemTime);

					MSGMartialRoundingEnd(martialend);
				}
				break;
			case en_martial_ut_smashing_skill :
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Smashing Skill End [%d]", g_LocalSystemTime);

					MSGMartialSmashingEnd(martialend);
				}
				break;
			default :
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Active or Hidden MA End [%d]", g_LocalSystemTime);

					MSGMartialEnd(martialend);
				}
				break;
			}
		}
		break;
		
	case MSG_NO_MONSTER_WARP :
		{
			MSG_MONSTER_WARP* monsterwarp = (MSG_MONSTER_WARP*)pPacket;
			_XPACKETSIZEERROR(&monsterwarp->header, MSG_MONSTER_WARP);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Monster warp [%d]", g_LocalSystemTime);

			MSGMonsterWarp(monsterwarp);
		}
		break;

		// ���� ó��
	case MSG_NO_CHARAC_CTRL :
		{
			MSG_CHARAC_CTRL* characctrl = (MSG_CHARAC_CTRL*)pPacket;
			_XPACKETSIZEERROR(&characctrl->header, MSG_CHARAC_CTRL);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Charac Ctrl [%d]", g_LocalSystemTime);
			
			MSGCharacCtrl(characctrl);
		}
		break;

	case MSG_NO_CHARAC_REBIRTH_TARGET :
		{
			MSG_CHARAC_REBIRTH_TARGET* characrebirthtarget = (MSG_CHARAC_REBIRTH_TARGET*)pPacket;
			_XPACKETSIZEERROR(&characrebirthtarget->header, MSG_CHARAC_REBIRTH_TARGET);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Charac Rebirth target [%d]", g_LocalSystemTime);
			
			MSGCharacRebirthTarget(characrebirthtarget);
		}
		break;

	case MSG_NO_SAVE_RESPAWN_POS_RESULT :
		{
			MSG_SAVE_RESPAWN_POS_RESULT* respawnposresult = (MSG_SAVE_RESPAWN_POS_RESULT*)pPacket;
			_XPACKETSIZEERROR(&respawnposresult->header, MSG_SAVE_RESPAWN_POS_RESULT);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Respawn pos result [%d]", g_LocalSystemTime);

			MSGSaveRespawnPosResult(respawnposresult);
		}
		break;

	case MSG_NO_ZONE_CHAT_NORMAL :
		{
			MSG_ZONE_CHAT_NORMAL* chatnormal = (MSG_ZONE_CHAT_NORMAL*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Chat message [%d]", g_LocalSystemTime);
									
			MSGZoneChatNormal(chatnormal);
		}
		break;

		// ������ ó��
	case MSG_NO_EQUIP_CHANGE	:
		{			
			MSG_EQUIP_CHANGE *itemequip = (MSG_EQUIP_CHANGE*)pPacket;
			_XPACKETSIZEERROR( &itemequip->header, MSG_EQUIP_CHANGE);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Item equip [%d]", g_LocalSystemTime);

			MSGItemEquipChange(itemequip);
		}
		break;

	case MSG_NO_ITEM_DROP : 
		{			
			MSG_ITEM_DROP *itemdrop = (MSG_ITEM_DROP*)pPacket;

			int size = 6 + itemdrop->cNoItems * sizeof(_sItem_Drop);
			if( size != itemdrop->header.sLength )
			{
				_XFatalError( "PACKET SIZE ERROR : ITEM_DROP [header length : %d] [org length : %d]", itemdrop->header.sLength, size );
			}

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Item drop [%d]", g_LocalSystemTime);

			MSGItemDrop(itemdrop);
		}
		break;

	case MSG_NO_MONEY_DROP :
		{
			MSG_MONEY_DROP* moneydrop = (MSG_MONEY_DROP*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Money Drop [%d]", g_LocalSystemTime);
			
			MSGMoneyDrop(moneydrop);
		}
		break;

	case MSG_NO_ITEM_REMOVE :
		{
			MSG_ITEM_REMOVE *itemremove = (MSG_ITEM_REMOVE*)pPacket;			
			_XPACKETSIZEERROR(&itemremove->header, MSG_ITEM_REMOVE);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : item remove [%d]", g_LocalSystemTime);

			MSGItemRemove(itemremove);
		}
		break;

	case MSG_NO_ITEM_GRAB_SUCCESS :
		{
			MSG_ITEM_GRAB_SUCCESS *successgrabitem = (MSG_ITEM_GRAB_SUCCESS*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : item grab success [%d]", g_LocalSystemTime);
			
			MSGItemGrabSuccess(successgrabitem);
		}
		break;
		
	case MSG_NO_MONEY_GRAB_SUCCESS :
		{
			MSG_MONEY_GRAB_SUCCESS* successgrabmoney = (MSG_MONEY_GRAB_SUCCESS*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : money grab success [%d]", g_LocalSystemTime);
			
			MSGMoneyGrabSuccess(successgrabmoney);
		}
		break;

	case MSG_NO_WORLD_ITEM_INFO :
		{
			MSG_WORLD_ITEM* worlditeminfo = (MSG_WORLD_ITEM*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : world item info [%d]", g_LocalSystemTime);
			
			MSGWorldItemInfo(worlditeminfo);
		}
		break;

	case MSG_NO_INVEN_CTRL :
		{
			MSG_INVEN_CTRL_RESULT* invenctrlresult = (MSG_INVEN_CTRL_RESULT*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Inven ctrl result [%d]", g_LocalSystemTime);
			
			MSGInvenCtrlResult(invenctrlresult);
		}
		break;

		// ������ ���� ��� ����
	case MSG_NO_NPC_ITEM_REPAIR_RESULT :
		{
			MSG_NPC_ITEM_REPAIR_RESULT* repairitemresult = (MSG_NPC_ITEM_REPAIR_RESULT*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Repair Item result [%d]", g_LocalSystemTime);
			
			MSGRepairItemResult(repairitemresult);
		}
		break;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   ����Ʈ ���� ��Ŷ ��� 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// ����Ʈ ó�� 
	case MSG_NO_QUEST_SCON_RESULT :
		{
			MSG_QUEST_AUTH_RESULT* questresult = (MSG_QUEST_AUTH_RESULT*)pPacket;

			_XPACKETSIZEERROR( &questresult->header, MSG_QUEST_AUTH_RESULT);

			_XDWPACKETLOG("RECIEVE MESSAGE : Quest SCON Result message [Qid:%d / TreeNo:%d] [%d]", questresult->usQuestID, questresult->cNodeID, g_LocalSystemTime);
			_XDWPACKETLOG("RECIEVE MESSAGE : Quest SCON Result [Result:%d / CInfo:%d] ", questresult->cResult, questresult->cInfo);

			MSGQuestSConResult(questresult);
		}
		break;
	case MSG_NO_QUEST_SET_RESULT :
		{
			MSG_QUEST_SET_RESULT* questresult = (MSG_QUEST_SET_RESULT*)pPacket;
			_XPACKETSIZEERROR( &questresult->header, MSG_QUEST_SET_RESULT);
			_XDWPACKETLOG("RECEIVE MESSAGE : Quest Set Result [Qid:%d / Tid:%d] [%d]", questresult->usQuestID, questresult->cResult, g_LocalSystemTime);
			
			MSGQuestSetResult(questresult);
		}
		break;		
		
		// ��Ƽ ����Ʈ ��ҽÿ� �޴� ��Ŷ 
	case MSG_NO_QUEST_USER_CANCEL :
		{
			MSG_QUEST_USER_CANCEL* partyquestcancle = (MSG_QUEST_USER_CANCEL*)pPacket;
			_XPACKETSIZEERROR( &partyquestcancle->header, MSG_QUEST_USER_CANCEL);
			_XDWPACKETLOG("RECEIVE MESSAGE : Party Quest Cancle [Qid:%d ] [%d]", partyquestcancle->usQuestID, g_LocalSystemTime);
			
			MSGPartyQuestCancle(partyquestcancle);
		}
		break;		
		
	// ����Ʈ ���� ��Ȳ ��Ŷó�� : ������
	case MSG_NO_QUEST_DATA :
		{
			MSG_QUEST_DATA* questdata = (MSG_QUEST_DATA *)pPacket;
			_XDWPACKETLOG("RECEIVE MESSAGE : Quest Data [%d]", g_LocalSystemTime);
			
			MSGQuestData(questdata);
		}
		break;
		
		// ����Ʈ �Ϸ�� ���� ó�� ��Ŷ : ������
	case MSG_NO_QUEST_COMPLETE_DATA :
		{
			MSG_QUESTCOMPLETE_DATA* questcompletedata = (MSG_QUESTCOMPLETE_DATA *)pPacket;
			_XDWPACKETLOG("RECEIVE MESSAGE : Quest Data [%d]", g_LocalSystemTime);
			
			MSGQuestCompleteData(questcompletedata);
		}
		break;
		
	// ����Ʈ ������ ȹ��
	case MSG_NO_QUESTITEM_RECEIVE :
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : quest item receive [%d]", g_LocalSystemTime);
			MSG_QUESTITEM_RECEIVE* questitem = (MSG_QUESTITEM_RECEIVE *)pPacket;
			_XPACKETSIZEERROR( &questitem->header, MSG_QUESTITEM_RECEIVE);
			
			QuestItemReceive(questitem);
		}
		break;
		
	// ����Ʈ ������ ����
	case MSG_NO_QUESTITEM_REMOVE :
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : quest item remove [%d]", g_LocalSystemTime);
			MSG_QUESTITEM_REMOVE* questitem = (MSG_QUESTITEM_REMOVE *)pPacket;
			
			QuestItemRemove(questitem);
		}
		break;
	// ����Ʈ ī���� ���
	case MSG_NO_QUEST_SETCOUNT_RESULT:
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : quest count result [%d]", g_LocalSystemTime);
			MSG_QUEST_SETCOUNT_RESULT* questcount = (MSG_QUEST_SETCOUNT_RESULT *)pPacket;
			_XPACKETSIZEERROR( &questcount->header, MSG_QUEST_SETCOUNT_RESULT);
			
			MSGQuestCounterResult(questcount);
		}
		break;
		
	// ��Ƽ����Ʈ ī���� ���
	case MSG_NO_QUEST_PARTY_SETCOUNT_RESULT	:
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : party quest count result [%d]", g_LocalSystemTime);
			MSG_QUEST_PARTY_SETCOUNT_RESULT* partyquestcount = (MSG_QUEST_PARTY_SETCOUNT_RESULT *)pPacket;
			_XPACKETSIZEERROR( &partyquestcount->header, MSG_QUEST_PARTY_SETCOUNT_RESULT);
			
			MSGPartyQuestCounterResult(partyquestcount);
		}
		break;
		
	// ����Ʈ ��ŷ ����Ʈ ���
	case MSG_NO_QUESTRANK_RES	:
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : ranking quest result [%d]", g_LocalSystemTime);
			MSG_QUESTRANK_RES* questrankinglist = (MSG_QUESTRANK_RES *)pPacket;
			_XPACKETSIZEERROR( &questrankinglist->header, MSG_QUESTRANK_RES);
			
			MSGQuestRankingListResult(questrankinglist);
		}
		break;
		
	// ����Ʈ�� ��ȿȭ �˻� ��� 
	case MSG_NO_QUEST_VALIDCHECK_RESULT:
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : quest check result [%d]", g_LocalSystemTime);
			MSG_QUEST_VALIDCHECK_RESULT* questcheck = (MSG_QUEST_VALIDCHECK_RESULT *)pPacket;
			_XPACKETSIZEERROR( &questcheck->header, MSG_QUEST_VALIDCHECK_RESULT);
			
			MSGQuestValidCheckResult(questcheck);
		}
		break;
		
	case MSG_NO_QUEST_VALIDCHECK_INGAME_RESULT:
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : quest check result [%d]", g_LocalSystemTime);
			MSG_QUEST_VALIDCHECK_INGAME_RESULT* questcheck = (MSG_QUEST_VALIDCHECK_INGAME_RESULT *)pPacket;
			_XPACKETSIZEERROR( &questcheck->header, MSG_QUEST_VALIDCHECK_INGAME_RESULT);
			
			MSGQuestValidCheckInGameResult(questcheck);
		}
		break;
		
	case MSG_NO_QUESTNODEBACK_ITEM_RESULT:
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : quest check node back item result [%d]", g_LocalSystemTime);
			MSG_QUEST_NODEBACK_ITEM_RESULT* questcheck = (MSG_QUEST_NODEBACK_ITEM_RESULT *)pPacket;
			_XPACKETSIZEERROR( &questcheck->header, MSG_QUEST_NODEBACK_ITEM_RESULT); 
			
			MSGQuestNodeBackItemResult(questcheck);
		}
		break;
		
	case MSG_NO_MONSTER_KILL: // ����Ʈ �α� ���� ���� üũ 
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : monsterkill result [%d]", g_LocalSystemTime);
			MSG_QUEST_MONSTER_KILL* mobkillcheck = (MSG_QUEST_MONSTER_KILL *)pPacket;
			_XPACKETSIZEERROR( &mobkillcheck->header, MSG_QUEST_MONSTER_KILL); 
						
			g_pLocalUser->m_CharacterLog.isKill = TRUE;
			g_pLocalUser->m_CharacterLog.killTime = g_ServerTimeCode;
			g_pLocalUser->m_CharacterLog.killTargetType = mobkillcheck->MobType - 4000;
		}
		break;
		
	// ���� �� ����.
	case MSG_NO_QUEST_REDISTRIBUTION:
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : quest charterpointreset result [%d]", g_LocalSystemTime);
			MSG_QUEST_REDISTRIBUTION* questcharterpointreset = (MSG_QUEST_REDISTRIBUTION *)pPacket;
			_XPACKETSIZEERROR( &questcharterpointreset->header, MSG_QUEST_REDISTRIBUTION);
			
			MSGQuestCharacterPointReset(questcharterpointreset);
		}
		break;
		
	// ���� ����
	case MSG_NO_LEARN_SKILL :
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : LEARN SKILL Info [%d]", g_LocalSystemTime);
			MSG_LEARN_SKILL* questskill = (MSG_LEARN_SKILL *)pPacket;
			_XPACKETSIZEERROR( &questskill->header, MSG_LEARN_SKILL);
			
			QuestLearnSkill(questskill);
		}
		break;
		
	// ���� ����
	case MSG_NO_FORGET_SKILL :
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : FORGET SKILL Info [%d]", g_LocalSystemTime);
			MSG_FORGET_SKILL* questskill = (MSG_FORGET_SKILL *)pPacket;
			_XPACKETSIZEERROR( &questskill->header, MSG_FORGET_SKILL);
			
			QuestForgetSkill(questskill);
		}
		break;
		
	// ���� ����
	case MSG_NO_UPDATE_STATUS :
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : UPDATE STATUS Info [%d]", g_LocalSystemTime);
			MSG_UPDATE_STATUS* queststatus = (MSG_UPDATE_STATUS *)pPacket;
			_XPACKETSIZEERROR( &queststatus->header, MSG_UPDATE_STATUS);
			
			QuestUpdateStatus(queststatus);
		}
		break;
		
	// ĳ���� ���� �̵�
	case MSG_NO_FORCED_MOVE :
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : FORCED MOVE Info [%d]", g_LocalSystemTime);
			MSG_FORCED_MOVE* questresult = (MSG_FORCED_MOVE *)pPacket;
			_XPACKETSIZEERROR( &questresult->header, MSG_FORCED_MOVE);

			g_pLocalUser->m_Position.x = questresult->fposX;
			g_pLocalUser->m_Position.z = questresult->fposZ;

			_XOBBData*  pUnderObb = NULL;
			g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, pUnderObb );
			g_pLocalUser->m_Velocity.y = 0.0f;

			g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;			
						
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

//			int param1 = (int)questresult->fposX;
//			int param2 = (int)questresult->fposZ;
//			InsertChatString( g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_MOVESOMEWHERE, &param1, &param2 ),
//							  _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL );
			

		}
		break;

	// ���� ����
	case MSG_NO_JOIN_GROUP :
		{
			_XDWPACKETLOG("RECEIVE MESSAGE : join group [%d]", g_LocalSystemTime);
			MSG_QUEST_JOIN_GROUP* questjoingroup = (MSG_QUEST_JOIN_GROUP *)pPacket;
			_XPACKETSIZEERROR( &questjoingroup->header, MSG_QUEST_JOIN_GROUP);
			
			QuestJoinGroup(questjoingroup);
		}
		break;
		
	// ������ ���
	case MSG_NO_USE_ITEM_RESULT :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : USE ITEM RESULT [%d]", g_LocalSystemTime);

			MSG_USE_ITEM_RESULT* useitemresult = (MSG_USE_ITEM_RESULT *)pPacket;
			_XPACKETSIZEERROR( &useitemresult->header, MSG_USE_ITEM_RESULT);

			MSGUseItem(useitemresult); // 2004.06.10->oneway48 insert
			
		}
		break;
	// ���� ���
	case MSG_NO_ELIXIR_USE_RESULT :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : USE ELIXIRITEM RESULT [%d]", g_LocalSystemTime);

			MSG_ELIXIR_USE_RESULT* useitemresult = (MSG_ELIXIR_USE_RESULT *)pPacket;
			_XPACKETSIZEERROR( &useitemresult->header, MSG_ELIXIR_USE_RESULT);

			MSGElixirUseItem(useitemresult);
		}
		break;
	case MSG_NO_ELIXIR_INTOX_TIME :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : USE INTOX RESULT [%d]", g_LocalSystemTime);

			MSG_ELIXIR_INTOX_TIME* intoxresult = (MSG_ELIXIR_INTOX_TIME *)pPacket;
			_XPACKETSIZEERROR( &intoxresult->header, MSG_ELIXIR_INTOX_TIME);

			MSGElixirIntox(intoxresult);
		}
		break;
	// ��Ƽ ó��	
	case MSG_NO_PARTY_JOIN_REQ :
		{
			MSG_PARTY_JOIN_REQ* joinparty = (MSG_PARTY_JOIN_REQ *)pPacket;
			_XPACKETSIZEERROR( &joinparty->header, MSG_PARTY_JOIN_REQ);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : JOIN PARTY REQ [%d]", g_LocalSystemTime);
			
			MSGPartyJoinReq(joinparty);
		}
		break;

	case MSG_NO_PARTY_RESPONSE :
		{
			MSG_PARTY_JOIN_RESPONSE* partyresponse = (MSG_PARTY_JOIN_RESPONSE *)pPacket;
			_XPACKETSIZEERROR( &partyresponse->header, MSG_PARTY_JOIN_RESPONSE);

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : PARTY RESPONCE [%d]", g_LocalSystemTime);
			
			MSGPartyResponse(partyresponse);
		}
		break;
		
	case MSG_NO_PARTY_INFO :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : PARTY INFO Success Party [%d]", g_LocalSystemTime);

			MSG_PARTY_INFO* partyinfo = (MSG_PARTY_INFO*)pPacket;
			_XPACKETSIZEERROR( &partyinfo->header, MSG_PARTY_INFO);
			
			PartyInfoProcess(partyinfo);
		}
		break;
		
	case MSG_NO_PARTY_MEMBER_UPDATE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE :  ARTY MEMBER UPDATE Member Update [%d]", g_LocalSystemTime);

			MSG_PARTY_MEMBER_UPDATE* memberupdate = (MSG_PARTY_MEMBER_UPDATE*)pPacket;
			_XPACKETSIZEERROR( &memberupdate->header, MSG_PARTY_MEMBER_UPDATE);
			
			PartyMemberUpdate(memberupdate);
		}
		break;
		
	case MSG_NO_PARTY_MEMBER_INFO :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : PARTY MEMBER INFO [%d]", g_LocalSystemTime);

			MSG_PARTY_MEMBER_INFO* memberinfo = (MSG_PARTY_MEMBER_INFO*)pPacket;
			_XPACKETSIZEERROR( &memberinfo->header, MSG_PARTY_MEMBER_INFO);
			
			PartyMemberInfo(memberinfo);
		}
		break;
		
	case MSG_NO_PARTY_ETC :
		{
			// ��Ÿ ó��
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : PARTY ETC [%d]", g_LocalSystemTime);

			MSG_PARTY_ETC* partyetc = (MSG_PARTY_ETC*)pPacket;
			_XPACKETSIZEERROR( &partyetc->header, MSG_PARTY_ETC);
			
			PartyETCProcess(partyetc);
		}
		break;
		
	case MSG_NO_NPC_TRADE_RESULT :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : NPC TRADE RESULT [%d]", g_LocalSystemTime);

			MSG_NPC_TRADE_RESULT* traderesult = (MSG_NPC_TRADE_RESULT*)pPacket;
			_XPACKETSIZEERROR( &traderesult->header, MSG_NPC_TRADE_RESULT);
			
			NPCTradeResult(traderesult);
		}
		break;
		// P2P Trade
	case MSG_NO_PVP_TRADE_REQ :
		{
			MSG_PVP_TRADE_REQ* pvptradereq = (MSG_PVP_TRADE_REQ*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : PvP Trade Request [%d]", g_LocalSystemTime);
			
			MSGPvPTradeReq(pvptradereq);
		}
		break;
		
	case MSG_NO_PVP_TRADE_START :
		{			
			MSG_PVP_TRADE_START* pvptradestart = (MSG_PVP_TRADE_START*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : PvP Trade Start [%d] Slot num:%d", g_LocalSystemTime, pvptradestart->TradeSlotNO);
			
			MSGPvPTradeStart(pvptradestart);
		}
		break;
		
	case MSG_NO_PVP_TRADE_DELETE :
		{
			MSG_PVP_TRADE_DELETE* pvptradedelete = (MSG_PVP_TRADE_DELETE*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : PvP Trade Delete [%d]", g_LocalSystemTime);
			
			MSGPvPTradeDelete(pvptradedelete);
		}
		break;
		
	case MSG_NO_PVP_TRADE_UPDATE_INFO :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : PvP Trade Info [%d]", g_LocalSystemTime);

			MSG_PVP_TRADE_UPDATE_INFO* pvptradeupdate = (MSG_PVP_TRADE_UPDATE_INFO*)pPacket;
			
			MSGPvPTradeUpdateInfo(pvptradeupdate);
		}
		break;
		
	case MSG_NO_PVP_TRADE_END :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : PvP Trade End [%d]", g_LocalSystemTime);

			MSG_PVP_TRADE_END* pvptradeend = (MSG_PVP_TRADE_END*)pPacket;
			
			MSGPvPTradeEnd(pvptradeend);
		}
		break;
		
	case MSG_NO_PVP_TRADE_MONEY :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : PvP Trade Money [%d]", g_LocalSystemTime);

			MSG_PVP_TRADE_MONEY* pvptrademoney = (MSG_PVP_TRADE_MONEY*)pPacket;
			
			MSGPvPTradeMoney(pvptrademoney);
		}
		break;
		
	case MSG_NO_PVP_TRADE_CANCEL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : PvP Trade Cancel [%d]", g_LocalSystemTime);

			MSG_PVP_TRADE_CANCEL* pvptradecancel = (MSG_PVP_TRADE_CANCEL*)pPacket;
			
			MSGPvPTradeCancel(pvptradecancel);
		}
		break;

		// ��ų ����, ����
	case MSG_NO_SKILL_CTRL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Skill Ctrl [%d]", g_LocalSystemTime);

			MSG_SKILL_CTRL* skillctrl = (MSG_SKILL_CTRL*)pPacket;
			
			MSGSkillCtrl(skillctrl);
		}
		
		break;
	case MSG_NO_SKILL_ACTION :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Skill Action reson", g_LocalSystemTime);			

			MSG_CANCEL_SKILL_CANNOT* skillaction = (MSG_CANCEL_SKILL_CANNOT*)pPacket;

			MSGSkillActionReson(skillaction);
			
		}
		break;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//   ����Ʈ ���� ��Ŷ ��� 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
	case MSG_NO_COLLECT_RES :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Collection response");

#ifdef _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI

			MSG_COLLECT_PACKET* collect = (MSG_COLLECT_PACKET*)pPacket;
			if(collect->ucMode == en_collect_mode_result)
			{
				MSG_COLLECT_RESULT* collection = (MSG_COLLECT_RESULT*)pPacket;
				MSGCollectionResult(collection);
			}
			
#else
			MSG_COLLECT_RES* collectionresponse = (MSG_COLLECT_RES*)pPacket;
			
			MSGCollectionResponse(collectionresponse);
#endif
		}
		
		break;
	//===============================================================================================================		
	// â�� ����
	case MSG_NO_STR_BUY_SLOT_RESULT:
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE :  Add Warehouse Slot Result [%d]", g_LocalSystemTime);

			MSG_STR_BUY_SLOT_RESULT* addslotresult = (MSG_STR_BUY_SLOT_RESULT*)pPacket;
			
			MSGAddStrSlotResult(addslotresult);
		}
		break;
		
		// Character control - GMS
	case MSG_NO_CTRL_CHARAC_SETTING :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Character Control Message [%d]", g_LocalSystemTime);

			MSG_CTRL_CHARAC_SETTING* characsetting = (MSG_CTRL_CHARAC_SETTING*)pPacket;

			MSGCtrlCharacSetting(characsetting);
		}
		break;

		// stance motion change
	case MSG_NO_STANCE_MOTION_CHANGE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Stance motion change [%d]", g_LocalSystemTime);

			MSG_STANCE_MOTION_CHANGE* stancemotionchange = (MSG_STANCE_MOTION_CHANGE*)pPacket;

			MSGStanceMotionChange(stancemotionchange);
		}
		break;
		
	//===============================================================================================================		
	// ������ ���� ��Ŷ
	case MSG_NO_EX_BATTLE_INVITE_S_C :// ������ ������ �ش� ĳ���鿡�� �ʴ�
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : boss invite");

			MSG_EX_BATTLE_INVITE* bossinvite = (MSG_EX_BATTLE_INVITE*)pPacket;

			MSGBossInviate(bossinvite);
		}
		break;
				
	case MSG_NO_EX_BATTLE_RESULT_S_C :// REQ �޽����鿡 ���� ���� �޽���
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : boss result");

			MSG_EX_BATTLE_RESULT* bossresult = (MSG_EX_BATTLE_RESULT*)pPacket;
			
			MSGBossResult(bossresult);
		}
		break;
		
	case MSG_NO_EX_BATTLE_PLAY_VIDEO :// �÷��� ���� �������..
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : play video ");

			MSG_EX_BATTLE_PLAY_VIDEO* bossplaymovie = (MSG_EX_BATTLE_PLAY_VIDEO*)pPacket;
			
			MSGBossPlayMovie(bossplaymovie);
		}
		break;
		
	case MSG_NO_EX_BATTLE_TIME_S_C :// ������ Ŭ���̾�Ʈ�� �ð� ����ȭ..
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : boss timer ");

			MSG_EX_BATTLE_TIME* bosstimer = (MSG_EX_BATTLE_TIME*)pPacket;
			
			MSGBossTimer(bosstimer);
		}
		break;
		
	case MSG_NO_EX_BATTLE_TYPE_INFO_RES_S_C :// �� Ÿ�Կ� ���� ��������
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : boss room type ");

			MSG_EX_BATTLE_TYPE_INFO* bossroomtpye = (MSG_EX_BATTLE_TYPE_INFO*)pPacket;
			
			MSGBossRoomType(bossroomtpye);
		}
		break;
		
	case MSG_NO_EX_BATTLE_CUR_STAGE_S_C :// �������� �������� ���� �������� ��ȣ.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : boss stage number ");
			
			MSG_EX_BATTLE_CUR_STAGE* bossstagenumber= (MSG_EX_BATTLE_CUR_STAGE*)pPacket;
			
			MSGBossStageNumber(bossstagenumber);
		}
		break;
		
	//===============================================================================================================		
	// ��ȣ ���� ��Ŷ
	case MSG_NO_NIC_LIST_S_C :// ��ȣ ����Ʈ�� ��ȯ
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : nickname list receive");

			MSG_NIC_LIST* nicknamelistreceive = (MSG_NIC_LIST*)pPacket;
			
			MSGNicknameListReceive(nicknamelistreceive);
		}
		break;
		
	case MSG_NO_NIC_SET_S_C :// ���Կ� ��ȣ�� �����Ѵ�
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : nickname setting");

			MSG_NIC_SET* nicknamesetting = (MSG_NIC_SET*)pPacket;
			
			MSGNicknameSetting(nicknamesetting);
		}
		break;
		
	case MSG_NO_NIC_SELECT_S_C :// ������ ��ȣ ������ ��ȯ�Ѵ�..
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : nickname select");

			MSG_NIC_SELECT* nicknameselect = (MSG_NIC_SELECT*)pPacket;
			
			MSGNicknameSelect(nicknameselect);
		}
		break;
		
	case MSG_NO_NIC_RES :// ��Ÿ REQ ��Ŷ�� ���� ���� �Ǵ� ���� ��ȯ��..
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : nickname result");

			MSG_NIC_RES* nicknameresult = (MSG_NIC_RES*)pPacket;
			
			MSGNicknameResult(nicknameresult);
		}
		break;	
	case MSG_NO_NIC_USEFULL_TIME_S_C :
		{			
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : nickname result");

			MSG_NIC_USEFULL_TIME* timeresult = (MSG_NIC_USEFULL_TIME*)pPacket;
			
			MSGNicknameRemainTime(timeresult);		
		}
		break;
	case MSG_NO_RANK_HIDING :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : rank hideing");

			MSG_RANK_HIDING* hidingresult = (MSG_RANK_HIDING*)pPacket;
			
			MSGRankHiding(hidingresult);
		}
		break;
#ifdef _XTS_NEW_HONOR2
	case MSG_NO_HONOR_PLAYER :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : honor result");

			MSG_HONOR_PLAYER* honorresult = (MSG_HONOR_PLAYER*)pPacket;
			
			MSGPlayerHonorResult(honorresult);
		}
		break;
#endif
		// �� ����
	case MSG_NO_MATCH_LIST :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Match List");
			
			MSG_MATCH_LIST* matchlist = (MSG_MATCH_LIST*)pPacket;
			
			switch(matchlist->ucMode)
			{
			case en_match_list_mode_register :
				{
					MSGMatchListModeRegister(matchlist);
				}
				break;
			case en_match_list_mode_delete :
				{
					MSGMatchListModeDelete(matchlist);
				}
				break;
			case en_match_list_mode_not_map :
				{
					MSGMatchListModeNotMap(matchlist);
				}
				break;
			case en_match_list_mode_list_update :
				{
					MSG_MATCH_LIST_INFO* matchlistinfo = (MSG_MATCH_LIST_INFO*)pPacket;
					MSGMatchListModeListUpdate(matchlistinfo);
				}
				break;
			case en_match_list_mode_list_continue :
				{
					MSG_MATCH_LIST_INFO* matchlistinfo = (MSG_MATCH_LIST_INFO*)pPacket;
					MSGMatchListModeListContinue(matchlistinfo);
				}
				break;
			}
		}
		break;
	case MSG_NO_MATCH_READY :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Match Ready");

			MSG_MATCH_READY* matchready = (MSG_MATCH_READY*)pPacket;

			switch(matchready->ucMode)
			{
			case en_match_ready_challenge_from_other :
				{
					MSG_MATCH_READY_CHALLENGE_FROM* matchreadyfrom = (MSG_MATCH_READY_CHALLENGE_FROM*)pPacket;
					MSGMatchReadyChallengeFromOther(matchreadyfrom);
				}
				break;
			case en_match_ready_challenge_cancel :
				{
					MSG_MATCH_READY_CHALLENGE_CANCEL* matchreadycancel = (MSG_MATCH_READY_CHALLENGE_CANCEL*)pPacket;
					MSGMatchReadyChallengeCancel(matchreadycancel);
				}
				break;
			case en_match_ready_reply :
				{
					MSG_MATCH_READY_REPLY* matchreadyreply = (MSG_MATCH_READY_REPLY*)pPacket;
					MSGMatchReadyReply(matchreadyreply);
				}
				break;
			case en_match_ready_start_make_table :
				{
					MSG_MATCH_READY_START_MAKE_TABLE* matchreadymaketable = (MSG_MATCH_READY_START_MAKE_TABLE*)pPacket;
					MSGMatchReadyStartMakeTable(matchreadymaketable);
				}
				break;
			case en_match_ready_update_waittime :
			case en_match_ready_update_recordtype :
			case en_match_ready_update_matchtype :
			case en_match_ready_update_matchtime :
			case en_match_ready_update_disable_hyperrun :
			case en_match_ready_update_keep_buff :
			case en_match_ready_update_balance :
			case en_match_ready_update_after_draw :
				{
					MSG_MATCH_READY_UPDATE* matchreadyupdate = (MSG_MATCH_READY_UPDATE*)pPacket;
					MSGMatchReadyUpdate(matchreadyupdate);
				}
				break;
			case en_match_ready_select_member :
				{
					MSG_MATCH_READY_SELECT_MEMBER* matchreadyselectmember = (MSG_MATCH_READY_SELECT_MEMBER*)pPacket;
					MSGMatchReadySelectMember(matchreadyselectmember);
				}
				break;
			case en_match_ready_deselect_member :
				{
					MSG_MATCH_READY_DESELECT_MEMBER* matchreadydeselectmember = (MSG_MATCH_READY_DESELECT_MEMBER*)pPacket;
					MSGMatchReadyDeselectMember(matchreadydeselectmember);
				}
				break;
			case en_match_ready_confirm :
				{
					MSGMatchReadyConfirm(matchready);
				}
				break;
			case en_match_ready_cancel_all :
				{
					MSGMatchReadyCancelAll(matchready);
				}
				break;
			case en_match_ready_errno_cannotupdate :
			case en_match_ready_errno_no_item :
				{
					MSGMatchReadyError(matchready);
				}
				break;
			}
		}
		break;
	case MSG_NO_MATCH_CTRL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Match Ctrl");

			MSG_MATCH_CTRL* matchctrl = (MSG_MATCH_CTRL*)pPacket;
			
			switch(matchctrl->ucMode)
			{
			case en_match_ctrl_start :
				{
					MSG_MATCH_CTRL_START* matchctrlstart = (MSG_MATCH_CTRL_START*)pPacket;
					MSGMatchCtrlStart(matchctrlstart);
				}
				break;
			case en_match_ctrl_wait :
				{
					MSG_MATCH_CTRL_WAIT* matchctrlwait = (MSG_MATCH_CTRL_WAIT*)pPacket;
					MSGMatchCtrlWait(matchctrlwait);
				}
				break;
			case en_match_ctrl_start_count :
				{
					MSGMatchCtrlStartCount(matchctrl);
				}
				break;
			case en_match_ctrl_start_fight :
				{
					MSGMatchCtrlStartFight(matchctrl);
				}
				break;
			case en_match_ctrl_before_5_sec :
				{
					MSGMatchCtrlBeforeCountDown(matchctrl);
				}
				break;
			case en_match_ctrl_end :
				{
					MSG_MATCH_CTRL_END* matchctrlend = (MSG_MATCH_CTRL_END*)pPacket;
					MSGMatchCtrlEnd(matchctrlend);
				}
				break;
			case en_match_ctrl_update_record_each :
				{
					MSGMatchCtrlUpdateRecord(matchctrl);
				}
				break;
			case en_match_ctrl_cannot_exit :
				{
					MSGMatchCtrlCannotExit(matchctrl);
				}
				break;
			case en_match_ctrl_exit :
				{
					MSG_MATCH_CTRL_EXIT* matchctrlexit = (MSG_MATCH_CTRL_EXIT*)pPacket;
					MSGMatchCtrlExit(matchctrlexit);
				}
			case en_match_ctrl_system_error :
				{
					MSGMatchCtrlSystemError(matchctrl);
				}
				break;
			case en_freematch_ctrl_enter :
				{
					MSG_MATCH_CTRL_FREE_ENTER* matchctrl = (MSG_MATCH_CTRL_FREE_ENTER*)pPacket;
					MSGMatchCtrlFreeEnter(matchctrl);
				}
				break;
			case en_freematch_ctrl_into_field :
				{
					MSG_MATCH_CTRL_FREE_INTO_FIELD* matchctrl = (MSG_MATCH_CTRL_FREE_INTO_FIELD*)pPacket;
//					MSGMatchCtrlFreeIntoField(matchctrl);
				}
				break;
			case en_freematch_ctrl_go_safezone :
				{
					MSG_MATCH_CTRL_FREE_GO_SAFEZONE* matchctrl = (MSG_MATCH_CTRL_FREE_GO_SAFEZONE*)pPacket;
//					MSGMatchCtrlFreeGoSafezone(matchctrl);
				}
				break;
			}
		}
		break;
	case MSG_NO_MATCH_NOTICE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Match Notice");

			MSG_MATCH_NOTICE* matchnotice = (MSG_MATCH_NOTICE*)pPacket;

			switch(matchnotice->ucMode)
			{
			case en_match_ntc_inform :
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Match Notice Inform");

					MSG_MATCH_NOTICE_INFORM* matchnoticeinform = (MSG_MATCH_NOTICE_INFORM*)pPacket;
					MSGMatchNoticeInform(matchnoticeinform);
				}
				break;
			case en_match_ntc_sb_info :
				{
					MSG_MATCH_NOTICE_SB_INFO* matchnoticesbinfo = (MSG_MATCH_NOTICE_SB_INFO*)pPacket;
					MSGMatchNoticeSBInfo(matchnoticesbinfo);
				}
				break;
			case en_match_ntc_lb_info :
				{
					MSG_MATCH_NOTICE_LB_INFO* matchnoticelbinfo = (MSG_MATCH_NOTICE_LB_INFO*)pPacket;
					MSGMatchNoticeLBInfo(matchnoticelbinfo);
				}
				break;
			case en_match_ntc_sb_update_vital :
				{
					MSG_MATCH_NOTICE_SB_UPDATE_VITAL* matchnoticesbvital = (MSG_MATCH_NOTICE_SB_UPDATE_VITAL*)pPacket;
					MSGMatchNoticeSBUpdateVital(matchnoticesbvital);
				}
				break;
			case en_match_ntc_lb_update_vital :
				{
					MSG_MATCH_NOTICE_LB_UPDATE_VITAL* matchnoticelbvital = (MSG_MATCH_NOTICE_LB_UPDATE_VITAL*)pPacket;
					MSGMatchNoticeLBUpdateVital(matchnoticelbvital);
				}
				break;
			case en_match_ntc_inform_end :
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Match Notice End");

					MSG_MATCH_NOTICE_INFORM_END* matchnoticeinformend = (MSG_MATCH_NOTICE_INFORM_END*)pPacket;
					MSGMatchNoticeInformEnd(matchnoticeinformend);
				}
				break;
			}
		}
		break;
		
	case MSG_NO_FREE_MATCH :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Free Match");

			MSG_FREE_MATCH* fm = (MSG_FREE_MATCH*)pPacket;

			switch(fm->ucFMMode)
			{
			case en_gsfm_ctrl :
				{
					MSG_FM_CTRL* fmctrl = (MSG_FM_CTRL*)pPacket;
					MSGFMCtrl(fmctrl);
				}
				break;
			case en_gsfm_notice :
				{	
					MSG_FM_NOTICE* fmnotice = (MSG_FM_NOTICE*)pPacket;
					MSGFMNotice(fmnotice);
				}
				break;
			case en_gsfm_notice_winner :
				{
					MSG_FM_NOTICE_WINNER* fmnoticewinner = (MSG_FM_NOTICE_WINNER*)pPacket;
					MSGFMNoticeWinner(fmnoticewinner);
				}
				break;
			case en_gsfm_notice_nowinner :
				{
					MSG_FM_NOTICE_NOWINNER* fmnowinner = (MSG_FM_NOTICE_NOWINNER*)pPacket;
					MSGFMNoticeNowinner(fmnowinner);
				}
				break;
			case en_gsfm_to_winner :
				{
					MSG_FM_TO_WINNER* towinner = (MSG_FM_TO_WINNER*)pPacket;
					MSGFMToWinner(towinner);
				}
				break;
			case en_gsfm_apply_success :
				{
					// ���� ����
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2947), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);	//���忡 �����Ͽ����ϴ�.
				}
				break;
			case en_gsfm_apply_fail :
				{
					// ���� ����
					MSG_FM_APPLY_FAIL* fmfail = (MSG_FM_APPLY_FAIL*)pPacket;
					MSGFMApplyFail(fmfail);
				}
				break;
			case en_gsfm_enter_match :
				{
					// ����
					MSG_FM_ENTER_MATCH* fmenter = (MSG_FM_ENTER_MATCH*)pPacket;
					MSGFMEnterMatch(fmenter);
				}
				break;
			case en_gsfm_reward_nick :
				{
					MSG_FM_REWARD_NICK* rewardnick = (MSG_FM_REWARD_NICK*)pPacket;
					MSGFMRewardNick(rewardnick);
				}
				break;
			case en_gsfm_reward_item :
				{	
					MSG_FM_REWARD_ITEM* rewarditem = (MSG_FM_REWARD_ITEM*)pPacket;
					MSGFMRewardItem(rewarditem);
				}
				break;
			case en_gsfm_set_nick :
				{
					MSG_FM_SET_NICK* setnick = (MSG_FM_SET_NICK*)pPacket;
					MSGFMSetNick(setnick);
				}
				break;
			case en_gsfm_current_state :
				{
					MSG_FM_CURRENT_STATE* state = (MSG_FM_CURRENT_STATE*)pPacket;
					MSGFMCurrentState(state);
				}	
				break;
			case en_gsfm_private_score :
				{
					MSG_FM_PRIVATE_SCORE* score = (MSG_FM_PRIVATE_SCORE*)pPacket;
					MSGFMPrivateScore(score);
				}
				break;
			}
		}
		break;

/*	case MSG_NO_MATCH_READY :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Match Ready");

			MSG_MATCH_READY* matchready = (MSG_MATCH_READY*)pPacket;
			
			switch(matchready->cCmd)
			{
			case en_match_ready_apply :
				{
					if(matchready->cMatchType == en_match_type_pvp || matchready->cMatchType == en_match_type_p_party)
					{
						MSG_MATCH_APPLY_FROM_CHARAC* matchapplycharac = (MSG_MATCH_APPLY_FROM_CHARAC*)pPacket;
						MSGMatchApplyFromCharac(matchapplycharac);
					}
					else
					{
						MSG_MATCH_APPLY_FROM_PARTY* matchapplyparty = (MSG_MATCH_APPLY_FROM_PARTY*)pPacket;
						MSGMatchApplyFromParty(matchapplyparty);
					}
				}
				break;
			case en_match_ready_cancel :
			case en_match_ready_answer :
				{
					MSG_MATCH_ANSWER* matchanswer = (MSG_MATCH_ANSWER*)pPacket;
					MSGMatchAnswer(matchanswer);
				}
				break;
			}
		}
		break;

	case MSG_NO_MATCH_CTRL :
		{	
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Match Ctrl");

			MSG_MATCH_CTRL* matchctrl = (MSG_MATCH_CTRL*)pPacket;

			switch(matchctrl->cMode)
			{
			case en_match_ctrl_start :
				{
					MSG_MATCH_START* matchstart = (MSG_MATCH_START*) pPacket;
					MSGMatchStart(matchstart);
				}
				break;
			case en_match_ctrl_result :
				{
					MSG_MATCH_RESULT* matchresult = (MSG_MATCH_RESULT*)pPacket;
					MSGMatchResult(matchresult);
				}
				break;
			case en_match_ctrl_answer :
				{
					MSG_MATCH_CTRL_ANSWER* matchctrlanswer = (MSG_MATCH_CTRL_ANSWER*)pPacket;
					MSGMatchCtrlAnswer(matchctrlanswer);
				}
				break;
			case en_match_ctrl_end_info :
				{
					MSG_MATCH_END_INFO* matchendinfo = (MSG_MATCH_END_INFO*)pPacket;
					MSGMatchEndInfo(matchendinfo);
				}
				break;
			default :
				break;
			}
		}
		break;

	case MSG_NO_MATCH_NOTICE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Match Notice");

			MSG_MATCH_NOTICE* matchnotice = (MSG_MATCH_NOTICE*)pPacket;
			MSGMatchNotice(matchnotice);
		}
		break;

		// ==============================================================================================
		// ��� ���� �����ִ� ������Ʈ ����
	case MSG_NO_OBJECT_SIMPLE_INFO :
		{
			MSG_OBJECT_SIMPLE_INFO* objectinfo = (MSG_OBJECT_SIMPLE_INFO*)pPacket;
			if(objectinfo->cObjectType == en_object_type_sega)
			{
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : Sega Flag simple Info");
			}
			else if(objectinfo->cObjectType == en_object_type_match_flag)
			{
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : Match Flag simple Info");

				MSG_OBJ_MATCH_FLAG_INFO* matchflaginfo = (MSG_OBJ_MATCH_FLAG_INFO*)pPacket;
				MSGObjMatchFlagInfo(matchflaginfo);
			}
		}
		break;*/
	case MSG_NO_OBJECT_INFO_REQUEST :
		{
			_XWindow_TargetInfo* targetinfo_window = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);

			MSG_CHARAC_MORE_INFO_REQ* characinfo = (MSG_CHARAC_MORE_INFO_REQ*)pPacket;
			
			switch(characinfo->cFlag) 
			{
			case MSG_CHARAC_MORE_INFO_REQ::_request :
				{					
					if( targetinfo_window )
					{
						if( g_bCommunityOptionRejectTargetInfo )	// �������� �ź� �ɼ��� ��������
						{
							strcpy( targetinfo_window->m_SendName, characinfo->cTo_or_From_Name );
							g_NetworkKernel.SendPacket(MSG_NO_OBJECT_INFO_REQUEST, characinfo->cObjectType, MSG_CHARAC_MORE_INFO_REQ::_response_no);
						}
						else
						{
							targetinfo_window->m_TargetInfoModefromRequest = (TargetInfoMode)characinfo->cObjectType ;
							BOOL npcdialogmode  = ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_bNPCDialogMode;  
							BOOL npcwideviewmode  = ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_bNPCDialogWideView; 
							
							if( !npcdialogmode && !npcwideviewmode )
							{
								if( g_pLocalUser->m_bTargetInfoLock )	// ���� �ٸ� ������� ��û���϶�
								{
									if( strlen(targetinfo_window->m_SendName) )
									{
										strcpy( targetinfo_window->m_SendTempName, characinfo->cTo_or_From_Name );
									}
									else
									{
										strcpy( targetinfo_window->m_SendName, characinfo->cTo_or_From_Name );
									}

									g_NetworkKernel.SendPacket(MSG_NO_OBJECT_INFO_REQUEST, characinfo->cObjectType, MSG_CHARAC_MORE_INFO_REQ::_response_no);									
								}
								else
								{
									if( strlen(targetinfo_window->m_SendName) && g_pLocalUser->m_bTargetInfoLock )	// �ι��� ��û �޾��� ��
									{
										strcpy( targetinfo_window->m_SendTempName, characinfo->cTo_or_From_Name );
										g_NetworkKernel.SendPacket(MSG_NO_OBJECT_INFO_REQUEST, characinfo->cObjectType, MSG_CHARAC_MORE_INFO_REQ::_response_no);
									}
									else	// ù��° ��û �޾��� �� 
									{
										strcpy( targetinfo_window->m_SendName, characinfo->cTo_or_From_Name );
										_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_TARGETINFONMESSAGEWINDOW);
										TCHAR	messagestring[256] ;
										memset(messagestring, 0, sizeof(TCHAR)*256) ;
										TCHAR requesttype[64] ;
										memset(requesttype, 0, sizeof(TCHAR)*64) ;
										switch(characinfo->cObjectType) 
										{
										case en_object_type_items_info :
											strcpy( requesttype, _XGETINTERFACETEXT(ID_STRING_NETWORK_1589)) ;
											break;
										case en_object_type_ability_info :
											strcpy( requesttype, _XGETINTERFACETEXT(ID_STRING_NETWORK_1590)) ;
											break;
										case en_object_type_all_info :
											strcpy( requesttype, _XGETINTERFACETEXT(ID_STRING_NETWORK_1591)) ;
											break;
										}

										LPTSTR param1 = characinfo->cTo_or_From_Name;
										LPTSTR param2 = requesttype;

										strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORK_1592, param1, param2 ) );
										
										pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_MONSTERSTATUS_INFOPOPUPYES, _XDEF_MONSTERSTATUS_INFOPOPUPNO) ;
										pMessageBox->StartTimer(10000, TRUE) ;
										
										g_pLocalUser->m_bTargetInfoLock = TRUE;
									}
								}
							}
							else	// NPC ����� ��� 
							{
								if( strlen(targetinfo_window->m_SendName) )
								{
									strcpy( targetinfo_window->m_SendTempName, characinfo->cTo_or_From_Name );
								}
								else
								{
									strcpy( targetinfo_window->m_SendName, characinfo->cTo_or_From_Name );
								}

								g_NetworkKernel.SendPacket(MSG_NO_OBJECT_INFO_REQUEST, characinfo->cObjectType, MSG_CHARAC_MORE_INFO_REQ::_response_no);															
							}							
						}							
					}										
				}
				break;
			case MSG_CHARAC_MORE_INFO_REQ::_response_yes :
				{
				}
				break;
			case MSG_CHARAC_MORE_INFO_REQ::_response_no :
				{
					if( targetinfo_window )
					{
						memset( targetinfo_window->m_SendName, 0, sizeof(TCHAR)*13);						
					}
					g_pLocalUser->m_bTargetInfoLock = FALSE;
					g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_TARGETINFONMESSAGEWINDOW);

					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW) ;
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1593), TRUE) ;
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			case MSG_CHARAC_MORE_INFO_REQ::_response_cancel :
				{
					if( targetinfo_window )
					{
						memset( targetinfo_window->m_SendName, 0, sizeof(TCHAR)*13);						
					}

					g_pLocalUser->m_bTargetInfoLock = FALSE;
					g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_TARGETINFONMESSAGEWINDOW);

					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW) ;
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1594), TRUE) ;
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			case MSG_CHARAC_MORE_INFO_REQ::_response_error :
				{
					if( targetinfo_window )
					{
						memset( targetinfo_window->m_SendName, 0, sizeof(TCHAR)*13);						
					}

					g_pLocalUser->m_bTargetInfoLock = FALSE;
					g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_TARGETINFONMESSAGEWINDOW);
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW) ;
					//pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1595), TRUE) ;
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3841), TRUE); //Ÿ ������ �ִ� ����������� �� �� �����ϴ�.
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			}
		}
		break;
	case MSG_NO_OBJECT_DETAIL_INFO :
		{
			MSG_OBJECT_DETAIL_INFO* objdetailinfo = (MSG_OBJECT_DETAIL_INFO*)pPacket;
			if(objdetailinfo->cObjectType == en_object_type_sega)
			{/*
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : Sega Flag Detail info");*/
			}
			else if(objdetailinfo->cObjectType == en_object_type_match_flag)
			{
				/*VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : Match Flag Detail info");

				MSG_MATCH_FLAG_DETAIL_INFO* matchflagdetailinfo = (MSG_MATCH_FLAG_DETAIL_INFO*)pPacket;
				MSGObjectDetailInfo(matchflagdetailinfo);*/
			}
			else if(objdetailinfo->cObjectType == em_object_type_simple_info	||
					objdetailinfo->cObjectType == en_object_type_items_info		||
					objdetailinfo->cObjectType == en_object_type_ability_info	||
					objdetailinfo->cObjectType == en_object_type_all_info )
			{
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : TargetInfo") ;

				MSG_CHARAC_MORE_INFO* targetinfo = (MSG_CHARAC_MORE_INFO*)pPacket;
				MSGTargetInfo( targetinfo );
			}
		}
		break;
		// ĳ���� ���� ȿ��
	case MSG_NO_EFFECT_CTRL :
		{
			MSG_EFFECT_CTRL_RESULT* effectctrl = (MSG_EFFECT_CTRL_RESULT*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Effect Ctrl [Mode:%d]", effectctrl->cMode);

			if(effectctrl->cMode >= en_eff_ctrl_rs_add_skill && effectctrl->cMode <= en_eff_ctrl_rs_skill_reserve4)
			{
				MSG_EFFECT_CTRL_RESULT_MY_SPELL* effectmyspell = (MSG_EFFECT_CTRL_RESULT_MY_SPELL*)pPacket;
				MSGEffectCtrlResultMySpell(effectmyspell);
			}
			else if(effectctrl->cMode >= en_eff_ctrl_rs_add_effect && effectctrl->cMode <= en_eff_ctrl_rs_condition_reserve4)
			{
				MSG_EFFECT_CTRL_RESULT_MY_EFFECT* effectmyeffect = (MSG_EFFECT_CTRL_RESULT_MY_EFFECT*)pPacket;
				MSGEffectCtrlResultMyEffect(effectmyeffect);
			}
			else if(effectctrl->cMode >= en_eff_ctrl_rs_other_add_skill && effectctrl->cMode <= en_eff_ctrl_rs_other_effect_res)
			{
				MSG_EFFECT_CTRL_RESULT_OTHER* effectother = (MSG_EFFECT_CTRL_RESULT_OTHER*)pPacket;
				MSGEffectCtrlResultOther(effectother);
			}
			else if(effectctrl->cMode >= en_eff_ctrl_rs_add_extra && effectctrl->cMode <= en_eff_ctrl_rs_del_extra)
			{
				MSG_EFFECT_CTRL_RESULT_EXTRA* effectextra = (MSG_EFFECT_CTRL_RESULT_EXTRA*)pPacket;
				MSGEffectCtrlResultExtra(effectextra);
			}
			else if(effectctrl->cMode >= en_eff_ctrl_rs_cancel_current && effectctrl->cMode <= en_eff_ctrl_rs_cancel_hyperrun)
			{
				MSGEffectCtrlResult(effectctrl);
			}
			else if(effectctrl->cMode == en_eff_ctrl_rs_add_special ) // ����� ���� �߰�
			{
				MSG_EFFECT_CTRL_RESULT_ADD_SPECIAL* pResultAddSpecial = (MSG_EFFECT_CTRL_RESULT_ADD_SPECIAL*)pPacket;
				MSGEffectCtrlResultAddSpecial(pResultAddSpecial);
			}
			else if(effectctrl->cMode == en_eff_ctrl_rs_update_special ) //���� ������Ʈ 
			{
				MSG_EFFECT_CTRL_RESULT_UPDATE_SPECIAL* pResultUpdateSpecial = (MSG_EFFECT_CTRL_RESULT_UPDATE_SPECIAL*)pPacket;
				MSGEffectCtrlResultUpdateSpecial(pResultUpdateSpecial);
			}
			else if(effectctrl->cMode ==  en_eff_ctrl_rs_del_special ) // ��������
			{
				MSG_EFFECT_CTRL_RESULT_DEL_SPECIAL* pResultDelSpecial = (MSG_EFFECT_CTRL_RESULT_DEL_SPECIAL*)pPacket;
				MSGEffectCtrlResultDelSpecial(pResultDelSpecial);
			}
		}
		break;
		
	case MSG_NO_EFFECT_INFO :
		{
			MSG_EFFECT_INFO* effectinfo = (MSG_EFFECT_INFO*)pPacket;
			if(effectinfo->cMode == eff_info_mode_my_bh_list)
			{
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : Effect My BH Info");

				MSG_EFFECT_MY_BH_INFO* effectmybhinfo = (MSG_EFFECT_MY_BH_INFO*)pPacket;
				MSGEffectMyBHInfo(effectmybhinfo);
			}
			else if(effectinfo->cMode == eff_info_mode_my_entry_list)
			{
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : Effect My Entry Info");

				MSG_EFFECT_MY_ENTRY_INFO* effectmyentryinfo = (MSG_EFFECT_MY_ENTRY_INFO*)pPacket;
				MSGEffectMyEntryInfo(effectmyentryinfo);
			}
			else if(effectinfo->cMode == eff_info_mode_other_bh_list)
			{
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : Effect Other bh Info");

				MSG_EFFECT_OTHER_INFO* effectotherinfo = (MSG_EFFECT_OTHER_INFO*)pPacket;
				MSGEffectOtherInfo(effectotherinfo);
			}
			else if(effectinfo->cMode == eff_info_mode_my_extra_list)
			{
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : Effect Extra Info");

				MSG_EFFECT_MY_EXTRA_INFO* effectextra = (MSG_EFFECT_MY_EXTRA_INFO*)pPacket;
				MSGEffectMyExtraInfo(effectextra);
			}
			else if(effectinfo->cMode == eff_info_mode_other_entry_list)
			{
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : Effect other entry Info");

				MSG_EFFECT_OTHER_INFO* effectotherinfo = (MSG_EFFECT_OTHER_INFO*)pPacket;
				MSGEffectOtherEntryInfo(effectotherinfo);
			}
			else if( effectinfo->cMode == eff_info_mode_special_buff_list ) // ����� ���� �߰�
			{
				VIEWPACKETLOG
					_XDWPACKETLOG("RECEIVE MESSAGE : Effect special buff Info");

				MSG_EFFECT_SPECIAL_BUFF_INFO* pSpecialBuffInfo = (MSG_EFFECT_SPECIAL_BUFF_INFO*)pPacket;
				MSGEffectMySpecialInfo(pSpecialBuffInfo);
			}
		}
		break;
		
		// mastery skill
	case MSG_NO_SKILL_INFO_MASTERY :
		{
			MSG_SKILL_INFO_MASTERY* skillinfomastery = (MSG_SKILL_INFO_MASTERY*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Skill info Mastery");

			MSGSkillInfoMastery(skillinfomastery);
		}
		break;

	case MSG_NO_MASTERY_POINT_RESULT :
		{
			MSG_MASTERY_POINT_RESULT* pointresult = (MSG_MASTERY_POINT_RESULT*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Mastery point use result");

			MSGMasteryPointResult(pointresult);
		}
		break;

		// skill ��� �㰡
	case MSG_NO_SKILL_SELECT :
		{
			MSG_SKILL_SELECT* skillselect = (MSG_SKILL_SELECT*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Skill select");

			MSGSkillSelect(skillselect);
		}
		break;

	case MSG_NO_GM_RESULT :
		{
			MSG_GM_RESULT* gmsetresult = (MSG_GM_RESULT*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : GMS Control result");

			MSGGMSResult(gmsetresult);
		}
		break;

		// cooldown time ����
	case MSG_NO_SKILL_INFO_COOLTIME :
		{
			MSG_SKILL_INFO_COOLTIME* cooltime = (MSG_SKILL_INFO_COOLTIME*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Skill CoolTime info");

			MSGSkillInfoCoolTime(cooltime);
		}
		break;
		
		
	//===============================================================================================================		
	// ���� ���� ����
	case MSG_NO_PS_CTRL :
		{
			MSG_PS_CTRL* psctrl = (MSG_PS_CTRL*)pPacket;
			
			switch(psctrl->ucCtrl) {
			case ps_ctrl_start_result:// 1  _NO_PS_STARTREQ_RESULT           MSG_NO_PS_STARTREQ			//  ���� ��û ���
				{
					MSG_PS_CTRL_START_RESULT* psstartresult = (MSG_PS_CTRL_START_RESULT*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Personal Store Start result");
					
					MSGPersonalStoreCtrlStartResult(psstartresult);
				}
				break;
			case ps_ctrl_broad_cast:// 3 #define MSG_NO_PS_BRD_CST                   MSG_NO_PS_BRD_CSTREQ		// 224
				{
					MSG_PS_CTRL_BROAD_CAST* psnamebroadcasting = (MSG_PS_CTRL_BROAD_CAST*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Personal Store Broad Casting result");
					
					MSGPersonalStoreCtrlNameBroadCasting(psnamebroadcasting);
				}
				break;
			case ps_ctrl_stop_result:// 5 #define MSG_NO_PS_STOPREQ_RESULT            MSG_NO_PS_STOPREQ			// 225 
				{
					MSG_PS_CTRL_STOP_RESULT* psstopresult = (MSG_PS_CTRL_STOP_RESULT*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Personal Store Stop result");
					
					MSGPersonalStoreCtrlStopResult(psstopresult);
				}
				break;
			case ps_ctrl_shopitem_list_result:// 7#define MSG_NO_PS_SHOPITEM_LIST_RESULT      MSG_NO_PS_SHOPITEM_LIST		// 226
				{
					MSG_PS_CTRL_SHOPITEM_LIST_RESULT* psshoplistresult = (MSG_PS_CTRL_SHOPITEM_LIST_RESULT*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Personal Store Shop List result");
					
					MSGPersonalStoreCtrlShoppingListResult(psshoplistresult);
				}
				break;
			case ps_ctrl_shopitem_buy_result:// 9 #define MSG_NO_PS_SHOPITEM_BUY_RESULT       MSG_NO_PS_SHOPITEM_BUY		// 227
				{
					MSG_PS_CTRL_SHOPITEM_BUY_RESULT* psbuyresult = (MSG_PS_CTRL_SHOPITEM_BUY_RESULT*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Personal Store Buy result");
					
					MSGPersonalStoreCtrlBuyResult(psbuyresult);
				}
				break;
			case ps_ctrl_stop_broad_cast:// 10 #define MSG_NO_PS_STOP_BRDCST               228	// 228 �����ο��� ������ ������ �˸�.
				{
					MSG_PS_CTRL_STOP_BROAD_CAST* psstopbroadcasting = (MSG_PS_CTRL_STOP_BROAD_CAST*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Personal Store Stop BroadCasting");
					
					MSGPersonalStoreCtrlStopBroadCasting(psstopbroadcasting);
				}
				break;
			case ps_ctrl_shopitem_sold:// 11 #define MSG_NO_PS_SHOPITEM_SOLD_BRDCST      229	// 229 �����ο��� ������ �ȸ��� �˸���.
				{
					MSG_PS_CTRL_SHOPITEM_SOLD* pstraderesultbroadcasting = (MSG_PS_CTRL_SHOPITEM_SOLD*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Personal Store Trade Result BroadCasting");
					
					MSGPersonalStoreCtrlTradeResultBroadCasting(pstraderesultbroadcasting);
				}
				break;
			case ps_ctrl_item_buy_to_owner:// 12 #define MSG_NO_PS_ITEM_BUY_TOOWNER		  230	// 230 �������� ������ �� ���� �������� �����۰� ���� ��ȭ.
				{
					MSG_PS_CTRL_ITEM_BUY_TO_OWNER* pstraderesulttoowner = (MSG_PS_CTRL_ITEM_BUY_TO_OWNER*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Personal Store Trade Result BroadCasting");
					
					MSGPersonalStoreCtrlTradeResultToOwner(pstraderesulttoowner);
				}
				break;
			}

		}
		break;
	case MSG_NO_ORGANIZATION_UNIT_UPDATE_ :
		{
			MSG_ORGANIZATION_UNIT_UPDATE_* unitupdete = (MSG_ORGANIZATION_UNIT_UPDATE_*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Organization Unit Updete result");

			MSGOrganizationUnitUpdate(unitupdete);
		}
		break;
	case MSG_NO_ORGANIZATION_INFO_RES :
		{
			MSG_ORGANIZATION_INFO_RES* inforesult = (MSG_ORGANIZATION_INFO_RES*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Organization Info result");
			
			MSGOrganizationInfoResult(inforesult);
		}
		break;
	case MSG_NO_ORGANIZATION_RESULT :
		{
			MSG_ORGANIZATION_RESULT* result = (MSG_ORGANIZATION_RESULT*)pPacket;

			VIEWPACKETLOG
			_XDWPACKETLOG("RECEIVE MESSAGE : Organization result");			

			MSGOrganizationResult(result);
		}
		break;
	case MSG_NO_ORGANIZATION_CLIENT :
		{
			MSG_ORGANIZATION_CLIENT_* clientresult = (MSG_ORGANIZATION_CLIENT_*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Organization Client result");			

			MSGOrganizationClient(clientresult);
		}
		break;
	case MSG_NO_ORGANIZATION_GLOBAL_INFO :
		{
			MSG_ORGANIZATION_GLOBAL_INFO* grobalinfo = (MSG_ORGANIZATION_GLOBAL_INFO*)pPacket;
			
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Organization Global Info");

			MSGOrganizationGrobalInfo(grobalinfo);	
		}
		break;
	case MSG_NO_ORGANIZATION_NOTICE :
		{
			MSG_ORGANIZATION_NOTICE* noticeinfo = (MSG_ORGANIZATION_NOTICE*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Organization Notice Info");

			MSGOrganizationNoticeInfo(noticeinfo);				
		}
		break;

	//////////////////////////////////////////////////////////////////////////
	// Billing message
	case MSG_NO_BILL_INFO :
		{
			MSG_BILL_INFO* billinginfo	=	(MSG_BILL_INFO*)pPacket;
			
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Billing Info");

			MSGBillingInfo(billinginfo);
		}
		break;
	
	case MSG_NO_BILLING_INFO_ACK :
		{
			MSG_BILLING_INFO_ACK* billinginfoack = (MSG_BILLING_INFO_ACK*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Vietnam billing Info");

			MSGVietnamBillingInfo(billinginfoack);
		}
		break;
		
	//////////////////////////////////////////////////////////////////////////
	// MINISTOP EVENT ���� ��Ŷ
	case MSG_NO_EVENT_RESULT :
		{
			MSG_EVENT_RESULT* ministopeventresult	=	(MSG_EVENT_RESULT*)pPacket;
			
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : MINISTOP EVENT RESULT");
			
			_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
			if( pEvent_Window ) 
			{
				POINT winpos = pEvent_Window->GetWindowPos();
				memset( pEvent_Window->m_SerialNumber, 0, sizeof(TCHAR)*(5*8) );
				pEvent_Window->m_IMESerialNumber.MoveWindow( winpos.x+1, winpos.y+293 );
				pEvent_Window->m_IMESerialNumber.ClearBuffer();
				pEvent_Window->m_btnSerialNumberOK->EnableWindow(TRUE);
			}
		#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ����� //breif : �̺�Ʈ ������ ������	
			switch( ministopeventresult->ucEventResultMode )
			{
			case en_event_result_one_event :
				{
					MSG_ONE_EVENT_RESULT* tempresult = (MSG_ONE_EVENT_RESULT*)pPacket;
					switch( tempresult->ucResult)
					{
						case en_event_success:			// ���ϵ帳�ϴ�. �̺�Ʈ��ǰ�� �����̽��ϴ�. �೶�� Ȯ���ϼ���
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3052), TRUE);
							pMessageBox->StartTimer(5000, TRUE);

							pEvent_Window->SetEventResult( tempresult->usEventID, TRUE ); // ������ �̺�Ʈ�� ����
							pEvent_Window->SetListItem(); //â�� ������� �� �����Ƿ� ����Ʈ ����

							_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
							if( pEventIcon && pEvent_Window)
							{
								if( pEvent_Window->GetCurrentEventStatus() || //�������� ���� �ʾҴ�. ����Ϸ� ���� �̺�Ʈ�� �ִ�.
                                    pEvent_Window->m_nEventIDinfo || //������ �̺�Ʈ�� ���� �ʾҴ�.
                                    (!pEventIcon->GetShowMessageBox() && g_pLocalUser->m_EventSkillList.m_sSkillID) || //��� ���� â�� Ŭ������ �ʾҴ�.
									pEvent_Window->m_bBuffEvent ) //���� �̺�Ʈ�� �ִ�
									pEventIcon->ShowWindow(TRUE);
								else
									pEventIcon->ShowWindow(FALSE);
							}
						}
						break;
					case en_event_errno_blank:		// ���Դϴ�. ������...
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1596), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
						}
						break;
					case en_event_errno_no_key:		// �߸��� Ű��.
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1597), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
						}
						break;
					case en_event_errno_wait:		// ����Ŀ� �ٽ� �õ����ּ���.
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1598), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
						}
						break;
					case en_event_errno_event_end:	// �̹� ���� �̺�Ʈ �Դϴ�.
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1599), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
						}
						break;
					case en_event_errno_no_event:	// �������� �̺�Ʈ�� �ƴմϴ�.
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1600), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
						}
						break;
					case en_event_errno_used:	// �̹� ����� Ű�� �Դϴ�.
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1601), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
						}
						break;
					case en_event_errno_no_item_slot:	// �κ��丮�� ��á��.
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE ) //Author : ����� //breif : ������ ���� �޽����� Ʋ����..
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3766), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); 
							else
								pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1881), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
						}
						break;
					case en_event_errno_no_charac:	// ������ �ð��.	: �α׸� ���ؼ��� ���δ�.
						{
						}
						break;
					case en_event_errno_fail_insert_item:	// ������ ����������, �������� ���� ���ߴ�.
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1603), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
						}
						break;
					case en_event_errno_system:		// ��Ŷ ���̵��� �ý��� ����.
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1604), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
						}
						break;
					case en_event_errno_no_premium_user: //�����̾� ������ �ƴմϴ�.
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1605), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
						}
						break;
					case en_event_errno_over_insert: //���� �Է�Ƚ�� �ʰ�
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1606), TRUE);
							pMessageBox->StartTimer(5000, TRUE) ;
						}
						break;
					case en_event_is_going_on : // �̺�Ʈ�� �������̴�.( �ű����� �̺�Ʈ�� ����, ������ �� �̸� �˷��ش�. )
						{
						}
						break;
					}
				}	
				break;
			case en_event_result_notice :
				{
					MSG_PRE_NOTICE_EVENT_RESULT* tempresult = (MSG_PRE_NOTICE_EVENT_RESULT*)pPacket;

					for( int i = 0; i < 8; i++ )
					{
						if(tempresult->EventResult[i].usEventID == 0 )
							continue;

						switch(tempresult->EventResult[i].ucResult)
						{
						case en_event_errno_event_end : //������ �̺�Ʈ
							{
								pEvent_Window->SetEventResult( i, tempresult->EventResult[i].usEventID, TRUE );
							}
							break;
						case en_event_is_going_on : // �������� ���� �̺�Ʈ
							{
								pEvent_Window->SetEventResult( i, tempresult->EventResult[i].usEventID, FALSE );
							}
							break;
						}
					}
				}
				break;
			case en_event_result_invite_friend :	// �̱� : ģ�� �ʴ� �̺�Ʈ
				{
					MSG_INVITE_FRIEND_RESULT* result = (MSG_INVITE_FRIEND_RESULT*)pPacket;

					// ��ų �߰�
					g_pLocalUser->m_EventSkillList.m_sSkillID = result->sAddHyperRunID;
					g_pLocalUser->m_EventSkillList.m_cSkillLevel = 11;			// ������ 12��
					g_pLocalUser->m_EventSkillList.m_cCurMission = 1;			// �̺�Ʈ ������ 1
					g_pLocalUser->m_EventSkillList.m_iSkillExp = 0;
					g_pLocalUser->m_EventSkillList.m_ucSkillState = 0;
					g_pLocalUser->m_EventSkillList.m_cSelectedSkillLevel = 11;	// ������ 12��
					g_pLocalUser->m_EventSkillList.m_cOnOff = 0;
					g_pLocalUser->m_EventSkillList.m_dwCooldownStartTime = 0;

					// ������ �ð�
					g_pLocalUser->m_EventSkillEndTime.dwYear = result->EndTime.uiYear;
					g_pLocalUser->m_EventSkillEndTime.dwMonth = result->EndTime.uiMonth;
					g_pLocalUser->m_EventSkillEndTime.dwDay = result->EndTime.uiDay;
					g_pLocalUser->m_EventSkillEndTime.dwHour = result->EndTime.uiHour;
					g_pLocalUser->m_EventSkillEndTime.dwMinute = result->EndTime.uiMinute;
					g_pLocalUser->m_EventSkillEndTime.dwSecond = result->EndTime.uiSecond;

					pEvent_Window->SetListItem(); //â�� ������� �� �����Ƿ� ����Ʈ ����

					_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
					if( pEventIcon )
					{
						// ��� �̺�Ʈ Ÿ�� ���� - 5 : Ŭ������ �� ó���� ���� �ϱ� ���� �����س���
						pEventIcon->SetEventType(5);
						pEventIcon->ShowWindow(TRUE);
					}					
				}
				break;
			}
		#else
			switch(ministopeventresult->ucResult) 
			{
			case en_event_success:			// ���ϵ帳�ϴ�. �̺�Ʈ��ǰ�� �����̽��ϴ�. �೶�� Ȯ���ϼ���
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3052), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			case en_event_errno_blank:		// ���Դϴ�. ������...
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1596), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			case en_event_errno_no_key:		// �߸��� Ű��.
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1597), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			case en_event_errno_wait:		// ����Ŀ� �ٽ� �õ����ּ���.
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1598), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			case en_event_errno_event_end:	// �̹� ���� �̺�Ʈ �Դϴ�.
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1599), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			case en_event_errno_no_event:	// �������� �̺�Ʈ�� �ƴմϴ�.
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1600), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			case en_event_errno_used:	// �̹� ����� Ű�� �Դϴ�.
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					
					if( pEvent_Window->m_SelectedEventID == 29 )
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _T("�̹� ����õ�� 7�ϱ� �������� ���� �ϼ̽��ϴ�."), TRUE);
					else
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1601), TRUE);

					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			case en_event_errno_no_item_slot:	// �κ��丮�� ��á��.
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1881), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			case en_event_errno_no_charac:	// ������ �ð��.	: �α׸� ���ؼ��� ���δ�.
				{
				}
				break;
			case en_event_errno_fail_insert_item:	// ������ ����������, �������� ���� ���ߴ�.
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1603), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			case en_event_errno_system:		// ��Ŷ ���̵��� �ý��� ����.
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1604), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			case en_event_errno_no_premium_user: //�����̾� ������ �ƴմϴ�.
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1605), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			case en_event_errno_over_insert: //���� �Է�Ƚ�� �ʰ�
				{
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_1606), TRUE);
					pMessageBox->StartTimer(5000, TRUE) ;
				}
				break;
			}
		#endif
		}
		break;
			//Author : ����� //breif : ���� �̺�Ʈ //Date : 08/03/19
	case MSG_NO_PRESENT :
		{
			MSG_PRESENT* pPresentEvent	=	(MSG_PRESENT*)pPacket;
			
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : MSG_PRESENT");

			switch( pPresentEvent->ucMode )
			{
			case en_present_mode_notice :
				{
					MSG_PRESENT_NOTICE* tempresult = (MSG_PRESENT_NOTICE*)pPacket;

					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof( messagestring));	

					_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3800), //"%d���� ������ ���� �߽��ϴ�. %d���� �೶ ������ �ʿ��մϴ�.", 
							tempresult->iInfo, tempresult->ucNeedInvenSlotCount );
					InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);
					
					//������ ������.. ����Ʈ�������� �����ϰ� �������� ����ؾ��Ѵ�.
					_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
					
				#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ����� //breif : �̺�Ʈ ������ ������	
					_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
					pEventIcon->ShowWindow(TRUE);
				#endif

					pEvent_Window->m_nEventIDinfo = tempresult->sEventID;
					pEvent_Window->SetListItem();
					
				}
				break;
			case en_present_mode_recv_present :	// s -> c : ������ �ش�.
				{
					MSG_PRESENT_RECV_PRESENT* tempresult = (MSG_PRESENT_RECV_PRESENT*)pPacket;

					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof( messagestring));	

					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_cType(tempresult->UserInven.cType);
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_cSecond(tempresult->UserInven.cSecond);
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_sID( tempresult->UserInven.sID );
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].m_cIdentify = tempresult->UserInven.Item_Money.S_NormalItem.cIdentify;
					
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_cThird(_XGameItem::GetThird(tempresult->UserInven.cType, tempresult->UserInven.sID));
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_ucCount(tempresult->UserInven.Item_Money.S_NormalItem.ucCount);
					
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_usCurDur( tempresult->UserInven.Item_Money.S_NormalItem.usCurDur );
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_usMaxDur( _XGameItem::GetMaxDurability(tempresult->UserInven.cType, tempresult->UserInven.sID) );

					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_ucSocketCount( tempresult->UserInven.Item_Money.S_NormalItem.ucSocketCount );
					if( tempresult->UserInven.Item_Money.S_NormalItem.ucSocketCount > 0 )
					{
						for( int i = 0; i < 4; ++i )
							g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_sSocket( i, tempresult->UserInven.sSocket[i] );
					}
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_ucStrength( tempresult->UserInven.ucStrength );
					#ifdef _XDEF_SMELT_ITEM //Author : �����
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].m_usCurProtect  = tempresult->UserInven.uselimit.usCurProtect; // �ٸ��븰�Ǽ�
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].m_usCurMaxProtect = tempresult->UserInven.uselimit.usCurMaxProtect; // �ִ밭�� ������  ������ ��ġ��
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].m_usUsedProtect = tempresult->UserInven.uselimit.usUsedProtect; // ��� ��뿩��
					
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_ucWeaponStrength( _XGameItem::GetNature(tempresult->UserInven.cType, tempresult->UserInven.sID) );
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_ucSharpness( tempresult->UserInven.ucSharpness );
					#else
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_ucSharpness( tempresult->UserInven.ucSharpness );
					#endif


					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].m_ucFlag1 = tempresult->UserInven.uselimit.ucFlag1;

					#ifndef _XDEF_SMELT_ITEM //author : ����� //breif : ������ �Ǿ������� ������ �ʴ´�
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].m_ucFlag2	= tempresult->UserInven.uselimit.ucFlag2;	
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].m_ucFlag3	= tempresult->UserInven.uselimit.ucFlag3;
					#endif
					g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].m_usValue1	= tempresult->UserInven.uselimit.usValue1;

					if( tempresult->UserInven.uselimit.ucYear > 0 )
					{				
						g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_ucYear(tempresult->UserInven.uselimit.ucYear);	
						g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_ucMonth(tempresult->UserInven.uselimit.ucMonth);	
						g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_ucDay( tempresult->UserInven.uselimit.ucDay );	
						g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_ucHour(tempresult->UserInven.uselimit.ucHour);	
						g_pLocalUser->m_UserItemList[tempresult->UserInven.cSlot].Set_m_ucMinute(tempresult->UserInven.uselimit.ucMinute);
					}

					//sprintf( messagestring, "%d���� ������ �����̽��ϴ�.", tempresult->iInfo );
					_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3052)); //���ϵ帳�ϴ�. �̺�Ʈ��ǰ�� �����̽��ϴ�. �೶�� Ȯ���ϼ���.
					InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);

					_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
					
					//������ �޾����� �ʱ�ȭ
					pEvent_Window->m_nEventIDinfo = 0;
					pEvent_Window->SetListItem();

				#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ����� //breif : �̺�Ʈ ������ ������	
					_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
					pEventIcon->ShowWindow(FALSE); //�̺�Ʈ�� �޾����� ���� �������� �ȱ׸���.

					if( pEvent_Window->GetCurrentEventStatus() || //�׷��� ������������ ���� �̺�Ʈ�� ���� ���̸� �ٽ� ���� �������� �׷��ش�
                        (!pEventIcon->GetShowMessageBox() && g_pLocalUser->m_EventSkillList.m_sSkillID) ||  //��� ���� �̺�Ʈ �����ڽ��� Ŭ�� ���� �ʾҴ�.
						pEvent_Window->m_bBuffEvent ) //���� �̺�Ʈ â
						pEventIcon->ShowWindow(TRUE);
				#endif
				}
				break;
			case en_present_mode_error :	// ���� �޽��� : req_present �� �������� ���ȴ�. 
				{
					MSG_PRESENT_ERROR* tempresult = (MSG_PRESENT_ERROR*)pPacket;

					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof( messagestring));	

					switch( tempresult->ucErrorCode )
					{
					case en_present_error_no_event :	// �׷� �̺�Ʈ ����. 
						{
							_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_1599)); //�̹� ���� �̺�Ʈ �Դϴ�.
							InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						}
						break;
					case en_present_error_already_recv :	// �̹� �޾ư� ����.
						{
							_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3801), tempresult->iInfo ); //"%d���� ������ �̹� �����̽��ϴ�."
							InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						}
						break;
					case en_present_error_need_inven :	// �κ� ������ ����. �ٽ� �õ��� ��.
						{
							_snprintf( messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1991) ); //�೶�� ������ �����մϴ�. ���� �ٽ� �õ��ϼ���.
							InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
						}
						break;
					}
				}
				break;
			}
		}
		break;
	case MSG_NO_US_AD_REWARD_ACK :
		{
#ifdef _ACCLAIM_IGAADSYSTEM
			MSG_US_AD_REWARD_ACK* us_ad_reward_ack = (MSG_US_AD_REWARD_ACK*)pPacket;
			
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Acclaim ad result");

			CEmperorOfDragonsApp* pApp = ((CEmperorOfDragonsApp*)gApp);
			_XProcess* pCurrentProcess = pApp->m_ProcessManager.GetCurrentProcess();
		
			if( pCurrentProcess == &(pApp->m_proc_maingame) )
			{
				_ADItemInfo* pItemInfo = NULL;
				
				pApp->m_proc_maingame.m_DeathAD_Gold = us_ad_reward_ack->dwGold;
				pApp->m_proc_maingame.m_DeathAD_ItemCnt = us_ad_reward_ack->ucItemCnt;
				
				SAFE_DELETE_ARRAY( pApp->m_proc_maingame.m_pDeathAD_ADIteminfo );
				pApp->m_proc_maingame.m_pDeathAD_ADIteminfo = new _ADItemInfo[ us_ad_reward_ack->ucItemCnt ];
				
				for( int i = 0; i < us_ad_reward_ack->ucItemCnt; i++ )
				{
					pItemInfo = (_ADItemInfo*)(pPacket + 8 + (i * sizeof(_ADItemInfo)) );
					memcpy( &pApp->m_proc_maingame.m_pDeathAD_ADIteminfo[i], pItemInfo, sizeof(_ADItemInfo) );
				}

				pApp->m_proc_maingame.m_DeathADRecievedResult = TRUE;
			}
			else if( pCurrentProcess == &(pApp->m_proc_loading) )
			{
				_ADItemInfo* pItemInfo = NULL;

				pApp->m_proc_loading.m_LoadingAD_Gold = us_ad_reward_ack->dwGold;
				pApp->m_proc_loading.m_LoadingAD_ItemCnt = us_ad_reward_ack->ucItemCnt;

				SAFE_DELETE_ARRAY( pApp->m_proc_loading.m_pLoadingAD_ADIteminfo );
				pApp->m_proc_loading.m_pLoadingAD_ADIteminfo = new _ADItemInfo[ us_ad_reward_ack->ucItemCnt ];
				
				for( int i = 0; i < us_ad_reward_ack->ucItemCnt; i++ )
				{
					pItemInfo = (_ADItemInfo*)(pPacket + 8 + (i * sizeof(_ADItemInfo)) );
					memcpy( &pApp->m_proc_loading.m_pLoadingAD_ADIteminfo[i], pItemInfo, sizeof(_ADItemInfo) );
				}

				pApp->m_proc_loading.m_RecievedADResult = TRUE;
			}
#endif
		}
		break;

	case MSG_NO_NATIONAL_DIST :
		{
			MSG_NATIONAL_DIST* ndist = (MSG_NATIONAL_DIST*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : National Dist");

			switch(ndist->ucMode)
			{
			case en_national_vina_sds_ctrl :
				{
	#ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI
					MSG_NDIST_VINA_SDS_CTRL* sdsctrl = (MSG_NDIST_VINA_SDS_CTRL*)pPacket;
					if(sdsctrl->ucStop == 0)
					{
						// stop
						if(g_pJinStatus_Window)
						{
							_XDWINPRINT(_T("[Shut down Stop] "));
//							_XLog(_T("[Shut down Stop] "));

							g_pJinStatus_Window->SetSDSState(1);
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3676), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);

							g_pJinStatus_Window->SetSDSSetTime(g_LocalSystemTime);
						}
					}
					else if(sdsctrl->ucStop == 1)
					{
						// continue
						if(g_pJinStatus_Window)
						{
							_XDWINPRINT(_T("[Shut down continue] Elapsed minute : %d / Elapsed second : %d"), sdsctrl->sCurFatigue, sdsctrl->ucElapsedSecond);
//							_XLog(_T("[Shut down continue] Elapsed minute : %d / Elapsed second : %d"), sdsctrl->sCurFatigue, sdsctrl->ucElapsedSecond);

							if( sdsctrl->sCurFatigue < 300 )
							{
								g_PlayLeftTime = (3600000*5) - (sdsctrl->sCurFatigue*60000) - sdsctrl->ucElapsedSecond;
							}
							else
							{
								g_PlayLeftTime = 0;
							}

							g_PlayTime = g_LocalSystemTime;
							g_pJinStatus_Window->SetSDSState(0);
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3675), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE);

							g_pJinStatus_Window->SetSDSSetTime(g_LocalSystemTime);
						}
					}
					else
					{
						_XDWINPRINT("SDS ctrl fail......");
					}
	#endif //_XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI
				}
				break;
			case en_national_metel_apex_kill_user :
				{
	#ifdef _XUSEAPEXMODULE
					MSG_NDIST_METEL_APEX_KILL_USER* apexkill = (MSG_NDIST_METEL_APEX_KILL_USER*)pPacket;

					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Apex user kill message");

					_XLog("===================================");
					_XLog("Receive Apex user kill message");
					_XLog("===================================");

					if(apexkill->ucError == 0)
					{
						g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3845), TRUE); //_T("apex kill message.")
					}
	#endif //_XUSEAPEXMODULE
				}
				break;
			}
		}
		break;
		
	case MSG_NO_INCHANT_CTRL :
		{
			MSG_INCHANT_PACKET* inchantresult = (MSG_INCHANT_PACKET*)pPacket;
			
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Inchant Result");

			MSGItemInchantResult(inchantresult);
		}
		break;
	case MSG_NO_NPC_BUFF_RESULT :
		{
			MSG_NPC_BUFF_RESULT* npcbuffresult = (MSG_NPC_BUFF_RESULT*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : NPC buff result");

			MSGNPCBuffResult(npcbuffresult);
		}
		break;
	case MSG_NO_BLOODPOINT_CTRL :
		{
			MSG_BLOODPOINT_CTRL* blpctrl = (MSG_BLOODPOINT_CTRL*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Blood point ctrl");

			switch(blpctrl->ucMode)
			{
			case en_bppk_mode_setting_point :	//���� ����Ʈ ����
				{
					MSG_BLOODPOINT_SETTING_POINT* setpoint = (MSG_BLOODPOINT_SETTING_POINT*)pPacket;

					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Blood point setting point");

					MSGBloodPointSettingPoint(setpoint);
				}
				break;
			case en_bppk_mode_setting_monster_kill :	// ���� ų��
				{
					// ���߿�
				}
				break;
			case en_bppk_mode_give_item :		// ������ �ֱ�
				{
					MSG_BLOODPOINT_GIVE_ITEM* giveitem = (MSG_BLOODPOINT_GIVE_ITEM*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Blood point give item");
					
					MSGBloodPointGiveItem(giveitem);
				}
				break;
			case en_bppk_mode_error :			// error
				{
					MSG_BLOODPOINT_ERROR* blperror = (MSG_BLOODPOINT_ERROR*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : Blood point Error");
					
					MSGBloodPointError(blperror);
				}
				break;
			}
		}
		break;
		
		// Pk ����
	case MSG_NO_PK_RECORD :
		{
			MSG_PK_RECORD* pkrecord = (MSG_PK_RECORD*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : PK Record");

			switch(pkrecord->ucMode)
			{
			case en_pk_record_win_lose_record :
				{
					MSG_PK_WIN_LOSE_RECORD* record = (MSG_PK_WIN_LOSE_RECORD*)pPacket;

					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : PK Record - win/lose record");

					MSGPKWinLoseRecord(record);
				}
				break;
			case en_pk_record_moonpa_record :
				{
					MSG_PK_MOONPA_RECORD* record = (MSG_PK_MOONPA_RECORD*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : PK Record - moonpa record");
					
					MSGPKMoonpaRecord(record);
				}
				break;
			case en_pk_record_class_record :
				{
					MSG_PK_CLASS_RECORD* record = (MSG_PK_CLASS_RECORD*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : PK Record - class record");
					
					MSGPKClassRecord(record);
				}
				break;
			case en_pk_record_update_win :
				{
					MSG_PK_UPDATE_WIN* record = (MSG_PK_UPDATE_WIN*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : PK Record - update win");
					
					MSGPKUpdateWin(record);
				}
				break;
			case en_pk_record_update_lose :
				{
					MSG_PK_UPDATE_LOSE* record = (MSG_PK_UPDATE_LOSE*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : PK Record - update lose");
					
					MSGPKUpdateLose(record);
				}
				break;
			case en_pk_record_update_moonpa_win :
				{
					MSG_PK_UPDATE_MOONPA_WIN* record = (MSG_PK_UPDATE_MOONPA_WIN*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : PK Record - update moonpa win");
					
					MSGPKUpdateMoonpaWin(record);
				}
				break;
			case en_pk_record_update_moonpa_lose :
				{
					MSG_PK_UPDATE_MOONPA_LOSE* record = (MSG_PK_UPDATE_MOONPA_LOSE*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : PK Record - update moonpa lose");
					
					MSGPKUpdateMoonpaLose(record);
				}
				break;
			case en_pk_record_update_class_win :
				{
					MSG_PK_UPDATE_CLASS_WIN* record = (MSG_PK_UPDATE_CLASS_WIN*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : PK Record - update class win");
					
					MSGPKUpdateClassWin(record);
				}
				break;
			case en_pk_record_update_class_lose :
				{
					MSG_PK_UPDATE_CLASS_LOSE* record = (MSG_PK_UPDATE_CLASS_LOSE*)pPacket;
					
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : PK Record - update class lose");
					
					MSGPKUpdateClassLose(record);
				}
				break;
			case en_pk_record_abuse_to_win :
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : PK Record - abuse to win");

					//_T("�ֱ� ���� ����� �ִ� ��븦 �����߷Ƚ��ϴ�. ���� ����� ������ ������ ���� ������ ��/�Ǹ��� ������� �ʽ��ϴ�.")
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2738), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
				}
				break;
			case en_pk_record_abuse_to_lose :
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : PK Record - abuse to lose");
					//_T("�ֱ� ���� ����� �ִ� ��뿡�� ���������ϴ�. ���� ����� ������ ������ ���� ������ ��/�Ǹ��� ������� �ʽ��ϴ�.")
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2739), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case en_pk_record_lack_honor_to_win :
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : PK Record - lack honor to win");

					//_T("����� ���� �Ǹ��� ���Ƽ� ���� ����� ������ ������ ���� ������ ��/�Ǹ��� ������� �ʽ��ϴ�.")
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2740), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case en_pk_record_lack_honor_to_lose :
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSAGE : PK Record - lack honor to win");

					//_T("�ڽ��� ���� �Ǹ��� ���Ƽ� ���� ����� ������ ������ ���� ������ ��/�Ǹ��� ������� �ʽ��ϴ�.")
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2741), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			}
		}
		break;
		
#ifdef _XDEF_CASHITEM_SHOUT
		// etc set - ��ġ�� etc.
	case MSG_NO_ETCSET :
		{
			MSG_ETCSET* etcset = (MSG_ETCSET*)pPacket;
			
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : ETC set");

			switch(etcset->ucMode)
			{
			case en_etcset_mode_shout_set :
			case en_etcset_mode_shout_update :
				{
					MSG_ETCSET_SHOUT_SET* shoutset = (MSG_ETCSET_SHOUT_SET*)pPacket;
					MSGEtcsetShoutSet(shoutset);
				}
				break;
			case en_etcset_mode_shout_end :
				{
					MSG_ETCSET_SHOUT_END* shoutend = (MSG_ETCSET_SHOUT_END*)pPacket;
					MSGEtcsetShoutEnd(shoutend);
				}
				break;
			case en_etcset_mode_shout_error :
				{
					MSG_ETCSET_SHOUT_ERROR* shouterror = (MSG_ETCSET_SHOUT_ERROR*)pPacket;
					MSGEtcsetShoutError(shouterror);
				}
				break;
			}
		}
		break;
#endif
		
		// ����ġ/���õ�/������� ������
	case MSG_NO_ACCOUNT_ITEM_USE_STATUS :
		{
			MSG_ACCOUNT_ITEM_USE_STATUS* aius = (MSG_ACCOUNT_ITEM_USE_STATUS*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Account Item use status");

			switch(aius->ucMode)
			{
			case en_aius_mode_grade_set :
				{
					MSG_AIUS_GRADE_SET* aiusset = (MSG_AIUS_GRADE_SET*)pPacket;
					MSGAiusGradeSet(aiusset);
				}
				break;
			case en_aius_mode_grade_end :
				{
					MSG_AIUS_GRADE_END* aiusend = (MSG_AIUS_GRADE_END*)pPacket;
					MSGAiusGradeEnd(aiusend);
				}
				break;
			}
		}
		break;
		
#ifdef _XTS_ITEMMALLBROWSER
	case MSG_NO_ITEM_SHOP :
		{
			MSG_ITEM_SHOP_BASE* itemshop = (MSG_ITEM_SHOP_BASE*)pPacket;
			
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE PACKET : Item Shop Ack");
			
			switch(itemshop->ucMode) 
			{
			case PKTMODE_PERSONAL_CASH_INVEN_ACK :
			case PKTMODE_PRESENT_CASH_INVEN_ACK :
				{
					//MSG_GS2U_ITEM_SHOP__PERSONAL_CASH_INVEN_ACK* cashiteminven = (MSG_GS2U_ITEM_SHOP__PERSONAL_CASH_INVEN_ACK*)pPacket;
					//MSG_GS2U_ITEM_SHOP__PRESENT_CASH_INVEN_ACK* cashiteminven = (MSG_GS2U_ITEM_SHOP__PRESENT_CASH_INVEN_ACK*)pPacket;
									
					_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
					if( pCashBox_Window )
					{
						
						if( !pCashBox_Window->m_listCashItem.empty() )
						{
							_XCASHITEMBOX_INFO* cashitem = NULL;
							list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
							for(iter_cashitem = pCashBox_Window->m_listCashItem.begin() ; iter_cashitem != pCashBox_Window->m_listCashItem.end() ; )
							{
								cashitem = *iter_cashitem;
								if( cashitem )
								{
									SAFE_DELETE(cashitem);
									iter_cashitem = pCashBox_Window->m_listCashItem.erase(iter_cashitem);
								}
							}
						}
						//Author : ����� //breif : m_listCashItem, clear ���൵ �����ϴ�..
						pCashBox_Window->m_listCashItem.clear();
					
						MSG_GS2U_ITEM_SHOP__PERSONAL_CASH_INVEN_ACK ack;
						int iOffset = sizeof(MSG_ITEM_SHOP_BASE);
						memcpy(&ack.ucRowCount, pPacket + iOffset, sizeof(UCHAR)); iOffset += sizeof(UCHAR);
						for(int i =0; i < ack.ucRowCount; ++i) 
						{
							memcpy(&ack.iOrderNo, pPacket + iOffset, sizeof(int));  iOffset += sizeof(int);
							memcpy(&ack.iProductNo, pPacket + iOffset, sizeof(int));  iOffset += sizeof(int);
							memcpy(&ack.iPkgIdx, pPacket + iOffset, sizeof(int));  iOffset += sizeof(int);
							
							_XCASHITEMBOX_INFO* pCashPackageItem = new _XCASHITEMBOX_INFO;
							pCashPackageItem->indexPackage = ack.iPkgIdx;//��Ű�� �ε���
							pCashPackageItem->OrderNumber = ack.iOrderNo;//�ֹ���ȣ
							pCashPackageItem->ProductNumber = ack.iProductNo;//��ǰ��ȣ
							pCashPackageItem->bCheck = FALSE;
							
							pCashBox_Window->m_listCashItem.push_back(pCashPackageItem);
						}

						pCashBox_Window->SetListItem();

						
						//pCashBox_Window->m_btnMoveAllItem->EnableWindow(TRUE);
						pCashBox_Window->m_btnMoveSelectedItem->EnableWindow(TRUE);
						pCashBox_Window->m_btnRenew->EnableWindow(TRUE);
						pCashBox_Window->m_TabButton[0]->EnableWindow(TRUE);
						pCashBox_Window->m_TabButton[1]->EnableWindow(TRUE);
					}
				}
				break;
			case PKTMODE_MOVE_PERSONAL_PACKAGE_ACK :
			case PKTMODE_MOVE_PRESENT_PACKAGE_ACK :
				{
					_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
					if( pCashBox_Window )
					{						
						MSG_GS2U_ITEM_SHOP__MOVE_PERSONAL_ITEM_ACK ack;
						int iOffset = sizeof(MSG_ITEM_SHOP_BASE);

						// ĳ�� �����Կ��� ���� ������
						memcpy(&ack.ucPkgCnt, (char*)pPacket + iOffset , sizeof(UCHAR)); iOffset += sizeof(UCHAR);
						if( ack.ucPkgCnt == 0 )
						{							
							// ���� ó��
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1698),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
						}
						else
						{
							for(int i =0; i < ack.ucPkgCnt; ++i) 
							{
								memcpy(&ack.iOrderNo, pPacket + iOffset, sizeof(int));  iOffset += sizeof(int);
								memcpy(&ack.iProductNo, pPacket + iOffset, sizeof(int));  iOffset += sizeof(int);
								memcpy(&ack.iPkgIdx, pPacket + iOffset, sizeof(int));  iOffset += sizeof(int);
								
								if( !pCashBox_Window->m_listCashItem.empty() )
								{
									_XCASHITEMBOX_INFO* cashitem = NULL;
									list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
									for(iter_cashitem = pCashBox_Window->m_listCashItem.begin() ; iter_cashitem != pCashBox_Window->m_listCashItem.end() ; )
									{
										cashitem = *iter_cashitem;
										if( cashitem )
										{
											if( cashitem->indexPackage == ack.iPkgIdx && cashitem->OrderNumber == ack.iOrderNo
												&& cashitem->ProductNumber == ack.iProductNo )
											{
												SAFE_DELETE(cashitem);
												iter_cashitem = pCashBox_Window->m_listCashItem.erase(iter_cashitem);
											}
											else
											{
												++iter_cashitem;
											}
										}
									}
								}
								
								// �೶�� �� ������
								memcpy(&ack.ucItemCnt, pPacket + iOffset , sizeof(UCHAR)); iOffset += sizeof(UCHAR);
								for( int j =0; j < ack.ucItemCnt; ++j) 
								{
									memcpy(&ack.cSlot, pPacket + iOffset, sizeof(char));  iOffset += sizeof(char);
									memcpy(&ack.cType, pPacket + iOffset, sizeof(char));  iOffset += sizeof(char);
									memcpy(&ack.sId, pPacket + iOffset, sizeof(short));  iOffset += sizeof(short);
									memcpy(&ack.ucNum, pPacket + iOffset, sizeof(UCHAR));  iOffset += sizeof(UCHAR);
									
									if( ack.cSlot >= 0 && ack.cSlot < _XINVENTORY_SLOTTYPE_TOTALCOUNT)
									{
										//g_pLocalUser->m_UserItemList[ack.cSlot].Reset();
                                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��
                                        if( _XINVENTORY_SLOTTYPE_CASHBACKPACK1 == ack.cSlot )
                                        {
                                            CheckValidItem(ack.cSlot);

                                            g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART;
                                            g_pInventory_Window->SetTabButtonPosition();
                                        
						                    g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
						                    g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                                        }
                                    #endif
										
										if(g_pLocalUser->m_UserItemList[ack.cSlot].Get_m_cType() == ack.cType&& 
											g_pLocalUser->m_UserItemList[ack.cSlot].Get_m_sID() == ack.sId)
										{
											if(g_pLocalUser->m_UserItemList[ack.cSlot].IsStackable())
											{
												g_pLocalUser->m_UserItemList[ack.cSlot].Set_m_ucCount( ack.ucNum );
											}
											else
											{									
												g_pLocalUser->m_UserItemList[ack.cSlot].Set_m_cType(ack.cType);
												g_pLocalUser->m_UserItemList[ack.cSlot].Set_m_sID( ack.sId );
												g_pLocalUser->m_UserItemList[ack.cSlot].Set_m_ucCount(ack.ucNum);
												
												// Second Type setting 
												g_pLocalUser->m_UserItemList[ack.cSlot].Set_m_cSecond(_XGameItem::GetItemSecond(ack.cType, ack.sId));
												
												// ������ setting
												g_pLocalUser->m_UserItemList[ack.cSlot].Set_m_usMaxDur( _XGameItem::GetMaxDurability(ack.cType, ack.sId) );
												g_pLocalUser->m_UserItemList[ack.cSlot].Set_m_usCurDur( _XGameItem::GetMaxDurability(ack.cType, ack.sId) );
													
											}
										}
										else
										{
											g_pLocalUser->m_UserItemList[ack.cSlot].Set_m_cType( ack.cType);
											g_pLocalUser->m_UserItemList[ack.cSlot].Set_m_sID(ack.sId);
											g_pLocalUser->m_UserItemList[ack.cSlot].Set_m_ucCount( ack.ucNum);
											
											// Second Type setting 
											g_pLocalUser->m_UserItemList[ack.cSlot].Set_m_cSecond(_XGameItem::GetItemSecond(ack.cType, ack.sId));
											
											// ������ setting
											g_pLocalUser->m_UserItemList[ack.cSlot].Set_m_usMaxDur(_XGameItem::GetMaxDurability(ack.cType, ack.sId));
											g_pLocalUser->m_UserItemList[ack.cSlot].Set_m_usCurDur(_XGameItem::GetMaxDurability(ack.cType, ack.sId));													
										}
									}
								}	
								// �೶�� �� ���� �� 
								
							}// end for(int i =0; i < ack.ucPkgCnt; ++i) 
							pCashBox_Window->SetListItem();
						}
						
						
//						pCashBox_Window->m_btnMoveAllItem->EnableWindow(TRUE);
						pCashBox_Window->m_btnMoveSelectedItem->EnableWindow(TRUE);
						pCashBox_Window->m_btnRenew->EnableWindow(TRUE);
						pCashBox_Window->m_TabButton[0]->EnableWindow(TRUE);
						pCashBox_Window->m_TabButton[1]->EnableWindow(TRUE);
					}				
				}
				break;
			}
		}
		break;		
#endif		
	// �Ⱓ�� ������ ���� 
	case MSG_NO_PERIOD_ITEM_BASE :
		{
			MSG_PERIOD_ITEM_BASE* perioditem = (MSG_PERIOD_ITEM_BASE*)pPacket;
			
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE PACKET : Period Item Result");
			
			switch(perioditem->ucMode) 
			{
			case en_pitem_add :
				{
					_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
					if( pCashBox_Window )
					{				
						MSG_PITEM_ADD addperioditem;
						int iOffset = sizeof(MSG_PERIOD_ITEM_BASE);
						// ĳ�� �����Կ��� ���� ������
						memcpy(&addperioditem.ucPkgCnt, (char*)pPacket + iOffset , sizeof(UCHAR)); iOffset += sizeof(UCHAR);
						if( addperioditem.ucPkgCnt == 0 )
						{							
							// ���� ó��
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1698),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
						}		
						else
						{
							
							for(int i =0; i < addperioditem.ucPkgCnt; ++i) 
							{
								memcpy(&addperioditem.iOrderNo, pPacket + iOffset, sizeof(int));  iOffset += sizeof(int);
								memcpy(&addperioditem.iProductNo, pPacket + iOffset, sizeof(int));  iOffset += sizeof(int);
								memcpy(&addperioditem.iPkgIdx, pPacket + iOffset, sizeof(int));  iOffset += sizeof(int);
								
								if( !pCashBox_Window->m_listCashItem.empty() )
								{
									_XCASHITEMBOX_INFO* cashitem = NULL;
									list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
									for(iter_cashitem = pCashBox_Window->m_listCashItem.begin() ; iter_cashitem != pCashBox_Window->m_listCashItem.end() ; )
									{
										cashitem = *iter_cashitem;
										if( cashitem )
										{
											if( cashitem->indexPackage == addperioditem.iPkgIdx && 
												cashitem->OrderNumber == addperioditem.iOrderNo && 
												cashitem->ProductNumber == addperioditem.iProductNo )
											{
												SAFE_DELETE(cashitem);
												iter_cashitem = pCashBox_Window->m_listCashItem.erase(iter_cashitem);
											}
											else
											{
												++iter_cashitem;
											}
										}
									}
								}
								
								// �೶�� �� ������
								memcpy(&addperioditem.ucItemCnt, pPacket + iOffset , sizeof(UCHAR)); iOffset += sizeof(UCHAR);
								for( int j =0; j < addperioditem.ucItemCnt; ++j) 
								{
									memcpy(&addperioditem.client_item , pPacket + iOffset, sizeof(_sUser_Inven_v1_all));  
									iOffset += sizeof(_sUser_Inven_v1_all);

									if( addperioditem.client_item.sID > 0 )
									{			
										int slotnumber = addperioditem.client_item.cSlot;
										
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_cType(addperioditem.client_item.cType);
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_cSecond(addperioditem.client_item.cSecond);
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_sID(addperioditem.client_item.sID);
										
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_cThird(_XGameItem::GetThird(addperioditem.client_item.cType, addperioditem.client_item.sID));  
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucCount(addperioditem.client_item.Item_Money.S_NormalItem.ucCount);
										
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_usCurDur( addperioditem.client_item.Item_Money.S_NormalItem.usCurDur );
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_usMaxDur( _XGameItem::GetMaxDurability(addperioditem.client_item.cType, addperioditem.client_item.sID) );
										
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSocketCount( addperioditem.client_item.Item_Money.S_NormalItem.ucSocketCount );
										if( addperioditem.client_item.Item_Money.S_NormalItem.ucSocketCount > 0 )
										{
											for( int i = 0; i < 4; ++i )
												g_pLocalUser->m_UserItemList[slotnumber].Set_m_sSocket( i, addperioditem.client_item.sSocket[i] );
										}									
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucStrength( addperioditem.client_item.ucStrength );
										
#ifdef _XDEF_SMELT_ITEM //Author : �����
										g_pLocalUser->m_UserItemList[slotnumber].m_usCurProtect  = addperioditem.client_item.uselimit.usCurProtect;
										//g_pLocalUser->m_UserItemList[slotnumber].m_ucIncProtect = addperioditem.client_item.ucIncProtect;
										g_pLocalUser->m_UserItemList[slotnumber].m_usCurMaxProtect = addperioditem.client_item.uselimit.usCurMaxProtect;
										g_pLocalUser->m_UserItemList[slotnumber].m_usUsedProtect = addperioditem.client_item.uselimit.usUsedProtect;
										
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucWeaponStrength( _XGameItem::GetNature(addperioditem.client_item.cType, addperioditem.client_item.sID) );
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSharpness( addperioditem.client_item.ucSharpness );
#else
										g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSharpness(addperioditem.client_item.ucSharpness);
#endif
										
										g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag1 = addperioditem.client_item.uselimit.ucFlag1;
#ifndef _XDEF_SMELT_ITEM //Author : ����� //breif �����εǾ������� ������ �ʴ´�
										g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag2		= addperioditem.client_item.uselimit.ucFlag2;	
										g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag3		= addperioditem.client_item.uselimit.ucFlag3;
#endif
										g_pLocalUser->m_UserItemList[slotnumber].m_usValue1	= addperioditem.client_item.uselimit.usValue1;

										if( addperioditem.client_item.uselimit.ucYear > 0 )
										{
											g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucYear(addperioditem.client_item.uselimit.ucYear);	
											g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucMonth(addperioditem.client_item.uselimit.ucMonth);	
											g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucDay(addperioditem.client_item.uselimit.ucDay);	
											g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucHour(addperioditem.client_item.uselimit.ucHour);	
											g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucMinute(addperioditem.client_item.uselimit.ucMinute);
										}

                                     #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : �೶ Ȯ��
                                        if( _XINVENTORY_SLOTTYPE_CASHBACKPACK1 == slotnumber )
                                        {
                                            CheckValidItem(slotnumber);

                                            g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART;
                                            g_pInventory_Window->SetTabButtonPosition();
                                        
						                    g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
						                    g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                                        }
                                    #endif
									}
									else
									{
#ifdef _XDWDEBUG
										_XLog( "Idx : %d : Type %d, Second %d, ID %d, Third %d, Count %d, Dur %d, MaxDur %d", i, addperioditem.client_item.cType, addperioditem.client_item.cSecond, addperioditem.client_item.sID,
											_XGameItem::GetThird(addperioditem.client_item.cType, addperioditem.client_item.sID), addperioditem.client_item.Item_Money.S_NormalItem.ucCount,
											addperioditem.client_item.Item_Money.S_NormalItem.usCurDur, _XGameItem::GetMaxDurability(addperioditem.client_item.cType, addperioditem.client_item.sID) );
#endif				
									}
								}	
								// �೶�� �� ���� �� 
								
							}// end for(int i =0; i < ack.ucPkgCnt; ++i) 
							pCashBox_Window->SetListItem();
						}
						
						pCashBox_Window->m_btnMoveSelectedItem->EnableWindow(TRUE);
						pCashBox_Window->m_btnRenew->EnableWindow(TRUE);
						pCashBox_Window->m_TabButton[0]->EnableWindow(TRUE);
						pCashBox_Window->m_TabButton[1]->EnableWindow(TRUE);
					}

				}
				break;
			case en_pitem_set_period :
				{
					MSG_PITEM_SET_PERIOD setperiod;
					int iOffset = sizeof(MSG_ITEM_SHOP_BASE);
					
					memcpy(&setperiod.slot, (char*)pPacket + iOffset , sizeof(UCHAR)); 
					iOffset += sizeof(UCHAR);
					int slotnumber = setperiod.slot;
					
					memcpy(&setperiod.period, (char*)pPacket + iOffset , sizeof(__inven_uselimit_info_v1)); 
					iOffset += sizeof(__inven_uselimit_info_v1);

					g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag1 = setperiod.period.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //Author : ����� //breif : ������ �Ǿ������� ������ �ʴ´� 
					g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag2		= setperiod.period.ucFlag2;	
					g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag3		= setperiod.period.ucFlag3;
#endif
					g_pLocalUser->m_UserItemList[slotnumber].m_usValue1		= setperiod.period.usValue1;

					if( setperiod.period.ucYear > 0 )
					{
						g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucYear(setperiod.period.ucYear);	
						g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucMonth( setperiod.period.ucMonth);	
						g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucDay(setperiod.period.ucDay);	
						g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucHour(setperiod.period.ucHour);	
						g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucMinute(setperiod.period.ucMinute);
					}
					
				}
				break;
			case en_pitem_remove :
				{
					MSG_PITEM_REMOVE removeperioditem;
					int iOffset = sizeof(MSG_ITEM_SHOP_BASE);
					
					memcpy(&removeperioditem.slot, (char*)pPacket + iOffset , sizeof(UCHAR)); 
					iOffset += sizeof(UCHAR);
					
					char itemtype	= g_pLocalUser->m_UserItemList[removeperioditem.slot].Get_m_cType();
					short itemid	= g_pLocalUser->m_UserItemList[removeperioditem.slot].Get_m_sID();

					if(removeperioditem.slot >= _XINVENTORY_SLOTTYPE_AMOR && removeperioditem.slot <= _XINVENTORY_SLOTTYPE_WEAPON_2)
					{
						DWORD lparam = removeperioditem.slot * 100000 + 0;		// ������ �����
						ChangeModel(lparam, 0);
						
						// ����Ʈ �������ǿ� ������ ���� ������ ������ ���������� ���������� üũ�� �Ѵ�.
						g_QuestScriptManager.RecheckWearItemTypeQuestStartCondition( itemtype, itemid  );

						if(removeperioditem.slot == _XINVENTORY_SLOTTYPE_AMOR)
						{
							g_pLocalUser->m_cAmorPocketNum = 0;
						}
						if(removeperioditem.slot == _XINVENTORY_SLOTTYPE_PANTS)
						{
							g_pLocalUser->m_cPantsPocketNum = 0;
						}
						
					}
					else if( removeperioditem.slot >= _XINVENTORY_SLOTTYPE_ACCESSORY_1 &&  removeperioditem.slot <= _XINVENTORY_SLOTTYPE_ACCESSORY_8 )
					{				
						// ���� ���� �Ǽ��縮�϶�  ����Ʈ �������ǿ� ������ ���� ������ ������ ���������� ���������� üũ�� �Ѵ�.
						// ����Ʈ �������ǿ� ������ ���� ������ ������ ���������� ���������� üũ�� �Ѵ�.
						g_QuestScriptManager.RecheckWearItemTypeQuestStartCondition( itemtype, itemid  );
						
					}
					
					// ����Ʈ ���� ���ǿ� ������ üũ�� ������ ���������� ���������� üũ�� �Ѵ�.
					g_QuestScriptManager.RecheckHaveItemTypeQuestStartCondition( itemtype, itemid  );
					
					g_pLocalUser->m_UserItemList[ removeperioditem.slot ].Reset();

                #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER	//2008.05.23 �ڼ��� ����
					if(removeperioditem.slot == _XINVENTORY_SLOTTYPE_CASHBACKPACK1)
					{
						// ĳ�� �೶ ���� ��
                        if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK2].Get_m_sID() > 0 )
                            g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_TOTALCOUNT;
                        else
                            g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART;

                        g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
						g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  136, 0, 168, 32 );
                        g_pInventory_Window->m_bIteminCashBackPack[0] = FALSE;

                        for( int i = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART ; i < _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART; i++ )
                        {
                            if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
                            {
                                g_pInventory_Window->m_bIteminCashBackPack[0] = TRUE;
                            #ifdef _XDWDEBUG
                                g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
					            g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                            #else
                                g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
					            g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                                //g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
	                            //g_pInventory_Window->m_BaseTabButton[3]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
                            #endif
                            }
                        }

                        g_pInventory_Window->SetTabButtonPosition();
					}

                    if(removeperioditem.slot == _XINVENTORY_SLOTTYPE_CASHBACKPACK2)
					{
						// ĳ�� �೶ ���� ��
                        if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 )
                            g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART;
                        else
                            g_pLocalUser->m_nCashBackPackMaxValue = _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART;

                        g_pInventory_Window->m_bIteminCashBackPack[1] = FALSE;

                        g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 136, 0, 168, 32 );
						g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  136, 0, 168, 32 );

                        for( int i = _XINVENTORY_SLOTTYPE_SECOND_CASHBACKSTART ; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; i++ )
                        {
                            if( g_pLocalUser->m_UserItemList[i].Get_m_sID() > 0 )
                            {
                                g_pInventory_Window->m_bIteminCashBackPack[1] = TRUE;
                            #ifdef _XDWDEBUG
                                g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
					            g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                            #else
                                g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 102, 0,  134, 32 );
					            g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  68, 0,  100, 32 );
                                //g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 34, 0,  66, 32 );
	                            //g_pInventory_Window->m_BaseTabButton[4]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 0,  32, 32 );
                            #endif
                            }
                        }

                        g_pInventory_Window->SetTabButtonPosition();
					}
                #endif
					
					g_pLocalUser->UpdateLocalUserAttackLevel();
					
					TCHAR	messagestring[256];
					memset(messagestring, 0, sizeof(messagestring));
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3160),_XGameItem::GetItemName(itemtype, itemid));//%s�� ���Ⱓ�� ����Ǿ� �����Ǿ����ϴ�.
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); 
				}
				break;
#ifdef _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI
			case en_storage_pitem_remove :
				{
					MSG_STORAGE_PITEM_REMOVE* strremove = (MSG_STORAGE_PITEM_REMOVE*)pPacket;

					TCHAR	messagestring[256];
					memset(messagestring, 0, sizeof(messagestring));
					_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_3160),_XGameItem::GetItemName(strremove->cType, strremove->sID));//%s�� ���Ⱓ�� ����Ǿ� �����Ǿ����ϴ�.
					g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); 
				}
				break;
#endif
			}
		}
		break;		

	case MSG_NO_QUICKSLOT :
		{
			MSG_QUICKSLOT* pQuickSlot = (MSG_QUICKSLOT_ALLINFO*)pPacket;

			if( pQuickSlot->ucQuickSlotMode == en_quickslotmode_allinfo )
			{
				VIEWPACKETLOG
				_XDWPACKETLOG("Recieve : Quickslot info");

				MSG_QUICKSLOT_ALLINFO* quickslotinfo = (MSG_QUICKSLOT_ALLINFO*)pPacket;			
				MSGQuickSlot( quickslotinfo );

				//_XLog("RECEIVE PACKET : Quickslot info");
			}
			else if( pQuickSlot->ucQuickSlotMode == en_helplog_allinfo )
			{
				VIEWPACKETLOG
				_XDWPACKETLOG("Recieve : HelpSystem info");

				MSG_HELPLOG_ALLINFO* helpsysteminfo = (MSG_HELPLOG_ALLINFO*)pPacket;
				MSGHelpSystem( helpsysteminfo );
				
				//_XLog("RECEIVE PACKET : HelpSystem info");
			}
		}
		break;
	
		/*
	case MSG_NO_CHECK_XTRAP :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : XTrap Check");

			_XLog("RECEIVE MESSAGE : XTrap Check");

			MSG_XTRAP* xtrap = (MSG_XTRAP*)pPacket;
			switch(xtrap->ucMode)
			{
			case en_xtrap_sc_request :
				{
					MSG_XTRAP_SC_REQUEST* screquest = (MSG_XTRAP_SC_REQUEST*)pPacket;
					MSGXTrapSCRequest(screquest);
				}
				break;
			case en_xtrap_sc_errno :
				{
					MSG_XTRAP_SC_ERRNO* err = (MSG_XTRAP_SC_ERRNO*)pPacket;
//					_XLog("Error<XTrap> : no[%d]", scerrno->ucError);
					MSGXTrapSCErrorNo(err);
				}
				break;
			case en_xtrap_sc_exit :
				{
					MSG_XTRAP_SC_EXIT* scexit = (MSG_XTRAP_SC_EXIT*)pPacket;
					MSGXTrapSCExit(scexit);
				}
				break;
			}
		}
		break;
		*/

	case MSG_NO_SERVER_INFO :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Server info ");

#ifdef _XTS_PKZONE_SERVERSETTING
			MSG_SVRINFO_PK_ZONE* pkinfo = (MSG_SVRINFO_PK_ZONE*)pPacket;

			if(pkinfo->ucMode == en_server_info_pk_zone)
				g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkserverinfo = pkinfo->ucPKZone;
#endif
		}
		break;
#ifdef _XTS_ALIANCE_20061018
		//////////////////////////////////////////////////////////////////////////
		// ���� ���� ��Ŷ.
		//////////////////////////////////////////////////////////////////////////
		
	case MSG_NO_ALLY_BASE:
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Alience Receive");
						
			MSG_ALLY_BASE* alience_receive = (MSG_ALLY_BASE*)pPacket;
			switch(alience_receive->ucMode)
			{
			case en_ally_mode_request_from  :	// ���� ��û�� �Դٴ� ���� �˸���.
				{
					MSG_ALLY_REQEUST_FROM* alience_request_from = (MSG_ALLY_REQEUST_FROM*)pPacket;
					MSGAlienceRequestFrom(alience_request_from);
				}
				break;
			case en_ally_mode_response_no  :	// ������ �������� No
				{
					MSG_ALLY_MODE_RESPONSE_NO* alience_request_no = (MSG_ALLY_MODE_RESPONSE_NO*)pPacket;
					MSGAlienceRequestNo(alience_request_no);
				}
				break;
			case en_ally_mode_ally_is_made :	// ������ ���������.
				{
					MSG_ALLY_MODE_IS_MADE* alience_made = (MSG_ALLY_MODE_IS_MADE*)pPacket;
					MSGAlienceMade(alience_made);
				}
				break;
			case en_ally_mode_org_enter_ally :	// �ڽ��� �������� ���Ϳ� ���ԉ��.
				{
					MSG_ALLY_MODE_ORG_ENTER_ALLY* alience_enter= (MSG_ALLY_MODE_ORG_ENTER_ALLY*)pPacket;
					MSGAlienceEnter(alience_enter);
				}
				break;
			case en_ally_mode_ally_all_info :	//  ������ ��� ����.
				{
					MSG_ALLY_MODE_ALLY_ALL_INFO* alience_allinfo = (MSG_ALLY_MODE_ALLY_ALL_INFO*)pPacket;
					MSGAlienceAllInfo(alience_allinfo);
				}
				break;
			case en_ally_mode_notice_enter_new_org :	// ���ο� �������� ���Դ�.
				{
					MSG_ALLY_MODE_NOTICE_NEW_ORG* alience_newmember = (MSG_ALLY_MODE_NOTICE_NEW_ORG*)pPacket;
					MSGAlienceNewMember(alience_newmember);
				}
				break;
			case en_ally_mode_notice_leave_org :	// �������� Ż���ߴ�.
				{
					MSG_ALLY_MODE_NOTICE_LEAVE_ORG* alience_leavemember = (MSG_ALLY_MODE_NOTICE_LEAVE_ORG*)pPacket;
					MSGAlienceLeaveMember(alience_leavemember);
				}
				break;
			case en_ally_mode_scattered :	// ������ ��ü�Ǿ���.
				{
					MSG_ALLY_MODE_SCATTERED* alience_scattered = (MSG_ALLY_MODE_SCATTERED*)pPacket;
					MSGAlienceScattered(alience_scattered);
				}
				break;
			case en_ally_mode_allow_leave :	// Ż�� ��û ����
				{
					MSG_ALLY_MODE_ALLOW_LEAVE* alienceaccept_leave= (MSG_ALLY_MODE_ALLOW_LEAVE*)pPacket;
					MSGAlienceAcceptLeave(alienceaccept_leave);
				}
				break;
			case en_ally_mode_allow_scatter :	// �ػ� ��û ����.
				{
					MSG_ALLY_MODE_ALLOW_SCATTER* alienceaccept_scatter= (MSG_ALLY_MODE_ALLOW_SCATTER*)pPacket;
					MSGAlienceAcceptScatter(alienceaccept_scatter);
				}
				break;
			case en_ally_mode_banish_complete :	// �߹�Ǿ��ٰ�, �����ֿ��� �˷���.
				{
					MSG_ALLY_MODE_BANISH_COMPLETE* alience_leaveresult = (MSG_ALLY_MODE_BANISH_COMPLETE*)pPacket;
					MSGAlienceLeaveResult(alience_leaveresult);
				}
				break;
			case en_ally_mode_other_ally_info :	// �ٸ� ������ ����	
				{
					MSG_ALLY_MODE_OTHER_ALLY_INFO * alience_otherinfor = (MSG_ALLY_MODE_OTHER_ALLY_INFO *)pPacket;
					MSGAlienceOtherInfo(alience_otherinfor);
				}
				break;
			case en_ally_mode_notice_banish_org :	// �������� �߹�Ǿ���.
				{
					MSG_ALLY_MODE_NOTICE_BANISH_ORG* alience_noticeleave= (MSG_ALLY_MODE_NOTICE_BANISH_ORG*)pPacket;
					MSGAlienceNoticeLeave(alience_noticeleave);
				}
				break;
			case en_ally_mode_no_ally_info :	// �������� �߹�Ǿ���.
				{
					MSG_ALLY_MODE_NO_ALLY_INFO* alience_noinfo = (MSG_ALLY_MODE_NO_ALLY_INFO*)pPacket;
					MSGAlienceNoInfo(alience_noinfo);
				}
				break;
				
				
			case en_ally_mode_error_we_already_ally :		// �̹� ������ �ΰ� �ִ�.
			case en_ally_mode_error_they_already_ally :		// ��� �������� �̹� ������ �ΰ� �ִ�.
			case en_ally_mode_error_cannot_ally :			// ��/�鵵 �� ������ ������ ���� �� ����.
			case en_ally_mode_error_not_found_info :		// ��û�� ������ ã�� �� �����ϴ�.
			case en_ally_mode_error_cannot_find_user :		// ������ ã�� �� ����.
			case en_ally_mode_error_i_am_busy :				// ���� ��û��..
			case en_ally_mode_error_busy :					// ����� �ٸ� �� ��.
			case en_ally_mode_error_cannot_request :		// �������� ���ų�, ���ְ� �ƴϴ�.
			case en_ally_mode_error_not_hero :				// ����� ������ ���ְ� �ƴϴ�.
				{
					MSG_ALLY_ERROR* alience_error = (MSG_ALLY_ERROR*)pPacket;
					MSGAlienceErrorMessage(alience_error);
				}
				break;
			}
		}
		break;
#endif

		// �����
#ifdef _XDEF_CASTLEBATTLE
	case MSG_NO_CB_SCHEDULE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : CB Schedule");
						
			MSG_CB_SCHEDULE* cbschedule = (MSG_CB_SCHEDULE*)pPacket;
			switch(cbschedule->ucMode)
			{
			case en_cb_owner_schedule_list :
				{
					MSG_CB_OWNER_SCHEDULE_LIST* schedule = (MSG_CB_OWNER_SCHEDULE_LIST*)pPacket;
					MSGCBOnwerScheduleList(schedule);
				}
				break;
			case en_cb_schedule_list :
				{
					MSG_CB_SCHEDULE_LIST* schedule = (MSG_CB_SCHEDULE_LIST*)pPacket;
					MSGCBScheduleList(schedule);
				}
				break;
			case en_cb_challenge_list :
				{
					MSG_CB_CHALLENGE_LIST* schedule = (MSG_CB_CHALLENGE_LIST*)pPacket;
					MSGCBChallengeList(schedule);
				}
				break;
			case en_cb_change_time_success :
				{
					MSG_CB_CHANGE_TIME_SUCCESS* schedule = (MSG_CB_CHANGE_TIME_SUCCESS*)pPacket;
					MSGCBChangeTimeSuccess(schedule);
				}
				break;
			case en_cb_change_time_fail :
				{
					MSG_CB_CHANGE_TIME_FAIL* schedule = (MSG_CB_CHANGE_TIME_FAIL*)pPacket;
					MSGCBChangeTimeFail(schedule);
				}
				break;
			case en_cb_challenge_success :
				{
					MSG_CB_CHALLENGE_SUCCESS* schedule = (MSG_CB_CHALLENGE_SUCCESS*)pPacket;
					MSGCBChallengeSuccess(schedule);
				}
				break;
			case en_cb_challenge_fail :
				{
					MSG_CB_CHALLENGE_FAIL* schedule = (MSG_CB_CHALLENGE_FAIL*)pPacket;
					MSGCBChallengeFail(schedule);
				}
				break;
			case en_cb_u_are_owner :
				{
					MSG_CB_U_ARE_OWNER* schedule = (MSG_CB_U_ARE_OWNER*)pPacket;
					MSGCBUAreOwner(schedule);
				}
				break;
			case en_cb_u_are_not_owner :
				{
					MSG_CB_U_ARE_NOT_OWNER* schedule = (MSG_CB_U_ARE_NOT_OWNER*)pPacket;
					MSGCBUAreNotOwner(schedule);
				}
				break;
			case en_cb_not_owner :
				{
					MSG_CB_NOT_OWNER* schedule = (MSG_CB_NOT_OWNER*)pPacket;
					MSGCBNotOwner(schedule);
				}
				break;
			case en_cb_no_such_castle :
				{
					MSG_CB_NO_SUCH_CASTLE* schedule = (MSG_CB_NO_SUCH_CASTLE*)pPacket;
					MSGCBNoSuchCastle(schedule);
				}
				break;
	#ifdef _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI
			case en_cb_cur_owner_info :
				{
					MSG_CB_CUR_OWNER_INFO* info = (MSG_CB_CUR_OWNER_INFO*)pPacket;
					MSGCBCurOwnerInfo(info);
				}
				break;
	#endif
			case en_cb_map_tax_rate :
				{
					MSG_CB_NO_MAP_TAX_RATE* schedule = (MSG_CB_NO_MAP_TAX_RATE*)pPacket;

					g_nMapTaxRate = schedule->ucMapRate;
					_XDWINPRINT("[CB/Network] Map Tax Rate : %d", g_nMapTaxRate);
				}	
				break;
			case en_cb_set_total_tax :
				{
					MSG_CB_TAX_SET* schedule = (MSG_CB_TAX_SET*)pPacket;
					MSGCBTaxSet(schedule);
				}
				break;
			case en_cb_tax_answer_take :
				{
					MSG_CB_TAX_ANSWER_TAKE* schedule = (MSG_CB_TAX_ANSWER_TAKE*)pPacket;
					MSGCBTaxAnswerTake(schedule);
				}
				break;
			case en_cb_schedule_error :
				{
					MSG_CB_SCHEDULE_ERROR* schedule = (MSG_CB_SCHEDULE_ERROR*)pPacket;
					MSGCBScheduleError(schedule);
				}
				break;
			}
		}
		break;
	case MSG_NO_COMBAT :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Combat");
						
			MSG_COMBAT* combat = (MSG_COMBAT*)pPacket;
			switch(combat->ucMode)
			{
			case en_cb_prog_ready_event :
				{
					MSG_CB_PROG_READY_EVENT* cevent = (MSG_CB_PROG_READY_EVENT*)pPacket;
					MSGCBProgReadyEvent(cevent);
				}
				break;
			case en_cb_prog_start_event :
				{
					MSG_CB_PROG_START_EVENT* cevent = (MSG_CB_PROG_START_EVENT*)pPacket;
					MSGCBProgStartEvent(cevent);
				}
				break;
			case en_cb_prog_end_event :
				{
					MSG_CB_PROG_END_EVENT* cevent = (MSG_CB_PROG_END_EVENT*)pPacket;
					MSGCBProgEndEvent(cevent);
				}
				break;
			case en_cb_threat_start_ok :
				{
					MSG_CB_THREAT_START_OK* threat = (MSG_CB_THREAT_START_OK*)pPacket;
					MSGCBThreatStartOK(threat);
				}
				break;
			case en_cb_threat_start_error :
				{
					MSG_CB_THREAT_START_ERROR* threat = (MSG_CB_THREAT_START_ERROR*)pPacket;
					MSGCBThreatStartError(threat);
				}
				break;
			case en_cb_threat_start_notice :
				{
					MSG_CB_THREAT_START_NOTICE* threat = (MSG_CB_THREAT_START_NOTICE*)pPacket;
					MSGCBThreatStartNotice(threat);
				}
				break;
			case en_cb_threat_cancel_ok :
				{
					MSG_CB_THREAT_CANCEL_OK* threat = (MSG_CB_THREAT_CANCEL_OK*)pPacket;
					MSGCBThreatCancelOK(threat);
				}
				break;
			case en_cb_threat_complete_ok :
				{
					MSG_CB_THREAT_COMPLETE_OK* threat = (MSG_CB_THREAT_COMPLETE_OK*)pPacket;
					MSGCBThreatCompleteOK(threat);
				}
				break;
			case en_cb_threat_complete_error :
				{
					MSG_CB_THREAT_COMPLETE_ERROR* threat = (MSG_CB_THREAT_COMPLETE_ERROR*)pPacket;
					MSGCBThreatCompleteError(threat);
				}
				break;
			case en_cb_threat_complete_notice :
				{
					MSG_CB_THREAT_COMPLETE_NOTICE* threat = (MSG_CB_THREAT_COMPLETE_NOTICE*)pPacket;
					MSGCBThreatCompleteNotice(threat);
				}
				break;
			case en_cb_info_current_info :
				{
					MSG_CB_INFO_CURRENT_INFO* info = (MSG_CB_INFO_CURRENT_INFO*)pPacket;
					MSGCBInfoCurrentInfo(info);
				}
				break;
			case en_cb_etc_loud_chat :
				{
					MSG_CB_ETC_LOUD_CHAT* chat = (MSG_CB_ETC_LOUD_CHAT*)pPacket;
					MSGCBEtcLoudChat(chat);
				}
				break;
			case en_cb_etc_notice :
				{
					MSG_CB_ETC_NOTICE* notice = (MSG_CB_ETC_NOTICE*)pPacket;
					MSGCBEtcNotice(notice);
				}
				break;
			case en_cb_etc_cannot_loud :
				{
					MSG_CB_ETC_CANNOT_LOUD* chat = (MSG_CB_ETC_CANNOT_LOUD*)pPacket;
					MSGCBEtcCannotLoud(chat);
				}
				break;
			case en_cb_dead_penalty :
				{
					MSG_CB_DEAD_PENALTY* dead = (MSG_CB_DEAD_PENALTY*)pPacket;
					MSGCBDeadPenalty(dead);
				}
				break;
			case en_cb_threatpoint_set :
				{
					MSG_CB_THREATPOINT_SET* point = (MSG_CB_THREATPOINT_SET*)pPacket;
					MSGCBThreatPointSet(point);
				}
				break;
#ifdef _XDEF_CASTLEBATTLE_RECORD
			case en_cb_record_pkt_info_res :
				{
					MSG_CB_RECORD_INFO_RES* res = (MSG_CB_RECORD_INFO_RES*)pPacket;
					MSGCBRecordInfoRes(res);
				}
				break;
			case en_cb_record_pkt_add_combat :
				{
					MSG_CB_RECORD_COMBAT_ADD* record = (MSG_CB_RECORD_COMBAT_ADD*)pPacket;
					MSGCBRecordCombatAdd(record);
				}
				break;
			case en_cb_record_pkt_pvp_award :	// 83
				{
					MSG_CB_RECORD_PVP_AWARD* award = (MSG_CB_RECORD_PVP_AWARD*)pPacket;
					MSGCBRecordPvpAward(award);
				}
				break;
			case en_cb_record_pkt_manor_rank :	// 84
				{
					MSG_CB_RECORD_RANK* rank = (MSG_CB_RECORD_RANK*)pPacket;
					MSGCBRecordRank(rank);
				}
				break;
			case en_cb_record_pkt_winner_award :	// 85
				{
					MSG_CB_RECORD_WINNER_AWARD* winner = (MSG_CB_RECORD_WINNER_AWARD*)pPacket;
					MSGCBRecordWinnerAward(winner);
				}
				break;
#endif
			}
		}
		break;
#endif

//Author : ����� 
#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
	case MSG_NO_GGAUTH_ACK:
		{
			MSG_GGAUTH_ACK* ggauthdata = NULL;
			ggauthdata = (MSG_GGAUTH_ACK*)pPacket;
#ifdef _USE_NPROTECTLOG
			_XLog( "RECEIVE PACKET : NPROTECT_GAME_GUARD  %08X %08X %08X %08X ", ggauthdata->data.dwIndex, ggauthdata->data.dwValue1, ggauthdata->data.dwValue2, ggauthdata->data.dwValue3);
#endif
			
#ifdef _XDEF_NPROTECT_GUARD_TIME_LOG
			//���� �ð��� ���´�
			SYSTEMTIME TimeInfo;
			GetLocalTime(&TimeInfo);

			_XLog( "RECIEVE_PACKET_TIME : [%d:%d:%d]", TimeInfo.wHour, TimeInfo.wMinute, TimeInfo.wSecond );
#endif

			DWORD dMsg = g_GameGuardInstance.Auth2(&ggauthdata->data);

#ifdef _USE_NPROTECTLOG
			_XLog( "RECEIVE PACKET_NPROTECT_GAME_GUARD" );
#endif			
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_GameGuardAuthenticationCount++;

			if( ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_GameGuardAuthenticationCount >= 2 )
				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_CompleteGameGuardAuth = TRUE;
		}
		break;

#endif
//Last updated: 07/02/02

	case MSG_NO_COMMON_NTF :
		{
			MSG_COMMON* pCommon = NULL;
			pCommon = (MSG_COMMON*)pPacket;
			
			switch(pCommon->wMode) 
			{
			case 0:
			case 1:
			case 2:
				{
//Author : �����
					MSG_COMMON_NTF* pCommonNTF = NULL;
					pCommonNTF = (MSG_COMMON_NTF*)pPacket;
					PcRoomStateResult( *pCommonNTF );
				}
				break;
#ifdef _XDEF_COMMONITEMMALL_20070402
			case 11://_CIS_CASH_INVEN_ACK
				{
					//ĳ�� �ڽ��� ������ ������ �˷��ش�.
					MSGCISInvenAck(pCommon);
				}
				break;
			case 16://_CIS_MOVE_ITEM_ACK
				{
					//ĳ�� �ڽ����� �೶���� �������� �̵��Ѵ�.
					MSGCISMoveItemAck(pCommon);
				}
				break;
			case 21://_CIS_PRESENT_MSG_ACK
				{
					//���� �޼����� ǥ���Ѵ�.
					MSGCISPresentMsg(pCommon);
				}
				break;
#endif 	
			}
		}
		break;
//#endif
//Last Updated :07/03/12
		
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK����ġ����, ������ PK����
	case MSG_NO_LTS :
		{			
			MSG_LTS* pktracing = (MSG_LTS*)pPacket;
			
			switch(pktracing->ucPktMode) {
			case en_lts_lvpk_info_gs_cl://������ �������
				{
					MSG_LTS_LVPK_INFO_GS_CL* pklevel = (MSG_LTS_LVPK_INFO_GS_CL*)pPacket;
					MSGPKLevelPenaltyInfo(pklevel);
				}
				break;
			case en_lts_lvpk_insert_gs_cl://������ �г�Ƽ ������ ����Ʈ �߰�
				{
					MSG_LTS_LVPK_INSERT_GS_CL* pklevel = (MSG_LTS_LVPK_INSERT_GS_CL*)pPacket;		
					MSGPKLevelPenaltyInsert(pklevel);				
				}
				break;
			case en_lts_lvpk_del_req_gs_cl://������ �г�Ƽ ������ ����Ʈ ����
				{
					MSG_LTS_LVPK_DEL_REQ_GS_CL* pklevel = (MSG_LTS_LVPK_DEL_REQ_GS_CL*)pPacket;		
					MSGPKLevelPenaltyDelete(pklevel);				
				}
				break;
				
			case en_lts_pk_info_gs_cl://PK�� �������
				{
					MSG_LTS_PK_INFO_GS_CL* pktracinginfo = (MSG_LTS_PK_INFO_GS_CL*)pPacket;		
					MSGPKTracingInfo(pktracinginfo);						
				}
				break;
			case en_lts_pk_insert_req_gs_cl :	// pk�� �̸�
				{
					MSG_LTS_PK_INSERT_REQ_GS_CL*  pkname = (MSG_LTS_PK_INSERT_REQ_GS_CL*)pPacket;
					MSGPKTargetName(pkname);
				}
				break;
			case en_lts_pk_insert_success_gs_cl://PK�� ����Ʈ �߰� ����
				{
					MSG_LTS_PK_INSERT_SUCCESS_GS_CL* pktracinginfo = (MSG_LTS_PK_INSERT_SUCCESS_GS_CL*)pPacket;
					MSGPKTracingInsertSuccess(pktracinginfo);	
				}
				break;
			case en_lts_pk_insert_failed_gs_cl://PK�� ����Ʈ �߰� ����
				{
					MSG_LTS_PK_INSERT_FAILED_GS_CL* pktracinginfo = (MSG_LTS_PK_INSERT_FAILED_GS_CL*)pPacket;
					MSGPKTracingInsertFail(pktracinginfo);		
				}
				break;
			case en_lts_pk_del_success_gs_cl://PK�� ����Ʈ ���� ����
				{
					MSG_LTS_PK_DEL_SUCCESS_GS_CL* pktracinginfo = (MSG_LTS_PK_DEL_SUCCESS_GS_CL*)pPacket;
					MSGPKTracingDeleteSuccess(pktracinginfo);		
				}
				break;
			case en_lts_sp_info_gs_cl://Ư���� �������
				{
					MSG_LTS_SP_INFO_GS_CL* tracinginfo = (MSG_LTS_SP_INFO_GS_CL*)pPacket;
					MSGSomeOneTracingInfo(tracinginfo);					
				}
				break;
			case en_lts_sp_insert_success_gs_cl://Ư���� ����Ʈ �߰� ����
				{
					MSG_LTS_SP_INSERT_SUCCESS_GS_CL* tracinginfo = (MSG_LTS_SP_INSERT_SUCCESS_GS_CL*)pPacket;
					MSGSomeOneTracingInsertSuccess(tracinginfo);
				}
				break;
			case en_lts_sp_insert_failed_gs_cl://Ư���� ����Ʈ �߰� ����
				{
					MSG_LTS_SP_INSERT_FAILED_GS_CL* tracinginfo = (MSG_LTS_SP_INSERT_FAILED_GS_CL*)pPacket;
					MSGSomeOneTracingInsertFail(tracinginfo);	
				}
				break;
			case en_lts_sp_del_success_gs_cl://Ư���� ����Ʈ ���� ����
				{
					MSG_LTS_SP_DEL_SUCCESS_GS_CL* tracinginfo = (MSG_LTS_SP_DEL_SUCCESS_GS_CL*)pPacket;
					MSGSomeOneTracingDeleteSuccess(tracinginfo);	
				}
				break;
			case en_lts_pos_info_gs_cl://3�ʸ��� PK�� Ư���� (ĳ���� + ��ġ����)
				{
					MSG_LTS_POS_INFO_GS_CL* tracinginfo = (MSG_LTS_POS_INFO_GS_CL*)pPacket;
					MSGTracingPositionInfo(tracinginfo);				
				}
				break;
			}
		}
		break;
#endif
	//Author : �����  //breif : ģ����ȯ
	case MSG_NO_RECALL_FRIEND :
		{
#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER
			MSG_RECALL_FRIEND* recallfriendresult = (MSG_RECALL_FRIEND*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : Recall Friend");

			MSGRecallItemResult( recallfriendresult );
#endif
		}
		break;
	// Ư�� ���� �˸�
	case MSG_NO_NOTIEFY_INFO :
		{
			MSG_NOTIFY_INFO* ni = (MSG_NOTIFY_INFO*)pPacket;

			VIEWPACKETLOG("RECEIVE MESSAGE : notify info");

			switch(ni->notify_type)
			{
			case en_notify_type_item_use :
				{
					MSG_NI_ITEM_USE* itemuse = (MSG_NI_ITEM_USE*)pPacket;
					MSGNiItemUse(itemuse);
				}
				
				break;
			}
		}
		break;
#ifdef _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI
	case MSG_NO_BW_WAR :
		{
			MSG_BW_WAR* bwwar = (MSG_BW_WAR*)pPacket;

			VIEWPACKETLOG("RECEIVE MESSAGE : bw war");

			switch(bwwar->ucBWMode)
			{
			case en_bwwar_left_time :
				{
					MSG_BW_LEFT_TIME* lefttime = (MSG_BW_LEFT_TIME*)pPacket;
					MSGBWLeftTime(lefttime);
				}
				break;
			case en_bwwar_enter_gate_success :
			case en_bwwar_enter_gate_fail :
				{
					MSG_BW_ENTER_GATE_SUCCESS* entergate = (MSG_BW_ENTER_GATE_SUCCESS*)pPacket;
					MSGBWEnterGateResult(entergate);
				}
				break;
			}
		}
		break;
#endif

#ifdef _XUSEAPEXMODULE
	case MSG_NO_SECURITY :
		{
			MSG_SECURITY* pSecurity = (MSG_SECURITY*)pPacket;
			VIEWPACKETLOG("RECEIVE MESSAGE : security");
			
			if( pSecurity->ucMode == en_apex_client_login )
			{
				_XSetDefaultPath();

				long re= CHCStart(_ApexNetSendToGameServer,g_pfRec);
				VIEWPACKETLOG("Start Apex,re=%d\n",re);

				g_ApexInitialized = TRUE;

				#ifdef _XAPEXMODULE_LOG
					_XLog( "Start Apex,re=%d\n",re );
				#endif

				if( !g_NetworkKernel.SendPacket( MSG_NO_SECURITY, en_apex_client_login_result, re ) ) 
					return -1;

				#ifdef _XAPEXMODULE_LOG
					_XLog( "Send Apex st result,re=%d\n",re );
				#endif
			}
			else if( pSecurity->ucMode == en_apex_req_client_data )
			{
				MSG_APEX_REQ_CLIENT_DATA* pSecurityRequest = (MSG_APEX_REQ_CLIENT_DATA*)pPacket;
				VIEWPACKETLOG("RECEIVE MESSAGE : security request");
							
				_ApexNotice_UserData( pSecurityRequest->data, pSecurityRequest->data_length );

				#ifdef _XAPEXMODULE_LOG
					_XLog( "Apex request, data length = %d", pSecurityRequest->data_length );
				#endif
			}
		}
		break;
#endif
	
	case MSG_NO_MASTER_AND_PUPIL : //����
		{
		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSAGE : MSG_NO_MASTER_AND_PUPIL");

			MSGMasternDiscipleResult((MSG_MASTER_AND_PUPIL*)pPacket);
		#endif
		}
		break;
		
	default :
		_XLog("WARNING : Unknown packet [%d]", pPacket[2] );
		break;
	}

	return TRUE;
}

#define _XXCOMMENT / ## / 
#ifdef _XDWDEBUG
#define _XDWSENDPACKETLOG(_str_) _XDebugOutputString( _str_, 1, 1 );
#else
#define _XDWSENDPACKETLOG		_XXCOMMENT
#endif

BOOL _XNetwork::_XSendPacket_Encapsulate( char* pPacket )
{
	int currentsequancenumber = g_PacketCrypto.GetSeqNum();
	if( currentsequancenumber == 0 )
	{
		_sHeader* pHeader = (_sHeader*)pPacket;

#ifdef _XDWDEBUG
		_XLog( "Uninitialized socket pipeline : 2308 [%d:%d:%d]", pHeader->sCrypto, pHeader->sLength, pPacket[2] );
#else
		_XLog( "Uninitialized socket pipeline : 2308 [%d:%d:%d]", pHeader->sCrypto, pHeader->sLength, pPacket[2] );
#endif
		return FALSE;
	}

	//LARGE_INTEGER frequency, count1, count2;
	//QueryPerformanceFrequency(&frequency);
	//QueryPerformanceCounter(&count1);
	
#ifdef _XUSEENCRYPTPATTERN

	extern bool _XGetEncryptPatternData( int patternindex, unsigned char sequancepos );
	extern unsigned char g_SelectedEncryptSequance;
	
	bool encypt = _XGetEncryptPatternData( g_SelectedEncryptSequance, currentsequancenumber );

	_sHeader* pHeader = (_sHeader*)pPacket;
	pHeader->sCrypto = encypt;

#endif


	_Encapsule_info	cryptoinfo = g_PacketCrypto.Encapsulate( pPacket );
	
	if( !cryptoinfo.buf )
	{
		_XFatalError( "Send error : 2309" );
		return FALSE;
	}

	//QueryPerformanceCounter(&count2);	 
	//double timeinmicroseconds = (double)( (double)((count2.QuadPart - count1.QuadPart) * 1000 * 1000) / (double)frequency.QuadPart );
	//_XLog( "%f second", timeinmicroseconds/1000000 );

#ifdef _XUSEENCRYPTPATTERN
	//BYTE packetnum = *(pPacket + sizeof(_sHeader));
	//_XLog( "Send Packet : stseq = %d, seq = %d, pn : %d, size = %d", g_SelectedEncryptSequance, currentsequancenumber, packetnum, cryptoinfo.length );		
#endif
	
	int ret = send( m_hClientSocket, (char*)cryptoinfo.buf, cryptoinfo.length, 0 );
	if( ret == SOCKET_ERROR ) 
	{
		ret = WSAGetLastError();

		if( ret == WSAEWOULDBLOCK )
		{
			_XLog( "WARNING : MG SP : TCP I/O blocking");
			Sleep( 50 );
		}
		else
		{
			_XLog( "Send error [%d]", ret );
			ProcessNetworkError();
		}
		return FALSE;
	}

	return TRUE;
}

BOOL _XNetwork::SendPacket( int type, int param1, int param2, int param3, int param4 ) 
{	
	if( !m_bConnected ) return FALSE;

#ifdef _XDWDEBUG
//	_XLog("Send packet : %d", type );
#endif

#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode ) return TRUE;
#endif
	switch ( type )
	{

	// ================================================================================================================
	// ���� ����/���� �� ĳ���� ���� ����
	case MSG_NO_SV_ENTER_GAME :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : Entering game server" );

			int size = sizeof( MSG_SV_ENTER_GAME );
			MSG_SV_ENTER_GAME	serverentergame;
			ZeroMemory ( &serverentergame, size );

			serverentergame.header.sLength = size;
			serverentergame.ucMessage = MSG_NO_SV_ENTER_GAME;
		
			strcpy( serverentergame.ID,	 g_pLocalUser->m_UserName );
			serverentergame.usUniqueID = g_pLocalUser->m_Uniqueid;

			return _XSendPacket_Encapsulate( (char*)&serverentergame );			
		}
		break;
	case MSG_NO_LOGOUT :
		{
			if( !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer ) // ������ �̵��߿��� ������ �ʴ´�.
			{			
				VIEWPACKETLOG
					_XDWPACKETLOG( "SEND MESSAGE : logout current user" );

				int size = sizeof( MSG_LOGOUT );
				
				MSG_LOGOUT logout;
				ZeroMemory ( &logout, size );			
				logout.header.sLength = size;
				logout.ucMessage = MSG_NO_LOGOUT;
				logout.usUniqueID = g_pLocalUser->m_Uniqueid;
				
				return _XSendPacket_Encapsulate( (char*)&logout );
			}
		}
		break;
	case MSG_NO_CHARAC_REQ_LIST :
		{
			if( !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MovingGameServer )
			{
				VIEWPACKETLOG
					_XDWPACKETLOG( "SEND MESSAGE : Character list request" );

				int size = sizeof( MSG_CHARAC_REQ_LIST );
				MSG_CHARAC_REQ_LIST	characterlistrequest;
				ZeroMemory ( &characterlistrequest, size );			
				characterlistrequest.header.sLength = size;
				characterlistrequest.ucMessage = MSG_NO_CHARAC_REQ_LIST;			
			
				return _XSendPacket_Encapsulate( (char*)&characterlistrequest );
			}
		}
		break;	
	case MSG_NO_CHARAC_SELECT :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : Select character" );

			int size = sizeof( MSG_CHARAC_SELECT );
			MSG_CHARAC_SELECT	selectcharacter;
			ZeroMemory ( &selectcharacter, size );
			selectcharacter.header.sLength = size;
			selectcharacter.ucMessage = MSG_NO_CHARAC_SELECT;
			strcpy( selectcharacter.cSelectName, g_pLocalUser->m_CharacterInfo.charactername );
			
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_CANCEL, _XGETINTERFACETEXT(ID_STRING_NETWORK_CONNECTSERVER), 
									 _XGETINTERFACETEXT(ID_STRING_NETWORK_CONNECTING), TRUE, 
									 _XDEF_CS_CANCELBUTTON, _XDEF_CS_CANCELBUTTON, _XDEF_CS_CANCELBUTTON );

			return _XSendPacket_Encapsulate( (char*)&selectcharacter );			
		}
		break;
	case MSG_NO_CHARAC_GAMESTARTSIGNAL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : game start signal" );
			
			// Local User�� ����ȿ�� visual effect ����
			if(g_pLocalUser->m_ModelDescriptor.m_EffectManager)
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->Dispose();
			
			int size = sizeof( MSG_CHARAC_GAMESTARTSIGNAL );
			MSG_CHARAC_GAMESTARTSIGNAL	gamestartsignal;
			ZeroMemory ( &gamestartsignal, size );
			gamestartsignal.header.sLength = size;
			gamestartsignal.ucMessage = MSG_NO_CHARAC_GAMESTARTSIGNAL;
			gamestartsignal.usUniqueID = g_pLocalUser->m_Uniqueid;

			_XLog("Start game...");
									
			return _XSendPacket_Encapsulate( (char*)&gamestartsignal );
		}
		break;
	case MSG_NO_CHARAC_RETURN_LOBBY :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : return to lobby" );

			int size = sizeof( MSG_CHARAC_RETURN_LOBBY );
			MSG_CHARAC_RETURN_LOBBY	returntolobby;
			ZeroMemory ( &returntolobby, size );
			returntolobby.header.sLength = size;
			returntolobby.ucMessage = MSG_NO_CHARAC_RETURN_LOBBY;
			returntolobby.usUniqueID = g_pLocalUser->m_Uniqueid;
			
			return _XSendPacket_Encapsulate( (char*)&returntolobby );
		}
		break;
	case MSG_NO_CHARAC_CREATE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : create character" );
			
			XProc_CreateCharacter*	proc_createcharacter = 
				&((CEmperorOfDragonsApp*)gApp)->m_proc_createcharacter;

			int size = sizeof( MSG_CHARAC_CREATE );
			MSG_CHARAC_CREATE	createcharacter;
			ZeroMemory ( &createcharacter, size );
			createcharacter.header.sLength = size;
			createcharacter.ucMessage = MSG_NO_CHARAC_CREATE;

			strcpy( createcharacter.cName, proc_createcharacter->m_CharacterNameString );
						
			createcharacter.cStr			=	proc_createcharacter->m_Parameter[0];
			createcharacter.cZen			=	proc_createcharacter->m_Parameter[1];			
			createcharacter.cIntelligence	=	proc_createcharacter->m_Parameter[2];
			createcharacter.cConstitution	=	proc_createcharacter->m_Parameter[3];
			createcharacter.cDexterity		=	proc_createcharacter->m_Parameter[4];

			createcharacter.cSex			=   proc_createcharacter->m_CharacterGender;
			createcharacter.cFace			=	proc_createcharacter->m_PreviewModelDescript.face;
			createcharacter.cHair			=	proc_createcharacter->m_PreviewModelDescript.hair;			
			
			createcharacter.cInitZone		=	proc_createcharacter->m_SelectedZone;

			if( proc_createcharacter->m_CharacterGender == _XGENDER_MAN )
			{
				createcharacter.ucClothSet = proc_createcharacter->m_ClothTypeManIndex;
			}
			else
			{
				createcharacter.ucClothSet = proc_createcharacter->m_ClothTypeWomanIndex;
			}
			
			return _XSendPacket_Encapsulate( (char*)&createcharacter );
		}
		break;
	case MSG_NO_CHARAC_DELETE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : delete character" );
			
			XProc_CreateCharacter*	proc_createcharacter = 
				&((CEmperorOfDragonsApp*)gApp)->m_proc_createcharacter;
			
			int size = sizeof( MSG_CHARAC_DELETE );
			MSG_CHARAC_DELETE	deletecharacter;
			ZeroMemory ( &deletecharacter, size );
			deletecharacter.header.sLength = size;
			deletecharacter.ucMessage = MSG_NO_CHARAC_DELETE;
			strcpy( deletecharacter.cName, g_pLocalUser->m_CharacterInfo.charactername );
			

			#ifdef _ACCLAIM_VERSION // acclaim version�� ��� Account DB Password ������ ����.
			if( strcmp( g_pLocalUser->m_UserPW, g_MessageBox.m_IMEControl.GetText() ) != 0 )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_CANNOTDELETE, TRUE, _XDEF_CS_CANCELDELETEBUTTON );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				_XLog( "ERRORLOG : Delete character : Invalid password");
				break;
			}

			#endif

			int maxsize = 7;
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || 
				g_LanguageType == _XLANGUAGE_TYPE_ENGLISH    || 
				g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE  ||
				g_LanguageType == _XLANGUAGE_TYPE_JAPANESE   ||
				g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN    )
			{
				maxsize = 12;
			}
			
			if( strlen(g_MessageBox.m_IMEControl.GetText()) > (size_t)maxsize )
			{
				strncpy( deletecharacter.cPassword, g_MessageBox.m_IMEControl.GetText(), maxsize );
			}
			else
			{
				strcpy( deletecharacter.cPassword, g_MessageBox.m_IMEControl.GetText() );
			}

#ifdef _XDEF_SELECT_ANOTHERCHARACTER_080721_KUKURI
			m_bDeleteCharacter = TRUE;
#endif

			return _XSendPacket_Encapsulate( (char*)&deletecharacter );
		}
		break;
	// ================================================================================================================
	// ���� ���� ����
	case MSG_NO_ZONE_RESTART :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "--Send request zone restart ");

			int size = sizeof( MSG_ZONE_RESTART );
			MSG_ZONE_RESTART requestzonerestart;
			ZeroMemory ( &requestzonerestart, size );
			requestzonerestart.header.sLength = size;
			requestzonerestart.ucMessage = MSG_NO_ZONE_RESTART;
						
			return _XSendPacket_Encapsulate( (char*)&requestzonerestart );
		}		
		break;
	case MSG_NO_SV_MOVE_REQ :	// ���� �̵�.
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : move zone" );

			int size = sizeof( MSG_SV_MOVE_REQ );
			MSG_SV_MOVE_REQ movezone;
			ZeroMemory ( &movezone, size );
			movezone.header.sLength = size;
			movezone.ucMessage = MSG_NO_SV_MOVE_REQ;

			movezone.bGate_NPC = false;
			movezone.iNo	   = ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_CurrentWarpZoneIndex;
			movezone.cGotoZone = param1;
			
			return _XSendPacket_Encapsulate( (char*)&movezone );
		}
		break;
	case MSG_NO_SV_ENTER_GSERVER :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : Entering game server[server vs server]" );

			int size = sizeof( MSG_SV_ENTER_GSERVER );
			MSG_SV_ENTER_GSERVER	entergameserver;
			ZeroMemory ( &entergameserver, size );
			
			entergameserver.header.sLength = size;
			entergameserver.ucMessage = MSG_NO_SV_ENTER_GSERVER;
			
			strcpy( entergameserver.ID,	 g_pLocalUser->m_UserName );
			entergameserver.usUniqueID = g_pLocalUser->m_Uniqueid;
			
			return _XSendPacket_Encapsulate( (char*)&entergameserver );
		}
		break;
	case MSG_NO_ZONE_MOVE :
		{	
			const int _xdef_sendmovepacket_maximumintervaltime = 166;

			VIEWPACKETLOG
				_XDWINPRINT("MOVE Packet Delay Time :%d", g_LocalSystemTime - m_MovePacket_lastSendedTime);

#ifdef _XDWDEBUG
			TCHAR _messagestring[64];
			memset(_messagestring, 0, sizeof(_messagestring));
			_snprintf( _messagestring, sizeof(_messagestring), "Packet send time : %d : %d", m_MovePacket_lastSendedTime, g_LocalSystemTime - m_MovePacket_lastSendedTime );
			_XDWSENDPACKETLOG( _messagestring );
#endif			

			m_MovePacket_lastSendedTime = g_LocalSystemTime;
			g_pLocalUser->m_PrevPacketSendedTime = g_LocalSystemTime;
			
			CEmperorOfDragonsApp* pApp = ((CEmperorOfDragonsApp*)gApp);
			_XProcess* pCurrentProcess = pApp->m_ProcessManager.GetCurrentProcess();
		
			if( pCurrentProcess == &(pApp->m_proc_maingame) || pCurrentProcess == &(pApp->m_proc_flash) )
			{

				if( !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LockMoveProcess )
				{					
					int size = sizeof( MSG_ZONE_MOVE );
					MSG_ZONE_MOVE charactermove;
					ZeroMemory ( &charactermove, size );
					charactermove.header.sLength = size;
					charactermove.ucMessage = MSG_NO_ZONE_MOVE;
					charactermove.usUniqueID = g_pLocalUser->m_Uniqueid;

					charactermove.fX = g_pLocalUser->m_Position.x;
					charactermove.fZ = g_pLocalUser->m_Position.z;
					charactermove.fgotoX = g_pLocalUser->m_TargetPosition.x;
					charactermove.fgotoZ = g_pLocalUser->m_TargetPosition.z;
					charactermove.sStandingOBB = 0;
					charactermove.sCharacIndex = 0;
					charactermove.sAnimationNumber = g_pLocalUser->m_ModelDescriptor.GetCurMotion();

					VIEWPACKETLOG
						_XDWPACKETLOG( "SEND MESSAGE : move character [Animation:%d]", g_pLocalUser->m_ModelDescriptor.GetCurMotion() );
					
					return _XSendPacket_Encapsulate( (char*)&charactermove );					
				}
			}
			else if( pCurrentProcess == &(pApp->m_proc_loading) )
			{
				if( !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LockMoveProcess )
				{					
					int size = sizeof( MSG_ZONE_MOVE );
					MSG_ZONE_MOVE charactermove;
					ZeroMemory ( &charactermove, size );
					charactermove.header.sLength = size;
					charactermove.ucMessage = MSG_NO_ZONE_MOVE;
					charactermove.usUniqueID = g_pLocalUser->m_Uniqueid;
					
					charactermove.fX = g_pLocalUser->m_NextRespawnPosition.x;
					charactermove.fZ = g_pLocalUser->m_NextRespawnPosition.z;
					charactermove.fgotoX = g_pLocalUser->m_NextRespawnPosition.x;
					charactermove.fgotoZ = g_pLocalUser->m_NextRespawnPosition.z;
					charactermove.sStandingOBB = 0;
					charactermove.sCharacIndex = 0;
					charactermove.sAnimationNumber = g_pLocalUser->m_ModelDescriptor.GetCurMotion();
					
					VIEWPACKETLOG
						_XDWPACKETLOG( "SEND MESSAGE : move character [Animation:%d]", g_pLocalUser->m_ModelDescriptor.GetCurMotion() );
					
					return _XSendPacket_Encapsulate( (char*)&charactermove );
				}				
			}
		}
		break;
	
	case MSG_NO_ZONE_REQ_CHARAC_INFO :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : request anothor character informations" );			

			int size = sizeof( MSG_ZONE_REQ_CHARAC_INFO );
			MSG_ZONE_REQ_CHARAC_INFO requestcharacterinfo;
			ZeroMemory ( &requestcharacterinfo, size );
			requestcharacterinfo.header.sLength = size;
			requestcharacterinfo.ucMessage = MSG_NO_ZONE_REQ_CHARAC_INFO;
			requestcharacterinfo.usUniqueID = g_LastRequestCharacterNameID;			
			
			return _XSendPacket_Encapsulate( (char*)&requestcharacterinfo );
		}	
		break;
	case MSG_NO_LEVEL_UP_REQ :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : level up request" );

			int size = sizeof( MSG_LEVEL_UP_REQ );
			MSG_LEVEL_UP_REQ levelupreq;
			ZeroMemory ( &levelupreq, size );
			levelupreq.header.sLength = size;
			levelupreq.ucMessage = MSG_NO_LEVEL_UP_REQ;
						
			return _XSendPacket_Encapsulate( (char*)&levelupreq );
		}
		break;
	case MSG_NO_POINT_USE :
		{			
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : point use" );

			int size = sizeof( MSG_POINT_USE_REQ );
			MSG_POINT_USE_REQ pointuse;
			ZeroMemory ( &pointuse, size );
			pointuse.header.sLength = size;
			pointuse.ucMessage = MSG_NO_POINT_USE;
			pointuse.cWhere = param1;
			
			return _XSendPacket_Encapsulate( (char*)&pointuse );
		}
		break;
	case MSG_NO_ITEM_GRAB_REQ :
		{	
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : grab item" );

			int size = sizeof( MSG_ITEM_GRAB_REQ );
			MSG_ITEM_GRAB_REQ grabitempacket;
			ZeroMemory ( &grabitempacket, size );
			grabitempacket.header.sLength = size;
			grabitempacket.ucMessage = MSG_NO_ITEM_GRAB_REQ;
			
			// 2004.06.03->oneway48 insert
			grabitempacket.fX = g_pLocalUser->m_Position.x;
			grabitempacket.fZ = g_pLocalUser->m_Position.z;
			// insert end
			
			grabitempacket.iItemWorldID = param1; 
			grabitempacket.cTo = param2;
			
			_XDWINPRINT("=== send item grab    invenindex[%d]", param2);
			return _XSendPacket_Encapsulate( (char*)&grabitempacket );
		}
		break;
	case MSG_NO_SKILL_SELECT :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : select skill" );

			int size = sizeof( MSG_SKILL_SELECT );
			MSG_SKILL_SELECT skillselectpacket;
			ZeroMemory ( &skillselectpacket, size );
			skillselectpacket.header.sLength = size;
			skillselectpacket.ucMessage = MSG_NO_SKILL_SELECT;
			
			skillselectpacket.sSkillID   = g_pLocalUser->m_SelectedSkillID;
			if( g_pLocalUser->m_SelectedSkillItem )
				skillselectpacket.cSelectLevel = g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel;
			else
				skillselectpacket.cSelectLevel = 0;
			
			return _XSendPacket_Encapsulate( (char*)&skillselectpacket );
		}
		break;

	//===============================================================================================================		
	// �κ��丮 ����		
	case MSG_NO_INVEN_REQ :
		{		
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : inventory request" );

			int size = sizeof( MSG_INVEN_REQ );
			MSG_INVEN_REQ inventoryrequest;
			ZeroMemory ( &inventoryrequest, size );
			inventoryrequest.header.sLength = size;
			inventoryrequest.ucMessage = MSG_NO_INVEN_REQ;
			
			return _XSendPacket_Encapsulate( (char*)&inventoryrequest );
		}
		break;

	case MSG_NO_INVEN_MOVE :
		{
			//Author : ����� //breif : ���⼭ �˻�� ������ �Ѵ� ��� ��Ŷ ������ ������ �ű�
			/*static int iLastInvenMoveTime = 0;
			if( iLastInvenMoveTime == 0 )
			{
				iLastInvenMoveTime = g_LocalSystemTime;
			}
			else
			{			
				if( g_LocalSystemTime - iLastInvenMoveTime < 1000 )
				{
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2329), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					break;
				}
			}
			
			if( g_pLocalUser->m_bMatchPRMode ) // �� ȫ���϶� 
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1826), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				break;
			}

			iLastInvenMoveTime = g_LocalSystemTime;
			
			if(g_pLocalUser->m_CurrentSelectedSkillType != _XSI_USEFULTYPE_ACTIVESKILL && g_pLocalUser->m_CurrentSelectedSkillType != 0)
			{
				if(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST)
				{
					if(g_pInventory_Window)
					{
						g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
					}
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1823), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					break;
				}
			}
			
			// �����߿��� ��ü �Ұ���
			if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_TRAINING)
			{
				if(g_pInventory_Window)
				{
					g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
					g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				}
				InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2330), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				break;
			}*/

			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : change inventory slot" );

			int size = sizeof( MSG_INVEN_MOVE );
			MSG_INVEN_MOVE invenmovepacket;
			ZeroMemory ( &invenmovepacket, size );
			invenmovepacket.header.sLength = size;
			invenmovepacket.ucMessage = MSG_NO_INVEN_MOVE;

			if(g_pInventory_Window)
			{
				invenmovepacket.cFrom = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
				invenmovepacket.cTo = param2;
				invenmovepacket.cFlag = (char)param1;
				invenmovepacket.ucSendCount = (unsigned char)g_pInventory_Window->m_MouseSlotItem.Get_m_ucCount();

				//Author : ����� //breif : ������ ��ü �� ���� ����
				g_pQuickSlot_Window->ReplacementOfItems(TRUE);

				//Author : �����  breif : ���⸦ ��ü ��..
				if( param1 == 2 ) 
					g_pLocalUser->SetChangingWeapon( TRUE );
			}
										
			return _XSendPacket_Encapsulate( (char*)&invenmovepacket );
		}
		break;
	case MSG_NO_INVEN_DROP :
		{
			if(g_pLocalUser->m_CurrentSelectedSkillType != _XSI_USEFULTYPE_ACTIVESKILL && g_pLocalUser->m_CurrentSelectedSkillType != 0)
			{
				if(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST)
				{
					if(g_pInventory_Window)
					{
						g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
					}
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_1607), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					break;
				}
			}

			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : drop inventory item" );
			
			int size = sizeof( MSG_INVEN_DROP );
			MSG_INVEN_DROP dropitempacket;
			ZeroMemory ( &dropitempacket, size );
			dropitempacket.header.sLength = size;
			dropitempacket.ucMessage = MSG_NO_INVEN_DROP;
			
			dropitempacket.cInvenIndex = (char)param1;
			dropitempacket.uiCount     = (unsigned int)param2;		
#ifdef _XDEF_WASTEBASKET_20070226
			dropitempacket.cBackPackExtend = (char)param3;
#endif
			
			return _XSendPacket_Encapsulate( (char*)&dropitempacket );
		}
		break;
	case MSG_NO_MONEY_DROP :
		{
			if(g_pLocalUser->m_CurrentSelectedSkillType != _XSI_USEFULTYPE_ACTIVESKILL && g_pLocalUser->m_CurrentSelectedSkillType != 0)
			{
				if(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST)
				{
					if(g_pInventory_Window)
					{
						g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
						g_pInventory_Window->m_bHaveMouseSlot = FALSE;
					}
					InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_1607), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
					break;
				}
			}
			
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : money drop");
			
			int size = sizeof(MSG_MONEY_DROP);
			MSG_MONEY_DROP moneydrop;
			ZeroMemory(&moneydrop, size);
			moneydrop.header.sLength = size;
			moneydrop.ucMessage = MSG_NO_MONEY_DROP;
			
			moneydrop.uiCount = (unsigned int)param1;
			
			return _XSendPacket_Encapsulate( (char*)&moneydrop );
		}
		break;

		//===============================================================================================================		
		// â�� ����
	case MSG_NO_STR_ITEM_REQ :
		{
			VIEWPACKETLOG			
				_XDWPACKETLOG( "SEND MESSAGE : storage list request" );
			
			int size = sizeof( MSG_STR_ITEM_REQ );
			MSG_STR_ITEM_REQ storagelistpacket;
			ZeroMemory ( &storagelistpacket, size );
			storagelistpacket.header.sLength = size;
			storagelistpacket.ucMessage = MSG_NO_STR_ITEM_REQ;
			
			return _XSendPacket_Encapsulate( (char*)&storagelistpacket );
		}
		break;	
		
	case MSG_NO_STR_ITEM_SAVE :
		{			
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : Send From Inventory To Warehouse" );
			
			int size = sizeof(MSG_STR_ITEM_SAVE);
			MSG_STR_ITEM_SAVE stritemsave;
			ZeroMemory(&stritemsave, size);
			stritemsave.header.sLength = size;
			stritemsave.ucMessage = MSG_NO_STR_ITEM_SAVE;
			
			_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
			if(pWarehouse_Window)
			{
				if(param1 == 0)
				{
					// ������
					stritemsave.cItemNum = (char)pWarehouse_Window->m_TempItemList.size();
					int cashcheck = 0;
					int i = 0;
					list <_XWarehouseItem*>::iterator iter_item;
					for(iter_item = pWarehouse_Window->m_TempItemList.begin() ; iter_item != pWarehouse_Window->m_TempItemList.end() ; ++iter_item)
					{
						_XWarehouseItem* pWarehouseItem = *iter_item;
						if(pWarehouse_Window)
						{
							if( pWarehouseItem->m_InventoryNumber >= 20 )
							{
								if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() == _XGI_FC_QUEST )
								{
									InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_NOTSAVEQUEST), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
									return TRUE;
								}
								
								#ifdef _XDEF_WAREHOSE_RESTRAINT_OF_TRADE //Author : ����� //breif : â������� ���� �߰�
								if( _XGameItem::CheckItemTempField4( pWarehouseItem->Get_m_cType() ,
																		pWarehouseItem->Get_m_sID() ) )
								{
									InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3404), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //�ش� �������� â�� ������ �� �����ϴ�.
									return TRUE;
								}
								#endif
								#ifdef _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI
								#else
								// �ӽ� ó�� - ĳ�� ������ �Ϻδ� â�� ������ �� ����
								cashcheck = CheckCashItem(pWarehouseItem->m_cType, pWarehouseItem->Get_m_sID());
								if(cashcheck == 2 || cashcheck == 3)
								{
									InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3309), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//�Ⱓ�� �������� â�� ������ �� �����ϴ�.
									return TRUE;
								}
								#endif
								
								stritemsave.Storage[i].cIdx = pWarehouseItem->m_InventoryNumber;
								stritemsave.Storage[i].usNumber = (u_char)pWarehouseItem->Get_m_ucCount();
								stritemsave.Storage[i].cIsstackable = (char)g_pLocalUser->m_UserItemList[pWarehouseItem->m_InventoryNumber].IsStackable();
								i++;
							}							
						}
					}
				}
				else if(param1 == 1)
				{
					// ��
					stritemsave.uiMoney = pWarehouse_Window->m_TempMoney;
				}
				
				pWarehouse_Window->m_bWaitForServerResult = TRUE;
				pWarehouse_Window->m_TradeDelayTime = g_LocalSystemTime;
				//_XLog( "CtoS_count %d index %d", stritemsave.cItemNum, stritemsave.Storage[0].cIdx );
				
				return _XSendPacket_Encapsulate( (char*)&stritemsave );
			}			
		}
		break;	
		
	case MSG_NO_STR_ITEM_EJECT :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : Send From Storage to Inventory" );
			
			int size = sizeof(MSG_STR_ITEM_EJECT);
			MSG_STR_ITEM_EJECT stritemeject;
			ZeroMemory(&stritemeject, size);
			stritemeject.header.sLength = size;
			stritemeject.ucMessage = MSG_NO_STR_ITEM_EJECT;
			
			_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
			if(pWarehouse_Window)
			{
				if(param1 == 0)
				{
					// ������
					stritemeject.cItemNum = (char)pWarehouse_Window->m_TempItemList.size();
					int i = 0;
					list <_XWarehouseItem*>::iterator iter_item;
					for(iter_item = pWarehouse_Window->m_TempItemList.begin() ; iter_item != pWarehouse_Window->m_TempItemList.end() ; ++iter_item)
					{
						_XWarehouseItem* pWarehouseItem = *iter_item;
						if(pWarehouse_Window)
						{
							stritemeject.Storage[i].cIdx = pWarehouseItem->m_WarehouseNumber;
							stritemeject.Storage[i].usNumber = (u_char)pWarehouseItem->Get_m_ucCount();
							stritemeject.Storage[i].cIsstackable = g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber].IsStackable();
							i++;
						}
					}
				}
				else if(param1 == 1)
				{
					// ��
					stritemeject.uiMoney = pWarehouse_Window->m_TempMoney;
				}
				
				pWarehouse_Window->m_bWaitForServerResult = TRUE;
				pWarehouse_Window->m_TradeDelayTime = g_LocalSystemTime;
				
				//_XLog( "CtoS_count %d index %d", stritemeject.cItemNum, stritemeject.Storage[0].cIdx );
				
				return _XSendPacket_Encapsulate( (char*)&stritemeject );
			}			
		}
		break;	

		// 2004.06.02->oneway48 insert
	case MSG_NO_STR_ITEM_EJECTEX :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : Send From Storage to Inventory : Terminate State" );
			
			int size = sizeof(MSG_STR_ITEM_EJECTEX);
			MSG_STR_ITEM_EJECTEX stritemeject;
			ZeroMemory(&stritemeject, size);
			stritemeject.header.sLength = size;
			stritemeject.ucMessage = MSG_NO_STR_ITEM_EJECTEX;
			
			_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
			if(pWarehouse_Window)
			{
				if(param1 == 0)
				{
					// ������
					stritemeject.cItemNum = (char)pWarehouse_Window->m_TempItemList.size();
					int i = 0;
					list <_XWarehouseItem*>::iterator iter_item;
					for(iter_item = pWarehouse_Window->m_TempItemList.begin() ; iter_item != pWarehouse_Window->m_TempItemList.end() ; ++iter_item)
					{
						_XWarehouseItem* pWarehouseItem = *iter_item;
						if(pWarehouse_Window)
						{
							stritemeject.Storage[i].cIdx = pWarehouseItem->m_WarehouseNumber;
							stritemeject.Storage[i].usNumber = (u_char)pWarehouseItem->Get_m_ucCount();
							stritemeject.Storage[i].cIsstackable = g_pLocalUser->m_WarehouseItemList[pWarehouseItem->m_WarehouseNumber].IsStackable();
							i++;
						}
					}
					pWarehouse_Window->m_TempMoney = (int)pWarehouse_Window->m_TempItemList.size()*50;
					if( pWarehouse_Window->m_TempMoney > g_pLocalUser->m_Money )
					{
						InsertChatString( _XGETINTERFACETEXT(ID_STRING_NPCTRADE_NOTENOUGHMONEY), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
						return FALSE;
					}
					stritemeject.uiMoney = pWarehouse_Window->m_TempMoney;
				}
				else if(param1 == 1)
				{
					// ��
					stritemeject.uiMoney = pWarehouse_Window->m_TempMoney ;
				}
				
				pWarehouse_Window->m_bWaitForServerResult = TRUE;
				pWarehouse_Window->m_TradeDelayTime = g_LocalSystemTime;
				
				return _XSendPacket_Encapsulate( (char*)&stritemeject );
			}			
		}
		break;	
		// insert end

		//===============================================================================================================		
		// â�� �߰� ��� ����
		// 2004.05.20->oneway48 insert
	case MSG_NO_STR_BUY_SLOT:
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : Add Warehouse Slot" );
			
			int size = sizeof(MSG_STR_BUY_SLOT);
			MSG_STR_BUY_SLOT addslot;
			ZeroMemory(&addslot, size);
			addslot.header.sLength = size;
			addslot.ucMessage = MSG_NO_STR_BUY_SLOT;
			
			_XWindow_AddSaveItem* pAddSaveItem_Window = (_XWindow_AddSaveItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ADDSAVEITEM);
			if(pAddSaveItem_Window)
			{
			#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
				addslot.ucReserveType =	pAddSaveItem_Window->GetReserveType();
			#else
				// 2004.06.02->oneway48 modify
				if( pAddSaveItem_Window->m_AddStorageSize == 20 )
				{
					if( pAddSaveItem_Window->m_AddStorageTerm == 120 ) 
					{
						addslot.ucReserveType = 0;
					}
					else if(pAddSaveItem_Window->m_AddStorageTerm == 240)
					{
						addslot.ucReserveType = 1;
					}
					else if(pAddSaveItem_Window->m_AddStorageTerm == 365)
					{
						addslot.ucReserveType = 2;
					}
				}
				else if( pAddSaveItem_Window->m_AddStorageSize == 30 )
				{
					if( pAddSaveItem_Window->m_AddStorageTerm == 120 ) 
					{
						addslot.ucReserveType = 3;
					}
					else if(pAddSaveItem_Window->m_AddStorageTerm == 240)
					{
						addslot.ucReserveType = 4;
					}
					else if(pAddSaveItem_Window->m_AddStorageTerm == 365)
					{
						addslot.ucReserveType = 5;
					}
				}
				else if( pAddSaveItem_Window->m_AddStorageSize == 40 )
				{
					if( pAddSaveItem_Window->m_AddStorageTerm == 120 ) 
					{
						addslot.ucReserveType = 6;
					}
					else if(pAddSaveItem_Window->m_AddStorageTerm == 240)
					{
						addslot.ucReserveType = 7;
					}
					else if(pAddSaveItem_Window->m_AddStorageTerm == 365)
					{
						addslot.ucReserveType = 8;
					}
				}
			#endif
				
				addslot.uiPayMoney = (u_int)pAddSaveItem_Window->m_AddStorageCost;
				
				return _XSendPacket_Encapsulate( (char*)&addslot );
			}			
		}
		break;
		
	case MSG_NO_STR_BUY_SLOT_CANCEL:
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : Add Warehouse Slot ---- Fail" );
			
			int size = sizeof(MSG_STR_BUY_SLOT_CANCEL);
			MSG_STR_BUY_SLOT_CANCEL addslotfail;
			ZeroMemory(&addslotfail, size);
			addslotfail.header.sLength = size;
			addslotfail.ucMessage = MSG_NO_STR_BUY_SLOT_CANCEL;
			addslotfail.uiMoney = g_pLocalUser->m_Money;// 2004.05.24->oneway48 insert
			
			return _XSendPacket_Encapsulate( (char*)&addslotfail );
		}
		break;		

		// 2004.06.01->oneway48 insert
	case MSG_NO_STR_BUY_SLOT_RELEASE:
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : Cancle Warehouse Slot ---- " );
			
			int size = sizeof(MSG_STR_BUY_SLOT_RELEASE);
			MSG_STR_BUY_SLOT_RELEASE canclewarehouse;
			ZeroMemory(&canclewarehouse, size);
			canclewarehouse.header.sLength = size;
			canclewarehouse.ucMessage = MSG_NO_STR_BUY_SLOT_RELEASE;
			
			return _XSendPacket_Encapsulate( (char*)&canclewarehouse );
		}
		break;
		// insert end
		
		//===============================================================================================================		
		// ���� ����
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : ����� breif : ��������
	case MSG_NO_ITEM_SOCKET_GAMBLE : 
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : Item Equip Socket Send ---- " );

			switch( (unsigned char)param1 )
			{
			case en_socket_gamble_mode_equip_socket:
			case en_socket_gamble_mode_boxgamble:
				{
					int size = sizeof(MSG_SM_EQUIP_SOCKET);
					MSG_SM_EQUIP_SOCKET socketitem;
					ZeroMemory(&socketitem, size);
					socketitem.header.sLength = size;
					socketitem.ucMessage = MSG_NO_ITEM_SOCKET_GAMBLE;
					socketitem.ucMode = param1;

					socketitem.ucToolItemIdx = g_pInventory_Window->m_UsedLifeItemIndex;
					socketitem.ucObjItemIdx = g_pInventory_Window->m_iSocketIndexList[0];
					socketitem.cAddonCounter = g_pInventory_Window->GetSocketListCount();
					
					int startpos = g_pInventory_Window->GetSocketListInsertPos();
					for( int i = startpos; i < socketitem.cAddonCounter+startpos; ++i )
					{
						socketitem.ucAddonItemIdx[i-1] = g_pInventory_Window->m_iSocketIndexList[i];
					}

					return _XSendPacket_Encapsulate( (char*)&socketitem );
				}
				break;
			case en_socket_gamble_mode_separate_socket: //���Ϻи�
				{
					int size = sizeof(MSG_SM_SEPARATE_SOCKET);
					MSG_SM_SEPARATE_SOCKET DetachSocket;
					ZeroMemory(&DetachSocket, size);
					DetachSocket.header.sLength = size;
					DetachSocket.ucMessage = MSG_NO_ITEM_SOCKET_GAMBLE;
					DetachSocket.ucMode = en_socket_gamble_mode_separate_socket;

					_XWindow_SocketDetach* pSocketDetachWindow = (_XWindow_SocketDetach*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETDETACH_WINDOW);

					DetachSocket.ucTargetSlot = pSocketDetachWindow->GetIndependentItemSlot();
					DetachSocket.ucSocketSlot = pSocketDetachWindow->GetSocketIndex();
					DetachSocket.ucToolSlot = pSocketDetachWindow->GetDetachItemSlot();
					DetachSocket.ucEmptySlot = pSocketDetachWindow->VacancySlot();

					return _XSendPacket_Encapsulate( (char*)&DetachSocket );
				}
				break;
			}
		}
		break;
#else
	case MSG_NO_ITEM_EQUIP_SOCKET :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : Item Equip Socket Send ---- " );

			int size = sizeof(MSG_ITEM_EQUIP_SOCKET);
			MSG_ITEM_EQUIP_SOCKET socketitem;
			ZeroMemory(&socketitem, size);
			socketitem.header.sLength = size;
			socketitem.ucMessage = MSG_NO_ITEM_EQUIP_SOCKET;			
			if( g_pInventory_Window )
			{
				socketitem.ucToolItemIdx = g_pInventory_Window->m_UsedLifeItemIndex;
				socketitem.ucObjItemIdx = g_pInventory_Window->m_iSocketIndexList[0];
				socketitem.cAddonCounter = g_pInventory_Window->GetSocketListCount();
				
				int startpos = g_pInventory_Window->GetSocketListInsertPos();
				for( int i = startpos; i < socketitem.cAddonCounter+startpos; ++i )
				{
					socketitem.ucAddonItemIdx[i-1] = g_pInventory_Window->m_iSocketIndexList[i];
				}

				socketitem.cMode = param1;

				return _XSendPacket_Encapsulate( (char*)&socketitem );
			}			
		}
		break;
#endif

		//===============================================================================================================		
		// ����Ʈ ����
	case MSG_NO_QUEST_SCON_REQUEST :
	case MSG_NO_QUEST_SET_SELECTION_NO :
	case MSG_NO_QUEST_USER_CANCEL :
	case MSG_NO_QUEST_SETCOUNT: // ����Ʈ ī���� ����
	case MSG_NO_QUEST_VALIDCHECK: // ����Ʈ ���� �ڵ� ��Ŷ :  ����Ʈ�� ��ȿ���� �˻縦 �Ѵ�.
	case MSG_NO_QUEST_VALIDCHECK_INGAME: // ���� ����󿡼� ����Ʈ ���� �ڵ� ��Ŷ :  ����Ʈ�� ��ȿ���� �˻縦 �Ѵ�.
	case MSG_NO_QUESTRANK_RES: // ����Ʈ ��ŷ�� ��û�Ѵ�.
		{			
			if(!Send_MSGQuest(type, param1, param2, param3, param4))
				return FALSE;
		}
		break;
		
	//===============================================================================================================		
		// ���� ���� ����
	case MSG_NO_PS_CTRL :
		{			
			if(!Send_MSGPersonalStore(type, param1, param2, param3, param4))
				return FALSE;
		}
		break;
		
		// ==========================================================================
		// ������ ���
	case MSG_NO_USE_ITEM_REQ :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : Use Item ");
			
			int size = sizeof(MSG_USE_ITEM_REQ);
			MSG_USE_ITEM_REQ useitemreq;
			ZeroMemory(&useitemreq, size);
			useitemreq.header.sLength = size;
			useitemreq.ucMessage = MSG_NO_USE_ITEM_REQ;
			
			useitemreq.ucInvenNumber = param1;
			useitemreq.cIsLpsKey	= (char)param2;
			useitemreq.sDNPCID		= (unsigned short)param3;

			if( g_pLocalUser->m_bInternalUse )
			{
				useitemreq.cIsFail		= (char)param2;
				useitemreq.cIsLpsKey	= 0;
			}
			else 
				useitemreq.cIsFail		= 0;
			
			return _XSendPacket_Encapsulate( (char*)&useitemreq );			
		}
		break;
// 2005.02.23->hotblood -------------------------------------------------------------------=
	case  MSG_NO_TEMP_INVEN_CTRL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : Temp Inven Item");
			
			int size = sizeof( MSG_TEMP_INVEN_CTRL );
			MSG_TEMP_INVEN_CTRL tempinven;
			ZeroMemory(&tempinven, size);
			tempinven.header.sLength = size;
			tempinven.ucMessage = MSG_NO_TEMP_INVEN_CTRL;

			tempinven.cTIdx = (char)param1;
			tempinven.cWhere = (char)param2;

			return _XSendPacket_Encapsulate( (char*)&tempinven );
		}
		break;
// ----------------------------------------------------------------------------------------=
	case MSG_NO_WORLD_ITEM_REQUEST :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : World Item Request");
			
			int size = sizeof(MSG_WORLD_ITEM_REQUEST);
			MSG_WORLD_ITEM_REQUEST worlditemrequest;
			ZeroMemory(&worlditemrequest, size);
			worlditemrequest.header.sLength = size;
			worlditemrequest.ucMessage = MSG_NO_WORLD_ITEM_REQUEST;
			
			return _XSendPacket_Encapsulate( (char*)&worlditemrequest );
		}
		break;

		//===============================================================================================================		
		// ������ ���� ��� ����
		// 2004.06.18->oneway48 insert
	case MSG_NO_NPC_ITEM_REPAIR_REQUEST :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : Repair Item ================================== ");
			
			int size = sizeof(MSG_NPC_ITEM_REPAIR_REQUEST);
			MSG_NPC_ITEM_REPAIR_REQUEST repairitemreq;
			ZeroMemory(&repairitemreq, size);
			
			_XWindow_RepairItem* pRepairItem_Window = (_XWindow_RepairItem*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REPAIRITEM );
			int repaircount = 0;
			
			if( pRepairItem_Window->m_SelectedSlotNoList.empty()) 
			{
				repaircount = 1;
				repairitemreq.ucInvenIndex[0] = pRepairItem_Window->m_SelectedSlotNo;
			}
			else
			{
				list <int>::iterator iter;
				for(iter = pRepairItem_Window->m_SelectedSlotNoList.begin() ; iter != pRepairItem_Window->m_SelectedSlotNoList.end() ; ++iter)
				{
					repairitemreq.ucInvenIndex[repaircount] = *iter;
					repaircount++;				
				}				
			}

			repairitemreq.header.sLength = 11 + repaircount;
			repairitemreq.ucMessage = MSG_NO_NPC_ITEM_REPAIR_REQUEST;
			
			repairitemreq.usNPCID = g_LastNPCScriptOwner->m_UniqueID;
			repairitemreq.cMode = (char)param1;

			if( repairitemreq.cMode == 0 )
			{
				repairitemreq.uiNeedMoney = 0;
			}
			else if( repairitemreq.cMode == 1 )
			{
				repairitemreq.uiNeedMoney = pRepairItem_Window->m_NeedMoney;
			}

			repairitemreq.ucRepairCount = repaircount;
			
			return _XSendPacket_Encapsulate( (char*)&repairitemreq );
		}
		break;

		// ==========================================================================
		// ��Ƽ ����
	case MSG_NO_PARTY_JOIN_REQ :
	case MSG_NO_PARTY_ETC :
	case MSG_NO_PARTY_RESPONSE :
		{
			if(!Send_MSGParty(type, param1, param2, param3))
				return FALSE;
		}
		break;
		
		// =========================================================================
		// Ʈ���̵� ����
	case MSG_NO_NPC_TRADE_REQ :
		{
			_XWindow_NPCTrade* pNPCTrade_Window = (_XWindow_NPCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADE);
			_XWindow_Inventory* pInventory_Window = (_XWindow_Inventory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INVENTORY);
			if(pNPCTrade_Window && pInventory_Window)
			{
				VIEWPACKETLOG
					_XDWPACKETLOG("SEND MESSAGE : NPC TRADE ============================");
				
				int size = sizeof(MSG_NPC_TRADE_REQ);
				MSG_NPC_TRADE_REQ npctradereq;
				ZeroMemory(&npctradereq, size);
				npctradereq.header.sLength = size;
				npctradereq.ucMessage = MSG_NO_NPC_TRADE_REQ;
						
				int slotnumber = 0;
				int itemNo = 0;
				char cType = 0;
				short sID = 0;

				npctradereq.cBuyFromNPC = (char)param1;

				switch(param1)
				{
				case en_npc_trade_mode_sell :
					{
						// �ȱ�
						npctradereq.ucCount = (unsigned char)pInventory_Window->m_MouseSlotItem.Get_m_ucCount();
						itemNo = pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
						cType = pInventory_Window->m_MouseSlotItem.Get_m_cType();
						sID = pInventory_Window->m_MouseSlotItem.Get_m_sID();

						// ����ó��
						if(npctradereq.ucCount <= 0)
						{
							npctradereq.ucCount = 1;
						}
					}
					break;
				case en_npc_trade_mode_buy_contribute :
				case en_npc_trade_mode_buy :
					{
						// ���
						npctradereq.ucCount = (unsigned char)pNPCTrade_Window->m_MouseSlotItem.Get_m_ucCount();
						pNPCTrade_Window->GetItemPropertyFromNo(pNPCTrade_Window->m_MouseSlotItem.m_InventoryNumber, cType, sID, slotnumber);
						itemNo = pNPCTrade_Window->m_MouseSlotItem.m_InventoryNumber;

						// ���� ó��
						if(npctradereq.ucCount <= 0)
						{
							npctradereq.ucCount = 1;
						}
					}
					break;
				case en_npc_trade_mode_contribute_moonpa :
					{
						// ���ı⿩
						_XWindow_Contribution* pContribution_Window = (_XWindow_Contribution*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CONTRIBUTIONWINDOW);
						npctradereq.ucCount = pContribution_Window->m_ContributionItemCount;					
						itemNo = pContribution_Window->m_ContributionIvenIndex;
                    #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
						if( itemNo == (_XINVENTORY_SLOTTYPE_MONEYINDEX-1) )
                    #else
                        if( itemNo == 126 )
                    #endif
						{
							npctradereq.ucCount = 1;
							param2 = pContribution_Window->m_ContributionItemCount;
						}
						else
						{
							if( itemNo >= _XINVENTORY_SLOTTYPE_TOTALCOUNT )
								return FALSE;
						}
						cType = pContribution_Window->m_ContributionType;
						sID = pContribution_Window->m_ContributionId;
					}
					break;
				case en_npc_trade_mode_heal :
					{
						// ġ��
						npctradereq.ucCount = 1;
					}
					break;
				case en_npc_trade_mode_gamble :
					{
						// �ϻ��ο��� ���
						itemNo = pNPCTrade_Window->m_GambleItemInfo[pNPCTrade_Window->m_MouseSlotItem.m_InventoryNumber].ItemNo;	// ������ ����Ʈ�� ���� : -1 ���ش�.
						cType = pNPCTrade_Window->m_GambleItemInfo[pNPCTrade_Window->m_MouseSlotItem.m_InventoryNumber].Type;
						sID = pNPCTrade_Window->m_GambleItemInfo[pNPCTrade_Window->m_MouseSlotItem.m_InventoryNumber].cluindex;	// �׷��ε���
						param2 = pNPCTrade_Window->m_GambleItemInfo[pNPCTrade_Window->m_MouseSlotItem.m_InventoryNumber].Price;
						npctradereq.ucCount = 1;
					}
					break;
				case en_npc_trade_mode_move_village :
					{
						// ǥ�� �̵�
						_XWindow_MoveVillage* pMoveVillage_Window = (_XWindow_MoveVillage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MOVEVILLAGE);
						if(pMoveVillage_Window)
						{
							itemNo = pMoveVillage_Window->GetSelectedInvenNumber();
							sID = pMoveVillage_Window->GetSelectedMoveVillageIndex()+1;
							npctradereq.ucCount = 1;
						}
					}
					break;

				}

				npctradereq.usNPCID = (unsigned short)pNPCTrade_Window->m_OwnerNPCID;
				npctradereq.ucItemNo = itemNo;
				npctradereq.cItemType = cType;
				npctradereq.sItemID = sID;
				npctradereq.uiNeedMoney = (unsigned int)param2;
				
				return _XSendPacket_Encapsulate( (char*)&npctradereq );
			}
		}
		break;

//#ifdef _XADMINISTRATORMODE
	case MSG_NO_GM_REQUEST		   :
		{
#ifdef _XGMCLIENT
			switch( param1 ) 
			{
			case en_gm_req_nextto_charac :
				{
					_XWindow_GM_UserCoordination* pWindow = (_XWindow_GM_UserCoordination*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERCOORDSETTINGWINDOW);
					if( pWindow )  
					{
						int size = sizeof(MSG_GM_REQ_NEXTTO_CHARAC);
						MSG_GM_REQ_NEXTTO_CHARAC req_nextto_charac;
						ZeroMemory(&req_nextto_charac, size);
						req_nextto_charac.header.sLength = size;
						req_nextto_charac.ucMessage = MSG_NO_GM_REQUEST;
						req_nextto_charac.ucRequest = en_gm_req_nextto_charac;

						strcpy( req_nextto_charac.cToCharac, pWindow->m_CharacterNameIMEControl.GetText() );
						
						return _XSendPacket_Encapsulate( (char*)&req_nextto_charac );
					}
				}
				break;
			case en_gm_req_charac_move   :
				{
					_XWindow_GM_UserCoordination* pWindow = (_XWindow_GM_UserCoordination*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERCOORDSETTINGWINDOW);
					if( pWindow )
					{
						int size = sizeof(MSG_GM_REQ_CHARAC_MOVE);
						MSG_GM_REQ_CHARAC_MOVE req_characmove;
						ZeroMemory(&req_characmove, size);
						req_characmove.header.sLength = size;
						req_characmove.ucMessage = MSG_NO_GM_REQUEST;
						req_characmove.ucRequest = en_gm_req_charac_move;
						
						req_characmove.cServerNo = g_pLocalUser->m_CharacterInfo.zoneuniqid;
						req_characmove.fX = atof(pWindow->m_XCoordIMEControl.GetText()) - g_LodTerrain.m_HalfWorldSize;
						req_characmove.fZ = atof(pWindow->m_ZCoordIMEControl.GetText()) - g_LodTerrain.m_HalfWorldSize;
						strcpy( req_characmove.cTargetName, pWindow->m_CharacterNameIMEControl.GetText() );
						
						return _XSendPacket_Encapsulate( (char*)&req_characmove );
					}
				}
				break;
			case en_gm_req_npc_kill_sign : 
				{

				}
				break;
			case en_gm_req_enter_warp	 :
				{

				}
				break;
			case en_gm_req_charac_moveto_warp :
				{
					
				}
				break;			
			} 
#endif
		}
		break;
	case MSG_NO_CTRL_MONSTER_SPAWN :
		{
#ifdef _XGMCLIENT
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : GMS Monster spawn ");

			if( param1 > 0 && param1 < _XDEF_MODELCLASSCOUNT_MOB-1 )
			{			
				int size = sizeof(MSG_CTRL_MONSTER_SPAWN);
				MSG_CTRL_MONSTER_SPAWN monsterspawn;
				ZeroMemory(&monsterspawn, size);
				monsterspawn.header.sLength = size;
				monsterspawn.ucMessage = MSG_NO_CTRL_MONSTER_SPAWN;
							
				monsterspawn.fX = g_pLocalUser->m_Position.x;
				monsterspawn.fZ = g_pLocalUser->m_Position.z;

				if( param2 <= 0 ) param2 = 1;
				if( param2 > 24 ) param2 = 24;

				param1 += 4000;

				monsterspawn.sMobType = param1;
				monsterspawn.cNumber   = param2;

				return _XSendPacket_Encapsulate( (char*)&monsterspawn );
			}
			else
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Admin error", _XGETINTERFACETEXT(ID_STRING_NETWORK_NOTREADYMOB), TRUE );
			}
#endif
		}
		break;
			
	case MSG_NO_CTRL_CHARAC_SETTING :
		{

#ifdef _XGMCLIENT

			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : GMS Charac Ctrl");

			int size = sizeof(MSG_CTRL_CHARAC_SETTING);
			MSG_CTRL_CHARAC_SETTING ctrlcharacsetting;
			ZeroMemory(&ctrlcharacsetting, size);
			ctrlcharacsetting.header.sLength = size;
			ctrlcharacsetting.ucMessage = MSG_NO_CTRL_CHARAC_SETTING;

			ctrlcharacsetting.cCommand = param1;
			switch(ctrlcharacsetting.cCommand)
			{
			case ctrl_ch_skill :
				{
					ctrlcharacsetting.skill.cMode = param2;

					switch(ctrlcharacsetting.skill.cMode)
					{
					case 0 :
						{
							// Add skill
							ctrlcharacsetting.skill.sSkillID = param3;
							ctrlcharacsetting.skill.ucSkillSlot = 0;
							ctrlcharacsetting.skill.cSkillStep = param4;
						}
						break;
					case 1 :
						{
							// Remove skill
							ctrlcharacsetting.skill.sSkillID = param3;
							ctrlcharacsetting.skill.ucSkillSlot = g_pLocalUser->GetLearnedSkillIndex(param3);
							ctrlcharacsetting.skill.cSkillStep = param4;
						}
						break;
					case 2 :
						{
							// Edit skill
							ctrlcharacsetting.skill.sSkillID = param3;
							ctrlcharacsetting.skill.ucSkillSlot = g_pLocalUser->GetLearnedSkillIndex(param3);
							ctrlcharacsetting.skill.cSkillStep = param4;
						}
						break;
					case 3 :
						{
							// Trainingstate = 1
							ctrlcharacsetting.skill.sSkillID = param3;
							ctrlcharacsetting.skill.ucSkillSlot = g_pLocalUser->GetLearnedSkillIndex(param3);
							ctrlcharacsetting.skill.cSkillStep = param4;
						}
						break;
					}

					return _XSendPacket_Encapsulate( (char*)&ctrlcharacsetting );
				}
				break;
			case ctrl_ch_quest :
				break;
			case ctrl_ch_class_grade :
				{
					ctrlcharacsetting.Class.cClass = (char)param2;
					ctrlcharacsetting.Class.cGrade = (char)param3;
					ctrlcharacsetting.Class.sAddMasteryPoint = (short)param4;

					return _XSendPacket_Encapsulate( (char*)&ctrlcharacsetting );
				}
				break;
			case ctrl_ch_set_transparent :
				{
					ctrlcharacsetting.Transparent.cMode = (char)param2;
					return _XSendPacket_Encapsulate( (char*)&ctrlcharacsetting );					
				}
				break;
			default :
				{
					return _XSendPacket_Encapsulate( (char*)&ctrlcharacsetting );
				}
				break;
			}
#endif
		}
		break;

//#endif

		// =================================================================================
		// ���� ����
	case MSG_NO_SELF_CHANNEL_START_REQUEST :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : Self Channeling start REQ - Channeling spell ==========================");
			
			//_XDWINPRINT("SEND MESSAGE : Self Channeling start REQ - Channeling spell [%d]", g_LocalSystemTime );
			
			int size = sizeof(MSG_SELF_CHANNEL_START_REQUEST);
			MSG_SELF_CHANNEL_START_REQUEST selfchannelstart;
			ZeroMemory(&selfchannelstart, size);
			selfchannelstart.header.sLength = size;
			selfchannelstart.ucMessage = MSG_NO_SELF_CHANNEL_START_REQUEST;

			selfchannelstart.cMode = param1;
			
			if(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->insideType > 0)	// Hidden
			{
					//20071112 ���� - Vina ������ ���� üũ�� ����.
#ifdef _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA// ��Ʈ���� ���� üũ �α׿����� ���s

				extern LONG g_ModuleFileSize;

				// �Ϻ��� gameguard basic license�� �������� ���� ��� ����.
				#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG) && !defined(_XGMCLIENT) && !defined(_XJAPANESE) && !defined(_XRUSSIAN) && !defined(_XVIETNAMESE) && !defined(_XTAIWANESE)

				if( g_ModuleFileSize < 2500000 ) // ��� ���� ����� �����϶��� ���� �������� ����.
				{
					int a = 100;
					int b = 21;
					int c = a + b;
					selfchannelstart.skillID = _XMEMORYCHECKDATA_ENCSHORT(c); // client version number
				}
				else	// ��� ���� ����� �������϶��� ���� ���������� ����.
				{
					selfchannelstart.skillID = _XMEMORYCHECKDATA_ENCSHORT(118); // client version number
				}
				
				#else

					int a = 100;
					int b = 21;
					int c = a + b;
					selfchannelstart.skillID = _XMEMORYCHECKDATA_ENCSHORT(c); // client version number

				#endif				
#else
				selfchannelstart.skillID = g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->insideType;
#endif
				selfchannelstart.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_SkillProperty[g_pLocalUser->m_SelectedSkillID]->insideType);
			}
			else // self channeling
			{
#ifdef _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA // ��Ʈ���� ���� üũ �α׿����� ���
				extern LONG g_ModuleFileSize;

				// �Ϻ��� gameguard basic license�� �������� ���� ��� ����.
				#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG) && !defined(_XGMCLIENT) && !defined(_XJAPANESE) && !defined(_XRUSSIAN) && !defined(_XVIETNAMESE) && !defined(_XTAIWANESE) 

				if( g_ModuleFileSize < 2500000 ) // ��� ���� ����� �����϶��� ���� �������� ����.
				{
					int a = 100;
					int b = 21;
					int c = a + b;
					selfchannelstart.skillID = _XMEMORYCHECKDATA_ENCSHORT(c); // client version number
				}
				else	// ��� ���� ����� �������϶��� ���� ���������� ����.
				{
					selfchannelstart.skillID = _XMEMORYCHECKDATA_ENCSHORT(118); // client version number
				}
				#else

					int a = 100;
					int b = 21;
					int c = a + b;
					selfchannelstart.skillID = _XMEMORYCHECKDATA_ENCSHORT(c); // client version number

				#endif
#else
				selfchannelstart.skillID = g_pLocalUser->m_SelectedSkillID;
#endif
				
#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
				if(g_pLocalUser->m_SelectedSkillID == g_pLocalUser->m_EventSkillList.m_sSkillID)
				{
					selfchannelstart.ucLearnSkillIndex = 250;	//�̺�Ʈ�� �� ��� ����ϱ�
				}
				else
#endif
				{
					selfchannelstart.ucLearnSkillIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);
				}
			}
			
			return _XSendPacket_Encapsulate( (char*)&selfchannelstart );
		}
		break;

	case MSG_NO_MINIGAME_START_REQ_C_S :
	case MSG_NO_MINIGAME_MAST_STAGE_C_S :
		{
			if(!Send_MSGMiniGame(type, param1, param2, param3))
				return FALSE;
		}
		break;
		
	case MSG_NO_MARTIAL_READY :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Martial Ready ========================");

			if(!Send_MSGMartialReady(type, param1, param2, param3))
				return FALSE;
		}
		break;
	case MSG_NO_MARTIAL_FIRE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Martial Fire =========================");

			if(!Send_MSGMartialFire(type, param1, param2, param3))
				return FALSE;
		}
		break;
	case MSG_NO_MARTIAL_END :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Martial End ==========================");

			if(!Send_MSGMartialEnd(type, param1, param2, param3))
				return FALSE;
		}
		break;
		
	// ==========================================================================
	// P2P Trade ����
	case MSG_NO_PVP_TRADE_REQ :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND MESSAGE : P2P Trade Request =======================");

			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);

			if(!pPCTrade_Window) return FALSE;

			int size = sizeof(MSG_PVP_TRADE_REQ);
			MSG_PVP_TRADE_REQ pvptradereq;
			ZeroMemory(&pvptradereq, size);
			pvptradereq.header.sLength = size;
			pvptradereq.ucMessage = MSG_NO_PVP_TRADE_REQ;

			pvptradereq.cSub = (char)param1;
			
			if(pPCTrade_Window)
			{
				pvptradereq.iTargetID = pPCTrade_Window->m_TargetID;
			}
			
			// Ÿ ������ �ŷ���û�� �����ϸ� Ʈ���̵� ��� Ȱ��ȭ
			if( pvptradereq.cSub == SUB_MSG_TRADE_REJECT ||
				pvptradereq.cSub == SUB_MSG_TRADE_CANCEL )
			{
				g_pLocalUser->m_CharacterLog.isTrade = FALSE;
			}
			else
			{
				g_pLocalUser->m_CharacterLog.isTrade = TRUE;
			}
			
			return _XSendPacket_Encapsulate( (char*)&pvptradereq );
		}
		break;

	case MSG_NO_PVP_TRADE_ADD :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : P2P Trade Add ============================");

			int size = sizeof(MSG_PVP_TRADE_ADD);
			MSG_PVP_TRADE_ADD pvptradeadd;
			ZeroMemory(&pvptradeadd, size);
			pvptradeadd.header.sLength = size;
			pvptradeadd.ucMessage = MSG_NO_PVP_TRADE_ADD;
			
			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
			_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
			if(pPCTrade_Window && pTradeConfirm_Window)
			{				
				pvptradeadd.SlotNo = pPCTrade_Window->m_TradeServerSlotNo;
				pvptradeadd.TWindowIndex = (unsigned char)param1;
				pvptradeadd.item.slot = pPCTrade_Window->m_SelfTradeItem[param1].m_InventoryNumber;
				pvptradeadd.item.number = (unsigned char)param2;

				pPCTrade_Window->m_bWaitTrade = TRUE;
				pvptradeadd.cResetConf = (char)pPCTrade_Window->m_bWaitTrade;
				
				pvptradeadd.cAddCount = 0;

				if(pPCTrade_Window->m_bWaitTrade)
				{
					pPCTrade_Window->ResetTradeSetting();
					pPCTrade_Window->m_bWaitTrade = FALSE;
				}
			}

			g_pLocalUser->m_CharacterLog.isTrade = TRUE;

			return _XSendPacket_Encapsulate( (char*)&pvptradeadd );
		}
		break;

	case MSG_NO_PVP_TRADE_DELETE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : P2P Trade Delete =============================");

			int size = sizeof(MSG_PVP_TRADE_DELETE);
			MSG_PVP_TRADE_DELETE pvptradedelete;
			ZeroMemory(&pvptradedelete, size);
			pvptradedelete.header.sLength = size;
			pvptradedelete.ucMessage = MSG_NO_PVP_TRADE_DELETE;

			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
			if(pPCTrade_Window)
			{
				pvptradedelete.SlotNo = pPCTrade_Window->m_TradeServerSlotNo;
				pvptradedelete.TWindowIndex = (unsigned char)param1;
				pvptradedelete.ucSubjectID = g_pLocalUser->m_Uniqueid;
				pvptradedelete.number = (unsigned char)param2;
				pvptradedelete.Inven_Index = 0;

				pPCTrade_Window->m_bWaitTrade = TRUE;
				pvptradedelete.cResetConf = pPCTrade_Window->m_bWaitTrade;

				if(pPCTrade_Window->m_bWaitTrade)
				{
					pPCTrade_Window->ResetTradeSetting();
					pPCTrade_Window->m_bWaitTrade = FALSE;
				}
			}

			g_pLocalUser->m_CharacterLog.isTrade = TRUE;

			pPCTrade_Window->m_MouseSlotItem.ResetMouseSlotItem();
			if( g_pDefaultTooltip_Window )
			{ 
				if( g_pDefaultTooltip_Window->GetShowStatus() )
					g_pDefaultTooltip_Window->ShowWindow( FALSE );
			}

			return _XSendPacket_Encapsulate( (char*)&pvptradedelete );
		}
		break;

	case MSG_NO_PVP_TRADE_MONEY :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : P2P Trade Money =============================");

			int size = sizeof(MSG_PVP_TRADE_MONEY);
			MSG_PVP_TRADE_MONEY pvptrademoney;
			ZeroMemory(&pvptrademoney, size);
			pvptrademoney.header.sLength = size;
			pvptrademoney.ucMessage = MSG_NO_PVP_TRADE_MONEY;

			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
			if(pPCTrade_Window)
			{
				pvptrademoney.SlotNo = pPCTrade_Window->m_TradeServerSlotNo;
				pvptrademoney.AddOrSub = (char)param1;
				pvptrademoney.uiMoney = (unsigned int)param2;
				
				pPCTrade_Window->ResetTradeSetting();
				pPCTrade_Window->m_bWaitTrade = FALSE;
			}

			return _XSendPacket_Encapsulate( (char*)&pvptrademoney );
		}
		break;

	case MSG_NO_PVP_TRADE_CANCEL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : P2P Trade Cancel ===============================");

			int size = sizeof(MSG_PVP_TRADE_CANCEL);
			MSG_PVP_TRADE_CANCEL pvptradecancel;
			ZeroMemory(&pvptradecancel, size);
			pvptradecancel.header.sLength = size;
			pvptradecancel.ucMessage = MSG_NO_PVP_TRADE_CANCEL;

			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
			if(pPCTrade_Window)
			{
				pvptradecancel.iTargetID = pPCTrade_Window->m_TargetID;

				for(int i = 0 ; i < _XDEF_PCTRADE_MAX_SLOTNUMBER ; ++i)
				{
					pvptradecancel.result[i].slot = (char)pPCTrade_Window->m_SelfTradeItem[i].m_InventoryNumber;
					pvptradecancel.result[i].number = pPCTrade_Window->m_SelfTradeItem[i].Get_m_ucCount();
				}
				pPCTrade_Window->m_bTrading = FALSE;
			}

			return _XSendPacket_Encapsulate( (char*)&pvptradecancel );
		}
		break;
		
		// =================================================================================
		// ��ų ���� ��û
	case MSG_NO_SKILL_INFO_REQ :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Skill Info Req");
			
			int size = sizeof(MSG_SKILL_INFO_REQ);
			MSG_SKILL_INFO_REQ skillinforeq;
			ZeroMemory(&skillinforeq, size);
			skillinforeq.header.sLength = size;
			skillinforeq.ucMessage = MSG_NO_SKILL_INFO_REQ;
			
			return _XSendPacket_Encapsulate( (char*)&skillinforeq );
		}
		break;
	case MSG_NO_SKILL_ACTION :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Skill And State Cancle Req");

			int size = sizeof(MSG_CANCEL_SKILL_EXTRA);
			MSG_CANCEL_SKILL_EXTRA skillaction;
			ZeroMemory(&skillaction, size);
			skillaction.header.sLength	= size;
			skillaction.ucMessage		= MSG_NO_SKILL_ACTION;
			skillaction.ucMode			= (char)param1;
			skillaction.cSlotNo			= (char)param2;
			
			return _XSendPacket_Encapsulate( (char*)&skillaction );
		}
		break;
		// =================================================================================
		// ��ų ����, ���� ����
	case MSG_NO_SKILL_CTRL_REQ :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Skill Ctrl Req");
			
			int size = sizeof(MSG_SKILL_CTRL_REQ);
			MSG_SKILL_CTRL_REQ skillctrlreq;
			ZeroMemory(&skillctrlreq, size);
			skillctrlreq.header.sLength = size;
			skillctrlreq.ucMessage = MSG_NO_SKILL_CTRL_REQ;

			BOOL bSendPacket = TRUE;
			
			skillctrlreq.cMode = (char)param1;
			switch(skillctrlreq.cMode)
			{
			case en_skill_ctrl_req_learn_from_npc :
				{
					// NPC�� ���� ����
					_XWindow_NPCSkill* pNPCSkill_Window = (_XWindow_NPCSkill*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSKILL);
					if(pNPCSkill_Window)
					{
						skillctrlreq.howto.S_from_npc.usNpcID = (unsigned short)pNPCSkill_Window->GetOwnerNPCID();
						skillctrlreq.howto.S_from_npc.ucClassIndex = (unsigned char)pNPCSkill_Window->GetSelectedSkillPackageID();
						skillctrlreq.howto.S_from_npc.ucIndexinList = (unsigned char)pNPCSkill_Window->GetSelectedSkillNo();
						skillctrlreq.howto.S_from_npc.sSkillID = pNPCSkill_Window->GetSelectedSkillID();
					}
				}
				break;
			case en_skill_ctrl_req_step_up :
				{
					// �ܰ� ��� ��û
					if(g_pLocalUser->m_SelectedSkillID != 0)
						bSendPacket = FALSE;

					skillctrlreq.howto.S_step_up.ucSkillSlotIndex = g_pLocalUser->GetLearnedSkillIndex(g_pLocalUser->m_SelectedSkillID);
				}
				break;
			case en_skill_ctrl_req_del_skill :
				{
					// ��ų ���� ��û
#ifdef _XTS_NEWSKILLWINDOW
					short skillid = (short)param2;					
					skillctrlreq.howto.S_delete_skill.ucSkillSlotIndex = g_pLocalUser->GetLearnedSkillIndex(skillid);
#endif
				}
				break;
			case en_skill_ctrl_req_passive :
				{
					// Passive On/Off ��û
					short skillid = (short)param2;
					int learnedindex = g_pLocalUser->GetLearnedSkillIndex(skillid);

					skillctrlreq.howto.S_Passive_OnOff.sSkillID = skillid;
					skillctrlreq.howto.S_Passive_OnOff.ucLearnSkillIndex = learnedindex;
					skillctrlreq.howto.S_Passive_OnOff.cSelectStep = g_pLocalUser->m_SkillList[learnedindex].m_cSelectedSkillLevel;

					if(g_pLocalUser->m_SkillList[learnedindex].m_cOnOff == 1)
						skillctrlreq.howto.S_Passive_OnOff.cOnOff = 0;
					else if(g_pLocalUser->m_SkillList[learnedindex].m_cOnOff == 0)
						skillctrlreq.howto.S_Passive_OnOff.cOnOff = 1;

					g_pLocalUser->m_bWaitPassiveReq = TRUE;
				}
				break;
				
			default :
				break;
			}
			
			return _XSendPacket_Encapsulate( (char*)&skillctrlreq );
		}
		break;

		// =================================================================================
		// ���� ó�� ����
	case MSG_NO_CHARAC_SELECT_MODE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Charac Select mode");
			
			int size = sizeof(MSG_CHARAC_SELECT_MODE);
			MSG_CHARAC_SELECT_MODE characselectmode;
			ZeroMemory(&characselectmode, size);
			characselectmode.header.sLength = size;
			characselectmode.ucMessage = MSG_NO_CHARAC_SELECT_MODE;
			
			characselectmode.cMode = (char)param1;
			characselectmode.cSelectPosition = 0;
			
			return _XSendPacket_Encapsulate( (char*)&characselectmode );
		}
		break;

	case MSG_NO_CHARAC_REBIRTH_TARGET :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Charac Rebirth Target");
			
			int size = sizeof(MSG_CHARAC_REBIRTH_TARGET_REQ);
			MSG_CHARAC_REBIRTH_TARGET_REQ characrebirthtargetreq;
			ZeroMemory(&characrebirthtargetreq, size);
			characrebirthtargetreq.header.sLength = size;
			characrebirthtargetreq.ucMessage = MSG_NO_CHARAC_REBIRTH_TARGET;
			
			_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
			if(pUser)
			{
				characrebirthtargetreq.target = pUser->m_Uniqueid;
			}
			characrebirthtargetreq.cFlag = (char)param1;
			
			// flag �� ���� ó�� �ʿ�
			characrebirthtargetreq.cIndex = 0;

			return _XSendPacket_Encapsulate( (char*)&characrebirthtargetreq );			
		}
		break;

	case MSG_NO_SAVE_RESPAWN_POS_REQUEST :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Save Respawn Pos Request");

			int size = sizeof(MSG_SAVE_RESPAWN_POS_REQUEST);
			MSG_SAVE_RESPAWN_POS_REQUEST respawnposrequest;
			ZeroMemory(&respawnposrequest, size);
			respawnposrequest.header.sLength = size;
			respawnposrequest.ucMessage = MSG_NO_SAVE_RESPAWN_POS_REQUEST;

			respawnposrequest.usNPCID = (unsigned short)param1;

			return _XSendPacket_Encapsulate( (char*)&respawnposrequest );
		}
		break;

	case MSG_NO_STATE_CHANGE_REQUEST :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : State Change Request");

			switch(param1)
			{
			case state_change_en_saferegion_req :
				{
					int size = sizeof(MSG_STATE_CHANGE_SAFEREGION_REQUEST);
					MSG_STATE_CHANGE_SAFEREGION_REQUEST statechangerequest;
					ZeroMemory(&statechangerequest, size);
					statechangerequest.header.sLength = size;
					statechangerequest.ucMessage = MSG_NO_STATE_CHANGE_REQUEST;

					statechangerequest.ucMode = (unsigned char)param1;
					statechangerequest.ucSafeRegionState = (unsigned char)param2;

					return _XSendPacket_Encapsulate( (char*)&statechangerequest );
				}
				break;
			case state_change_en_to_normal_state :
			case state_change_en_to_absent_state :
				{
					int size = sizeof(MSG_STATE_CHANGE_ABSENT);
					MSG_STATE_CHANGE_ABSENT statechange;
					ZeroMemory(&statechange, size);
					statechange.header.sLength = size;
					statechange.ucMessage = MSG_NO_STATE_CHANGE_REQUEST;

					statechange.ucMode = (unsigned char)param1;
					statechange.usUniqueID = g_pLocalUser->m_Uniqueid;

					return _XSendPacket_Encapsulate( (char*)&statechange );
				}
				break;
			default :
				{
					int size = sizeof(MSG_STATE_CHANGE_REQUEST);
					MSG_STATE_CHANGE_REQUEST statechangerequest;
					ZeroMemory(&statechangerequest, size);
					statechangerequest.header.sLength = size;
					statechangerequest.ucMessage = MSG_NO_STATE_CHANGE_REQUEST;

					statechangerequest.ucMode = (unsigned char)param1;
					if( param2 > 0 )
					{
						statechangerequest.cMapID = (char)param2;
						statechangerequest.cAreaCategory = (char)param3;
						statechangerequest.cAreaID = (char)param4;
					}

					return _XSendPacket_Encapsulate( (char*)&statechangerequest );
				}
				break;
			}
		}
		break;

	// ==========================================================================
	// Stance motion change
	case MSG_NO_STANCE_MOTION_CHANGE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Stance motion change");

			int size = sizeof(MSG_STANCE_MOTION_CHANGE);
			MSG_STANCE_MOTION_CHANGE stancemotionchange;
			ZeroMemory(&stancemotionchange, size);
			stancemotionchange.header.sLength = size;
			stancemotionchange.ucMessage = MSG_NO_STANCE_MOTION_CHANGE;

			stancemotionchange.usID = g_pLocalUser->m_Uniqueid;
			stancemotionchange.sStanceMotion = (short)param1;

			return _XSendPacket_Encapsulate( (char*)&stancemotionchange );
		}
		break;
		
	//===============================================================================================================		
	// ������ ��� ����
	case MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S : // ������ ���� ��û
	case MSG_NO_EX_BATTLE_ENTER_REQ_C_S : // �ʴ���� ĳ���Ͱ� �����濡 �����Ҷ�..
	case MSG_NO_EX_BATTLE_LEAVE_REQ_C_S : // �濡 �ִ� ĳ���Ͱ� ������..
	case MSG_NO_EX_BATTLE_PLAY_VIDEO : // �÷��� ���� ��ŵ�ϰų� ����մϴ�
	case MSG_NO_EX_BATTLE_TYPE_INFO_REQ_C_S : // �� Ÿ�Կ� ���� ������ ���´�
		{
			if(!Send_MSGBoss(type, param1, param2, param3))
				return FALSE;
		}
		break;
		
		
	//===============================================================================================================		
	// ��ȣ ��� ����
	case MSG_NO_NIC_LIST_REQ_C_S : // ��ȣ ����Ʈ�� ��û
	case MSG_NO_NIC_SELECT_REQ_C_S : // ��ȣ�� �����Ѵ�.
	case MSG_NO_NIC_DELETE_REQ_C_S : // ��ȣ �� ����
	case MSG_NO_RANK_HIDING :
	case MSG_NO_HONOR_PLAYER :
		{
			if(!Send_MSGNickName(type, param1, param2, param3))
				return FALSE;
		}
		break;	

	//===============================================================================================================		
	// ���� ����
	case MSG_NO_COLLECT_REQ : // ��ǰ Ȥ�� ����ǰ ��ȯ��û
		{
			if(!Send_MSGCollection(type, param1, param2, param3, param4))
				return FALSE;
		}
		break;
		
		//===============================================================================================================		
		// ��� ���� ���� �ִ� ������Ʈ ����
	case MSG_NO_OBJECT_INFO_REQUEST :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Object Detail info Request");

			if(param1 == 0)		// ����, ������ ����
			{/*
				// sega
				int size = sizeof(MSG_SEGA_INFO_REQUEST);
				MSG_SEGA_INFO_REQUEST segainfo;
				ZeroMemory(&segainfo, size);
				segainfo.header.sLength = size;
				segainfo.ucMessage = MSG_NO_OBJECT_INFO_REQUEST;

				segainfo.cObjectType = en_object_type_sega;
				segainfo.usID = (unsigned short)param2;

				return _XSendPacket_Encapsulate( (char*)&segainfo );*/

			}
			else if(param1 == 1)	// �� ����� ����
			{/*
				// match flag
				int size = sizeof(MSG_MATCH_FLAG_INFO_REQUEST);
				MSG_MATCH_FLAG_INFO_REQUEST matchflagreq;
				ZeroMemory(&matchflagreq, size);
				matchflagreq.header.sLength = size;
				matchflagreq.ucMessage = MSG_NO_OBJECT_INFO_REQUEST;

				matchflagreq.cObjectType = en_object_type_match_flag;
				matchflagreq.usID = (unsigned short)param2;

				g_pLocalUser->m_bWaitFlagInfo = TRUE;

				return _XSendPacket_Encapsulate( (char*)&matchflagreq );*/
			}
			else if( param1 == 2 ) // ĳ���� ���� ����
			{
				int size = sizeof(MSG_CHARAC_MORE_INFO_REQ);
				MSG_CHARAC_MORE_INFO_REQ characmorereq;
				ZeroMemory(&characmorereq, size);
				characmorereq.header.sLength = size;
				characmorereq.ucMessage = MSG_NO_OBJECT_INFO_REQUEST;
				characmorereq.cObjectType = em_object_type_simple_info;
				//_XUser* pUser = (_XUser*)g_pLocalUser->m_ViewTarget;
				//if(pUser)
				//{
				//	strcpy( characmorereq.cTo_or_From_Name, pUser->m_CharacterInfo.charactername );
				//}
				_XWindow_TargetInfo* targetinfo_window = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);
				strcpy( characmorereq.cTo_or_From_Name, targetinfo_window->m_SendName );
				memset( targetinfo_window->m_SendName, 0, sizeof(targetinfo_window->m_SendName));
				
				characmorereq.cFlag = (char)param2 ;

				return _XSendPacket_Encapsulate( (char*)&characmorereq );
			}
			else if( param1 == 3 ) // ������ ����
			{
				int size = sizeof(MSG_CHARAC_MORE_INFO_REQ);
				MSG_CHARAC_MORE_INFO_REQ characmorereq;
				ZeroMemory(&characmorereq, size);
				characmorereq.header.sLength = size;
				characmorereq.ucMessage = MSG_NO_OBJECT_INFO_REQUEST;
				characmorereq.cObjectType = en_object_type_items_info;
				characmorereq.cFlag = (char)param2 ;
				_XWindow_TargetInfo* targetinfo_window = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);
				if( targetinfo_window )					
				{
					targetinfo_window->m_TargetInfoModeToRequest = tm_items_info;
					if( strlen( targetinfo_window->m_SendTempName) )
					{
						strcpy( characmorereq.cTo_or_From_Name, targetinfo_window->m_SendTempName );
						memset( targetinfo_window->m_SendTempName, 0, sizeof(TCHAR)*13);
					}
					else
					{
						strcpy( characmorereq.cTo_or_From_Name, targetinfo_window->m_SendName );
						if( characmorereq.cFlag != MSG_CHARAC_MORE_INFO_REQ::_request &&
							characmorereq.cFlag != MSG_CHARAC_MORE_INFO_REQ::_response_no )
							memset( targetinfo_window->m_SendName, 0, sizeof(TCHAR)*13);
					}
				}
				
				return _XSendPacket_Encapsulate( (char*)&characmorereq );
			}
			else if( param1 == 4 ) // �ɷ�ġ ����
			{
				int size = sizeof(MSG_CHARAC_MORE_INFO_REQ);
				MSG_CHARAC_MORE_INFO_REQ characmorereq;
				ZeroMemory(&characmorereq, size);
				characmorereq.header.sLength = size;
				characmorereq.ucMessage = MSG_NO_OBJECT_INFO_REQUEST;
				characmorereq.cObjectType = en_object_type_ability_info;
				characmorereq.cFlag = (char)param2 ;
				_XWindow_TargetInfo* targetinfo_window = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);
				if( targetinfo_window )
				{
					targetinfo_window->m_TargetInfoModeToRequest = tm_ability_info;
					if( strlen( targetinfo_window->m_SendTempName) )
					{
						strcpy( characmorereq.cTo_or_From_Name, targetinfo_window->m_SendTempName );
						memset( targetinfo_window->m_SendTempName, 0, sizeof(TCHAR)*13);
					}
					else
					{
						strcpy( characmorereq.cTo_or_From_Name, targetinfo_window->m_SendName );
						if( characmorereq.cFlag != MSG_CHARAC_MORE_INFO_REQ::_request &&
							characmorereq.cFlag != MSG_CHARAC_MORE_INFO_REQ::_response_no )
							memset( targetinfo_window->m_SendName, 0, sizeof(TCHAR)*13);
					}
				}				

				return _XSendPacket_Encapsulate( (char*)&characmorereq );
			}
			else if( param1 == 5 ) // ��ü ����
			{
				int size = sizeof(MSG_CHARAC_MORE_INFO_REQ);
				MSG_CHARAC_MORE_INFO_REQ characmorereq;
				ZeroMemory(&characmorereq, size);
				characmorereq.header.sLength = size;
				characmorereq.ucMessage = MSG_NO_OBJECT_INFO_REQUEST;
				characmorereq.cObjectType = en_object_type_all_info;
				characmorereq.cFlag = (char)param2 ;
				_XWindow_TargetInfo* targetinfo_window = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);
				if( targetinfo_window )
				{
					targetinfo_window->m_TargetInfoModeToRequest = tm_all_info;
					if( strlen( targetinfo_window->m_SendTempName) )
					{
						strcpy( characmorereq.cTo_or_From_Name, targetinfo_window->m_SendTempName );
						memset( targetinfo_window->m_SendTempName, 0, sizeof(TCHAR)*13);
					}
					else
					{
						strcpy( characmorereq.cTo_or_From_Name, targetinfo_window->m_SendName );
						if( characmorereq.cFlag != MSG_CHARAC_MORE_INFO_REQ::_request &&
							characmorereq.cFlag != MSG_CHARAC_MORE_INFO_REQ::_response_no )
							memset( targetinfo_window->m_SendName, 0, sizeof(TCHAR)*13);
					}
				}

				return _XSendPacket_Encapsulate( (char*)&characmorereq );
			}
		}
		break;
		// ���� ȿ�� ����
	case MSG_NO_EFFECT_CTRL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Effect Ctrl");

			if(!Send_MSGEffectCtrl(type, param1, param2, param3))
			{
				ProcessNetworkError();
				return FALSE;
			}
		}
		break;

		// �����͸� ����Ʈ ���
	case MSG_NO_MASTERY_POINT_USE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Mastery point use");

			int size = sizeof(MSG_MASTERY_POINT_USE);
			MSG_MASTERY_POINT_USE masterypointuse;
			ZeroMemory(&masterypointuse, size);
			masterypointuse.header.sLength = size;
			masterypointuse.ucMessage = MSG_NO_MASTERY_POINT_USE;

			masterypointuse.ucPageNo = (unsigned char)param1;
			masterypointuse.sMasteryID = (short)param2;

			return _XSendPacket_Encapsulate( (char*)&masterypointuse );
		}
		break;

		// Speed Hack Check
	case MSG_NO_TIME_CHECK :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Time Check");

#ifdef _XDEF_SERVERTIMECHECK_071112_KERYGMA
			
			int size = sizeof(MSG_TIME_CHECK);
			MSG_TIME_CHECK timecheck;
			ZeroMemory(&timecheck, size);
			timecheck.header.sLength = size;
			timecheck.ucMessage = MSG_NO_TIME_CHECK;

			if( param1 == 0 ) // time packet
			{
				timecheck.ucMode = param1;
				
				timecheck.time = g_ServerTimeCode;
				g_SendTimerPacketTime = g_LocalSystemTime;

				_XLog("Server Time : %d", g_ServerTimeCode);
			}
			else if( param1 == 1 ) // hack tool report.
			{
				timecheck.ucMode = param1;
				timecheck.time = g_ServerTimeCode;
				strcpy( timecheck.prog1, g_szLastUsedCheatEngine );
			}			

			return _XSendPacket_Encapsulate( (char*)&timecheck );

#else
			int size = sizeof(MSG_TIME_CHECK);
			MSG_TIME_CHECK timecheck;
			ZeroMemory(&timecheck, size);
			timecheck.header.sLength = size;
			timecheck.ucMessage = MSG_NO_TIME_CHECK;

			timecheck.time = timeGetTime();
			g_SendTimerPacketTime = g_LocalSystemTime;

			return _XSendPacket_Encapsulate( (char*)&timecheck );
#endif
		}
		break;

		// Monster ���� ȿ��
	case MSG_NO_REQ_SPELLEFFECT :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Monster SpellList REQ");

			int size = sizeof(MSG_REQ_SPELLEFFECT);
			MSG_REQ_SPELLEFFECT spelleffect;
			ZeroMemory(&spelleffect, size);
			spelleffect.header.sLength = size;
			spelleffect.ucMessage = MSG_NO_REQ_SPELLEFFECT;

			spelleffect.usMobID = (unsigned short)param1;

			return _XSendPacket_Encapsulate( (char*)&spelleffect );
		}
		break;

	// ���� ���� ��û ��Ŷ ----------------------------------------------------------------=	
	case MSG_NO_ORGANIZATION_CREATE_REQ :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Organization Create REQ");

			int size = sizeof(MSG_ORGANIZATION_CREATE_REQ);
			MSG_ORGANIZATION_CREATE_REQ organizationcreate;
			ZeroMemory(&organizationcreate, size);
			organizationcreate.header.sLength = size;
			organizationcreate.ucMessage = MSG_NO_ORGANIZATION_CREATE_REQ;
			
			_XWindow_NPCScript* pNpcScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
			if( pNpcScript_Window )
			{
				memset( organizationcreate.cOrganazationName, 0, sizeof(char)*(_XDEF_ORGANIZATIONNAMELENGTH+1) );
				strcpy( organizationcreate.cOrganazationName, pNpcScript_Window->m_OrganizationNameIME.GetText() );
				pNpcScript_Window->m_OrganizationNameIME.ClearBuffer();
				_XIMEKERNEL.SetFocus( NULL );
				_XIMEKERNEL.ResetCombinationBuffer();
			}			

			return _XSendPacket_Encapsulate( (char*)&organizationcreate );
		}
		break;
	case MSG_NO_ORGANIZATION_UNIT_UPDATE_ :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Organization Unit Update REQ");
			
			int size = sizeof(MSG_ORGANIZATION_UNIT_UPDATE_);
			MSG_ORGANIZATION_UNIT_UPDATE_ organizationupdate;
			ZeroMemory(&organizationupdate, size);
			organizationupdate.header.sLength = size;
			organizationupdate.ucMessage = MSG_NO_ORGANIZATION_UNIT_UPDATE_;

			organizationupdate._unit = *g_pGroup_Window->GetUpdateUnit();
			organizationupdate.iCode = (int)param1;

			return _XSendPacket_Encapsulate( (char*)&organizationupdate );
		}
		break;
	case MSG_NO_ORGANIZATION_INFO_REQ :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Organization Info REQ");

			int size = sizeof(MSG_ORGANIZATION_INFO_REQ);
			MSG_ORGANIZATION_INFO_REQ organizationinfo;
			ZeroMemory(&organizationinfo, size);
			organizationinfo.header.sLength = size;
			organizationinfo.ucMessage = MSG_NO_ORGANIZATION_INFO_REQ;

			organizationinfo.cCode = (char)param1;
			if( organizationinfo.cCode == MSG_ORGANIZATION_INFO_REQ::_REQ_MY_OR_INFO_ )
				organizationinfo.index = g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex;
			else if( organizationinfo.cCode == MSG_ORGANIZATION_INFO_REQ::_REQ_OR_GLOBAL_INFO )
				organizationinfo.index = (int)param2;
			return _XSendPacket_Encapsulate( (char*)&organizationinfo );
		}
		break;
	case MSG_NO_ORGANIZATION_CLIENT :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Organization Client REQ");

			int size = sizeof(MSG_ORGANIZATION_CLIENT_);
			MSG_ORGANIZATION_CLIENT_ organizationclient;
			ZeroMemory(&organizationclient, size);
			organizationclient.header.sLength = size;
			organizationclient.ucMessage = MSG_NO_ORGANIZATION_CLIENT;
			organizationclient.iCode = (int)param1;			
			strcpy( organizationclient.cName, g_pGroup_Window->GetSendTargetName() );
			g_pGroup_Window->ResetSendTargetName();

			return _XSendPacket_Encapsulate( (char*)&organizationclient );
		}
		break;
	case MSG_NO_ORGANIZATION_NOTICE_REQ :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Organization Notice REQ");

			int size = sizeof(MSG_ORGANIZATION_NOTICE_REQ);
			MSG_ORGANIZATION_NOTICE_REQ organizationnoticereq;
			ZeroMemory(&organizationnoticereq, size);

			organizationnoticereq.header.sLength = size;
			organizationnoticereq.ucMessage = MSG_NO_ORGANIZATION_NOTICE_REQ;
			organizationnoticereq.m_seqNum = (int)param1;
			organizationnoticereq.m_iIndex = g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex;

			if( organizationnoticereq.m_seqNum == 0 )
			{
				memcpy( organizationnoticereq.m_NoticeData.writer, g_pGroup_Window->m_SendNoticeTempText.writer, 13 );
				memcpy( organizationnoticereq.m_NoticeData.writedate, g_pGroup_Window->m_SendNoticeTempText.writedate, 15 );
				memcpy( organizationnoticereq.m_NoticeData.notice, g_pGroup_Window->m_SendNoticeTempText.notice, 72 );
			}
			else
			{
				int index = 72 + (organizationnoticereq.m_seqNum-1)*100;
				memcpy( organizationnoticereq.m_cNotice, &g_pGroup_Window->m_SendNoticeTempText.notice[index], 100 );
			}			
			
			if( g_pGroup_Window->m_NoticeIMEControl.GetShowStatus() )
				g_pGroup_Window->m_NoticeIMEControl.ShowWindow(FALSE);

			return _XSendPacket_Encapsulate( (char*)&organizationnoticereq );
		}
		break;
		// ==========================================================================
		// MINISTOP EVENT ����	
	case MSG_NO_EVENT_REQUEST :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : EVENT ");
#ifdef _XDEF_EVENTWINDOW
			_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
			if( pEvent_Window ) 
			{
				int size = sizeof(MSG_EVENT_REQ_WITH_KEY);
				MSG_EVENT_REQ_WITH_KEY preorderevent;
				ZeroMemory(&preorderevent, size);
				
				preorderevent.header.sLength = size;
				preorderevent.ucMessage = MSG_NO_EVENT_REQUEST;
				
				// �̺�Ʈ ���̵� �ִ´�.
				preorderevent.usEventID = pEvent_Window->m_SelectedEventID;	// ���ȭ �̺�Ʈ�� ���̵�� 3�̴�.
				
				// �̺�Ʈ �ڵ带 �ִ´�.
				memset( preorderevent.cKey, 0, sizeof(char)*_EVENT_KEY_MAX_LENGTH_ );

				for( int i = 0; i < 5; ++i )
				{
					if( pEvent_Window->m_SelectedMaxSerialNumber[i] > 0 )
					{
						strncat(preorderevent.cKey, pEvent_Window->m_SerialNumber[i], sizeof(char)*pEvent_Window->m_SelectedMaxSerialNumber[i]);
					}
				}
				pEvent_Window->m_btnSerialNumberOK->EnableWindow(FALSE);
				_XIMEKERNEL.SetFocus( NULL );	
				_XIMEKERNEL.ResetCombinationBuffer();			
				
				return _XSendPacket_Encapsulate( (char*)&preorderevent);
			}

#else
			_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
			if( pNPCScript_Window )
			{	
				switch(pNPCScript_Window->m_EventType) {
				case 0: // ����� ���� �̺�Ʈ
					{
						int size = sizeof(MSG_MINISTOP_EVENT_REQ);
						MSG_MINISTOP_EVENT_REQ ministopevent;
						ZeroMemory(&ministopevent, size);
						
						ministopevent.header.sLength = size;
						ministopevent.ucMessage = MSG_NO_EVENT_REQUEST;

						// �̺�Ʈ ���̵� �ִ´�.
						ministopevent.usEventID = pNPCScript_Window->m_EventType+1;
						pNPCScript_Window->m_EventType = -1;
						
						// �̺�Ʈ �ڵ带 �ִ´�.
						memset( ministopevent.cKey, 0, sizeof(char)*16 );
						strncpy(ministopevent.cKey ,pNPCScript_Window->m_MiniStopEventIMEControl.GetText(),sizeof(char)*16);
						pNPCScript_Window->m_MiniStopEventIMEControl.ShowWindow(FALSE);
						pNPCScript_Window->m_MiniStopEventIMEControl.ClearBuffer();
						_XIMEKERNEL.SetFocus( NULL );	
						_XIMEKERNEL.ResetCombinationBuffer();			
						
						return _XSendPacket_Encapsulate( (char*)&ministopevent );
					}
					break;
				case 1://�ѹи���Ʈ - ������ �̺�Ʈ
					{
						int size = sizeof(MSG_FAMILYMART_2_EVENT_REQ);
						MSG_FAMILYMART_2_EVENT_REQ familymartevent;
						ZeroMemory(&familymartevent, size);
						
						familymartevent.header.sLength = size;
						familymartevent.ucMessage = MSG_NO_EVENT_REQUEST;
						
						// �̺�Ʈ ���̵� �ִ´�.
						familymartevent.usEventID = pNPCScript_Window->m_EventType+1;
						pNPCScript_Window->m_EventType = -1;
						
						// �̺�Ʈ �ڵ带 �ִ´�.
						memset( familymartevent.cKey, 0, sizeof(char)*9 );
						strncpy(familymartevent.cKey ,pNPCScript_Window->m_MiniStopEventIMEControl.GetText(),sizeof(char)*8);//�������� NULL�̾�� �Ѵ�.
						pNPCScript_Window->m_MiniStopEventIMEControl.ShowWindow(FALSE);
						pNPCScript_Window->m_MiniStopEventIMEControl.ClearBuffer();
						_XIMEKERNEL.SetFocus( NULL );	
						_XIMEKERNEL.ResetCombinationBuffer();			
							
						return _XSendPacket_Encapsulate( (char*)&familymartevent );
					}
					break;
				case 2:// ���డ�� �̺�Ʈ 
					{
						int size = sizeof(MSG_EVENT_REQ_WITH_KEY);
						MSG_EVENT_REQ_WITH_KEY preorderevent;
						ZeroMemory(&preorderevent, size);
						
						preorderevent.header.sLength = size;
						preorderevent.ucMessage = MSG_NO_EVENT_REQUEST;
						
						// �̺�Ʈ ���̵� �ִ´�.
						preorderevent.usEventID = 10;	// ���డ�� �̺�Ʈ�� ���̵� 10�̴�.
						pNPCScript_Window->m_EventType = -1;
						
						// �̺�Ʈ �ڵ带 �ִ´�.
						memset( preorderevent.cKey, 0, sizeof(char)*_EVENT_KEY_MAX_LENGTH_ );
						strncat(preorderevent.cKey, pNPCScript_Window->m_EventCodeString01, sizeof(char)*5);
						strncat(preorderevent.cKey, pNPCScript_Window->m_EventCodeString02, sizeof(char)*5);
						strncat(preorderevent.cKey, pNPCScript_Window->m_EventCodeString03, sizeof(char)*5);
						pNPCScript_Window->m_MiniStopEventIMEControl.ShowWindow(FALSE);
						pNPCScript_Window->m_MiniStopEventIMEControl.ClearBuffer();
						_XIMEKERNEL.SetFocus( NULL );	
						_XIMEKERNEL.ResetCombinationBuffer();			
						
						return _XSendPacket_Encapsulate( (char*)&preorderevent);
					}
					break;
				case 3:// ���ȭ �̺�Ʈ 
					{
						int size = sizeof(MSG_EVENT_REQ_WITH_KEY);
						MSG_EVENT_REQ_WITH_KEY preorderevent;
						ZeroMemory(&preorderevent, size);
						
						preorderevent.header.sLength = size;
						preorderevent.ucMessage = MSG_NO_EVENT_REQUEST;
						
						// �̺�Ʈ ���̵� �ִ´�.
						preorderevent.usEventID = 3;	// ���ȭ �̺�Ʈ�� ���̵�� 3�̴�.
						pNPCScript_Window->m_EventType = -1;
						
						// �̺�Ʈ �ڵ带 �ִ´�.
						memset( preorderevent.cKey, 0, sizeof(char)*_EVENT_KEY_MAX_LENGTH_ );
						strncat(preorderevent.cKey, pNPCScript_Window->m_EventCodeString01, sizeof(char)*5);
						strncat(preorderevent.cKey, pNPCScript_Window->m_EventCodeString02, sizeof(char)*5);
						strncat(preorderevent.cKey, pNPCScript_Window->m_EventCodeString03, sizeof(char)*5);
						pNPCScript_Window->m_MiniStopEventIMEControl.ShowWindow(FALSE);
						pNPCScript_Window->m_MiniStopEventIMEControl.ClearBuffer();
						_XIMEKERNEL.SetFocus( NULL );	
						_XIMEKERNEL.ResetCombinationBuffer();			
						
						return _XSendPacket_Encapsulate( (char*)&preorderevent);
					}
					break;
				}
			}

#endif
			
		}
		break;
		//Author : ����� //breif : ���� �̺�Ʈ //Date : 08/03/19
	case MSG_NO_PRESENT :
		{
			int size = sizeof(MSG_PRESENT_REQ_PRESENT);
			MSG_PRESENT_REQ_PRESENT PresentReq;
			ZeroMemory(&PresentReq, size);

			PresentReq.header.sLength = size;
			PresentReq.ucMessage = MSG_NO_PRESENT;
			PresentReq.ucMode = (unsigned char)en_present_mode_req_present;

			_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
			PresentReq.sEventID = pEvent_Window->m_SelectedEventID;

			return _XSendPacket_Encapsulate( (char*)&PresentReq);
		}
		break;

		// ==========================================================================
		// Acclaim ���� ���� Packet		
	case MSG_NO_US_AD_REWARD_REQ :
		{
#ifdef _ACCLAIM_IGAADSYSTEM
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Acclaim AD Reward REQ");

			int size = sizeof(MSG_US_AD_REWARD_REQ);
			MSG_US_AD_REWARD_REQ us_ad_reward_req;
			ZeroMemory(&us_ad_reward_req, size);

			us_ad_reward_req.header.sLength = size;
			us_ad_reward_req.ucMessage = MSG_NO_US_AD_REWARD_REQ;				
			us_ad_reward_req.Type = (US_AD_REWARD_TYPE)param1;

			return _XSendPacket_Encapsulate( (char*)&us_ad_reward_req );
#endif
		}
		break;
		// ==========================================================================
		// �� ����
	case MSG_NO_MATCH_LIST :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Match List");

			if(!Send_MatchList(type, param1, param2, param3, param4))
			{
				ProcessNetworkError();
				return FALSE;
			}
		}
		break;
	case MSG_NO_MATCH_READY :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Match Ready");

			if(!Send_MatchReady(type, param1, param2, param3, param4))
			{
				ProcessNetworkError();
				return FALSE;
			}
		}
		break;
	case MSG_NO_MATCH_CTRL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Match Ctrl");

			if(param1 == en_match_ctrl_request_exit)
			{
				int size = sizeof(MSG_MATCH_CTRL_REQUEST_EXIT);
				MSG_MATCH_CTRL_REQUEST_EXIT matchctrl;
				ZeroMemory(&matchctrl, size);

				matchctrl.header.sLength = size;
				matchctrl.ucMessage =	MSG_NO_MATCH_CTRL;
				matchctrl.ucMode = (unsigned char)param1;

				return _XSendPacket_Encapsulate((char*)&matchctrl);
			}
		}
		break;
	case MSG_NO_MATCH_NOTICE :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Match Ctrl");
			
			if(param1 == en_match_ntc_inform)
			{
				int size = sizeof(MSG_MATCH_NOTICE_INFORM);
				MSG_MATCH_NOTICE_INFORM matchpr;
				ZeroMemory(&matchpr, size);
				
				matchpr.header.sLength = size;
				matchpr.ucMessage =	MSG_NO_MATCH_NOTICE;
				matchpr.ucMode = (unsigned char)param1;
				
				matchpr.usUniqueID = g_pLocalUser->m_Uniqueid;
				matchpr.ucLevel = g_pLocalUser->m_CharacterInfo.Get_innerlevel();
				strcpy( matchpr.cName , g_pLocalUser->m_CharacterInfo.charactername );
				strcpy( matchpr.cInform , g_pLocalUser->m_stringMatchPR );

				VIEWPACKETLOG
					_XDWPACKETLOG("SEND PACKET : Match Ctrl Inform mode[%d]", matchpr.ucMode);

				return _XSendPacket_Encapsulate((char*)&matchpr);
			}
			else if( param1 == en_match_ntc_inform_end )
			{
				int size = sizeof(MSG_MATCH_NOTICE_INFORM_END);
				MSG_MATCH_NOTICE_INFORM_END matchprend;
				ZeroMemory(&matchprend, size);
				
				matchprend.header.sLength = size;
				matchprend.ucMessage =	MSG_NO_MATCH_NOTICE;
				matchprend.ucMode = (unsigned char)param1;
				
				matchprend.usUniqueID = g_pLocalUser->m_Uniqueid;

				VIEWPACKETLOG
					_XDWPACKETLOG("SEND PACKET : Match Ctrl End mode [%d]", matchprend.ucMode);
				
				return _XSendPacket_Encapsulate((char*)&matchprend);
			}
		}
		break;

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
	case MSG_NO_FREE_MATCH :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Free Match");

			if(!Send_FreeMatch(type, param1, param2, param3, param4))
			{
				ProcessNetworkError();
				return FALSE;
			}
		}
		break;
#endif

	case MSG_NO_INCHANT_CTRL :	// ���� ����
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Inchant Ctrl");

#ifdef _XDEF_SMELT_ITEM //Author : �����
			switch( (unsigned char)param1 )
			{
			case inchant_mode_str_req : // ����
				{
					int size = sizeof(MSG_INCHANT_STR_REQ);
					MSG_INCHANT_STR_REQ inchantreq;
					ZeroMemory(&inchantreq, size);
					inchantreq.header.sLength = size;
					inchantreq.ucMessage = MSG_NO_INCHANT_CTRL;
					inchantreq.ucMode = (unsigned char)param1;

					inchantreq.cTargetWeaponSlot = g_pInventory_Window->m_UsedLifeItemIndex;
					inchantreq.cNeedItemSlot = g_pInventory_Window->m_iSocketIndexList[0];
					inchantreq.ucNeedItemCount = g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].ucCount;
					inchantreq.uiNeedMoney = g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].uiPrice;

					return _XSendPacket_Encapsulate((char*)&inchantreq);			
				}
				break;
			case inchant_mode_str_complete : // ���ü���
				{
					int size = sizeof(MSG_INCHANT_STR_REQ);
					MSG_INCHANT_STR_REQ inchantreq;
					ZeroMemory(&inchantreq, size);
					inchantreq.header.sLength = size;
					inchantreq.ucMessage = MSG_NO_INCHANT_CTRL;
					inchantreq.ucMode = (unsigned char)param1;

					inchantreq.cTargetWeaponSlot = g_pInventory_Window->m_UsedLifeItemIndex;
					inchantreq.cNeedItemSlot = g_pInventory_Window->m_iSocketIndexList[0];
					inchantreq.ucNeedItemCount = g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].ucCount;
					inchantreq.uiNeedMoney = g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].uiPrice;

					return _XSendPacket_Encapsulate((char*)&inchantreq);			
				}
				break;
			case inchant_mode_protect_add_count_req : // �븰 �ٸ��� ��û c->s
				{
					XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
					
					if( !pSmletItemWindow )
						return FALSE;

					int size = sizeof(MSG_INCHANT_PROTECT_ADD_COUNT_REQ);
					MSG_INCHANT_PROTECT_ADD_COUNT_REQ scalereq;
					ZeroMemory(&scalereq, size);
					scalereq.header.sLength = size;
					scalereq.ucMessage = MSG_NO_INCHANT_CTRL;
					scalereq.ucMode = (unsigned char)param1;

					scalereq.ucWeaponSlot = pSmletItemWindow->GetUserItemindex();
					scalereq.ucProtectItemSlot = pSmletItemWindow->GetSmeltItemindex();
					scalereq.ucUseCount = pSmletItemWindow->GetCount();	// ����

					return _XSendPacket_Encapsulate((char*)&scalereq);
				}
				break;
			case inchant_mode_protect_inc_str_req :	// ��� �ٸ��� ��û c->s
				{
					XWindow_SmeltItem* pSmletItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
					
					if( !pSmletItemWindow )
						return FALSE;

					int size = sizeof(MSG_INCHANT_PROTECT_INC_STR_REQ);
					MSG_INCHANT_PROTECT_INC_STR_REQ tearreq;
					ZeroMemory(&tearreq, size);
					tearreq.header.sLength = size;
					tearreq.ucMessage = MSG_NO_INCHANT_CTRL;
					tearreq.ucMode = (unsigned char)param1;

					tearreq.ucWeaponSlot = pSmletItemWindow->GetUserItemindex();
					tearreq.ucIncStrItemSlot = pSmletItemWindow->GetSmeltItemindex();

					return _XSendPacket_Encapsulate((char*)&tearreq);
				}
				break;
			case inchant_mode_combine_item_req :
				{
					_XWindow_MixWeapon* pMixWeapon_Window = (_XWindow_MixWeapon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MIXWEAPON_WINDOW);

					if(!pMixWeapon_Window)
						return FALSE;

					int size = sizeof(MSG_COMBINE_ITEM_REQ);
					MSG_COMBINE_ITEM_REQ combinereq;
					ZeroMemory(&combinereq, size);
					combinereq.header.sLength = size;
					combinereq.ucMessage = MSG_NO_INCHANT_CTRL;
					combinereq.ucMode = (unsigned char)param1;

					combinereq.ucWeaponSlot = pMixWeapon_Window->m_ItemSlot[0].nInvenNumber;
					combinereq.ucAddItem1Slot = pMixWeapon_Window->m_ItemSlot[1].nInvenNumber;
					combinereq.ucAddItem2Slot = pMixWeapon_Window->m_ItemSlot[2].nInvenNumber;

					combinereq.uiNeedMoney = pMixWeapon_Window->GetSelectedWeaponCost();

					return _XSendPacket_Encapsulate((char*)&combinereq);
				}
				break;
			default :
				{
					//�̰��� ���ͼ��� �ȵ�....
					int size = sizeof(MSG_INCHANT_STR_REQ);
					MSG_INCHANT_STR_REQ inchantreq;
					ZeroMemory(&inchantreq, size);
					inchantreq.header.sLength = size;
					inchantreq.ucMessage = MSG_NO_INCHANT_CTRL;
					inchantreq.ucMode = (unsigned char)param1;

					inchantreq.cTargetWeaponSlot = g_pInventory_Window->m_UsedLifeItemIndex;
					inchantreq.cNeedItemSlot = g_pInventory_Window->m_iSocketIndexList[0];
					inchantreq.ucNeedItemCount = g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].ucCount;
					inchantreq.uiNeedMoney = g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].uiPrice;

					return _XSendPacket_Encapsulate((char*)&inchantreq);
				}
				break;
			}
#else
			switch( (unsigned char)param1 )
			{
			case inchant_mode_str_req : // ����
				{
					int size = sizeof(MSG_INCHANT_STR_REQ);
					MSG_INCHANT_STR_REQ inchantreq;
					ZeroMemory(&inchantreq, size);
					inchantreq.header.sLength = size;
					inchantreq.ucMessage = MSG_NO_INCHANT_CTRL;
					inchantreq.ucMode = (unsigned char)param1;

					inchantreq.cTargetWeaponSlot = g_pInventory_Window->m_UsedLifeItemIndex;
					inchantreq.cNeedItemSlot = g_pInventory_Window->m_iSocketIndexList[0];
					inchantreq.ucNeedItemCount = g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].ucCount;
					inchantreq.uiNeedMoney = g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].uiPrice;

					return _XSendPacket_Encapsulate((char*)&inchantreq);			
				}
				break;
			case inchant_mode_str_complete : // ���ü���
				{
					int size = sizeof(MSG_INCHANT_STR_REQ);
					MSG_INCHANT_STR_REQ inchantreq;
					ZeroMemory(&inchantreq, size);
					inchantreq.header.sLength = size;
					inchantreq.ucMessage = MSG_NO_INCHANT_CTRL;
					inchantreq.ucMode = (unsigned char)param1;

					inchantreq.cTargetWeaponSlot = g_pInventory_Window->m_UsedLifeItemIndex;
					inchantreq.cNeedItemSlot = g_pInventory_Window->m_iSocketIndexList[0];
					inchantreq.ucNeedItemCount = g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].ucCount;
					inchantreq.uiNeedMoney = g_ItemInchantTable[g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex].Get_m_ucStrength()].uiPrice;

					return _XSendPacket_Encapsulate((char*)&inchantreq);			
				}
				break;
			case inchant_mode_combine_item_req :	//���� ���� ��û
				{
					_XWindow_MixWeapon* pMixWeapon_Window = (_XWindow_MixWeapon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MIXWEAPON_WINDOW);

					if(!pMixWeapon_Window)
						return FALSE;

					int size = sizeof(MSG_COMBINE_ITEM_REQ);
					MSG_COMBINE_ITEM_REQ combinereq;
					ZeroMemory(&combinereq, size);
					combinereq.header.sLength = size;
					combinereq.ucMessage = MSG_NO_INCHANT_CTRL;
					combinereq.ucMode = (unsigned char)param1;

					combinereq.ucWeaponSlot = pMixWeapon_Window->m_ItemSlot[0].nInvenNumber;
					combinereq.ucAddItem1Slot = pMixWeapon_Window->m_ItemSlot[1].nInvenNumber;
					combinereq.ucAddItem2Slot = pMixWeapon_Window->m_ItemSlot[2].nInvenNumber;

					combinereq.uiNeedMoney = pMixWeapon_Window->GetSelectedWeaponCost();

					return _XSendPacket_Encapsulate((char*)&combinereq);
				}
				break;
			}
			
#endif
		}
		break;
	case MSG_NO_NPC_BUFF_REQ :		// npc buff
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : NPC Buff Req");

			int size = sizeof(MSG_NPC_BUFF_REQ);
			MSG_NPC_BUFF_REQ npcbuffreq;
			ZeroMemory(&npcbuffreq, size);

			npcbuffreq.header.sLength = size;
			npcbuffreq.ucMessage = MSG_NO_NPC_BUFF_REQ;

			npcbuffreq.ucReqIndex = (unsigned char)param1;
			npcbuffreq.uiNeedMoney = (unsigned int)param2;

			return _XSendPacket_Encapsulate((char*)&npcbuffreq);
		}
		break;

	// ==========================================================================
	// ���� ����
	case MSG_NO_BLOODPOINT_CTRL :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Blood point request item");

			int size = sizeof(MSG_BLOODPOINT_REQEUST_ITEM);
			MSG_BLOODPOINT_REQEUST_ITEM reqitem;
			ZeroMemory(&reqitem, size);

			reqitem.header.sLength = size;
			reqitem.ucMessage = MSG_NO_BLOODPOINT_CTRL;

			reqitem.ucMode = (unsigned char)param1;
			reqitem.cInsertedSlot = param2;

			return _XSendPacket_Encapsulate((char*)&reqitem);
		}
		break;

#ifdef _XTS_NEWCHARACLIST // _XTS_SERVERUNIFICATION �� ������ �������� ĳ���� �����ɸ� �����.

		case MSG_NO_UNI_GROUP_LOBBY :
		{
			switch(param1) 
			{
				case en_ug_change_name_request :
				{					
					int size = sizeof(MSG_UG_CHANGE_NAME_REQUEST);
					MSG_UG_CHANGE_NAME_REQUEST changenamereq;
					ZeroMemory(&changenamereq, size);
					
					changenamereq.header.sLength = size;
					changenamereq.ucMessage =	MSG_NO_UNI_GROUP_LOBBY;
					changenamereq.ucMode = (unsigned char)param1;

					int selectedservernum = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );

					_XWindowManager* SelectCharacterWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.GetWindowManagerPtr();
					if( !SelectCharacterWindowMgr )
						return FALSE;
					
					_XWindow_ChangeName* pSelect_ChangeName_Window = (_XWindow_ChangeName*)SelectCharacterWindowMgr->FindWindow(_XDEF_WTITLE_CHANGENAMEWINDOW);
					if( !pSelect_ChangeName_Window )
						return FALSE;
						
					changenamereq.ucIndex = pSelect_ChangeName_Window->GetSelectedIndex();
					strcpy( changenamereq.ucNewName, pSelect_ChangeName_Window->GetChangedName());
					
					return _XSendPacket_Encapsulate((char*)&changenamereq);
				}
				break;
			}
		}
		break;
#else

#ifdef _XTS_SERVERUNIFICATION
	case MSG_NO_UNI_GROUP_LOBBY :
		{
			_XWindowManager* ServerUnificationWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_serverunification.GetWindowManagerPtr();
			if( !ServerUnificationWindowMgr )
				return FALSE;				
			
			_XWindowManager* SelectCharacterWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.GetWindowManagerPtr();
			if( !SelectCharacterWindowMgr )
				return FALSE;
			
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Server Unification Req");

			switch(param1) 
			{
			case en_ug_req_simple_grp_info :
				{
					int size = sizeof(MSG_UG_REQ_SIMPLE_GRP_INFO);
					MSG_UG_REQ_SIMPLE_GRP_INFO simplereq;
					ZeroMemory(&simplereq, size);

					simplereq.header.sLength = size;
					simplereq.ucMessage = MSG_NO_UNI_GROUP_LOBBY;
					simplereq.ucMode = (unsigned char)param1;
					
					return _XSendPacket_Encapsulate((char*)&simplereq);
				}
				break;
			case en_ug_select_simple_grp :
				{
					_XWindow_ServerUnification* pServerUnification_Window = (_XWindow_ServerUnification*)ServerUnificationWindowMgr->FindWindow(_XDEF_WTITLE_SERVERUNIFICATIONWINDOW);
					if( !pServerUnification_Window )
						return FALSE;
					
					int size = sizeof(MSG_UG_SELECT_SIMPLE_GRP);
					MSG_UG_SELECT_SIMPLE_GRP simplegrp;
					ZeroMemory(&simplegrp, size);
					
					simplegrp.header.sLength = size;
					simplegrp.ucMessage =	MSG_NO_UNI_GROUP_LOBBY;
					simplegrp.ucMode = (unsigned char)param1;
					simplegrp.ucSelectNewGroupNo = pServerUnification_Window->GetSelectServerIndex();

					return _XSendPacket_Encapsulate((char*)&simplegrp);
				}
				break;
			case en_ug_move_request :
				{
					_XWindow_ServerUnification* pServerUnification_Window = (_XWindow_ServerUnification*)ServerUnificationWindowMgr->FindWindow(_XDEF_WTITLE_SERVERUNIFICATIONWINDOW);
					if( !pServerUnification_Window )
						return FALSE;					
					
					int size = sizeof(MSG_UG_MOVE_REQUEST);
					MSG_UG_MOVE_REQUEST movereq;
					ZeroMemory(&movereq, size);
					
					movereq.header.sLength = size;
					movereq.ucMessage =	MSG_NO_UNI_GROUP_LOBBY;
					movereq.ucMode = (unsigned char)param1;
					for( int i = 0; i < 3; ++i )
					{
						int* selectedindex = pServerUnification_Window->GetSelectCharacterIndex();
						if( selectedindex )
						{
							if( selectedindex[i] > -1 )
								movereq.ucSelectCharac[i] = selectedindex[i];								
							else
								movereq.ucSelectCharac[i] = 255;								
						}
					}
					if( pServerUnification_Window->GetSelectWarehouseIndex() > -1 )
						movereq.ucSelectDepotGroup = pServerUnification_Window->GetSelectWarehouseIndex();
					else
						movereq.ucSelectDepotGroup = 255;
					
					if( strlen(g_MessageBox.m_IMEControl.GetText()) > 7 )
					{
						strncpy( movereq.cPassword, g_MessageBox.m_IMEControl.GetText(), 7 );
					}
					else
					{
						strcpy( movereq.cPassword, g_MessageBox.m_IMEControl.GetText() );
					}

					return _XSendPacket_Encapsulate((char*)&movereq);
				}
				break;
			case en_ug_change_name_request :
				{					
					int size = sizeof(MSG_UG_CHANGE_NAME_REQUEST);
					MSG_UG_CHANGE_NAME_REQUEST changenamereq;
					ZeroMemory(&changenamereq, size);
					
					changenamereq.header.sLength = size;
					changenamereq.ucMessage =	MSG_NO_UNI_GROUP_LOBBY;
					changenamereq.ucMode = (unsigned char)param1;

					int selectedservernum = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
					if( selectedservernum == _GR_INDEX_UNION_ )
					{
						_XWindow_ChangeName* pServer_ChangeName_Window = (_XWindow_ChangeName*)ServerUnificationWindowMgr->FindWindow(_XDEF_WTITLE_CHANGENAMEWINDOW);
						if( !pServer_ChangeName_Window )
							return FALSE;

						changenamereq.ucIndex = pServer_ChangeName_Window->GetSelectedIndex();
						strcpy( changenamereq.ucNewName, pServer_ChangeName_Window->GetChangedName());
					}
					else
					{						
						_XWindow_ChangeName* pSelect_ChangeName_Window = (_XWindow_ChangeName*)SelectCharacterWindowMgr->FindWindow(_XDEF_WTITLE_CHANGENAMEWINDOW);
						if( !pSelect_ChangeName_Window )
							return FALSE;
						
						changenamereq.ucIndex = pSelect_ChangeName_Window->GetSelectedIndex();
						strcpy( changenamereq.ucNewName, pSelect_ChangeName_Window->GetChangedName());
					}				

					return _XSendPacket_Encapsulate((char*)&changenamereq);
				}
				break;
			case en_ug_force_use_charac_in_new :
				{
					int size = sizeof(MSG_UG_FORCE_USE_CHARAC_IN_NEW);
					MSG_UG_FORCE_USE_CHARAC_IN_NEW forceusecharac;
					ZeroMemory(&forceusecharac, size);
					
					forceusecharac.header.sLength = size;
					forceusecharac.ucMessage =	MSG_NO_UNI_GROUP_LOBBY;
					forceusecharac.ucMode = (unsigned char)param1;					
					
					if( strlen(g_MessageBox.m_IMEControl.GetText()) > 7 )
					{
						strncpy( forceusecharac.cPW, g_MessageBox.m_IMEControl.GetText(), 7 );
					}
					else
					{
						strcpy( forceusecharac.cPW, g_MessageBox.m_IMEControl.GetText() );
					}

					return _XSendPacket_Encapsulate((char*)&forceusecharac);
				}
				break;
			}
		}
		break;
#endif

#endif // #ifndef _XTS_NEWCHARACLIST

#ifdef _XTS_ITEMMALLBROWSER
	case MSG_NO_ITEM_SHOP :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Item Shop Req");
			
			switch(param1) 
			{
			case PKTMODE_PERSONAL_CASH_INVEN_REQ :
				{
					int size = sizeof(MSG_U2GS_ITEM_SHOP__PERSONAL_CASH_INVEN_REQ);
					MSG_U2GS_ITEM_SHOP__PERSONAL_CASH_INVEN_REQ cashinvenreq;
					ZeroMemory(&cashinvenreq, size);
					
					cashinvenreq.Header.sLength = size;
					cashinvenreq.ucMsg = MSG_NO_ITEM_SHOP;
					cashinvenreq.ucMode = (unsigned char)PKTMODE_PERSONAL_CASH_INVEN_REQ;
					
					cashinvenreq.wInvenSize		= (unsigned char)45;// ������ �ִ� ����
					
					return _XSendPacket_Encapsulate((char*)&cashinvenreq);
				}
				break;
			case PKTMODE_PRESENT_CASH_INVEN_REQ :
				{
					int size = sizeof(MSG_U2GS_ITEM_SHOP__PRESENT_CASH_INVEN_REQ);
					MSG_U2GS_ITEM_SHOP__PRESENT_CASH_INVEN_REQ cashinvenreq;
					ZeroMemory(&cashinvenreq, size);
					
					cashinvenreq.Header.sLength = size;
					cashinvenreq.ucMsg = MSG_NO_ITEM_SHOP;
					cashinvenreq.ucMode = (unsigned char)PKTMODE_PRESENT_CASH_INVEN_REQ;
					
					cashinvenreq.wInvenSize		= (unsigned char)45;// ������ �ִ� ����
					
					return _XSendPacket_Encapsulate((char*)&cashinvenreq);
				}
				break;
			case PKTMODE_MOVE_PERSONAL_PACKAGE_REQ :
				{
					_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
					if( pCashBox_Window )
					{
						char cashmovereq[1024];
						ZeroMemory(&cashmovereq, 1024);
						int offset = sizeof(MSG_ITEM_SHOP_BASE);

						UCHAR ucPkgCnt = (UCHAR)param2;
						if( ucPkgCnt > 10 )	break;

						memcpy(cashmovereq + offset, &ucPkgCnt, sizeof(UCHAR)); offset += sizeof(UCHAR);
						
						_XCASHITEMBOX_INFO* cashitem = NULL;
						list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;

                        BOOL bCashUse = FALSE;
                        int nCount = 0;
                    
						for(iter_cashitem = pCashBox_Window->m_listCashItem.begin() ; iter_cashitem != pCashBox_Window->m_listCashItem.end() ; ++iter_cashitem)
						{
							cashitem = *iter_cashitem;
							if( cashitem )
							{
								//���õ� �����۸� �̵��Ѵ�.
								if( cashitem->bCheck )
								{
                                #ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
                                    if(cashitem->indexPackage == 2210107 || 
                                        cashitem->indexPackage == 2210108 ||
                                        cashitem->indexPackage == 2210109 ||
                                        cashitem->indexPackage == 2210110 )
                                    {
                                        nCount++;
                                        if( nCount >= 2 )
                                        {
                                            bCashUse = TRUE;
                                            _XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
							                pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3827), TRUE, 0 );
                                            //���õ� ������ �� �Ⱓ�� �೶�� �ΰ� �̻��̹Ƿ� �̵��� �� �����ϴ�.
							                pMessageBox->StartTimer(10000, TRUE);
                                            break;
                                        }

                                        if( cashitem->indexPackage == 2210107 || cashitem->indexPackage == 2210109 ) //������ ���� 7 9 ���� 
                                        {
                                            if( _XGENDER_MAN != g_pLocalUser->m_CharacterInfo.gender )
                                            {
                                                bCashUse = TRUE;
                                                _XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
							                    pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3823), TRUE, 0 ); //�Ⱓ�� �೶�� ������ �ٸ��Ƿ� �̵��� �� �����ϴ�.
							                    pMessageBox->StartTimer(10000, TRUE);
                                                break;
                                            }
                                        }
                                        else if( cashitem->indexPackage == 2210108 || cashitem->indexPackage == 2210110 ) //������ ���� 8 10 ����
                                        {
                                            if( _XGENDER_WOMAN != g_pLocalUser->m_CharacterInfo.gender )
                                            {
                                                bCashUse = TRUE;
                                                _XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
							                    pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3823), TRUE, 0 ); //�Ⱓ�� �೶�� ������ �ٸ��Ƿ� �̵��� �� �����ϴ�.
							                    pMessageBox->StartTimer(10000, TRUE);
                                                break;
                                            }
                                        }
                                    
                                        if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_CASHBACKPACK1].Get_m_sID() > 0 )
                                        {
                                            bCashUse = TRUE;
                                            _XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
							                pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3826), TRUE, 0 );	
                                             //���� ���� ���� �Ⱓ�� �೶�� ����� �� �̵��� �����մϴ�.
							                pMessageBox->StartTimer(10000, TRUE);
                                            break;
                                        }
                                    }
                                #endif

									memcpy(cashmovereq + offset, &cashitem->OrderNumber, sizeof(int));  offset += sizeof(int);
									memcpy(cashmovereq + offset, &cashitem->ProductNumber, sizeof(int));  offset += sizeof(int);
									memcpy(cashmovereq + offset, &cashitem->indexPackage, sizeof(int));  offset += sizeof(int);
								}
							}
						}

                        if( bCashUse ) // ĳ���೶ ��뿩��
                        {
                            pCashBox_Window->m_btnMoveSelectedItem->EnableWindow(TRUE);
                            break;
                        }

						((MSG_ITEM_SHOP_BASE*)(&cashmovereq))->Header.sLength = offset;
						((MSG_ITEM_SHOP_BASE*)(&cashmovereq))->ucMsg = MSG_NO_ITEM_SHOP;
						((MSG_ITEM_SHOP_BASE*)(&cashmovereq))->ucMode = PKTMODE_MOVE_PERSONAL_PACKAGE_REQ;						
												
						return _XSendPacket_Encapsulate((char*)&cashmovereq);
					}
				}
				break;
			case PKTMODE_MOVE_PRESENT_PACKAGE_REQ :
				{
					int size = sizeof(MSG_U2GS_ITEM_SHOP__MOVE_PRESENT_ITEM_REQ);
					MSG_U2GS_ITEM_SHOP__MOVE_PRESENT_ITEM_REQ cashmovereq;
					ZeroMemory(&cashmovereq, size);
					
					cashmovereq.Header.sLength = size;
					cashmovereq.ucMsg = MSG_NO_ITEM_SHOP;
					cashmovereq.ucMode = (unsigned char)param1;
					
					cashmovereq.iOrderNo	= (unsigned char)param2;// �ֹ���ȣ
					cashmovereq.iPkgIdx		= (unsigned char)param3;// ��ǰ��ȣ
					cashmovereq.iProductNo	= (unsigned char)param4;// ���õ� ��Ű���� �ε���
					
					return _XSendPacket_Encapsulate((char*)&cashmovereq);
				}
				break;
			}
		}
		break;
#endif
	case MSG_NO_QUICKSLOT	:
		{
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : Quickslot change" );

			//////////////////////////////////////////////////////////////////////////
			// Quick slot save
			if( param1 == 0 ) // reset
			{
				int size = sizeof( MSG_QUICKSLOT_RESET_UNIT );
				MSG_QUICKSLOT_RESET_UNIT quickslotresetunit;
				ZeroMemory ( &quickslotresetunit, size );
				quickslotresetunit.header.sLength = size;
				quickslotresetunit.ucMessage = MSG_NO_QUICKSLOT;
				quickslotresetunit.ucQuickSlotMode = en_quickslotmode_reset;
				quickslotresetunit.slotNumber = param2;
				
				return _XSendPacket_Encapsulate( (char*)&quickslotresetunit );
			}
			else if( param1 == 1 ) // set
			{			
				int size = sizeof( MSG_QUICKSLOT_UPDATE_UNIT );
				MSG_QUICKSLOT_UPDATE_UNIT quickslotupdateunit;
				ZeroMemory ( &quickslotupdateunit, size );
				quickslotupdateunit.header.sLength = size;
				quickslotupdateunit.ucMessage = MSG_NO_QUICKSLOT;
				quickslotupdateunit.ucQuickSlotMode = en_quickslotmode_update;
				quickslotupdateunit.slotNumber = param2;

				_XWindow_QuickSlot* pQSlot_Window = (_XWindow_QuickSlot *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUICKSLOT);

				if( pQSlot_Window )
				{
					quickslotupdateunit.slotCont.slotType = pQSlot_Window->m_QuickSlot[param2].type;
					quickslotupdateunit.slotCont.skillID = pQSlot_Window->m_QuickSlot[param2].skillid;
					quickslotupdateunit.slotCont.lifeSkillID = pQSlot_Window->m_QuickSlot[param2].lifeskillid;
					quickslotupdateunit.slotCont.itemCType = pQSlot_Window->m_QuickSlot[param2].itemCType;
					quickslotupdateunit.slotCont.itemSID = pQSlot_Window->m_QuickSlot[param2].itemSID;
					quickslotupdateunit.slotCont.invenNumber = pQSlot_Window->m_QuickSlot[param2].invenNumber;			
					quickslotupdateunit.slotCont.reserved = 0;
					quickslotupdateunit.slotCont.reserved2 = 0;
				
					return _XSendPacket_Encapsulate( (char*)&quickslotupdateunit );
				}
			}
			//////////////////////////////////////////////////////////////////////////
			// help system save
			else if( param1 == 10 ) // reset
			{
				int size = sizeof( MSG_HELPLOG_RESET );
				MSG_HELPLOG_RESET helpsystemresetunit;
				ZeroMemory ( &helpsystemresetunit, size );
				helpsystemresetunit.header.sLength = size;
				helpsystemresetunit.ucMessage = MSG_NO_QUICKSLOT;
				helpsystemresetunit.ucQuickSlotMode = en_helplog_reset;

				return _XSendPacket_Encapsulate( (char*)&helpsystemresetunit );
			}
			else if( param1 == 11 ) // set
			{
				int size = sizeof( MSG_HELPLOG_UPDATE );
				MSG_HELPLOG_UPDATE helplogupdate;
				ZeroMemory ( &helplogupdate, size );
				helplogupdate.header.sLength = size;
				helplogupdate.ucMessage = MSG_NO_QUICKSLOT;
				helplogupdate.ucQuickSlotMode = en_helplog_update;
				
				g_TutorialInfo->ConvertToByte( (LPBOOL)helplogupdate.ucHelpLog, sizeof(BYTE) * 4 );
				
				return _XSendPacket_Encapsulate( (char*)&helplogupdate );
			}
		}
		break;
#ifdef _XTS_ALIANCE_20061018
		
		//////////////////////////////////////////////////////////////////////////
		// ���� ���� ��Ŷ.
		//////////////////////////////////////////////////////////////////////////
	case MSG_NO_ALLY_BASE:
		{			
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Alience Req");
			
			if(!Send_Alience(type, param1, param2, param3, param4))
			{
				ProcessNetworkError();
				return FALSE;
			}	
		}
		break;

#endif
		
#ifdef _XDEF_CASTLEBATTLE
		//�����
	case MSG_NO_CB_SCHEDULE :
		{
			if(param1 == 0)
			{
				// en_cb_owner_req_schedule_list = 0,	// 0 ������ ������ ����Ʈ ��û ��Ŷ
				return SendMSGCBOwnerReqScheduleList(type, param1, param2, param3, param4);
			}
			else if(param1 == 2)
			{
				// en_cb_req_schedule_list,			// 2 �Ϲ� ����(ȸ��) ������ ����Ʈ ��û ��Ŷ
				return SendMSGCBReqScheduleList(type, param1, param2, param3, param4);
			}
			else if(param1 == 4)
			{
				// en_cb_req_challenge_list,			// 4 ���� ��� ��û
				return SendMSGCBReqChallengeList(type, param1, param2, param3, param4);
			}
			else if(param1 == 20)
			{
				// en_cb_req_change_time = 20,			// 20 ������ �ð� ���� ��û
				return SendMSGCBReqChangeTime(type, param1, param2, param3, param4);
			}
			else if(param1 == 40)
			{
				// en_cb_req_challenge = 40,			// 40 ȸ��(����)�� ���� ��û
				return SendMSGCBReqChallenge(type, param1, param2, param3, param4);
			}
			else if(param1 == 43)
			{
				// en_cb_challenge_cancel,				// 43 ���� ��û ���
				return SendMSGCBChallengeCancel(type, param1, param2, param3, param4);
			}
			else if(param1 == 72)
			{
				// 72 ���ְ� ������ �ŵְ���
				_XDWINPRINT("SEND PACKET : Request Tax take");

				int size = sizeof(MSG_CB_TAX_REQUEST_TAKE);
				MSG_CB_TAX_REQUEST_TAKE req;
				ZeroMemory(&req, size);
				req.header.sLength = size;
				req.ucMessage = type;

				req.ucMode = (unsigned char)param1;
				req.ucCastleIndex = (unsigned char)g_pLocalUser->m_nCastleMapId;
				req.uiRequestMoney = g_CBManager.GetCastleTaxMoney(g_pLocalUser->m_nCastleMapId);

				return _XSendPacket_Encapsulate((char*)&req);

			}
		}
		break;
	case MSG_NO_COMBAT :
		{
			// ���� ���� ��û / ���� ���� ��û / ���� �Ϸ� ��û
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Request Combat ");
			
			if(param1 == en_cb_etc_loud_chat)
			{
				return SendMSGCBEtcLoudChat(type, param1, param2, param3, param4);
			}
			else if(param1 == en_cb_record_pkt_info_req)
			{
				_XDWINPRINT("SEND PACKET : MSG_NO_COMBAT [mode:en_cb_record_pkt_info_req]");

				int size = sizeof(MSG_CB_RECORD_INFO_REQ);
				MSG_CB_RECORD_INFO_REQ req;
				ZeroMemory(&req, size);
				req.header.sLength = size;
				req.ucMessage = type;

				req.ucMode = (unsigned char)param1;
				req.index = param2;
			
				return _XSendPacket_Encapsulate((char*)&req);
			}
			else
			{
				_XDWINPRINT("SEND PACKET : MSG_NO_COMBAT [mode:%d]", param1);

				int size = sizeof(MSG_COMBAT);
				MSG_COMBAT combat;
				ZeroMemory(&combat, size);
				combat.header.sLength = size;
				combat.ucMessage = type;

				combat.ucMode = (unsigned char)param1;
				
				g_pLocalUser->m_bSendThreatenPacket = true;
				_XDWINPRINT("Threaten bool : %d ", g_pLocalUser->m_bSendThreatenPacket);

				return _XSendPacket_Encapsulate((char*)&combat);
			}
		}
		break;

#endif

//Author : ����� 
#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
	
	case MSG_NO_GGAUTH_REQ:
		{
			
#ifdef _USE_NPROTECTLOG
			_XLog( "SEND PACKET : NPROTECT_GAME_GUARD" );
#endif

			int nSize = sizeof( MSG_GGAUTH_REQ );
			MSG_GGAUTH_REQ	ggauthdata;
			ZeroMemory ( &ggauthdata, nSize );

			ggauthdata.header.sLength = nSize;
			ggauthdata.ucMessage = MSG_NO_GGAUTH_REQ;
			memcpy( &ggauthdata.data, &g_GameGuardData.data, sizeof(GG_AUTH_DATA) );

#ifdef _USE_NPROTECTLOG
			_XLog( "SEND_PACKET_NPROTECT_GAME_GUARD : %08X %08X %08X %08X ", ggauthdata.data.dwIndex, ggauthdata.data.dwValue1, ggauthdata.data.dwValue2, ggauthdata.data.dwValue3);
#endif

#ifdef _XDEF_NPROTECT_GUARD_TIME_LOG
			//���� �ð��� ���´�
			SYSTEMTIME TimeInfo;
			GetLocalTime(&TimeInfo);

			_XLog( "SEND_PACKET_TIME : [%d:%d:%d]", TimeInfo.wHour, TimeInfo.wMinute, TimeInfo.wSecond );
#endif

			return _XSendPacket_Encapsulate( (char*)&ggauthdata );
		}
		break;

#endif 
//Last updated: 07/02/02

#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK����ġ����, ������ PK����
	case MSG_NO_LTS :
		{		
			if( param1 == en_lts_lvpk_del_req_cl_gs )
			{
				_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
				if(!pPKTracingRequest_Window)
				{
					break;
				}
				VIEWPACKETLOG
					_XDWPACKETLOG("SEND PACKET : Request PK Tracing ");
				
				int size = sizeof(MSG_LTS_LVPK_DEL_REQ_CL_GS);
				MSG_LTS_LVPK_DEL_REQ_CL_GS pktracing;
				ZeroMemory(&pktracing, size);
				pktracing.header.sLength = size;
				pktracing.ucMessage = MSG_NO_LTS;
				
				pktracing.ucPktMode = (unsigned char)param1;
				
				strncpy( pktracing.szDelCharacName, pPKTracingRequest_Window->m_cDeleteTargetName, _XDEF_MAX_USERNAMESTRINGLENGTH);
				return _XSendPacket_Encapsulate((char*)&pktracing);
			}
			else if(param1 == en_lts_pk_insert_req_cl_gs)
			{
				_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
				if(!pPKTracingRequest_Window)
				{
					break;
				}
				VIEWPACKETLOG
					_XDWPACKETLOG("SEND PACKET : Request PK Tracing ");
				
				int size = sizeof(MSG_LTS_PK_INSERT_REQ_CL_GS);
				MSG_LTS_PK_INSERT_REQ_CL_GS pktracing;
				ZeroMemory(&pktracing, size);
				pktracing.header.sLength = size;
				pktracing.ucMessage = MSG_NO_LTS;
				
				pktracing.ucPktMode = (unsigned char)param1;
				pktracing.ucTimeMode = (unsigned char)param2;

				strncpy( pktracing.szInsertCharacName, pPKTracingRequest_Window->m_cTargetName, _XDEF_MAX_USERNAMESTRINGLENGTH);
				return _XSendPacket_Encapsulate((char*)&pktracing);

			}
			else if(param1 == en_lts_sp_insert_req_cl_gs)
			{
				_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
				if(!pPKTracingRequest_Window)
				{
					break;
				}
				VIEWPACKETLOG
					_XDWPACKETLOG("SEND PACKET : Request PK SomeOne Tracing ");
				
				int size = sizeof(MSG_LTS_SP_INSERT_REQ_CL_GS);
				MSG_LTS_SP_INSERT_REQ_CL_GS pktracing;
				ZeroMemory(&pktracing, size);
				pktracing.header.sLength = size;
				pktracing.ucMessage = MSG_NO_LTS;
				
				pktracing.ucPktMode = (unsigned char)param1;
				pktracing.ucTimeMode = (unsigned char)param2;
				
				//strncpy( pktracing.szInsertCharacName, pPKTracingRequest_Window->m_cTargetName, _XDEF_MAX_USERNAMESTRINGLENGTH);
				//Ư���� ����
				strncpy( pktracing.szInsertCharacName, pPKTracingRequest_Window->m_cSpecialTargetName, _XDEF_MAX_USERNAMESTRINGLENGTH);
				return _XSendPacket_Encapsulate((char*)&pktracing);
			}
		}
		break;
#endif

#ifdef _XDEF_COMMONITEMMALL_20070402	// PK����ġ����, ������ PK����
	case MSG_NO_COMMOM_REQ :
		{		
			switch( param1 )
			{
				
			case 10://_CIS_CASH_INVEN_REQ
				{
					//ĳ�� �ڽ��� �������� ǥ���ϵ��� �䱸��.
					VIEWPACKETLOG
						_XDWPACKETLOG("SEND PACKET : Request CIS Inven ");
					
					int size = sizeof(MSG_COMMON_REQ);
					MSG_COMMON_REQ CISInvenReq;
					ZeroMemory(&CISInvenReq, size);

					int offset = sizeof(_sHeader);//breif : �̱��� ������ �޽��� ���̵� �ȳ���
					offset += sizeof(u_char);
					offset += sizeof(WORD);

//					CISInvenReq.header.sLength = size;
//					CISInvenReq.ucMessage = MSG_NO_COMMOM_REQ;
//					CISInvenReq.wMode = (WORD)param1;
					
					_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
					if( pCashBox_Window )
					{
						bool b = true;
						if( pCashBox_Window->m_SelectedTab == _XCASHBOXTAB_PERSONAL )
						{
							b = false;
							memcpy((char*)&CISInvenReq + offset, &b, sizeof(bool)); 
							offset += sizeof(bool);							
							//CISInvenReq._CIS_CASH_INVEN_REQ.bIsPresent = true;
						}
						else if( pCashBox_Window->m_SelectedTab == _XCASHBOXTAB_PRESENT)
						{
							b = true;
							memcpy((char*)&CISInvenReq + offset,  &b, sizeof(bool)); 
							offset += sizeof(bool);							
							//CISInvenReq._CIS_CASH_INVEN_REQ.bIsPresent = false;
						}
						pCashBox_Window->m_CurrentPageNumber = (int)param2 - 1; //���� �������� 0 �������� �ۼ�
					}
					WORD value1 = (WORD)param2;
					memcpy((char*)&CISInvenReq + offset, &value1, sizeof(WORD)); 
					offset += sizeof(WORD);		
					WORD value2 = (WORD)param3;
					memcpy((char*)&CISInvenReq + offset, &value2, sizeof(WORD)); 
					offset += sizeof(WORD);							
					
					((MSG_COMMON_REQ*)(&CISInvenReq))->header.sLength = offset;
					((MSG_COMMON_REQ*)(&CISInvenReq))->ucMessage = MSG_NO_COMMOM_REQ;
					((MSG_COMMON_REQ*)(&CISInvenReq))->wMode = (WORD)param1;		
//					CISInvenReq._CIS_CASH_INVEN_REQ.wPageNum = 1;
//					CISInvenReq._CIS_CASH_INVEN_REQ.wNumPerPage = 1;

					//������ ��ȣ�� �������̽� �۾��Ŀ� ó��
					
					//					bool	bIsPresent;		// 0: �Ϲ� ������, 1: ���� ������
					//					WORD	wPageNum;		// ������ ��ȣ
					//					WORD	wNumPerPage;	// ������ �� �ڷ��
					return _XSendPacket_Encapsulate((char*)&CISInvenReq);
				}
				break;
			case 15://_CIS_MOVE_ITEM_REQ
				{
					// ĳ�� �ڽ����� ���� �κ��丮�� �̵��ϵ��� �䱸��.
					VIEWPACKETLOG
						_XDWPACKETLOG("SEND PACKET : Request CIS Move Item ");
					
					int size = sizeof(MSG_COMMON_REQ);
					MSG_COMMON_REQ CISMoveItemReq;
					ZeroMemory(&CISMoveItemReq, size);
//					CISMoveItemReq.header.sLength = size;
//					CISMoveItemReq.ucMessage = MSG_NO_COMMOM_REQ;
//					CISMoveItemReq.wMode = (WORD)param1;
					int offset = sizeof(_sHeader);
					offset += sizeof(u_char);
					offset += sizeof(WORD);
										
					_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
					if( pCashBox_Window )
					{
												
						UCHAR ucPkgCnt = (UCHAR)param2;
						if( ucPkgCnt > 10 )	break;
						
						memcpy((char*)&CISMoveItemReq + offset, &ucPkgCnt, sizeof(UCHAR)); offset += sizeof(UCHAR);
						//CISMoveItemReq._CIS_MOVE_ITEM_REQ.ucPkgCnt = (UCHAR)param2;
						
						_XCASHITEMBOX_INFO* cashitem = NULL;
						list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
						for(iter_cashitem = pCashBox_Window->m_listCashItem.begin() ; iter_cashitem != pCashBox_Window->m_listCashItem.end() ; ++iter_cashitem)
						{
							cashitem = *iter_cashitem;
							if( cashitem )
							{
								//���õ� �����۸� �̵��Ѵ�.
								if( cashitem->bCheck )
								{									
									memcpy((char*)&CISMoveItemReq + offset, &cashitem->OrderNumber, sizeof(int)); 
									offset += sizeof(int);
									memcpy((char*)&CISMoveItemReq + offset, &cashitem->indexPackage, sizeof(int)); 
									offset += sizeof(int);
								}
							}
						}
						((MSG_COMMON_REQ*)(&CISMoveItemReq))->header.sLength = offset;
						((MSG_COMMON_REQ*)(&CISMoveItemReq))->ucMessage = MSG_NO_COMMOM_REQ;
						((MSG_COMMON_REQ*)(&CISMoveItemReq))->wMode = (WORD)param1;						
						
						return _XSendPacket_Encapsulate((char*)&CISMoveItemReq);
					}
					
					return _XSendPacket_Encapsulate((char*)&CISMoveItemReq);
				}
				break;
			case 20://_CIS_PRESENT_MSG_REQ
				{
					// ����
					VIEWPACKETLOG
						_XDWPACKETLOG("SEND PACKET : Request Present Msg ");
					
					int size = sizeof(MSG_COMMON_REQ);
					MSG_COMMON_REQ CISPresentMsg;
					ZeroMemory(&CISPresentMsg, size);
					
					int offset = sizeof(_sHeader);
					offset += sizeof(u_char);
					offset += sizeof(WORD);

//					//�ӽ÷� ����
//					int value = 1;// �������� Guid
//					memcpy((char*)&CISPresentMsg + offset, &value, sizeof(int)); 
//					offset += sizeof(int);		
					
					_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
					if( pCashBox_Window )
					{
						memcpy((char*)&CISPresentMsg + offset, &pCashBox_Window->m_SendOrderNumberPresentMessage, sizeof(int)); 
						offset += sizeof(int);
					}

					((MSG_COMMON_REQ*)(&CISPresentMsg))->header.sLength = offset;
					((MSG_COMMON_REQ*)(&CISPresentMsg))->ucMessage = MSG_NO_COMMOM_REQ;
					((MSG_COMMON_REQ*)(&CISPresentMsg))->wMode = (WORD)param1;	
					
//					CISPresentMsg.header.sLength = size;
//					CISPresentMsg.ucMessage = MSG_NO_COMMOM_REQ;
//					CISPresentMsg.wMode = (WORD)param1;
//					
//					int		iPkgGuid;	// �������� Guid
					return _XSendPacket_Encapsulate((char*)&CISPresentMsg);
				}
				break;
			}
		}
		break;
#endif
	//Author : ����� //ģ����ȯ
	case MSG_NO_RECALL_FRIEND :
		{
#ifdef _XDEF_RECALLSYSTEM_070604_MAGVIPER
			VIEWPACKETLOG
						_XDWPACKETLOG("SEND PACKET : Recall Friend ");

			switch( (unsigned char)param1 )
			{
			case en_rf_send_request_recall :
				{
					_XWindow_RecallItem* pRecallWindow = (_XWindow_RecallItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_RECALLITEM_WINDOW);
					if(pRecallWindow)
					{
						int size = sizeof(MSG_RF_SEND_REQUEST_RECALL);
						MSG_RF_SEND_REQUEST_RECALL req_recall;
						ZeroMemory(&req_recall, size);
						req_recall.header.sLength = size;
						req_recall.ucMessage = MSG_NO_RECALL_FRIEND;
						req_recall.ucFriendMode = en_rf_send_request_recall;
						
						req_recall.cNeedItemSlot = pRecallWindow->GetItemIndex();

						if( pRecallWindow->GetRecallIndex() == 0 )
						{
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3531), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS ); //ģ���� ���õ��� �ʾҽ��ϴ�.
							pRecallWindow->SetRecallButton(TRUE);
							break;
						}

						if( pRecallWindow->GetCharacterName( pRecallWindow->GetRecallIndex() ) == NULL )
						{
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3531), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS ); //ģ���� ���õ��� �ʾҽ��ϴ�.
							pRecallWindow->SetRecallButton(TRUE);
							break;
						}

						strcpy( req_recall.cTargetName, pRecallWindow->GetCharacterName( pRecallWindow->GetRecallIndex() ) );
						req_recall.fTargetX = g_pLocalUser->m_Position.x;
						req_recall.fTargetZ = g_pLocalUser->m_Position.z;

						return _XSendPacket_Encapsulate( (char*)&req_recall );
					}
				}
				break;
			case en_rf_send_answer_yes :
				{
					_XWindow_RecallItem* pRecallWindow = (_XWindow_RecallItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_RECALLITEM_WINDOW);
					if(pRecallWindow)
					{
						int size = sizeof(MSG_RF_SEND_ANSWER_YES);
						MSG_RF_SEND_ANSWER_YES req_recall_yes;
						ZeroMemory(&req_recall_yes, size);
						req_recall_yes.header.sLength = size;
						req_recall_yes.ucMessage = MSG_NO_RECALL_FRIEND;
						req_recall_yes.ucFriendMode = en_rf_send_answer_yes;

						strcpy( req_recall_yes.cFromName, pRecallWindow->GetFromName() );

						return _XSendPacket_Encapsulate( (char*)&req_recall_yes );
					}
				}
				break;
			case en_rf_send_answer_no :
				{
					_XWindow_RecallItem* pRecallWindow = (_XWindow_RecallItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_RECALLITEM_WINDOW);
					if(pRecallWindow)
					{
						int size = sizeof(MSG_RF_SEND_ANSWER_NO);
						MSG_RF_SEND_ANSWER_NO req_recall_no;
						ZeroMemory(&req_recall_no, size);
						req_recall_no.header.sLength = size;
						req_recall_no.ucMessage = MSG_NO_RECALL_FRIEND;
						req_recall_no.ucFriendMode = en_rf_send_answer_no;

						req_recall_no.ucNoReason = pRecallWindow->GetCharState();
						strcpy( req_recall_no.cFromName, pRecallWindow->GetFromName() );

						return _XSendPacket_Encapsulate( (char*)&req_recall_no );
					}
				}
				break;

			//�̵��� ����................
			case en_rf_send_request_goto :
				{
					_XWindow_RecallItem* pRecallWindow = (_XWindow_RecallItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_RECALLITEM_WINDOW);
					if(pRecallWindow)
					{
						int size = sizeof(MSG_RF_SEND_REQUEST_GOTO);
						MSG_RF_SEND_REQUEST_GOTO req_move;
						ZeroMemory(&req_move, size);
						req_move.header.sLength = size;
						req_move.ucMessage = MSG_NO_RECALL_FRIEND;
						req_move.ucFriendMode = en_rf_send_request_goto;
						
						req_move.ucNeedItemSlot = pRecallWindow->GetItemIndex();

						if( pRecallWindow->GetRecallIndex() == 0 )
						{
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3531), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS ); //ģ���� ���õ��� �ʾҽ��ϴ�.
							pRecallWindow->SetRecallButton(TRUE);
							break;
						}

						if( pRecallWindow->GetCharacterName( pRecallWindow->GetRecallIndex() ) == NULL )
						{
							InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3531), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS ); //ģ���� ���õ��� �ʾҽ��ϴ�.
							pRecallWindow->SetRecallButton(TRUE);
							break;
						}

						strcpy( req_move.cTargetName, pRecallWindow->GetCharacterName( pRecallWindow->GetRecallIndex() ) );

						return _XSendPacket_Encapsulate( (char*)&req_move );
					}
				}
				break;
			case en_rf_send_goto_answer_yes :
				{
					_XWindow_RecallItem* pRecallWindow = (_XWindow_RecallItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_RECALLITEM_WINDOW);
					if(pRecallWindow)
					{
						int size = sizeof(MSG_RF_SEND_GOTO_ANSWER_YES);
						MSG_RF_SEND_GOTO_ANSWER_YES req_move_yes;
						ZeroMemory(&req_move_yes, size);
						req_move_yes.header.sLength = size;
						req_move_yes.ucMessage = MSG_NO_RECALL_FRIEND;
						req_move_yes.ucFriendMode = en_rf_send_goto_answer_yes;

						req_move_yes.fTargetX = g_pLocalUser->m_Position.x;
						req_move_yes.fTargetZ = g_pLocalUser->m_Position.z;
						strcpy( req_move_yes.cFromName, pRecallWindow->GetFromName() );

						return _XSendPacket_Encapsulate( (char*)&req_move_yes );
					}
				}
				break;
			case en_rf_send_goto_answer_no :
				{
					_XWindow_RecallItem* pRecallWindow = (_XWindow_RecallItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_RECALLITEM_WINDOW);
					if(pRecallWindow)
					{
						int size = sizeof(MSG_RF_SEND_GOTO_ANSWER_NO);
						MSG_RF_SEND_GOTO_ANSWER_NO req_move_no;
						ZeroMemory(&req_move_no, size);
						req_move_no.header.sLength = size;
						req_move_no.ucMessage = MSG_NO_RECALL_FRIEND;
						req_move_no.ucFriendMode = en_rf_send_goto_answer_no;

						req_move_no.ucNoReason = pRecallWindow->GetCharState();
						strcpy( req_move_no.cFromName, pRecallWindow->GetFromName()  );

						return _XSendPacket_Encapsulate( (char*)&req_move_no );
					}
				}
				break;
			}
#endif 
		}
		break;

	// SDS ctrl
	case MSG_NO_NATIONAL_DIST :
		{
#ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI
			int size = sizeof(MSG_NDIST_VINA_SDS_CTRL);
			MSG_NDIST_VINA_SDS_CTRL sdsctrl;
			ZeroMemory(&sdsctrl, size);
			sdsctrl.header.sLength = size;
			sdsctrl.ucMessage = MSG_NO_NATIONAL_DIST;

			sdsctrl.ucMode = (unsigned char)param1;
			sdsctrl.ucStop = (unsigned char)param2;

			return _XSendPacket_Encapsulate( (char*)&sdsctrl );
#endif
		}
		break;

#ifdef _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI
	case MSG_NO_BW_WAR :
		{
			int size = sizeof(MSG_BW_ENTER_GATE_REQ);
			MSG_BW_ENTER_GATE_REQ enterreq;
			ZeroMemory(&enterreq, size);
			enterreq.header.sLength = size;
			enterreq.ucMessage = MSG_NO_BW_WAR;

			enterreq.ucBWMode = (unsigned char)param1;
			enterreq.usGateIndex = (unsigned char)param2;

			return _XSendPacket_Encapsulate((char*)&enterreq);
		}
		break;
#endif

#ifdef _XUSEAPEXMODULE // �븸 Apex module ���� 
	case MSG_NO_SECURITY :
		{
			if( param1 == en_apex_client_data )
			{
				int size = sizeof(MSG_APEX_CLIENT);
				MSG_APEX_CLIENT apexclientdata;
				ZeroMemory(&apexclientdata, size);
				//apexclientdata.header.sLength = size;

				apexclientdata.header.sLength = size - (APEX_MAX_LENGTH-param3);
				apexclientdata.ucMessage = MSG_NO_SECURITY;
				apexclientdata.ucMode = en_apex_client_data;
				
				apexclientdata.data_length = param3;
				char* pData = (char*)param2;

				memcpy( apexclientdata.data, pData, param3 );

				#ifdef _XAPEXMODULE_LOG
						_XLog( "Apex send client data, data length = %d", param3 );
				#endif
				
				return _XSendPacket_Encapsulate((char*)&apexclientdata);
			}
			else if( param1 == en_apex_client_login_result )
			{
				int size = sizeof(MSG_APEX_CLIENT_LOGIN_RESULT);
				MSG_APEX_CLIENT_LOGIN_RESULT apexclientresult;
				ZeroMemory(&apexclientresult, size);
				apexclientresult.header.sLength = size;

				apexclientresult.ucMessage = MSG_NO_SECURITY;
				apexclientresult.ucMode = en_apex_client_login_result;
								
				#ifdef _XAPEXMODULE_LOG
						_XLog( "Apex send CHCStart result = %d", param2 );
				#endif
				
				return _XSendPacket_Encapsulate((char*)&apexclientresult);
			}
		}
		break;
#endif
	case MSG_NO_MASTER_AND_PUPIL : //����
		{
		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : ����
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : MSG_NO_MASTER_AND_PUPIL");
			
			if(!Send_MnDSystem(type, param1, param2, param3, param4))
			{
				ProcessNetworkError();
				return FALSE;
			}	
		#endif
		}
		break;

	}

	return TRUE;
}

void _XNetwork::ProcessUDPNetworkError( void )
{	
	if( m_hLoginUDPSocket )
	{
		shutdown(m_hLoginUDPSocket, SD_BOTH);
		//closesocket(m_hClientSocket);	
		m_hLoginUDPSocket = NULL;
	}
	
	g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Login error", " ", TRUE, _XDEF_PROCESSTOPSIGNAL );
	TCHAR messagestring[256];
	strcpy(messagestring, _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILCONNECTLOGINSERVER));
	
	g_MessageBox.SetMessage( messagestring, WSAGetLastError() );
	g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
	_XUpdateWindow();
	
	_XDWINPRINT( "CRITICAL ERROR : UDP Network error" );
}

void _XNetwork::ProcessNetworkError( void )
{	
	if( !m_bConnected ) return;
	
	int ret = WSAGetLastError();
	g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE, _XDEF_PROCESSTOPSIGNAL );
	
	g_MessageBox.SetMessage( _XGETINTERFACETEXT(ID_STRING_NETWORK_DISCONNECTSERVER), ret );
	g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
	_XUpdateWindow();
	
	_XWindow_EventNotice* pEventNotice_Window = (_XWindow_EventNotice*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENTNOTICEWINDOW);
	if( pEventNotice_Window )
		pEventNotice_Window->ShowWindow(FALSE);
	
#ifdef _XDEF_WEBBOARD_20061211
	_XWindow_WebBoard* pWebBoard_Window = (_XWindow_WebBoard*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WEBBOARD );	
	if( pWebBoard_Window  )
	{
		pWebBoard_Window->ShowWindow(FALSE);
		ShowWindow(g_hWebBoardWindow, SW_HIDE );
		g_bShowItemMallWindow = FALSE;					
	}
#endif
	
#ifdef _XTS_ITEMMALLBROWSER	
	_XWindow_CashMall* pCashMall_Window = (_XWindow_CashMall*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHMALL );	
	if( pCashMall_Window )
	{
		pCashMall_Window->ShowWindow( FALSE );
		ShowWindow(g_hItemMallWindow, SW_HIDE);
		g_bShowItemMallWindow = FALSE;
		
#ifdef _XKOREAN
		// ������ �� �α� ���� ��Ų��..
		if( !g_bTestWebPageAddress )
			DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ninemall.nexon.com/logoff.asp");
		else
			DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://panda.nexon.com/logoff.asp");
#endif		
		
#ifdef _XENGLISH 
	//	DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ninemall.nexon.com/logoff.asp");		
#endif
		
#ifdef _XVIETNAMESE
	//DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://www.cuulong.com.vn/ktc/logout.aspx");		
	DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://cuulongid.zing.vn/ktc/logout.aspx");		
#endif
	
#ifdef _XJAPANESE
	//DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://ninetest.gameleon.jp/item_mall/item_logout.asp");		
	DisplayHTMLPage(g_hItemMallWebBrowserObject, "http://nine.gameleon.jp/item_mall/item_logout.asp");		
#endif
		
#ifdef _XTAIWANESE
	DisplayHTMLPage(g_hItemMallWebBrowserObject, "https://member.pfamily.com.tw/9ds_shopping_mall/logout.php"); 		
#endif
	}		
#endif
	
	_XLog( "Destroyed Network kernel. [_ECode:%d]", ret );

	// 2004.06.10->oneway48 insert
	if( m_hMessengerSocket )
	{
		shutdown(m_hMessengerSocket, SD_BOTH);
		closesocket(m_hMessengerSocket);
		m_hMessengerSocket = NULL;
	}	
	m_bMessengerServerConnected = FALSE;
	m_bMessengerServerLoginSuccess = FALSE;// 2004.06.19->oneway48 insert
	// insert end

	if( m_hClientSocket )
	{
		shutdown(m_hClientSocket, SD_BOTH);
		//closesocket(m_hClientSocket);	
		m_hClientSocket = NULL;
	}	
	m_bConnected = FALSE;	

#ifdef _XDEF_NEXONLOGIN // Nexon login
	
#ifdef _XDEF_NEXONLOGINFINAL	// �ؽ� �޽��� �α���
	if(strcmp(m_LOGINSERVER_ADDRESS, _T("221.147.34.19")) != 0)
	{
		ReleaseNMCO();
	}
#endif
				
#endif
}


void _XNetwork::MSGSVEnterResult(MSG_SV_ENTER_RESULT* serverenteringresult)
{
	if(serverenteringresult->cResult == 0)
	{
		VIEWPACKETLOG
			_XDWINPRINT("RECIEVE MESSAGE : Success server entering [ %s ]", g_pLocalUser->m_UserName);
		
		// ĳ���� ����Ʈ�� ��û
#ifdef _XTS_SERVERUNIFICATION
		int selectedservernum = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
		if( selectedservernum != _GR_INDEX_UNION_ )
			SendPacket(MSG_NO_CHARAC_REQ_LIST);
		else
		{
			if(((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.GetCurrentProcess() == &((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver)
			{
				// login process���� �޾��� �� 
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XPROCDEF_CONNECTEDSERVER, 0), (LPARAM)gHWnd);
			}
			else if(((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.GetCurrentProcess() == &((CEmperorOfDragonsApp*)gApp)->m_proc_createcharacter)
			{
				// create character process���� �޾��� �� 
				g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_SUCCESSCREATE, TRUE, _XDEF_CC_REQUESTCHARACTERLIST, 
					_XDEF_CC_REQUESTCHARACTERLIST, _XDEF_CC_REQUESTCHARACTERLIST);
				g_MessageBox.SetMessageColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			else if(((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.GetCurrentProcess() == &((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter)
			{
				// select character processs���� �޾��� ��
				((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.SetCharacterInfoWindow();// 2004.06.03->oneway48 insert
				
			#ifdef _XDEF_SELECT_ANOTHERCHARACTER_080721_KUKURI
				if(m_bDeleteCharacter)
			#endif
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_SUCCESSDELETE, TRUE, _XDEF_CS_REQUESTDELETECHARACTER);		
					g_MessageBox.SetMessageColor(D3DCOLOR_ARGB(255, 255, 255, 255));
					m_bDeleteCharacter = FALSE;
				}

			}
			else if(((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.GetCurrentProcess() == &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame)
			{
				// main game process���� �޾��� ��
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_GOTOSELECTCHARACTER, 0), (LPARAM)gHWnd);
			}
		}
#else
		SendPacket(MSG_NO_CHARAC_REQ_LIST);
#endif
	}
	else
	{
		_XDWINPRINT("RECIEVE MESSAGE : Failed server entering [ %s : %d ]", g_pLocalUser->m_UserName, serverenteringresult->cResult );
	}
}

void _XNetwork::MSGChangeServerGroup(MSG_SHIFT_STATE* changeservergroup)
{
	if( changeservergroup->uiStateFlag == 1) 
	{
		g_bNewClanQuest = TRUE; //�������� 32bit��������(1: ��������)
	}
	else
	{
		g_bNewClanQuest = FALSE;
	}
	// ���� ���� ���� �������� ���� ����Ʈ�� �ִٸ� 
	// ����Ʈ�� ����ϴ� �ڵ带 �ִ´�.
//	for( int i = 0; i < ID_QUEST_SYNC-1; i++ ) // ������ ����Ʈ�� �������� ����Ʈ���� ���� -> 5�� 
//	{
//		if( g_QuestScriptManager.m_ProgressSubQuestID[i] == �������� ����Ʈ ���̵� )		
//		{		
//			_XQuestScriptItem* currentquest = NULL;
//			currentquest = g_QuestScriptManager.m_mapSubQuestScriptList[g_QuestScriptManager.m_ProgressSubQuestID[i]];
//			if( currentquest )
//			{
//				SendPacket(MSG_NO_QUEST_USER_CANCEL, g_QuestScriptManager.m_ProgressSubQuestID[i], currentquest->GetCurrentTreeNumber());
//			}
//		}
//	}

}

void _XNetwork::MSGCharacList(MSG_CHARAC_LIST* characterlist)
{
	if( characterlist->uiStateFlag != 0) 
	{
		g_bNewClanQuest = TRUE; //�������� 32bit��������(1: ��������)		
	}
	else
	{
		g_bNewClanQuest = FALSE;
	}

	g_pLocalUser->m_CharacterCount = characterlist->nCharac;
	if(characterlist->nCharac > 0)
	{
		for(int i = 0 ; i < characterlist->nCharac ; ++i)
		{
			strcpy(g_pLocalUser->m_AllCharacterInfo[i].charactername, characterlist->List[i].cName);
			g_pLocalUser->m_AllCharacterInfo[i].groupindex					= (_XGROUPTYPE)characterlist->List[i].cGroup;
			g_pLocalUser->m_AllCharacterInfo[i].gender						= (_XGENDERTYPE)characterlist->List[i].cSex;
			g_pLocalUser->m_AllCharacterInfo[i].zoneuniqid					= characterlist->List[i].cZone;
			
			//_XLog("cn : %s, ci : %d, zi : %d", characterlist->List[i].cName, i, characterlist->List[i].cZone);

			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.upperbodyCategory	= (_XGI_FirstCategory)characterlist->List[i].cAmorType;
			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.lowerbodyCategory	= (_XGI_FirstCategory)characterlist->List[i].cPantsType;

			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.capCategory		= (_XGI_FirstCategory)characterlist->List[i].cCapType;
			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.adaeCategory		= (_XGI_FirstCategory)characterlist->List[i].cAdaeType;		

			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.backpackCategory	= (_XGI_FirstCategory)characterlist->List[i].cBackPackType;

			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.lfootCategory		= (_XGI_FirstCategory)characterlist->List[i].cShoesType;
			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.rfootCategory		= (_XGI_FirstCategory)characterlist->List[i].cShoesType;
			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.maskCategory		= (_XGI_FirstCategory)characterlist->List[i].cMaskType;

			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.face			= characterlist->List[i].cFace;
			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.hair			= characterlist->List[i].cHair;

			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.upperbody		= characterlist->List[i].sAmor;
			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.lowerbody		= characterlist->List[i].sPants;

			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.cap			= characterlist->List[i].sCap;
			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.adae			= characterlist->List[i].sAdae;
			
			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.lhand			= 0;
			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.rhand			= 0;

			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.backpack		= characterlist->List[i].sBackPack;

			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.lfoot			= characterlist->List[i].sShoes;
			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.rfoot			= characterlist->List[i].sShoes;

			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.mask			= characterlist->List[i].sMask;

#ifdef _XDEF_NEWANIMATIONLIST
			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.item1Category	= (_XGI_FirstCategory)characterlist->List[i].cWeaponType;			
			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.item1			= characterlist->List[i].sSelectWeaponID;
			g_pLocalUser->m_AllCharacterInfo[i].weaponStrength				= characterlist->List[i].ucWeaponStrength;
#endif			

			// ĳ���� ���� â���� ����� �Ⱥ��̹Ƿ� ���� ����� �������� �ʴ´�.

			g_pLocalUser->m_AllCharacterInfo[i].Set_innerlevel( characterlist->List[i].sInnerLevel );

			g_pLocalUser->m_AllCharacterInfo[i].Set_strength(characterlist->List[i].sStr);
			g_pLocalUser->m_AllCharacterInfo[i].Set_zen(characterlist->List[i].sZen);
			g_pLocalUser->m_AllCharacterInfo[i].Set_intelligence(characterlist->List[i].sIntelligence);
			g_pLocalUser->m_AllCharacterInfo[i].Set_constitution(characterlist->List[i].sConstitution);
			g_pLocalUser->m_AllCharacterInfo[i].Set_dexterity(characterlist->List[i].sDexterity);

			g_pLocalUser->m_AllCharacterInfo[i].Set_cClass(characterlist->List[i].cClass);
			g_pLocalUser->m_AllCharacterInfo[i].Set_cClassGrade( characterlist->List[i].cClassGrade);

			g_pLocalUser->m_AllCharacterInfo[i].lastposition.x				= characterlist->List[i].fPosX;
			g_pLocalUser->m_AllCharacterInfo[i].lastposition.y				= characterlist->List[i].fPosZ;
			g_pLocalUser->m_AllCharacterInfo[i].Set_gm_level( characterlist->List[i].cGM );
#ifdef _CHARAC_BLOCKING
			g_pLocalUser->m_AllCharacterInfo[i].blockingenddate				= characterlist->List[i].iBlockingEndDate;
#endif
//#ifdef _XTS_SERVERUNIFICATION	// ���� �Ϸ� ���Ŀ��� ��� ����
#ifdef _XTS_NEWCHARACLIST
			g_pLocalUser->m_AllCharacterInfo[i].ucChangeNameFlag			= characterlist->List[i].ucChangeNameFlag;
			g_pLocalUser->m_AllCharacterInfo[i].ucLeftFlag					= characterlist->List[i].ucLeftFlag;
#endif
		}
		g_pLocalUser->SelectCharacter(0);
	}
	else
	{
	}

	_XDWPACKETLOG("-- Receive player information ------------------------");
	_XDWPACKETLOG("-- Character count : %d ", characterlist->nCharac);
	_XDWPACKETLOG("-- Name		Nick		InnerExp");
	for(int i = 0 ; i < characterlist->nCharac ; ++i)
	{
		_XDWPACKETLOG("%2d %13s %13s %d", i, characterlist->List[i].cName, characterlist->List[i].cNick, characterlist->List[i].sInnerLevel);
	}

	if(((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.GetCurrentProcess() == &((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver)
	{
		// login process���� �޾��� �� 
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XPROCDEF_CONNECTEDSERVER, 0), (LPARAM)gHWnd);
	}
	else if(((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.GetCurrentProcess() == &((CEmperorOfDragonsApp*)gApp)->m_proc_createcharacter)
	{
		// create character process���� �޾��� �� 
		g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_SUCCESSCREATE, TRUE, _XDEF_CC_REQUESTCHARACTERLIST, 
								_XDEF_CC_REQUESTCHARACTERLIST, _XDEF_CC_REQUESTCHARACTERLIST);
		g_MessageBox.SetMessageColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else if(((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.GetCurrentProcess() == &((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter)
	{
		// select character processs���� �޾��� ��
		((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.SetCharacterInfoWindow();// 2004.06.03->oneway48 insert

	#ifdef _XDEF_SELECT_ANOTHERCHARACTER_080721_KUKURI
		if(m_bDeleteCharacter)
	#endif
		{
			g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_SUCCESSDELETE, TRUE, _XDEF_CS_REQUESTDELETECHARACTER);		
			g_MessageBox.SetMessageColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			m_bDeleteCharacter = FALSE;
		}
	}
	else if(((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.GetCurrentProcess() == &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame)
	{
		// main game process���� �޾��� ��
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_GOTOSELECTCHARACTER, 0), (LPARAM)gHWnd);
	}
	
	// ��Ʈ�� �˴ٿ� 
#ifdef _XDEF_SHUTDOWN_20061027
	_XLog(_T("[�˴ٿ� ���νð�] [%d]"), (int)characterlist->sFatigueWeight);
	if( characterlist->sFatigueWeight < 300 )
	{
		g_PlayLeftTime = (3600000*5) - (characterlist->sFatigueWeight*60000);
	}
	else
	{
		g_PlayLeftTime = 0;
	}

	#ifdef _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI

		if(g_pJinStatus_Window)
		{
			// �������� �˷��ִ� ���� �����Ѵ� - ����õ�� ����� üũ �߰�
			if(g_pLocalUser->m_AccountItemcType >= 0 && g_pLocalUser->m_AccountItemsID > 0)
			{
				// ����õ�� �����
				g_pJinStatus_Window->SetSDSState(characterlist->uiSDSMode);
				g_pJinStatus_Window->SetSDSSetTime(0);		// setting time �ʱ�ȭ
			}
			else
			{
				g_pJinStatus_Window->SetSDSState(2);
			}
		}

	#endif
	
#endif

#ifdef _XDEF_VINA_PCROOM_INFO
		if(characterlist->ucPCRoomInfo != 0)
		{
			// premium pc������ setting �Ѵ�.
			g_pLocalUser->SetPCroomState( _XDEF_PCROOM_PREMIUM, 1 );
			_XLog("VN : PC Room setting");
		}
#endif

#ifdef _XVNBILLINGSYSTEM
	g_LocalUserBillingInformations.BillMethod = characterlist->cBillMethod;
	
	if( characterlist->cBillMethod == _VNBILLING_BILLMETHOD_FREEOFCHARGE ||
		characterlist->cBillMethod == _VNBILLING_BILLMETHOD_FREEOFCHARGE_EVENT )
	{
		g_LocalUserBillingInformations.BillRemain = characterlist->dwRemainTime * 1000; // 1/1000�ʷ� ����

		TCHAR tempString[5];
		memset( tempString, 0, sizeof(TCHAR) * 5 );
		strncpy( tempString, characterlist->szExpireDate, 4);
		g_LocalUserBillingInformations.BillExpireYear = atoi( tempString );
		
		memset( tempString, 0, sizeof(TCHAR) * 5 );
		strncpy( tempString, characterlist->szExpireDate+4, 2);			
		g_LocalUserBillingInformations.BillExpireMonth = atoi( tempString );

		memset( tempString, 0, sizeof(TCHAR) * 5 );
		strncpy( tempString, characterlist->szExpireDate+6, 2);
		g_LocalUserBillingInformations.BillExpireDay = atoi( tempString );
		
		memset( tempString, 0, sizeof(TCHAR) * 5 );
		strncpy( tempString, characterlist->szExpireDate+8, 2);
		g_LocalUserBillingInformations.BillExpireTime = atoi( tempString );
		
		memset( tempString, 0, sizeof(TCHAR) * 5 );
		strncpy( tempString, characterlist->szExpireDate+10, 2);
		g_LocalUserBillingInformations.BillExpireMinute = atoi( tempString );
	}
	else if( characterlist->cBillMethod == _VNBILLING_BILLMETHOD_BASIC_FIXEDQUANTITYSYSTEM || 
			 characterlist->cBillMethod == _VNBILLING_BILLMETHOD_PREMIUM_FIXEDQUANTITYSYSTEM )
	{
		g_LocalUserBillingInformations.BillRemain = characterlist->dwRemainTime * 1000; // 1/1000�ʷ� ����
	}
	else
	{
		TCHAR tempString[5];
		memset( tempString, 0, sizeof(TCHAR) * 5 );
		strncpy( tempString, characterlist->szExpireDate, 4);
		g_LocalUserBillingInformations.BillExpireYear = atoi( tempString );
		
		memset( tempString, 0, sizeof(TCHAR) * 5 );
		strncpy( tempString, characterlist->szExpireDate+4, 2);			
		g_LocalUserBillingInformations.BillExpireMonth = atoi( tempString );

		memset( tempString, 0, sizeof(TCHAR) * 5 );
		strncpy( tempString, characterlist->szExpireDate+6, 2);
		g_LocalUserBillingInformations.BillExpireDay = atoi( tempString );
		
		memset( tempString, 0, sizeof(TCHAR) * 5 );
		strncpy( tempString, characterlist->szExpireDate+8, 2);
		g_LocalUserBillingInformations.BillExpireTime = atoi( tempString );
		
		memset( tempString, 0, sizeof(TCHAR) * 5 );
		strncpy( tempString, characterlist->szExpireDate+10, 2);
		g_LocalUserBillingInformations.BillExpireMinute = atoi( tempString );
	}
#endif
	
}

void _XNetwork::MSGCharacResult(MSG_CHARAC_RESULT* characterstatusresult)
{
	_XLog( "Character result : %d, %d", characterstatusresult->ucResult, characterstatusresult->ucPrevCommand );

	if(characterstatusresult->ucResult)
	{
		if(characterstatusresult->ucPrevCommand == MSG_NO_CHARAC_CREATE)
		{
			switch(characterstatusresult->ucResult)
			{
			case 1 :
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_EXISTEDCHARACTER), TRUE);
				}
				break;
			case 2 :
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_WARNNAME), TRUE);
				}
				break;
			case 3 :
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_WARNUSEPOINT), TRUE);
				}
				break;
			case 4 :
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_WARNITEM), TRUE);
				}
				break;
			case 5 :
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_WARNCREATE), TRUE);
				}
				break;
			}
		}
		else if( characterstatusresult->ucPrevCommand == MSG_NO_CHARAC_DELETE ) // ĳ���� ���� ����
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_CANNOTDELETE, TRUE, _XDEF_CS_CANCELDELETEBUTTON );
			g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );

#ifdef _XDEF_SELECT_ANOTHERCHARACTER_080721_KUKURI
			m_bDeleteCharacter = FALSE;
#endif
		}
		else if( characterstatusresult->ucPrevCommand == MSG_NO_CHARAC_SELECT ) // ĳ���� ���� ����
		{
			if(characterstatusresult->ucResult == 23)
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_REQUESTLATER), TRUE );//��� �Ŀ� �ٽ� �õ��Ͻʽÿ�. 
			}
			else
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_FAILCREATE, TRUE );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			}
		}
	}
}

void _XNetwork::MSGCharacInfo(MSG_CHARAC_INFO* characterinfo)
{
	if(((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.GetCurrentProcess() != &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame)
	{
		g_pLocalUser->m_CharacterInfo = g_pLocalUser->m_AllCharacterInfo[g_pLocalUser->m_SelectedCharacter];

		if(g_BattleLog.GetWriteLog())
		{
			g_BattleLog.Initialize(g_pLocalUser->m_CharacterInfo.charactername);

			// Battle log
			g_BattleLog.Print(_T("Account	%s\n"), g_pLocalUser->m_UserName);
			g_BattleLog.Print(_T("Character	%s\n"), g_pLocalUser->m_CharacterInfo.charactername);
			g_BattleLog.Print(_T("Group	%s\n"), g_JoinGroupName[g_pLocalUser->m_CharacterInfo.groupindex]);
			g_BattleLog.Print(_T("Level	%d\n"), g_pLocalUser->m_CharacterInfo.Get_innerlevel());
			
			// lable
			g_BattleLog.Print(_T("\nDate\tTime\tStr\tInt\tDex\tTargetGroup\tTargetName\tTargetIndex\t"));
			g_BattleLog.Print(_T("SkillIndex\tSkillName\tSkillLevel\tMartial#\tC-Type\tS-Type\tUsefulType\t"));
			g_BattleLog.Print(_T("Result\tAP1_Damage\tAP2_Damage\tAP3_Damage\tTotalDamage\t"));
			
			if(g_LanguageType == _XLANGUAGE_TYPE_KOREAN)
			{
				g_BattleLog.Print(_T("EffIndex1\tEffValue1\tEffTime1\tEffIndex2\tEffValue2\tEffTime2\tEffIndex1\tEffValue1\tEffTime3\t"));
				g_BattleLog.Print(_T("EffIndex4\tEffValue4\tEffTime4\tEffIndex5\tEffValue5\tEffTime5\tEffIndex6\tEffValue6\tEffTime6\t"));
				g_BattleLog.Print(_T("EffIndex7\tEffValue7\tEffTime7\tEffIndex8\tEffValue8\tEffTime8\t"));
				g_BattleLog.Print(_T("AdditiveEffIndex1\tAdditiveEffValue1\tAdditiveEffTime1\tAdditiveEffIndex2\tAdditiveEffValue2\tAdditiveEffTime2\t"));
				g_BattleLog.Print(_T("AdditiveEffIndex3\tAdditiveEffValue3\tAdditiveEffTime3\tAdditiveEffIndex4\tAdditiveEffValue4\tAdditiveEffTime4\t"));
			}

			g_BattleLog.Print(_T("\n"));
			g_BattleLog.Flush();
		}
	}

	g_pLocalUser->m_CharacterInfo.zoneuniqid = g_CurrentZoneInfoIndex = characterinfo->cZone;

	if( g_CurrentZoneInfoIndex <= 0 )
	{
		_XFatalError( _XGETINTERFACETEXT(ID_STRING_NETWORK_2331), g_CurrentZoneInfoIndex );
	}

	TCHAR	tstr[256];
	memset(tstr, 0, sizeof(tstr));
	_snprintf(tstr, sizeof(tstr), _XGETINTERFACETEXT(ID_STRING_NETWORK_MOVEZONE), g_ZoneInfoTable[g_CurrentZoneInfoIndex - 1].zonename);
	g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_NOTICE, _XGETINTERFACETEXT(ID_STRING_NETWORK_CONNECTSERVER), tstr, TRUE,
							_XDEF_CS_CANCELBUTTON, _XDEF_CS_CANCELBUTTON, _XDEF_CS_CANCELBUTTON);
	_XUpdateWindow();

	// ���� power
	g_pLocalUser->m_CharacterInfo.Set_current_lifepower( characterinfo->sCurLifePower );
	g_pLocalUser->m_CharacterInfo.Set_current_forcepower( characterinfo->sCurForcePower );
	g_pLocalUser->m_CharacterInfo.Set_current_concentrationpower( characterinfo->sCurConcentrationPower );	

	// max power
	g_pLocalUser->m_CharacterInfo.Set_max_lifepower(characterinfo->sMaxLifePower);
	g_pLocalUser->m_CharacterInfo.Set_max_forcepower(characterinfo->sMaxForcePower);
	g_pLocalUser->m_CharacterInfo.Set_max_concentrationpower(characterinfo->sMaxConcentrationPower);
	
	g_pLocalUser->m_MaxLifePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_lifepower() );
	g_pLocalUser->m_MaxForcePower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_forcepower() );
	g_pLocalUser->m_MaxConcentrationPower.Set_org( g_pLocalUser->m_CharacterInfo.Get_max_concentrationpower() );

	g_pLocalUser->m_CharacterInfo.Set_leftpoint( characterinfo->sLeftPoint);
	g_pLocalUser->m_CharacterInfo.Set_leftmasterypoint( characterinfo->sLeftMasteryPoint);

	g_pLocalUser->m_CharacterInfo.Set_jinlevel( characterinfo->uiLeft_Jin);
#ifdef _XTS_FAME
	g_pLocalUser->m_CharacterInfo.Set_Notoriety( characterinfo->iNotoriety);
#else
	g_pLocalUser->m_CharacterInfo.Set_gonglevel( characterinfo->uiLeft_Gong);
#endif
	g_pLocalUser->m_CharacterInfo.Set_retribution( characterinfo->sRetribution);
	g_pLocalUser->m_CharacterInfo.Set_honor( characterinfo->sHonor);
	g_pLocalUser->m_CharacterInfo.Set_showdown( characterinfo->sShowdowm);

	g_pLocalUser->m_CharacterInfo.Set_externalwoundrate( characterinfo->sWoundRate);
	g_pLocalUser->m_CharacterInfo.Set_internalwoundrate( characterinfo->sInsideWoundRate);
	g_pLocalUser->m_CharacterInfo.Set_fatiguerate( characterinfo->usFatigueRate);
	
	g_pLocalUser->m_CharacterInfo.Set_furyparameter( characterinfo->sFuryParameter);
	g_pLocalUser->m_CharacterInfo.Set_gm_level( characterinfo->cGM );
#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
	g_pLocalUser->m_CharacterInfo.cHindingMode	= characterinfo->ucHideMode;
#else
	g_pLocalUser->m_CharacterInfo.cHindingMode	= characterinfo->cHindingMode;
#endif

	_XWindow_Option* optionwindow = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
	if( optionwindow )
	{
		optionwindow->SetRankHidingState(g_pLocalUser->m_CharacterInfo.cHindingMode);		
	}

	// �ܻ��� ���� ���
/*	if(g_pLocalUser->m_CharacterInfo.Get_externalwoundrate() > 0)
	{
		if(g_pState_Window)
		{
			FLOAT percentage = (((FLOAT)characterinfo->sWoundRate)/(FLOAT)g_pLocalUser->m_MaxLifePower.Get_result())*100.0f;
//			FLOAT percentage = (((FLOAT)characterinfo->sWoundRate)/(FLOAT)g_pLocalUser->m_CharacterInfo.Get_max_lifepower())*100.0f;
			if(percentage >= 40.0f)
				g_pState_Window->InsertStateListFromExIn(_XST_EXTERNAL, 0, characterinfo->sWoundRate);
		}
	}

	// ������ ���� ���
	if(g_pLocalUser->m_CharacterInfo.Get_internalwoundrate() > 0)
	{
		if(g_pState_Window)
		{
			FLOAT percentage = (((FLOAT)characterinfo->sInsideWoundRate)/(FLOAT)g_pLocalUser->m_MaxForcePower.Get_result())*100.0f;
//			FLOAT percentage = (((FLOAT)characterinfo->sInsideWoundRate)/(FLOAT)g_pLocalUser->m_CharacterInfo.Get_max_forcepower())*100.0f;
			if(percentage >= 40.0f)
				g_pState_Window->InsertStateListFromExIn(_XST_INTERNAL, 0, characterinfo->sInsideWoundRate);
		}
	}
*/
	// ��ġ
	g_pLocalUser->m_NextRespawnPosition.x		= characterinfo->fInitPosX;
	g_pLocalUser->m_NextRespawnPosition.y		= 0.0f;
	g_pLocalUser->m_NextRespawnPosition.z		= characterinfo->fInitPosZ;
	g_pLocalUser->m_TargetPosition				= g_pLocalUser->m_NextRespawnPosition;

	if(characterinfo->cRespawnServerNo != 0)
	{
		g_pLocalUser->m_bSaveRespawnPos = TRUE;
		strcpy(g_pLocalUser->m_RespawnName, characterinfo->cRespawnPosName);
	}
	
	g_pLocalUser->m_CharacterInfo.winscore = characterinfo->iWinScore;
	g_pLocalUser->m_CharacterInfo.losescore = characterinfo->iLostScore;
	g_pLocalUser->m_CharacterInfo.tiescore = characterinfo->iTieScore;
	g_pLocalUser->m_CharacterInfo.seriesscore = characterinfo->iSeriesCount;
	g_pLocalUser->m_CharacterInfo.max_series_wincount = characterinfo->iMaxSeriesWinCount;
	g_pLocalUser->m_CharacterInfo.max_series_losecount = characterinfo->iMaxSeriesLoseCount;
	g_pLocalUser->m_CharacterInfo.abusecount = characterinfo->iAbuseCount;
	g_pLocalUser->m_CharacterInfo.prevmatchresult = characterinfo->cPrevMatchResult;

	g_pLocalUser->m_CharacterInfo.Set_cClass( characterinfo->cClass );
	g_pLocalUser->m_CharacterInfo.Set_cClassGrade( characterinfo->cClassGrade );
	g_pLocalUser->m_CharacterInfo.Set_iContribution( characterinfo->iContribution );

	// vital>0 �̸� animation setting - �װ� ���� �Ѿ���鼭 ��Ȱ ó��
	if(g_pLocalUser->m_CharacterInfo.Get_current_lifepower() > 0)
	{
		g_pLocalUser->m_CharacterInfo.animationdescript.motionclass = _XACTION_IDLE;
		g_pLocalUser->SetAnimation(_XACTION_IDLE);
		g_pLocalUser->m_ModelDescriptor.SetLockEndFrame(FALSE);
	}
#ifdef _XDEF_CASTLEBATTLE_MOD_CHARACINFO
	else
	{
		// ���� ���� ���
		if(characterinfo->ucPrevDeadFrom == en_dead_from_player)
		{
			// pk�� �׾���
			_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
			if(pRebirth_Window)
			{
				pRebirth_Window->m_bDeadInPK = TRUE;	// ������ â�� ������ field button disable �ȴ�
			}
		}
		else if(characterinfo->ucPrevDeadFrom == en_dead_from_monster_in_combat || characterinfo->ucPrevDeadFrom == en_dead_from_player_in_combat)
		{
			// ����� �߿� �׾����Ƿ� ��Ȱ �г�Ƽ �� ����
			g_pLocalUser->m_dwDeadCount = characterinfo->ucDeadCountInCombat;
			g_pLocalUser->m_dwDeadPenaltyTime = characterinfo->usLeftCombatPenaltyTime;

			_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
			if(pRebirth_Window)
			{
				pRebirth_Window->SetCastleBattleState(TRUE);
				pRebirth_Window->m_bDeadInCB = TRUE;
				
				if(characterinfo->ucPrevDeadFrom == en_dead_from_player_in_combat)
				{//Auhtor : ����� //breif : ����� PK ������ üũ ��������
					//pRebirth_Window->m_bDeadInPK = TRUE;
				}
			}
		}
	}
#endif
	
#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
	if(characterinfo->ucPeaceMode == 0)
#else
	if(characterinfo->cPeaceMode == 0)
#endif
	{
		g_pLocalUser->ResetPKMode();
		g_pLocalUser->SetBattleMode(_XBATTLE_PEACE);
	}
	else
	{
		g_pLocalUser->ResetPKMode();
		g_pLocalUser->SetBattleMode(_XBATTLE_BATTLE);
	}
	
#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
	if(characterinfo->ucClothView == 0)
	{
		// ġ�� �ǻ� ����
		g_bSelectClothesMask = TRUE;
	}
	else
	{
		//�Ϲ� �ǻ� ����
		g_bSelectClothesMask = FALSE;
	}

	g_pLocalUser->m_CharacterInfo.m_nClothView = characterinfo->ucClothView;
	g_pLocalUser->SetCharacterModel();
#endif

	// ��Ŷ ���� �ð� ���
	g_ServerTimeCode = characterinfo->iTimeCode;
	g_LocalServerTimeCode = timeGetTime();
	g_fServerTimeCode = 0.;
	g_SendTimerPacketTime = 0;

	//_XLog("Server Time : %d", g_ServerTimeCode);

	_XDWPACKETLOG("RECEIVE Server Time : %d", g_ServerTimeCode);

	::GetGameTime(characterinfo->iTimeCode, &g_ServerGameTime);
	
	((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_CurrentEnvironmentTime = g_ServerGameTime.Second * 1000;

	// StandingOBB ó���� ���¿� ���� �ð� ó���ؾ���

	g_pLocalUser->RecalcLocalUserParameter();
	
	//2004.06.24->oneway48 insert
	if(g_pJinStatus_Window)
	{
		if(g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience > 0)
		{
			if(g_pLocalUser->m_CharacterInfo.Get_jinlevel() >= g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience)
			{
				if(g_pLocalUser->m_CharacterInfo.Get_sublevel() < 12)
				{
					g_NetworkKernel.SendPacket(MSG_NO_LEVEL_UP_REQ);
				}
				else
				{
					g_pJinStatus_Window->SetButtonStatus(TRUE);
				}
			}
		}
		g_pJinStatus_Window->SetParameter();	
		// �ɷ�ġ�� ��ư Ȱ��ȭ  ----------------------------------=			
		if( g_pLocalUser->m_CharacterInfo.Get_leftpoint() > 0 || g_pLocalUser->m_CharacterInfo.Get_leftmasterypoint() > 0 )
		{
			g_pJinStatus_Window->m_AbilityUpButton->ShowWindow(TRUE);
			g_pJinStatus_Window->m_AbilityUpFrame = g_fLocalSystemTime;
			g_pJinStatus_Window->m_CurAbilityUpCount = 0;
		}
		else
		{
			g_pJinStatus_Window->m_AbilityUpButton->ShowWindow(FALSE);
		}
		// --------------------------------------------------------=
		
		// �г�/���ֻ��� clear - �г�/���� ���·� ���ӿ� �����ϴ� ���� ����
		g_pJinStatus_Window->ClearConvergence();

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
		g_pJinStatus_Window->SetClassIcon();	// class icon ����
#endif
	}		
	
	// JinStatus���� ȣ���
	//if(g_pCInfo_Window)
	//	g_pCInfo_Window->SetParameter();

//	if(g_pSkill_Window)
//		g_pSkill_Window->SetSkillButton();

	//g_NetworkKernel.SendPacket(MSG_NO_NIC_LIST_REQ_C_S);// ĳ���Ͱ� ���õǾ����� �� ĳ���Ͱ� ������ �ִ� ��ȣ ����Ʈ�� ��û�Ѵ�.

	if( ((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.GetCurrentProcess() == &((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter )
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_CharacterSelectedMode = TRUE;		
		((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_CharacterSelectedModeTimer = g_LocalSystemTime;

	}
	else
	{
		//����� ��忡�� ������ ���� �ε����� �ʱ�ȭ�Ѵ�.
		g_QuestScriptManager.m_indexRealZone = -1;
		g_QuestScriptManager.m_indexVirtualZone = -1;
		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_loading);
	}
}

void _XNetwork::MSGZoneRestart(MSG_ZONE_RESTART* zonerestart)
{
	if( g_pLocalUser->m_bIsBossBattle && zonerestart->cSeverNum != -1)
	{
		XProc_MainGame* pmaingameproc = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

		g_ChannelTempZoneInfoIndex = g_CurrentZoneInfoIndex; //ä�� �ʿ��� ���� ������ ���ư��� ���� ä�� ���� �����Ѵ�.
		g_CurrentZoneInfoIndex = zonerestart->cSeverNum ;
		
		g_pLocalUser->m_NextRespawnPosition = D3DXVECTOR3(zonerestart->fRestartPosX, 0, zonerestart->fRestartPosZ);
				
		pmaingameproc->m_ProcessLocalUserDieFlag = TRUE;
		pmaingameproc->m_UserDieScreenEffectFlag = FALSE;
		pmaingameproc->m_UserDieScreenAlphaLevel = 255;
		
		g_pLocalUser->m_bIsBossBattle				= FALSE;
		g_pLocalUser->m_bBossBattleEndProcess		= TRUE;
		
		g_pLocalUser->m_BossStartTime = 0;
		g_pLocalUser->m_BossTimeLimit = 0;
		
		g_pLocalUser->m_CharacterInfo.Set_current_lifepower(  zonerestart->sVitalPower );
		g_pLocalUser->m_CharacterInfo.Set_jinlevel( zonerestart->uiJin );
				
		g_LastPickedObject = NULL;
		g_LastPickedObjectType = _XPICKINGTARGET_NONE;
		g_pLocalUser->ResetAttackTarget();
		g_pLocalUser->ResetViewTarget();
		
		g_pLocalUser->m_ModelDescriptor.SetLockEndFrame(FALSE);
		g_pLocalUser->m_ModelDescriptor.PlayAnimation(TRUE);
		g_pLocalUser->ResetZoneData();
		g_pLocalUser->UpdatePosition();
		g_pLocalUser->RequestBattleMode(_XBATTLE_PEACE);
		g_pLocalUser->m_DyingStartTime = 0;
		g_pLocalUser->m_DyingTimer = 0;
		g_pLocalUser->m_LastCheckedDyingStartTime = -1;
		
		SendPacket(MSG_NO_ZONE_MOVE);
		
		g_pLocalUser->m_CharacterInfo.animationdescript.motionclass = _XACTION_IDLE;
		g_pLocalUser->SetAnimation(_XACTION_IDLE);
				
		//����� ��忡�� ������ ���� �ε����� �ʱ�ȭ�Ѵ�.
		g_QuestScriptManager.m_indexRealZone = -1;
		g_QuestScriptManager.m_indexVirtualZone = -1;

		pmaingameproc->m_MoveServer = TRUE;
		pmaingameproc->m_MovingGameServer = TRUE;
		pmaingameproc->m_LockMoveProcess = TRUE;				
		
		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_loading);
		return;		
	}
		
	XProc_MainGame* pmaingameproc = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

#ifdef ALL_RB_NPCSCRIPT_DEATH_070611_KUKURI
	// NPC â �ݱ�
	if( pmaingameproc->m_bNPCDialogMode || pmaingameproc->m_bNPCDialogWideView )
	{
		pmaingameproc->CloseNPCScriptWindow();
	}
#endif
#ifdef VN_RB_BOSSROOM_BUGFIX_070615_KUKURI
	if(pmaingameproc->m_bFunctionalObjectWideView)
	{
		_XWindow_EnterBossRoom* pEnterBossRoom_Window = (_XWindow_EnterBossRoom*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ENTERBOSSROOM);
		if(pEnterBossRoom_Window)
		{
			if(pEnterBossRoom_Window->GetShowStatus())
			{
				// �� �غ� ���̰�, ����̸�, ���
				if( g_pLocalUser->m_InParty )
				{
					if( g_pLocalUser->m_bWaitBossBattle )
					{
						g_NetworkKernel.SendPacket( MSG_NO_PARTY_ETC, 10 ); //����� ������� ��� 
					}				
				}
				pEnterBossRoom_Window->ShowWindow(FALSE);
				g_pInventory_Window->ShowWindow(FALSE);
				pmaingameproc->m_LockMoveProcess = FALSE;
			}
		}
	}
#endif
	
	g_pLocalUser->m_CharacterInfo.Set_current_lifepower( zonerestart->sVitalPower );
	g_pLocalUser->m_CharacterInfo.Set_jinlevel( zonerestart->uiJin );

	g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position = D3DXVECTOR3(zonerestart->fRestartPosX, 0, zonerestart->fRestartPosZ);
	
	_XOBBData* pLastPickedOBB = NULL;
	FLOAT height = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, pLastPickedOBB );
	g_pLocalUser->m_TargetPosition.y = g_pLocalUser->m_Position.y = height;
	g_pLocalUser->m_pGoundCollideOBB = pLastPickedOBB;
	
	g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;

	pmaingameproc->m_ProcessLocalUserDieFlag = TRUE;
	pmaingameproc->m_UserDieScreenEffectFlag = FALSE;
	pmaingameproc->m_UserDieScreenAlphaLevel = 255;

#ifdef _XDEF_USESCREEENMOTIONBLUR 
	if(g_ScrrenMotionBlurMode)
		g_ScrrenMotionBlurMode = FALSE;
#endif

	g_Moblist.disposeList();

	if(g_pLocalUser->m_UserState != _XDEF_USERSTATE_MATCH_FIGHT)
	{
		// �� �� �׾��� ������ ���� ����Ʈ ������ ����
		g_Userlist.disposeList();
	}

	// Local user�� log clear
	g_pLocalUser->m_CharacterLog.isAttack = FALSE;
	g_pLocalUser->m_CharacterLog.attackTime = 0;
	g_pLocalUser->m_CharacterLog.attackTargetType = 0;
	g_pLocalUser->m_CharacterLog.isKill = FALSE;
	g_pLocalUser->m_CharacterLog.killTargetType = 0;
	g_pLocalUser->m_CharacterLog.killTime = 0;
	g_pLocalUser->m_CharacterLog.isTrade = FALSE;
	g_pLocalUser->m_CharacterLog.tradeTime = 0;
	g_pLocalUser->m_CharacterLog.isTalk = FALSE;
	g_pLocalUser->m_CharacterLog.talkTime = 0;

	g_LastPickedObject = NULL;
	g_LastPickedObjectType = _XPICKINGTARGET_NONE;
	g_pLocalUser->ResetAttackTarget();
	g_pLocalUser->ResetViewTarget();
	g_pLocalUser->m_PrevMoveTime = 0;

	g_pLocalUser->m_ModelDescriptor.SetLockEndFrame(FALSE);
	g_pLocalUser->m_ModelDescriptor.PlayAnimation(TRUE);
	g_pLocalUser->ResetZoneData();
	g_pLocalUser->UpdatePosition();
	g_pLocalUser->m_DyingStartTime = 0;
	g_pLocalUser->m_DyingTimer = 0;
	g_pLocalUser->m_LastCheckedDyingStartTime = -1;

	g_pLocalUser->m_CharacterInfo.animationdescript.motionclass = _XACTION_IDLE;
	g_pLocalUser->SetAnimation(_XACTION_IDLE);

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

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
	if(g_pLocalUser->m_UserState != _XDEF_USERSTATE_MATCH_FIGHT 
		&& !g_AreaManager.FindPositionInArea(g_pLocalUser->m_Position, g_MatchEventManager.GetCurrentEventArea(), _XDEF_AREACATEGORY_PVPAREA))
#else
	if(g_pLocalUser->m_UserState != _XDEF_USERSTATE_MATCH_FIGHT)
#endif
	{
		// �� �߿��� state change ���� ����
		g_pLocalUser->RequestBattleMode(_XBATTLE_PEACE);
		SendPacket(MSG_NO_ZONE_MOVE);
	}

	if( g_pJinStatus_Window )
	{
		g_pJinStatus_Window->SetParameter();
		if(g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience > 0)
		{
			if(g_pLocalUser->m_CharacterInfo.Get_jinlevel() >= g_UserLevelTable[g_pLocalUser->m_CharacterInfo.Get_innerlevel() + 1].needjinexperience)
			{
				if(g_pLocalUser->m_CharacterInfo.Get_sublevel() < 12)
				{
					g_NetworkKernel.SendPacket(MSG_NO_LEVEL_UP_REQ);
				}
				else
				{
					g_pJinStatus_Window->SetButtonStatus(TRUE);
				}
			}
		}
	}		

	_XWindow_WorldMinimap* pMinimap_Window = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIMAPWINDOW);
	if(pMinimap_Window)
	{
		pMinimap_Window->SetRotateArrow(g_pLocalUser->m_RotateAngle + _X_PI);
		pMinimap_Window->SetLocalUserPosition();
	}
	
	// ��Ȱ�� ��� Rebirth ������ �ݱ�
	_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
	if(pRebirth_Window)
	{			
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321 // PK����ġ����, ������ PK����	
		if( pRebirth_Window->m_bEnablePKTracing )
		{			
			_XWindow_PKTracingRequest* pPKTracingRequest_Window = (_XWindow_PKTracingRequest*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PKTRACINGREQUEST);
			if(pPKTracingRequest_Window)
			{
				if( pPKTracingRequest_Window->m_cTargetName[0] != 0 )
					pPKTracingRequest_Window->ShowWindow(TRUE);
				
				pRebirth_Window->m_bEnablePKTracing = FALSE;
			}
		}
#endif
		if(pRebirth_Window->GetShowStatus())
		   pRebirth_Window->ShowWindow(FALSE);
		
		// ��� �̴ϰ����� �׾��� ���� ���� �߰� ó��
		_XWindow_TrainingMed* pMeditation_Window = (_XWindow_TrainingMed*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGMEDITATION);
		if(pMeditation_Window)
		{
			pMeditation_Window->OpenInterface();
		}
	}
	
	// ����� ���� ���� �ʱ�ȭ
	g_pLocalUser->m_dwDieStartTime = 0;
	g_pLocalUser->m_dwDeadPenaltyTime = 0;

	// ��� �޼��� �ð� �ʱ�ȭ
	_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
	if(pBloodPoint_Window)
	{
		pBloodPoint_Window->SetDeadMessageStartTime(0);
	}

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	// ä��â ��ġ ����
	_XWindow_ChattingDefault* defaultchatwindow = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );			
	if( defaultchatwindow )
	{
		defaultchatwindow->SetPositionDie(FALSE);
	}
#endif

	// ��Ȱ �α� ó��
	// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
	if( g_TutorialInfo )
	{
		if( !g_TutorialInfo->_XTutorial_019 )
		{
			_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
			pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_019);
			g_TutorialInfo->_XTutorial_019 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
			g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
		}
	}			
	// ----------------------------------------------------------------------------------------------------------------------------=
	g_pLocalUser->m_CharacterLog.isRevive = TRUE;
	g_pLocalUser->m_CharacterLog.reviveTime = g_ServerTimeCode;

#ifdef _ACCLAIM_IGAADSYSTEM
	
	#ifdef _ACCLAIM_IGAADSYSTEM_DISABLEDUPLICATE_AD_VIEW
		if( pRebirth_Window->m_VideoADCanceled )
		{
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_AcclaimADMode_DeathPhase	= _XACCLAIMAD_DEATHPHASE_STATICAD;
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_AcclaimADMode_MainGame	= _XACCLAIMAD_MAINGAME_DEATH;
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_DeathStaticBackgroundAlphaLevel = 160;
			g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_DEATH );
			g_IGAADWrapper.ShowWindow( TRUE );
			g_IGAADWrapper.StartDisplay();
		}
	#endif
#endif

}

void _XNetwork::MSGLogout(MSG_LOGOUT* logout)
{
	_XUser* pUser = (_XUser*)g_Userlist.FindUser(logout->usUniqueID);
	if(pUser)
	{
		_XDWPACKETLOG("Logout user : %d / %s", pUser->m_Uniqueid, pUser->m_CharacterInfo.charactername);
		g_Userlist.DeleteUser(logout->usUniqueID);
	}
}

void _XNetwork::MSGSVToGServer(MSG_SV_TOGSERVER movetoserver)
{	
	if( g_pLocalUser->m_bIsBossBattle )
	{
		g_pLocalUser->m_bIsBossBattle				= FALSE;
		g_pLocalUser->m_bBossBattleEndProcess		= FALSE; // GAMESTARTSIGNAL�� ����.
		g_pLocalUser->m_BossStartTime = 0;
		g_pLocalUser->m_BossTimeLimit = 0;
	}
		

	if( g_pLocalUser->m_InParty )
		g_pLocalUser->m_IsServerMoveInParty = TRUE;
	
	for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)
	{
		memset(g_pLocalUser->m_PartyInfo[i].playername, 0, sizeof(TCHAR)*13);
		g_pLocalUser->m_PartyInfo[i].playeruniqueid = 0;
		g_pLocalUser->m_PartyInfo[i].posX = 0;
		g_pLocalUser->m_PartyInfo[i].posZ = 0;
		g_pLocalUser->m_PartyInfo[i].serverno = 0;
		g_pLocalUser->m_PartyInfo[i].vitalpercent = 0;
		g_pLocalUser->m_PartyInfo[i].zenpercent = 0;
		g_pLocalUser->m_PartyInfo[i].innerlevel = 0;
		g_pLocalUser->m_PartyInfo[i].isLeader = FALSE;
		g_pLocalUser->m_PartyInfo[i].isValid = FALSE;
		
		DWORD windowid = _XDEF_WTITLE_MINIPARTY_MEMBER1 + i;
		if( windowid == _XDEF_WTITLE_MINIPARTY_MEMBER1 )
		{
			_XWindow_PartyMasterMiniBar* pMiniBar_Window = (_XWindow_PartyMasterMiniBar*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIPARTY_MEMBER1);
			if(pMiniBar_Window)
			{
				if(pMiniBar_Window->GetShowStatus())
					pMiniBar_Window->ShowWindow(FALSE);
			}
		}
		else
		{
			_XWindow_PartyMemberMiniBar* pMiniBar_Window = (_XWindow_PartyMemberMiniBar*)g_MainWindowManager.FindWindow(windowid);
			if(pMiniBar_Window)
			{
				if(pMiniBar_Window->GetShowStatus())
					pMiniBar_Window->ShowWindow(FALSE);
			}
		}
	}
	
	g_pLocalUser->m_InParty			 = FALSE;		
	g_pLocalUser->m_PartyMemberCount = 0;
	g_pLocalUser->m_PartySelfIndex   = -1;

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503			
	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
	if( pChatDefault_Window )
	{
		if( pChatDefault_Window->m_pChatFilterButton[2] )
		{
			pChatDefault_Window->m_pChatFilterButton[2]->EnableWindow( FALSE );
		}	
	}
#endif

	g_CurrentZoneInfoIndex = movetoserver.cZoneNumber;

	m_NextSERVER_ADDRESS = movetoserver.ulServerIP;
	m_NextSERVER_PORT = movetoserver.usServerPort;

	((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer = TRUE;
	
	DisconnectServer(TRUE);	
	InitializePacketBuffer();

	m_SERVER_ADDRESS = m_NextSERVER_ADDRESS;
	m_SERVER_PORT = m_NextSERVER_PORT;

	if(ConnectServer())
	{
		g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_CANCEL, ID_STRING_LOGINSERVER_LOGINGAMESERVER,
								TRUE, _XDEF_LS_CANCELLOGINSERVER, _XDEF_LS_CANCELLOGINSERVER, _XDEF_LS_CANCELLOGINSERVER);
		g_MessageBox.SetMessageColor();
		g_MessageBox.StartTimer(15000);
	}
	else
	{
		g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_CANCEL, ID_STRING_LOGINSERVER_CANNOTCONNECTGAMESERVER,
								TRUE, 0, 0, 0);
		g_MessageBox.SetMessageColor();
		g_MessageBox.StartTimer(15000);
	}
}

void _XNetwork::MSGZonePositioning(MSG_ZONE_POSITIONING* zonepositioning)
{
	g_pLocalUser->m_TargetPosition.x = g_pLocalUser->m_Position.x = zonepositioning->fInitPosX;
	g_pLocalUser->m_TargetPosition.y = g_pLocalUser->m_Position.y = 0.0f;
	g_pLocalUser->m_TargetPosition.z = g_pLocalUser->m_Position.z = zonepositioning->fInitPosZ;

	g_pLocalUser->SetAnimation(_XACTION_IDLE);

	_XOBBData* pLastPickedOBB = NULL;
	FLOAT height = g_LodTerrain.GetTerrainAndRideOBBHeight(g_pLocalUser->m_Position, pLastPickedOBB);
	g_pLocalUser->m_Position.y = height;
	g_pLocalUser->m_pGoundCollideOBB = pLastPickedOBB;
	g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;

	g_pLocalUser->UpdatePosition();

	g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(g_pLocalUser->m_Position.x,
														g_pLocalUser->m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA,
														g_pLocalUser->m_Position.z));
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
}

void _XNetwork::MSGZoneMove(MSG_ZONE_MOVE* charactermove)
{
	if(g_pLocalUser->m_Uniqueid == charactermove->usUniqueID)
	{
		VIEWPACKETLOG
			_XDWPACKETLOG("Message skip : move local user");

		return;
	}

	_XUser* pUser = g_Userlist.FindUser(charactermove->usUniqueID);

	if(!pUser)
	{
		FLOAT fLastAngle = 0.f;
		D3DXVECTOR3 vMoveVector(0, 0, 0);
		if(charactermove->fgotoX != charactermove->fX || charactermove->fgotoZ != charactermove->fZ)
		{
			fLastAngle = _XMeshMath_GetRotateRadian(D3DXVECTOR3(charactermove->fX, 0, charactermove->fZ), D3DXVECTOR3(charactermove->fgotoX, 0, charactermove->fgotoX));
			vMoveVector = D3DXVECTOR3(charactermove->fgotoX, 0, charactermove->fgotoX) - D3DXVECTOR3(charactermove->fX, 0, charactermove->fZ);
			D3DXVec3Normalize(&vMoveVector, &vMoveVector);
		}

		// ������ ������ ó�� �޾��� ���
		pUser = g_Userlist.CreateUser(charactermove->usUniqueID, charactermove->sAnimationNumber,
			fLastAngle, vMoveVector,
			D3DXVECTOR3(charactermove->fX, 0, charactermove->fZ),
			D3DXVECTOR3(charactermove->fgotoX, 0, charactermove->fgotoZ));
		
		if( pUser->m_CharacterInfo.gender == _XGENDER_MAN )
			pUser->m_ModelDescriptor.LinkBipedObject( &g_MaleBipedObject );
		else
			pUser->m_ModelDescriptor.LinkBipedObject( &g_FemaleBipedObject );

		pUser->m_ModelDescriptor.PlayAnimation(TRUE);

		g_LastRequestCharacterNameID = pUser->m_Uniqueid;
		pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
		SendPacket(MSG_NO_ZONE_REQ_CHARAC_INFO);
	}	

	if(pUser)
	{
		// ������ �����ϴ� ���, ó�� ���� ��� ��� ó���Ѵ� // Author : �ڼ���
		pUser->m_PrevPosition = pUser->m_Position;

		pUser->SetAnimationIndex( charactermove->sAnimationNumber );

		if(pUser->GetMotionClass() == _XACTION_MOVE && pUser->m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG)
		{
			//Author : ����� //biref: ��ų�� ���� ���� ����Ʈ�� ������µ�,, ���� ������ ���ε� ������ �����ش�,,
			// �׷���,, ����Ʈ�� �ִµ� ��� ������ �ʴ´ٸ�,, �߰��� �Ѵ�
			int effectid = _XGameItem::GetInchantEffectId( pUser->m_CharacterInfo.modeldescript.item1Category,
				pUser->m_CharacterInfo.modeldescript.item1, pUser->m_CharacterInfo.weaponStrength );

			if( effectid > 0 )
			{
				if( !pUser->m_ModelDescriptor.m_EffectManager->FindInstance(effectid) )
					pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
						effectid, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE );
			}

			/*
			if( fabs(pUser->m_Position.x - charactermove->fX) > 3.0f || fabs(pUser->m_Position.z - charactermove->fZ) > 3.0f ) 
			{		
				pUser->m_Position.x = charactermove->fX;
				pUser->m_Position.z = charactermove->fZ;
				pUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight(pUser->m_Position, pUser->m_pGoundCollideOBB);
			}
			*/
		}
		else
		{
			// �Ÿ��� �ʹ� �� ��� ���� �̵�
			if( fabs(pUser->m_Position.x - charactermove->fX) > 3.0f || fabs(pUser->m_Position.z - charactermove->fZ) > 3.0f ) 
			{		
				pUser->m_Position.x = charactermove->fX;
				pUser->m_Position.z = charactermove->fZ;
				pUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight(pUser->m_Position, pUser->m_pGoundCollideOBB);
			}
		}
				
		if(pUser->GetMotionClass() == _XACTION_MOVE)
		{
			if( pUser->m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG)
			{
				FLOAT dx = ( charactermove->fgotoX - pUser->m_Position.x);
				FLOAT dz = ( charactermove->fgotoZ - pUser->m_Position.z);
				FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );

				if( _distance > 0.5f )
				{
					pUser->m_TargetPosition.x = charactermove->fgotoX;
					pUser->m_TargetPosition.y = pUser->m_Position.y;
					pUser->m_TargetPosition.z = charactermove->fgotoZ;

					pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pUser->m_TargetPosition);
				}
			}
			else
			{
				pUser->m_TargetPosition.x = charactermove->fgotoX;
				pUser->m_TargetPosition.y = pUser->m_Position.y;
				pUser->m_TargetPosition.z = charactermove->fgotoZ;

				if( fabs(pUser->m_Position.x - pUser->m_TargetPosition.x) < EPSILON3 && 
					fabs(pUser->m_Position.z - pUser->m_TargetPosition.z) < EPSILON3 )
				{
					pUser->m_Position = pUser->m_PrevPosition;
				}

				//FLOAT prevrotateangle = pUser->m_RotateAngle;
				pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pUser->m_TargetPosition);
				
				/*
				if(pUser->m_RotateAngle == 0.0f && prevrotateangle != 0.0f )
				{
					pUser->m_RotateAngle = prevrotateangle;
					//_XInsertSystemMessage( 0, "%f %f, %f %f, %f", charactermove->fX, charactermove->fZ, charactermove->fgotoX, charactermove->fgotoZ, prevrotateangle );
				}
				*/

				/*

				D3DXVECTOR3 directionvector = pUser->m_TargetPosition - pUser->m_Position;

				if( directionvector.x > 0.0f || directionvector.z > 0.0f )
				{
					D3DXVec3Normalize(&pUser->m_MoveVector, &directionvector);
				}
				*/
			}
		}
		else if(pUser->GetMotionClass() == _XACTION_DYING)
		{
			pUser->m_TargetPosition.x = charactermove->fgotoX;
			pUser->m_TargetPosition.y = pUser->m_Position.y;
			pUser->m_TargetPosition.z = charactermove->fgotoZ;
		}
		else if(pUser->GetMotionClass() == _XACTION_IDLE)
		{
			//Author : ����� //biref: ��ų�� ���� ���� ����Ʈ�� ������µ�,, ���� ������ ���ε� ������ �����ش�,,
			// �׷���,, ����Ʈ�� �ִµ� ��� ������ �ʴ´ٸ�,, �߰��� �Ѵ�
			int effectid = _XGameItem::GetInchantEffectId( pUser->m_CharacterInfo.modeldescript.item1Category,
					pUser->m_CharacterInfo.modeldescript.item1, pUser->m_CharacterInfo.weaponStrength );

			if( effectid > 0 )
			{
				if( !pUser->m_ModelDescriptor.m_EffectManager->FindInstance(effectid) )
					pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
						effectid, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE );
			}

			pUser->m_TargetPosition.x = charactermove->fgotoX;
			pUser->m_TargetPosition.y = pUser->m_Position.y;
			pUser->m_TargetPosition.z = charactermove->fgotoZ;

			FLOAT dx = fabs(pUser->m_TargetPosition.x - pUser->m_Position.x);
			FLOAT dz = fabs(pUser->m_TargetPosition.z - pUser->m_Position.z);
			FLOAT leftlength = _XFC_sqrt(dx*dx + dz*dz);
			if( leftlength > 1.0f )
			{
#ifdef _XDEF_CASTLEBATTLE
				pUser->m_nIdleToMoveCnt++;
				if(pUser->m_nIdleToMoveCnt >= 2)
				{
					// 2�� �̻� position�� ���̰� ���� position setting �Ѵ�
					pUser->m_Position.x = pUser->m_TargetPosition.x;
					pUser->m_Position.z = pUser->m_TargetPosition.z;
					pUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight(pUser->m_Position, pUser->m_pGoundCollideOBB);
					
					//pUser->m_MoveVector.x = 0.0f;
					//pUser->m_MoveVector.y = 0.0f;
					//pUser->m_MoveVector.z = 0.0f;

					pUser->m_nIdleToMoveCnt = 0;
				}
#endif
				pUser->m_RotateAngle = _XMeshMath_GetRotateRadian(pUser->m_Position, pUser->m_TargetPosition);
				//pUser->m_MoveVector = pUser->m_TargetPosition - pUser->m_Position;
				//D3DXVec3Normalize(&pUser->m_MoveVector, &pUser->m_MoveVector);

				pUser->SetAnimation( _XACTION_MOVE );
			}
			else
			{
				pUser->m_TargetPosition.x = pUser->m_Position.x;
				pUser->m_TargetPosition.z = pUser->m_Position.z;
				
				//pUser->m_MoveVector.x = 0.0f;
				//pUser->m_MoveVector.y = 0.0f;
				//pUser->m_MoveVector.z = 0.0f;

#ifdef _XDEF_CASTLEBATTLE
				pUser->m_nIdleToMoveCnt = 0;
#endif
			}
		}
		else
		{
			pUser->m_TargetPosition.x = pUser->m_Position.x;
			pUser->m_TargetPosition.z = pUser->m_Position.z;

			//pUser->m_MoveVector.x = 0.0f;
			//pUser->m_MoveVector.y = 0.0f;
			//pUser->m_MoveVector.z = 0.0f;
#ifdef _XDEF_CASTLEBATTLE
			pUser->m_nIdleToMoveCnt = 0;
#endif
		}

		pUser->m_PrevProcessTime = g_LocalSystemTime;
		pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

		pUser->UpdatePosition();

		VIEWPACKETLOG
			_XDWPACKETLOG("RECEIVE MESSAGE : Character move [id:%d/%d] [%f]", charactermove->usUniqueID, charactermove->sAnimationNumber, pUser->m_RotateAngle);

#ifdef _XDWDEBUG
		if( g_EnableUserPacketFiltering )
		{
			if( strcmpi( pUser->m_CharacterInfo.charactername, g_PacketFilteringTargetName ) == 0 )
			{
				TCHAR _messagestring[64];
				memset(_messagestring, 0, sizeof(_messagestring));
				_snprintf( _messagestring, sizeof(_messagestring), "User move packet : X:%4.2f Y:%4.2f AnimIndex:%d Action:%d", charactermove->fgotoX, charactermove->fgotoZ, charactermove->sAnimationNumber, pUser->m_CharacterInfo.animationdescript.motionclass );
				_XDWSENDPACKETLOG( _messagestring );
			}
		}
#endif
		
	}
}

void _XNetwork::MSGZoneCharacInfo(MSG_ZONE_CHARAC_INFO* characterinfo)
{
	if(characterinfo->usUniqueID == g_pLocalUser->m_Uniqueid)
	{
		return;
	}

	_XUser* pUser = g_Userlist.FindUser(characterinfo->usUniqueID);
	if(pUser)
	{
		strcpy(pUser->m_CharacterInfo.charactername, characterinfo->cName);
		pUser->m_CharacterInfo.groupindex				= (_XGROUPTYPE)characterinfo->cGroup;
		pUser->m_CharacterInfo.gender					= (_XGENDERTYPE)characterinfo->cSex;
		pUser->m_CharacterInfo.Set_gm_level(characterinfo->cGM);
	
		pUser->m_CharacterInfo.modeldescript.upperbodyCategory	= (_XGI_FirstCategory)characterinfo->cAmorType;
		pUser->m_CharacterInfo.modeldescript.lowerbodyCategory	= (_XGI_FirstCategory)characterinfo->cPantsType;

		pUser->m_CharacterInfo.modeldescript.capCategory = (_XGI_FirstCategory)characterinfo->cCapType;
		pUser->m_CharacterInfo.modeldescript.adaeCategory = (_XGI_FirstCategory)characterinfo->cAdaeType;

		pUser->m_CharacterInfo.modeldescript.backpackCategory = (_XGI_FirstCategory)characterinfo->cBackPackType;

		pUser->m_CharacterInfo.modeldescript.lfootCategory = (_XGI_FirstCategory)characterinfo->cShoesType;
		pUser->m_CharacterInfo.modeldescript.rfootCategory = (_XGI_FirstCategory)characterinfo->cShoesType;

		pUser->m_CharacterInfo.modeldescript.face		= characterinfo->cFace;
		pUser->m_CharacterInfo.modeldescript.hair		= characterinfo->cHair;
		pUser->m_CharacterInfo.modeldescript.upperbody	= characterinfo->sAmor;
		pUser->m_CharacterInfo.modeldescript.lowerbody	= characterinfo->sPants;
		
		pUser->m_CharacterInfo.modeldescript.cap		= characterinfo->sCap;
		pUser->m_CharacterInfo.modeldescript.adae		= characterinfo->sAdae;
		
		pUser->m_CharacterInfo.modeldescript.lhand		= 0;
		pUser->m_CharacterInfo.modeldescript.rhand		= 0;
		
		pUser->m_CharacterInfo.modeldescript.backpack	= characterinfo->sBackPack;
		
		pUser->m_CharacterInfo.modeldescript.lfoot		= characterinfo->sShoes;
		pUser->m_CharacterInfo.modeldescript.rfoot		= characterinfo->sShoes;
		pUser->m_CharacterInfo.modeldescript.maskCategory = (_XGI_FirstCategory)characterinfo->cMaskType;
		pUser->m_CharacterInfo.modeldescript.mask		= characterinfo->sMask;
		
		pUser->m_HiperRunSkillID						= characterinfo->sHyperRunID;
		pUser->m_HiperRunStep							= characterinfo->ucHyperRunStep;
		pUser->m_CharacterInfo.m_OrganizationInfo.Reset();
		pUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex	= characterinfo->iOR_Index;	// �����ε���
		pUser->m_CharacterInfo.m_OrganizationInfo.m_cClass	= characterinfo->cOR_Class;		
#ifdef _XTS_ALIANCE_20061018
		pUser->m_CharacterInfo.m_AlienceInfo.Reset();
		pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex = characterinfo->iAllyIndex;
#endif
#ifdef _XTS_FAME
		if( characterinfo->ucHonorClass < 0 )
			characterinfo->ucHonorClass = 0;
		pUser->m_CharacterInfo.Set_honor( characterinfo->ucHonorClass );

		if( characterinfo->ucFameClass < 0 )
			characterinfo->ucFameClass = 0;						
		pUser->m_CharacterInfo.Set_Notoriety( characterinfo->ucFameClass );
#else
		if( characterinfo->iHonor > 0 )
		{
			if( characterinfo->iHonor > 2000000000 )
				characterinfo->iHonor = 2000000000;
		}
		else
		{
			if( characterinfo->iHonor < -2000000000 )		
				characterinfo->iHonor = -2000000000;
		}
		pUser->m_CharacterInfo.honor = characterinfo->iHonor;
#endif
		
#ifdef _XTS_ALIANCE_20061018
		if( pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 )
		{
			_XUSER_ALIENCE_GLOBAL_INFO* tempInfo = g_Userlist.SearchAlienceInfo(pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex);
			if( tempInfo )
			{
				pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex = tempInfo->m_iIndex;
				strcpy( pUser->m_CharacterInfo.m_AlienceInfo.m_cName, tempInfo->m_cName );		
				
				// ������ �����̸� ȸ�� �ƴϸ� ��ȸ�� �̴�
				if( pUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
				{				
					if( strcmp( pUser->m_CharacterInfo.charactername, tempInfo->m_cMasterName ) == 0 )
					{
						pUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_; 
					}
					else 
					{
						pUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_;
					}
				}
				else
				{				
					pUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
				}
			}
			else
			{
				// ��ȸ����Ʈ�� �ش� ��ȸ������ ������ ������ ��û�Ѵ�.
				// �ֺ� �������� ��ȸ ������ ��û�ϴ� ��Ŷ�� �ʿ��ϴ�.
				g_NetworkKernel.SendPacket(MSG_NO_ALLY_BASE, en_ally_mode_request_other_ally,pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex);
			}
		}
        else
        {
            pUser->m_CharacterInfo.m_AlienceInfo.Reset();
        }
#endif
		
		
		if( pUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
		{
			_XUSER_ORGANIZATION_GLOBAL_INFO* tempInfo = g_Userlist.SearchOrganizationInfo(pUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex);
			if( tempInfo )
			{
				pUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex = tempInfo->m_iIndex;
				pUser->m_CharacterInfo.m_OrganizationInfo.m_cProperty = tempInfo->m_cProperty;
				strcpy( pUser->m_CharacterInfo.m_OrganizationInfo.m_cName, tempInfo->m_cName );				
			}
			else
			{
				// ������������Ʈ�� �ش� ���������� ������ ������ ��û�Ѵ�.
				g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_INFO_REQ, MSG_ORGANIZATION_INFO_REQ::_REQ_OR_GLOBAL_INFO, pUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex);
			}
		}
        else
        {
            pUser->m_CharacterInfo.m_OrganizationInfo.Reset();
        }

		if( characterinfo->usSelectNicID == -1 )	// ���� ��ȣ ����
		{
			strcpy( pUser->m_CharacterInfo.m_NickName, characterinfo->cNickName );
			pUser->m_CharacterInfo.m_CurrentNIckIndex = -1;
		}
		else	// �� �� ��ȣ ����
		{
			for( int i = 0; i < g_MaxNickNameCount; ++i)
			{
				if( g_NickNameInfoTable[i].usIndex == characterinfo->usSelectNicID )
				{
					strcpy( pUser->m_CharacterInfo.m_NickName , g_NickNameInfoTable[i].cNickName );
					pUser->m_CharacterInfo.m_CurrentNIckIndex = i;					
					break;
				}
			}
		}

#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : ����� //breif : ��Ʈ ������
		//�ٸ� �ɸ����� ��Ʈ������ ����Ʈ�� �����ϰ� �ٽ� �߰��Ѵ�
		g_pSetIteminfo->UpdateAnotherUserEffect( pUser, characterinfo->sSetVisualEffect1, characterinfo->sSetVisualEffect2 );
#endif

		// ���� �����۰� ���� ��� ���� ���� ����Ʈ ����
		bool changeflag = true;
		if( pUser->m_CharacterInfo.modeldescript.item1Category == (_XGI_FirstCategory)characterinfo->cWeaponType &&
			pUser->m_CharacterInfo.modeldescript.item1 == characterinfo->sUseWeapon &&
			pUser->m_CharacterInfo.weaponStrength == characterinfo->ucWeaponStrength )
		{
			changeflag = false;			
		}
			
		if( changeflag )
		{
			int effectid = _XGameItem::GetInchantEffectId( pUser->m_CharacterInfo.modeldescript.item1Category,
					pUser->m_CharacterInfo.modeldescript.item1, pUser->m_CharacterInfo.weaponStrength );

			if( effectid > 0 )
			{
				pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(effectid);
			}
		}

		pUser->m_CharacterInfo.modeldescript.item1Category = (_XGI_FirstCategory)characterinfo->cWeaponType;
		pUser->m_CharacterInfo.weaponStrength = characterinfo->ucWeaponStrength;
		//_XLog("Item1Category : %s %d", characterinfo->cName, characterinfo->cWeaponType );
		
		if(pUser->m_AttackResult.skillType == _XSI_USEFULTYPE_HIDDENMA)
		{
			pUser->m_CurrentWeaponID = characterinfo->sUseWeapon;
		}
		else
		{
			pUser->m_CharacterInfo.modeldescript.item1		= characterinfo->sUseWeapon;
			
			int effectid = _XGameItem::GetInchantEffectId( pUser->m_CharacterInfo.modeldescript.item1Category,
					pUser->m_CharacterInfo.modeldescript.item1, pUser->m_CharacterInfo.weaponStrength );

			if( changeflag )
			{
				if( effectid > 0 )
				{
					pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
						effectid, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE );
				}
			}
		}

		if( pUser->m_CharacterInfo.gender == _XGENDER_MAN )
			pUser->m_ModelDescriptor.LinkBipedObject( &g_MaleBipedObject );
		else
			pUser->m_ModelDescriptor.LinkBipedObject( &g_FemaleBipedObject );

		
#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
		pUser->m_CharacterInfo.m_nClothView = characterinfo->ucClothView;
#endif

		pUser->SetCharacterModel(TRUE);		// animation change ���� ����
		pUser->m_ModelDescriptor.SetDrawSwordEffect( TRUE );
		pUser->m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
		pUser->m_LastRecivedMessageTime = g_LocalSystemTime;

		switch(characterinfo->cCharacState)
		{
		case 0 :
			{
				// Normal
				if(pUser->m_ModelDescriptor.m_EffectManager)
					pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_PLAYERID_BERSERK);
			}
			break;
		case 1 :
			{
				// ��� ����
				if(pUser->GetMotionClass() != _XACTION_DYING)
				{
					if(pUser->m_CharacterInfo.modeldescript.item1 == 0)
						pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
					else
					{
#ifdef _XTS_ITEM_OPTIMIZATION
						if( _XGI_FC_WEAPON == GetEqualItemType(pUser->m_CharacterInfo.modeldescript.item1Category) )
						{
							_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(pUser->m_CharacterInfo.modeldescript.item1Category);
							if( WeaponItem )
							{
								switch(WeaponItem[pUser->m_CharacterInfo.modeldescript.item1].cSecondType)
								{
								case _XGI_SC_WEAPON_SHORT :
									if(WeaponItem[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
									else if(WeaponItem[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(pUser->m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_PEACE);
										else
											pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_BATTLE);
									}
									else
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SHORTWEAPON);
									break;
								case _XGI_SC_WEAPON_LONG :
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_LONGWEAPON);
									break;
								case _XGI_SC_WEAPON_SOFT :
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
									break;
								case _XGI_SC_WEAPON_HIDDEN :
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
									break;
								case _XGI_SC_WEAPON_MUSICAL :
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
									break;
								case _XGI_SC_WEAPON_SPECIAL :
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
									break;
								}
							}
						}
#else
						if( pUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
						{
							switch(g_WeaponItemProperty[pUser->m_CharacterInfo.modeldescript.item1].cSecondType)
							{
							case _XGI_SC_WEAPON_SHORT :
								if(g_WeaponItemProperty[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								else if(g_WeaponItemProperty[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(pUser->m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_PEACE);
									else
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_BATTLE);
								}
								else
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SHORTWEAPON);
								break;
							case _XGI_SC_WEAPON_LONG :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_LONGWEAPON);
								break;
							case _XGI_SC_WEAPON_SOFT :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_HIDDEN :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_MUSICAL :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_SPECIAL :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							}
						}
						else if( pUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
						{
							switch(g_WeaponItemProperty2[pUser->m_CharacterInfo.modeldescript.item1].cSecondType)
							{
							case _XGI_SC_WEAPON_SHORT :
								if(g_WeaponItemProperty2[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								else if(g_WeaponItemProperty2[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(pUser->m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_PEACE);
									else
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_BATTLE);
								}
								else
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SHORTWEAPON);
								break;
							case _XGI_SC_WEAPON_LONG :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_LONGWEAPON);
								break;
							case _XGI_SC_WEAPON_SOFT :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_HIDDEN :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_MUSICAL :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_SPECIAL :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							}
						}
						else if( pUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
						{
							switch(g_WeaponItemProperty3[pUser->m_CharacterInfo.modeldescript.item1].cSecondType)
							{
							case _XGI_SC_WEAPON_SHORT :
								if(g_WeaponItemProperty3[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								else if(g_WeaponItemProperty3[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(pUser->m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_PEACE);
									else
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_BATTLE);
								}
								else
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SHORTWEAPON);
								break;
							case _XGI_SC_WEAPON_LONG :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_LONGWEAPON);
								break;
							case _XGI_SC_WEAPON_SOFT :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_HIDDEN :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_MUSICAL :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_SPECIAL :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							}
						}
#endif
					}
				}
			}
			break;
		case 2 :
			{
				// ȥ�� ����
				pUser->SetDieMode();
			}
			break;
		case 3 :
			{
				// �г� ����
			}
			break;
		case 4 :
			{
				// ���� ����
				if(pUser->m_ModelDescriptor.m_EffectManager)
				{
					if(!pUser->m_ModelDescriptor.m_EffectManager->FindInstance(_XDEF_PLAYERID_BERSERK))
					{
						pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
								_XDEF_PLAYERID_BERSERK, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);
					}
				}
			}
			break;
		}

		// Condition list ó��
		DWORD conditionOld = 0, conditionNew = 0;
		DWORD resultOld = 0, resultNew = 0;
		for(int i = 1 ; i < 33 ; ++i)
		{
			conditionOld = pUser->m_CharacterInfo.conditionState >> (i-1);
			resultOld = conditionOld & 0x00000001;

			conditionNew = characterinfo->dwConditionState >> (i-1);
			resultNew = conditionNew & 0x00000001;

			if(resultOld == 1 && resultNew == 0)
			{
				// visual effect ����
				for(int ii = 0 ; ii < 5 ; ++ii)
				{
					if(g_CharacterConditionTable[i-1].visualEffect[ii] > 0)
					{
						pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_CharacterConditionTable[i-1].visualEffect[ii]);
					}
				}
				pUser->m_StateList.DeleteConditionState(pUser->m_Uniqueid, i);
			}
			if(resultOld == 0 && resultNew == 1)
			{
				// visual effect �߰�
				for(int ii = 0 ; ii < 5 ; ++ii)
				{
					if(g_CharacterConditionTable[i-1].visualEffect[ii] > 0)
					{
						D3DXMATRIX offsetmat;
						D3DXMatrixIdentity(&offsetmat);
						pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
							g_CharacterConditionTable[i-1].visualEffect[ii], &offsetmat);
					}
				}
				pUser->m_StateList.InsertConditionState(pUser->m_Uniqueid, i);
			}
		}
		pUser->m_CharacterInfo.conditionState = characterinfo->dwConditionState;
		
		pUser->Set_m_MoveSpeedFromEffect( characterinfo->cMoveSpeed );
		pUser->Set_m_AttackSpeedFromEffect( characterinfo->cAttackSpeed );
		
#ifdef _XTS_PK
		if(characterinfo->ucGamingStatePK == 1)
			pUser->m_BattleMode = _XBATTLE_PK;
		else
			pUser->m_BattleMode = pUser->m_CharacterInfo.animationdescript.idletype;

		if(characterinfo->ucGamingStatePE == 1)
		{
			pUser->m_bPEMode = TRUE;

			if(pUser->m_ModelDescriptor.m_EffectManager)
			{
				if(!pUser->m_ModelDescriptor.m_EffectManager->FindInstance(_XDEF_INTERFACEID_PK_PICKING))
				{
					D3DXMATRIX offsetmat;
					D3DXMatrixIdentity(&offsetmat);
					pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_PK_PICKING, &offsetmat);
				}
			}
		}
#endif

#ifdef _XTS_SAFETYZONE
		MSG_STATE_CHANGE_SAFEREGION_NOTICE safenotice;
		memset(&safenotice, 0, sizeof(safenotice));

		safenotice.ucSafeRegionState = characterinfo->ucGamingStateSafe;
		safenotice.usCharacterUID = pUser->m_Uniqueid;
		MSGStateChangeSafeRegionNotice(&safenotice);
#endif

#ifdef _XDEF_PARTY_NOTAVAILABLE
		if(characterinfo->ucGamingStateAbsent == 0)
		{
			// ����
			pUser->m_bOut = FALSE;
		}
		else if(characterinfo->ucGamingStateAbsent == 1)
		{
			// �ڸ����
			pUser->m_bOut = TRUE;
//			pUser->SetAnimationIndex(�ɱ�);
		}
#endif

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
		if(characterinfo->ucGamingStateFM == 0)
		{
			pUser->m_bEnterMatchEvent = FALSE;
		}
		else if(characterinfo->ucGamingStateFM == 1)
		{
			pUser->m_bEnterMatchEvent = TRUE;
		}
#endif

#ifdef _XTS_VIEWPKTARGETLEVEL
		pUser->m_CharacterInfo.Set_innerlevel( characterinfo->sInnerLevel );
#endif

#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
		pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_NONE;

		pUser->m_CharacterInfo.m_b4Eff_ResMinus = FALSE;
		pUser->m_CharacterInfo.m_b4Eff_ResPlus = FALSE;
		pUser->m_CharacterInfo.m_b4Eff_ResHon = FALSE;
		pUser->m_CharacterInfo.m_b4Eff_ResSin = FALSE;

		int damage[4];
		int maxcnt = 0;
		memset(damage, 0, sizeof(damage));

		if(characterinfo->uc4Eff_MaxDam_minus == 1)
		{
			damage[0] = 1;
			maxcnt++;
			pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_MINUS;
		}
		if(characterinfo->uc4Eff_MaxDam_plus == 1)
		{
			damage[1] = 1;
			maxcnt++;
			pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_PLUS;
		}
		if(characterinfo->uc4Eff_MaxDam_hon == 1)
		{
			damage[2] = 1;
			maxcnt++;
			pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_HON;
		}
		if(characterinfo->uc4Eff_MaxDam_sin == 1)
		{
			damage[3] = 1;
			maxcnt++;
			pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_SIN;
		}

		if(maxcnt >= 2)
		{
			int rad = (_XRandom() % maxcnt) + 1;
			int index = 0;

			for(int i = 0 ; i < 4 ; ++i)
			{
				if(damage[i] == 1)
				{
					index++;
					if(rad == index)
					{
						break;
					}
				}
			}
			if(i < 4)
			{
				switch(i)
				{
				case 0 :
					pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_MINUS;
					break;
				case 1 :
					pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_PLUS;
					break;
				case 2 :
					pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_HON;
					break;
				case 3 :
					pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_SIN;
					break;
				}
			}
		}

		if(characterinfo->uc4Eff_Res_minus == 1)
			pUser->m_CharacterInfo.m_b4Eff_ResMinus = TRUE;
		if(characterinfo->uc4Eff_Res_plus == 1)
			pUser->m_CharacterInfo.m_b4Eff_ResPlus = TRUE;
		if(characterinfo->uc4Eff_Res_hon == 1)
			pUser->m_CharacterInfo.m_b4Eff_ResHon = TRUE;
		if(characterinfo->uc4Eff_Res_sin == 1)
			pUser->m_CharacterInfo.m_b4Eff_ResSin = TRUE;
#endif

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
		if(characterinfo->usFMNick == 0)
		{
			// reset
			if(g_pRewardNick_Window)
			{
				g_pRewardNick_Window->SetNickIndex(0);
//				g_pRewardNick_Window->ShowWindow(FALSE);
			}
		}
		else
		{
			if(g_pRewardNick_Window)
			{
				g_pRewardNick_Window->SetNickIndex(characterinfo->usFMNick);
//				g_pRewardNick_Window->ShowWindow(TRUE);
			}
		}

		pUser->m_nWinnerNickName = characterinfo->usFMNick;
#endif
		
	}
	else
	{
		// ������ ������ ó�� �޾��� ���
		pUser = g_Userlist.CreateUser(characterinfo->usUniqueID, _XACTION_IDLE,
			0, D3DXVECTOR3(0, 0, 0),
			D3DXVECTOR3(characterinfo->fCurPosX, 0, characterinfo->fCurPosZ),
			D3DXVECTOR3(characterinfo->fCurPosX, 0, characterinfo->fCurPosZ));

		strcpy(pUser->m_CharacterInfo.charactername, characterinfo->cName);
		pUser->m_CharacterInfo.groupindex				= (_XGROUPTYPE)characterinfo->cGroup;
		pUser->m_CharacterInfo.gender					= (_XGENDERTYPE)characterinfo->cSex;

		pUser->m_CharacterInfo.modeldescript.upperbodyCategory	= (_XGI_FirstCategory)characterinfo->cAmorType;
		pUser->m_CharacterInfo.modeldescript.lowerbodyCategory	= (_XGI_FirstCategory)characterinfo->cPantsType;

		pUser->m_CharacterInfo.modeldescript.capCategory = (_XGI_FirstCategory)characterinfo->cCapType;
		pUser->m_CharacterInfo.modeldescript.adaeCategory = (_XGI_FirstCategory)characterinfo->cAdaeType;

		pUser->m_CharacterInfo.modeldescript.backpackCategory = (_XGI_FirstCategory)characterinfo->cBackPackType;

		pUser->m_CharacterInfo.modeldescript.lfootCategory = (_XGI_FirstCategory)characterinfo->cShoesType;
		pUser->m_CharacterInfo.modeldescript.rfootCategory = (_XGI_FirstCategory)characterinfo->cShoesType;
		pUser->m_CharacterInfo.modeldescript.item1Category = (_XGI_FirstCategory)characterinfo->cWeaponType;
		pUser->m_CharacterInfo.modeldescript.maskCategory = (_XGI_FirstCategory)characterinfo->cMaskType;
		pUser->m_CharacterInfo.modeldescript.mask		= characterinfo->sMask;
	
		pUser->m_CharacterInfo.modeldescript.face		= characterinfo->cFace;
		pUser->m_CharacterInfo.modeldescript.hair		= characterinfo->cHair;
		pUser->m_CharacterInfo.modeldescript.upperbody	= characterinfo->sAmor;
		pUser->m_CharacterInfo.modeldescript.lowerbody	= characterinfo->sPants;
		
		pUser->m_CharacterInfo.modeldescript.cap		= characterinfo->sCap;
		pUser->m_CharacterInfo.modeldescript.adae		= characterinfo->sAdae;
		
		pUser->m_CharacterInfo.modeldescript.lhand		= 0;
		pUser->m_CharacterInfo.modeldescript.rhand		= 0;
		
		pUser->m_CharacterInfo.modeldescript.backpack	= characterinfo->sBackPack;
		
		pUser->m_CharacterInfo.modeldescript.lfoot		= characterinfo->sShoes;
		pUser->m_CharacterInfo.modeldescript.rfoot		= characterinfo->sShoes;
		pUser->m_CharacterInfo.modeldescript.item1		= characterinfo->sUseWeapon;

		pUser->m_CharacterInfo.weaponStrength = characterinfo->ucWeaponStrength;

		int effectid = _XGameItem::GetInchantEffectId( pUser->m_CharacterInfo.modeldescript.item1Category,
						pUser->m_CharacterInfo.modeldescript.item1, pUser->m_CharacterInfo.weaponStrength );
		if( effectid > 0 )
		{
			pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
																effectid, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE );
		}				
		
		pUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex	= characterinfo->iOR_Index;	// �����ε���

        //Author : ����� //breif : ��ȸ�� ������ �˻縦 Ŭ������ ������ �ϴµ� ���⿣ �װ� ����..-_- ������
        pUser->m_CharacterInfo.m_OrganizationInfo.m_cClass = characterinfo->cOR_Class;

		if( pUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
		{
			_XUSER_ORGANIZATION_GLOBAL_INFO* tempInfo = g_Userlist.SearchOrganizationInfo(pUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex);
			if( tempInfo )
			{
				pUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex = tempInfo->m_iIndex;
				pUser->m_CharacterInfo.m_OrganizationInfo.m_cProperty = tempInfo->m_cProperty;
				strcpy( pUser->m_CharacterInfo.m_OrganizationInfo.m_cName, tempInfo->m_cName );
			}
			else
			{
				// ������������Ʈ�� �ش� ���������� ������ ������ ��û�Ѵ�.
				g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_INFO_REQ, MSG_ORGANIZATION_INFO_REQ::_REQ_OR_GLOBAL_INFO, pUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex);			
			}
		}
        else
        {
            pUser->m_CharacterInfo.m_OrganizationInfo.Reset();
        }
		
#ifdef _XTS_ALIANCE_20061018
		pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex = characterinfo->iAllyIndex;
		if( pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 )
		{
			_XUSER_ALIENCE_GLOBAL_INFO* tempInfo = g_Userlist.SearchAlienceInfo(pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex);
			if( tempInfo )
			{
				pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex = tempInfo->m_iIndex;
				strcpy( pUser->m_CharacterInfo.m_AlienceInfo.m_cName, tempInfo->m_cName );			
                
            //Author : ����� //breif : ��ȸ�� ������ �˻縦 Ŭ������ ������ �ϴµ� ���⿣ �װ� ����..-_-���� �� ���߳�
                if( pUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
				{				
					if( strcmp( pUser->m_CharacterInfo.charactername, tempInfo->m_cMasterName ) == 0 )
					{
						pUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_; 
					}
					else 
					{
						pUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_;
					}
				}
				else
				{				
					pUser->m_CharacterInfo.m_AlienceInfo.m_cClass = _XUSERALIENCEINFO::_UNIT_CLASS_DEFAULT_UNIT_;
				}
			}
			else
			{
				// ��ȸ����Ʈ�� �ش� ��ȸ������ ������ ������ ��û�Ѵ�.
				// �ֺ� �������� ��ȸ ������ ��û�ϴ� ��Ŷ�� �ʿ��ϴ�.
				g_NetworkKernel.SendPacket(MSG_NO_ALLY_BASE, en_ally_mode_request_other_ally,pUser->m_CharacterInfo.m_AlienceInfo.m_iIndex);
			}
		}
        else
        {
            pUser->m_CharacterInfo.m_AlienceInfo.Reset();
        }
#endif
		
		if( pUser->m_CharacterInfo.gender == _XGENDER_MAN )
			pUser->m_ModelDescriptor.LinkBipedObject( &g_MaleBipedObject );
		else
			pUser->m_ModelDescriptor.LinkBipedObject( &g_FemaleBipedObject );
		
		pUser->SetCharacterModel();
		pUser->m_ModelDescriptor.SetDrawSwordEffect( TRUE );
		pUser->m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
		pUser->m_LastRecivedMessageTime = g_LocalSystemTime;
		
		switch(characterinfo->cCharacState)
		{
		case 0 :
			{
				// Normal
				if(pUser->m_ModelDescriptor.m_EffectManager)
					pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_PLAYERID_BERSERK);
			}
			break;
		case 1 :
			{
				// ��� ����
				if(pUser->GetMotionClass() != _XACTION_DYING)
				{
					if(pUser->m_CharacterInfo.modeldescript.item1 == 0)
						pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
					else
					{
#ifdef _XTS_ITEM_OPTIMIZATION
						if( _XGI_FC_WEAPON == GetEqualItemType(pUser->m_CharacterInfo.modeldescript.item1Category) )
						{
							_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(pUser->m_CharacterInfo.modeldescript.item1Category);
							if( WeaponItem )
							{
								switch(WeaponItem[pUser->m_CharacterInfo.modeldescript.item1].cSecondType)
								{
								case _XGI_SC_WEAPON_SHORT :
									if(WeaponItem[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
									else if(WeaponItem[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(pUser->m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_PEACE);
										else
											pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_BATTLE);
									}
									else
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SHORTWEAPON);
									break;
								case _XGI_SC_WEAPON_LONG :
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_LONGWEAPON);
									break;
								case _XGI_SC_WEAPON_SOFT :
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
									break;
								case _XGI_SC_WEAPON_HIDDEN :
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
									break;
								case _XGI_SC_WEAPON_MUSICAL :
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
									break;
								case _XGI_SC_WEAPON_SPECIAL :
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
									break;
								}
							}
						}
#else
						if( pUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
						{
							switch(g_WeaponItemProperty[pUser->m_CharacterInfo.modeldescript.item1].cSecondType)
							{
							case _XGI_SC_WEAPON_SHORT :
								if(g_WeaponItemProperty[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								else if(g_WeaponItemProperty[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(pUser->m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_PEACE);
									else
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_BATTLE);
								}
								else
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SHORTWEAPON);
								break;
							case _XGI_SC_WEAPON_LONG :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_LONGWEAPON);
								break;
							case _XGI_SC_WEAPON_SOFT :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_HIDDEN :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_MUSICAL :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_SPECIAL :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							}
						}
						else if( pUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
						{
							switch(g_WeaponItemProperty2[pUser->m_CharacterInfo.modeldescript.item1].cSecondType)
							{
							case _XGI_SC_WEAPON_SHORT :
								if(g_WeaponItemProperty2[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								else if(g_WeaponItemProperty2[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(pUser->m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_PEACE);
									else
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_BATTLE);
								}
								else
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SHORTWEAPON);
								break;
							case _XGI_SC_WEAPON_LONG :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_LONGWEAPON);
								break;
							case _XGI_SC_WEAPON_SOFT :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_HIDDEN :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_MUSICAL :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_SPECIAL :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							}
						}
						else if( pUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
						{
							switch(g_WeaponItemProperty3[pUser->m_CharacterInfo.modeldescript.item1].cSecondType)
							{
							case _XGI_SC_WEAPON_SHORT :
								if(g_WeaponItemProperty3[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								else if(g_WeaponItemProperty3[pUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(pUser->m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_PEACE);
									else
										pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_BATTLE);
								}
								else
									pUser->SetAnimationIndex(_XAN_DYINGREPEAT_SHORTWEAPON);
								break;
							case _XGI_SC_WEAPON_LONG :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_LONGWEAPON);
								break;
							case _XGI_SC_WEAPON_SOFT :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_HIDDEN :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_MUSICAL :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							case _XGI_SC_WEAPON_SPECIAL :
								pUser->SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
								break;
							}
						}
#endif
					}
				}
			}
			break;
		case 2 :
			{
				// ȥ�� ����
				pUser->SetDieMode();
			}
			break;
		case 3 :
			{
				// �г� ����
			}
			break;
		case 4 :
			{
				// ���� ����
				if(pUser->m_ModelDescriptor.m_EffectManager)
					pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
					_XDEF_PLAYERID_BERSERK, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE);
			}
			break;
		}

		pUser->m_CharacterInfo.conditionState = characterinfo->dwConditionState;
		DWORD condition = 0, result = 0;
		for(int i = 1 ; i < 33 ; ++i)
		{
			condition = characterinfo->dwConditionState >> (i-1);
			result = condition & 0x00000001;
						
			if(result == 1)
			{
				pUser->m_StateList.InsertConditionState(pUser->m_Uniqueid, i);
			}
		}
		
		pUser->Set_m_MoveSpeedFromEffect( characterinfo->cMoveSpeed );
		pUser->Set_m_AttackSpeedFromEffect( characterinfo->cAttackSpeed );

#ifdef _XTS_PK
		if(characterinfo->ucGamingStatePK == 1)
			pUser->m_BattleMode = _XBATTLE_PK;
		else
			pUser->m_BattleMode = pUser->m_CharacterInfo.animationdescript.idletype;

		if(characterinfo->ucGamingStatePE == 1)
		{
			pUser->m_bPEMode = TRUE;

			if(pUser->m_ModelDescriptor.m_EffectManager)
			{
				if(!pUser->m_ModelDescriptor.m_EffectManager->FindInstance(_XDEF_INTERFACEID_PK_PICKING))
				{
					D3DXMATRIX offsetmat;
					D3DXMatrixIdentity(&offsetmat);
					pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_PK_PICKING, &offsetmat);
				}
			}
		}
#endif

#ifdef _XTS_SAFETYZONE
		MSG_STATE_CHANGE_SAFEREGION_NOTICE safenotice;
		memset(&safenotice, 0, sizeof(safenotice));

		safenotice.ucSafeRegionState = characterinfo->ucGamingStateSafe;
		safenotice.usCharacterUID = pUser->m_Uniqueid;
		MSGStateChangeSafeRegionNotice(&safenotice);
#endif

#ifdef _XTS_VIEWPKTARGETLEVEL
		pUser->m_CharacterInfo.Set_innerlevel( characterinfo->sInnerLevel );
#endif
		
#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
		pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_NONE;

		pUser->m_CharacterInfo.m_b4Eff_ResMinus = FALSE;
		pUser->m_CharacterInfo.m_b4Eff_ResPlus = FALSE;
		pUser->m_CharacterInfo.m_b4Eff_ResHon = FALSE;
		pUser->m_CharacterInfo.m_b4Eff_ResSin = FALSE;

		int damage[4];
		int maxcnt = 0;
		memset(damage, 0, sizeof(damage));

		if(characterinfo->uc4Eff_MaxDam_minus == 1)
		{
			damage[0] = 1;
			maxcnt++;
			pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_MINUS;
		}
		if(characterinfo->uc4Eff_MaxDam_plus == 1)
		{
			damage[1] = 1;
			maxcnt++;
			pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_PLUS;
		}
		if(characterinfo->uc4Eff_MaxDam_hon == 1)
		{
			damage[2] = 1;
			maxcnt++;
			pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_HON;
		}
		if(characterinfo->uc4Eff_MaxDam_sin == 1)
		{
			damage[3] = 1;
			maxcnt++;
			pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_SIN;
		}

		if(maxcnt >= 2)
		{
			int rad = (_XRandom() % maxcnt) + 1;
			int index = 0;

			for(int i = 0 ; i < 4 ; ++i)
			{
				if(damage[i] == 1)
				{
					index++;
					if(rad == index)
					{
						break;
					}
				}
			}
			if(i < 4)
			{
				switch(i)
				{
				case 0 :
					pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_MINUS;
					break;
				case 1 :
					pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_PLUS;
					break;
				case 2 :
					pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_HON;
					break;
				case 3 :
					pUser->m_CharacterInfo.m_4Eff_MaxDam = _XEN_4EFF_SIN;
					break;
				}
			}
		}

		if(characterinfo->uc4Eff_Res_minus == 1)
			pUser->m_CharacterInfo.m_b4Eff_ResMinus = TRUE;
		if(characterinfo->uc4Eff_Res_plus == 1)
			pUser->m_CharacterInfo.m_b4Eff_ResPlus = TRUE;
		if(characterinfo->uc4Eff_Res_hon == 1)
			pUser->m_CharacterInfo.m_b4Eff_ResHon = TRUE;
		if(characterinfo->uc4Eff_Res_sin == 1)
			pUser->m_CharacterInfo.m_b4Eff_ResSin = TRUE;
#endif

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
		if(characterinfo->usFMNick == 0)
		{
			// reset
			if(g_pRewardNick_Window)
			{
				g_pRewardNick_Window->SetNickIndex(0);
//				g_pRewardNick_Window->ShowWindow(FALSE);
			}
		}
		else
		{
			if(g_pRewardNick_Window)
			{
				g_pRewardNick_Window->SetNickIndex(characterinfo->usFMNick);
//				g_pRewardNick_Window->ShowWindow(TRUE);
			}
		}

		pUser->m_nWinnerNickName = characterinfo->usFMNick;
#endif
		
		VIEWPACKETLOG
			_XDWPACKETLOG("New user added [%-8d %4.4f %4.4f]", characterinfo->usUniqueID, characterinfo->fCurPosX, characterinfo->fCurPosZ);		
	}
}

void _XNetwork::ProcessChatSpecialCode( MSG_ZONE_CHAT_NORMAL* chatnormal )
{
	_XUser* pUser = g_Userlist.FindUser( chatnormal->cName );

	if(!pUser) 
	{
		pUser = (_XUser*)g_pLocalUser;
	}


	if( strstr( chatnormal->cSay, _T("enteredgroom")) )		// not use string
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.StopSpecialBGMPlay();
	}
	else if( strstr( chatnormal->cSay, _T("entergroom") ) )	// not use string
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.SetSpecialBGMPlay( _T("bgm_wedding3.ogg") );
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2918),_XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);		//��ȥ ������� ���۵Ǿ����ϴ�.
	}	
	else if( strstr( chatnormal->cSay, _T("enteredbride") ) )	// not use string
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.StopSpecialBGMPlay();
	}
	else if( strstr( chatnormal->cSay, _T("enterbride") ) )	// not use string
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.SetSpecialBGMPlay( _T("bgm_wedding4.ogg") );
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2918),_XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);		//��ȥ ������� ���۵Ǿ����ϴ�.
	}	
	else if( strstr( chatnormal->cSay, _T("endedmarch") ) )	// not use string
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.StopSpecialBGMPlay();
	}
	else if( strstr( chatnormal->cSay,_T("beginmarch") ) )	// not use string
	{
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.SetSpecialBGMPlay( _T("bgm_wedding5.ogg") );
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2919),_XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);//"��ȥ �������� ���۵Ǿ����ϴ�."
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2920),_XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);//"�κ��� ��ȥ�� �������� ���ϵ帳�ϴ�."
		InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2921),_XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);//"������ ����Ͻð� �ູ�ϼ���."
	}
	else if( strstr( chatnormal->cSay, _T("salute") ) )	// not use string 
	{
		_XPlayInterfaceSound(g_PVPBattleFireworkSoundIndex);

		for( int i = 0; i < 15; ++i )
		{
			D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
			offsetmat._41 += (FLOAT)((rand() % 30) - 15);
			offsetmat._42 -= -1.98 + ((FLOAT)(rand() % 30) / 10.0f);
			offsetmat._43 += (FLOAT)((rand() % 30) - 15);
			
			int randindex = 0;
			if( i >= 10 ) randindex = 2;
			else if( i >= 5 ) randindex = 1;

			g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_MONSTERID_firework_001 + randindex, &offsetmat );
		}
	}
	else if( strstr( chatnormal->cSay, _T("flower1") ) )	// not use string 
	{
		g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 500, &pUser->m_ModelDescriptor.m_Position );		
	}
	else if( strstr( chatnormal->cSay, _T("flower2")) )	// not use string  
	{
		g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 501, &pUser->m_ModelDescriptor.m_Position );
	}
	else if( strstr( chatnormal->cSay, _T("flower3") ) )	// not use string 
	{
		g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 502, &pUser->m_ModelDescriptor.m_Position );
	}
	else if( strstr( chatnormal->cSay, _T("deleteweddinghall") ) )	// not use string
	{
		for( int i = 0; i < 12; ++i )
		{
			g_WorldEffectInstanceManager.DeleteInstance( 503 );
			g_WorldEffectInstanceManager.DeleteInstance( 504 );
			g_WorldEffectInstanceManager.DeleteInstance( 505 );
			g_WorldEffectInstanceManager.DeleteInstance( 500 );
			g_WorldEffectInstanceManager.DeleteInstance( 501 );
			g_WorldEffectInstanceManager.DeleteInstance( 506 );
		}
	}
	else if( strstr( chatnormal->cSay, _T("createweddinghall") ) )	// not use string
	{
		D3DXMATRIX dropposition1, dropposition2, rotmat;
		D3DXVECTOR3 curpos, terrainnormal;
		D3DXMatrixIdentity(&dropposition1);
		_XOBBData* pObbData = NULL;

		curpos = g_pLocalUser->m_Position;

		D3DXMatrixRotationY( &rotmat, _X_RAD( -45.0f ) );

		D3DXVECTOR2 stpos1  = D3DXVECTOR2( -209.7835f, -154.3210f );
		D3DXVECTOR2 endpos1 = D3DXVECTOR2( -195.0362f, -170.5534f );
		D3DXVECTOR2 stpos2  = D3DXVECTOR2( -180.4233f, -184.9498f );
		D3DXVECTOR2 endpos2 = D3DXVECTOR2( -167.3074f, -198.6263f );
		D3DXVECTOR2 normal1 = endpos1 - stpos1;
		D3DXVECTOR2 normal2 = endpos2 - stpos2;
		D3DXVec2Normalize( &normal1, &normal1 );
		D3DXVec2Normalize( &normal2, &normal2 );

		FLOAT dx = (endpos1.x - stpos1.x);
		FLOAT dz = (endpos1.y - stpos1.y);
		FLOAT fdiststep1 = _XDistance( dx, dz ) / 7.0f;

		dx = (endpos2.x - stpos2.x);
		dz = (endpos2.y - stpos2.y);
		FLOAT fdiststep2 = _XDistance( dx, dz ) / 7.0f;

		int centerheight = g_LodTerrain.GetTerrainAndOBBHeightWithCurrentHeight( curpos, pObbData, terrainnormal );
		D3DXMATRIX centermat, rotmat2;
		
		D3DXMatrixIdentity( &centermat );
		centermat._41 = -188.4231f;
		centermat._42 = centerheight + 5.0f;
		centermat._43 = -176.7230f;
		g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 500, &centermat );
		g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 501, &centermat );

		g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 505, &centermat );
		
		for( int i = 0; i < 8; ++i )
		{	
			D3DXMatrixIdentity( &centermat );
			centermat._41 = -188.4231f;
			centermat._42 = centerheight;
			centermat._43 = -176.7230f;
			D3DXMatrixRotationY( &rotmat2, _X_RAD( i * 45.0f + 45.0f ) );
			D3DXMatrixMultiply( &centermat, &rotmat2, &centermat);
			g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 506, &centermat );
		}

		for( i = 0; i < 7; ++i )
		{
			D3DXVECTOR2 nextpos = stpos1 + (normal1 * ((FLOAT)i*fdiststep1));
			D3DXVECTOR2 nextpos2 = stpos2 + (normal2 * ((FLOAT)i*fdiststep2));

			D3DXMatrixIdentity(&dropposition1);
			curpos.x = dropposition1._41 = nextpos.x;
			curpos.z = dropposition1._43 = nextpos.y;
			curpos.y = 50.0f;
						
			dropposition1._42 = g_LodTerrain.GetTerrainAndOBBHeightWithCurrentHeight( curpos, pObbData, terrainnormal );			
			D3DXMatrixMultiply( &dropposition1, &rotmat, &dropposition1 );

			D3DXMatrixIdentity(&dropposition2);
			curpos.x = dropposition2._41 = nextpos2.x;
			curpos.z = dropposition2._43 = nextpos2.y;
			curpos.y = 50.0f;
			
			dropposition2._42 = g_LodTerrain.GetTerrainAndOBBHeightWithCurrentHeight( curpos, pObbData, terrainnormal );			

			D3DXMatrixMultiply( &dropposition2, &rotmat, &dropposition2 );

			if( i % 2 )
			{
				g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 503, &dropposition1 );
				g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 503, &dropposition2 );
			}
			else
			{
				g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 504, &dropposition1 );
				g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 504, &dropposition2 );
			}

			if( !(i % 4) )
			{
				g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 505, &dropposition1 );
				g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 505, &dropposition2 );
			}

			if( !(i % 3) )
			{
				if( i == 3 || i == 9 )
				{
					g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 500, &dropposition1 );
					g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 500, &dropposition2 );
				}
				else 
				{
					g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 501, &dropposition1 );
					g_WorldEffectInstanceManager.InsertNewInstance( &g_WorldEffectGroupManager, 501, &dropposition2 );
				}
			}
		}
	}
	else if( strstr( chatnormal->cSay, _T("day") ) )	// ��
	{
		g_EnvironmentManager.m_EnvironmentTime = _XDEF_ENVTIMESTARTTIME_DAY;
	}
	else if( strstr( chatnormal->cSay, _T("nightfall") ) )	// ������
	{
		g_EnvironmentManager.m_EnvironmentTime = _XDEF_ENVTIMESTARTTIME_NIGHTFALL;
	}
	else if( strstr( chatnormal->cSay, _T("night") ) )	// ��
	{
		g_EnvironmentManager.m_EnvironmentTime = _XDEF_ENVTIMESTARTTIME_NIGHT;
	}
	
}

void _XNetwork::MSGZoneChatNormal(MSG_ZONE_CHAT_NORMAL* chatnormal)
{	

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	_XWindow_BlackList* pBlackList_Window = (_XWindow_BlackList*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_BLACKLISTWINDOW );
#endif

	// ��Ʈ�� �˴ٿ� 
#ifdef _XDEF_SHUTDOWN_20061027			
	if( chatnormal->cMode == 50 )
	{
		int lefttime = atoi(chatnormal->cSay);

		if( lefttime > 0 )
		{
			g_PlayLeftTime = lefttime * 3600000;

			g_PlayTime = g_LocalSystemTime;
			
			TCHAR szBuffer[256];
			memset( szBuffer, 0, sizeof(szBuffer) );
			if( lefttime != 0 )
			{
				_snprintf( szBuffer, sizeof(szBuffer), _XGETINTERFACETEXT(ID_STRING_NEW_3145), lefttime, lefttime );
			}
			else
			{
				_snprintf( szBuffer, sizeof(szBuffer), _XGETINTERFACETEXT(ID_STRING_NEW_3146), lefttime );
			}
			
			g_NetworkKernel.InsertChatString( szBuffer, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
		}
		else
		{
			g_PlayLeftTime = 0;

			TCHAR szBuffer[256];
			memset( szBuffer, 0, sizeof(szBuffer) );

			_snprintf( szBuffer, sizeof(szBuffer), _XGETINTERFACETEXT(ID_STRING_NEW_3146), g_PlayLeftTime );
			//g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_3161), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			g_NetworkKernel.InsertChatString( szBuffer, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			//g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_3148), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			//PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_GAMECLOSE, 0), (LPARAM)gHWnd);
			
			
//			// ������ ���� ����
//			_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
//			if( pRequestParty_Window )
//				pRequestParty_Window->m_RequestGroupButton->EnableWindow(FALSE);
//			
//			// ������ Ż�� ����
//			g_pGroup_Window->m_pBtnGroupLeave->EnableWindow( FALSE );
//
//			// ���� ����Ʈ �ο� �Ұ�
//			for( int i = 0; i < 5; i++ )
//			{		
//				g_pCInfo_Window->m_ModifyButton[i]->EnableWindow( FALSE );
//			}
			
			
		}
//		g_PlayTime = g_LocalSystemTime;
//		
//		TCHAR szBuffer[256];
//		memset( szBuffer, 0, sizeof(TCHAR)*256 );
//		if( lefttime != 0 )
//		{
//			sprintf( szBuffer, _XGETINTERFACETEXT(ID_STRING_NEW_3145), lefttime, lefttime );
//		}
//		else
//		{
//			sprintf( szBuffer, _XGETINTERFACETEXT(ID_STRING_NEW_3146), lefttime );
//		}
//
//		g_NetworkKernel.InsertChatString( szBuffer, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);

		return;
	}
#endif

	if( chatnormal->cMode == 126 ) // Golden time event message
	{
		InsertChatString( chatnormal->cSay, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
		return;
	}

	int CheckAdminAttrib = (chatnormal->cMode / _XDEF_CHATMESSAGEMODE_ADMINNOTICE);
	int CheckGlobalAttrib = (chatnormal->cMode / _XDEF_CHATMESSAGEMODE_GLOBALNOTICE);
	
	if(CheckAdminAttrib == 1)
	{
		TCHAR	chatbuffer[256];
		memset(chatbuffer, 0, sizeof(TCHAR)*256);
		int chatstringsize = chatnormal->header.sLength - 18;
		if( chatstringsize > 0 )
			strncpy(chatbuffer, chatnormal->cSay, chatstringsize );
		
		_XWindow_AdminNotice* pAdminNotice_Window = (_XWindow_AdminNotice*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NOTICEWINDOW);
		if(pAdminNotice_Window)
		{
			// NPC Script mode �϶��� ���� �������� ��ġ�� ���̵� �� �������� �̵�
			if( ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_bNPCDialogMode || 
				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_bNPCDialogWideView )
			{
				pAdminNotice_Window->MoveWindow( (gnWidth>>1)-155, ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_TopScreenVertex[0].y );
			}
			
			pAdminNotice_Window->SetNoticeMessage(chatbuffer);
		}
	}
	else
	{
		if( strncmp( chatnormal->cSay, _T("$%^"), 3 ) == 0 )
		{
			ProcessChatSpecialCode( chatnormal );
			return;
		}

		_XWindow_ChattingDefault* pChat_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHATTINGWINDOWDEFAULT);
		
		int chatlistslot = _XCHATMODE_NORMAL;
		if(chatnormal->cMode == 16)
		{
			chatlistslot = _XCHATMODE_PARTY;
		}
		else if(chatnormal->cMode == 17)
		{
			chatlistslot = _XCHATMODE_SUBCLAN;
		}
#ifdef _XDEF_ZONECHATTING_20070108
		else if(chatnormal->cMode == 15)
		{
			// �ɼ�â���� ������ȭ ���Ⱑ ���õǸ� ������ȭ�� ǥ������ �ʴ´�.
			if( g_bCommunityOptionDisableZoneChatting ) return;

			chatlistslot = _XCHATMODE_ZONE;
		}
#endif
#ifdef _XDEF_CASHITEM_SHOUT
		else if(chatnormal->cMode >= 96 && chatnormal->cMode <= 99)
		{
			chatlistslot = _XCHATMODE_SHOUT;
		}
#endif
		if(g_Chatlist.getitemcount() > _XDEF_CHATLIST_QUESIZE)
		{
			g_Chatlist.deleteItem(0);
		}
		
		BOOL	localmessage = FALSE;
		TCHAR	chatbuffer[256];
		memset(chatbuffer, 0, sizeof(TCHAR)*256);
		
		int chatstringsize = chatnormal->header.sLength - 18;
		
		if( chatstringsize > 0 )
			strncpy(chatbuffer, chatnormal->cSay, chatnormal->header.sLength - 18);

		if(strcmpi(chatnormal->cName, g_pLocalUser->m_CharacterInfo.charactername) == 0)
			localmessage = TRUE;
		
		if( g_pLocalUser->m_Lock.chatlock == 1)		
		{
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2332),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			g_pLocalUser->CharacterStateChatLock(chatbuffer);
		}
		
		int		tabwidth;
		TCHAR	tempbuffer[256];
		memset(tempbuffer, 0, sizeof(TCHAR)*256);

		if( chatnormal->cMode != 33 )	// ������°� �ƴҶ��� 		
			strcpy(tempbuffer, chatnormal->cName);

#ifdef _XTS_ALIANCE_20061018	
		
		//������ �����϶��� �̸� �ڿ� (����)�̶�� ���δ�.
	#ifdef ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA
		if(chatnormal->cMode == 18)
		{
			if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
			{
				if( g_pGroup_Window )
				{
					if( !g_pGroup_Window->IsMember( chatnormal->cName ) ) // ���� ���� �ƴϰ� ���� ��ȸ�϶��� (����) �߰�.
					{
						strcat(tempbuffer, _XGETINTERFACETEXT(ID_STRING_NEW_3128));
					}
				}
			}
		}
	#else
		if(chatnormal->cMode == 18)
		{
			strcat(tempbuffer, _XGETINTERFACETEXT(ID_STRING_NEW_3128));
		}
	#endif //ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA
#endif	//_XTS_ALIANCE_20061018

		strcat(tempbuffer, _T(":"));
		tabwidth = g_XBaseFont->GetWidth(tempbuffer);
		strcat(tempbuffer, chatbuffer);
				
		if(!localmessage)
		{

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
			BOOL blacklistuser = pBlackList_Window->FindCharacter( chatnormal->cName );	
			
			if( blacklistuser )
			{
				LPTSTR pStr = strstr( tempbuffer, chatnormal->cSay ); 
				if( pStr )
				{
					while( *pStr != NULL )
					{
						if( *pStr != _T(' ') )
						{
							*pStr = _T('*');
						}

						pStr++;
					}
				}
			}
#endif

			_XStringSplitInfo splitinfo;
			int viewsize = pChat_Window->GetWindowSize().cx - 28;
			
#ifdef _XDEF_NEWVERSONFILETERING
			g_WordFilteringManager.Filtering( tempbuffer,_XDEF_WORDFILETERINGTYPE_CHANGE | _XDEF_WORDFILETERINGTYPE_ALWAYS );
#else
			g_WordFilteringManager.Filtering( tempbuffer, _XDEF_WORDFILETERINGTYPE_ALWAYS );
#endif

			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->SplitString(tempbuffer, viewsize, splitinfo, tabwidth, _XSTRINGSPLITFUNCTION_SPACECHAR);
			}
			else
			{
				g_XBaseFont->SplitString(tempbuffer, viewsize, splitinfo, tabwidth);
			}
			
			TCHAR	splitstring[256];
			DWORD	attrib;
			for(int i = 0 ; i < splitinfo.splitcount ; ++i)
			{
				memset(splitstring, 0, sizeof(TCHAR)*256);
				strncpy(splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
				
				if(i != 0)
					attrib = (tabwidth << 16 | chatnormal->cMode);
				else
					attrib = chatnormal->cMode;
				
				if( chatnormal->cMode == 32 )// �ӼӸ� ���
				{
					attrib = _XDEF_CHATMESSAGECOLOR_WHISPER;
				}
#ifdef _XDEF_ZONECHATTING_20070108
				else if(chatnormal->cMode == 15)
				{
					attrib = _XDEF_CHATMESSAGECOLOR_ZONECHAT;
				}
#endif
				else if( chatnormal->cMode == 16 )
				{
					attrib = _XDEF_CHATMESSAGECOLOR_PARTY;
				}
				else if( chatnormal->cMode == 17 )
				{
					attrib = _XDEF_CHATMESSAGECOLOR_USERCLAN;
				}
#ifdef _XTS_ALIANCE_20061018

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
				else if(chatnormal->cMode == 18)
				{
					attrib = _XDEF_CHATMESSAGECOLOR_ALLY;
				}
#else
				else if(chatnormal->cMode == 18)
				{
					attrib = _XDEF_CHATMESSAGECOLOR_USERCLAN;
				}
#endif

#endif
#ifdef _XDEF_CASHITEM_SHOUT
				else if(chatnormal->cMode >= 96 && chatnormal->cMode <= 99)
				{
					attrib = _XDEF_CHATMESSAGECOLOR_SHOUT;
				}
#endif
				
				_XStringItem* chatmessage = new _XStringItem(splitstring, attrib, g_LocalSystemTime);
				if(chatmessage)
				{
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503		
					chatmessage->Set_Attribute3( blacklistuser );
#endif

					if( i == 0 )
					{
						chatmessage->Set_Attribute2( strlen(chatnormal->cName) );
					}
					else
					{
						chatmessage->Set_Attribute2( 0x00000000 );
					}

					g_Chatlist.resetList();
					g_Chatlist.insertItem(chatmessage);
				}
			}

			// Set chat balloon
			_XUser* pUser = g_Userlist.FindUser(chatnormal->cName);
			if(pUser)
			{
				pUser->m_ChatBalloonObject.m_MessageColor = g_ChattingStringColor[0];
				pUser->m_ChatBalloonObject.SetMessage(tempbuffer);
			}
		}
		else
		{
			g_pLocalUser->m_ChatBalloonObject.m_MessageColor = g_ChattingStringColor[0];
			g_pLocalUser->m_ChatBalloonObject.SetMessage(tempbuffer);
		}

		if(pChat_Window && pChat_Window->m_ChatListScrollBar)
		{
			//if(chatlistslot == pChat_Window->GetChatMode())
			{
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
				pChat_Window->m_ChatListScrollBar->SetTotalLineCount( pChat_Window->RecalcChattingListLength() );
#else
				pChat_Window->m_ChatListScrollBar->SetTotalLineCount(g_Chatlist.getitemcount());
#endif
				pChat_Window->m_ChatListScrollBar->UpdateData();
				
				// Ʈ���� Ʈ�����߿��� �� �Ʒ������� �ڵ���ũ�� ����
				if(!pChat_Window->m_ChatListScrollBar->Dragging())
				{
					pChat_Window->m_ChatListScrollBar->SetLineScrollPos(999999);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
////                                  �޽��� ���� �κ�                                    ////
//////////////////////////////////////////////////////////////////////////////////////////////

void _XNetwork::InitializeMessengerPacketBuffer( void )
{
	m_Messenger_nPI = 0;
	m_Messenger_nRI = 0;
	memset( m_Messenger_szPacket, 0, sizeof(BYTE) * _XDEF_PACKET_MAXBUFFER );
	m_Messenger_sPacketLen = 0;
	m_Messenger_mode = _X_PACKET_RECIEVE_PACKET_LENGTH;
	_XLog("Initialize messanger packet buffer");
}


 BOOL _XNetwork::InitializeMessengerSocket()
{
	 
#ifdef _XDWDEBUG
	 if( g_SinglePlayMode || g_ZeroLevelMode )
	 {
		 return TRUE;
	 }
#endif
	 
	 if( !m_bConnected ) return FALSE; // �������� ������ �������� �޽��� ������ ������ ���� �ʴ´�.
	 
	 _XLog("Initialize messanger socket");
	 
	 ReleaseMessengerSocket();
	 InitializeMessengerPacketBuffer(); //<- ���� ���� ��Ŷ�� �޽��� ���� ��Ŷ �ΰ��� ���ư��Ƿ� �� �ٸ� ���۰� �ʿ��ϴ�.
	 
	 
	 _XLog("Create messanger socket");

	 m_hMessengerSocket = socket(PF_INET, SOCK_STREAM, 0);
	 
	 if (m_hMessengerSocket == INVALID_SOCKET) 
	 {
		 m_hMessengerSocket = NULL;
		 _XFatalError( "Can't create Messenger socket." );
		 return FALSE;
	 }
	 
	 struct sockaddr_in addr;
	 
	 addr.sin_family = AF_INET;
	 addr.sin_addr.s_addr = INADDR_ANY;
	 addr.sin_port = 0;
	 
	 _XDWINPRINT( "Connecting login server" );                     
	 
	 _XLog("AsyncSelect messanger socket");

	 if( WSAAsyncSelect(m_hMessengerSocket, gHWnd, WM_USER_MESSENGERSOCKET, FD_CONNECT) == SOCKET_ERROR )
	 {
		 int nError = WSAGetLastError();
		 _XFatalError( _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILSELECT), nError);
		 return FALSE;
	 }
	 
	 addr.sin_family = AF_INET;
	 addr.sin_port = htons(m_MESSENGERSERVER_PORT);
	 addr.sin_addr.s_addr = m_MESSENGERSERVER_ADDRESS; 
	 

	 _XLog("Connect messanger server");

	 if (connect(m_hMessengerSocket, (struct sockaddr*) &addr, sizeof(addr)) == SOCKET_ERROR) 
	 {
		 int nError = WSAGetLastError();
		 
		 if( nError != WSAEWOULDBLOCK )
		 {
			 _XFatalError( _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILCONNECT), nError);
			 closesocket( m_hMessengerSocket ) ;
			 return FALSE;
		 }
	 }
	 
	 _XLog("Connected messanger server");

	 _XDWINPRINT( "Connected Messenger server...[%d]", g_LocalSystemTime );
	 
	return TRUE;
}

BOOL _XNetwork::ReleaseMessengerSocket()
{
	
#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode )
	{
		return TRUE;
	}
#endif
	
	if( m_hMessengerSocket )
	{
		shutdown(m_hMessengerSocket, SD_BOTH);
		closesocket(m_hMessengerSocket);
		m_hMessengerSocket = NULL;
	}
		
	_XLog("Messenger socket released.");
	_XDWINPRINT( "Messenger socket released." );
	
	return TRUE;	
}

void _XNetwork::ProcessMessengerNetworkError( void )
{	
	if( !m_bMessengerServerConnected ) return;
	
	int ret = WSAGetLastError();

	_XUpdateWindow();
	
	_XLog( "CRITICAL ERROR : Destroyed Messenger Network kernel. [_ECode:%d]  Reset _XNetworkKernel_module", ret );
	_XDWINPRINT( "CRITICAL ERROR : Destroyed Messenger Network kernel. [_ECode:%d]  Reset _XNetworkKernel_module", ret );
		
	if( m_hMessengerSocket )
	{
		shutdown(m_hMessengerSocket, SD_BOTH);
		m_hMessengerSocket = NULL;
	}	
	m_bMessengerServerConnected = FALSE;	
	m_bMessengerServerLoginSuccess = FALSE;	//2004.06.19->oneway48 insert

	_XLog("End of process : messenger network error");
}

void _XNetwork::ReadMessengerPacket()
{ 
	if( !m_bMessengerServerConnected ) return;
	
#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode ) return;
#endif	

	VIEWPACKETLOG
		_XDWPACKETLOG( "{## recieve Messenger socket message" );
	
#define _XDEF_MESSENGERPACKETPARSING_
#ifdef  _XDEF_MESSENGERPACKETPARSING_
	int		 nRead;
	_sHeader head;
		
	while ((nRead  = recv(m_hMessengerSocket, (char*)m_Messenger_szPacket + m_Messenger_nRI, _XDEF_PACKET_MAXBUFFER - m_Messenger_nRI, 0)) > 0)
	{   
		m_Messenger_nRI += nRead;

		while( m_Messenger_nPI < m_Messenger_nRI ) 
		{
			if(m_Messenger_mode == _X_PACKET_RECIEVE_PACKET_LENGTH) 
			{
				if( m_Messenger_nPI < m_Messenger_nRI-1 ) 
				{
					memcpy( &head, (m_Messenger_szPacket+m_Messenger_nPI), sizeof(_sHeader) );
					
					if( head.sLength <= 0 )
					{
						m_Messenger_sPacketLen = 0;
						_XLog( "WARNING : Messenger Packet header invalid length [%d %d %d]", head.sLength, head.sCrypto, head.sCompressed );
#ifdef _XDWDEBUG
 						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Messenger Network error", " ", TRUE );
						g_MessageBox.SetMessage( "WARNING : Messenger packet header invalid length [%d %d %d]", head.sLength, head.sCrypto, head.sCompressed );
						g_MessageBox.ShowWindow( TRUE );
#endif
						break;
					}
					else
					{
						m_Messenger_sPacketLen = head.sLength;
						m_Messenger_mode = _X_PACKET_RECIEVE_PACKET_BODY;
					}
				}
				else 
				{
					break;
				}
			}
			else if (m_Messenger_mode == _X_PACKET_RECIEVE_PACKET_BODY) 
			{
				if (m_Messenger_nPI + m_Messenger_sPacketLen <= m_Messenger_nRI)
				{
					if( m_Messenger_szPacket[m_Messenger_nPI+2] >= _XDEF_SERVERMESSAGE_MAX )
					{
						break;
					}
					else
					{
						ParsingMessengerPacket(m_Messenger_szPacket + m_Messenger_nPI);
					}
					m_Messenger_nPI += m_Messenger_sPacketLen;
					m_Messenger_mode = _X_PACKET_RECIEVE_PACKET_LENGTH;					
				}
				else
				{
					break;
				}
			}
		}
		if (m_Messenger_nPI == m_Messenger_nRI) 
		{
			m_Messenger_nPI = m_Messenger_nRI = 0;
		}
		else if (m_Messenger_nPI > 0) 
		{
			if( m_Messenger_nRI - m_Messenger_nPI < 1 )
			{
				if( m_Messenger_nRI - m_Messenger_nPI < 0 )
				{
					InitializeMessengerPacketBuffer();
					_XLog( "WARNING : Invalid Incompletion Messenger packet size [RI : %d / PI : %d]", m_Messenger_nRI, m_Messenger_nPI );
				}
				else
				{
					_XLog( "WARNING : Invalid Incompletion Messenger packet size zero [RI : %d / PI : %d]", m_Messenger_nRI, m_Messenger_nPI );
				}
#ifdef _XDWDEBUG
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Messenger Network error", " ", TRUE );
				g_MessageBox.SetMessage( "WARNING : Invalid Incompletion Messenger packet size [RI : %d / PI : %d]", m_Messenger_nRI, m_Messenger_nPI );
				g_MessageBox.ShowWindow( TRUE );
#endif				
			}
			else
			{
				if( m_Messenger_nRI >= _XDEF_PACKET_MAXBUFFER )
				{
					InitializeMessengerPacketBuffer();
#ifdef _XDWDEBUG
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
					g_MessageBox.SetMessage( "WARNING : Overflow Incompletion Messenger packet size [%d]", m_Messenger_nRI );
					g_MessageBox.ShowWindow( TRUE );
#endif
					_XLog( "WARNING : Overflow Incompletion Messenger packet size [%d]", m_Messenger_nRI );
				}
				else
				{
					int leftsize = m_Messenger_nRI - m_Messenger_nPI;

					BYTE	g_szTempPacket[_XDEF_PACKET_MAXBUFFER];
					memset( g_szTempPacket, 0, _XDEF_PACKET_MAXBUFFER );
					memcpy( g_szTempPacket, m_Messenger_szPacket + m_Messenger_nPI, leftsize );
					memset( m_Messenger_szPacket, 0, _XDEF_PACKET_MAXBUFFER );
					memcpy( m_Messenger_szPacket, g_szTempPacket, leftsize );

					m_Messenger_nRI = leftsize;
					m_Messenger_nPI = 0;

					if( m_Messenger_nRI <= -1 )
					{
#ifdef _XDWDEBUG
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
						g_MessageBox.SetMessage( "WARNING : Invalid Messenger packet recieve point  [%d]", m_Messenger_nRI );
						g_MessageBox.ShowWindow( TRUE );
#endif
						_XLog( "WARNING : Invalid Messenger packet recieve point  [%d]", m_Messenger_nRI );
					}
				}
			}
		}
	}
#else
	int nRead = 0 ;
	char buf[255] ;
	nRead = recv(m_hMessengerSocket, buf, 255, 0) ;
	if( nRead == SOCKET_ERROR || nRead == 0 )
	{
		ProcessNetworkError();
		
		g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Messenger Network error", " ", TRUE );
		g_MessageBox.SetMessage( _XGETINTERFACETEXT(ID_STRING_NETWORK_FAILRECEIVEDATA), WSAGetLastError() );
		g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
		_XUpdateWindow();		
		
		return;
	}
	
	ParsingMessengerPacket( buf );
#endif
	
	VIEWPACKETLOG
		_XDWPACKETLOG( "  End of recieve Messenger message ##}" );	
}

void _XNetwork::ParsingMessengerPacket(LPBYTE pPacket)
{
	
#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode ) return;
#endif
	
#ifdef _XDWDEBUG
	_sHeader* header = (_sHeader*)pPacket;
	if( header )
	{
		VIEWPACKETLOG
			_XDWPACKETLOG( "Parsing recieved message Type : [%d]  [%d]:[%d]:[%d]", pPacket[2], header->sCompressed, header->sCrypto, header->sLength );
	}
#endif
	
	VIEWPACKETLOG
		_XDWPACKETLOG( "Parsing recieved Messenger message Type : [%d]", pPacket[2] );

	switch ( pPacket[2] )
	{
	case MMSG_NO_LOGIN_RESULT :		// Messenger Server �α��� ���� : ģ���� �α��� ����..(�α��� ������ �ڽſ��Ե�..)
		{
			MMSG_MSLOGIN_RESULT* messengerloginresult = (MMSG_MSLOGIN_RESULT*)pPacket;

			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : Messenger Server Log in : Result [%d]", messengerloginresult->cResult);

			m_bMessengerServerLoginSuccess = FALSE; //2004.06.19->oneway48 insert

			switch(messengerloginresult->cResult) {
			case 0:
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : Messenger Server Log in : Success [%d]", g_LocalSystemTime);
					
					if( g_bCommunityOptionMessengerAutoLogin )// �޽��� �ڵ� ������ �� ���� �� 
					{						
						// �α����̶�� ���¸� �˷��ش�.
						SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 1 );
					}
					else
					{
						// �α����̶�� ���¸� �˷��ش�.
						SendMessengerPacket( MMSG_NO_MY_STATE_UPDATE_C_S, 0 );	// ������ �ص� �ڽ��� ���¸� �α׿����� �˸���.
					}
					m_bMessengerServerLoginSuccess = TRUE;
					m_ConnectTryCount = 0;
					
					// �޽��� â ���÷��� �����ϱ�
					_XWindow_Messenger* pMessenger_Window = (_XWindow_Messenger*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSENGERWINDOW);
					if( pMessenger_Window )
					{
						_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
						if(pMainMenu_Window)
						{
							pMainMenu_Window->SetMessengerStatus( pMessenger_Window->m_CurrentUserStatus );
						}
						if( g_bCommunityOptionMessengerAutoLogin )// �޽��� �ڵ� ������ �� ���� �� 
						{
							pMessenger_Window->m_CurrentUserStatus = 1;	
							pMessenger_Window->m_btnMessengerLogIn->ShowWindow(FALSE);
							pMessenger_Window->m_btnMessengerLogOut->ShowWindow(TRUE);							
						}
						else
						{
							pMessenger_Window->m_CurrentUserStatus = 0;	
							pMessenger_Window->m_btnMessengerLogIn->ShowWindow(TRUE);
							pMessenger_Window->m_btnMessengerLogOut->ShowWindow(FALSE);
						}						
					}

					// �α��ο� �����ϸ� �޽��� ģ�� ����Ʈ�� ��û
					SendMessengerPacket( MMSG_NO_FRIEND_LIST_REQ_C_S);
					// �α��ο� �����ϸ� ������ ����Ʈ�� ��û
					SendMessengerPacket( MMSG_NO_MAIL_LIST_REQ_C_S);
				}
				break;
			case 1:
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECIEVE MESSAGE : Messenger Failed to login <Invalid Character Name>");
				}
				break;
			case 2:
				{
					VIEWPACKETLOG
						_XDWPACKETLOG("RECIEVE MESSAGE : Messenger Failed to login <Invalid IP Address>");
				}
				break;
			case 3:
				{
					VIEWPACKETLOG
					_XDWPACKETLOG("RECIEVE MESSAGE : Messenger Failed to login <Already Lon in>");
				}
				break;
			default:
				break;
			}

			// 0 : �α��� ����
			// 1 : �߸��� ĳ���� �̸�
			// 2 : ������ �ּҰ� �ٸ�
			// 3 : �̹� �α�����...	
			
		}		
		break;	
		
	case MMSG_NO_LOGOUT_REPORT_S_C:// ģ���� �α׿��� ����..
		break;

	case MMSG_NO_MAKE_FRIEND_REQ_C_S_C:// ģ�� �α� ������ �޾��� �� 
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE JOIN FRIEND MESSAGE [%d]", g_LocalSystemTime);

			MMSG_MAKE_FRIEND_REQUEST*joinfriend = (MMSG_MAKE_FRIEND_REQUEST*)pPacket;
			MMSGMessengerRequestFriend(joinfriend);			
		}
		
		break;
			
	case MMSG_NO_MAKE_FRIEND_RES_S_C:// ģ�� ��� ��û ���
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE MAKE FRIEND RESULT [%d]", g_LocalSystemTime);

			MMSG_MAKE_FRIEND_RES* responsemakefriend = (MMSG_MAKE_FRIEND_RES*)pPacket;
			MMSGMessengerResponseMakeFriend(responsemakefriend);

		}		
		break;
		
	case MMSG_NO_CUT_FRIEND_REPORT_S_C:// ģ�� ���� �˸�~
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : CUTTING OFF FRIEND [%d]", g_LocalSystemTime);

			MMSG_CUT_FRIEND_REPORT* resultcutfriend = (MMSG_CUT_FRIEND_REPORT*)pPacket;
			MMSGMessengerResultCutFriend(resultcutfriend);

		}
		break;
		
	case MMSG_NO_FRIEND_LIST_RES_S_C :		// Messenger GroupList �� FriendList �ޱ� : ģ�� ����Ʈ ���
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : MESSENGER LIST INFO [%d]", g_LocalSystemTime);

			MMSG_FRIEND_LIST_RES* messengermemberinfo = (MMSG_FRIEND_LIST_RES*)pPacket;
			MMSGMessengerMemberInfo( messengermemberinfo );
		}			
		break;
		
	case MMSG_NO_FRIEND_STATE_REPORT_S_C:// ģ���� ���� ���� ���� ����..
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : USER STATE UPDATE RESULT [%d]", g_LocalSystemTime);

			MMSG_FRIEND_STATE_REPORT* userstateupdate = (MMSG_FRIEND_STATE_REPORT*)pPacket;
			MMSGMessengerUpdateStatus( userstateupdate );

		}			
		break;

	case MMSG_NO_CHAT_GM:// 2004.05.20->oneway48 insert : ��� ä�� �޽���
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE CHATTING MESSAGE [%d]", g_LocalSystemTime);

			MMSG_CHAT* chattingmessage = (MMSG_CHAT*)pPacket;
			MMSGMessengerChattingMessage( chattingmessage , TRUE );
		}
		break; 
		
	case MMSG_NO_CHAT:// ä�� �޽���..
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE CHATTING MESSAGE [%d]", g_LocalSystemTime);

			MMSG_CHAT* chattingmessage = (MMSG_CHAT*)pPacket;
			MMSGMessengerChattingMessage( chattingmessage );
		}
		break;
	case MMSG_NO_CHAT_WHISPER :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE WHISPER CHATTING MESSAGE [%d]", g_LocalSystemTime);

			MMSG_CHAT* chattingmessage = (MMSG_CHAT*)pPacket;
			MMSGWhisperChatMessage( chattingmessage );
		}
		break;
		
	case MMSG_NO_MAKE_GROUP_RES_S_C:// �׷� ����� ����
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE MAKE GROUP [%d]", g_LocalSystemTime);

			MMSG_MAKE_GROUP_RES* responsemakegroup = (MMSG_MAKE_GROUP_RES*)pPacket;
			MMSGMessengerResponseMakeGroup( responsemakegroup );
		}
		break;
		
	case MMSG_NO_CUT_GROUP_RES_S_C:// �׷� ���� ����
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE DELETE GROUP [%d]", g_LocalSystemTime);

			MMSG_CUT_GROUP_REQ* responsedeletegroup = (MMSG_CUT_GROUP_REQ*)pPacket;
			MMSGMessengerResponseDeleteGroup( responsedeletegroup );
		}
		break;

	case MMSG_NO_JOIN_GROUP_RES_S_C:// �׷� �̵� ��� 
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE MOVE GROUP [%d]", g_LocalSystemTime);

			MMSG_JOIN_GROUP_RES* responsemovegroup = (MMSG_JOIN_GROUP_RES*)pPacket;
			MMSGMessengerResponseMoveGroup( responsemovegroup );

		}
		break;
		
	case MMSG_NO_RENAME_GROUP_RES_S_C:// �׷� �̸� �ٲٱ� ��� 
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE DELETE GROUP [%d]", g_LocalSystemTime);

			MMSG_RENAME_GROUP_RES* responsechagegroupname = (MMSG_RENAME_GROUP_RES*)pPacket;
			MMSGMessengerResponseChangeGroupName( responsechagegroupname );
			
		}
		break;

		//2004.06.22->oneway48 insert
	case MMSG_NO_CONNECT_PERM:
		{			
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE CONNECTED MESSENGERSERVER [%d]", g_LocalSystemTime);

			MMSG_CONNECT_PERM* connectedmessenger = (MMSG_CONNECT_PERM*)pPacket;
			SendMessengerPacket(MMSG_NO_LOGIN_C_S);
			
		}
		break;	
	case MMSG_CHAT_RESULT_S_C :
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE WHISPERCHATRESULT MESSAGE [%d]", g_LocalSystemTime);

			MMSG_CHAT_RESULT* whisperchatmsg = (MMSG_CHAT_RESULT*)pPacket;
			MMSGWhisperChatFailMessage( whisperchatmsg );
		}
		break;
		//insert end
		
		
	/////////////////////////////////////////////////////////////////////////////////////
	//                              ������ ���� ��Ŷ                                   //
	/////////////////////////////////////////////////////////////////////////////////////		
	case MMSG_NO_MAIL_LIST_RES_S_C:// ���� ����Ʈ�� ��ȯ�޴´�. 
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE DELETE GROUP [%d]", g_LocalSystemTime);

			MMSG_MAIL_LIST_RES* responsemaillist = (MMSG_MAIL_LIST_RES*)pPacket;
			MMSGEmailListResponse( responsemaillist );
			
		}
		break;

	case MMSG_NO_MAIL_TEXT_RES_S_C:// ���� ������ ��ȯ�޴´�. 
		{
			VIEWPACKETLOG			
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE DELETE GROUP [%d]", g_LocalSystemTime);
			MMSG_MAIL_TEXT_RES* responsemailtext = (MMSG_MAIL_TEXT_RES*)pPacket;
			MMSGEmailTextResponse( responsemailtext );
			
		}
		break;
		
	case MMSG_NO_MAIL_SEND_RES_S_C:// ���� �߼ۿ� ���� ���. 
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE DELETE GROUP [%d]", g_LocalSystemTime);

			MMSG_MAIL_SEND_RES* successsendmail = (MMSG_MAIL_SEND_RES*)pPacket;
			MMSGEmailTextSuccess( successsendmail );
			
		}
		break;
		
	case MMSG_NO_MAIL_RECV_S_C:// �α��� �߿� ������ �������� ���. 
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE DELETE GROUP [%d]", g_LocalSystemTime);

			MMSG_MAIL_RECV* receivemail = (MMSG_MAIL_RECV*)pPacket;
			MMSGEmailTextReceive( receivemail );
			
		}
		break;
		
	case MMSG_NO_MAIL_FAIL_S_C:// ���� ��û�� ���� ����. 
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("RECEIVE MESSENGER  MESSAGE : RECEIVE DELETE GROUP [%d]", g_LocalSystemTime);

			MMSG_MAIL_FAIL* failemail = (MMSG_MAIL_FAIL*)pPacket;
			MMSGEmailFail( failemail );

			// cCmd : MMSG_NO_MAIL_LIST_RES_S_C : cResult == NULL
			// cCmd : MMSG_NO_MAIL_TEXT_RES_S_C : cResult == NULL
			// cCmd : MMSG_NO_MAIL_SEND_RES_S_C : cResult == 0 : �ش� ĳ���� �̸� �������� ����
			//				     							 1 : ���� ���Ϲڽ��� ������ ���̻� ������
			//												 2 : ���� ���� �ź���
			//												 3 : ��Ÿ ����
		}
		break;
		
	default :
		break;
	}			
}

BOOL _XNetwork::SendMessengerPacket(int type, int param1, int param2)
{
	if( !m_bMessengerServerConnected ) return FALSE;
	
#ifdef _XDWDEBUG
	if( g_SinglePlayMode || g_ZeroLevelMode ) return TRUE;
#endif
	switch ( type )
	{
	case MMSG_NO_LOGIN_C_S : // �޽��� ���� �α��� ��û 
	case MMSG_NO_LOGOUT_C_S:// �α׿��� ��û
	case MMSG_NO_MAKE_FRIEND_REQ_C_S_C:// ģ�� �α� ����..
	case MMSG_NO_MAKE_FRIEND_CON_C_S:// ģ�� �α� ���� or �ź�  : ������ ģ�� ��� ��û
	case MMSG_NO_CUT_FRIEND_REQ_C_S:// ģ�� ���� ��û..
	case MMSG_NO_FRIEND_LIST_REQ_C_S : // �޽��� ģ�� ����Ʈ ��û
	case MMSG_NO_MY_STATE_UPDATE_C_S:// ���� ���� ���� ������Ʈ ��û
	case MMSG_NO_MAKE_GROUP_REQ_C_S:// �׷� ����� ��û
	case MMSG_NO_CUT_GROUP_REQ_C_S:// �׷� ���� ��û
	case MMSG_NO_JOIN_GROUP_REQ_C_S:// �׷� �̵�..
	case MMSG_NO_RENAME_GROUP_REQ_C_S:// �׷� �̸� �ٲٱ� ..
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Messenger =========================");

			if( !Send_MMSGMessenger(type, param1, param2) )
				return FALSE;
		}
		break;
	case MMSG_NO_MAIL_LIST_REQ_C_S:// ���� ����Ʈ�� ��û�Ѵ�.
	case MMSG_NO_MAIL_TEXT_REQ_C_S:// ���� ������ ��û�Ѵ�.
	case MMSG_NO_MAIL_SEND_REQ_C_S:// ������ �߼��Ѵ�.
	case MMSG_NO_MAIL_DROP_C_S:// ���� ����.
	case MMSG_NO_MAIL_PUBLIC_SEND_REQ_C_S:// ������ ���� �߼� ��û
		{
			VIEWPACKETLOG
				_XDWPACKETLOG("SEND PACKET : Email =========================");

			if( !Send_MMSGEmail(type, param1, param2) )
				return FALSE;
		}
		break;			
		
	default:
		break;
	}
	
	return TRUE;
}

void _XNetwork::MSGStateEnterTutorial(MSG_STATE_CHANGE_ENTER_TURIAL* entertutorial)
{
	switch(g_QuestScriptManager.m_VirtualMode) {
	case _XQUEST_VIRTUALMODE_ENTER_OTHERZONE:// �����������(�ٸ������ٸ���ġ) �ʷε�
		{
			g_pLocalUser->m_bVirtualMode = TRUE;
			g_ChannelTempZoneInfoIndex = g_QuestScriptManager.m_indexRealZone; // ��������� �鰡�� ���� ��� ������ ����Ѵ�.
			g_CurrentZoneInfoIndex = g_QuestScriptManager.m_indexVirtualZone;
			
			g_pLocalUser->m_NextRespawnPosition.x		= g_QuestScriptManager.m_nVirtualZoneStartPositionX;
			g_pLocalUser->m_NextRespawnPosition.y		= 0.0f;
			g_pLocalUser->m_NextRespawnPosition.z		= g_QuestScriptManager.m_nVirtualZoneStartPositionZ;
			g_pLocalUser->m_TargetPosition				= g_pLocalUser->m_NextRespawnPosition;
			
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer = TRUE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MovingGameServer = TRUE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LockMoveProcess = TRUE;	
			
			((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_loading);
		}
		break;
	case _XQUEST_VIRTUALMODE_ENTER_CURRENTZONE:// �����������(��������������ġ)
		{
			g_pLocalUser->m_bVirtualMode = TRUE;			
		}
		break;
	case _XQUEST_VIRTUALMODE_ENTER_CURRENTZONEOTHERPOSITION:// �����������(���������ٸ���ġ)
		{
			g_pLocalUser->m_bVirtualMode = TRUE;
			
			FLOAT x	= g_QuestScriptManager.m_nVirtualZoneStartPositionX;
			FLOAT z	= g_QuestScriptManager.m_nVirtualZoneStartPositionZ;
			
			g_pLocalUser->m_Position.x = x - g_LodTerrain.m_HalfWorldSize;
			g_pLocalUser->m_Position.z = z - g_LodTerrain.m_HalfWorldSize;
			
			_XOBBData* pOBBData = NULL;
			
			g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, pOBBData );
			g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
			g_pLocalUser->m_LeftFinalTargetLength = 0.0f;
			g_pLocalUser->m_PathNodeCount = 0;
			g_pLocalUser->m_CurrentNavigatePathNodeIndex = 0;			
			
			g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
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
		}
		break;
	}
	
}

void _XNetwork::MSGStateLeaveTutorial(MSG_STATE_CHANGE_LEAVE_TUTORIAL* leavetutorial)
{
	switch(g_QuestScriptManager.m_VirtualMode) {
	case _XQUEST_VIRTUALMODE_LEAVE_COMEBACKZONE:
		{
			g_CurrentZoneInfoIndex = g_QuestScriptManager.m_indexRealZone;
			
			g_pLocalUser->m_NextRespawnPosition.x		= g_QuestScriptManager.m_nRealZoneStartPositionX;
			g_pLocalUser->m_NextRespawnPosition.y		= 0.0f;
			g_pLocalUser->m_NextRespawnPosition.z		= g_QuestScriptManager.m_nRealZoneStartPositionZ;
			g_pLocalUser->m_TargetPosition				= g_pLocalUser->m_NextRespawnPosition;
			
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer = TRUE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MovingGameServer = TRUE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LockMoveProcess = TRUE;	
			
			((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_loading);
		}
		break;
	case _XQUEST_VIRTUALMODE_LEAVE_COMEBACKZONEOTHERPOSITION:
		{
			g_CurrentZoneInfoIndex = g_QuestScriptManager.m_indexRealZone;
			
			g_pLocalUser->m_NextRespawnPosition.x		= g_QuestScriptManager.m_nRealZoneStartPositionX;
			g_pLocalUser->m_NextRespawnPosition.y		= 0.0f;
			g_pLocalUser->m_NextRespawnPosition.z		= g_QuestScriptManager.m_nRealZoneStartPositionZ;
			g_pLocalUser->m_TargetPosition				= g_pLocalUser->m_NextRespawnPosition;
			
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer = TRUE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MovingGameServer = TRUE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LockMoveProcess = TRUE;	
			
			((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_loading);
		}
		break;
	case _XQUEST_VIRTUALMODE_LEAVE_CURRENTZONE:
		{
			g_pLocalUser->m_bVirtualMode = FALSE;
		}
		break;
	case _XQUEST_VIRTUALMODE_LEAVE_CURRENTZONEOTHERPOSITION:
		{
			g_pLocalUser->m_bVirtualMode = FALSE;
			FLOAT x	= g_QuestScriptManager.m_nRealZoneStartPositionX;
			FLOAT z	= g_QuestScriptManager.m_nRealZoneStartPositionZ;
			
			g_pLocalUser->m_Position.x = x - g_LodTerrain.m_HalfWorldSize;
			g_pLocalUser->m_Position.z = z - g_LodTerrain.m_HalfWorldSize;
			
			_XOBBData* pOBBData = NULL;
			
			g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, pOBBData );
			g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
			g_pLocalUser->m_LeftFinalTargetLength = 0.0f;
			g_pLocalUser->m_PathNodeCount = 0;
			g_pLocalUser->m_CurrentNavigatePathNodeIndex = 0;			
			
			g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
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
		}
		break;
	case _XQUEST_VIRTUALMODE_LEAVE_OTHERZONEOTHERPOSITION:
		{			
			// �̵��Ϸ��� ������ ���� �ִ� �����̸� Ŭ���̾�Ʈ������ �ʷε��� �ϰ� �̵���Ų��.
			// �ٸ� �����̸� �������� �˾Ƽ� �̵���Ų��.
			if( g_QuestScriptManager.m_indexRealZone == leavetutorial->cMapID )
			{
				g_CurrentZoneInfoIndex = g_QuestScriptManager.m_indexRealZone;
				g_pLocalUser->m_NextRespawnPosition.x		= leavetutorial->fX;
				g_pLocalUser->m_NextRespawnPosition.y		= 0.0f;
				g_pLocalUser->m_NextRespawnPosition.z		= leavetutorial->fZ;
				g_pLocalUser->m_TargetPosition				= g_pLocalUser->m_NextRespawnPosition;
				
				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer = TRUE;
				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MovingGameServer = TRUE;
				((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LockMoveProcess = TRUE;	
				
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_loading);

			}
			else
			{
				g_pLocalUser->m_bVirtualMode = FALSE;
			}
		}
		break;
	}
}

void _XNetwork::MSGTargetInfo(MSG_CHARAC_MORE_INFO* targetinfo)
{
	g_pLocalUser->m_bTargetInfoLock = FALSE;
	g_MainWindowManager.DeleteWindow(_XDEF_WTITLE_TARGETINFONMESSAGEWINDOW);
	
	_XWindow_TargetInfo* targetinfo_window = (_XWindow_TargetInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOWINDOW);
	if( targetinfo_window )
	{
		memset( targetinfo_window->m_SendName, 0, sizeof(TCHAR)*13 );
		targetinfo_window->m_SelectedTargetInfo.reset() ;

		targetinfo_window->m_TargetInfoMode = (TargetInfoMode)targetinfo->cObjectType;
		strcpy( targetinfo_window->m_SelectedTargetInfo.cTargetName, targetinfo->cCharacName ) ;
		targetinfo_window->m_SelectedTargetInfo.sInnerLevel = targetinfo->sInnerLevel ;
		if( g_pLocalUser->m_CharacterInfo.Get_innerlevel()+7 <= (DWORD)targetinfo_window->m_SelectedTargetInfo.sInnerLevel )
		{
			targetinfo_window->m_PtoPLevelDef = 1;
		}
		else if( g_pLocalUser->m_CharacterInfo.Get_innerlevel()+6 >= (DWORD)targetinfo_window->m_SelectedTargetInfo.sInnerLevel &&
			g_pLocalUser->m_CharacterInfo.Get_innerlevel()-6 <= (DWORD)targetinfo_window->m_SelectedTargetInfo.sInnerLevel )
		{
			targetinfo_window->m_PtoPLevelDef = 2;
		}
		else if( g_pLocalUser->m_CharacterInfo.Get_innerlevel()-7 >= (DWORD)targetinfo_window->m_SelectedTargetInfo.sInnerLevel &&
			g_pLocalUser->m_CharacterInfo.Get_innerlevel()-19 < (DWORD)targetinfo_window->m_SelectedTargetInfo.sInnerLevel )
		{
			targetinfo_window->m_PtoPLevelDef = 3;
		}
		else if( g_pLocalUser->m_CharacterInfo.Get_innerlevel()-19 >= (DWORD)targetinfo_window->m_SelectedTargetInfo.sInnerLevel )
		{
			targetinfo_window->m_PtoPLevelDef = 4;
		}
		else 
		{
			targetinfo_window->m_PtoPLevelDef = 1;
		}

		targetinfo_window->m_SelectedTargetInfo.cClass = targetinfo->cClass ;
		targetinfo_window->m_SelectedTargetInfo.cClassGrade = targetinfo->cClassGrade ;
		targetinfo_window->m_SelectedTargetInfo.cMoonpa = targetinfo->cMoonpa ;
		if( targetinfo->cObjectType == tm_simple_info && targetinfo->sConstitution )	// ���� ����
		{
			targetinfo_window->m_PtoPLevelDef = 5;			
		}

		if( targetinfo->cObjectType == tm_ability_info || targetinfo->cObjectType == tm_all_info )
		{
			targetinfo_window->m_SelectedTargetInfo.sStr = targetinfo->sStr ;
			targetinfo_window->m_SelectedTargetInfo.sZen = targetinfo->sZen ;
			targetinfo_window->m_SelectedTargetInfo.sIntelligence = targetinfo->sIntelligence ;
			targetinfo_window->m_SelectedTargetInfo.sConstitution = targetinfo->sConstitution ;
			targetinfo_window->m_SelectedTargetInfo.sDexterity = targetinfo->sDexterity ;
		}		

		if( targetinfo->cObjectType == tm_items_info || targetinfo->cObjectType == tm_all_info )
		{		
			for( int i = MSG_CHARAC_MORE_INFO::pos_amor; i < MSG_CHARAC_MORE_INFO::get_on_max ; ++i )
			{
				targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_cType( targetinfo->getOnItems[i].cType );
				targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_sID( targetinfo->getOnItems[i].sID );
				targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_cSecond( targetinfo->getOnItems[i].cSecond );
				targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_ucSocketCount( targetinfo->getOnItems[i].Item_Money.S_NormalItem.ucSocketCount );
				targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_usCurDur( targetinfo->getOnItems[i].Item_Money.S_NormalItem.usCurDur );
				targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_usMaxDur( _XGameItem::GetMaxDurability(targetinfo->getOnItems[i].cType, targetinfo->getOnItems[i].sID ) ) ;

				if( targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Get_m_ucSocketCount() > 0 )
				{
					for( int j = 0; j < 4; ++j)
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_sSocket( j, targetinfo->getOnItems[i].sSocket[j] );
				}
				targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_ucStrength( targetinfo->getOnItems[i].ucStrength );

#ifdef _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI
	#ifdef _XDEF_SMELT_ITEM //Author : �����
				targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_usCurProtect = targetinfo->getOnItems[i].uselimit.usCurProtect;
				targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_usCurMaxProtect = targetinfo->getOnItems[i].uselimit.usCurMaxProtect;
				targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_usUsedProtect = targetinfo->getOnItems[i].uselimit.usUsedProtect;
				targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_ucWeaponStrength( _XGameItem::GetNature( targetinfo->getOnItems[i].cType, targetinfo->getOnItems[i].sID ) );
	#endif

				targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucFlag1 = targetinfo->getOnItems[i].uselimit.ucFlag1;
				targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_usValue1	= targetinfo->getOnItems[i].uselimit.usValue1;

				if( targetinfo->getOnItems[i].uselimit.ucYear > 0 )
				{
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_ucYear( targetinfo->getOnItems[i].uselimit.ucYear );	
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_ucMonth(targetinfo->getOnItems[i].uselimit.ucMonth );	
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_ucDay(targetinfo->getOnItems[i].uselimit.ucDay);	
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_ucHour( targetinfo->getOnItems[i].uselimit.ucHour);	
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].Set_m_ucMinute( targetinfo->getOnItems[i].uselimit.ucMinute);
				}

#else
	#ifdef _XDEF_SMELT_ITEM //Author : ����� //breif : �����ؾ� �ϴ°�
				if( i == MSG_CHARAC_MORE_INFO::pos_weapon_1 )
				{
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_usCurProtect = targetinfo->weapons[0].usCurProtect;
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_usCurMaxProtect = targetinfo->weapons[0].usCurMaxProtect;
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_usUsedProtect = targetinfo->weapons[0].usUsedProtect;
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucWeaponStrength = _XGameItem::GetNature( targetinfo->getOnItems[i].cType, targetinfo->getOnItems[i].sID );

					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucFlag1 = targetinfo->weapons[0].ucFlag1;
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_usValue1	= targetinfo->weapons[0].usValue1;

					if( targetinfo->weapons[0].ucYear > 0 )
					{
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucYear		= targetinfo->weapons[0].ucYear;	
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucMonth		= targetinfo->weapons[0].ucMonth;	
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucDay		= targetinfo->weapons[0].ucDay;	
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucHour		= targetinfo->weapons[0].ucHour;	
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucMinute	= targetinfo->weapons[0].ucMinute;
					}
				}
				else if( i == MSG_CHARAC_MORE_INFO::pos_weapon_2 )
				{
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_usCurProtect = targetinfo->weapons[1].usCurProtect;
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_usCurMaxProtect = targetinfo->weapons[1].usCurMaxProtect;
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_usUsedProtect = targetinfo->weapons[1].usUsedProtect;
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucWeaponStrength = _XGameItem::GetNature( targetinfo->getOnItems[i].cType, targetinfo->getOnItems[i].sID );

					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucFlag1 = targetinfo->weapons[1].ucFlag1;
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_usValue1	= targetinfo->weapons[1].usValue1;

					if( targetinfo->weapons[1].ucYear > 0 )
					{
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucYear		= targetinfo->weapons[1].ucYear;	
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucMonth		= targetinfo->weapons[1].ucMonth;	
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucDay		= targetinfo->weapons[1].ucDay;	
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucHour		= targetinfo->weapons[1].ucHour;	
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucMinute	= targetinfo->weapons[1].ucMinute;
					}
				}
				else
				{
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucFlag1 = targetinfo->weapons[0].ucFlag1;
					targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_usValue1	= targetinfo->weapons[0].usValue1;

					if( targetinfo->weapons[0].ucYear > 0 )
					{
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucYear		= targetinfo->weapons[0].ucYear;	
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucMonth		= targetinfo->weapons[0].ucMonth;	
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucDay		= targetinfo->weapons[0].ucDay;	
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucHour		= targetinfo->weapons[0].ucHour;	
						targetinfo_window->m_SelectedTargetInfo.getOnItems[i].m_ucMinute	= targetinfo->weapons[0].ucMinute;
					}
				}
	#endif //_XDEF_SMELT_ITEM
#endif
			}
		}
		targetinfo_window->ShowWindow(TRUE) ;
	}	
}

void _XNetwork::MSGGMSResult(MSG_GM_RESULT* gmsetresult)
{
	switch( gmsetresult->ucRequest ) 
	{
	case en_gm_req_nextto_charac :
		{
			switch( gmsetresult->ucResult ) 
			{
			case en_gm_result_success				: _XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2346) );
				break;
			case en_gm_result_charac_not_connect	: _XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2347) );
				break;
			case en_gm_result_charac_other_zone		: _XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2348) );
				break;
			case en_gm_result_server_off			: _XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2349) );
				break;
			}
		}
		break;
	case en_gm_req_charac_move	 :
		{
			switch( gmsetresult->ucResult ) 
			{
			case en_gm_result_success				: _XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2350) );
				break;
			case en_gm_result_charac_not_connect	: _XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2351) );
				break;
			case en_gm_result_charac_other_zone		: _XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2352) );
				break;
			case en_gm_result_server_off			: _XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2353) );
				break;
			}
		}		
		break;
	case en_gm_req_npc_kill_sign :
		{
			switch( gmsetresult->ucResult ) 
			{
			case en_gm_result_success				:
				break;
			case en_gm_result_npc_not_found			:
				break;
			case en_gm_result_server_off			:
				break;
			}
		}
		break;	
	case en_gm_req_enter_warp    :
		{
			switch( gmsetresult->ucResult ) 
			{
			case en_gm_result_success				:
				break;
			case en_gm_result_charac_not_connect	:
				break;
			case en_gm_result_charac_other_zone		:
				break;
			case en_gm_result_server_off			:
				break;
			}
		}
		break;	
	case en_gm_req_charac_moveto_warp :
		{
			switch( gmsetresult->ucResult ) 
			{
			case en_gm_result_success				:
				break;
			case en_gm_result_charac_not_connect	:
				break;
			case en_gm_result_charac_other_zone		:
				break;
			case en_gm_result_server_off			:
				break;
			}
		}
		break;	
	}


}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Billing process
void _XNetwork::MSGBillingInfo(MSG_BILL_INFO* billinginfo)
{
	g_SendBillingRequestPacket = FALSE;
	
	memset( &g_LocalUserBillingInformations, 0, sizeof(_XBillingInfo) );

	
	// ü�� PC���ϰ�� (�ӽ� ó��)
	/*
	if( strncmp( billinginfo->BillData.cBillingStatus, _T("PC"), 2 ) == 0 )
	{
		CEmperorOfDragonsApp* pApp = ((CEmperorOfDragonsApp*)gApp);		
		pApp->m_proc_maingame.m_ExperiencePCRoomConnect = TRUE;
	}
	*/

	// Who
	if( billinginfo->BillData.cWHO == 'H' )
	{
		g_LocalUserBillingInformations.BillingStatusWho = _XB_BillingStatusWho_Home;
	}
	else if( billinginfo->BillData.cWHO == 'P' )
	{
		g_LocalUserBillingInformations.BillingStatusWho = _XB_BillingStatusWho_PC;
	}
	else if( billinginfo->BillData.cWHO == 'S' )
	{
		g_LocalUserBillingInformations.BillingStatusWho = _XB_BillingStatusWho_ISP;
	}
	else if( billinginfo->BillData.cWHO == 0 )
	{
		g_LocalUserBillingInformations.BillingStatusWho = _XB_BillingStatusWho_Nothing;
	}
	
	// Where
	if( billinginfo->BillData.cWhere == 'H' )
	{
		g_LocalUserBillingInformations.BillingStatusWhere = _XB_BillingStatusWhere_Home;
	}
	else if( billinginfo->BillData.cWhere == 'P' )
	{
		g_LocalUserBillingInformations.BillingStatusWhere = _XB_BillingStatusWhere_PCRoom;
	}
	else if( billinginfo->BillData.cWhere == 'S' )
	{
		g_LocalUserBillingInformations.BillingStatusWhere = _XB_BillingStatusWhere_ISP;
	}

	// ETC Data
	g_LocalUserBillingInformations.BillingETCData = billinginfo->BillData.cETC;
	
	// Payment type	
	if( strncmp( billinginfo->BillData.cBillPayType, _T("DO"), 2 ) == 0 )
	{
		g_LocalUserBillingInformations.BillPayType = _XB_BillingPaymentType_DO;
	}
	else if( strncmp( billinginfo->BillData.cBillPayType, _T("FO"), 2 ) == 0 )
	{
		g_LocalUserBillingInformations.BillPayType = _XB_BillingPaymentType_FO;
	}	
	
	// Payment methods
	if( billinginfo->BillData.cBillMethod[0] == _T('N') )
	{
		g_LocalUserBillingInformations.BillMethod = _XB_BillingMethod_Free;
	}
	else if( billinginfo->BillData.cBillMethod[0] == _T('F') )
	{
		g_LocalUserBillingInformations.BillMethod = _XB_BillingMethod_Basic;
	}
	else if( billinginfo->BillData.cBillMethod[0] == _T('P') )
	{
		g_LocalUserBillingInformations.BillMethod = _XB_BillingMethod_Premium;
	}
	else if( billinginfo->BillData.cBillMethod[0] == _T('T') ) // ������
	{
		g_LocalUserBillingInformations.BillMethod = _XB_BillingMethod_Fixed;
	}
		
	g_LocalUserBillingInformations.BillRemain = billinginfo->BillData.iBillRemain * 1000;
	g_BillingRemainTimeCounter = g_LocalSystemTime;
	
	TCHAR tempbuffer[64];
	memset( tempbuffer, 0, sizeof(TCHAR) * 64 );
	
	strncpy( tempbuffer, billinginfo->BillData.cBillExpire, 4 );
	g_LocalUserBillingInformations.BillExpireYear = atoi( tempbuffer );
	
	memset( tempbuffer, 0, sizeof(TCHAR) * 64 );
	strncpy( tempbuffer, billinginfo->BillData.cBillExpire + 4, 2 );
	g_LocalUserBillingInformations.BillExpireMonth = atoi( tempbuffer );
	
	memset( tempbuffer, 0, sizeof(TCHAR) * 64 );
	strncpy( tempbuffer, billinginfo->BillData.cBillExpire + 6, 2 );
	g_LocalUserBillingInformations.BillExpireDay = atoi( tempbuffer );


	g_LocalUserBillingInformations.AlertCode = billinginfo->BillData.iAlertCode;


	strncpy( tempbuffer, billinginfo->BillData.cPCRoomEndDate + 2, 4 );
	g_LocalUserBillingInformations.PCRoomBillExpireYear = atoi( tempbuffer );
	
	memset( tempbuffer, 0, sizeof(TCHAR) * 64 );
	strncpy( tempbuffer, billinginfo->BillData.cPCRoomEndDate + 6, 2 );
	g_LocalUserBillingInformations.PCRoomBillExpireMonth = atoi( tempbuffer );

	memset( tempbuffer, 0, sizeof(TCHAR) * 64 );
	strncpy( tempbuffer, billinginfo->BillData.cPCRoomEndDate + 8, 2 );
	g_LocalUserBillingInformations.PCRoomBillExpireDay = atoi( tempbuffer );
	
	CEmperorOfDragonsApp* pApp = ((CEmperorOfDragonsApp*)gApp);
	_XProcess* pCurrentProcess = pApp->m_ProcessManager.GetCurrentProcess();

	
	switch( billinginfo->BillData.iAlertCode )  
	{
	case _PACKET_ALERT_LOGIN_INFO		:	// ���� �α��ν� �Ǵ� �⺻ ������ ����
		break;
	case _PACKET_ALERT_REMAIN_TIME		:	// ������� ���� �ð�
		{
			TCHAR szBuffer[256];
			memset(szBuffer, 0, sizeof(szBuffer));

			int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
			int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;

			if( remainhour > 0 )
			{
				strncpy( szBuffer,  g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORK_2354, &remainhour, &remainminute ), sizeof(szBuffer) );
			}
			else if( remainminute > 0 )
			{
				_snprintf( szBuffer, sizeof(szBuffer), _XGETINTERFACETEXT(ID_STRING_NETWORK_2355), remainminute );
			}
			else
			{
				_snprintf( szBuffer, sizeof(szBuffer), _XGETINTERFACETEXT(ID_STRING_NETWORK_2356), g_LocalUserBillingInformations.BillRemain / 1000 );
			}

			g_NetworkKernel.InsertChatString( szBuffer, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );

			//_XInsertSystemMessage( 0, szBuffer );
		}
		break;
	case _PACKET_ALERT_NOT_ENOUGH_POINT :	// �����ð� ����(��������)
		{
			if( pCurrentProcess == &(pApp->m_proc_maingame) )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NETWORK_2357), TRUE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE );				
				g_MessageBox.ShowWindow(TRUE);
				g_MessageBox.StartTimer( 5000, TRUE );
				_XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2357) );
			}
			else if( pCurrentProcess == &(pApp->m_proc_selectcharacter) )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NETWORK_2357), TRUE, _XDEF_CS_CANCELBUTTON, _XDEF_CS_CANCELBUTTON, _XDEF_CS_CANCELBUTTON );
				g_MessageBox.ShowWindow(TRUE);
				g_MessageBox.StartTimer( 5000, TRUE );
				_XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2357) );
			}
		}
		break;
	case _PACKET_ALERT_REBATE			:	// ȯ����ġ �Ǿ���(��������)
		if( pCurrentProcess == &(pApp->m_proc_maingame) )
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NETWORK_2358), TRUE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE );
			g_MessageBox.ShowWindow(TRUE);
			g_MessageBox.StartTimer( 5000, TRUE );			
			_XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2358) );
		}
		else if( pCurrentProcess == &(pApp->m_proc_selectcharacter) )
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NETWORK_2358), TRUE, _XDEF_CS_CANCELBUTTON, _XDEF_CS_CANCELBUTTON, _XDEF_CS_CANCELBUTTON );
			g_MessageBox.ShowWindow(TRUE);
			g_MessageBox.StartTimer( 5000, TRUE );			
			_XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2358) );
		}
		break;
	case _PACKET_ALERT_DOUBLE_LOGIN		:	// �ߺ� �α���(��������)
		if( pCurrentProcess == &(pApp->m_proc_maingame) )
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NETWORK_2359), TRUE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE );
			g_MessageBox.ShowWindow(TRUE);
			g_MessageBox.StartTimer( 5000, TRUE );			
			_XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2359) );
		}
		else if( pCurrentProcess == &(pApp->m_proc_selectcharacter) )
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NETWORK_2359), TRUE, _XDEF_CS_CANCELBUTTON, _XDEF_CS_CANCELBUTTON, _XDEF_CS_CANCELBUTTON );
			g_MessageBox.ShowWindow(TRUE);
			g_MessageBox.StartTimer( 5000, TRUE );			
			_XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2359) );
		}
		break;
	case _PACKET_ALERT_RENEW			:	// ����Ǿ� �ִ� ����ǰ���� ��ü
		if( pCurrentProcess == &(pApp->m_proc_maingame) )
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NETWORK_2360), TRUE );
			g_MessageBox.ShowWindow(TRUE);
			g_MessageBox.StartTimer( 5000, TRUE );			
			_XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2360) );
		}
		else if( pCurrentProcess == &(pApp->m_proc_selectcharacter) )
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NETWORK_2360), TRUE );
			g_MessageBox.ShowWindow(TRUE);
			g_MessageBox.StartTimer( 5000, TRUE );			
			_XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2360) );
		}
		break;
	case _PACKET_ALERT_ETC_ERROR_KILL	:	// ��Ÿ ����(���� ����)
		if( pCurrentProcess == &(pApp->m_proc_maingame) )
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NETWORK_2361), TRUE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE );
			g_MessageBox.ShowWindow(TRUE);
			g_MessageBox.StartTimer( 5000, TRUE );			
			_XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2361) );
		}
		else if( pCurrentProcess == &(pApp->m_proc_selectcharacter) )
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NETWORK_2361), TRUE, _XDEF_CS_CANCELBUTTON, _XDEF_CS_CANCELBUTTON, _XDEF_CS_CANCELBUTTON );
			g_MessageBox.ShowWindow(TRUE);
			g_MessageBox.StartTimer( 5000, TRUE );
			_XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2361) );
		}
		break;
	default:
		break;
	}

	pApp->m_proc_maingame.m_ExperiencePCRoomConnect = FALSE;
	
	if( g_LocalUserBillingInformations.BillingStatusWho == _XB_BillingStatusWho_Home )
	{
		// ���ι���,�����̾� ����
		if( g_LocalUserBillingInformations.BillingStatusWhere == _XB_BillingStatusWhere_Home )
		{
			if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Free )
			{
				// �����̿�/��				
			}
			else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Basic )
			{
				// ��������/��				
			}
			else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Premium )
			{
				// ���������̾�/��				
			}
			else
			{
				// Error
			}
		}
		else if( g_LocalUserBillingInformations.BillingStatusWhere == _XB_BillingStatusWhere_PCRoom )
		{
			// ������ �̻��� ������.
			if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Basic )
			{
				if( g_LocalUserBillingInformations.BillingETCData == 0  )
				{
					// ��������/ü���Ǿ���
					pApp->m_proc_maingame.m_ExperiencePCRoomConnect = TRUE;
				}
				else
				{
					// ��������/�����̾��Ǿ���
				}
			}
			else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Premium )
			{
				if( g_LocalUserBillingInformations.BillingETCData == 0  )
				{
					// �����̾�����/ü���Ǿ���
					pApp->m_proc_maingame.m_ExperiencePCRoomConnect = TRUE;
				}
				else
				{
					// �����̾�����/�����̾��Ǿ���
				}
			}
			else
			{
				// Error
			}					
		}
		else
		{
			// Error
		}
	}
	else if( g_LocalUserBillingInformations.BillingStatusWho == _XB_BillingStatusWho_PC )
	{
		if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Basic )
		{
			// �����̿�/�������Ǿ���
		}
		else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Fixed )
		{
			// �����̿�/�������Ǿ���
		}
		else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Premium )
		{
			// �����̿�/�����̾��Ǿ���
		}
		else 
		{
			// �����̿�/ü���Ǿ���
			pApp->m_proc_maingame.m_ExperiencePCRoomConnect = TRUE;
		}
	}
	else //	�ð��� ��� �Һ��� �����̿� ����.
	{		
		
	}

	_XWindow_PaymentWindow* pPaymentMethod_Window = (_XWindow_PaymentWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PAYMENTMETHODWINDOW);
	if(pPaymentMethod_Window)
		pPaymentMethod_Window->ResetTitleBar();
}

void _XNetwork::MSGVietnamBillingInfo(MSG_BILLING_INFO_ACK* billinginfoack)
{
	if( billinginfoack->ucType == 2 )
	{
		g_LocalUserBillingInformations.BillMethod = billinginfoack->Expire.cBillMethod;

		if( g_LocalUserBillingInformations.BillMethod == _VNBILLING_BILLMETHOD_FREEOFCHARGE ||
			g_LocalUserBillingInformations.BillMethod == _VNBILLING_BILLMETHOD_FREEOFCHARGE_EVENT )
		{
			g_LocalUserBillingInformations.BillRemain = billinginfoack->Expire.dwRemainTime * 1000; // 1/1000�ʷ� ����

			TCHAR tempString[5];
			memset( tempString, 0, sizeof(TCHAR) * 5 );
			strncpy( tempString, billinginfoack->Expire.szExpireDate, 4);
			g_LocalUserBillingInformations.BillExpireYear = atoi( tempString );
			
			memset( tempString, 0, sizeof(TCHAR) * 5 );
			strncpy( tempString, billinginfoack->Expire.szExpireDate+4, 2);			
			g_LocalUserBillingInformations.BillExpireMonth = atoi( tempString );

			memset( tempString, 0, sizeof(TCHAR) * 5 );
			strncpy( tempString, billinginfoack->Expire.szExpireDate+6, 2);
			g_LocalUserBillingInformations.BillExpireDay = atoi( tempString );
			
			memset( tempString, 0, sizeof(TCHAR) * 5 );
			strncpy( tempString, billinginfoack->Expire.szExpireDate+8, 2);
			g_LocalUserBillingInformations.BillExpireTime = atoi( tempString );
			
			memset( tempString, 0, sizeof(TCHAR) * 5 );
			strncpy( tempString, billinginfoack->Expire.szExpireDate+10, 2);
			g_LocalUserBillingInformations.BillExpireMinute = atoi( tempString );
		}
		else if( g_LocalUserBillingInformations.BillMethod == _VNBILLING_BILLMETHOD_BASIC_FIXEDQUANTITYSYSTEM || 
			     g_LocalUserBillingInformations.BillMethod == _VNBILLING_BILLMETHOD_PREMIUM_FIXEDQUANTITYSYSTEM )
		{
			g_LocalUserBillingInformations.BillRemain = billinginfoack->Expire.dwRemainTime * 1000; // 1/1000�ʷ� ����
		}
		else
		{
			TCHAR tempString[5];
			memset( tempString, 0, sizeof(TCHAR) * 5 );
			strncpy( tempString, billinginfoack->Expire.szExpireDate, 4);
			g_LocalUserBillingInformations.BillExpireYear = atoi( tempString );
			
			memset( tempString, 0, sizeof(TCHAR) * 5 );
			strncpy( tempString, billinginfoack->Expire.szExpireDate+4, 2);			
			g_LocalUserBillingInformations.BillExpireMonth = atoi( tempString );

			memset( tempString, 0, sizeof(TCHAR) * 5 );
			strncpy( tempString, billinginfoack->Expire.szExpireDate+6, 2);
			g_LocalUserBillingInformations.BillExpireDay = atoi( tempString );
			
			memset( tempString, 0, sizeof(TCHAR) * 5 );
			strncpy( tempString, billinginfoack->Expire.szExpireDate+8, 2);
			g_LocalUserBillingInformations.BillExpireTime = atoi( tempString );
			
			memset( tempString, 0, sizeof(TCHAR) * 5 );
			strncpy( tempString, billinginfoack->Expire.szExpireDate+10, 2);
			g_LocalUserBillingInformations.BillExpireMinute = atoi( tempString );
		}
	}
	else if( billinginfoack->ucType == 3 ) // countdown
	{
		g_LocalUserBillingInformations.AlertCode = _PACKET_ALERT_REMAIN_TIME;

		CEmperorOfDragonsApp* pApp = ((CEmperorOfDragonsApp*)gApp);
		_XProcess* pCurrentProcess = pApp->m_ProcessManager.GetCurrentProcess();

		TCHAR szBuffer[256];

		if( billinginfoack->ExpireCountDown.ucCount == 0 )
		{
			if( pCurrentProcess == &(pApp->m_proc_maingame) )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(" "), _XGETINTERFACETEXT(ID_STRING_NETWORK_2357), TRUE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE, _XDEF_GAMECLOSE );
				g_MessageBox.ShowWindow(TRUE);
				g_MessageBox.StartTimer( 5000, TRUE );
				_XInsertSystemMessage( 0, _XGETINTERFACETEXT(ID_STRING_NETWORK_2357) );
			}
		}
		else
		{
			int param1 = billinginfoack->ExpireCountDown.ucCount;
			int param2 = 0;

			strcpy( szBuffer,  g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORK_2354, &param1, &param2 ) );

			g_NetworkKernel.InsertChatString( szBuffer, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
		}
	}
}

void _XNetwork::MSGQuickSlot( MSG_QUICKSLOT_ALLINFO* quickslotinfo )
{
	_XWindow_QuickSlot* pQSlot_Window = (_XWindow_QuickSlot *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUICKSLOT);

	if( !pQSlot_Window ) return;


	for( int i = 0; i < _QUICKSLOT_COUNT_; i++ )
	{
		pQSlot_Window->m_QuickSlot[i].type = (_XQSlot_Type)quickslotinfo->m_QuickSlot[i].slotType;
		pQSlot_Window->m_QuickSlot[i].skillid = quickslotinfo->m_QuickSlot[i].skillID;
		pQSlot_Window->m_QuickSlot[i].lifeskillid = quickslotinfo->m_QuickSlot[i].lifeSkillID;
		pQSlot_Window->m_QuickSlot[i].itemCType = quickslotinfo->m_QuickSlot[i].itemCType;
		pQSlot_Window->m_QuickSlot[i].itemSID = quickslotinfo->m_QuickSlot[i].itemSID;
		pQSlot_Window->m_QuickSlot[i].invenNumber = quickslotinfo->m_QuickSlot[i].invenNumber;

		if( pQSlot_Window->m_QuickSlot[i].type == _XQS_SLOTTYPE_NONE )
		{
			pQSlot_Window->m_QuickSlot[i].used = false;
		}
		else
		{
			pQSlot_Window->m_QuickSlot[i].used = true;
		}

		pQSlot_Window->m_QuickSlot[i].bChangeItem = FALSE; //Author : ����� //breif : ������ ��ü �� ��� �ȵǰ�,,  _XQS_SLOTTYPE_SKILL �̰͸�
	}

	pQSlot_Window->CheckQuickSlotSkill();
	pQSlot_Window->CheckQuickSlotItem();
}

void _XNetwork::MSGHelpSystem( MSG_HELPLOG_ALLINFO* helpsysteminfo )
{
	if( g_TutorialInfo )
		g_TutorialInfo->ConvertToBit( (LPBOOL)helpsysteminfo->ucHelpLog );	
	
	// maingame initialize process���� �Űܿ�.
	if( g_TutorialInfo )
	{
		if( !g_TutorialInfo->_XTutorial_051 )
		{
			if( g_pLocalUser->m_CharacterInfo.Get_jinlevel() == 0 && g_pLocalUser->m_CharacterInfo.Get_innerlevel() == 0)
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_051);
				g_TutorialInfo->_XTutorial_051 = TRUE;

				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
			}			
		}
	}

	//_XLog( "Recieve help log" );
}

void _XNetwork::MSGStateChangeSafeRegionNotice(MSG_STATE_CHANGE_SAFEREGION_NOTICE* saferegionnotice)
{
	if(g_pLocalUser->m_Uniqueid == saferegionnotice->usCharacterUID)
	{
		g_pLocalUser->m_InsideSafetyZoneIndex = saferegionnotice->ucSafeRegionState;
	}
	else
	{
		_XUser* pUser = (_XUser*)g_Userlist.FindUser(saferegionnotice->usCharacterUID);
		if(pUser)
		{
			switch(saferegionnotice->ucSafeRegionState)
			{
			case 0 :
				{
					// �Ϲ�
					for(int i = 8042 ; i <= 8044 ; ++i)
					{
						if(!_XSkillItem::FindSkillProperty(i))
							break;

						short effectcount = g_SkillProperty[i]->detailInfo[0].effectCount;
						for(int ii = 0 ; ii < effectcount ; ++ii)
						{
							pUser->m_StateList.DeleteStateFromExtraSkill(pUser->m_Uniqueid, i, 0, ii, 3);
						}

						if(g_SkillProperty[i]->visualEffect > 0)
						{
							if(pUser->m_ModelDescriptor.m_EffectManager)
								pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[i]->visualEffect);
						}
					}
					
					_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
					if(pMonsterStatus_Window)
					{
						if(pMonsterStatus_Window->GetShowStatus())
						{
							if(pMonsterStatus_Window->m_TargetType == _XPICKINGTARGET_PC && pMonsterStatus_Window->m_Target == pUser)
							{
								pMonsterStatus_Window->SetStateImage();
							}
						}
					}
				}
				break;
			case 1 :
				{
					// ����
					if(!_XSkillItem::FindSkillProperty(8042))
						break;

					int skillid = pUser->m_StateList.FindStateExtraSkill(en_from_extraskill, 3);
					if(skillid == 8042)	// ���� �־���
						break;
					else
					{
						if(skillid > 0)
						{
							// ���� �ִ� �� ����
							short effectcount = g_SkillProperty[skillid]->detailInfo[0].effectCount;
							for(int i = 0 ; i < effectcount ; ++i)
							{
								pUser->m_StateList.DeleteStateFromExtraSkill(pUser->m_Uniqueid, skillid, 0, i, 3);
							}

							if(g_SkillProperty[skillid]->visualEffect > 0)
							{
								if(pUser->m_ModelDescriptor.m_EffectManager)
								{
									pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[skillid]->visualEffect);
								}
							}
						}

						// ���� ���� �߰�
						short effectcount = g_SkillProperty[8042]->detailInfo[0].effectCount;
						for(int i = 0 ; i < effectcount ; ++i)
						{
							pUser->m_StateList.InsertStateFromExtraSkill(pUser->m_Uniqueid, 8042, 0, 3, 0, i);
						}
						
						// effect �߰�
						if(g_SkillProperty[8042]->visualEffect > 0)
						{
							if(!pUser->m_ModelDescriptor.m_EffectManager->FindInstance(g_SkillProperty[8042]->visualEffect))
							{
								D3DXMATRIX offsetmat;
								D3DXMatrixIdentity(&offsetmat);
								pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									g_SkillProperty[8042]->visualEffect, &offsetmat, NULL, TRUE);
							}
						}

						_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
						if(pMonsterStatus_Window)
						{
							if(pMonsterStatus_Window->GetShowStatus())
							{
								if(pMonsterStatus_Window->m_TargetType == _XPICKINGTARGET_PC && pMonsterStatus_Window->m_Target == pUser)
								{
									pMonsterStatus_Window->SetStateImage();
								}
							}
						}
					}
				}
				break;
			case 2 :
				{
					// ���
					if(!_XSkillItem::FindSkillProperty(8043))
						break;

					int skillid = pUser->m_StateList.FindStateExtraSkill(en_from_extraskill, 3);
					if(skillid == 8043)	// ���� �־���
						break;
					else
					{
						if(skillid > 0)
						{
							// ���� �ִ� �� ����
							short effectcount = g_SkillProperty[skillid]->detailInfo[0].effectCount;
							for(int i = 0 ; i < effectcount ; ++i)
							{
								pUser->m_StateList.DeleteStateFromExtraSkill(pUser->m_Uniqueid, skillid, 0, i, 3);
							}

							if(g_SkillProperty[skillid]->visualEffect > 0)
							{
								if(pUser->m_ModelDescriptor.m_EffectManager)
								{
									pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[skillid]->visualEffect);
								}
							}
						}

						// ��� �߰�
						short effectcount = g_SkillProperty[8043]->detailInfo[0].effectCount;
						for(int i = 0 ; i < effectcount ; ++i)
						{
							pUser->m_StateList.InsertStateFromExtraSkill(pUser->m_Uniqueid, 8043, 0, 3, 0, i);
						}
						
						// effect �߰�
						if(g_SkillProperty[8043]->visualEffect > 0)
						{
							if(!pUser->m_ModelDescriptor.m_EffectManager->FindInstance(g_SkillProperty[8043]->visualEffect))
							{
								D3DXMATRIX offsetmat;
								D3DXMatrixIdentity(&offsetmat);
								pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									g_SkillProperty[8043]->visualEffect, &offsetmat, NULL, TRUE);
							}
						}

						_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
						if(pMonsterStatus_Window)
						{
							if(pMonsterStatus_Window->GetShowStatus())
							{
								if(pMonsterStatus_Window->m_TargetType == _XPICKINGTARGET_PC && pMonsterStatus_Window->m_Target == pUser)
								{
									pMonsterStatus_Window->SetStateImage();
								}
							}
						}
					}
				}
				break;
			case 3 :
				{
					// �߹�
					if(!_XSkillItem::FindSkillProperty(8044))
						break;

					int skillid = pUser->m_StateList.FindStateExtraSkill(en_from_extraskill, 3);
					if(skillid == 8044)	// ���� �־���
						break;
					else
					{
						if(skillid > 0)
						{
							// ���� �ִ� �� ����
							short effectcount = g_SkillProperty[skillid]->detailInfo[0].effectCount;
							for(int i = 0 ; i < effectcount ; ++i)
							{
								pUser->m_StateList.DeleteStateFromExtraSkill(pUser->m_Uniqueid, skillid, 0, i, 3);
							}

							if(g_SkillProperty[skillid]->visualEffect > 0)
							{
								if(pUser->m_ModelDescriptor.m_EffectManager)
								{
									pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(g_SkillProperty[skillid]->visualEffect);
								}
							}
						}

						// �߹� �߰�
						short effectcount = g_SkillProperty[8044]->detailInfo[0].effectCount;
						for(int i = 0 ; i < effectcount ; ++i)
						{
							pUser->m_StateList.InsertStateFromExtraSkill(pUser->m_Uniqueid, 8044, 0, 3, 0, i);
						}
						
						// effect �߰�
						if(g_SkillProperty[8044]->visualEffect > 0)
						{
							if(!pUser->m_ModelDescriptor.m_EffectManager->FindInstance(g_SkillProperty[8044]->visualEffect))
							{
								D3DXMATRIX offsetmat;
								D3DXMatrixIdentity(&offsetmat);
								pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									g_SkillProperty[8044]->visualEffect, &offsetmat, NULL, TRUE);
							}
						}

						_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
						if(pMonsterStatus_Window)
						{
							if(pMonsterStatus_Window->GetShowStatus())
							{
								if(pMonsterStatus_Window->m_TargetType == _XPICKINGTARGET_PC && pMonsterStatus_Window->m_Target == pUser)
								{
									pMonsterStatus_Window->SetStateImage();
								}
							}
						}
					}
				}
				break;
			}
		}
	}
}

//author : �����
void _XNetwork::PcRoomStateResult( const MSG_COMMON_NTF& CommonNTF )
{
	if( !CommonNTF.wMode )
	{
		switch( CommonNTF.NXPCB_Info.info.ucAddrDesc )
		{
		case 0 :
			g_pLocalUser->SetPCroomState( _XDEF_PCROOM_ERROR, 1 );
			_XLog( "PCROOM_ERROR" );
			break;
		case 'F' :
			g_pLocalUser->SetPCroomState( _XDEF_PCROOM_PREMIUM, 1 );
			_XLog( "PCROOM_PREMIUM" );
			break;
		case 'R' :
		case 'M' :
			g_pLocalUser->SetPCroomState( _XDEF_PCROOM_PREPAID, 1 );
			_XLog( "PCROOM_PREPAID" );
			break;
		case 'P' :
		case 'I' :
			g_pLocalUser->SetPCroomState( _XDEF_PCROOM_INTERNET, 1 );
			_XLog( "PCROOM_INTERNET" );
			break;
		default :
			g_pLocalUser->SetPCroomState( _XDEF_PCROOM_NONE, 1 );
			_XLog( "PCROOM_NONE" );
			break;
		}

		g_pLocalUser->SetPCroomRemainData( (DWORD)CommonNTF.NXPCB_Info.info.iArgument );
	}
}
//Last Updated : 07/03/14
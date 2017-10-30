#pragma once

#define _TRACE_
//#define _CRITICAL_SECTION_DEBUG

#ifdef _DEBUG
	//#define _PD_COMPANY_VINA_		// ����� �� ���� �ӽ÷�
	#define _PD_COMPANY_NXINTS_	// ����� �� ���� �ӽ÷�
	//#define _CIS_20070326_LOG_
#endif // #ifdef _DEBUG


// IDC ����.
#ifdef _PD_COMPANY_NEXON_
#include "hdr_nexon.h"
#endif

// IDC �� �系 �׽�Ʈ ����(���� ��õ)
#ifdef _PD_COMPANY_NXINTS_
#include "hdr_nx_innertest.h"
#endif

// ���� ���� ���� ����
#ifdef _PD_OPEN_TEST_
#include "hdr_open_test.h"
#endif

// ���� ���� ���� �׽�Ʈ�� ���� ���� : ���� ûȫ
#ifdef _PD_OT_INTS_
#include "hdr_ot_inner.h"
#endif

#ifdef _PD_COMPANY_METEL_
#include "hdr_metel.h"
#endif

#ifdef _PD_COMPANY_ACCLAIM_
#include "hdr_acclaim.h"
#endif

#ifdef _PD_COMPANY_JAPAN_
#include "hdr_japan.h"
#endif


#ifdef _PD_COMPANY_VINA_
#include "hdr_vina.h"
#endif

#ifdef _PD_COMPANY_RUSSIA_
#include "hdr_russia.h"
#endif

#ifdef _GLOBAL_OBJECT_DECLARE_

int g_ForestNum = 0 ;

#else

extern int g_ForestNum ;

#endif




//#define WINVER			0x500
#define _WIN32_WINNT	0x500
 
#ifndef _EXCLUDE_WINDOWS
#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
#include <afxcoll.h>
//#include <Mmsystem.h>
#endif

#ifndef _EXCLUDE_WINSOCK
#include <winsock2.h>
#endif

#include <stdio.h>
#include <assert.h>
#include <atldbcli.h>
#include <Mmsystem.h>

#include "Global\Global_Lock.h"
#include "Global\Global_Define.h"
#include "Global\Global_Function.h"
#include "Global\Global_struct.h"
#include "GEvent/GEventStructure.h"
#include "Global\tcp_packet.h"
#include "Global\udp_packet.h"
#include "Global\Global_LogFile.h"
#include "Global\Global_ServerInfo.h"
#include "Global\Global_Log_Packet.h"
#include "Global\Trace.h"

#include "Network\Network.h"
#include "ole_db_module\db_command.h"
#include "Packet_Queue\Packet_Queue.h"
#include "NDExceptionReport\NDExceptionReport.h"

#include "./MyLibrary/FileLog.h"

#ifndef _ERROR_LOG_FILE_WRITE
#define  _ERROR_LOG_FILE_WRITE
extern _j_GlobalLog * g_pSystemLog ;	// Log message saver
#endif


#ifndef _SYSTEM_TRACE
#define _SYSTEM_TRACE
extern CMYTrace * g_pTrace ;
#endif

#ifndef _EXCEPTION_REPORT
#define _EXCEPTION_REPORT
extern NDExceptionReport*	g_pNDExceptionReport ;
#endif

#ifndef _GLOBAL_CS_OBJECT
#define _GLOBAL_CS_OBJECT
// Critical sections
extern CTraceAbleLock g_csDebufPrint ;
#define print_lock() g_csDebufPrint.tlock(__FILE__, __LINE__)
#define print_unlock() g_csDebufPrint.tunlock(__FILE__, __LINE__)

#ifndef _UNICODE
typedef std::string		tstring;
#else
typedef std::wstring	tstring;
#endif

#endif


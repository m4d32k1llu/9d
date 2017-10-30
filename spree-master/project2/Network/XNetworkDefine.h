// XNetworkDefine.h
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _XNETWORKDEFINE_H_
#define _XNETWORKDEFINE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <winsock2.h>

/////////////////////////////////////////////////////////////////////////////
// Server informations...
#define _XDEF_SERVER_PORT					5124
#define	_XDEF_UDP_SERVERPORT				48300
#define	_XDEF_UDP_RECIEVEPORT				5002
#define _XDEF_MESSENGERSERVER_PORT			7411
#define _XDEF_MESSENGERSERVER_ADDR			"211.115.88.143"

#define _XDEF_LOGINSERVER_ADDR_IDC			"211.115.88.140"		// IDC
#define _XDEF_LOGINSERVER_ADDR_HS			"211.115.88.140"		

#ifdef _XDWDEBUG
#define _XDEF_LOGINSERVER_ADDR				_XDEF_LOGINSERVER_ADDR_HS
#else 
#define _XDEF_LOGINSERVER_ADDR				_XDEF_LOGINSERVER_ADDR_IDC
#endif

/////////////////////////////////////////////////////////////////////////////
// Utility macro...
#define _XUSER_LOCK_						EnterCriticalSection(&g_csUser);
#define _XUSER_UNLOCK_						LeaveCriticalSection(&g_csUser);

/////////////////////////////////////////////////////////////////////////////
// Chat macro & define...

#define _XDEF_CHATMESSAGEMODE_COUNT			5
#define _XDEF_CHATMESSAGEMODE_NORMAL		0
#define _XDEF_CHATMESSAGEMODE_MEMO			1

#define _XDEF_CHATMESSAGEMODE_WHISPER		32
#define _XDEF_CHATMESSAGEMODE_GLOBALNOTICE	64
#define _XDEF_CHATMESSAGEMODE_ADMINNOTICE	127

#define _XDEF_CHATMESSAGECOLOR_NORMAL			0	// �Ϲ� ä�� ��� 
#define _XDEF_CHATMESSAGECOLOR_MYMESSAGE		1	// �ڽ��� ä�� ��� 
#define _XDEF_CHATMESSAGECOLOR_WHISPER			2	// �ӼӸ� ä�� ���
#define _XDEF_CHATMESSAGECOLOR_WHOLE			3	// ��ü ä�� ���

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	#define _XDEF_CHATMESSAGECOLOR_USERCLAN			4	// ������ä�ø��
#else
	#define _XDEF_CHATMESSAGECOLOR_USERCLAN			4	// ��ȸ&������ ä�� ���
#endif
#define _XDEF_CHATMESSAGECOLOR_PARTY			5	// ��Ƽ ä�� ���
#define _XDEF_CHATMESSAGECOLOR_GLOBALNOTICE		6
#define _XDEF_CHATMESSAGECOLOR_ADMINNOTICE		7
#define _XDEF_CHATMESSAGECOLOR_ITEM				8
//#define _XDEF_CHATMESSAGECOLOR_QUEST			9	// ������� ����
#define _XDEF_CHATMESSAGECOLOR_ZONECHAT	 		9	// ���� ��ȭ ä�� ��� 

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	#define _XDEF_CHATMESSAGECOLOR_ALLY				10	// ��ȸ ä�� ���
#else
	#define _XDEF_CHATMESSAGECOLOR_LOCALMESSAGE		10	// ������� ����
#endif

#define _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL	11	// �ý��� �޼��� �Ϲ�
#define _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS		12	// �ý��� �޼��� �ҽ�
#define _XDEF_CHATMESSAGECOLOR_SYSTEMGET		13	// �ý��� �޼��� ����
#define _XDEF_CHATMESSAGECOLOR_GMBLOCKSTATUS	14

#ifdef _XDEF_CASHITEM_SHOUT
#define _XDEF_CHATMESSAGECOLOR_SHOUT	 		15	// ��ġ��
#else
#define _XDEF_CHATMESSAGECOLOR_INDEX15	 		15
#endif
//#ifdef _XDEF_ZONECHATTING_20070108
//#define _XDEF_CHATMESSAGECOLOR_ZONECHAT	 		16	// ���� ��ȭ 
//#endif

/////////////////////////////////////////////////////////////////////////////
// Network handling...

// Packet...
#define _XDEF_PACKET_MAXBUFFER				65536

#define _X_PACKET_RECIEVE_PACKET_LENGTH		0
#define _X_PACKET_RECIEVE_PACKET_BODY		1

#define _XDEF_SERVERMESSAGE_MAX				255

#endif // !defined(_XNETWORK_H_)

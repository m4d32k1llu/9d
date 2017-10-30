/********************************************************************************************************
 * TcpService.h : TcpService�� �ǽð� �����ùķ��̼� Game�� WorldWarII�� ��Ʈ���� ���� client����		*
					����̴�.																			*
 * Tcpthread�� receive�� ����Ͽ� Send�Լ��� ���ַ� �̷���� �ִ�.										*
 * by sukyun Kim																						*
 ********************************************************************************************************/

/********************************************************************************************************/
#if !defined(AFX_TCPSERVICE_H__8AB745B2_8FE0_11D4_9905_5254AB171A1A__INCLUDED_)
#define AFX_TCPSERVICE_H__8AB745B2_8FE0_11D4_9905_5254AB171A1A__INCLUDED_
#include <afxtempl.h>
#include <winsock.h>
#include <afxmt.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




#include "WW2Protocol.h"
#include "TcpResource.h"
class CWW2Client;
class CTcpThread;
class TcpService  
{
public:
	TcpService::TcpService();
	TcpService::~TcpService();

public:

	HWND GetServiceHandle();



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>***<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	
};

#endif // !defined(AFX_TCPSERVICE_H__8AB745B2_8FE0_11D4_9905_5254AB171A1A__INCLUDED_)

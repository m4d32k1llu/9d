//TcpInit.cpp
#include "stdafx.h"
#ifndef __TCPINIT_H__
#include "TcpInit.h"
#include "TcpService.h"

#include "RegStringDefine.h"

#endif
bool bLoop = true;
bool bSuccesPatch = false;
bool bSuccesBanner = false;
DWORD WINAPI DummyThreadFunc(LPVOID lparam);
TcpInit::TcpInit()
{
	hbannerFile = NULL;
	hpatchFile = NULL;
	m_dwtotlen =0;
	m_patchlen =0;
	m_ptcpService = new TcpService;
	Sendable = TRUE;
	BreakFlag = FALSE;
	bConnect = false;
	SendQueue.CurrentPos = 0; 
	SendQueue.DesPos = 0; 
	
	dummySock = NULL;

	for( int i=0; i<SendQueueSize; i++ )
		SendQueue.Packet[i] = NULL;
}
TcpInit::~TcpInit()
{
	delete m_ptcpService;
}
BOOL TcpInit::TcpDummyConnect(CString szip,long lport)
{
	HWND hService = m_ptcpService->GetServiceHandle();
	DWORD ThreadID;
	WSADATA wsadata;
	int err;
	SOCKADDR_IN sockAddr;									//���ͳ� ���� �ּ� ����ü
	ServerInfo tcpinfo;
	strcpy(tcpinfo.IP , szip.operator LPCTSTR());
	tcpinfo.port = lport;
	memset(&sockAddr,0,sizeof(sockAddr));

	WORD wRequestversion = MAKEWORD(2,2);
	WSAStartup(wRequestversion,&wsadata);

	
//	dummySock = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_IP,NULL,0,WSA_FLAG_OVERLAPPED);
	dummySock = socket(AF_INET,SOCK_STREAM,0);
	if(dummySock == INVALID_SOCKET)
	{
	//	closesocket(dummySock);
	//	WSACleanup();
		PostMessage(hService,WM_ERROR_NANGGEK,MODULE_TCPINIT,1);//
		CloseDummy();
		
		return FALSE;
	}
	
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons((u_short)tcpinfo.port);
	sockAddr.sin_addr.s_addr = inet_addr(tcpinfo.IP);

	if((hRecvEvent = WSACreateEvent()) == WSA_INVALID_EVENT)
	{
	//	AfxMessageBox("�߸��� �̺�Ʈ");
		PostMessage(hService,WM_ERROR_NANGGEK,MODULE_TCPINIT,2);//
		CloseDummy();
		return FALSE;
	}
	if(WSAEventSelect(dummySock,hRecvEvent,FD_CONNECT) != 0)
	{
	//	AfxMessageBox("�̺�Ʈ��ü ���� ����");
		PostMessage(hService,WM_ERROR_NANGGEK,MODULE_TCPINIT,3);//
		CloseDummy();
		return FALSE;
	}


	err = connect(dummySock,(LPSOCKADDR)&sockAddr,sizeof(sockAddr));
	if(err == SOCKET_ERROR)
	{
		int Error = WSAGetLastError();
		if(Error != WSAEWOULDBLOCK)
		{
			CloseDummy();
			return FALSE;
		}
	}
	
	
	dummyThread = ::CreateThread(NULL,0,DummyThreadFunc,this,0,&ThreadID);
	return TRUE;	
}

DWORD WINAPI DummyThreadFunc(LPVOID lparam)
{
	
	TcpInit * pTcpInit = (TcpInit*)lparam;
	HWND hService = pTcpInit->m_ptcpService->GetServiceHandle();
//	CString strTemp;
	char buffer[MAXDATA];
	BYTE DNQueue[64656];
	DWORD QueuePosition = 0;
	memset(DNQueue,0,64656);

	DWORD WaitStatus;

	WSAEVENT hEvent = pTcpInit->hRecvEvent;
	SOCKET   Sock	= pTcpInit->dummySock;
	WSANETWORKEVENTS Netevent;
	while(bLoop)
	{
		WaitStatus = WSAWaitForMultipleEvents(1,&hEvent,FALSE,WSA_INFINITE,FALSE);
	//	Sleep(1);
		if(pTcpInit->Sendable == TRUE)
		{
			pTcpInit->ProcessQueue();
		}

		if(WaitStatus == WSA_WAIT_EVENT_0)
		{
			if(WSAEnumNetworkEvents(Sock,hEvent,&Netevent) != 0)
			{
				return 0;
			}
		
			if(Netevent.lNetworkEvents & FD_CONNECT)
			{
				if(Netevent.iErrorCode[FD_CONNECT_BIT] == 0)
				{
					pTcpInit->bConnect = true;

					WSAEventSelect(Sock, hEvent,FD_READ | FD_WRITE | FD_CLOSE);
					//AfxMessageBox("������ ����");
					PostMessage(hService,WM_ERROR_NANGGEK,MODULE_TCPINIT,4);					
				}
				else
				{
					return 0;
				}
			}

			if((Netevent.lNetworkEvents & FD_CLOSE) == FD_CLOSE)
			{
				//closesocket(Sock);
				pTcpInit->CloseDummy();
				//AfxMessageBox("���� ����");
				PostMessage(hService,WM_ERROR_NANGGEK,MODULE_TCPINIT,5);
				return 1;
			}

			if((Netevent.lNetworkEvents & FD_READ) == FD_READ)
			{
				
				int rec = recv(pTcpInit->dummySock,buffer,MAXDATA,0);
				if(rec > 0)
				{
					DWORD Size;

					memcpy(&DNQueue[QueuePosition],buffer,rec);	//ť�� �����͸� �־�д�.
					QueuePosition += rec;

					while(1)
					{
						memcpy(&Size,&DNQueue[0],4);	//�ѻ���� �˾ƺ��� ���� ���� 4����Ʈ�� Size�� �����صд�.

					
					//���� ��Ŷ ũ�⸸ŭ �о��ٸ�
						if(QueuePosition >= Size)
						{
							char* Message = NULL;
							int nReturnMessage;
							Message = (char*)malloc(Size);
							memcpy(Message,&DNQueue[0],Size);
						//���� �����͸� �Լ��� ����� ó�� 
							nReturnMessage = pTcpInit->ProcABATE(Message);	//ť�� �ִ� ������ ���δٴ� �ǹ̷� abate ^^

							QueuePosition -= Size;
							memcpy(&DNQueue[0], &DNQueue[Size],QueuePosition);
							free(Message);
						}
						else
						{
							break;
						}
					}
				}
				
			}

	
			if((Netevent.lNetworkEvents & FD_WRITE) == FD_WRITE)
			{
				pTcpInit->Sendable = TRUE;
				
			}
		
		}
	}
	return 1;

}

int TcpInit::ProcABATE(char *Msg)
{

	memcpy(&msgbase.size,Msg,4);
	
	memcpy(&msgbase.type,&Msg[4],2);

	switch(msgbase.type)
	{
	case dCMSG_SRVLIST:
		memcpy(&msgservlist,Msg,sizeof(msgservlist));
		ProcWhenServer();
		break;
	case dSMSG_INIT:
		memcpy(&msginit,Msg,sizeof(msginit));
		ProcDInit();
		break;
	case dSMSG_INFOBANNER:
		memcpy(&msginfobanner,Msg,sizeof(msginfobanner));
		ProcInfoBanner();
		break;
	case dSMSG_TRANSBANNER:
		memcpy(&msgtransbanner,Msg,sizeof(msgtransbanner));
		ProcTransBanner();
		break;
	case dSMSG_INFOPATCH:
		memcpy(&msginfopatch,Msg,sizeof(msginfopatch));		
		ProcInfoPatch();
		break;
	case dSMSG_TRANSPATCH:
		memcpy(&msgtranspatch,Msg,sizeof(msgtranspatch));
		ProcTransPatch();
		break;
	case dCMSG_DISDMS:
		CloseDummy();
		break;

	default:break;
		
	}
	return 0;
}


BOOL TcpInit::SendPacket(char *Packet, int MsgSize )
{
	BOOL SendSucceeded = false;
	int TotalSendSize = 0;
	int SendSize = 0;
	
	if( Sendable == TRUE )
	{
		//ť�� Ȯ���غ���. ������ ������. �� ������ ������ ���� ��������.
		//���� �� ������ Packet�� �� ť�� �ִ´�.
		BOOL Processed = FALSE;
		
		Processed = ProcessQueue();

		if( Processed == TRUE )
		{
			TotalSendSize = 0;

			if( Sendable == TRUE )
			{
				while( 1 )
				{
					SendSize = send( dummySock, Packet, MsgSize, 0 );

					if( SendSize == SOCKET_ERROR )
					{
						int Error = WSAGetLastError();

						if( Error == WSAEWOULDBLOCK )
						{
			//				printf( "ť�� �����͸� �ֽ��ϴ�.\n" );

							Sendable = FALSE;
							InsertQueue( Packet, MsgSize, TotalSendSize );
							break;
						}
						else
						{
							BreakFlag = true;
							break;
						}
					}
					else
					{
						TotalSendSize += SendSize;

						if( TotalSendSize >= MsgSize )
						{
							SendSucceeded = TRUE;
							break;
						}
					}
				}
			}
			else
			{
				InsertQueue( Packet, MsgSize, TotalSendSize );
			//	printf( "ť�� �ְڽ��ϴ�.\n" );
			}
		}
		else
		{
			InsertQueue( Packet, MsgSize, TotalSendSize );
		//	printf( "��Ŷ�� ������ ���߽��ϴ�. ť�� �ְڽ��ϴ�.\n" );
		}
	}
	else
	{
		InsertQueue( Packet, MsgSize, TotalSendSize );
	//	printf( "��Ŷ�� ������ ���߽��ϴ�. ť�� �ְڽ��ϴ�.\n" );
	}

	return SendSucceeded;

}

int TcpInit::InsertQueue(char *Packet, int MsgSize, int TotalSendSize)
{
	if( SendQueue.Packet[SendQueue.DesPos] != NULL )
	{
		free(SendQueue.Packet[SendQueue.DesPos]);
		SendQueue.Packet[SendQueue.DesPos] = NULL;
	}

	SendQueue.Packet[SendQueue.DesPos] = (char *)malloc( MsgSize - TotalSendSize );

	memcpy( SendQueue.Packet[SendQueue.DesPos], &Packet[TotalSendSize], MsgSize - TotalSendSize );

	SendQueue.PacketSize[SendQueue.DesPos] = MsgSize - TotalSendSize;
	SendQueue.DesPos++;

	if( SendQueue.DesPos >= 8192 )
		 SendQueue.DesPos = 0;

	return 1;

}

BOOL TcpInit::ProcessQueue()
{
	BOOL Processed = FALSE;

	while( 1 )
	{
		if( SendQueue.DesPos == SendQueue.CurrentPos )
		{
			Processed = TRUE;
			break;
		}

		if( SendQueue.DesPos != SendQueue.CurrentPos )
		{
			int TotalSendSize = 0;
			int SendSize;
			int CurrentPos = SendQueue.CurrentPos;

			while( 1 )
			{
				SendSize = send( dummySock, SendQueue.Packet[CurrentPos], SendQueue.PacketSize[CurrentPos], 0 );
				TotalSendSize += SendSize;

				if( SendSize == SOCKET_ERROR )
				{
					int Error = WSAGetLastError();

					if( Error == WSAEWOULDBLOCK )
					{
						Sendable = FALSE;
						break;
					}
					else
					{
						BreakFlag = TRUE;
					}
				}
				else
				{
					if( TotalSendSize == SendQueue.PacketSize[CurrentPos] )
					{
						SendQueue.CurrentPos++;

						if( SendQueue.CurrentPos >= 8192 )
							SendQueue.CurrentPos = 0;

						break;
					}
				}
			}
		}
	}

	return Processed;

}

void TcpInit::SendDummyInit(WORD wVer, BYTE cBanner)
{
	cmsginit.base.type = dCMSG_INIT;
	cmsginit.base.size = sizeof(cmsginit);
	cmsginit.version = wVer;
	cmsginit.banner = cBanner;

	char * Packet;
	Packet = (char *)malloc(cmsginit.base.size);
	memcpy(&Packet[0],&cmsginit,cmsginit.base.size);
	SendPacket(Packet,cmsginit.base.size);
		

	free(Packet);	

//	int imsi = send(dummySock,(LPCTSTR)&cmsginit,sizeof(cmsginit),0);
}
void TcpInit::ProcDInit()
{
	HWND hService = m_ptcpService->GetServiceHandle();
	switch(msginit.result)
	{
	case dINIT_SUCCESS: //patch,banner both ��Ȯ��
		PostMessage(hService,WM_CONNECTINIT,1,10);
		CloseDummy();
		break;
	case dINIT_NOTVERSION:
		PostMessage(hService,WM_CONNECTINIT,2,10);
		
		SendReadPatch();
		break;
	case dINIT_NOTBANNER:		
		PostMessage(hService,WM_CONNECTINIT,3,10);
		
		SendReadBanner();
		break;
	case dINIT_BOTH:		
		PostMessage(hService,WM_CONNECTINIT,4,10);
		SendReadPatch();
		SendReadBanner();


		break;
	default:
		break;
	}
}

void TcpInit::ProcInfoBanner()
{
	HWND hService = m_ptcpService->GetServiceHandle();

	PostMessage( hService, WM_FILESIZE, 30, (LPARAM)msginfobanner.FileSize );
	
	NANGGEK_RegWriteInt(ID_REGSTR_MASTERKEY,ID_REGSTR_DREAMKEY,ID_REGSTR_BANNERVERSION,msginfobanner.BannerVersion);
	hbannerFile = ::CreateFile("Nanggekbanner.wwe" ,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hbannerFile == INVALID_HANDLE_VALUE)
	{
		PostMessage(hService,WM_ERROR_NANGGEK,MODULE_TCPINIT,30);//
	//	AfxMessageBox("file ���� ����");
	}

}

void TcpInit::ProcTransBanner()
{
	DWORD len;

	HWND hService = m_ptcpService->GetServiceHandle();


	if(msgtransbanner.cur < msginfobanner.tot)
	{
		if(m_dwtotlen < msginfobanner.FileSize)
		{
			SetFilePointer(hbannerFile,0,NULL,FILE_END);
			::WriteFile(hbannerFile,msgtransbanner.data,msgtransbanner.f_size,&len,NULL);
			m_dwtotlen = m_dwtotlen + len;

			PostMessage( hService, WM_PROGRESS_SIZE, 30, (LPARAM)m_dwtotlen );
//			sprintf(Temp,"recv  : banner part Size: %d / banner cur part: %d / banner total: %d \n"
//				,msgtransbanner.size,msgtransfile.cur ,m_dwtotlen);
//			WriteConsole(Temp);
			if(m_dwtotlen == msginfobanner.FileSize)
			{
				
				BOOL bClose = CloseHandle(hbannerFile);
				hbannerFile = NULL;
				if(bClose)
				{
					bSuccesBanner = true;
				//	AfxMessageBox("banner close success");
					PostMessage(hService,WM_BANNER_RECEVE,0,0);
				}

			}

		}
	}
}

void TcpInit::ProcWhenServer()
{	
	NANGGEK_RegWriteInt(ID_REGSTR_MASTERKEY,ID_REGSTR_DREAMKEY,ID_REGSTR_SERVERPORTNUMBER,msgservlist.port);
	NANGGEK_RegWriteString(ID_REGSTR_MASTERKEY,ID_REGSTR_DREAMKEY,ID_REGSTR_SERVERIPADDRESS,msgservlist.ip);
}

void TcpInit::SendReadBanner()
{
	cmsgredbanner.base.size	= sizeof(cmsgredbanner);
	cmsgredbanner.base.type	= dCMSG_REDBANNER;

	char * Packet;
	Packet = (char *)malloc(cmsgredbanner.base.size);
	memcpy(&Packet[0],&cmsgredbanner,cmsgredbanner.base.size);
	SendPacket(Packet,cmsgredbanner.base.size);
//	send(clientSock,(LPCTSTR)&ccmsgredbanner,sizeof(ccmsgredbanner),0);

}

void TcpInit::SendReadPatch()
{
	cmsgredpatch.base.size = sizeof(cmsgredpatch);
	cmsgredpatch.base.type = dCMSG_REDPATCH;

	char * Packet;
	Packet = (char *)malloc(cmsgredpatch.base.size);
	memcpy(&Packet[0],&cmsgredpatch,cmsgredpatch.base.size);
	SendPacket(Packet,cmsgredpatch.base.size);
}

void TcpInit::ProcInfoPatch()
{
	HWND hService = m_ptcpService->GetServiceHandle();
	
	PostMessage( hService, WM_FILESIZE, 40, (LPARAM)msginfopatch.FileSize );
	
	NANGGEK_RegWriteInt(ID_REGSTR_MASTERKEY,ID_REGSTR_DREAMKEY,ID_REGSTR_PATCHVERSION,msginfopatch.PatchVersion);

	hpatchFile = ::CreateFile("Patch.zip" ,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hpatchFile == INVALID_HANDLE_VALUE)
	{
		PostMessage(hService,WM_ERROR_NANGGEK,MODULE_TCPINIT,40);
		// AfxMessageBox("file ���� ����");
	}
}

void TcpInit::ProcTransPatch()
{
	DWORD len;

	HWND hService = m_ptcpService->GetServiceHandle();


	if(msgtranspatch.cur < msginfopatch.tot)
	{
		if(m_patchlen < msginfopatch.FileSize)
		{
			SetFilePointer(hpatchFile,0,NULL,FILE_END);
			::WriteFile(hpatchFile,msgtranspatch.data,msgtranspatch.f_size,&len,NULL);
			m_patchlen = m_patchlen + len;

			PostMessage( hService, WM_PROGRESS_SIZE, 40, (LPARAM)m_patchlen );
//			sprintf(Temp,"recv  : banner part Size: %d / banner cur part: %d / banner total: %d \n"
//				,msgtransfile.size,msgtransfile.cur ,m_dwtotlen);
//			WriteConsole(Temp);
			if(m_patchlen == msginfopatch.FileSize)
			{
				
				BOOL bClose = CloseHandle(hpatchFile);
				hpatchFile = NULL;
				if(bClose)
				{
					bSuccesPatch = true;
					//AfxMessageBox("file close success");
					PostMessage(hService,WM_BANNER_RECEVE,0,1);//������ lparam���� 1�̸� ��ġ������ �ٹ�����
				}

			}

		}
	}
}

void TcpInit::CloseDummy()
{
	if( hbannerFile )
	{		
		CloseHandle(hbannerFile);
		hbannerFile = NULL;
	}

	if( hpatchFile )
	{		
		CloseHandle(hpatchFile);
		hpatchFile = NULL;
	}

	if( dummySock ) 
	{
		closesocket(dummySock);
		dummySock = NULL;
		WSACleanup();
	}	

	bLoop = false;

	for( int i=0; i<SendQueueSize; i++ )
	{
		if(SendQueue.Packet[i] != NULL)
		{
			free( SendQueue.Packet[i] );
			SendQueue.Packet[i] = NULL;
		}
	}

}

// XFileDownload.cpp: implementation of the _XFileDownload class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <fstream>
#include <afxsock.h> 
#include "xlauncher.h"
#include "XFileDownload.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////

// DownloadFile.cpp

bool _XFileDownload( LPCTSTR IP, LPCTSTR URL, const char* FileName )
{
    CString Request;
    int nRead, nContentRead=0, nContent=1;
    char szTmp[4097], *psztmp;
    bool bContent=false;
	std::ofstream ofs;
	
    Request = "GET "; 
    Request += URL;
    Request += " HTTP/1.1\r\n";
    Request += "Host: ";
    Request += IP;
    Request += "\r\n\r\n"; // 2���� CRLF �� message-header�� ���� �����Ѵ�. 
	
    WSADATA wsaData;
    CSocket sock; 
	
    WSAStartup( MAKEWORD( 2, 2 ), &wsaData );   
	
    if ( sock.Create() == 0 ) 
	{
		return false;
	}
    
	if ( sock.Connect( IP, 80 ) == 0 ) 
	{
		return false;
	}
    
	if ( sock.Send( Request, Request.GetLength() ) == SOCKET_ERROR )
	{
		return false;	
	}

    ofs.open( FileName, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary );
	
    do
    {
        nRead = sock.Receive( szTmp, 4096 );
        if ( nRead == 0 ) // �޽����� �� �޾�����
            break;
		
        if ( nRead == WSAEWOULDBLOCK ) // sub-network fail
            break;
		
        szTmp[nRead] = 0;
		
        if ( bContent ) // message-body �κ��� �ް����� ��
        {
            nContentRead += nRead;
            ofs.write( szTmp, nRead );
        }
		
        else // message-header �κ��� �ް� ���� ��
        {
            // Content-Length == message-body�� ����
            if ( (psztmp = strstr( szTmp, "Content-Length:" )) != NULL )
                nContent = atoi( psztmp+16 ); 
			
            if ( ( psztmp = strstr( szTmp, "\r\n\r\n") ) != NULL) // message-header�� message-body�� ���(4BYTE)
            {
                bContent = true;
                nContentRead = nRead - (psztmp+4 - szTmp); // ���� �κ� �� message-header �κа� ��踦 ����
                ofs.write( psztmp+4, nContentRead );
            }
        }
    } while ( nContentRead < nContent ); // �ٿ� ���� message-body �κ��� Content-Length �� �������� ����    
	
    ofs.close();
    sock.Close();
    WSACleanup();   
	
    return true;
}
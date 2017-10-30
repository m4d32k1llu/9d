#include "..\stdafx.h"
#include "Network.h"
#include "..\Packet_Queue\Packet_Queue.h"

extern _queue_Packet *			g_pPacketQ	 ;	// Packet queue
extern _queue_Packet *			g_pUpdatePacketQ ;
extern _queue_Packet *			g_pCSPacketQ ;	// Party Packet queue
extern _queue_Packet *			g_pORPacketQ ;  // OR packet		
extern _queue_Packet *			g_pChatPacketQueue ; // Chat packet queue

void MessageInsert( _h_Recv_Overlapped * pOv, const int size )
{
	u_char ucMessageNum = static_cast<u_char>(pOv->pRealBuf[2]) ;

	// �Լ� �ۿ��� �θ� ���� ������, �����ϰ� ���⼭ ȣ���Ѵ�. 
	pOv->SetLastParsingSize( static_cast<u_short>(size) );
	pOv->SetLastParsingMsg( ucMessageNum );

	// �Ϲ� TCP packet
	if( ucMessageNum == CMSG_NO_UPDATE_CHARAC_ALL_ )
	{
		g_pUpdatePacketQ->insert( pOv, size ) ;
	}
	else if( ucMessageNum < 180 )
	{
		g_pPacketQ->insert( pOv, size ) ;
	}
	// PARTY ���� TCP Packet
	else if( ucMessageNum < 210 )
	{
		g_pCSPacketQ->insert( pOv, size ) ;
	}
	// ORGANIZATION ���� TCP Packet
	else if( ucMessageNum < 226 )
	{
		g_pORPacketQ->insert( pOv, size ) ;
	}
	else if( ucMessageNum < 255 )
	{
		g_pChatPacketQueue->insert( pOv, size ) ;
	}
	
}
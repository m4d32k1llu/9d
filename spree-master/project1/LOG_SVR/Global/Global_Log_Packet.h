#pragma once

#pragma pack( push, q23n4th )
#pragma pack(1)

#include "..\Global\Global_Log_Define.h"


class _h_DS_Com ;

// ������ �������� ���������θ� ���� ��Ŷ
#define MSG_NO_DISCONNECT					0
struct MSG_DISCONNECT						//# ������ ������ �������� ��Ŷ #
{
	u_short		sLength ;	
	u_char		cMessage ;

	_h_DS_Com*	pDSComm ;
} ;


#define MSG_NO_SERVER_STATE					1
struct MSG_SERVER_STATE {
	u_short		sLength ;	
	u_char		cMessage ;
	
	_ServerState		log_ServerState ;
};

#define MSG_NO_ITEM						2
struct MSG_ITEM {
	u_short		sLength ;	
	u_char		cMessage ;

	_ItemLog	log_Item ;
};


#define MSG_NO_CHARACTER				3
struct MSG_CHARACTER {
	u_short		sLength ;	
	u_char		cMessage ;
	
	_CharacLog	log_Charac ;
};

#define MSG_NO_LOGINLOG				4
struct MSG_LOGINLOG {
	u_short		sLength ;	
	u_char		cMessage ;
	
	_LoginLog	log_Login ;
};

#pragma pack( pop, q23n4th )
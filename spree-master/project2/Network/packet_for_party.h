#pragma once

// Party
// Main = 14
// 160 - 167

struct _sHeader;

#define MSG_NO_PARTY_JOIN_REQ				160		// ��Ƽ�� �߰� ��û �Ǵ� ���� ��û
#define MSG_NO_PARTY_MEMBER_UPDATE			161		// ��Ƽ�� �߰����� ���� �� ������
#define MSG_NO_PARTY_INFO					162		// ��Ƽ�� �����ϴ� ���������� �ѹ����� ������ ���ķδ� �����޼����� ���� ����
#define MSG_NO_PARTY_RESPONSE				163		// ��û ���
#define MSG_NO_PARTY_ETC					164		// ����
#define MSG_NO_PARTY_MEMBER_INFO			165		// ���� ���� �ִ� ��Ƽ���� ����

// PARTY_JOIN_REQ : S <=> C
// ��Ƽ���̰ų� �������� ���� ������ �ٸ� �������� ��Ƽ ��û�� �� ��, cTo�� ����� �̸��� 
// ���� ������ ����. ������ ����� ã�� ������ �˻��� �� ��󿡰� ���� ��Ŷ�� �����ų�, 
// ��û�ڿ��� RESPONSE�� ����.
//#define MSG_NO_PARTY_JOIN_REQ				112
struct MSG_PARTY_JOIN_REQ
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cTo[13] ;				// ��û��
} ;

// PARTY_JOIN_RESPONSE : S <=> C
// JOIN_REQ �� ���� �������� ���δ�. cCmd�� ��Ȳ�� ���� �ٸ� ���̴�.
// cCmd = -1 : ( S -> C ) ��Ƽ ������ �������� ��� ( ���� ������ ���� �� )
// cCmd = 0 : ( C -> S ) JOIN_REQ�� �޾��� ��, ��û���� �̸��� ä�� ������ ������.
// ��Ƽ ������ �����ϸ�, ��ٷ� PARTY_INFO �� �����̰�, �����ϸ� cCmd �� -1�� �����̴�. 
// cCmd = 1 : ( C -> S ) ����� �źθ� ������ ��, ��û���� �Ǹ��� ä�� ������ ������. 
// ( S -> C ) ��û�ڿ��� ����� �źθ� �������� �˸���.
// cCmd = 2 : ( S -> C ) ��Ƽ ��û�� �ߴµ�, ����� ������ ������ ����. �� ����� ���ٴ� �ǹ�. 
// cCmd = 3 : ( S -> C ) ����� �̹� �ٸ� ��Ƽ�� �����ִ�.
// cCmd = 4 : ( S -> C ) ��Ƽ �ο��� �� ���� ���̻� ��û�� �ȵȴ�.
// cCmd = 5 : ( S <=> C ) ... 
// cCmd = 6 : ( S -> C ) ����� �ٸ� �۾����̴�. ����� �õ��϶�.
// cCmd = 7 : ( S -> C ) ��ȣ�� ���迡 �������� �����̴�.

//#define MSG_NO_PARTY_RESPONSE				115
struct MSG_PARTY_JOIN_RESPONSE
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cCmd ;
	char				cToName[13] ;			// ���.
} ;



/* =====================================================================================
             MEMBER_UPDATE : ��Ƽ���� ������ ������Ʈ �ϰų�, ����/Ż����� �˸���.

             Cmd      0 - ĳ���͸� �߰� : cCharacName ��ȿ S->C
                      1 - ĳ���� ���� ����( ���� �̵� ) S->C
                      2 - ĳ���� ���� ���·�.
                      3 - ĳ���� �ڸ���� ���·�.
===================================================================================== */
//#define MSG_NO_PARTY_MEMBER_UPDATE		113
struct MSG_PARTY_MEMBER_UPDATE
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cCommand ;				// � �޽����ΰ�
	char				cMemberNo ;				// ��� ��ȣ 
	char				cCharacName[13] ;		// ��� ĳ���� �̸�
	u_short				ucUID ;					// unique id	
	char				cServerNo ;				
} ;

#define _XDEF_PARTY_MAX_MEMBER			9

//#define MSG_NO_PARTY_INFO			114
struct MSG_PARTY_INFO
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cShareExp ;				// ����ġ �й� ��� 0 = ���� ���, 1 = ���� ���, 
	char				cShareItem ;			// ������ �й� ��� 
	char				cCaptainNo ;			
	
	struct {		
		char			cMemberNo ;
		char			cMemberName[13] ;
		u_short			usUID ;
		char			cServerNo ;
#ifdef _XTS_PARTYMOREINFO
		short			sInnerLevel ;	// ��������(����)
		char			cMoonpa ;		// ����
		char			cClass ;		// ����
		int				iHonor ;		// ��
#endif

#ifdef _XDEF_PARTY_NOTAVAILABLE
		u_char			ucGameState : 2;		// �⺻=0 �ڸ����=1
        u_char			ucClassGrade : 2;	// ���� ��� : 0 ~ 3 : 20080410
		u_char			ucReserved : 4;
#endif
	} Member[_XDEF_PARTY_MAX_MEMBER] ;
} ;

// ��Ƽ ��Ÿ ��ɾ�.
// cCmd : 
// 0 : C->S ��ü ��û���� ��Ƽ���� �����̴�.
// 0 : S->C ��Ƽ���鿡�� ��Ƽ�� ��ü�� �˸���.
// 1 : C->S ����ġ �й� ����� �������� ��( ��Ƽ���� ����) 
//				-------------------------------------------------  
//				uFlag1					uFlag2			����
//				-------------------------------------------------
//				0(����ġ �й�)			0				���κй�
//										1				�����й�
//				-------------------------------------------------
//				1(������ �й�)			0				���κй�
//										1				�����й�
//										2				�����й�
//				-------------------------------------------------
// 1 : S->C ����ġ �й� ����� �ٲ���� ��, ��Ƽ���鿡�� �˸�
// 2 : S->C �������� ��Ƽ �̻����� ��ü�Ǿ���.
// 3 : C->S ����, ��Ƽ���� ����(MEMBER_INFO)�� ���� ��ϵ��� ���� ĳ������ ������ ���
// ���� �� ��ȣ�� ������ ��Ƽ ������ �ٽ� ��û�Ѵ�. 
// 4 : C->S cInfo = �����ȣ : �߹� ��û
// S->C cInfo = �����ȣ : ��Ƽ�忡 ���� �߹�Ǿ���.
// S->C cInfo = -1 : ��Ƽ���� �߹��û �̻�..
// 5 : C->S cInfo = ���Ǹ����ȣ : Ż�� ��û
// S->C cInfo = �����ȣ : Ż�� �Ͽ���.
// 6 : C->S cInfo = �����ȣ, ��Ƽ�忡 ���� ���� ���� ��û
// S->C cInfo = �����ȣ. ��Ƽ���� ������ �����ߴ�.
// 7 : S->C ����Ŀ� �õ��Ͻʽÿ�.( ���� ����̵� )
// 8 : C->S ������ : ������ ���� �ҷ�?
// 8 : S->C ���� : ������ ������ ���ڴµ�~
// 9 : C->S ����� : �׷�����.. (uFlag1: ���Թ�ȣ, uFlag2: 0.�ź�, 1.�³�)
// 9 : S->C(����) : �������� �����ϴ� ��Ƽ���� �����忡�� ������. (uFlag1: ���Թ�ȣ, uFlag2: 0.�ź�, 1.�³�)
//10 : C->S (����) : ������ ���� ���..
//10 : S->C (�����) : ������ ������ ���⸦ ��������� ������鿡�� �˸�..
//	info : 0,�������� ���� ���̾�α� �ڽ� �Ҹ�  , 1, �������� ������ ����Թ�ư���
	   
 //#define MSG_NO_PARTY_ETC				116		// ����
struct MSG_PARTY_ETC
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cCmd ;
	union{		
		char				cInfo ;
		struct {
			unsigned	uFlag1 : 4 ;
			unsigned	uFlag2 : 4 ;				
		} _Flag ;
	} ;
	
} ;

// ���� ������ �ִ� ��Ƽ���� �������� �˸���.
//#define MSG_NO_PARTY_MEMBER_INFO		117
struct MSG_PARTY_MEMBER_INFO
{
	_sHeader			header ;
	u_char				ucMessage ;
	char				cMemberNo ;			// ��� ��ȣ : �̰����� ������.
	
	char				cServerNo ;			// �� �ε���
	short				sPosX ;
	short				sPosZ ;
	short				sInnerLevel ;		// ��������(����)
	char				cVitalPercent ;		// ü�� �����
	char				cZenPercent ;		// ���� �����
	
	union {
		struct _SlotCheck {
			unsigned	slot0 : 1 ;
			unsigned	slot1 : 1 ;
			unsigned	slot2 : 1 ;
			unsigned	slot3 : 1 ;
			unsigned	slot4 : 1 ;
			unsigned	slot5 : 1 ;
			unsigned	slot6 : 1 ;
			unsigned	slot7 : 1 ;
			unsigned	slot8 : 1 ;			
		} _slot ;
		WORD sCheck ;
	};
	
} ;
	

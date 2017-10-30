#pragma once



//#################################################################################
//
//  Nic ��Ŷ 
//  27: 216 ~ 223
//
//#################################################################################


struct _sHeader ;

#define MSG_NO_NIC_LIST_REQ_C_S			216			// ��ȣ ����Ʈ�� ��û
#define MSG_NO_NIC_LIST_S_C				216			// ��ȣ ����Ʈ�� ��ȯ
//#define MSG_NO_NIC_ADD_REQ_C_S			217			// ��ȣ �߰��� ��û
#define MSG_NO_NIC_SET_S_C				217			// ���Կ� ��ȣ�� �����Ѵ�.
#define MSG_NO_NIC_DELETE_REQ_C_S		217			// ��ȣ ����
#define MSG_NO_NIC_SELECT_REQ_C_S		218			// ��ȣ�� �����Ѵ�.
#define MSG_NO_NIC_SELECT_S_C			218			// ������ ��ȣ ������ ��ȯ�Ѵ�.
#define MSG_NO_NIC_RES					219			// ��Ÿ REQ ��Ŷ�� ���� ���� �Ǵ� ���� ��ȯ��..
#define MSG_NO_NIC_USEFULL_TIME_S_C		220			// ������ ��ȣ�� ���� ��ȿ ���ð�(��)
#define MSG_NO_RANK_HIDING				221			// ���� ����,����  ( C<->S ) 
#define MSG_NO_HONOR_PLAYER				222			// �÷��̾��� �� ��

//#define MSG_NO_NIC_LIST_REQ_C_S
struct MSG_NIC_LIST_REQ {
	_sHeader	header ;
	u_char		ucMessage ;	
};
//#define MSG_NO_NIC_LIST_S_C	
enum 
{
	_NIC_COUNT_NOTHING			= 1,
	_NIC_COUNT_SPECIAL_SHOWDOWN = 10,
	_NIC_COUNT_SPECIAL_MOOMPA	= 10,
	_NIC_COUNT_SPECIAL_LEVEL	= 10,
	_NIC_COUNT_SPECIAL_MARTIAL	= 50,
	_NIC_COUNT_GENERAL_SHOWDOWN	= 10,
	_NIC_COUNT_GENERAL_QUEST	= 50,
	_NIC_COUNT_GENERAL_MOONPA	= 1,
	_NIC_COUNT_GENERAL_MAKER	= 10,
	_NIC_COUNT_GENERAL_GATER	= 10,
	_NIC_COUNT_GENERAL_EVENT	= 20,
	_NIC_COUNT_GENERAL_MONSTER	= 30,
	_NIC_COUNT_TOTAL			= 212,
} ;

struct _CHARAC_NIC {

	short			sSelectNic ;		// ������ ��ȣ
	short			sUsedTime ;			// ���Ǿ��� �ð�

	union
	{
		struct _NicData {
			unsigned short   usNothing[_NIC_COUNT_NOTHING] ;   // 0�� ������ �ƹ��͵� �������� �ʾ������� ������ ����д�.
			struct 						// Ư�� ��ȣ
			{
				u_short		usShowdown[_NIC_COUNT_SPECIAL_SHOWDOWN] ;	// Ư���񹫺�ȣ
				u_short		usMoonpa[_NIC_COUNT_SPECIAL_MOOMPA] ;		// Ư�����ĺ�ȣ
				u_short		usLevel[_NIC_COUNT_SPECIAL_LEVEL] ;		// Ư����޺�ȣ
				u_short		usMartial[_NIC_COUNT_SPECIAL_MARTIAL] ;		// Ư��������ȣ
			} _SpecialNic ;

			struct 						// �Ϲݺ�ȣ
			{
				u_short		usShowdown[_NIC_COUNT_GENERAL_SHOWDOWN] ;		// �񹫺�ȣ
				u_short		usQuest[_NIC_COUNT_GENERAL_QUEST] ;		// ǳ��� ��ȣ
				u_short		usMoonpa[_NIC_COUNT_GENERAL_MOONPA] ;		// ���� ��ȣ
				u_short		usMaker[_NIC_COUNT_GENERAL_MAKER] ;		// ���ۺ�ȣ
				u_short		usGater[_NIC_COUNT_GENERAL_GATER] ;		// ������Ī
				u_short		usEvent[_NIC_COUNT_GENERAL_EVENT] ;		// �̺�Ʈ��ȣ
				u_short		usMonster[_NIC_COUNT_GENERAL_MONSTER] ;
			} _GeneralNic ;
		} _Nic ;
		u_short				usNicID[_NIC_COUNT_TOTAL] ;
		BYTE				bitData[sizeof(_NicData)] ;
	} ;
	
};
struct MSG_NIC_LIST {
	_sHeader	header ;
	u_char		ucMessage ;
	
	_CHARAC_NIC	nicData ;
};

/*
//#define MSG_NO_NIC_ADD_REQ_C_S
struct MSG_NIC_ADD_REQ {
	_sHeader	header ;
	u_char		ucMessage ;

	char		cSlotNum ;
	u_short		usNicID ;
};
*/

//#define MSG_NO_NIC_SET_S_C	
struct MSG_NIC_SET {
	_sHeader	header ;
	u_char		ucMessage ; 

	short		sSlotNum ;
	u_short		usNicID ;
};
//#define MSG_NO_NIC_SELECT_REQ_C_S
struct MSG_NIC_SELECT_REQ {
	_sHeader	header ;
	u_char		ucMessage ;

	short		sSlotNum ;	// ������ ����.. ���Թ�ȣ�� ��û�Ѵ�. (0 �̸�.. ���� ����, -1 �̸� ���κ�ȣ ���� )	
	enum{
		_SELECT_FREE = 0,		// ��ȣ ���� ����
		_SELECT_PERSONAL = -1,	// ���� ��ȣ ����
		_SELECT_FREEMATCH = -2,	// ���� �� ��ȣ ����
	} ;
};

//#define MSG_NO_NIC_SELECT_S_C	
struct MSG_NIC_SELECT {
	_sHeader	header ;
	u_char		ucMessage ;
	
	u_short		usID ;			// ĳ������ uid
	u_short		usNicID ;		// ���Թ�ȣ�� ��û�޾�.. ���̵�� ��ȯ�Ѵ�. (�ڽ��� ������ �ֺ��� ��������.)
								// 0 �̸� ���� ����..	'0xff' �̸� ���κ�ȣ ����
	enum{
		_SELECTED_FREE_ = 0,			// ��ȣ ���� ������
		_SELECTED_PERSONAL_ = 0xffff,		// ���� ��ȣ ���õ�
	} ;
};
//MSG_ZONE_CHARAC_INFO ����

//#define MSG_NO_NIC_RES		
struct MSG_NIC_RES {
	_sHeader	header ;
	u_char		ucMessage ;

	u_char		ucCommand ;
	u_char		ucCode ;
};

/*============================================================================

  MSG_NIC_RES ����

	ucCommand : � ��Ŷ�� ���� ��� ��Ŷ���� �����ϱ� ����..
	ucCode	  : ���� �Ǵ� ���� �ڵ��̴�..

--------------------------------------------+-----------------------------------+---------------------------
 	ucCommand								|	iCode							|		��
--------------------------------------------+-----------------------------------+---------------------------
 	MSG_NO_NIC_LIST_REQ_C_S					|	_RESULT_CODE_SUCCESS			|	? (�����ϸ� ����Ʈ�� ��)
 											|	_RESULT_CODE_NOTCASE			|	���� �Ҹ���
											|	_RESULT_CODE_ERROR				|	��Ÿ ����
--------------------------------------------+-----------------------------+---------------------------------
	MSG_NO_NIC_UPDATE_REQ_C_S				|	_RESULT_CODE_SUCCESS			|	�ѽ�~ ����
											|	_RESULT_CODE_NOTCASE			|	���� �Ҹ���
											|	_RESULT_CODE_ERROR				|	��Ÿ ����
--------------------------------------------+-----------------------------+---------------------------------
	MSG_NO_NIC_ADD_REQ_C_S					|	_RESULT_CODE_SUCCESS			|	�ѽ�~ ����
											|	_RESULT_CODE_NOTCASE			|	����
											|	_RESULT_CODE_ERROR				|	��Ÿ ����
--------------------------------------------+-----------------------------+---------------------------------
	MSG_NO_NIC_SELECT_REQ_C_S				|	_RESULT_CODE_SUCCESS			|	? (�����ϸ� ������ NicID �� ��)
											|	_RESULT_CODE_NOTCASE			|	����
											|	_RESULT_CODE_ERROR				|	��Ÿ ����
--------------------------------------------+-----------------------------+---------------------------------
*/

//#define MSG_NO_NIC_USEFULL_TIME_S_C		220			// ������ ��ȣ�� ���� ��ȿ ���ð�(��)
struct MSG_NIC_USEFULL_TIME {
	_sHeader	header ;
	u_char		ucMessage ;

	short		time ;
};

//#define MSG_NO_NIC_DELETE_REQ_C_S		217			// ��ȣ ���� ��û
struct MSG_NIC_DELETE_REQ {
	_sHeader	header ;
	u_char		ucMessage ;

	short		sSlotNum ;	//	- ������ ���Թ�ȣ	
};

//#define MSG_NO_RANK_HIDING				221			// ���� ����,����  ( C<->S ) 
struct MSG_RANK_HIDING
{
	_sHeader	header ;
	u_char		ucMessage ;
	char		cMode ;
	enum
	{
		_HIDE_OFF=0,	// ���� ����
		_HIDE_ON,		// ���� ����
	};
};

//#define MSG_NO_HONOR_PLAYER				222			// �÷��̾��� �� ��
struct MSG_HONOR_PLAYER 
{
	_sHeader	header ;
	u_char		ucMessage ;
	
	char		cMode ;
	enum
	{
		_GIVE_REQ = 0,				// �� �ο� ��û
		_GIVE_SUCCESSED,			// �� �ο� ����
		_GIVE_FAILED,				// �� �ο� ����(�ð� ����)
		_TAKE_FROM_PLAYER,			// PC�� ���� ���� �ο��޾ҽ��ϴ�.
		_TAKE_FROM_SERVER,			// server �� ���� ���� �ο� �޾ҽ��ϴ�.
	} ;
	char		cToCharacName[13] ;
	int			iHohorValue ;		// +�ο�, - �϶�
};
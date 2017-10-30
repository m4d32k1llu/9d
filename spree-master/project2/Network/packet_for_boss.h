#pragma once

/*========================================================================================

  �������� �� (��������, ���ù� ��..) 

  Message Scope : 23( 184 ~ 191), 24 ( 192 ~ 199 )

  �츮 ������ ��Ŷ��ȣ�� ���Ǿ� ���� �ִ�.  �Ʋ�����.. 

=========================================================================================*/

#define MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S	184	// ������ ���� ��û
#define MSG_NO_EX_BATTLE_INVITE_S_C				185	// ������ ������ �ش� ĳ���鿡�� �ʴ�
#define MSG_NO_EX_BATTLE_ENTER_REQ_C_S			186	// �ʴ���� ĳ���Ͱ� �����濡 �����Ҷ�..
#define MSG_NO_EX_BATTLE_LEAVE_REQ_C_S			187	// �濡 �ִ� ĳ���Ͱ� ������..

#define MSG_NO_EX_BATTLE_RESULT_S_C				188	// REQ �޽����鿡 ���� ���� �޽���
#define MSG_NO_EX_BATTLE_PLAY_VIDEO				189	// �÷��� ���� �������..
#define MSG_NO_EX_BATTLE_TIME_S_C				190	// ������ Ŭ���̾�Ʈ�� �ð� ����ȭ..

#define MSG_NO_EX_BATTLE_TYPE_INFO_REQ_C_S		191 // �� Ÿ�Կ� ���� ������ ���´�.
#define MSG_NO_EX_BATTLE_TYPE_INFO_RES_S_C		192 // �� Ÿ�Կ� ���� ���� ����.
#define MSG_NO_EX_BATTLE_CUR_STAGE_S_C			193	// �������� �������� ���� �������� ��ȣ..


// �� ���
//enum {
//	_MODE_EX_BATTLE_ROOM_NORMAL,
//		_MODE_EX_BATTLE_ROOM_SOLO,
//		_MODE_EX_BATTLE_ROOM_PARTY,
//};

//#define MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S		// ������ ���� ��û
struct  MSG_EX_BATTLE_ROOM_CREATE_REQ 
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cType ;						// ������ ������ �ʹ�ȣ	
	char				cLevel ;					// ������ ���̵� (0-easy, 1-normal, 2-hard, 3-expert)
	char				cEnterItemSlotNum[3] ;		// ����� �������� �ִ� �κ� ���� ��ȣ..

#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI	
	char				cTicket;					// ��(ĳ����)���� ������ ���� (0 : ��3���Ҹ�    1 : ��1���Ҹ�)
#endif

	unsigned char		uMode : 4 ;					// ���� ����(0.Normal, 1.����, 2.���� )	
	unsigned char		uPadding : 4 ;

};

//#define MSG_NO_EX_BATTLE_INVITE_S_C					// ������ ������ �ش� ĳ���鿡�� �ʴ�
struct  MSG_EX_BATTLE_INVITE {
	_sHeader			header ;
	u_char				ucMessage ;

	int					iRoomID ;					// �� ����� ���̵�? ( �� ���� ���� ^^ )
	char				cType ;
	float				fX ;
	float				fZ ;
};

//#define MSG_NO_EX_BATTLE_ENTER_REQ_C_S				// �ʴ���� ĳ���Ͱ� �����濡 �����Ҷ�..
struct  MSG_EX_BATTLE_ENTER {
	_sHeader			header ;
	u_char				ucMessage ;

	int					iRoomID ;					// �� ����� ���̵�? ( �� ���� ���� ^^ )
};

// LEAVE ��Ŷ�� ���� �ʿ��ұ�? �켱 ������~
//#define MSG_NO_EX_BATTLE_LEAVE_REQ_C_S				// �濡 �ִ� ĳ���Ͱ� ������..
struct  MSG_EX_BATTLE_LEAVE {
	_sHeader			header ;
	u_char				ucMessage ;

	char				cCause ;					// ������ ����? ( 0. Ż�� 1.���, 2.�α׾ƿ�)	
};




/*============================================================================

  MSG_NO_EX_BATTLE_RESULT_S_C ����

	ucCommand : � ��Ŷ�� ���� ��� ��Ŷ���� �����ϱ� ����..
	iCode	  : ���� �Ǵ� ���� �ڵ��̴�..
	
  --------------------------------------------+-----------------------------------+---------------------------
  ucCommand								|	iCode							|		��
  --------------------------------------------+-----------------------------------+---------------------------
  MSG_NO_EX_BATTLE_ROOM_CREATE_REQ_C_S	|	_RESULT_CODE_SUCCESS			|	�ѽ�~ ����
									    |	_RESULT_CODE_NOTCASE			|	���� �Ҹ���
										|	_RESULT_CODE_ERROR				|	���̻� �� ���� �Ұ�
										|	_RESULT_CODE_ALREADY_PARTY		|	��Ƽ�� �̹� ���������̴�.											
  --------------------------------------------+-----------------------------+---------------------------------
  MSG_NO_EX_BATTLE_ENTER_REQ_C_S		|	_RESULT_CODE_SUCCESS			|	�ѽ�~ ����
										|	_RESULT_CODE_NOTCASE			|	���� �Ҹ���
										|	_RESULT_CODE_ERROR				|	��Ÿ ����
  --------------------------------------------+-----------------------------+---------------------------------
  MSG_NO_EX_BATTLE_LEAVE_REQ_C_S		|	_RESULT_CODE_SUCCESS			|	�ѽ�~ ����
										|	_RESULT_CODE_NOTCASE			|	���� (�̶� ��¼��?  ��.��)
										|	_RESULT_CODE_ERROR				|	��Ÿ ����
  --------------------------------------------+-----------------------------+---------------------------------
  MSG_NO_EX_BATTLE_TYPE_INFO_REQ_C_S	|	_RESULT_CODE_ALREADY_PARTY		|	��Ƽ�� �̹� ���������̴�.
  --------------------------------------------+-----------------------------+---------------------------------


=============================================================================*/

enum{
	_RESULT_CODE_SUCCESS,
		_RESULT_CODE_NOTCASE,
		_RESULT_CODE_ERROR,
		_RESULT_CODE_ALREADY_PARTY,
};

//#define MSG_NO_EX_BATTLE_RESULT_S_C					// REQ �޽����鿡 ���� ���� �޽���
struct  MSG_EX_BATTLE_RESULT 
{
	_sHeader			header ;
	u_char				ucMessage ;

	u_char				ucCommand ;					// �����Ŷ�� ������ ucMessage ��ȣ
	int					iCode ;						// ������

#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI
	// ���� �г� ���� ����Ͽ����� ����� �κ� ��ġ�� ������ �����Ѵ�,
	struct 
	{
		char			itemSlotNum;				// ��ġ
		u_char			itemCount;					// ����
	}_sUsedItem[3];

#endif

};

/*
������ ���� ���� ���

  1. ��Ƽ���� ���� �� �߰��� _SKIP_VIDEO_ �� ������ ������ �������� ��ȯ�ϰų�
  Ż�ⱸ�� ���µ��� �̺�Ʈ�� �߻���Ű�� �� �޽����� ��� ĳ���鿡��
  ��ε��ɽ��� �ϰ� �ȴ�. �� �޽����� �޴� ĳ���͵��� �������̴� ������ ��������.
  
	2. ��Ƽ���߿� Ȥ�� �ַκ����� ���� ������ ������ �� ������(�߰��� ��ŵ ����)�� ������
	_END_VIDEO_�޽����� ������. ������ �� �޽����� �ް� ���� ���͸� ��ȯ�ϰų�
	Ż�ⱸ�� �����̴�.
*/
enum{	// uCommand
	_SKIP_VIDEO_,
	_PLAY_VIDEO_,
	_END_VIDEO_,
} ;
enum{	// uStep
	_NOTHING_,
	_BOSS_STEP_,	
	_FINAL_STEP,
} ;
//#define MSG_NO_EX_BATTLE_PLAY_VIDEO				// �÷��� ���� �������..
struct MSG_EX_BATTLE_PLAY_VIDEO {
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cType ;						// �� ��ȣ
	u_char				uCommand ;					// 0. _SKIP_VIDEO_,  1. _PLAY_VIDEO_
	u_char	            uStep ;				   // ���° ����?(0���� ����) 
};

//#define MSG_NO_EX_BATTLE_TIME_S_C					// ������ Ŭ���̾�Ʈ���� ����ð��� �˷��ش�.(���������� ����)
struct MSG_EX_BATTLE_TIME {
	_sHeader			header ;
	u_char				ucMessage ; 

	u_short				usTimeCount ;				// ������ ���� �ð�..
};

//#define MSG_NO_EX_BATTLE_TYPE_INFO_REQ_C_S		191 // �� Ÿ�Կ� ���� ������ ���´�.
struct MSG_EX_BATTLE_TYPE_INFO_REQ {
	_sHeader			header ;
	u_char				ucMessage ; 
	char				cType ;
};


//#define MSG_NO_EX_BATTLE_TYPE_INFO_RES_S_C		192 // �� Ÿ�Կ� ���� ���� ����.
struct MSG_EX_BATTLE_TYPE_INFO 
{
	_sHeader			header ;
	u_char				ucMessage ;

	struct			// �ʿ��� ������ ����
	{
		char	cType ;		// ��з�..
		short	sID ;		// ������ ���̵�..
	} _sItem[3] ;

#ifdef _XDEF_DUNGEONTICKET_070615_KUKURI

	char				isTicket;			// �κ��� �ش� �ῡ ���� �а� �ִ°�? 1:�ִ� 0:����

	struct		// ĳ�� ������ ����
	{
		char			cType;
		short			sID;
	}_sChargedItem[3];

#endif
};

// #define MSG_NO_EX_BATTLE_CUR_STAGE_S_C			193	// �������� �������� ���� �������� ��ȣ..
struct MSG_EX_BATTLE_CUR_STAGE {
	_sHeader			header ;
	u_char				usMessage ;
	
	char				cCurStage ;			// ���� �������� ��ȣ..
};

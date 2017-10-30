#pragma once

#define MSG_NO_UNI_GROUP_LOBBY                                               240

struct MSG_UNI_GROUP_LOBBY                // ���� ������ ��Ŷ ���̽� : ��� ��Ŷ�� ucMode �� ����.
{
	_sHeader header ;
    u_char   ucMessage ;
	u_char   ucMode ;				// 
} ;

enum
{
	en_ug_simple_grp_info=0,                    // ���� ������ ����.
	en_ug_select_simple_grp=1,                  // ���� ������ ����.
	en_ug_select_simple_grp_fail=2,             // ���� ������ ���� ����.
	en_ug_req_simple_grp_info=3,				// ���� ������ ���� ��û 

	en_ug_detail_grp_info=10,                   // ���� �������� ����.
	en_ug_move_request,                         // 11 ĳ���Ϳ� â�� �̵� ��û�Ѵ�.
	en_ug_move_result_complete,                 // 12 �̵��� ����.
	en_ug_move_result_change_name,              // 13 �̸��� �ٲ� �ʿ䰡 �ִ�.
	en_ug_move_result_fail,                     // 14 �̵� ��û ����.

	en_ug_change_name_request=20,               // �̸� �ٲٱ� ��û.
	en_ug_change_name_result,                   // 21 ���.

	en_ug_notyet_move_in_new=30,		// 30 �ż����� : ���� ������ �Ϸ����� �ʾҴ�.
	en_ug_force_use_charac_in_new,		// 31 �ż����� : ���� ������ �Ϸ����� ���� ����������, ������ ������ �Ϸ��ϵ��� ����.
	en_ug_already_move_in_old,			// 32 �̹� ������ �Ϸ�� ������������ ����. �޼��� �����ְ� ƨ��.
	en_ug_force_use_result,				// 33 ������ force_use ��Ŷ �ϳ��� �Ϸ��� ������, �ֹι�ȣ �Է��� ���ԵǾ� ��û/�������� ���´�.
} ;

struct MSG_UG_SIMPLE_GROUP_INFO : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_simple_grp_info
	u_char	ucGroup_0_NoCharac ;   // ��õ�� ĳ���� ��.
	
	struct 
	{
		u_char	ucMoveState ;      // �̵� ���� : 0 - �̿�, 1 - �Ϸ�� ����(�̵���ų�� ����)
		u_char	ucNoCharac[3] ;    // �� �� �������� ĳ���� ��.
	} GroupInfo[3] ;               // �׷츶���� ���¿� ĳ���� ����.
} ;

struct MSG_UG_SELECT_SIMPLE_GRP : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_select_simple_grp
	u_char	ucSelectNewGroupNo ;   // ������
} ;

struct MSG_UG_SELECT_SIMPLE_GRP_FAIL : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_select_simple_grp_fail
	u_char	ucSelectSimpleGrpFail ;    // ���� ���� : "�̹� ���� �Ϸ�� �������Դϴ�."
} ;

struct MSG_UG_DETAIL_GRP_INFO : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_detail_grp_info=10
    // ĳ���� �̸�. ����. (â���.. )

    u_char		ucCharacCount ; // 3�� ���������� ĳ���� ����
    u_char      ucDepotCount ;  // �̾ ���� �Ǵ� â�� ������ ����. 0�̶�� â�� ���� �Ȱ�.

    struct 
	{
		char		cOldGroupIndex ;	// 1,2,3 / 4,5,6 / 7,8,9
		u_char      ucCharacIndex ;		// 0,1,2 ������ ĳ���� �ε���
		short       sLevel ;            // ���� �ܰ�
		char        cName[13] ;         // �̸� 
     } DetailInfo[9] ;   // Max 9
} ;

struct MSG_UG_MOVE_REQUEST : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_move_request,          // 11 ĳ���Ϳ� â�� �̵� ��û�Ѵ�.
	u_char	ucSelectCharac[3] ;		// ���� ĳ���� �ε���. ������. 0~2, 0~2 ( 0-1, 1-2 ) ������ 0xff
	u_char	ucSelectDepotGroup ;    // ���� â�� ������ �ε���. 0,1,2 ������ 0xff
	char    cPassword[13] ;         // �ֹι�ȣ ���ڸ� �Ǵ� ��Ÿ.
} ;

struct MSG_UG_MOVE_RESULT_COMPLETE : public MSG_UNI_GROUP_LOBBY
{
   // en_ug_move_result_complete

   // 
	char	ucCompleteName[3][13] ;    // �ű� ĳ���� �̸�.
	u_short	usInnerLevel[3] ;			// ĳ���� ����.
	u_char	ucState[3] ;               // 0 - �Ϸ�, 1 - �̸��� �ٲ�� �Ѵ�.
	u_char	ucCompleteDepotGroup ;     // ������, 0xff
} ;

struct MSG_UG_MOVE_RESULT_FAIL : public MSG_UNI_GROUP_LOBBY
{
	//     en_ug_move_result_fail,  // 14 �̵� ��û ����.
	u_char	ucFail ;     // �������...
} ;

struct MSG_UG_CHANGE_NAME_REQUEST : public MSG_UNI_GROUP_LOBBY
{
    // en_ug_change_name_request=20, // �̸� �ٲٱ� ��û.
	u_char	ucIndex ;                // 0,1,2 �ٲٷ��� ĳ������ �ε���.
	char	ucNewName[13] ;          // ���̸�
} ;

struct MSG_UG_CHANGE_NAME_RESULT : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_change_name_result,  // 21 ���.
	u_char	ucSuccess ;           // 0 - Success, ������ ����.
	u_char	ucIndex ;             // 
	char	ucNewName[13] ;       // 
} ;

struct MSG_UG_NOTYET_MOVE_IN_NEW : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_notyet_move_in_new : Server -> Client �뺸.
	
} ;

struct MSG_UG_FORCE_USE_CHARAC_IN_NEW : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_force_use_charac_in_new : Client -> Server �� ��û 
	// DS �� ���� ����, ������ ��ȣ�� ������ ��.	
	char  cPW[13]; 
} ;

struct MSG_UG_ALREADY_MOVE_IN_OLD : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_already_move_in_old : Server -> Client	
} ;

struct MSG_UG_REQ_SIMPLE_GRP_INFO : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_req_simple_grp_info
} ;

struct MSG_UG_FORCE_USE_RESULT : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_force_use_result
	u_char				ucResult ;	// 0 - Success, 1 - ��� Ȯ��, 2 - ����.
} ;
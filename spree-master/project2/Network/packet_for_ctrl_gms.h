#pragma once
/** 
	@file packet_for_ctrl_gms.h
*/

// GMS
// Main = 9
// 72 - 79

#define MSG_NO_CTRL_MONSTER_SPAWN		248
#define MSG_NO_SERVER_INFO				249
#define MSG_NO_CTRL_CHARAC_SETTING		250		//S->C

// ���� ��ȯ
struct MSG_CTRL_MONSTER_SPAWN
{
	_sHeader			header;
	u_char				ucMessage;

	short				sMobType;			// ��ȯ�� ���� Ÿ��

	float				fX;
	float				fZ;

	char				cNumber;			// ��ȯ�� ������ : �ִ� 24
};

//#define MSG_NO_CTRL_CHARAC_SETTING			250
enum __ctrl_charac_enum
{
	ctrl_ch_point=0,
	ctrl_ch_position,
	ctrl_ch_charac_moonpa,
	ctrl_ch_name,
	ctrl_ch_item,
	ctrl_ch_money,
	ctrl_ch_skill,
	ctrl_ch_quest,
	ctrl_ch_class_grade,
	ctrl_ch_set_transparent,	// ���� ���� ����
	ctrl_ch_set_all_point,		// ��� ����(�Ŀ�) �ʱ�ȭ : ����,����,���� ����.	
	ctrl_ch_contribution,		// �⿩�� ���� : �߰��κ� 2005/03/24
	ctrl_ch_mastery_point,		// �����͸� ����Ʈ ����
	ctrl_ch_set_mutetime,		// ä�� ���� �Ǵ� ����
	ctrl_ch_full_fury,			// �г븦 �� ä��
	ctrl_ch_full_bloodpoint		// ���� ����Ʈ �� ä��
} ;

// 5��/����/����Ʈ ����
struct __ctrl_charac_point
{
	char		cWhere ;		// 0 : �ǰ��� ����, 1 : ����� ����, 2. ������ ����, 3. ��ø 4, �ٷ� 5 - Left Point
	short		sValue1 ;		// �� ����Ʈ �� 
	short		sValue2 ;		// ���簪�� �ִ밪( Power ) �� �̰��� ����.
} ;


// ĳ���� ��ġ ����.
struct __ctrl_charac_position
{
	float		fX ;
	float		fZ ;
	char		cZone ;			// 0�̰ų�, ���� ������� �����̵� ����.
} ;

// ���� ����.
struct __ctrl_charac_moonpa
{
	char		cToMoonpa ;		// ���ĸ� �ٲ۴�.
} ;

// �̸��̳�, �г��� �ٲٱ�.
struct __ctrl_charac_nick_name
{
	char		cMode ;			// 0 �� �̸� �ٲٱ�, 1�� �г��� �ٲٱ�.
	
	char		cName[13] ;
} ;


// �κ���/����� ������ ����
struct __ctrl_charac_item
{
	char				cMode ;		// 0 - inven item setting, 1 - storage item setting
	_sUser_Inven_v1_all useritem;
} ;

// �� ����
struct __ctrl_charac_money 
{
	char		cWhere ;			// 0 - �κ��丮, 1 - â��.(Depot)
	u_int		uiGold ;			// ���� �������. ������ �״�� ����.
} ;


struct __ctrl_charac_skill
{
	char		cMode ;				// 0 - Add Skill, 1 - Remove Skill, 2 - Edit Skill(Set Step)
	
	u_char		ucSkillSlot ;
	short		sSkillID ;
	char		cSkillStep ;
	
	// 2006.02.06 insert
	int			iSkillExp;
} ;

struct __ctrl_quest_data
{
	char		cMode ;			// 0 - Add Skill, 1 - Remove Skill, 2 - Edit Skill(Set Step), 3 : trainstate=1
	
	u_short		usQuestID ;	// ����� ����Ʈ ���̵�. 
	char		cNodeID;    // ����� ��� ���̵�. 
};

struct __ctrl_class_update
{
	char		cClass;
	char		cGrade;
	short		sAddMasteryPoint;
};

struct __ctrl_charac_set_trasparent
{
	char		cMode;			// 0:Off   1:On
};


struct __ctrl_charac_all_point
{
	short		sStrength ;					// �ٷ� ����
	short		sZen;						// ����
	short		sIntelligence;				// ����
	short		sConstitution;				// �ǰ�
	short		sDexterity ;				// ��ø ����
	short		sMaxLifePower ;				// �ִ� ü��( ���� ȿ�� �������� �⺻ �ִ� ü�� )
	short		sMaxForcePower ;			// �ִ� ����
	short		sMaxConcentrationPower ;	// �ִ� ���� 
	short		sLeftStat;					// ���� ���� ����Ʈ	
	short		sLeftMastery;				// ���� ���� ����Ʈ
};

struct MSG_CTRL_CHARAC_SETTING
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cCommand ;			// __ctrl_charac_enum ��.
	
	union 
	{
		__ctrl_charac_point			point ;
		__ctrl_charac_position		position ;
		__ctrl_charac_moonpa		moonpa ;
		__ctrl_charac_nick_name		nickname ;
		__ctrl_charac_item			item ;
		__ctrl_charac_money			money ;
		__ctrl_charac_skill			skill ;
		__ctrl_quest_data			quest;
		__ctrl_class_update			Class;
		__ctrl_charac_set_trasparent	Transparent;
		__ctrl_charac_all_point		AllPoint;
		int							iContribution;
		short						sMasteryPoint;
		int							iMuteTime;
	} ;
} ;

#define MSG_NO_GM_REQUEST					252			// C->S : gm Ŭ���̾�Ʈ�� ��û
enum		// ��û ��Ŷ
{
	en_gm_req_nextto_charac=0,		// gm �� ĳ���� ������ ����.
	en_gm_req_charac_move,			// ����� �̵� ��Ŵ.
	en_gm_req_npc_kill_sign,		// npc�� ���ش�.
	en_gm_req_enter_warp,			// ��� �ڽ��� ���� �������� ����. : �̱���
	en_gm_req_charac_moveto_warp,	// ��� ĳ���͸� ���� �������� �鿩������. : �̱���
} ;

enum		// ��� or ���� ��ȣ.
{
	en_gm_result_success=0,				// �����̴�.( �� ���� �Ȱ����� �ִ�. )
	en_gm_result_charac_not_connect,	// ������ ���� �ʴ�.
	en_gm_result_charac_other_zone,		// �ٸ� �ʵ忡 �ִ�.
	en_gm_result_npc_not_found,			// �׷� npc ����.
	en_gm_result_server_off,			// ��� ������ ���� �ִ�.
} ;

struct MSG_GM_REQUEST		// �̰� ��ü�δ� ���� �ʰ�, ��ӹ޾� ����.
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucRequest ;
} ;


#define MSG_NO_GM_RESULT					252			// ���� ��ȣ�� ó��.
struct MSG_GM_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucRequest ;
	u_char				ucResult ;		// ���.( ���� ���� �� ����. )
	u_char				ucInfo ;		// ����� ���� �ǹ̰� �ٸ���.
} ;


// en_gm_req_moveto_charac �� ��, : �����ϸ� CharacSetting���� �� ���̴�.
// ������ MSG_CTRL_CHARAC_SETTING::ctrl_ch_position, �����ϸ�, MSG_GM_RESULT
struct MSG_GM_REQ_NEXTTO_CHARAC : public MSG_GM_REQUEST
{
	char			cToCharac[13] ;		// ĳ���� �̸�.
} ;


// en_gm_req_charac_move,			// ����� �̵� ��Ŵ.
// gm���Դ� MSG_GM_RESULT::success, ĳ���Ϳ��Դ� MSG_CTRL_CHARAC_SETTING::ctrl_ch_position
// ���нÿ��� MSG_GM_RESULT::en_gm_result_charac_not_connect, or en_gm_result_charac_other_zone
struct MSG_GM_REQ_CHARAC_MOVE : public MSG_GM_REQUEST
{
	char			cTargetName[13] ;	// �ű� ��� �̸�.
	
	char			cServerNo ;			// � ������ �ű� ���ΰ�.
	float			fX ;				// ����.
	float			fZ ;				// 
} ;

// en_gm_req_npc_kill_sign
// �����ÿ��� �ֺ��� �� ��Ŷ�� �ٽ� ����, ���нÿ��� en_gm_result_npc_not_found�� ����. 
struct MSG_GM_REQ_NPC_KILL_SIGN : public MSG_GM_REQUEST
{
	u_char			ucType ;			// ���� 0 , ��Ÿ..
	u_short			usKillTarget ;		// ���� Ÿ��.
} ;


//en_gm_req_enter_warp,			// ��� �ڽ��� ���� �������� ����. : �̱���
struct MSG_GM_REQ_ENTER_WARP : public MSG_GM_REQUEST
{
	
} ;




//en_gm_req_charac_moveto_warp,	// ��� ĳ���͸� ���� �������� �鿩������. : �̱���
struct MSG_GM_REQ_CHARAC_MOVETO_WARP : public MSG_GM_REQUEST
{
} ;

enum EN_CTRL_SERVER_INFO
{
	en_server_info_pk_zone = 0,			// pk ���� ����. �������� pk ������ �ƴ�
};

struct MSG_SERVER_INFO
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;
};

struct MSG_SVRINFO_PK_ZONE : public MSG_SERVER_INFO
{
	u_char			ucPKZone;			// 0:normal  1:pkzone
};


//Author : �����	
#if defined(_ACCLAIM_VERSION) /*|| defined(_XVIETNAMESE)*/ || defined(_XJAPANESE) || defined(_XKOREAN)
	#define _XENABLE_GAMEGUARD
#endif

#ifdef _XUSEAPEXMODULE
	#undef _XENABLE_GAMEGUARD
#endif

#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG) && !defined(_XGMCLIENT)
	#ifdef _XENABLE_GAMEGUARD
		#define _XUSE_GAMEGUARD
		#include "NPGameLib.h"
		#pragma message( "Include NPGameLib.h : Ctrl_GMS.h" )
		extern CNPGameLib	g_GameGuardInstance;
	#endif
#endif

#ifdef _XUSE_GAMEGUARD

#define MSG_NO_GGAUTH_REQ       158
#define MSG_NO_GGAUTH_ACK       158

typedef struct MSG_GGAUTH 
{
	_sHeader	header;
	u_char	ucMessage;
	GG_AUTH_DATA	data;
} MSG_GGAUTH_REQ, MSG_GGAUTH_ACK;

#endif
//Last updated: 07/01/18


/** @struct NEXON_PCB_INFO
 	@breif �ؽ� �Ǿ��� ���� ����
 */
struct NEXON_PCB_INFO {
	UCHAR   ucResult; 

/*!
@retval 0 Error	- �ؽ����κ��� ������� ���� ������
@retval I ü���� - Internet
@retval P ������ - Ratio PC
@retval F �����Ǿ��� - Flat Cafe
@retval R �����Ǿ��� - Ratio Cafe
@retval M �����Ǿ��� - Prepaid
@retval C Address - Coupon Cafe Coupon
*/
	UCHAR   ucAddrDesc; 
	UCHAR   ucAccDesc; 
	UCHAR   ucOption; 

	int	iArgument; 
	bool	bIsPCBBillType; 
};


#define MSG_NO_COMMOM_REQ				67	/*!< */
#define MSG_NO_COMMON_ACK				67	/*!< */
#define MSG_NO_COMMON_NTF				67	/*!< */

/*
0: GS2C_�Ǿ�������_NTF: ������ �ʵ� ����� �Ǿ��� �������� �ƴ���
1: GS2C_�����������ð�����ȭ_NTF: 10�� ���� ������ Ŭ���̾�Ʈ ���� ������ ���� �ð��� ����ȭ �Ѵ�.
2: DS2GS_NTF   [������ �Ǿ��� ����]
*/

typedef struct MSG_COMMON 
{
	_sHeader               header;
	u_char                 ucMessage;
	WORD                   wMode;

	union {
		BYTE  data[1024];
		
		// wMode = 0, 2
		struct NXPCB_INFO 
		{
			char	acc[en_max_lil+1];
			NEXON_PCB_INFO info; // �Ǿ��� ����
			BYTE	 dummy[1024-(en_max_lil+1)-sizeof(NEXON_PCB_INFO)];
		} NXPCB_Info;
		
		// wMode = 1
		struct NXPCB_TOUCH_ATTACK_REMAIN_MIN
		{
			WORD wRemainMin; // ������ ���� �ð� ��(min) ������ ����
			BYTE dummy[1024-2];
		} NXPCBTouchAttackRemainMin;
		
		// wMode = 10
		struct _CIS_CASH_INVEN_REQ {
			bool	bIsPresent;		// 0: �Ϲ� ������, 1: ���� ������
			WORD	wPageNum;		// ������ ��ȣ
			WORD	wNumPerPage;	// ������ �� �ڷ��
		} CIS_CASH_INVEN_REQ;
		
		// wMode = 11
		struct _CIS_CASH_INVEN_ACK {			
			USHORT  usTotalCnt;     // ĳ���κ��� (��� �������� ������) �� ������ ����; �� �������� �������� ����ϱ� ����
			UCHAR	ucRowCount;		// �������� ������ �� ������ ����
			// ucRowCount ��ŭ �ݺ�
			int		iPkgGuid;	// �������� Guid
			int		iPkgIdx;	// ��Ű�� �ε���
		} CIS_CASH_INVEN_ACK;
		
		// wMode = 15
		struct _CIS_MOVE_ITEM_REQ {
			UCHAR	ucPkgCnt;	// �̵��� ������ ����
			// ucPkgCnt ��ŭ �ݺ�
			int		iPkgGuid;	// �������� Guid
			int		iPkgIdx;	// ��Ű�� �ε���

		} CIS_MOVE_ITEM_REQ;
		
		// wMode = 16
		struct _CIS_MOVE_ITEM_ACK {
			UCHAR	ucPkgCnt;	// �ű�⿡ ������ ��Ű�� ������ ����
			// ucPkgCnt ��ŭ �ݺ�
			int		iPkgGuid;	// �������� Guid
			
			UCHAR	ucItemCnt;	// �೶���� �Ű��� ������ ����
			// ucItemCnt ��ŭ �ݺ�
//			char	cSlot;		// �೶�� ���� �ε���
//			char	cType;		// ������ �з�
//			short	sId;
//			UCHAR	ucItemNum;	// �������� ������ ����
			_sUser_Inven_v1_all	Item;
		} CIS_MOVE_ITEM_ACK;
		
		// wMode = 20
		struct _CIS_PRESENT_MSG_REQ {
			int		iPkgGuid;	// �������� Guid
		} CIS_PRESENT_MSG_REQ;
		
		// wMode = 21
		struct _CIS_PRESENT_MSG_ACK {
			char		acc[en_max_lil+1];	// ������ ������ Account
			SYSTEMTIME	time;				// ���� ���� ��¥(��/��/��/��/��/��); DBTIMESTAMP
			char		msg[100+1];			// ���� �޽���
		} CIS_PRESENT_MSG_ACK;
	};
} MSG_COMMON, MSG_COMMON_REQ, MSG_COMMON_ACK, MSG_COMMON_NTF;


#ifdef _XUSEAPEXMODULE

#define MSG_NO_SECURITY                                    (49)                                  // ���� ����� ��Ŷ ���ۿ� ���δ�. 

struct MSG_SECURITY 
{
	_sHeader                         header;
	u_char                             ucMessage;                    //
	u_char                             ucMode;                                       //  �� ���� ��⺰�� �и��ؼ� ���. �⺻. 10����. 25���� ����� ������ �� �ְ� �Ѵ�.
	
	MSG_SECURITY( u_short size, u_char mode )
			: header(size), ucMessage(MSG_NO_SECURITY), ucMode(mode)
	{
	}
};

 

enum ENUM_SECURITY_COM_MODE										// MSG_SECURITY::ucMode 
{
     // 0 - 9 : M-Etel Apex Module
     en_apex_client_login=0,									// Ŭ���̾�Ʈ�� Apex ���� ������ �˷��ش�.
	 en_apex_client_login_result,								// CHCStart �� ���ϰ���  �޴´�.
     en_apex_req_client_data,									// 1 �������� Ŭ���̾�Ʈ��.
     en_apex_client_data,										// 2 Ŭ���̾�Ʈ���� ����
	 en_security_mode_none = 255                   // �̰�, case �� ������ �׳� �߰��� ��.
};

#define APEX_MAX_LENGTH							(2048)
#define APEX_MAX_PACKETLENGTH					2048

// 0 - 9 : metel apex 
// en_apex_client_login=0,                         // Ŭ���̾�Ʈ�� Apex ���� ������ �˷��ش�.
struct MSG_APEX_CLIENT_LOGIN : public MSG_SECURITY
{
	MSG_APEX_CLIENT_LOGIN() : MSG_SECURITY( sizeof(MSG_APEX_CLIENT_LOGIN), en_apex_client_login )
	{
	}
};

struct MSG_APEX_CLIENT_LOGIN_RESULT : public MSG_SECURITY
{
	long                    ret_value;                         // CHCStart �� ���ϰ�.
	MSG_APEX_CLIENT_LOGIN_RESULT()
        : MSG_SECURITY( sizeof(MSG_APEX_CLIENT_LOGIN_RESULT), en_apex_client_login_result )
    {
    }
};



// en_apex_req_client_data,                      // 1 �������� Ŭ���̾�Ʈ��.
struct MSG_APEX_REQ_CLIENT_DATA : public MSG_SECURITY
{
	u_short               data_length;                                               // 
	char                   data[APEX_MAX_LENGTH];                       // APEX_MAX_LENGTH ���� ª�Ƶ� �ڸ��� �ʽ��ϴ�. 

	MSG_APEX_REQ_CLIENT_DATA() : MSG_SECURITY( sizeof(MSG_APEX_REQ_CLIENT_DATA), en_apex_req_client_data )
	{
		ZeroMemory( data, sizeof(data) );
	}
}; 

// en_apex_client_data,                             // 2 Ŭ���̾�Ʈ���� ������.
struct MSG_APEX_CLIENT : public MSG_SECURITY
{
	u_short               data_length;
	char                   data[APEX_MAX_LENGTH];

	MSG_APEX_CLIENT() : MSG_SECURITY( sizeof(MSG_APEX_CLIENT), en_apex_client_data )
	{
		ZeroMemory( data, sizeof(data) );
	}
};

#endif

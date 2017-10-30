#pragma  once

// Log ����ü ����..

struct  _ServerState
{	
	TCHAR m_server_name[21];
	SHORT m_state;
};


//EVENT CODE
#define _LOG_ITEM_DNPC_DROP_	      (1)
#define _LOG_ITEM_PC_DROP_             15
#define _LOG_ITEM_PC_GRAP_             16  // �ϴ� ����Ʈ ������ �ޱ⿡�� ����. 
#define _LOG_ITEM_PVP_TRADE_           17
#define _LOG_ITEM_BUY_FROM_NPC_        18  // �������� ������ ���. 
#define _LOG_ITEM_SELL_TO_NPC_         19  // �������� ������ �Ǵ�. 
#define _LOG_ITEM_SELL_CLAN_GIVE_      20  // ���ı�� ������ 
#define _LOG_ITEM_CLAN_GIVE_MONEY_     21  // ���� ��� �� .
#define _LOG_ITEM_QUEST_GIVE_          22  // ����Ʈ���� ������ ��. 
#define _LOG_ITEM_QUSET_REMOVE_        23  // ����Ʈ���� ������ ����. 
#define _LOG_ITEM_MONEY_INVAILD_       24  // ���� �α׾ƿ��ÿ� �׼��� ��� ����ġ�� �Ѿ���� �α׸� �����. 
#define _LOG_ITEM_BREAK_ITEM_          25  // �������� ������. �ǻ� Į
#define _LOG_ITEM_SAVE_STORAGE_        26  // â�� �ִ�.
#define _LOG_ITEM_TAKEOUT_STORAGE_     27  // â���� ������.
#define _LOG_ITEM_SOCKET_INSERT_       28  // ���� �����ֱ�.
#define _LOG_ITEM_CLAN_GIVE_KICKUSER_  29  // ���� �⿩���� �ҹ����� �õ� Ȥ�� �߸��� �����ͷ� ���Ͽ� ������ ű��.
#define _LOG_ITEM_STORAGE_KICKUSER_    30  // â�� ������ �ҹ����� �õ� Ȥ�� �߸��� �����ͷ� ���� ������ ű��.
#define _LOG_ITEM_PSSHOP_TRADE_        31  // ���λ������� ��ǰ �Ǹű��. 
#define _LOG_ITEM_SAVEMONEY_STORAGE    32  // â�� ���� �Ա���. 
#define _LOG_ITEM_TAKEOUTMONEY_STORAGE 33  // â���� ���� �����. 
#define _LOG_ITEM_BUYSTORAGE_          34  // â�� ��� �ÿ� �� ��.
#define _LOG_ITEM_GAMBLE_              35  // �׺�α�.
#define _LOG_ITEM_ELIXIR_              36  // ����α�.

#ifdef ITEM_V13
#define _LOG_ITEM_BOXGAMBLE_           37  // �ڽ� �׺� �α�.
#endif

#define _LOG_ITEM_ADD_FROM_GMS_SUCC		40	// GMS�κ��� ������ ����
#define _LOG_ITEM_ADD_FROM_GMS_FAIL		41	// GMS�κ��� ������ ����
#define _LOG_ITEM_REM_FROM_GMS			42	// GMS�� ���� ������ ����

#define _LOG_ITEM_DS_UPDATE_MONEY_		43	// DS ���� ���� ������Ʈ�Ѵ�.

#define _LOG_ITEM_ADD_FROM_EVENT_SUCC	45	// event�� ���� ������ ���� ����(�̴Ͻ�ž)
#define _LOG_ITEM_ADD_FROM_EVENT_FAIL	45	// event�� ���� ������ ���� ����(�̴Ͻ�ž)

#define _LOG_ITEM_INCHANT_RESULT_		50	// ���� ���.
#define _LOG_ITEM_INCHANT_SUCCESS_		51	// ���� ���� 
#define _LOG_ITEM_INCHANT_FAIL_			52	// ���� ����
#define _LOG_ITEM_INCHANT_REMOVE_		53	// �������� ���� ������ ����.

#define _LOG_ITEM_RESOURCE_REMOVE		61
#define _LOG_ITEM_COLLECT_GET			62
#define _LOG_ITEM_COLLECT_REMOVE		63
#define _LOG_ITEM_RESOURCE_GET			64

#define _LOG_ITEM_KILLMONSTER			71	//���͸� �׿��ٿ�(�� ���� ��� ItemLog�� ��)

#define _LOG_ITEM_PK_RESULT_			72	// pk ����� ����.

#define _LOG_ITEM_NAME_CHANGE_			73	// ĳ���� �̸� ���� �α� : from -> to.

#define _LOG_ITEM_TAX_ADD_				80	// �ŵֵ��� ����/�������� �� : DS
#define _LOG_ITEM_TAX_TAKE_				81	// ������ ���� �ݾ�

struct _ItemLog
{
	unsigned __int64          item_uid ;
	SHORT			 code ;		//EVENT CODE
	SHORT			 zone ;
	int				 from_uid ;
	TCHAR			 from[en_charac_name_length+1] ;
	int				 to_uid ;
	TCHAR			 to[en_charac_name_length+1] ;
	UINT			 money  ;
	SHORT			 type1  ;
	SHORT			 type2  ;
	union 
	{		
		struct Def_Item
		{
			_sServer_InvenItem	Item;
			char				padding[9];
		} _def_Item;	
		//BYTE	moreInfo[ sizeof(_sServer_InvenItem) + 9 ] ;
		struct charac_data
		{
			short sRealStrength;
			short sRealZen;
			short sRealintelligence;
			short sRealConstitution;
			short sRealDexterity;
			short sElixirStrength;
			short sElixirZen;
			short sElixirintelligence;
			short sElixirConstitution;
			short sElixirDexterity;
			char  cPadding[10];
		} chData;
	}Item_or_Byte;
};



// EVENT CODE
#define _LOG_CHARAC_CREATE		1
#define _LOG_CHARAC_DELETE		2

#define _LOG_CHARAC_LOGIN_		11
#define _LOG_CHARAC_LOGOUT_		12
#define _LOG_CHARAC_ENTER_		13
#define _LOG_CHARAC_LEAVE_		14

#define _LOG_CHARAC_LEVEL_UP_		20		// ĳ���� ���� ��.
#define _LOG_CHARAC_BINSA_			21		// ĳ���Ͱ� ��� ���·�
#define _LOG_CHARAC_DEAD_			22		// ĳ���� ���.
#define _LOG_CHARAC_EXP_RECORD_		23		// ����ġ ����.
#define _LOG_CHARAC_LEVEL_UP_FAST_	24		// �������� �ſ� ���� ��
#define _LOG_CHARAC_QUEST_          25		// ����Ʈ�� ��������
#define _LOG_CHARAC_QUESTEXP_		26		// ����Ʈ�� ���� ����ġ�� ����
#define _LOG_CHARAC_RESET_STAT_		27		// ���� �ʱ�ȭ.�α�
#define _LOG_CHARAC_CLASSCHANGE_	28		// ���� ���� �α�

#define _LOG_CHARAC_HONOR_			30		// �� ��ȭ/����
#define _LOG_CHARAC_FAME_			31		// �Ǹ� ��ȭ/����

#define _LOG_CHARAC_SEQUENCE_ERROR_		40	// ��Ŷ ������ ��ȣ ����.

#define _LOG_SKILL_ADD_				50		// ��ų�� �߰� �α�
#define _LOG_SKILL_STEP_UP_			51		// ��ų�� ��� �α�
#define _LOG_SKILL_DEL_NORMAL_		52		// ���� ����.
#define _LOG_SKILL_DEL_CLASS_		53		// ���� Ŭ���������� ������.
#define _LOG_SKILL_DEL_NOREF_		54		// ��ų ���̺� ��� ������.
#define _LOG_SKILL_NOTIFY_CLASS_	55		// ��ų ���̺� ��� ������.

// ������ ����
#define _LOG_OR_CREATE_				70		// ������ â��
#define _LOG_OR_DESTROY				71		// ������ ��ü
#define _LOG_OR_JOIN_				72		// ������ ����
#define _LOG_OR_OUT_				73		// ������ Ż��
#define _LOG_OR_ENTRUST_			74		// ���� ����

// ��ȣ ����
#define _LOG_NIC_CREATE_			80      // �г��� �ֱ�
#define _LOG_NICK_DELETE_REQ_		81		//	��ȣ ���� ��û
#define _LOG_NICK_DELETE_			82		//	��ȣ ����


// �̺�Ʈ ����
#define _LOG_EVENT_RESULT_			90		// ���� �̺�Ʈ�� ���.

#define _LOG_EVENT_BOSS_ROOM		100		// BOSS ROOM



struct _CharacLog {
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
	char	cAccount[en_max_lil +1] ;
#else
	char	account[13] ;
#endif
	int		charac_uid ;
	char	charac[en_charac_name_length+1] ;
	short	code ;
	short	zone ;
	short	level ;
	short	point ;
	char	ip[17] ;
	BYTE	moreInfo[15] ;
};



#define	_LOG_TYPE_LOGIN			0
struct _LoginLog {
	int			acc_uid ;
#ifdef _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_
	char		cAccount[en_max_lil +1] ;
	char		cCharac[en_charac_name_length+1] ;
#else
	char		acc[12] ;
	char		charac[en_charac_name_length] ;
#endif
	short		zone_no ;
	int			inner_level	;
	short		party ;
	short		m_class ;
	char		ip[15] ;
	char		pe_pay_code[2] ;	// Bill Method
	char		pc_pay_code[2] ;	// pc�� ����
	char		pc_uid[20] ;
	char		login_time[20] ;
	char		logout_time[20] ;
	int			play_time ;
	int			type ;
};
#pragma once

#pragma pack( push, enter_game )
#pragma pack(1)



/*==========================================================================================]
[				GS MSG ��Ŷ MESSAGE ��ȣ													]
[===========================================================================================]
[	���� : DS->GS  ��Ŷ�� U �� �����Ѵ�..													]
[			GSM ���� �޽��� ��ȣ�� �ߺ��� �����ϱ� ���� GMS->GS �޽��� ��ȣ�� 100 ������	]
[			�����մϴ�. UMSG_SHOWTEXT_REQ_DS_GS(��������) �޽����� ���ؼ� ��� Result ��	]
[			������ ���� ������ ���߽��ϴ�.(�ȹ޾Ƶ� �ɲ����Ƽ�..) ���� ���� �����簡	]
[			GS ���� ��û�� ���� MSG �� � �� �߰��� �����Դϴ�. ���߿� �߰��ȰǸ� ����	]
[			��Ƽ� �ٽ� �����帮�ڽ��ϴ�.	Ȯ���Ͻð� �ǵ�� �ּ���.						]
[											jkHong Add...									]
[==========================================================================================*/

//[to GS ����]===============================================================================================

// DS -> GS
#define		UMSG_NO_GSINFO_REQ_DS_GS		100	// GS�� ���� ���� ��û
#define		UMSG_NO_GSINFO_RES_GS_DS		101	// ���� ���� ���� *
#define		UMSG_NO_USER_LOGOUT_DS_GS		102	// ���� �α׾ƿ�
#define		UMSG_SHOWTEXT_REQ_DS_GS			103	// GS�� �������� ��� ��û
#define		UMSG_SHOWTEXT_RES_GS_DS			104	// �������� ��� ��� *
#define		UMSG_SYSTEM_OFF_REQ_NUM			110	// �ý��� ���� DS -> GS

#define		UMSG_NO_SETPOSITION_REQ			119	// ĳ���� ��ġ �ʱ�ȭ


//#define UMSG_NO_GSINFO_REQ_DS_GS				100	// �� �ֱ����� ��û ��Ŷ�̴�..
struct UMSG_GSINFO_REQ_DS_GS					// # DS->GS GS�� ���� ������ ��û�Ѵ�.
{
	int					iKey;					//��ȿ Ȯ�� Ű
	char				cMessage;				//�޼��� ��ȣ		
	char				cServerNum;				//���� ��ȣ(��� ���� ��Ģ�� ���߱� ����..)
};


//#define UMSG_NO_GSINFO_RES_GS_DS				101	//�ֱ��� ����, �Ǵ� �� �ֱ��� ��û�� ���� ����
struct UMSG_GSINFO_RES_GS_DS					// # GS->DS GS�� ���� ������ DS �� �˸���(UPDATE)
{
	int					iKey;					//��ȿ Ȯ�� Ű
	char				cMessage;				// MSG_NO_GSINFO_RES_GS_DS(5)
	char				cServerNum;

	char				cState;					// ������ ����
	int					iConnectCount;			//���� ����
	int					iPlayingCount;			//�������� ����

	struct  _ServerInfo{						// ���� ����		
		char		cServerName[13];
		char		cOS[16];					// ��� OS
		char		cIP[16];					// ��� IP
		char		cCPU;						// CPU ����
		int			iTotalMem;					// �� �޸�
		int			iUseMem;					// ������� �޸�
		int			iTotalHDD;					// �� ��ũ �뷮
		int			iUseHDD;					// ������� ��ũ �뷮
	} SVR_INFO;
};

// #define		UMSG_NO_USER_LOGOUT_DS_GS		102		// ���� �α׾ƿ�
struct UMSG_USER_LOGOUT_DS_GS
{
	int					iKey;					//��ȿ Ȯ�� Ű
	char				cMessage;				//�޼��� ��ȣ	
	char				cServerNum;				//������ȣ (������� ��Ģ�� ���߱� ����..)

	char				cAccount[en_max_lil+1];			//����� ����
	char				cReason;				//����

	// �� ��Ŷ�� ���������� uid�� �������� �ʾƵ� ��.
	// 0 : �ٸ� ������ �α���
	// 1 : ��ڿ����� �α׾ƿ�
	// 2 : ����( ����... )	
};


// #define		UMSG_SHOWTEXT_REQ_DS_GS			103	// GS�� �������� ��� ��û
struct UMSG_SHOWTEXT_REQ{
	int			iKey;
	char		cMessage;	
	char		cServerNum;

	char		cString[51];
};

// #define		UMSG_SHOWTEXT_RES_GS_DS			104	// �������� ��� ���
struct UMSG_SHOWTEXT_RES{
	int			iKey;
	char		cMessage;	
	char		cServerNum;

	char		cResult;
};

// #define		MSG_SYSTEM_OFF_REQ_NUM			110	// �ý��� ���� DS -> GS
struct UMSG_SYSTEM_OFF_REQ {
	int		iKey;
	char	cMessage;
	char	cServer_num;	
};

struct UMSG_SETPOSITION_REQ{
	int			iKey;
	char		cMessage;
	char		cServerNum;
	char		cCharacName[13];	// ĳ���� �̸�(�������� �ؾ��ϳ�? ������� �ǵ�� �ּ���.)
};





/*==========================================================================================]
[				LS MSG ��Ŷ MESSAGE ��ȣ													]
[===========================================================================================]
[	���� : DS<->LS  ��Ŷ�� U �� �����Ѵ�..													]
[			�ٸ� UDP ���� �޽��� ��ȣ�� �ߺ��� �����ϱ� ���� LS->DS �޽��� ��ȣ�� 200 ������]
[			�����մϴ�.																		]
[											jkHong Add...									]
[==========================================================================================*/
#define		UMSG_LOGIN_LS_DS_NUM				121
#define		UMSG_LOGIN_DS_LS_RESULT_NUM		122
#define		UMSG_LOGIN_INFO_REQ_LS_DS_NUM	123
#define		UMSG_LOGIN_INFO_RES_DS_LS_NUM	124
#define		MSG_CMD_USERKILL_NUM			31	// ����� ���� ����

// #define UMSG_LOGIN_DS_NUM	121
struct UMSG_LOGIN_LS_DS						//# LS->DS �α� ��û #
{
	int			iKey;
	char		cMessage ;				//	CMSG_NO_LOGIN_LS_DS(0)
	char		cServerNum ;

	char		cAccount[en_max_lil+1];			//	���� ����
#ifdef _NXPCB_AWARD_200703_
	char		cNXAcc[en_max_lil+1];			//	�ؽ� ����
#endif
	char		cPass[en_max_lpl+1];				// password
	int			iAccUid ;				// 
	int			iTempID ;				//  LS���� �ο��� UID
	u_int		iIp;					//	��������  IP
	int			iTime;					//	���� �õ� �ð�
	char		cClass ;
	
#ifdef _PD_VINA_PCROOM_INFO_
	u_char		ucPCRoomInfo;			// 0 - None, 1 ~. PC Bang grade( use 1 only )
#endif
	
#ifdef _PD_COMPANY_METEL_
	bool		bPCBang;				// PCBang User or not?
	METEL_BILLING_INFO	MetelBillingInfo;
#endif //_METEL_LOGIN_200608_
};


// �α��� ��� ��Ŷ
// #define UMSG_LOGIN_DS_LS_RESULT_NUM				122		// DS->LS LOGIN RESPONSE MESSAGE SUCCESS
// ���� ��ȣ
#define _ERRNO_USING_ACCOUNT_					-2
#define _ERRNO_DB_ERROR_						-3
#define _ERRNO_STATE_ERROR_						-4
#define _ERRNO_IS_FULL_							-5
struct UMSG_LOGIN_DS_LS_RESULT
{
	int			iKey ;
	char		cMessage;				// 1
	char		cForestNo ;				// Forest Index number

	char		cAccount[en_max_lil+1];			//	����
	u_short		usUid;					//	DS ���� �ο����� uid
	int			iTempID ;				//  LS���� �ο��� UID
	// �õ��� �� �� �ֱ� ������, ���������δ� ������ �� ����. 

	char		cServerNo ;				// ������ ���( sServerNo > 0 )�� ������ ��ȣ�� 
	// ������ ��쿡�� ���� ��ȣ�� ����. 
	// 0���� -1�� ���Ӽ��������� ����.
	// -2 : ������� ����
	// -3 : DB ����
	// -4 : ���� ����/�׳� ���з� ������.
};


//#define		UMSG_LOGIN_INFO_REQ_LS_DS_NUM	123
struct UMSG_LOGIN_INFO_REQ_LS_DS
{
	int			iKey ;
	char		cMessage ;
	char		cServerNum ;
};


//#define		UMSG_LOGIN_INFO_RES_DS_LS_NUM	124
struct UMSG_LOGIN_INFO_RES_DS_LS
{
	int			iKey ;
	char		cMessage ;
	char		cForestNum ;

	char		cForestState ;	
		//#define  _FOREST_STATE_RUNNING = 0			// ���� �۵���
		//#define  _FOREST_STATE_ISFULL = -1			// ���� ��ȭ����
		//#define  _FOREST_STATE_CHECKUP_ = -2			// ������ ������
};


struct _gmsHeader {
	int			iKey;					//��ȿ Ȯ�� Ű
	char		cMessage;				// MSG_NO_GSINFO_RES_GS_DS(5)
	u_int		uiTime;
	char		cGMName[13];
};


//#define		MSG_CMD_USERKILL				31	// ����� ���� ����
struct MSG_CMD_USERKILL {
	_gmsHeader	header;
	char		cAccount[en_max_lil+1];			//	����
};


/*==========================================================================================]
[				PING MESSAGE PACKET															]
[===========================================================================================]
[	���� : ������ ���� ���θ� �Ǵ��ϱ� ����.. ��� ������ ���̴� PING PACKET �Դϴ�. 		]
[											jkHong Add...									]
[==========================================================================================*/
#define		UMSG_NO_PING			0 		// ������ ����Ŷ�� ������ �ٷ� �������ֱ⸸ �ϸ� �ȴ�.
struct	UMSG_PING
{
	int			iKey ;
	char		cMessage ;
	int			iIndex ;					//	���� ���� (��Ŷ�� �����ϱ� ���ؼ�)
	DWORD		time ;						//	= GetTickCount() , ��Ŷ�� ���� �ð� (�̰����� ����� �ӵ��� ���Ѵ�.)
};


#pragma pack( pop, enter_game )
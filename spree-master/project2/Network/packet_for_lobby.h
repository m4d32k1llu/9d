#pragma once

struct _sHeader;
struct _CharacList;

// Lobby
// Main = 1
// 8 - 15

// CHARAC_ĳ���� ������ �κ� 8 - 15 ++ ����� ��Ŷ.
#define MSG_NO_CHARAC_REQ_LIST			8		// ĳ���� ����Ʈ�� �䱸�Ѵ�. C->S
#define MSG_NO_CHARAC_LIST				9		// ĳ���� ����Ʈ �䱸�� ���ϴ� ��Ŷ.

#define MSG_NO_SHIFT_STATE				MSG_NO_CHARAC_REQ_LIST	// ��������(ex: ��������)�� ����Ǿ��ٴ°��� �˸��� ��Ŷ. S->C

#define MSG_NO_CHARAC_SELECT            10      // ĳ���� â���� ĳ���͸� �����ߴ�.
#define MSG_NO_CHARAC_CREATE            11		// ĳ���� ����
#define MSG_NO_CHARAC_DELETE			12		// ĳ���� ����
#define MSG_NO_CHARAC_RESULT			13		// ĳ���� ����/���� �� ���

// ���� ���� ����
#define MSG_NO_BILL_INFO				15		// ���� ���� ����

//#define MSG_NO_CHARAC_REQ_LIST		8 
struct MSG_CHARAC_REQ_LIST
{
	_sHeader			header;
	u_char				ucMessage;
};

//#define MSG_NO_SHIFT_STATE				MSG_NO_CHARAC_REQ_LIST	// ��������(ex: ��������)�� ����Ǿ��ٴ°��� �˸��� ��Ŷ. S->C
struct MSG_SHIFT_STATE
{
	_sHeader			header;
	u_char				ucMessage;
	
	u_int				uiStateFlag;		//�������� 32bit��������(1: ��������)
};

//#define MSG_NO_CHARAC_LIST			9
struct MSG_CHARAC_LIST
{
	_sHeader			header;
	u_char				ucMessage;

	char				nCharac;						// ĳ���� ����
	_CharacList			List[3];						// ĳ���� ����
	u_int				uiStateFlag;					//�������� 32bit��������(1: ��������)
	
#ifdef _XDEF_SHUTDOWN_20061027	
	short               sFatigueWeight ;				//��Ʈ�� shutdown �Ƿε�(����: ��)	
#endif

#ifdef _XVNBILLINGSYSTEM	// ������� ����
	char				cBillMethod;					// 1: ȸ������ �� 2�� 24�ϰ� ����
														// 2: �̺�Ʈ ����
														// 3: �����Թ� ����
														// 4: �����Թ� ����
														// 5: ������� ����
														// 6: ������� ����
	
	DWORD				dwRemainTime;					// cBillMethod�� 1,4,6�� ��� = �����ð�(sec)
														// 3,5�� ��� = Zero
	
	char				szExpireDate[12+1];				// 1,3,5�� ��� = YYYYMMDDHHMM
#endif	
	
#if defined(_XDEF_VINA_PCROOM_INFO) || defined(_XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI)
	u_int				ucPCRoomInfo : 4;
	u_int				uiSDSMode : 4;
	u_int				uiReserve : 24;
#endif

};

//#define MSG_NO_CHARAC_SELECT			10
struct MSG_CHARAC_SELECT
{
	_sHeader			header;
	u_char				ucMessage;

	char				cSelectName[13];		//ĳ���� �̸�
};

//#define MSG_NO_CHARAC_CREATE			11

struct MSG_CHARAC_CREATE 
{ 
	_sHeader			header ; 
	u_char				ucMessage ; 

	char				cName[13] ;				// ĳ������ �̸� 
	
	// ���ġ 
	char				cConstitution ;			// �ǰ� : 5�⸦ ��ġ�� �޴´�. ( 8~18 �����̸�, �հ�� �ݵ�� 50�̾�� �Ѵ�. 
	char				cZen ;					// ���� : �￩���� ���ġ�� ����. 
	char				cIntelligence ;			// ���� 
	char				cDexterity ;			// ��ø 
	char				cStr ;					// �ٷ�
	
	char				cSex ;					// ���� 0 : ����, 1 : ���� 
	char				cFace ;					// ���� : max ������ ���� ����. 
	char				cHair ;					// �Ӹ� ��Ÿ�� : max ������ �������. 
	
	char				cInitZone;				// ó�� ��ġ�� ���� : ĳ���͸� ���鶧 ���´�. 
	
	u_char				ucClothSet ;			// ��Ʈ ��ȣ : 0, 1, 2 : 0�� �ƹ��͵� ��ġ�� ����.
	/* 2005/01/08 ���� : ������ ĳ���� ��Ʈ�� ��.
	short				sAmor ;					// ���� : 2�� �ۿ� ���µ� �Ѥ�
	short				sPants ;				// ���� : 2�� �ۿ� ����. 
	short				sShoes ;				// �Ź� :
	*/
} ;

//#define MSG_NO_CHARAC_DELETE			12
struct MSG_CHARAC_DELETE
{
	_sHeader			header;
	u_char				ucMessage;

	char				cName[13];				// ĳ���� �̸�
	char				cPassword[13];			// ��й�ȣ
};

//#define MSG_NO_CHARAC_RESULT			13
struct MSG_CHARAC_RESULT
{
	_sHeader			header;
	u_char				ucMessage;

	unsigned char		ucPrevCommand;			// � �޼����� ����ΰ�
	unsigned char		ucResult;				// 0�̸� ����, 1�̸� ���� ����

	// ucPrevCommand : ĳ���� ������ ���� �����̸�, �� ���� MSG_NO_CHARAC_CREATE �̰�, 
	// �����̶��, ucResult �� 0�� �ɰ��̰�, �ƴϸ�, ���� ������ �ɰ��̴�.
	// MSG_CHARAC_CREATE �� ���� ������� 
	//   1 : �̹� �ִ� ����.  2: ����� �� ���� �̸�. 3: ���ġ ����  4: �⺻ ������ ���� 5: ���̻� ����� ����.
	// MSG_CHARAC_DELETE �� ���� ��������
	//   10 : ������ �� ����.
	// MSG_CHARAC_SELECT �� ���� �������� 
	//   20 : ĳ���͸� ������ �� ����.
	//   21 : �ش缭�� OFF
};

//#define MSG_NO_BILL_INFO				15		// ���� ���� ����

struct _BillInfo {
	union
	{
		struct {
			char			cWHO ;				// ���� ����
			char			cWhere ;			// ���� ���
			char			cETC ;				// == 0
		} ;
		char				cBillingStatus[3] ;
	};
	char				cBillPayType[2];		// ���ҹ��
	char				cBillMethod[2] ;		// ���ݹ��
	char				cBillExpire[12] ;		// ���� ������YYYYMMDD
	int					iBillRemain ;			// ���� �ð� �� ����Ʈ
	
	char				cPCRoomEndDate[20] ;	// �ǽù� ���� ������ ���� (�տ�2����Ʈ�� ����, ������ �ڿ��� ��¥ ����)
    int					iAlertCode ;			// AlertMessage code
	char				cPedding[8] ;			// �����ʵ�
} ;

struct MSG_BILL_INFO
{
	_sHeader			header;
	u_char				ucMessage;
	
	_BillInfo			BillData;
} ;


// Packet_Result
#define _PACKET_RESULT_AUTH_OK_		1
#define _PACKET_RESULT_NONERROR_	0

// Result of alert ( _BillInfo::iAlertCode )
#define _PACKET_ALERT_LOGIN_INFO			 2	// �α��������� ��������
#define _PACKET_ALERT_REMAIN_TIME			 1	// ������� ���� �ð�
#define _PACKET_ALERT_NOT_ENOUGH_POINT		 0	// �����ð� ����(��������)
#define _PACKET_ALERT_REBATE				-1	// ȯ����ġ �Ǿ���(��������)
#define _PACKET_ALERT_DOUBLE_LOGIN			-2	// ���� �α���(��������)
#define _PACKET_ALERT_RENEW					-3	// ����Ǿ� �ִ� ����ǰ���� ��ü
#define _PACKET_ALERT_ETC_ERROR_KILL		-4	// ��Ÿ ����(���� ����)

/*
2005/07/22.   ozzywow 

--------------------------------------------------------------------------
MSG_BILL_INFO ��Ŷ�� ���� ���� : ������� ���� �����Դϴ�.
--------------------------------------------------------------------------

cBillingStatus ( ������� ���� ���� ) 
--------------------------------------------------------------------------
|     ��        |             ����                                       |
--------------------------------------------------------------------------
| HO			|	���λ����											 |
| PC			|	���ӹ� �����										 |
| SP			|	ISP �����											 |
--------------------------------------------------------------------------

cBillPayType ( ����(����) ��� ����)
--------------------------------------------------------------------------
|     ��        |             ����                                       |
--------------------------------------------------------------------------
| AO			|	�ĺ� ����											 |
| DO			|	����												 |
| FO			|	����												 |
| PO			|	���Ұ���(�Ϲ����� ��� ��� ����)					 |
--------------------------------------------------------------------------

cBillMethod ( ���� ��� ����)
--------------------------------------------------------------------------
|     ��        |             ����                                       |
--------------------------------------------------------------------------
| FM,FW, FD		|	������, �ָ� �� ����, ������						 |
| FD,DB,HB		|	������, �Ⱓ����, Day Block, Hour Block				 |
| TH			|	���� 												 |
| TB			|	����, ������� �ܾ׿��� �����ϴ°��� �ǹ�, Time Block|
| PM, PD		|	�÷�Ƽ�� ��(��) ����                                 |
| NO			|	���� �����(�ð� ����)                               |
--------------------------------------------------------------------------
*/

#define MSG_NO_BILLING_INFO_REQ		45
#define MSG_NO_BILLING_INFO_ACK		46

#define _VNBILLING_BILLMETHOD_FREEOFCHARGE					1	//ȸ�������� 2�� 24�ϰ� ����
#define _VNBILLING_BILLMETHOD_FREEOFCHARGE_EVENT			2	//�̺�Ʈ ����
#define _VNBILLING_BILLMETHOD_BASIC_FIXEDAMOUNTSYSTEM		3	//�����Թ� ����
#define _VNBILLING_BILLMETHOD_BASIC_FIXEDQUANTITYSYSTEM		4	//�����Թ� ����
#define _VNBILLING_BILLMETHOD_PREMIUM_FIXEDAMOUNTSYSTEM		5	//������� ����
#define _VNBILLING_BILLMETHOD_PREMIUM_FIXEDQUANTITYSYSTEM	6	//������� ����

//#ifdef _XVNBILLINGSYSTEM

struct MSG_BILLING_INFO_REQ 
{
	_sHeader		header;
	u_char			ucMessage;
};

struct MSG_BILLING_INFO_ACK 
{
	_sHeader		header;
	u_char			ucMessage;

	UCHAR			ucType; // 2: ��ȹ�� 5-3
							// 3: ��ȹ�� 5-4 & 5-5

	union 
	{
		// ucType = 1
		char	dummy[32];

		// ucType = 2
		struct	EXPIRE {
			char	szBillingInfoRecordTime[12+1]; // �������� ���� �ð�
			char	cBillMethod;
				// 1: ȸ�������� 2�� 24�ϰ� ����
				// 2: �̺�Ʈ ����
				// 3: �����Թ� ����
				// 4: �����Թ� ����
				// 5: ������� ����
				// 6: ������� ����
			DWORD	dwRemainTime;		// cBillMethod�� 1,4,6�� ��� = �����ð�(sec)
							// 3,5�� ��� = Zero
			char	szExpireDate[12+1];	// 1,3,5�� ��� = YYYYMMDDHHMM
			char	dummy;
		} Expire;

		// ucType = 3
		struct EXPIRE_COUNTDOWN {
			UCHAR	ucCount;	// 0 ~ 5
			char	dummy[31];
		} ExpireCountDown;

	}; //end of union
};

//#endif
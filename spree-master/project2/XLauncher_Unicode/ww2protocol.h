//WW2Protocol.h
//������ Ŭ���̾�Ʈ ������ ��� �������� 
 /*************************************************
  World War 2  Bettle Net Packet

  �ۼ����� ���� : 2001. 5. 18 (��)
  
	�޼��� �̸� ��Ģ
	 cCMSG_XXXXX
	 c : class
	  C : Client (S : Server) (���̼����� �޼����̸����� ���ܵ�)
	   MSG : (��� �޼����ϱ� ^^)
	      _XXXXX : �޼����� �̸�
	------------------------------------------------------------------------

 -  ���̼����� ���� �޼��� ���� �κ�
	(������ cMSG_DISDMS�޼����� ���� ������ ����Ʈ�� �޾� �����ϰ� cMSG_DISDMS�޼����� ���� �����Ѵ�.)
	cMSG_SEVLIST : ������ ����Ʈ�� �ϳ��� ������. Ŭ���̾�Ʈ��  cMSG_DISDMS �� �ö����� ��� recv�Ѵ�. 
	cMSG_DISDMS  : ���� ����Ʈ ������ ������ �˷��ش�.
 -  ��� ���� ǥ���� ��� 
	ó�� �α��� �Ҷ� �Ѳ����� �޴���, �ƴ� �� �������� ���� �޴³�..
	(�ʹݿ� �ѹ��� �޴°��� ^^)
  
 -	Ŭ���̾�Ʈ �޼��� ���� �κ� 
	cCMSG_LOGIN			:  (0x1001)�α��� ���� ����
	cCMSG_NEWID			:  (0x1002)���ο� �Ƶ� ��û
	cCMSG_CHGPW			:  (0x1003)�н����带 �ٲ۴�.
	cCMSG_CHAT			:  (0x1004)ä�� �޼���(�뺸)
	cCMSG_CREATEFIELD   :  (0x1005)ä���� �����.
	cCMSG_JOINFIELD     :  (0x1006)Ÿ ä�ο� �����Ѵ�. 
	cCMSG_LISTFIELD     :  (0x1007)ä�� ����Ʈ�� ��û�ϴ�. 
	cCMSG_CREATEBATTLE  :  (0x1008)�׹��� �����. 
	cCMSG_CREATERESULT  :  (0x1009)�׹��� ���������� ��������� Ȯ�����ش�.. (�뺸)
	cCMSG_CANCELBATTLE  :  (0x1010)�׹������� ���� �ǰڴ�. (�뺸)
	cCMSG_JOINBATTLE    :  (0x1011)�׹濡 �����Ѵ�.
	cCMSG_INFOBATTLE    :  (0x1012)������� �׹��� ������ �����Ѵ�. (�뺸)
	cCMSG_LAUNCHBATTLE  :  (0x1013)������ �����Ѵ�. 
	cCMSG_EXITBATTLE    :  (0x1014)�׹濡�� ���´�. (�뺸)(������.. ���������� ��� ä�η� ���� ���ؼ� �����ش�.)
	cCMSG_ENDBATTLE     :  (0x1015)�׹濡�� ���´�. (�뺸)(���߿� ���´�. ����� ���⿡ ����)
	(���������� �� �׹��� ��������, �״�� ������ �˰��ִ�.)
	cCMSG_LISTBATTLE    :  (0x1016)�׹渮��Ʈ�� �䱸�Ѵ�.
	cCMSG_LISTRANK		:  (0x1018)��ŷ�� �䱸�Ѵ�.
	cCMSG_FINDRANK		:  (0x1019)��ŷ����Ʈ���� ã�´�.
	cCMSG_REDBANNER		:  (0x1020)��ʸ� ���� �غ� ��ġ�� �������� ��û (cSMSG_TRANSFILE�� ���ƿ´�.)	
	cCMSG_LOGOUT		:  (0x1021)������ ������. (�뺸)
	cCMSG_JOINRESULT	:  (0x1022)�׿� ������ ����� �����ش�.(�Ƹ� ���� �����ϰ�����.. ���Ұ�쵵 ��������.. dp�� �󸶳� �����. -_-;)
	cCMSG_INIT			:  (0x1023)������ �������� ...
	********* ��Ÿ��� *****************************	
	cCMSG_FIND			:  (0x1051)�ٸ� ������ ã�´�.
	cCMSG_SAY			:  (0x1052)���濡�� �Ӹ��� �̾߱��Ѵ�.(�뺸 - �� �ϴ� ���, Ȥ�� �� �ݴ뵵 �����ϰ�... ^^)
	cCMSG_KICK			:  (0x1053)�׹��� ������ �ٸ� ����� �����Ҽ� �ִ�.(�뺸)
	cCMSG_PROFILE		:  (0x1054)������ ������ ��û�Ѵ�.
	cCMSG_INVITE		:  (0x1055)�׹����� ��û�Ѵ�. 

  
 -	���� �޼��� ���� �κ� 
	cSMSG_LOGIN			:  (0x2001)�α��� ��� ������
	cSMSG_NEWID			:  (0x2002)���ο� �Ƶ� ��û ���
	cSMSG_CHGPW			:  (0x2003)�н����� �ٲ� ����� �����ش�.
	cSMSG_CHAT			:  (0x2004)ä�� �޼���(�뺸)
	cSMSG_CREATEFIELD	:  (0x2005)ä�� ���� ����� �����ش�. 
	cSMSG_JOINFIELD		:  (0x2006)ä������ ����� �����ش�. (����/�뺸)
	cSMSG_UPDATEFIELD	:  (0x2007)ä���� ������ �����Ѵ�. (�뺸)
	cSMSG_LISTFIELD		:  (0x2008)ä�� ����Ʈ�� �ش�.
	cSMSG_CREATEBATTLE  :  (0x2009)�׹��� ��������� �����ش�.
	cSMSG_JOINBATTLE    :  (0x2010)�׹濡 ���ΰ���� �����ش�. 
	cSMSG_LISTBATTLE    :  (0x2011)�׹渮��Ʈ�� �����ش�.
	cSMSG_LISTRANK		:  (0x2012)��ŷ�� �����ش�.
	cSMSG_FINDRANK		:  (0x2013)��ŷ����Ʈ���� Ư�������� ������ �˻��ؼ� �����ش�. 
	cSMSG_INFOFIELD		:  (0x2014)20�� �߶� ���������� �����ش�.
	cSMSG_INIT			:  (0x2015)�ʱ� ������ Ȯ���Ѵ�.
	********* ��Ÿ��� *****************************
	cSMSG_FIND			:  (0x2051)ã�� ������ ����� �����ش�. 
	cSMSG_SAY			:  (0x2052)���濡�� �Ӹ������� �����Ѵ�. (�뺸)
	cSMSG_KICK			:  (0x2053)�����Ų��.(�뺸)
	cSMSG_PROFILE		:  (0x2054)��û�� ����� ������ �����ش�. 
	cSMSG_DIS           :  (0x2055)�����ڰ� ������ �����Ų��.(�Ⱓ�� ���Ұǰ�???)
	cSMSG_TRANSFILE		:  (0x2056)���� �����Ѵ�. 
	cSMSG_INVITE		:  (0x2057)��û �޼����� �����ش�. 


	�߰��۾� : 2001. 05. 26 (��)
	��    �� : ��.. ��Ÿ����� ������ ������ ��Ŷ�� �� �����ߴ�. 
			   �帲���� �Ѿ ���� �˰�����.. �ϴ��� �Ϸ�!!
			   �����ʹ� ������ ���� �ڵ��� ǫ �����ڱ�. --;

	�߰��۾� : 2001. 05. 29 (ȭ)
	��    �� : �Ʒ��� ������ �߰�, ����Ǿ���.(cSMSG_LOGIN�߿���...)	
		#define dLOGIN_NOTLOGIN		0x0002	//	ID Ȥ�� PW�� ��ġ���� �ʴ´�. 
		#define dLOGIN_FAIL			0x0008	//	�Ͽ��� �α��� �Ұ��� ^^;;

	�߰��۾� : 2001. 05. 30 (��)
	��    �� : 
		#define dDREATEFIELD_USEDNAME	0x0002	//	���̸��� �����Ѵ�.
		//	�ڽ��� ��޿� ���� ����.. updatefield,cSMSG_LISTRANK,cSMSG_FINDRANK�� ���Եȳ�����..
		BYTE	imclass;	//	����� �ѱ��. 
		dSMSG_INFOFIELD	�߰�..

	�߰��۾� : 2001. 05. 31 (��)
	��    �� : cCMSG_CREATEBATTLE����κ��� �����Ǿ���.
			   dLOGIN_FULL  �߰���.

	�߰��۾� : 2001. 06. 01 (��)
	��    �� : �ʿ���� �ø��� ���� ����.
			   cSMSG_LOGIN / cSMSG_CREATEFIELD / cSMSG_JOINFIELD / cSMSG_CREATEBATTLE / cSMSG_JOINBATTLE
			   cSMSG_CHAT�� char id[dMAXUSERIDSIZE]; �߰���.//
			   cCMSG_INIT, cSMSG_INIT �߰� login�� Ȯ���ϴ� ������ ��� �Ȱ��.

	�߰��۾� : 2001. 06. 02 (��)
	��    �� : ä�ø޼�����  fieldserial�� �����ߴ�. 
				joinfield�� fieldname �߰� 
				cCMSG_RESULTBATTLE ���� 
				cCMSG_ENDBATTLE�� last�߰� 

	�߰��۾� : 2001. 06. 04 (��)
	��    �� : �Ʒ��� ������ �߰�,����Ǿ���.
				#define	dJOINFIELD_DONTENER	0x0005	//	���� �ִ� ���°� �ƴϴ�.	
				#define dJOINFIELD_NOMORE	0X0006	//
				
2001.07.24 ȭ����
 ����: ����� ���ӹ��ı��� ȣ��Ʈ�� �̿ܿ��� ��� dEXITBATTLE�� ���� 
 *************************************************/

#ifndef _WW2PROTOCOL_H
#define _WW2PROTOCOL_H
#pragma pack(1) 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//CLASS ����
#define MODULE_TCPINIT				10
#define MODULE_TCPSERVICE			20
#define MODULE_TCPTHREAD			30

#define SendQueueSize				(8192)
#define WM_DIALOG					WM_USER+1
#define WM_CHATLIST					WM_USER+2
#define	WM_JOINFIELD				WM_USER+3
#define	WM_CREATEFIELD				WM_USER+4
#define WM_NEWID					WM_USER+5
#define WM_CHANGEPW					WM_USER+6
#define WM_CREATEBATTLE				WM_USER+7
#define WM_USERCHATUSERS			WM_USER+8
#define WM_BATTLELIST				WM_USER+9
#define WM_BATTLELISTPAGE			WM_USER+10
#define WM_INFO2BATTLE				WM_USER+11
#define WM_FINDRANK					WM_USER+12
#define WM_UPDATEFIELD				WM_USER+13
#define WM_LOGIN					WM_USER+14
#define WM_CONNECTINIT				WM_USER+15
#define WM_JOINBATTLE				WM_USER+16
#define WM_LISTRANK					WM_USER+17
#define WM_BANNER_RECEVE			WM_USER+18
#define WM_SAY						WM_USER+19
#define	WM_ERROR_NANGGEK			WM_USER+20
#define WM_FILESIZE                 WM_USER+21
#define WM_PROGRESS_SIZE            WM_USER+22

//#define COMMANDLENGTH (9)
typedef unsigned char   BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;

#define dMAXCDKEYSIZE				17

#define	dMAXUSERIDSIZE				17
#define	dMAXUSERPWSIZE				17

#define	dMAXFIELDNAMESIZE			21

#define	dMAXBATTLENAMESIZE			21
#define	dMAXBATTLEPWSIZE			17
#define	dMAXBATTLEMAPNAME			17

#define dMAXCHATSIZE				128
/***********************************************************************
 *��� �ӽù������� ���� 
 *�˴� 17�� ���� 5.26 ���
 ***********************************************************************/
//#define dMAXFIELDLIST				30						//5.28 ���� ����
#define dMAXFIELD					30						//5.28	30�� 
//#define dMAXRANKLIST				20						//5.28 ���� ����.
//#define dMAXUSERLIST				20		//	�ʵ���� �ѹ��� ������ ������.., 5.30
//#define dMAXBATTLELIST				30		//	�ѹ��� ������ �ִ� �ִ� �׹� ����Ʈ ��.....6.12
/**********************************************************************/
#define dIN_OUT				0		//	�̴��� ������......6.12
#define dIN_IN				1		//	�̴��� ���´�.......6.12

//-------------------------------------------------------------------------------------6.15	÷�� 
#define dIN					0
#define dOUT				1


#define dDBBACKUP_CYCLE		7	//	���� : ��
#define dDELETEID_CYCLE		3	//	���� : ��
#define dLIVECODE_CYCLE		4	//	���� : ��

#define dMAXTIMES			6	//	�ð��� ����ϴ� �ڸ���

#define dMAXCDKEYSIZE		17


#define	dMAXFIELDLIST_S		12		//	���� ��ȭ�鿡 �ѷ����� ����
#define	dMAXBATTLELIST_S	12		//	���� ��ȭ�鿡 �ѷ����� ����
//#define dMAXFIELDLIST		dMAXFIELDLIST_S*2		//	�ѹ��� ������ �ִ� �ִ� ä�� ����Ʈ ��
#define dMAXFIELDLIST		14		//	�ѹ��� ������ �ִ� �ִ� ä�� ����Ʈ �� 20���� 14�� 7.3

//#define dMAXBATTLELIST		dMAXBATTLELIST_S*2		//	�ѹ��� ������ �ִ� �ִ� �׹� ����Ʈ ��
#define dMAXBATTLELIST		14		//	�ѹ��� ������ �ִ� �ִ� �׹� ����Ʈ ��


#define dMAXRANKLIST_S		20		//	�ѹ��� ������ �ִ� �ִ� ��ŷ ����Ʈ ��
#define dMAXUSERLIST_S		20		//	�ʵ���� �ѹ��� ������ ������..
#define dMAXRANKLIST		20		//	�ѹ��� ������ �ִ� �ִ� ��ŷ ����Ʈ �� ,6.26  ��û�� ���� �ٲ�
#define dMAXUSERLIST		dMAXUSERLIST_S+10		//	�ʵ���� �ѹ��� ������ ������..

#define dMAXBANNER_SIZE		1024*4				//7.4�߰� ��� �ִ� Size
//----------------------------------------------------------------------------------------------
//	 ��޿� ���Ͽ�...
#define dCLASS_1			1	//	�̺�
#define dCLASS_2			2	//	�Ϻ�
#define dCLASS_3			3	//	��
#define dCLASS_4			4	//	����
#define dCLASS_11			11	//	�ϻ�
#define dCLASS_12			12	//	�߻�
#define dCLASS_13			13	//	���
#define dCLASS_21			21	//	����
#define dCLASS_22			22	//	����
#define dCLASS_23			23	//	����
#define dCLASS_31			31	//	�ҷ�
#define dCLASS_32			32	//	�߷�
#define dCLASS_33			33	//	���
#define dCLASS_43			43	//	����
#define dCLASS_44			44	//	����
#define dCLASS_45			45	//	����
#define dCLASS_46			46	//	����
#define dCLASS_47			47	//	����




#define MAXDATA											(8192)



typedef struct cMSG_BASE{
	DWORD				size;			//��Ŷ�� ���� ����
	WORD				type;			//��Ŷ�� ������ ���� 
}cmsg_base;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ���̼������� >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define dCMSG_SRVLIST									0x0001

typedef struct cMSG_SERVLIST{
	cmsg_base			base;
	char				name[17];
	char				ip[17];
	WORD				port;	//	hyun - add  (06. 09)
}cmsg_servlist;
/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û 
	 ��     ��  : �������� �ʱ� ��ȭ
	 ���� ��Ŷ  : cSMSG_INIT
	 ��     Ÿ  :																		//6.4�� �߰�
-----------------------------------------------------------*/
//#define dCMSG_INIT		0x1023
#define dCMSG_INIT		0x00A1								//8.22�� ���̷� ������
typedef struct cCMSG_INIT 
{
	cmsg_base			base;
	WORD				version;
	BYTE				banner;			//	Banner�� Version
	
}ccmsg_init;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : �������� �ʱ� ��ȭ
	 ���� ��Ŷ  : 
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_INIT		0x00B1
	#define dINIT_SUCCESS		0
	#define dINIT_NOTVERSION	1
	#define dINIT_NOTBANNER		2
	#define dINIT_BOTH			3

typedef struct cSMSG_INIT 
{
	cmsg_base			base;
	BYTE				result;

}csmsg_init;

#define dCMSG_REDBANNER		0x00A2

typedef struct cCMSG_REDBANNER 
{
	cmsg_base		base;
}ccmsg_redbanner;

#define	dSMSG_INFOBANNER		0x00B2

typedef	struct cSMSG_INFOBANNER
{
	cmsg_base			base;
	DWORD				FileSize;				//	ȭ���� ũ��
	BYTE				BannerVersion;			//	��� ����
	WORD				tot;

}csmsg_infobanner;

#define dSMSG_TRANSBANNER		0x00B3

typedef struct cSMSG_TRANSBANNER
{
	cmsg_base			base;
	WORD	cur;						//	�� ������ �� / ���� ��ȣ 
	WORD	f_size;						//	data�� ����� �ִ�.
	BYTE	data[dMAXBANNER_SIZE];		//	4kb�� ��Ƽ� ������.

}csmsg_transbanner;


#define dCMSG_REDPATCH		0x00A3

typedef struct cCMSG_REDPATCH
{
	cmsg_base		base;
}ccmsg_redpatch;

#define	dSMSG_INFOPATCH		0x00B4

typedef	struct cSMSG_INFOPATCH
{
	cmsg_base				base;
	DWORD					FileSize;				//	ȭ���� ũ��
	BYTE					PatchVersion;			//	��ġ ����
	WORD					tot;
}csmsg_infopatch;

#define dSMSG_TRANSPATCH		0x00B5

typedef struct cSMSG_TRANSPATCH
{
	cmsg_base				base;
	WORD					cur;						//	�� ������ �� / ���� ��ȣ 
	WORD					f_size;						//	data�� ����� �ִ�.
	BYTE					data[dMAXBANNER_SIZE];		//	.�� 1byte�� ������ ��¼�� �Ѥ�;

}csmsg_transpatch;

#define dCMSG_DISDMS									0x0002			//���̼������� ������ �������� ������ �˷��� 
//------------------------------------------------------------------------------7.31
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<���̼������� ��<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

typedef struct FIELDINFO{
	char	name[dMAXFIELDNAMESIZE];	//	ä���� �̸�
	char	hostid[dMAXUSERIDSIZE];		//	ä�� ���� 

	BYTE	usercount;				//	ä�ο� ���� �������� �ο�
	DWORD	times;						//	������ �ð�
}sfieldinfo;


typedef struct BATTLEINFO{
	char	name[dMAXBATTLENAMESIZE];	//	��Ʋ�� �̸�
	BYTE	maxcount;					//	��Ʋ �ִ� �ο�
	BYTE	usercount;					//	��Ʋ�� ���� �������� �ο�(���� ����)
	DWORD	times;						//	������ �ð�


	char	mapname[dMAXBATTLEMAPNAME];
	WORD	wsize,hsize;//	Ÿ�� ������
	BYTE	terrain;					//	����
	BYTE	secret;				//	����(0) / �����(1) 

}sbattleinfo;

//(((((((((((((((((((((((((((((((((((((((((((((���������� ����))))))))))))))))))))))))))))))))))))))))))))))))))))))

/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û 
	 ��     ��  : �α����� ��û�Ѵ�. 
	 ���� ��Ŷ  : cSMSG_LOGIN
	 ��     Ÿ  :													//6.4 ���� ���� ����.
-----------------------------------------------------------*/

typedef struct cMSG_LOGIN
{
	cmsg_base			base;
	char				id[dMAXUSERIDSIZE];
	char				pw[dMAXUSERPWSIZE];
	char				cdkey[dMAXCDKEYSIZE];		//	CDKEY�� ���̵� ����Ծ�� �ߴµ� --;
	DWORD				index;						//	CDKEY�� �ε��� (�������� ����...^^)
}cmsg_login;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û 
	 ��     ��  : ���ο� ������ �����.
	 ���� ��Ŷ  : cSMSG_NEWID
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_NEWID										0x1002
typedef struct cCMSG_NEWID
{
	cmsg_base			base;
	char				id[dMAXUSERIDSIZE];
	char				pw[dMAXUSERPWSIZE];
	char				cdkey[dMAXCDKEYSIZE];
	DWORD				index;
}ccmsg_newid;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û 
	 ��     ��  : �н����带 �ٲ۴�.
	 ���� ��Ŷ  : cSMSG_CHGPW
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_CHGPW		0x1003

typedef struct cCMSG_CHGPW 
{
	cmsg_base			base;
	char				id[dMAXUSERIDSIZE];			//6.20 �߰�

	char				oldpw[dMAXUSERPWSIZE];
	char				newpw[dMAXUSERPWSIZE];
	char				cdkey[dMAXCDKEYSIZE];
	DWORD				index;
	
}ccmsg_chgpw;

/*-----------------------------------------------------------
	 ��Ŷ ����  : �뺸
	 ��     ��  : �޼����� �����Ѵ�.
	 ���� ��Ŷ  : cSMSG_CHAT
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_CHAT										0x1004
typedef struct cCMSG_CHAT
{
	cmsg_base			base;
	char	msg[dMAXCHATSIZE];
}ccmsg_chat;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û
	 ��     ��  : ä���� �����.
	 ���� ��Ŷ  : cSMSG_CREATEFIELD
	 ��     Ÿ  :	*******************5.26.���
-----------------------------------------------------------*/
#define	dCMSG_CREATEFIELD   0x1005
typedef struct cCMSG_CREATEFIELD
{
	cmsg_base			base;
	char				name[dMAXFIELDNAMESIZE];
}ccmsg_createfield;
/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û
	 ��     ��  : Ÿ ä�ο� �����Ѵ�. 
	 ���� ��Ŷ  : cSMSG_JOINFIELD
	 ��     Ÿ  :   *******************5.26.���
-----------------------------------------------------------*/
#define dCMSG_JOINFIELD		0x1006
typedef struct cCMSG_JOINFIELD
{
	cmsg_base			base;
	char name[dMAXFIELDNAMESIZE];
}ccmsg_joinfield;
/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û
	 ��     ��  : ä�� ����Ʈ�� ��û�ϴ�. 
	 ���� ��Ŷ  : cSMSG_LISTFIELD
	 ��     Ÿ  :   *********************5.26.���
-----------------------------------------------------------*/
#define dCMSG_LISTFIELD		0x1007
	#define dLISTFIELD_UPDATE		0	//	���Ӱ� ���ŵ� ������ ��û�Ѵ�.
	#define dLISTFIELD_DEL			1	//	������ ����Ÿ�� �䱸�Ѵ�.
	#define dLISTFIELD_ADD			2	//	���Ӱ� �߰��� ������ �䱸�Ѵ�.
	#define dLISTFIELD_ONE			3	//	��û�� �������� �Ѿ��.


	#define dLISTFIELD_ALL			4	//	��� ������ ���Ӱ� �޴´�.


	//	�� ��� Ŭ���̾�Ʈ�� �޾Ƽ� ���͸� ������ ���ľ� �Ѵ�. 
	//	�̹� �Ѿ�� �ʵ��� ������ �������� �и��ϴ� �������� �ߺ� ���Ե� ���ɼ��� �ִ�.  
typedef struct cCMSG_LISTFIELD
{
	cmsg_base			base;
//	BYTE				sep;									//6.17 sunday�߰�	731����
//	BYTE				page;									//6.15 �߰�. 1 base�� ������ ��û��ȣ 731 delete

}ccmsg_listfield;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û
	 ��     ��  : �׹��� �����. 
	 ���� ��Ŷ  : cSMSG_CREATEBATTLE
	 ��     Ÿ  :   *********************5.26.���
-----------------------------------------------------------*/
#define dCMSG_CREATEBATTLE	0x1008

	#define dTERRAIN_GRASSLAND	0x0000	//	�ʿ�����
	#define	dTERRAIN_DESERT		0x0001	//	�縷����

typedef struct cCMSG_CREATEBATTLE
{
	cmsg_base			base;
	//	�׹��� ������ �̰� �������̴� �߰����� ����..
	char				name[dMAXBATTLENAMESIZE];
	char				pw[dMAXBATTLEPWSIZE];
	char				mapname[dMAXBATTLEMAPNAME];
	BYTE				type;									//	���� ����
	WORD				wsize,hsize;							//	Ÿ�� ������
	BYTE				terrain;								//	����
	BYTE				secret;	//	hyun - add  (06. 09)		//	����(0) / �����(1) 
	DWORD				mapdate;
//	WORD	year;												//	��ȭ���� �ۼ� ��
//	BYTE	month,day,hour,minute,second;						//	��ȭ���� �ۼ� ��,��,��,��
	BYTE	maxplayer;											//	�ִ� ���� �ο� 6.19 �߰� �Ѥ�

	char				address[17];
	WORD				port;
}ccmsg_createbattle;
/*-----------------------------------------------------------
	 ��Ŷ ����  : �뺸
	 ��     ��  : �׹��� ���������� ��������� Ȯ�����ش�.
	 ���� ��Ŷ  : 
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define	dCMSG_CREATERESULT		0x1009

	#define dCREATEBATTLE_MAKEIT	0
	#define dCREATEBATTLE_NOTMAKE	1

typedef struct cCMSG_CREATERESULT
{
	cmsg_base			base;
	BYTE				result;


}ccmsg_createresult;

/*-----------------------------------------------------------
	 ��Ŷ ����  : �뺸
	 ��     ��  : �׹������� ���� �ǰ���.
	 ���� ��Ŷ  : 
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_CANCELBATTLE		0x1010
	
typedef struct cCMSG_CANCELBATTLE
{
	cmsg_base			base;

	//	�ڽ��� ������ ��츸 �ѱ�ϱ� �˻��ؼ� �׿� +_+ !!
	
}ccmsg_cancelbattle;
/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û
	 ��     ��  : �׹濡 �����Ѵ�.
	 ���� ��Ŷ  : cSMSG_JOINBATTLE
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_JOINBATTLE	0x1011

typedef struct cCMSG_JOINBATTLE
{
	cmsg_base			base;
	char				name[dMAXBATTLENAMESIZE];
	char				pw[dMAXBATTLEPWSIZE];

}ccmsg_joinbattle;
/*-----------------------------------------------------------
	 ��Ŷ ����  : �뺸
	 ��     ��  : ������� �׹��� ������ �����Ѵ�. ȣ��Ʈ�� ������.
	 ���� ��Ŷ  : 
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_INFOBATTLE		0x1012

typedef struct cCMSG_INFOBATTLE
{
	cmsg_base			base;
	BYTE				maxplayer;
	BYTE				currentplayer;	//	���͵� ���� �����ؼ� �ѱ��.
							//	�� ������ ���� �������� �ٽ� �����Ѵ�. 

}ccmsg_infobattle;
/*-----------------------------------------------------------
	 ��Ŷ ����  : �뺸
	 ��     ��  : ������ �����Ѵ�. 
	 ���� ��Ŷ  : 
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_LAUNCHBATTLE		0x1013

typedef struct cCMSG_LAUNCHBATTLE 
{
	cmsg_base			base;
	//	�׹��� ���θ��� �� �޼����� �ѱ��. 
	
}ccmsg_launchbattle;
/*-----------------------------------------------------------
	 ��Ŷ ����  : �뺸
	 ��     ��  : �׹濡�� ���´�. 
					(������.. ���������� ��� ä�η� ���� ���ؼ� �����ش�.)
					(���� �������� ����쿡�� �� �޼����� �ѱ��.)
	 ���� ��Ŷ  : 
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_EXITBATTLE		0x1014

typedef struct cCMSG_EXITBATTLE
{
	cmsg_base				base;
	//	������ �� ���濡�� ������ �� �޼����� ������. 
	
}ccmsg_exitbattle;
/*-----------------------------------------------------------
	 ��Ŷ ����  : �뺸
	 ��     ��  : �׹濡�� ���´�. (���߿� ���´�. ����� resultbattle�� ����)
					(���������� �� �׹��� ��������, �״�� ������ �˰��ִ�.)
	 ���� ��Ŷ  : 
	 ��     Ÿ  : 

class cCMSG_ENDBATTLE : public cMSG_BASE{
public:
	BYTE	last;	//	0 - ���� ���»�� , 1 - �������� ������� �������.

	RESULTBATTLE		rb[8];

	cCMSG_ENDBATTLE()	{type=dCMSG_ENDBATTLE;}
};


-----------------------------------------------------------*/
#define dCMSG_ENDBATTLE		0x1015
typedef struct	RESULTBATTLE{
	char	id[dMAXUSERIDSIZE];
	WORD	result;		//	0	�й� / 1	�¸�
}sresultbattle;

typedef struct cCMSG_ENDBATTLE
{
	cmsg_base				base;
	BYTE					last;	//	0 - ���� ���»�� , 1 - �������� ������� �������.
	BYTE					count;	//������
	sresultbattle			sresult[8];	//	���⿡ �� ����� �޴´�.
}ccmsg_endbattle;
/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û
	 ��     ��  : �׹渮��Ʈ�� �䱸�Ѵ�.
	 ���� ��Ŷ  : dSMSG_LISTBATTLE
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_LISTBATTLE		0x1016



typedef struct cCMSG_LISTBATTLE
{
	cmsg_base				base;
	BYTE					sep;
	BYTE					page;
	BYTE						type;


}ccmsg_listbattle;
/*----------------------------------------------------------

	 ��Ŷ ����  : �뺸
	 ��     ��  : ���� ����� �����ش�.
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
	 #define dCMSG_RESULTBATTLE		0x1017
	 2001.6.4 �� ��Ŷ��ȯ�� ���� ����.........
-----------------------------------------------------------*/


/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û
	 ��     ��  : ��ŷ�� �䱸�Ѵ�.
	 ���� ��Ŷ  : cSMSG_LISTRANK
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_LISTRANK		0x1018

typedef struct cCMSG_LISTRANK
{
	cmsg_base			base;
	BYTE				page;//6.26 �߰�

}ccmsg_listrank;
/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û
	 ��     ��  : ��ŷ����Ʈ���� ã�´�.
	 ���� ��Ŷ  : cSMSG_FINDRANK
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_FINDRANK		0x1019

typedef struct cCMSG_FINDRANK
{
	cmsg_base			base;
	char	id[dMAXUSERIDSIZE];

}ccmsg_findrank;

/*-----------------------------------------------------------
	 ��Ŷ ����  : �뺸
	 ��     ��  : ������ ������.
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_LOGOUT	0x1021
typedef struct cCMSG_LOGOUT
{
	cmsg_base			base;
}ccmsg_logout;
/*-----------------------------------------------------------
	 ��Ŷ ����  : �뺸
	 ��     ��  : �׿� ������ ����� �����ش�.
				(�Ƹ� ���� �����ϰ�����.. ���Ұ�쵵 ��������.. dp�� �󸶳� �����. -_-;)
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_JOINRESULT	0x1022

#define dJOINRESULT_SUCCESS		0x0000
#define dJOINRESULT_FAIL		0x0001

typedef struct cCMSG_JOINRESULT
{
	cmsg_base			base;
	BYTE				result;
}ccmsg_joinresult;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Server Message  /////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : �α����� ����� �����ش�.
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_LOGIN		0x2001


//	���� - ��� ���� �� ���� �ٿ����ϴ�. ���� Ʋ��������.
	#define dLOGIN_SUCCESS		0x0000	//	���������� �α���
	#define dLOGIN_NOTLOGIN		0x0002	//	ID Ȥ�� PW�� ��ġ���� �ʴ´�. 
	#define dLOGIN_LOGINED		0x0003	//	������ ���� ID�� �α��� �Ǿ��ִ� �����̴�.
	#define dLOGIN_USECDKEY		0x0004	//	CDKEY�� ������̴�. 
	#define dLOGIN_NOTCDKEY		0x0005	//	CDKEY�� ��ġ���� �ʴ´�. 
	#define dLOGIN_CLOSECDKEY	0x0006	//	�� CDKEY�� �����ڿ� ���� �����ִ�.
	#define dLOGIN_FULL			0x0008	//	�� ä���� ����.
	#define dLOGIN_FAIL			0x0009	//	�Ͽ��� �α��� �Ұ��� ^^;;

typedef struct cSMSG_LOGIN
{
	cmsg_base			base;
	BYTE				result;
	char	fieldname[dMAXFIELDNAMESIZE];		//	�� ä���� �̸�. ����� 32��
//	WORD	fieldserial;			//	ä�� �ø���

/*	
	char	fieldname[dMAXFIELDNAMESIZE];		//	�� ä���� �̸�. ����� 32��
	WORD	fieldserial;			//	ä�� �ø���
	WORD	usercount;				//	�� ���� �ο�,
	WORD	battlecount,fieldcount;	//	���� ��,ä�� ��

	÷�� � ������ �������� ������ �����Ѵ�. 
	��, ��� �̷� ������ �ʿ��������, ������ ä�ο� ���� ������ �Ѱ���� ������µ�...
*/
}csmsg_login;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ���� 
	 ��     ��  : ���ο� ������ ���� ����� �����ش�.
	 ���� ��Ŷ  : cSMSG_NEWID
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_NEWID		0x2002

#define dNEWID_SUCCESS		0x0000	//	���ο� �Ƶ� ��� ����
#define dNEWID_USEID		0x0001	//	�����ϴ� �Ƶ�
#define dNEWID_USECDKEY		0x0002	//	CDKEY�� ������̴�. 
#define dNEWID_NOTCDKEY		0x0003	//	CDKEY�� ��ġ���� �ʴ´�. 
#define dNEWID_CLOSECDKEY	0x0004	//	�� CDKEY�� �����ڿ� ���� �����ִ�.
#define dNEWID_FAIL			0x0005	//	������ �������� ���� �Ұ�.(DB�� ���� �� ��� ���)
	
typedef struct cSMSG_MEWID
{
	cmsg_base		base;
	BYTE			result;

}csmsg_newid;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : �н����� �ٲ� ����� �����ش�.
	 ���� ��Ŷ  : 
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_CHGPW		0x2003

	#define dCHGPW_SUCCESS		0x0000	//	�н����带 ���������� ��ü
	#define dCHGPW_USEDID		0x0001	//	���̵� ������̴�.
	#define dCHGPW_USECDKEY		0x0002	//	cdkey�� ������̴�.
	#define dCHGPW_NOTCDKEY		0x0003	//	cdkey�� ��ġ���� �ʴ´�.
	#define	dCHGPW_CLOSECDKEY	0x0004	//	cdkey�� �����ִ�.
	#define dCHGPW_NOTPW		0x0005	//	������ �н������ ��ġ���� �ʴ´�.
	#define dCHGPW_FAIL			0x0006	//	������ �������� ���� �Ұ�.

typedef struct cSMSG_CHGPW 
{
	cmsg_base			base;
	BYTE				result;

}csmsg_chgpw;


/*----------------------
-------------------------------------
	 ��Ŷ ����  : �̷� �̰� ä�� �޽������� 
	 ��     ��  : 
	 ���� ��Ŷ  : 
	 ��     Ÿ  : 
-----------------------------------------------------------*/

#define dSMSG_CHAT		0x2004
typedef struct cSMSG_CHAT
{
	cmsg_base			base;
	char				msg[dMAXCHATSIZE];
}csmsg_chat;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : ä�� ���� ����� �����ش�.
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_CREATEFIELD		0x2005

	#define dCREATEFIELD_SUCCESS	0x0000
/*	#define dCREATEFIELD_FULL		0x0001	//	�ִ� ���� ���� �Ѿ�� ���̻� ������ �� ����.
	#define dCREATEFIELD_FAIL		0x0002	//	������ �������� ������ �� ����. (�̷� ��찡 ������ --;)
*/	//	�ϳ��� ��������.
	#define dCREATEFIELD_FULL		0x0001	//	�ִ� ���� ���� �Ѿ�� ���̻� ������ �� ����.
	#define dCREATEFIELD_USEDNAME	0x0002	//	���̸��� �����Ѵ�.
	#define dCREATEFIELD_FAIL		0x0003	//	������ �������� ������ �� ����. (�̷� ��찡 ������ --;)

typedef struct cSMSG_CREATEFIELD
{
	cmsg_base			base;
	BYTE				result;
	char				name[dMAXFIELDNAMESIZE];


}csmsg_createfield;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ����/�뺸
	 ��     ��  : ä������ ����� �����ش�. 
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_JOINFIELD		0x2006

	#define	dJOINFIELD_SUCCESS	0x0000
	#define	dJOINFIELD_FULL		0x0001	//	�ο��ʰ��� �� �� ����.
	#define	dJOINFIELD_NOTPW		0x0002	//	�н����尡 Ʋ�ȴ�.
	#define	dJOINFIELD_NOTNAME	0x0003	//	������ ���̸��� ����.
	#define	dJOINFIELD_FAIL		0x0004	//	����?! -_-+
	#define	dJOINFIELD_DONTENER	0x0005	//	���� �ִ� ���°� �ƴϴ�.	
	#define dJOINFIELD_NOMORE	0X0006	//	


typedef struct cSMSG_JOINFIELD
{
	cmsg_base					base;
	BYTE						result;
//	WORD						serial;		�̳��� �������� ���� ����� �� �κ��ε�...
	char	fieldname[dMAXFIELDNAMESIZE];	//	Ŭ���̾�Ʈ�� ��û���� �� �޼����� ���⵵ �ϱ� ����
}csmsg_joinfield;
/*-----------------------------------------------------------
	 ��Ŷ ����  : �뺸
	 ��     ��  : ä���� ������ �����Ѵ�.
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_UPDATEFIELD		0x2007

typedef struct cSMSG_UPDATEFIELD
{
	cmsg_base				base;
	char					id[dMAXUSERIDSIZE];
	BYTE					imclass;	//	����� �ѱ��.5.31 �߰�
	WORD					win,lost,draw;					//���� 8.2 �߰� 
	BYTE					in;			//	1 ����,0 ����
	//	��Ÿ ������ ���� ���뵵 �����ұ�?
}csmsg_updatefield;
/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : ä�� ����Ʈ�� �ش�.
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_LISTFIELD		0x2008
	//BYTE  cur ������
	
typedef struct cSMSG_LISTFIELD
{
	cmsg_base				base;
	
	BYTE					count;	//	�� ��������, �ڽ��� ������ ��ȣ, �ڽ��� ����Ʈ��
	sfieldinfo				sField[dMAXFIELDLIST];
}csmsg_listfield;
/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : �׹��� ��������� �����ش�.
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_CREATEBATTLE		0x2009

	#define cCREATEBATTE_SUCCESS	0x0000
	#define cCREATEBATTE_USEDNAME	0x0001	//	���� �̸��� �׹��� �����Ѵ�.
	#define cCREATEBATTE_FULL		0x0002	//	���̻� �׹��� ����� ����.
	#define cCREATEBATTE_FAIL		0x0003	//	���� (�۽� �̷���� ���ٴϱ� -_-;)

typedef struct cSMSG_CREATEBATTLE 
{
	cmsg_base				base;
	BYTE					result;
	char	name[dMAXBATTLENAMESIZE];	//6.23 �߰�

//	WORD					serial;			//	������ �� �ø����ȣ
//	�ø����� �� �����Ȱ� �������� �̾߱��ϼż� ��ģ�ǵ�...	
}csmsg_createbattle;
/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : �׹濡 ���ΰ���� �����ش�.
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_JOINBATTLE		0x2010

	#define dJOINBATTLE_SUCCESS		0x0000
	#define dJOINBATTLE_FULL		0x0001		//	�ο��� �� ���Ƽ� �ȵ�.
	#define dJOINBATTLE_NOTNAME		0x0002		//	�̸��� Ʋ����.
	#define dJOINBATTLE_NOTPW		0x0003		//	�н����尡 Ʋ����.
	#define dJOINBATTLE_RUN			0x0004		//	������ ���̴�. ��� -_-+
	#define dJOINBATTLE_FAIL		0x0005		//	���� �̸޼��� �������� ������ --

typedef struct cSMSG_JOINBATTLE
{
	cmsg_base				base;
	BYTE					result;
//	WORD					serial;	���������� �� ������.
	char					address[17];
	WORD					port;
	DWORD					mapdate;


}csmsg_joinbattle;
/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : �׹渮��Ʈ�� �����ش�.
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_LISTBATTLE	0x2011
	#define dLISTBATTLE_UPDATE		0		//	���Ӱ� ���ŵ� ������ ��û�Ѵ�.
//	#define dLISTBATTLE_DEL			1		//	������ ����Ÿ�� �䱸�Ѵ�.
//	#define	dLISTBATTLE_ADD			2		//	�ֱ� �߰��� ������ �޴´�.
	#define dLISTBATTLE_ONE			3		//	��û�� �������� �Ѿ��.

	#define dLISTBATTLE_ALL			4		//	��� ������ ���Ӱ� �޴´�.

typedef struct cSMSG_LISTBATTLE 
{
	cmsg_base				base;
	BYTE	sep;
	BYTE	tot,cur,count;	//	�� ��������, �ڽ��� ������ ��ȣ, �ڽ��� ����Ʈ��
	BYTE	type;
 
	sbattleinfo				sbattle[dMAXBATTLELIST];

}csmsg_listbattle;
/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : ��ŷ�� �����ش�.
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_LISTRANK	0x2012

typedef struct cSMSG_LISTRANK
{
	cmsg_base				base;
	//	�߰�
	BYTE		cur,count;	//	�� ��������, �ڽ��� ������ ��ȣ, �ڽ��� ����Ʈ�� 6.26�������� ����

	WORD					rank[dMAXRANKLIST];
	char					id[dMAXRANKLIST][dMAXUSERIDSIZE];
	BYTE					imclass[dMAXRANKLIST];
	WORD					win[dMAXRANKLIST];
	WORD					lost[dMAXRANKLIST];
	WORD					draw[dMAXRANKLIST];	//8.02
	DWORD					score[dMAXRANKLIST];
	

}csmsg_listrank;
/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : ��ŷ����Ʈ���� Ư�������� ������ �˻��ؼ� �����ش�. 
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_FINDRANK		0x2013

typedef struct cSMSG_FINDRANK 
{
	cmsg_base				base;

	//	����/ �Ƶ� / �� / �� / ����
	WORD					rank;
	char					id[dMAXUSERIDSIZE];
	BYTE					imclass;	//	����... 0 �̸� ��ã�����̴�.
	WORD					win;
	WORD					lost;
	WORD					draw;	//8.02
	DWORD					score;
}csmsg_findrank;

//*-----------------------------------------------------------------------------------------------------------------------------*
//* client�� ������ Server�� �޴� ��������																						*
//*-----------------------------------------------------------------------------------------------------------------------------*


#define dCMSG_LOGIN										0x1001

/*-----------------------------------------------------------
	 ��Ŷ ����  : �뺸
	 ��     ��  : 20�� �߶� ���������� �����ش�.
	 ���� ��Ŷ  :
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_INFOFIELD		0x2014
typedef struct USERINFO{
	char	id[dMAXUSERIDSIZE];
	WORD	win,lost,draw;
	BYTE	imclass;				//	����� �ѱ��.
}suserinfo;

typedef struct cSMSG_INFOFIELD
{
	cmsg_base					base;
	BYTE		count;	//	�� ��������, �ڽ��� ������ ��ȣ, �ڽ��� ����Ʈ��
	suserinfo	sUserInfo[dMAXUSERLIST];

}csmsg_infofield;


/*
	cSMSG_FIND			:  (0x2051)ã�� ������ ����� �����ش�. 
	cSMSG_SAY			:  (0x2052)���濡�� �Ӹ������� �����Ѵ�. (�뺸)
	cSMSG_KICK			:  (0x2053)�����Ų��.(�뺸)
	cSMSG_PROFILE		:  (0x2054)��û�� ����� ������ �����ش�. 
	cSMSG_DIS           :  (0x2055)�����ڰ� ������ �����Ų��.(�Ⱓ�� ���Ұǰ�???)
	cSMSG_TRANSFILE		:  (0x2056)���� �����Ѵ�. 
	cSMSG_INVITE		:  (0x2057)��û �޼����� �����ش�. 
*/
/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û
	 ��     ��  : ����ֳ�?
	 ���� ��Ŷ  : cCMSG_LIVE
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_LIVE			0x2017

typedef struct cSMSG_LIVE
{
	cmsg_base				base;

}csmsg_live;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : ����ִٰ� ����ϱ�
	 ��û ��Ŷ  : cSMSG_LIVE
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_LIVE			0x1024

typedef struct cCMSG_LIVE
{
	cmsg_base				base;
	
}ccmsg_live;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : ����� �ʵ��
	 ��û ��Ŷ  : cCMSG_LISTBATTLE
	 ��     Ÿ  : 6.16
-----------------------------------------------------------*/
#define dSMSG_CHGFIELD		0x2020

typedef struct cSMSG_CHGFIELD {
	cmsg_base			base;
	BYTE	count;
	char	name[dMAXFIELDLIST][dMAXFIELDNAMESIZE];
	BYTE	usercount[dMAXFIELDLIST];
}csmsg_chgfield;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : ����� �׹��
	 ��û ��Ŷ  : cCMSG_LISTBATTLE
	 ��     Ÿ  : 6.16
-----------------------------------------------------------*/
#define dSMSG_CHGBATTLE		0x2021

typedef struct cSMSG_CHGBATTLE {
	cmsg_base			base;
	BYTE	count;
	char	name[dMAXBATTLELIST][dMAXFIELDNAMESIZE];
	BYTE	usercount[dMAXBATTLELIST],maxusercount[dMAXBATTLELIST];
	BYTE	bStart;	//	0 - ��� , 1 - ���۵�.				6.23
	BYTE	type;

}csmsg_chgbattle;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : ������ �ʵ��
	 ��û ��Ŷ  : cCMSG_LISTFIELD
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_DELFIELD		0x2018

typedef struct cSMSG_DELFIELD
{
	cmsg_base			base;
	BYTE				sep,count;
	char	name[dMAXFIELDLIST][dMAXFIELDNAMESIZE];
	DWORD	times[dMAXFIELDLIST];

}csmsg_delfield;

/*-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : �׹��� ������ ����
	 ��û ��Ŷ  : cCMSG_INFO2BATTLE
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dSMSG_INFO2BATTLE		0x2022
	#define dINFO2BATTLE_SUCCESS		0
	#define dINFO2BATTLE_NOTFIND		1

typedef struct cSMSG_INFO2BATTLE
{
	cmsg_base			base;
//	BYTE				result;
	char	mapname[dMAXBATTLEMAPNAME];
//	BYTE	type;			//	���� ����
	WORD	wsize,hsize;	//	Ÿ�� ������
	BYTE	terrain;		//	����
	BYTE	secret;			//	����(0) / �����(1) 
//	BYTE	maxplayer;		//	�ִ� ���� �ο�
	char	address[17];
	WORD	port;

	
}csmsg_info2battle;


/*-----------------------------------------------------------
	 ��Ŷ ����  : ��û
	 ��     ��  : ���� ������ ������ ��û
	 ���� ��Ŷ  : cSMSG_INFO2BATTLE
	 ��     Ÿ  : 
-----------------------------------------------------------*/
#define dCMSG_INFO2BATTLE		0x1025

typedef struct cCMSG_INFO2BATTLE
{
	cmsg_base			base;
	char name[dMAXBATTLENAMESIZE];
}ccmsg_info2battle;
/*-----------------------------------------------------------
	��Ŷ : �ӼӸ�

  ----------------------------------------------------------*/
#define dCMSG_SAY				0x1052

typedef struct cCMSG_SAY
{
	cmsg_base			base;
	char				id[dMAXUSERIDSIZE];
	char				msg[dMAXCHATSIZE];
}ccmsg_say;

#define dSMSG_SAY			0x2052
typedef struct cSMSG_SAY
{
	cmsg_base			base;
	char				msg[dMAXCHATSIZE];
}csmsg_say;

#define dSMSG_TRANSFILE		0x2056
typedef struct cSMSG_TRANSFILE
{
	cmsg_base			base;	
	WORD	cur;					//	�� ������ �� / ���� ��ȣ 
	WORD	size;						//	data�� ����� �ִ�.
	BYTE	data[dMAXBANNER_SIZE];
	
}csmsg_transfile;

#define	dSMSG_ASKTRANS		0x2057

typedef struct cSMSG_ASKTRANS			
{
	cmsg_base			base;
	
	DWORD				FileSize;				//	ȭ���� ũ��
	BYTE				BannerVersion;			//	��� ����
	WORD				tot;				//7.5 ���� TRNASFILE���� �Űܿ�
}csmsg_asktrans;


typedef struct tagRoom
{
	char		strName[21];									//��ȭ���� �̸��� ���� ���ڿ� ����
	int			numUser;										//��ȭ����� ���� ���ڸ� ���� ����������
	BOOL		bCode;											//��ȭ���� ��ȣ�� �ɷ��ִ����� ���¸� üũ ,TRUE�� ��ȣ�� �ɸ���
}Room;

typedef struct tagUser
{
	char	id[dMAXUSERIDSIZE];
	WORD	win,lost,draw;
	BYTE	Mclass;

}tUser;
//dSMSG_LISTBATTLE
typedef struct tagBattle
{
	char		strName[dMAXFIELDNAMESIZE];
	BYTE	/*	�߰���...*/	maxcount;				//	��Ʋ �ִ� �ο�
	BYTE					usercount;				//	�׹濡 ���� �������� �ο�
	DWORD					times;
	char					mapname[dMAXBATTLEMAPNAME];
	WORD	wsize,hsize;//	Ÿ�� ������
	BYTE	terrain;					//	����
	BYTE	secret;						//	����(0) / �����(1) 

}BRoom;
typedef struct tagBattleRank
{
	WORD					rank;
	char					id[dMAXUSERIDSIZE];
	BYTE					imclass;
	WORD					win;
	WORD					lost;
	WORD					draw; //8.02
	DWORD					score;
}TBRank;
typedef struct tagFindRank
{
	
	WORD					rank;
	char					id[dMAXUSERIDSIZE];
	BYTE					imclass;	//	����... 0 �̸� ��ã�����̴�.
	WORD					win;
	WORD					lost;
	WORD					draw; //8.02
	DWORD					score;
}FindedRank;


//========================================================================================
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


typedef enum tagStateString
{
	BLOGIN,
	BDIALOG,
	AMUTE
}StateString;

typedef enum tagStateClient
{
	OUTROOM,
	INCHATROOM,
	INGAMEROOM,
	BEGAME
}StateClient;

typedef struct tagServerInfo
{
	char IP[255];
	long port;
}ServerInfo;	//�����Ϸ��� ������ ������ ��� ����ü




typedef struct tag_SendQueue
{
	int CurrentPos;
	int DesPos;

	int PacketSize[SendQueueSize];
	char *Packet[SendQueueSize];
}_SendQueue;



//�ӽ����� �ùķ��̼� ���� 
#define WM_BATTLEROOM_SUCCESS	WM_USER+100
#define WM_BATTLEROOM_CANCEL	WM_USER+101
#define WM_BATTLELAUNCH			WM_USER+102
#define WM_END_BATTLE			WM_USER+103
#define WM_BTN_FINDRNAK			WM_USER+104
#define WM_SEND_EXITBATTLE		WM_USER+105
#endif

#pragma pack()

/*-------------------------------------------------------------------------------------------------------------------------------------*
����� �����(6. 14)


cSMSG_INFOFIELD / dSMSG_UPDATEFIELD (ping���� ����)

-----------------------------------------------------------------------------------------
common.h���� ���� Ȥ�� �߰��� ���׵�

//	�����ϸ��� ���(������ 11��)�� �ϰ� �Ѵ޿� �ѹ� ���� ����ϰ� �����ϸ��� ������ ȭ���� �����Ѵ�. 

(������ dIN_IN�� dIN_OUT��� �Ʒ� �������� ����Ǿ���)
#define dIN					0
#define dOUT				1


#define dDBBACKUP_CYCLE		7	//	���� : ��
#define dDELETEID_CYCLE		3	//	���� : ��
#define dLIVECODE_CYCLE		4	//	���� : ��

#define dMAXTIMES			6	//	�ð��� ����ϴ� �ڸ���

#define dMAXCDKEYSIZE		17


#define	dMAXFIELDLIST_S		12		//	���� ��ȭ�鿡 �ѷ����� ����
#define	dMAXBATTLELIST_S	12		//	���� ��ȭ�鿡 �ѷ����� ����
#define dMAXFIELDLIST		dMAXFIELDLIST_S*2		//	�ѹ��� ������ �ִ� �ִ� ä�� ����Ʈ ��
#define dMAXBATTLELIST		dMAXBATTLELIST_S*2		//	�ѹ��� ������ �ִ� �ִ� �׹� ����Ʈ ��

#define dMAXRANKLIST_S		20		//	�ѹ��� ������ �ִ� �ִ� ��ŷ ����Ʈ ��
#define dMAXUSERLIST_S		20		//	�ʵ���� �ѹ��� ������ ������..
#define dMAXRANKLIST		dMAXRANKLIST_S+10		//	�ѹ��� ������ �ִ� �ִ� ��ŷ ����Ʈ ��
#define dMAXUSERLIST		dMAXUSERLIST_S+10		//	�ʵ���� �ѹ��� ������ ������..

#define dCLASS_41			41	//	����
#define dCLASS_42			42	//	����
#define dCLASS_43			43	//	����
#define dCLASS_44			44	//	����
#define dCLASS_45			45	//	����

-----------------------------------------------------------------------------------------

cCMSG_CREATEBATTLE�� �Ʒ� �߰�
	#define dBATTLETYPE_1		1
	#define dBATTLETYPE_2		2
	#define dBATTLETYPE_3		3
	#define dBATTLETYPE_4		4
	#define dBATTLETYPE_5		5

-----------------------------------------------------------------------------------------

#define dCMSG_LISTFIELD		0x1007

	#define dLISTFIELD_UPDATE		0	//	���Ӱ� ���ŵ� ������ ��û�Ѵ�.
	#define dLISTFIELD_ALL			121	//	��� ������ ���Ӱ� ���� �����̴�. �� ��� ó�� �������� �ް� �ȴ�. 
	#define dLISTFIELD_DEL			120	//	������ ����Ÿ�� �䱸�Ѵ�.
	//	�� ��� Ŭ���̾�Ʈ�� �޾Ƽ� ���͸� ������ ���ľ� �Ѵ�. 
	//	�̹� �Ѿ�� �ʵ��� ������ �������� �и��ϴ� �������� �ߺ� ���Ե� ���ɼ��� �ִ�.  
class cCMSG_LISTFIELD : public cMSG_BASE{
public:
	BYTE	page;

	cCMSG_LISTFIELD()	{size=sizeof(cCMSG_LISTFIELD);type=dCMSG_LISTFIELD;}
};

-----------------------------------------------------------------------------------------

#define dCMSG_LISTBATTLE		0x1016
	
	#define dLISTBATTLE_UPDATE		0		//	���Ӱ� ���ŵ� ������ ��û�Ѵ�.
	#define dLISTBATTLE_ALL			9999		//	��� ������ ���Ӱ� ���� �����̴�. �� ��� ó�� �������� �ް� �ȴ�. 
	#define dLISTBATTLE_DEL			10000	//	������ ����Ÿ�� �䱸�Ѵ�.
	//	�� ��� Ŭ���̾�Ʈ�� �޾Ƽ� ���͸� ������ ���ľ� �Ѵ�. 
	//	�̹� �Ѿ�� ��Ʋ�� ������ �������� �и��ϴ� �������� �ߺ� ���Ե� ���ɼ��� �ִ�.  
class cCMSG_LISTBATTLE : public cMSG_BASE{
public:
	WORD	page;

	cCMSG_LISTBATTLE()	{size=sizeof(cCMSG_LISTBATTLE);type=dCMSG_LISTBATTLE;}
};

-----------------------------------------------------------------------------------------

-----------------------------------------------------------
	 ��Ŷ ����  : ��û
	 ��     ��  : ����ֳ�?
	 ���� ��Ŷ  : cCMSG_LIVE
	 ��     Ÿ  : 
-----------------------------------------------------------
#define dSMSG_LIVE			0x2017

class cSMSG_LIVE : public cMSG_BASE{
public:

	cSMSG_LIVE()	{size=sizeof(cSMSG_LIVE);type=dSMSG_LIVE;}
};

-----------------------------------------------------------
	 ��Ŷ ����  : ����
	 ��     ��  : ����ִٰ� ����ϱ�
	 ��û ��Ŷ  : cSMSG_LIVE
	 ��     Ÿ  : 
-----------------------------------------------------------
#define dCMSG_LIVE			0x1024

class cCMSG_LIVE : public cMSG_BASE{
public:

	cCMSG_LIVE()	{size=sizeof(cCMSG_LIVE);type=dCMSG_LIVE;}
};

-----------------------------------------------------------------------------------------

<DB Table>

///////////////////////////////////////////////
//	User Table 
///////////////////////////////////////////////
char	id			17
char	pw			17
BYTE	imclass
word	win
word	lost
dword	score
word	f_win
word	f_lost
dword	f_score
word	lastdate		//	3������ ������ �ڸ���. to backup db
byte	isconnect

///////////////////////////////////////////////
//	Cdkey Table 
///////////////////////////////////////////////
dword	index
char	cdkey		17
byte	lock		(0 - open, 1 - close)



 *-------------------------------------------------------------------------------------------------------------------------------------*/
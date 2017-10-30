#pragma once

#define MSG_NO_PS_CTRL						151		// PS ���� �޼����� ��� �̰����� ó��.

enum {
	ps_ctrl_start_req=0,		//      151							// ���� ���� ������ ��û�� 151 
	ps_ctrl_start_result,		// 1  _NO_PS_STARTREQ_RESULT           MSG_NO_PS_STARTREQ			//  ���� ��û ���

	ps_ctrl_broad_req_cast,		// 2 #define MSG_NO_PS_BRD_CSTREQ                224							// 224  
	ps_ctrl_broad_cast,			// 3 #define MSG_NO_PS_BRD_CST                   MSG_NO_PS_BRD_CSTREQ		// 224

	ps_ctrl_stop_req,			// 4 #define MSG_NO_PS_STOPREQ                   225							// 225
	ps_ctrl_stop_result,		// 5 #define MSG_NO_PS_STOPREQ_RESULT            MSG_NO_PS_STOPREQ			// 225 

	ps_ctrl_shopitem_list_req,	// 6 #define MSG_NO_PS_SHOPITEM_LIST             226							// 226
	ps_ctrl_shopitem_list_result,	// 7#define MSG_NO_PS_SHOPITEM_LIST_RESULT      MSG_NO_PS_SHOPITEM_LIST		// 226

	ps_ctrl_shopitem_buy_req,	// 8 #define MSG_NO_PS_SHOPITEM_BUY              227							// 227
	ps_ctrl_shopitem_buy_result,// 9 #define MSG_NO_PS_SHOPITEM_BUY_RESULT       MSG_NO_PS_SHOPITEM_BUY		// 227

	ps_ctrl_stop_broad_cast,	// 10 #define MSG_NO_PS_STOP_BRDCST               228	// 228 �����ο��� ������ ������ �˸�.
	ps_ctrl_shopitem_sold,		// 11 #define MSG_NO_PS_SHOPITEM_SOLD_BRDCST      229	// 229 �����ο��� ������ �ȸ��� �˸���. 
	ps_ctrl_item_buy_to_owner	// 12 #define MSG_NO_PS_ITEM_BUY_TOOWNER		  230	// 230 �������� ������ �� ���� �������� �����۰� ���� ��ȭ.
} ;


//MSG_NO_PS_CTRL 151
struct MSG_PS_CTRL
{
	_sHeader			header ;
	u_char				ucMessage ;

	u_char				ucCtrl ;		// ���� ps_ctrl_ �����.
} ;

// HHH �� �������̴�. ���߿� ó���� ��. 
#define PSSHOP_MAX_ITEM_LIST            9           // �ǸŰ��� �ִ� ������.
#define PSSHOP_MAX_SHOP_NAME            37          // �ִ� ���� �̸� ������.
#define PSSHOP_MAX_WATCHER_NUM          10          // �ִ� ����� ����.

// ps_ctrl_start_req �ִ��� �����ϰ� ����ô�. 
struct MSG_PS_CTRL_START_REQ : public MSG_PS_CTRL
{
	char                cMode         ; // ��忡 ���� 
										// 0 �̸� ���� ���� 
										// 1 �̸� �ӽ÷� �ݾҴ� ���� �ٽ� open��.
	char                PSName[PSSHOP_MAX_SHOP_NAME]    ; // ���̸�. 
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    u_char                InvenIdx[PSSHOP_MAX_ITEM_LIST]   ; // �ִ� 9�� ������.
#else
	char                InvenIdx[PSSHOP_MAX_ITEM_LIST]   ; // �ִ� 9�� ������.
#endif
	u_char              ucItemCount[PSSHOP_MAX_ITEM_LIST]; // ������ ������ �÷����� ����. �κ��� ������ �ٸ� �� ������ �����϶�. 
	u_int               ItemPrice[PSSHOP_MAX_ITEM_LIST]  ; // �ִ� 9�� ������ ����. 
	char                ItemNum;        // ������ ����. 
};

// ps_ctrl_start_result
struct MSG_PS_CTRL_START_RESULT  : public MSG_PS_CTRL
{
	char                cMode     ; // ��忡 ����							�ݱ� ����϶� 
									// 0 �̸� shop ������ ���                       0�̸� ������ �ݱ� ���  
									// 1 �̸� �ӽ÷� �ݾҴ� ���� �ٽ� open�� ���    1�̸� �ӽ÷� �ݱ� ���  
									// 2 �̸� �ӽ÷� �ݱ� ��û�� ���� ��� 

	char                cReturn   ; // ���� �ڵ�. �����̸� ������ ������ ������ ���� ���̴�. -1�� ��� �ƿ� ������ ������ ����. 
									// cMode�� 2 �̸� 1�� ���� �� 			

	u_short             usPSSlotNo; // ���� ���� �ѹ�
};



// ps_ctrl_broad_req_cast
struct MSG_PS_CTRL_BROAD_REQ_CAST : public MSG_PS_CTRL
{
	u_short             usPSSLotNo;
	char                cMode     ; // STOPREQ�� ���϶� ���� ���� .
									// 0�̸� ���� �ݱ� 
									// 1�̸� ������ �ٲٱ�. 
									// 2�̸� �����ڰ� â�� �ݴ´�. 
};


// ps_ctrl_broad_cast
struct MSG_PS_CTRL_BROAD_CAST : public MSG_PS_CTRL
{
	u_short             usUID     ; // ���� UID
	char                PSName[PSSHOP_MAX_SHOP_NAME];
};



// ps_ctrl_stop_req
// ps_ctrl_stop_result
typedef MSG_PS_CTRL_BROAD_REQ_CAST      MSG_PS_CTRL_STOP_REQ;        // ���� ����(^^) ��Ŷ�� ��ε�ĳ��Ʈ ������Ʈ ��Ŷ�� ����. 
typedef MSG_PS_CTRL_START_RESULT		MSG_PS_CTRL_STOP_RESULT; // ��� ���� ��� ��Ŷ�� ��ŸƮ ��Ŷ ��� ��Ŷ�� ����. 



// ps_ctrl_stop_broad_cast
struct MSG_PS_CTRL_STOP_BROAD_CAST : public MSG_PS_CTRL
{
	u_short             usUID     ;  // ���� ������ UID
	char                cIsComplete; // 2 : ������ ��������� ��ε�ĳ����.  
									 // 1 : ������ �ݴ� ���� 
									 // 0 : ������ ������
};

// ps_ctrl_shopitem_list_req
struct MSG_PS_CTRL_SHOPITEM_LIST_REQ : public MSG_PS_CTRL         // ������ ����Ʈ ��û�� ���� ���� �����ڰ� â�� ���� �� ������ ����Ʈ �����ÿ��� ����. 
{
	u_short             usUID     ;
	char                cMode     ;  // 0 �̸� ������ ����Ʈ ��û 
									 // 1 �̸� ������ ����Ʈ���� �ڱ⸦ �����Ѵ�. 
};


struct PSItemList
{	
	char   cItemType   ;             // �ö� ������ Ÿ��.	
	short  sItem_Id    ;             //
	short  sSocket[4] ;
	u_char ucStrength ;

	u_short usCurDur   ;
	u_char ucSocketCount;            // 
	u_char ucItemCount ;             // ������ ����.	
	u_int  uiPrice     ;             // �������.

#ifdef _XDEF_SMELT_ITEM //Author : ����� // ���븰 ����ǥ��
	u_short usUsedProtect	: 1;	// ��縦 ����� ���� �ִ°�.
	u_short	usCurProtect	: 5;	// ������ ���, �ٸ� �븰�� ��. 0 - 15
	u_short	usCurMaxProtect	: 5;	// �ִ밭��

	u_short	usReserved		: 5;	// �����.
#endif

};

// ps_ctrl_shopitem_list_result
struct MSG_PS_CTRL_SHOPITEM_LIST_RESULT : public MSG_PS_CTRL
{
	char                cReturn    ; // -1�̸� ������ ����Ʈ�� ��ϵ� �����̸� ����Ʈ�� ��� �͵� �����Ͽ���. 
									 // -2�̸� �ڽ��� Ʈ���̵� ���̰ų� Ʈ���̵� ��û�� ���� �����̴�. ���� ���з� ó��. 
	PSItemList          ItemList[PSSHOP_MAX_ITEM_LIST];
};


// ps_ctrl_shopitem_buy_req
struct MSG_PS_CTRL_SHOPITEM_BUY_REQ : public MSG_PS_CTRL
{
	char                cReturn      ;
	u_short             usUID        ; // ���� ���� uid 
	char                cType        ; // ����� �������� Ÿ�� 
	short               sID          ; // ����� ������ ���̵�.              
	char                cSlotNoforBuy;
};

// ps_ctrl_item_buy_to_owner
struct MSG_PS_CTRL_ITEM_BUY_TO_OWNER : public MSG_PS_CTRL         // ������ �������� �����Ͽ��� �� ���ο��� ���󰡴� ��Ŷ. 
{	
	char                cReturn      ; // ����� ������ 1; 
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    u_char              cIndex       ; // �κ��͸� �������� �ȸ� ������ �ε���
#else
	char                cIndex       ; // �κ��͸� �������� �ȸ� ������ �ε���.
#endif
	char                cPSWIndex    ; // PS �����쿡���� �ε��� . 
	_sUser_Inven_v1_all Item		 ;
	u_int               uiMoney      ; // �ȸ��� �� ���� ���ݾ׼�. 
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// ps_ctrl_shopitem_buy_result
struct MSG_PS_CTRL_SHOPITEM_BUY_RESULT : public MSG_PS_CTRL
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    u_char              cReturn      ;  // ����� ���  �κ��͸� ���� �Ǵ� �ε��� 
#else
	char                cReturn      ;  // ����� ���  �κ��͸� ���� �Ǵ� �ε��� 
#endif
	// ���� �� ���� �����ڵ� �����ڵ�� ���Ŀ� �ڼ��ϰ� �����ϰ���. 
	_sUser_Inven_v1_all BuyItem		 ;
	u_int               uiMoney      ;  // ���� ��.	Ȯ�ο�
};



// ps_ctrl_shopitem_sold
struct MSG_PS_CTRL_SHOPITEM_SOLD : public MSG_PS_CTRL
{
	char                cSoldSlot    ; 
	u_short             OwnerUID     ; // ���� ���� UID
	u_short             UID          ; // �� ���  UID 
	char                cType        ; // �ȸ� ������ Ÿ��. Ȯ�ο�
	short               sID          ; // �ȸ� ������ ���̵�.  Ȯ�ο�
};
//PSHOP

#pragma once

struct _sHeader ;

//#################################################################################
//
//  Items, Trade, Use...
//  Main = 14, 15, 16, 17, 18
//  112 ~ 151
//
//#################################################################################
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define _V1_COUNT_INVEN_ITEM_			108		// 25(����) + 75(�κ��丮) + 8(�ӽ�)

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	#define _V1_COUNT_REAL_INVEN_			150		// ���� �κ��丮
#else
	#define _V1_COUNT_REAL_INVEN_			100		// ���� �κ��丮
#endif
#define _V1_INVEN_EXC_EQUIP_			75		// ������ ������ �κ��丮 ����.
#define _V1_INVEN_START_BACKPACK_		50		// 
#define _V1_TEMP_INVEN_COUNT_			8
#define _V1_TEMP_INVEN_START_			100
#define _V1_TEMP_INVEN_END_				108		// 108�� ���Ե��� ����.

#define _TOTAL_INVENTORY_				100		// ��ü ���� 2004.10.19->hotblood insert

#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
    #define _TOTAL_STORAGE_					60		// â�� ����Ʈ ����
#else
    #define _TOTAL_STORAGE_					40		// â�� ����Ʈ ����
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �κ��丮 ����.
// 14, 15 : 112 ~ 127

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �κ��丮 ������ ����, ������  
// 14
//#define MSG_NO_STORAGE_INFO             113     // â�� �ڽ��� �����û�� ���� ������ ����.s -> c 
#define MSG_NO_INVEN_MOVE				114		// �κ��丮 �������� ��ġ �̵�:����,Ż��,�賶����,�κ����� �� ��� ó��. 
#define MSG_NO_ITEM_GRAB_REQ			115		// ������ �Ա�
#define MSG_NO_ITEM_GRAB_SUCCESS		115		// ������ �Ա� ����/���� : ����ڿ��Ը� ����. **
#define MSG_NO_MONEY_GRAB_SUCCESS		116		// ���� �Ծ���.

#define MSG_NO_INVEN_DROP				117		// ������ �ٴڿ� ����. ( ������ ��û,���� )
#define MSG_NO_ITEM_DROP				118		// ������ �ٴڿ� ����. ( ���� )
#define MSG_NO_EQUIP_CHANGE				119		// ���� ��ȭ

#define MSG_NO_MONEY_DROP				120		// �� �������� ( ������ ��û,���� )
// 15
///////////////      ������ ���      ////////////////////////////////
#define MSG_NO_INVEN_INFO				121		// ĳ������ �κ��丮 ���� 
#define MSG_NO_USE_ITEM_REQ				122		// ������ ����û.
#define MSG_NO_USE_ITEM_RESULT			122		// ������ �����
#define MSG_NO_ELIXIR_USE_RESULT		231		// ���� ��� ���
#define MSG_NO_ELIXIR_INTOX_TIME        113		// �ߵ��ð� ��
#define MSG_NO_TEMP_INVEN_CTRL          123		// ���� �κ����� �κ����� �ű涧
#define MSG_NO_TEMP_INVEN_CTRL_RESULT	123		// ��� ��Ŷ

// Item Socket 2004.11.4->hotblood insert
#define MSG_NO_ITEM_EQUIP_SOCKET		125		// �����ۼ��Ͽ� ��Ĺ��� �ִ´�.   
#define MSG_NO_ITEM_EQUIP_SOCKET_RESULT 125
//#define MSG_NO_ITEM_ON_LAND				125		// �ٴڿ� �ִ� �����۵��� ������ �˷��ش�.

#define MSG_NO_ITEM_REMOVE				126		// �ٴڿ� �ִ� �������� �����϶�.

// 2004.06.18->oneway48 insert
#define MSG_NO_NPC_ITEM_REPAIR_REQUEST	132		// ������ ���� ��û ��Ŷ
#define MSG_NO_NPC_ITEM_REPAIR_RESULT	132		// ������ ���� ��û ��Ŷ

#define MSG_NO_PERIOD_ITEM_BASE			143		// �Ⱓ�� ������ ���� 

// Item broadcasting
#define MSG_NO_WORLD_ITEM_REQUEST		144		// ������ ��ε�ĳ��Ʈ ��û ��Ŷ
#define MSG_NO_WORLD_ITEM_INFO			144		// �ֺ� �������� ��Ȳ�� �����ִ� ��Ŷ

// �κ��丮 ����
#define MSG_NO_INVEN_CTRL				145		// �κ��͸� ����.
#define MSG_NO_INVEN_REQ				146		// �κ��丮 ��û

#define MSG_NO_BLOODPOINT_CTRL			127		// ���� ����Ʈ ���� ��Ŷ

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/* =====================================================================================
	ĳ������ �κ��丮 ����.

  #define MSG_NO_INVEN_INFO				// ĳ������ �κ��丮 ���� 
===================================================================================== */
/*
struct MSG_INVEN_INFO
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	unsigned int		uiMoney ;			// ��
	_sUser_InvenItem	Item[_TOTAL_INVENTORY_] ;			// 
} ;
*/
// insert end
// ���� 
struct Elixir_Data
{
	struct _Data
	{
		short __Data[10]; 
		char  __Grade;	
	};
	_Data Data[5];	// ���Ǻ� ������. 
					// 0 �ٷ� 
					// 1 ���� 
					// 2 ����			
					// 3 �ǰ� 
					// 4 ��ø 
					// ���� ���� �Ծ��� ����� �����ʹ� ��� �� ���ΰ�???
	
	short _sIntoxication; // �ߵ� ��ġ.
	char  Temp[3];  // �������.
};

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
struct MSG_INVEN_INFO
{
	_sHeader header;
	u_char ucMessage;
	u_char ucInvenMode;
};
/*
0 - Elixir
1 - Inventory : Pocket, Backpack, Money, Inven
2 - Cash Inventory : Cash backpack & Cash Inventory.
*/

enum EN_INVEN_INFO_MODE
{
	en_inven_info_mode_elixir=0,
	en_inven_info_mode_inventory=1
};

struct MSG_ELIXIR_DATA : public MSG_INVEN_INFO
{
	Elixir_Data         E_Data;
};
 
struct MSG_INVENTORY : public MSG_INVEN_INFO
{
	char cAmorPocketNum;	// �������� ����
	char cPantsPocketNum;	// �������� ����
	char cBackPackExtend;	// �賶      
	unsigned int uiMoney;	// ��

	u_char ucItemCount;	// �̾ ���ԵǴ� ��� ������ ����.
	char cInvenPack[ sizeof(_sUser_Inven_v1_all) * _V1_COUNT_REAL_INVEN_ ];
};
#else
// 2004. 10.15
struct MSG_INVEN_INFO
{
	_sHeader			header ;
	u_char				ucMessage ;
	u_char              ucItemCount;
	char		        cAmorPocketNum;			// �������� ����
	char			    cPantsPocketNum;		// �������� ����	
	char                cBackPackExtend;         // �賶      
	unsigned int		uiMoney ;				 // ��
	Elixir_Data         E_Data  ;                // ���� ������
	
	char				cInvenPack[ sizeof(_sUser_Inven_v1_all) * _V1_COUNT_REAL_INVEN_ ] ;
};
#endif
/* =====================================================================================
������ â�� ����. 

  #define MSG_NO_STORAGE_INFO			// ������ â��.
===================================================================================== */
// ������ ���丮�� ��� ��û�� ������ �������� �����ϴ� ��Ŷ. 
// 2004.05.20->oneway48 modify
struct MSG_STR_ITEM_INFO
{
	_sHeader			header ;
	u_char				ucMessage ;
	u_char              ucSavedItem;  // �� ����� ������ ����.
	u_char              ucTotalSlot;  // �� ���� ���� ����.
	u_char              ucLastItemIdx ;	// ������ â���� �������� �ִ� ������ �ε��� 

	WORD                wYear   ;
	u_char              ucMonth ;
	u_char              ucDay   ;
	u_char              ucHour  ;
  
	unsigned int		uiMoney ;     // ��
	char				cStorage[sizeof(_sUser_Inven_v1_all)*_TOTAL_STORAGE_] ;
};

// modify end

/* =====================================================================================
	�κ��丮 �������� ��ġ �̵�:����,Ż��,�賶����,�κ����� �� ��� ó��. 
	��û�� ������ ������ ���(������ ���) �����ϰ�, ����(��ҽ�Ŵ)�� cTo == -1

    ��, ������ ������ ���( ����Ű�� ��Ÿ ������� ó���ȴ�.) 
	cFrom�� cTo�� �����ϰ� �����Ѵ�. ��. 12 �� ������ ���⸦ �����ϰ� ���� ���
	cFrom = 12, cTo = 12 �� ������ ������ ����. 

  #define MSG_NO_INVEN_MOVE						// �κ��丮 �������� ��ġ �̵�:����,Ż��,�賶����,�κ����� �� ��� ó��. 
===================================================================================== */
struct MSG_INVEN_MOVE
{
	_sHeader			header ;
	u_char				ucMessage ;

	char				cAmorPocket;
	char				cPantsPocket;
	char                cBackPackExtend;// ���Ѱ�
	char				cFlag;
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    u_char				cFrom ;
	u_char				cTo ;
#else
	char				cFrom ;
	char				cTo ;
#endif
	u_char				ucSendCount;
} ;

/* =====================================================================================
	�ֿ� ���� �Ʒ� ��Ŷ���� ������ ��û�ϰ� ����� Grab_Success ��Ŷ���� �޴´�. 
	�ٸ� �������Դ� Grab_Success ��ſ� ������ ���� ��Ŷ(Item_Remove)�� ����. 

  #define MSG_NO_ITEM_GRAB_REQ					// ������ �Ա�
===================================================================================== */
struct MSG_ITEM_GRAB_REQ
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	float               fX; // 2004.06.03->oneway48 insert
	float               fZ;	

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    u_char				cTo ;					// �κ��丮 ���� ���� ���� ���ΰ�.
#else
	char				cTo ;					// �κ��丮 ���� ���� ���� ���ΰ�.
#endif
	int					iItemWorldID ;			// �������� ���� ���̵�
} ;


/* =====================================================================================

  #define MSG_NO_ITEM_GRAB_SUCCESS				// ������ �Ա� ����/���� : ����ڿ��Ը� ����. �ٸ� �������Դ� 
===================================================================================== */
struct MSG_ITEM_GRAB_SUCCESS
{
	_sHeader			header ;
	u_char				ucMessage ;
	
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    u_char				cInvenSlot ;			// �κ��丮 �ε���. 255�̸� ����, 254�� ���� ����. 
#else
	char				cInvenSlot ;			// �κ��丮 �ε���. -1�̸� ����, -2�� ���� ����. 
#endif
	int					ItemWorldID ;			// �������� ���� ���̵�
	_sUser_Inven_v1_all ItemInfo ;
	
	char				cCount ;				//
	_sitem_pair			result[_V1_INVEN_EXC_EQUIP_] ;			// ���
} ;



/* =====================================================================================
	���� �Ծ���. ���� ������Ʈ

  #define MSG_NO_MONEY_GRAB_SUCCESS			// ���� �Ծ���.
===================================================================================== */
struct MSG_MONEY_GRAB_SUCCESS
{
	_sHeader			header ;
	u_char				ucMessage ;

	int					iItemWorldID ;
	u_int				uiMoney ;
} ;

	
/* =====================================================================================
	�������� �������� �� �� �������� �������� �κ���ȣ�� ä���� ������ ������.
	
	��û�� ����� �����ϴ�. 
	������ ������ ��Ŷ�̰�, ����Ŀ�� �������� ��� ���� �������� uiCount �� ���� ����. 
	���д� cFrom = -1 �̴�. 
	�� ��Ŷ�� MSG_INVEN_MOVE �� ����, �ڽſ��Ը� ����, �κ����ۿ��� ���δ�. 
	�����ϰ� �Ǹ�, ITEM_DROP �޼����� ����. 
	
  #define MSG_NO_INVEN_DROP					// ������ �ٴڿ� ����. ( ���� )
===================================================================================== */
struct MSG_INVEN_DROP
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char                cBackPackExtend;		// ���Ѱ�.
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	u_char				cInvenIndex;		// �������� �������� �κ��丮 �ε���
#else
    char				cInvenIndex;		// �������� �������� �κ��丮 �ε���
#endif
	unsigned int		uiCount;			// �������� ���� 
} ;


/* =====================================================================================
	���Ͱ� ����߸��� �����۰� ������ ������ �������� �ٴڿ� ������ ��, 
	�� �޽����� ������ �˷�����. �������� ������ ���� _sItem_Drop �� �߷��� ���Ƿ�
	���̰� ���Ѵ�. 
	
  #define MSG_NO_ITEM_DROP					// ������ �ٴڿ� ����. ( ���� )
===================================================================================== */
struct MSG_ITEM_DROP 
{
	_sHeader		header ;
	u_char			ucMessage ;
	
	char			cNoItems ;					// �������� �������� ����. ���߿� ������ ���ϱ� �ڸ��� �ʰ� ������. 

	u_short			usUniqueID ;					// ����ġ �ְ� ����
	
	_sItem_Drop		items[14] ;					// ���ʹ� �ִ� 8���� �������� ������ �����Ƿ�..
} ;


/* =====================================================================================
	���� ���� ��Ŷ : ����ũ ���̵� �ش��ϴ� ĳ������ ������ ����Ǿ���. (�ڽ��̸� ����)
	���Ⱑ ����Ǿ��ٸ�, 
	Index = _EQUIP_POS_WEAPON_1_ 
	id = ���� ���̵�, 
	�� ���� �ȴ�. 
	
	Index �� ������ �ϳ��� ���̴�. 
	#define _EQUIP_POS_AMOR_		0		// ����		: ���� - ����,   �� - ����.
	#define _EQUIP_POS_PANTS_		1		// ����		: ���� - ����,   �� - ����.
	#define _EQUIP_POS_CAP_			2		// ����		: ���� - ����,   �� - ����
	#define _EQUIP_POS_ADAE_		3		// �ƴ�		: ���� - ����,   �� - ����
	#define _EQUIP_POS_SHOES_		4		// �Ź�		: ���� - ����,   �� - ����
	#define _EQUIP_POS_EARRING_		7		// �ͺκ�	: ���� - ����,   �� - ����.
	#define _EQUIP_POS_BACKPACK_	11		// �賶     : ���� - ����,   �� - ����.
	#define _EQUIP_POS_WEAPON_1_	12		// ����� �̰�����

	#define MSG_NO_EQUIP_CHANGE				
===================================================================================== */
struct MSG_EQUIP_CHANGE
{
	_sHeader		header ;
	u_char			ucMessage ;

	u_short			usUniqueID ;

	char			cIndex ;	
	char            cType; // ������ Ÿ��

	short			sID ;
	u_char			ucStrength;		// ������ ��� ��ȿ
};


//////////////////////////////////////////////////////////////////////////
// �� ��������
// MSG_NO_MONEY_DROP
struct MSG_MONEY_DROP
{
	_sHeader		header ;
	u_char			ucMessage ;		//

	bool			bSuccess ;		// ��û�϶��� ����, �����϶��� ����(true)����(false)
	u_int			uiCount ;		// �������� �� : �����϶� �� ���� �ڱ� �κ��丮���� �����Ѵ�.
} ;


//=================================================================================
// 2003/04/23 �߰� 
// #define MSG_NO_ITEM_ON_LAND				// �ٴڿ� �ִ� �����۵��� ������ �˷��ش�.	
struct MSG_ITEM_ON_LAND
{
	_sHeader		header ;
	u_char			ucMessage ;

	char			cNoItems ;					// �� ��Ŷ�� �߶� ������. ����.. Ŭ���̾�Ʈ �Ľ� ����.

	_sItem_Drop		items[50] ;				// �ٴ� �������� 100���� �Ѿ�� �̾ ��� ������.
} ;


//=================================================================================
// �ٴڿ��� id �� �ش��ϴ� �������� �����϶�. 
// #define MSG_NO_ITEM_REMOVE				// �ٴڿ� �ִ� �������� �����϶�.
struct MSG_ITEM_REMOVE
{
	_sHeader		header ;
	u_char			ucMessage ;

	int				iItemWorldID ;				// �����ؾ� �� �������� ���̵�. 
} ;


//================================== HSS Added.. 20030721
//#define MSG_NO_USE_ITEM_REQ				101		// ������ ����û.
struct MSG_USE_ITEM_REQ	
{
	_sHeader			header ;
	u_char				ucMessage ;
	u_char				ucInvenNumber;
	char                cIsLpsKey;   // LPS Ű �ΰ� : 0 �� ��� �Ϲ� ������ ��¡. 1�� ��� LPS�� ���� ������ ��¡.

	char				cIsFail;	// ���� ����ÿ� fail�� ó���� �Ǿ���ϴ���.... 1�ϰ�� 0 �ϰ�� ����ó����.

	unsigned short      sDNPCID  ;   // DNPCID
};

//	#define MSG_NO_USE_ITEM_RESULT				102		// ������ ����� 
struct MSG_USE_ITEM_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    u_char				ucResult;				// ���� or ���� & ���� ����. // 2004.06.09->oneway48 insert
												// -1 �� ��� �Ѿ�� �������� �̻��� ���.
												// -2 �� ��� �� �ٿ� �ð��� �ȸ´� ���̴� .	
#else
	char				cResult;				// ���� or ���� & ���� ����. // 2004.06.09->oneway48 insert
												// -1 �� ��� �Ѿ�� �������� �̻��� ���.
												// -2 �� ��� �� �ٿ� �ð��� �ȸ´� ���̴� .	
#endif
	u_char				ucInvenIndex ;			// �Ҹ��� �������� �κ���ȣ 
	u_char				ucCount;				// ���� �����ִ� ���� 0���� �ش� �κ��丮 ������ ���� 
	
												//	�߰� : ����Ʈ���� cResult ���� - �ϰ�� 
												//		�����ڵ� 
												//		
												//		-1 ������ �κ��͸� �ε����� ���� �ε����ų� ���� �������� ũ��. 
												//		-2 ������ �κ��͸� �ε����� �������� ����.
												//		-3 �������� ������ �̻��ϴ�. 

};

//#define MSG_NO_ELIXIR_USE_RESULT		231		// ���� ��� ���
struct MSG_ELIXIR_USE_RESULT
{
	_sHeader			header      ;			
	u_char				ucMessage   ;				
	Elixir_Data         E_Data      ;
};

//#define MSG_NO_ELIXIR_INTOX_TIME            113                         
struct MSG_ELIXIR_INTOX_TIME
{
	_sHeader            header      ;
	u_char              ucMessage   ;
	short               sElixirIntox; // ��              
};

// 2005.02.23->hotblood insert ------------------------------------------------------------------=
// ���ǳ� ���ǰ� ������ ��  Ŭ���̾�Ʈ�� �κ��� ����� ���� �����ϱ� ���ؼ� ���� ����ü�̴�. 
#define  _TEMP_INVEN_MAX_COUNT_ 8

struct _sindex_pair_
{
	u_char        cAccessoryIdx;
	u_char        cInvenIdx;
};
 
struct _sAccessoryItem
{
	char				cSecond ;   // �ߺз�.
	short				sID     ;	// ������ �Һз�. (������ ���̵�)
};

#ifdef _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI

struct MSG_TEMP_INVEN 
{
	_sHeader			header;
	u_char				ucMessage;
	char				cTo;
};

struct MSG_TEMP_INVEN_MOVE : public MSG_TEMP_INVEN
{
	// cTo == 0
	_sindex_pair_		IndexPair[_TEMP_INVEN_MAX_COUNT_];	// 0�� ��� �̰����� ....
};

struct MSG_TEMP_INVEN_INFO : public MSG_TEMP_INVEN
{
	// cTo == 1
	_sUser_Inven_v1_all	TInven[_TEMP_INVEN_MAX_COUNT_];		//1�� ��� �̰����� ....
};

#else

struct MSG_TEMP_INVEN 
{
	_sHeader           header      ;
	u_char             ucMessage   ; 
	char               cTo         ; // 0 �� ��� �κ����� �� �� .
					 // 1 �� ��� ���� �κ����� �� ���̴�. 
	union 
	{
		_sindex_pair_      IndexPair[_TEMP_INVEN_MAX_COUNT_]; // 0�� ��� �̰����� ....
		_sAccessoryItem	   TInven[_TEMP_INVEN_MAX_COUNT_];   // 1�� ��� �̰����� ....
	};			
};

#endif

// #define MSG_NO_TEMP_INVEN_CTRL          123		// ���� �κ����� ���� �κ����� �ű涧
struct MSG_TEMP_INVEN_CTRL
{
	_sHeader           header      ;
	u_char             ucMessage   ;
	char               cTIdx       ; // ������ �ϴ� �������� Temp �κ��͸� �ε���.
	char               cWhere      ; // 0 �̸� �κ����� 1�̸� ������( �� ���ִ� �� )
};

// #define MSG_NO_TEMP_INVEN_CTRL_RESULT	123		// ��� ��Ŷ
struct MSG_TEMP_INVEN_CTRL_RESULT 
{
	_sHeader           header      ;
	u_char             ucMessage   ;
	char               cTIdx       ; // ���� ������ Temp �κ��͸� �ε���. 
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	u_char               cInvenIdx   ; // ���� �Ǵ� �κ��� �ε���. ��� ���� ���⼭... ������ ���. ���з�....
#else
	char               cInvenIdx   ; // ���� �Ǵ� �κ��� �ε���. ��� ���� ���⼭... ������ ���. ���з�....
#endif
};
// ----------------------------------------------------------------------------------------------=

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 16 : 128 ~ 135
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifdef STORAGE_TEST
//#define MSG_NO_STR_RESERVE_SLOT        128  
#endif

#define MSG_NO_STR_BUY_SLOT			    128  
#define MSG_NO_STR_BUY_SLOT_RESULT      128      

#define MSG_NO_STR_BUY_SLOT_CANCEL        147
#define MSG_NO_STR_BUY_SLOT_CANCEL_RESULT 147
#define	MSG_NO_TEMP_INVEN				148		// ��.���ǰ� �ı������� ���� ��Ŷ
#define MSG_NO_STR_BUY_SLOT_RELEASE     149

// 2004.06.02->oneway48 modify
#define MSG_NO_STR_ITEM_EJECTEX        150 
#define MSG_NO_STR_ITEM_EJECTEX_RESULT 150
// modify end

#define MSG_NO_PVP_TRADE_CANCEL        129      // Ʈ���̵� ĵ��.                                          
#define MSG_NO_NPC_TRADE_REQ		   130	    // NPC���� ��ų� �ȶ�, (������ �ൿ�� �Ͼ ��)
#define MSG_NO_NPC_TRADE_RESULT		   MSG_NO_NPC_TRADE_REQ //130 NPC �ŷ� ���.

// â�� ���� HHH
#define MSG_NO_STR_ITEM_REQ            133		// â�� �ڽ��� ������ ��û��. c -> s
#define MSG_NO_STORAGE_INFO		    MSG_NO_STR_ITEM_REQ // â�� �ڽ��� �����û�� ���� ������ ����.s -> c 


#define MSG_NO_STR_ITEM_SAVE           134		// �κ����� ���丮���� �������� ������.
#define MSG_NO_STR_ITEM_SAVE_RESULT    134      // �κ����� ���丮���� �������� �����û�ÿ� ������ ����.

#define MSG_NO_STR_ITEM_EJECT          135		// ���丮������ �κ����� �������� ������.
#define MSG_NO_STR_ITEM_EJECT_RESULT   135    
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
struct _sUser_InvenItem	;
struct _tradeitem ;
/*
#ifdef STORAGE_TEST // ���丮�� �׽�Ʈ 
struct MSG_STR_RESERVE_SLOT
{
_sHeader			header  ;
u_char				ucMessage ;
char				cReserveType;
};
#endif
*/
// 2004.05.20->oneway48 insert
struct MSG_STR_BUY_SLOT
{
	_sHeader			header  ;
	u_char				ucMessage ;
	u_char				ucReserveType;
	u_int               uiPayMoney;
};

// 2004.05.22->oneway48 modify
struct MSG_STR_BUY_SLOT_RESULT
{
	_sHeader            header;
	u_char              ucMessage; 
	u_char              ucTotalUsableSlot; // �� �� �ִ� ���� �� ����.
	short				sCheckYear   ;
	u_char				ucCheckMonth ;
	u_char				ucCheckDay   ;
	u_char				ucCheckHour  ;    
	u_int               uiMoney;		   // ������ .
};
// modify end

struct MSG_STR_BUY_SLOT_CANCEL
{
	_sHeader            header;
	u_char              ucMessage;
	u_int               uiMoney;		   // ���� �ִ� �� ��ü.
};
//insert end

// ������ ���丮�� ����� ��û�� �� ����Ѵ�. 
struct MSG_STR_ITEM_REQ
{
	_sHeader			header    ;
	u_char				ucMessage ;
}; //  �� �ʿ��� ���� �ִ°�?

typedef MSG_STR_ITEM_REQ  MSG_STR_BUY_SLOT_RELEASE;// 2004.06.01->oneway48 insert : â�� ��� ����


// �κ����� ���丮���� �ִ� ���� ��û�ϴ� ��Ŷ.
struct _SM_Storage_Inven;



struct MSG_STR_ITEM_SAVE
{
	_sHeader			header     ;
	u_char				ucMessage  ;
	char                cItemNum   ;                 // ���� ��û�ϴ� ������ ����.   
	_SM_Storage_Inven   Storage[20];				 // ���丮���� �ѹ��� �� �� �ִ� ����.
	unsigned int		uiMoney    ;		
};

typedef MSG_STR_ITEM_SAVE  MSG_STR_ITEM_SAVE_RESULT;

// 2004.06.02->oneway48 insert
struct  MSG_STR_ITEM_EJECTEX
{
	_sHeader			header     ;
	u_char				ucMessage  ;
	char                cItemNum   ;                  //    
	_SM_Storage_Inven   Storage[20];                  // �ٽ� ���ư����� �����κ��� �ε����� �����Ѵ�. 
	unsigned int		uiMoney    ;				  // USER ��ü ��
};

typedef MSG_STR_ITEM_EJECTEX  MSG_STR_ITEM_EJECTEX_RESULT;
// insert end

// ���丮������ ������ �κ����� �ִ� ���� ��û�ϴ� ��Ŷ. 
struct MSG_STR_ITEM_EJECT
{
	_sHeader			header     ;
	u_char				ucMessage  ;
	char                cItemNum   ;                 // ���� ��û�ϴ� ������ ����.   
	_SM_Storage_Inven   Storage[20];				 // �ٽ� ���ư����� �����κ��� �ε����� �����Ѵ�. 
	unsigned int		uiMoney    ;		
};

typedef MSG_STR_ITEM_EJECT MSG_STR_ITEM_EJECT_RESULT;

/* =====================================================================================
		 NPC���� �� �� bBuyFromNPC = 1
		: sNPCID�� cItemNo�� NPC�� �ش��ϴ� �����̰�, cItemNo�� cItemType, sItemID�� ������ ���δ�.

		 NPC���� �� �� bBuyFromNPC = 0
		: �̶� cItemNo�� ĳ������ �κ��丮 �ε����̰�, �������� Type,ID �� ���̰�, 
		uiNeedMoney �� ������ ���δ�. 
  
		 NPC���� ���� �⿩�� �� bBuyFromNPC = 2
		�⿩�� ���� ������ ��.
	
		 NPC���� ġ�Ḧ ���� �� bBuyFromNPC = 5 ( �̶��� ucCount�� uiNeedMoney �� ����. )
		ucCount = 1 �� ������ �����ϰ�, uiNeedMoney ���� �ʿ��ѵ��� ����.
		uiNeedMoney �� �ܻ�� ������ ���� �� ���̴�. 
		Trade_Result �� ����. 

  #define MSG_NO_NPC_TRADE_REQ			82		// NPC���� ��ų� �ȶ�, (������ �ൿ�� �Ͼ ��)
===================================================================================== */

enum EN_NPC_TRADE_MODE
{
	en_npc_trade_mode_sell = 0,					//npc���� �ȶ�
	en_npc_trade_mode_buy = 1,					//npc���Լ� �춧
	en_npc_trade_mode_contribute_moonpa = 2,	//npc���� ���� �⿩
	en_npc_trade_mode_heal = 5,					//npc�κ��� ġ��
	en_npc_trade_mode_gamble = 6,				//npc���� ����
	en_npc_trade_mode_move_village = 7,			//np���� ǥ���̵� ��û
	en_npc_trade_mode_buy_contribute = 8,		//npc���� ���ı⿩���� ���
};

struct MSG_NPC_TRADE_REQ
{
	_sHeader			header      ;
	u_char				ucMessage   ;
	char				cBuyFromNPC ;	// EN_NPC_TRADE_MODE

	u_short				usNPCID     ;	// NPC ���̵�
	u_char				ucItemNo     ;	// NPC���� �춧�� NPC �� ���� ������ ����Ʈ�� ����	
										// NPC���� �ȶ��� ������ �κ��丮 ��ȣ. ���� �⿩�������� ���� �κ��丮 ��ȣ.

	char				cItemType   ;	// ������
	short				sItemID     ;	// ������ 
	u_char				ucCount     ;	// ����

	unsigned int		uiNeedMoney ;	// NPC���� �춧�� �ʿ��� ��, NPC���� �ȶ��� ����.
} ;

/* =====================================================================================
	NPC �ŷ��� ���. 
	�ŷ��� ��쿡��, �κ��丮 ������ ������ �������� �Ѵ�. 
	������ ���, �κ��丮 ���԰� ���� �������� ������ ������ �״�� ������Ʈ�Ѵ�.

  #define MSG_NO_NPC_TRADE_RESULT			83		// NPC �ŷ� ���.	
===================================================================================== */
struct MSG_NPC_TRADE_RESULT
{
	_sHeader			header      ;
	u_char				ucMessage   ;

	char                cResult;		// 0 : ��� �ȱ�
										// 1 : ���� �⿩��
										// 2 : ���� �⿩���� ��� ����
										// 5 : ġ�� ����
										// -1 : ġ�� ����
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	u_char				cInvenIndex ;	// ������ ��� 255, ������ ���� ���� �κ��丮 �ε���. 225�� ���� ���ı⿩�� ���, Result�� 5,-1 �̸� ����.
                                        // 253 �̸�, �ŷ��� �� ���� ������.
                                        // 254 : ������ Ʋ��.
                                        // 252 : �����̻�.
                                        // 245 : ���ı⿩�� ���ڸ�.
                                        // 244 : ���ı⿩���� Ʋ��. 
#else
	char				cInvenIndex ;	// -1 : ����
										// �����̸� �κ� �ε���
										// 127 : ���� �⿩�� ���
										// -11 : ���� �⿩�� ���ڶ�
										// -12 : ���� �⿩���� �ٸ�
#endif

	_sUser_Inven_v1_all	Item		;

	unsigned int		uiLeftMoney ;	// �ŷ������ ���� ���� ��. NPC���Լ� ���ı⿩���� ���� ��.  ���� �⿩�� �� ��.
} ;


// ������ ������ �ӽ� ��Ŷ. : ��û ��Ŷ�� ��� ��Ŷ ��� ���� ����.
//#define MSG_NO_NPC_ITEM_REPAIR_REQUEST		132		// ������ ���� ��û ��Ŷ
//#define MSG_NO_NPC_ITEM_REPAIR_RESULT			132		// ������ ���� ��û ��Ŷ
struct MSG_NPC_ITEM_REPAIR_REQUEST
{
	_sHeader			header      ;			// ���� = 11 + cRepairCount 
	u_char				ucMessage   ;
	
	u_short				usNPCID ;				// ��û�ϰ� �ִ� NPC ���̵�.
	char                cMode;                  // mode�� 0�� ��� ������ ���� ���ۿ�û . // mode�� 1�� ��� ������ ���� ��û.

	unsigned int		uiNeedMoney ;			// ������ : ������ ���߱� ���ؼ�.
	
	u_char				ucRepairCount ;			// �����Ϸ��� �������� ����
	
	u_char				ucInvenIndex[_V1_COUNT_REAL_INVEN_] ;			// �����Ϸ��� �������� ���� ��ȣ.
} ;

struct MSG_NPC_ITEM_REPAIR_RESULT
{
	_sHeader			header      ;			// ���� = 8 + sizeof(__Repair_Result) * cRepairCount ;
	u_char				ucMessage   ;

	char                cMode       ;           // mode�� 0�� ��� ������ ���� ����.
	
	unsigned int		uiLeftMoney ;			// ���� ��.

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	u_char				ucRepairCount ;			// �����Ϸ��� �������� ����
#else
	char				cRepairCount ;			// �����Ϸ��� �������� ����
#endif
	
	struct __Repair_Result
	{ 
		u_char				ucInvenIndex ;			// ������ ��� -1, ������ ���� ���� �κ��丮 �ε���.
		u_short				usCurDurability ;
		u_short				usMaxDurability ;
	} _S_Repair_Result[_V1_COUNT_REAL_INVEN_] ;
} ;

/* =====================================================================================
//#define _EQUIP_POS_AMOR_		0		// ����		: ���� - ����,   �� - ����.
//#define _EQUIP_POS_PANTS_		1		// ����		: ���� - ����,   �� - ����.
//#define _EQUIP_POS_CAP_		2		// ����		: ���� - ����,   �� - ����
//#define _EQUIP_POS_ADAE_		3		// �ƴ�		: ���� - ����,   �� - ����
//#define _EQUIP_POS_SHOES_		4		// �Ź�		: ���� - ����,   �� - ����
//
//#define _EQUIP_POS_GAP_UPPER_	5		// ���� ��  : ���� - �Ⱥ���, �� - �Ⱥ���
//#define _EQUIP_POS_GAP_LOWER_	6		// ���� ��  : ���� - �Ⱥ���, �� - �Ⱥ���
//
//#define _EQUIP_POS_EARRING_	7		// �ͺκ�	: ���� - ����,   �� - ����.
//#define _EQUIP_POS_RING_A_	8		// ���� 1   : ���� - �Ⱥ���, �� - �Ⱥ���.
//#define _EQUIP_POS_RING_B_	9		// ���� 1   : ���� - �Ⱥ���, �� - �Ⱥ���.
//#define _EQUIP_POS_NECKLACE_	10		// ��κ�   : ���� - �Ⱥ���, �� - �Ⱥ���.
//#define _EQUIP_POS_BACKPACK_	11		// �賶     : ���� - ����,   �� - ����.
//
//// ==> ���õ� ����� �ε����� ���� �ִ´�. ���� 0, 1, 2�� �����Ѵ�. 
//#define _EQUIP_POS_WEAPON_1_	12		// ����1 : ���� - ����, �� - ����
//#define _EQUIP_POS_WEAPON_2_	13		// ����2 : ��� �ǻ��� �ƴϴ�. ����!
//#define _EQUIP_POS_WEAPON_3_	14		// ����3 : ��� �ǻ��� �ƴϴ�. ����!
//#define _EQUIP_POS_MOUSE_		15		// ���콺 �����Ϳ� ���� ������.
===================================================================================== */







//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ĳ���Ͱ� ������ �ŷ� : PvP   17 : 136 ~ 143
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define MSG_NO_PVP_TRADE_REQ			136		// �Ǿ��� ������ �ŷ� ��û/����/���... ���.
#define MSG_NO_PVP_TRADE_START			137		// �� ĳ������ �ŷ� ���� : �̰� ������ �ŷ�â�� ���� �Ѵ�. 
#define MSG_NO_PVP_TRADE_ADD			138		// �ŷ� ����Ʈ�� �������� �÷� ���´�.
#define MSG_NO_PVP_TRADE_DELETE			139		// �ŷ� ����Ʈ���� �������� �����´�.
#define MSG_NO_PVP_TRADE_UPDATE_INFO	140		// ������ �ø� �����۵��� ����.
#define MSG_NO_PVP_TRADE_MONEY          141     // �ŷ� ��Ͽ��� ���� �������� ���� ���. 
#define MSG_NO_PVP_TRADE_END            142     // �ŷ��� ������ �˸�. ��������.




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=================================================================================
#define SUB_MSG_TRADE_REQEST			0		// �ŷ� ��û
#define SUB_MSG_TRADE_REJECT			1		// �ŷ� ��û ���� �Ǵ� ��û ���
#define SUB_MSG_TRADE_ALLOW				2		// �ŷ� ��û ����
#define SUB_MSG_TRADE_CONFIRM			3		// 'Ȯ��' ��ư Ŭ��
#define SUB_MSG_TRADE_CANCEL			4		// '���' ��ư Ŭ�� �Ǵ� ������ ���. ���� ������ �ŷ� �ʱ�ȭ.

// ���������� ������ �޽���.
//#define SUB_MSG_TRADE_COMPLETE			5		// �ŷ� ����. ( ������ �˾Ƽ� ������ ��Ŷ, Ŭ���̾�Ʈ�� ������ ���� )
#define SUB_MSG_TRADE_BUSY				6		// ������ �ŷ����̴�.
#define SUB_MSG_TRADE_UNKNOWN			7       // ������ ���ų� ������ Ÿ�� ���̵� �̻��ϴ�.
#define SUB_MSG_TRADE_SELFREJECT        8       // ��밡�� Ʈ���̵带 ��û�� �� ������ �ع��ȴ�.

// ������ �ŷ� : PvP A->B���� ��û�� ��.
// �ŷ� ��û��		   A->Server		: cSub = REQUEST,iTargetID = B.
// �ŷ� ��û��Ŷ		: Server->B		: cSub = REQUEST,iTargetID = A.
// �ŷ� ��û ��� �� : B->Server		: cSub = ALLOW,	 iTargetID = A.
// �ŷ� ��û ���� :	   B->Server		: cSub = REJECT, iTargetID = A.  => A�� ��û�� �ŷ� ����.
//						  Server->A		: cSub = REJECT, iTargetID = B.  
// �ŷ� ��û ��� :    A->Server		: cSub = REJECT, iTargetID = B.  => B���� ��û�� �ŷ� ���.
// �ŷ� �� ��� : (AorB)->Server		: cSub = CANCEL, iTargetID = (A or B) ��Ҹ� ���� ��.
// ���� �ŷ����϶� �� ��Ŷ�� ������(bOK�� true�� false�̰�) ������ �ŷ� ���.
// �ŷ����� �� : bOK = �� true �� ���� ������, ������ 'Ȯ��' ��ư�� Ŭ���� ��.
//=================================================================================
//#define MSG_NO_PVP_TRADE_REQ			96	// �Ǿ��� ������ �ŷ� ��û
struct MSG_PVP_TRADE_REQ
{
	_sHeader			header    ;
	u_char				ucMessage ;
	char				cSub      ;			// ��Ȳ�� ���� �ǹ̰� �ٸ�.
	u_short 			iTargetID ;			// �ŷ��� ��û�Ϸ��� ����� ���̵�
} ;

struct MSG_PVP_TRADE_CANCEL
{
	_sHeader			header    ;
	u_char				ucMessage ;
	//char				 cSub      ;		// ��Ȳ�� ���� �ǹ̰� �ٸ�.
	unsigned int		uiMoney   ;		   // �������� ��.
	u_short 			iTargetID ;			
	_sitem_pair         result[10];        // ����� ����.   
} ;



//=================================================================================
// �ŷ� �㰡�� ���ÿ� �ŷ� ���� : ��û�� ������ ��� �������� ��������.
//#define MSG_NO_PVP_TRADE_START		97		

struct MSG_PVP_TRADE_START
{
	_sHeader			header     ;
	u_char				ucMessage  ;
	u_short				iReqID     ;			// ��û�� ���̵�.
	u_short				iTargetID  ;			// ��� ���̵�.
	WORD                TradeSlotNO;            // Ʈ���̵� ���� �ѹ�. 
} ;

struct MSG_PVP_TRADE_END 
{
	_sHeader            header   ;
	u_char				ucMessage;
	int                 Reason   ;               // reason = 1 ���� �Ϸ�.
	_sitem_pair         User[10] ;	
	unsigned int        uiTotalMoney;
	u_short             iTargetID;											 													 
};
//====================================================================================================//
// �ŷ�â�� �������� �߰��Ѵ�. : �߰��ÿ��� �߰��Ǵ� ������ ������ ����.							  //
// ***********  �� ������ ��Ŷ�� ���̰� �ʹ� ��� ������ ���߿� �� �����ϵ��� ����. **************    //
//====================================================================================================//
/*struct _sitem_pair {
	char			slot ;			// ����
	unsigned short	number ;		// ����
};*/

//=================================================================================
// �ŷ�â�� '������'�� ������ ���õ� �����۵��� ������ �� ��Ŷ�� ��� ������.
// �̶�, Ŭ���̾�Ʈ�� �κ����� ���õ� �����۵��� �ӽ÷� ����, �߰��� �������� ������ 
// �ٽ� �����Ͽ� '������'�� ������, ���� ���õ� �����۵��� ������ �� ��Ŷ�� ��� ������. 
// ���� �߰��� �� ��Ŷ���� ��� ó���Ѵ�.
// ����Ŀ�� �������� ���, number�� �����ϰ� slot ��ȣ�� ��ȿ�Ѵ�. 
// ����Ŀ�� �������� number ��ŭ�� �߰��ϴ� ������ �����Ѵ�. slot = -1 ������ ��ȿ. 
//=================================================================================
// �ڼ��� ������ ��Ŷ����.doc ����.
//#define MSG_NO_PVP_TRADE_ADD			98


// ���� .
/*struct MSG_PVP_TRADE_ADD
{
	_sHeader			header ;		// 2 bytes : size = 127 bytes... 
	char				ucMessage ;		// 3 bytes
	
	unsigned int		uiAddMoney ;	// 	�߰��Ǵ� ��
	
	_sitem_pair			items[40] ;		// ���⿡ ���̴� �ε����� �κ��丮�� �ε���
} ;*/

// ������ Ʈ���̵� ���Կ� �������� ���� �� ������. Sending�� �̷����� �ȴ�. 
struct MSG_PVP_TRADE_ADD
{
	_sHeader		header       ;	// 2 bytes : size = 127 bytes... 
	u_char			ucMessage	 ;	// 3 bytes	
	WORD            SlotNo       ;  // Trade Slot Number.
	_sitem_pair		item         ;	// �ѹ��� �ϳ�����. 
	unsigned char   TWindowIndex ;  // Ʈ���̵� ������ �ε���.  
    char            cResetConf   ;  // Ȯ�� ��ư�� Ŭ���� ���¿���... �ٽ� �ʱ�ȭ �Ǿ���. 
	char            cAddCount    ;  // ������ �ö��ִ� ������ ������ ������ ���� �ø���.  
} ;

// ������ Ʈ���̵� ���Ի󿡼��� ������ ���� ��Ŷ. �������� ���� �ѱ� �ʿ䰡 ����..
struct MSG_PVP_TRADE_DELETE
{
	_sHeader		header       ;	// 2 bytes : size = 127 bytes... 
	u_char			ucMessage    ;	// 3 bytes 	
	u_char          Inven_Index  ;  // Ʈ���̵� �����쿡�� �������� �κ����� ���ư� �������� �κ� �ε���. 
	WORD            SlotNo       ;  // Trade Slot Number
    u_short         ucSubjectID  ;	// ������� �ϴ� ���� ���̵�.
	unsigned char	number       ;	
	unsigned char   TWindowIndex ;  // Ʈ���̵� �������ε���.   
	char            cResetConf   ;  // Ȯ�� ��ư�� Ŭ���� ���¿���... �ٽ� �ʱ�ȭ �Ǿ���. 1�� ���� ��ư�� �����ִ� ��
};
//#define MSG_NO_PVP_TRADE_DELETE		99
//typedef MSG_PVP_TRADE_ADD MSG_PVP_TRADE_DELETE ;

struct MSG_PVP_TRADE_MONEY
{
	_sHeader		header     ;	// 2 bytes : size = 127 bytes... 
	u_char			ucMessage  ;	// 3 bytes	
	WORD            SlotNo     ;    // Trade Slot Number.
	unsigned int	uiMoney	   ;	// �������� ��.
    char            AddOrSub   ;    // �����ΰ� ���� ���ΰ�. 1�̸� ���ϱ� 0�̸� �����. 
	char            cIsOppe    ;    // 1 : ��� 0 : �ڽ� 
} ;
//=================================================================================
// ������ �������� ���� : ������ �������� �ø���, �� ������ �� ��Ŷ���� ������.
// �߰����� ������ �ε����� �����ϰ�, �������� ������ ǥ���ϱ� ���� _sUser_InvenItem ���� �ٲ۴�. 
//
// ���� A �� �ŷ�â�� �������� �ø���, A�� ������ INSERT ��Ŷ���� ������, ������ 
// ���� ��Ŷ���� INSERT_INFO ��Ŷ�� �����Ͽ� B���� ������. cNo -1����.
//=================================================================================
//#define MSG_NO_PVP_TRADE_UPDATE_INFO		100
/*struct MSG_PVP_TRADE_UPDATE_INFO
{
	_sHeader			header ;
	char				ucMessage ;
	unsigned int		uiMoney ;
	_sUser_InvenItem	items[40] ;			// 
}; ����. */


struct MSG_PVP_TRADE_UPDATE_INFO 
{                                
	_sHeader			header  ;
	u_char				ucMessage ;
	_sUser_Inven_v1_all item	;
	unsigned char       TradeWindowSlotNo; // Ʈ���̵� ������ ���� ����
};


//////////////////////////////////////////////////////////////////////////
// 18 ���� ...
//////////////////////////////////////////////////////////////////////////

struct MSG_WORLD_ITEM
{
	_sHeader			header;
	u_char				ucMessage;
	u_char				ucPackNum; // 2004.05.28->oneway48 insert: ��Ŷ ���� �ѹ�
	short				sNearItemNo;
	_sItem_Drop			DropItem[80]; 
};

struct MSG_WORLD_ITEM_REQUEST
{
	_sHeader			header;
	u_char				ucMessage;
};

enum _inven_ctrl_mode
{
	inven_ctrl_item_count=0,	// �������� ������ �ٲ��.
		inven_ctrl_money_set,		// ���� ���� ��.
		inven_ctrl_item_dur_set,	// �������� ��.
} ;

struct MSG_INVEN_CTRL_RESULT
{
	_sHeader        header ;
	u_char          ucMessage ;
	
	char            mode;			// _inven_ctrl_mode �� ����.
	
	union 
	{
		//++++++ �������� ������ �����Ѵ�. ++++++//
		struct {
			char				cIdx ;			//
			u_char              ucCount ;	
			char				padding[2] ; 
		} _S_Item_Count  ;
		
		//++++++ ���� ���� �����Ѵ�. ++++++//
		struct {
			u_int			uiCount ;				// �� ��.
		} _S_Money_Set ;
		
		//++++++ �������� �����Ѵ�. ++++++//
		struct {
			char				cIdx ;			
			u_short				usDurability ;		// ����.
		} _S_Dur_Set ;
		
	};
} ;

// 2004.11.4->hotblood insert
struct MSG_ITEM_EQUIP_SOCKET
{
	_sHeader             header          ;
	u_char               ucMessage       ;
	u_char               ucToolItemIdx    ;
	u_char               ucObjItemIdx     ;
	char                 cAddonCounter   ; // ��� ���� �õ����ΰ�?
	u_char               ucAddonItemIdx[4]; // �ִ� 4�� �̹Ƿ�...
	char                 cMode           ; //  0�� ��� ���Ͽ� ���� 1�� ��� �ڽ� ����. 
};

struct MSG_ITEM_EQUIP_SOCKET_RESULT
{
	_sHeader             header      ;
	u_char               ucMessage   ;
	_sUser_Inven_v1_all  CompleteItem;
	char                 cMode           ; //  0�� ��� ���Ͽ� ���� 1�� ��� �ڽ� ����. 
	
#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
	_sitem_pair			leftItemInfo[4];
#endif
};

//#define MSG_NO_INVEN_REQ       146
struct MSG_INVEN_REQ
{
	_sHeader		header ;
	u_char			ucMessage ;
};

// ���� ------------------------------------------------------------------------------------��
#define MSG_NO_INCHANT_CTRL					124							// ���� ���� ��Ŷ.

struct MSG_INCHANT_PACKET 
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_char				ucMode ;
} ;

// ������, Ŭ���̾�Ʈ���� req �� ������, 
// ���������� ������ üũ�� �ϰ�, start_inchant �� �����ش�.
// 
enum
{
	inchant_mode_str_req=0,		// ���� ��û
	inchant_mode_str_req_fail,	// ���� �������� ��, ������ �Բ�. � ��������. 
							// 0 - �κн���, 1 - ��������.
	inchant_mode_str_start_inchant,	// ������ �����Ѵ�.( Ŭ����Ʈ ǥ�ý��� )
	inchant_mode_str_complete,	// ���� ���÷��̰� �Ϸ���. ������ ������ ó���� ��û�Ѵ�.
	inchant_mode_str_result,	// ���� ���

	//Author : �����  breif : �븰/��� �ý���	
	inchant_mode_protect_add_count_req=40,	// �븰 �ٸ��� ��û c->s
	inchant_mode_protect_add_count_success,	// �븰 �ٸ��� ���� s->c
	inchant_mode_protect_add_count_fail,	// �븰 �ٸ��� ���� s->c
	inchant_mode_protect_inc_str_req=50,	// ��� �ٸ��� ��û c->s
	inchant_mode_protect_inc_str_success,	// ��� �ٸ��� ���� s->c
	inchant_mode_protect_inc_str_fail,	// ��� �ٸ��� ���� s->c
	
	inchant_mode_combine_item_req = 100,	// ���� �ϻ��� ������ : ���� ��û
	inchant_mode_combine_success,			// ����
	inchant_mode_combine_fail,				// ����
} ;

// ��û ��Ŷ : ��� ������ �κ��丮�� ���� ��ȣ��, 
// �ʿ�������� �ִ� ������ ��ȣ��, �ʿ� �������� ������ ä���� ������ ��û�Ѵ�.
struct MSG_INCHANT_STR_REQ : public MSG_INCHANT_PACKET
{
	u_char			cTargetWeaponSlot ;
	u_char			cNeedItemSlot ;
	u_char			ucNeedItemCount ;		// ������.
	u_int			uiNeedMoney ;			// ������
} ;

struct MSG_INCHANT_STR_REQ_FAIL : public MSG_INCHANT_PACKET
{
	u_char			ucFailReason ;
	// 1 not_item_type,		// ������ �� ���� �������̴�.
	// 2 more_need_item,		// ��ᰡ �� �ʿ��ϴ�.
	// 3 more_need_money,		// ���� �� �ʿ��ϴ�.
	// 4 not_match_needs,		// �ʿ� ������ �ٸ���.
	// 5 cannot_more_inchant,	// ���̻� ������ �� ����
	// 6 wait,					// ����Ŀ� �ٽ�.
}
 ;

struct MSG_INCHANT_STR_START_INCHANT : public MSG_INCHANT_PACKET 
{
	u_char			ucResult ;				
	// 0 : ���� : �� ����� ����, ��� ��û ��Ŷ�� ������..
	// 1 : �κ� ���� ( ���� 1 )
	// 2 : ���� ���� ( ���� 2 )
	// 3 : �κ� ���� : �븰�� ���Ǿ� ����� �ı����� ����.
    // 4 : ���� ���� : �븰�� ���Ǿ� ����� �ı����� ����.
} ;


struct MSG_INCHANT_STR_RESULT : public MSG_INCHANT_PACKET
{
	u_char			ucResult ;			// ���� START_INCHANT ��Ŷ�� ����� ����.
	u_char			ucInchantGrade ;	// ��� �������� Strength

	u_char			cSlot ;				// ���н� ������ ���� ���� : ���� ������ �� ���̴�.
	u_char			ucLeftItemCount ;	// ���� ������ ����.
	u_int			uiLeftMoney ;		// ó�� ��� ���� ��.

	// �̿��� ������ ó�� ��Ŷ�� ���� �ʴ´�. ( ������ ���� ��Ŷ �� )
	// ����, ���� ������ 0�̸�, ������ ����.

#ifdef _XDEF_SMELT_ITEM //Author : �����
	u_char	ucLeftProtectCount;	// �븰�� �ٸ� ���� �ִٸ�, ����(1,2���) �ÿ�, -1 �Ͽ� �� ���̴�.
	u_char	ucIncMaxStr;	// ��縦 �ٸ� ���� �ִٸ�, ���������� -1 �Ͽ� �� ���̴�.
#endif
} ;

#ifdef _XDEF_SMELT_ITEM //Author : �����
// inchant_mode_protect_add_count_req
struct MSG_INCHANT_PROTECT_ADD_COUNT_REQ : public MSG_INCHANT_PACKET 
{
	u_char	ucWeaponSlot;	// �븰�� �ٸ����� ������ �κ��丮 ��ġ.( �ݵ�� �κ��丮�� �ű� �� �ϵ��� �˻� )
	u_char	ucProtectItemSlot;	// �븰�� ��ġ ==> ���� �ϳ����� ����Ѵ�.( �����ϳ��� 5, �ٸ� ���Կ� 3�� ������, �ִ�5���� �ٸ� �� �ִ�. )
	u_char	ucUseCount;	// �븰�� �� �� �ٸ� ���ΰ�.
};

// inchant_mode_protect_add_count_success,	// �븰 �ٸ��� ���� s->c
struct MSG_INCHANT_PROTECT_ADD_COUNT_SUCCESS : public MSG_INCHANT_PACKET 
{
	u_char	ucWeaponSlot;	// �븰�� �ٸ� ������ �κ��丮 ��ġ.
    u_char	ucReqProtectCount;	// ��û�� �븰 ȸ��.
    u_char	ucProtectCount;	// ���� �븰�� ȸ��. <== �ش� �����ۿ� ������ ��.
    u_char	ucProtectItemSlot;	// ����� �븰�� ��ġ.
    u_char	ucLeftProtectItem;	// ����ϰ� ���� �븰�� ����( 0 �̸� ������ ��� ��. )
};

// inchant_mode_protect_add_count_fail,	// �븰 �ٸ��� ���� s->c
struct MSG_INCHANT_PROTECT_ADD_COUNT_FAIL : public MSG_INCHANT_PACKET 
{
	u_char	ucWeaponSlot;	// �õ��� ������ �κ��丮 ��ġ
	u_char	ucFailReason;
	/* ucFailReason : 0 �� ���� ����.
				  1 : �븰�� �����ϴ�.
				  2 : ���⿡ ��û�� ��ŭ �ٸ� �� ����.(���̻� �ٸ� ������.)
				  3 : ���Ⱑ �ƴϴ�.
	*/
};

// inchant_mode_protect_inc_str_req=50,	// ��� �ٸ��� ��û c->s
struct MSG_INCHANT_PROTECT_INC_STR_REQ : public MSG_INCHANT_PACKET 
{
	u_char	ucWeaponSlot;	// ��縦 �ٸ����� ������ �κ��丮 ��ġ.( �ݵ�� �κ��丮�� �ű� �� �ϵ��� �˻� )
	u_char	ucIncStrItemSlot;	// ����� ��ġ
};

//          inchant_mode_protect_inc_str_success,                             // ��� �ٸ��� ���� s->c
struct MSG_INCHANT_PROTECT_INC_STR_SUCCESS : public MSG_INCHANT_PACKET 
{
	u_char	ucWeaponSlot;
    u_char	ucWeaponIncStr; // ������ ������ �ִ밭��.(ǥ�� = ����⺻���� + �̰� )
							// ���⿡ �����Ѵ�. ucIncProtect ���� �����ϰ�, ucUsedProtect ���� 1�� �� ��.

	u_char	ucIncStrItemSlot; // ����� ��ġ
	u_char	ucLeftIncStrItemCount;     // ����ϰ� ���� ����� ����. 0�̸� ����.
};

//inchant_mode_protect_inc_str_fail,	// ��� �ٸ��� ���� s->c
struct MSG_INCHANT_PROTECT_INC_STR_FAIL : public MSG_INCHANT_PACKET 
{
	u_char	ucWeaponSlot;	// ��û�ߴ� ������ ����.
	u_char	ucFailReason;	// ���� ����
                                                                                                                                                  
	/*
	1 - ��簡 ����. �Ǵ� ��簡 �ƴϴ�.
	2 - �̹� ��縦 �ٸ����� �ִ�.
	3 - ���� ���� �̻�.
	4 - ��Ÿ... 
	*/
};
#endif
// -----------------------------------------------------------------------------------------��

//inchant_mode__combine_item_req=100,             // ���� �ϻ��� ������ : ���� ��û
struct MSG_COMBINE_ITEM_REQ : public MSG_INCHANT_PACKET
{
	u_char			ucWeaponSlot;		// ���� 
	u_char			ucAddItem1Slot;		// ���� ������ 1
	u_char			ucAddItem2Slot;		// ���� ������ 2

	u_int			uiNeedMoney;		// �ʿ��� ��
};

//inchant_mode__combine_success,               // ���� ����.
struct MSG_COMBINE_SUCCESS : public MSG_INCHANT_PACKET
{
	_sUser_Inven_v1_all		WeaponItem;	// ��� ���� ������

	u_char			ucAddItem1Slot;		// ���� ������ 1
	u_char			ucLeftAddItem1;		// ���� ���� ������ 1 : 0�̸� ���� ����.

	u_char			ucAddItem2Slot;		// ���� ������ 2
	u_char			ucLeftAddItem2;		// ���� ���� ������ 2 : 0�̸� ���� ����.

	unsigned int	uiLeftMoney;		// ���� ��
};

//inchant_mode_combine_fail
struct MSG_COMBINE_FAIL : public MSG_INCHANT_PACKET
{
	u_char			ucFailReason;
	
	/*
		0 : ������ ���� ���� Ʋ��
		1 : ������ ���� �������� ����Ͽ� �ֽʽÿ�
		2 : ������ �� �� ���� ���� ������ �Դϴ�
		3 : ���õ� �������� ������ �� �� �����ϴ�.
		4 : ���տ� �ʿ��� ��ᰡ �ƴմϴ�.
		5 : ���� ���� �������� ���� ����Ͽ� �ֽʽÿ�.
		6 : �̹� ��ᰡ ��ϵǾ� �ֽ��ϴ�.
		7 :	���� �� %s ������ �ʿ��մϴ�. ������ �����Ͻðڽ��ϱ�?
		8 : ������ ����Ͽ����ϴ�.
		9 : ��ᰡ �����մϴ�.
		10 : �ݾ��� �����մϴ�.
	*/
};



// ���� ------------------------------------------------------------------------------------��
enum EN_BLOODPOINT_PACKET_MODE
{
	en_bppk_mode_setting_point = 0,		// ���� ����Ʈ ����
	en_bppk_mode_setting_monster_kill,	// ���� ���� ų��
	en_bppk_mode_request_item,			// �������� �ٲٱ� ��û
	en_bppk_mode_give_item,				// ���� ������ �ֱ�
	en_bppk_mode_error					// ��û ���н� ���� �ڵ�
};

struct MSG_BLOODPOINT_CTRL
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;
};

// mode : en_bppk_mode_setting_point
struct MSG_BLOODPOINT_SETTING_POINT : public MSG_BLOODPOINT_CTRL
{
	short			sPoint;
};

// mode : en_bppk_mode_setting_monster_kill
struct MSG_BLOODPOINT_SETTING_MONSTER_KILL : public MSG_BLOODPOINT_CTRL
{
	short			sMonsterKill;
};

// mode : en_bppk_mode_request_item
struct MSG_BLOODPOINT_REQEUST_ITEM : public MSG_BLOODPOINT_CTRL
{
	u_char			cInsertedSlot;		// �󽽷� �Ǵ� ������ �ִ� ���� : ������ �� �ڸ�.
} ;

// mode : en_bppk_mode_give_item
struct MSG_BLOODPOINT_GIVE_ITEM : public MSG_BLOODPOINT_CTRL
{
	u_char			cInsertedSlot;
	u_char			ucCount;
};

// mode : en_bppk_mode_error 
struct MSG_BLOODPOINT_ERROR : public MSG_BLOODPOINT_CTRL
{
	u_char			ucError;			// 0��������Ʈ�� �����մϴ�. 1�����̻�
};


// �Ⱓ�� ������ ���� 
//#define MSG_NO_PERIOD_ITEM_BASE                                 143
enum EN_PITEM_MODE
{            
	en_pitem_add=0,                                        
	en_pitem_set_period=1,
	en_pitem_remove=2,						// �Ⱓ�� �ٵǾ� �����.
	en_storage_pitem_remove = 3,			// â�� �ִ� �������� ����� �� : â�� ����, â���� ã�� �� ���� �ȴ�.
};

struct MSG_PERIOD_ITEM_BASE   //  : �Ⱓ�� ������ - �Ⱓ����/����/����
{
	_sHeader	header;
	u_char		ucMessage;
	u_char		ucMode;                          // EN_PITEM_MODE
};

struct MSG_PITEM_ADD : public MSG_PERIOD_ITEM_BASE
{
	u_char	ucPkgCnt;              // ĳ���κ����� ��Ű�� ����
	// rep. as ucPkgCnt
	int		iOrderNo;              // �ֹ���ȣ
	int		iProductNo;            // ��ǰ��ȣ
	int		iPkgIdx;               // ���õ� ��Ű���� �ε���
	
	u_char	ucItemCnt;
	// rep. as ucItemCnt;
	_sUser_Inven_v1_all                      client_item;         // client_item ucCount�� �ݺ��ȴ�.	
};

// ���� �����ۿ� �Ⱓ�� ������ ��.
struct MSG_PITEM_SET_PERIOD : public MSG_PERIOD_ITEM_BASE
{
	u_char						slot;                  // ���� �ε���.
	__inven_uselimit_info_v1	period;                // �Ⱓ ����.
};

// �Ⱓ�� �������� ���Ⱓ�� ������ ��.
struct MSG_PITEM_REMOVE : public MSG_PERIOD_ITEM_BASE
{
	u_char  slot;
};

// â�� �ִ� �������� ����� �� : â�� ����, â���� ã�� �� ���� �ȴ�.
struct MSG_STORAGE_PITEM_REMOVE : public MSG_PERIOD_ITEM_BASE
{
	char	cType;		// ������ ���� - ���� �������. 
	short	sID;
};

//Author : ����� //breif : ��������
#define MSG_NO_ITEM_SOCKET_GAMBLE	125	// ���� ��� �Ǵ� �ڽ� �׺�, ���� �и��� ��� ����.

// ������ ���� - ���Ͽ� ������ ����
enum 
{
	en_socket_gamble_mode_equip_socket = 0, // 0  ����/�ǻ� ���� �ڱ� : ������ EQUIP_SOCKET
	en_socket_gamble_mode_boxgamble,	// 1  �ڽ� �׺� : ������ EQUIP_SOCKET
	en_socket_gamble_mode_equip_result,	// 2  ���� ����� ���. : ������ EQUIP_SOCKET_RESULT
	en_socket_gamble_mode_boxgamble_result,	// 3  �ڽ� �׺��� ���. : ������ EQUIP_SOCKET_RESULT

	en_socket_gamble_mode_separate_socket = 10,	// 10  ���� �и� ��û
	en_socket_gamble_mode_separate_success,	// 11  ���� �и� ����
	en_socket_gamble_mode_separate_fail,	// 12  ���� �и� ����.
};

struct MSG_SOCKET_GAMBLE	// MSG_SM_
{
	_sHeader	header;
	u_char	ucMessage;
	u_char	ucMode;	// en_socket_gamble
};

// en_socket_gamble_mode_equip_socket
// en_socket_gamble_mode_boxgamble
struct MSG_SM_EQUIP_SOCKET : public MSG_SOCKET_GAMBLE
{
	u_char ucToolItemIdx;
	u_char ucObjItemIdx;
	char cAddonCounter;	// ��� ���� �õ����ΰ�?
	u_char ucAddonItemIdx[4];	// �ִ� 4�� �̹Ƿ�... 
};

 
// en_socket_gamble_mode_equip_result
// en_socket_gamble_mode_boxgamble_result
struct MSG_SM_EQUIP_RESULT : public MSG_SOCKET_GAMBLE
{
	_sUser_Inven_v1_all CompleteItem; // �ϼ��Ǿ��� ������ �ϳ�.

#ifdef VN_RB_GAMBLEBOX_070607_KUKURI
    _sitem_pair leftItemInfo[4];
#endif
};


// en_socket_gamble_mode_separate_socket=10,  // 10  ���� �и� ��û
struct MSG_SM_SEPARATE_SOCKET : public MSG_SOCKET_GAMBLE
{
	u_char ucTargetSlot;	// ����/�ǻ��� �ִ� �κ��丮 �ε���
	u_char ucSocketSlot;	// �и���ų ������ �ε���( 0 ~ 3 )
	u_char ucToolSlot;	// ��ĺи� ������ �ִ� �κ��丮 �ε���
	u_char ucEmptySlot;	// �и��� ������ �� �� �κ��丮 �ε���.
};

// en_socket_gamble_mode_separate_success,                 // 11  ���� �и� ����
struct MSG_SM_SEPARATE_SUCCESS : public MSG_SOCKET_GAMBLE 
{
	u_char ucTargetSlot;	// ����/�ǻ��� �ִ� �κ��丮 �ε���.
	_sUser_Inven_v1_all TargetItem; // ���� �и� ���� ����/�ǻ� ����.
	u_char ucResultSocketSlot;	// �и��� ������ �� �κ��丮 �ε���.(��û����, empty slot)
	_sUser_Inven_v1_all ResultSocket;	// �и��� ������ ����.

	u_char ucToolSlot;	// ��� �и������� �κ��丮 �ε���
	u_char ucLeftToolCount;	// ��� �и������� ���� ����.
};

// en_socket_gamble_mode_separate_fail,                          // 12  ���� �и� ����
struct MSG_SM_SEPARATE_FAIL : public MSG_SOCKET_GAMBLE
{
	u_char ucFailReason;

	/* ��ȹ�� �޽��� �״��.

	1 : Message#1 - ��Ĺ��� ���ų� ���� ������ ���� ��ǰ�Դϴ�.

	2 : Message#2 - ��ȭ���¿����� ��Ĺ��� �и��� �� �ֽ��ϴ�.

	3 : Message#3 - �೶�� ���� ������ ��� ��Ĺ� �и��� �����Ͽ����ϴ�.

	*/
}; 

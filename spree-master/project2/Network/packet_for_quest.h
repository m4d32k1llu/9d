#pragma once

// Quest
// Main = 9, 10, 11
// 72 - 95

struct _sHeader;

//=================================================================================
// QUEST ���� 9 : 72~79
// ��û/�뺸 ��Ŷ
#define MSG_NO_QUEST_DATA					72		// ĳ������ ����Ʈ ����Ÿ
#define MSG_NO_QUEST_SCON_REQUEST			73		// Ʈ���� �������� ���� ��û ��Ŷ

#define MSG_NO_QUEST_COMPLETE_DATA			74		// ������ ����Ʈ ������ ����Ʈ ����Ʈ ��û ��� ��Ŷ 
#define MSG_NO_QUEST_COMPLETEDATA_REQ   MSG_NO_QUEST_COMPLETE_DATA // ������ ����Ʈ ������ ����Ʈ ����Ʈ ��û ��Ŷ 
#define MSG_NO_QUEST_USER_CANCEL			75		// ������ ��� �뺸 ��Ŷ : ��� ���ǿ� �ɸ� ��� Ŭ���̾�Ʈ�� ���� �� �ְ�, 
												// ������ �Է��� ���ؼ� ���� �� �ִ�. 
// ��� ��Ŷ
#define MSG_NO_QUEST_SCON_RESULT			76		// Ʈ���� �������� ���� ���
#define MSG_NO_QUEST_FCON_RESULT			77		// Ʈ���� �������� ���� ���
#define MSG_NO_QUEST_SET_RESULT				78		// ����Ʈ ��� ����
#define MSG_NO_QUEST_SET_SELECTION_NO		79      // ������ ��ȣ ����
#define MSG_NO_QUESTITEM_RECEIVE			80		// ����Ʈ ������ S->C
#define MSG_NO_QUESTITEM_REMOVE				81		// ����Ʈ ������ ����
#define MSG_NO_LEARN_SKILL					82		// ���� ����
#define MSG_NO_FORGET_SKILL					83		// ���� ����
#define MSG_NO_UPDATE_STATUS				84		// ĳ�� ���� ����
#define MSG_NO_FORCED_MOVE					85		// ĳ�� ���� �̵�
#define MSG_NO_JOIN_GROUP					86		// ���� ����
#define MSG_NO_QUEST_SETCOUNT				87		// ī���� ���ÿ�û.
#define MSG_NO_QUEST_SETCOUNT_RESULT   MSG_NO_QUEST_SETCOUNT    // ������ �����߻���.....
#define MSG_NO_QUEST_REDISTRIBUTION			88      // ���� �� ����.

#define MSG_NO_QUEST_VALIDCHECK				89      // ����Ʈ ���� �ڵ� ��Ŷ :  ����Ʈ�� ��ȿ���� �˻縦 �Ѵ�.
#define MSG_NO_QUEST_VALIDCHECK_RESULT MSG_NO_QUEST_VALIDCHECK  	

#define MSG_NO_QUEST_VALIDCHECK_INGAME		91
#define MSG_NO_QUEST_VALIDCHECK_INGAME_RESULT MSG_NO_QUEST_VALIDCHECK_INGAME 

#define MSG_NO_QUESTNODEBACK_ITEM_RESULT	92 
#define MSG_NO_MONSTER_KILL					93		// ���� ���� üũ 

#define MSG_NO_QUEST_PARTY_SETCOUNT_RESULT	94		// ��Ƽ ����Ʈ���� ī��Ʈ ���
#define MSG_NO_QUESTRANK_REQ				95		// ����Ʈ ��ũ	
#define MSG_NO_QUESTRANK_RES			MSG_NO_QUESTRANK_REQ

#define _QUEST_COMPLETE_SUCCESS_			0x7f		// ����Ʈ �������� �ϼ�
#define _QUEST_COMPLETE_FAIL_				0x7e		// ����Ʈ ���з� �ϼ�
#define _QUEST_CANCEL_FAIL_					0x7d        // ����Ʈ ��� -> ���� �Ұ�. 
#define _QUEST_CANCEL_RESET_				0x00        // ����Ʈ ��� -> ���°���. ���� �غ���.

// ����Ʈ ���� ��û
struct MSG_QUEST_AUTH_REQUEST
{
	_sHeader			header     ;
	u_char				ucMessage  ;
	char                cNodeID    ;                // ��� �ѹ���.
	u_short             usQuestID  ;			    // ����Ʈ ���̵�.
	u_short				usUniqueID ;			    // USER�� SERVER�������� UNIQUE_ID : �ʿ��Ѱ�?

#ifdef _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI
	u_char				ucDungeon;					// 1-for dungeon, 0-default
	u_char				ucReserve;
	short				bBypassTorF;				// �����н��Ǵ� ������� TrueȤ�� False�� �����Ѵ�. 
#else
	BOOL                bBypassTorF;				// �����н��Ǵ� ������� TrueȤ�� False�� �����Ѵ�. 
#endif
};

// ����Ʈ ��� �뺸 ��Ŷ
typedef MSG_QUEST_AUTH_REQUEST MSG_QUEST_USER_CANCEL;

struct _sCharacQuestState;
struct _sQuestItem;
struct _sRunningQuest;
struct _sRunningQuestPacketData;

#define _QUEST_MAX_NUMBER_  300
#define _MAX_CURQUEST_NUM_  6

//=================================================================================
// ĳ������ ����Ʈ ���� : MSG_NO_QUEST_DATA				50		// ĳ������ ����Ʈ ����Ÿ
// ĳ���� ���ýÿ� �κ��丮 ���� ��Ŷ���� ������. 
// �� ��Ŷ�� ���� Ŭ���̾�Ʈ�� ����Ʈ ������ ������Ʈ �Ѵ�.
struct MSG_QUEST_DATA 
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	_CHARAC_COLLECTION_DATA	CollectionData;		//��ǰ���� ������
	
#ifdef _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI
	_CHARAC_COLLECTION_DATA	EventCollectionData;	// ��ǰ���� ������ Ȯ��
#endif
	
	char                cCountRQ;									// �� ���� ����Ʈ�� ����. 
	_sCharacMainQuestState MainQuestData;							// ������ ���� ����.	
	_sRunningQuestPacketData RunningQuestData[_MAX_CURQUEST_NUM_];  // 6�� . ������ ����. �ּ� �Ѱ�. �ִ� 6��. ������. 
	u_char              ucChapterFreeQuestData[99];                 // 99 ����Ʈ.  HHH
	
};

struct MSG_QUESTCOMPLETE_DATA 
{
	_sHeader             header;
	u_char               ucMessage;
	short                sCountCQ;            // �� �Ϸ� ����Ʈ�� ����.
	_sCharacQuestState   QuestData[_QUEST_MAX_NUMBER_]; // �Ϸ�� �͸� ������ ���� ��Ŷ. 80���� �ٿ�����.
};

struct MSG_QUEST_COMPLETEDATA_REQ
{
	_sHeader			header    ;
	u_char				ucMessage ;
}; //  �� �ʿ��� ���� �ִ°�?

// ĳ������ ����Ʈ ����
//struct MSG_QUEST_DATA {
//	_sHeader			header ;
//	u_char				ucMessage ;
//	_sCharacQuestState	QuestData[_QUEST_MAX_NUMBER_] ;			    // �������� �ƴ� �Ϲ����� ����Ʈ���� ������ �� ����. ���ڴ� 200�� ����.
//	_sCharacMainQuestState MainQuestData;                           // ������ ���� ����.	
//	_sRunningQuestPacketData RunningQuestData[_MAX_CURQUEST_NUM_];  // 6�� . ������ ����. 
//};


// ���� ���� ��� ��Ŷ
// CResult �� 
//	0 : ����
//		CInfo : 0 ���� �ܰ�, 1 ����Ʈ ���� �Ϸ�, 2 �κ� full
//	1 : �������� 
//		CInfo : ���� ���� ������ ���̵�
struct MSG_QUEST_AUTH_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char                cNodeID;                    // ��� �ѹ���.
	u_short             usQuestID;
	u_short             usNextQuestID;              // ������ �Ϸ��� ��� ���� ����Ʈ. 65535�϶� ������ ����Ʈ ��
	unsigned char		cResult ;					// ��� 
	unsigned char		cInfo ;						// ��� ����.
	char				cNeedInven;					// �ʿ��� �κ���(INVEN_FULL)�϶� 
};

// ����Ʈ ��� ���� ����
struct MSG_QUEST_SET_RESULT
{
	
	_sHeader			header ;
	u_char				ucMessage ;					// 
	
	char				cResult ;					// ���� �� : �����Ϸ�, ���пϷ�, �ʱ�ȭ.. ���.
	    
	u_short             usQuestID;
	u_int               uiMoney;					// ��� ���� ���� ���� �Ӵ�.
	_sSkill_pair        SkillDelete[5];				// ���ִ� ��ų.
	u_char              ucNum;						// ���ִ� ������ ����
	_sitem_pair         ItemDelete[20];				// ���ִ� ������. 
} ;


//#define MSG_NO_QUEST_SET_SELECTION_NO         79      // ������ ��ȣ ����
struct MSG_QUEST_SET_SELECTION_NO 
{
	_sHeader			header        ; 
	u_char				ucMessage     ;
	u_short             usQuestID     ;
	u_char              ucSelectionNo;         // ������ ��ȣ �����ϴ� ��Ŷ.      
} ;




//#define MSG_NO_QUESTITEM_RECEIVE			120
struct MSG_QUESTITEM_RECEIVE
{
	_sHeader			header ;
	u_char				ucMessage ;
	u_short             usQuestID;		
#ifdef _XDEF_PERIODITEM_20061027	
	_sUser_Inven_v1_all userItem;                                       // �Ⱓ�� ������ ������ ���� ������ ������ �ش�.
#else
	char				cItemType;					// Ÿ�Կ� ���� �Ϲ����� �ƴ��� üũ 
	unsigned short		cItemID;
	char				cInvenIndex ;				// �κ��丮 �ε���. -1�̸� ����, 108 �̸� ���̴�. 
	unsigned int		iCount;
#endif
	char				cCountablity;				// �ӽô� DBUpdate�� ������ ������ 
};

//#define MSG_NO_QUESTITEM_REMOVE			121 // ����Ʈ ������ ����
/*
struct MSG_QUESTITEM_REMOVE
{
	_sHeader			header;
	u_char				ucMessage;

	short				sQuestID;
	char				cInvenIndex;
	unsigned int		iCount;
};	
*/
struct MSG_QUESTITEM_REMOVE
{
	_sHeader			header ;
	u_char				ucMessage ;
	u_short				usQuestID;
	char				cSlotNum ;				//  -1�̸� ����
	unsigned int		iCount;
	_sitem_pair         ResItem[58] ;           // ���� ������ �κ��͸� ������ ���.
};


//#define MSG_NO_LEARN_SKILL				122
struct MSG_LEARN_SKILL
{
	_sHeader			header;
	u_char				ucMessage;
	
	char				cSlotNo ;			    // ��� ��ų�� ���Թ�ȣ.
	short				sSkillID;	            // 
	u_char				ucSkillLev;				// ��� ��ų ��.
	u_char				ucSkillState ;			// 0 : ��� ����, 1 : ���� �ʿ�.
};

//#define MSG_NO_FORGET_SKILL				123
struct MSG_FORGET_SKILL
{
	_sHeader			header;
	u_char				ucMessage;

	short				sSkillID;
};

#define ID_JINEXP							0		// ����ġ
#define ID_VITALITY							1		// ����
#define ID_ZEN								2		// ����
#define ID_SOUL								3		// ����
#define ID_HEART							4		// �ɱ�
#define ID_STAMINA							5		// ����
#define ID_VITALPOWER						6		// ü��
#define ID_INNERPOWER						7		// ����
#define ID_SOULPOWER						8		// ����
#define ID_HEARTPOWER						9		// �ɷ�
#define ID_STAMINAPOWER						10		// ����
#define ID_HONOR							11		// ��
//#define ID_RETRIBUTION						12		// ��
#define ID_FAME                             12          // �Ǹ�   

//#define MSG_NO_UPDATE_STATUS				124
struct MSG_UPDATE_STATUS
{
	_sHeader			header;
	u_char				ucMessage;

	char				cStatusType;
	unsigned int		uiUpdatedValue;
};

//#define MSG_NO_FORCED_MOVE				125
struct MSG_FORCED_MOVE
{
	_sHeader			header;
	u_char				ucMessage;

	float				fposX;
	float				fposZ;
};

//#define MSG_NO_JOIN_GROUP					126
struct MSG_QUEST_JOIN_GROUP
{
	_sHeader			header;
	u_char				ucMessage;

	char				cGroup;
};

// Ŭ���̾�Ʈ ���� ������.
#ifdef _XTS_NEWPARTYCOUNTER
struct MSG_QUEST_SETCOUNT
{	
	_sHeader            header    ;	
	u_char              ucMessage ;	
	u_short				usQuestId ;	
	char				cCounterNo;             // ������Ʈ�ؾ��� �����.	
	char                cNum      ;             // ������ ����.	
	u_short				usSum	  ;				// Ŭ���̾�Ʈ���� �������� �� ī��Ʈ ����.
	
};
#else
struct MSG_QUEST_SETCOUNT
{	
	_sHeader				header    ;	
	u_char					ucMessage ;	
	u_short					usQuestId ;	
	char					cCounterNo;             // ������Ʈ�ؾ��� �����.	
	u_char					ucNum     ;             // ������ ����.	
	u_char					ucSum     ;             // Ŭ���̾�Ʈ���� �������� �� ī��Ʈ ����.	
}; 
#endif


// �������� Ŭ���̾�Ʈ��... ( �����ÿ���.)
struct MSG_QUEST_SETCOUNT_RESULT
{
	_sHeader            header    ;
	u_char              ucMessage ;
	u_short             usQuestId ;                      // ����Ʈ ���̵� 	
	char                cCounterNo  ;
	int                 iRet      ;                      // ���ϰ��� -1�� ��� ������ ���հ� �� ���� ����.
	// ���ϰ��� -2�� ��� ���� ���� 255�� ����. (���� �÷ο�)	
	u_char              ucSum     ;                      // ������ ������ �������� �����ϰ� �ִ� �� ī��Ʈ ����. 
};

//#define MSG_NO_QUEST_REDISTRIBUTION     88      // ���� �� ����. 
struct MSG_QUEST_REDISTRIBUTION 
{
	_sHeader            header       ;
	u_char              ucMessage    ;
	short               sLeftPoint   ;
};


//#define MSG_NO_QUEST_VALIDCHECK         89      // ����Ʈ�� ��ȿ���� �˻縦 �Ѵ�.
struct MSG_QUEST_VALIDCHECK
{
	_sHeader            header       ;
	u_char              ucMessage    ;
	u_short	  			usQuestID    ; // �� ����Ʈ ���̵�.
	char				cNodeID      ; // �� ����Ʈ�� ������ ��� ��ȣ.
	char				cNeedItemRemove; // ������ �������� ������ 1 ������ 0;
};
typedef MSG_QUEST_VALIDCHECK MSG_QUEST_VALIDCHECK_INGAME;


// #define MSG_NO_QUEST_VALIDCHECK_RESULT MSG_NO_QUEST_VALIDCHECK 
struct MSG_QUEST_VALIDCHECK_RESULT 
{
	_sHeader            header       ;
	u_char              ucMessage    ;
	u_short				usQuestID    ; // �� ����Ʈ ���̵�.
	char	            cNodeID      ; // �� ����Ʈ�� ������ ��� ��ȣ.
};
typedef MSG_QUEST_VALIDCHECK_RESULT MSG_QUEST_VALIDCHECK_INGAME_RESULT;  


#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	#define _V1_COUNT_REAL_INVEN_			150		// ���� �κ��丮
#else
	#define _V1_COUNT_REAL_INVEN_			100		// ���� �κ��丮
#endif

//#define MSG_NO_QUESTNODEBACK_ITEM_RESULT 92 
struct MSG_QUEST_NODEBACK_ITEM_RESULT
{
	_sHeader            header       ;
	u_char              ucMessage    ;
	u_int               uiMoney      ;				// ���� ��.
	int                 contribution ;              // ���� ���ı⿩��.
	_sSkill_pair	    removeskill[5];				// �������� ��ų.
	_sitem_pair         removeitem[_V1_COUNT_REAL_INVEN_];				// �������� ������. 
	_sUser_Inven_v1_all additem[10];				// �־����� ������ . �ִ� ���ڴ� 10��.	
};


//#define MSG_NO_MONSTER_KILL					93 // ���� ���� üũ 
struct MSG_QUEST_MONSTER_KILL
{
	_sHeader            header;
	u_char              ucMessage;
	u_short             MobType;
};

struct _sPartyCounter
{
	u_short				usPersnal_Counter[4];
	
	int AllCounterAdd(){ return(static_cast<int>( usPersnal_Counter[0] + usPersnal_Counter[1] + usPersnal_Counter[2] + usPersnal_Counter[3] ) ); }
};

// ��Ƽ����Ʈ������ �׻� ī���� ��� ���� �޴´�.
//#define MSG_NO_QUEST_PARTY_SETCOUNT_RESULT	94 // ��Ƽ ����Ʈ���� ī��Ʈ ���
struct MSG_QUEST_PARTY_SETCOUNT_RESULT
{
	_sHeader            header			;
	u_char              ucMessage		;
	u_short             usQuestId		;         // ����Ʈ ���̵� 
	char                cMvp			;         // MVP	
	_sPartyCounter		_sPCounter[9]	;         // ��Ƽ ī����
};      

//#define MSG_NO_QUESTRANK_REQ				95		// ����Ʈ ��ũ	
struct MSG_QUESTRANK_REQ
{
	_sHeader			header;
	u_char				ucMessage;
	
	u_short				usQuestID;
};

//#define MSG_NO_QUESTRANK_RES			MSG_NO_QUESTRANK_REQ // ����Ʈ ��ũ ��
struct MSG_QUESTRANK_RES
{
	_sHeader			header;
	u_char				ucMessage;
	
	u_short				usQuestID;
	_QUEST_RANK_DATA	Data;
};



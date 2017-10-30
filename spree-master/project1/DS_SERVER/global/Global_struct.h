#pragma once

#pragma pack( push, enter_global_struct )
#pragma pack(1)




/*#################################################################################

��Ģ : 
1. ������ ���( TCP : GS<=>DS ) ��Ŷ�� ù���ڷ� 'C'�� �ִ´�.
( Ŭ���̾�Ʈ�� GS�� ��� ��Ŷ�� ��ĥ�� �����Ƿ� )
2. ������ ���( UDP ) �� 'U' ��  ����Ѵ�.

3. DS->GS �� ������ ������ ��� int uid�� �����ؾ� �Ѵ�. 

8�� ������ �����ϴ� ������ if ���� ������ �˻縦 �����ϱ� ���ؼ���.
message �� 8�� ������( ����Ʈ ���� ' >> 3 ' �� ���� ) ��� �������� 
������ ��� �������� �޽����� �����Ѵ�. 

( �׷��� ������ �ظ��ϸ� ����. 
���� ������ �ᵵ, �״�� ������ �� ����, �ٸ� �˾ƺ��� ����.) 

�߰� :
(â�� ���� ��Ŷ)10/21
#define CMSG_NO_DEPOT_REQ_GS_DS				25		// GS->DS Depot data request..
#define CMSG_NO_DEPOT_RES_GS_DS				26		// DS->GS Depot data result..
#define CMSG_NO_UPDATE_DEPOT_				53			// _USER_DEPOT

#################################################################################*/


/*
	�κ� ����ü
*/



union __item_detail
{
	char		weapon_none ;
	char		cloth_equippos ;			// �ǻ��� ��� ���� ��ġ
	char		book_none ;
	char		potion_none ;
	char		backpack_count ;			// �賶�� ��� �뷮
} ; 


union __item_or_money
{
	struct
	{
		u_short	 usCurDur ;		// ���� ������
		char	 cIdentify     : 4;	// ���� ����
		u_char   ucSocketCount : 4; 	// ������ ����� �ո� �� �ִ°�?
		u_char   ucCount ;		// �ִ� ���� 255��
	} S_NormalItem;
	struct 
	{
		u_int	 uiCount ;	 	// �Ӵ�..
	} S_MoneyItem;
} ;


struct _sServer_InvenItem
{
	unsigned __int64    ui64UniqueId;
	char				cType ;					// ������ ��з�
	char				cSecond ;
	short				sID ;					// ������ �Һз�. (������ ���̵�)

	__item_detail		detail ;				// ������ Ÿ�Կ� ���� �ٸ� ����.	
	__item_or_money		Item_Money ;			 		
	u_char              ucSocket[4];            
} ;


struct __inven_uselimit_info_v1	// ������� ���� ��Ÿ ����.
{
	u_char				ucFlag1 ;		// Flag1 = �������� �ִ°� ���°�..

	u_short				usResaved		: 1;	// �⺻���� ������ ����� �Ǿ��°�, ó�� �ʱ�ȭ�ÿ� �ѹ� ���δ�. : ������.
	u_short				usUsedProtect	: 1;	// ��縦 ����� ���� �ִ°�.
	u_short				usCurProtect	: 5;	// ������ ���, �ٸ� �븰�� ��. 0 - 15
	u_short				usCurMaxProtect	: 5;	// �ִ밭��

	u_short				usReserved		: 4;	// �����.u_short				usValue1 ;		// 

	u_short				usValue1 ;		// 

	u_char				ucYear ;		// Year - 2000 : 5 ~ ����.
	u_char				ucMonth ;		// Month
	u_char				ucDay ;			// Day
	u_char				ucHour ;		// Hour
	u_char				ucMinute ;		// Minute 
} ;



struct _sServer_InvenItem_v1
{
	unsigned __int64		ui64UniqueId;	// 8 
	char					cType ;			// ������ ��з� 9
	char					cSecond ;		// Second�� �ʱ�ȭ �� ������, ������ �����Ѵ�.
	short					sID ;			// ������ �Һз�. (������ ���̵�) 12

	__item_or_money			Item_Money ;	// 16 		

	// Ŭ���̾�Ʈ ��Ŷ �ڸ��� 1. ����/��Ÿ ������ ������ ����, �߶� ������.
	// ����/�ǻ��� ��쿡�� �ְ�, �ƴϸ� ����. 
	short					sSocket[4];		// 24

	u_char					ucStrength ;	// ������ ���, ����.
	u_char					ucSharpness ;	// ������ ���, ����.

	// Ŭ���̾�Ʈ ��Ŷ �ڸ��� 2. �������
	__inven_uselimit_info_v1	uselimit ;

	inline void convert_from_v0( _sServer_InvenItem * pv0 )
	{
		ui64UniqueId	= pv0->ui64UniqueId ;
		cType			= pv0->cType ;
		cSecond			= pv0->cSecond ;
		sID				= pv0->sID ;
		Item_Money.S_MoneyItem.uiCount = pv0->Item_Money.S_MoneyItem.uiCount ;	// ī��Ʈ�ϳ��� �����ϸ� �� �ȰŴ�.
		sSocket[0]		= static_cast<short>(pv0->ucSocket[0]) ;
		sSocket[1]		= static_cast<short>(pv0->ucSocket[1]) ;
		sSocket[2]		= static_cast<short>(pv0->ucSocket[2]) ;
		sSocket[3]		= 0 ;

		uselimit.ucFlag1	= 0;
		uselimit.usResaved	= 0;
		uselimit.usUsedProtect = 0;
		uselimit.usCurProtect	= 0;
		uselimit.usCurMaxProtect= 0;
		uselimit.usReserved		= 0;
		uselimit.usValue1	= 0 ;		// 

		uselimit.ucYear	= 0 ;		// Year - 2000 : 5 ~ ����.
		uselimit.ucMonth	= 0 ;		// Month
		uselimit.ucDay	= 0 ;			// Day
		uselimit.ucHour	= 0 ;		// Hour
		uselimit.ucMinute	= 0 ;		// Minute 
	}
} ;

/*#################################################################################

�⺻ ����ü 

#################################################################################*/
// ĳ���� �⺻ ���� : �⺻ ����

#ifdef CHARAC_CREATE_DATE_20070309
struct _sCharac_Create_Date 
{
	short sYear	: 6;	//(0 ~ 63) : Base 2000
	short sMonth : 4;	//(0 ~ 15)
	short sDay : 6;		//(0 ~ 63)
};
#endif

struct _CHARAC_BASIC {
	//	ND_V01_Charac
	int		iUniqueID;				// ����ũ���̵�	4
    char	cAccount[en_max_lil+1];				// ���� ���̵�  17   <----��� �ٲ�����
	char	cChrName[13];			// ĳ���� �̸�  30
	char	cChrNic[13];			// ĳ���� ��ȣ  43
	char	cSex;					// ����  44
	char	cParty;					// ��Ƽ  45
	char	cGamete[13];				// �����  58
	char	cHair;					// �Ӹ����  59
	char	cFace;					// �󱼸��	60
	char	cLuck;					// �  61

	char	cClass ;				// ���� : �⺻�� 0
	char	cClassGrade ;			// ���� ��� : �⺻�� 0		: 2004/11/10 �߰�. Ray
	int		iContribution ;			// ���� �⿩�� : �⺻�� 0	: 2004/11/10 �߰�. Ray

	char	cGM_Check ;				// GM ĳ���� ����..
	DWORD	dwPlayTime ;				// ����

	u_char	ucChangeName ;			// �̸� ���� ���� �÷��� 0 - not, 1 - possible

#ifdef CHARAC_CREATE_DATE_20070309
	_sCharac_Create_Date	CharacCreateDate;
#endif

} ;



//// ĳ������ ���� ���� ������ ��ġ
struct _CHARAC_CUR_BASIC {
	//	ND_V01_CurBasicInfo
	short	sZone;					//����  2
	short	sY;						//  4
	float	fX;						//��ġ��ǥ 8
	float	fZ;						// 12
	short	sLifePower;				//���� ü��  14
	short	sForcePower;			//���� ����  16
	short	sConcentrationPower;	//���� ����  18

	// �߰�
	char	cRespawnServerNo ;		// ��Ȱ ������ ���� ��ȣ
	char	cRespawnPosName[13] ;	// ��Ȱ ��ġ�� �̸�
	float	fRespawnPosX ;			//
	float	fRespawnPosZ ;			//
};



// 2004/06/16 ������Ʈ
//// �����ִ밪�� ����ġ
struct _CHARAC_LEVEL {
	//	ND_V01_5StarLevel
	short	sMaxLifePower;				//����  24  <= ����
	short	sMaxForcePower;				//����  26  <= ����
	short	sMaxConcentrationPower;		//������ 28 ? �׳� ��������...	
	short	sConstitution;				//�ǰ�  36 <= ����
	short	sZen;						//����  40 <= ����
	short	sIntelligence;				//����  44 <= ����
	short	sDexterity;					//��ø  48 <= �ɱ�
	short	sStr;						//�ٷ�  52 <= ����
	short	sLeftPoint;					//���� ����Ʈ
	short	sMasteryPoint  ;			// �����͸� ����Ʈ...
};

// ����ġ�� ����
// 2004/04/12 : �Ƿε� Ÿ�� ����, �ܻ�/���� �߰�
struct _CHARAC_STATE {
	//	ND_V01_ChatacState
	short	sInnerLevel;			// �������� 4
	unsigned int uiJin;				// ��
	int		iGong;					//	�Ǹ�
	short	sRetribution;			//	����  6
	int		iHonor;					//	��  8
	short	sShowdown;				//������	10
	u_short	usFatigue;				// �Ƿε� : �⺻�� 0
	short	sWoundValue ;			// �ܻ� : �⺻�� 0
	short	sInsideWoundValue ;		// ���� : �⺻�� 0 	
	short	sFuryParameter ;		// �г� �Ķ����.
	int		sLevelUpGameSecond ;	// ���� �� �� ���ӳ� �� �ð�.( getGlobalTime() )
	int		iORIndex ;				// ���� �ε���..	

	short	sPeaceMode ;			// �¼�. 0 - ��ȭ���,  1 - ���� ��� : 2005-04-14 Ray..

	int		iMuteTime ;				//	ä�ñ��� �ð�..(��)
	int		iHonorGaveDate ;		//	�� ������ ��¥ : �ٸ� ������ ���δ�.
	int		iHonorTakeDate ;		//	�� �򰡹��� ��¥
	int		iHiding ;				//	������
	int		iBlockingEndDate ;		//	��ŷ ���� ��¥(~����)
	
	short	sPkPrevDeadMode ;
	short	sPkDeadCount ;
	short	sPkKillCount ;
	short	sMonsterKill ;				// ���� ���� ���� ��

	// Not save
	short	sPartyIndex ;			// ���� �������̶�� �ε�����, �ƴϸ� 0.
	short	sPartySlotNo ;			// ��Ƽ���� ���� ��ȣ ???

#ifdef _PD_CASTLE_STEP_1_
	u_short usCombatDeadCount;		// �ѹ��� ����� �� ���� ȸ��
	u_short usCombatLeftPenaltyTime;// ���� �� ���� �г�Ƽ �ð�.

	int		iCombatLastDeadTime;	// ����� �� ���������� ���� �ð�. ��ȿ�� �Ǵܿ� ����.
#endif

#ifdef _PD_LEVEL_RATE_SAVE_
	short	sLevelModifyFlag;		// �������̺��� ����Ǹ�, DB �۾����� 1�� ������ ���̴�.
	double	dLevelRate;				// �� ������ ���� ����.
#endif
#ifdef  _ORG_PROMOTION_	// ����ü�ð�,Ż��ð� �߰�
	int		iOrgDisbandTime;		///<�� ��ü �ð�
	int		iOrgDropTime;			///<�� Ż�� �ð� 
#endif//_ORG_PROMOTION_
};


struct _CHARAC_QUEST_DATA 
{
	BYTE    ucCurQuestNum;      // �� ��� ����Ʈ�� ����ǰ� �ִ°�.
	BYTE    cCurQuestData[162]; // ���� ����Ʈ ����. ����5�� + ���� 1�� 
	BYTE    cMainQuestData[4];  
	BYTE    ucMainStoryNum;     // ������ �� ���ΰ�?
	short   sCompleteQuestNum;  // �Ϸ�� ����Ʈ ����.    
	BYTE    cQuestData[900];    // �⿬�� ������ ���� 
	BYTE	cChapterFreeQuest[100] ; // ���� �Ϸ�
#ifdef _PD_QUEST_MAX_600_
	struct __quest_data2
	{
		BYTE	cQuestData2[900];	// ����Ʈ ���� Ȯ�� ( +300 �� )
	} QuestData2;
#endif
};

// �������� ������ �����͸� ��ų�� ����Ʈ ����.
struct _sMasterySkill ;
struct _sServer_Mastery_Info
{
	short				skillID ;			// �����͸� ��ų�� ���̵�
	char				cPoint ;			// ���� ����Ʈ. 0�� �� ����. 0�̸� ���� ����.
	char				cReserve ;			// ����. �Ⱦ���.

	_sMasterySkill *	pMastery ;		// ���۷��� ������.
} ;

struct _CHARAC_MARTIAL_DATA {
	union
	{
		struct _Martial {
			BYTE	cMartialData[600];		// ��� ��ų ����
			BYTE	cMasteryData[384];		// ���� ���� ����
			BYTE	cCoolTimeData[256] ;	// 256 bytes sizeof() * _COUNT_COOL_TIME_
		} MartialData ;
		BYTE		cData[sizeof(_Martial)] ;
	} ;
	union
	{		
		struct _BookInfo {
			short		sBookID ;
			u_int		uiBookLeftTime ;			// ���� �ð�(�ʴ���) : �⺻�� 0

			short		sBookPad1 ;
			short		sBookPad2 ;					// ����.

			short		sBookReserved1 ;			// ���� ����
			short		sBookReserved2 ;			// 
			short		sBookReserved3 ;			// 
			int			iBookReserved4 ;
		} S_Bookinfo ;
		BYTE		cBookData[sizeof(_BookInfo)] ;
	} ;
#ifdef _PD_SKILL_EXTEND_
	BYTE cExtendMartial[400];
#endif
} ;


struct Elixir_Data
{
	struct _Data
	{
	public:
		short __Data[10]; 
		char  __Grade;
	
		short __Get_Data( char idx ){ return __Data[ idx ]; }
		void  __Set_Data( char idx, short Value ){ __Data[ idx ] = Value; }
		char  __Get_Grade(){ return __Grade; }
		void  __Set_Grade( char Value ){ __Grade = Value; } 
	};
public:	
	_Data Data[5];		  // ���Ǻ� ������. 
	// 0 �ٷ� 
	// 1 ���� 
	// 2 ����			
	// 3 �ǰ� 
	// 4 ��ø 

	short _sIntoxication; // �ߵ� ��ġ.

protected:
	char  Temp[3];		  // [0]��°�� Ÿ�̸� ������̾����� ���� ������̾��ٸ� ���� �α��� �ÿ� Ÿ�̸ӿ� �� ����Ѵ�.

public:
	char _Get_IsTimerAct(){ return Temp[0]; }
	void _Set_IsTimerAct(char cValue ){	Temp[0] = cValue; }

	short _Get_Add_Data( char idx ) // �ε����� ���Ǹ� ��Ÿ����. ������ �ϳ��� �ջ��ؼ� �����ϴ� �Լ�.
	{
		short sSum = 0;
		char cGrade = Data[idx].__Get_Grade();
		for( char cc = 0; cc < cGrade; ++cc )
		{
			sSum += Data[idx].__Get_Data(cc);
		}
		return sSum;
	}

	/* �ߵ� ��ġ */ 
	short _Get_Intox(){ return _sIntoxication; }
	void  _Set_Intox( short Value ){ _sIntoxication = Value; }

	void  _Init()
	{
		memset( &Data, 0, sizeof(_Data) * 5 ); 
		_sIntoxication = 0;
		Temp[0] = 0;
		Temp[1] = 0;
		Temp[2] = 0;
	}

	/* ������ ��� */ 
	char _Get_Grade( char idx ){ return Data[idx].__Get_Grade(); }
	void _Set_Grade( char idx, char Value ){ Data[idx].__Set_Grade( Value ); }

	/* ������ �������� �� �����Ǵ� ��ġ */ 
	void _Set_ElixirValue( char idx, short Value, char Grade )
	{
		Data[ idx ].__Set_Data( Grade,Value ); 
		if( !( Data[ idx ].__Get_Grade() >= Grade + 1 ) ) 
			Data[ idx ].__Set_Grade( Grade + 1 );
	}
};

//���� ������
#define COLLECTIONDATA_COMPLETEFLAG_INDEX	(15)
class ckCollectionData
{
public:
	unsigned short	m_usData;
	inline unsigned short	GetData()	{	return m_usData;	}
	bool					IsThisTrue(unsigned char index);
	void					SetTrue(unsigned char index);
	void					SetFalse(unsigned char index);

	bool					IsThisCompleted();
	void SetCompleted()
	{
		m_usData = m_usData | 0x8000 ;
	} ;
};


#define MAX_COLLECTION_INDEX_COUNT	(512)
struct _CHARAC_COLLECTION_DATA	//DB�� �����ϴ� ������ ����
{
	ckCollectionData		m_aData[MAX_COLLECTION_INDEX_COUNT];
};

#ifdef _PD_ADD_EVENT_COLLECTION_
#define BASE_EVENT_COLLECTION_INDEX				(1000)
#endif


#ifdef _PD_CASH_INVEN_
	#define _MAX_INVEN_SLOT_COUNT_		(158)
	#define _CASH_INVEN_START_SLOT_		(100)
	#define _TEMP_INVEN_START_SLOT_		(150)
#else
	#define _MAX_INVEN_SLOT_COUNT_		(108)
	#define _TEMP_INVEN_START_SLOT_		(100)
#endif

#define _INVEN_SIZE_				(3888)
#define _CASH_INVEN_SIZE_			(1800)

#define _MED_CHECK_SIZE	110
#define _COLLECTION_DATA_SIZE	sizeof(_CHARAC_COLLECTION_DATA)
struct _CHARAC_INVENTORY 
{
	BYTE    cInventory[_INVEN_SIZE_];	
#ifdef _PD_CASH_INVEN_
	BYTE	cCashInven[_CASH_INVEN_SIZE_];
#endif
	short	sSelectWeaponID ;			//������ 0, �������̸� �ε����� �ִ� ���� �ƴ϶� ID �� �ش�.
	unsigned int	iMoney  ;			//��
	
	BYTE	cElixirData[_MED_CHECK_SIZE];			//���������� �뺸 �� ��.
	BYTE	cCollectionData[_COLLECTION_DATA_SIZE];//��ǰ ������
#ifdef _PD_ADD_EVENT_COLLECTION_
	BYTE	cEventCollectionData[_COLLECTION_DATA_SIZE];//��ǰ ������
#endif

	short	sVersion ;					// item version

	short	sBloodPoint ;				// ���� ����Ʈ

	_sServer_InvenItem_v1 * GetSlot( u_char slot )
	{
		_sServer_InvenItem_v1 * p = NULL;
		if( slot < _MAX_INVEN_SLOT_COUNT_ )
		{
#ifdef _PD_CASH_INVEN_
			if( slot < _CASH_INVEN_START_SLOT_ )
			{
				p = reinterpret_cast<_sServer_InvenItem_v1*>( &cInventory[slot*sizeof(_sServer_InvenItem_v1)] );
			}
			else if( slot < _TEMP_INVEN_START_SLOT_ )
			{
				p = reinterpret_cast<_sServer_InvenItem_v1*>( &cCashInven[(slot-_CASH_INVEN_START_SLOT_)*sizeof(_sServer_InvenItem_v1)] );
			}
			else
			{
				p = reinterpret_cast<_sServer_InvenItem_v1*>( &cInventory[(slot-_TEMP_INVEN_START_SLOT_+_CASH_INVEN_START_SLOT_)*sizeof(_sServer_InvenItem_v1)] );
			}				
#else
			p = reinterpret_cast<_sServer_InvenItem_v1*>( &cInventory[slot*sizeof(_sServer_InvenItem_v1)] );
#endif
		}
		return p;
	}
};

/*
#define _MAX_NIC_COUNT	41`
enum{
	_RANK_SHOWDOWN_NIC = 0,
	_RANK_PART_NIC,
	_RANK_LEVEL_NIC,
	_RANK_MARTIAL_NIC,
	_GEN_SHOWDOWN_NIC = 5,
	_GEN_QUEST_NIC,
	_GEN_PART_NIC,
	_GEN_MARTIAL_NIC,
	_USE_NIC = 10,
	_SELECTED_SLOT_NIC = 40,		// 40�� ���Կ��� ������ ���� ��ȣ��..
} ;
*/




enum 
{
	_NIC_COUNT_NOTHING			= 1,
	_NIC_COUNT_SPECIAL_SHOWDOWN = 10,
	_NIC_COUNT_SPECIAL_MOOMPA	= 10,
	_NIC_COUNT_SPECIAL_LEVEL	= 10,
	_NIC_COUNT_SPECIAL_MARTIAL	= 50,
	_NIC_COUNT_GENERAL_SHOWDOWN	= 10,
	_NIC_COUNT_GENERAL_QUEST	= 50,
	_NIC_COUNT_GENERAL_MOONPA	= 1,
	_NIC_COUNT_GENERAL_MAKER	= 10,
	_NIC_COUNT_GENERAL_GATER	= 10,
	_NIC_COUNT_GENERAL_EVENT	= 20,
	_NIC_COUNT_GENERAL_MONSTER	= 30,
	_NIC_COUNT_TOTAL			= 212,
} ;

struct _CHARAC_NIC {

	short			sSelectNic ;		// ������ ��ȣ
	short			sUsedTime ;			// ���Ǿ��� �ð�

	union
	{		
		struct _NicData {
			u_short			usNothing[_NIC_COUNT_NOTHING] ;			// 0�� ������ �ƹ��͵� �������� �ʾ������� ������ ����д�.
			struct 						// Ư�� ��ȣ
			{
				u_short		usShowdown[_NIC_COUNT_SPECIAL_SHOWDOWN] ;	// Ư���񹫺�ȣ
				u_short		usMoonpa[_NIC_COUNT_SPECIAL_MOOMPA] ;		// Ư�����ĺ�ȣ
				u_short		usLevel[_NIC_COUNT_SPECIAL_LEVEL] ;		// Ư����޺�ȣ
				u_short		usMartial[_NIC_COUNT_SPECIAL_MARTIAL] ;		// Ư��������ȣ
			} _SpecialNic ;

			struct 						// �Ϲݺ�ȣ
			{
				u_short		usShowdown[_NIC_COUNT_GENERAL_SHOWDOWN] ;		// �񹫺�ȣ
				u_short		usQuest[_NIC_COUNT_GENERAL_QUEST] ;		// ǳ��� ��ȣ
				u_short		usMoonpa[_NIC_COUNT_GENERAL_MOONPA] ;		// ���� ��ȣ
				u_short		usMaker[_NIC_COUNT_GENERAL_MAKER] ;		// ���ۺ�ȣ
				u_short		usGater[_NIC_COUNT_GENERAL_GATER] ;		// ������Ī
				u_short		usEvent[_NIC_COUNT_GENERAL_EVENT] ;		// �̺�Ʈ��ȣ
				u_short		usMonster[_NIC_COUNT_GENERAL_MONSTER] ;
			} _GeneralNic ;
		} _Nic ;
		u_short				usNicID[_NIC_COUNT_TOTAL] ;
		BYTE				bitData[sizeof(_NicData)] ;
	} ;

};



struct _CHARAC_MATCH {
	int					iWinScore ;				// �� ȸ��.
	int					iLostScore ;			// �й� ȸ��
	int					iTieScore ;				// ���º��� ȸ��
	int					iSeriesCount ;			// ���� �Ǵ� ���� : ���� PrevMatchResult �� ���� �ٸ� �ǹ�.
	int					iMaxSeriesWinCount ;	// �ִ� ���� ���.
	int					iMaxSeriesLoseCount ;	// �ִ� ���� ���
	int					iAbuseCount ;			// ������ �й��� ȸ��(����)
	short				cPrevMatchResult ;		// 0 : ��, 1 : ��, 2 : ��
} ;



#define _MAX_CHARAC_BH_ENTRY_				32			// ���� �ΰ� �� : ���� ���� ������ 
#define _MAX_CHARAC_EFFECT_ENTRY_			32			//
#define _MAX_EXTRA_EFFECT_ENTRY_			10
#define _MAX_CHARAC_SPECIAL_BUFF_			12
/* =====================================================================================
_sDB_BHEntry : ��� ����� Buff/Hostile ���� 
_sUser_BHEntry : �������� ���� ����Ÿ.
===================================================================================== */
typedef struct 
{
	short			skillID ;			// ��ų�� ���̵�
	unsigned short	usLeftTimeSec ;		// ���� �ð�
	char			skillLevel ;		// ��ų�� �ܰ�
} _sDB_BHEntry, _sUser_BHEntry ;


/* =====================================================================================
ȿ�� ��� ���� : DB �Ǵ� User���� ���� ��.	
===================================================================================== */
typedef struct 
{
	short				id ;
	unsigned short		usLeftTimeSec ;		// ���� �ð� ( '��', ms ���� ��Ȯ�� �ʿ� ����. )
	short				value ;				// ȿ�� ��Ҵ� �ܰ谡 ����. �׷��� ���� �ʿ�.
} _sDB_EffectEntry, _sUser_EffectEntry ;


struct _CHARAC_EFFECT
{
	union
	{			
		struct _EffectData {
			_sDB_BHEntry		BHEntry[_MAX_CHARAC_BH_ENTRY_] ;
			_sDB_EffectEntry	EffectEntry[_MAX_CHARAC_EFFECT_ENTRY_] ;
			_sDB_EffectEntry	ExtraEntry[_MAX_EXTRA_EFFECT_ENTRY_] ;
		} _data ;		
		BYTE					byData[sizeof(_EffectData)] ;
	};

#ifdef _PD_BUFF_COUNT_MODIFY_
	union
	{
		_sDB_BHEntry			Special[_MAX_CHARAC_SPECIAL_BUFF_];
		BYTE					bySpecial[sizeof(_sDB_BHEntry)*_MAX_CHARAC_SPECIAL_BUFF_];
	};
#endif	// _PD_BUFF_COUNT_MODIFY_
} ;


// ĳ������ ������ ��� ��Ȳ.
// 1. ��ġ��
// 2. ����
struct _CHARAC_ITEM_USE_STATUS
{
	int		shout_left_time ;		// �Ѵ� 0 �� �ƴ� �� �ǹ̰� �ִ�. 
	short	shout_type ;
} ;


struct _sND_AIUS_Grade
{
	// ��/��/��/��/��/��. End Time
	u_int			uiYear		: 5 ;	// Base 2000. Year = 2000 + uiYear( 0-31 )
	u_int			uiMonth		: 4 ;	// Value 0 - 11 ( 0 - 15 )
	u_int			uiDay		: 6 ;	// Value 1 - 31 ( 0 - 63 )
	u_int			uiHour		: 5 ;	// Value 0 - 23 ( 0 - 31 ) 
	u_int			uiMinute	: 6 ;	// Value 0 - 59 ( 0 - 63 )
	u_int			uiSecond	: 6 ;	// Value 0 - 59 ( 0 - 63 )

	u_char			ucReserve ;		// �׳� 8 byte ���߷��� ����.
	char			cItemType ;		// ������� ������.
	short			sItemID ;		// ������� ������.
} ;

struct _ACCOUNT_ITEM_USE_STATUS
{
	_sND_AIUS_Grade		grade ;
	DWORD				uiEndTime ;		// 
	bool				bChanged ;		// 
} ;

#ifdef _VINA_SHUTDOWN_20061023_
struct _FATIGUE_ELEMENT_
{
	short				sFatigueWeight ; //�Ƿε�(���� : ��) 
	int					iLogoutTime ;	//�α׾ƿ��ð�( ���� : Hour) // ������ Day->Hour�� 2007.04.09�� �����Ǿ���
#ifdef _PD_SDS_GOSTOP_
	u_char				ucFatigueMode;
#endif	// _PD_SDS_GOSTOP_
} ;
#endif	// _VINA_SHUTDOWN_20061023_


#ifdef _PD_CHARAC_LTS_

enum EN_LTS_DATA_COUNT
{
	en_lts_lvpk_data_count = 30,
	en_lts_pk_data_count = 10,
	en_lts_sp_data_count = 1,
	en_lts_pos_info_count = 11,
};


//��ġ������ �⺻ ����
struct _sLTS_Data
{
	int			iCharacUID;					// ĳ���� ����ũ ���̵�.
	int			iStartTrackingTime;			// ��ġ������û�ð�
	u_short		usReserve;
	u_char		ucTimeMode;					// �Ƿ��� ��ġ���� ���� ( 1) 24�ð�, 2) 48�ð� ) 
	char		szCharacName[en_charac_name_length+1];
	_sLTS_Data()
		: iStartTrackingTime(0), usReserve(0), ucTimeMode(0)
	{}
};

struct _CHARAC_LTS
{
	_sLTS_Data		m_pLvPkData[en_lts_lvpk_data_count];
	_sLTS_Data		m_pPkData[en_lts_pk_data_count];
	_sLTS_Data		m_pSpData[en_lts_sp_data_count];
};

#endif	// _PD_CHARAC_LTS_



#ifdef _PD_MASTER_PUPIL_

struct _CHARAC_MP
{
	u_char				ucMPPosition;		// EN_MP_RELATION
	u_char				ucRes;				// �׳�.
	u_short				usRes;				// �׳�.
	int					iMasterIndex;		// ���� �ε���.
};

#endif	// _PD_MASTER_PUPIL_


struct _CHARAC_ETC {
	//	��Ÿ

	char				cState ;		//ĳ���ͻ���(�������� ��Ÿ��)	

	union{
		short					_sBACKUP_STATE_CHECKED ;	//0:����Ϸ� 1:����Ÿ ����
		struct {
			bool				bBackupStateBasic : 1;	//0:����Ϸ� 1:����Ÿ ����
			bool				bBackupStateLevel : 1;	//0:����Ϸ� 1:����Ÿ ����
			bool				bBackupStateState : 1;	//0:����Ϸ� 1:����Ÿ ����
			bool				bBackupStateCurBasicInfo : 1;	//0:����Ϸ� 1:����Ÿ ����
			bool				bBackupStateInven : 1;	//0:����Ϸ� 1:����Ÿ ����
			bool				bBackupStateQuest : 1;	//0:����Ϸ� 1:����Ÿ ����
			bool				bBackupStateMartial : 1;	//0:����Ϸ� 1:����Ÿ ����	
			bool				bBackupStateNic : 1 ;		//0:����Ϸ� 1:����Ÿ ����

			bool				bBackupStateEffect : 1 ;
			bool				bBackupStateMatch : 1 ;
			bool				bBackupStatePkRecord : 1 ;
			bool				bBackupStateCharacItemUseStatus : 1 ;
			bool				bBackupState_4 : 1 ;
			bool				bBackupState_3 : 1 ;
			bool				bBackupState_2 : 1 ;
			bool				bBackupState_1 : 1 ;
		} _backup;		
	} ;	
};




/* =====================================================================================
PK ������ ���δ�.
===================================================================================== */

#define _PK_RECORD_WIN_COUNT_		10
#define _PK_RECORD_LOSE_COUNT_		10
#define _PK_RECORD_ABUSING_CHECK_	20

enum
{
	en_pk_option_none=0,		// 0 : No Meanings
	en_pk_option_win_normal,	// 1 : Win - Normal
	en_pk_option_win_mask,		// 2 : Win - Mask
	en_pk_option_lose_normal,	// 3 : Lose - Normal
	en_pk_option_lose_mask,		// 4 : Lose - Mask
};


struct __cntPkRecordEntry		// ���� ��� : ������ �̸���, ��¥, ����.
{
	union {
		struct {
			u_int		uiOption	: 3 ;	// 0 : None, 1 : win-Normal, 2 : Win-Mask, 3 : lose-normal,
			// 4 : lose-mask, 5 : Res, ...
			u_int		uiYear		: 5 ;	// �⵵. 2006 + year
			u_int		uiMonth		: 6 ;	// 1 - 12 ��
			u_int		uiDay		: 6 ;	// 1 - 31
			u_int		uiHour		: 6 ;	//
			u_int		uiMinute	: 6 ;	// 0 - 59
		} ;
		u_int			uiValue ;			// 
	} ;


	char		name[13] ;			// �������� Option 2�� NULL �� ��. 

	char		cMoonpa ;			// ���� ����.
} ;

struct __cntPkCount				// �� ����, Ȥ�� �� ��å�� ���� ��/�� ��.
{
	int			iWin ;
	int			iLose ;
} ;


struct __svrPkRecordEntry
{
	u_int		uiTime ;			// ���������� GetGameTime�� ���ϰ����� �˻縦 �Ѵ�.
	int			iCharacUID ;		// DB�� ĳ���� ���̵�.
} ;


struct _CHARAC_PK_RECORD
{
	int				iKillCount ;
	int				iDeadCount ;

	BYTE			bWinRecord[sizeof(__cntPkRecordEntry)*_PK_RECORD_WIN_COUNT_] ;
	BYTE			bLoseRecord[sizeof(__cntPkRecordEntry)*_PK_RECORD_LOSE_COUNT_] ;		// 
	BYTE			bAbusingRecord[sizeof(__svrPkRecordEntry)*_PK_RECORD_ABUSING_CHECK_] ;

	BYTE			bMoonpaCount[sizeof(__cntPkCount)*_NUM_MOONPA_] ;
	BYTE			bClassCount[sizeof(__cntPkCount)*_NUM_CLASS_] ;
} ;

struct __client_quickslot
{
	u_char		slotType;		// _XQS_
	short 		skillID;
	short 		lifeSkillID;
	char		itemCType;
	short 		itemSID;
	u_short		invenNumber;	
	u_char		reserved;		// must be zero
	u_char		reserved2;		// must be zero
};	// 12 bytes

// 30��. : 360 bytes

#define _QUICKSLOT_COUNT_		(30)		//
#define _HELPLOG_MAX_SIZE_		(4*4)


struct _CHARAC_ETC_QUICKSLOT
{
	__client_quickslot		m_QuickSlot[_QUICKSLOT_COUNT_];
	u_char					m_helplog[_HELPLOG_MAX_SIZE_];
	bool					m_bChanged;
};

#ifdef _PD_PKEVENT_BOX_SPAWN_

struct _CHARAC_BW_WAR
{
	int				iWinTime;				// time() �Լ��� ���ϰ�.
};

#endif	// _PD_PKEVENT_BOX_SPAWN_

class _CHARAC {			/* ĳ���� ����ü�� ���� ����ü	*/
public:
	_CHARAC_BASIC			CHARAC_BASIC;			//ND_V01_Charac
	_CHARAC_CUR_BASIC		CHARAC_CUR_BASIC;		//ND_V01_CurBasicInfo
	_CHARAC_LEVEL			CHARAC_LEVEL;			//ND_V01_5StarLevel
	_CHARAC_STATE			CHARAC_STATE;			//ND_V01_CharacState	
	_CHARAC_INVENTORY		CHARAC_INVENTORY;		//ND_V01_Inventory
	_CHARAC_QUEST_DATA		CHARAC_QUEST_DATA;		//ND_V01_User_Quest_Data
	_CHARAC_MARTIAL_DATA	CHARAC_MARTIAL_DATA;	//ND_V01_Charac_Martial_Data
	_CHARAC_NIC				CHARAC_NIC ;			//ND_V01_Charac_Nic
	_CHARAC_MATCH			CHARAC_MATCH ;			//ND_V01_Charac_Match
	_CHARAC_EFFECT			CHARAC_EFFECT ;
	_CHARAC_PK_RECORD		CHARAC_PK_RECORD ;
	_CHARAC_ITEM_USE_STATUS		CHARAC_ITEM_STATUS ;

#ifdef _PD_PKEVENT_BOX_SPAWN_
	_CHARAC_BW_WAR			CHARAC_BW_WAR;
#endif	// _PD_PKEVENT_BOX_SPAWN_

#ifdef _PD_MASTER_PUPIL_
	_CHARAC_MP				CHARAC_MP;
#endif	// _PD_MASTER_PUPIL_

	_CHARAC_ETC				CHARAC_ETC;				//��Ÿ����	

	inline	void ResetCharac() { memset( this, 0, sizeof(*this) ) ; }
} ;

class _C_CHARAC : public _CHARAC
{
public:
	_CHARAC_ETC_QUICKSLOT		CHARAC_ETC_QUICKSLOT;

#ifdef _PD_CHARAC_LTS_
	_CHARAC_LTS					CHARAC_LTS;
#endif

public:
	CTraceAbleLock m_charac_cs;
#define charac_lock()		m_charac_cs.tlock( __FILE__, __LINE__ ) ;
#define charac_unlock()		m_charac_cs.tunlock( __FILE__, __LINE__ ) ;

	inline void Reset() 
	{
		ResetCharac();
		ZeroMemory( &CHARAC_ETC_QUICKSLOT, sizeof(CHARAC_ETC_QUICKSLOT) ) ;
#ifdef _PD_CHARAC_LTS_
		ZeroMemory( &CHARAC_LTS, sizeof(CHARAC_LTS) );
#endif

	}

	_C_CHARAC() 
	{
		Reset() ;
	}
	~_C_CHARAC() 
	{
		Reset() ;
	}

#ifdef _PD_CHARAC_LTS_
	bool lts_add_pk( _sLTS_Data& data );
	bool lts_del_pk( char * pDel );
	bool lts_add_sp( _sLTS_Data& data );
	bool lts_del_sp( char * pDel );
#endif

	
private:
	/*
	_C_CHARAC & _C_CHARAC::operator=( const _C_CHARAC & rhs )
	{
		try{
			if( this == &rhs ) { return *this ; }
			memcpy( this, &rhs, sizeof(_CHARAC) ) ;
		}
		catch(...){
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}
		return *this ;
	}

	_C_CHARAC & _C_CHARAC::operator=( const _CHARAC & rhs )
	{
		try{
			if( this == &rhs ) { return *this ; }
			memcpy( this, &rhs, sizeof(_CHARAC) ) ;
		}
		catch(...){
			::PrintConsole( "[EXCEPTION] %s>%d\n", __FILE__, __LINE__);
		}
		return *this ;
	}
	*/
	
	

};

#define PCHARAC _C_CHARAC*

//////////////////////////////////////////////////////
//	�̰��� ���ǵǴ� ����ü���� ���� �޸𸮿� �����	//
//	�����鿡 ���� ����ü��.. �׷��� ��Ŷ ����ü����	//
//	�����̴�.. ���� ���ǵ� ����ü�� ������ ����޸�	//
//	���� ������ �ȴ�..								//
//////////////////////////////////////////////////////

#define	_DEPOT_SLOT_NUM	40

#define  _DEPOT_BYTE_SIZE_		(sizeof(_sServer_InvenItem_v1) * _DEPOT_SLOT_NUM)
#define _EXT_DEPOT_BYTE_SIZE_	(sizeof(_sServer_InvenItem_v1) * 20)

struct _USER_DEPOT
{
	BYTE			cDepot[_DEPOT_BYTE_SIZE_];
#ifdef _PD_CASH_INVEN_
	BYTE			cExtendDepot[_EXT_DEPOT_BYTE_SIZE_];	// â�� Ȯ�� 20��.
#endif
	unsigned int	iMoney     ; // u_int �´�. 
	char			cCheckCount; 
	short			sCheckYear ; 
	char			cCheckMonth;
	char			cCheckDay  ;
	char			cCheckHour ;

	short			sVersion ;

	_sServer_InvenItem_v1 * GetSlot( u_char slot )
	{
		_sServer_InvenItem_v1 * p = NULL;
		if( slot < _DEPOT_SLOT_NUM )
			p = reinterpret_cast<_sServer_InvenItem_v1*>( &cDepot[slot*sizeof(_sServer_InvenItem_v1)] );
#ifdef _PD_CASH_INVEN_
		else if( slot < (_DEPOT_SLOT_NUM+20) )
		{
			p = reinterpret_cast<_sServer_InvenItem_v1*>( &cExtendDepot[(slot-_DEPOT_SLOT_NUM)*sizeof(_sServer_InvenItem_v1)] );
		}
#endif
		return p;
	
	}

};

#ifdef ITEMSHOP_PATCH_200607
// �������� ����ü
#define PRESENT_BOX_SLOT_SIZE		25
#define PRESENT_MESSAGE_BUF_SIZE	100
struct PRESENT_BOX {
	u_short		usItem;			// ������ �ε���
	u_char		ucItemCnt;		// ������ ����
	u_short		usExpireDay;	// ��ȿ�� ��
	char		pszMsg[PRESENT_MESSAGE_BUF_SIZE];	// ���� �޽���
};
#endif

#ifdef	_NXPCB_AWARD_200703_
struct NEXON_PCB_INFO {
	UCHAR	ucResult;
	UCHAR	ucAddrDesc;
	UCHAR	ucAccDesc;
	UCHAR	ucOption;
	int		iArgument;
	bool	bIsPCBBillType;
};
#endif

class _USER {							//	USER ������ ���� ����ü..
public:
	int					iUid;					//	����ũ ���̵�	
	char				cNDAccount[en_max_lil+1] ;		//	���� ���� : �̰� Define �Ȱɰ� ���ĵ� �ȴ�. 
#ifdef _NXPCB_AWARD_200703_
	char				cNXAcc[en_max_lil+1];			//	�ؽ� ����
#endif
	char				cPassword[en_max_lpl+1] ;			//	�н�����	
	u_short				usTempUniqueID;			//	LS �� �ο��� �ӽ� ����ũ���̵�
	u_int				iIp ;					//  USER �� IP Add
	int					iTime ;					//  USER �� ���� ���� �ð�
	DWORD				dwLoginTime ;			//  �α����ѽð�(�д���)
	int					iServer;				//	��Ʈ ����	
	char				cCharacCount;			//	ĳ������ ����
	char				cSelChrName[13];		//	���õ� ĳ���� �̸�
	char				cState;					//  ����	
	_USER_DEPOT			stUserDepot;			//	���� ��â
	_C_CHARAC			stCharac[3];			//	ĳ���� 3���� �迭��..
	unsigned			cDepotBackupState : 1 ;	//	â���� ��� ����
	unsigned			bMemState : 1 ;			// �޸𸮰� ���������..
	unsigned			bCheckCurEvent : 1;		// 0 - not checked, 1 - checked. 20080204
	unsigned			padding : 5 ;			// ���Ŀ� ����..
	
	char				cClass ;				// ������ ��� 18: �����, 19 : �����ο�

	_ACCOUNT_ITEM_USE_STATUS	AIUS ;			// 

#ifdef ITEMSHOP_PATCH_200607
	//PRESENT_BOX			PresentBox[PRESENT_BOX_SLOT_SIZE];
#endif

#ifdef _METEL_LOGIN_200608_
	BOOL			bPCBang;			// �Ǿ��� �������� �ƴ���(TRUE = �Ǿ��� ����)
	BOOL			bBillingLogin;		// ���� �α��� ����(TRUE = BillingLogin() ȣ��ǰ�, BillingLogout() ���� ȣ����� ���� ����)
										// BillingLogin() �ߺ� ȣ���� �����ϱ� ���� ����
	METEL_BILLING_INFO	MetelBillingInfo;
#endif //_METEL_LOGIN_200608_

#ifdef _VINA_SHUTDOWN_20061023_
	_FATIGUE_ELEMENT_  FatigueElement ;
	#ifdef _PD_VINA_PCROOM_INFO_
	u_char				ucPCRoomInfo;	// 0 - Home, else PC Room.
	#endif
#endif

#ifdef	_NXPCB_AWARD_200703_
	NEXON_PCB_INFO	NexonPcbInfo;
#endif

#ifdef _PD_EVENT_INVITE_FRIEND_
	int				iIF_DBIndex;
	int				iIF_EndTime;
#endif

	inline void Reset() 
	{ 

		iUid = 0 ;							//	����ũ ���̵�	
		memset( cNDAccount, 0, en_max_lil+1 ) ;			//	����
		memset( cPassword, 0, 13 ) ;		//	�н�����	
		usTempUniqueID = 0 ;				//	LS �� �ο��� �ӽ� ����ũ���̵�
		iIp = 0  ;							//  USER �� IP Add
		iTime = 0 ;							//  USER �� ���� ���� �ð�
		dwLoginTime = 0 ;					//  �α����ѽð�
		iServer = 0 ;						//	��Ʈ ����	
		cCharacCount = 0 ;					//	ĳ������ ����
		memset( cSelChrName, 0, 13 );		//	���õ� ĳ���� �̸�
		cState = 0 ;						//  ����	
		memset( &stUserDepot, 0, sizeof(_USER_DEPOT) );			//	���� â��
		
		
		cDepotBackupState = 0 ;				//	â���� ��� ����
		bMemState = 0 ;						// �޸𸮰� ���������..
		padding = 0 ;						// ���Ŀ� ����..
		
		ZeroMemory( &AIUS, sizeof(_ACCOUNT_ITEM_USE_STATUS) ) ;
        
		for( int i = 0 ; i < 3 ; i++ )
		{
			stCharac[i].Reset() ;
		}

#ifdef _METEL_LOGIN_200608_
		bPCBang = FALSE;
		bBillingLogin = FALSE;
		memset(&MetelBillingInfo, 0, sizeof(METEL_BILLING_INFO));
		MetelBillingInfo.iLoginSN = -1;
#endif //_METEL_LOGIN_200608_

#ifdef	_NXPCB_AWARD_200703_
		memset( cNXAcc, 0, en_max_lil+1 ) ;			//	����
		memset(&NexonPcbInfo, 0, sizeof(NEXON_PCB_INFO));
#endif
#ifdef _VINA_SHUTDOWN_20061023_
		memset( &FatigueElement, 0, sizeof(FatigueElement) );
#endif
#ifdef _PD_EVENT_INVITE_FRIEND_
		iIF_DBIndex = 0;
		iIF_EndTime = 0;
#endif
	}	
	
} ;


class CUser : public _USER
{

private :

public :
	CTraceAbleLock m_user_cs;

#define user_lock()			m_user_cs.tlock( __FILE__, __LINE__ ) ;
#define user_unlock()		m_user_cs.tunlock( __FILE__, __LINE__ ) ;

	static int	get_time_by_sec() { return GetTime_ByIntSec() ; } ;


	SOCKADDR_IN		m_myLsAddr ;

	CUser() 
	{
        Reset() ;
	}
	~CUser() 
	{
		Reset() ;
	}

	inline	_C_CHARAC * characFind( char * cCharacName ) 
	{
		if( cCharacName == NULL || cCharacName[0] == 0 )
		{
			return NULL ;
		}
		_C_CHARAC * pCharac = NULL ;

		user_lock() ;
		try{
			for( int i = 0 ; i < 3 ; i++ )
			{
				if( strcmp( stCharac[i].CHARAC_BASIC.cChrName, cCharacName ) == 0 )
				{
					pCharac = &stCharac[i] ;
					break ;
				}			
			}
		}
		catch(...){
			::PrintConsole( "[EXCEPTION] %s %d \n", __FILE__, __LINE__) ;
		}		
		user_unlock() ;

		return pCharac ;
	}

} ;

#define PUSER CUser*


// �Ʒ� ����ü �ϳ��� ��� ���� ������ ����Ʈ �� �� �ֵ���, ���������� ��� �����Ѵ�. 
// ĳ���� ����Ʈ�� �� ����ü
struct _CharacList								// # [����] ĳ������ �⺻ ���� #
{
	int					iUniqueID;				// ĳ������ ����ũ ���̵�
	char				cName[13] ;				// ĳ���� �� �̸�
	char				cNick[13] ;				// ĳ���� ��Ī

	char				cZone ;

	char				cSex ;					// ���� 0 : ����, 1 : ����
	char				cFace ;					// ���� : max 16�� 
	char				cHair ;					// �Ӹ� ��Ÿ�� : max 16�� 
	char				cGroup ;				// �Ҽ� ����

	char				cAmorType ;					// ����	0
	char				cPantsType ;				// ����	1
	char				cCapType ;					// ���� 4
	char				cAdaeType ;					// �ƴ� 2
	char				cShoesType ;				// �Ź� 3	
	char				cBackPackType ;				// �賶 7
	char                cWeaponType ;               // ���� Ÿ��
	char				cMaskType ;				// ����.

	short				sAmor ;					// ����	0
	short				sPants ;				// ����	1
	short				sCap ;					// ���� 4
	short				sAdae ;					// �ƴ� 2
	short				sShoes ;				// �Ź� 3	
	short				sBackPack ;				// �賶 7	
	short				sSelectWeaponID ;		// ������ ���� �ε���. 0, 1 ( ���� ����1, 2 )
	short				sMask ;					//
	u_char				ucWeaponStrength ;		// ���� ������ ���� ����.

	short				sStr ;					// �ٷ� <= ������ ����
	short				sZen ;					// ���� <= �״��
	short				sIntelligence ;			// ���� <= ����
	short				sConstitution ;			// �ǰ� <= ����
	short				sDexterity ;			// ��ø <= �ɱ�

	short				sInnerLevel ;			// ���� ���� 

	// 2004/11/10 Ray �߰�. class, classgrade
	char				cClass ;				// ���� 
	char				cClassGrade ;			// ���� ���
	char				cGM ;					// GM ����..  (1 �̻� GM,  -1���� :��ŷ)

	// 2005/01/07 Ray �߰� : ����â���� ĳ������ ��ġ ǥ��.
	float				fPosX ;					// ���� ��ġ.
	float				fPosZ ;					//

	int					iBlockingEndDate ;		// ��ŷ ���� �ð�

	u_char				ucChangeNameFlag : 2 ;	// 0 - Not, 1 - Possible Change Name
	u_char				ucLeftFlag : 6 ;		// ���� �Ⱦ���.

} ;

// �κ��丮

// ������ 0 : 2005-10-13�� ����.
// �κ��丮�� ���� ������ 1�̴�.
#define _VERSION_INFO_INVEN_LAST_		1
#define _V0_COUNT_INVEN_ITEM_			103		// ������ ���� ����.
#define _INVENTORY_MAX_COUNT_           95		// v0������ �κ��丮 ����. �� ���� �����κ������� ���۹�ȣ�� ����.
#define _TEMP_INVEN_MAXNUM_             8       // ���ļ� 103
#define _V1_COUNT_INVEN_ITEM_			108		// 25(����) + 75(�κ��丮) + 8(�ӽ�)


#define _ITEM_TYPE_COUNT_			20 // ��з� ���� 10����.


#define _ITEM_TYPE_WEAPON_			0  // ���� 
#define _ITEM_TYPE_CLOTH_			1  // �ǻ�
#define _ITEM_TYPE_BOOK_			2  // ���
#define _ITEM_TYPE_POTION_			3  // ȸ����	   // ��뿡 �ش���. 
#define _ITEM_TYPE_CONSUMING_		4  //

#define _ITEM_TYPE_ELIXIR_          5

#define _ITEM_TYPE_COUNTERACT_		6  // �ص� �ֹ��� // ��뿡 �ش���. 
#define _ITEM_TYPE_RESOURCE_		7  // �ڿ�
#define _ITEM_TYPE_QUEST_			8  // ����Ʈ�� ������ 
#define _ITEM_TYPE_ACCESSORY_		9  // �Ǽ��縮:�Ͱ���/����/�����/�賶
#define _ITEM_TYPE_LIFE_			10 // ��Ȱ ����
#define _ITEM_TYPE_MUSIC_			11 // �Ǳ�
#define _ITEM_TYPE_PASS_			12 // ������
#define _ITEM_TYPE_MONEY_			13 // ��(��)
#define _ITEM_TYPE_SOCKET_          14 // ���� ���Կ� ���͸���....
#define _ITEM_TYPE_WEAPON2_			15 // ���� Ȯ����
#define _ITEM_TYPE_CLOTH2_			16 // ��   Ȯ����
#define _ITEM_TYPE_BOX_             17
#define _ITEM_TYPE_BOXKEY_          18 
#define _ITEM_TYPE_WEAPON3_			19



enum equip_pos			// ������ ���� ��ġ �ε���.
{
	pos_amor=0,         // 0  ������ ��� Pocket ������ �����Ѵ�. 
	pos_pants,			// 1  ������ ��� Pocket ������ �����Ѵ�. 
	pos_adae,			// 2
	pos_shoes,			// 3
	pos_cap,			// 4
	pos_gap_upper,		// 5
	pos_gap_lower,		// 6
	pos_backpack,		// 7
	pos_weapon_1,		// 8
	pos_weapon_2,		// 9

	pos_accessory_1,	// 10 ���� 0,1�� Pocket������ ���� ���������� Ȱ��ȭ �Ǿ���Ѵ�.  ���ǿ�...
	pos_accessory_2,	// 11
	pos_accessory_3,	// 12
	pos_accessory_4,	// 13

	pos_accessory_5,	// 14  ���ǿ�.
	pos_accessory_6,	// 15
	pos_accessory_7,	// 16
	pos_accessory_8,	// 17
	pos_backpack2,      // 18 ���� �ι�°.
	pos_reserved        // 19 �������.  ������ ���� �ʴ´�. 
} ;


// v1 �κ��丮�� ������ 25 �Դϴ�.
enum v1_equip_pos
{
	v1_pos_amor=0,                            // 0  ������ ��� Pocket ������ �����Ѵ�. 
	v1_pos_pants,                               // 1  ������ ��� Pocket ������ �����Ѵ�. 
	v1_pos_adae,                               // 2
	v1_pos_shoes,                             // 3
	v1_pos_cap,                               // 4
	v1_pos_gap_upper,                      // 5
	v1_pos_gap_lower,                       // 6

	v1_pos_mask,                               // 7  ����ũ
	v1_pos_cloth_reserve,     // 8  �ǻ� ���� ��.
	v1_pos_cloth_reserve2,   // 9  �ǻ� ���� 2.

	v1_pos_weapon_1,                       // 10
	v1_pos_weapon_2,                       // 11

	v1_pos_amor_acc_1,                   // 12 ���� 0,1�� Pocket������ ���� ���������� Ȱ��ȭ �Ǿ���Ѵ�.  ���ǿ�...
	v1_pos_amor_acc_2,                    // 13
	v1_pos_amor_acc_3,                    // 14
	v1_pos_amor_acc_4,                    // 15

	v1_pos_pants_acc_1,                   // 16 ���ǿ� �Ǽ��縮.
	v1_pos_pants_acc_2,                   // 17
	v1_pos_pants_acc_3,                   // 18
	v1_pos_pants_acc_4,                   // 19

	v1_pos_extra_pocket,      // 20 ����ǰ ���� ����.
	v1_pos_reserve1,                          // 21
	v1_pos_reserve2,                          // 22

	v1_pos_backpack1,                       // 23
	v1_pos_backpack2                        // 24 ���� �ι�°.
} ;


struct _sUser_InvenItem						// ������ �κ��丮�� �� ������ �׸�
{	
	char				cType   ;		// ������ ��з�
	char				cSecond ;
	short				sID     ;		// ������ �Һз�. (������ ���̵�)
	char				cIdentify     : 4;	// ���� ����
	u_char              ucSocketCount : 4;      // ������ ��� ���ȴ���.
	u_char				ucCount  ;		// �ִ� ���� 255��
	u_short				usCurDur ;		// ���� ������
	u_char              ucSocket[4];        	// ���� 1,2,3,4
} ;	






inline void ConvertToLastVersion( OUT short & sInvenVersion, OUT _sServer_InvenItem_v1 * pInvenV1 , OUT _sServer_InvenItem * pInven)
{
	if( sInvenVersion == 0 )
	{
		::ZeroMemory( pInvenV1, sizeof(_sServer_InvenItem_v1)* _V1_COUNT_INVEN_ITEM_ ) ;

		int i ;

		// ���� �κ� ��ȯ.
		for( i = 0 ; i < pos_reserved + 1 ; ++i )
		{
			if( pInven[i].sID )
			{
				switch( i )
				{
				case pos_amor:			// 0  ������ ��� Pocket ������ �����Ѵ�. 
					pInvenV1[v1_pos_amor].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_pants:			// 1  ������ ��� Pocket ������ �����Ѵ�. 
					pInvenV1[v1_pos_pants].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_adae:			// 2
					pInvenV1[v1_pos_adae].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_shoes:			// 3
					pInvenV1[v1_pos_shoes].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_cap:			// 4
					pInvenV1[v1_pos_cap].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_gap_upper:		// 5
					pInvenV1[v1_pos_gap_upper].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_gap_lower:		// 6
					pInvenV1[v1_pos_gap_lower].convert_from_v0( &pInven[i] ) ;
					break ;

				case pos_backpack:		// 7
					pInvenV1[v1_pos_backpack1].convert_from_v0( &pInven[i] ) ;
					break ;


				case pos_weapon_1:		// 8
					pInvenV1[v1_pos_weapon_1].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_weapon_2:		// 9
					pInvenV1[v1_pos_weapon_2].convert_from_v0( &pInven[i] ) ;
					break ;


				case pos_accessory_1:	// 10 ���� 0,1�� Pocket������ ���� ���������� Ȱ��ȭ �Ǿ���Ѵ�.  ���ǿ�...
					pInvenV1[v1_pos_amor_acc_1].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_accessory_2:	// 11
					pInvenV1[v1_pos_amor_acc_2].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_accessory_3:	// 12
					pInvenV1[v1_pos_amor_acc_3].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_accessory_4:	// 13
					pInvenV1[v1_pos_amor_acc_4].convert_from_v0( &pInven[i] ) ;
					break ;


				case pos_accessory_5:	// 14  ���ǿ�.
					pInvenV1[v1_pos_pants_acc_1].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_accessory_6:	// 15
					pInvenV1[v1_pos_pants_acc_2].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_accessory_7:	// 16
					pInvenV1[v1_pos_pants_acc_3].convert_from_v0( &pInven[i] ) ;
					break ;
				case pos_accessory_8:	// 17
					pInvenV1[v1_pos_pants_acc_4].convert_from_v0( &pInven[i] ) ;
					break ;


				case pos_backpack2:		// 18 ���� �ι�°.
					pInvenV1[v1_pos_backpack2].convert_from_v0( &pInven[i] ) ;
					break ;
				}
			}
		}


		int j; 
		for( i = pos_reserved + 1, j = 1 ; i < _V0_COUNT_INVEN_ITEM_ ; ++i, ++j )
		{
			if( pInven[i].sID )
			{
				pInvenV1[j+v1_pos_backpack2].convert_from_v0( &pInven[i] ) ;
			}
		}

		sInvenVersion = 1 ;
	}
}





/*

// ����ũ ���̵� ���鶧 ȣ���ϴ� �Լ���. �Ľ̽ÿ� ������ �����ϴµ� ������ �� �� ���Ƽ� ÷����
extern volatile LONG lUniqueItemID;
extern volatile u_short usItemUniqueIDHour;

unsigned __int64 CItemManager::SM_Item_Make_UniqueID( )
{
unsigned __int64 ui64Temp = 0L;
SYSTEMTIME Date;
GetSystemTime(&Date);
u_char  * pcTemp = NULL;
u_short * psTemp = NULL;

pcTemp    = reinterpret_cast<u_char*>( &ui64Temp );
pcTemp[0] = static_cast<u_char>( g_sServerNo );
pcTemp[1] = static_cast<u_char>( Date.wYear - 2000 );
pcTemp[2] = static_cast<u_char>( Date.wMonth );
pcTemp[3] = static_cast<u_char>( Date.wDay );
pcTemp[4] = static_cast<u_char>( Date.wHour );
pcTemp[5] = static_cast<u_char>( Date.wMinute );

psTemp    = reinterpret_cast<u_short*>( &pcTemp[6] );
*psTemp   = static_cast<u_short>( lUniqueItemID ); 

if(	usItemUniqueIDHour != Date.wHour ){ lUniqueItemID = 0L; usItemUniqueIDHour = Date.wHour;}
else{ InterlockedIncrement( &lUniqueItemID ); }

return ui64Temp;
}
*/



struct _sCharac_Skill
{
	short	id ;
	char	cStep ;			    // ���� ���� �ܰ�

	u_char	ucSkillState : 2 ;	// 0 : ���� �Ϸ�  1 : �̿Ϸ� : ���ýÿ��� ��밡��. 2, 3,...
	u_char	ucPassiveState : 1 ;// 0 : Off, 1 : On for passive only
	u_char	ucCurMission : 5 ;	// �̼� ���� ī��Ʈ	0 - 31
	int		iCurSkillExp ;		// ���� ������ ����ġ

	u_char	ucPassiveSelectStep ;	// 
	u_char	ucTemp ;
};


//#ifdef NEW_QUEST_PACKET
/* ============================================================================
����Ʈ ���� 
============================================================================ */
struct _sCharacQuestState
{
	bool	bReword  : 1 ;	// ������ �޾Ҵ°�
	u_char	ucNodeID : 7 ;	// �� Node ���̵� ����ε� ĳ���ͷ� �ؾ��Ѵ�.
	u_short usQuestID;      // �� ����, �⿬, ���� ����Ʈ.

	_sCharacQuestState()
	{
		bReword   = false;
		ucNodeID  = 0;
		usQuestID = 0;
	}
} ;

/* ============================================================================
���� ����Ʈ ����� ���õ� ����ü. 
============================================================================ */
struct _sCharacMainQuestState
{
	bool	bReword  : 1 ;	// ������ �޾Ҵ°�
	u_char	ucNodeID : 7 ;	// �� Node ���̵�
	u_short usQuestID;      // �� ������ ����Ʈ ���̵�. ����� �ϴ� ���� ���� ������?
	u_char  padding;        // ������ ����.

	_sCharacMainQuestState()
	{
		padding   = 0;      // ������ ���� �÷��׸� ���� ������.
		usQuestID = 0;
		ucNodeID  = 0;
		bReword   = false;
	}
} ;
//==========================================
// ����Ʈ ������ ����ü (�Ӽ� ��)
//==========================================
struct _sQuestItem
{
	short		sID ;			// ������ �Һз�. (������ ���̵�)	
	u_int		uiCount ;		// ���̳� ȭ���/��ô���� ����.
	_sQuestItem() : sID(0), uiCount(0)
	{}
} ;

/*struct _sQuestItem_Pair
{
char cType;
union {
struct {
char		cSecond ;		// ������ �ߺз�
char		cIdentify ;		// 0,1,2,3 ������.
short		sID ;			// ������ �Һз�. (������ ���̵�)  ** �Ⱥ��δٸ�.. �ʿ� ����.
} S_detail ;
u_int			uiCount ;
} ;
u_char				ucCount ;		// ����
char                padding[4];
};*/ 

//===========================================
// ����Ʈ ���� ����ü.
//===========================================
#define QUEST_HISTORY_COUNT 50 

struct _sQuestHistory
{
	char cNodeHistory;	 // -1
	char cBranchHistory;	// 0
};

struct _sRunningQuest
{
	u_short usQuestID;

	char  cNodeID;
	int   dwQuestStartTime;
	int	  dwQuestTimer;
	char  cTimerNodeNo;
	bool  bBeUse;

	char   cTFRetry      : 4 ;     /* �� ������ �Ϸ� �Ҷ� true�׼� retry���� false�׼� retry���� */ 

	bool   bCounter1Flag : 1;      /* ī���Ͱ� ������ ���� �ִ� ���ΰ�? �ƴϸ� �׳� �Ϲ� ī�����ΰ�  �ʿ� ���� ����...*/ 
	bool   bCounter2Flag : 1;
	bool   bCounter3Flag : 1;
	bool   bCounter4Flag : 1;


	u_char ucSelectNo;		       /* �ִ� ���ð����� ���� �ѹ��� 255�� */ 
	u_char ucCounter[4];           /* ���� ī����. */ 
	short  sMobID[4];              /* ī�����÷��׿� ���� �����̵����� Ȥ�� ������ ���̵������� �ٲ�� */ 

	_sRunningQuest()
	{
		usQuestID        = 0;
		dwQuestTimer     = 0;
		dwQuestStartTime = 0;
		cTFRetry         = 0;	   /*�� ������ ��� �� ���� fail �׼��� ������ ����, true �׼��� ������ ����.*/ 

		bCounter1Flag    = false;  /*�� bit���� ī���� �÷��׸� ��Ÿ���µ� ����Ѵ�.*/ 
		bCounter1Flag    = false; 
		bCounter1Flag    = false; 
		bCounter1Flag    = false; 

		cNodeID          = 0;  
		cTimerNodeNo     = 0;
		bBeUse           = false; 
		ucCounter[0]     = 0;      /* �������� �̰� ���� */ 
		ucCounter[1]     = 0;
		ucCounter[2]     = 0;
		ucCounter[3]     = 0;
		sMobID[0]        = 0;
		sMobID[1]        = 0;
		sMobID[2]        = 0;
		sMobID[3]        = 0;
	}
} ; 



// ��������Ʈ������ ���� ������ �����丮�� ���ԵǾ�� �Ѵ�. 
// �����丮 �����ʹ� �б�ʹ� ������� ��尪�� �����ָ� �ȴ�.
struct _sRunningQuestPacketData
{
	u_short usQuestID;
	int   dwQuestStartTime;
	int	  dwQuestTimer;
	char  cTimerNodeNo;
	u_char ucSelectNo; 
	u_char ucCounter[4];

	_sRunningQuestPacketData()
	{
		usQuestID		 = 0;
		dwQuestStartTime = 0;
		dwQuestTimer	 = 0;
		cTimerNodeNo	 = 0;
		ucSelectNo       = 0;
		ucCounter[0]     = 0;
		ucCounter[1]     = 0;
		ucCounter[2]     = 0;
		ucCounter[3]     = 0;
	}
} ;




/* =====================================================================================
���� ���� ����..
===================================================================================== */

// ���� ���� ����ü(GLOBAL USE)

struct  _SERVER_SYSTEM_INFO{						// ���� ����		
#ifdef _GMS_SERVERNAME_LENGTH_INCREASED_
	char		cServerName[57];
#else
	char		cServerName[13];
#endif
	char		cOS[16];					// ��� OS		
	char		cCPU;						// CPU ����
	int			iTotalMem;					// �� �޸�
	int			iUseMem;					// ������� �޸�
	int			iTotalHDD;					// �� ��ũ �뷮
	int			iUseHDD;					// ������� ��ũ �뷮	

};

class _h_DS_Com ;
struct _SERVER_NET_INFO {
	_h_DS_Com *	ds_com;
	char		cIP[16];					// ��� IP
	short		sPort;						// UDP ��Ʈ��.
	SOCKADDR_IN	sockAddr ;
};

struct _SERVER_NET_INFO_GMS {	
	char		cIP[16];					// ��� IP
	short		sPort;						// UDP ��Ʈ��.	
};

struct _SERVER_STATE_INFO {
	char		cState;
	int			iConnectCount;
	int			iPlayingCount;

	int			iLevelExp;
	int			iSkillExp;

};


struct _POS_LIST {
	float	x;
	float	z;
	char	cName[16];
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// \class _sCombatRecord
/// \brief �������� ����� ������� - ����� �� 4�� ���� 
class _sCombatRecord
{
public:	
	_sCombatRecord(){InitializeCombatRecord();}
	~_sCombatRecord(){};

public:	
	inline	unsigned short	GetManorIndex(){return m_sManorIndex;}
	inline	void	SetManorIndex(short index){m_sManorIndex = index;}
	inline	unsigned short	GetEnrtyCount(){return m_sEntryCount;}
	inline	void	SetEnrtyCount(short entryCount){m_sEntryCount = entryCount;}
	inline	unsigned short	GetWinCount()	{return m_usWinCount;}
	inline	void	SetWinCount(unsigned short winCount){m_usWinCount = winCount;}
	inline	unsigned short	GetLoseCount()	{return m_usLoseCount;}
	inline	void	SetLoseCount(unsigned short loseCount){m_usLoseCount = loseCount;}
	inline	unsigned short	GetCombatCount()	{return m_usCombatCount;}
	inline	void	SetCombatCount(unsigned short combatCount){m_usCombatCount = combatCount;}
	inline	_sND_TimeUnit		GetCombatDate()	{return m_combat_date;}
	inline	void	SetCombatDate(_sND_TimeUnit* date)
	{
		memcpy(&m_combat_date,date,sizeof(_sND_TimeUnit));
	}


	bool	InitializeCombatRecord()
	{
		m_sManorIndex	= 0;
		m_sEntryCount	= 0;
		m_usWinCount	= 0;
		m_usLoseCount	= 0;
		m_usCombatCount	= 0;
		return true;
	}

private:	
	short				m_sManorIndex;	///<	��� �ε���
	short				m_sEntryCount;	///<	����� ���� Ƚ�� 
	unsigned short		m_usWinCount;	///<	����� ��  �¼�
	unsigned short		m_usLoseCount;	///<	����� ��  ��
	unsigned short		m_usCombatCount;///<	����� ��  ���� 
	_sND_TimeUnit		m_combat_date;	///<	����� ���� ��¥ 
};





// ��ȸ �ִ� �ο��� :200�� 
const	unsigned short MAX_PERSONAL_COUNT		= 256;
// ��20��(�ڽ� ����) ��ȸ��� �ϰ� �ִ� 5*21 = 105��
const	unsigned short MAX_ORG_COUNT			= 128;
// ��� ���� 4�� ���� 
const	unsigned short MAX_MANOR				= 2;
// ����� ����,���� ������ ���������� ������ �߰����� 10������ ����
// ����� �� _sPersonalRecord�� ���� �����Ƿ� 10���� ��ŷ üũ 
const	unsigned short	MANOR_RANK_ANNOUNCE		= 10;
// ������� ������ ���� �� ���� ���� `07.04.04 ���� ��õ�� �� ������ ���� 998�� 
const   unsigned short  MAX_ORG_COUNT_INSERVER	= 256;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// \class	_sOrgRecord
/// \brief	��� �����ܰ��� ��/�� ��� 
class _sOrgRecord
{
public:
	_sOrgRecord()
	{
		m_orgIndex = 0;
		InitializeOrzRecord();
	}
	~_sOrgRecord(){}
public:
	inline	unsigned short	GetIndex()	{return m_orgIndex;}
	inline	void	SetIndex(int index){m_orgIndex = index;}
	inline	unsigned short	GetAllyIndex()	{return m_AllyIndex;}
	inline	void	SetAllyIndex(int index){m_AllyIndex = index;}
	inline	unsigned short	GetWinCount()	{return m_usWinCount;}
	inline	void	SetWinCount(unsigned short winCount){m_usWinCount = winCount;}
	inline	unsigned short	GetLoseCount()	{return m_usLoseCount;}
	inline	void	SetLoseCount(unsigned short loseCount){m_usLoseCount = loseCount;}
	inline	unsigned short	GetCombatCount()	{return m_usCombatCount;}
	inline	void	SetCombatCount(unsigned short combatCount){m_usCombatCount = combatCount;}
    inline	u_char*	GetVsOrgName()	{return m_vsOrgName;}
	inline	void	SetVsOrgName(u_char* name)
	{
		memcpy(m_vsOrgName,name,24);
		m_vsOrgName[24]=0;
	}

	bool _sOrgRecord::InitializeOrzRecord()
	{
		m_orgIndex		= 0;
		m_usWinCount	= 0; 
		m_usLoseCount	= 0;
		m_usCombatCount	= 0;	
		return true;
	}	


	bool _sOrgRecord::AddRecord(const bool bResult)
	{
		if(bResult) //true
		{
			++m_usWinCount;
			++m_usCombatCount;
		}
		else 
		{
			++m_usLoseCount;
			++m_usCombatCount;
		}
		return true;
	}

	_sOrgRecord&  operator =(const _sOrgRecord& rhs)
	{
		m_orgIndex		= rhs.m_orgIndex;
		SetVsOrgName( const_cast<u_char*>(rhs.m_vsOrgName) );
		m_usWinCount	= rhs.m_usWinCount;
		m_usLoseCount	= rhs.m_usLoseCount;
		m_usCombatCount = rhs.m_usCombatCount;
		m_AllyIndex		= rhs.m_AllyIndex;
		return *this;
	}

private:
	///<	Ÿ ������(ȸ) �ε���
	int					m_orgIndex;	
	///<	Ÿ �� �� �̸�
	u_char				m_vsOrgName[25];
	int					m_AllyIndex;		//Ÿ�� �ε��� 0�̸� ���̰� 0�� �ƴϸ� ȸ�� �ε����̴�
	///<	�� �����ܺ� �¼�
	unsigned short		m_usWinCount;
	///<	�� �����ܺ� ��
	unsigned short		m_usLoseCount;
	///<	�� �����ܺ� ���� 
	unsigned short		m_usCombatCount;	
};

struct _effect
{
	short		id ;				// effect table(EffectScript.txt ����)�� ���ǵǾ� �ִ� id
	short		prob ;				// ����ȿ���� �ߵ��� Ȯ��(skill table ����)
	short		value ;				// ����ȿ���� �ߵ��� �� ����� ��(skill table ����)
	u_short		effect_prop ;		// % ���� enum ��. ��ų ���̺��� ������ �����.
	//////////////////////////////////////////////////////////////////////////
	// Property
	// none = ����. �̰����� �����ϸ�, ó������ ����.
	// general 		= �Ϲ� �Ӽ�
	// initial_once		= ó���� �ѹ� ����ǰ� ������
	// apply_additive	= ������ ��󿡰� ����
	// bonus_additive	= ������ Additive : ���ݽÿ� ���ʽ��� ��.
	// reflection		= ���ø��� �ǵ���
	// only_once		= �� �ѹ� : �����ϸ� �����
	// feed			= ���ݽ� From ���� ��������

	char		value_type ;		// %
	//////////////////////////////////////////////////////////////////////////
	// Value Type : ���밪 ��꿡 ���.
	// add_value		= ������ ����
	// add_percent		= %�� ����
	// sub_value		= ������ ����
	// sub_percent		= %�� ����

	u_char		where ;				// % ��� ����...
	u_char		from ;				// % ��𿡼� ���� ���ΰ�.
	//////////////////////////////////////////////////////////////////////////
	// From Target Type : 'feed', 'trans' �Ӽ������� ����, none �� ���� ����. 
	// none  �⺻ �Ӽ� : �̰� ���� �ȵ�. ��ũ��Ʈ ������ ó����.
	// hit,			�� ���������� 
	// life,		������ ü�¿���
	// force,		������ ���¿���
	// concentration	������ ��������

	u_char		condition_flag_id ;	// %
	// EN_CONDITION_ID ����

	u_short		count ;				// apply_additive�� ����� �ð�

	char		bInterval ;			// ������ ���� ���ΰ�. 0/1 : ��κ� 0, �ߵ� � 1
	// true�� ���, 3�ʸ��� �����

	char		cPadding ;			// �е�.


	_effect *	pNext ;		// ���� ��ų�� NULL ��. 
	// �ϳ��� ��ų/������ ���� ���� effect�� �����Ǿ� ���� �� �����Ƿ�,
	// �� effect���� ����Ʈ�� �����ȴ�. ������ effect�� NULL ���� ���� ��.

	_effect()
	{
		::ZeroMemory( this, sizeof(_effect) ) ;
	}
} ;

#pragma pack( pop, enter_global_struct )
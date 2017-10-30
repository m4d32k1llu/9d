#ifndef _XGAME_ITEM_DEFINE_
#define _XGAME_ITEM_DEFINE_

#define _XDEF_MAXCOUNT_WEAPONITEMSEED		379
#define _XDEF_MAXCOUNT_WEAPONITEM			29115
#define _XDEF_MAXCOUNT_CLOTHESITEM			28899
#define _XDEF_MAXCOUNT_QUESTITEMSEED		32
#define _XDEF_MAXCOUNT_ETCITEMSEED			32
#define _XDEF_MAXCOUNT_RECOVERYITEMSEED		44


#include "d3dx9.h"
#include "XKernel.h"

#define _XDEF_ITEMTYPE_COUNT				21

// =======================================================
// ������ ��з�
//
// 0 : ����
// 1 : �ǻ�
// 2 : ���
// 3 : ȸ����
// 4 : ��ô��
// 5 : �ߵ� �ֹ���
// 6 : �ص� �ֹ���
// 7 : �ڿ�
// 8 : ����Ʈ ������
// 9 : �Ǽ��縮 (�Ͱ���/����/�����/�賶)
// 10 : ��Ȱ ����
// 11 : �Ǳ�
// 12 : ������
// 13 : ��(��)
// 14 : ����
// 15 : ����2
// 16 : �ǻ�2
// 17 : �пձ�
// 18 : �пձ� ����
// 19 : ����3
// 20 : �ǻ�3
//
// ========================================================
typedef enum _XGI_FirstCategory
{
	_XGI_FC_WEAPON = 0,			// ���� = 0
	_XGI_FC_CLOTHES,			// �ǻ� = 1
	_XGI_FC_BOOK,				// ��� = 2
	_XGI_FC_POTION,				// ȸ�� = 3
	_XGI_FC_CONSUMING,			// ��ô = 4
	_XGI_FC_ELIXIR,				// ���� = 5
	_XGI_FC_COUNTERACT,			// �ص� �ֹ��� = 6
	_XGI_FC_RESOURCE,			// �ڿ� = 7
	_XGI_FC_QUEST,				// ����Ʈ = 8
	_XGI_FC_ACCESSORY,			// �Ǽ��縮 = 9
	_XGI_FC_LIFE,				// ��Ȱ ���� = 10
	_XGI_FC_MUSIC,				// �Ǳ� = 11
	_XGI_FC_PASS,				// ������ = 12
	_XGI_FC_MONEY,				// �� = 13
	_XGI_FC_SOCKET,				// ���� = 14
	_XGI_FC_WEAPON2,			// ����2 = 15
	_XGI_FC_CLOTHES2,			// �ǻ�2 = 16
	_XGI_FC_BOX,				// �пձ� = 17
	_XGI_FC_BOXKEY,				// �пձ� ���� = 18
	_XGI_FC_WEAPON3,			// ����3 = 19
	_XGI_FC_CLOTHES3,			// �ǻ�3 = 20
};


// ========================================================
// ���� ������ �ߺз� : �Һз�
//
// 0 - �ܺ��� : 0-�˹�  1-����  2-Ÿ����  3-Ÿ��  4-�ǰ�  5-��Ÿ  6-�ι�
// 1 - �庴�� : 0-�뵵  1-â��  2-���
// 2 - ������ : 0-���  1-�߼�  2-���
// 3 - �Ϻ��� : 0-ǥâ  1-��  2-��ô  3-����  4-�߻�  5-�Ҹ�(��ź)
// 4 - �Ǻ��� : 0-��  1-��  2-��  3-��  4-��
// 5 - Ư������ : 0-��
//
// =========================================================
typedef enum _XGI_SecondCategory_Weapon		// ���� �ߺз�
{
	_XGI_SC_WEAPON_SHORT = 0,			// �ܺ��� = 0
	_XGI_SC_WEAPON_LONG,				// �庴�� = 1
	_XGI_SC_WEAPON_SOFT,				// ������ = 2
	_XGI_SC_WEAPON_HIDDEN,				// �Ϻ��� = 3
	_XGI_SC_WEAPON_MUSICAL,				// �Ǻ��� = 4
	_XGI_SC_WEAPON_SPECIAL				// Ư������ = 5
};

typedef enum _XGI_ThirdCategory_Weapon_Short	// �ܺ��� �Һз�
{
	_XGI_TC_WEAPON_SWORD = 0,			// �˹� = 0
	_XGI_TC_WEAPON_KNIFE,				// ���� = 1
	_XGI_TC_WEAPON_CLUB,				// Ÿ���� = 2
	_XGI_TC_WEAPON_HIT,					// Ÿ�� = 3
	_XGI_TC_WEAPON_FIST,				// �ǰ� = 3
	_XGI_TC_WEAPON_SECRET,				// ��Ÿ = 5
	_XGI_TC_WEAPON_AXE,					// �ι� = 6
	_XGI_TC_WEAPON_RING					// �� = 7
};

typedef enum _XGI_ThirdCategory_Weapon_Long		// �庴�� �Һз�
{
	_XGI_TC_WEAPON_GLAIVE = 0,			// �뵵 = 0
	_XGI_TC_WEAPON_SPEAR,				// â�� = 1
	_XGI_TC_WEAPON_STAFF,				// ��� = 2
};

typedef enum _XGI_ThirdCategory_Weapon_Soft
{
	_XGI_TC_WEAPON_STEELWHIP = 0,		// ��� = 0
	_XGI_TC_WEAPON_HAMMER,				// �߼� = 1
	_XGI_TC_WEAPON_CHAIN				// ��� = 2
};

typedef enum _XGI_ThirdCategory_Weapon_Hidden
{
	_XGI_TC_WEAPON_DART = 0,			// ǥâ = 0
	_XGI_TC_WEAPON_DAGGER,				// �� = 1
	_XGI_TC_WEAPON_JAVELIN,				// ��ô = 2
	_XGI_TC_WEAPON_NEEDLE,				// ���� = 3
	_XGI_TC_WEAPON_ARROWLAUNCHER,		// �߻� = 4
	_XGI_TC_WEAPON_MINE					// �Ҹ�(��ź) = 5
};

typedef enum _XGI_ThirdCategory_Weapon_Musical
{
	_XGI_TC_WEAPON_RYUNG = 0,			// �� = 0
	_XGI_TC_WEAPON_GO,					// �� = 1
	_XGI_TC_WEAPON_SO,					// �� = 2
	_XGI_TC_WEAPON_KUEM,				// �� = 3
	_XGI_TC_WEAPON_JUCK					// �� = 4
};

typedef enum _XGI_ThirdCategory_Weapon_Special
{
	_XGI_TC_WEAPON_SERING = 0				// �� = 0
};


// ===========================================================
// �ǻ� ������ �ߺз�
//
// 0 : ����
// 1 : ����
// 2 : �ƴ�
// 3 : �Ź�
// 4 : ����
// 5 : �䰩
// 6 : �Ű�
// 7 : �賶
//
// ===========================================================
typedef enum _XGI_SecondCategory_Clothes
{
	_XGI_SC_CLOTHES_UPPER = 0,		// ���� = 0
	_XGI_SC_CLOTHES_LOWER,			// ���� = 1
	_XGI_SC_CLOTHES_GLOVES,			// �ƴ�(�尩) = 2
	_XGI_SC_CLOTHES_SHOES,			// �Ź� = 3
	_XGI_SC_CLOTHES_CAP,			// ���� = 4
	_XGI_SC_CLOTHES_CLOAK,			// �䰩 = 5
	_XGI_SC_CLOTHES_ARMOR,			// �Ű� = 6
	_XGI_SC_CLOTHES_BACKPACK,		// �賶 = 7
	_XGI_SC_CLOTHES_MASK			// ġ�� = 8
};


// ============================================================
// ��� ������ �ߺз�
//
// 0 : �������
// 1 : ������
// 2 : ��Ȱ���
// 3 : ����
// 4 : �Ϲ� å
// 5 : �η縶��
// 6 : �������2(�ߺ� ��ų�� ���� �� ����)
//
// ============================================================
typedef enum _XGI_SecondCategory_Book
{
	_XGI_SC_BOOK_SKILL = 0,		// ������� = 0		
	_XGI_SC_BOOK_STUDY,			// ������	= 1
	_XGI_SC_BOOK_ABILITY,		// ��Ȱ��� = 2
	_XGI_SC_BOOK_SCROLL,		// �η縶��	= 3
	_XGI_SC_BOOK_STUFF,			// �Ϲ� å	= 4
	_XGI_SC_BOOK_PAPER,			// ����		= 5
	_XGI_SC_BOOK_SKILL2,		// ������� = 6
};

typedef enum _XGI_ThirdCategory_Book_Help
{
	_XGI_TC_BOOK_APPOINTMENT = 0,	// ���������� = 0
	_XGI_TC_BOOK_GROUP,				// ���Ĺ����� = 1
	_XGI_TC_BOOK_DUTIES,			// ��å������ = 2
	_XGI_TC_BOOK_SERIES,			// �迭������ = 3
};
// =============================================================
// ȸ�� ������ �ߺз�
//
// 0 : ���� ȸ����
// 1 : �ܻ� ȸ����
// 2 : �Ƿ� ȸ����
//
// =============================================================
typedef enum _XGI_SecondCategory_Potion
{
	_XGI_SC_POTION_HERT_IN = 0,		// 0. ���� ġ����
	_XGI_SC_POTION_HERT_OUT,		// 1. �ܻ� ġ����
	_XGI_SC_POTION_FATIGUE,			// 2. �Ƿ� ȸ����
	_XGI_SC_POTION_NOTFIXED,		// 3. ����
	_XGI_SC_POTION_FOOD,			// 4. ����
	_XGI_SC_POTION_EVENT,			// 5. �ž�
	_XGI_SC_POTION_HWATA,			// 6. ȭŸ�Ŵ�
	_XGI_SC_POTION_MASK,			// 7. ����
	_XGI_SC_POTION_ANTI,			// 8. �ص��� 
	_XGI_SC_POTION_ADDEXP,			// 9. ����ġ/���õ� ����
	_XGI_SC_POTION_RESETPOINT,		// 10. ����/���� �ʱ�ȭ
	_XGI_SC_POTION_NICKNAME,		// 11. ��ȣ ����
	_XGI_SC_POTION_INCHANTRATE,		// 12. ���� ������ ���� ����
	_XGI_SC_POTION_SHOUT,			// 13. ��ġ��
	_XGI_SC_POTION_RECALL,			// 14. ��ȯ��/ȣ��� //Author : ����� �߰�
	_XGI_SC_POTION_FIREWORK,		// 15. ���׷�
	_XGI_SC_POTION_NOTMERGE,		// 16. merge ���� �ʴ� ���Ƿ�
	_XGI_SC_POTION_COOLDOWN,		// 17. ��ٿ� �ð� ���� ����ϴ� ���Ƿ�
	_XGI_SC_POTION_EVENT2			// 18. �ž� ������ ���ڸ��ٰ��ؼ� �߰� //Author : �����
};


// =============================================================
//	��ô ������ �ߺз�
// 
// 0 : ȭ��
// 1 : ���
// 2 : ź
//
// =============================================================
typedef enum _XGI_SecondCategory_Throw
{
	_XGI_SC_THROW_ARROW = 0,		// ȭ�� = 0
	_XGI_SC_THROW_THROW,			// ��� = 1
	_XGI_SC_THROW_BULLET			// ź = 2
};


// =============================================================
// �ߵ� ������ �ߺз�
// 
// 0 : ǳ
// 1 : �� 
// 2 : ��
// 3 : ��
// 4 : ��
// 5 : ȭ
// 6 : ��
// 7 : ��
// 8 : ��
// 9 : û
// 10 : ��
// 11 : ��
// 12 : ��
// 13 : ��
// 14 : ��
//
// ===============================================================
typedef enum _XGI_SecondCategory_Poison
{
	_XGI_SC_POISON_PUNG = 0,		// ǳ = 0
	_XGI_SC_POISON_NUE,				// �� = 1
	_XGI_SC_POISON_DOCK,			// �� = 2
	_XGI_SC_POISON_BING,			// �� = 3
	_XGI_SC_POISON_TO,				// �� = 4
	_XGI_SC_POISON_HWA,				// ȭ = 5
	_XGI_SC_POISON_SU,				// �� = 6
	_XGI_SC_POISON_MA,				// �� = 7
	_XGI_SC_POISON_JUNG,			// �� = 8
	_XGI_SC_POISON_CHUNG,			// û = 9
	_XGI_SC_POISON_MOCK,			// �� = 10
	_XGI_SC_POISON_KEUM,			// �� = 11
	_XGI_SC_POISON_BO,				// �� = 12
	_XGI_SC_POISON_PAE,				// �� = 13
	_XGI_SC_POISON_MYUNG			// �� = 14
};

// ===============================================================
// ���� ������ �ߺз�
typedef enum _XGI_SecondCategory_Elixir
{
	_XGI_SC_ELIXIR_SUPPORT = 0,		// �������� = 0
	_XGI_SC_ELIXIR_STRENGTH,		// �ٷ¿��� = 1
	_XGI_SC_ELIXIR_INNER,			// ���⿵�� = 2
	_XGI_SC_ELIXIR_HEART,			// �������� = 3
	_XGI_SC_ELIXIR_HEALTH,			// �ǰ����� = 4
	_XGI_SC_ELIXIR_DEX				// ��ø���� = 5	
};

// ===============================================================
// �ص� ������ �ߺз�
//
// 0 : ǳ
// 1 : �� 
// 2 : ��
// 3 : ��
// 4 : ��
// 5 : ȭ
// 6 : ��
// 7 : ��
// 8 : ��
// 9 : û
// 10 : ��
// 11 : ��
// 12 : ��
// 13 : ��
// 14 : ��
//
// ===============================================================
typedef enum _XGI_SecondCategory_Counteract
{
	_XGI_SC_COUNTERACT_PUNG = 0,	// ǳ = 0
	_XGI_SC_COUNTERACT_NUE,			// �� = 1
	_XGI_SC_COUNTERACT_DOCK,		// �� = 2
	_XGI_SC_COUNTERACT_BING,		// �� = 3
	_XGI_SC_COUNTERACT_TO,			// �� = 4
	_XGI_SC_COUNTERACT_HWA,			// ȭ = 5
	_XGI_SC_COUNTERACT_SU,			// �� = 6
	_XGI_SC_COUNTERACT_MA,			// �� = 7
	_XGI_SC_COUNTERACT_JUNG,		// �� = 8
	_XGI_SC_COUNTERACT_CHUNG,		// û = 9
	_XGI_SC_COUNTERACT_MOCK,		// �� = 10
	_XGI_SC_COUNTERACT_KEUM,		// �� = 11
	_XGI_SC_COUNTERACT_BO,			// �� = 12
	_XGI_SC_COUNTERACT_PAE,			// �� = 13
	_XGI_SC_COUNTERACT_MYUNG		// �� = 14
};


// ================================================================
// �ڿ� ������ �ߺз�
//
// 0 : ����
// 1 : ä��
// 2 : ����
// 3 : ǥ�� �̿��
//
// ================================================================
typedef enum _XGI_SecondCategory_Resource
{
	_XGI_SC_RESOURCE_PRODUCTION = 0,	// ���� = 0
	_XGI_SC_RESOURCE_COLLECTION,		// ä�� = 1
	_XGI_SC_RESOURCE_PRESENT,			// ���� = 2
	_XGI_SC_RESOURCE_MOVE				// ǥ�� �̿�� = 3
};


// ================================================================
// ����Ʈ ������ �ߺз�
//
// ================================================================
typedef enum _XGI_SecondCategory_Quest
{
};



// ================================================================
// �Ǽ��縮 �ߺз�
//
// 0 : ����
// 1 : ����
// 2 : �賶
// 3 : �����
// 4 : �Ͱ���
//
// ================================================================
typedef enum _XGI_SecondCategory_Accessory
{
	_XGI_SC_ACCESSORY_RING = 0,			// ���� = 0
	_XGI_SC_ACCESSORY_CHARM,			// ���� = 1
	_XGI_SC_ACCESSORY_BACKPACK,			// �賶 = 2
	_XGI_SC_ACCESSORY_NECKLACE,			// ����� = 3
	_XGI_SC_ACCESSORY_EARRING,			// �Ͱ��� = 4

	_XGI_SC_ACCESSORY_STRENGTH = 11,	// �ٷ����� = 11
	_XGI_SC_ACCESSORY_CONSTITUTION,		// �ǰ����� = 12
	_XGI_SC_ACCESSORY_ZEN,				// �������� = 13
	_XGI_SC_ACCESSORY_INT,				// �������� = 14
	_XGI_SC_ACCESSORY_DEX,				// �������� = 15

	_XGI_SC_ACCESSORY_ARMOR_MULTI = 50,	// ��, �ϰ� �ߺ�����
	_XGI_SC_ACCESSORY_ARMOR				// ��, �ϰ� �ߺ��Ұ�
};

// ================================================================
// ��Ȱ �ߺз�
//
// 0 : ��ĵ���
// 1 : ��ô������
//
// ================================================================
typedef enum _XGI_SecondCategory_Life
{
	_XGI_SC_LIFE_SOCKET = 0,
	_XGI_SC_LIFE_CONSUMING,
	_XGI_SC_LIFE_INCHANTSTUFF,
	_XGI_SC_LIFE_SOCKETSSTUFF,
	_XGI_SC_LIFE_SMLETITEM_SCALES , //Author : �����
	_XGI_SC_LIFE_SMLETITEM_TEAR,//Author : �����
	_XGI_SC_LIFE_MIX_SOCKET_NORMAL,		// ���վ����� : ����(�Ϲ�)
	_XGI_SC_LIFE_MIX_SOCKET_PERIOD,		// ���վ����� : ����(�Ⱓ��)
	_XGI_SC_LIFE_MIX_PENALTY			// ���վ����� : �г�Ƽ ����
};

// ================================================================
// ��Ȱ ���� ������
// �Ǳ� ������
// ������ ������
// �� 
//
// �ߺз� ����
//
// ================================================================


///////////////////////////////////////////////////
// Item table structure


// ------------------------------------------------
// ������ ����

#define ITEM_NAME_LENGTH		(33)

typedef struct XGI_Item_Base
{
	char			cType ;				// ��з�
	char			cSecondType ;		// �ߺз�
	short			sID ;				// ���̵�
	short			sModelIndex ;		// Ŭ���̾�Ʈ �� �ε���.
	
	short			sIcon_No  ;         // ������ �ѹ�.
	unsigned short	usItem_Rank;        // ������ ���.
#ifdef _XTS_ITEM_V20
	TCHAR*			cItemInfo;			// ������ �������̼� �ε���
#else
	TCHAR			cItemInfo[57] ;     // ������ �������̼�.
#endif


	char			cApplyClan;
	short			sClanPoint1;		// ����
	short			sClanPoint2;
	
	unsigned int	uiPrice ;			// ����

#ifdef _XTS_ITEM_V20
	TCHAR*		 	cName;				// ������ �̸��� ����.
#else
	TCHAR			cName[ITEM_NAME_LENGTH] ;			// ������ �̸��� ����.
#endif

	unsigned char	ucGrade;			// ���
	unsigned char	ucQuality;			// ���
	unsigned char	ucQuality2;			// new

	char	cDump;				//������ ����
	char	cUserTrade;			//������ ����� ������ ����
	char	cNPCTrade;			//NPC�Ǹ�, ���ı⿩ ����

	unsigned char	ucFame;		// �Ǹ� 
	
#ifdef _XTS_ITEM_V21
	char			cCashCheck;			// 0:�Ϲݾ�����  1:�Ϲ�ĳ��������  2:ĳ���Ⱓ��������  3:�ϹݱⰣ��������
#else
	unsigned short	usTempField2;
#endif
	
#ifdef _XDEF_PERIODITEM_20061027
	unsigned short	usTimeValue;
#else
	short	sTempField3;
#endif
	short	sTempField4;
	short	sTempField5;

#ifdef _XTS_ITEM_V20
	XGI_Item_Base()
	{
		cItemInfo = NULL;
		cName = NULL;
	}
#else
	XGI_Item_Base()
	{
		memset( cItemInfo, 0, sizeof(TCHAR) * 57 );
		memset( cName, 0, sizeof(TCHAR) * ITEM_NAME_LENGTH );
	}
#endif
}_XGI_Item_Base;


// ------------------------------------------------
// ���� ���� 
typedef enum _XGI_WeaponProp_material
{
	_XGI_WP_MATERIAL_FIST = 0,
	_XGI_WP_MATERIAL_WOOD,
	_XGI_WP_MATERIAL_METAL	
};

typedef enum _XGI_WeaponProp_soundmaterial
{
	_XGI_WP_SOUNDMATERIAL_FIST = 0,
	_XGI_WP_SOUNDMATERIAL_LIGHTWOOD,
	_XGI_WP_SOUNDMATERIAL_HEAVYWOOD,
	_XGI_WP_SOUNDMATERIAL_LIGHTMETAL,	
	_XGI_WP_SOUNDMATERIAL_MEDIUMMETAL,
	_XGI_WP_SOUNDMATERIAL_HEAYVYMETAL
};

typedef enum _XGI_WeaponProp_weightclass
{
	_XGI_WP_WEIGHTCLASS_LIGHT = 0,
	_XGI_WP_WEIGHTCLASS_MIDIUM,
	_XGI_WP_WEIGHTCLASS_HEAVY	
};

typedef struct XGI_WeaponItem_Property : public _XGI_Item_Base
{
	// ��з� : ���� 0
	// 
	// �Ӽ� :
#ifdef _XTS_ITEM_V20
	TCHAR*			cNick;
	unsigned short	usAddto;
#else
	char			cNick[ITEM_NAME_LENGTH];
#endif


	char			cThirdType ;
	short			sMinPower ;			// �ּ� ���ݷ�
	short			sMaxPower ;			// �ִ� ���ݷ�
	unsigned short	usDurability ;		// �ִ� ����
	char			cBalance ;			// ��ȭ ; �ִ�, �ּ� ���ݷ��� ����� ����.
	char			cAttackRate;
#ifdef _XTS_ITEM_V21
	short			sCriticalRate;
#else
	char			cCriticalRate;
#endif
	char			cSpeed;
	char			cStrength ;			// ������ ����
	char			cRange ;			// ���� �Ÿ�
	char			cNature ;			// Ưȭ �Ӽ�
	short			sReqLevel ;			// �䱸 ���� ����
	short			sReqSim ;			// �䱸 ��ø
	short			sReqJung ;			// �䱸 �ٷ�
	char			cRank ;				// ���� ��� : �������� ���õ� �� �ִ� �ִ� ���
	short			sHidden_ID ;		// �����⺸ ID
	char			cHidden_Value ;		// �����⺸�� �� Ȯ��
	unsigned char	ucMaxSocket;
	char			cRepairProb ;		// ���� Ȯ�� : ���������� ���, ����ġ������ ������.
	//short			sModel_Type;
	short			sG_Group;			//Gamble��
	short			sG_Case;			//Gamble��
	//char			cQuality ;			// ���� ���� : ���忡 ���ȴ�. Ŭ���̾�Ʈ��.

	D3DXVECTOR3		vecItemPosition[4];	
	
	unsigned short	usAdd_SEID[5];
	unsigned short	usAdd_Value[5];
	unsigned char	ucAdd_Prob[5];
	unsigned char	ucAP_Type[5];
	unsigned short	usAP_Per[5];

	//SPECIAL	modified by changkhan	2004.10.13
	unsigned short	usSpecial_ID_1;
	unsigned short	usSpecial_Time_1;
	unsigned char	ucSpecial_Value_1;
	unsigned char	ucSApply_Type_1;
	unsigned short	usSpecial_Prob_1;

	unsigned short	usSpecial_ID_2;
	unsigned short	usSpecial_Time_2;
	unsigned char	ucSpecial_Value_2;
	unsigned char	ucSApply_Type_2;
	unsigned short	usSpecial_Prob_2;

	unsigned short	usSpecial_ID_3;
	unsigned short	usSpecial_Time_3;
	unsigned char	ucSpecial_Value_3;
	unsigned char	ucSApply_Type_3;
	unsigned short	usSpecial_Prob_3;

	unsigned short	usSpecial_ID_4;
	unsigned short	usSpecial_Time_4;
	unsigned char	ucSpecial_Value_4;
	unsigned char	ucSApply_Type_4;
	unsigned short	usSpecial_Prob_4;

	short			sGroup; //Author : ����� //breif : 08/01/29 sAccord �� ����.. ��Ʈ���������� ���
	
#ifdef _XDEF_ITEM_V24
	unsigned char	ucSocketCheck;
#endif

#ifdef _XTS_ITEM_V20
	XGI_WeaponItem_Property()
	{
		cNick = NULL;
		usAddto = 0;
	}
#else
	XGI_WeaponItem_Property()
	{
		memset( cNick, 0, sizeof(char) * ITEM_NAME_LENGTH );
	}
#endif

}_XGI_WeaponItem_Property;


// ------------------------------------------------
// �ǻ� ����
typedef struct XGI_ClothesItem_Property : public _XGI_Item_Base
{
	// ���Ĵ� �ǻ��� ī�װ� 3���� �ν��Ѵ�.
#ifdef _XTS_ITEM_V20
	TCHAR*			cNick;
	unsigned short	usAddto;
#else
	char			cNick[ITEM_NAME_LENGTH];
#endif

	short			sG_Group;			// Gamble�� 
	short			sG_Case;			// Gamble�� 

	char			cPrecedence ;		// �켱 ����: �ǻ�� ���� �켱 ����
	char			cClan ;				// �䱸 - ���� Ư�� : 
#ifdef _XDEF_ITEM_V25
	unsigned char	ucCharacGrade;		// 2�� ���� ���� �ǻ� ǥ��
#endif
	char			cSexType ;			// �䱸 - ����
	//char			cAccord ;			// ��ȭ : �ǻ��� ��ȭ
	unsigned char	ucGroup;			//Author : ����� //breif : 08/01/29 cAccord �� ����.. ��Ʈ���������� ���

	// �Ӽ�
	char			cClass ;			// ����(���) : �ǻ��� ���� Ȥ�� ���
	char			cRepairProb ;		// ���� Ȯ��
	//char			cQuality ;			// ���� ����
	char			cRank ;				// ���� ���:���õ� �� �ִ� �ִ� ���

	float			fCenter_X ;			// �ǻ��� �߽��� : Ŭ���̾�Ʈ ����Ÿ.
	float			fCenter_Y ;			//
	float			fCenter_Z ;

	short			sHidden_ID ;		// �����⺸ ID
	char			cHidden_Value ;		// �����⺸�� �� Ȯ��

	// ���
	short			sDefence ;			// ����
	short			sStamina ;			// ����
	char			cWarm ;				// ���� ȿ��

	char			cCool_Resistance ;	// ��
	char			cHot_Resistance ;	// ��
	char			cSpirit_Resistance;	// ȥ
	char			cBody_Resistance ;	// ��
	
	unsigned short	usDurability ;		// �ִ� ����
	char			cStrength ;			// �ǻ��� ����

	unsigned short	usAdd_SEID[5];
	unsigned short	usAdd_Value[5];
	unsigned char	ucAdd_Prob[5];
	unsigned char	ucAP_Type[5];
	unsigned short	usAP_Per[5];

	//
	char			cPocket;
	
	// �䱸
	short			sReqLevel ;		// �䱸 ���� ����
	short			sReqSim ;		// �䱸 �ɱ�
	short			sReqJung ;		// ����

	//SPECIAL	modified by changkhan	2004.10.13
	unsigned short	usSpecial_ID_1;
	unsigned short	usSpecial_Time_1;
	unsigned char	ucSpecial_Value_1;
	unsigned char	ucSApply_Type_1;
	unsigned short	usSpecial_Prob_1;

	unsigned short	usSpecial_ID_2;
	unsigned short	usSpecial_Time_2;
	unsigned char	ucSpecial_Value_2;
	unsigned char	ucSApply_Type_2;
	unsigned short	usSpecial_Prob_2;

	unsigned short	usSpecial_ID_3;
	unsigned short	usSpecial_Time_3;
	unsigned char	ucSpecial_Value_3;
	unsigned char	ucSApply_Type_3;
	unsigned short	usSpecial_Prob_3;

	unsigned short	usSpecial_ID_4;
	unsigned short	usSpecial_Time_4;
	unsigned char	ucSpecial_Value_4;
	unsigned char	ucSApply_Type_4;
	unsigned short	usSpecial_Prob_4;

	//socket
	unsigned char	ucMaxSocket;
	char			cAttackRate;
	char			cEvasionRate;

#ifdef _XDEF_ITEM_V24
	unsigned char	ucSocketCheck;
#endif

#ifdef _XTS_ITEM_V20
	XGI_ClothesItem_Property()
	{
		usAddto = 0;
		cNick = NULL;
	}
#else
	XGI_ClothesItem_Property()
	{
		memset( cNick, 0, sizeof(char) * ITEM_NAME_LENGTH );
	}
#endif

}_XGI_ClothesItem_Property;


// ------------------------------------------------
// ��� ����
typedef struct XGI_BookItem_Property : public _XGI_Item_Base
{
	char			cThirdType;			
	unsigned short	usSkillID;
	unsigned char	ucValue;
	unsigned char	ucSkillGroup;
	unsigned char	ucSkillClass;
	char			cSkillStep;

	unsigned char	ucSkillLevel;
	unsigned short	usAbilityID;
	unsigned short	usHiddenID;
	unsigned short	usQuestID;

	unsigned char	ucClan;
#ifdef _XDEF_ITEM_V25
	unsigned char	ucCharacGrade;		// 2�� �����ڸ� ���� �� �ֵ���
#endif
	unsigned char	ucClass;

	unsigned short	usReqSkill1;
	char			cReqSkill1Step;
	unsigned short	usReqSkill2;
	char			cReqSkill2Step;
	unsigned short	usReqSkill3;
	char			cReqSkill3Step;
	unsigned short	usReqAbility;
	char			cReqAbilityStep;

	char			cProb;
	unsigned short	usDelay;
	unsigned char	ucDelete;
	unsigned int	uiApplyTime;

	unsigned short	usCoolDown;
	unsigned char	ucApplyClan;

	unsigned short	usReqLevel;
	unsigned short	usReq_STR;
	unsigned short	usReq_Force;
	unsigned short	usReq_Intell;
	unsigned short	usReq_Const;
	unsigned short	usReq_Dex;

	unsigned short	usLife;
	unsigned short	usForce;
	unsigned short	usConcent;
	
}_XGI_BookItem_Property;


// -------------------------------------------------
// ȸ�� ����
typedef struct XGI_PotionItem_Property : public _XGI_Item_Base
{
	char			cClan;					// �䱸 : ���� (0�̸� �پ���)
	
	short			sApplyValue;			// ���� Ÿ�Կ� ���� ��ġ �Ǵ� ����
	char			cApplyType;				// 0 : ��ġ  1 : ����
	
	// 2004.06.16->oneway48 insert 
	short           sEffectId    ;
	short           sInCEffectId ;   
	short           sVisualEfId  ;
	// insert end
	
	short			sCooldown;				// ��� �� ������
	
	// 2004.06.16->oneway48 insert 
	short           sReqLevel    ;
	short           sSimgi ;        
	short           sJungGi;
	// insert end
}_XGI_PotionItem_Property;


// -------------------------------------------------
// ��ô�� 
typedef struct XGI_ConsumingItem_Property : public _XGI_Item_Base
{
#ifdef _XTS_ITEM_V20
	TCHAR*			cNick;
#else
	char			cNick[ITEM_NAME_LENGTH];
#endif
	
	char			cThird_Type;
	char			cClan;
	char			cSex_Type;
	char			cClass;

	unsigned short	usSEID;
	unsigned short	usTime;	

	char			cSpeed;
	char			cStrength;
	char			cRange;

	// �߰� 
	unsigned short	usAdd_SEID_1 ;			// ȿ�� ��ȣ
	unsigned short	usAdd_Value_1;
	unsigned char	ucAdd_Prob_1;
	unsigned char	ucAP_Type_1;
	unsigned short	usAP_Per_1;

	unsigned short	usAdd_SEID_2 ;			// ȿ�� ��ȣ
	unsigned short	usAdd_Value_2;
	unsigned char	ucAdd_Prob_2;
	unsigned char	ucAP_Type_2;
	unsigned short	usAP_Per_2;

	unsigned short	usAdd_SEID_3 ;			// ȿ�� ��ȣ
	unsigned short	usAdd_Value_3;
	unsigned char	ucAdd_Prob_3;
	unsigned char	ucAP_Type_3;
	unsigned short	usAP_Per_3;

	unsigned short	usAdd_SEID_4 ;			// ȿ�� ��ȣ
	unsigned short	usAdd_Value_4;
	unsigned char	ucAdd_Prob_4;
	unsigned char	ucAP_Type_4;
	unsigned short	usAP_Per_4;

	unsigned short	usAdd_SEID_5 ;			// ȿ�� ��ȣ
	unsigned short	usAdd_Value_5;
	unsigned char	ucAdd_Prob_5;
	unsigned char	ucAP_Type_5;
	unsigned short	usAP_Per_5;

	short			sReq_Level ;	// �䱸 : ���� ����
	short			sReq_Jungki ;		// �䱸 : ���� ��ġ
	short			sReq_Simki ;		// �䱸 : �ɱ� ��ġ 

#ifdef _XTS_ITEM_V20
	XGI_ConsumingItem_Property()
	{
		cNick = 0;
	}
#else
	XGI_ConsumingItem_Property()
	{
		memset( cNick, 0, sizeof(char) * ITEM_NAME_LENGTH );
	}
#endif
}_XGI_ConsumingItem_Property;


// --------------------------------------------------
// �ߵ���
typedef struct XGI_PoisonItem_Property : public _XGI_Item_Base
{
	char			cTarget;				// ���
	FLOAT			fAttackRange;			// ���� ����

	short			sDamageMax;				// �ִ� ���ݷ�
	short			sDamageMin;				// �ּ� ���ݷ�

	short			sEffectID;				// ȿ�� ��ȣ
	short			sEffectTime;			// ȿ�� ���� �ð� : ��
	short			sEffectValue;			// ȿ�� ��ġ
	short			sEffectProb;			// ȿ�� �ߵ� Ȯ��

	FLOAT			fEffectRange;			// Ÿ�� ����Ʈ �߽����� ȿ���� ����Ǵ� ����

	short			sReqLevel;				// �䱸 ����
	short			sReqYoung;				// �䱸 ����
	short			sReqJung;				// �䱸 ����
}_XGI_PoisonItem_Property;


// --------------------------------------------------
// �ص���
typedef struct XGI_CounteractItem_Property : public _XGI_Item_Base
{
	char			cTarget;				// ���
	FLOAT			fAttackRange;			// ���� ����

	short			sDamageMax;				// �ִ� ���ݷ�
	short			sDamageMin;				// �ּ� ���ݷ�

	short			sEffectID;				// ȿ�� ��ȣ
	short			sEffectTime;			// ȿ�� ���� �ð� : ��
	short			sEffectValue;			// ȿ�� ��ġ
	short			sEffectProb;			// ȿ�� �ߵ� Ȯ��

	FLOAT			fEffectRange;			// Ÿ�� ����Ʈ�� �߽����� ȿ���� ����Ǵ� ����

	short			sReqLevel;				// �䱸 ����
	short			sReqYoung;				// �䱸 ����
	short			sReqJung;				// �䱸 ����
}_XGI_CounteractItem_Property;


// -------------------------------------------------
// �ڿ���
typedef struct XGI_ResourceItem_Property : public _XGI_Item_Base
{
}_XGI_ResourceItem_Property;


// -------------------------------------------------
// ����Ʈ
typedef struct XGI_QuestItem_Property : public _XGI_Item_Base
{
	short   sQuest_Id; // 2004.06.16->oneway48 insert
	unsigned short	usSEID;
	unsigned short	usValue;
	unsigned short	usDelay;
	short			sEffectID;
}_XGI_QuestItem_Property;


// -------------------------------------------------
// �Ǽ��縮
typedef struct XGI_AccessoryItem_Property : public _XGI_Item_Base
{
	short			sG_Group;			//Gamble��
	short			sG_Case;			//Gamble��

	char			cPack ;				// �賶�� ��쿡��, ���� �뷮
	char			cClan;
	char			cRank ;
	unsigned short	usDurability ;		// �ִ� ����

	unsigned short	usAdd_SEID[5];
	unsigned short	usAdd_Value[5];
	unsigned char	ucAdd_Prob[5];
	unsigned char	ucAP_Type[5];
	unsigned short	usAP_Per[5];

	// �䱸
	short			sReqLevel ;		// �䱸 ���� ����

#ifdef _XTS_ITEM_V22
	short			sReqDex ;		// �䱸 �ɱ�
	short			sReqStr ;		// ����
	short			sReqHealth;		// �ʿ� �ǰ�
	short			sReqJingi;		// �ʿ� ����
	short			sReqInt;		// �ʿ� ����
#else
	short			sReqSim;		// �䱸 �ɱ�
	short			sReqJung;		// �䱸 ����
#endif

	//SPECIAL	modified by changkhan	2004.10.13
	unsigned short	usSpecial_ID_1;
	unsigned short	usSpecial_Time_1;
	unsigned char	ucSpecial_Value_1;
	unsigned char	ucSApply_Type_1;
	unsigned short	usSpecial_Prob_1;

	unsigned short	usSpecial_ID_2;
	unsigned short	usSpecial_Time_2;
	unsigned char	ucSpecial_Value_2;
	unsigned char	ucSApply_Type_2;
	unsigned short	usSpecial_Prob_2;

	unsigned short	usSpecial_ID_3;
	unsigned short	usSpecial_Time_3;
	unsigned char	ucSpecial_Value_3;
	unsigned char	ucSApply_Type_3;
	unsigned short	usSpecial_Prob_3;

	unsigned short	usSpecial_ID_4;
	unsigned short	usSpecial_Time_4;
	unsigned char	ucSpecial_Value_4;
	unsigned char	ucSApply_Type_4;
	unsigned short	usSpecial_Prob_4;

	//socket
	unsigned char	ucMaxSocket;
	short			sGroup; //Author : ����� //breif : 08/01/29 sAccord �� ����.. ��Ʈ���������� ���

}_XGI_AccessoryItem_Property;


// ----------------------------------------------
// ��Ȱ ����
typedef struct XGI_LifeItem_Property : public _XGI_Item_Base
{
	char			cThirdType;
	char			cClanType;
	unsigned short	usDurability ;		// �ִ� ����
	unsigned char	ucCount;
	char			cBalance ;			// ��ȭ ; �ִ�, �ּ� ���ݷ��� ����� ����.
	char			cProb;
	char			cLuck;
	char			cStrength ;			// ������ ����
	char			cRank ;				// ���� ��� : �������� ���õ� �� �ִ� �ִ� ���
	unsigned short	usAbility_ID;
	unsigned char	ucMax_Merge;
	char			cRepair_Prob;
	short			sModel_Type;
}_XGI_LifeItem_Property;


// ----------------------------------------------
// �Ǳ�
typedef struct XGI_MusicItem_Property : public _XGI_Item_Base
{

}_XGI_MusicItem_Property;


// ----------------------------------------------
// ������
typedef struct XGI_PassItem_Property : public _XGI_Item_Base
{

}_XGI_PassItem_Property;



// -----------------------------------------------
// ����
typedef struct XGI_SocketItem_Property : public _XGI_Item_Base
{

#ifdef _XDEF_ITEM_V23 /*!< @author ����� @breif ������ ���̺� Thirdtype �߰�  */
	unsigned char	ucThirdType;
#endif

	short	sSkill_Effect_ID1;

#ifdef _XTS_ITEM_V20
	TCHAR*	cSkill_Info1;
#else
	char    cSkill_Info1[57] ;     // ������ �������̼�.
#endif
	char	cType1;
	short	sValue1;
	char	cProb1;
	char	cAP_Type1;
	short	sAP_Per1;
	short	sAP_Count1;

	short	sSkill_Effect_ID2;
#ifdef _XTS_ITEM_V20
	TCHAR*	cSkill_Info2;
#else
	char    cSkill_Info2[57] ;     // ������ �������̼�.
#endif
	char	cType2;
	short	sValue2;
	char	cProb2;
	char	cAP_Type2;
	short	sAP_Per2;
	short	sAP_Count2;
	
	short	sSkill_Effect_ID3;
#ifdef _XTS_ITEM_V20
	TCHAR*	cSkill_Info3;
#else
	char    cSkill_Info3[57] ;     // ������ �������̼�.
#endif
	char	cType3;
	short	sValue3;
	char	cProb3;
	char	cAP_Type3;
	short	sAP_Per3;
	short	sAP_Count3;

	short	sSkill_Effect_ID4;
#ifdef _XTS_ITEM_V20
	TCHAR*	cSkill_Info4;
#else
	char    cSkill_Info4[57] ;     // ������ �������̼�.
#endif
	char	cType4;
	short	sValue4;
	char	cProb4;
	char	cAP_Type4;
	short	sAP_Per4;
	short	sAP_Count4;

#ifdef _XTS_ITEM_V20
	XGI_SocketItem_Property()
	{
		cSkill_Info1 = NULL;
		cSkill_Info2 = NULL;
		cSkill_Info3 = NULL;
		cSkill_Info4 = NULL;
	}
#endif
}_XGI_SocketItem_Property;

// -----------------------------------------------
// ����
typedef struct XGI_ElixirItem_Property : public _XGI_Item_Base
{
	char			third_type;

	unsigned char	ucStep;
	unsigned char	ucReq_Level;
	unsigned short	usValue_Min;
	unsigned short	usValue_Max;
	unsigned short	usCoolDownDelay;
	unsigned short	usSuccess_Rate;

	unsigned short	usBody_Type[8];
	unsigned char	ucAddiction;
	unsigned char	ucVisualEffect;
	unsigned short	usTemp[4];	
}_XGI_ElixirItem_Property;

// -----------------------------------------------
// �пձ�
typedef struct XGI_BoxItem_Property: public _XGI_Item_Base
{
	char			third_type;

	unsigned short	usTemp0;

	unsigned char	ucCase[10];
	short			sCaseProb[10];	

	unsigned char	ucVisualEffect;
	unsigned short	usTemp[4];	
}_XGI_BoxItem_Property;

// -----------------------------------------------
// �пձ� ����
typedef struct XGI_BoxKeyItem_Property: public _XGI_Item_Base
{
	unsigned char	ucProbWeapon;
	unsigned char	ucProbClothes;
	unsigned char	ucProbEtc;

	unsigned short	usTemp[4];	
}_XGI_BoxKeyItem_Property;

// -----------------------------------------------

// NPC �Ǹ� ����
typedef struct XGI_ITEMTRADE_ITEM
{
	int			category;
	int			uniqueID;
}_XGI_ITEMTRADE_ITEM;


typedef struct XGI_ITEMTRADE_ITEMPACKAGE
{
	int						packageID;
	TCHAR					packageName[32];
	int						itemCount;
	_XGI_ITEMTRADE_ITEM		itemUnit[100];
}_XGI_ITEMTRADE_ITEMPACKAGE;


// ��ȣ ����
struct _XNicNameInfo {
	unsigned short	usIndex ;
	
#ifdef _XDEF_NEWSTRINGMANAGER		
	LPTSTR			cNickName;	
	LPTSTR			cNickInfo;
	LPTSTR			cNickInfoDetail;
#else
	char			cNickName[30] ;	
	char			cNickInfo[128] ;
	char			cNickInfoDetail[128];
#endif

	char			cNickIconId ;
	char			cApplyType;
	
	char			cType1;
	char			cType2;
	char			cType3;

	char			cNickGroup;
	char			cNickGrade;
	char			cClanType;
	char			cSexType;
	char			cDuplicate;
	char			cPickupCnt;

	struct  
	{
		unsigned short		usEffectIndex ;
		unsigned short		usEffectValue ;
		char				cEffectProbability ;
	} _Effect[5] ;
	
	short			sIconNumber;
};

// ������ �ͽ� ���̺� ����
struct _XItemMixTable {
	unsigned int ucIndex[5];
};

// ������ ���� ���̺�
struct _XItemInchantTable 
{	
	int				cType;
	int				sID;
	int				sID_2;
	
	int				ucCount;
	unsigned int	uiPrice;
	int				effectID;
	int				iatteckplus;
	float			fattecktimes;
	int				iforceplus;
	float			fforcetimes;

	_XItemInchantTable() 
		: cType(0), sID(0), sID_2(0), ucCount(0), uiPrice(0), effectID(0), iatteckplus(0), fattecktimes(0), iforceplus(0), fforcetimes(0)
	{
	}
};
#endif // _XGAME_ITEM_DEFINE_

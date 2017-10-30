// XGameItem.h: interface for the _XGameItem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XGAMEITEM_H_
#define _XGAMEITEM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XGameItemDefine.h"
#include "XMeshContainer.h"
#include "XAnimMeshContainer.h"
#include "XBipedController.h"
#include "XObjectManager.h"

extern _XGI_WeaponItem_Property*		g_WeaponItemProperty;
extern _XGI_ClothesItem_Property*		g_ClothesItemProperty;
extern _XGI_BookItem_Property*			g_BookItemProperty;
extern _XGI_PotionItem_Property*		g_PotionItemProperty;
extern _XGI_ConsumingItem_Property*		g_ConsumingItemProperty;
extern _XGI_PoisonItem_Property*		g_PoisonItemProperty;
extern _XGI_CounteractItem_Property*	g_CounteractItemProperty;
extern _XGI_ResourceItem_Property*		g_ResourceItemProperty;
extern _XGI_QuestItem_Property*			g_QuestItemProperty;
extern _XGI_AccessoryItem_Property*		g_AccessoryItemProperty;
extern _XGI_LifeItem_Property*			g_LifeItemProperty;
extern _XGI_MusicItem_Property*			g_MusicItemProperty;
extern _XGI_PassItem_Property*			g_PassItemProperty;
extern _XGI_SocketItem_Property*		g_SocketItemProperty;
extern _XGI_WeaponItem_Property*		g_WeaponItemProperty2;
extern _XGI_ClothesItem_Property*		g_ClothesItemProperty2;
extern _XGI_ElixirItem_Property*		g_ElixirItemProperty;
extern _XGI_BoxItem_Property*			g_BoxItemProperty;
extern _XGI_BoxKeyItem_Property*		g_BoxKeyItemProperty;
extern _XGI_WeaponItem_Property*		g_WeaponItemProperty3;
extern _XGI_ClothesItem_Property*		g_ClothesItemProperty3;

extern _XGI_ITEMTRADE_ITEMPACKAGE		g_ItemTradePackage[150];

extern _XNicNameInfo*					g_NickNameInfoTable; 
extern short							g_MaxNickNameCount;

extern short							g_MaxItemCount[_XDEF_ITEMTYPE_COUNT];
extern _XItemMixTable*					g_ItemMixTable;
extern _XItemInchantTable*				g_ItemInchantTable;
extern short							g_maxItemMixTableCount;

class _XGameItem : public _XItem  
{
public:	
	BOOL						m_InsideFrustum;
	D3DXVECTOR3					m_Position;	
	FLOAT						m_AdditionalHeight;
	_XAnimMeshAbstractClass*	m_pMeshContainer;
	DWORD						m_SelfDestroyTimer;

	int							m_ItemWorldID;
	
	char						m_cIdentity;
	char						m_cType;				// ��з�
	char						m_cSecond;				// �ߺз�
	short						m_sID;					// �Һз� (���̵�)
	
	unsigned short				m_usOwnerID;			// ������ ID
	
	unsigned char				m_ucCount;				// ���� (���� �ƴҶ�)
	unsigned int				m_uiCount;				// ���� (���϶�)
	
	DWORD						m_LastReceiveMessageTime;

	BOOL						m_bLiveWorldID; // 2004.06.03->oneway48 insert 

public:
	_XGameItem();
	virtual ~_XGameItem();	

	void				Create( int itemworldid, D3DXVECTOR3 position, FLOAT additionalheight, _XAnimMeshAbstractClass* pMeshContainer );

	static BOOL			LoadItemProperty( void );

	static BOOL			LoadWeaponItemProperty3(FILE* fileptr);
	static BOOL			LoadClothesItemProperty3(FILE* fileptr);
	static BOOL			LoadWeaponItemProperty2(FILE* fileptr);
	static BOOL			LoadClothesItemProperty2(FILE* fileptr);
	static BOOL			LoadWeaponItemProperty(FILE* fileptr);
	static BOOL			LoadClothesItemProperty(FILE* fileptr);
	static BOOL			LoadBookItemProperty(FILE* fileptr);
	static BOOL			LoadPotionItemProperty(FILE* fileptr);
	static BOOL			LoadConsumingItemProperty(FILE* fileptr);	
	static BOOL			LoadCounteractItemProperty(FILE* fileptr);
	static BOOL			LoadResourceItemProperty(FILE* fileptr);
	static BOOL			LoadQuestItemProperty(FILE* fileptr);
	static BOOL			LoadAccessoryItemProperty(FILE* fileptr);
	static BOOL			LoadLifeItemProperty(FILE* fileptr);
	static BOOL			LoadMusicItemProperty(FILE* fileptr);
	static BOOL			LoadPassItemProperty(FILE* fileptr);
	static BOOL			LoadSocketItemProperty(FILE* fileptr);
	static BOOL			LoadElixirItemProperty(FILE* fileptr);
	static BOOL			LoadBoxItemProperty(FILE* fileptr);
	static BOOL			LoadBoxKeyItemProperty(FILE* fileptr);

	static BOOL			LoadItemTradePackage(void);
	
	static BOOL			LoadNickNameInfoTable(void);//��ȣ ���̺��� �д� �Լ�
	static BOOL			ReLoadNickNameInfoTable(void);//��ȣ ���̺��� ��Ʈ���� �ٽ� ����

	static void			ReleaseItemProperty(void);
	static BOOL			LoadItemMixTable(void);	// ���� �������� ���� ���� ���̺�
	static BOOL			LoadItemInchantTable(void);	// ���� ���� ���̺�

	static TCHAR*		GetItemName(char cType, short sID);		// addto+name
	static TCHAR*		GetItemFullName(char cType, short sID);	// nick+addto+name
	static TCHAR*		GetItemNick(char cType, short sID);
	static char			GetThird(char cType, short sID);	
	static unsigned short GetMaxDurability(char cType, short sID);	// �ִ� ������
	static unsigned char	GetStrength( char cType, short sID ); // Strength ��ġ ���� ����//Author : �����
	static unsigned char	GetNature( char cType, short sID ); // Nature ��ġ ������ ����//Author : �����
	static char			GetPocketNum(char cType, short sID);	// �������� ��ű� ����
	static unsigned int	GetItemPrice( char cType, short sID);	
	static char			GetItemSecond( char cType, short sID);	
	static unsigned char GetItemGrade( char cType, short sID);
	static D3DCOLOR		GetGradeColor( char cType, short sID );
	static unsigned int GetAddEffectFromSocket(unsigned int index1, unsigned int index2, unsigned int index3, unsigned int index4);	// �ͽ��� ���� �߰� ȿ��
	static unsigned int GetInchantEffectId(char cType, short sID, char strength);	// ��þƮ�� �������� ����Ʈ���̵� 
	static TCHAR*		GetItemNameAddtoString(char cType, short sID);
	static BOOL			CheckDump( char cType, short sID );	// ���� �� �ִ� �� �˻�
	static BOOL			CheckUserTrade( char cType, short sID );	// ���� �� ��ȯ �� �� �ִ� �� �˻�
	static BOOL			CheckNPCTrade( char cType, short sID );		// �����ŷ� �� �� �ִ� �� �˻�
	static unsigned int GetInchantPlusPoint( char cSecond, int maxvalue, int grade );	// ��þƮ �� �������� �߰� ���ݷ� 
	static BOOL			CheckClan(int cType, int sID);	// ���� �˻�
	static BOOL			GetMoneyColor(TCHAR* money, int& colorindex, BOOL mode = FALSE );
	static BOOL			CheckFame(unsigned char ucfame);	// �� or �Ǹ� �ܰ� �˻�
	static BOOL			CheckItemTempField4( char cType, short sID ); // â�� �ŷ� ���� ������ �˻�
	static int			GetSetItem( char cType, short sID ); //��Ʈ���������� �˻� //Author : �����
	static short		GetTempField5( char cType, short sID ); // �������� ���հ��� �˻�
	static unsigned char	GetSocketCheck( char cType, short sID ); // �̰� �Ⱦ���..

	// ������ ���� ���� �߰�
	static BOOL			CheckAccessoryReq(int cType, int sID);
};


class _XGameItemList : public _XList
{
public:
	FLOAT			currentrotateangle;
public:
					_XGameItemList();
	virtual			~_XGameItemList();

	void			CheckInsideFrustum(void);
	void			Render( void );
	_XGameItem*		FindWorldItem( int itemworldid );
	void			RemoveWorldItem( int itemworldid );
	void			RemoveWorldItem(void);
	void			SetDefaultLiveWorldID();	// 2004.06.03->oneway48 insert 
	BOOL			GrabWorldItem();	// 2004.06.25->oneway48 insert 
};


// ������ ������ ������ 
class _XUserItem
{
private :	// �޸� ��ȣ ����. XOR Encrypt
	unsigned short	m_usCurDur;				// ���� ������	
	unsigned short	m_usMaxDur;				// �ƽ� ������
	short			m_sSocket[4];
	unsigned char	m_ucStrength;			// ������ ��� ���� ��ġ.	
	unsigned char	m_ucSharpness;			// ������ ���, ����.
	unsigned char	m_ucWeaponStrength;		// ������ ���, ���� //Author : �����
	unsigned char   m_ucSocketCount;		// ������ ��� ���ȴ���.
	
	short			m_sID;					// ������ �Һз� (���̵�)

	char			m_cType;				// ������ ��з�
	char			m_cSecond;	

	char			m_cThird;
	unsigned char	m_ucCount;				// �ִ� ���� 255��

	unsigned char				m_ucYear ;		// Year - 2000 : 5 ~ ����.
	unsigned char				m_ucMonth ;		// Month
	unsigned char				m_ucDay ;			// Day
	unsigned char				m_ucHour ;		// Hour
	unsigned char				m_ucMinute ;		// Minute 
public :
	
	unsigned short	Get_m_usCurDur(){	return _XMEMORYCHECKDATA_ENCWORD(m_usCurDur); }
	void Set_m_usCurDur(unsigned short data){ m_usCurDur = _XMEMORYCHECKDATA_ENCWORD(data); }
	unsigned short	Get_m_usMaxDur(){ 	return _XMEMORYCHECKDATA_ENCWORD(m_usMaxDur); }
	void Set_m_usMaxDur(unsigned short data){ m_usMaxDur = _XMEMORYCHECKDATA_ENCWORD(data); }
	short Get_m_sSocket(int index ){	return _XMEMORYCHECKDATA_ENCSHORT(m_sSocket[index]); }
	void Set_m_sSocket(int index, short data){ m_sSocket[index] = _XMEMORYCHECKDATA_ENCSHORT(data); }
	unsigned char	Get_m_ucStrength(){ return _XMEMORYCHECKDATA_ENCBYTE(m_ucStrength); }
	void Set_m_ucStrength(unsigned char data){ m_ucStrength = _XMEMORYCHECKDATA_ENCBYTE(data); }
	unsigned char	Get_m_ucSharpness(){ return _XMEMORYCHECKDATA_ENCBYTE(m_ucSharpness); }
	void Set_m_ucSharpness(unsigned char data){ m_ucSharpness = _XMEMORYCHECKDATA_ENCBYTE(data); }	
	unsigned char	Get_m_ucWeaponStrength(){ return _XMEMORYCHECKDATA_ENCBYTE(m_ucWeaponStrength); }
	void Set_m_ucWeaponStrength(unsigned char data){ m_ucWeaponStrength = _XMEMORYCHECKDATA_ENCBYTE(data); }
	unsigned char	Get_m_ucSocketCount(){ return _XMEMORYCHECKDATA_ENCBYTE(m_ucSocketCount); }
	void Set_m_ucSocketCount(unsigned char data){ m_ucSocketCount = _XMEMORYCHECKDATA_ENCBYTE(data); }
	short Get_m_sID(){ return _XMEMORYCHECKDATA_ENCSHORT(m_sID); }
	void Set_m_sID(short data){ m_sID = _XMEMORYCHECKDATA_ENCSHORT(data); }

	//08.09.05 �߰� 
	char Get_m_cType(){ return _XMEMORYCHECKDATA_ENCCHAR(m_cType); }
	void Set_m_cType(char data){ m_cType = _XMEMORYCHECKDATA_ENCCHAR(data); }
	char Get_m_cSecond(){ return _XMEMORYCHECKDATA_ENCCHAR(m_cSecond); }
	void Set_m_cSecond(char data){ m_cSecond = _XMEMORYCHECKDATA_ENCCHAR(data); }
	char Get_m_cThird(){ return _XMEMORYCHECKDATA_ENCCHAR(m_cThird); }
	void Set_m_cThird(char data){ m_cThird = _XMEMORYCHECKDATA_ENCCHAR(data); }
	unsigned char	Get_m_ucCount(){ return _XMEMORYCHECKDATA_ENCBYTE(m_ucCount); }
	void Set_m_ucCount(unsigned char data){ m_ucCount = _XMEMORYCHECKDATA_ENCBYTE(data); }
	unsigned char	Get_m_ucYear(){ return _XMEMORYCHECKDATA_ENCBYTE(m_ucYear); }
	void Set_m_ucYear(unsigned char data){ m_ucYear = _XMEMORYCHECKDATA_ENCBYTE(data); }
	unsigned char	Get_m_ucMonth(){ return _XMEMORYCHECKDATA_ENCBYTE(m_ucMonth); }
	void Set_m_ucMonth(unsigned char data){ m_ucMonth = _XMEMORYCHECKDATA_ENCBYTE(data); }
	unsigned char	Get_m_ucDay(){ return _XMEMORYCHECKDATA_ENCBYTE(m_ucDay); }
	void Set_m_ucDay(unsigned char data){ m_ucDay = _XMEMORYCHECKDATA_ENCBYTE(data); }
	unsigned char	Get_m_ucHour(){ return _XMEMORYCHECKDATA_ENCBYTE(m_ucHour); }
	void Set_m_ucHour(unsigned char data){ m_ucHour = _XMEMORYCHECKDATA_ENCBYTE(data); }
	unsigned char	Get_m_ucMinute(){ return _XMEMORYCHECKDATA_ENCBYTE(m_ucMinute); }
	void Set_m_ucMinute(unsigned char data){ m_ucMinute = _XMEMORYCHECKDATA_ENCBYTE(data); }

/*
	unsigned short	Get_m_usCurDur(){ return m_usCurDur; }
	void			Set_m_usCurDur(unsigned short data){ m_usCurDur = data; }
	unsigned short	Get_m_usMaxDur(){ return m_usMaxDur; }
	void			Set_m_usMaxDur(unsigned short data){ m_usMaxDur = data; }
	short			Get_m_sSocket(int index ){ return m_sSocket[index]; }
	void			Set_m_sSocket(int index, short data){ m_sSocket[index] = data; }
	unsigned char	Get_m_ucStrength(){ return m_ucStrength; }
	void			Set_m_ucStrength(unsigned char data){ m_ucStrength = data; }
	unsigned char	Get_m_ucSharpness(){ return m_ucSharpness; }
	void			Set_m_ucSharpness(unsigned char data){ m_ucSharpness = data; }	
	unsigned char	Get_m_ucWeaponStrength(){ return m_ucWeaponStrength; }
	void			Set_m_ucWeaponStrength(unsigned char data){ m_ucWeaponStrength = data; }
	unsigned char	Get_m_ucSocketCount(){ return m_ucSocketCount; }
	void			Set_m_ucSocketCount(unsigned char data){ m_ucSocketCount = data; }

	short			Get_m_sID(){ return m_sID; }
	void			Set_m_sID(short data){ m_sID = data; }
*/
public :
	char			m_cIdentify;			// ���� ����
	char			m_cReserve;

//Author : �����
	unsigned short	m_usCurProtect : 5;	// ������ ���, �ٸ� �븰�� ��. 0 - 15
	unsigned short	m_usCurMaxProtect : 5;	// �ִ밭��
	unsigned short	m_usUsedProtect : 1;	// ��縦 ����� ���� �ִ°�.

	BOOL			m_bDurabilityDecrease;
	BOOL			m_bNewItem;

	BOOL			m_bQuestReward; // ����Ʈ ���� �߿� ���� ������ : 
									// ����Ʈ�� ���� ������ ���� ���� �ȼ��� â�� ������ ���� ����.
									// ����Ʈ�� ���� �Ŀ��� ����� �� �� ����.
	
	DWORD			m_dwCooldownStartTime;
	
	unsigned char				m_ucFlag1 ;		// Flag1 = �������� �ִ°� ���°�..
	unsigned char				m_ucFlag2 ;		// 
	unsigned char				m_ucFlag3 ;
	unsigned short				m_usValue1 ;		// 

public :
	_XUserItem();
	virtual ~_XUserItem();

	virtual void	Reset(void);
	BOOL			IsStackable(void);
	virtual void	Copy(_XUserItem useritem);
};

// â��� ������
class _XWarehouseItem : public _XUserItem
{
public :
	int				m_InventoryNumber;
	int				m_WarehouseNumber;
	
	unsigned char	m_ReceiveCount;
	
public :
	_XWarehouseItem();
	virtual ~_XWarehouseItem();
	
	void			ResetWarehouseItem(void);
	
};

// Ʈ���̵�� ������
class _XTradeItem : public _XUserItem
{
public :
	int				m_InventoryNumber;		// PC�� Ʈ���̵忡�� ���� ��
	int				m_ItemNo;

public :
	_XTradeItem();
	virtual ~_XTradeItem();
	
	void			ResetTradeItem(void);
	
};
// ����,���� ������
class _XCollectionResourceItem : public _XUserItem
{
public :
	int				m_Index;			// ����,��ῡ ���̴� ��
	bool			m_bComplete;		// �ʿ��� �������� �Ϸ�Ǿ������� üũ�Ѵ�.
public :
	_XCollectionResourceItem();
	virtual ~_XCollectionResourceItem();
	
	void			ResetCollectionResourceItem(void);
	
};
/*
// 2004.06.14->oneway48 insert
// ����ϰ� �ִ� ������ : ���� ȭ�� ��ܿ� ǥ�� (potion or �������� ���� ������) 
class _XUsingItem : public _XUserItem
{
public :
	int				m_ItemNo;
	int				m_InventoryNumber;		
	DWORD			m_StartTime;// 2004.06.14->oneway48 insert :ȸ���� ������ ���� 
	
public :
	_XUsingItem();
	virtual ~_XUsingItem();
	
	void			ResetUsingItem(void);
	
};
// insert end
*/
// Mouse slot item class
class _XMouseSlotItem : public _XUserItem
{
public :
	int				m_InventoryNumber;
	
public :
	_XMouseSlotItem();
	virtual ~_XMouseSlotItem();
	
	void			ResetMouseSlotItem(void);
};

typedef enum
{
	_XDEF_DISARM_DIRECTING_START,
	_XDEF_DISARM_DIRECTING_LENEAR,
	_XDEF_DISARM_DIRECTING_CURVE,
	_XDEF_DISARM_DIRECTING_END
}DISARM_DIRECTING_STEP;

// ���� ������ ������ 
class _XDisarmamentItem
{
public :
	D3DXVECTOR3					m_vecPosition;
	_XAnimMeshAbstractClass*	m_pMeshContainer;

	short						m_sID;

	int							m_PathID;
	DISARM_DIRECTING_STEP		m_DirectingStep;
	DWORD						m_LastProcessTime;
	DWORD						m_StartProcessTime;

	FLOAT						m_RotateAngle;
	int							m_CurrentPoint;

public :
	_XDisarmamentItem();
	~_XDisarmamentItem();

	void						Create(short sID, D3DXVECTOR3 position, _XAnimMeshAbstractClass* pMesh);
	void						Render(void);
	void						Delete(void);
};
char GetEqualItemType( char cType );
int	CheckCashItem( char cType, short sID );
void* GetFirstTypeItemPorperty( char cType );

void PlayItemUseSound( char cType, short sID );
void PlayItemEquipSound( char cType, short sID );


class _XCashItemPackage {
public:
	int		m_indexCashItemPackage;			// ��Ű�� �ε���
	int		m_CashItemType;
	int		m_CashItemID;
	int		m_indexPackageIcon;		// 2D ��Ʈ�� ������ �ε���
#ifdef _XDEF_EXPANDSTRINGDBCATEGORY_20070321	
	LPTSTR	m_PackageName;	
	LPTSTR	m_PackageDiscription;
//	TCHAR	m_PackageName[ITEM_NAME_LENGTH];	// ��Ű�� �̸�
//	TCHAR	m_PackageDiscription[1024];	// ��Ű�� ����
#else
	TCHAR	m_PackageName[ITEM_NAME_LENGTH];	// ��Ű�� �̸�
	TCHAR	m_PackageDiscription[1024];	// ��Ű�� ����
#endif
	
public:
	_XCashItemPackage();
	~_XCashItemPackage();
};

class _XCashItemPackageManager {
public:
	list <_XCashItemPackage*>		m_listCashItemPackage;	// ���� ����Ʈ
	
public:
	_XCashItemPackageManager(); 
	~_XCashItemPackageManager();
	
	BOOL LoadCashItemPackage(FILE* fileptr);
};


#endif // !defined(_XGAMEITEM_H_)

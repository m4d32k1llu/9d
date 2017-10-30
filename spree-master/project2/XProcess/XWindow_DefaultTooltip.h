// XWindow_DefaultTooltip.h: interface for the _XWindow_DefaultTooltip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_DEFAULTTOOLTIP_H__A2939C6B_1C77_4645_9BAB_7E543B4B6BE2__INCLUDED_)
#define AFX_XWINDOW_DEFAULTTOOLTIP_H__A2939C6B_1C77_4645_9BAB_7E543B4B6BE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XKernel.h"

/**
 * \brief ����â 
 * Ŭ���� �ߴ� ��� ����
 *
 * ������ ������ ����. 
 * Ȯ�强�� �ʹ� ������(�������̽� ���� ���� ��ȹ���� ���ǰ� �ʿ�)
 *
 * �Ϲ� ������ Ȯ�� ����(���Ͼ������� ������ �����۵�- ����, �ǻ�) ������ ����
 * ������ ���� ������ ����( ���δ� �Һ�)
*/

/** �ϳ��� ������ �ʿ��� ������
 *	�ϳ��� ������ �ʿ��� ������ �þ� �� ��� Ȯ�� �ؾ� �� 		
*/
 
typedef struct _tag_ItemInfo
{
	// New -----------------=
	TCHAR NickBuf[256];
	TCHAR NameBuf[256];

	TCHAR Temp_Data_Buf[256];
	
	TCHAR DescBuf[256];
	TCHAR LData_Buf[512];
	TCHAR LData_Buf2[512];
	
	TCHAR Dur_Data_Buf[256];
	TCHAR Strength_Data_Buf[256]; //Author: �����
	TCHAR Data_Buf[16][256];
	TCHAR S_Data_Buf[5][256];
	TCHAR SetItemSkill_Data_Buf[10][256]; //Author : ����� //breif : ��Ʈ ������ ����
	TCHAR SetItem_Data_Buf[7][256]; //Author : ����� //breif : ��Ʈ ������ ����
	TCHAR E_Data_Buf[10][256];	

	int Data_Value1;
	int Data_Value2;
	int Data_Value3;
	int Data_Value4;
	
} _ITEMTOOLTIP_INFO;


/// ���� Ÿ��
typedef enum 
{
	TMODE_NONE=0,					
	TMODE_DEFAULTITEM,				///< �⺻���� , �κ�
	TMODE_CINFOITEM,				///< �ι�����â�� ������ ������ 
	TMODE_NPCTRADE_INVENITEM,		///< npc�ŷ��� �κ� ������
	TMODE_NPCTRADE_NPCITEM,			///< npc�ŷ��� �Ǹ� ������
	TMODE_WAREHOUSE_WAREHOUSEITEM,	///< â�� ������
	TMODE_PCTRADE_TARGETITEM,		///< pc �ŷ��� ���� ������
	TMODE_PCTRADE_SELFITEM,			///< pc �ŷ��� �ڽ��� ������
	TMODE_QUEST,					///< �ڽ��� questâ ������
	TMODE_NPCQUEST,					///< npc ����� ����Ʈ ������
	TMODE_PERSONALSTORE_TRADEITEM,	///< ���� ������
	TMODE_REPAIRITEM,				///< ���� ������
	TMODE_GAMBLE,					///< �ϻ��� �Ǹ� ������
	TMODE_TARGETINFO,				///< ���� �ι�����â ������
	TMODE_CASHITEM,					///< ĳ�� ������
	TMODE_INCHANT,					///< ����â ������ //���븰â ������ �߰�Author ����� 
	TMODE_LIFESKILLCOLLECTIONITEM,	///< ���� ������
	TMODE_LIFESKILLRESOURCEITEM,	///< ��� ������
	TMODE_NPCCOLLECTIONITEM,		///< npc���� ������
	TMODE_NPCRESOURCEITEM,			///< npc��� ������
	TMODE_SKILL,					///< ��ų
	TMODE_TRADESKILL,				///< ��������â ��ų
	TMODE_MASTERYSKILL				///< ����
}_TOOLTIP_MODE;

class _XWindow_MasterySkill;
class _XWindow_PersonalStore;
class _XWindow_TargetInfo;
class _XWindow_NPCCollection;
class _XWindow_NPCResource;
class _XWindow_NPCResource_New;

class _XWindow_DefaultTooltip : public _XWindow  
{
private:	
	_TOOLTIP_MODE		m_CurrentTooltipMode;
	SIZE				m_TooltipSize;				///< �⺻ ������
	SIZE				m_ExpandTooltipSize;		///< ������ ������ �������� ���(����,�ǻ�)
	SIZE				m_SetItemExpandTooltipSize; // Author : ����� //breif : ��Ʈ������ ������
	int					m_CurrentDrawPosY;

	_ITEMTOOLTIP_INFO	m_CurrentTooltipInfo;

	/**
	 * m_CurrentSlotNumber, m_ItemType, m_ItemId ���� tooltipmode�� �´� �����鸸 ��� 
	*/
	int					m_CurrentSlotNumber;
	int					m_ItemType;
	int					m_ItemId;
	int					m_CurrentItemSockekRealCount;	///< ������ ���� ������ ����
	int					m_CurrentItemSocketCount;		///< ������ �� �ִ� ���� ����
	int					m_DeleteSkillId;				///< ������ ��ų ���̵�
	
	int					m_nPriceMode;					// ����=0 or ���ı⿩��=1

	_XUserItem			m_AddSocketItemList[4];			///< ���� ������ ����

	_XImageStatic*		m_TooltipEdgeImage[4];			
	_XImageStatic*		m_TooltipIconBoxImage[2];		

	_XImageStatic*		m_MoneyImage;
	_XImageStatic*		m_ContributionImage;
	_XStickGauge*		m_SkillExp;
	_XButton*			m_DeleteSkillBtn;

	_XImageStatic*		m_pClanPointImage;

	_XWindow_NPCTrade*		m_pNPCTradeWindow;
	_XWindow_PCTrade*		m_pPCTradeWindow;
	_XWindow_Warehouse*		m_pWarehouseWindow;
	_XWindow_RepairItem*	m_pRepairItem_Window; 
	_XWindow_EnterBossRoom*	m_pEnterBossRoom_Window;
	_XWindow_SocketPlugIn*  m_pSocketPlugIn_Window;
	_XWindow_NPCScript*		m_pNPCScript_Window;
	_XWindow_PersonalStore* m_pPersonalStore_Window;
	_XWindow_Contribution*	m_pContribution_Window;
	_XWindow_TargetInfo*	m_pTargetInfo_Window;
	_XWindow_CashItem*		m_pCashItem_Window;
	_XWindow_NPCCollection*	m_pNPCCollection_Window;
	_XWindow_NPCResource*	m_pNPCResource_Window;
	_XWindow_Inchant* m_pInchant_Window;
	_XWindow_NPCResource_New* m_pNPCResource_New_Window;
	
private:
	void	ResetTooltipInfo();	

#ifdef _XTS_ITEM_OPTIMIZATION
	void	SetInfoWeaponItem();
	void	SetInfoClothesItem();
#endif
	void	SetInfoWeapon();
	void	SetInfoClothes();
	void	SetInfoWeapon2();
	void	SetInfoClothes2();
	void	SetInfoWeapon3();
	void	SetInfoClothes3();
	void	SetInfoBook();
	void	SetInfoPotion();
	void	SetInfoConsuming();
	void	SetElixir();
	void	SetInfoResource();
	void	SetInfoQuest();
	void	SetInfoAccessory();
	void	SetInfoLife();
	void	SetInfoSocket();
	void	SetInfoBox();
	void	SetInfoBoxKey();

#ifdef _XTS_ITEM_OPTIMIZATION
	void	DrawTextWeaponItem();
	void	DrawTextClothesItem();
#endif
	void	DrawTextWeapon();	
	void	DrawTextClothes();	
	void	DrawTextWeapon2();
	void	DrawTextClothes2();
	void	DrawTextWeapon3();
	void	DrawTextClothes3();
	void	DrawTextBook();
	void	DrawTextPotion();
	void	DrawTextConsuming();
	void	DrawElixir();
	void	DrawTextResource();
	void	DrawTextQuest();
	void	DrawTextAccessory();
	void	DrawTextLife();
	void	DrawTextSocket();
	void	DrawTextBox();
	void	DrawTextBoxKey();
	unsigned char GetInchantInfo();

	unsigned short GetSmeltItem_ScaleInfo(); //Author : �����
	unsigned short GetSmeltItem_MaxScaleInfo(); //Author : �����
	unsigned short GetSmeltItem_TearInfo(); //Author : �����
public:
	_XWindow_DefaultTooltip();
	virtual ~_XWindow_DefaultTooltip();

	BOOL	Initialize(void);
	void	DestroyWindow(void);
	
	void	Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL	Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	/// ��ó�� �����ϴ� �Լ��� ��忡 �´� �Ϲ� �ּ� ũ��� Ȯ�� ũ�� ���� 
	BOOL	SetTooltipMode( _TOOLTIP_MODE mode, int slotnumber, int type, int id, int nPriceMode = 0);

	/// �ش� ����� �Ϲ� ũ�� ����
	BOOL	SetTooltipInfo();
	void	ShowWindow(BOOL show);
	void	MoveWindow(int X, int Y);		
	
	/// ��忡 ���� �׸��� �Լ���
	void	DrawItemTooltip();
	void	DrawTooltipLine( int left, int top, int right, int bottom );
	void	DrawItemDefault();	
	void	DrawItemPrice( int nDrawY ); //Author : ����� //breif : ���� �ٲ�� ���� �������̾ �Լ��ȿ��� ó���Ҽ� ����
	void	DrawItemClanPoint( int nDrawY ); //Author : ����� //breif : ���� �ٲ�� ���� �������̾ �Լ��ȿ��� ó���Ҽ� ����
	void	DrawDurability();
	void	DrawSkillTooltip();
	void	DrawMasterySkillTooltip();
	void	DrawGambleItemTooltip();

	int		GetInnerLevel(int innerlevel);		/// ��(1~12) ���
	int		GetDeleteSkillID() { return m_DeleteSkillId; };
	void	SetDeleteSkillId() { m_DeleteSkillId = m_ItemId; };
 
	void SetNPCTradeWindowPtr(_XWindow_NPCTrade* windowptr)		{	m_pNPCTradeWindow = windowptr;	}
	void SetPCTradeWindowPtr (_XWindow_PCTrade* windowptr)		{	m_pPCTradeWindow = windowptr;	}
	void SetWarehouseWindowPtr(_XWindow_Warehouse* windowptr)	{	m_pWarehouseWindow = windowptr;	}
	void SetRepairItemWindowPtr(_XWindow_RepairItem* windowptr)	{	m_pRepairItem_Window = windowptr;	}
	void SetEnterBossRoomWindowPtr(_XWindow_EnterBossRoom* windowptr)	{	m_pEnterBossRoom_Window = windowptr;	}
	void SetSocketPlugInWindowPtr( _XWindow_SocketPlugIn* windowptr) { m_pSocketPlugIn_Window = windowptr; }	
	void SetNPCScriptWindowPtr( _XWindow_NPCScript* windowptr) { m_pNPCScript_Window = windowptr; }		
	void SetPersonalStoreWindowPtr( _XWindow_PersonalStore* windowptr) { m_pPersonalStore_Window = windowptr; }		
	void SetContributionWinddowPtr( _XWindow_Contribution* windowptr) { m_pContribution_Window = windowptr;	}
	void SetTargetInfoWindowPtr( _XWindow_TargetInfo* windowptr) { m_pTargetInfo_Window = windowptr; }
	void SetCashItemWindowPtr( _XWindow_CashItem* windowptr ) { m_pCashItem_Window = windowptr; 	}
	void SetNPCCollectionWindowPtr( _XWindow_NPCCollection* windowptr) { m_pNPCCollection_Window = windowptr; }
	void SetNPCResourceWindowPtr( _XWindow_NPCResource* windowptr ) { m_pNPCResource_Window = windowptr; 	}
	void SetInchantWindowPtr( _XWindow_Inchant* windowptr ) { m_pInchant_Window = windowptr; 	}
	void SetNewNPCResourceWindowPtr(_XWindow_NPCResource_New* windowptr)	{m_pNPCResource_New_Window = windowptr;	}
};

#endif // !defined(AFX_XWINDOW_DEFAULTTOOLTIP_H__A2939C6B_1C77_4645_9BAB_7E543B4B6BE2__INCLUDED_)

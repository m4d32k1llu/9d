// XWindow_NPCTrade.h: interface for the _XWindow_NPCTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NPCTRADE_H__E74C98E5_B9B4_43A5_9FB9_AADEDC404E7D__INCLUDED_)
#define AFX_XWINDOW_NPCTRADE_H__E74C98E5_B9B4_43A5_9FB9_AADEDC404E7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable: 4786)

#include <list>
#include "XKernel.h"
#include "XNPCScript.h"

using namespace std;

/**
 * \brief NPC �ŷ�â 
 * 
*/

#define _XDEF_MAX_NPCTRADESLOT			(25)

/// �ǸŸ�� �� - �ִ� 5�� 
typedef enum
{
	_XTRADETAB_ONE = 0,
	_XTRADETAB_TWO,
	_XTRADETAB_THREE,
	_XTRADETAB_FOUR,
	_XTRADETAB_FIVE
}_XNPCTRADETAB;		

typedef enum tagXNPCTRADEITEMEDGEICONTYPE
{
	_XNT_ITEMEDGEICONTYPE_EMPTY = 0,
	_XNT_ITEMEDGEICONTYPE_NORMAL,
	_XNT_ITEMEDGEICONTYPE_DISABLESLOT,
} _XNPCTRADEITEMEDGEICONTYPE;

/// �ϻ��� ������ 
struct _XGambleItem 
{
	int Type;
	int Id;
	int Price;
	int cluindex;
	int ItemNo;
};

enum
{
	_MODE_NORMAL = 0,		// ������ ����
	_MODE_CLANPOINT = 1,	// ���� �⿩���� ����
	_MODE_MNDPOINT = 2		// ���� �⿩���� ����
};

class _XWindow_NPCTrade : public _XWindow  
{
// add data ->2005.1.5
private:
	_XImageStatic*		m_NPCJobIcon;
	_XImageStatic		m_ItemEdgeIcon[3];
	_XCheckButton*		m_SelectTabButtton[5];	
	BOOL				m_BlackMarketMode;		///< �ϻ��θ��

	int					m_nPriceMode;			///< ������ �����, ���ı⿩���� �����
	
public :
	int*				m_GambleItemPatten;		///< �׺������ ���ϸ���Ʈ
	_XGambleItem*		m_GambleItemInfo;		///< �׺� ������ ����Ʈ

	_XNPCTRADETAB		m_SelectedTab;	
	
	int					m_OwnerNPCID;
	int					m_ItemPackageID;
	int					m_AdditionalItemPackageID;
	int					m_SpecialItemPackgeID;
	
	BOOL 				m_bClanMode;					///< �����Ǻ�
	BOOL 				m_bHonorMode1;					///< ���Ǻ� : ������ ������ �� ���
	BOOL 				m_bHonorMode2;					///< ���Ǻ� : ���ǿ� �´� ���̸� üũ -> �߰� ������ ����Ʈ 
	BOOL 				m_bCustomerMode;				///< �ܰ��Ǻ�

	_XMouseSlotItem		m_MouseSlotItem;

	BOOL				m_bHaveMouseSlot;
	BOOL				m_bDragState;
	POINT				m_IconDragStartPos;
	int					m_LastDraggingSlot;

	BOOL				m_bDrawSelectedItem;

	list <_XTradeItem>	m_TradeItemList;		///< �Ǹ� ������ ����Ʈ
	POINT				m_SlotPosition[_XDEF_MAX_NPCTRADESLOT];	///< �Ǹ� ������ ��ġ

	BOOL				m_bIsBuy;
	int					m_MaxViewCount;
	BOOL				m_bTradeMenu;
	
public:
	_XWindow_NPCTrade();
	virtual ~_XWindow_NPCTrade();
	
	BOOL			Initailize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusedobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	void			MoveWindow( int X, int Y ); 

	void			ChangeTab(_XNPCTRADETAB tab);

	void			SetTradeMode(_XNPCScriptItem* npcsubscript);
	void			SetOwnerNPCID(int npcid)
	{
		m_OwnerNPCID = npcid;
	}
	
	void			SetBlackMarketMode( BOOL mode )		///< �ϻ��� ��� 
	{
		m_BlackMarketMode = mode;
	}
	void			SetItemPackageID(int packageid);
	void			SetAdditionalItemPackageID(int packageid);
	void			SetSpecialItemPackageID(int packageid);

	void			ProcessBuy(void);
	void			ProcessSell(void);

	void			ProcessDblClick(void);

	void			SetDefaultPosition( void );	
	void			ShowWindow( BOOL show );	

	void			DrawTradeItemIcon(void);
	void			DrawSelectedItem(void);
	void			DrawDragIcon(void);
	int				GetSlotIndex(void);
	void			SetListItem(void);

	void			DrawItemTooltip(int slotnumber, RECT iconrect);

	BOOL			GetItemPropertyFromSlot(int selectedslot, char& cType, short& sID, int& itemNo);
	BOOL			GetItemPropertyFromNo(int itemNo, char& cType, short& sID, int& slotnumber);

	void			SetNPCJobIcon(int jobindex);
	BOOL			CheckCondition(char cType, int sId);
	int				PriceCalc(int slotnumber, int count = 1);		///< ���� ���
	
	void			LoadGambleItemTradePackage(int pattenindex);	///< �׺� ������ ��� �б�

	void			SetPriceMode(int mode)	{	m_nPriceMode = mode;	};
	int				GetPriceMode()			{	return m_nPriceMode;	};
};

#endif // !defined(AFX_XWINDOW_NPCTRADE_H__E74C98E5_B9B4_43A5_9FB9_AADEDC404E7D__INCLUDED_)

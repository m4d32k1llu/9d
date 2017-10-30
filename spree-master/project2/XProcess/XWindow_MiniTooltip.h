// XWindow_MiniTooltip.h: interface for the _XWindow_MiniTooltip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MINITOOLTIP_H__442EF62D_C27A_4BEF_B7FE_E1A086466FEB__INCLUDED_)
#define AFX_XWINDOW_MINITOOLTIP_H__442EF62D_C27A_4BEF_B7FE_E1A086466FEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "XWindow.h"
/**
 * \brief ����â 
 * ���콺 ������ �ߴ� ��� ����
 *
 * ������ ������ ����. 
 * Ȯ�强�� �ʹ� ������(�������̽� ���� ���� ��ȹ���� ���ǰ� �ʿ�)
 *
 * ������ ���� ������ ����( ���δ� �Һ�)
 * ���� ���� 
 * ���� �ܰ躰 �ִ�ġ ǥ�ø� �ҽ��ȿ��� �� 
 * ��ġ ����� MTMODE_CHARINFO_ABILITYSTEP�� �˻��ؼ� MaxValue���� ���� �����ؾ� ��
*/

/** �ϳ��� ������ �ʿ��� ������
 *	�ϳ��� ������ �ʿ��� ������ �þ� �� ��� Ȯ�� �ؾ� �� 		
*/
typedef struct _tag_Mini_ItemInfo
{
	TCHAR NameBuf[128];	
	TCHAR LData_Buf[512];
	TCHAR Data_Buf[12][128];	
}_MINIITEMTOOLTIP_INFO;

/// ���� Ÿ��
typedef enum 
{
	MTMODE_NONE=0,
	MTMODE_ITEM,					///< ������ ������
	MTMODE_SKILL,					///< ������ ��ų
	MTMODE_BUFF_EXTERNAL,			///< ����â �ܻ�
	MTMODE_BUFF_INTERNAL,			///< ����â ����
	MTMODE_BUFF_ITEM,				///< ����â ������
	MTMODE_BUFF_SKILL,				///< ����â ��ų
	MTMODE_BUFF_EFFECT,				///< ����â ����ȿ��
	MTMODE_BUFF_MONSTATS_SPELL,		///< ����â ��ų
	MTMODE_BUFF_MONSTATS_EFFECT,	///< ����â ����ȿ��
	MTMODE_BUFF_MONSTATS_STATE,		///< ����â ����
	MTMODE_CHARINFO,				///< JinStatusâ
	MTMODE_CHARINFO_ABILITY,		///< �ι�����â �ɷ� ����
	MTMODE_CHARINFO_ABILITYSTEP,	///< �ι�����â ���� ����
	MTMODE_CHARINFO_INTOXICATION,	///< ���� �ߵ� ����
	MTMODE_CHARINFO_FAME,			///< �� ����
	MTMODE_PARTYINFO,				///< ��Ƽ ����
	MTMODE_BUFF_INTOXICATION,		///< ���� �ߵ�
	MTMODE_BUFF_NICKNAME,			///< ��ȣ ����
	MTMODE_BUFF_STUDYBOOK,			///< ������ ����
	MTMODE_PAYMENTINFO,				///< SDS ����
	MTMODE_BUFF_PEMODE,				///< �������� ����
	MTMODE_PK_RECORD,				///< ������Ȳ
	MTMODE_PK_TRACINGSERVICES,		///< PK ��ġ���� ����
	MTMODE_ITEM_SHOUT,				///< ��ġ��
	MTMODE_ITEM_ACCOUNT,			///< ����ġ/���õ�/�������
	MTMODE_VIETNAMPAYMENTINFO,		///< ��Ʈ�� �˴ٿ�
	MTMODE_VIEWGROUP_CASTLEINFO,	///< ����â �������
	MTMODE_PCRPREMIUM,				///< PC�� ������
	MTMODE_CASTLEINFO,				///< ��� ������ ����
	MTMODE_NOTORIOUS,				///< ���� ����
	MTMODE_NPCBUFFINFO				///< NPC ���� ����
}_MINITOOLTIP_MODE;

class _XWindow_MiniTooltip : public _XWindow  
{
private:
	_MINIITEMTOOLTIP_INFO	m_CurrentMiniTooltipInfo;
	_MINITOOLTIP_MODE		m_CurrentMiniTooltipMode;
	SIZE					m_MiniTooltipSize;

	int						m_CurrentDrawPosY;
	int						m_CurrentSlotNumber;
	int						m_ItemType;
	int						m_ItemId;
	DWORD					m_TimeCount;
	_XImageStatic*			m_RoundEdgeImage[4];

	_XImageStatic*			m_pGroupIcon;

	void	ResetMiniTooltipInfo();
	
public:
	_XWindow_MiniTooltip();
	virtual ~_XWindow_MiniTooltip();

	BOOL	Initialize(void);
	void	DestroyWindow(void);
	
	void	Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL	Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void	DrawMiniItem();
	void	DrawMiniSkill();
	void	DrawCharInfo();
	void	DrawCharInfoAbility();
	void	DrawCharInfoAbilityStep();
	void	DrawCharInfoIntoxication();	
	void	DrawCharInfoFame();
	void	DrawMiniBuffExternal();
	void	DrawMiniBuffInternal();			
	void	DrawMiniBuffItem();			
	void	DrawMiniBuffSkill();
	void	DrawMiniBuffEffect();
	void	DrawMonstatsSpell();
	void	DrawMonstatsEffect();
	void	DrawMonstateState();
	void	DrawPartyInfo();
	void	DrawMiniBuffIntoxication();
	void	DrawMiniBuffNickname();
	void	DrawMiniBuffStudyBook();
	void	DrawMiniBuffPeMode();
	//void	DrawPaymentMethod(); ����� �ּ�ó��
	void	DrawPaymentMethod_NormalUser();
	void	DrawPaymentMethod_Premium1User();
	void	DrawPaymentMethod_Premium2User();
	void	DrawPaymentMethod_NormalPCRoom();
	void	DrawPaymentMethod_PremiumUser1NormalPCRoom();
	void	DrawPaymentMethod_PremiumUser2NormalPCRoom();
	void	DrawPaymentMethod_PremiumPCRoom();
	void	DrawPaymentMethod_PremiumUser1AndPCRoom();
	void	DrawPaymentMethod_PremiumUser2AndPCRoom();	
	void	DrawPKRecord();
	void	DrawVietnamPaymentMethod();

	void	DrawMiniItemShout();
	void	DrawAccountItemUse();

	void	DrawViewGroupCastleInfo();

	void	DrawCastleInfo();

	void	ShowWindow(BOOL show);

	BOOL	SetTooltipMode( _MINITOOLTIP_MODE mode, int slotnumber, int type, int id, DWORD Time = 0, int DrawX = 0, int DrawY = 0 );
	BOOL	SetTooltipInfo();
	_MINITOOLTIP_MODE GetMiniTooltipMode() { return m_CurrentMiniTooltipMode; }
	int     GetCurrentSlotNumber() { return m_CurrentSlotNumber; }

	int		GetItemId()		{	return m_ItemId;	}

private:
	void	DrawPaymentInfo();	//!< @author �����
	void	DrawPkTracingservices(); //!< @author �����
	void	DrawCharInfoNotorious();	//Author : ����� //breif : ����
	void	DrawNPCBuffWindowInfo(); //Author : ����� //breif : ������ ����â (�̱������� ��� )
	void	DrawPCRPremium();
};

#endif // !defined(AFX_XWINDOW_MINITOOLTIP_H__442EF62D_C27A_4BEF_B7FE_E1A086466FEB__INCLUDED_)

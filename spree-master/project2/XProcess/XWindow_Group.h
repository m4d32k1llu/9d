// XWindow_Group.h: interface for the _XWindow_Group class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_GROUP_H__452F2A05_6E59_47A7_94DD_C88636A496DA__INCLUDED_)
#define AFX_XWINDOW_GROUP_H__452F2A05_6E59_47A7_94DD_C88636A496DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "packet_for_organization.h"

/**
 * \brief ������ â- ����
 * ������ ���� ������ �����ش�.
 * ������ ���� ��� ����
*/

/// ������ �������� ���
typedef enum
{
	_XNOTICEMODE_READ		= 0,
	_XNOTICEMODE_WRITE,
	_XNOTICEMODE_MODIFY
}_XNoticeMode;

/// ������ ���� ����
struct XORGANIZATION_NOTICEINFO
{
	TCHAR		writer[13] ;
	TCHAR		writedate[15] ;
	TCHAR		notice[272] ;

	void reset()
	{
		memset( writer, 0, sizeof(writer) );
		memset( writedate, 0, sizeof(writedate) );
		memset( notice, 0, sizeof(notice) );
	} ;

} ;

typedef enum
{
	_XGROUPTAB_GROUPCLASS1 = 0,	// ������
	_XGROUPTAB_GROUPCLASS2,	// ��ȸ
	_XGROUPTAB_GROUPCLASS3,	// ���
	_XGROUPTAB_GROUPCLASS4,	// �����Ȳ
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ����� //breif : �����ý���
	_XGROUPTAB_GROUPCLASS5,	// ����
#endif
	_XGROUPTAB_TOTALCOUNT
}_XGroupWindowTab;



/// �������� �ִ���ڼ�
#define _XDEF_MAX_NOTICELENGTH	271

class _XWindow_MnDGroup; //Author : ����� //breif: �������� ��� �����ϸ� ������ �������..

class _XWindow_Group : public _XWindow  
{
public:	
	// ����
//#ifdef _XTS_ALIANCE_20061018	
	_XCheckButton*			m_BaseTabButton[_XGROUPTAB_TOTALCOUNT];
//#else	
//	_XCheckButton*			m_BaseTabButton;
//#endif
	
	// ������ ���� �� 
	_XStickGauge*			m_pCotributionBorder;
	_XImageStatic*			m_pContributionGaugeLeftBorder;
	_XImageStatic*			m_pContributionGaugeRightBorder;	
	
	_XImageStatic*			m_pImageContributionRateLeftBorder;		// ���� ��� â ���� 1
	_XImageStatic*			m_pImageContributionRateRightBorder;	// ���� ��� â ���� 2
	_XImageStatic*			m_pImageLineUpLeftBorder;				// ���� ��� â ���� 1
	_XImageStatic*			m_pImageLineUpRightBorder;				// ���� ��� â ���� 2

	// ��ȸ ���� �� 	
#ifdef _XTS_ALIANCE_20061018	
	_XImageStatic*			m_pImageAlienceBorder;					// ������ ���� â ����
	_XImageStatic*			m_pImageAlienceName1;					// ������ ���� â '��'��
	_XImageStatic*			m_pImageAlienceName2;					// ������ ���� â 'ȸ'��
	_XImageStatic*			m_pImageAlienceMemberBorder;			// ������ ���� �ɹ� ��ư �׵θ�
	_XImageStatic*			m_pImageAlienceMemberEmptyBorder;		// ������ ���� �ɹ����� �̹��� 
#endif
#ifdef _XDEF_CASTLEBATTLE	
	_XImageStatic*			m_pImageCastleBattleBorder1;			// ����� �ؽ��� ���� �̹��� 1
	_XImageStatic*			m_pImageCastleBattleBorder2;			// ����� �ؽ��� ���� �̹��� 2
#endif
	
	// ������ ���� �� 
	_XListBox*				m_OrganizationListBox;
	_XListBox*				m_SortTypeList;
	
	
	_XVScrollBar*			m_NoticeScrollBar;
	
	// ������ ���� �� 
	_XButton*				m_pNoticeWrite;
	_XButton*				m_pNoticemodify;
	_XButton*				m_pBtnGroupLeave;		// ������ Ż�� ��ư
	_XButton*				m_pBtnLineUpDropDown;	// ������ ���� ��ư 

	_XButton*				m_PopUpButton[8];
	
	// ��ȸ ���� �� 
#ifdef _XTS_ALIANCE_20061018	
	_XButton*				m_pBtnAlienceName[5];	// ������ ���� ������ ǥ���ϴ� ��ư 
	_XButton*				m_pBtnAlienceDestroy;	//������ ���� �ı� ��ư
#endif

#ifdef _XDEF_ALIANCE_WEBBOARD_20070130	
	_XButton*			m_pBtnGoToOrganization;		// ������ �Խ��� �ٷΰ��� ��ư
	_XButton*			m_pBtnGoToAliance;			// ����ȸ �Խ��� �ٷΰ��� ��ư
#endif

#ifdef _XDEF_CASTLEBATTLE	
	_XButton*			m_pBtnCastleBattleTimeChange;		// ����� �ð� ���� ��ư
	_XButton*			m_pBtnCastleBattleGetMoney;			// ����� ������ ȹ�� ��ư
		
	_XImageStatic*		m_pScheduleIcon[2];
	TCHAR				m_strDate[3][256];
	TCHAR				m_strPropertyDate[256];

	_XListBox*			m_lstboxChallengers;
	_XCastleInfo*		m_pMyCastleInfo;
	
	BOOL				m_bSetCastleTooltip;
#endif
	
	
	// ������ ���� �� 
	list<_Unit*>			m_OrganizationMemberList;	///< �����ܿ����� ����Ÿ 	
	list<_OutTime*>	m_OrganizationMemberOutTimeList;	///< �����ܿ����� ���� ���� ���� �ð�	
	
	// ��ȸ ���� �� 
#ifdef _XTS_ALIANCE_20061018
	list<__ally_client_info*>	m_listAlienceMember;	// ������ ���� �ɹ����� ������ �̸�, �ε���
#endif

	_XImageStatic			m_CornerImage[4];
	_OrganizationBase		m_OrganizationBaseInfo;	
	
	XORGANIZATION_NOTICEINFO	m_NoticeText;			///< ���� ����Ǿ��ִ� ������ ����	
	XORGANIZATION_NOTICEINFO	m_SendNoticeTempText;	///< ������ ������ ���� 
	_XIMEContainer			m_NoticeIMEControl;

	TCHAR					m_GroupInfoText[9][64]; 
	TCHAR					m_SendName[13];
	TCHAR					m_CurrentSortName[128];	///< ���� ����Ǵ� ����Ÿ�Ը�
	TCHAR					m_NoticeTempText[256][128];
	TCHAR					m_NoticeViewText[256][128];	
	
	// ��ȸ ���� �� 
#ifdef _XTS_ALIANCE_20061018
	TCHAR					m_AlienceMasterName[13];		// ȸ�� �̸��� �����Ѵ�.
#endif
	_Unit					m_UpdateUnit;

	POINT					m_DrawPosPopUp;
	
	// ������ ���� �� 
	int						m_PopUpCount;
	int						m_ManagerCount;

	int						m_NoticeTextScrollPos;
	int						m_NoticeTextLength;
	int						m_NoticeTextTotalCount;
	int						m_CurrentNoticeLinePos;	
	int						m_NoticeViewTextTotalCount;
	int						m_CurrentTargetedUserID;
	
	// ��ȸ ���� �� 
#ifdef _XTS_ALIANCE_20061018
	int						m_UniqueIndexAlience;		// ������ ���� ���� �ε���
#endif

	
	// ������ ���� �� 
	BOOL					m_ShowPopUpLine;

	/// ��������, �������� ���� �÷���
	BOOL					m_SortLevelFlag;	
	BOOL					m_SortZoneFlag;
	BOOL					m_SortOnlineFlag;
	
	// ��ȸ ���� �� 
		
	_XGroupWindowTab		m_SelectedTab;	//����â ���� �� : ������, ��ȸ
	_XNoticeMode			m_NoticeMode;
	
	// ��� ��Ȳ
	_XImageStatic*			m_TableBarLong;
	_XImageStatic*			m_TableBarShort;
	_XListBox*				m_lstboxOrgRecord;

	int						m_nEntryCount;	// ����� ����ȸ��

	int						m_nWinCount;		// �¸� ȸ��
	int						m_nLoseCount;		// ���� ȸ��
	int						m_nCombatCount;	// �� ȸ��
	int						m_nWinRate;		// ���� �·�

	int						m_nManorIndex;	// ������ ����� ������ ��

	// ������ ����� ���� ��¥
	int						m_nYear		: 8;
	int						m_nMonth		: 8;
	int						m_nDay		: 8;
	int						reserved	: 8;
	TCHAR					m_strLastWarDate[128];

	_XWindow_MnDGroup*		m_pMnDWindow;	//Author : ����� //breif : �������� ��

	
public:
	_XWindow_Group();
	virtual ~_XWindow_Group();

	BOOL 		Initialize( void );
	void		DestroyWindow( void );
	void		ResetData();
	
	void 		MoveWindow( int X, int Y );

	void 		Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void		DrawPopUp();
	BOOL		Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void		ShowWindow(BOOL show);
	void		ChangeTab(_XGroupWindowTab selectedtab);

	void		RefreshData(void);
	void		SetChallengerListItem(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//											������ ���� �Լ�													//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void		CopyBaseInfo(_OrganizationBase* data);		///< ���� -> ������ �⺻����	
	void		CopyMemberList(int count, _Unit* unitlist);	///< ���� -> �����ܿ� ���� 	
	void		CopyMemberOutTimeList( int count, _OutTime* unitouttimelist );	///< ���� -> �����ܿ� �������ӽð� ���� 	
	void		UpdateTextInfo();	///< ���� -> ������ ���� ������Ʈ	
	void		UpdateOrganizationListBox();	///< ���� -> �����ܿ� ���� ������Ʈ
	
	
	void		JoinOrganizationMember(_Unit* unit, _OutTime* outtime);		///< �����ܿ� ����	
	void		DeleteOrganizationMember(_Unit* unit, _OutTime* outtime);	///< �����ܿ� ����Ż��	
	void		DeleteCompulsionOrganizationMember(_Unit* unit, _OutTime* outtime);	///< �����ܿ� �߹�	
	void		UpdateOrganizationMember(_Unit* unit, _OutTime* outtime);	///< �����ܿ� ������Ʈ	
	void		UpdateOrganizationEntrust(_Unit* unit);	///< �����ܿ� Ŭ���� ���� 	
	void		BreakUpOrganization(void);	///< ������ ��ü
	// ------------------------------------------------=

	TCHAR*		GetSendTargetName() { return m_SendName;	};
	void		SetSendTargetName(TCHAR* targetname) { strcpy( m_SendName, targetname ); 	};
	void		ResetSendTargetName() { memset(m_SendName, 0, sizeof( TCHAR) * 13 );	};
	void		SetUpdeteUnit(_Unit* unit)  { m_UpdateUnit = *unit; };
	_Unit*		GetUpdateUnit() { return &m_UpdateUnit;	};
	int			GetManageCount() { return m_ManagerCount;	};
	int			GetTotalMemberCount() { return m_OrganizationBaseInfo.m_usCurMemberNum;	};

	void		ProcessPopUp(); ///< ������ �޴� ���� - ���� �̻�޸�
	void		ClosePopUpMenu();

	// Network Send ���� �Լ� ----------------------------=		
	void		GrantUnit();	///< ������� �ο� - ������ ��� 	
	void		GrantCancleUnit();	///< ������� ��Ż - ������ ��� 	
	void		TrustUnit();	///< ���� ���� - ������ ���	
	void		PrugeUnit();	///< �߹� - ������ ��� 	
	void		LeaveOrganization();	///< Ż�� - ������ ��� Ż�� �Ұ���(�ܿ��� 3���ϸ� ������ �ڵ� ��ü)
	// ---------------------------------------------------=

	// ���� �Լ� -----------------------------------------=
	/// ������ ���� ���� ( m_SortLevelFlag ����/���� ���� )
	void		LineUpLevel();
	void		SortLessLevelUp( int unitindex, BOOL sortflag );
	/// ������ ���� ���� ( m_SortZoneFlag ����/���� ���� )
	void		LineUpZone();
	void		SortLessZone( int unitindex, BOOL sortflag );
	/// ���ӿ��ο� ���� ���� ( m_SortOnlineFlag ����/���� ���� )
	void		LineUpOnline();
	void		SortLessOnline( int unitindex, BOOL sortflag );

	void		ClickSortButton( void );	///< ���ĸ���Ʈ View On/Off	
	void		SelectSortType( void );		///< ���� ��ư Ŭ���� Ÿ�Կ� ���� ó��
	// ---------------------------------------------------=

	void		CalculateNoticeTextength( void );	///< ������ ���� ���ڼ� ���	
	void		SelectNoticeMode( _XNoticeMode mode);	///< ������ ������� ���� - �б�, ����, ����
	
	BOOL		ProcessWheelMessage( short zDelta );
	
	void		CopySendNoticeText( void );	///< �ۼ��� �������� ������ Send	
	void		SetNoticeText(TCHAR* text, bool modifymode = false );	///< �������� ���� (�������� �޾��� �� , ���� ���� �� )
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//											��ȸ ���� �Լ�														//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
#ifdef _XTS_ALIANCE_20061018	
	void		SetAlienceButton(void);			// ������ ���� ����� ǥ���ϴ� ��ư�� �����.
	void		ResetAlienceInfo(void);
#endif
	
	void		SetCastleData(void);
	void		ShowCastleTooltip(void);
	void		HideCastleTooltip(void);
	void		ResetCastleData(void);
	
	// ����� ���� �Լ�
	void		DrawOwnerInfo(void);
	void		DrawCBOrgTotalRecord(void);
	void		SetRecordListItem(int count, _PerOrgRecord* record);
	void		ResetRecordData(void);

#ifdef ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA
	BOOL		IsMember( LPTSTR charactername );
#endif

};

#endif // !defined(AFX_XWINDOW_GROUP_H__452F2A05_6E59_47A7_94DD_C88636A496DA__INCLUDED_)

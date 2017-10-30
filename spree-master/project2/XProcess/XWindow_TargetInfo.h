// XWindow_TargetInfo.h: interface for the _XWindow_TargetInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TARGETINFO_H__E352666A_2C36_4C82_8BC0_703D946AF153__INCLUDED_)
#define AFX_XWINDOW_TARGETINFO_H__E352666A_2C36_4C82_8BC0_703D946AF153__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief ���� ���� ���� â
 * ��������� ���� ���� ���� ���� 
 * �������� ���� �Ͽ� �� �� ����
*/

enum TargetInfoMode {
	tm_none = 0,
	tm_simple_info = 2,	///< ĳ���� ���� ����
	tm_items_info,		///< ������ ����
	tm_ability_info,	///< �ɷ�ġ ����
	tm_all_info,		///< ��ü ����
};

/// ���� ����
struct _TARGETINFO
{
	char	cTargetName[128] ;			///< ��� �̸� 
	
	short	sInnerLevel ;				///< ����ܰ�
	char	cMoonpa ;					///< ����
	char	cClass ;					///< ��å	
	char	cClassGrade ;				///< ���� �ܰ�

	short	sConstitution ;				///< �ǰ�
	short	sZen ;						///< ����
	short	sIntelligence ;				///< ����
	short	sDexterity ;				///< ��ø
	short	sStr ;						///< �ٷ�

	/// ������ ���� ��ġ
	enum _GET_ON_ITEMS_SLOT
	{
		pos_amor=0,         // 0
		pos_pants,			// 1
		pos_adae,			// 2
		pos_shoes,			// 3
		pos_cap,			// 4
		pos_gap_upper,		// 5
		pos_gap_lower,		// 6
		pos_backpack,		// 7
		pos_weapon_1,		// 8
		pos_weapon_2,		// 9
		get_on_max
	};
	_XUserItem getOnItems[get_on_max] ;
	
	void reset()
	{
		memset(cTargetName, 0, sizeof(TCHAR)*128);

		sInnerLevel		= 0;			
		cMoonpa			= 0;			
		cClass			= 0;			
		cClassGrade		= 0;			

		sConstitution	= 0;			
		sZen			= 0;			
		sIntelligence	= 0;			
		sDexterity		= 0;			
		sStr			= 0;			
		for( int i = 0; i < get_on_max; i++)
			getOnItems[i].Reset();
	} ;
};

class _XWindow_TargetInfo : public _XWindow  
{
private:
	_XStatic				m_BorderTile;

	int						m_SelectedSlotIndex;			
	_XImageStatic			m_EquipedItemEdgeIcon[5];	///< ���� �������� ���� ��ȭ �̹���	

public :
	TargetInfoMode			m_TargetInfoMode;				///< ���� �������� ���
	TargetInfoMode			m_TargetInfoModefromRequest;	///< ���� ��û�� �������� ��� 
	TargetInfoMode			m_TargetInfoModeToRequest;		///< ������ ������ ��û�� �������� ���
	_TARGETINFO				m_SelectedTargetInfo;
	TCHAR					m_SendName[128];					
	TCHAR					m_SendTempName[128];		///< m_SendName �� ���� ��� �����ϴ� ����
	int						m_PtoPLevelDef;			///< ������� ���� ���� �ܰ�
	
public:
	_XWindow_TargetInfo();
	virtual ~_XWindow_TargetInfo();

	BOOL		Initialize( void );
	void		DestroyWindow( void );

	void		ShowWindow(BOOL show);

	void		Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL		Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void		DrawEquippedItemList(void);
	int			SearchSlotRegion(  RECT* pRect  );

	void		MoveWindow(int X, int Y);
	void		DrawItemTooltip(int slotnumber, RECT iconrect);
};

#endif // !defined(AFX_XWINDOW_TARGETINFO_H__E352666A_2C36_4C82_8BC0_703D946AF153__INCLUDED_)

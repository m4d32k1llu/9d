// XWindow_State.h: interface for the _XWindow_State class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_STATE_H__78911F8E_3C65_4408_A8B5_C79C8845A6CD__INCLUDED_)
#define AFX_XWINDOW_STATE_H__78911F8E_3C65_4408_A8B5_C79C8845A6CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable: 4786)
#include <list>

#include "XKernel.h"

using namespace std;

enum _XStateType
{
	_XST_NONE = 0,
	_XST_EXTERNAL,			// �ܻ�
	_XST_INTERNAL,			// ����
	_XST_ITEM,				// ������ ������ ����
	_XST_SKILL,				// ����ȿ�� - ��ų
	_XST_EFFECT,			// ����ȿ�� - �����
	_XST_PASSIVE,			// ����ȿ�� - �нú�
	_XST_INTOXICATION,		// ���� �ߵ�
	_XST_STUDYBOOK,			// ������
	_XST_NICKBUFF,			// ��ȣ ����
	_XST_PEMODE,			// ���� ���� ������
	_XST_SHOUT,				// ��ġ�� ������
	_XST_EXTRASKILL,		// �������� ���� ��
	_XST_SPECIALBUFF		// ����� ����
};

union _XSlot
{
	struct  
	{
		short	beforeexternal;
		short	afterexternal;
		short	useitemid;
		short	decreaserate;
	}slot_external;

	struct  
	{
		short	beforeinternal;
		short	afterinternal;
		short	useitemid;
		FLOAT	decreaserate;
	}slot_internal;

	struct 
	{
		char	ctype;
		short	id;
		char	invennumber;
		DWORD	starttime;
		bool	bRedicon;
	}slot_item;

	struct  
	{
		short	id;
		char	level;
		char	slotindex;	// 08.08.07 �߰� //Author:�ڼ���
	}slot_skill;

	struct  
	{
		short	id;
		char	slotindex;
	}slot_effect;
	
	struct 
	{
		char	ctype;
		short	id;		
		DWORD	starttime;
		DWORD	lefttime;
	}slot_studybook;
};

struct _XStateSlot
{
	_XStateType		type;
	_XImageStatic*	icon;

	bool			bSign;
	DWORD			showtime;

	_XSlot			slot;

	_XStateSlot()
	{
		type		= _XST_NONE;
		icon		= NULL;
		bSign		= false;
		showtime	= 0;
		memset(&slot, 0, sizeof(_XSlot));
	}
};

class _XWindow_State : public _XWindow
{
private :
	list <_XStateSlot*>		m_PlayerStateList;
	list <_XStateSlot*>		m_BuffStateList;
	list <_XStateSlot*>		m_HostileStateList;

	_XImageStatic*			m_BarImage;

	DWORD					m_PreProcessTime;

	// ��ų ��� ����( ����, ���� )
	char					m_BuffCancleindex;
	char					m_BuffCancleMode;

public:
	_XWindow_State();
	virtual ~_XWindow_State();

	BOOL			Initailize(void);
	void			DestroyWindow(void);

	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void			InsertStateListFromExIn(_XStateType type, short beforevalue, short aftervalue);	// external and internal
	void			InsertStateListFromOther(_XStateType type, short id, char ctype = 0, char invennumber = 0);			// item, skill, effect

	void			InsertPlayerStateList(_XStateType type, short id, char ctype , char invennumber, int index = 0);	// class ����/item ������ �ʿ��� ���

	void			InsertBuffStateList(_XStateType type, short id, char slotindex, int level = 0);
	void			InsertHostileStateList(_XStateType type, short id, char slotindex, int level = 0);
	void			InsertIntoxication(_XStateType type, short id);	// ���� �ߵ�
	void			InsertNicknameBuff(_XStateType type, short id);	// ��ȣ ����	
	void			InsertStudyBookBuff(_XStateType type, short id, int lefttime ); // ������ ����
	void			InsertPEModeIcon(_XStateType type, short id);	// ���� ����

	void			UpdatePlayerStateList(_XStateType type, short beforevalue, short aftervalue);
	BOOL			UpdatePlayerStateList(_XStateType type, short id, char ctype, char oldinvennumber, char newinvennumber);

	void			DeleteStateList(_XStateType type, short id, char ctype = 0, char invennumber = 0);
	void			DeletePlayerStateList(_XStateType type, short id, char ctype = 0, char invennumber = 0);

	BOOL			DeleteBuffStateList(_XStateType type, short id, char ctype = 0);
	BOOL			DeleteHostileStateList(_XStateType type, short id, char ctype = 0);
	void			DeleteElixirState(_XStateType type, char ctype=0);
	void			DeleteIntoxication(_XStateType type, short id);
	void			DeleteNicknameBuff(void);
	void			DeleteStudyBookBuff(_XStateType type, short id);
	void			DeletePEModeIcon(void);

	void			DeleteAllStateList(void);
	void			DeleteAllBuffStateList(void);
	void			DeleteAllHostileStateList(void);

	void			DeleteAllSkill(void);
	void			DeleteAllEffect(void);

	BOOL			FindUsingItemFromID(_XStateType type, short id, char ctype);
	BOOL			FindUsingItemFromSecondType(_XStateType type, char ctype, char secondtype);
	
	BOOL			FindPlayerStateList(_XStateType type, short id, char ctype, char invennumber, int index = 0);

	void			DrawStateList(void);
	void			DrawStateListTooltip(void);

	void			ShowWindow(BOOL show);

	BOOL			GetIconRect(short effectid, int& resourceindex, RECT& rect, bool smallicon = false);
	BOOL			GetIconRect_NIck(short effectid, int& resourceindex, RECT& rect, bool smallicon = false);
	void			SetRedLayer(_XStateType type, short id, char ctype, char invennumber = 0);
	void			SetIconWarning(_XStateType type, short id, char ctype);

	void			ClickSkillAction(void);
	void			PrintSystemMessage(int index);
	
	BOOL			FindPlayerStateList(_XStateType type, short id); // ����ȿ���� �����ϴ��� �˻�( Functional Object ���ǽĿ� ���) 
	BOOL			FindBuffStateList(_XStateType type, short id); // ����ȿ���� �����ϴ��� �˻�( Functional Object ���ǽĿ� ���) 
	BOOL			FindHostileStateList(_XStateType type, short id); // ����ȿ���� �����ϴ��� �˻�( Functional Object ���ǽĿ� ���) 

};

#endif // !defined(AFX_XWINDOW_STATE_H__78911F8E_3C65_4408_A8B5_C79C8845A6CD__INCLUDED_)

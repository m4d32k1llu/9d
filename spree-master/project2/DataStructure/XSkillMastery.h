// XSkillMastery.h: interface for the _XSkillMastery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSKILLMASTERY_H__6B2A617D_1418_4CC3_85D6_0D27D40286A4__INCLUDED_)
#define AFX_XSKILLMASTERY_H__6B2A617D_1418_4CC3_85D6_0D27D40286A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#pragma warning (disable : 4786)
#include <vector>
using namespace std;

class _XWindow_MasterySkill;

// ======================================================
// 
// 2�� ���� ���� ������ Ȯ�븦 ���� structure ����
//
// _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI �� define �Ǿ����� ���, 
// ������ g_SkillMasteryDisplayProperty_xx_xx ������� �ʰ� �Ʒ� class ����Ѵ�.
//
// MasteryDisplayTable�� ��ũ��Ʈ�� ������� �о �����ϰ�, 
// Lookup table�� �����Ͽ� ���ϴ� ���� ������ �Ѵ�.
//
// ======================================================= 

struct _XMasteryDisplayTableInfo 
{
	int skillid;
	int needprev;

	_XMasteryDisplayTableInfo() : skillid(0), needprev(0)
	{
	}

};

struct _XMasteryLookupTableInfo
{
	int	index;	// SkillMasteryDisplayProperty�� index
	int clan;
	int cClass;
	int grade;
	int	page;

	_XMasteryLookupTableInfo() : index(0), clan(0), cClass(0), grade(0), page(0)
	{
	}

};

struct _XMasteryNameTableInfo
{
	int index;
	int strid;

	_XMasteryNameTableInfo() : index(0), strid(0)
	{
	}
};

typedef vector <_XMasteryDisplayTableInfo*> _vec_MasteryDisplayTableInfo;
typedef vector <_XMasteryNameTableInfo*> _vec_MasteryName;
typedef vector <_XMasteryLookupTableInfo*> _vec_MasteryLookupTableInfo;

class _XSkillMastery  
{

private :
	_vec_MasteryDisplayTableInfo	m_vecMasteryDisplayTable;
	_vec_MasteryName				m_vecMasteryNameId;
	_vec_MasteryLookupTableInfo		m_vecMasteryLookupTable;

public:
	_XSkillMastery();
	virtual ~_XSkillMastery();

	BOOL	LoadSkillMasteryDisplayTable();

	int		GetMasteryDisplayTableIndex(int clan, int cClass, int grade, int page);
	int		GetMasteryNameId(int index);

	int		GetSkillIDFromMasteryDisplayTable(int index);
	int		GetNeedprevFromMasteryDisplayTable(int index);
};

#endif // !defined(AFX_XSKILLMASTERY_H__6B2A617D_1418_4CC3_85D6_0D27D40286A4__INCLUDED_)

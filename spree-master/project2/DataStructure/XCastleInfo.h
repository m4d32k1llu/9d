// XCastleInfo.h: interface for the _XCastleInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCASTLEINFO_H__2F0EF7B9_2FB5_4DFE_8B6C_302E55578209__INCLUDED_)
#define AFX_XCASTLEINFO_H__2F0EF7B9_2FB5_4DFE_8B6C_302E55578209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XCastleBaseType.h"
#include "XNetwork.h"

#include <vector>

// �������̽� ��
class _XCastleInfo  
{
public :
	int			m_nMapId;

	BOOL		m_bMine;
//	int			m_nPropertyDate;		// ������
	int			m_nStep;				// ��� �ܰ�
	int			m_nTax;
	DWORD		m_dwMoney;				// ��� â�� �ִ� ��

	int			m_nCurMatchCount;		// ���� ������� �������ΰ�?	0/1/2
	int			m_nMatchState;			// ���� 0:������  1:������

	_XTimeUnit	m_tPropertyDate;		// ������

	_XCastle_ChallengeSchedule	m_tWarSchedule[3];
	_XCastle_OrgInfo			m_tOwner;
	vector <_XCastle_OrgInfo*>	m_vecChallengers;

public:
	_XCastleInfo();
	virtual ~_XCastleInfo();

	void	Release();

	void SetScheduleList(_sCastleInfo* castleinfo, DWORD money = 0);
	void ResetChallengeList(void);
	void SetChallengeList(__cb_client_challenge_info* info);
	void ChangeTime(int nth, _sND_TimeUnit* time);

};

#endif // !defined(AFX_XCASTLEINFO_H__2F0EF7B9_2FB5_4DFE_8B6C_302E55578209__INCLUDED_)

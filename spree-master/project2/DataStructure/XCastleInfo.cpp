// XCastleInfo.cpp: implementation of the _XCastleInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XCastleInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XCastleInfo::_XCastleInfo()
{
	m_nMapId		= 0;
	m_bMine			= FALSE;
	m_nStep			= 0;
	m_nTax			= 0;
	m_dwMoney		= 0;
//	m_nNextWar		= 1;
}

_XCastleInfo::~_XCastleInfo()
{
	Release();
}

void _XCastleInfo::Release(void)
{
	ResetChallengeList();
}
void _XCastleInfo::SetScheduleList(_sCastleInfo* castleinfo, DWORD money)
{
	// ���� ������ ����
	m_tOwner.nOrIndex = castleinfo->iOrIndex;			// ��/ȸ index
	m_tOwner.nType = castleinfo->ucType;				// �� orȸ
	m_tOwner.nMemberCnt = castleinfo->usMemberCount;	// ���

	strncpy(m_tOwner.szOrgName, castleinfo->cOrName, _XDEF_ORGANIZATIONNAMELENGTH);	// ��/ȸ �̸�
	strncpy(m_tOwner.szMasterName, castleinfo->cMasterName, 12);	// ���� �̸�
	m_tOwner.nGroupOfMaster = castleinfo->cMasterMonpa;				// ������ ����

	// ��� ����
	m_nStep = castleinfo->ucLevel;

	// ������
	m_nTax = castleinfo->uiTotalMoney;
	m_dwMoney = castleinfo->uiPartMoney;

	// ������
	memcpy(&m_tPropertyDate, &castleinfo->timeUnit, sizeof(_XTimeUnit));

	// 3���� ���� ������
	memcpy(&m_tWarSchedule[0].tDate, &castleinfo->Schedule[0], sizeof(_XTimeUnit));
	memcpy(&m_tWarSchedule[1].tDate, &castleinfo->Schedule[1], sizeof(_XTimeUnit));
	memcpy(&m_tWarSchedule[2].tDate, &castleinfo->Schedule[2], sizeof(_XTimeUnit));

	m_nCurMatchCount = castleinfo->ucCurMatchCount;
	m_nMatchState = castleinfo->ucMatchState;
}

void _XCastleInfo::ResetChallengeList(void)
{
	vector <_XCastle_OrgInfo*>::iterator iter_orginfo;
	for(iter_orginfo = m_vecChallengers.begin() ; iter_orginfo != m_vecChallengers.end() ; ++iter_orginfo)
	{
		_XCastle_OrgInfo* orginfo = (*iter_orginfo);
		if(orginfo)
			SAFE_DELETE(orginfo);
	}
	m_vecChallengers.clear();
}

void _XCastleInfo::SetChallengeList(__cb_client_challenge_info* info)
{
	_XCastle_OrgInfo* orginfo = new _XCastle_OrgInfo;
	orginfo->nMemberCnt = info->usMemberCount;
	orginfo->nType = info->ucType;
	orginfo->nGroupOfMaster = info->ucMoonpa;
	strncpy(orginfo->szOrgName, info->cGroupName, _XDEF_ORGANIZATIONNAMELENGTH);
	strncpy(orginfo->szMasterName, info->cMasterName, 13);

	m_vecChallengers.push_back(orginfo);
}


void _XCastleInfo::ChangeTime(int nth, _sND_TimeUnit* time)
{
	if(nth >= 0 && nth < 3)
	{
		memcpy(&m_tWarSchedule[nth].tDate, time, sizeof(_XTimeUnit));
	}
}
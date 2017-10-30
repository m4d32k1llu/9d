// XCastleScheduler.h: interface for the _XCastleScheduler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCASTLESCHEDULER_H__7558B243_8CBB_41ED_8D25_BBAABCE6489B__INCLUDED_)
#define AFX_XCASTLESCHEDULER_H__7558B243_8CBB_41ED_8D25_BBAABCE6489B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XCastleBaseType.h"
#include "XKernel.h"

#include <vector>
#include <map>

extern BOOL GetCommand(char* cmd, FILE* fileptr, bool bnumber = false);

class _XCastleScheduler  
{
	// script���� ���� ����� ����
public :
	int				m_nMapId;				// map id
	int				m_nMapNameStrindex;		// map name index
	int				m_nMobMasterNameIndex;	// ���� �����϶� �̸� ǥ��
	
	_XCastle_Tax	m_tDefaultTax;			// ���� ����
	_XCastle_Tax	m_tCastleTax[5];		// ��� ���� (display��)

	int				m_nChallengeMoney[5];	// ���� ���
	float			m_fChallengeTax;		// ���� ��� ����

	int				m_nThreatenTime;		// ���� ���� �ð� (��)
	int				m_nThreatenDelayTime;	// ���� ���� ������ �ð� (��)

	int				m_nDeadMaxCnt;			// ȥ������ �ִ� �ݺ� Ƚ��
	int				m_nMaxShoutCnt;			// ��ġ�� �ִ� Ƚ��
	
	TCHAR*			m_szMinimapName;		// �̴ϸ� �̸�
	int				m_nMinimapPosition;		// �̴ϸ� Position

	map <int, TCHAR*>		m_mThreatenBGM;
	map <int, int>			m_mThreatenSE;

	_XCastle_OpenTime		m_tDefaultOpenTime[3];	// default  ���� �ð�

	vector <_XCastle_OpenTime*> m_vecChallengeTime;

	vector <_XCastle_DiePenalty*>	m_vecDiePenalty;

	map <int, _XCastle_Scheduler*>	m_mCastleScheduler;


public:
	_XCastleScheduler();
	virtual ~_XCastleScheduler();

	void			Release(void);

//	BOOL			GetCommand(char* cmd, FILE* fileptr, bool bnumber = false);
	BOOL			Load(TCHAR* filename);

	BOOL			ParseManorLevel(FILE* fp);
	BOOL			ParseManorOpenTimer(FILE* fp);
	BOOL			ParseManorDiePenalty(FILE* fp);
	BOOL			ParseThreatenBGM(FILE* fp);
	BOOL			ParseChallengeMoney(FILE* fp);
	BOOL			ParseManorChallengeTime(FILE* fp);

	BOOL			ParseEventTime(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseAlarmMsg(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseCharAni(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseEffect(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseBuff(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseState(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseMainBGM(FILE* fp, _XCastle_Scheduler* pScheduler, BOOL bGroup = FALSE);
	BOOL			ParseVictoryType(FILE* fp);
	BOOL			ParseMainSE(FILE* fp, _XCastle_Scheduler* pScheduler);
	
	BOOL			ParseSkipInfo(FILE* fp);

	int				GetTargetType(char* arg);

};

#endif // !defined(AFX_XCASTLESCHEDULER_H__7558B243_8CBB_41ED_8D25_BBAABCE6489B__INCLUDED_)

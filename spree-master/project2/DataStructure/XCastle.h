// XCastle.h: interface for the _XCastle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCASTLE_H__1A97C18F_D4A0_4ECF_AE71_C9AF049510E2__INCLUDED_)
#define AFX_XCASTLE_H__1A97C18F_D4A0_4ECF_AE71_C9AF049510E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XNetwork.h"

#include <vector>
#include <map>

// ����
struct _XCastle_Tax	
{
	float		fCastleRegion;
	float		fCommonRegion;

	_XCastle_Tax()
	{
		fCastleRegion = 0.f;
		fCommonRegion = 0.f;
	}
};

// ���� �ð�
struct _XCastle_OpenTime
{
	DWORD		dwHour : 8;
	DWORD		dwMinute : 8;
	DWORD		dwProgressTime : 16;

	_XCastle_OpenTime()
	{
		dwHour = dwMinute = dwProgressTime = 0;
	}
};

// ����� state 
enum _XCastle_State
{
	_XEN_CB_NONE = 0,
	_XEN_CB_START,				// ����
	_XEN_CB_READY = 10,			// ����� ���� ��
	_XEN_CB_PROGRESS = 20,		// ������
	_XEN_CB_END = 30			// ��
}; 

// 
struct _XCastle_Scheduler
{
	int			nWhen;
	int			nTime;

	_XCastle_State	enWarState;

	DWORD		dwAlarmMsgType : 16;
	DWORD		dwAlarmMsgTarget : 16;
	int			nTextIndex;

	DWORD		dwShout : 8;
	DWORD		dwGateState : 8;
	DWORD		dwThreaten : 8;
	DWORD		dwLightFoot : 8;

	DWORD		dwAnimationTarget : 8;
	DWORD		dwEffectTarget : 8;
	DWORD		dwBuffTarget : 8;
	DWORD		Reserved : 8;

	int			nAnimationIndex_male;
	int			nAnimationIndex_female;
	int			nEffectId;
	int			nSEId;
	int			nBuffId;
	int			nBuffTime;

	int			nBGMStartTime;
	int			nBGMEndTime;
	BOOL		bLoop;

	TCHAR*		szBGMFileName;

	_XCastle_Scheduler()
	{
		nWhen	= 0;
		nTime	= 0;
		
		dwAlarmMsgType		= 0;
		dwAlarmMsgTarget	= 0;
		nTextIndex			= 0;

		dwShout		= 0;
		dwGateState = 0;
		dwThreaten  = 0;
		dwLightFoot = 0;

		dwAnimationTarget	= 0;
		dwEffectTarget		= 0;
		dwBuffTarget		= 0;
		Reserved			= 0;

		nAnimationIndex_male	= 0;
		nAnimationIndex_female	= 0;

		nEffectId			= 0;
		nSEId				= 0;
		nBuffId				= 0;
		nBuffTime			= 0;

		nBGMStartTime	= 0;
		nBGMEndTime		= 0;
		bLoop			= FALSE;

		szBGMFileName	= NULL;
	}
};

struct _XTimeUnit
{
	DWORD		dwYear : 5;		// base 2000
	DWORD		dwMonth : 4;
	DWORD		dwDay : 6;
	DWORD		dwHour : 5;
	DWORD		dwMinute : 6;
	DWORD		dwSecond : 6;

	_XTimeUnit()
	{
		dwYear = dwMonth = dwDay = dwHour = dwMinute = dwSecond = 0;
	}
};

// ���� ����
struct _XCastle_ChallengeSchedule
{
	_XTimeUnit	tDate;
	BOOL		bValid;

	_XCastle_ChallengeSchedule()
	{
		bValid = FALSE;
	}
};

// ������ �� ������ ����
struct _XCastle_OrgInfo
{
	int			nMemberCnt;
	
	int			nOrIndex;
	int			nType;					// 0(��)  1(ȸ)
	int			nGroupOfMaster;
	TCHAR		szOrgName[_XDEF_ORGANIZATIONNAMELENGTH+1];
	TCHAR		szMasterName[13];

	_XCastle_OrgInfo()
	{
		nMemberCnt	= 0;
		nOrIndex	= 0;
		nType		= 0;
		nGroupOfMaster = 0;
		memset(szOrgName, 0, sizeof(szOrgName));
		memset(szMasterName, 0, sizeof(szMasterName));
	}
};

// ��Ȱ �г�Ƽ
struct _XCastle_DiePenalty
{
	int			nMinLevel;
	int			nMaxLevel;
	int			nAttackerPenalty;
	int			nDefenderPenalty;

	_XCastle_DiePenalty()
	{
		nMinLevel	= 0;
		nMaxLevel	= 0;
		nAttackerPenalty	= 0;
		nDefenderPenalty	= 0;
	}
};


///////////////////////////////////////////////////////////////////////////////////////
// ��� ���� - interface ��
class _XCastle  
{
public :
	int			m_nMapId;

	BOOL		m_bMine;
	int			m_nPropertyDate;		// ������
	int			m_nStep;				// ��� �ܰ�
	int			m_nTax;
	int			m_nMoney;

	int			m_nNextWar;				// ���� ������� �������ΰ�?

	_XCastle_ChallengeSchedule	m_tWarSchedule[3];
	_XCastle_OrgInfo			m_tOwner;
	vector <_XCastle_OrgInfo*>	m_vecChallengers;

public:
	_XCastle();
	virtual ~_XCastle();

	void SetScheduleList(_sCastleInfo* castleinfo, BOOL bFixed);
	void ResetChallengeList(void);
	void SetAllChallengeList(int nth, __cb_client_challenge_info* info);
	void SetChallengeList(int nth, int index, __cb_client_challenge_info* info);
	void ChangeTime(int nth, _sND_TimeUnit* time);

};

/////////////////////////////////////////////////////////////////////////////////////////
// ��� ���� information
class _XCastleCommonInfo
{
	// ���� �������� ����� ����
public :
	_XCastle_State	m_enWarState;			// ����� ���� ��Ȳ

	DWORD			m_dwTimeFromServer;		// �������� ��Ŷ ������ �ð� (ready, start, end)

	BOOL			m_bMsgDone;
	BOOL			m_bBGMDone;
	BOOL			m_bSEDone;
	BOOL			m_bShoutDone;
	BOOL			m_bGateDone;
	BOOL			m_bThreatenDone;
	BOOL			m_bLightFootDone;
	BOOL			m_bAnimationDone;
	BOOL			m_bEffectDone;
	BOOL			m_bBuffDone;

	// script���� ���� ����� ����
public :
	int				m_nMapId;				// map id
	
	_XCastle_Tax	m_tDefaultTax;			// ���� ����
	_XCastle_Tax	m_tCastleTax[5];		// ��� ����

	int				m_nChallengeMoney;		// ���� ���
	float			m_fChallengeTax;		// ���� ��� ����

	int				m_nThreatenTime;		// ���� ���� �ð� (��)

	int				m_nDeadMaxCnt;			// ȥ������ �ִ� �ݺ� Ƚ��

	map <int, TCHAR*>		m_mThreatenBGM;
	map <int, int>			m_mThreatenSE;

	_XCastle_OpenTime		m_tDefaultOpenTime[3];	// default  ���� �ð�

	vector <_XCastle_DiePenalty*>	m_vecDiePenalty;

	map <int, _XCastle_Scheduler*>	m_mCastleScheduler;

public :
	_XCastleCommonInfo();
	virtual ~_XCastleCommonInfo();

	void			Release(void);

	void			Reset(void);
	void			ClearState(void);

	void			Process(void);

	void			ProcessAlarmMsg(void);
	void			ProcessAnimation(void);
	void			ProcessVisualEffect(void);

	BOOL			GetCommand(char* cmd, FILE* fileptr, bool bnumber = false);
	BOOL			Load(int mapid);

	int				GetPenaltyTime(DWORD level);

	BOOL			ParseManorLevel(FILE* fp);
	BOOL			ParseManorOpenTimer(FILE* fp);
	BOOL			ParseManorDiePenalty(FILE* fp);
	BOOL			ParseThreatenBGM(FILE* fp);

	BOOL			ParseEventTime(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseAlarmMsg(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseCharAni(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseEffect(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseBuff(FILE* fp, _XCastle_Scheduler* pScheduler);
	BOOL			ParseState(FILE* fp, _XCastle_Scheduler* pScheduler);
	
	BOOL			ParseSkipInfo(FILE* fp);

	int				GetTargetType(char* arg);
};


#endif // !defined(AFX_XCASTLE_H__1A97C18F_D4A0_4ECF_AE71_C9AF049510E2__INCLUDED_)

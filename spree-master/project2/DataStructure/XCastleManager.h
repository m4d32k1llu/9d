// XCastleManager.h: interface for the _XCastleManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCASTLEMANAGER_H__EE88715B_C961_4D62_9892_2966834B4355__INCLUDED_)
#define AFX_XCASTLEMANAGER_H__EE88715B_C961_4D62_9892_2966834B4355__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XCastleInfo.h"
#include "XCastleScheduler.h"

#include <map>

class _XCastleManager  
{
private :
	// ��ü �� : ��ũ��Ʈ ����
	map <int, TCHAR*>				m_mManorScriptName;

	map <int, _XCastleInfo*>		m_mCastleInfo;
	map <int, _XCastleScheduler*>	m_mCBScheduler;
	
	// ���� �������� ����� ����
public :
	_XCastleScheduler*		m_pScheduler;	// ���� ���� ����� ����
	_XCastle_State			m_enWarState;	// ����� ���� ��Ȳ

	int				m_nMapId;

	DWORD			m_dwTimeFromServer;		// �������� ��Ŷ ������ �ð� (ready, start, end)
	DWORD			m_dwThreatenStartTime;	// ���� ���� ���� (���� ���)
	FLOAT			m_fThreatenPercentage;
	BOOL			m_bSuccessThreaten;
	int				m_nProgress;
	
	DWORD			m_dwThreatenRequestTime;	// ���� req ���� �ð� - error�� �͵� �ð� üũ�ؼ� packet �������� ������ �� ���´�

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
	
	DWORD			m_dwBGMStartTime;				// BGM ������ �� �ð� - Loop�� ����
	DWORD			m_dwStateTotalBGMStartTime;		// BGM ������ �� �ð� - Loop ����

	DWORD			m_dwSEStartTime;				// SE ������ �� �ð� 
	
	int				m_nSavePointFailResult;	// ����� �� ���� Ǯ�� fail ����
											// 1: ����� ������ �ƴ�
											// 2: �������� �ƴ�
											// 3: �������� �ƴ�
	
	BOOL			m_bDrawOwnerMessage;		// ��� ���� �ȳ� �޽��� ���
	DWORD			m_dwOwnerMessageStartTime;	// ��� ���� �ȳ� �޽��� ��� ���� �ð�

public:
	_XCastleManager();
	virtual ~_XCastleManager();

	void			Release();
	void			ReleaseCastleInfo(void);

	void			Reset(void);
	void			ClearState(void);

	void			Draw(void);
	void			Process(void);

	void			ProcessAlarmMsg(void);
	void			ProcessAnimation(void);
	void			ProcessVisualEffect(void);
	void			ProcessBGM(void);
	void			ProcessSE(void);
	
	BOOL			GetTargetType(int index);

	BOOL			Load(void);
	BOOL			LoadCastleScheduler(void);
	BOOL			ParseManorMapId(FILE* fp);
	BOOL			ParseSkipInfo(FILE* fp);

	void			SetCurrentScheduler(int mapid);

	_XCastleInfo*	GetCastleInfo(int mapid);
	int				GetCastleNameTextId(int mapid);

	// �Ʒ� get �Լ����� �������� ����� ���� �̹Ƿ� mapid �ʿ����
	int				GetDeadMaxCount();
	int				GetPenaltyTime(DWORD level);
	DWORD			GetThreatenTime();
	DWORD			GetThreatenDelayTime();
	int				GetShoutMaxCount();

	DWORD			GetGateState();
	DWORD			GetThreatenState();
	DWORD			GetLightFootState();
	
	int				GetMinimapPosition();

	// ���� ����� �������� ��������� ��������Ƿ� mapid �ʿ�
	int				GetChallengeMoney(int mapid, int index);
	FLOAT			GetDefaultTax(int mapid, int step);
	FLOAT			GetAdditiveTax(int mapid, int step);
	void			GetDefaultOpenTime(int mapid, int& first, int& second, int& third);
	int				GetMobMasterNameIndex(int mapid);
	void			GetChallengeTime(int mapid, int* hour, int* minute, int& count);
	
	void			SetScheduleList(int mapid, _sCastleInfo* castleinfo, DWORD money = 0);
	void			SetChallengeList(int mapid, __cb_client_challenge_info* info);
	void			ResetChallengeList(int mapid);
	void			ChangeTime(int mapid, int nth, _sND_TimeUnit* time);

	void			SetCastleTaxMoney(int mapid, DWORD totalmoney, DWORD partmoney);
	DWORD			GetCastleTaxMoney(int mapid);

	void			SetDrawOwnerMessage(BOOL draw)			{	m_bDrawOwnerMessage = draw;		};
	void			SetOwnerMessageStartTime(DWORD time)	{	m_dwOwnerMessageStartTime = time;	};

};

#endif // !defined(AFX_XCASTLEMANAGER_H__EE88715B_C961_4D62_9892_2966834B4355__INCLUDED_)

#pragma once

#ifdef _PD_CASTLE_STEP_2_

#include <map>
#include "../Global/Global_struct.h"


using namespace std;
class COrgRecord;
struct CMSG_RECORD_UPDATE_GS_DS;

typedef	map<int,COrgRecord*>	MapOrgRecord;

//////////////////////////////////////////////////////////////////////////
///	\class	CRecordOfCombat
/// \brief  ����� ����� ���� �ϴ� Ŭ���� - ���� ����, �����ܺ� ����, ������������ 
class CRecordOfCombat 
{
public:
	CRecordOfCombat(){}
	~CRecordOfCombat(){}
	void Create();
	void Destory();

public:
	void LockRecord()	{ EnterCriticalSection( &m_cs ); }
	void UnLockRecord()	{ LeaveCriticalSection( &m_cs ); }

	bool			GetUseRecord(){return m_bUse_record;}
	void			SetUseRecord(bool  bUsed){m_bUse_record = bUsed;}
	int				GetManorIndex(){return m_sManorIndex;}
	void			SetManorIndex(int  iIndex){m_sManorIndex = iIndex;}
	int*			GetCombatingOrg(){return m_combating_org;}
	void			SetCombatingOrg(int *combatingOrg)
	{
		memcpy(m_combating_org,combatingOrg,sizeof(int)*MAX_ORG_COUNT);
	}
	
	bool	Initialize();
	/// \brief	����� ���۵Ǹ� ����,Ÿ�ܰ������� �ʱ�ȭ 
	///         �����ϴ� �ܸ� �ʱ�ȭ�̹Ƿ� �� �ε����� �޾� �ʱ�ȭ.
	bool	ResetWarRecord();
	
	/// \brief	�ܺ� ����� ���� ���� 
	bool	LoadCombatRecord(int iAllyIndex,int iManorIndex,int iEntryCount,int iWinCount,int iLoseCount,DBTIMESTAMP combatDateDB);
	
	/// \brief	�� ������ ���� ���� 
	bool	LoadVsOrgRecord(int iAllyIndex,int iManorIndex,int iVSIndex,int iWinCount,int iLoseCount,int iVSAllyIndex,u_char* name);

	/// \brief �ܿ� ����� ������ ���� Total ������ �����.
	void	CombineManorRecord();
	/// \brief ��Ŷ�� ������ ä���ش�.
	void	FillPacket(int orgIndex,CMSG_RECORD_INFORMATION_DS_GS& info);
	/// \brief ��Ŷ�� �������� ������Ʈ ��Ų��.
	void	UpdateRecord(CMSG_RECORD_UPDATE_GS_DS& info);
		
	/// \brief �� �ε����� ���� ������� �� ã�´�.
	COrgRecord*	FindOrgRecord(int index)
	{
		COrgRecord*	pOrgRecord = NULL;
		LockRecord();
		map<int,COrgRecord*>::iterator itr = m_map_orgRecord.find(index);
		if(itr != m_map_orgRecord.end()){
			pOrgRecord = itr->second;		}
		UnLockRecord();
		return pOrgRecord;
	}

	//	m_pool_record�� Ǯ �߰� 
	void AddPool();	
	 MapOrgRecord* GetMapOrg() {return &m_map_orgRecord;}
private:	
	CRITICAL_SECTION			m_cs ;
	///< ������or��ȸ �ε��� COrgRecord* ���� ��
	MapOrgRecord				m_map_orgRecord;	
	COrgRecord*					m_pStartOrgRecord;
	bool						m_bUse_record;
	///< COrgRecord pool
	NDDList< COrgRecord* >		m_pool_record;	
	///<	m_pool_record�� ī��Ʈ 
	int							m_pool_count;
	///<	���� ����ǰ� ����ε��� 
	int							m_sManorIndex;	
	int							m_combating_org[MAX_ORG_COUNT];
};

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
/// \class COrgRecord
/// \brief ������ ����� ���
///        ���κ� ����, Ÿ �����ܰ��� ����, ������������
class COrgRecord
{
public:
	COrgRecord();
	~COrgRecord();
public:
	void Lock()		{ EnterCriticalSection( &m_cs ); }
	void UnLock()	{ LeaveCriticalSection( &m_cs ); }

	int				GetAllyIndex(){return m_iAllyIndex;}
	void			SetAllyIndex(int  iAllyIndex){m_iAllyIndex = iAllyIndex;}
	unsigned short	GetWinCount()	{return m_usWinCount;}
	void			SetWinCount(unsigned short winCount){m_usWinCount = winCount;}
	unsigned short	GetLoseCount()	{return m_usLoseCount;}
	void			SetLoseCount(unsigned short loseCount){m_usLoseCount = loseCount;}
	unsigned short	GetCombatCount()	{return m_usCombatCount;}
	void			SetCombatCount(unsigned short combatCount){m_usCombatCount = combatCount;}
	unsigned short	GetRecordState(){return m_usRecordState;}
	void			SetRecordState(unsigned short state){m_usRecordState = state;}
	unsigned short	GetEntryCount()	{return m_usEntryCount;}
	void			SetEntryCount(unsigned short entryCount){m_usEntryCount = entryCount;}
	int				GetManorIndex(){return m_sManorIndex;}
	void			SetManorIndex(int  iIndex){m_sManorIndex = iIndex;}

	/// \brief ����Ʈ�� org data �ֱ� 
	void	PushBackListOrgrecord(_sOrgRecord& record);
	
	/// \brief	��� �ִ� ����� ������� ���
	_sCombatRecord*	GetEmptyCombatRecord();
	
	///	\brief	��� �ε����� ��� ���� ���.
	_sCombatRecord* FindCombatRecord(short index);	

	/// \brief _sCombatRecord �� �޾� count ���� �°� ���� ���ش�
	void FillCombatRecord(_sCombatRecord &record,int count);
	/// \brief _sOrgRecord �� �޾� count ���� �°� ����
	int  FillOrgRecord(_sOrgRecord* record);

	/// \brief ��Ŷ�� �������� ����������� ���� ������Ʈ
	void UpdateCombatRecord(_sCombatRecord &record,int count);

	/// \brief ��Ŷ�� �������� Ÿ�ܰ������� ������Ʈ 
	void UpdateOrgRecord(_sOrgRecord* pRecord,int count);

	/// \brief DB�� _cUpdateOrgStandingsAccessor  Ÿ�ܰ��� ���� ������Ʈ
	/// \return return���� �ο� ���� ���� 
	int UpdateOrgStandings(BYTE* pos);

    /// \brief	all get againstOrg index
	int AllAgainstOrgIndex(int* pAgainst);

	///	\brief	Ÿ ���ε����� ���� ���. 
	_sOrgRecord*	FindVSOrgRecord(short index);	
	
	///\brief �ʱ�ȭ �Լ� 
	bool	InitializeOrzRecord();	
	void	Destory();

	/// \brief ���� �ʱ�ȭ �Լ� : ����� ���۽� ����,������ ��� Reset��Ų��.
	bool	ResetWarRecord();
	
	/// \brief ����� ������ ���� �� ������ ��� 
	void	CombineManorRecord();
	
	/// \brief �� �� �� �޸� Ǯ �߰� 
	void	AddOrgPool();

	void ViewAllOrgRecord();

private:
	CRITICAL_SECTION	m_cs ;
	int					m_iAllyIndex;	///<	�� �ε��� 
	unsigned short		m_usWinCount;	///<	�� �¼�
	unsigned short		m_usLoseCount;	///<	�� ��
	unsigned short		m_usCombatCount;///<	�� ���� 
	unsigned short		m_usRecordState;///<	��ϻ��� 
	unsigned short		m_usEntryCount;	///<	��ϻ��� 
	short				m_sManorIndex;	///<	���� �������� �ε���, �������� �ƴ϶�� ������ ����� �ε��� 
	NDDList<_sOrgRecord>		m_list_orgRecord ;	///< �ܺ� ���� ���
	
	///< �� ����� �������(�켱 ����� �ִ� 4���� �����Ѵ�) = `07.03.26 ���� ��� 2��
	_sCombatRecord*				m_array_combatRecord[MAX_MANOR];
};



#endif 
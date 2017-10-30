// XWindow_NoticeProcess.h: interface for the XWindow_NoticeProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX__XNOTICEPROCESS_H__3B906D3A_0CAB_4068_B77F_91A948E6EE53__INCLUDED_)
#define AFX__XNOTICEPROCESS_H__3B906D3A_0CAB_4068_B77F_91A948E6EE53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>
#include <vector>
using namespace std;

#include "XNoticeScript.h"

class _XNoticeProcess  
{
public:
	static _XNoticeProcess*  GetInstance();
	~_XNoticeProcess();

	bool SetNoticeProfile( int nMsgType, LPTSTR pMsg, int nMinStartTime, int nMinInterval, int nPrintCount, BOOL bLargeFont ); //�޽��� ���� �ð��� �д���...
	bool SetNoticeProfile( LPTSTR szFileName  ); //����
	void ModifyNoticeProfile( int nMsgType, LPTSTR pMsg = NULL, int nMinStartTime = -1, int nMinInterval = -1 ); // ������ ���ڿ� �������� ����
	void Update(float fAccumTime);
	void Destroy();

	void CompulsionMessage( int nID ); //���� �޽��� ������ ���

	inline double GetCurrentTime() { return m_dCurrnetTime; };  //���� ���μ����� ����� �ð��� ���´�
	
private:
	_XNoticeProcess();

	struct sMsgInfo
	{
		int nMsgID;
		int nIntervalTime;
		int nStartTime;
		int nPrintCount;
		bool bPrint;
		BOOL bLargeFont;

		sMsgInfo()
		{
			nMsgID = -1;
			nIntervalTime = 0;
			nStartTime = 0;
			nPrintCount = -1;
			bPrint = true;
			bLargeFont = FALSE;
		}

		void SetInfo( int nID, int nStart, int nInterval, int nCount, BOOL bLarge )
		{
			nMsgID = nID;
			//�ʴ����� �ٲ�����...
			nStartTime = nStart * 60;
			nIntervalTime = nInterval * 60;
			nPrintCount = nCount;
			bLargeFont = bLarge;
		}

		void SetStartTime( int nMinTime ) {	nStartTime = nMinTime * 60;	}
		void SetIntervalTime( int nMinTime ) {	nIntervalTime = nMinTime * 60;	}
		void CountDown() { if( nPrintCount > 0 ) nPrintCount--; }
		void SetPrint( bool bResult ) { bPrint = bResult; }
		bool GetPrintResult() { return bPrint; }
		BOOL GetLargeFont() { return bLargeFont; }
		int GetCount() { return nPrintCount; }
	};

	bool AddString( int Key, LPTSTR pString, bool bModify = false ); // ��Ʈ���� ����Ѵ�
	LPCTSTR GetDataByIndex( int nIndex ); // ���̵� �ش�� ��Ʈ���� ���´�
	
	void CheckingForMessage();	//��°����� �޽����� �ִ��� �׻� üũ Update �Լ� ������ ȣ��
	void PrintMessgae( vector<sMsgInfo*>::iterator iter ); //�޼����� ������ش�

	bool SearchSlotFlagItem(); // �Ⱓ�� ������
	int GetTotalDays(int nYear, int nMonth, int nDay ); //������¥ ����ϱ�

private:
	enum
	{
		_XDEF_INVENTORYITEMPOSX = 11,
		_XDEF_INVENTORYITEMPOSY = 71,
		_XDEF_INVENTORYSLOT_WIDTH = 5,
		_XDEF_INVENTORYSLOT_HEIGHT = 5
	};

	static const int nSearchInterval;
	SYSTEMTIME m_st;

	map<int, string>	m_mpData;
	vector<sMsgInfo*>	m_vtInfo;
	_XNoticeScript	m_XNoticeParser;

	double m_dCurrnetTime;
	bool m_bFirst;

	static _XNoticeProcess* m_pInstance;
};

#endif // !defined(AFX__XNOTICEPROCESS_H__3B906D3A_0CAB_4068_B77F_91A948E6EE53__INCLUDED_)

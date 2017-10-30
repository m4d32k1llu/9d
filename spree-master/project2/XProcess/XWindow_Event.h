// XWindow_Event.h: interface for the _XWindow_Event class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_EVENT_H__175FBFE1_1BDA_40C0_A3BE_D44BEECDFBB1__INCLUDED_)
#define AFX_XWINDOW_EVENT_H__175FBFE1_1BDA_40C0_A3BE_D44BEECDFBB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "Xwindow.h"

typedef struct XEventInfo
{
	int		eventid;
	int		eventtype;
	int		startyear;
	int		startmonth;
	int		startday;
	int		endyear;
	int		endmonth;
	int		endday;
	int		nStartTime; //Ÿ�� 6 ������
	int		nEndTime; //Ÿ�� 6 ������
	int		serialslot[5];
	TCHAR	eventname[512];
	TCHAR	eventdes[512];

	int		nPackageIndex; //Author : ����� //breif : ���� ���� �̺�Ʈ //Ÿ�� 6�϶�,, ���� ���̵�
	int		nLevel; //Author : ����� //breif : ���� ���� �̺�Ʈ // Ÿ�� 6 �϶� ���� ��,,
}_XEvnetInfo;

class _XWindow_Event : public _XWindow  
{
public:
	_XWindow_Event();
	virtual ~_XWindow_Event();
	
public:
	_XStatic				m_BorderTile;
	_XImageStatic			m_BorderGradationTitle;
	
	int						m_SelectedEventID;
	int						m_SelectedEventType;
	int						m_SelectedMaxSerialNumber[5];

	TCHAR					m_SelectedEventName[512];	// ���õ� �̺�Ʈ �̸�
	TCHAR					m_SelectedEventTerm[512];	// ���õ� �̺�Ʈ �Ⱓ
	TCHAR					m_SerialNumber[5][8];		// �ø��� �ѹ��� ǥ���ϴ� ���ڿ� �迭
	
	_XImageStatic*			m_imageIMEBorderSerialNumber;	// �Է�â ���� 

	_XListBox*				m_listboxEventTitle;		// �̺�Ʈ ����� �����ִ� ����Ʈ �ڽ�
	_XListBox*				m_listboxEventDescription;	// �̺�Ʈ ������ �����ִ� ����Ʈ �ڽ�
	
	_XButton*				m_btnSerialNumberOK;		// �̺�Ʈ �ø��� �Է� �Ϸ� ��ư 
	

	_XIMEContainer			m_IMESerialNumber;			// �̺�Ʈ �ø��� ��ȣ �Է�â

	list <_XEvnetInfo*>		m_listEventInfo;			// �̺�Ʈ�� ��� ������ ������ ����Ʈ 

	int						m_nEventIDinfo; //Author : ����� //breif ���� ���� �̺�Ʈ �ϳ��� �߰� �Ѵ� //Date :08/03/19
	BOOL					m_bBuffEvent; //Author : ����� //breif : Ÿ�� 6 ��, ���� ��¥�� �Ǹ� �������� �߰� �Ѵ�
	BOOL					m_bFirstSetListItem; //Author : ����� //breif : ����Ʈ����� ó�� �����ߴ�..
private :
	_XButton*				m_bGetItem;		// �ޱ� ��ư //Author : ����� //breif : ����õ�� 7�ϱǿ��� �켱 ����

	struct EventResult
	{
		int	nEventID;
		BOOL bGiveItem;
	};

	EventResult m_EventInfo[8];
	
public:
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL ProcessWheelMessage( short zDelta );
	
	void MoveWindow( int X, int Y );	
	void ShowWindow( BOOL show );
	
	void SetListItem(void);
	void SetSelectedListItme( int selectedeventid );
	
	BOOL LoadEventInfo(void);

	BOOL CheckSerialNumber( int serialslotnumber );	// �ø��� ��ȣ ������ �´����� Ȯ���Ѵ�.
	
	BOOL CheckSerialNumberBox1(void);
	BOOL CheckSerialNumberBox2(void);
	BOOL CheckSerialNumberBox3(void);
	BOOL CheckSerialNumberBox4(void);
	BOOL CheckSerialNumberBox5(void);

	void SetEventResult( int nArray, int nID, BOOL bRes )
	{
		m_EventInfo[nArray].nEventID = nID;
		m_EventInfo[nArray].bGiveItem = bRes;
	}
	void SetEventResult( int nID, BOOL bRes )
	{
		for( int i = 0; i < 8; i++ )
		{
			if( m_EventInfo[i].nEventID == nID )
			{
				m_EventInfo[i].bGiveItem = bRes;
				break;
			}
		}
	}
	const BOOL GetCurrentEventStatus() const// �������� �̺�Ʈ �� �������� �ִٸ�
	{
		for( int i = 0; i < 8; i++ )
		{
			if( m_EventInfo[i].nEventID > 0 && !m_EventInfo[i].bGiveItem )
				return TRUE;
		}

		return FALSE;
	}

private :
	int GetTotalCurrentDay( int nYear, int nMonth, int nDay ); //Author : ����� //breif : ��¥ ���..
};

#endif // !defined(AFX_XWINDOW_EVENT_H__175FBFE1_1BDA_40C0_A3BE_D44BEECDFBB1__INCLUDED_)

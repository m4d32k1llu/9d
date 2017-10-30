// XWindow_NoticeProcess.cpp: implementation of the XWindow_NoticeProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "XSR_STRINGHEADER.H"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XWindow_EventIcon.h"

#include "XNoticeProcess.h"

//Author : ����� 
#if defined(_ACCLAIM_VERSION) /*|| defined(_XVIETNAMESE)*/ || defined(_XJAPANESE)
	#define _XENABLE_GAMEGUARD
#endif

#ifdef _XUSEAPEXMODULE
	#undef _XENABLE_GAMEGUARD
#endif

#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG) && !defined(_XGMCLIENT) 
	#ifdef _XENABLE_GAMEGUARD
		#define _XUSE_GAMEGUARD
	#endif
#endif
//Last updated: 07/01/22

_XNoticeProcess* _XNoticeProcess::m_pInstance = 0;

const int _XNoticeProcess::nSearchInterval = 10;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XNoticeProcess::_XNoticeProcess() : m_dCurrnetTime(0), m_bFirst(true), m_st()
{
	GetLocalTime(&m_st);
}

_XNoticeProcess::~_XNoticeProcess()
{
}

_XNoticeProcess* _XNoticeProcess::GetInstance()
{
	if( m_pInstance == 0 )
	{
		m_pInstance = new _XNoticeProcess;
	}

	return m_pInstance;
}

void _XNoticeProcess::Destroy()
{
	vector<sMsgInfo*>::iterator vtiter;

	m_mpData.clear();

	for( vtiter = m_vtInfo.begin(); vtiter != m_vtInfo.end(); vtiter++ )
	{
		delete (*vtiter);
	}
	m_vtInfo.clear();

	m_XNoticeParser.Destroy();

	m_dCurrnetTime = 0;
	m_bFirst = true;

	if( _XNoticeProcess::m_pInstance )
	{
		delete _XNoticeProcess::m_pInstance;
		_XNoticeProcess::m_pInstance = 0;
	}
}

bool _XNoticeProcess::SetNoticeProfile( int nMsgType, LPTSTR pMsg, int nMinStartTime, int nMinInterval, int nPrintCount, BOOL bLargeFont )
{
	vector<sMsgInfo*>::iterator iter = m_vtInfo.begin();

	for(; iter != m_vtInfo.end(); ++iter )
	{
		// ID �� ��ϵǾ������� ����
		if( (*iter)->nMsgID == nMsgType )
			return false;
	}

	// �޼��� ���� ����
	sMsgInfo* pMsgInfo = new sMsgInfo;

	// ���̵�� ���۽ð�, �ݺ��ð��� �����Ѵ�
	// nPrintCount �� -1 �� �ƴ� ����� �����Ǿ������� �� ���� ��ŭ�� ���, 0 �̸� ���� ��� ���� ����
	pMsgInfo->SetInfo( nMsgType, nMinStartTime, nMinInterval, nPrintCount, bLargeFont );
	m_vtInfo.push_back( pMsgInfo );

	if(	!AddString( nMsgType, pMsg ))
		return false;

	return true;
}

bool _XNoticeProcess::SetNoticeProfile( LPTSTR szFileName )
{
	if( !m_XNoticeParser.Load( szFileName ))
		return false;

	//�޽��� �� ���� ��ŭ ����
	for( int i = 0; i < m_XNoticeParser.GetMaxInfo(); i++)
	{
		SetNoticeProfile( m_XNoticeParser.GetInfo(i).GetMsgID(),
					  _XGETINTERFACETEXT( m_XNoticeParser.GetInfo(i).GetMsgID() ),
					  m_XNoticeParser.GetInfo(i).GetStartTime(),
					  m_XNoticeParser.GetInfo(i).GetInterval(),
					  m_XNoticeParser.GetInfo(i).GetPrintCount(),
					  m_XNoticeParser.GetInfo(i).GetLargeFont() );
	}

	return true;
}

// ���� �������� �����ҷ��� �Ҷ�... ���� �ð��̳� �ݺ��ð� �޼������� �����Ҽ��ִ� 
// ���۽ð� �ݺ��ð��� -1 �ϰ��� ���� ������ �ִ� ������ ����
// �ʿ����
void _XNoticeProcess::ModifyNoticeProfile( int nMsgType, LPTSTR pMsg, int nMinStartTime, int nMinInterval )
{
	vector<sMsgInfo*>::iterator iter = m_vtInfo.begin();

	for(; iter != m_vtInfo.end(); ++iter )
	{
		if( (*iter)->nMsgID == nMsgType )
		{
			if( nMinStartTime != -1 )
				(*iter)->SetStartTime( nMinStartTime );

			if( nMinInterval != -1 )
				(*iter)->SetIntervalTime( nMinInterval );

			if( pMsg )
                AddString( (*iter)->nMsgID, pMsg , true);
		}
	}
}

void _XNoticeProcess::Update(float fAccumTime)
{
	//��ø�Ǵ½ð� m_dCurrnetTime�� ���α׷��� �����ѵڷκ��� ����� �ð�
	if( m_bFirst )
	{
		m_bFirst = false;
		m_dCurrnetTime = 0;
	}
	else
		m_dCurrnetTime += fAccumTime;

	static const DWORD iCurrentCheckTime = g_LocalSystemTime;

//Author : ����� 
//���Ӱ��� ��/�� �˻� : m_dCurrnetTime�� �̿�������  �ʹ� ���� ����Ǵ°� ���Ƽ� Ÿ�̸ӷ� ��ü <��24>
#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
	
	if( (g_LocalSystemTime - iCurrentCheckTime) > 20000 )
	{
		if( g_GameGuardInstance.Check() != NPGAMEMON_SUCCESS ) 
		{
			extern LPTSTR g_nProtectModuleCheckMessage;
			_XFatalError(g_nProtectModuleCheckMessage);
		}
	}

#endif

#ifdef _XDEF_DDAY_CONTRACTITEM_071122_MAGVIPER //Author : ����� //breif : �Ⱓ�� ������ �޽���
	if( (int)m_dCurrnetTime - nSearchInterval == 0 )
	{
		if(SearchSlotFlagItem())
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3757), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); 
	}
#endif

	if( (int)m_dCurrnetTime == 0 ) 
	{
		//breif : PC �� �߰� ����
		if( g_pLocalUser->GetPCroomState( _XDEF_PCROOM_PREMIUM ) || g_pLocalUser->GetPCroomState( _XDEF_PCROOM_PREPAID ) )
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3353), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //PC�� �߰� ������ ���� �� �Դϴ�.

		#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ����� //breif : �̺�Ʈ ������
		_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
		if( pEventIcon && pEventIcon->GetShowStatus())
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3784), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //���� ���� ��ư�� ����, ������ �����ñ� �ٶ��ϴ�.
		}
		#endif
	}
	//g_NetworkKernel.InsertChatString(_T("���Ⱓ�� ���� �Ǿ����ϴ�"), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); 

	//��°����� �޽����� �ִ��� üũ
    CheckingForMessage();
}

void _XNoticeProcess::CheckingForMessage()
{
	vector<sMsgInfo*>::iterator iter = m_vtInfo.begin();

	for( ; iter != m_vtInfo.end(); ++iter )
	{
		// 0 ���� ������ �� ������� �ʴ´� 
		//���� ����� �Ǿ��ϴ� �ð����� ����Ѵ�
		if( (*iter)->nIntervalTime == 0 && (int)m_dCurrnetTime - (*iter)->nStartTime == 0 )
		{
			PrintMessgae( iter );
		}
		
		else if( (*iter)->nIntervalTime != 0 && (int)m_dCurrnetTime - (*iter)->nStartTime >= 0 && 
				 ((int)m_dCurrnetTime - (*iter)->nStartTime) % (*iter)->nIntervalTime == 0 )
		{
            PrintMessgae( iter );
		}
		else
			(*iter)->SetPrint( true ); // ����� �ȵǴ� �ð����� ����Ҽ��ִ� �غ���¸� ������ش�
	}
}

void _XNoticeProcess::PrintMessgae( vector<sMsgInfo*>::iterator iter )
{
    LPCTSTR pString = GetDataByIndex( (*iter)->nMsgID );

	TCHAR msgbuffer[512];
	int nHour = ((int)m_dCurrnetTime)/3600;
	_snprintf(msgbuffer, sizeof(msgbuffer), pString, nHour);
	
	// ���� ����� �ȵǾ��ִ��� �˻�
	if( (*iter)->GetPrintResult() && 
		(*iter)->GetCount() != 0 ) 
	{
		//����� �Ǿ��ٸ� ������ ����� ���ϰ� �ٲ��ش� ( 1�ʿ� �� �ѹ��� ����� ���� )
		(*iter)->SetPrint( false );
		(*iter)->CountDown();		
		
		//���⿡ ��� ����� ������~~~~~~~~~~~~~~~~


		//���� �ð��� ���´�
		SYSTEMTIME TimeInfo;
		GetLocalTime(&TimeInfo);

        g_NetworkKernel.InsertChatString( msgbuffer, _XDEF_CHATMESSAGECOLOR_ITEM, (*iter)->GetLargeFont() );
	}
}

void _XNoticeProcess::CompulsionMessage( int nID )
{
	// ID ������ �ٷ� ��� �� �� �ְ� �Ѵ�
}

bool _XNoticeProcess::AddString( int Key, LPTSTR pString, bool bModify)
{
	map<int, string>::iterator iter;

	iter = m_mpData.find( Key );
	
	//���� Ű���� �ִ°��
	if( iter != m_mpData.end() && !bModify)
		return false;
	
	//���̵�� ��Ʈ�� ����
	m_mpData[Key] = pString;
	
	return true;
}

LPCTSTR  _XNoticeProcess::GetDataByIndex( int nIndex )
{
	map<int, string>::iterator iter;
	
	// �ε�����ȣ�� ã�´�
	iter = m_mpData.find( nIndex );
	
	// ������ NULL �� ����
	if( iter == m_mpData.end() )
		return NULL;

	return iter->second.c_str();
}

bool _XNoticeProcess::SearchSlotFlagItem()
{
	//�κ��� ���� ó�� ������ġ (����ĭ �� 25 ) //�ɸ��� ������ ��ġ�� 5
	for( int count = 5; count < _XINVENTORY_SLOTTYPE_TOTALCOUNT; count++ )
	{
		if( count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT ) break;

		if( g_pLocalUser->m_UserItemList[count].Get_m_sID() > 0 &&
				g_pLocalUser->m_UserItemList[count].Get_m_ucYear() > 0 )
		{
			//���� 3���ĸ� ��� �ؾ� ��..
			int DueDay = GetTotalDays( (int)(g_pLocalUser->m_UserItemList[count].Get_m_ucYear()+1900), (int)g_pLocalUser->m_UserItemList[count].Get_m_ucMonth(), (int)g_pLocalUser->m_UserItemList[count].Get_m_ucDay()) 
				- GetTotalDays( m_st.wYear, m_st.wMonth, m_st.wDay );

			if( DueDay <= 3 )
			{
				//3�� �����̸�..
				return true;
			}
		}
	}

	return false;
}

int _XNoticeProcess::GetTotalDays(int nYear, int nMonth, int nDay )
{
    static int nTempMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    
    int ntempDay = ( nYear - 1 ) * 365;
    // ���� Ƚ���� ������.
    // 4�⸶�� �ѹ��� ����, 100�⿡ �ѹ��� ���� �ƴ�, 400�⿡ �ѹ��� ������
    // ���ش� ���� �ؾ� �ϴ� year-1
    ntempDay = ntempDay + ( nYear - 1 ) / 4 - ( nYear - 1 ) / 100 + ( nYear - 1 ) / 400 ;


    //���� ���� üũ
    if ( nYear % 4 == 0 )
    {
        nTempMonth[2] = 29;

        if ( nYear % 100 == 0 && nYear % 400 != 0 )
            nTempMonth[2] = 28;
    }

    // ���� �ޱ��� ��� ������.
    for (int i = 1 ; i < nMonth; i ++)
        ntempDay += nTempMonth[i];

    // �̹��޿��� ���� ������ ������
    ntempDay += nDay;

    return ntempDay;
}
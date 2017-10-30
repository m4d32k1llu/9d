// _XNoticeScript.cpp: implementation of the _XNoticeScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XNoticeScript.h"
#include "EODEXTDEF_GlobalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XNoticeScript::_XNoticeScript() : m_nMaxInfo(0), m_pInfo(NULL), m_bFirst(TRUE)
{

}

_XNoticeScript::~_XNoticeScript()
{

}


void _XNoticeScript::Destroy()
{
	m_bFirst = TRUE;
	m_nMaxInfo = 0;
	m_pStream = NULL;

	if(m_pInfo)
        delete [] m_pInfo;
}

BOOL _XNoticeScript::Load( LPTSTR szFileName )
{
	if( !m_bFirst )
		return FALSE;

	m_bFirst = FALSE;

	BOOL bResult;
	
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];									

	_splitpath( szFileName, drive, dir, fname, ext );
	ext[4] = 0;

	//Ȯ���ڰ� txt ���� Ȯ��
	if( stricmp( ext, ".txt" ) )
		return FALSE;

	bResult = ImportTXT( szFileName );
	g_ScriptArchive.ClosePackage();
	m_pStream = NULL;

	return bResult;
}

BOOL _XNoticeScript::ImportTXT( LPTSTR szFileName )
{
	TCHAR pBuffer[256] = {0,};

	if(OpenStream( szFileName ) == -1)
		return FALSE;

	//�� ������ ��ΰ� Ȯ����  scriptpackage �� �ݰ� m_pStream �ʱ�ȭ�� ���� 
	GetMaxDataCountFromFile( "_XM_EOS" );

	if(OpenStream( szFileName ) == -1)
		return FALSE;

	//�� ������ŭ ������ ������ �����Ѵ�
	if( !m_pInfo && m_nMaxInfo )
	{
        m_pInfo = new sInfo[m_nMaxInfo];
		ZeroMemory( m_pInfo, sizeof( sInfo ) * m_nMaxInfo ); 
	}

	if( GetDataFromFileL( _T("_XM_NOTICE")) == -1 )
		return FALSE;

	for( int i = 0; i < m_nMaxInfo; i++ )
	{
		fgets( pBuffer, 256, m_pStream ); //���پ� �д´�

		//������ �������� �˻��Ѵ�
		if(CheckCommentFromFile( pBuffer )) 
		{
			i--;
			continue;
		}

		//������ �����͸� �����Ѵ�
		sscanf( pBuffer, "%d%d%d%d%d", &m_pInfo[i].nMsgID, 
								     &m_pInfo[i].nStartTime, 
								     &m_pInfo[i].nInterval, 
								     &m_pInfo[i].nPrintCount,
									 &m_pInfo[i].bLargeFont);

	}

	if( GetDataFromFileL( _T("_XM_EOS")) == -1 )
		return FALSE;

	return TRUE;
}

//���� ���پ� �˻��Ѵ�
BOOL _XNoticeScript::GetDataFromFileL( LPTSTR pString ) 
{
	TCHAR pBuffer[256] = {0,};
	int nSize = strlen( pString );

	while( !feof( m_pStream ) )
	{
		fgets( pBuffer, 256, m_pStream );		

		//ã�� ���ڿ��� �ִ����� ���� �˻�
		if( !strncmp(pBuffer, pString, nSize ) ) 
		{
			return TRUE;
		}
	}
	return ErrorCloseStream( "��ü���Ͽ��� �˻��Ҽ� �����ϴ�");	
}

BOOL _XNoticeScript::GetMaxDataCountFromFile( LPTSTR pString )
{
	int nCount = 0;
	TCHAR pBuffer[256] = {0,};

	while( !feof( m_pStream ) ) //������ ������ �˻�
	{
		pBuffer[0] = 0;
		fgets( pBuffer, 256, m_pStream );		

		if( !strncmp( pBuffer, pString, 7 ) )
		{
			m_nMaxInfo = nCount; //END �� �������������� ��ü ����������			
			return TRUE;
		}

		// ���ٸ��� ��ȿ�� ������ ���� �˻��Ѵ�
		if( CheckCommentFromFile(pBuffer ) )
		{
			continue;
		}

		//��ȿ�ѵ������� ����
		nCount++;
	}

	return FALSE;
}

BOOL _XNoticeScript::CheckCommentFromFile( LPTSTR pString )
{
	// ; # ���ͳ� ���� ������ ���� �����ʴ´�..	
	if( !strncmp( pString, ";", 1 ) || !strncmp( pString, "_", 1 ) || pString[0] == 10 || pString[0] == 13 )
		return TRUE;

	return FALSE;
}

BOOL _XNoticeScript::OpenStream( LPTSTR szFileName )
{
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	m_pStream = g_ScriptArchive.GetPackedFile(szFileName);

	if ( m_pStream == NULL)
	{
		return ErrorCloseStream( "������ ã�� ���߽��ϴ�.");				
	}	

	return TRUE;
}

BOOL _XNoticeScript::ErrorCloseStream( LPTSTR pMsg )
{
	if( pMsg != NULL )
	{
		TCHAR szBuffer[256];
		memset(szBuffer, 0, sizeof(szBuffer));

		_snprintf(szBuffer, sizeof(szBuffer), "[File: %s][Line: %d]\n[Note : %s]", __FILE__, __LINE__, pMsg);	
		MessageBox(NULL, szBuffer, "ERROR", MB_ICONERROR);	

	}

	return -1;
}
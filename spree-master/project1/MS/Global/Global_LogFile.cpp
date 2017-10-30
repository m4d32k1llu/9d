#include "..\stdafx.h"
#include "Global_LogFile.h"




bool _j_XFile::xFileCreate(char * cFileName, char * cMode)
{
	char cFile[512] ={0};	
	GetLocalTime( &curTime ) ;

	BOOL success ;
	success = CreateDirectory( "logfile", 0 ) ;
	if( !success )
	{
		int err = GetLastError() ;
		if( err != ERROR_ALREADY_EXISTS )
		{
			printf( "Create Directory Failed : %d\n", err ) ;
			return false ;
		}
	}

	sprintf(cFile,"logfile\\%s_%4d%02d%02d %02dh%02dm.txt", 
		cFileName,curTime.wYear,curTime.wMonth,curTime.wDay,curTime.wHour,curTime.wMinute);

	try{
		m_pStream = fopen(cFile,cMode);
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_XFile::xFileCreate() -fopen() \n");
		return false;
	}

	if(m_pStream == NULL){
		return false;
	}

	xFileWrite(cFileName);
	xFileWrite("\n");
	xFileWrite("============================================================\n");	
	xFileWrite(cFile);	
	xFileWrite("============================================================\n");		


	return true;
}

void _j_XFile::xFileWrite(char * pvWriteString)
{
	try{		
		fputs(pvWriteString, m_pStream) ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_XFile::xFileWrite() \n");
		return;
	}	
}

bool _j_XFile::xFileClose()
{
	char buf[512]={0};
	
	sprintf(buf,"�����ð� : %d��%d��%d��%d��%d��%d�� \n", 
		curTime.wYear,curTime.wMonth,curTime.wDay,curTime.wHour,curTime.wMinute,curTime.wSecond);		
	xFileWrite(buf) ;

	try{
		if (m_pStream ) {
			if(fclose(m_pStream) == 0)		
				return true;
		}		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_XFile::xFileClose() \n");
		return false;
	}

	return false;
}

bool _j_GlobalLog::LogFileCreate(char * fileName, char * mode)
{	
	
	iLineCount = 0;	//log count �ʱ�ȭ

	memset(m_cFileName, 0, sizeof(m_cFileName)) ;
	sprintf(m_cFileName, fileName) ;
	
	if(!xFileCreate(m_cFileName,mode)){
		return false;
	}		

	return true;
}
void _j_GlobalLog::LogFileWrite( const char *format,... )
{
	memset(m_szBuffer, 0, sizeof(m_szBuffer)) ;

	try{
		/*
		if(  LOG_MAX_LINE > 0 && iLineCount > LOG_MAX_LINE ){
			LogFileClose();			// ���� �α����� �ݰ�
			LogFileCreate(m_cFileName,"w+");		// �� �α����� ����
			iLineCount = 0;			
		}
		*/
		va_list arglist ;
		va_start(arglist, format) ;
		vsprintf(m_szBuffer, format, arglist) ;
		va_end(arglist) ;
		xFileWrite(m_szBuffer);
		iLineCount++;			// �α� ���� ī���� ����

		if( iLineCount % 32 )
			xFileFlush();
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_UserLoginLog::LogFileWrite(PUSER pUser, int mode)\n ");
		return;
	}
	
}
bool _j_GlobalLog::LogFileClose()
{
	return xFileClose();
}

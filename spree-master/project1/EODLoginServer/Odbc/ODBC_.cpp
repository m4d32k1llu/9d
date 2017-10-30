#include "..\stdafx.h"
#include "../myServer/FileLog.h"
#include "./ODBC_.h"

COdbc::COdbc() {
	hEnv = NULL;
	hDbc = NULL;
	hStmt = NULL;

	iAffectedCnt = -1;
	siCol = 0;

	for(int i = 0; i < MAX_COLUMN; i++) {
		iColInfo[i] = 0;
	}

	// ȯ�� �ڵ� ����
	SQLRETURN ret;
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		Logout(_T("COdbc::COdbc: #ERR ȯ�� �ڵ� ���� ����"));
	}

	ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		Logout(_T("COdbc::COdbc: #ERR ȯ�� �ڵ� �Ӽ� ���� ����"));
	}
}

COdbc::~COdbc() {
	if(hStmt != NULL) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = NULL;
	}

	if(hDbc != NULL) {
		SQLDisconnect(hDbc);
		Logout(_T("COdbc::~COdbc: ��� ���� ���� ����"));
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		hDbc = NULL;
	}

	if(hEnv != NULL) {
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		hEnv = NULL;
	}
}

BOOL COdbc::Connect() {
	return this->Connect(ConnectInfo.pszConStr, ConnectInfo.pszId, ConnectInfo.pszPass);
}

BOOL COdbc::Connect(COdbcConnectInfo info) {
	memcpy(&ConnectInfo, &info, sizeof(COdbcConnectInfo));
	return this->Connect(ConnectInfo.pszConStr, ConnectInfo.pszId, ConnectInfo.pszPass);
}

BOOL COdbc::Connect(LPTSTR pszConStr, LPTSTR pszId, LPTSTR pszPass) {
	this->Disconnect();

	if(pszConStr[0] == NULL || pszId[0] == NULL || pszPass[0] == NULL) {
		Logout(_T("COdbc::Connect: Param is NULL. pszConStr=%s, pszId=%s, pszPass=%s"), pszConStr, pszId, pszPass);
		return FALSE;
	}
	if(0 == lstrlen(ConnectInfo.pszConStr)) {
		Logout(_T("COdbc::Connect: ODBC ���� ���� ���"));
		lstrcpy(ConnectInfo.pszConStr, pszConStr);
		lstrcpy(ConnectInfo.pszId, pszId);
		lstrcpy(ConnectInfo.pszPass, pszPass);
	}
	Logout(_T("COdbc::Connect: DSN=%s, ID=%s"), pszConStr, pszId);

	SQLRETURN ret;

	// ���� �ڵ� ����
	if(hDbc == NULL) {
		ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
		if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			Logout(_T("COdbc::Connect: #ERR ���� �ڵ� ���� ����"));
		}
	}
	Logout(_T("COdbc::Connect: ��� ���� �ڵ� ���� ����"));

	SQLSetConnectAttr(hDbc, 0, (void*)SQL_LOGIN_TIMEOUT, 0);
	ret = SQLConnect(hDbc, (SQLTCHAR*)pszConStr, SQL_NTS, (SQLTCHAR*)pszId, SQL_NTS, (SQLTCHAR*)pszPass, SQL_NTS);
	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		Logout(_T("COdbc::Connect: #ERR ��� ���� ����"));
		return FALSE;
	}
	Logout(_T("COdbc::Connect: ��� ���� ����"));

	// ��� �ڵ� ����
	if(hStmt != NULL) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = NULL;
	}
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		Logout(_T("COdbc::Connect: #ERR ��� �ڵ� ���� ����"));
		return FALSE;
	}
	Logout(_T("COdbc::Connect: ��� ��� �ڵ� ���� ����"));
	Logout(_T("COdbc::Connect: ODBC ��� ��� �غ� �Ϸ�"));
	return TRUE;
}

void COdbc::Disconnect() {
	Logout(_T("COdbc::Disconnect: ��� ���� ���� �õ� DSN=%s, ID=%s"), ConnectInfo.pszConStr, ConnectInfo.pszId);
	//memset(&ConnectInfo, 0, sizeof(COdbcConnectInfo)); // �翬�� �õ��� ��, �ʿ��� ����

	if(hStmt != NULL) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = NULL;
		Logout(_T("COdbc::Disconnect: ��� ��� �ڵ� ����"));
	}
	if(hDbc != NULL) {
		SQLDisconnect(hDbc);
		Logout(_T("COdbc::Disconnect: ��� ���� ����"));
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		hDbc = NULL;
		Logout(_T("COdbc::Disconnect: ��� ���� �ڵ� ����"));
	}
	
	Logout(_T("COdbc::Connect: ODBC ��� ���� �Ϸ�"));
}

BOOL COdbc::Exec() {
	static BOOL bStatus = TRUE;	// ODBC ��� Connect ����
	static int iTimeInterval = 0;		// ���� �ð� ��, �翬�� �õ�
	if(bStatus == FALSE) {
		// ������ �������� ���, ���� �ð� �� ������ ��õ��ϵ���..
		if(GetTickCount() - iTimeInterval < 600000/*10��=600��*/) {
			return FALSE;
		}
		else {
			bStatus = TRUE;
		}
	}

	SQLRETURN ret = SQLExecute(hStmt);
	if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO || ret == SQL_NO_DATA)
		return TRUE;
	else {
		_GetDiagRec();

		/*
		if(!_Recover(RECOVER_HANDLE) || !Exec()) {
			if(!_Recover(RECOVER_CONNECT) || !Exec()) {
				bStatus = FALSE;
				iTimeInterval = GetTickCount();
				return FALSE;
			}
		}
		*/
		if(!_Recover(RECOVER_CONNECT) || !Exec()) {
			bStatus = FALSE;
			iTimeInterval = GetTickCount();
			return FALSE;
		}
		return TRUE;
	}
}

// SQL���� �����Ѵ�. ���н� ���� ������ ����ϰ� FALSE�� �����Ѵ�.
BOOL COdbc::Exec(LPTSTR pszSql) {
	static BOOL bStatus = TRUE;	// ODBC ��� Connect ����
	static int iTimeInterval = 0;		// ���� �ð� ��, �翬�� �õ�
	if(bStatus == FALSE) {
		// ������ �������� ���, ���� �ð� �� ������ ��õ��ϵ���..
		if(GetTickCount() - iTimeInterval < 600000/*10��=600��*/) {
			return FALSE;
		}
		else {
			bStatus = TRUE;
		}
	}

	// SQL_NO_DATA�� ������ ��쵵 �������� ����Ѵ�. 
	// �� ��� Fetch���� EOF�� �����ϵ��� �Ǿ� �ֱ� ������ ���� ������ ����� �ʿ�� ����.
	SQLRETURN ret = SQLExecDirect(hStmt, (SQLTCHAR*)pszSql, SQL_NTS);
	if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO || ret == SQL_NO_DATA)
		return TRUE;
	else {
		_GetDiagRec();

		/*
		if(!_Recover(RECOVER_HANDLE) || !Exec(pszSql)) {
			if(!_Recover(RECOVER_CONNECT) || !Exec(pszSql)) {
				bStatus = FALSE;
				iTimeInterval = GetTickCount();
				return FALSE;
			}
		}
		*/
		if(!_Recover(RECOVER_CONNECT) || !Exec(pszSql)) {
			bStatus = FALSE;
			iTimeInterval = GetTickCount();
			return FALSE;
		}
		return TRUE;
	}
}

// Ŀ���� �ݰ� ���ε� ������ �����Ѵ�.
void COdbc::Clear() {
	SQLCloseCursor(hStmt);
	SQLFreeStmt(hStmt, SQL_UNBIND);
}

BOOL COdbc::_Recover(RECOVER_PARAM param) {
	SQLRETURN ret;

labelRecover:
	switch(param) {
	case RECOVER_HANDLE:
		Logout(_T("COdbc::_Recover: ��� �ڵ� ���� �õ���"));
		if(hStmt != NULL) {
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			hStmt = NULL;
		}
		ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
		if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			Logout(_T("COdbc::_Recover: #ERR ��� �ڵ� ���� ����"));
			param = RECOVER_CONNECT;
			goto labelRecover;
		}
		return TRUE;

	case RECOVER_CONNECT:
		Logout(_T("COdbc::_Recover: ���� ���� �õ���"));
		return this->Connect();
	}

	return TRUE;
}

void COdbc::_GetDiagRec() {
	SQLRETURN SqlRet;
	SQLINTEGER iNativeError;
	SQLTCHAR pszSqlState[8], pszMsg[1024];
	SQLSMALLINT siMsgLen;

	SqlRet = SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, pszSqlState, &iNativeError, pszMsg, sizeof(pszMsg), &siMsgLen);
	if(iNativeError != 0xCCCCCCCC) {
		Logout(_T("COdbc::_Recover: #ERR SQLSTATE:%s, NativeError=%d, ��������:%s"), (LPTSTR)pszSqlState, iNativeError, (LPTSTR)pszMsg);
	}
	/*for(int iRecCnt = 1; ; iRecCnt++) {
	SqlRet = SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, iRecCnt, pszSqlState, &iNativeError, pszMsg, sizeof(pszMsg), &siMsgLen);
	Logout(_T("COdbc::_Recover: #ERR SQLSTATE:%s, NativeError=%d, ��������:%s"), (LPTSTR)pszSqlState, iNativeError, (LPTSTR)pszMsg);
	if(SQL_NO_DATA == SqlRet) break;
	}*/
}

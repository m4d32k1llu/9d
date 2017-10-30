// FTPTransfer.h: interface for the CFTPTransfer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FTPTRANSFER_H_
#define _FTPTRANSFER_H_

//#include <windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wininet.h>


/** FTP Transfer class.
 *
 * ������ file�� FTP ���������� ����Ͽ� �����ϴ� Ŭ����.
 * \author Suhyun, Park
 * \date 2003.08.18
 * \see XFileTransferDlg.cpp
 */
class CFTPTransfer  
{
protected :

	/// FTP ���� �ּ�
	CString			m_sServer;
	/// Proxy ���� �ּ�
	CString			m_sProxyServer;

	/// User name : default�� anonymous ���
	CString			m_sUserName;
	/// Password : anonymous�� ��� password ����
	CString			m_sPassWord;

	/// Passive setting�� ����ϴ��� ����
	BOOL			m_bUsePassive;
	/// Proxy server�� ����ϴ��� ����
	BOOL			m_bUseProxy;
	/// Preconfig option�� ����ϴ��� ����
	BOOL			m_bUsePreconfig;

	/// Internet session
	HINTERNET		m_hInternetSession;
	/// FTP session
	HINTERNET		m_hFTPSession;
	/// File connection - ���� ������� ����
	HINTERNET		m_FileConnection;

	/// WIN32_FIND_DATA - ���� ������� ����
	WIN32_FIND_DATA	m_sWFD;
	/// ���� ���
	BOOL			m_bResult;
	/// ������ ������ Local Full Path
	CString			m_sInputSpec;
	/// ������ ������ ����� FTP server full path
	CString			m_sOutputSpec;

public:
	/** Constructor.
	 */
	CFTPTransfer();
	/** Desctructor.
	 */
	virtual ~CFTPTransfer();

	/** �ʱ�ȭ �Լ�.
	 * 
	 * �����ּ�, user name, password ����, Passive/Proxy/Preconfig ��뿩�� üũ
	 * \return ���� ����
	 */
	BOOL			Initialze(void);
	/** FTP ������ �����ϴ� �Լ�.
	 *
	 * \return ���� ����
	 */
	BOOL			ConnetServer(void);
	/** FTP ������ Path �����ϴ� �Լ�.
	 *
	 * ���۵� ������ ����� ���丮�� �����ϰ�, �� ���丮�� �̵��Ѵ�.<br>
	 * ���丮 �̸��� EmperorOfDragons���� ���Ǵ� �����̸�_LocalTime() �̴�.
	 */
	void			SetServerPath( CString senderid );
	/** ���� ����.
	 *
	 * \return ���� ����
	 */
	BOOL			SendFile(void);

	/** m_sInputSpec setting.
	 *
	 * ������ ������ Local Full Path setting
	 */
	inline void		SetInputSpec(TCHAR* inputfilename)	{	m_sInputSpec = inputfilename;	}
	/** m_sInputSpec concatenation.
	 *
	 * ������ ������ Local Full Path setting
	 */
	inline void		ConcatInputSpec(TCHAR* filename)	{	m_sInputSpec += filename;		}
	/** m_sOutputSpec setting.
	 *
	 * ������ ������ ����� FTP server full path
	 */
	inline void		SetOutputSpec(TCHAR* outputfilename)	{	m_sOutputSpec = outputfilename;	}
	
};

#endif 

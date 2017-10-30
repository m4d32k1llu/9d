// XFileTransfer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "XFileTransfer.h"
#include "XFileTransferDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXFileTransferApp

BEGIN_MESSAGE_MAP(CXFileTransferApp, CWinApp)
	//{{AFX_MSG_MAP(CXFileTransferApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXFileTransferApp construction

CXFileTransferApp::CXFileTransferApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXFileTransferApp object

CXFileTransferApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CXFileTransferApp initialization

HMODULE g_hUnicowsModule = NULL;

HMODULE LoadUnicowsProc(void) 
{
	g_hUnicowsModule = LoadLibraryA("unicows.dll"); 
	
	return g_hUnicowsModule;
} 

#ifdef _cplusplus 
extern "C" { 
#endif 
	extern FARPROC _PfnLoadUnicows = (FARPROC) &LoadUnicowsProc; 
#ifdef _cplusplus 
} 
#endif 

BOOL CXFileTransferApp::InitInstance()
{
	_PfnLoadUnicows();
	
	if( !g_hUnicowsModule )
	{
		::MessageBox( NULL, _T("Could not load unicouws.dll"), _T("Error!"), MB_OK | MB_ICONWARNING );
		return FALSE;
	}

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CXFileTransferDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

// XWindow_Contribution.h: interface for the _XWindow_Contribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CONTRIBUTION_H__CD341A79_76BC_4013_A3FE_F0D160C2F24E__INCLUDED_)
#define AFX_XWINDOW_CONTRIBUTION_H__CD341A79_76BC_4013_A3FE_F0D160C2F24E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief ���ı⿩�� â 
 * �����۰� ������ ���ı⿩���� ��
 * 
 * [Apply Clan]
 * ������ True/False ����.
 * Apply Clan ��ġ�� ��� CP1 ����
 * Apply Clan ����ġ�� ��� CP2 ����

 * 0 - ����(����)
 * 1 - ����
 * 2 - ���
 * 3 - �Ҹ���
 * 4 - �츲��
 * 5 - ����
 * 6 - ����
 * 7 - ����
 * 8 - ����
 * 9 - ����
 * 10 - ��� ������ ����. ������ CP2.
 * 11 - ����, �Ҹ���, ����
 * 12 - ���, �츲��, ����
 * 13 - ����, �Ҹ���, ����, ����
 * 20 - ��� ����
*/

class _XWindow_Contribution : public _XWindow  
{
private:	
	_XImageStatic		m_ContributionBox;
public:
	_XWindow_Contribution();
	virtual ~_XWindow_Contribution();

	BOOL	Initailize(void);
	void	DestroyWindow(void);
	
	void	Draw(_XGUIObject*& pfocusedobject = g_CurrentFocusedObject);
	BOOL	Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);	

	void				MoveWindow( int X, int Y );
	void				ShowWindow(BOOL show);

// Draw item...
public:
	int					m_CurrentItemContribution;
	DWORD				m_SendDelayTime;
	
	int					m_ContributionType;
	int					m_ContributionSecondType;
	int					m_ContributionThirdType;
	int					m_ContributionId;
	int					m_ContributionModelIndex;
	int					m_ContributionGender;
	int					m_ContributionIvenIndex;
	int					m_ContributionItemCount;
	
	FLOAT				m_RotateAngle;
	D3DXMATRIX			m_RHWMatrix;
	_X3PCamera			m_ModelViewCamera;
	D3DVIEWPORT9		m_ModelViewPort;	

	void				DrawContributionItem( void );	///< 3�� �� ���÷���
	void				ResetContributionData();	
	void				SetAddData();	///< ������ ���� ����
	/// ������ ��Ű���� cApplyClan ���� ���� �⿩�� ��(cp1, cp2)�� ����
	int					CheckClanPoint(int type, int id);
	int					CalcContribution(int invennumber);	///< �⿩�� ���
};

#endif // !defined(AFX_XWINDOW_CONTRIBUTION_H__CD341A79_76BC_4013_A3FE_F0D160C2F24E__INCLUDED_)

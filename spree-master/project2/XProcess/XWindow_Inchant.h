// XWindow_WeaponUpgrade.h: interface for the _XWindow_WeaponUpgrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_WEAPONUPGRADE_H__A98EF4A3_3769_4C6A_878F_A444FC2FB0FC__INCLUDED_)
#define AFX_XWINDOW_WEAPONUPGRADE_H__A98EF4A3_3769_4C6A_878F_A444FC2FB0FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief ���� â
 * �������� ������
 * 
 * ������ 1���� �������� ����� �ް� �� ����� ���� �������� �����ְ� �Ǿ��ִ�.
 * 1�� ����� ���� �ĸ� ���������Ḧ �ص� ����� �״�� �����ִ�.
 * ������ ����2 ������ �ְ� ������ ���п� ���� ó���� Ʋ������.
*/

/// ���� �ܰ�
enum InchantStep{ US_NONE=0, US_READY, US_STEP1, US_STEP2, US_STEP3, US_STEP4, US_STEP5 };

class _XWindow_Inchant : public _XWindow  
{
private:
	_XButton*			m_InchantApplyButton;
	_XButton*			m_pApplyCancleButton;
	
	InchantStep			m_InchantStep;
	TCHAR				m_InchantInfoText[128];

	_XImageStatic*		m_slotImage;
	unsigned char		m_FirstResult;

public:
	_XUserItem*			m_InchantItem;
	/// m_InchantStuff[0] �� �ʿ��� �������� ǥ���ϱ� ���� ���� 
	/// m_InchantStuff[1] �� ���� ����� ������
	_XUserItem			m_InchantStuff[3];		// inchant table Ȯ��
												// m_InchantStuff[2]�� �ʿ��� ��� 2��°

public:
	_XWindow_Inchant();
	virtual ~_XWindow_Inchant();

	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void				ShowWindow(BOOL show);
	void				MoveWindow( int X, int Y );
	
	void				SetItemInchantStep(InchantStep step);	///< �ܰ迡 ���� ó��
	void				MessagePrint(int msgindex);			///< ���� �޼���
	void				ClickStartButton(void);				///< m_InchantApplyButton Ŭ�� ó��
	void				RebuildStuffInfo(void);

	FLOAT				m_InchantItemtRotateAngle;
	D3DXMATRIX			m_InchantItemRHWMatrix;
	_X3PCamera			m_InchantItemModelViewCamera;
	D3DVIEWPORT9		m_InchantItemModelViewPort;

	void				DrawInchantItem( void );
	int					GetInchantItemModelIndex(void);	

	int					CheckStuffCount(int selectslot);	///< ��� ���� �˻� : 1 �̸� ���� 
	void				SetFirstInchantResult(unsigned char result) { m_FirstResult = result; 	};
	unsigned char		GetFirstInchantResult() { return m_FirstResult;	};
	int					CheckWeaponGrade(int searchslot);	///< ���� ��޿� ���� ���� ���� üũ 
	
	BOOL				ProcessInchantCashItem(int slotindex);	// �Ⱓ�� ���������� üũ
	BOOL				ProcessInchant();
};

#endif // !defined(AFX_XWINDOW_WEAPONUPGRADE_H__A98EF4A3_3769_4C6A_878F_A444FC2FB0FC__INCLUDED_)

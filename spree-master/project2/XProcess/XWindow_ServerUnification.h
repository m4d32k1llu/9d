// XWindow_ServerUnification.h: interface for the _XWindow_ServerUnification class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_SERVERUNIFICATION_H__BF83032F_B6C8_4662_AE35_FA64F1CEB4C0__INCLUDED_)
#define AFX_XWINDOW_SERVERUNIFICATION_H__BF83032F_B6C8_4662_AE35_FA64F1CEB4C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XNetwork.h"
#include "packet_for_unigrouplobby.h"

/*! �������� �ܰ� �з�
	- ���ǻ���
	- ���� ����
	- â�� �� ĳ���� ����
	- ĳ���͸� �ߺ� üũ
	- �Ϸ�
*/

enum UNIFICATIONSTEP 
{ 
	SU_READRULE = 0,		
	SU_SELECTSERVER,		
	SU_SELECTCHARAC,
	SU_CHECKCHARAC,			
	SU_FINISH,				
};

//! ���� �޴� �ؽ�Ʈ ���� 
struct ServerUnificationInfo 
{
	TCHAR	Title[128];
	TCHAR	Infomation[10][256];
	char	InfomationLineCount;

	void reset()
	{		
		memset( Title, 0, sizeof(TCHAR) * 128 );
		memset( Infomation, 0, sizeof(TCHAR) * 10 * 256 );
		InfomationLineCount = 0;
	};
};

class _XWindow_ServerUnification : public _XWindow  
{
private:
	_XImageStatic				m_MenuEdge_Top;
	_XImageStatic				m_MenuEdge_Bottom;
	_XImageStatic				m_MenuEdge_Left;
	_XImageStatic				m_MenuEdge_Right;
	
	_XCheckButton*				m_checkbtnList[12];

	_XButton					m_NextButton;
	_XButton					m_PrevButton;
	_XButton					m_ExitButton;
	_XButton*					m_ChangeButton[3];
	//_XButton*					m_CompleteButton;

	_XImageStatic				m_TitleLeft;
	_XImageStatic				m_TitleRight;

	_XImageStatic				m_BoxLeft;
	_XImageStatic				m_BoxCenter;
	_XImageStatic				m_BoxRight;

	_XImageStatic				m_WidthLine;
	_XImageStatic				m_HeightLine;	

	_XImageStatic				m_ServerName;	

	UNIFICATIONSTEP				m_UnificationStep;

	ServerUnificationInfo*		m_TextInfo;
	SIZE						m_StepWindowSize;
	TCHAR						m_UnificationServerName[3][128];

	_XImageStatic				m_SelectBorderBox;
	_XImageStatic				m_SelectWarehouseBox;

	//! ��� ������ ����
	MSG_UG_SIMPLE_GROUP_INFO	m_Simple_Group_Info;		
	//! ������ ����
	int							m_SelectServerIndex;		
	//! ���õ� ������ ����
	MSG_UG_DETAIL_GRP_INFO		m_Detail_Group_Info;		
	//! ���õ� â�� ����
	int							m_SelectWarehouseIndex;		
	//! ���õ� ĳ��
	int							m_SelectCharacterIndex[3];	
	//! ���� ���
	MSG_UG_MOVE_RESULT_COMPLETE m_MoveResult;					
	
public:
	_XWindow_ServerUnification();
	virtual ~_XWindow_ServerUnification();

	BOOL				Initialize(_XTextureManager& texturearchive1, _XTextureManager& texturearchive2);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void				MoveWindow(int X, int Y);
	void				SetUnificationStep(UNIFICATIONSTEP step);
	UNIFICATIONSTEP		GetUnificationStep(void) { return m_UnificationStep; };

	void				DrawReadRule(void);
	void				DrawSelectServer(void);
	void				DrawSelectCharac(void);
	void				DrawCheckCharac(void);
	void				DrawFinish(void);

	//! ���õ� ����
	int					GetSelectServerIndex(void) { return m_SelectServerIndex; 	};
	//! ���õ� â��
	int					GetSelectWarehouseIndex(void) { return m_SelectWarehouseIndex; 	};
	int*				GetSelectCharacterIndex(void) { return m_SelectCharacterIndex; };
	//! ���õ� ĳ���� ��
	BOOL				GetSelectCharacterCount(void);

	//! ���� - �⺻����
	void				SetSimpleGroupInfo(MSG_UG_SIMPLE_GROUP_INFO* pInfo);
	//! ���� - ������
	void				SetDetailGroupInfo(MSG_UG_DETAIL_GRP_INFO* pInfo);
	//! ���� - ĳ���̵� ���
	void				SetMoveResult(MSG_UG_MOVE_RESULT_COMPLETE* pInfo);
	//! ���� - ĳ���̸� ���� ���
	void				SetChangeNameResult(MSG_UG_CHANGE_NAME_RESULT* pInfo);
	//! üũ��ư Ŭ�� ó��
	void				ClickCheckButton(char index);
};

#endif // !defined(AFX_XWINDOW_SERVERUNIFICATION_H__BF83032F_B6C8_4662_AE35_FA64F1CEB4C0__INCLUDED_)

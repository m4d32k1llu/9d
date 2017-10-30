// XWindow_TutorialIcon.h: interface for the _XWindow_TutorialIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TUTORIALICON_H__5BCB6A0F_7DB0_474D_B3DB_E37AB10B7A4E__INCLUDED_)
#define AFX_XWINDOW_TUTORIALICON_H__5BCB6A0F_7DB0_474D_B3DB_E37AB10B7A4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include <list>
using namespace std;

/**
 * \brief Ʃ�丮�� ������
 * �߻����ǿ� ������ �� ��Ÿ���� �� ������ 
 * 
 * �Ϲ� ȭ��� npc wide ȭ�� �� ������ ��Ÿ�� �� ����
 * _MAX_TUTORIALICON ������ ������ (cpp�� ����)
 * Ʃ�丮���� ���������� ǥ�õǴ� �Ϲ� Ʃ�丮��� 
 * ������ ���� �ٷ� â�� �ߴ� ���� Ʃ�丮���� ����.
 * Ʃ�丮�� Ŭ�������� ������ pc�� ���̵�� ����ȴ�.
 * npcmode Ʃ�丮�� �������� �Ϲ�ȭ������ ���ƿ��� �� �����.
*/

class _XWindow_ChattingDefault;

/// �ϳ��� icon Data
typedef struct tagTutorialData
{
	DWORD winId;
	_XWindow* winPtr;
} _XTUTORIALDATA;

class _XWindow_TutorialIcon : public _XWindow  
{
private:
	_XButton*						m_BtnTutorialIcon[8];
	_XWindow_ChattingDefault*		m_pChattingDefault_Window;
	int								m_ExternalTutorialCount;		///< �Ϲ� ȭ�� ���� 
	int								m_InternalTutorialCount;		///< npcmode ȭ�� ����
	int								m_EffectCount;
	DWORD							m_EffectTime;

	list<_XTUTORIALTYPE> 			m_ExternalTutorialIconLIst;		///< �Ϲ� ȭ�� Ʃ�丮�� Ÿ�� ����Ʈ
	list<_XTUTORIALTYPE> 			m_InternalTutorialIconLIst;		///< npcmode ȭ�� Ʃ�丮�� Ÿ�� ����Ʈ

	list<_XTUTORIALDATA*> 			m_ExternalTutorialDialogLIst;	///< �Ϲ� ȭ�� Ʃ�丮�� ����Ÿ ����Ʈ
	list<_XTUTORIALDATA*>			m_InternalTutorialDialogLIst;	///< npcmode ȭ�� Ʃ�丮�� ����Ÿ ����Ʈ

	void				SaveTutorialData(_XTUTORIALTYPE type);
	void				AllClearInternalTutorialIconList();			///< npcmode ��� Ʃ�丮�� â �ݱ�	
	
public:
	_XWindow_TutorialIcon();
	virtual ~_XWindow_TutorialIcon();

	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void				AllTutorialDraw();
	void				AllTutorialProcess();
	
	void				InsertTutorialIcon( _XTUTORIALTYPE type );	///< Ʃ�丮�� ������ �߰�
	void				RemoveTutorialIcon( _XTUTORIALTYPE type );	///< icon ���� Ʃ�丮�� Ŭ����
	void				RemoveTutorialIcon( int index );			///< icon �ִ� Ʃ�丮�� Ŭ����

	void				SetChattingDefaultWindowPtr( _XWindow_ChattingDefault* windowptr) { m_pChattingDefault_Window = windowptr;	};
	void				DeleteTutorialIDlgList( DWORD winid);	///< �ش� Ʃ�丮�� â ����
	
	void				AllCloseExternalTutorialWindow();
	void				AllOpenExternalTutorialWindow();
	void				ResetTutorialData();
	
	BOOL				EscapeKeyDown();	///< Ʃ�丮�� â �ݱ�
};

#endif // !defined(AFX_XWINDOW_TUTORIALICON_H__5BCB6A0F_7DB0_474D_B3DB_E37AB10B7A4E__INCLUDED_)

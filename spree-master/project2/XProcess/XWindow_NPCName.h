// XWindow_NPCName.h: interface for the _XWindow_NPCName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NPCNAME_H__79754000_B21F_4710_85DE_775CDDFEE946__INCLUDED_)
#define AFX_XWINDOW_NPCNAME_H__79754000_B21F_4710_85DE_775CDDFEE946__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#define _XDEF_MAXNPCNAMESTRINGSIZE		32

typedef enum _XNPCNameMode
{
	_XNPCNAMEMODE_NPC		= 0,
		_XNPCNAMEMODE_PCPERSONALSTORE,
		_XNPCNAMEMODE_PCMATCHPRMODE

		
}_XNPCNAME_MODE;

class _XWindow_NPCName 
{
public:
	POINT			m_WindowPosition;
	SIZE			m_WindowSize;

	_XImageStatic*	m_imageMatchPRLeftSide;				// �� ȫ���� ��Ÿ���� ������ ���� ���� 
	_XImageStatic*	m_imageMatchPRRightSide;			// �� ȫ���� ��Ÿ���� ������ ������ ���� 
	_XImageStatic*	m_imageMatchPRBorder;				// �� ȫ���� ��Ÿ���� ������ ���� 
	_XImageStatic*	m_imageMatchPRIcon;					// �� ȫ���� ��Ÿ���� ������
	
	_XImageStatic*	m_imagePersonalStoreLeftSide;		// ������ ��Ÿ���� ������ ���� ���� 
	_XImageStatic*	m_imagePersonalStoreRightSide;		// ������ ��Ÿ���� ������ ������ ���� 
	_XImageStatic*	m_imagePersonalStoreBorder;			// ������ ��Ÿ���� ������ ���� 
	_XImageStatic*	m_imagePersonalStoreIcon;			// ������ ��Ÿ���� ������
	_XImageStatic*	m_imageEventIcon01;					// ����ǹ��� �̺�Ʈ ������
	_XImageStatic*	m_imageEventIcon02;					// �ѹи���Ʈ �̺�Ʈ ������
	_XImageStatic*	m_imageEventIcon03;					// ���డ�� �̺�Ʈ ������
	_XImageStatic*	m_imageEventIcon04;					// ���ȭ �̺�Ʈ ������
	
#ifdef	_XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130 
	_XImageStatic*	m_imageCastleCaptainBorder;			// ���� NPC â �̹���
#endif
	
	_XImageStatic*	m_imageIconBorder;					// NPC�� ������ ��Ÿ���� �������� ����  
	_XImageStatic*	m_imageNPCNameBorder;				// NPC�� �̸� ������
	_XImageStatic*	m_imageNPCJobIcon;					// NPC�� ������ ��Ÿ���� ������

	_XImageStatic*	m_YellowImpressionMark;
	_XImageStatic*	m_GreenImpressionMark;
	_XImageStatic*	m_BlueImpressionMark;
	_XImageStatic*	m_OrangeImpressionMark;
	_XImageStatic*	m_WhiteImpressionMark;
	
	_XImageStatic*	m_YellowQuestionMark;
	_XImageStatic*	m_GreenQuestionMark;
	_XImageStatic*	m_BlueQuestionMark;
	_XImageStatic*	m_OrangeQuestionMark;
	_XImageStatic*	m_WhiteQuestionMark;
	
	TCHAR			m_NPCOccupationString[_XDEF_MAXNPCNAMESTRINGSIZE];
	TCHAR			m_NPCNameString[_XDEF_MAXNPCNAMESTRINGSIZE];
	
	int				m_EventType;	//�̺�Ʈ ���� 0:������-�йи���Ʈ 1:�������� 2:������߱�
public:
	_XWindow_NPCName();
	virtual ~_XWindow_NPCName();
	
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void Draw( int X, int Y );
	void Draw( int X, int Y, LPTSTR pNPCOccupationStr, LPTSTR pNPCNameStr, BOOL selected, _XNPCQUESTTYPE questtype, _XNPCJOBCLASS jobclass, _XNPCNAME_MODE npcnamemode = _XNPCNAMEMODE_NPC, _XFONT_ALIGNTYPE aligntype = _XFONT_ALIGNTYPE_CENTER );
	void MoveWindow( int X, int Y );
	
	void SetNPCName( LPTSTR pNPCOccupationStr, LPTSTR pNPCNameStr );
	void SetNPCJobIcon( _XNPCJOBCLASS jobclass );
};

#endif // !defined(AFX_XWINDOW_NPCNAME_H__79754000_B21F_4710_85DE_775CDDFEE946__INCLUDED_)

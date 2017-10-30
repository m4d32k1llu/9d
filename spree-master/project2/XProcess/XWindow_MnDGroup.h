// XWindow_MnDGroup.h: interface for the XWindow_MnDGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MNDGROUP_H__4B800B93_0192_4875_A69D_CD7A2076B592__INCLUDED_)
#define AFX_XWINDOW_MNDGROUP_H__4B800B93_0192_4875_A69D_CD7A2076B592__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMnDStructInfo.h"
#include <vector>
#include <map>
using namespace std;

/// ������ ���� ����
struct MND_NOTICEINFO
{
	TCHAR		writer[13];
	TCHAR		writedate[15];
	TCHAR		notice[272];

	MND_NOTICEINFO::MND_NOTICEINFO()
	{
		memset( writer, 0, sizeof(writer) );
		memset( writedate, 0, sizeof(writedate) );
		memset( notice, 0, sizeof(notice) );
	}

	void reset()
	{
		memset( writer, 0, sizeof(writer) );
		memset( writedate, 0, sizeof(writedate) );
		memset( notice, 0, sizeof(notice) );
	} ;

} ;

typedef enum
{
	_XMNDNOTICEMODE_READ		= 0,
	_XMNDNOTICEMODE_WRITE,
	_XMNDNOTICEMODE_MODIFY
}_XMnDNoticeMode;

class _XWindow_MnDGroup
{
public:
	_XWindow_MnDGroup();
	~_XWindow_MnDGroup();

	BOOL Initialize();
	void ShowMnDWindow(BOOL bShow);
	void ShowPopupWindow(BOOL bShow);

	void Draw(int nX, int nY);
	void Process();
	void MoveWindow();
	BOOL ProcessWheelMessage( short zDelta );

	void CalculateNoticeTextength(); //���� ����
	void SelectNoticeMode( _XMnDNoticeMode mode);	///< ������ ������� ���� - �б�, ����, ����
	void CopySendNoticeText( void );	///< �ۼ��� �������� ������ Send	
	void SetNoticeText(TCHAR* text, bool modifymode = false );	///< �������� ���� (�������� �޾��� �� , ���� ���� �� )

	void SetListItem(); //���� ����Ʈ
	void ResetData();

private:
	_XStickGauge*			m_pCotributionBorder;
	_XImageStatic*			m_pContributionGaugeLeftBorder;
	_XImageStatic*			m_pContributionGaugeRightBorder;	
	
	_XImageStatic*			m_pImageContributionRateLeftBorder;		// �ϴ� �⿩�� â
	_XImageStatic*			m_pImageContributionRateRightBorder;	// �ϴ� �⿩�� â

	_XListBox*				m_pMnDInfoListBox;

	_XButton*				m_pNoticeWrite;			//���� �ۼ� ��ư
	_XButton*				m_pNoticemodify;		//���� ���� ��ư
	_XButton*				m_pBtnGroupLeave;		//�ϻ� ��ư

	TCHAR					m_szString[8][128];//�Ϲ����� �۾�

	map<int, int> m_mpLastCotribution; //���� 

public :
	_XButton*				m_pPopUpButton[4];		//�˾� ��ư

	_XVScrollBar*			m_pNoticeScrollBar; //����â ��ũ�ѹ�
	_XIMEContainer			m_MnDNoticeIMEControl; //���� ���� 

	int m_nPopupCount; //����Ʈ Ŭ�� �� �� ��� ��ư�� �ߴ���
	int m_nNoticeScrollPos; //�������� ��ũ�� ��ġ
	int m_nNoticeLength; //�������� ����
	int m_nNoticeTotalCount; //�������� ����ī��Ʈ ��������
	int m_nCurrentNoticeLinePos; //���� ���° ������
	int m_nNoticeViewTextTotalCount; //��ü �������� ���� �ټ�

	_XMnDNoticeMode m_nNoticeMode;
	MND_NOTICEINFO	m_stNoticeText;			///< ���� ����Ǿ��ִ� ������ ����	
	MND_NOTICEINFO	m_stSendNoticeTempText;	///< ������ ������ ���� 
	TCHAR					m_szNoticeTempText[256][128]; //�ӽ� ���� ���� ����
	TCHAR					m_szNoticeViewText[256][128];	//Ȯ���� ��������

	vector<MnDGroupList*>	m_vtMnDGroup;
};

#endif // !defined(AFX_XWINDOW_MNDGROUP_H__4B800B93_0192_4875_A69D_CD7A2076B592__INCLUDED_)

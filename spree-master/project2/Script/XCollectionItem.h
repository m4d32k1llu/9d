// XCollectionItem.h: interface for the _XCollectionItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCOLLECTIONITEM_H__819D0878_D799_43D2_9057_72386E314D09__INCLUDED_)
#define AFX_XCOLLECTIONITEM_H__819D0878_D799_43D2_9057_72386E314D09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XGameItem.h"

#include <list>
using namespace std;

// ��ǰ�������� ������ ��Ÿ���� ���� Ŭ����
class _XCollectionRewardSet
{
public:
	int		m_Index;											// ���� ��Ʈ �ε���
	list <_XCollectionResourceItem*>		m_listRewardItem;	// ���������
public:
	_XCollectionRewardSet();
	virtual	~_XCollectionRewardSet();
		
	void	ReleaseRewardItem(void);
	void	InsertRewardItem(  char ctype ,short id, unsigned char count, int index);
};

#if defined(_XENGLISH) || defined(_XRUSSIAN)
#define _XDEF_MAX_COLLECTION_LINE	10
#else
#define _XDEF_MAX_COLLECTION_LINE	3
#endif

// ��ǰ�������� ���
class _XCollectionItem  
{
public:

	int		m_indexCollectionList;			// ���° ����Ʈ���� �� �� �ִ� �ε��� 
	int		m_CollectionIndex;				// ��ǰ���� �����ε��� 
	int		m_NeedItemTotalCount;			// �����ؾ� �� ������ ���� �� ����
	int		m_CompleteItemCurrentCount;		// ���� �Ϸ��� ������ ���� ����
	int		m_CollectionMinimumLevel;		// ������ �� �� �ִ� �ּ� ����
	int		m_PrecedeCollectionIndex[5];	// �������� �Ϸ��ؾ��� ���� �ε���
	short	m_CollectionVersion;			// ���� ����
	short	m_CollectionType;				// ����Ÿ��: ��ǰ,��ǰ,��ǰ,��ǰ
	bool	m_bEnableExchage;				// ���� �ؾ��� �������� �� ��Ƽ� ��ȯ������ ����
	bool	m_bComplete;					// ��ȯ���� �Ϸ��ؼ� ���̻� ���� �� �� ���� ����

#ifdef _XTS_NEWLIFEWINDOW
	LPTSTR	m_CollectionName;
#else
	TCHAR	m_CollectionName[128];			// ��ǰ������ : ���׼���..
#endif
	
#ifdef _XTS_NEWLIFEWINDOW
	LPTSTR	m_CollectionExplain;
	LPTSTR	m_TempString[_XDEF_MAX_COLLECTION_LINE];
#else
	TCHAR	m_CollectionExplain[_XDEF_MAX_COLLECTION_LINE][512];	// ��ǰ������ ���� ������ ��Ÿ����.
#endif

	int		m_CollectionExplainLineCount;

	
	list <_XCollectionResourceItem*>	m_listNeedItem;		// ��ǰ���� �ٲٱ� ���ؼ� �ʿ��� ������ ����Ʈ
	list <_XCollectionRewardSet*>		m_listRewardSet;	// ���� ��Ʈ 
	

public:
	_XCollectionItem();
	virtual ~_XCollectionItem();

	void	Release(void);
	void	InsertNeedItem(  char ctype ,short id, unsigned char count, int index );


};




#endif // !defined(AFX_XCOLLECTIONITEM_H__819D0878_D799_43D2_9057_72386E314D09__INCLUDED_)

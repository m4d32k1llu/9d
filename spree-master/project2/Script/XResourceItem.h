// XResourceItem.h: interface for the _XResourceItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XRESOURCEITEM_H__D4E53F12_C036_4160_955C_BE13EA291762__INCLUDED_)
#define AFX_XRESOURCEITEM_H__D4E53F12_C036_4160_955C_BE13EA291762__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XGameItem.h"

#include <list>
using namespace std;
// �ڿ��������� ���� �ٲپ� �ִ� ������ ������ �ִ� Ŭ����
class _XResourceItem  
{			
public:
	int		m_ResourceIndex;	
	int		m_currentcountNeeditem;			// ���� �Ϸ�� �ʿ��� ������ ����
	int		m_totalcountNeeditem;			// �ʿ��� ������ �� ����
	TCHAR	m_ResourceName[256];				// ��� �̸�
		
	_XUserItem	m_RewardItem;				// ���� ������ 
	list <_XCollectionResourceItem*>	m_listNeedItem;		// ���� �ٲٱ� ���ؼ� �ʿ��� ������ ����Ʈ

public:
	_XResourceItem();
	virtual ~_XResourceItem();

	void	Release(void);

};

// ��Ḯ��Ʈ �ٷ���
class _XResourcePackage  
{			
public:
	int			m_ResourcePackageIndex;				
	list <int>	m_listResourceItemIndex;	// ��� �ε����� ������ �ִ� ����Ʈ
	
public:
	_XResourcePackage();
	virtual ~_XResourcePackage();
	
	void	Release(void);
	
};


class _XResourceGroupItem
{			
public:
	int			m_ResourceGroupItemIndex;			// ��з��ε���		�뵿����, ������ ���� ...
	int			m_ResourceGroupSubItemIndex;		// �Һз��ε���		���׻��� ���ѱ�, ������� �ڱ��...
	int			m_ResourcePackageIndex;				// ��� ��Ű�� ���� �ε���
	bool		m_bSelectedGroupSubItem;			// ����Ʈ���� ���õǾ������� �� �� �ִ�.
	TCHAR		m_ResourceGroupSubItemName[256];	// ��� �̸�
		
public:
	_XResourceGroupItem();
	virtual ~_XResourceGroupItem();
	
	void	Release(void);
	
};

#endif // !defined(AFX_XRESOURCEITEM_H__D4E53F12_C036_4160_955C_BE13EA291762__INCLUDED_)

// XSetItemInfomation.h: interface for the XSetItemInfomation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSETITEMINFOMATION_H__99F6C2C5_9051_4BD5_9C4F_9D13682A61C2__INCLUDED_)
#define AFX_XSETITEMINFOMATION_H__99F6C2C5_9051_4BD5_9C4F_9D13682A61C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>
#include <list>
using namespace std;

#include "XWindow_DefaultTooltip.h"

class XSetItemInfomation  
{
public:
	XSetItemInfomation(void);
	~XSetItemInfomation(void);

	HRESULT LoadXMLTable( XMLPaser& XMLPaser, LPTSTR pTableName );
	
	void GroupItemSkillUpdate(); //��ų ������Ʈ �Ҷ� ����.
	
	int SearchGroupItem(int nGroup); //�ش� �׷��� �������� ��� �����ϰ� �ִ���
	int SetGroupSkillString( int nGroup, int nAccouter, SIZE TooltipSize, _ITEMTOOLTIP_INFO& TooltipInfo ); //�ش� �׷��� ���� ������ ��Ʈ���� �������� //������ ���δ� //���ϰ��� ��������
	int SetGroupItemString( int nGroup, _ITEMTOOLTIP_INFO& TooltipInfo ); // �ش� �׷��� ��Ʈ�� //������ ���δ� //���ϰ��� ���� ����
	void DrawToolTip( int nGroup, int DrawX, int DrawY, _ITEMTOOLTIP_INFO& TooltipInfo ); //���� ���� ǥ�� ������ �̷��� �ؾ��ϳ�,, ����� ���� �ƾƾ� ���ľ��Ѵ�..
    void DrawSkillToolTip( int nGroup, int DrawX, int DrawY, SIZE TooltipSize, int &nCurrentDrawYModify, _ITEMTOOLTIP_INFO& TooltipInfo ); //��ų ������ ���α׷���� �ϹǷ�..
	
	bool GetWearItem( int nGroup, int nType, int nID ); // ���� ���� �԰��ִ°�?? 

	void UpdateAnotherUserEffect( _XUser* pUser, int nEffectID, int nEffectID1 ); // �ٸ� �ɸ����� ����Ʈ

public :
	const int GetSetItemAccoutered() const { return m_nAccouteredItem; }
	const int GetSkillStringCount() const { return m_nSkillStringCount; }
	const int GetSetItemStringCount() const { return m_nSetItemStringCount; }

private :
	void UpdateGroupSkillInfo(int nGroup, int nAccouter ); // ��ų ���� �׷� ���̵�, ���� ����

private:
	struct ItemInfo
	{
		int nType; //�������� Ÿ��
		int nID; //�������� ���̵�
	};

	struct SkillInfo
	{
		int nSkillID; //��ų ���̵�
		int nSkillValue; //��ų ����
	};
	
	struct ItemDetailInfo
	{
		int nAccouteredItem; //������ ������ ������ ����
        int nEffectID; //����Ʈ ���̵�
		list<SkillInfo*> ltSkillInfo; //��ų ����

		~ItemDetailInfo()
		{
			list<SkillInfo*>::iterator iter;

			for( iter = ltSkillInfo.begin(); iter != ltSkillInfo.end(); ++iter )
			{
				delete (*iter);
				(*iter) = NULL;
			}
			ltSkillInfo.clear();
		}
	};

	struct SetInfo
	{
		int nGroupID; // �׷� ���̵� ������
		vector<ItemInfo*> vtItem; // �������� ����
		list<ItemDetailInfo*> ltItemInfo; //������ ������ ���� ����Ʈ�� Ʋ���� // ��ų�� ������..

		~SetInfo()
		{
			nGroupID = 0;
			vector<ItemInfo*>::iterator vtiter = vtItem.begin();
			list<ItemDetailInfo*>::iterator ltiter = ltItemInfo.begin();

			for(; vtiter != vtItem.end(); ++vtiter )
			{
				delete (*vtiter);
				(*vtiter) = NULL;
			}

			for(; ltiter != ltItemInfo.end(); ++ltiter )
			{
				delete (*ltiter);
				(*ltiter) = NULL;
			}

			vtItem.clear();
			ltItemInfo.clear();
		}
	};

private: //�־��� �ҽ��� ���Ͽ�..
	//������ ���� �� ���� ���ŵ�
    int m_nAccouteredItem; //������ ������ ����
	int m_nSkillStringCount; //��ų ��Ʈ�� ����
	int m_nSetItemStringCount; //��Ʈ ������ ��Ʈ�� ���� 

	map<int, SetInfo*>	m_mpData; //�׷���̵�, �׷�����
	list<ItemInfo> m_ltWearItem; //��ä ������ ������ ���� ����// �� �̰� ���� �ȵǴµ�.. //���� ���� ������ ���δ�..
	
	list<int> m_ltEffectID; // ��Ʈ������ ����Ʈ ����.. Update �� �������ش�
	map<int, int> m_mpSkillInfo; //�׷���̵�, �ش�׷� ���� � �����ߴ°�.. //��ų������Ʈ�Ҷ� ����
};

#endif // !defined(AFX_XSETITEMINFOMATION_H__99F6C2C5_9051_4BD5_9C4F_9D13682A61C2__INCLUDED_)

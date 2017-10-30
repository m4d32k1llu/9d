#pragma once
#include <algorithm>

//V ���� â���� ���̴� ����
struct MnDGroupList
{
	int nMnDClass; //����, �ϵ�����,, �̷��� ǥ��
	TCHAR szName[64]; //�ɸ��� ���̵�
	int nLevel; //���� ����
	DWORD nDiscipleContribution; //���� ���ڵ� �⿩��
	int nCurrentZone; //�� �ε��� ��ȣ.. ��� ������ �ִ���
	int nPlayerState; //�÷��̾� ���� ���� ������,, �׷���

	MnDGroupList::MnDGroupList()
	{
		nMnDClass = 0;
		memset(szName, 0, sizeof(szName));
		nLevel = 0;
		nDiscipleContribution = 0;
		nCurrentZone = 0;
		nPlayerState = 0;
	}

	bool operator< ( const MnDGroupList* rhs )
	{
		return nMnDClass < rhs->nMnDClass;
	}

};

//���� �ҷ��� ���ǵ�,, �ɹ� ���ø��� VC6 �� ����� ������ ���� ���Ѵ�..
//�׳� ��� �ϸ�� STL �� ������ ��ũ���� �׷� static ���� �ؾ��ϳ�??
template<typename T, T MnDGroupList::*p>
bool MnDGroupSort( MnDGroupList* lhs, MnDGroupList* rhs )
{
	return lhs->*p < rhs->*p;
}

static bool MnDGroupClassSort( MnDGroupList* lhs, MnDGroupList* rhs ) //���� ���� ������ ����
{
	return lhs->nMnDClass < rhs->nMnDClass;
}

//NPC ��������â, �޽��� ���� ��ȭâ�� ���������� ���̾���
struct MasterInfo
{
	int nRank; //���
	TCHAR szMasterNickName[64]; // ��ȣ
	TCHAR szMasterName[64]; // ���� �̸�
	int nMoonpaindex; //���� ����
	int nClassInfo; //��å ����
	int nOrganizationClass; //������ �������� �ܿ����� ����

	MasterInfo::MasterInfo()
	{
		nRank = 0;
		memset( szMasterNickName, 0, sizeof(szMasterNickName));
		memset( szMasterName, 0, sizeof(szMasterName));
		nMoonpaindex = 0;
		nClassInfo = 0;
		nOrganizationClass = 0;
	}
};

//���� ���� �޽��� ��ȭ�忡 ���� ����
struct DiscipleInfo
{
	int nLevel; //����
	TCHAR szName[64]; //�ɸ��� ���̵�
	int nStartIndex; //���� ���� ���ڷ� �޾Ƽ� ��Ʈ������ ǥ��
	int nClassInfo; //��å ����

	DiscipleInfo::DiscipleInfo()
	{
		nLevel = 0;
		memset( szName, 0, sizeof(szName));
		nStartIndex = 0;
		nClassInfo = 0;
	}
};
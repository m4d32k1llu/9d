#pragma once
#include <string>
#include <set>

using namespace std;


/*
ip ���͸� 
*/
class CIpFilter
{
private:
	// ���͸��� ������
	set<string>	m_IpFilterList ;	

public:

	CIpFilter( const char * fileName /*ip list file*/)
	{
		if( IpListFileRead(fileName) == false )
		{
			throw ;
		}		
	}
	~CIpFilter()
	{
	}
	
	// ���͸��� ������ ����Ʈ�� �д´�.
	bool IpListFileRead( const char * cFileName ) ;

	// ���͸��� �����Ǹ� �߰��Ѵ�.
	void InsertIp( char * ip )
	{
		//m_IpFilterList[ip] = 0 ;
		m_IpFilterList.insert(ip) ;
	}

	// ���͸��� ���������� �˻��Ѵ�.
	bool IsFilterIP( char * ip )
	{
		try{
			set<string>::iterator itr = m_IpFilterList.find(ip) ;
			if( itr == m_IpFilterList.end() ) 
				return false ;
			else 
				return true ;
		}
		catch (...) {
		}
		return false ;
	}
};



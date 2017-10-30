#include "..\stdafx.h"
#include "VMManager.h"

bool _h_VMManager::Create( unsigned int MegaSize )
{
	if( m_pStartAddr )		// create �Լ� �ߺ� ȣ�� üũ
	{

		printf( "[ME_ERR] ���� �޸��� Create �� 2�� �̻� ȣ���Ͽ����ϴ�.\n" ) ;


		return false ;
	}

	if( MegaSize > 400U )	// �ʹ� ū �޸� ��� üũ
		return false ;
	// ���� �޸𸮸� ������ �д�. 
	m_pStartAddr = VirtualAlloc( NULL, MegaSize * _MEGA_, MEM_RESERVE, PAGE_READWRITE ) ;

	
	// VirtualAlloc�� ���Ͽ� ������ �Ҵ�� �޸� ������ ���� ���� ////
	// ������ ���� Ȯ���Ѵ�. ���⼭�� �Ҵ�� ũ�⸸�� Ȯ���Ͽ���.
	MEMORY_BASIC_INFORMATION memoryinfo;
	VirtualQuery(m_pStartAddr, &memoryinfo, sizeof(memoryinfo));	
	printf("-> Memory zone Reserved (Bytes:%d Mega:%d Pages:%d )\n",
		(UINT)(memoryinfo.RegionSize),(UINT)(memoryinfo.RegionSize)/_MEGA_,(UINT)(memoryinfo.RegionSize/4096));
	////////////////////////////////////////////////////////////////////

	if( m_pStartAddr )
	{
		m_uiReserveSize = MegaSize * _MEGA_ ;
		m_pCurrAddr = m_pStartAddr ;
		return true ;
	}
	else
		return false ;
}

void * _h_VMManager::MemoryCommit( unsigned int Size )
{
	void * pVoid = NULL ;
	UINT realSize ;			// Size �� ���Ͽ�, ������ Ŀ���� �޸𸮸� ����ϰ�, �� ���� ����Ѵ�. 
	int temp ;
	UINT checkSize ;
	
	temp = Size % _PAGE_SIZE_ ;
	
	if( temp != 0 )
	{
		realSize = Size - temp + _PAGE_SIZE_ ;
	}
	else
	{
		realSize = Size ;
	}

	checkSize = m_uiCurrSize + realSize ;
	if( m_uiReserveSize < checkSize )
	{
		printf( "[ME_ERR] ����� ����� ��� ����Ͽ����ϴ�.\n" ) ;

		return NULL ;
	}

	pVoid = VirtualAlloc( m_pCurrAddr, realSize, MEM_COMMIT, PAGE_READWRITE ) ;
	if( pVoid )
	{
		memset( pVoid, 0, realSize ) ;
		m_pCurrAddr = (PBYTE)m_pCurrAddr + realSize ;
		m_uiCurrSize = checkSize ;

		printf( "[INFO] MEM_COMMIT : Reserve = %u, Used = %u, CurrAddr = 0x%X\n", m_uiReserveSize, m_uiCurrSize, pVoid ) ;		
		
	}
	
	return pVoid ;		// ���ϰ��� null �̸�, ������. 
}

void _h_VMManager::Destroy()
{
	if( m_pStartAddr )
	{
		if( VirtualFree( m_pStartAddr, m_uiReserveSize * _MEGA_, MEM_DECOMMIT ) == 0 )
			return ;

		VirtualFree( m_pStartAddr, 0, MEM_RELEASE ) ;
	}
}
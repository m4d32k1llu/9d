#ifdef _XDEF_NEXONLOGIN

#ifndef __NMCONEW_H_7E9B922A_A6D1_4865_93DD_69B59E27CF1C__
#define __NMCONEW_H_7E9B922A_A6D1_4865_93DD_69B59E27CF1C__

#ifdef _DLL_EXPORT
#define DllDecl	__declspec( dllexport )
#else
#define DllDecl	__declspec( dllimport )
#endif

extern "C"
{
	/*
	uFuncCode		: ȣ���ϴ� CNMFunc�� �ڵ�
	pCallingData	: Serialize�� ȣ�� ������
	ppReturnData	: ����� �������� ������ ������, ���۰� ������� ��� �����Ҵ��ؼ� ���ϵ�
	uReturnDataLen	: ȣ��� ��� ������ ������
					���Ͻ� ����� ���Ͽ� ����� �����ǻ�����
	����			: ���� ������ ũ�Ⱑ ���� ���� �����Ϳ� ���Ͽ� ���� ���
					���� �޸𸮸� �Ҵ��� ����� �����Ѵ�. �̶� ppReturnData
					���� ó�� ȣ��ÿ� �޶����� �Ǹ�, �̷��� ���� �Ҵ�Ǿ�
					�Ѿ �޸𸮴� NMCO_MemoryFree�� �̿��� ������ �־�� �Ѵ�.
	*/
	BOOL DllDecl NMCO_CallNMFunc(	IN		UINT32	uFuncCode,
									IN		BYTE*	pCallingData,
									IN OUT	BYTE**	ppReturnData,
									IN OUT	UINT32&	uReturnDataLen
									);

	void DllDecl NMCO_MemoryFree( IN LPVOID pBuffer );

	//
	//	��� ��ġ ���θ� �����մϴ�
	//	(�⺻�����δ� ��ġ�� �մϴ�)
	//
	BOOL DllDecl NMCO_SetPatchOption( IN BOOL bPatch );

	//
	//	ģ�� ���� ��� ���θ� �����մϴ�
	//	(�⺻�����δ� ���� �ֽ��ϴ�)
	//
	BOOL DllDecl NMCO_SetUseFriendModuleOption( IN BOOL bUserFriendModule );

	//
	//	��ġ �� ���, ���� ������ �����մϴ�
	//	(�⺻�����δ� �ѱ� ������ �����˴ϴ�)
	//
	BOOL DllDecl NMCO_SetVersionFileUrl( IN LPCTSTR pszVersionUrl );

	//
	//	NGM �� �̿��ؼ� ��ġ �մϴ�
	//	(�⺻�����δ� ���� �ֽ��ϴ�)
	//
	BOOL DllDecl NMCO_SetUseNGMOption( IN BOOL bUseNGM );

	//
	//	�޽����� ������ ������ ���� �ڵ带 �����մϴ�
	//
	//		-	�⺻�����δ� �ѱ����� �����ϵ��� �����Ǿ� �ֽ��ϴ�
	//
	BOOL DllDecl NMCO_SetLocale( IN UINT32 uLocaleID, IN UINT32 uRegionCode );
}

#endif	//	#ifndef __NMCONEW_H_7E9B922A_A6D1_4865_93DD_69B59E27CF1C__

#endif
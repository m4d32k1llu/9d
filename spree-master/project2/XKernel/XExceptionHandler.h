// XExceptionHandler.h : declaration of the _XExceptionHandler namespace
//
// Created Date : 2003.08.12
// Author		: Sohyun, Park
// DESC			: Exception Handler
//
//////////////////////////////////////////////////////////////////////

#ifndef __XEXCEPTION_HANDLER_H__
#define __XEXCEPTION_HANDLER_H__

#include <windows.h>

/** ����ó�� ������ ���� namespace.
 *
 * ����ó�� ���.
 * \author Sohyun, Park
 * \date 2003.08.18
 */

namespace _XExceptionHandler
{

	/** ����ó���� main �Լ�.
	 * Exception�� �Ͼ� ������, Exception code�� Exception address�� �޾Ƽ� �� ������ EOD.log�� ����Ѵ�.
	 * EOD.log�� ���� ���� �־�� �Ѵ�.
	 * \param [IN] DWORD _code : ���� Code.
	 * \param [IN] DWORD _address : ���ܰ� �Ͼ ���� ���ּ�.
	 * \todo Call Stack�� Register ���� ���.
	 */
	void	GenerateExceptionInformation(DWORD _code, const void* _address);

	/** ����ó�� String ���.
	 * �����ڵ忡 ���� String�� EOD.log�� ����Ѵ�.
	 * \param [IN] DWORD _code : ���� Code.
	 */
	void	GenerateExceptionString(DWORD _code);

	/** ���ܰ� �Ͼ ���� ���ּ� ���.
	 * ���ܰ� �Ͼ ���� �ּҸ� �޾Ƽ� module name, section, offset�� ���Ѵ�.
	 * \param [IN] const void* _address : ���ܰ� �Ͼ ���� ���ּ�.
	 * \param [IN/OUT] TCHAR* _modulename : ���ܰ� �Ͼ ���� ��� �̸�.
	 * \param [IN] DWORD len : _modulename�� ����.
	 * \param [OUT] DWORD& section : �� �ּ�.
	 * \param [OUT] DWORD& offset : �� �ּ�.
	 */
	BOOL	GetExceptionAddress(const void* _address, TCHAR* _modulename, DWORD len, DWORD& section, DWORD& offset);
}

#endif
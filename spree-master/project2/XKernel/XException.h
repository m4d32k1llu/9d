// XException.h: interface for the _XException class.
//
// Created Date : 2003.08.12
// Author		: Sohyun, Park
// DESC			: Structure Exception Handler Class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XEXCEPTION_H__7D41895A_BED5_482A_99FE_2A4200AA8E2F__INCLUDED_)
#define AFX_XEXCEPTION_H__7D41895A_BED5_482A_99FE_2A4200AA8E2F__INCLUDED_

#include <exception>
#include <windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/** ����ó�� Ŭ����.
 *
 * Structure Exception Handler ��ġ.
 * EXCEPTION_POINTERS�� ���� Exception Code�� Exception Address ����.
 * \author Sohyun, Park
 * \date 2003.08.18
 */
class _XException
{
public:
	/** Constructor.
	 */
	_XException(EXCEPTION_POINTERS const &info)  throw();

	/** Destructor.
	 */
	virtual ~_XException();

	/** Structure Exception Handler ��ġ.
	 */
	static	void			Install()	throw();

	/** Exception Code ����.
	 * \return DWORD Exception Code.
	 */
	DWORD					What()		const throw();

	/** Exception Address ����.
	 * \return void * Exception Address.
	 */
	void	const			*Where()	const throw();

private :
	/// Exception Address
	void	const			*m_Address;

	/// Exception Code
	DWORD					m_Code;

	/// Context Record
	PCONTEXT				m_ContextRecord;
};

#endif // !defined(AFX_XEXCEPTION_H__7D41895A_BED5_482A_99FE_2A4200AA8E2F__INCLUDED_)

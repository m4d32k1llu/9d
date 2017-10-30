#ifndef _FILEINPUT_HEADER_2006_01_13__
#define _FILEINPUT_HEADER_2006_01_13__

#include <string.h>

#include <string>
#include <iostream>
#include <fstream>
using namespace std ;

enum {
	en_cmd_not_file_open=0,		// 
	en_cmd_end,					// file end
	en_cmd_comment,				// comment or zero.
	en_cmd_over_length,			// length of command is over array size
	en_cmd_open_bracket,		// '{'
	en_cmd_close_bracket,		// '}'
	en_cmd_string,				// found string
} ;

class NDStreamFileInput
{
public:
	// /*
	NDStreamFileInput()
	{
		m_iCurLine = 0 ;
		m_countCommentChar = 0 ;
		::ZeroMemory(m_carrCommentChar, sizeof(m_carrCommentChar) ) ;

		m_pPosInLine = NULL ;

		file_add_comment_char( '/' ) ;
		file_add_comment_char( ';' ) ;
	}
	// */	// �⺻ �����ڸ� ���ش�. ��, �ݵ��, ���� �̸��� �Է����� ���� �����.
	NDStreamFileInput( const char * fname ) : m_ifsStream( fname ), m_strFileName( fname )
	{
		m_iCurLine = 0 ;
		m_countCommentChar = 0 ;
		::ZeroMemory(m_carrCommentChar, sizeof(m_carrCommentChar) ) ;

		m_pPosInLine = NULL ;

		file_add_comment_char( '/' ) ;
		file_add_comment_char( ';' ) ;
	}
	virtual ~NDStreamFileInput()
	{
		file_close() ;
	}

	// wrap methods.
	inline void file_open( const char * fname )		
	{
		m_strFileName.assign( fname ) ;
		m_ifsStream.open( fname, ios_base::in ) ; 
	}
	inline bool file_is_open()	{ return m_ifsStream.is_open() ; }

	inline void file_clear()	{ m_ifsStream.clear() ; } 
	
	inline void file_close()	{ if( file_is_open() ) m_ifsStream.close() ; }

	// add charactor that using as comment
	void file_add_comment_char( char ch )
	{
		if( m_countCommentChar < en_cmt_max_count )
		{
			m_carrCommentChar[m_countCommentChar] = ch ;
			++m_countCommentChar ;
		}
	}

	// show current comment charactor
	void file_show_cur_comment()
	{
		cout << m_carrCommentChar ;
	}

	// ��ɾ� ��� : ���� ������ ��� �ȴ�.
	int file_get_word( char * word, const int len ) ;	// return en_cmd. : word 
	int file_get_int() ;
	unsigned int file_get_uint() ;
	float file_get_float() ;
	int file_get_string( char * line, const int len ) ;	// ���ϰ��� ���ڿ� ����.

	// Ư�� ��ɾ��� ���, �ڿ� Ư�� ������ ���ڸ� ���ϰ� �Ǵµ�, 
	// ��Ʈ���� ���, ���� ������ �˻��ϱ� ����, 
	// �Լ��� ����� ����. ��ɾ� �ڷ�, ������ ������ �������� ������ ������ �����Ѵ�.
	int file_count_arguement() ;
	void file_skip_cur_line() ;			// ���� ������ �ǳ� �ڴ�.

	// ���� �̸� ����.
	const char * file_get_name()	{ return m_strFileName.c_str() ; }
	// ���� �Ľ����� ���μ� : �Ľ��� ��� ������ ���, ������ �Ѷ��μ��̴�.
	int file_get_cur_line_count()	{ return m_iCurLine ; }
	char * file_get_cur_line()		{ return &m_tempLine[0] ; }
	void file_make_lower( char * cmd ) ;


protected:
	bool file_is_comment( char ch )
	{
		for( int i = 0 ; i < m_countCommentChar ; ++i )
		{
			if( ch == m_carrCommentChar[i] )
				return true ;
		}

		return false ;
	}
	bool file_is_valid_char( char ch )
	{
		if( isalnum( ch ) || ch == '_' || ch == '{' || ch == '}' || ch < 0 || ch == '.' || ch == '-' || ch == '+' || ch == ':' )
			return true ;
		else
			return false ;
	}

protected:
	enum {
		en_cmt_max_count=12,	// �ڸ�Ʈ ���ڷ� ��޵Ǵ� ���ڵ��� ����. �ִ� 10��.
	} ;

	enum {
		en_count_max_line=512	// �� ������ �ִ� ���ڼ�.
	} ;

	

	ifstream		m_ifsStream ;		//
	string			m_strFileName ;		//

	int				m_iCurLine ;		// ���� �ټ�.

	int				m_countCommentChar ;					//
	char			m_carrCommentChar[en_cmt_max_count] ;	//

	char			m_tempLine[en_count_max_line] ;		// ���� ����.
	char *			m_pPosInLine ;			// ���� ���ο����� ���� ��ġ.

} ;



#endif
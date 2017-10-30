// XPackage.cpp: implementation of the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "XAudioPackage.h"

extern void _XLog (TCHAR *msg, ...);
extern TCHAR g_LastAccessPackedFile [_MAX_PATH];

void _XAudioEncodeData( LPSTR string, BYTE key )
{
	if( !string ) return;
	assert( string );

	int length = strlen( string );

	LPSTR tptr = string;

	for( int i = 0; i < length; i++ )
	{
		*tptr ^= key;
		tptr++;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
_XAudioPackageArchive::_XAudioPackageArchive()
{
	m_PackageFileptr = NULL;

	m_PackedFileCount = 0;

	m_PackefFileStartPosition = 0;

	m_PackageFilenameArray		= NULL;

	m_PackageFilePositionArray	= NULL;

	memset( m_ArchiveName, 0, _MAX_PATH );
}

_XAudioPackageArchive::~_XAudioPackageArchive()
{
	if( m_PackageFileptr ) fclose( m_PackageFileptr );

	if( m_PackageFilePositionArray )
	{
		delete[] m_PackageFilePositionArray;
		m_PackageFilePositionArray = NULL;
	}

	if( m_PackageFilenameArray )
	{
		delete[] m_PackageFilenameArray;
		m_PackageFilenameArray = NULL;
	}
}


BOOL _XAudioPackageArchive::ReOpen( void )
{
	if( !this->m_PackageFileptr )
	{
		this->m_PackageFileptr = fopen( m_ArchiveName, "rb" );	
		if( !this->m_PackageFileptr )
		{
			_XFatalError( "Package openning [%s]", m_ArchiveName );
			return FALSE;
		}
	}
	
	//_XLog( "ReOpen package file [%s]\n", m_ArchiveName );
	return TRUE;
}

BOOL _XAudioPackageArchive::Open( LPCTSTR filename )
{
	if( !filename[0] ) return FALSE;

	Close();

	memset( m_ArchiveName, 0, _MAX_PATH );

	this->m_PackageFileptr = fopen( filename, "rb" );

	if( !this->m_PackageFileptr )
	{
		_XFatalError( "Package openning [%s]", filename );
		return FALSE;
	}

	_AUDIOPACKAGEFILE_HEADER packagefileheader;

	if( fread( &packagefileheader, sizeof( _AUDIOPACKAGEFILE_HEADER ), 1, m_PackageFileptr ) < 1 )
	{
		_XFatalError( "Package header loading [%s]", filename );
		Close();
		return FALSE;
	}

	if( packagefileheader.HeaderIndicator != _HEADERINDICATOR_AUDIOPACKAGEFILE )
	{
		_XFatalError( "Package header loading [%s]", filename );
		Close();
		return FALSE;
	}

	if( packagefileheader.FileVersion != _HEADERINDICATOR_AUDIOPACKAGEFILEVERSION )
	{
		_XFatalError( "Package version [%s]", filename );
		Close();
		return FALSE;
	}

	this->m_PackedFileCount = packagefileheader.PackedFileCount;

	m_PackageFilePositionArray = new int[ this->m_PackedFileCount ];

	if( !m_PackageFilePositionArray )
	{
		_XFatalError( "Allocate position array [%s]", filename );
		Close();
		return FALSE;
	}

	// Alloc.. & Read filename informations...
	m_PackageFilenameArray = new _XAudioFileInfo[ this->m_PackedFileCount ];

	if( !m_PackageFilenameArray )
	{
		_XFatalError( "Allocate resource descrpition array [%s]", filename );
		Close();
		return FALSE;
	}

	WORD length;
	TCHAR tempbuffer[_MAX_PATH];

	for( int i = 0; i < this->m_PackedFileCount; i++ )
	{
		length = 0;
		memset( tempbuffer, 0, _MAX_PATH );

		if( fread( &length, sizeof(WORD), 1, m_PackageFileptr ) < 1 )
		{
			_XFatalError( "Read file descrpition array size [%s]", filename );
			Close();
			return FALSE;
		}

		if( fread( &tempbuffer , length, 1, m_PackageFileptr ) < 1 )
		{
			_XFatalError( "Read file descrpition array [%s]", filename );
			Close();
			return FALSE;
		}

		_XAudioEncodeData( tempbuffer, _AUDIOENCODEKEY );
		m_PackageFilenameArray[i].Set( tempbuffer );
	}

	if( fread( m_PackageFilePositionArray, sizeof(int)*this->m_PackedFileCount, 1, m_PackageFileptr ) < 1 )
	{
		_XFatalError( "Read position array [%s]", filename );
		Close();
		return FALSE;
	}

	m_PackefFileStartPosition =  sizeof( _AUDIOPACKAGEFILE_HEADER ) + (sizeof(int)*this->m_PackedFileCount);	

	strcpy( m_ArchiveName, filename );

	//_XLog( "Open package file [%s]\n", m_ArchiveName );
			
	return TRUE;
}

BOOL _XAudioPackageArchive::Close( BOOL resetfilelist )
{
	//if( strlen( m_ArchiveName ) && m_PackageFileptr )
	//	_XLog( "Close package file [%s]\n", m_ArchiveName );

	if( m_PackageFileptr )
	{
		fclose( m_PackageFileptr );
		m_PackageFileptr = NULL;
	}

	if( resetfilelist )
	{
		m_PackedFileCount = 0;

		m_PackefFileStartPosition = 0;	

		if( m_PackageFilenameArray )
		{
			delete[] m_PackageFilenameArray;
			m_PackageFilenameArray = NULL;
		}

		if( m_PackageFilePositionArray )
		{
			delete[] m_PackageFilePositionArray;
			m_PackageFilePositionArray = NULL;
		}	
	}

	return TRUE;
}

LONG  _XAudioPackageArchive::GetPackedFileSize( int file_id )
{
	if( !m_PackageFileptr )		
	{
		_XFatalError( "Not ready package [%s : %d]", m_ArchiveName, file_id  );
		return -1;
	}

	if( m_PackedFileCount <= file_id )
	{
		_XFatalError( "Invalid Package ID  [%s : %d]", m_ArchiveName, file_id );
		return -1;
	}

	if( file_id < m_PackedFileCount-1 )
	{
		return m_PackageFilePositionArray[file_id+1] - m_PackageFilePositionArray[file_id];
	}
	else // ��Ű���� �ǳ� �����̸�...
	{
		fseek( m_PackageFileptr, 0L, SEEK_END );
		LONG endptr = ftell( m_PackageFileptr );
		rewind( m_PackageFileptr );

		return ( endptr - m_PackageFilePositionArray[file_id] );
	}	

	return -1;
}

FILE* _XAudioPackageArchive::GetPackedFile( int file_id )
{
	if( !m_PackageFileptr )		
	{
		_XFatalError( "Not ready package [%s : %d]", m_ArchiveName, file_id  );
		return NULL;
	}

	if( m_PackedFileCount <= file_id )
	{
		_XFatalError( "Invalid Package ID  [%s : %d]", m_ArchiveName, file_id );
		return NULL;
	}
	
	if( fseek( this->m_PackageFileptr, m_PackageFilePositionArray[file_id], SEEK_SET ) == 0 )
	{
		return this->m_PackageFileptr;
	}
	else
	{
		_XFatalError( "Package not found [%s : %d]", m_ArchiveName, file_id );
		return NULL;
	}

	return NULL;
}

FILE* _XAudioPackageArchive::GetPackedFile( LPSTR file_name )
{	
	assert( file_name );

	memset( g_LastAccessPackedFile, 0, _MAX_PATH );

	if( !file_name ) return NULL;

	strcpy( g_LastAccessPackedFile, file_name );

	if( !m_PackageFileptr )		
	{
		_XFatalError( "Not ready package [%s : %s]", m_ArchiveName, file_name );
		return NULL;
	}

	int resourceindex = FindResource( file_name );

	if( resourceindex == -1 )
	{		
		_XFatalError( "Can't find resource [%s : %s]", m_ArchiveName, file_name );
		return NULL;
	}

	return GetPackedFile( resourceindex );
}

LPBYTE _XAudioPackageArchive::LoadPackedFileToMemory( LPSTR file_name, LONG* buffersize)
{
	assert( file_name );
	if( buffersize ) *buffersize = 0;
	
	memset( g_LastAccessPackedFile, 0, _MAX_PATH );
	
	if( !file_name ) return NULL;
	
	strcpy( g_LastAccessPackedFile, file_name );
	
	if( !m_PackageFileptr )
	{
		_XFatalError( "Not ready package [%s : %s]", m_ArchiveName, file_name );
		return NULL;
	}
	
	int resourceindex = FindResource( file_name );
	
	if( resourceindex == -1 )
	{		
		_XFatalError( "Can't find resource [%s : %s]", m_ArchiveName, file_name );
		return NULL;
	}
	
	FILE* fp = GetPackedFile( resourceindex );
	if( !fp )
	{
		return NULL;
	}
	
	LONG filesize = GetPackedFileSize( resourceindex );
	
	if( filesize <= 0 )
	{
		_XFatalError( "Invalid Resource size [%s : %s]", m_ArchiveName, file_name );
		return NULL;
	}
	
	LPBYTE pResourceData = NULL;
	pResourceData = new BYTE[ filesize ];
	if( !pResourceData )
	{
		_XFatalError( "Can't allocate Resource buffer [%s : %s]", m_ArchiveName, file_name );
		return NULL;	
	}
	
	if( fread( pResourceData, filesize, 1, fp  ) < 1 )
	{
		delete pResourceData;
		_XFatalError( "Can't read resource data [%s : %s]", m_ArchiveName, file_name );
		return NULL;	
	}

	if( buffersize ) *buffersize = filesize;
	return pResourceData;
}

LPBYTE _XAudioPackageArchive::LoadPackedFileToMemory( DWORD resourceindex, LONG* buffersize )
{	
	if( buffersize ) *buffersize = 0;
	
	memset( g_LastAccessPackedFile, 0, _MAX_PATH );
			
	if( !m_PackageFileptr )
	{
		_XFatalError( "Not ready package [%s : [ri:%d]]", m_ArchiveName, resourceindex );
		return NULL;
	}

	LONG filesize = GetPackedFileSize( resourceindex );	
	if( filesize <= 0 )
	{
		_XFatalError( "Invalid Resource size [%s : [ri:%d]]", m_ArchiveName, resourceindex );
		return NULL;
	}
			
	FILE* fp = GetPackedFile( resourceindex );
	if( !fp )
	{
		return NULL;
	}
			
	LPBYTE pResourceData = NULL;
	pResourceData = new BYTE[ filesize ];
	if( !pResourceData )
	{
		_XFatalError( "Can't allocate Resource buffer [%s : [ri:%d]]", m_ArchiveName, resourceindex );
		return NULL;	
	}
	
	if( fread( pResourceData, filesize, 1, fp  ) < 1 )
	{
		delete pResourceData;
		_XFatalError( "Can't read resource data [%s : [ri:%d]]", m_ArchiveName, resourceindex );
		return NULL;	
	}
	
	if( buffersize ) *buffersize = filesize;
	return pResourceData;
}

int  _XAudioPackageArchive::FindResource( LPSTR resourcename )
{
	assert( resourcename );
	if( !resourcename ) return -1;
	
	assert( m_PackageFilenameArray );
	if( !m_PackageFilenameArray ) return -1;

	for( int i = 0; i < m_PackedFileCount; i++ )
	{
		if( 0 == strcmpi( resourcename, m_PackageFilenameArray[i].m_filename ) )
		{
			return i;
		}
	}

	return -1;
}

LPSTR _XAudioPackageArchive::GetResourceName( int file_id )
{
	if( !m_PackageFileptr )		
	{
		_XFatalError( "Not ready package [%s : %d]", m_ArchiveName, file_id );
		return NULL;
	}

	if( m_PackedFileCount <= file_id )
	{
		_XFatalError( "Invalid Package ID [%s : %d]", m_ArchiveName, file_id );
		return NULL;
	}
	
	return m_PackageFilenameArray[ file_id ].m_filename;
}

int	_XAudioPackageArchive::GetResourceCount( void )
{
	if( !m_PackageFileptr ) return 0;

	return m_PackedFileCount;
}


#include "imagehlp.h" 

BOOL _XAudioPackageArchive::ExportFile( int file_id, LPCTSTR filename )
{	
	if( !filename || !filename[0] ) return FALSE;

	FILE* pExportFile = NULL;
	FILE* pPackageFile = this->GetPackedFile( file_id );	
	if( !pPackageFile )	return FALSE;

	TCHAR FilePath[ _MAX_PATH+1 ];				
	memset( FilePath, 0, _MAX_PATH+1 );
	strncpy( FilePath, filename, _MAX_PATH );
		
	for( int i = strlen(FilePath)-1; i > 0; i-- )
	{
		if( FilePath[i] == '\\' ) break;
		else FilePath[i] = NULL;
	}

	if(FilePath[0]) MakeSureDirectoryPathExists( FilePath );
	
	
	//if( CheckExistFile( filename, TRUE ) ) CFile::Remove( filename );

	
	pExportFile = fopen( filename, "wb" );

	if( !pExportFile )
	{
		_XFatalError( "Export file open [%s : %s]", m_ArchiveName, filename );
		return FALSE;
	}
	
	long filesize = 0L;
	long currentfilepos = 0L;
	int  databuffer = 0;
		
	if( file_id < m_PackedFileCount-1 )
	{		
		filesize = m_PackageFilePositionArray[file_id+1] - m_PackageFilePositionArray[file_id];
	}	
	else // ��Ű���� �ǳ� �����̸�...
	{
		filesize = -1;		
	}


	do
	{
		if( feof( pPackageFile ) ) break;

		databuffer = fgetc( pPackageFile );

		if( databuffer == EOF ) break;

		if( EOF == fputc( databuffer , pExportFile ) )
		{
			_XFatalError( "Export data [%s : %s]", m_ArchiveName, filename );
			break;
		}
		
		currentfilepos++;

		//�Ѱ� ������ ���̸� ����..
		if( filesize != -1 )
		{
			if( filesize == currentfilepos ) break;
		}

	}while( !feof( pPackageFile ) );

	fclose( pExportFile );

	return TRUE;
}


BOOL _XAudioPackageArchive::Patch( LPCTSTR packagefilename, LPCTSTR patchinfofilename )
{

/*
	if( !packagefilename[0] || !patchinfofilename[0] ) return FALSE;

	if( m_PackageFileptr ) this->Close();
	
	if( !this->Open( packagefilename ) )
	{		
		return FALSE;
	}

	FILE* pPatchinfoFile = NULL;

	pPatchinfoFile = fopen( patchinfofilename, "rt" );

	if( !pPatchinfoFile )
	{
		_DXMessage( "ERROR : Upgrade information file open" );
		this->Close();
		return FALSE;
	}

	TCHAR  parsingString[256];
	int   packageindexcount = 0;
	FILE* pCurrentpackagefile = NULL;

	TCHAR  currentPathString[_MAX_PATH];
	CString tFilenamestring;
	CString tMessagestring;

	::GetCurrentDirectory( _MAX_PATH, currentPathString );

	while( !feof( pPatchinfoFile ) )
	{
		if( feof( pPatchinfoFile ) ) break;

		memset( parsingString, 0, 256 );
		fgets( parsingString, 255, pPatchinfoFile );

		if( strlen(parsingString) && parsingString[0] != '\n' && parsingString[0] != ';' )
		{
			if( this->GetPackedFile( packageindexcount ) )
			{				
				tFilenamestring = currentPathString;
				
				tFilenamestring += parsingString;

				tFilenamestring.Remove( '\n' );				

				if( !this->ExportFile( packageindexcount, tFilenamestring ) ) break;

			}
			packageindexcount++;
		}
	}
	
	fclose( pPatchinfoFile );
	this->Close();
*/

	return TRUE;
}

/*
BOOL CheckExistFile( LPCSTR filename, BOOL resetAttrib )
{
	HANDLE      hFind;
	WIN32_FIND_DATA rFind;	

	hFind = FindFirstFile ( filename, &rFind );

	if ( INVALID_HANDLE_VALUE == hFind )
	{
		FindClose ( hFind );
		return FALSE;
	}

	FindClose ( hFind );

	if( resetAttrib )
	{
		CFileStatus filestatus;
		CFile::GetStatus( filename, filestatus );
		filestatus.m_attribute = 0x00; // normal file attrib setting
		CFile::SetStatus( filename, filestatus );
	}

	return TRUE;
}


BOOL WriteDirectory(CString dd)
{


	HANDLE		fFile;					// File Handle
	WIN32_FIND_DATA fileinfo;			// File Information Structure
	CStringArray	m_arr;				// CString Array to hold Directory Structures
	BOOL tt;							// BOOL used to test if Create Directory was successful
	int x1 = 0;							// Counter
	CString tem = "";					// Temporary CString Object

	// Before we go to a lot of work.  
	// Does the file exist

	fFile = FindFirstFile(dd,&fileinfo);

	// if the file exists and it is a directory
	if(fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
	{
		//  Directory Exists close file and return
		FindClose(fFile);
		return TRUE;
	}

	m_arr.RemoveAll();					// Not really necessary - Just habit
	for(x1=0;x1<dd.GetLength();x1++)	// Parse the supplied CString Directory String
	{									
		if(dd.GetAt(x1) != '\\')		// if the Charachter is not a \ 
			tem += dd.GetAt(x1);		// else add character to Temp String
		else
		{
			m_arr.Add(tem);				// if the Character is a \ Add the Temp String to the CString Array
			tem += "\\";				// Now add the \ to the temp string
		}
		if(x1 == dd.GetLength()-1)		// If we reached the end of the file add the remaining string
			m_arr.Add(tem);
	}


	// Close the file
	FindClose(fFile);
	
	// Now lets cycle through the String Array and create each directory in turn
	for(x1 = 1;x1<m_arr.GetSize();x1++)
	{
		tem = m_arr.GetAt(x1);
		tt = CreateDirectory(tem,NULL);

		// If the Directory exists it will return a false
		if(tt)
			SetFileAttributes(tem,FILE_ATTRIBUTE_NORMAL);
		// If we were successful we set the attributes to normal
	}
	m_arr.RemoveAll();
	//  Now lets see if the directory was successfully created
	fFile = FindFirstFile(dd,&fileinfo);

	// if the file exists and it is a directory
	if(fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
	{
		//  Directory Exists close file and return
		FindClose(fFile);
		return TRUE;
	}
	else
	{
		FindClose(fFile);
		return FALSE;
	}
}

*/

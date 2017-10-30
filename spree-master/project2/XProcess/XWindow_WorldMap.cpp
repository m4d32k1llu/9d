// XWindow_WorldMap.cpp: implementation of the XWindow_WorldMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "xbasedefs.h"
#include "xsr_stringheader.h"
#include "XMLPaser.h"

#include "XWindow_WorldMap.h"

XWindow_WorldMap*	g_pWorldMap_Window = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XWindow_WorldMap::XWindow_WorldMap() : m_pWorldmapTex(NULL), m_pWorldMapImage(NULL), m_pCharinfo(NULL), m_pBoard(NULL),
									   m_mpData(), m_nMaxLinkMapCount(0), m_nFindKey(-1), m_bAlphaProcess(FALSE)
{

}

XWindow_WorldMap::~XWindow_WorldMap()
{

}

BOOL XWindow_WorldMap::Initialize(XMLPaser& m_xXML)
{
	//�������� ũ�� //������
	RECT curwindowrect = { m_WindowPosition.x, m_WindowPosition.y, 
						   m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy };
	FLOAT fWidth  = curwindowrect.right  - curwindowrect.left;
	FLOAT fHeight = curwindowrect.bottom - curwindowrect.top;

	//�ؽ��� �ε�
	_XPackageArchive TextureArchive;

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	strcat( packagefilename, _T("\\Data\\Texture\\") );
	strcat( packagefilename, _XDEF_TEXTUREFILE_A );

	//����� �޸�
	LPBYTE pImageVFMemory = NULL;

	try
	{
		int nRes = g_MainInterfaceTextureArchive.FindResource( "worldmap_icon.tga" );
		
		if( nRes < 0 )	
		{
			_XLog( "Can't found worldmap_icon.tga file" );
			throw(0);
		}


		m_pBoard = new _XImageStatic;
		m_pBoard->Create( 20, 30, 20 + 341, 30 + 68, &g_MainInterfaceTextureArchive, nRes );	
		m_pBoard->SetClipRect( 0, 422, 341, 422 + 68 );
		m_pBoard->m_FColor = D3DCOLOR_ARGB( 129, 255,255,255 );
		InsertChildObject( m_pBoard );

		//����
		if( !TextureArchive.OpenPackage( packagefilename ) ) 
			throw(0);
	
		TCHAR messagestring[256];
		memset( messagestring, 0, sizeof(TCHAR)*256 );
		
#ifdef _XVIETNAMESE
		sprintf( messagestring, "Vn_");
		strcat( messagestring, "worldmap.tga" );
#elif _XTAIWANESE
		sprintf( messagestring, "Tw_");
		strcat( messagestring, "worldmap.tga" );
#elif _XENGLISH
		sprintf( messagestring, "Us_");
		strcat( messagestring, "worldmap.tga" );
#elif _XJAPANESE
		sprintf( messagestring, "Jp_");
		strcat( messagestring, "worldmap.tga" );
#elif _XRUSSIAN
		sprintf( messagestring, "Rs_");
		strcat( messagestring, "worldmap.tga" );
#else
		sprintf( messagestring, "worldmap_kr.tga");
#endif
		//������� ã�´�
		nRes = TextureArchive.FindResource( messagestring );

		if( nRes < 0 )
			throw(0);


		int   filesize = TextureArchive.GetPackedFileSize(nRes);
		FILE* pFP = TextureArchive.GetPackedFile(nRes);
		
		pImageVFMemory = new BYTE[ filesize ];

		if( !pImageVFMemory )
			throw(0);
		
		if( fread( pImageVFMemory, filesize, 1, pFP ) < 1 )
			throw(0);

		TextureArchive.FinalizePackage();
		
		SAFE_RELEASE( m_pWorldmapTex );

		if( FAILED( D3DXCreateTextureFromFileInMemoryEx( gpDev, pImageVFMemory, filesize, 
			D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
			D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
			D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
			0, NULL, NULL, &m_pWorldmapTex) ) )
		{
			throw(0);
		}
		
		delete[] pImageVFMemory;
		
		//�ݱ�
		TextureArchive.FinalizePackage();

		//�̹��� ����
		m_pWorldMapImage = new _XImageStatic;
		if( !m_pWorldMapImage )
			throw(0);

		m_pWorldMapImage->Create( 0, 0, 1024, 768, &g_MainInterfaceTextureArchive, -1 );
		m_pWorldMapImage->SetClipRect( 0, 0, 1024, 768 );

		/*if( gnWidth < 1024 && gnHeight < 768 )
		{
			float fWidthScale = (float)gnWidth/1024.f;
			float fHeightScale = (float)gnHeight/768.f;
			m_pWorldMapImage->SetScale( fWidthScale, fHeightScale );
		}
		else*/
			m_pWorldMapImage->SetScale( 1.0f, 1.0f );

		m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( 0, 255,255,255 );

		nRes = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
		_XBTN_STRUCTURE Closebtnstruct = 
		{ 
			TRUE, { m_pWorldMapImage->m_WindowRect.right - 90, m_pWorldMapImage->m_WindowRect.bottom - 64 }, { 60, 24 }, 
			_XDEF_WORLDMAP_CLOSEBUTTON, nRes, nRes, nRes, &g_MainInterfaceTextureArchive 
		};
		//���ư��� ��ư
		_XButton* pButton = new _XButton;
		pButton->Create( Closebtnstruct  );	
		pButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		pButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CLOSE); //�ݱ�
		pButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 187, 152, 247, 176 );
		pButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
		pButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
		pButton->ShowWindow( TRUE );
		InsertChildObject( pButton );

		//�ؽ��� ����
		nRes  = g_MainInterfaceTextureArchive.FindResource( "worldmap_icon.tga" );
		
		m_pCharinfo = new _XTextureAnimation( 0, 0, 64, 64, this, &g_MainInterfaceTextureArchive, nRes,
											  7, 1, 7, 0, 192 );
		m_pCharinfo->ShowWindow( FALSE );
		//m_pCharinfo.Create(0, 0, 64, 64, &g_MainInterfaceTextureArchive, nRes);
		//m_pCharinfo.SetWindow( this );
		//m_pCharinfo.InitLocationA( -1, 7, 1, 7, 0, 192 );

		LoadXMLTable(m_xXML, _T("world_map") );
	}
	catch(...)
	{
		if(pImageVFMemory)
			delete[] pImageVFMemory;

		SAFE_RELEASE( m_pWorldmapTex );
		SAFE_DELETE( m_pWorldMapImage );

		//�ݱ�
		TextureArchive.FinalizePackage();

		return FALSE;
	}

	return TRUE;
}

BOOL XWindow_WorldMap::LoadXMLTable(XMLPaser& m_xXML, LPTSTR pTableName )
{
	map<int, MapInfo*>::iterator iter;

	//��ư�� ���۾��̵�..
	int nBtnID = _XDEF_WORLDMAP_MAPBUTTON_01;

	//��ū�� ����..
	char szTemp[256];
	char* szToken;
	memset( szTemp, 0, sizeof(szTemp));

	//������ �ε� �ϱ� ����... �޸𸮷� �����Ͽ��� �Ѵ�
	//�׽�Ʈ��   ���� �� ���� ��ü
	TCHAR szBuffer[MAX_PATH];
	memset( szBuffer, 0, sizeof(szBuffer) );
	strcpy( szBuffer, gModulePath );
	strcat( szBuffer, "\\GameInfo.xml" );
	//FILE* fp = fopen( szBuffer, "rb"  );

#define _XDEF_BLOCKREAD
	
/*#ifdef _XDEF_BLOCKREAD
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

	int resid = g_ScriptArchive.FindResource(_T("GameInfo.xml"));	
	FILE* pFile = g_ScriptArchive.GetPackedFile(_T("GameInfo.xml"));

	if( resid >= 0 && pFile )
	{
		int filesize = g_ScriptArchive.GetPackedFileSize(resid);
		
		LPTSTR pBuffer = NULL;
		pBuffer = new TCHAR[ filesize+1 ];

		if( pBuffer )
		{
			memset( pBuffer, 0, sizeof(TCHAR) * (filesize + 1) );
			fread(  pBuffer, filesize, 1, pFile );
			
			if( E_FAIL == m_xXML.ND_hrOpenMemory(pBuffer) )
			{
				_XFatalError( "Can't read GameInfo.xml file" );
				
				delete[] pBuffer;
				g_ScriptArchive.ClosePackage();

				return FALSE;
			}		
			delete[] pBuffer;
		}
		else
		{
			g_ScriptArchive.ClosePackage();
			
			_XFatalError( "Can't allocate xml buffer" );			
			return FALSE;
		}
	}
	else
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	g_ScriptArchive.ClosePackage();

#else
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
	FILE* fp = g_ScriptArchive.GetPackedFile(_T("GameInfo.xml"));

	//���� ������ �ҷ�����
	if( !LoadFile( fp ) )
	{
		_XLog( "GameInfo.xml ���Ϸε� ����" );
		return FALSE;
	}
#endif*/
	//���� ���� �ε�
	//if( E_FAIL == m_xXML.ND_hrOpen( _T("system.xml")) )
	//	return FALSE;

	//�ش� ���̺��� �ִ��� �˻�
	if( ! m_xXML.ND_isThereTable( pTableName ) )
	{
		return FALSE;
	}

	int TableRowCount = m_xXML.ND_nGetTableRowCount( pTableName );

	if(TableRowCount <=0)	
		return FALSE;

	TableRowCount -= 1;

	//�ؽ��� �ε�
	_XPackageArchive TextureArchive;

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	strcat( packagefilename, _T("\\Data\\Texture\\") );
	strcat( packagefilename, _XDEF_TEXTUREFILE_A );

	//����
	if( !TextureArchive.OpenPackage( packagefilename ) ) 
		return FALSE;

	for( int i = 0; i < TableRowCount; i++ )
	{
		iter = m_mpData.find( i );
		
		//���� Ű���� �ִ°��
		if( iter != m_mpData.end() )
			continue;

		MapInfo* pData = new MapInfo;

		m_xXML.ND_hrGetTableValue( pTableName, "ID", i, &pData->nID ); // ���̵� ����
		m_xXML.ND_hrGetTableValue( pTableName, "Name", i, &pData->nName ); // �� �̸� ����

		//��ǥ�� �����ϱ� ����..
		memset( szTemp, 0, sizeof(szTemp));
		m_xXML.ND_hrGetTableValue( pTableName, "Icon_position", i, szTemp ); // �������� ��ǥ�� ����

		//, ����, �ݵ�� x,y ��ǥ�� �̷���� ������, ���� �������� ��ǥ�� �����Ѵ�
		szToken = strtok( szTemp, "," );
		if( szToken )
		{
			pData->IconPos.x = atol(szToken);
		}
		szToken = strtok( NULL, "," );
		if( szToken )
		{
			pData->IconPos.y = atol(szToken);
		}

		m_xXML.ND_hrGetTableValue( pTableName, "Icon_Index", i, &pData->nIconIndex ); //������ ���� ����

		m_xXML.ND_hrGetTableValue( pTableName, "MAP_Info_1", i, &pData->nMapInfo_1 ); // �� ù��° �ؽ�Ʈ ����
		
		memset( szTemp, 0, sizeof(szTemp));
		m_xXML.ND_hrGetTableValue( pTableName, "MAP_Info_Color_1", i, szTemp ); // �ؽ�Ʈ �÷� ����
		SetStringColor( szTemp, pData->dwMapInfoColor_1 );

		m_xXML.ND_hrGetTableValue( pTableName, "MAP_Info_2", i, &pData->nMapInfo_2 ); // �� �ι�° �ؽ�Ʈ ����

		memset( szTemp, 0, sizeof(szTemp));
		m_xXML.ND_hrGetTableValue( pTableName, "MAP_Info_Color_2", i, szTemp); //�ؽ�Ʈ �÷� ����
		SetStringColor( szTemp, pData->dwMapInfoColor_2);

		memset( szTemp, 0, sizeof(szTemp));
		m_xXML.ND_hrGetTableValue( pTableName, "Link_Map_ID", i, szTemp ); //��ũ �Ǿ��ִ� ���̵� ����
		
		int nLinkMapCheckCount = 0;
		int nTemp = 0;
		szToken = strtok( szTemp, "," );	
		if( szToken )
		{
			nTemp = atoi(szToken);
			pData->vtLinkID.push_back(nTemp);
			nLinkMapCheckCount++;
		}

		while( szToken != NULL )
		{
			szToken = strtok( NULL, "," );
			if( szToken )
			{
				nTemp = atoi(szToken);
				pData->vtLinkID.push_back(nTemp);
				nLinkMapCheckCount++;
			}
		}

		if( nLinkMapCheckCount > m_nMaxLinkMapCount )
			m_nMaxLinkMapCount = nLinkMapCheckCount;

		m_xXML.ND_hrGetTableValue( pTableName, "Application", i, &pData->bEnable ); //��� ���� ����

		//�̹����� ����
		SetMiniMapImage(TextureArchive, *pData );

		//�ش� ��ư�� �����Ѵ�
		SetButton(nBtnID, *pData );
		nBtnID++; //��ư ���̵� ���� ���� �����

		m_mpData[i] = pData;
	}

	//�ݱ�
	TextureArchive.FinalizePackage();

	//���ϸ��̼��� ����
	for( int j = 0; j < m_nMaxLinkMapCount; j++ )
	{
		_XTextureAnimation* xTexAni = new _XTextureAnimation( 0, 0, 64, 64, this, &g_MainInterfaceTextureArchive, 
															g_MainInterfaceTextureArchive.FindResource( "worldmap_icon.tga" ),
															8, 1, 8 );
		xTexAni->ShowWindow( FALSE );
		vtTextureAni.push_back( xTexAni );
	}

	return TRUE;
}

BOOL XWindow_WorldMap::LoadFile( FILE* fp )
{
	if( !fp )
		return FALSE;

	//������ ������
	long length = 0;
	long count = 0;
	//fseek( fp, 0, SEEK_END );
	//length = ftell( fp );
	//fseek( fp, 0, SEEK_SET );

	int nRes = g_ScriptArchive.FindResource( "GameInfo.xml" );
	length = g_ScriptArchive.GetPackedFileSize( nRes ) - 1;

	if ( length <= 0 )
		return FALSE;

	string strData = "";

	char* buf = new char[ length+1 ];
	buf[0] = 0;
	
	//�پ��� �÷����� �����ϰԲ� �ٲپ���Ѵ�
	while( fgets( buf, sizeof(buf), fp ) )
	{
		if( length <= count )
			break;
		strData += buf;
		count = strData.size();
	}

	//�ٸ� �÷����� ����..
	/*char* buf = new char[ length+1 ];
	buf[0] = 0;

	if ( fread( buf, length, 1, fp ) != 1 )
	{
		delete [] buf;
		return FALSE;
	}

	const char* lastPos = buf;
	const char* p = buf;

	buf[length] = 0;
	while( *p ) 
	{
		assert( p < (buf+length) );
		if ( *p == 0xa )
		{
			// Newline character. No special rules for this. Append all the characters
			// since the last string, and include the newline.
			data.append( lastPos, (p-lastPos+1) );	// append, include the newline
			++p;									// move past the newline
			lastPos = p;							// and point to the new buffer (may be 0)
			assert( p <= (buf+length) );
		}
		else if ( *p == 0xd )
		{
			// Carriage return. Append what we have so far, then
			// handle moving forward in the buffer.
			if ( (p-lastPos) > 0 ) {
				data.append( lastPos, p-lastPos );	// do not add the CR
			}
			data += (char)0xa;						// a proper newline

			if ( *(p+1) == 0xa )
			{
				// Carriage return - new line sequence
				p += 2;
				lastPos = p;
				assert( p <= (buf+length) );
			}
			else 
			{
				// it was followed by something else...that is presumably characters again.
				++p;
				lastPos = p;
				assert( p <= (buf+length) );
			}
		}
		else
		{
			++p;
		}
	}
	// Handle any left over characters.
	if ( p-lastPos )
	{
		data.append( lastPos, p-lastPos );
	}*/

	delete [] buf;
	buf = 0;

	g_ScriptArchive.ClosePackage();

/*	if( E_FAIL == m_xXML.ND_hrOpenMemory(strData.c_str()) )
	{
		_XLog( "GameInfo.xml �������� �޸� �ε� ����" );
		return FALSE;
	}*/

	return TRUE;
}

void XWindow_WorldMap::SetButton( const int nID, const MapInfo& MapData )
{
	//�̹����� ������
	RECT sizerect = {0,};

	sizerect.left = 64 * MapData.nIconIndex;
	sizerect.top = 64;
	sizerect.bottom = 128;
	sizerect.right = 64 + (64 * MapData.nIconIndex);

	//�׸��̹���
	int Imageindex = g_MainInterfaceTextureArchive.FindResource( "worldmap_icon.tga" );

	//�ؽ��İ� ����
	if( Imageindex < 0)
		return;

	//��ư���� ..
	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, 
		{ MapData.IconPos.x, MapData.IconPos.y }, 
		{ sizerect.right - sizerect.left, sizerect.bottom - sizerect.top },
		nID, Imageindex, Imageindex, Imageindex, &g_MainInterfaceTextureArchive 
	};

	_XButton* pButton = new _XButton;
	pButton->Create( btnstruct );	
	pButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, sizerect.left, sizerect.top, sizerect.right, sizerect.bottom);
	pButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, sizerect.left, sizerect.top + 64, sizerect.right, sizerect.bottom + 64);
	pButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  sizerect.left, sizerect.top + 64, sizerect.right, sizerect.bottom + 64);
	pButton->ShowWindow( MapData.bEnable );

	InsertChildObject( pButton );	
}

BOOL XWindow_WorldMap::SetMiniMapImage(_XPackageArchive& TextureArchive, MapInfo& MapData )
{
	//���̸� ����
	TCHAR minimapnamestr[_MAX_PATH];
	memset( minimapnamestr, 0, _MAX_PATH );

	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{			
		strcpy( minimapnamestr, "Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		strcpy( minimapnamestr, "vn_Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		strcpy( minimapnamestr, "us_Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		strcpy( minimapnamestr, "tw_Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		strcpy( minimapnamestr, "jp_Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		strcpy( minimapnamestr, "rs_Mini" );
	}
	else
	{
		strcpy( minimapnamestr, "Mini" );
	}

	switch( MapData.nID )
	{
	case 0 : //����
		strcat( minimapnamestr, "map_gaebang.tga" );
		break;
	case 1 : //�Ҹ���
		strcat( minimapnamestr, "map_sorim.tga" );
		break;
	case 2 : //����
		strcat( minimapnamestr, "map_moodang.tga" );
		break;
	case 3 : //�츲
		strcat( minimapnamestr, "map_noklim.tga" );
		break;
	case 4 : //���
		strcat( minimapnamestr, "map_beegoong.tga" );
		break;
	case 5 : //����
		strcat( minimapnamestr, "map_magyo.tga" );
		break;
	case 6 : //�պ�
		strcat( minimapnamestr, "map_hapbi.tga" );
		break;
	case 7 : //�ϰ�
		strcat( minimapnamestr, ".tga" );
		break;
	case 8 : //����
		strcat( minimapnamestr, "map_jungju.tga" );
		break;
	case 9 : //������
		strcat( minimapnamestr, "map_seokgaja.tga" );
		break;
	case 10 : //����
		strcat( minimapnamestr, "map_jaenam.tga" );
		break;
	case 11 : //�뵿
		strcat( minimapnamestr, "map_daedong.tga" );
		break;
	case 12 : //��â
		strcat( minimapnamestr, "map_namchang.tga" );
		break;
	case 13 : //����
		strcat( minimapnamestr, "map_hangjoo.tga" );
		break;
	case 14 : //�䵿
		strcat( minimapnamestr, "map_Yodong.tga" );
		break;
	case 15 : //���غ���
		strcat( minimapnamestr, "map_BookHae.tga" );
		break;
	case 16 : //������
		strcat( minimapnamestr, "map_hyulsapyung.tga" );
		break;
	case 17 : //�пյ�
		strcat( minimapnamestr, "map_Paewangdong.tga" );
		break;
	case 18 : //���޶���
		strcat( minimapnamestr, ".tga" );
		break;
	}

	//����� �޸�
	LPBYTE pImageVFMemory = NULL;

	//������� ã�´�
	int nRes = TextureArchive.FindResource( minimapnamestr );

	if( nRes < 0 )
		return FALSE;

	int   filesize = TextureArchive.GetPackedFileSize(nRes);
	FILE* pFP = TextureArchive.GetPackedFile(nRes);
	
	pImageVFMemory = new BYTE[ filesize ];

	if( !pImageVFMemory )
		return FALSE;
	
	if( fread( pImageVFMemory, filesize, 1, pFP ) < 1 )
		return FALSE;

	if( FAILED( D3DXCreateTextureFromFileInMemoryEx( gpDev, pImageVFMemory, filesize, 
		D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
		D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
		0, NULL, NULL, &MapData.pImageTex) ) )
	{
		return FALSE;
	}
	
	delete[] pImageVFMemory;

	MapData.pMapImage = new _XImageStatic;

	if( !MapData.pMapImage )
		return FALSE;

	MapData.pMapImage->Create( 0, 0, 512, 512, &g_MainInterfaceTextureArchive, -1 );
	MapData.pMapImage->SetClipRect( 0, 0, 512, 512 );
	MapData.pMapImage->SetScale( 0.58f, 0.58f );

	return TRUE;
}

void XWindow_WorldMap::SetStringColor( const char* szString, DWORD& dwColor )
{
	dwColor = 0;
	//���ڸ��� ���ڸ� ���ڷ� ��ȯ
	int nNum[255] = {0,};
	int nStrlen = strlen(szString);

	for( int i = 0; i < nStrlen; i++ )
	{
		if( szString[i] >= '0' && szString[i] <= '9' )
		{
			nNum[i] = szString[i] - '0';
		}
		else if( szString[i] >= 'a' && szString[i] <= 'f' )
		{
			nNum[i] = szString[i] - 'a' + 10;
		}
		else if( szString[i] >= 'A' && szString[i] <= 'F' )
		{
			nNum[i] = szString[i] - 'A' + 10;
		}
		else
		{
			//16������ ��Ÿ���� ���ڰ� �ƴҰ��
			nNum[i] = 0;
		}
	}

	//���ڸ��� ���ڸ� �¼��� ���ؼ�.. 10������ �����
	i = 0;
	int j = nStrlen;
	for( ; i < nStrlen; i++, j-- )
	{
		//16�� j�� // ���ڴ� �տ��� ���� ���� 
		int multiplier = 1;
		for( int h = j-1; h >= 1; h-- )
			multiplier *= 16;

		dwColor = dwColor + ( nNum[i] * multiplier );
	}
}

void XWindow_WorldMap::DestroyWindow()
{
	_XWindow::DestroyWindow();

	if(m_pCharinfo)
	{
		delete m_pCharinfo;
		m_pCharinfo = NULL;
	}

	map<int, MapInfo*>::iterator iter;

	if(m_mpData.size() > 0)
	{
		for( iter = m_mpData.begin(); iter != m_mpData.end(); ++iter )
		{
			MapInfo* pData = iter->second;
			pData->vtLinkID.clear();
			
			if( pData->pMapImage )
			{
				delete pData->pMapImage;
				pData->pMapImage = NULL;
			}

			if(pData->pImageTex )
			{
				pData->pImageTex->Release();
				pData->pImageTex = NULL;
			}
			
			delete iter->second;
		}
		m_mpData.clear();
	}

	vector<_XTextureAnimation*>::iterator vtTexiter = vtTextureAni.begin();
	for( ; vtTexiter != vtTextureAni.end(); ++vtTexiter )
	{
		delete (*vtTexiter);
	}

	vtTextureAni.clear();

	SAFE_RELEASE( m_pWorldmapTex );
	SAFE_DELETE( m_pWorldMapImage );
}

void XWindow_WorldMap::ShowWindow( BOOL show )
{
	m_nFindKey = -1;
	m_pBoard->ShowWindow(FALSE);
	
#ifdef _ACCLAIM_RUBICONADSYSTEM
		if( (g_pLocalUser->m_AccountItemcType >= 0 && g_pLocalUser->m_AccountItemsID > 0) || 
			g_RubiconADFreeLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
		{
			g_ViewRubiconAD = FALSE;
			::ShowWindow(g_hRubiconADWindow, SW_HIDE);
		}
		else
		{
			g_ViewRubiconAD = TRUE;
			::ShowWindow(g_hRubiconADWindow, SW_SHOW);
		}
#endif
	
	vector<_XTextureAnimation*>::iterator vtTexiter = vtTextureAni.begin();
	for( ; vtTexiter != vtTextureAni.end(); ++vtTexiter  )
	{
		(*vtTexiter)->ShowWindow(FALSE);
	}

	m_pCharinfo->ShowWindow( show );

	if(show)
	{
		SetDrawPostion( m_nFirstPos.x, m_nFirstPos.y );

		switch(g_CurrentZoneInfoIndex)
		{
		case 1 : //�Ҹ�
			SetCharPostion(1);
			break;
		case 2 : //����
			SetCharPostion(0);
			break;
		case 3 : //�츲��
			SetCharPostion(3);
			break;
		case 4 : //���
			SetCharPostion(4);
			break;
		case 5 : //����
			SetCharPostion(2);
			break;
		case 6 : //����
			SetCharPostion(5);
			break;
		case 7 : //����
			SetCharPostion(8);
			break;
		case 8 : //������
			SetCharPostion(9);
			break;
		case 9 : //����
			SetCharPostion(10);
			break;
		case 10 : //�뵿
			SetCharPostion(11);
			break;
		case 11 : //��â
			SetCharPostion(12);
			break;
		case 13 : //����
			SetCharPostion(13);
			break;
		case 16 : //�䵿
			SetCharPostion(14);
			break;
		case 17 : //�պ�
			SetCharPostion(6);
			break;
		case 20 : //������
			SetCharPostion(16);
			break;
		case 21 : //�пյ�
			SetCharPostion(17);
			break;
		case 23 : //���غ���
			SetCharPostion(15);
			break;
		default :
			m_pCharinfo->ShowWindow(FALSE);
		}
	}

	_XWindow::ShowWindow( show );
}

void XWindow_WorldMap::SetCharPostion(int nKey )
{
	map<int, MapInfo*>::iterator iter;
	
	// �ε�����ȣ�� ã�´�
	iter = m_mpData.find( nKey );

	// ������ ����
	if( iter == m_mpData.end() )
	{
		m_pCharinfo->ShowWindow(FALSE);
		return;
	}

	POINT pos = iter->second->IconPos;
	m_pCharinfo->SetDrawPostion( m_nPos.x + pos.x, m_nPos.y + (pos.y - 32) );
}

void XWindow_WorldMap::MoveWindow( int X, int Y )
{			
	_XWindow::MoveWindow( X, Y );
}

void XWindow_WorldMap::Draw( _XGUIObject*& pfocusobject )
{
	//�׸��� ����
	if( !this->m_ShowWindow ) return;

	m_pWorldMapImage->Draw( m_nPos.x, m_nPos.y, m_pWorldmapTex );

	if(SetAlphaProcess(m_bAlphaProcess))
		return;

	m_pCharinfo->Update(0.1f);

	_XWindow::Draw(pfocusobject);

	if( m_nFindKey == -1 )
		return;

	map<int, MapInfo*>::iterator iter;
	iter = m_mpData.find( m_nFindKey );
	if( iter == m_mpData.end() )
		return;

	if( !(iter->second->bEnable) )
		return;

	_XDrawSolidBar( m_nPos.x + 30, m_nPos.y + 68, 
					m_nPos.x + 30 + 320, m_nPos.y + 68 + 550, 0x60000000 );
	//_XDrawRectAngle( m_WindowPosition.x + 30, m_WindowPosition.y + 118, m_WindowPosition.x + 30 + 320, m_WindowPosition.y + 118 + 388, 1.0f, 0xFF000000 );

	g_XLargeFont->SetColor( _XSC_DEFAULT_WHITE );
	g_XLargeFont->PutsAlign(m_nPos.x + 180, m_nPos.y + 60, 
							_XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(iter->second->nName));
	g_XLargeFont->Flush();

	if(iter->second->pImageTex)
		iter->second->pMapImage->Draw( m_nPos.x + 37, m_nPos.y + 90, iter->second->pImageTex );

	g_XBaseFont->SetColor( iter->second->dwMapInfoColor_1 );
	g_XBaseFont->Puts_SeparateAlign( m_nPos.x + 40, m_nPos.y + 400, 
									_XGETINTERFACETEXT(iter->second->nMapInfo_1), 305, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );

	g_XBaseFont->SetColor( iter->second->dwMapInfoColor_2 );
	g_XBaseFont->Puts_SeparateAlign( m_nPos.x + 40, m_nPos.y + 520, 
									_XGETINTERFACETEXT(iter->second->nMapInfo_2), 250, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );

	g_XBaseFont->Flush();

	//��ũ�� ���ϸ��̼� �ؽ��� ��
	vector<_XTextureAnimation*>::iterator vtTexiter = vtTextureAni.begin();
	for( ; vtTexiter != vtTextureAni.end(); ++vtTexiter )
	{
		(*vtTexiter)->Update(0.1f);
	}
}

BOOL XWindow_WorldMap::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	if( m_nFindKey == -1 )
		return FALSE;

	map<int, MapInfo*>::iterator iter;
	iter = m_mpData.find( m_nFindKey );
	if( iter == m_mpData.end() )
		return FALSE;

	return TRUE;
}

void XWindow_WorldMap::SetKey( const int nKey )
{
	map<int, MapInfo*>::iterator iter;
	
	// �ε�����ȣ�� ã�´�
	iter = m_mpData.find( nKey );

	// ������ ����
	if( iter == m_mpData.end() )
		return;

	//����� ��ũ ���̵��� ������ �������� ã�Ƽ� ���ϸ��̼��� ��ġ�� �ٲ��ش� 
	vector<_XTextureAnimation*>::iterator vtTexiter = vtTextureAni.begin();
	vector<int>::iterator vtiter = iter->second->vtLinkID.begin();

	for( ; vtTexiter != vtTextureAni.end(); ++vtTexiter  )
	{
		map<int, MapInfo*>::iterator tempiter = m_mpData.find( (*vtiter) );

		if( tempiter == m_mpData.end() )
		{
			//�ش� �������� ���� ���� ������
			(*vtTexiter)->ShowWindow(FALSE); 
			if( vtiter != iter->second->vtLinkID.end() )
				++vtiter;

			continue;
		}

		if( vtiter != iter->second->vtLinkID.end() )
			++vtiter;
		else
		{
			(*vtTexiter)->ShowWindow(FALSE);
			continue;
		}

		POINT pos = tempiter->second->IconPos;
		(*vtTexiter)->SetDrawPostion( m_nPos.x + pos.x, m_nPos.y + pos.y );
		(*vtTexiter)->ShowWindow(TRUE);
	}

	m_nFindKey = nKey;
	m_pBoard->ShowWindow(TRUE);
}

BOOL XWindow_WorldMap::SetAlphaProcess(BOOL bShow)
{
	if( bShow )
	{
		int nAlpha = m_pWorldMapImage->m_FColor >> 24;

		nAlpha += 15;

		if( nAlpha >= 255 )
		{
			m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
			return FALSE;
		}
		else
			m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );		
	}
	else
	{
		int nAlpha = m_pWorldMapImage->m_FColor >> 24;

		nAlpha -= 15;

		if( nAlpha <= 0 )
		{
			m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( 0, 255, 255, 255 );
			ShowWindow(FALSE);
			return FALSE;
		}
		else
			m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	}

	return TRUE;
}
// XTagString.cpp: implementation of the XTagString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "XTagString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XTagString::XTagString() : m_ltString()
{
}

XTagString::~XTagString(void)
{
	list<TagInfo*>::iterator iter;

	for( iter = m_ltString.begin(); iter != m_ltString.end(); ++iter )
	{
		//(*iter)->strString.clear();
		delete (*iter);
	}
	m_ltString.clear();
}

void XTagString::StringClear()
{
	list<TagInfo*>::iterator iter;

	for( iter = m_ltString.begin(); iter != m_ltString.end(); ++iter )
	{
		//(*iter)->strString.clear();
		delete (*iter);
	}
	m_ltString.clear();
}

void XTagString::Update()
{
	if(m_ltString.empty())
		return;

	list<TagInfo*>::iterator iter = m_ltString.begin();

	//������ġ�� �Ѿ����... ������ġ..
	int nCurrentDrawPosY = 0;

	for( ; iter != m_ltString.end(); ++iter )
	{
		if( !((*iter)->strString.empty()) )
		{
            //printf( (*iter)->strString.c_str() );
			//printf( "\n�轺Ʈ��,,    ���� ���� : %d, �÷��� : %x\n\n", (*iter)->bStrong, (*iter)->dwColor);
		}
        //g_XBaseFont->SetBoldMode((*iter)->bStrong);
		//g_XBaseFont->SetColor((*iter)->dwColor);
		
		//�ΰ������ι���
		//g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, (*iter)->strString );
		//nCurrentDrawPosY += 16; //������ ���ʹ� �Ʒ��ٷ� ����ؾ� ������....

		//g_XBaseFont->SetColor(_XSC_DEFAULT);
		//g_XBaseFont->SetBoldMode(FALSE);
	}
}

void XTagString::UpdateListBox( _XListBox& listbox, int& linecount)
{
	if(m_ltString.empty())
		return;

	TCHAR buffer[2048];

	list<TagInfo*>::iterator iter = m_ltString.begin();

	for( ; iter != m_ltString.end(); ++iter )
	{
		//����Ʈ �ڽ��� ������..
		_XStringSplitInfo splitinfo; //�۾��� ��������
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			g_XBaseFont->SplitString( (*iter)->strString.c_str(), 300, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
		}
		else
		{
			g_XBaseFont->SplitStringByCR( (*iter)->strString.c_str(), splitinfo); //��Ʈ���� \n���� ������..
		}

		for(int i = 0 ; i < splitinfo.splitcount ; i++) //���� ��ŭ..
		{
			memset(buffer, 0, sizeof(buffer));
			strncpy(buffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
			listbox.InsertListItem(_T(""), 0, 0);
			listbox.SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

			listbox.SetItemText(linecount, 1, buffer, (*iter)->bStrong); //3��° ���ڰ� �۾�ü ���ϱ� ����
			listbox.SetItemAttrib(linecount, 1, (*iter)->dwColor);

			linecount++;
		}
	}
	//��..
}

void XTagString::StringCompare(string pStirng, BOOL bBold, DWORD dwColor)
{
	if( pStirng.empty() )
		return; 

	string::size_type colorindex = 0;
	string::size_type boldindex = 0;
	string::size_type Endcolorindex = 0;
	string::size_type Endboldindex = 0;

	string tempStr(pStirng);
	
	boldindex = tempStr.find( "<bold>" );
	colorindex = tempStr.find( "<color=");

	if( (boldindex == string::npos) && (colorindex == string::npos) ) // �±װ� ���� ���� �ູ�� ��� //��� ȣ��� ��Ʈ������
	{
		TagString(pStirng, bBold, dwColor);
		return;
	}

	if( boldindex != string::npos ) //���尡 �ִ�.. 
	{
		Endboldindex = tempStr.find("</bold>");
		if( Endboldindex != string::npos ) // ���� �˻�
		{
			string boldClearStr(tempStr.begin(), tempStr.begin()+boldindex); 
			StringCompare(boldClearStr, FALSE, 0xFF000000);

			//���� ��Ʈ��
            string saveStr( tempStr.begin()+boldindex + 6, tempStr.begin()+Endboldindex); //���� �ڿ� ���� �±׸��°� �ձ���
			StringCompare(saveStr, TRUE, dwColor);

			//��Ʈ�� ������ �κ��� �����Ѵ�...
			tempStr.erase( tempStr.begin(), tempStr.begin()+Endboldindex + 7);
			StringCompare(tempStr, FALSE, 0xFF000000);

			return;
		}
	}

	if( colorindex != string::npos ) //�÷��� ����...
	{
		//�÷��� ����� ���õǾ��ִ��� �˻��Ѵ�
		string checkStr(tempStr.begin()+colorindex, tempStr.end());
		Endcolorindex = tempStr.find("</color>");
		
		if( checkStr.find(">") == 15 )
		{
			if( Endcolorindex != string::npos ) // �÷��� ����..
			{
				//�÷��� ����
				string colorStr( checkStr.begin()+7, checkStr.begin()+15);
				SetStringColor( colorStr.c_str(), dwColor );
			}
			string colorClearStr(tempStr.begin(), tempStr.begin()+colorindex); 
			StringCompare(colorClearStr, FALSE, 0xFF000000);

			//�÷� �� ���� ��Ʈ��
			string printStr(tempStr.begin()+colorindex+16, tempStr.begin()+Endcolorindex);
			StringCompare(printStr, bBold, dwColor );

			//��Ʈ�� ������ �κ��� �����Ѵ�...
			tempStr.erase( tempStr.begin(), tempStr.begin()+Endcolorindex + 8);
			StringCompare(tempStr, FALSE, 0xFF000000);
			return;
		}
	}

	TagString(pStirng, bBold, dwColor);
}

void XTagString::CheckBold(const string::size_type& index, const string pString)
{
}

void XTagString::CheckColor(const string::size_type& index, const string pString)
{
}

void XTagString::SetStringColor( const char* szString, DWORD& dwColor )
{
	dwColor = 0;
	//���ڸ��� ���ڸ� ���ڷ� ��ȯ
	int nNum[255] = {0,};
	size_t nStrlen = strlen(szString);

	for( unsigned int i = 0; i < nStrlen; i++ )
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
	size_t j = nStrlen;
	for( ; i < nStrlen; i++, j-- )
	{
		//16�� j�� // ���ڴ� �տ��� ���� ���� 
		int multiplier = 1;
		for( size_t h = j-1; h >= 1; h-- )
			multiplier *= 16;

		dwColor = dwColor + ( nNum[i] * multiplier );
	}
}
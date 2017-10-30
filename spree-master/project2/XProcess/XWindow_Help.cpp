// XWindow_Help.cpp: implementation of the _XWindow_Help class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_Help.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Help::_XWindow_Help()
{

}

_XWindow_Help::~_XWindow_Help()
{

}

BOOL _XWindow_Help::Initialize( void )
{
	m_SettingOn = FALSE;
	m_TutorialState = 0;	// zerolevel
	m_MiniGameState = MT_NONE;
	m_WindowPosition.x += gnWidth-1024;
	m_WindowPosition.y += gnHeight-768;

//	int closebuttonindex = g_MainInterfaceTextureArchive.FindResource( "window_closebutton.tga" );
//	
//	_XBTN_STRUCTURE minimizebtnstruct = { TRUE, { 192, 0 },{ 12, 12 }, _XDEF_CI_CLOSEBUTTON, 
//		closebuttonindex,closebuttonindex,closebuttonindex, 
//		&g_MainInterfaceTextureArchive };
//	_XButton* minimizebutton = new _XButton;
//	minimizebutton->Create( minimizebtnstruct );
//	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0,  0, 12, 12 );
//	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  12, 0, 24, 12 );
//	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  24, 0, 36, 16 );
//	InsertChildObject( minimizebutton );
	// modify end
	
	// �̴ϰ���(�������)�� ���� �̹��� -----------------------------------------------------------------------------------------------��
	for( int i = 0; i < 5; i++ )
	{
		m_Mini_Meditation1[i].Create( m_WindowPosition.x+5, m_WindowPosition.y+(i*44)+53,
									  m_WindowPosition.x+5+31, m_WindowPosition.y+(i*44)+53+31,
									  &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("help.tga") );
		m_Mini_Meditation1[i].SetClipRect((i* 32), 0, (i* 32)+31, 32);
	}
	// -------------------------------------------------------------------------------------------------------------------------��

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {442, 286}, {46, 17},
		_XDEF_TRAINING_HELP_CLOSE,
		g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
		g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
		g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
		&g_MainInterfaceTextureArchive
	};
	m_CloseButton = new _XButton;
	m_CloseButton->Create(btnstruct);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 13, 47, 30);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 30, 47, 48);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 47, 47, 64);
	m_CloseButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_CloseButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_OK);	// _T("Ȯ��")
	InsertChildObject(m_CloseButton);
	
	SetWindowMoveMode(_XWMOVE_FIXED);

	return TRUE;
}

void _XWindow_Help::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw(pfocusobject);	

	if( !this->m_ShowWindow ) return;

	switch(m_MiniGameState) 
	{
	case MT_MEDITATION:
		{
			for( int i = 0; i < 5; i++ )
			{
				m_Mini_Meditation1[i].m_Position.x = m_WindowPosition.x+5;
				m_Mini_Meditation1[i].m_Position.y = m_WindowPosition.y+(i*44)+53;
				m_Mini_Meditation1[i].DrawWithRegion();
			}
			DrawMeditationText();				
			_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, 
				m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, _XSC_DEFAULT_WHITE );
		}
		break;

	case MT_DUMMY :
		{
			DrawDummyText();
			_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, 
				m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, _XSC_DEFAULT_WHITE );
		}
		break;
		
	case MT_SPELL :
		{
			DrawSpellText();
			_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, 
				m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, _XSC_DEFAULT_WHITE );
		}
		break;
	case MT_TUTORIAL :		// zerolevel
		{
			DrawTutorialText();			
		}
		break;
	}
}

BOOL _XWindow_Help::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow )
	{
		m_SettingOn = FALSE;
		return FALSE;
	}

	if( !this->m_Enable  ) return FALSE;
	
	if( !m_SettingOn )
	{		
		switch(m_MiniGameState) 
		{
		case MT_MEDITATION:
			{
				// ������ ����
				SetUseBorderImage(FALSE);
				SIZE tmpsize;
				tmpsize.cx = 500;
				tmpsize.cy = 308;
				m_WindowPosition.x = (gnWidth-tmpsize.cx)/2;
				m_WindowPosition.y = (gnHeight-tmpsize.cy)/2;						
				SetWindowSize(tmpsize);
				//m_CloseButton->MoveWindow(442, 286);
				m_SettingOn = TRUE;
			}
			break;
			
		case MT_DUMMY :
			{
				// ������ ����
				SetUseBorderImage(FALSE);
				SIZE tmpsize;
				tmpsize.cx = 500;
				tmpsize.cy = 308;
				m_WindowPosition.x = (gnWidth-tmpsize.cx)/2;
				m_WindowPosition.y = (gnHeight-tmpsize.cy)/2;						
				SetWindowSize(tmpsize);
				m_CloseButton->MoveWindow(m_WindowPosition.x+442, m_WindowPosition.y+286);
				m_SettingOn = TRUE;
			}
			break;
			
		case MT_SPELL :
			{
				// ������ ����
				SetUseBorderImage(FALSE);
				SIZE tmpsize;
				tmpsize.cx = 500;
				tmpsize.cy = 308;
				m_WindowPosition.x = (gnWidth-tmpsize.cx)/2;
				m_WindowPosition.y = (gnHeight-tmpsize.cy)/2;
				SetWindowSize(tmpsize);
				m_CloseButton->MoveWindow(m_WindowPosition.x+442, m_WindowPosition.y+286);
				m_SettingOn = TRUE;
			}
			break;
		case MT_TUTORIAL :		// zerolevel
			{
				// ������ ����
				SIZE tmpsize;
				tmpsize.cx = 387;
				tmpsize.cy = 182;
				 
				SetWindowSize(tmpsize);
				XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;
				int index = pZeroLevelProcess->GetTextureZero()->FindResource( "talkfaceback.tga" );
				SetUseBorderImage(TRUE);								
				SetBorderImage(pZeroLevelProcess->GetTextureZero(), index);
				MoveWindow( ((gnWidth>>1)-(tmpsize.cx>>1)), ((gnHeight>>1)-(tmpsize.cy>>1))-150 );
				m_borderstatic.SetClipRect( 0, 280, 387, 462 );
				
				m_CloseButton->MoveWindow(m_WindowPosition.x+170, m_WindowPosition.y+155);
				g_MainWindowManager.SetTopWindow(this);
				m_SettingOn = TRUE;
			}
			break;
		}		
	}

	if( !_XWindow::Process(pfocusobject) ) return FALSE;

	return TRUE;
}

void _XWindow_Help::SetWindowSize( SIZE size )
{
	m_WindowSize = size;
	m_solidborderstatic.MoveWindow( m_WindowPosition.x, m_WindowPosition.y, 
		m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy );
}

void _XWindow_Help::DrawMeditationText()
{
	TCHAR szText[256];	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2822) );//_T("��������� �����ϸ� '����'�� ������� '���'�� '���'�� ���� ���� �� ���� ���ϴ�.") );	
	g_XBaseFont->SetColor( 0xFFFFFF00 );
	g_XBaseFont->Puts( m_WindowPosition.x + 5 +2, m_WindowPosition.y + 5 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2823) );//_T("'���'�� '����' ���� �������� ��ġ�ϴ� ���� ���콺 ��ư�� Ŭ���ϼ���.") );	
	g_XBaseFont->SetColor( 0xFFFFFF00 );
	g_XBaseFont->Puts( m_WindowPosition.x + 5 +2, m_WindowPosition.y + 19 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2824) );//_T("������ ����(����)") );	
	g_XBaseFont->SetColor( 0xFFFF0000 );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 53 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2825) );//_T("�ʼ����� �����μ� ") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 67 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2826) );//_T("<���콺 ���� ��ư Ŭ��>") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 148, m_WindowPosition.y + 67 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2827) );//_T("�� �����մϴ�.") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 290 +2, m_WindowPosition.y + 67 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2828) );//_T("�Ķ��� ����(����)") );	
	g_XBaseFont->SetColor( 0xFF0000FF );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 97 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2829) );//_T("�ΰ����� �����μ� ") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 111 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2830) );//_T("<���콺 ������ ��ư Ŭ��>") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 148 +2, m_WindowPosition.y + 111 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2831) );//_T("�� �����մϴ�.") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 302 +2, m_WindowPosition.y + 111 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2832) );//_T("���(���)") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 141 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2833) );//_T("�� ���� ���(��թ)�� ���� �����̴�") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 155 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2834) );//_T("��(Ѩ) ���") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 240 +2, m_WindowPosition.y + 155 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2835) );//_T("�Դϴ�.") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 318 +2, m_WindowPosition.y + 155 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2836) );//_T("���(��թ)") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 185 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2837) );//_T("������ ������ ������") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 199 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2838) );//_T("���(���)�� �̵� ���") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 164 +2, m_WindowPosition.y + 199 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2839) );//_T("�Դϴ�.") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 296 +2, m_WindowPosition.y + 199 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2840) );//_T("����(ӡ�)") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 229 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2841) );//_T("��(Ѩ)�� ���߽�Ű�� �����μ�") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 243 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2842) );//_T("��������� ���۵Ǵ� ����") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 210 +2, m_WindowPosition.y + 243 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2843) );//_T("�Դϴ�.") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 353 +2, m_WindowPosition.y + 243 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2844) );//_T("������: ��������� ���۵� �Ŀ��� ���� ������ ���߿� ���� �� �����ϴ�.") );	
	g_XBaseFont->SetColor( 0xFFFFFF00 );
	g_XBaseFont->Puts( m_WindowPosition.x + 5, m_WindowPosition.y + 293, szText );
	g_XBaseFont->Flush();	
}

void _XWindow_Help::DrawDummyText(void)
{
	g_XBaseFont->SetColor(0xFF999999);
	g_XBaseFont->Puts(m_WindowPosition.x+5+2, m_WindowPosition.y+19+2, _XGETINTERFACETEXT(ID_STRING_NEW_2845) );//"����ġ�� ����");
	g_XBaseFont->Flush();
}

void _XWindow_Help::DrawSpellText(void)
{
	g_XBaseFont->SetColor(0xFF999999);
	g_XBaseFont->Puts(m_WindowPosition.x+5+2, m_WindowPosition.y+19+2, _XGETINTERFACETEXT(ID_STRING_NEW_2846) );//"��� ���� ����");
	g_XBaseFont->Flush();
}

void _XWindow_Help::DrawTutorialText(void)
{
	//this->SetBorderColorFactor( D3DCOLOR_ARGB( 100, 0,0,0 ) );

	switch(m_TutorialState) 
	{
	case 1:
		g_XBaseFont->SetColor(0xFF351507);
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF9B7527 );
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+20, _XGETINTERFACETEXT(ID_STRING_NEW_2847) );//"                    [ ���� ��ġ�� ]");		
		g_XBaseFont->DisableGrowMode();
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+50, _XGETINTERFACETEXT(ID_STRING_NEW_2848) );//"���콺�� ������Ŭ���� ���� ä�� �����̸�");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+65, _XGETINTERFACETEXT(ID_STRING_NEW_2849) );//"������ ��ȯ�� �� �ֽ��ϴ�.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+80, _XGETINTERFACETEXT(ID_STRING_NEW_2850) );//"���콺�� ���� �̿��� �ָ� ���ų� ������ �� ���� �ֽ��ϴ�.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+110, _XGETINTERFACETEXT(ID_STRING_NEW_2851) );//"���� ������ ������ �����, 1��Ī��尡 �˴ϴ�.");
		g_XBaseFont->Flush();		
		break;
	case 2:
		g_XBaseFont->SetColor(0xFF351507);
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF9B7527 );
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+20, _XGETINTERFACETEXT(ID_STRING_NEW_2852) );//"                    [ �̴ϸ� ���� ]");		
		g_XBaseFont->DisableGrowMode();
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+50, _XGETINTERFACETEXT(ID_STRING_NEW_2853) );//"ȭ���� ������ ��ܿ� �̴ϸ��� ��ġ�� �ֽ��ϴ�.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+65, _XGETINTERFACETEXT(ID_STRING_NEW_2854) );//"Ȯ���Ϸ��� F2Ű��,");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+80, _XGETINTERFACETEXT(ID_STRING_NEW_2855) );//"����Ϸ��� F3Ű��,");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+95, _XGETINTERFACETEXT(ID_STRING_NEW_2856) );//"ū ���� ���÷��� �� ��ư�� �����ּ���.");
		g_XBaseFont->Flush();
		break;	
	case 3:
		g_XBaseFont->SetColor(0xFF351507);
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF9B7527 );
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+20, _XGETINTERFACETEXT(ID_STRING_NEW_2857) );//"                      [ �����̶� ]");		
		g_XBaseFont->DisableGrowMode();
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+50, _XGETINTERFACETEXT(ID_STRING_NEW_2858) );//"������ ������ ���� �����");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+65, _XGETINTERFACETEXT(ID_STRING_NEW_2859) );//"�ϳ��� ������ �� ���� �ʽ����� �̷���� �ֽ��ϴ�.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+80, _XGETINTERFACETEXT(ID_STRING_NEW_2860) );//"������ �ܰ谡 ����Ҽ��� �ʽ��� ���ڰ� �þ��,");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+95, _XGETINTERFACETEXT(ID_STRING_NEW_2861) );//"���µ� ������ ���ϴ�.");
		g_XBaseFont->Flush();
		break;	
	case 4:
		g_XBaseFont->SetColor(0xFF351507);
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF9B7527 );
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+20, _XGETINTERFACETEXT(ID_STRING_NEW_2862) );//"                    [ ������ ��� ]");		
		g_XBaseFont->DisableGrowMode();
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+50, _XGETINTERFACETEXT(ID_STRING_NEW_2863) );//"���� �ϴ��� ţ���Կ� ������ ��ϵǾ� ������,");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+65, _XGETINTERFACETEXT(ID_STRING_NEW_2864) );//"1~9�������� ����Ű�� �̿��� ������ ������ �� �ֽ��ϴ�.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+80, _XGETINTERFACETEXT(ID_STRING_NEW_2865) );//"�� �� ���ϴ� ��븦 Ŭ���ϸ� ������ �����մϴ�.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+95, _XGETINTERFACETEXT(ID_STRING_NEW_2866) );//"(����� 1���� �����Դϴ�.)");
		g_XBaseFont->Flush();
		break;	
	case 5:
		g_XBaseFont->SetColor(0xFF351507);
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF9B7527 );
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+20, _XGETINTERFACETEXT(ID_STRING_NEW_2867) );//"                      [ ������ų ]");		
		g_XBaseFont->DisableGrowMode();
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+50, _XGETINTERFACETEXT(ID_STRING_NEW_2868) );//"�⸦ ��� ��뿡�� Ÿ���� �ִ� ����");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+65, _XGETINTERFACETEXT(ID_STRING_NEW_2869) );//"'������ų'�̶�� �մϴ�.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+80, _XGETINTERFACETEXT(ID_STRING_NEW_2870) );//"������ų�� �⸦ �������� ��� ��������ν�,");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+95, _XGETINTERFACETEXT(ID_STRING_NEW_2871) );//"���� ������ ������ ������ �� �ֽ��ϴ�.");
		g_XBaseFont->Flush();
		break;	
	case 6:
		g_XBaseFont->SetColor(0xFF351507);
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF9B7527 );
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+20, _XGETINTERFACETEXT(ID_STRING_NEW_2872) );//"                  [ ���� ��ų�� ��� ]");	
		g_XBaseFont->DisableGrowMode();
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+50, _XGETINTERFACETEXT(ID_STRING_NEW_2873) );//"������ų�� ����ϱ� ���ؼ��� ����, ����â�� ����� ����");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+65, _XGETINTERFACETEXT(ID_STRING_NEW_2874) );//"���� ��ų�� ����Ű�� ����, �⸦ �����ϴ�.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+80, _XGETINTERFACETEXT(ID_STRING_NEW_2875) );//"(����� 2���� ���� ��ų �Դϴ�.)");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+110, _XGETINTERFACETEXT(ID_STRING_NEW_2876) );//"����Ű�� ������ ������ �ɸ��� �Ӹ� ���� �Ķ��� ���밡");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+125, _XGETINTERFACETEXT(ID_STRING_NEW_2877) );//"���� ���� ���� ���� �� �Դϴ�.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+140, _XGETINTERFACETEXT(ID_STRING_NEW_2878) );//"�� �� �����⸦ �����ϸ�, ���� ��ų�� �����˴ϴ�.");
		g_XBaseFont->Flush();
		break;	
	}	
}
// XProc_SelectHero.cpp: implementation of the XProc_SelectHero class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XProc_SelectHero.h"
#include "XProc_ZeroLevel.h"

#include "EODEXTDEF_GlobalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XProc_SelectHero::XProc_SelectHero()
{
	setProcessInfo( 0, "Select hero", __FILE__ );
}

XProc_SelectHero::~XProc_SelectHero()
{

}

BOOL XProc_SelectHero::InitializeProcess(void)
{
	m_Player.SetPitchLimits(-45, 45);
	m_Player.SetRollLimits(-10, 10);
	m_Player.SetType(camFly);

	m_TextureArchive.DisposeTexture();	

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_INTERFACE.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_INTERFACE.XP") );
#endif
	
	m_TextureArchive.SetTexturePackage( packagefilename );

	m_TextureArchive.GetResourceIndex("HeroSelect.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_soora.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_fist.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_bull.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_noklim.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_namgoong.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_hosansan.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_devil.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_sword.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_lightning.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("zerolevel_startbutton.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);

	m_TextureArchive.BuildTextureList(TRUE);

	m_iDrawPosX = gnWidth - 1024;
	m_iDrawPosY = gnHeight - 768;

	int tmpimageindex = m_TextureArchive.FindResource("HeroSelect.tga");

	// ���� ���� : ���� - ö�� - �ҷ� - �츲 - ���� - ȣ�� - õ�� - �ɿ� - ���� 
	m_HeroCharacterImage[0].Create( m_iDrawPosX+788, m_iDrawPosY+293, m_iDrawPosX+788+183, m_iDrawPosY+293+324, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[0].SetClipRect( 260, 20, 443, 344 );
	m_HeroCharacterImage[0].m_FColor = 0xff888888;
	
	m_HeroNameImage[0].Create( 0, 0, 255, 255, &m_TextureArchive,m_TextureArchive.FindResource("hero_soora.tga") );
	m_HeroNameImage[0].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[1].Create( m_iDrawPosX+622, m_iDrawPosY+227, m_iDrawPosX+622+226, m_iDrawPosY+227+390, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[1].SetClipRect( 0, 0, 226, 390 );
	m_HeroCharacterImage[1].m_FColor = 0xff888888;
	m_HeroNameImage[1].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_fist.tga") );
	m_HeroNameImage[1].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[2].Create( m_iDrawPosX+369, m_iDrawPosY+375, m_iDrawPosX+369+250, m_iDrawPosY+375+242, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[2].SetClipRect( 202, 382, 452, 624 );
	m_HeroCharacterImage[2].m_FColor = 0xff888888;
	m_HeroNameImage[2].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_bull.tga") );
	m_HeroNameImage[2].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[3].Create( m_iDrawPosX+204, m_iDrawPosY+401, m_iDrawPosX+204+198, m_iDrawPosY+401+215, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[3].SetClipRect( 0, 415, 198, 630 );
	m_HeroCharacterImage[3].m_FColor = 0xff888888;
	m_HeroNameImage[3].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_noklim.tga") );
	m_HeroNameImage[3].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[4].Create( m_iDrawPosX+720, m_iDrawPosY+141, m_iDrawPosX+720+292, m_iDrawPosY+141+445, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[4].SetClipRect( 731, 578, 1023, 1023 );
	m_HeroCharacterImage[4].m_FColor = 0xff888888;
	m_HeroNameImage[4].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_namgoong.tga") );
	m_HeroNameImage[4].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[5].Create( m_iDrawPosX+549, m_iDrawPosY+262, m_iDrawPosX+549+266, m_iDrawPosY+262+355, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[5].SetClipRect( 0, 668, 266, 1023 );
	m_HeroCharacterImage[5].m_FColor = 0xff888888;
	m_HeroNameImage[5].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_hosansan.tga") );
	m_HeroNameImage[5].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[6].Create( m_iDrawPosX+318, m_iDrawPosY+233, m_iDrawPosX+318+338, m_iDrawPosY+233+383, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[6].SetClipRect( 281, 640, 619, 1023 );
	m_HeroCharacterImage[6].m_FColor = 0xff888888;
	m_HeroNameImage[6].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_devil.tga") );
	m_HeroNameImage[6].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[7].Create( m_iDrawPosX+383, m_iDrawPosY+115, m_iDrawPosX+383+304, m_iDrawPosY+115+494, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[7].SetClipRect( 719, 0, 1023, 494 );	
	m_HeroCharacterImage[7].m_FColor = 0xff888888;
	m_HeroNameImage[7].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_sword.tga") );
	m_HeroNameImage[7].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[8].Create( m_iDrawPosX+633, m_iDrawPosY+61, m_iDrawPosX+633+210, m_iDrawPosY+61+552, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[8].SetClipRect( 494, 0, 704, 552 );	
	m_HeroCharacterImage[8].m_FColor = 0xff888888;
	m_HeroNameImage[8].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_lightning.tga") );
	m_HeroNameImage[8].SetClipRect(0, 0, 255, 255);
	
	int btnimageindex = m_TextureArchive.FindResource( "zerolevel_startbutton.tga" );
	_XBTN_STRUCTURE startbtnstruct = { TRUE, { m_iDrawPosX+20, m_iDrawPosY+210 }, { 100, 30 }, _XDEF_ZEROLEVEL_START,
		btnimageindex,btnimageindex,btnimageindex, &m_TextureArchive };
	m_StartButton = new _XButton;
	m_StartButton->Create( startbtnstruct );
	m_StartButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 0,99,29 );
	m_StartButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0,60,99,89 );
	m_StartButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  0,30,99,59 );
	m_StartButton->AttachXFont(_XFONTID_LARGE);	
	m_StartButton->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	m_StartButton->SetButtonTextID( 0,0 );	// ���ӽ���
	

	m_StartButton->EnableWindow( FALSE );
	m_StartButton->ShowWindow( FALSE );
	
	m_iSelectedHero = -1;

	InitDeviceObject();

	// �ʱ�ȭ�� ������ ���μ��� Ȱ��ȭ..
	SetEnableProcess( TRUE );

	return TRUE;
}

void XProc_SelectHero::DestroyProcess( void )
{
	SetEnableProcess( FALSE );

	m_TextureArchive.DisposeTexture();
}

bool XProc_SelectHero::InitDeviceObject( void )
{
	m_TextureArchive.InitDeviceObject();

	m_Player.Set(m_Player.GetType(), 60, (float)gnWidth/(float)gnHeight, 0.001f, 10.0f);
	m_Player.UpdateViewMatrix();
	m_Player.UpdateProjMatrix();

	D3DVIEWPORT9 vp;
	vp.X = vp.Y = 0;
	vp.Width = gnWidth;
	vp.Height = gnHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	gpDev->SetViewport(&vp);

	gpDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	gpDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);

	gpDev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    gpDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    gpDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	
	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// 16��Ʈ ����� ��� ������� ���ش�.
	//if( gfmtDepth == D3DFMT_D16 ) gpDev->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	//else gpDev->SetRenderState( D3DRS_DITHERENABLE,   FALSE ); 
	
    gpDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    
	gpDev->SetRenderState( D3DRS_ZENABLE,        g_RS_UseZBuffer );
	gpDev->SetRenderState( D3DRS_ZFUNC,			 g_RS_ZBufferFunc );
	gpDev->SetRenderState( D3DRS_DEPTHBIAS ,			 0 );
	
#ifdef _XDEF_USETERRAINDIFFUSECOLOR
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_COLOR1 );
#else
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_MATERIAL);
#endif

	gpDev->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	gpDev->SetRenderState( D3DRS_LOCALVIEWER, FALSE );
	
	gpDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	
	// Alpha blend state
	gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,  FALSE );
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
	gpDev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );		
	gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	
	// Alpha test state
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );
	gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	
	D3DLIGHT9	light;
	D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, 0.0f, 160.0f, 0.0f );
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 0.5f;
	light.Range = 100.0f;
	gpDev->SetLight( 0, &light );

	gpDev->SetRenderState( D3DRS_LIGHTING, TRUE );	
	gpDev->LightEnable( 0, TRUE );	

	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

	return true;
}

void XProc_SelectHero::ReleaseDeviceObject( void )
{

}

void XProc_SelectHero::PauseProcess( void )
{

}
void XProc_SelectHero::UnPauseProcess( void )
{

}

void XProc_SelectHero::Draw( void )
{
	gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	
	// Begin the scene
	gpDev->BeginScene();

	m_Player.UpdateViewMatrix();
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_Player.GetViewMatrix());
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 5 );

	for( int i = 8; i >= 0; i-- )
	{
		m_HeroCharacterImage[i].DrawWithRegion();
		m_HeroCharacterImage[i].m_FColor = 0xff888888;
	}	
	if( m_iSelectedHero > -1 )
	{
		m_HeroNameImage[m_iSelectedHero].DrawWithRegion();
		DrawHeroSpeech(m_iSelectedHero);
		m_StartButton->Draw();
	}
	
	// End the scene
	gpDev->EndScene();
	
	// Present the backbuffer contents to the display	
	HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);	
	
	if( D3DERR_DEVICELOST == hr )
        gApp->m_bDeviceLost = true;

}

bool XProc_SelectHero::Process( void )
{
	static bool flag = true;
	m_StartButton->Process();

	for( int i = 0; i < 9; i++ )
	{
		if( m_HeroCharacterImage[i].CheckMousePosition() )
		{
			m_HeroCharacterImage[i].m_FColor = 0xffffffff;
			if( gpInput->GetMouseState()->bButton[0] && flag )
			{
				flag = false;
				m_iSelectedHero = i;
				if( m_iSelectedHero == 5 || m_iSelectedHero == 3 || m_iSelectedHero == 2 || m_iSelectedHero == 0 )
					m_StartButton->EnableWindow( TRUE );
				else
					m_StartButton->EnableWindow( FALSE );
				m_StartButton->ShowWindow( TRUE );
			}
			break;
		}
	}
	if( !gpInput->GetMouseState()->bButton[0] && !flag )
		flag = true;

	if( m_iSelectedHero > -1 )
		m_HeroCharacterImage[m_iSelectedHero].m_FColor = 0xffffffff;

	if( m_StartButton->CheckMousePosition() && gpInput->GetMouseState()->bButton[0] )
	{
		XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;
		switch(m_iSelectedHero) 
		{
		case 0 : // ������
			{
				pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_BEEGOONG);
				pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_SORIM);
				pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_WOMAN);
			}
			break;
		case 1 : // ö�� ������
			break;
		case 2 : // �ҷ�ȭ��
			{
				pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_SORIM);
				pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_BEEGOONG);
				pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_MAN);				
			}
			break;
		case 3 : // �츲��
			{
				pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_NOCKRIM);
				pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_GAEBANG);
				pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_MAN);				
			}			
			break;
		case 4 : // ������
			break;
		case 5 : // ȣ���
			{
				pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_GAEBANG);
				pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_NOCKRIM);
				pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_WOMAN);				
			}
			break;
		case 6 : // õ������
			break;
		case 7 : // �ɿ��
			break;
		case 8 : // ����
			break;
		}	
		
		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel );	
	}

	return true;	
}

void XProc_SelectHero::DrawHeroSpeech(int select)
{
	TCHAR szText[256];
	switch(select) 
	{
	case 0:
		{
			sprintf(szText, "%s", _T("����ִٴ� �� ���־�. ������ Į�� ���� �� �� �� �� �ɾ�� ����. ���� ���� ���̶� �ϳ��� ����.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			
			sprintf(szText, "%s", _T("�ƴ�, �� �ϳ� ����.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			
			sprintf(szText, "%s", _T("���� ������ ������ �������Գ� �����ϰ� ������ �´ٴ� �ž�. �Ƚ�ó�� ����� ������...") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();	
		}
		break;
	case 1:
		{
			sprintf(szText, "%s", _T("������� ����� ���� �з� ǥ���ϵ� ��ư���. ���� �Ѷ��� �׷���.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			
			sprintf(szText, "%s", _T("�׷��� ������ ���ο��� ���� ���޾Ҵ�. ����� ����� ���� ������ ����̶�� ����...") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			
			sprintf(szText, "%s", _T("���� ���� �� ����� ���� ��� ��Ʋ�� ���������� ��� ���Ѵ�. �� �� �ָ�����!") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 2:
		{
			sprintf(szText, "%s", _T("�� ��� ���ڸ�ġ ��پ� �����. �� ��� ���ڸ�ġ ��پ� �����. �� ��� ���ڸ�ġ...") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			
			sprintf(szText, "%s", _T("�뼭�϶�. ���� ���� ��踦 �� ���ۿ� ������.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			
			sprintf(szText, "%s", _T("���� ���� ������ �پ���� �ʰ� ��� �߻��� ���Ϸ�.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 3:
		{
			sprintf(szText, "%s", _T("�����ɾ� ��ٰ� �ذ�Ǵ� �� �ֳ�? �ϴ��� �����ؼ� �������� �� �־�?") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			
			sprintf(szText, "%s", _T("�� �ϱ� �����ΰž�.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			
			sprintf(szText, "%s", _T("� ó���� �������� �ּ��� ���ؾ���. �װ� �λ��̾�.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 4:
		{
			sprintf(szText, "%s", _T("���� ���� �Ӹ� ����, �ο� ���ϰ�, �� �����, �� ���ϰ�, ���ǵ� �ٸ���.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			
			sprintf(szText, "%s", _T("�װ� �ٷ� ����.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			
			sprintf(szText, "%s", _T("�Ѱ��� ������ �ִٸ�... ����.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 5:
		{
			sprintf(szText, "%s", _T("���δ� ���������� ����. ���δ� �������� ����.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			
			sprintf(szText, "%s", _T("��¼�� ���鿡 ����ɾ� �ִ� �ڽŵ� �� �ܷο��� �������� ����.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			
			sprintf(szText, "%s", _T("������ �λ� �� �� �ֳ���? ������ ��ô�.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 6:
		{
			sprintf(szText, "%s", _T("������ �Ұ����ϰ� �װ��� �ٷ������ �ŵ� ��ó�� ����. ���� �ʰ� ��ư��� ���ؼ��� �ڽ��� ���� �Ǵ� ���ۿ� ����.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			g_XBaseFont->Flush();
			
			sprintf(szText, "%s", _T("���� ���̰� ��ó��. ���� õ����. õ���� ������ ���� ���� ���ۿ� ����.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			g_XBaseFont->Flush();	

			sprintf(szText, "%s", _T("õ������, �װ� ����. �׸��� �װ� ���� ���̴�.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 7:
		{
			sprintf(szText, "%s", _T("�±��� �׸��� ������ ����. �� �ڷ� ���� ���س��� Ǫ���� �����Ҹ�.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			g_XBaseFont->Flush();
			
			sprintf(szText, "%s", _T("�׸���..... �޺� �Ʒ��� �׸���.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			g_XBaseFont->Flush();	

			sprintf(szText, "%s", _T("�� ��� ���� �� �� �ȿ� �ִ�.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 8:
		{
			sprintf(szText, "%s", _T("������ �¸��� �ڸ� �߽����� �����δ�. ������ ������ ������ ���� �Ұ��ϴ�. ������ ���� �� ���� ���̴�.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			g_XBaseFont->Flush();
			
			sprintf(szText, "%s", _T("���� �׻� ���ڰ� �Ǳ� ���Ѵ�.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			g_XBaseFont->Flush();	

			sprintf(szText, "%s", _T("�׷��Ƿ� ���� �ñ��� ������ �߱��Ѵ�.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;	
	}
}
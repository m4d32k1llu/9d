// XWindow_WorldMap.h: interface for the XWindow_WorldMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_WORLDMAP_H__D1D4838F_B55B_4819_82B0_B4D272379C5E__INCLUDED_)
#define AFX_XWINDOW_WORLDMAP_H__D1D4838F_B55B_4819_82B0_B4D272379C5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

#include <map>
#include <vector>
#include <string>
using namespace std;

#include "XTextureAnimation.h"

struct MapInfo
{
	int nID; //�� ���̵�
	int	nName; //���̸�
	
	POINT	IconPos; //�������� ��ǥ
	int	nIconIndex; // ������ �׸�

	int nMapInfo_1; //�� �ؽ�Ʈ ����
	DWORD	dwMapInfoColor_1; // ���ؽ�Ʈ �÷�

	int nMapInfo_2; //�� �ؽ�Ʈ ����
	DWORD	dwMapInfoColor_2; // ���ؽ�Ʈ �÷�

	vector<int>	vtLinkID; //��ũ �� ���̵�
	BOOL bEnable; //�ش���� ������� ����

	_XImageStatic* pMapImage; //�ش� �̴ϸ�
	LPDIRECT3DTEXTURE9 pImageTex; 

	MapInfo()
	{
		vtLinkID.clear();
		pMapImage = NULL;
		pImageTex = NULL;
	}
};

class XWindow_WorldMap : public _XWindow
{
public:
	XWindow_WorldMap();
	virtual ~XWindow_WorldMap();

	BOOL	Initialize(XMLPaser& m_xXML);
	void	DestroyWindow( void );
	
	void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void	ShowWindow(BOOL show);
	void	MoveWindow( int X, int Y );

	BOOL	SetShowWindowProcess( BOOL bShow );

	void    SetKey( const int nKey );
	void	SetDrawPostion( int nX, int nY );
	void	FirstPostion( int nX, int nY );
	
private :
	BOOL	LoadXMLTable(XMLPaser& m_xXML, LPTSTR pTableName );
	BOOL	LoadFile( FILE* fp );
	
	BOOL	SetAlphaProcess(BOOL bShow);
	BOOL	SetMiniMapImage(_XPackageArchive& TextureArchive, MapInfo& MapData );
	void	SetButton( const int nID, const MapInfo& MapData ); 
	void	SetStringColor( const char* szString, DWORD& dwColor );
	void	SetCharPostion(int nKey);

private :
	LPDIRECT3DTEXTURE9 m_pWorldmapTex;
	_XImageStatic*	m_pWorldMapImage;
	_XTextureAnimation* m_pCharinfo; //�ɸ��� ��ġ

	_XImageStatic* m_pBoard;

	map<int, MapInfo*>	m_mpData; //���̵�, ������
	
	int m_nMaxLinkMapCount;
	int m_nFindKey;

	//�ؽ��� ���ϸ��̼�
	vector<_XTextureAnimation*> vtTextureAni; //�ؽ��� ���ϸ��̼� ����

	POINT m_nPos;
	POINT m_nFirstPos;

	BOOL m_bAlphaProcess;
};


//inline-------------------------------------------------------------------------------------
inline BOOL XWindow_WorldMap::SetShowWindowProcess( BOOL bShow )
{
	m_bAlphaProcess = bShow;
	if( m_bAlphaProcess )
	{
		m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( 0, 255, 255, 255 );
		ShowWindow(TRUE);
		
#ifdef _ACCLAIM_RUBICONADSYSTEM	
		g_ViewRubiconAD = FALSE;
		::ShowWindow(g_hRubiconADWindow, SW_HIDE);
#endif
	}
	else
	{
		m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
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
	}

	return m_bAlphaProcess;
}

inline void XWindow_WorldMap::SetDrawPostion( int nX, int nY )
{
	m_nPos.x = nX;
	m_nPos.y = nY;
}

inline void XWindow_WorldMap::FirstPostion( int nX, int nY )
{
	m_nFirstPos.x = nX;
	m_nFirstPos.y = nY;
}

//���� 333
//�Ҹ��� 335
//���� 337
//�츲 336
//��� 334
//���� 338
//�պ� 2166
//�ϰ� 3486
//���� 3491
//������ 3492
//���� 3493
//�뵿 3494
//��â 3495
//���� 3496
//�䵿 3002
//���غ��� 3418
//���޶��� 3488
//������	3224
//�пյ� 3225

#endif // !defined(AFX_XWINDOW_WORLDMAP_H__D1D4838F_B55B_4819_82B0_B4D272379C5E__INCLUDED_)

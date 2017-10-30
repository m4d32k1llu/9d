// _XArea.h: interface for the _XArea class.
//
// Created Date : 2004.01.28 
// Author : Oneway Yang (oneway48)
// Desc : ������ ���� Format�� ��ȭ�� ���ؼ� ��ȭ�� Format�� �����ϴ� Ŭ������ ����   
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX__XAREA_H__42F1B703_0C0C_40E0_8DC8_7A99C4CCE1A7__INCLUDED_)
#define AFX__XAREA_H__42F1B703_0C0C_40E0_8DC8_7A99C4CCE1A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>


#define _XDEF_AREACATEGORY_COUNT			12

#define _XDEF_AREACATEGORY_SAFE				0
#define _XDEF_AREACATEGORY_PLAYERSPAWN		1
#define _XDEF_AREACATEGORY_MOBSPAWN			2
#define _XDEF_AREACATEGORY_NPCAREA			3
#define _XDEF_AREACATEGORY_QUESTAREA		4
#define _XDEF_AREACATEGORY_EVENTAREA		5
#define _XDEF_AREACATEGORY_MAPWARP			6
#define _XDEF_AREACATEGORY_PVPAREA			7
#define _XDEF_AREACATEGORY_CASTLE			8
#define _XDEF_AREACATEGORY_LOGOUT			9
#define _XDEF_AREACATEGORY_NOTUSESPEEDSKILL	10
#define _XDEF_AREACATEGORY_PUBLICPVPAREA	11	//Author : ����� //breif : �߰�

#define _XDEF_AREASHAPETYPE_BOX				0
#define _XDEF_AREASHAPETYPE_SPHERE			1

typedef struct
{
	FLOAT	x;
	FLOAT	z;
}_XAREAPOINTSTRUCT;

class _XArea  
{
public :
	BYTE				m_AreaCategory;
	int					m_Num;

#ifdef _XDEF_USENEWAREATEXTSYSTEM
	LPTSTR				m_Name;

#ifdef _XJAPANESE	
	LPTSTR				m_HiraganaName;
#endif	

#else
	TCHAR				m_Name[21];
#endif
	
	_XAREAPOINTSTRUCT	m_Point[4];
	_XAREAPOINTSTRUCT	m_CenterPoint;
	FLOAT				m_Radius;
	
	BYTE				m_ShapeType;
	
	int					m_TargetZone;
	int					m_TargetWarpzone;
	
	int					m_MonsterSpawnType;
	int					m_MonsterSpawnCount;

	BYTE				m_PVPZoneType;	//0:�ʵ���� 1:���ĺ����(����/����-�ܽ�,����) 2:���ĺ����(����-����,���)
										//3:�񹫴�ȸ�� ���� 4:�񹫴�ȸ�� 5:�񹫾������� 6:�񹫿Ϸ�����
	int					m_indexPVPSafeZone[4];
	int					m_indexPVPCompleteZone;

	int					m_SpecialFunction;
	
	// ���߸� �̵��� �� �ʿ��� ���� 
	int					m_MultiTargetZone1;
	int					m_MultiTargetWarpzone1;
	int					m_MultiWarpzonePassage1; // ��� ������ ��Ÿ����.
	
#ifdef _XDEF_USENEWAREATEXTSYSTEM
	LPTSTR				m_WarpZoneName1;
#else
	TCHAR				m_WarpZoneName1[21];
#endif
	
	int					m_MultiTargetZone2;
	int					m_MultiTargetWarpzone2;
	int					m_MultiWarpzonePassage2; // ��� ������ ��Ÿ����.
#ifdef _XDEF_USENEWAREATEXTSYSTEM
	LPTSTR				m_WarpZoneName2;
#else
	TCHAR				m_WarpZoneName2[21];
#endif
	
	int					m_MultiTargetZone3;
	int					m_MultiTargetWarpzone3;
	int					m_MultiWarpzonePassage3; // ��� ������ ��Ÿ����.
#ifdef _XDEF_USENEWAREATEXTSYSTEM
	LPTSTR				m_WarpZoneName3;
#else
	TCHAR				m_WarpZoneName3[21];
#endif
	
public:
	_XArea();
	virtual ~_XArea();
	
	BOOL				Load(FILE* fileptr);
};

#endif // !defined(AFX__XAREA_H__42F1B703_0C0C_40E0_8DC8_7A99C4CCE1A7__INCLUDED_)

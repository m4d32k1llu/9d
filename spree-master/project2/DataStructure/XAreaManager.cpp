// _XAreaManager.cpp: implementation of the _XAreaManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "emperorofdragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XAreaManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XAreaManager::_XAreaManager()
{
	//m_AreaTable = NULL;
	m_AreaTable = new _XAREALISTTABLE[_XDEF_AREACATEGORY_COUNT];
	
	m_PotalOBBCount		= 0;
	m_PortalOBBDataList = NULL;
}

_XAreaManager::~_XAreaManager()
{
	Release();
	SAFE_DELETE_ARRAY( m_AreaTable );
}


void _XAreaManager::Release()
{	
	if( m_AreaTable )
	{
		_XArea*		currentarea = NULL;

		for(int i = 0 ; i < _XDEF_AREACATEGORY_COUNT ; ++i)
		{
			list <_XArea *>::iterator iter;
			for(iter = m_AreaTable[i].AreaList.begin() ; iter != m_AreaTable[i].AreaList.end() ; ++iter)
			{
				currentarea = *iter;
				if(currentarea)
				{
					delete currentarea;
				}
			}
			
			m_AreaTable[i].AreaList.clear();
		}
	}

	m_PotalOBBCount		= 0;
	SAFE_DELETE_ARRAY( m_PortalOBBDataList );
}


BOOL _XAreaManager::LoadAreaList(FILE *fileptr)
{
	g_CurrentCachedMobcount = 0;
	for( int i = 0; i < _XDEF_MODELCLASSCOUNT_MOB; ++i )
	{
		g_MobAttribTable[i].usecurrentzone = false;
	}
	
	if(!fileptr)
	{
		_XFatalError("Load Area list : Invalid file pointer" );
		return FALSE;
	}
	
	int		areacount = 0;
	if(fread(&areacount, sizeof(int), 1, fileptr) < 1)
	{
		_XFatalError("Load Area list : Read area count" );
		return FALSE;
	}
	
	if(areacount < 0)
		return FALSE;
	
	_XArea* currentarea = NULL;


	for(i = 0 ; i < areacount ; ++i)
	{
		currentarea = new _XArea;
		if(!currentarea->Load(fileptr))
		{
			_XFatalError("Load Area list : Read region information");
			return FALSE;
		}
		
		// ī�װ����� ���������� ������ List Table�� �����Ѵ�.
		BuildTable(currentarea);
	}

	SAFE_DELETE_ARRAY( m_PortalOBBDataList );
	m_PotalOBBCount = m_AreaTable[_XDEF_AREACATEGORY_MAPWARP].AreaList.size();

	if( m_PotalOBBCount > 0 )
	{
		m_PortalOBBDataList = new _XOBBData[ m_PotalOBBCount ];

		int index = 0;
		_XArea* currentarea = NULL;

		list <_XArea *>::iterator iter;			
		for(iter = m_AreaTable[_XDEF_AREACATEGORY_MAPWARP].AreaList.begin() ; iter != m_AreaTable[_XDEF_AREACATEGORY_MAPWARP].AreaList.end() ; ++iter, ++index)
		{
			currentarea = *iter;

			D3DXVECTOR3 point[8];
			_XOBBData*	OBBData = NULL;
			D3DXVECTOR3 centerpoint;
			centerpoint.x = currentarea->m_CenterPoint.x;
			centerpoint.z = currentarea->m_CenterPoint.z;
			/*
			for(int i = 0 ; i < 4 ; i++)
			{
				point[i].x = currentarea->m_Point[i].x;				
				point[i].z = currentarea->m_Point[i].z;
				point[i].y = g_LodTerrain.GetTerrainHeight(point[i]);
					
			}

			for(i = 4 ; i < 8 ; i++)
			{
				point[i].x = point[i-4].x;				
				point[i].z = point[i-4].z;
				point[i].y = g_LodTerrain.GetTerrainAndOBBHeight(point[i], OBBData) + 2.5f;
			}
			*/

			point[0].x = point[1].x = currentarea->m_Point[0].x;
			point[0].z = point[1].z = currentarea->m_Point[0].z;			
			
			point[2].x = point[3].x = currentarea->m_Point[1].x;
			point[2].z = point[3].z = currentarea->m_Point[1].z;
			
			point[4].x = point[5].x = currentarea->m_Point[2].x;
			point[4].z = point[5].z = currentarea->m_Point[2].z;
			
			point[6].x = point[7].x = currentarea->m_Point[3].x;
			point[6].z = point[7].z = currentarea->m_Point[3].z;

			point[0].y = point[2].y = point[4].y = point[6].y = g_LodTerrain.GetTerrainHeight(centerpoint);
			point[1].y = point[3].y = point[5].y = point[7].y = g_LodTerrain.GetTerrainAndOBBHeight(centerpoint, OBBData) + 2.5f;
			
			_XCreateOBBFromVertexList( point, 8, &m_PortalOBBDataList[index] );

#ifdef _XDWDEBUG
			m_PortalOBBDataList[index].m_OBBModel.InitBox( m_PortalOBBDataList[index].Vertices(),
														   m_PortalOBBDataList[index].Center() );
#endif
		}
	}
	
	for( i = 0; i < _XDEF_MODELCLASSCOUNT_MOB; ++i )
	{
		if( g_MobAttribTable[i].usecurrentzone )
		{
			g_CurrentCachedMobcount++;
		}
	}

	return TRUE;
}

void _XAreaManager::BuildTable(_XArea* currentarea)
{
	switch(currentarea->m_AreaCategory)
	{
	case _XDEF_AREACATEGORY_SAFE :
		m_AreaTable[_XDEF_AREACATEGORY_SAFE].AreaList.push_back(currentarea);
		break;
	case _XDEF_AREACATEGORY_PLAYERSPAWN :
		m_AreaTable[_XDEF_AREACATEGORY_PLAYERSPAWN].AreaList.push_back(currentarea);
		break;
	case _XDEF_AREACATEGORY_MOBSPAWN :
		m_AreaTable[_XDEF_AREACATEGORY_MOBSPAWN].AreaList.push_back(currentarea);
		break;
	case _XDEF_AREACATEGORY_NPCAREA :
		m_AreaTable[_XDEF_AREACATEGORY_NPCAREA].AreaList.push_back(currentarea);
		break;
	case _XDEF_AREACATEGORY_QUESTAREA :
		m_AreaTable[_XDEF_AREACATEGORY_QUESTAREA].AreaList.push_back(currentarea);
		break;
	case _XDEF_AREACATEGORY_EVENTAREA :
		m_AreaTable[_XDEF_AREACATEGORY_EVENTAREA].AreaList.push_back(currentarea);
		break;
	case _XDEF_AREACATEGORY_MAPWARP :
		m_AreaTable[_XDEF_AREACATEGORY_MAPWARP].AreaList.push_back(currentarea);
		break;
	case _XDEF_AREACATEGORY_PVPAREA :
		m_AreaTable[_XDEF_AREACATEGORY_PVPAREA].AreaList.push_back(currentarea);
		break;
	case _XDEF_AREACATEGORY_CASTLE :
		m_AreaTable[_XDEF_AREACATEGORY_CASTLE].AreaList.push_back(currentarea);
		break;
	case _XDEF_AREACATEGORY_LOGOUT :
		m_AreaTable[_XDEF_AREACATEGORY_LOGOUT].AreaList.push_back(currentarea);
		break;
	case _XDEF_AREACATEGORY_NOTUSESPEEDSKILL :
		m_AreaTable[_XDEF_AREACATEGORY_NOTUSESPEEDSKILL].AreaList.push_back(currentarea);
		break;
	case _XDEF_AREACATEGORY_PUBLICPVPAREA : //Author : ����� �߰�
		m_AreaTable[_XDEF_AREACATEGORY_PUBLICPVPAREA].AreaList.push_back(currentarea);
		break;
	default :
		break;
	}
}


BOOL _XAreaManager::FindPositionInArea(D3DXVECTOR3 position, int areanumber, int areacategory, BOOL questareacheck )
{

	_XArea* currentarea = NULL;
	list <_XArea *>::iterator iter;

	for(iter = m_AreaTable[areacategory].AreaList.begin() ; iter != m_AreaTable[areacategory].AreaList.end() ; ++iter)
	{
		currentarea = *iter;

		if(currentarea->m_Num == areanumber)
		{
			if( CheckInArea(position, currentarea, questareacheck) )
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	return FALSE;

}

_XArea* _XAreaManager::GetArea(int areacategory, int areaindex)
{
	_XArea* currentarea = NULL;
	list <_XArea *>::iterator iter;
	
	int index = 0;
	for(iter = m_AreaTable[areacategory].AreaList.begin() ; iter != m_AreaTable[areacategory].AreaList.end() ; ++iter, ++index)
	{
		
		currentarea = *iter;
		if( currentarea->m_Num == areaindex )
		{
			return *iter;
		}
	}
	return NULL;
}

TCHAR* _XAreaManager::GetAreaName(int areacategory, int areaindex)
{
	_XArea* currentarea = NULL;
	list <_XArea *>::iterator iter;
	
	int index = 0;
	for(iter = m_AreaTable[areacategory].AreaList.begin() ; iter != m_AreaTable[areacategory].AreaList.end() ; ++iter, ++index)
	{
		
		currentarea = *iter;
		if( currentarea->m_Num == areaindex )
		{
			return currentarea->m_Name;
		}
	}
	return NULL;
}

BOOL _XAreaManager::FindAreaCenterPosition(D3DXVECTOR3& position, int areanumber, int areacategory)
{	
	_XArea* currentarea = NULL;
	list <_XArea *>::iterator iter;
	
	for(iter = m_AreaTable[areacategory].AreaList.begin() ; iter != m_AreaTable[areacategory].AreaList.end() ; ++iter)
	{
		currentarea = *iter;
		if(currentarea->m_Num == areanumber)
		{
			D3DXVECTOR3 point[4];
			for(int i = 0 ; i < 4 ; ++i)
			{
				point[i].x = currentarea->m_Point[i].x;
				point[i].y = 0.0f;
				point[i].z = currentarea->m_Point[i].z;
			}
			
			D3DXVECTOR3 diagonal = point[2] - point[0];
			FLOAT length = D3DXVec3Length(&diagonal);
			D3DXVECTOR3 unit;
			D3DXVec3Normalize(&unit, &diagonal);
			
			position = point[0] + ((length/2)*unit); 

			_XOBBData* OBBData = NULL;
			position.y = g_LodTerrain.GetTerrainAndOBBHeight(position, OBBData);
			if(position.y >= -9999.9f && position.y <= -9998.0f)
			{
				position.y = g_LodTerrain.GetTerrainHeight(position);
			}
			return TRUE;
		}
		
	}

	return FALSE;
}

BOOL _XAreaManager::FindAreaNumber(D3DXVECTOR3 position, list <int>& arealist)
{
	int		category = 0;

	_XArea* currentarea = NULL;
	list <_XArea *>::iterator iter;

	for(category=0 ; category<_XDEF_AREACATEGORY_COUNT ; ++category)
	{
		for(iter = m_AreaTable[category].AreaList.begin() ; iter != m_AreaTable[category].AreaList.end() ; ++iter)
		{
			currentarea = *iter;
				
			if( CheckInArea(position, currentarea) )
				arealist.push_back(currentarea->m_Num);
		
		}		
	}
	
	if(!arealist.empty())
		return TRUE;
	else
		return FALSE;
}

BOOL _XAreaManager::CheckInsideArea(D3DXVECTOR3 position, int category, int& areanumber )
{
	_XArea* currentarea = NULL;
	list <_XArea *>::iterator iter;

	if(category < 0 || category >= _XDEF_AREACATEGORY_COUNT ) return FALSE;
	{
		for(iter = m_AreaTable[category].AreaList.begin() ; iter != m_AreaTable[category].AreaList.end() ; ++iter)
		{
			currentarea = *iter;
			
			if( CheckInArea(position, currentarea) )
			{
				areanumber = currentarea->m_Num;
				return TRUE;
			}
		}		
	}
	
	return FALSE;
}

BOOL _XAreaManager::IsEmpty(D3DXVECTOR3 position)
{
	int		category = 0;
	
	_XArea* currentarea = NULL;
	list <_XArea *>::iterator iter;
	
	for(category=0 ; category<_XDEF_AREACATEGORY_COUNT ; ++category)
	{
		for(iter = m_AreaTable[category].AreaList.begin() ; iter != m_AreaTable[category].AreaList.end() ; ++iter)
		{
			currentarea = *iter;
			
			if( CheckInArea(position, currentarea) )
				return FALSE;
		}
	}		
	
	return TRUE;
}

BOOL _XAreaManager::CheckInArea(D3DXVECTOR3 position, _XArea* currentarea, BOOL questareacheck)
{
	D3DXVECTOR3 v0,v1,v2,v3,//������ ��Ÿ���� v0,v1,v2,v3�� 4���� 
		vNormal,			//������ ���ƺк��� ���� NormalVector�� vNormal
		vPoint,				//����� ��ġ�� ��Ÿ���� vPoint
		vUp;				//NormalVector�� ���ϱ� ���� UpVector
	
	D3DXVECTOR3 vLineHalf,	//���� �Ѻ��� ������ ��Ÿ���� ����
		vTemp;				//���� �Ѻ��� �������� �ٶ󺸴� ����� ��ġ 
	
	vUp = D3DXVECTOR3(0,1,0);
	
	if( questareacheck ) // ����Ʈ ���� üũ �϶� ���߿� �ʿ��ϸ� ó��
	{
/*
		D3DXMATRIX matTrans;	// �������� �̵��� �Ÿ��� �����ϱ� ���� ����		
		D3DXMATRIX matRotY;		// NPC ������ �ٶ󺸴� ������ �����ϱ� ���� ���� 
		D3DXMATRIX matResult;
		D3DXMATRIX matTargetPos;// ī�޶� �ٶ󺸴� ����
		
		D3DXMatrixIdentity(&matTrans);
		D3DXMatrixIdentity(&matRotY);
		D3DXMatrixIdentity(&matResult);
		D3DXMatrixIdentity(&matTargetPos);
		
		
		D3DXMatrixTranslation( &matTrans, m_NPCWideViewMove, 0.0f, 0.0f );
		D3DXMatrixRotationY( &matRotY, rad_fYaw );
		
		D3DXMatrixMultiply( &matResult, &matTrans, &matRotY );
		
		D3DXMatrixTranslation( &matTrans, g_LastNPCScriptOwner->m_matWorldPosition._41
			, g_LastNPCScriptOwner->m_matWorldPosition._42
			, g_LastNPCScriptOwner->m_matWorldPosition._43 );
		
		D3DXMatrixMultiply( &matTargetPos, &matTargetPos, &matResult );// �������� ��� (�̵�, ȸ��)
		D3DXMatrixMultiply( &matTargetPos, &matTargetPos, &matTrans );// �������� ��� �Ŀ� NPC ��ġ�� �̵� ��ȯ
		
		position.x = matTargetPos._41;	
		position.y = matTargetPos._42 + g_LastNPCScriptOwner->m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1]/2 *3 ;
		position.z = matTargetPos._43;	
*/
		
	}
	else
	{
		v0 = D3DXVECTOR3(currentarea->m_Point[0].x,0,currentarea->m_Point[0].z);
		v1 = D3DXVECTOR3(currentarea->m_Point[1].x,0,currentarea->m_Point[1].z);
		v2 = D3DXVECTOR3(currentarea->m_Point[2].x,0,currentarea->m_Point[2].z);
		v3 = D3DXVECTOR3(currentarea->m_Point[3].x,0,currentarea->m_Point[3].z);
	}
	

	vPoint = position;
	vPoint.y = 0.0f;
	
	// ���� �Ѻ��� ���ؼ� ������ üũ�Ͽ� �ٱ����̸� �������� ����� �����Ѵ�.
	// �����̸� �� �������� ���ؼ� ������ üũ�Ѵ�. 
	//  -> �̷� ������� ������ ��纯�� �˻��Ѵ�.
	// 
	// * �ȹ��� üũ�ϴ� ��� 
	// üũ�� ������ �Ǵ� Line�� ����Vector�� ���ϰ�
	// Line�� �������� �ٶ󺸴� ����� ��ġVector�� ���� ����,
	// ����Vector�� ��ġVector�� ������ ���� ���, ���� ������
	// Line�� �ȹ��� �����Ѵ�.
	//
	// �� Line�� �������� ����� �ٶ󺸴� ��ġVector�� �Ź� ���ϴ� ������
	// ������ �߽����� ����� �ٶ󺸴� vPoint�� ����� ��쿡
	// vPoint�� ��ġ�� 1,2,3,4��и麰�� 
	// ����üũ�� ���س������� ��ȣ�� �������� �����Ƿ�
	// ���� ����� ���� ������ �ϹǷ� ȿ�������� ���ϱ� �����̴�.
	// üũ�ϴ� ���ؿ� �°� �������ְ� ���ϸ� ������ ������ ������ �Ǻ��� �� �ִ�.
	
	D3DXVec3Cross(&vNormal,&(v0-v1),&vUp);	//����Vector�� �Ѳ����� ���α��ϴ� ���� �ս��̴�.	
	D3DXVec3Normalize(&vNormal,&vNormal);	
	vLineHalf = D3DXVECTOR3((v0.x+v1.x)/2,0,(v0.z+v1.z)/2);
	vTemp = vPoint - vLineHalf;	
	
	
	if(D3DXVec3Dot(&vNormal,&vTemp)>0)
	{
		D3DXVec3Cross(&vNormal,&(v1-v2),&vUp);
		D3DXVec3Normalize(&vNormal,&vNormal);
		vLineHalf = D3DXVECTOR3((v1.x+v2.x)/2,0,(v1.z+v2.z)/2);
		vTemp = vPoint - vLineHalf;
		
		if(D3DXVec3Dot(&vNormal,&vTemp)>0)
		{
			D3DXVec3Cross(&vNormal,&(v2-v3),&vUp);
			D3DXVec3Normalize(&vNormal,&vNormal);
			vLineHalf = D3DXVECTOR3((v2.x+v3.x)/2,0,(v2.z+v3.z)/2);
			vTemp = vPoint - vLineHalf;
			
			if(D3DXVec3Dot(&vNormal,&vTemp)>0)
			{
				D3DXVec3Cross(&vNormal,&(v3-v0),&vUp);
				D3DXVec3Normalize(&vNormal,&vNormal);
				vLineHalf = D3DXVECTOR3((v3.x+v0.x)/2,0,(v3.z+v0.z)/2);
				vTemp = vPoint - vLineHalf;
				
				if(D3DXVec3Dot(&vNormal,&vTemp)>0) 
				{
					return TRUE;
				}
			}
		}
	} 

	return FALSE;
}

BOOL _XAreaManager::GetMyAreaCenterPosition(D3DXVECTOR3 myposition, D3DXVECTOR3& centerposition)
{
	int		category = 0;
	
	_XArea* currentarea = NULL;
	list <_XArea *>::iterator iter;
	
	for(category=0 ; category<_XDEF_AREACATEGORY_COUNT ; ++category)
	{
		for(iter = m_AreaTable[category].AreaList.begin() ; iter != m_AreaTable[category].AreaList.end() ; ++iter)
		{
			currentarea = *iter;
			
			if( CheckInArea(myposition, currentarea) )
			{
#ifdef _XDWDEBUG
				_XDWINPRINT("category : %d  areanum : %d", category, currentarea->m_Num);
#else
				return FindAreaCenterPosition(centerposition, currentarea->m_Num, currentarea->m_AreaCategory);
#endif
			}
		}
	}

	return FALSE;
}
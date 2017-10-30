#ifndef __SUCOLLIDE_H__
#define __SUCOLLIDE_H__

#include "d3d9types.h"
#include "d3dx9math.h"


struct SUVERTEX
{
    FLOAT	x, y, z, rhw;
    DWORD	color;
	FLOAT   tu, tv; // Vertex texture coordinates
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct PLANE2
{
 	float		Distance;
	D3DXVECTOR3	Normal;
};

struct SuVIEWVOLUME
{
	PLANE2 Plane[6];
};

struct SUPOLIGON
{
	D3DXVECTOR3 p[4];
};

struct SUSQUARE
{
	D3DXVECTOR3 p[4];
};

struct SUINTERSECTION
{
    int			dwFace;                 // mesh face that was intersected
    FLOAT		fBary1, fBary2;         // barycentric coords of intersection
    FLOAT		fDist;                  // distance from ray origin to intersection
    FLOAT		tu, tv;                 // texture coords of intersection
	SUPOLIGON	poly;
};

//===============================================================================================================
// ĳ���͸� ���콺 ��ŷ�� ���ؼ� ������ �ٿ�� ���ڰ� �Ǵ�⸦ ���� ����ü
// ���� : ����ü ���� �ʿ��ϴ�. �� ��ȿ������ TT
//===============================================================================================================
struct BOUNDINGCROSS  
{
	SUPOLIGON	FrontPoly[2] ;			// ���ڰ� �Ǵ���� XY�� ���� �� ������ �Ǵ���� ������4��
	SUPOLIGON	SidePoly[2] ;			// ���ڰ� �Ǵ���� ZY�� �� ������ �Ǵ���� ������4��
	WORD		wIndex;					// ���� �ڱ�ĳ���� �ε���
	float		fIntersectedLength;		// �信�� ��ŷ�� �Ÿ�
	bool		IsPicked;
};
//----------ī�޶� ���õ�  ������ ��
#define CAMERSTOP					1		//ī�޶� ����
#define CAMERAFORWARD				2		//ī�޶� ����
#define CAMERABACKWARD				3		//ī�޶� ����	

//------------------------------------

//----------------------------------------------------------------------------------------------------
//----------���� ROAM�� ���� ������ ��

// Our custom FVF, which describes our custom vertex structure
// ���ؽ� ���ۿ��� FVF�� ����ϱ� ���ؼ� �Ʒ��� �÷��� ������ ����Ѵ�.
// D3DFVF_XYZ : ������ ��ȯ(transformation)���� ������ 
// Direct3D�� ������ ����� ���������� ������ ������ ����, ��, ��������� ��� �Ͽ� ��ȯ �Ѵ�.

//#define MAXSIZE							1024		// ���� ���簢���̹Ƿ� 
#define FRAMEVARIANCE					50			// ������ ��ȭ��
#define MOVEOFFSET_PATCHCENTERPOINT		32			// ���� �̵��� offset
// Desired number of Binary Triangle tessellations per frame.
// This is not the desired number of triangles rendered!
// There are usually twice as many Binary Triangle structures as there are rendered triangles.
#define DESIREDTRIS		10000;

// Observer and Follower modes
#define FOLLOW_MODE		(0)
#define OBSERVE_MODE	(1)
#define DRIVE_MODE		(2)
#define FLY_MODE		(3)

//----------------------------------------------------------------------------------------------------

void ExtractFrustumPlanes(SuVIEWVOLUME* pViewVolume, LPDIRECT3DDEVICE9	pd3dDevice);//, PLANE2 *Planes )

bool IsPointInViewVolume( SuVIEWVOLUME* pViewVolume, D3DXVECTOR3* pvPoint);

BOOL IntersectRayTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
							  D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
							  SUINTERSECTION* pintersectionInfo);

BOOL CharicPick( POINT ptCursor, BOUNDINGCROSS* pBoundingCross, LPDIRECT3DDEVICE9 pd3dDevice, 
				 D3DXMATRIX* p_Viewmat, D3DXMATRIX*	pProjMat, D3DVIEWPORT9*	pvp, SUINTERSECTION* pIntersectionInfo );
BOOL RayAndSphereIntersect( D3DXVECTOR3& vO, D3DXVECTOR3& vD, D3DXVECTOR3& vC, float fr); //���� ������ ��������
BOOL RayAndSphereIntersectFindPoint( D3DXVECTOR3& vO, D3DXVECTOR3& vD, D3DXVECTOR3& vC, float fr, float& ft); //���� ������ ����÷�� ���Ѵ�.
bool MgcTestIntersection (const D3DXVECTOR3& rkRay, const D3DXVECTOR3& vC, float fR);
FLOAT _XSphereIntersectRay( D3DXVECTOR3 &p_RayO, D3DXVECTOR3 &p_RayV, D3DXVECTOR3 &p_SO, FLOAT p_dSR );

int SercheClosestNo(SUINTERSECTION* interInfo, int n);
D3DXVECTOR3* FindYValueOfm_pVertex( D3DXVECTOR3* pv0 ,SUPOLIGON* pPlan);
int SercheClosestIntersectionInfo(SUINTERSECTION* interInfo, int n);
BOUNDINGCROSS* CreateAndUpdateBoundCross( BOUNDINGCROSS* pBoundingCrss, float fcenterXPo, float fcenterYPo, float fcenterZPo, 
					  int nXOffset, int nYOffset , int nZOffset );

#endif	// __SUCOLLIDE_H__

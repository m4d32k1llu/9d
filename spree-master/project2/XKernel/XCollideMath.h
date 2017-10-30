#ifndef SUMATH_H
#define SUMATH_H

#include <d3dx9.h>
#include <d3d9types.h>
#include <d3dx9math.h>

struct PLANEQUATION
{
 	D3DXVECTOR3 vNomal; //Top left
	float		fD;		//Top right
};

HRESULT SuMath_VectorMatrixMultiply( D3DXVECTOR3* vDest, D3DXVECTOR3* vSrc,
									D3DMATRIX* mat);
VOID	MakeIdentiVec( D3DXVECTOR3* psuVector );
VOID	FindThePointOnTheLine(D3DXVECTOR3* pOutput, D3DXVECTOR3* pvO, D3DXVECTOR3* pvD, float t);
VOID	Vectorsub(D3DXVECTOR3* pOutput, D3DXVECTOR3* pvO, D3DXVECTOR3* pv1);
int		LineEquation(D3DXVECTOR3* vP0, D3DXVECTOR3* vP1, D3DXVECTOR3* vP2 );
VOID	PlaneEquation( PLANEQUATION* pPlaneEq, D3DXVECTOR3* vP0, D3DXVECTOR3* vP1, D3DXVECTOR3* vP2);
float	GetRotateRadian( LPDIRECT3DDEVICE9 pd3dDevice, POINT ptCursor );
float	GetRotateRadianFromTwoPoint( LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3* pvbasis, D3DXVECTOR3* pvDir,
									D3DXMATRIX*	pViewMat )	;
float	GetRotateRadian( LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3* pvbasis, POINT ptCursor, D3DXMATRIX*	pViewMat, D3DXMATRIX*	pProjMat, D3DVIEWPORT9*	pvp );
VOID	FindePoOnCircle(D3DXVECTOR3& vrOrig, D3DXVECTOR3& vCurrentPo, float fRadian, float fR, int nID);
float   GetPitchRadian( D3DXVECTOR3& vrOrig, D3DXVECTOR3& vCurrendtPo);
float	GetRadian( D3DXVECTOR3& vrOrig , D3DXVECTOR3& vCurrendtPo, int nID);

VOID	FindePoOnPitchRotatedCircle(D3DXVECTOR3& vrOrig, D3DXVECTOR3& vCurrendtPo, float fYawRaian, float fPich, float fR);
float	SuVec3Length( D3DXVECTOR3& v1, D3DXVECTOR3& v2 );
D3DXVECTOR3 GetAxisVector( D3DXVECTOR3& v1, D3DXVECTOR3& vTarget, D3DXVECTOR3& vAxis ); //�� ���͸� ���Ѵ�.
float	GetAngleUseingDot( D3DXVECTOR3* pvA, D3DXVECTOR3* pvB); // �� ������ ������ �̿��� �� ���Ͱ� �̷�� ���� ���Ѵ�.
VOID	InitRotateMat( float fDegree, D3DXMATRIX&	matRotate);	// Y���� ������Ų ȸ���� ��Ʈ������ �����.
VOID	GetRotateMat( D3DMATRIX& matRoate, D3DXVECTOR3& Axis, float fRadian); //�� Axis�� �������� fRadian ��ŭ ȸ���� ��Ʈ������ ���Ѵ�.
VOID	GetRightAxis( D3DXVECTOR3& vRight, const D3DXVECTOR3& vDirection); //���� ������ vDirection���� vRight���͸� ���Ѵ�.
VOID	GetRight_Up_Axis( D3DXVECTOR3& vRight, D3DXVECTOR3& vUp, const D3DXVECTOR3& vDirection );
//������ �𸥴� ���ؽ��� ��ǥ���� ���� ������.. obb �ڽ��� ���� ����. 3���� ���� ���Ҽ� �ִ�..
//��� ���� ������ ��.. 3D �� ����å ���� �����..
void	SuGaussPointsFit ( int iQuantity, const D3DXVECTOR3* akPoint, D3DXVECTOR3& rkCenter, D3DXVECTOR3 akAxis[3], float afExtent[3] ); 

#endif
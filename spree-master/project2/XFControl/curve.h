/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright � Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc. 

  This file contains routines for working with quadratic bezier curve segments
***************************************************************************/
#ifndef CURVE_INCLUDED
#define CURVE_INCLUDED

#ifndef GEOM_INCLUDED
#include "geom.h"
#endif

typedef struct {
	SPOINT anchor1;
	SPOINT control;
	SPOINT anchor2;
	int    isLine;
} CURVE, *P_CURVE;

void CurveSet(P_SPOINT anchor1, P_SPOINT control, P_SPOINT anchor2, P_CURVE dst);
// Set the curve to a curve segment

void CurveSetLine(P_SPOINT pt1, P_SPOINT pt2, P_CURVE dst);
// Set the curve to represent a line segment

void CurveBounds(P_CURVE c, P_SRECT bounds);
// Calculate a bounds rectangle for the curve

void CurveTightBounds(P_CURVE c, P_SRECT bounds);
// Calculate a smaller bounds rectangle for the curve - is slower

void CurveExtrema(P_CURVE c, P_SCOORD x, P_SCOORD y);
// calculates the maxima or minima of the x and y coordinates

void CurveShift(P_CURVE src, S16 shift, P_CURVE dst);
// Copy src to dst with a shift factor, a positive shift multiplies by 2^n,
//	negative divides by 2^n.

BOOL CurveEqual(P_CURVE c1, P_CURVE c2, SCOORD err);
// Returns true if the curves are equal within 'err', point order is important

BOOL CurveSimilar(P_CURVE c1, P_CURVE c2, SCOORD err);
// Returns true if the curves are equal, point order is not important

#define CurveIsLine(c) (c->isLine)
// Returns true if the curve represents a line segment

BOOL CurveTestLine(P_CURVE c, SCOORD err);
// Returns true if the curve represents a line segment within the given error

SCOORD CurveFlatness(P_CURVE c);
// Returns the error generated by approximating this curve with a line segment

void CurveDivide(P_CURVE c, P_CURVE dst1, P_CURVE dst2);	// u == fixedHalf
void CurveDivide(P_CURVE c, SFIXED u, P_CURVE dst1, P_CURVE dst2);
// Split the curve segment at the parameter value 'u'

void CurveDivideTwo(P_CURVE c, SFIXED u1, SFIXED u2, P_CURVE dst1, P_CURVE dst2, P_CURVE dst3);
// Spit the curve segment at two points

void CurveCalcPoint(P_CURVE c, SFIXED u, P_SPOINT dst);
// Calculate the point on the curve with parameter value 'u'

SFIXED CurveTangentAngle(P_CURVE c, SFIXED u);
// Find the tangent angle at parameter 'u' along the curve
// The angle always points in the forward direction along the curve

SCOORD CurveLength(P_CURVE c);
// Find the total arc length of the curve segment

SFIXED CurveFindLength(P_CURVE c, SCOORD len);
// Find the parameter value that is at the specified arc length along the
//	curve segment

SFIXED CurveNearestPoint(P_CURVE c, P_SPOINT pt, SCOORD width, P_SPOINT dst);
// Find the nearest point to 'pt' on the curve
// Returns -1 if pt is not within width distance of the curve
//	other with the parameter value is returned
// The actual nearest point is also returned of 'dst' is not nil

SCOORD CurveNearestApproach(P_CURVE a, P_CURVE b, SCOORD err, SCOORD flatness, P_SFIXED uA, P_SFIXED uB);
// Finds the point on each curve where they are closest. If the curves are more
//	that err apart simply returns maxS32. Flatness controls the precision of the approximation.
//	returns the distance.

void CurveTransform(P_CURVE c, MATRIX* m, P_CURVE dst);
// Transforms the points of the curve segment with the matrix

void CurveOffset(P_CURVE c, SCOORD dist, P_CURVE dst);
// Generate a curve that is offset from the curve by the specified distance
// For best results, it should not be used with curves that have extreme curvature

void CurveReverse(P_CURVE c, P_CURVE dst);
// Reverse the order of points on the curve segment

BOOL CurveIsExtreme(P_CURVE c);
// Return true if curve has an extreme curvature.

void CurveSimplify(P_CURVE c);
// If the curve segment is approximately a line, make it a line

SCOORD CurveFitPoints(P_SPOINT pts, U16 n, P_CURVE dst, P_SPOINT prevControl);
// Make a curve segment that approximates the points specified in 'pts'
// Returns the error resulting from the approximation
// prevControl is a way of optionally providing a smoothness constraint
//	it prevControl is non nil then the curve fit will ensure 2nd order continuity
//	(e.g. tangents equal) on the anchor1 of the dst curve

BOOL CurveMap2To1(P_CURVE a, P_CURVE b, SCOORD maxErr, P_CURVE dst);
// Finds an curve (dst) that approximates a and b within maxErr tolerance
// Return true a valid curve was found and the error is < maxErr

U16 CurveCoincident(P_CURVE a, P_CURVE b, SCOORD err);
// Returns values below if the two curves are coincident within the given error tolerence.
#define curveNotCoincident 		0
#define curveCoincidentSame 	1
#define curveCoincidentReverse 	2

void CurveAdjust(P_CURVE src, P_SPOINT pt1, P_SPOINT pt2, P_CURVE dst);
// Setup dst curve so that it has the same endpoint tangencies as src but has
//	anchor points pt1 and pt2, this is used to offset a curve

typedef struct {
	BOOL isCoincident;
	SFIXED uA, uB;
	SPOINT ptA, ptB;		// mainly for internal use...
} CURVE_INTERSECTION, *P_CURVE_INTERSECTION;

typedef struct {
	CURVE_INTERSECTION points[4];
	U16 n;

} CURVE_SECT_LIST, *P_CURVE_SECT_LIST;

void CurveIntersectionPoints(P_CURVE curveA, P_CURVE curveB, P_CURVE_SECT_LIST list, SCOORD err);
// Calculate where two curve segments intersect

U16 CurveXRaySect(P_CURVE c, P_SPOINT pt, U16 depth);
// Count the number of intersections of a horizontal ray starting
//	at pt with the curve c. Will count end point intersections 
//	for connected paths only once.
// Always pass zero for depth.


//
// The curve stepper is a quick way to find approximate points along the curve
// Note that currently it suffers from very poor positional accuracy but it is 
//	useful for calculating curve lengths

typedef struct {
	U16 nLeft;	// the number of steps left
	
	SFIXED u;
	SPOINT pt;
	SPOINT prevPt;
	
	SFIXED du;
	SPOINT dxH;		// Note that these values are all left shifted by 8 bits
	SPOINT dvH;		// to increase accuracy
	SPOINT ptH;
} CURVE_STEPPER;

void CurveInitStepper(P_CURVE c, U16 n, CURVE_STEPPER* stepper);
BOOL CurveStep(CURVE_STEPPER* stepper);


//
// Cubic Curve Routines
//

void CubicDivide(SPOINT cubic[4], SPOINT a[4], SPOINT b[4]);
int CubicToQuadratic(SPOINT cubic[4], CURVE* quad, int maxN, SCOORD maxErr);
void QuadraticToCubic(CURVE* quad, SPOINT cubic[4]);


#endif

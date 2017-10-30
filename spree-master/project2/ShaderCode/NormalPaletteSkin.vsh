/************************************************************************
NormalPaletteSkin.vsh
Copyright (C) 2002-2004, Indy21
*************************************************************************/

// CV_BONESTART + index is the start of the bones
//
// CV_LIGHT_DIRECTION is the light vector
// CV_WORLDVIEWPROJ_0-3 is the world-view-proj matrix
//
// CV_CONSTANTS = (0.0f, 0.5f, 1.0f, 0.2f)
// CV_CONSTANTS1 = (0.0f, 0.0f, ����ŧ�� �Ŀ�, 0.0f)
//

;------------------------------------------------------------------------------
; oPos	  = Output position
; oD0	  = Diffuse
; oD1	  = Specular
; oT0	  = texture coordinates
;------------------------------------------------------------------------------

vs.1.1

dcl_position0		v0             
dcl_normal			v1		
dcl_texcoord0		v2	
dcl_texcoord1		v3
dcl_texcoord2		v4		; S
dcl_texcoord3		v5      ; T
dcl_texcoord4		v6      ; S*T

; v3�� ����Ʈ ������ �ε��� ������ �ִ�
; FLOAT weight0 = v3.x, weight1 = v3.y;	
; FLOAT index0 = v3.z,   index1 = v3.w;  //���ؽ��� 2���� �� ����Ʈ�� �׿� ����� ���� �ε���

#include "../MeshControl/XVSAConstants.h"

; first bone results -> r1
mov a0.x, v3.z 

dp4 r1.x, v0, c[a0.x + 0] 
dp4 r1.y, v0, c[a0.x + 1] 
dp4 r1.z, v0, c[a0.x + 2] 

; 1th Normal transform
dp3 r3.x, v1, c[a0.x + 0] 
dp3 r3.y, v1, c[a0.x + 1] 
dp3 r3.z, v1, c[a0.x + 2] 

//second bone results -> r2
mov a0.x, v3.w 

dp4 r2.x, v0, c[a0.x + 0] 
dp4 r2.y, v0, c[a0.x + 1] 
dp4 r2.z, v0, c[a0.x + 2] 

; 2th Normal transform
dp3 r4.x, v1, c[a0.x + 0] 
dp3 r4.y, v1, c[a0.x + 1] 
dp3 r4.z, v1, c[a0.x + 2] 

//blend between transformed Position r1 and r2
mul r1.xyz, r1.xyz, v3.x 
mad r2, r2.xyz, v3.y, r1.xyz
mov r2.w, c[CV_CONSTANTS].z //set w to 1.0f

//r2 now contains final position
// transform to clip space
dp4 oPos.x, r2, c[CV_WORLDVIEWPROJ_0]
dp4 oPos.y, r2, c[CV_WORLDVIEWPROJ_1]
dp4 oPos.z, r2, c[CV_WORLDVIEWPROJ_2]
dp4 oPos.w, r2, c[CV_WORLDVIEWPROJ_3]

;blend between transformed Normal r3 and r4
;r4 Final Transformed Normal
mul r3.xyz, r3.xyz, v3.x 
mad r4, r4.xyz, v3.y, r3.xyz

; normalize Transformed Normal r4 
dp3 r4.w, r4, r4
rsq r4.w, r4.w
mul r4, r4, r4.w

;Dot product of L dot N
;and store it in r0.x for use with the lit instruction
dp3 r0.x, r4, -c[CV_LIGHT_DIRECTION]; 

;Compute the Vector from eye to vertex in object space
;and store the vector in r1
;r2 Tranformed Position
sub r1, c8, r2

;Normalize the eye vector
dp3 r1.w, r1, r1
rsq r1.w, r1.w
mul r1, r1, r1.w

;Compute half angle between the eye vector and the light
;vector. The result in stored in r2 = Half Vector
add r2, r1, -c[CV_LIGHT_DIRECTION]; -c4
dp3 r2.w, r2, r2
rsq r2.w, r2.w
mul r2, r2, r2.w

;Find N dot H and store the result in r0.y
dp3 r0.y, r2, v1

;Place the power in r0.w
mov r0.w, c9.y

;Find the final diffuse and specular intensities
lit r4, r0

;Compute the specular light value
mul r5, r4.z, c10

;Compute the diffuse light value and add the ambient light
mul r6, r4.y, c7
mov r8, c7
mad r6, r6, r8, c6

;Add the specular component and output the final color
add oD0, r5, r6

//pass through texcoords
mov oT0.xy, v2



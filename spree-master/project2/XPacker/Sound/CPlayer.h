#ifndef CDPLAYER_H
#define CDPLAYER_H
typedef struct _COLLISION_PLAYER{
	LPMYOBJ Obj;
	D3DMATRIXHANDLE HeadMatrix;
	D3DMATRIXHANDLE BodyMatrix;
} COLLISION_PLAYER;

extern COLLISION_PLAYER CollisionPlayer;
#endif 
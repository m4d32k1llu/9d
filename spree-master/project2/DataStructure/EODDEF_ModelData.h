#ifndef _EODDEF_MODELDATA
#define _EODDEF_MODELDATA

#include "EODEXTDEF_GlobalObject.h"

#include "XTextureController.h"

//------------------------------------------------------------------------------------
// Model texture manager...
_XTextureManager		g_CharacterTextureArchive;
_XTextureManager		g_MobTextureArchive;

//------------------------------------------------------------------------------------
// Character model archive...

int						g_Model_GenderSeparateIndex = 300;		// �̼�ġ���� ������ ������
int						g_MaskModel_GenderSeparateIndex = 70;	// �̼�ġ���� ����ũ�� ������ ������
// ���� �߰��� ���� �� �����̳� �� 220�� ������. ���ڴ� 140������ �̹Ƿ� �ι�° �� �����̳� ������ 80ĭ��
int						g_MaskModel_GenderSeparateIndex2 = 80;	// �̼�ġ���� ����ũ�� �ι�° �߰� �� ������ ������
int						g_FaceMaskModel_GenderSeparateIndex = 5;	// �̼�ġ���� ����� ������ ������

int						g_Model_SorimHairSeparateIndex = 100;	// �̼�ġ���� �Ҹ��� �Ӹ��� ������

int						g_Model_GroupSeparateIndex[9];

// Common object <biped & mesh>
_XBipedController		g_FemaleBipedObject;
_XBipedController		g_MaleBipedObject;

D3DXMATRIX				g_BaseOBBAxisMatrix;
_XOBBData				g_CameraOBB;

_XAnimMeshAbstractClass* g_UserModel_HairType	[_XDEF_USERMODELTYPECOUNT_HAIR    ];
_XAnimMeshAbstractClass* g_UserModel_FaceType	[_XDEF_USERMODELTYPECOUNT_FACE    ];
_XAnimMeshAbstractClass* g_UserModel_UBodyType	[_XDEF_USERMODELTYPECOUNT_UBODY   ];
_XAnimMeshAbstractClass* g_UserModel_LBodyType	[_XDEF_USERMODELTYPECOUNT_LBODY   ];
_XAnimMeshAbstractClass* g_UserModel_LHandType	[_XDEF_USERMODELTYPECOUNT_LHAND   ];
_XAnimMeshAbstractClass* g_UserModel_RHandType	[_XDEF_USERMODELTYPECOUNT_RHAND   ];
_XAnimMeshAbstractClass* g_UserModel_LFootType	[_XDEF_USERMODELTYPECOUNT_LFOOT   ];
_XAnimMeshAbstractClass* g_UserModel_RFootType	[_XDEF_USERMODELTYPECOUNT_RFOOT   ];
_XAnimMeshAbstractClass* g_UserModel_Cap		[_XDEF_USERMODELTYPECOUNT_CAP     ];
_XAnimMeshAbstractClass* g_UserModel_Adae		[_XDEF_USERMODELTYPECOUNT_ADAE    ];
_XAnimMeshAbstractClass* g_UserModel_Earring	[_XDEF_USERMODELTYPECOUNT_EARRING ];
_XAnimMeshAbstractClass* g_UserModel_BackPack	[_XDEF_USERMODELTYPECOUNT_BACKPACK];
_XAnimMeshAbstractClass* g_UserModel_Mask		[_XDEF_USERMODELTYPECOUNT_MASK		];
_XAnimMeshAbstractClass* g_UserModel_FaceMask	[_XDEF_USERMODELTYPECOUNT_FACEMASK	];

_XAnimMeshAbstractClass* g_ItemModel_Weapon		[2][_XDEF_ITEMMODELTYPECOUNT_WEAPON	 ];
_XAnimMeshAbstractClass* g_ItemModel_MartialBook[_XDEF_ITEMMODELTYPECOUNT_MARTIALBOOK ];
_XAnimMeshAbstractClass* g_ItemModel_Potion		[_XDEF_ITEMMODELTYPECOUNT_POTION	 ];
_XAnimMeshAbstractClass* g_ItemModel_Throw		[_XDEF_ITEMMODELTYPECOUNT_THROW		 ];
_XAnimMeshAbstractClass* g_ItemModel_Socery		[_XDEF_ITEMMODELTYPECOUNT_SOCERY	 ];
_XAnimMeshAbstractClass* g_ItemModel_ETC		[_XDEF_ITEMMODELTYPECOUNT_ETC		 ];
_XAnimMeshAbstractClass* g_ItemModel_JewelBox	[_XDEF_ITEMMODELTYPECOUNT_JEWELBOX	 ];
_XAnimMeshAbstractClass* g_ItemModel_Elixir		[_XDEF_ITEMMODELTYPECOUNT_ELIXIR	 ];

//=================================================================================
// Monster model archive...
_XBipedController		 g_MOBBIPEDMODEL[_XDEF_MODELCLASSCOUNT_MOB];
_XAnimMeshAbstractClass* g_MOBMODEL[_XDEF_MODELCLASSCOUNT_MOB];

//=================================================================================
// NPC model archive...
_XBipedController		 g_NPCBIPEDMODEL[_XDEF_MODELCLASSCOUNT_NPC];
_XAnimMeshAbstractClass* g_NPCMODEL[_XDEF_MODELCLASSCOUNT_NPC];


// ================================================================================
// Dummy model archive...
_XBipedController			g_DUMMYBIPEDMODEL[_XDEF_MODELCLASSCOUNT_DUMMY];
_XAnimMeshAbstractClass*	g_DUMMYMODEL[_XDEF_MODELCLASSCOUNT_DUMMY];

// ================================================================================
// Seat model archive...
_XAnimMeshAbstractClass*	g_SEATMODEL[_XDEF_MODELCLASSCOUNT_DUMMY];

// ================================================================================
// Flag model archive... - �� ȫ���� flag
_XAnimMeshAbstractClass*	g_FLAGMODEL[_XDEF_MODELCLASSCOUNT_FLAG];


#ifdef _XDWDEBUG	
_XMeshContainer			g_AxisModel;
#endif

#endif
#pragma once

// Monster, Move, Restart
// Main = 2, 3, 4, 5
// 16 - 47

struct _sHeader;

// Zone : Move, Monster, Restart etc..
#define MSG_NO_CHARAC_INFO						16		// ĳ���� ������ (�ڱ��ڽſ��Ը�)
#define MSG_NO_ZONE_RESTART						17		// ĳ���� ����� ����� ��Ŷ
#define MSG_NO_CHARAC_SET_STATE					18		// ĳ���� ���� ����. �÷��׿� ���� �ٸ� �ǹ�.

#define MSG_NO_STATE_CHANGE_REQUEST				19		// ĳ������ ���� ���� ��û : �г��� ������� ���� ���� ��� ���
#define MSG_NO_STATE_CHANGE_RESULT				19		// ���� ���� ��û�� ����

#define MSG_NO_CHARAC_GAMESTARTSIGNAL			20		// ���� ���� �ñ׳�
#define MSG_NO_CHARAC_RETURN_LOBBY				21		// ������ �κ�� ����
#define MSG_NO_DISCONNECT						22		// ���������� ���� �޽����� ������ �������� ��
#define MSG_NO_LOGOUT							23		// �α� �ƿ�

enum EN_MOB_WALK_MODE
{
	MOB_WALK_MODE=0,
	MOB_RUN_MODE
};

enum EN_DEAD_FROM
{
	en_dead_from_monster = 0,
	en_dead_from_player,
	en_dead_from_monster_in_combat,				// ����� ���� �÷��̾ ��� ���Ϳ���
	en_dead_from_player_in_combat				// ����� ���� �÷��̾ ����� ���� �÷��̾��
};

// �ʵ� ���� ���� �޾Ƽ� ĳ���� ���¸� ������Ʈ �Ѵ�.
// ���, �ִ���, ���� ����ġ 
//#define MST_NO_CHARAC_INFO					16
struct MSG_CHARAC_INFO		// 2004/08/14 ������Ʈ
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cZone ;					// �о���� �� ��ȣ...
	
	// ���� Power 
	short				sCurLifePower ;			// ���� ����
	short				sCurForcePower ;		// ���� ����
	short				sCurConcentrationPower ;// ���� ����
	
	// �ִ� Power
	short				sMaxLifePower ;			// �ִ� ����
	short				sMaxForcePower ;		// �ִ� ����
	short				sMaxConcentrationPower ;// �ִ� ����
	
	short				sLeftPoint ;
	short				sLeftMasteryPoint;		// ���� �����͸� ����Ʈ
	
	// ���� �� : �ִ� ���� ���� ���������� ���� �����ǹǷ�, ���Խ�Ű�� �ʾƵ� �ȴ�. 
	unsigned int		uiLeft_Jin ;			// ���� ��
#ifdef _XTS_FAME
	int					iNotoriety ;			// �Ǹ� ��ġ�� ����.
#else
	unsigned int		uiLeft_Gong ;			// ���� ��
#endif
	
	short				sRetribution ;			// ���� 
	int					sHonor;					//��  8
	short				sShowdowm ;				// �� ����
	
	// ��ġ ����
	float				fInitPosX ;				// �ʱ� ��ġ 
	float				fInitPosZ ;				// �ʱ� ��ġ
	int					iTimeCode ;				// ���� ���� �ð�
	short				sStandingOBB ;			// ���ִ� ��ֹ� 
	
	short				sWoundRate ;			// �ܻ� ���� 0 - 100
	short				sInsideWoundRate ;		// ���� ���� 0 - 100
	u_short				usFatigueRate ;			// �Ƿε�	 0 - 100
	
	short				sFuryParameter;			// �г� �Ķ���� : 0 - 10000 ������

	char				cRespawnServerNo;		// ��Ȱ ��ġ�� ����� ���� ��ȣ : 0�̸� ����
	char				cRespawnPosName[13];	// ��Ȱ ��ġ �̸�. ServerNo�� 0�� �ƴ� ��쿡�� ��ȿ

	// �� ���� ����
	int					iWinScore;
	int					iLostScore;
	int					iTieScore;
	int					iSeriesCount;

	int					iMaxSeriesWinCount;		// �ִ� ���� ���
	int					iMaxSeriesLoseCount;	// �ִ� ���� ���
	int					iAbuseCount;
	char				cPrevMatchResult;		// 0:��, 1:��, 2:��
	
	// ����, ���� ���, �⿩��
	char				cClass;
	char				cClassGrade;
	int					iContribution;
	
	char				cGM ;					// 0: �Ϲ� ĳ����, 1�̻�: GM ĳ����
	
	int					iLeftLevelupLimit;		// 11������ 12�������� ���� ���� �ð� : 0�̸� ����
	
#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
	u_char				ucPeaceMode : 2;		// 0:��ȭ���   1:�������   2:PK���
	u_char				ucHideMode : 1;			// 0:��������   1:���ż�������
	u_char				ucClothView : 1;		// 0:ġ���ǻ� ���̱�(Default)   1:�Ϲ��ǻ� 
	u_char				ucCharacModeReserve : 4;
#else
	char				cPeaceMode ;			// 0 - ��ȭ ���. 1 - ���� ���.	
	char				cHindingMode ;			// 0 - ��������, 1 - ���ż�������
#endif

#ifdef _XDEF_CASTLEBATTLE_MOD_CHARACINFO
	u_char				ucPrevDeadFrom;			// �������� �׾��°� EN_DEAD_FROM
	u_char				ucDeadCountInCombat;	// ����� �� ���� ȸ��

	u_short				usLeftCombatPenaltyTime;	// ����� ��Ȱ �г�Ƽ
#endif
} ;


// ����� ����� ��Ŷ 
// ����޴� ��ġ = ����ġ(��), ��ġ, ü�¸� ȸ��(1/4)�ǰ�, �������� �״�� �����Ѵ�. 
//#define MSG_NO_ZONE_RESTART					17
struct MSG_ZONE_RESTART
{
	_sHeader			header;
	u_char				ucMessage;

	float				fRestartPosX;			// ����� ��ġ
	float				fRestartPosZ;			// ����� ��ġ

	unsigned int		uiJin;					// ���� ��

	short				sVitalPower;			// ���� ü��
	char				cSeverNum ;				// ���� ��ȣ
	
};

//#define MSG_NO_CHARAC_GAMESTARTSIGNAL				20
struct MSG_CHARAC_GAMESTARTSIGNAL
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usUniqueID;
};

//#define MSG_NO_CHARAC_RETURN_LOBBY				21
struct MSG_CHARAC_RETURN_LOBBY
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usUniqueID;
};

//#define MSG_NO_DISCONNECT							22
struct MSG_DISCONNECT
{
	_sHeader			header;
	u_char				ucMessage;
};

//#define MSG_NO_LOGOUT								23
struct MSG_LOGOUT
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usUniqueID;
};


// Zone ������ �ൿ
// �̵�, ä��. �J�� etc..
#define MSG_NO_ZONE_MOVE							24		// ĳ���� �̵�
#define MSG_NO_ZONE_POSITIONING						25		// ���̵��� �ʱ���ġ�� ������ �޼���, ���� Positioning�� ���δ�.
#define MSG_NO_ZONE_REQ_CHARAC_INFO					26		// �ٸ� ĳ������ �� ������ �䱸�Ѵ�.
#define MSG_NO_ZONE_CHARAC_INFO						27		// ĳ������ �� ���� (�ٸ� �������� �����ϴ� ����)

#define MSG_NO_ZONE_CHAT_NORMAL						28		// �Ϲ� ä��
#define MSG_NO_MONSTER_WARP							29
#define MSG_NO_MONSTER_MOVE							30		// ���� �̵�
#define MSG_NO_MONSTER_VITAL						31		// ���� ü��
#define MSG_NO_CHARAC_CTRL							32		// �ɸ��� ��Ʈ�� �޽���.(���,...) S->C
#define MSG_NO_CHARAC_SELECT_MODE					33		// C->S : ����Ŀ� ��Ȱ ��� ����.
#define MSG_NO_CHARAC_REBIRTH_TARGET				34		// �������� �츰��.(�������� ������Ŷ. -1 ����. �ܴ̿� ����. )
#define MSG_NO_MONSTER_TRANSFORM					35		// ���Ͱ� �����Ѵ�. 

//#define MSG_NO_ZONE_MOVE							24
struct MSG_ZONE_MOVE
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usUniqueID;					// ����ũ ���̵�

	float				fX;							// ���ִ� X��ǥ
	float				fZ;							// ���ִ� Z��ǥ
	float				fgotoX;						// �̵��� X��ǥ
	float				fgotoZ;						// �̵��� Z��ǥ
	short				sStandingOBB;				// ��� ���ִ� OBB�ε���
	short				sCharacIndex : 4;			// ĳ���� �ε���
	short				sAnimationNumber : 12;		// �ִϸ��̼� ��ȣ
};

// �̵� �ý��� ���� =============================
struct MSG_MOVE
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				move_mode;
	u_char				sequence;

	enum
	{
		en_move_mode_req_move = 0,		// 0 �̵� ��û, �����ϸ� error�� ����,
		en_move_mode_player_move,		// 1 req_move�� �Ϲ����� �������� �ֺ��� �ٸ� �÷��̾�Ե� ���޵ȴ�
		en_move_mode_charac_stop,		// 2 �������� �� C->S
		en_move_mode_stop,				// 3 �������� �� S->C

		en_move_mode_lps_jump_req = 10,	// 10 LPS���� ���� �̵���û�� ���δ�.
		en_move_mode_lps_jump_success,	// 11 ���� �̵� ��û ����

		en_move_mode_goto_pos = 20,		// 20 ���� -> Ŭ���̾�Ʈ�� ������ �̵� ��� :sequence�� ����
		en_move_mode_set_sequence,		// 21 �������� �ٽ� ����

		en_move_mode_error = 30,		// 30 OBB �Ǵ� tile ���� �����̷� �Ѵ�.
	};
};

// en_move_mode_req_move = 0
struct MSG_MOVE_REQ_MOVE : public MSG_MOVE
{
	float				goto_x;
	float				goto_z;

	u_char				move_speed;		// 100�� ����, -30%�̸� 70, +50%�̸� 150
	u_char				direction;		// ���� - ������

	u_short				standing_obb;
	u_short				charac_index : 4;
	u_short				ani_number : 12;
};

// en_move_mode_player_move = 1
struct MSG_MOVE_PLAYER_MOVE : public MSG_MOVE 
{
	float				goto_x;
	float				goto_z;

	u_short				usUID;			// unique id

	u_char				move_speed;		// 100�� ����, -30%�̸� 70, +50%�̸� 150
	u_char				direction;		// ���� - ������

	u_short				standing_obb;
	u_short				charac_index : 4;
	u_short				ani_number : 12;
};

// en_move_mode_charac_stop = 2
struct MSG_MOVE_CHARAC_STOP : public MSG_MOVE
{
	float				x;
	float				z;

	u_char				move_speed;		// 100�� ����, -30%�̸� 70, +50%�̸� 150
	u_char				direction;		// ���� - ������

	u_short				standing_obb;
	u_short				charac_index : 4;
	u_short				ani_number : 12;
};

// en_move_mode_stop = 3
struct MSG_MOVE_STOP : public MSG_MOVE 
{
	float				x;
	float				z;

	u_short				usUID;

	u_char				move_speed;		// 100�� ����, -30%�̸� 70, +50%�̸� 150
	u_char				direction;		// ���� - ������

	u_short				standing_obb;
	u_short				charac_index : 4;
	u_short				ani_number : 12;
};

// en_move_mode_lps_jump_req = 10
struct MSG_MOVE_LPS_JUMP_REQ : public MSG_MOVE
{
	float				next_x;
	float				next_z;

	u_short				lps_index;

	u_char				check_type;		// ������ üũ�ϴ���?
	u_short				check_index;	// üũ�ϴ� ���� index
};

// en_move_mode_lps_jump_success = 11
struct MSG_MOVE_LPS_JUMP_SUCCESS : public MSG_MOVE
{
	float				goto_x;
	float				goto_z;

	u_short				usUID;
};

// en_move_mode_goto_pos = 20
struct MSG_MOVE_GOTO_POS : public MSG_MOVE
{
	float				goto_x;
	float				goto_z;

	u_short				usUID;
};

// en_move_mode_set_seqence = 21
struct MSG_MOVE_SET_SEQUENCE : public MSG_MOVE
{
};

// en_move_mode_error = 30
struct MSG_MOVE_ERROR : public MSG_MOVE
{
	u_char				ucCode;

	/* 
		0 - OBB �Ǵ� Tile���� �����̷� �Ѵ�. ����
		1 - ���� �̵��� ���� �̻����� ��û ����
	*/
};

// ==========================================

// ���� �̵�, �ð� ���� �Ҵ�
//#define MSG_NO_ZONE_POSITIONING					25
struct MSG_ZONE_POSITIONING
{
	_sHeader			header;
	u_char				ucMessage;

	char				cZone;

	float				fInitPosX;					// ��ġ
	float				fInitPosZ;					// ��ġ
	short				sStandingOBB;
	int					iTimeCode;					// ���� ���� �ð�
}; 

//#define MSG_NO_ZONE_REQ_CHARAC_INFO				26
struct MSG_ZONE_REQ_CHARAC_INFO
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usUniqueID;					// �˰��� �ϴ� ������ ����ũ ���̵�
};

//#define MSG_NO_ZONE_CHARAC_INFO					27
struct MSG_ZONE_CHARAC_INFO
{
	_sHeader			header;
	u_char				ucMessage;
	
	char				cName[13];					// ĳ���� �̸�
	char				cNickName[13];				// ��ȣ	

	u_short				usSelectNicID ;				// ������ Ư�� ��ȣ ID	'0xffff' �̸� cNickName(���κ�ȣ ���û���)
 
	
	u_short				usUniqueID;
	
	char				cGM			: 5 ;			// GM �̸� 1 
	char 				cSex		: 3 ;			// ĳ���� ���� 0 : ����, 1 : ����.
	
	char				cFace;						// ��
	char				cHair;						// �Ӹ� ���
	
	char				cGroup;						// �Ҽ� ����
	
	char				cAmorType;					// ����	0
	char				cPantsType;					// ����	1
	char				cCapType;					// ���� 2
	char				cAdaeType;					// �ƴ� 3
	char				cShoesType;					// �Ź� 4
	char				cBackPackType;				// �賶 
	char                cWeaponType ;               // ���� Ÿ��
	
	short				sAmor;						// ���� 0
	short				sPants;						// ���� 1
	short				sCap;						// ���� 2
	short				sAdae;						// �ƴ� 3
	short				sShoes;						// �Ź� 4
	short				sBackPack;					// �賶
	
	short				sUseWeapon;					// ��� ����
	
	char				cCharacState;				// ĳ������ ���� : 0 = Alive, 1 : ���, 2 : ȥ��  3: �г�  4 : ����

	float				fCurPosX ;					// 
	float				fCurPosZ ;					//

	DWORD				dwConditionState ;			// ������ ���� : Condition�� ��Ÿ ����(��,��Ƽ,��)...
	
	char				cMoveSpeed;
	char				cAttackSpeed;

	u_char				ucGamingStatePK : 1;		// 1�̸� PK, 0�̸� ���Ľ�
	u_char				ucGamingStatePE : 1;		// 1�̸� ���� ����
	u_char				ucGamingStateSafe : 2;		// 0:�Ϲ� 1:�������� 2:��� 3:�߹� 
	u_char				ucGamingStateAbsent : 2;	// 0:Normal  1:�ڸ����
	u_char				ucGamingStateFM : 1;		// 0:�Ϲ�	1:��������
	u_char				ucGamingState : 1;			// reserved
	
	short				sHyperRunID ;			// ��� ��ų ���̵�
	u_char				ucHyperRunStep ;		// 0 - 11 ����. ( ��, ��ų ���̵� ��ȿ�� ���� �� �� ���� )
	int					iOR_Index ;				// ����, ��ȸ	
	char				cOR_Class ;				// ���
#ifdef _XTS_ALIANCE_20061018	
	int                 iAllyIndex;				// ���� ����.
#endif	//#ifdef _XTS_ALIANCE_20061018

#ifdef _XTS_FAME
	u_char				ucFameClass : 4 ;		// �Ǹ� Ŭ����
	u_char				ucHonorClass : 4 ;		// �� Ŭ����

	#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI

		// ����ȥ�� ���ݷ��� �ִ밪�� ���� �Ӽ��� 1 ����, �ٸ� �͵��� 0
		u_char				uc4Eff_MaxDam_minus : 1;
		u_char				uc4Eff_MaxDam_plus : 1;
		u_char				uc4Eff_MaxDam_hon : 1;
		u_char				uc4Eff_MaxDam_sin : 1;

		// ����ȥ�� ���׷��� 1 �̻��� ���� 1�� ����
		u_char				uc4Eff_Res_minus : 1;
		u_char				uc4Eff_Res_plus : 1;
		u_char				uc4Eff_Res_hon : 1;
		u_char				uc4Eff_Res_sin : 1;

	#else
		u_char				ucReserve1 ;			// 
	#endif //_XDEF_4EFF_SYSTEM_070625_KUKURI
	
	#ifdef _XTS_VIEWPKTARGETLEVEL
		short				sInnerLevel;			// level
	#else
		u_short				usReserve2 ;			//
	#endif //_XTS_VIEWPKTARGETLEVEL

#else
	int					iHonor;					// ��
#endif	//#ifdef _XTS_FAME


#ifdef ALL_RB_SELECT_CLOTHES_080404_KUKURI
	u_char				ucClothView : 1;		// 0:ġ���ǻ�   1:�Ϲ��ǻ�
	u_char				ucTemp1 : 3;
#else
	u_char              ucTemp1 : 4 ;
#endif

	u_char              ucWeaponStrength : 4 ;   // ������ ���� ȸ��. 0 - 15
	char				cMaskType;				/// ���� Ÿ��
	short				sMask;					// ����
	
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
	u_short				usFMNick;
#endif

#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : ����� //breif : ��Ʈ ������
	 short				sSetVisualEffect1;	// ��Ʈ �������� ���־� ����Ʈ
	 short				sSetVisualEffect2;
#endif
};

struct MSG_CHARAC_SET_STATE
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cFlag ;			// EN_STATE_PACKET_FLAG
	
	__charac_state		state ;
};

// �Ϲ� ä��
// sLength = strlen(cSay) + 18 (NULL ���� ���� )
// ���� ä�� ���ڿ�: strlen(cSay) = sLength - 18 
// �߸��� ��Ŷ�� ���͵� ä�� ���ڿ��� �������� ������ 0���� ������ ����
// cSay[ucLength-18] = 0 ; ���� ucLength �� 68 �� �Ѵ� ���� �б���
//#define MSG_NO_ZONE_CHAT_NORMAL					28

#define _XDEF_CHATMAXLENGTH		81

struct MSG_ZONE_CHAT_NORMAL
{
	_sHeader			header;
	u_char				ucMessage;

	char				cMode;		//	0-15�Ϲ�A(Į��), 16 ���� 32 �ͼӸ� 64 ��ε�, 96-99 ��ġ�� 127 ����� �޼���. ������ ��.
									// 50: [Vina Shutdown] �Ƿε� ����(1�ð����� ���)


	char				cName[13];
	char				cSay[_XDEF_CHATMAXLENGTH];		//	ä�� �޽���( strlen + 1 : NULL ���� ) 
														// _XDEF_CHATMAXLENGTH�� �ִ���̴�..���� �� ©�� ����.
														// ��尡 50�϶� ���� ���� �Ƿε� 

};

//#define MSG_NO_MONSTER_WARP		 : 29 ������ �����̵� 
struct MSG_MONSTER_WARP
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobType : 13;				// ���� Ÿ�� (0-8192)
	u_short				usInfo : 3;					// 0 : ���� ����
													// 1 : ������ ���� ����� (��ġ ����)
													// 2 : ������ ���� ��Ÿ����

	u_short				usMobID;					// ������ ���̵�. 0 - 4191 ������ �����ϰ� ���� �ִ�. 4000����...

	char				cMobVitalRate;				// -1�̸� ���� ��.

	float				fGotoX;
	float				fGotoZ;

	u_char				ucMobWalkMode : 4 ;		// ������ �ȱ� ��� : 0 �Ϲ�, 1 : �޸��ų� �����̵�...
	u_char				ucMobSizeType : 4 ;		// 0 : �Ϲ� ũ��, 1 ~ 15 : ���� ũ��.
};

//#define MSG_NO_MONSTER_MOVE						30
struct MSG_MONSTER_MOVE
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_short				usMobType ;			// ������ Ÿ��
	
	u_short				usMobID ;			// ������ ���̵�. 
	
	float				fX ;					
	float				fZ ;
	float				fGotoX ;
	float				fGotoZ ;
	
	char				cMobVitalRate ;			// ���� ü��
	
	u_char				ucMobWalkMode : 1;		// ������ �ȱ� ��� : 0 �Ϲ�, 1 : �޸��ų� �����̵�...
	u_char				ucSpeedRate: 7;			//(������ �����ӵ�/5)�� �� (ex: �����ӵ� = 100/5 = 20)

} ;

//#define MSG_NO_MONSTER_VITAL						31
struct MSG_MONSTER_VITAL
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobID;
	char				cMobVitalRate;		// -1 �̸� ������
};

// ���� ���� ��Ŷ : ������ ���� ����. 
//#define MSG_NO_MONSTER_TRANSFORM		35		// ���Ͱ� �����Ѵ�. 
struct MSG_MONSTER_TRANSFORM
{
	_sHeader			header ;
	u_char				ucMessage ;
	char				cChangeMode ;			// 0 : ����, 1 : ������ ���ư���.				
												// ( ����Ʈ�� �ٸ� �� �ؼ�, ��带 ������ �д�. )

	u_short				usMobID ;				// ��� ����
	u_short				usChangeType ;			// ��ȭ�ϴ� Ÿ��
	
	char				cMobVitalRate ;			// ���� ü��
	
	
} ;

// ���� ó�� ===========================================================

enum EN_CHARAC_CTRL_FLAG
{
	charac_ctrl_binsa=0,				// ü�� 0.. �����·� �����϶�. : usTime
	charac_ctrl_dead,					// ȥ�� ���·� ���� : usTime ���� ī��Ʈ
	charac_ctrl_freeze,					// ��¦�� : ��� �ʿ��� ��.
	charac_ctrl_release,				// Freeze ����
	charac_ctrl_awake_binsa,			// ��� ������ ���� �����
	charac_ctrl_awake_honsu,
	charac_ctrl_erase,					// ������..( �ٸ� ������ ����Ŀ� ��Ÿ����. )
	charac_ctrl_dead_in_match,			// ���� ���
	charac_ctrl_dead_by_pk,				// ������ �˸��� ��Ŷ : �ڽ��̸� ����
	charac_ctrl_dead_in_fm,				// �������� ���
} ;

union __charac_ctrl
{
	u_short		usTime ;	// Time(��) ���� �������� ���ϵ���.. 0xffff �̸� ������... 
	// �� .Release �ްų� �ð� ������ Ǯ������. 
} ;


struct MSG_CHARAC_CTRL
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cFlag ;
	
	u_short				uid ;			// uid �� ȥ�� Ȥ�� ��翡 ������.
	
	__charac_ctrl		context ;
} ;

enum EN_CHARAC_SELECT_MODE
{
	en_charac_select_at_city = 0,			// ����
	en_charac_select_at_save_position,		// ���� Ǭ ��
	en_charac_select_at_last_position,		// �� �ڸ�
	en_charac_select_wait_other_help,		// ������ ��ٸ���.
	en_charac_select_cancel_wait,			// ��翡�� ��ٸ��� �ʰ� �ٷ� ȥ�����·� ����.
	en_charac_select_spell_rebirth,			// �������� �������� ��Ȱ �Ǿ���
	en_charac_select_item_use,				// ȭŸ�Ŵ����� ��Ȱ
	en_charac_select_in_castle,				// ���� ���� ���� �� ���� ��û
};

struct MSG_CHARAC_SELECT_MODE
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cMode ;				// 0 : Waiting, 1 : Return, 2 : Rebirth, 
	char				cSelectPosition ;	// ��尡 1�϶�, ������ ��ġ �ε���. : ���� �Ⱦ���. 0���� ��.
} ; 


// ��� Ȥ�� ȥ�� ������ ������ ����� �Ҷ�, 
struct MSG_CHARAC_REBIRTH_TARGET_REQ
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	u_short				target ;		// ����� ���̵� �Ἥ ������.
	
	char				cFlag ;			// 0 : ������ �����,  1 : ���������� �츮��, 2 : ��ų�� �츮��.
	char				cIndex ;		// �÷��װ��� 1�̸� ���� �������� �κ��ε���, 2�̸� ����ų�� �ε���.
} ;


// uid�� Ÿ���� ������. �Ǵ� �����ߴ�. 
struct MSG_CHARAC_REBIRTH_TARGET
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cFlag ;			// 0 : ������ �����,  1 : ȥ������ �����. -1 ����.
	u_short				uid ;			
	u_short				usTargetID ;
} ;

// ��Ȱ ó��
#define MSG_NO_SAVE_RESPAWN_POS_REQUEST		251			// C->S ���� Ǯ�� ��û ��Ŷ
#define MSG_NO_SAVE_RESPAWN_POS_RESULT		251			// S->C �� ���� Ǯ�� ���. Yes/No�� ������ ����

struct MSG_SAVE_RESPAWN_POS_REQUEST
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usNPCID;		// ���� Ǯ�� NPC�� ���̵�
};

struct MSG_SAVE_RESPAWN_POS_RESULT
{
	_sHeader			header;
	u_char				ucMessage;

	char				cResult;		// 0: ����  1: NPC ���̵� ����  2: ���� ����
	char				cName[13];
};

// State Change ��û
enum STATE_CHANGE_ENUM
{
	state_change_en_normal_fury = 0,			// 0���� �ٽ� ���� : C->S �г� ���� ��� �Һ�Ǹ� ������ ��û�Ѵ�.
	state_change_en_start_fury,					// �г� ���� : S->C
	state_change_en_start_convergence,			// ���� ���� : C<->S ��û�� ����
	state_change_en_peace_mode,					// ��ȭ ���� �ٲ� ��
	state_change_en_battle_mode,				// ���� ���� �ٲ� ��
	state_change_en_to_pk_mode,					// pk ���� �ٲ� ��
	state_change_en_to_peace_mode,				// pk���� ��ȭ�� ��ȯ
	state_change_en_to_battle_mode,				// pk���� ������ ��ȯ
	state_change_en_notice_to_peace_mode,		// �������� pk���� ��ȭ�� ��ȯ
	state_change_en_notice_to_battle_mode,		// �������� pk���� ������ ��ȯ
	state_change_en_notice_to_pk_mode,			// �������� pk�� ��ȯ
	state_change_en_on_pe_mode,					// ���� ���� ���
	state_change_en_off_pe_mode,				// 
	state_change_en_notice_on_pe_mode,			// ������
	state_change_en_notice_off_pe_mode,
	state_change_en_req_pe_end,					// 15 C->S PE �� ������ ��, ������ ��û. : MSG_STATE_CHANGE_REQUEST
	state_change_en_res_not_pe_end,				// MSG_STATE_CHANGE_TO_PE : req_pe_end �� No ����. ���� ������ �ʾ���. �ð� ������Ʈ
	state_change_en_enter_tutorial_req,			// 17 Ʃ�丮�� �� �� ��û
	state_change_en_enter_tutorial,				// 18 Ʃ�丮�� ����.
	
	state_change_en_leave_tutorial_req,			// 19 ���丮�� ������ ��û.
	state_change_en_leave_tutorial,				// 20 Ʃ�丮�� ������.
	
	state_change_en_saferegion_req,				// 21 ���� ���� �Ӽ� ���� ��û 0:�Ϲ� 1:����
	state_change_en_saferegion_notice,			// 22 ������ ���� ��ȭ�� �˸��� 0:�Ϲ� 1:���� 2:��� 3:�߹�

	state_change_en_to_normal_state,			// 23 �ڸ���� -> ����
	state_change_en_to_absent_state,			// 24 ���� -> �ڸ����

	state_change_en_clothview_select_deck = 30,	// 30 �ǻ��̱� : ġ���ǻ� (��û/����)
	state_change_en_clothview_select_normal,	// 31 �ǻ��̱� : �Ϲ��ǻ� (��û/����)
};

struct MSG_STATE_CHANGE 
{
	_sHeader			header;
	u_char				ucMessage;
	
	u_char				ucMode;
};

struct MSG_STATE_CHANGE_REQUEST : public MSG_STATE_CHANGE
{
	char				cMapID;
	char				cAreaCategory;
	char				cAreaID;
};

struct MSG_STATE_CHANGE_SAFEREGION_REQUEST : public MSG_STATE_CHANGE
{
	u_char				ucSafeRegionState;		// 0,1 �Ϲݰ� �������� ��ȯ ��û 2:���, 3:�߹� �߰�
};

struct MSG_STATE_CHANGE_SAFEREGION_NOTICE : public MSG_STATE_CHANGE
{
	u_char				ucSafeRegionState;
	u_short				usCharacterUID;
};

// ó�� �Ǵ� �߰��� pk ���� ��ȯ�Ǹ�, �ڽſ���, MSG_STATE_CHANGE_TO_PK �� ����,
// �ֺ����� MSG_STATE_CHANGE_NOTICE �� ����.

// pk ��忡��, ��ȭ/���� ���� ����� ����, �ڽſ��� CHANGE_RESULT �� �ѹ� �� �Ŀ�, 
// ������ CHANGE_NOTICE �� ���� �ȴ�. 

struct MSG_STATE_CHANGE_RESULT : public MSG_STATE_CHANGE
{
	u_char				ucResult;				// 0�� OK, �������� No
};

// �ڱ� �ڽ�
struct MSG_STATE_CHANGE_TO_PK : public MSG_STATE_CHANGE
{
	u_short				ucLeftTime;				// �ִ� 600��
};


// �ٸ� ��� - pk/pe
struct MSG_STATE_CHANGE_NOTICE : public MSG_STATE_CHANGE 
{
	u_short				usUniqueID;
};


// pe mode --------------------------------------------------------��
// on_pe, off_pe
struct MSG_STATE_CHANGE_TO_PE : public MSG_STATE_CHANGE
{
	u_short				usLeftTime;				// ���� �ð� (��)
};
//-----------------------------------------------------------------��

// state_change_en_to_normal_state
// state_change_en_to_absent_state
struct MSG_STATE_CHANGE_ABSENT : public MSG_STATE_CHANGE
{
	u_short				usUniqueID;
};

// state_change_en_clothview_select_deck 
struct MSG_STATE_CHANGE_CLOTHVIEW_SELECT_DECK : public MSG_STATE_CHANGE
{
};

// state_change_en_clothview_select_normal
struct MSG_STATE_CHANGE_CLOTHVIEW_SELECT_NORMAL : public MSG_STATE_CHANGE
{
};


#define MSG_NO_STANCE_MOTION_CHANGE			36		// ���Ľ� ��� ����
struct MSG_STANCE_MOTION_CHANGE
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usID;
	short				sStanceMotion;
};


// �� ����
#define MSG_NO_OBJECT_SIMPLE_INFO			37		// Object���� ��ε�ĳ���� ����
#define MSG_NO_OBJECT_INFO_REQUEST			38		// �� ���� ��û ��Ŷ
#define MSG_NO_OBJECT_DETAIL_INFO			39		// Request�� ��û�ϰ� ����Ŷ���� ����

// �� ��߰� �׿� ������Ʈ��
enum EN_OBJECT_TYPE
{
	en_object_type_sega = 0,				// ����, ������ ����
	en_object_type_match_flag,				// �� ����� ����
	em_object_type_simple_info,		// ĳ���� ���� ����
	en_object_type_items_info,		// ������ ����
	en_object_type_ability_info,	// �ɷ�ġ ����
	en_object_type_all_info,		// ��ü ����
};

struct MSG_OBJECT_SIMPLE_INFO
{
	_sHeader			header;
	u_char				ucMessage;

	char				cObjectType;		// EN_OBJECT_TYPE
};

// �� ����� �ܼ� ���� : ������ 5�ʿ� �ѹ��� ��ε�ĳ����
struct MSG_OBJ_MATCH_FLAG_INFO : public MSG_OBJECT_SIMPLE_INFO
{
	char				cMatchMode;

	u_short				usMatchID;			// ���� ���̵�

	float				fFlagPosX;			// ����� ��ġ
	float				fFlagPosZ;
};

struct MSG_OBJECT_INFO_REQUEST
{
	_sHeader			header;
	u_char				ucMessage;

	char				cObjectType;		// EN_OBJECT_TYPE
};

struct MSG_MATCH_FLAG_INFO_REQUEST : public MSG_OBJECT_INFO_REQUEST 
{
	u_short				usID;
};

struct MSG_SEGA_INFO_REQUEST : public MSG_OBJECT_INFO_REQUEST
{
	u_short				usID;
};

//MSG_NO_OBJECT_INFO_REQUEST
struct MSG_CHARAC_MORE_INFO_REQ : public MSG_OBJECT_INFO_REQUEST
{
	char				cTo_or_From_Name[13] ;	
	char				cFlag ;
	enum
	{
		_request,			// a->b ���� �������� ��û
		_response_yes,		// b->a ���� �¶�
		_response_no,		// b->a ���� �ź�
		_response_cancel,	// �������� ���
		_response_error,	// ����(��밡 �α׿����� �Ǵ� �˼� ���� ����)
	};
};

struct MSG_OBJECT_DETAIL_INFO
{
	_sHeader			header;
	u_char				ucMessage;

	char				cObjectType;
};

struct MSG_MATCH_FLAG_DETAIL_INFO : public MSG_OBJECT_DETAIL_INFO 
{
	char				cMatchMode;

	u_short				usMatchID;

	u_short				usMatchCharac1;
	u_short				usMatchCharac2;
};

struct MSG_CHARAC_MORE_INFO : public MSG_OBJECT_DETAIL_INFO
{
	// TODO : ĳ������ ����(��������, ��ü����, ���� ������ ���� ��... )	

	char	cCharacName[13] ;

	// ��������
	short	sInnerLevel ;	// ����ܰ�
	char	cMoonpa ;		// ����
	char	cClass ;		// ��å	
	char	cClassGrade ;	// ���� �ܰ�

	// �ɷ�ġ ����
	short	sConstitution;				//�ǰ�
	short	sZen;						//����
	short	sIntelligence;				//����
	short	sDexterity;					//��ø
	short	sStr;						//�ٷ�


	// ������ ����
	enum _GET_ON_ITEMS_SLOT
	{
		pos_amor=0,         // 0
		pos_pants,			// 1
		pos_adae,			// 2
		pos_shoes,			// 3
		pos_cap,			// 4
		pos_gap_upper,		// 5
		pos_gap_lower,		// 6
		pos_backpack,		// 7
		pos_weapon_1,		// 8
		pos_weapon_2,		// 9
		get_on_max
	};
	
#ifdef _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI

	_sUser_Inven_v1_all getOnItems[get_on_max];

#else

	_sUser_Inven_v1_extension getOnItems[get_on_max];

	#ifdef _XDEF_SMELT_ITEM // Author : ����� 
	__inven_uselimit_info_v1        weapons[2]; 
	#endif

#endif
};

#define MSG_NO_TIME_CHECK				14

#ifdef _XDEF_SERVERTIMECHECK_071112_KERYGMA

// 3�� ���� �ѹ��� ������ �ð��� ä���� ������. 

struct MSG_TIME_CHECK
{
	_sHeader			header ;
	u_char				ucMessage ;
	u_char				ucMode;				// 0 - Timer, 1 - Hack Prog. List
	int					time;				// Mode 0
	char				prog1[17];			// 16byte. Mode 1
	char				prog2[15];			// 14byte. Mode 2
} ;

#else

// 60�� ���� �ѹ��� ������ �ð��� ä���� ������. 
// MSG_NO_TIME_CHECK 14 
struct MSG_TIME_CHECK 
{ 
	_sHeader header ; 
	u_char ucMessage ; 
	DWORD time ; // Ŭ���̾�Ʈ�� ���� �ð��� �ִ´�. : timeGetTime() 
} ;

#endif



#define MSG_NO_MONSTER_EXIST			40			// ������ ����
#define MSG_NO_REQ_SPELLEFFECT			41			// �ش� ���Ϳ��� �ɷ��ִ� ����, ����� ������ ��û
#define MSG_NO_RES_SPELLEFFECT			41			// �ش� ���Ϳ��� �ɷ��ִ� ����, ����� ������
#define MSG_NO_SPELLLIST_CHANGED		42			// ���� �����Ͱ� ������
#define MSG_NO_CONDITION_CHANGED		43			// ������ ������� �ٲ����

struct MSG_MONSTER_EXIST
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobType;
	u_short				usMobID;

	float				fX;
	float				fZ;
	char				cMobVitalRate;

	DWORD				dwConditionBit;		// 32���� �����
};

struct MSG_REQ_SPELLEFFECT
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobID;				// ������ ���̵�
};

#define MAX_SPELLEFFECTDATA_PER_PACKET	8		// ��Ŷ �ϳ��� ���� SPELL_EFFECT data�� ����

struct MSG_RES_SPELLEFFECT
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobID;

	u_char				ucRemain;				// 0:���� ������ ����   1:���� ������ ����

	short				aSpellID[MAX_SPELLEFFECTDATA_PER_PACKET];		// spell id array
	char				aSpellLevel[MAX_SPELLEFFECTDATA_PER_PACKET];	// �� �������� ������ ����

	u_short				aEffectID[MAX_SPELLEFFECTDATA_PER_PACKET];		// Effect ID array
};

struct MSG_SPELLLIST_CHANGED
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobID;
	u_char				ucMsgType;				// 0:���� ������  1:���� �߰�  2:Effect ������  3:Effect �߰���

	short				usSpellEffectID;
	char				ucSpellLevel;
};

struct MSG_CONDITION_CHANGED
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usMobID;

	DWORD				dwConditionBit;		// 32���� ����� ������
};

#define MSG_NO_MONSTER_CHASE_NOTICE		26		// ���Ͱ� ���� �Ѱ������� �˸���.

struct MSG_MONSTER_CHASE_NOTICE
{
	_sHeader			header;
	u_char				ucMessage;
	
	unsigned short		usDNPCID;				//�Ѱ��ִ� ������ ID
};

// state_change_en_enter_tutorial_req,	// 17 Ʃ�丮�� �� �� ��û
struct MSG_STATE_CHANGE_ENTER_TUTORIAL_REQ : public MSG_STATE_CHANGE
{
} ;

// state_change_en_enter_tutorial,		// 18 Ʃ�丮�� ����.
struct MSG_STATE_CHANGE_ENTER_TURIAL : public MSG_STATE_CHANGE
{
	u_char				ucResult ;		// 0 : ���� 
	// 1 : �̹� �� �ߴ�. �� ��.
} ;


// state_change_en_leave_tutorial_req,	// 19 ���丮�� ������ ��û.
struct MSG_STATE_CHANGE_LEAVE_TUTORIAL_REQ : public MSG_STATE_CHANGE
{
} ;


// state_change_en_leave_tutorial,		// 20 Ʃ�丮�� ������. : ��û���� ������ �����̴�. 
struct MSG_STATE_CHANGE_LEAVE_TUTORIAL : public MSG_STATE_CHANGE
{
	char				cMapID;
	float				fX;
	float				fZ;
} ;


#define MSG_NO_ETCSET								224		// ������ ����(��ġ�� etc.)

// ETC_SET  - ��ġ�� etc.
enum EN_CHAT_MODE
{
	en_chat_mode_white=96,
	en_chat_mode_black=97,
	en_chat_mode_class=98,
	en_chat_mode_all=99	
} ;

struct MSG_ETCSET
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;
};

enum EN_ETCSET_MODE
{
	en_etcset_mode_shout_set = 0,
	en_etcset_mode_shout_update,
	en_etcset_mode_shout_end,
	en_etcset_mode_shout_error
};

struct MSG_ETCSET_SHOUT_SET : public MSG_ETCSET
{
	// en_etcset_mode_shout_set : S->C
	// en_etcset_mode_shout_update : S->C

	char			cType;			// ä�ø��
	int				iLeftSecond;	// �����ð� - ��
};

struct MSG_ETCSET_SHOUT_END : public MSG_ETCSET
{
	// en_etcset_mode_shout_end : S->C
};

struct MSG_ETCSET_SHOUT_ERROR : public MSG_ETCSET
{
	// en_etcset_mode_shout_error : S->C

	u_char			ucError;

	/*	
	  0 - not in shout mode., ������ �����ִٸ�, ���ּ���.
	*/
};


// 
// ����ġ/���õ�/�������
//

#define MSG_NO_ACCOUNT_ITEM_USE_STATUS			225

struct _sND_AIUS_Grade
{
	// ��/��/��/��/��/�� End Time
	u_int			uiYear		: 5;		// base 2000, Year = 2000 + uiYear
	u_int			uiMonth		: 4;
	u_int			uiDay		: 6;
	u_int			uiHour		: 5;
	u_int			uiMinute	: 6;
	u_int			uiSecond	: 6;

	u_char			ucReserve;

	char			cItemType;				// ������� ������
	short			sItemID;
} ;

struct MSG_ACCOUNT_ITEM_USE_STATUS
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucMode;

	MSG_ACCOUNT_ITEM_USE_STATUS(u_short length, u_char mode)
	{
		header.sLength = length;
		header.sCrypto = 0;
		header.sCompressed = 0;

		ucMessage = MSG_NO_ACCOUNT_ITEM_USE_STATUS;

		ucMode = mode;
	}
};

enum EN_ACCOUNT_ITEM_USE_STATUS_MODE		// AIUS
{
		en_aius_mode_grade_set = 0,
		en_aius_mode_grade_end
};


/*
 *	
	���Ӽ����� ����,  
	������� ����ġ/���õ� �������� �ִٸ�,
	en_aius_mode_grade_set ���� �����ϴ�.
	���ٸ�, �ƹ��͵� ���� �ʽ��ϴ�. ���� ������ �����Ͻø� �˴ϴ�.
		
	�������� �����, 
	�����ϸ�, en_aius_mode_grade_set �� ����, 
	�����ϸ�, ������ ������ ��� ���и� ���ϴ�. 
 
	�׸���, set ��Ŷ�� cItemType, sItemID �� ����ߴ�,
	�������� �����Դϴ�. �� ������ �̿��Ͽ�, ���÷��� �Ͻø� �˴ϴ�.	
*				  
*/

struct MSG_AIUS_GRADE_SET : public MSG_ACCOUNT_ITEM_USE_STATUS
{
	// en_aius_mode_grade_set = 0
	_sND_AIUS_Grade			curGrade;

	MSG_AIUS_GRADE_SET() 
		: MSG_ACCOUNT_ITEM_USE_STATUS(sizeof(MSG_AIUS_GRADE_SET), en_aius_mode_grade_set)
	{
	}
};

struct MSG_AIUS_GRADE_END : public MSG_ACCOUNT_ITEM_USE_STATUS
{
	// en_aius_mode_grade_end  : s->C
	MSG_AIUS_GRADE_END()
		: MSG_ACCOUNT_ITEM_USE_STATUS(sizeof(MSG_AIUS_GRADE_END), en_aius_mode_grade_end)
	
	{
	}
};

#define MSG_NO_BW_WAR				180		// ������ ���� ��Ŷ

struct MSG_BW_WAR 
{
	_sHeader		header;
	u_char			ucMessage;

	u_char			ucBWMode;

	MSG_BW_WAR(u_short size, u_char mode)
		: header(size), ucMessage(MSG_NO_BW_WAR), ucBWMode(mode)
	{
	}
};

enum EN_BW_WAR_MODE
{
	en_bwwar_left_time = 0,			// S->C �пյ� ������ ���� ���� �����ð� �˷���
									// �� ��Ŷ�� ���� ������ ������ ���� ���̴�.

	en_bwwar_enter_gate_req,		// C->S ���� ��ȭ�� �� �ִ��� ����
	en_bwwar_enter_gate_success,	// S->C ����
	en_bwwar_enter_gate_fail,		// S->C ����
};

// en_bwwar_left_time = 0
struct MSG_BW_LEFT_TIME : public MSG_BW_WAR
{
	int				iLeftTime;		// �����ð� : ��, 0�̸� ���� ����

	MSG_BW_LEFT_TIME()
		: MSG_BW_WAR(sizeof(MSG_BW_LEFT_TIME), en_bwwar_left_time)
	{
		iLeftTime = 0;
	}
};

// en_bwwar_enter_gate_req
struct MSG_BW_ENTER_GATE_REQ : public MSG_BW_WAR
{
	u_short			usGateIndex;	// Object index

	MSG_BW_ENTER_GATE_REQ()
		: MSG_BW_WAR(sizeof(MSG_BW_ENTER_GATE_REQ), en_bwwar_enter_gate_req)
	{
	}
};

// en_bwwar_enter_gate_success
struct MSG_BW_ENTER_GATE_SUCCESS : public MSG_BW_WAR
{
	u_short			usGateIndex;

	MSG_BW_ENTER_GATE_SUCCESS()
		: MSG_BW_WAR(sizeof(MSG_BW_ENTER_GATE_SUCCESS), en_bwwar_enter_gate_success)
	{
	}
};

// en_bwwar_enter_gate_fail
struct MSG_BW_ENTER_GATE_FAIL : public MSG_BW_WAR
{
	u_short			usGateIndex;

	MSG_BW_ENTER_GATE_FAIL()
		: MSG_BW_WAR( sizeof(MSG_BW_ENTER_GATE_FAIL), en_bwwar_enter_gate_fail )
	{
	}
};

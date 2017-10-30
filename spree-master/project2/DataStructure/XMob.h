// XMob.h: interface for the _XMob class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XMOB_H_
#define _XMOB_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XUser.h"
#include "XObjectManager.h"
#include "XLodTerrain.h"
#include "XActionTable.h"
#include "EODEXTDEF_ModelData.h"
#include "XProc_MainGameDef.h"

#include "XModelManager.h"
#include "XMemoryPool.h"
#include <map>
#include <list>

#define _XDEF_MOBCOLLIDERADIUS				0.80f
#define _XDEF_MOBCOLLIDEOFFSET_DEFAULT		0.75f

#define _XDEF_MOBCOLLIDEFRUSTUM_DEFAULT		0.8f

#define _XDEF_MOB_KILLTIME					20000    // ������ �޼������� �ð� ���� ���� ��� �ð��� ������ ����


#define _XDEF_MOB_KILLSIGNAL_STOP			0
#define _XDEF_MOB_KILLSIGNAL_START			1
#define _XDEF_MOB_KILLSIGNAL_END			2

typedef struct 
{	
	DWORD		action      :	8;
	DWORD		idlemode    :	1;
	DWORD		movemode    :	1;
	DWORD		attacktype  :   3;
	DWORD		dietype		:	1;
	DWORD		skilltype	:	1;
	DWORD		reserved	:  17;
} _XMOB_ANIMATIONTYPE;

// �̵� ���� Ÿ��
typedef enum tagMonsterMoveType
{
	_XMONSTERMOVETYPE_WALK = 0,
	_XMONSTERMOVETYPE_RUN
} _XMOTION_MONSTERMOVETYPE;

typedef enum tagMonsterAttackType
{
	_XMONSTERATTACKTYPE_ATTACK1 = 0,
	_XMONSTERATTACKTYPE_ATTACK2,
	_XMONSTERATTACKTYPE_ATTACK3,
	_XMONSTERATTACKTYPE_RATTACK
} _XMOTION_MONSTERATTACKTYPE;

typedef enum tagMonsterDieType
{
	_XMONSTERDIETYPE_DIE1 = 0,
	_XMONSTERDIETYPE_DIE2
} _XMOTION_MONSTERDIETYPE;

typedef enum tagMonsterSkillType
{
	_XMONSTERSKILLTYPE_SKILL1 = 0,
	_XMONSTERSKILLTYPE_SKILL2
} _XMOTION_MONSTERSKILLTYPE;

typedef enum tagMonsterTransformStep
{
	_XMONSTERTRANSFORMSTEP_NONE = 0,
	_XMONSTERTRANSFORMSTEP_START,
	_XMONSTERTRANSFORMSTEP_END		
} _XMONSTERTRANSFORMSTEP;

// monster skill id
#define _XMOB_SKILL_TRANSFORM			6		//����
#define _XMOB_SKILL_TRANSFORM_BACK		7		//���� �ǵ��ư���
#define _XMOB_SKILL_STRONGBODY			8		//�ݰ��ұ�
#define _XMOB_SKILL_INVISIBLE			9		//������
#define _XMOB_SKILL_MEDITATION			10		//�������
#define _XMOB_SKILL_CURE				11		//ġ��
#define _XMOB_SKILL_INSTIGATION			12		//����
#define _XMOB_SKILL_RECKLESS			13		//����
#define _XMOB_SKILL_EXTERN_TRANSFORM	14		//������ȭ
#define _XMOB_SKILL_SIZE_TRANSFORM		15		//ũ�⺯��
#define _XMOB_SKILL_CHANGE_WEAPON		16		//���� ��ȯ
#define _XMOB_SKILL_SEPARATION			17		//�и�
#define _XMOB_SKILL_UNION				18		//��ü
#define _XMOB_SKILL_HALLUCINATION		19		//�н�
#define _XMOB_SKILL_SUICIDE				20		//�ڻ�
#define _XMOB_SKILL_REBIRTH				21		//��Ȱ
#define _XMOB_SKILL_CALL				22		//ȣ��
#define _XMOB_SKILL_SUMMON				23		//��ȯ

///////////////////////////////////////////////////////////////////////
// Monster action number...

#define		_XMOBACTIONNUMBER_IDLE			0
#define		_XMOBACTIONNUMBER_WALK			1
#define		_XMOBACTIONNUMBER_RUN			2
#define		_XMOBACTIONNUMBER_ATTACK1		3
#define		_XMOBACTIONNUMBER_ATTACK2		4
#define		_XMOBACTIONNUMBER_ATTACK3		5
#define		_XMOBACTIONNUMBER_BLOW			6
#define		_XMOBACTIONNUMBER_DIE1			7
#define		_XMOBACTIONNUMBER_DIE2			8
#define		_XMOBACTIONNUMBER_SKILL1		9
#define		_XMOBACTIONNUMBER_SKILL2		10
#define		_XMOBACTIONNUMBER_ENTERING		11
#define		_XMOBACTIONNUMBER_RATTACK		12

// ���� ���ۺ� Ÿ��
typedef enum tagMobMotionclassType
{
	_XMOBACTION_NONE	=	0,
	_XMOBACTION_IDLE,
	_XMOBACTION_MOVE,
	_XMOBACTION_ATTACK,
	_XMOBACTION_BLOW,
	_XMOBACTION_DIE,
	_XMOBACTION_SKILL,
	_XMOBACTION_ENTERING,
} _XMOBMOTIONCLASS_TYPE;

typedef struct _XMobMotionExistTable
{
	char	motion_idle;			
	char	motion_walk;
	char	motion_run;	
	char	motion_attack1;
	char	motion_attack2;
	char	motion_attack3;
	char	motion_blow;	
	char	motion_die1;
	char	motion_die2;
	char	motion_skill1;
	char	motion_skill2;
	char	motion_entering;
	char	motion_rattack;
	
	_XMobMotionExistTable()
	{
		motion_idle		=	-1;
		motion_walk		=	-1;
		motion_run		=	-1;		
		motion_attack1	=	-1;
		motion_attack2	=	-1;
		motion_attack3	=	-1;		
		motion_blow		=	-1;		
		motion_die1		=	-1;
		motion_die2		=	-1;		
		motion_skill1	=	-1;
		motion_skill2	=	-1;
		motion_entering	=	-1;
		motion_rattack	=	-1;
	}	
} _XMobMotionExistTable;

typedef struct 
{
	FLOAT xsize;
	FLOAT ysize;	
	FLOAT collideradius;
	FLOAT movespeed_walk;	
	FLOAT movespeed_run;	
	FLOAT attackrange;
	BOOL  standingerect;
	BYTE  weaponmaterial;
	BYTE  weaponweightclass;
	bool  soundplayed;		// Ÿ�Ժ��� �ѹ����� �������

	short level;
	short skill1_id;
	short skill2_id;
	short attackpowermax;	// Ŭ���̾�Ʈ ���Ϳ����� ���ݷ��� ����.
	char  party;
	
#ifdef _XTS_MONSTERREF_13
	unsigned short	scripttype;
#else
	char  scripttype;// LPS ������ �����Ѵ�.
#endif

	char  key_type;// LPS ������ �����Ѵ�.
	short key_id;// LPS ������ �����Ѵ�.
	
	char  rank;		// monster rank - ���̵�, ���� ���� etc..
	
	bool  usecurrentzone;
	char  explevel;
#ifdef _XDEF_MONSTERREF_14
	char  droplevel[30];
#else
	char  droplevel[12];
#endif

	int	resistMinus;	//�����׷�
	int	resistPlus;		//�����׷�
	int	resistHon;		//ȥ���׷�
	int	resistSin;		//�����׷�
	
	int	attribute;		// ����ȥ�� �Ӽ� (���� �̸� ǥ�ÿ� ���� v1.5���� ���� ��ȿ)

} _MOB_ATTRIBTABLE;

typedef struct _XMob_AttackResult
{
	short	sAddEffect[3];
	short	sEff_Time[3];
}_XMOB_ATTACKRESULT;

enum STATE_MOBACT { SA_NONE, SA_MOVE, SA_ATTACK };	// hotblood zerolevel

// ����ȿ��
struct _XMOB_STATE
{
	char	type;				// 0 : spell		1 : effect

	short	spellID;
	char	spellStep;

	short	 effectID;
};

// by mahwang
// ������ _XItem �� ��� ���� �ʾƵ� ��� ������ ���� ������ ����Ʈ üũ �ϴ� �κ��� �־ 
// �׳� ���� �д�.
class _XMob : public _XItem  
{

public:	
	unsigned short			m_MobID;					// �������� ���Ϳ��� �ο��ϴ� ����ũ ���̵� 
	unsigned short			m_MobType;					// ���� ������ ��Ÿ���� ���� 
	// transform
	unsigned short			m_NextTransformType;
	
	char					m_HealthPercentage;
	char					m_DisplayHealthPercentage;
		
	BOOL					m_InsideFrustum;
	BOOL					m_bShowMobName;
	BOOL					m_bShowBoldMobName;
	BOOL					m_bShowHPGauge;
	// Impact value
	BOOL					m_bImpact;
	// transform
	BOOL					m_RestoreOrgMonsterType;
	// hotblood zerolevel
	BOOL					m_bHoming;
	BOOL					m_bTargetOn;
	// client monster 
	BOOL					m_bClientMob;				// Ŭ���̾�Ʈ������ ó���ϴ� ���������� �� �� �ִ� ����
	// client DNPC
	BOOL					m_bClientDNPC;				// Ŭ���̾�Ʈ������ ó���ϴ� DNPC������ �� �� �ִ� ����

	int						m_PrevMotion;
	int						m_AlphaLevel;	
	int						m_AttackInfo;
	int						m_KillSignal;
	int						m_SpeedRate;
	// Impact value
	int						m_ImpactValue;
	// hotblood zerolevel
	int						m_Gender;
	int						m_Groupindex;
	int						m_ItemNo;
	int						m_AttackType;	// ��� �ε��� 	
	int						m_SkillIndex;	// ��ų �ε��� 	
	int						m_MaxLife;	
	int						m_CurrentLife;
	int						m_QueueNumber;	

	FLOAT					m_fMoveSpeed;
	FLOAT					m_PrevTime;	
	FLOAT					m_RotateAngle;
	FLOAT					m_AttackHitValue;
	// hotblood zerolevel
	FLOAT					m_OldRotateAngle;

	DWORD					m_LastRecivedMessageTime;	// ���������� �޼����� ���� �ð�... ���� �ð��� ������ �ڵ� �Ҹ�
	DWORD					m_DieToDeleteTimer;
	DWORD					m_DieSignalReceiveTime;
	// transform
	DWORD					m_TranformTimer;
	// ���� ȿ��
	DWORD					m_ConditionList;
	
	
	D3DXVECTOR3				m_Position;	
	D3DXVECTOR3				m_MoveVector;
	D3DXVECTOR3				m_EndPosition;
	// Impact value	
	D3DXVECTOR3				m_OriginPosition;
	D3DXVECTOR3				m_ViewDirection;

	D3DXMATRIX				m_MobMatrix;
				
	_XModelDescriptor		m_ModelDescriptor;	
	_XMOB_ANIMATIONTYPE		m_Action;
	_XMOB_ATTACKRESULT		m_AttackResult;
	STATE_MOBACT			m_MobActState;	

	_XStickGauge			m_MonsterMiniHpBorder;
	_XImageStatic			m_imageMiniHpBackBorder;
	
	_XPICKINGTARGETTYPE		m_AttackTargetObjectType;
	// transform
	_XMONSTERTRANSFORMSTEP  m_TransformStep;

	_XItem*					m_AttackTarget;
	// ���� ȿ��
	list <_XMOB_STATE*>		m_StateList;

public:
	_XMob();
	virtual ~_XMob();

	virtual	void			InitializeData();
	virtual	void			DestroyData();

	virtual	void			Create( unsigned short mobid, unsigned short mobtype, D3DXVECTOR3 position, FLOAT angle,
									unsigned char health, int defaultmotion );
	
	virtual	void			CheckInsideFrustum( void );

	virtual	void			Process( void );
	virtual	void			Render( void );	

	inline void				SetPosition( D3DXVECTOR3 position ){ m_Position = position; }
	inline void				SetAngle( FLOAT angle ){ m_RotateAngle = angle; }	

	virtual	void			ResetMobProcessTime( void );

	virtual	void			ResetAnimationState( void );
	virtual void			SetAnimation( int animationindex );
	virtual	void			SetAnimationIndex( int animationindex );
	virtual	void			SetBlowMotion(BOOL bCritical = false);

	virtual	void			UpdatePosition( void );

	virtual	void			CreateEffectManager( void );
	virtual	void			StartImpactEffect(void);
	virtual	void			SetImpactPosition(void);
	virtual	void			ResetImpactEffect(void);
	
	virtual	void			PlayAttackSound(void);
	virtual	void			SetCharacterMobAnimation( _XMOBMOTIONCLASS_TYPE motionclass, bool changemotion = true );//hotblood zerolevel
	
	BOOL					CheckAnimationPriority(int animationindex);
	
	void					ResetAttackTarget(void);
	void					MoveNextPostion(BOOL bInterpolate = FALSE);
	
	void					ProcessTargetMobDie(void);

	// ���� ȿ��
	void					InsertState(char type, short id, char spellstep = 0);
	void					DeleteState(char type, short id, char spellstep = 0);
	void					ReleaseState(void);

	// Condition
	void					InsertCondition(int index);
	void					DeleteCondition(int index);
};

// hotblood zerolevel
class _XZeroLevelMob : public _XMob
{
public:
	void			UpdatePosition( void );
	void			Render( void );	

};

struct DELETE_MOB_INFO
{
	unsigned short	usMobType;
	FLOAT			fTime;
};

typedef std::map<unsigned short, _XMob*>	smdef_XMob;

typedef std::list<DELETE_MOB_INFO>			sldef_DeleteMobType;

class _XMobList : public _XModelManager
{

private:
public:
	DWORD					m_LastPlaySoundTime;
	DWORD					m_ClientMobAIDelayTime;

	XMemoryPool<_XMob>		m_MobPool;
	smdef_XMob				m_smMobList;	

	sldef_DeleteMobType		m_slDeleteMotType;
	
public:

	_XMobList();
	~_XMobList();

	void	CreatePool(void);
	void	DeletePool(void);

private:
	_XMob*	NewMob();
	void	DeleteMob(_XMob* pMob);
public:

	void	disposeList(void);
	void	disposeList_zero(void);
	
	BOOL	AddReferenceModel(unsigned short usMobType) ;
	BOOL	AddReferenceModel_Precaching(unsigned short usMobType) ;
	void	ReleaseReferenceModel(unsigned short usMobType, BOOL bConstraint = FALSE);

	_XMob*	CreateMob(unsigned short usMobId, unsigned short usMobType, unsigned char ucMobWalkMode, 
								char cMobVitalRate,
								D3DXVECTOR3& vSrcPosition, D3DXVECTOR3& vDestPosition, bool bRebirth = false);
	
	_XMob*	CreateMobFromMove(unsigned short usMobId, unsigned short usMobType, unsigned char ucMobWalkMode,
								char cMobVitalRate, D3DXVECTOR3& vScrPosition, D3DXVECTOR3& vDestPosition);
			
private:
	// AddMob �� CreateMob �Լ� �ȿ����� ȣ�� �ǵ��� �Ѵ�.
	// Mob �� �����Ҵ�� �׻� CreateMob �� �̿��ؼ� ȣ�� �Ѵ�.
	BOOL	AddMob		( _XMob* mob	);
public:
	// zero level ������ Ư�� ó���� ���� �Լ�
	BOOL	AddMob_zero	( _XMob* mob	)		{ return AddMob(mob); }

	_XMob*  FindMob		( unsigned short mobid	);
	void	DeleteMob	( UINT mobid	);
	
	// Ŭ���̾�Ʈ ���͸� ������ �� Ŭ���̾�Ʈ ������ ���̵� ����.
	int		FindClientMobID	( void);
public:

	void    Process		( void );
	void	Process_ZeroLevel( void );	// hotblood zerolevel
	void	Process_VirtualMode( void );	// ������������� ���� ���μ����� ���� ó���Ѵ�.
	void	Render		( void );
	void	Render_ZeroLevel( void );	// hotblood zerolevel

	void    RemoveAttackTarget( _XUser* user );
	void	RemoveEffectInstance( int templateid );

	void	CheckInsideFrustum( void );
	void	MobAIProcess( void );//hotblood zerolevel
	void	ClientMobAIProcess( void );//Ŭ���̾�Ʈ���� ���͸� ������. Ʃ�丮�� ��忡�� ���ȴ�.
};


extern int						g_CurrentCachedMobcount;
extern _MOB_ATTRIBTABLE			g_MobAttribTable[_XDEF_MODELCLASSCOUNT_MOB];
extern _XMobMotionExistTable	g_MobMotionExistTable[_XDEF_MODELCLASSCOUNT_MOB];

#endif // !defined(_XMOB_H_)

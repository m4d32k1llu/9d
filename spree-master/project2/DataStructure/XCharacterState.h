// XCharacterState.h: interface for the _XCharacterState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCHARACTERSTATE_H__BBF56854_4252_425E_BFEE_914C7293E217__INCLUDED_)
#define AFX_XCHARACTERSTATE_H__BBF56854_4252_425E_BFEE_914C7293E217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#pragma warning (disable : 4786)
#include <map>
#include <list>
using namespace std;

// ȿ�� Ư�� Effect Property : 15�� 
#define _XCL_EFFECT_PROP_NONE				(0)
#define _XCL_EFFECT_PROP_GENERAL			(0x01)		// ���� �ð� ���� ���� : ����ġ
#define _XCL_EFFECT_PROP_INTERVAL			(0x01<<1)	// ���� �ð� ���� �ݺ� : �ߵ�
#define _XCL_EFFECT_PROP_INITIAL_ONCE		(0x01<<2)	// ���� ������ �ѹ� ����ǰ� ��
#define _XCL_EFFECT_PROP_ADDITIVE			(0x01<<3)	// ���ݽ� ���濡�� �߰��� ��
#define _XCL_EFFECT_PROP_REFLECTION			(0x01<<4)	// ���� �ݻ� ����
#define _XCL_EFFECT_PROP_ONLY_ONCE			(0x01<<5)	// �ѹ� �����ϸ� �����
#define _XCL_EFFECT_PROP_FEED				(0x01<<6)	// ���ݽ� �������� ���� ��������
// �������� ����

// Feed Ư���� �� ��� ���� ������
enum EN_EFFECT_FROM_TARGET
{
	en_eff_from_none = 0,					// �⺻ �Ӽ�
	en_eff_from_hit,						// �� ����������
	en_eff_from_life,						// ������ ü��
	en_eff_from_force,						// ������ ����
	en_eff_from_concentration				// ������ ����
};

// Effect Property
enum EN_EFFECT_VALUE_TYPE
{
	en_eff_vt_add_value = 0,				// value
	en_eff_vt_add_percent,					// percent
	en_eff_vt_sub_value,					// value
	en_eff_vt_sub_percent					// percent
};

// ��� �������� ��� Ÿ���� ��ȣ.
enum EN_EFFECT_WHERE_APPLY
{
	en_eff_where_none=0,					// 0 ���� �����ؼ� �ִ� 255...
	en_eff_where_attack_rate,				// ���� ����ġ : 'attack_rate'
	en_eff_where_avoid_rate,				// ȸ�� ����ġ : 'avoid_rate'
	en_eff_where_attack_damage,				// 3 ���ݷ�
	en_eff_where_defence, 					// 4 ����
	en_eff_where_defence_rate,				// 5 ��� Ȯ��
	en_eff_where_m_attack_rate,				// 6 ��� ���� ����ġ
	en_eff_where_m_avoid_rate,				// 7 ��� ȸ�� ����ġ
	en_eff_where_m_attack_damage,			// 8 ��� ���ݷ�
	en_eff_where_m_defence,					// 9 ��� ����
	en_eff_where_m_defence_rate,				// 10 ��� ��� Ȯ��
	
	en_eff_where_life,						// 11 �����
	en_eff_where_force,						// 12 ����
	en_eff_where_concentration,				// 13 ����
	en_eff_where_max_life,					// 14 �ִ� �����(ü��)
	en_eff_where_max_force,					// 15 �ִ� ����
	en_eff_where_max_concentration,			// 16 �ִ� ���� 
	en_eff_where_strength,					// 17		// �ٷ� ���ʽ�
	en_eff_where_zen,						// 18		// ����
	en_eff_where_intelligence,				// 19		// ����
	en_eff_where_constitution,				// 20		// �ǰ�
	
	en_eff_where_dexterity,					// 21		// ��ø
	en_eff_where_critical_rate,				// 22 ġ��Ÿ Ȯ�� ����.
	en_eff_where_cri_avoid_rate,			// 23 ġ��Ÿ ȸ�� Ȯ�� ����
	en_eff_where_ref_p_damage,				// 24 ���� ������ �ݻ�
	en_eff_where_ref_m_damage,				// 25 ��� ���ݷ� �ݻ�
	en_eff_where_ref_hon_damage,			// 26 ȥ�� ������ �ݻ�
	en_eff_where_ref_sin_damage,			// 27 ���� ������ �ݻ�
	
	en_eff_where_add_minus_damage,			// 28 ���� ������ �߰�
	en_eff_where_add_plus_damage,			// 29 ���� ������ �߰�
	en_eff_where_add_hon_damage,			// 30 ȥ�� ������ �߰�
	en_eff_where_add_sin_damage,			// 31 ���� ������ �߰�
	
	en_eff_where_wound_out,					// 32 �ܻ�
	en_eff_where_wound_in,					// 33 ����
	
	en_eff_where_regen_life,				// 34 �ڵ� ���� : ����
	en_eff_where_regen_force,				// 35 �ڵ� ���� : ����
	en_eff_where_regen_concentration,		// 36 �ڵ� ���� : ����
	
	en_eff_where_resist_minus,				// 37 ���� ���׷�
	en_eff_where_resist_plus,				// 38 ���� ���׷�
	en_eff_where_resist_hon,				// 39 ȥ�� ���׷�
	en_eff_where_resist_sin,				// 40 ���� ���׷�
	
	en_eff_where_dam_to_force,				// 41 ���� ���
	en_eff_where_dam_to_concentration,		// 42 ���� ���
	
	en_eff_where_give_life,					// 43 �ڽ��� ���� ����
	en_eff_where_give_force,				// 44 �ڽ��� ���� ����
	en_eff_where_trans_force_to_life,		// 45 �������� ������ �������� ��ȯ
	en_eff_where_trans_life_to_force,		// 46 �������� ������ �������� ��ȯ

	// 2005.01.07 �߰�

	// 2005.07.18 �̸� ����
	en_eff_where_incre_sk_mabi_prob,		// 47 ���� Ȯ�� ����

	en_eff_where_add_condition,				// 48 ���� �߰�
	en_eff_where_remove_condition,			// 49 ���� ����
	
	// 2005.01.18 �߰�
	en_eff_where_remove_spell,				// 50 ���� ����
	en_eff_where_move_speed,				// 51 �̵� �ӵ� % ���� / ����
	en_eff_where_attack_speed,				// 52 ���� �ӵ� % ���� / ����

	// 2005.03.18 �߰�
	en_eff_where_absolute_avoid,			// 53 ��-ȸ�� nȸ ȸ�� v apply_additive
	en_eff_where_absolute_reflect,			// 54 ��-�ݻ� nȸ �ݻ� v apply_additive
	en_eff_where_absolute_attack,			// 55 ��-����	v apply_additive
	en_eff_where_absolute_critical,			// 56 ��-ġ��	v apply_additive
	en_eff_where_feed_life, 				// 57 ������ ������ ���Ѵ´�.	v initial_once
	en_eff_where_feed_force,		 		// 58 ������ ������ ���Ѵ´�.	v initial_once
	en_eff_where_feed_concentration,		// 59 ������ ������ ���Ѵ´�.	v initial_once
	en_eff_where_ref_minus_damage,			// 60 ���� ������ �ݻ�	v reflect
	en_eff_where_ref_plus_damage,			// 61 ���� ������ �ݻ�	v reflect
	
	// 2005.04.27 �߰�
	en_eff_where_add_minus_spell_dam,		// 62 ���� ����� �߰� �������� �ش�. general, apply_additive
	en_eff_where_add_plus_spell_dam,		// 63 ���� ����� �߰� �������� �ش�. general, apply_additive
	en_eff_where_add_hon_spell_dam,			// 64 ȥ�� ����� �߰� �������� �ش�. general, apply_additive
	en_eff_where_add_sin_spell_dam,			// 65 ���� ����� �߰� �������� �ش�. general, apply_additive
	en_eff_where_shield_skill_atk,			// 66 ��� - ����. ������ n ��ŭ ���. apply_additive
	en_eff_where_shield_spell_atk,			// 67 ��� - ���. ������ n ��ŭ ���. apply_additive
	en_eff_where_shield_minus_atk,			// 68 ��� - ��. ������ n ��ŭ ���. apply_additive
	en_eff_where_shield_plus_atk,			// 69 ��� - ��. ������ n ��ŭ ���. apply_additive
	en_eff_where_shield_hon_atk,			// 70 ��� - ȥ. ������ n ��ŭ ���. apply_additive
	en_eff_where_shield_sin_atk,			// 71 ��� - ��. ������ n ��ŭ ���. apply_additive
	en_eff_where_shield_all_atk,			// 72 ��� - ��. ������ n ��ŭ �g��. apply_additive
	en_eff_where_all_stat,					// 73 �߿� ����/����
	
	// 2005.05.30 �߰�
	en_eff_where_weapon_dam,				// 74 ���� ������ ����/����

	// 2005.06.15 �߰�
	en_eff_where_rebirth_target,			// 75 ��Ȱ ��Ų��
	
	// 2005.07.12 �߰�
	en_eff_where_weapon_max_dam,			// 76 ���� �ִ� ������ ����
	en_eff_where_weapon_min_dam,			// 77 ���� �ּ� ������ ����
	en_eff_where_item_drop_percent,			// 78 ������ ��� Ȯ�� ����
	en_eff_where_balance,					// 79 ���� ����/����
	en_eff_where_cri_dam_bonus,				// 80 ġ��Ÿ ���ݷ� % ����
	en_eff_where_sk_target_modify,			// 81 ������ Ÿ�� Ÿ�� ����
	en_eff_where_sk_effectapply_modify,		// 82 ������ ���� Ÿ�� ����
	en_eff_where_sk_angle_modify,			// 83 ������ ���� �Ӽ� ����
	en_eff_where_sk_fera_modify,			// 84 ������ ���� �Ӽ� ����
	en_eff_where_sk_ferp_modify,			// 85 ������ ��� �Ӽ� ����
	en_eff_where_sk_distance,				// 86 ������ distance ����/����
	en_eff_where_sk_radius,					// 87 ������ radius ����/����
	en_eff_where_sk_min_dam,				// 88 ������ �ּ� ������ ����/����
	en_eff_where_sk_max_dam,				// 89 ������ �ִ� ������ ����/����
	en_eff_where_sk_attackrate,				// 90 ������ ����ġ ����/����
	en_eff_where_sk_avoidrate,				// 91 ������ ȸ�� ����/����
	en_eff_where_sk_costinner,				// 92 ������ �Ҹ� ���� ����
	en_eff_where_sk_costmind,				// 93 ������ �Ҹ� ���� ����
	en_eff_where_sk_finishspeed,			// 94 �ǴϽ� ���ǵ� ����
	en_eff_where_sk_castingtime,			// 95 ĳ���� ����
	en_eff_where_sk_delay,					// 96 ���� ������ ����
	en_eff_where_sk_cooldown,				// 97 ���� ��ٿ� ����

	// 2005.07.18 �̸� ����
	en_eff_where_sk_incre_effecttime,		// 98 ������ ����ð� ����

	// 2005.07.18 �߰�
	en_eff_where_sk_additive_1_time,		// 99 ������ ����ȿ�� 1 ���� �ð� ����
	en_eff_where_sk_additive_1_value,		// 100 ������ ����ȿ�� 1 �� ����
	en_eff_where_sk_additive_1_prob,		// 101 ������ ����ȿ�� 1 Ȯ�� ����
	en_eff_where_sk_additive_2_time,		// 102 ������ ����ȿ�� 2 ���� �ð� ����
	en_eff_where_sk_additive_2_value,		// 103 ������ ����ȿ�� 2 �� ����
	en_eff_where_sk_additive_2_prob,		// 104 ������ ����ȿ�� 2 Ȯ�� ����
	en_eff_where_sk_additive_3_time,		// 105 ������ ����ȿ�� 3 ���� �ð� ����
	en_eff_where_sk_additive_3_value,		// 106 ������ ����ȿ�� 3 �� ����
	en_eff_where_sk_additive_3_prob,		// 107 ������ ����ȿ�� 3 Ȯ�� ����
	en_eff_where_sk_additive_4_time,		// 108 ������ ����ȿ�� 4 ���� �ð� ����
	en_eff_where_sk_additive_4_value,		// 109 ������ ����ȿ�� 4 �� ����
	en_eff_where_sk_additive_4_prob,		// 110 ������ ����ȿ�� 4 Ȯ�� ����
	en_eff_where_incre_sk_mabi_time,		// 111 ���� ���� �ð� ����
	en_eff_where_incre_sp_mabi_prob,		// 112 ��� ���� Ȯ��
	en_eff_where_incre_sp_mabi_time,		// 113 ��� ���� �ð� 
	
	// 2006.01.17 �߰�
	en_eff_where_m_balance,					// 114  ��� �뷱��
	en_eff_where_m_critical_rate,			// 115  ��� ġ��Ÿ ������
	en_eff_where_m_cri_avoid_rate,			// 116	��� ġ��Ÿ ȸ����
	en_eff_where_p_critical_rate,			// 117	���� ġ��Ÿ ������
	en_eff_where_p_cri_avoid_rate,			// 118	���� ġ��Ÿ ȸ����
	en_eff_where_monster_taunt,				// 119	���� Ÿ��Ʈ
	en_eff_where_monster_detaunt,			// 120  ���� ��Ÿ��Ʈ
	en_eff_where_monster_hate,				// 121 	���� ����Ʈ
	en_eff_where_p_balance,					// 122	���� �뷱��
	en_eff_where_max_life_and_force,		// 123	�ִ� ����� ���� ���
	en_eff_where_disable_skill,				// 124 	Ư�� ���� ��� �Ұ�
	en_eff_where_disable_s_type,			// 125 	Ư�� s type ���� ��� �Ұ�
	en_eff_where_cancel_current,			// 126 	���� �������� ������ ��� ��Ų��
	en_eff_where_cancel_hyperrun,			// 127 	�������� ����� ��� ��Ų��
	en_eff_where_destroy_force,				// 128 	���� �ı�
	en_eff_where_resist_prob_for_mabi,		// 129 	���� ���� Ȯ��
	
	en_eff_where_delete_all_buff,			// 130	���� ���� : 2006/02/01 : initial
	en_eff_where_resistance_poison_pk,		// 131	�пյ� ���� ���� : 2006/02/27 : only apply & food - 5 
	en_eff_where_no_effect,					// 132 
	en_eff_where_fury,						// 133	�г� ��ġ ����. initial (�ش� ȿ�� �ߵ� �� �߰� n����Ʈ ȹ��), general (�г� ��ġ ȹ�� ��Ȳ �߻� �� n% �߰� ȹ��)
	en_eff_where_remove_buff,				// 134	debuff n��
	en_eff_where_remove_hostile,			// 135	dehostile n��
	en_eff_where_sk_feed_life,				// 136	������ ��� : feed
	en_eff_where_sk_feed_force,				// 137	������ ��� : feed
	en_eff_where_sk_feed_concentration,		// 138	������ ��� : feed
	en_eff_where_sp_feed_life,				// 139	����� ��� : feed
	en_eff_where_sp_feed_force,				// 140	����� ��� : feed
	en_eff_where_sp_feed_concentration,		// 141	����� ��� : feed

	en_eff_where_sr_safe,					// 142	���� ���� ����ȿ��.
	en_eff_where_light_crime,				// 143	����� ����ȿ��.
	en_eff_where_sr_heavy_crime,			// 144	�߹��� ����ȿ��.
	
	// 2007.07.19 �߰�
	en_eff_where_resist_all,				// 145	 ��� ���׷� ����
	
	// 2008.03.03 �߰�
	en_eff_resistance_battleroom_poison,	// 146	�� ���� ����
	en_reduce_rb_time,						// 147	��Ȱ �޿��� �ð� ���� : general/additive
	en_reduce_rb_exp,						// 148	��Ȱ �޿��� ����ġ�϶�% ���� : general/additive
	en_protect_rate,						// 149	���� ���� Ȯ���� % ������ ���� : general/additive
	en_dam_inc_for_human_type,				// 150	�ΰ��� ���Ϳ��� ���ݷ��� %������ ���� : general/additive
	en_dam_inc_for_animal_type,				// 151  ������ ���Ϳ��� ���ݷ��� %������ ���� : general/additive
	en_dam_inc_at_day,						// 152	�� �ð��� ���ݷ��� %������ ���� : general/additive
	en_exp_inc_at_day,						// 153	�� �ð��� ����ġ�� %������ ���� : general/additive
	en_dam_inc_at_night,					// 154	�� �ð��� ���ݷ��� %������ ���� : general/additive
	en_exp_inc_at_night,					// 155	�� �ð��� ����ġ�� %������ ���� : general/additive

} ;

// =====================================
// character state(effect) table =======
struct _XCharacterStateTable
{
	short			id;

#ifdef _XDEF_NEWSTRINGMANAGER	
	LPTSTR			name;
	LPTSTR			concept;
#else
	TCHAR			name[64];
	TCHAR			concept[128];
#endif

	char			type;					// 0 : buff, 1 : hostile
	unsigned char	iconNumber;
	unsigned char	effectNumber;	
	
	// from server script
	unsigned short	effect_prop;			// �Ӽ� - property
	char			value_type;				// ���� ���� ����ϴ� ���
	unsigned char	where;					// ���� �� ��
	unsigned char	from;					// ���濡�� ������ ��
	unsigned char	condition_flag_id;
	char			count;
	
	short			prob;
	short			value;
	short			additiveCount;
};
extern map <int, _XCharacterStateTable*>	g_CharacterStateTable;

// =====================================
// character condition table ===========
struct _XCharacterCondition
{
	short			id;
	
#ifdef _XDEF_NEWSTRINGMANAGER
	LPTSTR			name;
#else
	TCHAR			name[16];
#endif

	int				visualEffect[5];

	unsigned short	movelock : 1;
	unsigned short	tradelock : 1;
	unsigned short	battlelock : 1;
	unsigned short	medlock : 1;
	unsigned short	spelllock : 1;
	unsigned short	speedlock : 1;
	unsigned short	modelock : 1;
	unsigned short	skilllock : 1;
	unsigned short	chattinglock : 1;
	unsigned short	partylock : 1;
	unsigned short	reserved : 6;

	short			soundprop[5];
	short			soundindex[5];

#ifdef _XDEF_NEWSTRINGMANAGER
	LPTSTR			concept;
#else
	TCHAR			concept[128];
#endif
};
extern _XCharacterCondition g_CharacterConditionTable[32];

// =====================================
// character state	====================
class _XCharacterState  
{
public :

	char				m_FromType;			// 0:skill, 1:condition, 2:item

	short				m_sSkillID;
	char				m_cStep;

	char				m_cSlotIndex;

	short				m_sEffectID;
	short				m_sValue;
	
	unsigned short		m_usLeftTimeSec;

	DWORD				m_dwStateStartTime;	
	bool				m_bDeleteReqPacket;
	

public:
	_XCharacterState();
	virtual ~_XCharacterState();

};

enum
{
	en_from_skill = 0,
	en_from_effect,
	en_from_extra,
	en_from_passive,
	en_from_nickname,
	en_from_extraskill,		// ���� ���� ����
	en_from_specialbuff		// �����/���Ű������ ���� ���
};

// =====================================
// character state list	================
class _XCharacterStateList
{
public :
	map <int, _XCharacterState*>	m_mapCharacterStateList;
	int								m_TotalListCount;

public :
	_XCharacterStateList();
	virtual ~_XCharacterStateList();

	// fromtype -  0:skill  1:effect  2:item
	void					InsertState(unsigned short uid, short id, unsigned short lefttime, char fromtype, short val = 0, short skillid = 0, char step = 0);
	void					DeleteState(unsigned short uid, char fromtype, short effectid, short skillid = 0);
	
	void					InsertStateFromSpell(short skillid, char skillstep, unsigned short lefttime, int effectindex);
	void					UpdateStateFromSpell(short skillid, char skillstep, unsigned short lefttime, int effectindex);
	void					DeleteStateFromSpell(short skillid, char skillstep, int effectindex);

	void					InsertStateFromEffect(unsigned short uid, short effectid, unsigned short lefttime, short value);
	void					UpdateStateFromEffect(unsigned short uid, short effectid, unsigned short lefttime, short value);
	void					DeleteStateFromEffect(unsigned short uid, short effectid);

	void					InsertStateFromExtra(unsigned short uid, short effectid, char slotindex, unsigned short lefttime, short value);
	void					UpdateStateFromExtra(unsigned short uid, short effectid, char slotindex, unsigned short lefttime, short value);
	void					DeleteStateFromExtra(unsigned short uid, short effectid, char slotindex);

	void					InsertStateFromPassive(short skillid, char skillstep, int effectindex);
	void					DeleteStateFromPassive(short skillid, char skillstep, int effectindex);

	// ��ȣ ȿ�� 
	void					InsertStateFromNicknameEffect(short effectid, short value );
	void					UpdateStateFromNicknameEffect(short effectid, short value );
	void					DeleteStateFromNicknameEffect(void);

	// extra slot�̸鼭 skill id�� ����ϴ� ��� - ���� ���� ���� ���� ��
	void					InsertStateFromExtraSkill(unsigned short uid, short skillid, char skillstep, char slotindex, unsigned short lefttime, int effectindex);
	void					UpdateStateFromExtraSkill(unsigned short uid, short skillid, char skillstep, char slotindex, unsigned short lefttime, int effectindex);
	void					DeleteStateFromExtraSkill(unsigned short uid, short skillid, char skillstep, int effectindex ,char slotindex);

	// ��������� ����ϴ� ����
	void					InsertStateFromSpecialBuff(unsigned short uid, short skillid, char skillstep, char slotindex, unsigned short lefttime = 0, int effectindex = 0);
	void					UpdateStateFromSpecialBuff(unsigned short uid, short skillid, char skillstep, char slotindex, unsigned short lefttime = 0, int effectindex = 0);
	void					DeleteStateFromSpecialBuff(unsigned short uid, short skillid, char skillstep, int effectindex, char slotindex);

	int						FindState(char fromtype, short effectid, short skillid, char slotindex = 0);
	int						FindStateExtraSkill(char fromtype, char slotindex);
	int						FindStateSpecialBuff(char slotindex, int& skillid, int& skilllevel);

	void					Release(void);

	static BOOL				LoadCharacterStateTable(void);
	static void				ReleaseCharacterStateTable(void);
	static BOOL				ReloadCharacterStateTable(void);

	static BOOL				LoadEffectScriptServerFormat(_XPackageArchive& scriptpackage);
	static BOOL				GetCommand(char* cmd, FILE* fileptr, bool bnumber = false);
	static BOOL				ParseDescriptor(FILE* fileptr);
	static BOOL				ParseEffectScript(FILE* fileptr);
	
	static void				InsertEffectScriptName(char* eff_name, short eff_where);
	static void				ReleaseEffectScriptName(void);
	static unsigned char	GetEffectWhereFromName(char* eff_name);
	static unsigned char	GetEffectProperty(char* property);
	static unsigned char	GetEffectFromID(char* from);
	static char				GetEffectValueType(char* value);

	static BOOL				LoadCharacterConditionTable(_XPackageArchive& scriptpackage);
	
	void		ProcessState(void);
	void		UpdateState(unsigned short uid, short id, unsigned short lefttime, char fromtype, short val = 0, short skillid = 0, char step = 0);

//	void		AddResult(short id, short val);
//	void		DeleteResult(short id);

	void		DeleteAllBuff(void);
	void		DeleteAllHostile(void);

	void		DeleteAllEffect(unsigned short uid);
	void		DeleteAllSkill(unsigned short uid);
	
	void		DeleteAll(void);		// ��� ����ȿ�� ����, Result, Effect, icon ��� ����

	// Condition
	void		InsertConditionState(unsigned short uid, int index);
	void		DeleteConditionState(unsigned short uid, int index);
	BOOL		FindConditionState(unsigned short uid, int index);
	
	// ����ȿ�� Renewal
	void		ApplyBonusPoint(void);
	void		ApplyBonusAdditional(void);
	void		ApplyFinalValue(void);

	void		GetCharacterBaseValue(void);

	void		AddStrengthBonus(short id, short val);
	void		AddStrengthBonus(short percentage);

	void		AddZenBonus(short id, short val);
	void		AddZenBonus(short percentage);

	void		AddIntBonus(short id, short val);
	void		AddIntBonus(short percentage);

	void		AddConstitutionBonus(short id, short val);
	void		AddConstitutionBonus(short percentage);

	void		AddDexBonus(short id, short val);
	void		AddDexBonus(short percentage);

	void		AddPAttackRateBonus(short id, short val);
	void		AddPAttackRateBonus(short percentage);

	void		AddPAvoidRateBonus(short id, short val);
	void		AddPAvoidRateBonus(short percentage);

	void		AddPAttackDamageBonus(short id, short val);
	void		AddPAttackDamageBonus(short percentage);

	void		AddDefenceBonus(short id, short val);
	void		AddDefenceBonus(short percentage);

	void		AddMAttackRateBonus(short id, short val);
	void		AddMAttackRateBonus(short percentage);

	void		AddMDefenceBonus(short id, short val);
	void		AddMDefenceBonus(short percentage);

	void		AddMAvoidRateBonus(short id, short val);
	void		AddMAvoidRateBonus(short percentage);

	void		AddMAttackDamageBonus(short id, short val);
	void		AddMAttackDamageBonus(short percentage);

	void		AddMaxLifeBonus(short id, short val);
	void		AddMaxLifeBonus(short percentage);

	void		AddMaxForceBonus(short id, short val);
	void		AddMaxForceBonus(short percentage);

	void		AddMaxConcentrationBonus(short id, short val);
	void		AddMaxConcentrationBonus(short percentage);

	void		AddMoveSpeedBonus(short id, short val);
	void		AddMoveSpeedBonus(short percentage);
	
	void		AddWeaponDamage(short id, short val);
	void		AddWeaponDamage(short percentage);

	void		AddWeaponMaxDamage(short id, short val);
	void		AddWeaponMaxDamage(short percentage);

	void		AddWeaponMinDamage(short id, short val);
	void		AddWeaponMinDamage(short percentage);

	void		AddDisableSkill(short id, short val);
	void		AddDisableSType(short id, short val);
	
	void		AddResistMinus(short id, short val);
	void		AddResistMinus(short percentage);

	void		AddResistPlus(short id, short val);
	void		AddResistPlus(short percentage);

	void		AddResistHon(short id, short val);
	void		AddResistHon(short percentage);

	void		AddResistSin(short id, short val);
	void		AddResistSin(short percentage);

	void		AddResistAll(short id, short val);
	void		AddResistAll(short percentage);

	void		AddAttackDamageMinus(short id, short val);
	void		AddAttackDamageMinus(short percentage);

	void		AddAttackDamagePlus(short id, short val);
	void		AddAttackDamagePlus(short percentage);

	void		AddAttackDamageHon(short id, short val);
	void		AddAttackDamageHon(short percentage);

	void		AddAttackDamageSin(short id, short val);
	void		AddAttackDamageSin(short percentage);

	void		GetBaseBonusFromMastery(void);
	void		GetAdditionalBonusFromMastery(void);
	void		GetSkillModifiedFromMastery(void);

	void		GetAdditionalBonusFromItem(void);
	void		GetBaseBonusFromItem(void);

	// ���� - ���� ��ȭ
	void		ModifySkillTargetType(short skillid, char point, short id, short value);
	void		ModifySkillDistance(short skillid, char point, short id, short value);
	void		ModifySkillMinDamage(short skillid, char point, short id, short value);
	void		ModifySkillMaxDamage(short skillid, char point, short id, short value);
	void		ModifySkillAttackRate(short skillid, char point, short id, short value);
	void		ModifySkillAvoidRate(short skillid, char point, short id, short value);
	void		ModifySkillCostInner(short skillid, char point, short id, short value);
	void		ModifySkillCostMind(short skillid, char point, short id, short value);
	void		ModifySkillFinishSpeed(short skillid, char point, short id, short value);
	void		ModifySkillCasting(short skillid, char point, short id, short value);
	void		ModifySkillDelay(short skillid, char point, short id, short value);
	void		ModifySkillCooldown(short skillid, char point, short id, short value);
	void		ModifySkillIncreEffectTime(short skillid, char point, short id, short value);
	void		ModifySkillAdditiveValue(short skillid, char point, int index, short id, short value);
	void		ModifySkillAdditiveProb(short skillid, char point, int index, short id, short value);
	void		ModifySkillAdditiveTime(short skillid, char point, int index, short id, short value);

};

#endif // !defined(AFX_XCHARACTERSTATE_H__BBF56854_4252_425E_BFEE_914C7293E217__INCLUDED_)

#pragma once

#define _PD_BUFF_NPC_CHECK_DIST_				// 20080901 : ������ NPC ���� �Ÿ� üũ.

#define _PD_FREEMATCH_ABUSING_					// 20080825 : ������ȸ ���¡ ���� - ó�� ������, ������ȸ ���� ���·� �����ϰ�, �պ񿡼� �����ų�, �׸��α� ������ ����.

#define _PD_MASTER_PUPIL_						// 20080818 : ���� �ý��� �߰�.

#define _PD_LVPK_REGION_ATTACK_MOD_				// 20080720 : �������� ��� ������ �ɸ�.
#define _PD_CASTLE_MOB_ATTACKED_ONLY_ATTACKER_	// 20080720 : ���������, ������ ����, ������ ������ ���Ϳ� ������� �ʵ��� �Ѵ�. ( _PD_CASTLE_STEP_1_ ��  ���� ������ �� �� )
//#define _PD_CHARAC_LTS_REGION_ATTACK_EXC_		// 20080716 : ������ ����, LVPK ����Ʈ�� ĳ���Ͱ����� ���, ������ ���� �ʵ��� �Ѵ�. _PD_CHARAC_LTS_ 
//#define _PD_LEVEL_RANK_OVERLAP_					// 20080710 : ����/����ġ�� ���� ���, ������ �ߺ��� �� �ֵ��� ����.

#define _PD_COMMON_RANK_						// 20080702 : ���� ��ũ �߰�.( CommonRank.txt )

#define _PD_BUGFIX_20080630_					// 20080630 : 7/3�� ��ġ�� ������ ����. ����ȿ�� ���� ����ȭ, ���� �����ð� üũ.
#define _PD_BUGFIX_20080616_					// 20080610 : 6/26 �� ��ġ�� ������ ����. : �ٸ� ����� ������ �� ��, ���� �� ��.

#define _PD_SKILL_EXTEND_						// 20080522 : ��ų ������ ���� Ȯ��(80��). DB/DS/GS ExtendSkillSlot.doc ����.
//#define _PD_SKILL_EXTEND_100_					// 20080520 : ��ų ������ ������ 100 ������ Ȯ��. GS �� ������Ʈ

#define _PD_QUEST_FOR_DUNGEON_					// 20080520

#define _PD_ITEM_BONUS_SPELL_APPLY_				// 20080506 : ������/ĳ������ ���ʽ� Apply ��, ������� ������� �ʴ� �� ����. : ���� ������, ���ش�.
#define _PD_MOB_M_DEFENCE_FIX_					// 20080506 : ���� ����/��� ��� ��� ���� ���� + ĳ���� ��� ��� ��� ����. : ���� ������, ���ش�.

#define _PD_CASH_INVEN_							// 20080522 : ĳ�� �κ� �� ǥ�� Ȯ�� : DS/GS/GMS/Client : CashInventory.doc

#define _PD_CLOTH_VIEW_SELECT_					// 20080406 : �ǻ� ���⸦ ġ���ǻ�� �Ϲ��ǻ� �߿� ������ �� �ֵ��� �Ѵ�. : client

#define _PD_ADD_EVENT_COLLECTION_				// 20080327 : DS/GS/GMS ���� ����Ÿ ��������, �̺�Ʈ�� ������ 1000 �� ���� ����ϵ���, Ȯ��.


#define _PD_DELAY_BOX_SPAWN_					// 20080508 : �� - �п�ȭ���� ������ �����屺 ���ķ� �̷��.
#define _PD_PKEVENT_BOX_SPAWN_					// 20080306 : �����п�ȭ���� ������ �����屺 ���� �������� �ű��. : DB/DS/GS & Client

#define _PD_SECOND_JOB_							// 20080324 : 2�� ���� - ����ȿ���� Ŭ���� ����.
#define _PD_ITEM_V25_							// 20080324 : ������ ���̺� 2.5 ����. �ǻ�� �������� CharacGrade �߰�.
#define _MASTERY_SECOND_PATCH_					// 20080324 : 2�� ���� ����ȿ�� ����.

#define _PD_SPELL_WEAPON_PENALTY_				// 20080324 : ������ �г�Ƽ ���� -- ����->��ø ����.

#define _PD_SETITEM_							// 20080226 : ��Ʈ ������ ����.
#define _PD_MOB_APPLYEFFECT_MOD_				// 20080212 : ���׿��� ����Ǵ� ����ȿ�� ��, Ư�� ����ȿ���� �ٸ� ������ �ٲ��.

#define _PD_COLLECT_PACKET_MODIFY_				// 20080324 : ���� ��Ŷ�� �ʿ� ���� �������� ���� �߻� -> �ʿ� ���� �߰�

#define _PD_EXPBONUS_SAVE_
//#define _PD_QUEST_MAX_600_					// 20071206 ����Ʈ ���� Ȯ�� : 300 => 600 ���� Ȯ��.

#define _PD_CASTLE_RENEWAL_EXP_					// 20071226 ����� ������ - ���ʽ� ����ġ �ֱ�.
#define _PD_CASTLE_RENEWAL_1_					// 20071226 ����� ������ - �����ٸ�, ���� �߰�.

#define _PD_GAME_EVENT_							// 20071226 Game Event(DB/DS/GS) : Free Match 

#define _PD_ITEM_PM_COMMON_1024					// 20071126 : �������� ����/����/��ȸ���� ����/����� ��� �����ϰ�, ���� ����.

#define _PD_MONSTER_REF_1_5_					// 20071126 : monster ref 1.5
#define _PD_ITEM_RENEWAL_1008_					// 20071126 : ���� �ϻ��� ������ : ���� ����
//#define _PD_QUEST_SCRIPT_VER_1_1_				// ����Ʈ ��ũ��Ʈ ���� 1.1 ������Ʈ

#define _PD_GAMBLE_FIX_0821_					// �׺� ����.
#define _PD_DS_COM_TIMEOUT_						// DS ���� ��ſ��� Ÿ�� �ƿ� üũ. : DS 

#define _PD_SEPARATE_SOCKET_					// 20070710 ���� �и� ��� �߰�.

#define _PD_ITEM_V24_SOCKET_					// 20071126 ������ v2.4 ����� ���� ���� ���� �˻� �߰�. _PD_SEPARATE_SOCKET_ ���� ������ ��.
//	#define _ORG_PROMOTION_							// 2007.07.11 ������ �±� 08/30 ������Ʈ  - YunTaek
#define _PD_LEVEL_RATE_SAVE_					// ������ ����(0-1 ����)�� �����Ͽ�, ���� �÷��װ� ������, ������ �����ϰ� �Ѵ�. DS/GS
#define _PD_4_EFFECT_ADD_						// ����ȥ�� ����/���׷� �߰�. : ���ݷ�/�ݻ�/���/���׷� : ��ũ��Ʈ(penalty_of_zone.txt)

#define _PD_RECALL_FRIEND_						// ģ����ȯ 20070604 ����. : DS/GS
#define _PD_LOGDB_ACCOUNT_LENGTH_MODIFY_		// 20070601 �α� ���� ��Ŷ ����.

#define _PD_WEAPON_PROTECT_						// �븰/��� �ý���. ���� : 20070420

#define _PD_CASTLE_STEP_2_						// ����� : 2�ܰ� ����/����/1�����׼��� : 20070403

#define _PD_RETURN_LOBBY						// 20070329 �߰�. 20080903 15�� ���Ŀ��� DS �� �Բ� ������ ��.

#define _PD_CHARAC_LTS_							// LTS IJA. ������ �ϳ��� �ٿ��. : DS/DB( GetCharacUID() )
#define _PD_PARTY_STATE_ADD_					// 20070321 ������ ���� ��ȭ �ڸ���� : DS 

//#define CHARAC_CREATE_DATE_20070309
#define _PD_EXPBONUS_SAVE_
#define _PD_SKILL_ADD_FROM_MASTERY_				// ���ǿ� ���� ��ų �߰�.

#define _PD_PVP_EFFECT_MODIFY_					// ȿ���� ����, PVP ���� ȿ���� �ٸ��� ����.
#define _PD_CASTLE_STEP_1_						// ����� : 1�ܰ� �����ٷ�, ���, ����� �⺻. ���� : 20070124
												// _PD_ALLY_ORG_STEP_1_ ���Ŀ� �������� ��.
//#define ITEMSHOP_PATCH_200607					// ItemShop : ���Ŀ� 2006/08/17 �ø�. by Ray.
#define _PD_SAFEZONE_TEST_MODE_					// �������� �������� �ð��� ª���Ѵ�.

#define	_MASTERY_NEED_PAGE_POINT_				// �ʿ� ����Ʈ ����κ�

#define _PD_MONSTER_SKILL_DAM_					// ������ ��ų ���ݷ� �� ����ġ ����. 2006/08/18 Ray.

#define _PD_DEBUG_ON_
//#define _PD_ACCOUNT_PLAY_TIME_					// 
#define _PD_EXP_NOTICE_

#define _PD_CHECK_ATK_IN_CHANNEL_				// 20071101 ä�θ� �߿� ������ ����, ���´�.
#define _PD_CHECK_ATKPACKET_DISTANCE_			// 20071012 : �Ÿ� üũ�ϱ�. üũ�Ǹ�, ������ ���еȴ�.


//////////////////////////////////////////////////////////////////////////
// ��ŷ ����
//////////////////////////////////////////////////////////////////////////
#define _PD_SKIP_CHECK_							// 20080820 : ��Ŷ �������� �ȸ¾Ƶ� �Ѿ��.
#define _PD_CRYPTO_GETKEY_MOD_					// 20080721
#define _PD_RECV_HACKPROGRAM_LIST_				// 20080721
#define _PD_CHECK_VERSION_BY_CHANNEL_			// 20080721
#define _PD_PACKET_CRYPTO_CHECK_				// 20080721 : ��Ŷ���� ��ȣȭ�� üũ�Ѵ�.
//////////////////////////////////////////////////////////////////////////



#define	_NXPCB_AWARD_200703_					// �ؽ� �Ǿ��� ����
//#define _NXPCB_TEMPORALY_BLOCK_AWARD_

#define _GM_CTRL_ON_
#define _PD_KILL_GHOST_							// ����� ���� ����, ���� ���� �ð��� �ø���. 

//#define	_CIS_20070326_		// �����۸�
//#define	_CIS_20070326_LOG_

//////////////////////////////////////////////////////////////////////////
// Monster Lock
//////////////////////////////////////////////////////////////////////////
#define _PD_CHECK_MOBLOCK_
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Game Guard
//////////////////////////////////////////////////////////////////////////
//#define _PD_USE_GAMEGUARD_						// 20080311 : ���� ���� ���� ���.
//#define _PD_GAMEGUARD_VER_1_					// 20080311 : ���Ӱ��� ���� ���� 1 ���. : 2008/02/28 ����. ������ ��, lib ��ü. ������. Dll ����.
//#define _PD_GAMEGUARD_VER_2_					// 20080311 : ���Ӱ��� ���� ���� 2 ���. : 2008/02/28 ����. ������ ��, lib ��ü. ������. Dll ����.


//////////////////////////////////////////////////////////////////////////
// Event 
//////////////////////////////////////////////////////////////////////////
//#define _PD_LEVEL_PRESENT_						// 20080324 : ���� �޼� ���� �ֱ�.
#define _PD_EVENT_INVITE_FRIEND_				// 20080406 : ģ�� �ʴ� �̺�Ʈ(�̱�)
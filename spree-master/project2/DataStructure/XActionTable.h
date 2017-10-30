#ifndef		_XACTIONTABLE_DEFINE
#define		_XACTIONTABLE_DEFINE

///////////////////////////////////////////////////////////////////////
// Character action number...

typedef enum _XCHARACTER_MOTION
{
	////////////////////////////////////////////////////////////////////////////////
	// ��ȭ��� �⺻ ����
	_XAN_IDLE_NONE		=	0,				//	0 �Ǽ� ���	
	_XAN_IDLE_NONE_ACTION,					//	1 �Ǽ� ��� �߰� �׼� 
	
	_XAN_IDLE_SHORTWEAPON,					//	2 �ܺ��� ���	
	_XAN_IDLE_SHORTWEAPON_ACTION,			//	3 �ܺ��� ��� �߰� �׼� 

	_XAN_IDLE_LONGWEAPON,					//	4 �庴�� ���	
	_XAN_IDLE_LONGWEAPON_ACTION,			//	5 �庴�� ��� �߰� �׼� 	

	////////////////////////////////////////////////////////////////////////////////
	// �Ǽ�	
	_XAN_MOVE_NONE_START,				    //	6 �Ǽ� �̵�	����	
	_XAN_MOVE_NONE_REPEAT,				    //	7 �Ǽ� �̵�	�ݺ�
	_XAN_MOVE_NONE_END,						//	8 �Ǽ� �̵�	��
	
	_XAN_FASTMOVE_NONE_START,				//	9 �Ǽ� ����	����	
	_XAN_FASTMOVE_NONE_REPEAT,				// 10 �Ǽ� ����	�ݺ�
	_XAN_FASTMOVE_NONE_END,					// 11 �Ǽ� ����	��

	////////////////////////////////////////////////////////////////////////////////
	// �ܺ���	
	_XAN_MOVE_SHORTWEAPON_START,			// 12 �ܺ��� �̵�	����	
	_XAN_MOVE_SHORTWEAPON_REPEAT,			// 13 �ܺ��� �̵�	�ݺ�
	_XAN_MOVE_SHORTWEAPON_END,				// 14 �ܺ��� �̵�	��
		
	_XAN_FASTMOVE_SHORTWEAPON_START,		// 15 �ܺ��� ����	����	
	_XAN_FASTMOVE_SHORTWEAPON_REPEAT,		// 16 �ܺ��� ����	�ݺ�
	_XAN_FASTMOVE_SHORTWEAPON_END,			// 17 �ܺ��� ����	��

	////////////////////////////////////////////////////////////////////////////////
	// �庴��	
	_XAN_MOVE_LONGWEAPON_START,				// 18 �庴�� �̵�	����	
	_XAN_MOVE_LONGWEAPON_REPEAT,			// 19 �庴�� �̵�	�ݺ�
	_XAN_MOVE_LONGWEAPON_END,				// 20 �庴�� �̵�	��
		
	_XAN_FASTMOVE_LONGWEAPON_START,			// 21 �庴�� ����	����	
	_XAN_FASTMOVE_LONGWEAPON_REPEAT,		// 22 �庴�� ����	�ݺ�
	_XAN_FASTMOVE_LONGWEAPON_END,			// 23 �庴�� ����	��

	////////////////////////////////////////////////////////////////////////////////
	// ������	
	_XAN_MOVE_SOFTWEAPON_START,				// 24 ������ �̵�	����	
	_XAN_MOVE_SOFTWEAPON_REPEAT,			// 25 ������ �̵�	�ݺ�
	_XAN_MOVE_SOFTWEAPON_END,				// 26 ������ �̵�	��
		
	_XAN_FASTMOVE_SOFTWEAPON_START,			// 27 ������ ����	����	
	_XAN_FASTMOVE_SOFTWEAPON_REPEAT,		// 28 ������ ����	�ݺ�
	_XAN_FASTMOVE_SOFTWEAPON_END,			// 29 ������ ����	��
	
	////////////////////////////////////////////////////////////////////////////////
	// ���� ��� �⺻ ����
	_XAN_BATTLEIDLE_NONE,					// 30 �Ǽ� ���
	_XAN_BATTLEIDLE_SHORTWEAPON,			// 31 �ܺ��� ���
	_XAN_BATTLEIDLE_LONGWEAPON,				// 32 �庴�� ���
	
	////////////////////////////////////////////////////////////////////////////////
	// ���� �Ϲ� ����
	_XAN_NI_ATTACK_KK,						// 33 �ǰ���	
	_XAN_NI_ATTACK_SHORTWEAPON,				// 34 �ܺ���	
	_XAN_NI_ATTACK_LONGWEAPON,				// 35 �庴��
	_XAN_NI_ATTACK_SOFTWEAPON,				// 36 ������
	_XAN_NI_ATTACK_HIDDENWEAPON,			// 37 �ϱ�
	_XAN_NI_ATTACK_THROWWEAPON,				// 38 ������

	////////////////////////////////////////////////////////////////////////////////
	// ���
	_XAN_DEFENCE_NONE,						//  39 �Ǽ� ����
	_XAN_DEFENCE_SHORTWEAPON,				//  40 �ܺ��� ����
	_XAN_DEFENCE_LONGWEAPON,				//  41 �庴�� ����
	_XAN_DEFENCE_SOFTWEAPON,				//  42 ������ ����

	////////////////////////////////////////////////////////////////////////////////
	// ���±�
	_XAN_BLOW_NONE,							//  43 �Ǽ�   ���±�
	_XAN_BLOW_SHORTWEAPON,					//  44 �ܺ��� ���±�
	_XAN_BLOW_LONGWEAPON,					//  45 �庴�� ���±�	
	_XAN_BLOW_SOFTWEAPON,					//  46 ������ ���±�	

	////////////////////////////////////////////////////////////////////////////////
	// ���� ó��
	_XAN_DYINGREADY_NONE,						//  47 �Ǽ� �и���
	_XAN_DYINGREADY_SHORTWEAPON,				//  48 �ܺ��� �и���
	_XAN_DYINGREADY_LONGWEAPON,					//  49 �庴�� �и���
	
	_XAN_DYINGREPEAT_NONE,						//  50 �Ǽ� ��ȣ��
	_XAN_DYINGREPEAT_SHORTWEAPON,				//  51 �ܺ��� ��ȣ��
	_XAN_DYINGREPEAT_LONGWEAPON,				//  52 �庴�� ��ȣ��

	////////////////////////////////////////////////////////////////////////////////
	// ��ġ��
	_XAN_HITBACK_NONE,						//  53 �Ǽ� ���� ġ��
	_XAN_HITBACK_SHORTWEAPON,				//  54 �ܺ��� ��� ���� ġ��
	_XAN_HITBACK_LONGWEAPON,				//  55 �庴�� ��� ���� ġ��

	////////////////////////////////////////////////////////////////////////////////
	// �����
	_XAN_WAKEUP_NONE,						//  56 �Ǽ�   �����
	_XAN_WAKEUP_SHORTWEAPON,				//  57 �ܺ��� �����
	_XAN_WAKEUP_LONGWEAPON,					//  58 �庴�� �����
	
	////////////////////////////////////////////////////////////////////////////////
	// ��������
	_XAN_DIE,								//  59 �Ǽ� �ױ�

	////////////////////////////////////////////////////////////////////////////////
	// ���� ǥ��
	_XAN_EMOTION_BOW,						// 60 ������ �λ�
	_XAN_EMOTION_BOWPOLITELY,				// 61 ������ �λ�	
	_XAN_EMOTION_EXCITEMENT,				// 62 ����
	_XAN_EMOTION_RUSH,						// 63 ����
	
	_XAN_EMOTION_CRINGESTART,				// 64 �����ݱ� ����
	_XAN_EMOTION_CRINGEREPEAT,				// 65 �����ݱ� �ݺ�
	_XAN_EMOTION_CRINGEEND,					// 66 �����ݱ� ��

	_XAN_EMOTION_CHEER,						// 67 ȯȣ
	_XAN_EMOTION_BREAKDOWN,					// 68 ����
	_XAN_EMOTION_CRY,						// 69 ���
	_XAN_EMOTION_LAUGH,						// 70 ����	
	_XAN_EMOTION_ANGRY,						// 71 ȭ��
	_XAN_EMOTION_DOZ,						// 72 ����
	
	_XAN_EMOTION_SITDOWNSTART,				// 73 �ɱ� ����
	_XAN_EMOTION_SITDOWNREPEAT,				// 74 �ɱ� �ݺ�
	_XAN_EMOTION_SITDOWNEND,				// 75 �ɱ� �� 

	_XAN_EMOTION_MARRYCLAPPINGHAND,			// 76 �ڼ�	
	_XAN_EMOTION_MARRYBOX,					// 77 �λ�
	_XAN_EMOTION_MARRYBOWPOLITELY,			// 78 ū��

	_XAN_EMOTION_CLAPPINGHAND,				// 79 �ڼ�
	
	//////////////////////////////////////////////////////////////////////
	// ���� ���� �߰�
	_XAN_NI_ATTACK_AXE,						// 80 ���� ���� �ι�

	//////////////////////////////////////////////////////////////////////
	// �������� �߰�
	_XAN_DIE_SHORTWEAPON,					//  81 �ܺ��� �ױ�

	//////////////////////////////////////////////////////////////////////
	// ���� ǥ�� �߰�
	_XAN_EMOTION_ANIMATION,					//	82 ��������

	//////////////////////////////////////////////////////////////////////
	// �������� �߰�
	_XAN_DIE_LONGWEAPON,					// 83  �庴�� �ױ�
	
	//////////////////////////////////////////////////////////////////////
	// Damaged action

	_XAN_MEDITATIONDAMAGE,					// 84  ������� ������

	_XAN_COMASTATEWALK,						// 85  ��ȭ�Ը� �ȱ�

	_XAN_COMASTATEIDLE,						// 86  ��ȭ�Ը� ���
	
	//////////////////////////////////////////////////////////////////////
	// LPS Action

	_XAN_LPS_HANDTOUCH,						// 87  �Ǽ� �ǵ帮��

	_XAN_LPS_SHORTWEAPON_HANDTOUCH,			// 88  �ܺ��� �ǵ帮��

	_XAN_LPS_LONGWEAPON_HANDTOUCH,			// 89  �庴�� �ǵ帮��

	_XAN_LPS_FOOTTOUCH,						// 90  �Ǽ� �߷� �ǵ帮��
	
	_XAN_LPS_SHORTWEAPON_FOOTTOUCH,			// 91  �ܺ��� �߷� �ǵ帮��
	
	_XAN_LPS_LONGWEAPON_FOOTTOUCH,			// 92  �庴�� �߷� �ǵ帮��

	_XAN_LPS_USEKEY_HAND,					// 93  �Ǽ�   ����� ����
	_XAN_LPS_USEKEY_SHORTWEAPON,			// 94  �ܺ��� ����� ����
	_XAN_LPS_USEKEY_LONGWEAPON,				// 95  �庴�� ����� ����

	_XAN_ITEMPICKING,						// 96  ������ �ݱ�
	
	_XAN_MATCH_NONE,						// 97  �� �Ǽ�
	_XAN_MATCH_SHORTWEAPON,					// 98  �� �ܺ���
	_XAN_MATCH_LONGWEAPON,					// 99  �� �庴��
	_XAN_MATCH_CANCEL,						// 100 �� ���

	_XAN_IDLE_AXE,							// 101 �ι� ���

	_XAN_ELIXIR_FAIL,						// 102 ���� ���� ����
	
	//////////////////////////////////////////////////////////////////////
	// �� ����
	_XAN_IDLE_SWORD_PEACE,					// 103 �� ���(��ȭ)
	_XAN_IDLE_SWORD_ACTION,					// 104 �� �׼�(��ȭ)
	
	_XAN_MOVE_SWORD_REPEAT_PEACE,			// 105 �� �̵�(��ȭ)
	_XAN_MOVE_SWORD_REPEAT_BATTLE,			// 106 �� �̵�(����)

	_XAN_FASTMOVE_SWORD_START_PEACE,		// 107 �� ���� ����(��ȭ)
	_XAN_FASTMOVE_SWORD_REPEAT_PEACE,		// 108 �� ���� �ݺ�(��ȭ)
	_XAN_FASTMOVE_SWORD_END_PEACE,			// 109 �� ���� ����(��ȭ)

	_XAN_FASTMOVE_SWORD_START_BATTLE,		// 110 �� ���� ����(����)
	_XAN_FASTMOVE_SWORD_REPEAT_BATTLE,		// 111 �� ���� �ݺ�(����)
	_XAN_FASTMOVE_SWORD_END_BATTLE,			// 112 �� ���� ����(����)

	_XAN_BATTLEIDLE_SWORD_START,			// 113 �� ��� ��ȯ ����
	_XAN_BATTLEIDLE_SWORD_REPEAT,			// 114 �� ��� ��ȯ �ݺ�
	_XAN_BATTLEIDLE_SWORD_END,				// 115 �� ��� ��ȯ ����

	_XAN_NI_ATTACK_SWORD,					// 116 �� ����

	_XAN_DYINGREADY_SWORD_PEACE,			// 117 �� �и���(��ȭ)
	_XAN_DYINGREPEAT_SWORD_PEACE,			// 118 �� ��ȣ��(��ȭ)
	_XAN_HITBACK_SWORD_PEACE,				// 119 �� ��ġ��(��ȭ)
	_XAN_WAKEUP_SWORD_PEACE,				// 120 �� �����(��ȭ)
	_XAN_DIE_SWORD_PEACE,					// 121 �� ��������(��ȭ)

	_XAN_DYINGREADY_SWORD_BATTLE,			// 122 �� �и���(����)
	_XAN_DYINGREPEAT_SWORD_BATTLE,			// 123 �� ��ȣ��(����)
	_XAN_HITBACK_SWORD_BATTLE,				// 124 �� ��ġ��(����)
	_XAN_WAKEUP_SWORD_BATTLE,				// 125 �� �����(����)
	_XAN_DIE_SWORD_BATTLE,					// 126 �� ��������(����)

	_XAN_KNEE_SWORD_START,					// 127 �� �����ݱ� ����
	_XAN_KNEE_SWORD_REPEAT,					// 128 �� �����ݱ� �ݺ�
	_XAN_KNEE_SWORD_END,					// 129 �� �����ݱ� ����

	_XAN_LPS_USEKEY_SWORD_PEACE,			// 130 �� ����� ����(��ȭ)
	_XAN_LPS_USEKEY_SWORD_BATTLE,			// 131 �� ����� ����(����)
	
	_XAN_MATCH_WINNER_START,				// 132 �� - ����
	_XAN_MATCH_WINNER_REPEAT,				// 133 �� - ����

	_XAN_MATCH_LOSER_START,					// 134 �� - ����
	_XAN_MATCH_LOSER_REPEAT,				// 135 �� - ����

	_XAN_MATCH_DIE_START,					// 136 �� - ȥ��
	_XAN_MATCH_DIE_REPEAT,					// 137 �� - ȥ��

	_XAN_MATCH_VIEW_START,					// 138 �� - ȥ���� ����
	_XAN_MATCH_VIEW_REPEAT,					// 139 �� - ȥ���� ����

	_XAN_MATCH_PR_START,					// 140 �� - ȫ��
	_XAN_MATCH_PR_REPEAT,					// 141 �� - ȫ��
	_XAN_MATCH_PR_END,						// 142 �� - ȫ��
	
#ifdef _XDEF_CASTLEBATTLE
	_XAN_MANOR_WIN = 219,					// 219 ����� - �¸��� ���
	_XAN_MANOR_SHOUT,						// 220 ����� - ������
#endif
	
}_XCHARACTER_MOTION;





#endif
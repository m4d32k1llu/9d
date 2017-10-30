#ifndef _XOBJECTSCRIPTDEFINE_H_
#define _XOBJECTSCRIPTDEFINE_H_

//////////////////////////////////////////////////////////////////////////////////////
/////  OBJECT Trigger condition ���� /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_OBJECT_CONDITION_COUNT								9

#define ID_OBJECT_CONDITION_CHECKCLAN							0		// �÷��̾�_���ĺ�
#define ID_OBJECT_CONDITION_CHECKITEM							1		// �÷��̾�_�����۰˻�
#define ID_OBJECT_CONDITION_CHECKGENDER							2		// �÷��̾�_�����˻�
#define ID_OBJECT_CONDITION_CHECKQUESTCOMPLETE					3		// ����Ʈ �Ϸ� �˻�
#define ID_OBJECT_CONDITION_CHECKPROGRESSQUEST					4		// �������� ����Ʈ �˻�
#define ID_OBJECT_CONDITION_CHECKSTATE							5		// �÷��̾�_����ȿ���˻�
#define ID_OBJECT_CONDITION_CHECKCASTLEBATTLESTAGE				6		// �����_�ܰ�˻�
#define ID_OBJECT_CONDITION_CHECKCASTLEBATTLETHREAT				7		// �����_���ڰ��ɻ��°˻�
#define ID_OBJECT_CONDITION_CHECKPLAYERROLE						8		// �÷��̾�_���Ұ˻�


//////////////////////////////////////////////////////////////////////////////////////
/////  OBJECT Trigger action ���� ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_OBJECT_ACTION_COUNT									7

#define ID_OBJECT_ACTION_PLAYERMOVE								0		// �÷��̾�_�̵�
#define ID_OBJECT_ACTION_BOSSMOVE								1		// �÷��̾�_�������̵�
#define ID_OBJECT_ACTION_SETATTACKOBJECTSTATE					2		// �÷��̾�_������Ʈ���ݰ��ɻ��¼���
#define ID_OBJECT_ACTION_TEXTDISPLAY							3		// TEXT ���
#define ID_OBJECT_ACTION_MATCHINFO								4		// �� ������ ���
#define ID_OBJECT_ACTION_MATCHZONEENTEROREXIT					5		// ���� ����/����
#define ID_OBJECT_ACTION_CONDITIONALATTACTSETTING				6		// �÷��̾�_���Ǻ����ݼ���



//////////////////////////////////////////////////////////////////////////////////////
/////  OBJECT Script Item Type	  ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_OBJECT_TYPE_GATE										0		// ���Թ�
#define ID_OBJECT_TYPE_BOSSGATE									1		// ������ ���Թ�
#define ID_OBJECT_TYPE_TRAPLPS									2		// TRAP_LPS
#define ID_OBJECT_TYPE_MATCHINFO								3		// ��������


// Ʈ���� ���ǹ��� ��� ���� ----------------------------------------------------------
#define ID_OBJECT_TRIGGER_CONDITIONFUNCTION_OR			        0
#define ID_OBJECT_TRIGGER_CONDITIONFUNCTION_AND				    1

#endif
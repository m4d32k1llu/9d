//
//	KUQuestScriptBaseType.h
//
// Created Date : 2003.01.09 
// Author : Sohyun Park (kukuri), Iljun Park
// Desc : ����Ʈ ��ũ��Ʈ condition/action structure
///////////////////////////////////////////////////////////////////

#ifndef _KUQUESTSCRIPTBASETYPE_H_
#define _KUQUESTSCRIPTBASETYPE_H_

#define _XQUEST_MAX_INVENTORYSLOT			5

#include "XKernel.h"

// Trigger Condition Structure
typedef struct 
{
	BOOL				m_enable;				// ���ǽ� Ȱ��ȭ
	int					m_conditionclass;		// Ʈ���� ���ǽ� Ŭ����

	int					m_compareclass1 : 8;	// ��� ���� Ŭ����
	int					m_compareclass2 : 8;
	int					m_compareclass3 : 8;
	int					m_compareclass4 : 8;

	int					m_numericvalue1;			// ���ǽ� ���� ��ġ
	int					m_numericvalue2;
	int					m_numericvalue3;
	int					m_numericvalue4;
}_XTRIGGERDATA_CONDITION;


// trigger action structure
typedef struct 
{
	BOOL				m_enable;			// �ൿ�� Ȱ��ȭ 
	int					m_actionclass;		// �׼� ����

	int					m_actiondata1 : 8;	// �ൿ�� ����Ÿ
	int					m_actiondata2 : 8;
	int					m_actiondata3 : 8;
	int					m_actiondata4 : 8;

	int					m_numericvalue1;		// �ൿ�� ���� ��ġ
	int					m_numericvalue2;
	int					m_numericvalue3;
	int					m_numericvalue4;
	int					m_numericvalue5;
}_XTRIGGERDATA_ACTION;


// ����Ʈ�� ���� ��� ��
typedef enum
{
	QUEST_COMPLETE_RESULT_NONE = 0,		// ����Ʈ ó�� ���� ����
	QUEST_COMPLETE_RESULT_SOLVE,			// ����Ʈ �ذ�
	QUEST_COMPLETE_RESULT_UNSOLVE,			// ����Ʈ ���ذ� (����Ʈ reset)
	QUEST_COMPLETE_RESULT_MISS				// ����Ʈ ���� (�ٽ� �Ҽ�����)
}_XQUEST_COMPLETE_RESULT;


// ����Ʈ ���
typedef enum
{
	QUEST_QUEST_RESULT_SUCCESS = 0,		// ����
	QUEST_QUEST_RESULT_PROCESS,			// ����Ʈ ó���� (Ʈ���� ó����)
	
}_XQUESTITEM_RESULT;


// ���� ���� üũ ���
typedef enum
{
	QUEST_STARTCONDITION_RESULT_NOTIFYSERVER = 0,	// ���� ���� ����, ����Ʈ �����ϱ⸦ ����
	QUEST_STARTCONDITION_RESULT_NOTIFYUSER,			// ���� ���� ����, �������� �˷���
	QUEST_STARTCONDITION_RESULT_FAIL,				// ���� ���� �������� ����
	QUEST_STARTCONDITION_RESULT_ALREADY				// ���� ������ �̹� ������
}_XQUESTSTARTCONDITION_RESULT;

// condition ���
typedef enum
{
	QUEST_CONDITION_RESULT_FAIL = 0,			// ����
	QUEST_CONDITION_RESULT_SUCCESS,				// ����
	QUEST_CONDITION_RESULT_PROCESS,				// ������ (���� ����Ʈ)
	QUEST_CONDITION_RESULT_ALREADYPROCESS,		// �Ϸ� (���� ����Ʈ)
	QUEST_CONDITION_RESULT_ERROR				// ����
}_XQUESTCONDITION_RESULT;


// action ���
typedef enum
{
	QUEST_ACTION_RESULT_FAIL = 0,				// ����
	QUEST_ACTION_RESULT_SUCCESS,				// ���� ��
	QUEST_ACTION_RESULT_PROCESS,				// ���� �� Ȥ�� ������ (���ฮ��Ʈ�� ���� 0)
	QUEST_ACTION_RESULT_ALREADYPROCESS,			// �Ϸ� (���ฮ��Ʈ 1)
	QUEST_ACTION_RESULT_ERROR					// ����
}_XQUESTACTION_RESULT;

// trigger ���
typedef enum
{
	QUEST_TRIGGER_RESULT_WAITSERVER = 0,		// ������ request�� ������ (������ �� �����ؼ� ������ ��ٸ�)
	QUEST_TRIGGER_RESULT_SUCCESS,				// Ʈ���� ���� �Ϸ�
	QUEST_TRIGGER_RESULT_FAIL,					// Ʈ���� ���� �Ϸ�( �б�ÿ� ������ �������� �ʾ��� �� �ൿ���� �����ϰ� �����ϴ� �� )
	QUEST_TRIGGER_RESULT_PROCESS,				// ������ (������ �� �������� �ʾҰų� �׼��� �� �������� �ʾҰų�)
	QUEST_TRIGGER_RESULT_ERROR,					// ����
	QUEST_TRIGGER_RESULT_BLOCK					// �б� Ʈ���ŷ� ���� ������ Ʈ����
}_XQUESTTRIGGER_RESULT;


// Ʈ���� ����
typedef enum
{
	TRIGGER_SUCCESS = 0,
	TRIGGER_FAIL,
}_XTRIGGER_CLASS;

// action ����
typedef enum
{
	ACTION_NORMAL = 0,
	ACTION_EXCEPTED
}_XACTION_CLASS;


// Quest inventory
typedef struct 
{
	char			cItemType;
	unsigned short	usItemID;
	unsigned int	unCount;
}_XQUEST_ITEM;

// other server quest inventory
typedef struct  
{
	_XQUEST_ITEM	questinven[5];
	int				triggertimer;
}_XQUEST_OTHERINVEN;

// Quest Type
typedef enum _XQuest_Type
{
	_XQUEST_TYPE_NONE = 0,
		_XQUEST_TYPE_1,
		_XQUEST_TYPE_2,
		_XQUEST_TYPE_3,
		_XQUEST_TYPE_4,
		_XQUEST_TYPE_5
}_XQUEST_TYPE;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////         Functional Object ����        //////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                  
// condition ���
typedef enum
{
	FUNCTIONALOBJECT_CONDITION_RESULT_FAIL = 0,			// ����
		FUNCTIONALOBJECT_CONDITION_RESULT_SUCCESS		// ����
}_XFUNCTIONALOBJECT_CONDITION_RESULT;


// action ���
typedef enum
{
	FUNCTIONALOBJECT_ACTION_RESULT_FAIL = 0,				// ����
		FUNCTIONALOBJECT_ACTION_RESULT_SUCCESS,			// ���� ��
}_XFUNCTIONALOBJECT_ACTION_RESULT;


#ifdef _XDEF_USENEWQUESTTEXTSYSTEM
extern int	g_LoadQuestScriptText_LastIndex;
extern int*	g_LoadQuestScriptText_IndexTable;
#endif

#endif
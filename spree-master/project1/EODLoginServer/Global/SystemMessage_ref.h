#pragma once 
/*
	�ý��� �޽�����..
	//  DB Error Code : ODBC �Ǵ� DB �Լ����� �����ڵ�
	#define LOGIN_ERROR_NO_ID				1				// ��ϵ��� ���� ����
	#define LOGIN_ERROR_ALREADY_LOGIN		2				// �ٸ� ������ �α��� �߽��ϴ�
	#define LOGIN_ERROR_SUCCESS_MASTER		-1				// ��ڴ� : ���� 
	#define LOGIN_ERROR_SUCCESS				0				// �Ϲ� ������ : ���� 
	#define LOGIN_ERROR_INCORRECT_PASSWORD	3				// ��ȣ�� Ʋ�Ƚ��ϴ�. 
	#define LOGIN_ERROR_SQL_ERROR			5				// �� �� ����  ���� �Դϴ�. 
*/


// Forest State
#define  _FOREST_STATE_RUNNING				0					// ���� �۵���
#define  _FOREST_STATE_OFF					-1					// ���� ����
#define  _FOREST_STATE_CHECKUP_				-2					// ������ ������
#define  _FOREST_STATE_FULL_				-3					// ��ȭ����

// Login DB return value
#define  _LOGIN_DB_RETURN__NOT_FOUND_			1	// ���� ����
#define  _LOGIN_DB_RETURN__NOT_MATCH_PASSWORD	2	// ��ȣƲ��
#define  _LOGIN_DB_RETURN__AREADY_LOGINER		3	// �̹� �α�����
#define	 _LOGIN_DB_RETURN__NOT_UNI_NEXON		4	// �ؽ� ���̵� ����. �����϶�. 
#define  _LOGIN_DB_RETURN__BLOCKING_USER		6	// ��ŷ ���� ����
#define  _LOGIN_DB_RETURN__BLOCKING_CASE		100	// �̰ͺ��� ũ�� �������̴�.
#define  _LOGIN_DB_RETURN__ERROR_				9

// Ŭ���� ��ȣ..
#define _CLASS_BLOCK_NOTHING_				0		// �������� ���� ����
#define _CLASS_BLOCK_TAKING_				1		// �з�����
#define _CLASS_BLOCK_OFF_					2		// ������ ����
#define _CLASS_BLOCK_STOP_					3		// �Ͻ� ������ ����
#define _CLASS_BLOCK_TEEN_					4		// ���ѿ��� �̴���..
#define _CLASS_BLOCK_TESTER_ONLY			5		// �׽�Ʈ ������ ���� ����
#define _CLASS_BLOCK_GENERAL				12		// �⺻���� �����
#define _CLASS_BLOCK_SUPPORTER				18		// ���»� ��������
#define _CLASS_BLOCK_INSIDER_SEC			19		// ���� �ο� ����
#define _CLASS_BLOCK_INSIDER_FIR			20		// ���� �ο� ����
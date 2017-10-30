#pragma once

// Server �̵� ����
// Main = 19
// 152 - 159

struct _sHeader;

// Server�� �̵�, ���� ����
#define MSG_NO_SV_ENTER_GAME			152			// �α��� ������ ���� ���� ������ ����
#define MSG_NO_SV_ENTER_GSERVER			153			// �ٸ� ���� �������� �� ������ ���´�.
#define MSG_NO_SV_ENTER_RESULT			154			// ������ ���� ��û�� �ź��Ѵ�. ���� ���Ŀ� closesocket
#define MSG_NO_SV_MOVE_REQ				155			// �� �̵� ��û : ���� �Ǵ� NPC�� �̿��� ���� �̵���
#define MSG_NO_SV_TOGSERVER				156			// �� ������ ip, port�� ������ �ش�.

//#define MSG_NO_SV_ENTER_GAME			104
struct MSG_SV_ENTER_GAME
{
	_sHeader			header;
	u_char				ucMessage;

	char				ID[25];

	u_short				usUniqueID;
};

// ���� ������ �̵��� ��ǥ ������ ���� ���� ������ �Ѵ�. �Ⱥ����� ������.
//#define MSG_NO_SV_ENVER_GSERVER		105
struct MSG_SV_ENTER_GSERVER
{
	_sHeader			header;
	u_char				ucMessage;

	char				ID[25];

	u_short				usUniqueID;
};

//#define MSG_NO_SV_ENTER_RESULT		106
struct MSG_SV_ENTER_RESULT
{
	_sHeader			header;
	u_char				ucMessage;

	char				cResult;					// 0 ���� !0 ����
	char				cReserved;
};

//#define MSG_NO_SV_MOVE_REQ			107
struct MSG_SV_MOVE_REQ
{
	_sHeader			header;
	u_char				ucMessage;
	
	bool				bGate_NPC;					// ������ ���� �̵��̸� false, NPC�� ���� �̵��̸� true

	int					iNo;						// �����̳� NPC �ε���
	char				cGotoZone;
};

//#define MSG_NO_SV_TOGSERVER			108
struct MSG_SV_TOGSERVER
{
	_sHeader			header;
	u_char				ucMessage;

	char				cZoneNumber;				// Ŭ���̾�Ʈ�� � ���� �ε����� �������ش�. 
	                                                // == 0 �ϰ�� ���� OFF

	unsigned short		usServerPort;				// Ŭ���̾�Ʈ htons�� �ٲ㼭 ����Ѵ�.
	unsigned long		ulServerIP;					// Ŭ���̾�Ʈ �̰��� �״�� ����Ѵ�.
};
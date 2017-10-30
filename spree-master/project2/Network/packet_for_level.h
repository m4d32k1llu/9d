#pragma once

struct _sHeader;

// Level 
// Main = 13
// 104 - 111

#define MSG_NO_SET_LEVEL			104		// ĳ������ ����ġ�� �ƽ� ü�� ��
#define MSG_NO_SET_POWER			105		// ĳ������ ���� power ��
#define MSG_NO_LEVEL_UP_REQ			106
#define MSG_NO_POINT_USE			107

//#define MSG_NO_SET_LEVEL			64
struct MSG_SET_LEVEL
{
	_sHeader			header;
	u_char				ucMessage;

	unsigned int		uiJin;				// ��
};

//#define MSG_NO_SET_POWER			65
struct MSG_SET_POWER
{
	_sHeader			header ;
	u_char				ucMessage ;				// �ʿ��Ѱ�..
	bool				bMax ;					// true : �ִ밪 ����, false : ���簪 ����.
	
	short				sLifePower ;			// 
	short				sForcePower ;			//
	short				sConcentrationPower ;			//
} ;

// ���� �� ��ư�� ��������
// ������ ���� �޽��� ��ȣ�� MSG_LEVEL_UP_RESULT ��Ŷ�� ����. 
// �����ϸ� sInnerLevel �� 0���� ����.
struct MSG_LEVEL_UP_REQ
{
	_sHeader			header;
	u_char				ucMessage;
};

struct MSG_LEVEL_UP_RESULT
{
	_sHeader			header;
	u_char				ucMessage;

	short				sInnerLevel;		// ���� ����
	short				sLeftPoint;			// ���
	short				sLeftMateryPoint;	// ���� �����͸� ����Ʈ
	
	u_char				ucLevelupInfo ;		// 0 : ����, 1 : ���� ���� �� 2 : �ʿ���� ����
											// �ʿ���� �����ÿ��� ����ġ�� �� ä���! ������ ���� �˸� ����
};

// �����ִ� ������Ʈ�� 5�⿡ ����Ѵ�. 
// ������ ���� ��ȣ�� MSG_POINT_USE_RESULT �� ����.
struct MSG_POINT_USE_REQ
{
	_sHeader			header;
	u_char				ucMessage;

	char				cWhere;				// 0 : ����, 1 : ����, 2 : ����, 3 : �ɷ�, 4 : ����
};

struct MSG_POINT_USE_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	char				cWhere ;			// 0 : ����(�ǰ�), 1 : ����(����), 2. ����(����), 5�� ���� ����.
	short				sPoint ;			// �� ����Ʈ
	short				sPower ;			// Power
	short				sLeftPoint ;		// Ȯ���� �ִ´�.
} ;

#define MSG_NO_OTHER_CHARAC_LEVEL_UP				109

struct MSG_OTHER_CHARAC_LEVEL_UP
{
	_sHeader			header;
	u_char				ucMessage;

	u_short				usUniqueID;
};
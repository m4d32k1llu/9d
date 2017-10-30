#pragma once

#pragma pack(push,PREV_PACK)
#pragma pack(1)

// �̰����� ��Ŷ���� ���ԵǴ� ����ü�� �����Ѵ�.


enum
{
	max_count_of_challenge_list=20,

	max_win_count_of_castle=255,
};



/*
__cb_challenge_info  �� ����ü�� GS ���� ��Ŷ�� ���δ�. 
����, ����� �ٲ�� �ݵ�� GS �� ��ġ��ų ��.
*/



struct __cb_challenge_info
{
	int						iDBSeqNumber;		// DB ������ ������ �ε��� : �˻��� �̰����� �Ѵ�.

	int						iIndex;				// Ally or Org Index

	u_short					usMemberCount;		// �ο���.

	u_char					ucMoonpa : 5;		// not use
	u_char					ucRes1 : 1;			// 
	u_char					ucState : 1;		// 0 : normal, 1 : fighting state.
	u_char					ucType : 1;			// Organization(0) or Ally(1)

	u_short					usLoudCount;		// �ʱⰪ 0 : �ִ� 20

	u_short					usReserve;			// 

	char					cGroupName[en_or_name_length+1];
	char					cMasterName[en_charac_name_length+1];

	__cb_challenge_info()
	{
		reset();
	}
	inline void reset()	
	{
		ZeroMemory( this, sizeof(__cb_challenge_info) );
	}

	/*
	void convert_to_client( __cb_client_challenge_info& ref )
	{
	ref.usMemberCount = usMemberCount;
	ref.ucReserve = 0;
	ref.ucType = (ucType==0) ? 0 : 1;
	ref.ucState = (ucState==0) ? 0 : 1;
	CopyMemory( ref.cGroupName, cGroupName, en_or_name_length );
	ref.cGroupName[en_or_name_length] = 0;
	CopyMemory( ref.cMasterName, cMasterName, en_charac_name_length );
	ref.cMasterName[en_charac_name_length] = 0;
	}
	*/
};

/*
_sCastleInfo : ����� ���� ����
GS ���� ��Ŷ�� ���ǹǷ�, ����� �ٲ�� �ݵ�� GS �� ��ġ��ų ��.
*/

#ifdef _PD_CASTLE_RENEWAL_1_

struct _sCastleInfo
{
	int			iOrIndex;			// default 0
	u_char		ucType;				// 0 - Organization. 1 - Ally, 2 - Monster
	u_short		usMemberCount;		// ������ �Ǵ� ��ȸ�� �ο���.

	u_char		ucLevel;			// 0 - 4 ��� ����

	u_char		ucCurMatchCount_not;	// 0,1,2 �� �����.	 : ucCurMatchCount => renewal ���� �Ⱦ�.
	u_char		ucCombatState;		// 0 : ���� ��, 1 : ������,

	char		cOrName[en_or_name_length+1];
	char		cMasterName[en_charac_name_length+1];
	char		cMasterMoonpa;				//

	u_int		uiTotalMoney;
	u_int		uiPartMoney;

	u_short		usWinCount;			// ���� ȸ��.


	// ��� ���� ����. ��/��/��/��/��/��. End Time
	_sND_TimeUnit	timeOfGettCastle;			// ������ ��¥.
	_sND_TimeUnit	timeNextSchedule;		// ���� ���� ��¥
	_sND_TimeUnit	timeunit_not_use[2];		// �����󿡼� ����ü�� ũ�⿡ ������ ������ �����.

	inline int get_owner_index()		{ return iOrIndex; }
	inline u_char get_owner_type()		{ return ucType; }
	inline char * get_owner_name()		{ return cOrName; }
	inline u_char get_level()			{ return ucLevel; }
	inline char * get_master_name()		{ return cMasterName; }
	inline u_int get_total_money()		{ return uiTotalMoney; }
	inline u_int get_part_money()		{ return uiPartMoney; }
	inline u_short get_win_count()		{ return usWinCount; }

	inline _sND_TimeUnit * get_own_date()	{ return &timeOfGettCastle; }
	inline _sND_TimeUnit * get_schedule( int slot )	{ return &timeNextSchedule; }
};

#else 

struct _sCastleInfo
{
	int			iOrIndex;			// default 0
	u_char		ucType;				// 0 - Organization. 1 - Ally, 2 - Monster
	u_short		usMemberCount;		// ������ �Ǵ� ��ȸ�� �ο���.

	u_char		ucLevel;			// 0 - 4 ��� ����

	/* �Ʒ� ���� ucCurMatchCount, ucMatchState �� ����.
	1���� �����̸� : Count = 0, State = 0,
	2���� ���̶�� : Count = 1, satet = 1,
	3���� �����̸� : Count = 2, state = 0,	*/
	u_char		ucCurMatchCount;	// 0,1,2 �� �����.
	u_char		ucMatchState;		// 0 : ���� ��, 1 : ������,

	char		cOrName[en_or_name_length+1];
	char		cMasterName[en_charac_name_length+1];
	char		cMasterMoonpa;				//

	u_int		uiTotalMoney;
	u_int		uiPartMoney;

	u_short		usWinCount;			// ���� ȸ��.


	// ��� ���� ����. ��/��/��/��/��/��. End Time
	_sND_TimeUnit	timeUnit;

	_sND_TimeUnit	Schedule[3];		// 3���������� ������.

	inline int get_owner_index()		{ return iOrIndex; }
	inline u_char get_owner_type()		{ return ucType; }
	inline char * get_owner_name()		{ return cOrName; }
	inline u_char get_level()			{ return ucLevel; }
	inline u_char get_cur_match_count()	{ return ucCurMatchCount; }
	inline char * get_master_name()		{ return cMasterName; }
	inline u_int get_total_money()		{ return uiTotalMoney; }
	inline u_int get_part_money()		{ return uiPartMoney; }
	inline u_short get_win_count()		{ return usWinCount; }

	inline _sND_TimeUnit * get_own_date()	{ return &timeUnit; }
	inline _sND_TimeUnit * get_schedule( int slot )	{ return &Schedule[slot]; }
};

#endif

#pragma pack(pop,PREV_PACK)
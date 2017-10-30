#pragma once

//#################################################################################
//
//  Organization ��Ŷ : Main 21 ��� 
//  21 : 168 ~ 175
//
//#################################################################################

#define  _MAX_OR_UNIT_NUM 100

// ���� ���� ���� �ð� ����
struct _OutTime
{
	u_char	year ;
	u_char	month ;
	u_char	day ;
	u_char	h ;
	u_char  m ;
	u_char	s ;

	_OutTime & _OutTime::operator=( const _OutTime& rhs ) 
	{
		if( this == &rhs ) { return *this ; }
		
		year = rhs.year;
		month = rhs.month;
		day = rhs.day;
		h = rhs.h;
		m = rhs.m;
		s = rhs.s;
		return *this ;
	}

	void reset() 
	{		
		year = 0;
		month = 0;
		day = 0;
		h = 0;
		m = 0;
		s = 0;		
	} ;
} ;

class _Unit
{
public:	
	TCHAR			m_cName[13] ;	
	bool			m_bOnline ;
	char			m_cClass ;				// ���
	enum{
		_UNIT_CLASS_DEFAULT_UNIT_,			// �⺻ ������
		_UNIT_CLASS_SPECIAL_UNIT_,			// ��� ������
		_UNIT_CALSS_CAPTAIN_UNIT_,			// ���� ������..
	} ;
	short			m_sInnerLevel ;
	char			m_cZone ;	

	_Unit & _Unit::operator=( const _Unit& rhs ) 
	{
		if( this == &rhs ) { return *this ; }
		
		//memcpy( this, &rhs, sizeof(_Unit) ) ;
		memset( m_cName, 0, sizeof( TCHAR ) * 13 );
		strcpy( m_cName, rhs.m_cName );
		m_bOnline = rhs.m_bOnline;
		m_cClass = rhs.m_cClass;
		m_sInnerLevel = rhs.m_sInnerLevel;
		m_cZone = rhs.m_cZone;
		return *this ;
	}

	inline void reset() 
	{		
		//memset(this, 0, sizeof(_Unit) ) ; 
		memset( m_cName, 0, sizeof( TCHAR ) * 13 );
		m_bOnline = false;
		m_cClass = 0;
		m_sInnerLevel = 0;
		m_cZone = 0;

	}
};


// ������ �⺻ ����Ÿ��.. ��Ŷ���� ������ �̷� ����ü ������ ������ ó���ϱ� ���ϰڴ�..
class _OrganizationBase
{
public:
	int				m_iIndex ;
	char			m_cName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;			// ���� �̸�
	char			m_cProperty ;			// ���� �Ӽ�(���� | ��ȸ)
	char			m_cBaseParty ;			// �Ҽ� ����
	u_int			m_uiExperience ;		// ���� ����ġ
	u_int			m_uiContribution ;		// ������ ���� �⿩��
	u_short			m_usCurMemberNum ;		// ���� �ο�	
	char			m_cCaptainName[13] ;	
	
	_OrganizationBase & _OrganizationBase::operator=( const _OrganizationBase& rhs ) 
	{
		if( this == &rhs ) { return *this ; }
		
		memcpy( this, &rhs, sizeof(_OrganizationBase) ) ;
		return *this ;
	}	
};

struct _sHeader ;

// ���� �޽���..
#define MSG_NO_ORGANIZATION_CREATE_REQ			168	// ���� ����
#define MSG_NO_ORGANIZATION_UNIT_UPDATE_		169	// ������ �߰�, ����, ��������, �� ����, Ŭ���� ���� ��...(��û�� ���� ��� ����)
#define MSG_NO_ORGANIZATION_INFO_REQ			170	// ���� ���� ��û
#define MSG_NO_ORGANIZATION_INFO_RES			170	// ���� ���� ����
#define MSG_NO_ORGANIZATION_RESULT				172	// REQ �� ���� ���� �� ���� �޽���..
#define MSG_NO_ORGANIZATION_CLIENT				173	// // ���� ���� ��û�� ����..
#define MSG_NO_ORGANIZATION_GLOBAL_INFO			174	// �ֺ� ĳ���� ���� ����
#define MSG_NO_ORGANIZATION_NOTICE_REQ			175	// ���� ���� ��û
#define MSG_NO_ORGANIZATION_NOTICE				175	// ���� ���� ����

//#define MSG_NO_ORGANIZATION_CREATE_REQ			// ���� ����
struct MSG_ORGANIZATION_CREATE_REQ
{
	_sHeader			header ;
	u_char				ucMessage ;

	char				cOrganazationName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;	
};

//#define MSG_NO_ORGANIZATION_UNIT_UPDATE_		// ����, ���� ������ �ο�, Ŭ���� ���� ��... (��û�� ���� ��� ����)
struct MSG_ORGANIZATION_UNIT_UPDATE_
{
	_sHeader			header ;
	u_char				ucMessage ;	
	
	_Unit				_unit ;
	int					iCode ;
	enum{				// iCode
				//�ޱ� ����..
		_OR_UNIT_JOIN_,					// �������� �߰��϶�.. S -> C
		_OR_UNIT_DROP_,					// ������ �����϶�  S -> C		
		_OR_UNIT_KICK_,					// ���� ����
		// �ְ� �ޱ�.. 
		_OR_UNIT_ENTRUST_,				// �� ���� ( �� �� ����) S -> C
		_OR_UNIT_UPDATE_,				// ������ ������Ʈ( �α��� ����, Ŭ���� ����, ����ġ �����..) C -> S -> C
	} ;
	_OutTime			m_outTime ;		// (iCode == _OR_UNIT_UPDATE_ && m_outTime.year > 0) �϶��� �ð��� �����ϼ���..
};

//#define MSG_NO_ORGANIZATION_INFO_REQ			// ���� ��ü ���� ��û
struct MSG_ORGANIZATION_INFO_REQ
{
	_sHeader			header ;
	u_char				ucMessage ;

	int					index ;
	char				cCode ;
	enum{
		_REQ_MY_OR_INFO_,			// �� ���� ���� ��û
		_REQ_OR_GLOBAL_INFO,		// �ֺ� ĳ���� �������� ��û
	};
	// ���н� MSG_ORGANIZATION_RESULT�� ����� ������ �ڵ�
	enum{
		_FAILE_CODE_NOT_INNER_,				// ������ �������� ����..		
		_FAILE_CODE_ETC_,						// ��Ÿ �������� ����
	};
};

// %% ���� %% �� ��Ŷ�� �������� ��Ŷ�Դϴ�. �������� �ο����� ���� ��Ŷ ���̰� �޶���..
//#define MSG_NO_ORGANIZATION_INFO_RES		// ���� ���� ����
struct MSG_ORGANIZATION_INFO_RES
{
	_sHeader			header ;
	u_char				ucMessage ;

	bool				bCreateAble ;					// true : ���� ����,
	_OrganizationBase	_organizationBase ;	
	char				cMemberCount ;					// ������ �ο�..
	_Unit				_memberList[_MAX_OR_UNIT_NUM] ;	// ���������� ����Ʈ
	_OutTime			outTimeList[_MAX_OR_UNIT_NUM] ;
};


//#define MSG_NO_ORGANIZATION_RESULT				// REQ �� ���� ���� �� ���� �޽���..
struct MSG_ORGANIZATION_RESULT
{
	_sHeader			header ;
	u_char				ucMessage ;

	int					iCode ;						// ���� �ڵ��Դϴ�.
	
	// ���н� MSG_ORGANIZATION_RESULT�� ����� ������ �ڵ�
	enum{
		_OR_DESTROY_,		// ������ �����Ǿ���..(ucFromMessage �� ��� ����)
		
		_FAILED_CODE_ALREADY_INNER_,				// �̹� ������ ������..
		_FAILED_CODE_NEED_MORE_MEMBER_,			// �ʱ� �߱� �ο��� ������(5�� �̻�)
		_FAILED_CODE_NOT_PARTY_JOINNER_,			// ��Ƽ ���°� �ƴմϴ�.
		_FAILED_CODE_CANT_PARTY_LEADER,			// ��Ƽ�常 �����մϴ�.
		_FAILED_CODE_UNLIKE_MOONPA,				// �ٸ� ���İ� �ֽ��ϴ�.
		_FAILED_CODE_UNLIKE_ZONE,				// �ٸ� ������ �ֽ��ϴ�.
		_FAILED_CODE_USING_NAME,					// ������� �̸��Դϴ�.
		_FAILED_CODE_ETC_,						// ��Ÿ �������� ����
		_FAILED_CODE_FULL,						// ��� �ο��� �� �� �ִ�..
		_FAILED_CODE_NEED_MORE_LEVEL,			// ������ ������..
	};
	
};


//#define MSG_NO_ORGANIZATION_CLIENT				// ���� ���� ��û�� ����..
struct MSG_ORGANIZATION_CLIENT_						// Client <-> GS <-> Client
{
	_sHeader			header ;
	u_char				ucMessage ;

	int					iCode ;	
	char				cName[13] ;			// ������ ��û�Ҷ��� ���� �̸�.. �����Ҷ��� �߽��� �̸�..
	
	
	enum{
		// ��û
		_OR_REQ_INVITE_,					// �� ���������� �츮 ������ ������� ����
		_OR_REQ_ENTRUST_,					// �ϰ� ¯ ������� ����..

		_OR_REQ_MOONPA_CHECK_,				// ��Ƽ������ ���İ� �������� �˻��Ѵ�.
	
		// ����
		_OR_REQ_INVITE_OK_,					// ������ �����ϰڴ�.
		_OR_REQ_INVITE_NO_,					// ������ �������� �ʰڴ�.		
		_OR_REQ_ENTRUST_OK_,				// ¯ �ϰڴ�.
		_OR_REQ_ENTRUST_NO_,				// ¯ ���ϰڴ�.
		_OR_REQ_LOGOFF_,					// ������ �α׿�����..
		_OR_REQ_ALREADY_INNER_,				// �̹� ������ ���

		_OR_REQ_MOONPA_OK_,					// ���İ� �����̴�.
		
	} ;
};

//#define MSG_NO_ORGANIZATION_GLOBAL_INFO			174	// �ֺ� ĳ���� ���� ����
struct MSG_ORGANIZATION_GLOBAL_INFO 
{
	_sHeader			header ;
	u_char				ucMessage ;

	int				m_iIndex ;
	char			m_cName[_XDEF_ORGANIZATIONNAMELENGTH+1] ;			// ���� �̸�
	char			m_cProperty ;			// ���� �Ӽ�(���� | ��ȸ)
	char			m_cBaseParty ;			// �Ҽ� ����
	u_int			m_uiExperience ;		// ���� ����ġ	
};

//#define MSG_NO_ORGANIZATION_NOTICE_REQ			175	// ���� ���� ��û / ����
struct MSG_ORGANIZATION_NOTICE_REQ 
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	int					m_seqNum ;	// 0, 1, 2(LAST)
	int					m_iIndex ;
	union
	{
		struct _Notice_Data
		{
			char		writer[13] ;
			char		writedate[15] ;
			char		notice[72] ;
		} m_NoticeData ;

		char			m_cNotice[100] ;		/*#define _OR_NOTICE_ONE_PIECE_SIZE		100*/		
	};	
	
};

//#define MSG_NO_ORGANIZATION_NOTICE				175	// ���� ���� ��û / ����
struct MSG_ORGANIZATION_NOTICE 
{
	_sHeader			header ;
	u_char				ucMessage ;
	
	union
	{
		struct _Notice_Data
		{
			char		writer[13] ;
			char		writedate[15] ;
			char		notice[272] ;
		} m_NoticeData ;

		char			m_cNotice[300] ;
	};
};




#ifdef _XTS_ALIANCE_20061018

//////////////////////////////////////////////////////////////////////////
// ���� ���� ��Ŷ.
//////////////////////////////////////////////////////////////////////////
#define MSG_NO_ALLY_BASE						171	// ���Ϳ� ���õ� ��� ��Ŷ.

enum EN_ALLY_PACKET_MODE
{
	en_ally_mode_request_ally=0,				// 0 ���� ����
		en_ally_mode_request_from,				// 1 ���������� ������ �Դ�
		en_ally_mode_response_yes,				// 2 ������ �������� Yes
		en_ally_mode_response_no,				// 3 ������ �������� No
		en_ally_mode_request_ally_cancel,		// 4 ���� ���� ����ϱ�.
		en_ally_mode_ally_is_made,				// 5 ������ �Ἲ�Ǿ���.		
		en_ally_mode_org_enter_ally,            // 6 �ڽ��� �������� ���Ϳ� ���ԉ��.		
		en_ally_mode_request_banish,            // 7 �߹� ��û		
		en_ally_mode_banish_complete,           // 8 �߹�Ǿ��ٰ�, �����ֿ��� �˷���.
		

		en_ally_mode_request_ally_info=20,		// 20 ������ �� ���� ��û.
		en_ally_mode_ally_all_info,				// 21 ������ ��� ����.
		en_ally_mode_notice_enter_new_org,		// 23 ���ο� �������� ���Դ�.
		en_ally_mode_notice_leave_org,			// 24 �������� Ż���ߴ�.
		en_ally_mode_scattered,					// 25 ������ ��ü�Ǿ���.		
		en_ally_mode_req_leave,					// 26 Ż�� ��û
		en_ally_mode_allow_leave,				// 27 Ż�� ��û ����.
		en_ally_mode_req_scatter,				// 28 �ػ� ��û
		en_ally_mode_allow_scatter,				// 29 �ػ� ��û ����.		
		en_ally_mode_request_other_ally,        // 30 �ٸ� ������ ���� ��û		
		en_ally_mode_other_ally_info,           // 31 �ٸ� ������ ����		
		en_ally_mode_notice_banish_org,         // 32 �������� �߹�Ǿ���.
		en_ally_mode_no_ally_info,				// 33 ������ ������ ����.
		

		en_ally_mode_error_we_already_ally=100,	// 100 �̹� ������ �ΰ� �ִ�.		
		en_ally_mode_error_they_already_ally,	// 101 ��� �������� �̹� ������ �ΰ� �ִ�.		
		en_ally_mode_error_cannot_ally,         // 102 ��/�鵵 �� ������ ������ ���� �� ����.		
		en_ally_mode_error_not_found_info,		// 103 ��û�� ������ ã�� �� �����ϴ�.		
		en_ally_mode_error_cannot_find_user,	// 104 ������ ã�� �� ����.		
		en_ally_mode_error_i_am_busy,           // 105 ���� ��û��.		
		en_ally_mode_error_busy,				// 106 ����� �ٸ� �� ��.		
		en_ally_mode_error_cannot_request,      // 107 �������� ���ų�, ���ְ� �ƴϴ�. : Add
		en_ally_mode_error_not_hero,            // 108 ����� ������ ���ְ� �ƴϴ�.
		en_ally_mode_error_in_combat,			// 109 �������� ������� ��û�� �����̹Ƿ� ��ȸ�� �Ἲ�� �� ����
		en_ally_mode_error_target_in_combat,	// 110 Ÿ���� ������� ��û�� �����̹Ƿ� �߰��� �� ����.

};
//#define MSG_NO_ALLY_BASE						171	// ���Ϳ� ���õ� ��� ��Ŷ.
struct MSG_ALLY_BASE
{
	_sHeader      header;
	u_char        ucMessage;
	
	u_char        ucMode;	
};

struct __ally_client_info
{
	int		iOrIndex;
	char	iOrName[_XDEF_ORGANIZATIONNAMELENGTH+1];
};


// en_ally_mode_request_ally : C->S, ������ ��û�� ��,
struct MSG_ALLY_REQUEST_ALLY : public MSG_ALLY_BASE
{
	u_short         usWhom;   // �������� ��û�ϴ� ��.
};

// en_ally_mode_request_from : S->C, ���� ��û�� �Դ�.
struct MSG_ALLY_REQEUST_FROM :public MSG_ALLY_BASE
{
	u_short         usFrom;
	u_char          ucType;   // 0 : ������, 1 :����
	char            cName[_XDEF_ORGANIZATIONNAMELENGTH+1];
};

//en_ally_mode_response_yes : �������� Yes �� ������.
struct MSG_ALLY_MODE_RESPONSE_YES :public MSG_ALLY_BASE
{
};

//en_ally_mode_response_no
struct MSG_ALLY_MODE_RESPONSE_NO :public MSG_ALLY_BASE
{
};

//en_ally_mode_request_ally_cancel : ������ ���.
struct MSG_ALLY_MODE_REQUEST_ALLY_CANCEL :public MSG_ALLY_BASE
{
};

// en_ally_mode_ally_is_made :������ ���������.
struct MSG_ALLY_MODE_IS_MADE : public MSG_ALLY_BASE
{
	
	int		iAllyIndex;                        // ������� ������ �ε���.
	
	int		iOr1Index;                        // �����ְ� �Ǵ� �������� �ε��� (������ ������ ������ ������ �̸�)
	char	iOr1Name[_XDEF_ORGANIZATIONNAMELENGTH+1];
	
	int		iOr2Index;                        // ���Ե� �ι�° �������� �ε��� (������ ���Ϳ� �����ϴ� ������ �̸�)
	char	iOr2Name[_XDEF_ORGANIZATIONNAMELENGTH+1];	
};

//		en_ally_mode_org_enter_ally,            // 6 �ڽ��� �������� ���Ϳ� ���ԉ��.	
struct MSG_ALLY_MODE_ORG_ENTER_ALLY : public MSG_ALLY_BASE	// �Ʒ��� MSG_ALLY_MODE_ALLY_ALL_INFO �� �����ϰ� �����.
{
	int						iAllyIndex;			// ���� �ε���
	char					cMasterName[13];	// ȸ�� �̸�
	u_char					ucNoOrg;			// ���͸ɹ� ����
	__ally_client_info		info[5];			//
	
};


//en_ally_mode_request_banish,               // 7 �߹� ��û
struct MSG_ALLY_MODE_REQUEST_BANISH : public MSG_ALLY_BASE
{
	int		iOrgIndex;                        // �߹��Ϸ��� �������� �ε���.
};

//en_ally_mode_banish_complete,				// 8 �߹�Ǿ��ٰ�, �����ֿ��� �˷���.
struct MSG_ALLY_MODE_BANISH_COMPLETE : public MSG_ALLY_BASE
{
	int		iOrgIndex;                      // 
};


//en_ally_mode_request_ally_info=20,		// 20 ������ �� ���� ��û.
//en_ally_mode_ally_all_info,				// 21 ������ ��� ����.
//en_ally_mode_notice_enter_new_org,		// 23 ���ο� �������� ���Դ�.
//en_ally_mode_notice_leave_org,			// 24 �������� Ż���ߴ�.
//en_ally_mode_scattered,					// 25 ������ ��ü�Ǿ���.		
//en_ally_mode_req_leave,					// 26 Ż�� ��û
//en_ally_mode_allow_leave,				// 27 Ż�� ��û ����.
//en_ally_mode_req_scatter,				// 28 �ػ� ��û
//en_ally_mode_allow_scatter,				// 29 �ػ� ��û ����.		
//en_ally_mode_request_other_ally,        // 30 �ٸ� ������ ���� ��û		
//en_ally_mode_other_ally_info,           // 31 �ٸ� ������ ����		
//en_ally_mode_notice_banish_org,         // 32 �������� �߹�Ǿ���.

//en_ally_mode_request_ally_info=20,// 20 ������ �� ���� ��û.
struct MSG_ALLY_MODE_REQUEST_ALLY_INFO : public MSG_ALLY_BASE
{
	int		iAllyIndex;					// ��û�ϴ� ������ �ε���.
};

// ally_info_update �� ����. ���� ������ �ٲ�� �Ǹ� �� ��Ŷ���� �ֺ��� �˸��� �ȴ�.
// ������ ���� �ȹٲ�ϱ�.
//en_ally_mode_ally_all_info,       // 21 ������ ��� ����.
struct MSG_ALLY_MODE_ALLY_ALL_INFO : public MSG_ALLY_BASE
{
	// ucNoOrg �� ����, ��������.ucNoOrg �� �߶� �Ľ��� ��.
	int		iAllyIndex;				// ���� �ε���.
	char	cMasterName[13];		// ȸ�� �̸� 
	u_char	ucNoOrg;				// ���� ��� ����
	__ally_client_info	info[5];    //
	
	void set_length()
	{
		header.sLength = sizeof(MSG_ALLY_BASE) + sizeof(int) +
			sizeof(u_char) + ucNoOrg*sizeof(__ally_client_info);
	}
	
};

// �� ��Ŷ�� ������ ���Ϳ� ���Ե� �ܿ��鿡�Ը� ���� ���̴�.
//en_ally_mode_notice_enter_new_org,// 23 ���ο� �������� ���Դ�.
struct MSG_ALLY_MODE_NOTICE_NEW_ORG : public MSG_ALLY_BASE
{
	__ally_client_info	addInfo;              // �߰��Ǵ� �������� ����.
};

//en_ally_mode_notice_leave_org,    // 24 �������� Ż���ߴ�.
struct MSG_ALLY_MODE_NOTICE_LEAVE_ORG : public MSG_ALLY_BASE
{
	int	iLeaveOrgIndex;               // Ż���ϴ� �������� �ε���.
};

//en_ally_mode_scattered,           // 25 ������ ��ü�Ǿ���.
struct MSG_ALLY_MODE_SCATTERED : public MSG_ALLY_BASE
{
	u_char	ucScatterReason;            // 0 - �������� ��ü. 1 - ������ Ż�� ���� ��ü.
};


//en_ally_mode_req_leave,                                    // 26 Ż�� ��û
struct MSG_ALLY_MODE_REQ_LEAVE : public MSG_ALLY_BASE
{
};

//en_ally_mode_allow_leave,                                  // 27 Ż�� ��û ����.
// �� ��Ŷ�� ������ Ż�� �������� �ռ�, Ż�� ��û�� ���ֿ��Ը� �������� �޽�����.
struct MSG_ALLY_MODE_ALLOW_LEAVE : public MSG_ALLY_BASE
{
};

//en_ally_mode_req_scatter,                                   // 28 �ػ� ��û
struct MSG_ALLY_MODE_REQ_SCATTER : public MSG_ALLY_BASE
{
};

//en_ally_mode_allow_scatter,                                // 29 �ػ� ��û ����.
struct MSG_ALLY_MODE_ALLOW_SCATTER : public MSG_ALLY_BASE
{
};

//en_ally_mode_request_other_ally,          // 30 �ٸ� ������ ���� ��û
struct MSG_ALLY_MODE_REQUEST_OTHER_ALLY : public MSG_ALLY_BASE
{
	int		iAllyIndex;
};

//en_ally_mode_other_ally_info,                 // 31 �ٸ� ������ ����
struct MSG_ALLY_MODE_OTHER_ALLY_INFO : public MSG_ALLY_BASE
{
	int		iAllyIndex;			// ���� �ε��� 
	char	cAllyMaster[13];	// ������ ������ �̸� 
	char	cAllyName[_XDEF_ORGANIZATIONNAMELENGTH+1];
};

//en_ally_mode_notice_banish_org,                       // 32 �������� �߹�Ǿ���.
struct MSG_ALLY_MODE_NOTICE_BANISH_ORG : public MSG_ALLY_BASE
{
	int		iOrgIndex;
	char	cOrgName[_XDEF_ORGANIZATIONNAMELENGTH+1];
};

//en_ally_mode_no_ally_info  // 33 ������ ������ ����.
struct MSG_ALLY_MODE_NO_ALLY_INFO : public MSG_ALLY_BASE
{
	int		iAllyIndex;			// �ش� ������ ������ ����. 
};


// en_ally_mode_error_we_already_ally=100,	// 100 �̹� ������ �ΰ� �ִ�.
// en_ally_mode_error_they_already_ally,	// 101 ��� �������� �̹� ������ �ΰ� �ִ�.
// en_ally_mode_error_cannot_ally,			// 102 ��/�鵵 �� ������ ������ ���� �� ����.
// en_ally_mode_error_not_found_info,		// 103 ��û�� ������ ã�� �� �����ϴ�.
// en_ally_mode_error_cannot_find_user,		// 104 ������ ã�� �� ����.
// en_ally_mode_error_i_am_busy,			// 105 ���� ��û��.
// en_ally_mode_error_busy,					// 106 ����� �ٸ� �� ��.
// en_ally_mode_error_cannot_request,       // 107 �������� ���ų�, ���ְ� �ƴϴ�. 
// en_ally_mode_error_not_hero,				// 108 ����� ������ ���ְ� �ƴϴ�.


struct MSG_ALLY_ERROR : public MSG_ALLY_BASE
{
};



#endif
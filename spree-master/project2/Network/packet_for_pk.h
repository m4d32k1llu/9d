#pragma once
// #pragma pack(1)
/*enum
{
	en_charac_name_length=12,
};*/

//��ġ������ �⺻ ����
struct _sLTS_Data
{
	int			iStartTrackingTime;			// ��ġ������û�ð�	
	u_short     usReserve;
	u_char		ucTimeMode;					// �Ƿ��� ��ġ���� ���� ( 1) 24�ð�, 2) 48�ð� ) 
	char		szCharacName[en_charac_name_length+1];
	_sLTS_Data()
		: iStartTrackingTime(0), ucTimeMode(0)
	{}
};


//��ġ ���� (USER_GS : x,z ��ǥ�� �ʿ�)
struct _sCL_LTS_Data
{
	u_char		ucPkOrSpMode;	//1.Pk 2. Ư����
	char		szCharacName[en_charac_name_length+1];

	short sPosX;
	short sPosZ;
	
	_sCL_LTS_Data()
		:ucPkOrSpMode(0),sPosX(0),sPosZ(0)
	{}
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define MSG_NO_LTS						22		// ��ġ ����.

enum EN_LTS_DATA_COUNT
{
	en_lts_lvpk_data_count = 30,
	en_lts_pk_data_count = 10,
	en_lts_sp_data_count = 1,
	en_lts_pos_info_count = 11,
};


/////////////////
//�⺻
//��ġ���� �⺻ ��Ŷ
struct MSG_LTS
{
	_sHeader				header;
	u_char					ucMessage;

    u_char					ucPktMode;

//	MSG_LTS(u_short length, u_char mode)
//		: header(length),ucMessage(MSG_NO_LTS),ucPktMode(mode)
//	{}
};

enum EN_LTS_CL_GS
{
	// �������� ū PK ��
	en_lts_lvpk_info_gs_cl=0,			//������ �������
	en_lts_lvpk_insert_gs_cl,	
	en_lts_lvpk_del_req_cl_gs,	
	en_lts_lvpk_del_req_gs_cl,
	



	// PK �� ����
	en_lts_pk_info_gs_cl=10,			//PK�� �������
	en_lts_pk_insert_req_gs_cl,			// pk�� �̸��� �˷��ش� - �׾��� �� �������� �˷���
	en_lts_pk_insert_req_cl_gs,
	en_lts_pk_insert_success_gs_cl,
	en_lts_pk_insert_failed_gs_cl,
	en_lts_pk_del_success_gs_cl,

	// Ư���� ����
	en_lts_sp_info_gs_cl=20,			//Ư���� �������
	en_lts_sp_insert_req_cl_gs,
	en_lts_sp_insert_success_gs_cl,
	en_lts_sp_insert_failed_gs_cl,
	en_lts_sp_del_success_gs_cl,
		
	en_lts_pos_info_gs_cl =30, //3�ʸ��� PK�� Ư���� (ĳ���� + ��ġ����)
};


// �������� ū PK ��
// en_lts_lvpk_info_gs_cl=0,
struct MSG_LTS_LVPK_INFO_GS_CL : public MSG_LTS
{
	u_char ucCount; //������ ����
	_sLTS_Data LvPkData[en_lts_lvpk_data_count];
	
//	MSG_LTS_LVPK_INFO_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_LVPK_INFO_GS_CL), en_lts_lvpk_info_gs_cl)
//	{}

	void set_length()
	{
		if( ucCount > en_lts_lvpk_data_count )
			ucCount = en_lts_lvpk_data_count;

		header.sLength = sizeof(MSG_LTS_LVPK_INFO_GS_CL) - ( en_lts_lvpk_data_count - ucCount ) * sizeof(_sLTS_Data);
	}
};

//	en_lts_lvpk_insert_gs_cl,	
struct MSG_LTS_LVPK_INSERT_GS_CL : public MSG_LTS
{
	_sLTS_Data	PkData;

//	MSG_LTS_LVPK_INSERT_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_LVPK_INSERT_GS_CL), en_lts_lvpk_insert_gs_cl)
//	{}
};

//	en_lts_lvpk_del_req_cl_gs,
struct MSG_LTS_LVPK_DEL_REQ_CL_GS : public MSG_LTS
{
	char szDelCharacName[en_charac_name_length+1];
	
//	MSG_LTS_LVPK_DEL_REQ_CL_GS()
//		: MSG_LTS(sizeof(MSG_LTS_LVPK_DEL_REQ_CL_GS), en_lts_lvpk_del_req_cl_gs)
//	{}
};

//en_lts_lvpk_del_req_gs_cl
struct MSG_LTS_LVPK_DEL_REQ_GS_CL : public MSG_LTS
{
	char szDelCharacName[en_charac_name_length+1];
//	MSG_LTS_LVPK_DEL_REQ_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_LVPK_DEL_REQ_GS_CL), en_lts_lvpk_del_req_gs_cl)
//	{}
};



// PK �� ����
//	en_lts_pk_info_gs_cl=10,
struct MSG_LTS_PK_INFO_GS_CL : public MSG_LTS
{
	u_char			ucCount;//����
	_sLTS_Data		PkData[en_lts_pk_data_count];
	
//	MSG_LTS_PK_INFO_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_PK_INFO_GS_CL), en_lts_pk_info_gs_cl)
//	{}

	void set_length()
	{
		if( ucCount > en_lts_pk_data_count )
			ucCount = en_lts_pk_data_count;

		header.sLength = sizeof(MSG_LTS_PK_INFO_GS_CL) - ( en_lts_pk_data_count - ucCount ) * sizeof(_sLTS_Data);
	}
};

// 2007_03_30 �߰� - �׾��� �� ���� �׿����� �������� �˷��� / �Ƿ��� �� �� �̸����� ������
struct MSG_LTS_PK_INSERT_REQ_GS_CL : public MSG_LTS
{
	char szInsertCharacName[en_charac_name_length+1];
};

	
//2. Pk�� ��ġ���� en_lts_pk_insert_req_cl_gs,
struct MSG_LTS_PK_INSERT_REQ_CL_GS : public MSG_LTS
{
	char szInsertCharacName[en_charac_name_length+1];
	
	u_char		ucTimeMode;		//1)24�ð�, 2)48�ð�
	
//	MSG_LTS_PK_INSERT_REQ_CL_GS()
//		:MSG_LTS(sizeof(MSG_LTS_PK_INSERT_REQ_CL_GS), en_lts_pk_insert_req_cl_gs)
//	{}
};

//	en_lts_pk_insert_success_gs_cl,
struct MSG_LTS_PK_INSERT_SUCCESS_GS_CL : public MSG_LTS
{
	_sLTS_Data		PkData;
	u_int			uiRemainMoney;//����ϰ� ������.

	
//	MSG_LTS_PK_INSERT_SUCCESS_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_PK_INSERT_SUCCESS_GS_CL), en_lts_pk_insert_success_gs_cl)
//	{}
};

//	en_lts_pk_insert_failed_gs_cl,
struct MSG_LTS_PK_INSERT_FAILED_GS_CL : public MSG_LTS
{
	u_char ucRes;
	
//	MSG_LTS_PK_INSERT_FAILED_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_PK_INSERT_FAILED_GS_CL), en_lts_pk_insert_failed_gs_cl)
//	{}
};

//	en_lts_pk_del_success_gs_cl,
//2. pk�� ��ġ����
struct MSG_LTS_PK_DEL_SUCCESS_GS_CL : public MSG_LTS
{
	char		szDelCharacName[en_charac_name_length+1];
	
//	MSG_LTS_PK_DEL_SUCCESS_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_PK_DEL_SUCCESS_GS_CL), en_lts_pk_del_success_gs_cl)
//	{}
};


//	en_lts_sp_info_gs_cl=20,
struct MSG_LTS_SP_INFO_GS_CL : public MSG_LTS
{
	
	u_char     ucCount;//����
	_sLTS_Data SpData[en_lts_sp_data_count];
	
//	MSG_LTS_SP_INFO_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_SP_INFO_GS_CL), en_lts_sp_info_gs_cl)
//	{}
};


//	en_lts_sp_insert_req_cl_gs,
//3. Ư����
struct MSG_LTS_SP_INSERT_REQ_CL_GS : public MSG_LTS
{
	char szInsertCharacName[en_charac_name_length+1];
	
	u_char ucTimeMode;
	
//	MSG_LTS_SP_INSERT_REQ_CL_GS()
//		: MSG_LTS(sizeof(MSG_LTS_SP_INSERT_REQ_CL_GS), en_lts_sp_insert_req_cl_gs)
//	{}
};

	
//	en_lts_sp_insert_success_gs_cl,
struct MSG_LTS_SP_INSERT_SUCCESS_GS_CL : public MSG_LTS
{
	_sLTS_Data SpData;
	u_int			uiRemainMoney;//����ϰ� ������.
	
//	MSG_LTS_SP_INSERT_SUCCESS_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_SP_INSERT_SUCCESS_GS_CL), en_lts_sp_insert_success_gs_cl)
//	{}
};


//	en_lts_sp_insert_failed_gs_cl,
struct MSG_LTS_SP_INSERT_FAILED_GS_CL : public MSG_LTS
{
	u_char	ucRes;
	
//	MSG_LTS_SP_INSERT_FAILED_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_SP_INSERT_FAILED_GS_CL), en_lts_sp_insert_failed_gs_cl)
//	{}
};

//	en_lts_sp_del_success_gs_cl,
//3. Ư���� ��ġ����
struct MSG_LTS_SP_DEL_SUCCESS_GS_CL : public MSG_LTS
{
	char	szDelCharacName[en_charac_name_length+1];
	
//	MSG_LTS_SP_DEL_SUCCESS_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_SP_DEL_SUCCESS_GS_CL), en_lts_sp_del_success_gs_cl)
//	{}
};


//	en_lts_pos_info_gs_cl,
struct MSG_LTS_POS_INFO_GS_CL : public MSG_LTS
{
	u_char			ucPkCount;//Pk �� ����
	_sCL_LTS_Data	LtsPosData[en_lts_pk_data_count +  en_lts_sp_data_count];

//	MSG_LTS_POS_INFO_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_POS_INFO_GS_CL), en_lts_pos_info_gs_cl)
//	{}

	void set_length()
	{
		if( ucPkCount > en_lts_pk_data_count + en_lts_sp_data_count)
			ucPkCount = en_lts_pk_data_count + en_lts_sp_data_count;

		header.sLength = sizeof(MSG_LTS_POS_INFO_GS_CL) - ( en_lts_pk_data_count + en_lts_sp_data_count - ucPkCount ) * sizeof(_sCL_LTS_Data);

	}
};

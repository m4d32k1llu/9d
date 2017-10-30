#pragma once

struct _sHeader ;

#define MSG_NO_COLLECT_REQ				159		// ��ǰ Ȥ�� ����ǰ ��ȯ��û
#define MSG_NO_COLLECT_RES				MSG_NO_COLLECT_REQ	//���� ���� ����ǰ ��ȯ ���
//MSG_NO_COLLECT_REQ				159		// ��ǰ Ȥ�� ����ǰ ��ȯ��û
struct MSG_COLLECT_REQ
{
	_sHeader			header;
	u_char				ucMessage;

	//���� ��Ŷ�� Ÿ��
	u_char				ucPacketType;	//0: �ڿ� ����, 1: ��ǰ save, 2: ��ǰ ����

	u_short				usIndex;		//�ڿ� Ȥ�� ��ǰ �ε���

	u_char				ucSlotIndex;	//�ڿ� Ȥ�� ��ǰ�� ���� �ε���(��ǰ save�ÿ��� ���)
};

//MSG_NO_COLLECT_RES				MSG_NO_COLLECT_REQ	//���� ���� ����ǰ ��ȯ ���
struct MSG_COLLECT_RES
{
	_sHeader			header;
	u_char				ucMessage;

	//���� ��Ŷ�� Ÿ��
	u_char				ucPacketType;	//0: �ڿ� ����, 1: ��ǰ save, 2: ��ǰ ����
	u_char				ucFlag;			//0: ����, 1: ����
};

// ���� ��Ŷ ����

#define MAX_MATERIAL_RESOURCE_ITEM			16	// ��� ���� �ִ� ����

struct MSG_COLLECT_PACKET 
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucMode;		// en_collect_mode..
};

enum
{
	en_collect_mode_complete_resource = 0,		// �ڿ� ����
	en_collect_mode_save_collection = 1,		// ��ǰ ����
	en_collect_mode_complete_collection = 2,	// ��ǰ ����

	en_collect_mode_result = 10
};

// en_collect_mode_complete_resource		// �ڿ� ���� (������)
struct MSG_COLLECT_COMPLETE_RESOURCE : public MSG_COLLECT_PACKET
{
	u_short				usIndex;			//�ڿ� Ȥ�� ��ǰ �ε���
	u_char				item_slot_index[MAX_MATERIAL_RESOURCE_ITEM];	// ��� �������� �κ��丮 ���� �ε���
};

// en_collect_mode_save_collection			// ��ǰ ����
struct MSG_COLLECT_SAVE_COLLECTION : public MSG_COLLECT_PACKET
{
	u_short				usIndex;				//�ڿ� Ȥ�� ��ǰ �ε���
	u_char				ucCollectionSlotIndex;	// ��ǰ�� ��� ��� �ε��� 
	u_char				ucInvenSlotIndex;		// ��ᰡ �ִ� �κ��丮 �ε���
};

// en_collect_mode_complete_collection = 2	// ��ǰ ����
struct MSG_COLLECT_COMPLETE_COLLECTION : public MSG_COLLECT_PACKET
{
	u_short				usIndex;
	u_char				ucSlotIndex;
};

// en_collect_mode_result = 10
struct MSG_COLLECT_RESULT : public MSG_COLLECT_PACKET
{
	u_char				ucReqMode;		// � ��û�� ��� �ΰ�
	u_char				ucFail;			// 0:fail   1:success
};
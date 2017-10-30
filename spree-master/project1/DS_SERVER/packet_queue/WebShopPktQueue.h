#pragma once

#ifdef ITEMSHOP_PATCH_200607

//////////////////////////////////////////////////////////////////////////
// 2006.05.30 Molla, Nexon Web Shop
// safe-thread class
//////////////////////////////////////////////////////////////////////////

class CWebShopPktQueue {
public:
	enum QUEUE_RESULT {
		QRES_OK					= 1,

		QRES_NOT_INITIALIZED	= 0,	// ��ü�� �ʱ�ȭ ���� �ʾ���
		
		// push() �� ��
		QRES_OVERFLOW			= -100,	// overflow
		QRES_IMPROPER_PARAM		= -101, // �������� �Լ� �Ķ����
		
		// pop() �� ��
		QRES_NOT_ENOUGH_DATA	= -200	// pop() �� ��, ť ���ۿ� ����� ������ ����
	};

public:
	char* pBuf;

	int f; // front indicator
	int r; // rear indicator

	CRITICAL_SECTION csThis;

private:
	enum {
		QUEUE_BUF_SIZE = 819200
	};

public:
	CWebShopPktQueue();
	virtual ~CWebShopPktQueue();

	BOOL InitThis();
	void ReInitThis();

public:
	QUEUE_RESULT Push(IN const char* pData, IN int iDataSize);
	QUEUE_RESULT Pop(OUT char* pOutBuf);

protected:
	int _GetFreeSize();			// ť ���� ���� ���� ũ��
	int _GetFrontFreeSize();	// ť ���� ���� ���� ���� ũ��

	int _GetDataSize();
	int _GetFrontDataSize();

	// ��Ŷ Ÿ�Կ� ���� ������ ũ�� ���ϱ�
};

#endif //ITEMSHOP_PATCH_200607

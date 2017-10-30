#ifdef	_SMALL_NETWORK_

#ifndef	_PKTQUE_
#define _PKTQUE_

class CPktQue {
public:
	enum QUEUE_RESULT {
		QRES_OK							= 1,
		QRES_NOT_INITIALIZED		= 0,	// ��ü�� �ʱ�ȭ ���� �ʾ���

		// push() �� ��
		QRES_OVERFLOW				= -100,	// ���� ��ħ
		QRES_IMPROPER_PARAM	= -101, // �������� �Լ� �Ķ����

		// pop() �� ��
		QRES_NOT_ENOUGH_DATA	= -200,	// �ϳ� �̻��� ������ ��Ŷ ������ ����
		QRES_BROKEN_PKT			= -201	// ���Ἲ ���� ��Ŷ
	};

private:
	enum { 
		QUEUE_BUF_SIZE = 40960
	};

private:
	CS		cs; // for this
	char*	pBuf;
	int		f;	// front indicator
	int		r;	// rear indicator

public:
	CPktQue();
	~CPktQue();
	BOOL	Create();
	void	Init();	// reusable init

public:
	QUEUE_RESULT Push(IN CPacket* pPkt, IN int iDataSize);
	QUEUE_RESULT Pop(OUT CPacket* pPkt);

protected:
	int			_GetFreeSize();			// ���� ��ü�� ���� ���� ũ��
	int			_GetFrontFreeSize();	// ���� ������ ���� ���� ũ��, ����: f or r ~ ���۳�

	int			_GetDataSize();			// ���� ��ü�� ������ ũ��
	int			_GetFrontDataSize();	// ���� ������ ������ ũ��
};

#endif //_PKTQUE_

#endif //_SMALL_NETWORK_
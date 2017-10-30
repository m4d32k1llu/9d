#ifndef _XTIMER_HEADER
#define _XTIMER_HEADER


#define	MAX_TIMER_COUNT		20						// �ִ� timer ��


class _XTimer
{
private:	// ��� 
	float	m_thistime[MAX_TIMER_COUNT];			// ������ �ð��� �����Ѵ�.
	float	m_oldtime[MAX_TIMER_COUNT];				// ������ �������� ��µ� �ð��� �����Ѵ�.

	int		m_thisframe[MAX_TIMER_COUNT];			// ���� ������ 

	int		m_fps[MAX_TIMER_COUNT];					// ���� Frame Per Second





private:	// �Լ� 



public:		// �Լ�
	_XTimer();
	virtual ~_XTimer();



	// timer count
	void	Init(int tm);												// id���� �ʱ�ȭ �Ѵ�.
	void	Init_All();													// id���� �ʱ�ȭ �Ѵ�.
	void    UnInit(int tm);												// id���� �ִ밪���� �ʱ�ȭ �Ѵ�.
	void    UnInit_All();												// id���� �ִ밪���� �ʱ�ȭ �Ѵ�.

	BOOL	FrameSkip(int tm, float frame);								// ������ ��ŵ�Լ�

	int		GetFPS(int tm)			{	return 	this->m_fps[tm];	}	// fps�� ���´�.
	float	GetThisTime( int tm )	{	return  this->m_thistime[tm];}  

	void	Clear();													// info clear
	
};



#endif



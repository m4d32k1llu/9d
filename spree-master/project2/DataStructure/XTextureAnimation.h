// XTextureAnimation.h: interface for the XTextureAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTEXTUREANIMATION_H__635C32CA_5FCC_46D4_A317_4D37DD429511__INCLUDED_)
#define AFX_XTEXTUREANIMATION_H__635C32CA_5FCC_46D4_A317_4D37DD429511__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//breif : �ؽ��� ���ϸ��̼�
// �Թ��� ����... 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//1. �� �������� ���� ( �� ������ ���� ���� ������ �� �ٷ� ������ �� �� �������� ���� )
//2. �� �������� ���� ( �� ������ ���� ���� ������ �� �ٷ� ������ �� �� �������� ���� ) ���� ���� ������ �� �������� ����
//����, ������ ũ��� �����ؾ� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class _XTextureAnimation  
{
public:
	_XTextureAnimation( int left, int top, int right, int bottom, _XWindow* pWindow,_XTextureManager* ptexturemanager, int textureindex,
						unsigned int nNumOfFrameX, unsigned int nNumOfFrameY, unsigned int nAllOfFrame, 
						unsigned int nCurrentXpos = 0,unsigned int nCurrentYpos = 0	);
	~_XTextureAnimation();

	//���ϸ��̼� ����
	void Update( float fdealyTime, BOOL bReverse = FALSE );

	void SetAniLoopCount( int nLoop ); // ���ϸ��̼� �ݺ� Ƚ�� ����
	void SetDrawFirstAni( bool bRes ); // ���ϰ� ������ ù��° �ִϸ� �׸��� �Ҳ���
	void SetDrawEndAni(bool bDraw ); //�ִϰ� ������ �ȱ׸��� �׸���

	void InitFrame(); // ó�� ������ ��ġ��
	bool IsEndAnimate(); //�ִ� ���̳�

	void ShowWindow( BOOL show );
	void SetDrawPostion( int X, int Y );
	_XImageStatic* GetImageStatic();

private:
	//!!!�׷����� �̹��� ũ��!!! ������ ���� �̹��� ũ�Ⱑ �ƴ�, ȭ�鿡 �׷����� �̹��� ũ��, g_MainInterfaceTextureArchive, �ؽ��� ��ȣ
	bool Create( int left, int top, int right, int bottom, _XTextureManager* ptexturemanager, int textureindex );

	//Sprite, ���̵�� ���� ������..  ( ���̵�, x �� ������ ����, y�� ������ ����, �� ������ ����, �̹��� ���Ͽ��� ���� x, �̹��� ���Ͽ��� ���� y )
	void InitLocationA(	unsigned int nID, 
						unsigned int nNumOfFrameX, unsigned int nNumOfFrameY, 
						unsigned int nAllOfFrame, unsigned int nCurrentXpos,unsigned int nCurrentYpos );

	void SetWindow( _XWindow* pWindow );

	void Release();

	void NextFrame(); // ���ϸ��̼� ���� ���ϸ��̼��� ������ ó������ �ǵ�����
	void ReverseFrame(); // ������ ���� �ٽ� ���ư���..

private:
	//�ؽ��� ������
	_XImageStatic*	m_pImage;

	DWORD m_dwCurrentCheckTime; //����ð�

	bool m_bIsEndAnimate; // ���ϸ��̼��� ��������
	bool m_bDrawFirstAnimate; //���ϸ��̼��� ������(m_bIsEndAnimate) ù��° �ִϷ� ��������
	bool m_bDrawEndAnimate; //���� ���ϸ��̼� ��� �׸��� ����

	bool m_bReverse; //������ ����

	//object ID
	unsigned int m_nID;

	//���ϸ��̼� x,y ����
	unsigned int m_nNumOfFrameX;
	unsigned int m_nNumOfFrameY;

	unsigned int m_nNumberOfAnimateFrame; //��ü ���ϸ��̼� ����

	unsigned int m_nAniLoopCount;	//�ִϸ��̼� �� �ݺ� Ƚ��   ( 0 �̸� ������ )
	unsigned int m_nAniCurrentLoopCount; //���� ����� �ݺ� Ƚ��

	int m_nCurrentFrame; // ���� �׷����� �ִ� ������ ��ȣ

	//�� ������ �� �̹��� ũ��
	int m_nFrameImageSIzeWidth;
	int m_nFrameImageSizeHeight;

	//�̹��� ��ü ũ�� ( �׸����� �̹��� ����� �ƴ� )
	//������ ���ϸ��̼� ��ü�� �̹��� ������
	int	m_nImageTotalWidth;
	int	m_nImageTotalHeight;

	//��µ� �̹���, �׸� ���� ��ǥ RECT
	int	m_nCurrentXpos;
	int	m_nCurrentYpos;
	int	m_nCurrentEndXpos;
	int	m_nCurrentEndYpos;

	//ù �̹��� ��ǥ RECT
	int m_nStartXPos;
	int m_nStartYPos;
	int m_nEndXPos;
	int m_nEndYPos;

	//�׷����� �̹��� ���� RECT
	int	m_nCenterXpos;
	int	m_nCenterYpos;
};


//inline----------------------------------------------------------------------------------------------------------------
inline void _XTextureAnimation::SetAniLoopCount( int nLoop )
{
	m_nAniLoopCount = nLoop;
}

inline void _XTextureAnimation::SetDrawFirstAni( bool bRes )
{
	m_bDrawFirstAnimate = bRes;
}

inline void _XTextureAnimation::SetDrawEndAni( bool bDraw )
{
	m_bDrawEndAnimate = bDraw;
}

inline bool _XTextureAnimation::IsEndAnimate()
{ 
	return m_bIsEndAnimate; 
}

inline _XImageStatic* _XTextureAnimation::GetImageStatic()
{ 
	return m_pImage; 
}
//inline----------------------------------------------------------------------------------------------------------------

#endif // !defined(AFX_XTEXTUREANIMATION_H__635C32CA_5FCC_46D4_A317_4D37DD429511__INCLUDED_)

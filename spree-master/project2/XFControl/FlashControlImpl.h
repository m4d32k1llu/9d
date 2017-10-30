
#include "d3dx9.h"
#include "FlashControl.h" // CFlashControlInterface �� ����ִ� �������

#include "./win32/FSControl.h"

// CFlashControlInterfaceImpl �� CFlashControlInterface Ŭ������ virtual �Լ�����
// ������ ������ Ŭ�����̸�, ���������� NativePlayerWnd �� ����մϴ�.
class CFlashControlInterfaceImpl: public CFlashControlInterface
{
public:
	CFlashControlInterfaceImpl(){};
	~CFlashControlInterfaceImpl(){};

	NativePlayerWnd m_ctrPlayer;
	

public:
	virtual	void initialize( HWND hwnd, HINSTANCE hinst, LPDIRECT3DTEXTURE9	pOffScreenTexture );
	virtual	void SetOffScreenTextureObject( LPDIRECT3DTEXTURE9	pOffScreenTexture );
	virtual	void SetResourceFilePtr( FILE* pResourcePointer, long iResourceSize );
	virtual	HDC  GetNWindowDC() ;				// This is a call for the Flash code to use
	virtual	void SetDC( HDC dc ) ;  // Only for use by the Window Message Handler
	virtual	void PlayTimerFire() ;
	virtual BOOL UpdateCursor() ;	
	virtual	void CursorTimerFire() ;
	virtual void UpdateFullTexture() ;

	virtual void Repaint(RECT sr) ;
	virtual void HandleKeyDown(int key, int modifiers) ;
	virtual void MouseMove(int x, int y, BOOL mouseIsDown) ;
	virtual void ControlOpen(char* filename ) ;
	virtual void FreeBuffer() ;
	virtual void MouseUp(int x, int y ) ;
	virtual void MouseDown(int x, int y );

	virtual void ControlZoomIn();
	virtual void ControlZoomOut();
	virtual void ControlViewAll();
	virtual void ControlHighQuality();
	virtual void SetLowQuality();
	virtual void ControlPlay();
	virtual void ControlLoop();
	virtual void ControlRewind();
	virtual void ControlForward();
	virtual void ControlBack();
	virtual void ControlClose();

	virtual	BOOL IsPlaying() ;

	virtual const char*	 GetTargetName() { return m_ctrPlayer.m_strtargetname; }

};
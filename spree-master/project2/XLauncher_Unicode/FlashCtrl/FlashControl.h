#ifndef _FLASHCONTROLINTERFACE_
#define _FLASHCONTROLINTERFACE_

// Flash Control �� Ŭ������ ȣ���ϱ� ���� Virtual �Լ� ������ ( ���� 1 )
// ��.. 100���� ����??
#include "stdafx.h"

class CFlashControlInterface
{
public:
	CFlashControlInterface(){};
	virtual	~CFlashControlInterface(){};

public:
	// ������ ���� ��� �����Լ����� �����մϴ�.

	virtual	void initialize( HWND hwnd ) = 0;
	virtual	HDC  GetNWindowDC() = 0;				// This is a call for the Flash code to use
	virtual	void SetDC( HDC dc ) = 0;  // Only for use by the Window Message Handler
	virtual	void PlayTimerFire() = 0;
	virtual BOOL UpdateCursor() = 0;	
	virtual	void CursorTimerFire() = 0;

	virtual void Repaint(RECT sr) = 0;
	virtual void HandleKeyDown(int key, int modifiers) = 0;
	virtual void MouseMove(int x, int y, BOOL mouseIsDown) = 0;
	virtual void ControlOpen(char* filename ) = 0;
	virtual void FreeBuffer() = 0;
	virtual void MouseUp(int x, int y ) = 0;
	virtual void MouseDown(int x, int y ) = 0;

	// Context �޴��� ���� pure virtual �Լ���
	virtual void ControlZoomIn() = 0;
	virtual void ControlZoomOut() = 0;
	virtual void ControlViewAll() = 0;
	virtual void ControlHighQuality() = 0;
	virtual void SetLowQuality() = 0;
	virtual void ControlPlay() = 0;
	virtual void ControlLoop() = 0;
	virtual void ControlRewind() = 0;
	virtual void ControlForward() = 0;
	virtual void ControlBack() = 0;
	virtual void ControlClose() = 0;
};

extern CFlashControlInterface* GetFlashControl();

struct LOGPALETTE256 {
    WORD			palVersion;
    WORD			palNumEntries;
    PALETTEENTRY	palEntry[256];
};

#endif

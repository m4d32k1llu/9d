/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright � Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc. 
****************************************************************************/

/****************************************************************************
  Porting Version of Flash 4 SDK
  Middlesoft, Inc. October 1999
  Release 10/22/1999 Lee Thomason, lee@middlesoft.com 
****************************************************************************/

#include "..\\stdafx.h"
#include "..\\global.h"
#include "debug.h"
#include "..\\bitbuf.h"
#include "..\\sbitmap.h"
#include "display.h"
#include "palette.h"
#include "FSControl.h"

struct BITMAPINFO256 
{
   BITMAPINFOHEADER bmiHeader; 
   RGBQUAD          bmiColors[256]; 
};

struct LOGPALETTE256 {
    WORD			palVersion;
    WORD			palNumEntries;
    PALETTEENTRY	palEntry[256];
};


const int NativeDisplayTester::mapPixelFormatToDepth[] = {1,2,4,8,16,16,24,32,32};


int NativeDisplayTester::DepthToSimplestPixelFormat( int depth )
{	
	int pixelFormat;

	switch ( depth ) 
	{
		case 1:		pixelFormat = pix1;  break;
		case 4:		pixelFormat = pix4;  break;
		case 8:		pixelFormat = pix8;  break;
		case 16:	pixelFormat = pix16; break;
		case 24:	pixelFormat = pix24; break;
		case 32:	pixelFormat = pix32; break;
		default:
			FLASHASSERT(false);
	}
	return pixelFormat;
}


NativeDisplayTester::NativeDisplayTester()
{
	bestFormat = pix32; 
	bestInvert = true;
}


NativeDisplayTester::~NativeDisplayTester()
{
}


int NativeDisplayTester::CurrentScreenBitDepth()
{
	return 32;
}


U32 NativeDisplayTester::CurrentScreenColors()
{
	int depth;
	depth = CurrentScreenBitDepth();
	return ( 1 << depth );
}


void NativeDisplayTester::TestDepth( int depth )
{
	// There is so much more that can (and is done by the standard Macromedia player) 
	// be done here. Create a bitmap, blt some stuff around and see what works best.
	// This is the brain-dead implementation.

	bestFormat = DepthToSimplestPixelFormat( depth );

	// And we cheat! Less than 256 colors is set up to be 256 colors.
	if ( depth < 8 )
		bestFormat = pix8;
}



NativeBitmap::NativeBitmap( NativePlayerWnd* native,
							const SColorTable* table, 
							int _width, 
							int _height, 
							int _depth, 
							int _pixelFormat )
{
	U32 nColors;	
	LOGPALETTE256 logpal = { 0x300, 256	};

	FLASHOUTPUT( "NativeBitmap created\n" );

	width = ( _width + 3 ) & ( ~0x03 );	// increase the width to be byte-aligned
	height = _height;
	depth = _depth;
	format = _pixelFormat;
	nColors = NativeDisplayTester::CurrentScreenColors();

	bytesPerLine = ( width * depth + 7 ) / 8;

	if ( depth < 8 )
	{
		FLASHASSERT( 0 );	// not supported in this version....
	}
	else if ( depth == 8 )
	{
		for ( U32 i=0; i<nColors; i++ )
		{
			logpal.palEntry[i].peRed    = info.bmiColors[i].rgbRed   = table->colors[i].red;
			logpal.palEntry[i].peGreen  = info.bmiColors[i].rgbGreen = table->colors[i].green;
			logpal.palEntry[i].peBlue   = info.bmiColors[i].rgbBlue  = table->colors[i].blue;
										  
			info.bmiColors[i].rgbReserved = 0;
			
			if ( i >= 10 && i < 246 )
				logpal.palEntry[i].peFlags  = PC_NOCOLLAPSE;
			else
				logpal.palEntry[i].peFlags  = 0;
		}

		// Create a Palette object
		palette = ::CreatePalette((LOGPALETTE *)&logpal);
	}
	else
	{
		nColors = 0;
		palette = 0;
	}	
	
	memset( &info.bmiHeader, 0, sizeof( BITMAPINFOHEADER ) );
	info.bmiHeader.biSize = sizeof( BITMAPINFOHEADER ); // + nColors * sizeof( RGBQUAD );
	info.bmiHeader.biWidth = width;
	info.bmiHeader.biHeight = height;
	info.bmiHeader.biPlanes = 1;		// Must be 1. Thank you windows.
	info.bmiHeader.biBitCount = depth;	
	info.bmiHeader.biCompression = BI_RGB;	
	info.bmiHeader.biSizeImage = height * bytesPerLine;	
	info.bmiHeader.biClrUsed = nColors;	
	info.bmiHeader.biClrImportant = nColors;	
	
	HDC hdc = native->GetNWindowDC();

	hBitmap = CreateDIBSection( hdc, (BITMAPINFO*) &info, DIB_RGB_COLORS, (void**) &bits, 0, 0);
//  	HBITMAP hbmOld = (HBITMAP)::SelectObject(hdc, hBitmap);

	
	native->ReleaseWindowDC();

	FLASHASSERT( hBitmap );
}

NativeBitmap::~NativeBitmap()
{
	DeleteObject( hBitmap );
	if ( palette )
	{
		DeleteObject( palette );
	}
}
/*
void NativeBitmap::BltNativeToScreen( 	NativePlayerWnd* native,
										int xDest, int yDest, int cx, int cy, 
										int xSource, int ySource	)
{
	HDC hdc = native->GetNWindowDC();

	HPALETTE oldPalette = 0;

	if ( palette )
	{
 		oldPalette = SelectPalette( hdc, palette, FALSE );
		RealizePalette( hdc );
	}

	HDC mdc = CreateCompatibleDC( hdc );

 	HBITMAP hbmOld = (HBITMAP)::SelectObject(mdc, hBitmap);

// 	BITMAP bitmap;
// 	GetObject( hBitmap, sizeof( BITMAP ), &bitmap );
// 	
// 	PALETTEENTRY pal[256];
// 	GetPaletteEntries( palette, 0, 256, pal );
// 
//  RGBQUAD rgb[256];
// 	memset( rgb, 0, 256 * sizeof( RGBQUAD ) );
// 	count = GetDIBColorTable( mdc, 0, 256, rgb );
// 
// 	PALETTEENTRY	palEntry[256];
// 	GetSystemPaletteEntries( mdc, 0, 256, palEntry );

	BitBlt(  hdc, 
			 xDest, yDest,
			 cx, cy,
			 mdc,
			 xSource, ySource,
			 SRCCOPY);

// 	POINT point[5] = 
// 	{
// 		{	xDest,		yDest },
// 		{	xDest + cx,	yDest },
// 		{	xDest + cx,	yDest + cy },
// 		{	xDest,		yDest + cy },
// 		{	xDest,		yDest }
// 	};
// 
// 	Polyline( hdc, point, 5 );

	if ( oldPalette )
	{
		SelectPalette(hdc, oldPalette, TRUE);
	}

	::SelectObject(mdc, hbmOld);
	DeleteDC(mdc);

	native->ReleaseWindowDC();
}


void NativeBitmap::ClearScreen( NativePlayerWnd* native )
{	
	HDC hdc = native->GetNWindowDC();
	
	HDC mdc = CreateCompatibleDC( hdc );
	HBITMAP hbmOld = (HBITMAP)::SelectObject(mdc, hBitmap);
	BitBlt(  hdc, 
		0, 0,
		native->Width(), native->Height(),
		mdc,
		0, 0,
		WHITENESS);
	::SelectObject(mdc, hbmOld);
	DeleteDC(mdc);
	
	native->ReleaseWindowDC();
}
*/

//#define _XDEF_FULLTEXTURERECTUPDATE

void NativeBitmap::BltNativeToScreen( 	NativePlayerWnd* native,
									 int xDest, int yDest, int cx, int cy, 
									 int xSource, int ySource	)
{
	if( !native->m_pOffScreenTexture ) return;

	D3DLOCKED_RECT  lockrect;
	D3DSURFACE_DESC surfdesc;
	native->m_pOffScreenTexture->GetLevelDesc( 0, &surfdesc );
	HRESULT result = native->m_pOffScreenTexture->LockRect( 0, &lockrect, 0, 0 );

	if( xDest + cx >= (int)surfdesc.Width  )
		cx = surfdesc.Width - xDest;
	
	if( yDest + cy >= (int)surfdesc.Height  )
		cy = surfdesc.Height - yDest;

#ifdef _XDEF_FULLTEXTURERECTUPDATE
	if( SUCCEEDED( result ) )
	{
		LPBYTE pTarget = (LPBYTE)lockrect.pBits + ((surfdesc.Height-1) * lockrect.Pitch);
		LPBYTE pSource = (LPBYTE)bits;

		for( UINT i = 0; i < surfdesc.Height; ++i )
		{
			memcpy( pTarget, pSource, lockrect.Pitch );
			pTarget-=lockrect.Pitch;
			pSource+=bytesPerLine;
		}
	}
#else
	
	if( SUCCEEDED( result ) )
	{
		LPBYTE pTarget = ((LPBYTE)lockrect.pBits) + (xDest<<2) + ( yDest * lockrect.Pitch );
		LPBYTE pSource = ((LPBYTE)bits) + (xSource<<2) + ( (height - ySource - 1) * bytesPerLine );
		
		for( int i = 0; i < cy; ++i )
		{
			memcpy( pTarget, pSource, (cx<<2) );
			pTarget+=lockrect.Pitch;
			pSource-=bytesPerLine;
		}
	}
	
#endif

	native->m_pOffScreenTexture->UnlockRect(0);
}

void NativeBitmap::UpdateFullTexture(  NativePlayerWnd* native )
{
	if( !native->m_pOffScreenTexture ) return;
	
	D3DLOCKED_RECT  lockrect;
	D3DSURFACE_DESC surfdesc;
	native->m_pOffScreenTexture->GetLevelDesc( 0, &surfdesc );
	HRESULT result = native->m_pOffScreenTexture->LockRect( 0, &lockrect, 0, 0 );
		
	if( SUCCEEDED( result ) )
	{
		LPBYTE pTarget = (LPBYTE)lockrect.pBits + ((surfdesc.Height-1) * lockrect.Pitch);
		LPBYTE pSource = (LPBYTE)bits;
		
		for( UINT i = 0; i < surfdesc.Height; ++i )
		{
			memcpy( pTarget, pSource, lockrect.Pitch );
			pTarget-=lockrect.Pitch;
			pSource+=bytesPerLine;
		}
	}
	
	native->m_pOffScreenTexture->UnlockRect(0);
}

void NativeBitmap::ClearScreen( NativePlayerWnd* native )
{	
	if( !native->m_pOffScreenTexture ) return;

	D3DLOCKED_RECT  lockrect;
	D3DSURFACE_DESC surfdesc;
	native->m_pOffScreenTexture->GetLevelDesc( 0, &surfdesc );
	HRESULT result = native->m_pOffScreenTexture->LockRect( 0, &lockrect, 0, 0 );
	
	if( SUCCEEDED( result ) )
	{
		memset( lockrect.pBits, 0, lockrect.Pitch * surfdesc.Height );
	}
	
	native->m_pOffScreenTexture->UnlockRect(0);
}


#include <WindowsX.h>
#include <TChar.h>

#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/DirectX/DxDisplay.h>

#define  SafeRelease( obj )			{ if( obj != 0 ) obj->Release(); obj = 0; }

DxDisplay::DxDisplay()
	: directDraw( 0 ), frontSurface( 0 ), backSurface( 0 ), dxFrontSurface( 0 ), dxBackSurface( 0 ),
	handleWindow( 0 ), windowMode( false ), prevTime( 0 ), fps( 0.0f ), frameCount( 0 )
{
}

DxDisplay::~DxDisplay()
{
	destroyObjects();
}

void DxDisplay::destroyObjects()
{
	if( dxFrontSurface != 0 )
	{	
		delete dxFrontSurface;
		dxFrontSurface = 0;
	}
	if( dxBackSurface != 0 )
	{
		delete dxBackSurface;
		dxBackSurface = 0;
	}

	SafeRelease( backSurface );
	SafeRelease( frontSurface );

	if( directDraw != 0 )
	{
		directDraw->SetCooperativeLevel( handleWindow, DDSCL_NORMAL );

		SafeRelease( directDraw );
	}
}

bool DxDisplay::createFullScreenDisplay( HWND hWnd, DWORD width, DWORD height, DWORD bpp )
{
    HRESULT hResult;
    DDSURFACEDESC2 ddsd;
    DDSCAPS2 ddscaps;

	destroyObjects();

	handleWindow = hWnd;


	//  Create direct draw object
	{
		hResult = DirectDrawCreateEx( 0, ( void** )&directDraw, IID_IDirectDraw7, 0 );
		if( FAILED( hResult ) )
			return false;

		hResult = directDraw->SetCooperativeLevel( handleWindow, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
		if( FAILED( hResult ) )
			return false;

		hResult = directDraw->SetDisplayMode( width, height, bpp, 0, 0 );
		if( FAILED( hResult ) )
			return false;
	}


    //  Create primary surface( with backbuffer attached ).
	{
		::ZeroMemory( &ddsd, sizeof( ddsd ) );
		ddsd.dwSize            = sizeof( ddsd );
		ddsd.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
		ddsd.dwBackBufferCount = 1;

		hResult = directDraw->CreateSurface( &ddsd, &frontSurface, 0 );
		if( FAILED( hResult ) )
			return false;
	}


    //  Get a pointer to the back buffer
	{
		ZeroMemory( &ddscaps, sizeof( ddscaps ) );
		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;

		hResult = frontSurface->GetAttachedSurface( &ddscaps, &backSurface );
		if( FAILED( hResult ) )
			return false;

		backSurface->AddRef();
	}

	{
		if( dxFrontSurface != 0 )		delete dxFrontSurface;
		if( dxBackSurface != 0 )		delete dxBackSurface;

		dxFrontSurface = new DxSurface();
		dxFrontSurface->create( frontSurface );

		dxBackSurface = new DxSurface();
		dxBackSurface->create( backSurface );
	}

	screenWidth  = width;
	screenHeight = height;

	windowMode = false;
	updateBounds();

    return true;
}

bool DxDisplay::createWindowedDisplay( HWND hWnd, DWORD width, DWORD height )
{
	HRESULT hResult;
	LPDIRECTDRAWCLIPPER clipper;
	DDSURFACEDESC2 ddsd;

    destroyObjects();

	handleWindow = hWnd;

	{
		hResult = DirectDrawCreateEx( 0, ( void** )&directDraw, IID_IDirectDraw7, 0 );
		if( FAILED( hResult ) )
			return false;

		hResult = directDraw->SetCooperativeLevel( handleWindow, DDSCL_NORMAL );
		if( FAILED( hResult ) )
			return false;
	}


	//  Setting window style
	{
		RECT  rectWnd;
		RECT  rect;
		DWORD windowStyle;

		windowStyle  = GetWindowStyle( handleWindow );
		windowStyle &= ~WS_POPUP;
		windowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX;
		::SetWindowLong( hWnd, GWL_STYLE, windowStyle );

		::SetRect( &rect, 0, 0, width, height );

		::AdjustWindowRectEx( &rect, GetWindowStyle( handleWindow ),
					::GetMenu( handleWindow ) != 0, GetWindowExStyle( handleWindow ) );

		::SetWindowPos( handleWindow, 0, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
														SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );
		::SetWindowPos( handleWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

		::SystemParametersInfo( SPI_GETWORKAREA, 0, &rectWnd, 0 );
		::GetWindowRect( handleWindow, &rect );

		if( rect.left < rectWnd.left ) rect.left = rectWnd.left;
		if( rect.top  < rectWnd.top )  rect.top  = rectWnd.top;
		::SetWindowPos( handleWindow, 0, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
	}


    //  Create the primary surface
	{
	
		ZeroMemory( &ddsd, sizeof( ddsd ) );
		ddsd.dwSize         = sizeof( ddsd );
		ddsd.dwFlags        = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		if( FAILED( directDraw->CreateSurface( &ddsd, &frontSurface, 0 ) ) )
			return false;
	}

    //  Create the backbuffer surface
	{
		ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;    
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
		ddsd.dwWidth        = width;
		ddsd.dwHeight       = height;

		hResult = directDraw->CreateSurface( &ddsd, &backSurface, 0 );
		if( FAILED( hResult ) )
			return false;
	}


	//  Create clipper
	{
		hResult = directDraw->CreateClipper( 0, &clipper, 0 );
		if( FAILED( hResult ) )
			return false;

		hResult = clipper->SetHWnd( 0, handleWindow );
		if( FAILED( hResult ) )
		{
			clipper->Release();

			return false;
		}

		hResult = frontSurface->SetClipper( clipper );
		if( FAILED( hResult ) )
		{
			clipper->Release();

			return false;
		}

		clipper->Release();
	}

    {
		if( dxFrontSurface != 0 )		delete dxFrontSurface;
		if( dxBackSurface != 0 )		delete dxBackSurface;

		dxFrontSurface = new DxSurface();
		dxFrontSurface->create( frontSurface );

		dxBackSurface = new DxSurface();
		dxBackSurface->create( backSurface );
	}

	screenWidth  = width;
	screenHeight = height;

	windowMode = true;
    updateBounds();

    return true;
}

void DxDisplay::updateBounds()
{
	if( windowMode == true )
	{
		::GetClientRect( handleWindow, &rectWindow );

		::ClientToScreen( handleWindow, ( POINT* )&rectWindow );
		::ClientToScreen( handleWindow, ( POINT* )&rectWindow + 1 );
	}
	else
		::SetRect( &rectWindow, 0, 0, ::GetSystemMetrics( SM_CXSCREEN ), ::GetSystemMetrics( SM_CYSCREEN ) );
}

void DxDisplay::present()
{
    HRESULT hResult;
	DWORD fpsTime;

	if( frontSurface == 0 || backSurface == 0 )
		return;

	//  Calculating FPS
	{
		frameCount++;

		fpsTime = ::GetTickCount() - prevTime;
		if( fpsTime > 1000 )
		{
			fps = ( float )( 1000 * frameCount ) / ( float )fpsTime;
			prevTime = ::GetTickCount();
			frameCount = 0;
		}
	}

	while( 1 )
	{
		if( windowMode == true )
			hResult = frontSurface->Blt( &rectWindow, backSurface, 0, DDBLT_WAIT, 0 );
		else
			hResult = frontSurface->Flip( 0, 0 );

		if( hResult == DDERR_SURFACELOST )
		{
			frontSurface->Restore();
			backSurface->Restore();
		}

		if( hResult != DDERR_WASSTILLDRAWING )
			break;
	}
}

HRESULT DxDisplay::clear( DWORD color )
{
	if( backSurface == 0 )
		return E_POINTER;

	DDBLTFX fx;

	::ZeroMemory( &fx, sizeof( fx ) );
	fx.dwSize		= sizeof( fx );
	fx.dwFillColor	= color;

	return backSurface->Blt( 0, 0, 0, DDBLT_COLORFILL, &fx );
}

HRESULT DxDisplay::blt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 dds, RECT* rt, DWORD flags )
{
	if( backSurface == 0 )
		return E_POINTER;

	return backSurface->BltFast( x, y, dds, rt, flags );
}

HRESULT DxDisplay::blt( DWORD x, DWORD y, DxSurface* surface, RECT* rt )
{
    if( backSurface == 0 )
		return E_POINTER;

	//  Clipping
	RECT rect;
	int drawX, drawY;

	drawX = x;
	drawY = y;

	{
		int width, height;

		if( rt != 0 )
		{
			rect.left   = rt->left;
			rect.top    = rt->top;
			rect.right  = rt->right;
			rect.bottom = rt->bottom;
		}
		else
		{
			rect.left   = 0;
			rect.top    = 0;
			rect.right  = rect.left + surface->getWidth();
			rect.bottom = rect.top + surface->getHeight();
		}

		if( drawX < 0 )
		{
			rect.left += 0 - drawX;
			drawX = 0;
		}
		if( drawY < 0 )
		{
			rect.top += 0 - drawY;
			drawY = 0;
		}

		width  = rect.right - rect.left;// + 1;
		height = rect.bottom - rect.top;// + 1;

		if( drawX + width > screenWidth )
			rect.right -= drawX + width - screenWidth;
		if( drawY + height > screenHeight )
			rect.bottom -= drawY + height - screenHeight;
	}

	if( surface->usingColorKey() )
		return blt( drawX, drawY, surface->getSurface(), &rect, DDBLTFAST_SRCCOLORKEY );
	else
		return blt( drawX, drawY, surface->getSurface(), &rect, 0 );
}

HRESULT DxDisplay::colorKeyBlt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 dds, RECT* rt )
{
	if( backSurface == 0 )
		return E_POINTER;

	return backSurface->BltFast( x, y, dds, rt, DDBLTFAST_SRCCOLORKEY );
}

HRESULT DxDisplay::drawText( HFONT hFont, LPCTSTR string, DWORD x, DWORD y, bool transparent, COLORREF background, COLORREF foreground )
{
	if( dxBackSurface == 0 )
		return E_POINTER;

	return dxBackSurface->drawText( hFont, string, x, y, transparent, background, foreground );
}

bool DxDisplay::createSurface( DxSurface** surface, DWORD width, DWORD height )
{
	if( directDraw == 0 || surface == 0 )
		return false;

    HRESULT			hResult;
    DDSURFACEDESC2	ddsd;

    ::ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize         = sizeof( ddsd );
    ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT; 
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    ddsd.dwWidth        = width;
    ddsd.dwHeight       = height;

    *surface = new DxSurface();

	hResult = ( *surface )->create( directDraw, &ddsd );
	if( FAILED( hResult ) )
    {
        delete *surface;

        return false;
    }

	( *surface )->bitmapWidth  = ddsd.dwWidth;
	( *surface )->bitmapHeight = ddsd.dwHeight;

    return true;
}

bool DxDisplay::createSurfaceFromBitmap( DxSurface** surface, LPCTSTR fileName, DWORD desiredWidth, DWORD desiredHeight )
{
    HRESULT        hResult;
    HBITMAP        handleBitmap;
    BITMAP         bitmap;
    DDSURFACEDESC2 ddsd;

    if( directDraw == 0 || fileName == 0 || surface == 0 ) 
        return false;

    *surface = 0;

    handleBitmap = ( HBITMAP )::LoadImage( GetModuleHandle( 0 ), fileName, IMAGE_BITMAP, desiredWidth, desiredHeight,
																									LR_CREATEDIBSECTION );
    if( handleBitmap == 0 )
    {
        handleBitmap = ( HBITMAP )::LoadImage( 0, fileName, IMAGE_BITMAP, desiredWidth, desiredHeight, 
																					LR_LOADFROMFILE | LR_CREATEDIBSECTION );
        if( handleBitmap == 0 )
            return false;
    }

    ::GetObject( handleBitmap, sizeof( bitmap ), &bitmap );

    // Create a DirectDrawSurface for this bitmap
    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize         = sizeof( ddsd );
    ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    ddsd.dwWidth        = bitmap.bmWidth;
    ddsd.dwHeight       = bitmap.bmHeight;

    *surface = new DxSurface();

	hResult = ( *surface )->create( directDraw, &ddsd );
    if( FAILED( hResult ) )
    {
        delete *surface;

        return false;
    }

	( *surface )->bitmapWidth  = ddsd.dwWidth;
	( *surface )->bitmapHeight = ddsd.dwHeight;

    // Draw the bitmap on this surface
    if( FAILED( hResult = ( *surface )->drawBitmap( handleBitmap, 0, 0, 0, 0 ) ) )
    {
        ::DeleteObject( handleBitmap );

        return false;
    }

    ::DeleteObject( handleBitmap );

    return true;
}

bool DxDisplay::createSurfaceFromText( DxSurface** surface, HFONT hFont, LPCTSTR string, bool transparent, COLORREF background, COLORREF foreground )
{
    HDC                  hDC;
    HRESULT              hResult;
    DDSURFACEDESC2       ddsd;
    SIZE                 sizeText;

    if( directDraw == 0 || string == 0 || surface == 0 )
        return false;

    *surface = 0;

    hDC = GetDC( 0 );

    if( hFont != 0 )
        ::SelectObject( hDC, hFont );

    ::GetTextExtentPoint32( hDC, string, _tcslen( string ), &sizeText );
    ::ReleaseDC( 0, hDC );

    // Create a DirectDrawSurface for this bitmap
    ::ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize         = sizeof( ddsd );
    ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    ddsd.dwWidth        = sizeText.cx;
    ddsd.dwHeight       = sizeText.cy;

    *surface = new DxSurface();
    if( FAILED( hResult = ( *surface )->create( directDraw, &ddsd ) ) )
    {
        delete *surface;

        return false;
    }

    if( FAILED( hResult = ( *surface )->drawText( hFont, string, 0, 0, transparent, background, foreground ) ) )
        return false;

    return true;
}

bool DxDisplay::createSurfaceFromBackSurface( DxSurface** surface, RECT* rt )
{
	HRESULT				hResult;
	DDSURFACEDESC2		ddsd;

	*surface = new DxSurface();
	if( FAILED( hResult = ( *surface )->create( directDraw, &ddsd ) ) )
	{
		delete *surface;

		return false;
	}

	( *surface )->blt( 0, 0, backSurface, rt );

	return true;
}

HDC DxDisplay::getDC()
{
	HDC hDC;

	if( backSurface == 0 )
		return 0;

	backSurface->GetDC( &hDC );

	return hDC;
}

void DxDisplay::releaseDC( HDC hDC )
{
	if( backSurface != 0 )
		backSurface->ReleaseDC( hDC );
}
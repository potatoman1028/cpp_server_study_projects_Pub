#include <TChar.h>

#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/DirectX/DxSurface.h>

#define  SafeRelease( obj )			{ if( obj != 0 ) obj->Release(); obj; }

DxSurface::DxSurface()
	: surface( 0 ), useColorKey( false ), bitmapWidth( 0 ), bitmapHeight( 0 )
{
}

DxSurface::~DxSurface()
{
	destroy();
}

void DxSurface::destroy()
{
	SafeRelease( surface );
}

HRESULT DxSurface::create( LPDIRECTDRAWSURFACE7 dds )
{
	surface = dds;

    if( surface != 0 )
    {
		surface->AddRef();

		surfDDSD.dwSize = sizeof( surfDDSD );
		surface->GetSurfaceDesc( &surfDDSD );
    }

    return S_OK;
}

HRESULT DxSurface::create( LPDIRECTDRAW7 directDraw, DDSURFACEDESC2* ddsd )
{
    HRESULT hResult;

	hResult = directDraw->CreateSurface( ddsd, &surface, 0 );
	if( FAILED( hResult ) )
		return hResult;

	surfDDSD.dwSize = sizeof( surfDDSD );
	surface->GetSurfaceDesc( &surfDDSD );

    return S_OK;
}

HRESULT DxSurface::drawBitmap( HBITMAP handleBitmap, DWORD x, DWORD y, DWORD width, DWORD height )
{
    HDC            hDCImage;
    HDC            hDC;
    BITMAP         bitmap;
    DDSURFACEDESC2 ddsd;
    HRESULT        hResult;

    if( handleBitmap == 0 || surface == 0 )
        return E_INVALIDARG;

	hResult = surface->Restore();
    if( FAILED( hResult ) )
        return hResult;


	{
		ddsd.dwSize  = sizeof( ddsd );
		surface->GetSurfaceDesc( &ddsd );

		if( ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC )
			return E_NOTIMPL;
	}


    hDCImage = ::CreateCompatibleDC( 0 );
    if( hDCImage == 0 )
        return E_FAIL;

    ::SelectObject( hDCImage, handleBitmap );
    ::GetObject( handleBitmap, sizeof( bitmap ), &bitmap );


    width  = ( width  == 0 ? bitmap.bmWidth  : width );    
    height = ( height == 0 ? bitmap.bmHeight : height );

	hResult = surface->GetDC( &hDC );
    if( FAILED( hResult ) )
        return hResult;

    ::StretchBlt( hDC, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hDCImage, x, y, width, height, SRCCOPY );

	hResult = surface->ReleaseDC( hDC );
    if( FAILED( hResult ) )
        return hResult;

    ::DeleteDC( hDCImage );

    return S_OK;
}

HRESULT DxSurface::drawText( HFONT hFont, LPCTSTR string, DWORD x, DWORD y, bool transparent, COLORREF background, COLORREF foreground )
{
    HDC     hDC;
    HRESULT hResult;

    if( surface == 0 || string == 0 )
        return E_INVALIDARG;

    // Make sure this surface is restored.
	hResult = surface->Restore();
    if( FAILED( hResult ) )
        return hResult;

	hResult = surface->GetDC( &hDC );
    if( FAILED( hResult ) )
        return hResult;

    // Set the background and foreground color
    ::SetBkColor( hDC, background );
    ::SetTextColor( hDC, foreground );

	if( transparent == true )
		::SetBkMode( hDC, TRANSPARENT );

    if( hFont != 0 )
        ::SelectObject( hDC, hFont );

    // Use GDI to draw the text on the surface
    ::TextOut( hDC, x, y, string, _tcslen( string ) );

	hResult = surface->ReleaseDC( hDC );
    if( FAILED( hResult ) )
        return hResult;

    return S_OK;
}

HRESULT DxSurface::setColorKey( DWORD colorKey )
{
    if( surface == 0 )
        return E_POINTER;

    useColorKey = true;

    DDCOLORKEY ddck;

    ddck.dwColorSpaceLowValue  = convertGDIColor( colorKey );
    ddck.dwColorSpaceHighValue = convertGDIColor( colorKey );
    
    return surface->SetColorKey( DDCKEY_SRCBLT, &ddck );
}

DWORD DxSurface::convertGDIColor( COLORREF GDIColor )
{
    if( surface == 0 )
	    return 0;

    COLORREF       rgbT;
    HDC            hDC;
    DWORD          dw = CLR_INVALID;
    DDSURFACEDESC2 ddsd;
    HRESULT        hResult;

    if( GDIColor != CLR_INVALID && surface->GetDC( &hDC ) == DD_OK )
    {
        rgbT = ::GetPixel( hDC, 0, 0 );
        ::SetPixel( hDC, 0, 0, GDIColor );

        surface->ReleaseDC( hDC );
    }

    ddsd.dwSize = sizeof( ddsd );
    hResult = surface->Lock( 0, &ddsd, DDLOCK_WAIT, 0 );
    if( hResult == DD_OK )
    {
        dw = *( DWORD * )ddsd.lpSurface; 
        if( ddsd.ddpfPixelFormat.dwRGBBitCount < 32 )
            dw &= ( 1 << ddsd.ddpfPixelFormat.dwRGBBitCount ) - 1;  

        surface->Unlock( 0 );
    }

    if( GDIColor != CLR_INVALID && surface->GetDC( &hDC ) == DD_OK )
    {
        ::SetPixel( hDC, 0, 0, rgbT );
        surface->ReleaseDC( hDC );
    }
    
    return dw;    
}

HDC DxSurface::getDC()
{
	HDC hDC;

	if( surface == 0 )
		return 0;

	surface->GetDC( &hDC );

	return hDC;
}

void DxSurface::releaseDC( HDC hDC )
{
	if( surface != 0 )
		surface->ReleaseDC( hDC );
}


HRESULT DxSurface::blt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 dds, RECT* rt, DWORD flags )
{
	if( surface == 0 )
		return E_POINTER;

	return surface->BltFast( x, y, dds, rt, flags );
}

HRESULT DxSurface::blt( DWORD x, DWORD y, DxSurface* surf, RECT* rt )
{
    if( surface == 0 )
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
			rect.right  = rect.left + surf->getWidth();
			rect.bottom = rect.top + surf->getHeight();
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

		if( drawX + width > getWidth() )
			rect.right -= drawX + width - getWidth();
		if( drawY + height > getHeight() )
			rect.bottom -= drawY + height - getHeight();
	}

	if( surf->usingColorKey() )
		return blt( drawX, drawY, surf->getSurface(), &rect, DDBLTFAST_SRCCOLORKEY );
	else
		return blt( drawX, drawY, surf->getSurface(), &rect, 0 );
}

HRESULT DxSurface::colorKeyBlt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 dds, RECT* rt )
{
	if( surface == 0 )
		return E_POINTER;

	return surface->BltFast( x, y, dds, rt, DDBLTFAST_SRCCOLORKEY );
}
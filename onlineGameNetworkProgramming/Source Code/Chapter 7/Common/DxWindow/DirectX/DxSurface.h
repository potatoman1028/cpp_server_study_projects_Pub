#ifndef  __DxSurface_H
#define  __DxSurface_H

#include <DDraw.h>

class DxSurface
{
	friend class DxDisplay;

public:
			DxSurface();
	virtual	~DxSurface();

			LPDIRECTDRAWSURFACE7	getSurface(){ return surface; }
			BOOL					usingColorKey(){ return useColorKey; }

			HRESULT					create( LPDIRECTDRAWSURFACE7 dds );
			HRESULT					create( LPDIRECTDRAW7 directDraw, DDSURFACEDESC2* ddsd );

			HRESULT					drawBitmap( HBITMAP handleBitmap, DWORD x = 0, DWORD y = 0, DWORD width = 0, DWORD height = 0 );
			HRESULT					drawText( HFONT hFont, LPCTSTR string, DWORD x, DWORD y,
																bool transparent = true, COLORREF background = 0, COLORREF foreground = 0 );

			HRESULT					setColorKey( DWORD colorKey );
			DWORD					convertGDIColor( COLORREF GDIColor );

			HRESULT					blt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 dds, RECT* rt = 0, DWORD flags = 0 );
			HRESULT					blt( DWORD x, DWORD y, DxSurface* surf, RECT* rt = 0 );
			HRESULT					colorKeyBlt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 dds, RECT* rt = 0 );

			void					destroy();

			HDC						getDC();
			void					releaseDC( HDC hDC );

 			int						getWidth(){ return bitmapWidth; }
			int						getHeight(){ return bitmapHeight; }

private:
			LPDIRECTDRAWSURFACE7	surface;
			DDSURFACEDESC2			surfDDSD;
			BOOL					useColorKey;

			int						bitmapWidth, bitmapHeight;
};

#endif
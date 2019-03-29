#ifndef  __DxDisplay_H
#define  __DxDisplay_H

#include <Common/DxWindow/DirectX/DxSurface.h>

class DxDisplay
{
public:
			DxDisplay();
	virtual	~DxDisplay();

			void					destroyObjects();

			//  Status functions
			HWND					getHWnd()			{ return handleWindow; }
			LPDIRECTDRAW7			getDirectDraw()		{ return directDraw; }

			DxSurface*				getFrontSurface()	{ return dxFrontSurface; }
			DxSurface*				getBackSurface()	{ return dxBackSurface; }

			bool					isWindowMode()		{ return windowMode; }

			//  Creation/destruction methods
			bool					createFullScreenDisplay( HWND hWnd, DWORD width, DWORD height, DWORD bpp );
			bool					createWindowedDisplay( HWND hWnd, DWORD width, DWORD height );
			void					updateBounds();

			//  Methods to create child objects
			bool					createSurface( DxSurface** surface, DWORD width, DWORD height );
			bool					createSurfaceFromBitmap( DxSurface** surface, LPCTSTR fileName, DWORD desiredWidth, DWORD desiredHeight );
			bool					createSurfaceFromText( DxSurface** surface, HFONT hFont, LPCTSTR string,
															bool transparent = true, COLORREF background = 0, COLORREF foreground = 0 );
			bool					createSurfaceFromBackSurface( DxSurface** surface, RECT* rt = 0 );

			//  Display methods
			HRESULT					clear( DWORD color = 0 );

			HRESULT					blt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 dds, RECT* rt = 0, DWORD dwFlags = 0 );
			HRESULT					blt( DWORD x, DWORD y, DxSurface* surface, RECT* rt = 0 );
			HRESULT					colorKeyBlt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 dds, RECT* rt = 0 );
			HRESULT					drawText( HFONT hFont, LPCTSTR string, DWORD x, DWORD y,
															bool transparent = true, COLORREF background = 0, COLORREF foreground = 0 );

			void					present();

			HDC						getDC();
			void					releaseDC( HDC hDC );


			//  FPS methods
			float					getFPS(){ return fps; }

private:
			LPDIRECTDRAW7			directDraw;
			LPDIRECTDRAWSURFACE7	frontSurface;
			LPDIRECTDRAWSURFACE7	backSurface;

			DxSurface*				dxFrontSurface;
			DxSurface*				dxBackSurface;

			int						screenWidth;
			int						screenHeight;

			HWND					handleWindow;
			RECT					rectWindow;
			bool					windowMode;


			DWORD					frameCount;
			DWORD					prevTime;
			float					fps;
};

#endif

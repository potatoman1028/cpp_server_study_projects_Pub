#ifndef  __IWindowCtrl_H
#define  __IWindowCtrl_H

#include <Common/DxWindow/Windows/FrameView.h>

enum ControlReaction
{
	CR_DOWN,
	CR_LISTCTRL_SELECTEDITEM
};

class IWindowCtrl : public FrameView
{
public:
			IWindowCtrl( DWORD id, DxDisplay* disp );
	virtual	~IWindowCtrl();

	virtual bool		create( LPCTSTR fileName, int x, int y, DWORD attr, Window* parent );

			DWORD		getControlID(){ return cID; }

private:
			DWORD		cID;
};

#endif

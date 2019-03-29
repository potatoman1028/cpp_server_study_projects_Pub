#ifndef  __DlgLogin_H
#define  __DlgLogin_H

#include <Common/DxWindow/Windows/FrameView.h>
#include <Common/DxWindow/Windows/Controls/ButtonCtrl.h>
#include <Common/DxWindow/Windows/Controls/EditBoxCtrl.h>


#define  WM_DLGLOGIN_CONNECT			100
#define  WM_DLGLOGIN_EXIT				101


class DlgLogin : public FrameView
{
public:
			DlgLogin( DxDisplay* disp );
	virtual	~DlgLogin();

	virtual	void			onCreate();

	virtual	void			onDraw();
	virtual	bool			process();

			CString			getAccount();

protected:
	virtual	void			parseMessage( DWORD message, WPARAM wParam, LPARAM lParam );

private:
			ButtonCtrl*		btnConnect;
			ButtonCtrl*		btnExit;
			EditBoxCtrl*	editAccount;
};

#endif
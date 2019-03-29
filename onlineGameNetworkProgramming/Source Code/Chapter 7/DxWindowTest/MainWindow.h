#ifndef  __MainWindow_H
#define  __MainWindow_H

#include <Common/DxWindow/Windows/FrameView.h>
#include <Common/DxWindow/Windows/Controls/ListCtrl.h>
#include <Common/DxWindow/Windows/Controls/EditBoxCtrl.h>

class MainWindow : public FrameView
{
public:
			MainWindow( HWND hwnd, DxDisplay* disp );
	virtual	~MainWindow();

	virtual	void			onCreate();
	virtual	void			onDestroy();

	virtual	void			preTranslateMessage( MSG* msg );
	virtual	bool			process();

protected:
	virtual	void			onDraw();
	virtual void			parseMessage( DWORD message, WPARAM wParam, LPARAM lParam );


private:
			HWND			hwndMainFrm;

			ListCtrl*		ctrlList;
			EditBoxCtrl*	ctrlEdit;
			ButtonCtrl*		btnExit;


			Window*			wndRed;
			Window*			wndGreen;
			Window*			wndYellow;

			bool			exitProgram;
};

#endif
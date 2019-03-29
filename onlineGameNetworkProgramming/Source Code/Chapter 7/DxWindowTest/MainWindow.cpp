#include "stdafx.h"
#include "DxWindowTest.h"
#include "MainWindow.h"

#define  CID_LISTBOX			1000
#define  CID_EDITBOX			1001
#define  CID_BUTTON_EXIT		1002

MainWindow::MainWindow( HWND hwnd, DxDisplay* disp )
	: FrameView( disp ), hwndMainFrm( hwnd ), exitProgram( false )
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::onCreate()
{
	FrameView::onCreate();

	//  ListCtrl�� �����Ѵ�.
	ctrlList = new ListCtrl( CID_LISTBOX, display );
	ctrlList->create( _T( "Data/ListBack.bmp" ), _T( "Data/ListSelectBar.bmp" ), 14, 14,
								Window::Attr_ShowWindow, this );
	ctrlList->createScrollBar( _T( "Data/ScrollBack.bmp" ), _T( "Data/ScrollBar.bmp" ),
								_T( "Data/ScrollDec.bmp" ), _T( "Data/ScrollInc.bmp" ), ScrollBarCtrl::SB_Vertical );


	//  EditBoxCtrl�� �����Ѵ�.
	ctrlEdit = new EditBoxCtrl( CID_EDITBOX, display );
	ctrlEdit->create( _T( "Data/EditBox.bmp" ), 14, 168, this,
		FontCatalog::handleFont[Font_Medium_Normal], -1, _T( "Data/CaretEng.bmp" ), _T( "Data/CaretHan.bmp" ) );
	ctrlEdit->setTextColor( RGB( 0, 0, 0 ) );
	ctrlEdit->setIMEWIndowHandle( hwndMainFrm );


	//  Exit ButtonCtrl�� �����Ѵ�.
	btnExit = new ButtonCtrl( CID_BUTTON_EXIT, display );
	btnExit->create( _T( "Data/BtnExit.bmp" ), 14, 196, Window::Attr_ShowWindow, this );



	//  3 ���� �����츦 �����Ѵ�.
	wndRed = new Window( display );
	wndRed->create( _T( "Data/WndRed.bmp" ), 400, 50, Window::Attr_ShowWindow | Window::Attr_Movable, this );

	wndGreen = new Window( display );
	wndGreen->create( _T( "Data/WndGreen.bmp" ), 450, 100, Window::Attr_ShowWindow | Window::Attr_Movable, this );

	wndYellow = new Window( display );
	wndYellow->create( _T( "Data/WndYellow.bmp" ), 500, 150, Window::Attr_ShowWindow | Window::Attr_Movable, this );
}

void MainWindow::onDestroy()
{
	FrameView::onDestroy();
}

void MainWindow::onDraw()
{
	FrameView::onDraw();
}

void MainWindow::preTranslateMessage( MSG* msg )
{
	if( msg->message == WM_KEYDOWN )
	{
		if( msg->wParam == VK_RETURN && ctrlEdit->getString().IsEmpty() == FALSE )
		{
			//  EditBox�� �Է��� ���ڿ��� ListCtrl�� �߰��Ѵ�.
			ctrlList->addString( ctrlEdit->getString() );
			ctrlEdit->empty();
		}
	}

	FrameView::preTranslateMessage( msg );
}

bool MainWindow::process()
{
	if( FrameView::process() == false )
		return false;

	if( exitProgram == true )
		return false;

	ctrlEdit->setFocus();

	return true;
}

void MainWindow::parseMessage( DWORD message, WPARAM wParam, LPARAM lParam )
{
	//  CR_DOWN �޽����� �ƴ϶�� �����Ѵ�.
	if( message != CR_DOWN )
		return;

	//  Exit button�� ������ ��� ���α׷��� �����Ѵ�.
	if( wParam == CID_BUTTON_EXIT )			exitProgram = true;
}

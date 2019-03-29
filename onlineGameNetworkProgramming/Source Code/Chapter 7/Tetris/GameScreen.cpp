#include "stdafx.h"
#include "Tetris.h"
#include "GameScreen.h"
#include "Protocol.h"

ClientSocket*		GameScreen::clientSocket = 0;

GameScreen::GameScreen( HWND hwnd, DxDisplay* disp, ClientSocket* cs )
	: FrameView( disp ), exitGame( false ), handleMainFrm( hwnd )
{
	clientSocket = cs;
}

GameScreen::~GameScreen()
{
}

void GameScreen::onCreate()
{
	FrameView::onCreate();

	{
		display->createSurfaceFromBitmap( &surfLargeBlock, _T( "Data/LargeBlock.bmp" ), 0, 0 );
		display->createSurfaceFromBitmap( &surfSmallBlock, _T( "Data/SmallBlock.bmp" ), 0, 0 );

		display->createSurfaceFromBitmap( &surfLoseSmall, _T( "Data/LoseSmall.bmp" ), 0, 0 );
		surfLoseSmall->setColorKey( 0 );
		display->createSurfaceFromBitmap( &surfLoseLarge, _T( "Data/LoseLarge.bmp" ), 0, 0 );
		surfLoseLarge->setColorKey( 0 );
	}

	{
		ctrlReadyCheck = new CheckBoxCtrl( 0, display );
		ctrlReadyCheck->create( _T( "Data/checkReady.bmp" ), 400, 150, Window::Attr_ShowWindow, this );
		ctrlReadyCheck->enable( false );

		btnStart = new ButtonCtrl( 1, display );
		btnStart->create( _T( "Data/btnStart.bmp" ), 400, 220, Window::Attr_ShowWindow, this );
		btnStart->enable( false );

		editChatMessage = new EditBoxCtrl( 2, display );
		editChatMessage->create( _T( "Data/EditBoxBack.bmp" ),
							526, 109, this, 0, -1, _T( "Data/CaretEng.bmp" ), _T( "Data/CaretHan.bmp" ) );
		editChatMessage->setIMEWIndowHandle( handleMainFrm );

		dlgLogin = new DlgLogin( display );
		dlgLogin->create( _T( "Data/LoginBackground.bmp" ), 290, 200, Window::Attr_ShowWindow | Window::Attr_Movable, this );
	}

	pressedKey = false;
	blockDrop = false;
	playGame = false;

	myBoardNo = 0;
}

void GameScreen::onDestroy()
{
	FrameView::onDestroy();

	delete surfLargeBlock;
	delete surfSmallBlock;
	delete surfLoseSmall;
	delete surfLoseLarge;

	while( listChatMessage.GetCount() > 0 )
		delete listChatMessage.RemoveHead();
}

void GameScreen::onSocketConnect( WPARAM wParam, LPARAM lParam )
{
}

void GameScreen::onSocketClose( WPARAM wParam, LPARAM lParam )
{
}

void GameScreen::packetParsing( Packet& packet )
{
	switch( packet.id() )
	{
	case  GS_CONNECTIONSUCCESS_ACK :	onGSConnectionSuccessAck( packet );		break;
	case  GG_ENTERGAMEROOM_ACK :		onGGEnterGameRoomAck( packet );			break;
	case  GG_USERLIST_ACK :				onGGUserListAck( packet );				break;
	case  GG_LEAVEGAMEROOM_ACK :		onGGLeaveGameRoomAck( packet );			break;
	case  GG_CHATMESSAGE_ACK :			onGGChatMessageAck( packet );			break;

	case  GG_READY_ACK :				onGGReadyAck( packet );					break;
	case  GG_STARTGAME_ACK :			onGGStartGameAck( packet );				break;
	case  GG_NEWBLOCK_ACK :				onGGNewBlockAck( packet );				break;
	case  GG_MOVEBLOCK_ACK :			onGGMoveBlockAck( packet );				break;
	case  GG_FIXBLOCK_ACK :				onGGFixBlockAck( packet );				break;
	case  GG_GAMEOVER_ACK :				onGGGameOverAck( packet );				break;
	}
}

void GameScreen::parseMessage( DWORD message, WPARAM wParam, LPARAM lParam )
{
	if( message == WM_DLGLOGIN_CONNECT )
	{
		dlgLogin->showWindow( false );
		clientSocket->connectTo( "127.0.0.1", 9630 );
	}
	if( message == WM_DLGLOGIN_EXIT )
	{
		exitGame = true;
	}

	if( message == CR_DOWN )
	{
		if( wParam == 0 )		//  Ready check box
		{
			Packet sendPacket( GG_READY_REQ );

			sendPacket << ctrlReadyCheck->isChecked();
			clientSocket->sendPacket( sendPacket );
		}
		if( wParam == 1 )		//  Start game button
		{
			Packet sendPacket( GG_STARTGAME_REQ );

			clientSocket->sendPacket( sendPacket );
		}
	}
}

void GameScreen::preTranslateMessage( MSG* msg )
{
	FrameView::preTranslateMessage( msg );

	if( msg->message == WM_KEYDOWN )
	{
		if( msg->wParam == VK_RETURN )
		{
			Packet sendPacket( GG_CHATMESSAGE_REQ );

			sendPacket << editChatMessage->getString().GetBuffer( 0 );
			clientSocket->sendPacket( sendPacket );

			editChatMessage->empty();
		}
	}
}

bool GameScreen::process()
{
	if( exitGame == true )
		return false;

	if( FrameView::process() == false )
		return false;

	if( playGame == true && userList[myBoardNo].gameOver == false )
	{
		processInkey();
		userList[myBoardNo].board.run();
	}

	if( dlgLogin->isShowWindow() == false )
		editChatMessage->setFocus();

	return true;
}

void GameScreen::onDraw()
{
	FrameView::onDraw();

	drawBoard();
	if( playGame == true )
		drawBlock();

	//  Output chat message
	{
		POSITION pos;
		int count = 0;

		pos = listChatMessage.GetHeadPosition();
		while( pos != 0 )
		{
			char* account = listChatMessage.GetNext( pos );
			char* message = listChatMessage.GetNext( pos );

			drawText( Font_Medium_Normal, account, 533, 28 + count * 16, RGB( 255, 200, 150 ) );
			drawText( Font_Medium_Normal, message, 600, 28 + count * 16, RGB( 150, 255, 200 ) );

			count++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
////////////////////////////  Packet parsing  ////////////////////////////

void GameScreen::onGSConnectionSuccessAck( Packet& packet )
{
	Packet sendPacket( GG_ENTERGAMEROOM_REQ );

	sendPacket << dlgLogin->getAccount().GetBuffer( 0 );
	clientSocket->sendPacket( sendPacket );

	ctrlReadyCheck->enable( true );
	btnStart->enable( true );
}

void GameScreen::onGGEnterGameRoomAck( Packet& packet )
{
	char account[127];
	bool self;
	int i;

	packet >> self >> account;

	if( self == true )
	{
		Packet sendPacket( GG_USERLIST_REQ );

		clientSocket->sendPacket( sendPacket );

		userList[myBoardNo].account = account;
	}
	else
	{
		for( i = 0 ; i < 5 ; i++ )
		{
			if( userList[i].account.IsEmpty() == TRUE )
			{
				userList[i].account = account;
				break;
			}
		}
	}
}

void GameScreen::onGGUserListAck( Packet& packet )
{
	char account[127];
	int count, i, j;

	packet >> count;

	for( i = 0 ; i < count ; i++ )
	{
		packet >> account;

		if( userList[myBoardNo].account != account )
		{
			for( j = 0 ; j < 5 ; j++ )
			{
				if( userList[j].account.IsEmpty() == TRUE )
				{
					userList[j].account = account;
					break;
				}
			}
		}
	}
}

void GameScreen::onGGLeaveGameRoomAck( Packet& packet )
{
	char account[127];
	int i;

	packet >> account;

	for( i = 0 ; i < 5 ; i++ )
	{
		if( userList[i].account == account )
		{
			userList[i].ready = false;
			userList[i].account.Empty();
		}
	}
}

void GameScreen::onGGChatMessageAck( Packet& packet )
{
	char* account = new char [127];
	char* message = new char [127];

	packet >> account >> message;

	listChatMessage.AddTail( account );
	listChatMessage.AddTail( message );

	if( listChatMessage.GetCount() > 8 )
	{
		delete listChatMessage.RemoveHead();
		delete listChatMessage.RemoveHead();
	}
}

void GameScreen::onGGReadyAck( Packet& packet )
{
	char account[127];
	bool ready;
	int i;

	packet >> account >> ready;
	for( i = 0 ; i < 5 ; i++ )
	{
		if( userList[i].account == account )
		{
			userList[i].ready = ready;
			break;
		}
	}
}

void GameScreen::onGGStartGameAck( Packet& packet )
{
	packet >> playGame;
	if( playGame == true )
	{
		playGame = true;

		int i, seed;

		packet >> seed;
		for( i = 0 ; i < 5 ; i++ )
			userList[i].board.initialize( seed );
	}
}

void GameScreen::onGGNewBlockAck( Packet& packet )
{
	char account[127];
	int no, i;

	packet >> account >> no;
	for( i = 0 ; i < 5 ; i++ )
	{
		if( userList[i].account == account )
		{
			userList[i].board.newBlock( no );
			break;
		}
	}
}

void GameScreen::onGGMoveBlockAck( Packet& packet )
{
	char account[127];
	int x, y, dir, i;

	packet >> account >> x >> y >> dir;
	for( i = 0 ; i < 5 ; i++ )
	{
		if( userList[i].account == account )
		{
			userList[i].board.moveBlockAt( x, y, dir );
			break;
		}
	}
}

void GameScreen::onGGFixBlockAck( Packet& packet )
{
	char account[127];
	int x, y, dir, i;

	packet >> account >> x >> y >> dir;
	for( i = 0 ; i < 5 ; i++ )
	{
		if( userList[i].account == account )
		{
			userList[i].score += userList[i].board.fixBlock( x, y, dir );
			break;
		}
	}
}

void GameScreen::onGGGameOverAck( Packet& packet )
{
	char account[127];
	int i;

	packet >> account;
	for( i = 0 ; i < 5 ; i++ )
	{
		if( userList[i].account == account )
		{
			userList[i].gameOver = true;
			break;
		}
	}
}


///////////////////////////////////////////////////////////////////////////
////////////////////////////  Game processing  ////////////////////////////

void GameScreen::drawBoard()
{
	POINT ptAccount[4] = { { 90, 65 }, { 310, 65 }, { 90, 330 }, { 310, 330 } };
	POINT ptBoard[4] = { { 52, 112 }, { 269, 112 }, { 52, 375 }, { 269, 375 } };
	RECT rt = { 0, 0, 0, 0 };
	int info[BOARD_HEIGHT][BOARD_WIDTH];
	int i, j, k, count = 0;
	CString score;

	k = 0;
	for( i = 0 ; i < 5 ; i++ )
	{
		if( userList[i].account.IsEmpty() == TRUE )
			continue;

		score.Format( _T( "%d" ), userList[i].score );
		if( i == myBoardNo )
		{
			drawText( Font_Medium_Bold, userList[i].account, 570, 143, RGB( 0, 0, 0 ) );
			drawText( Font_Medium_Bold, score, 570, 163, RGB( 0, 0, 0 ) );
		}
		else
		{
			drawText( Font_Medium_Bold, userList[i].account, ptAccount[k].x, ptAccount[k].y, RGB( 0, 0, 0 ) );
			drawText( Font_Medium_Bold, score, ptAccount[k].x, ptAccount[k].y + 20, RGB( 0, 0, 0 ) );
			k++;
		}
	}

	for( k = 0 ; k < 5 ; k++ )
	{
		if( userList[k].account.IsEmpty() == TRUE )
			continue;

		userList[k].board.getBoard( info );
		for( j = 0 ; j < BOARD_HEIGHT - 1 ; j++ )
		{
			for( i = 1 ; i < BOARD_WIDTH - 1 ; i++ )
			{
				if( info[j][i] != 0 )
				{
					if( k == myBoardNo )
					{
						rt.left    = ( info[j][i] - 1 ) * 16;
						rt.right   = rt.left + 16;
						rt.bottom  = 16;

						display->blt( 534 + i * 16, 198 + j * 16, surfLargeBlock, &rt );
					}
					else
					{
						rt.left    = ( info[j][i] - 1 ) * 8;
						rt.right   = rt.left + 8;
						rt.bottom  = 8;

						display->blt( ptBoard[count].x + i* 8, ptBoard[count].y + j * 8, surfSmallBlock, &rt );
					}
				}
			}
		}

		if( userList[k].gameOver == true )
		{
			if( k == myBoardNo )
				display->blt( 550, 300, surfLoseLarge );
			else
				display->blt( ptBoard[count].x + 10, ptBoard[count].y + 50, surfLoseSmall );
		}


		if( k != myBoardNo )
			count++;
	}
}

void GameScreen::drawBlock()
{
	RECT rt = { 0, 0, 0, 0 };
	POINT ptBoard[4] = { { 52, 112 }, { 269, 112 }, { 52, 375 }, { 269, 375 } };
	int info[16];
	int i, j, k, no, count = 0;
	int x, y;

	for( k = 0 ; k < 5 ; k++ )
	{
		if( userList[k].account.IsEmpty() == TRUE )
			continue;

		userList[k].board.getBlock( info );
		x = userList[k].board.getBlockPosX();
		y = userList[k].board.getBlockPosY();

		for( j = 0 ; j < 4 ; j++ )
		{
			for( i = 0 ; i < 4 ; i++ )
			{
				no = info[j * 4 + i];
				if( no != 0 )
				{
					if( k == myBoardNo )
					{
						rt.left    = ( no - 1 ) * 16;
						rt.right   = rt.left + 16;
						rt.bottom  = 16;

						display->blt( 534 + ( x + i ) * 16, 198 + ( y + j ) * 16, surfLargeBlock, &rt );
					}
					else
					{
						rt.left    = ( no - 1 ) * 8;
						rt.right   = rt.left + 8;
						rt.bottom  = 8;

						display->blt( ptBoard[count].x + ( x + i ) * 8,
									ptBoard[count].y + ( y + j ) * 8, surfSmallBlock, &rt );
					}
				}
			}
		}

		if( k != myBoardNo )
			count++;
	}
}

void GameScreen::processInkey()
{
	int oldX, oldY, oldDir;

	oldX = userList[myBoardNo].board.getBlockPosX();
	oldY = userList[myBoardNo].board.getBlockPosY();
	oldDir = userList[myBoardNo].board.getBlockDir();

	if( blockDrop == false )
	{
		if( ::GetAsyncKeyState( VK_LEFT ) != 0 )		userList[myBoardNo].board.moveBlock( -1, 0 );
		if( ::GetAsyncKeyState( VK_RIGHT ) != 0 )		userList[myBoardNo].board.moveBlock( 1, 0 );
		if( ::GetAsyncKeyState( VK_DOWN ) != 0 )
		{
			if( userList[myBoardNo].board.moveBlock( 0, 1 ) == false )
			{
				userList[myBoardNo].score += userList[myBoardNo].board.fixBlock();
				userList[myBoardNo].board.newBlock();
			}
		}
		if( ::GetAsyncKeyState( VK_UP ) & 0xFF )		userList[myBoardNo].board.changeBlockDir();
		if( ::GetAsyncKeyState( VK_SPACE ) & 0xFF )		blockDrop = true;
	}

	if( blockDrop == true )
	{
		if( userList[myBoardNo].board.moveBlock( 0, 2 ) == false )
		{
			userList[myBoardNo].board.moveBlock( 0, 1 );
			userList[myBoardNo].score += userList[myBoardNo].board.fixBlock();
			userList[myBoardNo].board.newBlock();

			blockDrop = false;
		}
	}
}
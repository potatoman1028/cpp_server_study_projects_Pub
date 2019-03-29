#ifndef  __GameScreen_H
#define  __GameScreen_H

#include <AfxTempl.h>

#include <Common/DxWindow/DirectX/DxDisplay.h>
#include <Common/DxWindow/Windows/FrameView.h>

#include <Common/DxWindow/Windows/Controls/CheckBoxCtrl.h>
#include <Common/DxWindow/Windows/Controls/ButtonCtrl.h>
#include <Common/DxWindow/Windows/Controls/EditBoxCtrl.h>

#include "UserInfo.h"
#include "ClientSocket.h"
#include "DlgLogin.h"

class GameScreen : public FrameView
{
public:
			GameScreen( HWND hwnd, DxDisplay* disp, ClientSocket* cs );
	virtual	~GameScreen();

	virtual	void			onCreate();
	virtual	void			onDestroy();

	virtual	void			onDraw();
	virtual	bool			process();

	virtual	void			preTranslateMessage( MSG* msg );

	static	ClientSocket*	getClientSocket(){ return clientSocket; }


			void			onSocketConnect( WPARAM wParam, LPARAM lParam );
			void			onSocketClose( WPARAM wParam, LPARAM lParam );
			void			packetParsing( Packet& packet );

protected:
	virtual	void			parseMessage( DWORD message, WPARAM wParam, LPARAM lParam );

private:
			void			onGSConnectionSuccessAck( Packet& packet );
			void			onGGEnterGameRoomAck( Packet& packet );
			void			onGGUserListAck( Packet& packet );
			void			onGGLeaveGameRoomAck( Packet& packet );
			void			onGGChatMessageAck( Packet& packet );

			void			onGGReadyAck( Packet& packet );
			void			onGGStartGameAck( Packet& packet );
			void			onGGNewBlockAck( Packet& packet );
			void			onGGMoveBlockAck( Packet& packet );
			void			onGGFixBlockAck( Packet& packet );
			void			onGGGameOverAck( Packet& packet );


			void			drawBoard();
			void			drawBlock();

			void			processInkey();


			HWND			handleMainFrm;
			DxSurface*		surfLargeBlock;
			DxSurface*		surfSmallBlock;
			DxSurface*		surfLoseSmall;
			DxSurface*		surfLoseLarge;

			bool			pressedKey;
			bool			blockDrop;


			UserInfo		userList[5];
			int				myBoardNo;
			bool			playGame;
			bool			exitGame;

			DlgLogin*		dlgLogin;

			CheckBoxCtrl*	ctrlReadyCheck;
			ButtonCtrl*		btnStart;
			EditBoxCtrl*	editChatMessage;

			CList	< char*, char* >	listChatMessage;

	static	ClientSocket*	clientSocket;
};

#endif
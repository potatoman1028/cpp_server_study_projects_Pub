#include "stdafx.h"
#include "Tetris.h"
#include "TetrisBoard.h"
#include "GameScreen.h"
#include "Protocol.h"

int  blockData[7][4][16] =
{
	{
		{ 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0 }
	},
	{
		{ 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 2, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 2, 2 },
		{ 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 2, 2, 0 },
		{ 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 0, 0, 0 }
	},
	{
		{ 0, 4, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0 },
		{ 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 4, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0 },
		{ 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0, 0, 5, 5, 0, 5, 5, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 5, 0, 0, 0, 5, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 5, 5, 0, 0 },
		{ 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 5, 0, 0, 0, 5, 0 }
	},
	{
		{ 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 6, 0, 0, 6, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 6, 6 },
		{ 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 6, 0, 0, 6, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 6, 6 }
	},
	{
		{ 0, 0, 0, 0, 7, 7, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 7, 0, 0, 0, 7, 7, 0, 0, 7, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0 },
		{ 0, 0, 0, 0, 0, 0, 7, 0, 0, 7, 7, 0, 0, 0, 7, 0 }
	}
};

TetrisBoard::TetrisBoard()
{
}

TetrisBoard::~TetrisBoard()
{
}

void TetrisBoard::initialize( int seed )
{
	int i , j;

	for( i = 0 ; i < BOARD_HEIGHT ; i++)
		for( j = 0 ; j < BOARD_WIDTH ; j++)
			board[i][j] = 0;

	for( i = 0 ; i < BOARD_WIDTH ; i++ )
		board[BOARD_HEIGHT - 1][i] = 99;

	for( i = 0 ; i < BOARD_HEIGHT ; i++ )
	{
		board[i][0] = 99;
		board[i][BOARD_WIDTH - 1] = 99;
	}

	srand( seed );

	newBlock();
	tickFreeDrop = ::GetTickCount();
}

void TetrisBoard::processFreeDrop()
{
	if( ::GetTickCount() - tickFreeDrop > 1500 )
	{
		tickFreeDrop = ::GetTickCount();

		if( isCollide( blockPosX, blockPosY + 1 ) == true )
		{
			fixBlock();
			newBlock();
		}
		else
			moveBlock( 0, 1 );
	}
}

bool TetrisBoard::isCollide( int x, int y )
{
	int i, j;

	if( x < 0 || y < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT )
		return true;

	for( j = 0 ; j < 4 ; j++ )
	{
		for( i = 0 ; i < 4 ; i++ )
		{
			if( board[y + j][x + i] != 0 && blockData[blockNo][blockDir][j * 4 + i] != 0 )
				return true;
		}
	} 

	return false;
}

int TetrisBoard::fixBlock()
{
	int j, i;

	for( j = 0 ; j < 4 ; j++ )
	{
		for( i = 0 ; i < 4 ; i++ )
		{
			if( blockData[blockNo][blockDir][j * 4 + i] != 0 )
				board[blockPosY + j][blockPosX + i] = blockData[blockNo][blockDir][j * 4 + i];
		}
	}

	Packet sendPacket( GG_FIXBLOCK_REQ );

	sendPacket << blockPosX << blockPosY << blockDir;
	GameScreen::getClientSocket()->sendPacket( sendPacket );

	return checkFullLine();
}

int TetrisBoard::fixBlock( int x, int y, int dir )
{
	int j, i;

	for( j = 0 ; j < 4 ; j++ )
	{
		for( i = 0 ; i < 4 ; i++ )
		{
			if( blockData[blockNo][dir][j * 4 + i] != 0 )
				board[y + j][x + i] = blockData[blockNo][dir][j * 4 + i];
		}
	}

	return checkFullLine();
}

void TetrisBoard::newBlock( int no )
{
	blockDir = 0;
	blockPosX = 4;
	blockPosY = 0;

	if( no == -1 )
	{
		Packet sendPacket;

		blockNo = rand() % 7;

		if( isCollide( blockPosX, blockPosY ) == true )
			sendPacket.id( GG_GAMEOVER_REQ );
		else
			sendPacket.id( GG_NEWBLOCK_REQ ) << blockNo;

		GameScreen::getClientSocket()->sendPacket( sendPacket );
	}
	else
		blockNo = no;
}

int TetrisBoard::checkFullLine()
{
	int i, j, countWidth; 
	int ret = 0;

	while( 1 )
	{
		for( j = BOARD_HEIGHT - 2 ; j != 0 ; j--)
		{

			countWidth = 0;

			for( i = 0 ; i < BOARD_WIDTH ; i++ )
			{
				if( board[j][i] != 0 ) 
					countWidth++;
			}

			if( countWidth == BOARD_WIDTH )
				break;
		}

		if( countWidth != BOARD_WIDTH )
			break;

	 	removeLine( j );
		ret += 15;
	}

	return ret;
}

void TetrisBoard::removeLine( int y )
{
	int j, i;

	for( j = y ; j > 0 ; j-- )
		for( i = 0 ; i < BOARD_WIDTH ; i++ )
			board[j][i] = board[j - 1][i];
}

void TetrisBoard::getBoard( int info[BOARD_HEIGHT][BOARD_WIDTH] )
{
	::memcpy( info, board, BOARD_HEIGHT * BOARD_WIDTH * sizeof( int ) );
}

void TetrisBoard::getBlock( int info[16], int blockType )
{
	if( blockType == -1 )
		::memcpy( info, blockData[blockNo][blockDir], 16 * sizeof( int ) );
	else
		::memcpy( info, blockData[blockType][0], 16 * sizeof( int ) );
}

void TetrisBoard::run()
{
	processFreeDrop();
}

bool TetrisBoard::moveBlock( int offsetX, int offsetY )
{
	if( isCollide( blockPosX + offsetX, blockPosY + offsetY ) == false )
	{
		blockPosX += offsetX;
		blockPosY += offsetY;


		Packet sendPacket( GG_MOVEBLOCK_REQ );

		sendPacket << blockPosX << blockPosY << blockDir;
		GameScreen::getClientSocket()->sendPacket( sendPacket );

		return true;
	}

	return false;
}

bool TetrisBoard::moveBlockAt( int x, int y, int dir )
{
	int oldX = blockPosX;
	int oldY = blockPosY;

	blockPosX = x;
	blockPosY = y;
	if( dir != -1 )
		blockDir = dir;

	if( isCollide( blockPosX, blockPosY ) == true )
	{
		blockPosX = oldX;
		blockPosY = oldY;

		return false;
	}

	return true;
}

void TetrisBoard::changeBlockDir()
{
	int oldDir = blockDir;
	int oldX = blockPosX;
	int oldY = blockPosY;

	blockDir = ( blockDir + 1 ) % 4;

	if( isCollide( blockPosX, blockPosY ) == true )
	{
		blockPosX--;
		if( isCollide( blockPosX, blockPosY ) == true )
		{
			blockPosX--;
			if( isCollide( blockPosX, blockPosY ) == true )
			{
				blockPosX += 3;
				if( isCollide( blockPosX, blockPosY ) == true )
				{
					blockPosX++;
					if( isCollide( blockPosX, blockPosY ) == true )
					{
						blockPosX = oldX;
						blockPosY = oldY;

						blockDir = oldDir;
					}
				}
			}
		}
	}
}
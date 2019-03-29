#ifndef  __TetrisBoard_H
#define  __TetrisBoard_H

#define  BOARD_WIDTH		14
#define  BOARD_HEIGHT		21

class TetrisBoard
{
public:
			TetrisBoard();
	virtual	~TetrisBoard();

			void		initialize( int seed );

			void		run();

			void		getBoard( int info[BOARD_HEIGHT][BOARD_WIDTH] );
			void		getBlock( int info[16], int blockType = -1 );

			int			getBlockPosX(){ return blockPosX; }
			int			getBlockPosY(){ return blockPosY; }
			int			getBlockDir(){ return blockDir; }

			bool		moveBlock( int offsetX, int offsetY );
			bool		moveBlockAt( int x, int y, int dir = -1 );


			int			fixBlock();
			int			fixBlock( int x, int y, int dir );
			void		newBlock( int no = -1 );
			void		changeBlockDir();

private:
			bool		isCollide( int x, int y );

			void		processFreeDrop();
			int			checkFullLine();
			void		removeLine( int y );


			int			board[BOARD_HEIGHT][BOARD_WIDTH];
			int			blockNo, blockDir;
			int			blockPosX, blockPosY;

			DWORD		tickFreeDrop;
};

#endif
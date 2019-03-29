#include "PatchCatalog.h"

PatchCatalog*		PatchCatalog::selfInstance = 0;

PatchCatalog::PatchCatalog()
{
	loaderVersion = 1;
	clientVersion = MAKELONG( 0, 2 );

	ftpAddress = _T( "127.0.0.1" );
	ftpAccount = _T( "PatchAccount" );
	ftpPassword = _T( "PatchPassword" );

	//  Setting loader patch list
	{
		PatchFileInfo* info;
	}

	//  Setting client patch list
	{
		PatchFileInfo* info;

		info = new PatchFileInfo();
		info->remoteFile = _T( "TestVer_1/File0.dat" );
		info->localFile = _T( "File0.dat" );
		info->version = MAKELONG( 0, 2 );

		listClientPatch.pushTail( info );


		info = new PatchFileInfo();
		info->remoteFile = _T( "TestVer_1/Folder/File.dat" );
		info->localFile = _T( "Folder/File.dat" );
		info->version = MAKELONG( 0, 2 );

		listClientPatch.pushTail( info );
	}
}

PatchCatalog::~PatchCatalog()
{
}

PatchCatalog* PatchCatalog::getInstance()
{
	if( selfInstance == 0 )
		selfInstance = new PatchCatalog();

	return selfInstance;
}

void PatchCatalog::releaseInstance()
{
	if( selfInstance != 0 )
	{
		delete selfInstance;
		selfInstance = 0;
	}
}

DWORD PatchCatalog::getLoaderVersion()
{
	return loaderVersion;
}

DWORD PatchCatalog::getClientVersion()
{
	return clientVersion;
}

void PatchCatalog::getFTPServerInfo( String* address, String* account, String* password )
{
	*address  = ftpAddress;
	*account  = ftpAccount;
	*password = ftpPassword;
}

void PatchCatalog::makeLoaderUpdateList( DWORD version, PatchFileInfo* info, DWORD* count )
{
	PatchFileInfo* itemInfo;
	List::Position pos;
	DWORD clientVersion, fileVersion;

	clientVersion = HIWORD( version ) | ( LOWORD( version ) << 16 );
	*count = 0;
	pos = listLoaderPatch.headPosition();
	while( listLoaderPatch.isEnd( pos ) == false )
	{
		itemInfo = ( PatchFileInfo* )listLoaderPatch.getItem( pos++ );
		fileVersion = HIWORD( itemInfo->version ) | ( LOWORD( itemInfo->version ) << 16 );
	
		if( fileVersion > clientVersion )
		{
			info[*count].localFile  = itemInfo->localFile;
			info[*count].remoteFile = itemInfo->remoteFile;
			info[*count].version = itemInfo->version;

			( *count )++;
		}
	}
}

void PatchCatalog::makeClientUpdateList( DWORD version, List* info, DWORD* count )
{
	PatchFileInfo* itemInfo;
	List::Position pos;

	*count = 0;

	pos = listClientPatch.headPosition();
	while( listClientPatch.isEnd( pos ) == false )
	{
		itemInfo = ( PatchFileInfo* )listClientPatch.getItem( pos++ );
	
		if( LOWORD( clientVersion ) < LOWORD( itemInfo->version ) ||
			( LOWORD( clientVersion ) == LOWORD( itemInfo->version ) &&
				HIWORD( clientVersion ) < HIWORD( itemInfo->version ) ) )
			continue;

		DWORD majorClient, majorItem;
		DWORD minorClient, minorItem;

		majorClient = LOWORD( version );
		minorClient = HIWORD( version );
		majorItem   = LOWORD( itemInfo->version );
		minorItem   = HIWORD( itemInfo->version );

		if( majorItem > majorClient ||
			( majorItem == majorClient && minorItem > minorClient ) )
		{
			info->pushTail( itemInfo );
			( *count )++;
		}
	}
}

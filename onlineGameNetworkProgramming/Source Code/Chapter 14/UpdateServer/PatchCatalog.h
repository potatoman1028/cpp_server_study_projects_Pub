#ifndef  __PatchCatalog_H
#define  __PatchCatalog_H

#include <cpp/util/List.h>
#include <cpp/lang/String.h>

typedef cpp::util::List List;

class PatchCatalog : public Object
{
public:
	class PatchFileInfo : public Object
	{
	public:
				PatchFileInfo(){}
		virtual	~PatchFileInfo(){}

				String		localFile;
				String		remoteFile;
				int			version;
	};

public:
	static	PatchCatalog*		getInstance();
	static	void				releaseInstance();

			DWORD				getLoaderVersion();
			DWORD				getClientVersion();
			void				getFTPServerInfo( String* address, String* account, String* password );

			void				makeLoaderUpdateList( DWORD version, PatchFileInfo* info, DWORD* count );
			void				makeClientUpdateList( DWORD version, List* info, DWORD* count );

private:
			PatchCatalog();
	virtual	~PatchCatalog();

	static	PatchCatalog*		selfInstance;

			List				listLoaderPatch;
			List				listClientPatch;

			DWORD				loaderVersion;
			DWORD				clientVersion;

			String				ftpAddress, ftpAccount, ftpPassword;
};

#endif
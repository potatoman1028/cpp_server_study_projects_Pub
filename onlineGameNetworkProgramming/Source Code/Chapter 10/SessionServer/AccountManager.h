#ifndef  __AccountManager_H
#define  __AccountManager_H

#include <cpp/lang/String.h>

enum VerifyResult
{
	VR_Success,
	VR_ConnectingAccount,
	VR_InvalidAccount,
	VR_InvalidPassword
};

class AccountManager
{
public:
	static	AccountManager*			getInstance();
	static	void					releaseInstance();

			VerifyResult			verifyAccount( String& account, String& password );

private:
			AccountManager();
	virtual	~AccountManager();

	static	AccountManager*			selfInstance;
};

#endif
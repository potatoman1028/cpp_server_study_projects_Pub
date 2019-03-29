#include "AccountManager.h"

AccountManager* AccountManager::selfInstance = 0;

AccountManager::AccountManager()
{
}

AccountManager::~AccountManager()
{
}

AccountManager* AccountManager::getInstance()
{
	if( selfInstance == 0 )
		selfInstance = new AccountManager();

	return selfInstance;
}

void AccountManager::releaseInstance()
{
	if( selfInstance == 0 )
		return;

	delete selfInstance;
	selfInstance = 0;
}

VerifyResult AccountManager::verifyAccount( String& account, String& password )
{
	return VR_Success;
}
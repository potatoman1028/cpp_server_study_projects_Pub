#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/DxWindowEntry.h>
#include <Common/DxWindow/Windows/FontCatalog.h>

DxWindowEntry*		DxWindowEntry::selfInstance = 0;

DxWindowEntry::DxWindowEntry()
{
}

DxWindowEntry::~DxWindowEntry()
{
}

DxWindowEntry* DxWindowEntry::getInstance()
{
	if( selfInstance == 0 )
		selfInstance = new DxWindowEntry();

	return selfInstance;
}

void DxWindowEntry::releaseInstance()
{
	if( selfInstance == 0 )
		return;

	delete selfInstance;
	selfInstance = 0;
}

bool DxWindowEntry::startUp()
{
	FontCatalog::getInstance();

	return true;
}

void DxWindowEntry::close()
{
	FontCatalog::releaseInstance();
}
#include "stdafx.h"
#include "Character.h"

CCharacter::CCharacter(VOID)
{
	mIndex		= 0;
	mJob		= 0;
	mType		= 0;
	mExp		= 0;

	memset(mEquips, 0, sizeof(mEquips));
	memset(mName, 0, sizeof(mName));
}

CCharacter::~CCharacter(VOID)
{
}

VOID CCharacter::Initialize(VOID)
{
	mIndex		= 0;
	mJob		= 0;
	mType		= 0;
	mExp		= 0;

	memset(mEquips, 0, sizeof(mEquips));
	memset(mName, 0, sizeof(mName));
}
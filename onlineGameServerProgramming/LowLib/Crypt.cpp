#include "stdafx.h"
#include "Crypt.h"

const INT		C1				= 52845;
const INT		C2				= 22719;
const INT		KEY				= 72957;

BOOL CCrypt::Encrypt(BYTE *source, BYTE *destination, DWORD length)
{
	DWORD	i;
	INT		Key = KEY;

	if (!source || !destination || length <= 0)
		return FALSE;

	for (i=0;i<length;i++)
	{
		destination[i] = source[i]^Key >> 8;
		Key = (destination[i] + Key) * C1 + C2;
	}

	return TRUE;
}

BOOL CCrypt::Decrypt(BYTE *source, BYTE *destination, DWORD length)
{
	DWORD	i;
	BYTE	PreviousBlock;
	INT		Key	= KEY;

	if (!source || !destination || length <= 0)
		return FALSE;

	for (i=0;i<length;i++)
	{
		PreviousBlock = source[i];
		destination[i] = source[i]^Key >> 8;
		Key = (PreviousBlock + Key) * C1 + C2;
	}

	return TRUE;
}

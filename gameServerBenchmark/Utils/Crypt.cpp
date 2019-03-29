#include "stdafx.h"
#include "Crypt.h"

const INT C1	= 52845;
const INT C2	= 22719;
const INT KEY	= 72957;

BOOL CCrypt::Encrypt(BYTE *pucSrc, BYTE *pucDest, DWORD dwLength)
{
	INT iKey = KEY;

	if (!pucSrc || !pucDest)
		return FALSE;

	for (DWORD i=0;i<dwLength;i++)
	{
		pucDest[i] = pucSrc[i]^iKey >> 8;
		iKey = (pucDest[i] + iKey) * C1 + C2;
	}

	return TRUE;
}

BOOL CCrypt::Decrypt(BYTE *pucSrc, BYTE *pucDest, DWORD dwLength)
{
	BYTE	PrevBlock;
	INT		iKey = KEY;

	if (!pucSrc || !pucDest)
		return FALSE;

	for (DWORD i=0;i<dwLength;i++)
	{
		PrevBlock = pucSrc[i];
		pucDest[i] = pucSrc[i]^iKey >> 8;
		iKey = (PrevBlock + iKey) * C1 + C2;
	}

	return TRUE;
}
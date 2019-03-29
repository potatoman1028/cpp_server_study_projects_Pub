#include "StdAfx.h"
#include "Random.h"

CRandom::CRandom(UINT uiSeed)
{
	m_uiSeed = uiSeed;
}

CRandom::~CRandom(VOID)
{
}

INT CRandom::Rand(VOID)
{
	return (((m_uiSeed = m_uiSeed * 214013L + 2531011L) >> 16) & 0x7FFF);
}

INT CRandom::SimpleRandom(VOID)
{
	return (((m_uiSeed = m_uiSeed + 2531011L) >> 3) & 0x7FFF);
}

INT CRandom::ComplexRandom(VOID)
{
	UINT uiNext = m_uiSeed;
	INT  iResult;

	uiNext *= 1103515245;
	uiNext += 12345;
	iResult = (UINT) (uiNext / 65536) % 2048;

	uiNext *= 1103515245;
	uiNext += 12345;
	iResult <<= 10;
	iResult ^= (UINT) (uiNext / 65536) % 1024;

	uiNext *= 1103515245;
	uiNext += 12345;
	iResult <<= 10;
	iResult ^= (UINT) (uiNext / 65536) % 1024;

	m_uiSeed = uiNext;
	return iResult;
}


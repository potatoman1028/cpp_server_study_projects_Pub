#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "MemoryPool.h"
#include "Stream.h"

CStream::CStream(VOID)
{
	mBufferPointer	= NULL;
	mLength			= 0;
}

CStream::~CStream(VOID)
{
}

BOOL CStream::SetBuffer(BYTE *buffer)
{
	if (!buffer)
		return FALSE;

	mBufferPointer	= buffer;
	mLength			= 0;

	return TRUE;
}

BOOL CStream::SetBuffer()
{
	if (!mBufferPointer)
		return FALSE;

	mLength			= 0;

	return TRUE;
}

BOOL CStream::ReadInt32(INT *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(INT));

	mLength += sizeof(INT);

	return TRUE;
}

BOOL CStream::ReadDWORD(DWORD *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(DWORD));

	mLength += sizeof(DWORD);

	return TRUE;
}

BOOL CStream::ReadDWORD_PTR(DWORD_PTR *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(DWORD_PTR));

	mLength += sizeof(DWORD_PTR);

	return TRUE;
}

BOOL CStream::ReadByte(BYTE *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(BYTE));

	mLength += sizeof(BYTE);

	return TRUE;
}

BOOL CStream::ReadBytes(BYTE *data, DWORD length)
{
	memcpy(data, mBufferPointer + mLength, length);

	mLength += length;

	return TRUE;
}

BOOL CStream::ReadFloat(FLOAT *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(FLOAT));

	mLength += sizeof(FLOAT);

	return TRUE;
}

BOOL CStream::ReadInt64(INT64 *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(INT64));

	mLength += sizeof(INT64);

	return TRUE;
}

BOOL CStream::ReadSHORT(SHORT *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(SHORT));

	mLength += sizeof(SHORT);

	return TRUE;
}

BOOL CStream::ReadUSHORT(USHORT *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(USHORT));

	mLength += sizeof(USHORT);

	return TRUE;
}

BOOL CStream::ReadBOOL(BOOL *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(BOOL));

	mLength += sizeof(BOOL);

	return TRUE;
}

BOOL CStream::ReadWCHAR(WCHAR *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(WCHAR));

	mLength += sizeof(WCHAR);

	return TRUE;
}

BOOL CStream::ReadWCHARs(LPWSTR data, DWORD length)
{
	memcpy(data, mBufferPointer + mLength, length * sizeof(WCHAR));

	mLength += length * sizeof(WCHAR);

	return TRUE;
}

BOOL CStream::WriteInt32(INT data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(INT));

	mLength += sizeof(INT);

	return TRUE;
}

BOOL CStream::WriteDWORD(DWORD data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(DWORD));

	mLength += sizeof(DWORD);

	return TRUE;
}

BOOL CStream::WriteDWORD_PTR(DWORD_PTR data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(DWORD_PTR));

	mLength += sizeof(DWORD_PTR);

	return TRUE;
}

BOOL CStream::WriteByte(BYTE data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(BYTE));

	mLength += sizeof(BYTE);

	return TRUE;
}

BOOL CStream::WriteBytes(BYTE *data, DWORD length)
{
	memcpy(mBufferPointer + mLength, data, length);

	mLength += length;

	return TRUE;
}

BOOL CStream::WriteFloat(FLOAT data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(FLOAT));

	mLength += sizeof(FLOAT);

	return TRUE;
}

BOOL CStream::WriteInt64(INT64 data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(INT64));

	mLength += sizeof(INT64);

	return TRUE;
}

BOOL CStream::WriteSHORT(SHORT data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(SHORT));

	mLength += sizeof(SHORT);

	return TRUE;
}

BOOL CStream::WriteUSHORT(USHORT data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(USHORT));

	mLength += sizeof(USHORT);

	return TRUE;
}

BOOL CStream::WriteBOOL(BOOL data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(BOOL));

	mLength += sizeof(BOOL);

	return TRUE;
}

BOOL CStream::WriteWCHAR(WCHAR data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(WCHAR));

	mLength += sizeof(WCHAR);

	return TRUE;
}

BOOL CStream::WriteWCHARs(LPCWSTR data, DWORD length)
{
	memcpy(mBufferPointer + mLength, data, length * sizeof(WCHAR));

	mLength += length * sizeof(WCHAR);

	return TRUE;
}

DWORD CStream::GetLength(VOID)
{
	return mLength;
}

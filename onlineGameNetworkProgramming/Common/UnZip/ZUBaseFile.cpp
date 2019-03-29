// ZUBaseFile.cpp: implementation of the CZUBaseFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZUBaseFile.h"
#include <afxcoll.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZUBaseFile::CZUBaseFile()
{
	m_bZipFile = true;
	m_bDetectZlibMemoryLeaks = true;
	m_pFile = NULL;
}

CZUBaseFile::~CZUBaseFile()
{
	if (m_list.GetCount())
	{
// some memory hasn't been freed due to an error in zlib, so free it now
		POSITION pos = m_list.GetHeadPosition();
		while (pos)
			delete[] m_list.GetNext(pos);
		
	}
}

bool CZUBaseFile::IsClosed()
{
	return  !m_pFile || (m_pFile->m_hFile == CFile::hFileNull);
}

void* CZUBaseFile::myalloc(void* opaque, UINT items, UINT size)
{
	void* p = new char[size * items];
	if (opaque)
	{
		CPtrList* list  = (CPtrList*) opaque;
		list->AddTail(p);
	}
	return p;
}

void CZUBaseFile::myfree(void* opaque, void* address)
{
	if (opaque)
	{
		CPtrList* list  = (CPtrList*) opaque;
		POSITION pos = list->Find(address);
		if (pos)
			list->RemoveAt(pos);
	}
	delete[] address;
}

void CZUBaseFile::ThrowError(int err)
{
	CString fileName = IsClosed() ? "" : m_pFile->GetFilePath();
	AfxThrowZipException(err, fileName);//, m_bZipFile);
}

void CZUBaseFile::CheckForError(int err)
{
// 	ASSERT (err != Z_NEED_DICT);
// 	ASSERT (err != Z_STREAM_END);

	if ((err == Z_OK) || (err == Z_NEED_DICT))
		return;

	ThrowError(err);

}
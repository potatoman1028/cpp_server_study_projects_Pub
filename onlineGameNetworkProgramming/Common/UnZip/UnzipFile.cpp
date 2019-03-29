// UnzipFile.cpp: implementation of the CUnzipFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnzipFile.h"

#define SIZECENTRALDIRITEM (0x2e)
#define SIZEZIPLOCALHEADER (0x1e)


#ifndef UNZ_BUFSIZE
#define UNZ_BUFSIZE (16384)
#endif

#ifndef UNZ_MAXFILENAMEINZIP
#define UNZ_MAXFILENAMEINZIP (256)
#endif

#define BUFREADCOMMENT (0x400)


#define UNZ_PARAMERROR                  (-102)
#define UNZ_BADZIPFILE                  (-103)
#define UNZ_CRCERROR                    (-105)

unz_s::unz_s()
{
	pfile_in_zip_read = NULL;
}

void unz_s::alloc_pfile_in_zip_read()
{
	if (!pfile_in_zip_read)
		pfile_in_zip_read = new file_in_zip_read_info;
}

void unz_s::free_pfile_in_zip_read()
{
	if (pfile_in_zip_read)
	{
		delete pfile_in_zip_read;
		pfile_in_zip_read = NULL;
	}

}

unz_s::~unz_s()
{
	free_pfile_in_zip_read();
}

file_in_zip_read_info::file_in_zip_read_info()
{
	read_buffer = new char[UNZ_BUFSIZE];
	stream_initialised  = 0;
	pos_local_extrafield = 0;
	crc32 = 0;
	stream.total_out = 0;
}


file_in_zip_read_info::~file_in_zip_read_info()
{
	delete[] read_buffer;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnzipFile::CUnzipFile()
{
	m_bZipFile = false;
	m_pFile = &uf.file;
}


CUnzipFile::CUnzipFile(LPCTSTR lpszPath)
{
	m_bZipFile = false;
	m_pFile = &uf.file;
	Open(lpszPath);
}

//   Close a ZipFile
void CUnzipFile::Close()
{
	if (IsClosed())
		return;

    if (uf.pfile_in_zip_read)
        CloseCurrentFile();
	
	uf.file.Close();
}

//   Close the file in zip opened with OpenCurrentFile
void CUnzipFile::CloseCurrentFile()
{

	if (uf.pfile_in_zip_read == NULL)
		return; // already closed - do not throw an exception

	if (uf.pfile_in_zip_read->rest_read_uncompressed == 0)
	{
		if (uf.pfile_in_zip_read->crc32 != uf.pfile_in_zip_read->crc32_wait)
			ThrowError(UNZ_CRCERROR);
	}

	
	if (uf.pfile_in_zip_read->stream_initialised)
		inflateEnd(&uf.pfile_in_zip_read->stream);

	uf.free_pfile_in_zip_read();

}

/*
  Set the current file of the zipfile to the first file.
*/
void CUnzipFile::GoToFirstFile()
{
	uf.pos_in_central_dir = uf.offset_central_dir;
	uf.num_file = 0;
	unzlocal_GetCurrentFileInfoInternal(uf.cur_file_info,
										uf.cur_file_info_internal,
										NULL,0,NULL,0,NULL,0);
	uf.current_file_ok = 1;
}


/*
  Try locate the file szFileName in the zipfile.

  return value :
  true if the file is found. It becomes the current file.
  false if the file is not found
*/
bool CUnzipFile::LocateFile(CString szFileName, bool bCaseSensitive)
{

    if (szFileName.GetLength() >= UNZ_MAXFILENAMEINZIP)
        ThrowError(UNZ_PARAMERROR);

	if (!uf.current_file_ok)
		return false;

	uLong num_fileSaved = uf.num_file;
	uLong pos_in_central_dirSaved = uf.pos_in_central_dir;

	GoToFirstFile();

	bool res = true;
	while (res)
	{
		char szCurrentFileName[UNZ_MAXFILENAMEINZIP+1];
		GetCurrentFileInfo(NULL, szCurrentFileName, sizeof(szCurrentFileName)-1,
								NULL,0,NULL,0);
		if (StringFileNameCompare(szCurrentFileName, szFileName, bCaseSensitive)==0)
			return true;

		res = GoToNextFile();
	}

	uf.num_file = num_fileSaved ;
	uf.pos_in_central_dir = pos_in_central_dirSaved ;

	return false;
}



/*
  Set the current file of the zipfile to the next file.
  return true if there is no problem
  return false if the actual file was the latest.
*/
bool CUnzipFile::GoToNextFile()
{
	if (!uf.current_file_ok)
		return false;

	if (uf.num_file + 1 == uf.gi.number_entry)
		return false;

	uf.pos_in_central_dir += SIZECENTRALDIRITEM + uf.cur_file_info.size_filename +
			uf.cur_file_info.size_file_extra + uf.cur_file_info.size_file_comment ;

	uf.num_file++;
	unzlocal_GetCurrentFileInfoInternal(uf.cur_file_info,
										uf.cur_file_info_internal,
										NULL,0,NULL,0,NULL,0);
	uf.current_file_ok = 1;
	return true;
}

/*
  Read the local header of the current zipfile
  Check the coherency of the local header and info in the end of central
        directory about this file
  store in *piSizeVar the size of extra info in local header
        (filename and size of extra field data)
*/
void CUnzipFile::unzlocal_CheckCurrentFileCoherencyHeader (uInt & iSizeVar,
													uLong & offset_local_extrafield,
													uLong & size_local_extrafield)
{

	iSizeVar = 0;
	offset_local_extrafield = 0;
	size_local_extrafield = 0;

	uf.file.Seek(uf.cur_file_info_internal.offset_curfile + uf.byte_before_the_zipfile, CFile::begin);

	uLong uMagic;
	unzlocal_getLong(uMagic);
	if (uMagic!=0x04034b50)
		ThrowError(UNZ_BADZIPFILE);

	uLong uData;
	unzlocal_getShort(uData);

// 	if (uData != uf.cur_file_info.version)
// 		ThrowError(UNZ_BADZIPFILE);

	uLong uFlags;
	unzlocal_getShort(uFlags);

	unzlocal_getShort(uData);
	if (uData != uf.cur_file_info.compression_method)
		ThrowError(UNZ_BADZIPFILE);

    if ((uf.cur_file_info.compression_method != 0) &&
                         (uf.cur_file_info.compression_method != Z_DEFLATED))
        ThrowError(UNZ_BADZIPFILE);

	unzlocal_getLong(uData); /* date/time */

	unzlocal_getLong(uData); /* crc */
	if ((uData != uf.cur_file_info.crc) &&
		                      ((uFlags & 8)==0))
		ThrowError(UNZ_BADZIPFILE);

	unzlocal_getLong(uData); /* size compr */

	if ((uData != uf.cur_file_info.compressed_size) &&
							  ((uFlags & 8) == 0))
		ThrowError(UNZ_BADZIPFILE);

	unzlocal_getLong(uData); /* size uncompr */

	if ((uData!=uf.cur_file_info.uncompressed_size) && 
							  ((uFlags & 8)==0))
		ThrowError(UNZ_BADZIPFILE);

	uLong size_filename;
	unzlocal_getShort(size_filename);

	if ((size_filename != uf.cur_file_info.size_filename))
		ThrowError(UNZ_BADZIPFILE);

	iSizeVar = (uInt)size_filename;

	uLong size_extra_field;
	unzlocal_getShort(size_extra_field);

	offset_local_extrafield = uf.cur_file_info_internal.offset_curfile +
									SIZEZIPLOCALHEADER + size_filename;
	size_local_extrafield = (uInt)size_extra_field;

	iSizeVar += (uInt)size_extra_field;

}


//   Open for reading data the current file in the zipfile.
void CUnzipFile::OpenCurrentFile()
{

	ASSERT(uf.current_file_ok);

	if (!uf.current_file_ok)
		ThrowError(UNZ_PARAMERROR);

    if (uf.pfile_in_zip_read)
        CloseCurrentFile();

	uInt iSizeVar;
	uLong offset_local_extrafield;  /* offset of the local extra field */
	uLong  size_local_extrafield;    /* size of the local extra field */

	unzlocal_CheckCurrentFileCoherencyHeader(iSizeVar,
				offset_local_extrafield, size_local_extrafield);

	uf.alloc_pfile_in_zip_read();


	uf.pfile_in_zip_read->offset_local_extrafield = offset_local_extrafield;
	uf.pfile_in_zip_read->size_local_extrafield = size_local_extrafield;

	
	if ((uf.cur_file_info.compression_method!=0) &&
        (uf.cur_file_info.compression_method!=Z_DEFLATED))
		ThrowError(UNZ_BADZIPFILE);


	uf.pfile_in_zip_read->crc32_wait = uf.cur_file_info.crc;
	uf.pfile_in_zip_read->compression_method =
            uf.cur_file_info.compression_method;
	
	uf.pfile_in_zip_read->byte_before_the_zipfile=uf.byte_before_the_zipfile;


	if (uf.cur_file_info.compression_method != 0)
	{
	  uf.pfile_in_zip_read->stream.zalloc = (alloc_func)myalloc;
	  uf.pfile_in_zip_read->stream.zfree = (free_func)myfree;
	  uf.pfile_in_zip_read->stream.opaque = m_bDetectZlibMemoryLeaks ? &m_list : 0;
      
	  int err = inflateInit2(&uf.pfile_in_zip_read->stream, -MAX_WBITS);

	  if (err == Z_OK)
	    uf.pfile_in_zip_read->stream_initialised=1;
	  else 
		  CheckForError(err);
        /* windowBits is passed < 0 to tell that there is no zlib header.
         * Note that in this case inflate *requires* an extra "dummy" byte
         * after the compressed stream in order to complete decompression and
         * return Z_STREAM_END. 
         * In unzip, i don't wait absolutely Z_STREAM_END because I known the 
         * size of both compressed and uncompressed data
         */
	}
	uf.pfile_in_zip_read->rest_read_compressed = 
            uf.cur_file_info.compressed_size ;
	uf.pfile_in_zip_read->rest_read_uncompressed = 
            uf.cur_file_info.uncompressed_size ;

	
	uf.pfile_in_zip_read->pos_in_zipfile = 
            uf.cur_file_info_internal.offset_curfile + SIZEZIPLOCALHEADER + 
			  iSizeVar;
	
	uf.pfile_in_zip_read->stream.avail_in = (uInt)0;

}
/*
  Write info about the ZipFile in the *pglobal_info structure.
  No preparation of the structure is needed
*/
void CUnzipFile::GetGlobalInfo(unz_global_info &global_info)
{
	global_info = uf.gi;
}

void CUnzipFile::GetCurrentFileInfo ( unz_file_info* file_info,
                                         LPSTR szFileName,
										 uLong fileNameBufferSize,
										 void *extraField,
										 uLong extraFieldBufferSize,
										 LPSTR szComment,
										 uLong commentBufferSize)
{

	
	unz_file_info temp;
	if (!file_info)
		file_info = &temp;

	unz_file_info_internal temp1;
	unzlocal_GetCurrentFileInfoInternal(*file_info, temp1, szFileName, fileNameBufferSize,
		extraField, extraFieldBufferSize, szComment, commentBufferSize);
}



/*
  Read bytes from the current file.
  buf contain buffer where data must be copied
  len the size of buf.

  return the number of byte copied if somes bytes are copied
  return 0 if the end of file was reached
*/
int CUnzipFile::ReadCurrentFile(void* buf, UINT len)
{

	if (!uf.pfile_in_zip_read)
		ThrowError(UNZ_PARAMERROR);

	if ((len == 0) || !buf)
		return 0;

	uf.pfile_in_zip_read->stream.next_out = (Bytef*)buf;
	uf.pfile_in_zip_read->stream.avail_out = (uInt)len;
	
	if (len > uf.pfile_in_zip_read->rest_read_uncompressed)
		uf.pfile_in_zip_read->stream.avail_out = 
		  (uInt)uf.pfile_in_zip_read->rest_read_uncompressed;


	uInt iRead = 0;

	while (uf.pfile_in_zip_read->stream.avail_out>0)
	{
		if ((uf.pfile_in_zip_read->stream.avail_in==0) &&
            (uf.pfile_in_zip_read->rest_read_compressed>0))
		{
			uInt uReadThis = UNZ_BUFSIZE;
			if (uf.pfile_in_zip_read->rest_read_compressed<uReadThis)
				uReadThis = (uInt)uf.pfile_in_zip_read->rest_read_compressed;
			if (uReadThis == 0)
				return 0;


			uf.file.Seek(uf.pfile_in_zip_read->pos_in_zipfile + 
				uf.pfile_in_zip_read->byte_before_the_zipfile, CFile::begin);

			uf.file.Read(uf.pfile_in_zip_read->read_buffer, uReadThis);
			uf.pfile_in_zip_read->pos_in_zipfile += uReadThis;

			uf.pfile_in_zip_read->rest_read_compressed-=uReadThis;
			
			uf.pfile_in_zip_read->stream.next_in = 
                (Bytef*)uf.pfile_in_zip_read->read_buffer;
			uf.pfile_in_zip_read->stream.avail_in = (uInt)uReadThis;
		}

		if (uf.pfile_in_zip_read->compression_method==0)
		{
			uInt uDoCopy;
			if (uf.pfile_in_zip_read->stream.avail_out < 
                            uf.pfile_in_zip_read->stream.avail_in)
				uDoCopy = uf.pfile_in_zip_read->stream.avail_out ;
			else
				uDoCopy = uf.pfile_in_zip_read->stream.avail_in ;

			memcpy(uf.pfile_in_zip_read->stream.next_out, 
				uf.pfile_in_zip_read->stream.next_in, uDoCopy);
					
			uf.pfile_in_zip_read->crc32 = crc32(uf.pfile_in_zip_read->crc32,
								uf.pfile_in_zip_read->stream.next_out,
								uDoCopy);

			uf.pfile_in_zip_read->rest_read_uncompressed-=uDoCopy;
			uf.pfile_in_zip_read->stream.avail_in -= uDoCopy;
			uf.pfile_in_zip_read->stream.avail_out -= uDoCopy;
			uf.pfile_in_zip_read->stream.next_out += uDoCopy;
			uf.pfile_in_zip_read->stream.next_in += uDoCopy;
            uf.pfile_in_zip_read->stream.total_out += uDoCopy;
			iRead += uDoCopy;
		}
		else
		{
			uLong uTotalOutBefore = uf.pfile_in_zip_read->stream.total_out;
			const Bytef *bufBefore = uf.pfile_in_zip_read->stream.next_out;
			int flush=Z_SYNC_FLUSH;

			/*
			if ((uf.pfile_in_zip_read->rest_read_uncompressed ==
			         uf.pfile_in_zip_read->stream.avail_out) &&
				(uf.pfile_in_zip_read->rest_read_compressed == 0))
				flush = Z_FINISH;
			*/
			int err = inflate(&uf.pfile_in_zip_read->stream,flush);

			uLong uTotalOutAfter = uf.pfile_in_zip_read->stream.total_out;
			uLong uOutThis = uTotalOutAfter-uTotalOutBefore;
			
			uf.pfile_in_zip_read->crc32 = 
                crc32(uf.pfile_in_zip_read->crc32,bufBefore,
                        (uInt)(uOutThis));

			uf.pfile_in_zip_read->rest_read_uncompressed -=
                uOutThis;

			iRead += (uInt)(uTotalOutAfter - uTotalOutBefore);
            
			if (err==Z_STREAM_END)
				return iRead;

			CheckForError(err);
		}
	}

	return iRead;
}

/*
  Read extra field from the current file (opened by OpenCurrentFile)
  This is the local-header version of the extra field (sometimes, there is
    more info in the local-header version than in the central-header)

  if buf==NULL, it return the size of the local extra field that can be read

  if buf!=NULL, len is the size of the buffer, the extra header is copied in
	buf.
  the return value is the number of bytes copied in buf
*/
int CUnzipFile::GetLocalExtrafield (void* buf, UINT len)
{
	if (!uf.pfile_in_zip_read)
		ThrowError(UNZ_PARAMERROR);


	uLong size_to_read = (uf.pfile_in_zip_read->size_local_extrafield - 
				uf.pfile_in_zip_read->pos_local_extrafield);

	if (!buf)
		return (int)size_to_read;
	
	uInt read_now;

	if (len>size_to_read)
		read_now = (uInt)size_to_read;
	else
		read_now = (uInt)len ;

	if (!read_now)
		return 0;

	
	uf.file.Seek(uf.pfile_in_zip_read->offset_local_extrafield + 
			  uf.pfile_in_zip_read->pos_local_extrafield, CFile::begin);


	return (int)uf.file.Read(buf, read_now);
}
/*
  Get the global comment string of the ZipFile, in the szComment buffer.
  uSizeBuf is the size of the szComment buffer.
  return the number of byte copied
*/
int CUnzipFile::GetGlobalComment (char* szComment, uLong uSizeBuf)
{

	uLong uReadThis = uSizeBuf;
	if (uReadThis > uf.gi.size_comment)
		uReadThis = uf.gi.size_comment;

	uf.file.Seek(uf.central_pos+22, CFile::begin);

	if (uReadThis)
    {
      *szComment = '\0';
	  uReadThis = uf.file.Read(szComment, (uInt)uReadThis);
    }

	if (szComment && (uSizeBuf > uf.gi.size_comment))
		*(szComment+uf.gi.size_comment)='\0';

	return (int)uReadThis;
}

/*
  Give the current position in uncompressed data
*/
z_off_t CUnzipFile::tell()
{
	if (!uf.pfile_in_zip_read)
		ThrowError(UNZ_PARAMERROR);

	return (z_off_t)uf.pfile_in_zip_read->stream.total_out;
}


/*
  return true if the end of file was reached, false elsewhere 
*/
bool CUnzipFile::eof()
{

	if (!uf.pfile_in_zip_read)
		ThrowError(UNZ_PARAMERROR);
	
	return uf.pfile_in_zip_read->rest_read_uncompressed == 0;
}

CUnzipFile::~CUnzipFile()
{
// 	Close(); // cannot be here: if an exception is thrown strange things can happen

}

void CUnzipFile::unzlocal_getByte(int & pi)
{

    unsigned char c;
	uf.file.Read(&c, 1);
	pi = (int)c;
    
}

void CUnzipFile::unzlocal_getShort (uLong & pX)
{
    int i;

    unzlocal_getByte(i);
    uLong x = (uLong)i;
    unzlocal_getByte(i);
    x += ((uLong)i)<<8;
   
	pX = x;
}

void CUnzipFile::unzlocal_getLong (uLong & pX)
{
	

	uLong x;
	unzlocal_getShort(x);
	uLong y;
	unzlocal_getShort(y);
	x += y << 16;
    pX = x;
}

//    Compare two filename (fileName1,fileName2).
int CUnzipFile::StringFileNameCompare(CString fileName1, CString fileName2, bool caseSensitive)
{
	return caseSensitive ? fileName1.Collate(fileName2) : 	fileName1.CollateNoCase(fileName2);
}

/*
  Locate the Central directory of a zipfile (at the end, just before
    the global comment)
*/
uLong CUnzipFile::unzlocal_SearchCentralDir()
{
	
	uLong uMaxBack=0xffff; /* maximum size of global comment */

	uLong uSizeFile = uf.file.GetLength();

	if (uMaxBack > uSizeFile)
		uMaxBack = uSizeFile;

	char* buf = new char[BUFREADCOMMENT + 4];

	uLong uBackRead = 4;
	uLong uPosFound = 0;

	try
	{
		while ((uBackRead < uMaxBack) && !uPosFound)
		{
			uLong uReadSize,uReadPos ;

			if (uBackRead + BUFREADCOMMENT > uMaxBack) 
				uBackRead = uMaxBack;
			else
				uBackRead += BUFREADCOMMENT;

			uReadPos = uSizeFile - uBackRead ;
			
			uReadSize = ((BUFREADCOMMENT + 4) < (uSizeFile - uReadPos)) ? 
						 (BUFREADCOMMENT + 4) : (uSizeFile - uReadPos);

			uf.file.Seek(uReadPos, CFile::begin);
			uf.file.Read(buf, uReadSize);

			for (int i= (int)uReadSize - 3; (i--) > 0 ;)
				if (((*(buf+i)) == 0x50) && ((*(buf+i+1))==0x4b) && 
					((*(buf+i+2))==0x05) && ((*(buf+i+3))==0x06))
				{
					uPosFound = uReadPos + i;
					break;
				}

		}
	}
	catch (CException*)
	{
		delete[] buf;
		throw;
	}
	delete[] buf;

	if (!uPosFound)
		ThrowError(UNZ_BADZIPFILE);

	return uPosFound;
	
}

/*
   Translate date/time from Dos format to tm_unz (readable more easilty)
*/
void CUnzipFile::unzlocal_DosDateToTmuDate(unz_file_info &file_info)
{
	CTime t(HIWORD(file_info.dosDate), LOWORD(file_info.dosDate));
	file_info.tmu_date = t;
}


/*
  Get Info about the current file in the zipfile, with internal only info
*/
void CUnzipFile::unzlocal_GetCurrentFileInfoInternal( unz_file_info & file_info,
                                         unz_file_info_internal & file_info_internal,
                                         LPSTR szFileName,
										 uLong fileNameBufferSize,
										 void *extraField,
										 uLong extraFieldBufferSize,
										 LPSTR szComment,
										 uLong commentBufferSize)
{

	
	uf.file.Seek(uf.pos_in_central_dir + uf.byte_before_the_zipfile, CFile::begin);
	
	uLong uMagic;

	/* we check the magic */
	unzlocal_getLong(uMagic);
	if (uMagic != 0x02014b50)
		ThrowError(UNZ_BADZIPFILE);

	unzlocal_getShort(file_info.version);
	unzlocal_getShort(file_info.version_needed);
	unzlocal_getShort(file_info.flag);
	unzlocal_getShort(file_info.compression_method);
	unzlocal_getLong(file_info.dosDate);

    unzlocal_DosDateToTmuDate(file_info);

	unzlocal_getLong(file_info.crc);
	unzlocal_getLong(file_info.compressed_size);
	unzlocal_getLong(file_info.uncompressed_size);
	unzlocal_getShort(file_info.size_filename);
	unzlocal_getShort(file_info.size_file_extra);
	unzlocal_getShort(file_info.size_file_comment);
	unzlocal_getShort(file_info.disk_num_start);
	unzlocal_getShort(file_info.internal_fa);
	unzlocal_getLong(file_info.external_fa);
	unzlocal_getLong(file_info_internal.offset_curfile);

	uLong lSeek = file_info.size_filename;

	if (szFileName)
	{
		uLong uSizeRead ;
		if (file_info.size_filename < fileNameBufferSize)
		{
			*(szFileName + file_info.size_filename) = '\0';
			uSizeRead = file_info.size_filename;
		}
		else
			uSizeRead = fileNameBufferSize;

		if ((file_info.size_filename>0) && (fileNameBufferSize>0))
			uf.file.Read(szFileName, uSizeRead);

		lSeek -= uSizeRead;
	}

	
	if (extraField)
	{
		uLong uSizeRead ;
		if (file_info.size_file_extra < extraFieldBufferSize)
			uSizeRead = file_info.size_file_extra;
		else
			uSizeRead = extraFieldBufferSize;

		if (lSeek != 0)
		{
			uf.file.Seek(lSeek, CFile::begin);
			lSeek=0;
		}
		
		if ((file_info.size_file_extra>0) && (extraFieldBufferSize>0))
			uf.file.Read(extraField, uSizeRead);

		lSeek += file_info.size_file_extra - uSizeRead;
	}
	else
		lSeek+=file_info.size_file_extra; 

	
	if (szComment)
	{
		uLong uSizeRead ;

		if (file_info.size_file_comment<commentBufferSize)
		{
			*(szComment+file_info.size_file_comment)='\0';
			uSizeRead = file_info.size_file_comment;
		}
		else
			uSizeRead = commentBufferSize;

		if (lSeek != 0)
		{
			uf.file.Seek(lSeek, CFile::begin);
			lSeek=0;
		}

		if ((file_info.size_file_comment>0) && (commentBufferSize>0))
			uf.file.Read(szComment, uSizeRead);
			
		lSeek+=file_info.size_file_comment - uSizeRead;
	}
	else
		lSeek+=file_info.size_file_comment;

}


void CUnzipFile::Open(LPCTSTR lpszPath)
{
	if (!IsClosed())
		return;

	CFileException* e = new CFileException;
	if (!uf.file.Open(lpszPath, CFile::modeRead | CFile::shareDenyWrite, e))
		throw e;
	e->Delete();

	uLong central_pos = unzlocal_SearchCentralDir();

	uf.file.Seek(central_pos, CFile::begin);

	/* the signature, already checked */
	uLong uL;
	unzlocal_getLong(uL);

	uLong number_disk;          /* number of the current dist, used for 
								   spaning ZIP, unsupported, always 0*/
	unzlocal_getShort(number_disk);

	uLong number_disk_with_CD;  /* number the the disk with central dir, used
								   for spaning ZIP, unsupported, always 0*/
	unzlocal_getShort(number_disk_with_CD);

	/* number of the disk with the start of the central directory */
	unzlocal_getShort(uf.gi.number_entry);


	uLong number_entry_CD;      /* total number of entries in
	                               the central dir 
	                               (same than number_entry on nospan) */
	unzlocal_getShort(number_entry_CD);

	
	if ( (number_entry_CD != uf.gi.number_entry) ||
		(number_disk_with_CD != 0) ||
		(number_disk != 0))
			ThrowError(UNZ_BADZIPFILE);

	/* size of the central directory */
	unzlocal_getLong(uf.size_central_dir);

	/* offset of start of central directory with respect to the 
	      starting disk number */
	unzlocal_getLong(uf.offset_central_dir);

	/* zipfile comment length */
	unzlocal_getShort(uf.gi.size_comment);

	if ( central_pos < uf.offset_central_dir + uf.size_central_dir)
		ThrowError(UNZ_BADZIPFILE);


	uf.byte_before_the_zipfile = central_pos -
		(uf.offset_central_dir + uf.size_central_dir);

	uf.central_pos = central_pos;
	
	GoToFirstFile();	

}

void CUnzipFile::UpdateFileStatus(CFile &f, unz_file_info &ui)
{
	CString s = f.GetFilePath();
	f.Close();
	CFileStatus fs;
	fs.m_ctime = fs.m_atime = CTime::GetCurrentTime();
	fs.m_attribute = 0;
	fs.m_mtime = ui.tmu_date;
	CFile::SetStatus(s, fs);
	SetFileAttributes(s, ui.external_fa);

}

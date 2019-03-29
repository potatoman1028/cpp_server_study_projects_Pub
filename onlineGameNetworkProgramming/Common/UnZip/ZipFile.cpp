// ZipFile.cpp: implementation of the CZipFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZipFile.h"


#ifndef VERSIONMADEBY
# define VERSIONMADEBY   (0x0) /* platform depedent */
#endif

#define FLAG_LOCALHEADER_OFFSET (0x06)
#define CRC_LOCALHEADER_OFFSET  (0x0e)

#define SIZECENTRALHEADER (0x2e) /* 46 */



#ifndef Z_BUFSIZE
#define Z_BUFSIZE (16384)
#endif

#ifndef Z_MAXFILENAMEINZIP
#define Z_MAXFILENAMEINZIP (256)
#endif

#ifndef DEF_MEM_LEVEL
#if MAX_MEM_LEVEL >= 8
#  define DEF_MEM_LEVEL 8
#else
#  define DEF_MEM_LEVEL  MAX_MEM_LEVEL
#endif
#endif

#define SIZEDATA_INDATABLOCK (4096-(4*4))

#define LOCALHEADERMAGIC    (0x04034b50)
#define CENTRALHEADERMAGIC  (0x02014b50)
#define ENDHEADERMAGIC      (0x06054b50)


#define SIZECENTRALHEADER (0x2e) /* 46 */

// it doesn't compile anyway
// const char z_copyright[] =
//    " unzip & zip 0.15 Copyright 1998 Gilles Vollant & MFC/C++ revision Copyright 2000 Tadeusz Dracz";

#define ZIP_PARAMERROR                  (-102)

zip_fileinfo::zip_fileinfo()
{
	tmz_date = CTime::GetCurrentTime();
	dosDate = internal_fa = external_fa = 0;
}

uLong zip_fileinfo::get_dos_date()
{
	if (dosDate)
		return dosDate;

    uLong year = (uLong)tmz_date.GetYear();
    if (year>1980)
        year-=1980;
    else if (year>80)
        year-=80;
    return
      (uLong) ((tmz_date.GetDay() + 32 * tmz_date.GetMonth() + 512 * year) << 16) |
        (tmz_date.GetSecond() / 2 + 32 * tmz_date.GetMinute() + 2048 * (uLong)tmz_date.GetHour());

}

void curfile_info::alloc_central_header()
{
	ASSERT(!central_header);
	if (size_centralheader)
		central_header = new char[size_centralheader];
}

void curfile_info::free_central_header()
{
	if (central_header)
	{
		delete[] central_header;
		central_header = NULL;
	}
}


curfile_info::curfile_info()
{
	buffered_data = new Byte [Z_BUFSIZE];
	central_header = NULL;
}

curfile_info::~curfile_info()
{
	free_central_header();
	delete[] buffered_data;	
}

linkedlist_datablock_internal::linkedlist_datablock_internal()
{
	next_datablock = NULL;
	avail_in_this_block = SIZEDATA_INDATABLOCK;
	filled_in_this_block = 0;
	data = new unsigned char [SIZEDATA_INDATABLOCK];
}

linkedlist_datablock_internal::~linkedlist_datablock_internal()
{
	delete[] data;
}


linkedlist_data::linkedlist_data()
{
	first_block = last_block = NULL;
}

linkedlist_data::~linkedlist_data()
{
	linkedlist_datablock_internal* b = first_block;
	while (b)
	{
		linkedlist_datablock_internal* a = b->next_datablock;
		delete b;
		b = a;
	}
}


void linkedlist_data::add_data_in_datablock(char* buf, uLong len)
{
    if (!last_block)
        first_block = last_block = new linkedlist_datablock_internal;

    linkedlist_datablock_internal* ldi = last_block;

    while (len>0)
    {
        uInt copy_this;
        unsigned char* to_copy;

        if (ldi->avail_in_this_block == 0)
        {
            ldi->next_datablock = new linkedlist_datablock_internal;
            ldi = ldi->next_datablock ;
            last_block = ldi;
        }

        if (ldi->avail_in_this_block < len)
            copy_this = (uInt)ldi->avail_in_this_block;
        else
            copy_this = (uInt)len;

        to_copy = &(ldi->data[ldi->filled_in_this_block]);

		memcpy(to_copy, buf, copy_this);

        ldi->filled_in_this_block += copy_this;
        ldi->avail_in_this_block -= copy_this;
        buf += copy_this ;
        len -= copy_this;
    }
}

int linkedlist_data::write_datablock(CFile & f)
{
    linkedlist_datablock_internal* ldi;
    ldi = first_block;
	int size = 0;
    while (ldi)
    {
        if (ldi->filled_in_this_block > 0)
			f.Write(ldi->data, (uInt)ldi->filled_in_this_block);

		size += ldi->filled_in_this_block;
        ldi = ldi->next_datablock;
    }
	return size;
}

zip_internal::zip_internal()
{
	in_opened_file_inzip = 0;
	ci.stream_initialised = 0;
	number_entry = 0;
}
zip_internal::~zip_internal()
{

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CZipFile::CZipFile()
{

	m_pFile = &zi.filezip;
}


CZipFile::CZipFile(LPCTSTR pathname, bool append)
{
	m_pFile = &zi.filezip;
	Open(pathname, append);
}

void CZipFile::Open(LPCTSTR pathname, bool append)
{
	if (!IsClosed())
		return;
	CFileException* e = new CFileException;
	if (!zi.filezip.Open(pathname, (append ? CFile::modeNoTruncate : CFile::modeCreate)|CFile::modeWrite | CFile::shareDenyWrite, e))
		throw e;
	e->Delete();

	zi.filezip.SeekToEnd();
	zi.begin_pos = zi.filezip.GetPosition();
	
}


CZipFile::~CZipFile()
{
// 	Close(); // cannot be here: if an exception is thrown strange things can happen
}

/* ===========================================================================
   Outputs a long in LSB order to the given file
   nbByte == 1, 2 or 4 (byte, short or long)
*/

void CZipFile::ziplocal_putValue(uLong x, int nbByte)
{
    unsigned char buf[4];
    int n;
    for (n = 0; n < nbByte; n++) {
        buf[n] = (unsigned char)(x & 0xff);
        x >>= 8;
    }
    zi.filezip.Write(buf, nbByte);
}

void CZipFile::ziplocal_putValue_inmemory(Byte dest, uLong x, int nbByte)
{
    unsigned char* buf=(unsigned char*)zi.ci.central_header + dest;
    int n;
    for (n = 0; n < nbByte; n++) 
	{
        buf[n] = (unsigned char)(x & 0xff);
        x >>= 8;
	}
}


// 
void CZipFile::OpenNewFileInZip (CString filename, 
								zip_fileinfo& zipfi, 
								int level,
								CString comment, 
								const void* extrafield_local, 
								uInt size_extrafield_local, 
								const void* extrafield_global, 
								uInt size_extrafield_global, 
								int method)
{
    if ((method != Z_DEFLATED) || (!level)) 
        ThrowError(ZIP_PARAMERROR);


    if (zi.in_opened_file_inzip == 1)
        CloseFileInZip();

	if (filename.IsEmpty())
		filename = "-";

	zi.ci.dosDate = zipfi.get_dos_date();

    zi.ci.flag = 0;
    if ((level == 8) || (level == 9))
      zi.ci.flag |= 2;
    if (level == 2)
      zi.ci.flag |= 4;
    if (level == 1)
      zi.ci.flag |= 6;

    zi.ci.crc32 = 0;
    zi.ci.method = method;
    zi.ci.stream_initialised = 0;
    zi.ci.pos_in_buffered_data = 0;
    zi.ci.pos_local_header = zi.filezip.GetPosition();
    zi.ci.size_centralheader = SIZECENTRALHEADER + filename.GetLength() + 
                                      size_extrafield_global + comment.GetLength();

    zi.ci.alloc_central_header();

    ziplocal_putValue_inmemory(0, (uLong)CENTRALHEADERMAGIC,4);
    /* version info */
    ziplocal_putValue_inmemory(4,(uLong)VERSIONMADEBY,2);
    ziplocal_putValue_inmemory(6,(uLong)20, 2);
    ziplocal_putValue_inmemory(8,(uLong)zi.ci.flag,2);
    ziplocal_putValue_inmemory(10,(uLong)zi.ci.method,2);
    ziplocal_putValue_inmemory(12,(uLong)zi.ci.dosDate,4);
    ziplocal_putValue_inmemory(16,(uLong)0, 4); /*crc*/
    ziplocal_putValue_inmemory(20,(uLong)0, 4); /*compr size*/
    ziplocal_putValue_inmemory(24,(uLong)0, 4); /*uncompr size*/
    ziplocal_putValue_inmemory(28,(uLong)filename.GetLength(), 2);
    ziplocal_putValue_inmemory(30,(uLong)size_extrafield_global,2);
    ziplocal_putValue_inmemory(32,(uLong)comment.GetLength(), 2);
    ziplocal_putValue_inmemory(34,(uLong)0, 2); /*disk nm start*/

	ziplocal_putValue_inmemory(36,(uLong)zipfi.internal_fa, 2); 
	ziplocal_putValue_inmemory(38,(uLong)zipfi.external_fa, 4);

    ziplocal_putValue_inmemory(42,(uLong)zi.ci.pos_local_header, 4);

	char* pDest = zi.ci.central_header + SIZECENTRALHEADER;
	memcpy(pDest, (LPCTSTR)filename, filename.GetLength());
	pDest += filename.GetLength();
	memcpy(pDest, extrafield_global, size_extrafield_global);
	pDest += size_extrafield_global;
	memcpy(pDest, (LPCTSTR)comment, comment.GetLength());

    /* write the local header */
    ziplocal_putValue((uLong)LOCALHEADERMAGIC, 4);
	ziplocal_putValue((uLong)20, 2);/* version needed to extract */
    ziplocal_putValue((uLong)zi.ci.flag, 2);
	ziplocal_putValue((uLong)zi.ci.method, 2);
	ziplocal_putValue((uLong)zi.ci.dosDate, 4);
	ziplocal_putValue((uLong)0, 4); /* crc 32, unknown */
    ziplocal_putValue((uLong)0, 4); /* compressed size, unknown */
	ziplocal_putValue((uLong)0, 4); /* uncompressed size, unknown */
	ziplocal_putValue((uLong)filename.GetLength(), 2);
	ziplocal_putValue((uLong)size_extrafield_local, 2);

	zi.filezip.Write(filename, filename.GetLength());

	if (size_extrafield_local > 0)
		zi.filezip.Write(extrafield_local, size_extrafield_local);

    zi.ci.stream.avail_in = (uInt)0;
    zi.ci.stream.avail_out = (uInt)Z_BUFSIZE;
    zi.ci.stream.next_out = zi.ci.buffered_data;
    zi.ci.stream.total_in = 0;
    zi.ci.stream.total_out = 0;

    if ((zi.ci.method == Z_DEFLATED))
    {
        zi.ci.stream.zalloc = (alloc_func)myalloc;
        zi.ci.stream.zfree = (free_func)myfree;
        zi.ci.stream.opaque = m_bDetectZlibMemoryLeaks ? &m_list : 0;

        int err = deflateInit2(&zi.ci.stream, level,
               Z_DEFLATED, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY);

		CheckForError(err);
        zi.ci.stream_initialised = 1;
    }


        zi.in_opened_file_inzip = 1;

}


void CZipFile::WriteInFileInZip(const void *buf, UINT len)
{
    if (zi.in_opened_file_inzip == 0)
        ThrowError(ZIP_PARAMERROR);

    zi.ci.stream.next_in = (unsigned char*)buf;
    zi.ci.stream.avail_in = len;
    zi.ci.crc32 = crc32(zi.ci.crc32, (unsigned char*)buf, len);

	int err = Z_OK;

    while ((err == Z_OK) && (zi.ci.stream.avail_in > 0))
    {

        if (zi.ci.stream.avail_out == 0)
        {
			zi.filezip.Write(zi.ci.buffered_data, zi.ci.pos_in_buffered_data);
            zi.ci.pos_in_buffered_data = 0;
            zi.ci.stream.avail_out = (uInt)Z_BUFSIZE;
            zi.ci.stream.next_out = zi.ci.buffered_data;
        }

        if (zi.ci.method == Z_DEFLATED)
        {
            uLong uTotalOutBefore = zi.ci.stream.total_out;
            err = deflate(&zi.ci.stream,  Z_NO_FLUSH);
            zi.ci.pos_in_buffered_data += (uInt)(zi.ci.stream.total_out - uTotalOutBefore) ;
        }
        else
        {
            uInt copy_this;
            if (zi.ci.stream.avail_in < zi.ci.stream.avail_out)
                copy_this = zi.ci.stream.avail_in;
            else
                copy_this = zi.ci.stream.avail_out;

			memcpy(zi.ci.stream.next_out, zi.ci.stream.next_in, copy_this);

            zi.ci.stream.avail_in -= copy_this;
            zi.ci.stream.avail_out -= copy_this;
            zi.ci.stream.next_in += copy_this;
            zi.ci.stream.next_out += copy_this;
            zi.ci.stream.total_in += copy_this;
            zi.ci.stream.total_out += copy_this;
            zi.ci.pos_in_buffered_data += copy_this;
        }
    }

	CheckForError(err);

}


void CZipFile::CloseFileInZip()
{
    if (zi.in_opened_file_inzip == 0)    
        return;

	int err = Z_OK;

    zi.ci.stream.avail_in = 0;
    
    if (zi.ci.method == Z_DEFLATED)
        while (err == Z_OK)
		{
			uLong uTotalOutBefore;
			if (zi.ci.stream.avail_out == 0)
			{
				zi.filezip.Write(zi.ci.buffered_data, zi.ci.pos_in_buffered_data);
				zi.ci.pos_in_buffered_data = 0;
				zi.ci.stream.avail_out = (uInt)Z_BUFSIZE;
				zi.ci.stream.next_out = zi.ci.buffered_data;
			}
			uTotalOutBefore = zi.ci.stream.total_out;
			err = deflate(&zi.ci.stream,  Z_FINISH);
			zi.ci.pos_in_buffered_data += (uInt)(zi.ci.stream.total_out - uTotalOutBefore) ;
		}

    if (err == Z_STREAM_END)
        err = Z_OK; /* this is normal */

	CheckForError(err);

	if (zi.ci.pos_in_buffered_data > 0)
		zi.filezip.Write(zi.ci.buffered_data, zi.ci.pos_in_buffered_data);

	if (zi.ci.method == Z_DEFLATED)
	{
		err = deflateEnd(&zi.ci.stream);
		zi.ci.stream_initialised = 0;
	}

	CheckForError(err);

	ziplocal_putValue_inmemory(16, (uLong)zi.ci.crc32, 4); /*crc*/
	ziplocal_putValue_inmemory(20, (uLong)zi.ci.stream.total_out, 4); /*compr size*/
	ziplocal_putValue_inmemory(24, (uLong)zi.ci.stream.total_in, 4); /*uncompr size*/

	zi.central_dir.add_data_in_datablock(zi.ci.central_header, (uLong)zi.ci.size_centralheader);

	zi.ci.free_central_header();

    long cur_pos_inzip = zi.filezip.GetPosition();
	zi.filezip.Seek(zi.ci.pos_local_header + 14, CFile::begin);
	ziplocal_putValue((uLong)zi.ci.crc32, 4); /* crc 32, unknown */
    ziplocal_putValue((uLong)zi.ci.stream.total_out, 4);/* compressed size, unknown */
	ziplocal_putValue((uLong)zi.ci.stream.total_in, 4); /* uncompressed size, unknown */
	zi.filezip.Seek(cur_pos_inzip, CFile::begin);

    zi.number_entry++;
    zi.in_opened_file_inzip = 0;
}

void CZipFile::Close(CString global_comment)
{

	if (IsClosed())
		return;

    if (zi.in_opened_file_inzip == 1)
        CloseFileInZip ();


    uLong centraldir_pos_inzip = zi.filezip.GetPosition();

	uLong size_centraldir = zi.central_dir.write_datablock(zi.filezip);
    

    /* Magic End */
    ziplocal_putValue((uLong)ENDHEADERMAGIC, 4);
	ziplocal_putValue((uLong)0, 2);/* number of this disk */
	ziplocal_putValue((uLong)0, 2);/* number of the disk with the start of the central directory */
    ziplocal_putValue((uLong)zi.number_entry, 2);/* total number of entries in the central dir on this disk */
    ziplocal_putValue((uLong)zi.number_entry,2);/* total number of entries in the central dir */
	ziplocal_putValue((uLong)size_centraldir,4);/* size of the central directory */
	ziplocal_putValue((uLong)centraldir_pos_inzip ,4);/* offset of start of central directory with respect to the starting disk number */
	ziplocal_putValue((uLong)global_comment.GetLength(), 2);/* zipfile comment length */
	if (!global_comment.IsEmpty())
		zi.filezip.Write(global_comment, global_comment.GetLength());

	zi.filezip.Close();

}

void CZipFile::UpdateZipInfo(zip_fileinfo &zi, CFile &f)
{
	CFileStatus fs;
	f.GetStatus(fs);
	zi.tmz_date = fs.m_mtime;
	zi.external_fa = ::GetFileAttributes(f.GetFilePath()); // mfc bug: m_attribute is 1-byte

}

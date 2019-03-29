#include "String.h"

String::String()
	: chData( 0 ), stringLength( 0 ), allocSize( 0 )
{
	allocData( StringBlockSize );
}

String::String( const String& source )
	: chData( 0 ), stringLength( 0 ), allocSize( 0 )
{
	allocData( source.stringLength + 1 );

	_tcscpy( chData, source.chData );

	stringLength = source.stringLength;
}

String::String( LPTSTR str )
	: chData( 0 ), stringLength( 0 ), allocSize( 0 )
{
	*this = str;
}

String::String( LPCTSTR str )
	: chData( 0 ), stringLength( 0 ), allocSize( 0 )
{
	*this = str;
}

String::String( int n, bool hex )
	: chData( 0 ), stringLength( 0 ), allocSize( 0 )
{
	allocData( StringBlockSize );

	if( hex == true )
		format( _T( "%x" ), n );
	else
		format( _T( "%d" ), n );
}

String::~String()
{
	if( chData != 0 )
		delete [] chData;
}

void String::allocData( int strLen )
{
	if( strLen > allocSize )
	{
		while( strLen > allocSize )
			allocSize += StringBlockSize;

		if( chData != 0 )
			delete [] chData;
		chData = new TCHAR [allocSize];

		::ZeroMemory( chData, allocSize );
	}
}

void String::calcStringSize()
{
	int i;

	stringLength = 0;
	for( i = 0 ; i < allocSize ; i++, stringLength++ )
		if( chData[i] == 0 )
			break;
}

void String::format( LPCTSTR strFormat, ... )
{
	TCHAR buffer[1024];		//  The maximum size of the buffer is 1024 bytes in wsprintf func.
	va_list args;

	va_start( args, strFormat );
	wvsprintf( buffer, strFormat, args );
	va_end( args );

	*this = buffer;
}

LPTSTR String::getBuffer()
{
	return chData;
}

bool String::isEmpty()
{
	if( chData == 0 || _tcslen( chData ) == 0 )
		return true;

	return false;
}

void String::empty()
{
	if( chData == 0 )
		return;

	format( _T( "" ) );
}

TCHAR String::getAt( int nIndex )
{
	if( nIndex >= stringLength )
		return 0;

	return chData[nIndex];
}

void String::setAt( int nIndex, TCHAR ch )
{
	if( nIndex >= stringLength )
		return;

	chData[nIndex] = ch;
}

void String::left( int nCount )
{
	chData[nCount] = 0;

	calcStringSize();
}

void String::right( int nCount )
{
	LPTSTR temp = new TCHAR[stringLength + 1];
	int nOffset = stringLength - nCount;

	memcpy( temp, ( chData + nOffset ), sizeof( TCHAR ) * ( nCount + 1 ) );
	memcpy( chData, temp, sizeof( TCHAR ) * ( nCount + 1 ) );

	delete [] ( LPTSTR )temp;
	calcStringSize();
}

void String::makeUpper()
{
	_tcsupr( chData );
}

void String::makeLower()
{
	_tcslwr( chData );
}

int String::compareTo( String& str )
{
	return _tcscmp( getBuffer(), str.getBuffer() );
}

int String::compareTo( LPCTSTR str )
{
	return _tcscmp( getBuffer(), str );
}

int String::compareTo( String& str, int count )
{
	return _tcsncmp( getBuffer(), str.getBuffer(), count );
}

int String::compareTo( LPCTSTR str, int count )
{
	return _tcsncmp( getBuffer(), str, count );
}

LPCWSTR String::toWideChar( LPCSTR str )
{
	static	WCHAR		wideCharBuff[1024];
	int len = strlen( str );

	::MultiByteToWideChar( CP_ACP, 0, str, -1, wideCharBuff, len + 1 );

	return wideCharBuff;
}

LPCSTR String::toMultiByte( LPCWSTR str )
{
	static	CHAR		multiByteBuff[1024];
	int len = wcslen( str );

	::WideCharToMultiByte( CP_ACP, 0, str, -1, multiByteBuff, len + 1, 0, 0 );

	return multiByteBuff;
}

const String& String::operator = ( String& str )
{
	*this = str.getBuffer();

	return *this;
}

const String& String::operator = ( LPTSTR str )
{
	stringLength = _tcslen( str );
	allocData( stringLength + 1 );

	_tcscpy( chData, str );	

	calcStringSize();

	return *this;
}

const String& String::operator = ( LPCTSTR str )
{
	stringLength = _tcslen( str );
	allocData( stringLength + 1 );

	_tcscpy( chData, str );	

	calcStringSize();

	return *this;
}

const String& String::operator += ( String& str )
{
	LPTSTR temp = new TCHAR[allocSize];

	//  Save my string value.
	_tcscpy( temp, chData );

	//  Resizing memory.
	stringLength += str.stringLength;
	allocData( stringLength + 1 );

	//  Recover my string.
	_tcscpy( chData, temp );

	//  Add new string to mine.
	_tcscat( chData, str.chData );

	delete [] ( LPTSTR )temp;

	calcStringSize();

	return *this;
}

const String& String::operator += ( LPTSTR str )
{
	String temp( str );
	
	return ( *this += temp );
}

TCHAR String::operator [] ( int nIndex ) const
{
	if( nIndex >= stringLength )
		return 0;

	return chData[nIndex];
}

String::operator LPTSTR() const
{
	return chData;
}

bool cpp::lang::operator == ( String& str1, String& str2 )
{
	return ( str1.compareTo( str2 ) == 0 );
}

bool cpp::lang::operator == ( String& str1, LPTSTR str2 )
{
	return ( str1.compareTo( str2 ) == 0 );
}

bool cpp::lang::operator == ( LPTSTR str1, String& str2 )
{
	return ( str2.compareTo( str1 ) == 0 );
}

bool cpp::lang::operator != ( String& str1, String& str2 )
{
	return ( str1.compareTo( str2 ) != 0 );
}

bool cpp::lang::operator != ( String& str1, LPTSTR str2 )
{
	return ( str1.compareTo( str2 ) != 0 );
}

bool cpp::lang::operator != ( LPTSTR str1, String& str2 )
{
	return ( str2.compareTo( str1 ) != 0 );
}

String cpp::lang::operator + ( String& ls, String& rs )
{
	String ret;

	ret = ls;
	ret += rs;

	return ret;
}

String cpp::lang::operator + ( String& ls, LPTSTR rs )
{
	String ret;

	ret = ls;
	ret += rs;

	return ret;
}

String cpp::lang::operator + ( LPTSTR ls, String& rs )
{
	String ret;

	ret = ls;
	ret += rs;

	return ret;
}

String cpp::lang::operator + ( String& ls, int n )
{
	String ret, temp;

	temp.format( _T( "%d" ), n );
	ret = ls + temp;

	return ret;
}

String cpp::lang::operator + ( int n, String& rs )
{
	String ret, temp;

	temp.format( _T( "%d" ), n );
	ret = temp + rs;

	return ret;
}

String cpp::lang::operator + ( String& ls, DWORD n )
{
	String ret, temp;

	temp.format( _T( "%d" ), n );
	ret = ls + temp;

	return ret;
}

String cpp::lang::operator + ( DWORD n, String& rs )
{
	String ret, temp;

	temp.format( _T( "%d" ), n );
	ret = temp + rs;

	return ret;
}

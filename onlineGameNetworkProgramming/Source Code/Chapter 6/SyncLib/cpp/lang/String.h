#ifndef  __Sync_String_H
#define  __Sync_String_H

#include <TChar.h>

#include <cpp/lang/Object.h>

namespace cpp
{
	namespace lang
	{
		#define  StringBlockSize		48

		class String : public Object
		{
		public:
					String();
					String( const String& source );
					String( LPTSTR str );
					String( LPCTSTR str );
					String( int n, bool hex = false );
			virtual ~String();

					void		format( LPCTSTR strFormat, ... );
					int			length(){ return stringLength; }

					LPTSTR		getBuffer();

					bool		isEmpty();
					void		empty();

					TCHAR		getAt( int nIndex );
					void		setAt( int nIndex, TCHAR ch );
					void		left( int nCount );
					void		right( int nCount );
					void		makeUpper();
					void		makeLower();

					int			compareTo( String& str );
					int			compareTo( LPCTSTR str );
					int			compareTo( String& str, int count );
					int			compareTo( LPCTSTR str, int count );

			static	LPCWSTR		toWideChar( LPCSTR str );
			static	LPCSTR		toMultiByte( LPCWSTR str );

			const	String&		operator = ( String& str );
			const	String&		operator = ( LPTSTR str );
			const	String&		operator = ( LPCTSTR str );

			const	String&		operator += ( String& str );
			const	String&		operator += ( LPTSTR str );

					TCHAR		operator [] ( int nIndex ) const;

								operator	LPTSTR() const;

			friend	bool		operator == ( String& str1, String& str2 );
			friend	bool		operator == ( String& str1, LPTSTR str2 );
			friend	bool		operator == ( LPTSTR str1, String& str2 );

			friend	bool		operator != ( String& str1, String& str2 );
			friend	bool		operator != ( String& str1, LPTSTR str2 );
			friend	bool		operator != ( LPTSTR str1, String& str2 );

			friend	String		operator + ( String& ls, LPTSTR rs );
			friend	String		operator + ( LPTSTR ls, String& rs );
			friend	String		operator + ( String& ls, String& rs );

			friend	String		operator + ( String& ls, int n );
			friend	String		operator + ( int n, String& rs );
			friend	String		operator + ( String& ls, DWORD n );
			friend	String		operator + ( DWORD n, String& rs );

		protected:
					void		allocData( int strLen );
					void		calcStringSize();

					LPTSTR		chData;
					int			stringLength;
					int			allocSize;
		};
	}
}

#endif
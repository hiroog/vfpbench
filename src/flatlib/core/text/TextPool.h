// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/ut/BinaryPool.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace text {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class TextPool {
public:
	typedef	ut::BinaryPool::SizeType	SizeType;
private:
	ut::BinaryPool	Pool;
	char*			FormatBuffer;
public:
	TextPool( const TextPool& )= delete;
	TextPool( TextPool&& src );
	TextPool& operator=( const TextPool& )= delete;
	TextPool& operator=( TextPool&& );
	TextPool();
	explicit TextPool( SizeType size );
	~TextPool();
	void	Finalize();
	void	AddText( const char* ptr );
	void	AddChar( char ch );
	const char*	GetText() const;
	void*		GetBuffer() const;
	SizeType	GetBufferSize() const;
	SizeType	GetDataSize() const;
	void	AddFormatV( const char* format, va_list ap );
	void	AddFormat( const char* format ... );
	void	SubstSize( SizeType size );
	void	Truncate( SizeType size );
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	BENCH_APPLICATION_H_
#define	BENCH_APPLICATION_H_

#include	<minilib/Platform.h>
#include	<minilib/BinaryBuffer.h>
#include	"ResultData.h"


class ITestBase;

class BenchApplication {
private:

	flatlib::util::FixedArray<ResultData>	DataArray;

public:
	static void	save_size( flatlib::util::BinaryBuffer32& buffer, const void* ptr, unsigned int size );
	static void	save_line( flatlib::util::BinaryBuffer32& buffer, const char* ptr );
	static void	save_format( flatlib::util::BinaryBuffer32& buffer, const char* format, va_list arg );
	static void	print( flatlib::util::BinaryBuffer32& buffer, const char* format ... );

private:
	static void	ExportLine( flatlib::util::BinaryBuffer32& buffer, const ResultLine& line );
	static void	ExportData( flatlib::util::BinaryBuffer32& buffer, const ResultData& data );

	void	LoadLine( ResultLine& line, const char* ptr );
	void	SaveLine( flatlib::util::BinaryBuffer32& buffer, const ResultLine& line ) const;
	void	SaveData( flatlib::util::BinaryBuffer32& buffer, const ResultData& data ) const;
public:

	BenchApplication();
	void	Init( unsigned int bench_count );
	void	InitData( unsigned int btype, ITestBase* bench );

	unsigned int	GetDataCount() const
	{
		return	DataArray.GetSize();
	}

	const ResultData& GetData( unsigned int data_index ) const
	{
		return	DataArray[data_index];
	}

	ResultData& GetData( unsigned int data_index )
	{
		return	DataArray[data_index];
	}

	void	UpdateResult( unsigned int btype, ITestBase* bench );

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	void	ExportCPUInfo( flatlib::util::BinaryBuffer32& buffer ) const;
	void	ExportFlops( flatlib::util::BinaryBuffer32& buffer ) const;
	void	ExportLog( flatlib::util::BinaryBuffer32& buffer ) const;

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	void	SaveFile( const char* file_name ) const;
	void	LoadFile( const char* file_name );

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	double	GetMFLOPS( unsigned int loop_type, bool multithread, unsigned int group ) const;
	double	GetTotalMFLOPS( unsigned int loop_type, bool multithread ) const;

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
};

#endif







// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	BENCH_APPLICATION_H_
#define	BENCH_APPLICATION_H_

#include	<flatlib/core/core.h>
#include	<flatlib/core/ut/FixedArray.h>
#include	<flatlib/core/text/TextPool.h>
#include	"ResultData.h"


class ITestBase;

class BenchApplication {
private:

	flatlib::ut::FixedArray<ResultData>	DataArray;
	char	DateTimeStr[24];
private:
	static void	ExportLine( flatlib::text::TextPool& buffer, const ResultLine& line );
	static void	ExportData( flatlib::text::TextPool& buffer, const ResultData& data );

	void	LoadLine( ResultLine& line, const char* ptr );
	void	SaveLine( flatlib::text::TextPool& buffer, const ResultLine& line ) const;
	void	SaveData( flatlib::text::TextPool& buffer, const ResultData& data ) const;
public:

	BenchApplication();
	void	Init( unsigned int bench_count );
	void	InitData( unsigned int btype, ITestBase* bench );

	unsigned int	GetDataCount() const
	{
		return	DataArray.GetDataSize();
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
	void	UpdateTimestamp();

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	void	ExportCPUInfo( flatlib::text::TextPool& buffer ) const;
	void	ExportFlops( flatlib::text::TextPool& buffer ) const;
	void	ExportLog( flatlib::text::TextPool& buffer ) const;

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	void	SaveFile( const char* file_name ) const;
	void	LoadFile( const char* file_name );

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	double	GetMFLOPS( unsigned int loop_type, bool multithread, unsigned int group ) const;
	double	GetTotalMFLOPS( unsigned int loop_type, bool multithread ) const;
	double	GetMaxMFLOPS( unsigned int loop_type, bool multithread ) const;

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
};

#endif



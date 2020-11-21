// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	RESULT_DATA_H_
#define	RESULT_DATA_H_

#include	<minilib/CoreLib.h>
#include	<minilib/FixedArray.h>


struct ResultLine {
public:
	const char*	Title;
	double		Time;
	double		Flops;
	double		Ops;
	double		Fop;
	double		Ipc;
	double		Max;
public:
	ResultLine() : Title( nullptr ), Time( 0.0 ), Flops( 0.0 ), Ops( 0.0 ), Fop( 0.0 ), Ipc( 0.0 ), Max( 0.0 )
	{
	}
	void Clear()
	{
		Title= nullptr;
		Time= 0.0;
		Flops= 0.0;
		Ops= 0.0;
		Fop= 0.0;
		Ipc= 0.0;
		Max= 0.0;
	}
	bool IsActive() const
	{
		return	Time >= 1e-9;
	}
};





class ResultData {
private:

	flatlib::ut::FixedArray<ResultLine>	DataList;
	unsigned int	UpdatePosition= 0;

	const char*		Title= nullptr;
	unsigned int	LoopType= 0;
	unsigned int	BenchIndex= 0;		// benchmark index
	unsigned int	CoreGroup= 0;		// group index
	unsigned int	CoreClock= 0;		// core clock
	bool			Multithread= false;

public:
	ResultData()
	{
	}
	void	InitSize( unsigned int size );
	void	SetInfo( const char* title, unsigned int loop_type, unsigned int bench_index, bool multithread, unsigned int group, unsigned int clock )
	{
		Title= title;
		LoopType= loop_type;
		BenchIndex= bench_index;
		CoreGroup= group;
		CoreClock= clock;
		Multithread= multithread;
	}
	const char*	GetTitle() const
	{
		return	Title;
	}
	unsigned int	GetLoopType() const
	{
		return	LoopType;
	}
	bool	IsMultithread() const
	{
		return	Multithread;
	}
	unsigned int	GetCoreGroup() const
	{
		return	CoreGroup;
	}

	unsigned int	GetBenchIndex() const
	{
		return	BenchIndex;
	}

	void	Clear()
	{
		DataList.Clear();
		UpdatePosition= 0;
	}


	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	void	UpdateBegin()
	{
		UpdatePosition= 0;
	}
	void	UpdateEnd();
	void	Update( double time, double flops, double ops, double fop, double ipc );


	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	const ResultLine&	Get( int index ) const
	{
		return	DataList[ index ];
	}

	ResultLine&	Get( int index )
	{
		return	DataList[ index ];
	}

	unsigned int	GetSize() const
	{
		return	DataList.GetSize();
	}

	const ResultLine&	GetAverage() const
	{
		return	Get( GetSize() -2 );
	}

	ResultLine&	GetAverage()
	{
		return	Get( GetSize() -2 );
	}

	const ResultLine&	GetHighest() const
	{
		return	Get( GetSize() -1 );
	}


	ResultLine&	GetHighest()
	{
		return	Get( GetSize() -1 );
	}



	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	void	Dump();
};






#endif


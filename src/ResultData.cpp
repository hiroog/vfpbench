// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	"ResultData.h"


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void	ResultData::InitSize( unsigned int size )
{
	Clear();
	DataList.Init( size + 2 );

	for( unsigned int si= 0 ; si< size ; si++ ){
		ResultLine&	line= Get( si );
		line.Clear();
	}
	GetAverage().Title= "Average";
	GetHighest().Title= "Highest";
}


void	ResultData::UpdateEnd()
{
	unsigned int	count= GetSize() - 2;
	unsigned int	active_count= 0;
	double	total_time= 0;
	double	total_flops= 0;
	double	total_highest= 0;
	double	total_ops= 0;
	double	total_fop= 0;
	double	total_ipc= 0;
	double	min_time= 1e30;
	double	max_flops= 0;
	double	max_highest= 0;
	double	max_ops= 0;
	double	max_fop= 0;
	double	max_ipc= 0;
	for( int ci= 0 ; ci< count ; ci++ ){
		ResultLine&	line= Get( ci );
		if( !line.IsActive() ){
			continue;
		}
		active_count++;
		
		total_time+= line.Time;
		total_flops+= line.Flops;
		total_highest+= line.Max;
		total_ops+= line.Ops;
		total_fop+= line.Fop;
		total_ipc+= line.Ipc;
		if( line.Time < min_time ){
			min_time= line.Time;
		}
		if( line.Flops > max_flops ){
			max_flops= line.Flops;
		}
		if( line.Max > max_highest ){
			max_highest= line.Max;
		}
		if( line.Ops > max_ops ){
			max_ops= line.Ops;
		}
		if( line.Fop > max_fop ){
			max_fop= line.Fop;
		}
		if( line.Ipc > max_ipc ){
			max_ipc= line.Ipc;
		}
	}
	if( active_count <= 0 ){
		return;
	}

	ResultLine&	avarage= GetAverage();
	avarage.Time= total_time / active_count;
	avarage.Flops= total_flops / active_count;
	avarage.Max= total_highest / active_count;
	avarage.Ops= total_ops / active_count;
	avarage.Fop= total_fop / active_count;
	avarage.Ipc= total_ipc / active_count;

	ResultLine&	highest= GetHighest();
	highest.Time= min_time;
	highest.Flops= max_flops;
	highest.Max= max_highest;
	highest.Ops= max_ops;
	highest.Fop= max_fop;
	highest.Ipc= max_ipc;
}

void	ResultData::Update( double time, double flops, double ops, double fop, double ipc )
{
	if( UpdatePosition >= GetSize() ){
		FL_LOG( "ResultData access error\n" );
		return;
	}
	ResultLine&	line= Get( UpdatePosition );
	line.Time= time;
	line.Flops= flops;
	if( flops > line.Max ){
		line.Max= flops;
	}
	line.Ops= ops;
	line.Fop= fop;
	line.Ipc= ipc;
	UpdatePosition++;
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void ResultData::Dump()
{
	unsigned int	count= GetSize();
	FL_PRINT( "** %s  (loop=%d, mt=%d)\n", GetTitle(), GetLoopType(), IsMultithread() );
	for( unsigned int ci= 0 ; ci< count ; ci++ ){
		const ResultLine&	line= Get( ci );
		if( line.IsActive() ){
			FL_PRINT( "%-30s: %8.3f sec  %9.1f MFLOPS %9.1f MOPS %3.0f/%3.1f (%9.1f MFLOPS)\n", line.Title, line.Time, line.Flops, line.Ops, line.Fop, line.Ipc, line.Max );
		}else{
			FL_PRINT( "%-30s:        - sec          - MFLOPS         - MOPS         (        - MFLOPS)\n", line.Title );
		}
	}
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



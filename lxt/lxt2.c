/*****************************************************************************
 * Copyright 1994-2005, Elliot Mednick and Mark Hummel
 * This file is part of Veriwell.
 *
 * Veriwell is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Veriwell is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <veriuser.h>
#include <acc_user.h>
#include <lxt2_write.h>

#define DEBUG 0

/*****************************************************
 *
 *	lxt2.c	
 * 	- pli interface routines to dump lxt2 files
 *
 *****************************************************
 */
/*
 * add this to veriuser.c
 * { usertask, 0, lxt2_recordvars, 0, lxt2_recordvars, lxt2_recordvars, "$lxt2_recordvars" },
 * { usertask, 0, lxt2_recordon, 0, lxt2_recordon, lxt2_recordon, "$lxt2_recordon" },
 * { usertask, 0, lxt2_recordoff, 0, lxt2_recordoff, lxt2_recordoff, "$lxt2_recordoff" },
 * { usertask, 0, lxt2_recordclose, 0, lxt2_recordclose, lxt2_recordclose, "$lxt2_recordclose" },
 * { usertask, 0, lxt2_recordfile, 0, lxt2_recordfile, lxt2_recordfile, "$lxt2_recordfile" },
 * { usertask, 0, lxt2_recordsetup, 0, lxt2_recordsetup, lxt2_recordsetup, "$lxt2_recordsetup" },
 */

/*
 * supported options:
 *  incsize=#
 *  sequence
 *  nosequence
 *  design=name
 *  depth=#
 *  speed
 *  space
 */


struct info_s {
    handle	      object;
    char*             name;
    int		      sequence:1;
    int		      event:1;
    int		      real:1;
    int		      pad:29;
    struct info_s*    next;
    struct info_s*    updateNext;
    struct lxt2_wr_symbol* symbol;
};
typedef struct info_s info_t, *info_p;


/*
 * global control structure
 */
struct lxt2_s {
    char*            filename;
    int	             enabled;
    int	             sequence;
    char*            design;
    int		     compress;
    unsigned         incSize;
    struct lxt2_wr_trace* t;
    int	             inited;
    int		     depth;
    info_p	     objectList;    
    info_p	     updateList;    
    info_p	     eventList;    
    int		     hunk;
};
typedef struct lxt2_s lxt2_t, *lxt2_p;
static lxt2_t lxt = { 0, 0, 0, 0, 1, 0x40000000UL, 0, 0, 0, 0, 0, 0, 0 };

static void lxt2_close();
static void lxt2_enable( int on );
static void lxt2_option( char* str );
static void lxt2_add( handle object, int depth );
static void lxt2_init();
static void lxt2_dump( info_p info, int init );
static int  lxt2_changed( p_vc_record vc_record );
static void lxt2_timemarker();
static void lxt2_timemarkerp1();
static void lxt2_nexttimemarker();
static char* ginstance;


/* ??? mdh - speed up variable dumping */

/*************************************************
   lxt2_recordvars
   - add objects to be recorded
 ************************************************/
int lxt2_recordvars( int data, int reason )
{
	int update = 0;
	int objects = 0;
	int i;
	acc_initialize();

	switch( reason ) {
	case reason_calltf:
		break;
	case reason_checktf:
		goto DONE;	
	case reason_finish:
		if( lxt.inited ) {
		    lxt2_close();
		}
		goto DONE;	
	case reason_rosynch:
		update = (lxt.updateList != NULL);
		while( lxt.updateList ) {
			info_p info;
			info = lxt.updateList;
			lxt2_dump( info, 0 );
			lxt.updateList = info->updateNext;
			info->updateNext = 0;
		}
		if( update ) {
		    lxt2_timemarkerp1(); 
		}
		while( lxt.eventList ) {
			info_p info;
			info = lxt.eventList;
			lxt2_dump( info, 1 );
			lxt.eventList = info->updateNext;
			info->updateNext = 0;
		}
		lxt2_nexttimemarker(); 
		goto DONE;	
	default:
		goto DONE;	
	}

	ginstance = tf_getinstance();
	/*
	 * parse options first
	 */
	for( i = 1; i <= tf_nump(); ++i ) {
	    handle object;
	    if( tf_typep(i) == tf_nullparam ) {
		continue;
	    }
	    if( tf_typep(i) == tf_string ) {
		char* str = acc_fetch_tfarg_str(i);
		lxt2_option( str );
		continue;
	    } 
	}
	/*
	 * on first call, initialize structure
	 */
	if( !lxt.inited ) {
	    lxt2_init();
	}
	for( i = 1; i <= tf_nump(); ++i ) {
	    handle object;
	    if( tf_typep(i) == tf_nullparam ) {
		continue;
	    }
	    if( tf_typep(i) == tf_string ) {
		continue;
	    } 
	    object = acc_handle_tfarg(i);
	    if( !object ) {
		tf_error( "cannot find object" );
		tf_dofinish();
		goto DONE;
	    }
	    lxt2_add( object, lxt.depth );
	    objects++;
	}
	if( objects == 0 ) {
#if DEBUG
	    io_printf( "lxt2_recordvars: defaultpath=%s\n",
			 acc_fetch_fullname(acc_handle_parent(acc_handle_tfinst())), lxt.depth );
#endif
	    lxt2_add( acc_handle_parent(acc_handle_tfinst()), lxt.depth );
	}
	lxt2_dump( lxt.objectList, 1 );
DONE:
	acc_close();
	return 0;
}	

/*************************************************
   lxt2_recordon
   - enable recording
 ************************************************/
int lxt2_recordon( int data, int reason )
{
    acc_initialize();

    switch( reason ) {
    case reason_checktf:
	if( tf_nump() != 0 ) {
	    tf_error( "too many arguments to recordon" );
	    tf_dofinish();
	}
	goto DONE;
    case reason_calltf:
	if( !lxt.inited ) {
	    tf_error( "recording has not started" );
	    tf_dofinish();
	    goto DONE;
	}
	break;
    default:
	goto DONE;
    }
    lxt2_enable( 1 );
DONE:
    acc_close();
    return 0;
}

/*************************************************
   lxt2_recordoff
   - disable recording
 ************************************************/
int lxt2_recordoff( int data, int reason )
{
    acc_initialize();

    switch( reason ) {
    case reason_checktf:
	if( tf_nump() != 0 ) {
	    tf_error( "too many arguments to recordoff" );
	    tf_dofinish();
	}
	goto DONE;
    case reason_calltf:
	if( !lxt.inited ) {
	    tf_error( "recording has not started" );
	    tf_dofinish();
	    goto DONE;
	}
	break;
    default:
	goto DONE;
    }
    lxt2_enable( 0 );
DONE:
    acc_close();
    return 0;
}

/*************************************************
   lxt2_recordclose
   - close lxt file and stop recording
 ************************************************/
int lxt2_recordclose( int data, int reason )
{
    acc_initialize();

    switch( reason ) {
    case reason_checktf:
	if( tf_nump() != 0 ) {
	    tf_error( "too many arguments to recordclose" );
	    tf_dofinish();
	}
	goto DONE;
    case reason_calltf:
	if( !lxt.inited ) {
	    tf_error( "recording has not started" );
	    tf_dofinish();
	    goto DONE;
	}
	break;
    default:
	goto DONE;
    }
    lxt2_close();
DONE:
    acc_close();
    return 0;
}

/*************************************************
   lxt2_recordfile
   - specify lxt file and options
 ************************************************/
int lxt2_recordfile( int data, int reason )
{
    int i;
    acc_initialize();


    switch( reason ) {
    case reason_checktf:
	if( tf_nump() == 0 ) {
	    tf_error( "not enough arguments to recordfile" );
	    tf_dofinish();
	}
	goto DONE;
    case reason_calltf:
	if( lxt.inited ) {
	    tf_error( "recording has already started" );
	    tf_dofinish();
	    goto DONE;
	}
	break;
    default:
	goto DONE;
    }
    lxt.filename = strdup( acc_fetch_tfarg_str(1) );
    for( i = 2; i <= tf_nump(); ++i ) {
	lxt2_option( acc_fetch_tfarg_str(i) );
    }
DONE:
    acc_close();
    return 0;
}

/*************************************************
   lxt2_recordsetup
   - specify options
 ************************************************/
int lxt2_recordsetup( int data, int reason )
{
    int i;
    acc_initialize();

    switch( reason ) {
    case reason_checktf:
	if( tf_nump() == 0 ) {
	    tf_error( "not enough arguments to recordsetup" );
	    tf_dofinish();
	}
	goto DONE;
    case reason_calltf:
	if( lxt.inited ) {
	    tf_error( "recording has already started" );
	    tf_dofinish();
	}
	break;
    default:
	goto DONE;
    }
    for( i = 1; i <= tf_nump(); ++i ) {
	lxt2_option( acc_fetch_tfarg_str(i) );
    }
DONE:
    acc_close();
    return 0;
}
/*************************************************
  lxt2_close
    - close lxt file
 ************************************************/
static void lxt2_close()
{
    lxt2_timemarker();
#if DEBUG
    io_printf( "lxt2_close: %p\n", lxt.t );
#endif
    lxt2_wr_close( lxt.t );
    lxt.inited = 0;
    if( lxt.filename ) {
	free( lxt.filename );
	lxt.filename = 0;
    }
    if( lxt.design ) {
        free( lxt.design );
        lxt.design = 0;
    }
    while( lxt.objectList ) {
	info_p next;
	next = lxt.objectList->next;
	free( lxt.objectList->name );
	free( lxt.objectList );
	lxt.objectList = next;
    }
}
/*************************************************
  lxt2_enable
    - turn on/off variable change collection
 ************************************************/
static void lxt2_enable( int on )
{
    if( on == lxt.enabled ) {
	return;
    }
    lxt.enabled = on;
    if( on ) {
	info_p info;
	lxt2_timemarker();
	lxt2_wr_set_dumpon( lxt.t );
	for( info = lxt.objectList; info; info = info->next ) {
	    lxt2_dump( info, 1 );
	}
    } else {
	info_p info;
	lxt2_timemarker();
	lxt2_wr_set_dumpoff( lxt.t );
	for( info = lxt.objectList; info; info = info->next ) {
      	    if( !info->real ) {
    	    	lxt2_wr_emit_value_bit_string( lxt.t, info->symbol, 0, "x" ); 
	    }
	}
    }
}
/*************************************************
  lxt2_option
    - set a option
 ************************************************/
static void lxt2_option( char* str )
{
    char* eq = index( str, '=' );
    int   len = strlen( str );
    if( eq ) {
	len -= strlen( eq );
    } else if( !strncmp( str, "incsize", len ) ) {
	lxt.incSize = atoi( eq+1 );
    } else if( !strncmp( str, "speed", len ) ) {
	lxt.compress = 0;
    } else if( !strncmp( str, "space", len ) ) {
	lxt.compress = 1;
    } else if( !strncmp( str, "sequence", len ) ) {
	lxt.sequence = 1;
    } else if( !strncmp( str, "nosequence", len ) ) {
	lxt.sequence = 0;
    } else if( !strncmp( str, "design", len ) ) {
	lxt.design = strdup( eq+1 );
    } else if( !strncmp( str, "depth", len ) ) {
	lxt.depth = atoi( eq+1 );
    } else {
	tf_error( "option %s not supported", str );
        tf_dofinish();
    }
}
/*************************************************
  lxt2_add
    - add object to file
 ************************************************/
static void lxt2_add( handle object, int depth )
{
    int	   real  = 0;
    int	   event = 0;
    int	   flags;
    int    msb;
    int	   lsb;
    info_p info;
    handle block;
    handle term;
    static int filter[] = {
	accIntegerVar,
	accNamedEvent,
	accNet,
	accRealVar,
	accRegister,
	accTimeVar,
	0
    };

    switch( acc_fetch_type(object) ) {
    case accNamedEvent:
	flags = LXT2_WR_SYM_F_BITS;
	event = 1;
	break;
    case accRealVar:
	flags = LXT2_WR_SYM_F_DOUBLE;
        real = 1;
	break;
    case accIntegerVar:
    case accNet:
    case accPort:
    case accReg:
    case accTimeVar:
    case accParameter:
	flags = LXT2_WR_SYM_F_BITS;
	break;
    case accStatement:
    case accTask:
    case accModule:
	term = null;
	while(1) {
	    term = acc_next( filter, object, term );
	    if( term == null ) {
		break;
	    }
	    lxt2_add( term, depth );
	} 
	if( depth == 1 ) {
	    return;
	}
	block = null;
	while(1) {
	    block = acc_next_child( object, block );
	    if( block == null ) {
		break;
	    }
	    lxt2_add( block, (depth==0) ? 0 : depth-1 );
	} 
	return;
    default:
	return;
    }	
    info = (info_p)malloc( sizeof(info_t) );
    if( !info ) {
	tf_error( "cannot allocate memory" );
        tf_dofinish();
	return;
    }
    info->object     = object;
    info->name       = strdup( acc_fetch_fullname(object) );
    info->next       = lxt.objectList;
    lxt.objectList   = info;
    info->sequence   = lxt.sequence;
    info->event      = event;
    info->real	     = real;
    info->updateNext = 0;

    if( real ) {
    	info->symbol = lxt2_wr_symbol_add( lxt.t, info->name, 0, 0, 0, flags );
    } else if( event ) {
    	info->symbol = lxt2_wr_symbol_add( lxt.t, info->name, 0, 0, 0, flags );
    } else {
    	acc_fetch_range( object, &msb, &lsb );
#if DEBUG
	io_printf( "lxt2_add: %s [ %d : %d ]\n", 
		info->name, msb, lsb );
#endif
    	info->symbol = lxt2_wr_symbol_add( lxt.t, info->name, 0, msb, lsb, flags );
    }
    acc_vcl_add( object, lxt2_changed, (char*)info, vcl_verilog_logic );
#if DEBUG
    io_printf( "lxt2_recordvars: adding %p %s\n", info->symbol, info->name );
#endif
}
/*************************************************
  lxt2_nexttimemarker
    - record next time
 ************************************************/
static void lxt2_nexttimemarker()
{
    int       high;
    int       low;
    lxttime_t time;

    tf_getnextlongtime( &low, &high );
    time = (((lxttime_t)(unsigned)high) << 32) + (unsigned)low;
    lxt2_wr_set_time64( lxt.t, time );  
#if DEBUG
    io_printf( "lxt2_nexttimemarker: %u %d\n", (unsigned)time, ftell(lxt.t->handle) );
#endif
}
/*************************************************
  lxt2_timemarker
    - record current time
 ************************************************/
static void lxt2_timemarker()
{
    int       high;
    lxttime_t time;

    time = (unsigned)tf_igetlongtime( &high, ginstance );
    time += ((lxttime_t)(unsigned)high) << 32;
    lxt2_wr_set_time64( lxt.t, time );  
}
/*************************************************
  lxt2_timemarkerp1
    - record current time plus 1 tick
 ************************************************/
static void lxt2_timemarkerp1()
{
    int       high;
    lxttime_t time;

    time = (unsigned)tf_igetlongtime( &high, ginstance );
    time += ((lxttime_t)(unsigned)high) << 32;
    lxt2_wr_set_time64( lxt.t, time+1 );  
}
/*************************************************
  lxt2_dump
    - dump symbol value
 ************************************************/
static void lxt2_dump( info_p info, int init )
{
    if( info->real ) {
	s_acc_value value;
	value.format = accRealVal;
	acc_fetch_value( info->object, "%%", &value );
    	lxt2_wr_emit_value_double( lxt.t, info->symbol, 0, value.value.real ); 
    } else if( info->event ) {
	if( init ) {
    	    lxt2_wr_emit_value_bit_string( lxt.t, info->symbol, 0, "0" ); 
        } else {
    	    lxt2_wr_emit_value_bit_string( lxt.t, info->symbol, 0, "1" ); 
	    info->updateNext = lxt.eventList;
	    lxt.eventList = info;
	}
    } else {
    	lxt2_wr_emit_value_bit_string( lxt.t, info->symbol, 0, 
			acc_fetch_value( info->object, "%b", null ) );
#if DEBUG
	io_printf( "lxt2_dump: %s %s\n", info->name, acc_fetch_value( info->object,
			"%b", null ) );
#endif
    }
}
/*************************************************
  lxt2_changed
    - record variable change
 ************************************************/
static int lxt2_changed( p_vc_record vc_record )
{
    info_p info = (info_p)vc_record->user_data;
    if( !lxt.enabled ) {
	return 0;
    }
    lxt2_timemarker();
    if( info->sequence || info->event ) {
	lxt2_dump( info, 0 );
    } else {
	if( !info->updateNext ) {
	    info->updateNext = lxt.updateList;
	    lxt.updateList = info;
	}
    }
    tf_irosynchronize(ginstance);
    return 0;
}
/*************************************************
  lxt2_init
    - Open lxt file and enable collection
 ************************************************/
static void lxt2_init()
{
    char*  filename;
    if( lxt.inited ) {
	tf_error( "recording has alreay begun" );
        tf_dofinish();
	return;
    }
    if( lxt.filename ) {
	filename = lxt.filename;
    } else if( lxt.design ) {
	filename = (char*)malloc( strlen(lxt.design)+4+1 );
	if( !filename ) {
	    tf_error( "could not allocate memory" );
            tf_dofinish();
	    return;
	}
	sprintf( filename, "%s.lxt", lxt.design );
	lxt.filename = filename;
    } else {
	char* top = acc_fetch_name( acc_next_topmod(null) );
	filename = (char*)malloc( strlen(top)+3+1 );
	sprintf( filename, "%s.lxt", top );	
	lxt.filename = filename;
    }
    lxt.t = lxt2_wr_init( filename );
#if DEBUG
    io_printf( "lxt2_init: %p\n", lxt.t );
#endif
    if( !lxt.t ) {
	tf_error( "could not create file '%s'", filename );
        tf_dofinish();
	return;
    }

    lxt2_wr_set_timescale( lxt.t, acc_fetch_precision() );
    if( lxt.compress ) {
	lxt2_wr_set_compression_depth(lxt.t, 9);
	lxt2_wr_set_partial_off(lxt.t);
    } else {
	lxt2_wr_set_compression_depth(lxt.t, 4);
	lxt2_wr_set_partial_on(lxt.t, 1);
    }
    lxt2_wr_set_break_size(lxt.t, lxt.incSize);

    lxt.inited     = 1;
    lxt.enabled    = 1;
    lxt.updateList = 0;
    lxt.eventList  = 0;
    lxt.hunk	   = 0;

    lxt2_wr_set_initial_value( lxt.t, 'x' );
    lxt2_wr_symbol_bracket_stripping( lxt.t, 1 );
    lxt2_timemarker(); 
}





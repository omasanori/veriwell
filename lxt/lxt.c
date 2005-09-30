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
#include <lxt_write.h>

#define DEBUG 0

/*****************************************************
 *
 *	lxt.c	
 * 	- pli interface routines to dump lxt files
 *
 *****************************************************
 */
/*
 * add this to veriuser.c
 * { usertask, 0, lxt_recordvars, 0, lxt_recordvars, lxt_recordvars, "$lxt_recordvars" },
 * { usertask, 0, lxt_recordon, 0, lxt_recordon, lxt_recordon, "$lxt_recordon" },
 * { usertask, 0, lxt_recordoff, 0, lxt_recordoff, lxt_recordoff, "$lxt_recordoff" },
 * { usertask, 0, lxt_recordclose, 0, lxt_recordclose, lxt_recordclose, "$lxt_recordclose" },
 * { usertask, 0, lxt_recordfile, 0, lxt_recordfile, lxt_recordfile, "$lxt_recordfile" },
 * { usertask, 0, lxt_recordsetup, 0, lxt_recordsetup, lxt_recordsetup, "$lxt_recordsetup" },
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
    struct lt_symbol* symbol;
};
typedef struct info_s info_t, *info_p;


/*
 * global control structure
 */
struct lxt_s {
    char*            filename;
    int	             enabled;
    int	             sequence;
    char*            design;
    int		     compress;
    unsigned         incSize;
    struct lt_trace* t;
    int	             inited;
    int		     depth;
    info_p	     objectList;    
    info_p	     updateList;    
    info_p	     eventList;    
    int		     hunk;
};
typedef struct lxt_s lxt_t, *lxt_p;
lxt_t lxt = { 0, 0, 0, 0, 1, 100*1024*1024, 0, 0, 0, 0, 0, 0, 0 };

static void lxt_close();
static void lxt_enable( int on );
static void lxt_option( char* str );
static void lxt_add( handle object, int depth );
static void lxt_init();
static void lxt_dump( info_p info, int init );
static int  lxt_changed( p_vc_record vc_record );
static void lxt_timemarker();
static void lxt_timemarkerp1();
static void lxt_nexttimemarker();
static void lxt_incinit();
static char* ginstance;

/* ??? mdh - speed up variable dumping */

/*************************************************
   lxt_recordvars
   - add objects to be recorded
 ************************************************/
int lxt_recordvars( int data, int reason )
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
		    lxt_close();
		}
		goto DONE;	
	case reason_rosynch:
		update = (lxt.updateList != NULL);
		while( lxt.updateList ) {
			info_p info;
			info = lxt.updateList;
			lxt_dump( info, 0 );
			lxt.updateList = info->updateNext;
			info->updateNext = 0;
		}
		if( update ) {
		    lxt_timemarkerp1(); 
		}
		while( lxt.eventList ) {
			info_p info;
			info = lxt.eventList;
			lxt_dump( info, 1 );
			lxt.eventList = info->updateNext;
			info->updateNext = 0;
		}
		if( lxt.t->position > lxt.incSize ) {
		    lxt_incinit();
		}
		lxt_nexttimemarker(); 
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
		lxt_option( str );
		continue;
	    } 
	}
	/*
	 * on first call, initialize structure
	 */
	if( !lxt.inited ) {
	    lxt_init();
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
	    lxt_add( object, lxt.depth );
	    objects++;
	}
	if( objects == 0 ) {
#if DEBUG
	    io_printf( "lxt_recordvars: defaultpath=%s\n",
			 acc_fetch_fullname(acc_handle_parent(acc_handle_tfinst())), lxt.depth );
#endif
	    lxt_add( acc_handle_parent(acc_handle_tfinst()), lxt.depth );
	}
DONE:
	acc_close();
	return 0;
}	

/*************************************************
   lxt_recordon
   - enable recording
 ************************************************/
int lxt_recordon( int data, int reason )
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
    lxt_enable( 1 );
DONE:
    acc_close();
    return 0;
}

/*************************************************
   lxt_recordoff
   - disable recording
 ************************************************/
int lxt_recordoff( int data, int reason )
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
    lxt_enable( 0 );
DONE:
    acc_close();
    return 0;
}

/*************************************************
   lxt_recordclose
   - close lxt file and stop recording
 ************************************************/
int lxt_recordclose( int data, int reason )
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
    lxt_close();
DONE:
    acc_close();
    return 0;
}

/*************************************************
   lxt_recordfile
   - specify lxt file and options
 ************************************************/
int lxt_recordfile( int data, int reason )
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
	lxt_option( acc_fetch_tfarg_str(i) );
    }
DONE:
    acc_close();
    return 0;
}

/*************************************************
   lxt_recordsetup
   - specify options
 ************************************************/
int lxt_recordsetup( int data, int reason )
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
	lxt_option( acc_fetch_tfarg_str(i) );
    }
DONE:
    acc_close();
    return 0;
}
/*************************************************
  lxt_close
    - close lxt file
 ************************************************/
static void lxt_close()
{
    lxt_timemarker();
#if DEBUG
    io_printf( "lxt_close: %p\n", lxt.t );
#endif
    lt_close( lxt.t );
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
  lxt_enable
    - turn on/off variable change collection
 ************************************************/
static void lxt_enable( int on )
{
    if( on == lxt.enabled ) {
	return;
    }
    lxt.enabled = on;
    if( on ) {
	info_p info;
	lxt_timemarker();
	for( info = lxt.objectList; info; info = info->next ) {
	    lxt_dump( info, 1 );
	}
    } else {
	info_p info;
	lxt_timemarker();
	for( info = lxt.objectList; info; info = info->next ) {
      	    if( !info->real ) {
    	    	lt_emit_value_bit_string( lxt.t, info->symbol, 0, "-" ); 
	    }
	}
    }
}
/*************************************************
  lxt_option
    - set a option
 ************************************************/
static void lxt_option( char* str )
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
  lxt_add
    - add object to file
 ************************************************/
static void lxt_add( handle object, int depth )
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
	flags = LT_SYM_F_BITS;
	event = 1;
	break;
    case accRealVar:
	flags = LT_SYM_F_DOUBLE;
        real = 1;
	break;
    case accIntegerVar:
    case accNet:
    case accPort:
    case accReg:
    case accTimeVar:
    case accParameter:
	flags = LT_SYM_F_BITS;
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
	    lxt_add( term, depth );
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
	    lxt_add( block, (depth==0) ? 0 : depth-1 );
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
    	info->symbol = lt_symbol_add( lxt.t, info->name, 0, 0, 0, flags );
    } else if( event ) {
    	info->symbol = lt_symbol_add( lxt.t, info->name, 0, 0, 0, flags );
    } else {
    	acc_fetch_range( object, &msb, &lsb );
#if DEBUG
	io_printf( "lxt_add: %s [ %d : %d ]\n", 
		info->name, msb, lsb );
#endif
    	info->symbol = lt_symbol_add( lxt.t, info->name, 0, msb, lsb, flags );
    }
    lxt_dump( info, 1 );
    acc_vcl_add( object, lxt_changed, (char*)info, vcl_verilog_logic );
#if DEBUG
    io_printf( "lxt_recordvars: adding %p %s\n", info->symbol, info->name );
#endif
}
/*************************************************
  lxt_nexttimemarker
    - record next time
 ************************************************/
static void lxt_nexttimemarker()
{
    int       high;
    int       low;
    lxttime_t time;

    tf_getnextlongtime( &low, &high );
    time = (((lxttime_t)(unsigned)high) << 32) + (unsigned)low;
    lt_set_time64( lxt.t, time );  
#if DEBUG
    io_printf( "lxt_nexttimemarker: %u %d\n", (unsigned)time, ftell(lxt.t->handle) );
#endif
}
/*************************************************
  lxt_timemarker
    - record current time
 ************************************************/
static void lxt_timemarker()
{
    int       high;
    lxttime_t time;

    time = (unsigned)tf_igetlongtime( &high, ginstance );
    time += ((lxttime_t)(unsigned)high) << 32;
    lt_set_time64( lxt.t, time );  
}
/*************************************************
  lxt_timemarkerp1
    - record current time plus 1 tick
 ************************************************/
static void lxt_timemarkerp1()
{
    int       high;
    lxttime_t time;

    time = (unsigned)tf_igetlongtime( &high, ginstance );
    time += ((lxttime_t)(unsigned)high) << 32;
    lt_set_time64( lxt.t, time+1 );  
}
/*************************************************
  lxt_dump
    - dump symbol value
 ************************************************/
static void lxt_dump( info_p info, int init )
{
    if( info->real ) {
	s_acc_value value;
	value.format = accRealVal;
	acc_fetch_value( info->object, "%%", &value );
    	lt_emit_value_double( lxt.t, info->symbol, 0, value.value.real ); 
    } else if( info->event ) {
	if( init ) {
    	    lt_emit_value_bit_string( lxt.t, info->symbol, 0, "0" ); 
        } else {
    	    lt_emit_value_bit_string( lxt.t, info->symbol, 0, "1" ); 
	    info->updateNext = lxt.eventList;
	    lxt.eventList = info;
	}
    } else {
    	lt_emit_value_bit_string( lxt.t, info->symbol, 0, 
			acc_fetch_value( info->object, "%b", null ) );
#if DEBUG
	io_printf( "lxt_dump: %s %s\n", info->name, acc_fetch_value( info->object,
			"%b", null ) );
#endif
    }
}
/*************************************************
  lxt_changed
    - record variable change
 ************************************************/
static int lxt_changed( p_vc_record vc_record )
{
    info_p info = (info_p)vc_record->user_data;
    if( !lxt.enabled ) {
	return 0;
    }
    lxt_timemarker();
    if( info->sequence || info->event ) {
	lxt_dump( info, 0 );
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
  lxt_init
    - Open lxt file and enable collection
 ************************************************/
static void lxt_init()
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
    lxt.t = lt_init( filename );
#if DEBUG
    io_printf( "lxt_init: %p\n", lxt.t );
#endif
    if( !lxt.t ) {
	tf_error( "could not create file '%s'", filename );
        tf_dofinish();
	return;
    }
    lt_set_clock_compress( lxt.t );
    if( lxt.compress ) {
	lt_set_chg_compress( lxt.t ); 
    }
    lxt.inited     = 1;
    lxt.enabled    = 1;
    lxt.updateList = 0;
    lxt.eventList  = 0;
    lxt.hunk	   = 0;

    lt_set_initial_value( lxt.t, 'x' );
    lt_symbol_bracket_stripping( lxt.t, 1 );
    lt_set_timescale( lxt.t, acc_fetch_precision() );
    lxt_timemarker(); 
}
/*************************************************
  lxt_incinit
    - close current file and open a new one
      maintaining the same trace info
 ************************************************/
static void lxt_incinit()
{
    char* filename;
    char* dot;
    info_p info;
#if DEBUG
    io_printf( "lxt_incinit: %p\n", lxt.t );
#endif
    /*
     * pinch off old file
     */
    lxt_timemarker();
    lt_close( lxt.t );

    /*
     * create new filename
     */
    lxt.hunk++;
    filename = (char*)malloc( strlen(lxt.filename)+10+1 );
    dot = rindex( lxt.filename, '.' );
    *dot = 0;
    if( lxt.hunk > 1 ) {
	dot = rindex( lxt.filename, '-' );
	*dot = 0;
    }
    sprintf( filename, "%s-%d.lxt", lxt.filename, lxt.hunk );
    free( lxt.filename );
    lxt.filename = filename;

    /*
     * open new wave file
     */
    lxt.t = lt_init( filename );
    if( !lxt.t ) {
	tf_error( "could not create file '%s'", filename );
        tf_dofinish();
	return;
    }
    lt_set_clock_compress( lxt.t );
    lt_set_chg_compress( lxt.t ); 
    lxt.updateList = 0;
    lxt.eventList  = 0;
    lt_set_initial_value( lxt.t, 'x' );
    lt_symbol_bracket_stripping( lxt.t, 1 );
    lt_set_timescale( lxt.t, acc_fetch_precision() );
    lxt_timemarker(); 

    /*
     * add existing objects to new file
     */
    info = lxt.objectList;
    while( info ) {
	int flags;
	int msb;
	int lsb;
	if( info->real ) {
	    flags = LT_SYM_F_DOUBLE;
	} else {
	    flags = LT_SYM_F_BITS;
	}
	if( info->real ) {
	   info->symbol = lt_symbol_add( lxt.t, info->name, 0, 0, 0, flags );
	} else if( info->event ) {
	   	info->symbol = lt_symbol_add( lxt.t, info->name, 0, 0, 0, flags );
	} else {
	    acc_fetch_range( info->object, &msb, &lsb );
	    info->symbol = lt_symbol_add( lxt.t, info->name, 0, msb, lsb, flags );
	}
	lxt_dump( info, 1 );
	info = info->next;
    }
}





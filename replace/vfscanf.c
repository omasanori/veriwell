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

/**************************************************************
	vfscanf replacment functions
 **************************************************************/

#include <stdio.h>
#include <stdarg.h>

int vfscanf(FILE *stream, const char *format, va_list ap)
{

    if( !strcmp( format, "%s" ) ) {
	char* p = va_arg(ap,char*);
	fscanf( stream, format, p );	
    } else if( !strcmp( format, "%lx" ) ) {
	long* p = va_arg(ap,long*);
	fscanf( stream, format, p );	
    } else {
    	fprintf( stderr, "FIXME: format '%s' is currently unsupported in replacement vfscanf\n", format );
    	abort();
    }
}

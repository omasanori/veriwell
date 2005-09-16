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
	readline replacment functions
 **************************************************************/

#include <stdio.h>
#include <string.h>

char *readline( char* prompt )
{
    char  buffer[256];
    char* result; 
    if( prompt ) {
    	printf( "%s ", prompt );
    }
    result = fgets( buffer, sizeof(buffer), stdin );
    if( result ) {
	result = strdup( result );
    }
    return result;
}

/*****************************************************************************
 * Copyright 1994-2008, Elliot Mednick and Mark Hummel
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

/* file.cc - file abstraction */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include "file.h"
#include "vtypes.h"


namespace veriwell {
File* File::stdin_ = new File(stdin);

File::File(FILE* f) :
    f(f), 
    isBuffer(0), 
    buffer(0), 
    len(0), 
    offset(0), 
    own(0) 
{
} 

File::File(char *buffer, int len, int own) :
    f(0), 
    isBuffer(1),
    buffer(buffer), 
    len(len), 
    offset(0),
    own(own) 
{
}

File *File::fopen(const char *name, char *mode) 
{
    FILE* fin = ::fopen(name, mode);
    if( !fin ) {
    	return NULL;
    }
    return new File(fin);
}

File* File::Stdin() 
{ 
    return stdin_; 
}

void File::fclose() 
{
    if (!isBuffer) {
        ::fclose(f);
    } else if (own) {
        free(buffer);
    }
    delete this;
}

int File::isatty() 
{
    return !isBuffer && ::isatty(fileno(f));
}

File* File::fopenbuf(char *buffer, int len) 
{
    return new File(buffer, len);
}

int File::fgetc() 
{
    int result = EOF;
    if (isBuffer) {
        if (offset < len) {
    	result = buffer[offset++];
        }
    } else {
        result = getc(f);
    }
    return result;
}

int File::fscanf(char *format, ...) 
{
    int result;
    va_list args;
    va_start(args, format);
    if (isBuffer) {
        /* fscanf not support on in memory buffers */
        ASSERT(FALSE);;
        result = 0;
    } else {
        result = vfscanf(f, format, args);
    }
    va_end(args);
    return result;
}
int File::fungetc(int c) {
    if (isBuffer) {
        if (offset != 0 && c != EOF) {
    	buffer[--offset] = c;
    	return c;
        } else {
    	return EOF;
        }
    } else {
        return ungetc(c, f);
    }
}
char *File::fgets(char *s, int size) {
    if (isBuffer) {
        int i;
        for (i = 0; i < size - 1; ++i) {
    	     s[i] = fgetc();
    	     if (s[i] == EOF) {
    	         if (i == 0) {
    	     	return NULL;
    	         } else {
    	     	break;
    	         }
    	     } else if (s[i] == '\n') {
                 i++;
    	         break;
             }
        }
        s[i] = 0;
        return s;
    } else {
        return::fgets(s, size, f);
    }
}

}


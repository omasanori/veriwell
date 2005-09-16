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

/******************************************************************************
 Glue.hpp

                This file contains the headers for glue routines to interface the shell
                to the back end.
        
        
 ******************************************************************************/

#ifndef GLUE_H
#define GLUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <signal.h>
#include "vtypes.h"

#ifndef HAVE_VFSCANF
extern "C" int vfscanf( FILE*, char*, ... );
#endif

BEGIN_C_DECLS

    typedef void (*sig_handler) (int);

    FILE *shell_fopen(char *name, char *mode);
    void shell_abort(void);
    void shell_exit(int status);
    void shell_process(void);
    sig_handler shell_signal(int sig, sig_handler func);
    void shell_assert(char *file, int line);
    void shell_fclose(FILE * file);
    int shell_getchar(void);
    int shell_printf(const char *format, ...);
    int shell_vprintf(const char *format, va_list argptr);
    int shell_vfprintf(FILE * fp, const char *format, va_list argptr);
    void shell_ungetc(int c, FILE * f);
    void GetProgramPath(char *string);
    void SetProgramPath();
    FILE *shell_fopenbuf(char *buffer, int len);
    void shell_fflush(FILE * f);



END_C_DECLS

void error(char *s, char *v, char *v2);
void printf_V_key(char *, ...);
char *xmalloc(unsigned);
/*
 * file class
 */
class File {
    FILE *f;
    int isBuffer;
    int len;
    char *buffer;
    int offset;
    int own;
  public:
    static File *stdin_;
     File(FILE * f):f(f), isBuffer(0), buffer(0), len(0), offset(0), own(0) {
  } File(char *buffer, int len, int own = 0):f(0), isBuffer(1),
	buffer(buffer), len(len), offset(0),
	own(own) {
    }
    static File *fopen(char *name, char *mode) {
	FILE* fin = shell_fopen(name, mode);
	if( !fin ) {
		return NULL;
	}
	return new File(fin);
    }
    static File* Stdin() { return stdin_; }
    void fclose() {
	if (!isBuffer) {
	    shell_fclose(f);
	} else if (own) {
	    free(buffer);
	}
	delete this;
    }
    int isatty() {
	return !isBuffer &&::isatty(fileno(f));
    }
    static File *fopenbuf(char *buffer, int len) {
	return new File(buffer, len);
    }
    int fgetc() {
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
    int fscanf(char *format, ...) {
	int result;
	va_list args;
	va_start(args, format);
	if (isBuffer) {
	    /* fscanf not support on in memory buffers */
	    error("Internal error", NULL, NULL);
	    result = 0;
	} else {
	    result = vfscanf(f, format, args);
	}
	va_end(args);
	return result;
    }
    int fungetc(int c) {
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
    char *fgets(char *s, int size) {
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
};

#endif				// GLUE_H

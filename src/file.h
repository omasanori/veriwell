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

/* file.h - file abstraction */

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdarg.h>


namespace veriwell {
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
    static File *fopen(const char *name, char *mode);
    static File* Stdin();
    static File* fopenbuf(char *buffer, int len);
    File(FILE * f);
    File(char *buffer, int len, int own = 0);
    void  fclose();
    int   isatty();
    int   fgetc();
    int   fscanf(char *format, ...);
    int   fungetc(int c);
    char* fgets(char *s, int size);
};

} // namespace veriwell

#endif				//FILE_H



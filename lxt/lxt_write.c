/*
 * Copyright (c) 2001 Tony Bybell.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "lxt_write.h"

/*
 * functions which emit various big endian
 * data to a file
 */ 
static int lt_emit_u8(struct lt_trace *lt, int value)
{
unsigned char buf[1];
int nmemb;

buf[0] = value & 0xff;
nmemb=fwrite(buf, sizeof(char), 1, lt->handle);
lt->position+=nmemb;
return(nmemb);
}


static int lt_emit_u16(struct lt_trace *lt, int value)
{
unsigned char buf[2];
int nmemb;

buf[0] = (value>>8) & 0xff;
buf[1] = value & 0xff;
nmemb = fwrite(buf, sizeof(char), 2, lt->handle);
lt->position+=nmemb;
return(nmemb);
}


static int lt_emit_u24(struct lt_trace *lt, int value)
{
unsigned char buf[3];
int nmemb;

buf[0] = (value>>16) & 0xff;
buf[1] = (value>>8) & 0xff;
buf[2] = value & 0xff;
nmemb=fwrite(buf, sizeof(char), 3, lt->handle);
lt->position+=nmemb;
return(nmemb);
}


static int lt_emit_u32(struct lt_trace *lt, int value)
{
unsigned char buf[4];
int nmemb;

buf[0] = (value>>24) & 0xff;
buf[1] = (value>>16) & 0xff;
buf[2] = (value>>8) & 0xff;
buf[3] = value & 0xff;
nmemb=fwrite(buf, sizeof(char), 4, lt->handle);
lt->position+=nmemb;
return(nmemb);
}


static int lt_emit_u64(struct lt_trace *lt, int valueh, int valuel)
{
int rc;

if((rc=lt_emit_u32(lt, valueh)))
	{
	rc=lt_emit_u32(lt, valuel);
	}

return(rc);
}


static int lt_emit_double(struct lt_trace *lt, double value)
{
int nmemb;

nmemb=fwrite(&value, sizeof(char), sizeof(double)/sizeof(char), lt->handle);
lt->position+=nmemb;
return(nmemb);
}


static int lt_emit_string(struct lt_trace *lt, char *value)
{
int rc=1;
do  
	{
        rc&=lt_emit_u8(lt, *value);
        } while(*(value++));
return(rc);
}


/*
 * gzfunctions which emit various big endian
 * data to a file.  (lt->position needs to be
 * fixed up on gzclose so the tables don't
 * get out of sync!)
 */ 
static int lt_emit_u8z(struct lt_trace *lt, int value)
{
unsigned char buf[1];
int nmemb;

buf[0] = value & 0xff;
nmemb=gzwrite(lt->zhandle, buf, 1);
lt->zpackcount++;
lt->position++;
return(nmemb);
}


static int lt_emit_u16z(struct lt_trace *lt, int value)
{
unsigned char buf[2];
int nmemb;

buf[0] = (value>>8) & 0xff;
buf[1] = value & 0xff;
nmemb = gzwrite(lt->zhandle, buf, 2);
lt->zpackcount+=2;
lt->position+=2;
return(nmemb);
}


static int lt_emit_u24z(struct lt_trace *lt, int value)
{
unsigned char buf[3];
int nmemb;

buf[0] = (value>>16) & 0xff;
buf[1] = (value>>8) & 0xff;
buf[2] = value & 0xff;
nmemb=gzwrite(lt->zhandle, buf, 3);
lt->zpackcount+=3;
lt->position+=3;
return(nmemb);
}


static int lt_emit_u32z(struct lt_trace *lt, int value)
{
unsigned char buf[4];
int nmemb;

buf[0] = (value>>24) & 0xff;
buf[1] = (value>>16) & 0xff;
buf[2] = (value>>8) & 0xff;
buf[3] = value & 0xff;
nmemb=gzwrite(lt->zhandle, buf, 4);

lt->zpackcount+=4;
lt->position+=4;
return(nmemb);
}


static int lt_emit_u64z(struct lt_trace *lt, int valueh, int valuel)
{
int rc;

if((rc=lt_emit_u32z(lt, valueh)))
	{
	rc=lt_emit_u32z(lt, valuel);
	}

return(rc);
}


static int lt_emit_doublez(struct lt_trace *lt, double value)
{
int nmemb;

nmemb=gzwrite(lt->zhandle, &value, sizeof(double)/sizeof(char));
lt->zpackcount+=(sizeof(double)/sizeof(char));
lt->position+=(sizeof(double)/sizeof(char));;
return(nmemb);
}


static int lt_emit_stringz(struct lt_trace *lt, char *value)
{
int rc=1;
do  
	{
        rc&=lt_emit_u8z(lt, *value);
        } while(*(value++));
return(rc);
}


/*
 * switch between compression modes above
 */
static void lt_set_zmode(struct lt_trace *lt, int mode)
{
if(!mode)
	{
	lt->lt_emit_u8 = lt_emit_u8;
	lt->lt_emit_u16 = lt_emit_u16;
	lt->lt_emit_u24 = lt_emit_u24;
	lt->lt_emit_u32 = lt_emit_u32;
	lt->lt_emit_u64 = lt_emit_u64;
	lt->lt_emit_double = lt_emit_double;
	lt->lt_emit_string = lt_emit_string;
	}
	else
	{
	lt->lt_emit_u8 = lt_emit_u8z;
	lt->lt_emit_u16 = lt_emit_u16z;
	lt->lt_emit_u24 = lt_emit_u24z;
	lt->lt_emit_u32 = lt_emit_u32z;
	lt->lt_emit_u64 = lt_emit_u64z;
	lt->lt_emit_double = lt_emit_doublez;
	lt->lt_emit_string = lt_emit_stringz;
	}
}


/*
 * hash/symtable manipulation
 */
static int lt_hash(const char *s)
{
const char *p;
char ch;
unsigned int h=0, h2=0, pos=0, g;
for(p=s;*p;p++)
        {
	ch=*p;
	h2<<=3;
	h2-=((unsigned int)ch+(pos++));		/* this handles stranded vectors quite well.. */

        h=(h<<4)+ch;
        if((g=h&0xf0000000))
                {
                h=h^(g>>24);
                h=h^g;
                }   
        }

h^=h2;						/* combine the two hashes */
return(h%LT_SYMPRIME);
}


static struct lt_symbol *lt_symadd(struct lt_trace *lt, const char *name, int hv)
{
struct lt_symbol *s;

s=(struct lt_symbol *)calloc(1,sizeof(struct lt_symbol));
strcpy(s->name=(char *)malloc((s->namlen=strlen(name))+1),name);
s->next=lt->sym[hv];
lt->sym[hv]=s;
return(s);
}


static struct lt_symbol *lt_symfind(struct lt_trace *lt, const char *s)
{
int hv;
struct lt_symbol *temp;

hv=lt_hash(s);
if(!(temp=lt->sym[hv])) return(NULL); /* no hash entry, add here wanted to add */
	
while(temp)
        {
        if(!strcmp(temp->name,s))
                {
                return(temp); /* in table already */    
                }
        if(!temp->next) break;
        temp=temp->next;
        }
	
return(NULL); /* not found, add here if you want to add*/
}


/*
 * compress facs to a prefix count + string + 0x00
 */
static void lt_compress_fac(struct lt_trace *lt, char *str)
{
int i;
int len = strlen(str);
int minlen = (len<lt->compress_fac_len) ? len : lt->compress_fac_len;

if(minlen>65535) minlen=65535;    /* keep in printable range--most hierarchies won't be this big anyway */

if(lt->compress_fac_str)
        {
        for(i=0;i<minlen;i++)
                {
                if(lt->compress_fac_str[i]!=str[i]) break;
                }
	lt_emit_u16z(lt, i);
	lt_emit_stringz(lt, str+i);
        free(lt->compress_fac_str);
        } 
        else
        {
	lt_emit_u16z(lt, 0);
	lt_emit_stringz(lt, str);
        }
         
lt->compress_fac_str = (char *) malloc((lt->compress_fac_len=len)+1);
strcpy(lt->compress_fac_str, str);
}


/*
 * emit facs in sorted order along with geometry
 * and sync table info
 */
static int lt_compare(const void *v1, const void *v2)
{
struct lt_symbol *s1 = *(struct lt_symbol **)v1;
struct lt_symbol *s2 = *(struct lt_symbol **)v2;
int rc = strcmp(s1->name, s2->name);
if(rc)
	{
	return(rc);
	}
	else
	{
	return(s1->msb - s2->msb);
	}
}


static void strip_brack(struct lt_symbol *s)
{
char *lastch = s->name+s->namlen - 1;
if(*lastch!=']') return;
if(s->namlen<3) return;
lastch--;
while(lastch!=s->name)
	{
	if(*lastch=='[')
		{
		*lastch=0x00;	
		return;
		}
	lastch--;	
	}
return;
}


static void lt_emitfacs(struct lt_trace *lt)
{
int i;

if((lt)&&(lt->numfacs))
	{
	struct lt_symbol *s = lt->symchain;
	if((lt->sorted_facs = (struct lt_symbol **)calloc(lt->numfacs, sizeof(struct lt_symbol *))))
		{
		if(lt->do_strip_brackets)
		for(i=0;i<lt->numfacs;i++)
			{
			lt->sorted_facs[i] = s;
			strip_brack(s);
			s=s->symchain;
			}
		else	
		for(i=0;i<lt->numfacs;i++)
			{
			lt->sorted_facs[i] = s;
			s=s->symchain;
			}	
		qsort((void *)lt->sorted_facs, lt->numfacs, sizeof(struct lt_symbol *), lt_compare);
		lt->facname_offset=lt->position;

		lt_emit_u32(lt, lt->numfacs);		/* uncompressed */
		lt_emit_u32(lt, lt->numfacbytes);	/* uncompressed */
		fflush(lt->handle);
		lt->zfacname_size = lt->position;
		lt->zhandle = gzdopen(dup(fileno(lt->handle)), "wb9");

		lt->zpackcount = 0;
		for(i=0;i<lt->numfacs;i++)
			{
			lt->sorted_facs[i]->facnum = i;
		 	lt_compress_fac(lt, lt->sorted_facs[i]->name);
			}
		free(lt->compress_fac_str); lt->compress_fac_str=NULL;
		lt->compress_fac_len=0;
		lt->zfacname_predec_size = lt->zpackcount;
	
		gzclose(lt->zhandle);
		fseek(lt->handle, 0L, SEEK_END);
		lt->position=ftell(lt->handle);
		lt->zfacname_size = lt->position - lt->zfacname_size;

		lt->zhandle = gzdopen(dup(fileno(lt->handle)), "wb9");

		lt->facgeometry_offset = lt->position;
		for(i=0;i<lt->numfacs;i++)
			{
			if((lt->sorted_facs[i]->flags&LT_SYM_F_ALIAS)==0)
				{
				lt_emit_u32z(lt, lt->sorted_facs[i]->rows);
				lt_emit_u32z(lt, lt->sorted_facs[i]->msb);
				lt_emit_u32z(lt, lt->sorted_facs[i]->lsb);
				lt_emit_u32z(lt, lt->sorted_facs[i]->flags);
				}
				else
				{
				lt_emit_u32z(lt, lt->sorted_facs[i]->aliased_to->facnum);
				lt_emit_u32z(lt, lt->sorted_facs[i]->msb);
				lt_emit_u32z(lt, lt->sorted_facs[i]->lsb);
				lt_emit_u32z(lt, LT_SYM_F_ALIAS);
				}
			}

		gzclose(lt->zhandle);
		fseek(lt->handle, 0L, SEEK_END);
		lt->position=ftell(lt->handle);
		lt->zfacgeometry_size = lt->position - lt->facgeometry_offset;
		lt->zhandle = gzdopen(dup(fileno(lt->handle)), "wb9");

		lt->sync_table_offset = lt->position;
		for(i=0;i<lt->numfacs;i++)
			{
			lt_emit_u32z(lt, lt->sorted_facs[i]->last_change);
			}

		gzclose(lt->zhandle); lt->zhandle = NULL;
		fseek(lt->handle, 0L, SEEK_END);
		lt->position=ftell(lt->handle);
		lt->zsync_table_size = lt->position - lt->sync_table_offset;
		}
	}
}


/* 
 * initialize the trace and get back and lt context
 */
struct lt_trace *lt_init(const char *name)
{
struct lt_trace *lt=(struct lt_trace *)calloc(1, sizeof(struct lt_trace));

if(!(lt->handle=fopen(name, "wb")))
	{
	free(lt);
	lt=NULL;
	}
	else
	{
	lt_emit_u16(lt, LT_HDRID);
	lt_emit_u16(lt, LT_VERSION);
	lt->change_field_offset = lt->position;
	lt->initial_value = -1;				/* if a user sets this it will have a different POSITIVE val */
	lt->timescale = -256;				/* will be in range of -128<=x<=127 if set */

	lt_set_zmode(lt, 0);

	lt->mintime=ULLDescriptor(1);
	lt->maxtime=ULLDescriptor(0);
	}

return(lt);
}

/*
 * clock flushing..
 */
static void lt_flushclock(struct lt_trace *lt, struct lt_symbol *s)
{
unsigned int last_change_delta = lt->position - s->last_change - 2;
unsigned int start_position = lt->position;
int tag;
int numbytes, numbytes_trans;
int numtrans = s->clk_numtrans - LT_CLKPACK - 1;

if(numtrans<0)
	{
	/* it never got around to caching */
	fprintf(stderr, "Possible Problem with %s with %d?\n", s->name, s->clk_numtrans);
	return;
	}

if(last_change_delta >= 256*65536)
	{
	numbytes = 3;
	}
else
if(last_change_delta >= 65536)
	{
	numbytes = 2;
	}
else
if(last_change_delta >= 256)
	{
	numbytes = 1;
	}
else
	{
	numbytes = 0;
	}

if(numtrans >= 256*65536)
	{
	numbytes_trans = 3;
	}
else
if(numtrans >= 65536)
	{
	numbytes_trans = 2;
	}
else
if(numtrans >= 256)
	{
	numbytes_trans = 1;
	}
else
	{
	numbytes_trans = 0;
	}

tag = (numbytes<<4) + 0xC + numbytes_trans;	/* yields xC..xF */

lt->lt_emit_u8(lt, tag);
switch(numbytes&3)
	{
	case 0: lt->lt_emit_u8(lt, last_change_delta); break;
	case 1: lt->lt_emit_u16(lt, last_change_delta); break;
	case 2: lt->lt_emit_u24(lt, last_change_delta); break;
	case 3: lt->lt_emit_u32(lt, last_change_delta); break;
	}

s->last_change = start_position;

/* s->clk_prevval		CAN BE INFERRED! */
/* s->clk_prevtrans		CAN BE INFERRED! */
/* s->clk_delta 		CAN BE INFERRED! */

switch(numbytes_trans&3)
	{
	case 0: lt->lt_emit_u8(lt, numtrans); break;
	case 1: lt->lt_emit_u16(lt, numtrans); break;
	case 2: lt->lt_emit_u24(lt, numtrans); break;
	case 3: lt->lt_emit_u32(lt, numtrans); break;
	}

/* printf("Clock finish for '%s' at %Ld ending with '%c' for %d repeats over a switch delta of %d\n", 
	s->name, lt->timeval, s->clk_prevval, s->clk_numtrans - LT_CLKPACK, s->clk_delta); */
s->clk_prevtrans = ULLDescriptor(~0);
s->clk_numtrans = 0;
}

static void lt_flushclock_m(struct lt_trace *lt, struct lt_symbol *s)
{
unsigned int last_change_delta = lt->position - s->last_change - 2;
unsigned int start_position = lt->position;
int tag;
int numbytes, numbytes_trans;
int numtrans = s->clk_numtrans - LT_CLKPACK_M - 1;

if(numtrans<0)
	{
	/* it never got around to caching */
	fprintf(stderr, "Possible Problem with %s with %d?\n", s->name, s->clk_numtrans);
	return;
	}

if(last_change_delta >= 256*65536)
	{
	numbytes = 3;
	}
else
if(last_change_delta >= 65536)
	{
	numbytes = 2;
	}
else
if(last_change_delta >= 256)
	{
	numbytes = 1;
	}
else
	{
	numbytes = 0;
	}

if(numtrans >= 256*65536)
	{
	numbytes_trans = 3;
	}
else
if(numtrans >= 65536)
	{
	numbytes_trans = 2;
	}
else
if(numtrans >= 256)
	{
	numbytes_trans = 1;
	}
else
	{
	numbytes_trans = 0;
	}

tag = (numbytes<<4) + 0xC + numbytes_trans;	/* yields xC..xF */

lt->lt_emit_u8(lt, tag);
switch(numbytes&3)
	{
	case 0: lt->lt_emit_u8(lt, last_change_delta); break;
	case 1: lt->lt_emit_u16(lt, last_change_delta); break;
	case 2: lt->lt_emit_u24(lt, last_change_delta); break;
	case 3: lt->lt_emit_u32(lt, last_change_delta); break;
	}

s->last_change = start_position;

/* s->clk_prevval		CAN BE INFERRED! */
/* s->clk_prevtrans		CAN BE INFERRED! */
/* s->clk_delta 		CAN BE INFERRED! */

switch(numbytes_trans&3)
	{
	case 0: lt->lt_emit_u8(lt, numtrans); break;
	case 1: lt->lt_emit_u16(lt, numtrans); break;
	case 2: lt->lt_emit_u24(lt, numtrans); break;
	case 3: lt->lt_emit_u32(lt, numtrans); break;
	}

/* printf("Clock finish for '%s' at %Ld ending with '%08x' for %d repeats over a switch delta of %Ld\n", 
	s->name, lt->timeval, s->clk_prevval, s->clk_numtrans - LT_CLKPACK_M, s->clk_delta); */
s->clk_prevtrans = ULLDescriptor(~0);
s->clk_numtrans = 0;
}


/*
 * close out the trace and fixate it
 */
void lt_close(struct lt_trace *lt)
{
lxttime_t lasttime=ULLDescriptor(0);
int lastposition=0;
char is64=0;

if(lt)
	{
	struct lt_symbol *s = lt->symchain;

	if(lt->clock_compress)
	while(s)
		{
               if(s->clk_prevtrans!=ULLDescriptor(~0))
                        {
                        int len = ((s->flags)&LT_SYM_F_INTEGER) ? 32 : s->len;
                        if(len>1)
                                {
                                if(s->clk_numtrans > LT_CLKPACK_M) lt_flushclock_m(lt, s);
                                }
                                else
                                {   
                                if(s->clk_numtrans > LT_CLKPACK) lt_flushclock(lt, s);
                                }
                        }
                         
                s=s->symchain;
		}	

	if(lt->zmode)
		{
		lt->chg_table_size = lt->position - lt->change_field_offset;
		gzclose(lt->zhandle); lt->zhandle = NULL;
		fseek(lt->handle, 0L, SEEK_END);
		lt->position=ftell(lt->handle);

		lt_set_zmode(lt, 0);
		lt->zchg_table_size = lt->position - lt->change_field_offset;
		}

	lt_emitfacs(lt);

	if(lt->timebuff)
		{
		free(lt->timebuff);
		lt->timebuff=NULL;
		}
	if(lt->timehead)
		{
		struct lt_timetrail *t=lt->timehead;
		struct lt_timetrail *t2;

		lt->time_table_offset = lt->position;

		lt_emit_u32(lt, lt->timechangecount);	/* this is uncompressed! */

		fflush(lt->handle);
		lt->zhandle = gzdopen(dup(fileno(lt->handle)), "wb9");
		lt->ztime_table_size = lt->position;

		is64=(lt->maxtime > ULLDescriptor(0xFFFFFFFF));

		if(is64)
			{
			lt_emit_u64z(lt, (int)((lt->mintime)>>32), (int)lt->mintime);
			lt_emit_u64z(lt, (int)((lt->maxtime)>>32), (int)lt->maxtime);
			}
			else
			{
			lt_emit_u32z(lt, (int)lt->mintime);
			lt_emit_u32z(lt, (int)lt->maxtime);
			}

		while(t)
			{
			lt_emit_u32z(lt, t->position - lastposition); lastposition = t->position;
			t=t->next;			
			}

		t=lt->timehead;
		if(is64)
			{
			while(t)
				{
				lxttime_t delta = t->timeval - lasttime;
				lt_emit_u64z(lt, (int)(delta>>32), (int)delta); lasttime = t->timeval;
				
				t2=t->next;
				free(t);			
				t=t2;
				}
			}
		else
			{
			while(t)
				{
				lt_emit_u32z(lt, (int)(t->timeval - lasttime)); lasttime = t->timeval;
				
				t2=t->next;
				free(t);			
				t=t2;
				}
	
			lt->timehead = lt->timecurr = NULL;
			}

		gzclose(lt->zhandle); lt->zhandle = NULL;
		fseek(lt->handle, 0L, SEEK_END);
		lt->position=ftell(lt->handle);
		lt->ztime_table_size = lt->position - lt->ztime_table_size;
		}

	if(lt->initial_value>=0) 
		{
		lt->initial_value_offset = lt->position;
		lt_emit_u8(lt, lt->initial_value);
		}

	if((lt->timescale>-129)&(lt->timescale<128))
		{
		lt->timescale_offset = lt->position;
		lt_emit_u8(lt, lt->timescale);
		}

	if(lt->double_used)
		{
		lt->double_test_offset = lt->position;
		lt_emit_double(lt, 3.14159);		
		}

	lt_emit_u8(lt, LT_SECTION_END);
	if(lt->change_field_offset) { lt_emit_u32(lt, lt->change_field_offset); lt_emit_u8(lt, LT_SECTION_CHG); }
	if(lt->sync_table_offset) { lt_emit_u32(lt, lt->sync_table_offset); lt_emit_u8(lt, LT_SECTION_SYNC_TABLE); }
	if(lt->facname_offset) { lt_emit_u32(lt, lt->facname_offset); lt_emit_u8(lt, LT_SECTION_FACNAME); }
	if(lt->facgeometry_offset) { lt_emit_u32(lt, lt->facgeometry_offset); lt_emit_u8(lt, LT_SECTION_FACNAME_GEOMETRY); }
	if(lt->timescale_offset) { lt_emit_u32(lt, lt->timescale_offset); lt_emit_u8(lt, LT_SECTION_TIMESCALE); }
	if(lt->time_table_offset) { lt_emit_u32(lt, lt->time_table_offset); lt_emit_u8(lt, is64 ? LT_SECTION_TIME_TABLE64 : LT_SECTION_TIME_TABLE); }
	if(lt->initial_value_offset) { lt_emit_u32(lt, lt->initial_value_offset); lt_emit_u8(lt, LT_SECTION_INITIAL_VALUE); }
	if(lt->double_test_offset) { lt_emit_u32(lt, lt->double_test_offset); lt_emit_u8(lt, LT_SECTION_DOUBLE_TEST); }

	/* Version 2 adds */
	if(lt->zfacname_predec_size) { lt_emit_u32(lt, lt->zfacname_predec_size); lt_emit_u8(lt, LT_SECTION_ZFACNAME_PREDEC_SIZE); }
	if(lt->zfacname_size) { lt_emit_u32(lt, lt->zfacname_size); lt_emit_u8(lt, LT_SECTION_ZFACNAME_SIZE); }
	if(lt->zfacgeometry_size) { lt_emit_u32(lt, lt->zfacgeometry_size); lt_emit_u8(lt, LT_SECTION_ZFACNAME_GEOMETRY_SIZE); }
	if(lt->zsync_table_size) { lt_emit_u32(lt, lt->zsync_table_size); lt_emit_u8(lt, LT_SECTION_ZSYNC_SIZE); }
	if(lt->ztime_table_size) { lt_emit_u32(lt, lt->ztime_table_size); lt_emit_u8(lt, LT_SECTION_ZTIME_TABLE_SIZE); }
	if(lt->chg_table_size) { lt_emit_u32(lt, lt->chg_table_size); lt_emit_u8(lt, LT_SECTION_ZCHG_PREDEC_SIZE); }
	if(lt->zchg_table_size) { lt_emit_u32(lt, lt->zchg_table_size); lt_emit_u8(lt, LT_SECTION_ZCHG_SIZE); }

	lt_emit_u8(lt, LT_TRLID);

	if(lt->symchain)
		{
		struct lt_symbol *s = lt->symchain;
		struct lt_symbol *s2;
		
		while(s)
			{
			free(s->name);
			s2=s->next;
			free(s);
			s=s2;
			}
		}
	
	fclose(lt->handle);
	free(lt);
	}

}


/*
 * maint function for finding a symbol if it exists
 */
struct lt_symbol *lt_symbol_find(struct lt_trace *lt, const char *name)
{
struct lt_symbol *s=NULL;

if((lt)&&(name)) s=lt_symfind(lt, name);
return(s);
}


/*
 * add a trace (if it doesn't exist already)
 */
struct lt_symbol *lt_symbol_add(struct lt_trace *lt, const char *name, unsigned int rows, int msb, int lsb, int flags)
{
struct lt_symbol *s;
int len;
int flagcnt;

flagcnt = ((flags&LT_SYM_F_INTEGER)!=0) + ((flags&LT_SYM_F_DOUBLE)!=0) + ((flags&LT_SYM_F_STRING)!=0);

if((flagcnt>1)||(!lt)||(!name)||(lt_symfind(lt, name))) return (NULL);

if(flags&LT_SYM_F_DOUBLE) lt->double_used = 1;

s=lt_symadd(lt, name, lt_hash(name));
s->rows = rows;
s->flags = flags&(~LT_SYM_F_ALIAS);	/* aliasing makes no sense here.. */

if(!flagcnt)
	{
	s->msb = msb;
	s->lsb = lsb;
	s->len = (msb<lsb) ? (lsb-msb+1) : (msb-lsb+1);

	if((s->len==1)&&(s->rows==0)) s->clk_prevtrans = ULLDescriptor(~0);
	}

s->symchain = lt->symchain;
lt->symchain = s;
lt->numfacs++;
if((len=strlen(name)) > lt->longestname) lt->longestname = len;
lt->numfacbytes += (len+1);

return(s);
}

/*
 * add an alias trace (if it doesn't exist already and orig is found)
 */
struct lt_symbol *lt_symbol_alias(struct lt_trace *lt, const char *existing_name, const char *alias, int msb, int lsb)
{
struct lt_symbol *s, *sa;
int len;
int bitlen;
int flagcnt;

if((!lt)||(!existing_name)||(!alias)||(!(s=lt_symfind(lt, existing_name)))||(lt_symfind(lt, alias))) return (NULL);

while(s->aliased_to)	/* find root alias */
	{
	s=s->aliased_to;
	}

flagcnt = ((s->flags&LT_SYM_F_INTEGER)!=0) + ((s->flags&LT_SYM_F_DOUBLE)!=0) + ((s->flags&LT_SYM_F_STRING)!=0);
bitlen = (msb<lsb) ? (lsb-msb+1) : (msb-lsb+1);
if((!flagcnt)&&(bitlen!=s->len)) return(NULL);

sa=lt_symadd(lt, alias, lt_hash(alias));
sa->flags = LT_SYM_F_ALIAS;	/* only point this can get set */
sa->aliased_to = s;

if(!flagcnt)
	{
	sa->msb = msb;
	sa->lsb = lsb;
	sa->len = bitlen;
	}

sa->symchain = lt->symchain;
lt->symchain = sa;
lt->numfacs++;
if((len=strlen(alias)) > lt->longestname) lt->longestname = len;
lt->numfacbytes += (len+1);

return(sa);
}


/* 
 * set current time
 */
int lt_inc_time_by_delta(struct lt_trace *lt, unsigned int timeval)
{
return(lt_set_time64(lt, lt->maxtime + (lxttime_t)timeval));
}

int lt_set_time(struct lt_trace *lt, unsigned int timeval)
{
return(lt_set_time64(lt, (lxttime_t)timeval));
}

int lt_inc_time_by_delta64(struct lt_trace *lt, lxttime_t timeval)
{
return(lt_set_time64(lt, lt->maxtime + timeval));
}

int lt_set_time64(struct lt_trace *lt, lxttime_t timeval)
{
int rc=0;

if(lt)
	{
	struct lt_timetrail *trl=(struct lt_timetrail *)calloc(1, sizeof(struct lt_timetrail));
	if(trl)
		{
		trl->timeval = timeval;
		trl->position = lt->position;

		if((lt->timecurr)||(lt->timebuff))
			{
			if(((timeval>lt->mintime)&&(timeval>lt->maxtime))||((lt->mintime==ULLDescriptor(1))&&(lt->maxtime==ULLDescriptor(0))))
				{
				lt->maxtime = timeval;
				}
				else
				{
				free(trl);
				goto bail;				
				}
			}
			else
			{
			lt->mintime = lt->maxtime = timeval;
			}

		if(lt->timebuff)
			{
			free(lt->timebuff);
			}
		lt->timebuff = trl;
		lt->timeval = timeval;
		rc=1;
		}
	}

bail:
return(rc);
}


/*
 * sets trace timescale as 10**x seconds
 */
void lt_set_timescale(struct lt_trace *lt, int timescale)
{
if(lt)
	{
	lt->timescale = timescale;
	}
}


/*
 * sets clock compression heuristic
 */
void lt_set_clock_compress(struct lt_trace *lt)
{
if(lt)
	{
	lt->clock_compress = 1;
	}
}


/*
 * sets change dump compression
 */
void lt_set_chg_compress(struct lt_trace *lt)
{
if(lt)
	{
	if((!lt->zmode)&&(!lt->emitted))
		{
		lt_set_zmode(lt, lt->zmode = 1);
		fflush(lt->handle);
		lt->zhandle = gzdopen(dup(fileno(lt->handle)), "wb9");
		}
	}
}




/*
 * sets trace initial value
 */
void lt_set_initial_value(struct lt_trace *lt, char value)
{
if(lt)
	{
	int tag;
	switch(value)
		{		
		case '0':	tag = 0; break;
		case '1':	tag = 1; break;
		case 'Z':	
		case 'z':	tag = 2; break;
		case 'X':	
		case 'x':	tag = 3; break;
		case 'H':
		case 'h':	tag = 4; break;
		case 'U':
		case 'u':	tag = 5; break;
		case 'W':
		case 'w':	tag = 6; break;
		case 'L':
		case 'l':	tag = 0x7; break;
		case '-':	tag = 0x8; break;
		default:	tag = -1;  break;
		}
	lt->initial_value = tag;
	}
}


/*
 * Sets bracket stripping (useful for VCD conversions of
 * bitblasted nets)
 */
void lt_symbol_bracket_stripping(struct lt_trace *lt, int doit)
{
if(lt)
	{
	lt->do_strip_brackets = (doit!=0);
	}
}


/*
 * emission for trace values..
 */
static int lt_optimask[]=
{
0x00000000,

0x00000001,
0x00000003,
0x00000007,
0x0000000f,

0x0000001f,
0x0000003f,
0x0000007f,
0x000000ff,

0x000001ff,
0x000003ff,
0x000007ff,
0x00000fff,

0x00001fff,
0x00003fff,
0x00007fff,
0x0000ffff,

0x0001ffff,
0x0003ffff,
0x0007ffff,
0x000fffff,

0x001fffff,
0x003fffff,
0x007fffff,
0x00ffffff,

0x01ffffff,
0x03ffffff,
0x07ffffff,
0x0fffffff,

0x1fffffff,
0x3fffffff,
0x7fffffff,
0xffffffff
};


int lt_emit_value_int(struct lt_trace *lt, struct lt_symbol *s, unsigned int row, int value)
{
int rc=0;

if((!lt)||(!s)) return(rc);

if(!lt->emitted) lt->emitted = 1;

while(s->aliased_to)	/* find root alias if exists */
	{
	s=s->aliased_to;
	}

if(!(s->flags&(LT_SYM_F_DOUBLE|LT_SYM_F_STRING)))
	{
	int numbytes;				/* number of bytes to store value minus one */
	int len = ((s->flags)&LT_SYM_F_INTEGER) ? 32 : s->len;
	unsigned int last_change_delta;

	if((lt->clock_compress)&&(s->rows==0))
	if((len>1)&&(len<=32))
		{
		int ivalue = value;
		s->clk_mask <<= 1;
		s->clk_mask |= 1;

		if(((s->clk_mask&0x1f)==0x1f) && ((ivalue - s->clk_prevval1)==(s->clk_prevval1 - s->clk_prevval3)) &&
			((s->clk_prevval - s->clk_prevval2)==(s->clk_prevval2 - s->clk_prevval4)))
			{
			if(s->clk_prevtrans==ULLDescriptor(~0))
				{
				s->clk_prevtrans = lt->timeval;
				s->clk_numtrans = 0;
				}
			else
			if(s->clk_numtrans == 0)
				{
				s->clk_delta = lt->timeval - s->clk_prevtrans;
				s->clk_prevtrans = lt->timeval;
				s->clk_numtrans++;
				}
			else
				{
				if(s->clk_delta == (lt->timeval - s->clk_prevtrans))
					{
					s->clk_numtrans++;
					s->clk_prevtrans = lt->timeval;
					if(s->clk_numtrans > LT_CLKPACK_M)
						{
						s->clk_prevval4 = s->clk_prevval3;
						s->clk_prevval3 = s->clk_prevval2;
						s->clk_prevval2 = s->clk_prevval1;
						s->clk_prevval1 = s->clk_prevval;
						s->clk_prevval  = ivalue;

						/* printf("Clock value '%08x' for '%s' at %Ld (#%d)\n", ivalue, s->name, lt->timeval, s->clk_numtrans); */
						return(1);
						}
					}
					else
					{
					if(s->clk_numtrans > LT_CLKPACK_M)
						{
						lt_flushclock_m(lt, s);
						/* flush clock then continue below! */
						}
						else
						{
						s->clk_prevtrans=ULLDescriptor(~0);
						}
					}
				}

			}
			else
			{
			if(s->clk_numtrans > LT_CLKPACK_M)
				{
				lt_flushclock_m(lt, s);
				/* flush clock then continue below! */
				}
				else
				{
				s->clk_prevtrans=ULLDescriptor(~0);
				}
			}

		s->clk_prevval4 = s->clk_prevval3;
		s->clk_prevval3 = s->clk_prevval2;
		s->clk_prevval2 = s->clk_prevval1;
		s->clk_prevval1 = s->clk_prevval;
		s->clk_prevval  = ivalue;
		}
	else
	if(len==1) /* possible clock handling */
		{
		int ivalue = value&1;

		if(((s->clk_prevval == '1') && (ivalue==0)) || ((s->clk_prevval == '0') && (ivalue==1)))
			{
			if(s->clk_prevtrans==ULLDescriptor(~0))
				{
				s->clk_prevtrans = lt->timeval;
				s->clk_numtrans = 0;
				}
			else
			if(s->clk_numtrans == 0)
				{
				s->clk_delta = lt->timeval - s->clk_prevtrans;
				s->clk_prevtrans = lt->timeval;
				s->clk_numtrans++;
				}
			else
				{
				if(s->clk_delta == (lt->timeval - s->clk_prevtrans))
					{
					s->clk_numtrans++;
					s->clk_prevtrans = lt->timeval;
					if(s->clk_numtrans > LT_CLKPACK)
						{
						s->clk_prevval = ivalue + '0';

						/* printf("Clock value '%d' for '%s' at %d (#%d)\n", ivalue, s->name, lt->timeval, s->clk_numtrans); */
						return(1);
						}
					}
					else
					{
					if(s->clk_numtrans > LT_CLKPACK)
						{
						lt_flushclock(lt, s);
						/* flush clock then continue below! */
						}
						else
						{
						s->clk_prevtrans=ULLDescriptor(~0);
						}
					}
				}

			}
			else
			{
			if(s->clk_numtrans > LT_CLKPACK)
				{
				lt_flushclock(lt, s);
				/* flush clock then continue below! */
				}
				else
				{
				s->clk_prevtrans=ULLDescriptor(~0);
				}
			}

		s->clk_prevval = ivalue + '0';
		}

	/* normal trace handling */

	last_change_delta = lt->position - s->last_change - 2;

	if(last_change_delta >= 256*65536)
		{
		numbytes = 3;
		}
	else
	if(last_change_delta >= 65536)
		{
		numbytes = 2;
		}
	else
	if(last_change_delta >= 256)
		{
		numbytes = 1;
		}
	else
		{
		numbytes = 0;
		}

	if(len<=32)
		{
		int start_position = lt->position;
		int tag;
		int optimized0 = ((value&lt_optimask[len])==0);
		int optimized1 = ((value&lt_optimask[len])==lt_optimask[len]);
		int optimized  = optimized0|optimized1;

		if(optimized)
			{
			tag = (numbytes<<4) | (3+optimized1);	/* for x3 and x4 cases */
			}
			else
			{
			tag = (numbytes<<4);
			}

		lt->lt_emit_u8(lt, tag);
		switch(numbytes&3)
			{
			case 0: lt->lt_emit_u8(lt, last_change_delta); break;
			case 1: lt->lt_emit_u16(lt, last_change_delta); break;
			case 2: lt->lt_emit_u24(lt, last_change_delta); break;
			case 3: lt->lt_emit_u32(lt, last_change_delta); break;
			}
	
		s->last_change = start_position;

		if(s->rows>0)
			{
			if(s->rows >= 256*65536)
				{
				numbytes = 3;
				}
			else
			if(s->rows >= 65536)
				{
				numbytes = 2;
				}
			else
			if(s->rows >= 256)
				{
				numbytes = 1;
				}
			else
				{
				numbytes = 0;
				}

			switch(numbytes&3)
				{
				case 0: lt->lt_emit_u8(lt, row); break;
				case 1: lt->lt_emit_u16(lt, row); break;
				case 2: lt->lt_emit_u24(lt, row); break;
				case 3: lt->lt_emit_u32(lt, row); break;
				}
			}

		if(!optimized)
			{
			if(len<9)
				{
				value <<= (8-len);
				rc=lt->lt_emit_u8(lt, value);
				}
			else
			if(len<17)
				{
				value <<= (16-len);
				rc=lt->lt_emit_u16(lt, value);
				}
			else
			if(len<25)
				{
				value <<= (24-len);
				rc=lt->lt_emit_u24(lt, value);
				}
			else			
				{
				value <<= (32-len);
				rc=lt->lt_emit_u32(lt, value);
				}
			}
		}	
	
	if(lt->timebuff)
		{
		lt->timechangecount++;
		if(lt->timecurr)
			{
			lt->timecurr->next = lt->timebuff;
			lt->timecurr = lt->timebuff;
			}
			else
			{
			lt->timehead = lt->timecurr = lt->timebuff;
			}

		lt->timebuff=NULL;
		}
	}

return(rc);
}


int lt_emit_value_double(struct lt_trace *lt, struct lt_symbol *s, unsigned int row, double value)
{
int rc=0;
int start_position;
int tag;

if((!lt)||(!s)) return(rc);

if(!lt->emitted) lt->emitted = 1;

while(s->aliased_to)	/* find root alias if exists */
	{
	s=s->aliased_to;
	}

if((s->flags)&LT_SYM_F_DOUBLE)
	{
	int numbytes;				/* number of bytes to store value minus one */
	unsigned int last_change_delta = lt->position - s->last_change - 2;

	if(last_change_delta >= 256*65536)
		{
		numbytes = 3;
		}
	else
	if(last_change_delta >= 65536)
		{
		numbytes = 2;
		}
	else
	if(last_change_delta >= 256)
		{
		numbytes = 1;
		}
	else
		{
		numbytes = 0;
		}

	start_position = lt->position;
	tag = (numbytes<<4);
	lt->lt_emit_u8(lt, tag);
	switch(numbytes&3)
		{
		case 0: lt->lt_emit_u8(lt, last_change_delta); break;
		case 1: lt->lt_emit_u16(lt, last_change_delta); break;
		case 2: lt->lt_emit_u24(lt, last_change_delta); break;
		case 3: lt->lt_emit_u32(lt, last_change_delta); break;
		}

	s->last_change = start_position;

	if(s->rows>0)
		{
		if(s->rows >= 256*65536)
			{
			numbytes = 3;
			}
		else
		if(s->rows >= 65536)
			{
			numbytes = 2;
			}
		else
		if(s->rows >= 256)
			{
			numbytes = 1;
			}
		else
			{
			numbytes = 0;
			}

		switch(numbytes&3)
			{
			case 0: lt->lt_emit_u8(lt, row); break;
			case 1: lt->lt_emit_u16(lt, row); break;
			case 2: lt->lt_emit_u24(lt, row); break;
			case 3: lt->lt_emit_u32(lt, row); break;
			}
		}

	rc=lt->lt_emit_double(lt, value);
	
	if(lt->timebuff)
		{
		lt->timechangecount++;
		if(lt->timecurr)
			{
			lt->timecurr->next = lt->timebuff;
			lt->timecurr = lt->timebuff;
			}
			else
			{
			lt->timehead = lt->timecurr = lt->timebuff;
			}

		lt->timebuff=NULL;
		}
	}

return(rc);
}


int lt_emit_value_string(struct lt_trace *lt, struct lt_symbol *s, unsigned int row, char *value)
{
int rc=0;
int start_position;
int tag;

if((!lt)||(!s)||(!value)) return(rc);

if(!lt->emitted) lt->emitted = 1;

while(s->aliased_to)	/* find root alias if exists */
	{
	s=s->aliased_to;
	}

if((s->flags)&LT_SYM_F_STRING)
	{
	int numbytes;				/* number of bytes to store value minus one */
	unsigned int last_change_delta = lt->position - s->last_change - 2;

	if(last_change_delta >= 256*65536)
		{
		numbytes = 3;
		}
	else
	if(last_change_delta >= 65536)
		{
		numbytes = 2;
		}
	else
	if(last_change_delta >= 256)
		{
		numbytes = 1;
		}
	else
		{
		numbytes = 0;
		}

	start_position = lt->position;
	tag = (numbytes<<4);
	lt->lt_emit_u8(lt, tag);
	switch(numbytes&3)
		{
		case 0: lt->lt_emit_u8(lt, last_change_delta); break;
		case 1: lt->lt_emit_u16(lt, last_change_delta); break;
		case 2: lt->lt_emit_u24(lt, last_change_delta); break;
		case 3: lt->lt_emit_u32(lt, last_change_delta); break;
		}

	s->last_change = start_position;

	if(s->rows>0)
		{
		if(s->rows >= 256*65536)
			{
			numbytes = 3;
			}
		else
		if(s->rows >= 65536)
			{
			numbytes = 2;
			}
		else
		if(s->rows >= 256)
			{
			numbytes = 1;
			}
		else
			{
			numbytes = 0;
			}

		switch(numbytes&3)
			{
			case 0: lt->lt_emit_u8(lt, row); break;
			case 1: lt->lt_emit_u16(lt, row); break;
			case 2: lt->lt_emit_u24(lt, row); break;
			case 3: lt->lt_emit_u32(lt, row); break;
			}
		}

	rc=lt->lt_emit_string(lt, value);
	
	if(lt->timebuff)
		{
		lt->timechangecount++;
		if(lt->timecurr)
			{
			lt->timecurr->next = lt->timebuff;
			lt->timecurr = lt->timebuff;
			}
			else
			{
			lt->timehead = lt->timecurr = lt->timebuff;
			}

		lt->timebuff=NULL;
		}
	}

return(rc);
}


int lt_emit_value_bit_string(struct lt_trace *lt, struct lt_symbol *s, unsigned int row, char *value)
{
int rc=0;
int start_position;
int tag, tagadd;

if((!lt)||(!s)||(!value)||(!*value)) return(rc);

if(!lt->emitted) lt->emitted = 1;

while(s->aliased_to)	/* find root alias if exists */
	{
	s=s->aliased_to;
	}

if(!(s->flags&(LT_SYM_F_DOUBLE|LT_SYM_F_STRING)))
	{
	int numbytes;				/* number of bytes to store value minus one */
	char *pnt;
	int mvl=0;
	char ch;
	char prevch;
	unsigned int last_change_delta;

	int len = ((s->flags)&LT_SYM_F_INTEGER) ? 32 : s->len;

	if((lt->clock_compress)&&(s->rows==0))
	if((len>1)&&(len<=32))
		{
		int legal = 0;
		int ivalue = 0;
		int i;
		char *pnt = value;

		for(i=0;i<len;i++)
			{
			if((*pnt!='0')&&(*pnt!='1'))
				{
				if((!*pnt)&&(i>0))
					{
					pnt--;
					}
					else
					{
					legal = 0;
					break;
					}
				}

			ivalue = (((unsigned int)ivalue) << 1);
			ivalue |= (*pnt & 1);
			legal = 1;
			pnt++;
			}
		s->clk_mask <<= 1;
		s->clk_mask |= legal;

		if(((s->clk_mask&0x1f)==0x1f) && ((ivalue - s->clk_prevval1)==(s->clk_prevval1 - s->clk_prevval3)) &&
			((s->clk_prevval - s->clk_prevval2)==(s->clk_prevval2 - s->clk_prevval4)))
			{
			if(s->clk_prevtrans==ULLDescriptor(~0))
				{
				s->clk_prevtrans = lt->timeval;
				s->clk_numtrans = 0;
				}
			else
			if(s->clk_numtrans == 0)
				{
				s->clk_delta = lt->timeval - s->clk_prevtrans;
				s->clk_prevtrans = lt->timeval;
				s->clk_numtrans++;
				}
			else
				{
				if(s->clk_delta == (lt->timeval - s->clk_prevtrans))
					{
					s->clk_numtrans++;
					s->clk_prevtrans = lt->timeval;
					if(s->clk_numtrans > LT_CLKPACK_M)
						{
						s->clk_prevval4 = s->clk_prevval3;
						s->clk_prevval3 = s->clk_prevval2;
						s->clk_prevval2 = s->clk_prevval1;
						s->clk_prevval1 = s->clk_prevval;
						s->clk_prevval  = ivalue;

						/* printf("Clock value '%08x' for '%s' [len=%d] at %Ld (#%d)\n", 
							ivalue, s->name, len, lt->timeval, s->clk_numtrans); */
						return(1);
						}
					}
					else
					{
					if(s->clk_numtrans > LT_CLKPACK_M)
						{
						lt_flushclock_m(lt, s);
						/* flush clock then continue below! */
						}
						else
						{
						s->clk_prevtrans=ULLDescriptor(~0);
						}
					}
				}

			}
			else
			{
			if(s->clk_numtrans > LT_CLKPACK_M)
				{
				lt_flushclock_m(lt, s);
				/* flush clock then continue below! */
				}
				else
				{
				s->clk_prevtrans=ULLDescriptor(~0);
				}
			}

		s->clk_prevval4 = s->clk_prevval3;
		s->clk_prevval3 = s->clk_prevval2;
		s->clk_prevval2 = s->clk_prevval1;
		s->clk_prevval1 = s->clk_prevval;
		s->clk_prevval  = ivalue;
		}
	else
	if(len==1) /* possible clock handling */
		{
		if(((s->clk_prevval == '1') && (value[0]=='0')) || ((s->clk_prevval == '0') && (value[0]=='1')))
			{
			if(s->clk_prevtrans==ULLDescriptor(~0))
				{
				s->clk_prevtrans = lt->timeval;
				s->clk_numtrans = 0;
				}
			else
			if(s->clk_numtrans == 0)
				{
				s->clk_delta = lt->timeval - s->clk_prevtrans;
				s->clk_prevtrans = lt->timeval;
				s->clk_numtrans++;
				}
			else
				{
				if(s->clk_delta == (lt->timeval - s->clk_prevtrans))
					{
					s->clk_numtrans++;
					s->clk_prevtrans = lt->timeval;
					if(s->clk_numtrans > LT_CLKPACK)
						{
						s->clk_prevval = value[0];

						/* printf("Clock value '%c' for '%s' at %Ld (#%d)\n", value[0], s->name, lt->timeval, s->clk_numtrans); */
						return(1);
						}
					}
					else
					{
					if(s->clk_numtrans > LT_CLKPACK)
						{
						lt_flushclock(lt, s);
						/* flush clock then continue below! */
						}
						else
						{
						s->clk_prevtrans=ULLDescriptor(~0);
						}
					}
				}

			}
			else
			{
			if(s->clk_numtrans > LT_CLKPACK)
				{
				lt_flushclock(lt, s);
				/* flush clock then continue below! */
				}
				else
				{
				s->clk_prevtrans=ULLDescriptor(~0);
				}
			}

		s->clk_prevval = value[0];
		}

	/* normal trace handling */

	last_change_delta = lt->position - s->last_change - 2;

	if(last_change_delta >= 256*65536)
		{
		numbytes = 3;
		}
	else
	if(last_change_delta >= 65536)
		{
		numbytes = 2;
		}
	else
	if(last_change_delta >= 256)
		{
		numbytes = 1;
		}
	else
		{
		numbytes = 0;
		}

	pnt = value;
	prevch = *pnt;
	while((ch=*(pnt++)))
		{
		switch(ch)
			{		
			case '0':
			case '1':	mvl|=LT_MVL_2; break;
			case 'Z':	
			case 'z':	
			case 'X':	
			case 'x':	mvl|=LT_MVL_4; break;
			default:	mvl|=LT_MVL_9; break;
			}

		if(prevch!=ch) prevch = 0;
		}

	switch(prevch)
		{		
		case 0x00:	tagadd = 0; break;
		case '0':	tagadd = 3; break;
		case '1':	tagadd = 4; break;
		case 'Z':	
		case 'z':	tagadd = 5; break;
		case 'X':	
		case 'x':	tagadd = 6; break;
		case 'H':
		case 'h':	tagadd = 7; break;
		case 'U':
		case 'u':	tagadd = 8; break;
		case 'W':
		case 'w':	tagadd = 9; break;
		case 'L':
		case 'l':	tagadd = 0xa; break;
		default:	tagadd = 0xb; break;
		}

	if(mvl)
		{
		start_position = lt->position;
		if(tagadd)
			{
			tag = (numbytes<<4) + tagadd;
			}
			else
			{
			tag = (numbytes<<4) + ((mvl&LT_MVL_9)? 2 : ((mvl&LT_MVL_4)? 1 : 0));
			}
		lt->lt_emit_u8(lt, tag);
		switch(numbytes&3)
			{
			case 0: lt->lt_emit_u8(lt, last_change_delta); break;
			case 1: lt->lt_emit_u16(lt, last_change_delta); break;
			case 2: lt->lt_emit_u24(lt, last_change_delta); break;
			case 3: lt->lt_emit_u32(lt, last_change_delta); break;
			}

		s->last_change = start_position;

		if(s->rows>0)
			{
			if(s->rows >= 256*65536)
				{
				numbytes = 3;
				}
			else
			if(s->rows >= 65536)
				{
				numbytes = 2;
				}
			else
			if(s->rows >= 256)
				{
				numbytes = 1;
				}
			else
				{
				numbytes = 0;
				}

			switch(numbytes&3)
				{
				case 0: lt->lt_emit_u8(lt, row); break;
				case 1: lt->lt_emit_u16(lt, row); break;
				case 2: lt->lt_emit_u24(lt, row); break;
				case 3: lt->lt_emit_u32(lt, row); break;
				}
			}

		if(!tagadd)
			{
			int len = ((s->flags)&LT_SYM_F_INTEGER) ? 32 : s->len;
			if((mvl & (LT_MVL_2|LT_MVL_4|LT_MVL_9)) == LT_MVL_2)
				{
				int i;
				int bitpos = 7;
				int outval = 0;
				int thisval= 0;

				pnt = value;				

				for(i=0;i<len;i++)
					{
					if(*pnt)
						{
						thisval = (*pnt=='1');
						pnt++;
						}
					outval |= (thisval<<bitpos);
					bitpos--;
					if((bitpos==-1)||(i==len-1))
						{					
						lt->lt_emit_u8(lt, outval); 
						outval = 0;
						bitpos = 7;
						}
					}
				}
			else
			if((mvl & (LT_MVL_4|LT_MVL_9)) == LT_MVL_4)
				{
				int i;
				int bitpos = 6;
				int outval = 0;
				int thisval= 0;

				pnt = value;				

				for(i=0;i<len;i++)
					{
					if(*pnt)
						{
						switch(*pnt)
							{
							case '0':	thisval = 0; break;
							case '1':	thisval = 1; break;
							case 'X':
							case 'x':	thisval = 3; break;
							default:	thisval = 2; break;
							}
						pnt++;
						}
					outval |= (thisval<<bitpos);
					bitpos-=2;
					if((bitpos==-2)||(i==len-1))
						{					
						lt->lt_emit_u8(lt, outval); 
						outval = 0;
						bitpos = 6;
						}
					}
				}
			else
			/* if(mvl & LT_MVL_9) */
				{
				int i;
				int bitpos = 4;
				int outval = 0;
				int thisval= 0;

				pnt = value;				

				for(i=0;i<len;i++)
					{
					if(*pnt)
						{
						switch(*pnt)
							{
							case '0':	thisval = 0; break;
							case '1':	thisval = 1; break;
							case 'Z':
							case 'z':	thisval = 2; break;
							case 'X':
							case 'x':	thisval = 3; break;
							case 'H':
							case 'h':	thisval = 4; break;
							case 'U':
							case 'u':	thisval = 5; break;
							case 'W':
							case 'w':	thisval = 6; break;
							case 'L':
							case 'l':	thisval = 7; break;
							default:	thisval = 8; break;
							}
						pnt++;
						}
					outval |= (thisval<<bitpos);
					bitpos-=4;
					if((bitpos==-4)||(i==len-1))
						{					
						lt->lt_emit_u8(lt, outval); 
						outval = 0;
						bitpos = 4;
						}
					}
				}
			}

		rc=1;
		}
	
	if(lt->timebuff)
		{
		lt->timechangecount++;
		if(lt->timecurr)
			{
			lt->timecurr->next = lt->timebuff;
			lt->timecurr = lt->timebuff;
			}
			else
			{
			lt->timehead = lt->timecurr = lt->timebuff;
			}

		lt->timebuff=NULL;
		}
	}

return(rc);
}

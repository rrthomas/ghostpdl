

/* Portions Copyright (C) 2005 artofcode LLC.
   Portions Copyright (C) 2005 Artifex Software Inc.
   This software is based in part on the work of the Independent JPEG Group.
   All Rights Reserved.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/ or
   contact Artifex Software, Inc., 101 Lucas Valley Road #110,
   San Rafael, CA  94903, (415)492-9861, for further information. */
/*$Id$*/

#include "scommon.h"
#include "szlibxx.h"

#define ZIP_BLOCK_SIZE 1024
typedef struct zip_block_s zip_block_t;
struct zip_block_s
{
    long readoffset;
    long writeoffset;

    byte data[ZIP_BLOCK_SIZE];
    zip_block_t *next;
};

typedef struct zip_dir_s
{
    unsigned short gp_bitflag;
    unsigned short comp_method;
    long csize;                      /* compressed size */
    long usize;                      /* uncompressed size */

    unsigned short metasize;         /* meta not saved */
    unsigned short namesize;                   
    long skip_len;                  
    byte *name;                     /* file name missing leading / */

    long csaved;                    /* compressed saved max is csize */ 

    zip_block_t *head;              /* read from head */
    zip_block_t *tail;             /* save uncompressed to tail */

    bool keep_it;                  /* don't delete after decompress/read */

    /* compressed buffer and its stream */
    byte *buf;                     /* write buffer */
    stream_cursor s;              /* uncompressed stream */
    z_stream *zs;                  /* zlib stream, 1 per file ? */
    bool newfile;                  /* end of compressed stream triggers new file */
} zip_dir_t;

typedef struct zip_state_s
{
    gs_memory_t *memory;
    bool zip_mode;
    
    bool need_dir;
    bool needfiledata;
    int read_state;
    int dir_read_state;

    int read_dir;                 /* file being read */
    int num_files;    
    zip_dir_t *dir[1024];   /* sk: perhaps a list */
    zip_block_t *free_list;

    
} zip_state_t;

typedef enum zip_error_status_code_s {
    eNeedData = 42,
    eEndOfStream = 911,
    eWriteStall = 912,
} zip_error_status_code_t;


int zip_process(met_parser_state_t *st, met_state_t *mets, zip_state_t *pzip, stream_cursor_read *pr);

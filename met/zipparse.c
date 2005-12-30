#include "assert.h"
#include "gserrors.h"
#include "memory_.h"
#include "zipparse.h"

#include "pltop.h"  /* NB hack to enable disable zip/xml parsing */

inline int stream_has(stream_cursor_read *pr, unsigned int cnt)
{
    return( pr->limit - pr->ptr >= cnt );
}


inline byte read_byte(stream_cursor_read *pr)
{
    pr->ptr++;
    return *pr->ptr;
}

inline unsigned int read2(stream_cursor_read *pr, unsigned short *a)
{
    if (stream_has(pr, 2)) {
	*a =  read_byte(pr) | (read_byte(pr) << 8) ;
	// dprintf2(gs_mem_ptr, "read2 %0hd:%0hx\n", *a, *a);
    }
    else
	return eNeedData;
    return 0;
}

inline unsigned long read4(stream_cursor_read *pr, unsigned long *a)
{
    if (stream_has(pr, 4)) {
	*a = read_byte(pr) |  
	    (read_byte(pr) << 8) |  
	    (read_byte(pr) << 16) |  
	    (read_byte(pr) << 24);
	// dprintf2(gs_mem_ptr, "read4 %0ld:%0lx\n", *a, *a);
    }
    else
	return eNeedData;
    return 0;
}


int zip_init_part(zip_state_t *pzip)
{
    zip_part_t *part = (zip_part_t *)gs_alloc_bytes(pzip->memory, size_of(zip_part_t), 
						    "zip_init_part");
    pzip->num_files++;
    pzip->parts[pzip->num_files - 1] = part;
    pzip->part_read_state = 0;
    part->tail = part->head = part->csaved = part->metasize = part->namesize = 0;
    zip_new_block(pzip, part);
    part->head = part->curr = part->tail;
    part->keep_it = false;  /* read once by default */
    part->buf = NULL;
    part->zs = NULL;
    part->newfile = true;
    
    return 0;
}

int zip_read_part(zip_state_t *pzip, stream_cursor_read *pr)
{
    unsigned long id;
    unsigned short shortInt;
    unsigned long longInt;
    int code = 0;
    zip_part_t *part =0;
    int i;

    switch( pzip->part_read_state ) {
    case 0:
	if ((code = read4(pr, &id)) != 0)
	    return code;
        if (id == 0x04034b50) {
	    if ((code = zip_init_part(pzip)) != 0 )
		return code;
	    part = pzip->parts[pzip->num_files - 1];
	} else { // if (id == 0x02014b50) 
	    // end of archive 
	    pzip->part_read_state = 20;
	    return 0;
	}
	pzip->part_read_state++;

    case 1: /* version to extract*/
	if ((code = read2(pr, &shortInt)) != 0)
	    return code;
	pzip->part_read_state++;

    case 2: /* general*/
	part = pzip->parts[pzip->num_files - 1];
	if ((code = read2(pr, &part->gp_bitflag)) != 0)
	    return code;
	pzip->part_read_state++;
	 
    case 3: /* method */
	part = pzip->parts[pzip->num_files - 1];
	if ((code = read2(pr, &part->comp_method)) != 0)
	    return code;
	pzip->part_read_state++;
   
    case 4: /* last mod file time */
	if ((code = read2(pr, &shortInt)) != 0)
	    return code;
	pzip->part_read_state++;
	    
    case 5: /* last mod file date */
	if ((code = read2(pr, &shortInt)) != 0)
	    return code;
	pzip->part_read_state++;

    case 6: /* crc-32 */
	if ((code = read4(pr, &longInt)) != 0)
	    return code;
	pzip->part_read_state++;

    case 7: /* csize */
	part = pzip->parts[pzip->num_files - 1];
	if ((code = read4(pr, &part->csize)) != 0)
	    return code;
	pzip->part_read_state++;	
	
    case 8: /* usize */
	part = pzip->parts[pzip->num_files - 1];
	if ((code = read4(pr, &part->usize)) != 0)
	    return code;
	pzip->part_read_state++;

    case 9: /* namesize */
	part = pzip->parts[pzip->num_files - 1];
	if ((code = read2(pr, &part->namesize)) != 0)
	    return code;
	pzip->part_read_state++;

    case 10: /* metasize */
	part = pzip->parts[pzip->num_files - 1];
	if ((code = read2(pr, &part->metasize)) != 0)
	    return code;
	pzip->part_read_state++; 

    case 11:
	part = pzip->parts[pzip->num_files - 1];
	if (stream_has(pr, part->namesize)) {
	    part->name = gs_alloc_bytes(pzip->memory, part->namesize + 1, "pzip part name");
	    if (part->name == NULL) 
		return -1;  // memory error
	    memcpy(part->name, pr->ptr +1, part->namesize);
	    part->name[part->namesize] = 0;
	    // Perhaps -ZI spew
	    // dprintf1(pzip->memory, "zip file name:%s\n", part->name );
	    pr->ptr += part->namesize;
	    pzip->part_read_state++;
	} 
	else 
	    return eNeedData;
    case 12:
	part = pzip->parts[pzip->num_files - 1];
	if (stream_has(pr, part->metasize)) {
	    pr->ptr += part->metasize;
	    pzip->part_read_state++;
	}
	else if (part->metasize){
	    part->metasize -= pr->limit - pr->ptr;
	    pr->ptr = pr->limit;
	    return eNeedData;
	}
    case 13:    
	pzip->read_state = 1;  /* read file after this header */
	pzip->part_read_state = 0; /* next call looks for header */
	break;

    case 20:
	/* read extra foo after last file 
	   [archive decryption header] (EFS)
	   [archive extra data record] (EFS)
	   [central directory]
	   [zip64 end of central directory record]
	   [zip64 end of central directory locator] 
	   [end of central directory record]
	*/
	for ( i = pr->limit - pr->ptr; i > 4; --i, pr->ptr++ ) {
	    if ( pr->ptr[1] == 0x50 && 
		 pr->ptr[2] == 0x4b &&
		 pr->ptr[3] == 0x05 &&
		 pr->ptr[4] == 0x06 ) {
		pzip->part_read_state++;
		pr->ptr += 4;  
		break;
	    }
	}
	break;
    case 21:

	if (stream_has(pr, 18) == 0 )
	    return eNeedData;
	pr->ptr += 16;
	if (stream_has(pr, 3)) {
	    read2(pr, &part->skip_len);
	    pzip->part_read_state++;
	} else {
	    pr->ptr = pr->limit - 1;
	    pzip->part_read_state = 0;  /* reset for next zip file */
	    return -102; // e_ExitLanguage;
	}
    case 22:
	if (stream_has(pr, part->skip_len) == 0 )
	    pr->ptr += part->skip_len;


	pzip->part_read_state = 0;  /* reset for next zip file */
	return -102; // e_ExitLanguage;
    
    default:
	/* coding error */
	return -1;
    }
    
    return code;
}

int zip_new_block(zip_state_t *pzip, zip_part_t *part)
{
    int code = 0;
    zip_block_t *blk;

    if (pzip->free_list) {
	blk = pzip->free_list;
	pzip->free_list = blk->next;
	blk->next = NULL;
	blk->readoffset = blk->writeoffset = 0;
    }
    else {
	blk = (zip_block_t *) gs_alloc_bytes(pzip->memory, 
					     sizeof(zip_block_t), 
					     "zip_new_block");	
	if (blk == NULL)
	    return gs_error_VMerror;
	blk->readoffset = blk->writeoffset = 0;
	blk->next = NULL;
    }
    assert(part->write);
    assert(part->write->writeoffset == ZIP_BLOCK_SIZE);

    memset(blk->data, 0xF0, ZIP_BLOCK_SIZE);

    if (part->tail)
	part->tail->next = blk;
    part->tail = blk;

    return code;
}

int zip_save_data(zip_state_t *pzip, stream_cursor_read *pr)
{
    int code = 0;
    zip_part_t *part = pzip->parts[pzip->num_files - 1];
    long wlen = ZIP_BLOCK_SIZE - part->tail->writeoffset;
    long slen = pr->limit - pr->ptr;
    long cleft = max(0, part->csize - part->csaved);

    if (slen == 0)
	return code;

    if (wlen == 0) {
	if ((code = zip_new_block(pzip, part)) != 0) 
	    return code;
	wlen = ZIP_BLOCK_SIZE - part->tail->writeoffset;
    }
    if (cleft == 0 && part->comp_method == 8)
	cleft = slen;
    //  zero for length seems to be the norm.  
    if (cleft) {
	byte *wptr = &part->tail->data[part->tail->writeoffset];
	wlen = min(wlen, slen);
	wlen = min(wlen, cleft);
	memcpy(wptr, pr->ptr + 1, wlen);
	pr->ptr += wlen;
	part->tail->writeoffset += wlen;
	part->csaved += wlen;
    }
    return code;
}

int zip_init_write_stream(zip_state_t *pzip, zip_part_t *part)
{
    if (part->zs == NULL) {
	part->zs = gs_alloc_bytes(pzip->memory, size_of(z_stream), "zip z_stream");
	if (part->zs == NULL)
	    return -1;
	part->zs->zalloc = 0;
	part->zs->zfree = 0;
	part->zs->opaque = 0;
    }
    return 0;
}

int zip_decompress_data(zip_state_t *pzip, stream_cursor_read *pin )
{
    int code = 0;
    zip_part_t *part = pzip->parts[pzip->read_part];
    z_stream *zs = 0;
    long rlen = pin->limit - pin->ptr - 1;
    long wlen = 0;
    byte *wptr = NULL;

    int rstart = pin->ptr;
    long len;
    int status;

    /* add to tail of part's block list */
    wptr = &part->tail->data[part->tail->writeoffset];

    if (ZIP_BLOCK_SIZE <= part->tail->writeoffset + 1) {
	if ((code = zip_new_block(pzip, part)))
	    return code;
	wptr = &part->tail->data[part->tail->writeoffset];
	wlen = ZIP_BLOCK_SIZE;
    }
    else 
	wlen = ((ZIP_BLOCK_SIZE - 1) - part->tail->writeoffset) + 1;

    zip_init_write_stream(pzip, part);
	
    if (part->comp_method == 0) {
	rlen = min(rlen, wlen);
	memcpy(wptr, pin->ptr, rlen);
	part->tail->writeoffset += rlen;
	pin->ptr += rlen;
    }
    else {  /* 8 == flate */
	zs = part->zs;
	zs->next_in = pin->ptr + 1;
	zs->avail_in = rlen;
	zs->next_out = wptr;
	zs->avail_out = wlen;

	if (part->newfile) 	/* init for new file */
	    inflateInit2(part->zs, -15);  /* neg zip, 15bit window */

	status = inflate(zs, Z_NO_FLUSH);

	part->csaved += zs->next_in - pin->ptr - 1;  // update comressed read for zips with csize
	pin->ptr = zs->next_in - 1;
	part->tail->writeoffset += zs->next_out - wptr;

	switch (status) {
        case Z_OK:
	    if (pin->ptr + 1 < pin->limit)
		code = 0;
	    else
		code = pin->ptr > rstart ? 0 : 1;
	    part->newfile = false;
	    break;

        case Z_STREAM_END:
	    part->newfile = true;
	    inflateEnd(zs);
	    code = eEndOfStream; // was: EOFC;
	    break;

        default:
	    code = ERRC;
	}
    }
    return code;
}

int met_set_initial_read_part(met_parser_state_t *st, met_state_t *mets, 
			     zip_state_t *pzip)
{
    int code = 0;
    // hard coded foo
    st->next_read_part = 0;
    return code;
}


bool zip_is_image( zip_part_t *part )
{
    char *str = part->name + strlen(part->name) - 6; 
    if (strncmp(str, "1.xaml", 6) == 0)
	return true; // return false;
    return true;
}


bool zip_have_page( zip_part_t *rpart )
{
    return false;
}

int zip_decomp_process(met_parser_state_t *st, met_state_t *mets, zip_state_t *pzip, 
		       stream_cursor_read *pr)
{
    int code = 0;
    zip_part_t *rpart = NULL;
    

    /* update reading pointer 
     * NB: client should be able to choose a different file to read
     * currently just serially sends file after file.
     */

    if (st->next_read_part < 0)
	return met_set_initial_read_part(st, mets, pzip);
    else if (st->next_read_part < pzip->num_files)
	pzip->read_part = st->next_read_part;
    else 
	return code; // Nothing to do till we have read the start of file.

    rpart = pzip->parts[pzip->read_part];

    /* decompress and send data to parser */
    code = zip_decompress_data(pzip, pr);
    if (code == eEndOfStream) { // good end of input stream 
	
	/* setup to read next part */
	st->next_read_part++;
	pzip->read_state = 2; 
	code = 0;
    

	/* determine if the xps should be parsed and call the xml parser
	 */
	zip_page(st, mets, pzip, rpart);

	if (0) { // stream adjust
	    long len = rpart->s.r.limit - rpart->s.r.ptr;
	    if ( len )
		memmove(rpart->buf, rpart->s.r.ptr, len);
	    rpart->s.r.ptr = rpart->buf -1;
	    rpart->s.r.limit = rpart->buf + ( len - 1 );
	}
    }
    else if (code == eWriteStall) { // write stall on output
	long len = rpart->s.r.limit - rpart->s.r.ptr;
	if ( len && rpart->buf -1 != rpart->s.r.ptr )
	    memmove(rpart->buf, rpart->s.r.ptr, len);
	rpart->s.r.ptr = rpart->buf -1;
	rpart->s.r.limit = rpart->buf + ( len - 1 );
	
	code = 0;  // let it be read
    }
    if (code == eNeedData)  // need data
	return 0;
    return code;
}


/* 
 * code == eNeedData is returned as 0 here.
 */
int
zip_process(met_parser_state_t *st, met_state_t *mets, zip_state_t *pzip, stream_cursor_read *pr)
{
    int code = 0;
    zip_part_t *rpart = NULL;
    zip_part_t *wpart = NULL;
    int last_len = pr->limit - pr->ptr;

    while (pr->limit - pr->ptr > 4 && code == 0) {

	switch (pzip->read_state) {
	case 0: /* local file header and skip of end of zip file records */
	    if ((code = zip_read_part(pzip, pr)) != 0)
		return code;
	    /* 0 : more header to read 
	     * 1 : file to read
	     * 3 : end of file 
	     */
	    break;
	case 1: /* file data */	
	    // don't save compressed data
	    //if ((code = zip_save_data(pzip, pr)) != 0)
	    //return code;

	case 2: /* optional Data descriptor */
	    wpart = pzip->parts[pzip->num_files - 1];
	    if (wpart->csaved && wpart->csaved == wpart->csize ) {
		/* have count and fall through */

		if (wpart->gp_bitflag & 1 << 3) { 
		    /* Skip Data descriptor */
		    if (stream_has(pr, 12) == 0) {
			pzip->read_state = 2;
			return 0;
		    }
		    pr->ptr += 12; 
		}
		pzip->read_state = 0; 
		break;
	    }
	    else if (pzip->read_state == 2  && wpart->gp_bitflag & 1 << 3) {
		 /* else end of stream goto after decompress */ 
		if ( pr->ptr[1] == 0x50 &&
		     pr->ptr[2] == 0x4b &&
		     pr->ptr[3] == 0x07 &&
		     pr->ptr[4] == 0x08 ) {
		    /* check for split archive marker in the middle of the file
		     * spec says it must be first ?
		     */
		    if (stream_has(pr, 16) == 0) 
			return 0;
		    pr->ptr += 16; 
		}
		else {
		    if (stream_has(pr, 12) == 0) 
			return 0;
		    pr->ptr += 12; 
		}
		pzip->read_state = 0; 
		break;
	    }

	case 3:
    
	    if ((code = zip_decomp_process(st, mets, pzip, pr)) != 0 )
		return code;
	    break;

	case 4:
	default :
	    break;
	    //pzip->read_state = 0; 
	}
    }
    if (code == eNeedData) 
	return 0;
    return code;
}


zip_state_t *zip_init_instance(gs_memory_t *mem)
{
    zip_state_t *zip;

    zip = gs_alloc_bytes(mem, sizeof(zip_state_t), "zip_init alloc zip state");

    zip->memory = mem;

#ifdef PARSE_XML_HACK
    zip->zip_mode = false;
#else
    zip->zip_mode = true;
#endif

    zip->read_state = 0;
    zip->part_read_state = 0;
    zip->read_part = -1;
    zip->num_files = 0;
    zip->free_list = NULL;

    return zip;
}

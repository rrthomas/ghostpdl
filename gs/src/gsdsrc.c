/* Copyright (C) 1997 Aladdin Enterprises.  All rights reserved.
  
  This file is part of Aladdin Ghostscript.
  
  Aladdin Ghostscript is distributed with NO WARRANTY OF ANY KIND.  No author
  or distributor accepts any responsibility for the consequences of using it,
  or for whether it serves any particular purpose or works at all, unless he
  or she says so in writing.  Refer to the Aladdin Ghostscript Free Public
  License (the "License") for full details.
  
  Every copy of Aladdin Ghostscript must include a copy of the License,
  normally in a plain ASCII text file named PUBLIC.  The License grants you
  the right to copy, modify and redistribute Aladdin Ghostscript, but only
  under certain conditions described in the License.  Among other things, the
  License requires that the copyright notice and this notice be preserved on
  all copies.
*/

/* gsdsrc.c */
/* DataSource procedures */

#include "memory_.h"
#include "gx.h"
#include "gsdsrc.h"
#include "gserrors.h"
#include "stream.h"

/* GC descriptor */
public_st_data_source();
#define psrc ((gs_data_source_t *)vptr)
private ENUM_PTRS_BEGIN(data_source_enum_ptrs) {
	if ( psrc->access == data_source_access_string )
	  ENUM_RETURN_CONST_STRING_PTR(gs_data_source_t, data.str);
	else if ( psrc->access == data_source_access_bytes )
	  ENUM_RETURN_PTR(gs_data_source_t, data.str.data);
	else /*if ( psrc->access == data_source_access_stream )*/
	  ENUM_RETURN_PTR(gs_data_source_t, data.strm);
} ENUM_PTRS_END
private RELOC_PTRS_BEGIN(data_source_reloc_ptrs) {
	if ( psrc->access == data_source_access_string )
	  RELOC_CONST_STRING_PTR(gs_data_source_t, data.str);
	else if ( psrc->access == data_source_access_bytes )
	  RELOC_PTR(gs_data_source_t, data.str.data);
	else /*if ( psrc->access == data_source_access_stream )*/
	  RELOC_PTR(gs_data_source_t, data.strm);
} RELOC_PTRS_END
#undef psrc

/* Access data from a string or a byte object. */
/* Does *not* check bounds. */
int
data_source_access_string(const gs_data_source_t *psrc, ulong start,
  uint length, byte *buf, const byte **ptr)
{	const byte *p = psrc->data.str.data + start;

	if ( ptr )
	  *ptr = p;
	else
	  memcpy(buf, p, length);
	return 0;
}
/* access_bytes is identical to access_string, but has a different */
/* GC procedure. */
int
data_source_access_bytes(const gs_data_source_t *psrc, ulong start,
  uint length, byte *buf, const byte **ptr)
{	const byte *p = psrc->data.str.data + start;

	if ( ptr )
	  *ptr = p;
	else
	  memcpy(buf, p, length);
	return 0;
}

/* Access data from a stream. */
/* Returns gs_error_rangecheck if out of bounds. */
int
data_source_access_stream(const gs_data_source_t *psrc, ulong start,
  uint length, byte *buf, const byte **ptr)
{	stream *s = psrc->data.strm;
	const byte *p;

	if ( start >= s->position &&
	     (p = start - s->position + s->cbuf) + length <=
	       s->cursor.r.limit + 1
	   ) {
	  if ( ptr )
	    *ptr = p;
	  else
	    memcpy(buf, p, length);
	} else {
	  uint nread;
	  int code = sseek(s, start);

	  if ( code < 0 )
	    return_error(gs_error_rangecheck);
	  code = sgets(s, buf, length, &nread);
	  if ( code < 0 )
	    return_error(gs_error_rangecheck);
	  if ( nread != length )
	    return_error(gs_error_rangecheck);
	  if ( ptr )
	    *ptr = buf;
	}
	return 0;
}

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

/* gxfunc.h */
/* Internal definitions for Functions */

#ifndef gxfunc_INCLUDED
#  define gxfunc_INCLUDED

#include "gsfunc.h"
#include "gsstruct.h"

/* ---------------- Types and structures ---------------- */

/* Define the generic Function structure type.  This is never instantiated. */
extern_st(st_function);
#define public_st_function()	/* in gsfunc.c */\
  gs_public_st_ptrs2(st_function, gs_function_t, "gs_function_t",\
    function_enum_ptrs, function_reloc_ptrs, params.Domain, params.Range)

/* ---------------- Internal procedures ---------------- */

/* Generic free_params implementation. */
void fn_common_free_params(P2(gs_function_params_t *params, gs_memory_t *mem));

/* Generic free implementation. */
void fn_common_free(P3(gs_function_t *pfn, bool free_params, gs_memory_t *mem));

/* Free an array of subsidiary Functions. */
void fn_free_functions(P3(gs_function_t **Functions, int count, gs_memory_t *mem));

/* Check the values of m, n, Domain, and (if supplied) Range. */
int fn_check_mnDR_proc(P3(const gs_function_params_t *params, int m, int n));
#define fn_check_mnDR(params, m, n)\
  do {\
    int code = fn_check_mnDR_proc((const gs_function_params_t *)(params), m, n);\
    if ( code < 0 ) return code;\
  } while ( 0 )

#endif					/* gxfunc_INCLUDED */

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

/* ifunc.h */
/* Internal interpreter interfaces for Functions */

#ifndef ifunc_INCLUDED
#  define ifunc_INCLUDED

/* Define build procedures for the various function types. */
#define build_function_proc(proc)\
  int proc(P4(const_os_ptr op, const gs_function_params_t *params, int depth,\
	      gs_function_t **ppfn))
build_function_proc(build_function_undefined);

/* Define the table of build procedures, indexed by FunctionType. */
extern build_function_proc((*build_function_procs[5]));

/* Build a function structure from a PostScript dictionary. */
int fn_build_sub_function(P3(const ref *op, gs_function_t **ppfn, int depth));
#define fn_build_function(op, ppfn)\
  fn_build_sub_function(op, ppfn, 0)

/* Allocate an array of function objects. */
int ialloc_function_array(P2(uint count, gs_function_t ***pFunctions));

/* Collect a heap-allocated array of floats. */
/* If the key is missing, set *pparray = 0 and return 0; */
/* otherwise set *pparray and return the number of elements. */
int fn_build_float_array(P5(const ref *op, const char _ds *kstr, bool required,
			    bool even, const float **pparray));

#endif					/* ifunc_INCLUDED */

/* Portions Copyright (C) 2001 artofcode LLC.
   Portions Copyright (C) 1996, 2001 Artifex Software Inc.
   Portions Copyright (C) 1988, 2000 Aladdin Enterprises.
   This software is based in part on the work of the Independent JPEG Group.
   All Rights Reserved.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/ or
   contact Artifex Software, Inc., 101 Lucas Valley Road #110,
   San Rafael, CA  94903, (415)492-9861, for further information. */

/*$RCSfile$ $Revision$ */
/* PostScript operators for managing the device filter stack */

/* We probably don't need all of these, they were copied from zdevice.c. */
#include "string_.h"
#include "ghost.h"
#include "oper.h"
#include "ialloc.h"
#include "idict.h"
#include "igstate.h"
#include "iname.h"
#include "interp.h"
#include "iparam.h"
#include "ivmspace.h"
#include "gsmatrix.h"
#include "gsstate.h"
#include "gxdevice.h"
#include "gxgetbit.h"
#include "store.h"
#include "gsdfilt.h"
#include "gdevp14.h"

#ifdef DFILTER_TEST
private int
/* - .pushtestdevicefilter - */
zpushtestdevicefilter(i_ctx_t *i_ctx_p)
{
    gs_device_filter_t *df;
    int code;
    gs_memory_t *mem = gs_memory_stable(imemory);

    code = gs_test_device_filter(&df, mem);
    if (code < 0)
	return code;
    code = gs_push_device_filter(mem, igs, df);
    return code;
}
#endif

private int
/* depth .pushpdf14devicefilter - */
zpushpdf14devicefilter(i_ctx_t *i_ctx_p)
{
    gs_device_filter_t *df;
    int code;
    gs_memory_t *mem = gs_memory_stable(imemory);
    os_ptr op = osp;

    check_type(*op, t_integer);
    code = gs_pdf14_device_filter(&df, op->value.intval, mem);
    if (code < 0)
	return code;
    code = gs_push_device_filter(mem, igs, df); 
    if (code < 0)
	return code;
    pop(1);
    return 0;
}

/* - .popdevicefilter - */
private int
zpopdevicefilter(i_ctx_t *i_ctx_p)
{
    gs_memory_t *mem = gs_memory_stable(imemory);

    return gs_pop_device_filter(mem, igs);
}

const op_def zdfilter_op_defs[] =
{
#ifdef DFILTER_TEST
    {"0.pushtestdevicefilter", zpushtestdevicefilter},
#endif
    {"1.pushpdf14devicefilter", zpushpdf14devicefilter},
    {"0.popdevicefilter", zpopdevicefilter},
    op_def_end(0)
};

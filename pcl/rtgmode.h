/*
 * Copyright (C) 1998 Aladdin Enterprises.
 * All rights reserved.
 *
 * This file is part of Aladdin Ghostscript.
 *
 * Aladdin Ghostscript is distributed with NO WARRANTY OF ANY KIND.  No author
 * or distributor accepts any responsibility for the consequences of using it,
 * or for whether it serves any particular purpose or works at all, unless he
 * or she says so in writing.  Refer to the Aladdin Ghostscript Free Public
 * License (the "License") for full details.
 *
 * Every copy of Aladdin Ghostscript must include a copy of the License,
 * normally in a plain ASCII text file named PUBLIC.  The License grants you
 * the right to copy, modify and redistribute Aladdin Ghostscript, but only
 * under certain conditions described in the License.  Among other things, the
 * License requires that the copyright notice and this notice be preserved on
 * all copies.
 */

/* rtgmode.h - interface to PCL graphics (raster) mode */

#ifndef rtgmode_INCLUDED
#define rtgmode_INCLUDED

#include "rtrstst.h"
#include "pcstate.h"
#include "pcommand.h"

/*
 * Types of entry into graphics mode. Note that implicit entry is distinct
 * from any of the explicit modes.
 */
typedef enum {
    NO_SCALE_LEFT_MARG = 0,
    NO_SCALE_CUR_PT = 1,
    SCALE_LEFT_MARG = 2,
    SCALE_CUR_PTR = 3,
    IMPLICIT = 100
} pcl_gmode_entry_t;


/* enter raster graphics mode */
extern  int     pcl_enter_graphics_mode(
    pcl_state_t *       pcs,
    pcl_gmode_entry_t   mode
);

/* exit raster graphics mode */
extern  int     pcl_end_graphics_mode( pcl_state_t * pcs );

extern  const pcl_init_t    rtgmode_init;
extern  const pcl_init_t    rtlrastr_init;

#endif			/* rtgmode_INCLUDED */

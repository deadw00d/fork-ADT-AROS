/*
    (C) 1997 AROS - The Amiga Research OS
    $Id$

    Desc:
    Lang: english
*/
#include "graphics_intern.h"

/*****************************************************************************

    NAME */
#include <clib/graphics_protos.h>

	AROS_LH6(LONG, ReadPixelLine8,

/*  SYNOPSIS */
	AROS_LHA(struct RastPort * 	, rp		, A0),
	AROS_LHA(ULONG			, xstart	, D0),
	AROS_LHA(ULONG			, ystart	, D1),
	AROS_LHA(ULONG			, width		, D2),
	AROS_LHA(UBYTE           *	, array		, A2),
	AROS_LHA(struct RastPort * 	, tempRP	, A1),

/*  LOCATION */
	struct GfxBase *, GfxBase, 128, Graphics)

/*  FUNCTION

    INPUTS

    RESULT

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO

    INTERNALS

    HISTORY
	27-11-96    digulla automatically created from
			    graphics_lib.fd and clib/graphics_protos.h

*****************************************************************************/
{
    AROS_LIBFUNC_INIT
    
    return driver_ReadPixelLine8(rp
    	, xstart, ystart
	, width
	, array, tempRP
	, GfxBase
    );

    AROS_LIBFUNC_EXIT
} /* ReadPixelLine8 */

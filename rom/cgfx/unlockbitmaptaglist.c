/*
    (C) 1997 AROS - The Amiga Research OS
    $Id$

    Desc:
    Lang: english
*/
#include "cybergraphics_intern.h"

/*****************************************************************************

    NAME */
#include <clib/cybergraphics_protos.h>

	AROS_LH2(void, UnLockBitMapTagList,

/*  SYNOPSIS */
	AROS_LHA(APTR            , , A0),
	AROS_LHA(struct TagItem *, , A1),

/*  LOCATION */
	struct Library *, CyberGfxBase, 30, Cybergraphics)

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
			    cybergraphics_lib.fd and clib/cybergraphics_protos.h

*****************************************************************************/
{
    AROS_LIBFUNC_INIT
    AROS_LIBBASE_EXT_DECL(struct Library *,CyberGfxBase)
    extern void aros_print_not_implemented (char *);

    aros_print_not_implemented ("UnLockBitMapTagList");

    AROS_LIBFUNC_EXIT
} /* UnLockBitMapTagList */

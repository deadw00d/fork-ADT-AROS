/*
    Copyright � 1995-2001, The AROS Development Team. All rights reserved.
    $Id$

    Desc: Write a big endian floating point (32bit) from a streamhook
    Lang: english
*/

#include <proto/dos.h>

/******************************************************************************

    NAME */
#include <stdio.h>
#include <aros/bigendianio.h>
#include <proto/alib.h>

	BOOL WriteFloat (

/*  SYNOPSIS */
	struct Hook * hook,
	FLOAT	      data,
	void	    * stream)

/*  FUNCTION
	Writes one big endian 32bit floating point value to a streamhook.

    INPUTS
	hook - Write to this streamhook
	data - Data to be written
	stream - Stream passed to streamhook

    RESULT
	The function returns TRUE on success and FALSE otherwise.
	See IoErr() for the reason in case of an error.

    NOTES
	This function writes big endian values to a file even on little
	endian machines.

    EXAMPLE

    BUGS

    SEE ALSO
	ReadByte(), ReadWord(), ReadLong(), ReadFloat(), ReadDouble(),
	ReadString(), ReadStruct(), WriteByte(), WriteWord(), WriteLong(),
	WriteFloat(), WriteDouble(), WriteString(), WriteStruct()

    HISTORY

******************************************************************************/
{
    UBYTE * ptr;

#if AROS_BIG_ENDIAN
    ptr = (UBYTE *)&data;
#   define NEXT ++
#else
    ptr = ((UBYTE *)&data) + 3;
#   define NEXT --
#endif

#define WRITE_ONE_BYTE	    \
    if (CallHook (hook, stream, BEIO_WRITE, *ptr NEXT) == EOF) \
	return FALSE

    WRITE_ONE_BYTE;
    WRITE_ONE_BYTE;
    WRITE_ONE_BYTE;
    WRITE_ONE_BYTE;

    return TRUE;
} /* WriteFloat */


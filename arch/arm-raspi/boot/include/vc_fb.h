/*
    Copyright (C) 2013, The AROS Development Team. All rights reserved.
*/

#ifndef VC_FB_H
#define VC_FB_H

#include <inttypes.h>

extern int vcfb_init(void);
extern void fb_Putc(char chr);

#if !defined(KRN_Dummy)
#define KRN_Dummy               (TAG_USER + 0x03d00000)
#endif
#if !defined(KRN_FuncPutC)
#define KRN_FuncPutC      (KRN_Dummy + 99) /* RAW FrameBuffer descriptor */
#endif

#endif	/* VC_FB_H */

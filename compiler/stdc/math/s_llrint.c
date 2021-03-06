/*
    Copyright (C) 1995-2014, The AROS Development Team. All rights reserved.
*/

__FBSDID("$FreeBSD: src/lib/msun/src/s_llrint.c,v 1.1 2005/01/11 23:12:55 das Exp $");

#define type            double
#define roundit         rint
#define dtype           long long
#define fn              llrint
#define fnld    llrintl

#include "s_lrint.c"

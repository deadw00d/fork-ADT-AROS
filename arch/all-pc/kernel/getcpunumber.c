/*
    Copyright (C) 1995-2017, The AROS Development Team. All rights reserved.
*/

#include <aros/kernel.h>
#include <aros/libcall.h>

#include "kernel_base.h"
#include "kernel_intern.h"
#include "apic.h"

AROS_LH0(unsigned int, KrnGetCPUNumber,
         struct KernelBase *, KernelBase, 41, Kernel)
{
    AROS_LIBFUNC_INIT

    return core_APIC_GetNumber(KernelBase->kb_PlatformData->kb_APIC);

    AROS_LIBFUNC_EXIT
}

/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */


#ifndef PTL_PORTALS_COMPAT_H
#define PTL_PORTALS_COMPAT_H

#if PTL_PORTALS_UTCP

#include <p3nal_utcp.h>
#include <p3rt/p3rt.h>
#include <p3api/debug.h>

#elif PTL_PORTALS_REDSTORM

#error "Red Storm Compatibility not implemented"

#else

#error "Unknown Portals library configuration"

#endif

#endif /* PTL_PORTALS_NAL_H */

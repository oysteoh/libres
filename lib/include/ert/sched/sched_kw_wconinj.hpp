/*
   Copyright (C) 2011  Equinor ASA, Norway.

   The file 'sched_kw_wconinj.h' is part of ERT - Ensemble based Reservoir Tool.

   ERT is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   ERT is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.

   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
   for more details.
*/

#ifndef ERT_SCHED_KW_WCONINJ_H
#define ERT_SCHED_KW_WCONINJ_H


#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdbool.h>

#include <ert/util/hash.hpp>
#include <ert/util/stringlist.hpp>

#include <ert/sched/sched_macros.hpp>


typedef struct sched_kw_wconinj_struct sched_kw_wconinj_type;


char ** sched_kw_wconinj_alloc_wells_copy( const sched_kw_wconinj_type * , int * );

/*******************************************************************/

KW_HEADER(wconinj)

#ifdef __cplusplus
}
#endif
#endif

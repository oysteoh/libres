/*
   Copyright (C) 2011  Equinor ASA, Norway.

   The file 'sched_kw_untyped.h' is part of ERT - Ensemble based Reservoir Tool.

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

#ifndef ERT_SCHED_KW_UNTYPED_H
#define ERT_SCHED_KW_UNTYPED_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>

#include <ert/util/stringlist.hpp>

#include <ert/sched/sched_macros.hpp>

typedef struct sched_kw_untyped_struct sched_kw_untyped_type;

sched_kw_untyped_type * sched_kw_untyped_alloc(const stringlist_type * tokens , int * token_index , int rec_len);
sched_kw_untyped_type * sched_kw_untyped_alloc_empty(const char * , int rec_len);
void                    sched_kw_untyped_fprintf(const sched_kw_untyped_type *, FILE *);
void                    sched_kw_untyped_free(sched_kw_untyped_type * );
sched_kw_untyped_type * sched_kw_untyped_fread_alloc(FILE *);
void                    sched_kw_untyped_fwrite(const sched_kw_untyped_type * , FILE *);
void                    sched_kw_untyped_add_line(sched_kw_untyped_type *  , const char *, bool);
void                    sched_kw_untyped_add_tokens( sched_kw_untyped_type * kw , const stringlist_type * tokens);
char **                 sched_kw_untyped_iget_entries_alloc(const sched_kw_untyped_type *, int, int *);

/*******************************************************************/

KW_FREE_HEADER(untyped)
KW_FPRINTF_HEADER(untyped)
KW_COPYC_HEADER(untyped)

#ifdef __cplusplus
}
#endif
#endif

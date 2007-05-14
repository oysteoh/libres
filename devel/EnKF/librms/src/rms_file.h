#ifndef __RMS_FILE_H__
#define __RMS_FILE_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <rms_tag.h>


typedef struct rms_file_struct   rms_file_type;

FILE               * rms_file_fopen(const rms_file_type *, bool );
void                 rms_file_set_filename(rms_file_type * , const char * , bool);
rms_file_type      * rms_file_alloc  	(const char *, bool );
void                 rms_file_fread  	(rms_file_type *);
void                 rms_file_fwrite 	(const rms_file_type * , const char *);
void                 rms_file_printf 	(const rms_file_type * , FILE *);
void                 rms_file_free   	(rms_file_type *);
void                 rms_file_free_data (rms_file_type *);
rms_tag_type       * rms_file_get_tag   (const rms_file_type *, const char *, const char *, const char *);
void                 rms_file_assert_dimensions(const rms_file_type *, int , int , int );
rms_tag_type       * rms_file_fread_alloc_tag(rms_file_type * , const char *, const char *, const char *);
rms_tagkey_type    * rms_file_fread_alloc_data_tagkey(rms_file_type * , const char *, const char *, const char *);
void 		     rms_file_complete_fwrite(const rms_file_type * , FILE * );
void 		     rms_file_init_fwrite(const rms_file_type * , const char * , FILE *);

#endif

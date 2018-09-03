#ifndef __PBSTRU_MAPHEADERENTRY_H__
#define __PBSTRU_MAPHEADERENTRY_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"

/*
message MapheaderEntry {
  string key = 1;
  bytes value = 2;
}
*/

typedef struct _st_mapheaderentry {
    BOOL has_key;
    ps_string var_key;  /* tag:1 */
    BOOL has_value;
    ps_bytes var_value;  /* tag:2 */
} st_mapheaderentry;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_MapheaderEntry(st_mapheaderentry *msg);
/* destruct msg */
void destru_message_MapheaderEntry(st_mapheaderentry *msg);

/* clear and reuse msg */
void clear_message_MapheaderEntry(st_mapheaderentry *msg);
size_t encode_message_MapheaderEntry(const st_mapheaderentry* const msg, BYTE* const buf);
BOOL decode_message_MapheaderEntry(BYTE* const buf, const size_t buf_len, st_mapheaderentry* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

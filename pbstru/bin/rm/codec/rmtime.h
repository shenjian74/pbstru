#ifndef __PBSTRU_RMTIME_H__
#define __PBSTRU_RMTIME_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"

/*
message RmTime {
  required uint32 type = 1;
  required bytes key = 2;
  required uint32 time = 3;
  optional uint32 data = 4;
}
*/

typedef struct _st_rmtime {
    DWORD var_type;  /* tag:1 */
    ps_bytes var_key;  /* tag:2 */
    DWORD var_time;  /* tag:3 */
    BOOL has_data;
    DWORD var_data;  /* tag:4 */
} st_rmtime;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RmTime(st_rmtime *msg);
/* destruct msg */
void destru_message_RmTime(st_rmtime *msg);

/* clear and reuse msg */
void clear_message_RmTime(st_rmtime *msg);
size_t encode_message_RmTime(const st_rmtime* const msg, BYTE* const buf);
BOOL decode_message_RmTime(BYTE* const buf, const size_t buf_len, st_rmtime* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

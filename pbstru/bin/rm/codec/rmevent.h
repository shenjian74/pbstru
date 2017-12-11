#ifndef __PBSTRU_RMEVENT_H__
#define __PBSTRU_RMEVENT_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"

/*
message RmEvent {
  required uint32 type = 1;
  required bytes value = 2;
  optional uint32 data = 3;
}
*/

typedef struct _st_rmevent {
    DWORD var_type;  /* tag:1 */
    ps_bytes var_value;  /* tag:2 */
    BOOL has_data;
    DWORD var_data;  /* tag:3 */
} st_rmevent;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RmEvent(st_rmevent *msg);
/* destruct msg */
void destru_message_RmEvent(st_rmevent *msg);

/* clear and reuse msg */
void clear_message_RmEvent(st_rmevent *msg);
size_t encode_message_RmEvent(const st_rmevent* const msg, BYTE* const buf);
BOOL decode_message_RmEvent(BYTE* const buf, const size_t buf_len, st_rmevent* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

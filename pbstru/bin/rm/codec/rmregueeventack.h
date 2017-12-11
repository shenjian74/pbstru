#ifndef __PBSTRU_RMREGUEEVENTACK_H__
#define __PBSTRU_RMREGUEEVENTACK_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "rmevent.h"

/*
message RMRegUeEventAck {
  required uint32 result = 1;
  optional bytes primary = 2;
  repeated .rm.RmEvent event = 3;
}
*/
#define PBSTRU_MAX_EVENT_IN_RMREGUEEVENTACK 10

typedef struct _st_event_in_rmregueeventack_rmevent_list {
    size_t count;
    st_rmevent item[PBSTRU_MAX_EVENT_IN_RMREGUEEVENTACK];  /* tag:3 type:message */
} st_event_in_rmregueeventack_rmevent_list;

typedef struct _st_rmregueeventack {
    DWORD var_result;  /* tag:1 */
    BOOL has_primary;
    ps_bytes var_primary;  /* tag:2 */
    st_event_in_rmregueeventack_rmevent_list var_event;  /* tag:3 */
} st_rmregueeventack;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RMRegUeEventAck(st_rmregueeventack *msg);
/* destruct msg */
void destru_message_RMRegUeEventAck(st_rmregueeventack *msg);

/* clear and reuse msg */
void clear_message_RMRegUeEventAck(st_rmregueeventack *msg);
size_t encode_message_RMRegUeEventAck(const st_rmregueeventack* const msg, BYTE* const buf);
BOOL decode_message_RMRegUeEventAck(BYTE* const buf, const size_t buf_len, st_rmregueeventack* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

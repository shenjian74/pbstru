#ifndef __PBSTRU_RMDEREGUEEVENTREQ_H__
#define __PBSTRU_RMDEREGUEEVENTREQ_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "rmevent.h"

/*
message RMDeregUeEventReq {
  required bytes primary = 1;
  repeated .rm.RmEvent event = 2;
  required uint32 srvtype = 3;
}
*/
#define PBSTRU_MAX_EVENT_IN_RMDEREGUEEVENTREQ 10

typedef struct _st_event_in_rmderegueeventreq_rmevent_list {
    size_t count;
    st_rmevent item[PBSTRU_MAX_EVENT_IN_RMDEREGUEEVENTREQ];  /* tag:2 type:message */
} st_event_in_rmderegueeventreq_rmevent_list;

typedef struct _st_rmderegueeventreq {
    ps_bytes var_primary;  /* tag:1 */
    st_event_in_rmderegueeventreq_rmevent_list var_event;  /* tag:2 */
    DWORD var_srvtype;  /* tag:3 */
} st_rmderegueeventreq;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RMDeregUeEventReq(st_rmderegueeventreq *msg);
/* destruct msg */
void destru_message_RMDeregUeEventReq(st_rmderegueeventreq *msg);

/* clear and reuse msg */
void clear_message_RMDeregUeEventReq(st_rmderegueeventreq *msg);
size_t encode_message_RMDeregUeEventReq(const st_rmderegueeventreq* const msg, BYTE* const buf);
BOOL decode_message_RMDeregUeEventReq(BYTE* const buf, const size_t buf_len, st_rmderegueeventreq* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

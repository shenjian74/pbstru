#ifndef __PBSTRU_RMREGUEEVENTREQ_H__
#define __PBSTRU_RMREGUEEVENTREQ_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "rmevent.h"

/*
message RMRegUeEventReq {
  required bytes primary = 1;
  repeated .rm.RmEvent event = 2;
  repeated uint32 srvtype = 3;
}
*/
#define PBSTRU_MAX_EVENT_IN_RMREGUEEVENTREQ 10
#define PBSTRU_MAX_SRVTYPE_IN_RMREGUEEVENTREQ 10

typedef struct _st_event_in_rmregueeventreq_rmevent_list {
    size_t count;
    st_rmevent item[PBSTRU_MAX_EVENT_IN_RMREGUEEVENTREQ];  /* tag:2 type:message */
} st_event_in_rmregueeventreq_rmevent_list;

typedef struct _st_srvtype_in_rmregueeventreq_uint32_list {
    size_t count;
    DWORD item[PBSTRU_MAX_SRVTYPE_IN_RMREGUEEVENTREQ];  /* tag:3 type:uint32 */
} st_srvtype_in_rmregueeventreq_uint32_list;

typedef struct _st_rmregueeventreq {
    ps_bytes var_primary;  /* tag:1 */
    st_event_in_rmregueeventreq_rmevent_list var_event;  /* tag:2 */
    st_srvtype_in_rmregueeventreq_uint32_list var_srvtype;  /* tag:3 */
} st_rmregueeventreq;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RMRegUeEventReq(st_rmregueeventreq *msg);
/* destruct msg */
void destru_message_RMRegUeEventReq(st_rmregueeventreq *msg);

/* clear and reuse msg */
void clear_message_RMRegUeEventReq(st_rmregueeventreq *msg);
size_t encode_message_RMRegUeEventReq(const st_rmregueeventreq* const msg, BYTE* const buf);
BOOL decode_message_RMRegUeEventReq(BYTE* const buf, const size_t buf_len, st_rmregueeventreq* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

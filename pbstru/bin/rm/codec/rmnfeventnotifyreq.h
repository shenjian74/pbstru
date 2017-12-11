#ifndef __PBSTRU_RMNFEVENTNOTIFYREQ_H__
#define __PBSTRU_RMNFEVENTNOTIFYREQ_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "rmevent.h"

/*
message RMNfEventNotifyReq {
  required .rm.RmEvent event = 1;
  required uint32 srvtype = 3;
}
*/

typedef struct _st_rmnfeventnotifyreq {
    st_rmevent var_event;  /* tag:1 */
    DWORD var_srvtype;  /* tag:3 */
} st_rmnfeventnotifyreq;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RMNfEventNotifyReq(st_rmnfeventnotifyreq *msg);
/* destruct msg */
void destru_message_RMNfEventNotifyReq(st_rmnfeventnotifyreq *msg);

/* clear and reuse msg */
void clear_message_RMNfEventNotifyReq(st_rmnfeventnotifyreq *msg);
size_t encode_message_RMNfEventNotifyReq(const st_rmnfeventnotifyreq* const msg, BYTE* const buf);
BOOL decode_message_RMNfEventNotifyReq(BYTE* const buf, const size_t buf_len, st_rmnfeventnotifyreq* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

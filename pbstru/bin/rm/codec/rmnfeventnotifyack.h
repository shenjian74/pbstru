#ifndef __PBSTRU_RMNFEVENTNOTIFYACK_H__
#define __PBSTRU_RMNFEVENTNOTIFYACK_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "rmevent.h"

/*
message RMNfEventNotifyAck {
  required uint32 result = 1;
  optional .rm.RmEvent event = 2;
}
*/

typedef struct _st_rmnfeventnotifyack {
    DWORD var_result;  /* tag:1 */
    BOOL has_event;
    st_rmevent var_event;  /* tag:2 */
} st_rmnfeventnotifyack;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RMNfEventNotifyAck(st_rmnfeventnotifyack *msg);
/* destruct msg */
void destru_message_RMNfEventNotifyAck(st_rmnfeventnotifyack *msg);

/* clear and reuse msg */
void clear_message_RMNfEventNotifyAck(st_rmnfeventnotifyack *msg);
size_t encode_message_RMNfEventNotifyAck(const st_rmnfeventnotifyack* const msg, BYTE* const buf);
BOOL decode_message_RMNfEventNotifyAck(BYTE* const buf, const size_t buf_len, st_rmnfeventnotifyack* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

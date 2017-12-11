#ifndef __PBSTRU_RMNOTIFYNFGENERAL_H__
#define __PBSTRU_RMNOTIFYNFGENERAL_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "rmnotify.h"

/*
message RMNotifyNfGeneral {
  repeated .rm.RMNotify notify = 1;
}
*/
#define PBSTRU_MAX_NOTIFY_IN_RMNOTIFYNFGENERAL 10

typedef struct _st_notify_in_rmnotifynfgeneral_rmnotify_list {
    size_t count;
    st_rmnotify item[PBSTRU_MAX_NOTIFY_IN_RMNOTIFYNFGENERAL];  /* tag:1 type:message */
} st_notify_in_rmnotifynfgeneral_rmnotify_list;

typedef struct _st_rmnotifynfgeneral {
    st_notify_in_rmnotifynfgeneral_rmnotify_list var_notify;  /* tag:1 */
} st_rmnotifynfgeneral;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RMNotifyNfGeneral(st_rmnotifynfgeneral *msg);
/* destruct msg */
void destru_message_RMNotifyNfGeneral(st_rmnotifynfgeneral *msg);

/* clear and reuse msg */
void clear_message_RMNotifyNfGeneral(st_rmnotifynfgeneral *msg);
size_t encode_message_RMNotifyNfGeneral(const st_rmnotifynfgeneral* const msg, BYTE* const buf);
BOOL decode_message_RMNotifyNfGeneral(BYTE* const buf, const size_t buf_len, st_rmnotifynfgeneral* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

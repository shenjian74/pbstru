#ifndef __PBSTRU_RMNOTIFY_H__
#define __PBSTRU_RMNOTIFY_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "rmevent.h"
#include "rmtime.h"

/*
message RMNotify {
  required bytes primary = 1;
  repeated .rm.RmTime time = 2;
  repeated .rm.RmEvent event = 3;
}
*/
#define PBSTRU_MAX_TIME_IN_RMNOTIFY 10
#define PBSTRU_MAX_EVENT_IN_RMNOTIFY 10

typedef struct _st_time_in_rmnotify_rmtime_list {
    size_t count;
    st_rmtime item[PBSTRU_MAX_TIME_IN_RMNOTIFY];  /* tag:2 type:message */
} st_time_in_rmnotify_rmtime_list;

typedef struct _st_event_in_rmnotify_rmevent_list {
    size_t count;
    st_rmevent item[PBSTRU_MAX_EVENT_IN_RMNOTIFY];  /* tag:3 type:message */
} st_event_in_rmnotify_rmevent_list;

typedef struct _st_rmnotify {
    ps_bytes var_primary;  /* tag:1 */
    st_time_in_rmnotify_rmtime_list var_time;  /* tag:2 */
    st_event_in_rmnotify_rmevent_list var_event;  /* tag:3 */
} st_rmnotify;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RMNotify(st_rmnotify *msg);
/* destruct msg */
void destru_message_RMNotify(st_rmnotify *msg);

/* clear and reuse msg */
void clear_message_RMNotify(st_rmnotify *msg);
size_t encode_message_RMNotify(const st_rmnotify* const msg, BYTE* const buf);
BOOL decode_message_RMNotify(BYTE* const buf, const size_t buf_len, st_rmnotify* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

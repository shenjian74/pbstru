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
  optional .rm.RmTime time = 2;
  optional .rm.RmEvent event = 3;
}
*/

typedef struct _st_rmnotify {
    ps_bytes var_primary;  /* tag:1 */
    BOOL has_time;
    st_rmtime var_time;  /* tag:2 */
    BOOL has_event;
    st_rmevent var_event;  /* tag:3 */
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

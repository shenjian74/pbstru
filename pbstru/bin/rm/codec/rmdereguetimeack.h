#ifndef __PBSTRU_RMDEREGUETIMEACK_H__
#define __PBSTRU_RMDEREGUETIMEACK_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "rmtime.h"

/*
message RMDeregUeTimeAck {
  required uint32 result = 1;
  optional bytes primary = 2;
  repeated .rm.RmTime time = 3;
}
*/
#define PBSTRU_MAX_TIME_IN_RMDEREGUETIMEACK 10

typedef struct _st_time_in_rmdereguetimeack_rmtime_list {
    size_t count;
    st_rmtime item[PBSTRU_MAX_TIME_IN_RMDEREGUETIMEACK];  /* tag:3 type:message */
} st_time_in_rmdereguetimeack_rmtime_list;

typedef struct _st_rmdereguetimeack {
    DWORD var_result;  /* tag:1 */
    BOOL has_primary;
    ps_bytes var_primary;  /* tag:2 */
    st_time_in_rmdereguetimeack_rmtime_list var_time;  /* tag:3 */
} st_rmdereguetimeack;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RMDeregUeTimeAck(st_rmdereguetimeack *msg);
/* destruct msg */
void destru_message_RMDeregUeTimeAck(st_rmdereguetimeack *msg);

/* clear and reuse msg */
void clear_message_RMDeregUeTimeAck(st_rmdereguetimeack *msg);
size_t encode_message_RMDeregUeTimeAck(const st_rmdereguetimeack* const msg, BYTE* const buf);
BOOL decode_message_RMDeregUeTimeAck(BYTE* const buf, const size_t buf_len, st_rmdereguetimeack* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

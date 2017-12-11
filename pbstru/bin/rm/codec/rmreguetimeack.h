#ifndef __PBSTRU_RMREGUETIMEACK_H__
#define __PBSTRU_RMREGUETIMEACK_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "rmtime.h"

/*
message RMRegUeTimeAck {
  required uint32 result = 1;
  optional bytes primary = 2;
  repeated .rm.RmTime time = 3;
}
*/
#define PBSTRU_MAX_TIME_IN_RMREGUETIMEACK 10

typedef struct _st_time_in_rmreguetimeack_rmtime_list {
    size_t count;
    st_rmtime item[PBSTRU_MAX_TIME_IN_RMREGUETIMEACK];  /* tag:3 type:message */
} st_time_in_rmreguetimeack_rmtime_list;

typedef struct _st_rmreguetimeack {
    DWORD var_result;  /* tag:1 */
    BOOL has_primary;
    ps_bytes var_primary;  /* tag:2 */
    st_time_in_rmreguetimeack_rmtime_list var_time;  /* tag:3 */
} st_rmreguetimeack;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RMRegUeTimeAck(st_rmreguetimeack *msg);
/* destruct msg */
void destru_message_RMRegUeTimeAck(st_rmreguetimeack *msg);

/* clear and reuse msg */
void clear_message_RMRegUeTimeAck(st_rmreguetimeack *msg);
size_t encode_message_RMRegUeTimeAck(const st_rmreguetimeack* const msg, BYTE* const buf);
BOOL decode_message_RMRegUeTimeAck(BYTE* const buf, const size_t buf_len, st_rmreguetimeack* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

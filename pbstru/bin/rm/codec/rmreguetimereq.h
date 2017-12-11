#ifndef __PBSTRU_RMREGUETIMEREQ_H__
#define __PBSTRU_RMREGUETIMEREQ_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "rmtime.h"

/*
message RMRegUeTimeReq {
  required bytes primary = 1;
  repeated .rm.RmTime time = 2;
  required uint32 srvtype = 3;
}
*/
#define PBSTRU_MAX_TIME_IN_RMREGUETIMEREQ 10

typedef struct _st_time_in_rmreguetimereq_rmtime_list {
    size_t count;
    st_rmtime item[PBSTRU_MAX_TIME_IN_RMREGUETIMEREQ];  /* tag:2 type:message */
} st_time_in_rmreguetimereq_rmtime_list;

typedef struct _st_rmreguetimereq {
    ps_bytes var_primary;  /* tag:1 */
    st_time_in_rmreguetimereq_rmtime_list var_time;  /* tag:2 */
    DWORD var_srvtype;  /* tag:3 */
} st_rmreguetimereq;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RMRegUeTimeReq(st_rmreguetimereq *msg);
/* destruct msg */
void destru_message_RMRegUeTimeReq(st_rmreguetimereq *msg);

/* clear and reuse msg */
void clear_message_RMRegUeTimeReq(st_rmreguetimereq *msg);
size_t encode_message_RMRegUeTimeReq(const st_rmreguetimereq* const msg, BYTE* const buf);
BOOL decode_message_RMRegUeTimeReq(BYTE* const buf, const size_t buf_len, st_rmreguetimereq* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

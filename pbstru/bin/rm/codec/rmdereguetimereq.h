#ifndef __PBSTRU_RMDEREGUETIMEREQ_H__
#define __PBSTRU_RMDEREGUETIMEREQ_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "rmtime.h"

/*
message RMDeregUeTimeReq {
  required bytes primary = 1;
  repeated .rm.RmTime time = 2;
  repeated uint32 srvtype = 3;
}
*/
#define PBSTRU_MAX_TIME_IN_RMDEREGUETIMEREQ 10
#define PBSTRU_MAX_SRVTYPE_IN_RMDEREGUETIMEREQ 10

typedef struct _st_time_in_rmdereguetimereq_rmtime_list {
    size_t count;
    st_rmtime item[PBSTRU_MAX_TIME_IN_RMDEREGUETIMEREQ];  /* tag:2 type:message */
} st_time_in_rmdereguetimereq_rmtime_list;

typedef struct _st_srvtype_in_rmdereguetimereq_uint32_list {
    size_t count;
    DWORD item[PBSTRU_MAX_SRVTYPE_IN_RMDEREGUETIMEREQ];  /* tag:3 type:uint32 */
} st_srvtype_in_rmdereguetimereq_uint32_list;

typedef struct _st_rmdereguetimereq {
    ps_bytes var_primary;  /* tag:1 */
    st_time_in_rmdereguetimereq_rmtime_list var_time;  /* tag:2 */
    st_srvtype_in_rmdereguetimereq_uint32_list var_srvtype;  /* tag:3 */
} st_rmdereguetimereq;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RMDeregUeTimeReq(st_rmdereguetimereq *msg);
/* destruct msg */
void destru_message_RMDeregUeTimeReq(st_rmdereguetimereq *msg);

/* clear and reuse msg */
void clear_message_RMDeregUeTimeReq(st_rmdereguetimereq *msg);
size_t encode_message_RMDeregUeTimeReq(const st_rmdereguetimereq* const msg, BYTE* const buf);
BOOL decode_message_RMDeregUeTimeReq(BYTE* const buf, const size_t buf_len, st_rmdereguetimereq* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

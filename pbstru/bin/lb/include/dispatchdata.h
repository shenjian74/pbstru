#ifndef __PBSTRU_DISPATCHDATA_H__
#define __PBSTRU_DISPATCHDATA_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"

/*
message DispatchData {
  bytes soName = 1;
  bytes soData = 2;
}
*/

typedef struct _st_dispatchdata {
    BOOL has_soName;
    ps_bytes var_soName;  /* tag:1 */
    BOOL has_soData;
    ps_bytes var_soData;  /* tag:2 */
} st_dispatchdata;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_DispatchData(st_dispatchdata *msg);
/* destruct msg */
void destru_message_DispatchData(st_dispatchdata *msg);

/* clear and reuse msg */
void clear_message_DispatchData(st_dispatchdata *msg);
size_t encode_message_DispatchData(const st_dispatchdata* const msg, BYTE* const buf);
BOOL decode_message_DispatchData(BYTE* const buf, const size_t buf_len, st_dispatchdata* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

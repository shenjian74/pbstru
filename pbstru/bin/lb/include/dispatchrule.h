#ifndef __PBSTRU_DISPATCHRULE_H__
#define __PBSTRU_DISPATCHRULE_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "dispatchdata.h"

/*
message DispatchRule {
  bytes serviceName = 1;
  .http2proxy.DispatchData serviceData = 2;
}
*/

typedef struct _st_dispatchrule {
    BOOL has_serviceName;
    ps_bytes var_serviceName;  /* tag:1 */
    BOOL has_serviceData;
    st_dispatchdata var_serviceData;  /* tag:2 */
} st_dispatchrule;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_DispatchRule(st_dispatchrule *msg);
/* destruct msg */
void destru_message_DispatchRule(st_dispatchrule *msg);

/* clear and reuse msg */
void clear_message_DispatchRule(st_dispatchrule *msg);
size_t encode_message_DispatchRule(const st_dispatchrule* const msg, BYTE* const buf);
BOOL decode_message_DispatchRule(BYTE* const buf, const size_t buf_len, st_dispatchrule* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

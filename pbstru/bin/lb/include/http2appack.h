#ifndef __PBSTRU_HTTP2APPACK_H__
#define __PBSTRU_HTTP2APPACK_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "app2httpreq.h"
#include "http2transparam.h"

/*
message Http2AppAck {
  .http2proxy.Http2TransParam transParam = 1;
  .http2proxy.App2HttpReq req = 2;
}
*/

typedef struct _st_http2appack {
    BOOL has_transParam;
    st_http2transparam var_transParam;  /* tag:1 */
    BOOL has_req;
    st_app2httpreq var_req;  /* tag:2 */
} st_http2appack;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_Http2AppAck(st_http2appack *msg);
/* destruct msg */
void destru_message_Http2AppAck(st_http2appack *msg);

/* clear and reuse msg */
void clear_message_Http2AppAck(st_http2appack *msg);
size_t encode_message_Http2AppAck(const st_http2appack* const msg, BYTE* const buf);
BOOL decode_message_Http2AppAck(BYTE* const buf, const size_t buf_len, st_http2appack* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */


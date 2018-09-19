#ifndef __PBSTRU_HTTP2TRANSPARAM_H__
#define __PBSTRU_HTTP2TRANSPARAM_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "http2ipaddr.h"

/*
message Http2TransParam {
  .http2proxy.Http2IpAddr local = 1;
  .http2proxy.Http2IpAddr remote = 2;
  uint32 streamId = 3;
}
*/

typedef struct _st_http2transparam {
    BOOL has_local;
    st_http2ipaddr var_local;  /* tag:1 */
    BOOL has_remote;
    st_http2ipaddr var_remote;  /* tag:2 */
    BOOL has_streamId;
    DWORD var_streamId;  /* tag:3 */
} st_http2transparam;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_Http2TransParam(st_http2transparam *msg);
/* destruct msg */
void destru_message_Http2TransParam(st_http2transparam *msg);

/* clear and reuse msg */
void clear_message_Http2TransParam(st_http2transparam *msg);
size_t encode_message_Http2TransParam(const st_http2transparam* const msg, BYTE* const buf);
BOOL decode_message_Http2TransParam(BYTE* const buf, const size_t buf_len, st_http2transparam* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */


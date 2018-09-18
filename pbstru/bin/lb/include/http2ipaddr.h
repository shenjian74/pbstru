#ifndef __PBSTRU_HTTP2IPADDR_H__
#define __PBSTRU_HTTP2IPADDR_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"

/*
message Http2IpAddr {
  bytes ip = 1;
  uint32 port = 2;
}
*/

typedef struct _st_http2ipaddr {
    BOOL has_ip;
    ps_bytes var_ip;  /* tag:1 */
    BOOL has_port;
    DWORD var_port;  /* tag:2 */
} st_http2ipaddr;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_Http2IpAddr(st_http2ipaddr *msg);
/* destruct msg */
void destru_message_Http2IpAddr(st_http2ipaddr *msg);

/* clear and reuse msg */
void clear_message_Http2IpAddr(st_http2ipaddr *msg);
size_t encode_message_Http2IpAddr(const st_http2ipaddr* const msg, BYTE* const buf);
BOOL decode_message_Http2IpAddr(BYTE* const buf, const size_t buf_len, st_http2ipaddr* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */


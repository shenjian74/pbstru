#ifndef __PBSTRU_HTTP2APPREQ_H__
#define __PBSTRU_HTTP2APPREQ_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "http2jid.h"
#include "http2transparam.h"
#include "mapheaderentry.h"

/*
message Http2AppReq {
  bytes url = 1;
  bytes method = 2;
  bytes data = 3;
  uint64 sequence = 4;
  bytes accept = 5;
  bytes contentType = 6;
  .http2proxy.Http2TransParam transParam = 7;
  repeated .http2proxy.MapheaderEntry header = 8;
  .http2proxy.Http2Jid httpJid = 9;
}
*/
#define PBSTRU_MAX_HEADER_IN_HTTP2APPREQ 10

typedef struct _st_header_in_http2appreq_mapheaderentry_list {
    size_t count;
    st_mapheaderentry item[PBSTRU_MAX_HEADER_IN_HTTP2APPREQ];  /* tag:8 type:message */
} st_header_in_http2appreq_mapheaderentry_list;

typedef struct _st_http2appreq {
    BOOL has_url;
    ps_bytes var_url;  /* tag:1 */
    BOOL has_method;
    ps_bytes var_method;  /* tag:2 */
    BOOL has_data;
    ps_bytes var_data;  /* tag:3 */
    BOOL has_sequence;
    WORD64 var_sequence;  /* tag:4 */
    BOOL has_accept;
    ps_bytes var_accept;  /* tag:5 */
    BOOL has_contentType;
    ps_bytes var_contentType;  /* tag:6 */
    BOOL has_transParam;
    st_http2transparam var_transParam;  /* tag:7 */
    st_header_in_http2appreq_mapheaderentry_list var_header;  /* tag:8 */
    BOOL has_httpJid;
    st_http2jid var_httpJid;  /* tag:9 */
} st_http2appreq;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_Http2AppReq(st_http2appreq *msg);
/* destruct msg */
void destru_message_Http2AppReq(st_http2appreq *msg);

/* clear and reuse msg */
void clear_message_Http2AppReq(st_http2appreq *msg);
size_t encode_message_Http2AppReq(const st_http2appreq* const msg, BYTE* const buf);
BOOL decode_message_Http2AppReq(BYTE* const buf, const size_t buf_len, st_http2appreq* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

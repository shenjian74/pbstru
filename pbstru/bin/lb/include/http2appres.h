#ifndef __PBSTRU_HTTP2APPRES_H__
#define __PBSTRU_HTTP2APPRES_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "http2transparam.h"
#include "mapheaderentry.h"

/*
message Http2AppRes {
  bytes url = 1;
  bytes method = 2;
  bytes data = 3;
  uint32 returnCode = 4;
  bytes sessionID = 5;
  uint32 responseCode = 6;
  bytes contentType = 7;
  bytes location = 8;
  .http2proxy.Http2TransParam transParam = 9;
  repeated .http2proxy.MapheaderEntry header = 10;
  .http2proxy.ReqType reqType = 21;
}
*/
#define PBSTRU_MAX_HEADER_IN_HTTP2APPRES 10

#ifndef ENUM_REQTYPE_DEFINED
#define ENUM_REQTYPE_DEFINED
typedef enum {
    NORMAL_REQ_M = 0,
    SUBSCRIBE_REQ_M = 1
} enum_ReqType;
#endif

typedef struct _st_header_in_http2appres_mapheaderentry_list {
    size_t count;
    st_mapheaderentry item[PBSTRU_MAX_HEADER_IN_HTTP2APPRES];  /* tag:10 type:message */
} st_header_in_http2appres_mapheaderentry_list;

typedef struct _st_http2appres {
    BOOL has_url;
    ps_bytes var_url;  /* tag:1 */
    BOOL has_method;
    ps_bytes var_method;  /* tag:2 */
    BOOL has_data;
    ps_bytes var_data;  /* tag:3 */
    BOOL has_returnCode;
    DWORD var_returnCode;  /* tag:4 */
    BOOL has_sessionID;
    ps_bytes var_sessionID;  /* tag:5 */
    BOOL has_responseCode;
    DWORD var_responseCode;  /* tag:6 */
    BOOL has_contentType;
    ps_bytes var_contentType;  /* tag:7 */
    BOOL has_location;
    ps_bytes var_location;  /* tag:8 */
    BOOL has_transParam;
    st_http2transparam var_transParam;  /* tag:9 */
    st_header_in_http2appres_mapheaderentry_list var_header;  /* tag:10 */
    BOOL has_reqType;
    enum_ReqType var_reqType;  /* tag:21 */
} st_http2appres;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_Http2AppRes(st_http2appres *msg);
/* destruct msg */
void destru_message_Http2AppRes(st_http2appres *msg);

/* clear and reuse msg */
void clear_message_Http2AppRes(st_http2appres *msg);
size_t encode_message_Http2AppRes(const st_http2appres* const msg, BYTE* const buf);
BOOL decode_message_Http2AppRes(BYTE* const buf, const size_t buf_len, st_http2appres* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

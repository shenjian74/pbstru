#ifndef __PBSTRU_APP2HTTPREQ_H__
#define __PBSTRU_APP2HTTPREQ_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "headerentry.h"
#include "http2appinfo.h"

/*
message App2HttpReq {
  bytes url = 1;
  bytes method = 2;
  bytes data = 3;
  bytes instance = 4;
  bytes sessionID = 5;
  bytes accept = 6;
  bytes contentType = 7;
  bool traced = 8;
  map<string, bytes> header = 9;
  .http2proxy.Http2AppInfo appInfo = 10;
  uint32 linkID = 11;
  bytes userid = 12;
  .http2proxy.ReqType reqType = 21;
}
*/
#define PBSTRU_MAX_HEADER_IN_APP2HTTPREQ 10

#ifndef ENUM_REQTYPE_DEFINED
#define ENUM_REQTYPE_DEFINED
typedef enum {
    NORMAL_REQ_M = 0,
    SUBSCRIBE_REQ_M = 1
} enum_ReqType;
#endif

typedef struct _st_header_in_app2httpreq_headerentry_list {
    size_t count;
    st_headerentry item[PBSTRU_MAX_HEADER_IN_APP2HTTPREQ];  /* tag:9 type:message */
} st_header_in_app2httpreq_headerentry_list;

typedef struct _st_app2httpreq {
    BOOL has_url;
    ps_bytes var_url;  /* tag:1 */
    BOOL has_method;
    ps_bytes var_method;  /* tag:2 */
    BOOL has_data;
    ps_bytes var_data;  /* tag:3 */
    BOOL has_instance;
    ps_bytes var_instance;  /* tag:4 */
    BOOL has_sessionID;
    ps_bytes var_sessionID;  /* tag:5 */
    BOOL has_accept;
    ps_bytes var_accept;  /* tag:6 */
    BOOL has_contentType;
    ps_bytes var_contentType;  /* tag:7 */
    BOOL has_traced;
    BOOL var_traced;  /* tag:8 */
    st_header_in_app2httpreq_headerentry_list var_header;  /* tag:9 */
    BOOL has_appInfo;
    st_http2appinfo var_appInfo;  /* tag:10 */
    BOOL has_linkID;
    DWORD var_linkID;  /* tag:11 */
    BOOL has_userid;
    ps_bytes var_userid;  /* tag:12 */
    BOOL has_reqType;
    enum_ReqType var_reqType;  /* tag:21 */
} st_app2httpreq;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_App2HttpReq(st_app2httpreq *msg);
/* destruct msg */
void destru_message_App2HttpReq(st_app2httpreq *msg);

/* clear and reuse msg */
void clear_message_App2HttpReq(st_app2httpreq *msg);
size_t encode_message_App2HttpReq(const st_app2httpreq* const msg, BYTE* const buf);
BOOL decode_message_App2HttpReq(BYTE* const buf, const size_t buf_len, st_app2httpreq* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

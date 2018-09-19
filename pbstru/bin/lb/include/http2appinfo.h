#ifndef __PBSTRU_HTTP2APPINFO_H__
#define __PBSTRU_HTTP2APPINFO_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "http2jid.h"

/*
message Http2AppInfo {
  bytes serviceAbility = 1;
  bytes serviceTypeName = 2;
  uint32 serviceInst = 3;
  bytes scTypeName = 4;
  uint32 userGroupId = 5;
  .http2proxy.Http2Jid jid = 6;
}
*/

typedef struct _st_http2appinfo {
    BOOL has_serviceAbility;
    ps_bytes var_serviceAbility;  /* tag:1 */
    BOOL has_serviceTypeName;
    ps_bytes var_serviceTypeName;  /* tag:2 */
    BOOL has_serviceInst;
    DWORD var_serviceInst;  /* tag:3 */
    BOOL has_scTypeName;
    ps_bytes var_scTypeName;  /* tag:4 */
    BOOL has_userGroupId;
    DWORD var_userGroupId;  /* tag:5 */
    BOOL has_jid;
    st_http2jid var_jid;  /* tag:6 */
} st_http2appinfo;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_Http2AppInfo(st_http2appinfo *msg);
/* destruct msg */
void destru_message_Http2AppInfo(st_http2appinfo *msg);

/* clear and reuse msg */
void clear_message_Http2AppInfo(st_http2appinfo *msg);
size_t encode_message_Http2AppInfo(const st_http2appinfo* const msg, BYTE* const buf);
BOOL decode_message_Http2AppInfo(BYTE* const buf, const size_t buf_len, st_http2appinfo* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */


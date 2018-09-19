#ifndef __PBSTRU_HTTP2JID_H__
#define __PBSTRU_HTTP2JID_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"

/*
message Http2Jid {
  uint32 jno = 1;
  uint32 comId = 2;
}
*/

typedef struct _st_http2jid {
    BOOL has_jno;
    DWORD var_jno;  /* tag:1 */
    BOOL has_comId;
    DWORD var_comId;  /* tag:2 */
} st_http2jid;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_Http2Jid(st_http2jid *msg);
/* destruct msg */
void destru_message_Http2Jid(st_http2jid *msg);

/* clear and reuse msg */
void clear_message_Http2Jid(st_http2jid *msg);
size_t encode_message_Http2Jid(const st_http2jid* const msg, BYTE* const buf);
BOOL decode_message_Http2Jid(BYTE* const buf, const size_t buf_len, st_http2jid* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */


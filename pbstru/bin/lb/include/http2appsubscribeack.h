#ifndef __PBSTRU_HTTP2APPSUBSCRIBEACK_H__
#define __PBSTRU_HTTP2APPSUBSCRIBEACK_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "loadruleresult.h"

/*
message Http2AppSubscribeAck {
  uint32 sequence = 1;
  uint32 errorCode = 2;
  repeated .http2proxy.LoadRuleResult results = 3;
}
*/

typedef struct _st_results_in_http2appsubscribeack_loadruleresult_list {
    struct _st_results_in_http2appsubscribeack_loadruleresult_list *next;
    st_loadruleresult value;  /* tag:3 type:message */
} st_results_in_http2appsubscribeack_loadruleresult_list;

typedef struct _st_http2appsubscribeack {
    BOOL has_sequence;
    DWORD var_sequence;  /* tag:1 */
    BOOL has_errorCode;
    DWORD var_errorCode;  /* tag:2 */
    st_results_in_http2appsubscribeack_loadruleresult_list *var_results;  /* tag:3 链表头指针 */
    st_results_in_http2appsubscribeack_loadruleresult_list *var_results_tail;  /* 链表尾指针 */
} st_http2appsubscribeack;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_Http2AppSubscribeAck(st_http2appsubscribeack *msg);
/* destruct msg */
void destru_message_Http2AppSubscribeAck(st_http2appsubscribeack *msg);

/* clear and reuse msg */
void clear_message_Http2AppSubscribeAck(st_http2appsubscribeack *msg);
size_t encode_message_Http2AppSubscribeAck(const st_http2appsubscribeack* const msg, BYTE* const buf);
BOOL decode_message_Http2AppSubscribeAck(BYTE* const buf, const size_t buf_len, st_http2appsubscribeack* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

#ifndef __PBSTRU_APP2HTTPSUBSCRIBE_H__
#define __PBSTRU_APP2HTTPSUBSCRIBE_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "discoversc.h"
#include "dispatchrule.h"
#include "http2appinfo.h"

/*
message App2HttpSubscribe {
  uint32 sequence = 1;
  .http2proxy.DiscoverSC scInfo = 2;
  .http2proxy.Http2AppInfo appInfo = 3;
  repeated .http2proxy.DispatchRule rules = 4;
}
*/
#define PBSTRU_MAX_RULES_IN_APP2HTTPSUBSCRIBE 10

typedef struct _st_rules_in_app2httpsubscribe_dispatchrule_list {
    size_t count;
    st_dispatchrule item[PBSTRU_MAX_RULES_IN_APP2HTTPSUBSCRIBE];  /* tag:4 type:message */
} st_rules_in_app2httpsubscribe_dispatchrule_list;

typedef struct _st_app2httpsubscribe {
    BOOL has_sequence;
    DWORD var_sequence;  /* tag:1 */
    BOOL has_scInfo;
    st_discoversc var_scInfo;  /* tag:2 */
    BOOL has_appInfo;
    st_http2appinfo var_appInfo;  /* tag:3 */
    st_rules_in_app2httpsubscribe_dispatchrule_list var_rules;  /* tag:4 */
} st_app2httpsubscribe;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_App2HttpSubscribe(st_app2httpsubscribe *msg);
/* destruct msg */
void destru_message_App2HttpSubscribe(st_app2httpsubscribe *msg);

/* clear and reuse msg */
void clear_message_App2HttpSubscribe(st_app2httpsubscribe *msg);
size_t encode_message_App2HttpSubscribe(const st_app2httpsubscribe* const msg, BYTE* const buf);
BOOL decode_message_App2HttpSubscribe(BYTE* const buf, const size_t buf_len, st_app2httpsubscribe* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

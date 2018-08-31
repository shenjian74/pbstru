#ifndef __PBSTRU_APP2HTTPRES_H__
#define __PBSTRU_APP2HTTPRES_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"
#include "headerentry.h"
#include "http2transparam.h"

/*
message App2HttpRes {
  uint64 sequence = 1;
  uint32 responseCode = 2;
  bytes data = 3;
  bytes contentType = 4;
  bytes location = 5;
  .http2proxy.Http2TransParam transParam = 6;
  map<string, bytes> header = 7;
}
*/
#define PBSTRU_MAX_HEADER_IN_APP2HTTPRES 10

typedef struct _st_header_in_app2httpres_headerentry_list {
    size_t count;
    st_headerentry item[PBSTRU_MAX_HEADER_IN_APP2HTTPRES];  /* tag:7 type:message */
} st_header_in_app2httpres_headerentry_list;

typedef struct _st_app2httpres {
    BOOL has_sequence;
    WORD64 var_sequence;  /* tag:1 */
    BOOL has_responseCode;
    DWORD var_responseCode;  /* tag:2 */
    BOOL has_data;
    ps_bytes var_data;  /* tag:3 */
    BOOL has_contentType;
    ps_bytes var_contentType;  /* tag:4 */
    BOOL has_location;
    ps_bytes var_location;  /* tag:5 */
    BOOL has_transParam;
    st_http2transparam var_transParam;  /* tag:6 */
    st_header_in_app2httpres_headerentry_list var_header;  /* tag:7 */
} st_app2httpres;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_App2HttpRes(st_app2httpres *msg);
/* destruct msg */
void destru_message_App2HttpRes(st_app2httpres *msg);

/* clear and reuse msg */
void clear_message_App2HttpRes(st_app2httpres *msg);
size_t encode_message_App2HttpRes(const st_app2httpres* const msg, BYTE* const buf);
BOOL decode_message_App2HttpRes(BYTE* const buf, const size_t buf_len, st_app2httpres* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

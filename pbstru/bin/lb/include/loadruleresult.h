#ifndef __PBSTRU_LOADRULERESULT_H__
#define __PBSTRU_LOADRULERESULT_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"

/*
message LoadRuleResult {
  bytes serviceName = 1;
  uint32 errorCode = 2;
}
*/

typedef struct _st_loadruleresult {
    BOOL has_serviceName;
    ps_bytes var_serviceName;  /* tag:1 */
    BOOL has_errorCode;
    DWORD var_errorCode;  /* tag:2 */
} st_loadruleresult;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_LoadRuleResult(st_loadruleresult *msg);
/* destruct msg */
void destru_message_LoadRuleResult(st_loadruleresult *msg);

/* clear and reuse msg */
void clear_message_LoadRuleResult(st_loadruleresult *msg);
size_t encode_message_LoadRuleResult(const st_loadruleresult* const msg, BYTE* const buf);
BOOL decode_message_LoadRuleResult(BYTE* const buf, const size_t buf_len, st_loadruleresult* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

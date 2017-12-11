#ifndef __PBSTRU_RMREGNFACK_H__
#define __PBSTRU_RMREGNFACK_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"

/*
message RMRegNfAck {
  required uint32 result = 1;
  optional uint32 tenantid = 2;
  optional uint32 nftype = 3;
}
*/

typedef struct _st_rmregnfack {
    DWORD var_result;  /* tag:1 */
    BOOL has_tenantid;
    DWORD var_tenantid;  /* tag:2 */
    BOOL has_nftype;
    DWORD var_nftype;  /* tag:3 */
} st_rmregnfack;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RMRegNfAck(st_rmregnfack *msg);
/* destruct msg */
void destru_message_RMRegNfAck(st_rmregnfack *msg);

/* clear and reuse msg */
void clear_message_RMRegNfAck(st_rmregnfack *msg);
size_t encode_message_RMRegNfAck(const st_rmregnfack* const msg, BYTE* const buf);
BOOL decode_message_RMRegNfAck(BYTE* const buf, const size_t buf_len, st_rmregnfack* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

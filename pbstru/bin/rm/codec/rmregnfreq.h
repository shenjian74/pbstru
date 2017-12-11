#ifndef __PBSTRU_RMREGNFREQ_H__
#define __PBSTRU_RMREGNFREQ_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"

/*
message RMRegNfReq {
  required uint32 tenantid = 1;
  required uint32 nftype = 2;
}
*/

typedef struct _st_rmregnfreq {
    DWORD var_tenantid;  /* tag:1 */
    DWORD var_nftype;  /* tag:2 */
} st_rmregnfreq;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_RMRegNfReq(st_rmregnfreq *msg);
/* destruct msg */
void destru_message_RMRegNfReq(st_rmregnfreq *msg);

/* clear and reuse msg */
void clear_message_RMRegNfReq(st_rmregnfreq *msg);
size_t encode_message_RMRegNfReq(const st_rmregnfreq* const msg, BYTE* const buf);
BOOL decode_message_RMRegNfReq(BYTE* const buf, const size_t buf_len, st_rmregnfreq* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

#ifndef __PBSTRU_DISCOVERSC_H__
#define __PBSTRU_DISCOVERSC_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "pbstru_comm.h"

/*
message DiscoverSC {
  uint32 serviceTypeID = 1;
  uint32 serviceInst = 2;
  uint32 scTypeID = 3;
}
*/

typedef struct _st_discoversc {
    BOOL has_serviceTypeID;
    DWORD var_serviceTypeID;  /* tag:1 */
    BOOL has_serviceInst;
    DWORD var_serviceInst;  /* tag:2 */
    BOOL has_scTypeID;
    DWORD var_scTypeID;  /* tag:3 */
} st_discoversc;

/* construct msg when first use, call clear_message_*() internaly. */
void constru_message_DiscoverSC(st_discoversc *msg);
/* destruct msg */
void destru_message_DiscoverSC(st_discoversc *msg);

/* clear and reuse msg */
void clear_message_DiscoverSC(st_discoversc *msg);
size_t encode_message_DiscoverSC(const st_discoversc* const msg, BYTE* const buf);
BOOL decode_message_DiscoverSC(BYTE* const buf, const size_t buf_len, st_discoversc* const msg);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */

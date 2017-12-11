#include "rmregnfreq.h"

/* lint -save -e701 -e647 */

void constru_message_RMRegNfReq(st_rmregnfreq *var_RMRegNfReq){
}

void destru_message_RMRegNfReq(st_rmregnfreq* var_RMRegNfReq){
    clear_message_RMRegNfReq(var_RMRegNfReq);
}

void clear_message_RMRegNfReq(st_rmregnfreq* var_RMRegNfReq){
    var_RMRegNfReq->var_tenantid = 0;
    var_RMRegNfReq->var_nftype = 0;
}

size_t encode_message_RMRegNfReq(const st_rmregnfreq* var_RMRegNfReq, BYTE* buf){
    size_t offset = 0;

    /* tag:1 type:uint32 */
    encode_tag_byte(buf, 1, WIRE_TYPE_VARINT, &offset);
    encode_varint(var_RMRegNfReq->var_tenantid, buf, &offset);

    /* tag:2 type:uint32 */
    encode_tag_byte(buf, 2, WIRE_TYPE_VARINT, &offset);
    encode_varint(var_RMRegNfReq->var_nftype, buf, &offset);
    return offset;
}

BOOL decode_message_RMRegNfReq(BYTE* buf, const size_t buf_len, st_rmregnfreq* var_RMRegNfReq){
	size_t offset = 0;
    WORD field_num;
    BYTE wire_type;

    clear_message_RMRegNfReq(var_RMRegNfReq);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:uint32 */
        case 1:
            decode_varint(buf + offset, &(var_RMRegNfReq->var_tenantid), &offset);
            break;
        /* type:uint32 */
        case 2:
            decode_varint(buf + offset, &(var_RMRegNfReq->var_nftype), &offset);
            break;
        default:
        	deal_unknown_field(wire_type, buf+offset, &offset);
         	break;
        }
    }
    return TRUE;
}

/* lint -restore */
/* end of file */

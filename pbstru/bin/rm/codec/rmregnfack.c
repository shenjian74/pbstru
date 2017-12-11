#include "rmregnfack.h"

/* lint -save -e701 -e647 */

void constru_message_RMRegNfAck(st_rmregnfack *var_RMRegNfAck){
    var_RMRegNfAck->has_tenantid = FALSE;
    var_RMRegNfAck->has_nftype = FALSE;
}

void destru_message_RMRegNfAck(st_rmregnfack* var_RMRegNfAck){
    clear_message_RMRegNfAck(var_RMRegNfAck);
}

void clear_message_RMRegNfAck(st_rmregnfack* var_RMRegNfAck){
    var_RMRegNfAck->var_result = 0;
    if(TRUE == var_RMRegNfAck->has_tenantid){
        var_RMRegNfAck->var_tenantid = 0;
    }
    var_RMRegNfAck->has_tenantid = FALSE;
    if(TRUE == var_RMRegNfAck->has_nftype){
        var_RMRegNfAck->var_nftype = 0;
    }
    var_RMRegNfAck->has_nftype = FALSE;
}

size_t encode_message_RMRegNfAck(const st_rmregnfack* var_RMRegNfAck, BYTE* buf){
    size_t offset = 0;

    /* tag:1 type:uint32 */
    encode_tag_byte(buf, 1, WIRE_TYPE_VARINT, &offset);
    encode_varint(var_RMRegNfAck->var_result, buf, &offset);

    if(var_RMRegNfAck->has_tenantid){
        /* tag:2 type:uint32 */
        encode_tag_byte(buf, 2, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_RMRegNfAck->var_tenantid, buf, &offset);
    }

    if(var_RMRegNfAck->has_nftype){
        /* tag:3 type:uint32 */
        encode_tag_byte(buf, 3, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_RMRegNfAck->var_nftype, buf, &offset);
    }
    return offset;
}

BOOL decode_message_RMRegNfAck(BYTE* buf, const size_t buf_len, st_rmregnfack* var_RMRegNfAck){
	size_t offset = 0;
    WORD field_num;
    BYTE wire_type;

    clear_message_RMRegNfAck(var_RMRegNfAck);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:uint32 */
        case 1:
            decode_varint(buf + offset, &(var_RMRegNfAck->var_result), &offset);
            break;
        /* type:uint32 */
        case 2:
            decode_varint(buf + offset, &(var_RMRegNfAck->var_tenantid), &offset);
            var_RMRegNfAck->has_tenantid = TRUE;
            break;
        /* type:uint32 */
        case 3:
            decode_varint(buf + offset, &(var_RMRegNfAck->var_nftype), &offset);
            var_RMRegNfAck->has_nftype = TRUE;
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

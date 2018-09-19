#include "discoversc.h"

/* lint -save -e701 -e647 */

void constru_message_DiscoverSC(st_discoversc *var_DiscoverSC){
    var_DiscoverSC->has_serviceTypeID = FALSE;
    var_DiscoverSC->has_serviceInst = FALSE;
    var_DiscoverSC->has_scTypeID = FALSE;
}

void destru_message_DiscoverSC(st_discoversc* var_DiscoverSC){
    clear_message_DiscoverSC(var_DiscoverSC);
}

void clear_message_DiscoverSC(st_discoversc* var_DiscoverSC){
    if(TRUE == var_DiscoverSC->has_serviceTypeID){
        var_DiscoverSC->var_serviceTypeID = 0;
    }
    var_DiscoverSC->has_serviceTypeID = FALSE;
    if(TRUE == var_DiscoverSC->has_serviceInst){
        var_DiscoverSC->var_serviceInst = 0;
    }
    var_DiscoverSC->has_serviceInst = FALSE;
    if(TRUE == var_DiscoverSC->has_scTypeID){
        var_DiscoverSC->var_scTypeID = 0;
    }
    var_DiscoverSC->has_scTypeID = FALSE;
}

size_t encode_message_DiscoverSC(const st_discoversc* var_DiscoverSC, BYTE* buf){
    size_t offset = 0;

    if(var_DiscoverSC->has_serviceTypeID){
        /* tag:1 type:uint32 */
        encode_tag_byte(buf, 1, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_DiscoverSC->var_serviceTypeID, buf, &offset);
    }

    if(var_DiscoverSC->has_serviceInst){
        /* tag:2 type:uint32 */
        encode_tag_byte(buf, 2, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_DiscoverSC->var_serviceInst, buf, &offset);
    }

    if(var_DiscoverSC->has_scTypeID){
        /* tag:3 type:uint32 */
        encode_tag_byte(buf, 3, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_DiscoverSC->var_scTypeID, buf, &offset);
    }
    return offset;
}

BOOL decode_message_DiscoverSC(BYTE* buf, const size_t buf_len, st_discoversc* var_DiscoverSC){
	size_t offset = 0;
    WORD field_num;
    BYTE wire_type;

    clear_message_DiscoverSC(var_DiscoverSC);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:uint32 */
        case 1:
            decode_varint(buf + offset, &(var_DiscoverSC->var_serviceTypeID), &offset);
            var_DiscoverSC->has_serviceTypeID = TRUE;
            break;
        /* type:uint32 */
        case 2:
            decode_varint(buf + offset, &(var_DiscoverSC->var_serviceInst), &offset);
            var_DiscoverSC->has_serviceInst = TRUE;
            break;
        /* type:uint32 */
        case 3:
            decode_varint(buf + offset, &(var_DiscoverSC->var_scTypeID), &offset);
            var_DiscoverSC->has_scTypeID = TRUE;
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


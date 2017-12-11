#include "rmevent.h"

/* lint -save -e701 -e647 */

void constru_message_RmEvent(st_rmevent *var_RmEvent){
    var_RmEvent->has_data = FALSE;
}

void destru_message_RmEvent(st_rmevent* var_RmEvent){
    clear_message_RmEvent(var_RmEvent);
}

void clear_message_RmEvent(st_rmevent* var_RmEvent){
    var_RmEvent->var_type = 0;
    var_RmEvent->var_value.length = 0;
    var_RmEvent->var_value.data = NULL;
    if(TRUE == var_RmEvent->has_data){
        var_RmEvent->var_data = 0;
    }
    var_RmEvent->has_data = FALSE;
}

size_t encode_message_RmEvent(const st_rmevent* var_RmEvent, BYTE* buf){
    size_t offset = 0;

    /* tag:1 type:uint32 */
    encode_tag_byte(buf, 1, WIRE_TYPE_VARINT, &offset);
    encode_varint(var_RmEvent->var_type, buf, &offset);

    /* tag:2 type:bytes */
    encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
    encode_varint(var_RmEvent->var_value.length, buf, &offset);
    if (NULL != buf) {
        memcpy(buf + offset, var_RmEvent->var_value.data, var_RmEvent->var_value.length);
    }
    offset += var_RmEvent->var_value.length;

    if(var_RmEvent->has_data){
        /* tag:3 type:uint32 */
        encode_tag_byte(buf, 3, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_RmEvent->var_data, buf, &offset);
    }
    return offset;
}

BOOL decode_message_RmEvent(BYTE* buf, const size_t buf_len, st_rmevent* var_RmEvent){
	size_t offset = 0;
    WORD field_num;
    BYTE wire_type;

    clear_message_RmEvent(var_RmEvent);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:uint32 */
        case 1:
            decode_varint(buf + offset, &(var_RmEvent->var_type), &offset);
            break;
        /* type:bytes */
        case 2:
            decode_varint(buf + offset, &(var_RmEvent->var_value.length), &offset);
            var_RmEvent->var_value.data = buf + offset;
            offset += var_RmEvent->var_value.length;
            break;
        /* type:uint32 */
        case 3:
            decode_varint(buf + offset, &(var_RmEvent->var_data), &offset);
            var_RmEvent->has_data = TRUE;
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

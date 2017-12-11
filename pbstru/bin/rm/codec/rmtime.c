#include "rmtime.h"

/* lint -save -e701 -e647 */

void constru_message_RmTime(st_rmtime *var_RmTime){
    var_RmTime->has_data = FALSE;
}

void destru_message_RmTime(st_rmtime* var_RmTime){
    clear_message_RmTime(var_RmTime);
}

void clear_message_RmTime(st_rmtime* var_RmTime){
    var_RmTime->var_type = 0;
    var_RmTime->var_key.length = 0;
    var_RmTime->var_key.data = NULL;
    var_RmTime->var_time = 0;
    if(TRUE == var_RmTime->has_data){
        var_RmTime->var_data = 0;
    }
    var_RmTime->has_data = FALSE;
}

size_t encode_message_RmTime(const st_rmtime* var_RmTime, BYTE* buf){
    size_t offset = 0;

    /* tag:1 type:uint32 */
    encode_tag_byte(buf, 1, WIRE_TYPE_VARINT, &offset);
    encode_varint(var_RmTime->var_type, buf, &offset);

    /* tag:2 type:bytes */
    encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
    encode_varint(var_RmTime->var_key.length, buf, &offset);
    if (NULL != buf) {
        memcpy(buf + offset, var_RmTime->var_key.data, var_RmTime->var_key.length);
    }
    offset += var_RmTime->var_key.length;

    /* tag:3 type:uint32 */
    encode_tag_byte(buf, 3, WIRE_TYPE_VARINT, &offset);
    encode_varint(var_RmTime->var_time, buf, &offset);

    if(var_RmTime->has_data){
        /* tag:4 type:uint32 */
        encode_tag_byte(buf, 4, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_RmTime->var_data, buf, &offset);
    }
    return offset;
}

BOOL decode_message_RmTime(BYTE* buf, const size_t buf_len, st_rmtime* var_RmTime){
	size_t offset = 0;
    WORD field_num;
    BYTE wire_type;

    clear_message_RmTime(var_RmTime);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:uint32 */
        case 1:
            decode_varint(buf + offset, &(var_RmTime->var_type), &offset);
            break;
        /* type:bytes */
        case 2:
            decode_varint(buf + offset, &(var_RmTime->var_key.length), &offset);
            var_RmTime->var_key.data = buf + offset;
            offset += var_RmTime->var_key.length;
            break;
        /* type:uint32 */
        case 3:
            decode_varint(buf + offset, &(var_RmTime->var_time), &offset);
            break;
        /* type:uint32 */
        case 4:
            decode_varint(buf + offset, &(var_RmTime->var_data), &offset);
            var_RmTime->has_data = TRUE;
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

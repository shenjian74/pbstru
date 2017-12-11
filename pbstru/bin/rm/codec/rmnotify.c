#include "rmnotify.h"

/* lint -save -e701 -e647 */

void constru_message_RMNotify(st_rmnotify *var_RMNotify){
        constru_message_RmTime(&(var_RMNotify->var_time));
    var_RMNotify->has_time = FALSE;
        constru_message_RmEvent(&(var_RMNotify->var_event));
    var_RMNotify->has_event = FALSE;
}

void destru_message_RMNotify(st_rmnotify* var_RMNotify){
    clear_message_RMNotify(var_RMNotify);
}

void clear_message_RMNotify(st_rmnotify* var_RMNotify){
    var_RMNotify->var_primary.length = 0;
    var_RMNotify->var_primary.data = NULL;
    if(TRUE == var_RMNotify->has_time){
        clear_message_RmTime(&(var_RMNotify->var_time));
    }
    var_RMNotify->has_time = FALSE;
    if(TRUE == var_RMNotify->has_event){
        clear_message_RmEvent(&(var_RMNotify->var_event));
    }
    var_RMNotify->has_event = FALSE;
}

size_t encode_message_RMNotify(const st_rmnotify* var_RMNotify, BYTE* buf){
    size_t encode_buf_len;
    size_t offset = 0;

    /* tag:1 type:bytes */
    encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
    encode_varint(var_RMNotify->var_primary.length, buf, &offset);
    if (NULL != buf) {
        memcpy(buf + offset, var_RMNotify->var_primary.data, var_RMNotify->var_primary.length);
    }
    offset += var_RMNotify->var_primary.length;

    if(var_RMNotify->has_time){
        /* tag:2 type:message */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_RmTime(&(var_RMNotify->var_time), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_RmTime(&(var_RMNotify->var_time), buf + offset);
        }
        offset += encode_buf_len;
    }

    if(var_RMNotify->has_event){
        /* tag:3 type:message */
        encode_tag_byte(buf, 3, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_RmEvent(&(var_RMNotify->var_event), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_RmEvent(&(var_RMNotify->var_event), buf + offset);
        }
        offset += encode_buf_len;
    }
    return offset;
}

BOOL decode_message_RMNotify(BYTE* buf, const size_t buf_len, st_rmnotify* var_RMNotify){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_RMNotify(var_RMNotify);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:bytes */
        case 1:
            decode_varint(buf + offset, &(var_RMNotify->var_primary.length), &offset);
            var_RMNotify->var_primary.data = buf + offset;
            offset += var_RMNotify->var_primary.length;
            break;
        /* type:message */
        case 2:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_RmTime(buf + offset, tmp_field_len, &(var_RMNotify->var_time));
            offset += tmp_field_len;
            var_RMNotify->has_time = TRUE;
            break;
        /* type:message */
        case 3:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_RmEvent(buf + offset, tmp_field_len, &(var_RMNotify->var_event));
            offset += tmp_field_len;
            var_RMNotify->has_event = TRUE;
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

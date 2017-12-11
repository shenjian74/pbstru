#include "rmnfeventnotifyack.h"

/* lint -save -e701 -e647 */

void constru_message_RMNfEventNotifyAck(st_rmnfeventnotifyack *var_RMNfEventNotifyAck){
        constru_message_RmEvent(&(var_RMNfEventNotifyAck->var_event));
    var_RMNfEventNotifyAck->has_event = FALSE;
}

void destru_message_RMNfEventNotifyAck(st_rmnfeventnotifyack* var_RMNfEventNotifyAck){
    clear_message_RMNfEventNotifyAck(var_RMNfEventNotifyAck);
}

void clear_message_RMNfEventNotifyAck(st_rmnfeventnotifyack* var_RMNfEventNotifyAck){
    var_RMNfEventNotifyAck->var_result = 0;
    if(TRUE == var_RMNfEventNotifyAck->has_event){
        clear_message_RmEvent(&(var_RMNfEventNotifyAck->var_event));
    }
    var_RMNfEventNotifyAck->has_event = FALSE;
}

size_t encode_message_RMNfEventNotifyAck(const st_rmnfeventnotifyack* var_RMNfEventNotifyAck, BYTE* buf){
    size_t encode_buf_len;
    size_t offset = 0;

    /* tag:1 type:uint32 */
    encode_tag_byte(buf, 1, WIRE_TYPE_VARINT, &offset);
    encode_varint(var_RMNfEventNotifyAck->var_result, buf, &offset);

    if(var_RMNfEventNotifyAck->has_event){
        /* tag:2 type:message */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_RmEvent(&(var_RMNfEventNotifyAck->var_event), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_RmEvent(&(var_RMNfEventNotifyAck->var_event), buf + offset);
        }
        offset += encode_buf_len;
    }
    return offset;
}

BOOL decode_message_RMNfEventNotifyAck(BYTE* buf, const size_t buf_len, st_rmnfeventnotifyack* var_RMNfEventNotifyAck){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_RMNfEventNotifyAck(var_RMNfEventNotifyAck);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:uint32 */
        case 1:
            decode_varint(buf + offset, &(var_RMNfEventNotifyAck->var_result), &offset);
            break;
        /* type:message */
        case 2:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_RmEvent(buf + offset, tmp_field_len, &(var_RMNfEventNotifyAck->var_event));
            offset += tmp_field_len;
            var_RMNfEventNotifyAck->has_event = TRUE;
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

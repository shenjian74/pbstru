#include "rmreguetimeack.h"

/* lint -save -e701 -e647 */

void constru_message_RMRegUeTimeAck(st_rmreguetimeack *var_RMRegUeTimeAck){
    size_t i = 0;
    var_RMRegUeTimeAck->has_primary = FALSE;
    for(i=0; i<PBSTRU_MAX_TIME_IN_RMREGUETIMEACK; ++i){
        constru_message_RmTime(&(var_RMRegUeTimeAck->var_time.item[i]));
    }
    var_RMRegUeTimeAck->var_time.count = 0;
}

void destru_message_RMRegUeTimeAck(st_rmreguetimeack* var_RMRegUeTimeAck){
    clear_message_RMRegUeTimeAck(var_RMRegUeTimeAck);
}

void clear_message_RMRegUeTimeAck(st_rmreguetimeack* var_RMRegUeTimeAck){
    size_t i = 0;
    var_RMRegUeTimeAck->var_result = 0;
    if(TRUE == var_RMRegUeTimeAck->has_primary){
        var_RMRegUeTimeAck->var_primary.length = 0;
        var_RMRegUeTimeAck->var_primary.data = NULL;
    }
    var_RMRegUeTimeAck->has_primary = FALSE;
    for(i=0; i<var_RMRegUeTimeAck->var_time.count; ++i){
        clear_message_RmTime(&(var_RMRegUeTimeAck->var_time.item[i]));
    }
    var_RMRegUeTimeAck->var_time.count = 0;
}

size_t encode_message_RMRegUeTimeAck(const st_rmreguetimeack* var_RMRegUeTimeAck, BYTE* buf){
    size_t encode_buf_len;
    size_t i;
    size_t offset = 0;

    /* tag:1 type:uint32 */
    encode_tag_byte(buf, 1, WIRE_TYPE_VARINT, &offset);
    encode_varint(var_RMRegUeTimeAck->var_result, buf, &offset);

    if(var_RMRegUeTimeAck->has_primary){
        /* tag:2 type:bytes */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_RMRegUeTimeAck->var_primary.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_RMRegUeTimeAck->var_primary.data, var_RMRegUeTimeAck->var_primary.length);
        }
        offset += var_RMRegUeTimeAck->var_primary.length;
    }

    for(i = 0; i < var_RMRegUeTimeAck->var_time.count; ++i){
        /* tag:3 type:message */
        encode_tag_byte(buf, 3, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_RmTime(&(var_RMRegUeTimeAck->var_time.item[i]), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_RmTime(&(var_RMRegUeTimeAck->var_time.item[i]), buf + offset);
        }
        offset += encode_buf_len;
    }
    return offset;
}

BOOL decode_message_RMRegUeTimeAck(BYTE* buf, const size_t buf_len, st_rmreguetimeack* var_RMRegUeTimeAck){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_RMRegUeTimeAck(var_RMRegUeTimeAck);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:uint32 */
        case 1:
            decode_varint(buf + offset, &(var_RMRegUeTimeAck->var_result), &offset);
            break;
        /* type:bytes */
        case 2:
            decode_varint(buf + offset, &(var_RMRegUeTimeAck->var_primary.length), &offset);
            var_RMRegUeTimeAck->var_primary.data = buf + offset;
            offset += var_RMRegUeTimeAck->var_primary.length;
            var_RMRegUeTimeAck->has_primary = TRUE;
            break;
        /* type:message */
        case 3:
            if(var_RMRegUeTimeAck->var_time.count >= PBSTRU_MAX_TIME_IN_RMREGUETIMEACK) {
                return FALSE;  /* Êý×é³¬ÏÞ */
            }
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_RmTime(buf + offset, tmp_field_len, &(var_RMRegUeTimeAck->var_time.item[var_RMRegUeTimeAck->var_time.count]));
            offset += tmp_field_len;
            var_RMRegUeTimeAck->var_time.count += 1;
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

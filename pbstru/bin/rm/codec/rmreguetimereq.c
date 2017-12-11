#include "rmreguetimereq.h"

/* lint -save -e701 -e647 */

void constru_message_RMRegUeTimeReq(st_rmreguetimereq *var_RMRegUeTimeReq){
    size_t i = 0;
    for(i=0; i<PBSTRU_MAX_TIME_IN_RMREGUETIMEREQ; ++i){
        constru_message_RmTime(&(var_RMRegUeTimeReq->var_time.item[i]));
    }
    var_RMRegUeTimeReq->var_time.count = 0;
}

void destru_message_RMRegUeTimeReq(st_rmreguetimereq* var_RMRegUeTimeReq){
    clear_message_RMRegUeTimeReq(var_RMRegUeTimeReq);
}

void clear_message_RMRegUeTimeReq(st_rmreguetimereq* var_RMRegUeTimeReq){
    size_t i = 0;
    var_RMRegUeTimeReq->var_primary.length = 0;
    var_RMRegUeTimeReq->var_primary.data = NULL;
    for(i=0; i<var_RMRegUeTimeReq->var_time.count; ++i){
        clear_message_RmTime(&(var_RMRegUeTimeReq->var_time.item[i]));
    }
    var_RMRegUeTimeReq->var_time.count = 0;
    var_RMRegUeTimeReq->var_srvtype = 0;
}

size_t encode_message_RMRegUeTimeReq(const st_rmreguetimereq* var_RMRegUeTimeReq, BYTE* buf){
    size_t encode_buf_len;
    size_t i;
    size_t offset = 0;

    /* tag:1 type:bytes */
    encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
    encode_varint(var_RMRegUeTimeReq->var_primary.length, buf, &offset);
    if (NULL != buf) {
        memcpy(buf + offset, var_RMRegUeTimeReq->var_primary.data, var_RMRegUeTimeReq->var_primary.length);
    }
    offset += var_RMRegUeTimeReq->var_primary.length;

    for(i = 0; i < var_RMRegUeTimeReq->var_time.count; ++i){
        /* tag:2 type:message */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_RmTime(&(var_RMRegUeTimeReq->var_time.item[i]), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_RmTime(&(var_RMRegUeTimeReq->var_time.item[i]), buf + offset);
        }
        offset += encode_buf_len;
    }

    /* tag:3 type:uint32 */
    encode_tag_byte(buf, 3, WIRE_TYPE_VARINT, &offset);
    encode_varint(var_RMRegUeTimeReq->var_srvtype, buf, &offset);
    return offset;
}

BOOL decode_message_RMRegUeTimeReq(BYTE* buf, const size_t buf_len, st_rmreguetimereq* var_RMRegUeTimeReq){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_RMRegUeTimeReq(var_RMRegUeTimeReq);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:bytes */
        case 1:
            decode_varint(buf + offset, &(var_RMRegUeTimeReq->var_primary.length), &offset);
            var_RMRegUeTimeReq->var_primary.data = buf + offset;
            offset += var_RMRegUeTimeReq->var_primary.length;
            break;
        /* type:message */
        case 2:
            if(var_RMRegUeTimeReq->var_time.count >= PBSTRU_MAX_TIME_IN_RMREGUETIMEREQ) {
                return FALSE;  /* Êý×é³¬ÏÞ */
            }
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_RmTime(buf + offset, tmp_field_len, &(var_RMRegUeTimeReq->var_time.item[var_RMRegUeTimeReq->var_time.count]));
            offset += tmp_field_len;
            var_RMRegUeTimeReq->var_time.count += 1;
            break;
        /* type:uint32 */
        case 3:
            decode_varint(buf + offset, &(var_RMRegUeTimeReq->var_srvtype), &offset);
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

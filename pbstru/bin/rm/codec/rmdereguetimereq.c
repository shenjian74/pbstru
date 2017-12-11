#include "rmdereguetimereq.h"

/* lint -save -e701 -e647 */

void constru_message_RMDeregUeTimeReq(st_rmdereguetimereq *var_RMDeregUeTimeReq){
    size_t i = 0;
    for(i=0; i<PBSTRU_MAX_TIME_IN_RMDEREGUETIMEREQ; ++i){
        constru_message_RmTime(&(var_RMDeregUeTimeReq->var_time.item[i]));
    }
    var_RMDeregUeTimeReq->var_time.count = 0;
}

void destru_message_RMDeregUeTimeReq(st_rmdereguetimereq* var_RMDeregUeTimeReq){
    clear_message_RMDeregUeTimeReq(var_RMDeregUeTimeReq);
}

void clear_message_RMDeregUeTimeReq(st_rmdereguetimereq* var_RMDeregUeTimeReq){
    size_t i = 0;
    var_RMDeregUeTimeReq->var_primary.length = 0;
    var_RMDeregUeTimeReq->var_primary.data = NULL;
    for(i=0; i<var_RMDeregUeTimeReq->var_time.count; ++i){
        clear_message_RmTime(&(var_RMDeregUeTimeReq->var_time.item[i]));
    }
    var_RMDeregUeTimeReq->var_time.count = 0;
    var_RMDeregUeTimeReq->var_srvtype = 0;
}

size_t encode_message_RMDeregUeTimeReq(const st_rmdereguetimereq* var_RMDeregUeTimeReq, BYTE* buf){
    size_t encode_buf_len;
    size_t i;
    size_t offset = 0;

    /* tag:1 type:bytes */
    encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
    encode_varint(var_RMDeregUeTimeReq->var_primary.length, buf, &offset);
    if (NULL != buf) {
        memcpy(buf + offset, var_RMDeregUeTimeReq->var_primary.data, var_RMDeregUeTimeReq->var_primary.length);
    }
    offset += var_RMDeregUeTimeReq->var_primary.length;

    for(i = 0; i < var_RMDeregUeTimeReq->var_time.count; ++i){
        /* tag:2 type:message */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_RmTime(&(var_RMDeregUeTimeReq->var_time.item[i]), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_RmTime(&(var_RMDeregUeTimeReq->var_time.item[i]), buf + offset);
        }
        offset += encode_buf_len;
    }

    /* tag:3 type:uint32 */
    encode_tag_byte(buf, 3, WIRE_TYPE_VARINT, &offset);
    encode_varint(var_RMDeregUeTimeReq->var_srvtype, buf, &offset);
    return offset;
}

BOOL decode_message_RMDeregUeTimeReq(BYTE* buf, const size_t buf_len, st_rmdereguetimereq* var_RMDeregUeTimeReq){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_RMDeregUeTimeReq(var_RMDeregUeTimeReq);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:bytes */
        case 1:
            decode_varint(buf + offset, &(var_RMDeregUeTimeReq->var_primary.length), &offset);
            var_RMDeregUeTimeReq->var_primary.data = buf + offset;
            offset += var_RMDeregUeTimeReq->var_primary.length;
            break;
        /* type:message */
        case 2:
            if(var_RMDeregUeTimeReq->var_time.count >= PBSTRU_MAX_TIME_IN_RMDEREGUETIMEREQ) {
                return FALSE;  /* Êý×é³¬ÏÞ */
            }
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_RmTime(buf + offset, tmp_field_len, &(var_RMDeregUeTimeReq->var_time.item[var_RMDeregUeTimeReq->var_time.count]));
            offset += tmp_field_len;
            var_RMDeregUeTimeReq->var_time.count += 1;
            break;
        /* type:uint32 */
        case 3:
            decode_varint(buf + offset, &(var_RMDeregUeTimeReq->var_srvtype), &offset);
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

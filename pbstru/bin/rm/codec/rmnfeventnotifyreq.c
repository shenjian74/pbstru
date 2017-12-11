#include "rmnfeventnotifyreq.h"

/* lint -save -e701 -e647 */

void constru_message_RMNfEventNotifyReq(st_rmnfeventnotifyreq *var_RMNfEventNotifyReq){
        constru_message_RmEvent(&(var_RMNfEventNotifyReq->var_event));
    var_RMNfEventNotifyReq->var_srvtype.count = 0;
}

void destru_message_RMNfEventNotifyReq(st_rmnfeventnotifyreq* var_RMNfEventNotifyReq){
    clear_message_RMNfEventNotifyReq(var_RMNfEventNotifyReq);
}

void clear_message_RMNfEventNotifyReq(st_rmnfeventnotifyreq* var_RMNfEventNotifyReq){
    clear_message_RmEvent(&(var_RMNfEventNotifyReq->var_event));
    var_RMNfEventNotifyReq->var_srvtype.count = 0;
}

size_t encode_message_RMNfEventNotifyReq(const st_rmnfeventnotifyreq* var_RMNfEventNotifyReq, BYTE* buf){
    size_t encode_buf_len;
    size_t i;
    size_t offset = 0;

    /* tag:1 type:message */
    encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
    encode_buf_len = encode_message_RmEvent(&(var_RMNfEventNotifyReq->var_event), NULL);
    encode_varint(encode_buf_len, buf, &offset);
    if(NULL != buf){
        encode_message_RmEvent(&(var_RMNfEventNotifyReq->var_event), buf + offset);
    }
    offset += encode_buf_len;

    for(i = 0; i < var_RMNfEventNotifyReq->var_srvtype.count; ++i){
        /* tag:3 type:uint32 */
        encode_tag_byte(buf, 3, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_RMNfEventNotifyReq->var_srvtype.item[i], buf, &offset);
    }
    return offset;
}

BOOL decode_message_RMNfEventNotifyReq(BYTE* buf, const size_t buf_len, st_rmnfeventnotifyreq* var_RMNfEventNotifyReq){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_RMNfEventNotifyReq(var_RMNfEventNotifyReq);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:message */
        case 1:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_RmEvent(buf + offset, tmp_field_len, &(var_RMNfEventNotifyReq->var_event));
            offset += tmp_field_len;
            break;
        /* type:uint32 */
        case 3:
            if(var_RMNfEventNotifyReq->var_srvtype.count >= PBSTRU_MAX_SRVTYPE_IN_RMNFEVENTNOTIFYREQ) {
                return FALSE;  /* Êý×é³¬ÏÞ */
            }
            decode_varint(buf + offset, &(var_RMNfEventNotifyReq->var_srvtype.item[var_RMNfEventNotifyReq->var_srvtype.count]), &offset);
            var_RMNfEventNotifyReq->var_srvtype.count += 1;
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

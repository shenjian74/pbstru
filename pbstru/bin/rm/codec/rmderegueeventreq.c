#include "rmderegueeventreq.h"

/* lint -save -e701 -e647 */

void constru_message_RMDeregUeEventReq(st_rmderegueeventreq *var_RMDeregUeEventReq){
    size_t i = 0;
    for(i=0; i<PBSTRU_MAX_EVENT_IN_RMDEREGUEEVENTREQ; ++i){
        constru_message_RmEvent(&(var_RMDeregUeEventReq->var_event.item[i]));
    }
    var_RMDeregUeEventReq->var_event.count = 0;
    var_RMDeregUeEventReq->var_srvtype.count = 0;
}

void destru_message_RMDeregUeEventReq(st_rmderegueeventreq* var_RMDeregUeEventReq){
    clear_message_RMDeregUeEventReq(var_RMDeregUeEventReq);
}

void clear_message_RMDeregUeEventReq(st_rmderegueeventreq* var_RMDeregUeEventReq){
    size_t i = 0;
    var_RMDeregUeEventReq->var_primary.length = 0;
    var_RMDeregUeEventReq->var_primary.data = NULL;
    for(i=0; i<var_RMDeregUeEventReq->var_event.count; ++i){
        clear_message_RmEvent(&(var_RMDeregUeEventReq->var_event.item[i]));
    }
    var_RMDeregUeEventReq->var_event.count = 0;
    var_RMDeregUeEventReq->var_srvtype.count = 0;
}

size_t encode_message_RMDeregUeEventReq(const st_rmderegueeventreq* var_RMDeregUeEventReq, BYTE* buf){
    size_t encode_buf_len;
    size_t i;
    size_t offset = 0;

    /* tag:1 type:bytes */
    encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
    encode_varint(var_RMDeregUeEventReq->var_primary.length, buf, &offset);
    if (NULL != buf) {
        memcpy(buf + offset, var_RMDeregUeEventReq->var_primary.data, var_RMDeregUeEventReq->var_primary.length);
    }
    offset += var_RMDeregUeEventReq->var_primary.length;

    for(i = 0; i < var_RMDeregUeEventReq->var_event.count; ++i){
        /* tag:2 type:message */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_RmEvent(&(var_RMDeregUeEventReq->var_event.item[i]), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_RmEvent(&(var_RMDeregUeEventReq->var_event.item[i]), buf + offset);
        }
        offset += encode_buf_len;
    }

    for(i = 0; i < var_RMDeregUeEventReq->var_srvtype.count; ++i){
        /* tag:3 type:uint32 */
        encode_tag_byte(buf, 3, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_RMDeregUeEventReq->var_srvtype.item[i], buf, &offset);
    }
    return offset;
}

BOOL decode_message_RMDeregUeEventReq(BYTE* buf, const size_t buf_len, st_rmderegueeventreq* var_RMDeregUeEventReq){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_RMDeregUeEventReq(var_RMDeregUeEventReq);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:bytes */
        case 1:
            decode_varint(buf + offset, &(var_RMDeregUeEventReq->var_primary.length), &offset);
            var_RMDeregUeEventReq->var_primary.data = buf + offset;
            offset += var_RMDeregUeEventReq->var_primary.length;
            break;
        /* type:message */
        case 2:
            if(var_RMDeregUeEventReq->var_event.count >= PBSTRU_MAX_EVENT_IN_RMDEREGUEEVENTREQ) {
                return FALSE;  /* 数组超限 */
            }
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_RmEvent(buf + offset, tmp_field_len, &(var_RMDeregUeEventReq->var_event.item[var_RMDeregUeEventReq->var_event.count]));
            offset += tmp_field_len;
            var_RMDeregUeEventReq->var_event.count += 1;
            break;
        /* type:uint32 */
        case 3:
            if(var_RMDeregUeEventReq->var_srvtype.count >= PBSTRU_MAX_SRVTYPE_IN_RMDEREGUEEVENTREQ) {
                return FALSE;  /* 数组超限 */
            }
            decode_varint(buf + offset, &(var_RMDeregUeEventReq->var_srvtype.item[var_RMDeregUeEventReq->var_srvtype.count]), &offset);
            var_RMDeregUeEventReq->var_srvtype.count += 1;
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

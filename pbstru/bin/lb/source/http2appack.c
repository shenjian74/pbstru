#include "http2appack.h"

/* lint -save -e701 -e647 */

void constru_message_Http2AppAck(st_http2appack *var_Http2AppAck){
        constru_message_Http2TransParam(&(var_Http2AppAck->var_transParam));
    var_Http2AppAck->has_transParam = FALSE;
        constru_message_App2HttpReq(&(var_Http2AppAck->var_req));
    var_Http2AppAck->has_req = FALSE;
}

void destru_message_Http2AppAck(st_http2appack* var_Http2AppAck){
    clear_message_Http2AppAck(var_Http2AppAck);
}

void clear_message_Http2AppAck(st_http2appack* var_Http2AppAck){
    if(TRUE == var_Http2AppAck->has_transParam){
        clear_message_Http2TransParam(&(var_Http2AppAck->var_transParam));
    }
    var_Http2AppAck->has_transParam = FALSE;
    if(TRUE == var_Http2AppAck->has_req){
        clear_message_App2HttpReq(&(var_Http2AppAck->var_req));
    }
    var_Http2AppAck->has_req = FALSE;
}

size_t encode_message_Http2AppAck(const st_http2appack* var_Http2AppAck, BYTE* buf){
    size_t encode_buf_len;
    size_t offset = 0;

    if(var_Http2AppAck->has_transParam){
        /* tag:1 type:message */
        encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_Http2TransParam(&(var_Http2AppAck->var_transParam), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_Http2TransParam(&(var_Http2AppAck->var_transParam), buf + offset);
        }
        offset += encode_buf_len;
    }

    if(var_Http2AppAck->has_req){
        /* tag:2 type:message */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_App2HttpReq(&(var_Http2AppAck->var_req), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_App2HttpReq(&(var_Http2AppAck->var_req), buf + offset);
        }
        offset += encode_buf_len;
    }
    return offset;
}

BOOL decode_message_Http2AppAck(BYTE* buf, const size_t buf_len, st_http2appack* var_Http2AppAck){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_Http2AppAck(var_Http2AppAck);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:message */
        case 1:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_Http2TransParam(buf + offset, tmp_field_len, &(var_Http2AppAck->var_transParam));
            offset += tmp_field_len;
            var_Http2AppAck->has_transParam = TRUE;
            break;
        /* type:message */
        case 2:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_App2HttpReq(buf + offset, tmp_field_len, &(var_Http2AppAck->var_req));
            offset += tmp_field_len;
            var_Http2AppAck->has_req = TRUE;
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


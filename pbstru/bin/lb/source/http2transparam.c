#include "http2transparam.h"

/* lint -save -e701 -e647 */

void constru_message_Http2TransParam(st_http2transparam *var_Http2TransParam){
        constru_message_Http2IpAddr(&(var_Http2TransParam->var_local));
    var_Http2TransParam->has_local = FALSE;
        constru_message_Http2IpAddr(&(var_Http2TransParam->var_remote));
    var_Http2TransParam->has_remote = FALSE;
    var_Http2TransParam->has_streamId = FALSE;
}

void destru_message_Http2TransParam(st_http2transparam* var_Http2TransParam){
    clear_message_Http2TransParam(var_Http2TransParam);
}

void clear_message_Http2TransParam(st_http2transparam* var_Http2TransParam){
    if(TRUE == var_Http2TransParam->has_local){
        clear_message_Http2IpAddr(&(var_Http2TransParam->var_local));
    }
    var_Http2TransParam->has_local = FALSE;
    if(TRUE == var_Http2TransParam->has_remote){
        clear_message_Http2IpAddr(&(var_Http2TransParam->var_remote));
    }
    var_Http2TransParam->has_remote = FALSE;
    if(TRUE == var_Http2TransParam->has_streamId){
        var_Http2TransParam->var_streamId = 0;
    }
    var_Http2TransParam->has_streamId = FALSE;
}

size_t encode_message_Http2TransParam(const st_http2transparam* var_Http2TransParam, BYTE* buf){
    size_t encode_buf_len;
    size_t offset = 0;

    if(var_Http2TransParam->has_local){
        /* tag:1 type:message */
        encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_Http2IpAddr(&(var_Http2TransParam->var_local), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_Http2IpAddr(&(var_Http2TransParam->var_local), buf + offset);
        }
        offset += encode_buf_len;
    }

    if(var_Http2TransParam->has_remote){
        /* tag:2 type:message */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_Http2IpAddr(&(var_Http2TransParam->var_remote), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_Http2IpAddr(&(var_Http2TransParam->var_remote), buf + offset);
        }
        offset += encode_buf_len;
    }

    if(var_Http2TransParam->has_streamId){
        /* tag:3 type:uint32 */
        encode_tag_byte(buf, 3, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_Http2TransParam->var_streamId, buf, &offset);
    }
    return offset;
}

BOOL decode_message_Http2TransParam(BYTE* buf, const size_t buf_len, st_http2transparam* var_Http2TransParam){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_Http2TransParam(var_Http2TransParam);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:message */
        case 1:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_Http2IpAddr(buf + offset, tmp_field_len, &(var_Http2TransParam->var_local));
            offset += tmp_field_len;
            var_Http2TransParam->has_local = TRUE;
            break;
        /* type:message */
        case 2:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_Http2IpAddr(buf + offset, tmp_field_len, &(var_Http2TransParam->var_remote));
            offset += tmp_field_len;
            var_Http2TransParam->has_remote = TRUE;
            break;
        /* type:uint32 */
        case 3:
            decode_varint(buf + offset, &(var_Http2TransParam->var_streamId), &offset);
            var_Http2TransParam->has_streamId = TRUE;
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


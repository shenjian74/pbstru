#include "http2ipaddr.h"

/* lint -save -e701 -e647 */

void constru_message_Http2IpAddr(st_http2ipaddr *var_Http2IpAddr){
    var_Http2IpAddr->has_ip = FALSE;
    var_Http2IpAddr->has_port = FALSE;
}

void destru_message_Http2IpAddr(st_http2ipaddr* var_Http2IpAddr){
    clear_message_Http2IpAddr(var_Http2IpAddr);
}

void clear_message_Http2IpAddr(st_http2ipaddr* var_Http2IpAddr){
    if(TRUE == var_Http2IpAddr->has_ip){
        var_Http2IpAddr->var_ip.length = 0;
        var_Http2IpAddr->var_ip.data = NULL;
    }
    var_Http2IpAddr->has_ip = FALSE;
    if(TRUE == var_Http2IpAddr->has_port){
        var_Http2IpAddr->var_port = 0;
    }
    var_Http2IpAddr->has_port = FALSE;
}

size_t encode_message_Http2IpAddr(const st_http2ipaddr* var_Http2IpAddr, BYTE* buf){
    size_t offset = 0;

    if(var_Http2IpAddr->has_ip){
        /* tag:1 type:bytes */
        encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2IpAddr->var_ip.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2IpAddr->var_ip.data, var_Http2IpAddr->var_ip.length);
        }
        offset += var_Http2IpAddr->var_ip.length;
    }

    if(var_Http2IpAddr->has_port){
        /* tag:2 type:uint32 */
        encode_tag_byte(buf, 2, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_Http2IpAddr->var_port, buf, &offset);
    }
    return offset;
}

BOOL decode_message_Http2IpAddr(BYTE* buf, const size_t buf_len, st_http2ipaddr* var_Http2IpAddr){
	size_t offset = 0;
    WORD field_num;
    BYTE wire_type;

    clear_message_Http2IpAddr(var_Http2IpAddr);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:bytes */
        case 1:
            decode_varint(buf + offset, &(var_Http2IpAddr->var_ip.length), &offset);
            var_Http2IpAddr->var_ip.data = buf + offset;
            offset += var_Http2IpAddr->var_ip.length;
            var_Http2IpAddr->has_ip = TRUE;
            break;
        /* type:uint32 */
        case 2:
            decode_varint(buf + offset, &(var_Http2IpAddr->var_port), &offset);
            var_Http2IpAddr->has_port = TRUE;
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


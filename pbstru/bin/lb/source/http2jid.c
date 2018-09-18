#include "http2jid.h"

/* lint -save -e701 -e647 */

void constru_message_Http2Jid(st_http2jid *var_Http2Jid){
    var_Http2Jid->has_jno = FALSE;
    var_Http2Jid->has_comId = FALSE;
}

void destru_message_Http2Jid(st_http2jid* var_Http2Jid){
    clear_message_Http2Jid(var_Http2Jid);
}

void clear_message_Http2Jid(st_http2jid* var_Http2Jid){
    if(TRUE == var_Http2Jid->has_jno){
        var_Http2Jid->var_jno = 0;
    }
    var_Http2Jid->has_jno = FALSE;
    if(TRUE == var_Http2Jid->has_comId){
        var_Http2Jid->var_comId = 0;
    }
    var_Http2Jid->has_comId = FALSE;
}

size_t encode_message_Http2Jid(const st_http2jid* var_Http2Jid, BYTE* buf){
    size_t offset = 0;

    if(var_Http2Jid->has_jno){
        /* tag:1 type:uint32 */
        encode_tag_byte(buf, 1, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_Http2Jid->var_jno, buf, &offset);
    }

    if(var_Http2Jid->has_comId){
        /* tag:2 type:uint32 */
        encode_tag_byte(buf, 2, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_Http2Jid->var_comId, buf, &offset);
    }
    return offset;
}

BOOL decode_message_Http2Jid(BYTE* buf, const size_t buf_len, st_http2jid* var_Http2Jid){
	size_t offset = 0;
    WORD field_num;
    BYTE wire_type;

    clear_message_Http2Jid(var_Http2Jid);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:uint32 */
        case 1:
            decode_varint(buf + offset, &(var_Http2Jid->var_jno), &offset);
            var_Http2Jid->has_jno = TRUE;
            break;
        /* type:uint32 */
        case 2:
            decode_varint(buf + offset, &(var_Http2Jid->var_comId), &offset);
            var_Http2Jid->has_comId = TRUE;
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


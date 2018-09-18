#include "http2appsubscribeack.h"

/* lint -save -e701 -e647 */

void constru_message_Http2AppSubscribeAck(st_http2appsubscribeack *var_Http2AppSubscribeAck){
    size_t i = 0;
    var_Http2AppSubscribeAck->has_sequence = FALSE;
    var_Http2AppSubscribeAck->has_errorCode = FALSE;
    for(i=0; i<PBSTRU_MAX_RESULTS_IN_HTTP2APPSUBSCRIBEACK; ++i){
        constru_message_LoadRuleResult(&(var_Http2AppSubscribeAck->var_results.item[i]));
    }
    var_Http2AppSubscribeAck->var_results.count = 0;
}

void destru_message_Http2AppSubscribeAck(st_http2appsubscribeack* var_Http2AppSubscribeAck){
    clear_message_Http2AppSubscribeAck(var_Http2AppSubscribeAck);
}

void clear_message_Http2AppSubscribeAck(st_http2appsubscribeack* var_Http2AppSubscribeAck){
    size_t i = 0;
    if(TRUE == var_Http2AppSubscribeAck->has_sequence){
        var_Http2AppSubscribeAck->var_sequence = 0;
    }
    var_Http2AppSubscribeAck->has_sequence = FALSE;
    if(TRUE == var_Http2AppSubscribeAck->has_errorCode){
        var_Http2AppSubscribeAck->var_errorCode = 0;
    }
    var_Http2AppSubscribeAck->has_errorCode = FALSE;
    for(i=0; i<var_Http2AppSubscribeAck->var_results.count; ++i){
        clear_message_LoadRuleResult(&(var_Http2AppSubscribeAck->var_results.item[i]));
    }
    var_Http2AppSubscribeAck->var_results.count = 0;
}

size_t encode_message_Http2AppSubscribeAck(const st_http2appsubscribeack* var_Http2AppSubscribeAck, BYTE* buf){
    size_t encode_buf_len;
    size_t i;
    size_t offset = 0;

    if(var_Http2AppSubscribeAck->has_sequence){
        /* tag:1 type:uint32 */
        encode_tag_byte(buf, 1, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_Http2AppSubscribeAck->var_sequence, buf, &offset);
    }

    if(var_Http2AppSubscribeAck->has_errorCode){
        /* tag:2 type:uint32 */
        encode_tag_byte(buf, 2, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_Http2AppSubscribeAck->var_errorCode, buf, &offset);
    }

    for(i = 0; i < var_Http2AppSubscribeAck->var_results.count; ++i){
        /* tag:3 type:message */
        encode_tag_byte(buf, 3, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_LoadRuleResult(&(var_Http2AppSubscribeAck->var_results.item[i]), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_LoadRuleResult(&(var_Http2AppSubscribeAck->var_results.item[i]), buf + offset);
        }
        offset += encode_buf_len;
    }
    return offset;
}

BOOL decode_message_Http2AppSubscribeAck(BYTE* buf, const size_t buf_len, st_http2appsubscribeack* var_Http2AppSubscribeAck){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_Http2AppSubscribeAck(var_Http2AppSubscribeAck);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:uint32 */
        case 1:
            decode_varint(buf + offset, &(var_Http2AppSubscribeAck->var_sequence), &offset);
            var_Http2AppSubscribeAck->has_sequence = TRUE;
            break;
        /* type:uint32 */
        case 2:
            decode_varint(buf + offset, &(var_Http2AppSubscribeAck->var_errorCode), &offset);
            var_Http2AppSubscribeAck->has_errorCode = TRUE;
            break;
        /* type:message */
        case 3:
            if(var_Http2AppSubscribeAck->var_results.count >= PBSTRU_MAX_RESULTS_IN_HTTP2APPSUBSCRIBEACK) {
                return FALSE;  /* Êý×é³¬ÏÞ */
            }
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_LoadRuleResult(buf + offset, tmp_field_len, &(var_Http2AppSubscribeAck->var_results.item[var_Http2AppSubscribeAck->var_results.count]));
            offset += tmp_field_len;
            var_Http2AppSubscribeAck->var_results.count += 1;
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


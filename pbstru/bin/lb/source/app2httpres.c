#include "app2httpres.h"

/* lint -save -e701 -e647 */

void constru_message_App2HttpRes(st_app2httpres *var_App2HttpRes){
    size_t i = 0;
    var_App2HttpRes->has_sequence = FALSE;
    var_App2HttpRes->has_responseCode = FALSE;
    var_App2HttpRes->has_data = FALSE;
    var_App2HttpRes->has_contentType = FALSE;
    var_App2HttpRes->has_location = FALSE;
        constru_message_Http2TransParam(&(var_App2HttpRes->var_transParam));
    var_App2HttpRes->has_transParam = FALSE;
    for(i=0; i<PBSTRU_MAX_HEADER_IN_APP2HTTPRES; ++i){
        constru_message_MapheaderEntry(&(var_App2HttpRes->var_header.item[i]));
    }
    var_App2HttpRes->var_header.count = 0;
}

void destru_message_App2HttpRes(st_app2httpres* var_App2HttpRes){
    clear_message_App2HttpRes(var_App2HttpRes);
}

void clear_message_App2HttpRes(st_app2httpres* var_App2HttpRes){
    size_t i = 0;
    if(TRUE == var_App2HttpRes->has_sequence){
        var_App2HttpRes->var_sequence = 0;
    }
    var_App2HttpRes->has_sequence = FALSE;
    if(TRUE == var_App2HttpRes->has_responseCode){
        var_App2HttpRes->var_responseCode = 0;
    }
    var_App2HttpRes->has_responseCode = FALSE;
    if(TRUE == var_App2HttpRes->has_data){
        var_App2HttpRes->var_data.length = 0;
        var_App2HttpRes->var_data.data = NULL;
    }
    var_App2HttpRes->has_data = FALSE;
    if(TRUE == var_App2HttpRes->has_contentType){
        var_App2HttpRes->var_contentType.length = 0;
        var_App2HttpRes->var_contentType.data = NULL;
    }
    var_App2HttpRes->has_contentType = FALSE;
    if(TRUE == var_App2HttpRes->has_location){
        var_App2HttpRes->var_location.length = 0;
        var_App2HttpRes->var_location.data = NULL;
    }
    var_App2HttpRes->has_location = FALSE;
    if(TRUE == var_App2HttpRes->has_transParam){
        clear_message_Http2TransParam(&(var_App2HttpRes->var_transParam));
    }
    var_App2HttpRes->has_transParam = FALSE;
    for(i=0; i<var_App2HttpRes->var_header.count; ++i){
        clear_message_MapheaderEntry(&(var_App2HttpRes->var_header.item[i]));
    }
    var_App2HttpRes->var_header.count = 0;
}

size_t encode_message_App2HttpRes(const st_app2httpres* var_App2HttpRes, BYTE* buf){
    size_t encode_buf_len;
    size_t i;
    size_t offset = 0;

    if(var_App2HttpRes->has_sequence){
        /* tag:1 type:uint64 */
        encode_tag_byte(buf, 1, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_App2HttpRes->var_sequence, buf, &offset);
    }

    if(var_App2HttpRes->has_responseCode){
        /* tag:2 type:uint32 */
        encode_tag_byte(buf, 2, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_App2HttpRes->var_responseCode, buf, &offset);
    }

    if(var_App2HttpRes->has_data){
        /* tag:3 type:bytes */
        encode_tag_byte(buf, 3, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_App2HttpRes->var_data.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_App2HttpRes->var_data.data, var_App2HttpRes->var_data.length);
        }
        offset += var_App2HttpRes->var_data.length;
    }

    if(var_App2HttpRes->has_contentType){
        /* tag:4 type:bytes */
        encode_tag_byte(buf, 4, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_App2HttpRes->var_contentType.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_App2HttpRes->var_contentType.data, var_App2HttpRes->var_contentType.length);
        }
        offset += var_App2HttpRes->var_contentType.length;
    }

    if(var_App2HttpRes->has_location){
        /* tag:5 type:bytes */
        encode_tag_byte(buf, 5, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_App2HttpRes->var_location.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_App2HttpRes->var_location.data, var_App2HttpRes->var_location.length);
        }
        offset += var_App2HttpRes->var_location.length;
    }

    if(var_App2HttpRes->has_transParam){
        /* tag:6 type:message */
        encode_tag_byte(buf, 6, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_Http2TransParam(&(var_App2HttpRes->var_transParam), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_Http2TransParam(&(var_App2HttpRes->var_transParam), buf + offset);
        }
        offset += encode_buf_len;
    }

    for(i = 0; i < var_App2HttpRes->var_header.count; ++i){
        /* tag:7 type:message */
        encode_tag_byte(buf, 7, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_MapheaderEntry(&(var_App2HttpRes->var_header.item[i]), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_MapheaderEntry(&(var_App2HttpRes->var_header.item[i]), buf + offset);
        }
        offset += encode_buf_len;
    }
    return offset;
}

BOOL decode_message_App2HttpRes(BYTE* buf, const size_t buf_len, st_app2httpres* var_App2HttpRes){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_App2HttpRes(var_App2HttpRes);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:uint64 */
        case 1:
            decode_varint(buf + offset, &(var_App2HttpRes->var_sequence), &offset);
            var_App2HttpRes->has_sequence = TRUE;
            break;
        /* type:uint32 */
        case 2:
            decode_varint(buf + offset, &(var_App2HttpRes->var_responseCode), &offset);
            var_App2HttpRes->has_responseCode = TRUE;
            break;
        /* type:bytes */
        case 3:
            decode_varint(buf + offset, &(var_App2HttpRes->var_data.length), &offset);
            var_App2HttpRes->var_data.data = buf + offset;
            offset += var_App2HttpRes->var_data.length;
            var_App2HttpRes->has_data = TRUE;
            break;
        /* type:bytes */
        case 4:
            decode_varint(buf + offset, &(var_App2HttpRes->var_contentType.length), &offset);
            var_App2HttpRes->var_contentType.data = buf + offset;
            offset += var_App2HttpRes->var_contentType.length;
            var_App2HttpRes->has_contentType = TRUE;
            break;
        /* type:bytes */
        case 5:
            decode_varint(buf + offset, &(var_App2HttpRes->var_location.length), &offset);
            var_App2HttpRes->var_location.data = buf + offset;
            offset += var_App2HttpRes->var_location.length;
            var_App2HttpRes->has_location = TRUE;
            break;
        /* type:message */
        case 6:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_Http2TransParam(buf + offset, tmp_field_len, &(var_App2HttpRes->var_transParam));
            offset += tmp_field_len;
            var_App2HttpRes->has_transParam = TRUE;
            break;
        /* type:message */
        case 7:
            if(var_App2HttpRes->var_header.count >= PBSTRU_MAX_HEADER_IN_APP2HTTPRES) {
                return FALSE;  /* Êý×é³¬ÏÞ */
            }
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_MapheaderEntry(buf + offset, tmp_field_len, &(var_App2HttpRes->var_header.item[var_App2HttpRes->var_header.count]));
            offset += tmp_field_len;
            var_App2HttpRes->var_header.count += 1;
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

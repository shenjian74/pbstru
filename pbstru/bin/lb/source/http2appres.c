#include "http2appres.h"

/* lint -save -e701 -e647 */

void constru_message_Http2AppRes(st_http2appres *var_Http2AppRes){
    size_t i = 0;
    var_Http2AppRes->has_url = FALSE;
    var_Http2AppRes->has_method = FALSE;
    var_Http2AppRes->has_data = FALSE;
    var_Http2AppRes->has_returnCode = FALSE;
    var_Http2AppRes->has_sessionID = FALSE;
    var_Http2AppRes->has_responseCode = FALSE;
    var_Http2AppRes->has_contentType = FALSE;
    var_Http2AppRes->has_location = FALSE;
        constru_message_Http2TransParam(&(var_Http2AppRes->var_transParam));
    var_Http2AppRes->has_transParam = FALSE;
    for(i=0; i<PBSTRU_MAX_HEADER_IN_HTTP2APPRES; ++i){
        constru_message_MapheaderEntry(&(var_Http2AppRes->var_header.item[i]));
    }
    var_Http2AppRes->var_header.count = 0;
    var_Http2AppRes->has_reqType = FALSE;
}

void destru_message_Http2AppRes(st_http2appres* var_Http2AppRes){
    clear_message_Http2AppRes(var_Http2AppRes);
}

void clear_message_Http2AppRes(st_http2appres* var_Http2AppRes){
    size_t i = 0;
    if(TRUE == var_Http2AppRes->has_url){
        var_Http2AppRes->var_url.length = 0;
        var_Http2AppRes->var_url.data = NULL;
    }
    var_Http2AppRes->has_url = FALSE;
    if(TRUE == var_Http2AppRes->has_method){
        var_Http2AppRes->var_method.length = 0;
        var_Http2AppRes->var_method.data = NULL;
    }
    var_Http2AppRes->has_method = FALSE;
    if(TRUE == var_Http2AppRes->has_data){
        var_Http2AppRes->var_data.length = 0;
        var_Http2AppRes->var_data.data = NULL;
    }
    var_Http2AppRes->has_data = FALSE;
    if(TRUE == var_Http2AppRes->has_returnCode){
        var_Http2AppRes->var_returnCode = 0;
    }
    var_Http2AppRes->has_returnCode = FALSE;
    if(TRUE == var_Http2AppRes->has_sessionID){
        var_Http2AppRes->var_sessionID.length = 0;
        var_Http2AppRes->var_sessionID.data = NULL;
    }
    var_Http2AppRes->has_sessionID = FALSE;
    if(TRUE == var_Http2AppRes->has_responseCode){
        var_Http2AppRes->var_responseCode = 0;
    }
    var_Http2AppRes->has_responseCode = FALSE;
    if(TRUE == var_Http2AppRes->has_contentType){
        var_Http2AppRes->var_contentType.length = 0;
        var_Http2AppRes->var_contentType.data = NULL;
    }
    var_Http2AppRes->has_contentType = FALSE;
    if(TRUE == var_Http2AppRes->has_location){
        var_Http2AppRes->var_location.length = 0;
        var_Http2AppRes->var_location.data = NULL;
    }
    var_Http2AppRes->has_location = FALSE;
    if(TRUE == var_Http2AppRes->has_transParam){
        clear_message_Http2TransParam(&(var_Http2AppRes->var_transParam));
    }
    var_Http2AppRes->has_transParam = FALSE;
    for(i=0; i<var_Http2AppRes->var_header.count; ++i){
        clear_message_MapheaderEntry(&(var_Http2AppRes->var_header.item[i]));
    }
    var_Http2AppRes->var_header.count = 0;
    if(TRUE == var_Http2AppRes->has_reqType){
    }
    var_Http2AppRes->has_reqType = FALSE;
}

size_t encode_message_Http2AppRes(const st_http2appres* var_Http2AppRes, BYTE* buf){
    size_t encode_buf_len;
    size_t i;
    size_t offset = 0;

    if(var_Http2AppRes->has_url){
        /* tag:1 type:bytes */
        encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppRes->var_url.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppRes->var_url.data, var_Http2AppRes->var_url.length);
        }
        offset += var_Http2AppRes->var_url.length;
    }

    if(var_Http2AppRes->has_method){
        /* tag:2 type:bytes */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppRes->var_method.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppRes->var_method.data, var_Http2AppRes->var_method.length);
        }
        offset += var_Http2AppRes->var_method.length;
    }

    if(var_Http2AppRes->has_data){
        /* tag:3 type:bytes */
        encode_tag_byte(buf, 3, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppRes->var_data.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppRes->var_data.data, var_Http2AppRes->var_data.length);
        }
        offset += var_Http2AppRes->var_data.length;
    }

    if(var_Http2AppRes->has_returnCode){
        /* tag:4 type:uint32 */
        encode_tag_byte(buf, 4, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_Http2AppRes->var_returnCode, buf, &offset);
    }

    if(var_Http2AppRes->has_sessionID){
        /* tag:5 type:bytes */
        encode_tag_byte(buf, 5, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppRes->var_sessionID.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppRes->var_sessionID.data, var_Http2AppRes->var_sessionID.length);
        }
        offset += var_Http2AppRes->var_sessionID.length;
    }

    if(var_Http2AppRes->has_responseCode){
        /* tag:6 type:uint32 */
        encode_tag_byte(buf, 6, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_Http2AppRes->var_responseCode, buf, &offset);
    }

    if(var_Http2AppRes->has_contentType){
        /* tag:7 type:bytes */
        encode_tag_byte(buf, 7, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppRes->var_contentType.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppRes->var_contentType.data, var_Http2AppRes->var_contentType.length);
        }
        offset += var_Http2AppRes->var_contentType.length;
    }

    if(var_Http2AppRes->has_location){
        /* tag:8 type:bytes */
        encode_tag_byte(buf, 8, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppRes->var_location.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppRes->var_location.data, var_Http2AppRes->var_location.length);
        }
        offset += var_Http2AppRes->var_location.length;
    }

    if(var_Http2AppRes->has_transParam){
        /* tag:9 type:message */
        encode_tag_byte(buf, 9, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_Http2TransParam(&(var_Http2AppRes->var_transParam), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_Http2TransParam(&(var_Http2AppRes->var_transParam), buf + offset);
        }
        offset += encode_buf_len;
    }

    for(i = 0; i < var_Http2AppRes->var_header.count; ++i){
        /* tag:10 type:message */
        encode_tag_byte(buf, 10, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_MapheaderEntry(&(var_Http2AppRes->var_header.item[i]), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_MapheaderEntry(&(var_Http2AppRes->var_header.item[i]), buf + offset);
        }
        offset += encode_buf_len;
    }

    if(var_Http2AppRes->has_reqType){
        /* tag:21 type:enum */
        encode_tag_byte(buf, 21, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_Http2AppRes->var_reqType, buf, &offset);
    }
    return offset;
}

BOOL decode_message_Http2AppRes(BYTE* buf, const size_t buf_len, st_http2appres* var_Http2AppRes){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_Http2AppRes(var_Http2AppRes);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:bytes */
        case 1:
            decode_varint(buf + offset, &(var_Http2AppRes->var_url.length), &offset);
            var_Http2AppRes->var_url.data = buf + offset;
            offset += var_Http2AppRes->var_url.length;
            var_Http2AppRes->has_url = TRUE;
            break;
        /* type:bytes */
        case 2:
            decode_varint(buf + offset, &(var_Http2AppRes->var_method.length), &offset);
            var_Http2AppRes->var_method.data = buf + offset;
            offset += var_Http2AppRes->var_method.length;
            var_Http2AppRes->has_method = TRUE;
            break;
        /* type:bytes */
        case 3:
            decode_varint(buf + offset, &(var_Http2AppRes->var_data.length), &offset);
            var_Http2AppRes->var_data.data = buf + offset;
            offset += var_Http2AppRes->var_data.length;
            var_Http2AppRes->has_data = TRUE;
            break;
        /* type:uint32 */
        case 4:
            decode_varint(buf + offset, &(var_Http2AppRes->var_returnCode), &offset);
            var_Http2AppRes->has_returnCode = TRUE;
            break;
        /* type:bytes */
        case 5:
            decode_varint(buf + offset, &(var_Http2AppRes->var_sessionID.length), &offset);
            var_Http2AppRes->var_sessionID.data = buf + offset;
            offset += var_Http2AppRes->var_sessionID.length;
            var_Http2AppRes->has_sessionID = TRUE;
            break;
        /* type:uint32 */
        case 6:
            decode_varint(buf + offset, &(var_Http2AppRes->var_responseCode), &offset);
            var_Http2AppRes->has_responseCode = TRUE;
            break;
        /* type:bytes */
        case 7:
            decode_varint(buf + offset, &(var_Http2AppRes->var_contentType.length), &offset);
            var_Http2AppRes->var_contentType.data = buf + offset;
            offset += var_Http2AppRes->var_contentType.length;
            var_Http2AppRes->has_contentType = TRUE;
            break;
        /* type:bytes */
        case 8:
            decode_varint(buf + offset, &(var_Http2AppRes->var_location.length), &offset);
            var_Http2AppRes->var_location.data = buf + offset;
            offset += var_Http2AppRes->var_location.length;
            var_Http2AppRes->has_location = TRUE;
            break;
        /* type:message */
        case 9:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_Http2TransParam(buf + offset, tmp_field_len, &(var_Http2AppRes->var_transParam));
            offset += tmp_field_len;
            var_Http2AppRes->has_transParam = TRUE;
            break;
        /* type:message */
        case 10:
            if(var_Http2AppRes->var_header.count >= PBSTRU_MAX_HEADER_IN_HTTP2APPRES) {
                return FALSE;  /* Êý×é³¬ÏÞ */
            }
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_MapheaderEntry(buf + offset, tmp_field_len, &(var_Http2AppRes->var_header.item[var_Http2AppRes->var_header.count]));
            offset += tmp_field_len;
            var_Http2AppRes->var_header.count += 1;
            break;
        /* type:enum */
        case 21:
            decode_varint(buf + offset, &(var_Http2AppRes->var_reqType), &offset);
            var_Http2AppRes->has_reqType = TRUE;
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

#include "http2appreq.h"

/* lint -save -e701 -e647 */

void constru_message_Http2AppReq(st_http2appreq *var_Http2AppReq){
    var_Http2AppReq->has_url = FALSE;
    var_Http2AppReq->has_method = FALSE;
    var_Http2AppReq->has_data = FALSE;
    var_Http2AppReq->has_sequence = FALSE;
    var_Http2AppReq->has_accept = FALSE;
    var_Http2AppReq->has_contentType = FALSE;
        constru_message_Http2TransParam(&(var_Http2AppReq->var_transParam));
    var_Http2AppReq->has_transParam = FALSE;
    var_Http2AppReq->var_header = NULL;
    var_Http2AppReq->var_header_tail = NULL;

        constru_message_Http2Jid(&(var_Http2AppReq->var_httpJid));
    var_Http2AppReq->has_httpJid = FALSE;
}

void destru_message_Http2AppReq(st_http2appreq* var_Http2AppReq){
    clear_message_Http2AppReq(var_Http2AppReq);
}

void clear_message_Http2AppReq(st_http2appreq* var_Http2AppReq){
    if(TRUE == var_Http2AppReq->has_url){
        var_Http2AppReq->var_url.length = 0;
        var_Http2AppReq->var_url.data = NULL;
    }
    var_Http2AppReq->has_url = FALSE;
    if(TRUE == var_Http2AppReq->has_method){
        var_Http2AppReq->var_method.length = 0;
        var_Http2AppReq->var_method.data = NULL;
    }
    var_Http2AppReq->has_method = FALSE;
    if(TRUE == var_Http2AppReq->has_data){
        var_Http2AppReq->var_data.length = 0;
        var_Http2AppReq->var_data.data = NULL;
    }
    var_Http2AppReq->has_data = FALSE;
    if(TRUE == var_Http2AppReq->has_sequence){
        var_Http2AppReq->var_sequence = 0;
    }
    var_Http2AppReq->has_sequence = FALSE;
    if(TRUE == var_Http2AppReq->has_accept){
        var_Http2AppReq->var_accept.length = 0;
        var_Http2AppReq->var_accept.data = NULL;
    }
    var_Http2AppReq->has_accept = FALSE;
    if(TRUE == var_Http2AppReq->has_contentType){
        var_Http2AppReq->var_contentType.length = 0;
        var_Http2AppReq->var_contentType.data = NULL;
    }
    var_Http2AppReq->has_contentType = FALSE;
    if(TRUE == var_Http2AppReq->has_transParam){
        clear_message_Http2TransParam(&(var_Http2AppReq->var_transParam));
    }
    var_Http2AppReq->has_transParam = FALSE;
    st_header_in_http2appreq_headerentry_list *it1=var_Http2AppReq->var_header;
    for(; NULL!=it1; ) {
        st_header_in_http2appreq_headerentry_list *it_tmp = it1;
        it1=it1->next;
        clear_message_HeaderEntry(&(it_tmp->value));
        pbstru_free((void *)it_tmp);
    }
    var_Http2AppReq->var_header = NULL;
    var_Http2AppReq->var_header_tail = NULL;

    if(TRUE == var_Http2AppReq->has_httpJid){
        clear_message_Http2Jid(&(var_Http2AppReq->var_httpJid));
    }
    var_Http2AppReq->has_httpJid = FALSE;
}

size_t encode_message_Http2AppReq(const st_http2appreq* var_Http2AppReq, BYTE* buf){
    size_t encode_buf_len;
    size_t offset = 0;
    st_header_in_http2appreq_headerentry_list *it1=var_Http2AppReq->var_header;

    if(var_Http2AppReq->has_url){
        /* tag:1 type:bytes */
        encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppReq->var_url.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppReq->var_url.data, var_Http2AppReq->var_url.length);
        }
        offset += var_Http2AppReq->var_url.length;
    }

    if(var_Http2AppReq->has_method){
        /* tag:2 type:bytes */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppReq->var_method.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppReq->var_method.data, var_Http2AppReq->var_method.length);
        }
        offset += var_Http2AppReq->var_method.length;
    }

    if(var_Http2AppReq->has_data){
        /* tag:3 type:bytes */
        encode_tag_byte(buf, 3, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppReq->var_data.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppReq->var_data.data, var_Http2AppReq->var_data.length);
        }
        offset += var_Http2AppReq->var_data.length;
    }

    if(var_Http2AppReq->has_sequence){
        /* tag:4 type:uint64 */
        encode_tag_byte(buf, 4, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_Http2AppReq->var_sequence, buf, &offset);
    }

    if(var_Http2AppReq->has_accept){
        /* tag:5 type:bytes */
        encode_tag_byte(buf, 5, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppReq->var_accept.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppReq->var_accept.data, var_Http2AppReq->var_accept.length);
        }
        offset += var_Http2AppReq->var_accept.length;
    }

    if(var_Http2AppReq->has_contentType){
        /* tag:6 type:bytes */
        encode_tag_byte(buf, 6, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppReq->var_contentType.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppReq->var_contentType.data, var_Http2AppReq->var_contentType.length);
        }
        offset += var_Http2AppReq->var_contentType.length;
    }

    if(var_Http2AppReq->has_transParam){
        /* tag:7 type:message */
        encode_tag_byte(buf, 7, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_Http2TransParam(&(var_Http2AppReq->var_transParam), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_Http2TransParam(&(var_Http2AppReq->var_transParam), buf + offset);
        }
        offset += encode_buf_len;
    }

    for(it1=var_Http2AppReq->var_header; NULL!=it1; it1=it1->next) {
        /* tag:8 type:message */
        encode_tag_byte(buf, 8, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_HeaderEntry(&(it1->value), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_HeaderEntry(&(it1->value), buf + offset);
        }
        offset += encode_buf_len;
    }

    if(var_Http2AppReq->has_httpJid){
        /* tag:9 type:message */
        encode_tag_byte(buf, 9, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_Http2Jid(&(var_Http2AppReq->var_httpJid), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_Http2Jid(&(var_Http2AppReq->var_httpJid), buf + offset);
        }
        offset += encode_buf_len;
    }
    return offset;
}

BOOL decode_message_Http2AppReq(BYTE* buf, const size_t buf_len, st_http2appreq* var_Http2AppReq){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_Http2AppReq(var_Http2AppReq);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:bytes */
        case 1:
            decode_varint(buf + offset, &(var_Http2AppReq->var_url.length), &offset);
            var_Http2AppReq->var_url.data = buf + offset;
            offset += var_Http2AppReq->var_url.length;
            var_Http2AppReq->has_url = TRUE;
            break;
        /* type:bytes */
        case 2:
            decode_varint(buf + offset, &(var_Http2AppReq->var_method.length), &offset);
            var_Http2AppReq->var_method.data = buf + offset;
            offset += var_Http2AppReq->var_method.length;
            var_Http2AppReq->has_method = TRUE;
            break;
        /* type:bytes */
        case 3:
            decode_varint(buf + offset, &(var_Http2AppReq->var_data.length), &offset);
            var_Http2AppReq->var_data.data = buf + offset;
            offset += var_Http2AppReq->var_data.length;
            var_Http2AppReq->has_data = TRUE;
            break;
        /* type:uint64 */
        case 4:
            decode_varint(buf + offset, &(var_Http2AppReq->var_sequence), &offset);
            var_Http2AppReq->has_sequence = TRUE;
            break;
        /* type:bytes */
        case 5:
            decode_varint(buf + offset, &(var_Http2AppReq->var_accept.length), &offset);
            var_Http2AppReq->var_accept.data = buf + offset;
            offset += var_Http2AppReq->var_accept.length;
            var_Http2AppReq->has_accept = TRUE;
            break;
        /* type:bytes */
        case 6:
            decode_varint(buf + offset, &(var_Http2AppReq->var_contentType.length), &offset);
            var_Http2AppReq->var_contentType.data = buf + offset;
            offset += var_Http2AppReq->var_contentType.length;
            var_Http2AppReq->has_contentType = TRUE;
            break;
        /* type:message */
        case 7:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_Http2TransParam(buf + offset, tmp_field_len, &(var_Http2AppReq->var_transParam));
            offset += tmp_field_len;
            var_Http2AppReq->has_transParam = TRUE;
            break;
        /* type:message */
        case 8:
            if(NULL == var_Http2AppReq->var_header){
                var_Http2AppReq->var_header = (st_header_in_http2appreq_headerentry_list *)pbstru_malloc(sizeof(st_header_in_http2appreq_headerentry_list));
                if(NULL == var_Http2AppReq->var_header){
                    return FALSE;
                } else {
                    var_Http2AppReq->var_header_tail = var_Http2AppReq->var_header;
                    var_Http2AppReq->var_header_tail->next = NULL;
                }
            } else {
                st_header_in_http2appreq_headerentry_list *tmptr = (st_header_in_http2appreq_headerentry_list *)pbstru_malloc(sizeof(st_header_in_http2appreq_headerentry_list));
                if(NULL == tmptr){
                    return FALSE;
                } else {
                    var_Http2AppReq->var_header_tail->next = tmptr;
                    var_Http2AppReq->var_header_tail = tmptr;
                    var_Http2AppReq->var_header_tail->next = NULL;
                }
            }
            constru_message_HeaderEntry(&(var_Http2AppReq->var_header_tail->value));
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_HeaderEntry(buf + offset, tmp_field_len, &(var_Http2AppReq->var_header_tail->value));
            offset += tmp_field_len;
            break;
        /* type:message */
        case 9:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_Http2Jid(buf + offset, tmp_field_len, &(var_Http2AppReq->var_httpJid));
            offset += tmp_field_len;
            var_Http2AppReq->has_httpJid = TRUE;
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

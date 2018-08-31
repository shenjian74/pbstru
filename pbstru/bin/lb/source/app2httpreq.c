#include "app2httpreq.h"

/* lint -save -e701 -e647 */

void constru_message_App2HttpReq(st_app2httpreq *var_App2HttpReq){
    var_App2HttpReq->has_url = FALSE;
    var_App2HttpReq->has_method = FALSE;
    var_App2HttpReq->has_data = FALSE;
    var_App2HttpReq->has_instance = FALSE;
    var_App2HttpReq->has_sessionID = FALSE;
    var_App2HttpReq->has_accept = FALSE;
    var_App2HttpReq->has_contentType = FALSE;
    var_App2HttpReq->has_traced = FALSE;
    var_App2HttpReq->var_header = NULL;
    var_App2HttpReq->var_header_tail = NULL;

        constru_message_Http2AppInfo(&(var_App2HttpReq->var_appInfo));
    var_App2HttpReq->has_appInfo = FALSE;
    var_App2HttpReq->has_linkID = FALSE;
    var_App2HttpReq->has_userid = FALSE;
    var_App2HttpReq->has_reqType = FALSE;
}

void destru_message_App2HttpReq(st_app2httpreq* var_App2HttpReq){
    clear_message_App2HttpReq(var_App2HttpReq);
}

void clear_message_App2HttpReq(st_app2httpreq* var_App2HttpReq){
    if(TRUE == var_App2HttpReq->has_url){
        var_App2HttpReq->var_url.length = 0;
        var_App2HttpReq->var_url.data = NULL;
    }
    var_App2HttpReq->has_url = FALSE;
    if(TRUE == var_App2HttpReq->has_method){
        var_App2HttpReq->var_method.length = 0;
        var_App2HttpReq->var_method.data = NULL;
    }
    var_App2HttpReq->has_method = FALSE;
    if(TRUE == var_App2HttpReq->has_data){
        var_App2HttpReq->var_data.length = 0;
        var_App2HttpReq->var_data.data = NULL;
    }
    var_App2HttpReq->has_data = FALSE;
    if(TRUE == var_App2HttpReq->has_instance){
        var_App2HttpReq->var_instance.length = 0;
        var_App2HttpReq->var_instance.data = NULL;
    }
    var_App2HttpReq->has_instance = FALSE;
    if(TRUE == var_App2HttpReq->has_sessionID){
        var_App2HttpReq->var_sessionID.length = 0;
        var_App2HttpReq->var_sessionID.data = NULL;
    }
    var_App2HttpReq->has_sessionID = FALSE;
    if(TRUE == var_App2HttpReq->has_accept){
        var_App2HttpReq->var_accept.length = 0;
        var_App2HttpReq->var_accept.data = NULL;
    }
    var_App2HttpReq->has_accept = FALSE;
    if(TRUE == var_App2HttpReq->has_contentType){
        var_App2HttpReq->var_contentType.length = 0;
        var_App2HttpReq->var_contentType.data = NULL;
    }
    var_App2HttpReq->has_contentType = FALSE;
    if(TRUE == var_App2HttpReq->has_traced){
        var_App2HttpReq->var_traced = FALSE;
    }
    var_App2HttpReq->has_traced = FALSE;
    st_header_in_app2httpreq_headerentry_list *it1=var_App2HttpReq->var_header;
    for(; NULL!=it1; ) {
        st_header_in_app2httpreq_headerentry_list *it_tmp = it1;
        it1=it1->next;
        clear_message_HeaderEntry(&(it_tmp->value));
        pbstru_free((void *)it_tmp);
    }
    var_App2HttpReq->var_header = NULL;
    var_App2HttpReq->var_header_tail = NULL;

    if(TRUE == var_App2HttpReq->has_appInfo){
        clear_message_Http2AppInfo(&(var_App2HttpReq->var_appInfo));
    }
    var_App2HttpReq->has_appInfo = FALSE;
    if(TRUE == var_App2HttpReq->has_linkID){
        var_App2HttpReq->var_linkID = 0;
    }
    var_App2HttpReq->has_linkID = FALSE;
    if(TRUE == var_App2HttpReq->has_userid){
        var_App2HttpReq->var_userid.length = 0;
        var_App2HttpReq->var_userid.data = NULL;
    }
    var_App2HttpReq->has_userid = FALSE;
    if(TRUE == var_App2HttpReq->has_reqType){
    }
    var_App2HttpReq->has_reqType = FALSE;
}

size_t encode_message_App2HttpReq(const st_app2httpreq* var_App2HttpReq, BYTE* buf){
    size_t encode_buf_len;
    size_t offset = 0;
    st_header_in_app2httpreq_headerentry_list *it1=var_App2HttpReq->var_header;

    if(var_App2HttpReq->has_url){
        /* tag:1 type:bytes */
        encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_App2HttpReq->var_url.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_App2HttpReq->var_url.data, var_App2HttpReq->var_url.length);
        }
        offset += var_App2HttpReq->var_url.length;
    }

    if(var_App2HttpReq->has_method){
        /* tag:2 type:bytes */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_App2HttpReq->var_method.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_App2HttpReq->var_method.data, var_App2HttpReq->var_method.length);
        }
        offset += var_App2HttpReq->var_method.length;
    }

    if(var_App2HttpReq->has_data){
        /* tag:3 type:bytes */
        encode_tag_byte(buf, 3, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_App2HttpReq->var_data.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_App2HttpReq->var_data.data, var_App2HttpReq->var_data.length);
        }
        offset += var_App2HttpReq->var_data.length;
    }

    if(var_App2HttpReq->has_instance){
        /* tag:4 type:bytes */
        encode_tag_byte(buf, 4, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_App2HttpReq->var_instance.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_App2HttpReq->var_instance.data, var_App2HttpReq->var_instance.length);
        }
        offset += var_App2HttpReq->var_instance.length;
    }

    if(var_App2HttpReq->has_sessionID){
        /* tag:5 type:bytes */
        encode_tag_byte(buf, 5, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_App2HttpReq->var_sessionID.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_App2HttpReq->var_sessionID.data, var_App2HttpReq->var_sessionID.length);
        }
        offset += var_App2HttpReq->var_sessionID.length;
    }

    if(var_App2HttpReq->has_accept){
        /* tag:6 type:bytes */
        encode_tag_byte(buf, 6, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_App2HttpReq->var_accept.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_App2HttpReq->var_accept.data, var_App2HttpReq->var_accept.length);
        }
        offset += var_App2HttpReq->var_accept.length;
    }

    if(var_App2HttpReq->has_contentType){
        /* tag:7 type:bytes */
        encode_tag_byte(buf, 7, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_App2HttpReq->var_contentType.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_App2HttpReq->var_contentType.data, var_App2HttpReq->var_contentType.length);
        }
        offset += var_App2HttpReq->var_contentType.length;
    }

    if(var_App2HttpReq->has_traced){
        /* tag:8 type:bool */
        encode_tag_byte(buf, 8, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_App2HttpReq->var_traced, buf, &offset);
    }

    for(it1=var_App2HttpReq->var_header; NULL!=it1; it1=it1->next) {
        /* tag:9 type:message */
        encode_tag_byte(buf, 9, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_HeaderEntry(&(it1->value), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_HeaderEntry(&(it1->value), buf + offset);
        }
        offset += encode_buf_len;
    }

    if(var_App2HttpReq->has_appInfo){
        /* tag:10 type:message */
        encode_tag_byte(buf, 10, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_Http2AppInfo(&(var_App2HttpReq->var_appInfo), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_Http2AppInfo(&(var_App2HttpReq->var_appInfo), buf + offset);
        }
        offset += encode_buf_len;
    }

    if(var_App2HttpReq->has_linkID){
        /* tag:11 type:uint32 */
        encode_tag_byte(buf, 11, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_App2HttpReq->var_linkID, buf, &offset);
    }

    if(var_App2HttpReq->has_userid){
        /* tag:12 type:bytes */
        encode_tag_byte(buf, 12, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_App2HttpReq->var_userid.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_App2HttpReq->var_userid.data, var_App2HttpReq->var_userid.length);
        }
        offset += var_App2HttpReq->var_userid.length;
    }

    if(var_App2HttpReq->has_reqType){
        /* tag:21 type:enum */
        encode_tag_byte(buf, 21, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_App2HttpReq->var_reqType, buf, &offset);
    }
    return offset;
}

BOOL decode_message_App2HttpReq(BYTE* buf, const size_t buf_len, st_app2httpreq* var_App2HttpReq){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_App2HttpReq(var_App2HttpReq);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:bytes */
        case 1:
            decode_varint(buf + offset, &(var_App2HttpReq->var_url.length), &offset);
            var_App2HttpReq->var_url.data = buf + offset;
            offset += var_App2HttpReq->var_url.length;
            var_App2HttpReq->has_url = TRUE;
            break;
        /* type:bytes */
        case 2:
            decode_varint(buf + offset, &(var_App2HttpReq->var_method.length), &offset);
            var_App2HttpReq->var_method.data = buf + offset;
            offset += var_App2HttpReq->var_method.length;
            var_App2HttpReq->has_method = TRUE;
            break;
        /* type:bytes */
        case 3:
            decode_varint(buf + offset, &(var_App2HttpReq->var_data.length), &offset);
            var_App2HttpReq->var_data.data = buf + offset;
            offset += var_App2HttpReq->var_data.length;
            var_App2HttpReq->has_data = TRUE;
            break;
        /* type:bytes */
        case 4:
            decode_varint(buf + offset, &(var_App2HttpReq->var_instance.length), &offset);
            var_App2HttpReq->var_instance.data = buf + offset;
            offset += var_App2HttpReq->var_instance.length;
            var_App2HttpReq->has_instance = TRUE;
            break;
        /* type:bytes */
        case 5:
            decode_varint(buf + offset, &(var_App2HttpReq->var_sessionID.length), &offset);
            var_App2HttpReq->var_sessionID.data = buf + offset;
            offset += var_App2HttpReq->var_sessionID.length;
            var_App2HttpReq->has_sessionID = TRUE;
            break;
        /* type:bytes */
        case 6:
            decode_varint(buf + offset, &(var_App2HttpReq->var_accept.length), &offset);
            var_App2HttpReq->var_accept.data = buf + offset;
            offset += var_App2HttpReq->var_accept.length;
            var_App2HttpReq->has_accept = TRUE;
            break;
        /* type:bytes */
        case 7:
            decode_varint(buf + offset, &(var_App2HttpReq->var_contentType.length), &offset);
            var_App2HttpReq->var_contentType.data = buf + offset;
            offset += var_App2HttpReq->var_contentType.length;
            var_App2HttpReq->has_contentType = TRUE;
            break;
        /* type:bool */
        case 8:
            decode_varint(buf + offset, &(var_App2HttpReq->var_traced), &offset);
            var_App2HttpReq->has_traced = TRUE;
            break;
        /* type:message */
        case 9:
            if(NULL == var_App2HttpReq->var_header){
                var_App2HttpReq->var_header = (st_header_in_app2httpreq_headerentry_list *)pbstru_malloc(sizeof(st_header_in_app2httpreq_headerentry_list));
                if(NULL == var_App2HttpReq->var_header){
                    return FALSE;
                } else {
                    var_App2HttpReq->var_header_tail = var_App2HttpReq->var_header;
                    var_App2HttpReq->var_header_tail->next = NULL;
                }
            } else {
                st_header_in_app2httpreq_headerentry_list *tmptr = (st_header_in_app2httpreq_headerentry_list *)pbstru_malloc(sizeof(st_header_in_app2httpreq_headerentry_list));
                if(NULL == tmptr){
                    return FALSE;
                } else {
                    var_App2HttpReq->var_header_tail->next = tmptr;
                    var_App2HttpReq->var_header_tail = tmptr;
                    var_App2HttpReq->var_header_tail->next = NULL;
                }
            }
            constru_message_HeaderEntry(&(var_App2HttpReq->var_header_tail->value));
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_HeaderEntry(buf + offset, tmp_field_len, &(var_App2HttpReq->var_header_tail->value));
            offset += tmp_field_len;
            break;
        /* type:message */
        case 10:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_Http2AppInfo(buf + offset, tmp_field_len, &(var_App2HttpReq->var_appInfo));
            offset += tmp_field_len;
            var_App2HttpReq->has_appInfo = TRUE;
            break;
        /* type:uint32 */
        case 11:
            decode_varint(buf + offset, &(var_App2HttpReq->var_linkID), &offset);
            var_App2HttpReq->has_linkID = TRUE;
            break;
        /* type:bytes */
        case 12:
            decode_varint(buf + offset, &(var_App2HttpReq->var_userid.length), &offset);
            var_App2HttpReq->var_userid.data = buf + offset;
            offset += var_App2HttpReq->var_userid.length;
            var_App2HttpReq->has_userid = TRUE;
            break;
        /* type:enum */
        case 21:
            decode_varint(buf + offset, &(var_App2HttpReq->var_reqType), &offset);
            var_App2HttpReq->has_reqType = TRUE;
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

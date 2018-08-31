#include "http2appsubscribeack.h"

/* lint -save -e701 -e647 */

void constru_message_Http2AppSubscribeAck(st_http2appsubscribeack *var_Http2AppSubscribeAck){
    var_Http2AppSubscribeAck->has_sequence = FALSE;
    var_Http2AppSubscribeAck->has_errorCode = FALSE;
    var_Http2AppSubscribeAck->var_results = NULL;
    var_Http2AppSubscribeAck->var_results_tail = NULL;

}

void destru_message_Http2AppSubscribeAck(st_http2appsubscribeack* var_Http2AppSubscribeAck){
    clear_message_Http2AppSubscribeAck(var_Http2AppSubscribeAck);
}

void clear_message_Http2AppSubscribeAck(st_http2appsubscribeack* var_Http2AppSubscribeAck){
    if(TRUE == var_Http2AppSubscribeAck->has_sequence){
        var_Http2AppSubscribeAck->var_sequence = 0;
    }
    var_Http2AppSubscribeAck->has_sequence = FALSE;
    if(TRUE == var_Http2AppSubscribeAck->has_errorCode){
        var_Http2AppSubscribeAck->var_errorCode = 0;
    }
    var_Http2AppSubscribeAck->has_errorCode = FALSE;
    st_results_in_http2appsubscribeack_loadruleresult_list *it1=var_Http2AppSubscribeAck->var_results;
    for(; NULL!=it1; ) {
        st_results_in_http2appsubscribeack_loadruleresult_list *it_tmp = it1;
        it1=it1->next;
        clear_message_LoadRuleResult(&(it_tmp->value));
        pbstru_free((void *)it_tmp);
    }
    var_Http2AppSubscribeAck->var_results = NULL;
    var_Http2AppSubscribeAck->var_results_tail = NULL;

}

size_t encode_message_Http2AppSubscribeAck(const st_http2appsubscribeack* var_Http2AppSubscribeAck, BYTE* buf){
    size_t encode_buf_len;
    size_t offset = 0;
    st_results_in_http2appsubscribeack_loadruleresult_list *it1=var_Http2AppSubscribeAck->var_results;

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

    for(it1=var_Http2AppSubscribeAck->var_results; NULL!=it1; it1=it1->next) {
        /* tag:3 type:message */
        encode_tag_byte(buf, 3, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_LoadRuleResult(&(it1->value), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_LoadRuleResult(&(it1->value), buf + offset);
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
            if(NULL == var_Http2AppSubscribeAck->var_results){
                var_Http2AppSubscribeAck->var_results = (st_results_in_http2appsubscribeack_loadruleresult_list *)pbstru_malloc(sizeof(st_results_in_http2appsubscribeack_loadruleresult_list));
                if(NULL == var_Http2AppSubscribeAck->var_results){
                    return FALSE;
                } else {
                    var_Http2AppSubscribeAck->var_results_tail = var_Http2AppSubscribeAck->var_results;
                    var_Http2AppSubscribeAck->var_results_tail->next = NULL;
                }
            } else {
                st_results_in_http2appsubscribeack_loadruleresult_list *tmptr = (st_results_in_http2appsubscribeack_loadruleresult_list *)pbstru_malloc(sizeof(st_results_in_http2appsubscribeack_loadruleresult_list));
                if(NULL == tmptr){
                    return FALSE;
                } else {
                    var_Http2AppSubscribeAck->var_results_tail->next = tmptr;
                    var_Http2AppSubscribeAck->var_results_tail = tmptr;
                    var_Http2AppSubscribeAck->var_results_tail->next = NULL;
                }
            }
            constru_message_LoadRuleResult(&(var_Http2AppSubscribeAck->var_results_tail->value));
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_LoadRuleResult(buf + offset, tmp_field_len, &(var_Http2AppSubscribeAck->var_results_tail->value));
            offset += tmp_field_len;
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

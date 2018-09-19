#include "app2httpsubscribe.h"

/* lint -save -e701 -e647 */

void constru_message_App2HttpSubscribe(st_app2httpsubscribe *var_App2HttpSubscribe){
    size_t i = 0;
    var_App2HttpSubscribe->has_sequence = FALSE;
        constru_message_DiscoverSC(&(var_App2HttpSubscribe->var_scInfo));
    var_App2HttpSubscribe->has_scInfo = FALSE;
        constru_message_Http2AppInfo(&(var_App2HttpSubscribe->var_appInfo));
    var_App2HttpSubscribe->has_appInfo = FALSE;
    for(i=0; i<PBSTRU_MAX_RULES_IN_APP2HTTPSUBSCRIBE; ++i){
        constru_message_DispatchRule(&(var_App2HttpSubscribe->var_rules.item[i]));
    }
    var_App2HttpSubscribe->var_rules.count = 0;
}

void destru_message_App2HttpSubscribe(st_app2httpsubscribe* var_App2HttpSubscribe){
    clear_message_App2HttpSubscribe(var_App2HttpSubscribe);
}

void clear_message_App2HttpSubscribe(st_app2httpsubscribe* var_App2HttpSubscribe){
    size_t i = 0;
    if(TRUE == var_App2HttpSubscribe->has_sequence){
        var_App2HttpSubscribe->var_sequence = 0;
    }
    var_App2HttpSubscribe->has_sequence = FALSE;
    if(TRUE == var_App2HttpSubscribe->has_scInfo){
        clear_message_DiscoverSC(&(var_App2HttpSubscribe->var_scInfo));
    }
    var_App2HttpSubscribe->has_scInfo = FALSE;
    if(TRUE == var_App2HttpSubscribe->has_appInfo){
        clear_message_Http2AppInfo(&(var_App2HttpSubscribe->var_appInfo));
    }
    var_App2HttpSubscribe->has_appInfo = FALSE;
    for(i=0; i<var_App2HttpSubscribe->var_rules.count; ++i){
        clear_message_DispatchRule(&(var_App2HttpSubscribe->var_rules.item[i]));
    }
    var_App2HttpSubscribe->var_rules.count = 0;
}

size_t encode_message_App2HttpSubscribe(const st_app2httpsubscribe* var_App2HttpSubscribe, BYTE* buf){
    size_t encode_buf_len;
    size_t i;
    size_t offset = 0;

    if(var_App2HttpSubscribe->has_sequence){
        /* tag:1 type:uint32 */
        encode_tag_byte(buf, 1, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_App2HttpSubscribe->var_sequence, buf, &offset);
    }

    if(var_App2HttpSubscribe->has_scInfo){
        /* tag:2 type:message */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_DiscoverSC(&(var_App2HttpSubscribe->var_scInfo), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_DiscoverSC(&(var_App2HttpSubscribe->var_scInfo), buf + offset);
        }
        offset += encode_buf_len;
    }

    if(var_App2HttpSubscribe->has_appInfo){
        /* tag:3 type:message */
        encode_tag_byte(buf, 3, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_Http2AppInfo(&(var_App2HttpSubscribe->var_appInfo), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_Http2AppInfo(&(var_App2HttpSubscribe->var_appInfo), buf + offset);
        }
        offset += encode_buf_len;
    }

    for(i = 0; i < var_App2HttpSubscribe->var_rules.count; ++i){
        /* tag:4 type:message */
        encode_tag_byte(buf, 4, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_DispatchRule(&(var_App2HttpSubscribe->var_rules.item[i]), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_DispatchRule(&(var_App2HttpSubscribe->var_rules.item[i]), buf + offset);
        }
        offset += encode_buf_len;
    }
    return offset;
}

BOOL decode_message_App2HttpSubscribe(BYTE* buf, const size_t buf_len, st_app2httpsubscribe* var_App2HttpSubscribe){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_App2HttpSubscribe(var_App2HttpSubscribe);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:uint32 */
        case 1:
            decode_varint(buf + offset, &(var_App2HttpSubscribe->var_sequence), &offset);
            var_App2HttpSubscribe->has_sequence = TRUE;
            break;
        /* type:message */
        case 2:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_DiscoverSC(buf + offset, tmp_field_len, &(var_App2HttpSubscribe->var_scInfo));
            offset += tmp_field_len;
            var_App2HttpSubscribe->has_scInfo = TRUE;
            break;
        /* type:message */
        case 3:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_Http2AppInfo(buf + offset, tmp_field_len, &(var_App2HttpSubscribe->var_appInfo));
            offset += tmp_field_len;
            var_App2HttpSubscribe->has_appInfo = TRUE;
            break;
        /* type:message */
        case 4:
            if(var_App2HttpSubscribe->var_rules.count >= PBSTRU_MAX_RULES_IN_APP2HTTPSUBSCRIBE) {
                return FALSE;  /* Êý×é³¬ÏÞ */
            }
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_DispatchRule(buf + offset, tmp_field_len, &(var_App2HttpSubscribe->var_rules.item[var_App2HttpSubscribe->var_rules.count]));
            offset += tmp_field_len;
            var_App2HttpSubscribe->var_rules.count += 1;
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


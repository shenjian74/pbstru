#include "dispatchrule.h"

/* lint -save -e701 -e647 */

void constru_message_DispatchRule(st_dispatchrule *var_DispatchRule){
    var_DispatchRule->has_serviceName = FALSE;
        constru_message_DispatchData(&(var_DispatchRule->var_serviceData));
    var_DispatchRule->has_serviceData = FALSE;
}

void destru_message_DispatchRule(st_dispatchrule* var_DispatchRule){
    clear_message_DispatchRule(var_DispatchRule);
}

void clear_message_DispatchRule(st_dispatchrule* var_DispatchRule){
    if(TRUE == var_DispatchRule->has_serviceName){
        var_DispatchRule->var_serviceName.length = 0;
        var_DispatchRule->var_serviceName.data = NULL;
    }
    var_DispatchRule->has_serviceName = FALSE;
    if(TRUE == var_DispatchRule->has_serviceData){
        clear_message_DispatchData(&(var_DispatchRule->var_serviceData));
    }
    var_DispatchRule->has_serviceData = FALSE;
}

size_t encode_message_DispatchRule(const st_dispatchrule* var_DispatchRule, BYTE* buf){
    size_t encode_buf_len;
    size_t offset = 0;

    if(var_DispatchRule->has_serviceName){
        /* tag:1 type:bytes */
        encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_DispatchRule->var_serviceName.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_DispatchRule->var_serviceName.data, var_DispatchRule->var_serviceName.length);
        }
        offset += var_DispatchRule->var_serviceName.length;
    }

    if(var_DispatchRule->has_serviceData){
        /* tag:2 type:message */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_DispatchData(&(var_DispatchRule->var_serviceData), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_DispatchData(&(var_DispatchRule->var_serviceData), buf + offset);
        }
        offset += encode_buf_len;
    }
    return offset;
}

BOOL decode_message_DispatchRule(BYTE* buf, const size_t buf_len, st_dispatchrule* var_DispatchRule){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_DispatchRule(var_DispatchRule);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:bytes */
        case 1:
            decode_varint(buf + offset, &(var_DispatchRule->var_serviceName.length), &offset);
            var_DispatchRule->var_serviceName.data = buf + offset;
            offset += var_DispatchRule->var_serviceName.length;
            var_DispatchRule->has_serviceName = TRUE;
            break;
        /* type:message */
        case 2:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_DispatchData(buf + offset, tmp_field_len, &(var_DispatchRule->var_serviceData));
            offset += tmp_field_len;
            var_DispatchRule->has_serviceData = TRUE;
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

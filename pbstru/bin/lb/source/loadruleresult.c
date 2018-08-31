#include "loadruleresult.h"

/* lint -save -e701 -e647 */

void constru_message_LoadRuleResult(st_loadruleresult *var_LoadRuleResult){
    var_LoadRuleResult->has_serviceName = FALSE;
    var_LoadRuleResult->has_errorCode = FALSE;
}

void destru_message_LoadRuleResult(st_loadruleresult* var_LoadRuleResult){
    clear_message_LoadRuleResult(var_LoadRuleResult);
}

void clear_message_LoadRuleResult(st_loadruleresult* var_LoadRuleResult){
    if(TRUE == var_LoadRuleResult->has_serviceName){
        var_LoadRuleResult->var_serviceName.length = 0;
        var_LoadRuleResult->var_serviceName.data = NULL;
    }
    var_LoadRuleResult->has_serviceName = FALSE;
    if(TRUE == var_LoadRuleResult->has_errorCode){
        var_LoadRuleResult->var_errorCode = 0;
    }
    var_LoadRuleResult->has_errorCode = FALSE;
}

size_t encode_message_LoadRuleResult(const st_loadruleresult* var_LoadRuleResult, BYTE* buf){
    size_t offset = 0;

    if(var_LoadRuleResult->has_serviceName){
        /* tag:1 type:bytes */
        encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_LoadRuleResult->var_serviceName.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_LoadRuleResult->var_serviceName.data, var_LoadRuleResult->var_serviceName.length);
        }
        offset += var_LoadRuleResult->var_serviceName.length;
    }

    if(var_LoadRuleResult->has_errorCode){
        /* tag:2 type:uint32 */
        encode_tag_byte(buf, 2, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_LoadRuleResult->var_errorCode, buf, &offset);
    }
    return offset;
}

BOOL decode_message_LoadRuleResult(BYTE* buf, const size_t buf_len, st_loadruleresult* var_LoadRuleResult){
	size_t offset = 0;
    WORD field_num;
    BYTE wire_type;

    clear_message_LoadRuleResult(var_LoadRuleResult);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:bytes */
        case 1:
            decode_varint(buf + offset, &(var_LoadRuleResult->var_serviceName.length), &offset);
            var_LoadRuleResult->var_serviceName.data = buf + offset;
            offset += var_LoadRuleResult->var_serviceName.length;
            var_LoadRuleResult->has_serviceName = TRUE;
            break;
        /* type:uint32 */
        case 2:
            decode_varint(buf + offset, &(var_LoadRuleResult->var_errorCode), &offset);
            var_LoadRuleResult->has_errorCode = TRUE;
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

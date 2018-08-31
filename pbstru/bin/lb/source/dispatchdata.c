#include "dispatchdata.h"

/* lint -save -e701 -e647 */

void constru_message_DispatchData(st_dispatchdata *var_DispatchData){
    var_DispatchData->has_soName = FALSE;
    var_DispatchData->has_soData = FALSE;
}

void destru_message_DispatchData(st_dispatchdata* var_DispatchData){
    clear_message_DispatchData(var_DispatchData);
}

void clear_message_DispatchData(st_dispatchdata* var_DispatchData){
    if(TRUE == var_DispatchData->has_soName){
        var_DispatchData->var_soName.length = 0;
        var_DispatchData->var_soName.data = NULL;
    }
    var_DispatchData->has_soName = FALSE;
    if(TRUE == var_DispatchData->has_soData){
        var_DispatchData->var_soData.length = 0;
        var_DispatchData->var_soData.data = NULL;
    }
    var_DispatchData->has_soData = FALSE;
}

size_t encode_message_DispatchData(const st_dispatchdata* var_DispatchData, BYTE* buf){
    size_t offset = 0;

    if(var_DispatchData->has_soName){
        /* tag:1 type:bytes */
        encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_DispatchData->var_soName.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_DispatchData->var_soName.data, var_DispatchData->var_soName.length);
        }
        offset += var_DispatchData->var_soName.length;
    }

    if(var_DispatchData->has_soData){
        /* tag:2 type:bytes */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_DispatchData->var_soData.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_DispatchData->var_soData.data, var_DispatchData->var_soData.length);
        }
        offset += var_DispatchData->var_soData.length;
    }
    return offset;
}

BOOL decode_message_DispatchData(BYTE* buf, const size_t buf_len, st_dispatchdata* var_DispatchData){
	size_t offset = 0;
    WORD field_num;
    BYTE wire_type;

    clear_message_DispatchData(var_DispatchData);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:bytes */
        case 1:
            decode_varint(buf + offset, &(var_DispatchData->var_soName.length), &offset);
            var_DispatchData->var_soName.data = buf + offset;
            offset += var_DispatchData->var_soName.length;
            var_DispatchData->has_soName = TRUE;
            break;
        /* type:bytes */
        case 2:
            decode_varint(buf + offset, &(var_DispatchData->var_soData.length), &offset);
            var_DispatchData->var_soData.data = buf + offset;
            offset += var_DispatchData->var_soData.length;
            var_DispatchData->has_soData = TRUE;
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

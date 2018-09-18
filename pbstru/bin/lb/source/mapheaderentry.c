#include "mapheaderentry.h"

/* lint -save -e701 -e647 */

void constru_message_MapheaderEntry(st_mapheaderentry *var_MapheaderEntry){
    var_MapheaderEntry->has_key = FALSE;
    var_MapheaderEntry->has_value = FALSE;
}

void destru_message_MapheaderEntry(st_mapheaderentry* var_MapheaderEntry){
    clear_message_MapheaderEntry(var_MapheaderEntry);
}

void clear_message_MapheaderEntry(st_mapheaderentry* var_MapheaderEntry){
    if(TRUE == var_MapheaderEntry->has_key){
        var_MapheaderEntry->var_key.length = 0;
        var_MapheaderEntry->var_key.data = NULL;
    }
    var_MapheaderEntry->has_key = FALSE;
    if(TRUE == var_MapheaderEntry->has_value){
        var_MapheaderEntry->var_value.length = 0;
        var_MapheaderEntry->var_value.data = NULL;
    }
    var_MapheaderEntry->has_value = FALSE;
}

size_t encode_message_MapheaderEntry(const st_mapheaderentry* var_MapheaderEntry, BYTE* buf){
    size_t offset = 0;

    if(var_MapheaderEntry->has_key){
        /* tag:1 type:string */
        encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_MapheaderEntry->var_key.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, (unsigned char *)var_MapheaderEntry->var_key.data, var_MapheaderEntry->var_key.length);
        }
        offset += var_MapheaderEntry->var_key.length;
    }

    if(var_MapheaderEntry->has_value){
        /* tag:2 type:bytes */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_MapheaderEntry->var_value.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_MapheaderEntry->var_value.data, var_MapheaderEntry->var_value.length);
        }
        offset += var_MapheaderEntry->var_value.length;
    }
    return offset;
}

BOOL decode_message_MapheaderEntry(BYTE* buf, const size_t buf_len, st_mapheaderentry* var_MapheaderEntry){
	size_t offset = 0;
    WORD field_num;
    BYTE wire_type;

    clear_message_MapheaderEntry(var_MapheaderEntry);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:string */
        case 1:
            decode_varint(buf + offset, &(var_MapheaderEntry->var_key.length), &offset);
            var_MapheaderEntry->var_key.data = (char *)(buf + offset);
            offset += var_MapheaderEntry->var_key.length;
            var_MapheaderEntry->has_key = TRUE;
            break;
        /* type:bytes */
        case 2:
            decode_varint(buf + offset, &(var_MapheaderEntry->var_value.length), &offset);
            var_MapheaderEntry->var_value.data = buf + offset;
            offset += var_MapheaderEntry->var_value.length;
            var_MapheaderEntry->has_value = TRUE;
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


#include "http2appinfo.h"

/* lint -save -e701 -e647 */

void constru_message_Http2AppInfo(st_http2appinfo *var_Http2AppInfo){
    var_Http2AppInfo->has_serviceAbility = FALSE;
    var_Http2AppInfo->has_serviceTypeName = FALSE;
    var_Http2AppInfo->has_serviceInst = FALSE;
    var_Http2AppInfo->has_scTypeName = FALSE;
    var_Http2AppInfo->has_userGroupId = FALSE;
        constru_message_Http2Jid(&(var_Http2AppInfo->var_jid));
    var_Http2AppInfo->has_jid = FALSE;
}

void destru_message_Http2AppInfo(st_http2appinfo* var_Http2AppInfo){
    clear_message_Http2AppInfo(var_Http2AppInfo);
}

void clear_message_Http2AppInfo(st_http2appinfo* var_Http2AppInfo){
    if(TRUE == var_Http2AppInfo->has_serviceAbility){
        var_Http2AppInfo->var_serviceAbility.length = 0;
        var_Http2AppInfo->var_serviceAbility.data = NULL;
    }
    var_Http2AppInfo->has_serviceAbility = FALSE;
    if(TRUE == var_Http2AppInfo->has_serviceTypeName){
        var_Http2AppInfo->var_serviceTypeName.length = 0;
        var_Http2AppInfo->var_serviceTypeName.data = NULL;
    }
    var_Http2AppInfo->has_serviceTypeName = FALSE;
    if(TRUE == var_Http2AppInfo->has_serviceInst){
        var_Http2AppInfo->var_serviceInst = 0;
    }
    var_Http2AppInfo->has_serviceInst = FALSE;
    if(TRUE == var_Http2AppInfo->has_scTypeName){
        var_Http2AppInfo->var_scTypeName.length = 0;
        var_Http2AppInfo->var_scTypeName.data = NULL;
    }
    var_Http2AppInfo->has_scTypeName = FALSE;
    if(TRUE == var_Http2AppInfo->has_userGroupId){
        var_Http2AppInfo->var_userGroupId = 0;
    }
    var_Http2AppInfo->has_userGroupId = FALSE;
    if(TRUE == var_Http2AppInfo->has_jid){
        clear_message_Http2Jid(&(var_Http2AppInfo->var_jid));
    }
    var_Http2AppInfo->has_jid = FALSE;
}

size_t encode_message_Http2AppInfo(const st_http2appinfo* var_Http2AppInfo, BYTE* buf){
    size_t encode_buf_len;
    size_t offset = 0;

    if(var_Http2AppInfo->has_serviceAbility){
        /* tag:1 type:bytes */
        encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppInfo->var_serviceAbility.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppInfo->var_serviceAbility.data, var_Http2AppInfo->var_serviceAbility.length);
        }
        offset += var_Http2AppInfo->var_serviceAbility.length;
    }

    if(var_Http2AppInfo->has_serviceTypeName){
        /* tag:2 type:bytes */
        encode_tag_byte(buf, 2, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppInfo->var_serviceTypeName.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppInfo->var_serviceTypeName.data, var_Http2AppInfo->var_serviceTypeName.length);
        }
        offset += var_Http2AppInfo->var_serviceTypeName.length;
    }

    if(var_Http2AppInfo->has_serviceInst){
        /* tag:3 type:uint32 */
        encode_tag_byte(buf, 3, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_Http2AppInfo->var_serviceInst, buf, &offset);
    }

    if(var_Http2AppInfo->has_scTypeName){
        /* tag:4 type:bytes */
        encode_tag_byte(buf, 4, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_varint(var_Http2AppInfo->var_scTypeName.length, buf, &offset);
        if (NULL != buf) {
            memcpy(buf + offset, var_Http2AppInfo->var_scTypeName.data, var_Http2AppInfo->var_scTypeName.length);
        }
        offset += var_Http2AppInfo->var_scTypeName.length;
    }

    if(var_Http2AppInfo->has_userGroupId){
        /* tag:5 type:uint32 */
        encode_tag_byte(buf, 5, WIRE_TYPE_VARINT, &offset);
        encode_varint(var_Http2AppInfo->var_userGroupId, buf, &offset);
    }

    if(var_Http2AppInfo->has_jid){
        /* tag:6 type:message */
        encode_tag_byte(buf, 6, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_Http2Jid(&(var_Http2AppInfo->var_jid), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_Http2Jid(&(var_Http2AppInfo->var_jid), buf + offset);
        }
        offset += encode_buf_len;
    }
    return offset;
}

BOOL decode_message_Http2AppInfo(BYTE* buf, const size_t buf_len, st_http2appinfo* var_Http2AppInfo){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_Http2AppInfo(var_Http2AppInfo);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:bytes */
        case 1:
            decode_varint(buf + offset, &(var_Http2AppInfo->var_serviceAbility.length), &offset);
            var_Http2AppInfo->var_serviceAbility.data = buf + offset;
            offset += var_Http2AppInfo->var_serviceAbility.length;
            var_Http2AppInfo->has_serviceAbility = TRUE;
            break;
        /* type:bytes */
        case 2:
            decode_varint(buf + offset, &(var_Http2AppInfo->var_serviceTypeName.length), &offset);
            var_Http2AppInfo->var_serviceTypeName.data = buf + offset;
            offset += var_Http2AppInfo->var_serviceTypeName.length;
            var_Http2AppInfo->has_serviceTypeName = TRUE;
            break;
        /* type:uint32 */
        case 3:
            decode_varint(buf + offset, &(var_Http2AppInfo->var_serviceInst), &offset);
            var_Http2AppInfo->has_serviceInst = TRUE;
            break;
        /* type:bytes */
        case 4:
            decode_varint(buf + offset, &(var_Http2AppInfo->var_scTypeName.length), &offset);
            var_Http2AppInfo->var_scTypeName.data = buf + offset;
            offset += var_Http2AppInfo->var_scTypeName.length;
            var_Http2AppInfo->has_scTypeName = TRUE;
            break;
        /* type:uint32 */
        case 5:
            decode_varint(buf + offset, &(var_Http2AppInfo->var_userGroupId), &offset);
            var_Http2AppInfo->has_userGroupId = TRUE;
            break;
        /* type:message */
        case 6:
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_Http2Jid(buf + offset, tmp_field_len, &(var_Http2AppInfo->var_jid));
            offset += tmp_field_len;
            var_Http2AppInfo->has_jid = TRUE;
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

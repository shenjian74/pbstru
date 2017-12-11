#include "rmnotifynfgeneral.h"

/* lint -save -e701 -e647 */

void constru_message_RMNotifyNfGeneral(st_rmnotifynfgeneral *var_RMNotifyNfGeneral){
    size_t i = 0;
    for(i=0; i<PBSTRU_MAX_NOTIFY_IN_RMNOTIFYNFGENERAL; ++i){
        constru_message_RMNotify(&(var_RMNotifyNfGeneral->var_notify.item[i]));
    }
    var_RMNotifyNfGeneral->var_notify.count = 0;
}

void destru_message_RMNotifyNfGeneral(st_rmnotifynfgeneral* var_RMNotifyNfGeneral){
    clear_message_RMNotifyNfGeneral(var_RMNotifyNfGeneral);
}

void clear_message_RMNotifyNfGeneral(st_rmnotifynfgeneral* var_RMNotifyNfGeneral){
    size_t i = 0;
    for(i=0; i<var_RMNotifyNfGeneral->var_notify.count; ++i){
        clear_message_RMNotify(&(var_RMNotifyNfGeneral->var_notify.item[i]));
    }
    var_RMNotifyNfGeneral->var_notify.count = 0;
}

size_t encode_message_RMNotifyNfGeneral(const st_rmnotifynfgeneral* var_RMNotifyNfGeneral, BYTE* buf){
    size_t encode_buf_len;
    size_t i;
    size_t offset = 0;

    for(i = 0; i < var_RMNotifyNfGeneral->var_notify.count; ++i){
        /* tag:1 type:message */
        encode_tag_byte(buf, 1, WIRE_TYPE_LENGTH_DELIMITED, &offset);
        encode_buf_len = encode_message_RMNotify(&(var_RMNotifyNfGeneral->var_notify.item[i]), NULL);
        encode_varint(encode_buf_len, buf, &offset);
        if(NULL != buf){
            encode_message_RMNotify(&(var_RMNotifyNfGeneral->var_notify.item[i]), buf + offset);
        }
        offset += encode_buf_len;
    }
    return offset;
}

BOOL decode_message_RMNotifyNfGeneral(BYTE* buf, const size_t buf_len, st_rmnotifynfgeneral* var_RMNotifyNfGeneral){
	size_t offset = 0;
	size_t tmp_field_len;
    WORD field_num;
    BYTE wire_type;

    clear_message_RMNotifyNfGeneral(var_RMNotifyNfGeneral);

    for(;offset < buf_len;){
        parse_tag_byte(buf + offset, &field_num, &wire_type, &offset);
        switch(field_num){
        /* type:message */
        case 1:
            if(var_RMNotifyNfGeneral->var_notify.count >= PBSTRU_MAX_NOTIFY_IN_RMNOTIFYNFGENERAL) {
                return FALSE;  /* Êý×é³¬ÏÞ */
            }
            decode_varint(buf + offset, &tmp_field_len, &offset);
            decode_message_RMNotify(buf + offset, tmp_field_len, &(var_RMNotifyNfGeneral->var_notify.item[var_RMNotifyNfGeneral->var_notify.count]));
            offset += tmp_field_len;
            var_RMNotifyNfGeneral->var_notify.count += 1;
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

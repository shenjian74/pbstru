/* This file is generated by [pbstru](https://gitlab.zte.com.cn/10016632/pbstru) */
/* version: 3.1-140-fa8a76a */
/* DO NOT modify it manually. */

#include "pbstru_comm.h"

/* 对tag信息进行解码 */
BOOL TEST_parse_tag_byte_191120085222(const BYTE *buf, const size_t buflen, WORD *field_num, BYTE *wire_type, size_t *offset) {
    if(buf[0] & 0x80) {
        if(buflen<2) {
            return FALSE;
        }
        *field_num = (buf[0] & 0x7F) + (buf[1] >> 3) * 128;
        *wire_type = buf[1] & 0x07;
        *offset += 2;
    } else {
        if(buflen<1) {
            return FALSE;
        }
        *field_num = buf[0] >> 3;
        *wire_type = buf[0] & 0x07;
        *offset += 1;
    }
    return TRUE;
}

void TEST_encode_tag_byte_191120085222(BYTE *buf, const BYTE tag, const BYTE wire_type, size_t *offset) {
    if(tag < 16) {
        if (NULL != buf) {
            buf[*offset] = (BYTE)(tag << 3) | wire_type;
        }
        *offset += 1;
    } else {
        if (NULL != buf) {
            buf[*offset] = tag | 0x80;
            buf[*offset+1] = ((BYTE)(tag/16) & 0x78) | wire_type;
        }
        *offset += 2;
    }
}

/* end of file */
